<<<<<<< HEAD
/*
 * Copyright (C) 2007-2012 B.A.T.M.A.N. contributors:
 *
 * Marek Lindner, Simon Wunderlich
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
 *
 */

#include "main.h"
#include "soft-interface.h"
#include "hard-interface.h"
#include "routing.h"
#include "send.h"
#include "bat_debugfs.h"
#include "translation-table.h"
#include "hash.h"
#include "gateway_common.h"
#include "gateway_client.h"
#include "bat_sysfs.h"
#include "originator.h"
#include <linux/slab.h>
#include <linux/ethtool.h>
#include <linux/etherdevice.h>
#include <linux/if_vlan.h>
#include "unicast.h"


static int bat_get_settings(struct net_device *dev, struct ethtool_cmd *cmd);
static void bat_get_drvinfo(struct net_device *dev,
			    struct ethtool_drvinfo *info);
static u32 bat_get_msglevel(struct net_device *dev);
static void bat_set_msglevel(struct net_device *dev, u32 value);
static u32 bat_get_link(struct net_device *dev);

static const struct ethtool_ops bat_ethtool_ops = {
	.get_settings = bat_get_settings,
	.get_drvinfo = bat_get_drvinfo,
	.get_msglevel = bat_get_msglevel,
	.set_msglevel = bat_set_msglevel,
	.get_link = bat_get_link,
};

