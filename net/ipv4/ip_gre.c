<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *	Linux NET3:	GRE over IP protocol decoder.
 *
 *	Authors: Alexey Kuznetsov (kuznet@ms2.inr.ac.ru)
<<<<<<< HEAD
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/capability.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/slab.h>
<<<<<<< HEAD
#include <asm/uaccess.h>
=======
#include <linux/uaccess.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/in.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/if_arp.h>
<<<<<<< HEAD
#include <linux/mroute.h>
=======
#include <linux/if_vlan.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/init.h>
#include <linux/in6.h>
#include <linux/inetdevice.h>
#include <linux/igmp.h>
#include <linux/netfilter_ipv4.h>
#include <linux/etherdevice.h>
#include <linux/if_ether.h>

#include <net/sock.h>
#include <net/ip.h>
#include <net/icmp.h>
#include <net/protocol.h>
<<<<<<< HEAD
#include <net/ipip.h>
=======
#include <net/ip_tunnels.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <net/arp.h>
#include <net/checksum.h>
#include <net/dsfield.h>
#include <net/inet_ecn.h>
#include <net/xfrm.h>
#include <net/net_namespace.h>
#include <net/netns/generic.h>
#include <net/rtnetlink.h>
#include <net/gre.h>
<<<<<<< HEAD

#if IS_ENABLED(CONFIG_IPV6)
#include <net/ipv6.h>
#include <net/ip6_fib.h>
#include <net/ip6_route.h>
#endif
=======
#include <net/dst_metadata.h>
#include <net/erspan.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
   Problems & solutions
   --------------------

   1. The most important issue is detecting local dead loops.
   They would cause complete host lockup in transmit, which
   would be "resolved" by stack overflow or, if queueing is enabled,
   with infinite looping in net_bh.

   We cannot track such dead loops during route installation,
   it is infeasible task. The most general solutions would be
   to keep skb->encapsulation counter (sort of local ttl),
   and silently drop packet when it expires. It is a good
   solution, but it supposes maintaining new variable in ALL
   skb, even if no tunneling is used.

   Current solution: xmit_recursion breaks dead loops. This is a percpu
   counter, since when we enter the first ndo_xmit(), cpu migration is
   forbidden. We force an exit if this counter reaches RECURSION_LIMIT

   2. Networking dead loops would not kill routers, but would really
   kill network. IP hop limit plays role of "t->recursion" in this case,
   if we copy it from packet being encapsulated to upper header.
   It is very good solution, but it introduces two problems:

   - Routing protocols, using packets with ttl=1 (OSPF, RIP2),
     do not work over tunnels.
   - traceroute does not work. I planned to relay ICMP from tunnel,
     so that this problem would be solved and traceroute output
     would even more informative. This idea appeared to be wrong:
     only Linux complies to rfc1812 now (yes, guys, Linux is the only
     true router now :-)), all routers (at least, in neighbourhood of mine)
     return only 8 bytes of payload. It is the end.

   Hence, if we want that OSPF worked or traceroute said something reasonable,
   we should search for another solution.

   One of them is to parse packet trying to detect inner encapsulation
   made by our node. It is difficult or even impossible, especially,
   taking into account fragmentation. TO be short, ttl is not solution at all.

   Current solution: The solution was UNEXPECTEDLY SIMPLE.
   We force DF flag on tunnels with preconfigured hop limit,
   that is ALL. :-) Well, it does not remove the problem completely,
   but exponential growth of network traffic is changed to linear
   (branches, that exceed pmtu are pruned) and tunnel mtu
   rapidly degrades to value <68, where looping stops.
   Yes, it is not good if there exists a router in the loop,
   which does not force DF, even when encapsulating packets have DF set.
   But it is not our problem! Nobody could accuse us, we made
   all that we could make. Even if it is your gated who injected
   fatal route to network, even if it were you who configured
   fatal static route: you are innocent. :-)

<<<<<<< HEAD


   3. Really, ipv4/ipip.c, ipv4/ip_gre.c and ipv6/sit.c contain
   practically identical code. It would be good to glue them
   together, but it is not very evident, how to make them modular.
   sit is integral part of IPv6, ipip and gre are naturally modular.
   We could extract common parts (hash table, ioctl etc)
   to a separate module (ip_tunnel.c).

   Alexey Kuznetsov.
 */

static struct rtnl_link_ops ipgre_link_ops __read_mostly;
static int ipgre_tunnel_init(struct net_device *dev);
static void ipgre_tunnel_setup(struct net_device *dev);
static int ipgre_tunnel_bind_dev(struct net_device *dev);

/* Fallback tunnel: no source, no destination, no key, no options */

#define HASH_SIZE  16

static int ipgre_net_id __read_mostly;
struct ipgre_net {
	struct ip_tunnel __rcu *tunnels[4][HASH_SIZE];

	struct net_device *fb_tunnel_dev;
};

/* Tunnel hash table */

/*
   4 hash tables:

   3: (remote,local)
   2: (remote,*)
   1: (*,local)
   0: (*,*)

   We require exact key match i.e. if a key is present in packet
   it will match only tunnel with the same key; if it is not present,
   it will match only keyless tunnel.

   All keysless packets, if not matched configured keyless tunnels
   will match fallback tunnel.
 */

#define HASH(addr) (((__force u32)addr^((__force u32)addr>>4))&0xF)

#define tunnels_r_l	tunnels[3]
#define tunnels_r	tunnels[2]
#define tunnels_l	tunnels[1]
#define tunnels_wc	tunnels[0]
/*
 * Locking : hash tables are protected by RCU and RTNL
 */

#define for_each_ip_tunnel_rcu(start) \
	for (t = rcu_dereference(start); t; t = rcu_dereference(t->next))

/* often modified stats are per cpu, other are shared (netdev->stats) */
struct pcpu_tstats {
	u64	rx_packets;
	u64	rx_bytes;
	u64	tx_packets;
	u64	tx_bytes;
	struct u64_stats_sync	syncp;
};

static struct rtnl_link_stats64 *ipgre_get_stats64(struct net_device *dev,
						   struct rtnl_link_stats64 *tot)
{
	int i;

	for_each_possible_cpu(i) {
		const struct pcpu_tstats *tstats = per_cpu_ptr(dev->tstats, i);
		u64 rx_packets, rx_bytes, tx_packets, tx_bytes;
		unsigned int start;

		do {
			start = u64_stats_fetch_begin_irq(&tstats->syncp);
			rx_packets = tstats->rx_packets;
			tx_packets = tstats->tx_packets;
			rx_bytes = tstats->rx_bytes;
			tx_bytes = tstats->tx_bytes;
		} while (u64_stats_fetch_retry_irq(&tstats->syncp, start));

		tot->rx_packets += rx_packets;
		tot->tx_packets += tx_packets;
		tot->rx_bytes   += rx_bytes;
		tot->tx_bytes   += tx_bytes;
	}

	tot->multicast = dev->stats.multicast;
	tot->rx_crc_errors = dev->stats.rx_crc_errors;
	tot->rx_fifo_errors = dev->stats.rx_fifo_errors;
	tot->rx_length_errors = dev->stats.rx_length_errors;
	tot->rx_errors = dev->stats.rx_errors;
	tot->tx_fifo_errors = dev->stats.tx_fifo_errors;
	tot->tx_carrier_errors = dev->stats.tx_carrier_errors;
	tot->tx_dropped = dev->stats.tx_dropped;
	tot->tx_aborted_errors = dev->stats.tx_aborted_errors;
	tot->tx_errors = dev->stats.tx_errors;

	return tot;
}

/* Given src, dst and key, find appropriate for input tunnel. */

static struct ip_tunnel * ipgre_tunnel_lookup(struct net_device *dev,
					      __be32 remote, __be32 local,
					      __be32 key, __be16 gre_proto)
{
	struct net *net = dev_net(dev);
	int link = dev->ifindex;
	unsigned int h0 = HASH(remote);
	unsigned int h1 = HASH(key);
	struct ip_tunnel *t, *cand = NULL;
	struct ipgre_net *ign = net_generic(net, ipgre_net_id);
	int dev_type = (gre_proto == htons(ETH_P_TEB)) ?
		       ARPHRD_ETHER : ARPHRD_IPGRE;
	int score, cand_score = 4;

	for_each_ip_tunnel_rcu(ign->tunnels_r_l[h0 ^ h1]) {
		if (local != t->parms.iph.saddr ||
		    remote != t->parms.iph.daddr ||
		    key != t->parms.i_key ||
		    !(t->dev->flags & IFF_UP))
			continue;

		if (t->dev->type != ARPHRD_IPGRE &&
		    t->dev->type != dev_type)
			continue;

		score = 0;
		if (t->parms.link != link)
			score |= 1;
		if (t->dev->type != dev_type)
			score |= 2;
		if (score == 0)
			return t;

		if (score < cand_score) {
			cand = t;
			cand_score = score;
		}
	}

	for_each_ip_tunnel_rcu(ign->tunnels_r[h0 ^ h1]) {
		if (remote != t->parms.iph.daddr ||
		    key != t->parms.i_key ||
		    !(t->dev->flags & IFF_UP))
			continue;

		if (t->dev->type != ARPHRD_IPGRE &&
		    t->dev->type != dev_type)
			continue;

		score = 0;
		if (t->parms.link != link)
			score |= 1;
		if (t->dev->type != dev_type)
			score |= 2;
		if (score == 0)
			return t;

		if (score < cand_score) {
			cand = t;
			cand_score = score;
		}
	}

	for_each_ip_tunnel_rcu(ign->tunnels_l[h1]) {
		if ((local != t->parms.iph.saddr &&
		     (local != t->parms.iph.daddr ||
		      !ipv4_is_multicast(local))) ||
		    key != t->parms.i_key ||
		    !(t->dev->flags & IFF_UP))
			continue;

		if (t->dev->type != ARPHRD_IPGRE &&
		    t->dev->type != dev_type)
			continue;

		score = 0;
		if (t->parms.link != link)
			score |= 1;
		if (t->dev->type != dev_type)
			score |= 2;
		if (score == 0)
			return t;

		if (score < cand_score) {
			cand = t;
			cand_score = score;
		}
	}

	for_each_ip_tunnel_rcu(ign->tunnels_wc[h1]) {
		if (t->parms.i_key != key ||
		    !(t->dev->flags & IFF_UP))
			continue;

		if (t->dev->type != ARPHRD_IPGRE &&
		    t->dev->type != dev_type)
			continue;

		score = 0;
		if (t->parms.link != link)
			score |= 1;
		if (t->dev->type != dev_type)
			score |= 2;
		if (score == 0)
			return t;

		if (score < cand_score) {
			cand = t;
			cand_score = score;
		}
	}

	if (cand != NULL)
		return cand;

	dev = ign->fb_tunnel_dev;
	if (dev->flags & IFF_UP)
		return netdev_priv(dev);

	return NULL;
}

static struct ip_tunnel __rcu **__ipgre_bucket(struct ipgre_net *ign,
		struct ip_tunnel_parm *parms)
{
	__be32 remote = parms->iph.daddr;
	__be32 local = parms->iph.saddr;
	__be32 key = parms->i_key;
	unsigned int h = HASH(key);
	int prio = 0;

	if (local)
		prio |= 1;
	if (remote && !ipv4_is_multicast(remote)) {
		prio |= 2;
		h ^= HASH(remote);
	}

	return &ign->tunnels[prio][h];
}

static inline struct ip_tunnel __rcu **ipgre_bucket(struct ipgre_net *ign,
		struct ip_tunnel *t)
{
	return __ipgre_bucket(ign, &t->parms);
}

static void ipgre_tunnel_link(struct ipgre_net *ign, struct ip_tunnel *t)
{
	struct ip_tunnel __rcu **tp = ipgre_bucket(ign, t);

	rcu_assign_pointer(t->next, rtnl_dereference(*tp));
	rcu_assign_pointer(*tp, t);
}

static void ipgre_tunnel_unlink(struct ipgre_net *ign, struct ip_tunnel *t)
{
	struct ip_tunnel __rcu **tp;
	struct ip_tunnel *iter;

	for (tp = ipgre_bucket(ign, t);
	     (iter = rtnl_dereference(*tp)) != NULL;
	     tp = &iter->next) {
		if (t == iter) {
			rcu_assign_pointer(*tp, t->next);
			break;
		}
	}
}

static struct ip_tunnel *ipgre_tunnel_find(struct net *net,
					   struct ip_tunnel_parm *parms,
					   int type)
{
	__be32 remote = parms->iph.daddr;
	__be32 local = parms->iph.saddr;
	__be32 key = parms->i_key;
	int link = parms->link;
	struct ip_tunnel *t;
	struct ip_tunnel __rcu **tp;
	struct ipgre_net *ign = net_generic(net, ipgre_net_id);

	for (tp = __ipgre_bucket(ign, parms);
	     (t = rtnl_dereference(*tp)) != NULL;
	     tp = &t->next)
		if (local == t->parms.iph.saddr &&
		    remote == t->parms.iph.daddr &&
		    key == t->parms.i_key &&
		    link == t->parms.link &&
		    type == t->dev->type)
			break;

	return t;
}

static struct ip_tunnel *ipgre_tunnel_locate(struct net *net,
		struct ip_tunnel_parm *parms, int create)
{
	struct ip_tunnel *t, *nt;
	struct net_device *dev;
	char name[IFNAMSIZ];
	struct ipgre_net *ign = net_generic(net, ipgre_net_id);

	t = ipgre_tunnel_find(net, parms, ARPHRD_IPGRE);
	if (t || !create)
		return t;

	if (parms->name[0])
		strlcpy(name, parms->name, IFNAMSIZ);
	else
		strcpy(name, "gre%d");

	dev = alloc_netdev(sizeof(*t), name, ipgre_tunnel_setup);
	if (!dev)
		return NULL;

	dev_net_set(dev, net);

	nt = netdev_priv(dev);
	nt->parms = *parms;
	dev->rtnl_link_ops = &ipgre_link_ops;

	dev->mtu = ipgre_tunnel_bind_dev(dev);

	if (register_netdevice(dev) < 0)
		goto failed_free;

	/* Can use a lockless transmit, unless we generate output sequences */
	if (!(nt->parms.o_flags & GRE_SEQ))
		dev->features |= NETIF_F_LLTX;

