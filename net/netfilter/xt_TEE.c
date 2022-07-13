<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *	"TEE" target extension for Xtables
 *	Copyright © Sebastian Claßen, 2007
 *	Jan Engelhardt, 2007-2010
 *
 *	based on ipt_ROUTE.c from Cédric de Launois
 *	<delaunois@info.ucl.be>
<<<<<<< HEAD
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	version 2 or later, as published by the Free Software Foundation.
 */
#include <linux/ip.h>
#include <linux/module.h>
#include <linux/percpu.h>
#include <linux/route.h>
#include <linux/skbuff.h>
#include <linux/notifier.h>
#include <net/checksum.h>
#include <net/icmp.h>
#include <net/ip.h>
#include <net/ipv6.h>
#include <net/ip6_route.h>
#include <net/route.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter/xt_TEE.h>

#if IS_ENABLED(CONFIG_NF_CONNTRACK)
#	define WITH_CONNTRACK 1
#	include <net/netfilter/nf_conntrack.h>
#endif

struct xt_tee_priv {
	struct notifier_block	notifier;
=======
 */
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/route.h>
#include <linux/netfilter/x_tables.h>
#include <net/net_namespace.h>
#include <net/netns/generic.h>
#include <net/route.h>
#include <net/netfilter/ipv4/nf_dup_ipv4.h>
#include <net/netfilter/ipv6/nf_dup_ipv6.h>
#include <linux/netfilter/xt_TEE.h>

struct xt_tee_priv {
	struct list_head	list;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct xt_tee_tginfo	*tginfo;
	int			oif;
};

<<<<<<< HEAD
static const union nf_inet_addr tee_zero_address;
static DEFINE_PER_CPU(bool, tee_active);

static struct net *pick_net(struct sk_buff *skb)
{
#ifdef CONFIG_NET_NS
	const struct dst_entry *dst;

	if (skb->dev != NULL)
		return dev_net(skb->dev);
	dst = skb_dst(skb);
	if (dst != NULL && dst->dev != NULL)
		return dev_net(dst->dev);
#endif
	return &init_net;
}

static bool
tee_tg_route4(struct sk_buff *skb, const struct xt_tee_tginfo *info)
{
	const struct iphdr *iph = ip_hdr(skb);
	struct net *net = pick_net(skb);
	struct rtable *rt;
	struct flowi4 fl4;

	memset(&fl4, 0, sizeof(fl4));
	if (info->priv) {
		if (info->priv->oif == -1)
			return false;
		fl4.flowi4_oif = info->priv->oif;
	}
	fl4.daddr = info->gw.ip;
	fl4.flowi4_tos = RT_TOS(iph->tos);
	fl4.flowi4_scope = RT_SCOPE_UNIVERSE;
	rt = ip_route_output_key(net, &fl4);
	if (IS_ERR(rt))
		return false;

	skb_dst_drop(skb);
	skb_dst_set(skb, &rt->dst);
	skb->dev      = rt->dst.dev;
	skb->protocol = htons(ETH_P_IP);
	return true;
}
=======
static unsigned int tee_net_id __read_mostly;
static const union nf_inet_addr tee_zero_address;

struct tee_net {
	struct list_head priv_list;
	/* lock protects the priv_list */
	struct mutex lock;
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static unsigned int
tee_tg4(struct sk_buff *skb, const struct xt_action_param *par)
{
	const struct xt_tee_tginfo *info = par->targinfo;
<<<<<<< HEAD
	struct iphdr *iph;

	if (percpu_read(tee_active))
		return XT_CONTINUE;
	/*
	 * Copy the skb, and route the copy. Will later return %XT_CONTINUE for
	 * the original skb, which should continue on its way as if nothing has
	 * happened. The copy should be independently delivered to the TEE
	 * --gateway.
	 */
	skb = pskb_copy(skb, GFP_ATOMIC);
	if (skb == NULL)
		return XT_CONTINUE;

#ifdef WITH_CONNTRACK
	/* Avoid counting cloned packets towards the original connection. */
	nf_conntrack_put(skb->nfct);
	skb->nfct     = &nf_ct_untracked_get()->ct_general;
	skb->nfctinfo = IP_CT_NEW;
	nf_conntrack_get(skb->nfct);
#endif
	/*
	 * If we are in PREROUTING/INPUT, the checksum must be recalculated
	 * since the length could have changed as a result of defragmentation.
	 *
	 * We also decrease the TTL to mitigate potential TEE loops
	 * between two hosts.
	 *
	 * Set %IP_DF so that the original source is notified of a potentially
	 * decreased MTU on the clone route. IPv6 does this too.
	 */
	iph = ip_hdr(skb);
	iph->frag_off |= htons(IP_DF);
	if (par->hooknum == NF_INET_PRE_ROUTING ||
	    par->hooknum == NF_INET_LOCAL_IN)
		--iph->ttl;
	ip_send_check(iph);

	if (tee_tg_route4(skb, info)) {
		percpu_write(tee_active, true);
		ip_local_out(skb);
		percpu_write(tee_active, false);
	} else {
		kfree_skb(skb);
	}
	return XT_CONTINUE;
}

#if IS_ENABLED(CONFIG_IPV6)
static bool
tee_tg_route6(struct sk_buff *skb, const struct xt_tee_tginfo *info)
{
	const struct ipv6hdr *iph = ipv6_hdr(skb);
	struct net *net = pick_net(skb);
	struct dst_entry *dst;
	struct flowi6 fl6;

	memset(&fl6, 0, sizeof(fl6));
	if (info->priv) {
		if (info->priv->oif == -1)
			return false;
		fl6.flowi6_oif = info->priv->oif;
	}
	fl6.daddr = info->gw.in6;
	fl6.flowlabel = ((iph->flow_lbl[0] & 0xF) << 16) |
			   (iph->flow_lbl[1] << 8) | iph->flow_lbl[2];
	dst = ip6_route_output(net, NULL, &fl6);
	if (dst->error) {
		dst_release(dst);
		return false;
	}
	skb_dst_drop(skb);
	skb_dst_set(skb, dst);
	skb->dev      = dst->dev;
	skb->protocol = htons(ETH_P_IPV6);
	return true;
}

=======
	int oif = info->priv ? info->priv->oif : 0;

