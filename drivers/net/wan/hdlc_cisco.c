<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Generic HDLC support routines for Linux
 * Cisco HDLC support
 *
 * Copyright (C) 2000 - 2006 Krzysztof Halasa <khc@pm.waw.pl>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License
 * as published by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/errno.h>
#include <linux/hdlc.h>
#include <linux/if_arp.h>
#include <linux/inetdevice.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pkt_sched.h>
#include <linux/poll.h>
#include <linux/rtnetlink.h>
#include <linux/skbuff.h>

#undef DEBUG_HARD_HEADER

#define CISCO_MULTICAST		0x8F	/* Cisco multicast address */
#define CISCO_UNICAST		0x0F	/* Cisco unicast address */
#define CISCO_KEEPALIVE		0x8035	/* Cisco keepalive protocol */
#define CISCO_SYS_INFO		0x2000	/* Cisco interface/system info */
#define CISCO_ADDR_REQ		0	/* Cisco address request */
#define CISCO_ADDR_REPLY	1	/* Cisco address reply */
#define CISCO_KEEPALIVE_REQ	2	/* Cisco keepalive request */

<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct hdlc_header {
	u8 address;
	u8 control;
	__be16 protocol;
<<<<<<< HEAD
}__packed;

=======
} __packed;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct cisco_packet {
	__be32 type;		/* code */
	__be32 par1;
	__be32 par2;
	__be16 rel;		/* reliability */
	__be32 time;
<<<<<<< HEAD
}__packed;
#define	CISCO_PACKET_LEN	18
#define	CISCO_BIG_PACKET_LEN	20


=======
} __packed;
#define	CISCO_PACKET_LEN	18
#define	CISCO_BIG_PACKET_LEN	20

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct cisco_state {
	cisco_proto settings;

	struct timer_list timer;
<<<<<<< HEAD
=======
	struct net_device *dev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spinlock_t lock;
	unsigned long last_poll;
	int up;
	u32 txseq; /* TX sequence number, 0 = none */
	u32 rxseq; /* RX sequence number */
};

<<<<<<< HEAD

static int cisco_ioctl(struct net_device *dev, struct ifreq *ifr);


static inline struct cisco_state* state(hdlc_device *hdlc)
=======
static int cisco_ioctl(struct net_device *dev, struct if_settings *ifs);

static inline struct cisco_state *state(hdlc_device *hdlc)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return (struct cisco_state *)hdlc->state;
}

<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int cisco_hard_header(struct sk_buff *skb, struct net_device *dev,
			     u16 type, const void *daddr, const void *saddr,
			     unsigned int len)
{
	struct hdlc_header *data;
#ifdef DEBUG_HARD_HEADER
<<<<<<< HEAD
	printk(KERN_DEBUG "%s: cisco_hard_header called\n", dev->name);
#endif

	skb_push(skb, sizeof(struct hdlc_header));
	data = (struct hdlc_header*)skb->data;
=======
	netdev_dbg(dev, "%s called\n", __func__);
#endif

	skb_push(skb, sizeof(struct hdlc_header));
	data = (struct hdlc_header *)skb->data;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (type == CISCO_KEEPALIVE)
		data->address = CISCO_MULTICAST;
	else
		data->address = CISCO_UNICAST;
	data->control = 0;
	data->protocol = htons(type);

	return sizeof(struct hdlc_header);
}

<<<<<<< HEAD


