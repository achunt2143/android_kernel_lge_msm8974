<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Linux network device link state notification
 *
 * Author:
 *     Stefan Rompf <sux@loplof.de>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/if.h>
#include <net/sock.h>
#include <net/pkt_sched.h>
#include <linux/rtnetlink.h>
#include <linux/jiffies.h>
#include <linux/spinlock.h>
#include <linux/workqueue.h>
#include <linux/bitops.h>
<<<<<<< HEAD
#include <asm/types.h>

=======
#include <linux/types.h>

#include "dev.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

enum lw_bits {
	LW_URGENT = 0,
};

static unsigned long linkwatch_flags;
static unsigned long linkwatch_nextevent;

static void linkwatch_event(struct work_struct *dummy);
static DECLARE_DELAYED_WORK(linkwatch_work, linkwatch_event);

static LIST_HEAD(lweventlist);
static DEFINE_SPINLOCK(lweventlist_lock);

<<<<<<< HEAD
static unsigned char default_operstate(const struct net_device *dev)
{
	if (!netif_carrier_ok(dev))
		return (dev->ifindex != dev->iflink ?
			IF_OPER_LOWERLAYERDOWN : IF_OPER_DOWN);
=======
static unsigned int default_operstate(const struct net_device *dev)
{
	if (netif_testing(dev))
		return IF_OPER_TESTING;

	/* Some uppers (DSA) have additional sources for being down, so
	 * first check whether lower is indeed the source of its down state.
	 */
	if (!netif_carrier_ok(dev)) {
		int iflink = dev_get_iflink(dev);
		struct net_device *peer;

		if (iflink == dev->ifindex)
			return IF_OPER_DOWN;

		peer = __dev_get_by_index(dev_net(dev), iflink);
		if (!peer)
			return IF_OPER_DOWN;

		return netif_carrier_ok(peer) ? IF_OPER_DOWN :
						IF_OPER_LOWERLAYERDOWN;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (netif_dormant(dev))
		return IF_OPER_DORMANT;

	return IF_OPER_UP;
}

<<<<<<< HEAD

static void rfc2863_policy(struct net_device *dev)
{
	unsigned char operstate = default_operstate(dev);

	if (operstate == dev->operstate)
		return;

	write_lock_bh(&dev_base_lock);

	switch(dev->link_mode) {
=======
static void rfc2863_policy(struct net_device *dev)
{
	unsigned int operstate = default_operstate(dev);

	if (operstate == READ_ONCE(dev->operstate))
		return;

	switch(dev->link_mode) {
	case IF_LINK_MODE_TESTING:
		if (operstate == IF_OPER_UP)
			operstate = IF_OPER_TESTING;
		break;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case IF_LINK_MODE_DORMANT:
		if (operstate == IF_OPER_UP)
			operstate = IF_OPER_DORMANT;
		break;
<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case IF_LINK_MODE_DEFAULT:
	default:
		break;
	}

<<<<<<< HEAD
	dev->operstate = operstate;

	write_unlock_bh(&dev_base_lock);
=======
	WRITE_ONCE(dev->operstate, operstate);
}


void linkwatch_init_dev(struct net_device *dev)
{
	/* Handle pre-registration link state changes */
	if (!netif_carrier_ok(dev) || netif_dormant(dev) ||
	    netif_testing(dev))
		rfc2863_policy(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}


static bool linkwatch_urgent_event(struct net_device *dev)
{
	if (!netif_running(dev))
		return false;

<<<<<<< HEAD
	if (dev->ifindex != dev->iflink)
=======
	if (dev->ifindex != dev_get_iflink(dev))
		return true;

	if (netif_is_lag_port(dev) || netif_is_lag_master(dev))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return true;

	return netif_carrier_ok(dev) &&	qdisc_tx_changing(dev);
}


static void linkwatch_add_event(struct net_device *dev)
{
	unsigned long flags;

	spin_lock_irqsave(&lweventlist_lock, flags);
	if (list_empty(&dev->link_watch_list)) {
		list_add_tail(&dev->link_watch_list, &lweventlist);
<<<<<<< HEAD
		dev_hold(dev);
=======
		netdev_hold(dev, &dev->linkwatch_dev_tracker, GFP_ATOMIC);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	spin_unlock_irqrestore(&lweventlist_lock, flags);
}


static void linkwatch_schedule_work(int urgent)
{
	unsigned long delay = linkwatch_nextevent - jiffies;

	if (test_bit(LW_URGENT, &linkwatch_flags))
		return;

	/* Minimise down-time: drop delay for up event. */
	if (urgent) {
		if (test_and_set_bit(LW_URGENT, &linkwatch_flags))
			return;
		delay = 0;
	}

	/* If we wrap around we'll delay it by at most HZ. */
	if (delay > HZ)
		delay = 0;

	/*
<<<<<<< HEAD
	 * This is true if we've scheduled it immeditately or if we don't
	 * need an immediate execution and it's already pending.
	 */
	if (schedule_delayed_work(&linkwatch_work, delay) == !delay)
		return;

	/* Don't bother if there is nothing urgent. */
	if (!test_bit(LW_URGENT, &linkwatch_flags))
		return;

	/* It's already running which is good enough. */
	if (!__cancel_delayed_work(&linkwatch_work))
		return;

	/* Otherwise we reschedule it again for immediate execution. */
	schedule_delayed_work(&linkwatch_work, 0);
=======
	 * If urgent, schedule immediate execution; otherwise, don't
	 * override the existing timer.
	 */
	if (test_bit(LW_URGENT, &linkwatch_flags))
		mod_delayed_work(system_wq, &linkwatch_work, 0);
	else
		schedule_delayed_work(&linkwatch_work, delay);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}


static void linkwatch_do_dev(struct net_device *dev)
{
	/*
	 * Make sure the above read is complete since it can be
	 * rewritten as soon as we clear the bit below.
	 */
<<<<<<< HEAD
	smp_mb__before_clear_bit();
=======
	smp_mb__before_atomic();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* We are about to handle this device,
	 * so new events can be accepted
	 */
	clear_bit(__LINK_STATE_LINKWATCH_PENDING, &dev->state);

	rfc2863_policy(dev);
	if (dev->flags & IFF_UP) {
		if (netif_carrier_ok(dev))
			dev_activate(dev);
		else
			dev_deactivate(dev);

		netdev_state_change(dev);
	}
<<<<<<< HEAD
	dev_put(dev);
=======
	/* Note: our callers are responsible for calling netdev_tracker_free().
	 * This is the reason we use __dev_put() instead of dev_put().
	 */
	__dev_put(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void __linkwatch_run_queue(int urgent_only)
{
<<<<<<< HEAD
	struct net_device *dev;
	LIST_HEAD(wrk);

=======
#define MAX_DO_DEV_PER_LOOP	100

	int do_dev = MAX_DO_DEV_PER_LOOP;
	/* Use a local list here since we add non-urgent
	 * events back to the global one when called with
	 * urgent_only=1.
	 */
	LIST_HEAD(wrk);

	/* Give urgent case more budget */
	if (urgent_only)
		do_dev += MAX_DO_DEV_PER_LOOP;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Limit the number of linkwatch events to one
	 * per second so that a runaway driver does not
	 * cause a storm of messages on the netlink
	 * socket.  This limit does not apply to up events
	 * while the device qdisc is down.
	 */
	if (!urgent_only)
		linkwatch_nextevent = jiffies + HZ;
	/* Limit wrap-around effect on delay. */
	else if (time_after(linkwatch_nextevent, jiffies + HZ))
		linkwatch_nextevent = jiffies;

	clear_bit(LW_URGENT, &linkwatch_flags);

	spin_lock_irq(&lweventlist_lock);
	list_splice_init(&lweventlist, &wrk);

<<<<<<< HEAD
	while (!list_empty(&wrk)) {
=======
	while (!list_empty(&wrk) && do_dev > 0) {
		struct net_device *dev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		dev = list_first_entry(&wrk, struct net_device, link_watch_list);
		list_del_init(&dev->link_watch_list);

<<<<<<< HEAD
		if (urgent_only && !linkwatch_urgent_event(dev)) {
			list_add_tail(&dev->link_watch_list, &lweventlist);
			continue;
		}
		spin_unlock_irq(&lweventlist_lock);
		linkwatch_do_dev(dev);
		spin_lock_irq(&lweventlist_lock);
	}

=======
		if (!netif_device_present(dev) ||
		    (urgent_only && !linkwatch_urgent_event(dev))) {
			list_add_tail(&dev->link_watch_list, &lweventlist);
			continue;
		}
		/* We must free netdev tracker under
		 * the spinlock protection.
		 */
		netdev_tracker_free(dev, &dev->linkwatch_dev_tracker);
		spin_unlock_irq(&lweventlist_lock);
		linkwatch_do_dev(dev);
		do_dev--;
		spin_lock_irq(&lweventlist_lock);
	}

	/* Add the remaining work back to lweventlist */
	list_splice_init(&wrk, &lweventlist);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!list_empty(&lweventlist))
		linkwatch_schedule_work(0);
	spin_unlock_irq(&lweventlist_lock);
}

<<<<<<< HEAD
void linkwatch_forget_dev(struct net_device *dev)
=======
void linkwatch_sync_dev(struct net_device *dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned long flags;
	int clean = 0;

	spin_lock_irqsave(&lweventlist_lock, flags);
	if (!list_empty(&dev->link_watch_list)) {
		list_del_init(&dev->link_watch_list);
		clean = 1;
<<<<<<< HEAD
=======
		/* We must release netdev tracker under
		 * the spinlock protection.
		 */
		netdev_tracker_free(dev, &dev->linkwatch_dev_tracker);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	spin_unlock_irqrestore(&lweventlist_lock, flags);
	if (clean)
		linkwatch_do_dev(dev);
}


/* Must be called with the rtnl semaphore held */
void linkwatch_run_queue(void)
{
	__linkwatch_run_queue(0);
}


static void linkwatch_event(struct work_struct *dummy)
{
	rtnl_lock();
	__linkwatch_run_queue(time_after(linkwatch_nextevent, jiffies));
	rtnl_unlock();
}


void linkwatch_fire_event(struct net_device *dev)
{
	bool urgent = linkwatch_urgent_event(dev);

	if (!test_and_set_bit(__LINK_STATE_LINKWATCH_PENDING, &dev->state)) {
		linkwatch_add_event(dev);
	} else if (!urgent)
		return;

	linkwatch_schedule_work(urgent);
}
EXPORT_SYMBOL(linkwatch_fire_event);
