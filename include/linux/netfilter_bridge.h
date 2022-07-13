<<<<<<< HEAD
#ifndef __LINUX_BRIDGE_NETFILTER_H
#define __LINUX_BRIDGE_NETFILTER_H

/* bridge-specific defines for netfilter. 
 */

#include <linux/netfilter.h>
#include <linux/if_ether.h>
#include <linux/if_vlan.h>
#include <linux/if_pppox.h>

/* Bridge Hooks */
/* After promisc drops, checksum checks. */
#define NF_BR_PRE_ROUTING	0
/* If the packet is destined for this box. */
#define NF_BR_LOCAL_IN		1
/* If the packet is destined for another interface. */
#define NF_BR_FORWARD		2
/* Packets coming from a local process. */
#define NF_BR_LOCAL_OUT		3
/* Packets about to hit the wire. */
#define NF_BR_POST_ROUTING	4
/* Not really a hook, but used for the ebtables broute table */
#define NF_BR_BROUTING		5
#define NF_BR_NUMHOOKS		6

#ifdef __KERNEL__

enum nf_br_hook_priorities {
	NF_BR_PRI_FIRST = INT_MIN,
	NF_BR_PRI_NAT_DST_BRIDGED = -300,
	NF_BR_PRI_FILTER_BRIDGED = -200,
	NF_BR_PRI_BRNF = 0,
	NF_BR_PRI_NAT_DST_OTHER = 100,
	NF_BR_PRI_FILTER_OTHER = 200,
	NF_BR_PRI_NAT_SRC = 300,
	NF_BR_PRI_LAST = INT_MAX,
};

#ifdef CONFIG_BRIDGE_NETFILTER

#define BRNF_PKT_TYPE			0x01
#define BRNF_BRIDGED_DNAT		0x02
#define BRNF_BRIDGED			0x04
#define BRNF_NF_BRIDGE_PREROUTING	0x08
#define BRNF_8021Q			0x10
#define BRNF_PPPoE			0x20

/* Only used in br_forward.c */
extern int nf_bridge_copy_header(struct sk_buff *skb);
static inline int nf_bridge_maybe_copy_header(struct sk_buff *skb)
{
	if (skb->nf_bridge &&
	    skb->nf_bridge->mask & (BRNF_BRIDGED | BRNF_BRIDGED_DNAT))
		return nf_bridge_copy_header(skb);
  	return 0;
}

static inline unsigned int nf_bridge_encap_header_len(const struct sk_buff *skb)
{
	switch (skb->protocol) {
	case __cpu_to_be16(ETH_P_8021Q):
		return VLAN_HLEN;
	case __cpu_to_be16(ETH_P_PPP_SES):
		return PPPOE_SES_HLEN;
	default:
		return 0;
	}
}

static inline unsigned int nf_bridge_mtu_reduction(const struct sk_buff *skb)
{
	if (unlikely(skb->nf_bridge->mask & BRNF_PPPoE))
		return PPPOE_SES_HLEN;
	return 0;
}

extern int br_handle_frame_finish(struct sk_buff *skb);
/* Only used in br_device.c */
static inline int br_nf_pre_routing_finish_bridge_slow(struct sk_buff *skb)
{
	struct nf_bridge_info *nf_bridge = skb->nf_bridge;

	skb_pull(skb, ETH_HLEN);
	nf_bridge->mask ^= BRNF_BRIDGED_DNAT;
	skb_copy_to_linear_data_offset(skb, -(ETH_HLEN-ETH_ALEN),
				       skb->nf_bridge->data, ETH_HLEN-ETH_ALEN);
	skb->dev = nf_bridge->physindev;
	return br_handle_frame_finish(skb);
}

/* This is called by the IP fragmenting code and it ensures there is
 * enough room for the encapsulating header (if there is one). */
static inline unsigned int nf_bridge_pad(const struct sk_buff *skb)
{
	if (skb->nf_bridge)
		return nf_bridge_encap_header_len(skb);
	return 0;
}

struct bridge_skb_cb {
	union {
		__be32 ipv4;
	} daddr;
};
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LINUX_BRIDGE_NETFILTER_H
#define __LINUX_BRIDGE_NETFILTER_H

#include <uapi/linux/netfilter_bridge.h>
#include <linux/skbuff.h>

struct nf_bridge_frag_data {
	char    mac[ETH_HLEN];
	bool    vlan_present;
	u16     vlan_tci;
	__be16  vlan_proto;
};

#if IS_ENABLED(CONFIG_BRIDGE_NETFILTER)

int br_handle_frame_finish(struct net *net, struct sock *sk, struct sk_buff *skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline void br_drop_fake_rtable(struct sk_buff *skb)
{
	struct dst_entry *dst = skb_dst(skb);

	if (dst && (dst->flags & DST_FAKE_RTABLE))
		skb_dst_drop(skb);
}

<<<<<<< HEAD
#else
#define nf_bridge_maybe_copy_header(skb)	(0)
#define nf_bridge_pad(skb)			(0)
#define br_drop_fake_rtable(skb)	        do { } while (0)
#endif /* CONFIG_BRIDGE_NETFILTER */

#endif /* __KERNEL__ */
=======
static inline struct nf_bridge_info *
nf_bridge_info_get(const struct sk_buff *skb)
{
	return skb_ext_find(skb, SKB_EXT_BRIDGE_NF);
}

static inline bool nf_bridge_info_exists(const struct sk_buff *skb)
{
	return skb_ext_exist(skb, SKB_EXT_BRIDGE_NF);
}

static inline int nf_bridge_get_physinif(const struct sk_buff *skb)
{
	const struct nf_bridge_info *nf_bridge = nf_bridge_info_get(skb);

	if (!nf_bridge)
		return 0;

	return nf_bridge->physinif;
}

static inline int nf_bridge_get_physoutif(const struct sk_buff *skb)
{
	const struct nf_bridge_info *nf_bridge = nf_bridge_info_get(skb);

	if (!nf_bridge)
		return 0;

	return nf_bridge->physoutdev ? nf_bridge->physoutdev->ifindex : 0;
}

static inline struct net_device *
nf_bridge_get_physindev(const struct sk_buff *skb, struct net *net)
{
	const struct nf_bridge_info *nf_bridge = nf_bridge_info_get(skb);

	return nf_bridge ? dev_get_by_index_rcu(net, nf_bridge->physinif) : NULL;
}

static inline struct net_device *
nf_bridge_get_physoutdev(const struct sk_buff *skb)
{
	const struct nf_bridge_info *nf_bridge = nf_bridge_info_get(skb);

	return nf_bridge ? nf_bridge->physoutdev : NULL;
}

static inline bool nf_bridge_in_prerouting(const struct sk_buff *skb)
{
	const struct nf_bridge_info *nf_bridge = nf_bridge_info_get(skb);

	return nf_bridge && nf_bridge->in_prerouting;
}
#else
#define br_drop_fake_rtable(skb)	        do { } while (0)
static inline bool nf_bridge_in_prerouting(const struct sk_buff *skb)
{
	return false;
}
#endif /* CONFIG_BRIDGE_NETFILTER */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
