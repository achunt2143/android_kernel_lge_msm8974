<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * net/sched/cls_flow.c		Generic flow classifier
 *
 * Copyright (c) 2007, 2008 Patrick McHardy <kaber@trash.net>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/jhash.h>
#include <linux/random.h>
#include <linux/pkt_cls.h>
#include <linux/skbuff.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/if_vlan.h>
#include <linux/slab.h>
#include <linux/module.h>
<<<<<<< HEAD
=======
#include <net/inet_sock.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <net/pkt_cls.h>
#include <net/ip.h>
#include <net/route.h>
<<<<<<< HEAD
#include <net/flow_keys.h>

#if defined(CONFIG_NF_CONNTRACK) || defined(CONFIG_NF_CONNTRACK_MODULE)
=======
#include <net/flow_dissector.h>
#include <net/tc_wrapper.h>

#if IS_ENABLED(CONFIG_NF_CONNTRACK)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <net/netfilter/nf_conntrack.h>
#endif

struct flow_head {
	struct list_head	filters;
<<<<<<< HEAD
=======
	struct rcu_head		rcu;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct flow_filter {
	struct list_head	list;
	struct tcf_exts		exts;
	struct tcf_ematch_tree	ematches;
<<<<<<< HEAD
=======
	struct tcf_proto	*tp;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct timer_list	perturb_timer;
	u32			perturb_period;
	u32			handle;

