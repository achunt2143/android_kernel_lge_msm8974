<<<<<<< HEAD
/* (C) 1999-2001 Paul `Rusty' Russell
 * (C) 2002-2004 Netfilter Core Team <coreteam@netfilter.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
// SPDX-License-Identifier: GPL-2.0-only
/* (C) 1999-2001 Paul `Rusty' Russell
 * (C) 2002-2004 Netfilter Core Team <coreteam@netfilter.org>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/types.h>
#include <linux/ipv6.h>
#include <linux/in6.h>
#include <linux/netfilter.h>
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/icmp.h>
<<<<<<< HEAD
#include <linux/sysctl.h>
#include <net/ipv6.h>
#include <net/inet_frag.h>

#include <linux/netfilter_ipv6.h>
#include <linux/netfilter_bridge.h>
#if defined(CONFIG_NF_CONNTRACK) || defined(CONFIG_NF_CONNTRACK_MODULE)
#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_helper.h>
#include <net/netfilter/nf_conntrack_l4proto.h>
#include <net/netfilter/nf_conntrack_l3proto.h>
=======
#include <linux/rcupdate.h>
#include <linux/sysctl.h>
#include <net/ipv6_frag.h>

#include <linux/netfilter_ipv6.h>
#include <linux/netfilter_bridge.h>
#if IS_ENABLED(CONFIG_NF_CONNTRACK)
#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_helper.h>
#include <net/netfilter/nf_conntrack_l4proto.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <net/netfilter/nf_conntrack_core.h>
#include <net/netfilter/ipv6/nf_conntrack_ipv6.h>
#endif
#include <net/netfilter/nf_conntrack_zones.h>
#include <net/netfilter/ipv6/nf_defrag_ipv6.h>

<<<<<<< HEAD
static enum ip6_defrag_users nf_ct6_defrag_user(unsigned int hooknum,
						struct sk_buff *skb)
{
	u16 zone = NF_CT_DEFAULT_ZONE;

#if defined(CONFIG_NF_CONNTRACK) || defined(CONFIG_NF_CONNTRACK_MODULE)
	if (skb->nfct)
		zone = nf_ct_zone((struct nf_conn *)skb->nfct);
#endif

#ifdef CONFIG_BRIDGE_NETFILTER
	if (skb->nf_bridge &&
	    skb->nf_bridge->mask & BRNF_NF_BRIDGE_PREROUTING)
		return IP6_DEFRAG_CONNTRACK_BRIDGE_IN + zone;
#endif
	if (hooknum == NF_INET_PRE_ROUTING)
		return IP6_DEFRAG_CONNTRACK_IN + zone;
	else
		return IP6_DEFRAG_CONNTRACK_OUT + zone;

}

static unsigned int ipv6_defrag(unsigned int hooknum,
				struct sk_buff *skb,
				const struct net_device *in,
				const struct net_device *out,
				int (*okfn)(struct sk_buff *))
{
	struct sk_buff *reasm;

#if defined(CONFIG_NF_CONNTRACK) || defined(CONFIG_NF_CONNTRACK_MODULE)
	/* Previously seen (loopback)?	*/
	if (skb->nfct && !nf_ct_is_template((struct nf_conn *)skb->nfct))
		return NF_ACCEPT;
#endif

	reasm = nf_ct_frag6_gather(skb, nf_ct6_defrag_user(hooknum, skb));
	/* queued */
	if (reasm == NULL)
		return NF_STOLEN;

	/* error occurred or not fragmented */
	if (reasm == skb)
		return NF_ACCEPT;

	nf_ct_frag6_output(hooknum, reasm, (struct net_device *)in,
			   (struct net_device *)out, okfn);

	return NF_STOLEN;
}

static struct nf_hook_ops ipv6_defrag_ops[] = {
	{
		.hook		= ipv6_defrag,
		.owner		= THIS_MODULE,
=======
static DEFINE_MUTEX(defrag6_mutex);

static enum ip6_defrag_users nf_ct6_defrag_user(unsigned int hooknum,
						struct sk_buff *skb)
{
	u16 zone_id = NF_CT_DEFAULT_ZONE_ID;
#if IS_ENABLED(CONFIG_NF_CONNTRACK)
	if (skb_nfct(skb)) {
		enum ip_conntrack_info ctinfo;
		const struct nf_conn *ct = nf_ct_get(skb, &ctinfo);

		zone_id = nf_ct_zone_id(nf_ct_zone(ct), CTINFO2DIR(ctinfo));
	}
#endif
	if (nf_bridge_in_prerouting(skb))
		return IP6_DEFRAG_CONNTRACK_BRIDGE_IN + zone_id;

