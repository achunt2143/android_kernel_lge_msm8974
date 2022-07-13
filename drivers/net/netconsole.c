<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  linux/drivers/net/netconsole.c
 *
 *  Copyright (C) 2001  Ingo Molnar <mingo@redhat.com>
 *
 *  This file contains the implementation of an IRQ-safe, crash-safe
 *  kernel console implementation that outputs kernel messages to the
 *  network.
 *
 * Modification history:
 *
 * 2001-09-17    started by Ingo Molnar.
 * 2003-08-11    2.6 port by Matt Mackall
 *               simplified options
 *               generic card hooks
 *               works non-modular
 * 2003-09-07    rewritten with netpoll api
 */

/****************************************************************
<<<<<<< HEAD
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2, or (at your option)
 *      any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 ****************************************************************/

=======
 *
 ****************************************************************/

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/mm.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/console.h>
#include <linux/moduleparam.h>
<<<<<<< HEAD
=======
#include <linux/kernel.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/string.h>
#include <linux/netpoll.h>
#include <linux/inet.h>
#include <linux/configfs.h>
<<<<<<< HEAD
=======
#include <linux/etherdevice.h>
#include <linux/utsname.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

MODULE_AUTHOR("Maintainer: Matt Mackall <mpm@selenic.com>");
MODULE_DESCRIPTION("Console driver for network interfaces");
MODULE_LICENSE("GPL");

<<<<<<< HEAD
#define MAX_PARAM_LENGTH	256
#define MAX_PRINT_CHUNK		1000
=======
#define MAX_PARAM_LENGTH		256
#define MAX_USERDATA_ENTRY_LENGTH	256
#define MAX_USERDATA_VALUE_LENGTH	200
/* The number 3 comes from userdata entry format characters (' ', '=', '\n') */
#define MAX_USERDATA_NAME_LENGTH	(MAX_USERDATA_ENTRY_LENGTH - \
					MAX_USERDATA_VALUE_LENGTH - 3)
#define MAX_USERDATA_ITEMS		16
#define MAX_PRINT_CHUNK			1000
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static char config[MAX_PARAM_LENGTH];
module_param_string(netconsole, config, MAX_PARAM_LENGTH, 0);
MODULE_PARM_DESC(netconsole, " netconsole=[src-port]@[src-ip]/[dev],[tgt-port]@<tgt-ip>/[tgt-macaddr]");

