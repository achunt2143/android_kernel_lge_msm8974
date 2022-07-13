<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * ip_vs_xmit.c: various packet transmitters for IPVS
 *
 * Authors:     Wensong Zhang <wensong@linuxvirtualserver.org>
 *              Julian Anastasov <ja@ssi.bg>
 *
<<<<<<< HEAD
 *              This program is free software; you can redistribute it and/or
 *              modify it under the terms of the GNU General Public License
 *              as published by the Free Software Foundation; either version
 *              2 of the License, or (at your option) any later version.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Changes:
 *
 * Description of forwarding methods:
 * - all transmitters are called from LOCAL_IN (remote clients) and
 * LOCAL_OUT (local clients) but for ICMP can be called from FORWARD
 * - not all connections have destination server, for example,
 * connections in backup server when fwmark is used
 * - bypass connections use daddr from packet
<<<<<<< HEAD
=======
 * - we can use dst without ref while sending in RCU section, we use
 * ref when returning NF_ACCEPT for NAT-ed packet via loopback
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * LOCAL_OUT rules:
 * - skb->dev is NULL, skb->protocol is not set (both are set in POST_ROUTING)
 * - skb->pkt_type is not set yet
 * - the only place where we can see skb->sk != NULL
 */

#define KMSG_COMPONENT "IPVS"
#define pr_fmt(fmt) KMSG_COMPONENT ": " fmt

#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/tcp.h>                  /* for tcphdr */
#include <net/ip.h>
<<<<<<< HEAD
=======
#include <net/gue.h>
#include <net/gre.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <net/tcp.h>                    /* for csum_tcpudp_magic */
#include <net/udp.h>
#include <net/icmp.h>                   /* for icmp_send */
#include <net/route.h>                  /* for ip_route_output */
#include <net/ipv6.h>
#include <net/ip6_route.h>
<<<<<<< HEAD
=======
#include <net/ip_tunnels.h>
#include <net/ip6_checksum.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <net/addrconf.h>
#include <linux/icmpv6.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>

#include <net/ip_vs.h>

enum {
	IP_VS_RT_MODE_LOCAL	= 1, /* Allow local dest */
	IP_VS_RT_MODE_NON_LOCAL	= 2, /* Allow non-local dest */
	IP_VS_RT_MODE_RDR	= 4, /* Allow redirect from remote daddr to
				      * local
				      */
<<<<<<< HEAD
};

=======
	IP_VS_RT_MODE_CONNECT	= 8, /* Always bind route to saddr */
	IP_VS_RT_MODE_KNOWN_NH	= 16,/* Route via remote addr */
	IP_VS_RT_MODE_TUNNEL	= 32,/* Tunnel mode */
};

static inline struct ip_vs_dest_dst *ip_vs_dest_dst_alloc(void)
{
	return kmalloc(sizeof(struct ip_vs_dest_dst), GFP_ATOMIC);
}