	dev_hold(dev);
	ipgre_tunnel_link(ign, nt);
	return nt;

failed_free:
	free_netdev(dev);
	return NULL;
}

static void ipgre_tunnel_uninit(struct net_device *dev)
{
	struct net *net = dev_net(dev);
	struct ipgre_net *ign = net_generic(net, ipgre_net_id);

	ipgre_tunnel_unlink(ign, netdev_priv(dev));
	dev_put(dev);
}


static void ipgre_err(struct sk_buff *skb, u32 info)
{

/* All the routers (except for Linux) return only
   8 bytes of packet payload. It means, that precise relaying of
   ICMP in the real Internet is absolutely infeasible.

   Moreover, Cisco "wise men" put GRE key to the third word
   in GRE header. It makes impossible maintaining even soft state for keyed
   GRE tunnels with enabled checksum. Tell them "thank you".

   Well, I wonder, rfc1812 was written by Cisco employee,
   what the hell these idiots break standards established
   by themselves???
 */

	const struct iphdr *iph = (const struct iphdr *)skb->data;
	__be16	     *p = (__be16*)(skb->data+(iph->ihl<<2));
	int grehlen = (iph->ihl<<2) + 4;
	const int type = icmp_hdr(skb)->type;
	const int code = icmp_hdr(skb)->code;
	struct ip_tunnel *t;
	__be16 flags;

	flags = p[0];
	if (flags&(GRE_CSUM|GRE_KEY|GRE_SEQ|GRE_ROUTING|GRE_VERSION)) {
		if (flags&(GRE_VERSION|GRE_ROUTING))
			return;
		if (flags&GRE_KEY) {
			grehlen += 4;
			if (flags&GRE_CSUM)
				grehlen += 4;
		}
	}

	/* If only 8 bytes returned, keyed message will be dropped here */
	if (skb_headlen(skb) < grehlen)
		return;
=======
   Alexey Kuznetsov.
 */

static bool log_ecn_error = true;
module_param(log_ecn_error, bool, 0644);
MODULE_PARM_DESC(log_ecn_error, "Log packets received with corrupted ECN");

static struct rtnl_link_ops ipgre_link_ops __read_mostly;
static const struct header_ops ipgre_header_ops;

static int ipgre_tunnel_init(struct net_device *dev);
static void erspan_build_header(struct sk_buff *skb,
				u32 id, u32 index,
				bool truncate, bool is_ipv4);

static unsigned int ipgre_net_id __read_mostly;
static unsigned int gre_tap_net_id __read_mostly;
static unsigned int erspan_net_id __read_mostly;

static int ipgre_err(struct sk_buff *skb, u32 info,
		     const struct tnl_ptk_info *tpi)
{

	/* All the routers (except for Linux) return only
	   8 bytes of packet payload. It means, that precise relaying of
	   ICMP in the real Internet is absolutely infeasible.

	   Moreover, Cisco "wise men" put GRE key to the third word
	   in GRE header. It makes impossible maintaining even soft
	   state for keyed GRE tunnels with enabled checksum. Tell
	   them "thank you".

	   Well, I wonder, rfc1812 was written by Cisco employee,
	   what the hell these idiots break standards established
	   by themselves???
	   */
	struct net *net = dev_net(skb->dev);
	struct ip_tunnel_net *itn;
	const struct iphdr *iph;
	const int type = icmp_hdr(skb)->type;
	const int code = icmp_hdr(skb)->code;
	unsigned int data_len = 0;
	struct ip_tunnel *t;

	if (tpi->proto == htons(ETH_P_TEB))
		itn = net_generic(net, gre_tap_net_id);
	else if (tpi->proto == htons(ETH_P_ERSPAN) ||
		 tpi->proto == htons(ETH_P_ERSPAN2))
		itn = net_generic(net, erspan_net_id);
	else
		itn = net_generic(net, ipgre_net_id);

	iph = (const struct iphdr *)(icmp_hdr(skb) + 1);
	t = ip_tunnel_lookup(itn, skb->dev->ifindex, tpi->flags,
			     iph->daddr, iph->saddr, tpi->key);

	if (!t)
		return -ENOENT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (type) {
	default:
	case ICMP_PARAMETERPROB:
<<<<<<< HEAD
		return;
=======
		return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	case ICMP_DEST_UNREACH:
		switch (code) {
		case ICMP_SR_FAILED:
		case ICMP_PORT_UNREACH:
			/* Impossible event. */
<<<<<<< HEAD
			return;
		case ICMP_FRAG_NEEDED:
			/* Soft state for pmtu is maintained by IP core. */
			return;
=======
			return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		default:
			/* All others are translated to HOST_UNREACH.
			   rfc2003 contains "deep thoughts" about NET_UNREACH,
			   I believe they are just ether pollution. --ANK
			 */
			break;
		}
		break;
<<<<<<< HEAD
	case ICMP_TIME_EXCEEDED:
		if (code != ICMP_EXC_TTL)
			return;
		break;
	}

	rcu_read_lock();
	t = ipgre_tunnel_lookup(skb->dev, iph->daddr, iph->saddr,
				flags & GRE_KEY ?
				*(((__be32 *)p) + (grehlen / 4) - 1) : 0,
				p[1]);
	if (t == NULL || t->parms.iph.daddr == 0 ||
	    ipv4_is_multicast(t->parms.iph.daddr))
		goto out;

	if (t->parms.iph.ttl == 0 && type == ICMP_TIME_EXCEEDED)
		goto out;
=======

	case ICMP_TIME_EXCEEDED:
		if (code != ICMP_EXC_TTL)
			return 0;
		data_len = icmp_hdr(skb)->un.reserved[1] * 4; /* RFC 4884 4.1 */
		break;

	case ICMP_REDIRECT:
		break;
	}

#if IS_ENABLED(CONFIG_IPV6)
	if (tpi->proto == htons(ETH_P_IPV6) &&
	    !ip6_err_gen_icmpv6_unreach(skb, iph->ihl * 4 + tpi->hdr_len,
					type, data_len))
		return 0;
#endif

	if (t->parms.iph.daddr == 0 ||
	    ipv4_is_multicast(t->parms.iph.daddr))
		return 0;

	if (t->parms.iph.ttl == 0 && type == ICMP_TIME_EXCEEDED)
		return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (time_before(jiffies, t->err_time + IPTUNNEL_ERR_TIMEO))
		t->err_count++;
	else
		t->err_count = 1;
	t->err_time = jiffies;
<<<<<<< HEAD
out:
	rcu_read_unlock();
}

static inline void ipgre_ecn_decapsulate(const struct iphdr *iph, struct sk_buff *skb)
{
	if (INET_ECN_is_ce(iph->tos)) {
		if (skb->protocol == htons(ETH_P_IP)) {
			IP_ECN_set_ce(ip_hdr(skb));
		} else if (skb->protocol == htons(ETH_P_IPV6)) {
			IP6_ECN_set_ce(ipv6_hdr(skb));
		}
	}
}

static inline u8
ipgre_ecn_encapsulate(u8 tos, const struct iphdr *old_iph, struct sk_buff *skb)
{
	u8 inner = 0;
	if (skb->protocol == htons(ETH_P_IP))
		inner = old_iph->tos;
	else if (skb->protocol == htons(ETH_P_IPV6))
		inner = ipv6_get_dsfield((const struct ipv6hdr *)old_iph);
	return INET_ECN_encapsulate(tos, inner);
}

static int ipgre_rcv(struct sk_buff *skb)
{
	const struct iphdr *iph;
	u8     *h;
	__be16    flags;
	__sum16   csum = 0;
	__be32 key = 0;
	u32    seqno = 0;
	struct ip_tunnel *tunnel;
	int    offset = 4;
	__be16 gre_proto;

	if (!pskb_may_pull(skb, 16))
		goto drop_nolock;

	iph = ip_hdr(skb);
	h = skb->data;
	flags = *(__be16*)h;

	if (flags&(GRE_CSUM|GRE_KEY|GRE_ROUTING|GRE_SEQ|GRE_VERSION)) {
		/* - Version must be 0.
		   - We do not support routing headers.
		 */
		if (flags&(GRE_VERSION|GRE_ROUTING))
			goto drop_nolock;

		if (flags&GRE_CSUM) {
			switch (skb->ip_summed) {
			case CHECKSUM_COMPLETE:
				csum = csum_fold(skb->csum);
				if (!csum)
					break;
				/* fall through */
			case CHECKSUM_NONE:
				skb->csum = 0;
				csum = __skb_checksum_complete(skb);
				skb->ip_summed = CHECKSUM_COMPLETE;
			}
			offset += 4;
		}
		if (flags&GRE_KEY) {
			key = *(__be32*)(h + offset);
			offset += 4;
		}
		if (flags&GRE_SEQ) {
			seqno = ntohl(*(__be32*)(h + offset));
			offset += 4;
		}
	}

	gre_proto = *(__be16 *)(h + 2);

	rcu_read_lock();
	if ((tunnel = ipgre_tunnel_lookup(skb->dev,
					  iph->saddr, iph->daddr, key,
					  gre_proto))) {
		struct pcpu_tstats *tstats;

		secpath_reset(skb);

		skb->protocol = gre_proto;
		/* WCCP version 1 and 2 protocol decoding.
		 * - Change protocol to IP
		 * - When dealing with WCCPv2, Skip extra 4 bytes in GRE header
		 */
		if (flags == 0 && gre_proto == htons(ETH_P_WCCP)) {
			skb->protocol = htons(ETH_P_IP);
			if ((*(h + offset) & 0xF0) != 0x40)
				offset += 4;
		}

		skb->mac_header = skb->network_header;
		__pskb_pull(skb, offset);
		skb_postpull_rcsum(skb, skb_transport_header(skb), offset);
		skb->pkt_type = PACKET_HOST;
#ifdef CONFIG_NET_IPGRE_BROADCAST
		if (ipv4_is_multicast(iph->daddr)) {
			/* Looped back packet, drop it! */
			if (rt_is_output_route(skb_rtable(skb)))
				goto drop;
			tunnel->dev->stats.multicast++;
			skb->pkt_type = PACKET_BROADCAST;
		}
#endif

		if (((flags&GRE_CSUM) && csum) ||
		    (!(flags&GRE_CSUM) && tunnel->parms.i_flags&GRE_CSUM)) {
			tunnel->dev->stats.rx_crc_errors++;
			tunnel->dev->stats.rx_errors++;
			goto drop;
		}
		if (tunnel->parms.i_flags&GRE_SEQ) {
			if (!(flags&GRE_SEQ) ||
			    (tunnel->i_seqno && (s32)(seqno - tunnel->i_seqno) < 0)) {
				tunnel->dev->stats.rx_fifo_errors++;
				tunnel->dev->stats.rx_errors++;
				goto drop;
			}
			tunnel->i_seqno = seqno + 1;
		}

		/* Warning: All skb pointers will be invalidated! */
		if (tunnel->dev->type == ARPHRD_ETHER) {
			if (!pskb_may_pull(skb, ETH_HLEN)) {
				tunnel->dev->stats.rx_length_errors++;
				tunnel->dev->stats.rx_errors++;
				goto drop;
			}

			iph = ip_hdr(skb);
			skb->protocol = eth_type_trans(skb, tunnel->dev);
			skb_postpull_rcsum(skb, eth_hdr(skb), ETH_HLEN);
		}

		tstats = this_cpu_ptr(tunnel->dev->tstats);
		u64_stats_update_begin(&tstats->syncp);
		tstats->rx_packets++;
		tstats->rx_bytes += skb->len;
		u64_stats_update_end(&tstats->syncp);

		__skb_tunnel_rx(skb, tunnel->dev);

		skb_reset_network_header(skb);
		ipgre_ecn_decapsulate(iph, skb);

		netif_rx(skb);

		rcu_read_unlock();
		return 0;
	}
	icmp_send(skb, ICMP_DEST_UNREACH, ICMP_PORT_UNREACH, 0);

drop:
	rcu_read_unlock();
drop_nolock:
=======

	return 0;
}

static void gre_err(struct sk_buff *skb, u32 info)
{
	/* All the routers (except for Linux) return only
	 * 8 bytes of packet payload. It means, that precise relaying of
	 * ICMP in the real Internet is absolutely infeasible.
	 *
	 * Moreover, Cisco "wise men" put GRE key to the third word
	 * in GRE header. It makes impossible maintaining even soft
	 * state for keyed
	 * GRE tunnels with enabled checksum. Tell them "thank you".
	 *
	 * Well, I wonder, rfc1812 was written by Cisco employee,
	 * what the hell these idiots break standards established
	 * by themselves???
	 */

	const struct iphdr *iph = (struct iphdr *)skb->data;
	const int type = icmp_hdr(skb)->type;
	const int code = icmp_hdr(skb)->code;
	struct tnl_ptk_info tpi;

	if (gre_parse_header(skb, &tpi, NULL, htons(ETH_P_IP),
			     iph->ihl * 4) < 0)
		return;

	if (type == ICMP_DEST_UNREACH && code == ICMP_FRAG_NEEDED) {
		ipv4_update_pmtu(skb, dev_net(skb->dev), info,
				 skb->dev->ifindex, IPPROTO_GRE);
		return;
	}
	if (type == ICMP_REDIRECT) {
		ipv4_redirect(skb, dev_net(skb->dev), skb->dev->ifindex,
			      IPPROTO_GRE);
		return;
	}

	ipgre_err(skb, info, &tpi);
}

static bool is_erspan_type1(int gre_hdr_len)
{
	/* Both ERSPAN type I (version 0) and type II (version 1) use
	 * protocol 0x88BE, but the type I has only 4-byte GRE header,
	 * while type II has 8-byte.
	 */
	return gre_hdr_len == 4;
}