<<<<<<< HEAD
#ifndef	MODULE
static int __init option_setup(char *opt)
{
	strlcpy(config, opt, MAX_PARAM_LENGTH);
=======
static bool oops_only;
module_param(oops_only, bool, 0600);
MODULE_PARM_DESC(oops_only, "Only log oops messages");

#define NETCONSOLE_PARAM_TARGET_PREFIX "cmdline"

#ifndef	MODULE
static int __init option_setup(char *opt)
{
	strscpy(config, opt, MAX_PARAM_LENGTH);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 1;
}
__setup("netconsole=", option_setup);
#endif	/* MODULE */

/* Linked list of all configured targets */
static LIST_HEAD(target_list);

/* This needs to be a spinlock because write_msg() cannot sleep */
static DEFINE_SPINLOCK(target_list_lock);

<<<<<<< HEAD
/**
 * struct netconsole_target - Represents a configured netconsole target.
 * @list:	Links this target into the target_list.
 * @item:	Links us into the configfs subsystem hierarchy.
=======
/*
 * Console driver for extended netconsoles.  Registered on the first use to
 * avoid unnecessarily enabling ext message formatting.
 */
static struct console netconsole_ext;

/**
 * struct netconsole_target - Represents a configured netconsole target.
 * @list:	Links this target into the target_list.
 * @group:	Links us into the configfs subsystem hierarchy.
 * @userdata_group:	Links to the userdata configfs hierarchy
 * @userdata_complete:	Cached, formatted string of append
 * @userdata_length:	String length of userdata_complete
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @enabled:	On / off knob to enable / disable target.
 *		Visible from userspace (read-write).
 *		We maintain a strict 1:1 correspondence between this and
 *		whether the corresponding netpoll is active or inactive.
 *		Also, other parameters of a target may be modified at
 *		runtime only when it is disabled (enabled == 0).
<<<<<<< HEAD
=======
 * @extended:	Denotes whether console is extended or not.
 * @release:	Denotes whether kernel release version should be prepended
 *		to the message. Depends on extended console.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @np:		The netpoll structure for this target.
 *		Contains the other userspace visible parameters:
 *		dev_name	(read-write)
 *		local_port	(read-write)
 *		remote_port	(read-write)
 *		local_ip	(read-write)
 *		remote_ip	(read-write)
 *		local_mac	(read-only)
 *		remote_mac	(read-write)
 */
struct netconsole_target {
	struct list_head	list;
#ifdef	CONFIG_NETCONSOLE_DYNAMIC
<<<<<<< HEAD
	struct config_item	item;
#endif
	int			enabled;
=======
	struct config_group	group;
	struct config_group	userdata_group;
	char userdata_complete[MAX_USERDATA_ENTRY_LENGTH * MAX_USERDATA_ITEMS];
	size_t			userdata_length;
#endif
	bool			enabled;
	bool			extended;
	bool			release;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct netpoll		np;
};

#ifdef	CONFIG_NETCONSOLE_DYNAMIC

static struct configfs_subsystem netconsole_subsys;
<<<<<<< HEAD
=======
static DEFINE_MUTEX(dynamic_netconsole_mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int __init dynamic_netconsole_init(void)
{
	config_group_init(&netconsole_subsys.su_group);
	mutex_init(&netconsole_subsys.su_mutex);
	return configfs_register_subsystem(&netconsole_subsys);
}

static void __exit dynamic_netconsole_exit(void)
{
	configfs_unregister_subsystem(&netconsole_subsys);
}

/*
 * Targets that were created by parsing the boot/module option string
 * do not exist in the configfs hierarchy (and have NULL names) and will
 * never go away, so make these a no-op for them.
 */
static void netconsole_target_get(struct netconsole_target *nt)
{
<<<<<<< HEAD
	if (config_item_name(&nt->item))
		config_item_get(&nt->item);
=======
	if (config_item_name(&nt->group.cg_item))
		config_group_get(&nt->group);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void netconsole_target_put(struct netconsole_target *nt)
{
<<<<<<< HEAD
	if (config_item_name(&nt->item))
		config_item_put(&nt->item);
=======
	if (config_item_name(&nt->group.cg_item))
		config_group_put(&nt->group);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#else	/* !CONFIG_NETCONSOLE_DYNAMIC */

static int __init dynamic_netconsole_init(void)
{
	return 0;
}

static void __exit dynamic_netconsole_exit(void)
{
}

/*
 * No danger of targets going away from under us when dynamic
 * reconfigurability is off.
 */
static void netconsole_target_get(struct netconsole_target *nt)
{
}

static void netconsole_target_put(struct netconsole_target *nt)
{
}

<<<<<<< HEAD
#endif	/* CONFIG_NETCONSOLE_DYNAMIC */

/* Allocate new target (from boot/module param) and setup netpoll for it */
static struct netconsole_target *alloc_param_target(char *target_config)
{
	int err = -ENOMEM;
	struct netconsole_target *nt;

	/*
	 * Allocate and initialize with defaults.
	 * Note that these targets get their config_item fields zeroed-out.
	 */
	nt = kzalloc(sizeof(*nt), GFP_KERNEL);
	if (!nt)
		goto fail;

	nt->np.name = "netconsole";
	strlcpy(nt->np.dev_name, "eth0", IFNAMSIZ);
	nt->np.local_port = 6665;
	nt->np.remote_port = 6666;
	memset(nt->np.remote_mac, 0xff, ETH_ALEN);

	/* Parse parameters and setup netpoll */
	err = netpoll_parse_options(&nt->np, target_config);
	if (err)
		goto fail;

	err = netpoll_setup(&nt->np);
	if (err)
		goto fail;

	nt->enabled = 1;

	return nt;

fail:
	kfree(nt);
	return ERR_PTR(err);
}

/* Cleanup netpoll for given target (from boot/module param) and free it */
static void free_param_target(struct netconsole_target *nt)
{
	netpoll_cleanup(&nt->np);
	kfree(nt);
=======
static void populate_configfs_item(struct netconsole_target *nt,
				   int cmdline_count)
{
}
#endif	/* CONFIG_NETCONSOLE_DYNAMIC */

/* Allocate and initialize with defaults.
 * Note that these targets get their config_item fields zeroed-out.
 */
static struct netconsole_target *alloc_and_init(void)
{
	struct netconsole_target *nt;

	nt = kzalloc(sizeof(*nt), GFP_KERNEL);
	if (!nt)
		return nt;

	if (IS_ENABLED(CONFIG_NETCONSOLE_EXTENDED_LOG))
		nt->extended = true;
	if (IS_ENABLED(CONFIG_NETCONSOLE_PREPEND_RELEASE))
		nt->release = true;

	nt->np.name = "netconsole";
	strscpy(nt->np.dev_name, "eth0", IFNAMSIZ);
	nt->np.local_port = 6665;
	nt->np.remote_port = 6666;
	eth_broadcast_addr(nt->np.remote_mac);

	return nt;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#ifdef	CONFIG_NETCONSOLE_DYNAMIC

/*
 * Our subsystem hierarchy is:
 *
 * /sys/kernel/config/netconsole/
 *				|
 *				<target>/
 *				|	enabled
<<<<<<< HEAD
=======
 *				|	release
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *				|	dev_name
 *				|	local_port
 *				|	remote_port
 *				|	local_ip
 *				|	remote_ip
 *				|	local_mac
 *				|	remote_mac
<<<<<<< HEAD
=======
 *				|	userdata/
 *				|		<key>/
 *				|			value
 *				|		...
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *				|
 *				<target>/...
 */

<<<<<<< HEAD
struct netconsole_target_attr {
	struct configfs_attribute	attr;
	ssize_t				(*show)(struct netconsole_target *nt,
						char *buf);
	ssize_t				(*store)(struct netconsole_target *nt,
						 const char *buf,
						 size_t count);
};

static struct netconsole_target *to_target(struct config_item *item)
{
	return item ?
		container_of(item, struct netconsole_target, item) :
		NULL;
=======
static struct netconsole_target *to_target(struct config_item *item)
{
	struct config_group *cfg_group;

	cfg_group = to_config_group(item);
	if (!cfg_group)
		return NULL;
	return container_of(to_config_group(item),
			    struct netconsole_target, group);
}

/* Get rid of possible trailing newline, returning the new length */
static void trim_newline(char *s, size_t maxlen)
{
	size_t len;

	len = strnlen(s, maxlen);
	if (s[len - 1] == '\n')
		s[len - 1] = '\0';
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Attribute operations for netconsole_target.
 */

<<<<<<< HEAD
static ssize_t show_enabled(struct netconsole_target *nt, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%d\n", nt->enabled);
}

static ssize_t show_dev_name(struct netconsole_target *nt, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%s\n", nt->np.dev_name);
}

static ssize_t show_local_port(struct netconsole_target *nt, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%d\n", nt->np.local_port);
}

static ssize_t show_remote_port(struct netconsole_target *nt, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%d\n", nt->np.remote_port);
}

static ssize_t show_local_ip(struct netconsole_target *nt, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%pI4\n", &nt->np.local_ip);
}

static ssize_t show_remote_ip(struct netconsole_target *nt, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%pI4\n", &nt->np.remote_ip);
}

static ssize_t show_local_mac(struct netconsole_target *nt, char *buf)
{
	struct net_device *dev = nt->np.dev;
	static const u8 bcast[ETH_ALEN] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

	return snprintf(buf, PAGE_SIZE, "%pM\n", dev ? dev->dev_addr : bcast);
}

static ssize_t show_remote_mac(struct netconsole_target *nt, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%pM\n", nt->np.remote_mac);
=======
static ssize_t enabled_show(struct config_item *item, char *buf)
{
	return sysfs_emit(buf, "%d\n", to_target(item)->enabled);
}

static ssize_t extended_show(struct config_item *item, char *buf)
{
	return sysfs_emit(buf, "%d\n", to_target(item)->extended);
}

static ssize_t release_show(struct config_item *item, char *buf)
{
	return sysfs_emit(buf, "%d\n", to_target(item)->release);
}

static ssize_t dev_name_show(struct config_item *item, char *buf)
{
	return sysfs_emit(buf, "%s\n", to_target(item)->np.dev_name);
}

static ssize_t local_port_show(struct config_item *item, char *buf)
{
	return sysfs_emit(buf, "%d\n", to_target(item)->np.local_port);
}

static ssize_t remote_port_show(struct config_item *item, char *buf)
{
	return sysfs_emit(buf, "%d\n", to_target(item)->np.remote_port);
}

static ssize_t local_ip_show(struct config_item *item, char *buf)
{
	struct netconsole_target *nt = to_target(item);

	if (nt->np.ipv6)
		return sysfs_emit(buf, "%pI6c\n", &nt->np.local_ip.in6);
	else
		return sysfs_emit(buf, "%pI4\n", &nt->np.local_ip);
}

static ssize_t remote_ip_show(struct config_item *item, char *buf)
{
	struct netconsole_target *nt = to_target(item);

	if (nt->np.ipv6)
		return sysfs_emit(buf, "%pI6c\n", &nt->np.remote_ip.in6);
	else
		return sysfs_emit(buf, "%pI4\n", &nt->np.remote_ip);
}

static ssize_t local_mac_show(struct config_item *item, char *buf)
{
	struct net_device *dev = to_target(item)->np.dev;
	static const u8 bcast[ETH_ALEN] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

	return sysfs_emit(buf, "%pM\n", dev ? dev->dev_addr : bcast);
}

static ssize_t remote_mac_show(struct config_item *item, char *buf)
{
	return sysfs_emit(buf, "%pM\n", to_target(item)->np.remote_mac);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * This one is special -- targets created through the configfs interface
 * are not enabled (and the corresponding netpoll activated) by default.
 * The user is expected to set the desired parameters first (which
 * would enable him to dynamically add new netpoll targets for new
 * network interfaces as and when they come up).
 */
<<<<<<< HEAD
static ssize_t store_enabled(struct netconsole_target *nt,
			     const char *buf,
			     size_t count)
{
	int enabled;
	int err;

	err = kstrtoint(buf, 10, &enabled);
	if (err < 0)
		return err;
	if (enabled < 0 || enabled > 1)
		return -EINVAL;
	if (enabled == nt->enabled) {
		printk(KERN_INFO "netconsole: network logging has already %s\n",
				nt->enabled ? "started" : "stopped");
		return -EINVAL;
	}

	if (enabled) {	/* 1 */
=======
static ssize_t enabled_store(struct config_item *item,
		const char *buf, size_t count)
{
	struct netconsole_target *nt = to_target(item);
	unsigned long flags;
	bool enabled;
	int err;

	mutex_lock(&dynamic_netconsole_mutex);
	err = kstrtobool(buf, &enabled);
	if (err)
		goto out_unlock;

	err = -EINVAL;
	if ((bool)enabled == nt->enabled) {
		pr_info("network logging has already %s\n",
			nt->enabled ? "started" : "stopped");
		goto out_unlock;
	}

	if (enabled) {	/* true */
		if (nt->release && !nt->extended) {
			pr_err("Not enabling netconsole. Release feature requires extended log message");
			goto out_unlock;
		}

		if (nt->extended && !console_is_registered(&netconsole_ext))
			register_console(&netconsole_ext);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/*
		 * Skip netpoll_parse_options() -- all the attributes are
		 * already configured via configfs. Just print them out.
		 */
		netpoll_print_options(&nt->np);

		err = netpoll_setup(&nt->np);
		if (err)
<<<<<<< HEAD
			return err;

		printk(KERN_INFO "netconsole: network logging started\n");

	} else {	/* 0 */
=======
			goto out_unlock;

		pr_info("network logging started\n");
	} else {	/* false */
		/* We need to disable the netconsole before cleaning it up
		 * otherwise we might end up in write_msg() with
		 * nt->np.dev == NULL and nt->enabled == true
		 */
		spin_lock_irqsave(&target_list_lock, flags);
		nt->enabled = false;
		spin_unlock_irqrestore(&target_list_lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		netpoll_cleanup(&nt->np);
	}

	nt->enabled = enabled;

<<<<<<< HEAD
	return strnlen(buf, count);
}

static ssize_t store_dev_name(struct netconsole_target *nt,
			      const char *buf,
			      size_t count)
{
	size_t len;

	if (nt->enabled) {
		printk(KERN_ERR "netconsole: target (%s) is enabled, "
				"disable to update parameters\n",
				config_item_name(&nt->item));
		return -EINVAL;
	}

	strlcpy(nt->np.dev_name, buf, IFNAMSIZ);

	/* Get rid of possible trailing newline from echo(1) */
	len = strnlen(nt->np.dev_name, IFNAMSIZ);
	if (nt->np.dev_name[len - 1] == '\n')
		nt->np.dev_name[len - 1] = '\0';

	return strnlen(buf, count);
}

static ssize_t store_local_port(struct netconsole_target *nt,
				const char *buf,
				size_t count)
{
	int rv;

	if (nt->enabled) {
		printk(KERN_ERR "netconsole: target (%s) is enabled, "
				"disable to update parameters\n",
				config_item_name(&nt->item));
		return -EINVAL;
=======
	mutex_unlock(&dynamic_netconsole_mutex);
	return strnlen(buf, count);
out_unlock:
	mutex_unlock(&dynamic_netconsole_mutex);
	return err;
}

static ssize_t release_store(struct config_item *item, const char *buf,
			     size_t count)
{
	struct netconsole_target *nt = to_target(item);
	bool release;
	int err;

	mutex_lock(&dynamic_netconsole_mutex);
	if (nt->enabled) {
		pr_err("target (%s) is enabled, disable to update parameters\n",
		       config_item_name(&nt->group.cg_item));
		err = -EINVAL;
		goto out_unlock;
	}

	err = kstrtobool(buf, &release);
	if (err)
		goto out_unlock;

	nt->release = release;

	mutex_unlock(&dynamic_netconsole_mutex);
	return strnlen(buf, count);
out_unlock:
	mutex_unlock(&dynamic_netconsole_mutex);
	return err;
}

static ssize_t extended_store(struct config_item *item, const char *buf,
		size_t count)
{
	struct netconsole_target *nt = to_target(item);
	bool extended;
	int err;

	mutex_lock(&dynamic_netconsole_mutex);
	if (nt->enabled) {
		pr_err("target (%s) is enabled, disable to update parameters\n",
		       config_item_name(&nt->group.cg_item));
		err = -EINVAL;
		goto out_unlock;
	}

	err = kstrtobool(buf, &extended);
	if (err)
		goto out_unlock;

	nt->extended = extended;

	mutex_unlock(&dynamic_netconsole_mutex);
	return strnlen(buf, count);
out_unlock:
	mutex_unlock(&dynamic_netconsole_mutex);
	return err;
}

static ssize_t dev_name_store(struct config_item *item, const char *buf,
		size_t count)
{
	struct netconsole_target *nt = to_target(item);

	mutex_lock(&dynamic_netconsole_mutex);
	if (nt->enabled) {
		pr_err("target (%s) is enabled, disable to update parameters\n",
		       config_item_name(&nt->group.cg_item));
		mutex_unlock(&dynamic_netconsole_mutex);
		return -EINVAL;
	}

	strscpy(nt->np.dev_name, buf, IFNAMSIZ);
	trim_newline(nt->np.dev_name, IFNAMSIZ);

	mutex_unlock(&dynamic_netconsole_mutex);
	return strnlen(buf, count);
}

static ssize_t local_port_store(struct config_item *item, const char *buf,
		size_t count)
{
	struct netconsole_target *nt = to_target(item);
	int rv = -EINVAL;

	mutex_lock(&dynamic_netconsole_mutex);
	if (nt->enabled) {
		pr_err("target (%s) is enabled, disable to update parameters\n",
		       config_item_name(&nt->group.cg_item));
		goto out_unlock;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	rv = kstrtou16(buf, 10, &nt->np.local_port);
	if (rv < 0)
<<<<<<< HEAD
		return rv;
	return strnlen(buf, count);
}

static ssize_t store_remote_port(struct netconsole_target *nt,
				 const char *buf,
				 size_t count)
{
	int rv;

	if (nt->enabled) {
		printk(KERN_ERR "netconsole: target (%s) is enabled, "
				"disable to update parameters\n",
				config_item_name(&nt->item));
		return -EINVAL;
=======
		goto out_unlock;
	mutex_unlock(&dynamic_netconsole_mutex);
	return strnlen(buf, count);
out_unlock:
	mutex_unlock(&dynamic_netconsole_mutex);
	return rv;
}

static ssize_t remote_port_store(struct config_item *item,
		const char *buf, size_t count)
{
	struct netconsole_target *nt = to_target(item);
	int rv = -EINVAL;

	mutex_lock(&dynamic_netconsole_mutex);
	if (nt->enabled) {
		pr_err("target (%s) is enabled, disable to update parameters\n",
		       config_item_name(&nt->group.cg_item));
		goto out_unlock;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	rv = kstrtou16(buf, 10, &nt->np.remote_port);
	if (rv < 0)
<<<<<<< HEAD
		return rv;
	return strnlen(buf, count);
}

static ssize_t store_local_ip(struct netconsole_target *nt,
			      const char *buf,
			      size_t count)
{
	if (nt->enabled) {
		printk(KERN_ERR "netconsole: target (%s) is enabled, "
				"disable to update parameters\n",
				config_item_name(&nt->item));
		return -EINVAL;
	}

	nt->np.local_ip = in_aton(buf);

	return strnlen(buf, count);
}

static ssize_t store_remote_ip(struct netconsole_target *nt,
			       const char *buf,
			       size_t count)
{
	if (nt->enabled) {
		printk(KERN_ERR "netconsole: target (%s) is enabled, "
				"disable to update parameters\n",
				config_item_name(&nt->item));
		return -EINVAL;
	}

	nt->np.remote_ip = in_aton(buf);

	return strnlen(buf, count);
}

static ssize_t store_remote_mac(struct netconsole_target *nt,
				const char *buf,
				size_t count)
{
	u8 remote_mac[ETH_ALEN];

	if (nt->enabled) {
		printk(KERN_ERR "netconsole: target (%s) is enabled, "
				"disable to update parameters\n",
				config_item_name(&nt->item));
		return -EINVAL;
	}

	if (!mac_pton(buf, remote_mac))
		return -EINVAL;
	if (buf[3 * ETH_ALEN - 1] && buf[3 * ETH_ALEN - 1] != '\n')
		return -EINVAL;
	memcpy(nt->np.remote_mac, remote_mac, ETH_ALEN);

	return strnlen(buf, count);
}

/*
 * Attribute definitions for netconsole_target.
 */

#define NETCONSOLE_TARGET_ATTR_RO(_name)				\
static struct netconsole_target_attr netconsole_target_##_name =	\
	__CONFIGFS_ATTR(_name, S_IRUGO, show_##_name, NULL)

#define NETCONSOLE_TARGET_ATTR_RW(_name)				\
static struct netconsole_target_attr netconsole_target_##_name =	\
	__CONFIGFS_ATTR(_name, S_IRUGO | S_IWUSR, show_##_name, store_##_name)

NETCONSOLE_TARGET_ATTR_RW(enabled);
NETCONSOLE_TARGET_ATTR_RW(dev_name);
NETCONSOLE_TARGET_ATTR_RW(local_port);
NETCONSOLE_TARGET_ATTR_RW(remote_port);
NETCONSOLE_TARGET_ATTR_RW(local_ip);
NETCONSOLE_TARGET_ATTR_RW(remote_ip);
NETCONSOLE_TARGET_ATTR_RO(local_mac);
NETCONSOLE_TARGET_ATTR_RW(remote_mac);

static struct configfs_attribute *netconsole_target_attrs[] = {
	&netconsole_target_enabled.attr,
	&netconsole_target_dev_name.attr,
	&netconsole_target_local_port.attr,
	&netconsole_target_remote_port.attr,
	&netconsole_target_local_ip.attr,
	&netconsole_target_remote_ip.attr,
	&netconsole_target_local_mac.attr,
	&netconsole_target_remote_mac.attr,
=======
		goto out_unlock;
	mutex_unlock(&dynamic_netconsole_mutex);
	return strnlen(buf, count);
out_unlock:
	mutex_unlock(&dynamic_netconsole_mutex);
	return rv;
}

static ssize_t local_ip_store(struct config_item *item, const char *buf,
		size_t count)
{
	struct netconsole_target *nt = to_target(item);

	mutex_lock(&dynamic_netconsole_mutex);
	if (nt->enabled) {
		pr_err("target (%s) is enabled, disable to update parameters\n",
		       config_item_name(&nt->group.cg_item));
		goto out_unlock;
	}

	if (strnchr(buf, count, ':')) {
		const char *end;

		if (in6_pton(buf, count, nt->np.local_ip.in6.s6_addr, -1, &end) > 0) {
			if (*end && *end != '\n') {
				pr_err("invalid IPv6 address at: <%c>\n", *end);
				goto out_unlock;
			}
			nt->np.ipv6 = true;
		} else
			goto out_unlock;
	} else {
		if (!nt->np.ipv6)
			nt->np.local_ip.ip = in_aton(buf);
		else
			goto out_unlock;
	}

	mutex_unlock(&dynamic_netconsole_mutex);
	return strnlen(buf, count);
out_unlock:
	mutex_unlock(&dynamic_netconsole_mutex);
	return -EINVAL;
}

static ssize_t remote_ip_store(struct config_item *item, const char *buf,
	       size_t count)
{
	struct netconsole_target *nt = to_target(item);

	mutex_lock(&dynamic_netconsole_mutex);
	if (nt->enabled) {
		pr_err("target (%s) is enabled, disable to update parameters\n",
		       config_item_name(&nt->group.cg_item));
		goto out_unlock;
	}

	if (strnchr(buf, count, ':')) {
		const char *end;

		if (in6_pton(buf, count, nt->np.remote_ip.in6.s6_addr, -1, &end) > 0) {
			if (*end && *end != '\n') {
				pr_err("invalid IPv6 address at: <%c>\n", *end);
				goto out_unlock;
			}
			nt->np.ipv6 = true;
		} else
			goto out_unlock;
	} else {
		if (!nt->np.ipv6)
			nt->np.remote_ip.ip = in_aton(buf);
		else
			goto out_unlock;
	}

	mutex_unlock(&dynamic_netconsole_mutex);
	return strnlen(buf, count);
out_unlock:
	mutex_unlock(&dynamic_netconsole_mutex);
	return -EINVAL;
}

static ssize_t remote_mac_store(struct config_item *item, const char *buf,
		size_t count)
{
	struct netconsole_target *nt = to_target(item);
	u8 remote_mac[ETH_ALEN];

	mutex_lock(&dynamic_netconsole_mutex);
	if (nt->enabled) {
		pr_err("target (%s) is enabled, disable to update parameters\n",
		       config_item_name(&nt->group.cg_item));
		goto out_unlock;
	}

	if (!mac_pton(buf, remote_mac))
		goto out_unlock;
	if (buf[3 * ETH_ALEN - 1] && buf[3 * ETH_ALEN - 1] != '\n')
		goto out_unlock;
	memcpy(nt->np.remote_mac, remote_mac, ETH_ALEN);

	mutex_unlock(&dynamic_netconsole_mutex);
	return strnlen(buf, count);
out_unlock:
	mutex_unlock(&dynamic_netconsole_mutex);
	return -EINVAL;
}

struct userdatum {
	struct config_item item;
	char value[MAX_USERDATA_VALUE_LENGTH];
};

static struct userdatum *to_userdatum(struct config_item *item)
{
	return container_of(item, struct userdatum, item);
}

struct userdata {
	struct config_group group;
};

static struct userdata *to_userdata(struct config_item *item)
{
	return container_of(to_config_group(item), struct userdata, group);
}

static struct netconsole_target *userdata_to_target(struct userdata *ud)
{
	struct config_group *netconsole_group;

	netconsole_group = to_config_group(ud->group.cg_item.ci_parent);
	return to_target(&netconsole_group->cg_item);
}

static ssize_t userdatum_value_show(struct config_item *item, char *buf)
{
	return sysfs_emit(buf, "%s\n", &(to_userdatum(item)->value[0]));
}

static void update_userdata(struct netconsole_target *nt)
{
	int complete_idx = 0, child_count = 0;
	struct list_head *entry;

	/* Clear the current string in case the last userdatum was deleted */
	nt->userdata_length = 0;
	nt->userdata_complete[0] = 0;

	list_for_each(entry, &nt->userdata_group.cg_children) {
		struct userdatum *udm_item;
		struct config_item *item;

		if (child_count >= MAX_USERDATA_ITEMS)
			break;
		child_count++;

		item = container_of(entry, struct config_item, ci_entry);
		udm_item = to_userdatum(item);

		/* Skip userdata with no value set */
		if (strnlen(udm_item->value, MAX_USERDATA_VALUE_LENGTH) == 0)
			continue;

		/* This doesn't overflow userdata_complete since it will write
		 * one entry length (1/MAX_USERDATA_ITEMS long), entry count is
		 * checked to not exceed MAX items with child_count above
		 */
		complete_idx += scnprintf(&nt->userdata_complete[complete_idx],
					  MAX_USERDATA_ENTRY_LENGTH, " %s=%s\n",
					  item->ci_name, udm_item->value);
	}
	nt->userdata_length = strnlen(nt->userdata_complete,
				      sizeof(nt->userdata_complete));
}

static ssize_t userdatum_value_store(struct config_item *item, const char *buf,
				     size_t count)
{
	struct userdatum *udm = to_userdatum(item);
	struct netconsole_target *nt;
	struct userdata *ud;
	int ret;

	if (count > MAX_USERDATA_VALUE_LENGTH)
		return -EMSGSIZE;

	mutex_lock(&dynamic_netconsole_mutex);

	ret = strscpy(udm->value, buf, sizeof(udm->value));
	if (ret < 0)
		goto out_unlock;
	trim_newline(udm->value, sizeof(udm->value));

	ud = to_userdata(item->ci_parent);
	nt = userdata_to_target(ud);
	update_userdata(nt);

	mutex_unlock(&dynamic_netconsole_mutex);
	return count;
out_unlock:
	mutex_unlock(&dynamic_netconsole_mutex);
	return ret;
}

CONFIGFS_ATTR(userdatum_, value);

static struct configfs_attribute *userdatum_attrs[] = {
	&userdatum_attr_value,
	NULL,
};

static void userdatum_release(struct config_item *item)
{
	kfree(to_userdatum(item));
}

static struct configfs_item_operations userdatum_ops = {
	.release = userdatum_release,
};

static const struct config_item_type userdatum_type = {
	.ct_item_ops	= &userdatum_ops,
	.ct_attrs	= userdatum_attrs,
	.ct_owner	= THIS_MODULE,
};

static struct config_item *userdatum_make_item(struct config_group *group,
					       const char *name)
{
	struct netconsole_target *nt;
	struct userdatum *udm;
	struct userdata *ud;
	size_t child_count;

	if (strlen(name) > MAX_USERDATA_NAME_LENGTH)
		return ERR_PTR(-ENAMETOOLONG);

	ud = to_userdata(&group->cg_item);
	nt = userdata_to_target(ud);
	child_count = list_count_nodes(&nt->userdata_group.cg_children);
	if (child_count >= MAX_USERDATA_ITEMS)
		return ERR_PTR(-ENOSPC);

	udm = kzalloc(sizeof(*udm), GFP_KERNEL);
	if (!udm)
		return ERR_PTR(-ENOMEM);

	config_item_init_type_name(&udm->item, name, &userdatum_type);
	return &udm->item;
}

static void userdatum_drop(struct config_group *group, struct config_item *item)
{
	struct netconsole_target *nt;
	struct userdata *ud;

	ud = to_userdata(&group->cg_item);
	nt = userdata_to_target(ud);

	mutex_lock(&dynamic_netconsole_mutex);
	update_userdata(nt);
	config_item_put(item);
	mutex_unlock(&dynamic_netconsole_mutex);
}

static struct configfs_attribute *userdata_attrs[] = {
	NULL,
};

static struct configfs_group_operations userdata_ops = {
	.make_item		= userdatum_make_item,
	.drop_item		= userdatum_drop,
};

static struct config_item_type userdata_type = {
	.ct_item_ops	= &userdatum_ops,
	.ct_group_ops	= &userdata_ops,
	.ct_attrs	= userdata_attrs,
	.ct_owner	= THIS_MODULE,
};

CONFIGFS_ATTR(, enabled);
CONFIGFS_ATTR(, extended);
CONFIGFS_ATTR(, dev_name);
CONFIGFS_ATTR(, local_port);
CONFIGFS_ATTR(, remote_port);
CONFIGFS_ATTR(, local_ip);
CONFIGFS_ATTR(, remote_ip);
CONFIGFS_ATTR_RO(, local_mac);
CONFIGFS_ATTR(, remote_mac);
CONFIGFS_ATTR(, release);

static struct configfs_attribute *netconsole_target_attrs[] = {
	&attr_enabled,
	&attr_extended,
	&attr_release,
	&attr_dev_name,
	&attr_local_port,
	&attr_remote_port,
	&attr_local_ip,
	&attr_remote_ip,
	&attr_local_mac,
	&attr_remote_mac,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	NULL,
};

/*
 * Item operations and type for netconsole_target.
 */

static void netconsole_target_release(struct config_item *item)
{
	kfree(to_target(item));
}

<<<<<<< HEAD
static ssize_t netconsole_target_attr_show(struct config_item *item,
					   struct configfs_attribute *attr,
					   char *buf)
{
	ssize_t ret = -EINVAL;
	struct netconsole_target *nt = to_target(item);
	struct netconsole_target_attr *na =
		container_of(attr, struct netconsole_target_attr, attr);

	if (na->show)
		ret = na->show(nt, buf);

	return ret;
}

static ssize_t netconsole_target_attr_store(struct config_item *item,
					    struct configfs_attribute *attr,
					    const char *buf,
					    size_t count)
{
	ssize_t ret = -EINVAL;
	struct netconsole_target *nt = to_target(item);
	struct netconsole_target_attr *na =
		container_of(attr, struct netconsole_target_attr, attr);

	if (na->store)
		ret = na->store(nt, buf, count);

	return ret;
}

static struct configfs_item_operations netconsole_target_item_ops = {
	.release		= netconsole_target_release,
	.show_attribute		= netconsole_target_attr_show,
	.store_attribute	= netconsole_target_attr_store,
};

static struct config_item_type netconsole_target_type = {
=======
static struct configfs_item_operations netconsole_target_item_ops = {
	.release		= netconsole_target_release,
};

static const struct config_item_type netconsole_target_type = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.ct_attrs		= netconsole_target_attrs,
	.ct_item_ops		= &netconsole_target_item_ops,
	.ct_owner		= THIS_MODULE,
};

<<<<<<< HEAD
=======
static void init_target_config_group(struct netconsole_target *nt,
				     const char *name)
{
	config_group_init_type_name(&nt->group, name, &netconsole_target_type);
	config_group_init_type_name(&nt->userdata_group, "userdata",
				    &userdata_type);
	configfs_add_default_group(&nt->userdata_group, &nt->group);
}

static struct netconsole_target *find_cmdline_target(const char *name)
{
	struct netconsole_target *nt, *ret = NULL;
	unsigned long flags;

	spin_lock_irqsave(&target_list_lock, flags);
	list_for_each_entry(nt, &target_list, list) {
		if (!strcmp(nt->group.cg_item.ci_name, name)) {
			ret = nt;
			break;
		}
	}
	spin_unlock_irqrestore(&target_list_lock, flags);

	return ret;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Group operations and type for netconsole_subsys.
 */

<<<<<<< HEAD
static struct config_item *make_netconsole_target(struct config_group *group,
						  const char *name)
{
	unsigned long flags;
	struct netconsole_target *nt;

	/*
	 * Allocate and initialize with defaults.
	 * Target is disabled at creation (enabled == 0).
	 */
	nt = kzalloc(sizeof(*nt), GFP_KERNEL);
	if (!nt)
		return ERR_PTR(-ENOMEM);

	nt->np.name = "netconsole";
	strlcpy(nt->np.dev_name, "eth0", IFNAMSIZ);
	nt->np.local_port = 6665;
	nt->np.remote_port = 6666;
	memset(nt->np.remote_mac, 0xff, ETH_ALEN);

	/* Initialize the config_item member */
	config_item_init_type_name(&nt->item, name, &netconsole_target_type);
=======
static struct config_group *make_netconsole_target(struct config_group *group,
						   const char *name)
{
	struct netconsole_target *nt;
	unsigned long flags;

	/* Checking if a target by this name was created at boot time.  If so,
	 * attach a configfs entry to that target.  This enables dynamic
	 * control.
	 */
	if (!strncmp(name, NETCONSOLE_PARAM_TARGET_PREFIX,
		     strlen(NETCONSOLE_PARAM_TARGET_PREFIX))) {
		nt = find_cmdline_target(name);
		if (nt) {
			init_target_config_group(nt, name);
			return &nt->group;
		}
	}

	nt = alloc_and_init();
	if (!nt)
		return ERR_PTR(-ENOMEM);

	/* Initialize the config_group member */
	init_target_config_group(nt, name);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Adding, but it is disabled */
	spin_lock_irqsave(&target_list_lock, flags);
	list_add(&nt->list, &target_list);
	spin_unlock_irqrestore(&target_list_lock, flags);

<<<<<<< HEAD
	return &nt->item;
=======
	return &nt->group;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void drop_netconsole_target(struct config_group *group,
				   struct config_item *item)
{
	unsigned long flags;
	struct netconsole_target *nt = to_target(item);

	spin_lock_irqsave(&target_list_lock, flags);
	list_del(&nt->list);
	spin_unlock_irqrestore(&target_list_lock, flags);

	/*
	 * The target may have never been enabled, or was manually disabled
	 * before being removed so netpoll may have already been cleaned up.
	 */
	if (nt->enabled)
		netpoll_cleanup(&nt->np);

<<<<<<< HEAD
	config_item_put(&nt->item);
}

static struct configfs_group_operations netconsole_subsys_group_ops = {
	.make_item	= make_netconsole_target,
	.drop_item	= drop_netconsole_target,
};

static struct config_item_type netconsole_subsys_type = {
=======
	config_item_put(&nt->group.cg_item);
}

static struct configfs_group_operations netconsole_subsys_group_ops = {
	.make_group	= make_netconsole_target,
	.drop_item	= drop_netconsole_target,
};

static const struct config_item_type netconsole_subsys_type = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.ct_group_ops	= &netconsole_subsys_group_ops,
	.ct_owner	= THIS_MODULE,
};

/* The netconsole configfs subsystem */
static struct configfs_subsystem netconsole_subsys = {
	.su_group	= {
		.cg_item	= {
			.ci_namebuf	= "netconsole",
			.ci_type	= &netconsole_subsys_type,
		},
	},
};

<<<<<<< HEAD
=======
static void populate_configfs_item(struct netconsole_target *nt,
				   int cmdline_count)
{
	char target_name[16];

	snprintf(target_name, sizeof(target_name), "%s%d",
		 NETCONSOLE_PARAM_TARGET_PREFIX, cmdline_count);
	init_target_config_group(nt, target_name);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif	/* CONFIG_NETCONSOLE_DYNAMIC */

/* Handle network interface device notifications */
static int netconsole_netdev_event(struct notifier_block *this,
<<<<<<< HEAD
				   unsigned long event,
				   void *ptr)
{
	unsigned long flags;
	struct netconsole_target *nt;
	struct net_device *dev = ptr;
=======
				   unsigned long event, void *ptr)
{
	unsigned long flags;
	struct netconsole_target *nt;
	struct net_device *dev = netdev_notifier_info_to_dev(ptr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	bool stopped = false;

	if (!(event == NETDEV_CHANGENAME || event == NETDEV_UNREGISTER ||
	      event == NETDEV_RELEASE || event == NETDEV_JOIN))
		goto done;

	spin_lock_irqsave(&target_list_lock, flags);
restart:
	list_for_each_entry(nt, &target_list, list) {
		netconsole_target_get(nt);
		if (nt->np.dev == dev) {
			switch (event) {
			case NETDEV_CHANGENAME:
<<<<<<< HEAD
				strlcpy(nt->np.dev_name, dev->name, IFNAMSIZ);
=======
				strscpy(nt->np.dev_name, dev->name, IFNAMSIZ);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				break;
			case NETDEV_RELEASE:
			case NETDEV_JOIN:
			case NETDEV_UNREGISTER:
<<<<<<< HEAD
				/*
				 * we might sleep in __netpoll_cleanup()
				 * rtnl_lock already held
				 */
				spin_unlock_irqrestore(&target_list_lock, flags);
				__netpoll_cleanup(&nt->np);
				spin_lock_irqsave(&target_list_lock, flags);
				dev_put(nt->np.dev);
				nt->np.dev = NULL;
				nt->enabled = 0;
=======
				/* rtnl_lock already held
				 * we might sleep in __netpoll_cleanup()
				 */
				spin_unlock_irqrestore(&target_list_lock, flags);

				__netpoll_cleanup(&nt->np);

				spin_lock_irqsave(&target_list_lock, flags);
				netdev_put(nt->np.dev, &nt->np.dev_tracker);
				nt->np.dev = NULL;
				nt->enabled = false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				stopped = true;
				netconsole_target_put(nt);
				goto restart;
			}
		}
		netconsole_target_put(nt);
	}
	spin_unlock_irqrestore(&target_list_lock, flags);
	if (stopped) {
<<<<<<< HEAD
		printk(KERN_INFO "netconsole: network logging stopped on "
		       "interface %s as it ", dev->name);
		switch (event) {
		case NETDEV_UNREGISTER:
			printk(KERN_CONT "unregistered\n");
			break;
		case NETDEV_RELEASE:
			printk(KERN_CONT "released slaves\n");
			break;
		case NETDEV_JOIN:
			printk(KERN_CONT "is joining a master device\n");
			break;
		}
=======
		const char *msg = "had an event";

		switch (event) {
		case NETDEV_UNREGISTER:
			msg = "unregistered";
			break;
		case NETDEV_RELEASE:
			msg = "released slaves";
			break;
		case NETDEV_JOIN:
			msg = "is joining a master device";
			break;
		}
		pr_info("network logging stopped on interface %s as it %s\n",
			dev->name, msg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

done:
	return NOTIFY_DONE;
}

static struct notifier_block netconsole_netdev_notifier = {
	.notifier_call  = netconsole_netdev_event,
};

<<<<<<< HEAD
=======
/**
 * send_ext_msg_udp - send extended log message to target
 * @nt: target to send message to
 * @msg: extended log message to send
 * @msg_len: length of message
 *
 * Transfer extended log @msg to @nt.  If @msg is longer than
 * MAX_PRINT_CHUNK, it'll be split and transmitted in multiple chunks with
 * ncfrag header field added to identify them.
 */
static void send_ext_msg_udp(struct netconsole_target *nt, const char *msg,
			     int msg_len)
{
	static char buf[MAX_PRINT_CHUNK]; /* protected by target_list_lock */
	const char *header, *body;
	int offset = 0;
	int header_len, body_len;
	const char *msg_ready = msg;
	const char *release;
	int release_len = 0;
	int userdata_len = 0;
	char *userdata = NULL;

#ifdef CONFIG_NETCONSOLE_DYNAMIC
	userdata = nt->userdata_complete;
	userdata_len = nt->userdata_length;
#endif

	if (nt->release) {
		release = init_utsname()->release;
		release_len = strlen(release) + 1;
	}

	if (msg_len + release_len + userdata_len <= MAX_PRINT_CHUNK) {
		/* No fragmentation needed */
		if (nt->release) {
			scnprintf(buf, MAX_PRINT_CHUNK, "%s,%s", release, msg);
			msg_len += release_len;
		} else {
			memcpy(buf, msg, msg_len);
		}

		if (userdata)
			msg_len += scnprintf(&buf[msg_len],
					     MAX_PRINT_CHUNK - msg_len,
					     "%s", userdata);

		msg_ready = buf;
		netpoll_send_udp(&nt->np, msg_ready, msg_len);
		return;
	}

	/* need to insert extra header fields, detect header and body */
	header = msg;
	body = memchr(msg, ';', msg_len);
	if (WARN_ON_ONCE(!body))
		return;

	header_len = body - header;
	body_len = msg_len - header_len - 1;
	body++;

	/*
	 * Transfer multiple chunks with the following extra header.
	 * "ncfrag=<byte-offset>/<total-bytes>"
	 */
	if (nt->release)
		scnprintf(buf, MAX_PRINT_CHUNK, "%s,", release);
	memcpy(buf + release_len, header, header_len);
	header_len += release_len;

	while (offset < body_len + userdata_len) {
		int this_header = header_len;
		int this_offset = 0;
		int this_chunk = 0;

		this_header += scnprintf(buf + this_header,
					 sizeof(buf) - this_header,
					 ",ncfrag=%d/%d;", offset,
					 body_len + userdata_len);

		/* Not all body data has been written yet */
		if (offset < body_len) {
			this_chunk = min(body_len - offset,
					 MAX_PRINT_CHUNK - this_header);
			if (WARN_ON_ONCE(this_chunk <= 0))
				return;
			memcpy(buf + this_header, body + offset, this_chunk);
			this_offset += this_chunk;
		}
		/* Body is fully written and there is pending userdata to write,
		 * append userdata in this chunk
		 */
		if (offset + this_offset >= body_len &&
		    offset + this_offset < userdata_len + body_len) {
			int sent_userdata = (offset + this_offset) - body_len;
			int preceding_bytes = this_chunk + this_header;

			if (WARN_ON_ONCE(sent_userdata < 0))
				return;

			this_chunk = min(userdata_len - sent_userdata,
					 MAX_PRINT_CHUNK - preceding_bytes);
			if (WARN_ON_ONCE(this_chunk <= 0))
				return;
			memcpy(buf + this_header + this_offset,
			       userdata + sent_userdata,
			       this_chunk);
			this_offset += this_chunk;
		}

		netpoll_send_udp(&nt->np, buf, this_header + this_offset);
		offset += this_offset;
	}
}

static void write_ext_msg(struct console *con, const char *msg,
			  unsigned int len)
{
	struct netconsole_target *nt;
	unsigned long flags;

	if ((oops_only && !oops_in_progress) || list_empty(&target_list))
		return;

	spin_lock_irqsave(&target_list_lock, flags);
	list_for_each_entry(nt, &target_list, list)
		if (nt->extended && nt->enabled && netif_running(nt->np.dev))
			send_ext_msg_udp(nt, msg, len);
	spin_unlock_irqrestore(&target_list_lock, flags);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void write_msg(struct console *con, const char *msg, unsigned int len)
{
	int frag, left;
	unsigned long flags;
	struct netconsole_target *nt;
	const char *tmp;

<<<<<<< HEAD
=======
	if (oops_only && !oops_in_progress)
		return;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Avoid taking lock and disabling interrupts unnecessarily */
	if (list_empty(&target_list))
		return;

	spin_lock_irqsave(&target_list_lock, flags);
	list_for_each_entry(nt, &target_list, list) {
<<<<<<< HEAD
		netconsole_target_get(nt);
		if (nt->enabled && netif_running(nt->np.dev)) {
=======
		if (!nt->extended && nt->enabled && netif_running(nt->np.dev)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/*
			 * We nest this inside the for-each-target loop above
			 * so that we're able to get as much logging out to
			 * at least one target if we die inside here, instead
			 * of unnecessarily keeping all targets in lock-step.
			 */
			tmp = msg;
			for (left = len; left;) {
				frag = min(left, MAX_PRINT_CHUNK);
				netpoll_send_udp(&nt->np, tmp, frag);
				tmp += frag;
				left -= frag;
			}
		}
<<<<<<< HEAD
		netconsole_target_put(nt);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	spin_unlock_irqrestore(&target_list_lock, flags);
}

<<<<<<< HEAD
=======
/* Allocate new target (from boot/module param) and setup netpoll for it */
static struct netconsole_target *alloc_param_target(char *target_config,
						    int cmdline_count)
{
	struct netconsole_target *nt;
	int err;

	nt = alloc_and_init();
	if (!nt) {
		err = -ENOMEM;
		goto fail;
	}

	if (*target_config == '+') {
		nt->extended = true;
		target_config++;
	}

	if (*target_config == 'r') {
		if (!nt->extended) {
			pr_err("Netconsole configuration error. Release feature requires extended log message");
			err = -EINVAL;
			goto fail;
		}
		nt->release = true;
		target_config++;
	}

	/* Parse parameters and setup netpoll */
	err = netpoll_parse_options(&nt->np, target_config);
	if (err)
		goto fail;

	err = netpoll_setup(&nt->np);
	if (err)
		goto fail;

	populate_configfs_item(nt, cmdline_count);
	nt->enabled = true;

	return nt;

fail:
	kfree(nt);
	return ERR_PTR(err);
}

/* Cleanup netpoll for given target (from boot/module param) and free it */
static void free_param_target(struct netconsole_target *nt)
{
	netpoll_cleanup(&nt->np);
	kfree(nt);
}

static struct console netconsole_ext = {
	.name	= "netcon_ext",
	.flags	= CON_ENABLED | CON_EXTENDED,
	.write	= write_ext_msg,
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct console netconsole = {
	.name	= "netcon",
	.flags	= CON_ENABLED,
	.write	= write_msg,
};

static int __init init_netconsole(void)
{
	int err;
	struct netconsole_target *nt, *tmp;
<<<<<<< HEAD
=======
	unsigned int count = 0;
	bool extended = false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned long flags;
	char *target_config;
	char *input = config;

	if (strnlen(input, MAX_PARAM_LENGTH)) {
		while ((target_config = strsep(&input, ";"))) {
<<<<<<< HEAD
			nt = alloc_param_target(target_config);
=======
			nt = alloc_param_target(target_config, count);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (IS_ERR(nt)) {
				err = PTR_ERR(nt);
				goto fail;
			}
			/* Dump existing printks when we register */
<<<<<<< HEAD
			netconsole.flags |= CON_PRINTBUFFER;
=======
			if (nt->extended) {
				extended = true;
				netconsole_ext.flags |= CON_PRINTBUFFER;
			} else {
				netconsole.flags |= CON_PRINTBUFFER;
			}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

			spin_lock_irqsave(&target_list_lock, flags);
			list_add(&nt->list, &target_list);
			spin_unlock_irqrestore(&target_list_lock, flags);
<<<<<<< HEAD
=======
			count++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}

	err = register_netdevice_notifier(&netconsole_netdev_notifier);
	if (err)
		goto fail;

	err = dynamic_netconsole_init();
	if (err)
		goto undonotifier;

<<<<<<< HEAD
	register_console(&netconsole);
	printk(KERN_INFO "netconsole: network logging started\n");
=======
	if (extended)
		register_console(&netconsole_ext);
	register_console(&netconsole);
	pr_info("network logging started\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return err;

undonotifier:
	unregister_netdevice_notifier(&netconsole_netdev_notifier);

fail:
<<<<<<< HEAD
	printk(KERN_ERR "netconsole: cleaning up\n");
=======
	pr_err("cleaning up\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Remove all targets and destroy them (only targets created
	 * from the boot/module option exist here). Skipping the list
	 * lock is safe here, and netpoll_cleanup() will sleep.
	 */
	list_for_each_entry_safe(nt, tmp, &target_list, list) {
		list_del(&nt->list);
		free_param_target(nt);
	}

	return err;
}

static void __exit cleanup_netconsole(void)
{
	struct netconsole_target *nt, *tmp;

<<<<<<< HEAD
=======
	if (console_is_registered(&netconsole_ext))
		unregister_console(&netconsole_ext);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unregister_console(&netconsole);
	dynamic_netconsole_exit();
	unregister_netdevice_notifier(&netconsole_netdev_notifier);

	/*
	 * Targets created via configfs pin references on our module
	 * and would first be rmdir(2)'ed from userspace. We reach
	 * here only when they are already destroyed, and only those
	 * created from the boot/module option are left, so remove and
	 * destroy them. Skipping the list lock is safe here, and
	 * netpoll_cleanup() will sleep.
	 */
	list_for_each_entry_safe(nt, tmp, &target_list, list) {
		list_del(&nt->list);
		free_param_target(nt);
	}
}

/*
 * Use late_initcall to ensure netconsole is
 * initialized after network device driver if built-in.
 *
 * late_initcall() and module_init() are identical if built as module.
 */
late_initcall(init_netconsole);
module_exit(cleanup_netconsole);
