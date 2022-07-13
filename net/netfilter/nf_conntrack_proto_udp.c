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
 * (C) 2006-2012 Patrick McHardy <kaber@trash.net>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/types.h>
#include <linux/timer.h>
#include <linux/module.h>
#include <linux/udp.h>
#include <linux/seq_file.h>
#include <linux/skbuff.h>
#include <linux/ipv6.h>
#include <net/ip6_checksum.h>
#include <net/checksum.h>

#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/netfilter_ipv6.h>
#include <net/netfilter/nf_conntrack_l4proto.h>
#include <net/netfilter/nf_conntrack_ecache.h>
<<<<<<< HEAD
=======
#include <net/netfilter/nf_conntrack_timeout.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <net/netfilter/nf_log.h>
#include <net/netfilter/ipv4/nf_conntrack_ipv4.h>
#include <net/netfilter/ipv6/nf_conntrack_ipv6.h>

<<<<<<< HEAD
enum udp_conntrack {
	UDP_CT_UNREPLIED,
	UDP_CT_REPLIED,
	UDP_CT_MAX
};

static unsigned int udp_timeouts[UDP_CT_MAX] = {
	[UDP_CT_UNREPLIED]	= 30*HZ,
	[UDP_CT_REPLIED]	= 180*HZ,
};

static bool udp_pkt_to_tuple(const struct sk_buff *skb,
			     unsigned int dataoff,
			     struct nf_conntrack_tuple *tuple)
{
	const struct udphdr *hp;
	struct udphdr _hdr;

	/* Actually only need first 8 bytes. */
	hp = skb_header_pointer(skb, dataoff, sizeof(_hdr), &_hdr);
	if (hp == NULL)
		return false;

	tuple->src.u.udp.port = hp->source;
	tuple->dst.u.udp.port = hp->dest;

	return true;
}

static bool udp_invert_tuple(struct nf_conntrack_tuple *tuple,
			     const struct nf_conntrack_tuple *orig)
{
	tuple->src.u.udp.port = orig->dst.u.udp.port;
	tuple->dst.u.udp.port = orig->src.u.udp.port;
	return true;
}

/* Print out the per-protocol part of the tuple. */
static int udp_print_tuple(struct seq_file *s,
			   const struct nf_conntrack_tuple *tuple)
{
	return seq_printf(s, "sport=%hu dport=%hu ",
			  ntohs(tuple->src.u.udp.port),
			  ntohs(tuple->dst.u.udp.port));
}

static unsigned int *udp_get_timeouts(struct net *net)
{
	return udp_timeouts;
}

/* Returns verdict for packet, and may modify conntracktype */
static int udp_packet(struct nf_conn *ct,
		      const struct sk_buff *skb,
		      unsigned int dataoff,
		      enum ip_conntrack_info ctinfo,
		      u_int8_t pf,
		      unsigned int hooknum,
		      unsigned int *timeouts)
{
	/* If we've seen traffic both ways, this is some kind of UDP
	   stream.  Extend timeout. */
	if (test_bit(IPS_SEEN_REPLY_BIT, &ct->status)) {
		nf_ct_refresh_acct(ct, ctinfo, skb,
				   timeouts[UDP_CT_REPLIED]);
		/* Also, more likely to be important, and not a probe */
		if (!test_and_set_bit(IPS_ASSURED_BIT, &ct->status))
			nf_conntrack_event_cache(IPCT_ASSURED, ct);
	} else {
		nf_ct_refresh_acct(ct, ctinfo, skb,
				   timeouts[UDP_CT_UNREPLIED]);
	}
	return NF_ACCEPT;
}

/* Called when a new connection for this protocol found. */
static bool udp_new(struct nf_conn *ct, const struct sk_buff *skb,
		    unsigned int dataoff, unsigned int *timeouts)
{
	return true;
}

static int udp_error(struct net *net, struct nf_conn *tmpl, struct sk_buff *skb,
		     unsigned int dataoff, enum ip_conntrack_info *ctinfo,
		     u_int8_t pf,
		     unsigned int hooknum)
=======
static const unsigned int udp_timeouts[UDP_CT_MAX] = {
	[UDP_CT_UNREPLIED]	= 30*HZ,
	[UDP_CT_REPLIED]	= 120*HZ,
};

