<<<<<<< HEAD
/*
 * Copyright (c) 2007-2011 Nicira Networks.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 2 of the GNU General Public
 * License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA
 */

#include <linux/hardirq.h>
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2007-2012 Nicira, Inc.
 */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/if_vlan.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/ethtool.h>
#include <linux/skbuff.h>

#include <net/dst.h>
#include <net/xfrm.h>
<<<<<<< HEAD
=======
#include <net/rtnetlink.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include "datapath.h"
#include "vport-internal_dev.h"
#include "vport-netdev.h"

struct internal_dev {
	struct vport *vport;
};

<<<<<<< HEAD
=======
static struct vport_ops ovs_internal_vport_ops;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct internal_dev *internal_dev_priv(struct net_device *netdev)
{
	return netdev_priv(netdev);
}

<<<<<<< HEAD
/* This function is only called by the kernel network layer.*/
static struct rtnl_link_stats64 *internal_dev_get_stats(struct net_device *netdev,
							struct rtnl_link_stats64 *stats)
{
	struct vport *vport = ovs_internal_dev_get_vport(netdev);
	struct ovs_vport_stats vport_stats;

	ovs_vport_get_stats(vport, &vport_stats);

	/* The tx and rx stats need to be swapped because the
	 * switch and host OS have opposite perspectives. */
	stats->rx_packets	= vport_stats.tx_packets;
	stats->tx_packets	= vport_stats.rx_packets;
	stats->rx_bytes		= vport_stats.tx_bytes;
	stats->tx_bytes		= vport_stats.rx_bytes;
	stats->rx_errors	= vport_stats.tx_errors;
	stats->tx_errors	= vport_stats.rx_errors;
	stats->rx_dropped	= vport_stats.tx_dropped;
	stats->tx_dropped	= vport_stats.rx_dropped;

	return stats;
}

static int internal_dev_mac_addr(struct net_device *dev, void *p)
{
	struct sockaddr *addr = p;

	if (!is_valid_ether_addr(addr->sa_data))
		return -EADDRNOTAVAIL;
	dev->addr_assign_type &= ~NET_ADDR_RANDOM;
	memcpy(dev->dev_addr, addr->sa_data, dev->addr_len);
	return 0;
}