static inline void ip_vs_dest_dst_free(struct ip_vs_dest_dst *dest_dst)
{
	kfree(dest_dst);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *      Destination cache to speed up outgoing route lookup
 */
static inline void
<<<<<<< HEAD
__ip_vs_dst_set(struct ip_vs_dest *dest, u32 rtos, struct dst_entry *dst,
		u32 dst_cookie)
{
	struct dst_entry *old_dst;

	old_dst = dest->dst_cache;
	dest->dst_cache = dst;
	dest->dst_rtos = rtos;
	dest->dst_cookie = dst_cookie;
	dst_release(old_dst);
}

static inline struct dst_entry *
__ip_vs_dst_check(struct ip_vs_dest *dest, u32 rtos)
{
	struct dst_entry *dst = dest->dst_cache;

	if (!dst)
		return NULL;
	if ((dst->obsolete || rtos != dest->dst_rtos) &&
	    dst->ops->check(dst, dest->dst_cookie) == NULL) {
		dest->dst_cache = NULL;
		dst_release(dst);
		return NULL;
	}
	dst_hold(dst);
	return dst;
}

/* Get route to destination or remote server */
static struct rtable *
__ip_vs_get_out_rt(struct sk_buff *skb, struct ip_vs_dest *dest,
		   __be32 daddr, u32 rtos, int rt_mode, __be32 *ret_saddr)
{
	struct net *net = dev_net(skb_dst(skb)->dev);
	struct rtable *rt;			/* Route to the other host */
	struct rtable *ort;			/* Original route */
	int local;

	if (dest) {
		spin_lock(&dest->dst_lock);
		if (!(rt = (struct rtable *)
		      __ip_vs_dst_check(dest, rtos))) {
			struct flowi4 fl4;

			memset(&fl4, 0, sizeof(fl4));
			fl4.daddr = dest->addr.ip;
			fl4.flowi4_tos = rtos;
			rt = ip_route_output_key(net, &fl4);
			if (IS_ERR(rt)) {
				spin_unlock(&dest->dst_lock);
				IP_VS_DBG_RL("ip_route_output error, dest: %pI4\n",
					     &dest->addr.ip);
				return NULL;
			}
			__ip_vs_dst_set(dest, rtos, dst_clone(&rt->dst), 0);
			dest->dst_saddr.ip = fl4.saddr;
			IP_VS_DBG(10, "new dst %pI4, src %pI4, refcnt=%d, "
				  "rtos=%X\n",
				  &dest->addr.ip, &dest->dst_saddr.ip,
				  atomic_read(&rt->dst.__refcnt), rtos);
		}
		daddr = dest->addr.ip;
		if (ret_saddr)
			*ret_saddr = dest->dst_saddr.ip;
		spin_unlock(&dest->dst_lock);
	} else {
		struct flowi4 fl4;

		memset(&fl4, 0, sizeof(fl4));
		fl4.daddr = daddr;
		fl4.flowi4_tos = rtos;
		rt = ip_route_output_key(net, &fl4);
		if (IS_ERR(rt)) {
			IP_VS_DBG_RL("ip_route_output error, dest: %pI4\n",
				     &daddr);
			return NULL;
		}
		if (ret_saddr)
			*ret_saddr = fl4.saddr;
	}

	local = rt->rt_flags & RTCF_LOCAL;
	if (!((local ? IP_VS_RT_MODE_LOCAL : IP_VS_RT_MODE_NON_LOCAL) &
	      rt_mode)) {
		IP_VS_DBG_RL("Stopping traffic to %s address, dest: %pI4\n",
			     (rt->rt_flags & RTCF_LOCAL) ?
			     "local":"non-local", &daddr);
		ip_rt_put(rt);
		return NULL;
	}
	if (local && !(rt_mode & IP_VS_RT_MODE_RDR) &&
	    !((ort = skb_rtable(skb)) && ort->rt_flags & RTCF_LOCAL)) {
		IP_VS_DBG_RL("Redirect from non-local address %pI4 to local "
			     "requires NAT method, dest: %pI4\n",
			     &ip_hdr(skb)->daddr, &daddr);
		ip_rt_put(rt);
		return NULL;
	}
	if (unlikely(!local && ipv4_is_loopback(ip_hdr(skb)->saddr))) {
		IP_VS_DBG_RL("Stopping traffic from loopback address %pI4 "
			     "to non-local address, dest: %pI4\n",
			     &ip_hdr(skb)->saddr, &daddr);
		ip_rt_put(rt);
		return NULL;
	}

	return rt;
}

/* Reroute packet to local IPv4 stack after DNAT */
static int
__ip_vs_reroute_locally(struct sk_buff *skb)
{
	struct rtable *rt = skb_rtable(skb);
	struct net_device *dev = rt->dst.dev;
	struct net *net = dev_net(dev);
	struct iphdr *iph = ip_hdr(skb);

	if (rt_is_input_route(rt)) {
		unsigned long orefdst = skb->_skb_refdst;

		if (ip_route_input(skb, iph->daddr, iph->saddr,
				   iph->tos, skb->dev))
			return 0;
		refdst_drop(orefdst);
	} else {
		struct flowi4 fl4 = {
			.daddr = iph->daddr,
			.saddr = iph->saddr,
			.flowi4_tos = RT_TOS(iph->tos),
			.flowi4_mark = skb->mark,
		};

		rt = ip_route_output_key(net, &fl4);
		if (IS_ERR(rt))
			return 0;
		if (!(rt->rt_flags & RTCF_LOCAL)) {
			ip_rt_put(rt);
			return 0;
		}
		/* Drop old route. */
		skb_dst_drop(skb);
		skb_dst_set(skb, &rt->dst);
	}
	return 1;
}

#ifdef CONFIG_IP_VS_IPV6

=======
__ip_vs_dst_set(struct ip_vs_dest *dest, struct ip_vs_dest_dst *dest_dst,
		struct dst_entry *dst, u32 dst_cookie)
{
	struct ip_vs_dest_dst *old;

	old = rcu_dereference_protected(dest->dest_dst,
					lockdep_is_held(&dest->dst_lock));

	if (dest_dst) {
		dest_dst->dst_cache = dst;
		dest_dst->dst_cookie = dst_cookie;
	}
	rcu_assign_pointer(dest->dest_dst, dest_dst);

	if (old)
		call_rcu(&old->rcu_head, ip_vs_dest_dst_rcu_free);
}

static inline struct ip_vs_dest_dst *
__ip_vs_dst_check(struct ip_vs_dest *dest)
{
	struct ip_vs_dest_dst *dest_dst = rcu_dereference(dest->dest_dst);
	struct dst_entry *dst;

	if (!dest_dst)
		return NULL;
	dst = dest_dst->dst_cache;
	if (dst->obsolete &&
	    dst->ops->check(dst, dest_dst->dst_cookie) == NULL)
		return NULL;
	return dest_dst;
}

static inline bool
__mtu_check_toobig_v6(const struct sk_buff *skb, u32 mtu)
{
	if (IP6CB(skb)->frag_max_size) {
		/* frag_max_size tell us that, this packet have been
		 * defragmented by netfilter IPv6 conntrack module.
		 */
		if (IP6CB(skb)->frag_max_size > mtu)
			return true; /* largest fragment violate MTU */
	}
	else if (skb->len > mtu && !skb_is_gso(skb)) {
		return true; /* Packet size violate MTU size */
	}
	return false;
}

/* Get route to daddr, update *saddr, optionally bind route to saddr */
static struct rtable *do_output_route4(struct net *net, __be32 daddr,
				       int rt_mode, __be32 *saddr)
{
	struct flowi4 fl4;
	struct rtable *rt;
	bool loop = false;

	memset(&fl4, 0, sizeof(fl4));
	fl4.daddr = daddr;
	fl4.flowi4_flags = (rt_mode & IP_VS_RT_MODE_KNOWN_NH) ?
			   FLOWI_FLAG_KNOWN_NH : 0;

retry:
	rt = ip_route_output_key(net, &fl4);
	if (IS_ERR(rt)) {
		/* Invalid saddr ? */
		if (PTR_ERR(rt) == -EINVAL && *saddr &&
		    rt_mode & IP_VS_RT_MODE_CONNECT && !loop) {
			*saddr = 0;
			flowi4_update_output(&fl4, 0, daddr, 0);
			goto retry;
		}
		IP_VS_DBG_RL("ip_route_output error, dest: %pI4\n", &daddr);
		return NULL;
	} else if (!*saddr && rt_mode & IP_VS_RT_MODE_CONNECT && fl4.saddr) {
		ip_rt_put(rt);
		*saddr = fl4.saddr;
		flowi4_update_output(&fl4, 0, daddr, fl4.saddr);
		loop = true;
		goto retry;
	}
	*saddr = fl4.saddr;
	return rt;
}

#ifdef CONFIG_IP_VS_IPV6
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline int __ip_vs_is_local_route6(struct rt6_info *rt)
{
	return rt->dst.dev && rt->dst.dev->flags & IFF_LOOPBACK;
}
<<<<<<< HEAD

static struct dst_entry *
__ip_vs_route_output_v6(struct net *net, struct in6_addr *daddr,
			struct in6_addr *ret_saddr, int do_xfrm)
=======
#endif

static inline bool crosses_local_route_boundary(int skb_af, struct sk_buff *skb,
						int rt_mode,
						bool new_rt_is_local)
{
	bool rt_mode_allow_local = !!(rt_mode & IP_VS_RT_MODE_LOCAL);
	bool rt_mode_allow_non_local = !!(rt_mode & IP_VS_RT_MODE_NON_LOCAL);
	bool rt_mode_allow_redirect = !!(rt_mode & IP_VS_RT_MODE_RDR);
	bool source_is_loopback;
	bool old_rt_is_local;

#ifdef CONFIG_IP_VS_IPV6
	if (skb_af == AF_INET6) {
		int addr_type = ipv6_addr_type(&ipv6_hdr(skb)->saddr);

		source_is_loopback =
			(!skb->dev || skb->dev->flags & IFF_LOOPBACK) &&
			(addr_type & IPV6_ADDR_LOOPBACK);
		old_rt_is_local = __ip_vs_is_local_route6(
			(struct rt6_info *)skb_dst(skb));
	} else
#endif
	{
		source_is_loopback = ipv4_is_loopback(ip_hdr(skb)->saddr);
		old_rt_is_local = skb_rtable(skb)->rt_flags & RTCF_LOCAL;
	}

	if (unlikely(new_rt_is_local)) {
		if (!rt_mode_allow_local)
			return true;
		if (!rt_mode_allow_redirect && !old_rt_is_local)
			return true;
	} else {
		if (!rt_mode_allow_non_local)
			return true;
		if (source_is_loopback)
			return true;
	}
	return false;
}

static inline void maybe_update_pmtu(int skb_af, struct sk_buff *skb, int mtu)
{
	struct sock *sk = skb->sk;
	struct rtable *ort = skb_rtable(skb);

	if (!skb->dev && sk && sk_fullsock(sk))
		ort->dst.ops->update_pmtu(&ort->dst, sk, NULL, mtu, true);
}

static inline bool ensure_mtu_is_adequate(struct netns_ipvs *ipvs, int skb_af,
					  int rt_mode,
					  struct ip_vs_iphdr *ipvsh,
					  struct sk_buff *skb, int mtu)
{
#ifdef CONFIG_IP_VS_IPV6
	if (skb_af == AF_INET6) {
		struct net *net = ipvs->net;

		if (unlikely(__mtu_check_toobig_v6(skb, mtu))) {
			if (!skb->dev)
				skb->dev = net->loopback_dev;
			/* only send ICMP too big on first fragment */
			if (!ipvsh->fragoffs && !ip_vs_iph_icmp(ipvsh))
				icmpv6_send(skb, ICMPV6_PKT_TOOBIG, 0, mtu);
			IP_VS_DBG(1, "frag needed for %pI6c\n",
				  &ipv6_hdr(skb)->saddr);
			return false;
		}
	} else
#endif
	{
		/* If we're going to tunnel the packet and pmtu discovery
		 * is disabled, we'll just fragment it anyway
		 */
		if ((rt_mode & IP_VS_RT_MODE_TUNNEL) && !sysctl_pmtu_disc(ipvs))
			return true;

		if (unlikely(ip_hdr(skb)->frag_off & htons(IP_DF) &&
			     skb->len > mtu && !skb_is_gso(skb) &&
			     !ip_vs_iph_icmp(ipvsh))) {
			icmp_send(skb, ICMP_DEST_UNREACH, ICMP_FRAG_NEEDED,
				  htonl(mtu));
			IP_VS_DBG(1, "frag needed for %pI4\n",
				  &ip_hdr(skb)->saddr);
			return false;
		}
	}

	return true;
}

static inline bool decrement_ttl(struct netns_ipvs *ipvs,
				 int skb_af,
				 struct sk_buff *skb)
{
	struct net *net = ipvs->net;

#ifdef CONFIG_IP_VS_IPV6
	if (skb_af == AF_INET6) {
		struct dst_entry *dst = skb_dst(skb);

		/* check and decrement ttl */
		if (ipv6_hdr(skb)->hop_limit <= 1) {
			struct inet6_dev *idev = __in6_dev_get_safely(skb->dev);

			/* Force OUTPUT device used as source address */
			skb->dev = dst->dev;
			icmpv6_send(skb, ICMPV6_TIME_EXCEED,
				    ICMPV6_EXC_HOPLIMIT, 0);
			IP6_INC_STATS(net, idev, IPSTATS_MIB_INHDRERRORS);

			return false;
		}

		/* don't propagate ttl change to cloned packets */
		if (skb_ensure_writable(skb, sizeof(struct ipv6hdr)))
			return false;

		ipv6_hdr(skb)->hop_limit--;
	} else
#endif
	{
		if (ip_hdr(skb)->ttl <= 1) {
			/* Tell the sender its packet died... */
			IP_INC_STATS(net, IPSTATS_MIB_INHDRERRORS);
			icmp_send(skb, ICMP_TIME_EXCEEDED, ICMP_EXC_TTL, 0);
			return false;
		}

		/* don't propagate ttl change to cloned packets */
		if (skb_ensure_writable(skb, sizeof(struct iphdr)))
			return false;

		/* Decrease ttl */
		ip_decrease_ttl(ip_hdr(skb));
	}

	return true;
}

/* Get route to destination or remote server */
static int
__ip_vs_get_out_rt(struct netns_ipvs *ipvs, int skb_af, struct sk_buff *skb,
		   struct ip_vs_dest *dest,
		   __be32 daddr, int rt_mode, __be32 *ret_saddr,
		   struct ip_vs_iphdr *ipvsh)
{
	struct net *net = ipvs->net;
	struct ip_vs_dest_dst *dest_dst;
	struct rtable *rt;			/* Route to the other host */
	int mtu;
	int local, noref = 1;

	if (dest) {
		dest_dst = __ip_vs_dst_check(dest);
		if (likely(dest_dst))
			rt = (struct rtable *) dest_dst->dst_cache;
		else {
			dest_dst = ip_vs_dest_dst_alloc();
			spin_lock_bh(&dest->dst_lock);
			if (!dest_dst) {
				__ip_vs_dst_set(dest, NULL, NULL, 0);
				spin_unlock_bh(&dest->dst_lock);
				goto err_unreach;
			}
			rt = do_output_route4(net, dest->addr.ip, rt_mode,
					      &dest_dst->dst_saddr.ip);
			if (!rt) {
				__ip_vs_dst_set(dest, NULL, NULL, 0);
				spin_unlock_bh(&dest->dst_lock);
				ip_vs_dest_dst_free(dest_dst);
				goto err_unreach;
			}
			__ip_vs_dst_set(dest, dest_dst, &rt->dst, 0);
			spin_unlock_bh(&dest->dst_lock);
			IP_VS_DBG(10, "new dst %pI4, src %pI4, refcnt=%d\n",
				  &dest->addr.ip, &dest_dst->dst_saddr.ip,
				  rcuref_read(&rt->dst.__rcuref));
		}
		if (ret_saddr)
			*ret_saddr = dest_dst->dst_saddr.ip;
	} else {
		__be32 saddr = htonl(INADDR_ANY);

		noref = 0;

		/* For such unconfigured boxes avoid many route lookups
		 * for performance reasons because we do not remember saddr
		 */
		rt_mode &= ~IP_VS_RT_MODE_CONNECT;
		rt = do_output_route4(net, daddr, rt_mode, &saddr);
		if (!rt)
			goto err_unreach;
		if (ret_saddr)
			*ret_saddr = saddr;
	}

	local = (rt->rt_flags & RTCF_LOCAL) ? 1 : 0;
	if (unlikely(crosses_local_route_boundary(skb_af, skb, rt_mode,
						  local))) {
		IP_VS_DBG_RL("We are crossing local and non-local addresses"
			     " daddr=%pI4\n", &daddr);
		goto err_put;
	}

	if (unlikely(local)) {
		/* skb to local stack, preserve old route */
		if (!noref)
			ip_rt_put(rt);
		return local;
	}

	if (!decrement_ttl(ipvs, skb_af, skb))
		goto err_put;

	if (likely(!(rt_mode & IP_VS_RT_MODE_TUNNEL))) {
		mtu = dst_mtu(&rt->dst);
	} else {
		mtu = dst_mtu(&rt->dst) - sizeof(struct iphdr);
		if (!dest)
			goto err_put;
		if (dest->tun_type == IP_VS_CONN_F_TUNNEL_TYPE_GUE) {
			mtu -= sizeof(struct udphdr) + sizeof(struct guehdr);
			if ((dest->tun_flags &
			     IP_VS_TUNNEL_ENCAP_FLAG_REMCSUM) &&
			    skb->ip_summed == CHECKSUM_PARTIAL)
				mtu -= GUE_PLEN_REMCSUM + GUE_LEN_PRIV;
		} else if (dest->tun_type == IP_VS_CONN_F_TUNNEL_TYPE_GRE) {
			__be16 tflags = 0;

			if (dest->tun_flags & IP_VS_TUNNEL_ENCAP_FLAG_CSUM)
				tflags |= TUNNEL_CSUM;
			mtu -= gre_calc_hlen(tflags);
		}
		if (mtu < 68) {
			IP_VS_DBG_RL("%s(): mtu less than 68\n", __func__);
			goto err_put;
		}
		maybe_update_pmtu(skb_af, skb, mtu);
	}

	if (!ensure_mtu_is_adequate(ipvs, skb_af, rt_mode, ipvsh, skb, mtu))
		goto err_put;

	skb_dst_drop(skb);
	if (noref)
		skb_dst_set_noref(skb, &rt->dst);
	else
		skb_dst_set(skb, &rt->dst);

	return local;

err_put:
	if (!noref)
		ip_rt_put(rt);
	return -1;

err_unreach:
	dst_link_failure(skb);
	return -1;
}

#ifdef CONFIG_IP_VS_IPV6
static struct dst_entry *
__ip_vs_route_output_v6(struct net *net, struct in6_addr *daddr,
			struct in6_addr *ret_saddr, int do_xfrm, int rt_mode)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct dst_entry *dst;
	struct flowi6 fl6 = {
		.daddr = *daddr,
	};

<<<<<<< HEAD
=======
	if (rt_mode & IP_VS_RT_MODE_KNOWN_NH)
		fl6.flowi6_flags = FLOWI_FLAG_KNOWN_NH;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dst = ip6_route_output(net, NULL, &fl6);
	if (dst->error)
		goto out_err;
	if (!ret_saddr)
		return dst;
	if (ipv6_addr_any(&fl6.saddr) &&
	    ipv6_dev_get_saddr(net, ip6_dst_idev(dst)->dev,
			       &fl6.daddr, 0, &fl6.saddr) < 0)
		goto out_err;
	if (do_xfrm) {
		dst = xfrm_lookup(net, dst, flowi6_to_flowi(&fl6), NULL, 0);
		if (IS_ERR(dst)) {
			dst = NULL;
			goto out_err;
		}
	}
	*ret_saddr = fl6.saddr;
	return dst;

out_err:
	dst_release(dst);
	IP_VS_DBG_RL("ip6_route_output error, dest: %pI6\n", daddr);
	return NULL;
}

/*
 * Get route to destination or remote server
 */