=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void cisco_keepalive_send(struct net_device *dev, u32 type,
				 __be32 par1, __be32 par2)
{
	struct sk_buff *skb;
	struct cisco_packet *data;

	skb = dev_alloc_skb(sizeof(struct hdlc_header) +
			    sizeof(struct cisco_packet));
<<<<<<< HEAD
	if (!skb) {
		netdev_warn(dev, "Memory squeeze on cisco_keepalive_send()\n");
		return;
	}
	skb_reserve(skb, 4);
	cisco_hard_header(skb, dev, CISCO_KEEPALIVE, NULL, NULL, 0);
	data = (struct cisco_packet*)(skb->data + 4);
=======
	if (!skb)
		return;

	skb_reserve(skb, 4);
	cisco_hard_header(skb, dev, CISCO_KEEPALIVE, NULL, NULL, 0);
	data = (struct cisco_packet *)(skb->data + 4);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	data->type = htonl(type);
	data->par1 = par1;
	data->par2 = par2;
	data->rel = cpu_to_be16(0xFFFF);
	/* we will need do_div here if 1000 % HZ != 0 */
	data->time = htonl((jiffies - INITIAL_JIFFIES) * (1000 / HZ));

	skb_put(skb, sizeof(struct cisco_packet));
	skb->priority = TC_PRIO_CONTROL;
	skb->dev = dev;
<<<<<<< HEAD
=======
	skb->protocol = htons(ETH_P_HDLC);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	skb_reset_network_header(skb);

	dev_queue_xmit(skb);
}

<<<<<<< HEAD