static int erspan_rcv(struct sk_buff *skb, struct tnl_ptk_info *tpi,
		      int gre_hdr_len)
{
	struct net *net = dev_net(skb->dev);
	struct metadata_dst *tun_dst = NULL;
	struct erspan_base_hdr *ershdr;
	struct ip_tunnel_net *itn;
	struct ip_tunnel *tunnel;
	const struct iphdr *iph;
	struct erspan_md2 *md2;
	int ver;
	int len;

	itn = net_generic(net, erspan_net_id);
	iph = ip_hdr(skb);
	if (is_erspan_type1(gre_hdr_len)) {
		ver = 0;
		tunnel = ip_tunnel_lookup(itn, skb->dev->ifindex,
					  tpi->flags | TUNNEL_NO_KEY,
					  iph->saddr, iph->daddr, 0);
	} else {
		if (unlikely(!pskb_may_pull(skb,
					    gre_hdr_len + sizeof(*ershdr))))
			return PACKET_REJECT;

		ershdr = (struct erspan_base_hdr *)(skb->data + gre_hdr_len);
		ver = ershdr->ver;
		iph = ip_hdr(skb);
		tunnel = ip_tunnel_lookup(itn, skb->dev->ifindex,
					  tpi->flags | TUNNEL_KEY,
					  iph->saddr, iph->daddr, tpi->key);
	}

	if (tunnel) {
		if (is_erspan_type1(gre_hdr_len))
			len = gre_hdr_len;
		else
			len = gre_hdr_len + erspan_hdr_len(ver);

		if (unlikely(!pskb_may_pull(skb, len)))
			return PACKET_REJECT;

		if (__iptunnel_pull_header(skb,
					   len,
					   htons(ETH_P_TEB),
					   false, false) < 0)
			goto drop;

		if (tunnel->collect_md) {
			struct erspan_metadata *pkt_md, *md;
			struct ip_tunnel_info *info;
			unsigned char *gh;
			__be64 tun_id;
			__be16 flags;

			tpi->flags |= TUNNEL_KEY;
			flags = tpi->flags;
			tun_id = key32_to_tunnel_id(tpi->key);

			tun_dst = ip_tun_rx_dst(skb, flags,
						tun_id, sizeof(*md));
			if (!tun_dst)
				return PACKET_REJECT;

			/* skb can be uncloned in __iptunnel_pull_header, so
			 * old pkt_md is no longer valid and we need to reset
			 * it
			 */
			gh = skb_network_header(skb) +
			     skb_network_header_len(skb);
			pkt_md = (struct erspan_metadata *)(gh + gre_hdr_len +
							    sizeof(*ershdr));
			md = ip_tunnel_info_opts(&tun_dst->u.tun_info);
			md->version = ver;
			md2 = &md->u.md2;
			memcpy(md2, pkt_md, ver == 1 ? ERSPAN_V1_MDSIZE :
						       ERSPAN_V2_MDSIZE);

			info = &tun_dst->u.tun_info;
			info->key.tun_flags |= TUNNEL_ERSPAN_OPT;
			info->options_len = sizeof(*md);
		}

		skb_reset_mac_header(skb);
		ip_tunnel_rcv(tunnel, skb, tpi, tun_dst, log_ecn_error);
		return PACKET_RCVD;
	}
	return PACKET_REJECT;

drop:
	kfree_skb(skb);
	return PACKET_RCVD;
}

static int __ipgre_rcv(struct sk_buff *skb, const struct tnl_ptk_info *tpi,
		       struct ip_tunnel_net *itn, int hdr_len, bool raw_proto)
{
	struct metadata_dst *tun_dst = NULL;
	const struct iphdr *iph;
	struct ip_tunnel *tunnel;

	iph = ip_hdr(skb);
	tunnel = ip_tunnel_lookup(itn, skb->dev->ifindex, tpi->flags,
				  iph->saddr, iph->daddr, tpi->key);

	if (tunnel) {
		const struct iphdr *tnl_params;

		if (__iptunnel_pull_header(skb, hdr_len, tpi->proto,
					   raw_proto, false) < 0)
			goto drop;

		/* Special case for ipgre_header_parse(), which expects the
		 * mac_header to point to the outer IP header.
		 */
		if (tunnel->dev->header_ops == &ipgre_header_ops)
			skb_pop_mac_header(skb);
		else
			skb_reset_mac_header(skb);

		tnl_params = &tunnel->parms.iph;
		if (tunnel->collect_md || tnl_params->daddr == 0) {
			__be16 flags;
			__be64 tun_id;

			flags = tpi->flags & (TUNNEL_CSUM | TUNNEL_KEY);
			tun_id = key32_to_tunnel_id(tpi->key);
			tun_dst = ip_tun_rx_dst(skb, flags, tun_id, 0);
			if (!tun_dst)
				return PACKET_REJECT;
		}

		ip_tunnel_rcv(tunnel, skb, tpi, tun_dst, log_ecn_error);
		return PACKET_RCVD;
	}
	return PACKET_NEXT;

drop:
	kfree_skb(skb);
	return PACKET_RCVD;
}

static int ipgre_rcv(struct sk_buff *skb, const struct tnl_ptk_info *tpi,
		     int hdr_len)
{
	struct net *net = dev_net(skb->dev);
	struct ip_tunnel_net *itn;
	int res;

	if (tpi->proto == htons(ETH_P_TEB))
		itn = net_generic(net, gre_tap_net_id);
	else
		itn = net_generic(net, ipgre_net_id);

	res = __ipgre_rcv(skb, tpi, itn, hdr_len, false);
	if (res == PACKET_NEXT && tpi->proto == htons(ETH_P_TEB)) {
		/* ipgre tunnels in collect metadata mode should receive
		 * also ETH_P_TEB traffic.
		 */
		itn = net_generic(net, ipgre_net_id);
		res = __ipgre_rcv(skb, tpi, itn, hdr_len, true);
	}
	return res;
}

static int gre_rcv(struct sk_buff *skb)
{
	struct tnl_ptk_info tpi;
	bool csum_err = false;
	int hdr_len;

#ifdef CONFIG_NET_IPGRE_BROADCAST
	if (ipv4_is_multicast(ip_hdr(skb)->daddr)) {
		/* Looped back packet, drop it! */
		if (rt_is_output_route(skb_rtable(skb)))
			goto drop;
	}
#endif

	hdr_len = gre_parse_header(skb, &tpi, &csum_err, htons(ETH_P_IP), 0);
	if (hdr_len < 0)
		goto drop;

	if (unlikely(tpi.proto == htons(ETH_P_ERSPAN) ||
		     tpi.proto == htons(ETH_P_ERSPAN2))) {
		if (erspan_rcv(skb, &tpi, hdr_len) == PACKET_RCVD)
			return 0;
		goto out;
	}

	if (ipgre_rcv(skb, &tpi, hdr_len) == PACKET_RCVD)
		return 0;

out:
	icmp_send(skb, ICMP_DEST_UNREACH, ICMP_PORT_UNREACH, 0);
drop:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kfree_skb(skb);
	return 0;
}

<<<<<<< HEAD
static netdev_tx_t ipgre_tunnel_xmit(struct sk_buff *skb, struct net_device *dev)
{
	struct ip_tunnel *tunnel = netdev_priv(dev);
	struct pcpu_tstats *tstats;
	const struct iphdr  *old_iph = ip_hdr(skb);
	const struct iphdr  *tiph;
	struct flowi4 fl4;
	u8     tos;
	__be16 df;
	struct rtable *rt;     			/* Route to the other host */
	struct net_device *tdev;		/* Device to other host */
	struct iphdr  *iph;			/* Our new IP header */
	unsigned int max_headroom;		/* The extra header space needed */
	int    gre_hlen;
	__be32 dst;
	int    mtu;

	if (dev->type == ARPHRD_ETHER)
		IPCB(skb)->flags = 0;

	if (dev->header_ops && dev->type == ARPHRD_IPGRE) {
		gre_hlen = 0;
		tiph = (const struct iphdr *)skb->data;
	} else {
		gre_hlen = tunnel->hlen;
		tiph = &tunnel->parms.iph;
	}

	memset(&(IPCB(skb)->opt), 0, sizeof(IPCB(skb)->opt));
	if ((dst = tiph->daddr) == 0) {
		/* NBMA tunnel */

		if (skb_dst(skb) == NULL) {
			dev->stats.tx_fifo_errors++;
			goto tx_error;
		}

		if (skb->protocol == htons(ETH_P_IP)) {
			rt = skb_rtable(skb);
			dst = rt->rt_gateway;
		}
#if IS_ENABLED(CONFIG_IPV6)
		else if (skb->protocol == htons(ETH_P_IPV6)) {
			const struct in6_addr *addr6;
			struct neighbour *neigh;
			bool do_tx_error_icmp;
			int addr_type;

			neigh = dst_neigh_lookup(skb_dst(skb), &ipv6_hdr(skb)->daddr);
			if (neigh == NULL)
				goto tx_error;

			addr6 = (const struct in6_addr *)&neigh->primary_key;
			addr_type = ipv6_addr_type(addr6);

			if (addr_type == IPV6_ADDR_ANY) {
				addr6 = &ipv6_hdr(skb)->daddr;
				addr_type = ipv6_addr_type(addr6);
			}

			if ((addr_type & IPV6_ADDR_COMPATv4) == 0)
				do_tx_error_icmp = true;
			else {
				do_tx_error_icmp = false;
				dst = addr6->s6_addr32[3];
			}
			neigh_release(neigh);
			if (do_tx_error_icmp)
				goto tx_error_icmp;
		}
#endif
		else
			goto tx_error;
	}

	tos = tiph->tos;
	if (tos == 1) {
		tos = 0;
		if (skb->protocol == htons(ETH_P_IP))
			tos = old_iph->tos;
		else if (skb->protocol == htons(ETH_P_IPV6))
			tos = ipv6_get_dsfield((const struct ipv6hdr *)old_iph);
	}

	rt = ip_route_output_gre(dev_net(dev), &fl4, dst, tiph->saddr,
				 tunnel->parms.o_key, RT_TOS(tos),
				 tunnel->parms.link);
	if (IS_ERR(rt)) {
		dev->stats.tx_carrier_errors++;
		goto tx_error;
	}
	tdev = rt->dst.dev;

	if (tdev == dev) {
		ip_rt_put(rt);
		dev->stats.collisions++;
		goto tx_error;
	}

	df = tiph->frag_off;
	if (df)
		mtu = dst_mtu(&rt->dst) - dev->hard_header_len - tunnel->hlen;
	else
		mtu = skb_dst(skb) ? dst_mtu(skb_dst(skb)) : dev->mtu;

	if (skb_dst(skb))
		skb_dst(skb)->ops->update_pmtu(skb_dst(skb), mtu);

	if (skb->protocol == htons(ETH_P_IP)) {
		df |= (old_iph->frag_off&htons(IP_DF));

		if ((old_iph->frag_off&htons(IP_DF)) &&
		    mtu < ntohs(old_iph->tot_len)) {
			icmp_send(skb, ICMP_DEST_UNREACH, ICMP_FRAG_NEEDED, htonl(mtu));
			ip_rt_put(rt);
			goto tx_error;
		}
	}
#if IS_ENABLED(CONFIG_IPV6)
	else if (skb->protocol == htons(ETH_P_IPV6)) {
		struct rt6_info *rt6 = (struct rt6_info *)skb_dst(skb);

		if (rt6 && mtu < dst_mtu(skb_dst(skb)) && mtu >= IPV6_MIN_MTU) {
			if ((tunnel->parms.iph.daddr &&
			     !ipv4_is_multicast(tunnel->parms.iph.daddr)) ||
			    rt6->rt6i_dst.plen == 128) {
				rt6->rt6i_flags |= RTF_MODIFIED;
				dst_metric_set(skb_dst(skb), RTAX_MTU, mtu);
			}
		}

		if (mtu >= IPV6_MIN_MTU && mtu < skb->len - tunnel->hlen + gre_hlen) {
			icmpv6_send(skb, ICMPV6_PKT_TOOBIG, 0, mtu);
			ip_rt_put(rt);
			goto tx_error;
		}
	}
#endif

	if (tunnel->err_count > 0) {
		if (time_before(jiffies,
				tunnel->err_time + IPTUNNEL_ERR_TIMEO)) {
			tunnel->err_count--;

			dst_link_failure(skb);
		} else
			tunnel->err_count = 0;
	}

	max_headroom = LL_RESERVED_SPACE(tdev) + gre_hlen + rt->dst.header_len;

	if (skb_headroom(skb) < max_headroom || skb_shared(skb)||
	    (skb_cloned(skb) && !skb_clone_writable(skb, 0))) {
		struct sk_buff *new_skb = skb_realloc_headroom(skb, max_headroom);
		if (max_headroom > dev->needed_headroom)
			dev->needed_headroom = max_headroom;
		if (!new_skb) {
			ip_rt_put(rt);
			dev->stats.tx_dropped++;
			dev_kfree_skb(skb);
			return NETDEV_TX_OK;
		}
		if (skb->sk)
			skb_set_owner_w(new_skb, skb->sk);
		dev_kfree_skb(skb);
		skb = new_skb;
		old_iph = ip_hdr(skb);
	}

	skb_reset_transport_header(skb);
	skb_push(skb, gre_hlen);
	skb_reset_network_header(skb);
	IPCB(skb)->flags &= ~(IPSKB_XFRM_TUNNEL_SIZE | IPSKB_XFRM_TRANSFORMED |
			      IPSKB_REROUTED);
	skb_dst_drop(skb);
	skb_dst_set(skb, &rt->dst);

	/*
	 *	Push down and install the IPIP header.
	 */

	iph 			=	ip_hdr(skb);
	iph->version		=	4;
	iph->ihl		=	sizeof(struct iphdr) >> 2;
	iph->frag_off		=	df;
	iph->protocol		=	IPPROTO_GRE;
	iph->tos		=	ipgre_ecn_encapsulate(tos, old_iph, skb);
	iph->daddr		=	fl4.daddr;
	iph->saddr		=	fl4.saddr;

	if ((iph->ttl = tiph->ttl) == 0) {
		if (skb->protocol == htons(ETH_P_IP))
			iph->ttl = old_iph->ttl;
#if IS_ENABLED(CONFIG_IPV6)
		else if (skb->protocol == htons(ETH_P_IPV6))
			iph->ttl = ((const struct ipv6hdr *)old_iph)->hop_limit;
#endif
		else
			iph->ttl = ip4_dst_hoplimit(&rt->dst);
	}

	((__be16 *)(iph + 1))[0] = tunnel->parms.o_flags;
	((__be16 *)(iph + 1))[1] = (dev->type == ARPHRD_ETHER) ?
				   htons(ETH_P_TEB) : skb->protocol;

	if (tunnel->parms.o_flags&(GRE_KEY|GRE_CSUM|GRE_SEQ)) {
		__be32 *ptr = (__be32*)(((u8*)iph) + tunnel->hlen - 4);

		if (tunnel->parms.o_flags&GRE_SEQ) {
			++tunnel->o_seqno;
			*ptr = htonl(tunnel->o_seqno);
			ptr--;
		}
		if (tunnel->parms.o_flags&GRE_KEY) {
			*ptr = tunnel->parms.o_key;
			ptr--;
		}
		if (tunnel->parms.o_flags&GRE_CSUM) {
			*ptr = 0;
			*(__sum16*)ptr = ip_compute_csum((void*)(iph+1), skb->len - sizeof(struct iphdr));
		}
	}

	nf_reset(skb);
	tstats = this_cpu_ptr(dev->tstats);
	__IPTUNNEL_XMIT(tstats, &dev->stats);
	return NETDEV_TX_OK;

#if IS_ENABLED(CONFIG_IPV6)
tx_error_icmp:
	dst_link_failure(skb);
#endif
tx_error:
	dev->stats.tx_errors++;
	dev_kfree_skb(skb);
	return NETDEV_TX_OK;
}