	if (hooknum == NF_INET_PRE_ROUTING)
		return IP6_DEFRAG_CONNTRACK_IN + zone_id;
	else
		return IP6_DEFRAG_CONNTRACK_OUT + zone_id;
}

static unsigned int ipv6_defrag(void *priv,
				struct sk_buff *skb,
				const struct nf_hook_state *state)
{
	int err;

#if IS_ENABLED(CONFIG_NF_CONNTRACK)
	/* Previously seen (loopback)?	*/
	if (skb_nfct(skb) && !nf_ct_is_template((struct nf_conn *)skb_nfct(skb)))
		return NF_ACCEPT;

	if (skb->_nfct == IP_CT_UNTRACKED)
		return NF_ACCEPT;
#endif

	err = nf_ct_frag6_gather(state->net, skb,
				 nf_ct6_defrag_user(state->hook, skb));
	/* queued */
	if (err == -EINPROGRESS)
		return NF_STOLEN;

	return err == 0 ? NF_ACCEPT : NF_DROP;
}

static const struct nf_hook_ops ipv6_defrag_ops[] = {
	{
		.hook		= ipv6_defrag,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.pf		= NFPROTO_IPV6,
		.hooknum	= NF_INET_PRE_ROUTING,
		.priority	= NF_IP6_PRI_CONNTRACK_DEFRAG,
	},
	{
		.hook		= ipv6_defrag,
<<<<<<< HEAD
		.owner		= THIS_MODULE,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.pf		= NFPROTO_IPV6,
		.hooknum	= NF_INET_LOCAL_OUT,
		.priority	= NF_IP6_PRI_CONNTRACK_DEFRAG,
	},
};

<<<<<<< HEAD
=======
static void __net_exit defrag6_net_exit(struct net *net)
{
	if (net->nf.defrag_ipv6_users) {
		nf_unregister_net_hooks(net, ipv6_defrag_ops,
					ARRAY_SIZE(ipv6_defrag_ops));
		net->nf.defrag_ipv6_users = 0;
	}
}

static const struct nf_defrag_hook defrag_hook = {
	.owner = THIS_MODULE,
	.enable = nf_defrag_ipv6_enable,
	.disable = nf_defrag_ipv6_disable,
};

static struct pernet_operations defrag6_net_ops = {
	.exit = defrag6_net_exit,
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int __init nf_defrag_init(void)
{
	int ret = 0;

	ret = nf_ct_frag6_init();
	if (ret < 0) {
		pr_err("nf_defrag_ipv6: can't initialize frag6.\n");
		return ret;
	}
<<<<<<< HEAD
	ret = nf_register_hooks(ipv6_defrag_ops, ARRAY_SIZE(ipv6_defrag_ops));
	if (ret < 0) {
		pr_err("nf_defrag_ipv6: can't register hooks\n");
		goto cleanup_frag6;
	}
=======
	ret = register_pernet_subsys(&defrag6_net_ops);
	if (ret < 0) {
		pr_err("nf_defrag_ipv6: can't register pernet ops\n");
		goto cleanup_frag6;
	}

	rcu_assign_pointer(nf_defrag_v6_hook, &defrag_hook);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;

cleanup_frag6:
	nf_ct_frag6_cleanup();
	return ret;

}

static void __exit nf_defrag_fini(void)
{
<<<<<<< HEAD
	nf_unregister_hooks(ipv6_defrag_ops, ARRAY_SIZE(ipv6_defrag_ops));
	nf_ct_frag6_cleanup();
}

void nf_defrag_ipv6_enable(void)
{
}
EXPORT_SYMBOL_GPL(nf_defrag_ipv6_enable);

=======
	rcu_assign_pointer(nf_defrag_v6_hook, NULL);
	unregister_pernet_subsys(&defrag6_net_ops);
	nf_ct_frag6_cleanup();
}

int nf_defrag_ipv6_enable(struct net *net)
{
	int err = 0;

	mutex_lock(&defrag6_mutex);
	if (net->nf.defrag_ipv6_users == UINT_MAX) {
		err = -EOVERFLOW;
		goto out_unlock;
	}

	if (net->nf.defrag_ipv6_users) {
		net->nf.defrag_ipv6_users++;
		goto out_unlock;
	}

	err = nf_register_net_hooks(net, ipv6_defrag_ops,
				    ARRAY_SIZE(ipv6_defrag_ops));
	if (err == 0)
		net->nf.defrag_ipv6_users = 1;

 out_unlock:
	mutex_unlock(&defrag6_mutex);
	return err;
}
EXPORT_SYMBOL_GPL(nf_defrag_ipv6_enable);

void nf_defrag_ipv6_disable(struct net *net)
{
	mutex_lock(&defrag6_mutex);
	if (net->nf.defrag_ipv6_users) {
		net->nf.defrag_ipv6_users--;
		if (net->nf.defrag_ipv6_users == 0)
			nf_unregister_net_hooks(net, ipv6_defrag_ops,
						ARRAY_SIZE(ipv6_defrag_ops));
	}
	mutex_unlock(&defrag6_mutex);
}
EXPORT_SYMBOL_GPL(nf_defrag_ipv6_disable);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
module_init(nf_defrag_init);
module_exit(nf_defrag_fini);

MODULE_LICENSE("GPL");
<<<<<<< HEAD
=======
MODULE_DESCRIPTION("IPv6 defragmentation support");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