	nf_dup_ipv4(xt_net(par), skb, xt_hooknum(par), &info->gw.in, oif);

	return XT_CONTINUE;
}

#if IS_ENABLED(CONFIG_IP6_NF_IPTABLES)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static unsigned int
tee_tg6(struct sk_buff *skb, const struct xt_action_param *par)
{
	const struct xt_tee_tginfo *info = par->targinfo;
<<<<<<< HEAD

	if (percpu_read(tee_active))
		return XT_CONTINUE;
	skb = pskb_copy(skb, GFP_ATOMIC);
	if (skb == NULL)
		return XT_CONTINUE;

#ifdef WITH_CONNTRACK
	nf_conntrack_put(skb->nfct);
	skb->nfct     = &nf_ct_untracked_get()->ct_general;
	skb->nfctinfo = IP_CT_NEW;
	nf_conntrack_get(skb->nfct);
#endif
	if (par->hooknum == NF_INET_PRE_ROUTING ||
	    par->hooknum == NF_INET_LOCAL_IN) {
		struct ipv6hdr *iph = ipv6_hdr(skb);
		--iph->hop_limit;
	}
	if (tee_tg_route6(skb, info)) {
		percpu_write(tee_active, true);
		ip6_local_out(skb);
		percpu_write(tee_active, false);
	} else {
		kfree_skb(skb);
	}
=======
	int oif = info->priv ? info->priv->oif : 0;

	nf_dup_ipv6(xt_net(par), skb, xt_hooknum(par), &info->gw.in6, oif);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return XT_CONTINUE;
}
#endif

static int tee_netdev_event(struct notifier_block *this, unsigned long event,
			    void *ptr)
{
<<<<<<< HEAD
	struct net_device *dev = ptr;
	struct xt_tee_priv *priv;

	priv = container_of(this, struct xt_tee_priv, notifier);
	switch (event) {
	case NETDEV_REGISTER:
		if (!strcmp(dev->name, priv->tginfo->oif))
			priv->oif = dev->ifindex;
		break;
	case NETDEV_UNREGISTER:
		if (dev->ifindex == priv->oif)
			priv->oif = -1;
		break;
	case NETDEV_CHANGENAME:
		if (!strcmp(dev->name, priv->tginfo->oif))
			priv->oif = dev->ifindex;
		else if (dev->ifindex == priv->oif)
			priv->oif = -1;
		break;
	}
=======
	struct net_device *dev = netdev_notifier_info_to_dev(ptr);
	struct net *net = dev_net(dev);
	struct tee_net *tn = net_generic(net, tee_net_id);
	struct xt_tee_priv *priv;

	mutex_lock(&tn->lock);
	list_for_each_entry(priv, &tn->priv_list, list) {
		switch (event) {
		case NETDEV_REGISTER:
			if (!strcmp(dev->name, priv->tginfo->oif))
				priv->oif = dev->ifindex;
			break;
		case NETDEV_UNREGISTER:
			if (dev->ifindex == priv->oif)
				priv->oif = -1;
			break;
		case NETDEV_CHANGENAME:
			if (!strcmp(dev->name, priv->tginfo->oif))
				priv->oif = dev->ifindex;
			else if (dev->ifindex == priv->oif)
				priv->oif = -1;
			break;
		}
	}
	mutex_unlock(&tn->lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return NOTIFY_DONE;
}

static int tee_tg_check(const struct xt_tgchk_param *par)
{
<<<<<<< HEAD
=======
	struct tee_net *tn = net_generic(par->net, tee_net_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct xt_tee_tginfo *info = par->targinfo;
	struct xt_tee_priv *priv;

	/* 0.0.0.0 and :: not allowed */
	if (memcmp(&info->gw, &tee_zero_address,
		   sizeof(tee_zero_address)) == 0)
		return -EINVAL;

	if (info->oif[0]) {
<<<<<<< HEAD
=======
		struct net_device *dev;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (info->oif[sizeof(info->oif)-1] != '\0')
			return -EINVAL;

		priv = kzalloc(sizeof(*priv), GFP_KERNEL);
		if (priv == NULL)
			return -ENOMEM;

		priv->tginfo  = info;
		priv->oif     = -1;
<<<<<<< HEAD
		priv->notifier.notifier_call = tee_netdev_event;
		info->priv    = priv;

		register_netdevice_notifier(&priv->notifier);
	} else
		info->priv = NULL;

=======
		info->priv    = priv;

		dev = dev_get_by_name(par->net, info->oif);
		if (dev) {
			priv->oif = dev->ifindex;
			dev_put(dev);
		}
		mutex_lock(&tn->lock);
		list_add(&priv->list, &tn->priv_list);
		mutex_unlock(&tn->lock);
	} else
		info->priv = NULL;

	static_key_slow_inc(&xt_tee_enabled);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static void tee_tg_destroy(const struct xt_tgdtor_param *par)
{
<<<<<<< HEAD
	struct xt_tee_tginfo *info = par->targinfo;

	if (info->priv) {
		unregister_netdevice_notifier(&info->priv->notifier);
		kfree(info->priv);
	}
=======
	struct tee_net *tn = net_generic(par->net, tee_net_id);
	struct xt_tee_tginfo *info = par->targinfo;

	if (info->priv) {
		mutex_lock(&tn->lock);
		list_del(&info->priv->list);
		mutex_unlock(&tn->lock);
		kfree(info->priv);
	}
	static_key_slow_dec(&xt_tee_enabled);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct xt_target tee_tg_reg[] __read_mostly = {
	{
		.name       = "TEE",
		.revision   = 1,
		.family     = NFPROTO_IPV4,
		.target     = tee_tg4,
		.targetsize = sizeof(struct xt_tee_tginfo),
<<<<<<< HEAD
=======
		.usersize   = offsetof(struct xt_tee_tginfo, priv),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.checkentry = tee_tg_check,
		.destroy    = tee_tg_destroy,
		.me         = THIS_MODULE,
	},
<<<<<<< HEAD
#if IS_ENABLED(CONFIG_IPV6)
=======
#if IS_ENABLED(CONFIG_IP6_NF_IPTABLES)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{
		.name       = "TEE",
		.revision   = 1,
		.family     = NFPROTO_IPV6,
		.target     = tee_tg6,
		.targetsize = sizeof(struct xt_tee_tginfo),
<<<<<<< HEAD
=======
		.usersize   = offsetof(struct xt_tee_tginfo, priv),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.checkentry = tee_tg_check,
		.destroy    = tee_tg_destroy,
		.me         = THIS_MODULE,
	},
#endif
};

<<<<<<< HEAD
static int __init tee_tg_init(void)
{
	return xt_register_targets(tee_tg_reg, ARRAY_SIZE(tee_tg_reg));
=======
static int __net_init tee_net_init(struct net *net)
{
	struct tee_net *tn = net_generic(net, tee_net_id);

	INIT_LIST_HEAD(&tn->priv_list);
	mutex_init(&tn->lock);
	return 0;
}

static struct pernet_operations tee_net_ops = {
	.init = tee_net_init,
	.id   = &tee_net_id,
	.size = sizeof(struct tee_net),
};

static struct notifier_block tee_netdev_notifier = {
	.notifier_call = tee_netdev_event,
};

static int __init tee_tg_init(void)
{
	int ret;

	ret = register_pernet_subsys(&tee_net_ops);
	if (ret < 0)
		return ret;

	ret = xt_register_targets(tee_tg_reg, ARRAY_SIZE(tee_tg_reg));
	if (ret < 0)
		goto cleanup_subsys;

	ret = register_netdevice_notifier(&tee_netdev_notifier);
	if (ret < 0)
		goto unregister_targets;

	return 0;

unregister_targets:
	xt_unregister_targets(tee_tg_reg, ARRAY_SIZE(tee_tg_reg));
cleanup_subsys:
	unregister_pernet_subsys(&tee_net_ops);
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void __exit tee_tg_exit(void)
{
<<<<<<< HEAD
	xt_unregister_targets(tee_tg_reg, ARRAY_SIZE(tee_tg_reg));
=======
	unregister_netdevice_notifier(&tee_netdev_notifier);
	xt_unregister_targets(tee_tg_reg, ARRAY_SIZE(tee_tg_reg));
	unregister_pernet_subsys(&tee_net_ops);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

module_init(tee_tg_init);
module_exit(tee_tg_exit);
MODULE_AUTHOR("Sebastian Claßen <sebastian.classen@freenet.ag>");
MODULE_AUTHOR("Jan Engelhardt <jengelh@medozas.de>");
MODULE_DESCRIPTION("Xtables: Reroute packet copy");
MODULE_LICENSE("GPL");
MODULE_ALIAS("ipt_TEE");
MODULE_ALIAS("ip6t_TEE");
