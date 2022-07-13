<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *      broadcast connection tracking helper
 *
 *      (c) 2005 Patrick McHardy <kaber@trash.net>
<<<<<<< HEAD
 *
 *      This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/module.h>
#include <linux/ip.h>
#include <net/route.h>
#include <linux/inetdevice.h>
#include <linux/skbuff.h>

#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_helper.h>
#include <net/netfilter/nf_conntrack_expect.h>

int nf_conntrack_broadcast_help(struct sk_buff *skb,
<<<<<<< HEAD
				unsigned int protoff,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				struct nf_conn *ct,
				enum ip_conntrack_info ctinfo,
				unsigned int timeout)
{
<<<<<<< HEAD
=======
	const struct nf_conntrack_helper *helper;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct nf_conntrack_expect *exp;
	struct iphdr *iph = ip_hdr(skb);
	struct rtable *rt = skb_rtable(skb);
	struct in_device *in_dev;
	struct nf_conn_help *help = nfct_help(ct);
	__be32 mask = 0;

	/* we're only interested in locally generated packets */
<<<<<<< HEAD
	if (skb->sk == NULL)
=======
	if (skb->sk == NULL || !net_eq(nf_ct_net(ct), sock_net(skb->sk)))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;
	if (rt == NULL || !(rt->rt_flags & RTCF_BROADCAST))
		goto out;
	if (CTINFO2DIR(ctinfo) != IP_CT_DIR_ORIGINAL)
		goto out;

<<<<<<< HEAD
	rcu_read_lock();
	in_dev = __in_dev_get_rcu(rt->dst.dev);
	if (in_dev != NULL) {
		for_primary_ifa(in_dev) {
=======
	in_dev = __in_dev_get_rcu(rt->dst.dev);
	if (in_dev != NULL) {
		const struct in_ifaddr *ifa;

		in_dev_for_each_ifa_rcu(ifa, in_dev) {
			if (ifa->ifa_flags & IFA_F_SECONDARY)
				continue;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (ifa->ifa_broadcast == iph->daddr) {
				mask = ifa->ifa_mask;
				break;
			}
<<<<<<< HEAD
		} endfor_ifa(in_dev);
	}
	rcu_read_unlock();
=======
		}
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (mask == 0)
		goto out;

	exp = nf_ct_expect_alloc(ct);
	if (exp == NULL)
		goto out;

	exp->tuple                = ct->tuplehash[IP_CT_DIR_REPLY].tuple;
<<<<<<< HEAD
	exp->tuple.src.u.udp.port = help->helper->tuple.src.u.udp.port;
=======

	helper = rcu_dereference(help->helper);
	if (helper)
		exp->tuple.src.u.udp.port = helper->tuple.src.u.udp.port;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	exp->mask.src.u3.ip       = mask;
	exp->mask.src.u.udp.port  = htons(0xFFFF);

	exp->expectfn             = NULL;
	exp->flags                = NF_CT_EXPECT_PERMANENT;
	exp->class		  = NF_CT_EXPECT_CLASS_DEFAULT;
	exp->helper               = NULL;

<<<<<<< HEAD
	nf_ct_expect_related(exp);
=======
	nf_ct_expect_related(exp, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	nf_ct_expect_put(exp);

	nf_ct_refresh(ct, skb, timeout * HZ);
out:
	return NF_ACCEPT;
}
EXPORT_SYMBOL_GPL(nf_conntrack_broadcast_help);

MODULE_LICENSE("GPL");
<<<<<<< HEAD
=======
MODULE_DESCRIPTION("Broadcast connection tracking helper");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
