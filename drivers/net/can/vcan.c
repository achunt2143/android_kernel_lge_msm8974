<<<<<<< HEAD
/*
 * vcan.c - Virtual CAN interface
 *
 * Copyright (c) 2002-2007 Volkswagen Group Electronic Research
=======
/* vcan.c - Virtual CAN interface
 *
 * Copyright (c) 2002-2017 Volkswagen Group Electronic Research
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of Volkswagen nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * Alternatively, provided that this notice is retained in full, this
 * software may be distributed under the terms of the GNU General
 * Public License ("GPL") version 2, in which case the provisions of the
 * GPL apply INSTEAD OF those given above.
 *
 * The provided data structures and external interfaces from this code
 * are not restricted to be used by modules with a GPL compatible license.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 */

<<<<<<< HEAD
=======
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/ethtool.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/module.h>
#include <linux/init.h>
#include <linux/netdevice.h>
#include <linux/if_arp.h>
#include <linux/if_ether.h>
#include <linux/can.h>
<<<<<<< HEAD
#include <linux/can/dev.h>
#include <linux/slab.h>
#include <net/rtnetlink.h>

static __initdata const char banner[] =
	KERN_INFO "vcan: Virtual CAN interface driver\n";
=======
#include <linux/can/can-ml.h>
#include <linux/can/dev.h>
#include <linux/can/skb.h>
#include <linux/slab.h>
#include <net/rtnetlink.h>

#define DRV_NAME "vcan"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

MODULE_DESCRIPTION("virtual CAN interface");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Urs Thuermann <urs.thuermann@volkswagen.de>");
<<<<<<< HEAD


/*
 * CAN test feature:
 * Enable the echo on driver level for testing the CAN core echo modes.
 * See Documentation/networking/can.txt for details.
 */

static bool echo; /* echo testing. Default: 0 (Off) */
module_param(echo, bool, S_IRUGO);
MODULE_PARM_DESC(echo, "Echo sent frames (for testing). Default: 0 (Off)");


