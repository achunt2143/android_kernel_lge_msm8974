<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * INET		An implementation of the TCP/IP protocol suite for the LINUX
 *		operating system.  INET is implemented using the  BSD Socket
 *		interface as the means of communication with the user level.
 *
 *		IPv4 Forwarding Information Base: policy rules.
 *
 * Authors:	Alexey Kuznetsov, <kuznet@ms2.inr.ac.ru>
 *		Thomas Graf <tgraf@suug.ch>
 *
<<<<<<< HEAD
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Fixes:
 *		Rani Assaf	:	local_rule cannot be deleted
 *		Marc Boucher	:	routing by fwmark
 */

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/netlink.h>
#include <linux/inetdevice.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/rcupdate.h>
#include <linux/export.h>
<<<<<<< HEAD
=======
#include <net/inet_dscp.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <net/ip.h>
#include <net/route.h>
#include <net/tcp.h>
#include <net/ip_fib.h>
<<<<<<< HEAD
#include <net/fib_rules.h>
=======
#include <net/nexthop.h>
#include <net/fib_rules.h>
#include <linux/indirect_call_wrapper.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct fib4_rule {
	struct fib_rule		common;
	u8			dst_len;
	u8			src_len;
<<<<<<< HEAD
	u8			tos;
=======
	dscp_t			dscp;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	__be32			src;
	__be32			srcmask;
	__be32			dst;
	__be32			dstmask;
#ifdef CONFIG_IP_ROUTE_CLASSID
	u32			tclassid;
#endif
};

<<<<<<< HEAD
#ifdef CONFIG_IP_ROUTE_CLASSID
u32 fib_rules_tclass(const struct fib_result *res)
{
	return res->r ? ((struct fib4_rule *) res->r)->tclassid : 0;
}
#endif

int fib_lookup(struct net *net, struct flowi4 *flp, struct fib_result *res)
{
	struct fib_lookup_arg arg = {
		.result = res,
		.flags = FIB_LOOKUP_NOREF,
	};
	int err;

	err = fib_rules_lookup(net->ipv4.rules_ops, flowi4_to_flowi(flp), 0, &arg);
	res->r = arg.rule;

	return err;
}
EXPORT_SYMBOL_GPL(fib_lookup);

static int fib4_rule_action(struct fib_rule *rule, struct flowi *flp,
			    int flags, struct fib_lookup_arg *arg)
{
	int err = -EAGAIN;
	struct fib_table *tbl;
=======
static bool fib4_rule_matchall(const struct fib_rule *rule)
{
	struct fib4_rule *r = container_of(rule, struct fib4_rule, common);

	if (r->dst_len || r->src_len || r->dscp)
		return false;
	return fib_rule_matchall(rule);
}

bool fib4_rule_default(const struct fib_rule *rule)
{
	if (!fib4_rule_matchall(rule) || rule->action != FR_ACT_TO_TBL ||
	    rule->l3mdev)
		return false;
	if (rule->table != RT_TABLE_LOCAL && rule->table != RT_TABLE_MAIN &&
	    rule->table != RT_TABLE_DEFAULT)
		return false;
	return true;
}
EXPORT_SYMBOL_GPL(fib4_rule_default);

int fib4_rules_dump(struct net *net, struct notifier_block *nb,
		    struct netlink_ext_ack *extack)
{
	return fib_rules_dump(net, nb, AF_INET, extack);
}

unsigned int fib4_rules_seq_read(struct net *net)
{
	return fib_rules_seq_read(net, AF_INET);
}

int __fib_lookup(struct net *net, struct flowi4 *flp,
		 struct fib_result *res, unsigned int flags)
{
	struct fib_lookup_arg arg = {
		.result = res,
		.flags = flags,
	};
	int err;

	/* update flow if oif or iif point to device enslaved to l3mdev */
	l3mdev_update_flow(net, flowi4_to_flowi(flp));

	err = fib_rules_lookup(net->ipv4.rules_ops, flowi4_to_flowi(flp), 0, &arg);
#ifdef CONFIG_IP_ROUTE_CLASSID
	if (arg.rule)
		res->tclassid = ((struct fib4_rule *)arg.rule)->tclassid;
	else
		res->tclassid = 0;
#endif

	if (err == -ESRCH)
		err = -ENETUNREACH;

	return err;
}
EXPORT_SYMBOL_GPL(__fib_lookup);

INDIRECT_CALLABLE_SCOPE int fib4_rule_action(struct fib_rule *rule,
					     struct flowi *flp, int flags,
					     struct fib_lookup_arg *arg)
{
	int err = -EAGAIN;
	struct fib_table *tbl;
	u32 tb_id;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (rule->action) {
	case FR_ACT_TO_TBL:
		break;

	case FR_ACT_UNREACHABLE:
<<<<<<< HEAD
		err = -ENETUNREACH;
		goto errout;

	case FR_ACT_PROHIBIT:
		err = -EACCES;
		goto errout;

	case FR_ACT_BLACKHOLE:
	default:
		err = -EINVAL;
		goto errout;
	}

	tbl = fib_get_table(rule->fr_net, rule->table);
	if (!tbl)
		goto errout;

	err = fib_table_lookup(tbl, &flp->u.ip4, (struct fib_result *) arg->result, arg->flags);
	if (err > 0)
		err = -EAGAIN;
errout:
	return err;
}


static int fib4_rule_match(struct fib_rule *rule, struct flowi *fl, int flags)
=======
		return -ENETUNREACH;

	case FR_ACT_PROHIBIT:
		return -EACCES;

	case FR_ACT_BLACKHOLE:
	default:
		return -EINVAL;
	}

	rcu_read_lock();

	tb_id = fib_rule_get_table(rule, arg);
	tbl = fib_get_table(rule->fr_net, tb_id);
	if (tbl)
		err = fib_table_lookup(tbl, &flp->u.ip4,
				       (struct fib_result *)arg->result,
				       arg->flags);

	rcu_read_unlock();
	return err;
}