/* Called with rcu_read_lock_bh. */
static int internal_dev_xmit(struct sk_buff *skb, struct net_device *netdev)
{
	rcu_read_lock();
	ovs_vport_receive(internal_dev_priv(netdev)->vport, skb);
	rcu_read_unlock();
	return 0;
=======
/* Called with rcu_read_lock_bh. */
static netdev_tx_t
internal_dev_xmit(struct sk_buff *skb, struct net_device *netdev)
{
	int len, err;

	/* store len value because skb can be freed inside ovs_vport_receive() */
	len = skb->len;

	rcu_read_lock();
	err = ovs_vport_receive(internal_dev_priv(netdev)->vport, skb, NULL);
	rcu_read_unlock();

	if (likely(!err))
		dev_sw_netstats_tx_add(netdev, 1, len);
	else
		netdev->stats.tx_errors++;

	return NETDEV_TX_OK;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int internal_dev_open(struct net_device *netdev)
{
	netif_start_queue(netdev);
	return 0;
}

static int internal_dev_stop(struct net_device *netdev)
{
	netif_stop_queue(netdev);
	return 0;
}

static void internal_dev_getinfo(struct net_device *netdev,
				 struct ethtool_drvinfo *info)
{
<<<<<<< HEAD
	strcpy(info->driver, "openvswitch");
=======
	strscpy(info->driver, "openvswitch", sizeof(info->driver));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const struct ethtool_ops internal_dev_ethtool_ops = {
	.get_drvinfo	= internal_dev_getinfo,
	.get_link	= ethtool_op_get_link,
};

<<<<<<< HEAD
static int internal_dev_change_mtu(struct net_device *netdev, int new_mtu)
{
	if (new_mtu < 68)
		return -EINVAL;

	netdev->mtu = new_mtu;
	return 0;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void internal_dev_destructor(struct net_device *dev)
{
	struct vport *vport = ovs_internal_dev_get_vport(dev);

	ovs_vport_free(vport);
<<<<<<< HEAD
	free_netdev(dev);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const struct net_device_ops internal_dev_netdev_ops = {
	.ndo_open = internal_dev_open,
	.ndo_stop = internal_dev_stop,
	.ndo_start_xmit = internal_dev_xmit,
<<<<<<< HEAD
	.ndo_set_mac_address = internal_dev_mac_addr,
	.ndo_change_mtu = internal_dev_change_mtu,
	.ndo_get_stats64 = internal_dev_get_stats,
=======
	.ndo_set_mac_address = eth_mac_addr,
	.ndo_get_stats64 = dev_get_tstats64,
};

static struct rtnl_link_ops internal_dev_link_ops __read_mostly = {
	.kind = "openvswitch",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static void do_setup(struct net_device *netdev)
{
	ether_setup(netdev);

<<<<<<< HEAD
	netdev->netdev_ops = &internal_dev_netdev_ops;

	netdev->priv_flags &= ~IFF_TX_SKB_SHARING;
	netdev->destructor = internal_dev_destructor;
	SET_ETHTOOL_OPS(netdev, &internal_dev_ethtool_ops);
	netdev->tx_queue_len = 0;

	netdev->features = NETIF_F_LLTX | NETIF_F_SG | NETIF_F_FRAGLIST |
				NETIF_F_HIGHDMA | NETIF_F_HW_CSUM | NETIF_F_TSO;

	netdev->vlan_features = netdev->features;
	netdev->features |= NETIF_F_HW_VLAN_TX;
	netdev->hw_features = netdev->features & ~NETIF_F_LLTX;
=======
	netdev->max_mtu = ETH_MAX_MTU;

	netdev->netdev_ops = &internal_dev_netdev_ops;

	netdev->priv_flags &= ~IFF_TX_SKB_SHARING;
	netdev->priv_flags |= IFF_LIVE_ADDR_CHANGE | IFF_OPENVSWITCH |
			      IFF_NO_QUEUE;
	netdev->needs_free_netdev = true;
	netdev->priv_destructor = NULL;
	netdev->ethtool_ops = &internal_dev_ethtool_ops;
	netdev->rtnl_link_ops = &internal_dev_link_ops;

	netdev->features = NETIF_F_LLTX | NETIF_F_SG | NETIF_F_FRAGLIST |
			   NETIF_F_HIGHDMA | NETIF_F_HW_CSUM |
			   NETIF_F_GSO_SOFTWARE | NETIF_F_GSO_ENCAP_ALL;

	netdev->vlan_features = netdev->features;
	netdev->hw_enc_features = netdev->features;
	netdev->features |= NETIF_F_HW_VLAN_CTAG_TX | NETIF_F_HW_VLAN_STAG_TX;
	netdev->hw_features = netdev->features & ~NETIF_F_LLTX;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	eth_hw_addr_random(netdev);
}

static struct vport *internal_dev_create(const struct vport_parms *parms)
{
	struct vport *vport;
<<<<<<< HEAD
	struct netdev_vport *netdev_vport;
	struct internal_dev *internal_dev;
	int err;

	vport = ovs_vport_alloc(sizeof(struct netdev_vport),
				&ovs_internal_vport_ops, parms);
=======
	struct internal_dev *internal_dev;
	struct net_device *dev;
	int err;

	vport = ovs_vport_alloc(0, &ovs_internal_vport_ops, parms);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (IS_ERR(vport)) {
		err = PTR_ERR(vport);
		goto error;
	}

<<<<<<< HEAD
	netdev_vport = netdev_vport_priv(vport);

	netdev_vport->dev = alloc_netdev(sizeof(struct internal_dev),
					 parms->name, do_setup);
	if (!netdev_vport->dev) {
		err = -ENOMEM;
		goto error_free_vport;
	}

	internal_dev = internal_dev_priv(netdev_vport->dev);
	internal_dev->vport = vport;

	err = register_netdevice(netdev_vport->dev);
	if (err)
		goto error_free_netdev;

	dev_set_promiscuity(netdev_vport->dev, 1);
	netif_start_queue(netdev_vport->dev);

	return vport;

error_free_netdev:
	free_netdev(netdev_vport->dev);
=======
	dev = alloc_netdev(sizeof(struct internal_dev),
			   parms->name, NET_NAME_USER, do_setup);
	vport->dev = dev;
	if (!vport->dev) {
		err = -ENOMEM;
		goto error_free_vport;
	}
	vport->dev->tstats = netdev_alloc_pcpu_stats(struct pcpu_sw_netstats);
	if (!vport->dev->tstats) {
		err = -ENOMEM;
		goto error_free_netdev;
	}

	dev_net_set(vport->dev, ovs_dp_get_net(vport->dp));
	dev->ifindex = parms->desired_ifindex;
	internal_dev = internal_dev_priv(vport->dev);
	internal_dev->vport = vport;

	/* Restrict bridge port to current netns. */
	if (vport->port_no == OVSP_LOCAL)
		vport->dev->features |= NETIF_F_NETNS_LOCAL;

	rtnl_lock();
	err = register_netdevice(vport->dev);
	if (err)
		goto error_unlock;
	vport->dev->priv_destructor = internal_dev_destructor;

	dev_set_promiscuity(vport->dev, 1);
	rtnl_unlock();
	netif_start_queue(vport->dev);

	return vport;

error_unlock:
	rtnl_unlock();
	free_percpu(dev->tstats);
error_free_netdev:
	free_netdev(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
error_free_vport:
	ovs_vport_free(vport);
error:
	return ERR_PTR(err);
}

static void internal_dev_destroy(struct vport *vport)
{
<<<<<<< HEAD
	struct netdev_vport *netdev_vport = netdev_vport_priv(vport);

	netif_stop_queue(netdev_vport->dev);
	dev_set_promiscuity(netdev_vport->dev, -1);

	/* unregister_netdevice() waits for an RCU grace period. */
	unregister_netdevice(netdev_vport->dev);
}

static int internal_dev_recv(struct vport *vport, struct sk_buff *skb)
{
	struct net_device *netdev = netdev_vport_priv(vport)->dev;
	int len;

	len = skb->len;

	skb_dst_drop(skb);
	nf_reset(skb);
	secpath_reset(skb);

	skb->dev = netdev;
	skb->pkt_type = PACKET_HOST;
	skb->protocol = eth_type_trans(skb, netdev);

	netif_rx(skb);

	return len;
}

const struct vport_ops ovs_internal_vport_ops = {
	.type		= OVS_VPORT_TYPE_INTERNAL,
	.create		= internal_dev_create,
	.destroy	= internal_dev_destroy,
	.get_name	= ovs_netdev_get_name,
	.get_ifindex	= ovs_netdev_get_ifindex,
=======
	netif_stop_queue(vport->dev);
	rtnl_lock();
	dev_set_promiscuity(vport->dev, -1);

	/* unregister_netdevice() waits for an RCU grace period. */
	unregister_netdevice(vport->dev);
	free_percpu(vport->dev->tstats);
	rtnl_unlock();
}

static int internal_dev_recv(struct sk_buff *skb)
{
	struct net_device *netdev = skb->dev;

	if (unlikely(!(netdev->flags & IFF_UP))) {
		kfree_skb(skb);
		netdev->stats.rx_dropped++;
		return NETDEV_TX_OK;
	}

	skb_dst_drop(skb);
	nf_reset_ct(skb);
	secpath_reset(skb);

	skb->pkt_type = PACKET_HOST;
	skb->protocol = eth_type_trans(skb, netdev);
	skb_postpull_rcsum(skb, eth_hdr(skb), ETH_HLEN);
	dev_sw_netstats_rx_add(netdev, skb->len);

	netif_rx(skb);
	return NETDEV_TX_OK;
}

static struct vport_ops ovs_internal_vport_ops = {
	.type		= OVS_VPORT_TYPE_INTERNAL,
	.create		= internal_dev_create,
	.destroy	= internal_dev_destroy,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.send		= internal_dev_recv,
};

int ovs_is_internal_dev(const struct net_device *netdev)
{
	return netdev->netdev_ops == &internal_dev_netdev_ops;
}

struct vport *ovs_internal_dev_get_vport(struct net_device *netdev)
{
	if (!ovs_is_internal_dev(netdev))
		return NULL;

	return internal_dev_priv(netdev)->vport;
}
<<<<<<< HEAD
=======

int ovs_internal_dev_rtnl_link_register(void)
{
	int err;

	err = rtnl_link_register(&internal_dev_link_ops);
	if (err < 0)
		return err;

	err = ovs_vport_ops_register(&ovs_internal_vport_ops);
	if (err < 0)
		rtnl_link_unregister(&internal_dev_link_ops);

	return err;
}

void ovs_internal_dev_rtnl_link_unregister(void)
{
	ovs_vport_ops_unregister(&ovs_internal_vport_ops);
	rtnl_link_unregister(&internal_dev_link_ops);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