<<<<<<< HEAD
static struct rt6_info *
__ip_vs_get_out_rt_v6(struct sk_buff *skb, struct ip_vs_dest *dest,
		      struct in6_addr *daddr, struct in6_addr *ret_saddr,
		      int do_xfrm, int rt_mode)
{
	struct net *net = dev_net(skb_dst(skb)->dev);
	struct rt6_info *rt;			/* Route to the other host */
	struct rt6_info *ort;			/* Original route */
	struct dst_entry *dst;
	int local;

	if (dest) {
		spin_lock(&dest->dst_lock);
		rt = (struct rt6_info *)__ip_vs_dst_check(dest, 0);
		if (!rt) {
			u32 cookie;

			dst = __ip_vs_route_output_v6(net, &dest->addr.in6,
						      &dest->dst_saddr.in6,
						      do_xfrm);
			if (!dst) {
				spin_unlock(&dest->dst_lock);
				return NULL;
			}
			rt = (struct rt6_info *) dst;
			cookie = rt->rt6i_node ? rt->rt6i_node->fn_sernum : 0;
			__ip_vs_dst_set(dest, 0, dst_clone(&rt->dst), cookie);
			IP_VS_DBG(10, "new dst %pI6, src %pI6, refcnt=%d\n",
				  &dest->addr.in6, &dest->dst_saddr.in6,
				  atomic_read(&rt->dst.__refcnt));
		}
		if (ret_saddr)
			*ret_saddr = dest->dst_saddr.in6;
		spin_unlock(&dest->dst_lock);
	} else {
		dst = __ip_vs_route_output_v6(net, daddr, ret_saddr, do_xfrm);
		if (!dst)
			return NULL;
=======
static int
__ip_vs_get_out_rt_v6(struct netns_ipvs *ipvs, int skb_af, struct sk_buff *skb,
		      struct ip_vs_dest *dest,
		      struct in6_addr *daddr, struct in6_addr *ret_saddr,
		      struct ip_vs_iphdr *ipvsh, int do_xfrm, int rt_mode)
{
	struct net *net = ipvs->net;
	struct ip_vs_dest_dst *dest_dst;
	struct rt6_info *rt;			/* Route to the other host */
	struct dst_entry *dst;
	int mtu;
	int local, noref = 1;

	if (dest) {
		dest_dst = __ip_vs_dst_check(dest);
		if (likely(dest_dst))
			rt = (struct rt6_info *) dest_dst->dst_cache;
		else {
			u32 cookie;

			dest_dst = ip_vs_dest_dst_alloc();
			spin_lock_bh(&dest->dst_lock);
			if (!dest_dst) {
				__ip_vs_dst_set(dest, NULL, NULL, 0);
				spin_unlock_bh(&dest->dst_lock);
				goto err_unreach;
			}
			dst = __ip_vs_route_output_v6(net, &dest->addr.in6,
						      &dest_dst->dst_saddr.in6,
						      do_xfrm, rt_mode);
			if (!dst) {
				__ip_vs_dst_set(dest, NULL, NULL, 0);
				spin_unlock_bh(&dest->dst_lock);
				ip_vs_dest_dst_free(dest_dst);
				goto err_unreach;
			}
			rt = (struct rt6_info *) dst;
			cookie = rt6_get_cookie(rt);
			__ip_vs_dst_set(dest, dest_dst, &rt->dst, cookie);
			spin_unlock_bh(&dest->dst_lock);
			IP_VS_DBG(10, "new dst %pI6, src %pI6, refcnt=%d\n",
				  &dest->addr.in6, &dest_dst->dst_saddr.in6,
				  rcuref_read(&rt->dst.__rcuref));
		}
		if (ret_saddr)
			*ret_saddr = dest_dst->dst_saddr.in6;
	} else {
		noref = 0;
		dst = __ip_vs_route_output_v6(net, daddr, ret_saddr, do_xfrm,
					      rt_mode);
		if (!dst)
			goto err_unreach;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		rt = (struct rt6_info *) dst;
	}

	local = __ip_vs_is_local_route6(rt);
<<<<<<< HEAD
	if (!((local ? IP_VS_RT_MODE_LOCAL : IP_VS_RT_MODE_NON_LOCAL) &
	      rt_mode)) {
		IP_VS_DBG_RL("Stopping traffic to %s address, dest: %pI6\n",
			     local ? "local":"non-local", daddr);
		dst_release(&rt->dst);
		return NULL;
	}
	if (local && !(rt_mode & IP_VS_RT_MODE_RDR) &&
	    !((ort = (struct rt6_info *) skb_dst(skb)) &&
	      __ip_vs_is_local_route6(ort))) {
		IP_VS_DBG_RL("Redirect from non-local address %pI6 to local "
			     "requires NAT method, dest: %pI6\n",
			     &ipv6_hdr(skb)->daddr, daddr);
		dst_release(&rt->dst);
		return NULL;
	}
	if (unlikely(!local && (!skb->dev || skb->dev->flags & IFF_LOOPBACK) &&
		     ipv6_addr_type(&ipv6_hdr(skb)->saddr) &
				    IPV6_ADDR_LOOPBACK)) {
		IP_VS_DBG_RL("Stopping traffic from loopback address %pI6 "
			     "to non-local address, dest: %pI6\n",
			     &ipv6_hdr(skb)->saddr, daddr);
		dst_release(&rt->dst);
		return NULL;
	}

	return rt;
=======

	if (unlikely(crosses_local_route_boundary(skb_af, skb, rt_mode,
						  local))) {
		IP_VS_DBG_RL("We are crossing local and non-local addresses"
			     " daddr=%pI6\n", daddr);
		goto err_put;
	}

	if (unlikely(local)) {
		/* skb to local stack, preserve old route */
		if (!noref)
			dst_release(&rt->dst);
		return local;
	}

	if (!decrement_ttl(ipvs, skb_af, skb))
		goto err_put;

	/* MTU checking */
	if (likely(!(rt_mode & IP_VS_RT_MODE_TUNNEL)))
		mtu = dst_mtu(&rt->dst);
	else {
		mtu = dst_mtu(&rt->dst) - sizeof(struct ipv6hdr);
		if (!dest)
			goto err_put;
		if (dest->tun_type == IP_VS_CONN_F_TUNNEL_TYPE_GUE) {
			mtu -= sizeof(struct udphdr) + sizeof(struct guehdr);
			if ((dest->tun_flags &
			     IP_VS_TUNNEL_ENCAP_FLAG_REMCSUM) &&
			    skb->ip_summed == CHECKSUM_PARTIAL)
				mtu -= GUE_PLEN_REMCSUM + GUE_LEN_PRIV;
		} else if (dest->tun_type == IP_VS_CONN_F_TUNNEL_TYPE_GRE) {
			__be16 tflags = 0;

			if (dest->tun_flags & IP_VS_TUNNEL_ENCAP_FLAG_CSUM)
				tflags |= TUNNEL_CSUM;
			mtu -= gre_calc_hlen(tflags);
		}
		if (mtu < IPV6_MIN_MTU) {
			IP_VS_DBG_RL("%s(): mtu less than %d\n", __func__,
				     IPV6_MIN_MTU);
			goto err_put;
		}
		maybe_update_pmtu(skb_af, skb, mtu);
	}

	if (!ensure_mtu_is_adequate(ipvs, skb_af, rt_mode, ipvsh, skb, mtu))
		goto err_put;

	skb_dst_drop(skb);
	if (noref)
		skb_dst_set_noref(skb, &rt->dst);
	else
		skb_dst_set(skb, &rt->dst);

	return local;

err_put:
	if (!noref)
		dst_release(&rt->dst);
	return -1;

err_unreach:
	/* The ip6_link_failure function requires the dev field to be set
	 * in order to get the net (further for the sake of fwmark
	 * reflection).
	 */
	if (!skb->dev)
		skb->dev = skb_dst(skb)->dev;

	dst_link_failure(skb);
	return -1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
#endif


<<<<<<< HEAD
/*
 *	Release dest->dst_cache before a dest is removed
 */
void
ip_vs_dst_reset(struct ip_vs_dest *dest)
{
	struct dst_entry *old_dst;

	old_dst = dest->dst_cache;
	dest->dst_cache = NULL;
	dst_release(old_dst);
}

#define IP_VS_XMIT_TUNNEL(skb, cp)				\
({								\
	int __ret = NF_ACCEPT;					\
								\
	(skb)->ipvs_property = 1;				\
	if (unlikely((cp)->flags & IP_VS_CONN_F_NFCT))		\
		__ret = ip_vs_confirm_conntrack(skb);		\
	if (__ret == NF_ACCEPT) {				\
		nf_reset(skb);					\
		skb_forward_csum(skb);				\
	}							\
	__ret;							\
})

#define IP_VS_XMIT_NAT(pf, skb, cp, local)		\
do {							\
	(skb)->ipvs_property = 1;			\
	if (likely(!((cp)->flags & IP_VS_CONN_F_NFCT)))	\
		ip_vs_notrack(skb);			\
	else						\
		ip_vs_update_conntrack(skb, cp, 1);	\
	if (local)					\
		return NF_ACCEPT;			\
	skb_forward_csum(skb);				\
	NF_HOOK(pf, NF_INET_LOCAL_OUT, (skb), NULL,	\
		skb_dst(skb)->dev, dst_output);		\
} while (0)

#define IP_VS_XMIT(pf, skb, cp, local)			\
do {							\
	(skb)->ipvs_property = 1;			\
	if (likely(!((cp)->flags & IP_VS_CONN_F_NFCT)))	\
		ip_vs_notrack(skb);			\
	if (local)					\
		return NF_ACCEPT;			\
	skb_forward_csum(skb);				\
	NF_HOOK(pf, NF_INET_LOCAL_OUT, (skb), NULL,	\
		skb_dst(skb)->dev, dst_output);		\
} while (0)
=======
/* return NF_ACCEPT to allow forwarding or other NF_xxx on error */
static inline int ip_vs_tunnel_xmit_prepare(struct sk_buff *skb,
					    struct ip_vs_conn *cp)
{
	int ret = NF_ACCEPT;

	skb->ipvs_property = 1;
	if (unlikely(cp->flags & IP_VS_CONN_F_NFCT))
		ret = ip_vs_confirm_conntrack(skb);
	if (ret == NF_ACCEPT) {
		nf_reset_ct(skb);
		skb_forward_csum(skb);
		if (skb->dev)
			skb_clear_tstamp(skb);
	}
	return ret;
}

/* In the event of a remote destination, it's possible that we would have
 * matches against an old socket (particularly a TIME-WAIT socket). This
 * causes havoc down the line (ip_local_out et. al. expect regular sockets
 * and invalid memory accesses will happen) so simply drop the association
 * in this case.
*/
static inline void ip_vs_drop_early_demux_sk(struct sk_buff *skb)
{
	/* If dev is set, the packet came from the LOCAL_IN callback and
	 * not from a local TCP socket.
	 */
	if (skb->dev)
		skb_orphan(skb);
}

/* return NF_STOLEN (sent) or NF_ACCEPT if local=1 (not sent) */
static inline int ip_vs_nat_send_or_cont(int pf, struct sk_buff *skb,
					 struct ip_vs_conn *cp, int local)
{
	int ret = NF_STOLEN;

	skb->ipvs_property = 1;
	if (likely(!(cp->flags & IP_VS_CONN_F_NFCT)))
		ip_vs_notrack(skb);
	else
		ip_vs_update_conntrack(skb, cp, 1);

	/* Remove the early_demux association unless it's bound for the
	 * exact same port and address on this host after translation.
	 */
	if (!local || cp->vport != cp->dport ||
	    !ip_vs_addr_equal(cp->af, &cp->vaddr, &cp->daddr))
		ip_vs_drop_early_demux_sk(skb);

	if (!local) {
		skb_forward_csum(skb);
		if (skb->dev)
			skb_clear_tstamp(skb);
		NF_HOOK(pf, NF_INET_LOCAL_OUT, cp->ipvs->net, NULL, skb,
			NULL, skb_dst(skb)->dev, dst_output);
	} else
		ret = NF_ACCEPT;

	return ret;
}

/* return NF_STOLEN (sent) or NF_ACCEPT if local=1 (not sent) */
static inline int ip_vs_send_or_cont(int pf, struct sk_buff *skb,
				     struct ip_vs_conn *cp, int local)
{
	int ret = NF_STOLEN;

	skb->ipvs_property = 1;
	if (likely(!(cp->flags & IP_VS_CONN_F_NFCT)))
		ip_vs_notrack(skb);
	if (!local) {
		ip_vs_drop_early_demux_sk(skb);
		skb_forward_csum(skb);
		if (skb->dev)
			skb_clear_tstamp(skb);
		NF_HOOK(pf, NF_INET_LOCAL_OUT, cp->ipvs->net, NULL, skb,
			NULL, skb_dst(skb)->dev, dst_output);
	} else
		ret = NF_ACCEPT;
	return ret;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)


/*
 *      NULL transmitter (do nothing except return NF_ACCEPT)
 */
int
ip_vs_null_xmit(struct sk_buff *skb, struct ip_vs_conn *cp,
<<<<<<< HEAD
		struct ip_vs_protocol *pp)
{
	/* we do not touch skb and do not need pskb ptr */
	IP_VS_XMIT(NFPROTO_IPV4, skb, cp, 1);
=======
		struct ip_vs_protocol *pp, struct ip_vs_iphdr *ipvsh)
{
	/* we do not touch skb and do not need pskb ptr */
	return ip_vs_send_or_cont(NFPROTO_IPV4, skb, cp, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}


/*
 *      Bypass transmitter
 *      Let packets bypass the destination when the destination is not
 *      available, it may be only used in transparent cache cluster.
 */
int
ip_vs_bypass_xmit(struct sk_buff *skb, struct ip_vs_conn *cp,
<<<<<<< HEAD
		  struct ip_vs_protocol *pp)
{
	struct rtable *rt;			/* Route to the other host */
	struct iphdr  *iph = ip_hdr(skb);
	int    mtu;

	EnterFunction(10);

	if (!(rt = __ip_vs_get_out_rt(skb, NULL, iph->daddr, RT_TOS(iph->tos),
				      IP_VS_RT_MODE_NON_LOCAL, NULL)))
		goto tx_error_icmp;

	/* MTU checking */
	mtu = dst_mtu(&rt->dst);
	if ((skb->len > mtu) && (iph->frag_off & htons(IP_DF)) &&
	    !skb_is_gso(skb)) {
		ip_rt_put(rt);
		icmp_send(skb, ICMP_DEST_UNREACH,ICMP_FRAG_NEEDED, htonl(mtu));
		IP_VS_DBG_RL("%s(): frag needed\n", __func__);
		goto tx_error;
	}

	/*
	 * Call ip_send_check because we are not sure it is called
	 * after ip_defrag. Is copy-on-write needed?
	 */
	if (unlikely((skb = skb_share_check(skb, GFP_ATOMIC)) == NULL)) {
		ip_rt_put(rt);
		return NF_STOLEN;
	}
	ip_send_check(ip_hdr(skb));

	/* drop old route */
	skb_dst_drop(skb);
	skb_dst_set(skb, &rt->dst);

	/* Another hack: avoid icmp_send in ip_fragment */
	skb->local_df = 1;

	IP_VS_XMIT(NFPROTO_IPV4, skb, cp, 0);

	LeaveFunction(10);
	return NF_STOLEN;

 tx_error_icmp:
	dst_link_failure(skb);
 tx_error:
	kfree_skb(skb);
	LeaveFunction(10);
=======
		  struct ip_vs_protocol *pp, struct ip_vs_iphdr *ipvsh)
{
	struct iphdr  *iph = ip_hdr(skb);

	if (__ip_vs_get_out_rt(cp->ipvs, cp->af, skb, NULL, iph->daddr,
			       IP_VS_RT_MODE_NON_LOCAL, NULL, ipvsh) < 0)
		goto tx_error;

	ip_send_check(iph);

	/* Another hack: avoid icmp_send in ip_fragment */
	skb->ignore_df = 1;

	ip_vs_send_or_cont(NFPROTO_IPV4, skb, cp, 0);

	return NF_STOLEN;

 tx_error:
	kfree_skb(skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return NF_STOLEN;
}

#ifdef CONFIG_IP_VS_IPV6
int
ip_vs_bypass_xmit_v6(struct sk_buff *skb, struct ip_vs_conn *cp,
<<<<<<< HEAD
		     struct ip_vs_protocol *pp)
{
	struct rt6_info *rt;			/* Route to the other host */
	struct ipv6hdr  *iph = ipv6_hdr(skb);
	int    mtu;

	EnterFunction(10);

	if (!(rt = __ip_vs_get_out_rt_v6(skb, NULL, &iph->daddr, NULL, 0,
					 IP_VS_RT_MODE_NON_LOCAL)))
		goto tx_error_icmp;

	/* MTU checking */
	mtu = dst_mtu(&rt->dst);
	if (IP6CB(skb)->frag_max_size) {
		/* frag_max_size tell us that, this packet have been
		 * defragmented by netfilter IPv6 conntrack module.
		 */
		if (IP6CB(skb)->frag_max_size > mtu)
			return true; /* largest fragment violate MTU */
	}
	else if (skb->len > mtu && !skb_is_gso(skb)) {
		if (!skb->dev) {
			struct net *net = dev_net(skb_dst(skb)->dev);

			skb->dev = net->loopback_dev;
		}
		icmpv6_send(skb, ICMPV6_PKT_TOOBIG, 0, mtu);
		dst_release(&rt->dst);
		IP_VS_DBG_RL("%s(): frag needed\n", __func__);
		goto tx_error;
	}

	/*
	 * Call ip_send_check because we are not sure it is called
	 * after ip_defrag. Is copy-on-write needed?
	 */
	skb = skb_share_check(skb, GFP_ATOMIC);
	if (unlikely(skb == NULL)) {
		dst_release(&rt->dst);
		return NF_STOLEN;
	}

	/* drop old route */
	skb_dst_drop(skb);
	skb_dst_set(skb, &rt->dst);

	/* Another hack: avoid icmp_send in ip_fragment */
	skb->local_df = 1;

	IP_VS_XMIT(NFPROTO_IPV6, skb, cp, 0);

	LeaveFunction(10);
	return NF_STOLEN;

 tx_error_icmp:
	dst_link_failure(skb);
 tx_error:
	kfree_skb(skb);
	LeaveFunction(10);
=======
		     struct ip_vs_protocol *pp, struct ip_vs_iphdr *ipvsh)
{
	struct ipv6hdr *iph = ipv6_hdr(skb);

	if (__ip_vs_get_out_rt_v6(cp->ipvs, cp->af, skb, NULL,
				  &iph->daddr, NULL,
				  ipvsh, 0, IP_VS_RT_MODE_NON_LOCAL) < 0)
		goto tx_error;

	/* Another hack: avoid icmp_send in ip_fragment */
	skb->ignore_df = 1;

	ip_vs_send_or_cont(NFPROTO_IPV6, skb, cp, 0);

	return NF_STOLEN;

 tx_error:
	kfree_skb(skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return NF_STOLEN;
}
#endif

/*
 *      NAT transmitter (only for outside-to-inside nat forwarding)
 *      Not used for related ICMP
 */
int
ip_vs_nat_xmit(struct sk_buff *skb, struct ip_vs_conn *cp,
<<<<<<< HEAD
	       struct ip_vs_protocol *pp)
{
	struct rtable *rt;		/* Route to the other host */
	int mtu;
	struct iphdr *iph = ip_hdr(skb);
	int local;

	EnterFunction(10);
=======
	       struct ip_vs_protocol *pp, struct ip_vs_iphdr *ipvsh)
{
	struct rtable *rt;		/* Route to the other host */
	int local, rc, was_input;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* check if it is a connection of no-client-port */
	if (unlikely(cp->flags & IP_VS_CONN_F_NO_CPORT)) {
		__be16 _pt, *p;
<<<<<<< HEAD
		p = skb_header_pointer(skb, iph->ihl*4, sizeof(_pt), &_pt);
=======

		p = skb_header_pointer(skb, ipvsh->len, sizeof(_pt), &_pt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (p == NULL)
			goto tx_error;
		ip_vs_conn_fill_cport(cp, *p);
		IP_VS_DBG(10, "filled cport=%d\n", ntohs(*p));
	}

<<<<<<< HEAD
	if (!(rt = __ip_vs_get_out_rt(skb, cp->dest, cp->daddr.ip,
				      RT_TOS(iph->tos),
				      IP_VS_RT_MODE_LOCAL |
					IP_VS_RT_MODE_NON_LOCAL |
					IP_VS_RT_MODE_RDR, NULL)))
		goto tx_error_icmp;
	local = rt->rt_flags & RTCF_LOCAL;
=======
	was_input = rt_is_input_route(skb_rtable(skb));
	local = __ip_vs_get_out_rt(cp->ipvs, cp->af, skb, cp->dest, cp->daddr.ip,
				   IP_VS_RT_MODE_LOCAL |
				   IP_VS_RT_MODE_NON_LOCAL |
				   IP_VS_RT_MODE_RDR, NULL, ipvsh);
	if (local < 0)
		goto tx_error;
	rt = skb_rtable(skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Avoid duplicate tuple in reply direction for NAT traffic
	 * to local address when connection is sync-ed
	 */
#if IS_ENABLED(CONFIG_NF_CONNTRACK)
	if (cp->flags & IP_VS_CONN_F_SYNC && local) {
		enum ip_conntrack_info ctinfo;
<<<<<<< HEAD
		struct nf_conn *ct = ct = nf_ct_get(skb, &ctinfo);

		if (ct && !nf_ct_is_untracked(ct)) {
			IP_VS_DBG_RL_PKT(10, AF_INET, pp, skb, 0,
					 "ip_vs_nat_xmit(): "
					 "stopping DNAT to local address");
			goto tx_error_put;
=======
		struct nf_conn *ct = nf_ct_get(skb, &ctinfo);

		if (ct) {
			IP_VS_DBG_RL_PKT(10, AF_INET, pp, skb, ipvsh->off,
					 "ip_vs_nat_xmit(): "
					 "stopping DNAT to local address");
			goto tx_error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}
#endif

	/* From world but DNAT to loopback address? */
<<<<<<< HEAD
	if (local && ipv4_is_loopback(cp->daddr.ip) &&
	    rt_is_input_route(skb_rtable(skb))) {
		IP_VS_DBG_RL_PKT(1, AF_INET, pp, skb, 0, "ip_vs_nat_xmit(): "
				 "stopping DNAT to loopback address");
		goto tx_error_put;
	}

	/* MTU checking */
	mtu = dst_mtu(&rt->dst);
	if ((skb->len > mtu) && (iph->frag_off & htons(IP_DF)) &&
	    !skb_is_gso(skb)) {
		icmp_send(skb, ICMP_DEST_UNREACH,ICMP_FRAG_NEEDED, htonl(mtu));
		IP_VS_DBG_RL_PKT(0, AF_INET, pp, skb, 0,
				 "ip_vs_nat_xmit(): frag needed for");
		goto tx_error_put;
	}

	/* copy-on-write the packet before mangling it */
	if (!skb_make_writable(skb, sizeof(struct iphdr)))
		goto tx_error_put;

	if (skb_cow(skb, rt->dst.dev->hard_header_len))
		goto tx_error_put;

	/* mangle the packet */
	if (pp->dnat_handler && !pp->dnat_handler(skb, pp, cp))
		goto tx_error_put;
	ip_hdr(skb)->daddr = cp->daddr.ip;
	ip_send_check(ip_hdr(skb));

	if (!local) {
		/* drop old route */
		skb_dst_drop(skb);
		skb_dst_set(skb, &rt->dst);
	} else {
		ip_rt_put(rt);
		/*
		 * Some IPv4 replies get local address from routes,
		 * not from iph, so while we DNAT after routing
		 * we need this second input/output route.
		 */
		if (!__ip_vs_reroute_locally(skb))
			goto tx_error;
	}

	IP_VS_DBG_PKT(10, AF_INET, pp, skb, 0, "After DNAT");
=======
	if (local && ipv4_is_loopback(cp->daddr.ip) && was_input) {
		IP_VS_DBG_RL_PKT(1, AF_INET, pp, skb, ipvsh->off,
				 "ip_vs_nat_xmit(): stopping DNAT to loopback "
				 "address");
		goto tx_error;
	}

	/* copy-on-write the packet before mangling it */
	if (skb_ensure_writable(skb, sizeof(struct iphdr)))
		goto tx_error;

	if (skb_cow(skb, rt->dst.dev->hard_header_len))
		goto tx_error;

	/* mangle the packet */
	if (pp->dnat_handler && !pp->dnat_handler(skb, pp, cp, ipvsh))
		goto tx_error;
	ip_hdr(skb)->daddr = cp->daddr.ip;
	ip_send_check(ip_hdr(skb));

	IP_VS_DBG_PKT(10, AF_INET, pp, skb, ipvsh->off, "After DNAT");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* FIXME: when application helper enlarges the packet and the length
	   is larger than the MTU of outgoing device, there will be still
	   MTU problem. */

	/* Another hack: avoid icmp_send in ip_fragment */
<<<<<<< HEAD
	skb->local_df = 1;

	IP_VS_XMIT_NAT(NFPROTO_IPV4, skb, cp, local);

	LeaveFunction(10);
	return NF_STOLEN;

  tx_error_icmp:
	dst_link_failure(skb);
  tx_error:
	kfree_skb(skb);
	LeaveFunction(10);
	return NF_STOLEN;
  tx_error_put:
	ip_rt_put(rt);
	goto tx_error;
=======
	skb->ignore_df = 1;

	rc = ip_vs_nat_send_or_cont(NFPROTO_IPV4, skb, cp, local);

	return rc;

  tx_error:
	kfree_skb(skb);
	return NF_STOLEN;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#ifdef CONFIG_IP_VS_IPV6
int
ip_vs_nat_xmit_v6(struct sk_buff *skb, struct ip_vs_conn *cp,
<<<<<<< HEAD
		  struct ip_vs_protocol *pp)
{
	struct rt6_info *rt;		/* Route to the other host */
	int mtu;
	int local;

	EnterFunction(10);

	/* check if it is a connection of no-client-port */
	if (unlikely(cp->flags & IP_VS_CONN_F_NO_CPORT)) {
		__be16 _pt, *p;
		p = skb_header_pointer(skb, sizeof(struct ipv6hdr),
				       sizeof(_pt), &_pt);
=======
		  struct ip_vs_protocol *pp, struct ip_vs_iphdr *ipvsh)
{
	struct rt6_info *rt;		/* Route to the other host */
	int local, rc;

	/* check if it is a connection of no-client-port */
	if (unlikely(cp->flags & IP_VS_CONN_F_NO_CPORT && !ipvsh->fragoffs)) {
		__be16 _pt, *p;
		p = skb_header_pointer(skb, ipvsh->len, sizeof(_pt), &_pt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (p == NULL)
			goto tx_error;
		ip_vs_conn_fill_cport(cp, *p);
		IP_VS_DBG(10, "filled cport=%d\n", ntohs(*p));
	}

<<<<<<< HEAD
	if (!(rt = __ip_vs_get_out_rt_v6(skb, cp->dest, &cp->daddr.in6, NULL,
					 0, (IP_VS_RT_MODE_LOCAL |
					     IP_VS_RT_MODE_NON_LOCAL |
					     IP_VS_RT_MODE_RDR))))
		goto tx_error_icmp;
	local = __ip_vs_is_local_route6(rt);
=======
	local = __ip_vs_get_out_rt_v6(cp->ipvs, cp->af, skb, cp->dest,
				      &cp->daddr.in6,
				      NULL, ipvsh, 0,
				      IP_VS_RT_MODE_LOCAL |
				      IP_VS_RT_MODE_NON_LOCAL |
				      IP_VS_RT_MODE_RDR);
	if (local < 0)
		goto tx_error;
	rt = (struct rt6_info *) skb_dst(skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Avoid duplicate tuple in reply direction for NAT traffic
	 * to local address when connection is sync-ed
	 */
#if IS_ENABLED(CONFIG_NF_CONNTRACK)
	if (cp->flags & IP_VS_CONN_F_SYNC && local) {
		enum ip_conntrack_info ctinfo;
<<<<<<< HEAD
		struct nf_conn *ct = ct = nf_ct_get(skb, &ctinfo);

		if (ct && !nf_ct_is_untracked(ct)) {
			IP_VS_DBG_RL_PKT(10, AF_INET6, pp, skb, 0,
					 "ip_vs_nat_xmit_v6(): "
					 "stopping DNAT to local address");
			goto tx_error_put;
=======
		struct nf_conn *ct = nf_ct_get(skb, &ctinfo);

		if (ct) {
			IP_VS_DBG_RL_PKT(10, AF_INET6, pp, skb, ipvsh->off,
					 "ip_vs_nat_xmit_v6(): "
					 "stopping DNAT to local address");
			goto tx_error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}
#endif

	/* From world but DNAT to loopback address? */
	if (local && skb->dev && !(skb->dev->flags & IFF_LOOPBACK) &&
<<<<<<< HEAD
	    ipv6_addr_type(&rt->rt6i_dst.addr) & IPV6_ADDR_LOOPBACK) {
		IP_VS_DBG_RL_PKT(1, AF_INET6, pp, skb, 0,
				 "ip_vs_nat_xmit_v6(): "
				 "stopping DNAT to loopback address");
		goto tx_error_put;
	}

	/* MTU checking */
	mtu = dst_mtu(&rt->dst);
	if (IP6CB(skb)->frag_max_size) {
		/* frag_max_size tell us that, this packet have been
		 * defragmented by netfilter IPv6 conntrack module.
		 */
		if (IP6CB(skb)->frag_max_size > mtu)
			return true; /* largest fragment violate MTU */
	}
	else if (skb->len > mtu && !skb_is_gso(skb)) {
		if (!skb->dev) {
			struct net *net = dev_net(skb_dst(skb)->dev);

			skb->dev = net->loopback_dev;
		}
		icmpv6_send(skb, ICMPV6_PKT_TOOBIG, 0, mtu);
		IP_VS_DBG_RL_PKT(0, AF_INET6, pp, skb, 0,
				 "ip_vs_nat_xmit_v6(): frag needed for");
		goto tx_error_put;
	}

	/* copy-on-write the packet before mangling it */
	if (!skb_make_writable(skb, sizeof(struct ipv6hdr)))
		goto tx_error_put;

	if (skb_cow(skb, rt->dst.dev->hard_header_len))
		goto tx_error_put;

	/* mangle the packet */
	if (pp->dnat_handler && !pp->dnat_handler(skb, pp, cp))
		goto tx_error;
	ipv6_hdr(skb)->daddr = cp->daddr.in6;

	if (!local || !skb->dev) {
		/* drop the old route when skb is not shared */
		skb_dst_drop(skb);
		skb_dst_set(skb, &rt->dst);
	} else {
		/* destined to loopback, do we need to change route? */
		dst_release(&rt->dst);
	}

	IP_VS_DBG_PKT(10, AF_INET6, pp, skb, 0, "After DNAT");
=======
	    ipv6_addr_type(&cp->daddr.in6) & IPV6_ADDR_LOOPBACK) {
		IP_VS_DBG_RL_PKT(1, AF_INET6, pp, skb, ipvsh->off,
				 "ip_vs_nat_xmit_v6(): "
				 "stopping DNAT to loopback address");
		goto tx_error;
	}

	/* copy-on-write the packet before mangling it */
	if (skb_ensure_writable(skb, sizeof(struct ipv6hdr)))
		goto tx_error;

	if (skb_cow(skb, rt->dst.dev->hard_header_len))
		goto tx_error;

	/* mangle the packet */
	if (pp->dnat_handler && !pp->dnat_handler(skb, pp, cp, ipvsh))
		goto tx_error;
	ipv6_hdr(skb)->daddr = cp->daddr.in6;

	IP_VS_DBG_PKT(10, AF_INET6, pp, skb, ipvsh->off, "After DNAT");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* FIXME: when application helper enlarges the packet and the length
	   is larger than the MTU of outgoing device, there will be still
	   MTU problem. */

	/* Another hack: avoid icmp_send in ip_fragment */
<<<<<<< HEAD
	skb->local_df = 1;

	IP_VS_XMIT_NAT(NFPROTO_IPV6, skb, cp, local);

	LeaveFunction(10);
	return NF_STOLEN;

tx_error_icmp:
	dst_link_failure(skb);
tx_error:
	LeaveFunction(10);
	kfree_skb(skb);
	return NF_STOLEN;
tx_error_put:
	dst_release(&rt->dst);
	goto tx_error;
}
#endif

=======
	skb->ignore_df = 1;

	rc = ip_vs_nat_send_or_cont(NFPROTO_IPV6, skb, cp, local);

	return rc;

tx_error:
	kfree_skb(skb);
	return NF_STOLEN;
}
#endif

/* When forwarding a packet, we must ensure that we've got enough headroom
 * for the encapsulation packet in the skb.  This also gives us an
 * opportunity to figure out what the payload_len, dsfield, ttl, and df
 * values should be, so that we won't need to look at the old ip header
 * again
 */
static struct sk_buff *
ip_vs_prepare_tunneled_skb(struct sk_buff *skb, int skb_af,
			   unsigned int max_headroom, __u8 *next_protocol,
			   __u32 *payload_len, __u8 *dsfield, __u8 *ttl,
			   __be16 *df)
{
	struct sk_buff *new_skb = NULL;
	struct iphdr *old_iph = NULL;
	__u8 old_dsfield;
#ifdef CONFIG_IP_VS_IPV6
	struct ipv6hdr *old_ipv6h = NULL;
#endif

	ip_vs_drop_early_demux_sk(skb);

	if (skb_headroom(skb) < max_headroom || skb_cloned(skb)) {
		new_skb = skb_realloc_headroom(skb, max_headroom);
		if (!new_skb)
			goto error;
		if (skb->sk)
			skb_set_owner_w(new_skb, skb->sk);
		consume_skb(skb);
		skb = new_skb;
	}

#ifdef CONFIG_IP_VS_IPV6
	if (skb_af == AF_INET6) {
		old_ipv6h = ipv6_hdr(skb);
		*next_protocol = IPPROTO_IPV6;
		if (payload_len)
			*payload_len =
				ntohs(old_ipv6h->payload_len) +
				sizeof(*old_ipv6h);
		old_dsfield = ipv6_get_dsfield(old_ipv6h);
		*ttl = old_ipv6h->hop_limit;
		if (df)
			*df = 0;
	} else
#endif
	{
		old_iph = ip_hdr(skb);
		/* Copy DF, reset fragment offset and MF */
		if (df)
			*df = (old_iph->frag_off & htons(IP_DF));
		*next_protocol = IPPROTO_IPIP;

		/* fix old IP header checksum */
		ip_send_check(old_iph);
		old_dsfield = ipv4_get_dsfield(old_iph);
		*ttl = old_iph->ttl;
		if (payload_len)
			*payload_len = skb_ip_totlen(skb);
	}

	/* Implement full-functionality option for ECN encapsulation */
	*dsfield = INET_ECN_encapsulate(old_dsfield, old_dsfield);

	return skb;
error:
	kfree_skb(skb);
	return ERR_PTR(-ENOMEM);
}

static inline int __tun_gso_type_mask(int encaps_af, int orig_af)
{
	switch (encaps_af) {
	case AF_INET:
		return SKB_GSO_IPXIP4;
	case AF_INET6:
		return SKB_GSO_IPXIP6;
	default:
		return 0;
	}
}

static int
ipvs_gue_encap(struct net *net, struct sk_buff *skb,
	       struct ip_vs_conn *cp, __u8 *next_protocol)
{
	__be16 dport;
	__be16 sport = udp_flow_src_port(net, skb, 0, 0, false);
	struct udphdr  *udph;	/* Our new UDP header */
	struct guehdr  *gueh;	/* Our new GUE header */
	size_t hdrlen, optlen = 0;
	void *data;
	bool need_priv = false;

	if ((cp->dest->tun_flags & IP_VS_TUNNEL_ENCAP_FLAG_REMCSUM) &&
	    skb->ip_summed == CHECKSUM_PARTIAL) {
		optlen += GUE_PLEN_REMCSUM + GUE_LEN_PRIV;
		need_priv = true;
	}

	hdrlen = sizeof(struct guehdr) + optlen;

	skb_push(skb, hdrlen);

	gueh = (struct guehdr *)skb->data;

	gueh->control = 0;
	gueh->version = 0;
	gueh->hlen = optlen >> 2;
	gueh->flags = 0;
	gueh->proto_ctype = *next_protocol;

	data = &gueh[1];

	if (need_priv) {
		__be32 *flags = data;
		u16 csum_start = skb_checksum_start_offset(skb);
		__be16 *pd;

		gueh->flags |= GUE_FLAG_PRIV;
		*flags = 0;
		data += GUE_LEN_PRIV;

		if (csum_start < hdrlen)
			return -EINVAL;

		csum_start -= hdrlen;
		pd = data;
		pd[0] = htons(csum_start);
		pd[1] = htons(csum_start + skb->csum_offset);

		if (!skb_is_gso(skb)) {
			skb->ip_summed = CHECKSUM_NONE;
			skb->encapsulation = 0;
		}

		*flags |= GUE_PFLAG_REMCSUM;
		data += GUE_PLEN_REMCSUM;
	}

	skb_push(skb, sizeof(struct udphdr));
	skb_reset_transport_header(skb);

	udph = udp_hdr(skb);

	dport = cp->dest->tun_port;
	udph->dest = dport;
	udph->source = sport;
	udph->len = htons(skb->len);
	udph->check = 0;

	*next_protocol = IPPROTO_UDP;

	return 0;
}

static void
ipvs_gre_encap(struct net *net, struct sk_buff *skb,
	       struct ip_vs_conn *cp, __u8 *next_protocol)
{
	__be16 proto = *next_protocol == IPPROTO_IPIP ?
				htons(ETH_P_IP) : htons(ETH_P_IPV6);
	__be16 tflags = 0;
	size_t hdrlen;

	if (cp->dest->tun_flags & IP_VS_TUNNEL_ENCAP_FLAG_CSUM)
		tflags |= TUNNEL_CSUM;

	hdrlen = gre_calc_hlen(tflags);
	gre_build_header(skb, hdrlen, tflags, proto, 0, 0);

	*next_protocol = IPPROTO_GRE;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 *   IP Tunneling transmitter
 *
 *   This function encapsulates the packet in a new IP packet, its
 *   destination will be set to cp->daddr. Most code of this function
 *   is taken from ipip.c.
 *
 *   It is used in VS/TUN cluster. The load balancer selects a real
 *   server from a cluster based on a scheduling algorithm,
 *   encapsulates the request packet and forwards it to the selected
 *   server. For example, all real servers are configured with
 *   "ifconfig tunl0 <Virtual IP Address> up". When the server receives
 *   the encapsulated packet, it will decapsulate the packet, processe
 *   the request and return the response packets directly to the client
 *   without passing the load balancer. This can greatly increase the
 *   scalability of virtual server.
 *
 *   Used for ANY protocol
 */
int
ip_vs_tunnel_xmit(struct sk_buff *skb, struct ip_vs_conn *cp,
<<<<<<< HEAD
		  struct ip_vs_protocol *pp)
{
	struct rtable *rt;			/* Route to the other host */
	__be32 saddr;				/* Source for tunnel */
	struct net_device *tdev;		/* Device to other host */
	struct iphdr  *old_iph = ip_hdr(skb);
	u8     tos = old_iph->tos;
	__be16 df = old_iph->frag_off;
	struct iphdr  *iph;			/* Our new IP header */
	unsigned int max_headroom;		/* The extra header space needed */
	int    mtu;
	int ret;

	EnterFunction(10);

	if (!(rt = __ip_vs_get_out_rt(skb, cp->dest, cp->daddr.ip,
				      RT_TOS(tos), IP_VS_RT_MODE_LOCAL |
						   IP_VS_RT_MODE_NON_LOCAL,
						   &saddr)))
		goto tx_error_icmp;
	if (rt->rt_flags & RTCF_LOCAL) {
		ip_rt_put(rt);
		IP_VS_XMIT(NFPROTO_IPV4, skb, cp, 1);
	}

	tdev = rt->dst.dev;

	mtu = dst_mtu(&rt->dst) - sizeof(struct iphdr);
	if (mtu < 68) {
		IP_VS_DBG_RL("%s(): mtu less than 68\n", __func__);
		goto tx_error_put;
	}
	if (skb_dst(skb))
		skb_dst(skb)->ops->update_pmtu(skb_dst(skb), mtu);

	df |= (old_iph->frag_off & htons(IP_DF));

	if ((old_iph->frag_off & htons(IP_DF) &&
	    mtu < ntohs(old_iph->tot_len) && !skb_is_gso(skb))) {
		icmp_send(skb, ICMP_DEST_UNREACH,ICMP_FRAG_NEEDED, htonl(mtu));
		IP_VS_DBG_RL("%s(): frag needed\n", __func__);
		goto tx_error_put;
	}

=======
		  struct ip_vs_protocol *pp, struct ip_vs_iphdr *ipvsh)
{
	struct netns_ipvs *ipvs = cp->ipvs;
	struct net *net = ipvs->net;
	struct rtable *rt;			/* Route to the other host */
	__be32 saddr;				/* Source for tunnel */
	struct net_device *tdev;		/* Device to other host */
	__u8 next_protocol = 0;
	__u8 dsfield = 0;
	__u8 ttl = 0;
	__be16 df = 0;
	__be16 *dfp = NULL;
	struct iphdr  *iph;			/* Our new IP header */
	unsigned int max_headroom;		/* The extra header space needed */
	int ret, local;
	int tun_type, gso_type;
	int tun_flags;

	local = __ip_vs_get_out_rt(ipvs, cp->af, skb, cp->dest, cp->daddr.ip,
				   IP_VS_RT_MODE_LOCAL |
				   IP_VS_RT_MODE_NON_LOCAL |
				   IP_VS_RT_MODE_CONNECT |
				   IP_VS_RT_MODE_TUNNEL, &saddr, ipvsh);
	if (local < 0)
		goto tx_error;
	if (local)
		return ip_vs_send_or_cont(NFPROTO_IPV4, skb, cp, 1);

	rt = skb_rtable(skb);
	tdev = rt->dst.dev;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Okay, now see if we can stuff it in the buffer as-is.
	 */
	max_headroom = LL_RESERVED_SPACE(tdev) + sizeof(struct iphdr);

<<<<<<< HEAD
	if (skb_headroom(skb) < max_headroom
	    || skb_cloned(skb) || skb_shared(skb)) {
		struct sk_buff *new_skb =
			skb_realloc_headroom(skb, max_headroom);
		if (!new_skb) {
			ip_rt_put(rt);
			kfree_skb(skb);
			IP_VS_ERR_RL("%s(): no memory\n", __func__);
			return NF_STOLEN;
		}
		kfree_skb(skb);
		skb = new_skb;
		old_iph = ip_hdr(skb);
	}

	skb->transport_header = skb->network_header;

	/* fix old IP header checksum */
	ip_send_check(old_iph);
=======
	tun_type = cp->dest->tun_type;
	tun_flags = cp->dest->tun_flags;

	if (tun_type == IP_VS_CONN_F_TUNNEL_TYPE_GUE) {
		size_t gue_hdrlen, gue_optlen = 0;

		if ((tun_flags & IP_VS_TUNNEL_ENCAP_FLAG_REMCSUM) &&
		    skb->ip_summed == CHECKSUM_PARTIAL) {
			gue_optlen += GUE_PLEN_REMCSUM + GUE_LEN_PRIV;
		}
		gue_hdrlen = sizeof(struct guehdr) + gue_optlen;

		max_headroom += sizeof(struct udphdr) + gue_hdrlen;
	} else if (tun_type == IP_VS_CONN_F_TUNNEL_TYPE_GRE) {
		size_t gre_hdrlen;
		__be16 tflags = 0;

		if (tun_flags & IP_VS_TUNNEL_ENCAP_FLAG_CSUM)
			tflags |= TUNNEL_CSUM;
		gre_hdrlen = gre_calc_hlen(tflags);

		max_headroom += gre_hdrlen;
	}

	/* We only care about the df field if sysctl_pmtu_disc(ipvs) is set */
	dfp = sysctl_pmtu_disc(ipvs) ? &df : NULL;
	skb = ip_vs_prepare_tunneled_skb(skb, cp->af, max_headroom,
					 &next_protocol, NULL, &dsfield,
					 &ttl, dfp);
	if (IS_ERR(skb))
		return NF_STOLEN;

	gso_type = __tun_gso_type_mask(AF_INET, cp->af);
	if (tun_type == IP_VS_CONN_F_TUNNEL_TYPE_GUE) {
		if ((tun_flags & IP_VS_TUNNEL_ENCAP_FLAG_CSUM) ||
		    (tun_flags & IP_VS_TUNNEL_ENCAP_FLAG_REMCSUM))
			gso_type |= SKB_GSO_UDP_TUNNEL_CSUM;
		else
			gso_type |= SKB_GSO_UDP_TUNNEL;
		if ((tun_flags & IP_VS_TUNNEL_ENCAP_FLAG_REMCSUM) &&
		    skb->ip_summed == CHECKSUM_PARTIAL) {
			gso_type |= SKB_GSO_TUNNEL_REMCSUM;
		}
	} else if (tun_type == IP_VS_CONN_F_TUNNEL_TYPE_GRE) {
		if (tun_flags & IP_VS_TUNNEL_ENCAP_FLAG_CSUM)
			gso_type |= SKB_GSO_GRE_CSUM;
		else
			gso_type |= SKB_GSO_GRE;
	}

	if (iptunnel_handle_offloads(skb, gso_type))
		goto tx_error;

	skb->transport_header = skb->network_header;

	skb_set_inner_ipproto(skb, next_protocol);
	skb_set_inner_mac_header(skb, skb_inner_network_offset(skb));

	if (tun_type == IP_VS_CONN_F_TUNNEL_TYPE_GUE) {
		bool check = false;

		if (ipvs_gue_encap(net, skb, cp, &next_protocol))
			goto tx_error;

		if ((tun_flags & IP_VS_TUNNEL_ENCAP_FLAG_CSUM) ||
		    (tun_flags & IP_VS_TUNNEL_ENCAP_FLAG_REMCSUM))
			check = true;

		udp_set_csum(!check, skb, saddr, cp->daddr.ip, skb->len);
	} else if (tun_type == IP_VS_CONN_F_TUNNEL_TYPE_GRE)
		ipvs_gre_encap(net, skb, cp, &next_protocol);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	skb_push(skb, sizeof(struct iphdr));
	skb_reset_network_header(skb);
	memset(&(IPCB(skb)->opt), 0, sizeof(IPCB(skb)->opt));

<<<<<<< HEAD
	/* drop old route */
	skb_dst_drop(skb);
	skb_dst_set(skb, &rt->dst);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 *	Push down and install the IPIP header.
	 */
	iph			=	ip_hdr(skb);
	iph->version		=	4;
	iph->ihl		=	sizeof(struct iphdr)>>2;
	iph->frag_off		=	df;
<<<<<<< HEAD
	iph->protocol		=	IPPROTO_IPIP;
	iph->tos		=	tos;
	iph->daddr		=	cp->daddr.ip;
	iph->saddr		=	saddr;
	iph->ttl		=	old_iph->ttl;
	ip_select_ident(skb, NULL);

	/* Another hack: avoid icmp_send in ip_fragment */
	skb->local_df = 1;

	ret = IP_VS_XMIT_TUNNEL(skb, cp);
	if (ret == NF_ACCEPT)
		ip_local_out(skb);
	else if (ret == NF_DROP)
		kfree_skb(skb);

	LeaveFunction(10);

	return NF_STOLEN;

  tx_error_icmp:
	dst_link_failure(skb);
  tx_error:
	kfree_skb(skb);
	LeaveFunction(10);
	return NF_STOLEN;
tx_error_put:
	ip_rt_put(rt);
	goto tx_error;
=======
	iph->protocol		=	next_protocol;
	iph->tos		=	dsfield;
	iph->daddr		=	cp->daddr.ip;
	iph->saddr		=	saddr;
	iph->ttl		=	ttl;
	ip_select_ident(net, skb, NULL);

	/* Another hack: avoid icmp_send in ip_fragment */
	skb->ignore_df = 1;

	ret = ip_vs_tunnel_xmit_prepare(skb, cp);
	if (ret == NF_ACCEPT)
		ip_local_out(net, skb->sk, skb);
	else if (ret == NF_DROP)
		kfree_skb(skb);

	return NF_STOLEN;

  tx_error:
	kfree_skb(skb);
	return NF_STOLEN;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#ifdef CONFIG_IP_VS_IPV6
int
ip_vs_tunnel_xmit_v6(struct sk_buff *skb, struct ip_vs_conn *cp,
<<<<<<< HEAD
		     struct ip_vs_protocol *pp)
{
	struct rt6_info *rt;		/* Route to the other host */
	struct in6_addr saddr;		/* Source for tunnel */
	struct net_device *tdev;	/* Device to other host */
	struct ipv6hdr  *old_iph = ipv6_hdr(skb);
	struct ipv6hdr  *iph;		/* Our new IP header */
	unsigned int max_headroom;	/* The extra header space needed */
	int    mtu;
	int ret;

	EnterFunction(10);

	if (!(rt = __ip_vs_get_out_rt_v6(skb, cp->dest, &cp->daddr.in6,
					 &saddr, 1, (IP_VS_RT_MODE_LOCAL |
						     IP_VS_RT_MODE_NON_LOCAL))))
		goto tx_error_icmp;
	if (__ip_vs_is_local_route6(rt)) {
		dst_release(&rt->dst);
		IP_VS_XMIT(NFPROTO_IPV6, skb, cp, 1);
	}

	tdev = rt->dst.dev;

	mtu = dst_mtu(&rt->dst) - sizeof(struct ipv6hdr);
	if (mtu < IPV6_MIN_MTU) {
		IP_VS_DBG_RL("%s(): mtu less than %d\n", __func__,
			     IPV6_MIN_MTU);
		goto tx_error_put;
	}
	if (skb_dst(skb))
		skb_dst(skb)->ops->update_pmtu(skb_dst(skb), mtu);

	if (mtu < ntohs(old_iph->payload_len) + sizeof(struct ipv6hdr) &&
	    !skb_is_gso(skb)) {
		if (!skb->dev) {
			struct net *net = dev_net(skb_dst(skb)->dev);

			skb->dev = net->loopback_dev;
		}
		icmpv6_send(skb, ICMPV6_PKT_TOOBIG, 0, mtu);
		IP_VS_DBG_RL("%s(): frag needed\n", __func__);
		goto tx_error_put;
	}

=======
		     struct ip_vs_protocol *pp, struct ip_vs_iphdr *ipvsh)
{
	struct netns_ipvs *ipvs = cp->ipvs;
	struct net *net = ipvs->net;
	struct rt6_info *rt;		/* Route to the other host */
	struct in6_addr saddr;		/* Source for tunnel */
	struct net_device *tdev;	/* Device to other host */
	__u8 next_protocol = 0;
	__u32 payload_len = 0;
	__u8 dsfield = 0;
	__u8 ttl = 0;
	struct ipv6hdr  *iph;		/* Our new IP header */
	unsigned int max_headroom;	/* The extra header space needed */
	int ret, local;
	int tun_type, gso_type;
	int tun_flags;

	local = __ip_vs_get_out_rt_v6(ipvs, cp->af, skb, cp->dest,
				      &cp->daddr.in6,
				      &saddr, ipvsh, 1,
				      IP_VS_RT_MODE_LOCAL |
				      IP_VS_RT_MODE_NON_LOCAL |
				      IP_VS_RT_MODE_TUNNEL);
	if (local < 0)
		goto tx_error;
	if (local)
		return ip_vs_send_or_cont(NFPROTO_IPV6, skb, cp, 1);

	rt = (struct rt6_info *) skb_dst(skb);
	tdev = rt->dst.dev;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Okay, now see if we can stuff it in the buffer as-is.
	 */
	max_headroom = LL_RESERVED_SPACE(tdev) + sizeof(struct ipv6hdr);

<<<<<<< HEAD
	if (skb_headroom(skb) < max_headroom
	    || skb_cloned(skb) || skb_shared(skb)) {
		struct sk_buff *new_skb =
			skb_realloc_headroom(skb, max_headroom);
		if (!new_skb) {
			dst_release(&rt->dst);
			kfree_skb(skb);
			IP_VS_ERR_RL("%s(): no memory\n", __func__);
			return NF_STOLEN;
		}
		kfree_skb(skb);
		skb = new_skb;
		old_iph = ipv6_hdr(skb);
	}

	skb->transport_header = skb->network_header;

=======
	tun_type = cp->dest->tun_type;
	tun_flags = cp->dest->tun_flags;

	if (tun_type == IP_VS_CONN_F_TUNNEL_TYPE_GUE) {
		size_t gue_hdrlen, gue_optlen = 0;

		if ((tun_flags & IP_VS_TUNNEL_ENCAP_FLAG_REMCSUM) &&
		    skb->ip_summed == CHECKSUM_PARTIAL) {
			gue_optlen += GUE_PLEN_REMCSUM + GUE_LEN_PRIV;
		}
		gue_hdrlen = sizeof(struct guehdr) + gue_optlen;

		max_headroom += sizeof(struct udphdr) + gue_hdrlen;
	} else if (tun_type == IP_VS_CONN_F_TUNNEL_TYPE_GRE) {
		size_t gre_hdrlen;
		__be16 tflags = 0;

		if (tun_flags & IP_VS_TUNNEL_ENCAP_FLAG_CSUM)
			tflags |= TUNNEL_CSUM;
		gre_hdrlen = gre_calc_hlen(tflags);

		max_headroom += gre_hdrlen;
	}

	skb = ip_vs_prepare_tunneled_skb(skb, cp->af, max_headroom,
					 &next_protocol, &payload_len,
					 &dsfield, &ttl, NULL);
	if (IS_ERR(skb))
		return NF_STOLEN;

	gso_type = __tun_gso_type_mask(AF_INET6, cp->af);
	if (tun_type == IP_VS_CONN_F_TUNNEL_TYPE_GUE) {
		if ((tun_flags & IP_VS_TUNNEL_ENCAP_FLAG_CSUM) ||
		    (tun_flags & IP_VS_TUNNEL_ENCAP_FLAG_REMCSUM))
			gso_type |= SKB_GSO_UDP_TUNNEL_CSUM;
		else
			gso_type |= SKB_GSO_UDP_TUNNEL;
		if ((tun_flags & IP_VS_TUNNEL_ENCAP_FLAG_REMCSUM) &&
		    skb->ip_summed == CHECKSUM_PARTIAL) {
			gso_type |= SKB_GSO_TUNNEL_REMCSUM;
		}
	} else if (tun_type == IP_VS_CONN_F_TUNNEL_TYPE_GRE) {
		if (tun_flags & IP_VS_TUNNEL_ENCAP_FLAG_CSUM)
			gso_type |= SKB_GSO_GRE_CSUM;
		else
			gso_type |= SKB_GSO_GRE;
	}

	if (iptunnel_handle_offloads(skb, gso_type))
		goto tx_error;

	skb->transport_header = skb->network_header;

	skb_set_inner_ipproto(skb, next_protocol);
	skb_set_inner_mac_header(skb, skb_inner_network_offset(skb));

	if (tun_type == IP_VS_CONN_F_TUNNEL_TYPE_GUE) {
		bool check = false;

		if (ipvs_gue_encap(net, skb, cp, &next_protocol))
			goto tx_error;

		if ((tun_flags & IP_VS_TUNNEL_ENCAP_FLAG_CSUM) ||
		    (tun_flags & IP_VS_TUNNEL_ENCAP_FLAG_REMCSUM))
			check = true;

		udp6_set_csum(!check, skb, &saddr, &cp->daddr.in6, skb->len);
	} else if (tun_type == IP_VS_CONN_F_TUNNEL_TYPE_GRE)
		ipvs_gre_encap(net, skb, cp, &next_protocol);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	skb_push(skb, sizeof(struct ipv6hdr));
	skb_reset_network_header(skb);
	memset(&(IPCB(skb)->opt), 0, sizeof(IPCB(skb)->opt));

<<<<<<< HEAD
	/* drop old route */
	skb_dst_drop(skb);
	skb_dst_set(skb, &rt->dst);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 *	Push down and install the IPIP header.
	 */
	iph			=	ipv6_hdr(skb);
	iph->version		=	6;
<<<<<<< HEAD
	iph->nexthdr		=	IPPROTO_IPV6;
	iph->payload_len	=	old_iph->payload_len;
	be16_add_cpu(&iph->payload_len, sizeof(*old_iph));
	iph->priority		=	old_iph->priority;
	memset(&iph->flow_lbl, 0, sizeof(iph->flow_lbl));
	iph->daddr = cp->daddr.in6;
	iph->saddr = saddr;
	iph->hop_limit		=	old_iph->hop_limit;

	/* Another hack: avoid icmp_send in ip_fragment */
	skb->local_df = 1;

	ret = IP_VS_XMIT_TUNNEL(skb, cp);
	if (ret == NF_ACCEPT)
		ip6_local_out(skb);
	else if (ret == NF_DROP)
		kfree_skb(skb);

	LeaveFunction(10);

	return NF_STOLEN;

tx_error_icmp:
	dst_link_failure(skb);
tx_error:
	kfree_skb(skb);
	LeaveFunction(10);
	return NF_STOLEN;
tx_error_put:
	dst_release(&rt->dst);
	goto tx_error;
=======
	iph->nexthdr		=	next_protocol;
	iph->payload_len	=	htons(payload_len);
	memset(&iph->flow_lbl, 0, sizeof(iph->flow_lbl));
	ipv6_change_dsfield(iph, 0, dsfield);
	iph->daddr = cp->daddr.in6;
	iph->saddr = saddr;
	iph->hop_limit		=	ttl;

	/* Another hack: avoid icmp_send in ip_fragment */
	skb->ignore_df = 1;

	ret = ip_vs_tunnel_xmit_prepare(skb, cp);
	if (ret == NF_ACCEPT)
		ip6_local_out(net, skb->sk, skb);
	else if (ret == NF_DROP)
		kfree_skb(skb);

	return NF_STOLEN;

tx_error:
	kfree_skb(skb);
	return NF_STOLEN;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
#endif


/*
 *      Direct Routing transmitter
 *      Used for ANY protocol
 */
int
ip_vs_dr_xmit(struct sk_buff *skb, struct ip_vs_conn *cp,
<<<<<<< HEAD
	      struct ip_vs_protocol *pp)
{
	struct rtable *rt;			/* Route to the other host */
	struct iphdr  *iph = ip_hdr(skb);
	int    mtu;

	EnterFunction(10);

	if (!(rt = __ip_vs_get_out_rt(skb, cp->dest, cp->daddr.ip,
				      RT_TOS(iph->tos),
				      IP_VS_RT_MODE_LOCAL |
					IP_VS_RT_MODE_NON_LOCAL, NULL)))
		goto tx_error_icmp;
	if (rt->rt_flags & RTCF_LOCAL) {
		ip_rt_put(rt);
		IP_VS_XMIT(NFPROTO_IPV4, skb, cp, 1);
	}

	/* MTU checking */
	mtu = dst_mtu(&rt->dst);
	if ((iph->frag_off & htons(IP_DF)) && skb->len > mtu &&
	    !skb_is_gso(skb)) {
		icmp_send(skb, ICMP_DEST_UNREACH,ICMP_FRAG_NEEDED, htonl(mtu));
		ip_rt_put(rt);
		IP_VS_DBG_RL("%s(): frag needed\n", __func__);
		goto tx_error;
	}

	/*
	 * Call ip_send_check because we are not sure it is called
	 * after ip_defrag. Is copy-on-write needed?
	 */
	if (unlikely((skb = skb_share_check(skb, GFP_ATOMIC)) == NULL)) {
		ip_rt_put(rt);
		return NF_STOLEN;
	}
	ip_send_check(ip_hdr(skb));

	/* drop old route */
	skb_dst_drop(skb);
	skb_dst_set(skb, &rt->dst);

	/* Another hack: avoid icmp_send in ip_fragment */
	skb->local_df = 1;

	IP_VS_XMIT(NFPROTO_IPV4, skb, cp, 0);

	LeaveFunction(10);
	return NF_STOLEN;

  tx_error_icmp:
	dst_link_failure(skb);
  tx_error:
	kfree_skb(skb);
	LeaveFunction(10);
=======
	      struct ip_vs_protocol *pp, struct ip_vs_iphdr *ipvsh)
{
	int local;

	local = __ip_vs_get_out_rt(cp->ipvs, cp->af, skb, cp->dest, cp->daddr.ip,
				   IP_VS_RT_MODE_LOCAL |
				   IP_VS_RT_MODE_NON_LOCAL |
				   IP_VS_RT_MODE_KNOWN_NH, NULL, ipvsh);
	if (local < 0)
		goto tx_error;
	if (local)
		return ip_vs_send_or_cont(NFPROTO_IPV4, skb, cp, 1);

	ip_send_check(ip_hdr(skb));

	/* Another hack: avoid icmp_send in ip_fragment */
	skb->ignore_df = 1;

	ip_vs_send_or_cont(NFPROTO_IPV4, skb, cp, 0);

	return NF_STOLEN;

  tx_error:
	kfree_skb(skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return NF_STOLEN;
}

#ifdef CONFIG_IP_VS_IPV6
int
ip_vs_dr_xmit_v6(struct sk_buff *skb, struct ip_vs_conn *cp,
<<<<<<< HEAD
		 struct ip_vs_protocol *pp)
{
	struct rt6_info *rt;			/* Route to the other host */
	int    mtu;

	EnterFunction(10);

	if (!(rt = __ip_vs_get_out_rt_v6(skb, cp->dest, &cp->daddr.in6, NULL,
					 0, (IP_VS_RT_MODE_LOCAL |
					     IP_VS_RT_MODE_NON_LOCAL))))
		goto tx_error_icmp;
	if (__ip_vs_is_local_route6(rt)) {
		dst_release(&rt->dst);
		IP_VS_XMIT(NFPROTO_IPV6, skb, cp, 1);
	}

	/* MTU checking */
	mtu = dst_mtu(&rt->dst);
	if (skb->len > mtu) {
		if (!skb->dev) {
			struct net *net = dev_net(skb_dst(skb)->dev);

			skb->dev = net->loopback_dev;
		}
		icmpv6_send(skb, ICMPV6_PKT_TOOBIG, 0, mtu);
		dst_release(&rt->dst);
		IP_VS_DBG_RL("%s(): frag needed\n", __func__);
		goto tx_error;
	}

	/*
	 * Call ip_send_check because we are not sure it is called
	 * after ip_defrag. Is copy-on-write needed?
	 */
	skb = skb_share_check(skb, GFP_ATOMIC);
	if (unlikely(skb == NULL)) {
		dst_release(&rt->dst);
		return NF_STOLEN;
	}

	/* drop old route */
	skb_dst_drop(skb);
	skb_dst_set(skb, &rt->dst);

	/* Another hack: avoid icmp_send in ip_fragment */
	skb->local_df = 1;

	IP_VS_XMIT(NFPROTO_IPV6, skb, cp, 0);

	LeaveFunction(10);
	return NF_STOLEN;

tx_error_icmp:
	dst_link_failure(skb);
tx_error:
	kfree_skb(skb);
	LeaveFunction(10);
=======
		 struct ip_vs_protocol *pp, struct ip_vs_iphdr *ipvsh)
{
	int local;

	local = __ip_vs_get_out_rt_v6(cp->ipvs, cp->af, skb, cp->dest,
				      &cp->daddr.in6,
				      NULL, ipvsh, 0,
				      IP_VS_RT_MODE_LOCAL |
				      IP_VS_RT_MODE_NON_LOCAL |
				      IP_VS_RT_MODE_KNOWN_NH);
	if (local < 0)
		goto tx_error;
	if (local)
		return ip_vs_send_or_cont(NFPROTO_IPV6, skb, cp, 1);

	/* Another hack: avoid icmp_send in ip_fragment */
	skb->ignore_df = 1;

	ip_vs_send_or_cont(NFPROTO_IPV6, skb, cp, 0);

	return NF_STOLEN;

tx_error:
	kfree_skb(skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return NF_STOLEN;
}
#endif


/*
 *	ICMP packet transmitter
 *	called by the ip_vs_in_icmp
 */
int
ip_vs_icmp_xmit(struct sk_buff *skb, struct ip_vs_conn *cp,
<<<<<<< HEAD
		struct ip_vs_protocol *pp, int offset, unsigned int hooknum)
{
	struct rtable	*rt;	/* Route to the other host */
	int mtu;
	int rc;
	int local;
	int rt_mode;

	EnterFunction(10);
=======
		struct ip_vs_protocol *pp, int offset, unsigned int hooknum,
		struct ip_vs_iphdr *iph)
{
	struct rtable	*rt;	/* Route to the other host */
	int rc;
	int local;
	int rt_mode, was_input;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* The ICMP packet for VS/TUN, VS/DR and LOCALNODE will be
	   forwarded directly here, because there is no need to
	   translate address/port back */
	if (IP_VS_FWD_METHOD(cp) != IP_VS_CONN_F_MASQ) {
		if (cp->packet_xmit)
<<<<<<< HEAD
			rc = cp->packet_xmit(skb, cp, pp);
=======
			rc = cp->packet_xmit(skb, cp, pp, iph);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		else
			rc = NF_ACCEPT;
		/* do not touch skb anymore */
		atomic_inc(&cp->in_pkts);
<<<<<<< HEAD
		goto out;
=======
		return rc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/*
	 * mangle and send the packet here (only for VS/NAT)
	 */
<<<<<<< HEAD
=======
	was_input = rt_is_input_route(skb_rtable(skb));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* LOCALNODE from FORWARD hook is not supported */
	rt_mode = (hooknum != NF_INET_FORWARD) ?
		  IP_VS_RT_MODE_LOCAL | IP_VS_RT_MODE_NON_LOCAL |
		  IP_VS_RT_MODE_RDR : IP_VS_RT_MODE_NON_LOCAL;
<<<<<<< HEAD
	if (!(rt = __ip_vs_get_out_rt(skb, cp->dest, cp->daddr.ip,
				      RT_TOS(ip_hdr(skb)->tos),
				      rt_mode, NULL)))
		goto tx_error_icmp;
	local = rt->rt_flags & RTCF_LOCAL;
=======
	local = __ip_vs_get_out_rt(cp->ipvs, cp->af, skb, cp->dest, cp->daddr.ip, rt_mode,
				   NULL, iph);
	if (local < 0)
		goto tx_error;
	rt = skb_rtable(skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Avoid duplicate tuple in reply direction for NAT traffic
	 * to local address when connection is sync-ed
	 */
#if IS_ENABLED(CONFIG_NF_CONNTRACK)
	if (cp->flags & IP_VS_CONN_F_SYNC && local) {
		enum ip_conntrack_info ctinfo;
<<<<<<< HEAD
		struct nf_conn *ct = ct = nf_ct_get(skb, &ctinfo);

		if (ct && !nf_ct_is_untracked(ct)) {
			IP_VS_DBG(10, "%s(): "
				  "stopping DNAT to local address %pI4\n",
				  __func__, &cp->daddr.ip);
			goto tx_error_put;
=======
		struct nf_conn *ct = nf_ct_get(skb, &ctinfo);

		if (ct) {
			IP_VS_DBG(10, "%s(): "
				  "stopping DNAT to local address %pI4\n",
				  __func__, &cp->daddr.ip);
			goto tx_error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}
#endif

	/* From world but DNAT to loopback address? */
<<<<<<< HEAD
	if (local && ipv4_is_loopback(cp->daddr.ip) &&
	    rt_is_input_route(skb_rtable(skb))) {
		IP_VS_DBG(1, "%s(): "
			  "stopping DNAT to loopback %pI4\n",
			  __func__, &cp->daddr.ip);
		goto tx_error_put;
	}

	/* MTU checking */
	mtu = dst_mtu(&rt->dst);
	if ((skb->len > mtu) && (ip_hdr(skb)->frag_off & htons(IP_DF)) &&
	    !skb_is_gso(skb)) {
		icmp_send(skb, ICMP_DEST_UNREACH, ICMP_FRAG_NEEDED, htonl(mtu));
		IP_VS_DBG_RL("%s(): frag needed\n", __func__);
		goto tx_error_put;
	}

	/* copy-on-write the packet before mangling it */
	if (!skb_make_writable(skb, offset))
		goto tx_error_put;

	if (skb_cow(skb, rt->dst.dev->hard_header_len))
		goto tx_error_put;

	ip_vs_nat_icmp(skb, pp, cp, 0);

	if (!local) {
		/* drop the old route when skb is not shared */
		skb_dst_drop(skb);
		skb_dst_set(skb, &rt->dst);
	} else {
		ip_rt_put(rt);
		/*
		 * Some IPv4 replies get local address from routes,
		 * not from iph, so while we DNAT after routing
		 * we need this second input/output route.
		 */
		if (!__ip_vs_reroute_locally(skb))
			goto tx_error;
	}

	/* Another hack: avoid icmp_send in ip_fragment */
	skb->local_df = 1;

	IP_VS_XMIT_NAT(NFPROTO_IPV4, skb, cp, local);

	rc = NF_STOLEN;
	goto out;

  tx_error_icmp:
	dst_link_failure(skb);
  tx_error:
	dev_kfree_skb(skb);
	rc = NF_STOLEN;
  out:
	LeaveFunction(10);
	return rc;
  tx_error_put:
	ip_rt_put(rt);
	goto tx_error;
=======
	if (local && ipv4_is_loopback(cp->daddr.ip) && was_input) {
		IP_VS_DBG(1, "%s(): "
			  "stopping DNAT to loopback %pI4\n",
			  __func__, &cp->daddr.ip);
		goto tx_error;
	}

	/* copy-on-write the packet before mangling it */
	if (skb_ensure_writable(skb, offset))
		goto tx_error;

	if (skb_cow(skb, rt->dst.dev->hard_header_len))
		goto tx_error;

	ip_vs_nat_icmp(skb, pp, cp, 0);

	/* Another hack: avoid icmp_send in ip_fragment */
	skb->ignore_df = 1;

	return ip_vs_nat_send_or_cont(NFPROTO_IPV4, skb, cp, local);

  tx_error:
	kfree_skb(skb);
	rc = NF_STOLEN;
	return rc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#ifdef CONFIG_IP_VS_IPV6
int
ip_vs_icmp_xmit_v6(struct sk_buff *skb, struct ip_vs_conn *cp,
<<<<<<< HEAD
		struct ip_vs_protocol *pp, int offset, unsigned int hooknum)
{
	struct rt6_info	*rt;	/* Route to the other host */
	int mtu;
=======
		struct ip_vs_protocol *pp, int offset, unsigned int hooknum,
		struct ip_vs_iphdr *ipvsh)
{
	struct rt6_info	*rt;	/* Route to the other host */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int rc;
	int local;
	int rt_mode;

<<<<<<< HEAD
	EnterFunction(10);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* The ICMP packet for VS/TUN, VS/DR and LOCALNODE will be
	   forwarded directly here, because there is no need to
	   translate address/port back */
	if (IP_VS_FWD_METHOD(cp) != IP_VS_CONN_F_MASQ) {
		if (cp->packet_xmit)
<<<<<<< HEAD
			rc = cp->packet_xmit(skb, cp, pp);
=======
			rc = cp->packet_xmit(skb, cp, pp, ipvsh);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		else
			rc = NF_ACCEPT;
		/* do not touch skb anymore */
		atomic_inc(&cp->in_pkts);
<<<<<<< HEAD
		goto out;
=======
		return rc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/*
	 * mangle and send the packet here (only for VS/NAT)
	 */

	/* LOCALNODE from FORWARD hook is not supported */
	rt_mode = (hooknum != NF_INET_FORWARD) ?
		  IP_VS_RT_MODE_LOCAL | IP_VS_RT_MODE_NON_LOCAL |
		  IP_VS_RT_MODE_RDR : IP_VS_RT_MODE_NON_LOCAL;
<<<<<<< HEAD
	if (!(rt = __ip_vs_get_out_rt_v6(skb, cp->dest, &cp->daddr.in6, NULL,
					 0, rt_mode)))
		goto tx_error_icmp;

	local = __ip_vs_is_local_route6(rt);
=======
	local = __ip_vs_get_out_rt_v6(cp->ipvs, cp->af, skb, cp->dest,
				      &cp->daddr.in6, NULL, ipvsh, 0, rt_mode);
	if (local < 0)
		goto tx_error;
	rt = (struct rt6_info *) skb_dst(skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Avoid duplicate tuple in reply direction for NAT traffic
	 * to local address when connection is sync-ed
	 */
#if IS_ENABLED(CONFIG_NF_CONNTRACK)
	if (cp->flags & IP_VS_CONN_F_SYNC && local) {
		enum ip_conntrack_info ctinfo;
<<<<<<< HEAD
		struct nf_conn *ct = ct = nf_ct_get(skb, &ctinfo);

		if (ct && !nf_ct_is_untracked(ct)) {
			IP_VS_DBG(10, "%s(): "
				  "stopping DNAT to local address %pI6\n",
				  __func__, &cp->daddr.in6);
			goto tx_error_put;
=======
		struct nf_conn *ct = nf_ct_get(skb, &ctinfo);

		if (ct) {
			IP_VS_DBG(10, "%s(): "
				  "stopping DNAT to local address %pI6\n",
				  __func__, &cp->daddr.in6);
			goto tx_error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}
#endif

	/* From world but DNAT to loopback address? */
	if (local && skb->dev && !(skb->dev->flags & IFF_LOOPBACK) &&
<<<<<<< HEAD
	    ipv6_addr_type(&rt->rt6i_dst.addr) & IPV6_ADDR_LOOPBACK) {
		IP_VS_DBG(1, "%s(): "
			  "stopping DNAT to loopback %pI6\n",
			  __func__, &cp->daddr.in6);
		goto tx_error_put;
	}

	/* MTU checking */
	mtu = dst_mtu(&rt->dst);
	if (IP6CB(skb)->frag_max_size) {
		/* frag_max_size tell us that, this packet have been
		 * defragmented by netfilter IPv6 conntrack module.
		 */
		if (IP6CB(skb)->frag_max_size > mtu)
			return true; /* largest fragment violate MTU */
	}
	else if (skb->len > mtu && !skb_is_gso(skb)) {
		if (!skb->dev) {
			struct net *net = dev_net(skb_dst(skb)->dev);

			skb->dev = net->loopback_dev;
		}
		icmpv6_send(skb, ICMPV6_PKT_TOOBIG, 0, mtu);
		IP_VS_DBG_RL("%s(): frag needed\n", __func__);
		goto tx_error_put;
	}

	/* copy-on-write the packet before mangling it */
	if (!skb_make_writable(skb, offset))
		goto tx_error_put;

	if (skb_cow(skb, rt->dst.dev->hard_header_len))
		goto tx_error_put;

	ip_vs_nat_icmp_v6(skb, pp, cp, 0);

	if (!local || !skb->dev) {
		/* drop the old route when skb is not shared */
		skb_dst_drop(skb);
		skb_dst_set(skb, &rt->dst);
	} else {
		/* destined to loopback, do we need to change route? */
		dst_release(&rt->dst);
	}

	/* Another hack: avoid icmp_send in ip_fragment */
	skb->local_df = 1;

	IP_VS_XMIT_NAT(NFPROTO_IPV6, skb, cp, local);

	rc = NF_STOLEN;
	goto out;

tx_error_icmp:
	dst_link_failure(skb);
tx_error:
	dev_kfree_skb(skb);
	rc = NF_STOLEN;
out:
	LeaveFunction(10);
	return rc;
tx_error_put:
	dst_release(&rt->dst);
	goto tx_error;
=======
	    ipv6_addr_type(&cp->daddr.in6) & IPV6_ADDR_LOOPBACK) {
		IP_VS_DBG(1, "%s(): "
			  "stopping DNAT to loopback %pI6\n",
			  __func__, &cp->daddr.in6);
		goto tx_error;
	}

	/* copy-on-write the packet before mangling it */
	if (skb_ensure_writable(skb, offset))
		goto tx_error;

	if (skb_cow(skb, rt->dst.dev->hard_header_len))
		goto tx_error;

	ip_vs_nat_icmp_v6(skb, pp, cp, 0);

	/* Another hack: avoid icmp_send in ip_fragment */
	skb->ignore_df = 1;

	return ip_vs_nat_send_or_cont(NFPROTO_IPV6, skb, cp, local);

tx_error:
	kfree_skb(skb);
	rc = NF_STOLEN;
	return rc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
#endif