INDIRECT_CALLABLE_SCOPE bool fib4_rule_suppress(struct fib_rule *rule,
						int flags,
						struct fib_lookup_arg *arg)
{
	struct fib_result *result = arg->result;
	struct net_device *dev = NULL;

	if (result->fi) {
		struct fib_nh_common *nhc = fib_info_nhc(result->fi, 0);

		dev = nhc->nhc_dev;
	}

	/* do not accept result if the route does
	 * not meet the required prefix length
	 */
	if (result->prefixlen <= rule->suppress_prefixlen)
		goto suppress_route;

	/* do not accept result if the route uses a device
	 * belonging to a forbidden interface group
	 */
	if (rule->suppress_ifgroup != -1 && dev && dev->group == rule->suppress_ifgroup)
		goto suppress_route;

	return false;

suppress_route:
	if (!(arg->flags & FIB_LOOKUP_NOREF))
		fib_info_put(result->fi);
	return true;
}

INDIRECT_CALLABLE_SCOPE int fib4_rule_match(struct fib_rule *rule,
					    struct flowi *fl, int flags)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct fib4_rule *r = (struct fib4_rule *) rule;
	struct flowi4 *fl4 = &fl->u.ip4;
	__be32 daddr = fl4->daddr;
	__be32 saddr = fl4->saddr;

	if (((saddr ^ r->src) & r->srcmask) ||
	    ((daddr ^ r->dst) & r->dstmask))
		return 0;

<<<<<<< HEAD
	if (r->tos && (r->tos != fl4->flowi4_tos))
=======
	if (r->dscp && r->dscp != inet_dsfield_to_dscp(fl4->flowi4_tos))
		return 0;

	if (rule->ip_proto && (rule->ip_proto != fl4->flowi4_proto))
		return 0;

	if (fib_rule_port_range_set(&rule->sport_range) &&
	    !fib_rule_port_inrange(&rule->sport_range, fl4->fl4_sport))
		return 0;

	if (fib_rule_port_range_set(&rule->dport_range) &&
	    !fib_rule_port_inrange(&rule->dport_range, fl4->fl4_dport))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;

	return 1;
}

static struct fib_table *fib_empty_table(struct net *net)
{
<<<<<<< HEAD
	u32 id;

	for (id = 1; id <= RT_TABLE_MAX; id++)
		if (fib_get_table(net, id) == NULL)
			return fib_new_table(net, id);
	return NULL;
}

static const struct nla_policy fib4_rule_policy[FRA_MAX+1] = {
	FRA_GENERIC_POLICY,
	[FRA_FLOW]	= { .type = NLA_U32 },
};

static int fib4_rule_configure(struct fib_rule *rule, struct sk_buff *skb,
			       struct fib_rule_hdr *frh,
			       struct nlattr **tb)
=======
	u32 id = 1;

	while (1) {
		if (!fib_get_table(net, id))
			return fib_new_table(net, id);

		if (id++ == RT_TABLE_MAX)
			break;
	}
	return NULL;
}

static int fib4_rule_configure(struct fib_rule *rule, struct sk_buff *skb,
			       struct fib_rule_hdr *frh,
			       struct nlattr **tb,
			       struct netlink_ext_ack *extack)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct net *net = sock_net(skb->sk);
	int err = -EINVAL;
	struct fib4_rule *rule4 = (struct fib4_rule *) rule;

