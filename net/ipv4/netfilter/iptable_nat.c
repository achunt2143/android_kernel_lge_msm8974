<<<<<<< HEAD
/* (C) 1999-2001 Paul `Rusty' Russell
 * (C) 2002-2006 Netfilter Core Team <coreteam@netfilter.org>
 * (C) 2011 Patrick McHardy <kaber@trash.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
// SPDX-License-Identifier: GPL-2.0-only
/* (C) 1999-2001 Paul `Rusty' Russell
 * (C) 2002-2006 Netfilter Core Team <coreteam@netfilter.org>
 * (C) 2011 Patrick McHardy <kaber@trash.net>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/module.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <linux/ip.h>
#include <net/ip.h>

#include <net/netfilter/nf_nat.h>
<<<<<<< HEAD
#include <net/netfilter/nf_nat_core.h>
#include <net/netfilter/nf_nat_l3proto.h>
=======

struct iptable_nat_pernet {
	struct nf_hook_ops *nf_nat_ops;
};

static unsigned int iptable_nat_net_id __read_mostly;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static const struct xt_table nf_nat_ipv4_table = {
	.name		= "nat",
	.valid_hooks	= (1 << NF_INET_PRE_ROUTING) |
			  (1 << NF_INET_POST_ROUTING) |
			  (1 << NF_INET_LOCAL_OUT) |
			  (1 << NF_INET_LOCAL_IN),
	.me		= THIS_MODULE,
	.af		= NFPROTO_IPV4,
};

<<<<<<< HEAD
static unsigned int alloc_null_binding(struct nf_conn *ct, unsigned int hooknum)
{
	/* Force range to this IP; let proto decide mapping for
	 * per-proto parts (hence not IP_NAT_RANGE_PROTO_SPECIFIED).
	 */
	struct nf_nat_range range;

	range.flags = 0;
	pr_debug("Allocating NULL binding for %p (%pI4)\n", ct,
		 HOOK2MANIP(hooknum) == NF_NAT_MANIP_SRC ?
		 &ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip :
		 &ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip);

	return nf_nat_setup_info(ct, &range, HOOK2MANIP(hooknum));
}

static unsigned int nf_nat_rule_find(struct sk_buff *skb, unsigned int hooknum,
				     const struct net_device *in,
				     const struct net_device *out,
				     struct nf_conn *ct)
{
	struct net *net = nf_ct_net(ct);
	unsigned int ret;

	ret = ipt_do_table(skb, hooknum, in, out, net->ipv4.nat_table);
	if (ret == NF_ACCEPT) {
		if (!nf_nat_initialized(ct, HOOK2MANIP(hooknum)))
			ret = alloc_null_binding(ct, hooknum);
	}
	return ret;
}