	u32			nkeys;
	u32			keymask;
	u32			mode;
	u32			mask;
	u32			xor;
	u32			rshift;
	u32			addend;
	u32			divisor;
	u32			baseclass;
	u32			hashrnd;
<<<<<<< HEAD
};

static const struct tcf_ext_map flow_ext_map = {
	.action	= TCA_FLOW_ACT,
	.police	= TCA_FLOW_POLICE,
=======
	struct rcu_work		rwork;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static inline u32 addr_fold(void *addr)
{
	unsigned long a = (unsigned long)addr;

	return (a & 0xFFFFFFFF) ^ (BITS_PER_LONG > 32 ? a >> 32 : 0);
}

static u32 flow_get_src(const struct sk_buff *skb, const struct flow_keys *flow)
{
<<<<<<< HEAD
	if (flow->src)
		return ntohl(flow->src);
=======
	__be32 src = flow_get_u32_src(flow);

	if (src)
		return ntohl(src);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return addr_fold(skb->sk);
}

static u32 flow_get_dst(const struct sk_buff *skb, const struct flow_keys *flow)
{
<<<<<<< HEAD
	if (flow->dst)
		return ntohl(flow->dst);
	return addr_fold(skb_dst(skb)) ^ (__force u16)skb->protocol;
}

static u32 flow_get_proto(const struct sk_buff *skb, const struct flow_keys *flow)
{
	return flow->ip_proto;
}

static u32 flow_get_proto_src(const struct sk_buff *skb, const struct flow_keys *flow)
{
	if (flow->ports)
		return ntohs(flow->port16[0]);
=======
	__be32 dst = flow_get_u32_dst(flow);

	if (dst)
		return ntohl(dst);

	return addr_fold(skb_dst(skb)) ^ (__force u16)skb_protocol(skb, true);
}

static u32 flow_get_proto(const struct sk_buff *skb,
			  const struct flow_keys *flow)
{
	return flow->basic.ip_proto;
}

static u32 flow_get_proto_src(const struct sk_buff *skb,
			      const struct flow_keys *flow)
{
	if (flow->ports.ports)
		return ntohs(flow->ports.src);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return addr_fold(skb->sk);
}

<<<<<<< HEAD
static u32 flow_get_proto_dst(const struct sk_buff *skb, const struct flow_keys *flow)
{
	if (flow->ports)
		return ntohs(flow->port16[1]);

	return addr_fold(skb_dst(skb)) ^ (__force u16)skb->protocol;
=======
static u32 flow_get_proto_dst(const struct sk_buff *skb,
			      const struct flow_keys *flow)
{
	if (flow->ports.ports)
		return ntohs(flow->ports.dst);

	return addr_fold(skb_dst(skb)) ^ (__force u16)skb_protocol(skb, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static u32 flow_get_iif(const struct sk_buff *skb)
{
	return skb->skb_iif;
}

static u32 flow_get_priority(const struct sk_buff *skb)
{
	return skb->priority;
}

static u32 flow_get_mark(const struct sk_buff *skb)
{
	return skb->mark;
}

static u32 flow_get_nfct(const struct sk_buff *skb)
{
<<<<<<< HEAD
#if defined(CONFIG_NF_CONNTRACK) || defined(CONFIG_NF_CONNTRACK_MODULE)
	return addr_fold(skb->nfct);
=======
#if IS_ENABLED(CONFIG_NF_CONNTRACK)
	return addr_fold(skb_nfct(skb));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#else
	return 0;
#endif
}

<<<<<<< HEAD
#if defined(CONFIG_NF_CONNTRACK) || defined(CONFIG_NF_CONNTRACK_MODULE)
=======
#if IS_ENABLED(CONFIG_NF_CONNTRACK)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define CTTUPLE(skb, member)						\
({									\
	enum ip_conntrack_info ctinfo;					\
	const struct nf_conn *ct = nf_ct_get(skb, &ctinfo);		\
	if (ct == NULL)							\
		goto fallback;						\
	ct->tuplehash[CTINFO2DIR(ctinfo)].tuple.member;			\
})
#else
#define CTTUPLE(skb, member)						\
({									\
	goto fallback;							\
	0;								\
})
#endif

<<<<<<< HEAD
static u32 flow_get_nfct_src(const struct sk_buff *skb, const struct flow_keys *flow)
{
	switch (skb->protocol) {
=======
static u32 flow_get_nfct_src(const struct sk_buff *skb,
			     const struct flow_keys *flow)
{
	switch (skb_protocol(skb, true)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case htons(ETH_P_IP):
		return ntohl(CTTUPLE(skb, src.u3.ip));
	case htons(ETH_P_IPV6):
		return ntohl(CTTUPLE(skb, src.u3.ip6[3]));
	}
fallback:
	return flow_get_src(skb, flow);
}

<<<<<<< HEAD
static u32 flow_get_nfct_dst(const struct sk_buff *skb, const struct flow_keys *flow)
{
	switch (skb->protocol) {
=======
static u32 flow_get_nfct_dst(const struct sk_buff *skb,
			     const struct flow_keys *flow)
{
	switch (skb_protocol(skb, true)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case htons(ETH_P_IP):
		return ntohl(CTTUPLE(skb, dst.u3.ip));
	case htons(ETH_P_IPV6):
		return ntohl(CTTUPLE(skb, dst.u3.ip6[3]));
	}
fallback:
	return flow_get_dst(skb, flow);
}

<<<<<<< HEAD
static u32 flow_get_nfct_proto_src(const struct sk_buff *skb, const struct flow_keys *flow)
=======
static u32 flow_get_nfct_proto_src(const struct sk_buff *skb,
				   const struct flow_keys *flow)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return ntohs(CTTUPLE(skb, src.u.all));
fallback:
	return flow_get_proto_src(skb, flow);
}

<<<<<<< HEAD
static u32 flow_get_nfct_proto_dst(const struct sk_buff *skb, const struct flow_keys *flow)
=======
static u32 flow_get_nfct_proto_dst(const struct sk_buff *skb,
				   const struct flow_keys *flow)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return ntohs(CTTUPLE(skb, dst.u.all));
fallback:
	return flow_get_proto_dst(skb, flow);
}

static u32 flow_get_rtclassid(const struct sk_buff *skb)
{
#ifdef CONFIG_IP_ROUTE_CLASSID
	if (skb_dst(skb))
		return skb_dst(skb)->tclassid;
#endif
	return 0;
}

static u32 flow_get_skuid(const struct sk_buff *skb)
{
<<<<<<< HEAD
	if (skb->sk && skb->sk->sk_socket && skb->sk->sk_socket->file)
		return skb->sk->sk_socket->file->f_cred->fsuid;
=======
	struct sock *sk = skb_to_full_sk(skb);

	if (sk && sk->sk_socket && sk->sk_socket->file) {
		kuid_t skuid = sk->sk_socket->file->f_cred->fsuid;

		return from_kuid(&init_user_ns, skuid);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static u32 flow_get_skgid(const struct sk_buff *skb)
{
<<<<<<< HEAD
	if (skb->sk && skb->sk->sk_socket && skb->sk->sk_socket->file)
		return skb->sk->sk_socket->file->f_cred->fsgid;
=======
	struct sock *sk = skb_to_full_sk(skb);

	if (sk && sk->sk_socket && sk->sk_socket->file) {
		kgid_t skgid = sk->sk_socket->file->f_cred->fsgid;

		return from_kgid(&init_user_ns, skgid);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static u32 flow_get_vlan_tag(const struct sk_buff *skb)
{
<<<<<<< HEAD
	u16 uninitialized_var(tag);
=======
	u16 tag;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (vlan_get_tag(skb, &tag) < 0)
		return 0;
	return tag & VLAN_VID_MASK;
}

static u32 flow_get_rxhash(struct sk_buff *skb)
{
<<<<<<< HEAD
	return skb_get_rxhash(skb);
=======
	return skb_get_hash(skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static u32 flow_key_get(struct sk_buff *skb, int key, struct flow_keys *flow)
{
	switch (key) {
	case FLOW_KEY_SRC:
		return flow_get_src(skb, flow);
	case FLOW_KEY_DST:
		return flow_get_dst(skb, flow);
	case FLOW_KEY_PROTO:
		return flow_get_proto(skb, flow);
	case FLOW_KEY_PROTO_SRC:
		return flow_get_proto_src(skb, flow);
	case FLOW_KEY_PROTO_DST:
		return flow_get_proto_dst(skb, flow);
	case FLOW_KEY_IIF:
		return flow_get_iif(skb);
	case FLOW_KEY_PRIORITY:
		return flow_get_priority(skb);
	case FLOW_KEY_MARK:
		return flow_get_mark(skb);
	case FLOW_KEY_NFCT:
		return flow_get_nfct(skb);
	case FLOW_KEY_NFCT_SRC:
		return flow_get_nfct_src(skb, flow);
	case FLOW_KEY_NFCT_DST:
		return flow_get_nfct_dst(skb, flow);
	case FLOW_KEY_NFCT_PROTO_SRC:
		return flow_get_nfct_proto_src(skb, flow);
	case FLOW_KEY_NFCT_PROTO_DST:
		return flow_get_nfct_proto_dst(skb, flow);
	case FLOW_KEY_RTCLASSID:
		return flow_get_rtclassid(skb);
	case FLOW_KEY_SKUID:
		return flow_get_skuid(skb);
	case FLOW_KEY_SKGID:
		return flow_get_skgid(skb);
	case FLOW_KEY_VLAN_TAG:
		return flow_get_vlan_tag(skb);
	case FLOW_KEY_RXHASH:
		return flow_get_rxhash(skb);
	default:
		WARN_ON(1);
		return 0;
	}
}

#define FLOW_KEYS_NEEDED ((1 << FLOW_KEY_SRC) | 		\
			  (1 << FLOW_KEY_DST) |			\
			  (1 << FLOW_KEY_PROTO) |		\
			  (1 << FLOW_KEY_PROTO_SRC) |		\
			  (1 << FLOW_KEY_PROTO_DST) | 		\
			  (1 << FLOW_KEY_NFCT_SRC) |		\
			  (1 << FLOW_KEY_NFCT_DST) |		\
			  (1 << FLOW_KEY_NFCT_PROTO_SRC) |	\
			  (1 << FLOW_KEY_NFCT_PROTO_DST))

<<<<<<< HEAD
static int flow_classify(struct sk_buff *skb, const struct tcf_proto *tp,
			 struct tcf_result *res)
{
	struct flow_head *head = tp->root;
=======
TC_INDIRECT_SCOPE int flow_classify(struct sk_buff *skb,
				    const struct tcf_proto *tp,
				    struct tcf_result *res)
{
	struct flow_head *head = rcu_dereference_bh(tp->root);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct flow_filter *f;
	u32 keymask;
	u32 classid;
	unsigned int n, key;
	int r;

<<<<<<< HEAD
	list_for_each_entry(f, &head->filters, list) {
=======
	list_for_each_entry_rcu(f, &head->filters, list) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		u32 keys[FLOW_KEY_MAX + 1];
		struct flow_keys flow_keys;

		if (!tcf_em_tree_match(skb, &f->ematches, NULL))
			continue;

		keymask = f->keymask;
		if (keymask & FLOW_KEYS_NEEDED)
<<<<<<< HEAD
			skb_flow_dissect(skb, &flow_keys);
=======
			skb_flow_dissect_flow_keys(skb, &flow_keys, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		for (n = 0; n < f->nkeys; n++) {
			key = ffs(keymask) - 1;
			keymask &= ~(1 << key);
			keys[n] = flow_key_get(skb, key, &flow_keys);
		}

		if (f->mode == FLOW_MODE_HASH)
			classid = jhash2(keys, f->nkeys, f->hashrnd);
		else {
			classid = keys[0];
			classid = (classid & f->mask) ^ f->xor;
			classid = (classid >> f->rshift) + f->addend;
		}

		if (f->divisor)
			classid %= f->divisor;

		res->class   = 0;
		res->classid = TC_H_MAKE(f->baseclass, f->baseclass + classid);

		r = tcf_exts_exec(skb, &f->exts, res);
		if (r < 0)
			continue;
		return r;
	}
	return -1;
}

<<<<<<< HEAD
static void flow_perturbation(unsigned long arg)
{
	struct flow_filter *f = (struct flow_filter *)arg;
=======
static void flow_perturbation(struct timer_list *t)
{
	struct flow_filter *f = from_timer(f, t, perturb_timer);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	get_random_bytes(&f->hashrnd, 4);
	if (f->perturb_period)
		mod_timer(&f->perturb_timer, jiffies + f->perturb_period);
}

static const struct nla_policy flow_policy[TCA_FLOW_MAX + 1] = {
	[TCA_FLOW_KEYS]		= { .type = NLA_U32 },
	[TCA_FLOW_MODE]		= { .type = NLA_U32 },
	[TCA_FLOW_BASECLASS]	= { .type = NLA_U32 },
	[TCA_FLOW_RSHIFT]	= { .type = NLA_U32 },
	[TCA_FLOW_ADDEND]	= { .type = NLA_U32 },
	[TCA_FLOW_MASK]		= { .type = NLA_U32 },
	[TCA_FLOW_XOR]		= { .type = NLA_U32 },
	[TCA_FLOW_DIVISOR]	= { .type = NLA_U32 },
	[TCA_FLOW_ACT]		= { .type = NLA_NESTED },
	[TCA_FLOW_POLICE]	= { .type = NLA_NESTED },
	[TCA_FLOW_EMATCHES]	= { .type = NLA_NESTED },
	[TCA_FLOW_PERTURB]	= { .type = NLA_U32 },
};

<<<<<<< HEAD
static int flow_change(struct tcf_proto *tp, unsigned long base,
		       u32 handle, struct nlattr **tca,
		       unsigned long *arg)
{
	struct flow_head *head = tp->root;
	struct flow_filter *f;
	struct nlattr *opt = tca[TCA_OPTIONS];
	struct nlattr *tb[TCA_FLOW_MAX + 1];
	struct tcf_exts e;
	struct tcf_ematch_tree t;
=======
static void __flow_destroy_filter(struct flow_filter *f)
{
	timer_shutdown_sync(&f->perturb_timer);
	tcf_exts_destroy(&f->exts);
	tcf_em_tree_destroy(&f->ematches);
	tcf_exts_put_net(&f->exts);
	kfree(f);
}

static void flow_destroy_filter_work(struct work_struct *work)
{
	struct flow_filter *f = container_of(to_rcu_work(work),
					     struct flow_filter,
					     rwork);
	rtnl_lock();
	__flow_destroy_filter(f);
	rtnl_unlock();
}

static int flow_change(struct net *net, struct sk_buff *in_skb,
		       struct tcf_proto *tp, unsigned long base,
		       u32 handle, struct nlattr **tca,
		       void **arg, u32 flags,
		       struct netlink_ext_ack *extack)
{
	struct flow_head *head = rtnl_dereference(tp->root);
	struct flow_filter *fold, *fnew;
	struct nlattr *opt = tca[TCA_OPTIONS];
	struct nlattr *tb[TCA_FLOW_MAX + 1];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned int nkeys = 0;
	unsigned int perturb_period = 0;
	u32 baseclass = 0;
	u32 keymask = 0;
	u32 mode;
	int err;

	if (opt == NULL)
		return -EINVAL;

<<<<<<< HEAD
	err = nla_parse_nested(tb, TCA_FLOW_MAX, opt, flow_policy);
=======
	err = nla_parse_nested_deprecated(tb, TCA_FLOW_MAX, opt, flow_policy,
					  NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err < 0)
		return err;

	if (tb[TCA_FLOW_BASECLASS]) {
		baseclass = nla_get_u32(tb[TCA_FLOW_BASECLASS]);
		if (TC_H_MIN(baseclass) == 0)
			return -EINVAL;
	}

	if (tb[TCA_FLOW_KEYS]) {
		keymask = nla_get_u32(tb[TCA_FLOW_KEYS]);

		nkeys = hweight32(keymask);
		if (nkeys == 0)
			return -EINVAL;

		if (fls(keymask) - 1 > FLOW_KEY_MAX)
			return -EOPNOTSUPP;
<<<<<<< HEAD
	}

	err = tcf_exts_validate(tp, tb, tca[TCA_RATE], &e, &flow_ext_map);
	if (err < 0)
		return err;

	err = tcf_em_tree_validate(tp, tb[TCA_FLOW_EMATCHES], &t);
	if (err < 0)
		goto err1;

	f = (struct flow_filter *)*arg;
	if (f != NULL) {
		err = -EINVAL;
		if (f->handle != handle && handle)
			goto err2;

		mode = f->mode;
=======

		if ((keymask & (FLOW_KEY_SKUID|FLOW_KEY_SKGID)) &&
		    sk_user_ns(NETLINK_CB(in_skb).sk) != &init_user_ns)
			return -EOPNOTSUPP;
	}

	fnew = kzalloc(sizeof(*fnew), GFP_KERNEL);
	if (!fnew)
		return -ENOBUFS;

	err = tcf_em_tree_validate(tp, tb[TCA_FLOW_EMATCHES], &fnew->ematches);
	if (err < 0)
		goto err1;

	err = tcf_exts_init(&fnew->exts, net, TCA_FLOW_ACT, TCA_FLOW_POLICE);
	if (err < 0)
		goto err2;

	err = tcf_exts_validate(net, tp, tb, tca[TCA_RATE], &fnew->exts, flags,
				extack);
	if (err < 0)
		goto err2;

	fold = *arg;
	if (fold) {
		err = -EINVAL;
		if (fold->handle != handle && handle)
			goto err2;

		/* Copy fold into fnew */
		fnew->tp = fold->tp;
		fnew->handle = fold->handle;
		fnew->nkeys = fold->nkeys;
		fnew->keymask = fold->keymask;
		fnew->mode = fold->mode;
		fnew->mask = fold->mask;
		fnew->xor = fold->xor;
		fnew->rshift = fold->rshift;
		fnew->addend = fold->addend;
		fnew->divisor = fold->divisor;
		fnew->baseclass = fold->baseclass;
		fnew->hashrnd = fold->hashrnd;

		mode = fold->mode;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (tb[TCA_FLOW_MODE])
			mode = nla_get_u32(tb[TCA_FLOW_MODE]);
		if (mode != FLOW_MODE_HASH && nkeys > 1)
			goto err2;

		if (mode == FLOW_MODE_HASH)
<<<<<<< HEAD
			perturb_period = f->perturb_period;
=======
			perturb_period = fold->perturb_period;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (tb[TCA_FLOW_PERTURB]) {
			if (mode != FLOW_MODE_HASH)
				goto err2;
			perturb_period = nla_get_u32(tb[TCA_FLOW_PERTURB]) * HZ;
		}
	} else {
		err = -EINVAL;
		if (!handle)
			goto err2;
		if (!tb[TCA_FLOW_KEYS])
			goto err2;

		mode = FLOW_MODE_MAP;
		if (tb[TCA_FLOW_MODE])
			mode = nla_get_u32(tb[TCA_FLOW_MODE]);
		if (mode != FLOW_MODE_HASH && nkeys > 1)
			goto err2;

		if (tb[TCA_FLOW_PERTURB]) {
			if (mode != FLOW_MODE_HASH)
				goto err2;
			perturb_period = nla_get_u32(tb[TCA_FLOW_PERTURB]) * HZ;
		}

<<<<<<< HEAD
		if (TC_H_MAJ(baseclass) == 0)
			baseclass = TC_H_MAKE(tp->q->handle, baseclass);
		if (TC_H_MIN(baseclass) == 0)
			baseclass = TC_H_MAKE(baseclass, 1);

		err = -ENOBUFS;
		f = kzalloc(sizeof(*f), GFP_KERNEL);
		if (f == NULL)
			goto err2;

		f->handle = handle;
		f->mask	  = ~0U;

		get_random_bytes(&f->hashrnd, 4);
		f->perturb_timer.function = flow_perturbation;
		f->perturb_timer.data = (unsigned long)f;
		init_timer_deferrable(&f->perturb_timer);
	}

	tcf_exts_change(tp, &f->exts, &e);
	tcf_em_tree_change(tp, &f->ematches, &t);

	tcf_tree_lock(tp);

	if (tb[TCA_FLOW_KEYS]) {
		f->keymask = keymask;
		f->nkeys   = nkeys;
	}

	f->mode = mode;

	if (tb[TCA_FLOW_MASK])
		f->mask = nla_get_u32(tb[TCA_FLOW_MASK]);
	if (tb[TCA_FLOW_XOR])
		f->xor = nla_get_u32(tb[TCA_FLOW_XOR]);
	if (tb[TCA_FLOW_RSHIFT])
		f->rshift = nla_get_u32(tb[TCA_FLOW_RSHIFT]);
	if (tb[TCA_FLOW_ADDEND])
		f->addend = nla_get_u32(tb[TCA_FLOW_ADDEND]);

	if (tb[TCA_FLOW_DIVISOR])
		f->divisor = nla_get_u32(tb[TCA_FLOW_DIVISOR]);
	if (baseclass)
		f->baseclass = baseclass;

	f->perturb_period = perturb_period;
	del_timer(&f->perturb_timer);
	if (perturb_period)
		mod_timer(&f->perturb_timer, jiffies + perturb_period);

	if (*arg == 0)
		list_add_tail(&f->list, &head->filters);

	tcf_tree_unlock(tp);

	*arg = (unsigned long)f;
	return 0;

err2:
	tcf_em_tree_destroy(tp, &t);
err1:
	tcf_exts_destroy(tp, &e);
	return err;
}

static void flow_destroy_filter(struct tcf_proto *tp, struct flow_filter *f)
{
	del_timer_sync(&f->perturb_timer);
	tcf_exts_destroy(tp, &f->exts);
	tcf_em_tree_destroy(tp, &f->ematches);
	kfree(f);
}

static int flow_delete(struct tcf_proto *tp, unsigned long arg)
{
	struct flow_filter *f = (struct flow_filter *)arg;

	tcf_tree_lock(tp);
	list_del(&f->list);
	tcf_tree_unlock(tp);
	flow_destroy_filter(tp, f);
=======
		if (TC_H_MAJ(baseclass) == 0) {
			struct Qdisc *q = tcf_block_q(tp->chain->block);

			baseclass = TC_H_MAKE(q->handle, baseclass);
		}
		if (TC_H_MIN(baseclass) == 0)
			baseclass = TC_H_MAKE(baseclass, 1);

		fnew->handle = handle;
		fnew->mask  = ~0U;
		fnew->tp = tp;
		get_random_bytes(&fnew->hashrnd, 4);
	}

	timer_setup(&fnew->perturb_timer, flow_perturbation, TIMER_DEFERRABLE);

	tcf_block_netif_keep_dst(tp->chain->block);

	if (tb[TCA_FLOW_KEYS]) {
		fnew->keymask = keymask;
		fnew->nkeys   = nkeys;
	}

	fnew->mode = mode;

	if (tb[TCA_FLOW_MASK])
		fnew->mask = nla_get_u32(tb[TCA_FLOW_MASK]);
	if (tb[TCA_FLOW_XOR])
		fnew->xor = nla_get_u32(tb[TCA_FLOW_XOR]);
	if (tb[TCA_FLOW_RSHIFT])
		fnew->rshift = nla_get_u32(tb[TCA_FLOW_RSHIFT]);
	if (tb[TCA_FLOW_ADDEND])
		fnew->addend = nla_get_u32(tb[TCA_FLOW_ADDEND]);

	if (tb[TCA_FLOW_DIVISOR])
		fnew->divisor = nla_get_u32(tb[TCA_FLOW_DIVISOR]);
	if (baseclass)
		fnew->baseclass = baseclass;

	fnew->perturb_period = perturb_period;
	if (perturb_period)
		mod_timer(&fnew->perturb_timer, jiffies + perturb_period);

	if (!*arg)
		list_add_tail_rcu(&fnew->list, &head->filters);
	else
		list_replace_rcu(&fold->list, &fnew->list);

	*arg = fnew;

	if (fold) {
		tcf_exts_get_net(&fold->exts);
		tcf_queue_work(&fold->rwork, flow_destroy_filter_work);
	}
	return 0;

err2:
	tcf_exts_destroy(&fnew->exts);
	tcf_em_tree_destroy(&fnew->ematches);
err1:
	kfree(fnew);
	return err;
}

static int flow_delete(struct tcf_proto *tp, void *arg, bool *last,
		       bool rtnl_held, struct netlink_ext_ack *extack)
{
	struct flow_head *head = rtnl_dereference(tp->root);
	struct flow_filter *f = arg;

	list_del_rcu(&f->list);
	tcf_exts_get_net(&f->exts);
	tcf_queue_work(&f->rwork, flow_destroy_filter_work);
	*last = list_empty(&head->filters);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static int flow_init(struct tcf_proto *tp)
{
	struct flow_head *head;

	head = kzalloc(sizeof(*head), GFP_KERNEL);
	if (head == NULL)
		return -ENOBUFS;
	INIT_LIST_HEAD(&head->filters);
<<<<<<< HEAD
	tp->root = head;
	return 0;
}

static void flow_destroy(struct tcf_proto *tp)
{
	struct flow_head *head = tp->root;
	struct flow_filter *f, *next;

	list_for_each_entry_safe(f, next, &head->filters, list) {
		list_del(&f->list);
		flow_destroy_filter(tp, f);
	}
	kfree(head);
}

static unsigned long flow_get(struct tcf_proto *tp, u32 handle)
{
	struct flow_head *head = tp->root;
=======
	rcu_assign_pointer(tp->root, head);
	return 0;
}

static void flow_destroy(struct tcf_proto *tp, bool rtnl_held,
			 struct netlink_ext_ack *extack)
{
	struct flow_head *head = rtnl_dereference(tp->root);
	struct flow_filter *f, *next;

	list_for_each_entry_safe(f, next, &head->filters, list) {
		list_del_rcu(&f->list);
		if (tcf_exts_get_net(&f->exts))
			tcf_queue_work(&f->rwork, flow_destroy_filter_work);
		else
			__flow_destroy_filter(f);
	}
	kfree_rcu(head, rcu);
}

static void *flow_get(struct tcf_proto *tp, u32 handle)
{
	struct flow_head *head = rtnl_dereference(tp->root);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct flow_filter *f;

	list_for_each_entry(f, &head->filters, list)
		if (f->handle == handle)
<<<<<<< HEAD
			return (unsigned long)f;
	return 0;
}

static void flow_put(struct tcf_proto *tp, unsigned long f)
{
}

static int flow_dump(struct tcf_proto *tp, unsigned long fh,
		     struct sk_buff *skb, struct tcmsg *t)
{
	struct flow_filter *f = (struct flow_filter *)fh;
=======
			return f;
	return NULL;
}

static int flow_dump(struct net *net, struct tcf_proto *tp, void *fh,
		     struct sk_buff *skb, struct tcmsg *t, bool rtnl_held)
{
	struct flow_filter *f = fh;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct nlattr *nest;

	if (f == NULL)
		return skb->len;

	t->tcm_handle = f->handle;

<<<<<<< HEAD
	nest = nla_nest_start(skb, TCA_OPTIONS);
	if (nest == NULL)
		goto nla_put_failure;

	NLA_PUT_U32(skb, TCA_FLOW_KEYS, f->keymask);
	NLA_PUT_U32(skb, TCA_FLOW_MODE, f->mode);

	if (f->mask != ~0 || f->xor != 0) {
		NLA_PUT_U32(skb, TCA_FLOW_MASK, f->mask);
		NLA_PUT_U32(skb, TCA_FLOW_XOR, f->xor);
	}
	if (f->rshift)
		NLA_PUT_U32(skb, TCA_FLOW_RSHIFT, f->rshift);
	if (f->addend)
		NLA_PUT_U32(skb, TCA_FLOW_ADDEND, f->addend);

	if (f->divisor)
		NLA_PUT_U32(skb, TCA_FLOW_DIVISOR, f->divisor);
	if (f->baseclass)
		NLA_PUT_U32(skb, TCA_FLOW_BASECLASS, f->baseclass);

	if (f->perturb_period)
		NLA_PUT_U32(skb, TCA_FLOW_PERTURB, f->perturb_period / HZ);

	if (tcf_exts_dump(skb, &f->exts, &flow_ext_map) < 0)
=======
	nest = nla_nest_start_noflag(skb, TCA_OPTIONS);
	if (nest == NULL)
		goto nla_put_failure;

	if (nla_put_u32(skb, TCA_FLOW_KEYS, f->keymask) ||
	    nla_put_u32(skb, TCA_FLOW_MODE, f->mode))
		goto nla_put_failure;

	if (f->mask != ~0 || f->xor != 0) {
		if (nla_put_u32(skb, TCA_FLOW_MASK, f->mask) ||
		    nla_put_u32(skb, TCA_FLOW_XOR, f->xor))
			goto nla_put_failure;
	}
	if (f->rshift &&
	    nla_put_u32(skb, TCA_FLOW_RSHIFT, f->rshift))
		goto nla_put_failure;
	if (f->addend &&
	    nla_put_u32(skb, TCA_FLOW_ADDEND, f->addend))
		goto nla_put_failure;

	if (f->divisor &&
	    nla_put_u32(skb, TCA_FLOW_DIVISOR, f->divisor))
		goto nla_put_failure;
	if (f->baseclass &&
	    nla_put_u32(skb, TCA_FLOW_BASECLASS, f->baseclass))
		goto nla_put_failure;

	if (f->perturb_period &&
	    nla_put_u32(skb, TCA_FLOW_PERTURB, f->perturb_period / HZ))
		goto nla_put_failure;

	if (tcf_exts_dump(skb, &f->exts) < 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto nla_put_failure;
#ifdef CONFIG_NET_EMATCH
	if (f->ematches.hdr.nmatches &&
	    tcf_em_tree_dump(skb, &f->ematches, TCA_FLOW_EMATCHES) < 0)
		goto nla_put_failure;
#endif
	nla_nest_end(skb, nest);

<<<<<<< HEAD
	if (tcf_exts_dump_stats(skb, &f->exts, &flow_ext_map) < 0)
=======
	if (tcf_exts_dump_stats(skb, &f->exts) < 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto nla_put_failure;

	return skb->len;

nla_put_failure:
<<<<<<< HEAD
	nlmsg_trim(skb, nest);
	return -1;
}

static void flow_walk(struct tcf_proto *tp, struct tcf_walker *arg)
{
	struct flow_head *head = tp->root;
	struct flow_filter *f;

	list_for_each_entry(f, &head->filters, list) {
		if (arg->count < arg->skip)
			goto skip;
		if (arg->fn(tp, (unsigned long)f, arg) < 0) {
			arg->stop = 1;
			break;
		}
skip:
		arg->count++;
=======
	nla_nest_cancel(skb, nest);
	return -1;
}

static void flow_walk(struct tcf_proto *tp, struct tcf_walker *arg,
		      bool rtnl_held)
{
	struct flow_head *head = rtnl_dereference(tp->root);
	struct flow_filter *f;

	list_for_each_entry(f, &head->filters, list) {
		if (!tc_cls_stats_dump(tp, arg, f))
			break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

static struct tcf_proto_ops cls_flow_ops __read_mostly = {
	.kind		= "flow",
	.classify	= flow_classify,
	.init		= flow_init,
	.destroy	= flow_destroy,
	.change		= flow_change,
	.delete		= flow_delete,
	.get		= flow_get,
<<<<<<< HEAD
	.put		= flow_put,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.dump		= flow_dump,
	.walk		= flow_walk,
	.owner		= THIS_MODULE,
};
<<<<<<< HEAD
=======
MODULE_ALIAS_NET_CLS("flow");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int __init cls_flow_init(void)
{
	return register_tcf_proto_ops(&cls_flow_ops);
}

static void __exit cls_flow_exit(void)
{
	unregister_tcf_proto_ops(&cls_flow_ops);
}

module_init(cls_flow_init);
module_exit(cls_flow_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Patrick McHardy <kaber@trash.net>");
MODULE_DESCRIPTION("TC flow classifier");
