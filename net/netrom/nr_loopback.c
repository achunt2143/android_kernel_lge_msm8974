<<<<<<< HEAD
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Copyright Tomi Manninen OH2BNS (oh2bns@sral.fi)
 */
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/socket.h>
#include <linux/timer.h>
#include <net/ax25.h>
#include <linux/skbuff.h>
#include <net/netrom.h>
#include <linux/init.h>

<<<<<<< HEAD
static void nr_loopback_timer(unsigned long);

static struct sk_buff_head loopback_queue;
static DEFINE_TIMER(loopback_timer, nr_loopback_timer, 0, 0);
=======
static void nr_loopback_timer(struct timer_list *);

static struct sk_buff_head loopback_queue;
static DEFINE_TIMER(loopback_timer, nr_loopback_timer);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

void __init nr_loopback_init(void)
{
	skb_queue_head_init(&loopback_queue);
}

static inline int nr_loopback_running(void)
{
	return timer_pending(&loopback_timer);
}

int nr_loopback_queue(struct sk_buff *skb)
{
	struct sk_buff *skbn;

	if ((skbn = alloc_skb(skb->len, GFP_ATOMIC)) != NULL) {
		skb_copy_from_linear_data(skb, skb_put(skbn, skb->len), skb->len);
		skb_reset_transport_header(skbn);

		skb_queue_tail(&loopback_queue, skbn);

		if (!nr_loopback_running())
			mod_timer(&loopback_timer, jiffies + 10);
	}

	kfree_skb(skb);
	return 1;
}

<<<<<<< HEAD
static void nr_loopback_timer(unsigned long param)
=======
static void nr_loopback_timer(struct timer_list *unused)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct sk_buff *skb;
	ax25_address *nr_dest;
	struct net_device *dev;

	if ((skb = skb_dequeue(&loopback_queue)) != NULL) {
		nr_dest = (ax25_address *)(skb->data + 7);

		dev = nr_dev_get(nr_dest);

		if (dev == NULL || nr_rx_frame(skb, dev) == 0)
			kfree_skb(skb);

<<<<<<< HEAD
		if (dev != NULL)
			dev_put(dev);
=======
		dev_put(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (!skb_queue_empty(&loopback_queue) && !nr_loopback_running())
			mod_timer(&loopback_timer, jiffies + 10);
	}
}

<<<<<<< HEAD
void __exit nr_loopback_clear(void)
=======
void nr_loopback_clear(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	del_timer_sync(&loopback_timer);
	skb_queue_purge(&loopback_queue);
}
