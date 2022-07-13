<<<<<<< HEAD
/* L3/L4 protocol support for nf_conntrack. */

/* (C) 1999-2001 Paul `Rusty' Russell
 * (C) 2002-2006 Netfilter Core Team <coreteam@netfilter.org>
 * (C) 2003,2004 USAGI/WIDE Project <http://www.linux-ipv6.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <linux/types.h>
#include <linux/netfilter.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/mutex.h>
#include <linux/vmalloc.h>
#include <linux/stddef.h>
#include <linux/err.h>
#include <linux/percpu.h>
#include <linux/notifier.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
<<<<<<< HEAD
#include <linux/rtnetlink.h>

#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_l3proto.h>
#include <net/netfilter/nf_conntrack_l4proto.h>
#include <net/netfilter/nf_conntrack_core.h>

static struct nf_conntrack_l4proto __rcu **nf_ct_protos[PF_MAX] __read_mostly;
struct nf_conntrack_l3proto __rcu *nf_ct_l3protos[AF_MAX] __read_mostly;
EXPORT_SYMBOL_GPL(nf_ct_l3protos);
=======

#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_l4proto.h>
#include <net/netfilter/nf_conntrack_core.h>
#include <net/netfilter/nf_conntrack_bridge.h>
#include <net/netfilter/nf_log.h>

#include <linux/ip.h>
#include <linux/icmp.h>
#include <linux/sysctl.h>
#include <net/route.h>
#include <net/ip.h>

#include <linux/netfilter_ipv4.h>
#include <linux/netfilter_ipv6.h>
#include <linux/netfilter_ipv6/ip6_tables.h>
#include <net/netfilter/nf_conntrack_helper.h>
#include <net/netfilter/nf_conntrack_zones.h>
#include <net/netfilter/nf_conntrack_seqadj.h>
#include <net/netfilter/ipv4/nf_conntrack_ipv4.h>
#include <net/netfilter/ipv6/nf_conntrack_ipv6.h>
#include <net/netfilter/nf_nat_helper.h>
#include <net/netfilter/ipv4/nf_defrag_ipv4.h>
#include <net/netfilter/ipv6/nf_defrag_ipv6.h>

#include <linux/ipv6.h>
#include <linux/in6.h>
#include <net/ipv6.h>
#include <net/inet_frag.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static DEFINE_MUTEX(nf_ct_proto_mutex);

#ifdef CONFIG_SYSCTL
<<<<<<< HEAD
static int
nf_ct_register_sysctl(struct ctl_table_header **header, struct ctl_path *path,
		      struct ctl_table *table, unsigned int *users)
{
	if (*header == NULL) {
		*header = register_sysctl_paths(path, table);
		if (*header == NULL)
			return -ENOMEM;
	}
	if (users != NULL)
		(*users)++;
	return 0;
}

static void
nf_ct_unregister_sysctl(struct ctl_table_header **header,
			struct ctl_table *table, unsigned int *users)
{
	if (users != NULL && --*users > 0)
		return;

	unregister_sysctl_table(*header);
	*header = NULL;
}
#endif

struct nf_conntrack_l4proto *
__nf_ct_l4proto_find(u_int16_t l3proto, u_int8_t l4proto)
{
	if (unlikely(l3proto >= AF_MAX || nf_ct_protos[l3proto] == NULL))
		return &nf_conntrack_l4proto_generic;

	return rcu_dereference(nf_ct_protos[l3proto][l4proto]);
}
EXPORT_SYMBOL_GPL(__nf_ct_l4proto_find);

/* this is guaranteed to always return a valid protocol helper, since
 * it falls back to generic_protocol */
struct nf_conntrack_l3proto *
nf_ct_l3proto_find_get(u_int16_t l3proto)
{
	struct nf_conntrack_l3proto *p;

	rcu_read_lock();
	p = __nf_ct_l3proto_find(l3proto);
	if (!try_module_get(p->me))
		p = &nf_conntrack_l3proto_generic;
	rcu_read_unlock();

	return p;
}
EXPORT_SYMBOL_GPL(nf_ct_l3proto_find_get);

void nf_ct_l3proto_put(struct nf_conntrack_l3proto *p)
{
	module_put(p->me);
}
EXPORT_SYMBOL_GPL(nf_ct_l3proto_put);

