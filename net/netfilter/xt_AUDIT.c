<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Creates audit record for dropped/accepted packets
 *
 * (C) 2010-2011 Thomas Graf <tgraf@redhat.com>
 * (C) 2010-2011 Red Hat, Inc.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
*/

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/audit.h>
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/if_arp.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter/xt_AUDIT.h>
#include <linux/netfilter_bridge/ebtables.h>
#include <net/ipv6.h>
#include <net/ip.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Thomas Graf <tgraf@redhat.com>");
MODULE_DESCRIPTION("Xtables: creates audit records for dropped/accepted packets");
MODULE_ALIAS("ipt_AUDIT");
MODULE_ALIAS("ip6t_AUDIT");
MODULE_ALIAS("ebt_AUDIT");
MODULE_ALIAS("arpt_AUDIT");

<<<<<<< HEAD
static void audit_proto(struct audit_buffer *ab, struct sk_buff *skb,
			unsigned int proto, unsigned int offset)
{
	switch (proto) {
	case IPPROTO_TCP:
	case IPPROTO_UDP:
	case IPPROTO_UDPLITE: {
		const __be16 *pptr;
		__be16 _ports[2];

		pptr = skb_header_pointer(skb, offset, sizeof(_ports), _ports);
		if (pptr == NULL) {
			audit_log_format(ab, " truncated=1");
			return;
		}

		audit_log_format(ab, " sport=%hu dport=%hu",
				 ntohs(pptr[0]), ntohs(pptr[1]));
		}
		break;

	case IPPROTO_ICMP:
	case IPPROTO_ICMPV6: {
		const u8 *iptr;
		u8 _ih[2];

		iptr = skb_header_pointer(skb, offset, sizeof(_ih), &_ih);
		if (iptr == NULL) {
			audit_log_format(ab, " truncated=1");
			return;
		}

		audit_log_format(ab, " icmptype=%hhu icmpcode=%hhu",
				 iptr[0], iptr[1]);

		}
		break;
	}
}

static void audit_ip4(struct audit_buffer *ab, struct sk_buff *skb)
=======
static bool audit_ip4(struct audit_buffer *ab, struct sk_buff *skb)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct iphdr _iph;
	const struct iphdr *ih;

<<<<<<< HEAD
	ih = skb_header_pointer(skb, 0, sizeof(_iph), &_iph);
	if (!ih) {
		audit_log_format(ab, " truncated=1");
		return;
	}

	audit_log_format(ab, " saddr=%pI4 daddr=%pI4 ipid=%hu proto=%hhu",
		&ih->saddr, &ih->daddr, ntohs(ih->id), ih->protocol);

	if (ntohs(ih->frag_off) & IP_OFFSET) {
		audit_log_format(ab, " frag=1");
		return;
	}

	audit_proto(ab, skb, ih->protocol, ih->ihl * 4);
}

static void audit_ip6(struct audit_buffer *ab, struct sk_buff *skb)
=======
	ih = skb_header_pointer(skb, skb_network_offset(skb), sizeof(_iph), &_iph);
	if (!ih)
		return false;

	audit_log_format(ab, " saddr=%pI4 daddr=%pI4 proto=%hhu",
			 &ih->saddr, &ih->daddr, ih->protocol);

	return true;
}

static bool audit_ip6(struct audit_buffer *ab, struct sk_buff *skb)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ipv6hdr _ip6h;
	const struct ipv6hdr *ih;
	u8 nexthdr;
	__be16 frag_off;
<<<<<<< HEAD
	int offset;

	ih = skb_header_pointer(skb, skb_network_offset(skb), sizeof(_ip6h), &_ip6h);
	if (!ih) {
		audit_log_format(ab, " truncated=1");
		return;
	}

	nexthdr = ih->nexthdr;
	offset = ipv6_skip_exthdr(skb, skb_network_offset(skb) + sizeof(_ip6h),
				  &nexthdr, &frag_off);
=======

	ih = skb_header_pointer(skb, skb_network_offset(skb), sizeof(_ip6h), &_ip6h);
	if (!ih)
		return false;

	nexthdr = ih->nexthdr;
	ipv6_skip_exthdr(skb, skb_network_offset(skb) + sizeof(_ip6h), &nexthdr, &frag_off);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	audit_log_format(ab, " saddr=%pI6c daddr=%pI6c proto=%hhu",
			 &ih->saddr, &ih->daddr, nexthdr);

<<<<<<< HEAD
	if (offset)
		audit_proto(ab, skb, nexthdr, offset);
=======
	return true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static unsigned int