static unsigned int
nf_nat_ipv4_fn(unsigned int hooknum,
	       struct sk_buff *skb,
	       const struct net_device *in,
	       const struct net_device *out,
	       int (*okfn)(struct sk_buff *))
{
	struct nf_conn *ct;
	enum ip_conntrack_info ctinfo;
	struct nf_conn_nat *nat;
	/* maniptype == SRC for postrouting. */
	enum nf_nat_manip_type maniptype = HOOK2MANIP(hooknum);

	/* We never see fragments: conntrack defrags on pre-routing
	 * and local-out, and nf_nat_out protects post-routing.
	 */
	NF_CT_ASSERT(!ip_is_fragment(ip_hdr(skb)));

	ct = nf_ct_get(skb, &ctinfo);
	/* Can't track?  It's not due to stress, or conntrack would
	 * have dropped it.  Hence it's the user's responsibilty to
	 * packet filter it out, or implement conntrack/NAT for that
	 * protocol. 8) --RR
	 */
	if (!ct)
		return NF_ACCEPT;

	/* Don't try to NAT if this packet is not conntracked */
	if (nf_ct_is_untracked(ct))
		return NF_ACCEPT;

	nat = nfct_nat(ct);
	if (!nat) {
		/* NAT module was loaded late. */
		if (nf_ct_is_confirmed(ct))
			return NF_ACCEPT;
		nat = nf_ct_ext_add(ct, NF_CT_EXT_NAT, GFP_ATOMIC);
		if (nat == NULL) {
			pr_debug("failed to add NAT extension\n");
			return NF_ACCEPT;
		}
	}

	switch (ctinfo) {
	case IP_CT_RELATED:
	case IP_CT_RELATED_REPLY:
		if (ip_hdr(skb)->protocol == IPPROTO_ICMP) {
			if (!nf_nat_icmp_reply_translation(skb, ct, ctinfo,
							   hooknum))
				return NF_DROP;
			else
				return NF_ACCEPT;
		}
		/* Fall thru... (Only ICMPs can be IP_CT_IS_REPLY) */
	case IP_CT_NEW:
		/* Seen it before?  This can happen for loopback, retrans,
		 * or local packets.
		 */
		if (!nf_nat_initialized(ct, maniptype)) {
			unsigned int ret;

			ret = nf_nat_rule_find(skb, hooknum, in, out, ct);
			if (ret != NF_ACCEPT)
				return ret;
		} else {
			pr_debug("Already setup manip %s for ct %p\n",
				 maniptype == NF_NAT_MANIP_SRC ? "SRC" : "DST",
				 ct);
			if (nf_nat_oif_changed(hooknum, ctinfo, nat, out))
				goto oif_changed;
		}
		break;

	default:
		/* ESTABLISHED */
		NF_CT_ASSERT(ctinfo == IP_CT_ESTABLISHED ||
			     ctinfo == IP_CT_ESTABLISHED_REPLY);
		if (nf_nat_oif_changed(hooknum, ctinfo, nat, out))
			goto oif_changed;
	}

	return nf_nat_packet(ct, ctinfo, hooknum, skb);

oif_changed:
	nf_ct_kill_acct(ct, ctinfo, skb);
	return NF_DROP;
}

static unsigned int
nf_nat_ipv4_in(unsigned int hooknum,
	       struct sk_buff *skb,
	       const struct net_device *in,
	       const struct net_device *out,
	       int (*okfn)(struct sk_buff *))
{
	unsigned int ret;
	__be32 daddr = ip_hdr(skb)->daddr;

	ret = nf_nat_ipv4_fn(hooknum, skb, in, out, okfn);
	if (ret != NF_DROP && ret != NF_STOLEN &&
	    daddr != ip_hdr(skb)->daddr)
		skb_dst_drop(skb);

	return ret;
}

static unsigned int
nf_nat_ipv4_out(unsigned int hooknum,
		struct sk_buff *skb,
		const struct net_device *in,
		const struct net_device *out,
		int (*okfn)(struct sk_buff *))
{
#ifdef CONFIG_XFRM
	const struct nf_conn *ct;
	enum ip_conntrack_info ctinfo;
#endif
	unsigned int ret;

	/* root is playing with raw sockets. */
	if (skb->len < sizeof(struct iphdr) ||
	    ip_hdrlen(skb) < sizeof(struct iphdr))
		return NF_ACCEPT;

	ret = nf_nat_ipv4_fn(hooknum, skb, in, out, okfn);
#ifdef CONFIG_XFRM
	if (ret != NF_DROP && ret != NF_STOLEN &&
	    !(IPCB(skb)->flags & IPSKB_XFRM_TRANSFORMED) &&
	    (ct = nf_ct_get(skb, &ctinfo)) != NULL) {
		enum ip_conntrack_dir dir = CTINFO2DIR(ctinfo);

		if ((ct->tuplehash[dir].tuple.src.u3.ip !=
		     ct->tuplehash[!dir].tuple.dst.u3.ip) ||
		    (ct->tuplehash[dir].tuple.src.u.all !=
		     ct->tuplehash[!dir].tuple.dst.u.all))
			if (nf_xfrm_me_harder(skb, AF_INET) < 0)
				ret = NF_DROP;
	}
#endif
	return ret;
}

