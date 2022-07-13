<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * net-sysfs.c - network device class and attributes
 *
 * Copyright (c) 2003 Stephen Hemminger <shemminger@osdl.org>
<<<<<<< HEAD
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/capability.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/if_arp.h>
#include <linux/slab.h>
<<<<<<< HEAD
=======
#include <linux/sched/signal.h>
#include <linux/sched/isolation.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/nsproxy.h>
#include <net/sock.h>
#include <net/net_namespace.h>
#include <linux/rtnetlink.h>
<<<<<<< HEAD
#include <linux/wireless.h>
#include <linux/vmalloc.h>
#include <linux/export.h>
#include <linux/jiffies.h>
#include <net/wext.h>

=======
#include <linux/vmalloc.h>
#include <linux/export.h>
#include <linux/jiffies.h>
#include <linux/pm_runtime.h>
#include <linux/of.h>
#include <linux/of_net.h>
#include <linux/cpu.h>
#include <net/netdev_rx_queue.h>
#include <net/rps.h>

#include "dev.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "net-sysfs.h"

#ifdef CONFIG_SYSFS
static const char fmt_hex[] = "%#x\n";
<<<<<<< HEAD
static const char fmt_long_hex[] = "%#lx\n";
static const char fmt_dec[] = "%d\n";
static const char fmt_udec[] = "%u\n";
static const char fmt_ulong[] = "%lu\n";
static const char fmt_u64[] = "%llu\n";