static unsigned int *udp_get_timeouts(struct net *net)
{
	return nf_udp_pernet(net)->timeouts;
}

static void udp_error_log(const struct sk_buff *skb,
			  const struct nf_hook_state *state,
			  const char *msg)
{
	nf_l4proto_log_invalid(skb, state, IPPROTO_UDP, "%s", msg);
}

static bool udp_error(struct sk_buff *skb,
		      unsigned int dataoff,
		      const struct nf_hook_state *state)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned int udplen = skb->len - dataoff;
	const struct udphdr *hdr;
	struct udphdr _hdr;

	/* Header is too small? */
	hdr = skb_header_pointer(skb, dataoff, sizeof(_hdr), &_hdr);
<<<<<<< HEAD
	if (hdr == NULL) {
		if (LOG_INVALID(net, IPPROTO_UDP))
			nf_log_packet(pf, 0, skb, NULL, NULL, NULL,
				      "nf_ct_udp: short packet ");
		return -NF_ACCEPT;
=======
	if (!hdr) {
		udp_error_log(skb, state, "short packet");
		return true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* Truncated/malformed packets */
	if (ntohs(hdr->len) > udplen || ntohs(hdr->len) < sizeof(*hdr)) {
<<<<<<< HEAD
		if (LOG_INVALID(net, IPPROTO_UDP))
			nf_log_packet(pf, 0, skb, NULL, NULL, NULL,
				"nf_ct_udp: truncated/malformed packet ");
		return -NF_ACCEPT;
=======
		udp_error_log(skb, state, "truncated/malformed packet");
		return true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* Packet with no checksum */
	if (!hdr->check)
<<<<<<< HEAD
		return NF_ACCEPT;
=======
		return false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Checksum invalid? Ignore.
	 * We skip checking packets on the outgoing path
	 * because the checksum is assumed to be correct.
	 * FIXME: Source route IP option packets --RR */
<<<<<<< HEAD
	if (net->ct.sysctl_checksum && hooknum == NF_INET_PRE_ROUTING &&
	    nf_checksum(skb, hooknum, dataoff, IPPROTO_UDP, pf)) {
		if (LOG_INVALID(net, IPPROTO_UDP))
			nf_log_packet(pf, 0, skb, NULL, NULL, NULL,
				"nf_ct_udp: bad UDP checksum ");
		return -NF_ACCEPT;
	}

	return NF_ACCEPT;
}

#if IS_ENABLED(CONFIG_NF_CT_NETLINK_TIMEOUT)
=======
	if (state->hook == NF_INET_PRE_ROUTING &&
	    state->net->ct.sysctl_checksum &&
	    nf_checksum(skb, state->hook, dataoff, IPPROTO_UDP, state->pf)) {
		udp_error_log(skb, state, "bad checksum");
		return true;
	}

	return false;
}

/* Returns verdict for packet, and may modify conntracktype */
int nf_conntrack_udp_packet(struct nf_conn *ct,
			    struct sk_buff *skb,
			    unsigned int dataoff,
			    enum ip_conntrack_info ctinfo,
			    const struct nf_hook_state *state)
{
	unsigned int *timeouts;
	unsigned long status;

	if (udp_error(skb, dataoff, state))
		return -NF_ACCEPT;

	timeouts = nf_ct_timeout_lookup(ct);
	if (!timeouts)
		timeouts = udp_get_timeouts(nf_ct_net(ct));

	status = READ_ONCE(ct->status);
	if ((status & IPS_CONFIRMED) == 0)
		ct->proto.udp.stream_ts = 2 * HZ + jiffies;

	/* If we've seen traffic both ways, this is some kind of UDP
	 * stream. Set Assured.
	 */
	if (status & IPS_SEEN_REPLY) {
		unsigned long extra = timeouts[UDP_CT_UNREPLIED];
		bool stream = false;

		/* Still active after two seconds? Extend timeout. */
		if (time_after(jiffies, ct->proto.udp.stream_ts)) {
			extra = timeouts[UDP_CT_REPLIED];
			stream = (status & IPS_ASSURED) == 0;
		}

		nf_ct_refresh_acct(ct, ctinfo, skb, extra);

		/* never set ASSURED for IPS_NAT_CLASH, they time out soon */
		if (unlikely((status & IPS_NAT_CLASH)))
			return NF_ACCEPT;

		/* Also, more likely to be important, and not a probe */
		if (stream && !test_and_set_bit(IPS_ASSURED_BIT, &ct->status))
			nf_conntrack_event_cache(IPCT_ASSURED, ct);
	} else {
		nf_ct_refresh_acct(ct, ctinfo, skb, timeouts[UDP_CT_UNREPLIED]);
	}
	return NF_ACCEPT;
}

#ifdef CONFIG_NF_CT_PROTO_UDPLITE
static void udplite_error_log(const struct sk_buff *skb,
			      const struct nf_hook_state *state,
			      const char *msg)
{
	nf_l4proto_log_invalid(skb, state, IPPROTO_UDPLITE, "%s", msg);
}

static bool udplite_error(struct sk_buff *skb,
			  unsigned int dataoff,
			  const struct nf_hook_state *state)
{
	unsigned int udplen = skb->len - dataoff;
	const struct udphdr *hdr;
	struct udphdr _hdr;
	unsigned int cscov;

	/* Header is too small? */
	hdr = skb_header_pointer(skb, dataoff, sizeof(_hdr), &_hdr);
	if (!hdr) {
		udplite_error_log(skb, state, "short packet");
		return true;
	}

	cscov = ntohs(hdr->len);
	if (cscov == 0) {
		cscov = udplen;
	} else if (cscov < sizeof(*hdr) || cscov > udplen) {
		udplite_error_log(skb, state, "invalid checksum coverage");
		return true;
	}

	/* UDPLITE mandates checksums */
	if (!hdr->check) {
		udplite_error_log(skb, state, "checksum missing");
		return true;
	}

	/* Checksum invalid? Ignore. */
	if (state->hook == NF_INET_PRE_ROUTING &&
	    state->net->ct.sysctl_checksum &&
	    nf_checksum_partial(skb, state->hook, dataoff, cscov, IPPROTO_UDP,
				state->pf)) {
		udplite_error_log(skb, state, "bad checksum");
		return true;
	}

	return false;
}

/* Returns verdict for packet, and may modify conntracktype */
int nf_conntrack_udplite_packet(struct nf_conn *ct,
				struct sk_buff *skb,
				unsigned int dataoff,
				enum ip_conntrack_info ctinfo,
				const struct nf_hook_state *state)
{
	unsigned int *timeouts;

	if (udplite_error(skb, dataoff, state))
		return -NF_ACCEPT;

	timeouts = nf_ct_timeout_lookup(ct);
	if (!timeouts)
		timeouts = udp_get_timeouts(nf_ct_net(ct));

	/* If we've seen traffic both ways, this is some kind of UDP
	   stream.  Extend timeout. */
	if (test_bit(IPS_SEEN_REPLY_BIT, &ct->status)) {
		nf_ct_refresh_acct(ct, ctinfo, skb,
				   timeouts[UDP_CT_REPLIED]);

		if (unlikely((ct->status & IPS_NAT_CLASH)))
			return NF_ACCEPT;

		/* Also, more likely to be important, and not a probe */
		if (!test_and_set_bit(IPS_ASSURED_BIT, &ct->status))
			nf_conntrack_event_cache(IPCT_ASSURED, ct);
	} else {
		nf_ct_refresh_acct(ct, ctinfo, skb, timeouts[UDP_CT_UNREPLIED]);
	}
	return NF_ACCEPT;
}
#endif

#ifdef CONFIG_NF_CONNTRACK_TIMEOUT
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <linux/netfilter/nfnetlink.h>
#include <linux/netfilter/nfnetlink_cttimeout.h>

<<<<<<< HEAD
static int udp_timeout_nlattr_to_obj(struct nlattr *tb[], void *data)
{
	unsigned int *timeouts = data;

	/* set default timeouts for UDP. */
	timeouts[UDP_CT_UNREPLIED] = udp_timeouts[UDP_CT_UNREPLIED];
	timeouts[UDP_CT_REPLIED] = udp_timeouts[UDP_CT_REPLIED];
=======
static int udp_timeout_nlattr_to_obj(struct nlattr *tb[],
				     struct net *net, void *data)
{
	unsigned int *timeouts = data;
	struct nf_udp_net *un = nf_udp_pernet(net);

	if (!timeouts)
		timeouts = un->timeouts;

	/* set default timeouts for UDP. */
	timeouts[UDP_CT_UNREPLIED] = un->timeouts[UDP_CT_UNREPLIED];
	timeouts[UDP_CT_REPLIED] = un->timeouts[UDP_CT_REPLIED];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (tb[CTA_TIMEOUT_UDP_UNREPLIED]) {
		timeouts[UDP_CT_UNREPLIED] =
			ntohl(nla_get_be32(tb[CTA_TIMEOUT_UDP_UNREPLIED])) * HZ;
	}
	if (tb[CTA_TIMEOUT_UDP_REPLIED]) {
		timeouts[UDP_CT_REPLIED] =
			ntohl(nla_get_be32(tb[CTA_TIMEOUT_UDP_REPLIED])) * HZ;
	}
	return 0;
}

static int
udp_timeout_obj_to_nlattr(struct sk_buff *skb, const void *data)
{
	const unsigned int *timeouts = data;

<<<<<<< HEAD
	NLA_PUT_BE32(skb, CTA_TIMEOUT_UDP_UNREPLIED,
			htonl(timeouts[UDP_CT_UNREPLIED] / HZ));
	NLA_PUT_BE32(skb, CTA_TIMEOUT_UDP_REPLIED,
			htonl(timeouts[UDP_CT_REPLIED] / HZ));
=======
	if (nla_put_be32(skb, CTA_TIMEOUT_UDP_UNREPLIED,
			 htonl(timeouts[UDP_CT_UNREPLIED] / HZ)) ||
	    nla_put_be32(skb, CTA_TIMEOUT_UDP_REPLIED,
			 htonl(timeouts[UDP_CT_REPLIED] / HZ)))
		goto nla_put_failure;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;

nla_put_failure:
	return -ENOSPC;
}

static const struct nla_policy
udp_timeout_nla_policy[CTA_TIMEOUT_UDP_MAX+1] = {
       [CTA_TIMEOUT_UDP_UNREPLIED]	= { .type = NLA_U32 },
       [CTA_TIMEOUT_UDP_REPLIED]	= { .type = NLA_U32 },
};
<<<<<<< HEAD
#endif /* CONFIG_NF_CT_NETLINK_TIMEOUT */

#ifdef CONFIG_SYSCTL
static unsigned int udp_sysctl_table_users;
static struct ctl_table_header *udp_sysctl_header;
static struct ctl_table udp_sysctl_table[] = {
	{
		.procname	= "nf_conntrack_udp_timeout",
		.data		= &udp_timeouts[UDP_CT_UNREPLIED],
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	{
		.procname	= "nf_conntrack_udp_timeout_stream",
		.data		= &udp_timeouts[UDP_CT_REPLIED],
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	{ }
};
#ifdef CONFIG_NF_CONNTRACK_PROC_COMPAT
static struct ctl_table udp_compat_sysctl_table[] = {
	{
		.procname	= "ip_conntrack_udp_timeout",
		.data		= &udp_timeouts[UDP_CT_UNREPLIED],
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	{
		.procname	= "ip_conntrack_udp_timeout_stream",
		.data		= &udp_timeouts[UDP_CT_REPLIED],
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	{ }
};
#endif /* CONFIG_NF_CONNTRACK_PROC_COMPAT */
#endif /* CONFIG_SYSCTL */

struct nf_conntrack_l4proto nf_conntrack_l4proto_udp4 __read_mostly =
{
	.l3proto		= PF_INET,
	.l4proto		= IPPROTO_UDP,
	.name			= "udp",
	.pkt_to_tuple		= udp_pkt_to_tuple,
	.invert_tuple		= udp_invert_tuple,
	.print_tuple		= udp_print_tuple,
	.packet			= udp_packet,
	.get_timeouts		= udp_get_timeouts,
	.new			= udp_new,
	.error			= udp_error,
=======
#endif /* CONFIG_NF_CONNTRACK_TIMEOUT */

void nf_conntrack_udp_init_net(struct net *net)
{
	struct nf_udp_net *un = nf_udp_pernet(net);
	int i;

	for (i = 0; i < UDP_CT_MAX; i++)
		un->timeouts[i] = udp_timeouts[i];

#if IS_ENABLED(CONFIG_NF_FLOW_TABLE)
	un->offload_timeout = 30 * HZ;
#endif
}

const struct nf_conntrack_l4proto nf_conntrack_l4proto_udp =
{
	.l4proto		= IPPROTO_UDP,
	.allow_clash		= true,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#if IS_ENABLED(CONFIG_NF_CT_NETLINK)
	.tuple_to_nlattr	= nf_ct_port_tuple_to_nlattr,
	.nlattr_to_tuple	= nf_ct_port_nlattr_to_tuple,
	.nlattr_tuple_size	= nf_ct_port_nlattr_tuple_size,
	.nla_policy		= nf_ct_port_nla_policy,
#endif
<<<<<<< HEAD
#if IS_ENABLED(CONFIG_NF_CT_NETLINK_TIMEOUT)
=======
#ifdef CONFIG_NF_CONNTRACK_TIMEOUT
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.ctnl_timeout		= {
		.nlattr_to_obj	= udp_timeout_nlattr_to_obj,
		.obj_to_nlattr	= udp_timeout_obj_to_nlattr,
		.nlattr_max	= CTA_TIMEOUT_UDP_MAX,
		.obj_size	= sizeof(unsigned int) * CTA_TIMEOUT_UDP_MAX,
		.nla_policy	= udp_timeout_nla_policy,
	},
<<<<<<< HEAD
#endif /* CONFIG_NF_CT_NETLINK_TIMEOUT */
#ifdef CONFIG_SYSCTL
	.ctl_table_users	= &udp_sysctl_table_users,
	.ctl_table_header	= &udp_sysctl_header,
	.ctl_table		= udp_sysctl_table,
#ifdef CONFIG_NF_CONNTRACK_PROC_COMPAT
	.ctl_compat_table	= udp_compat_sysctl_table,
#endif
#endif
};
EXPORT_SYMBOL_GPL(nf_conntrack_l4proto_udp4);

struct nf_conntrack_l4proto nf_conntrack_l4proto_udp6 __read_mostly =
{
	.l3proto		= PF_INET6,
	.l4proto		= IPPROTO_UDP,
	.name			= "udp",
	.pkt_to_tuple		= udp_pkt_to_tuple,
	.invert_tuple		= udp_invert_tuple,
	.print_tuple		= udp_print_tuple,
	.packet			= udp_packet,
	.get_timeouts		= udp_get_timeouts,
	.new			= udp_new,
	.error			= udp_error,
=======
#endif /* CONFIG_NF_CONNTRACK_TIMEOUT */
};

#ifdef CONFIG_NF_CT_PROTO_UDPLITE
const struct nf_conntrack_l4proto nf_conntrack_l4proto_udplite =
{
	.l4proto		= IPPROTO_UDPLITE,
	.allow_clash		= true,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#if IS_ENABLED(CONFIG_NF_CT_NETLINK)
	.tuple_to_nlattr	= nf_ct_port_tuple_to_nlattr,
	.nlattr_to_tuple	= nf_ct_port_nlattr_to_tuple,
	.nlattr_tuple_size	= nf_ct_port_nlattr_tuple_size,
	.nla_policy		= nf_ct_port_nla_policy,
#endif
<<<<<<< HEAD
#if IS_ENABLED(CONFIG_NF_CT_NETLINK_TIMEOUT)
=======
#ifdef CONFIG_NF_CONNTRACK_TIMEOUT
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.ctnl_timeout		= {
		.nlattr_to_obj	= udp_timeout_nlattr_to_obj,
		.obj_to_nlattr	= udp_timeout_obj_to_nlattr,
		.nlattr_max	= CTA_TIMEOUT_UDP_MAX,
		.obj_size	= sizeof(unsigned int) * CTA_TIMEOUT_UDP_MAX,
		.nla_policy	= udp_timeout_nla_policy,
	},
<<<<<<< HEAD
#endif /* CONFIG_NF_CT_NETLINK_TIMEOUT */
#ifdef CONFIG_SYSCTL
	.ctl_table_users	= &udp_sysctl_table_users,
	.ctl_table_header	= &udp_sysctl_header,
	.ctl_table		= udp_sysctl_table,
#endif
};
EXPORT_SYMBOL_GPL(nf_conntrack_l4proto_udp6);
=======
#endif /* CONFIG_NF_CONNTRACK_TIMEOUT */
};
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