static int ipgre_tunnel_bind_dev(struct net_device *dev)
{
	struct net_device *tdev = NULL;
	struct ip_tunnel *tunnel;
	const struct iphdr *iph;
	int hlen = LL_MAX_HEADER;
	int mtu = ETH_DATA_LEN;
	int addend = sizeof(struct iphdr) + 4;

	tunnel = netdev_priv(dev);
	iph = &tunnel->parms.iph;

	/* Guess output device to choose reasonable mtu and needed_headroom */

	if (iph->daddr) {
		struct flowi4 fl4;
		struct rtable *rt;

		rt = ip_route_output_gre(dev_net(dev), &fl4,
					 iph->daddr, iph->saddr,
					 tunnel->parms.o_key,
					 RT_TOS(iph->tos),
					 tunnel->parms.link);
		if (!IS_ERR(rt)) {
			tdev = rt->dst.dev;
			ip_rt_put(rt);
		}

		if (dev->type != ARPHRD_ETHER)
			dev->flags |= IFF_POINTOPOINT;
	}

	if (!tdev && tunnel->parms.link)
		tdev = __dev_get_by_index(dev_net(dev), tunnel->parms.link);

	if (tdev) {
		hlen = tdev->hard_header_len + tdev->needed_headroom;
		mtu = tdev->mtu;
	}
	dev->iflink = tunnel->parms.link;

	/* Precalculate GRE options length */
	if (tunnel->parms.o_flags&(GRE_CSUM|GRE_KEY|GRE_SEQ)) {
		if (tunnel->parms.o_flags&GRE_CSUM)
			addend += 4;
		if (tunnel->parms.o_flags&GRE_KEY)
			addend += 4;
		if (tunnel->parms.o_flags&GRE_SEQ)
			addend += 4;
	}
	dev->needed_headroom = addend + hlen;
	mtu -= dev->hard_header_len + addend;

	if (mtu < 68)
		mtu = 68;

	tunnel->hlen = addend;

	return mtu;
}

static int
ipgre_tunnel_ioctl (struct net_device *dev, struct ifreq *ifr, int cmd)
{
	int err = 0;
	struct ip_tunnel_parm p;
	struct ip_tunnel *t;
	struct net *net = dev_net(dev);
	struct ipgre_net *ign = net_generic(net, ipgre_net_id);

	switch (cmd) {
	case SIOCGETTUNNEL:
		t = NULL;
		if (dev == ign->fb_tunnel_dev) {
			if (copy_from_user(&p, ifr->ifr_ifru.ifru_data, sizeof(p))) {
				err = -EFAULT;
				break;
			}
			t = ipgre_tunnel_locate(net, &p, 0);
		}
		if (t == NULL)
			t = netdev_priv(dev);
		memcpy(&p, &t->parms, sizeof(p));
		if (copy_to_user(ifr->ifr_ifru.ifru_data, &p, sizeof(p)))
			err = -EFAULT;
		break;

	case SIOCADDTUNNEL:
	case SIOCCHGTUNNEL:
		err = -EPERM;
		if (!ns_capable(net->user_ns, CAP_NET_ADMIN))
			goto done;

		err = -EFAULT;
		if (copy_from_user(&p, ifr->ifr_ifru.ifru_data, sizeof(p)))
			goto done;

		err = -EINVAL;
		if (p.iph.version != 4 || p.iph.protocol != IPPROTO_GRE ||
		    p.iph.ihl != 5 || (p.iph.frag_off&htons(~IP_DF)) ||
		    ((p.i_flags|p.o_flags)&(GRE_VERSION|GRE_ROUTING)))
			goto done;
		if (p.iph.ttl)
			p.iph.frag_off |= htons(IP_DF);

		if (!(p.i_flags&GRE_KEY))
			p.i_key = 0;
		if (!(p.o_flags&GRE_KEY))
			p.o_key = 0;

		t = ipgre_tunnel_locate(net, &p, cmd == SIOCADDTUNNEL);

		if (dev != ign->fb_tunnel_dev && cmd == SIOCCHGTUNNEL) {
			if (t != NULL) {
				if (t->dev != dev) {
					err = -EEXIST;
					break;
				}
			} else {
				unsigned int nflags = 0;

				t = netdev_priv(dev);

				if (ipv4_is_multicast(p.iph.daddr))
					nflags = IFF_BROADCAST;
				else if (p.iph.daddr)
					nflags = IFF_POINTOPOINT;

				if ((dev->flags^nflags)&(IFF_POINTOPOINT|IFF_BROADCAST)) {
					err = -EINVAL;
					break;
				}
				ipgre_tunnel_unlink(ign, t);
				synchronize_net();
				t->parms.iph.saddr = p.iph.saddr;
				t->parms.iph.daddr = p.iph.daddr;
				t->parms.i_key = p.i_key;
				t->parms.o_key = p.o_key;
				memcpy(dev->dev_addr, &p.iph.saddr, 4);
				memcpy(dev->broadcast, &p.iph.daddr, 4);
				ipgre_tunnel_link(ign, t);
				netdev_state_change(dev);
			}
		}

		if (t) {
			err = 0;
			if (cmd == SIOCCHGTUNNEL) {
				t->parms.iph.ttl = p.iph.ttl;
				t->parms.iph.tos = p.iph.tos;
				t->parms.iph.frag_off = p.iph.frag_off;
				if (t->parms.link != p.link) {
					t->parms.link = p.link;
					dev->mtu = ipgre_tunnel_bind_dev(dev);
					netdev_state_change(dev);
				}
			}
			if (copy_to_user(ifr->ifr_ifru.ifru_data, &t->parms, sizeof(p)))
				err = -EFAULT;
		} else
			err = (cmd == SIOCADDTUNNEL ? -ENOBUFS : -ENOENT);
		break;

	case SIOCDELTUNNEL:
		err = -EPERM;
		if (!ns_capable(net->user_ns, CAP_NET_ADMIN))
			goto done;

		if (dev == ign->fb_tunnel_dev) {
			err = -EFAULT;
			if (copy_from_user(&p, ifr->ifr_ifru.ifru_data, sizeof(p)))
				goto done;
			err = -ENOENT;
			if ((t = ipgre_tunnel_locate(net, &p, 0)) == NULL)
				goto done;
			err = -EPERM;
			if (t == netdev_priv(ign->fb_tunnel_dev))
				goto done;
			dev = t->dev;
		}
		unregister_netdevice(dev);
		err = 0;
		break;

	default:
		err = -EINVAL;
	}

done:
	return err;
}

