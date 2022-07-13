<<<<<<< HEAD
/*
 * Copyright (c) 2007-2012 Nicira Networks.
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
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2007-2014 Nicira, Inc.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/init.h>
#include <linux/module.h>
#include <linux/if_arp.h>
#include <linux/if_vlan.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/jhash.h>
#include <linux/delay.h>
#include <linux/time.h>
#include <linux/etherdevice.h>
#include <linux/genetlink.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/mutex.h>
#include <linux/percpu.h>
#include <linux/rcupdate.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/ethtool.h>
#include <linux/wait.h>
#include <asm/div64.h>
#include <linux/highmem.h>
#include <linux/netfilter_bridge.h>
#include <linux/netfilter_ipv4.h>
#include <linux/inetdevice.h>
#include <linux/list.h>
#include <linux/openvswitch.h>
#include <linux/rculist.h>
#include <linux/dmi.h>
<<<<<<< HEAD
#include <linux/workqueue.h>
#include <net/genetlink.h>

#include "datapath.h"
#include "flow.h"
#include "vport-internal_dev.h"
=======
#include <net/genetlink.h>
#include <net/gso.h>
#include <net/net_namespace.h>
#include <net/netns/generic.h>
#include <net/pkt_cls.h>

#include "datapath.h"
#include "drop.h"
#include "flow.h"
#include "flow_table.h"
#include "flow_netlink.h"
#include "meter.h"
#include "openvswitch_trace.h"
#include "vport-internal_dev.h"
#include "vport-netdev.h"

unsigned int ovs_net_id __read_mostly;

static struct genl_family dp_packet_genl_family;
static struct genl_family dp_flow_genl_family;
static struct genl_family dp_datapath_genl_family;

static const struct nla_policy flow_policy[];

static const struct genl_multicast_group ovs_dp_flow_multicast_group = {
	.name = OVS_FLOW_MCGROUP,
};

static const struct genl_multicast_group ovs_dp_datapath_multicast_group = {
	.name = OVS_DATAPATH_MCGROUP,
};

static const struct genl_multicast_group ovs_dp_vport_multicast_group = {
	.name = OVS_VPORT_MCGROUP,
};

/* Check if need to build a reply message.
 * OVS userspace sets the NLM_F_ECHO flag if it needs the reply. */
static bool ovs_must_notify(struct genl_family *family, struct genl_info *info,
			    unsigned int group)
{
	return info->nlhdr->nlmsg_flags & NLM_F_ECHO ||
	       genl_has_listeners(family, genl_info_net(info), group);
}

static void ovs_notify(struct genl_family *family,
		       struct sk_buff *skb, struct genl_info *info)
{
	genl_notify(family, skb, info, 0, GFP_KERNEL);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * DOC: Locking:
 *
<<<<<<< HEAD
 * Writes to device state (add/remove datapath, port, set operations on vports,
 * etc.) are protected by RTNL.
 *
 * Writes to other state (flow table modifications, set miscellaneous datapath
 * parameters, etc.) are protected by genl_mutex.  The RTNL lock nests inside
 * genl_mutex.
=======
 * All writes e.g. Writes to device state (add/remove datapath, port, set
 * operations on vports, etc.), Writes to other state (flow table
 * modifications, set miscellaneous datapath parameters, etc.) are protected
 * by ovs_lock.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Reads are protected by RCU.
 *
 * There are a few special cases (mostly stats) that have their own
 * synchronization but they nest under all of above and don't interact with
 * each other.
<<<<<<< HEAD
 */

/* Global list of datapaths to enable dumping them all out.
 * Protected by genl_mutex.
 */
static LIST_HEAD(dps);

#define REHASH_FLOW_INTERVAL (10 * 60 * HZ)
static void rehash_flow_table(struct work_struct *work);
static DECLARE_DELAYED_WORK(rehash_flow_wq, rehash_flow_table);

static struct vport *new_vport(const struct vport_parms *);
static int queue_gso_packets(int dp_ifindex, struct sk_buff *,
			     const struct dp_upcall_info *);
static int queue_userspace_packet(int dp_ifindex, struct sk_buff *,
				  const struct dp_upcall_info *);

/* Must be called with rcu_read_lock, genl_mutex, or RTNL lock. */
static struct datapath *get_dp(int dp_ifindex)
{
	struct datapath *dp = NULL;
	struct net_device *dev;

	rcu_read_lock();
	dev = dev_get_by_index_rcu(&init_net, dp_ifindex);
	if (dev) {
		struct vport *vport = ovs_internal_dev_get_vport(dev);
		if (vport)
			dp = vport->dp;
	}
	rcu_read_unlock();

	return dp;
}

/* Must be called with rcu_read_lock or RTNL lock. */
const char *ovs_dp_name(const struct datapath *dp)
{
	struct vport *vport = rcu_dereference_rtnl(dp->ports[OVSP_LOCAL]);
	return vport->ops->get_name(vport);
}

static int get_dpifindex(struct datapath *dp)
=======
 *
 * The RTNL lock nests inside ovs_mutex.
 */

static DEFINE_MUTEX(ovs_mutex);

void ovs_lock(void)
{
	mutex_lock(&ovs_mutex);
}

void ovs_unlock(void)
{
	mutex_unlock(&ovs_mutex);
}

#ifdef CONFIG_LOCKDEP
int lockdep_ovsl_is_held(void)
{
	if (debug_locks)
		return lockdep_is_held(&ovs_mutex);
	else
		return 1;
}
#endif

static struct vport *new_vport(const struct vport_parms *);
static int queue_gso_packets(struct datapath *dp, struct sk_buff *,
			     const struct sw_flow_key *,
			     const struct dp_upcall_info *,
			     uint32_t cutlen);
static int queue_userspace_packet(struct datapath *dp, struct sk_buff *,
				  const struct sw_flow_key *,
				  const struct dp_upcall_info *,
				  uint32_t cutlen);

static void ovs_dp_masks_rebalance(struct work_struct *work);

static int ovs_dp_set_upcall_portids(struct datapath *, const struct nlattr *);

/* Must be called with rcu_read_lock or ovs_mutex. */
const char *ovs_dp_name(const struct datapath *dp)
{
	struct vport *vport = ovs_vport_ovsl_rcu(dp, OVSP_LOCAL);
	return ovs_vport_name(vport);
}

static int get_dpifindex(const struct datapath *dp)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct vport *local;
	int ifindex;

	rcu_read_lock();

<<<<<<< HEAD
	local = rcu_dereference(dp->ports[OVSP_LOCAL]);
	if (local)
		ifindex = local->ops->get_ifindex(local);
=======
	local = ovs_vport_rcu(dp, OVSP_LOCAL);
	if (local)
		ifindex = local->dev->ifindex;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	else
		ifindex = 0;

	rcu_read_unlock();

	return ifindex;
}

static void destroy_dp_rcu(struct rcu_head *rcu)
{
	struct datapath *dp = container_of(rcu, struct datapath, rcu);

<<<<<<< HEAD
	ovs_flow_tbl_destroy((__force struct flow_table *)dp->table);
	free_percpu(dp->stats_percpu);
	kfree(dp);
}

/* Called with RTNL lock and genl_lock. */
=======
	ovs_flow_tbl_destroy(&dp->table);
	free_percpu(dp->stats_percpu);
	kfree(dp->ports);
	ovs_meters_exit(dp);
	kfree(rcu_dereference_raw(dp->upcall_portids));
	kfree(dp);
}

static struct hlist_head *vport_hash_bucket(const struct datapath *dp,
					    u16 port_no)
{
	return &dp->ports[port_no & (DP_VPORT_HASH_BUCKETS - 1)];
}

/* Called with ovs_mutex or RCU read lock. */
struct vport *ovs_lookup_vport(const struct datapath *dp, u16 port_no)
{
	struct vport *vport;
	struct hlist_head *head;

	head = vport_hash_bucket(dp, port_no);
	hlist_for_each_entry_rcu(vport, head, dp_hash_node,
				 lockdep_ovsl_is_held()) {
		if (vport->port_no == port_no)
			return vport;
	}
	return NULL;
}

/* Called with ovs_mutex. */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct vport *new_vport(const struct vport_parms *parms)
{
	struct vport *vport;

	vport = ovs_vport_add(parms);
	if (!IS_ERR(vport)) {
		struct datapath *dp = parms->dp;
<<<<<<< HEAD

		rcu_assign_pointer(dp->ports[parms->port_no], vport);
		list_add(&vport->node, &dp->port_list);
	}

	return vport;
}

/* Called with RTNL lock. */
void ovs_dp_detach_port(struct vport *p)
{
	ASSERT_RTNL();

	/* First drop references to device. */
	list_del(&p->node);
	rcu_assign_pointer(p->dp->ports[p->port_no], NULL);
=======
		struct hlist_head *head = vport_hash_bucket(dp, vport->port_no);

		hlist_add_head_rcu(&vport->dp_hash_node, head);
	}
	return vport;
}

static void ovs_vport_update_upcall_stats(struct sk_buff *skb,
					  const struct dp_upcall_info *upcall_info,
					  bool upcall_result)
{
	struct vport *p = OVS_CB(skb)->input_vport;
	struct vport_upcall_stats_percpu *stats;

	if (upcall_info->cmd != OVS_PACKET_CMD_MISS &&
	    upcall_info->cmd != OVS_PACKET_CMD_ACTION)
		return;

	stats = this_cpu_ptr(p->upcall_stats);
	u64_stats_update_begin(&stats->syncp);
	if (upcall_result)
		u64_stats_inc(&stats->n_success);
	else
		u64_stats_inc(&stats->n_fail);
	u64_stats_update_end(&stats->syncp);
}

void ovs_dp_detach_port(struct vport *p)
{
	ASSERT_OVSL();

	/* First drop references to device. */
	hlist_del_rcu(&p->dp_hash_node);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Then destroy it. */
	ovs_vport_del(p);
}