static inline int dev_isalive(const struct net_device *dev)
{
	return dev->reg_state <= NETREG_REGISTERED;
=======
static const char fmt_dec[] = "%d\n";
static const char fmt_ulong[] = "%lu\n";
static const char fmt_u64[] = "%llu\n";

/* Caller holds RTNL or RCU */
static inline int dev_isalive(const struct net_device *dev)
{
	return READ_ONCE(dev->reg_state) <= NETREG_REGISTERED;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* use same locking rules as GIF* ioctl's */
static ssize_t netdev_show(const struct device *dev,
			   struct device_attribute *attr, char *buf,
			   ssize_t (*format)(const struct net_device *, char *))
{
<<<<<<< HEAD
	struct net_device *net = to_net_dev(dev);
	ssize_t ret = -EINVAL;

	read_lock(&dev_base_lock);
	if (dev_isalive(net))
		ret = (*format)(net, buf);
	read_unlock(&dev_base_lock);
=======
	struct net_device *ndev = to_net_dev(dev);
	ssize_t ret = -EINVAL;

	rcu_read_lock();
	if (dev_isalive(ndev))
		ret = (*format)(ndev, buf);
	rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ret;
}

/* generate a show function for simple field */
#define NETDEVICE_SHOW(field, format_string)				\
<<<<<<< HEAD
static ssize_t format_##field(const struct net_device *net, char *buf)	\
{									\
	return sprintf(buf, format_string, net->field);			\
}									\
static ssize_t show_##field(struct device *dev,				\
			    struct device_attribute *attr, char *buf)	\
{									\
	return netdev_show(dev, attr, buf, format_##field);		\
}

=======
static ssize_t format_##field(const struct net_device *dev, char *buf)	\
{									\
	return sysfs_emit(buf, format_string, READ_ONCE(dev->field));		\
}									\
static ssize_t field##_show(struct device *dev,				\
			    struct device_attribute *attr, char *buf)	\
{									\
	return netdev_show(dev, attr, buf, format_##field);		\
}									\

#define NETDEVICE_SHOW_RO(field, format_string)				\
NETDEVICE_SHOW(field, format_string);					\
static DEVICE_ATTR_RO(field)

#define NETDEVICE_SHOW_RW(field, format_string)				\
NETDEVICE_SHOW(field, format_string);					\
static DEVICE_ATTR_RW(field)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* use same locking and permission rules as SIF* ioctl's */
static ssize_t netdev_store(struct device *dev, struct device_attribute *attr,
			    const char *buf, size_t len,
			    int (*set)(struct net_device *, unsigned long))
{
<<<<<<< HEAD
	struct net_device *net = to_net_dev(dev);
	char *endp;
	unsigned long new;
	int ret = -EINVAL;

	if (!capable(CAP_NET_ADMIN))
		return -EPERM;

	new = simple_strtoul(buf, &endp, 0);
	if (endp == buf)
=======
	struct net_device *netdev = to_net_dev(dev);
	struct net *net = dev_net(netdev);
	unsigned long new;
	int ret;

	if (!ns_capable(net->user_ns, CAP_NET_ADMIN))
		return -EPERM;

	ret = kstrtoul(buf, 0, &new);
	if (ret)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto err;

	if (!rtnl_trylock())
		return restart_syscall();

<<<<<<< HEAD
	if (dev_isalive(net)) {
		if ((ret = (*set)(net, new)) == 0)
=======
	if (dev_isalive(netdev)) {
		ret = (*set)(netdev, new);
		if (ret == 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			ret = len;
	}
	rtnl_unlock();
 err:
	return ret;
}

<<<<<<< HEAD
NETDEVICE_SHOW(dev_id, fmt_hex);
NETDEVICE_SHOW(addr_assign_type, fmt_dec);
NETDEVICE_SHOW(addr_len, fmt_dec);
NETDEVICE_SHOW(iflink, fmt_dec);
NETDEVICE_SHOW(ifindex, fmt_dec);
NETDEVICE_SHOW(type, fmt_dec);
NETDEVICE_SHOW(link_mode, fmt_dec);

/* use same locking rules as GIFHWADDR ioctl's */
static ssize_t show_address(struct device *dev, struct device_attribute *attr,
			    char *buf)
{
	struct net_device *net = to_net_dev(dev);
	ssize_t ret = -EINVAL;

	read_lock(&dev_base_lock);
	if (dev_isalive(net))
		ret = sysfs_format_mac(buf, net->dev_addr, net->addr_len);
	read_unlock(&dev_base_lock);
	return ret;
}

static ssize_t show_broadcast(struct device *dev,
			    struct device_attribute *attr, char *buf)
{
	struct net_device *net = to_net_dev(dev);
	if (dev_isalive(net))
		return sysfs_format_mac(buf, net->broadcast, net->addr_len);
	return -EINVAL;
}

static ssize_t show_carrier(struct device *dev,
			    struct device_attribute *attr, char *buf)
{
	struct net_device *netdev = to_net_dev(dev);
	if (netif_running(netdev)) {
		return sprintf(buf, fmt_dec, !!netif_carrier_ok(netdev));
	}
	return -EINVAL;
}

static ssize_t show_speed(struct device *dev,
=======
NETDEVICE_SHOW_RO(dev_id, fmt_hex);
NETDEVICE_SHOW_RO(dev_port, fmt_dec);
NETDEVICE_SHOW_RO(addr_assign_type, fmt_dec);
NETDEVICE_SHOW_RO(addr_len, fmt_dec);
NETDEVICE_SHOW_RO(ifindex, fmt_dec);
NETDEVICE_SHOW_RO(type, fmt_dec);
NETDEVICE_SHOW_RO(link_mode, fmt_dec);

static ssize_t iflink_show(struct device *dev, struct device_attribute *attr,
			   char *buf)
{
	struct net_device *ndev = to_net_dev(dev);

	return sysfs_emit(buf, fmt_dec, dev_get_iflink(ndev));
}
static DEVICE_ATTR_RO(iflink);

static ssize_t format_name_assign_type(const struct net_device *dev, char *buf)
{
	return sysfs_emit(buf, fmt_dec, READ_ONCE(dev->name_assign_type));
}

static ssize_t name_assign_type_show(struct device *dev,
				     struct device_attribute *attr,
				     char *buf)
{
	struct net_device *ndev = to_net_dev(dev);
	ssize_t ret = -EINVAL;

	if (READ_ONCE(ndev->name_assign_type) != NET_NAME_UNKNOWN)
		ret = netdev_show(dev, attr, buf, format_name_assign_type);

	return ret;
}
static DEVICE_ATTR_RO(name_assign_type);

/* use same locking rules as GIFHWADDR ioctl's (dev_get_mac_address()) */
static ssize_t address_show(struct device *dev, struct device_attribute *attr,
			    char *buf)
{
	struct net_device *ndev = to_net_dev(dev);
	ssize_t ret = -EINVAL;

	down_read(&dev_addr_sem);

	rcu_read_lock();
	if (dev_isalive(ndev))
		ret = sysfs_format_mac(buf, ndev->dev_addr, ndev->addr_len);
	rcu_read_unlock();

	up_read(&dev_addr_sem);
	return ret;
}
static DEVICE_ATTR_RO(address);

static ssize_t broadcast_show(struct device *dev,
			      struct device_attribute *attr, char *buf)
{
	struct net_device *ndev = to_net_dev(dev);
	int ret = -EINVAL;

	rcu_read_lock();
	if (dev_isalive(ndev))
		ret = sysfs_format_mac(buf, ndev->broadcast, ndev->addr_len);
	rcu_read_unlock();
	return ret;
}
static DEVICE_ATTR_RO(broadcast);

static int change_carrier(struct net_device *dev, unsigned long new_carrier)
{
	if (!netif_running(dev))
		return -EINVAL;
	return dev_change_carrier(dev, (bool)new_carrier);
}

static ssize_t carrier_store(struct device *dev, struct device_attribute *attr,
			     const char *buf, size_t len)
{
	struct net_device *netdev = to_net_dev(dev);

	/* The check is also done in change_carrier; this helps returning early
	 * without hitting the trylock/restart in netdev_store.
	 */
	if (!netdev->netdev_ops->ndo_change_carrier)
		return -EOPNOTSUPP;

	return netdev_store(dev, attr, buf, len, change_carrier);
}

static ssize_t carrier_show(struct device *dev,
			    struct device_attribute *attr, char *buf)
{
	struct net_device *netdev = to_net_dev(dev);
	int ret = -EINVAL;

	if (!rtnl_trylock())
		return restart_syscall();

	if (netif_running(netdev)) {
		/* Synchronize carrier state with link watch,
		 * see also rtnl_getlink().
		 */
		linkwatch_sync_dev(netdev);

		ret = sysfs_emit(buf, fmt_dec, !!netif_carrier_ok(netdev));
	}
	rtnl_unlock();

	return ret;
}
static DEVICE_ATTR_RW(carrier);

static ssize_t speed_show(struct device *dev,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			  struct device_attribute *attr, char *buf)
{
	struct net_device *netdev = to_net_dev(dev);
	int ret = -EINVAL;

<<<<<<< HEAD
	if (!rtnl_trylock())
		return restart_syscall();

	if (netif_running(netdev)) {
		struct ethtool_cmd cmd;
		if (!__ethtool_get_settings(netdev, &cmd))
			ret = sprintf(buf, fmt_udec, ethtool_cmd_speed(&cmd));
=======
	/* The check is also done in __ethtool_get_link_ksettings; this helps
	 * returning early without hitting the trylock/restart below.
	 */
	if (!netdev->ethtool_ops->get_link_ksettings)
		return ret;

	if (!rtnl_trylock())
		return restart_syscall();

	if (netif_running(netdev) && netif_device_present(netdev)) {
		struct ethtool_link_ksettings cmd;

		if (!__ethtool_get_link_ksettings(netdev, &cmd))
			ret = sysfs_emit(buf, fmt_dec, cmd.base.speed);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	rtnl_unlock();
	return ret;
}
<<<<<<< HEAD

static ssize_t show_duplex(struct device *dev,
=======
static DEVICE_ATTR_RO(speed);

static ssize_t duplex_show(struct device *dev,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			   struct device_attribute *attr, char *buf)
{
	struct net_device *netdev = to_net_dev(dev);
	int ret = -EINVAL;

<<<<<<< HEAD
=======
	/* The check is also done in __ethtool_get_link_ksettings; this helps
	 * returning early without hitting the trylock/restart below.
	 */
	if (!netdev->ethtool_ops->get_link_ksettings)
		return ret;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!rtnl_trylock())
		return restart_syscall();

	if (netif_running(netdev)) {
<<<<<<< HEAD
		struct ethtool_cmd cmd;
		if (!__ethtool_get_settings(netdev, &cmd))
			ret = sprintf(buf, "%s\n",
				      cmd.duplex ? "full" : "half");
=======
		struct ethtool_link_ksettings cmd;

		if (!__ethtool_get_link_ksettings(netdev, &cmd)) {
			const char *duplex;

			switch (cmd.base.duplex) {
			case DUPLEX_HALF:
				duplex = "half";
				break;
			case DUPLEX_FULL:
				duplex = "full";
				break;
			default:
				duplex = "unknown";
				break;
			}
			ret = sysfs_emit(buf, "%s\n", duplex);
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	rtnl_unlock();
	return ret;
}
<<<<<<< HEAD

static ssize_t show_dormant(struct device *dev,
=======
static DEVICE_ATTR_RO(duplex);

static ssize_t testing_show(struct device *dev,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			    struct device_attribute *attr, char *buf)
{
	struct net_device *netdev = to_net_dev(dev);

	if (netif_running(netdev))
<<<<<<< HEAD
		return sprintf(buf, fmt_dec, !!netif_dormant(netdev));

	return -EINVAL;
}
=======
		return sysfs_emit(buf, fmt_dec, !!netif_testing(netdev));

	return -EINVAL;
}
static DEVICE_ATTR_RO(testing);

static ssize_t dormant_show(struct device *dev,
			    struct device_attribute *attr, char *buf)
{
	struct net_device *netdev = to_net_dev(dev);

	if (netif_running(netdev))
		return sysfs_emit(buf, fmt_dec, !!netif_dormant(netdev));

	return -EINVAL;
}
static DEVICE_ATTR_RO(dormant);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static const char *const operstates[] = {
	"unknown",
	"notpresent", /* currently unused */
	"down",
	"lowerlayerdown",
<<<<<<< HEAD
	"testing", /* currently unused */
=======
	"testing",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	"dormant",
	"up"
};

<<<<<<< HEAD
static ssize_t show_operstate(struct device *dev,
=======
static ssize_t operstate_show(struct device *dev,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			      struct device_attribute *attr, char *buf)
{
	const struct net_device *netdev = to_net_dev(dev);
	unsigned char operstate;

<<<<<<< HEAD
	read_lock(&dev_base_lock);
	operstate = netdev->operstate;
	if (!netif_running(netdev))
		operstate = IF_OPER_DOWN;
	read_unlock(&dev_base_lock);
=======
	operstate = READ_ONCE(netdev->operstate);
	if (!netif_running(netdev))
		operstate = IF_OPER_DOWN;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (operstate >= ARRAY_SIZE(operstates))
		return -EINVAL; /* should not happen */

<<<<<<< HEAD
	return sprintf(buf, "%s\n", operstates[operstate]);
}

/* read-write attributes */
NETDEVICE_SHOW(mtu, fmt_dec);

static int change_mtu(struct net_device *net, unsigned long new_mtu)
{
	return dev_set_mtu(net, (int) new_mtu);
}

static ssize_t store_mtu(struct device *dev, struct device_attribute *attr,
=======
	return sysfs_emit(buf, "%s\n", operstates[operstate]);
}
static DEVICE_ATTR_RO(operstate);

static ssize_t carrier_changes_show(struct device *dev,
				    struct device_attribute *attr,
				    char *buf)
{
	struct net_device *netdev = to_net_dev(dev);

	return sysfs_emit(buf, fmt_dec,
			  atomic_read(&netdev->carrier_up_count) +
			  atomic_read(&netdev->carrier_down_count));
}
static DEVICE_ATTR_RO(carrier_changes);

static ssize_t carrier_up_count_show(struct device *dev,
				     struct device_attribute *attr,
				     char *buf)
{
	struct net_device *netdev = to_net_dev(dev);

	return sysfs_emit(buf, fmt_dec, atomic_read(&netdev->carrier_up_count));
}
static DEVICE_ATTR_RO(carrier_up_count);

static ssize_t carrier_down_count_show(struct device *dev,
				       struct device_attribute *attr,
				       char *buf)
{
	struct net_device *netdev = to_net_dev(dev);

	return sysfs_emit(buf, fmt_dec, atomic_read(&netdev->carrier_down_count));
}
static DEVICE_ATTR_RO(carrier_down_count);

/* read-write attributes */

static int change_mtu(struct net_device *dev, unsigned long new_mtu)
{
	return dev_set_mtu(dev, (int)new_mtu);
}

static ssize_t mtu_store(struct device *dev, struct device_attribute *attr,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			 const char *buf, size_t len)
{
	return netdev_store(dev, attr, buf, len, change_mtu);
}
<<<<<<< HEAD

NETDEVICE_SHOW(flags, fmt_hex);

static int change_flags(struct net_device *net, unsigned long new_flags)
{
	return dev_change_flags(net, (unsigned) new_flags);
}

static ssize_t store_flags(struct device *dev, struct device_attribute *attr,
=======
NETDEVICE_SHOW_RW(mtu, fmt_dec);

static int change_flags(struct net_device *dev, unsigned long new_flags)
{
	return dev_change_flags(dev, (unsigned int)new_flags, NULL);
}

static ssize_t flags_store(struct device *dev, struct device_attribute *attr,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			   const char *buf, size_t len)
{
	return netdev_store(dev, attr, buf, len, change_flags);
}
<<<<<<< HEAD

NETDEVICE_SHOW(tx_queue_len, fmt_ulong);

static int change_tx_queue_len(struct net_device *net, unsigned long new_len)
{
	net->tx_queue_len = new_len;
	return 0;
}

static ssize_t store_tx_queue_len(struct device *dev,
				  struct device_attribute *attr,
				  const char *buf, size_t len)
{
	return netdev_store(dev, attr, buf, len, change_tx_queue_len);
}

static ssize_t store_ifalias(struct device *dev, struct device_attribute *attr,
			     const char *buf, size_t len)
{
	struct net_device *netdev = to_net_dev(dev);
	size_t count = len;
	ssize_t ret;

	if (!capable(CAP_NET_ADMIN))
=======
NETDEVICE_SHOW_RW(flags, fmt_hex);

static ssize_t tx_queue_len_store(struct device *dev,
				  struct device_attribute *attr,
				  const char *buf, size_t len)
{
	if (!capable(CAP_NET_ADMIN))
		return -EPERM;

	return netdev_store(dev, attr, buf, len, dev_change_tx_queue_len);
}
NETDEVICE_SHOW_RW(tx_queue_len, fmt_dec);

static int change_gro_flush_timeout(struct net_device *dev, unsigned long val)
{
	WRITE_ONCE(dev->gro_flush_timeout, val);
	return 0;
}

static ssize_t gro_flush_timeout_store(struct device *dev,
				       struct device_attribute *attr,
				       const char *buf, size_t len)
{
	if (!capable(CAP_NET_ADMIN))
		return -EPERM;

	return netdev_store(dev, attr, buf, len, change_gro_flush_timeout);
}
NETDEVICE_SHOW_RW(gro_flush_timeout, fmt_ulong);

static int change_napi_defer_hard_irqs(struct net_device *dev, unsigned long val)
{
	WRITE_ONCE(dev->napi_defer_hard_irqs, val);
	return 0;
}

static ssize_t napi_defer_hard_irqs_store(struct device *dev,
					  struct device_attribute *attr,
					  const char *buf, size_t len)
{
	if (!capable(CAP_NET_ADMIN))
		return -EPERM;

	return netdev_store(dev, attr, buf, len, change_napi_defer_hard_irqs);
}
NETDEVICE_SHOW_RW(napi_defer_hard_irqs, fmt_dec);

static ssize_t ifalias_store(struct device *dev, struct device_attribute *attr,
			     const char *buf, size_t len)
{
	struct net_device *netdev = to_net_dev(dev);
	struct net *net = dev_net(netdev);
	size_t count = len;
	ssize_t ret = 0;

	if (!ns_capable(net->user_ns, CAP_NET_ADMIN))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EPERM;

	/* ignore trailing newline */
	if (len >  0 && buf[len - 1] == '\n')
		--count;

	if (!rtnl_trylock())
		return restart_syscall();
<<<<<<< HEAD
	ret = dev_set_alias(netdev, buf, count);
	rtnl_unlock();

	return ret < 0 ? ret : len;
}

static ssize_t show_ifalias(struct device *dev,
			    struct device_attribute *attr, char *buf)
{
	const struct net_device *netdev = to_net_dev(dev);
	ssize_t ret = 0;

	if (!rtnl_trylock())
		return restart_syscall();
	if (netdev->ifalias)
		ret = sprintf(buf, "%s\n", netdev->ifalias);
=======

	if (dev_isalive(netdev)) {
		ret = dev_set_alias(netdev, buf, count);
		if (ret < 0)
			goto err;
		ret = len;
		netdev_state_change(netdev);
	}
err:
	rtnl_unlock();

	return ret;
}

static ssize_t ifalias_show(struct device *dev,
			    struct device_attribute *attr, char *buf)
{
	const struct net_device *netdev = to_net_dev(dev);
	char tmp[IFALIASZ];
	ssize_t ret = 0;

	ret = dev_get_alias(netdev, tmp, sizeof(tmp));
	if (ret > 0)
		ret = sysfs_emit(buf, "%s\n", tmp);
	return ret;
}
static DEVICE_ATTR_RW(ifalias);

static int change_group(struct net_device *dev, unsigned long new_group)
{
	dev_set_group(dev, (int)new_group);
	return 0;
}

static ssize_t group_store(struct device *dev, struct device_attribute *attr,
			   const char *buf, size_t len)
{
	return netdev_store(dev, attr, buf, len, change_group);
}
NETDEVICE_SHOW(group, fmt_dec);
static DEVICE_ATTR(netdev_group, 0644, group_show, group_store);

static int change_proto_down(struct net_device *dev, unsigned long proto_down)
{
	return dev_change_proto_down(dev, (bool)proto_down);
}

static ssize_t proto_down_store(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t len)
{
	return netdev_store(dev, attr, buf, len, change_proto_down);
}
NETDEVICE_SHOW_RW(proto_down, fmt_dec);

static ssize_t phys_port_id_show(struct device *dev,
				 struct device_attribute *attr, char *buf)
{
	struct net_device *netdev = to_net_dev(dev);
	ssize_t ret = -EINVAL;

	/* The check is also done in dev_get_phys_port_id; this helps returning
	 * early without hitting the trylock/restart below.
	 */
	if (!netdev->netdev_ops->ndo_get_phys_port_id)
		return -EOPNOTSUPP;

	if (!rtnl_trylock())
		return restart_syscall();

	if (dev_isalive(netdev)) {
		struct netdev_phys_item_id ppid;

		ret = dev_get_phys_port_id(netdev, &ppid);
		if (!ret)
			ret = sysfs_emit(buf, "%*phN\n", ppid.id_len, ppid.id);
	}
	rtnl_unlock();

	return ret;
}
static DEVICE_ATTR_RO(phys_port_id);

static ssize_t phys_port_name_show(struct device *dev,
				   struct device_attribute *attr, char *buf)
{
	struct net_device *netdev = to_net_dev(dev);
	ssize_t ret = -EINVAL;

	/* The checks are also done in dev_get_phys_port_name; this helps
	 * returning early without hitting the trylock/restart below.
	 */
	if (!netdev->netdev_ops->ndo_get_phys_port_name &&
	    !netdev->devlink_port)
		return -EOPNOTSUPP;

	if (!rtnl_trylock())
		return restart_syscall();

	if (dev_isalive(netdev)) {
		char name[IFNAMSIZ];

		ret = dev_get_phys_port_name(netdev, name, sizeof(name));
		if (!ret)
			ret = sysfs_emit(buf, "%s\n", name);
	}
	rtnl_unlock();

	return ret;
}
static DEVICE_ATTR_RO(phys_port_name);

static ssize_t phys_switch_id_show(struct device *dev,
				   struct device_attribute *attr, char *buf)
{
	struct net_device *netdev = to_net_dev(dev);
	ssize_t ret = -EINVAL;

	/* The checks are also done in dev_get_phys_port_name; this helps
	 * returning early without hitting the trylock/restart below. This works
	 * because recurse is false when calling dev_get_port_parent_id.
	 */
	if (!netdev->netdev_ops->ndo_get_port_parent_id &&
	    !netdev->devlink_port)
		return -EOPNOTSUPP;

	if (!rtnl_trylock())
		return restart_syscall();

	if (dev_isalive(netdev)) {
		struct netdev_phys_item_id ppid = { };

		ret = dev_get_port_parent_id(netdev, &ppid, false);
		if (!ret)
			ret = sysfs_emit(buf, "%*phN\n", ppid.id_len, ppid.id);
	}
	rtnl_unlock();

	return ret;
}
static DEVICE_ATTR_RO(phys_switch_id);

static ssize_t threaded_show(struct device *dev,
			     struct device_attribute *attr, char *buf)
{
	struct net_device *netdev = to_net_dev(dev);
	ssize_t ret = -EINVAL;

	if (!rtnl_trylock())
		return restart_syscall();

	if (dev_isalive(netdev))
		ret = sysfs_emit(buf, fmt_dec, netdev->threaded);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rtnl_unlock();
	return ret;
}

<<<<<<< HEAD
NETDEVICE_SHOW(group, fmt_dec);

static int change_group(struct net_device *net, unsigned long new_group)
{
	dev_set_group(net, (int) new_group);
	return 0;
}

static ssize_t store_group(struct device *dev, struct device_attribute *attr,
			 const char *buf, size_t len)
{
	return netdev_store(dev, attr, buf, len, change_group);
}

static struct device_attribute net_class_attributes[] = {
	__ATTR(addr_assign_type, S_IRUGO, show_addr_assign_type, NULL),
	__ATTR(addr_len, S_IRUGO, show_addr_len, NULL),
	__ATTR(dev_id, S_IRUGO, show_dev_id, NULL),
	__ATTR(ifalias, S_IRUGO | S_IWUSR, show_ifalias, store_ifalias),
	__ATTR(iflink, S_IRUGO, show_iflink, NULL),
	__ATTR(ifindex, S_IRUGO, show_ifindex, NULL),
	__ATTR(type, S_IRUGO, show_type, NULL),
	__ATTR(link_mode, S_IRUGO, show_link_mode, NULL),
	__ATTR(address, S_IRUGO, show_address, NULL),
	__ATTR(broadcast, S_IRUGO, show_broadcast, NULL),
	__ATTR(carrier, S_IRUGO, show_carrier, NULL),
	__ATTR(speed, S_IRUGO, show_speed, NULL),
	__ATTR(duplex, S_IRUGO, show_duplex, NULL),
	__ATTR(dormant, S_IRUGO, show_dormant, NULL),
	__ATTR(operstate, S_IRUGO, show_operstate, NULL),
	__ATTR(mtu, S_IRUGO | S_IWUSR, show_mtu, store_mtu),
	__ATTR(flags, S_IRUGO | S_IWUSR, show_flags, store_flags),
	__ATTR(tx_queue_len, S_IRUGO | S_IWUSR, show_tx_queue_len,
	       store_tx_queue_len),
	__ATTR(netdev_group, S_IRUGO | S_IWUSR, show_group, store_group),
	{}
};
=======
static int modify_napi_threaded(struct net_device *dev, unsigned long val)
{
	int ret;

	if (list_empty(&dev->napi_list))
		return -EOPNOTSUPP;

	if (val != 0 && val != 1)
		return -EOPNOTSUPP;

	ret = dev_set_threaded(dev, val);

	return ret;
}

static ssize_t threaded_store(struct device *dev,
			      struct device_attribute *attr,
			      const char *buf, size_t len)
{
	return netdev_store(dev, attr, buf, len, modify_napi_threaded);
}
static DEVICE_ATTR_RW(threaded);

static struct attribute *net_class_attrs[] __ro_after_init = {
	&dev_attr_netdev_group.attr,
	&dev_attr_type.attr,
	&dev_attr_dev_id.attr,
	&dev_attr_dev_port.attr,
	&dev_attr_iflink.attr,
	&dev_attr_ifindex.attr,
	&dev_attr_name_assign_type.attr,
	&dev_attr_addr_assign_type.attr,
	&dev_attr_addr_len.attr,
	&dev_attr_link_mode.attr,
	&dev_attr_address.attr,
	&dev_attr_broadcast.attr,
	&dev_attr_speed.attr,
	&dev_attr_duplex.attr,
	&dev_attr_dormant.attr,
	&dev_attr_testing.attr,
	&dev_attr_operstate.attr,
	&dev_attr_carrier_changes.attr,
	&dev_attr_ifalias.attr,
	&dev_attr_carrier.attr,
	&dev_attr_mtu.attr,
	&dev_attr_flags.attr,
	&dev_attr_tx_queue_len.attr,
	&dev_attr_gro_flush_timeout.attr,
	&dev_attr_napi_defer_hard_irqs.attr,
	&dev_attr_phys_port_id.attr,
	&dev_attr_phys_port_name.attr,
	&dev_attr_phys_switch_id.attr,
	&dev_attr_proto_down.attr,
	&dev_attr_carrier_up_count.attr,
	&dev_attr_carrier_down_count.attr,
	&dev_attr_threaded.attr,
	NULL,
};
ATTRIBUTE_GROUPS(net_class);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Show a given an attribute in the statistics group */
static ssize_t netstat_show(const struct device *d,
			    struct device_attribute *attr, char *buf,
			    unsigned long offset)
{
	struct net_device *dev = to_net_dev(d);
	ssize_t ret = -EINVAL;

	WARN_ON(offset > sizeof(struct rtnl_link_stats64) ||
<<<<<<< HEAD
			offset % sizeof(u64) != 0);

	read_lock(&dev_base_lock);
=======
		offset % sizeof(u64) != 0);

	rcu_read_lock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (dev_isalive(dev)) {
		struct rtnl_link_stats64 temp;
		const struct rtnl_link_stats64 *stats = dev_get_stats(dev, &temp);

<<<<<<< HEAD
		ret = sprintf(buf, fmt_u64, *(u64 *)(((u8 *) stats) + offset));
	}
	read_unlock(&dev_base_lock);
=======
		ret = sysfs_emit(buf, fmt_u64, *(u64 *)(((u8 *)stats) + offset));
	}
	rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

/* generate a read-only statistics attribute */
#define NETSTAT_ENTRY(name)						\
<<<<<<< HEAD
static ssize_t show_##name(struct device *d,				\
			   struct device_attribute *attr, char *buf) 	\
=======
static ssize_t name##_show(struct device *d,				\
			   struct device_attribute *attr, char *buf)	\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{									\
	return netstat_show(d, attr, buf,				\
			    offsetof(struct rtnl_link_stats64, name));	\
}									\
<<<<<<< HEAD
static DEVICE_ATTR(name, S_IRUGO, show_##name, NULL)
=======
static DEVICE_ATTR_RO(name)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

NETSTAT_ENTRY(rx_packets);
NETSTAT_ENTRY(tx_packets);
NETSTAT_ENTRY(rx_bytes);
NETSTAT_ENTRY(tx_bytes);
NETSTAT_ENTRY(rx_errors);
NETSTAT_ENTRY(tx_errors);
NETSTAT_ENTRY(rx_dropped);
NETSTAT_ENTRY(tx_dropped);
NETSTAT_ENTRY(multicast);
NETSTAT_ENTRY(collisions);
NETSTAT_ENTRY(rx_length_errors);
NETSTAT_ENTRY(rx_over_errors);
NETSTAT_ENTRY(rx_crc_errors);
NETSTAT_ENTRY(rx_frame_errors);
NETSTAT_ENTRY(rx_fifo_errors);
NETSTAT_ENTRY(rx_missed_errors);
NETSTAT_ENTRY(tx_aborted_errors);
NETSTAT_ENTRY(tx_carrier_errors);
NETSTAT_ENTRY(tx_fifo_errors);
NETSTAT_ENTRY(tx_heartbeat_errors);
NETSTAT_ENTRY(tx_window_errors);
NETSTAT_ENTRY(rx_compressed);
NETSTAT_ENTRY(tx_compressed);
<<<<<<< HEAD

static struct attribute *netstat_attrs[] = {
=======
NETSTAT_ENTRY(rx_nohandler);

static struct attribute *netstat_attrs[] __ro_after_init = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	&dev_attr_rx_packets.attr,
	&dev_attr_tx_packets.attr,
	&dev_attr_rx_bytes.attr,
	&dev_attr_tx_bytes.attr,
	&dev_attr_rx_errors.attr,
	&dev_attr_tx_errors.attr,
	&dev_attr_rx_dropped.attr,
	&dev_attr_tx_dropped.attr,
	&dev_attr_multicast.attr,
	&dev_attr_collisions.attr,
	&dev_attr_rx_length_errors.attr,
	&dev_attr_rx_over_errors.attr,
	&dev_attr_rx_crc_errors.attr,
	&dev_attr_rx_frame_errors.attr,
	&dev_attr_rx_fifo_errors.attr,
	&dev_attr_rx_missed_errors.attr,
	&dev_attr_tx_aborted_errors.attr,
	&dev_attr_tx_carrier_errors.attr,
	&dev_attr_tx_fifo_errors.attr,
	&dev_attr_tx_heartbeat_errors.attr,
	&dev_attr_tx_window_errors.attr,
	&dev_attr_rx_compressed.attr,
	&dev_attr_tx_compressed.attr,
<<<<<<< HEAD
	NULL
};


static struct attribute_group netstat_group = {
=======
	&dev_attr_rx_nohandler.attr,
	NULL
};

static const struct attribute_group netstat_group = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.name  = "statistics",
	.attrs  = netstat_attrs,
};

<<<<<<< HEAD
#ifdef CONFIG_WIRELESS_EXT_SYSFS
/* helper function that does all the locking etc for wireless stats */
static ssize_t wireless_show(struct device *d, char *buf,
			     ssize_t (*format)(const struct iw_statistics *,
					       char *))
{
	struct net_device *dev = to_net_dev(d);
	const struct iw_statistics *iw;
	ssize_t ret = -EINVAL;

	if (!rtnl_trylock())
		return restart_syscall();
	if (dev_isalive(dev)) {
		iw = get_wireless_stats(dev);
		if (iw)
			ret = (*format)(iw, buf);
	}
	rtnl_unlock();

	return ret;
}

/* show function template for wireless fields */
#define WIRELESS_SHOW(name, field, format_string)			\
static ssize_t format_iw_##name(const struct iw_statistics *iw, char *buf) \
{									\
	return sprintf(buf, format_string, iw->field);			\
}									\
static ssize_t show_iw_##name(struct device *d,				\
			      struct device_attribute *attr, char *buf)	\
{									\
	return wireless_show(d, buf, format_iw_##name);			\
}									\
static DEVICE_ATTR(name, S_IRUGO, show_iw_##name, NULL)

WIRELESS_SHOW(status, status, fmt_hex);
WIRELESS_SHOW(link, qual.qual, fmt_dec);
WIRELESS_SHOW(level, qual.level, fmt_dec);
WIRELESS_SHOW(noise, qual.noise, fmt_dec);
WIRELESS_SHOW(nwid, discard.nwid, fmt_dec);
WIRELESS_SHOW(crypt, discard.code, fmt_dec);
WIRELESS_SHOW(fragment, discard.fragment, fmt_dec);
WIRELESS_SHOW(misc, discard.misc, fmt_dec);
WIRELESS_SHOW(retries, discard.retries, fmt_dec);
WIRELESS_SHOW(beacon, miss.beacon, fmt_dec);

static struct attribute *wireless_attrs[] = {
	&dev_attr_status.attr,
	&dev_attr_link.attr,
	&dev_attr_level.attr,
	&dev_attr_noise.attr,
	&dev_attr_nwid.attr,
	&dev_attr_crypt.attr,
	&dev_attr_fragment.attr,
	&dev_attr_retries.attr,
	&dev_attr_misc.attr,
	&dev_attr_beacon.attr,
	NULL
};

static struct attribute_group wireless_group = {
	.name = "wireless",
	.attrs = wireless_attrs,
};
#endif
#endif /* CONFIG_SYSFS */

#ifdef CONFIG_RPS
/*
 * RX queue sysfs structures and functions.
 */
struct rx_queue_attribute {
	struct attribute attr;
	ssize_t (*show)(struct netdev_rx_queue *queue,
	    struct rx_queue_attribute *attr, char *buf);
	ssize_t (*store)(struct netdev_rx_queue *queue,
	    struct rx_queue_attribute *attr, const char *buf, size_t len);
};
#define to_rx_queue_attr(_attr) container_of(_attr,		\
    struct rx_queue_attribute, attr)
=======
static struct attribute *wireless_attrs[] = {
	NULL
};

static const struct attribute_group wireless_group = {
	.name = "wireless",
	.attrs = wireless_attrs,
};

static bool wireless_group_needed(struct net_device *ndev)
{
#if IS_ENABLED(CONFIG_CFG80211)
	if (ndev->ieee80211_ptr)
		return true;
#endif
#if IS_ENABLED(CONFIG_WIRELESS_EXT)
	if (ndev->wireless_handlers)
		return true;
#endif
	return false;
}

#else /* CONFIG_SYSFS */
#define net_class_groups	NULL
#endif /* CONFIG_SYSFS */

#ifdef CONFIG_SYSFS
#define to_rx_queue_attr(_attr) \
	container_of(_attr, struct rx_queue_attribute, attr)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define to_rx_queue(obj) container_of(obj, struct netdev_rx_queue, kobj)

static ssize_t rx_queue_attr_show(struct kobject *kobj, struct attribute *attr,
				  char *buf)
{
<<<<<<< HEAD
	struct rx_queue_attribute *attribute = to_rx_queue_attr(attr);
=======
	const struct rx_queue_attribute *attribute = to_rx_queue_attr(attr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct netdev_rx_queue *queue = to_rx_queue(kobj);

	if (!attribute->show)
		return -EIO;

<<<<<<< HEAD
	return attribute->show(queue, attribute, buf);
=======
	return attribute->show(queue, buf);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static ssize_t rx_queue_attr_store(struct kobject *kobj, struct attribute *attr,
				   const char *buf, size_t count)
{
<<<<<<< HEAD
	struct rx_queue_attribute *attribute = to_rx_queue_attr(attr);
=======
	const struct rx_queue_attribute *attribute = to_rx_queue_attr(attr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct netdev_rx_queue *queue = to_rx_queue(kobj);

	if (!attribute->store)
		return -EIO;

<<<<<<< HEAD
	return attribute->store(queue, attribute, buf, count);
=======
	return attribute->store(queue, buf, count);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const struct sysfs_ops rx_queue_sysfs_ops = {
	.show = rx_queue_attr_show,
	.store = rx_queue_attr_store,
};

<<<<<<< HEAD
static ssize_t show_rps_map(struct netdev_rx_queue *queue,
			    struct rx_queue_attribute *attribute, char *buf)
{
	struct rps_map *map;
	cpumask_var_t mask;
	size_t len = 0;
	int i;
=======
#ifdef CONFIG_RPS
static ssize_t show_rps_map(struct netdev_rx_queue *queue, char *buf)
{
	struct rps_map *map;
	cpumask_var_t mask;
	int i, len;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!zalloc_cpumask_var(&mask, GFP_KERNEL))
		return -ENOMEM;

	rcu_read_lock();
	map = rcu_dereference(queue->rps_map);
	if (map)
		for (i = 0; i < map->len; i++)
			cpumask_set_cpu(map->cpus[i], mask);

<<<<<<< HEAD
	len += cpumask_scnprintf(buf + len, PAGE_SIZE, mask);
	if (PAGE_SIZE - len < 3) {
		rcu_read_unlock();
		free_cpumask_var(mask);
		return -EINVAL;
	}
	rcu_read_unlock();

	free_cpumask_var(mask);
	len += sprintf(buf + len, "\n");
	return len;
}

static ssize_t store_rps_map(struct netdev_rx_queue *queue,
		      struct rx_queue_attribute *attribute,
		      const char *buf, size_t len)
{
	struct rps_map *old_map, *map;
	cpumask_var_t mask;
	int err, cpu, i;
	static DEFINE_SPINLOCK(rps_map_lock);
=======
	len = sysfs_emit(buf, "%*pb\n", cpumask_pr_args(mask));
	rcu_read_unlock();
	free_cpumask_var(mask);

	return len < PAGE_SIZE ? len : -EINVAL;
}

static int netdev_rx_queue_set_rps_mask(struct netdev_rx_queue *queue,
					cpumask_var_t mask)
{
	static DEFINE_MUTEX(rps_map_mutex);
	struct rps_map *old_map, *map;
	int cpu, i;

	map = kzalloc(max_t(unsigned int,
			    RPS_MAP_SIZE(cpumask_weight(mask)), L1_CACHE_BYTES),
		      GFP_KERNEL);
	if (!map)
		return -ENOMEM;

	i = 0;
	for_each_cpu_and(cpu, mask, cpu_online_mask)
		map->cpus[i++] = cpu;

	if (i) {
		map->len = i;
	} else {
		kfree(map);
		map = NULL;
	}

	mutex_lock(&rps_map_mutex);
	old_map = rcu_dereference_protected(queue->rps_map,
					    mutex_is_locked(&rps_map_mutex));
	rcu_assign_pointer(queue->rps_map, map);

	if (map)
		static_branch_inc(&rps_needed);
	if (old_map)
		static_branch_dec(&rps_needed);

	mutex_unlock(&rps_map_mutex);

	if (old_map)
		kfree_rcu(old_map, rcu);
	return 0;
}

int rps_cpumask_housekeeping(struct cpumask *mask)
{
	if (!cpumask_empty(mask)) {
		cpumask_and(mask, mask, housekeeping_cpumask(HK_TYPE_DOMAIN));
		cpumask_and(mask, mask, housekeeping_cpumask(HK_TYPE_WQ));
		if (cpumask_empty(mask))
			return -EINVAL;
	}
	return 0;
}

static ssize_t store_rps_map(struct netdev_rx_queue *queue,
			     const char *buf, size_t len)
{
	cpumask_var_t mask;
	int err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!capable(CAP_NET_ADMIN))
		return -EPERM;

	if (!alloc_cpumask_var(&mask, GFP_KERNEL))
		return -ENOMEM;

	err = bitmap_parse(buf, len, cpumask_bits(mask), nr_cpumask_bits);
<<<<<<< HEAD
	if (err) {
		free_cpumask_var(mask);
		return err;
	}

	map = kzalloc(max_t(unsigned,
	    RPS_MAP_SIZE(cpumask_weight(mask)), L1_CACHE_BYTES),
	    GFP_KERNEL);
	if (!map) {
		free_cpumask_var(mask);
		return -ENOMEM;
	}

	i = 0;
	for_each_cpu_and(cpu, mask, cpu_online_mask)
		map->cpus[i++] = cpu;

	if (i)
		map->len = i;
	else {
		kfree(map);
		map = NULL;
	}

	spin_lock(&rps_map_lock);
	old_map = rcu_dereference_protected(queue->rps_map,
					    lockdep_is_held(&rps_map_lock));
	rcu_assign_pointer(queue->rps_map, map);
	spin_unlock(&rps_map_lock);

	if (map)
		static_key_slow_inc(&rps_needed);
	if (old_map) {
		kfree_rcu(old_map, rcu);
		static_key_slow_dec(&rps_needed);
	}
	free_cpumask_var(mask);
	return len;
}

static ssize_t show_rps_dev_flow_table_cnt(struct netdev_rx_queue *queue,
					   struct rx_queue_attribute *attr,
=======
	if (err)
		goto out;

	err = rps_cpumask_housekeeping(mask);
	if (err)
		goto out;

	err = netdev_rx_queue_set_rps_mask(queue, mask);

out:
	free_cpumask_var(mask);
	return err ? : len;
}

static ssize_t show_rps_dev_flow_table_cnt(struct netdev_rx_queue *queue,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					   char *buf)
{
	struct rps_dev_flow_table *flow_table;
	unsigned long val = 0;

	rcu_read_lock();
	flow_table = rcu_dereference(queue->rps_flow_table);
	if (flow_table)
		val = (unsigned long)flow_table->mask + 1;
	rcu_read_unlock();

<<<<<<< HEAD
	return sprintf(buf, "%lu\n", val);
}

static void rps_dev_flow_table_release_work(struct work_struct *work)
{
	struct rps_dev_flow_table *table = container_of(work,
	    struct rps_dev_flow_table, free_work);

	vfree(table);
=======
	return sysfs_emit(buf, "%lu\n", val);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void rps_dev_flow_table_release(struct rcu_head *rcu)
{
	struct rps_dev_flow_table *table = container_of(rcu,
	    struct rps_dev_flow_table, rcu);
<<<<<<< HEAD

	INIT_WORK(&table->free_work, rps_dev_flow_table_release_work);
	schedule_work(&table->free_work);
}

static ssize_t store_rps_dev_flow_table_cnt(struct netdev_rx_queue *queue,
				     struct rx_queue_attribute *attr,
				     const char *buf, size_t len)
=======
	vfree(table);
}

static ssize_t store_rps_dev_flow_table_cnt(struct netdev_rx_queue *queue,
					    const char *buf, size_t len)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned long mask, count;
	struct rps_dev_flow_table *table, *old_table;
	static DEFINE_SPINLOCK(rps_dev_flow_lock);
	int rc;

	if (!capable(CAP_NET_ADMIN))
		return -EPERM;

	rc = kstrtoul(buf, 0, &count);
	if (rc < 0)
		return rc;

	if (count) {
		mask = count - 1;
		/* mask = roundup_pow_of_two(count) - 1;
		 * without overflows...
		 */
		while ((mask | (mask >> 1)) != mask)
			mask |= (mask >> 1);
		/* On 64 bit arches, must check mask fits in table->mask (u32),
<<<<<<< HEAD
		 * and on 32bit arches, must check RPS_DEV_FLOW_TABLE_SIZE(mask + 1)
		 * doesnt overflow.
=======
		 * and on 32bit arches, must check
		 * RPS_DEV_FLOW_TABLE_SIZE(mask + 1) doesn't overflow.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		 */
#if BITS_PER_LONG > 32
		if (mask > (unsigned long)(u32)mask)
			return -EINVAL;
#else
		if (mask > (ULONG_MAX - RPS_DEV_FLOW_TABLE_SIZE(1))
				/ sizeof(struct rps_dev_flow)) {
			/* Enforce a limit to prevent overflow */
			return -EINVAL;
		}
#endif
		table = vmalloc(RPS_DEV_FLOW_TABLE_SIZE(mask + 1));
		if (!table)
			return -ENOMEM;

		table->mask = mask;
		for (count = 0; count <= mask; count++)
			table->flows[count].cpu = RPS_NO_CPU;
<<<<<<< HEAD
	} else
		table = NULL;
=======
	} else {
		table = NULL;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spin_lock(&rps_dev_flow_lock);
	old_table = rcu_dereference_protected(queue->rps_flow_table,
					      lockdep_is_held(&rps_dev_flow_lock));
	rcu_assign_pointer(queue->rps_flow_table, table);
	spin_unlock(&rps_dev_flow_lock);

	if (old_table)
		call_rcu(&old_table->rcu, rps_dev_flow_table_release);

	return len;
}

<<<<<<< HEAD
static struct rx_queue_attribute rps_cpus_attribute =
	__ATTR(rps_cpus, S_IRUGO | S_IWUSR, show_rps_map, store_rps_map);


static struct rx_queue_attribute rps_dev_flow_table_cnt_attribute =
	__ATTR(rps_flow_cnt, S_IRUGO | S_IWUSR,
	    show_rps_dev_flow_table_cnt, store_rps_dev_flow_table_cnt);

static struct attribute *rx_queue_default_attrs[] = {
	&rps_cpus_attribute.attr,
	&rps_dev_flow_table_cnt_attribute.attr,
	NULL
};
=======
static struct rx_queue_attribute rps_cpus_attribute __ro_after_init
	= __ATTR(rps_cpus, 0644, show_rps_map, store_rps_map);

static struct rx_queue_attribute rps_dev_flow_table_cnt_attribute __ro_after_init
	= __ATTR(rps_flow_cnt, 0644,
		 show_rps_dev_flow_table_cnt, store_rps_dev_flow_table_cnt);
#endif /* CONFIG_RPS */

static struct attribute *rx_queue_default_attrs[] __ro_after_init = {
#ifdef CONFIG_RPS
	&rps_cpus_attribute.attr,
	&rps_dev_flow_table_cnt_attribute.attr,
#endif
	NULL
};
ATTRIBUTE_GROUPS(rx_queue_default);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static void rx_queue_release(struct kobject *kobj)
{
	struct netdev_rx_queue *queue = to_rx_queue(kobj);
<<<<<<< HEAD
	struct rps_map *map;
	struct rps_dev_flow_table *flow_table;


=======
#ifdef CONFIG_RPS
	struct rps_map *map;
	struct rps_dev_flow_table *flow_table;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	map = rcu_dereference_protected(queue->rps_map, 1);
	if (map) {
		RCU_INIT_POINTER(queue->rps_map, NULL);
		kfree_rcu(map, rcu);
	}

	flow_table = rcu_dereference_protected(queue->rps_flow_table, 1);
	if (flow_table) {
		RCU_INIT_POINTER(queue->rps_flow_table, NULL);
		call_rcu(&flow_table->rcu, rps_dev_flow_table_release);
	}
<<<<<<< HEAD

	memset(kobj, 0, sizeof(*kobj));
	dev_put(queue->dev);
}

static struct kobj_type rx_queue_ktype = {
	.sysfs_ops = &rx_queue_sysfs_ops,
	.release = rx_queue_release,
	.default_attrs = rx_queue_default_attrs,
};

static int rx_queue_add_kobject(struct net_device *net, int index)
{
	struct netdev_rx_queue *queue = net->_rx + index;
	struct kobject *kobj = &queue->kobj;
	int error = 0;

	kobj->kset = net->queues_kset;
	error = kobject_init_and_add(kobj, &rx_queue_ktype, NULL,
	    "rx-%u", index);
	if (error) {
		kobject_put(kobj);
		return error;
	}

	kobject_uevent(kobj, KOBJ_ADD);
	dev_hold(queue->dev);

	return error;
}
#endif /* CONFIG_RPS */

int
net_rx_queue_update_kobjects(struct net_device *net, int old_num, int new_num)
{
#ifdef CONFIG_RPS
	int i;
	int error = 0;

	for (i = old_num; i < new_num; i++) {
		error = rx_queue_add_kobject(net, i);
=======
#endif

	memset(kobj, 0, sizeof(*kobj));
	netdev_put(queue->dev, &queue->dev_tracker);
}

static const void *rx_queue_namespace(const struct kobject *kobj)
{
	struct netdev_rx_queue *queue = to_rx_queue(kobj);
	struct device *dev = &queue->dev->dev;
	const void *ns = NULL;

	if (dev->class && dev->class->ns_type)
		ns = dev->class->namespace(dev);

	return ns;
}

static void rx_queue_get_ownership(const struct kobject *kobj,
				   kuid_t *uid, kgid_t *gid)
{
	const struct net *net = rx_queue_namespace(kobj);

	net_ns_get_ownership(net, uid, gid);
}

static const struct kobj_type rx_queue_ktype = {
	.sysfs_ops = &rx_queue_sysfs_ops,
	.release = rx_queue_release,
	.default_groups = rx_queue_default_groups,
	.namespace = rx_queue_namespace,
	.get_ownership = rx_queue_get_ownership,
};

static int rx_queue_default_mask(struct net_device *dev,
				 struct netdev_rx_queue *queue)
{
#if IS_ENABLED(CONFIG_RPS) && IS_ENABLED(CONFIG_SYSCTL)
	struct cpumask *rps_default_mask = READ_ONCE(dev_net(dev)->core.rps_default_mask);

	if (rps_default_mask && !cpumask_empty(rps_default_mask))
		return netdev_rx_queue_set_rps_mask(queue, rps_default_mask);
#endif
	return 0;
}

static int rx_queue_add_kobject(struct net_device *dev, int index)
{
	struct netdev_rx_queue *queue = dev->_rx + index;
	struct kobject *kobj = &queue->kobj;
	int error = 0;

	/* Kobject_put later will trigger rx_queue_release call which
	 * decreases dev refcount: Take that reference here
	 */
	netdev_hold(queue->dev, &queue->dev_tracker, GFP_KERNEL);

	kobj->kset = dev->queues_kset;
	error = kobject_init_and_add(kobj, &rx_queue_ktype, NULL,
				     "rx-%u", index);
	if (error)
		goto err;

	if (dev->sysfs_rx_queue_group) {
		error = sysfs_create_group(kobj, dev->sysfs_rx_queue_group);
		if (error)
			goto err;
	}

	error = rx_queue_default_mask(dev, queue);
	if (error)
		goto err;

	kobject_uevent(kobj, KOBJ_ADD);

	return error;

err:
	kobject_put(kobj);
	return error;
}

static int rx_queue_change_owner(struct net_device *dev, int index, kuid_t kuid,
				 kgid_t kgid)
{
	struct netdev_rx_queue *queue = dev->_rx + index;
	struct kobject *kobj = &queue->kobj;
	int error;

	error = sysfs_change_owner(kobj, kuid, kgid);
	if (error)
		return error;

	if (dev->sysfs_rx_queue_group)
		error = sysfs_group_change_owner(
			kobj, dev->sysfs_rx_queue_group, kuid, kgid);

	return error;
}
#endif /* CONFIG_SYSFS */

int
net_rx_queue_update_kobjects(struct net_device *dev, int old_num, int new_num)
{
#ifdef CONFIG_SYSFS
	int i;
	int error = 0;

#ifndef CONFIG_RPS
	if (!dev->sysfs_rx_queue_group)
		return 0;
#endif
	for (i = old_num; i < new_num; i++) {
		error = rx_queue_add_kobject(dev, i);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (error) {
			new_num = old_num;
			break;
		}
	}

<<<<<<< HEAD
	while (--i >= new_num)
		kobject_put(&net->_rx[i].kobj);
=======
	while (--i >= new_num) {
		struct kobject *kobj = &dev->_rx[i].kobj;

		if (!refcount_read(&dev_net(dev)->ns.count))
			kobj->uevent_suppress = 1;
		if (dev->sysfs_rx_queue_group)
			sysfs_remove_group(kobj, dev->sysfs_rx_queue_group);
		kobject_put(kobj);
	}

	return error;
#else
	return 0;
#endif
}

static int net_rx_queue_change_owner(struct net_device *dev, int num,
				     kuid_t kuid, kgid_t kgid)
{
#ifdef CONFIG_SYSFS
	int error = 0;
	int i;

#ifndef CONFIG_RPS
	if (!dev->sysfs_rx_queue_group)
		return 0;
#endif
	for (i = 0; i < num; i++) {
		error = rx_queue_change_owner(dev, i, kuid, kgid);
		if (error)
			break;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return error;
#else
	return 0;
#endif
}

#ifdef CONFIG_SYSFS
/*
 * netdev_queue sysfs structures and functions.
 */
struct netdev_queue_attribute {
	struct attribute attr;
<<<<<<< HEAD
	ssize_t (*show)(struct netdev_queue *queue,
	    struct netdev_queue_attribute *attr, char *buf);
	ssize_t (*store)(struct netdev_queue *queue,
	    struct netdev_queue_attribute *attr, const char *buf, size_t len);
};
#define to_netdev_queue_attr(_attr) container_of(_attr,		\
    struct netdev_queue_attribute, attr)
=======
	ssize_t (*show)(struct netdev_queue *queue, char *buf);
	ssize_t (*store)(struct netdev_queue *queue,
			 const char *buf, size_t len);
};
#define to_netdev_queue_attr(_attr) \
	container_of(_attr, struct netdev_queue_attribute, attr)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define to_netdev_queue(obj) container_of(obj, struct netdev_queue, kobj)

static ssize_t netdev_queue_attr_show(struct kobject *kobj,
				      struct attribute *attr, char *buf)
{
<<<<<<< HEAD
	struct netdev_queue_attribute *attribute = to_netdev_queue_attr(attr);
=======
	const struct netdev_queue_attribute *attribute
		= to_netdev_queue_attr(attr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct netdev_queue *queue = to_netdev_queue(kobj);

	if (!attribute->show)
		return -EIO;

<<<<<<< HEAD
	return attribute->show(queue, attribute, buf);
=======
	return attribute->show(queue, buf);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static ssize_t netdev_queue_attr_store(struct kobject *kobj,
				       struct attribute *attr,
				       const char *buf, size_t count)
{
<<<<<<< HEAD
	struct netdev_queue_attribute *attribute = to_netdev_queue_attr(attr);
=======
	const struct netdev_queue_attribute *attribute
		= to_netdev_queue_attr(attr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct netdev_queue *queue = to_netdev_queue(kobj);

	if (!attribute->store)
		return -EIO;

<<<<<<< HEAD
	return attribute->store(queue, attribute, buf, count);
=======
	return attribute->store(queue, buf, count);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const struct sysfs_ops netdev_queue_sysfs_ops = {
	.show = netdev_queue_attr_show,
	.store = netdev_queue_attr_store,
};

<<<<<<< HEAD
static ssize_t show_trans_timeout(struct netdev_queue *queue,
				  struct netdev_queue_attribute *attribute,
				  char *buf)
{
	unsigned long trans_timeout;

	spin_lock_irq(&queue->_xmit_lock);
	trans_timeout = queue->trans_timeout;
	spin_unlock_irq(&queue->_xmit_lock);

	return sprintf(buf, "%lu", trans_timeout);
}

static struct netdev_queue_attribute queue_trans_timeout =
	__ATTR(tx_timeout, S_IRUGO, show_trans_timeout, NULL);
=======
static ssize_t tx_timeout_show(struct netdev_queue *queue, char *buf)
{
	unsigned long trans_timeout = atomic_long_read(&queue->trans_timeout);

	return sysfs_emit(buf, fmt_ulong, trans_timeout);
}

static unsigned int get_netdev_queue_index(struct netdev_queue *queue)
{
	struct net_device *dev = queue->dev;
	unsigned int i;

	i = queue - dev->_tx;
	BUG_ON(i >= dev->num_tx_queues);

	return i;
}

static ssize_t traffic_class_show(struct netdev_queue *queue,
				  char *buf)
{
	struct net_device *dev = queue->dev;
	int num_tc, tc;
	int index;

	if (!netif_is_multiqueue(dev))
		return -ENOENT;

	if (!rtnl_trylock())
		return restart_syscall();

	index = get_netdev_queue_index(queue);

	/* If queue belongs to subordinate dev use its TC mapping */
	dev = netdev_get_tx_queue(dev, index)->sb_dev ? : dev;

	num_tc = dev->num_tc;
	tc = netdev_txq_to_tc(dev, index);

	rtnl_unlock();

	if (tc < 0)
		return -EINVAL;

	/* We can report the traffic class one of two ways:
	 * Subordinate device traffic classes are reported with the traffic
	 * class first, and then the subordinate class so for example TC0 on
	 * subordinate device 2 will be reported as "0-2". If the queue
	 * belongs to the root device it will be reported with just the
	 * traffic class, so just "0" for TC 0 for example.
	 */
	return num_tc < 0 ? sysfs_emit(buf, "%d%d\n", tc, num_tc) :
			    sysfs_emit(buf, "%d\n", tc);
}

#ifdef CONFIG_XPS
static ssize_t tx_maxrate_show(struct netdev_queue *queue,
			       char *buf)
{
	return sysfs_emit(buf, "%lu\n", queue->tx_maxrate);
}

static ssize_t tx_maxrate_store(struct netdev_queue *queue,
				const char *buf, size_t len)
{
	struct net_device *dev = queue->dev;
	int err, index = get_netdev_queue_index(queue);
	u32 rate = 0;

	if (!capable(CAP_NET_ADMIN))
		return -EPERM;

	/* The check is also done later; this helps returning early without
	 * hitting the trylock/restart below.
	 */
	if (!dev->netdev_ops->ndo_set_tx_maxrate)
		return -EOPNOTSUPP;

	err = kstrtou32(buf, 10, &rate);
	if (err < 0)
		return err;

	if (!rtnl_trylock())
		return restart_syscall();

	err = -EOPNOTSUPP;
	if (dev->netdev_ops->ndo_set_tx_maxrate)
		err = dev->netdev_ops->ndo_set_tx_maxrate(dev, index, rate);

	rtnl_unlock();
	if (!err) {
		queue->tx_maxrate = rate;
		return len;
	}
	return err;
}

static struct netdev_queue_attribute queue_tx_maxrate __ro_after_init
	= __ATTR_RW(tx_maxrate);
#endif

static struct netdev_queue_attribute queue_trans_timeout __ro_after_init
	= __ATTR_RO(tx_timeout);

static struct netdev_queue_attribute queue_traffic_class __ro_after_init
	= __ATTR_RO(traffic_class);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_BQL
/*
 * Byte queue limits sysfs structures and functions.
 */
static ssize_t bql_show(char *buf, unsigned int value)
{
<<<<<<< HEAD
	return sprintf(buf, "%u\n", value);
=======
	return sysfs_emit(buf, "%u\n", value);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static ssize_t bql_set(const char *buf, const size_t count,
		       unsigned int *pvalue)
{
	unsigned int value;
	int err;

<<<<<<< HEAD
	if (!strcmp(buf, "max") || !strcmp(buf, "max\n"))
		value = DQL_MAX_LIMIT;
	else {
=======
	if (!strcmp(buf, "max") || !strcmp(buf, "max\n")) {
		value = DQL_MAX_LIMIT;
	} else {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = kstrtouint(buf, 10, &value);
		if (err < 0)
			return err;
		if (value > DQL_MAX_LIMIT)
			return -EINVAL;
	}

	*pvalue = value;

	return count;
}

static ssize_t bql_show_hold_time(struct netdev_queue *queue,
<<<<<<< HEAD
				  struct netdev_queue_attribute *attr,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				  char *buf)
{
	struct dql *dql = &queue->dql;

<<<<<<< HEAD
	return sprintf(buf, "%u\n", jiffies_to_msecs(dql->slack_hold_time));
}

static ssize_t bql_set_hold_time(struct netdev_queue *queue,
				 struct netdev_queue_attribute *attribute,
				 const char *buf, size_t len)
{
	struct dql *dql = &queue->dql;
	unsigned value;
=======
	return sysfs_emit(buf, "%u\n", jiffies_to_msecs(dql->slack_hold_time));
}

static ssize_t bql_set_hold_time(struct netdev_queue *queue,
				 const char *buf, size_t len)
{
	struct dql *dql = &queue->dql;
	unsigned int value;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int err;

	err = kstrtouint(buf, 10, &value);
	if (err < 0)
		return err;

	dql->slack_hold_time = msecs_to_jiffies(value);

	return len;
}

<<<<<<< HEAD
static struct netdev_queue_attribute bql_hold_time_attribute =
	__ATTR(hold_time, S_IRUGO | S_IWUSR, bql_show_hold_time,
	    bql_set_hold_time);

static ssize_t bql_show_inflight(struct netdev_queue *queue,
				 struct netdev_queue_attribute *attr,
=======
static struct netdev_queue_attribute bql_hold_time_attribute __ro_after_init
	= __ATTR(hold_time, 0644,
		 bql_show_hold_time, bql_set_hold_time);

static ssize_t bql_show_stall_thrs(struct netdev_queue *queue, char *buf)
{
	struct dql *dql = &queue->dql;

	return sprintf(buf, "%u\n", jiffies_to_msecs(dql->stall_thrs));
}

static ssize_t bql_set_stall_thrs(struct netdev_queue *queue,
				  const char *buf, size_t len)
{
	struct dql *dql = &queue->dql;
	unsigned int value;
	int err;

	err = kstrtouint(buf, 10, &value);
	if (err < 0)
		return err;

	value = msecs_to_jiffies(value);
	if (value && (value < 4 || value > 4 / 2 * BITS_PER_LONG))
		return -ERANGE;

	if (!dql->stall_thrs && value)
		dql->last_reap = jiffies;
	/* Force last_reap to be live */
	smp_wmb();
	dql->stall_thrs = value;

	return len;
}

static struct netdev_queue_attribute bql_stall_thrs_attribute __ro_after_init =
	__ATTR(stall_thrs, 0644, bql_show_stall_thrs, bql_set_stall_thrs);

static ssize_t bql_show_stall_max(struct netdev_queue *queue, char *buf)
{
	return sprintf(buf, "%u\n", READ_ONCE(queue->dql.stall_max));
}

static ssize_t bql_set_stall_max(struct netdev_queue *queue,
				 const char *buf, size_t len)
{
	WRITE_ONCE(queue->dql.stall_max, 0);
	return len;
}

static struct netdev_queue_attribute bql_stall_max_attribute __ro_after_init =
	__ATTR(stall_max, 0644, bql_show_stall_max, bql_set_stall_max);

static ssize_t bql_show_stall_cnt(struct netdev_queue *queue, char *buf)
{
	struct dql *dql = &queue->dql;

	return sprintf(buf, "%lu\n", dql->stall_cnt);
}

static struct netdev_queue_attribute bql_stall_cnt_attribute __ro_after_init =
	__ATTR(stall_cnt, 0444, bql_show_stall_cnt, NULL);

static ssize_t bql_show_inflight(struct netdev_queue *queue,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				 char *buf)
{
	struct dql *dql = &queue->dql;

<<<<<<< HEAD
	return sprintf(buf, "%u\n", dql->num_queued - dql->num_completed);
}

static struct netdev_queue_attribute bql_inflight_attribute =
	__ATTR(inflight, S_IRUGO, bql_show_inflight, NULL);

#define BQL_ATTR(NAME, FIELD)						\
static ssize_t bql_show_ ## NAME(struct netdev_queue *queue,		\
				 struct netdev_queue_attribute *attr,	\
=======
	return sysfs_emit(buf, "%u\n", dql->num_queued - dql->num_completed);
}

static struct netdev_queue_attribute bql_inflight_attribute __ro_after_init =
	__ATTR(inflight, 0444, bql_show_inflight, NULL);

#define BQL_ATTR(NAME, FIELD)						\
static ssize_t bql_show_ ## NAME(struct netdev_queue *queue,		\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				 char *buf)				\
{									\
	return bql_show(buf, queue->dql.FIELD);				\
}									\
									\
static ssize_t bql_set_ ## NAME(struct netdev_queue *queue,		\
<<<<<<< HEAD
				struct netdev_queue_attribute *attr,	\
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				const char *buf, size_t len)		\
{									\
	return bql_set(buf, len, &queue->dql.FIELD);			\
}									\
									\
<<<<<<< HEAD
static struct netdev_queue_attribute bql_ ## NAME ## _attribute =	\
	__ATTR(NAME, S_IRUGO | S_IWUSR, bql_show_ ## NAME,		\
	    bql_set_ ## NAME);

BQL_ATTR(limit, limit)
BQL_ATTR(limit_max, max_limit)
BQL_ATTR(limit_min, min_limit)

static struct attribute *dql_attrs[] = {
=======
static struct netdev_queue_attribute bql_ ## NAME ## _attribute __ro_after_init \
	= __ATTR(NAME, 0644,				\
		 bql_show_ ## NAME, bql_set_ ## NAME)

BQL_ATTR(limit, limit);
BQL_ATTR(limit_max, max_limit);
BQL_ATTR(limit_min, min_limit);

static struct attribute *dql_attrs[] __ro_after_init = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	&bql_limit_attribute.attr,
	&bql_limit_max_attribute.attr,
	&bql_limit_min_attribute.attr,
	&bql_hold_time_attribute.attr,
	&bql_inflight_attribute.attr,
<<<<<<< HEAD
	NULL
};

static struct attribute_group dql_group = {
	.name  = "byte_queue_limits",
	.attrs  = dql_attrs,
};
#endif /* CONFIG_BQL */

#ifdef CONFIG_XPS
static inline unsigned int get_netdev_queue_index(struct netdev_queue *queue)
{
	struct net_device *dev = queue->dev;
	int i;

	for (i = 0; i < dev->num_tx_queues; i++)
		if (queue == &dev->_tx[i])
			break;

	BUG_ON(i >= dev->num_tx_queues);

	return i;
}


static ssize_t show_xps_map(struct netdev_queue *queue,
			    struct netdev_queue_attribute *attribute, char *buf)
{
	struct net_device *dev = queue->dev;
	struct xps_dev_maps *dev_maps;
	cpumask_var_t mask;
	unsigned long index;
	size_t len = 0;
	int i;

	if (!zalloc_cpumask_var(&mask, GFP_KERNEL))
		return -ENOMEM;

	index = get_netdev_queue_index(queue);

	rcu_read_lock();
	dev_maps = rcu_dereference(dev->xps_maps);
	if (dev_maps) {
		for_each_possible_cpu(i) {
			struct xps_map *map =
			    rcu_dereference(dev_maps->cpu_map[i]);
			if (map) {
				int j;
				for (j = 0; j < map->len; j++) {
					if (map->queues[j] == index) {
						cpumask_set_cpu(i, mask);
						break;
					}
				}
			}
		}
	}
	rcu_read_unlock();

	len += cpumask_scnprintf(buf + len, PAGE_SIZE, mask);
	if (PAGE_SIZE - len < 3) {
		free_cpumask_var(mask);
		return -EINVAL;
	}

	free_cpumask_var(mask);
	len += sprintf(buf + len, "\n");
	return len;
}

static DEFINE_MUTEX(xps_map_mutex);
#define xmap_dereference(P)		\
	rcu_dereference_protected((P), lockdep_is_held(&xps_map_mutex))

static void xps_queue_release(struct netdev_queue *queue)
{
	struct net_device *dev = queue->dev;
	struct xps_dev_maps *dev_maps;
	struct xps_map *map;
	unsigned long index;
	int i, pos, nonempty = 0;

	index = get_netdev_queue_index(queue);

	mutex_lock(&xps_map_mutex);
	dev_maps = xmap_dereference(dev->xps_maps);

	if (dev_maps) {
		for_each_possible_cpu(i) {
			map = xmap_dereference(dev_maps->cpu_map[i]);
			if (!map)
				continue;

			for (pos = 0; pos < map->len; pos++)
				if (map->queues[pos] == index)
					break;

			if (pos < map->len) {
				if (map->len > 1)
					map->queues[pos] =
					    map->queues[--map->len];
				else {
					RCU_INIT_POINTER(dev_maps->cpu_map[i],
					    NULL);
					kfree_rcu(map, rcu);
					map = NULL;
				}
			}
			if (map)
				nonempty = 1;
		}

		if (!nonempty) {
			RCU_INIT_POINTER(dev->xps_maps, NULL);
			kfree_rcu(dev_maps, rcu);
		}
	}
	mutex_unlock(&xps_map_mutex);
}

static ssize_t store_xps_map(struct netdev_queue *queue,
		      struct netdev_queue_attribute *attribute,
		      const char *buf, size_t len)
{
	struct net_device *dev = queue->dev;
	cpumask_var_t mask;
	int err, i, cpu, pos, map_len, alloc_len, need_set;
	unsigned long index;
	struct xps_map *map, *new_map;
	struct xps_dev_maps *dev_maps, *new_dev_maps;
	int nonempty = 0;
	int numa_node_id = -2;
=======
	&bql_stall_thrs_attribute.attr,
	&bql_stall_cnt_attribute.attr,
	&bql_stall_max_attribute.attr,
	NULL
};

static const struct attribute_group dql_group = {
	.name  = "byte_queue_limits",
	.attrs  = dql_attrs,
};
#else
/* Fake declaration, all the code using it should be dead */
extern const struct attribute_group dql_group;
#endif /* CONFIG_BQL */

#ifdef CONFIG_XPS
static ssize_t xps_queue_show(struct net_device *dev, unsigned int index,
			      int tc, char *buf, enum xps_map_type type)
{
	struct xps_dev_maps *dev_maps;
	unsigned long *mask;
	unsigned int nr_ids;
	int j, len;

	rcu_read_lock();
	dev_maps = rcu_dereference(dev->xps_maps[type]);

	/* Default to nr_cpu_ids/dev->num_rx_queues and do not just return 0
	 * when dev_maps hasn't been allocated yet, to be backward compatible.
	 */
	nr_ids = dev_maps ? dev_maps->nr_ids :
		 (type == XPS_CPUS ? nr_cpu_ids : dev->num_rx_queues);

	mask = bitmap_zalloc(nr_ids, GFP_NOWAIT);
	if (!mask) {
		rcu_read_unlock();
		return -ENOMEM;
	}

	if (!dev_maps || tc >= dev_maps->num_tc)
		goto out_no_maps;

	for (j = 0; j < nr_ids; j++) {
		int i, tci = j * dev_maps->num_tc + tc;
		struct xps_map *map;

		map = rcu_dereference(dev_maps->attr_map[tci]);
		if (!map)
			continue;

		for (i = map->len; i--;) {
			if (map->queues[i] == index) {
				__set_bit(j, mask);
				break;
			}
		}
	}
out_no_maps:
	rcu_read_unlock();

	len = bitmap_print_to_pagebuf(false, buf, mask, nr_ids);
	bitmap_free(mask);

	return len < PAGE_SIZE ? len : -EINVAL;
}

static ssize_t xps_cpus_show(struct netdev_queue *queue, char *buf)
{
	struct net_device *dev = queue->dev;
	unsigned int index;
	int len, tc;

	if (!netif_is_multiqueue(dev))
		return -ENOENT;

	index = get_netdev_queue_index(queue);

	if (!rtnl_trylock())
		return restart_syscall();

	/* If queue belongs to subordinate dev use its map */
	dev = netdev_get_tx_queue(dev, index)->sb_dev ? : dev;

	tc = netdev_txq_to_tc(dev, index);
	if (tc < 0) {
		rtnl_unlock();
		return -EINVAL;
	}

	/* Make sure the subordinate device can't be freed */
	get_device(&dev->dev);
	rtnl_unlock();

	len = xps_queue_show(dev, index, tc, buf, XPS_CPUS);

	put_device(&dev->dev);
	return len;
}

static ssize_t xps_cpus_store(struct netdev_queue *queue,
			      const char *buf, size_t len)
{
	struct net_device *dev = queue->dev;
	unsigned int index;
	cpumask_var_t mask;
	int err;

	if (!netif_is_multiqueue(dev))
		return -ENOENT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!capable(CAP_NET_ADMIN))
		return -EPERM;

	if (!alloc_cpumask_var(&mask, GFP_KERNEL))
		return -ENOMEM;

	index = get_netdev_queue_index(queue);

	err = bitmap_parse(buf, len, cpumask_bits(mask), nr_cpumask_bits);
	if (err) {
		free_cpumask_var(mask);
		return err;
	}

<<<<<<< HEAD
	new_dev_maps = kzalloc(max_t(unsigned,
	    XPS_DEV_MAPS_SIZE, L1_CACHE_BYTES), GFP_KERNEL);
	if (!new_dev_maps) {
		free_cpumask_var(mask);
		return -ENOMEM;
	}

	mutex_lock(&xps_map_mutex);

	dev_maps = xmap_dereference(dev->xps_maps);

	for_each_possible_cpu(cpu) {
		map = dev_maps ?
			xmap_dereference(dev_maps->cpu_map[cpu]) : NULL;
		new_map = map;
		if (map) {
			for (pos = 0; pos < map->len; pos++)
				if (map->queues[pos] == index)
					break;
			map_len = map->len;
			alloc_len = map->alloc_len;
		} else
			pos = map_len = alloc_len = 0;

		need_set = cpumask_test_cpu(cpu, mask) && cpu_online(cpu);
#ifdef CONFIG_NUMA
		if (need_set) {
			if (numa_node_id == -2)
				numa_node_id = cpu_to_node(cpu);
			else if (numa_node_id != cpu_to_node(cpu))
				numa_node_id = -1;
		}
#endif
		if (need_set && pos >= map_len) {
			/* Need to add queue to this CPU's map */
			if (map_len >= alloc_len) {
				alloc_len = alloc_len ?
				    2 * alloc_len : XPS_MIN_MAP_ALLOC;
				new_map = kzalloc_node(XPS_MAP_SIZE(alloc_len),
						       GFP_KERNEL,
						       cpu_to_node(cpu));
				if (!new_map)
					goto error;
				new_map->alloc_len = alloc_len;
				for (i = 0; i < map_len; i++)
					new_map->queues[i] = map->queues[i];
				new_map->len = map_len;
			}
			new_map->queues[new_map->len++] = index;
		} else if (!need_set && pos < map_len) {
			/* Need to remove queue from this CPU's map */
			if (map_len > 1)
				new_map->queues[pos] =
				    new_map->queues[--new_map->len];
			else
				new_map = NULL;
		}
		RCU_INIT_POINTER(new_dev_maps->cpu_map[cpu], new_map);
	}

	/* Cleanup old maps */
	for_each_possible_cpu(cpu) {
		map = dev_maps ?
			xmap_dereference(dev_maps->cpu_map[cpu]) : NULL;
		if (map && xmap_dereference(new_dev_maps->cpu_map[cpu]) != map)
			kfree_rcu(map, rcu);
		if (new_dev_maps->cpu_map[cpu])
			nonempty = 1;
	}

	if (nonempty) {
		rcu_assign_pointer(dev->xps_maps, new_dev_maps);
	} else {
		kfree(new_dev_maps);
		RCU_INIT_POINTER(dev->xps_maps, NULL);
	}

	if (dev_maps)
		kfree_rcu(dev_maps, rcu);

	netdev_queue_numa_node_write(queue, (numa_node_id >= 0) ? numa_node_id :
					    NUMA_NO_NODE);

	mutex_unlock(&xps_map_mutex);

	free_cpumask_var(mask);
	return len;

error:
	mutex_unlock(&xps_map_mutex);

	if (new_dev_maps)
		for_each_possible_cpu(i)
			kfree(rcu_dereference_protected(
				new_dev_maps->cpu_map[i],
				1));
	kfree(new_dev_maps);
	free_cpumask_var(mask);
	return -ENOMEM;
}

static struct netdev_queue_attribute xps_cpus_attribute =
    __ATTR(xps_cpus, S_IRUGO | S_IWUSR, show_xps_map, store_xps_map);
#endif /* CONFIG_XPS */

static struct attribute *netdev_queue_default_attrs[] = {
	&queue_trans_timeout.attr,
#ifdef CONFIG_XPS
	&xps_cpus_attribute.attr,
#endif
	NULL
};
=======
	if (!rtnl_trylock()) {
		free_cpumask_var(mask);
		return restart_syscall();
	}

	err = netif_set_xps_queue(dev, mask, index);
	rtnl_unlock();

	free_cpumask_var(mask);

	return err ? : len;
}

static struct netdev_queue_attribute xps_cpus_attribute __ro_after_init
	= __ATTR_RW(xps_cpus);

static ssize_t xps_rxqs_show(struct netdev_queue *queue, char *buf)
{
	struct net_device *dev = queue->dev;
	unsigned int index;
	int tc;

	index = get_netdev_queue_index(queue);

	if (!rtnl_trylock())
		return restart_syscall();

	tc = netdev_txq_to_tc(dev, index);
	rtnl_unlock();
	if (tc < 0)
		return -EINVAL;

	return xps_queue_show(dev, index, tc, buf, XPS_RXQS);
}

static ssize_t xps_rxqs_store(struct netdev_queue *queue, const char *buf,
			      size_t len)
{
	struct net_device *dev = queue->dev;
	struct net *net = dev_net(dev);
	unsigned long *mask;
	unsigned int index;
	int err;

	if (!ns_capable(net->user_ns, CAP_NET_ADMIN))
		return -EPERM;

	mask = bitmap_zalloc(dev->num_rx_queues, GFP_KERNEL);
	if (!mask)
		return -ENOMEM;

	index = get_netdev_queue_index(queue);

	err = bitmap_parse(buf, len, mask, dev->num_rx_queues);
	if (err) {
		bitmap_free(mask);
		return err;
	}

	if (!rtnl_trylock()) {
		bitmap_free(mask);
		return restart_syscall();
	}

	cpus_read_lock();
	err = __netif_set_xps_queue(dev, mask, index, XPS_RXQS);
	cpus_read_unlock();

	rtnl_unlock();

	bitmap_free(mask);
	return err ? : len;
}

static struct netdev_queue_attribute xps_rxqs_attribute __ro_after_init
	= __ATTR_RW(xps_rxqs);
#endif /* CONFIG_XPS */

static struct attribute *netdev_queue_default_attrs[] __ro_after_init = {
	&queue_trans_timeout.attr,
	&queue_traffic_class.attr,
#ifdef CONFIG_XPS
	&xps_cpus_attribute.attr,
	&xps_rxqs_attribute.attr,
	&queue_tx_maxrate.attr,
#endif
	NULL
};
ATTRIBUTE_GROUPS(netdev_queue_default);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static void netdev_queue_release(struct kobject *kobj)
{
	struct netdev_queue *queue = to_netdev_queue(kobj);

<<<<<<< HEAD
#ifdef CONFIG_XPS
	xps_queue_release(queue);
#endif

	memset(kobj, 0, sizeof(*kobj));
	dev_put(queue->dev);
}

static struct kobj_type netdev_queue_ktype = {
	.sysfs_ops = &netdev_queue_sysfs_ops,
	.release = netdev_queue_release,
	.default_attrs = netdev_queue_default_attrs,
};

static int netdev_queue_add_kobject(struct net_device *net, int index)
{
	struct netdev_queue *queue = net->_tx + index;
	struct kobject *kobj = &queue->kobj;
	int error = 0;

	kobj->kset = net->queues_kset;
	error = kobject_init_and_add(kobj, &netdev_queue_ktype, NULL,
	    "tx-%u", index);
	if (error)
		goto exit;

#ifdef CONFIG_BQL
	error = sysfs_create_group(kobj, &dql_group);
	if (error)
		goto exit;
#endif

	kobject_uevent(kobj, KOBJ_ADD);
	dev_hold(queue->dev);

	return 0;
exit:
	kobject_put(kobj);
	return error;
}
#endif /* CONFIG_SYSFS */

int
netdev_queue_update_kobjects(struct net_device *net, int old_num, int new_num)
=======
	memset(kobj, 0, sizeof(*kobj));
	netdev_put(queue->dev, &queue->dev_tracker);
}

static const void *netdev_queue_namespace(const struct kobject *kobj)
{
	struct netdev_queue *queue = to_netdev_queue(kobj);
	struct device *dev = &queue->dev->dev;
	const void *ns = NULL;

	if (dev->class && dev->class->ns_type)
		ns = dev->class->namespace(dev);

	return ns;
}

static void netdev_queue_get_ownership(const struct kobject *kobj,
				       kuid_t *uid, kgid_t *gid)
{
	const struct net *net = netdev_queue_namespace(kobj);

	net_ns_get_ownership(net, uid, gid);
}

static const struct kobj_type netdev_queue_ktype = {
	.sysfs_ops = &netdev_queue_sysfs_ops,
	.release = netdev_queue_release,
	.default_groups = netdev_queue_default_groups,
	.namespace = netdev_queue_namespace,
	.get_ownership = netdev_queue_get_ownership,
};

static bool netdev_uses_bql(const struct net_device *dev)
{
	if (dev->features & NETIF_F_LLTX ||
	    dev->priv_flags & IFF_NO_QUEUE)
		return false;

	return IS_ENABLED(CONFIG_BQL);
}

static int netdev_queue_add_kobject(struct net_device *dev, int index)
{
	struct netdev_queue *queue = dev->_tx + index;
	struct kobject *kobj = &queue->kobj;
	int error = 0;

	/* Kobject_put later will trigger netdev_queue_release call
	 * which decreases dev refcount: Take that reference here
	 */
	netdev_hold(queue->dev, &queue->dev_tracker, GFP_KERNEL);

	kobj->kset = dev->queues_kset;
	error = kobject_init_and_add(kobj, &netdev_queue_ktype, NULL,
				     "tx-%u", index);
	if (error)
		goto err;

	if (netdev_uses_bql(dev)) {
		error = sysfs_create_group(kobj, &dql_group);
		if (error)
			goto err;
	}

	kobject_uevent(kobj, KOBJ_ADD);
	return 0;

err:
	kobject_put(kobj);
	return error;
}

static int tx_queue_change_owner(struct net_device *ndev, int index,
				 kuid_t kuid, kgid_t kgid)
{
	struct netdev_queue *queue = ndev->_tx + index;
	struct kobject *kobj = &queue->kobj;
	int error;

	error = sysfs_change_owner(kobj, kuid, kgid);
	if (error)
		return error;

	if (netdev_uses_bql(ndev))
		error = sysfs_group_change_owner(kobj, &dql_group, kuid, kgid);

	return error;
}
#endif /* CONFIG_SYSFS */

int
netdev_queue_update_kobjects(struct net_device *dev, int old_num, int new_num)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
#ifdef CONFIG_SYSFS
	int i;
	int error = 0;

<<<<<<< HEAD
	for (i = old_num; i < new_num; i++) {
		error = netdev_queue_add_kobject(net, i);
=======
	/* Tx queue kobjects are allowed to be updated when a device is being
	 * unregistered, but solely to remove queues from qdiscs. Any path
	 * adding queues should be fixed.
	 */
	WARN(dev->reg_state == NETREG_UNREGISTERING && new_num > old_num,
	     "New queues can't be registered after device unregistration.");

	for (i = old_num; i < new_num; i++) {
		error = netdev_queue_add_kobject(dev, i);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (error) {
			new_num = old_num;
			break;
		}
	}

	while (--i >= new_num) {
<<<<<<< HEAD
		struct netdev_queue *queue = net->_tx + i;

#ifdef CONFIG_BQL
		sysfs_remove_group(&queue->kobj, &dql_group);
#endif
=======
		struct netdev_queue *queue = dev->_tx + i;

		if (!refcount_read(&dev_net(dev)->ns.count))
			queue->kobj.uevent_suppress = 1;

		if (netdev_uses_bql(dev))
			sysfs_remove_group(&queue->kobj, &dql_group);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		kobject_put(&queue->kobj);
	}

	return error;
#else
	return 0;
#endif /* CONFIG_SYSFS */
}

<<<<<<< HEAD
static int register_queue_kobjects(struct net_device *net)
=======
static int net_tx_queue_change_owner(struct net_device *dev, int num,
				     kuid_t kuid, kgid_t kgid)
{
#ifdef CONFIG_SYSFS
	int error = 0;
	int i;

	for (i = 0; i < num; i++) {
		error = tx_queue_change_owner(dev, i, kuid, kgid);
		if (error)
			break;
	}

	return error;
#else
	return 0;
#endif /* CONFIG_SYSFS */
}

static int register_queue_kobjects(struct net_device *dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int error = 0, txq = 0, rxq = 0, real_rx = 0, real_tx = 0;

#ifdef CONFIG_SYSFS
<<<<<<< HEAD
	net->queues_kset = kset_create_and_add("queues",
	    NULL, &net->dev.kobj);
	if (!net->queues_kset)
		return -ENOMEM;
#endif

#ifdef CONFIG_RPS
	real_rx = net->real_num_rx_queues;
#endif
	real_tx = net->real_num_tx_queues;

	error = net_rx_queue_update_kobjects(net, 0, real_rx);
=======
	dev->queues_kset = kset_create_and_add("queues",
					       NULL, &dev->dev.kobj);
	if (!dev->queues_kset)
		return -ENOMEM;
	real_rx = dev->real_num_rx_queues;
#endif
	real_tx = dev->real_num_tx_queues;

	error = net_rx_queue_update_kobjects(dev, 0, real_rx);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (error)
		goto error;
	rxq = real_rx;

<<<<<<< HEAD
	error = netdev_queue_update_kobjects(net, 0, real_tx);
=======
	error = netdev_queue_update_kobjects(dev, 0, real_tx);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (error)
		goto error;
	txq = real_tx;

	return 0;

error:
<<<<<<< HEAD
	netdev_queue_update_kobjects(net, txq, 0);
	net_rx_queue_update_kobjects(net, rxq, 0);
	return error;
}

static void remove_queue_kobjects(struct net_device *net)
{
	int real_rx = 0, real_tx = 0;

#ifdef CONFIG_RPS
	real_rx = net->real_num_rx_queues;
#endif
	real_tx = net->real_num_tx_queues;

	net_rx_queue_update_kobjects(net, real_rx, 0);
	netdev_queue_update_kobjects(net, real_tx, 0);
#ifdef CONFIG_SYSFS
	kset_unregister(net->queues_kset);
#endif
=======
	netdev_queue_update_kobjects(dev, txq, 0);
	net_rx_queue_update_kobjects(dev, rxq, 0);
#ifdef CONFIG_SYSFS
	kset_unregister(dev->queues_kset);
#endif
	return error;
}

static int queue_change_owner(struct net_device *ndev, kuid_t kuid, kgid_t kgid)
{
	int error = 0, real_rx = 0, real_tx = 0;

#ifdef CONFIG_SYSFS
	if (ndev->queues_kset) {
		error = sysfs_change_owner(&ndev->queues_kset->kobj, kuid, kgid);
		if (error)
			return error;
	}
	real_rx = ndev->real_num_rx_queues;
#endif
	real_tx = ndev->real_num_tx_queues;

	error = net_rx_queue_change_owner(ndev, real_rx, kuid, kgid);
	if (error)
		return error;

	error = net_tx_queue_change_owner(ndev, real_tx, kuid, kgid);
	if (error)
		return error;

	return 0;
}

static void remove_queue_kobjects(struct net_device *dev)
{
	int real_rx = 0, real_tx = 0;

#ifdef CONFIG_SYSFS
	real_rx = dev->real_num_rx_queues;
#endif
	real_tx = dev->real_num_tx_queues;

	net_rx_queue_update_kobjects(dev, real_rx, 0);
	netdev_queue_update_kobjects(dev, real_tx, 0);

	dev->real_num_rx_queues = 0;
	dev->real_num_tx_queues = 0;
#ifdef CONFIG_SYSFS
	kset_unregister(dev->queues_kset);
#endif
}

static bool net_current_may_mount(void)
{
	struct net *net = current->nsproxy->net_ns;

	return ns_capable(net->user_ns, CAP_SYS_ADMIN);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void *net_grab_current_ns(void)
{
	struct net *ns = current->nsproxy->net_ns;
#ifdef CONFIG_NET_NS
	if (ns)
<<<<<<< HEAD
		atomic_inc(&ns->passive);
=======
		refcount_inc(&ns->passive);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
	return ns;
}

static const void *net_initial_ns(void)
{
	return &init_net;
}

static const void *net_netlink_ns(struct sock *sk)
{
	return sock_net(sk);
}

<<<<<<< HEAD
struct kobj_ns_type_operations net_ns_type_operations = {
	.type = KOBJ_NS_TYPE_NET,
=======
const struct kobj_ns_type_operations net_ns_type_operations = {
	.type = KOBJ_NS_TYPE_NET,
	.current_may_mount = net_current_may_mount,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.grab_current_ns = net_grab_current_ns,
	.netlink_ns = net_netlink_ns,
	.initial_ns = net_initial_ns,
	.drop_ns = net_drop_ns,
};
EXPORT_SYMBOL_GPL(net_ns_type_operations);

<<<<<<< HEAD
#ifdef CONFIG_HOTPLUG
static int netdev_uevent(struct device *d, struct kobj_uevent_env *env)
{
	struct net_device *dev = to_net_dev(d);
=======
static int netdev_uevent(const struct device *d, struct kobj_uevent_env *env)
{
	const struct net_device *dev = to_net_dev(d);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int retval;

	/* pass interface to uevent. */
	retval = add_uevent_var(env, "INTERFACE=%s", dev->name);
	if (retval)
		goto exit;

	/* pass ifindex to uevent.
	 * ifindex is useful as it won't change (interface name may change)
<<<<<<< HEAD
	 * and is what RtNetlink uses natively. */
=======
	 * and is what RtNetlink uses natively.
	 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	retval = add_uevent_var(env, "IFINDEX=%d", dev->ifindex);

exit:
	return retval;
}
<<<<<<< HEAD
#endif
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 *	netdev_release -- destroy and free a dead device.
 *	Called when last reference to device kobject is gone.
 */
static void netdev_release(struct device *d)
{
	struct net_device *dev = to_net_dev(d);

	BUG_ON(dev->reg_state != NETREG_RELEASED);

<<<<<<< HEAD
	kfree(dev->ifalias);
	kfree((char *)dev - dev->padded);
}

static const void *net_namespace(struct device *d)
{
	struct net_device *dev;
	dev = container_of(d, struct net_device, dev);
	return dev_net(dev);
}

static struct class net_class = {
	.name = "net",
	.dev_release = netdev_release,
#ifdef CONFIG_SYSFS
	.dev_attrs = net_class_attributes,
#endif /* CONFIG_SYSFS */
#ifdef CONFIG_HOTPLUG
	.dev_uevent = netdev_uevent,
#endif
	.ns_type = &net_ns_type_operations,
	.namespace = net_namespace,
};

/* Delete sysfs entries but hold kobject reference until after all
 * netdev references are gone.
 */
void netdev_unregister_kobject(struct net_device * net)
{
	struct device *dev = &(net->dev);

	kobject_get(&dev->kobj);

	remove_queue_kobjects(net);
=======
	/* no need to wait for rcu grace period:
	 * device is dead and about to be freed.
	 */
	kfree(rcu_access_pointer(dev->ifalias));
	netdev_freemem(dev);
}

static const void *net_namespace(const struct device *d)
{
	const struct net_device *dev = to_net_dev(d);

	return dev_net(dev);
}

static void net_get_ownership(const struct device *d, kuid_t *uid, kgid_t *gid)
{
	const struct net_device *dev = to_net_dev(d);
	const struct net *net = dev_net(dev);

	net_ns_get_ownership(net, uid, gid);
}

static struct class net_class __ro_after_init = {
	.name = "net",
	.dev_release = netdev_release,
	.dev_groups = net_class_groups,
	.dev_uevent = netdev_uevent,
	.ns_type = &net_ns_type_operations,
	.namespace = net_namespace,
	.get_ownership = net_get_ownership,
};

#ifdef CONFIG_OF
static int of_dev_node_match(struct device *dev, const void *data)
{
	for (; dev; dev = dev->parent) {
		if (dev->of_node == data)
			return 1;
	}

	return 0;
}

/*
 * of_find_net_device_by_node - lookup the net device for the device node
 * @np: OF device node
 *
 * Looks up the net_device structure corresponding with the device node.
 * If successful, returns a pointer to the net_device with the embedded
 * struct device refcount incremented by one, or NULL on failure. The
 * refcount must be dropped when done with the net_device.
 */
struct net_device *of_find_net_device_by_node(struct device_node *np)
{
	struct device *dev;

	dev = class_find_device(&net_class, NULL, np, of_dev_node_match);
	if (!dev)
		return NULL;

	return to_net_dev(dev);
}
EXPORT_SYMBOL(of_find_net_device_by_node);
#endif

/* Delete sysfs entries but hold kobject reference until after all
 * netdev references are gone.
 */
void netdev_unregister_kobject(struct net_device *ndev)
{
	struct device *dev = &ndev->dev;

	if (!refcount_read(&dev_net(ndev)->ns.count))
		dev_set_uevent_suppress(dev, 1);

	kobject_get(&dev->kobj);

	remove_queue_kobjects(ndev);

	pm_runtime_set_memalloc_noio(dev, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	device_del(dev);
}

/* Create sysfs entries for network device. */
<<<<<<< HEAD
int netdev_register_kobject(struct net_device *net)
{
	struct device *dev = &(net->dev);
	const struct attribute_group **groups = net->sysfs_groups;
=======
int netdev_register_kobject(struct net_device *ndev)
{
	struct device *dev = &ndev->dev;
	const struct attribute_group **groups = ndev->sysfs_groups;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int error = 0;

	device_initialize(dev);
	dev->class = &net_class;
<<<<<<< HEAD
	dev->platform_data = net;
	dev->groups = groups;

	dev_set_name(dev, "%s", net->name);
=======
	dev->platform_data = ndev;
	dev->groups = groups;

	dev_set_name(dev, "%s", ndev->name);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_SYSFS
	/* Allow for a device specific group */
	if (*groups)
		groups++;

	*groups++ = &netstat_group;
<<<<<<< HEAD
#ifdef CONFIG_WIRELESS_EXT_SYSFS
	if (net->ieee80211_ptr)
		*groups++ = &wireless_group;
#ifdef CONFIG_WIRELESS_EXT
	else if (net->wireless_handlers)
		*groups++ = &wireless_group;
#endif
#endif
=======

	if (wireless_group_needed(ndev))
		*groups++ = &wireless_group;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* CONFIG_SYSFS */

	error = device_add(dev);
	if (error)
		return error;

<<<<<<< HEAD
	error = register_queue_kobjects(net);
=======
	error = register_queue_kobjects(ndev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (error) {
		device_del(dev);
		return error;
	}

<<<<<<< HEAD
	return error;
}

int netdev_class_create_file(struct class_attribute *class_attr)
{
	return class_create_file(&net_class, class_attr);
}
EXPORT_SYMBOL(netdev_class_create_file);

void netdev_class_remove_file(struct class_attribute *class_attr)
{
	class_remove_file(&net_class, class_attr);
}
EXPORT_SYMBOL(netdev_class_remove_file);

int netdev_kobject_init(void)
=======
	pm_runtime_set_memalloc_noio(dev, true);

	return error;
}

/* Change owner for sysfs entries when moving network devices across network
 * namespaces owned by different user namespaces.
 */
int netdev_change_owner(struct net_device *ndev, const struct net *net_old,
			const struct net *net_new)
{
	kuid_t old_uid = GLOBAL_ROOT_UID, new_uid = GLOBAL_ROOT_UID;
	kgid_t old_gid = GLOBAL_ROOT_GID, new_gid = GLOBAL_ROOT_GID;
	struct device *dev = &ndev->dev;
	int error;

	net_ns_get_ownership(net_old, &old_uid, &old_gid);
	net_ns_get_ownership(net_new, &new_uid, &new_gid);

	/* The network namespace was changed but the owning user namespace is
	 * identical so there's no need to change the owner of sysfs entries.
	 */
	if (uid_eq(old_uid, new_uid) && gid_eq(old_gid, new_gid))
		return 0;

	error = device_change_owner(dev, new_uid, new_gid);
	if (error)
		return error;

	error = queue_change_owner(ndev, new_uid, new_gid);
	if (error)
		return error;

	return 0;
}

int netdev_class_create_file_ns(const struct class_attribute *class_attr,
				const void *ns)
{
	return class_create_file_ns(&net_class, class_attr, ns);
}
EXPORT_SYMBOL(netdev_class_create_file_ns);

void netdev_class_remove_file_ns(const struct class_attribute *class_attr,
				 const void *ns)
{
	class_remove_file_ns(&net_class, class_attr, ns);
}
EXPORT_SYMBOL(netdev_class_remove_file_ns);

int __init netdev_kobject_init(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	kobj_ns_type_register(&net_ns_type_operations);
	return class_register(&net_class);
}
