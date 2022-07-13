<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *	X.25 Packet Layer release 002
 *
 *	This is ALPHA test software. This code may break your machine,
 *	randomly fail to work with new releases, misbehave and/or generally
 *	screw up. It might even work.
 *
 *	This code REQUIRES 2.1.15 or higher
 *
<<<<<<< HEAD
 *	This module:
 *		This module is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *	History
 *	X.25 001	Jonathan Naylor	Started coding.
 */

#include <linux/if_arp.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <net/x25.h>
<<<<<<< HEAD
#ifdef KW_TAINT_ANALYSIS
   extern void * get_tainted_stuff();
#endif
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
LIST_HEAD(x25_route_list);
DEFINE_RWLOCK(x25_route_list_lock);

/*
 *	Add a new route.
 */
static int x25_add_route(struct x25_address *address, unsigned int sigdigits,
			 struct net_device *dev)
{
	struct x25_route *rt;
<<<<<<< HEAD
	struct list_head *entry;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int rc = -EINVAL;

	write_lock_bh(&x25_route_list_lock);

<<<<<<< HEAD
	list_for_each(entry, &x25_route_list) {
		rt = list_entry(entry, struct x25_route, node);

=======
	list_for_each_entry(rt, &x25_route_list, node) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!memcmp(&rt->address, address, sigdigits) &&
		    rt->sigdigits == sigdigits)
			goto out;
	}

	rt = kmalloc(sizeof(*rt), GFP_ATOMIC);
	rc = -ENOMEM;
	if (!rt)
		goto out;

	strcpy(rt->address.x25_addr, "000000000000000");
	memcpy(rt->address.x25_addr, address->x25_addr, sigdigits);

	rt->sigdigits = sigdigits;
	rt->dev       = dev;
<<<<<<< HEAD
	atomic_set(&rt->refcnt, 1);
=======
	refcount_set(&rt->refcnt, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	list_add(&rt->node, &x25_route_list);
	rc = 0;
out:
	write_unlock_bh(&x25_route_list_lock);
	return rc;
}

/**
 * __x25_remove_route - remove route from x25_route_list
<<<<<<< HEAD
 * @rt - route to remove
=======
 * @rt: route to remove
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Remove route from x25_route_list. If it was there.
 * Caller must hold x25_route_list_lock.
 */
static void __x25_remove_route(struct x25_route *rt)
{
	if (rt->node.next) {
		list_del(&rt->node);
		x25_route_put(rt);
	}
}

static int x25_del_route(struct x25_address *address, unsigned int sigdigits,
			 struct net_device *dev)
{
	struct x25_route *rt;
<<<<<<< HEAD
	struct list_head *entry;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int rc = -EINVAL;

	write_lock_bh(&x25_route_list_lock);

<<<<<<< HEAD
	list_for_each(entry, &x25_route_list) {
		rt = list_entry(entry, struct x25_route, node);

=======
	list_for_each_entry(rt, &x25_route_list, node) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!memcmp(&rt->address, address, sigdigits) &&
		    rt->sigdigits == sigdigits && rt->dev == dev) {
			__x25_remove_route(rt);
			rc = 0;
			break;
		}
	}

	write_unlock_bh(&x25_route_list_lock);
	return rc;
}

/*
 *	A device has been removed, remove its routes.
 */
void x25_route_device_down(struct net_device *dev)
{
	struct x25_route *rt;
	struct list_head *entry, *tmp;

	write_lock_bh(&x25_route_list_lock);

	list_for_each_safe(entry, tmp, &x25_route_list) {
		rt = list_entry(entry, struct x25_route, node);

		if (rt->dev == dev)
			__x25_remove_route(rt);
	}
	write_unlock_bh(&x25_route_list_lock);
<<<<<<< HEAD

	/* Remove any related forwarding */
	x25_clear_forward_by_dev(dev);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 *	Check that the device given is a valid X.25 interface that is "up".
 */
struct net_device *x25_dev_get(char *devname)
{
	struct net_device *dev = dev_get_by_name(&init_net, devname);

<<<<<<< HEAD
	if (dev &&
	    (!(dev->flags & IFF_UP) || (dev->type != ARPHRD_X25
#if IS_ENABLED(CONFIG_LLC)
					&& dev->type != ARPHRD_ETHER
#endif
					))){
=======
	if (dev && (!(dev->flags & IFF_UP) || dev->type != ARPHRD_X25)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		dev_put(dev);
		dev = NULL;
	}

	return dev;
}

/**
 * 	x25_get_route -	Find a route given an X.25 address.
<<<<<<< HEAD
 * 	@addr - address to find a route for
=======
 *	@addr: - address to find a route for
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * 	Find a route given an X.25 address.
 */
struct x25_route *x25_get_route(struct x25_address *addr)
{
	struct x25_route *rt, *use = NULL;
<<<<<<< HEAD
	struct list_head *entry;

	read_lock_bh(&x25_route_list_lock);

	list_for_each(entry, &x25_route_list) {
		rt = list_entry(entry, struct x25_route, node);

=======

	read_lock_bh(&x25_route_list_lock);

	list_for_each_entry(rt, &x25_route_list, node) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!memcmp(&rt->address, addr, rt->sigdigits)) {
			if (!use)
				use = rt;
			else if (rt->sigdigits > use->sigdigits)
				use = rt;
		}
	}

	if (use)
		x25_route_hold(use);

	read_unlock_bh(&x25_route_list_lock);
	return use;
}

/*
 *	Handle the ioctls that control the routing functions.
 */
<<<<<<< HEAD
int x25_route_ioctl(unsigned int cmd, void __user *arg_actual)
=======
int x25_route_ioctl(unsigned int cmd, void __user *arg)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct x25_route_struct rt;
	struct net_device *dev;
	int rc = -EINVAL;
<<<<<<< HEAD
	#ifdef KW_TAINT_ANALYSIS
	void __user *arg = (void __user *)get_tainted_stuff();
	#else
	void __user *arg = arg_actual;
	#endif
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (cmd != SIOCADDRT && cmd != SIOCDELRT)
		goto out;

	rc = -EFAULT;
	if (copy_from_user(&rt, arg, sizeof(rt)))
		goto out;

	rc = -EINVAL;
	if (rt.sigdigits > 15)
		goto out;

	dev = x25_dev_get(rt.device);
	if (!dev)
		goto out;

	if (cmd == SIOCADDRT)
		rc = x25_add_route(&rt.address, rt.sigdigits, dev);
	else
		rc = x25_del_route(&rt.address, rt.sigdigits, dev);
	dev_put(dev);
out:
	return rc;
}

/*
 *	Release all memory associated with X.25 routing structures.
 */
void __exit x25_route_free(void)
{
	struct x25_route *rt;
	struct list_head *entry, *tmp;

	write_lock_bh(&x25_route_list_lock);
	list_for_each_safe(entry, tmp, &x25_route_list) {
		rt = list_entry(entry, struct x25_route, node);
		__x25_remove_route(rt);
	}
	write_unlock_bh(&x25_route_list_lock);
}