static unsigned int
nf_nat_ipv4_local_fn(unsigned int hooknum,
		     struct sk_buff *skb,
		     const struct net_device *in,
		     const struct net_device *out,
		     int (*okfn)(struct sk_buff *))
{
	const struct nf_conn *ct;
	enum ip_conntrack_info ctinfo;
	unsigned int ret;

	/* root is playing with raw sockets. */
	if (skb->len < sizeof(struct iphdr) ||
	    ip_hdrlen(skb) < sizeof(struct iphdr))
		return NF_ACCEPT;

	ret = nf_nat_ipv4_fn(hooknum, skb, in, out, okfn);
	if (ret != NF_DROP && ret != NF_STOLEN &&
	    (ct = nf_ct_get(skb, &ctinfo)) != NULL) {
		enum ip_conntrack_dir dir = CTINFO2DIR(ctinfo);

		if (ct->tuplehash[dir].tuple.dst.u3.ip !=
		    ct->tuplehash[!dir].tuple.src.u3.ip) {
			if (ip_route_me_harder(skb, RTN_UNSPEC))
				ret = NF_DROP;
		}
#ifdef CONFIG_XFRM
		else if (!(IPCB(skb)->flags & IPSKB_XFRM_TRANSFORMED) &&
			 ct->tuplehash[dir].tuple.dst.u.all !=
			 ct->tuplehash[!dir].tuple.src.u.all)
			if (nf_xfrm_me_harder(skb, AF_INET) < 0)
				ret = NF_DROP;
#endif
	}
	return ret;
}