static int ipgre_tunnel_change_mtu(struct net_device *dev, int new_mtu)
{
	struct ip_tunnel *tunnel = netdev_priv(dev);
	if (new_mtu < 68 ||
	    new_mtu > 0xFFF8 - dev->hard_header_len - tunnel->hlen)
		return -EINVAL;
	dev->mtu = new_mtu;
=======
static void __gre_xmit(struct sk_buff *skb, struct net_device *dev,
		       const struct iphdr *tnl_params,
		       __be16 proto)
{
	struct ip_tunnel *tunnel = netdev_priv(dev);
	__be16 flags = tunnel->parms.o_flags;

	/* Push GRE header. */
	gre_build_header(skb, tunnel->tun_hlen,
			 flags, proto, tunnel->parms.o_key,
			 (flags & TUNNEL_SEQ) ? htonl(atomic_fetch_inc(&tunnel->o_seqno)) : 0);

	ip_tunnel_xmit(skb, dev, tnl_params, tnl_params->protocol);
}

static int gre_handle_offloads(struct sk_buff *skb, bool csum)
{
	return iptunnel_handle_offloads(skb, csum ? SKB_GSO_GRE_CSUM : SKB_GSO_GRE);
}

static void gre_fb_xmit(struct sk_buff *skb, struct net_device *dev,
			__be16 proto)
{
	struct ip_tunnel *tunnel = netdev_priv(dev);
	struct ip_tunnel_info *tun_info;
	const struct ip_tunnel_key *key;
	int tunnel_hlen;
	__be16 flags;

	tun_info = skb_tunnel_info(skb);
	if (unlikely(!tun_info || !(tun_info->mode & IP_TUNNEL_INFO_TX) ||
		     ip_tunnel_info_af(tun_info) != AF_INET))
		goto err_free_skb;

	key = &tun_info->key;
	tunnel_hlen = gre_calc_hlen(key->tun_flags);

	if (skb_cow_head(skb, dev->needed_headroom))
		goto err_free_skb;

	/* Push Tunnel header. */
	if (gre_handle_offloads(skb, !!(tun_info->key.tun_flags & TUNNEL_CSUM)))
		goto err_free_skb;

	flags = tun_info->key.tun_flags &
		(TUNNEL_CSUM | TUNNEL_KEY | TUNNEL_SEQ);
	gre_build_header(skb, tunnel_hlen, flags, proto,
			 tunnel_id_to_key32(tun_info->key.tun_id),
			 (flags & TUNNEL_SEQ) ? htonl(atomic_fetch_inc(&tunnel->o_seqno)) : 0);

	ip_md_tunnel_xmit(skb, dev, IPPROTO_GRE, tunnel_hlen);

	return;

err_free_skb:
	kfree_skb(skb);
	DEV_STATS_INC(dev, tx_dropped);
}

static void erspan_fb_xmit(struct sk_buff *skb, struct net_device *dev)
{
	struct ip_tunnel *tunnel = netdev_priv(dev);
	struct ip_tunnel_info *tun_info;
	const struct ip_tunnel_key *key;
	struct erspan_metadata *md;
	bool truncate = false;
	__be16 proto;
	int tunnel_hlen;
	int version;
	int nhoff;

	tun_info = skb_tunnel_info(skb);
	if (unlikely(!tun_info || !(tun_info->mode & IP_TUNNEL_INFO_TX) ||
		     ip_tunnel_info_af(tun_info) != AF_INET))
		goto err_free_skb;

	key = &tun_info->key;
	if (!(tun_info->key.tun_flags & TUNNEL_ERSPAN_OPT))
		goto err_free_skb;
	if (tun_info->options_len < sizeof(*md))
		goto err_free_skb;
	md = ip_tunnel_info_opts(tun_info);

	/* ERSPAN has fixed 8 byte GRE header */
	version = md->version;
	tunnel_hlen = 8 + erspan_hdr_len(version);

	if (skb_cow_head(skb, dev->needed_headroom))
		goto err_free_skb;

	if (gre_handle_offloads(skb, false))
		goto err_free_skb;

	if (skb->len > dev->mtu + dev->hard_header_len) {
		if (pskb_trim(skb, dev->mtu + dev->hard_header_len))
			goto err_free_skb;
		truncate = true;
	}

	nhoff = skb_network_offset(skb);
	if (skb->protocol == htons(ETH_P_IP) &&
	    (ntohs(ip_hdr(skb)->tot_len) > skb->len - nhoff))
		truncate = true;

	if (skb->protocol == htons(ETH_P_IPV6)) {
		int thoff;

		if (skb_transport_header_was_set(skb))
			thoff = skb_transport_offset(skb);
		else
			thoff = nhoff + sizeof(struct ipv6hdr);
		if (ntohs(ipv6_hdr(skb)->payload_len) > skb->len - thoff)
			truncate = true;
	}

	if (version == 1) {
		erspan_build_header(skb, ntohl(tunnel_id_to_key32(key->tun_id)),
				    ntohl(md->u.index), truncate, true);
		proto = htons(ETH_P_ERSPAN);
	} else if (version == 2) {
		erspan_build_header_v2(skb,
				       ntohl(tunnel_id_to_key32(key->tun_id)),
				       md->u.md2.dir,
				       get_hwid(&md->u.md2),
				       truncate, true);
		proto = htons(ETH_P_ERSPAN2);
	} else {
		goto err_free_skb;
	}

	gre_build_header(skb, 8, TUNNEL_SEQ,
			 proto, 0, htonl(atomic_fetch_inc(&tunnel->o_seqno)));

	ip_md_tunnel_xmit(skb, dev, IPPROTO_GRE, tunnel_hlen);

	return;

err_free_skb:
	kfree_skb(skb);
	DEV_STATS_INC(dev, tx_dropped);
}

static int gre_fill_metadata_dst(struct net_device *dev, struct sk_buff *skb)
{
	struct ip_tunnel_info *info = skb_tunnel_info(skb);
	const struct ip_tunnel_key *key;
	struct rtable *rt;
	struct flowi4 fl4;

	if (ip_tunnel_info_af(info) != AF_INET)
		return -EINVAL;

	key = &info->key;
	ip_tunnel_init_flow(&fl4, IPPROTO_GRE, key->u.ipv4.dst, key->u.ipv4.src,
			    tunnel_id_to_key32(key->tun_id),
			    key->tos & ~INET_ECN_MASK, dev_net(dev), 0,
			    skb->mark, skb_get_hash(skb), key->flow_flags);
	rt = ip_route_output_key(dev_net(dev), &fl4);
	if (IS_ERR(rt))
		return PTR_ERR(rt);

	ip_rt_put(rt);
	info->key.u.ipv4.src = fl4.saddr;
	return 0;
}

static netdev_tx_t ipgre_xmit(struct sk_buff *skb,
			      struct net_device *dev)
{
	struct ip_tunnel *tunnel = netdev_priv(dev);
	const struct iphdr *tnl_params;

	if (!pskb_inet_may_pull(skb))
		goto free_skb;

	if (tunnel->collect_md) {
		gre_fb_xmit(skb, dev, skb->protocol);
		return NETDEV_TX_OK;
	}

	if (dev->header_ops) {
		int pull_len = tunnel->hlen + sizeof(struct iphdr);

		if (skb_cow_head(skb, 0))
			goto free_skb;

		tnl_params = (const struct iphdr *)skb->data;

		if (!pskb_network_may_pull(skb, pull_len))
			goto free_skb;

		/* ip_tunnel_xmit() needs skb->data pointing to gre header. */
		skb_pull(skb, pull_len);
		skb_reset_mac_header(skb);

		if (skb->ip_summed == CHECKSUM_PARTIAL &&
		    skb_checksum_start(skb) < skb->data)
			goto free_skb;
	} else {
		if (skb_cow_head(skb, dev->needed_headroom))
			goto free_skb;

		tnl_params = &tunnel->parms.iph;
	}

	if (gre_handle_offloads(skb, !!(tunnel->parms.o_flags & TUNNEL_CSUM)))
		goto free_skb;

	__gre_xmit(skb, dev, tnl_params, skb->protocol);
	return NETDEV_TX_OK;

free_skb:
	kfree_skb(skb);
	DEV_STATS_INC(dev, tx_dropped);
	return NETDEV_TX_OK;
}

static netdev_tx_t erspan_xmit(struct sk_buff *skb,
			       struct net_device *dev)
{
	struct ip_tunnel *tunnel = netdev_priv(dev);
	bool truncate = false;
	__be16 proto;

	if (!pskb_inet_may_pull(skb))
		goto free_skb;

	if (tunnel->collect_md) {
		erspan_fb_xmit(skb, dev);
		return NETDEV_TX_OK;
	}

	if (gre_handle_offloads(skb, false))
		goto free_skb;

	if (skb_cow_head(skb, dev->needed_headroom))
		goto free_skb;

	if (skb->len > dev->mtu + dev->hard_header_len) {
		if (pskb_trim(skb, dev->mtu + dev->hard_header_len))
			goto free_skb;
		truncate = true;
	}

	/* Push ERSPAN header */
	if (tunnel->erspan_ver == 0) {
		proto = htons(ETH_P_ERSPAN);
		tunnel->parms.o_flags &= ~TUNNEL_SEQ;
	} else if (tunnel->erspan_ver == 1) {
		erspan_build_header(skb, ntohl(tunnel->parms.o_key),
				    tunnel->index,
				    truncate, true);
		proto = htons(ETH_P_ERSPAN);
	} else if (tunnel->erspan_ver == 2) {
		erspan_build_header_v2(skb, ntohl(tunnel->parms.o_key),
				       tunnel->dir, tunnel->hwid,
				       truncate, true);
		proto = htons(ETH_P_ERSPAN2);
	} else {
		goto free_skb;
	}

	tunnel->parms.o_flags &= ~TUNNEL_KEY;
	__gre_xmit(skb, dev, &tunnel->parms.iph, proto);
	return NETDEV_TX_OK;

free_skb:
	kfree_skb(skb);
	DEV_STATS_INC(dev, tx_dropped);
	return NETDEV_TX_OK;
}

static netdev_tx_t gre_tap_xmit(struct sk_buff *skb,
				struct net_device *dev)
{
	struct ip_tunnel *tunnel = netdev_priv(dev);

	if (!pskb_inet_may_pull(skb))
		goto free_skb;

	if (tunnel->collect_md) {
		gre_fb_xmit(skb, dev, htons(ETH_P_TEB));
		return NETDEV_TX_OK;
	}

	if (gre_handle_offloads(skb, !!(tunnel->parms.o_flags & TUNNEL_CSUM)))
		goto free_skb;

	if (skb_cow_head(skb, dev->needed_headroom))
		goto free_skb;

	__gre_xmit(skb, dev, &tunnel->parms.iph, htons(ETH_P_TEB));
	return NETDEV_TX_OK;

free_skb:
	kfree_skb(skb);
	DEV_STATS_INC(dev, tx_dropped);
	return NETDEV_TX_OK;
}

static void ipgre_link_update(struct net_device *dev, bool set_mtu)
{
	struct ip_tunnel *tunnel = netdev_priv(dev);
	__be16 flags;
	int len;

	len = tunnel->tun_hlen;
	tunnel->tun_hlen = gre_calc_hlen(tunnel->parms.o_flags);
	len = tunnel->tun_hlen - len;
	tunnel->hlen = tunnel->hlen + len;

	if (dev->header_ops)
		dev->hard_header_len += len;
	else
		dev->needed_headroom += len;

	if (set_mtu)
		dev->mtu = max_t(int, dev->mtu - len, 68);

	flags = tunnel->parms.o_flags;

	if (flags & TUNNEL_SEQ ||
	    (flags & TUNNEL_CSUM && tunnel->encap.type != TUNNEL_ENCAP_NONE)) {
		dev->features &= ~NETIF_F_GSO_SOFTWARE;
		dev->hw_features &= ~NETIF_F_GSO_SOFTWARE;
	} else {
		dev->features |= NETIF_F_GSO_SOFTWARE;
		dev->hw_features |= NETIF_F_GSO_SOFTWARE;
	}
}

static int ipgre_tunnel_ctl(struct net_device *dev, struct ip_tunnel_parm *p,
			    int cmd)
{
	int err;

	if (cmd == SIOCADDTUNNEL || cmd == SIOCCHGTUNNEL) {
		if (p->iph.version != 4 || p->iph.protocol != IPPROTO_GRE ||
		    p->iph.ihl != 5 || (p->iph.frag_off & htons(~IP_DF)) ||
		    ((p->i_flags | p->o_flags) & (GRE_VERSION | GRE_ROUTING)))
			return -EINVAL;
	}

	p->i_flags = gre_flags_to_tnl_flags(p->i_flags);
	p->o_flags = gre_flags_to_tnl_flags(p->o_flags);

	err = ip_tunnel_ctl(dev, p, cmd);
	if (err)
		return err;

	if (cmd == SIOCCHGTUNNEL) {
		struct ip_tunnel *t = netdev_priv(dev);

		t->parms.i_flags = p->i_flags;
		t->parms.o_flags = p->o_flags;

		if (strcmp(dev->rtnl_link_ops->kind, "erspan"))
			ipgre_link_update(dev, true);
	}

	p->i_flags = gre_tnl_flags_to_gre_flags(p->i_flags);
	p->o_flags = gre_tnl_flags_to_gre_flags(p->o_flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

/* Nice toy. Unfortunately, useless in real life :-)
   It allows to construct virtual multiprotocol broadcast "LAN"
   over the Internet, provided multicast routing is tuned.


   I have no idea was this bicycle invented before me,
   so that I had to set ARPHRD_IPGRE to a random value.
   I have an impression, that Cisco could make something similar,
   but this feature is apparently missing in IOS<=11.2(8).

   I set up 10.66.66/24 and fec0:6666:6666::0/96 as virtual networks
   with broadcast 224.66.66.66. If you have access to mbone, play with me :-)

   ping -t 255 224.66.66.66

   If nobody answers, mbone does not work.

   ip tunnel add Universe mode gre remote 224.66.66.66 local <Your_real_addr> ttl 255
   ip addr add 10.66.66.<somewhat>/24 dev Universe
   ifconfig Universe up
   ifconfig Universe add fe80::<Your_real_addr>/10
   ifconfig Universe add fec0:6666:6666::<Your_real_addr>/96
   ftp 10.66.66.66
   ...
   ftp fec0:6666:6666::193.233.7.65
   ...
<<<<<<< HEAD

 */

=======
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int ipgre_header(struct sk_buff *skb, struct net_device *dev,
			unsigned short type,
			const void *daddr, const void *saddr, unsigned int len)
{
	struct ip_tunnel *t = netdev_priv(dev);
<<<<<<< HEAD
	struct iphdr *iph = (struct iphdr *)skb_push(skb, t->hlen);
	__be16 *p = (__be16*)(iph+1);

	memcpy(iph, &t->parms.iph, sizeof(struct iphdr));
	p[0]		= t->parms.o_flags;
	p[1]		= htons(type);

	/*
	 *	Set the source hardware address.
	 */

=======
	struct iphdr *iph;
	struct gre_base_hdr *greh;

	iph = skb_push(skb, t->hlen + sizeof(*iph));
	greh = (struct gre_base_hdr *)(iph+1);
	greh->flags = gre_tnl_flags_to_gre_flags(t->parms.o_flags);
	greh->protocol = htons(type);

	memcpy(iph, &t->parms.iph, sizeof(struct iphdr));

	/* Set the source hardware address. */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (saddr)
		memcpy(&iph->saddr, saddr, 4);
	if (daddr)
		memcpy(&iph->daddr, daddr, 4);
	if (iph->daddr)
<<<<<<< HEAD
		return t->hlen;

	return -t->hlen;
=======
		return t->hlen + sizeof(*iph);

	return -(t->hlen + sizeof(*iph));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int ipgre_header_parse(const struct sk_buff *skb, unsigned char *haddr)
{
	const struct iphdr *iph = (const struct iphdr *) skb_mac_header(skb);
	memcpy(haddr, &iph->saddr, 4);
	return 4;
}

static const struct header_ops ipgre_header_ops = {
	.create	= ipgre_header,
	.parse	= ipgre_header_parse,
};

#ifdef CONFIG_NET_IPGRE_BROADCAST
static int ipgre_open(struct net_device *dev)
{
	struct ip_tunnel *t = netdev_priv(dev);

	if (ipv4_is_multicast(t->parms.iph.daddr)) {
		struct flowi4 fl4;
		struct rtable *rt;

<<<<<<< HEAD
		rt = ip_route_output_gre(dev_net(dev), &fl4,
=======
		rt = ip_route_output_gre(t->net, &fl4,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					 t->parms.iph.daddr,
					 t->parms.iph.saddr,
					 t->parms.o_key,
					 RT_TOS(t->parms.iph.tos),
					 t->parms.link);
		if (IS_ERR(rt))
			return -EADDRNOTAVAIL;
		dev = rt->dst.dev;
		ip_rt_put(rt);
<<<<<<< HEAD
		if (__in_dev_get_rtnl(dev) == NULL)
=======
		if (!__in_dev_get_rtnl(dev))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -EADDRNOTAVAIL;
		t->mlink = dev->ifindex;
		ip_mc_inc_group(__in_dev_get_rtnl(dev), t->parms.iph.daddr);
	}
	return 0;
}

static int ipgre_close(struct net_device *dev)
{
	struct ip_tunnel *t = netdev_priv(dev);

	if (ipv4_is_multicast(t->parms.iph.daddr) && t->mlink) {
		struct in_device *in_dev;
<<<<<<< HEAD
		in_dev = inetdev_by_index(dev_net(dev), t->mlink);
=======
		in_dev = inetdev_by_index(t->net, t->mlink);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (in_dev)
			ip_mc_dec_group(in_dev, t->parms.iph.daddr);
	}
	return 0;
}
<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

static const struct net_device_ops ipgre_netdev_ops = {
	.ndo_init		= ipgre_tunnel_init,
<<<<<<< HEAD
	.ndo_uninit		= ipgre_tunnel_uninit,
=======
	.ndo_uninit		= ip_tunnel_uninit,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_NET_IPGRE_BROADCAST
	.ndo_open		= ipgre_open,
	.ndo_stop		= ipgre_close,
#endif
<<<<<<< HEAD
	.ndo_start_xmit		= ipgre_tunnel_xmit,
	.ndo_do_ioctl		= ipgre_tunnel_ioctl,
	.ndo_change_mtu		= ipgre_tunnel_change_mtu,
	.ndo_get_stats64	= ipgre_get_stats64,
};

static void ipgre_dev_free(struct net_device *dev)
{
	free_percpu(dev->tstats);
	free_netdev(dev);
}
=======
	.ndo_start_xmit		= ipgre_xmit,
	.ndo_siocdevprivate	= ip_tunnel_siocdevprivate,
	.ndo_change_mtu		= ip_tunnel_change_mtu,
	.ndo_get_stats64	= dev_get_tstats64,
	.ndo_get_iflink		= ip_tunnel_get_iflink,
	.ndo_tunnel_ctl		= ipgre_tunnel_ctl,
};

#define GRE_FEATURES (NETIF_F_SG |		\
		      NETIF_F_FRAGLIST |	\
		      NETIF_F_HIGHDMA |		\
		      NETIF_F_HW_CSUM)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static void ipgre_tunnel_setup(struct net_device *dev)
{
	dev->netdev_ops		= &ipgre_netdev_ops;
<<<<<<< HEAD
	dev->destructor 	= ipgre_dev_free;

	dev->type		= ARPHRD_IPGRE;
	dev->needed_headroom 	= LL_MAX_HEADER + sizeof(struct iphdr) + 4;
	dev->mtu		= ETH_DATA_LEN - sizeof(struct iphdr) - 4;
	dev->flags		= IFF_NOARP;
	dev->iflink		= 0;
	dev->addr_len		= 4;
	dev->features		|= NETIF_F_NETNS_LOCAL;
	dev->priv_flags		&= ~IFF_XMIT_DST_RELEASE;
=======
	dev->type		= ARPHRD_IPGRE;
	ip_tunnel_setup(dev, ipgre_net_id);
}

static void __gre_tunnel_init(struct net_device *dev)
{
	struct ip_tunnel *tunnel;
	__be16 flags;

	tunnel = netdev_priv(dev);
	tunnel->tun_hlen = gre_calc_hlen(tunnel->parms.o_flags);
	tunnel->parms.iph.protocol = IPPROTO_GRE;

	tunnel->hlen = tunnel->tun_hlen + tunnel->encap_hlen;
	dev->needed_headroom = tunnel->hlen + sizeof(tunnel->parms.iph);

	dev->features		|= GRE_FEATURES | NETIF_F_LLTX;
	dev->hw_features	|= GRE_FEATURES;

	flags = tunnel->parms.o_flags;

	/* TCP offload with GRE SEQ is not supported, nor can we support 2
	 * levels of outer headers requiring an update.
	 */
	if (flags & TUNNEL_SEQ)
		return;
	if (flags & TUNNEL_CSUM && tunnel->encap.type != TUNNEL_ENCAP_NONE)
		return;

	dev->features |= NETIF_F_GSO_SOFTWARE;
	dev->hw_features |= NETIF_F_GSO_SOFTWARE;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int ipgre_tunnel_init(struct net_device *dev)
{
<<<<<<< HEAD
	struct ip_tunnel *tunnel;
	struct iphdr *iph;

	tunnel = netdev_priv(dev);
	iph = &tunnel->parms.iph;

	tunnel->dev = dev;
	strcpy(tunnel->parms.name, dev->name);

	memcpy(dev->dev_addr, &tunnel->parms.iph.saddr, 4);
	memcpy(dev->broadcast, &tunnel->parms.iph.daddr, 4);

	if (iph->daddr) {
=======
	struct ip_tunnel *tunnel = netdev_priv(dev);
	struct iphdr *iph = &tunnel->parms.iph;

	__gre_tunnel_init(dev);

	__dev_addr_set(dev, &iph->saddr, 4);
	memcpy(dev->broadcast, &iph->daddr, 4);

	dev->flags		= IFF_NOARP;
	netif_keep_dst(dev);
	dev->addr_len		= 4;

	if (iph->daddr && !tunnel->collect_md) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_NET_IPGRE_BROADCAST
		if (ipv4_is_multicast(iph->daddr)) {
			if (!iph->saddr)
				return -EINVAL;
			dev->flags = IFF_BROADCAST;
			dev->header_ops = &ipgre_header_ops;
<<<<<<< HEAD
		}
#endif
	} else
		dev->header_ops = &ipgre_header_ops;

	dev->tstats = alloc_percpu(struct pcpu_tstats);
	if (!dev->tstats)
		return -ENOMEM;

	return 0;
}

static void ipgre_fb_tunnel_init(struct net_device *dev)
{
	struct ip_tunnel *tunnel = netdev_priv(dev);
	struct iphdr *iph = &tunnel->parms.iph;

	tunnel->dev = dev;
	strcpy(tunnel->parms.name, dev->name);

	iph->version		= 4;
	iph->protocol		= IPPROTO_GRE;
	iph->ihl		= 5;
	tunnel->hlen		= sizeof(struct iphdr) + 4;

	dev_hold(dev);
}


static const struct gre_protocol ipgre_protocol = {
	.handler     = ipgre_rcv,
	.err_handler = ipgre_err,
};

static void ipgre_destroy_tunnels(struct ipgre_net *ign, struct list_head *head)
{
	int prio;

	for (prio = 0; prio < 4; prio++) {
		int h;
		for (h = 0; h < HASH_SIZE; h++) {
			struct ip_tunnel *t;

			t = rtnl_dereference(ign->tunnels[prio][h]);

			while (t != NULL) {
				unregister_netdevice_queue(t->dev, head);
				t = rtnl_dereference(t->next);
			}
		}
	}
}

static int __net_init ipgre_init_net(struct net *net)
{
	struct ipgre_net *ign = net_generic(net, ipgre_net_id);
	int err;

	ign->fb_tunnel_dev = alloc_netdev(sizeof(struct ip_tunnel), "gre0",
					   ipgre_tunnel_setup);
	if (!ign->fb_tunnel_dev) {
		err = -ENOMEM;
		goto err_alloc_dev;
	}
	dev_net_set(ign->fb_tunnel_dev, net);

	ipgre_fb_tunnel_init(ign->fb_tunnel_dev);
	ign->fb_tunnel_dev->rtnl_link_ops = &ipgre_link_ops;

	if ((err = register_netdev(ign->fb_tunnel_dev)))
		goto err_reg_dev;

	rcu_assign_pointer(ign->tunnels_wc[0],
			   netdev_priv(ign->fb_tunnel_dev));
	return 0;

err_reg_dev:
	ipgre_dev_free(ign->fb_tunnel_dev);
err_alloc_dev:
	return err;
}

static void __net_exit ipgre_exit_net(struct net *net)
{
	struct ipgre_net *ign;
	LIST_HEAD(list);

	ign = net_generic(net, ipgre_net_id);
	rtnl_lock();
	ipgre_destroy_tunnels(ign, &list);
	unregister_netdevice_many(&list);
	rtnl_unlock();
=======
			dev->hard_header_len = tunnel->hlen + sizeof(*iph);
			dev->needed_headroom = 0;
		}
#endif
	} else if (!tunnel->collect_md) {
		dev->header_ops = &ipgre_header_ops;
		dev->hard_header_len = tunnel->hlen + sizeof(*iph);
		dev->needed_headroom = 0;
	}

	return ip_tunnel_init(dev);
}

static const struct gre_protocol ipgre_protocol = {
	.handler     = gre_rcv,
	.err_handler = gre_err,
};

static int __net_init ipgre_init_net(struct net *net)
{
	return ip_tunnel_init_net(net, ipgre_net_id, &ipgre_link_ops, NULL);
}

static void __net_exit ipgre_exit_batch_rtnl(struct list_head *list_net,
					     struct list_head *dev_to_kill)
{
	ip_tunnel_delete_nets(list_net, ipgre_net_id, &ipgre_link_ops,
			      dev_to_kill);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct pernet_operations ipgre_net_ops = {
	.init = ipgre_init_net,
<<<<<<< HEAD
	.exit = ipgre_exit_net,
	.id   = &ipgre_net_id,
	.size = sizeof(struct ipgre_net),
};

static int ipgre_tunnel_validate(struct nlattr *tb[], struct nlattr *data[])
=======
	.exit_batch_rtnl = ipgre_exit_batch_rtnl,
	.id   = &ipgre_net_id,
	.size = sizeof(struct ip_tunnel_net),
};

static int ipgre_tunnel_validate(struct nlattr *tb[], struct nlattr *data[],
				 struct netlink_ext_ack *extack)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	__be16 flags;

	if (!data)
		return 0;

	flags = 0;
	if (data[IFLA_GRE_IFLAGS])
		flags |= nla_get_be16(data[IFLA_GRE_IFLAGS]);
	if (data[IFLA_GRE_OFLAGS])
		flags |= nla_get_be16(data[IFLA_GRE_OFLAGS]);
	if (flags & (GRE_VERSION|GRE_ROUTING))
		return -EINVAL;

<<<<<<< HEAD
	return 0;
}

static int ipgre_tap_validate(struct nlattr *tb[], struct nlattr *data[])
=======
	if (data[IFLA_GRE_COLLECT_METADATA] &&
	    data[IFLA_GRE_ENCAP_TYPE] &&
	    nla_get_u16(data[IFLA_GRE_ENCAP_TYPE]) != TUNNEL_ENCAP_NONE)
		return -EINVAL;

	return 0;
}