audit_tg(struct sk_buff *skb, const struct xt_action_param *par)
{
<<<<<<< HEAD
	const struct xt_audit_info *info = par->targinfo;
	struct audit_buffer *ab;

=======
	struct audit_buffer *ab;
	int fam = -1;

	if (audit_enabled == AUDIT_OFF)
		goto errout;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ab = audit_log_start(NULL, GFP_ATOMIC, AUDIT_NETFILTER_PKT);
	if (ab == NULL)
		goto errout;

<<<<<<< HEAD
	audit_log_format(ab, "action=%hhu hook=%u len=%u inif=%s outif=%s",
			 info->type, par->hooknum, skb->len,
			 par->in ? par->in->name : "?",
			 par->out ? par->out->name : "?");

	if (skb->mark)
		audit_log_format(ab, " mark=%#x", skb->mark);

	if (skb->dev && skb->dev->type == ARPHRD_ETHER) {
		audit_log_format(ab, " smac=%pM dmac=%pM macproto=0x%04x",
				 eth_hdr(skb)->h_source, eth_hdr(skb)->h_dest,
				 ntohs(eth_hdr(skb)->h_proto));

		if (par->family == NFPROTO_BRIDGE) {
			switch (eth_hdr(skb)->h_proto) {
			case __constant_htons(ETH_P_IP):
				audit_ip4(ab, skb);
				break;

			case __constant_htons(ETH_P_IPV6):
				audit_ip6(ab, skb);
				break;
			}
		}
	}

	switch (par->family) {
	case NFPROTO_IPV4:
		audit_ip4(ab, skb);
		break;

	case NFPROTO_IPV6:
		audit_ip6(ab, skb);
		break;
	}

#ifdef CONFIG_NETWORK_SECMARK
	if (skb->secmark)
		audit_log_secctx(ab, skb->secmark);
#endif
=======
	audit_log_format(ab, "mark=%#x", skb->mark);

	switch (xt_family(par)) {
	case NFPROTO_BRIDGE:
		switch (eth_hdr(skb)->h_proto) {
		case htons(ETH_P_IP):
			fam = audit_ip4(ab, skb) ? NFPROTO_IPV4 : -1;
			break;
		case htons(ETH_P_IPV6):
			fam = audit_ip6(ab, skb) ? NFPROTO_IPV6 : -1;
			break;
		}
		break;
	case NFPROTO_IPV4:
		fam = audit_ip4(ab, skb) ? NFPROTO_IPV4 : -1;
		break;
	case NFPROTO_IPV6:
		fam = audit_ip6(ab, skb) ? NFPROTO_IPV6 : -1;
		break;
	}

	if (fam == -1)
		audit_log_format(ab, " saddr=? daddr=? proto=-1");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	audit_log_end(ab);

errout:
	return XT_CONTINUE;
}

static unsigned int
audit_tg_ebt(struct sk_buff *skb, const struct xt_action_param *par)
{
	audit_tg(skb, par);
	return EBT_CONTINUE;
}

static int audit_tg_check(const struct xt_tgchk_param *par)
{
	const struct xt_audit_info *info = par->targinfo;

	if (info->type > XT_AUDIT_TYPE_MAX) {
<<<<<<< HEAD
		pr_info("Audit type out of range (valid range: 0..%hhu)\n",
			XT_AUDIT_TYPE_MAX);
=======
		pr_info_ratelimited("Audit type out of range (valid range: 0..%u)\n",
				    XT_AUDIT_TYPE_MAX);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ERANGE;
	}

	return 0;
}

static struct xt_target audit_tg_reg[] __read_mostly = {
	{
		.name		= "AUDIT",
		.family		= NFPROTO_UNSPEC,
		.target		= audit_tg,
		.targetsize	= sizeof(struct xt_audit_info),
		.checkentry	= audit_tg_check,
		.me		= THIS_MODULE,
	},
	{
		.name		= "AUDIT",
		.family		= NFPROTO_BRIDGE,
		.target		= audit_tg_ebt,
		.targetsize	= sizeof(struct xt_audit_info),
		.checkentry	= audit_tg_check,
		.me		= THIS_MODULE,
	},
};

static int __init audit_tg_init(void)
{
	return xt_register_targets(audit_tg_reg, ARRAY_SIZE(audit_tg_reg));
}

static void __exit audit_tg_exit(void)
{
	xt_unregister_targets(audit_tg_reg, ARRAY_SIZE(audit_tg_reg));
}

module_init(audit_tg_init);
module_exit(audit_tg_exit);