static struct nf_hook_ops nf_nat_ipv4_ops[] __read_mostly = {
	/* Before packet filtering, change destination */
	{
		.hook		= nf_nat_ipv4_in,
		.owner		= THIS_MODULE,
=======
static const struct nf_hook_ops nf_nat_ipv4_ops[] = {
	{
		.hook		= ipt_do_table,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.pf		= NFPROTO_IPV4,
		.hooknum	= NF_INET_PRE_ROUTING,
		.priority	= NF_IP_PRI_NAT_DST,
	},
<<<<<<< HEAD
	/* After packet filtering, change source */
	{
		.hook		= nf_nat_ipv4_out,
		.owner		= THIS_MODULE,
=======
	{
		.hook		= ipt_do_table,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.pf		= NFPROTO_IPV4,
		.hooknum	= NF_INET_POST_ROUTING,
		.priority	= NF_IP_PRI_NAT_SRC,
	},
<<<<<<< HEAD
	/* Before packet filtering, change destination */
	{
		.hook		= nf_nat_ipv4_local_fn,
		.owner		= THIS_MODULE,
=======
	{
		.hook		= ipt_do_table,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.pf		= NFPROTO_IPV4,
		.hooknum	= NF_INET_LOCAL_OUT,
		.priority	= NF_IP_PRI_NAT_DST,
	},
<<<<<<< HEAD
	/* After packet filtering, change source */
	{
		.hook		= nf_nat_ipv4_fn,
		.owner		= THIS_MODULE,
=======
	{
		.hook		= ipt_do_table,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.pf		= NFPROTO_IPV4,
		.hooknum	= NF_INET_LOCAL_IN,
		.priority	= NF_IP_PRI_NAT_SRC,
	},
};

<<<<<<< HEAD
static int __net_init iptable_nat_net_init(struct net *net)
{
	struct ipt_replace *repl;
=======
static int ipt_nat_register_lookups(struct net *net)
{
	struct iptable_nat_pernet *xt_nat_net;
	struct nf_hook_ops *ops;
	struct xt_table *table;
	int i, ret;

	xt_nat_net = net_generic(net, iptable_nat_net_id);
	table = xt_find_table(net, NFPROTO_IPV4, "nat");
	if (WARN_ON_ONCE(!table))
		return -ENOENT;

	ops = kmemdup(nf_nat_ipv4_ops, sizeof(nf_nat_ipv4_ops), GFP_KERNEL);
	if (!ops)
		return -ENOMEM;

	for (i = 0; i < ARRAY_SIZE(nf_nat_ipv4_ops); i++) {
		ops[i].priv = table;
		ret = nf_nat_ipv4_register_fn(net, &ops[i]);
		if (ret) {
			while (i)
				nf_nat_ipv4_unregister_fn(net, &ops[--i]);

			kfree(ops);
			return ret;
		}
	}

	xt_nat_net->nf_nat_ops = ops;
	return 0;
}

static void ipt_nat_unregister_lookups(struct net *net)
{
	struct iptable_nat_pernet *xt_nat_net = net_generic(net, iptable_nat_net_id);
	struct nf_hook_ops *ops = xt_nat_net->nf_nat_ops;
	int i;

	if (!ops)
		return;

	for (i = 0; i < ARRAY_SIZE(nf_nat_ipv4_ops); i++)
		nf_nat_ipv4_unregister_fn(net, &ops[i]);

	kfree(ops);
}

static int iptable_nat_table_init(struct net *net)
{
	struct ipt_replace *repl;
	int ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	repl = ipt_alloc_initial_table(&nf_nat_ipv4_table);
	if (repl == NULL)
		return -ENOMEM;
<<<<<<< HEAD
	net->ipv4.nat_table = ipt_register_table(net, &nf_nat_ipv4_table, repl);
	kfree(repl);
	if (IS_ERR(net->ipv4.nat_table))
		return PTR_ERR(net->ipv4.nat_table);
	return 0;
=======

	ret = ipt_register_table(net, &nf_nat_ipv4_table, repl, NULL);
	if (ret < 0) {
		kfree(repl);
		return ret;
	}

	ret = ipt_nat_register_lookups(net);
	if (ret < 0)
		ipt_unregister_table_exit(net, "nat");

	kfree(repl);
	return ret;
}

static void __net_exit iptable_nat_net_pre_exit(struct net *net)
{
	ipt_nat_unregister_lookups(net);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void __net_exit iptable_nat_net_exit(struct net *net)
{
<<<<<<< HEAD
	ipt_unregister_table(net, net->ipv4.nat_table);
}

static struct pernet_operations iptable_nat_net_ops = {
	.init	= iptable_nat_net_init,
	.exit	= iptable_nat_net_exit,
=======
	ipt_unregister_table_exit(net, "nat");
}

static struct pernet_operations iptable_nat_net_ops = {
	.pre_exit = iptable_nat_net_pre_exit,
	.exit	= iptable_nat_net_exit,
	.id	= &iptable_nat_net_id,
	.size	= sizeof(struct iptable_nat_pernet),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static int __init iptable_nat_init(void)
{
<<<<<<< HEAD
	int err;

	err = register_pernet_subsys(&iptable_nat_net_ops);
	if (err < 0)
		goto err1;

	err = nf_register_hooks(nf_nat_ipv4_ops, ARRAY_SIZE(nf_nat_ipv4_ops));
	if (err < 0)
		goto err2;
	return 0;

err2:
	unregister_pernet_subsys(&iptable_nat_net_ops);
err1:
	return err;
=======
	int ret = xt_register_template(&nf_nat_ipv4_table,
				       iptable_nat_table_init);

	if (ret < 0)
		return ret;

	ret = register_pernet_subsys(&iptable_nat_net_ops);
	if (ret < 0) {
		xt_unregister_template(&nf_nat_ipv4_table);
		return ret;
	}

	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void __exit iptable_nat_exit(void)
{
<<<<<<< HEAD
	nf_unregister_hooks(nf_nat_ipv4_ops, ARRAY_SIZE(nf_nat_ipv4_ops));
	unregister_pernet_subsys(&iptable_nat_net_ops);
=======
	unregister_pernet_subsys(&iptable_nat_net_ops);
	xt_unregister_template(&nf_nat_ipv4_table);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

module_init(iptable_nat_init);
module_exit(iptable_nat_exit);

MODULE_LICENSE("GPL");
<<<<<<< HEAD
=======
MODULE_DESCRIPTION("iptables legacy nat table");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