<<<<<<< HEAD
	if (frh->tos & ~IPTOS_TOS_MASK)
		goto errout;

	if (rule->table == RT_TABLE_UNSPEC) {
=======
	if (!inet_validate_dscp(frh->tos)) {
		NL_SET_ERR_MSG(extack,
			       "Invalid dsfield (tos): ECN bits must be 0");
		goto errout;
	}
	/* IPv4 currently doesn't handle high order DSCP bits correctly */
	if (frh->tos & ~IPTOS_TOS_MASK) {
		NL_SET_ERR_MSG(extack, "Invalid tos");
		goto errout;
	}
	rule4->dscp = inet_dsfield_to_dscp(frh->tos);

	/* split local/main if they are not already split */
	err = fib_unmerge(net);
	if (err)
		goto errout;

	if (rule->table == RT_TABLE_UNSPEC && !rule->l3mdev) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (rule->action == FR_ACT_TO_TBL) {
			struct fib_table *table;

			table = fib_empty_table(net);
<<<<<<< HEAD
			if (table == NULL) {
=======
			if (!table) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				err = -ENOBUFS;
				goto errout;
			}

			rule->table = table->tb_id;
		}
	}

	if (frh->src_len)
<<<<<<< HEAD
		rule4->src = nla_get_be32(tb[FRA_SRC]);

	if (frh->dst_len)
		rule4->dst = nla_get_be32(tb[FRA_DST]);

#ifdef CONFIG_IP_ROUTE_CLASSID
	if (tb[FRA_FLOW])
		rule4->tclassid = nla_get_u32(tb[FRA_FLOW]);
#endif

=======
		rule4->src = nla_get_in_addr(tb[FRA_SRC]);

	if (frh->dst_len)
		rule4->dst = nla_get_in_addr(tb[FRA_DST]);

#ifdef CONFIG_IP_ROUTE_CLASSID
	if (tb[FRA_FLOW]) {
		rule4->tclassid = nla_get_u32(tb[FRA_FLOW]);
		if (rule4->tclassid)
			atomic_inc(&net->ipv4.fib_num_tclassid_users);
	}
#endif

	if (fib_rule_requires_fldissect(rule))
		net->ipv4.fib_rules_require_fldissect++;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rule4->src_len = frh->src_len;
	rule4->srcmask = inet_make_mask(rule4->src_len);
	rule4->dst_len = frh->dst_len;
	rule4->dstmask = inet_make_mask(rule4->dst_len);
<<<<<<< HEAD
	rule4->tos = frh->tos;
=======

	net->ipv4.fib_has_custom_rules = true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	err = 0;
errout:
	return err;
}

