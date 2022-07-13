<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * INET		An implementation of the TCP/IP protocol suite for the LINUX
 *		operating system.  INET is implemented using the  BSD Socket
 *		interface as the means of communication with the user level.
 *
 *		Routing netlink socket interface: protocol independent part.
 *
 * Authors:	Alexey Kuznetsov, <kuznet@ms2.inr.ac.ru>
 *
<<<<<<< HEAD
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
 *	Fixes:
 *	Vitaly E. Lavrov		RTA_OK arithmetics was wrong.
 */

=======
 *	Fixes:
 *	Vitaly E. Lavrov		RTA_OK arithmetic was wrong.
 */

#include <linux/bitops.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/errno.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/socket.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/string.h>
#include <linux/sockios.h>
#include <linux/net.h>
#include <linux/fcntl.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/capability.h>
#include <linux/skbuff.h>
#include <linux/init.h>
#include <linux/security.h>
#include <linux/mutex.h>
#include <linux/if_addr.h>
<<<<<<< HEAD
#include <linux/pci.h>

#include <asm/uaccess.h>
=======
#include <linux/if_bridge.h>
#include <linux/if_vlan.h>
#include <linux/pci.h>
#include <linux/etherdevice.h>
#include <linux/bpf.h>

#include <linux/uaccess.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <linux/inet.h>
#include <linux/netdevice.h>
#include <net/ip.h>
#include <net/protocol.h>
#include <net/arp.h>
#include <net/route.h>
#include <net/udp.h>
<<<<<<< HEAD
=======
#include <net/tcp.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <net/sock.h>
#include <net/pkt_sched.h>
#include <net/fib_rules.h>
#include <net/rtnetlink.h>
#include <net/net_namespace.h>
<<<<<<< HEAD
=======
#include <net/devlink.h>
#if IS_ENABLED(CONFIG_IPV6)
#include <net/addrconf.h>
#endif
#include <linux/dpll.h>

#include "dev.h"

