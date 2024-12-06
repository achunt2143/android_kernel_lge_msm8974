<<<<<<< HEAD
/*
 * Copyright (c) 2011 Florian Westphal <fw@strlen.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2011 Florian Westphal <fw@strlen.de>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/route.h>
#include <net/ip6_fib.h>
#include <net/ip6_route.h>

#include <linux/netfilter/xt_rpfilter.h>
#include <linux/netfilter/x_tables.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Florian Westphal <fw@strlen.de>");
MODULE_DESCRIPTION("Xtables: IPv6 reverse path filter match");

static bool rpfilter_addr_unicast(const struct in6_addr *addr)
{
	int addr_type = ipv6_addr_type(addr);
	return addr_type & IPV6_ADDR_UNICAST;
}

<<<<<<< HEAD
static bool rpfilter_lookup_reverse6(const struct sk_buff *skb,
=======
static bool rpfilter_addr_linklocal(const struct in6_addr *addr)
{
	int addr_type = ipv6_addr_type(addr);
	return addr_type & IPV6_ADDR_LINKLOCAL;
}

static bool rpfilter_lookup_reverse6(struct net *net, const struct sk_buff *skb,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				     const struct net_device *dev, u8 flags)
{
	struct rt6_info *rt;
	struct ipv6hdr *iph = ipv6_hdr(skb);
	bool ret = false;
	struct flowi6 fl6 = {
<<<<<<< HEAD
		.flowlabel = (* (__be32 *) iph) & IPV6_FLOWINFO_MASK,
		.flowi6_proto = iph->nexthdr,
=======
		.flowi6_iif = LOOPBACK_IFINDEX,
		.flowi6_l3mdev = l3mdev_master_ifindex_rcu(dev),
		.flowlabel = (* (__be32 *) iph) & IPV6_FLOWINFO_MASK,
		.flowi6_proto = iph->nexthdr,
		.flowi6_uid = sock_net_uid(net, NULL),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.daddr = iph->saddr,
	};
	int lookup_flags;

	if (rpfilter_addr_unicast(&iph->daddr)) {
		memcpy(&fl6.saddr, &iph->daddr, sizeof(struct in6_addr));
		lookup_flags = RT6_LOOKUP_F_HAS_SADDR;
	} else {
		lookup_flags = 0;
	}

	fl6.flowi6_mark = flags & XT_RPFILTER_VALID_MARK ? skb->mark : 0;
<<<<<<< HEAD
	if ((flags & XT_RPFILTER_LOOSE) == 0) {
		fl6.flowi6_oif = dev->ifindex;
		lookup_flags |= RT6_LOOKUP_F_IFACE;
	}

	rt = (void *) ip6_route_lookup(dev_net(dev), &fl6, lookup_flags);
=======

	if (rpfilter_addr_linklocal(&iph->saddr)) {
		lookup_flags |= RT6_LOOKUP_F_IFACE;
		fl6.flowi6_oif = dev->ifindex;
	} else if ((flags & XT_RPFILTER_LOOSE) == 0)
		fl6.flowi6_oif = dev->ifindex;

	rt = (void *)ip6_route_lookup(net, &fl6, skb, lookup_flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (rt->dst.error)
		goto out;

	if (rt->rt6i_flags & (RTF_REJECT|RTF_ANYCAST))
		goto out;

	if (rt->rt6i_flags & RTF_LOCAL) {
		ret = flags & XT_RPFILTER_ACCEPT_LOCAL;
		goto out;
	}

<<<<<<< HEAD
	if (rt->rt6i_idev->dev == dev || (flags & XT_RPFILTER_LOOSE))
		ret = true;
 out:
	dst_release(&rt->dst);
	return ret;
}

static bool rpfilter_is_local(const struct sk_buff *skb)
{
	const struct rt6_info *rt = (const void *) skb_dst(skb);
	return rt && (rt->rt6i_flags & RTF_LOCAL);
=======
	if (rt->rt6i_idev->dev == dev ||
	    l3mdev_master_ifindex_rcu(rt->rt6i_idev->dev) == dev->ifindex ||
	    (flags & XT_RPFILTER_LOOSE))
		ret = true;
 out:
	ip6_rt_put(rt);
	return ret;
}

static bool
rpfilter_is_loopback(const struct sk_buff *skb, const struct net_device *in)
{
	return skb->pkt_type == PACKET_LOOPBACK || in->flags & IFF_LOOPBACK;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static bool rpfilter_mt(const struct sk_buff *skb, struct xt_action_param *par)
{
	const struct xt_rpfilter_info *info = par->matchinfo;
	int saddrtype;
	struct ipv6hdr *iph;
	bool invert = info->flags & XT_RPFILTER_INVERT;

<<<<<<< HEAD
	if (rpfilter_is_local(skb))
=======
	if (rpfilter_is_loopback(skb, xt_in(par)))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return true ^ invert;

	iph = ipv6_hdr(skb);
	saddrtype = ipv6_addr_type(&iph->saddr);
	if (unlikely(saddrtype == IPV6_ADDR_ANY))
		return true ^ invert; /* not routable: forward path will drop it */

<<<<<<< HEAD
	return rpfilter_lookup_reverse6(skb, par->in, info->flags) ^ invert;
=======
	return rpfilter_lookup_reverse6(xt_net(par), skb, xt_in(par),
					info->flags) ^ invert;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int rpfilter_check(const struct xt_mtchk_param *par)
{
	const struct xt_rpfilter_info *info = par->matchinfo;
	unsigned int options = ~XT_RPFILTER_OPTION_MASK;

	if (info->flags & options) {
<<<<<<< HEAD
		pr_info("unknown options encountered");
=======
		pr_info_ratelimited("unknown options\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;
	}

	if (strcmp(par->table, "mangle") != 0 &&
	    strcmp(par->table, "raw") != 0) {
<<<<<<< HEAD
		pr_info("match only valid in the \'raw\' "
			"or \'mangle\' tables, not \'%s\'.\n", par->table);
=======
		pr_info_ratelimited("only valid in \'raw\' or \'mangle\' table, not \'%s\'\n",
				    par->table);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;
	}

	return 0;
}

static struct xt_match rpfilter_mt_reg __read_mostly = {
	.name		= "rpfilter",
	.family		= NFPROTO_IPV6,
	.checkentry	= rpfilter_check,
	.match		= rpfilter_mt,
	.matchsize	= sizeof(struct xt_rpfilter_info),
	.hooks		= (1 << NF_INET_PRE_ROUTING),
	.me		= THIS_MODULE
};

static int __init rpfilter_mt_init(void)
{
	return xt_register_match(&rpfilter_mt_reg);
}

static void __exit rpfilter_mt_exit(void)
{
	xt_unregister_match(&rpfilter_mt_reg);
}

module_init(rpfilter_mt_init);
module_exit(rpfilter_mt_exit);
