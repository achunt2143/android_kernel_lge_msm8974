<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/if_vlan.h>
#include <linux/netpoll.h>
#include <linux/export.h>
<<<<<<< HEAD
=======
#include <net/gro.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "vlan.h"

bool vlan_do_receive(struct sk_buff **skbp)
{
	struct sk_buff *skb = *skbp;
<<<<<<< HEAD
	u16 vlan_id = skb->vlan_tci & VLAN_VID_MASK;
	struct net_device *vlan_dev;
	struct vlan_pcpu_stats *rx_stats;

	vlan_dev = vlan_find_dev(skb->dev, vlan_id);
=======
	__be16 vlan_proto = skb->vlan_proto;
	u16 vlan_id = skb_vlan_tag_get_id(skb);
	struct net_device *vlan_dev;
	struct vlan_pcpu_stats *rx_stats;

	vlan_dev = vlan_find_dev(skb->dev, vlan_proto, vlan_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!vlan_dev)
		return false;

	skb = *skbp = skb_share_check(skb, GFP_ATOMIC);
	if (unlikely(!skb))
		return false;

<<<<<<< HEAD
	skb->dev = vlan_dev;
	if (skb->pkt_type == PACKET_OTHERHOST) {
		/* Our lower layer thinks this is not local, let's make sure.
		 * This allows the VLAN to have a different MAC than the
		 * underlying device, and still route correctly. */
		if (!compare_ether_addr(eth_hdr(skb)->h_dest,
					vlan_dev->dev_addr))
			skb->pkt_type = PACKET_HOST;
	}

	if (!(vlan_dev_priv(vlan_dev)->flags & VLAN_FLAG_REORDER_HDR)) {
=======
	if (unlikely(!(vlan_dev->flags & IFF_UP))) {
		kfree_skb(skb);
		*skbp = NULL;
		return false;
	}

	skb->dev = vlan_dev;
	if (unlikely(skb->pkt_type == PACKET_OTHERHOST)) {
		/* Our lower layer thinks this is not local, let's make sure.
		 * This allows the VLAN to have a different MAC than the
		 * underlying device, and still route correctly. */
		if (ether_addr_equal_64bits(eth_hdr(skb)->h_dest, vlan_dev->dev_addr))
			skb->pkt_type = PACKET_HOST;
	}

	if (!(vlan_dev_priv(vlan_dev)->flags & VLAN_FLAG_REORDER_HDR) &&
	    !netif_is_macvlan_port(vlan_dev) &&
	    !netif_is_bridge_port(vlan_dev)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		unsigned int offset = skb->data - skb_mac_header(skb);

		/*
		 * vlan_insert_tag expect skb->data pointing to mac header.
		 * So change skb->data before calling it and change back to
		 * original position later
		 */
		skb_push(skb, offset);
<<<<<<< HEAD
		skb = *skbp = vlan_insert_tag(skb, skb->vlan_tci);
=======
		skb = *skbp = vlan_insert_inner_tag(skb, skb->vlan_proto,
						    skb->vlan_tci, skb->mac_len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!skb)
			return false;
		skb_pull(skb, offset + VLAN_HLEN);
		skb_reset_mac_len(skb);
	}

	skb->priority = vlan_get_ingress_priority(vlan_dev, skb->vlan_tci);
<<<<<<< HEAD
	skb->vlan_tci = 0;
=======
	__vlan_hwaccel_clear_tag(skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	rx_stats = this_cpu_ptr(vlan_dev_priv(vlan_dev)->vlan_pcpu_stats);

	u64_stats_update_begin(&rx_stats->syncp);
<<<<<<< HEAD
	rx_stats->rx_packets++;
	rx_stats->rx_bytes += skb->len;
	if (skb->pkt_type == PACKET_MULTICAST)
		rx_stats->rx_multicast++;
=======
	u64_stats_inc(&rx_stats->rx_packets);
	u64_stats_add(&rx_stats->rx_bytes, skb->len);
	if (skb->pkt_type == PACKET_MULTICAST)
		u64_stats_inc(&rx_stats->rx_multicast);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u64_stats_update_end(&rx_stats->syncp);

	return true;
}

<<<<<<< HEAD
/* Must be invoked with rcu_read_lock or with RTNL. */
struct net_device *__vlan_find_dev_deep(struct net_device *real_dev,
					u16 vlan_id)
{
	struct vlan_info *vlan_info = rcu_dereference_rtnl(real_dev->vlan_info);

	if (vlan_info) {
		return vlan_group_get_device(&vlan_info->grp, vlan_id);
	} else {
		/*
		 * Bonding slaves do not have grp assigned to themselves.
		 * Grp is assigned to bonding master instead.
		 */
		if (netif_is_bond_slave(real_dev))
			return __vlan_find_dev_deep(real_dev->master, vlan_id);
=======
/* Must be invoked with rcu_read_lock. */
struct net_device *__vlan_find_dev_deep_rcu(struct net_device *dev,
					__be16 vlan_proto, u16 vlan_id)
{
	struct vlan_info *vlan_info = rcu_dereference(dev->vlan_info);

	if (vlan_info) {
		return vlan_group_get_device(&vlan_info->grp,
					     vlan_proto, vlan_id);
	} else {
		/*
		 * Lower devices of master uppers (bonding, team) do not have
		 * grp assigned to themselves. Grp is assigned to upper device
		 * instead.
		 */
		struct net_device *upper_dev;

		upper_dev = netdev_master_upper_dev_get_rcu(dev);
		if (upper_dev)
			return __vlan_find_dev_deep_rcu(upper_dev,
						    vlan_proto, vlan_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return NULL;
}
<<<<<<< HEAD
EXPORT_SYMBOL(__vlan_find_dev_deep);

struct net_device *vlan_dev_real_dev(const struct net_device *dev)
{
	return vlan_dev_priv(dev)->real_dev;
=======
EXPORT_SYMBOL(__vlan_find_dev_deep_rcu);

struct net_device *vlan_dev_real_dev(const struct net_device *dev)
{
	struct net_device *ret = vlan_dev_priv(dev)->real_dev;

	while (is_vlan_dev(ret))
		ret = vlan_dev_priv(ret)->real_dev;

	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL(vlan_dev_real_dev);

u16 vlan_dev_vlan_id(const struct net_device *dev)
{
	return vlan_dev_priv(dev)->vlan_id;
}
EXPORT_SYMBOL(vlan_dev_vlan_id);

<<<<<<< HEAD
static struct sk_buff *vlan_reorder_header(struct sk_buff *skb)
{
	if (skb_cow(skb, skb_headroom(skb)) < 0) {
		kfree_skb(skb);
		return NULL;
	}

	memmove(skb->data - ETH_HLEN, skb->data - VLAN_ETH_HLEN, 2 * ETH_ALEN);
	skb->mac_header += VLAN_HLEN;
	return skb;
}

struct sk_buff *vlan_untag(struct sk_buff *skb)
{
	struct vlan_hdr *vhdr;
	u16 vlan_tci;

	if (unlikely(vlan_tx_tag_present(skb))) {
		/* vlan_tci is already set-up so leave this for another time */
		return skb;
	}

	skb = skb_share_check(skb, GFP_ATOMIC);
	if (unlikely(!skb))
		goto err_free;

	if (unlikely(!pskb_may_pull(skb, VLAN_HLEN)))
		goto err_free;

	vhdr = (struct vlan_hdr *) skb->data;
	vlan_tci = ntohs(vhdr->h_vlan_TCI);
	__vlan_hwaccel_put_tag(skb, vlan_tci);

	skb_pull_rcsum(skb, VLAN_HLEN);
	vlan_set_encap_proto(skb, vhdr);

	skb = vlan_reorder_header(skb);
	if (unlikely(!skb))
		goto err_free;

	skb_reset_network_header(skb);
	skb_reset_transport_header(skb);
	skb_reset_mac_len(skb);

	return skb;

err_free:
	kfree_skb(skb);
	return NULL;
}

=======
__be16 vlan_dev_vlan_proto(const struct net_device *dev)
{
	return vlan_dev_priv(dev)->vlan_proto;
}
EXPORT_SYMBOL(vlan_dev_vlan_proto);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * vlan info and vid list
 */

static void vlan_group_free(struct vlan_group *grp)
{
<<<<<<< HEAD
	int i;

	for (i = 0; i < VLAN_GROUP_ARRAY_SPLIT_PARTS; i++)
		kfree(grp->vlan_devices_arrays[i]);
=======
	int i, j;

	for (i = 0; i < VLAN_PROTO_NUM; i++)
		for (j = 0; j < VLAN_GROUP_ARRAY_SPLIT_PARTS; j++)
			kfree(grp->vlan_devices_arrays[i][j]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void vlan_info_free(struct vlan_info *vlan_info)
{
	vlan_group_free(&vlan_info->grp);
	kfree(vlan_info);
}

static void vlan_info_rcu_free(struct rcu_head *rcu)
{
	vlan_info_free(container_of(rcu, struct vlan_info, rcu));
}

static struct vlan_info *vlan_info_alloc(struct net_device *dev)
{
	struct vlan_info *vlan_info;

	vlan_info = kzalloc(sizeof(struct vlan_info), GFP_KERNEL);
	if (!vlan_info)
		return NULL;

	vlan_info->real_dev = dev;
	INIT_LIST_HEAD(&vlan_info->vid_list);
	return vlan_info;
}

struct vlan_vid_info {
	struct list_head list;
<<<<<<< HEAD
	unsigned short vid;
	int refcount;
};

static struct vlan_vid_info *vlan_vid_info_get(struct vlan_info *vlan_info,
					       unsigned short vid)
=======
	__be16 proto;
	u16 vid;
	int refcount;
};

static bool vlan_hw_filter_capable(const struct net_device *dev, __be16 proto)
{
	if (proto == htons(ETH_P_8021Q) &&
	    dev->features & NETIF_F_HW_VLAN_CTAG_FILTER)
		return true;
	if (proto == htons(ETH_P_8021AD) &&
	    dev->features & NETIF_F_HW_VLAN_STAG_FILTER)
		return true;
	return false;
}

static struct vlan_vid_info *vlan_vid_info_get(struct vlan_info *vlan_info,
					       __be16 proto, u16 vid)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct vlan_vid_info *vid_info;

	list_for_each_entry(vid_info, &vlan_info->vid_list, list) {
<<<<<<< HEAD
		if (vid_info->vid == vid)
=======
		if (vid_info->proto == proto && vid_info->vid == vid)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return vid_info;
	}
	return NULL;
}

<<<<<<< HEAD
static struct vlan_vid_info *vlan_vid_info_alloc(unsigned short vid)
=======
static struct vlan_vid_info *vlan_vid_info_alloc(__be16 proto, u16 vid)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct vlan_vid_info *vid_info;

	vid_info = kzalloc(sizeof(struct vlan_vid_info), GFP_KERNEL);
	if (!vid_info)
		return NULL;
<<<<<<< HEAD
=======
	vid_info->proto = proto;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	vid_info->vid = vid;

	return vid_info;
}

<<<<<<< HEAD
static int __vlan_vid_add(struct vlan_info *vlan_info, unsigned short vid,
			  struct vlan_vid_info **pvid_info)
{
	struct net_device *dev = vlan_info->real_dev;
	const struct net_device_ops *ops = dev->netdev_ops;
	struct vlan_vid_info *vid_info;
	int err;

	vid_info = vlan_vid_info_alloc(vid);
	if (!vid_info)
		return -ENOMEM;

	if ((dev->features & NETIF_F_HW_VLAN_FILTER) &&
	    ops->ndo_vlan_rx_add_vid) {
		err =  ops->ndo_vlan_rx_add_vid(dev, vid);
		if (err) {
			kfree(vid_info);
			return err;
		}
	}
=======
static int vlan_add_rx_filter_info(struct net_device *dev, __be16 proto, u16 vid)
{
	if (!vlan_hw_filter_capable(dev, proto))
		return 0;

	if (netif_device_present(dev))
		return dev->netdev_ops->ndo_vlan_rx_add_vid(dev, proto, vid);
	else
		return -ENODEV;
}

static int vlan_kill_rx_filter_info(struct net_device *dev, __be16 proto, u16 vid)
{
	if (!vlan_hw_filter_capable(dev, proto))
		return 0;

	if (netif_device_present(dev))
		return dev->netdev_ops->ndo_vlan_rx_kill_vid(dev, proto, vid);
	else
		return -ENODEV;
}

int vlan_for_each(struct net_device *dev,
		  int (*action)(struct net_device *dev, int vid, void *arg),
		  void *arg)
{
	struct vlan_vid_info *vid_info;
	struct vlan_info *vlan_info;
	struct net_device *vdev;
	int ret;

	ASSERT_RTNL();

	vlan_info = rtnl_dereference(dev->vlan_info);
	if (!vlan_info)
		return 0;

	list_for_each_entry(vid_info, &vlan_info->vid_list, list) {
		vdev = vlan_group_get_device(&vlan_info->grp, vid_info->proto,
					     vid_info->vid);
		ret = action(vdev, vid_info->vid, arg);
		if (ret)
			return ret;
	}

	return 0;
}
EXPORT_SYMBOL(vlan_for_each);

int vlan_filter_push_vids(struct vlan_info *vlan_info, __be16 proto)
{
	struct net_device *real_dev = vlan_info->real_dev;
	struct vlan_vid_info *vlan_vid_info;
	int err;

	list_for_each_entry(vlan_vid_info, &vlan_info->vid_list, list) {
		if (vlan_vid_info->proto == proto) {
			err = vlan_add_rx_filter_info(real_dev, proto,
						      vlan_vid_info->vid);
			if (err)
				goto unwind;
		}
	}

	return 0;

unwind:
	list_for_each_entry_continue_reverse(vlan_vid_info,
					     &vlan_info->vid_list, list) {
		if (vlan_vid_info->proto == proto)
			vlan_kill_rx_filter_info(real_dev, proto,
						 vlan_vid_info->vid);
	}

	return err;
}
EXPORT_SYMBOL(vlan_filter_push_vids);

void vlan_filter_drop_vids(struct vlan_info *vlan_info, __be16 proto)
{
	struct vlan_vid_info *vlan_vid_info;

	list_for_each_entry(vlan_vid_info, &vlan_info->vid_list, list)
		if (vlan_vid_info->proto == proto)
			vlan_kill_rx_filter_info(vlan_info->real_dev,
						 vlan_vid_info->proto,
						 vlan_vid_info->vid);
}
EXPORT_SYMBOL(vlan_filter_drop_vids);

static int __vlan_vid_add(struct vlan_info *vlan_info, __be16 proto, u16 vid,
			  struct vlan_vid_info **pvid_info)
{
	struct net_device *dev = vlan_info->real_dev;
	struct vlan_vid_info *vid_info;
	int err;

	vid_info = vlan_vid_info_alloc(proto, vid);
	if (!vid_info)
		return -ENOMEM;

	err = vlan_add_rx_filter_info(dev, proto, vid);
	if (err) {
		kfree(vid_info);
		return err;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	list_add(&vid_info->list, &vlan_info->vid_list);
	vlan_info->nr_vids++;
	*pvid_info = vid_info;
	return 0;
}

<<<<<<< HEAD
int vlan_vid_add(struct net_device *dev, unsigned short vid)
=======
int vlan_vid_add(struct net_device *dev, __be16 proto, u16 vid)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct vlan_info *vlan_info;
	struct vlan_vid_info *vid_info;
	bool vlan_info_created = false;
	int err;

	ASSERT_RTNL();

	vlan_info = rtnl_dereference(dev->vlan_info);
	if (!vlan_info) {
		vlan_info = vlan_info_alloc(dev);
		if (!vlan_info)
			return -ENOMEM;
		vlan_info_created = true;
	}
<<<<<<< HEAD
	vid_info = vlan_vid_info_get(vlan_info, vid);
	if (!vid_info) {
		err = __vlan_vid_add(vlan_info, vid, &vid_info);
=======
	vid_info = vlan_vid_info_get(vlan_info, proto, vid);
	if (!vid_info) {
		err = __vlan_vid_add(vlan_info, proto, vid, &vid_info);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (err)
			goto out_free_vlan_info;
	}
	vid_info->refcount++;

	if (vlan_info_created)
		rcu_assign_pointer(dev->vlan_info, vlan_info);

	return 0;

out_free_vlan_info:
	if (vlan_info_created)
		kfree(vlan_info);
	return err;
}
EXPORT_SYMBOL(vlan_vid_add);

static void __vlan_vid_del(struct vlan_info *vlan_info,
			   struct vlan_vid_info *vid_info)
{
	struct net_device *dev = vlan_info->real_dev;
<<<<<<< HEAD
	const struct net_device_ops *ops = dev->netdev_ops;
	unsigned short vid = vid_info->vid;
	int err;

	if ((dev->features & NETIF_F_HW_VLAN_FILTER) &&
	     ops->ndo_vlan_rx_kill_vid) {
		err = ops->ndo_vlan_rx_kill_vid(dev, vid);
		if (err) {
			pr_warn("failed to kill vid %d for device %s\n",
				vid, dev->name);
		}
	}
=======
	__be16 proto = vid_info->proto;
	u16 vid = vid_info->vid;
	int err;

	err = vlan_kill_rx_filter_info(dev, proto, vid);
	if (err && dev->reg_state != NETREG_UNREGISTERING)
		netdev_warn(dev, "failed to kill vid %04x/%d\n", proto, vid);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	list_del(&vid_info->list);
	kfree(vid_info);
	vlan_info->nr_vids--;
}

<<<<<<< HEAD
void vlan_vid_del(struct net_device *dev, unsigned short vid)
=======
void vlan_vid_del(struct net_device *dev, __be16 proto, u16 vid)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct vlan_info *vlan_info;
	struct vlan_vid_info *vid_info;

	ASSERT_RTNL();

	vlan_info = rtnl_dereference(dev->vlan_info);
	if (!vlan_info)
		return;

<<<<<<< HEAD
	vid_info = vlan_vid_info_get(vlan_info, vid);
=======
	vid_info = vlan_vid_info_get(vlan_info, proto, vid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!vid_info)
		return;
	vid_info->refcount--;
	if (vid_info->refcount == 0) {
		__vlan_vid_del(vlan_info, vid_info);
		if (vlan_info->nr_vids == 0) {
			RCU_INIT_POINTER(dev->vlan_info, NULL);
			call_rcu(&vlan_info->rcu, vlan_info_rcu_free);
		}
	}
}
EXPORT_SYMBOL(vlan_vid_del);

int vlan_vids_add_by_dev(struct net_device *dev,
			 const struct net_device *by_dev)
{
	struct vlan_vid_info *vid_info;
	struct vlan_info *vlan_info;
	int err;

	ASSERT_RTNL();

	vlan_info = rtnl_dereference(by_dev->vlan_info);
	if (!vlan_info)
		return 0;

	list_for_each_entry(vid_info, &vlan_info->vid_list, list) {
<<<<<<< HEAD
		err = vlan_vid_add(dev, vid_info->vid);
=======
		if (!vlan_hw_filter_capable(by_dev, vid_info->proto))
			continue;
		err = vlan_vid_add(dev, vid_info->proto, vid_info->vid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (err)
			goto unwind;
	}
	return 0;

unwind:
	list_for_each_entry_continue_reverse(vid_info,
					     &vlan_info->vid_list,
					     list) {
<<<<<<< HEAD
		vlan_vid_del(dev, vid_info->vid);
=======
		if (!vlan_hw_filter_capable(by_dev, vid_info->proto))
			continue;
		vlan_vid_del(dev, vid_info->proto, vid_info->vid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return err;
}
EXPORT_SYMBOL(vlan_vids_add_by_dev);

void vlan_vids_del_by_dev(struct net_device *dev,
			  const struct net_device *by_dev)
{
	struct vlan_vid_info *vid_info;
	struct vlan_info *vlan_info;

	ASSERT_RTNL();

	vlan_info = rtnl_dereference(by_dev->vlan_info);
	if (!vlan_info)
		return;

<<<<<<< HEAD
	list_for_each_entry(vid_info, &vlan_info->vid_list, list)
		vlan_vid_del(dev, vid_info->vid);
}
EXPORT_SYMBOL(vlan_vids_del_by_dev);
=======
	list_for_each_entry(vid_info, &vlan_info->vid_list, list) {
		if (!vlan_hw_filter_capable(by_dev, vid_info->proto))
			continue;
		vlan_vid_del(dev, vid_info->proto, vid_info->vid);
	}
}
EXPORT_SYMBOL(vlan_vids_del_by_dev);

bool vlan_uses_dev(const struct net_device *dev)
{
	struct vlan_info *vlan_info;

	ASSERT_RTNL();

	vlan_info = rtnl_dereference(dev->vlan_info);
	if (!vlan_info)
		return false;
	return vlan_info->grp.nr_vlan_devs ? true : false;
}
EXPORT_SYMBOL(vlan_uses_dev);

static struct sk_buff *vlan_gro_receive(struct list_head *head,
					struct sk_buff *skb)
{
	const struct packet_offload *ptype;
	unsigned int hlen, off_vlan;
	struct sk_buff *pp = NULL;
	struct vlan_hdr *vhdr;
	struct sk_buff *p;
	__be16 type;
	int flush = 1;

	off_vlan = skb_gro_offset(skb);
	hlen = off_vlan + sizeof(*vhdr);
	vhdr = skb_gro_header(skb, hlen, off_vlan);
	if (unlikely(!vhdr))
		goto out;

	NAPI_GRO_CB(skb)->network_offsets[NAPI_GRO_CB(skb)->encap_mark] = hlen;

	type = vhdr->h_vlan_encapsulated_proto;

	ptype = gro_find_receive_by_type(type);
	if (!ptype)
		goto out;

	flush = 0;

	list_for_each_entry(p, head, list) {
		struct vlan_hdr *vhdr2;

		if (!NAPI_GRO_CB(p)->same_flow)
			continue;

		vhdr2 = (struct vlan_hdr *)(p->data + off_vlan);
		if (compare_vlan_header(vhdr, vhdr2))
			NAPI_GRO_CB(p)->same_flow = 0;
	}

	skb_gro_pull(skb, sizeof(*vhdr));
	skb_gro_postpull_rcsum(skb, vhdr, sizeof(*vhdr));

	pp = indirect_call_gro_receive_inet(ptype->callbacks.gro_receive,
					    ipv6_gro_receive, inet_gro_receive,
					    head, skb);

out:
	skb_gro_flush_final(skb, pp, flush);

	return pp;
}

static int vlan_gro_complete(struct sk_buff *skb, int nhoff)
{
	struct vlan_hdr *vhdr = (struct vlan_hdr *)(skb->data + nhoff);
	__be16 type = vhdr->h_vlan_encapsulated_proto;
	struct packet_offload *ptype;
	int err = -ENOENT;

	ptype = gro_find_complete_by_type(type);
	if (ptype)
		err = INDIRECT_CALL_INET(ptype->callbacks.gro_complete,
					 ipv6_gro_complete, inet_gro_complete,
					 skb, nhoff + sizeof(*vhdr));

	return err;
}

static struct packet_offload vlan_packet_offloads[] __read_mostly = {
	{
		.type = cpu_to_be16(ETH_P_8021Q),
		.priority = 10,
		.callbacks = {
			.gro_receive = vlan_gro_receive,
			.gro_complete = vlan_gro_complete,
		},
	},
	{
		.type = cpu_to_be16(ETH_P_8021AD),
		.priority = 10,
		.callbacks = {
			.gro_receive = vlan_gro_receive,
			.gro_complete = vlan_gro_complete,
		},
	},
};

static int __init vlan_offload_init(void)
{
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(vlan_packet_offloads); i++)
		dev_add_offload(&vlan_packet_offloads[i]);

	return 0;
}

fs_initcall(vlan_offload_init);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