#define RTNL_MAX_TYPE		50
#define RTNL_SLAVE_MAX_TYPE	44
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct rtnl_link {
	rtnl_doit_func		doit;
	rtnl_dumpit_func	dumpit;
<<<<<<< HEAD
	rtnl_calcit_func 	calcit;
=======
	struct module		*owner;
	unsigned int		flags;
	struct rcu_head		rcu;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static DEFINE_MUTEX(rtnl_mutex);

void rtnl_lock(void)
{
	mutex_lock(&rtnl_mutex);
}
EXPORT_SYMBOL(rtnl_lock);

<<<<<<< HEAD
void __rtnl_unlock(void)
{
	mutex_unlock(&rtnl_mutex);
=======
int rtnl_lock_killable(void)
{
	return mutex_lock_killable(&rtnl_mutex);
}
EXPORT_SYMBOL(rtnl_lock_killable);

static struct sk_buff *defer_kfree_skb_list;
void rtnl_kfree_skbs(struct sk_buff *head, struct sk_buff *tail)
{
	if (head && tail) {
		tail->next = defer_kfree_skb_list;
		defer_kfree_skb_list = head;
	}
}
EXPORT_SYMBOL(rtnl_kfree_skbs);

void __rtnl_unlock(void)
{
	struct sk_buff *head = defer_kfree_skb_list;

	defer_kfree_skb_list = NULL;

	/* Ensure that we didn't actually add any TODO item when __rtnl_unlock()
	 * is used. In some places, e.g. in cfg80211, we have code that will do
	 * something like
	 *   rtnl_lock()
	 *   wiphy_lock()
	 *   ...
	 *   rtnl_unlock()
	 *
	 * and because netdev_run_todo() acquires the RTNL for items on the list
	 * we could cause a situation such as this:
	 * Thread 1			Thread 2
	 *				  rtnl_lock()
	 *				  unregister_netdevice()
	 *				  __rtnl_unlock()
	 * rtnl_lock()
	 * wiphy_lock()
	 * rtnl_unlock()
	 *   netdev_run_todo()
	 *     __rtnl_unlock()
	 *
	 *     // list not empty now
	 *     // because of thread 2
	 *				  rtnl_lock()
	 *     while (!list_empty(...))
	 *       rtnl_lock()
	 *				  wiphy_lock()
	 * **** DEADLOCK ****
	 *
	 * However, usage of __rtnl_unlock() is rare, and so we can ensure that
	 * it's not used in cases where something is added to do the list.
	 */
	WARN_ON(!list_empty(&net_todo_list));

	mutex_unlock(&rtnl_mutex);

	while (head) {
		struct sk_buff *next = head->next;

		kfree_skb(head);
		cond_resched();
		head = next;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void rtnl_unlock(void)
{
	/* This fellow will unlock it for us. */
	netdev_run_todo();
}
EXPORT_SYMBOL(rtnl_unlock);

int rtnl_trylock(void)
{
	return mutex_trylock(&rtnl_mutex);
}
EXPORT_SYMBOL(rtnl_trylock);

int rtnl_is_locked(void)
{
	return mutex_is_locked(&rtnl_mutex);
}
EXPORT_SYMBOL(rtnl_is_locked);

<<<<<<< HEAD
#ifdef CONFIG_PROVE_LOCKING
int lockdep_rtnl_is_held(void)
=======
bool refcount_dec_and_rtnl_lock(refcount_t *r)
{
	return refcount_dec_and_mutex_lock(r, &rtnl_mutex);
}
EXPORT_SYMBOL(refcount_dec_and_rtnl_lock);

#ifdef CONFIG_PROVE_LOCKING
bool lockdep_rtnl_is_held(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return lockdep_is_held(&rtnl_mutex);
}
EXPORT_SYMBOL(lockdep_rtnl_is_held);
#endif /* #ifdef CONFIG_PROVE_LOCKING */

<<<<<<< HEAD
static struct rtnl_link *rtnl_msg_handlers[RTNL_FAMILY_MAX + 1];
=======
static struct rtnl_link __rcu *__rcu *rtnl_msg_handlers[RTNL_FAMILY_MAX + 1];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline int rtm_msgindex(int msgtype)
{
	int msgindex = msgtype - RTM_BASE;

	/*
	 * msgindex < 0 implies someone tried to register a netlink
	 * control code. msgindex >= RTM_NR_MSGTYPES may indicate that
	 * the message type has not been added to linux/rtnetlink.h
	 */
	BUG_ON(msgindex < 0 || msgindex >= RTM_NR_MSGTYPES);

	return msgindex;
}

<<<<<<< HEAD
static rtnl_doit_func rtnl_get_doit(int protocol, int msgindex)
{
	struct rtnl_link *tab;

	if (protocol <= RTNL_FAMILY_MAX)
		tab = rtnl_msg_handlers[protocol];
	else
		tab = NULL;

	if (tab == NULL || tab[msgindex].doit == NULL)
		tab = rtnl_msg_handlers[PF_UNSPEC];

	return tab ? tab[msgindex].doit : NULL;
}

static rtnl_dumpit_func rtnl_get_dumpit(int protocol, int msgindex)
{
	struct rtnl_link *tab;

	if (protocol <= RTNL_FAMILY_MAX)
		tab = rtnl_msg_handlers[protocol];
	else
		tab = NULL;

	if (tab == NULL || tab[msgindex].dumpit == NULL)
		tab = rtnl_msg_handlers[PF_UNSPEC];

	return tab ? tab[msgindex].dumpit : NULL;
}

static rtnl_calcit_func rtnl_get_calcit(int protocol, int msgindex)
{
	struct rtnl_link *tab;

	if (protocol <= RTNL_FAMILY_MAX)
		tab = rtnl_msg_handlers[protocol];
	else
		tab = NULL;

	if (tab == NULL || tab[msgindex].calcit == NULL)
		tab = rtnl_msg_handlers[PF_UNSPEC];

	return tab ? tab[msgindex].calcit : NULL;
}

/**
 * __rtnl_register - Register a rtnetlink message type
=======
static struct rtnl_link *rtnl_get_link(int protocol, int msgtype)
{
	struct rtnl_link __rcu **tab;

	if (protocol >= ARRAY_SIZE(rtnl_msg_handlers))
		protocol = PF_UNSPEC;

	tab = rcu_dereference_rtnl(rtnl_msg_handlers[protocol]);
	if (!tab)
		tab = rcu_dereference_rtnl(rtnl_msg_handlers[PF_UNSPEC]);

	return rcu_dereference_rtnl(tab[msgtype]);
}

static int rtnl_register_internal(struct module *owner,
				  int protocol, int msgtype,
				  rtnl_doit_func doit, rtnl_dumpit_func dumpit,
				  unsigned int flags)
{
	struct rtnl_link *link, *old;
	struct rtnl_link __rcu **tab;
	int msgindex;
	int ret = -ENOBUFS;

	BUG_ON(protocol < 0 || protocol > RTNL_FAMILY_MAX);
	msgindex = rtm_msgindex(msgtype);

	rtnl_lock();
	tab = rtnl_dereference(rtnl_msg_handlers[protocol]);
	if (tab == NULL) {
		tab = kcalloc(RTM_NR_MSGTYPES, sizeof(void *), GFP_KERNEL);
		if (!tab)
			goto unlock;

		/* ensures we see the 0 stores */
		rcu_assign_pointer(rtnl_msg_handlers[protocol], tab);
	}

	old = rtnl_dereference(tab[msgindex]);
	if (old) {
		link = kmemdup(old, sizeof(*old), GFP_KERNEL);
		if (!link)
			goto unlock;
	} else {
		link = kzalloc(sizeof(*link), GFP_KERNEL);
		if (!link)
			goto unlock;
	}

	WARN_ON(link->owner && link->owner != owner);
	link->owner = owner;

	WARN_ON(doit && link->doit && link->doit != doit);
	if (doit)
		link->doit = doit;
	WARN_ON(dumpit && link->dumpit && link->dumpit != dumpit);
	if (dumpit)
		link->dumpit = dumpit;

	WARN_ON(rtnl_msgtype_kind(msgtype) != RTNL_KIND_DEL &&
		(flags & RTNL_FLAG_BULK_DEL_SUPPORTED));
	link->flags |= flags;

	/* publish protocol:msgtype */
	rcu_assign_pointer(tab[msgindex], link);
	ret = 0;
	if (old)
		kfree_rcu(old, rcu);
unlock:
	rtnl_unlock();
	return ret;
}

/**
 * rtnl_register_module - Register a rtnetlink message type
 *
 * @owner: module registering the hook (THIS_MODULE)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @protocol: Protocol family or PF_UNSPEC
 * @msgtype: rtnetlink message type
 * @doit: Function pointer called for each request message
 * @dumpit: Function pointer called for each dump request (NLM_F_DUMP) message
<<<<<<< HEAD
 * @calcit: Function pointer to calc size of dump message
=======
 * @flags: rtnl_link_flags to modify behaviour of doit/dumpit functions
 *
 * Like rtnl_register, but for use by removable modules.
 */
int rtnl_register_module(struct module *owner,
			 int protocol, int msgtype,
			 rtnl_doit_func doit, rtnl_dumpit_func dumpit,
			 unsigned int flags)
{
	return rtnl_register_internal(owner, protocol, msgtype,
				      doit, dumpit, flags);
}
EXPORT_SYMBOL_GPL(rtnl_register_module);

/**
 * rtnl_register - Register a rtnetlink message type
 * @protocol: Protocol family or PF_UNSPEC
 * @msgtype: rtnetlink message type
 * @doit: Function pointer called for each request message
 * @dumpit: Function pointer called for each dump request (NLM_F_DUMP) message
 * @flags: rtnl_link_flags to modify behaviour of doit/dumpit functions
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Registers the specified function pointers (at least one of them has
 * to be non-NULL) to be called whenever a request message for the
 * specified protocol family and message type is received.
 *
 * The special protocol family PF_UNSPEC may be used to define fallback
 * function pointers for the case when no entry for the specific protocol
 * family exists.
<<<<<<< HEAD
 *
 * Returns 0 on success or a negative error code.
 */
int __rtnl_register(int protocol, int msgtype,
		    rtnl_doit_func doit, rtnl_dumpit_func dumpit,
		    rtnl_calcit_func calcit)
{
	struct rtnl_link *tab;
	int msgindex;

	BUG_ON(protocol < 0 || protocol > RTNL_FAMILY_MAX);
	msgindex = rtm_msgindex(msgtype);

	tab = rtnl_msg_handlers[protocol];
	if (tab == NULL) {
		tab = kcalloc(RTM_NR_MSGTYPES, sizeof(*tab), GFP_KERNEL);
		if (tab == NULL)
			return -ENOBUFS;

		rtnl_msg_handlers[protocol] = tab;
	}

	if (doit)
		tab[msgindex].doit = doit;

	if (dumpit)
		tab[msgindex].dumpit = dumpit;

	if (calcit)
		tab[msgindex].calcit = calcit;

	return 0;
}
EXPORT_SYMBOL_GPL(__rtnl_register);

/**
 * rtnl_register - Register a rtnetlink message type
 *
 * Identical to __rtnl_register() but panics on failure. This is useful
 * as failure of this function is very unlikely, it can only happen due
 * to lack of memory when allocating the chain to store all message
 * handlers for a protocol. Meant for use in init functions where lack
 * of memory implies no sense in continuing.
 */
void rtnl_register(int protocol, int msgtype,
		   rtnl_doit_func doit, rtnl_dumpit_func dumpit,
		   rtnl_calcit_func calcit)
{
	if (__rtnl_register(protocol, msgtype, doit, dumpit, calcit) < 0)
		panic("Unable to register rtnetlink message handler, "
		      "protocol = %d, message type = %d\n",
		      protocol, msgtype);
}
EXPORT_SYMBOL_GPL(rtnl_register);
=======
 */
void rtnl_register(int protocol, int msgtype,
		   rtnl_doit_func doit, rtnl_dumpit_func dumpit,
		   unsigned int flags)
{
	int err;

	err = rtnl_register_internal(NULL, protocol, msgtype, doit, dumpit,
				     flags);
	if (err)
		pr_err("Unable to register rtnetlink message handler, "
		       "protocol = %d, message type = %d\n", protocol, msgtype);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * rtnl_unregister - Unregister a rtnetlink message type
 * @protocol: Protocol family or PF_UNSPEC
 * @msgtype: rtnetlink message type
 *
 * Returns 0 on success or a negative error code.
 */
int rtnl_unregister(int protocol, int msgtype)
{
<<<<<<< HEAD
=======
	struct rtnl_link __rcu **tab;
	struct rtnl_link *link;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int msgindex;

	BUG_ON(protocol < 0 || protocol > RTNL_FAMILY_MAX);
	msgindex = rtm_msgindex(msgtype);

<<<<<<< HEAD
	if (rtnl_msg_handlers[protocol] == NULL)
		return -ENOENT;

	rtnl_msg_handlers[protocol][msgindex].doit = NULL;
	rtnl_msg_handlers[protocol][msgindex].dumpit = NULL;
=======
	rtnl_lock();
	tab = rtnl_dereference(rtnl_msg_handlers[protocol]);
	if (!tab) {
		rtnl_unlock();
		return -ENOENT;
	}

	link = rcu_replace_pointer_rtnl(tab[msgindex], NULL);
	rtnl_unlock();

	kfree_rcu(link, rcu);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}
EXPORT_SYMBOL_GPL(rtnl_unregister);

/**
 * rtnl_unregister_all - Unregister all rtnetlink message type of a protocol
 * @protocol : Protocol family or PF_UNSPEC
 *
 * Identical to calling rtnl_unregster() for all registered message types
 * of a certain protocol family.
 */
void rtnl_unregister_all(int protocol)
{
<<<<<<< HEAD
	BUG_ON(protocol < 0 || protocol > RTNL_FAMILY_MAX);

	kfree(rtnl_msg_handlers[protocol]);
	rtnl_msg_handlers[protocol] = NULL;
=======
	struct rtnl_link __rcu **tab;
	struct rtnl_link *link;
	int msgindex;

	BUG_ON(protocol < 0 || protocol > RTNL_FAMILY_MAX);

	rtnl_lock();
	tab = rcu_replace_pointer_rtnl(rtnl_msg_handlers[protocol], NULL);
	if (!tab) {
		rtnl_unlock();
		return;
	}
	for (msgindex = 0; msgindex < RTM_NR_MSGTYPES; msgindex++) {
		link = rcu_replace_pointer_rtnl(tab[msgindex], NULL);
		kfree_rcu(link, rcu);
	}
	rtnl_unlock();

	synchronize_net();

	kfree(tab);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(rtnl_unregister_all);

static LIST_HEAD(link_ops);

static const struct rtnl_link_ops *rtnl_link_ops_get(const char *kind)
{
	const struct rtnl_link_ops *ops;

	list_for_each_entry(ops, &link_ops, list) {
		if (!strcmp(ops->kind, kind))
			return ops;
	}
	return NULL;
}

/**
 * __rtnl_link_register - Register rtnl_link_ops with rtnetlink.
 * @ops: struct rtnl_link_ops * to register
 *
 * The caller must hold the rtnl_mutex. This function should be used
 * by drivers that create devices during module initialization. It
 * must be called before registering the devices.
 *
 * Returns 0 on success or a negative error code.
 */
int __rtnl_link_register(struct rtnl_link_ops *ops)
{
	if (rtnl_link_ops_get(ops->kind))
		return -EEXIST;

<<<<<<< HEAD
	if (!ops->dellink)
=======
	/* The check for alloc/setup is here because if ops
	 * does not have that filled up, it is not possible
	 * to use the ops for creating device. So do not
	 * fill up dellink as well. That disables rtnl_dellink.
	 */
	if ((ops->alloc || ops->setup) && !ops->dellink)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ops->dellink = unregister_netdevice_queue;

	list_add_tail(&ops->list, &link_ops);
	return 0;
}
EXPORT_SYMBOL_GPL(__rtnl_link_register);

/**
 * rtnl_link_register - Register rtnl_link_ops with rtnetlink.
 * @ops: struct rtnl_link_ops * to register
 *
 * Returns 0 on success or a negative error code.
 */
int rtnl_link_register(struct rtnl_link_ops *ops)
{
	int err;

<<<<<<< HEAD
=======
	/* Sanity-check max sizes to avoid stack buffer overflow. */
	if (WARN_ON(ops->maxtype > RTNL_MAX_TYPE ||
		    ops->slave_maxtype > RTNL_SLAVE_MAX_TYPE))
		return -EINVAL;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rtnl_lock();
	err = __rtnl_link_register(ops);
	rtnl_unlock();
	return err;
}
EXPORT_SYMBOL_GPL(rtnl_link_register);

static void __rtnl_kill_links(struct net *net, struct rtnl_link_ops *ops)
{
	struct net_device *dev;
	LIST_HEAD(list_kill);

	for_each_netdev(net, dev) {
		if (dev->rtnl_link_ops == ops)
			ops->dellink(dev, &list_kill);
	}
	unregister_netdevice_many(&list_kill);
}

/**
 * __rtnl_link_unregister - Unregister rtnl_link_ops from rtnetlink.
 * @ops: struct rtnl_link_ops * to unregister
 *
<<<<<<< HEAD
 * The caller must hold the rtnl_mutex.
=======
 * The caller must hold the rtnl_mutex and guarantee net_namespace_list
 * integrity (hold pernet_ops_rwsem for writing to close the race
 * with setup_net() and cleanup_net()).
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
void __rtnl_link_unregister(struct rtnl_link_ops *ops)
{
	struct net *net;

	for_each_net(net) {
		__rtnl_kill_links(net, ops);
	}
	list_del(&ops->list);
}
EXPORT_SYMBOL_GPL(__rtnl_link_unregister);

<<<<<<< HEAD
=======
/* Return with the rtnl_lock held when there are no network
 * devices unregistering in any network namespace.
 */
static void rtnl_lock_unregistering_all(void)
{
	DEFINE_WAIT_FUNC(wait, woken_wake_function);

	add_wait_queue(&netdev_unregistering_wq, &wait);
	for (;;) {
		rtnl_lock();
		/* We held write locked pernet_ops_rwsem, and parallel
		 * setup_net() and cleanup_net() are not possible.
		 */
		if (!atomic_read(&dev_unreg_count))
			break;
		__rtnl_unlock();

		wait_woken(&wait, TASK_UNINTERRUPTIBLE, MAX_SCHEDULE_TIMEOUT);
	}
	remove_wait_queue(&netdev_unregistering_wq, &wait);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * rtnl_link_unregister - Unregister rtnl_link_ops from rtnetlink.
 * @ops: struct rtnl_link_ops * to unregister
 */
void rtnl_link_unregister(struct rtnl_link_ops *ops)
{
<<<<<<< HEAD
	rtnl_lock();
	__rtnl_link_unregister(ops);
	rtnl_unlock();
}
EXPORT_SYMBOL_GPL(rtnl_link_unregister);

=======
	/* Close the race with setup_net() and cleanup_net() */
	down_write(&pernet_ops_rwsem);
	rtnl_lock_unregistering_all();
	__rtnl_link_unregister(ops);
	rtnl_unlock();
	up_write(&pernet_ops_rwsem);
}
EXPORT_SYMBOL_GPL(rtnl_link_unregister);

static size_t rtnl_link_get_slave_info_data_size(const struct net_device *dev)
{
	struct net_device *master_dev;
	const struct rtnl_link_ops *ops;
	size_t size = 0;

	rcu_read_lock();

	master_dev = netdev_master_upper_dev_get_rcu((struct net_device *)dev);
	if (!master_dev)
		goto out;

	ops = master_dev->rtnl_link_ops;
	if (!ops || !ops->get_slave_size)
		goto out;
	/* IFLA_INFO_SLAVE_DATA + nested data */
	size = nla_total_size(sizeof(struct nlattr)) +
	       ops->get_slave_size(master_dev, dev);

out:
	rcu_read_unlock();
	return size;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static size_t rtnl_link_get_size(const struct net_device *dev)
{
	const struct rtnl_link_ops *ops = dev->rtnl_link_ops;
	size_t size;

	if (!ops)
		return 0;

	size = nla_total_size(sizeof(struct nlattr)) + /* IFLA_LINKINFO */
	       nla_total_size(strlen(ops->kind) + 1);  /* IFLA_INFO_KIND */

	if (ops->get_size)
		/* IFLA_INFO_DATA + nested data */
		size += nla_total_size(sizeof(struct nlattr)) +
			ops->get_size(dev);

	if (ops->get_xstats_size)
		/* IFLA_INFO_XSTATS */
		size += nla_total_size(ops->get_xstats_size(dev));

<<<<<<< HEAD
=======
	size += rtnl_link_get_slave_info_data_size(dev);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return size;
}

static LIST_HEAD(rtnl_af_ops);

static const struct rtnl_af_ops *rtnl_af_lookup(const int family)
{
	const struct rtnl_af_ops *ops;

<<<<<<< HEAD
=======
	ASSERT_RTNL();

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	list_for_each_entry(ops, &rtnl_af_ops, list) {
		if (ops->family == family)
			return ops;
	}

	return NULL;
}

/**
<<<<<<< HEAD
 * __rtnl_af_register - Register rtnl_af_ops with rtnetlink.
 * @ops: struct rtnl_af_ops * to register
 *
 * The caller must hold the rtnl_mutex.
 *
 * Returns 0 on success or a negative error code.
 */
int __rtnl_af_register(struct rtnl_af_ops *ops)
{
	list_add_tail(&ops->list, &rtnl_af_ops);
	return 0;
}
EXPORT_SYMBOL_GPL(__rtnl_af_register);

/**
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * rtnl_af_register - Register rtnl_af_ops with rtnetlink.
 * @ops: struct rtnl_af_ops * to register
 *
 * Returns 0 on success or a negative error code.
 */
<<<<<<< HEAD
int rtnl_af_register(struct rtnl_af_ops *ops)
{
	int err;

	rtnl_lock();
	err = __rtnl_af_register(ops);
	rtnl_unlock();
	return err;
=======
void rtnl_af_register(struct rtnl_af_ops *ops)
{
	rtnl_lock();
	list_add_tail_rcu(&ops->list, &rtnl_af_ops);
	rtnl_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(rtnl_af_register);

/**
<<<<<<< HEAD
 * __rtnl_af_unregister - Unregister rtnl_af_ops from rtnetlink.
 * @ops: struct rtnl_af_ops * to unregister
 *
 * The caller must hold the rtnl_mutex.
 */
void __rtnl_af_unregister(struct rtnl_af_ops *ops)
{
	list_del(&ops->list);
}
EXPORT_SYMBOL_GPL(__rtnl_af_unregister);

/**
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * rtnl_af_unregister - Unregister rtnl_af_ops from rtnetlink.
 * @ops: struct rtnl_af_ops * to unregister
 */
void rtnl_af_unregister(struct rtnl_af_ops *ops)
{
	rtnl_lock();
<<<<<<< HEAD
	__rtnl_af_unregister(ops);
	rtnl_unlock();
}
EXPORT_SYMBOL_GPL(rtnl_af_unregister);

static size_t rtnl_link_get_af_size(const struct net_device *dev)
=======
	list_del_rcu(&ops->list);
	rtnl_unlock();

	synchronize_rcu();
}
EXPORT_SYMBOL_GPL(rtnl_af_unregister);

static size_t rtnl_link_get_af_size(const struct net_device *dev,
				    u32 ext_filter_mask)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct rtnl_af_ops *af_ops;
	size_t size;

	/* IFLA_AF_SPEC */
	size = nla_total_size(sizeof(struct nlattr));

<<<<<<< HEAD
	list_for_each_entry(af_ops, &rtnl_af_ops, list) {
		if (af_ops->get_link_af_size) {
			/* AF_* + nested data */
			size += nla_total_size(sizeof(struct nlattr)) +
				af_ops->get_link_af_size(dev);
		}
	}
=======
	rcu_read_lock();
	list_for_each_entry_rcu(af_ops, &rtnl_af_ops, list) {
		if (af_ops->get_link_af_size) {
			/* AF_* + nested data */
			size += nla_total_size(sizeof(struct nlattr)) +
				af_ops->get_link_af_size(dev, ext_filter_mask);
		}
	}
	rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return size;
}

<<<<<<< HEAD
static int rtnl_link_fill(struct sk_buff *skb, const struct net_device *dev)
{
	const struct rtnl_link_ops *ops = dev->rtnl_link_ops;
	struct nlattr *linkinfo, *data;
	int err = -EMSGSIZE;

	linkinfo = nla_nest_start(skb, IFLA_LINKINFO);
	if (linkinfo == NULL)
		goto out;

	if (nla_put_string(skb, IFLA_INFO_KIND, ops->kind) < 0)
		goto err_cancel_link;
	if (ops->fill_xstats) {
		err = ops->fill_xstats(skb, dev);
		if (err < 0)
			goto err_cancel_link;
	}
	if (ops->fill_info) {
		data = nla_nest_start(skb, IFLA_INFO_DATA);
		if (data == NULL) {
			err = -EMSGSIZE;
			goto err_cancel_link;
		}
=======
static bool rtnl_have_link_slave_info(const struct net_device *dev)
{
	struct net_device *master_dev;
	bool ret = false;

	rcu_read_lock();

	master_dev = netdev_master_upper_dev_get_rcu((struct net_device *)dev);
	if (master_dev && master_dev->rtnl_link_ops)
		ret = true;
	rcu_read_unlock();
	return ret;
}

static int rtnl_link_slave_info_fill(struct sk_buff *skb,
				     const struct net_device *dev)
{
	struct net_device *master_dev;
	const struct rtnl_link_ops *ops;
	struct nlattr *slave_data;
	int err;

	master_dev = netdev_master_upper_dev_get((struct net_device *) dev);
	if (!master_dev)
		return 0;
	ops = master_dev->rtnl_link_ops;
	if (!ops)
		return 0;
	if (nla_put_string(skb, IFLA_INFO_SLAVE_KIND, ops->kind) < 0)
		return -EMSGSIZE;
	if (ops->fill_slave_info) {
		slave_data = nla_nest_start_noflag(skb, IFLA_INFO_SLAVE_DATA);
		if (!slave_data)
			return -EMSGSIZE;
		err = ops->fill_slave_info(skb, master_dev, dev);
		if (err < 0)
			goto err_cancel_slave_data;
		nla_nest_end(skb, slave_data);
	}
	return 0;

err_cancel_slave_data:
	nla_nest_cancel(skb, slave_data);
	return err;
}

static int rtnl_link_info_fill(struct sk_buff *skb,
			       const struct net_device *dev)
{
	const struct rtnl_link_ops *ops = dev->rtnl_link_ops;
	struct nlattr *data;
	int err;

	if (!ops)
		return 0;
	if (nla_put_string(skb, IFLA_INFO_KIND, ops->kind) < 0)
		return -EMSGSIZE;
	if (ops->fill_xstats) {
		err = ops->fill_xstats(skb, dev);
		if (err < 0)
			return err;
	}
	if (ops->fill_info) {
		data = nla_nest_start_noflag(skb, IFLA_INFO_DATA);
		if (data == NULL)
			return -EMSGSIZE;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = ops->fill_info(skb, dev);
		if (err < 0)
			goto err_cancel_data;
		nla_nest_end(skb, data);
	}
<<<<<<< HEAD

	nla_nest_end(skb, linkinfo);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;

err_cancel_data:
	nla_nest_cancel(skb, data);
<<<<<<< HEAD
=======
	return err;
}

static int rtnl_link_fill(struct sk_buff *skb, const struct net_device *dev)
{
	struct nlattr *linkinfo;
	int err = -EMSGSIZE;

	linkinfo = nla_nest_start_noflag(skb, IFLA_LINKINFO);
	if (linkinfo == NULL)
		goto out;

	err = rtnl_link_info_fill(skb, dev);
	if (err < 0)
		goto err_cancel_link;

	err = rtnl_link_slave_info_fill(skb, dev);
	if (err < 0)
		goto err_cancel_link;

	nla_nest_end(skb, linkinfo);
	return 0;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
err_cancel_link:
	nla_nest_cancel(skb, linkinfo);
out:
	return err;
}

<<<<<<< HEAD
static const int rtm_min[RTM_NR_FAMILIES] =
{
	[RTM_FAM(RTM_NEWLINK)]      = NLMSG_LENGTH(sizeof(struct ifinfomsg)),
	[RTM_FAM(RTM_NEWADDR)]      = NLMSG_LENGTH(sizeof(struct ifaddrmsg)),
	[RTM_FAM(RTM_NEWROUTE)]     = NLMSG_LENGTH(sizeof(struct rtmsg)),
	[RTM_FAM(RTM_NEWRULE)]      = NLMSG_LENGTH(sizeof(struct fib_rule_hdr)),
	[RTM_FAM(RTM_NEWQDISC)]     = NLMSG_LENGTH(sizeof(struct tcmsg)),
	[RTM_FAM(RTM_NEWTCLASS)]    = NLMSG_LENGTH(sizeof(struct tcmsg)),
	[RTM_FAM(RTM_NEWTFILTER)]   = NLMSG_LENGTH(sizeof(struct tcmsg)),
	[RTM_FAM(RTM_NEWACTION)]    = NLMSG_LENGTH(sizeof(struct tcamsg)),
	[RTM_FAM(RTM_GETMULTICAST)] = NLMSG_LENGTH(sizeof(struct rtgenmsg)),
	[RTM_FAM(RTM_GETANYCAST)]   = NLMSG_LENGTH(sizeof(struct rtgenmsg)),
};

static const int rta_max[RTM_NR_FAMILIES] =
{
	[RTM_FAM(RTM_NEWLINK)]      = IFLA_MAX,
	[RTM_FAM(RTM_NEWADDR)]      = IFA_MAX,
	[RTM_FAM(RTM_NEWROUTE)]     = RTA_MAX,
	[RTM_FAM(RTM_NEWRULE)]      = FRA_MAX,
	[RTM_FAM(RTM_NEWQDISC)]     = TCA_MAX,
	[RTM_FAM(RTM_NEWTCLASS)]    = TCA_MAX,
	[RTM_FAM(RTM_NEWTFILTER)]   = TCA_MAX,
	[RTM_FAM(RTM_NEWACTION)]    = TCAA_MAX,
};

void __rta_fill(struct sk_buff *skb, int attrtype, int attrlen, const void *data)
{
	struct rtattr *rta;
	int size = RTA_LENGTH(attrlen);

	rta = (struct rtattr *)skb_put(skb, RTA_ALIGN(size));
	rta->rta_type = attrtype;
	rta->rta_len = size;
	memcpy(RTA_DATA(rta), data, attrlen);
	memset(RTA_DATA(rta) + attrlen, 0, RTA_ALIGN(size) - size);
}
EXPORT_SYMBOL(__rta_fill);

int rtnetlink_send(struct sk_buff *skb, struct net *net, u32 pid, unsigned group, int echo)
{
	struct sock *rtnl = net->rtnl;
	int err = 0;

	NETLINK_CB(skb).dst_group = group;
	if (echo)
		atomic_inc(&skb->users);
	netlink_broadcast(rtnl, skb, pid, group, GFP_KERNEL);
	if (echo)
		err = netlink_unicast(rtnl, skb, pid, MSG_DONTWAIT);
	return err;
=======
int rtnetlink_send(struct sk_buff *skb, struct net *net, u32 pid, unsigned int group, int echo)
{
	struct sock *rtnl = net->rtnl;

	return nlmsg_notify(rtnl, skb, pid, group, echo, GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int rtnl_unicast(struct sk_buff *skb, struct net *net, u32 pid)
{
	struct sock *rtnl = net->rtnl;

	return nlmsg_unicast(rtnl, skb, pid);
}
EXPORT_SYMBOL(rtnl_unicast);

void rtnl_notify(struct sk_buff *skb, struct net *net, u32 pid, u32 group,
<<<<<<< HEAD
		 struct nlmsghdr *nlh, gfp_t flags)
{
	struct sock *rtnl = net->rtnl;
	int report = 0;

	if (nlh)
		report = nlmsg_report(nlh);

	nlmsg_notify(rtnl, skb, pid, group, report, flags);
=======
		 const struct nlmsghdr *nlh, gfp_t flags)
{
	struct sock *rtnl = net->rtnl;

	nlmsg_notify(rtnl, skb, pid, group, nlmsg_report(nlh), flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL(rtnl_notify);

void rtnl_set_sk_err(struct net *net, u32 group, int error)
{
	struct sock *rtnl = net->rtnl;

	netlink_set_err(rtnl, 0, group, error);
}
EXPORT_SYMBOL(rtnl_set_sk_err);

int rtnetlink_put_metrics(struct sk_buff *skb, u32 *metrics)
{
	struct nlattr *mx;
	int i, valid = 0;

<<<<<<< HEAD
	mx = nla_nest_start(skb, RTA_METRICS);
=======
	/* nothing is dumped for dst_default_metrics, so just skip the loop */
	if (metrics == dst_default_metrics.metrics)
		return 0;

	mx = nla_nest_start_noflag(skb, RTA_METRICS);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (mx == NULL)
		return -ENOBUFS;

	for (i = 0; i < RTAX_MAX; i++) {
		if (metrics[i]) {
<<<<<<< HEAD
			valid++;
			NLA_PUT_U32(skb, i+1, metrics[i]);
=======
			if (i == RTAX_CC_ALGO - 1) {
				char tmp[TCP_CA_NAME_MAX], *name;

				name = tcp_ca_get_name_by_key(metrics[i], tmp);
				if (!name)
					continue;
				if (nla_put_string(skb, i + 1, name))
					goto nla_put_failure;
			} else if (i == RTAX_FEATURES - 1) {
				u32 user_features = metrics[i] & RTAX_FEATURE_MASK;

				if (!user_features)
					continue;
				BUILD_BUG_ON(RTAX_FEATURE_MASK & DST_FEATURE_MASK);
				if (nla_put_u32(skb, i + 1, user_features))
					goto nla_put_failure;
			} else {
				if (nla_put_u32(skb, i + 1, metrics[i]))
					goto nla_put_failure;
			}
			valid++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}

	if (!valid) {
		nla_nest_cancel(skb, mx);
		return 0;
	}

	return nla_nest_end(skb, mx);

nla_put_failure:
	nla_nest_cancel(skb, mx);
	return -EMSGSIZE;
}
EXPORT_SYMBOL(rtnetlink_put_metrics);

int rtnl_put_cacheinfo(struct sk_buff *skb, struct dst_entry *dst, u32 id,
<<<<<<< HEAD
		       u32 ts, u32 tsage, long expires, u32 error)
{
	struct rta_cacheinfo ci = {
		.rta_lastuse = jiffies_to_clock_t(jiffies - dst->lastuse),
		.rta_used = dst->__use,
		.rta_clntref = atomic_read(&(dst->__refcnt)),
		.rta_error = error,
		.rta_id =  id,
		.rta_ts = ts,
		.rta_tsage = tsage,
	};

	if (expires)
		ci.rta_expires = jiffies_to_clock_t(expires);

=======
		       long expires, u32 error)
{
	struct rta_cacheinfo ci = {
		.rta_error = error,
		.rta_id =  id,
	};

	if (dst) {
		ci.rta_lastuse = jiffies_delta_to_clock_t(jiffies - dst->lastuse);
		ci.rta_used = dst->__use;
		ci.rta_clntref = rcuref_read(&dst->__rcuref);
	}
	if (expires) {
		unsigned long clock;

		clock = jiffies_to_clock_t(abs(expires));
		clock = min_t(unsigned long, clock, INT_MAX);
		ci.rta_expires = (expires > 0) ? clock : -clock;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return nla_put(skb, RTA_CACHEINFO, sizeof(ci), &ci);
}
EXPORT_SYMBOL_GPL(rtnl_put_cacheinfo);

<<<<<<< HEAD
static void set_operstate(struct net_device *dev, unsigned char transition)
{
	unsigned char operstate = dev->operstate;
=======
void netdev_set_operstate(struct net_device *dev, int newstate)
{
	unsigned int old = READ_ONCE(dev->operstate);

	do {
		if (old == newstate)
			return;
	} while (!try_cmpxchg(&dev->operstate, &old, newstate));

	netdev_state_change(dev);
}
EXPORT_SYMBOL(netdev_set_operstate);

static void set_operstate(struct net_device *dev, unsigned char transition)
{
	unsigned char operstate = READ_ONCE(dev->operstate);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (transition) {
	case IF_OPER_UP:
		if ((operstate == IF_OPER_DORMANT ||
<<<<<<< HEAD
		     operstate == IF_OPER_UNKNOWN) &&
		    !netif_dormant(dev))
			operstate = IF_OPER_UP;
		break;

	case IF_OPER_DORMANT:
		if (operstate == IF_OPER_UP ||
		    operstate == IF_OPER_UNKNOWN)
=======
		     operstate == IF_OPER_TESTING ||
		     operstate == IF_OPER_UNKNOWN) &&
		    !netif_dormant(dev) && !netif_testing(dev))
			operstate = IF_OPER_UP;
		break;

	case IF_OPER_TESTING:
		if (netif_oper_up(dev))
			operstate = IF_OPER_TESTING;
		break;

	case IF_OPER_DORMANT:
		if (netif_oper_up(dev))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			operstate = IF_OPER_DORMANT;
		break;
	}

<<<<<<< HEAD
	if (dev->operstate != operstate) {
		write_lock_bh(&dev_base_lock);
		dev->operstate = operstate;
		write_unlock_bh(&dev_base_lock);
		netdev_state_change(dev);
	}
=======
	netdev_set_operstate(dev, operstate);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static unsigned int rtnl_dev_get_flags(const struct net_device *dev)
{
	return (dev->flags & ~(IFF_PROMISC | IFF_ALLMULTI)) |
	       (dev->gflags & (IFF_PROMISC | IFF_ALLMULTI));
}

static unsigned int rtnl_dev_combine_flags(const struct net_device *dev,
					   const struct ifinfomsg *ifm)
{
	unsigned int flags = ifm->ifi_flags;

	/* bugwards compatibility: ifi_change == 0 is treated as ~0 */
	if (ifm->ifi_change)
		flags = (flags & ifm->ifi_change) |
			(rtnl_dev_get_flags(dev) & ~ifm->ifi_change);

	return flags;
}

static void copy_rtnl_link_stats(struct rtnl_link_stats *a,
				 const struct rtnl_link_stats64 *b)
{
	a->rx_packets = b->rx_packets;
	a->tx_packets = b->tx_packets;
	a->rx_bytes = b->rx_bytes;
	a->tx_bytes = b->tx_bytes;
	a->rx_errors = b->rx_errors;
	a->tx_errors = b->tx_errors;
	a->rx_dropped = b->rx_dropped;
	a->tx_dropped = b->tx_dropped;

	a->multicast = b->multicast;
	a->collisions = b->collisions;

	a->rx_length_errors = b->rx_length_errors;
	a->rx_over_errors = b->rx_over_errors;
	a->rx_crc_errors = b->rx_crc_errors;
	a->rx_frame_errors = b->rx_frame_errors;
	a->rx_fifo_errors = b->rx_fifo_errors;
	a->rx_missed_errors = b->rx_missed_errors;

	a->tx_aborted_errors = b->tx_aborted_errors;
	a->tx_carrier_errors = b->tx_carrier_errors;
	a->tx_fifo_errors = b->tx_fifo_errors;
	a->tx_heartbeat_errors = b->tx_heartbeat_errors;
	a->tx_window_errors = b->tx_window_errors;

	a->rx_compressed = b->rx_compressed;
	a->tx_compressed = b->tx_compressed;
<<<<<<< HEAD
}

static void copy_rtnl_link_stats64(void *v, const struct rtnl_link_stats64 *b)
{
	memcpy(v, b, sizeof(*b));
=======

	a->rx_nohandler = b->rx_nohandler;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* All VF info */
static inline int rtnl_vfinfo_size(const struct net_device *dev,
				   u32 ext_filter_mask)
{
<<<<<<< HEAD
	if (dev->dev.parent && dev_is_pci(dev->dev.parent) &&
	    (ext_filter_mask & RTEXT_FILTER_VF)) {
		int num_vfs = dev_num_vf(dev->dev.parent);
		size_t size = nla_total_size(sizeof(struct nlattr));
		size += nla_total_size(num_vfs * sizeof(struct nlattr));
		size += num_vfs *
			(nla_total_size(sizeof(struct ifla_vf_mac)) +
			 nla_total_size(sizeof(struct ifla_vf_vlan)) +
			 nla_total_size(sizeof(struct ifla_vf_tx_rate)) +
			 nla_total_size(sizeof(struct ifla_vf_spoofchk)));
=======
	if (dev->dev.parent && (ext_filter_mask & RTEXT_FILTER_VF)) {
		int num_vfs = dev_num_vf(dev->dev.parent);
		size_t size = nla_total_size(0);
		size += num_vfs *
			(nla_total_size(0) +
			 nla_total_size(sizeof(struct ifla_vf_mac)) +
			 nla_total_size(sizeof(struct ifla_vf_broadcast)) +
			 nla_total_size(sizeof(struct ifla_vf_vlan)) +
			 nla_total_size(0) + /* nest IFLA_VF_VLAN_LIST */
			 nla_total_size(MAX_VLAN_LIST_LEN *
					sizeof(struct ifla_vf_vlan_info)) +
			 nla_total_size(sizeof(struct ifla_vf_spoofchk)) +
			 nla_total_size(sizeof(struct ifla_vf_tx_rate)) +
			 nla_total_size(sizeof(struct ifla_vf_rate)) +
			 nla_total_size(sizeof(struct ifla_vf_link_state)) +
			 nla_total_size(sizeof(struct ifla_vf_rss_query_en)) +
			 nla_total_size(sizeof(struct ifla_vf_trust)));
		if (~ext_filter_mask & RTEXT_FILTER_SKIP_STATS) {
			size += num_vfs *
				(nla_total_size(0) + /* nest IFLA_VF_STATS */
				 /* IFLA_VF_STATS_RX_PACKETS */
				 nla_total_size_64bit(sizeof(__u64)) +
				 /* IFLA_VF_STATS_TX_PACKETS */
				 nla_total_size_64bit(sizeof(__u64)) +
				 /* IFLA_VF_STATS_RX_BYTES */
				 nla_total_size_64bit(sizeof(__u64)) +
				 /* IFLA_VF_STATS_TX_BYTES */
				 nla_total_size_64bit(sizeof(__u64)) +
				 /* IFLA_VF_STATS_BROADCAST */
				 nla_total_size_64bit(sizeof(__u64)) +
				 /* IFLA_VF_STATS_MULTICAST */
				 nla_total_size_64bit(sizeof(__u64)) +
				 /* IFLA_VF_STATS_RX_DROPPED */
				 nla_total_size_64bit(sizeof(__u64)) +
				 /* IFLA_VF_STATS_TX_DROPPED */
				 nla_total_size_64bit(sizeof(__u64)));
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return size;
	} else
		return 0;
}

static size_t rtnl_port_size(const struct net_device *dev,
			     u32 ext_filter_mask)
{
	size_t port_size = nla_total_size(4)		/* PORT_VF */
		+ nla_total_size(PORT_PROFILE_MAX)	/* PORT_PROFILE */
<<<<<<< HEAD
		+ nla_total_size(sizeof(struct ifla_port_vsi))
							/* PORT_VSI_TYPE */
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		+ nla_total_size(PORT_UUID_MAX)		/* PORT_INSTANCE_UUID */
		+ nla_total_size(PORT_UUID_MAX)		/* PORT_HOST_UUID */
		+ nla_total_size(1)			/* PROT_VDP_REQUEST */
		+ nla_total_size(2);			/* PORT_VDP_RESPONSE */
	size_t vf_ports_size = nla_total_size(sizeof(struct nlattr));
	size_t vf_port_size = nla_total_size(sizeof(struct nlattr))
		+ port_size;
	size_t port_self_size = nla_total_size(sizeof(struct nlattr))
		+ port_size;

	if (!dev->netdev_ops->ndo_get_vf_port || !dev->dev.parent ||
	    !(ext_filter_mask & RTEXT_FILTER_VF))
		return 0;
	if (dev_num_vf(dev->dev.parent))
		return port_self_size + vf_ports_size +
			vf_port_size * dev_num_vf(dev->dev.parent);
	else
		return port_self_size;
}

<<<<<<< HEAD
=======
static size_t rtnl_xdp_size(void)
{
	size_t xdp_size = nla_total_size(0) +	/* nest IFLA_XDP */
			  nla_total_size(1) +	/* XDP_ATTACHED */
			  nla_total_size(4) +	/* XDP_PROG_ID (or 1st mode) */
			  nla_total_size(4);	/* XDP_<mode>_PROG_ID */

	return xdp_size;
}

static size_t rtnl_prop_list_size(const struct net_device *dev)
{
	struct netdev_name_node *name_node;
	unsigned int cnt = 0;

	rcu_read_lock();
	list_for_each_entry_rcu(name_node, &dev->name_node->list, list)
		cnt++;
	rcu_read_unlock();

	if (!cnt)
		return 0;

	return nla_total_size(0) + cnt * nla_total_size(ALTIFNAMSIZ);
}

static size_t rtnl_proto_down_size(const struct net_device *dev)
{
	size_t size = nla_total_size(1);

	if (dev->proto_down_reason)
		size += nla_total_size(0) + nla_total_size(4);

	return size;
}

static size_t rtnl_devlink_port_size(const struct net_device *dev)
{
	size_t size = nla_total_size(0); /* nest IFLA_DEVLINK_PORT */

	if (dev->devlink_port)
		size += devlink_nl_port_handle_size(dev->devlink_port);

	return size;
}

static size_t rtnl_dpll_pin_size(const struct net_device *dev)
{
	size_t size = nla_total_size(0); /* nest IFLA_DPLL_PIN */

	size += dpll_netdev_pin_handle_size(dev);

	return size;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static noinline size_t if_nlmsg_size(const struct net_device *dev,
				     u32 ext_filter_mask)
{
	return NLMSG_ALIGN(sizeof(struct ifinfomsg))
	       + nla_total_size(IFNAMSIZ) /* IFLA_IFNAME */
	       + nla_total_size(IFALIASZ) /* IFLA_IFALIAS */
	       + nla_total_size(IFNAMSIZ) /* IFLA_QDISC */
<<<<<<< HEAD
	       + nla_total_size(sizeof(struct rtnl_link_ifmap))
	       + nla_total_size(sizeof(struct rtnl_link_stats))
	       + nla_total_size(sizeof(struct rtnl_link_stats64))
=======
	       + nla_total_size_64bit(sizeof(struct rtnl_link_ifmap))
	       + nla_total_size(sizeof(struct rtnl_link_stats))
	       + nla_total_size_64bit(sizeof(struct rtnl_link_stats64))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	       + nla_total_size(MAX_ADDR_LEN) /* IFLA_ADDRESS */
	       + nla_total_size(MAX_ADDR_LEN) /* IFLA_BROADCAST */
	       + nla_total_size(4) /* IFLA_TXQLEN */
	       + nla_total_size(4) /* IFLA_WEIGHT */
	       + nla_total_size(4) /* IFLA_MTU */
	       + nla_total_size(4) /* IFLA_LINK */
	       + nla_total_size(4) /* IFLA_MASTER */
<<<<<<< HEAD
	       + nla_total_size(1) /* IFLA_OPERSTATE */
	       + nla_total_size(1) /* IFLA_LINKMODE */
=======
	       + nla_total_size(1) /* IFLA_CARRIER */
	       + nla_total_size(4) /* IFLA_PROMISCUITY */
	       + nla_total_size(4) /* IFLA_ALLMULTI */
	       + nla_total_size(4) /* IFLA_NUM_TX_QUEUES */
	       + nla_total_size(4) /* IFLA_NUM_RX_QUEUES */
	       + nla_total_size(4) /* IFLA_GSO_MAX_SEGS */
	       + nla_total_size(4) /* IFLA_GSO_MAX_SIZE */
	       + nla_total_size(4) /* IFLA_GRO_MAX_SIZE */
	       + nla_total_size(4) /* IFLA_GSO_IPV4_MAX_SIZE */
	       + nla_total_size(4) /* IFLA_GRO_IPV4_MAX_SIZE */
	       + nla_total_size(4) /* IFLA_TSO_MAX_SIZE */
	       + nla_total_size(4) /* IFLA_TSO_MAX_SEGS */
	       + nla_total_size(1) /* IFLA_OPERSTATE */
	       + nla_total_size(1) /* IFLA_LINKMODE */
	       + nla_total_size(4) /* IFLA_CARRIER_CHANGES */
	       + nla_total_size(4) /* IFLA_LINK_NETNSID */
	       + nla_total_size(4) /* IFLA_GROUP */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	       + nla_total_size(ext_filter_mask
			        & RTEXT_FILTER_VF ? 4 : 0) /* IFLA_NUM_VF */
	       + rtnl_vfinfo_size(dev, ext_filter_mask) /* IFLA_VFINFO_LIST */
	       + rtnl_port_size(dev, ext_filter_mask) /* IFLA_VF_PORTS + IFLA_PORT_SELF */
	       + rtnl_link_get_size(dev) /* IFLA_LINKINFO */
<<<<<<< HEAD
	       + rtnl_link_get_af_size(dev); /* IFLA_AF_SPEC */
=======
	       + rtnl_link_get_af_size(dev, ext_filter_mask) /* IFLA_AF_SPEC */
	       + nla_total_size(MAX_PHYS_ITEM_ID_LEN) /* IFLA_PHYS_PORT_ID */
	       + nla_total_size(MAX_PHYS_ITEM_ID_LEN) /* IFLA_PHYS_SWITCH_ID */
	       + nla_total_size(IFNAMSIZ) /* IFLA_PHYS_PORT_NAME */
	       + rtnl_xdp_size() /* IFLA_XDP */
	       + nla_total_size(4)  /* IFLA_EVENT */
	       + nla_total_size(4)  /* IFLA_NEW_NETNSID */
	       + nla_total_size(4)  /* IFLA_NEW_IFINDEX */
	       + rtnl_proto_down_size(dev)  /* proto down */
	       + nla_total_size(4)  /* IFLA_TARGET_NETNSID */
	       + nla_total_size(4)  /* IFLA_CARRIER_UP_COUNT */
	       + nla_total_size(4)  /* IFLA_CARRIER_DOWN_COUNT */
	       + nla_total_size(4)  /* IFLA_MIN_MTU */
	       + nla_total_size(4)  /* IFLA_MAX_MTU */
	       + rtnl_prop_list_size(dev)
	       + nla_total_size(MAX_ADDR_LEN) /* IFLA_PERM_ADDRESS */
	       + rtnl_devlink_port_size(dev)
	       + rtnl_dpll_pin_size(dev)
	       + 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int rtnl_vf_ports_fill(struct sk_buff *skb, struct net_device *dev)
{
	struct nlattr *vf_ports;
	struct nlattr *vf_port;
	int vf;
	int err;

<<<<<<< HEAD
	vf_ports = nla_nest_start(skb, IFLA_VF_PORTS);
=======
	vf_ports = nla_nest_start_noflag(skb, IFLA_VF_PORTS);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!vf_ports)
		return -EMSGSIZE;

	for (vf = 0; vf < dev_num_vf(dev->dev.parent); vf++) {
<<<<<<< HEAD
		vf_port = nla_nest_start(skb, IFLA_VF_PORT);
		if (!vf_port)
			goto nla_put_failure;
		NLA_PUT_U32(skb, IFLA_PORT_VF, vf);
=======
		vf_port = nla_nest_start_noflag(skb, IFLA_VF_PORT);
		if (!vf_port)
			goto nla_put_failure;
		if (nla_put_u32(skb, IFLA_PORT_VF, vf))
			goto nla_put_failure;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = dev->netdev_ops->ndo_get_vf_port(dev, vf, skb);
		if (err == -EMSGSIZE)
			goto nla_put_failure;
		if (err) {
			nla_nest_cancel(skb, vf_port);
			continue;
		}
		nla_nest_end(skb, vf_port);
	}

	nla_nest_end(skb, vf_ports);

	return 0;

nla_put_failure:
	nla_nest_cancel(skb, vf_ports);
	return -EMSGSIZE;
}

static int rtnl_port_self_fill(struct sk_buff *skb, struct net_device *dev)
{
	struct nlattr *port_self;
	int err;

<<<<<<< HEAD
	port_self = nla_nest_start(skb, IFLA_PORT_SELF);
=======
	port_self = nla_nest_start_noflag(skb, IFLA_PORT_SELF);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!port_self)
		return -EMSGSIZE;

	err = dev->netdev_ops->ndo_get_vf_port(dev, PORT_SELF_VF, skb);
	if (err) {
		nla_nest_cancel(skb, port_self);
		return (err == -EMSGSIZE) ? err : 0;
	}

	nla_nest_end(skb, port_self);

	return 0;
}

static int rtnl_port_fill(struct sk_buff *skb, struct net_device *dev,
			  u32 ext_filter_mask)
{
	int err;

	if (!dev->netdev_ops->ndo_get_vf_port || !dev->dev.parent ||
	    !(ext_filter_mask & RTEXT_FILTER_VF))
		return 0;

	err = rtnl_port_self_fill(skb, dev);
	if (err)
		return err;

	if (dev_num_vf(dev->dev.parent)) {
		err = rtnl_vf_ports_fill(skb, dev);
		if (err)
			return err;
	}

	return 0;
}

<<<<<<< HEAD
static int rtnl_fill_ifinfo(struct sk_buff *skb, struct net_device *dev,
			    int type, u32 pid, u32 seq, u32 change,
			    unsigned int flags, u32 ext_filter_mask)
{
	struct ifinfomsg *ifm;
	struct nlmsghdr *nlh;
	struct rtnl_link_stats64 temp;
	const struct rtnl_link_stats64 *stats;
	struct nlattr *attr, *af_spec;
	struct rtnl_af_ops *af_ops;
=======
static int rtnl_phys_port_id_fill(struct sk_buff *skb, struct net_device *dev)
{
	int err;
	struct netdev_phys_item_id ppid;

	err = dev_get_phys_port_id(dev, &ppid);
	if (err) {
		if (err == -EOPNOTSUPP)
			return 0;
		return err;
	}

	if (nla_put(skb, IFLA_PHYS_PORT_ID, ppid.id_len, ppid.id))
		return -EMSGSIZE;

	return 0;
}

static int rtnl_phys_port_name_fill(struct sk_buff *skb, struct net_device *dev)
{
	char name[IFNAMSIZ];
	int err;

	err = dev_get_phys_port_name(dev, name, sizeof(name));
	if (err) {
		if (err == -EOPNOTSUPP)
			return 0;
		return err;
	}

	if (nla_put_string(skb, IFLA_PHYS_PORT_NAME, name))
		return -EMSGSIZE;

	return 0;
}

static int rtnl_phys_switch_id_fill(struct sk_buff *skb, struct net_device *dev)
{
	struct netdev_phys_item_id ppid = { };
	int err;

	err = dev_get_port_parent_id(dev, &ppid, false);
	if (err) {
		if (err == -EOPNOTSUPP)
			return 0;
		return err;
	}

	if (nla_put(skb, IFLA_PHYS_SWITCH_ID, ppid.id_len, ppid.id))
		return -EMSGSIZE;

	return 0;
}

static noinline_for_stack int rtnl_fill_stats(struct sk_buff *skb,
					      struct net_device *dev)
{
	struct rtnl_link_stats64 *sp;
	struct nlattr *attr;

	attr = nla_reserve_64bit(skb, IFLA_STATS64,
				 sizeof(struct rtnl_link_stats64), IFLA_PAD);
	if (!attr)
		return -EMSGSIZE;

	sp = nla_data(attr);
	dev_get_stats(dev, sp);

	attr = nla_reserve(skb, IFLA_STATS,
			   sizeof(struct rtnl_link_stats));
	if (!attr)
		return -EMSGSIZE;

	copy_rtnl_link_stats(nla_data(attr), sp);

	return 0;
}

static noinline_for_stack int rtnl_fill_vfinfo(struct sk_buff *skb,
					       struct net_device *dev,
					       int vfs_num,
					       u32 ext_filter_mask)
{
	struct ifla_vf_rss_query_en vf_rss_query_en;
	struct nlattr *vf, *vfstats, *vfvlanlist;
	struct ifla_vf_link_state vf_linkstate;
	struct ifla_vf_vlan_info vf_vlan_info;
	struct ifla_vf_spoofchk vf_spoofchk;
	struct ifla_vf_tx_rate vf_tx_rate;
	struct ifla_vf_stats vf_stats;
	struct ifla_vf_trust vf_trust;
	struct ifla_vf_vlan vf_vlan;
	struct ifla_vf_rate vf_rate;
	struct ifla_vf_mac vf_mac;
	struct ifla_vf_broadcast vf_broadcast;
	struct ifla_vf_info ivi;
	struct ifla_vf_guid node_guid;
	struct ifla_vf_guid port_guid;

	memset(&ivi, 0, sizeof(ivi));

	/* Not all SR-IOV capable drivers support the
	 * spoofcheck and "RSS query enable" query.  Preset to
	 * -1 so the user space tool can detect that the driver
	 * didn't report anything.
	 */
	ivi.spoofchk = -1;
	ivi.rss_query_en = -1;
	ivi.trusted = -1;
	/* The default value for VF link state is "auto"
	 * IFLA_VF_LINK_STATE_AUTO which equals zero
	 */
	ivi.linkstate = 0;
	/* VLAN Protocol by default is 802.1Q */
	ivi.vlan_proto = htons(ETH_P_8021Q);
	if (dev->netdev_ops->ndo_get_vf_config(dev, vfs_num, &ivi))
		return 0;

	memset(&vf_vlan_info, 0, sizeof(vf_vlan_info));
	memset(&node_guid, 0, sizeof(node_guid));
	memset(&port_guid, 0, sizeof(port_guid));

	vf_mac.vf =
		vf_vlan.vf =
		vf_vlan_info.vf =
		vf_rate.vf =
		vf_tx_rate.vf =
		vf_spoofchk.vf =
		vf_linkstate.vf =
		vf_rss_query_en.vf =
		vf_trust.vf =
		node_guid.vf =
		port_guid.vf = ivi.vf;

	memcpy(vf_mac.mac, ivi.mac, sizeof(ivi.mac));
	memcpy(vf_broadcast.broadcast, dev->broadcast, dev->addr_len);
	vf_vlan.vlan = ivi.vlan;
	vf_vlan.qos = ivi.qos;
	vf_vlan_info.vlan = ivi.vlan;
	vf_vlan_info.qos = ivi.qos;
	vf_vlan_info.vlan_proto = ivi.vlan_proto;
	vf_tx_rate.rate = ivi.max_tx_rate;
	vf_rate.min_tx_rate = ivi.min_tx_rate;
	vf_rate.max_tx_rate = ivi.max_tx_rate;
	vf_spoofchk.setting = ivi.spoofchk;
	vf_linkstate.link_state = ivi.linkstate;
	vf_rss_query_en.setting = ivi.rss_query_en;
	vf_trust.setting = ivi.trusted;
	vf = nla_nest_start_noflag(skb, IFLA_VF_INFO);
	if (!vf)
		return -EMSGSIZE;
	if (nla_put(skb, IFLA_VF_MAC, sizeof(vf_mac), &vf_mac) ||
	    nla_put(skb, IFLA_VF_BROADCAST, sizeof(vf_broadcast), &vf_broadcast) ||
	    nla_put(skb, IFLA_VF_VLAN, sizeof(vf_vlan), &vf_vlan) ||
	    nla_put(skb, IFLA_VF_RATE, sizeof(vf_rate),
		    &vf_rate) ||
	    nla_put(skb, IFLA_VF_TX_RATE, sizeof(vf_tx_rate),
		    &vf_tx_rate) ||
	    nla_put(skb, IFLA_VF_SPOOFCHK, sizeof(vf_spoofchk),
		    &vf_spoofchk) ||
	    nla_put(skb, IFLA_VF_LINK_STATE, sizeof(vf_linkstate),
		    &vf_linkstate) ||
	    nla_put(skb, IFLA_VF_RSS_QUERY_EN,
		    sizeof(vf_rss_query_en),
		    &vf_rss_query_en) ||
	    nla_put(skb, IFLA_VF_TRUST,
		    sizeof(vf_trust), &vf_trust))
		goto nla_put_vf_failure;

	if (dev->netdev_ops->ndo_get_vf_guid &&
	    !dev->netdev_ops->ndo_get_vf_guid(dev, vfs_num, &node_guid,
					      &port_guid)) {
		if (nla_put(skb, IFLA_VF_IB_NODE_GUID, sizeof(node_guid),
			    &node_guid) ||
		    nla_put(skb, IFLA_VF_IB_PORT_GUID, sizeof(port_guid),
			    &port_guid))
			goto nla_put_vf_failure;
	}
	vfvlanlist = nla_nest_start_noflag(skb, IFLA_VF_VLAN_LIST);
	if (!vfvlanlist)
		goto nla_put_vf_failure;
	if (nla_put(skb, IFLA_VF_VLAN_INFO, sizeof(vf_vlan_info),
		    &vf_vlan_info)) {
		nla_nest_cancel(skb, vfvlanlist);
		goto nla_put_vf_failure;
	}
	nla_nest_end(skb, vfvlanlist);
	if (~ext_filter_mask & RTEXT_FILTER_SKIP_STATS) {
		memset(&vf_stats, 0, sizeof(vf_stats));
		if (dev->netdev_ops->ndo_get_vf_stats)
			dev->netdev_ops->ndo_get_vf_stats(dev, vfs_num,
							  &vf_stats);
		vfstats = nla_nest_start_noflag(skb, IFLA_VF_STATS);
		if (!vfstats)
			goto nla_put_vf_failure;
		if (nla_put_u64_64bit(skb, IFLA_VF_STATS_RX_PACKETS,
				      vf_stats.rx_packets, IFLA_VF_STATS_PAD) ||
		    nla_put_u64_64bit(skb, IFLA_VF_STATS_TX_PACKETS,
				      vf_stats.tx_packets, IFLA_VF_STATS_PAD) ||
		    nla_put_u64_64bit(skb, IFLA_VF_STATS_RX_BYTES,
				      vf_stats.rx_bytes, IFLA_VF_STATS_PAD) ||
		    nla_put_u64_64bit(skb, IFLA_VF_STATS_TX_BYTES,
				      vf_stats.tx_bytes, IFLA_VF_STATS_PAD) ||
		    nla_put_u64_64bit(skb, IFLA_VF_STATS_BROADCAST,
				      vf_stats.broadcast, IFLA_VF_STATS_PAD) ||
		    nla_put_u64_64bit(skb, IFLA_VF_STATS_MULTICAST,
				      vf_stats.multicast, IFLA_VF_STATS_PAD) ||
		    nla_put_u64_64bit(skb, IFLA_VF_STATS_RX_DROPPED,
				      vf_stats.rx_dropped, IFLA_VF_STATS_PAD) ||
		    nla_put_u64_64bit(skb, IFLA_VF_STATS_TX_DROPPED,
				      vf_stats.tx_dropped, IFLA_VF_STATS_PAD)) {
			nla_nest_cancel(skb, vfstats);
			goto nla_put_vf_failure;
		}
		nla_nest_end(skb, vfstats);
	}
	nla_nest_end(skb, vf);
	return 0;

nla_put_vf_failure:
	nla_nest_cancel(skb, vf);
	return -EMSGSIZE;
}

static noinline_for_stack int rtnl_fill_vf(struct sk_buff *skb,
					   struct net_device *dev,
					   u32 ext_filter_mask)
{
	struct nlattr *vfinfo;
	int i, num_vfs;

	if (!dev->dev.parent || ((ext_filter_mask & RTEXT_FILTER_VF) == 0))
		return 0;

	num_vfs = dev_num_vf(dev->dev.parent);
	if (nla_put_u32(skb, IFLA_NUM_VF, num_vfs))
		return -EMSGSIZE;

	if (!dev->netdev_ops->ndo_get_vf_config)
		return 0;

	vfinfo = nla_nest_start_noflag(skb, IFLA_VFINFO_LIST);
	if (!vfinfo)
		return -EMSGSIZE;

	for (i = 0; i < num_vfs; i++) {
		if (rtnl_fill_vfinfo(skb, dev, i, ext_filter_mask)) {
			nla_nest_cancel(skb, vfinfo);
			return -EMSGSIZE;
		}
	}

	nla_nest_end(skb, vfinfo);
	return 0;
}

static int rtnl_fill_link_ifmap(struct sk_buff *skb,
				const struct net_device *dev)
{
	struct rtnl_link_ifmap map;

	memset(&map, 0, sizeof(map));
	map.mem_start = READ_ONCE(dev->mem_start);
	map.mem_end   = READ_ONCE(dev->mem_end);
	map.base_addr = READ_ONCE(dev->base_addr);
	map.irq       = READ_ONCE(dev->irq);
	map.dma       = READ_ONCE(dev->dma);
	map.port      = READ_ONCE(dev->if_port);

	if (nla_put_64bit(skb, IFLA_MAP, sizeof(map), &map, IFLA_PAD))
		return -EMSGSIZE;

	return 0;
}

static u32 rtnl_xdp_prog_skb(struct net_device *dev)
{
	const struct bpf_prog *generic_xdp_prog;

	ASSERT_RTNL();

	generic_xdp_prog = rtnl_dereference(dev->xdp_prog);
	if (!generic_xdp_prog)
		return 0;
	return generic_xdp_prog->aux->id;
}

static u32 rtnl_xdp_prog_drv(struct net_device *dev)
{
	return dev_xdp_prog_id(dev, XDP_MODE_DRV);
}

static u32 rtnl_xdp_prog_hw(struct net_device *dev)
{
	return dev_xdp_prog_id(dev, XDP_MODE_HW);
}

static int rtnl_xdp_report_one(struct sk_buff *skb, struct net_device *dev,
			       u32 *prog_id, u8 *mode, u8 tgt_mode, u32 attr,
			       u32 (*get_prog_id)(struct net_device *dev))
{
	u32 curr_id;
	int err;

	curr_id = get_prog_id(dev);
	if (!curr_id)
		return 0;

	*prog_id = curr_id;
	err = nla_put_u32(skb, attr, curr_id);
	if (err)
		return err;

	if (*mode != XDP_ATTACHED_NONE)
		*mode = XDP_ATTACHED_MULTI;
	else
		*mode = tgt_mode;

	return 0;
}

static int rtnl_xdp_fill(struct sk_buff *skb, struct net_device *dev)
{
	struct nlattr *xdp;
	u32 prog_id;
	int err;
	u8 mode;

	xdp = nla_nest_start_noflag(skb, IFLA_XDP);
	if (!xdp)
		return -EMSGSIZE;

	prog_id = 0;
	mode = XDP_ATTACHED_NONE;
	err = rtnl_xdp_report_one(skb, dev, &prog_id, &mode, XDP_ATTACHED_SKB,
				  IFLA_XDP_SKB_PROG_ID, rtnl_xdp_prog_skb);
	if (err)
		goto err_cancel;
	err = rtnl_xdp_report_one(skb, dev, &prog_id, &mode, XDP_ATTACHED_DRV,
				  IFLA_XDP_DRV_PROG_ID, rtnl_xdp_prog_drv);
	if (err)
		goto err_cancel;
	err = rtnl_xdp_report_one(skb, dev, &prog_id, &mode, XDP_ATTACHED_HW,
				  IFLA_XDP_HW_PROG_ID, rtnl_xdp_prog_hw);
	if (err)
		goto err_cancel;

	err = nla_put_u8(skb, IFLA_XDP_ATTACHED, mode);
	if (err)
		goto err_cancel;

	if (prog_id && mode != XDP_ATTACHED_MULTI) {
		err = nla_put_u32(skb, IFLA_XDP_PROG_ID, prog_id);
		if (err)
			goto err_cancel;
	}

	nla_nest_end(skb, xdp);
	return 0;

err_cancel:
	nla_nest_cancel(skb, xdp);
	return err;
}

static u32 rtnl_get_event(unsigned long event)
{
	u32 rtnl_event_type = IFLA_EVENT_NONE;

	switch (event) {
	case NETDEV_REBOOT:
		rtnl_event_type = IFLA_EVENT_REBOOT;
		break;
	case NETDEV_FEAT_CHANGE:
		rtnl_event_type = IFLA_EVENT_FEATURES;
		break;
	case NETDEV_BONDING_FAILOVER:
		rtnl_event_type = IFLA_EVENT_BONDING_FAILOVER;
		break;
	case NETDEV_NOTIFY_PEERS:
		rtnl_event_type = IFLA_EVENT_NOTIFY_PEERS;
		break;
	case NETDEV_RESEND_IGMP:
		rtnl_event_type = IFLA_EVENT_IGMP_RESEND;
		break;
	case NETDEV_CHANGEINFODATA:
		rtnl_event_type = IFLA_EVENT_BONDING_OPTIONS;
		break;
	default:
		break;
	}

	return rtnl_event_type;
}

static int put_master_ifindex(struct sk_buff *skb, struct net_device *dev)
{
	const struct net_device *upper_dev;
	int ret = 0;

	rcu_read_lock();

	upper_dev = netdev_master_upper_dev_get_rcu(dev);
	if (upper_dev)
		ret = nla_put_u32(skb, IFLA_MASTER, upper_dev->ifindex);

	rcu_read_unlock();
	return ret;
}

static int nla_put_iflink(struct sk_buff *skb, const struct net_device *dev,
			  bool force)
{
	int iflink = dev_get_iflink(dev);

	if (force || READ_ONCE(dev->ifindex) != iflink)
		return nla_put_u32(skb, IFLA_LINK, iflink);

	return 0;
}

static noinline_for_stack int nla_put_ifalias(struct sk_buff *skb,
					      struct net_device *dev)
{
	char buf[IFALIASZ];
	int ret;

	ret = dev_get_alias(dev, buf, sizeof(buf));
	return ret > 0 ? nla_put_string(skb, IFLA_IFALIAS, buf) : 0;
}

static int rtnl_fill_link_netnsid(struct sk_buff *skb,
				  const struct net_device *dev,
				  struct net *src_net, gfp_t gfp)
{
	bool put_iflink = false;

	if (dev->rtnl_link_ops && dev->rtnl_link_ops->get_link_net) {
		struct net *link_net = dev->rtnl_link_ops->get_link_net(dev);

		if (!net_eq(dev_net(dev), link_net)) {
			int id = peernet2id_alloc(src_net, link_net, gfp);

			if (nla_put_s32(skb, IFLA_LINK_NETNSID, id))
				return -EMSGSIZE;

			put_iflink = true;
		}
	}

	return nla_put_iflink(skb, dev, put_iflink);
}

static int rtnl_fill_link_af(struct sk_buff *skb,
			     const struct net_device *dev,
			     u32 ext_filter_mask)
{
	const struct rtnl_af_ops *af_ops;
	struct nlattr *af_spec;

	af_spec = nla_nest_start_noflag(skb, IFLA_AF_SPEC);
	if (!af_spec)
		return -EMSGSIZE;

	list_for_each_entry_rcu(af_ops, &rtnl_af_ops, list) {
		struct nlattr *af;
		int err;

		if (!af_ops->fill_link_af)
			continue;

		af = nla_nest_start_noflag(skb, af_ops->family);
		if (!af)
			return -EMSGSIZE;

		err = af_ops->fill_link_af(skb, dev, ext_filter_mask);
		/*
		 * Caller may return ENODATA to indicate that there
		 * was no data to be dumped. This is not an error, it
		 * means we should trim the attribute header and
		 * continue.
		 */
		if (err == -ENODATA)
			nla_nest_cancel(skb, af);
		else if (err < 0)
			return -EMSGSIZE;

		nla_nest_end(skb, af);
	}

	nla_nest_end(skb, af_spec);
	return 0;
}

static int rtnl_fill_alt_ifnames(struct sk_buff *skb,
				 const struct net_device *dev)
{
	struct netdev_name_node *name_node;
	int count = 0;

	list_for_each_entry_rcu(name_node, &dev->name_node->list, list) {
		if (nla_put_string(skb, IFLA_ALT_IFNAME, name_node->name))
			return -EMSGSIZE;
		count++;
	}
	return count;
}

/* RCU protected. */
static int rtnl_fill_prop_list(struct sk_buff *skb,
			       const struct net_device *dev)
{
	struct nlattr *prop_list;
	int ret;

	prop_list = nla_nest_start(skb, IFLA_PROP_LIST);
	if (!prop_list)
		return -EMSGSIZE;

	ret = rtnl_fill_alt_ifnames(skb, dev);
	if (ret <= 0)
		goto nest_cancel;

	nla_nest_end(skb, prop_list);
	return 0;

nest_cancel:
	nla_nest_cancel(skb, prop_list);
	return ret;
}

static int rtnl_fill_proto_down(struct sk_buff *skb,
				const struct net_device *dev)
{
	struct nlattr *pr;
	u32 preason;

	if (nla_put_u8(skb, IFLA_PROTO_DOWN, dev->proto_down))
		goto nla_put_failure;

	preason = dev->proto_down_reason;
	if (!preason)
		return 0;

	pr = nla_nest_start(skb, IFLA_PROTO_DOWN_REASON);
	if (!pr)
		return -EMSGSIZE;

	if (nla_put_u32(skb, IFLA_PROTO_DOWN_REASON_VALUE, preason)) {
		nla_nest_cancel(skb, pr);
		goto nla_put_failure;
	}

	nla_nest_end(skb, pr);
	return 0;

nla_put_failure:
	return -EMSGSIZE;
}

static int rtnl_fill_devlink_port(struct sk_buff *skb,
				  const struct net_device *dev)
{
	struct nlattr *devlink_port_nest;
	int ret;

	devlink_port_nest = nla_nest_start(skb, IFLA_DEVLINK_PORT);
	if (!devlink_port_nest)
		return -EMSGSIZE;

	if (dev->devlink_port) {
		ret = devlink_nl_port_handle_fill(skb, dev->devlink_port);
		if (ret < 0)
			goto nest_cancel;
	}

	nla_nest_end(skb, devlink_port_nest);
	return 0;

nest_cancel:
	nla_nest_cancel(skb, devlink_port_nest);
	return ret;
}

static int rtnl_fill_dpll_pin(struct sk_buff *skb,
			      const struct net_device *dev)
{
	struct nlattr *dpll_pin_nest;
	int ret;

	dpll_pin_nest = nla_nest_start(skb, IFLA_DPLL_PIN);
	if (!dpll_pin_nest)
		return -EMSGSIZE;

	ret = dpll_netdev_add_pin_handle(skb, dev);
	if (ret < 0)
		goto nest_cancel;

	nla_nest_end(skb, dpll_pin_nest);
	return 0;

nest_cancel:
	nla_nest_cancel(skb, dpll_pin_nest);
	return ret;
}

static int rtnl_fill_ifinfo(struct sk_buff *skb,
			    struct net_device *dev, struct net *src_net,
			    int type, u32 pid, u32 seq, u32 change,
			    unsigned int flags, u32 ext_filter_mask,
			    u32 event, int *new_nsid, int new_ifindex,
			    int tgt_netnsid, gfp_t gfp)
{
	struct ifinfomsg *ifm;
	struct nlmsghdr *nlh;
	struct Qdisc *qdisc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ASSERT_RTNL();
	nlh = nlmsg_put(skb, pid, seq, type, sizeof(*ifm), flags);
	if (nlh == NULL)
		return -EMSGSIZE;

	ifm = nlmsg_data(nlh);
	ifm->ifi_family = AF_UNSPEC;
	ifm->__ifi_pad = 0;
	ifm->ifi_type = dev->type;
	ifm->ifi_index = dev->ifindex;
	ifm->ifi_flags = dev_get_flags(dev);
	ifm->ifi_change = change;

<<<<<<< HEAD
	NLA_PUT_STRING(skb, IFLA_IFNAME, dev->name);
	NLA_PUT_U32(skb, IFLA_TXQLEN, dev->tx_queue_len);
	NLA_PUT_U8(skb, IFLA_OPERSTATE,
		   netif_running(dev) ? dev->operstate : IF_OPER_DOWN);
	NLA_PUT_U8(skb, IFLA_LINKMODE, dev->link_mode);
	NLA_PUT_U32(skb, IFLA_MTU, dev->mtu);
	NLA_PUT_U32(skb, IFLA_GROUP, dev->group);

	if (dev->ifindex != dev->iflink)
		NLA_PUT_U32(skb, IFLA_LINK, dev->iflink);

	if (dev->master)
		NLA_PUT_U32(skb, IFLA_MASTER, dev->master->ifindex);

	if (dev->qdisc)
		NLA_PUT_STRING(skb, IFLA_QDISC, dev->qdisc->ops->id);

	if (dev->ifalias)
		NLA_PUT_STRING(skb, IFLA_IFALIAS, dev->ifalias);

	if (1) {
		struct rtnl_link_ifmap map;
		memset(&map, 0, sizeof(map));
		map.mem_start   = dev->mem_start;
		map.mem_end     = dev->mem_end;
		map.base_addr   = dev->base_addr;
		map.irq         = dev->irq;
		map.dma         = dev->dma;
		map.port        = dev->if_port;
		NLA_PUT(skb, IFLA_MAP, sizeof(map), &map);
	}

	if (dev->addr_len) {
		NLA_PUT(skb, IFLA_ADDRESS, dev->addr_len, dev->dev_addr);
		NLA_PUT(skb, IFLA_BROADCAST, dev->addr_len, dev->broadcast);
	}

	attr = nla_reserve(skb, IFLA_STATS,
			sizeof(struct rtnl_link_stats));
	if (attr == NULL)
		goto nla_put_failure;

	stats = dev_get_stats(dev, &temp);
	copy_rtnl_link_stats(nla_data(attr), stats);

	attr = nla_reserve(skb, IFLA_STATS64,
			sizeof(struct rtnl_link_stats64));
	if (attr == NULL)
		goto nla_put_failure;
	copy_rtnl_link_stats64(nla_data(attr), stats);

	if (dev->dev.parent && (ext_filter_mask & RTEXT_FILTER_VF))
		NLA_PUT_U32(skb, IFLA_NUM_VF, dev_num_vf(dev->dev.parent));

	if (dev->netdev_ops->ndo_get_vf_config && dev->dev.parent
	    && (ext_filter_mask & RTEXT_FILTER_VF)) {
		int i;

		struct nlattr *vfinfo, *vf;
		int num_vfs = dev_num_vf(dev->dev.parent);

		vfinfo = nla_nest_start(skb, IFLA_VFINFO_LIST);
		if (!vfinfo)
			goto nla_put_failure;
		for (i = 0; i < num_vfs; i++) {
			struct ifla_vf_info ivi;
			struct ifla_vf_mac vf_mac;
			struct ifla_vf_vlan vf_vlan;
			struct ifla_vf_tx_rate vf_tx_rate;
			struct ifla_vf_spoofchk vf_spoofchk;

			/*
			 * Not all SR-IOV capable drivers support the
			 * spoofcheck query.  Preset to -1 so the user
			 * space tool can detect that the driver didn't
			 * report anything.
			 */
			ivi.spoofchk = -1;
			memset(ivi.mac, 0, sizeof(ivi.mac));
			if (dev->netdev_ops->ndo_get_vf_config(dev, i, &ivi))
				break;
			vf_mac.vf =
				vf_vlan.vf =
				vf_tx_rate.vf =
				vf_spoofchk.vf = ivi.vf;

			memcpy(vf_mac.mac, ivi.mac, sizeof(ivi.mac));
			vf_vlan.vlan = ivi.vlan;
			vf_vlan.qos = ivi.qos;
			vf_tx_rate.rate = ivi.tx_rate;
			vf_spoofchk.setting = ivi.spoofchk;
			vf = nla_nest_start(skb, IFLA_VF_INFO);
			if (!vf) {
				nla_nest_cancel(skb, vfinfo);
				goto nla_put_failure;
			}
			NLA_PUT(skb, IFLA_VF_MAC, sizeof(vf_mac), &vf_mac);
			NLA_PUT(skb, IFLA_VF_VLAN, sizeof(vf_vlan), &vf_vlan);
			NLA_PUT(skb, IFLA_VF_TX_RATE, sizeof(vf_tx_rate),
				&vf_tx_rate);
			NLA_PUT(skb, IFLA_VF_SPOOFCHK, sizeof(vf_spoofchk),
				&vf_spoofchk);
			nla_nest_end(skb, vf);
		}
		nla_nest_end(skb, vfinfo);
	}

	if (rtnl_port_fill(skb, dev, ext_filter_mask))
		goto nla_put_failure;

	if (dev->rtnl_link_ops) {
=======
	if (tgt_netnsid >= 0 && nla_put_s32(skb, IFLA_TARGET_NETNSID, tgt_netnsid))
		goto nla_put_failure;

	qdisc = rtnl_dereference(dev->qdisc);
	if (nla_put_string(skb, IFLA_IFNAME, dev->name) ||
	    nla_put_u32(skb, IFLA_TXQLEN, dev->tx_queue_len) ||
	    nla_put_u8(skb, IFLA_OPERSTATE,
		       netif_running(dev) ? dev->operstate : IF_OPER_DOWN) ||
	    nla_put_u8(skb, IFLA_LINKMODE, dev->link_mode) ||
	    nla_put_u32(skb, IFLA_MTU, dev->mtu) ||
	    nla_put_u32(skb, IFLA_MIN_MTU, dev->min_mtu) ||
	    nla_put_u32(skb, IFLA_MAX_MTU, dev->max_mtu) ||
	    nla_put_u32(skb, IFLA_GROUP, dev->group) ||
	    nla_put_u32(skb, IFLA_PROMISCUITY, dev->promiscuity) ||
	    nla_put_u32(skb, IFLA_ALLMULTI, dev->allmulti) ||
	    nla_put_u32(skb, IFLA_NUM_TX_QUEUES, dev->num_tx_queues) ||
	    nla_put_u32(skb, IFLA_GSO_MAX_SEGS, dev->gso_max_segs) ||
	    nla_put_u32(skb, IFLA_GSO_MAX_SIZE, dev->gso_max_size) ||
	    nla_put_u32(skb, IFLA_GRO_MAX_SIZE, dev->gro_max_size) ||
	    nla_put_u32(skb, IFLA_GSO_IPV4_MAX_SIZE, dev->gso_ipv4_max_size) ||
	    nla_put_u32(skb, IFLA_GRO_IPV4_MAX_SIZE, dev->gro_ipv4_max_size) ||
	    nla_put_u32(skb, IFLA_TSO_MAX_SIZE, dev->tso_max_size) ||
	    nla_put_u32(skb, IFLA_TSO_MAX_SEGS, dev->tso_max_segs) ||
#ifdef CONFIG_RPS
	    nla_put_u32(skb, IFLA_NUM_RX_QUEUES, dev->num_rx_queues) ||
#endif
	    put_master_ifindex(skb, dev) ||
	    nla_put_u8(skb, IFLA_CARRIER, netif_carrier_ok(dev)) ||
	    (qdisc &&
	     nla_put_string(skb, IFLA_QDISC, qdisc->ops->id)) ||
	    nla_put_ifalias(skb, dev) ||
	    nla_put_u32(skb, IFLA_CARRIER_CHANGES,
			atomic_read(&dev->carrier_up_count) +
			atomic_read(&dev->carrier_down_count)) ||
	    nla_put_u32(skb, IFLA_CARRIER_UP_COUNT,
			atomic_read(&dev->carrier_up_count)) ||
	    nla_put_u32(skb, IFLA_CARRIER_DOWN_COUNT,
			atomic_read(&dev->carrier_down_count)))
		goto nla_put_failure;

	if (rtnl_fill_proto_down(skb, dev))
		goto nla_put_failure;

	if (event != IFLA_EVENT_NONE) {
		if (nla_put_u32(skb, IFLA_EVENT, event))
			goto nla_put_failure;
	}

	if (dev->addr_len) {
		if (nla_put(skb, IFLA_ADDRESS, dev->addr_len, dev->dev_addr) ||
		    nla_put(skb, IFLA_BROADCAST, dev->addr_len, dev->broadcast))
			goto nla_put_failure;
	}

	if (rtnl_phys_port_id_fill(skb, dev))
		goto nla_put_failure;

	if (rtnl_phys_port_name_fill(skb, dev))
		goto nla_put_failure;

	if (rtnl_phys_switch_id_fill(skb, dev))
		goto nla_put_failure;

	if (rtnl_fill_stats(skb, dev))
		goto nla_put_failure;

	if (rtnl_fill_vf(skb, dev, ext_filter_mask))
		goto nla_put_failure;

	if (rtnl_port_fill(skb, dev, ext_filter_mask))
		goto nla_put_failure;

	if (rtnl_xdp_fill(skb, dev))
		goto nla_put_failure;

	if (dev->rtnl_link_ops || rtnl_have_link_slave_info(dev)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (rtnl_link_fill(skb, dev) < 0)
			goto nla_put_failure;
	}

<<<<<<< HEAD
	if (!(af_spec = nla_nest_start(skb, IFLA_AF_SPEC)))
		goto nla_put_failure;

	list_for_each_entry(af_ops, &rtnl_af_ops, list) {
		if (af_ops->fill_link_af) {
			struct nlattr *af;
			int err;

			if (!(af = nla_nest_start(skb, af_ops->family)))
				goto nla_put_failure;

			err = af_ops->fill_link_af(skb, dev);

			/*
			 * Caller may return ENODATA to indicate that there
			 * was no data to be dumped. This is not an error, it
			 * means we should trim the attribute header and
			 * continue.
			 */
			if (err == -ENODATA)
				nla_nest_cancel(skb, af);
			else if (err < 0)
				goto nla_put_failure;

			nla_nest_end(skb, af);
		}
	}

	nla_nest_end(skb, af_spec);

	return nlmsg_end(skb, nlh);

=======
	if (rtnl_fill_link_netnsid(skb, dev, src_net, gfp))
		goto nla_put_failure;

	if (new_nsid &&
	    nla_put_s32(skb, IFLA_NEW_NETNSID, *new_nsid) < 0)
		goto nla_put_failure;
	if (new_ifindex &&
	    nla_put_s32(skb, IFLA_NEW_IFINDEX, new_ifindex) < 0)
		goto nla_put_failure;

	if (memchr_inv(dev->perm_addr, '\0', dev->addr_len) &&
	    nla_put(skb, IFLA_PERM_ADDRESS, dev->addr_len, dev->perm_addr))
		goto nla_put_failure;

	rcu_read_lock();
	if (rtnl_fill_link_af(skb, dev, ext_filter_mask))
		goto nla_put_failure_rcu;
	if (rtnl_fill_link_ifmap(skb, dev))
		goto nla_put_failure_rcu;
	if (rtnl_fill_prop_list(skb, dev))
		goto nla_put_failure_rcu;
	rcu_read_unlock();

	if (dev->dev.parent &&
	    nla_put_string(skb, IFLA_PARENT_DEV_NAME,
			   dev_name(dev->dev.parent)))
		goto nla_put_failure;

	if (dev->dev.parent && dev->dev.parent->bus &&
	    nla_put_string(skb, IFLA_PARENT_DEV_BUS_NAME,
			   dev->dev.parent->bus->name))
		goto nla_put_failure;

	if (rtnl_fill_devlink_port(skb, dev))
		goto nla_put_failure;

	if (rtnl_fill_dpll_pin(skb, dev))
		goto nla_put_failure;

	nlmsg_end(skb, nlh);
	return 0;

nla_put_failure_rcu:
	rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
nla_put_failure:
	nlmsg_cancel(skb, nlh);
	return -EMSGSIZE;
}

<<<<<<< HEAD
static int rtnl_dump_ifinfo(struct sk_buff *skb, struct netlink_callback *cb)
{
	struct net *net = sock_net(skb->sk);
	int h, s_h;
	int idx = 0, s_idx;
	struct net_device *dev;
	struct hlist_head *head;
	struct hlist_node *node;
	struct nlattr *tb[IFLA_MAX+1];
	u32 ext_filter_mask = 0;
	int err;

	s_h = cb->args[0];
	s_idx = cb->args[1];

	rcu_read_lock();
	cb->seq = net->dev_base_seq;

	if (nlmsg_parse(cb->nlh, sizeof(struct ifinfomsg), tb, IFLA_MAX,
			ifla_policy) >= 0) {

		if (tb[IFLA_EXT_MASK])
			ext_filter_mask = nla_get_u32(tb[IFLA_EXT_MASK]);
	}

	for (h = s_h; h < NETDEV_HASHENTRIES; h++, s_idx = 0) {
		idx = 0;
		head = &net->dev_index_head[h];
		hlist_for_each_entry_rcu(dev, node, head, index_hlist) {
			if (idx < s_idx)
				goto cont;
			err = rtnl_fill_ifinfo(skb, dev, RTM_NEWLINK,
					       NETLINK_CB(cb->skb).pid,
					       cb->nlh->nlmsg_seq, 0,
					       NLM_F_MULTI,
					       ext_filter_mask);
			/* If we ran out of room on the first message,
			 * we're in trouble
			 */
			WARN_ON((err == -EMSGSIZE) && (skb->len == 0));

			if (err <= 0)
				goto out;

			nl_dump_check_consistent(cb, nlmsg_hdr(skb));
cont:
			idx++;
		}
	}
out:
	rcu_read_unlock();
	cb->args[1] = idx;
	cb->args[0] = h;

	return skb->len;
}

const struct nla_policy ifla_policy[IFLA_MAX+1] = {
=======
static const struct nla_policy ifla_policy[IFLA_MAX+1] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	[IFLA_IFNAME]		= { .type = NLA_STRING, .len = IFNAMSIZ-1 },
	[IFLA_ADDRESS]		= { .type = NLA_BINARY, .len = MAX_ADDR_LEN },
	[IFLA_BROADCAST]	= { .type = NLA_BINARY, .len = MAX_ADDR_LEN },
	[IFLA_MAP]		= { .len = sizeof(struct rtnl_link_ifmap) },
	[IFLA_MTU]		= { .type = NLA_U32 },
	[IFLA_LINK]		= { .type = NLA_U32 },
	[IFLA_MASTER]		= { .type = NLA_U32 },
<<<<<<< HEAD
=======
	[IFLA_CARRIER]		= { .type = NLA_U8 },
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	[IFLA_TXQLEN]		= { .type = NLA_U32 },
	[IFLA_WEIGHT]		= { .type = NLA_U32 },
	[IFLA_OPERSTATE]	= { .type = NLA_U8 },
	[IFLA_LINKMODE]		= { .type = NLA_U8 },
	[IFLA_LINKINFO]		= { .type = NLA_NESTED },
	[IFLA_NET_NS_PID]	= { .type = NLA_U32 },
	[IFLA_NET_NS_FD]	= { .type = NLA_U32 },
<<<<<<< HEAD
	[IFLA_IFALIAS]	        = { .type = NLA_STRING, .len = IFALIASZ-1 },
=======
	/* IFLA_IFALIAS is a string, but policy is set to NLA_BINARY to
	 * allow 0-length string (needed to remove an alias).
	 */
	[IFLA_IFALIAS]	        = { .type = NLA_BINARY, .len = IFALIASZ - 1 },
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	[IFLA_VFINFO_LIST]	= {. type = NLA_NESTED },
	[IFLA_VF_PORTS]		= { .type = NLA_NESTED },
	[IFLA_PORT_SELF]	= { .type = NLA_NESTED },
	[IFLA_AF_SPEC]		= { .type = NLA_NESTED },
	[IFLA_EXT_MASK]		= { .type = NLA_U32 },
<<<<<<< HEAD
};
EXPORT_SYMBOL(ifla_policy);
=======
	[IFLA_PROMISCUITY]	= { .type = NLA_U32 },
	[IFLA_NUM_TX_QUEUES]	= { .type = NLA_U32 },
	[IFLA_NUM_RX_QUEUES]	= { .type = NLA_U32 },
	[IFLA_GSO_MAX_SEGS]	= { .type = NLA_U32 },
	[IFLA_GSO_MAX_SIZE]	= { .type = NLA_U32 },
	[IFLA_PHYS_PORT_ID]	= { .type = NLA_BINARY, .len = MAX_PHYS_ITEM_ID_LEN },
	[IFLA_CARRIER_CHANGES]	= { .type = NLA_U32 },  /* ignored */
	[IFLA_PHYS_SWITCH_ID]	= { .type = NLA_BINARY, .len = MAX_PHYS_ITEM_ID_LEN },
	[IFLA_LINK_NETNSID]	= { .type = NLA_S32 },
	[IFLA_PROTO_DOWN]	= { .type = NLA_U8 },
	[IFLA_XDP]		= { .type = NLA_NESTED },
	[IFLA_EVENT]		= { .type = NLA_U32 },
	[IFLA_GROUP]		= { .type = NLA_U32 },
	[IFLA_TARGET_NETNSID]	= { .type = NLA_S32 },
	[IFLA_CARRIER_UP_COUNT]	= { .type = NLA_U32 },
	[IFLA_CARRIER_DOWN_COUNT] = { .type = NLA_U32 },
	[IFLA_MIN_MTU]		= { .type = NLA_U32 },
	[IFLA_MAX_MTU]		= { .type = NLA_U32 },
	[IFLA_PROP_LIST]	= { .type = NLA_NESTED },
	[IFLA_ALT_IFNAME]	= { .type = NLA_STRING,
				    .len = ALTIFNAMSIZ - 1 },
	[IFLA_PERM_ADDRESS]	= { .type = NLA_REJECT },
	[IFLA_PROTO_DOWN_REASON] = { .type = NLA_NESTED },
	[IFLA_NEW_IFINDEX]	= NLA_POLICY_MIN(NLA_S32, 1),
	[IFLA_PARENT_DEV_NAME]	= { .type = NLA_NUL_STRING },
	[IFLA_GRO_MAX_SIZE]	= { .type = NLA_U32 },
	[IFLA_TSO_MAX_SIZE]	= { .type = NLA_REJECT },
	[IFLA_TSO_MAX_SEGS]	= { .type = NLA_REJECT },
	[IFLA_ALLMULTI]		= { .type = NLA_REJECT },
	[IFLA_GSO_IPV4_MAX_SIZE]	= { .type = NLA_U32 },
	[IFLA_GRO_IPV4_MAX_SIZE]	= { .type = NLA_U32 },
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static const struct nla_policy ifla_info_policy[IFLA_INFO_MAX+1] = {
	[IFLA_INFO_KIND]	= { .type = NLA_STRING },
	[IFLA_INFO_DATA]	= { .type = NLA_NESTED },
<<<<<<< HEAD
=======
	[IFLA_INFO_SLAVE_KIND]	= { .type = NLA_STRING },
	[IFLA_INFO_SLAVE_DATA]	= { .type = NLA_NESTED },
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static const struct nla_policy ifla_vf_policy[IFLA_VF_MAX+1] = {
	[IFLA_VF_MAC]		= { .len = sizeof(struct ifla_vf_mac) },
<<<<<<< HEAD
	[IFLA_VF_VLAN]		= { .len = sizeof(struct ifla_vf_vlan) },
	[IFLA_VF_TX_RATE]	= { .len = sizeof(struct ifla_vf_tx_rate) },
	[IFLA_VF_SPOOFCHK]	= { .len = sizeof(struct ifla_vf_spoofchk) },
=======
	[IFLA_VF_BROADCAST]	= { .type = NLA_REJECT },
	[IFLA_VF_VLAN]		= { .len = sizeof(struct ifla_vf_vlan) },
	[IFLA_VF_VLAN_LIST]     = { .type = NLA_NESTED },
	[IFLA_VF_TX_RATE]	= { .len = sizeof(struct ifla_vf_tx_rate) },
	[IFLA_VF_SPOOFCHK]	= { .len = sizeof(struct ifla_vf_spoofchk) },
	[IFLA_VF_RATE]		= { .len = sizeof(struct ifla_vf_rate) },
	[IFLA_VF_LINK_STATE]	= { .len = sizeof(struct ifla_vf_link_state) },
	[IFLA_VF_RSS_QUERY_EN]	= { .len = sizeof(struct ifla_vf_rss_query_en) },
	[IFLA_VF_STATS]		= { .type = NLA_NESTED },
	[IFLA_VF_TRUST]		= { .len = sizeof(struct ifla_vf_trust) },
	[IFLA_VF_IB_NODE_GUID]	= { .len = sizeof(struct ifla_vf_guid) },
	[IFLA_VF_IB_PORT_GUID]	= { .len = sizeof(struct ifla_vf_guid) },
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static const struct nla_policy ifla_port_policy[IFLA_PORT_MAX+1] = {
	[IFLA_PORT_VF]		= { .type = NLA_U32 },
	[IFLA_PORT_PROFILE]	= { .type = NLA_STRING,
				    .len = PORT_PROFILE_MAX },
<<<<<<< HEAD
	[IFLA_PORT_VSI_TYPE]	= { .type = NLA_BINARY,
				    .len = sizeof(struct ifla_port_vsi)},
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	[IFLA_PORT_INSTANCE_UUID] = { .type = NLA_BINARY,
				      .len = PORT_UUID_MAX },
	[IFLA_PORT_HOST_UUID]	= { .type = NLA_STRING,
				    .len = PORT_UUID_MAX },
	[IFLA_PORT_REQUEST]	= { .type = NLA_U8, },
	[IFLA_PORT_RESPONSE]	= { .type = NLA_U16, },
<<<<<<< HEAD
};

=======

	/* Unused, but we need to keep it here since user space could
	 * fill it. It's also broken with regard to NLA_BINARY use in
	 * combination with structs.
	 */
	[IFLA_PORT_VSI_TYPE]	= { .type = NLA_BINARY,
				    .len = sizeof(struct ifla_port_vsi) },
};

static const struct nla_policy ifla_xdp_policy[IFLA_XDP_MAX + 1] = {
	[IFLA_XDP_UNSPEC]	= { .strict_start_type = IFLA_XDP_EXPECTED_FD },
	[IFLA_XDP_FD]		= { .type = NLA_S32 },
	[IFLA_XDP_EXPECTED_FD]	= { .type = NLA_S32 },
	[IFLA_XDP_ATTACHED]	= { .type = NLA_U8 },
	[IFLA_XDP_FLAGS]	= { .type = NLA_U32 },
	[IFLA_XDP_PROG_ID]	= { .type = NLA_U32 },
};

static const struct rtnl_link_ops *linkinfo_to_kind_ops(const struct nlattr *nla)
{
	const struct rtnl_link_ops *ops = NULL;
	struct nlattr *linfo[IFLA_INFO_MAX + 1];

	if (nla_parse_nested_deprecated(linfo, IFLA_INFO_MAX, nla, ifla_info_policy, NULL) < 0)
		return NULL;

	if (linfo[IFLA_INFO_KIND]) {
		char kind[MODULE_NAME_LEN];

		nla_strscpy(kind, linfo[IFLA_INFO_KIND], sizeof(kind));
		ops = rtnl_link_ops_get(kind);
	}

	return ops;
}

static bool link_master_filtered(struct net_device *dev, int master_idx)
{
	struct net_device *master;

	if (!master_idx)
		return false;

	master = netdev_master_upper_dev_get(dev);

	/* 0 is already used to denote IFLA_MASTER wasn't passed, therefore need
	 * another invalid value for ifindex to denote "no master".
	 */
	if (master_idx == -1)
		return !!master;

	if (!master || master->ifindex != master_idx)
		return true;

	return false;
}

static bool link_kind_filtered(const struct net_device *dev,
			       const struct rtnl_link_ops *kind_ops)
{
	if (kind_ops && dev->rtnl_link_ops != kind_ops)
		return true;

	return false;
}

static bool link_dump_filtered(struct net_device *dev,
			       int master_idx,
			       const struct rtnl_link_ops *kind_ops)
{
	if (link_master_filtered(dev, master_idx) ||
	    link_kind_filtered(dev, kind_ops))
		return true;

	return false;
}

/**
 * rtnl_get_net_ns_capable - Get netns if sufficiently privileged.
 * @sk: netlink socket
 * @netnsid: network namespace identifier
 *
 * Returns the network namespace identified by netnsid on success or an error
 * pointer on failure.
 */
struct net *rtnl_get_net_ns_capable(struct sock *sk, int netnsid)
{
	struct net *net;

	net = get_net_ns_by_id(sock_net(sk), netnsid);
	if (!net)
		return ERR_PTR(-EINVAL);

	/* For now, the caller is required to have CAP_NET_ADMIN in
	 * the user namespace owning the target net ns.
	 */
	if (!sk_ns_capable(sk, net->user_ns, CAP_NET_ADMIN)) {
		put_net(net);
		return ERR_PTR(-EACCES);
	}
	return net;
}
EXPORT_SYMBOL_GPL(rtnl_get_net_ns_capable);

static int rtnl_valid_dump_ifinfo_req(const struct nlmsghdr *nlh,
				      bool strict_check, struct nlattr **tb,
				      struct netlink_ext_ack *extack)
{
	int hdrlen;

	if (strict_check) {
		struct ifinfomsg *ifm;

		if (nlh->nlmsg_len < nlmsg_msg_size(sizeof(*ifm))) {
			NL_SET_ERR_MSG(extack, "Invalid header for link dump");
			return -EINVAL;
		}

		ifm = nlmsg_data(nlh);
		if (ifm->__ifi_pad || ifm->ifi_type || ifm->ifi_flags ||
		    ifm->ifi_change) {
			NL_SET_ERR_MSG(extack, "Invalid values in header for link dump request");
			return -EINVAL;
		}
		if (ifm->ifi_index) {
			NL_SET_ERR_MSG(extack, "Filter by device index not supported for link dumps");
			return -EINVAL;
		}

		return nlmsg_parse_deprecated_strict(nlh, sizeof(*ifm), tb,
						     IFLA_MAX, ifla_policy,
						     extack);
	}

	/* A hack to preserve kernel<->userspace interface.
	 * The correct header is ifinfomsg. It is consistent with rtnl_getlink.
	 * However, before Linux v3.9 the code here assumed rtgenmsg and that's
	 * what iproute2 < v3.9.0 used.
	 * We can detect the old iproute2. Even including the IFLA_EXT_MASK
	 * attribute, its netlink message is shorter than struct ifinfomsg.
	 */
	hdrlen = nlmsg_len(nlh) < sizeof(struct ifinfomsg) ?
		 sizeof(struct rtgenmsg) : sizeof(struct ifinfomsg);

	return nlmsg_parse_deprecated(nlh, hdrlen, tb, IFLA_MAX, ifla_policy,
				      extack);
}

static int rtnl_dump_ifinfo(struct sk_buff *skb, struct netlink_callback *cb)
{
	const struct rtnl_link_ops *kind_ops = NULL;
	struct netlink_ext_ack *extack = cb->extack;
	const struct nlmsghdr *nlh = cb->nlh;
	struct net *net = sock_net(skb->sk);
	unsigned int flags = NLM_F_MULTI;
	struct nlattr *tb[IFLA_MAX+1];
	struct {
		unsigned long ifindex;
	} *ctx = (void *)cb->ctx;
	struct net *tgt_net = net;
	u32 ext_filter_mask = 0;
	struct net_device *dev;
	int master_idx = 0;
	int netnsid = -1;
	int err, i;

	err = rtnl_valid_dump_ifinfo_req(nlh, cb->strict_check, tb, extack);
	if (err < 0) {
		if (cb->strict_check)
			return err;

		goto walk_entries;
	}

	for (i = 0; i <= IFLA_MAX; ++i) {
		if (!tb[i])
			continue;

		/* new attributes should only be added with strict checking */
		switch (i) {
		case IFLA_TARGET_NETNSID:
			netnsid = nla_get_s32(tb[i]);
			tgt_net = rtnl_get_net_ns_capable(skb->sk, netnsid);
			if (IS_ERR(tgt_net)) {
				NL_SET_ERR_MSG(extack, "Invalid target network namespace id");
				return PTR_ERR(tgt_net);
			}
			break;
		case IFLA_EXT_MASK:
			ext_filter_mask = nla_get_u32(tb[i]);
			break;
		case IFLA_MASTER:
			master_idx = nla_get_u32(tb[i]);
			break;
		case IFLA_LINKINFO:
			kind_ops = linkinfo_to_kind_ops(tb[i]);
			break;
		default:
			if (cb->strict_check) {
				NL_SET_ERR_MSG(extack, "Unsupported attribute in link dump request");
				return -EINVAL;
			}
		}
	}

	if (master_idx || kind_ops)
		flags |= NLM_F_DUMP_FILTERED;

walk_entries:
	err = 0;
	for_each_netdev_dump(tgt_net, dev, ctx->ifindex) {
		if (link_dump_filtered(dev, master_idx, kind_ops))
			continue;
		err = rtnl_fill_ifinfo(skb, dev, net, RTM_NEWLINK,
				       NETLINK_CB(cb->skb).portid,
				       nlh->nlmsg_seq, 0, flags,
				       ext_filter_mask, 0, NULL, 0,
				       netnsid, GFP_KERNEL);
		if (err < 0)
			break;
	}
	cb->seq = tgt_net->dev_base_seq;
	nl_dump_check_consistent(cb, nlmsg_hdr(skb));
	if (netnsid >= 0)
		put_net(tgt_net);

	return err;
}

int rtnl_nla_parse_ifinfomsg(struct nlattr **tb, const struct nlattr *nla_peer,
			     struct netlink_ext_ack *exterr)
{
	const struct ifinfomsg *ifmp;
	const struct nlattr *attrs;
	size_t len;

	ifmp = nla_data(nla_peer);
	attrs = nla_data(nla_peer) + sizeof(struct ifinfomsg);
	len = nla_len(nla_peer) - sizeof(struct ifinfomsg);

	if (ifmp->ifi_index < 0) {
		NL_SET_ERR_MSG_ATTR(exterr, nla_peer,
				    "ifindex can't be negative");
		return -EINVAL;
	}

	return nla_parse_deprecated(tb, IFLA_MAX, attrs, len, ifla_policy,
				    exterr);
}
EXPORT_SYMBOL(rtnl_nla_parse_ifinfomsg);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct net *rtnl_link_get_net(struct net *src_net, struct nlattr *tb[])
{
	struct net *net;
	/* Examine the link attributes and figure out which
	 * network namespace we are talking about.
	 */
	if (tb[IFLA_NET_NS_PID])
		net = get_net_ns_by_pid(nla_get_u32(tb[IFLA_NET_NS_PID]));
	else if (tb[IFLA_NET_NS_FD])
		net = get_net_ns_by_fd(nla_get_u32(tb[IFLA_NET_NS_FD]));
	else
		net = get_net(src_net);
	return net;
}
EXPORT_SYMBOL(rtnl_link_get_net);

<<<<<<< HEAD
static int validate_linkmsg(struct net_device *dev, struct nlattr *tb[])
{
	if (dev) {
		if (tb[IFLA_ADDRESS] &&
		    nla_len(tb[IFLA_ADDRESS]) < dev->addr_len)
			return -EINVAL;

		if (tb[IFLA_BROADCAST] &&
		    nla_len(tb[IFLA_BROADCAST]) < dev->addr_len)
			return -EINVAL;
=======
/* Figure out which network namespace we are talking about by
 * examining the link attributes in the following order:
 *
 * 1. IFLA_NET_NS_PID
 * 2. IFLA_NET_NS_FD
 * 3. IFLA_TARGET_NETNSID
 */
static struct net *rtnl_link_get_net_by_nlattr(struct net *src_net,
					       struct nlattr *tb[])
{
	struct net *net;

	if (tb[IFLA_NET_NS_PID] || tb[IFLA_NET_NS_FD])
		return rtnl_link_get_net(src_net, tb);

	if (!tb[IFLA_TARGET_NETNSID])
		return get_net(src_net);

	net = get_net_ns_by_id(src_net, nla_get_u32(tb[IFLA_TARGET_NETNSID]));
	if (!net)
		return ERR_PTR(-EINVAL);

	return net;
}

static struct net *rtnl_link_get_net_capable(const struct sk_buff *skb,
					     struct net *src_net,
					     struct nlattr *tb[], int cap)
{
	struct net *net;

	net = rtnl_link_get_net_by_nlattr(src_net, tb);
	if (IS_ERR(net))
		return net;

	if (!netlink_ns_capable(skb, net->user_ns, cap)) {
		put_net(net);
		return ERR_PTR(-EPERM);
	}

	return net;
}

/* Verify that rtnetlink requests do not pass additional properties
 * potentially referring to different network namespaces.
 */
static int rtnl_ensure_unique_netns(struct nlattr *tb[],
				    struct netlink_ext_ack *extack,
				    bool netns_id_only)
{

	if (netns_id_only) {
		if (!tb[IFLA_NET_NS_PID] && !tb[IFLA_NET_NS_FD])
			return 0;

		NL_SET_ERR_MSG(extack, "specified netns attribute not supported");
		return -EOPNOTSUPP;
	}

	if (tb[IFLA_TARGET_NETNSID] && (tb[IFLA_NET_NS_PID] || tb[IFLA_NET_NS_FD]))
		goto invalid_attr;

	if (tb[IFLA_NET_NS_PID] && (tb[IFLA_TARGET_NETNSID] || tb[IFLA_NET_NS_FD]))
		goto invalid_attr;

	if (tb[IFLA_NET_NS_FD] && (tb[IFLA_TARGET_NETNSID] || tb[IFLA_NET_NS_PID]))
		goto invalid_attr;

	return 0;

invalid_attr:
	NL_SET_ERR_MSG(extack, "multiple netns identifying attributes specified");
	return -EINVAL;
}

static	int rtnl_set_vf_rate(struct net_device *dev, int vf, int min_tx_rate,
			     int max_tx_rate)
{
	const struct net_device_ops *ops = dev->netdev_ops;

	if (!ops->ndo_set_vf_rate)
		return -EOPNOTSUPP;
	if (max_tx_rate && max_tx_rate < min_tx_rate)
		return -EINVAL;

	return ops->ndo_set_vf_rate(dev, vf, min_tx_rate, max_tx_rate);
}

static int validate_linkmsg(struct net_device *dev, struct nlattr *tb[],
			    struct netlink_ext_ack *extack)
{
	if (tb[IFLA_ADDRESS] &&
	    nla_len(tb[IFLA_ADDRESS]) < dev->addr_len)
		return -EINVAL;

	if (tb[IFLA_BROADCAST] &&
	    nla_len(tb[IFLA_BROADCAST]) < dev->addr_len)
		return -EINVAL;

	if (tb[IFLA_GSO_MAX_SIZE] &&
	    nla_get_u32(tb[IFLA_GSO_MAX_SIZE]) > dev->tso_max_size) {
		NL_SET_ERR_MSG(extack, "too big gso_max_size");
		return -EINVAL;
	}

	if (tb[IFLA_GSO_MAX_SEGS] &&
	    (nla_get_u32(tb[IFLA_GSO_MAX_SEGS]) > GSO_MAX_SEGS ||
	     nla_get_u32(tb[IFLA_GSO_MAX_SEGS]) > dev->tso_max_segs)) {
		NL_SET_ERR_MSG(extack, "too big gso_max_segs");
		return -EINVAL;
	}

	if (tb[IFLA_GRO_MAX_SIZE] &&
	    nla_get_u32(tb[IFLA_GRO_MAX_SIZE]) > GRO_MAX_SIZE) {
		NL_SET_ERR_MSG(extack, "too big gro_max_size");
		return -EINVAL;
	}

	if (tb[IFLA_GSO_IPV4_MAX_SIZE] &&
	    nla_get_u32(tb[IFLA_GSO_IPV4_MAX_SIZE]) > dev->tso_max_size) {
		NL_SET_ERR_MSG(extack, "too big gso_ipv4_max_size");
		return -EINVAL;
	}

	if (tb[IFLA_GRO_IPV4_MAX_SIZE] &&
	    nla_get_u32(tb[IFLA_GRO_IPV4_MAX_SIZE]) > GRO_MAX_SIZE) {
		NL_SET_ERR_MSG(extack, "too big gro_ipv4_max_size");
		return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (tb[IFLA_AF_SPEC]) {
		struct nlattr *af;
		int rem, err;

		nla_for_each_nested(af, tb[IFLA_AF_SPEC], rem) {
			const struct rtnl_af_ops *af_ops;

<<<<<<< HEAD
			if (!(af_ops = rtnl_af_lookup(nla_type(af))))
=======
			af_ops = rtnl_af_lookup(nla_type(af));
			if (!af_ops)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				return -EAFNOSUPPORT;

			if (!af_ops->set_link_af)
				return -EOPNOTSUPP;

			if (af_ops->validate_link_af) {
<<<<<<< HEAD
				err = af_ops->validate_link_af(dev, af);
=======
				err = af_ops->validate_link_af(dev, af, extack);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				if (err < 0)
					return err;
			}
		}
	}

	return 0;
}

<<<<<<< HEAD
=======
static int handle_infiniband_guid(struct net_device *dev, struct ifla_vf_guid *ivt,
				  int guid_type)
{
	const struct net_device_ops *ops = dev->netdev_ops;

	return ops->ndo_set_vf_guid(dev, ivt->vf, ivt->guid, guid_type);
}

static int handle_vf_guid(struct net_device *dev, struct ifla_vf_guid *ivt, int guid_type)
{
	if (dev->type != ARPHRD_INFINIBAND)
		return -EOPNOTSUPP;

	return handle_infiniband_guid(dev, ivt, guid_type);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int do_setvfinfo(struct net_device *dev, struct nlattr **tb)
{
	const struct net_device_ops *ops = dev->netdev_ops;
	int err = -EINVAL;

	if (tb[IFLA_VF_MAC]) {
		struct ifla_vf_mac *ivm = nla_data(tb[IFLA_VF_MAC]);
<<<<<<< HEAD
=======

		if (ivm->vf >= INT_MAX)
			return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = -EOPNOTSUPP;
		if (ops->ndo_set_vf_mac)
			err = ops->ndo_set_vf_mac(dev, ivm->vf,
						  ivm->mac);
		if (err < 0)
			return err;
	}

	if (tb[IFLA_VF_VLAN]) {
		struct ifla_vf_vlan *ivv = nla_data(tb[IFLA_VF_VLAN]);

<<<<<<< HEAD
		err = -EOPNOTSUPP;
		if (ops->ndo_set_vf_vlan)
			err = ops->ndo_set_vf_vlan(dev, ivv->vf, ivv->vlan,
						   ivv->qos);
=======
		if (ivv->vf >= INT_MAX)
			return -EINVAL;
		err = -EOPNOTSUPP;
		if (ops->ndo_set_vf_vlan)
			err = ops->ndo_set_vf_vlan(dev, ivv->vf, ivv->vlan,
						   ivv->qos,
						   htons(ETH_P_8021Q));
		if (err < 0)
			return err;
	}

	if (tb[IFLA_VF_VLAN_LIST]) {
		struct ifla_vf_vlan_info *ivvl[MAX_VLAN_LIST_LEN];
		struct nlattr *attr;
		int rem, len = 0;

		err = -EOPNOTSUPP;
		if (!ops->ndo_set_vf_vlan)
			return err;

		nla_for_each_nested(attr, tb[IFLA_VF_VLAN_LIST], rem) {
			if (nla_type(attr) != IFLA_VF_VLAN_INFO ||
			    nla_len(attr) < sizeof(struct ifla_vf_vlan_info)) {
				return -EINVAL;
			}
			if (len >= MAX_VLAN_LIST_LEN)
				return -EOPNOTSUPP;
			ivvl[len] = nla_data(attr);

			len++;
		}
		if (len == 0)
			return -EINVAL;

		if (ivvl[0]->vf >= INT_MAX)
			return -EINVAL;
		err = ops->ndo_set_vf_vlan(dev, ivvl[0]->vf, ivvl[0]->vlan,
					   ivvl[0]->qos, ivvl[0]->vlan_proto);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (err < 0)
			return err;
	}

	if (tb[IFLA_VF_TX_RATE]) {
		struct ifla_vf_tx_rate *ivt = nla_data(tb[IFLA_VF_TX_RATE]);
<<<<<<< HEAD

		if (ops->ndo_set_vf_tx_rate)
			err = ops->ndo_set_vf_tx_rate(dev, ivt->vf,
						      ivt->rate);
=======
		struct ifla_vf_info ivf;

		if (ivt->vf >= INT_MAX)
			return -EINVAL;
		err = -EOPNOTSUPP;
		if (ops->ndo_get_vf_config)
			err = ops->ndo_get_vf_config(dev, ivt->vf, &ivf);
		if (err < 0)
			return err;

		err = rtnl_set_vf_rate(dev, ivt->vf,
				       ivf.min_tx_rate, ivt->rate);
		if (err < 0)
			return err;
	}

	if (tb[IFLA_VF_RATE]) {
		struct ifla_vf_rate *ivt = nla_data(tb[IFLA_VF_RATE]);

		if (ivt->vf >= INT_MAX)
			return -EINVAL;

		err = rtnl_set_vf_rate(dev, ivt->vf,
				       ivt->min_tx_rate, ivt->max_tx_rate);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (err < 0)
			return err;
	}

	if (tb[IFLA_VF_SPOOFCHK]) {
		struct ifla_vf_spoofchk *ivs = nla_data(tb[IFLA_VF_SPOOFCHK]);

<<<<<<< HEAD
=======
		if (ivs->vf >= INT_MAX)
			return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = -EOPNOTSUPP;
		if (ops->ndo_set_vf_spoofchk)
			err = ops->ndo_set_vf_spoofchk(dev, ivs->vf,
						       ivs->setting);
		if (err < 0)
			return err;
	}

<<<<<<< HEAD
	return err;
}

static int do_set_master(struct net_device *dev, int ifindex)
{
	struct net_device *master_dev;
	const struct net_device_ops *ops;
	int err;

	if (dev->master) {
		if (dev->master->ifindex == ifindex)
			return 0;
		ops = dev->master->netdev_ops;
		if (ops->ndo_del_slave) {
			err = ops->ndo_del_slave(dev->master, dev);
=======
	if (tb[IFLA_VF_LINK_STATE]) {
		struct ifla_vf_link_state *ivl = nla_data(tb[IFLA_VF_LINK_STATE]);

		if (ivl->vf >= INT_MAX)
			return -EINVAL;
		err = -EOPNOTSUPP;
		if (ops->ndo_set_vf_link_state)
			err = ops->ndo_set_vf_link_state(dev, ivl->vf,
							 ivl->link_state);
		if (err < 0)
			return err;
	}

	if (tb[IFLA_VF_RSS_QUERY_EN]) {
		struct ifla_vf_rss_query_en *ivrssq_en;

		err = -EOPNOTSUPP;
		ivrssq_en = nla_data(tb[IFLA_VF_RSS_QUERY_EN]);
		if (ivrssq_en->vf >= INT_MAX)
			return -EINVAL;
		if (ops->ndo_set_vf_rss_query_en)
			err = ops->ndo_set_vf_rss_query_en(dev, ivrssq_en->vf,
							   ivrssq_en->setting);
		if (err < 0)
			return err;
	}

	if (tb[IFLA_VF_TRUST]) {
		struct ifla_vf_trust *ivt = nla_data(tb[IFLA_VF_TRUST]);

		if (ivt->vf >= INT_MAX)
			return -EINVAL;
		err = -EOPNOTSUPP;
		if (ops->ndo_set_vf_trust)
			err = ops->ndo_set_vf_trust(dev, ivt->vf, ivt->setting);
		if (err < 0)
			return err;
	}

	if (tb[IFLA_VF_IB_NODE_GUID]) {
		struct ifla_vf_guid *ivt = nla_data(tb[IFLA_VF_IB_NODE_GUID]);

		if (ivt->vf >= INT_MAX)
			return -EINVAL;
		if (!ops->ndo_set_vf_guid)
			return -EOPNOTSUPP;
		return handle_vf_guid(dev, ivt, IFLA_VF_IB_NODE_GUID);
	}

	if (tb[IFLA_VF_IB_PORT_GUID]) {
		struct ifla_vf_guid *ivt = nla_data(tb[IFLA_VF_IB_PORT_GUID]);

		if (ivt->vf >= INT_MAX)
			return -EINVAL;
		if (!ops->ndo_set_vf_guid)
			return -EOPNOTSUPP;

		return handle_vf_guid(dev, ivt, IFLA_VF_IB_PORT_GUID);
	}

	return err;
}

static int do_set_master(struct net_device *dev, int ifindex,
			 struct netlink_ext_ack *extack)
{
	struct net_device *upper_dev = netdev_master_upper_dev_get(dev);
	const struct net_device_ops *ops;
	int err;

	if (upper_dev) {
		if (upper_dev->ifindex == ifindex)
			return 0;
		ops = upper_dev->netdev_ops;
		if (ops->ndo_del_slave) {
			err = ops->ndo_del_slave(upper_dev, dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (err)
				return err;
		} else {
			return -EOPNOTSUPP;
		}
	}

	if (ifindex) {
<<<<<<< HEAD
		master_dev = __dev_get_by_index(dev_net(dev), ifindex);
		if (!master_dev)
			return -EINVAL;
		ops = master_dev->netdev_ops;
		if (ops->ndo_add_slave) {
			err = ops->ndo_add_slave(master_dev, dev);
=======
		upper_dev = __dev_get_by_index(dev_net(dev), ifindex);
		if (!upper_dev)
			return -EINVAL;
		ops = upper_dev->netdev_ops;
		if (ops->ndo_add_slave) {
			err = ops->ndo_add_slave(upper_dev, dev, extack);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (err)
				return err;
		} else {
			return -EOPNOTSUPP;
		}
	}
	return 0;
}

<<<<<<< HEAD
static int do_setlink(struct net_device *dev, struct ifinfomsg *ifm,
		      struct nlattr **tb, char *ifname, int modified)
{
	const struct net_device_ops *ops = dev->netdev_ops;
	int send_addr_notify = 0;
	int err;

	if (tb[IFLA_NET_NS_PID] || tb[IFLA_NET_NS_FD]) {
		struct net *net = rtnl_link_get_net(dev_net(dev), tb);
=======
static const struct nla_policy ifla_proto_down_reason_policy[IFLA_PROTO_DOWN_REASON_VALUE + 1] = {
	[IFLA_PROTO_DOWN_REASON_MASK]	= { .type = NLA_U32 },
	[IFLA_PROTO_DOWN_REASON_VALUE]	= { .type = NLA_U32 },
};

static int do_set_proto_down(struct net_device *dev,
			     struct nlattr *nl_proto_down,
			     struct nlattr *nl_proto_down_reason,
			     struct netlink_ext_ack *extack)
{
	struct nlattr *pdreason[IFLA_PROTO_DOWN_REASON_MAX + 1];
	unsigned long mask = 0;
	u32 value;
	bool proto_down;
	int err;

	if (!(dev->priv_flags & IFF_CHANGE_PROTO_DOWN)) {
		NL_SET_ERR_MSG(extack,  "Protodown not supported by device");
		return -EOPNOTSUPP;
	}

	if (nl_proto_down_reason) {
		err = nla_parse_nested_deprecated(pdreason,
						  IFLA_PROTO_DOWN_REASON_MAX,
						  nl_proto_down_reason,
						  ifla_proto_down_reason_policy,
						  NULL);
		if (err < 0)
			return err;

		if (!pdreason[IFLA_PROTO_DOWN_REASON_VALUE]) {
			NL_SET_ERR_MSG(extack, "Invalid protodown reason value");
			return -EINVAL;
		}

		value = nla_get_u32(pdreason[IFLA_PROTO_DOWN_REASON_VALUE]);

		if (pdreason[IFLA_PROTO_DOWN_REASON_MASK])
			mask = nla_get_u32(pdreason[IFLA_PROTO_DOWN_REASON_MASK]);

		dev_change_proto_down_reason(dev, mask, value);
	}

	if (nl_proto_down) {
		proto_down = nla_get_u8(nl_proto_down);

		/* Don't turn off protodown if there are active reasons */
		if (!proto_down && dev->proto_down_reason) {
			NL_SET_ERR_MSG(extack, "Cannot clear protodown, active reasons");
			return -EBUSY;
		}
		err = dev_change_proto_down(dev,
					    proto_down);
		if (err)
			return err;
	}

	return 0;
}

#define DO_SETLINK_MODIFIED	0x01
/* notify flag means notify + modified. */
#define DO_SETLINK_NOTIFY	0x03
static int do_setlink(const struct sk_buff *skb,
		      struct net_device *dev, struct ifinfomsg *ifm,
		      struct netlink_ext_ack *extack,
		      struct nlattr **tb, int status)
{
	const struct net_device_ops *ops = dev->netdev_ops;
	char ifname[IFNAMSIZ];
	int err;

	if (tb[IFLA_IFNAME])
		nla_strscpy(ifname, tb[IFLA_IFNAME], IFNAMSIZ);
	else
		ifname[0] = '\0';

	if (tb[IFLA_NET_NS_PID] || tb[IFLA_NET_NS_FD] || tb[IFLA_TARGET_NETNSID]) {
		const char *pat = ifname[0] ? ifname : NULL;
		struct net *net;
		int new_ifindex;

		net = rtnl_link_get_net_capable(skb, dev_net(dev),
						tb, CAP_NET_ADMIN);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (IS_ERR(net)) {
			err = PTR_ERR(net);
			goto errout;
		}
<<<<<<< HEAD
		err = dev_change_net_namespace(dev, net, ifname);
		put_net(net);
		if (err)
			goto errout;
		modified = 1;
=======

		if (tb[IFLA_NEW_IFINDEX])
			new_ifindex = nla_get_s32(tb[IFLA_NEW_IFINDEX]);
		else
			new_ifindex = 0;

		err = __dev_change_net_namespace(dev, net, pat, new_ifindex);
		put_net(net);
		if (err)
			goto errout;
		status |= DO_SETLINK_MODIFIED;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (tb[IFLA_MAP]) {
		struct rtnl_link_ifmap *u_map;
		struct ifmap k_map;

		if (!ops->ndo_set_config) {
			err = -EOPNOTSUPP;
			goto errout;
		}

		if (!netif_device_present(dev)) {
			err = -ENODEV;
			goto errout;
		}

		u_map = nla_data(tb[IFLA_MAP]);
		k_map.mem_start = (unsigned long) u_map->mem_start;
		k_map.mem_end = (unsigned long) u_map->mem_end;
		k_map.base_addr = (unsigned short) u_map->base_addr;
		k_map.irq = (unsigned char) u_map->irq;
		k_map.dma = (unsigned char) u_map->dma;
		k_map.port = (unsigned char) u_map->port;

		err = ops->ndo_set_config(dev, &k_map);
		if (err < 0)
			goto errout;

<<<<<<< HEAD
		modified = 1;
=======
		status |= DO_SETLINK_NOTIFY;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (tb[IFLA_ADDRESS]) {
		struct sockaddr *sa;
		int len;

<<<<<<< HEAD
		if (!ops->ndo_set_mac_address) {
			err = -EOPNOTSUPP;
			goto errout;
		}

		if (!netif_device_present(dev)) {
			err = -ENODEV;
			goto errout;
		}

		len = sizeof(sa_family_t) + dev->addr_len;
=======
		len = sizeof(sa_family_t) + max_t(size_t, dev->addr_len,
						  sizeof(*sa));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		sa = kmalloc(len, GFP_KERNEL);
		if (!sa) {
			err = -ENOMEM;
			goto errout;
		}
		sa->sa_family = dev->type;
		memcpy(sa->sa_data, nla_data(tb[IFLA_ADDRESS]),
		       dev->addr_len);
<<<<<<< HEAD
		err = ops->ndo_set_mac_address(dev, sa);
		kfree(sa);
		if (err)
			goto errout;
		send_addr_notify = 1;
		modified = 1;
		add_device_randomness(dev->dev_addr, dev->addr_len);
	}

	if (tb[IFLA_MTU]) {
		err = dev_set_mtu(dev, nla_get_u32(tb[IFLA_MTU]));
		if (err < 0)
			goto errout;
		modified = 1;
=======
		err = dev_set_mac_address_user(dev, sa, extack);
		kfree(sa);
		if (err)
			goto errout;
		status |= DO_SETLINK_MODIFIED;
	}

	if (tb[IFLA_MTU]) {
		err = dev_set_mtu_ext(dev, nla_get_u32(tb[IFLA_MTU]), extack);
		if (err < 0)
			goto errout;
		status |= DO_SETLINK_MODIFIED;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (tb[IFLA_GROUP]) {
		dev_set_group(dev, nla_get_u32(tb[IFLA_GROUP]));
<<<<<<< HEAD
		modified = 1;
=======
		status |= DO_SETLINK_NOTIFY;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/*
	 * Interface selected by interface index but interface
	 * name provided implies that a name change has been
	 * requested.
	 */
	if (ifm->ifi_index > 0 && ifname[0]) {
		err = dev_change_name(dev, ifname);
		if (err < 0)
			goto errout;
<<<<<<< HEAD
		modified = 1;
=======
		status |= DO_SETLINK_MODIFIED;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (tb[IFLA_IFALIAS]) {
		err = dev_set_alias(dev, nla_data(tb[IFLA_IFALIAS]),
				    nla_len(tb[IFLA_IFALIAS]));
		if (err < 0)
			goto errout;
<<<<<<< HEAD
		modified = 1;
=======
		status |= DO_SETLINK_NOTIFY;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (tb[IFLA_BROADCAST]) {
		nla_memcpy(dev->broadcast, tb[IFLA_BROADCAST], dev->addr_len);
<<<<<<< HEAD
		send_addr_notify = 1;
	}

	if (ifm->ifi_flags || ifm->ifi_change) {
		err = dev_change_flags(dev, rtnl_dev_combine_flags(dev, ifm));
=======
		call_netdevice_notifiers(NETDEV_CHANGEADDR, dev);
	}

	if (ifm->ifi_flags || ifm->ifi_change) {
		err = dev_change_flags(dev, rtnl_dev_combine_flags(dev, ifm),
				       extack);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (err < 0)
			goto errout;
	}

	if (tb[IFLA_MASTER]) {
<<<<<<< HEAD
		err = do_set_master(dev, nla_get_u32(tb[IFLA_MASTER]));
		if (err)
			goto errout;
		modified = 1;
	}

	if (tb[IFLA_TXQLEN])
		dev->tx_queue_len = nla_get_u32(tb[IFLA_TXQLEN]);
=======
		err = do_set_master(dev, nla_get_u32(tb[IFLA_MASTER]), extack);
		if (err)
			goto errout;
		status |= DO_SETLINK_MODIFIED;
	}

	if (tb[IFLA_CARRIER]) {
		err = dev_change_carrier(dev, nla_get_u8(tb[IFLA_CARRIER]));
		if (err)
			goto errout;
		status |= DO_SETLINK_MODIFIED;
	}

	if (tb[IFLA_TXQLEN]) {
		unsigned int value = nla_get_u32(tb[IFLA_TXQLEN]);

		err = dev_change_tx_queue_len(dev, value);
		if (err)
			goto errout;
		status |= DO_SETLINK_MODIFIED;
	}

	if (tb[IFLA_GSO_MAX_SIZE]) {
		u32 max_size = nla_get_u32(tb[IFLA_GSO_MAX_SIZE]);

		if (dev->gso_max_size ^ max_size) {
			netif_set_gso_max_size(dev, max_size);
			status |= DO_SETLINK_MODIFIED;
		}
	}

	if (tb[IFLA_GSO_MAX_SEGS]) {
		u32 max_segs = nla_get_u32(tb[IFLA_GSO_MAX_SEGS]);

		if (dev->gso_max_segs ^ max_segs) {
			netif_set_gso_max_segs(dev, max_segs);
			status |= DO_SETLINK_MODIFIED;
		}
	}

	if (tb[IFLA_GRO_MAX_SIZE]) {
		u32 gro_max_size = nla_get_u32(tb[IFLA_GRO_MAX_SIZE]);

		if (dev->gro_max_size ^ gro_max_size) {
			netif_set_gro_max_size(dev, gro_max_size);
			status |= DO_SETLINK_MODIFIED;
		}
	}

	if (tb[IFLA_GSO_IPV4_MAX_SIZE]) {
		u32 max_size = nla_get_u32(tb[IFLA_GSO_IPV4_MAX_SIZE]);

		if (dev->gso_ipv4_max_size ^ max_size) {
			netif_set_gso_ipv4_max_size(dev, max_size);
			status |= DO_SETLINK_MODIFIED;
		}
	}

	if (tb[IFLA_GRO_IPV4_MAX_SIZE]) {
		u32 gro_max_size = nla_get_u32(tb[IFLA_GRO_IPV4_MAX_SIZE]);

		if (dev->gro_ipv4_max_size ^ gro_max_size) {
			netif_set_gro_ipv4_max_size(dev, gro_max_size);
			status |= DO_SETLINK_MODIFIED;
		}
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (tb[IFLA_OPERSTATE])
		set_operstate(dev, nla_get_u8(tb[IFLA_OPERSTATE]));

	if (tb[IFLA_LINKMODE]) {
<<<<<<< HEAD
		write_lock_bh(&dev_base_lock);
		dev->link_mode = nla_get_u8(tb[IFLA_LINKMODE]);
		write_unlock_bh(&dev_base_lock);
=======
		unsigned char value = nla_get_u8(tb[IFLA_LINKMODE]);

		if (dev->link_mode ^ value)
			status |= DO_SETLINK_NOTIFY;
		WRITE_ONCE(dev->link_mode, value);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (tb[IFLA_VFINFO_LIST]) {
		struct nlattr *vfinfo[IFLA_VF_MAX + 1];
		struct nlattr *attr;
		int rem;

		nla_for_each_nested(attr, tb[IFLA_VFINFO_LIST], rem) {
			if (nla_type(attr) != IFLA_VF_INFO ||
			    nla_len(attr) < NLA_HDRLEN) {
				err = -EINVAL;
				goto errout;
			}
<<<<<<< HEAD
			err = nla_parse_nested(vfinfo, IFLA_VF_MAX, attr,
					       ifla_vf_policy);
=======
			err = nla_parse_nested_deprecated(vfinfo, IFLA_VF_MAX,
							  attr,
							  ifla_vf_policy,
							  NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (err < 0)
				goto errout;
			err = do_setvfinfo(dev, vfinfo);
			if (err < 0)
				goto errout;
<<<<<<< HEAD
			modified = 1;
=======
			status |= DO_SETLINK_NOTIFY;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}
	err = 0;

	if (tb[IFLA_VF_PORTS]) {
		struct nlattr *port[IFLA_PORT_MAX+1];
		struct nlattr *attr;
		int vf;
		int rem;

		err = -EOPNOTSUPP;
		if (!ops->ndo_set_vf_port)
			goto errout;

		nla_for_each_nested(attr, tb[IFLA_VF_PORTS], rem) {
<<<<<<< HEAD
			if (nla_type(attr) != IFLA_VF_PORT)
				continue;
			err = nla_parse_nested(port, IFLA_PORT_MAX,
				attr, ifla_port_policy);
=======
			if (nla_type(attr) != IFLA_VF_PORT ||
			    nla_len(attr) < NLA_HDRLEN) {
				err = -EINVAL;
				goto errout;
			}
			err = nla_parse_nested_deprecated(port, IFLA_PORT_MAX,
							  attr,
							  ifla_port_policy,
							  NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (err < 0)
				goto errout;
			if (!port[IFLA_PORT_VF]) {
				err = -EOPNOTSUPP;
				goto errout;
			}
			vf = nla_get_u32(port[IFLA_PORT_VF]);
			err = ops->ndo_set_vf_port(dev, vf, port);
			if (err < 0)
				goto errout;
<<<<<<< HEAD
			modified = 1;
=======
			status |= DO_SETLINK_NOTIFY;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}
	err = 0;

	if (tb[IFLA_PORT_SELF]) {
		struct nlattr *port[IFLA_PORT_MAX+1];

<<<<<<< HEAD
		err = nla_parse_nested(port, IFLA_PORT_MAX,
			tb[IFLA_PORT_SELF], ifla_port_policy);
=======
		err = nla_parse_nested_deprecated(port, IFLA_PORT_MAX,
						  tb[IFLA_PORT_SELF],
						  ifla_port_policy, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (err < 0)
			goto errout;

		err = -EOPNOTSUPP;
		if (ops->ndo_set_vf_port)
			err = ops->ndo_set_vf_port(dev, PORT_SELF_VF, port);
		if (err < 0)
			goto errout;
<<<<<<< HEAD
		modified = 1;
=======
		status |= DO_SETLINK_NOTIFY;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (tb[IFLA_AF_SPEC]) {
		struct nlattr *af;
		int rem;

		nla_for_each_nested(af, tb[IFLA_AF_SPEC], rem) {
			const struct rtnl_af_ops *af_ops;

<<<<<<< HEAD
			if (!(af_ops = rtnl_af_lookup(nla_type(af))))
				BUG();

			err = af_ops->set_link_af(dev, af);
			if (err < 0)
				goto errout;

			modified = 1;
=======
			BUG_ON(!(af_ops = rtnl_af_lookup(nla_type(af))));

			err = af_ops->set_link_af(dev, af, extack);
			if (err < 0)
				goto errout;

			status |= DO_SETLINK_NOTIFY;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}
	err = 0;

<<<<<<< HEAD
errout:
	if (err < 0 && modified && net_ratelimit())
		printk(KERN_WARNING "A link change request failed with "
		       "some changes committed already. Interface %s may "
		       "have been left with an inconsistent configuration, "
		       "please check.\n", dev->name);

	if (send_addr_notify)
		call_netdevice_notifiers(NETDEV_CHANGEADDR, dev);
=======
	if (tb[IFLA_PROTO_DOWN] || tb[IFLA_PROTO_DOWN_REASON]) {
		err = do_set_proto_down(dev, tb[IFLA_PROTO_DOWN],
					tb[IFLA_PROTO_DOWN_REASON], extack);
		if (err)
			goto errout;
		status |= DO_SETLINK_NOTIFY;
	}

	if (tb[IFLA_XDP]) {
		struct nlattr *xdp[IFLA_XDP_MAX + 1];
		u32 xdp_flags = 0;

		err = nla_parse_nested_deprecated(xdp, IFLA_XDP_MAX,
						  tb[IFLA_XDP],
						  ifla_xdp_policy, NULL);
		if (err < 0)
			goto errout;

		if (xdp[IFLA_XDP_ATTACHED] || xdp[IFLA_XDP_PROG_ID]) {
			err = -EINVAL;
			goto errout;
		}

		if (xdp[IFLA_XDP_FLAGS]) {
			xdp_flags = nla_get_u32(xdp[IFLA_XDP_FLAGS]);
			if (xdp_flags & ~XDP_FLAGS_MASK) {
				err = -EINVAL;
				goto errout;
			}
			if (hweight32(xdp_flags & XDP_FLAGS_MODES) > 1) {
				err = -EINVAL;
				goto errout;
			}
		}

		if (xdp[IFLA_XDP_FD]) {
			int expected_fd = -1;

			if (xdp_flags & XDP_FLAGS_REPLACE) {
				if (!xdp[IFLA_XDP_EXPECTED_FD]) {
					err = -EINVAL;
					goto errout;
				}
				expected_fd =
					nla_get_s32(xdp[IFLA_XDP_EXPECTED_FD]);
			}

			err = dev_change_xdp_fd(dev, extack,
						nla_get_s32(xdp[IFLA_XDP_FD]),
						expected_fd,
						xdp_flags);
			if (err)
				goto errout;
			status |= DO_SETLINK_NOTIFY;
		}
	}

errout:
	if (status & DO_SETLINK_MODIFIED) {
		if ((status & DO_SETLINK_NOTIFY) == DO_SETLINK_NOTIFY)
			netdev_state_change(dev);

		if (err < 0)
			net_warn_ratelimited("A link change request failed with some changes committed already. Interface %s may have been left with an inconsistent configuration, please check.\n",
					     dev->name);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return err;
}

<<<<<<< HEAD
static int rtnl_setlink(struct sk_buff *skb, struct nlmsghdr *nlh, void *arg)
=======
static struct net_device *rtnl_dev_get(struct net *net,
				       struct nlattr *tb[])
{
	char ifname[ALTIFNAMSIZ];

	if (tb[IFLA_IFNAME])
		nla_strscpy(ifname, tb[IFLA_IFNAME], IFNAMSIZ);
	else if (tb[IFLA_ALT_IFNAME])
		nla_strscpy(ifname, tb[IFLA_ALT_IFNAME], ALTIFNAMSIZ);
	else
		return NULL;

	return __dev_get_by_name(net, ifname);
}

static int rtnl_setlink(struct sk_buff *skb, struct nlmsghdr *nlh,
			struct netlink_ext_ack *extack)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct net *net = sock_net(skb->sk);
	struct ifinfomsg *ifm;
	struct net_device *dev;
	int err;
	struct nlattr *tb[IFLA_MAX+1];
<<<<<<< HEAD
	char ifname[IFNAMSIZ];

	err = nlmsg_parse(nlh, sizeof(*ifm), tb, IFLA_MAX, ifla_policy);
	if (err < 0)
		goto errout;

	if (tb[IFLA_IFNAME])
		nla_strlcpy(ifname, tb[IFLA_IFNAME], IFNAMSIZ);
	else
		ifname[0] = '\0';
=======

	err = nlmsg_parse_deprecated(nlh, sizeof(*ifm), tb, IFLA_MAX,
				     ifla_policy, extack);
	if (err < 0)
		goto errout;

	err = rtnl_ensure_unique_netns(tb, extack, false);
	if (err < 0)
		goto errout;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	err = -EINVAL;
	ifm = nlmsg_data(nlh);
	if (ifm->ifi_index > 0)
		dev = __dev_get_by_index(net, ifm->ifi_index);
<<<<<<< HEAD
	else if (tb[IFLA_IFNAME])
		dev = __dev_get_by_name(net, ifname);
=======
	else if (tb[IFLA_IFNAME] || tb[IFLA_ALT_IFNAME])
		dev = rtnl_dev_get(net, tb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	else
		goto errout;

	if (dev == NULL) {
		err = -ENODEV;
		goto errout;
	}

<<<<<<< HEAD
	err = validate_linkmsg(dev, tb);
	if (err < 0)
		goto errout;

	err = do_setlink(dev, ifm, tb, ifname, 0);
=======
	err = validate_linkmsg(dev, tb, extack);
	if (err < 0)
		goto errout;

	err = do_setlink(skb, dev, ifm, extack, tb, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
errout:
	return err;
}

<<<<<<< HEAD
static int rtnl_dellink(struct sk_buff *skb, struct nlmsghdr *nlh, void *arg)
{
	struct net *net = sock_net(skb->sk);
	const struct rtnl_link_ops *ops;
	struct net_device *dev;
	struct ifinfomsg *ifm;
	char ifname[IFNAMSIZ];
	struct nlattr *tb[IFLA_MAX+1];
	int err;
	LIST_HEAD(list_kill);

	err = nlmsg_parse(nlh, sizeof(*ifm), tb, IFLA_MAX, ifla_policy);
	if (err < 0)
		return err;

	if (tb[IFLA_IFNAME])
		nla_strlcpy(ifname, tb[IFLA_IFNAME], IFNAMSIZ);

	ifm = nlmsg_data(nlh);
	if (ifm->ifi_index > 0)
		dev = __dev_get_by_index(net, ifm->ifi_index);
	else if (tb[IFLA_IFNAME])
		dev = __dev_get_by_name(net, ifname);
	else
		return -EINVAL;

	if (!dev)
		return -ENODEV;

	ops = dev->rtnl_link_ops;
	if (!ops)
		return -EOPNOTSUPP;

	ops->dellink(dev, &list_kill);
	unregister_netdevice_many(&list_kill);
	list_del(&list_kill);
	return 0;
}

int rtnl_configure_link(struct net_device *dev, const struct ifinfomsg *ifm)
=======
static int rtnl_group_dellink(const struct net *net, int group)
{
	struct net_device *dev, *aux;
	LIST_HEAD(list_kill);
	bool found = false;

	if (!group)
		return -EPERM;

	for_each_netdev(net, dev) {
		if (dev->group == group) {
			const struct rtnl_link_ops *ops;

			found = true;
			ops = dev->rtnl_link_ops;
			if (!ops || !ops->dellink)
				return -EOPNOTSUPP;
		}
	}

	if (!found)
		return -ENODEV;

	for_each_netdev_safe(net, dev, aux) {
		if (dev->group == group) {
			const struct rtnl_link_ops *ops;

			ops = dev->rtnl_link_ops;
			ops->dellink(dev, &list_kill);
		}
	}
	unregister_netdevice_many(&list_kill);

	return 0;
}

int rtnl_delete_link(struct net_device *dev, u32 portid, const struct nlmsghdr *nlh)
{
	const struct rtnl_link_ops *ops;
	LIST_HEAD(list_kill);

	ops = dev->rtnl_link_ops;
	if (!ops || !ops->dellink)
		return -EOPNOTSUPP;

	ops->dellink(dev, &list_kill);
	unregister_netdevice_many_notify(&list_kill, portid, nlh);

	return 0;
}
EXPORT_SYMBOL_GPL(rtnl_delete_link);

static int rtnl_dellink(struct sk_buff *skb, struct nlmsghdr *nlh,
			struct netlink_ext_ack *extack)
{
	struct net *net = sock_net(skb->sk);
	u32 portid = NETLINK_CB(skb).portid;
	struct net *tgt_net = net;
	struct net_device *dev = NULL;
	struct ifinfomsg *ifm;
	struct nlattr *tb[IFLA_MAX+1];
	int err;
	int netnsid = -1;

	err = nlmsg_parse_deprecated(nlh, sizeof(*ifm), tb, IFLA_MAX,
				     ifla_policy, extack);
	if (err < 0)
		return err;

	err = rtnl_ensure_unique_netns(tb, extack, true);
	if (err < 0)
		return err;

	if (tb[IFLA_TARGET_NETNSID]) {
		netnsid = nla_get_s32(tb[IFLA_TARGET_NETNSID]);
		tgt_net = rtnl_get_net_ns_capable(NETLINK_CB(skb).sk, netnsid);
		if (IS_ERR(tgt_net))
			return PTR_ERR(tgt_net);
	}

	err = -EINVAL;
	ifm = nlmsg_data(nlh);
	if (ifm->ifi_index > 0)
		dev = __dev_get_by_index(tgt_net, ifm->ifi_index);
	else if (tb[IFLA_IFNAME] || tb[IFLA_ALT_IFNAME])
		dev = rtnl_dev_get(net, tb);
	else if (tb[IFLA_GROUP])
		err = rtnl_group_dellink(tgt_net, nla_get_u32(tb[IFLA_GROUP]));
	else
		goto out;

	if (!dev) {
		if (tb[IFLA_IFNAME] || tb[IFLA_ALT_IFNAME] || ifm->ifi_index > 0)
			err = -ENODEV;

		goto out;
	}

	err = rtnl_delete_link(dev, portid, nlh);

out:
	if (netnsid >= 0)
		put_net(tgt_net);

	return err;
}

int rtnl_configure_link(struct net_device *dev, const struct ifinfomsg *ifm,
			u32 portid, const struct nlmsghdr *nlh)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned int old_flags;
	int err;

	old_flags = dev->flags;
	if (ifm && (ifm->ifi_flags || ifm->ifi_change)) {
<<<<<<< HEAD
		err = __dev_change_flags(dev, rtnl_dev_combine_flags(dev, ifm));
=======
		err = __dev_change_flags(dev, rtnl_dev_combine_flags(dev, ifm),
					 NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (err < 0)
			return err;
	}

<<<<<<< HEAD
	dev->rtnl_link_state = RTNL_LINK_INITIALIZED;
	rtmsg_ifinfo(RTM_NEWLINK, dev, ~0U);

	__dev_notify_flags(dev, old_flags);
=======
	if (dev->rtnl_link_state == RTNL_LINK_INITIALIZED) {
		__dev_notify_flags(dev, old_flags, (old_flags ^ dev->flags), portid, nlh);
	} else {
		dev->rtnl_link_state = RTNL_LINK_INITIALIZED;
		__dev_notify_flags(dev, old_flags, ~0U, portid, nlh);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}
EXPORT_SYMBOL(rtnl_configure_link);

<<<<<<< HEAD
struct net_device *rtnl_create_link(struct net *src_net, struct net *net,
	char *ifname, const struct rtnl_link_ops *ops, struct nlattr *tb[])
{
	int err;
	struct net_device *dev;
	unsigned int num_queues = 1;
	unsigned int real_num_queues = 1;

	if (ops->get_tx_queues) {
		err = ops->get_tx_queues(src_net, tb, &num_queues,
					 &real_num_queues);
		if (err)
			goto err;
	}
	err = -ENOMEM;
	dev = alloc_netdev_mq(ops->priv_size, ifname, ops->setup, num_queues);
	if (!dev)
		goto err;
=======
struct net_device *rtnl_create_link(struct net *net, const char *ifname,
				    unsigned char name_assign_type,
				    const struct rtnl_link_ops *ops,
				    struct nlattr *tb[],
				    struct netlink_ext_ack *extack)
{
	struct net_device *dev;
	unsigned int num_tx_queues = 1;
	unsigned int num_rx_queues = 1;
	int err;

	if (tb[IFLA_NUM_TX_QUEUES])
		num_tx_queues = nla_get_u32(tb[IFLA_NUM_TX_QUEUES]);
	else if (ops->get_num_tx_queues)
		num_tx_queues = ops->get_num_tx_queues();

	if (tb[IFLA_NUM_RX_QUEUES])
		num_rx_queues = nla_get_u32(tb[IFLA_NUM_RX_QUEUES]);
	else if (ops->get_num_rx_queues)
		num_rx_queues = ops->get_num_rx_queues();

	if (num_tx_queues < 1 || num_tx_queues > 4096) {
		NL_SET_ERR_MSG(extack, "Invalid number of transmit queues");
		return ERR_PTR(-EINVAL);
	}

	if (num_rx_queues < 1 || num_rx_queues > 4096) {
		NL_SET_ERR_MSG(extack, "Invalid number of receive queues");
		return ERR_PTR(-EINVAL);
	}

	if (ops->alloc) {
		dev = ops->alloc(tb, ifname, name_assign_type,
				 num_tx_queues, num_rx_queues);
		if (IS_ERR(dev))
			return dev;
	} else {
		dev = alloc_netdev_mqs(ops->priv_size, ifname,
				       name_assign_type, ops->setup,
				       num_tx_queues, num_rx_queues);
	}

	if (!dev)
		return ERR_PTR(-ENOMEM);

	err = validate_linkmsg(dev, tb, extack);
	if (err < 0) {
		free_netdev(dev);
		return ERR_PTR(err);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	dev_net_set(dev, net);
	dev->rtnl_link_ops = ops;
	dev->rtnl_link_state = RTNL_LINK_INITIALIZING;

<<<<<<< HEAD
	if (tb[IFLA_MTU])
		dev->mtu = nla_get_u32(tb[IFLA_MTU]);
	if (tb[IFLA_ADDRESS])
		memcpy(dev->dev_addr, nla_data(tb[IFLA_ADDRESS]),
				nla_len(tb[IFLA_ADDRESS]));
=======
	if (tb[IFLA_MTU]) {
		u32 mtu = nla_get_u32(tb[IFLA_MTU]);

		err = dev_validate_mtu(dev, mtu, extack);
		if (err) {
			free_netdev(dev);
			return ERR_PTR(err);
		}
		dev->mtu = mtu;
	}
	if (tb[IFLA_ADDRESS]) {
		__dev_addr_set(dev, nla_data(tb[IFLA_ADDRESS]),
			       nla_len(tb[IFLA_ADDRESS]));
		dev->addr_assign_type = NET_ADDR_SET;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (tb[IFLA_BROADCAST])
		memcpy(dev->broadcast, nla_data(tb[IFLA_BROADCAST]),
				nla_len(tb[IFLA_BROADCAST]));
	if (tb[IFLA_TXQLEN])
		dev->tx_queue_len = nla_get_u32(tb[IFLA_TXQLEN]);
	if (tb[IFLA_OPERSTATE])
		set_operstate(dev, nla_get_u8(tb[IFLA_OPERSTATE]));
	if (tb[IFLA_LINKMODE])
		dev->link_mode = nla_get_u8(tb[IFLA_LINKMODE]);
	if (tb[IFLA_GROUP])
		dev_set_group(dev, nla_get_u32(tb[IFLA_GROUP]));
<<<<<<< HEAD

	return dev;

err:
	return ERR_PTR(err);
}
EXPORT_SYMBOL(rtnl_create_link);

static int rtnl_group_changelink(struct net *net, int group,
		struct ifinfomsg *ifm,
=======
	if (tb[IFLA_GSO_MAX_SIZE])
		netif_set_gso_max_size(dev, nla_get_u32(tb[IFLA_GSO_MAX_SIZE]));
	if (tb[IFLA_GSO_MAX_SEGS])
		netif_set_gso_max_segs(dev, nla_get_u32(tb[IFLA_GSO_MAX_SEGS]));
	if (tb[IFLA_GRO_MAX_SIZE])
		netif_set_gro_max_size(dev, nla_get_u32(tb[IFLA_GRO_MAX_SIZE]));
	if (tb[IFLA_GSO_IPV4_MAX_SIZE])
		netif_set_gso_ipv4_max_size(dev, nla_get_u32(tb[IFLA_GSO_IPV4_MAX_SIZE]));
	if (tb[IFLA_GRO_IPV4_MAX_SIZE])
		netif_set_gro_ipv4_max_size(dev, nla_get_u32(tb[IFLA_GRO_IPV4_MAX_SIZE]));

	return dev;
}
EXPORT_SYMBOL(rtnl_create_link);

static int rtnl_group_changelink(const struct sk_buff *skb,
		struct net *net, int group,
		struct ifinfomsg *ifm,
		struct netlink_ext_ack *extack,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		struct nlattr **tb)
{
	struct net_device *dev, *aux;
	int err;

	for_each_netdev_safe(net, dev, aux) {
		if (dev->group == group) {
<<<<<<< HEAD
			err = do_setlink(dev, ifm, tb, NULL, 0);
=======
			err = validate_linkmsg(dev, tb, extack);
			if (err < 0)
				return err;
			err = do_setlink(skb, dev, ifm, extack, tb, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (err < 0)
				return err;
		}
	}

	return 0;
}

<<<<<<< HEAD
static int rtnl_newlink(struct sk_buff *skb, struct nlmsghdr *nlh, void *arg)
{
	struct net *net = sock_net(skb->sk);
	const struct rtnl_link_ops *ops;
	struct net_device *dev;
	struct ifinfomsg *ifm;
	char kind[MODULE_NAME_LEN];
	char ifname[IFNAMSIZ];
	struct nlattr *tb[IFLA_MAX+1];
	struct nlattr *linkinfo[IFLA_INFO_MAX+1];
=======
static int rtnl_newlink_create(struct sk_buff *skb, struct ifinfomsg *ifm,
			       const struct rtnl_link_ops *ops,
			       const struct nlmsghdr *nlh,
			       struct nlattr **tb, struct nlattr **data,
			       struct netlink_ext_ack *extack)
{
	unsigned char name_assign_type = NET_NAME_USER;
	struct net *net = sock_net(skb->sk);
	u32 portid = NETLINK_CB(skb).portid;
	struct net *dest_net, *link_net;
	struct net_device *dev;
	char ifname[IFNAMSIZ];
	int err;

	if (!ops->alloc && !ops->setup)
		return -EOPNOTSUPP;

	if (tb[IFLA_IFNAME]) {
		nla_strscpy(ifname, tb[IFLA_IFNAME], IFNAMSIZ);
	} else {
		snprintf(ifname, IFNAMSIZ, "%s%%d", ops->kind);
		name_assign_type = NET_NAME_ENUM;
	}

	dest_net = rtnl_link_get_net_capable(skb, net, tb, CAP_NET_ADMIN);
	if (IS_ERR(dest_net))
		return PTR_ERR(dest_net);

	if (tb[IFLA_LINK_NETNSID]) {
		int id = nla_get_s32(tb[IFLA_LINK_NETNSID]);

		link_net = get_net_ns_by_id(dest_net, id);
		if (!link_net) {
			NL_SET_ERR_MSG(extack, "Unknown network namespace id");
			err =  -EINVAL;
			goto out;
		}
		err = -EPERM;
		if (!netlink_ns_capable(skb, link_net->user_ns, CAP_NET_ADMIN))
			goto out;
	} else {
		link_net = NULL;
	}

	dev = rtnl_create_link(link_net ? : dest_net, ifname,
			       name_assign_type, ops, tb, extack);
	if (IS_ERR(dev)) {
		err = PTR_ERR(dev);
		goto out;
	}

	dev->ifindex = ifm->ifi_index;

	if (ops->newlink)
		err = ops->newlink(link_net ? : net, dev, tb, data, extack);
	else
		err = register_netdevice(dev);
	if (err < 0) {
		free_netdev(dev);
		goto out;
	}

	err = rtnl_configure_link(dev, ifm, portid, nlh);
	if (err < 0)
		goto out_unregister;
	if (link_net) {
		err = dev_change_net_namespace(dev, dest_net, ifname);
		if (err < 0)
			goto out_unregister;
	}
	if (tb[IFLA_MASTER]) {
		err = do_set_master(dev, nla_get_u32(tb[IFLA_MASTER]), extack);
		if (err)
			goto out_unregister;
	}
out:
	if (link_net)
		put_net(link_net);
	put_net(dest_net);
	return err;
out_unregister:
	if (ops->newlink) {
		LIST_HEAD(list_kill);

		ops->dellink(dev, &list_kill);
		unregister_netdevice_many(&list_kill);
	} else {
		unregister_netdevice(dev);
	}
	goto out;
}

struct rtnl_newlink_tbs {
	struct nlattr *tb[IFLA_MAX + 1];
	struct nlattr *attr[RTNL_MAX_TYPE + 1];
	struct nlattr *slave_attr[RTNL_SLAVE_MAX_TYPE + 1];
};

static int __rtnl_newlink(struct sk_buff *skb, struct nlmsghdr *nlh,
			  struct rtnl_newlink_tbs *tbs,
			  struct netlink_ext_ack *extack)
{
	struct nlattr *linkinfo[IFLA_INFO_MAX + 1];
	struct nlattr ** const tb = tbs->tb;
	const struct rtnl_link_ops *m_ops;
	struct net_device *master_dev;
	struct net *net = sock_net(skb->sk);
	const struct rtnl_link_ops *ops;
	struct nlattr **slave_data;
	char kind[MODULE_NAME_LEN];
	struct net_device *dev;
	struct ifinfomsg *ifm;
	struct nlattr **data;
	bool link_specified;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int err;

#ifdef CONFIG_MODULES
replay:
#endif
<<<<<<< HEAD
	err = nlmsg_parse(nlh, sizeof(*ifm), tb, IFLA_MAX, ifla_policy);
	if (err < 0)
		return err;

	if (tb[IFLA_IFNAME])
		nla_strlcpy(ifname, tb[IFLA_IFNAME], IFNAMSIZ);
	else
		ifname[0] = '\0';

	ifm = nlmsg_data(nlh);
	if (ifm->ifi_index > 0)
		dev = __dev_get_by_index(net, ifm->ifi_index);
	else {
		if (ifname[0])
			dev = __dev_get_by_name(net, ifname);
		else
			dev = NULL;
	}

	err = validate_linkmsg(dev, tb);
	if (err < 0)
		return err;

	if (tb[IFLA_LINKINFO]) {
		err = nla_parse_nested(linkinfo, IFLA_INFO_MAX,
				       tb[IFLA_LINKINFO], ifla_info_policy);
=======
	err = nlmsg_parse_deprecated(nlh, sizeof(*ifm), tb, IFLA_MAX,
				     ifla_policy, extack);
	if (err < 0)
		return err;

	err = rtnl_ensure_unique_netns(tb, extack, false);
	if (err < 0)
		return err;

	ifm = nlmsg_data(nlh);
	if (ifm->ifi_index > 0) {
		link_specified = true;
		dev = __dev_get_by_index(net, ifm->ifi_index);
	} else if (ifm->ifi_index < 0) {
		NL_SET_ERR_MSG(extack, "ifindex can't be negative");
		return -EINVAL;
	} else if (tb[IFLA_IFNAME] || tb[IFLA_ALT_IFNAME]) {
		link_specified = true;
		dev = rtnl_dev_get(net, tb);
	} else {
		link_specified = false;
		dev = NULL;
	}

	master_dev = NULL;
	m_ops = NULL;
	if (dev) {
		master_dev = netdev_master_upper_dev_get(dev);
		if (master_dev)
			m_ops = master_dev->rtnl_link_ops;
	}

	if (tb[IFLA_LINKINFO]) {
		err = nla_parse_nested_deprecated(linkinfo, IFLA_INFO_MAX,
						  tb[IFLA_LINKINFO],
						  ifla_info_policy, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (err < 0)
			return err;
	} else
		memset(linkinfo, 0, sizeof(linkinfo));

	if (linkinfo[IFLA_INFO_KIND]) {
<<<<<<< HEAD
		nla_strlcpy(kind, linkinfo[IFLA_INFO_KIND], sizeof(kind));
=======
		nla_strscpy(kind, linkinfo[IFLA_INFO_KIND], sizeof(kind));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ops = rtnl_link_ops_get(kind);
	} else {
		kind[0] = '\0';
		ops = NULL;
	}

<<<<<<< HEAD
	if (1) {
		struct nlattr *attr[ops ? ops->maxtype + 1 : 0], **data = NULL;
		struct net *dest_net;

		if (ops) {
			if (ops->maxtype && linkinfo[IFLA_INFO_DATA]) {
				err = nla_parse_nested(attr, ops->maxtype,
						       linkinfo[IFLA_INFO_DATA],
						       ops->policy);
				if (err < 0)
					return err;
				data = attr;
			}
			if (ops->validate) {
				err = ops->validate(tb, data);
				if (err < 0)
					return err;
			}
		}

		if (dev) {
			int modified = 0;

			if (nlh->nlmsg_flags & NLM_F_EXCL)
				return -EEXIST;
			if (nlh->nlmsg_flags & NLM_F_REPLACE)
				return -EOPNOTSUPP;

			if (linkinfo[IFLA_INFO_DATA]) {
				if (!ops || ops != dev->rtnl_link_ops ||
				    !ops->changelink)
					return -EOPNOTSUPP;

				err = ops->changelink(dev, tb, data);
				if (err < 0)
					return err;
				modified = 1;
			}

			return do_setlink(dev, ifm, tb, ifname, modified);
		}

		if (!(nlh->nlmsg_flags & NLM_F_CREATE)) {
			if (ifm->ifi_index == 0 && tb[IFLA_GROUP])
				return rtnl_group_changelink(net,
						nla_get_u32(tb[IFLA_GROUP]),
						ifm, tb);
			return -ENODEV;
		}

		if (ifm->ifi_index)
			return -EOPNOTSUPP;
		if (tb[IFLA_MAP] || tb[IFLA_MASTER] || tb[IFLA_PROTINFO])
			return -EOPNOTSUPP;

		if (!ops) {
#ifdef CONFIG_MODULES
			if (kind[0]) {
				__rtnl_unlock();
				request_module("rtnl-link-%s", kind);
				rtnl_lock();
				ops = rtnl_link_ops_get(kind);
				if (ops)
					goto replay;
			}
#endif
			return -EOPNOTSUPP;
		}

		if (!ifname[0])
			snprintf(ifname, IFNAMSIZ, "%s%%d", ops->kind);

		dest_net = rtnl_link_get_net(net, tb);
		if (IS_ERR(dest_net))
			return PTR_ERR(dest_net);

		dev = rtnl_create_link(net, dest_net, ifname, ops, tb);

		if (IS_ERR(dev))
			err = PTR_ERR(dev);
		else if (ops->newlink)
			err = ops->newlink(net, dev, tb, data);
		else
			err = register_netdevice(dev);

		if (err < 0 && !IS_ERR(dev))
			free_netdev(dev);
		if (err < 0)
			goto out;

		err = rtnl_configure_link(dev, ifm);
		if (err < 0)
			unregister_netdevice(dev);
out:
		put_net(dest_net);
		return err;
	}
}

static int rtnl_getlink(struct sk_buff *skb, struct nlmsghdr* nlh, void *arg)
{
	struct net *net = sock_net(skb->sk);
	struct ifinfomsg *ifm;
	char ifname[IFNAMSIZ];
	struct nlattr *tb[IFLA_MAX+1];
	struct net_device *dev = NULL;
	struct sk_buff *nskb;
	int err;
	u32 ext_filter_mask = 0;

	err = nlmsg_parse(nlh, sizeof(*ifm), tb, IFLA_MAX, ifla_policy);
	if (err < 0)
		return err;

	if (tb[IFLA_IFNAME])
		nla_strlcpy(ifname, tb[IFLA_IFNAME], IFNAMSIZ);
=======
	data = NULL;
	if (ops) {
		if (ops->maxtype > RTNL_MAX_TYPE)
			return -EINVAL;

		if (ops->maxtype && linkinfo[IFLA_INFO_DATA]) {
			err = nla_parse_nested_deprecated(tbs->attr, ops->maxtype,
							  linkinfo[IFLA_INFO_DATA],
							  ops->policy, extack);
			if (err < 0)
				return err;
			data = tbs->attr;
		}
		if (ops->validate) {
			err = ops->validate(tb, data, extack);
			if (err < 0)
				return err;
		}
	}

	slave_data = NULL;
	if (m_ops) {
		if (m_ops->slave_maxtype > RTNL_SLAVE_MAX_TYPE)
			return -EINVAL;

		if (m_ops->slave_maxtype &&
		    linkinfo[IFLA_INFO_SLAVE_DATA]) {
			err = nla_parse_nested_deprecated(tbs->slave_attr,
							  m_ops->slave_maxtype,
							  linkinfo[IFLA_INFO_SLAVE_DATA],
							  m_ops->slave_policy,
							  extack);
			if (err < 0)
				return err;
			slave_data = tbs->slave_attr;
		}
	}

	if (dev) {
		int status = 0;

		if (nlh->nlmsg_flags & NLM_F_EXCL)
			return -EEXIST;
		if (nlh->nlmsg_flags & NLM_F_REPLACE)
			return -EOPNOTSUPP;

		err = validate_linkmsg(dev, tb, extack);
		if (err < 0)
			return err;

		if (linkinfo[IFLA_INFO_DATA]) {
			if (!ops || ops != dev->rtnl_link_ops ||
			    !ops->changelink)
				return -EOPNOTSUPP;

			err = ops->changelink(dev, tb, data, extack);
			if (err < 0)
				return err;
			status |= DO_SETLINK_NOTIFY;
		}

		if (linkinfo[IFLA_INFO_SLAVE_DATA]) {
			if (!m_ops || !m_ops->slave_changelink)
				return -EOPNOTSUPP;

			err = m_ops->slave_changelink(master_dev, dev, tb,
						      slave_data, extack);
			if (err < 0)
				return err;
			status |= DO_SETLINK_NOTIFY;
		}

		return do_setlink(skb, dev, ifm, extack, tb, status);
	}

	if (!(nlh->nlmsg_flags & NLM_F_CREATE)) {
		/* No dev found and NLM_F_CREATE not set. Requested dev does not exist,
		 * or it's for a group
		*/
		if (link_specified)
			return -ENODEV;
		if (tb[IFLA_GROUP])
			return rtnl_group_changelink(skb, net,
						nla_get_u32(tb[IFLA_GROUP]),
						ifm, extack, tb);
		return -ENODEV;
	}

	if (tb[IFLA_MAP] || tb[IFLA_PROTINFO])
		return -EOPNOTSUPP;

	if (!ops) {
#ifdef CONFIG_MODULES
		if (kind[0]) {
			__rtnl_unlock();
			request_module("rtnl-link-%s", kind);
			rtnl_lock();
			ops = rtnl_link_ops_get(kind);
			if (ops)
				goto replay;
		}
#endif
		NL_SET_ERR_MSG(extack, "Unknown device type");
		return -EOPNOTSUPP;
	}

	return rtnl_newlink_create(skb, ifm, ops, nlh, tb, data, extack);
}

static int rtnl_newlink(struct sk_buff *skb, struct nlmsghdr *nlh,
			struct netlink_ext_ack *extack)
{
	struct rtnl_newlink_tbs *tbs;
	int ret;

	tbs = kmalloc(sizeof(*tbs), GFP_KERNEL);
	if (!tbs)
		return -ENOMEM;

	ret = __rtnl_newlink(skb, nlh, tbs, extack);
	kfree(tbs);
	return ret;
}

static int rtnl_valid_getlink_req(struct sk_buff *skb,
				  const struct nlmsghdr *nlh,
				  struct nlattr **tb,
				  struct netlink_ext_ack *extack)
{
	struct ifinfomsg *ifm;
	int i, err;

	if (nlh->nlmsg_len < nlmsg_msg_size(sizeof(*ifm))) {
		NL_SET_ERR_MSG(extack, "Invalid header for get link");
		return -EINVAL;
	}

	if (!netlink_strict_get_check(skb))
		return nlmsg_parse_deprecated(nlh, sizeof(*ifm), tb, IFLA_MAX,
					      ifla_policy, extack);

	ifm = nlmsg_data(nlh);
	if (ifm->__ifi_pad || ifm->ifi_type || ifm->ifi_flags ||
	    ifm->ifi_change) {
		NL_SET_ERR_MSG(extack, "Invalid values in header for get link request");
		return -EINVAL;
	}

	err = nlmsg_parse_deprecated_strict(nlh, sizeof(*ifm), tb, IFLA_MAX,
					    ifla_policy, extack);
	if (err)
		return err;

	for (i = 0; i <= IFLA_MAX; i++) {
		if (!tb[i])
			continue;

		switch (i) {
		case IFLA_IFNAME:
		case IFLA_ALT_IFNAME:
		case IFLA_EXT_MASK:
		case IFLA_TARGET_NETNSID:
			break;
		default:
			NL_SET_ERR_MSG(extack, "Unsupported attribute in get link request");
			return -EINVAL;
		}
	}

	return 0;
}

static int rtnl_getlink(struct sk_buff *skb, struct nlmsghdr *nlh,
			struct netlink_ext_ack *extack)
{
	struct net *net = sock_net(skb->sk);
	struct net *tgt_net = net;
	struct ifinfomsg *ifm;
	struct nlattr *tb[IFLA_MAX+1];
	struct net_device *dev = NULL;
	struct sk_buff *nskb;
	int netnsid = -1;
	int err;
	u32 ext_filter_mask = 0;

	err = rtnl_valid_getlink_req(skb, nlh, tb, extack);
	if (err < 0)
		return err;

	err = rtnl_ensure_unique_netns(tb, extack, true);
	if (err < 0)
		return err;

	if (tb[IFLA_TARGET_NETNSID]) {
		netnsid = nla_get_s32(tb[IFLA_TARGET_NETNSID]);
		tgt_net = rtnl_get_net_ns_capable(NETLINK_CB(skb).sk, netnsid);
		if (IS_ERR(tgt_net))
			return PTR_ERR(tgt_net);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (tb[IFLA_EXT_MASK])
		ext_filter_mask = nla_get_u32(tb[IFLA_EXT_MASK]);

<<<<<<< HEAD
	ifm = nlmsg_data(nlh);
	if (ifm->ifi_index > 0)
		dev = __dev_get_by_index(net, ifm->ifi_index);
	else if (tb[IFLA_IFNAME])
		dev = __dev_get_by_name(net, ifname);
	else
		return -EINVAL;

	if (dev == NULL)
		return -ENODEV;

	nskb = nlmsg_new(if_nlmsg_size(dev, ext_filter_mask), GFP_KERNEL);
	if (nskb == NULL)
		return -ENOBUFS;

	err = rtnl_fill_ifinfo(nskb, dev, RTM_NEWLINK, NETLINK_CB(skb).pid,
			       nlh->nlmsg_seq, 0, 0, ext_filter_mask);
=======
	err = -EINVAL;
	ifm = nlmsg_data(nlh);
	if (ifm->ifi_index > 0)
		dev = __dev_get_by_index(tgt_net, ifm->ifi_index);
	else if (tb[IFLA_IFNAME] || tb[IFLA_ALT_IFNAME])
		dev = rtnl_dev_get(tgt_net, tb);
	else
		goto out;

	err = -ENODEV;
	if (dev == NULL)
		goto out;

	err = -ENOBUFS;
	nskb = nlmsg_new_large(if_nlmsg_size(dev, ext_filter_mask));
	if (nskb == NULL)
		goto out;

	/* Synchronize the carrier state so we don't report a state
	 * that we're not actually going to honour immediately; if
	 * the driver just did a carrier off->on transition, we can
	 * only TX if link watch work has run, but without this we'd
	 * already report carrier on, even if it doesn't work yet.
	 */
	linkwatch_sync_dev(dev);

	err = rtnl_fill_ifinfo(nskb, dev, net,
			       RTM_NEWLINK, NETLINK_CB(skb).portid,
			       nlh->nlmsg_seq, 0, 0, ext_filter_mask,
			       0, NULL, 0, netnsid, GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err < 0) {
		/* -EMSGSIZE implies BUG in if_nlmsg_size */
		WARN_ON(err == -EMSGSIZE);
		kfree_skb(nskb);
	} else
<<<<<<< HEAD
		err = rtnl_unicast(nskb, net, NETLINK_CB(skb).pid);
=======
		err = rtnl_unicast(nskb, net, NETLINK_CB(skb).portid);
out:
	if (netnsid >= 0)
		put_net(tgt_net);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return err;
}

<<<<<<< HEAD
static u16 rtnl_calcit(struct sk_buff *skb, struct nlmsghdr *nlh)
{
	struct net *net = sock_net(skb->sk);
	struct net_device *dev;
	struct nlattr *tb[IFLA_MAX+1];
	u32 ext_filter_mask = 0;
	u16 min_ifinfo_dump_size = 0;

	if (nlmsg_parse(nlh, sizeof(struct ifinfomsg), tb, IFLA_MAX,
			ifla_policy) >= 0) {
=======
static int rtnl_alt_ifname(int cmd, struct net_device *dev, struct nlattr *attr,
			   bool *changed, struct netlink_ext_ack *extack)
{
	char *alt_ifname;
	size_t size;
	int err;

	err = nla_validate(attr, attr->nla_len, IFLA_MAX, ifla_policy, extack);
	if (err)
		return err;

	if (cmd == RTM_NEWLINKPROP) {
		size = rtnl_prop_list_size(dev);
		size += nla_total_size(ALTIFNAMSIZ);
		if (size >= U16_MAX) {
			NL_SET_ERR_MSG(extack,
				       "effective property list too long");
			return -EINVAL;
		}
	}

	alt_ifname = nla_strdup(attr, GFP_KERNEL_ACCOUNT);
	if (!alt_ifname)
		return -ENOMEM;

	if (cmd == RTM_NEWLINKPROP) {
		err = netdev_name_node_alt_create(dev, alt_ifname);
		if (!err)
			alt_ifname = NULL;
	} else if (cmd == RTM_DELLINKPROP) {
		err = netdev_name_node_alt_destroy(dev, alt_ifname);
	} else {
		WARN_ON_ONCE(1);
		err = -EINVAL;
	}

	kfree(alt_ifname);
	if (!err)
		*changed = true;
	return err;
}

static int rtnl_linkprop(int cmd, struct sk_buff *skb, struct nlmsghdr *nlh,
			 struct netlink_ext_ack *extack)
{
	struct net *net = sock_net(skb->sk);
	struct nlattr *tb[IFLA_MAX + 1];
	struct net_device *dev;
	struct ifinfomsg *ifm;
	bool changed = false;
	struct nlattr *attr;
	int err, rem;

	err = nlmsg_parse(nlh, sizeof(*ifm), tb, IFLA_MAX, ifla_policy, extack);
	if (err)
		return err;

	err = rtnl_ensure_unique_netns(tb, extack, true);
	if (err)
		return err;

	ifm = nlmsg_data(nlh);
	if (ifm->ifi_index > 0)
		dev = __dev_get_by_index(net, ifm->ifi_index);
	else if (tb[IFLA_IFNAME] || tb[IFLA_ALT_IFNAME])
		dev = rtnl_dev_get(net, tb);
	else
		return -EINVAL;

	if (!dev)
		return -ENODEV;

	if (!tb[IFLA_PROP_LIST])
		return 0;

	nla_for_each_nested(attr, tb[IFLA_PROP_LIST], rem) {
		switch (nla_type(attr)) {
		case IFLA_ALT_IFNAME:
			err = rtnl_alt_ifname(cmd, dev, attr, &changed, extack);
			if (err)
				return err;
			break;
		}
	}

	if (changed)
		netdev_state_change(dev);
	return 0;
}

static int rtnl_newlinkprop(struct sk_buff *skb, struct nlmsghdr *nlh,
			    struct netlink_ext_ack *extack)
{
	return rtnl_linkprop(RTM_NEWLINKPROP, skb, nlh, extack);
}

static int rtnl_dellinkprop(struct sk_buff *skb, struct nlmsghdr *nlh,
			    struct netlink_ext_ack *extack)
{
	return rtnl_linkprop(RTM_DELLINKPROP, skb, nlh, extack);
}

static u32 rtnl_calcit(struct sk_buff *skb, struct nlmsghdr *nlh)
{
	struct net *net = sock_net(skb->sk);
	size_t min_ifinfo_dump_size = 0;
	struct nlattr *tb[IFLA_MAX+1];
	u32 ext_filter_mask = 0;
	struct net_device *dev;
	int hdrlen;

	/* Same kernel<->userspace interface hack as in rtnl_dump_ifinfo. */
	hdrlen = nlmsg_len(nlh) < sizeof(struct ifinfomsg) ?
		 sizeof(struct rtgenmsg) : sizeof(struct ifinfomsg);

	if (nlmsg_parse_deprecated(nlh, hdrlen, tb, IFLA_MAX, ifla_policy, NULL) >= 0) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (tb[IFLA_EXT_MASK])
			ext_filter_mask = nla_get_u32(tb[IFLA_EXT_MASK]);
	}

	if (!ext_filter_mask)
		return NLMSG_GOODSIZE;
	/*
	 * traverse the list of net devices and compute the minimum
	 * buffer size based upon the filter mask.
	 */
<<<<<<< HEAD
	list_for_each_entry(dev, &net->dev_base_head, dev_list) {
		min_ifinfo_dump_size = max_t(u16, min_ifinfo_dump_size,
					     if_nlmsg_size(dev,
						           ext_filter_mask));
	}

	return min_ifinfo_dump_size;
=======
	rcu_read_lock();
	for_each_netdev_rcu(net, dev) {
		min_ifinfo_dump_size = max(min_ifinfo_dump_size,
					   if_nlmsg_size(dev, ext_filter_mask));
	}
	rcu_read_unlock();

	return nlmsg_total_size(min_ifinfo_dump_size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int rtnl_dump_all(struct sk_buff *skb, struct netlink_callback *cb)
{
	int idx;
	int s_idx = cb->family;
<<<<<<< HEAD

	if (s_idx == 0)
		s_idx = 1;
	for (idx = 1; idx <= RTNL_FAMILY_MAX; idx++) {
		int type = cb->nlh->nlmsg_type-RTM_BASE;
		if (idx < s_idx || idx == PF_PACKET)
			continue;
		if (rtnl_msg_handlers[idx] == NULL ||
		    rtnl_msg_handlers[idx][type].dumpit == NULL)
			continue;
		if (idx > s_idx)
			memset(&cb->args[0], 0, sizeof(cb->args));
		if (rtnl_msg_handlers[idx][type].dumpit(skb, cb))
=======
	int type = cb->nlh->nlmsg_type - RTM_BASE;
	int ret = 0;

	if (s_idx == 0)
		s_idx = 1;

	for (idx = 1; idx <= RTNL_FAMILY_MAX; idx++) {
		struct rtnl_link __rcu **tab;
		struct rtnl_link *link;
		rtnl_dumpit_func dumpit;

		if (idx < s_idx || idx == PF_PACKET)
			continue;

		if (type < 0 || type >= RTM_NR_MSGTYPES)
			continue;

		tab = rcu_dereference_rtnl(rtnl_msg_handlers[idx]);
		if (!tab)
			continue;

		link = rcu_dereference_rtnl(tab[type]);
		if (!link)
			continue;

		dumpit = link->dumpit;
		if (!dumpit)
			continue;

		if (idx > s_idx) {
			memset(&cb->args[0], 0, sizeof(cb->args));
			cb->prev_seq = 0;
			cb->seq = 0;
		}
		ret = dumpit(skb, cb);
		if (ret)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
	}
	cb->family = idx;

<<<<<<< HEAD
	return skb->len;
}

void rtmsg_ifinfo(int type, struct net_device *dev, unsigned change)
=======
	return skb->len ? : ret;
}

struct sk_buff *rtmsg_ifinfo_build_skb(int type, struct net_device *dev,
				       unsigned int change,
				       u32 event, gfp_t flags, int *new_nsid,
				       int new_ifindex, u32 portid,
				       const struct nlmsghdr *nlh)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct net *net = dev_net(dev);
	struct sk_buff *skb;
	int err = -ENOBUFS;
<<<<<<< HEAD
	size_t if_info_size;

	skb = nlmsg_new((if_info_size = if_nlmsg_size(dev, 0)), GFP_KERNEL);
	if (skb == NULL)
		goto errout;

	err = rtnl_fill_ifinfo(skb, dev, type, 0, 0, change, 0, 0);
=======
	u32 seq = 0;

	skb = nlmsg_new(if_nlmsg_size(dev, 0), flags);
	if (skb == NULL)
		goto errout;

	if (nlmsg_report(nlh))
		seq = nlmsg_seq(nlh);
	else
		portid = 0;

	err = rtnl_fill_ifinfo(skb, dev, dev_net(dev),
			       type, portid, seq, change, 0, 0, event,
			       new_nsid, new_ifindex, -1, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err < 0) {
		/* -EMSGSIZE implies BUG in if_nlmsg_size() */
		WARN_ON(err == -EMSGSIZE);
		kfree_skb(skb);
		goto errout;
	}
<<<<<<< HEAD
	rtnl_notify(skb, net, 0, RTNLGRP_LINK, NULL, GFP_KERNEL);
	return;
errout:
	if (err < 0)
		rtnl_set_sk_err(net, RTNLGRP_LINK, err);
}

/* Protected by RTNL sempahore.  */
static struct rtattr **rta_buf;
static int rtattr_max;

/* Process one rtnetlink message. */

static int rtnetlink_rcv_msg(struct sk_buff *skb, struct nlmsghdr *nlh)
{
	struct net *net = sock_net(skb->sk);
	rtnl_doit_func doit;
	int sz_idx, kind;
	int min_len;
	int family;
	int type;
	int err;
=======
	return skb;
errout:
	if (err < 0)
		rtnl_set_sk_err(net, RTNLGRP_LINK, err);
	return NULL;
}

void rtmsg_ifinfo_send(struct sk_buff *skb, struct net_device *dev, gfp_t flags,
		       u32 portid, const struct nlmsghdr *nlh)
{
	struct net *net = dev_net(dev);

	rtnl_notify(skb, net, portid, RTNLGRP_LINK, nlh, flags);
}

static void rtmsg_ifinfo_event(int type, struct net_device *dev,
			       unsigned int change, u32 event,
			       gfp_t flags, int *new_nsid, int new_ifindex,
			       u32 portid, const struct nlmsghdr *nlh)
{
	struct sk_buff *skb;

	if (dev->reg_state != NETREG_REGISTERED)
		return;

	skb = rtmsg_ifinfo_build_skb(type, dev, change, event, flags, new_nsid,
				     new_ifindex, portid, nlh);
	if (skb)
		rtmsg_ifinfo_send(skb, dev, flags, portid, nlh);
}

void rtmsg_ifinfo(int type, struct net_device *dev, unsigned int change,
		  gfp_t flags, u32 portid, const struct nlmsghdr *nlh)
{
	rtmsg_ifinfo_event(type, dev, change, rtnl_get_event(0), flags,
			   NULL, 0, portid, nlh);
}

void rtmsg_ifinfo_newnet(int type, struct net_device *dev, unsigned int change,
			 gfp_t flags, int *new_nsid, int new_ifindex)
{
	rtmsg_ifinfo_event(type, dev, change, rtnl_get_event(0), flags,
			   new_nsid, new_ifindex, 0, NULL);
}

static int nlmsg_populate_fdb_fill(struct sk_buff *skb,
				   struct net_device *dev,
				   u8 *addr, u16 vid, u32 pid, u32 seq,
				   int type, unsigned int flags,
				   int nlflags, u16 ndm_state)
{
	struct nlmsghdr *nlh;
	struct ndmsg *ndm;

	nlh = nlmsg_put(skb, pid, seq, type, sizeof(*ndm), nlflags);
	if (!nlh)
		return -EMSGSIZE;

	ndm = nlmsg_data(nlh);
	ndm->ndm_family  = AF_BRIDGE;
	ndm->ndm_pad1	 = 0;
	ndm->ndm_pad2    = 0;
	ndm->ndm_flags	 = flags;
	ndm->ndm_type	 = 0;
	ndm->ndm_ifindex = dev->ifindex;
	ndm->ndm_state   = ndm_state;

	if (nla_put(skb, NDA_LLADDR, dev->addr_len, addr))
		goto nla_put_failure;
	if (vid)
		if (nla_put(skb, NDA_VLAN, sizeof(u16), &vid))
			goto nla_put_failure;

	nlmsg_end(skb, nlh);
	return 0;

nla_put_failure:
	nlmsg_cancel(skb, nlh);
	return -EMSGSIZE;
}

static inline size_t rtnl_fdb_nlmsg_size(const struct net_device *dev)
{
	return NLMSG_ALIGN(sizeof(struct ndmsg)) +
	       nla_total_size(dev->addr_len) +	/* NDA_LLADDR */
	       nla_total_size(sizeof(u16)) +	/* NDA_VLAN */
	       0;
}

static void rtnl_fdb_notify(struct net_device *dev, u8 *addr, u16 vid, int type,
			    u16 ndm_state)
{
	struct net *net = dev_net(dev);
	struct sk_buff *skb;
	int err = -ENOBUFS;

	skb = nlmsg_new(rtnl_fdb_nlmsg_size(dev), GFP_ATOMIC);
	if (!skb)
		goto errout;

	err = nlmsg_populate_fdb_fill(skb, dev, addr, vid,
				      0, 0, type, NTF_SELF, 0, ndm_state);
	if (err < 0) {
		kfree_skb(skb);
		goto errout;
	}

	rtnl_notify(skb, net, 0, RTNLGRP_NEIGH, NULL, GFP_ATOMIC);
	return;
errout:
	rtnl_set_sk_err(net, RTNLGRP_NEIGH, err);
}

/*
 * ndo_dflt_fdb_add - default netdevice operation to add an FDB entry
 */
int ndo_dflt_fdb_add(struct ndmsg *ndm,
		     struct nlattr *tb[],
		     struct net_device *dev,
		     const unsigned char *addr, u16 vid,
		     u16 flags)
{
	int err = -EINVAL;

	/* If aging addresses are supported device will need to
	 * implement its own handler for this.
	 */
	if (ndm->ndm_state && !(ndm->ndm_state & NUD_PERMANENT)) {
		netdev_info(dev, "default FDB implementation only supports local addresses\n");
		return err;
	}

	if (tb[NDA_FLAGS_EXT]) {
		netdev_info(dev, "invalid flags given to default FDB implementation\n");
		return err;
	}

	if (vid) {
		netdev_info(dev, "vlans aren't supported yet for dev_uc|mc_add()\n");
		return err;
	}

	if (is_unicast_ether_addr(addr) || is_link_local_ether_addr(addr))
		err = dev_uc_add_excl(dev, addr);
	else if (is_multicast_ether_addr(addr))
		err = dev_mc_add_excl(dev, addr);

	/* Only return duplicate errors if NLM_F_EXCL is set */
	if (err == -EEXIST && !(flags & NLM_F_EXCL))
		err = 0;

	return err;
}
EXPORT_SYMBOL(ndo_dflt_fdb_add);

static int fdb_vid_parse(struct nlattr *vlan_attr, u16 *p_vid,
			 struct netlink_ext_ack *extack)
{
	u16 vid = 0;

	if (vlan_attr) {
		if (nla_len(vlan_attr) != sizeof(u16)) {
			NL_SET_ERR_MSG(extack, "invalid vlan attribute size");
			return -EINVAL;
		}

		vid = nla_get_u16(vlan_attr);

		if (!vid || vid >= VLAN_VID_MASK) {
			NL_SET_ERR_MSG(extack, "invalid vlan id");
			return -EINVAL;
		}
	}
	*p_vid = vid;
	return 0;
}

static int rtnl_fdb_add(struct sk_buff *skb, struct nlmsghdr *nlh,
			struct netlink_ext_ack *extack)
{
	struct net *net = sock_net(skb->sk);
	struct ndmsg *ndm;
	struct nlattr *tb[NDA_MAX+1];
	struct net_device *dev;
	u8 *addr;
	u16 vid;
	int err;

	err = nlmsg_parse_deprecated(nlh, sizeof(*ndm), tb, NDA_MAX, NULL,
				     extack);
	if (err < 0)
		return err;

	ndm = nlmsg_data(nlh);
	if (ndm->ndm_ifindex == 0) {
		NL_SET_ERR_MSG(extack, "invalid ifindex");
		return -EINVAL;
	}

	dev = __dev_get_by_index(net, ndm->ndm_ifindex);
	if (dev == NULL) {
		NL_SET_ERR_MSG(extack, "unknown ifindex");
		return -ENODEV;
	}

	if (!tb[NDA_LLADDR] || nla_len(tb[NDA_LLADDR]) != ETH_ALEN) {
		NL_SET_ERR_MSG(extack, "invalid address");
		return -EINVAL;
	}

	if (dev->type != ARPHRD_ETHER) {
		NL_SET_ERR_MSG(extack, "FDB add only supported for Ethernet devices");
		return -EINVAL;
	}

	addr = nla_data(tb[NDA_LLADDR]);

	err = fdb_vid_parse(tb[NDA_VLAN], &vid, extack);
	if (err)
		return err;

	err = -EOPNOTSUPP;

	/* Support fdb on master device the net/bridge default case */
	if ((!ndm->ndm_flags || ndm->ndm_flags & NTF_MASTER) &&
	    netif_is_bridge_port(dev)) {
		struct net_device *br_dev = netdev_master_upper_dev_get(dev);
		const struct net_device_ops *ops = br_dev->netdev_ops;

		err = ops->ndo_fdb_add(ndm, tb, dev, addr, vid,
				       nlh->nlmsg_flags, extack);
		if (err)
			goto out;
		else
			ndm->ndm_flags &= ~NTF_MASTER;
	}

	/* Embedded bridge, macvlan, and any other device support */
	if ((ndm->ndm_flags & NTF_SELF)) {
		if (dev->netdev_ops->ndo_fdb_add)
			err = dev->netdev_ops->ndo_fdb_add(ndm, tb, dev, addr,
							   vid,
							   nlh->nlmsg_flags,
							   extack);
		else
			err = ndo_dflt_fdb_add(ndm, tb, dev, addr, vid,
					       nlh->nlmsg_flags);

		if (!err) {
			rtnl_fdb_notify(dev, addr, vid, RTM_NEWNEIGH,
					ndm->ndm_state);
			ndm->ndm_flags &= ~NTF_SELF;
		}
	}
out:
	return err;
}

/*
 * ndo_dflt_fdb_del - default netdevice operation to delete an FDB entry
 */
int ndo_dflt_fdb_del(struct ndmsg *ndm,
		     struct nlattr *tb[],
		     struct net_device *dev,
		     const unsigned char *addr, u16 vid)
{
	int err = -EINVAL;

	/* If aging addresses are supported device will need to
	 * implement its own handler for this.
	 */
	if (!(ndm->ndm_state & NUD_PERMANENT)) {
		netdev_info(dev, "default FDB implementation only supports local addresses\n");
		return err;
	}

	if (is_unicast_ether_addr(addr) || is_link_local_ether_addr(addr))
		err = dev_uc_del(dev, addr);
	else if (is_multicast_ether_addr(addr))
		err = dev_mc_del(dev, addr);

	return err;
}
EXPORT_SYMBOL(ndo_dflt_fdb_del);

static int rtnl_fdb_del(struct sk_buff *skb, struct nlmsghdr *nlh,
			struct netlink_ext_ack *extack)
{
	bool del_bulk = !!(nlh->nlmsg_flags & NLM_F_BULK);
	struct net *net = sock_net(skb->sk);
	const struct net_device_ops *ops;
	struct ndmsg *ndm;
	struct nlattr *tb[NDA_MAX+1];
	struct net_device *dev;
	__u8 *addr = NULL;
	int err;
	u16 vid;

	if (!netlink_capable(skb, CAP_NET_ADMIN))
		return -EPERM;

	if (!del_bulk) {
		err = nlmsg_parse_deprecated(nlh, sizeof(*ndm), tb, NDA_MAX,
					     NULL, extack);
	} else {
		/* For bulk delete, the drivers will parse the message with
		 * policy.
		 */
		err = nlmsg_parse(nlh, sizeof(*ndm), tb, NDA_MAX, NULL, extack);
	}
	if (err < 0)
		return err;

	ndm = nlmsg_data(nlh);
	if (ndm->ndm_ifindex == 0) {
		NL_SET_ERR_MSG(extack, "invalid ifindex");
		return -EINVAL;
	}

	dev = __dev_get_by_index(net, ndm->ndm_ifindex);
	if (dev == NULL) {
		NL_SET_ERR_MSG(extack, "unknown ifindex");
		return -ENODEV;
	}

	if (!del_bulk) {
		if (!tb[NDA_LLADDR] || nla_len(tb[NDA_LLADDR]) != ETH_ALEN) {
			NL_SET_ERR_MSG(extack, "invalid address");
			return -EINVAL;
		}
		addr = nla_data(tb[NDA_LLADDR]);

		err = fdb_vid_parse(tb[NDA_VLAN], &vid, extack);
		if (err)
			return err;
	}

	if (dev->type != ARPHRD_ETHER) {
		NL_SET_ERR_MSG(extack, "FDB delete only supported for Ethernet devices");
		return -EINVAL;
	}

	err = -EOPNOTSUPP;

	/* Support fdb on master device the net/bridge default case */
	if ((!ndm->ndm_flags || ndm->ndm_flags & NTF_MASTER) &&
	    netif_is_bridge_port(dev)) {
		struct net_device *br_dev = netdev_master_upper_dev_get(dev);

		ops = br_dev->netdev_ops;
		if (!del_bulk) {
			if (ops->ndo_fdb_del)
				err = ops->ndo_fdb_del(ndm, tb, dev, addr, vid, extack);
		} else {
			if (ops->ndo_fdb_del_bulk)
				err = ops->ndo_fdb_del_bulk(nlh, dev, extack);
		}

		if (err)
			goto out;
		else
			ndm->ndm_flags &= ~NTF_MASTER;
	}

	/* Embedded bridge, macvlan, and any other device support */
	if (ndm->ndm_flags & NTF_SELF) {
		ops = dev->netdev_ops;
		if (!del_bulk) {
			if (ops->ndo_fdb_del)
				err = ops->ndo_fdb_del(ndm, tb, dev, addr, vid, extack);
			else
				err = ndo_dflt_fdb_del(ndm, tb, dev, addr, vid);
		} else {
			/* in case err was cleared by NTF_MASTER call */
			err = -EOPNOTSUPP;
			if (ops->ndo_fdb_del_bulk)
				err = ops->ndo_fdb_del_bulk(nlh, dev, extack);
		}

		if (!err) {
			if (!del_bulk)
				rtnl_fdb_notify(dev, addr, vid, RTM_DELNEIGH,
						ndm->ndm_state);
			ndm->ndm_flags &= ~NTF_SELF;
		}
	}
out:
	return err;
}

static int nlmsg_populate_fdb(struct sk_buff *skb,
			      struct netlink_callback *cb,
			      struct net_device *dev,
			      int *idx,
			      struct netdev_hw_addr_list *list)
{
	struct netdev_hw_addr *ha;
	int err;
	u32 portid, seq;

	portid = NETLINK_CB(cb->skb).portid;
	seq = cb->nlh->nlmsg_seq;

	list_for_each_entry(ha, &list->list, list) {
		if (*idx < cb->args[2])
			goto skip;

		err = nlmsg_populate_fdb_fill(skb, dev, ha->addr, 0,
					      portid, seq,
					      RTM_NEWNEIGH, NTF_SELF,
					      NLM_F_MULTI, NUD_PERMANENT);
		if (err < 0)
			return err;
skip:
		*idx += 1;
	}
	return 0;
}

/**
 * ndo_dflt_fdb_dump - default netdevice operation to dump an FDB table.
 * @skb: socket buffer to store message in
 * @cb: netlink callback
 * @dev: netdevice
 * @filter_dev: ignored
 * @idx: the number of FDB table entries dumped is added to *@idx
 *
 * Default netdevice operation to dump the existing unicast address list.
 * Returns number of addresses from list put in skb.
 */
int ndo_dflt_fdb_dump(struct sk_buff *skb,
		      struct netlink_callback *cb,
		      struct net_device *dev,
		      struct net_device *filter_dev,
		      int *idx)
{
	int err;

	if (dev->type != ARPHRD_ETHER)
		return -EINVAL;

	netif_addr_lock_bh(dev);
	err = nlmsg_populate_fdb(skb, cb, dev, idx, &dev->uc);
	if (err)
		goto out;
	err = nlmsg_populate_fdb(skb, cb, dev, idx, &dev->mc);
out:
	netif_addr_unlock_bh(dev);
	return err;
}
EXPORT_SYMBOL(ndo_dflt_fdb_dump);

static int valid_fdb_dump_strict(const struct nlmsghdr *nlh,
				 int *br_idx, int *brport_idx,
				 struct netlink_ext_ack *extack)
{
	struct nlattr *tb[NDA_MAX + 1];
	struct ndmsg *ndm;
	int err, i;

	if (nlh->nlmsg_len < nlmsg_msg_size(sizeof(*ndm))) {
		NL_SET_ERR_MSG(extack, "Invalid header for fdb dump request");
		return -EINVAL;
	}

	ndm = nlmsg_data(nlh);
	if (ndm->ndm_pad1  || ndm->ndm_pad2  || ndm->ndm_state ||
	    ndm->ndm_flags || ndm->ndm_type) {
		NL_SET_ERR_MSG(extack, "Invalid values in header for fdb dump request");
		return -EINVAL;
	}

	err = nlmsg_parse_deprecated_strict(nlh, sizeof(struct ndmsg), tb,
					    NDA_MAX, NULL, extack);
	if (err < 0)
		return err;

	*brport_idx = ndm->ndm_ifindex;
	for (i = 0; i <= NDA_MAX; ++i) {
		if (!tb[i])
			continue;

		switch (i) {
		case NDA_IFINDEX:
			if (nla_len(tb[i]) != sizeof(u32)) {
				NL_SET_ERR_MSG(extack, "Invalid IFINDEX attribute in fdb dump request");
				return -EINVAL;
			}
			*brport_idx = nla_get_u32(tb[NDA_IFINDEX]);
			break;
		case NDA_MASTER:
			if (nla_len(tb[i]) != sizeof(u32)) {
				NL_SET_ERR_MSG(extack, "Invalid MASTER attribute in fdb dump request");
				return -EINVAL;
			}
			*br_idx = nla_get_u32(tb[NDA_MASTER]);
			break;
		default:
			NL_SET_ERR_MSG(extack, "Unsupported attribute in fdb dump request");
			return -EINVAL;
		}
	}

	return 0;
}

static int valid_fdb_dump_legacy(const struct nlmsghdr *nlh,
				 int *br_idx, int *brport_idx,
				 struct netlink_ext_ack *extack)
{
	struct nlattr *tb[IFLA_MAX+1];
	int err;

	/* A hack to preserve kernel<->userspace interface.
	 * Before Linux v4.12 this code accepted ndmsg since iproute2 v3.3.0.
	 * However, ndmsg is shorter than ifinfomsg thus nlmsg_parse() bails.
	 * So, check for ndmsg with an optional u32 attribute (not used here).
	 * Fortunately these sizes don't conflict with the size of ifinfomsg
	 * with an optional attribute.
	 */
	if (nlmsg_len(nlh) != sizeof(struct ndmsg) &&
	    (nlmsg_len(nlh) != sizeof(struct ndmsg) +
	     nla_attr_size(sizeof(u32)))) {
		struct ifinfomsg *ifm;

		err = nlmsg_parse_deprecated(nlh, sizeof(struct ifinfomsg),
					     tb, IFLA_MAX, ifla_policy,
					     extack);
		if (err < 0) {
			return -EINVAL;
		} else if (err == 0) {
			if (tb[IFLA_MASTER])
				*br_idx = nla_get_u32(tb[IFLA_MASTER]);
		}

		ifm = nlmsg_data(nlh);
		*brport_idx = ifm->ifi_index;
	}
	return 0;
}

static int rtnl_fdb_dump(struct sk_buff *skb, struct netlink_callback *cb)
{
	struct net_device *dev;
	struct net_device *br_dev = NULL;
	const struct net_device_ops *ops = NULL;
	const struct net_device_ops *cops = NULL;
	struct net *net = sock_net(skb->sk);
	struct hlist_head *head;
	int brport_idx = 0;
	int br_idx = 0;
	int h, s_h;
	int idx = 0, s_idx;
	int err = 0;
	int fidx = 0;

	if (cb->strict_check)
		err = valid_fdb_dump_strict(cb->nlh, &br_idx, &brport_idx,
					    cb->extack);
	else
		err = valid_fdb_dump_legacy(cb->nlh, &br_idx, &brport_idx,
					    cb->extack);
	if (err < 0)
		return err;

	if (br_idx) {
		br_dev = __dev_get_by_index(net, br_idx);
		if (!br_dev)
			return -ENODEV;

		ops = br_dev->netdev_ops;
	}

	s_h = cb->args[0];
	s_idx = cb->args[1];

	for (h = s_h; h < NETDEV_HASHENTRIES; h++, s_idx = 0) {
		idx = 0;
		head = &net->dev_index_head[h];
		hlist_for_each_entry(dev, head, index_hlist) {

			if (brport_idx && (dev->ifindex != brport_idx))
				continue;

			if (!br_idx) { /* user did not specify a specific bridge */
				if (netif_is_bridge_port(dev)) {
					br_dev = netdev_master_upper_dev_get(dev);
					cops = br_dev->netdev_ops;
				}
			} else {
				if (dev != br_dev &&
				    !netif_is_bridge_port(dev))
					continue;

				if (br_dev != netdev_master_upper_dev_get(dev) &&
				    !netif_is_bridge_master(dev))
					continue;
				cops = ops;
			}

			if (idx < s_idx)
				goto cont;

			if (netif_is_bridge_port(dev)) {
				if (cops && cops->ndo_fdb_dump) {
					err = cops->ndo_fdb_dump(skb, cb,
								br_dev, dev,
								&fidx);
					if (err == -EMSGSIZE)
						goto out;
				}
			}

			if (dev->netdev_ops->ndo_fdb_dump)
				err = dev->netdev_ops->ndo_fdb_dump(skb, cb,
								    dev, NULL,
								    &fidx);
			else
				err = ndo_dflt_fdb_dump(skb, cb, dev, NULL,
							&fidx);
			if (err == -EMSGSIZE)
				goto out;

			cops = NULL;

			/* reset fdb offset to 0 for rest of the interfaces */
			cb->args[2] = 0;
			fidx = 0;
cont:
			idx++;
		}
	}

out:
	cb->args[0] = h;
	cb->args[1] = idx;
	cb->args[2] = fidx;

	return skb->len;
}

static int valid_fdb_get_strict(const struct nlmsghdr *nlh,
				struct nlattr **tb, u8 *ndm_flags,
				int *br_idx, int *brport_idx, u8 **addr,
				u16 *vid, struct netlink_ext_ack *extack)
{
	struct ndmsg *ndm;
	int err, i;

	if (nlh->nlmsg_len < nlmsg_msg_size(sizeof(*ndm))) {
		NL_SET_ERR_MSG(extack, "Invalid header for fdb get request");
		return -EINVAL;
	}

	ndm = nlmsg_data(nlh);
	if (ndm->ndm_pad1  || ndm->ndm_pad2  || ndm->ndm_state ||
	    ndm->ndm_type) {
		NL_SET_ERR_MSG(extack, "Invalid values in header for fdb get request");
		return -EINVAL;
	}

	if (ndm->ndm_flags & ~(NTF_MASTER | NTF_SELF)) {
		NL_SET_ERR_MSG(extack, "Invalid flags in header for fdb get request");
		return -EINVAL;
	}

	err = nlmsg_parse_deprecated_strict(nlh, sizeof(struct ndmsg), tb,
					    NDA_MAX, nda_policy, extack);
	if (err < 0)
		return err;

	*ndm_flags = ndm->ndm_flags;
	*brport_idx = ndm->ndm_ifindex;
	for (i = 0; i <= NDA_MAX; ++i) {
		if (!tb[i])
			continue;

		switch (i) {
		case NDA_MASTER:
			*br_idx = nla_get_u32(tb[i]);
			break;
		case NDA_LLADDR:
			if (nla_len(tb[i]) != ETH_ALEN) {
				NL_SET_ERR_MSG(extack, "Invalid address in fdb get request");
				return -EINVAL;
			}
			*addr = nla_data(tb[i]);
			break;
		case NDA_VLAN:
			err = fdb_vid_parse(tb[i], vid, extack);
			if (err)
				return err;
			break;
		case NDA_VNI:
			break;
		default:
			NL_SET_ERR_MSG(extack, "Unsupported attribute in fdb get request");
			return -EINVAL;
		}
	}

	return 0;
}

static int rtnl_fdb_get(struct sk_buff *in_skb, struct nlmsghdr *nlh,
			struct netlink_ext_ack *extack)
{
	struct net_device *dev = NULL, *br_dev = NULL;
	const struct net_device_ops *ops = NULL;
	struct net *net = sock_net(in_skb->sk);
	struct nlattr *tb[NDA_MAX + 1];
	struct sk_buff *skb;
	int brport_idx = 0;
	u8 ndm_flags = 0;
	int br_idx = 0;
	u8 *addr = NULL;
	u16 vid = 0;
	int err;

	err = valid_fdb_get_strict(nlh, tb, &ndm_flags, &br_idx,
				   &brport_idx, &addr, &vid, extack);
	if (err < 0)
		return err;

	if (!addr) {
		NL_SET_ERR_MSG(extack, "Missing lookup address for fdb get request");
		return -EINVAL;
	}

	if (brport_idx) {
		dev = __dev_get_by_index(net, brport_idx);
		if (!dev) {
			NL_SET_ERR_MSG(extack, "Unknown device ifindex");
			return -ENODEV;
		}
	}

	if (br_idx) {
		if (dev) {
			NL_SET_ERR_MSG(extack, "Master and device are mutually exclusive");
			return -EINVAL;
		}

		br_dev = __dev_get_by_index(net, br_idx);
		if (!br_dev) {
			NL_SET_ERR_MSG(extack, "Invalid master ifindex");
			return -EINVAL;
		}
		ops = br_dev->netdev_ops;
	}

	if (dev) {
		if (!ndm_flags || (ndm_flags & NTF_MASTER)) {
			if (!netif_is_bridge_port(dev)) {
				NL_SET_ERR_MSG(extack, "Device is not a bridge port");
				return -EINVAL;
			}
			br_dev = netdev_master_upper_dev_get(dev);
			if (!br_dev) {
				NL_SET_ERR_MSG(extack, "Master of device not found");
				return -EINVAL;
			}
			ops = br_dev->netdev_ops;
		} else {
			if (!(ndm_flags & NTF_SELF)) {
				NL_SET_ERR_MSG(extack, "Missing NTF_SELF");
				return -EINVAL;
			}
			ops = dev->netdev_ops;
		}
	}

	if (!br_dev && !dev) {
		NL_SET_ERR_MSG(extack, "No device specified");
		return -ENODEV;
	}

	if (!ops || !ops->ndo_fdb_get) {
		NL_SET_ERR_MSG(extack, "Fdb get operation not supported by device");
		return -EOPNOTSUPP;
	}

	skb = nlmsg_new(NLMSG_GOODSIZE, GFP_KERNEL);
	if (!skb)
		return -ENOBUFS;

	if (br_dev)
		dev = br_dev;
	err = ops->ndo_fdb_get(skb, tb, dev, addr, vid,
			       NETLINK_CB(in_skb).portid,
			       nlh->nlmsg_seq, extack);
	if (err)
		goto out;

	return rtnl_unicast(skb, net, NETLINK_CB(in_skb).portid);
out:
	kfree_skb(skb);
	return err;
}

static int brport_nla_put_flag(struct sk_buff *skb, u32 flags, u32 mask,
			       unsigned int attrnum, unsigned int flag)
{
	if (mask & flag)
		return nla_put_u8(skb, attrnum, !!(flags & flag));
	return 0;
}

int ndo_dflt_bridge_getlink(struct sk_buff *skb, u32 pid, u32 seq,
			    struct net_device *dev, u16 mode,
			    u32 flags, u32 mask, int nlflags,
			    u32 filter_mask,
			    int (*vlan_fill)(struct sk_buff *skb,
					     struct net_device *dev,
					     u32 filter_mask))
{
	struct nlmsghdr *nlh;
	struct ifinfomsg *ifm;
	struct nlattr *br_afspec;
	struct nlattr *protinfo;
	u8 operstate = netif_running(dev) ? dev->operstate : IF_OPER_DOWN;
	struct net_device *br_dev = netdev_master_upper_dev_get(dev);
	int err = 0;

	nlh = nlmsg_put(skb, pid, seq, RTM_NEWLINK, sizeof(*ifm), nlflags);
	if (nlh == NULL)
		return -EMSGSIZE;

	ifm = nlmsg_data(nlh);
	ifm->ifi_family = AF_BRIDGE;
	ifm->__ifi_pad = 0;
	ifm->ifi_type = dev->type;
	ifm->ifi_index = dev->ifindex;
	ifm->ifi_flags = dev_get_flags(dev);
	ifm->ifi_change = 0;


	if (nla_put_string(skb, IFLA_IFNAME, dev->name) ||
	    nla_put_u32(skb, IFLA_MTU, dev->mtu) ||
	    nla_put_u8(skb, IFLA_OPERSTATE, operstate) ||
	    (br_dev &&
	     nla_put_u32(skb, IFLA_MASTER, br_dev->ifindex)) ||
	    (dev->addr_len &&
	     nla_put(skb, IFLA_ADDRESS, dev->addr_len, dev->dev_addr)) ||
	    (dev->ifindex != dev_get_iflink(dev) &&
	     nla_put_u32(skb, IFLA_LINK, dev_get_iflink(dev))))
		goto nla_put_failure;

	br_afspec = nla_nest_start_noflag(skb, IFLA_AF_SPEC);
	if (!br_afspec)
		goto nla_put_failure;

	if (nla_put_u16(skb, IFLA_BRIDGE_FLAGS, BRIDGE_FLAGS_SELF)) {
		nla_nest_cancel(skb, br_afspec);
		goto nla_put_failure;
	}

	if (mode != BRIDGE_MODE_UNDEF) {
		if (nla_put_u16(skb, IFLA_BRIDGE_MODE, mode)) {
			nla_nest_cancel(skb, br_afspec);
			goto nla_put_failure;
		}
	}
	if (vlan_fill) {
		err = vlan_fill(skb, dev, filter_mask);
		if (err) {
			nla_nest_cancel(skb, br_afspec);
			goto nla_put_failure;
		}
	}
	nla_nest_end(skb, br_afspec);

	protinfo = nla_nest_start(skb, IFLA_PROTINFO);
	if (!protinfo)
		goto nla_put_failure;

	if (brport_nla_put_flag(skb, flags, mask,
				IFLA_BRPORT_MODE, BR_HAIRPIN_MODE) ||
	    brport_nla_put_flag(skb, flags, mask,
				IFLA_BRPORT_GUARD, BR_BPDU_GUARD) ||
	    brport_nla_put_flag(skb, flags, mask,
				IFLA_BRPORT_FAST_LEAVE,
				BR_MULTICAST_FAST_LEAVE) ||
	    brport_nla_put_flag(skb, flags, mask,
				IFLA_BRPORT_PROTECT, BR_ROOT_BLOCK) ||
	    brport_nla_put_flag(skb, flags, mask,
				IFLA_BRPORT_LEARNING, BR_LEARNING) ||
	    brport_nla_put_flag(skb, flags, mask,
				IFLA_BRPORT_LEARNING_SYNC, BR_LEARNING_SYNC) ||
	    brport_nla_put_flag(skb, flags, mask,
				IFLA_BRPORT_UNICAST_FLOOD, BR_FLOOD) ||
	    brport_nla_put_flag(skb, flags, mask,
				IFLA_BRPORT_PROXYARP, BR_PROXYARP) ||
	    brport_nla_put_flag(skb, flags, mask,
				IFLA_BRPORT_MCAST_FLOOD, BR_MCAST_FLOOD) ||
	    brport_nla_put_flag(skb, flags, mask,
				IFLA_BRPORT_BCAST_FLOOD, BR_BCAST_FLOOD)) {
		nla_nest_cancel(skb, protinfo);
		goto nla_put_failure;
	}

	nla_nest_end(skb, protinfo);

	nlmsg_end(skb, nlh);
	return 0;
nla_put_failure:
	nlmsg_cancel(skb, nlh);
	return err ? err : -EMSGSIZE;
}
EXPORT_SYMBOL_GPL(ndo_dflt_bridge_getlink);

static int valid_bridge_getlink_req(const struct nlmsghdr *nlh,
				    bool strict_check, u32 *filter_mask,
				    struct netlink_ext_ack *extack)
{
	struct nlattr *tb[IFLA_MAX+1];
	int err, i;

	if (strict_check) {
		struct ifinfomsg *ifm;

		if (nlh->nlmsg_len < nlmsg_msg_size(sizeof(*ifm))) {
			NL_SET_ERR_MSG(extack, "Invalid header for bridge link dump");
			return -EINVAL;
		}

		ifm = nlmsg_data(nlh);
		if (ifm->__ifi_pad || ifm->ifi_type || ifm->ifi_flags ||
		    ifm->ifi_change || ifm->ifi_index) {
			NL_SET_ERR_MSG(extack, "Invalid values in header for bridge link dump request");
			return -EINVAL;
		}

		err = nlmsg_parse_deprecated_strict(nlh,
						    sizeof(struct ifinfomsg),
						    tb, IFLA_MAX, ifla_policy,
						    extack);
	} else {
		err = nlmsg_parse_deprecated(nlh, sizeof(struct ifinfomsg),
					     tb, IFLA_MAX, ifla_policy,
					     extack);
	}
	if (err < 0)
		return err;

	/* new attributes should only be added with strict checking */
	for (i = 0; i <= IFLA_MAX; ++i) {
		if (!tb[i])
			continue;

		switch (i) {
		case IFLA_EXT_MASK:
			*filter_mask = nla_get_u32(tb[i]);
			break;
		default:
			if (strict_check) {
				NL_SET_ERR_MSG(extack, "Unsupported attribute in bridge link dump request");
				return -EINVAL;
			}
		}
	}

	return 0;
}

static int rtnl_bridge_getlink(struct sk_buff *skb, struct netlink_callback *cb)
{
	const struct nlmsghdr *nlh = cb->nlh;
	struct net *net = sock_net(skb->sk);
	struct net_device *dev;
	int idx = 0;
	u32 portid = NETLINK_CB(cb->skb).portid;
	u32 seq = nlh->nlmsg_seq;
	u32 filter_mask = 0;
	int err;

	err = valid_bridge_getlink_req(nlh, cb->strict_check, &filter_mask,
				       cb->extack);
	if (err < 0 && cb->strict_check)
		return err;

	rcu_read_lock();
	for_each_netdev_rcu(net, dev) {
		const struct net_device_ops *ops = dev->netdev_ops;
		struct net_device *br_dev = netdev_master_upper_dev_get(dev);

		if (br_dev && br_dev->netdev_ops->ndo_bridge_getlink) {
			if (idx >= cb->args[0]) {
				err = br_dev->netdev_ops->ndo_bridge_getlink(
						skb, portid, seq, dev,
						filter_mask, NLM_F_MULTI);
				if (err < 0 && err != -EOPNOTSUPP) {
					if (likely(skb->len))
						break;

					goto out_err;
				}
			}
			idx++;
		}

		if (ops->ndo_bridge_getlink) {
			if (idx >= cb->args[0]) {
				err = ops->ndo_bridge_getlink(skb, portid,
							      seq, dev,
							      filter_mask,
							      NLM_F_MULTI);
				if (err < 0 && err != -EOPNOTSUPP) {
					if (likely(skb->len))
						break;

					goto out_err;
				}
			}
			idx++;
		}
	}
	err = skb->len;
out_err:
	rcu_read_unlock();
	cb->args[0] = idx;

	return err;
}

static inline size_t bridge_nlmsg_size(void)
{
	return NLMSG_ALIGN(sizeof(struct ifinfomsg))
		+ nla_total_size(IFNAMSIZ)	/* IFLA_IFNAME */
		+ nla_total_size(MAX_ADDR_LEN)	/* IFLA_ADDRESS */
		+ nla_total_size(sizeof(u32))	/* IFLA_MASTER */
		+ nla_total_size(sizeof(u32))	/* IFLA_MTU */
		+ nla_total_size(sizeof(u32))	/* IFLA_LINK */
		+ nla_total_size(sizeof(u32))	/* IFLA_OPERSTATE */
		+ nla_total_size(sizeof(u8))	/* IFLA_PROTINFO */
		+ nla_total_size(sizeof(struct nlattr))	/* IFLA_AF_SPEC */
		+ nla_total_size(sizeof(u16))	/* IFLA_BRIDGE_FLAGS */
		+ nla_total_size(sizeof(u16));	/* IFLA_BRIDGE_MODE */
}

static int rtnl_bridge_notify(struct net_device *dev)
{
	struct net *net = dev_net(dev);
	struct sk_buff *skb;
	int err = -EOPNOTSUPP;

	if (!dev->netdev_ops->ndo_bridge_getlink)
		return 0;

	skb = nlmsg_new(bridge_nlmsg_size(), GFP_ATOMIC);
	if (!skb) {
		err = -ENOMEM;
		goto errout;
	}

	err = dev->netdev_ops->ndo_bridge_getlink(skb, 0, 0, dev, 0, 0);
	if (err < 0)
		goto errout;

	/* Notification info is only filled for bridge ports, not the bridge
	 * device itself. Therefore, a zero notification length is valid and
	 * should not result in an error.
	 */
	if (!skb->len)
		goto errout;

	rtnl_notify(skb, net, 0, RTNLGRP_LINK, NULL, GFP_ATOMIC);
	return 0;
errout:
	WARN_ON(err == -EMSGSIZE);
	kfree_skb(skb);
	if (err)
		rtnl_set_sk_err(net, RTNLGRP_LINK, err);
	return err;
}

static int rtnl_bridge_setlink(struct sk_buff *skb, struct nlmsghdr *nlh,
			       struct netlink_ext_ack *extack)
{
	struct net *net = sock_net(skb->sk);
	struct ifinfomsg *ifm;
	struct net_device *dev;
	struct nlattr *br_spec, *attr, *br_flags_attr = NULL;
	int rem, err = -EOPNOTSUPP;
	u16 flags = 0;

	if (nlmsg_len(nlh) < sizeof(*ifm))
		return -EINVAL;

	ifm = nlmsg_data(nlh);
	if (ifm->ifi_family != AF_BRIDGE)
		return -EPFNOSUPPORT;

	dev = __dev_get_by_index(net, ifm->ifi_index);
	if (!dev) {
		NL_SET_ERR_MSG(extack, "unknown ifindex");
		return -ENODEV;
	}

	br_spec = nlmsg_find_attr(nlh, sizeof(struct ifinfomsg), IFLA_AF_SPEC);
	if (br_spec) {
		nla_for_each_nested(attr, br_spec, rem) {
			if (nla_type(attr) == IFLA_BRIDGE_FLAGS && !br_flags_attr) {
				if (nla_len(attr) < sizeof(flags))
					return -EINVAL;

				br_flags_attr = attr;
				flags = nla_get_u16(attr);
			}

			if (nla_type(attr) == IFLA_BRIDGE_MODE) {
				if (nla_len(attr) < sizeof(u16))
					return -EINVAL;
			}
		}
	}

	if (!flags || (flags & BRIDGE_FLAGS_MASTER)) {
		struct net_device *br_dev = netdev_master_upper_dev_get(dev);

		if (!br_dev || !br_dev->netdev_ops->ndo_bridge_setlink) {
			err = -EOPNOTSUPP;
			goto out;
		}

		err = br_dev->netdev_ops->ndo_bridge_setlink(dev, nlh, flags,
							     extack);
		if (err)
			goto out;

		flags &= ~BRIDGE_FLAGS_MASTER;
	}

	if ((flags & BRIDGE_FLAGS_SELF)) {
		if (!dev->netdev_ops->ndo_bridge_setlink)
			err = -EOPNOTSUPP;
		else
			err = dev->netdev_ops->ndo_bridge_setlink(dev, nlh,
								  flags,
								  extack);
		if (!err) {
			flags &= ~BRIDGE_FLAGS_SELF;

			/* Generate event to notify upper layer of bridge
			 * change
			 */
			err = rtnl_bridge_notify(dev);
		}
	}

	if (br_flags_attr)
		memcpy(nla_data(br_flags_attr), &flags, sizeof(flags));
out:
	return err;
}

static int rtnl_bridge_dellink(struct sk_buff *skb, struct nlmsghdr *nlh,
			       struct netlink_ext_ack *extack)
{
	struct net *net = sock_net(skb->sk);
	struct ifinfomsg *ifm;
	struct net_device *dev;
	struct nlattr *br_spec, *attr = NULL;
	int rem, err = -EOPNOTSUPP;
	u16 flags = 0;
	bool have_flags = false;

	if (nlmsg_len(nlh) < sizeof(*ifm))
		return -EINVAL;

	ifm = nlmsg_data(nlh);
	if (ifm->ifi_family != AF_BRIDGE)
		return -EPFNOSUPPORT;

	dev = __dev_get_by_index(net, ifm->ifi_index);
	if (!dev) {
		NL_SET_ERR_MSG(extack, "unknown ifindex");
		return -ENODEV;
	}

	br_spec = nlmsg_find_attr(nlh, sizeof(struct ifinfomsg), IFLA_AF_SPEC);
	if (br_spec) {
		nla_for_each_nested(attr, br_spec, rem) {
			if (nla_type(attr) == IFLA_BRIDGE_FLAGS) {
				if (nla_len(attr) < sizeof(flags))
					return -EINVAL;

				have_flags = true;
				flags = nla_get_u16(attr);
				break;
			}
		}
	}

	if (!flags || (flags & BRIDGE_FLAGS_MASTER)) {
		struct net_device *br_dev = netdev_master_upper_dev_get(dev);

		if (!br_dev || !br_dev->netdev_ops->ndo_bridge_dellink) {
			err = -EOPNOTSUPP;
			goto out;
		}

		err = br_dev->netdev_ops->ndo_bridge_dellink(dev, nlh, flags);
		if (err)
			goto out;

		flags &= ~BRIDGE_FLAGS_MASTER;
	}

	if ((flags & BRIDGE_FLAGS_SELF)) {
		if (!dev->netdev_ops->ndo_bridge_dellink)
			err = -EOPNOTSUPP;
		else
			err = dev->netdev_ops->ndo_bridge_dellink(dev, nlh,
								  flags);

		if (!err) {
			flags &= ~BRIDGE_FLAGS_SELF;

			/* Generate event to notify upper layer of bridge
			 * change
			 */
			err = rtnl_bridge_notify(dev);
		}
	}

	if (have_flags)
		memcpy(nla_data(attr), &flags, sizeof(flags));
out:
	return err;
}

static bool stats_attr_valid(unsigned int mask, int attrid, int idxattr)
{
	return (mask & IFLA_STATS_FILTER_BIT(attrid)) &&
	       (!idxattr || idxattr == attrid);
}

static bool
rtnl_offload_xstats_have_ndo(const struct net_device *dev, int attr_id)
{
	return dev->netdev_ops &&
	       dev->netdev_ops->ndo_has_offload_stats &&
	       dev->netdev_ops->ndo_get_offload_stats &&
	       dev->netdev_ops->ndo_has_offload_stats(dev, attr_id);
}

static unsigned int
rtnl_offload_xstats_get_size_ndo(const struct net_device *dev, int attr_id)
{
	return rtnl_offload_xstats_have_ndo(dev, attr_id) ?
	       sizeof(struct rtnl_link_stats64) : 0;
}

static int
rtnl_offload_xstats_fill_ndo(struct net_device *dev, int attr_id,
			     struct sk_buff *skb)
{
	unsigned int size = rtnl_offload_xstats_get_size_ndo(dev, attr_id);
	struct nlattr *attr = NULL;
	void *attr_data;
	int err;

	if (!size)
		return -ENODATA;

	attr = nla_reserve_64bit(skb, attr_id, size,
				 IFLA_OFFLOAD_XSTATS_UNSPEC);
	if (!attr)
		return -EMSGSIZE;

	attr_data = nla_data(attr);
	memset(attr_data, 0, size);

	err = dev->netdev_ops->ndo_get_offload_stats(attr_id, dev, attr_data);
	if (err)
		return err;

	return 0;
}

static unsigned int
rtnl_offload_xstats_get_size_stats(const struct net_device *dev,
				   enum netdev_offload_xstats_type type)
{
	bool enabled = netdev_offload_xstats_enabled(dev, type);

	return enabled ? sizeof(struct rtnl_hw_stats64) : 0;
}

struct rtnl_offload_xstats_request_used {
	bool request;
	bool used;
};

static int
rtnl_offload_xstats_get_stats(struct net_device *dev,
			      enum netdev_offload_xstats_type type,
			      struct rtnl_offload_xstats_request_used *ru,
			      struct rtnl_hw_stats64 *stats,
			      struct netlink_ext_ack *extack)
{
	bool request;
	bool used;
	int err;

	request = netdev_offload_xstats_enabled(dev, type);
	if (!request) {
		used = false;
		goto out;
	}

	err = netdev_offload_xstats_get(dev, type, stats, &used, extack);
	if (err)
		return err;

out:
	if (ru) {
		ru->request = request;
		ru->used = used;
	}
	return 0;
}

static int
rtnl_offload_xstats_fill_hw_s_info_one(struct sk_buff *skb, int attr_id,
				       struct rtnl_offload_xstats_request_used *ru)
{
	struct nlattr *nest;

	nest = nla_nest_start(skb, attr_id);
	if (!nest)
		return -EMSGSIZE;

	if (nla_put_u8(skb, IFLA_OFFLOAD_XSTATS_HW_S_INFO_REQUEST, ru->request))
		goto nla_put_failure;

	if (nla_put_u8(skb, IFLA_OFFLOAD_XSTATS_HW_S_INFO_USED, ru->used))
		goto nla_put_failure;

	nla_nest_end(skb, nest);
	return 0;

nla_put_failure:
	nla_nest_cancel(skb, nest);
	return -EMSGSIZE;
}

static int
rtnl_offload_xstats_fill_hw_s_info(struct sk_buff *skb, struct net_device *dev,
				   struct netlink_ext_ack *extack)
{
	enum netdev_offload_xstats_type t_l3 = NETDEV_OFFLOAD_XSTATS_TYPE_L3;
	struct rtnl_offload_xstats_request_used ru_l3;
	struct nlattr *nest;
	int err;

	err = rtnl_offload_xstats_get_stats(dev, t_l3, &ru_l3, NULL, extack);
	if (err)
		return err;

	nest = nla_nest_start(skb, IFLA_OFFLOAD_XSTATS_HW_S_INFO);
	if (!nest)
		return -EMSGSIZE;

	if (rtnl_offload_xstats_fill_hw_s_info_one(skb,
						   IFLA_OFFLOAD_XSTATS_L3_STATS,
						   &ru_l3))
		goto nla_put_failure;

	nla_nest_end(skb, nest);
	return 0;

nla_put_failure:
	nla_nest_cancel(skb, nest);
	return -EMSGSIZE;
}

static int rtnl_offload_xstats_fill(struct sk_buff *skb, struct net_device *dev,
				    int *prividx, u32 off_filter_mask,
				    struct netlink_ext_ack *extack)
{
	enum netdev_offload_xstats_type t_l3 = NETDEV_OFFLOAD_XSTATS_TYPE_L3;
	int attr_id_hw_s_info = IFLA_OFFLOAD_XSTATS_HW_S_INFO;
	int attr_id_l3_stats = IFLA_OFFLOAD_XSTATS_L3_STATS;
	int attr_id_cpu_hit = IFLA_OFFLOAD_XSTATS_CPU_HIT;
	bool have_data = false;
	int err;

	if (*prividx <= attr_id_cpu_hit &&
	    (off_filter_mask &
	     IFLA_STATS_FILTER_BIT(attr_id_cpu_hit))) {
		err = rtnl_offload_xstats_fill_ndo(dev, attr_id_cpu_hit, skb);
		if (!err) {
			have_data = true;
		} else if (err != -ENODATA) {
			*prividx = attr_id_cpu_hit;
			return err;
		}
	}

	if (*prividx <= attr_id_hw_s_info &&
	    (off_filter_mask & IFLA_STATS_FILTER_BIT(attr_id_hw_s_info))) {
		*prividx = attr_id_hw_s_info;

		err = rtnl_offload_xstats_fill_hw_s_info(skb, dev, extack);
		if (err)
			return err;

		have_data = true;
		*prividx = 0;
	}

	if (*prividx <= attr_id_l3_stats &&
	    (off_filter_mask & IFLA_STATS_FILTER_BIT(attr_id_l3_stats))) {
		unsigned int size_l3;
		struct nlattr *attr;

		*prividx = attr_id_l3_stats;

		size_l3 = rtnl_offload_xstats_get_size_stats(dev, t_l3);
		if (!size_l3)
			goto skip_l3_stats;
		attr = nla_reserve_64bit(skb, attr_id_l3_stats, size_l3,
					 IFLA_OFFLOAD_XSTATS_UNSPEC);
		if (!attr)
			return -EMSGSIZE;

		err = rtnl_offload_xstats_get_stats(dev, t_l3, NULL,
						    nla_data(attr), extack);
		if (err)
			return err;

		have_data = true;
skip_l3_stats:
		*prividx = 0;
	}

	if (!have_data)
		return -ENODATA;

	*prividx = 0;
	return 0;
}

static unsigned int
rtnl_offload_xstats_get_size_hw_s_info_one(const struct net_device *dev,
					   enum netdev_offload_xstats_type type)
{
	return nla_total_size(0) +
		/* IFLA_OFFLOAD_XSTATS_HW_S_INFO_REQUEST */
		nla_total_size(sizeof(u8)) +
		/* IFLA_OFFLOAD_XSTATS_HW_S_INFO_USED */
		nla_total_size(sizeof(u8)) +
		0;
}

static unsigned int
rtnl_offload_xstats_get_size_hw_s_info(const struct net_device *dev)
{
	enum netdev_offload_xstats_type t_l3 = NETDEV_OFFLOAD_XSTATS_TYPE_L3;

	return nla_total_size(0) +
		/* IFLA_OFFLOAD_XSTATS_L3_STATS */
		rtnl_offload_xstats_get_size_hw_s_info_one(dev, t_l3) +
		0;
}

static int rtnl_offload_xstats_get_size(const struct net_device *dev,
					u32 off_filter_mask)
{
	enum netdev_offload_xstats_type t_l3 = NETDEV_OFFLOAD_XSTATS_TYPE_L3;
	int attr_id_cpu_hit = IFLA_OFFLOAD_XSTATS_CPU_HIT;
	int nla_size = 0;
	int size;

	if (off_filter_mask &
	    IFLA_STATS_FILTER_BIT(attr_id_cpu_hit)) {
		size = rtnl_offload_xstats_get_size_ndo(dev, attr_id_cpu_hit);
		nla_size += nla_total_size_64bit(size);
	}

	if (off_filter_mask &
	    IFLA_STATS_FILTER_BIT(IFLA_OFFLOAD_XSTATS_HW_S_INFO))
		nla_size += rtnl_offload_xstats_get_size_hw_s_info(dev);

	if (off_filter_mask &
	    IFLA_STATS_FILTER_BIT(IFLA_OFFLOAD_XSTATS_L3_STATS)) {
		size = rtnl_offload_xstats_get_size_stats(dev, t_l3);
		nla_size += nla_total_size_64bit(size);
	}

	if (nla_size != 0)
		nla_size += nla_total_size(0);

	return nla_size;
}

struct rtnl_stats_dump_filters {
	/* mask[0] filters outer attributes. Then individual nests have their
	 * filtering mask at the index of the nested attribute.
	 */
	u32 mask[IFLA_STATS_MAX + 1];
};

static int rtnl_fill_statsinfo(struct sk_buff *skb, struct net_device *dev,
			       int type, u32 pid, u32 seq, u32 change,
			       unsigned int flags,
			       const struct rtnl_stats_dump_filters *filters,
			       int *idxattr, int *prividx,
			       struct netlink_ext_ack *extack)
{
	unsigned int filter_mask = filters->mask[0];
	struct if_stats_msg *ifsm;
	struct nlmsghdr *nlh;
	struct nlattr *attr;
	int s_prividx = *prividx;
	int err;

	ASSERT_RTNL();

	nlh = nlmsg_put(skb, pid, seq, type, sizeof(*ifsm), flags);
	if (!nlh)
		return -EMSGSIZE;

	ifsm = nlmsg_data(nlh);
	ifsm->family = PF_UNSPEC;
	ifsm->pad1 = 0;
	ifsm->pad2 = 0;
	ifsm->ifindex = dev->ifindex;
	ifsm->filter_mask = filter_mask;

	if (stats_attr_valid(filter_mask, IFLA_STATS_LINK_64, *idxattr)) {
		struct rtnl_link_stats64 *sp;

		attr = nla_reserve_64bit(skb, IFLA_STATS_LINK_64,
					 sizeof(struct rtnl_link_stats64),
					 IFLA_STATS_UNSPEC);
		if (!attr) {
			err = -EMSGSIZE;
			goto nla_put_failure;
		}

		sp = nla_data(attr);
		dev_get_stats(dev, sp);
	}

	if (stats_attr_valid(filter_mask, IFLA_STATS_LINK_XSTATS, *idxattr)) {
		const struct rtnl_link_ops *ops = dev->rtnl_link_ops;

		if (ops && ops->fill_linkxstats) {
			*idxattr = IFLA_STATS_LINK_XSTATS;
			attr = nla_nest_start_noflag(skb,
						     IFLA_STATS_LINK_XSTATS);
			if (!attr) {
				err = -EMSGSIZE;
				goto nla_put_failure;
			}

			err = ops->fill_linkxstats(skb, dev, prividx, *idxattr);
			nla_nest_end(skb, attr);
			if (err)
				goto nla_put_failure;
			*idxattr = 0;
		}
	}

	if (stats_attr_valid(filter_mask, IFLA_STATS_LINK_XSTATS_SLAVE,
			     *idxattr)) {
		const struct rtnl_link_ops *ops = NULL;
		const struct net_device *master;

		master = netdev_master_upper_dev_get(dev);
		if (master)
			ops = master->rtnl_link_ops;
		if (ops && ops->fill_linkxstats) {
			*idxattr = IFLA_STATS_LINK_XSTATS_SLAVE;
			attr = nla_nest_start_noflag(skb,
						     IFLA_STATS_LINK_XSTATS_SLAVE);
			if (!attr) {
				err = -EMSGSIZE;
				goto nla_put_failure;
			}

			err = ops->fill_linkxstats(skb, dev, prividx, *idxattr);
			nla_nest_end(skb, attr);
			if (err)
				goto nla_put_failure;
			*idxattr = 0;
		}
	}

	if (stats_attr_valid(filter_mask, IFLA_STATS_LINK_OFFLOAD_XSTATS,
			     *idxattr)) {
		u32 off_filter_mask;

		off_filter_mask = filters->mask[IFLA_STATS_LINK_OFFLOAD_XSTATS];
		*idxattr = IFLA_STATS_LINK_OFFLOAD_XSTATS;
		attr = nla_nest_start_noflag(skb,
					     IFLA_STATS_LINK_OFFLOAD_XSTATS);
		if (!attr) {
			err = -EMSGSIZE;
			goto nla_put_failure;
		}

		err = rtnl_offload_xstats_fill(skb, dev, prividx,
					       off_filter_mask, extack);
		if (err == -ENODATA)
			nla_nest_cancel(skb, attr);
		else
			nla_nest_end(skb, attr);

		if (err && err != -ENODATA)
			goto nla_put_failure;
		*idxattr = 0;
	}

	if (stats_attr_valid(filter_mask, IFLA_STATS_AF_SPEC, *idxattr)) {
		struct rtnl_af_ops *af_ops;

		*idxattr = IFLA_STATS_AF_SPEC;
		attr = nla_nest_start_noflag(skb, IFLA_STATS_AF_SPEC);
		if (!attr) {
			err = -EMSGSIZE;
			goto nla_put_failure;
		}

		rcu_read_lock();
		list_for_each_entry_rcu(af_ops, &rtnl_af_ops, list) {
			if (af_ops->fill_stats_af) {
				struct nlattr *af;

				af = nla_nest_start_noflag(skb,
							   af_ops->family);
				if (!af) {
					rcu_read_unlock();
					err = -EMSGSIZE;
					goto nla_put_failure;
				}
				err = af_ops->fill_stats_af(skb, dev);

				if (err == -ENODATA) {
					nla_nest_cancel(skb, af);
				} else if (err < 0) {
					rcu_read_unlock();
					goto nla_put_failure;
				}

				nla_nest_end(skb, af);
			}
		}
		rcu_read_unlock();

		nla_nest_end(skb, attr);

		*idxattr = 0;
	}

	nlmsg_end(skb, nlh);

	return 0;

nla_put_failure:
	/* not a multi message or no progress mean a real error */
	if (!(flags & NLM_F_MULTI) || s_prividx == *prividx)
		nlmsg_cancel(skb, nlh);
	else
		nlmsg_end(skb, nlh);

	return err;
}

static size_t if_nlmsg_stats_size(const struct net_device *dev,
				  const struct rtnl_stats_dump_filters *filters)
{
	size_t size = NLMSG_ALIGN(sizeof(struct if_stats_msg));
	unsigned int filter_mask = filters->mask[0];

	if (stats_attr_valid(filter_mask, IFLA_STATS_LINK_64, 0))
		size += nla_total_size_64bit(sizeof(struct rtnl_link_stats64));

	if (stats_attr_valid(filter_mask, IFLA_STATS_LINK_XSTATS, 0)) {
		const struct rtnl_link_ops *ops = dev->rtnl_link_ops;
		int attr = IFLA_STATS_LINK_XSTATS;

		if (ops && ops->get_linkxstats_size) {
			size += nla_total_size(ops->get_linkxstats_size(dev,
									attr));
			/* for IFLA_STATS_LINK_XSTATS */
			size += nla_total_size(0);
		}
	}

	if (stats_attr_valid(filter_mask, IFLA_STATS_LINK_XSTATS_SLAVE, 0)) {
		struct net_device *_dev = (struct net_device *)dev;
		const struct rtnl_link_ops *ops = NULL;
		const struct net_device *master;

		/* netdev_master_upper_dev_get can't take const */
		master = netdev_master_upper_dev_get(_dev);
		if (master)
			ops = master->rtnl_link_ops;
		if (ops && ops->get_linkxstats_size) {
			int attr = IFLA_STATS_LINK_XSTATS_SLAVE;

			size += nla_total_size(ops->get_linkxstats_size(dev,
									attr));
			/* for IFLA_STATS_LINK_XSTATS_SLAVE */
			size += nla_total_size(0);
		}
	}

	if (stats_attr_valid(filter_mask, IFLA_STATS_LINK_OFFLOAD_XSTATS, 0)) {
		u32 off_filter_mask;

		off_filter_mask = filters->mask[IFLA_STATS_LINK_OFFLOAD_XSTATS];
		size += rtnl_offload_xstats_get_size(dev, off_filter_mask);
	}

	if (stats_attr_valid(filter_mask, IFLA_STATS_AF_SPEC, 0)) {
		struct rtnl_af_ops *af_ops;

		/* for IFLA_STATS_AF_SPEC */
		size += nla_total_size(0);

		rcu_read_lock();
		list_for_each_entry_rcu(af_ops, &rtnl_af_ops, list) {
			if (af_ops->get_stats_af_size) {
				size += nla_total_size(
					af_ops->get_stats_af_size(dev));

				/* for AF_* */
				size += nla_total_size(0);
			}
		}
		rcu_read_unlock();
	}

	return size;
}

#define RTNL_STATS_OFFLOAD_XSTATS_VALID ((1 << __IFLA_OFFLOAD_XSTATS_MAX) - 1)

static const struct nla_policy
rtnl_stats_get_policy_filters[IFLA_STATS_MAX + 1] = {
	[IFLA_STATS_LINK_OFFLOAD_XSTATS] =
		    NLA_POLICY_MASK(NLA_U32, RTNL_STATS_OFFLOAD_XSTATS_VALID),
};

static const struct nla_policy
rtnl_stats_get_policy[IFLA_STATS_GETSET_MAX + 1] = {
	[IFLA_STATS_GET_FILTERS] =
		    NLA_POLICY_NESTED(rtnl_stats_get_policy_filters),
};

static const struct nla_policy
ifla_stats_set_policy[IFLA_STATS_GETSET_MAX + 1] = {
	[IFLA_STATS_SET_OFFLOAD_XSTATS_L3_STATS] = NLA_POLICY_MAX(NLA_U8, 1),
};

static int rtnl_stats_get_parse_filters(struct nlattr *ifla_filters,
					struct rtnl_stats_dump_filters *filters,
					struct netlink_ext_ack *extack)
{
	struct nlattr *tb[IFLA_STATS_MAX + 1];
	int err;
	int at;

	err = nla_parse_nested(tb, IFLA_STATS_MAX, ifla_filters,
			       rtnl_stats_get_policy_filters, extack);
	if (err < 0)
		return err;

	for (at = 1; at <= IFLA_STATS_MAX; at++) {
		if (tb[at]) {
			if (!(filters->mask[0] & IFLA_STATS_FILTER_BIT(at))) {
				NL_SET_ERR_MSG(extack, "Filtered attribute not enabled in filter_mask");
				return -EINVAL;
			}
			filters->mask[at] = nla_get_u32(tb[at]);
		}
	}

	return 0;
}

static int rtnl_stats_get_parse(const struct nlmsghdr *nlh,
				u32 filter_mask,
				struct rtnl_stats_dump_filters *filters,
				struct netlink_ext_ack *extack)
{
	struct nlattr *tb[IFLA_STATS_GETSET_MAX + 1];
	int err;
	int i;

	filters->mask[0] = filter_mask;
	for (i = 1; i < ARRAY_SIZE(filters->mask); i++)
		filters->mask[i] = -1U;

	err = nlmsg_parse(nlh, sizeof(struct if_stats_msg), tb,
			  IFLA_STATS_GETSET_MAX, rtnl_stats_get_policy, extack);
	if (err < 0)
		return err;

	if (tb[IFLA_STATS_GET_FILTERS]) {
		err = rtnl_stats_get_parse_filters(tb[IFLA_STATS_GET_FILTERS],
						   filters, extack);
		if (err)
			return err;
	}

	return 0;
}

static int rtnl_valid_stats_req(const struct nlmsghdr *nlh, bool strict_check,
				bool is_dump, struct netlink_ext_ack *extack)
{
	struct if_stats_msg *ifsm;

	if (nlh->nlmsg_len < nlmsg_msg_size(sizeof(*ifsm))) {
		NL_SET_ERR_MSG(extack, "Invalid header for stats dump");
		return -EINVAL;
	}

	if (!strict_check)
		return 0;

	ifsm = nlmsg_data(nlh);

	/* only requests using strict checks can pass data to influence
	 * the dump. The legacy exception is filter_mask.
	 */
	if (ifsm->pad1 || ifsm->pad2 || (is_dump && ifsm->ifindex)) {
		NL_SET_ERR_MSG(extack, "Invalid values in header for stats dump request");
		return -EINVAL;
	}
	if (ifsm->filter_mask >= IFLA_STATS_FILTER_BIT(IFLA_STATS_MAX + 1)) {
		NL_SET_ERR_MSG(extack, "Invalid stats requested through filter mask");
		return -EINVAL;
	}

	return 0;
}

static int rtnl_stats_get(struct sk_buff *skb, struct nlmsghdr *nlh,
			  struct netlink_ext_ack *extack)
{
	struct rtnl_stats_dump_filters filters;
	struct net *net = sock_net(skb->sk);
	struct net_device *dev = NULL;
	int idxattr = 0, prividx = 0;
	struct if_stats_msg *ifsm;
	struct sk_buff *nskb;
	int err;

	err = rtnl_valid_stats_req(nlh, netlink_strict_get_check(skb),
				   false, extack);
	if (err)
		return err;

	ifsm = nlmsg_data(nlh);
	if (ifsm->ifindex > 0)
		dev = __dev_get_by_index(net, ifsm->ifindex);
	else
		return -EINVAL;

	if (!dev)
		return -ENODEV;

	if (!ifsm->filter_mask) {
		NL_SET_ERR_MSG(extack, "Filter mask must be set for stats get");
		return -EINVAL;
	}

	err = rtnl_stats_get_parse(nlh, ifsm->filter_mask, &filters, extack);
	if (err)
		return err;

	nskb = nlmsg_new(if_nlmsg_stats_size(dev, &filters), GFP_KERNEL);
	if (!nskb)
		return -ENOBUFS;

	err = rtnl_fill_statsinfo(nskb, dev, RTM_NEWSTATS,
				  NETLINK_CB(skb).portid, nlh->nlmsg_seq, 0,
				  0, &filters, &idxattr, &prividx, extack);
	if (err < 0) {
		/* -EMSGSIZE implies BUG in if_nlmsg_stats_size */
		WARN_ON(err == -EMSGSIZE);
		kfree_skb(nskb);
	} else {
		err = rtnl_unicast(nskb, net, NETLINK_CB(skb).portid);
	}

	return err;
}

static int rtnl_stats_dump(struct sk_buff *skb, struct netlink_callback *cb)
{
	struct netlink_ext_ack *extack = cb->extack;
	int h, s_h, err, s_idx, s_idxattr, s_prividx;
	struct rtnl_stats_dump_filters filters;
	struct net *net = sock_net(skb->sk);
	unsigned int flags = NLM_F_MULTI;
	struct if_stats_msg *ifsm;
	struct hlist_head *head;
	struct net_device *dev;
	int idx = 0;

	s_h = cb->args[0];
	s_idx = cb->args[1];
	s_idxattr = cb->args[2];
	s_prividx = cb->args[3];

	cb->seq = net->dev_base_seq;

	err = rtnl_valid_stats_req(cb->nlh, cb->strict_check, true, extack);
	if (err)
		return err;

	ifsm = nlmsg_data(cb->nlh);
	if (!ifsm->filter_mask) {
		NL_SET_ERR_MSG(extack, "Filter mask must be set for stats dump");
		return -EINVAL;
	}

	err = rtnl_stats_get_parse(cb->nlh, ifsm->filter_mask, &filters,
				   extack);
	if (err)
		return err;

	for (h = s_h; h < NETDEV_HASHENTRIES; h++, s_idx = 0) {
		idx = 0;
		head = &net->dev_index_head[h];
		hlist_for_each_entry(dev, head, index_hlist) {
			if (idx < s_idx)
				goto cont;
			err = rtnl_fill_statsinfo(skb, dev, RTM_NEWSTATS,
						  NETLINK_CB(cb->skb).portid,
						  cb->nlh->nlmsg_seq, 0,
						  flags, &filters,
						  &s_idxattr, &s_prividx,
						  extack);
			/* If we ran out of room on the first message,
			 * we're in trouble
			 */
			WARN_ON((err == -EMSGSIZE) && (skb->len == 0));

			if (err < 0)
				goto out;
			s_prividx = 0;
			s_idxattr = 0;
			nl_dump_check_consistent(cb, nlmsg_hdr(skb));
cont:
			idx++;
		}
	}
out:
	cb->args[3] = s_prividx;
	cb->args[2] = s_idxattr;
	cb->args[1] = idx;
	cb->args[0] = h;

	return skb->len;
}

void rtnl_offload_xstats_notify(struct net_device *dev)
{
	struct rtnl_stats_dump_filters response_filters = {};
	struct net *net = dev_net(dev);
	int idxattr = 0, prividx = 0;
	struct sk_buff *skb;
	int err = -ENOBUFS;

	ASSERT_RTNL();

	response_filters.mask[0] |=
		IFLA_STATS_FILTER_BIT(IFLA_STATS_LINK_OFFLOAD_XSTATS);
	response_filters.mask[IFLA_STATS_LINK_OFFLOAD_XSTATS] |=
		IFLA_STATS_FILTER_BIT(IFLA_OFFLOAD_XSTATS_HW_S_INFO);

	skb = nlmsg_new(if_nlmsg_stats_size(dev, &response_filters),
			GFP_KERNEL);
	if (!skb)
		goto errout;

	err = rtnl_fill_statsinfo(skb, dev, RTM_NEWSTATS, 0, 0, 0, 0,
				  &response_filters, &idxattr, &prividx, NULL);
	if (err < 0) {
		kfree_skb(skb);
		goto errout;
	}

	rtnl_notify(skb, net, 0, RTNLGRP_STATS, NULL, GFP_KERNEL);
	return;

errout:
	rtnl_set_sk_err(net, RTNLGRP_STATS, err);
}
EXPORT_SYMBOL(rtnl_offload_xstats_notify);

static int rtnl_stats_set(struct sk_buff *skb, struct nlmsghdr *nlh,
			  struct netlink_ext_ack *extack)
{
	enum netdev_offload_xstats_type t_l3 = NETDEV_OFFLOAD_XSTATS_TYPE_L3;
	struct rtnl_stats_dump_filters response_filters = {};
	struct nlattr *tb[IFLA_STATS_GETSET_MAX + 1];
	struct net *net = sock_net(skb->sk);
	struct net_device *dev = NULL;
	struct if_stats_msg *ifsm;
	bool notify = false;
	int err;

	err = rtnl_valid_stats_req(nlh, netlink_strict_get_check(skb),
				   false, extack);
	if (err)
		return err;

	ifsm = nlmsg_data(nlh);
	if (ifsm->family != AF_UNSPEC) {
		NL_SET_ERR_MSG(extack, "Address family should be AF_UNSPEC");
		return -EINVAL;
	}

	if (ifsm->ifindex > 0)
		dev = __dev_get_by_index(net, ifsm->ifindex);
	else
		return -EINVAL;

	if (!dev)
		return -ENODEV;

	if (ifsm->filter_mask) {
		NL_SET_ERR_MSG(extack, "Filter mask must be 0 for stats set");
		return -EINVAL;
	}

	err = nlmsg_parse(nlh, sizeof(*ifsm), tb, IFLA_STATS_GETSET_MAX,
			  ifla_stats_set_policy, extack);
	if (err < 0)
		return err;

	if (tb[IFLA_STATS_SET_OFFLOAD_XSTATS_L3_STATS]) {
		u8 req = nla_get_u8(tb[IFLA_STATS_SET_OFFLOAD_XSTATS_L3_STATS]);

		if (req)
			err = netdev_offload_xstats_enable(dev, t_l3, extack);
		else
			err = netdev_offload_xstats_disable(dev, t_l3);

		if (!err)
			notify = true;
		else if (err != -EALREADY)
			return err;

		response_filters.mask[0] |=
			IFLA_STATS_FILTER_BIT(IFLA_STATS_LINK_OFFLOAD_XSTATS);
		response_filters.mask[IFLA_STATS_LINK_OFFLOAD_XSTATS] |=
			IFLA_STATS_FILTER_BIT(IFLA_OFFLOAD_XSTATS_HW_S_INFO);
	}

	if (notify)
		rtnl_offload_xstats_notify(dev);

	return 0;
}

static int rtnl_mdb_valid_dump_req(const struct nlmsghdr *nlh,
				   struct netlink_ext_ack *extack)
{
	struct br_port_msg *bpm;

	if (nlh->nlmsg_len < nlmsg_msg_size(sizeof(*bpm))) {
		NL_SET_ERR_MSG(extack, "Invalid header for mdb dump request");
		return -EINVAL;
	}

	bpm = nlmsg_data(nlh);
	if (bpm->ifindex) {
		NL_SET_ERR_MSG(extack, "Filtering by device index is not supported for mdb dump request");
		return -EINVAL;
	}
	if (nlmsg_attrlen(nlh, sizeof(*bpm))) {
		NL_SET_ERR_MSG(extack, "Invalid data after header in mdb dump request");
		return -EINVAL;
	}

	return 0;
}

struct rtnl_mdb_dump_ctx {
	long idx;
};

static int rtnl_mdb_dump(struct sk_buff *skb, struct netlink_callback *cb)
{
	struct rtnl_mdb_dump_ctx *ctx = (void *)cb->ctx;
	struct net *net = sock_net(skb->sk);
	struct net_device *dev;
	int idx, s_idx;
	int err;

	NL_ASSERT_DUMP_CTX_FITS(struct rtnl_mdb_dump_ctx);

	if (cb->strict_check) {
		err = rtnl_mdb_valid_dump_req(cb->nlh, cb->extack);
		if (err)
			return err;
	}

	s_idx = ctx->idx;
	idx = 0;

	for_each_netdev(net, dev) {
		if (idx < s_idx)
			goto skip;
		if (!dev->netdev_ops->ndo_mdb_dump)
			goto skip;

		err = dev->netdev_ops->ndo_mdb_dump(dev, skb, cb);
		if (err == -EMSGSIZE)
			goto out;
		/* Moving on to next device, reset markers and sequence
		 * counters since they are all maintained per-device.
		 */
		memset(cb->ctx, 0, sizeof(cb->ctx));
		cb->prev_seq = 0;
		cb->seq = 0;
skip:
		idx++;
	}

out:
	ctx->idx = idx;
	return skb->len;
}

static int rtnl_validate_mdb_entry_get(const struct nlattr *attr,
				       struct netlink_ext_ack *extack)
{
	struct br_mdb_entry *entry = nla_data(attr);

	if (nla_len(attr) != sizeof(struct br_mdb_entry)) {
		NL_SET_ERR_MSG_ATTR(extack, attr, "Invalid attribute length");
		return -EINVAL;
	}

	if (entry->ifindex) {
		NL_SET_ERR_MSG(extack, "Entry ifindex cannot be specified");
		return -EINVAL;
	}

	if (entry->state) {
		NL_SET_ERR_MSG(extack, "Entry state cannot be specified");
		return -EINVAL;
	}

	if (entry->flags) {
		NL_SET_ERR_MSG(extack, "Entry flags cannot be specified");
		return -EINVAL;
	}

	if (entry->vid >= VLAN_VID_MASK) {
		NL_SET_ERR_MSG(extack, "Invalid entry VLAN id");
		return -EINVAL;
	}

	if (entry->addr.proto != htons(ETH_P_IP) &&
	    entry->addr.proto != htons(ETH_P_IPV6) &&
	    entry->addr.proto != 0) {
		NL_SET_ERR_MSG(extack, "Unknown entry protocol");
		return -EINVAL;
	}

	return 0;
}

static const struct nla_policy mdba_get_policy[MDBA_GET_ENTRY_MAX + 1] = {
	[MDBA_GET_ENTRY] = NLA_POLICY_VALIDATE_FN(NLA_BINARY,
						  rtnl_validate_mdb_entry_get,
						  sizeof(struct br_mdb_entry)),
	[MDBA_GET_ENTRY_ATTRS] = { .type = NLA_NESTED },
};

static int rtnl_mdb_get(struct sk_buff *in_skb, struct nlmsghdr *nlh,
			struct netlink_ext_ack *extack)
{
	struct nlattr *tb[MDBA_GET_ENTRY_MAX + 1];
	struct net *net = sock_net(in_skb->sk);
	struct br_port_msg *bpm;
	struct net_device *dev;
	int err;

	err = nlmsg_parse(nlh, sizeof(struct br_port_msg), tb,
			  MDBA_GET_ENTRY_MAX, mdba_get_policy, extack);
	if (err)
		return err;

	bpm = nlmsg_data(nlh);
	if (!bpm->ifindex) {
		NL_SET_ERR_MSG(extack, "Invalid ifindex");
		return -EINVAL;
	}

	dev = __dev_get_by_index(net, bpm->ifindex);
	if (!dev) {
		NL_SET_ERR_MSG(extack, "Device doesn't exist");
		return -ENODEV;
	}

	if (NL_REQ_ATTR_CHECK(extack, NULL, tb, MDBA_GET_ENTRY)) {
		NL_SET_ERR_MSG(extack, "Missing MDBA_GET_ENTRY attribute");
		return -EINVAL;
	}

	if (!dev->netdev_ops->ndo_mdb_get) {
		NL_SET_ERR_MSG(extack, "Device does not support MDB operations");
		return -EOPNOTSUPP;
	}

	return dev->netdev_ops->ndo_mdb_get(dev, tb, NETLINK_CB(in_skb).portid,
					    nlh->nlmsg_seq, extack);
}

static int rtnl_validate_mdb_entry(const struct nlattr *attr,
				   struct netlink_ext_ack *extack)
{
	struct br_mdb_entry *entry = nla_data(attr);

	if (nla_len(attr) != sizeof(struct br_mdb_entry)) {
		NL_SET_ERR_MSG_ATTR(extack, attr, "Invalid attribute length");
		return -EINVAL;
	}

	if (entry->ifindex == 0) {
		NL_SET_ERR_MSG(extack, "Zero entry ifindex is not allowed");
		return -EINVAL;
	}

	if (entry->addr.proto == htons(ETH_P_IP)) {
		if (!ipv4_is_multicast(entry->addr.u.ip4) &&
		    !ipv4_is_zeronet(entry->addr.u.ip4)) {
			NL_SET_ERR_MSG(extack, "IPv4 entry group address is not multicast or 0.0.0.0");
			return -EINVAL;
		}
		if (ipv4_is_local_multicast(entry->addr.u.ip4)) {
			NL_SET_ERR_MSG(extack, "IPv4 entry group address is local multicast");
			return -EINVAL;
		}
#if IS_ENABLED(CONFIG_IPV6)
	} else if (entry->addr.proto == htons(ETH_P_IPV6)) {
		if (ipv6_addr_is_ll_all_nodes(&entry->addr.u.ip6)) {
			NL_SET_ERR_MSG(extack, "IPv6 entry group address is link-local all nodes");
			return -EINVAL;
		}
#endif
	} else if (entry->addr.proto == 0) {
		/* L2 mdb */
		if (!is_multicast_ether_addr(entry->addr.u.mac_addr)) {
			NL_SET_ERR_MSG(extack, "L2 entry group is not multicast");
			return -EINVAL;
		}
	} else {
		NL_SET_ERR_MSG(extack, "Unknown entry protocol");
		return -EINVAL;
	}

	if (entry->state != MDB_PERMANENT && entry->state != MDB_TEMPORARY) {
		NL_SET_ERR_MSG(extack, "Unknown entry state");
		return -EINVAL;
	}
	if (entry->vid >= VLAN_VID_MASK) {
		NL_SET_ERR_MSG(extack, "Invalid entry VLAN id");
		return -EINVAL;
	}

	return 0;
}

static const struct nla_policy mdba_policy[MDBA_SET_ENTRY_MAX + 1] = {
	[MDBA_SET_ENTRY_UNSPEC] = { .strict_start_type = MDBA_SET_ENTRY_ATTRS + 1 },
	[MDBA_SET_ENTRY] = NLA_POLICY_VALIDATE_FN(NLA_BINARY,
						  rtnl_validate_mdb_entry,
						  sizeof(struct br_mdb_entry)),
	[MDBA_SET_ENTRY_ATTRS] = { .type = NLA_NESTED },
};

static int rtnl_mdb_add(struct sk_buff *skb, struct nlmsghdr *nlh,
			struct netlink_ext_ack *extack)
{
	struct nlattr *tb[MDBA_SET_ENTRY_MAX + 1];
	struct net *net = sock_net(skb->sk);
	struct br_port_msg *bpm;
	struct net_device *dev;
	int err;

	err = nlmsg_parse_deprecated(nlh, sizeof(*bpm), tb,
				     MDBA_SET_ENTRY_MAX, mdba_policy, extack);
	if (err)
		return err;

	bpm = nlmsg_data(nlh);
	if (!bpm->ifindex) {
		NL_SET_ERR_MSG(extack, "Invalid ifindex");
		return -EINVAL;
	}

	dev = __dev_get_by_index(net, bpm->ifindex);
	if (!dev) {
		NL_SET_ERR_MSG(extack, "Device doesn't exist");
		return -ENODEV;
	}

	if (NL_REQ_ATTR_CHECK(extack, NULL, tb, MDBA_SET_ENTRY)) {
		NL_SET_ERR_MSG(extack, "Missing MDBA_SET_ENTRY attribute");
		return -EINVAL;
	}

	if (!dev->netdev_ops->ndo_mdb_add) {
		NL_SET_ERR_MSG(extack, "Device does not support MDB operations");
		return -EOPNOTSUPP;
	}

	return dev->netdev_ops->ndo_mdb_add(dev, tb, nlh->nlmsg_flags, extack);
}

static int rtnl_validate_mdb_entry_del_bulk(const struct nlattr *attr,
					    struct netlink_ext_ack *extack)
{
	struct br_mdb_entry *entry = nla_data(attr);
	struct br_mdb_entry zero_entry = {};

	if (nla_len(attr) != sizeof(struct br_mdb_entry)) {
		NL_SET_ERR_MSG_ATTR(extack, attr, "Invalid attribute length");
		return -EINVAL;
	}

	if (entry->state != MDB_PERMANENT && entry->state != MDB_TEMPORARY) {
		NL_SET_ERR_MSG(extack, "Unknown entry state");
		return -EINVAL;
	}

	if (entry->flags) {
		NL_SET_ERR_MSG(extack, "Entry flags cannot be set");
		return -EINVAL;
	}

	if (entry->vid >= VLAN_N_VID - 1) {
		NL_SET_ERR_MSG(extack, "Invalid entry VLAN id");
		return -EINVAL;
	}

	if (memcmp(&entry->addr, &zero_entry.addr, sizeof(entry->addr))) {
		NL_SET_ERR_MSG(extack, "Entry address cannot be set");
		return -EINVAL;
	}

	return 0;
}

static const struct nla_policy mdba_del_bulk_policy[MDBA_SET_ENTRY_MAX + 1] = {
	[MDBA_SET_ENTRY] = NLA_POLICY_VALIDATE_FN(NLA_BINARY,
						  rtnl_validate_mdb_entry_del_bulk,
						  sizeof(struct br_mdb_entry)),
	[MDBA_SET_ENTRY_ATTRS] = { .type = NLA_NESTED },
};

static int rtnl_mdb_del(struct sk_buff *skb, struct nlmsghdr *nlh,
			struct netlink_ext_ack *extack)
{
	bool del_bulk = !!(nlh->nlmsg_flags & NLM_F_BULK);
	struct nlattr *tb[MDBA_SET_ENTRY_MAX + 1];
	struct net *net = sock_net(skb->sk);
	struct br_port_msg *bpm;
	struct net_device *dev;
	int err;

	if (!del_bulk)
		err = nlmsg_parse_deprecated(nlh, sizeof(*bpm), tb,
					     MDBA_SET_ENTRY_MAX, mdba_policy,
					     extack);
	else
		err = nlmsg_parse(nlh, sizeof(*bpm), tb, MDBA_SET_ENTRY_MAX,
				  mdba_del_bulk_policy, extack);
	if (err)
		return err;

	bpm = nlmsg_data(nlh);
	if (!bpm->ifindex) {
		NL_SET_ERR_MSG(extack, "Invalid ifindex");
		return -EINVAL;
	}

	dev = __dev_get_by_index(net, bpm->ifindex);
	if (!dev) {
		NL_SET_ERR_MSG(extack, "Device doesn't exist");
		return -ENODEV;
	}

	if (NL_REQ_ATTR_CHECK(extack, NULL, tb, MDBA_SET_ENTRY)) {
		NL_SET_ERR_MSG(extack, "Missing MDBA_SET_ENTRY attribute");
		return -EINVAL;
	}

	if (del_bulk) {
		if (!dev->netdev_ops->ndo_mdb_del_bulk) {
			NL_SET_ERR_MSG(extack, "Device does not support MDB bulk deletion");
			return -EOPNOTSUPP;
		}
		return dev->netdev_ops->ndo_mdb_del_bulk(dev, tb, extack);
	}

	if (!dev->netdev_ops->ndo_mdb_del) {
		NL_SET_ERR_MSG(extack, "Device does not support MDB operations");
		return -EOPNOTSUPP;
	}

	return dev->netdev_ops->ndo_mdb_del(dev, tb, extack);
}

/* Process one rtnetlink message. */

static int rtnetlink_rcv_msg(struct sk_buff *skb, struct nlmsghdr *nlh,
			     struct netlink_ext_ack *extack)
{
	struct net *net = sock_net(skb->sk);
	struct rtnl_link *link;
	enum rtnl_kinds kind;
	struct module *owner;
	int err = -EOPNOTSUPP;
	rtnl_doit_func doit;
	unsigned int flags;
	int family;
	int type;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	type = nlh->nlmsg_type;
	if (type > RTM_MAX)
		return -EOPNOTSUPP;

	type -= RTM_BASE;

	/* All the messages must have at least 1 byte length */
<<<<<<< HEAD
	if (nlh->nlmsg_len < NLMSG_LENGTH(sizeof(struct rtgenmsg)))
		return 0;

	family = ((struct rtgenmsg *)NLMSG_DATA(nlh))->rtgen_family;
	sz_idx = type>>2;
	kind = type&3;

	if (kind != 2 && !capable(CAP_NET_ADMIN))
		return -EPERM;

	if (kind == 2 && nlh->nlmsg_flags&NLM_F_DUMP) {
		struct sock *rtnl;
		rtnl_dumpit_func dumpit;
		rtnl_calcit_func calcit;
		u16 min_dump_alloc = 0;

		dumpit = rtnl_get_dumpit(family, type);
		if (dumpit == NULL)
			return -EOPNOTSUPP;
		calcit = rtnl_get_calcit(family, type);
		if (calcit)
			min_dump_alloc = calcit(skb, nlh);

		__rtnl_unlock();
		rtnl = net->rtnl;
		{
			struct netlink_dump_control c = {
				.dump		= dumpit,
				.min_dump_alloc	= min_dump_alloc,
			};
			err = netlink_dump_start(rtnl, skb, nlh, &c);
		}
		rtnl_lock();
		return err;
	}

	memset(rta_buf, 0, (rtattr_max * sizeof(struct rtattr *)));

	min_len = rtm_min[sz_idx];
	if (nlh->nlmsg_len < min_len)
		return -EINVAL;

	if (nlh->nlmsg_len > min_len) {
		int attrlen = nlh->nlmsg_len - NLMSG_ALIGN(min_len);
		struct rtattr *attr = (void *)nlh + NLMSG_ALIGN(min_len);

		while (RTA_OK(attr, attrlen)) {
			unsigned int flavor = attr->rta_type & NLA_TYPE_MASK;
			if (flavor) {
				if (flavor > rta_max[sz_idx])
					return -EINVAL;
				rta_buf[flavor-1] = attr;
			}
			attr = RTA_NEXT(attr, attrlen);
		}
	}

	doit = rtnl_get_doit(family, type);
	if (doit == NULL)
		return -EOPNOTSUPP;

	return doit(skb, nlh, (void *)&rta_buf[0]);
=======
	if (nlmsg_len(nlh) < sizeof(struct rtgenmsg))
		return 0;

	family = ((struct rtgenmsg *)nlmsg_data(nlh))->rtgen_family;
	kind = rtnl_msgtype_kind(type);

	if (kind != RTNL_KIND_GET && !netlink_net_capable(skb, CAP_NET_ADMIN))
		return -EPERM;

	rcu_read_lock();
	if (kind == RTNL_KIND_GET && (nlh->nlmsg_flags & NLM_F_DUMP)) {
		struct sock *rtnl;
		rtnl_dumpit_func dumpit;
		u32 min_dump_alloc = 0;

		link = rtnl_get_link(family, type);
		if (!link || !link->dumpit) {
			family = PF_UNSPEC;
			link = rtnl_get_link(family, type);
			if (!link || !link->dumpit)
				goto err_unlock;
		}
		owner = link->owner;
		dumpit = link->dumpit;
		flags = link->flags;

		if (type == RTM_GETLINK - RTM_BASE)
			min_dump_alloc = rtnl_calcit(skb, nlh);

		err = 0;
		/* need to do this before rcu_read_unlock() */
		if (!try_module_get(owner))
			err = -EPROTONOSUPPORT;

		rcu_read_unlock();

		rtnl = net->rtnl;
		if (err == 0) {
			struct netlink_dump_control c = {
				.dump		= dumpit,
				.min_dump_alloc	= min_dump_alloc,
				.module		= owner,
				.flags		= flags,
			};
			err = netlink_dump_start(rtnl, skb, nlh, &c);
			/* netlink_dump_start() will keep a reference on
			 * module if dump is still in progress.
			 */
			module_put(owner);
		}
		return err;
	}

	link = rtnl_get_link(family, type);
	if (!link || !link->doit) {
		family = PF_UNSPEC;
		link = rtnl_get_link(PF_UNSPEC, type);
		if (!link || !link->doit)
			goto out_unlock;
	}

	owner = link->owner;
	if (!try_module_get(owner)) {
		err = -EPROTONOSUPPORT;
		goto out_unlock;
	}

	flags = link->flags;
	if (kind == RTNL_KIND_DEL && (nlh->nlmsg_flags & NLM_F_BULK) &&
	    !(flags & RTNL_FLAG_BULK_DEL_SUPPORTED)) {
		NL_SET_ERR_MSG(extack, "Bulk delete is not supported");
		module_put(owner);
		goto err_unlock;
	}

	if (flags & RTNL_FLAG_DOIT_UNLOCKED) {
		doit = link->doit;
		rcu_read_unlock();
		if (doit)
			err = doit(skb, nlh, extack);
		module_put(owner);
		return err;
	}
	rcu_read_unlock();

	rtnl_lock();
	link = rtnl_get_link(family, type);
	if (link && link->doit)
		err = link->doit(skb, nlh, extack);
	rtnl_unlock();

	module_put(owner);

	return err;

out_unlock:
	rcu_read_unlock();
	return err;

err_unlock:
	rcu_read_unlock();
	return -EOPNOTSUPP;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void rtnetlink_rcv(struct sk_buff *skb)
{
<<<<<<< HEAD
	rtnl_lock();
	netlink_rcv_skb(skb, &rtnetlink_rcv_msg);
	rtnl_unlock();
=======
	netlink_rcv_skb(skb, &rtnetlink_rcv_msg);
}

static int rtnetlink_bind(struct net *net, int group)
{
	switch (group) {
	case RTNLGRP_IPV4_MROUTE_R:
	case RTNLGRP_IPV6_MROUTE_R:
		if (!ns_capable(net->user_ns, CAP_NET_ADMIN))
			return -EPERM;
		break;
	}
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int rtnetlink_event(struct notifier_block *this, unsigned long event, void *ptr)
{
<<<<<<< HEAD
	struct net_device *dev = ptr;

	switch (event) {
	case NETDEV_UP:
	case NETDEV_DOWN:
	case NETDEV_PRE_UP:
	case NETDEV_POST_INIT:
	case NETDEV_REGISTER:
	case NETDEV_CHANGE:
	case NETDEV_PRE_TYPE_CHANGE:
	case NETDEV_GOING_DOWN:
	case NETDEV_UNREGISTER:
	case NETDEV_UNREGISTER_BATCH:
	case NETDEV_RELEASE:
	case NETDEV_JOIN:
		break;
	default:
		rtmsg_ifinfo(RTM_NEWLINK, dev, 0);
=======
	struct net_device *dev = netdev_notifier_info_to_dev(ptr);

	switch (event) {
	case NETDEV_REBOOT:
	case NETDEV_CHANGEMTU:
	case NETDEV_CHANGEADDR:
	case NETDEV_CHANGENAME:
	case NETDEV_FEAT_CHANGE:
	case NETDEV_BONDING_FAILOVER:
	case NETDEV_POST_TYPE_CHANGE:
	case NETDEV_NOTIFY_PEERS:
	case NETDEV_CHANGEUPPER:
	case NETDEV_RESEND_IGMP:
	case NETDEV_CHANGEINFODATA:
	case NETDEV_CHANGELOWERSTATE:
	case NETDEV_CHANGE_TX_QUEUE_LEN:
		rtmsg_ifinfo_event(RTM_NEWLINK, dev, 0, rtnl_get_event(event),
				   GFP_KERNEL, NULL, 0, 0, NULL);
		break;
	default:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	}
	return NOTIFY_DONE;
}

static struct notifier_block rtnetlink_dev_notifier = {
	.notifier_call	= rtnetlink_event,
};


static int __net_init rtnetlink_net_init(struct net *net)
{
	struct sock *sk;
<<<<<<< HEAD
	sk = netlink_kernel_create(net, NETLINK_ROUTE, RTNLGRP_MAX,
				   rtnetlink_rcv, &rtnl_mutex, THIS_MODULE);
=======
	struct netlink_kernel_cfg cfg = {
		.groups		= RTNLGRP_MAX,
		.input		= rtnetlink_rcv,
		.cb_mutex	= &rtnl_mutex,
		.flags		= NL_CFG_F_NONROOT_RECV,
		.bind		= rtnetlink_bind,
	};

	sk = netlink_kernel_create(net, NETLINK_ROUTE, &cfg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!sk)
		return -ENOMEM;
	net->rtnl = sk;
	return 0;
}

static void __net_exit rtnetlink_net_exit(struct net *net)
{
	netlink_kernel_release(net->rtnl);
	net->rtnl = NULL;
}

static struct pernet_operations rtnetlink_net_ops = {
	.init = rtnetlink_net_init,
	.exit = rtnetlink_net_exit,
};

void __init rtnetlink_init(void)
{
<<<<<<< HEAD
	int i;

	rtattr_max = 0;
	for (i = 0; i < ARRAY_SIZE(rta_max); i++)
		if (rta_max[i] > rtattr_max)
			rtattr_max = rta_max[i];
	rta_buf = kmalloc(rtattr_max * sizeof(struct rtattr *), GFP_KERNEL);
	if (!rta_buf)
		panic("rtnetlink_init: cannot allocate rta_buf\n");

	if (register_pernet_subsys(&rtnetlink_net_ops))
		panic("rtnetlink_init: cannot initialize rtnetlink\n");

	netlink_set_nonroot(NETLINK_ROUTE, NL_NONROOT_RECV);
	register_netdevice_notifier(&rtnetlink_dev_notifier);

	rtnl_register(PF_UNSPEC, RTM_GETLINK, rtnl_getlink,
		      rtnl_dump_ifinfo, rtnl_calcit);
	rtnl_register(PF_UNSPEC, RTM_SETLINK, rtnl_setlink, NULL, NULL);
	rtnl_register(PF_UNSPEC, RTM_NEWLINK, rtnl_newlink, NULL, NULL);
	rtnl_register(PF_UNSPEC, RTM_DELLINK, rtnl_dellink, NULL, NULL);

	rtnl_register(PF_UNSPEC, RTM_GETADDR, NULL, rtnl_dump_all, NULL);
	rtnl_register(PF_UNSPEC, RTM_GETROUTE, NULL, rtnl_dump_all, NULL);
}

=======
	if (register_pernet_subsys(&rtnetlink_net_ops))
		panic("rtnetlink_init: cannot initialize rtnetlink\n");

	register_netdevice_notifier(&rtnetlink_dev_notifier);

	rtnl_register(PF_UNSPEC, RTM_GETLINK, rtnl_getlink,
		      rtnl_dump_ifinfo, 0);
	rtnl_register(PF_UNSPEC, RTM_SETLINK, rtnl_setlink, NULL, 0);
	rtnl_register(PF_UNSPEC, RTM_NEWLINK, rtnl_newlink, NULL, 0);
	rtnl_register(PF_UNSPEC, RTM_DELLINK, rtnl_dellink, NULL, 0);

	rtnl_register(PF_UNSPEC, RTM_GETADDR, NULL, rtnl_dump_all, 0);
	rtnl_register(PF_UNSPEC, RTM_GETROUTE, NULL, rtnl_dump_all, 0);
	rtnl_register(PF_UNSPEC, RTM_GETNETCONF, NULL, rtnl_dump_all, 0);

	rtnl_register(PF_UNSPEC, RTM_NEWLINKPROP, rtnl_newlinkprop, NULL, 0);
	rtnl_register(PF_UNSPEC, RTM_DELLINKPROP, rtnl_dellinkprop, NULL, 0);

	rtnl_register(PF_BRIDGE, RTM_NEWNEIGH, rtnl_fdb_add, NULL, 0);
	rtnl_register(PF_BRIDGE, RTM_DELNEIGH, rtnl_fdb_del, NULL,
		      RTNL_FLAG_BULK_DEL_SUPPORTED);
	rtnl_register(PF_BRIDGE, RTM_GETNEIGH, rtnl_fdb_get, rtnl_fdb_dump, 0);

	rtnl_register(PF_BRIDGE, RTM_GETLINK, NULL, rtnl_bridge_getlink, 0);
	rtnl_register(PF_BRIDGE, RTM_DELLINK, rtnl_bridge_dellink, NULL, 0);
	rtnl_register(PF_BRIDGE, RTM_SETLINK, rtnl_bridge_setlink, NULL, 0);

	rtnl_register(PF_UNSPEC, RTM_GETSTATS, rtnl_stats_get, rtnl_stats_dump,
		      0);
	rtnl_register(PF_UNSPEC, RTM_SETSTATS, rtnl_stats_set, NULL, 0);

	rtnl_register(PF_BRIDGE, RTM_GETMDB, rtnl_mdb_get, rtnl_mdb_dump, 0);
	rtnl_register(PF_BRIDGE, RTM_NEWMDB, rtnl_mdb_add, NULL, 0);
	rtnl_register(PF_BRIDGE, RTM_DELMDB, rtnl_mdb_del, NULL,
		      RTNL_FLAG_BULK_DEL_SUPPORTED);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