int
nf_ct_l3proto_try_module_get(unsigned short l3proto)
{
	int ret;
	struct nf_conntrack_l3proto *p;

retry:	p = nf_ct_l3proto_find_get(l3proto);
	if (p == &nf_conntrack_l3proto_generic) {
		ret = request_module("nf_conntrack-%d", l3proto);
		if (!ret)
			goto retry;

		return -EPROTOTYPE;
=======
__printf(4, 5)
void nf_l4proto_log_invalid(const struct sk_buff *skb,
			    const struct nf_hook_state *state,
			    u8 protonum,
			    const char *fmt, ...)
{
	struct net *net = state->net;
	struct va_format vaf;
	va_list args;

	if (net->ct.sysctl_log_invalid != protonum &&
	    net->ct.sysctl_log_invalid != IPPROTO_RAW)
		return;

	va_start(args, fmt);
	vaf.fmt = fmt;
	vaf.va = &args;

	nf_log_packet(net, state->pf, 0, skb, state->in, state->out,
		      NULL, "nf_ct_proto_%d: %pV ", protonum, &vaf);
	va_end(args);
}
EXPORT_SYMBOL_GPL(nf_l4proto_log_invalid);

__printf(4, 5)
void nf_ct_l4proto_log_invalid(const struct sk_buff *skb,
			       const struct nf_conn *ct,
			       const struct nf_hook_state *state,
			       const char *fmt, ...)
{
	struct va_format vaf;
	struct net *net;
	va_list args;

	net = nf_ct_net(ct);
	if (likely(net->ct.sysctl_log_invalid == 0))
		return;

	va_start(args, fmt);
	vaf.fmt = fmt;
	vaf.va = &args;

	nf_l4proto_log_invalid(skb, state,
			       nf_ct_protonum(ct), "%pV", &vaf);
	va_end(args);
}
EXPORT_SYMBOL_GPL(nf_ct_l4proto_log_invalid);
#endif

const struct nf_conntrack_l4proto *nf_ct_l4proto_find(u8 l4proto)
{
	switch (l4proto) {
	case IPPROTO_UDP: return &nf_conntrack_l4proto_udp;
	case IPPROTO_TCP: return &nf_conntrack_l4proto_tcp;
	case IPPROTO_ICMP: return &nf_conntrack_l4proto_icmp;
#ifdef CONFIG_NF_CT_PROTO_DCCP
	case IPPROTO_DCCP: return &nf_conntrack_l4proto_dccp;
#endif
#ifdef CONFIG_NF_CT_PROTO_SCTP
	case IPPROTO_SCTP: return &nf_conntrack_l4proto_sctp;
#endif
#ifdef CONFIG_NF_CT_PROTO_UDPLITE
	case IPPROTO_UDPLITE: return &nf_conntrack_l4proto_udplite;
#endif
#ifdef CONFIG_NF_CT_PROTO_GRE
	case IPPROTO_GRE: return &nf_conntrack_l4proto_gre;
#endif
#if IS_ENABLED(CONFIG_IPV6)
	case IPPROTO_ICMPV6: return &nf_conntrack_l4proto_icmpv6;
#endif /* CONFIG_IPV6 */
	}

	return &nf_conntrack_l4proto_generic;
};
EXPORT_SYMBOL_GPL(nf_ct_l4proto_find);

static bool in_vrf_postrouting(const struct nf_hook_state *state)
{
#if IS_ENABLED(CONFIG_NET_L3_MASTER_DEV)
	if (state->hook == NF_INET_POST_ROUTING &&
	    netif_is_l3_master(state->out))
		return true;
#endif
	return false;
}

unsigned int nf_confirm(void *priv,
			struct sk_buff *skb,
			const struct nf_hook_state *state)
{
	const struct nf_conn_help *help;
	enum ip_conntrack_info ctinfo;
	unsigned int protoff;
	struct nf_conn *ct;
	bool seqadj_needed;
	__be16 frag_off;
	int start;
	u8 pnum;

	ct = nf_ct_get(skb, &ctinfo);
	if (!ct || in_vrf_postrouting(state))
		return NF_ACCEPT;

	help = nfct_help(ct);

	seqadj_needed = test_bit(IPS_SEQ_ADJUST_BIT, &ct->status) && !nf_is_loopback_packet(skb);
	if (!help && !seqadj_needed)
		return nf_conntrack_confirm(skb);

	/* helper->help() do not expect ICMP packets */
	if (ctinfo == IP_CT_RELATED_REPLY)
		return nf_conntrack_confirm(skb);

	switch (nf_ct_l3num(ct)) {
	case NFPROTO_IPV4:
		protoff = skb_network_offset(skb) + ip_hdrlen(skb);
		break;
	case NFPROTO_IPV6:
		pnum = ipv6_hdr(skb)->nexthdr;
		start = ipv6_skip_exthdr(skb, sizeof(struct ipv6hdr), &pnum, &frag_off);
		if (start < 0 || (frag_off & htons(~0x7)) != 0)
			return nf_conntrack_confirm(skb);

		protoff = start;
		break;
	default:
		return nf_conntrack_confirm(skb);
	}

	if (help) {
		const struct nf_conntrack_helper *helper;
		int ret;

		/* rcu_read_lock()ed by nf_hook */
		helper = rcu_dereference(help->helper);
		if (helper) {
			ret = helper->help(skb,
					   protoff,
					   ct, ctinfo);
			if (ret != NF_ACCEPT)
				return ret;
		}
	}

	if (seqadj_needed &&
	    !nf_ct_seq_adjust(skb, ct, ctinfo, protoff)) {
		NF_CT_STAT_INC_ATOMIC(nf_ct_net(ct), drop);
		return NF_DROP;
	}

	/* We've seen it coming out the other side: confirm it */
	return nf_conntrack_confirm(skb);
}
EXPORT_SYMBOL_GPL(nf_confirm);

static unsigned int ipv4_conntrack_in(void *priv,
				      struct sk_buff *skb,
				      const struct nf_hook_state *state)
{
	return nf_conntrack_in(skb, state);
}

static unsigned int ipv4_conntrack_local(void *priv,
					 struct sk_buff *skb,
					 const struct nf_hook_state *state)
{
	if (ip_is_fragment(ip_hdr(skb))) { /* IP_NODEFRAG setsockopt set */
		enum ip_conntrack_info ctinfo;
		struct nf_conn *tmpl;

		tmpl = nf_ct_get(skb, &ctinfo);
		if (tmpl && nf_ct_is_template(tmpl)) {
			/* when skipping ct, clear templates to avoid fooling
			 * later targets/matches
			 */
			skb->_nfct = 0;
			nf_ct_put(tmpl);
		}
		return NF_ACCEPT;
	}

	return nf_conntrack_in(skb, state);
}

/* Connection tracking may drop packets, but never alters them, so
 * make it the first hook.
 */
static const struct nf_hook_ops ipv4_conntrack_ops[] = {
	{
		.hook		= ipv4_conntrack_in,
		.pf		= NFPROTO_IPV4,
		.hooknum	= NF_INET_PRE_ROUTING,
		.priority	= NF_IP_PRI_CONNTRACK,
	},
	{
		.hook		= ipv4_conntrack_local,
		.pf		= NFPROTO_IPV4,
		.hooknum	= NF_INET_LOCAL_OUT,
		.priority	= NF_IP_PRI_CONNTRACK,
	},
	{
		.hook		= nf_confirm,
		.pf		= NFPROTO_IPV4,
		.hooknum	= NF_INET_POST_ROUTING,
		.priority	= NF_IP_PRI_CONNTRACK_CONFIRM,
	},
	{
		.hook		= nf_confirm,
		.pf		= NFPROTO_IPV4,
		.hooknum	= NF_INET_LOCAL_IN,
		.priority	= NF_IP_PRI_CONNTRACK_CONFIRM,
	},
};

/* Fast function for those who don't want to parse /proc (and I don't
 * blame them).
 * Reversing the socket's dst/src point of view gives us the reply
 * mapping.
 */
static int
getorigdst(struct sock *sk, int optval, void __user *user, int *len)
{
	const struct inet_sock *inet = inet_sk(sk);
	const struct nf_conntrack_tuple_hash *h;
	struct nf_conntrack_tuple tuple;

	memset(&tuple, 0, sizeof(tuple));

	lock_sock(sk);
	tuple.src.u3.ip = inet->inet_rcv_saddr;
	tuple.src.u.tcp.port = inet->inet_sport;
	tuple.dst.u3.ip = inet->inet_daddr;
	tuple.dst.u.tcp.port = inet->inet_dport;
	tuple.src.l3num = PF_INET;
	tuple.dst.protonum = sk->sk_protocol;
	release_sock(sk);

	/* We only do TCP and SCTP at the moment: is there a better way? */
	if (tuple.dst.protonum != IPPROTO_TCP &&
	    tuple.dst.protonum != IPPROTO_SCTP)
		return -ENOPROTOOPT;

	if ((unsigned int)*len < sizeof(struct sockaddr_in))
		return -EINVAL;

	h = nf_conntrack_find_get(sock_net(sk), &nf_ct_zone_dflt, &tuple);
	if (h) {
		struct sockaddr_in sin;
		struct nf_conn *ct = nf_ct_tuplehash_to_ctrack(h);

		sin.sin_family = AF_INET;
		sin.sin_port = ct->tuplehash[IP_CT_DIR_ORIGINAL]
			.tuple.dst.u.tcp.port;
		sin.sin_addr.s_addr = ct->tuplehash[IP_CT_DIR_ORIGINAL]
			.tuple.dst.u3.ip;
		memset(sin.sin_zero, 0, sizeof(sin.sin_zero));

		nf_ct_put(ct);
		if (copy_to_user(user, &sin, sizeof(sin)) != 0)
			return -EFAULT;
		else
			return 0;
	}
	return -ENOENT;
}

static struct nf_sockopt_ops so_getorigdst = {
	.pf		= PF_INET,
	.get_optmin	= SO_ORIGINAL_DST,
	.get_optmax	= SO_ORIGINAL_DST + 1,
	.get		= getorigdst,
	.owner		= THIS_MODULE,
};

#if IS_ENABLED(CONFIG_IPV6)
static int
ipv6_getorigdst(struct sock *sk, int optval, void __user *user, int *len)
{
	struct nf_conntrack_tuple tuple = { .src.l3num = NFPROTO_IPV6 };
	const struct ipv6_pinfo *inet6 = inet6_sk(sk);
	const struct inet_sock *inet = inet_sk(sk);
	const struct nf_conntrack_tuple_hash *h;
	struct sockaddr_in6 sin6;
	struct nf_conn *ct;
	__be32 flow_label;
	int bound_dev_if;

	lock_sock(sk);
	tuple.src.u3.in6 = sk->sk_v6_rcv_saddr;
	tuple.src.u.tcp.port = inet->inet_sport;
	tuple.dst.u3.in6 = sk->sk_v6_daddr;
	tuple.dst.u.tcp.port = inet->inet_dport;
	tuple.dst.protonum = sk->sk_protocol;
	bound_dev_if = sk->sk_bound_dev_if;
	flow_label = inet6->flow_label;
	release_sock(sk);

	if (tuple.dst.protonum != IPPROTO_TCP &&
	    tuple.dst.protonum != IPPROTO_SCTP)
		return -ENOPROTOOPT;

	if (*len < 0 || (unsigned int)*len < sizeof(sin6))
		return -EINVAL;

	h = nf_conntrack_find_get(sock_net(sk), &nf_ct_zone_dflt, &tuple);
	if (!h)
		return -ENOENT;

	ct = nf_ct_tuplehash_to_ctrack(h);

	sin6.sin6_family = AF_INET6;
	sin6.sin6_port = ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.tcp.port;
	sin6.sin6_flowinfo = flow_label & IPV6_FLOWINFO_MASK;
	memcpy(&sin6.sin6_addr,
	       &ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.in6,
	       sizeof(sin6.sin6_addr));

	nf_ct_put(ct);
	sin6.sin6_scope_id = ipv6_iface_scope_id(&sin6.sin6_addr, bound_dev_if);
	return copy_to_user(user, &sin6, sizeof(sin6)) ? -EFAULT : 0;
}

static struct nf_sockopt_ops so_getorigdst6 = {
	.pf		= NFPROTO_IPV6,
	.get_optmin	= IP6T_SO_ORIGINAL_DST,
	.get_optmax	= IP6T_SO_ORIGINAL_DST + 1,
	.get		= ipv6_getorigdst,
	.owner		= THIS_MODULE,
};

static unsigned int ipv6_conntrack_in(void *priv,
				      struct sk_buff *skb,
				      const struct nf_hook_state *state)
{
	return nf_conntrack_in(skb, state);
}

static unsigned int ipv6_conntrack_local(void *priv,
					 struct sk_buff *skb,
					 const struct nf_hook_state *state)
{
	return nf_conntrack_in(skb, state);
}

static const struct nf_hook_ops ipv6_conntrack_ops[] = {
	{
		.hook		= ipv6_conntrack_in,
		.pf		= NFPROTO_IPV6,
		.hooknum	= NF_INET_PRE_ROUTING,
		.priority	= NF_IP6_PRI_CONNTRACK,
	},
	{
		.hook		= ipv6_conntrack_local,
		.pf		= NFPROTO_IPV6,
		.hooknum	= NF_INET_LOCAL_OUT,
		.priority	= NF_IP6_PRI_CONNTRACK,
	},
	{
		.hook		= nf_confirm,
		.pf		= NFPROTO_IPV6,
		.hooknum	= NF_INET_POST_ROUTING,
		.priority	= NF_IP6_PRI_LAST,
	},
	{
		.hook		= nf_confirm,
		.pf		= NFPROTO_IPV6,
		.hooknum	= NF_INET_LOCAL_IN,
		.priority	= NF_IP6_PRI_LAST - 1,
	},
};
#endif

static int nf_ct_tcp_fixup(struct nf_conn *ct, void *_nfproto)
{
	u8 nfproto = (unsigned long)_nfproto;

	if (nf_ct_l3num(ct) != nfproto)
		return 0;

	if (nf_ct_protonum(ct) == IPPROTO_TCP &&
	    ct->proto.tcp.state == TCP_CONNTRACK_ESTABLISHED) {
		ct->proto.tcp.seen[0].td_maxwin = 0;
		ct->proto.tcp.seen[1].td_maxwin = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return 0;
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(nf_ct_l3proto_try_module_get);

void nf_ct_l3proto_module_put(unsigned short l3proto)
{
	struct nf_conntrack_l3proto *p;

	/* rcu_read_lock not necessary since the caller holds a reference, but
	 * taken anyways to avoid lockdep warnings in __nf_ct_l3proto_find()
	 */
	rcu_read_lock();
	p = __nf_ct_l3proto_find(l3proto);
	module_put(p->me);
	rcu_read_unlock();
}
EXPORT_SYMBOL_GPL(nf_ct_l3proto_module_put);

struct nf_conntrack_l4proto *
nf_ct_l4proto_find_get(u_int16_t l3num, u_int8_t l4num)
{
	struct nf_conntrack_l4proto *p;

	rcu_read_lock();
	p = __nf_ct_l4proto_find(l3num, l4num);
	if (!try_module_get(p->me))
		p = &nf_conntrack_l4proto_generic;
	rcu_read_unlock();

	return p;
}
EXPORT_SYMBOL_GPL(nf_ct_l4proto_find_get);

void nf_ct_l4proto_put(struct nf_conntrack_l4proto *p)
{
	module_put(p->me);
}
EXPORT_SYMBOL_GPL(nf_ct_l4proto_put);

static int kill_l3proto(struct nf_conn *i, void *data)
{
	return nf_ct_l3num(i) == ((struct nf_conntrack_l3proto *)data)->l3proto;
}

static int kill_l4proto(struct nf_conn *i, void *data)
{
	struct nf_conntrack_l4proto *l4proto;
	l4proto = (struct nf_conntrack_l4proto *)data;
	return nf_ct_protonum(i) == l4proto->l4proto &&
	       nf_ct_l3num(i) == l4proto->l3proto;
}

static int nf_ct_l3proto_register_sysctl(struct nf_conntrack_l3proto *l3proto)
{
	int err = 0;

#ifdef CONFIG_SYSCTL
	if (l3proto->ctl_table != NULL) {
		err = nf_ct_register_sysctl(&l3proto->ctl_table_header,
					    l3proto->ctl_table_path,
					    l3proto->ctl_table, NULL);
	}
#endif
	return err;
}

static void nf_ct_l3proto_unregister_sysctl(struct nf_conntrack_l3proto *l3proto)
{
#ifdef CONFIG_SYSCTL
	if (l3proto->ctl_table_header != NULL)
		nf_ct_unregister_sysctl(&l3proto->ctl_table_header,
					l3proto->ctl_table, NULL);
#endif
}

int nf_conntrack_l3proto_register(struct nf_conntrack_l3proto *proto)
{
	int ret = 0;
	struct nf_conntrack_l3proto *old;

	if (proto->l3proto >= AF_MAX)
		return -EBUSY;

	if (proto->tuple_to_nlattr && !proto->nlattr_tuple_size)
		return -EINVAL;

	mutex_lock(&nf_ct_proto_mutex);
	old = rcu_dereference_protected(nf_ct_l3protos[proto->l3proto],
					lockdep_is_held(&nf_ct_proto_mutex));
	if (old != &nf_conntrack_l3proto_generic) {
		ret = -EBUSY;
		goto out_unlock;
	}

	ret = nf_ct_l3proto_register_sysctl(proto);
	if (ret < 0)
		goto out_unlock;

	if (proto->nlattr_tuple_size)
		proto->nla_size = 3 * proto->nlattr_tuple_size();

	rcu_assign_pointer(nf_ct_l3protos[proto->l3proto], proto);

out_unlock:
	mutex_unlock(&nf_ct_proto_mutex);
	return ret;
}
EXPORT_SYMBOL_GPL(nf_conntrack_l3proto_register);

void nf_conntrack_l3proto_unregister(struct nf_conntrack_l3proto *proto)
{
	struct net *net;

	BUG_ON(proto->l3proto >= AF_MAX);

	mutex_lock(&nf_ct_proto_mutex);
	BUG_ON(rcu_dereference_protected(nf_ct_l3protos[proto->l3proto],
					 lockdep_is_held(&nf_ct_proto_mutex)
					 ) != proto);
	rcu_assign_pointer(nf_ct_l3protos[proto->l3proto],
			   &nf_conntrack_l3proto_generic);
	nf_ct_l3proto_unregister_sysctl(proto);
	mutex_unlock(&nf_ct_proto_mutex);

	synchronize_rcu();

	/* Remove all contrack entries for this protocol */
	rtnl_lock();
	for_each_net(net)
		nf_ct_iterate_cleanup(net, kill_l3proto, proto);
	rtnl_unlock();
}
EXPORT_SYMBOL_GPL(nf_conntrack_l3proto_unregister);

static int nf_ct_l4proto_register_sysctl(struct nf_conntrack_l4proto *l4proto)
{
	int err = 0;

#ifdef CONFIG_SYSCTL
	if (l4proto->ctl_table != NULL) {
		err = nf_ct_register_sysctl(l4proto->ctl_table_header,
					    nf_net_netfilter_sysctl_path,
					    l4proto->ctl_table,
					    l4proto->ctl_table_users);
		if (err < 0)
			goto out;
	}
#ifdef CONFIG_NF_CONNTRACK_PROC_COMPAT
	if (l4proto->ctl_compat_table != NULL) {
		err = nf_ct_register_sysctl(&l4proto->ctl_compat_table_header,
					    nf_net_ipv4_netfilter_sysctl_path,
					    l4proto->ctl_compat_table, NULL);
		if (err == 0)
			goto out;
		nf_ct_unregister_sysctl(l4proto->ctl_table_header,
					l4proto->ctl_table,
					l4proto->ctl_table_users);
	}
#endif /* CONFIG_NF_CONNTRACK_PROC_COMPAT */
out:
#endif /* CONFIG_SYSCTL */
	return err;
}

static void nf_ct_l4proto_unregister_sysctl(struct nf_conntrack_l4proto *l4proto)
{
#ifdef CONFIG_SYSCTL
	if (l4proto->ctl_table_header != NULL &&
	    *l4proto->ctl_table_header != NULL)
		nf_ct_unregister_sysctl(l4proto->ctl_table_header,
					l4proto->ctl_table,
					l4proto->ctl_table_users);
#ifdef CONFIG_NF_CONNTRACK_PROC_COMPAT
	if (l4proto->ctl_compat_table_header != NULL)
		nf_ct_unregister_sysctl(&l4proto->ctl_compat_table_header,
					l4proto->ctl_compat_table, NULL);
#endif /* CONFIG_NF_CONNTRACK_PROC_COMPAT */
#endif /* CONFIG_SYSCTL */
}

/* FIXME: Allow NULL functions and sub in pointers to generic for
   them. --RR */
int nf_conntrack_l4proto_register(struct nf_conntrack_l4proto *l4proto)
{
	int ret = 0;

	if (l4proto->l3proto >= PF_MAX)
		return -EBUSY;

	if ((l4proto->to_nlattr && !l4proto->nlattr_size)
		|| (l4proto->tuple_to_nlattr && !l4proto->nlattr_tuple_size))
		return -EINVAL;

	mutex_lock(&nf_ct_proto_mutex);
	if (!nf_ct_protos[l4proto->l3proto]) {
		/* l3proto may be loaded latter. */
		struct nf_conntrack_l4proto __rcu **proto_array;
		int i;

		proto_array = kmalloc(MAX_NF_CT_PROTO *
				      sizeof(struct nf_conntrack_l4proto *),
				      GFP_KERNEL);
		if (proto_array == NULL) {
			ret = -ENOMEM;
			goto out_unlock;
		}

		for (i = 0; i < MAX_NF_CT_PROTO; i++)
			RCU_INIT_POINTER(proto_array[i], &nf_conntrack_l4proto_generic);

		/* Before making proto_array visible to lockless readers,
		 * we must make sure its content is committed to memory.
		 */
		smp_wmb();

		nf_ct_protos[l4proto->l3proto] = proto_array;
	} else if (rcu_dereference_protected(
			nf_ct_protos[l4proto->l3proto][l4proto->l4proto],
			lockdep_is_held(&nf_ct_proto_mutex)
			) != &nf_conntrack_l4proto_generic) {
		ret = -EBUSY;
		goto out_unlock;
	}

	ret = nf_ct_l4proto_register_sysctl(l4proto);
	if (ret < 0)
		goto out_unlock;

	l4proto->nla_size = 0;
	if (l4proto->nlattr_size)
		l4proto->nla_size += l4proto->nlattr_size();
	if (l4proto->nlattr_tuple_size)
		l4proto->nla_size += 3 * l4proto->nlattr_tuple_size();

	rcu_assign_pointer(nf_ct_protos[l4proto->l3proto][l4proto->l4proto],
			   l4proto);

out_unlock:
	mutex_unlock(&nf_ct_proto_mutex);
	return ret;
}
EXPORT_SYMBOL_GPL(nf_conntrack_l4proto_register);

void nf_conntrack_l4proto_unregister(struct nf_conntrack_l4proto *l4proto)
{
	struct net *net;

	BUG_ON(l4proto->l3proto >= PF_MAX);

	mutex_lock(&nf_ct_proto_mutex);
	BUG_ON(rcu_dereference_protected(
			nf_ct_protos[l4proto->l3proto][l4proto->l4proto],
			lockdep_is_held(&nf_ct_proto_mutex)
			) != l4proto);
	rcu_assign_pointer(nf_ct_protos[l4proto->l3proto][l4proto->l4proto],
			   &nf_conntrack_l4proto_generic);
	nf_ct_l4proto_unregister_sysctl(l4proto);
	mutex_unlock(&nf_ct_proto_mutex);

	synchronize_rcu();

	/* Remove all contrack entries for this protocol */
	rtnl_lock();
	for_each_net(net)
		nf_ct_iterate_cleanup(net, kill_l4proto, l4proto);
	rtnl_unlock();
}
EXPORT_SYMBOL_GPL(nf_conntrack_l4proto_unregister);

int nf_conntrack_proto_init(void)
{
	unsigned int i;
	int err;

	err = nf_ct_l4proto_register_sysctl(&nf_conntrack_l4proto_generic);
	if (err < 0)
		return err;

	for (i = 0; i < AF_MAX; i++)
		rcu_assign_pointer(nf_ct_l3protos[i],
				   &nf_conntrack_l3proto_generic);
	return 0;
=======

static struct nf_ct_bridge_info *nf_ct_bridge_info;

static int nf_ct_netns_do_get(struct net *net, u8 nfproto)
{
	struct nf_conntrack_net *cnet = nf_ct_pernet(net);
	bool fixup_needed = false, retry = true;
	int err = 0;
retry:
	mutex_lock(&nf_ct_proto_mutex);

	switch (nfproto) {
	case NFPROTO_IPV4:
		cnet->users4++;
		if (cnet->users4 > 1)
			goto out_unlock;
		err = nf_defrag_ipv4_enable(net);
		if (err) {
			cnet->users4 = 0;
			goto out_unlock;
		}

		err = nf_register_net_hooks(net, ipv4_conntrack_ops,
					    ARRAY_SIZE(ipv4_conntrack_ops));
		if (err)
			cnet->users4 = 0;
		else
			fixup_needed = true;
		break;
#if IS_ENABLED(CONFIG_IPV6)
	case NFPROTO_IPV6:
		cnet->users6++;
		if (cnet->users6 > 1)
			goto out_unlock;
		err = nf_defrag_ipv6_enable(net);
		if (err < 0) {
			cnet->users6 = 0;
			goto out_unlock;
		}

		err = nf_register_net_hooks(net, ipv6_conntrack_ops,
					    ARRAY_SIZE(ipv6_conntrack_ops));
		if (err)
			cnet->users6 = 0;
		else
			fixup_needed = true;
		break;
#endif
	case NFPROTO_BRIDGE:
		if (!nf_ct_bridge_info) {
			if (!retry) {
				err = -EPROTO;
				goto out_unlock;
			}
			mutex_unlock(&nf_ct_proto_mutex);
			request_module("nf_conntrack_bridge");
			retry = false;
			goto retry;
		}
		if (!try_module_get(nf_ct_bridge_info->me)) {
			err = -EPROTO;
			goto out_unlock;
		}
		cnet->users_bridge++;
		if (cnet->users_bridge > 1)
			goto out_unlock;

		err = nf_register_net_hooks(net, nf_ct_bridge_info->ops,
					    nf_ct_bridge_info->ops_size);
		if (err)
			cnet->users_bridge = 0;
		else
			fixup_needed = true;
		break;
	default:
		err = -EPROTO;
		break;
	}
 out_unlock:
	mutex_unlock(&nf_ct_proto_mutex);

	if (fixup_needed) {
		struct nf_ct_iter_data iter_data = {
			.net	= net,
			.data	= (void *)(unsigned long)nfproto,
		};
		nf_ct_iterate_cleanup_net(nf_ct_tcp_fixup, &iter_data);
	}

	return err;
}

static void nf_ct_netns_do_put(struct net *net, u8 nfproto)
{
	struct nf_conntrack_net *cnet = nf_ct_pernet(net);

	mutex_lock(&nf_ct_proto_mutex);
	switch (nfproto) {
	case NFPROTO_IPV4:
		if (cnet->users4 && (--cnet->users4 == 0)) {
			nf_unregister_net_hooks(net, ipv4_conntrack_ops,
						ARRAY_SIZE(ipv4_conntrack_ops));
			nf_defrag_ipv4_disable(net);
		}
		break;
#if IS_ENABLED(CONFIG_IPV6)
	case NFPROTO_IPV6:
		if (cnet->users6 && (--cnet->users6 == 0)) {
			nf_unregister_net_hooks(net, ipv6_conntrack_ops,
						ARRAY_SIZE(ipv6_conntrack_ops));
			nf_defrag_ipv6_disable(net);
		}
		break;
#endif
	case NFPROTO_BRIDGE:
		if (!nf_ct_bridge_info)
			break;
		if (cnet->users_bridge && (--cnet->users_bridge == 0))
			nf_unregister_net_hooks(net, nf_ct_bridge_info->ops,
						nf_ct_bridge_info->ops_size);

		module_put(nf_ct_bridge_info->me);
		break;
	}
	mutex_unlock(&nf_ct_proto_mutex);
}

static int nf_ct_netns_inet_get(struct net *net)
{
	int err;

	err = nf_ct_netns_do_get(net, NFPROTO_IPV4);
#if IS_ENABLED(CONFIG_IPV6)
	if (err < 0)
		goto err1;
	err = nf_ct_netns_do_get(net, NFPROTO_IPV6);
	if (err < 0)
		goto err2;

	return err;
err2:
	nf_ct_netns_put(net, NFPROTO_IPV4);
err1:
#endif
	return err;
}

int nf_ct_netns_get(struct net *net, u8 nfproto)
{
	int err;

	switch (nfproto) {
	case NFPROTO_INET:
		err = nf_ct_netns_inet_get(net);
		break;
	case NFPROTO_BRIDGE:
		err = nf_ct_netns_do_get(net, NFPROTO_BRIDGE);
		if (err < 0)
			return err;

		err = nf_ct_netns_inet_get(net);
		if (err < 0) {
			nf_ct_netns_put(net, NFPROTO_BRIDGE);
			return err;
		}
		break;
	default:
		err = nf_ct_netns_do_get(net, nfproto);
		break;
	}
	return err;
}
EXPORT_SYMBOL_GPL(nf_ct_netns_get);

void nf_ct_netns_put(struct net *net, uint8_t nfproto)
{
	switch (nfproto) {
	case NFPROTO_BRIDGE:
		nf_ct_netns_do_put(net, NFPROTO_BRIDGE);
		fallthrough;
	case NFPROTO_INET:
		nf_ct_netns_do_put(net, NFPROTO_IPV4);
		nf_ct_netns_do_put(net, NFPROTO_IPV6);
		break;
	default:
		nf_ct_netns_do_put(net, nfproto);
		break;
	}
}
EXPORT_SYMBOL_GPL(nf_ct_netns_put);

void nf_ct_bridge_register(struct nf_ct_bridge_info *info)
{
	WARN_ON(nf_ct_bridge_info);
	mutex_lock(&nf_ct_proto_mutex);
	nf_ct_bridge_info = info;
	mutex_unlock(&nf_ct_proto_mutex);
}
EXPORT_SYMBOL_GPL(nf_ct_bridge_register);

void nf_ct_bridge_unregister(struct nf_ct_bridge_info *info)
{
	WARN_ON(!nf_ct_bridge_info);
	mutex_lock(&nf_ct_proto_mutex);
	nf_ct_bridge_info = NULL;
	mutex_unlock(&nf_ct_proto_mutex);
}
EXPORT_SYMBOL_GPL(nf_ct_bridge_unregister);

int nf_conntrack_proto_init(void)
{
	int ret;

	ret = nf_register_sockopt(&so_getorigdst);
	if (ret < 0)
		return ret;

#if IS_ENABLED(CONFIG_IPV6)
	ret = nf_register_sockopt(&so_getorigdst6);
	if (ret < 0)
		goto cleanup_sockopt;
#endif

	return ret;

#if IS_ENABLED(CONFIG_IPV6)
cleanup_sockopt:
	nf_unregister_sockopt(&so_getorigdst);
#endif
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void nf_conntrack_proto_fini(void)
{
<<<<<<< HEAD
	unsigned int i;

	nf_ct_l4proto_unregister_sysctl(&nf_conntrack_l4proto_generic);

	/* free l3proto protocol tables */
	for (i = 0; i < PF_MAX; i++)
		kfree(nf_ct_protos[i]);
}
=======
	nf_unregister_sockopt(&so_getorigdst);
#if IS_ENABLED(CONFIG_IPV6)
	nf_unregister_sockopt(&so_getorigdst6);
#endif
}

void nf_conntrack_proto_pernet_init(struct net *net)
{
	nf_conntrack_generic_init_net(net);
	nf_conntrack_udp_init_net(net);
	nf_conntrack_tcp_init_net(net);
	nf_conntrack_icmp_init_net(net);
#if IS_ENABLED(CONFIG_IPV6)
	nf_conntrack_icmpv6_init_net(net);
#endif
#ifdef CONFIG_NF_CT_PROTO_DCCP
	nf_conntrack_dccp_init_net(net);
#endif
#ifdef CONFIG_NF_CT_PROTO_SCTP
	nf_conntrack_sctp_init_net(net);
#endif
#ifdef CONFIG_NF_CT_PROTO_GRE
	nf_conntrack_gre_init_net(net);
#endif
}

module_param_call(hashsize, nf_conntrack_set_hashsize, param_get_uint,
		  &nf_conntrack_htable_size, 0600);

MODULE_ALIAS("ip_conntrack");
MODULE_ALIAS("nf_conntrack-" __stringify(AF_INET));
MODULE_ALIAS("nf_conntrack-" __stringify(AF_INET6));
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("IPv4 and IPv6 connection tracking");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