static int ipgre_tap_validate(struct nlattr *tb[], struct nlattr *data[],
			      struct netlink_ext_ack *extack)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	__be32 daddr;

	if (tb[IFLA_ADDRESS]) {
		if (nla_len(tb[IFLA_ADDRESS]) != ETH_ALEN)
			return -EINVAL;
		if (!is_valid_ether_addr(nla_data(tb[IFLA_ADDRESS])))
			return -EADDRNOTAVAIL;
	}

	if (!data)
		goto out;

	if (data[IFLA_GRE_REMOTE]) {
		memcpy(&daddr, nla_data(data[IFLA_GRE_REMOTE]), 4);
		if (!daddr)
			return -EINVAL;
	}

out:
<<<<<<< HEAD
	return ipgre_tunnel_validate(tb, data);
}

static void ipgre_netlink_parms(struct nlattr *data[],
				struct ip_tunnel_parm *parms)
{
=======
	return ipgre_tunnel_validate(tb, data, extack);
}

static int erspan_validate(struct nlattr *tb[], struct nlattr *data[],
			   struct netlink_ext_ack *extack)
{
	__be16 flags = 0;
	int ret;

	if (!data)
		return 0;

	ret = ipgre_tap_validate(tb, data, extack);
	if (ret)
		return ret;

	if (data[IFLA_GRE_ERSPAN_VER] &&
	    nla_get_u8(data[IFLA_GRE_ERSPAN_VER]) == 0)
		return 0;

	/* ERSPAN type II/III should only have GRE sequence and key flag */
	if (data[IFLA_GRE_OFLAGS])
		flags |= nla_get_be16(data[IFLA_GRE_OFLAGS]);
	if (data[IFLA_GRE_IFLAGS])
		flags |= nla_get_be16(data[IFLA_GRE_IFLAGS]);
	if (!data[IFLA_GRE_COLLECT_METADATA] &&
	    flags != (GRE_SEQ | GRE_KEY))
		return -EINVAL;

	/* ERSPAN Session ID only has 10-bit. Since we reuse
	 * 32-bit key field as ID, check it's range.
	 */
	if (data[IFLA_GRE_IKEY] &&
	    (ntohl(nla_get_be32(data[IFLA_GRE_IKEY])) & ~ID_MASK))
		return -EINVAL;

	if (data[IFLA_GRE_OKEY] &&
	    (ntohl(nla_get_be32(data[IFLA_GRE_OKEY])) & ~ID_MASK))
		return -EINVAL;

	return 0;
}