<<<<<<< HEAD
=======
static int fib4_rule_delete(struct fib_rule *rule)
{
	struct net *net = rule->fr_net;
	int err;

	/* split local/main if they are not already split */
	err = fib_unmerge(net);
	if (err)
		goto errout;

#ifdef CONFIG_IP_ROUTE_CLASSID
	if (((struct fib4_rule *)rule)->tclassid)
		atomic_dec(&net->ipv4.fib_num_tclassid_users);
#endif
	net->ipv4.fib_has_custom_rules = true;

	if (net->ipv4.fib_rules_require_fldissect &&
	    fib_rule_requires_fldissect(rule))
		net->ipv4.fib_rules_require_fldissect--;
errout:
	return err;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int fib4_rule_compare(struct fib_rule *rule, struct fib_rule_hdr *frh,
			     struct nlattr **tb)
{
	struct fib4_rule *rule4 = (struct fib4_rule *) rule;

	if (frh->src_len && (rule4->src_len != frh->src_len))
		return 0;

	if (frh->dst_len && (rule4->dst_len != frh->dst_len))
		return 0;

<<<<<<< HEAD
	if (frh->tos && (rule4->tos != frh->tos))
=======
	if (frh->tos && inet_dscp_to_dsfield(rule4->dscp) != frh->tos)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;

#ifdef CONFIG_IP_ROUTE_CLASSID
	if (tb[FRA_FLOW] && (rule4->tclassid != nla_get_u32(tb[FRA_FLOW])))
		return 0;
#endif

<<<<<<< HEAD
	if (frh->src_len && (rule4->src != nla_get_be32(tb[FRA_SRC])))
		return 0;

	if (frh->dst_len && (rule4->dst != nla_get_be32(tb[FRA_DST])))
=======
	if (frh->src_len && (rule4->src != nla_get_in_addr(tb[FRA_SRC])))
		return 0;

	if (frh->dst_len && (rule4->dst != nla_get_in_addr(tb[FRA_DST])))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;

	return 1;
}

static int fib4_rule_fill(struct fib_rule *rule, struct sk_buff *skb,
			  struct fib_rule_hdr *frh)
{
	struct fib4_rule *rule4 = (struct fib4_rule *) rule;

	frh->dst_len = rule4->dst_len;
	frh->src_len = rule4->src_len;
<<<<<<< HEAD
	frh->tos = rule4->tos;

	if (rule4->dst_len)
		NLA_PUT_BE32(skb, FRA_DST, rule4->dst);

	if (rule4->src_len)
		NLA_PUT_BE32(skb, FRA_SRC, rule4->src);

#ifdef CONFIG_IP_ROUTE_CLASSID
	if (rule4->tclassid)
		NLA_PUT_U32(skb, FRA_FLOW, rule4->tclassid);
=======
	frh->tos = inet_dscp_to_dsfield(rule4->dscp);

	if ((rule4->dst_len &&
	     nla_put_in_addr(skb, FRA_DST, rule4->dst)) ||
	    (rule4->src_len &&
	     nla_put_in_addr(skb, FRA_SRC, rule4->src)))
		goto nla_put_failure;
#ifdef CONFIG_IP_ROUTE_CLASSID
	if (rule4->tclassid &&
	    nla_put_u32(skb, FRA_FLOW, rule4->tclassid))
		goto nla_put_failure;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
	return 0;

nla_put_failure:
	return -ENOBUFS;
}

static size_t fib4_rule_nlmsg_payload(struct fib_rule *rule)
{
	return nla_total_size(4) /* dst */
	       + nla_total_size(4) /* src */
	       + nla_total_size(4); /* flow */
}

static void fib4_rule_flush_cache(struct fib_rules_ops *ops)
{
<<<<<<< HEAD
	rt_cache_flush(ops->fro_net, -1);
}

static const struct fib_rules_ops __net_initdata fib4_rules_ops_template = {
=======
	rt_cache_flush(ops->fro_net);
}

static const struct fib_rules_ops __net_initconst fib4_rules_ops_template = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.family		= AF_INET,
	.rule_size	= sizeof(struct fib4_rule),
	.addr_size	= sizeof(u32),
	.action		= fib4_rule_action,
<<<<<<< HEAD
	.match		= fib4_rule_match,
	.configure	= fib4_rule_configure,
	.compare	= fib4_rule_compare,
	.fill		= fib4_rule_fill,
	.default_pref	= fib_default_rule_pref,
	.nlmsg_payload	= fib4_rule_nlmsg_payload,
	.flush_cache	= fib4_rule_flush_cache,
	.nlgroup	= RTNLGRP_IPV4_RULE,
	.policy		= fib4_rule_policy,
=======
	.suppress	= fib4_rule_suppress,
	.match		= fib4_rule_match,
	.configure	= fib4_rule_configure,
	.delete		= fib4_rule_delete,
	.compare	= fib4_rule_compare,
	.fill		= fib4_rule_fill,
	.nlmsg_payload	= fib4_rule_nlmsg_payload,
	.flush_cache	= fib4_rule_flush_cache,
	.nlgroup	= RTNLGRP_IPV4_RULE,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.owner		= THIS_MODULE,
};

static int fib_default_rules_init(struct fib_rules_ops *ops)
{
	int err;

<<<<<<< HEAD
	err = fib_default_rule_add(ops, 0, RT_TABLE_LOCAL, 0);
	if (err < 0)
		return err;
	err = fib_default_rule_add(ops, 0x7FFE, RT_TABLE_MAIN, 0);
	if (err < 0)
		return err;
	err = fib_default_rule_add(ops, 0x7FFF, RT_TABLE_DEFAULT, 0);
=======
	err = fib_default_rule_add(ops, 0, RT_TABLE_LOCAL);
	if (err < 0)
		return err;
	err = fib_default_rule_add(ops, 0x7FFE, RT_TABLE_MAIN);
	if (err < 0)
		return err;
	err = fib_default_rule_add(ops, 0x7FFF, RT_TABLE_DEFAULT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err < 0)
		return err;
	return 0;
}

int __net_init fib4_rules_init(struct net *net)
{
	int err;
	struct fib_rules_ops *ops;

	ops = fib_rules_register(&fib4_rules_ops_template, net);
	if (IS_ERR(ops))
		return PTR_ERR(ops);

	err = fib_default_rules_init(ops);
	if (err < 0)
		goto fail;
	net->ipv4.rules_ops = ops;
<<<<<<< HEAD
=======
	net->ipv4.fib_has_custom_rules = false;
	net->ipv4.fib_rules_require_fldissect = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;

fail:
	/* also cleans all rules already added */
	fib_rules_unregister(ops);
	return err;
}

void __net_exit fib4_rules_exit(struct net *net)
{
	fib_rules_unregister(net->ipv4.rules_ops);
}