static void vcan_rx(struct sk_buff *skb, struct net_device *dev)
{
	struct can_frame *cf = (struct can_frame *)skb->data;
	struct net_device_stats *stats = &dev->stats;

	stats->rx_packets++;
	stats->rx_bytes += cf->can_dlc;

	skb->protocol  = htons(ETH_P_CAN);
=======
MODULE_ALIAS_RTNL_LINK(DRV_NAME);

/* CAN test feature:
 * Enable the echo on driver level for testing the CAN core echo modes.
 * See Documentation/networking/can.rst for details.
 */

static bool echo; /* echo testing. Default: 0 (Off) */
module_param(echo, bool, 0444);
MODULE_PARM_DESC(echo, "Echo sent frames (for testing). Default: 0 (Off)");

static void vcan_rx(struct sk_buff *skb, struct net_device *dev)
{
	struct net_device_stats *stats = &dev->stats;

	stats->rx_packets++;
	stats->rx_bytes += can_skb_get_data_len(skb);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	skb->pkt_type  = PACKET_BROADCAST;
	skb->dev       = dev;
	skb->ip_summed = CHECKSUM_UNNECESSARY;

<<<<<<< HEAD
	netif_rx_ni(skb);
=======
	netif_rx(skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static netdev_tx_t vcan_tx(struct sk_buff *skb, struct net_device *dev)
{
<<<<<<< HEAD
	struct can_frame *cf = (struct can_frame *)skb->data;
	struct net_device_stats *stats = &dev->stats;
=======
	struct net_device_stats *stats = &dev->stats;
	unsigned int len;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int loop;

	if (can_dropped_invalid_skb(dev, skb))
		return NETDEV_TX_OK;

<<<<<<< HEAD
	stats->tx_packets++;
	stats->tx_bytes += cf->can_dlc;
=======
	len = can_skb_get_data_len(skb);
	stats->tx_packets++;
	stats->tx_bytes += len;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* set flag whether this packet has to be looped back */
	loop = skb->pkt_type == PACKET_LOOPBACK;

<<<<<<< HEAD
	if (!echo) {
		/* no echo handling available inside this driver */

		if (loop) {
			/*
			 * only count the packets here, because the
			 * CAN core already did the echo for us
			 */
			stats->rx_packets++;
			stats->rx_bytes += cf->can_dlc;
		}
		kfree_skb(skb);
=======
	skb_tx_timestamp(skb);

	if (!echo) {
		/* no echo handling available inside this driver */
		if (loop) {
			/* only count the packets here, because the
			 * CAN core already did the echo for us
			 */
			stats->rx_packets++;
			stats->rx_bytes += len;
		}
		consume_skb(skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return NETDEV_TX_OK;
	}

	/* perform standard echo handling for CAN network interfaces */

	if (loop) {
<<<<<<< HEAD
		struct sock *srcsk = skb->sk;

		skb = skb_share_check(skb, GFP_ATOMIC);
=======
		skb = can_create_echo_skb(skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!skb)
			return NETDEV_TX_OK;

		/* receive with packet counting */
<<<<<<< HEAD
		skb->sk = srcsk;
		vcan_rx(skb, dev);
	} else {
		/* no looped packets => no counting */
		kfree_skb(skb);
=======
		vcan_rx(skb, dev);
	} else {
		/* no looped packets => no counting */
		consume_skb(skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return NETDEV_TX_OK;
}

<<<<<<< HEAD
static const struct net_device_ops vcan_netdev_ops = {
	.ndo_start_xmit = vcan_tx,
=======
static int vcan_change_mtu(struct net_device *dev, int new_mtu)
{
	/* Do not allow changing the MTU while running */
	if (dev->flags & IFF_UP)
		return -EBUSY;

	if (new_mtu != CAN_MTU && new_mtu != CANFD_MTU &&
	    !can_is_canxl_dev_mtu(new_mtu))
		return -EINVAL;

	dev->mtu = new_mtu;
	return 0;
}

static const struct net_device_ops vcan_netdev_ops = {
	.ndo_start_xmit = vcan_tx,
	.ndo_change_mtu = vcan_change_mtu,
};

static const struct ethtool_ops vcan_ethtool_ops = {
	.get_ts_info = ethtool_op_get_ts_info,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static void vcan_setup(struct net_device *dev)
{
	dev->type		= ARPHRD_CAN;
<<<<<<< HEAD
	dev->mtu		= sizeof(struct can_frame);
=======
	dev->mtu		= CANFD_MTU;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dev->hard_header_len	= 0;
	dev->addr_len		= 0;
	dev->tx_queue_len	= 0;
	dev->flags		= IFF_NOARP;
<<<<<<< HEAD
=======
	can_set_ml_priv(dev, netdev_priv(dev));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* set flags according to driver capabilities */
	if (echo)
		dev->flags |= IFF_ECHO;

	dev->netdev_ops		= &vcan_netdev_ops;
<<<<<<< HEAD
	dev->destructor		= free_netdev;
}

static struct rtnl_link_ops vcan_link_ops __read_mostly = {
	.kind	= "vcan",
	.setup	= vcan_setup,
=======
	dev->ethtool_ops	= &vcan_ethtool_ops;
	dev->needs_free_netdev	= true;
}

static struct rtnl_link_ops vcan_link_ops __read_mostly = {
	.kind = DRV_NAME,
	.priv_size = sizeof(struct can_ml_priv),
	.setup = vcan_setup,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static __init int vcan_init_module(void)
{
<<<<<<< HEAD
	printk(banner);

	if (echo)
		printk(KERN_INFO "vcan: enabled echo on driver level.\n");
=======
	pr_info("Virtual CAN interface driver\n");

	if (echo)
		pr_info("enabled echo on driver level.\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return rtnl_link_register(&vcan_link_ops);
}

static __exit void vcan_cleanup_module(void)
{
	rtnl_link_unregister(&vcan_link_ops);
}

module_init(vcan_init_module);
module_exit(vcan_cleanup_module);