static int ipgre_netlink_parms(struct net_device *dev,
				struct nlattr *data[],
				struct nlattr *tb[],
				struct ip_tunnel_parm *parms,
				__u32 *fwmark)
{
	struct ip_tunnel *t = netdev_priv(dev);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	memset(parms, 0, sizeof(*parms));

	parms->iph.protocol = IPPROTO_GRE;

	if (!data)
<<<<<<< HEAD
		return;
=======
		return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (data[IFLA_GRE_LINK])
		parms->link = nla_get_u32(data[IFLA_GRE_LINK]);

	if (data[IFLA_GRE_IFLAGS])
<<<<<<< HEAD
		parms->i_flags = nla_get_be16(data[IFLA_GRE_IFLAGS]);

	if (data[IFLA_GRE_OFLAGS])
		parms->o_flags = nla_get_be16(data[IFLA_GRE_OFLAGS]);
=======
		parms->i_flags = gre_flags_to_tnl_flags(nla_get_be16(data[IFLA_GRE_IFLAGS]));

	if (data[IFLA_GRE_OFLAGS])
		parms->o_flags = gre_flags_to_tnl_flags(nla_get_be16(data[IFLA_GRE_OFLAGS]));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (data[IFLA_GRE_IKEY])
		parms->i_key = nla_get_be32(data[IFLA_GRE_IKEY]);

	if (data[IFLA_GRE_OKEY])
		parms->o_key = nla_get_be32(data[IFLA_GRE_OKEY]);

	if (data[IFLA_GRE_LOCAL])
<<<<<<< HEAD
		parms->iph.saddr = nla_get_be32(data[IFLA_GRE_LOCAL]);

	if (data[IFLA_GRE_REMOTE])
		parms->iph.daddr = nla_get_be32(data[IFLA_GRE_REMOTE]);
=======
		parms->iph.saddr = nla_get_in_addr(data[IFLA_GRE_LOCAL]);

	if (data[IFLA_GRE_REMOTE])
		parms->iph.daddr = nla_get_in_addr(data[IFLA_GRE_REMOTE]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (data[IFLA_GRE_TTL])
		parms->iph.ttl = nla_get_u8(data[IFLA_GRE_TTL]);

	if (data[IFLA_GRE_TOS])
		parms->iph.tos = nla_get_u8(data[IFLA_GRE_TOS]);

<<<<<<< HEAD
	if (!data[IFLA_GRE_PMTUDISC] || nla_get_u8(data[IFLA_GRE_PMTUDISC]))
		parms->iph.frag_off = htons(IP_DF);
}

static int ipgre_tap_init(struct net_device *dev)
{
	struct ip_tunnel *tunnel;

	tunnel = netdev_priv(dev);

	tunnel->dev = dev;
	strcpy(tunnel->parms.name, dev->name);

	ipgre_tunnel_bind_dev(dev);

	dev->tstats = alloc_percpu(struct pcpu_tstats);
	if (!dev->tstats)
		return -ENOMEM;
=======
	if (!data[IFLA_GRE_PMTUDISC] || nla_get_u8(data[IFLA_GRE_PMTUDISC])) {
		if (t->ignore_df)
			return -EINVAL;
		parms->iph.frag_off = htons(IP_DF);
	}

	if (data[IFLA_GRE_COLLECT_METADATA]) {
		t->collect_md = true;
		if (dev->type == ARPHRD_IPGRE)
			dev->type = ARPHRD_NONE;
	}

	if (data[IFLA_GRE_IGNORE_DF]) {
		if (nla_get_u8(data[IFLA_GRE_IGNORE_DF])
		  && (parms->iph.frag_off & htons(IP_DF)))
			return -EINVAL;
		t->ignore_df = !!nla_get_u8(data[IFLA_GRE_IGNORE_DF]);
	}

	if (data[IFLA_GRE_FWMARK])
		*fwmark = nla_get_u32(data[IFLA_GRE_FWMARK]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static const struct net_device_ops ipgre_tap_netdev_ops = {
	.ndo_init		= ipgre_tap_init,
	.ndo_uninit		= ipgre_tunnel_uninit,
	.ndo_start_xmit		= ipgre_tunnel_xmit,
	.ndo_set_mac_address 	= eth_mac_addr,
	.ndo_validate_addr	= eth_validate_addr,
	.ndo_change_mtu		= ipgre_tunnel_change_mtu,
	.ndo_get_stats64	= ipgre_get_stats64,
=======
static int erspan_netlink_parms(struct net_device *dev,
				struct nlattr *data[],
				struct nlattr *tb[],
				struct ip_tunnel_parm *parms,
				__u32 *fwmark)
{
	struct ip_tunnel *t = netdev_priv(dev);
	int err;

	err = ipgre_netlink_parms(dev, data, tb, parms, fwmark);
	if (err)
		return err;
	if (!data)
		return 0;

	if (data[IFLA_GRE_ERSPAN_VER]) {
		t->erspan_ver = nla_get_u8(data[IFLA_GRE_ERSPAN_VER]);

		if (t->erspan_ver > 2)
			return -EINVAL;
	}

	if (t->erspan_ver == 1) {
		if (data[IFLA_GRE_ERSPAN_INDEX]) {
			t->index = nla_get_u32(data[IFLA_GRE_ERSPAN_INDEX]);
			if (t->index & ~INDEX_MASK)
				return -EINVAL;
		}
	} else if (t->erspan_ver == 2) {
		if (data[IFLA_GRE_ERSPAN_DIR]) {
			t->dir = nla_get_u8(data[IFLA_GRE_ERSPAN_DIR]);
			if (t->dir & ~(DIR_MASK >> DIR_OFFSET))
				return -EINVAL;
		}
		if (data[IFLA_GRE_ERSPAN_HWID]) {
			t->hwid = nla_get_u16(data[IFLA_GRE_ERSPAN_HWID]);
			if (t->hwid & ~(HWID_MASK >> HWID_OFFSET))
				return -EINVAL;
		}
	}

	return 0;
}

/* This function returns true when ENCAP attributes are present in the nl msg */
static bool ipgre_netlink_encap_parms(struct nlattr *data[],
				      struct ip_tunnel_encap *ipencap)
{
	bool ret = false;

	memset(ipencap, 0, sizeof(*ipencap));

	if (!data)
		return ret;

	if (data[IFLA_GRE_ENCAP_TYPE]) {
		ret = true;
		ipencap->type = nla_get_u16(data[IFLA_GRE_ENCAP_TYPE]);
	}

	if (data[IFLA_GRE_ENCAP_FLAGS]) {
		ret = true;
		ipencap->flags = nla_get_u16(data[IFLA_GRE_ENCAP_FLAGS]);
	}

	if (data[IFLA_GRE_ENCAP_SPORT]) {
		ret = true;
		ipencap->sport = nla_get_be16(data[IFLA_GRE_ENCAP_SPORT]);
	}

	if (data[IFLA_GRE_ENCAP_DPORT]) {
		ret = true;
		ipencap->dport = nla_get_be16(data[IFLA_GRE_ENCAP_DPORT]);
	}

	return ret;
}

static int gre_tap_init(struct net_device *dev)
{
	__gre_tunnel_init(dev);
	dev->priv_flags |= IFF_LIVE_ADDR_CHANGE;
	netif_keep_dst(dev);

	return ip_tunnel_init(dev);
}

static const struct net_device_ops gre_tap_netdev_ops = {
	.ndo_init		= gre_tap_init,
	.ndo_uninit		= ip_tunnel_uninit,
	.ndo_start_xmit		= gre_tap_xmit,
	.ndo_set_mac_address 	= eth_mac_addr,
	.ndo_validate_addr	= eth_validate_addr,
	.ndo_change_mtu		= ip_tunnel_change_mtu,
	.ndo_get_stats64	= dev_get_tstats64,
	.ndo_get_iflink		= ip_tunnel_get_iflink,
	.ndo_fill_metadata_dst	= gre_fill_metadata_dst,
};

static int erspan_tunnel_init(struct net_device *dev)
{
	struct ip_tunnel *tunnel = netdev_priv(dev);

	if (tunnel->erspan_ver == 0)
		tunnel->tun_hlen = 4; /* 4-byte GRE hdr. */
	else
		tunnel->tun_hlen = 8; /* 8-byte GRE hdr. */

	tunnel->parms.iph.protocol = IPPROTO_GRE;
	tunnel->hlen = tunnel->tun_hlen + tunnel->encap_hlen +
		       erspan_hdr_len(tunnel->erspan_ver);

	dev->features		|= GRE_FEATURES;
	dev->hw_features	|= GRE_FEATURES;
	dev->priv_flags		|= IFF_LIVE_ADDR_CHANGE;
	netif_keep_dst(dev);

	return ip_tunnel_init(dev);
}

static const struct net_device_ops erspan_netdev_ops = {
	.ndo_init		= erspan_tunnel_init,
	.ndo_uninit		= ip_tunnel_uninit,
	.ndo_start_xmit		= erspan_xmit,
	.ndo_set_mac_address	= eth_mac_addr,
	.ndo_validate_addr	= eth_validate_addr,
	.ndo_change_mtu		= ip_tunnel_change_mtu,
	.ndo_get_stats64	= dev_get_tstats64,
	.ndo_get_iflink		= ip_tunnel_get_iflink,
	.ndo_fill_metadata_dst	= gre_fill_metadata_dst,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static void ipgre_tap_setup(struct net_device *dev)
{
<<<<<<< HEAD

	ether_setup(dev);

	dev->netdev_ops		= &ipgre_tap_netdev_ops;
	dev->destructor 	= ipgre_dev_free;

	dev->iflink		= 0;
	dev->features		|= NETIF_F_NETNS_LOCAL;
}

static int ipgre_newlink(struct net *src_net, struct net_device *dev, struct nlattr *tb[],
			 struct nlattr *data[])
{
	struct ip_tunnel *nt;
	struct net *net = dev_net(dev);
	struct ipgre_net *ign = net_generic(net, ipgre_net_id);
	int mtu;
	int err;

	nt = netdev_priv(dev);
	ipgre_netlink_parms(data, &nt->parms);

	if (ipgre_tunnel_find(net, &nt->parms, dev->type))
		return -EEXIST;

	if (dev->type == ARPHRD_ETHER && !tb[IFLA_ADDRESS])
		eth_hw_addr_random(dev);

	mtu = ipgre_tunnel_bind_dev(dev);
	if (!tb[IFLA_MTU])
		dev->mtu = mtu;

	/* Can use a lockless transmit, unless we generate output sequences */
	if (!(nt->parms.o_flags & GRE_SEQ))
		dev->features |= NETIF_F_LLTX;

	err = register_netdevice(dev);
	if (err)
		goto out;

	dev_hold(dev);
	ipgre_tunnel_link(ign, nt);

out:
	return err;
}

static int ipgre_changelink(struct net_device *dev, struct nlattr *tb[],
			    struct nlattr *data[])
{
	struct ip_tunnel *t, *nt;
	struct net *net = dev_net(dev);
	struct ipgre_net *ign = net_generic(net, ipgre_net_id);
	struct ip_tunnel_parm p;
	int mtu;

	if (dev == ign->fb_tunnel_dev)
		return -EINVAL;

	nt = netdev_priv(dev);
	ipgre_netlink_parms(data, &p);

	t = ipgre_tunnel_locate(net, &p, 0);

	if (t) {
		if (t->dev != dev)
			return -EEXIST;
	} else {
		t = nt;

		if (dev->type != ARPHRD_ETHER) {
			unsigned int nflags = 0;

			if (ipv4_is_multicast(p.iph.daddr))
				nflags = IFF_BROADCAST;
			else if (p.iph.daddr)
				nflags = IFF_POINTOPOINT;

			if ((dev->flags ^ nflags) &
			    (IFF_POINTOPOINT | IFF_BROADCAST))
				return -EINVAL;
		}

		ipgre_tunnel_unlink(ign, t);
		t->parms.iph.saddr = p.iph.saddr;
		t->parms.iph.daddr = p.iph.daddr;
		t->parms.i_key = p.i_key;
		if (dev->type != ARPHRD_ETHER) {
			memcpy(dev->dev_addr, &p.iph.saddr, 4);
			memcpy(dev->broadcast, &p.iph.daddr, 4);
		}
		ipgre_tunnel_link(ign, t);
		netdev_state_change(dev);
	}

	t->parms.o_key = p.o_key;
	t->parms.iph.ttl = p.iph.ttl;
	t->parms.iph.tos = p.iph.tos;
	t->parms.iph.frag_off = p.iph.frag_off;

	if (t->parms.link != p.link) {
		t->parms.link = p.link;
		mtu = ipgre_tunnel_bind_dev(dev);
		if (!tb[IFLA_MTU])
			dev->mtu = mtu;
		netdev_state_change(dev);
	}
=======
	ether_setup(dev);
	dev->max_mtu = 0;
	dev->netdev_ops	= &gre_tap_netdev_ops;
	dev->priv_flags &= ~IFF_TX_SKB_SHARING;
	dev->priv_flags	|= IFF_LIVE_ADDR_CHANGE;
	ip_tunnel_setup(dev, gre_tap_net_id);
}

static int
ipgre_newlink_encap_setup(struct net_device *dev, struct nlattr *data[])
{
	struct ip_tunnel_encap ipencap;

	if (ipgre_netlink_encap_parms(data, &ipencap)) {
		struct ip_tunnel *t = netdev_priv(dev);
		int err = ip_tunnel_encap_setup(t, &ipencap);

		if (err < 0)
			return err;
	}

	return 0;
}

static int ipgre_newlink(struct net *src_net, struct net_device *dev,
			 struct nlattr *tb[], struct nlattr *data[],
			 struct netlink_ext_ack *extack)
{
	struct ip_tunnel_parm p;
	__u32 fwmark = 0;
	int err;

	err = ipgre_newlink_encap_setup(dev, data);
	if (err)
		return err;

	err = ipgre_netlink_parms(dev, data, tb, &p, &fwmark);
	if (err < 0)
		return err;
	return ip_tunnel_newlink(dev, tb, &p, fwmark);
}

static int erspan_newlink(struct net *src_net, struct net_device *dev,
			  struct nlattr *tb[], struct nlattr *data[],
			  struct netlink_ext_ack *extack)
{
	struct ip_tunnel_parm p;
	__u32 fwmark = 0;
	int err;

	err = ipgre_newlink_encap_setup(dev, data);
	if (err)
		return err;

	err = erspan_netlink_parms(dev, data, tb, &p, &fwmark);
	if (err)
		return err;
	return ip_tunnel_newlink(dev, tb, &p, fwmark);
}

static int ipgre_changelink(struct net_device *dev, struct nlattr *tb[],
			    struct nlattr *data[],
			    struct netlink_ext_ack *extack)
{
	struct ip_tunnel *t = netdev_priv(dev);
	__u32 fwmark = t->fwmark;
	struct ip_tunnel_parm p;
	int err;

	err = ipgre_newlink_encap_setup(dev, data);
	if (err)
		return err;

	err = ipgre_netlink_parms(dev, data, tb, &p, &fwmark);
	if (err < 0)
		return err;

	err = ip_tunnel_changelink(dev, tb, &p, fwmark);
	if (err < 0)
		return err;

	t->parms.i_flags = p.i_flags;
	t->parms.o_flags = p.o_flags;

	ipgre_link_update(dev, !tb[IFLA_MTU]);

	return 0;
}

static int erspan_changelink(struct net_device *dev, struct nlattr *tb[],
			     struct nlattr *data[],
			     struct netlink_ext_ack *extack)
{
	struct ip_tunnel *t = netdev_priv(dev);
	__u32 fwmark = t->fwmark;
	struct ip_tunnel_parm p;
	int err;

	err = ipgre_newlink_encap_setup(dev, data);
	if (err)
		return err;

	err = erspan_netlink_parms(dev, data, tb, &p, &fwmark);
	if (err < 0)
		return err;

	err = ip_tunnel_changelink(dev, tb, &p, fwmark);
	if (err < 0)
		return err;

	t->parms.i_flags = p.i_flags;
	t->parms.o_flags = p.o_flags;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static size_t ipgre_get_size(const struct net_device *dev)
{
	return
		/* IFLA_GRE_LINK */
		nla_total_size(4) +
		/* IFLA_GRE_IFLAGS */
		nla_total_size(2) +
		/* IFLA_GRE_OFLAGS */
		nla_total_size(2) +
		/* IFLA_GRE_IKEY */
		nla_total_size(4) +
		/* IFLA_GRE_OKEY */
		nla_total_size(4) +
		/* IFLA_GRE_LOCAL */
		nla_total_size(4) +
		/* IFLA_GRE_REMOTE */
		nla_total_size(4) +
		/* IFLA_GRE_TTL */
		nla_total_size(1) +
		/* IFLA_GRE_TOS */
		nla_total_size(1) +
		/* IFLA_GRE_PMTUDISC */
		nla_total_size(1) +
<<<<<<< HEAD
=======
		/* IFLA_GRE_ENCAP_TYPE */
		nla_total_size(2) +
		/* IFLA_GRE_ENCAP_FLAGS */
		nla_total_size(2) +
		/* IFLA_GRE_ENCAP_SPORT */
		nla_total_size(2) +
		/* IFLA_GRE_ENCAP_DPORT */
		nla_total_size(2) +
		/* IFLA_GRE_COLLECT_METADATA */
		nla_total_size(0) +
		/* IFLA_GRE_IGNORE_DF */
		nla_total_size(1) +
		/* IFLA_GRE_FWMARK */
		nla_total_size(4) +
		/* IFLA_GRE_ERSPAN_INDEX */
		nla_total_size(4) +
		/* IFLA_GRE_ERSPAN_VER */
		nla_total_size(1) +
		/* IFLA_GRE_ERSPAN_DIR */
		nla_total_size(1) +
		/* IFLA_GRE_ERSPAN_HWID */
		nla_total_size(2) +
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		0;
}

static int ipgre_fill_info(struct sk_buff *skb, const struct net_device *dev)
{
	struct ip_tunnel *t = netdev_priv(dev);
	struct ip_tunnel_parm *p = &t->parms;
<<<<<<< HEAD

	NLA_PUT_U32(skb, IFLA_GRE_LINK, p->link);
	NLA_PUT_BE16(skb, IFLA_GRE_IFLAGS, p->i_flags);
	NLA_PUT_BE16(skb, IFLA_GRE_OFLAGS, p->o_flags);
	NLA_PUT_BE32(skb, IFLA_GRE_IKEY, p->i_key);
	NLA_PUT_BE32(skb, IFLA_GRE_OKEY, p->o_key);
	NLA_PUT_BE32(skb, IFLA_GRE_LOCAL, p->iph.saddr);
	NLA_PUT_BE32(skb, IFLA_GRE_REMOTE, p->iph.daddr);
	NLA_PUT_U8(skb, IFLA_GRE_TTL, p->iph.ttl);
	NLA_PUT_U8(skb, IFLA_GRE_TOS, p->iph.tos);
	NLA_PUT_U8(skb, IFLA_GRE_PMTUDISC, !!(p->iph.frag_off & htons(IP_DF)));
=======
	__be16 o_flags = p->o_flags;

	if (nla_put_u32(skb, IFLA_GRE_LINK, p->link) ||
	    nla_put_be16(skb, IFLA_GRE_IFLAGS,
			 gre_tnl_flags_to_gre_flags(p->i_flags)) ||
	    nla_put_be16(skb, IFLA_GRE_OFLAGS,
			 gre_tnl_flags_to_gre_flags(o_flags)) ||
	    nla_put_be32(skb, IFLA_GRE_IKEY, p->i_key) ||
	    nla_put_be32(skb, IFLA_GRE_OKEY, p->o_key) ||
	    nla_put_in_addr(skb, IFLA_GRE_LOCAL, p->iph.saddr) ||
	    nla_put_in_addr(skb, IFLA_GRE_REMOTE, p->iph.daddr) ||
	    nla_put_u8(skb, IFLA_GRE_TTL, p->iph.ttl) ||
	    nla_put_u8(skb, IFLA_GRE_TOS, p->iph.tos) ||
	    nla_put_u8(skb, IFLA_GRE_PMTUDISC,
		       !!(p->iph.frag_off & htons(IP_DF))) ||
	    nla_put_u32(skb, IFLA_GRE_FWMARK, t->fwmark))
		goto nla_put_failure;

	if (nla_put_u16(skb, IFLA_GRE_ENCAP_TYPE,
			t->encap.type) ||
	    nla_put_be16(skb, IFLA_GRE_ENCAP_SPORT,
			 t->encap.sport) ||
	    nla_put_be16(skb, IFLA_GRE_ENCAP_DPORT,
			 t->encap.dport) ||
	    nla_put_u16(skb, IFLA_GRE_ENCAP_FLAGS,
			t->encap.flags))
		goto nla_put_failure;

	if (nla_put_u8(skb, IFLA_GRE_IGNORE_DF, t->ignore_df))
		goto nla_put_failure;

	if (t->collect_md) {
		if (nla_put_flag(skb, IFLA_GRE_COLLECT_METADATA))
			goto nla_put_failure;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;

nla_put_failure:
	return -EMSGSIZE;
}

<<<<<<< HEAD
=======
static int erspan_fill_info(struct sk_buff *skb, const struct net_device *dev)
{
	struct ip_tunnel *t = netdev_priv(dev);

	if (t->erspan_ver <= 2) {
		if (t->erspan_ver != 0 && !t->collect_md)
			t->parms.o_flags |= TUNNEL_KEY;

		if (nla_put_u8(skb, IFLA_GRE_ERSPAN_VER, t->erspan_ver))
			goto nla_put_failure;

		if (t->erspan_ver == 1) {
			if (nla_put_u32(skb, IFLA_GRE_ERSPAN_INDEX, t->index))
				goto nla_put_failure;
		} else if (t->erspan_ver == 2) {
			if (nla_put_u8(skb, IFLA_GRE_ERSPAN_DIR, t->dir))
				goto nla_put_failure;
			if (nla_put_u16(skb, IFLA_GRE_ERSPAN_HWID, t->hwid))
				goto nla_put_failure;
		}
	}

	return ipgre_fill_info(skb, dev);

nla_put_failure:
	return -EMSGSIZE;
}

static void erspan_setup(struct net_device *dev)
{
	struct ip_tunnel *t = netdev_priv(dev);

	ether_setup(dev);
	dev->max_mtu = 0;
	dev->netdev_ops = &erspan_netdev_ops;
	dev->priv_flags &= ~IFF_TX_SKB_SHARING;
	dev->priv_flags |= IFF_LIVE_ADDR_CHANGE;
	ip_tunnel_setup(dev, erspan_net_id);
	t->erspan_ver = 1;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static const struct nla_policy ipgre_policy[IFLA_GRE_MAX + 1] = {
	[IFLA_GRE_LINK]		= { .type = NLA_U32 },
	[IFLA_GRE_IFLAGS]	= { .type = NLA_U16 },
	[IFLA_GRE_OFLAGS]	= { .type = NLA_U16 },
	[IFLA_GRE_IKEY]		= { .type = NLA_U32 },
	[IFLA_GRE_OKEY]		= { .type = NLA_U32 },
<<<<<<< HEAD
	[IFLA_GRE_LOCAL]	= { .len = FIELD_SIZEOF(struct iphdr, saddr) },
	[IFLA_GRE_REMOTE]	= { .len = FIELD_SIZEOF(struct iphdr, daddr) },
	[IFLA_GRE_TTL]		= { .type = NLA_U8 },
	[IFLA_GRE_TOS]		= { .type = NLA_U8 },
	[IFLA_GRE_PMTUDISC]	= { .type = NLA_U8 },
=======
	[IFLA_GRE_LOCAL]	= { .len = sizeof_field(struct iphdr, saddr) },
	[IFLA_GRE_REMOTE]	= { .len = sizeof_field(struct iphdr, daddr) },
	[IFLA_GRE_TTL]		= { .type = NLA_U8 },
	[IFLA_GRE_TOS]		= { .type = NLA_U8 },
	[IFLA_GRE_PMTUDISC]	= { .type = NLA_U8 },
	[IFLA_GRE_ENCAP_TYPE]	= { .type = NLA_U16 },
	[IFLA_GRE_ENCAP_FLAGS]	= { .type = NLA_U16 },
	[IFLA_GRE_ENCAP_SPORT]	= { .type = NLA_U16 },
	[IFLA_GRE_ENCAP_DPORT]	= { .type = NLA_U16 },
	[IFLA_GRE_COLLECT_METADATA]	= { .type = NLA_FLAG },
	[IFLA_GRE_IGNORE_DF]	= { .type = NLA_U8 },
	[IFLA_GRE_FWMARK]	= { .type = NLA_U32 },
	[IFLA_GRE_ERSPAN_INDEX]	= { .type = NLA_U32 },
	[IFLA_GRE_ERSPAN_VER]	= { .type = NLA_U8 },
	[IFLA_GRE_ERSPAN_DIR]	= { .type = NLA_U8 },
	[IFLA_GRE_ERSPAN_HWID]	= { .type = NLA_U16 },
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static struct rtnl_link_ops ipgre_link_ops __read_mostly = {
	.kind		= "gre",
	.maxtype	= IFLA_GRE_MAX,
	.policy		= ipgre_policy,
	.priv_size	= sizeof(struct ip_tunnel),
	.setup		= ipgre_tunnel_setup,
	.validate	= ipgre_tunnel_validate,
	.newlink	= ipgre_newlink,
	.changelink	= ipgre_changelink,
<<<<<<< HEAD
	.get_size	= ipgre_get_size,
	.fill_info	= ipgre_fill_info,
=======
	.dellink	= ip_tunnel_dellink,
	.get_size	= ipgre_get_size,
	.fill_info	= ipgre_fill_info,
	.get_link_net	= ip_tunnel_get_link_net,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static struct rtnl_link_ops ipgre_tap_ops __read_mostly = {
	.kind		= "gretap",
	.maxtype	= IFLA_GRE_MAX,
	.policy		= ipgre_policy,
	.priv_size	= sizeof(struct ip_tunnel),
	.setup		= ipgre_tap_setup,
	.validate	= ipgre_tap_validate,
	.newlink	= ipgre_newlink,
	.changelink	= ipgre_changelink,
<<<<<<< HEAD
	.get_size	= ipgre_get_size,
	.fill_info	= ipgre_fill_info,
};

/*
 *	And now the modules code and kernel interface.
 */
=======
	.dellink	= ip_tunnel_dellink,
	.get_size	= ipgre_get_size,
	.fill_info	= ipgre_fill_info,
	.get_link_net	= ip_tunnel_get_link_net,
};

static struct rtnl_link_ops erspan_link_ops __read_mostly = {
	.kind		= "erspan",
	.maxtype	= IFLA_GRE_MAX,
	.policy		= ipgre_policy,
	.priv_size	= sizeof(struct ip_tunnel),
	.setup		= erspan_setup,
	.validate	= erspan_validate,
	.newlink	= erspan_newlink,
	.changelink	= erspan_changelink,
	.dellink	= ip_tunnel_dellink,
	.get_size	= ipgre_get_size,
	.fill_info	= erspan_fill_info,
	.get_link_net	= ip_tunnel_get_link_net,
};

struct net_device *gretap_fb_dev_create(struct net *net, const char *name,
					u8 name_assign_type)
{
	struct nlattr *tb[IFLA_MAX + 1];
	struct net_device *dev;
	LIST_HEAD(list_kill);
	struct ip_tunnel *t;
	int err;

	memset(&tb, 0, sizeof(tb));

	dev = rtnl_create_link(net, name, name_assign_type,
			       &ipgre_tap_ops, tb, NULL);
	if (IS_ERR(dev))
		return dev;

	/* Configure flow based GRE device. */
	t = netdev_priv(dev);
	t->collect_md = true;

	err = ipgre_newlink(net, dev, tb, NULL, NULL);
	if (err < 0) {
		free_netdev(dev);
		return ERR_PTR(err);
	}

	/* openvswitch users expect packet sizes to be unrestricted,
	 * so set the largest MTU we can.
	 */
	err = __ip_tunnel_change_mtu(dev, IP_MAX_MTU, false);
	if (err)
		goto out;

	err = rtnl_configure_link(dev, NULL, 0, NULL);
	if (err < 0)
		goto out;

	return dev;
out:
	ip_tunnel_dellink(dev, &list_kill);
	unregister_netdevice_many(&list_kill);
	return ERR_PTR(err);
}
EXPORT_SYMBOL_GPL(gretap_fb_dev_create);

static int __net_init ipgre_tap_init_net(struct net *net)
{
	return ip_tunnel_init_net(net, gre_tap_net_id, &ipgre_tap_ops, "gretap0");
}

static void __net_exit ipgre_tap_exit_batch_rtnl(struct list_head *list_net,
						 struct list_head *dev_to_kill)
{
	ip_tunnel_delete_nets(list_net, gre_tap_net_id, &ipgre_tap_ops,
			      dev_to_kill);
}

static struct pernet_operations ipgre_tap_net_ops = {
	.init = ipgre_tap_init_net,
	.exit_batch_rtnl = ipgre_tap_exit_batch_rtnl,
	.id   = &gre_tap_net_id,
	.size = sizeof(struct ip_tunnel_net),
};

static int __net_init erspan_init_net(struct net *net)
{
	return ip_tunnel_init_net(net, erspan_net_id,
				  &erspan_link_ops, "erspan0");
}

static void __net_exit erspan_exit_batch_rtnl(struct list_head *net_list,
					      struct list_head *dev_to_kill)
{
	ip_tunnel_delete_nets(net_list, erspan_net_id, &erspan_link_ops,
			      dev_to_kill);
}

static struct pernet_operations erspan_net_ops = {
	.init = erspan_init_net,
	.exit_batch_rtnl = erspan_exit_batch_rtnl,
	.id   = &erspan_net_id,
	.size = sizeof(struct ip_tunnel_net),
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int __init ipgre_init(void)
{
	int err;

	pr_info("GRE over IPv4 tunneling driver\n");

	err = register_pernet_device(&ipgre_net_ops);
	if (err < 0)
		return err;

<<<<<<< HEAD
=======
	err = register_pernet_device(&ipgre_tap_net_ops);
	if (err < 0)
		goto pnet_tap_failed;

	err = register_pernet_device(&erspan_net_ops);
	if (err < 0)
		goto pnet_erspan_failed;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	err = gre_add_protocol(&ipgre_protocol, GREPROTO_CISCO);
	if (err < 0) {
		pr_info("%s: can't add protocol\n", __func__);
		goto add_proto_failed;
	}

	err = rtnl_link_register(&ipgre_link_ops);
	if (err < 0)
		goto rtnl_link_failed;

	err = rtnl_link_register(&ipgre_tap_ops);
	if (err < 0)
		goto tap_ops_failed;

<<<<<<< HEAD
out:
	return err;

=======
	err = rtnl_link_register(&erspan_link_ops);
	if (err < 0)
		goto erspan_link_failed;

	return 0;

erspan_link_failed:
	rtnl_link_unregister(&ipgre_tap_ops);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
tap_ops_failed:
	rtnl_link_unregister(&ipgre_link_ops);
rtnl_link_failed:
	gre_del_protocol(&ipgre_protocol, GREPROTO_CISCO);
add_proto_failed:
<<<<<<< HEAD
	unregister_pernet_device(&ipgre_net_ops);
	goto out;
=======
	unregister_pernet_device(&erspan_net_ops);
pnet_erspan_failed:
	unregister_pernet_device(&ipgre_tap_net_ops);
pnet_tap_failed:
	unregister_pernet_device(&ipgre_net_ops);
	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void __exit ipgre_fini(void)
{
	rtnl_link_unregister(&ipgre_tap_ops);
	rtnl_link_unregister(&ipgre_link_ops);
<<<<<<< HEAD
	if (gre_del_protocol(&ipgre_protocol, GREPROTO_CISCO) < 0)
		pr_info("%s: can't remove protocol\n", __func__);
	unregister_pernet_device(&ipgre_net_ops);
=======
	rtnl_link_unregister(&erspan_link_ops);
	gre_del_protocol(&ipgre_protocol, GREPROTO_CISCO);
	unregister_pernet_device(&ipgre_tap_net_ops);
	unregister_pernet_device(&ipgre_net_ops);
	unregister_pernet_device(&erspan_net_ops);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

module_init(ipgre_init);
module_exit(ipgre_fini);
<<<<<<< HEAD
MODULE_LICENSE("GPL");
MODULE_ALIAS_RTNL_LINK("gre");
MODULE_ALIAS_RTNL_LINK("gretap");
MODULE_ALIAS_NETDEV("gre0");
=======
MODULE_DESCRIPTION("IPv4 GRE tunnels over IP library");
MODULE_LICENSE("GPL");
MODULE_ALIAS_RTNL_LINK("gre");
MODULE_ALIAS_RTNL_LINK("gretap");
MODULE_ALIAS_RTNL_LINK("erspan");
MODULE_ALIAS_NETDEV("gre0");
MODULE_ALIAS_NETDEV("gretap0");
MODULE_ALIAS_NETDEV("erspan0");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