/* Must be called with rcu_read_lock. */
<<<<<<< HEAD
void ovs_dp_process_received_packet(struct vport *p, struct sk_buff *skb)
{
	struct datapath *dp = p->dp;
	struct sw_flow *flow;
	struct dp_stats_percpu *stats;
	struct sw_flow_key key;
	u64 *stats_counter;
	int error;
	int key_len;

	stats = per_cpu_ptr(dp->stats_percpu, smp_processor_id());

	/* Extract flow from 'skb' into 'key'. */
	error = ovs_flow_extract(skb, p->port_no, &key, &key_len);
	if (unlikely(error)) {
		kfree_skb(skb);
		return;
	}

	/* Look up flow. */
	flow = ovs_flow_tbl_lookup(rcu_dereference(dp->table), &key, key_len);
	if (unlikely(!flow)) {
		struct dp_upcall_info upcall;

		upcall.cmd = OVS_PACKET_CMD_MISS;
		upcall.key = &key;
		upcall.userdata = NULL;
		upcall.pid = p->upcall_pid;
		ovs_dp_upcall(dp, skb, &upcall);
		consume_skb(skb);
=======
void ovs_dp_process_packet(struct sk_buff *skb, struct sw_flow_key *key)
{
	const struct vport *p = OVS_CB(skb)->input_vport;
	struct datapath *dp = p->dp;
	struct sw_flow *flow;
	struct sw_flow_actions *sf_acts;
	struct dp_stats_percpu *stats;
	u64 *stats_counter;
	u32 n_mask_hit;
	u32 n_cache_hit;
	int error;

	stats = this_cpu_ptr(dp->stats_percpu);

	/* Look up flow. */
	flow = ovs_flow_tbl_lookup_stats(&dp->table, key, skb_get_hash(skb),
					 &n_mask_hit, &n_cache_hit);
	if (unlikely(!flow)) {
		struct dp_upcall_info upcall;

		memset(&upcall, 0, sizeof(upcall));
		upcall.cmd = OVS_PACKET_CMD_MISS;

		if (dp->user_features & OVS_DP_F_DISPATCH_UPCALL_PER_CPU)
			upcall.portid =
			    ovs_dp_get_upcall_portid(dp, smp_processor_id());
		else
			upcall.portid = ovs_vport_find_upcall_portid(p, skb);

		upcall.mru = OVS_CB(skb)->mru;
		error = ovs_dp_upcall(dp, skb, key, &upcall, 0);
		switch (error) {
		case 0:
		case -EAGAIN:
		case -ERESTARTSYS:
		case -EINTR:
			consume_skb(skb);
			break;
		default:
			kfree_skb(skb);
			break;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		stats_counter = &stats->n_missed;
		goto out;
	}

<<<<<<< HEAD
	OVS_CB(skb)->flow = flow;

	stats_counter = &stats->n_hit;
	ovs_flow_used(OVS_CB(skb)->flow, skb);
	ovs_execute_actions(dp, skb);

out:
	/* Update datapath statistics. */
	u64_stats_update_begin(&stats->sync);
	(*stats_counter)++;
	u64_stats_update_end(&stats->sync);
}

static struct genl_family dp_packet_genl_family = {
	.id = GENL_ID_GENERATE,
	.hdrsize = sizeof(struct ovs_header),
	.name = OVS_PACKET_FAMILY,
	.version = OVS_PACKET_VERSION,
	.maxattr = OVS_PACKET_ATTR_MAX
};

int ovs_dp_upcall(struct datapath *dp, struct sk_buff *skb,
	      const struct dp_upcall_info *upcall_info)
{
	struct dp_stats_percpu *stats;
	int dp_ifindex;
	int err;

	if (upcall_info->pid == 0) {
=======
	ovs_flow_stats_update(flow, key->tp.flags, skb);
	sf_acts = rcu_dereference(flow->sf_acts);
	error = ovs_execute_actions(dp, skb, sf_acts, key);
	if (unlikely(error))
		net_dbg_ratelimited("ovs: action execution error on datapath %s: %d\n",
				    ovs_dp_name(dp), error);

	stats_counter = &stats->n_hit;

out:
	/* Update datapath statistics. */
	u64_stats_update_begin(&stats->syncp);
	(*stats_counter)++;
	stats->n_mask_hit += n_mask_hit;
	stats->n_cache_hit += n_cache_hit;
	u64_stats_update_end(&stats->syncp);
}

int ovs_dp_upcall(struct datapath *dp, struct sk_buff *skb,
		  const struct sw_flow_key *key,
		  const struct dp_upcall_info *upcall_info,
		  uint32_t cutlen)
{
	struct dp_stats_percpu *stats;
	int err;

	if (trace_ovs_dp_upcall_enabled())
		trace_ovs_dp_upcall(dp, skb, key, upcall_info);

	if (upcall_info->portid == 0) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = -ENOTCONN;
		goto err;
	}

<<<<<<< HEAD
	dp_ifindex = get_dpifindex(dp);
	if (!dp_ifindex) {
		err = -ENODEV;
		goto err;
	}

	if (!skb_is_gso(skb))
		err = queue_userspace_packet(dp_ifindex, skb, upcall_info);
	else
		err = queue_gso_packets(dp_ifindex, skb, upcall_info);
=======
	if (!skb_is_gso(skb))
		err = queue_userspace_packet(dp, skb, key, upcall_info, cutlen);
	else
		err = queue_gso_packets(dp, skb, key, upcall_info, cutlen);

	ovs_vport_update_upcall_stats(skb, upcall_info, !err);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err)
		goto err;

	return 0;

err:
<<<<<<< HEAD
	stats = per_cpu_ptr(dp->stats_percpu, smp_processor_id());

	u64_stats_update_begin(&stats->sync);
	stats->n_lost++;
	u64_stats_update_end(&stats->sync);
=======
	stats = this_cpu_ptr(dp->stats_percpu);

	u64_stats_update_begin(&stats->syncp);
	stats->n_lost++;
	u64_stats_update_end(&stats->syncp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return err;
}

<<<<<<< HEAD
static int queue_gso_packets(int dp_ifindex, struct sk_buff *skb,
			     const struct dp_upcall_info *upcall_info)
{
	struct dp_upcall_info later_info;
=======
static int queue_gso_packets(struct datapath *dp, struct sk_buff *skb,
			     const struct sw_flow_key *key,
			     const struct dp_upcall_info *upcall_info,
			     uint32_t cutlen)
{
	unsigned int gso_type = skb_shinfo(skb)->gso_type;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct sw_flow_key later_key;
	struct sk_buff *segs, *nskb;
	int err;

<<<<<<< HEAD
	segs = skb_gso_segment(skb, NETIF_F_SG | NETIF_F_HW_CSUM);
=======
	BUILD_BUG_ON(sizeof(*OVS_CB(skb)) > SKB_GSO_CB_OFFSET);
	segs = __skb_gso_segment(skb, NETIF_F_SG, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (IS_ERR(segs))
		return PTR_ERR(segs);
	if (segs == NULL)
		return -EINVAL;

<<<<<<< HEAD
	/* Queue all of the segments. */
	skb = segs;
	do {
		err = queue_userspace_packet(dp_ifindex, skb, upcall_info);
		if (err)
			break;

		if (skb == segs && skb_shinfo(skb)->gso_type & SKB_GSO_UDP) {
			/* The initial flow key extracted by ovs_flow_extract()
			 * in this case is for a first fragment, so we need to
			 * properly mark later fragments.
			 */
			later_key = *upcall_info->key;
			later_key.ip.frag = OVS_FRAG_TYPE_LATER;

			later_info = *upcall_info;
			later_info.key = &later_key;
			upcall_info = &later_info;
		}
	} while ((skb = skb->next));

	/* Free all of the segments. */
	skb = segs;
	do {
		nskb = skb->next;
=======
	if (gso_type & SKB_GSO_UDP) {
		/* The initial flow key extracted by ovs_flow_key_extract()
		 * in this case is for a first fragment, so we need to
		 * properly mark later fragments.
		 */
		later_key = *key;
		later_key.ip.frag = OVS_FRAG_TYPE_LATER;
	}

	/* Queue all of the segments. */
	skb_list_walk_safe(segs, skb, nskb) {
		if (gso_type & SKB_GSO_UDP && skb != segs)
			key = &later_key;

		err = queue_userspace_packet(dp, skb, key, upcall_info, cutlen);
		if (err)
			break;

	}

	/* Free all of the segments. */
	skb_list_walk_safe(segs, skb, nskb) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (err)
			kfree_skb(skb);
		else
			consume_skb(skb);
<<<<<<< HEAD
	} while ((skb = nskb));
	return err;
}

static int queue_userspace_packet(int dp_ifindex, struct sk_buff *skb,
				  const struct dp_upcall_info *upcall_info)
{
	struct ovs_header *upcall;
	struct sk_buff *nskb = NULL;
	struct sk_buff *user_skb; /* to be queued to userspace */
	struct nlattr *nla;
	unsigned int len;
	int err;

	if (vlan_tx_tag_present(skb)) {
=======
	}
	return err;
}

static size_t upcall_msg_size(const struct dp_upcall_info *upcall_info,
			      unsigned int hdrlen, int actions_attrlen)
{
	size_t size = NLMSG_ALIGN(sizeof(struct ovs_header))
		+ nla_total_size(hdrlen) /* OVS_PACKET_ATTR_PACKET */
		+ nla_total_size(ovs_key_attr_size()) /* OVS_PACKET_ATTR_KEY */
		+ nla_total_size(sizeof(unsigned int)) /* OVS_PACKET_ATTR_LEN */
		+ nla_total_size(sizeof(u64)); /* OVS_PACKET_ATTR_HASH */

	/* OVS_PACKET_ATTR_USERDATA */
	if (upcall_info->userdata)
		size += NLA_ALIGN(upcall_info->userdata->nla_len);

	/* OVS_PACKET_ATTR_EGRESS_TUN_KEY */
	if (upcall_info->egress_tun_info)
		size += nla_total_size(ovs_tun_key_attr_size());

	/* OVS_PACKET_ATTR_ACTIONS */
	if (upcall_info->actions_len)
		size += nla_total_size(actions_attrlen);

	/* OVS_PACKET_ATTR_MRU */
	if (upcall_info->mru)
		size += nla_total_size(sizeof(upcall_info->mru));

	return size;
}

static void pad_packet(struct datapath *dp, struct sk_buff *skb)
{
	if (!(dp->user_features & OVS_DP_F_UNALIGNED)) {
		size_t plen = NLA_ALIGN(skb->len) - skb->len;

		if (plen > 0)
			skb_put_zero(skb, plen);
	}
}

static int queue_userspace_packet(struct datapath *dp, struct sk_buff *skb,
				  const struct sw_flow_key *key,
				  const struct dp_upcall_info *upcall_info,
				  uint32_t cutlen)
{
	struct ovs_header *upcall;
	struct sk_buff *nskb = NULL;
	struct sk_buff *user_skb = NULL; /* to be queued to userspace */
	struct nlattr *nla;
	size_t len;
	unsigned int hlen;
	int err, dp_ifindex;
	u64 hash;

	dp_ifindex = get_dpifindex(dp);
	if (!dp_ifindex)
		return -ENODEV;

	if (skb_vlan_tag_present(skb)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		nskb = skb_clone(skb, GFP_ATOMIC);
		if (!nskb)
			return -ENOMEM;

<<<<<<< HEAD
		nskb = __vlan_put_tag(nskb, vlan_tx_tag_get(nskb));
		if (!nskb)
			return -ENOMEM;

		nskb->vlan_tci = 0;
=======
		nskb = __vlan_hwaccel_push_inside(nskb);
		if (!nskb)
			return -ENOMEM;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		skb = nskb;
	}

	if (nla_attr_size(skb->len) > USHRT_MAX) {
		err = -EFBIG;
		goto out;
	}

<<<<<<< HEAD
	len = sizeof(struct ovs_header);
	len += nla_total_size(skb->len);
	len += nla_total_size(FLOW_BUFSIZE);
	if (upcall_info->cmd == OVS_PACKET_CMD_ACTION)
		len += nla_total_size(8);

=======
	/* Complete checksum if needed */
	if (skb->ip_summed == CHECKSUM_PARTIAL &&
	    (err = skb_csum_hwoffload_help(skb, 0)))
		goto out;

	/* Older versions of OVS user space enforce alignment of the last
	 * Netlink attribute to NLA_ALIGNTO which would require extensive
	 * padding logic. Only perform zerocopy if padding is not required.
	 */
	if (dp->user_features & OVS_DP_F_UNALIGNED)
		hlen = skb_zerocopy_headlen(skb);
	else
		hlen = skb->len;

	len = upcall_msg_size(upcall_info, hlen - cutlen,
			      OVS_CB(skb)->acts_origlen);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	user_skb = genlmsg_new(len, GFP_ATOMIC);
	if (!user_skb) {
		err = -ENOMEM;
		goto out;
	}

	upcall = genlmsg_put(user_skb, 0, 0, &dp_packet_genl_family,
			     0, upcall_info->cmd);
<<<<<<< HEAD
	upcall->dp_ifindex = dp_ifindex;

	nla = nla_nest_start(user_skb, OVS_PACKET_ATTR_KEY);
	ovs_flow_to_nlattrs(upcall_info->key, user_skb);
	nla_nest_end(user_skb, nla);

	if (upcall_info->userdata)
		nla_put_u64(user_skb, OVS_PACKET_ATTR_USERDATA,
			    nla_get_u64(upcall_info->userdata));

	nla = __nla_reserve(user_skb, OVS_PACKET_ATTR_PACKET, skb->len);

	skb_copy_and_csum_dev(skb, nla_data(nla));

	err = genlmsg_unicast(&init_net, user_skb, upcall_info->pid);

out:
	kfree_skb(nskb);
	return err;
}

/* Called with genl_mutex. */
static int flush_flows(int dp_ifindex)
{
	struct flow_table *old_table;
	struct flow_table *new_table;
	struct datapath *dp;

	dp = get_dp(dp_ifindex);
	if (!dp)
		return -ENODEV;

	old_table = genl_dereference(dp->table);
	new_table = ovs_flow_tbl_alloc(TBL_MIN_BUCKETS);
	if (!new_table)
		return -ENOMEM;

	rcu_assign_pointer(dp->table, new_table);

	ovs_flow_tbl_deferred_destroy(old_table);
	return 0;
}

static int validate_actions(const struct nlattr *attr,
				const struct sw_flow_key *key, int depth);

static int validate_sample(const struct nlattr *attr,
				const struct sw_flow_key *key, int depth)
{
	const struct nlattr *attrs[OVS_SAMPLE_ATTR_MAX + 1];
	const struct nlattr *probability, *actions;
	const struct nlattr *a;
	int rem;

	memset(attrs, 0, sizeof(attrs));
	nla_for_each_nested(a, attr, rem) {
		int type = nla_type(a);
		if (!type || type > OVS_SAMPLE_ATTR_MAX || attrs[type])
			return -EINVAL;
		attrs[type] = a;
	}
	if (rem)
		return -EINVAL;

	probability = attrs[OVS_SAMPLE_ATTR_PROBABILITY];
	if (!probability || nla_len(probability) != sizeof(u32))
		return -EINVAL;

	actions = attrs[OVS_SAMPLE_ATTR_ACTIONS];
	if (!actions || (nla_len(actions) && nla_len(actions) < NLA_HDRLEN))
		return -EINVAL;
	return validate_actions(actions, key, depth + 1);
}

static int validate_tp_port(const struct sw_flow_key *flow_key)
{
	if (flow_key->eth.type == htons(ETH_P_IP)) {
		if (flow_key->ipv4.tp.src && flow_key->ipv4.tp.dst)
			return 0;
	} else if (flow_key->eth.type == htons(ETH_P_IPV6)) {
		if (flow_key->ipv6.tp.src && flow_key->ipv6.tp.dst)
			return 0;
	}

	return -EINVAL;
}

static int validate_set(const struct nlattr *a,
			const struct sw_flow_key *flow_key)
{
	const struct nlattr *ovs_key = nla_data(a);
	int key_type = nla_type(ovs_key);

	/* There can be only one key in a action */
	if (nla_total_size(nla_len(ovs_key)) != nla_len(a))
		return -EINVAL;

	if (key_type > OVS_KEY_ATTR_MAX ||
	    nla_len(ovs_key) != ovs_key_lens[key_type])
		return -EINVAL;

	switch (key_type) {
	const struct ovs_key_ipv4 *ipv4_key;

	case OVS_KEY_ATTR_PRIORITY:
	case OVS_KEY_ATTR_ETHERNET:
		break;

	case OVS_KEY_ATTR_IPV4:
		if (flow_key->eth.type != htons(ETH_P_IP))
			return -EINVAL;

		if (!flow_key->ipv4.addr.src || !flow_key->ipv4.addr.dst)
			return -EINVAL;

		ipv4_key = nla_data(ovs_key);
		if (ipv4_key->ipv4_proto != flow_key->ip.proto)
			return -EINVAL;

		if (ipv4_key->ipv4_frag != flow_key->ip.frag)
			return -EINVAL;

		break;

	case OVS_KEY_ATTR_TCP:
		if (flow_key->ip.proto != IPPROTO_TCP)
			return -EINVAL;

		return validate_tp_port(flow_key);

	case OVS_KEY_ATTR_UDP:
		if (flow_key->ip.proto != IPPROTO_UDP)
			return -EINVAL;

		return validate_tp_port(flow_key);

	default:
		return -EINVAL;
	}

	return 0;
}

static int validate_userspace(const struct nlattr *attr)
{
	static const struct nla_policy userspace_policy[OVS_USERSPACE_ATTR_MAX + 1] =	{
		[OVS_USERSPACE_ATTR_PID] = {.type = NLA_U32 },
		[OVS_USERSPACE_ATTR_USERDATA] = {.type = NLA_U64 },
	};
	struct nlattr *a[OVS_USERSPACE_ATTR_MAX + 1];
	int error;

	error = nla_parse_nested(a, OVS_USERSPACE_ATTR_MAX,
				 attr, userspace_policy);
	if (error)
		return error;

	if (!a[OVS_USERSPACE_ATTR_PID] ||
	    !nla_get_u32(a[OVS_USERSPACE_ATTR_PID]))
		return -EINVAL;

	return 0;
}

static int validate_actions(const struct nlattr *attr,
				const struct sw_flow_key *key,  int depth)
{
	const struct nlattr *a;
	int rem, err;

	if (depth >= SAMPLE_ACTION_DEPTH)
		return -EOVERFLOW;

	nla_for_each_nested(a, attr, rem) {
		/* Expected argument lengths, (u32)-1 for variable length. */
		static const u32 action_lens[OVS_ACTION_ATTR_MAX + 1] = {
			[OVS_ACTION_ATTR_OUTPUT] = sizeof(u32),
			[OVS_ACTION_ATTR_USERSPACE] = (u32)-1,
			[OVS_ACTION_ATTR_PUSH_VLAN] = sizeof(struct ovs_action_push_vlan),
			[OVS_ACTION_ATTR_POP_VLAN] = 0,
			[OVS_ACTION_ATTR_SET] = (u32)-1,
			[OVS_ACTION_ATTR_SAMPLE] = (u32)-1
		};
		const struct ovs_action_push_vlan *vlan;
		int type = nla_type(a);

		if (type > OVS_ACTION_ATTR_MAX ||
		    (action_lens[type] != nla_len(a) &&
		     action_lens[type] != (u32)-1))
			return -EINVAL;

		switch (type) {
		case OVS_ACTION_ATTR_UNSPEC:
			return -EINVAL;

		case OVS_ACTION_ATTR_USERSPACE:
			err = validate_userspace(a);
			if (err)
				return err;
			break;

		case OVS_ACTION_ATTR_OUTPUT:
			if (nla_get_u32(a) >= DP_MAX_PORTS)
				return -EINVAL;
			break;


		case OVS_ACTION_ATTR_POP_VLAN:
			break;

		case OVS_ACTION_ATTR_PUSH_VLAN:
			vlan = nla_data(a);
			if (vlan->vlan_tpid != htons(ETH_P_8021Q))
				return -EINVAL;
			if (!(vlan->vlan_tci & htons(VLAN_TAG_PRESENT)))
				return -EINVAL;
			break;

		case OVS_ACTION_ATTR_SET:
			err = validate_set(a, key);
			if (err)
				return err;
			break;

		case OVS_ACTION_ATTR_SAMPLE:
			err = validate_sample(a, key, depth);
			if (err)
				return err;
			break;

		default:
			return -EINVAL;
		}
	}

	if (rem > 0)
		return -EINVAL;

	return 0;
}

static void clear_stats(struct sw_flow *flow)
{
	flow->used = 0;
	flow->tcp_flags = 0;
	flow->packet_count = 0;
	flow->byte_count = 0;
=======
	if (!upcall) {
		err = -EINVAL;
		goto out;
	}
	upcall->dp_ifindex = dp_ifindex;

	err = ovs_nla_put_key(key, key, OVS_PACKET_ATTR_KEY, false, user_skb);
	if (err)
		goto out;

	if (upcall_info->userdata)
		__nla_put(user_skb, OVS_PACKET_ATTR_USERDATA,
			  nla_len(upcall_info->userdata),
			  nla_data(upcall_info->userdata));

	if (upcall_info->egress_tun_info) {
		nla = nla_nest_start_noflag(user_skb,
					    OVS_PACKET_ATTR_EGRESS_TUN_KEY);
		if (!nla) {
			err = -EMSGSIZE;
			goto out;
		}
		err = ovs_nla_put_tunnel_info(user_skb,
					      upcall_info->egress_tun_info);
		if (err)
			goto out;

		nla_nest_end(user_skb, nla);
	}

	if (upcall_info->actions_len) {
		nla = nla_nest_start_noflag(user_skb, OVS_PACKET_ATTR_ACTIONS);
		if (!nla) {
			err = -EMSGSIZE;
			goto out;
		}
		err = ovs_nla_put_actions(upcall_info->actions,
					  upcall_info->actions_len,
					  user_skb);
		if (!err)
			nla_nest_end(user_skb, nla);
		else
			nla_nest_cancel(user_skb, nla);
	}

	/* Add OVS_PACKET_ATTR_MRU */
	if (upcall_info->mru &&
	    nla_put_u16(user_skb, OVS_PACKET_ATTR_MRU, upcall_info->mru)) {
		err = -ENOBUFS;
		goto out;
	}

	/* Add OVS_PACKET_ATTR_LEN when packet is truncated */
	if (cutlen > 0 &&
	    nla_put_u32(user_skb, OVS_PACKET_ATTR_LEN, skb->len)) {
		err = -ENOBUFS;
		goto out;
	}

	/* Add OVS_PACKET_ATTR_HASH */
	hash = skb_get_hash_raw(skb);
	if (skb->sw_hash)
		hash |= OVS_PACKET_HASH_SW_BIT;

	if (skb->l4_hash)
		hash |= OVS_PACKET_HASH_L4_BIT;

	if (nla_put(user_skb, OVS_PACKET_ATTR_HASH, sizeof (u64), &hash)) {
		err = -ENOBUFS;
		goto out;
	}

	/* Only reserve room for attribute header, packet data is added
	 * in skb_zerocopy() */
	if (!(nla = nla_reserve(user_skb, OVS_PACKET_ATTR_PACKET, 0))) {
		err = -ENOBUFS;
		goto out;
	}
	nla->nla_len = nla_attr_size(skb->len - cutlen);

	err = skb_zerocopy(user_skb, skb, skb->len - cutlen, hlen);
	if (err)
		goto out;

	/* Pad OVS_PACKET_ATTR_PACKET if linear copy was performed */
	pad_packet(dp, user_skb);

	((struct nlmsghdr *) user_skb->data)->nlmsg_len = user_skb->len;

	err = genlmsg_unicast(ovs_dp_get_net(dp), user_skb, upcall_info->portid);
	user_skb = NULL;
out:
	if (err)
		skb_tx_error(skb);
	consume_skb(user_skb);
	consume_skb(nskb);

	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int ovs_packet_cmd_execute(struct sk_buff *skb, struct genl_info *info)
{
<<<<<<< HEAD
	struct ovs_header *ovs_header = info->userhdr;
=======
	struct ovs_header *ovs_header = genl_info_userhdr(info);
	struct net *net = sock_net(skb->sk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct nlattr **a = info->attrs;
	struct sw_flow_actions *acts;
	struct sk_buff *packet;
	struct sw_flow *flow;
<<<<<<< HEAD
	struct datapath *dp;
	struct ethhdr *eth;
	int len;
	int err;
	int key_len;

	err = -EINVAL;
	if (!a[OVS_PACKET_ATTR_PACKET] || !a[OVS_PACKET_ATTR_KEY] ||
	    !a[OVS_PACKET_ATTR_ACTIONS] ||
	    nla_len(a[OVS_PACKET_ATTR_PACKET]) < ETH_HLEN)
=======
	struct sw_flow_actions *sf_acts;
	struct datapath *dp;
	struct vport *input_vport;
	u16 mru = 0;
	u64 hash;
	int len;
	int err;
	bool log = !a[OVS_PACKET_ATTR_PROBE];

	err = -EINVAL;
	if (!a[OVS_PACKET_ATTR_PACKET] || !a[OVS_PACKET_ATTR_KEY] ||
	    !a[OVS_PACKET_ATTR_ACTIONS])
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto err;

	len = nla_len(a[OVS_PACKET_ATTR_PACKET]);
	packet = __dev_alloc_skb(NET_IP_ALIGN + len, GFP_KERNEL);
	err = -ENOMEM;
	if (!packet)
		goto err;
	skb_reserve(packet, NET_IP_ALIGN);

<<<<<<< HEAD
	memcpy(__skb_put(packet, len), nla_data(a[OVS_PACKET_ATTR_PACKET]), len);

	skb_reset_mac_header(packet);
	eth = eth_hdr(packet);

	/* Normally, setting the skb 'protocol' field would be handled by a
	 * call to eth_type_trans(), but it assumes there's a sending
	 * device, which we may not have. */
	if (ntohs(eth->h_proto) >= 1536)
		packet->protocol = eth->h_proto;
	else
		packet->protocol = htons(ETH_P_802_2);
=======
	nla_memcpy(__skb_put(packet, len), a[OVS_PACKET_ATTR_PACKET], len);

	/* Set packet's mru */
	if (a[OVS_PACKET_ATTR_MRU]) {
		mru = nla_get_u16(a[OVS_PACKET_ATTR_MRU]);
		packet->ignore_df = 1;
	}
	OVS_CB(packet)->mru = mru;

	if (a[OVS_PACKET_ATTR_HASH]) {
		hash = nla_get_u64(a[OVS_PACKET_ATTR_HASH]);

		__skb_set_hash(packet, hash & 0xFFFFFFFFULL,
			       !!(hash & OVS_PACKET_HASH_SW_BIT),
			       !!(hash & OVS_PACKET_HASH_L4_BIT));
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Build an sw_flow for sending this packet. */
	flow = ovs_flow_alloc();
	err = PTR_ERR(flow);
	if (IS_ERR(flow))
		goto err_kfree_skb;

<<<<<<< HEAD
	err = ovs_flow_extract(packet, -1, &flow->key, &key_len);
	if (err)
		goto err_flow_free;

	err = ovs_flow_metadata_from_nlattrs(&flow->key.phy.priority,
					     &flow->key.phy.in_port,
					     a[OVS_PACKET_ATTR_KEY]);
	if (err)
		goto err_flow_free;

	err = validate_actions(a[OVS_PACKET_ATTR_ACTIONS], &flow->key, 0);
	if (err)
		goto err_flow_free;

	flow->hash = ovs_flow_hash(&flow->key, key_len);

	acts = ovs_flow_actions_alloc(a[OVS_PACKET_ATTR_ACTIONS]);
	err = PTR_ERR(acts);
	if (IS_ERR(acts))
		goto err_flow_free;
	rcu_assign_pointer(flow->sf_acts, acts);

	OVS_CB(packet)->flow = flow;
	packet->priority = flow->key.phy.priority;

	rcu_read_lock();
	dp = get_dp(ovs_header->dp_ifindex);
=======
	err = ovs_flow_key_extract_userspace(net, a[OVS_PACKET_ATTR_KEY],
					     packet, &flow->key, log);
	if (err)
		goto err_flow_free;

	err = ovs_nla_copy_actions(net, a[OVS_PACKET_ATTR_ACTIONS],
				   &flow->key, &acts, log);
	if (err)
		goto err_flow_free;

	rcu_assign_pointer(flow->sf_acts, acts);
	packet->priority = flow->key.phy.priority;
	packet->mark = flow->key.phy.skb_mark;

	rcu_read_lock();
	dp = get_dp_rcu(net, ovs_header->dp_ifindex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	err = -ENODEV;
	if (!dp)
		goto err_unlock;

<<<<<<< HEAD
	local_bh_disable();
	err = ovs_execute_actions(dp, packet);
	local_bh_enable();
	rcu_read_unlock();

	ovs_flow_free(flow);
=======
	input_vport = ovs_vport_rcu(dp, flow->key.phy.in_port);
	if (!input_vport)
		input_vport = ovs_vport_rcu(dp, OVSP_LOCAL);

	if (!input_vport)
		goto err_unlock;

	packet->dev = input_vport->dev;
	OVS_CB(packet)->input_vport = input_vport;
	sf_acts = rcu_dereference(flow->sf_acts);

	local_bh_disable();
	err = ovs_execute_actions(dp, packet, sf_acts, &flow->key);
	local_bh_enable();
	rcu_read_unlock();

	ovs_flow_free(flow, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;

err_unlock:
	rcu_read_unlock();
err_flow_free:
<<<<<<< HEAD
	ovs_flow_free(flow);
=======
	ovs_flow_free(flow, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
err_kfree_skb:
	kfree_skb(packet);
err:
	return err;
}

static const struct nla_policy packet_policy[OVS_PACKET_ATTR_MAX + 1] = {
<<<<<<< HEAD
	[OVS_PACKET_ATTR_PACKET] = { .type = NLA_UNSPEC },
	[OVS_PACKET_ATTR_KEY] = { .type = NLA_NESTED },
	[OVS_PACKET_ATTR_ACTIONS] = { .type = NLA_NESTED },
};

static struct genl_ops dp_packet_genl_ops[] = {
	{ .cmd = OVS_PACKET_CMD_EXECUTE,
	  .flags = GENL_ADMIN_PERM, /* Requires CAP_NET_ADMIN privilege. */
	  .policy = packet_policy,
=======
	[OVS_PACKET_ATTR_PACKET] = { .len = ETH_HLEN },
	[OVS_PACKET_ATTR_KEY] = { .type = NLA_NESTED },
	[OVS_PACKET_ATTR_ACTIONS] = { .type = NLA_NESTED },
	[OVS_PACKET_ATTR_PROBE] = { .type = NLA_FLAG },
	[OVS_PACKET_ATTR_MRU] = { .type = NLA_U16 },
	[OVS_PACKET_ATTR_HASH] = { .type = NLA_U64 },
};

static const struct genl_small_ops dp_packet_genl_ops[] = {
	{ .cmd = OVS_PACKET_CMD_EXECUTE,
	  .validate = GENL_DONT_VALIDATE_STRICT | GENL_DONT_VALIDATE_DUMP,
	  .flags = GENL_UNS_ADMIN_PERM, /* Requires CAP_NET_ADMIN privilege. */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	  .doit = ovs_packet_cmd_execute
	}
};

<<<<<<< HEAD
static void get_dp_stats(struct datapath *dp, struct ovs_dp_stats *stats)
{
	int i;
	struct flow_table *table = genl_dereference(dp->table);

	stats->n_flows = ovs_flow_tbl_count(table);

	stats->n_hit = stats->n_missed = stats->n_lost = 0;
=======
static struct genl_family dp_packet_genl_family __ro_after_init = {
	.hdrsize = sizeof(struct ovs_header),
	.name = OVS_PACKET_FAMILY,
	.version = OVS_PACKET_VERSION,
	.maxattr = OVS_PACKET_ATTR_MAX,
	.policy = packet_policy,
	.netnsok = true,
	.parallel_ops = true,
	.small_ops = dp_packet_genl_ops,
	.n_small_ops = ARRAY_SIZE(dp_packet_genl_ops),
	.resv_start_op = OVS_PACKET_CMD_EXECUTE + 1,
	.module = THIS_MODULE,
};

static void get_dp_stats(const struct datapath *dp, struct ovs_dp_stats *stats,
			 struct ovs_dp_megaflow_stats *mega_stats)
{
	int i;

	memset(mega_stats, 0, sizeof(*mega_stats));

	stats->n_flows = ovs_flow_tbl_count(&dp->table);
	mega_stats->n_masks = ovs_flow_tbl_num_masks(&dp->table);

	stats->n_hit = stats->n_missed = stats->n_lost = 0;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	for_each_possible_cpu(i) {
		const struct dp_stats_percpu *percpu_stats;
		struct dp_stats_percpu local_stats;
		unsigned int start;

		percpu_stats = per_cpu_ptr(dp->stats_percpu, i);

		do {
<<<<<<< HEAD
			start = u64_stats_fetch_begin_irq(&percpu_stats->sync);
			local_stats = *percpu_stats;
		} while (u64_stats_fetch_retry_irq(&percpu_stats->sync, start));
=======
			start = u64_stats_fetch_begin(&percpu_stats->syncp);
			local_stats = *percpu_stats;
		} while (u64_stats_fetch_retry(&percpu_stats->syncp, start));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		stats->n_hit += local_stats.n_hit;
		stats->n_missed += local_stats.n_missed;
		stats->n_lost += local_stats.n_lost;
<<<<<<< HEAD
	}
}

static const struct nla_policy flow_policy[OVS_FLOW_ATTR_MAX + 1] = {
	[OVS_FLOW_ATTR_KEY] = { .type = NLA_NESTED },
	[OVS_FLOW_ATTR_ACTIONS] = { .type = NLA_NESTED },
	[OVS_FLOW_ATTR_CLEAR] = { .type = NLA_FLAG },
};

static struct genl_family dp_flow_genl_family = {
	.id = GENL_ID_GENERATE,
	.hdrsize = sizeof(struct ovs_header),
	.name = OVS_FLOW_FAMILY,
	.version = OVS_FLOW_VERSION,
	.maxattr = OVS_FLOW_ATTR_MAX
};

static struct genl_multicast_group ovs_dp_flow_multicast_group = {
	.name = OVS_FLOW_MCGROUP
};

/* Called with genl_lock. */
static int ovs_flow_cmd_fill_info(struct sw_flow *flow, struct datapath *dp,
				  struct sk_buff *skb, u32 pid,
				  u32 seq, u32 flags, u8 cmd)
{
	const int skb_orig_len = skb->len;
	const struct sw_flow_actions *sf_acts;
	struct ovs_flow_stats stats;
	struct ovs_header *ovs_header;
	struct nlattr *nla;
	unsigned long used;
	u8 tcp_flags;
	int err;

	sf_acts = rcu_dereference_protected(flow->sf_acts,
					    lockdep_genl_is_held());

	ovs_header = genlmsg_put(skb, pid, seq, &dp_flow_genl_family, flags, cmd);
	if (!ovs_header)
		return -EMSGSIZE;

	ovs_header->dp_ifindex = get_dpifindex(dp);

	nla = nla_nest_start(skb, OVS_FLOW_ATTR_KEY);
	if (!nla)
		goto nla_put_failure;
	err = ovs_flow_to_nlattrs(&flow->key, skb);
	if (err)
		goto error;
	nla_nest_end(skb, nla);

	spin_lock_bh(&flow->lock);
	used = flow->used;
	stats.n_packets = flow->packet_count;
	stats.n_bytes = flow->byte_count;
	tcp_flags = flow->tcp_flags;
	spin_unlock_bh(&flow->lock);

	if (used)
		NLA_PUT_U64(skb, OVS_FLOW_ATTR_USED, ovs_flow_used_time(used));

	if (stats.n_packets)
		NLA_PUT(skb, OVS_FLOW_ATTR_STATS,
			sizeof(struct ovs_flow_stats), &stats);

	if (tcp_flags)
		NLA_PUT_U8(skb, OVS_FLOW_ATTR_TCP_FLAGS, tcp_flags);
=======
		mega_stats->n_mask_hit += local_stats.n_mask_hit;
		mega_stats->n_cache_hit += local_stats.n_cache_hit;
	}
}

static bool should_fill_key(const struct sw_flow_id *sfid, uint32_t ufid_flags)
{
	return ovs_identifier_is_ufid(sfid) &&
	       !(ufid_flags & OVS_UFID_F_OMIT_KEY);
}

static bool should_fill_mask(uint32_t ufid_flags)
{
	return !(ufid_flags & OVS_UFID_F_OMIT_MASK);
}

static bool should_fill_actions(uint32_t ufid_flags)
{
	return !(ufid_flags & OVS_UFID_F_OMIT_ACTIONS);
}

static size_t ovs_flow_cmd_msg_size(const struct sw_flow_actions *acts,
				    const struct sw_flow_id *sfid,
				    uint32_t ufid_flags)
{
	size_t len = NLMSG_ALIGN(sizeof(struct ovs_header));

	/* OVS_FLOW_ATTR_UFID, or unmasked flow key as fallback
	 * see ovs_nla_put_identifier()
	 */
	if (sfid && ovs_identifier_is_ufid(sfid))
		len += nla_total_size(sfid->ufid_len);
	else
		len += nla_total_size(ovs_key_attr_size());

	/* OVS_FLOW_ATTR_KEY */
	if (!sfid || should_fill_key(sfid, ufid_flags))
		len += nla_total_size(ovs_key_attr_size());

	/* OVS_FLOW_ATTR_MASK */
	if (should_fill_mask(ufid_flags))
		len += nla_total_size(ovs_key_attr_size());

	/* OVS_FLOW_ATTR_ACTIONS */
	if (should_fill_actions(ufid_flags))
		len += nla_total_size(acts->orig_len);

	return len
		+ nla_total_size_64bit(sizeof(struct ovs_flow_stats)) /* OVS_FLOW_ATTR_STATS */
		+ nla_total_size(1) /* OVS_FLOW_ATTR_TCP_FLAGS */
		+ nla_total_size_64bit(8); /* OVS_FLOW_ATTR_USED */
}

/* Called with ovs_mutex or RCU read lock. */
static int ovs_flow_cmd_fill_stats(const struct sw_flow *flow,
				   struct sk_buff *skb)
{
	struct ovs_flow_stats stats;
	__be16 tcp_flags;
	unsigned long used;

	ovs_flow_stats_get(flow, &stats, &used, &tcp_flags);

	if (used &&
	    nla_put_u64_64bit(skb, OVS_FLOW_ATTR_USED, ovs_flow_used_time(used),
			      OVS_FLOW_ATTR_PAD))
		return -EMSGSIZE;

	if (stats.n_packets &&
	    nla_put_64bit(skb, OVS_FLOW_ATTR_STATS,
			  sizeof(struct ovs_flow_stats), &stats,
			  OVS_FLOW_ATTR_PAD))
		return -EMSGSIZE;

	if ((u8)ntohs(tcp_flags) &&
	     nla_put_u8(skb, OVS_FLOW_ATTR_TCP_FLAGS, (u8)ntohs(tcp_flags)))
		return -EMSGSIZE;

	return 0;
}

/* Called with ovs_mutex or RCU read lock. */
static int ovs_flow_cmd_fill_actions(const struct sw_flow *flow,
				     struct sk_buff *skb, int skb_orig_len)
{
	struct nlattr *start;
	int err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* If OVS_FLOW_ATTR_ACTIONS doesn't fit, skip dumping the actions if
	 * this is the first flow to be dumped into 'skb'.  This is unusual for
	 * Netlink but individual action lists can be longer than
	 * NLMSG_GOODSIZE and thus entirely undumpable if we didn't do this.
	 * The userspace caller can always fetch the actions separately if it
	 * really wants them.  (Most userspace callers in fact don't care.)
	 *
	 * This can only fail for dump operations because the skb is always
	 * properly sized for single flows.
	 */
<<<<<<< HEAD
	err = nla_put(skb, OVS_FLOW_ATTR_ACTIONS, sf_acts->actions_len,
		      sf_acts->actions);
	if (err < 0 && skb_orig_len)
		goto error;

	return genlmsg_end(skb, ovs_header);

nla_put_failure:
	err = -EMSGSIZE;
=======
	start = nla_nest_start_noflag(skb, OVS_FLOW_ATTR_ACTIONS);
	if (start) {
		const struct sw_flow_actions *sf_acts;

		sf_acts = rcu_dereference_ovsl(flow->sf_acts);
		err = ovs_nla_put_actions(sf_acts->actions,
					  sf_acts->actions_len, skb);

		if (!err)
			nla_nest_end(skb, start);
		else {
			if (skb_orig_len)
				return err;

			nla_nest_cancel(skb, start);
		}
	} else if (skb_orig_len) {
		return -EMSGSIZE;
	}

	return 0;
}

/* Called with ovs_mutex or RCU read lock. */
static int ovs_flow_cmd_fill_info(const struct sw_flow *flow, int dp_ifindex,
				  struct sk_buff *skb, u32 portid,
				  u32 seq, u32 flags, u8 cmd, u32 ufid_flags)
{
	const int skb_orig_len = skb->len;
	struct ovs_header *ovs_header;
	int err;

	ovs_header = genlmsg_put(skb, portid, seq, &dp_flow_genl_family,
				 flags, cmd);
	if (!ovs_header)
		return -EMSGSIZE;

	ovs_header->dp_ifindex = dp_ifindex;

	err = ovs_nla_put_identifier(flow, skb);
	if (err)
		goto error;

	if (should_fill_key(&flow->id, ufid_flags)) {
		err = ovs_nla_put_masked_key(flow, skb);
		if (err)
			goto error;
	}

	if (should_fill_mask(ufid_flags)) {
		err = ovs_nla_put_mask(flow, skb);
		if (err)
			goto error;
	}

	err = ovs_flow_cmd_fill_stats(flow, skb);
	if (err)
		goto error;

	if (should_fill_actions(ufid_flags)) {
		err = ovs_flow_cmd_fill_actions(flow, skb, skb_orig_len);
		if (err)
			goto error;
	}

	genlmsg_end(skb, ovs_header);
	return 0;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
error:
	genlmsg_cancel(skb, ovs_header);
	return err;
}

<<<<<<< HEAD
static struct sk_buff *ovs_flow_cmd_alloc_info(struct sw_flow *flow)
{
	const struct sw_flow_actions *sf_acts;
	int len;

	sf_acts = rcu_dereference_protected(flow->sf_acts,
					    lockdep_genl_is_held());

	/* OVS_FLOW_ATTR_KEY */
	len = nla_total_size(FLOW_BUFSIZE);
	/* OVS_FLOW_ATTR_ACTIONS */
	len += nla_total_size(sf_acts->actions_len);
	/* OVS_FLOW_ATTR_STATS */
	len += nla_total_size(sizeof(struct ovs_flow_stats));
	/* OVS_FLOW_ATTR_TCP_FLAGS */
	len += nla_total_size(1);
	/* OVS_FLOW_ATTR_USED */
	len += nla_total_size(8);

	len += NLMSG_ALIGN(sizeof(struct ovs_header));

	return genlmsg_new(len, GFP_KERNEL);
}

static struct sk_buff *ovs_flow_cmd_build_info(struct sw_flow *flow,
					       struct datapath *dp,
					       u32 pid, u32 seq, u8 cmd)
=======
/* May not be called with RCU read lock. */
static struct sk_buff *ovs_flow_cmd_alloc_info(const struct sw_flow_actions *acts,
					       const struct sw_flow_id *sfid,
					       struct genl_info *info,
					       bool always,
					       uint32_t ufid_flags)
{
	struct sk_buff *skb;
	size_t len;

	if (!always && !ovs_must_notify(&dp_flow_genl_family, info, 0))
		return NULL;

	len = ovs_flow_cmd_msg_size(acts, sfid, ufid_flags);
	skb = genlmsg_new(len, GFP_KERNEL);
	if (!skb)
		return ERR_PTR(-ENOMEM);

	return skb;
}

/* Called with ovs_mutex. */
static struct sk_buff *ovs_flow_cmd_build_info(const struct sw_flow *flow,
					       int dp_ifindex,
					       struct genl_info *info, u8 cmd,
					       bool always, u32 ufid_flags)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct sk_buff *skb;
	int retval;

<<<<<<< HEAD
	skb = ovs_flow_cmd_alloc_info(flow);
	if (!skb)
		return ERR_PTR(-ENOMEM);

	retval = ovs_flow_cmd_fill_info(flow, dp, skb, pid, seq, 0, cmd);
	BUG_ON(retval < 0);
	return skb;
}

static int ovs_flow_cmd_new_or_set(struct sk_buff *skb, struct genl_info *info)
{
	struct nlattr **a = info->attrs;
	struct ovs_header *ovs_header = info->userhdr;
	struct sw_flow_key key;
	struct sw_flow *flow;
	struct sk_buff *reply;
	struct datapath *dp;
	struct flow_table *table;
	int error;
	int key_len;

	/* Extract key. */
	error = -EINVAL;
	if (!a[OVS_FLOW_ATTR_KEY])
		goto error;
	error = ovs_flow_from_nlattrs(&key, &key_len, a[OVS_FLOW_ATTR_KEY]);
	if (error)
		goto error;

	/* Validate actions. */
	if (a[OVS_FLOW_ATTR_ACTIONS]) {
		error = validate_actions(a[OVS_FLOW_ATTR_ACTIONS], &key,  0);
		if (error)
			goto error;
	} else if (info->genlhdr->cmd == OVS_FLOW_CMD_NEW) {
		error = -EINVAL;
		goto error;
	}

	dp = get_dp(ovs_header->dp_ifindex);
	error = -ENODEV;
	if (!dp)
		goto error;

	table = genl_dereference(dp->table);
	flow = ovs_flow_tbl_lookup(table, &key, key_len);
	if (!flow) {
		struct sw_flow_actions *acts;

		/* Bail out if we're not allowed to create a new flow. */
		error = -ENOENT;
		if (info->genlhdr->cmd == OVS_FLOW_CMD_SET)
			goto error;

		/* Expand table, if necessary, to make room. */
		if (ovs_flow_tbl_need_to_expand(table)) {
			struct flow_table *new_table;

			new_table = ovs_flow_tbl_expand(table);
			if (!IS_ERR(new_table)) {
				rcu_assign_pointer(dp->table, new_table);
				ovs_flow_tbl_deferred_destroy(table);
				table = genl_dereference(dp->table);
			}
		}

		/* Allocate flow. */
		flow = ovs_flow_alloc();
		if (IS_ERR(flow)) {
			error = PTR_ERR(flow);
			goto error;
		}
		flow->key = key;
		clear_stats(flow);

		/* Obtain actions. */
		acts = ovs_flow_actions_alloc(a[OVS_FLOW_ATTR_ACTIONS]);
		error = PTR_ERR(acts);
		if (IS_ERR(acts))
			goto error_free_flow;
		rcu_assign_pointer(flow->sf_acts, acts);

		/* Put flow in bucket. */
		flow->hash = ovs_flow_hash(&key, key_len);
		ovs_flow_tbl_insert(table, flow);

		reply = ovs_flow_cmd_build_info(flow, dp, info->snd_pid,
						info->snd_seq,
						OVS_FLOW_CMD_NEW);
	} else {
		/* We found a matching flow. */
		struct sw_flow_actions *old_acts;
		struct nlattr *acts_attrs;
=======
	skb = ovs_flow_cmd_alloc_info(ovsl_dereference(flow->sf_acts),
				      &flow->id, info, always, ufid_flags);
	if (IS_ERR_OR_NULL(skb))
		return skb;

	retval = ovs_flow_cmd_fill_info(flow, dp_ifindex, skb,
					info->snd_portid, info->snd_seq, 0,
					cmd, ufid_flags);
	if (WARN_ON_ONCE(retval < 0)) {
		kfree_skb(skb);
		skb = ERR_PTR(retval);
	}
	return skb;
}

static int ovs_flow_cmd_new(struct sk_buff *skb, struct genl_info *info)
{
	struct net *net = sock_net(skb->sk);
	struct nlattr **a = info->attrs;
	struct ovs_header *ovs_header = genl_info_userhdr(info);
	struct sw_flow *flow = NULL, *new_flow;
	struct sw_flow_mask mask;
	struct sk_buff *reply;
	struct datapath *dp;
	struct sw_flow_key *key;
	struct sw_flow_actions *acts;
	struct sw_flow_match match;
	u32 ufid_flags = ovs_nla_get_ufid_flags(a[OVS_FLOW_ATTR_UFID_FLAGS]);
	int error;
	bool log = !a[OVS_FLOW_ATTR_PROBE];

	/* Must have key and actions. */
	error = -EINVAL;
	if (!a[OVS_FLOW_ATTR_KEY]) {
		OVS_NLERR(log, "Flow key attr not present in new flow.");
		goto error;
	}
	if (!a[OVS_FLOW_ATTR_ACTIONS]) {
		OVS_NLERR(log, "Flow actions attr not present in new flow.");
		goto error;
	}

	/* Most of the time we need to allocate a new flow, do it before
	 * locking.
	 */
	new_flow = ovs_flow_alloc();
	if (IS_ERR(new_flow)) {
		error = PTR_ERR(new_flow);
		goto error;
	}

	/* Extract key. */
	key = kzalloc(sizeof(*key), GFP_KERNEL);
	if (!key) {
		error = -ENOMEM;
		goto err_kfree_flow;
	}

	ovs_match_init(&match, key, false, &mask);
	error = ovs_nla_get_match(net, &match, a[OVS_FLOW_ATTR_KEY],
				  a[OVS_FLOW_ATTR_MASK], log);
	if (error)
		goto err_kfree_key;

	ovs_flow_mask_key(&new_flow->key, key, true, &mask);

	/* Extract flow identifier. */
	error = ovs_nla_get_identifier(&new_flow->id, a[OVS_FLOW_ATTR_UFID],
				       key, log);
	if (error)
		goto err_kfree_key;

	/* Validate actions. */
	error = ovs_nla_copy_actions(net, a[OVS_FLOW_ATTR_ACTIONS],
				     &new_flow->key, &acts, log);
	if (error) {
		OVS_NLERR(log, "Flow actions may not be safe on all matching packets.");
		goto err_kfree_key;
	}

	reply = ovs_flow_cmd_alloc_info(acts, &new_flow->id, info, false,
					ufid_flags);
	if (IS_ERR(reply)) {
		error = PTR_ERR(reply);
		goto err_kfree_acts;
	}

	ovs_lock();
	dp = get_dp(net, ovs_header->dp_ifindex);
	if (unlikely(!dp)) {
		error = -ENODEV;
		goto err_unlock_ovs;
	}

	/* Check if this is a duplicate flow */
	if (ovs_identifier_is_ufid(&new_flow->id))
		flow = ovs_flow_tbl_lookup_ufid(&dp->table, &new_flow->id);
	if (!flow)
		flow = ovs_flow_tbl_lookup(&dp->table, key);
	if (likely(!flow)) {
		rcu_assign_pointer(new_flow->sf_acts, acts);

		/* Put flow in bucket. */
		error = ovs_flow_tbl_insert(&dp->table, new_flow, &mask);
		if (unlikely(error)) {
			acts = NULL;
			goto err_unlock_ovs;
		}

		if (unlikely(reply)) {
			error = ovs_flow_cmd_fill_info(new_flow,
						       ovs_header->dp_ifindex,
						       reply, info->snd_portid,
						       info->snd_seq, 0,
						       OVS_FLOW_CMD_NEW,
						       ufid_flags);
			BUG_ON(error < 0);
		}
		ovs_unlock();
	} else {
		struct sw_flow_actions *old_acts;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/* Bail out if we're not allowed to modify an existing flow.
		 * We accept NLM_F_CREATE in place of the intended NLM_F_EXCL
		 * because Generic Netlink treats the latter as a dump
		 * request.  We also accept NLM_F_EXCL in case that bug ever
		 * gets fixed.
		 */
<<<<<<< HEAD
		error = -EEXIST;
		if (info->genlhdr->cmd == OVS_FLOW_CMD_NEW &&
		    info->nlhdr->nlmsg_flags & (NLM_F_CREATE | NLM_F_EXCL))
			goto error;

		/* Update actions. */
		old_acts = rcu_dereference_protected(flow->sf_acts,
						     lockdep_genl_is_held());
		acts_attrs = a[OVS_FLOW_ATTR_ACTIONS];
		if (acts_attrs &&
		   (old_acts->actions_len != nla_len(acts_attrs) ||
		   memcmp(old_acts->actions, nla_data(acts_attrs),
			  old_acts->actions_len))) {
			struct sw_flow_actions *new_acts;

			new_acts = ovs_flow_actions_alloc(acts_attrs);
			error = PTR_ERR(new_acts);
			if (IS_ERR(new_acts))
				goto error;

			rcu_assign_pointer(flow->sf_acts, new_acts);
			ovs_flow_deferred_free_acts(old_acts);
		}

		reply = ovs_flow_cmd_build_info(flow, dp, info->snd_pid,
					       info->snd_seq, OVS_FLOW_CMD_NEW);

		/* Clear stats. */
		if (a[OVS_FLOW_ATTR_CLEAR]) {
			spin_lock_bh(&flow->lock);
			clear_stats(flow);
			spin_unlock_bh(&flow->lock);
		}
	}

	if (!IS_ERR(reply))
		genl_notify(reply, genl_info_net(info), info->snd_pid,
			   ovs_dp_flow_multicast_group.id, info->nlhdr,
			   GFP_KERNEL);
	else
		netlink_set_err(init_net.genl_sock, 0,
				ovs_dp_flow_multicast_group.id, PTR_ERR(reply));
	return 0;

error_free_flow:
	ovs_flow_free(flow);
=======
		if (unlikely(info->nlhdr->nlmsg_flags & (NLM_F_CREATE
							 | NLM_F_EXCL))) {
			error = -EEXIST;
			goto err_unlock_ovs;
		}
		/* The flow identifier has to be the same for flow updates.
		 * Look for any overlapping flow.
		 */
		if (unlikely(!ovs_flow_cmp(flow, &match))) {
			if (ovs_identifier_is_key(&flow->id))
				flow = ovs_flow_tbl_lookup_exact(&dp->table,
								 &match);
			else /* UFID matches but key is different */
				flow = NULL;
			if (!flow) {
				error = -ENOENT;
				goto err_unlock_ovs;
			}
		}
		/* Update actions. */
		old_acts = ovsl_dereference(flow->sf_acts);
		rcu_assign_pointer(flow->sf_acts, acts);

		if (unlikely(reply)) {
			error = ovs_flow_cmd_fill_info(flow,
						       ovs_header->dp_ifindex,
						       reply, info->snd_portid,
						       info->snd_seq, 0,
						       OVS_FLOW_CMD_NEW,
						       ufid_flags);
			BUG_ON(error < 0);
		}
		ovs_unlock();

		ovs_nla_free_flow_actions_rcu(old_acts);
		ovs_flow_free(new_flow, false);
	}

	if (reply)
		ovs_notify(&dp_flow_genl_family, reply, info);

	kfree(key);
	return 0;

err_unlock_ovs:
	ovs_unlock();
	kfree_skb(reply);
err_kfree_acts:
	ovs_nla_free_flow_actions(acts);
err_kfree_key:
	kfree(key);
err_kfree_flow:
	ovs_flow_free(new_flow, false);
error:
	return error;
}

/* Factor out action copy to avoid "Wframe-larger-than=1024" warning. */
static noinline_for_stack
struct sw_flow_actions *get_flow_actions(struct net *net,
					 const struct nlattr *a,
					 const struct sw_flow_key *key,
					 const struct sw_flow_mask *mask,
					 bool log)
{
	struct sw_flow_actions *acts;
	struct sw_flow_key masked_key;
	int error;

	ovs_flow_mask_key(&masked_key, key, true, mask);
	error = ovs_nla_copy_actions(net, a, &masked_key, &acts, log);
	if (error) {
		OVS_NLERR(log,
			  "Actions may not be safe on all matching packets");
		return ERR_PTR(error);
	}

	return acts;
}

/* Factor out match-init and action-copy to avoid
 * "Wframe-larger-than=1024" warning. Because mask is only
 * used to get actions, we new a function to save some
 * stack space.
 *
 * If there are not key and action attrs, we return 0
 * directly. In the case, the caller will also not use the
 * match as before. If there is action attr, we try to get
 * actions and save them to *acts. Before returning from
 * the function, we reset the match->mask pointer. Because
 * we should not to return match object with dangling reference
 * to mask.
 * */
static noinline_for_stack int
ovs_nla_init_match_and_action(struct net *net,
			      struct sw_flow_match *match,
			      struct sw_flow_key *key,
			      struct nlattr **a,
			      struct sw_flow_actions **acts,
			      bool log)
{
	struct sw_flow_mask mask;
	int error = 0;

	if (a[OVS_FLOW_ATTR_KEY]) {
		ovs_match_init(match, key, true, &mask);
		error = ovs_nla_get_match(net, match, a[OVS_FLOW_ATTR_KEY],
					  a[OVS_FLOW_ATTR_MASK], log);
		if (error)
			goto error;
	}

	if (a[OVS_FLOW_ATTR_ACTIONS]) {
		if (!a[OVS_FLOW_ATTR_KEY]) {
			OVS_NLERR(log,
				  "Flow key attribute not present in set flow.");
			error = -EINVAL;
			goto error;
		}

		*acts = get_flow_actions(net, a[OVS_FLOW_ATTR_ACTIONS], key,
					 &mask, log);
		if (IS_ERR(*acts)) {
			error = PTR_ERR(*acts);
			goto error;
		}
	}

	/* On success, error is 0. */
error:
	match->mask = NULL;
	return error;
}

static int ovs_flow_cmd_set(struct sk_buff *skb, struct genl_info *info)
{
	struct net *net = sock_net(skb->sk);
	struct nlattr **a = info->attrs;
	struct ovs_header *ovs_header = genl_info_userhdr(info);
	struct sw_flow_key key;
	struct sw_flow *flow;
	struct sk_buff *reply = NULL;
	struct datapath *dp;
	struct sw_flow_actions *old_acts = NULL, *acts = NULL;
	struct sw_flow_match match;
	struct sw_flow_id sfid;
	u32 ufid_flags = ovs_nla_get_ufid_flags(a[OVS_FLOW_ATTR_UFID_FLAGS]);
	int error = 0;
	bool log = !a[OVS_FLOW_ATTR_PROBE];
	bool ufid_present;

	ufid_present = ovs_nla_get_ufid(&sfid, a[OVS_FLOW_ATTR_UFID], log);
	if (!a[OVS_FLOW_ATTR_KEY] && !ufid_present) {
		OVS_NLERR(log,
			  "Flow set message rejected, Key attribute missing.");
		return -EINVAL;
	}

	error = ovs_nla_init_match_and_action(net, &match, &key, a,
					      &acts, log);
	if (error)
		goto error;

	if (acts) {
		/* Can allocate before locking if have acts. */
		reply = ovs_flow_cmd_alloc_info(acts, &sfid, info, false,
						ufid_flags);
		if (IS_ERR(reply)) {
			error = PTR_ERR(reply);
			goto err_kfree_acts;
		}
	}

	ovs_lock();
	dp = get_dp(net, ovs_header->dp_ifindex);
	if (unlikely(!dp)) {
		error = -ENODEV;
		goto err_unlock_ovs;
	}
	/* Check that the flow exists. */
	if (ufid_present)
		flow = ovs_flow_tbl_lookup_ufid(&dp->table, &sfid);
	else
		flow = ovs_flow_tbl_lookup_exact(&dp->table, &match);
	if (unlikely(!flow)) {
		error = -ENOENT;
		goto err_unlock_ovs;
	}

	/* Update actions, if present. */
	if (likely(acts)) {
		old_acts = ovsl_dereference(flow->sf_acts);
		rcu_assign_pointer(flow->sf_acts, acts);

		if (unlikely(reply)) {
			error = ovs_flow_cmd_fill_info(flow,
						       ovs_header->dp_ifindex,
						       reply, info->snd_portid,
						       info->snd_seq, 0,
						       OVS_FLOW_CMD_SET,
						       ufid_flags);
			BUG_ON(error < 0);
		}
	} else {
		/* Could not alloc without acts before locking. */
		reply = ovs_flow_cmd_build_info(flow, ovs_header->dp_ifindex,
						info, OVS_FLOW_CMD_SET, false,
						ufid_flags);

		if (IS_ERR(reply)) {
			error = PTR_ERR(reply);
			goto err_unlock_ovs;
		}
	}

	/* Clear stats. */
	if (a[OVS_FLOW_ATTR_CLEAR])
		ovs_flow_stats_clear(flow);
	ovs_unlock();

	if (reply)
		ovs_notify(&dp_flow_genl_family, reply, info);
	if (old_acts)
		ovs_nla_free_flow_actions_rcu(old_acts);

	return 0;

err_unlock_ovs:
	ovs_unlock();
	kfree_skb(reply);
err_kfree_acts:
	ovs_nla_free_flow_actions(acts);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
error:
	return error;
}

static int ovs_flow_cmd_get(struct sk_buff *skb, struct genl_info *info)
{
	struct nlattr **a = info->attrs;
<<<<<<< HEAD
	struct ovs_header *ovs_header = info->userhdr;
=======
	struct ovs_header *ovs_header = genl_info_userhdr(info);
	struct net *net = sock_net(skb->sk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct sw_flow_key key;
	struct sk_buff *reply;
	struct sw_flow *flow;
	struct datapath *dp;
<<<<<<< HEAD
	struct flow_table *table;
	int err;
	int key_len;

	if (!a[OVS_FLOW_ATTR_KEY])
		return -EINVAL;
	err = ovs_flow_from_nlattrs(&key, &key_len, a[OVS_FLOW_ATTR_KEY]);
	if (err)
		return err;

	dp = get_dp(ovs_header->dp_ifindex);
	if (!dp)
		return -ENODEV;

	table = genl_dereference(dp->table);
	flow = ovs_flow_tbl_lookup(table, &key, key_len);
	if (!flow)
		return -ENOENT;

	reply = ovs_flow_cmd_build_info(flow, dp, info->snd_pid,
					info->snd_seq, OVS_FLOW_CMD_NEW);
	if (IS_ERR(reply))
		return PTR_ERR(reply);

	return genlmsg_reply(reply, info);
=======
	struct sw_flow_match match;
	struct sw_flow_id ufid;
	u32 ufid_flags = ovs_nla_get_ufid_flags(a[OVS_FLOW_ATTR_UFID_FLAGS]);
	int err = 0;
	bool log = !a[OVS_FLOW_ATTR_PROBE];
	bool ufid_present;

	ufid_present = ovs_nla_get_ufid(&ufid, a[OVS_FLOW_ATTR_UFID], log);
	if (a[OVS_FLOW_ATTR_KEY]) {
		ovs_match_init(&match, &key, true, NULL);
		err = ovs_nla_get_match(net, &match, a[OVS_FLOW_ATTR_KEY], NULL,
					log);
	} else if (!ufid_present) {
		OVS_NLERR(log,
			  "Flow get message rejected, Key attribute missing.");
		err = -EINVAL;
	}
	if (err)
		return err;

	ovs_lock();
	dp = get_dp(sock_net(skb->sk), ovs_header->dp_ifindex);
	if (!dp) {
		err = -ENODEV;
		goto unlock;
	}

	if (ufid_present)
		flow = ovs_flow_tbl_lookup_ufid(&dp->table, &ufid);
	else
		flow = ovs_flow_tbl_lookup_exact(&dp->table, &match);
	if (!flow) {
		err = -ENOENT;
		goto unlock;
	}

	reply = ovs_flow_cmd_build_info(flow, ovs_header->dp_ifindex, info,
					OVS_FLOW_CMD_GET, true, ufid_flags);
	if (IS_ERR(reply)) {
		err = PTR_ERR(reply);
		goto unlock;
	}

	ovs_unlock();
	return genlmsg_reply(reply, info);
unlock:
	ovs_unlock();
	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int ovs_flow_cmd_del(struct sk_buff *skb, struct genl_info *info)
{
	struct nlattr **a = info->attrs;
<<<<<<< HEAD
	struct ovs_header *ovs_header = info->userhdr;
	struct sw_flow_key key;
	struct sk_buff *reply;
	struct sw_flow *flow;
	struct datapath *dp;
	struct flow_table *table;
	int err;
	int key_len;

	if (!a[OVS_FLOW_ATTR_KEY])
		return flush_flows(ovs_header->dp_ifindex);
	err = ovs_flow_from_nlattrs(&key, &key_len, a[OVS_FLOW_ATTR_KEY]);
	if (err)
		return err;

	dp = get_dp(ovs_header->dp_ifindex);
	if (!dp)
		return -ENODEV;

	table = genl_dereference(dp->table);
	flow = ovs_flow_tbl_lookup(table, &key, key_len);
	if (!flow)
		return -ENOENT;

	reply = ovs_flow_cmd_alloc_info(flow);
	if (!reply)
		return -ENOMEM;

	ovs_flow_tbl_remove(table, flow);

	err = ovs_flow_cmd_fill_info(flow, dp, reply, info->snd_pid,
				     info->snd_seq, 0, OVS_FLOW_CMD_DEL);
	BUG_ON(err < 0);

	ovs_flow_deferred_free(flow);

	genl_notify(reply, genl_info_net(info), info->snd_pid,
		    ovs_dp_flow_multicast_group.id, info->nlhdr, GFP_KERNEL);
	return 0;
=======
	struct ovs_header *ovs_header = genl_info_userhdr(info);
	struct net *net = sock_net(skb->sk);
	struct sw_flow_key key;
	struct sk_buff *reply;
	struct sw_flow *flow = NULL;
	struct datapath *dp;
	struct sw_flow_match match;
	struct sw_flow_id ufid;
	u32 ufid_flags = ovs_nla_get_ufid_flags(a[OVS_FLOW_ATTR_UFID_FLAGS]);
	int err;
	bool log = !a[OVS_FLOW_ATTR_PROBE];
	bool ufid_present;

	ufid_present = ovs_nla_get_ufid(&ufid, a[OVS_FLOW_ATTR_UFID], log);
	if (a[OVS_FLOW_ATTR_KEY]) {
		ovs_match_init(&match, &key, true, NULL);
		err = ovs_nla_get_match(net, &match, a[OVS_FLOW_ATTR_KEY],
					NULL, log);
		if (unlikely(err))
			return err;
	}

	ovs_lock();
	dp = get_dp(sock_net(skb->sk), ovs_header->dp_ifindex);
	if (unlikely(!dp)) {
		err = -ENODEV;
		goto unlock;
	}

	if (unlikely(!a[OVS_FLOW_ATTR_KEY] && !ufid_present)) {
		err = ovs_flow_tbl_flush(&dp->table);
		goto unlock;
	}

	if (ufid_present)
		flow = ovs_flow_tbl_lookup_ufid(&dp->table, &ufid);
	else
		flow = ovs_flow_tbl_lookup_exact(&dp->table, &match);
	if (unlikely(!flow)) {
		err = -ENOENT;
		goto unlock;
	}

	ovs_flow_tbl_remove(&dp->table, flow);
	ovs_unlock();

	reply = ovs_flow_cmd_alloc_info((const struct sw_flow_actions __force *) flow->sf_acts,
					&flow->id, info, false, ufid_flags);
	if (likely(reply)) {
		if (!IS_ERR(reply)) {
			rcu_read_lock();	/*To keep RCU checker happy. */
			err = ovs_flow_cmd_fill_info(flow, ovs_header->dp_ifindex,
						     reply, info->snd_portid,
						     info->snd_seq, 0,
						     OVS_FLOW_CMD_DEL,
						     ufid_flags);
			rcu_read_unlock();
			if (WARN_ON_ONCE(err < 0)) {
				kfree_skb(reply);
				goto out_free;
			}

			ovs_notify(&dp_flow_genl_family, reply, info);
		} else {
			netlink_set_err(sock_net(skb->sk)->genl_sock, 0, 0,
					PTR_ERR(reply));
		}
	}

out_free:
	ovs_flow_free(flow, true);
	return 0;
unlock:
	ovs_unlock();
	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int ovs_flow_cmd_dump(struct sk_buff *skb, struct netlink_callback *cb)
{
<<<<<<< HEAD
	struct ovs_header *ovs_header = genlmsg_data(nlmsg_data(cb->nlh));
	struct datapath *dp;
	struct flow_table *table;

	dp = get_dp(ovs_header->dp_ifindex);
	if (!dp)
		return -ENODEV;

	table = genl_dereference(dp->table);

=======
	struct nlattr *a[__OVS_FLOW_ATTR_MAX];
	struct ovs_header *ovs_header = genlmsg_data(nlmsg_data(cb->nlh));
	struct table_instance *ti;
	struct datapath *dp;
	u32 ufid_flags;
	int err;

	err = genlmsg_parse_deprecated(cb->nlh, &dp_flow_genl_family, a,
				       OVS_FLOW_ATTR_MAX, flow_policy, NULL);
	if (err)
		return err;
	ufid_flags = ovs_nla_get_ufid_flags(a[OVS_FLOW_ATTR_UFID_FLAGS]);

	rcu_read_lock();
	dp = get_dp_rcu(sock_net(skb->sk), ovs_header->dp_ifindex);
	if (!dp) {
		rcu_read_unlock();
		return -ENODEV;
	}

	ti = rcu_dereference(dp->table.ti);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	for (;;) {
		struct sw_flow *flow;
		u32 bucket, obj;

		bucket = cb->args[0];
		obj = cb->args[1];
<<<<<<< HEAD
		flow = ovs_flow_tbl_next(table, &bucket, &obj);
		if (!flow)
			break;

		if (ovs_flow_cmd_fill_info(flow, dp, skb,
					   NETLINK_CB(cb->skb).pid,
					   cb->nlh->nlmsg_seq, NLM_F_MULTI,
					   OVS_FLOW_CMD_NEW) < 0)
=======
		flow = ovs_flow_tbl_dump_next(ti, &bucket, &obj);
		if (!flow)
			break;

		if (ovs_flow_cmd_fill_info(flow, ovs_header->dp_ifindex, skb,
					   NETLINK_CB(cb->skb).portid,
					   cb->nlh->nlmsg_seq, NLM_F_MULTI,
					   OVS_FLOW_CMD_GET, ufid_flags) < 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;

		cb->args[0] = bucket;
		cb->args[1] = obj;
	}
<<<<<<< HEAD
	return skb->len;
}

static struct genl_ops dp_flow_genl_ops[] = {
	{ .cmd = OVS_FLOW_CMD_NEW,
	  .flags = GENL_ADMIN_PERM, /* Requires CAP_NET_ADMIN privilege. */
	  .policy = flow_policy,
	  .doit = ovs_flow_cmd_new_or_set
	},
	{ .cmd = OVS_FLOW_CMD_DEL,
	  .flags = GENL_ADMIN_PERM, /* Requires CAP_NET_ADMIN privilege. */
	  .policy = flow_policy,
	  .doit = ovs_flow_cmd_del
	},
	{ .cmd = OVS_FLOW_CMD_GET,
	  .flags = 0,		    /* OK for unprivileged users. */
	  .policy = flow_policy,
=======
	rcu_read_unlock();
	return skb->len;
}

static const struct nla_policy flow_policy[OVS_FLOW_ATTR_MAX + 1] = {
	[OVS_FLOW_ATTR_KEY] = { .type = NLA_NESTED },
	[OVS_FLOW_ATTR_MASK] = { .type = NLA_NESTED },
	[OVS_FLOW_ATTR_ACTIONS] = { .type = NLA_NESTED },
	[OVS_FLOW_ATTR_CLEAR] = { .type = NLA_FLAG },
	[OVS_FLOW_ATTR_PROBE] = { .type = NLA_FLAG },
	[OVS_FLOW_ATTR_UFID] = { .type = NLA_UNSPEC, .len = 1 },
	[OVS_FLOW_ATTR_UFID_FLAGS] = { .type = NLA_U32 },
};

static const struct genl_small_ops dp_flow_genl_ops[] = {
	{ .cmd = OVS_FLOW_CMD_NEW,
	  .validate = GENL_DONT_VALIDATE_STRICT | GENL_DONT_VALIDATE_DUMP,
	  .flags = GENL_UNS_ADMIN_PERM, /* Requires CAP_NET_ADMIN privilege. */
	  .doit = ovs_flow_cmd_new
	},
	{ .cmd = OVS_FLOW_CMD_DEL,
	  .validate = GENL_DONT_VALIDATE_STRICT | GENL_DONT_VALIDATE_DUMP,
	  .flags = GENL_UNS_ADMIN_PERM, /* Requires CAP_NET_ADMIN privilege. */
	  .doit = ovs_flow_cmd_del
	},
	{ .cmd = OVS_FLOW_CMD_GET,
	  .validate = GENL_DONT_VALIDATE_STRICT | GENL_DONT_VALIDATE_DUMP,
	  .flags = 0,		    /* OK for unprivileged users. */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	  .doit = ovs_flow_cmd_get,
	  .dumpit = ovs_flow_cmd_dump
	},
	{ .cmd = OVS_FLOW_CMD_SET,
<<<<<<< HEAD
	  .flags = GENL_ADMIN_PERM, /* Requires CAP_NET_ADMIN privilege. */
	  .policy = flow_policy,
	  .doit = ovs_flow_cmd_new_or_set,
	},
};

static const struct nla_policy datapath_policy[OVS_DP_ATTR_MAX + 1] = {
	[OVS_DP_ATTR_NAME] = { .type = NLA_NUL_STRING, .len = IFNAMSIZ - 1 },
	[OVS_DP_ATTR_UPCALL_PID] = { .type = NLA_U32 },
};

static struct genl_family dp_datapath_genl_family = {
	.id = GENL_ID_GENERATE,
	.hdrsize = sizeof(struct ovs_header),
	.name = OVS_DATAPATH_FAMILY,
	.version = OVS_DATAPATH_VERSION,
	.maxattr = OVS_DP_ATTR_MAX
};

static struct genl_multicast_group ovs_dp_datapath_multicast_group = {
	.name = OVS_DATAPATH_MCGROUP
};

static int ovs_dp_cmd_fill_info(struct datapath *dp, struct sk_buff *skb,
				u32 pid, u32 seq, u32 flags, u8 cmd)
{
	struct ovs_header *ovs_header;
	struct ovs_dp_stats dp_stats;
	int err;

	ovs_header = genlmsg_put(skb, pid, seq, &dp_datapath_genl_family,
				   flags, cmd);
=======
	  .validate = GENL_DONT_VALIDATE_STRICT | GENL_DONT_VALIDATE_DUMP,
	  .flags = GENL_UNS_ADMIN_PERM, /* Requires CAP_NET_ADMIN privilege. */
	  .doit = ovs_flow_cmd_set,
	},
};

static struct genl_family dp_flow_genl_family __ro_after_init = {
	.hdrsize = sizeof(struct ovs_header),
	.name = OVS_FLOW_FAMILY,
	.version = OVS_FLOW_VERSION,
	.maxattr = OVS_FLOW_ATTR_MAX,
	.policy = flow_policy,
	.netnsok = true,
	.parallel_ops = true,
	.small_ops = dp_flow_genl_ops,
	.n_small_ops = ARRAY_SIZE(dp_flow_genl_ops),
	.resv_start_op = OVS_FLOW_CMD_SET + 1,
	.mcgrps = &ovs_dp_flow_multicast_group,
	.n_mcgrps = 1,
	.module = THIS_MODULE,
};

static size_t ovs_dp_cmd_msg_size(void)
{
	size_t msgsize = NLMSG_ALIGN(sizeof(struct ovs_header));

	msgsize += nla_total_size(IFNAMSIZ);
	msgsize += nla_total_size_64bit(sizeof(struct ovs_dp_stats));
	msgsize += nla_total_size_64bit(sizeof(struct ovs_dp_megaflow_stats));
	msgsize += nla_total_size(sizeof(u32)); /* OVS_DP_ATTR_USER_FEATURES */
	msgsize += nla_total_size(sizeof(u32)); /* OVS_DP_ATTR_MASKS_CACHE_SIZE */
	msgsize += nla_total_size(sizeof(u32) * nr_cpu_ids); /* OVS_DP_ATTR_PER_CPU_PIDS */

	return msgsize;
}

/* Called with ovs_mutex. */
static int ovs_dp_cmd_fill_info(struct datapath *dp, struct sk_buff *skb,
				u32 portid, u32 seq, u32 flags, u8 cmd)
{
	struct ovs_header *ovs_header;
	struct ovs_dp_stats dp_stats;
	struct ovs_dp_megaflow_stats dp_megaflow_stats;
	struct dp_nlsk_pids *pids = ovsl_dereference(dp->upcall_portids);
	int err, pids_len;

	ovs_header = genlmsg_put(skb, portid, seq, &dp_datapath_genl_family,
				 flags, cmd);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!ovs_header)
		goto error;

	ovs_header->dp_ifindex = get_dpifindex(dp);

<<<<<<< HEAD
	rcu_read_lock();
	err = nla_put_string(skb, OVS_DP_ATTR_NAME, ovs_dp_name(dp));
	rcu_read_unlock();
	if (err)
		goto nla_put_failure;

	get_dp_stats(dp, &dp_stats);
	NLA_PUT(skb, OVS_DP_ATTR_STATS, sizeof(struct ovs_dp_stats), &dp_stats);

	return genlmsg_end(skb, ovs_header);
=======
	err = nla_put_string(skb, OVS_DP_ATTR_NAME, ovs_dp_name(dp));
	if (err)
		goto nla_put_failure;

	get_dp_stats(dp, &dp_stats, &dp_megaflow_stats);
	if (nla_put_64bit(skb, OVS_DP_ATTR_STATS, sizeof(struct ovs_dp_stats),
			  &dp_stats, OVS_DP_ATTR_PAD))
		goto nla_put_failure;

	if (nla_put_64bit(skb, OVS_DP_ATTR_MEGAFLOW_STATS,
			  sizeof(struct ovs_dp_megaflow_stats),
			  &dp_megaflow_stats, OVS_DP_ATTR_PAD))
		goto nla_put_failure;

	if (nla_put_u32(skb, OVS_DP_ATTR_USER_FEATURES, dp->user_features))
		goto nla_put_failure;

	if (nla_put_u32(skb, OVS_DP_ATTR_MASKS_CACHE_SIZE,
			ovs_flow_tbl_masks_cache_size(&dp->table)))
		goto nla_put_failure;

	if (dp->user_features & OVS_DP_F_DISPATCH_UPCALL_PER_CPU && pids) {
		pids_len = min(pids->n_pids, nr_cpu_ids) * sizeof(u32);
		if (nla_put(skb, OVS_DP_ATTR_PER_CPU_PIDS, pids_len, &pids->pids))
			goto nla_put_failure;
	}

	genlmsg_end(skb, ovs_header);
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

nla_put_failure:
	genlmsg_cancel(skb, ovs_header);
error:
	return -EMSGSIZE;
}

<<<<<<< HEAD
static struct sk_buff *ovs_dp_cmd_build_info(struct datapath *dp, u32 pid,
					     u32 seq, u8 cmd)
{
	struct sk_buff *skb;
	int retval;

	skb = nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_KERNEL);
	if (!skb)
		return ERR_PTR(-ENOMEM);

	retval = ovs_dp_cmd_fill_info(dp, skb, pid, seq, 0, cmd);
	if (retval < 0) {
		kfree_skb(skb);
		return ERR_PTR(retval);
	}
	return skb;
}

/* Called with genl_mutex and optionally with RTNL lock also. */
static struct datapath *lookup_datapath(struct ovs_header *ovs_header,
=======
static struct sk_buff *ovs_dp_cmd_alloc_info(void)
{
	return genlmsg_new(ovs_dp_cmd_msg_size(), GFP_KERNEL);
}

/* Called with rcu_read_lock or ovs_mutex. */
static struct datapath *lookup_datapath(struct net *net,
					const struct ovs_header *ovs_header,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					struct nlattr *a[OVS_DP_ATTR_MAX + 1])
{
	struct datapath *dp;

	if (!a[OVS_DP_ATTR_NAME])
<<<<<<< HEAD
		dp = get_dp(ovs_header->dp_ifindex);
	else {
		struct vport *vport;

		rcu_read_lock();
		vport = ovs_vport_locate(nla_data(a[OVS_DP_ATTR_NAME]));
		dp = vport && vport->port_no == OVSP_LOCAL ? vport->dp : NULL;
		rcu_read_unlock();
=======
		dp = get_dp(net, ovs_header->dp_ifindex);
	else {
		struct vport *vport;

		vport = ovs_vport_locate(net, nla_data(a[OVS_DP_ATTR_NAME]));
		dp = vport && vport->port_no == OVSP_LOCAL ? vport->dp : NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return dp ? dp : ERR_PTR(-ENODEV);
}

<<<<<<< HEAD
=======
static void ovs_dp_reset_user_features(struct sk_buff *skb,
				       struct genl_info *info)
{
	struct datapath *dp;

	dp = lookup_datapath(sock_net(skb->sk), genl_info_userhdr(info),
			     info->attrs);
	if (IS_ERR(dp))
		return;

	pr_warn("%s: Dropping previously announced user features\n",
		ovs_dp_name(dp));
	dp->user_features = 0;
}

static int ovs_dp_set_upcall_portids(struct datapath *dp,
			      const struct nlattr *ids)
{
	struct dp_nlsk_pids *old, *dp_nlsk_pids;

	if (!nla_len(ids) || nla_len(ids) % sizeof(u32))
		return -EINVAL;

	old = ovsl_dereference(dp->upcall_portids);

	dp_nlsk_pids = kmalloc(sizeof(*dp_nlsk_pids) + nla_len(ids),
			       GFP_KERNEL);
	if (!dp_nlsk_pids)
		return -ENOMEM;

	dp_nlsk_pids->n_pids = nla_len(ids) / sizeof(u32);
	nla_memcpy(dp_nlsk_pids->pids, ids, nla_len(ids));

	rcu_assign_pointer(dp->upcall_portids, dp_nlsk_pids);

	kfree_rcu(old, rcu);

	return 0;
}

u32 ovs_dp_get_upcall_portid(const struct datapath *dp, uint32_t cpu_id)
{
	struct dp_nlsk_pids *dp_nlsk_pids;

	dp_nlsk_pids = rcu_dereference(dp->upcall_portids);

	if (dp_nlsk_pids) {
		if (cpu_id < dp_nlsk_pids->n_pids) {
			return dp_nlsk_pids->pids[cpu_id];
		} else if (dp_nlsk_pids->n_pids > 0 &&
			   cpu_id >= dp_nlsk_pids->n_pids) {
			/* If the number of netlink PIDs is mismatched with
			 * the number of CPUs as seen by the kernel, log this
			 * and send the upcall to an arbitrary socket (0) in
			 * order to not drop packets
			 */
			pr_info_ratelimited("cpu_id mismatch with handler threads");
			return dp_nlsk_pids->pids[cpu_id %
						  dp_nlsk_pids->n_pids];
		} else {
			return 0;
		}
	} else {
		return 0;
	}
}

static int ovs_dp_change(struct datapath *dp, struct nlattr *a[])
{
	u32 user_features = 0, old_features = dp->user_features;
	int err;

	if (a[OVS_DP_ATTR_USER_FEATURES]) {
		user_features = nla_get_u32(a[OVS_DP_ATTR_USER_FEATURES]);

		if (user_features & ~(OVS_DP_F_VPORT_PIDS |
				      OVS_DP_F_UNALIGNED |
				      OVS_DP_F_TC_RECIRC_SHARING |
				      OVS_DP_F_DISPATCH_UPCALL_PER_CPU))
			return -EOPNOTSUPP;

#if !IS_ENABLED(CONFIG_NET_TC_SKB_EXT)
		if (user_features & OVS_DP_F_TC_RECIRC_SHARING)
			return -EOPNOTSUPP;
#endif
	}

	if (a[OVS_DP_ATTR_MASKS_CACHE_SIZE]) {
		int err;
		u32 cache_size;

		cache_size = nla_get_u32(a[OVS_DP_ATTR_MASKS_CACHE_SIZE]);
		err = ovs_flow_tbl_masks_cache_resize(&dp->table, cache_size);
		if (err)
			return err;
	}

	dp->user_features = user_features;

	if (dp->user_features & OVS_DP_F_DISPATCH_UPCALL_PER_CPU &&
	    a[OVS_DP_ATTR_PER_CPU_PIDS]) {
		/* Upcall Netlink Port IDs have been updated */
		err = ovs_dp_set_upcall_portids(dp,
						a[OVS_DP_ATTR_PER_CPU_PIDS]);
		if (err)
			return err;
	}

	if ((dp->user_features & OVS_DP_F_TC_RECIRC_SHARING) &&
	    !(old_features & OVS_DP_F_TC_RECIRC_SHARING))
		tc_skb_ext_tc_enable();
	else if (!(dp->user_features & OVS_DP_F_TC_RECIRC_SHARING) &&
		 (old_features & OVS_DP_F_TC_RECIRC_SHARING))
		tc_skb_ext_tc_disable();

	return 0;
}

static int ovs_dp_stats_init(struct datapath *dp)
{
	dp->stats_percpu = netdev_alloc_pcpu_stats(struct dp_stats_percpu);
	if (!dp->stats_percpu)
		return -ENOMEM;

	return 0;
}

static int ovs_dp_vport_init(struct datapath *dp)
{
	int i;

	dp->ports = kmalloc_array(DP_VPORT_HASH_BUCKETS,
				  sizeof(struct hlist_head),
				  GFP_KERNEL);
	if (!dp->ports)
		return -ENOMEM;

	for (i = 0; i < DP_VPORT_HASH_BUCKETS; i++)
		INIT_HLIST_HEAD(&dp->ports[i]);

	return 0;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int ovs_dp_cmd_new(struct sk_buff *skb, struct genl_info *info)
{
	struct nlattr **a = info->attrs;
	struct vport_parms parms;
	struct sk_buff *reply;
	struct datapath *dp;
	struct vport *vport;
<<<<<<< HEAD
=======
	struct ovs_net *ovs_net;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int err;

	err = -EINVAL;
	if (!a[OVS_DP_ATTR_NAME] || !a[OVS_DP_ATTR_UPCALL_PID])
		goto err;

<<<<<<< HEAD
	rtnl_lock();
	err = -ENODEV;
	if (!try_module_get(THIS_MODULE))
		goto err_unlock_rtnl;
=======
	reply = ovs_dp_cmd_alloc_info();
	if (!reply)
		return -ENOMEM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	err = -ENOMEM;
	dp = kzalloc(sizeof(*dp), GFP_KERNEL);
	if (dp == NULL)
<<<<<<< HEAD
		goto err_put_module;
	INIT_LIST_HEAD(&dp->port_list);

	/* Allocate table. */
	err = -ENOMEM;
	rcu_assign_pointer(dp->table, ovs_flow_tbl_alloc(TBL_MIN_BUCKETS));
	if (!dp->table)
		goto err_free_dp;

	dp->stats_percpu = alloc_percpu(struct dp_stats_percpu);
	if (!dp->stats_percpu) {
		err = -ENOMEM;
		goto err_destroy_table;
	}

	for_each_possible_cpu(i) {
		struct dp_stats_percpu *dpath_stats;
		dpath_stats = per_cpu_ptr(dp->stats_percpu, i);
		u64_stats_init(&dpath_stats->sync);
	}
=======
		goto err_destroy_reply;

	ovs_dp_set_net(dp, sock_net(skb->sk));

	/* Allocate table. */
	err = ovs_flow_tbl_init(&dp->table);
	if (err)
		goto err_destroy_dp;

	err = ovs_dp_stats_init(dp);
	if (err)
		goto err_destroy_table;

	err = ovs_dp_vport_init(dp);
	if (err)
		goto err_destroy_stats;

	err = ovs_meters_init(dp);
	if (err)
		goto err_destroy_ports;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Set up our datapath device. */
	parms.name = nla_data(a[OVS_DP_ATTR_NAME]);
	parms.type = OVS_VPORT_TYPE_INTERNAL;
	parms.options = NULL;
	parms.dp = dp;
	parms.port_no = OVSP_LOCAL;
<<<<<<< HEAD
	parms.upcall_pid = nla_get_u32(a[OVS_DP_ATTR_UPCALL_PID]);
=======
	parms.upcall_portids = a[OVS_DP_ATTR_UPCALL_PID];
	parms.desired_ifindex = a[OVS_DP_ATTR_IFINDEX]
		? nla_get_s32(a[OVS_DP_ATTR_IFINDEX]) : 0;

	/* So far only local changes have been made, now need the lock. */
	ovs_lock();

	err = ovs_dp_change(dp, a);
	if (err)
		goto err_unlock_and_destroy_meters;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	vport = new_vport(&parms);
	if (IS_ERR(vport)) {
		err = PTR_ERR(vport);
		if (err == -EBUSY)
			err = -EEXIST;

<<<<<<< HEAD
		goto err_destroy_percpu;
	}

	reply = ovs_dp_cmd_build_info(dp, info->snd_pid,
				      info->snd_seq, OVS_DP_CMD_NEW);
	err = PTR_ERR(reply);
	if (IS_ERR(reply))
		goto err_destroy_local_port;

	list_add_tail(&dp->list_node, &dps);
	rtnl_unlock();

	genl_notify(reply, genl_info_net(info), info->snd_pid,
		    ovs_dp_datapath_multicast_group.id, info->nlhdr,
		    GFP_KERNEL);
	return 0;

err_destroy_local_port:
	ovs_dp_detach_port(rtnl_dereference(dp->ports[OVSP_LOCAL]));
err_destroy_percpu:
	free_percpu(dp->stats_percpu);
err_destroy_table:
	ovs_flow_tbl_destroy(genl_dereference(dp->table));
err_free_dp:
	kfree(dp);
err_put_module:
	module_put(THIS_MODULE);
err_unlock_rtnl:
	rtnl_unlock();
=======
		if (err == -EEXIST) {
			/* An outdated user space instance that does not understand
			 * the concept of user_features has attempted to create a new
			 * datapath and is likely to reuse it. Drop all user features.
			 */
			if (info->genlhdr->version < OVS_DP_VER_FEATURES)
				ovs_dp_reset_user_features(skb, info);
		}

		goto err_destroy_portids;
	}

	err = ovs_dp_cmd_fill_info(dp, reply, info->snd_portid,
				   info->snd_seq, 0, OVS_DP_CMD_NEW);
	BUG_ON(err < 0);

	ovs_net = net_generic(ovs_dp_get_net(dp), ovs_net_id);
	list_add_tail_rcu(&dp->list_node, &ovs_net->dps);

	ovs_unlock();

	ovs_notify(&dp_datapath_genl_family, reply, info);
	return 0;

err_destroy_portids:
	kfree(rcu_dereference_raw(dp->upcall_portids));
err_unlock_and_destroy_meters:
	ovs_unlock();
	ovs_meters_exit(dp);
err_destroy_ports:
	kfree(dp->ports);
err_destroy_stats:
	free_percpu(dp->stats_percpu);
err_destroy_table:
	ovs_flow_tbl_destroy(&dp->table);
err_destroy_dp:
	kfree(dp);
err_destroy_reply:
	kfree_skb(reply);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
err:
	return err;
}

<<<<<<< HEAD
static int ovs_dp_cmd_del(struct sk_buff *skb, struct genl_info *info)
{
	struct vport *vport, *next_vport;
=======
/* Called with ovs_mutex. */
static void __dp_destroy(struct datapath *dp)
{
	struct flow_table *table = &dp->table;
	int i;

	if (dp->user_features & OVS_DP_F_TC_RECIRC_SHARING)
		tc_skb_ext_tc_disable();

	for (i = 0; i < DP_VPORT_HASH_BUCKETS; i++) {
		struct vport *vport;
		struct hlist_node *n;

		hlist_for_each_entry_safe(vport, n, &dp->ports[i], dp_hash_node)
			if (vport->port_no != OVSP_LOCAL)
				ovs_dp_detach_port(vport);
	}

	list_del_rcu(&dp->list_node);

	/* OVSP_LOCAL is datapath internal port. We need to make sure that
	 * all ports in datapath are destroyed first before freeing datapath.
	 */
	ovs_dp_detach_port(ovs_vport_ovsl(dp, OVSP_LOCAL));

	/* Flush sw_flow in the tables. RCU cb only releases resource
	 * such as dp, ports and tables. That may avoid some issues
	 * such as RCU usage warning.
	 */
	table_instance_flow_flush(table, ovsl_dereference(table->ti),
				  ovsl_dereference(table->ufid_ti));

	/* RCU destroy the ports, meters and flow tables. */
	call_rcu(&dp->rcu, destroy_dp_rcu);
}

static int ovs_dp_cmd_del(struct sk_buff *skb, struct genl_info *info)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct sk_buff *reply;
	struct datapath *dp;
	int err;

<<<<<<< HEAD
	rtnl_lock();
	dp = lookup_datapath(info->userhdr, info->attrs);
	err = PTR_ERR(dp);
	if (IS_ERR(dp))
		goto exit_unlock;

	reply = ovs_dp_cmd_build_info(dp, info->snd_pid,
				      info->snd_seq, OVS_DP_CMD_DEL);
	err = PTR_ERR(reply);
	if (IS_ERR(reply))
		goto exit_unlock;

	list_for_each_entry_safe(vport, next_vport, &dp->port_list, node)
		if (vport->port_no != OVSP_LOCAL)
			ovs_dp_detach_port(vport);

	list_del(&dp->list_node);
	ovs_dp_detach_port(rtnl_dereference(dp->ports[OVSP_LOCAL]));

	/* rtnl_unlock() will wait until all the references to devices that
	 * are pending unregistration have been dropped.  We do it here to
	 * ensure that any internal devices (which contain DP pointers) are
	 * fully destroyed before freeing the datapath.
	 */
	rtnl_unlock();

	call_rcu(&dp->rcu, destroy_dp_rcu);
	module_put(THIS_MODULE);

	genl_notify(reply, genl_info_net(info), info->snd_pid,
		    ovs_dp_datapath_multicast_group.id, info->nlhdr,
		    GFP_KERNEL);

	return 0;

exit_unlock:
	rtnl_unlock();
=======
	reply = ovs_dp_cmd_alloc_info();
	if (!reply)
		return -ENOMEM;

	ovs_lock();
	dp = lookup_datapath(sock_net(skb->sk), genl_info_userhdr(info),
			     info->attrs);
	err = PTR_ERR(dp);
	if (IS_ERR(dp))
		goto err_unlock_free;

	err = ovs_dp_cmd_fill_info(dp, reply, info->snd_portid,
				   info->snd_seq, 0, OVS_DP_CMD_DEL);
	BUG_ON(err < 0);

	__dp_destroy(dp);
	ovs_unlock();

	ovs_notify(&dp_datapath_genl_family, reply, info);

	return 0;

err_unlock_free:
	ovs_unlock();
	kfree_skb(reply);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

static int ovs_dp_cmd_set(struct sk_buff *skb, struct genl_info *info)
{
	struct sk_buff *reply;
	struct datapath *dp;
	int err;

<<<<<<< HEAD
	dp = lookup_datapath(info->userhdr, info->attrs);
	if (IS_ERR(dp))
		return PTR_ERR(dp);

	reply = ovs_dp_cmd_build_info(dp, info->snd_pid,
				      info->snd_seq, OVS_DP_CMD_NEW);
	if (IS_ERR(reply)) {
		err = PTR_ERR(reply);
		netlink_set_err(init_net.genl_sock, 0,
				ovs_dp_datapath_multicast_group.id, err);
		return 0;
	}

	genl_notify(reply, genl_info_net(info), info->snd_pid,
		    ovs_dp_datapath_multicast_group.id, info->nlhdr,
		    GFP_KERNEL);

	return 0;
=======
	reply = ovs_dp_cmd_alloc_info();
	if (!reply)
		return -ENOMEM;

	ovs_lock();
	dp = lookup_datapath(sock_net(skb->sk), genl_info_userhdr(info),
			     info->attrs);
	err = PTR_ERR(dp);
	if (IS_ERR(dp))
		goto err_unlock_free;

	err = ovs_dp_change(dp, info->attrs);
	if (err)
		goto err_unlock_free;

	err = ovs_dp_cmd_fill_info(dp, reply, info->snd_portid,
				   info->snd_seq, 0, OVS_DP_CMD_SET);
	BUG_ON(err < 0);

	ovs_unlock();
	ovs_notify(&dp_datapath_genl_family, reply, info);

	return 0;

err_unlock_free:
	ovs_unlock();
	kfree_skb(reply);
	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int ovs_dp_cmd_get(struct sk_buff *skb, struct genl_info *info)
{
	struct sk_buff *reply;
	struct datapath *dp;
<<<<<<< HEAD

	dp = lookup_datapath(info->userhdr, info->attrs);
	if (IS_ERR(dp))
		return PTR_ERR(dp);

	reply = ovs_dp_cmd_build_info(dp, info->snd_pid,
				      info->snd_seq, OVS_DP_CMD_NEW);
	if (IS_ERR(reply))
		return PTR_ERR(reply);

	return genlmsg_reply(reply, info);
=======
	int err;

	reply = ovs_dp_cmd_alloc_info();
	if (!reply)
		return -ENOMEM;

	ovs_lock();
	dp = lookup_datapath(sock_net(skb->sk), genl_info_userhdr(info),
			     info->attrs);
	if (IS_ERR(dp)) {
		err = PTR_ERR(dp);
		goto err_unlock_free;
	}
	err = ovs_dp_cmd_fill_info(dp, reply, info->snd_portid,
				   info->snd_seq, 0, OVS_DP_CMD_GET);
	BUG_ON(err < 0);
	ovs_unlock();

	return genlmsg_reply(reply, info);

err_unlock_free:
	ovs_unlock();
	kfree_skb(reply);
	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int ovs_dp_cmd_dump(struct sk_buff *skb, struct netlink_callback *cb)
{
<<<<<<< HEAD
=======
	struct ovs_net *ovs_net = net_generic(sock_net(skb->sk), ovs_net_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct datapath *dp;
	int skip = cb->args[0];
	int i = 0;

<<<<<<< HEAD
	list_for_each_entry(dp, &dps, list_node) {
		if (i >= skip &&
		    ovs_dp_cmd_fill_info(dp, skb, NETLINK_CB(cb->skb).pid,
					 cb->nlh->nlmsg_seq, NLM_F_MULTI,
					 OVS_DP_CMD_NEW) < 0)
			break;
		i++;
	}
=======
	ovs_lock();
	list_for_each_entry(dp, &ovs_net->dps, list_node) {
		if (i >= skip &&
		    ovs_dp_cmd_fill_info(dp, skb, NETLINK_CB(cb->skb).portid,
					 cb->nlh->nlmsg_seq, NLM_F_MULTI,
					 OVS_DP_CMD_GET) < 0)
			break;
		i++;
	}
	ovs_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	cb->args[0] = i;

	return skb->len;
}

<<<<<<< HEAD
static struct genl_ops dp_datapath_genl_ops[] = {
	{ .cmd = OVS_DP_CMD_NEW,
	  .flags = GENL_ADMIN_PERM, /* Requires CAP_NET_ADMIN privilege. */
	  .policy = datapath_policy,
	  .doit = ovs_dp_cmd_new
	},
	{ .cmd = OVS_DP_CMD_DEL,
	  .flags = GENL_ADMIN_PERM, /* Requires CAP_NET_ADMIN privilege. */
	  .policy = datapath_policy,
	  .doit = ovs_dp_cmd_del
	},
	{ .cmd = OVS_DP_CMD_GET,
	  .flags = 0,		    /* OK for unprivileged users. */
	  .policy = datapath_policy,
=======
static const struct nla_policy datapath_policy[OVS_DP_ATTR_MAX + 1] = {
	[OVS_DP_ATTR_NAME] = { .type = NLA_NUL_STRING, .len = IFNAMSIZ - 1 },
	[OVS_DP_ATTR_UPCALL_PID] = { .type = NLA_U32 },
	[OVS_DP_ATTR_USER_FEATURES] = { .type = NLA_U32 },
	[OVS_DP_ATTR_MASKS_CACHE_SIZE] =  NLA_POLICY_RANGE(NLA_U32, 0,
		PCPU_MIN_UNIT_SIZE / sizeof(struct mask_cache_entry)),
	[OVS_DP_ATTR_IFINDEX] = NLA_POLICY_MIN(NLA_S32, 0),
};

static const struct genl_small_ops dp_datapath_genl_ops[] = {
	{ .cmd = OVS_DP_CMD_NEW,
	  .validate = GENL_DONT_VALIDATE_STRICT | GENL_DONT_VALIDATE_DUMP,
	  .flags = GENL_UNS_ADMIN_PERM, /* Requires CAP_NET_ADMIN privilege. */
	  .doit = ovs_dp_cmd_new
	},
	{ .cmd = OVS_DP_CMD_DEL,
	  .validate = GENL_DONT_VALIDATE_STRICT | GENL_DONT_VALIDATE_DUMP,
	  .flags = GENL_UNS_ADMIN_PERM, /* Requires CAP_NET_ADMIN privilege. */
	  .doit = ovs_dp_cmd_del
	},
	{ .cmd = OVS_DP_CMD_GET,
	  .validate = GENL_DONT_VALIDATE_STRICT | GENL_DONT_VALIDATE_DUMP,
	  .flags = 0,		    /* OK for unprivileged users. */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	  .doit = ovs_dp_cmd_get,
	  .dumpit = ovs_dp_cmd_dump
	},
	{ .cmd = OVS_DP_CMD_SET,
<<<<<<< HEAD
	  .flags = GENL_ADMIN_PERM, /* Requires CAP_NET_ADMIN privilege. */
	  .policy = datapath_policy,
=======
	  .validate = GENL_DONT_VALIDATE_STRICT | GENL_DONT_VALIDATE_DUMP,
	  .flags = GENL_UNS_ADMIN_PERM, /* Requires CAP_NET_ADMIN privilege. */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	  .doit = ovs_dp_cmd_set,
	},
};

<<<<<<< HEAD
static const struct nla_policy vport_policy[OVS_VPORT_ATTR_MAX + 1] = {
	[OVS_VPORT_ATTR_NAME] = { .type = NLA_NUL_STRING, .len = IFNAMSIZ - 1 },
	[OVS_VPORT_ATTR_STATS] = { .len = sizeof(struct ovs_vport_stats) },
	[OVS_VPORT_ATTR_PORT_NO] = { .type = NLA_U32 },
	[OVS_VPORT_ATTR_TYPE] = { .type = NLA_U32 },
	[OVS_VPORT_ATTR_UPCALL_PID] = { .type = NLA_U32 },
	[OVS_VPORT_ATTR_OPTIONS] = { .type = NLA_NESTED },
};

static struct genl_family dp_vport_genl_family = {
	.id = GENL_ID_GENERATE,
	.hdrsize = sizeof(struct ovs_header),
	.name = OVS_VPORT_FAMILY,
	.version = OVS_VPORT_VERSION,
	.maxattr = OVS_VPORT_ATTR_MAX
};

struct genl_multicast_group ovs_dp_vport_multicast_group = {
	.name = OVS_VPORT_MCGROUP
};

/* Called with RTNL lock or RCU read lock. */
static int ovs_vport_cmd_fill_info(struct vport *vport, struct sk_buff *skb,
				   u32 pid, u32 seq, u32 flags, u8 cmd)
=======
static struct genl_family dp_datapath_genl_family __ro_after_init = {
	.hdrsize = sizeof(struct ovs_header),
	.name = OVS_DATAPATH_FAMILY,
	.version = OVS_DATAPATH_VERSION,
	.maxattr = OVS_DP_ATTR_MAX,
	.policy = datapath_policy,
	.netnsok = true,
	.parallel_ops = true,
	.small_ops = dp_datapath_genl_ops,
	.n_small_ops = ARRAY_SIZE(dp_datapath_genl_ops),
	.resv_start_op = OVS_DP_CMD_SET + 1,
	.mcgrps = &ovs_dp_datapath_multicast_group,
	.n_mcgrps = 1,
	.module = THIS_MODULE,
};

/* Called with ovs_mutex or RCU read lock. */
static int ovs_vport_cmd_fill_info(struct vport *vport, struct sk_buff *skb,
				   struct net *net, u32 portid, u32 seq,
				   u32 flags, u8 cmd, gfp_t gfp)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ovs_header *ovs_header;
	struct ovs_vport_stats vport_stats;
	int err;

<<<<<<< HEAD
	ovs_header = genlmsg_put(skb, pid, seq, &dp_vport_genl_family,
=======
	ovs_header = genlmsg_put(skb, portid, seq, &dp_vport_genl_family,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				 flags, cmd);
	if (!ovs_header)
		return -EMSGSIZE;

	ovs_header->dp_ifindex = get_dpifindex(vport->dp);

<<<<<<< HEAD
	NLA_PUT_U32(skb, OVS_VPORT_ATTR_PORT_NO, vport->port_no);
	NLA_PUT_U32(skb, OVS_VPORT_ATTR_TYPE, vport->ops->type);
	NLA_PUT_STRING(skb, OVS_VPORT_ATTR_NAME, vport->ops->get_name(vport));
	NLA_PUT_U32(skb, OVS_VPORT_ATTR_UPCALL_PID, vport->upcall_pid);

	ovs_vport_get_stats(vport, &vport_stats);
	NLA_PUT(skb, OVS_VPORT_ATTR_STATS, sizeof(struct ovs_vport_stats),
		&vport_stats);
=======
	if (nla_put_u32(skb, OVS_VPORT_ATTR_PORT_NO, vport->port_no) ||
	    nla_put_u32(skb, OVS_VPORT_ATTR_TYPE, vport->ops->type) ||
	    nla_put_string(skb, OVS_VPORT_ATTR_NAME,
			   ovs_vport_name(vport)) ||
	    nla_put_u32(skb, OVS_VPORT_ATTR_IFINDEX, vport->dev->ifindex))
		goto nla_put_failure;

	if (!net_eq(net, dev_net(vport->dev))) {
		int id = peernet2id_alloc(net, dev_net(vport->dev), gfp);

		if (nla_put_s32(skb, OVS_VPORT_ATTR_NETNSID, id))
			goto nla_put_failure;
	}

	ovs_vport_get_stats(vport, &vport_stats);
	if (nla_put_64bit(skb, OVS_VPORT_ATTR_STATS,
			  sizeof(struct ovs_vport_stats), &vport_stats,
			  OVS_VPORT_ATTR_PAD))
		goto nla_put_failure;

	if (ovs_vport_get_upcall_stats(vport, skb))
		goto nla_put_failure;

	if (ovs_vport_get_upcall_portids(vport, skb))
		goto nla_put_failure;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	err = ovs_vport_get_options(vport, skb);
	if (err == -EMSGSIZE)
		goto error;

<<<<<<< HEAD
	return genlmsg_end(skb, ovs_header);
=======
	genlmsg_end(skb, ovs_header);
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

nla_put_failure:
	err = -EMSGSIZE;
error:
	genlmsg_cancel(skb, ovs_header);
	return err;
}

<<<<<<< HEAD
/* Called with RTNL lock or RCU read lock. */
struct sk_buff *ovs_vport_cmd_build_info(struct vport *vport, u32 pid,
					 u32 seq, u8 cmd)
=======
static struct sk_buff *ovs_vport_cmd_alloc_info(void)
{
	return nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_KERNEL);
}

/* Called with ovs_mutex, only via ovs_dp_notify_wq(). */
struct sk_buff *ovs_vport_cmd_build_info(struct vport *vport, struct net *net,
					 u32 portid, u32 seq, u8 cmd)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct sk_buff *skb;
	int retval;

<<<<<<< HEAD
	skb = nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_ATOMIC);
	if (!skb)
		return ERR_PTR(-ENOMEM);

	retval = ovs_vport_cmd_fill_info(vport, skb, pid, seq, 0, cmd);
	if (retval < 0) {
		kfree_skb(skb);
		return ERR_PTR(retval);
	}
	return skb;
}

/* Called with RTNL lock or RCU read lock. */
static struct vport *lookup_vport(struct ovs_header *ovs_header,
=======
	skb = nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_KERNEL);
	if (!skb)
		return ERR_PTR(-ENOMEM);

	retval = ovs_vport_cmd_fill_info(vport, skb, net, portid, seq, 0, cmd,
					 GFP_KERNEL);
	BUG_ON(retval < 0);

	return skb;
}

/* Called with ovs_mutex or RCU read lock. */
static struct vport *lookup_vport(struct net *net,
				  const struct ovs_header *ovs_header,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				  struct nlattr *a[OVS_VPORT_ATTR_MAX + 1])
{
	struct datapath *dp;
	struct vport *vport;

<<<<<<< HEAD
	if (a[OVS_VPORT_ATTR_NAME]) {
		vport = ovs_vport_locate(nla_data(a[OVS_VPORT_ATTR_NAME]));
=======
	if (a[OVS_VPORT_ATTR_IFINDEX])
		return ERR_PTR(-EOPNOTSUPP);
	if (a[OVS_VPORT_ATTR_NAME]) {
		vport = ovs_vport_locate(net, nla_data(a[OVS_VPORT_ATTR_NAME]));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!vport)
			return ERR_PTR(-ENODEV);
		if (ovs_header->dp_ifindex &&
		    ovs_header->dp_ifindex != get_dpifindex(vport->dp))
			return ERR_PTR(-ENODEV);
		return vport;
	} else if (a[OVS_VPORT_ATTR_PORT_NO]) {
		u32 port_no = nla_get_u32(a[OVS_VPORT_ATTR_PORT_NO]);

		if (port_no >= DP_MAX_PORTS)
			return ERR_PTR(-EFBIG);

<<<<<<< HEAD
		dp = get_dp(ovs_header->dp_ifindex);
		if (!dp)
			return ERR_PTR(-ENODEV);

		vport = rcu_dereference_rtnl(dp->ports[port_no]);
		if (!vport)
			return ERR_PTR(-ENOENT);
		return vport;
	} else
		return ERR_PTR(-EINVAL);
=======
		dp = get_dp(net, ovs_header->dp_ifindex);
		if (!dp)
			return ERR_PTR(-ENODEV);

		vport = ovs_vport_ovsl_rcu(dp, port_no);
		if (!vport)
			return ERR_PTR(-ENODEV);
		return vport;
	} else
		return ERR_PTR(-EINVAL);

}

static unsigned int ovs_get_max_headroom(struct datapath *dp)
{
	unsigned int dev_headroom, max_headroom = 0;
	struct net_device *dev;
	struct vport *vport;
	int i;

	for (i = 0; i < DP_VPORT_HASH_BUCKETS; i++) {
		hlist_for_each_entry_rcu(vport, &dp->ports[i], dp_hash_node,
					 lockdep_ovsl_is_held()) {
			dev = vport->dev;
			dev_headroom = netdev_get_fwd_headroom(dev);
			if (dev_headroom > max_headroom)
				max_headroom = dev_headroom;
		}
	}

	return max_headroom;
}

/* Called with ovs_mutex */
static void ovs_update_headroom(struct datapath *dp, unsigned int new_headroom)
{
	struct vport *vport;
	int i;

	dp->max_headroom = new_headroom;
	for (i = 0; i < DP_VPORT_HASH_BUCKETS; i++) {
		hlist_for_each_entry_rcu(vport, &dp->ports[i], dp_hash_node,
					 lockdep_ovsl_is_held())
			netdev_set_rx_headroom(vport->dev, new_headroom);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int ovs_vport_cmd_new(struct sk_buff *skb, struct genl_info *info)
{
	struct nlattr **a = info->attrs;
<<<<<<< HEAD
	struct ovs_header *ovs_header = info->userhdr;
=======
	struct ovs_header *ovs_header = genl_info_userhdr(info);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct vport_parms parms;
	struct sk_buff *reply;
	struct vport *vport;
	struct datapath *dp;
<<<<<<< HEAD
	u32 port_no;
	int err;

	err = -EINVAL;
	if (!a[OVS_VPORT_ATTR_NAME] || !a[OVS_VPORT_ATTR_TYPE] ||
	    !a[OVS_VPORT_ATTR_UPCALL_PID])
		goto exit;

	rtnl_lock();
	dp = get_dp(ovs_header->dp_ifindex);
	err = -ENODEV;
	if (!dp)
		goto exit_unlock;

	if (a[OVS_VPORT_ATTR_PORT_NO]) {
		port_no = nla_get_u32(a[OVS_VPORT_ATTR_PORT_NO]);

		err = -EFBIG;
		if (port_no >= DP_MAX_PORTS)
			goto exit_unlock;

		vport = rtnl_dereference(dp->ports[port_no]);
		err = -EBUSY;
		if (vport)
			goto exit_unlock;
=======
	unsigned int new_headroom;
	u32 port_no;
	int err;

	if (!a[OVS_VPORT_ATTR_NAME] || !a[OVS_VPORT_ATTR_TYPE] ||
	    !a[OVS_VPORT_ATTR_UPCALL_PID])
		return -EINVAL;

	parms.type = nla_get_u32(a[OVS_VPORT_ATTR_TYPE]);

	if (a[OVS_VPORT_ATTR_IFINDEX] && parms.type != OVS_VPORT_TYPE_INTERNAL)
		return -EOPNOTSUPP;

	port_no = a[OVS_VPORT_ATTR_PORT_NO]
		? nla_get_u32(a[OVS_VPORT_ATTR_PORT_NO]) : 0;
	if (port_no >= DP_MAX_PORTS)
		return -EFBIG;

	reply = ovs_vport_cmd_alloc_info();
	if (!reply)
		return -ENOMEM;

	ovs_lock();
restart:
	dp = get_dp(sock_net(skb->sk), ovs_header->dp_ifindex);
	err = -ENODEV;
	if (!dp)
		goto exit_unlock_free;

	if (port_no) {
		vport = ovs_vport_ovsl(dp, port_no);
		err = -EBUSY;
		if (vport)
			goto exit_unlock_free;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		for (port_no = 1; ; port_no++) {
			if (port_no >= DP_MAX_PORTS) {
				err = -EFBIG;
<<<<<<< HEAD
				goto exit_unlock;
			}
			vport = rtnl_dereference(dp->ports[port_no]);
=======
				goto exit_unlock_free;
			}
			vport = ovs_vport_ovsl(dp, port_no);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (!vport)
				break;
		}
	}

	parms.name = nla_data(a[OVS_VPORT_ATTR_NAME]);
<<<<<<< HEAD
	parms.type = nla_get_u32(a[OVS_VPORT_ATTR_TYPE]);
	parms.options = a[OVS_VPORT_ATTR_OPTIONS];
	parms.dp = dp;
	parms.port_no = port_no;
	parms.upcall_pid = nla_get_u32(a[OVS_VPORT_ATTR_UPCALL_PID]);

	vport = new_vport(&parms);
	err = PTR_ERR(vport);
	if (IS_ERR(vport))
		goto exit_unlock;

	reply = ovs_vport_cmd_build_info(vport, info->snd_pid, info->snd_seq,
					 OVS_VPORT_CMD_NEW);
	if (IS_ERR(reply)) {
		err = PTR_ERR(reply);
		ovs_dp_detach_port(vport);
		goto exit_unlock;
	}
	genl_notify(reply, genl_info_net(info), info->snd_pid,
		    ovs_dp_vport_multicast_group.id, info->nlhdr, GFP_KERNEL);

exit_unlock:
	rtnl_unlock();
exit:
=======
	parms.options = a[OVS_VPORT_ATTR_OPTIONS];
	parms.dp = dp;
	parms.port_no = port_no;
	parms.upcall_portids = a[OVS_VPORT_ATTR_UPCALL_PID];
	parms.desired_ifindex = a[OVS_VPORT_ATTR_IFINDEX]
		? nla_get_s32(a[OVS_VPORT_ATTR_IFINDEX]) : 0;

	vport = new_vport(&parms);
	err = PTR_ERR(vport);
	if (IS_ERR(vport)) {
		if (err == -EAGAIN)
			goto restart;
		goto exit_unlock_free;
	}

	err = ovs_vport_cmd_fill_info(vport, reply, genl_info_net(info),
				      info->snd_portid, info->snd_seq, 0,
				      OVS_VPORT_CMD_NEW, GFP_KERNEL);

	new_headroom = netdev_get_fwd_headroom(vport->dev);

	if (new_headroom > dp->max_headroom)
		ovs_update_headroom(dp, new_headroom);
	else
		netdev_set_rx_headroom(vport->dev, dp->max_headroom);

	BUG_ON(err < 0);
	ovs_unlock();

	ovs_notify(&dp_vport_genl_family, reply, info);
	return 0;

exit_unlock_free:
	ovs_unlock();
	kfree_skb(reply);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

static int ovs_vport_cmd_set(struct sk_buff *skb, struct genl_info *info)
{
	struct nlattr **a = info->attrs;
	struct sk_buff *reply;
	struct vport *vport;
	int err;

<<<<<<< HEAD
	rtnl_lock();
	vport = lookup_vport(info->userhdr, a);
	err = PTR_ERR(vport);
	if (IS_ERR(vport))
		goto exit_unlock;

	err = 0;
	if (a[OVS_VPORT_ATTR_TYPE] &&
	    nla_get_u32(a[OVS_VPORT_ATTR_TYPE]) != vport->ops->type)
		err = -EINVAL;

	if (!err && a[OVS_VPORT_ATTR_OPTIONS])
		err = ovs_vport_set_options(vport, a[OVS_VPORT_ATTR_OPTIONS]);
	if (!err && a[OVS_VPORT_ATTR_UPCALL_PID])
		vport->upcall_pid = nla_get_u32(a[OVS_VPORT_ATTR_UPCALL_PID]);

	reply = ovs_vport_cmd_build_info(vport, info->snd_pid, info->snd_seq,
					 OVS_VPORT_CMD_NEW);
	if (IS_ERR(reply)) {
		netlink_set_err(init_net.genl_sock, 0,
				ovs_dp_vport_multicast_group.id, PTR_ERR(reply));
		goto exit_unlock;
	}

	genl_notify(reply, genl_info_net(info), info->snd_pid,
		    ovs_dp_vport_multicast_group.id, info->nlhdr, GFP_KERNEL);

exit_unlock:
	rtnl_unlock();
=======
	reply = ovs_vport_cmd_alloc_info();
	if (!reply)
		return -ENOMEM;

	ovs_lock();
	vport = lookup_vport(sock_net(skb->sk), genl_info_userhdr(info), a);
	err = PTR_ERR(vport);
	if (IS_ERR(vport))
		goto exit_unlock_free;

	if (a[OVS_VPORT_ATTR_TYPE] &&
	    nla_get_u32(a[OVS_VPORT_ATTR_TYPE]) != vport->ops->type) {
		err = -EINVAL;
		goto exit_unlock_free;
	}

	if (a[OVS_VPORT_ATTR_OPTIONS]) {
		err = ovs_vport_set_options(vport, a[OVS_VPORT_ATTR_OPTIONS]);
		if (err)
			goto exit_unlock_free;
	}


	if (a[OVS_VPORT_ATTR_UPCALL_PID]) {
		struct nlattr *ids = a[OVS_VPORT_ATTR_UPCALL_PID];

		err = ovs_vport_set_upcall_portids(vport, ids);
		if (err)
			goto exit_unlock_free;
	}

	err = ovs_vport_cmd_fill_info(vport, reply, genl_info_net(info),
				      info->snd_portid, info->snd_seq, 0,
				      OVS_VPORT_CMD_SET, GFP_KERNEL);
	BUG_ON(err < 0);

	ovs_unlock();
	ovs_notify(&dp_vport_genl_family, reply, info);
	return 0;

exit_unlock_free:
	ovs_unlock();
	kfree_skb(reply);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

static int ovs_vport_cmd_del(struct sk_buff *skb, struct genl_info *info)
{
<<<<<<< HEAD
	struct nlattr **a = info->attrs;
	struct sk_buff *reply;
	struct vport *vport;
	int err;

	rtnl_lock();
	vport = lookup_vport(info->userhdr, a);
	err = PTR_ERR(vport);
	if (IS_ERR(vport))
		goto exit_unlock;

	if (vport->port_no == OVSP_LOCAL) {
		err = -EINVAL;
		goto exit_unlock;
	}

	reply = ovs_vport_cmd_build_info(vport, info->snd_pid, info->snd_seq,
					 OVS_VPORT_CMD_DEL);
	err = PTR_ERR(reply);
	if (IS_ERR(reply))
		goto exit_unlock;

	ovs_dp_detach_port(vport);

	genl_notify(reply, genl_info_net(info), info->snd_pid,
		    ovs_dp_vport_multicast_group.id, info->nlhdr, GFP_KERNEL);

exit_unlock:
	rtnl_unlock();
=======
	bool update_headroom = false;
	struct nlattr **a = info->attrs;
	struct sk_buff *reply;
	struct datapath *dp;
	struct vport *vport;
	unsigned int new_headroom;
	int err;

	reply = ovs_vport_cmd_alloc_info();
	if (!reply)
		return -ENOMEM;

	ovs_lock();
	vport = lookup_vport(sock_net(skb->sk), genl_info_userhdr(info), a);
	err = PTR_ERR(vport);
	if (IS_ERR(vport))
		goto exit_unlock_free;

	if (vport->port_no == OVSP_LOCAL) {
		err = -EINVAL;
		goto exit_unlock_free;
	}

	err = ovs_vport_cmd_fill_info(vport, reply, genl_info_net(info),
				      info->snd_portid, info->snd_seq, 0,
				      OVS_VPORT_CMD_DEL, GFP_KERNEL);
	BUG_ON(err < 0);

	/* the vport deletion may trigger dp headroom update */
	dp = vport->dp;
	if (netdev_get_fwd_headroom(vport->dev) == dp->max_headroom)
		update_headroom = true;

	netdev_reset_rx_headroom(vport->dev);
	ovs_dp_detach_port(vport);

	if (update_headroom) {
		new_headroom = ovs_get_max_headroom(dp);

		if (new_headroom < dp->max_headroom)
			ovs_update_headroom(dp, new_headroom);
	}
	ovs_unlock();

	ovs_notify(&dp_vport_genl_family, reply, info);
	return 0;

exit_unlock_free:
	ovs_unlock();
	kfree_skb(reply);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

static int ovs_vport_cmd_get(struct sk_buff *skb, struct genl_info *info)
{
	struct nlattr **a = info->attrs;
<<<<<<< HEAD
	struct ovs_header *ovs_header = info->userhdr;
=======
	struct ovs_header *ovs_header = genl_info_userhdr(info);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct sk_buff *reply;
	struct vport *vport;
	int err;

<<<<<<< HEAD
	rcu_read_lock();
	vport = lookup_vport(ovs_header, a);
	err = PTR_ERR(vport);
	if (IS_ERR(vport))
		goto exit_unlock;

	reply = ovs_vport_cmd_build_info(vport, info->snd_pid, info->snd_seq,
					 OVS_VPORT_CMD_NEW);
	err = PTR_ERR(reply);
	if (IS_ERR(reply))
		goto exit_unlock;

=======
	reply = ovs_vport_cmd_alloc_info();
	if (!reply)
		return -ENOMEM;

	rcu_read_lock();
	vport = lookup_vport(sock_net(skb->sk), ovs_header, a);
	err = PTR_ERR(vport);
	if (IS_ERR(vport))
		goto exit_unlock_free;
	err = ovs_vport_cmd_fill_info(vport, reply, genl_info_net(info),
				      info->snd_portid, info->snd_seq, 0,
				      OVS_VPORT_CMD_GET, GFP_ATOMIC);
	BUG_ON(err < 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rcu_read_unlock();

	return genlmsg_reply(reply, info);

<<<<<<< HEAD
exit_unlock:
	rcu_read_unlock();
=======
exit_unlock_free:
	rcu_read_unlock();
	kfree_skb(reply);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

static int ovs_vport_cmd_dump(struct sk_buff *skb, struct netlink_callback *cb)
{
	struct ovs_header *ovs_header = genlmsg_data(nlmsg_data(cb->nlh));
	struct datapath *dp;
<<<<<<< HEAD
	u32 port_no;
	int retval;

	dp = get_dp(ovs_header->dp_ifindex);
	if (!dp)
		return -ENODEV;

	rcu_read_lock();
	for (port_no = cb->args[0]; port_no < DP_MAX_PORTS; port_no++) {
		struct vport *vport;

		vport = rcu_dereference(dp->ports[port_no]);
		if (!vport)
			continue;

		if (ovs_vport_cmd_fill_info(vport, skb, NETLINK_CB(cb->skb).pid,
					    cb->nlh->nlmsg_seq, NLM_F_MULTI,
					    OVS_VPORT_CMD_NEW) < 0)
			break;
	}
	rcu_read_unlock();

	cb->args[0] = port_no;
	retval = skb->len;

	return retval;
}

static void rehash_flow_table(struct work_struct *work)
{
	struct datapath *dp;

	genl_lock();

	list_for_each_entry(dp, &dps, list_node) {
		struct flow_table *old_table = genl_dereference(dp->table);
		struct flow_table *new_table;

		new_table = ovs_flow_tbl_rehash(old_table);
		if (!IS_ERR(new_table)) {
			rcu_assign_pointer(dp->table, new_table);
			ovs_flow_tbl_deferred_destroy(old_table);
		}
	}

	genl_unlock();

	schedule_delayed_work(&rehash_flow_wq, REHASH_FLOW_INTERVAL);
}

static struct genl_ops dp_vport_genl_ops[] = {
	{ .cmd = OVS_VPORT_CMD_NEW,
	  .flags = GENL_ADMIN_PERM, /* Requires CAP_NET_ADMIN privilege. */
	  .policy = vport_policy,
	  .doit = ovs_vport_cmd_new
	},
	{ .cmd = OVS_VPORT_CMD_DEL,
	  .flags = GENL_ADMIN_PERM, /* Requires CAP_NET_ADMIN privilege. */
	  .policy = vport_policy,
	  .doit = ovs_vport_cmd_del
	},
	{ .cmd = OVS_VPORT_CMD_GET,
	  .flags = 0,		    /* OK for unprivileged users. */
	  .policy = vport_policy,
=======
	int bucket = cb->args[0], skip = cb->args[1];
	int i, j = 0;

	rcu_read_lock();
	dp = get_dp_rcu(sock_net(skb->sk), ovs_header->dp_ifindex);
	if (!dp) {
		rcu_read_unlock();
		return -ENODEV;
	}
	for (i = bucket; i < DP_VPORT_HASH_BUCKETS; i++) {
		struct vport *vport;

		j = 0;
		hlist_for_each_entry_rcu(vport, &dp->ports[i], dp_hash_node) {
			if (j >= skip &&
			    ovs_vport_cmd_fill_info(vport, skb,
						    sock_net(skb->sk),
						    NETLINK_CB(cb->skb).portid,
						    cb->nlh->nlmsg_seq,
						    NLM_F_MULTI,
						    OVS_VPORT_CMD_GET,
						    GFP_ATOMIC) < 0)
				goto out;

			j++;
		}
		skip = 0;
	}
out:
	rcu_read_unlock();

	cb->args[0] = i;
	cb->args[1] = j;

	return skb->len;
}

static void ovs_dp_masks_rebalance(struct work_struct *work)
{
	struct ovs_net *ovs_net = container_of(work, struct ovs_net,
					       masks_rebalance.work);
	struct datapath *dp;

	ovs_lock();

	list_for_each_entry(dp, &ovs_net->dps, list_node)
		ovs_flow_masks_rebalance(&dp->table);

	ovs_unlock();

	schedule_delayed_work(&ovs_net->masks_rebalance,
			      msecs_to_jiffies(DP_MASKS_REBALANCE_INTERVAL));
}

static const struct nla_policy vport_policy[OVS_VPORT_ATTR_MAX + 1] = {
	[OVS_VPORT_ATTR_NAME] = { .type = NLA_NUL_STRING, .len = IFNAMSIZ - 1 },
	[OVS_VPORT_ATTR_STATS] = { .len = sizeof(struct ovs_vport_stats) },
	[OVS_VPORT_ATTR_PORT_NO] = { .type = NLA_U32 },
	[OVS_VPORT_ATTR_TYPE] = { .type = NLA_U32 },
	[OVS_VPORT_ATTR_UPCALL_PID] = { .type = NLA_UNSPEC },
	[OVS_VPORT_ATTR_OPTIONS] = { .type = NLA_NESTED },
	[OVS_VPORT_ATTR_IFINDEX] = NLA_POLICY_MIN(NLA_S32, 0),
	[OVS_VPORT_ATTR_NETNSID] = { .type = NLA_S32 },
	[OVS_VPORT_ATTR_UPCALL_STATS] = { .type = NLA_NESTED },
};

static const struct genl_small_ops dp_vport_genl_ops[] = {
	{ .cmd = OVS_VPORT_CMD_NEW,
	  .validate = GENL_DONT_VALIDATE_STRICT | GENL_DONT_VALIDATE_DUMP,
	  .flags = GENL_UNS_ADMIN_PERM, /* Requires CAP_NET_ADMIN privilege. */
	  .doit = ovs_vport_cmd_new
	},
	{ .cmd = OVS_VPORT_CMD_DEL,
	  .validate = GENL_DONT_VALIDATE_STRICT | GENL_DONT_VALIDATE_DUMP,
	  .flags = GENL_UNS_ADMIN_PERM, /* Requires CAP_NET_ADMIN privilege. */
	  .doit = ovs_vport_cmd_del
	},
	{ .cmd = OVS_VPORT_CMD_GET,
	  .validate = GENL_DONT_VALIDATE_STRICT | GENL_DONT_VALIDATE_DUMP,
	  .flags = 0,		    /* OK for unprivileged users. */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	  .doit = ovs_vport_cmd_get,
	  .dumpit = ovs_vport_cmd_dump
	},
	{ .cmd = OVS_VPORT_CMD_SET,
<<<<<<< HEAD
	  .flags = GENL_ADMIN_PERM, /* Requires CAP_NET_ADMIN privilege. */
	  .policy = vport_policy,
=======
	  .validate = GENL_DONT_VALIDATE_STRICT | GENL_DONT_VALIDATE_DUMP,
	  .flags = GENL_UNS_ADMIN_PERM, /* Requires CAP_NET_ADMIN privilege. */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	  .doit = ovs_vport_cmd_set,
	},
};

<<<<<<< HEAD
struct genl_family_and_ops {
	struct genl_family *family;
	struct genl_ops *ops;
	int n_ops;
	struct genl_multicast_group *group;
};

static const struct genl_family_and_ops dp_genl_families[] = {
	{ &dp_datapath_genl_family,
	  dp_datapath_genl_ops, ARRAY_SIZE(dp_datapath_genl_ops),
	  &ovs_dp_datapath_multicast_group },
	{ &dp_vport_genl_family,
	  dp_vport_genl_ops, ARRAY_SIZE(dp_vport_genl_ops),
	  &ovs_dp_vport_multicast_group },
	{ &dp_flow_genl_family,
	  dp_flow_genl_ops, ARRAY_SIZE(dp_flow_genl_ops),
	  &ovs_dp_flow_multicast_group },
	{ &dp_packet_genl_family,
	  dp_packet_genl_ops, ARRAY_SIZE(dp_packet_genl_ops),
	  NULL },
=======
struct genl_family dp_vport_genl_family __ro_after_init = {
	.hdrsize = sizeof(struct ovs_header),
	.name = OVS_VPORT_FAMILY,
	.version = OVS_VPORT_VERSION,
	.maxattr = OVS_VPORT_ATTR_MAX,
	.policy = vport_policy,
	.netnsok = true,
	.parallel_ops = true,
	.small_ops = dp_vport_genl_ops,
	.n_small_ops = ARRAY_SIZE(dp_vport_genl_ops),
	.resv_start_op = OVS_VPORT_CMD_SET + 1,
	.mcgrps = &ovs_dp_vport_multicast_group,
	.n_mcgrps = 1,
	.module = THIS_MODULE,
};

static struct genl_family * const dp_genl_families[] = {
	&dp_datapath_genl_family,
	&dp_vport_genl_family,
	&dp_flow_genl_family,
	&dp_packet_genl_family,
	&dp_meter_genl_family,
#if	IS_ENABLED(CONFIG_NETFILTER_CONNCOUNT)
	&dp_ct_limit_genl_family,
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static void dp_unregister_genl(int n_families)
{
	int i;

	for (i = 0; i < n_families; i++)
<<<<<<< HEAD
		genl_unregister_family(dp_genl_families[i].family);
}

static int dp_register_genl(void)
{
	int n_registered;
	int err;
	int i;

	n_registered = 0;
	for (i = 0; i < ARRAY_SIZE(dp_genl_families); i++) {
		const struct genl_family_and_ops *f = &dp_genl_families[i];

		err = genl_register_family_with_ops(f->family, f->ops,
						    f->n_ops);
		if (err)
			goto error;
		n_registered++;

		if (f->group) {
			err = genl_register_mc_group(f->family, f->group);
			if (err)
				goto error;
		}
=======
		genl_unregister_family(dp_genl_families[i]);
}

static int __init dp_register_genl(void)
{
	int err;
	int i;

	for (i = 0; i < ARRAY_SIZE(dp_genl_families); i++) {

		err = genl_register_family(dp_genl_families[i]);
		if (err)
			goto error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return 0;

error:
<<<<<<< HEAD
	dp_unregister_genl(n_registered);
	return err;
}

static int __init dp_init(void)
{
	struct sk_buff *dummy_skb;
	int err;

	BUILD_BUG_ON(sizeof(struct ovs_skb_cb) > sizeof(dummy_skb->cb));

	pr_info("Open vSwitch switching datapath\n");

	err = ovs_flow_init();
	if (err)
		goto error;

=======
	dp_unregister_genl(i);
	return err;
}

static int __net_init ovs_init_net(struct net *net)
{
	struct ovs_net *ovs_net = net_generic(net, ovs_net_id);
	int err;

	INIT_LIST_HEAD(&ovs_net->dps);
	INIT_WORK(&ovs_net->dp_notify_work, ovs_dp_notify_wq);
	INIT_DELAYED_WORK(&ovs_net->masks_rebalance, ovs_dp_masks_rebalance);

	err = ovs_ct_init(net);
	if (err)
		return err;

	schedule_delayed_work(&ovs_net->masks_rebalance,
			      msecs_to_jiffies(DP_MASKS_REBALANCE_INTERVAL));
	return 0;
}

static void __net_exit list_vports_from_net(struct net *net, struct net *dnet,
					    struct list_head *head)
{
	struct ovs_net *ovs_net = net_generic(net, ovs_net_id);
	struct datapath *dp;

	list_for_each_entry(dp, &ovs_net->dps, list_node) {
		int i;

		for (i = 0; i < DP_VPORT_HASH_BUCKETS; i++) {
			struct vport *vport;

			hlist_for_each_entry(vport, &dp->ports[i], dp_hash_node) {
				if (vport->ops->type != OVS_VPORT_TYPE_INTERNAL)
					continue;

				if (dev_net(vport->dev) == dnet)
					list_add(&vport->detach_list, head);
			}
		}
	}
}

static void __net_exit ovs_exit_net(struct net *dnet)
{
	struct datapath *dp, *dp_next;
	struct ovs_net *ovs_net = net_generic(dnet, ovs_net_id);
	struct vport *vport, *vport_next;
	struct net *net;
	LIST_HEAD(head);

	ovs_lock();

	ovs_ct_exit(dnet);

	list_for_each_entry_safe(dp, dp_next, &ovs_net->dps, list_node)
		__dp_destroy(dp);

	down_read(&net_rwsem);
	for_each_net(net)
		list_vports_from_net(net, dnet, &head);
	up_read(&net_rwsem);

	/* Detach all vports from given namespace. */
	list_for_each_entry_safe(vport, vport_next, &head, detach_list) {
		list_del(&vport->detach_list);
		ovs_dp_detach_port(vport);
	}

	ovs_unlock();

	cancel_delayed_work_sync(&ovs_net->masks_rebalance);
	cancel_work_sync(&ovs_net->dp_notify_work);
}

static struct pernet_operations ovs_net_ops = {
	.init = ovs_init_net,
	.exit = ovs_exit_net,
	.id   = &ovs_net_id,
	.size = sizeof(struct ovs_net),
};

static const char * const ovs_drop_reasons[] = {
#define S(x)	(#x),
	OVS_DROP_REASONS(S)
#undef S
};

static struct drop_reason_list drop_reason_list_ovs = {
	.reasons = ovs_drop_reasons,
	.n_reasons = ARRAY_SIZE(ovs_drop_reasons),
};

static int __init dp_init(void)
{
	int err;

	BUILD_BUG_ON(sizeof(struct ovs_skb_cb) >
		     sizeof_field(struct sk_buff, cb));

	pr_info("Open vSwitch switching datapath\n");

	err = action_fifos_init();
	if (err)
		goto error;

	err = ovs_internal_dev_rtnl_link_register();
	if (err)
		goto error_action_fifos_exit;

	err = ovs_flow_init();
	if (err)
		goto error_unreg_rtnl_link;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	err = ovs_vport_init();
	if (err)
		goto error_flow_exit;

<<<<<<< HEAD
	err = register_netdevice_notifier(&ovs_dp_device_notifier);
	if (err)
		goto error_vport_exit;

	err = dp_register_genl();
	if (err < 0)
		goto error_unreg_notifier;

	schedule_delayed_work(&rehash_flow_wq, REHASH_FLOW_INTERVAL);

	return 0;

error_unreg_notifier:
	unregister_netdevice_notifier(&ovs_dp_device_notifier);
=======
	err = register_pernet_device(&ovs_net_ops);
	if (err)
		goto error_vport_exit;

	err = register_netdevice_notifier(&ovs_dp_device_notifier);
	if (err)
		goto error_netns_exit;

	err = ovs_netdev_init();
	if (err)
		goto error_unreg_notifier;

	err = dp_register_genl();
	if (err < 0)
		goto error_unreg_netdev;

	drop_reasons_register_subsys(SKB_DROP_REASON_SUBSYS_OPENVSWITCH,
				     &drop_reason_list_ovs);

	return 0;

error_unreg_netdev:
	ovs_netdev_exit();
error_unreg_notifier:
	unregister_netdevice_notifier(&ovs_dp_device_notifier);
error_netns_exit:
	unregister_pernet_device(&ovs_net_ops);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
error_vport_exit:
	ovs_vport_exit();
error_flow_exit:
	ovs_flow_exit();
<<<<<<< HEAD
=======
error_unreg_rtnl_link:
	ovs_internal_dev_rtnl_link_unregister();
error_action_fifos_exit:
	action_fifos_exit();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
error:
	return err;
}

static void dp_cleanup(void)
{
<<<<<<< HEAD
	cancel_delayed_work_sync(&rehash_flow_wq);
	rcu_barrier();
	dp_unregister_genl(ARRAY_SIZE(dp_genl_families));
	unregister_netdevice_notifier(&ovs_dp_device_notifier);
	ovs_vport_exit();
	ovs_flow_exit();
=======
	dp_unregister_genl(ARRAY_SIZE(dp_genl_families));
	ovs_netdev_exit();
	unregister_netdevice_notifier(&ovs_dp_device_notifier);
	unregister_pernet_device(&ovs_net_ops);
	drop_reasons_unregister_subsys(SKB_DROP_REASON_SUBSYS_OPENVSWITCH);
	rcu_barrier();
	ovs_vport_exit();
	ovs_flow_exit();
	ovs_internal_dev_rtnl_link_unregister();
	action_fifos_exit();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

module_init(dp_init);
module_exit(dp_cleanup);

MODULE_DESCRIPTION("Open vSwitch switching datapath");
MODULE_LICENSE("GPL");
<<<<<<< HEAD
=======
MODULE_ALIAS_GENL_FAMILY(OVS_DATAPATH_FAMILY);
MODULE_ALIAS_GENL_FAMILY(OVS_VPORT_FAMILY);
MODULE_ALIAS_GENL_FAMILY(OVS_FLOW_FAMILY);
MODULE_ALIAS_GENL_FAMILY(OVS_PACKET_FAMILY);
MODULE_ALIAS_GENL_FAMILY(OVS_METER_FAMILY);
MODULE_ALIAS_GENL_FAMILY(OVS_CT_LIMIT_FAMILY);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