static __be16 cisco_type_trans(struct sk_buff *skb, struct net_device *dev)
{
	struct hdlc_header *data = (struct hdlc_header*)skb->data;
=======
static __be16 cisco_type_trans(struct sk_buff *skb, struct net_device *dev)
{
	struct hdlc_header *data = (struct hdlc_header *)skb->data;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (skb->len < sizeof(struct hdlc_header))
		return cpu_to_be16(ETH_P_HDLC);

	if (data->address != CISCO_MULTICAST &&
	    data->address != CISCO_UNICAST)
		return cpu_to_be16(ETH_P_HDLC);

	switch (data->protocol) {
	case cpu_to_be16(ETH_P_IP):
	case cpu_to_be16(ETH_P_IPX):
	case cpu_to_be16(ETH_P_IPV6):
		skb_pull(skb, sizeof(struct hdlc_header));
		return data->protocol;
	default:
		return cpu_to_be16(ETH_P_HDLC);
	}
}

<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int cisco_rx(struct sk_buff *skb)
{
	struct net_device *dev = skb->dev;
	hdlc_device *hdlc = dev_to_hdlc(dev);
	struct cisco_state *st = state(hdlc);
<<<<<<< HEAD
	struct hdlc_header *data = (struct hdlc_header*)skb->data;
=======
	struct hdlc_header *data = (struct hdlc_header *)skb->data;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct cisco_packet *cisco_data;
	struct in_device *in_dev;
	__be32 addr, mask;
	u32 ack;

	if (skb->len < sizeof(struct hdlc_header))
		goto rx_error;

	if (data->address != CISCO_MULTICAST &&
	    data->address != CISCO_UNICAST)
		goto rx_error;

	switch (ntohs(data->protocol)) {
	case CISCO_SYS_INFO:
		/* Packet is not needed, drop it. */
		dev_kfree_skb_any(skb);
		return NET_RX_SUCCESS;

	case CISCO_KEEPALIVE:
		if ((skb->len != sizeof(struct hdlc_header) +
		     CISCO_PACKET_LEN) &&
		    (skb->len != sizeof(struct hdlc_header) +
		     CISCO_BIG_PACKET_LEN)) {
			netdev_info(dev, "Invalid length of Cisco control packet (%d bytes)\n",
				    skb->len);
			goto rx_error;
		}

<<<<<<< HEAD
		cisco_data = (struct cisco_packet*)(skb->data + sizeof
						    (struct hdlc_header));

		switch (ntohl (cisco_data->type)) {
=======
		cisco_data = (struct cisco_packet *)(skb->data + sizeof
						    (struct hdlc_header));

		switch (ntohl(cisco_data->type)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		case CISCO_ADDR_REQ: /* Stolen from syncppp.c :-) */
			rcu_read_lock();
			in_dev = __in_dev_get_rcu(dev);
			addr = 0;
			mask = ~cpu_to_be32(0); /* is the mask correct? */

			if (in_dev != NULL) {
<<<<<<< HEAD
				struct in_ifaddr **ifap = &in_dev->ifa_list;

				while (*ifap != NULL) {
					if (strcmp(dev->name,
						   (*ifap)->ifa_label) == 0) {
						addr = (*ifap)->ifa_local;
						mask = (*ifap)->ifa_mask;
						break;
					}
					ifap = &(*ifap)->ifa_next;
=======
				const struct in_ifaddr *ifa;

				in_dev_for_each_ifa_rcu(ifa, in_dev) {
					if (strcmp(dev->name,
						   ifa->ifa_label) == 0) {
						addr = ifa->ifa_local;
						mask = ifa->ifa_mask;
						break;
					}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				}

				cisco_keepalive_send(dev, CISCO_ADDR_REPLY,
						     addr, mask);
			}
			rcu_read_unlock();
			dev_kfree_skb_any(skb);
			return NET_RX_SUCCESS;

		case CISCO_ADDR_REPLY:
			netdev_info(dev, "Unexpected Cisco IP address reply\n");
			goto rx_error;

		case CISCO_KEEPALIVE_REQ:
			spin_lock(&st->lock);
			st->rxseq = ntohl(cisco_data->par1);
			ack = ntohl(cisco_data->par2);
			if (ack && (ack == st->txseq ||
				    /* our current REQ may be in transit */
				    ack == st->txseq - 1)) {
				st->last_poll = jiffies;
				if (!st->up) {
					u32 sec, min, hrs, days;
<<<<<<< HEAD
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					sec = ntohl(cisco_data->time) / 1000;
					min = sec / 60; sec -= min * 60;
					hrs = min / 60; min -= hrs * 60;
					days = hrs / 24; hrs -= days * 24;
					netdev_info(dev, "Link up (peer uptime %ud%uh%um%us)\n",
						    days, hrs, min, sec);
					netif_dormant_off(dev);
					st->up = 1;
				}
			}
			spin_unlock(&st->lock);

			dev_kfree_skb_any(skb);
			return NET_RX_SUCCESS;
		} /* switch (keepalive type) */
	} /* switch (protocol) */

	netdev_info(dev, "Unsupported protocol %x\n", ntohs(data->protocol));
	dev_kfree_skb_any(skb);
	return NET_RX_DROP;

rx_error:
	dev->stats.rx_errors++; /* Mark error */
	dev_kfree_skb_any(skb);
	return NET_RX_DROP;
}

<<<<<<< HEAD


static void cisco_timer(unsigned long arg)
{
	struct net_device *dev = (struct net_device *)arg;
	hdlc_device *hdlc = dev_to_hdlc(dev);
	struct cisco_state *st = state(hdlc);
=======
static void cisco_timer(struct timer_list *t)
{
	struct cisco_state *st = from_timer(st, t, timer);
	struct net_device *dev = st->dev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spin_lock(&st->lock);
	if (st->up &&
	    time_after(jiffies, st->last_poll + st->settings.timeout * HZ)) {
		st->up = 0;
		netdev_info(dev, "Link down\n");
		netif_dormant_on(dev);
	}

	cisco_keepalive_send(dev, CISCO_KEEPALIVE_REQ, htonl(++st->txseq),
			     htonl(st->rxseq));
	spin_unlock(&st->lock);

	st->timer.expires = jiffies + st->settings.interval * HZ;
<<<<<<< HEAD
	st->timer.function = cisco_timer;
	st->timer.data = arg;
	add_timer(&st->timer);
}



=======
	add_timer(&st->timer);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void cisco_start(struct net_device *dev)
{
	hdlc_device *hdlc = dev_to_hdlc(dev);
	struct cisco_state *st = state(hdlc);
	unsigned long flags;

	spin_lock_irqsave(&st->lock, flags);
	st->up = st->txseq = st->rxseq = 0;
	spin_unlock_irqrestore(&st->lock, flags);

<<<<<<< HEAD
	init_timer(&st->timer);
	st->timer.expires = jiffies + HZ; /* First poll after 1 s */
	st->timer.function = cisco_timer;
	st->timer.data = (unsigned long)dev;
	add_timer(&st->timer);
}



=======
	st->dev = dev;
	timer_setup(&st->timer, cisco_timer, 0);
	st->timer.expires = jiffies + HZ; /* First poll after 1 s */
	add_timer(&st->timer);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void cisco_stop(struct net_device *dev)
{
	hdlc_device *hdlc = dev_to_hdlc(dev);
	struct cisco_state *st = state(hdlc);
	unsigned long flags;

	del_timer_sync(&st->timer);

	spin_lock_irqsave(&st->lock, flags);
	netif_dormant_on(dev);
	st->up = st->txseq = 0;
	spin_unlock_irqrestore(&st->lock, flags);
}

<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct hdlc_proto proto = {
	.start		= cisco_start,
	.stop		= cisco_stop,
	.type_trans	= cisco_type_trans,
	.ioctl		= cisco_ioctl,
	.netif_rx	= cisco_rx,
	.module		= THIS_MODULE,
};

static const struct header_ops cisco_header_ops = {
	.create = cisco_hard_header,
};

<<<<<<< HEAD
static int cisco_ioctl(struct net_device *dev, struct ifreq *ifr)
{
	cisco_proto __user *cisco_s = ifr->ifr_settings.ifs_ifsu.cisco;
=======
static int cisco_ioctl(struct net_device *dev, struct if_settings *ifs)
{
	cisco_proto __user *cisco_s = ifs->ifs_ifsu.cisco;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	const size_t size = sizeof(cisco_proto);
	cisco_proto new_settings;
	hdlc_device *hdlc = dev_to_hdlc(dev);
	int result;

<<<<<<< HEAD
	switch (ifr->ifr_settings.type) {
	case IF_GET_PROTO:
		if (dev_to_hdlc(dev)->proto != &proto)
			return -EINVAL;
		ifr->ifr_settings.type = IF_PROTO_CISCO;
		if (ifr->ifr_settings.size < size) {
			ifr->ifr_settings.size = size; /* data size wanted */
=======
	switch (ifs->type) {
	case IF_GET_PROTO:
		if (dev_to_hdlc(dev)->proto != &proto)
			return -EINVAL;
		ifs->type = IF_PROTO_CISCO;
		if (ifs->size < size) {
			ifs->size = size; /* data size wanted */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -ENOBUFS;
		}
		if (copy_to_user(cisco_s, &state(hdlc)->settings, size))
			return -EFAULT;
		return 0;

	case IF_PROTO_CISCO:
		if (!capable(CAP_NET_ADMIN))
			return -EPERM;

		if (dev->flags & IFF_UP)
			return -EBUSY;

		if (copy_from_user(&new_settings, cisco_s, size))
			return -EFAULT;

		if (new_settings.interval < 1 ||
		    new_settings.timeout < 2)
			return -EINVAL;

<<<<<<< HEAD
		result = hdlc->attach(dev, ENCODING_NRZ,PARITY_CRC16_PR1_CCITT);
=======
		result = hdlc->attach(dev, ENCODING_NRZ,
				      PARITY_CRC16_PR1_CCITT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (result)
			return result;

		result = attach_hdlc_protocol(dev, &proto,
					      sizeof(struct cisco_state));
		if (result)
			return result;

		memcpy(&state(hdlc)->settings, &new_settings, size);
		spin_lock_init(&state(hdlc)->lock);
		dev->header_ops = &cisco_header_ops;
<<<<<<< HEAD
		dev->type = ARPHRD_CISCO;
=======
		dev->hard_header_len = sizeof(struct hdlc_header);
		dev->type = ARPHRD_CISCO;
		call_netdevice_notifiers(NETDEV_POST_TYPE_CHANGE, dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		netif_dormant_on(dev);
		return 0;
	}

	return -EINVAL;
}

<<<<<<< HEAD

static int __init mod_init(void)
=======
static int __init hdlc_cisco_init(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	register_hdlc_protocol(&proto);
	return 0;
}

<<<<<<< HEAD


static void __exit mod_exit(void)
=======
static void __exit hdlc_cisco_exit(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unregister_hdlc_protocol(&proto);
}

<<<<<<< HEAD

module_init(mod_init);
module_exit(mod_exit);
=======
module_init(hdlc_cisco_init);
module_exit(hdlc_cisco_exit);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

MODULE_AUTHOR("Krzysztof Halasa <khc@pm.waw.pl>");
MODULE_DESCRIPTION("Cisco HDLC protocol support for generic HDLC");
MODULE_LICENSE("GPL v2");