int my_skb_head_push(struct sk_buff *skb, unsigned int len)
{
	int result;

	/**
	 * TODO: We must check if we can release all references to non-payload
	 * data using skb_header_release in our skbs to allow skb_cow_header to
	 * work optimally. This means that those skbs are not allowed to read
=======
// SPDX-License-Identifier: GPL-2.0
/* Copyright (C) B.A.T.M.A.N. contributors:
 *
 * Marek Lindner, Simon Wunderlich
 */

#include "soft-interface.h"
#include "main.h"

#include <linux/atomic.h>
#include <linux/byteorder/generic.h>
#include <linux/cache.h>
#include <linux/compiler.h>
#include <linux/container_of.h>
#include <linux/cpumask.h>
#include <linux/errno.h>
#include <linux/etherdevice.h>
#include <linux/ethtool.h>
#include <linux/gfp.h>
#include <linux/if_ether.h>
#include <linux/if_vlan.h>
#include <linux/jiffies.h>
#include <linux/kref.h>
#include <linux/list.h>
#include <linux/lockdep.h>
#include <linux/netdevice.h>
#include <linux/netlink.h>
#include <linux/percpu.h>
#include <linux/random.h>
#include <linux/rculist.h>
#include <linux/rcupdate.h>
#include <linux/skbuff.h>
#include <linux/slab.h>
#include <linux/socket.h>
#include <linux/spinlock.h>
#include <linux/stddef.h>
#include <linux/string.h>
#include <linux/types.h>
#include <net/net_namespace.h>
#include <net/netlink.h>
#include <uapi/linux/batadv_packet.h>
#include <uapi/linux/batman_adv.h>

#include "bat_algo.h"
#include "bridge_loop_avoidance.h"
#include "distributed-arp-table.h"
#include "gateway_client.h"
#include "hard-interface.h"
#include "multicast.h"
#include "network-coding.h"
#include "send.h"
#include "translation-table.h"

/**
 * batadv_skb_head_push() - Increase header size and move (push) head pointer
 * @skb: packet buffer which should be modified
 * @len: number of bytes to add
 *
 * Return: 0 on success or negative error number in case of failure
 */
int batadv_skb_head_push(struct sk_buff *skb, unsigned int len)
{
	int result;

	/* TODO: We must check if we can release all references to non-payload
	 * data using __skb_header_release in our skbs to allow skb_cow_header
	 * to work optimally. This means that those skbs are not allowed to read
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * or write any data which is before the current position of skb->data
	 * after that call and thus allow other skbs with the same data buffer
	 * to write freely in that area.
	 */
	result = skb_cow_head(skb, len);
	if (result < 0)
		return result;

	skb_push(skb, len);
	return 0;
}

<<<<<<< HEAD
static void softif_neigh_free_ref(struct softif_neigh *softif_neigh)
{
	if (atomic_dec_and_test(&softif_neigh->refcount))
		kfree_rcu(softif_neigh, rcu);
}

static void softif_neigh_vid_free_rcu(struct rcu_head *rcu)
{
	struct softif_neigh_vid *softif_neigh_vid;
	struct softif_neigh *softif_neigh;
	struct hlist_node *node, *node_tmp;
	struct bat_priv *bat_priv;

	softif_neigh_vid = container_of(rcu, struct softif_neigh_vid, rcu);
	bat_priv = softif_neigh_vid->bat_priv;

	spin_lock_bh(&bat_priv->softif_neigh_lock);
	hlist_for_each_entry_safe(softif_neigh, node, node_tmp,
				  &softif_neigh_vid->softif_neigh_list, list) {
		hlist_del_rcu(&softif_neigh->list);
		softif_neigh_free_ref(softif_neigh);
	}
	spin_unlock_bh(&bat_priv->softif_neigh_lock);

	kfree(softif_neigh_vid);
}

static void softif_neigh_vid_free_ref(struct softif_neigh_vid *softif_neigh_vid)
{
	if (atomic_dec_and_test(&softif_neigh_vid->refcount))
		call_rcu(&softif_neigh_vid->rcu, softif_neigh_vid_free_rcu);
}

static struct softif_neigh_vid *softif_neigh_vid_get(struct bat_priv *bat_priv,
						     short vid)
{
	struct softif_neigh_vid *softif_neigh_vid;
	struct hlist_node *node;

	rcu_read_lock();
	hlist_for_each_entry_rcu(softif_neigh_vid, node,
				 &bat_priv->softif_neigh_vids, list) {
		if (softif_neigh_vid->vid != vid)
			continue;

		if (!atomic_inc_not_zero(&softif_neigh_vid->refcount))
			continue;

		goto out;
	}

	softif_neigh_vid = kzalloc(sizeof(*softif_neigh_vid), GFP_ATOMIC);
	if (!softif_neigh_vid)
		goto out;

	softif_neigh_vid->vid = vid;
	softif_neigh_vid->bat_priv = bat_priv;

	/* initialize with 2 - caller decrements counter by one */
	atomic_set(&softif_neigh_vid->refcount, 2);
	INIT_HLIST_HEAD(&softif_neigh_vid->softif_neigh_list);
	INIT_HLIST_NODE(&softif_neigh_vid->list);
	spin_lock_bh(&bat_priv->softif_neigh_vid_lock);
	hlist_add_head_rcu(&softif_neigh_vid->list,
			   &bat_priv->softif_neigh_vids);
	spin_unlock_bh(&bat_priv->softif_neigh_vid_lock);

out:
	rcu_read_unlock();
	return softif_neigh_vid;
}

static struct softif_neigh *softif_neigh_get(struct bat_priv *bat_priv,
					     const uint8_t *addr, short vid)
{
	struct softif_neigh_vid *softif_neigh_vid;
	struct softif_neigh *softif_neigh = NULL;
	struct hlist_node *node;

	softif_neigh_vid = softif_neigh_vid_get(bat_priv, vid);
	if (!softif_neigh_vid)
		goto out;

	rcu_read_lock();
	hlist_for_each_entry_rcu(softif_neigh, node,
				 &softif_neigh_vid->softif_neigh_list,
				 list) {
		if (!compare_eth(softif_neigh->addr, addr))
			continue;

		if (!atomic_inc_not_zero(&softif_neigh->refcount))
			continue;

		softif_neigh->last_seen = jiffies;
		goto unlock;
	}

	softif_neigh = kzalloc(sizeof(*softif_neigh), GFP_ATOMIC);
	if (!softif_neigh)
		goto unlock;

	memcpy(softif_neigh->addr, addr, ETH_ALEN);
	softif_neigh->last_seen = jiffies;
	/* initialize with 2 - caller decrements counter by one */
	atomic_set(&softif_neigh->refcount, 2);

	INIT_HLIST_NODE(&softif_neigh->list);
	spin_lock_bh(&bat_priv->softif_neigh_lock);
	hlist_add_head_rcu(&softif_neigh->list,
			   &softif_neigh_vid->softif_neigh_list);
	spin_unlock_bh(&bat_priv->softif_neigh_lock);

unlock:
	rcu_read_unlock();
out:
	if (softif_neigh_vid)
		softif_neigh_vid_free_ref(softif_neigh_vid);
	return softif_neigh;
}

static struct softif_neigh *softif_neigh_get_selected(
				struct softif_neigh_vid *softif_neigh_vid)
{
	struct softif_neigh *softif_neigh;

	rcu_read_lock();
	softif_neigh = rcu_dereference(softif_neigh_vid->softif_neigh);

	if (softif_neigh && !atomic_inc_not_zero(&softif_neigh->refcount))
		softif_neigh = NULL;

	rcu_read_unlock();
	return softif_neigh;
}

static struct softif_neigh *softif_neigh_vid_get_selected(
						struct bat_priv *bat_priv,
						short vid)
{
	struct softif_neigh_vid *softif_neigh_vid;
	struct softif_neigh *softif_neigh = NULL;

	softif_neigh_vid = softif_neigh_vid_get(bat_priv, vid);
	if (!softif_neigh_vid)
		goto out;

	softif_neigh = softif_neigh_get_selected(softif_neigh_vid);
out:
	if (softif_neigh_vid)
		softif_neigh_vid_free_ref(softif_neigh_vid);
	return softif_neigh;
}

static void softif_neigh_vid_select(struct bat_priv *bat_priv,
				    struct softif_neigh *new_neigh,
				    short vid)
{
	struct softif_neigh_vid *softif_neigh_vid;
	struct softif_neigh *curr_neigh;

	softif_neigh_vid = softif_neigh_vid_get(bat_priv, vid);
	if (!softif_neigh_vid)
		goto out;

	spin_lock_bh(&bat_priv->softif_neigh_lock);

	if (new_neigh && !atomic_inc_not_zero(&new_neigh->refcount))
		new_neigh = NULL;

	curr_neigh = rcu_dereference_protected(softif_neigh_vid->softif_neigh,
					       1);
	rcu_assign_pointer(softif_neigh_vid->softif_neigh, new_neigh);

	if ((curr_neigh) && (!new_neigh))
		bat_dbg(DBG_ROUTES, bat_priv,
			"Removing mesh exit point on vid: %d (prev: %pM).\n",
			vid, curr_neigh->addr);
	else if ((curr_neigh) && (new_neigh))
		bat_dbg(DBG_ROUTES, bat_priv,
			"Changing mesh exit point on vid: %d from %pM to %pM.\n",
			vid, curr_neigh->addr, new_neigh->addr);
	else if ((!curr_neigh) && (new_neigh))
		bat_dbg(DBG_ROUTES, bat_priv,
			"Setting mesh exit point on vid: %d to %pM.\n",
			vid, new_neigh->addr);

	if (curr_neigh)
		softif_neigh_free_ref(curr_neigh);

	spin_unlock_bh(&bat_priv->softif_neigh_lock);

out:
	if (softif_neigh_vid)
		softif_neigh_vid_free_ref(softif_neigh_vid);
}

static void softif_neigh_vid_deselect(struct bat_priv *bat_priv,
				      struct softif_neigh_vid *softif_neigh_vid)
{
	struct softif_neigh *curr_neigh;
	struct softif_neigh *softif_neigh = NULL, *softif_neigh_tmp;
	struct hard_iface *primary_if = NULL;
	struct hlist_node *node;

	primary_if = primary_if_get_selected(bat_priv);
	if (!primary_if)
		goto out;

	/* find new softif_neigh immediately to avoid temporary loops */
	rcu_read_lock();
	curr_neigh = rcu_dereference(softif_neigh_vid->softif_neigh);

	hlist_for_each_entry_rcu(softif_neigh_tmp, node,
				 &softif_neigh_vid->softif_neigh_list,
				 list) {
		if (softif_neigh_tmp == curr_neigh)
			continue;

		/* we got a neighbor but its mac is 'bigger' than ours  */
		if (memcmp(primary_if->net_dev->dev_addr,
			   softif_neigh_tmp->addr, ETH_ALEN) < 0)
			continue;

		if (!atomic_inc_not_zero(&softif_neigh_tmp->refcount))
			continue;

		softif_neigh = softif_neigh_tmp;
		goto unlock;
	}

unlock:
	rcu_read_unlock();
out:
	softif_neigh_vid_select(bat_priv, softif_neigh, softif_neigh_vid->vid);

	if (primary_if)
		hardif_free_ref(primary_if);
	if (softif_neigh)
		softif_neigh_free_ref(softif_neigh);
}

int softif_neigh_seq_print_text(struct seq_file *seq, void *offset)
{
	struct net_device *net_dev = (struct net_device *)seq->private;
	struct bat_priv *bat_priv = netdev_priv(net_dev);
	struct softif_neigh_vid *softif_neigh_vid;
	struct softif_neigh *softif_neigh;
	struct hard_iface *primary_if;
	struct hlist_node *node, *node_tmp;
	struct softif_neigh *curr_softif_neigh;
	int ret = 0, last_seen_secs, last_seen_msecs;

	primary_if = primary_if_get_selected(bat_priv);
	if (!primary_if) {
		ret = seq_printf(seq,
				 "BATMAN mesh %s disabled - please specify interfaces to enable it\n",
				 net_dev->name);
		goto out;
	}

	if (primary_if->if_status != IF_ACTIVE) {
		ret = seq_printf(seq,
				 "BATMAN mesh %s disabled - primary interface not active\n",
				 net_dev->name);
		goto out;
	}

	seq_printf(seq, "Softif neighbor list (%s)\n", net_dev->name);

	rcu_read_lock();
	hlist_for_each_entry_rcu(softif_neigh_vid, node,
				 &bat_priv->softif_neigh_vids, list) {
		seq_printf(seq, "     %-15s %s on vid: %d\n",
			   "Originator", "last-seen", softif_neigh_vid->vid);

		curr_softif_neigh = softif_neigh_get_selected(softif_neigh_vid);

		hlist_for_each_entry_rcu(softif_neigh, node_tmp,
					 &softif_neigh_vid->softif_neigh_list,
					 list) {
			last_seen_secs = jiffies_to_msecs(jiffies -
						softif_neigh->last_seen) / 1000;
			last_seen_msecs = jiffies_to_msecs(jiffies -
						softif_neigh->last_seen) % 1000;
			seq_printf(seq, "%s %pM  %3i.%03is\n",
				   curr_softif_neigh == softif_neigh
				   ? "=>" : "  ", softif_neigh->addr,
				   last_seen_secs, last_seen_msecs);
		}

		if (curr_softif_neigh)
			softif_neigh_free_ref(curr_softif_neigh);

		seq_printf(seq, "\n");
	}
	rcu_read_unlock();

out:
	if (primary_if)
		hardif_free_ref(primary_if);
	return ret;
}

void softif_neigh_purge(struct bat_priv *bat_priv)
{
	struct softif_neigh *softif_neigh, *curr_softif_neigh;
	struct softif_neigh_vid *softif_neigh_vid;
	struct hlist_node *node, *node_tmp, *node_tmp2;
	int do_deselect;

	rcu_read_lock();
	hlist_for_each_entry_rcu(softif_neigh_vid, node,
				 &bat_priv->softif_neigh_vids, list) {
		if (!atomic_inc_not_zero(&softif_neigh_vid->refcount))
			continue;

		curr_softif_neigh = softif_neigh_get_selected(softif_neigh_vid);
		do_deselect = 0;

		spin_lock_bh(&bat_priv->softif_neigh_lock);
		hlist_for_each_entry_safe(softif_neigh, node_tmp, node_tmp2,
					  &softif_neigh_vid->softif_neigh_list,
					  list) {
			if ((!has_timed_out(softif_neigh->last_seen,
					    SOFTIF_NEIGH_TIMEOUT)) &&
			    (atomic_read(&bat_priv->mesh_state) == MESH_ACTIVE))
				continue;

			if (curr_softif_neigh == softif_neigh) {
				bat_dbg(DBG_ROUTES, bat_priv,
					"Current mesh exit point on vid: %d '%pM' vanished.\n",
					softif_neigh_vid->vid,
					softif_neigh->addr);
				do_deselect = 1;
			}

			hlist_del_rcu(&softif_neigh->list);
			softif_neigh_free_ref(softif_neigh);
		}
		spin_unlock_bh(&bat_priv->softif_neigh_lock);

		/* soft_neigh_vid_deselect() needs to acquire the
		 * softif_neigh_lock */
		if (do_deselect)
			softif_neigh_vid_deselect(bat_priv, softif_neigh_vid);

		if (curr_softif_neigh)
			softif_neigh_free_ref(curr_softif_neigh);

		softif_neigh_vid_free_ref(softif_neigh_vid);
	}
	rcu_read_unlock();

	spin_lock_bh(&bat_priv->softif_neigh_vid_lock);
	hlist_for_each_entry_safe(softif_neigh_vid, node, node_tmp,
				  &bat_priv->softif_neigh_vids, list) {
		if (!hlist_empty(&softif_neigh_vid->softif_neigh_list))
			continue;

		hlist_del_rcu(&softif_neigh_vid->list);
		softif_neigh_vid_free_ref(softif_neigh_vid);
	}
	spin_unlock_bh(&bat_priv->softif_neigh_vid_lock);

}

static void softif_batman_recv(struct sk_buff *skb, struct net_device *dev,
			       short vid)
{
	struct bat_priv *bat_priv = netdev_priv(dev);
	struct ethhdr *ethhdr = (struct ethhdr *)skb->data;
	struct batman_ogm_packet *batman_ogm_packet;
	struct softif_neigh *softif_neigh = NULL;
	struct hard_iface *primary_if = NULL;
	struct softif_neigh *curr_softif_neigh = NULL;

	if (ntohs(ethhdr->h_proto) == ETH_P_8021Q)
		batman_ogm_packet = (struct batman_ogm_packet *)
					(skb->data + ETH_HLEN + VLAN_HLEN);
	else
		batman_ogm_packet = (struct batman_ogm_packet *)
							(skb->data + ETH_HLEN);

	if (batman_ogm_packet->header.version != COMPAT_VERSION)
		goto out;

	if (batman_ogm_packet->header.packet_type != BAT_OGM)
		goto out;

	if (!(batman_ogm_packet->flags & PRIMARIES_FIRST_HOP))
		goto out;

	if (is_my_mac(batman_ogm_packet->orig))
		goto out;

	softif_neigh = softif_neigh_get(bat_priv, batman_ogm_packet->orig, vid);
	if (!softif_neigh)
		goto out;

	curr_softif_neigh = softif_neigh_vid_get_selected(bat_priv, vid);
	if (curr_softif_neigh == softif_neigh)
		goto out;

	primary_if = primary_if_get_selected(bat_priv);
	if (!primary_if)
		goto out;

	/* we got a neighbor but its mac is 'bigger' than ours  */
	if (memcmp(primary_if->net_dev->dev_addr,
		   softif_neigh->addr, ETH_ALEN) < 0)
		goto out;

	/* close own batX device and use softif_neigh as exit node */
	if (!curr_softif_neigh) {
		softif_neigh_vid_select(bat_priv, softif_neigh, vid);
		goto out;
	}

	/* switch to new 'smallest neighbor' */
	if (memcmp(softif_neigh->addr, curr_softif_neigh->addr, ETH_ALEN) < 0)
		softif_neigh_vid_select(bat_priv, softif_neigh, vid);

out:
	kfree_skb(skb);
	if (softif_neigh)
		softif_neigh_free_ref(softif_neigh);
	if (curr_softif_neigh)
		softif_neigh_free_ref(curr_softif_neigh);
	if (primary_if)
		hardif_free_ref(primary_if);
	return;
}

static int interface_open(struct net_device *dev)
=======
static int batadv_interface_open(struct net_device *dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	netif_start_queue(dev);
	return 0;
}

<<<<<<< HEAD
static int interface_release(struct net_device *dev)
=======
static int batadv_interface_release(struct net_device *dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	netif_stop_queue(dev);
	return 0;
}

<<<<<<< HEAD
static struct net_device_stats *interface_stats(struct net_device *dev)
{
	struct bat_priv *bat_priv = netdev_priv(dev);
	return &bat_priv->stats;
}

static int interface_set_mac_addr(struct net_device *dev, void *p)
{
	struct bat_priv *bat_priv = netdev_priv(dev);
	struct sockaddr *addr = p;
=======
/**
 * batadv_sum_counter() - Sum the cpu-local counters for index 'idx'
 * @bat_priv: the bat priv with all the soft interface information
 * @idx: index of counter to sum up
 *
 * Return: sum of all cpu-local counters
 */
static u64 batadv_sum_counter(struct batadv_priv *bat_priv,  size_t idx)
{
	u64 *counters, sum = 0;
	int cpu;

	for_each_possible_cpu(cpu) {
		counters = per_cpu_ptr(bat_priv->bat_counters, cpu);
		sum += counters[idx];
	}

	return sum;
}

static struct net_device_stats *batadv_interface_stats(struct net_device *dev)
{
	struct batadv_priv *bat_priv = netdev_priv(dev);
	struct net_device_stats *stats = &dev->stats;

	stats->tx_packets = batadv_sum_counter(bat_priv, BATADV_CNT_TX);
	stats->tx_bytes = batadv_sum_counter(bat_priv, BATADV_CNT_TX_BYTES);
	stats->tx_dropped = batadv_sum_counter(bat_priv, BATADV_CNT_TX_DROPPED);
	stats->rx_packets = batadv_sum_counter(bat_priv, BATADV_CNT_RX);
	stats->rx_bytes = batadv_sum_counter(bat_priv, BATADV_CNT_RX_BYTES);
	return stats;
}

static int batadv_interface_set_mac_addr(struct net_device *dev, void *p)
{
	struct batadv_priv *bat_priv = netdev_priv(dev);
	struct batadv_softif_vlan *vlan;
	struct sockaddr *addr = p;
	u8 old_addr[ETH_ALEN];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!is_valid_ether_addr(addr->sa_data))
		return -EADDRNOTAVAIL;

<<<<<<< HEAD
	/* only modify transtable if it has been initialized before */
	if (atomic_read(&bat_priv->mesh_state) == MESH_ACTIVE) {
		tt_local_remove(bat_priv, dev->dev_addr,
				"mac address changed", false);
		tt_local_add(dev, addr->sa_data, NULL_IFINDEX);
	}

	memcpy(dev->dev_addr, addr->sa_data, ETH_ALEN);
	dev->addr_assign_type &= ~NET_ADDR_RANDOM;
	return 0;
}

static int interface_change_mtu(struct net_device *dev, int new_mtu)
{
	/* check ranges */
	if ((new_mtu < 68) || (new_mtu > hardif_min_mtu(dev)))
		return -EINVAL;

	dev->mtu = new_mtu;
=======
	ether_addr_copy(old_addr, dev->dev_addr);
	eth_hw_addr_set(dev, addr->sa_data);

	/* only modify transtable if it has been initialized before */
	if (atomic_read(&bat_priv->mesh_state) != BATADV_MESH_ACTIVE)
		return 0;

	rcu_read_lock();
	hlist_for_each_entry_rcu(vlan, &bat_priv->softif_vlan_list, list) {
		batadv_tt_local_remove(bat_priv, old_addr, vlan->vid,
				       "mac address changed", false);
		batadv_tt_local_add(dev, addr->sa_data, vlan->vid,
				    BATADV_NULL_IFINDEX, BATADV_NO_MARK);
	}
	rcu_read_unlock();

	return 0;
}

static int batadv_interface_change_mtu(struct net_device *dev, int new_mtu)
{
	struct batadv_priv *bat_priv = netdev_priv(dev);

	/* check ranges */
	if (new_mtu < ETH_MIN_MTU || new_mtu > batadv_hardif_min_mtu(dev))
		return -EINVAL;

	dev->mtu = new_mtu;
	bat_priv->mtu_set_by_user = new_mtu;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static int interface_tx(struct sk_buff *skb, struct net_device *soft_iface)
{
	struct ethhdr *ethhdr = (struct ethhdr *)skb->data;
	struct bat_priv *bat_priv = netdev_priv(soft_iface);
	struct hard_iface *primary_if = NULL;
	struct bcast_packet *bcast_packet;
	struct vlan_ethhdr *vhdr;
	struct softif_neigh *curr_softif_neigh = NULL;
	unsigned int header_len = 0;
	int data_len = skb->len, ret;
	short vid = -1;
	bool do_bcast = false;

	if (atomic_read(&bat_priv->mesh_state) != MESH_ACTIVE)
		goto dropped;

	soft_iface->trans_start = jiffies;

	switch (ntohs(ethhdr->h_proto)) {
	case ETH_P_8021Q:
		vhdr = (struct vlan_ethhdr *)skb->data;
		vid = ntohs(vhdr->h_vlan_TCI) & VLAN_VID_MASK;

		if (ntohs(vhdr->h_vlan_encapsulated_proto) != ETH_P_BATMAN)
			break;

		/* fall through */
	case ETH_P_BATMAN:
		softif_batman_recv(skb, soft_iface, vid);
		goto end;
	}

	/**
	 * if we have a another chosen mesh exit node in range
	 * it will transport the packets to the mesh
	 */
	curr_softif_neigh = softif_neigh_vid_get_selected(bat_priv, vid);
	if (curr_softif_neigh)
		goto dropped;

	/* Register the client MAC in the transtable */
	tt_local_add(soft_iface, ethhdr->h_source, skb->skb_iif);

	if (is_multicast_ether_addr(ethhdr->h_dest)) {
		do_bcast = true;

		switch (atomic_read(&bat_priv->gw_mode)) {
		case GW_MODE_SERVER:
			/* gateway servers should not send dhcp
			 * requests into the mesh */
			ret = gw_is_dhcp_target(skb, &header_len);
			if (ret)
				goto dropped;
			break;
		case GW_MODE_CLIENT:
			/* gateway clients should send dhcp requests
			 * via unicast to their gateway */
			ret = gw_is_dhcp_target(skb, &header_len);
			if (ret)
				do_bcast = false;
			break;
		case GW_MODE_OFF:
		default:
			break;
		}
	}

	/* ethernet packet should be broadcasted */
	if (do_bcast) {
		primary_if = primary_if_get_selected(bat_priv);
		if (!primary_if)
			goto dropped;

		if (my_skb_head_push(skb, sizeof(*bcast_packet)) < 0)
			goto dropped;

		bcast_packet = (struct bcast_packet *)skb->data;
		bcast_packet->header.version = COMPAT_VERSION;
		bcast_packet->header.ttl = TTL;

		/* batman packet type: broadcast */
		bcast_packet->header.packet_type = BAT_BCAST;

		/* hw address of first interface is the orig mac because only
		 * this mac is known throughout the mesh */
		memcpy(bcast_packet->orig,
		       primary_if->net_dev->dev_addr, ETH_ALEN);

		/* set broadcast sequence number */
		bcast_packet->seqno =
			htonl(atomic_inc_return(&bat_priv->bcast_seqno));

		add_bcast_packet_to_list(bat_priv, skb, 1);

		/* a copy is stored in the bcast list, therefore removing
		 * the original skb. */
		kfree_skb(skb);

	/* unicast packet */
	} else {
		if (atomic_read(&bat_priv->gw_mode) != GW_MODE_OFF) {
			ret = gw_out_of_range(bat_priv, skb, ethhdr);
			if (ret)
				goto dropped;
		}

		ret = unicast_send_skb(skb, bat_priv);
		if (ret != 0)
			goto dropped_freed;
	}

	bat_priv->stats.tx_packets++;
	bat_priv->stats.tx_bytes += data_len;
=======
/**
 * batadv_interface_set_rx_mode() - set the rx mode of a device
 * @dev: registered network device to modify
 *
 * We do not actually need to set any rx filters for the virtual batman
 * soft interface. However a dummy handler enables a user to set static
 * multicast listeners for instance.
 */
static void batadv_interface_set_rx_mode(struct net_device *dev)
{
}

static netdev_tx_t batadv_interface_tx(struct sk_buff *skb,
				       struct net_device *soft_iface)
{
	struct ethhdr *ethhdr;
	struct batadv_priv *bat_priv = netdev_priv(soft_iface);
	struct batadv_hard_iface *primary_if = NULL;
	struct batadv_bcast_packet *bcast_packet;
	static const u8 stp_addr[ETH_ALEN] = {0x01, 0x80, 0xC2, 0x00,
					      0x00, 0x00};
	static const u8 ectp_addr[ETH_ALEN] = {0xCF, 0x00, 0x00, 0x00,
					       0x00, 0x00};
	enum batadv_dhcp_recipient dhcp_rcp = BATADV_DHCP_NO;
	u8 *dst_hint = NULL, chaddr[ETH_ALEN];
	struct vlan_ethhdr *vhdr;
	unsigned int header_len = 0;
	int data_len = skb->len, ret;
	unsigned long brd_delay = 0;
	bool do_bcast = false, client_added;
	unsigned short vid;
	u32 seqno;
	int gw_mode;
	enum batadv_forw_mode forw_mode = BATADV_FORW_BCAST;
	int mcast_is_routable = 0;
	int network_offset = ETH_HLEN;
	__be16 proto;

	if (atomic_read(&bat_priv->mesh_state) != BATADV_MESH_ACTIVE)
		goto dropped;

	/* reset control block to avoid left overs from previous users */
	memset(skb->cb, 0, sizeof(struct batadv_skb_cb));

	netif_trans_update(soft_iface);
	vid = batadv_get_vid(skb, 0);

	skb_reset_mac_header(skb);
	ethhdr = eth_hdr(skb);

	proto = ethhdr->h_proto;

	switch (ntohs(proto)) {
	case ETH_P_8021Q:
		if (!pskb_may_pull(skb, sizeof(*vhdr)))
			goto dropped;
		vhdr = vlan_eth_hdr(skb);
		proto = vhdr->h_vlan_encapsulated_proto;

		/* drop batman-in-batman packets to prevent loops */
		if (proto != htons(ETH_P_BATMAN)) {
			network_offset += VLAN_HLEN;
			break;
		}

		fallthrough;
	case ETH_P_BATMAN:
		goto dropped;
	}

	skb_set_network_header(skb, network_offset);

	if (batadv_bla_tx(bat_priv, skb, vid))
		goto dropped;

	/* skb->data might have been reallocated by batadv_bla_tx() */
	ethhdr = eth_hdr(skb);

	/* Register the client MAC in the transtable */
	if (!is_multicast_ether_addr(ethhdr->h_source) &&
	    !batadv_bla_is_loopdetect_mac(ethhdr->h_source)) {
		client_added = batadv_tt_local_add(soft_iface, ethhdr->h_source,
						   vid, skb->skb_iif,
						   skb->mark);
		if (!client_added)
			goto dropped;
	}

	/* Snoop address candidates from DHCPACKs for early DAT filling */
	batadv_dat_snoop_outgoing_dhcp_ack(bat_priv, skb, proto, vid);

	/* don't accept stp packets. STP does not help in meshes.
	 * better use the bridge loop avoidance ...
	 *
	 * The same goes for ECTP sent at least by some Cisco Switches,
	 * it might confuse the mesh when used with bridge loop avoidance.
	 */
	if (batadv_compare_eth(ethhdr->h_dest, stp_addr))
		goto dropped;

	if (batadv_compare_eth(ethhdr->h_dest, ectp_addr))
		goto dropped;

	gw_mode = atomic_read(&bat_priv->gw.mode);
	if (is_multicast_ether_addr(ethhdr->h_dest)) {
		/* if gw mode is off, broadcast every packet */
		if (gw_mode == BATADV_GW_MODE_OFF) {
			do_bcast = true;
			goto send;
		}

		dhcp_rcp = batadv_gw_dhcp_recipient_get(skb, &header_len,
							chaddr);
		/* skb->data may have been modified by
		 * batadv_gw_dhcp_recipient_get()
		 */
		ethhdr = eth_hdr(skb);
		/* if gw_mode is on, broadcast any non-DHCP message.
		 * All the DHCP packets are going to be sent as unicast
		 */
		if (dhcp_rcp == BATADV_DHCP_NO) {
			do_bcast = true;
			goto send;
		}

		if (dhcp_rcp == BATADV_DHCP_TO_CLIENT)
			dst_hint = chaddr;
		else if ((gw_mode == BATADV_GW_MODE_SERVER) &&
			 (dhcp_rcp == BATADV_DHCP_TO_SERVER))
			/* gateways should not forward any DHCP message if
			 * directed to a DHCP server
			 */
			goto dropped;

send:
		if (do_bcast && !is_broadcast_ether_addr(ethhdr->h_dest)) {
			forw_mode = batadv_mcast_forw_mode(bat_priv, skb, vid,
							   &mcast_is_routable);
			switch (forw_mode) {
			case BATADV_FORW_BCAST:
				break;
			case BATADV_FORW_UCASTS:
			case BATADV_FORW_MCAST:
				do_bcast = false;
				break;
			case BATADV_FORW_NONE:
				fallthrough;
			default:
				goto dropped;
			}
		}
	}

	batadv_skb_set_priority(skb, 0);

	/* ethernet packet should be broadcasted */
	if (do_bcast) {
		primary_if = batadv_primary_if_get_selected(bat_priv);
		if (!primary_if)
			goto dropped;

		/* in case of ARP request, we do not immediately broadcasti the
		 * packet, instead we first wait for DAT to try to retrieve the
		 * correct ARP entry
		 */
		if (batadv_dat_snoop_outgoing_arp_request(bat_priv, skb))
			brd_delay = msecs_to_jiffies(ARP_REQ_DELAY);

		if (batadv_skb_head_push(skb, sizeof(*bcast_packet)) < 0)
			goto dropped;

		bcast_packet = (struct batadv_bcast_packet *)skb->data;
		bcast_packet->version = BATADV_COMPAT_VERSION;
		bcast_packet->ttl = BATADV_TTL - 1;

		/* batman packet type: broadcast */
		bcast_packet->packet_type = BATADV_BCAST;
		bcast_packet->reserved = 0;

		/* hw address of first interface is the orig mac because only
		 * this mac is known throughout the mesh
		 */
		ether_addr_copy(bcast_packet->orig,
				primary_if->net_dev->dev_addr);

		/* set broadcast sequence number */
		seqno = atomic_inc_return(&bat_priv->bcast_seqno);
		bcast_packet->seqno = htonl(seqno);

		batadv_send_bcast_packet(bat_priv, skb, brd_delay, true);
	/* unicast packet */
	} else {
		/* DHCP packets going to a server will use the GW feature */
		if (dhcp_rcp == BATADV_DHCP_TO_SERVER) {
			ret = batadv_gw_out_of_range(bat_priv, skb);
			if (ret)
				goto dropped;
			ret = batadv_send_skb_via_gw(bat_priv, skb, vid);
		} else if (forw_mode == BATADV_FORW_UCASTS) {
			ret = batadv_mcast_forw_send(bat_priv, skb, vid,
						     mcast_is_routable);
		} else if (forw_mode == BATADV_FORW_MCAST) {
			ret = batadv_mcast_forw_mcsend(bat_priv, skb);
		} else {
			if (batadv_dat_snoop_outgoing_arp_request(bat_priv,
								  skb))
				goto dropped;

			batadv_dat_snoop_outgoing_arp_reply(bat_priv, skb);

			ret = batadv_send_skb_via_tt(bat_priv, skb, dst_hint,
						     vid);
		}
		if (ret != NET_XMIT_SUCCESS)
			goto dropped_freed;
	}

	batadv_inc_counter(bat_priv, BATADV_CNT_TX);
	batadv_add_counter(bat_priv, BATADV_CNT_TX_BYTES, data_len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	goto end;

dropped:
	kfree_skb(skb);
dropped_freed:
<<<<<<< HEAD
	bat_priv->stats.tx_dropped++;
end:
	if (curr_softif_neigh)
		softif_neigh_free_ref(curr_softif_neigh);
	if (primary_if)
		hardif_free_ref(primary_if);
	return NETDEV_TX_OK;
}

void interface_rx(struct net_device *soft_iface,
		  struct sk_buff *skb, struct hard_iface *recv_if,
		  int hdr_size)
{
	struct bat_priv *bat_priv = netdev_priv(soft_iface);
	struct unicast_packet *unicast_packet;
	struct ethhdr *ethhdr;
	struct vlan_ethhdr *vhdr;
	struct softif_neigh *curr_softif_neigh = NULL;
	short vid = -1;
	int ret;

	/* check if enough space is available for pulling, and pull */
	if (!pskb_may_pull(skb, hdr_size))
		goto dropped;
=======
	batadv_inc_counter(bat_priv, BATADV_CNT_TX_DROPPED);
end:
	batadv_hardif_put(primary_if);
	return NETDEV_TX_OK;
}

/**
 * batadv_interface_rx() - receive ethernet frame on local batman-adv interface
 * @soft_iface: local interface which will receive the ethernet frame
 * @skb: ethernet frame for @soft_iface
 * @hdr_size: size of already parsed batman-adv header
 * @orig_node: originator from which the batman-adv packet was sent
 *
 * Sends an ethernet frame to the receive path of the local @soft_iface.
 * skb->data has still point to the batman-adv header with the size @hdr_size.
 * The caller has to have parsed this header already and made sure that at least
 * @hdr_size bytes are still available for pull in @skb.
 *
 * The packet may still get dropped. This can happen when the encapsulated
 * ethernet frame is invalid or contains again an batman-adv packet. Also
 * unicast packets will be dropped directly when it was sent between two
 * isolated clients.
 */
void batadv_interface_rx(struct net_device *soft_iface,
			 struct sk_buff *skb, int hdr_size,
			 struct batadv_orig_node *orig_node)
{
	struct batadv_bcast_packet *batadv_bcast_packet;
	struct batadv_priv *bat_priv = netdev_priv(soft_iface);
	struct vlan_ethhdr *vhdr;
	struct ethhdr *ethhdr;
	unsigned short vid;
	int packet_type;

	batadv_bcast_packet = (struct batadv_bcast_packet *)skb->data;
	packet_type = batadv_bcast_packet->packet_type;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	skb_pull_rcsum(skb, hdr_size);
	skb_reset_mac_header(skb);

<<<<<<< HEAD
	ethhdr = (struct ethhdr *)skb_mac_header(skb);

	switch (ntohs(ethhdr->h_proto)) {
	case ETH_P_8021Q:
		vhdr = (struct vlan_ethhdr *)skb->data;
		vid = ntohs(vhdr->h_vlan_TCI) & VLAN_VID_MASK;

		if (ntohs(vhdr->h_vlan_encapsulated_proto) != ETH_P_BATMAN)
			break;

		/* fall through */
=======
	/* clean the netfilter state now that the batman-adv header has been
	 * removed
	 */
	nf_reset_ct(skb);

	if (unlikely(!pskb_may_pull(skb, ETH_HLEN)))
		goto dropped;

	vid = batadv_get_vid(skb, 0);
	ethhdr = eth_hdr(skb);

	switch (ntohs(ethhdr->h_proto)) {
	case ETH_P_8021Q:
		if (!pskb_may_pull(skb, VLAN_ETH_HLEN))
			goto dropped;

		vhdr = skb_vlan_eth_hdr(skb);

		/* drop batman-in-batman packets to prevent loops */
		if (vhdr->h_vlan_encapsulated_proto != htons(ETH_P_BATMAN))
			break;

		fallthrough;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case ETH_P_BATMAN:
		goto dropped;
	}

<<<<<<< HEAD
	/**
	 * if we have a another chosen mesh exit node in range
	 * it will transport the packets to the non-mesh network
	 */
	curr_softif_neigh = softif_neigh_vid_get_selected(bat_priv, vid);
	if (curr_softif_neigh) {
		skb_push(skb, hdr_size);
		unicast_packet = (struct unicast_packet *)skb->data;

		if ((unicast_packet->header.packet_type != BAT_UNICAST) &&
		    (unicast_packet->header.packet_type != BAT_UNICAST_FRAG))
			goto dropped;

		skb_reset_mac_header(skb);

		memcpy(unicast_packet->dest,
		       curr_softif_neigh->addr, ETH_ALEN);
		ret = route_unicast_packet(skb, recv_if);
		if (ret == NET_RX_DROP)
			goto dropped;

		goto out;
	}

	/* skb->dev & skb->pkt_type are set here */
	if (unlikely(!pskb_may_pull(skb, ETH_HLEN)))
		goto dropped;
	skb->protocol = eth_type_trans(skb, soft_iface);

	/* should not be necessary anymore as we use skb_pull_rcsum()
	 * TODO: please verify this and remove this TODO
	 * -- Dec 21st 2009, Simon Wunderlich */

/*	skb->ip_summed = CHECKSUM_UNNECESSARY;*/

	bat_priv->stats.rx_packets++;
	bat_priv->stats.rx_bytes += skb->len + sizeof(struct ethhdr);

	soft_iface->last_rx = jiffies;

	if (is_ap_isolated(bat_priv, ethhdr->h_source, ethhdr->h_dest))
		goto dropped;
=======
	/* skb->dev & skb->pkt_type are set here */
	skb->protocol = eth_type_trans(skb, soft_iface);
	skb_postpull_rcsum(skb, eth_hdr(skb), ETH_HLEN);

	batadv_inc_counter(bat_priv, BATADV_CNT_RX);
	batadv_add_counter(bat_priv, BATADV_CNT_RX_BYTES,
			   skb->len + ETH_HLEN);

	/* Let the bridge loop avoidance check the packet. If will
	 * not handle it, we can safely push it up.
	 */
	if (batadv_bla_rx(bat_priv, skb, vid, packet_type))
		goto out;

	if (orig_node)
		batadv_tt_add_temporary_global_entry(bat_priv, orig_node,
						     ethhdr->h_source, vid);

	if (is_multicast_ether_addr(ethhdr->h_dest)) {
		/* set the mark on broadcast packets if AP isolation is ON and
		 * the packet is coming from an "isolated" client
		 */
		if (batadv_vlan_ap_isola_get(bat_priv, vid) &&
		    batadv_tt_global_is_isolated(bat_priv, ethhdr->h_source,
						 vid)) {
			/* save bits in skb->mark not covered by the mask and
			 * apply the mark on the rest
			 */
			skb->mark &= ~bat_priv->isolation_mark_mask;
			skb->mark |= bat_priv->isolation_mark;
		}
	} else if (batadv_is_ap_isolated(bat_priv, ethhdr->h_source,
					 ethhdr->h_dest, vid)) {
		goto dropped;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	netif_rx(skb);
	goto out;

dropped:
	kfree_skb(skb);
out:
<<<<<<< HEAD
	if (curr_softif_neigh)
		softif_neigh_free_ref(curr_softif_neigh);
	return;
}

static const struct net_device_ops bat_netdev_ops = {
	.ndo_open = interface_open,
	.ndo_stop = interface_release,
	.ndo_get_stats = interface_stats,
	.ndo_set_mac_address = interface_set_mac_addr,
	.ndo_change_mtu = interface_change_mtu,
	.ndo_start_xmit = interface_tx,
	.ndo_validate_addr = eth_validate_addr
};

static void interface_setup(struct net_device *dev)
{
	struct bat_priv *priv = netdev_priv(dev);

	ether_setup(dev);

	dev->netdev_ops = &bat_netdev_ops;
	dev->destructor = free_netdev;
	dev->tx_queue_len = 0;

	/**
	 * can't call min_mtu, because the needed variables
	 * have not been initialized yet
	 */
	dev->mtu = ETH_DATA_LEN;
	/* reserve more space in the skbuff for our header */
	dev->hard_header_len = BAT_HEADER_LEN;
=======
	return;
}

/**
 * batadv_softif_vlan_release() - release vlan from lists and queue for free
 *  after rcu grace period
 * @ref: kref pointer of the vlan object
 */
void batadv_softif_vlan_release(struct kref *ref)
{
	struct batadv_softif_vlan *vlan;

	vlan = container_of(ref, struct batadv_softif_vlan, refcount);

	spin_lock_bh(&vlan->bat_priv->softif_vlan_list_lock);
	hlist_del_rcu(&vlan->list);
	spin_unlock_bh(&vlan->bat_priv->softif_vlan_list_lock);

	kfree_rcu(vlan, rcu);
}

/**
 * batadv_softif_vlan_get() - get the vlan object for a specific vid
 * @bat_priv: the bat priv with all the soft interface information
 * @vid: the identifier of the vlan object to retrieve
 *
 * Return: the private data of the vlan matching the vid passed as argument or
 * NULL otherwise. The refcounter of the returned object is incremented by 1.
 */
struct batadv_softif_vlan *batadv_softif_vlan_get(struct batadv_priv *bat_priv,
						  unsigned short vid)
{
	struct batadv_softif_vlan *vlan_tmp, *vlan = NULL;

	rcu_read_lock();
	hlist_for_each_entry_rcu(vlan_tmp, &bat_priv->softif_vlan_list, list) {
		if (vlan_tmp->vid != vid)
			continue;

		if (!kref_get_unless_zero(&vlan_tmp->refcount))
			continue;

		vlan = vlan_tmp;
		break;
	}
	rcu_read_unlock();

	return vlan;
}

/**
 * batadv_softif_create_vlan() - allocate the needed resources for a new vlan
 * @bat_priv: the bat priv with all the soft interface information
 * @vid: the VLAN identifier
 *
 * Return: 0 on success, a negative error otherwise.
 */
int batadv_softif_create_vlan(struct batadv_priv *bat_priv, unsigned short vid)
{
	struct batadv_softif_vlan *vlan;

	spin_lock_bh(&bat_priv->softif_vlan_list_lock);

	vlan = batadv_softif_vlan_get(bat_priv, vid);
	if (vlan) {
		batadv_softif_vlan_put(vlan);
		spin_unlock_bh(&bat_priv->softif_vlan_list_lock);
		return -EEXIST;
	}

	vlan = kzalloc(sizeof(*vlan), GFP_ATOMIC);
	if (!vlan) {
		spin_unlock_bh(&bat_priv->softif_vlan_list_lock);
		return -ENOMEM;
	}

	vlan->bat_priv = bat_priv;
	vlan->vid = vid;
	kref_init(&vlan->refcount);

	atomic_set(&vlan->ap_isolation, 0);

	kref_get(&vlan->refcount);
	hlist_add_head_rcu(&vlan->list, &bat_priv->softif_vlan_list);
	spin_unlock_bh(&bat_priv->softif_vlan_list_lock);

	/* add a new TT local entry. This one will be marked with the NOPURGE
	 * flag
	 */
	batadv_tt_local_add(bat_priv->soft_iface,
			    bat_priv->soft_iface->dev_addr, vid,
			    BATADV_NULL_IFINDEX, BATADV_NO_MARK);

	/* don't return reference to new softif_vlan */
	batadv_softif_vlan_put(vlan);

	return 0;
}

/**
 * batadv_softif_destroy_vlan() - remove and destroy a softif_vlan object
 * @bat_priv: the bat priv with all the soft interface information
 * @vlan: the object to remove
 */
static void batadv_softif_destroy_vlan(struct batadv_priv *bat_priv,
				       struct batadv_softif_vlan *vlan)
{
	/* explicitly remove the associated TT local entry because it is marked
	 * with the NOPURGE flag
	 */
	batadv_tt_local_remove(bat_priv, bat_priv->soft_iface->dev_addr,
			       vlan->vid, "vlan interface destroyed", false);

	batadv_softif_vlan_put(vlan);
}

/**
 * batadv_interface_add_vid() - ndo_add_vid API implementation
 * @dev: the netdev of the mesh interface
 * @proto: protocol of the vlan id
 * @vid: identifier of the new vlan
 *
 * Set up all the internal structures for handling the new vlan on top of the
 * mesh interface
 *
 * Return: 0 on success or a negative error code in case of failure.
 */
static int batadv_interface_add_vid(struct net_device *dev, __be16 proto,
				    unsigned short vid)
{
	struct batadv_priv *bat_priv = netdev_priv(dev);
	struct batadv_softif_vlan *vlan;

	/* only 802.1Q vlans are supported.
	 * batman-adv does not know how to handle other types
	 */
	if (proto != htons(ETH_P_8021Q))
		return -EINVAL;

	vid |= BATADV_VLAN_HAS_TAG;

	/* if a new vlan is getting created and it already exists, it means that
	 * it was not deleted yet. batadv_softif_vlan_get() increases the
	 * refcount in order to revive the object.
	 *
	 * if it does not exist then create it.
	 */
	vlan = batadv_softif_vlan_get(bat_priv, vid);
	if (!vlan)
		return batadv_softif_create_vlan(bat_priv, vid);

	/* add a new TT local entry. This one will be marked with the NOPURGE
	 * flag. This must be added again, even if the vlan object already
	 * exists, because the entry was deleted by kill_vid()
	 */
	batadv_tt_local_add(bat_priv->soft_iface,
			    bat_priv->soft_iface->dev_addr, vid,
			    BATADV_NULL_IFINDEX, BATADV_NO_MARK);

	return 0;
}

/**
 * batadv_interface_kill_vid() - ndo_kill_vid API implementation
 * @dev: the netdev of the mesh interface
 * @proto: protocol of the vlan id
 * @vid: identifier of the deleted vlan
 *
 * Destroy all the internal structures used to handle the vlan identified by vid
 * on top of the mesh interface
 *
 * Return: 0 on success, -EINVAL if the specified prototype is not ETH_P_8021Q
 * or -ENOENT if the specified vlan id wasn't registered.
 */
static int batadv_interface_kill_vid(struct net_device *dev, __be16 proto,
				     unsigned short vid)
{
	struct batadv_priv *bat_priv = netdev_priv(dev);
	struct batadv_softif_vlan *vlan;

	/* only 802.1Q vlans are supported. batman-adv does not know how to
	 * handle other types
	 */
	if (proto != htons(ETH_P_8021Q))
		return -EINVAL;

	vlan = batadv_softif_vlan_get(bat_priv, vid | BATADV_VLAN_HAS_TAG);
	if (!vlan)
		return -ENOENT;

	batadv_softif_destroy_vlan(bat_priv, vlan);

	/* finally free the vlan object */
	batadv_softif_vlan_put(vlan);

	return 0;
}

/* batman-adv network devices have devices nesting below it and are a special
 * "super class" of normal network devices; split their locks off into a
 * separate class since they always nest.
 */
static struct lock_class_key batadv_netdev_xmit_lock_key;
static struct lock_class_key batadv_netdev_addr_lock_key;

/**
 * batadv_set_lockdep_class_one() - Set lockdep class for a single tx queue
 * @dev: device which owns the tx queue
 * @txq: tx queue to modify
 * @_unused: always NULL
 */
static void batadv_set_lockdep_class_one(struct net_device *dev,
					 struct netdev_queue *txq,
					 void *_unused)
{
	lockdep_set_class(&txq->_xmit_lock, &batadv_netdev_xmit_lock_key);
}

/**
 * batadv_set_lockdep_class() - Set txq and addr_list lockdep class
 * @dev: network device to modify
 */
static void batadv_set_lockdep_class(struct net_device *dev)
{
	lockdep_set_class(&dev->addr_list_lock, &batadv_netdev_addr_lock_key);
	netdev_for_each_tx_queue(dev, batadv_set_lockdep_class_one, NULL);
}

/**
 * batadv_softif_init_late() - late stage initialization of soft interface
 * @dev: registered network device to modify
 *
 * Return: error code on failures
 */
static int batadv_softif_init_late(struct net_device *dev)
{
	struct batadv_priv *bat_priv;
	u32 random_seqno;
	int ret;
	size_t cnt_len = sizeof(u64) * BATADV_CNT_NUM;

	batadv_set_lockdep_class(dev);

	bat_priv = netdev_priv(dev);
	bat_priv->soft_iface = dev;

	/* batadv_interface_stats() needs to be available as soon as
	 * register_netdevice() has been called
	 */
	bat_priv->bat_counters = __alloc_percpu(cnt_len, __alignof__(u64));
	if (!bat_priv->bat_counters)
		return -ENOMEM;

	atomic_set(&bat_priv->aggregated_ogms, 1);
	atomic_set(&bat_priv->bonding, 0);
#ifdef CONFIG_BATMAN_ADV_BLA
	atomic_set(&bat_priv->bridge_loop_avoidance, 1);
#endif
#ifdef CONFIG_BATMAN_ADV_DAT
	atomic_set(&bat_priv->distributed_arp_table, 1);
#endif
#ifdef CONFIG_BATMAN_ADV_MCAST
	atomic_set(&bat_priv->multicast_mode, 1);
	atomic_set(&bat_priv->multicast_fanout, 16);
	atomic_set(&bat_priv->mcast.num_want_all_unsnoopables, 0);
	atomic_set(&bat_priv->mcast.num_want_all_ipv4, 0);
	atomic_set(&bat_priv->mcast.num_want_all_ipv6, 0);
	atomic_set(&bat_priv->mcast.num_no_mc_ptype_capa, 0);
#endif
	atomic_set(&bat_priv->gw.mode, BATADV_GW_MODE_OFF);
	atomic_set(&bat_priv->gw.bandwidth_down, 100);
	atomic_set(&bat_priv->gw.bandwidth_up, 20);
	atomic_set(&bat_priv->orig_interval, 1000);
	atomic_set(&bat_priv->hop_penalty, 30);
#ifdef CONFIG_BATMAN_ADV_DEBUG
	atomic_set(&bat_priv->log_level, 0);
#endif
	atomic_set(&bat_priv->fragmentation, 1);
	atomic_set(&bat_priv->packet_size_max, ETH_DATA_LEN);
	atomic_set(&bat_priv->bcast_queue_left, BATADV_BCAST_QUEUE_LEN);
	atomic_set(&bat_priv->batman_queue_left, BATADV_BATMAN_QUEUE_LEN);

	atomic_set(&bat_priv->mesh_state, BATADV_MESH_INACTIVE);
	atomic_set(&bat_priv->bcast_seqno, 1);
	atomic_set(&bat_priv->tt.vn, 0);
	atomic_set(&bat_priv->tt.local_changes, 0);
	atomic_set(&bat_priv->tt.ogm_append_cnt, 0);
#ifdef CONFIG_BATMAN_ADV_BLA
	atomic_set(&bat_priv->bla.num_requests, 0);
#endif
	atomic_set(&bat_priv->tp_num, 0);

	bat_priv->tt.last_changeset = NULL;
	bat_priv->tt.last_changeset_len = 0;
	bat_priv->isolation_mark = 0;
	bat_priv->isolation_mark_mask = 0;

	/* randomize initial seqno to avoid collision */
	get_random_bytes(&random_seqno, sizeof(random_seqno));
	atomic_set(&bat_priv->frag_seqno, random_seqno);

	bat_priv->primary_if = NULL;

	batadv_nc_init_bat_priv(bat_priv);

	if (!bat_priv->algo_ops) {
		ret = batadv_algo_select(bat_priv, batadv_routing_algo);
		if (ret < 0)
			goto free_bat_counters;
	}

	ret = batadv_mesh_init(dev);
	if (ret < 0)
		goto free_bat_counters;

	return 0;

free_bat_counters:
	free_percpu(bat_priv->bat_counters);
	bat_priv->bat_counters = NULL;

	return ret;
}

/**
 * batadv_softif_slave_add() - Add a slave interface to a batadv_soft_interface
 * @dev: batadv_soft_interface used as master interface
 * @slave_dev: net_device which should become the slave interface
 * @extack: extended ACK report struct
 *
 * Return: 0 if successful or error otherwise.
 */
static int batadv_softif_slave_add(struct net_device *dev,
				   struct net_device *slave_dev,
				   struct netlink_ext_ack *extack)
{
	struct batadv_hard_iface *hard_iface;
	int ret = -EINVAL;

	hard_iface = batadv_hardif_get_by_netdev(slave_dev);
	if (!hard_iface || hard_iface->soft_iface)
		goto out;

	ret = batadv_hardif_enable_interface(hard_iface, dev);

out:
	batadv_hardif_put(hard_iface);
	return ret;
}

/**
 * batadv_softif_slave_del() - Delete a slave iface from a batadv_soft_interface
 * @dev: batadv_soft_interface used as master interface
 * @slave_dev: net_device which should be removed from the master interface
 *
 * Return: 0 if successful or error otherwise.
 */
static int batadv_softif_slave_del(struct net_device *dev,
				   struct net_device *slave_dev)
{
	struct batadv_hard_iface *hard_iface;
	int ret = -EINVAL;

	hard_iface = batadv_hardif_get_by_netdev(slave_dev);

	if (!hard_iface || hard_iface->soft_iface != dev)
		goto out;

	batadv_hardif_disable_interface(hard_iface);
	ret = 0;

out:
	batadv_hardif_put(hard_iface);
	return ret;
}

static const struct net_device_ops batadv_netdev_ops = {
	.ndo_init = batadv_softif_init_late,
	.ndo_open = batadv_interface_open,
	.ndo_stop = batadv_interface_release,
	.ndo_get_stats = batadv_interface_stats,
	.ndo_vlan_rx_add_vid = batadv_interface_add_vid,
	.ndo_vlan_rx_kill_vid = batadv_interface_kill_vid,
	.ndo_set_mac_address = batadv_interface_set_mac_addr,
	.ndo_change_mtu = batadv_interface_change_mtu,
	.ndo_set_rx_mode = batadv_interface_set_rx_mode,
	.ndo_start_xmit = batadv_interface_tx,
	.ndo_validate_addr = eth_validate_addr,
	.ndo_add_slave = batadv_softif_slave_add,
	.ndo_del_slave = batadv_softif_slave_del,
};

static void batadv_get_drvinfo(struct net_device *dev,
			       struct ethtool_drvinfo *info)
{
	strscpy(info->driver, "B.A.T.M.A.N. advanced", sizeof(info->driver));
	strscpy(info->version, BATADV_SOURCE_VERSION, sizeof(info->version));
	strscpy(info->fw_version, "N/A", sizeof(info->fw_version));
	strscpy(info->bus_info, "batman", sizeof(info->bus_info));
}

/* Inspired by drivers/net/ethernet/dlink/sundance.c:1702
 * Declare each description string in struct.name[] to get fixed sized buffer
 * and compile time checking for strings longer than ETH_GSTRING_LEN.
 */
static const struct {
	const char name[ETH_GSTRING_LEN];
} batadv_counters_strings[] = {
	{ "tx" },
	{ "tx_bytes" },
	{ "tx_dropped" },
	{ "rx" },
	{ "rx_bytes" },
	{ "forward" },
	{ "forward_bytes" },
	{ "mgmt_tx" },
	{ "mgmt_tx_bytes" },
	{ "mgmt_rx" },
	{ "mgmt_rx_bytes" },
	{ "frag_tx" },
	{ "frag_tx_bytes" },
	{ "frag_rx" },
	{ "frag_rx_bytes" },
	{ "frag_fwd" },
	{ "frag_fwd_bytes" },
	{ "tt_request_tx" },
	{ "tt_request_rx" },
	{ "tt_response_tx" },
	{ "tt_response_rx" },
	{ "tt_roam_adv_tx" },
	{ "tt_roam_adv_rx" },
#ifdef CONFIG_BATMAN_ADV_MCAST
	{ "mcast_tx" },
	{ "mcast_tx_bytes" },
	{ "mcast_tx_local" },
	{ "mcast_tx_local_bytes" },
	{ "mcast_rx" },
	{ "mcast_rx_bytes" },
	{ "mcast_rx_local" },
	{ "mcast_rx_local_bytes" },
	{ "mcast_fwd" },
	{ "mcast_fwd_bytes" },
#endif
#ifdef CONFIG_BATMAN_ADV_DAT
	{ "dat_get_tx" },
	{ "dat_get_rx" },
	{ "dat_put_tx" },
	{ "dat_put_rx" },
	{ "dat_cached_reply_tx" },
#endif
#ifdef CONFIG_BATMAN_ADV_NC
	{ "nc_code" },
	{ "nc_code_bytes" },
	{ "nc_recode" },
	{ "nc_recode_bytes" },
	{ "nc_buffer" },
	{ "nc_decode" },
	{ "nc_decode_bytes" },
	{ "nc_decode_failed" },
	{ "nc_sniffed" },
#endif
};

static void batadv_get_strings(struct net_device *dev, u32 stringset, u8 *data)
{
	if (stringset == ETH_SS_STATS)
		memcpy(data, batadv_counters_strings,
		       sizeof(batadv_counters_strings));
}

static void batadv_get_ethtool_stats(struct net_device *dev,
				     struct ethtool_stats *stats, u64 *data)
{
	struct batadv_priv *bat_priv = netdev_priv(dev);
	int i;

	for (i = 0; i < BATADV_CNT_NUM; i++)
		data[i] = batadv_sum_counter(bat_priv, i);
}

static int batadv_get_sset_count(struct net_device *dev, int stringset)
{
	if (stringset == ETH_SS_STATS)
		return BATADV_CNT_NUM;

	return -EOPNOTSUPP;
}

static const struct ethtool_ops batadv_ethtool_ops = {
	.get_drvinfo = batadv_get_drvinfo,
	.get_link = ethtool_op_get_link,
	.get_strings = batadv_get_strings,
	.get_ethtool_stats = batadv_get_ethtool_stats,
	.get_sset_count = batadv_get_sset_count,
};

/**
 * batadv_softif_free() - Deconstructor of batadv_soft_interface
 * @dev: Device to cleanup and remove
 */
static void batadv_softif_free(struct net_device *dev)
{
	batadv_mesh_free(dev);

	/* some scheduled RCU callbacks need the bat_priv struct to accomplish
	 * their tasks. Wait for them all to be finished before freeing the
	 * netdev and its private data (bat_priv)
	 */
	rcu_barrier();
}

/**
 * batadv_softif_init_early() - early stage initialization of soft interface
 * @dev: registered network device to modify
 */
static void batadv_softif_init_early(struct net_device *dev)
{
	ether_setup(dev);

	dev->netdev_ops = &batadv_netdev_ops;
	dev->needs_free_netdev = true;
	dev->priv_destructor = batadv_softif_free;
	dev->features |= NETIF_F_HW_VLAN_CTAG_FILTER | NETIF_F_NETNS_LOCAL;
	dev->features |= NETIF_F_LLTX;
	dev->priv_flags |= IFF_NO_QUEUE;

	/* can't call min_mtu, because the needed variables
	 * have not been initialized yet
	 */
	dev->mtu = ETH_DATA_LEN;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* generate random address */
	eth_hw_addr_random(dev);

<<<<<<< HEAD
	SET_ETHTOOL_OPS(dev, &bat_ethtool_ops);

	memset(priv, 0, sizeof(*priv));
}

struct net_device *softif_create(const char *name)
{
	struct net_device *soft_iface;
	struct bat_priv *bat_priv;
	int ret;

	soft_iface = alloc_netdev(sizeof(*bat_priv), name, interface_setup);

	if (!soft_iface)
		goto out;

	ret = register_netdevice(soft_iface);
	if (ret < 0) {
		pr_err("Unable to register the batman interface '%s': %i\n",
		       name, ret);
		goto free_soft_iface;
	}

	bat_priv = netdev_priv(soft_iface);

	atomic_set(&bat_priv->aggregated_ogms, 1);
	atomic_set(&bat_priv->bonding, 0);
	atomic_set(&bat_priv->ap_isolation, 0);
	atomic_set(&bat_priv->vis_mode, VIS_TYPE_CLIENT_UPDATE);
	atomic_set(&bat_priv->gw_mode, GW_MODE_OFF);
	atomic_set(&bat_priv->gw_sel_class, 20);
	atomic_set(&bat_priv->gw_bandwidth, 41);
	atomic_set(&bat_priv->orig_interval, 1000);
	atomic_set(&bat_priv->hop_penalty, 10);
	atomic_set(&bat_priv->log_level, 0);
	atomic_set(&bat_priv->fragmentation, 1);
	atomic_set(&bat_priv->bcast_queue_left, BCAST_QUEUE_LEN);
	atomic_set(&bat_priv->batman_queue_left, BATMAN_QUEUE_LEN);

	atomic_set(&bat_priv->mesh_state, MESH_INACTIVE);
	atomic_set(&bat_priv->bcast_seqno, 1);
	atomic_set(&bat_priv->ttvn, 0);
	atomic_set(&bat_priv->tt_local_changes, 0);
	atomic_set(&bat_priv->tt_ogm_append_cnt, 0);

	bat_priv->tt_buff = NULL;
	bat_priv->tt_buff_len = 0;
	bat_priv->tt_poss_change = false;

	bat_priv->primary_if = NULL;
	bat_priv->num_ifaces = 0;

	ret = bat_algo_select(bat_priv, bat_routing_algo);
	if (ret < 0)
		goto unreg_soft_iface;

	ret = sysfs_add_meshif(soft_iface);
	if (ret < 0)
		goto unreg_soft_iface;

	ret = debugfs_add_meshif(soft_iface);
	if (ret < 0)
		goto unreg_sysfs;

	ret = mesh_init(soft_iface);
	if (ret < 0)
		goto unreg_debugfs;

	return soft_iface;

unreg_debugfs:
	debugfs_del_meshif(soft_iface);
unreg_sysfs:
	sysfs_del_meshif(soft_iface);
unreg_soft_iface:
	unregister_netdevice(soft_iface);
	return NULL;

free_soft_iface:
	free_netdev(soft_iface);
out:
	return NULL;
}

void softif_destroy(struct net_device *soft_iface)
{
	debugfs_del_meshif(soft_iface);
	sysfs_del_meshif(soft_iface);
	mesh_free(soft_iface);
	unregister_netdevice(soft_iface);
}

int softif_is_valid(const struct net_device *net_dev)
{
	if (net_dev->netdev_ops->ndo_start_xmit == interface_tx)
		return 1;

	return 0;
}

/* ethtool */
static int bat_get_settings(struct net_device *dev, struct ethtool_cmd *cmd)
{
	cmd->supported = 0;
	cmd->advertising = 0;
	ethtool_cmd_speed_set(cmd, SPEED_10);
	cmd->duplex = DUPLEX_FULL;
	cmd->port = PORT_TP;
	cmd->phy_address = 0;
	cmd->transceiver = XCVR_INTERNAL;
	cmd->autoneg = AUTONEG_DISABLE;
	cmd->maxtxpkt = 0;
	cmd->maxrxpkt = 0;

	return 0;
}

static void bat_get_drvinfo(struct net_device *dev,
			    struct ethtool_drvinfo *info)
{
	strcpy(info->driver, "B.A.T.M.A.N. advanced");
	strcpy(info->version, SOURCE_VERSION);
	strcpy(info->fw_version, "N/A");
	strcpy(info->bus_info, "batman");
}

static u32 bat_get_msglevel(struct net_device *dev)
{
	return -EOPNOTSUPP;
}

static void bat_set_msglevel(struct net_device *dev, u32 value)
{
}

static u32 bat_get_link(struct net_device *dev)
{
	return 1;
}
=======
	dev->ethtool_ops = &batadv_ethtool_ops;
}

/**
 * batadv_softif_validate() - validate configuration of new batadv link
 * @tb: IFLA_INFO_DATA netlink attributes
 * @data: enum batadv_ifla_attrs attributes
 * @extack: extended ACK report struct
 *
 * Return: 0 if successful or error otherwise.
 */
static int batadv_softif_validate(struct nlattr *tb[], struct nlattr *data[],
				  struct netlink_ext_ack *extack)
{
	struct batadv_algo_ops *algo_ops;

	if (!data)
		return 0;

	if (data[IFLA_BATADV_ALGO_NAME]) {
		algo_ops = batadv_algo_get(nla_data(data[IFLA_BATADV_ALGO_NAME]));
		if (!algo_ops)
			return -EINVAL;
	}

	return 0;
}

/**
 * batadv_softif_newlink() - pre-initialize and register new batadv link
 * @src_net: the applicable net namespace
 * @dev: network device to register
 * @tb: IFLA_INFO_DATA netlink attributes
 * @data: enum batadv_ifla_attrs attributes
 * @extack: extended ACK report struct
 *
 * Return: 0 if successful or error otherwise.
 */
static int batadv_softif_newlink(struct net *src_net, struct net_device *dev,
				 struct nlattr *tb[], struct nlattr *data[],
				 struct netlink_ext_ack *extack)
{
	struct batadv_priv *bat_priv = netdev_priv(dev);
	const char *algo_name;
	int err;

	if (data && data[IFLA_BATADV_ALGO_NAME]) {
		algo_name = nla_data(data[IFLA_BATADV_ALGO_NAME]);
		err = batadv_algo_select(bat_priv, algo_name);
		if (err)
			return -EINVAL;
	}

	return register_netdevice(dev);
}

/**
 * batadv_softif_destroy_netlink() - deletion of batadv_soft_interface via
 *  netlink
 * @soft_iface: the to-be-removed batman-adv interface
 * @head: list pointer
 */
static void batadv_softif_destroy_netlink(struct net_device *soft_iface,
					  struct list_head *head)
{
	struct batadv_priv *bat_priv = netdev_priv(soft_iface);
	struct batadv_hard_iface *hard_iface;
	struct batadv_softif_vlan *vlan;

	list_for_each_entry(hard_iface, &batadv_hardif_list, list) {
		if (hard_iface->soft_iface == soft_iface)
			batadv_hardif_disable_interface(hard_iface);
	}

	/* destroy the "untagged" VLAN */
	vlan = batadv_softif_vlan_get(bat_priv, BATADV_NO_FLAGS);
	if (vlan) {
		batadv_softif_destroy_vlan(bat_priv, vlan);
		batadv_softif_vlan_put(vlan);
	}

	unregister_netdevice_queue(soft_iface, head);
}

/**
 * batadv_softif_is_valid() - Check whether device is a batadv soft interface
 * @net_dev: device which should be checked
 *
 * Return: true when net_dev is a batman-adv interface, false otherwise
 */
bool batadv_softif_is_valid(const struct net_device *net_dev)
{
	if (net_dev->netdev_ops->ndo_start_xmit == batadv_interface_tx)
		return true;

	return false;
}

static const struct nla_policy batadv_ifla_policy[IFLA_BATADV_MAX + 1] = {
	[IFLA_BATADV_ALGO_NAME]	= { .type = NLA_NUL_STRING },
};

struct rtnl_link_ops batadv_link_ops __read_mostly = {
	.kind		= "batadv",
	.priv_size	= sizeof(struct batadv_priv),
	.setup		= batadv_softif_init_early,
	.maxtype	= IFLA_BATADV_MAX,
	.policy		= batadv_ifla_policy,
	.validate	= batadv_softif_validate,
	.newlink	= batadv_softif_newlink,
	.dellink	= batadv_softif_destroy_netlink,
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
