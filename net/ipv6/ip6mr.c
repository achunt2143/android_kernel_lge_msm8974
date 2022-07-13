<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *	Linux IPv6 multicast routing support for BSD pim6sd
 *	Based on net/ipv4/ipmr.c.
 *
 *	(c) 2004 Mickael Hoerdt, <hoerdt@clarinet.u-strasbg.fr>
 *		LSIIT Laboratory, Strasbourg, France
 *	(c) 2004 Jean-Philippe Andriot, <jean-philippe.andriot@6WIND.com>
 *		6WIND, Paris, France
 *	Copyright (C)2007,2008 USAGI/WIDE Project
 *		YOSHIFUJI Hideaki <yoshfuji@linux-ipv6.org>
<<<<<<< HEAD
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
 *
 */

#include <asm/uaccess.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <linux/errno.h>
#include <linux/timer.h>
=======
 */

#include <linux/uaccess.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <linux/errno.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/mm.h>
#include <linux/kernel.h>
#include <linux/fcntl.h>
#include <linux/stat.h>
#include <linux/socket.h>
#include <linux/inet.h>
#include <linux/netdevice.h>
#include <linux/inetdevice.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/init.h>
<<<<<<< HEAD
#include <linux/slab.h>
#include <linux/compat.h>
#include <net/protocol.h>
#include <linux/skbuff.h>
#include <net/sock.h>
=======
#include <linux/compat.h>
#include <linux/rhashtable.h>
#include <net/protocol.h>
#include <linux/skbuff.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <net/raw.h>
#include <linux/notifier.h>
#include <linux/if_arp.h>
#include <net/checksum.h>
#include <net/netlink.h>
#include <net/fib_rules.h>

#include <net/ipv6.h>
#include <net/ip6_route.h>
#include <linux/mroute6.h>
#include <linux/pim.h>
#include <net/addrconf.h>
#include <linux/netfilter_ipv6.h>
#include <linux/export.h>
#include <net/ip6_checksum.h>
<<<<<<< HEAD

struct mr6_table {
	struct list_head	list;
#ifdef CONFIG_NET_NS
	struct net		*net;
#endif
	u32			id;
	struct sock		*mroute6_sk;
	struct timer_list	ipmr_expire_timer;
	struct list_head	mfc6_unres_queue;
	struct list_head	mfc6_cache_array[MFC6_LINES];
	struct mif_device	vif6_table[MAXMIFS];
	int			maxvif;
	atomic_t		cache_resolve_queue_len;
	int			mroute_do_assert;
	int			mroute_do_pim;
#ifdef CONFIG_IPV6_PIMSM_V2
	int			mroute_reg_vif_num;
#endif
};
=======
#include <linux/netconf.h>
#include <net/ip_tunnels.h>

#include <linux/nospec.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct ip6mr_rule {
	struct fib_rule		common;
};

struct ip6mr_result {
<<<<<<< HEAD
	struct mr6_table	*mrt;
=======
	struct mr_table	*mrt;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/* Big lock, protecting vif table, mrt cache and mroute socket state.
   Note that the changes are semaphored via rtnl_lock.
 */

<<<<<<< HEAD
static DEFINE_RWLOCK(mrt_lock);

/*
 *	Multicast router control variables
 */

#define MIF_EXISTS(_mrt, _idx) ((_mrt)->vif6_table[_idx].dev != NULL)
=======
static DEFINE_SPINLOCK(mrt_lock);

static struct net_device *vif_dev_read(const struct vif_device *vif)
{
	return rcu_dereference(vif->dev);
}

/* Multicast router control variables */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Special spinlock for queue of unresolved entries */
static DEFINE_SPINLOCK(mfc_unres_lock);

/* We return to original Alan's scheme. Hash table of resolved
   entries is changed only in process context and protected
   with weak lock mrt_lock. Queue of unresolved entries is protected
   with strong spinlock mfc_unres_lock.

   In this case data path is free of exclusive locks at all.
 */

static struct kmem_cache *mrt_cachep __read_mostly;

<<<<<<< HEAD
static struct mr6_table *ip6mr_new_table(struct net *net, u32 id);
static void ip6mr_free_table(struct mr6_table *mrt);

static int ip6_mr_forward(struct net *net, struct mr6_table *mrt,
			  struct sk_buff *skb, struct mfc6_cache *cache);
static int ip6mr_cache_report(struct mr6_table *mrt, struct sk_buff *pkt,
			      mifi_t mifi, int assert);
static int __ip6mr_fill_mroute(struct mr6_table *mrt, struct sk_buff *skb,
			       struct mfc6_cache *c, struct rtmsg *rtm);
static int ip6mr_rtm_dumproute(struct sk_buff *skb,
			       struct netlink_callback *cb);
static void mroute_clean_tables(struct mr6_table *mrt, bool all);
static void ipmr_expire_process(unsigned long arg);

#ifdef CONFIG_IPV6_MROUTE_MULTIPLE_TABLES
#define ip6mr_for_each_table(mrt, net) \
	list_for_each_entry_rcu(mrt, &net->ipv6.mr6_tables, list)

static struct mr6_table *ip6mr_get_table(struct net *net, u32 id)
{
	struct mr6_table *mrt;
=======
static struct mr_table *ip6mr_new_table(struct net *net, u32 id);
static void ip6mr_free_table(struct mr_table *mrt);

static void ip6_mr_forward(struct net *net, struct mr_table *mrt,
			   struct net_device *dev, struct sk_buff *skb,
			   struct mfc6_cache *cache);
static int ip6mr_cache_report(const struct mr_table *mrt, struct sk_buff *pkt,
			      mifi_t mifi, int assert);
static void mr6_netlink_event(struct mr_table *mrt, struct mfc6_cache *mfc,
			      int cmd);
static void mrt6msg_netlink_event(const struct mr_table *mrt, struct sk_buff *pkt);
static int ip6mr_rtm_getroute(struct sk_buff *in_skb, struct nlmsghdr *nlh,
			      struct netlink_ext_ack *extack);
static int ip6mr_rtm_dumproute(struct sk_buff *skb,
			       struct netlink_callback *cb);
static void mroute_clean_tables(struct mr_table *mrt, int flags);
static void ipmr_expire_process(struct timer_list *t);

#ifdef CONFIG_IPV6_MROUTE_MULTIPLE_TABLES
#define ip6mr_for_each_table(mrt, net) \
	list_for_each_entry_rcu(mrt, &net->ipv6.mr6_tables, list, \
				lockdep_rtnl_is_held() || \
				list_empty(&net->ipv6.mr6_tables))

static struct mr_table *ip6mr_mr_table_iter(struct net *net,
					    struct mr_table *mrt)
{
	struct mr_table *ret;

	if (!mrt)
		ret = list_entry_rcu(net->ipv6.mr6_tables.next,
				     struct mr_table, list);
	else
		ret = list_entry_rcu(mrt->list.next,
				     struct mr_table, list);

	if (&ret->list == &net->ipv6.mr6_tables)
		return NULL;
	return ret;
}

static struct mr_table *ip6mr_get_table(struct net *net, u32 id)
{
	struct mr_table *mrt;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ip6mr_for_each_table(mrt, net) {
		if (mrt->id == id)
			return mrt;
	}
	return NULL;
}

static int ip6mr_fib_lookup(struct net *net, struct flowi6 *flp6,
<<<<<<< HEAD
			    struct mr6_table **mrt)
=======
			    struct mr_table **mrt)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int err;
	struct ip6mr_result res;
	struct fib_lookup_arg arg = {
		.result = &res,
		.flags = FIB_LOOKUP_NOREF,
	};

<<<<<<< HEAD
=======
	/* update flow if oif or iif point to device enslaved to l3mdev */
	l3mdev_update_flow(net, flowi6_to_flowi(flp6));

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	err = fib_rules_lookup(net->ipv6.mr6_rules_ops,
			       flowi6_to_flowi(flp6), 0, &arg);
	if (err < 0)
		return err;
	*mrt = res.mrt;
	return 0;
}

static int ip6mr_rule_action(struct fib_rule *rule, struct flowi *flp,
			     int flags, struct fib_lookup_arg *arg)
{
	struct ip6mr_result *res = arg->result;
<<<<<<< HEAD
	struct mr6_table *mrt;
=======
	struct mr_table *mrt;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (rule->action) {
	case FR_ACT_TO_TBL:
		break;
	case FR_ACT_UNREACHABLE:
		return -ENETUNREACH;
	case FR_ACT_PROHIBIT:
		return -EACCES;
	case FR_ACT_BLACKHOLE:
	default:
		return -EINVAL;
	}

<<<<<<< HEAD
	mrt = ip6mr_get_table(rule->fr_net, rule->table);
	if (mrt == NULL)
=======
	arg->table = fib_rule_get_table(rule, arg);

	mrt = ip6mr_get_table(rule->fr_net, arg->table);
	if (!mrt)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EAGAIN;
	res->mrt = mrt;
	return 0;
}

static int ip6mr_rule_match(struct fib_rule *rule, struct flowi *flp, int flags)
{
	return 1;
}

<<<<<<< HEAD
static const struct nla_policy ip6mr_rule_policy[FRA_MAX + 1] = {
	FRA_GENERIC_POLICY,
};

static int ip6mr_rule_configure(struct fib_rule *rule, struct sk_buff *skb,
				struct fib_rule_hdr *frh, struct nlattr **tb)
=======
static int ip6mr_rule_configure(struct fib_rule *rule, struct sk_buff *skb,
				struct fib_rule_hdr *frh, struct nlattr **tb,
				struct netlink_ext_ack *extack)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return 0;
}

static int ip6mr_rule_compare(struct fib_rule *rule, struct fib_rule_hdr *frh,
			      struct nlattr **tb)
{
	return 1;
}

static int ip6mr_rule_fill(struct fib_rule *rule, struct sk_buff *skb,
			   struct fib_rule_hdr *frh)
{
	frh->dst_len = 0;
	frh->src_len = 0;
	frh->tos     = 0;
	return 0;
}

<<<<<<< HEAD
static const struct fib_rules_ops __net_initdata ip6mr_rules_ops_template = {
=======
static const struct fib_rules_ops __net_initconst ip6mr_rules_ops_template = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.family		= RTNL_FAMILY_IP6MR,
	.rule_size	= sizeof(struct ip6mr_rule),
	.addr_size	= sizeof(struct in6_addr),
	.action		= ip6mr_rule_action,
	.match		= ip6mr_rule_match,
	.configure	= ip6mr_rule_configure,
	.compare	= ip6mr_rule_compare,
<<<<<<< HEAD
	.default_pref	= fib_default_rule_pref,
	.fill		= ip6mr_rule_fill,
	.nlgroup	= RTNLGRP_IPV6_RULE,
	.policy		= ip6mr_rule_policy,
=======
	.fill		= ip6mr_rule_fill,
	.nlgroup	= RTNLGRP_IPV6_RULE,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.owner		= THIS_MODULE,
};

static int __net_init ip6mr_rules_init(struct net *net)
{
	struct fib_rules_ops *ops;
<<<<<<< HEAD
	struct mr6_table *mrt;
=======
	struct mr_table *mrt;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int err;

	ops = fib_rules_register(&ip6mr_rules_ops_template, net);
	if (IS_ERR(ops))
		return PTR_ERR(ops);

	INIT_LIST_HEAD(&net->ipv6.mr6_tables);

	mrt = ip6mr_new_table(net, RT6_TABLE_DFLT);
<<<<<<< HEAD
	if (mrt == NULL) {
		err = -ENOMEM;
		goto err1;
	}

	err = fib_default_rule_add(ops, 0x7fff, RT6_TABLE_DFLT, 0);
=======
	if (IS_ERR(mrt)) {
		err = PTR_ERR(mrt);
		goto err1;
	}

	err = fib_default_rule_add(ops, 0x7fff, RT6_TABLE_DFLT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err < 0)
		goto err2;

	net->ipv6.mr6_rules_ops = ops;
	return 0;

err2:
<<<<<<< HEAD
	kfree(mrt);
=======
	rtnl_lock();
	ip6mr_free_table(mrt);
	rtnl_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
err1:
	fib_rules_unregister(ops);
	return err;
}

static void __net_exit ip6mr_rules_exit(struct net *net)
{
<<<<<<< HEAD
	struct mr6_table *mrt, *next;

	rtnl_lock();
=======
	struct mr_table *mrt, *next;

	ASSERT_RTNL();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	list_for_each_entry_safe(mrt, next, &net->ipv6.mr6_tables, list) {
		list_del(&mrt->list);
		ip6mr_free_table(mrt);
	}
<<<<<<< HEAD
	rtnl_unlock();
	fib_rules_unregister(net->ipv6.mr6_rules_ops);
}
=======
	fib_rules_unregister(net->ipv6.mr6_rules_ops);
}

static int ip6mr_rules_dump(struct net *net, struct notifier_block *nb,
			    struct netlink_ext_ack *extack)
{
	return fib_rules_dump(net, nb, RTNL_FAMILY_IP6MR, extack);
}

static unsigned int ip6mr_rules_seq_read(struct net *net)
{
	return fib_rules_seq_read(net, RTNL_FAMILY_IP6MR);
}

bool ip6mr_rule_default(const struct fib_rule *rule)
{
	return fib_rule_matchall(rule) && rule->action == FR_ACT_TO_TBL &&
	       rule->table == RT6_TABLE_DFLT && !rule->l3mdev;
}
EXPORT_SYMBOL(ip6mr_rule_default);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#else
#define ip6mr_for_each_table(mrt, net) \
	for (mrt = net->ipv6.mrt6; mrt; mrt = NULL)

<<<<<<< HEAD
static struct mr6_table *ip6mr_get_table(struct net *net, u32 id)
=======
static struct mr_table *ip6mr_mr_table_iter(struct net *net,
					    struct mr_table *mrt)
{
	if (!mrt)
		return net->ipv6.mrt6;
	return NULL;
}

static struct mr_table *ip6mr_get_table(struct net *net, u32 id)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return net->ipv6.mrt6;
}

static int ip6mr_fib_lookup(struct net *net, struct flowi6 *flp6,
<<<<<<< HEAD
			    struct mr6_table **mrt)
=======
			    struct mr_table **mrt)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	*mrt = net->ipv6.mrt6;
	return 0;
}

static int __net_init ip6mr_rules_init(struct net *net)
{
<<<<<<< HEAD
	net->ipv6.mrt6 = ip6mr_new_table(net, RT6_TABLE_DFLT);
	return net->ipv6.mrt6 ? 0 : -ENOMEM;
=======
	struct mr_table *mrt;

	mrt = ip6mr_new_table(net, RT6_TABLE_DFLT);
	if (IS_ERR(mrt))
		return PTR_ERR(mrt);
	net->ipv6.mrt6 = mrt;
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void __net_exit ip6mr_rules_exit(struct net *net)
{
<<<<<<< HEAD
	rtnl_lock();
	ip6mr_free_table(net->ipv6.mrt6);
	net->ipv6.mrt6 = NULL;
	rtnl_unlock();
}
#endif

static struct mr6_table *ip6mr_new_table(struct net *net, u32 id)
{
	struct mr6_table *mrt;
	unsigned int i;

	mrt = ip6mr_get_table(net, id);
	if (mrt != NULL)
		return mrt;

	mrt = kzalloc(sizeof(*mrt), GFP_KERNEL);
	if (mrt == NULL)
		return NULL;
	mrt->id = id;
	write_pnet(&mrt->net, net);

	/* Forwarding cache */
	for (i = 0; i < MFC6_LINES; i++)
		INIT_LIST_HEAD(&mrt->mfc6_cache_array[i]);

	INIT_LIST_HEAD(&mrt->mfc6_unres_queue);

	setup_timer(&mrt->ipmr_expire_timer, ipmr_expire_process,
		    (unsigned long)mrt);

#ifdef CONFIG_IPV6_PIMSM_V2
	mrt->mroute_reg_vif_num = -1;
#endif
#ifdef CONFIG_IPV6_MROUTE_MULTIPLE_TABLES
	list_add_tail_rcu(&mrt->list, &net->ipv6.mr6_tables);
#endif
	return mrt;
}

static void ip6mr_free_table(struct mr6_table *mrt)
{
	del_timer_sync(&mrt->ipmr_expire_timer);
	mroute_clean_tables(mrt, true);
=======
	ASSERT_RTNL();
	ip6mr_free_table(net->ipv6.mrt6);
	net->ipv6.mrt6 = NULL;
}

static int ip6mr_rules_dump(struct net *net, struct notifier_block *nb,
			    struct netlink_ext_ack *extack)
{
	return 0;
}

static unsigned int ip6mr_rules_seq_read(struct net *net)
{
	return 0;
}
#endif

static int ip6mr_hash_cmp(struct rhashtable_compare_arg *arg,
			  const void *ptr)
{
	const struct mfc6_cache_cmp_arg *cmparg = arg->key;
	struct mfc6_cache *c = (struct mfc6_cache *)ptr;

	return !ipv6_addr_equal(&c->mf6c_mcastgrp, &cmparg->mf6c_mcastgrp) ||
	       !ipv6_addr_equal(&c->mf6c_origin, &cmparg->mf6c_origin);
}

static const struct rhashtable_params ip6mr_rht_params = {
	.head_offset = offsetof(struct mr_mfc, mnode),
	.key_offset = offsetof(struct mfc6_cache, cmparg),
	.key_len = sizeof(struct mfc6_cache_cmp_arg),
	.nelem_hint = 3,
	.obj_cmpfn = ip6mr_hash_cmp,
	.automatic_shrinking = true,
};

static void ip6mr_new_table_set(struct mr_table *mrt,
				struct net *net)
{
#ifdef CONFIG_IPV6_MROUTE_MULTIPLE_TABLES
	list_add_tail_rcu(&mrt->list, &net->ipv6.mr6_tables);
#endif
}

static struct mfc6_cache_cmp_arg ip6mr_mr_table_ops_cmparg_any = {
	.mf6c_origin = IN6ADDR_ANY_INIT,
	.mf6c_mcastgrp = IN6ADDR_ANY_INIT,
};

static struct mr_table_ops ip6mr_mr_table_ops = {
	.rht_params = &ip6mr_rht_params,
	.cmparg_any = &ip6mr_mr_table_ops_cmparg_any,
};

static struct mr_table *ip6mr_new_table(struct net *net, u32 id)
{
	struct mr_table *mrt;

	mrt = ip6mr_get_table(net, id);
	if (mrt)
		return mrt;

	return mr_table_alloc(net, id, &ip6mr_mr_table_ops,
			      ipmr_expire_process, ip6mr_new_table_set);
}

static void ip6mr_free_table(struct mr_table *mrt)
{
	timer_shutdown_sync(&mrt->ipmr_expire_timer);
	mroute_clean_tables(mrt, MRT6_FLUSH_MIFS | MRT6_FLUSH_MIFS_STATIC |
				 MRT6_FLUSH_MFC | MRT6_FLUSH_MFC_STATIC);
	rhltable_destroy(&mrt->mfc_hash);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kfree(mrt);
}

#ifdef CONFIG_PROC_FS
<<<<<<< HEAD

struct ipmr_mfc_iter {
	struct seq_net_private p;
	struct mr6_table *mrt;
	struct list_head *cache;
	int ct;
};


static struct mfc6_cache *ipmr_mfc_seq_idx(struct net *net,
					   struct ipmr_mfc_iter *it, loff_t pos)
{
	struct mr6_table *mrt = it->mrt;
	struct mfc6_cache *mfc;

	read_lock(&mrt_lock);
	for (it->ct = 0; it->ct < MFC6_LINES; it->ct++) {
		it->cache = &mrt->mfc6_cache_array[it->ct];
		list_for_each_entry(mfc, it->cache, list)
			if (pos-- == 0)
				return mfc;
	}
	read_unlock(&mrt_lock);

	spin_lock_bh(&mfc_unres_lock);
	it->cache = &mrt->mfc6_unres_queue;
	list_for_each_entry(mfc, it->cache, list)
		if (pos-- == 0)
			return mfc;
	spin_unlock_bh(&mfc_unres_lock);

	it->cache = NULL;
	return NULL;
}

/*
 *	The /proc interfaces to multicast routing /proc/ip6_mr_cache /proc/ip6_mr_vif
 */

struct ipmr_vif_iter {
	struct seq_net_private p;
	struct mr6_table *mrt;
	int ct;
};

static struct mif_device *ip6mr_vif_seq_idx(struct net *net,
					    struct ipmr_vif_iter *iter,
					    loff_t pos)
{
	struct mr6_table *mrt = iter->mrt;

	for (iter->ct = 0; iter->ct < mrt->maxvif; ++iter->ct) {
		if (!MIF_EXISTS(mrt, iter->ct))
			continue;
		if (pos-- == 0)
			return &mrt->vif6_table[iter->ct];
	}
	return NULL;
}

static void *ip6mr_vif_seq_start(struct seq_file *seq, loff_t *pos)
	__acquires(mrt_lock)
{
	struct ipmr_vif_iter *iter = seq->private;
	struct net *net = seq_file_net(seq);
	struct mr6_table *mrt;

	mrt = ip6mr_get_table(net, RT6_TABLE_DFLT);
	if (mrt == NULL)
=======
/* The /proc interfaces to multicast routing
 * /proc/ip6_mr_cache /proc/ip6_mr_vif
 */

static void *ip6mr_vif_seq_start(struct seq_file *seq, loff_t *pos)
	__acquires(RCU)
{
	struct mr_vif_iter *iter = seq->private;
	struct net *net = seq_file_net(seq);
	struct mr_table *mrt;

	mrt = ip6mr_get_table(net, RT6_TABLE_DFLT);
	if (!mrt)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return ERR_PTR(-ENOENT);

	iter->mrt = mrt;

<<<<<<< HEAD
	read_lock(&mrt_lock);
	return *pos ? ip6mr_vif_seq_idx(net, seq->private, *pos - 1)
		: SEQ_START_TOKEN;
}

static void *ip6mr_vif_seq_next(struct seq_file *seq, void *v, loff_t *pos)
{
	struct ipmr_vif_iter *iter = seq->private;
	struct net *net = seq_file_net(seq);
	struct mr6_table *mrt = iter->mrt;

	++*pos;
	if (v == SEQ_START_TOKEN)
		return ip6mr_vif_seq_idx(net, iter, 0);

	while (++iter->ct < mrt->maxvif) {
		if (!MIF_EXISTS(mrt, iter->ct))
			continue;
		return &mrt->vif6_table[iter->ct];
	}
	return NULL;
}

static void ip6mr_vif_seq_stop(struct seq_file *seq, void *v)
	__releases(mrt_lock)
{
	read_unlock(&mrt_lock);
=======
	rcu_read_lock();
	return mr_vif_seq_start(seq, pos);
}

static void ip6mr_vif_seq_stop(struct seq_file *seq, void *v)
	__releases(RCU)
{
	rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int ip6mr_vif_seq_show(struct seq_file *seq, void *v)
{
<<<<<<< HEAD
	struct ipmr_vif_iter *iter = seq->private;
	struct mr6_table *mrt = iter->mrt;
=======
	struct mr_vif_iter *iter = seq->private;
	struct mr_table *mrt = iter->mrt;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (v == SEQ_START_TOKEN) {
		seq_puts(seq,
			 "Interface      BytesIn  PktsIn  BytesOut PktsOut Flags\n");
	} else {
<<<<<<< HEAD
		const struct mif_device *vif = v;
		const char *name = vif->dev ? vif->dev->name : "none";

		seq_printf(seq,
			   "%2td %-10s %8ld %7ld  %8ld %7ld %05X\n",
			   vif - mrt->vif6_table,
=======
		const struct vif_device *vif = v;
		const struct net_device *vif_dev;
		const char *name;

		vif_dev = vif_dev_read(vif);
		name = vif_dev ? vif_dev->name : "none";

		seq_printf(seq,
			   "%2td %-10s %8ld %7ld  %8ld %7ld %05X\n",
			   vif - mrt->vif_table,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			   name, vif->bytes_in, vif->pkt_in,
			   vif->bytes_out, vif->pkt_out,
			   vif->flags);
	}
	return 0;
}

static const struct seq_operations ip6mr_vif_seq_ops = {
	.start = ip6mr_vif_seq_start,
<<<<<<< HEAD
	.next  = ip6mr_vif_seq_next,
=======
	.next  = mr_vif_seq_next,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.stop  = ip6mr_vif_seq_stop,
	.show  = ip6mr_vif_seq_show,
};

<<<<<<< HEAD
static int ip6mr_vif_open(struct inode *inode, struct file *file)
{
	return seq_open_net(inode, file, &ip6mr_vif_seq_ops,
			    sizeof(struct ipmr_vif_iter));
}

static const struct file_operations ip6mr_vif_fops = {
	.owner	 = THIS_MODULE,
	.open    = ip6mr_vif_open,
	.read    = seq_read,
	.llseek  = seq_lseek,
	.release = seq_release_net,
};

static void *ipmr_mfc_seq_start(struct seq_file *seq, loff_t *pos)
{
	struct ipmr_mfc_iter *it = seq->private;
	struct net *net = seq_file_net(seq);
	struct mr6_table *mrt;

	mrt = ip6mr_get_table(net, RT6_TABLE_DFLT);
	if (mrt == NULL)
		return ERR_PTR(-ENOENT);

	it->mrt = mrt;
	return *pos ? ipmr_mfc_seq_idx(net, seq->private, *pos - 1)
		: SEQ_START_TOKEN;
}

static void *ipmr_mfc_seq_next(struct seq_file *seq, void *v, loff_t *pos)
{
	struct mfc6_cache *mfc = v;
	struct ipmr_mfc_iter *it = seq->private;
	struct net *net = seq_file_net(seq);
	struct mr6_table *mrt = it->mrt;

	++*pos;

	if (v == SEQ_START_TOKEN)
		return ipmr_mfc_seq_idx(net, seq->private, 0);

	if (mfc->list.next != it->cache)
		return list_entry(mfc->list.next, struct mfc6_cache, list);

	if (it->cache == &mrt->mfc6_unres_queue)
		goto end_of_list;

	BUG_ON(it->cache != &mrt->mfc6_cache_array[it->ct]);

	while (++it->ct < MFC6_LINES) {
		it->cache = &mrt->mfc6_cache_array[it->ct];
		if (list_empty(it->cache))
			continue;
		return list_first_entry(it->cache, struct mfc6_cache, list);
	}

	/* exhausted cache_array, show unresolved */
	read_unlock(&mrt_lock);
	it->cache = &mrt->mfc6_unres_queue;
	it->ct = 0;

	spin_lock_bh(&mfc_unres_lock);
	if (!list_empty(it->cache))
		return list_first_entry(it->cache, struct mfc6_cache, list);

 end_of_list:
	spin_unlock_bh(&mfc_unres_lock);
	it->cache = NULL;

	return NULL;
}

static void ipmr_mfc_seq_stop(struct seq_file *seq, void *v)
{
	struct ipmr_mfc_iter *it = seq->private;
	struct mr6_table *mrt = it->mrt;

	if (it->cache == &mrt->mfc6_unres_queue)
		spin_unlock_bh(&mfc_unres_lock);
	else if (it->cache == mrt->mfc6_cache_array)
		read_unlock(&mrt_lock);
=======
static void *ipmr_mfc_seq_start(struct seq_file *seq, loff_t *pos)
{
	struct net *net = seq_file_net(seq);
	struct mr_table *mrt;

	mrt = ip6mr_get_table(net, RT6_TABLE_DFLT);
	if (!mrt)
		return ERR_PTR(-ENOENT);

	return mr_mfc_seq_start(seq, pos, mrt, &mfc_unres_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int ipmr_mfc_seq_show(struct seq_file *seq, void *v)
{
	int n;

	if (v == SEQ_START_TOKEN) {
		seq_puts(seq,
			 "Group                            "
			 "Origin                           "
			 "Iif      Pkts  Bytes     Wrong  Oifs\n");
	} else {
		const struct mfc6_cache *mfc = v;
<<<<<<< HEAD
		const struct ipmr_mfc_iter *it = seq->private;
		struct mr6_table *mrt = it->mrt;

		seq_printf(seq, "%pI6 %pI6 %-3hd",
			   &mfc->mf6c_mcastgrp, &mfc->mf6c_origin,
			   mfc->mf6c_parent);

		if (it->cache != &mrt->mfc6_unres_queue) {
			seq_printf(seq, " %8lu %8lu %8lu",
				   mfc->mfc_un.res.pkt,
				   mfc->mfc_un.res.bytes,
				   mfc->mfc_un.res.wrong_if);
			for (n = mfc->mfc_un.res.minvif;
			     n < mfc->mfc_un.res.maxvif; n++) {
				if (MIF_EXISTS(mrt, n) &&
				    mfc->mfc_un.res.ttls[n] < 255)
					seq_printf(seq,
						   " %2d:%-3d",
						   n, mfc->mfc_un.res.ttls[n]);
=======
		const struct mr_mfc_iter *it = seq->private;
		struct mr_table *mrt = it->mrt;

		seq_printf(seq, "%pI6 %pI6 %-3hd",
			   &mfc->mf6c_mcastgrp, &mfc->mf6c_origin,
			   mfc->_c.mfc_parent);

		if (it->cache != &mrt->mfc_unres_queue) {
			seq_printf(seq, " %8lu %8lu %8lu",
				   mfc->_c.mfc_un.res.pkt,
				   mfc->_c.mfc_un.res.bytes,
				   mfc->_c.mfc_un.res.wrong_if);
			for (n = mfc->_c.mfc_un.res.minvif;
			     n < mfc->_c.mfc_un.res.maxvif; n++) {
				if (VIF_EXISTS(mrt, n) &&
				    mfc->_c.mfc_un.res.ttls[n] < 255)
					seq_printf(seq,
						   " %2d:%-3d", n,
						   mfc->_c.mfc_un.res.ttls[n]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			}
		} else {
			/* unresolved mfc_caches don't contain
			 * pkt, bytes and wrong_if values
			 */
			seq_printf(seq, " %8lu %8lu %8lu", 0ul, 0ul, 0ul);
		}
		seq_putc(seq, '\n');
	}
	return 0;
}

static const struct seq_operations ipmr_mfc_seq_ops = {
	.start = ipmr_mfc_seq_start,
<<<<<<< HEAD
	.next  = ipmr_mfc_seq_next,
	.stop  = ipmr_mfc_seq_stop,
	.show  = ipmr_mfc_seq_show,
};

static int ipmr_mfc_open(struct inode *inode, struct file *file)
{
	return seq_open_net(inode, file, &ipmr_mfc_seq_ops,
			    sizeof(struct ipmr_mfc_iter));
}

static const struct file_operations ip6mr_mfc_fops = {
	.owner	 = THIS_MODULE,
	.open    = ipmr_mfc_open,
	.read    = seq_read,
	.llseek  = seq_lseek,
	.release = seq_release_net,
};
=======
	.next  = mr_mfc_seq_next,
	.stop  = mr_mfc_seq_stop,
	.show  = ipmr_mfc_seq_show,
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

#ifdef CONFIG_IPV6_PIMSM_V2

static int pim6_rcv(struct sk_buff *skb)
{
	struct pimreghdr *pim;
	struct ipv6hdr   *encap;
	struct net_device  *reg_dev = NULL;
	struct net *net = dev_net(skb->dev);
<<<<<<< HEAD
	struct mr6_table *mrt;
=======
	struct mr_table *mrt;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct flowi6 fl6 = {
		.flowi6_iif	= skb->dev->ifindex,
		.flowi6_mark	= skb->mark,
	};
	int reg_vif_num;

	if (!pskb_may_pull(skb, sizeof(*pim) + sizeof(*encap)))
		goto drop;

	pim = (struct pimreghdr *)skb_transport_header(skb);
<<<<<<< HEAD
	if (pim->type != ((PIM_VERSION << 4) | PIM_REGISTER) ||
=======
	if (pim->type != ((PIM_VERSION << 4) | PIM_TYPE_REGISTER) ||
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	    (pim->flags & PIM_NULL_REGISTER) ||
	    (csum_ipv6_magic(&ipv6_hdr(skb)->saddr, &ipv6_hdr(skb)->daddr,
			     sizeof(*pim), IPPROTO_PIM,
			     csum_partial((void *)pim, sizeof(*pim), 0)) &&
	     csum_fold(skb_checksum(skb, 0, skb->len, 0))))
		goto drop;

	/* check if the inner packet is destined to mcast group */
	encap = (struct ipv6hdr *)(skb_transport_header(skb) +
				   sizeof(*pim));

	if (!ipv6_addr_is_multicast(&encap->daddr) ||
	    encap->payload_len == 0 ||
	    ntohs(encap->payload_len) + sizeof(*pim) > skb->len)
		goto drop;

	if (ip6mr_fib_lookup(net, &fl6, &mrt) < 0)
		goto drop;
<<<<<<< HEAD
	reg_vif_num = mrt->mroute_reg_vif_num;

	read_lock(&mrt_lock);
	if (reg_vif_num >= 0)
		reg_dev = mrt->vif6_table[reg_vif_num].dev;
	if (reg_dev)
		dev_hold(reg_dev);
	read_unlock(&mrt_lock);

	if (reg_dev == NULL)
=======

	/* Pairs with WRITE_ONCE() in mif6_add()/mif6_delete() */
	reg_vif_num = READ_ONCE(mrt->mroute_reg_vif_num);
	if (reg_vif_num >= 0)
		reg_dev = vif_dev_read(&mrt->vif_table[reg_vif_num]);

	if (!reg_dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto drop;

	skb->mac_header = skb->network_header;
	skb_pull(skb, (u8 *)encap - skb->data);
	skb_reset_network_header(skb);
	skb->protocol = htons(ETH_P_IPV6);
	skb->ip_summed = CHECKSUM_NONE;
<<<<<<< HEAD
	skb->pkt_type = PACKET_HOST;

	skb_tunnel_rx(skb, reg_dev);

	netif_rx(skb);

	dev_put(reg_dev);
=======

	skb_tunnel_rx(skb, reg_dev, dev_net(reg_dev));

	netif_rx(skb);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
 drop:
	kfree_skb(skb);
	return 0;
}

static const struct inet6_protocol pim6_protocol = {
	.handler	=	pim6_rcv,
};

/* Service routines creating virtual interfaces: PIMREG */

static netdev_tx_t reg_vif_xmit(struct sk_buff *skb,
				      struct net_device *dev)
{
	struct net *net = dev_net(dev);
<<<<<<< HEAD
	struct mr6_table *mrt;
=======
	struct mr_table *mrt;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct flowi6 fl6 = {
		.flowi6_oif	= dev->ifindex,
		.flowi6_iif	= skb->skb_iif ? : LOOPBACK_IFINDEX,
		.flowi6_mark	= skb->mark,
	};
<<<<<<< HEAD
	int err;

	err = ip6mr_fib_lookup(net, &fl6, &mrt);
	if (err < 0) {
		kfree_skb(skb);
		return err;
	}

	read_lock(&mrt_lock);
	dev->stats.tx_bytes += skb->len;
	dev->stats.tx_packets++;
	ip6mr_cache_report(mrt, skb, mrt->mroute_reg_vif_num, MRT6MSG_WHOLEPKT);
	read_unlock(&mrt_lock);
=======

	if (!pskb_inet_may_pull(skb))
		goto tx_err;

	if (ip6mr_fib_lookup(net, &fl6, &mrt) < 0)
		goto tx_err;

	DEV_STATS_ADD(dev, tx_bytes, skb->len);
	DEV_STATS_INC(dev, tx_packets);
	rcu_read_lock();
	ip6mr_cache_report(mrt, skb, READ_ONCE(mrt->mroute_reg_vif_num),
			   MRT6MSG_WHOLEPKT);
	rcu_read_unlock();
	kfree_skb(skb);
	return NETDEV_TX_OK;

tx_err:
	DEV_STATS_INC(dev, tx_errors);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kfree_skb(skb);
	return NETDEV_TX_OK;
}

<<<<<<< HEAD
static const struct net_device_ops reg_vif_netdev_ops = {
	.ndo_start_xmit	= reg_vif_xmit,
=======
static int reg_vif_get_iflink(const struct net_device *dev)
{
	return 0;
}

static const struct net_device_ops reg_vif_netdev_ops = {
	.ndo_start_xmit	= reg_vif_xmit,
	.ndo_get_iflink = reg_vif_get_iflink,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static void reg_vif_setup(struct net_device *dev)
{
	dev->type		= ARPHRD_PIMREG;
	dev->mtu		= 1500 - sizeof(struct ipv6hdr) - 8;
	dev->flags		= IFF_NOARP;
	dev->netdev_ops		= &reg_vif_netdev_ops;
<<<<<<< HEAD
	dev->destructor		= free_netdev;
	dev->features		|= NETIF_F_NETNS_LOCAL;
}

static struct net_device *ip6mr_reg_vif(struct net *net, struct mr6_table *mrt)
=======
	dev->needs_free_netdev	= true;
	dev->features		|= NETIF_F_NETNS_LOCAL;
}

static struct net_device *ip6mr_reg_vif(struct net *net, struct mr_table *mrt)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct net_device *dev;
	char name[IFNAMSIZ];

	if (mrt->id == RT6_TABLE_DFLT)
		sprintf(name, "pim6reg");
	else
		sprintf(name, "pim6reg%u", mrt->id);

<<<<<<< HEAD
	dev = alloc_netdev(0, name, reg_vif_setup);
	if (dev == NULL)
=======
	dev = alloc_netdev(0, name, NET_NAME_UNKNOWN, reg_vif_setup);
	if (!dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return NULL;

	dev_net_set(dev, net);

	if (register_netdevice(dev)) {
		free_netdev(dev);
		return NULL;
	}
<<<<<<< HEAD
	dev->iflink = 0;

	if (dev_open(dev))
=======

	if (dev_open(dev, NULL))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto failure;

	dev_hold(dev);
	return dev;

failure:
<<<<<<< HEAD
	/* allow the register to be completed before unregistering. */
	rtnl_unlock();
	rtnl_lock();

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unregister_netdevice(dev);
	return NULL;
}
#endif

<<<<<<< HEAD
/*
 *	Delete a VIF entry
 */

static int mif6_delete(struct mr6_table *mrt, int vifi, struct list_head *head)
{
	struct mif_device *v;
=======
static int call_ip6mr_vif_entry_notifiers(struct net *net,
					  enum fib_event_type event_type,
					  struct vif_device *vif,
					  struct net_device *vif_dev,
					  mifi_t vif_index, u32 tb_id)
{
	return mr_call_vif_notifiers(net, RTNL_FAMILY_IP6MR, event_type,
				     vif, vif_dev, vif_index, tb_id,
				     &net->ipv6.ipmr_seq);
}

static int call_ip6mr_mfc_entry_notifiers(struct net *net,
					  enum fib_event_type event_type,
					  struct mfc6_cache *mfc, u32 tb_id)
{
	return mr_call_mfc_notifiers(net, RTNL_FAMILY_IP6MR, event_type,
				     &mfc->_c, tb_id, &net->ipv6.ipmr_seq);
}

/* Delete a VIF entry */
static int mif6_delete(struct mr_table *mrt, int vifi, int notify,
		       struct list_head *head)
{
	struct vif_device *v;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct net_device *dev;
	struct inet6_dev *in6_dev;

	if (vifi < 0 || vifi >= mrt->maxvif)
		return -EADDRNOTAVAIL;

<<<<<<< HEAD
	v = &mrt->vif6_table[vifi];

	write_lock_bh(&mrt_lock);
	dev = v->dev;
	v->dev = NULL;

	if (!dev) {
		write_unlock_bh(&mrt_lock);
		return -EADDRNOTAVAIL;
	}

#ifdef CONFIG_IPV6_PIMSM_V2
	if (vifi == mrt->mroute_reg_vif_num)
		mrt->mroute_reg_vif_num = -1;
=======
	v = &mrt->vif_table[vifi];

	dev = rtnl_dereference(v->dev);
	if (!dev)
		return -EADDRNOTAVAIL;

	call_ip6mr_vif_entry_notifiers(read_pnet(&mrt->net),
				       FIB_EVENT_VIF_DEL, v, dev,
				       vifi, mrt->id);
	spin_lock(&mrt_lock);
	RCU_INIT_POINTER(v->dev, NULL);

#ifdef CONFIG_IPV6_PIMSM_V2
	if (vifi == mrt->mroute_reg_vif_num) {
		/* Pairs with READ_ONCE() in ip6mr_cache_report() and reg_vif_xmit() */
		WRITE_ONCE(mrt->mroute_reg_vif_num, -1);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

	if (vifi + 1 == mrt->maxvif) {
		int tmp;
		for (tmp = vifi - 1; tmp >= 0; tmp--) {
<<<<<<< HEAD
			if (MIF_EXISTS(mrt, tmp))
				break;
		}
		mrt->maxvif = tmp + 1;
	}

	write_unlock_bh(&mrt_lock);
=======
			if (VIF_EXISTS(mrt, tmp))
				break;
		}
		WRITE_ONCE(mrt->maxvif, tmp + 1);
	}

	spin_unlock(&mrt_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	dev_set_allmulti(dev, -1);

	in6_dev = __in6_dev_get(dev);
<<<<<<< HEAD
	if (in6_dev)
		in6_dev->cnf.mc_forwarding--;

	if (v->flags & MIFF_REGISTER)
		unregister_netdevice_queue(dev, head);

	dev_put(dev);
	return 0;
}

static inline void ip6mr_cache_free(struct mfc6_cache *c)
{
	kmem_cache_free(mrt_cachep, c);
=======
	if (in6_dev) {
		atomic_dec(&in6_dev->cnf.mc_forwarding);
		inet6_netconf_notify_devconf(dev_net(dev), RTM_NEWNETCONF,
					     NETCONFA_MC_FORWARDING,
					     dev->ifindex, &in6_dev->cnf);
	}

	if ((v->flags & MIFF_REGISTER) && !notify)
		unregister_netdevice_queue(dev, head);

	netdev_put(dev, &v->dev_tracker);
	return 0;
}

static inline void ip6mr_cache_free_rcu(struct rcu_head *head)
{
	struct mr_mfc *c = container_of(head, struct mr_mfc, rcu);

	kmem_cache_free(mrt_cachep, (struct mfc6_cache *)c);
}

static inline void ip6mr_cache_free(struct mfc6_cache *c)
{
	call_rcu(&c->_c.rcu, ip6mr_cache_free_rcu);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Destroy an unresolved cache entry, killing queued skbs
   and reporting error to netlink readers.
 */

<<<<<<< HEAD
static void ip6mr_destroy_unres(struct mr6_table *mrt, struct mfc6_cache *c)
=======
static void ip6mr_destroy_unres(struct mr_table *mrt, struct mfc6_cache *c)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct net *net = read_pnet(&mrt->net);
	struct sk_buff *skb;

	atomic_dec(&mrt->cache_resolve_queue_len);

<<<<<<< HEAD
	while((skb = skb_dequeue(&c->mfc_un.unres.unresolved)) != NULL) {
		if (ipv6_hdr(skb)->version == 0) {
			struct nlmsghdr *nlh = (struct nlmsghdr *)skb_pull(skb, sizeof(struct ipv6hdr));
			nlh->nlmsg_type = NLMSG_ERROR;
			nlh->nlmsg_len = NLMSG_LENGTH(sizeof(struct nlmsgerr));
			skb_trim(skb, nlh->nlmsg_len);
			((struct nlmsgerr *)NLMSG_DATA(nlh))->error = -ETIMEDOUT;
			rtnl_unicast(skb, net, NETLINK_CB(skb).pid);
=======
	while ((skb = skb_dequeue(&c->_c.mfc_un.unres.unresolved)) != NULL) {
		if (ipv6_hdr(skb)->version == 0) {
			struct nlmsghdr *nlh = skb_pull(skb,
							sizeof(struct ipv6hdr));
			nlh->nlmsg_type = NLMSG_ERROR;
			nlh->nlmsg_len = nlmsg_msg_size(sizeof(struct nlmsgerr));
			skb_trim(skb, nlh->nlmsg_len);
			((struct nlmsgerr *)nlmsg_data(nlh))->error = -ETIMEDOUT;
			rtnl_unicast(skb, net, NETLINK_CB(skb).portid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		} else
			kfree_skb(skb);
	}

	ip6mr_cache_free(c);
}


/* Timer process for all the unresolved queue. */

<<<<<<< HEAD
static void ipmr_do_expire_process(struct mr6_table *mrt)
{
	unsigned long now = jiffies;
	unsigned long expires = 10 * HZ;
	struct mfc6_cache *c, *next;

	list_for_each_entry_safe(c, next, &mrt->mfc6_unres_queue, list) {
=======
static void ipmr_do_expire_process(struct mr_table *mrt)
{
	unsigned long now = jiffies;
	unsigned long expires = 10 * HZ;
	struct mr_mfc *c, *next;

	list_for_each_entry_safe(c, next, &mrt->mfc_unres_queue, list) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (time_after(c->mfc_un.unres.expires, now)) {
			/* not yet... */
			unsigned long interval = c->mfc_un.unres.expires - now;
			if (interval < expires)
				expires = interval;
			continue;
		}

		list_del(&c->list);
<<<<<<< HEAD
		ip6mr_destroy_unres(mrt, c);
	}

	if (!list_empty(&mrt->mfc6_unres_queue))
		mod_timer(&mrt->ipmr_expire_timer, jiffies + expires);
}

static void ipmr_expire_process(unsigned long arg)
{
	struct mr6_table *mrt = (struct mr6_table *)arg;
=======
		mr6_netlink_event(mrt, (struct mfc6_cache *)c, RTM_DELROUTE);
		ip6mr_destroy_unres(mrt, (struct mfc6_cache *)c);
	}

	if (!list_empty(&mrt->mfc_unres_queue))
		mod_timer(&mrt->ipmr_expire_timer, jiffies + expires);
}

static void ipmr_expire_process(struct timer_list *t)
{
	struct mr_table *mrt = from_timer(mrt, t, ipmr_expire_timer);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!spin_trylock(&mfc_unres_lock)) {
		mod_timer(&mrt->ipmr_expire_timer, jiffies + 1);
		return;
	}

<<<<<<< HEAD
	if (!list_empty(&mrt->mfc6_unres_queue))
=======
	if (!list_empty(&mrt->mfc_unres_queue))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ipmr_do_expire_process(mrt);

	spin_unlock(&mfc_unres_lock);
}

<<<<<<< HEAD
/* Fill oifs list. It is called under write locked mrt_lock. */

static void ip6mr_update_thresholds(struct mr6_table *mrt, struct mfc6_cache *cache,
=======
/* Fill oifs list. It is called under locked mrt_lock. */

static void ip6mr_update_thresholds(struct mr_table *mrt,
				    struct mr_mfc *cache,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				    unsigned char *ttls)
{
	int vifi;

	cache->mfc_un.res.minvif = MAXMIFS;
	cache->mfc_un.res.maxvif = 0;
	memset(cache->mfc_un.res.ttls, 255, MAXMIFS);

	for (vifi = 0; vifi < mrt->maxvif; vifi++) {
<<<<<<< HEAD
		if (MIF_EXISTS(mrt, vifi) &&
=======
		if (VIF_EXISTS(mrt, vifi) &&
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		    ttls[vifi] && ttls[vifi] < 255) {
			cache->mfc_un.res.ttls[vifi] = ttls[vifi];
			if (cache->mfc_un.res.minvif > vifi)
				cache->mfc_un.res.minvif = vifi;
			if (cache->mfc_un.res.maxvif <= vifi)
				cache->mfc_un.res.maxvif = vifi + 1;
		}
	}
<<<<<<< HEAD
}

static int mif6_add(struct net *net, struct mr6_table *mrt,
		    struct mif6ctl *vifc, int mrtsock)
{
	int vifi = vifc->mif6c_mifi;
	struct mif_device *v = &mrt->vif6_table[vifi];
=======
	cache->mfc_un.res.lastuse = jiffies;
}

static int mif6_add(struct net *net, struct mr_table *mrt,
		    struct mif6ctl *vifc, int mrtsock)
{
	int vifi = vifc->mif6c_mifi;
	struct vif_device *v = &mrt->vif_table[vifi];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct net_device *dev;
	struct inet6_dev *in6_dev;
	int err;

	/* Is vif busy ? */
<<<<<<< HEAD
	if (MIF_EXISTS(mrt, vifi))
=======
	if (VIF_EXISTS(mrt, vifi))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EADDRINUSE;

	switch (vifc->mif6c_flags) {
#ifdef CONFIG_IPV6_PIMSM_V2
	case MIFF_REGISTER:
		/*
		 * Special Purpose VIF in PIM
		 * All the packets will be sent to the daemon
		 */
		if (mrt->mroute_reg_vif_num >= 0)
			return -EADDRINUSE;
		dev = ip6mr_reg_vif(net, mrt);
		if (!dev)
			return -ENOBUFS;
		err = dev_set_allmulti(dev, 1);
		if (err) {
			unregister_netdevice(dev);
			dev_put(dev);
			return err;
		}
		break;
#endif
	case 0:
		dev = dev_get_by_index(net, vifc->mif6c_pifi);
		if (!dev)
			return -EADDRNOTAVAIL;
		err = dev_set_allmulti(dev, 1);
		if (err) {
			dev_put(dev);
			return err;
		}
		break;
	default:
		return -EINVAL;
	}

	in6_dev = __in6_dev_get(dev);
<<<<<<< HEAD
	if (in6_dev)
		in6_dev->cnf.mc_forwarding++;

	/*
	 *	Fill in the VIF structures
	 */
	v->rate_limit = vifc->vifc_rate_limit;
	v->flags = vifc->mif6c_flags;
	if (!mrtsock)
		v->flags |= VIFF_STATIC;
	v->threshold = vifc->vifc_threshold;
	v->bytes_in = 0;
	v->bytes_out = 0;
	v->pkt_in = 0;
	v->pkt_out = 0;
	v->link = dev->ifindex;
	if (v->flags & MIFF_REGISTER)
		v->link = dev->iflink;

	/* And finish update writing critical data */
	write_lock_bh(&mrt_lock);
	v->dev = dev;
#ifdef CONFIG_IPV6_PIMSM_V2
	if (v->flags & MIFF_REGISTER)
		mrt->mroute_reg_vif_num = vifi;
#endif
	if (vifi + 1 > mrt->maxvif)
		mrt->maxvif = vifi + 1;
	write_unlock_bh(&mrt_lock);
	return 0;
}

static struct mfc6_cache *ip6mr_cache_find(struct mr6_table *mrt,
					   const struct in6_addr *origin,
					   const struct in6_addr *mcastgrp)
{
	int line = MFC6_HASH(mcastgrp, origin);
	struct mfc6_cache *c;

	list_for_each_entry(c, &mrt->mfc6_cache_array[line], list) {
		if (ipv6_addr_equal(&c->mf6c_origin, origin) &&
		    ipv6_addr_equal(&c->mf6c_mcastgrp, mcastgrp))
			return c;
	}
	return NULL;
}

/*
 *	Allocate a multicast cache entry
 */
static struct mfc6_cache *ip6mr_cache_alloc(void)
{
	struct mfc6_cache *c = kmem_cache_zalloc(mrt_cachep, GFP_KERNEL);
	if (c == NULL)
		return NULL;
	c->mfc_un.res.minvif = MAXMIFS;
=======
	if (in6_dev) {
		atomic_inc(&in6_dev->cnf.mc_forwarding);
		inet6_netconf_notify_devconf(dev_net(dev), RTM_NEWNETCONF,
					     NETCONFA_MC_FORWARDING,
					     dev->ifindex, &in6_dev->cnf);
	}

	/* Fill in the VIF structures */
	vif_device_init(v, dev, vifc->vifc_rate_limit, vifc->vifc_threshold,
			vifc->mif6c_flags | (!mrtsock ? VIFF_STATIC : 0),
			MIFF_REGISTER);

	/* And finish update writing critical data */
	spin_lock(&mrt_lock);
	rcu_assign_pointer(v->dev, dev);
	netdev_tracker_alloc(dev, &v->dev_tracker, GFP_ATOMIC);
#ifdef CONFIG_IPV6_PIMSM_V2
	if (v->flags & MIFF_REGISTER)
		WRITE_ONCE(mrt->mroute_reg_vif_num, vifi);
#endif
	if (vifi + 1 > mrt->maxvif)
		WRITE_ONCE(mrt->maxvif, vifi + 1);
	spin_unlock(&mrt_lock);
	call_ip6mr_vif_entry_notifiers(net, FIB_EVENT_VIF_ADD,
				       v, dev, vifi, mrt->id);
	return 0;
}

static struct mfc6_cache *ip6mr_cache_find(struct mr_table *mrt,
					   const struct in6_addr *origin,
					   const struct in6_addr *mcastgrp)
{
	struct mfc6_cache_cmp_arg arg = {
		.mf6c_origin = *origin,
		.mf6c_mcastgrp = *mcastgrp,
	};

	return mr_mfc_find(mrt, &arg);
}

/* Look for a (*,G) entry */
static struct mfc6_cache *ip6mr_cache_find_any(struct mr_table *mrt,
					       struct in6_addr *mcastgrp,
					       mifi_t mifi)
{
	struct mfc6_cache_cmp_arg arg = {
		.mf6c_origin = in6addr_any,
		.mf6c_mcastgrp = *mcastgrp,
	};

	if (ipv6_addr_any(mcastgrp))
		return mr_mfc_find_any_parent(mrt, mifi);
	return mr_mfc_find_any(mrt, mifi, &arg);
}

/* Look for a (S,G,iif) entry if parent != -1 */
static struct mfc6_cache *
ip6mr_cache_find_parent(struct mr_table *mrt,
			const struct in6_addr *origin,
			const struct in6_addr *mcastgrp,
			int parent)
{
	struct mfc6_cache_cmp_arg arg = {
		.mf6c_origin = *origin,
		.mf6c_mcastgrp = *mcastgrp,
	};

	return mr_mfc_find_parent(mrt, &arg, parent);
}

/* Allocate a multicast cache entry */
static struct mfc6_cache *ip6mr_cache_alloc(void)
{
	struct mfc6_cache *c = kmem_cache_zalloc(mrt_cachep, GFP_KERNEL);
	if (!c)
		return NULL;
	c->_c.mfc_un.res.last_assert = jiffies - MFC_ASSERT_THRESH - 1;
	c->_c.mfc_un.res.minvif = MAXMIFS;
	c->_c.free = ip6mr_cache_free_rcu;
	refcount_set(&c->_c.mfc_un.res.refcount, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return c;
}

static struct mfc6_cache *ip6mr_cache_alloc_unres(void)
{
	struct mfc6_cache *c = kmem_cache_zalloc(mrt_cachep, GFP_ATOMIC);
<<<<<<< HEAD
	if (c == NULL)
		return NULL;
	skb_queue_head_init(&c->mfc_un.unres.unresolved);
	c->mfc_un.unres.expires = jiffies + 10 * HZ;
=======
	if (!c)
		return NULL;
	skb_queue_head_init(&c->_c.mfc_un.unres.unresolved);
	c->_c.mfc_un.unres.expires = jiffies + 10 * HZ;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return c;
}

/*
 *	A cache entry has gone into a resolved state from queued
 */

<<<<<<< HEAD
static void ip6mr_cache_resolve(struct net *net, struct mr6_table *mrt,
=======
static void ip6mr_cache_resolve(struct net *net, struct mr_table *mrt,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				struct mfc6_cache *uc, struct mfc6_cache *c)
{
	struct sk_buff *skb;

	/*
	 *	Play the pending entries through our router
	 */

<<<<<<< HEAD
	while((skb = __skb_dequeue(&uc->mfc_un.unres.unresolved))) {
		if (ipv6_hdr(skb)->version == 0) {
			struct nlmsghdr *nlh = (struct nlmsghdr *)skb_pull(skb, sizeof(struct ipv6hdr));

			if (__ip6mr_fill_mroute(mrt, skb, c, NLMSG_DATA(nlh)) > 0) {
				nlh->nlmsg_len = skb_tail_pointer(skb) - (u8 *)nlh;
			} else {
				nlh->nlmsg_type = NLMSG_ERROR;
				nlh->nlmsg_len = NLMSG_LENGTH(sizeof(struct nlmsgerr));
				skb_trim(skb, nlh->nlmsg_len);
				((struct nlmsgerr *)NLMSG_DATA(nlh))->error = -EMSGSIZE;
			}
			rtnl_unicast(skb, net, NETLINK_CB(skb).pid);
		} else
			ip6_mr_forward(net, mrt, skb, c);
=======
	while ((skb = __skb_dequeue(&uc->_c.mfc_un.unres.unresolved))) {
		if (ipv6_hdr(skb)->version == 0) {
			struct nlmsghdr *nlh = skb_pull(skb,
							sizeof(struct ipv6hdr));

			if (mr_fill_mroute(mrt, skb, &c->_c,
					   nlmsg_data(nlh)) > 0) {
				nlh->nlmsg_len = skb_tail_pointer(skb) - (u8 *)nlh;
			} else {
				nlh->nlmsg_type = NLMSG_ERROR;
				nlh->nlmsg_len = nlmsg_msg_size(sizeof(struct nlmsgerr));
				skb_trim(skb, nlh->nlmsg_len);
				((struct nlmsgerr *)nlmsg_data(nlh))->error = -EMSGSIZE;
			}
			rtnl_unicast(skb, net, NETLINK_CB(skb).portid);
		} else {
			rcu_read_lock();
			ip6_mr_forward(net, mrt, skb->dev, skb, c);
			rcu_read_unlock();
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

/*
<<<<<<< HEAD
 *	Bounce a cache query up to pim6sd. We could use netlink for this but pim6sd
 *	expects the following bizarre scheme.
 *
 *	Called under mrt_lock.
 */

static int ip6mr_cache_report(struct mr6_table *mrt, struct sk_buff *pkt,
			      mifi_t mifi, int assert)
{
=======
 *	Bounce a cache query up to pim6sd and netlink.
 *
 *	Called under rcu_read_lock()
 */

static int ip6mr_cache_report(const struct mr_table *mrt, struct sk_buff *pkt,
			      mifi_t mifi, int assert)
{
	struct sock *mroute6_sk;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct sk_buff *skb;
	struct mrt6msg *msg;
	int ret;

#ifdef CONFIG_IPV6_PIMSM_V2
<<<<<<< HEAD
	if (assert == MRT6MSG_WHOLEPKT)
=======
	if (assert == MRT6MSG_WHOLEPKT || assert == MRT6MSG_WRMIFWHOLE)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		skb = skb_realloc_headroom(pkt, -skb_network_offset(pkt)
						+sizeof(*msg));
	else
#endif
		skb = alloc_skb(sizeof(struct ipv6hdr) + sizeof(*msg), GFP_ATOMIC);

	if (!skb)
		return -ENOBUFS;

	/* I suppose that internal messages
	 * do not require checksums */

	skb->ip_summed = CHECKSUM_UNNECESSARY;

#ifdef CONFIG_IPV6_PIMSM_V2
<<<<<<< HEAD
	if (assert == MRT6MSG_WHOLEPKT) {
=======
	if (assert == MRT6MSG_WHOLEPKT || assert == MRT6MSG_WRMIFWHOLE) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* Ugly, but we have no choice with this interface.
		   Duplicate old header, fix length etc.
		   And all this only to mangle msg->im6_msgtype and
		   to set msg->im6_mbz to "mbz" :-)
		 */
<<<<<<< HEAD
		skb_push(skb, -skb_network_offset(pkt));
=======
		__skb_pull(skb, skb_network_offset(pkt));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		skb_push(skb, sizeof(*msg));
		skb_reset_transport_header(skb);
		msg = (struct mrt6msg *)skb_transport_header(skb);
		msg->im6_mbz = 0;
<<<<<<< HEAD
		msg->im6_msgtype = MRT6MSG_WHOLEPKT;
		msg->im6_mif = mrt->mroute_reg_vif_num;
=======
		msg->im6_msgtype = assert;
		if (assert == MRT6MSG_WRMIFWHOLE)
			msg->im6_mif = mifi;
		else
			msg->im6_mif = READ_ONCE(mrt->mroute_reg_vif_num);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		msg->im6_pad = 0;
		msg->im6_src = ipv6_hdr(pkt)->saddr;
		msg->im6_dst = ipv6_hdr(pkt)->daddr;

		skb->ip_summed = CHECKSUM_UNNECESSARY;
	} else
#endif
	{
	/*
	 *	Copy the IP header
	 */

	skb_put(skb, sizeof(struct ipv6hdr));
	skb_reset_network_header(skb);
	skb_copy_to_linear_data(skb, ipv6_hdr(pkt), sizeof(struct ipv6hdr));

	/*
	 *	Add our header
	 */
	skb_put(skb, sizeof(*msg));
	skb_reset_transport_header(skb);
	msg = (struct mrt6msg *)skb_transport_header(skb);

	msg->im6_mbz = 0;
	msg->im6_msgtype = assert;
	msg->im6_mif = mifi;
	msg->im6_pad = 0;
	msg->im6_src = ipv6_hdr(pkt)->saddr;
	msg->im6_dst = ipv6_hdr(pkt)->daddr;

	skb_dst_set(skb, dst_clone(skb_dst(pkt)));
	skb->ip_summed = CHECKSUM_UNNECESSARY;
	}

<<<<<<< HEAD
	if (mrt->mroute6_sk == NULL) {
=======
	mroute6_sk = rcu_dereference(mrt->mroute_sk);
	if (!mroute6_sk) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		kfree_skb(skb);
		return -EINVAL;
	}

<<<<<<< HEAD
	/*
	 *	Deliver to user space multicast routing algorithms
	 */
	ret = sock_queue_rcv_skb(mrt->mroute6_sk, skb);
	if (ret < 0) {
		if (net_ratelimit())
			printk(KERN_WARNING "mroute6: pending queue full, dropping entries.\n");
=======
	mrt6msg_netlink_event(mrt, skb);

	/* Deliver to user space multicast routing algorithms */
	ret = sock_queue_rcv_skb(mroute6_sk, skb);

	if (ret < 0) {
		net_warn_ratelimited("mroute6: pending queue full, dropping entries\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		kfree_skb(skb);
	}

	return ret;
}

<<<<<<< HEAD
/*
 *	Queue a packet for resolution. It gets locked cache entry!
 */

static int
ip6mr_cache_unresolved(struct mr6_table *mrt, mifi_t mifi, struct sk_buff *skb)
{
	bool found = false;
	int err;
	struct mfc6_cache *c;

	spin_lock_bh(&mfc_unres_lock);
	list_for_each_entry(c, &mrt->mfc6_unres_queue, list) {
=======
/* Queue a packet for resolution. It gets locked cache entry! */
static int ip6mr_cache_unresolved(struct mr_table *mrt, mifi_t mifi,
				  struct sk_buff *skb, struct net_device *dev)
{
	struct mfc6_cache *c;
	bool found = false;
	int err;

	spin_lock_bh(&mfc_unres_lock);
	list_for_each_entry(c, &mrt->mfc_unres_queue, _c.list) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (ipv6_addr_equal(&c->mf6c_mcastgrp, &ipv6_hdr(skb)->daddr) &&
		    ipv6_addr_equal(&c->mf6c_origin, &ipv6_hdr(skb)->saddr)) {
			found = true;
			break;
		}
	}

	if (!found) {
		/*
		 *	Create a new entry if allowable
		 */

<<<<<<< HEAD
		if (atomic_read(&mrt->cache_resolve_queue_len) >= 10 ||
		    (c = ip6mr_cache_alloc_unres()) == NULL) {
=======
		c = ip6mr_cache_alloc_unres();
		if (!c) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			spin_unlock_bh(&mfc_unres_lock);

			kfree_skb(skb);
			return -ENOBUFS;
		}

<<<<<<< HEAD
		/*
		 *	Fill in the new cache entry
		 */
		c->mf6c_parent = -1;
=======
		/* Fill in the new cache entry */
		c->_c.mfc_parent = -1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		c->mf6c_origin = ipv6_hdr(skb)->saddr;
		c->mf6c_mcastgrp = ipv6_hdr(skb)->daddr;

		/*
		 *	Reflect first query at pim6sd
		 */
		err = ip6mr_cache_report(mrt, skb, mifi, MRT6MSG_NOCACHE);
		if (err < 0) {
			/* If the report failed throw the cache entry
			   out - Brad Parker
			 */
			spin_unlock_bh(&mfc_unres_lock);

			ip6mr_cache_free(c);
			kfree_skb(skb);
			return err;
		}

		atomic_inc(&mrt->cache_resolve_queue_len);
<<<<<<< HEAD
		list_add(&c->list, &mrt->mfc6_unres_queue);
=======
		list_add(&c->_c.list, &mrt->mfc_unres_queue);
		mr6_netlink_event(mrt, c, RTM_NEWROUTE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		ipmr_do_expire_process(mrt);
	}

<<<<<<< HEAD
	/*
	 *	See if we can append the packet
	 */
	if (c->mfc_un.unres.unresolved.qlen > 3) {
		kfree_skb(skb);
		err = -ENOBUFS;
	} else {
		skb_queue_tail(&c->mfc_un.unres.unresolved, skb);
=======
	/* See if we can append the packet */
	if (c->_c.mfc_un.unres.unresolved.qlen > 3) {
		kfree_skb(skb);
		err = -ENOBUFS;
	} else {
		if (dev) {
			skb->dev = dev;
			skb->skb_iif = dev->ifindex;
		}
		skb_queue_tail(&c->_c.mfc_un.unres.unresolved, skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = 0;
	}

	spin_unlock_bh(&mfc_unres_lock);
	return err;
}

/*
 *	MFC6 cache manipulation by user space
 */

<<<<<<< HEAD
static int ip6mr_mfc_delete(struct mr6_table *mrt, struct mf6cctl *mfc)
{
	int line;
	struct mfc6_cache *c, *next;

	line = MFC6_HASH(&mfc->mf6cc_mcastgrp.sin6_addr, &mfc->mf6cc_origin.sin6_addr);

	list_for_each_entry_safe(c, next, &mrt->mfc6_cache_array[line], list) {
		if (ipv6_addr_equal(&c->mf6c_origin, &mfc->mf6cc_origin.sin6_addr) &&
		    ipv6_addr_equal(&c->mf6c_mcastgrp, &mfc->mf6cc_mcastgrp.sin6_addr)) {
			write_lock_bh(&mrt_lock);
			list_del(&c->list);
			write_unlock_bh(&mrt_lock);

			ip6mr_cache_free(c);
			return 0;
		}
	}
	return -ENOENT;
=======
static int ip6mr_mfc_delete(struct mr_table *mrt, struct mf6cctl *mfc,
			    int parent)
{
	struct mfc6_cache *c;

	/* The entries are added/deleted only under RTNL */
	rcu_read_lock();
	c = ip6mr_cache_find_parent(mrt, &mfc->mf6cc_origin.sin6_addr,
				    &mfc->mf6cc_mcastgrp.sin6_addr, parent);
	rcu_read_unlock();
	if (!c)
		return -ENOENT;
	rhltable_remove(&mrt->mfc_hash, &c->_c.mnode, ip6mr_rht_params);
	list_del_rcu(&c->_c.list);

	call_ip6mr_mfc_entry_notifiers(read_pnet(&mrt->net),
				       FIB_EVENT_ENTRY_DEL, c, mrt->id);
	mr6_netlink_event(mrt, c, RTM_DELROUTE);
	mr_cache_put(&c->_c);
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int ip6mr_device_event(struct notifier_block *this,
			      unsigned long event, void *ptr)
{
<<<<<<< HEAD
	struct net_device *dev = ptr;
	struct net *net = dev_net(dev);
	struct mr6_table *mrt;
	struct mif_device *v;
	int ct;
	LIST_HEAD(list);
=======
	struct net_device *dev = netdev_notifier_info_to_dev(ptr);
	struct net *net = dev_net(dev);
	struct mr_table *mrt;
	struct vif_device *v;
	int ct;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (event != NETDEV_UNREGISTER)
		return NOTIFY_DONE;

	ip6mr_for_each_table(mrt, net) {
<<<<<<< HEAD
		v = &mrt->vif6_table[0];
		for (ct = 0; ct < mrt->maxvif; ct++, v++) {
			if (v->dev == dev)
				mif6_delete(mrt, ct, &list);
		}
	}
	unregister_netdevice_many(&list);
=======
		v = &mrt->vif_table[0];
		for (ct = 0; ct < mrt->maxvif; ct++, v++) {
			if (rcu_access_pointer(v->dev) == dev)
				mif6_delete(mrt, ct, 1, NULL);
		}
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return NOTIFY_DONE;
}

<<<<<<< HEAD
=======
static unsigned int ip6mr_seq_read(struct net *net)
{
	ASSERT_RTNL();

	return net->ipv6.ipmr_seq + ip6mr_rules_seq_read(net);
}

static int ip6mr_dump(struct net *net, struct notifier_block *nb,
		      struct netlink_ext_ack *extack)
{
	return mr_dump(net, nb, RTNL_FAMILY_IP6MR, ip6mr_rules_dump,
		       ip6mr_mr_table_iter, extack);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct notifier_block ip6_mr_notifier = {
	.notifier_call = ip6mr_device_event
};

<<<<<<< HEAD
/*
 *	Setup for IP multicast routing
 */

=======
static const struct fib_notifier_ops ip6mr_notifier_ops_template = {
	.family		= RTNL_FAMILY_IP6MR,
	.fib_seq_read	= ip6mr_seq_read,
	.fib_dump	= ip6mr_dump,
	.owner		= THIS_MODULE,
};

static int __net_init ip6mr_notifier_init(struct net *net)
{
	struct fib_notifier_ops *ops;

	net->ipv6.ipmr_seq = 0;

	ops = fib_notifier_ops_register(&ip6mr_notifier_ops_template, net);
	if (IS_ERR(ops))
		return PTR_ERR(ops);

	net->ipv6.ip6mr_notifier_ops = ops;

	return 0;
}

static void __net_exit ip6mr_notifier_exit(struct net *net)
{
	fib_notifier_ops_unregister(net->ipv6.ip6mr_notifier_ops);
	net->ipv6.ip6mr_notifier_ops = NULL;
}

/* Setup for IP multicast routing */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int __net_init ip6mr_net_init(struct net *net)
{
	int err;

<<<<<<< HEAD
	err = ip6mr_rules_init(net);
	if (err < 0)
		goto fail;

#ifdef CONFIG_PROC_FS
	err = -ENOMEM;
	if (!proc_net_fops_create(net, "ip6_mr_vif", 0, &ip6mr_vif_fops))
		goto proc_vif_fail;
	if (!proc_net_fops_create(net, "ip6_mr_cache", 0, &ip6mr_mfc_fops))
=======
	err = ip6mr_notifier_init(net);
	if (err)
		return err;

	err = ip6mr_rules_init(net);
	if (err < 0)
		goto ip6mr_rules_fail;

#ifdef CONFIG_PROC_FS
	err = -ENOMEM;
	if (!proc_create_net("ip6_mr_vif", 0, net->proc_net, &ip6mr_vif_seq_ops,
			sizeof(struct mr_vif_iter)))
		goto proc_vif_fail;
	if (!proc_create_net("ip6_mr_cache", 0, net->proc_net, &ipmr_mfc_seq_ops,
			sizeof(struct mr_mfc_iter)))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto proc_cache_fail;
#endif

	return 0;

#ifdef CONFIG_PROC_FS
proc_cache_fail:
<<<<<<< HEAD
	proc_net_remove(net, "ip6_mr_vif");
proc_vif_fail:
	ip6mr_rules_exit(net);
#endif
fail:
=======
	remove_proc_entry("ip6_mr_vif", net->proc_net);
proc_vif_fail:
	rtnl_lock();
	ip6mr_rules_exit(net);
	rtnl_unlock();
#endif
ip6mr_rules_fail:
	ip6mr_notifier_exit(net);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

static void __net_exit ip6mr_net_exit(struct net *net)
{
#ifdef CONFIG_PROC_FS
<<<<<<< HEAD
	proc_net_remove(net, "ip6_mr_cache");
	proc_net_remove(net, "ip6_mr_vif");
#endif
	ip6mr_rules_exit(net);
=======
	remove_proc_entry("ip6_mr_cache", net->proc_net);
	remove_proc_entry("ip6_mr_vif", net->proc_net);
#endif
	ip6mr_notifier_exit(net);
}

static void __net_exit ip6mr_net_exit_batch(struct list_head *net_list)
{
	struct net *net;

	rtnl_lock();
	list_for_each_entry(net, net_list, exit_list)
		ip6mr_rules_exit(net);
	rtnl_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct pernet_operations ip6mr_net_ops = {
	.init = ip6mr_net_init,
	.exit = ip6mr_net_exit,
<<<<<<< HEAD
=======
	.exit_batch = ip6mr_net_exit_batch,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

int __init ip6_mr_init(void)
{
	int err;

<<<<<<< HEAD
	mrt_cachep = kmem_cache_create("ip6_mrt_cache",
				       sizeof(struct mfc6_cache),
				       0, SLAB_HWCACHE_ALIGN,
				       NULL);
=======
	mrt_cachep = KMEM_CACHE(mfc6_cache, SLAB_HWCACHE_ALIGN);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!mrt_cachep)
		return -ENOMEM;

	err = register_pernet_subsys(&ip6mr_net_ops);
	if (err)
		goto reg_pernet_fail;

	err = register_netdevice_notifier(&ip6_mr_notifier);
	if (err)
		goto reg_notif_fail;
#ifdef CONFIG_IPV6_PIMSM_V2
	if (inet6_add_protocol(&pim6_protocol, IPPROTO_PIM) < 0) {
<<<<<<< HEAD
		printk(KERN_ERR "ip6_mr_init: can't add PIM protocol\n");
=======
		pr_err("%s: can't add PIM protocol\n", __func__);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = -EAGAIN;
		goto add_proto_fail;
	}
#endif
<<<<<<< HEAD
	rtnl_register(RTNL_FAMILY_IP6MR, RTM_GETROUTE, NULL,
		      ip6mr_rtm_dumproute, NULL);
	return 0;
#ifdef CONFIG_IPV6_PIMSM_V2
=======
	err = rtnl_register_module(THIS_MODULE, RTNL_FAMILY_IP6MR, RTM_GETROUTE,
				   ip6mr_rtm_getroute, ip6mr_rtm_dumproute, 0);
	if (err == 0)
		return 0;

#ifdef CONFIG_IPV6_PIMSM_V2
	inet6_del_protocol(&pim6_protocol, IPPROTO_PIM);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
add_proto_fail:
	unregister_netdevice_notifier(&ip6_mr_notifier);
#endif
reg_notif_fail:
	unregister_pernet_subsys(&ip6mr_net_ops);
reg_pernet_fail:
	kmem_cache_destroy(mrt_cachep);
	return err;
}

void ip6_mr_cleanup(void)
{
<<<<<<< HEAD
=======
	rtnl_unregister(RTNL_FAMILY_IP6MR, RTM_GETROUTE);
#ifdef CONFIG_IPV6_PIMSM_V2
	inet6_del_protocol(&pim6_protocol, IPPROTO_PIM);
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unregister_netdevice_notifier(&ip6_mr_notifier);
	unregister_pernet_subsys(&ip6mr_net_ops);
	kmem_cache_destroy(mrt_cachep);
}

<<<<<<< HEAD
static int ip6mr_mfc_add(struct net *net, struct mr6_table *mrt,
			 struct mf6cctl *mfc, int mrtsock)
{
	bool found = false;
	int line;
	struct mfc6_cache *uc, *c;
	unsigned char ttls[MAXMIFS];
	int i;
=======
static int ip6mr_mfc_add(struct net *net, struct mr_table *mrt,
			 struct mf6cctl *mfc, int mrtsock, int parent)
{
	unsigned char ttls[MAXMIFS];
	struct mfc6_cache *uc, *c;
	struct mr_mfc *_uc;
	bool found;
	int i, err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (mfc->mf6cc_parent >= MAXMIFS)
		return -ENFILE;

	memset(ttls, 255, MAXMIFS);
	for (i = 0; i < MAXMIFS; i++) {
		if (IF_ISSET(i, &mfc->mf6cc_ifset))
			ttls[i] = 1;
<<<<<<< HEAD

	}

	line = MFC6_HASH(&mfc->mf6cc_mcastgrp.sin6_addr, &mfc->mf6cc_origin.sin6_addr);

	list_for_each_entry(c, &mrt->mfc6_cache_array[line], list) {
		if (ipv6_addr_equal(&c->mf6c_origin, &mfc->mf6cc_origin.sin6_addr) &&
		    ipv6_addr_equal(&c->mf6c_mcastgrp, &mfc->mf6cc_mcastgrp.sin6_addr)) {
			found = true;
			break;
		}
	}

	if (found) {
		write_lock_bh(&mrt_lock);
		c->mf6c_parent = mfc->mf6cc_parent;
		ip6mr_update_thresholds(mrt, c, ttls);
		if (!mrtsock)
			c->mfc_flags |= MFC_STATIC;
		write_unlock_bh(&mrt_lock);
		return 0;
	}

	if (!ipv6_addr_is_multicast(&mfc->mf6cc_mcastgrp.sin6_addr))
		return -EINVAL;

	c = ip6mr_cache_alloc();
	if (c == NULL)
=======
	}

	/* The entries are added/deleted only under RTNL */
	rcu_read_lock();
	c = ip6mr_cache_find_parent(mrt, &mfc->mf6cc_origin.sin6_addr,
				    &mfc->mf6cc_mcastgrp.sin6_addr, parent);
	rcu_read_unlock();
	if (c) {
		spin_lock(&mrt_lock);
		c->_c.mfc_parent = mfc->mf6cc_parent;
		ip6mr_update_thresholds(mrt, &c->_c, ttls);
		if (!mrtsock)
			c->_c.mfc_flags |= MFC_STATIC;
		spin_unlock(&mrt_lock);
		call_ip6mr_mfc_entry_notifiers(net, FIB_EVENT_ENTRY_REPLACE,
					       c, mrt->id);
		mr6_netlink_event(mrt, c, RTM_NEWROUTE);
		return 0;
	}

	if (!ipv6_addr_any(&mfc->mf6cc_mcastgrp.sin6_addr) &&
	    !ipv6_addr_is_multicast(&mfc->mf6cc_mcastgrp.sin6_addr))
		return -EINVAL;

	c = ip6mr_cache_alloc();
	if (!c)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ENOMEM;

	c->mf6c_origin = mfc->mf6cc_origin.sin6_addr;
	c->mf6c_mcastgrp = mfc->mf6cc_mcastgrp.sin6_addr;
<<<<<<< HEAD
	c->mf6c_parent = mfc->mf6cc_parent;
	ip6mr_update_thresholds(mrt, c, ttls);
	if (!mrtsock)
		c->mfc_flags |= MFC_STATIC;

	write_lock_bh(&mrt_lock);
	list_add(&c->list, &mrt->mfc6_cache_array[line]);
	write_unlock_bh(&mrt_lock);

	/*
	 *	Check to see if we resolved a queued list. If so we
	 *	need to send on the frames and tidy up.
	 */
	found = false;
	spin_lock_bh(&mfc_unres_lock);
	list_for_each_entry(uc, &mrt->mfc6_unres_queue, list) {
		if (ipv6_addr_equal(&uc->mf6c_origin, &c->mf6c_origin) &&
		    ipv6_addr_equal(&uc->mf6c_mcastgrp, &c->mf6c_mcastgrp)) {
			list_del(&uc->list);
=======
	c->_c.mfc_parent = mfc->mf6cc_parent;
	ip6mr_update_thresholds(mrt, &c->_c, ttls);
	if (!mrtsock)
		c->_c.mfc_flags |= MFC_STATIC;

	err = rhltable_insert_key(&mrt->mfc_hash, &c->cmparg, &c->_c.mnode,
				  ip6mr_rht_params);
	if (err) {
		pr_err("ip6mr: rhtable insert error %d\n", err);
		ip6mr_cache_free(c);
		return err;
	}
	list_add_tail_rcu(&c->_c.list, &mrt->mfc_cache_list);

	/* Check to see if we resolved a queued list. If so we
	 * need to send on the frames and tidy up.
	 */
	found = false;
	spin_lock_bh(&mfc_unres_lock);
	list_for_each_entry(_uc, &mrt->mfc_unres_queue, list) {
		uc = (struct mfc6_cache *)_uc;
		if (ipv6_addr_equal(&uc->mf6c_origin, &c->mf6c_origin) &&
		    ipv6_addr_equal(&uc->mf6c_mcastgrp, &c->mf6c_mcastgrp)) {
			list_del(&_uc->list);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			atomic_dec(&mrt->cache_resolve_queue_len);
			found = true;
			break;
		}
	}
<<<<<<< HEAD
	if (list_empty(&mrt->mfc6_unres_queue))
=======
	if (list_empty(&mrt->mfc_unres_queue))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		del_timer(&mrt->ipmr_expire_timer);
	spin_unlock_bh(&mfc_unres_lock);

	if (found) {
		ip6mr_cache_resolve(net, mrt, uc, c);
		ip6mr_cache_free(uc);
	}
<<<<<<< HEAD
=======
	call_ip6mr_mfc_entry_notifiers(net, FIB_EVENT_ENTRY_ADD,
				       c, mrt->id);
	mr6_netlink_event(mrt, c, RTM_NEWROUTE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

/*
 *	Close the multicast socket, and clear the vif tables etc
 */

<<<<<<< HEAD
static void mroute_clean_tables(struct mr6_table *mrt, bool all)
{
	int i;
	LIST_HEAD(list);
	struct mfc6_cache *c, *next;

	/*
	 *	Shut down all active vif entries
	 */
	for (i = 0; i < mrt->maxvif; i++) {
		if (!all && (mrt->vif6_table[i].flags & VIFF_STATIC))
			continue;
		mif6_delete(mrt, i, &list);
	}
	unregister_netdevice_many(&list);

	/*
	 *	Wipe the cache
	 */
	for (i = 0; i < MFC6_LINES; i++) {
		list_for_each_entry_safe(c, next, &mrt->mfc6_cache_array[i], list) {
			if (!all && (c->mfc_flags & MFC_STATIC))
				continue;
			write_lock_bh(&mrt_lock);
			list_del(&c->list);
			write_unlock_bh(&mrt_lock);

			ip6mr_cache_free(c);
		}
	}

	if (atomic_read(&mrt->cache_resolve_queue_len) != 0) {
		spin_lock_bh(&mfc_unres_lock);
		list_for_each_entry_safe(c, next, &mrt->mfc6_unres_queue, list) {
			list_del(&c->list);
			ip6mr_destroy_unres(mrt, c);
		}
		spin_unlock_bh(&mfc_unres_lock);
	}
}

static int ip6mr_sk_init(struct mr6_table *mrt, struct sock *sk)
=======
static void mroute_clean_tables(struct mr_table *mrt, int flags)
{
	struct mr_mfc *c, *tmp;
	LIST_HEAD(list);
	int i;

	/* Shut down all active vif entries */
	if (flags & (MRT6_FLUSH_MIFS | MRT6_FLUSH_MIFS_STATIC)) {
		for (i = 0; i < mrt->maxvif; i++) {
			if (((mrt->vif_table[i].flags & VIFF_STATIC) &&
			     !(flags & MRT6_FLUSH_MIFS_STATIC)) ||
			    (!(mrt->vif_table[i].flags & VIFF_STATIC) && !(flags & MRT6_FLUSH_MIFS)))
				continue;
			mif6_delete(mrt, i, 0, &list);
		}
		unregister_netdevice_many(&list);
	}

	/* Wipe the cache */
	if (flags & (MRT6_FLUSH_MFC | MRT6_FLUSH_MFC_STATIC)) {
		list_for_each_entry_safe(c, tmp, &mrt->mfc_cache_list, list) {
			if (((c->mfc_flags & MFC_STATIC) && !(flags & MRT6_FLUSH_MFC_STATIC)) ||
			    (!(c->mfc_flags & MFC_STATIC) && !(flags & MRT6_FLUSH_MFC)))
				continue;
			rhltable_remove(&mrt->mfc_hash, &c->mnode, ip6mr_rht_params);
			list_del_rcu(&c->list);
			call_ip6mr_mfc_entry_notifiers(read_pnet(&mrt->net),
						       FIB_EVENT_ENTRY_DEL,
						       (struct mfc6_cache *)c, mrt->id);
			mr6_netlink_event(mrt, (struct mfc6_cache *)c, RTM_DELROUTE);
			mr_cache_put(c);
		}
	}

	if (flags & MRT6_FLUSH_MFC) {
		if (atomic_read(&mrt->cache_resolve_queue_len) != 0) {
			spin_lock_bh(&mfc_unres_lock);
			list_for_each_entry_safe(c, tmp, &mrt->mfc_unres_queue, list) {
				list_del(&c->list);
				mr6_netlink_event(mrt, (struct mfc6_cache *)c,
						  RTM_DELROUTE);
				ip6mr_destroy_unres(mrt, (struct mfc6_cache *)c);
			}
			spin_unlock_bh(&mfc_unres_lock);
		}
	}
}

static int ip6mr_sk_init(struct mr_table *mrt, struct sock *sk)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int err = 0;
	struct net *net = sock_net(sk);

	rtnl_lock();
<<<<<<< HEAD
	write_lock_bh(&mrt_lock);
	if (likely(mrt->mroute6_sk == NULL)) {
		mrt->mroute6_sk = sk;
		net->ipv6.devconf_all->mc_forwarding++;
	}
	else
		err = -EADDRINUSE;
	write_unlock_bh(&mrt_lock);

=======
	spin_lock(&mrt_lock);
	if (rtnl_dereference(mrt->mroute_sk)) {
		err = -EADDRINUSE;
	} else {
		rcu_assign_pointer(mrt->mroute_sk, sk);
		sock_set_flag(sk, SOCK_RCU_FREE);
		atomic_inc(&net->ipv6.devconf_all->mc_forwarding);
	}
	spin_unlock(&mrt_lock);

	if (!err)
		inet6_netconf_notify_devconf(net, RTM_NEWNETCONF,
					     NETCONFA_MC_FORWARDING,
					     NETCONFA_IFINDEX_ALL,
					     net->ipv6.devconf_all);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rtnl_unlock();

	return err;
}

int ip6mr_sk_done(struct sock *sk)
{
<<<<<<< HEAD
	int err = -EACCES;
	struct net *net = sock_net(sk);
	struct mr6_table *mrt;

	rtnl_lock();
	ip6mr_for_each_table(mrt, net) {
		if (sk == mrt->mroute6_sk) {
			write_lock_bh(&mrt_lock);
			mrt->mroute6_sk = NULL;
			net->ipv6.devconf_all->mc_forwarding--;
			write_unlock_bh(&mrt_lock);

			mroute_clean_tables(mrt, false);
=======
	struct net *net = sock_net(sk);
	struct ipv6_devconf *devconf;
	struct mr_table *mrt;
	int err = -EACCES;

	if (sk->sk_type != SOCK_RAW ||
	    inet_sk(sk)->inet_num != IPPROTO_ICMPV6)
		return err;

	devconf = net->ipv6.devconf_all;
	if (!devconf || !atomic_read(&devconf->mc_forwarding))
		return err;

	rtnl_lock();
	ip6mr_for_each_table(mrt, net) {
		if (sk == rtnl_dereference(mrt->mroute_sk)) {
			spin_lock(&mrt_lock);
			RCU_INIT_POINTER(mrt->mroute_sk, NULL);
			/* Note that mroute_sk had SOCK_RCU_FREE set,
			 * so the RCU grace period before sk freeing
			 * is guaranteed by sk_destruct()
			 */
			atomic_dec(&devconf->mc_forwarding);
			spin_unlock(&mrt_lock);
			inet6_netconf_notify_devconf(net, RTM_NEWNETCONF,
						     NETCONFA_MC_FORWARDING,
						     NETCONFA_IFINDEX_ALL,
						     net->ipv6.devconf_all);

			mroute_clean_tables(mrt, MRT6_FLUSH_MIFS | MRT6_FLUSH_MFC);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			err = 0;
			break;
		}
	}
	rtnl_unlock();

	return err;
}

<<<<<<< HEAD
struct sock *mroute6_socket(struct net *net, struct sk_buff *skb)
{
	struct mr6_table *mrt;
	struct flowi6 fl6 = {
		.flowi6_iif	= skb->skb_iif,
=======
bool mroute6_is_socket(struct net *net, struct sk_buff *skb)
{
	struct mr_table *mrt;
	struct flowi6 fl6 = {
		.flowi6_iif	= skb->skb_iif ? : LOOPBACK_IFINDEX,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.flowi6_oif	= skb->dev->ifindex,
		.flowi6_mark	= skb->mark,
	};

	if (ip6mr_fib_lookup(net, &fl6, &mrt) < 0)
		return NULL;

<<<<<<< HEAD
	return mrt->mroute6_sk;
}
=======
	return rcu_access_pointer(mrt->mroute_sk);
}
EXPORT_SYMBOL(mroute6_is_socket);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 *	Socket options and virtual interface manipulation. The whole
 *	virtual interface system is a complete heap, but unfortunately
 *	that's how BSD mrouted happens to think. Maybe one day with a proper
 *	MOSPF/PIM router set up we can clean this up.
 */

<<<<<<< HEAD
int ip6_mroute_setsockopt(struct sock *sk, int optname, char __user *optval, unsigned int optlen)
{
	int ret;
=======
int ip6_mroute_setsockopt(struct sock *sk, int optname, sockptr_t optval,
			  unsigned int optlen)
{
	int ret, parent = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct mif6ctl vif;
	struct mf6cctl mfc;
	mifi_t mifi;
	struct net *net = sock_net(sk);
<<<<<<< HEAD
	struct mr6_table *mrt;

	mrt = ip6mr_get_table(net, raw6_sk(sk)->ip6mr_table ? : RT6_TABLE_DFLT);
	if (mrt == NULL)
		return -ENOENT;

	if (optname != MRT6_INIT) {
		if (sk != mrt->mroute6_sk && !capable(CAP_NET_ADMIN))
=======
	struct mr_table *mrt;

	if (sk->sk_type != SOCK_RAW ||
	    inet_sk(sk)->inet_num != IPPROTO_ICMPV6)
		return -EOPNOTSUPP;

	mrt = ip6mr_get_table(net, raw6_sk(sk)->ip6mr_table ? : RT6_TABLE_DFLT);
	if (!mrt)
		return -ENOENT;

	if (optname != MRT6_INIT) {
		if (sk != rcu_access_pointer(mrt->mroute_sk) &&
		    !ns_capable(net->user_ns, CAP_NET_ADMIN))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -EACCES;
	}

	switch (optname) {
	case MRT6_INIT:
<<<<<<< HEAD
		if (sk->sk_type != SOCK_RAW ||
		    inet_sk(sk)->inet_num != IPPROTO_ICMPV6)
			return -EOPNOTSUPP;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (optlen < sizeof(int))
			return -EINVAL;

		return ip6mr_sk_init(mrt, sk);

	case MRT6_DONE:
		return ip6mr_sk_done(sk);

	case MRT6_ADD_MIF:
		if (optlen < sizeof(vif))
			return -EINVAL;
<<<<<<< HEAD
		if (copy_from_user(&vif, optval, sizeof(vif)))
=======
		if (copy_from_sockptr(&vif, optval, sizeof(vif)))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -EFAULT;
		if (vif.mif6c_mifi >= MAXMIFS)
			return -ENFILE;
		rtnl_lock();
<<<<<<< HEAD
		ret = mif6_add(net, mrt, &vif, sk == mrt->mroute6_sk);
=======
		ret = mif6_add(net, mrt, &vif,
			       sk == rtnl_dereference(mrt->mroute_sk));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		rtnl_unlock();
		return ret;

	case MRT6_DEL_MIF:
		if (optlen < sizeof(mifi_t))
			return -EINVAL;
<<<<<<< HEAD
		if (copy_from_user(&mifi, optval, sizeof(mifi_t)))
			return -EFAULT;
		rtnl_lock();
		ret = mif6_delete(mrt, mifi, NULL);
=======
		if (copy_from_sockptr(&mifi, optval, sizeof(mifi_t)))
			return -EFAULT;
		rtnl_lock();
		ret = mif6_delete(mrt, mifi, 0, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		rtnl_unlock();
		return ret;

	/*
	 *	Manipulate the forwarding caches. These live
	 *	in a sort of kernel/user symbiosis.
	 */
	case MRT6_ADD_MFC:
	case MRT6_DEL_MFC:
<<<<<<< HEAD
		if (optlen < sizeof(mfc))
			return -EINVAL;
		if (copy_from_user(&mfc, optval, sizeof(mfc)))
			return -EFAULT;
		rtnl_lock();
		if (optname == MRT6_DEL_MFC)
			ret = ip6mr_mfc_delete(mrt, &mfc);
		else
			ret = ip6mr_mfc_add(net, mrt, &mfc, sk == mrt->mroute6_sk);
		rtnl_unlock();
		return ret;

=======
		parent = -1;
		fallthrough;
	case MRT6_ADD_MFC_PROXY:
	case MRT6_DEL_MFC_PROXY:
		if (optlen < sizeof(mfc))
			return -EINVAL;
		if (copy_from_sockptr(&mfc, optval, sizeof(mfc)))
			return -EFAULT;
		if (parent == 0)
			parent = mfc.mf6cc_parent;
		rtnl_lock();
		if (optname == MRT6_DEL_MFC || optname == MRT6_DEL_MFC_PROXY)
			ret = ip6mr_mfc_delete(mrt, &mfc, parent);
		else
			ret = ip6mr_mfc_add(net, mrt, &mfc,
					    sk ==
					    rtnl_dereference(mrt->mroute_sk),
					    parent);
		rtnl_unlock();
		return ret;

	case MRT6_FLUSH:
	{
		int flags;

		if (optlen != sizeof(flags))
			return -EINVAL;
		if (copy_from_sockptr(&flags, optval, sizeof(flags)))
			return -EFAULT;
		rtnl_lock();
		mroute_clean_tables(mrt, flags);
		rtnl_unlock();
		return 0;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 *	Control PIM assert (to activate pim will activate assert)
	 */
	case MRT6_ASSERT:
	{
		int v;
<<<<<<< HEAD
		if (get_user(v, (int __user *)optval))
			return -EFAULT;
		mrt->mroute_do_assert = !!v;
=======

		if (optlen != sizeof(v))
			return -EINVAL;
		if (copy_from_sockptr(&v, optval, sizeof(v)))
			return -EFAULT;
		mrt->mroute_do_assert = v;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;
	}

#ifdef CONFIG_IPV6_PIMSM_V2
	case MRT6_PIM:
	{
<<<<<<< HEAD
		int v;
		if (get_user(v, (int __user *)optval))
			return -EFAULT;
=======
		bool do_wrmifwhole;
		int v;

		if (optlen != sizeof(v))
			return -EINVAL;
		if (copy_from_sockptr(&v, optval, sizeof(v)))
			return -EFAULT;

		do_wrmifwhole = (v == MRT6MSG_WRMIFWHOLE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		v = !!v;
		rtnl_lock();
		ret = 0;
		if (v != mrt->mroute_do_pim) {
			mrt->mroute_do_pim = v;
			mrt->mroute_do_assert = v;
<<<<<<< HEAD
=======
			mrt->mroute_do_wrvifwhole = do_wrmifwhole;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		rtnl_unlock();
		return ret;
	}

#endif
#ifdef CONFIG_IPV6_MROUTE_MULTIPLE_TABLES
	case MRT6_TABLE:
	{
		u32 v;

		if (optlen != sizeof(u32))
			return -EINVAL;
<<<<<<< HEAD
		if (get_user(v, (u32 __user *)optval))
			return -EFAULT;
		if (sk == mrt->mroute6_sk)
=======
		if (copy_from_sockptr(&v, optval, sizeof(v)))
			return -EFAULT;
		/* "pim6reg%u" should not exceed 16 bytes (IFNAMSIZ) */
		if (v != RT_TABLE_DEFAULT && v >= 100000000)
			return -EINVAL;
		if (sk == rcu_access_pointer(mrt->mroute_sk))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -EBUSY;

		rtnl_lock();
		ret = 0;
<<<<<<< HEAD
		if (!ip6mr_new_table(net, v))
			ret = -ENOMEM;
		raw6_sk(sk)->ip6mr_table = v;
=======
		mrt = ip6mr_new_table(net, v);
		if (IS_ERR(mrt))
			ret = PTR_ERR(mrt);
		else
			raw6_sk(sk)->ip6mr_table = v;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		rtnl_unlock();
		return ret;
	}
#endif
	/*
	 *	Spurious command, or MRT6_VERSION which you cannot
	 *	set.
	 */
	default:
		return -ENOPROTOOPT;
	}
}

/*
 *	Getsock opt support for the multicast routing system.
 */

<<<<<<< HEAD
int ip6_mroute_getsockopt(struct sock *sk, int optname, char __user *optval,
			  int __user *optlen)
=======
int ip6_mroute_getsockopt(struct sock *sk, int optname, sockptr_t optval,
			  sockptr_t optlen)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int olr;
	int val;
	struct net *net = sock_net(sk);
<<<<<<< HEAD
	struct mr6_table *mrt;

	mrt = ip6mr_get_table(net, raw6_sk(sk)->ip6mr_table ? : RT6_TABLE_DFLT);
	if (mrt == NULL)
=======
	struct mr_table *mrt;

	if (sk->sk_type != SOCK_RAW ||
	    inet_sk(sk)->inet_num != IPPROTO_ICMPV6)
		return -EOPNOTSUPP;

	mrt = ip6mr_get_table(net, raw6_sk(sk)->ip6mr_table ? : RT6_TABLE_DFLT);
	if (!mrt)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ENOENT;

	switch (optname) {
	case MRT6_VERSION:
		val = 0x0305;
		break;
#ifdef CONFIG_IPV6_PIMSM_V2
	case MRT6_PIM:
		val = mrt->mroute_do_pim;
		break;
#endif
	case MRT6_ASSERT:
		val = mrt->mroute_do_assert;
		break;
	default:
		return -ENOPROTOOPT;
	}

<<<<<<< HEAD
	if (get_user(olr, optlen))
=======
	if (copy_from_sockptr(&olr, optlen, sizeof(int)))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EFAULT;

	olr = min_t(int, olr, sizeof(int));
	if (olr < 0)
		return -EINVAL;

<<<<<<< HEAD
	if (put_user(olr, optlen))
		return -EFAULT;
	if (copy_to_user(optval, &val, olr))
=======
	if (copy_to_sockptr(optlen, &olr, sizeof(int)))
		return -EFAULT;
	if (copy_to_sockptr(optval, &val, olr))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EFAULT;
	return 0;
}

/*
 *	The IP multicast ioctl support routines.
 */
<<<<<<< HEAD

int ip6mr_ioctl(struct sock *sk, int cmd, void __user *arg)
{
	struct sioc_sg_req6 sr;
	struct sioc_mif_req6 vr;
	struct mif_device *vif;
	struct mfc6_cache *c;
	struct net *net = sock_net(sk);
	struct mr6_table *mrt;

	mrt = ip6mr_get_table(net, raw6_sk(sk)->ip6mr_table ? : RT6_TABLE_DFLT);
	if (mrt == NULL)
=======
int ip6mr_ioctl(struct sock *sk, int cmd, void *arg)
{
	struct sioc_sg_req6 *sr;
	struct sioc_mif_req6 *vr;
	struct vif_device *vif;
	struct mfc6_cache *c;
	struct net *net = sock_net(sk);
	struct mr_table *mrt;

	mrt = ip6mr_get_table(net, raw6_sk(sk)->ip6mr_table ? : RT6_TABLE_DFLT);
	if (!mrt)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ENOENT;

	switch (cmd) {
	case SIOCGETMIFCNT_IN6:
<<<<<<< HEAD
		if (copy_from_user(&vr, arg, sizeof(vr)))
			return -EFAULT;
		if (vr.mifi >= mrt->maxvif)
			return -EINVAL;
		read_lock(&mrt_lock);
		vif = &mrt->vif6_table[vr.mifi];
		if (MIF_EXISTS(mrt, vr.mifi)) {
			vr.icount = vif->pkt_in;
			vr.ocount = vif->pkt_out;
			vr.ibytes = vif->bytes_in;
			vr.obytes = vif->bytes_out;
			read_unlock(&mrt_lock);

			if (copy_to_user(arg, &vr, sizeof(vr)))
				return -EFAULT;
			return 0;
		}
		read_unlock(&mrt_lock);
		return -EADDRNOTAVAIL;
	case SIOCGETSGCNT_IN6:
		if (copy_from_user(&sr, arg, sizeof(sr)))
			return -EFAULT;

		read_lock(&mrt_lock);
		c = ip6mr_cache_find(mrt, &sr.src.sin6_addr, &sr.grp.sin6_addr);
		if (c) {
			sr.pktcnt = c->mfc_un.res.pkt;
			sr.bytecnt = c->mfc_un.res.bytes;
			sr.wrong_if = c->mfc_un.res.wrong_if;
			read_unlock(&mrt_lock);

			if (copy_to_user(arg, &sr, sizeof(sr)))
				return -EFAULT;
			return 0;
		}
		read_unlock(&mrt_lock);
=======
		vr = (struct sioc_mif_req6 *)arg;
		if (vr->mifi >= mrt->maxvif)
			return -EINVAL;
		vr->mifi = array_index_nospec(vr->mifi, mrt->maxvif);
		rcu_read_lock();
		vif = &mrt->vif_table[vr->mifi];
		if (VIF_EXISTS(mrt, vr->mifi)) {
			vr->icount = READ_ONCE(vif->pkt_in);
			vr->ocount = READ_ONCE(vif->pkt_out);
			vr->ibytes = READ_ONCE(vif->bytes_in);
			vr->obytes = READ_ONCE(vif->bytes_out);
			rcu_read_unlock();
			return 0;
		}
		rcu_read_unlock();
		return -EADDRNOTAVAIL;
	case SIOCGETSGCNT_IN6:
		sr = (struct sioc_sg_req6 *)arg;

		rcu_read_lock();
		c = ip6mr_cache_find(mrt, &sr->src.sin6_addr,
				     &sr->grp.sin6_addr);
		if (c) {
			sr->pktcnt = c->_c.mfc_un.res.pkt;
			sr->bytecnt = c->_c.mfc_un.res.bytes;
			sr->wrong_if = c->_c.mfc_un.res.wrong_if;
			rcu_read_unlock();
			return 0;
		}
		rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EADDRNOTAVAIL;
	default:
		return -ENOIOCTLCMD;
	}
}

#ifdef CONFIG_COMPAT
struct compat_sioc_sg_req6 {
	struct sockaddr_in6 src;
	struct sockaddr_in6 grp;
	compat_ulong_t pktcnt;
	compat_ulong_t bytecnt;
	compat_ulong_t wrong_if;
};

struct compat_sioc_mif_req6 {
	mifi_t	mifi;
	compat_ulong_t icount;
	compat_ulong_t ocount;
	compat_ulong_t ibytes;
	compat_ulong_t obytes;
};

int ip6mr_compat_ioctl(struct sock *sk, unsigned int cmd, void __user *arg)
{
	struct compat_sioc_sg_req6 sr;
	struct compat_sioc_mif_req6 vr;
<<<<<<< HEAD
	struct mif_device *vif;
	struct mfc6_cache *c;
	struct net *net = sock_net(sk);
	struct mr6_table *mrt;

	mrt = ip6mr_get_table(net, raw6_sk(sk)->ip6mr_table ? : RT6_TABLE_DFLT);
	if (mrt == NULL)
=======
	struct vif_device *vif;
	struct mfc6_cache *c;
	struct net *net = sock_net(sk);
	struct mr_table *mrt;

	mrt = ip6mr_get_table(net, raw6_sk(sk)->ip6mr_table ? : RT6_TABLE_DFLT);
	if (!mrt)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ENOENT;

	switch (cmd) {
	case SIOCGETMIFCNT_IN6:
		if (copy_from_user(&vr, arg, sizeof(vr)))
			return -EFAULT;
		if (vr.mifi >= mrt->maxvif)
			return -EINVAL;
<<<<<<< HEAD
		read_lock(&mrt_lock);
		vif = &mrt->vif6_table[vr.mifi];
		if (MIF_EXISTS(mrt, vr.mifi)) {
			vr.icount = vif->pkt_in;
			vr.ocount = vif->pkt_out;
			vr.ibytes = vif->bytes_in;
			vr.obytes = vif->bytes_out;
			read_unlock(&mrt_lock);
=======
		vr.mifi = array_index_nospec(vr.mifi, mrt->maxvif);
		rcu_read_lock();
		vif = &mrt->vif_table[vr.mifi];
		if (VIF_EXISTS(mrt, vr.mifi)) {
			vr.icount = READ_ONCE(vif->pkt_in);
			vr.ocount = READ_ONCE(vif->pkt_out);
			vr.ibytes = READ_ONCE(vif->bytes_in);
			vr.obytes = READ_ONCE(vif->bytes_out);
			rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

			if (copy_to_user(arg, &vr, sizeof(vr)))
				return -EFAULT;
			return 0;
		}
<<<<<<< HEAD
		read_unlock(&mrt_lock);
=======
		rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EADDRNOTAVAIL;
	case SIOCGETSGCNT_IN6:
		if (copy_from_user(&sr, arg, sizeof(sr)))
			return -EFAULT;

<<<<<<< HEAD
		read_lock(&mrt_lock);
		c = ip6mr_cache_find(mrt, &sr.src.sin6_addr, &sr.grp.sin6_addr);
		if (c) {
			sr.pktcnt = c->mfc_un.res.pkt;
			sr.bytecnt = c->mfc_un.res.bytes;
			sr.wrong_if = c->mfc_un.res.wrong_if;
			read_unlock(&mrt_lock);
=======
		rcu_read_lock();
		c = ip6mr_cache_find(mrt, &sr.src.sin6_addr, &sr.grp.sin6_addr);
		if (c) {
			sr.pktcnt = c->_c.mfc_un.res.pkt;
			sr.bytecnt = c->_c.mfc_un.res.bytes;
			sr.wrong_if = c->_c.mfc_un.res.wrong_if;
			rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

			if (copy_to_user(arg, &sr, sizeof(sr)))
				return -EFAULT;
			return 0;
		}
<<<<<<< HEAD
		read_unlock(&mrt_lock);
=======
		rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EADDRNOTAVAIL;
	default:
		return -ENOIOCTLCMD;
	}
}
#endif

<<<<<<< HEAD
static inline int ip6mr_forward2_finish(struct sk_buff *skb)
{
	IP6_INC_STATS_BH(dev_net(skb_dst(skb)->dev), ip6_dst_idev(skb_dst(skb)),
			 IPSTATS_MIB_OUTFORWDATAGRAMS);
	return dst_output(skb);
=======
static inline int ip6mr_forward2_finish(struct net *net, struct sock *sk, struct sk_buff *skb)
{
	IP6_INC_STATS(net, ip6_dst_idev(skb_dst(skb)),
		      IPSTATS_MIB_OUTFORWDATAGRAMS);
	return dst_output(net, sk, skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 *	Processing handlers for ip6mr_forward
 */

<<<<<<< HEAD
static int ip6mr_forward2(struct net *net, struct mr6_table *mrt,
			  struct sk_buff *skb, struct mfc6_cache *c, int vifi)
{
	struct ipv6hdr *ipv6h;
	struct mif_device *vif = &mrt->vif6_table[vifi];
	struct net_device *dev;
	struct dst_entry *dst;
	struct flowi6 fl6;

	if (vif->dev == NULL)
=======
static int ip6mr_forward2(struct net *net, struct mr_table *mrt,
			  struct sk_buff *skb, int vifi)
{
	struct vif_device *vif = &mrt->vif_table[vifi];
	struct net_device *vif_dev;
	struct ipv6hdr *ipv6h;
	struct dst_entry *dst;
	struct flowi6 fl6;

	vif_dev = vif_dev_read(vif);
	if (!vif_dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out_free;

#ifdef CONFIG_IPV6_PIMSM_V2
	if (vif->flags & MIFF_REGISTER) {
<<<<<<< HEAD
		vif->pkt_out++;
		vif->bytes_out += skb->len;
		vif->dev->stats.tx_bytes += skb->len;
		vif->dev->stats.tx_packets++;
=======
		WRITE_ONCE(vif->pkt_out, vif->pkt_out + 1);
		WRITE_ONCE(vif->bytes_out, vif->bytes_out + skb->len);
		DEV_STATS_ADD(vif_dev, tx_bytes, skb->len);
		DEV_STATS_INC(vif_dev, tx_packets);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ip6mr_cache_report(mrt, skb, vifi, MRT6MSG_WHOLEPKT);
		goto out_free;
	}
#endif

	ipv6h = ipv6_hdr(skb);

	fl6 = (struct flowi6) {
		.flowi6_oif = vif->link,
		.daddr = ipv6h->daddr,
	};

	dst = ip6_route_output(net, NULL, &fl6);
	if (dst->error) {
		dst_release(dst);
		goto out_free;
	}

	skb_dst_drop(skb);
	skb_dst_set(skb, dst);

	/*
	 * RFC1584 teaches, that DVMRP/PIM router must deliver packets locally
	 * not only before forwarding, but after forwarding on all output
	 * interfaces. It is clear, if mrouter runs a multicasting
	 * program, it should receive packets not depending to what interface
	 * program is joined.
	 * If we will not make it, the program will have to join on all
	 * interfaces. On the other hand, multihoming host (or router, but
	 * not mrouter) cannot join to more than one interface - it will
	 * result in receiving multiple packets.
	 */
<<<<<<< HEAD
	dev = vif->dev;
	skb->dev = dev;
	vif->pkt_out++;
	vif->bytes_out += skb->len;

	/* We are about to write */
	/* XXX: extension headers? */
	if (skb_cow(skb, sizeof(*ipv6h) + LL_RESERVED_SPACE(dev)))
=======
	skb->dev = vif_dev;
	WRITE_ONCE(vif->pkt_out, vif->pkt_out + 1);
	WRITE_ONCE(vif->bytes_out, vif->bytes_out + skb->len);

	/* We are about to write */
	/* XXX: extension headers? */
	if (skb_cow(skb, sizeof(*ipv6h) + LL_RESERVED_SPACE(vif_dev)))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out_free;

	ipv6h = ipv6_hdr(skb);
	ipv6h->hop_limit--;

	IP6CB(skb)->flags |= IP6SKB_FORWARDED;

<<<<<<< HEAD
	return NF_HOOK(NFPROTO_IPV6, NF_INET_FORWARD, skb, skb->dev, dev,
=======
	return NF_HOOK(NFPROTO_IPV6, NF_INET_FORWARD,
		       net, NULL, skb, skb->dev, vif_dev,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		       ip6mr_forward2_finish);

out_free:
	kfree_skb(skb);
	return 0;
}

<<<<<<< HEAD
static int ip6mr_find_vif(struct mr6_table *mrt, struct net_device *dev)
{
	int ct;

	for (ct = mrt->maxvif - 1; ct >= 0; ct--) {
		if (mrt->vif6_table[ct].dev == dev)
=======
/* Called with rcu_read_lock() */
static int ip6mr_find_vif(struct mr_table *mrt, struct net_device *dev)
{
	int ct;

	/* Pairs with WRITE_ONCE() in mif6_delete()/mif6_add() */
	for (ct = READ_ONCE(mrt->maxvif) - 1; ct >= 0; ct--) {
		if (rcu_access_pointer(mrt->vif_table[ct].dev) == dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
	}
	return ct;
}

<<<<<<< HEAD
static int ip6_mr_forward(struct net *net, struct mr6_table *mrt,
			  struct sk_buff *skb, struct mfc6_cache *cache)
{
	int psend = -1;
	int vif, ct;

	vif = cache->mf6c_parent;
	cache->mfc_un.res.pkt++;
	cache->mfc_un.res.bytes += skb->len;
=======
/* Called under rcu_read_lock() */
static void ip6_mr_forward(struct net *net, struct mr_table *mrt,
			   struct net_device *dev, struct sk_buff *skb,
			   struct mfc6_cache *c)
{
	int psend = -1;
	int vif, ct;
	int true_vifi = ip6mr_find_vif(mrt, dev);

	vif = c->_c.mfc_parent;
	c->_c.mfc_un.res.pkt++;
	c->_c.mfc_un.res.bytes += skb->len;
	c->_c.mfc_un.res.lastuse = jiffies;

	if (ipv6_addr_any(&c->mf6c_origin) && true_vifi >= 0) {
		struct mfc6_cache *cache_proxy;

		/* For an (*,G) entry, we only check that the incoming
		 * interface is part of the static tree.
		 */
		cache_proxy = mr_mfc_find_any_parent(mrt, vif);
		if (cache_proxy &&
		    cache_proxy->_c.mfc_un.res.ttls[true_vifi] < 255)
			goto forward;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Wrong interface: drop packet and (maybe) send PIM assert.
	 */
<<<<<<< HEAD
	if (mrt->vif6_table[vif].dev != skb->dev) {
		int true_vifi;

		cache->mfc_un.res.wrong_if++;
		true_vifi = ip6mr_find_vif(mrt, skb->dev);
=======
	if (rcu_access_pointer(mrt->vif_table[vif].dev) != dev) {
		c->_c.mfc_un.res.wrong_if++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (true_vifi >= 0 && mrt->mroute_do_assert &&
		    /* pimsm uses asserts, when switching from RPT to SPT,
		       so that we cannot check that packet arrived on an oif.
		       It is bad, but otherwise we would need to move pretty
		       large chunk of pimd to kernel. Ough... --ANK
		     */
		    (mrt->mroute_do_pim ||
<<<<<<< HEAD
		     cache->mfc_un.res.ttls[true_vifi] < 255) &&
		    time_after(jiffies,
			       cache->mfc_un.res.last_assert + MFC_ASSERT_THRESH)) {
			cache->mfc_un.res.last_assert = jiffies;
			ip6mr_cache_report(mrt, skb, true_vifi, MRT6MSG_WRONGMIF);
=======
		     c->_c.mfc_un.res.ttls[true_vifi] < 255) &&
		    time_after(jiffies,
			       c->_c.mfc_un.res.last_assert +
			       MFC_ASSERT_THRESH)) {
			c->_c.mfc_un.res.last_assert = jiffies;
			ip6mr_cache_report(mrt, skb, true_vifi, MRT6MSG_WRONGMIF);
			if (mrt->mroute_do_wrvifwhole)
				ip6mr_cache_report(mrt, skb, true_vifi,
						   MRT6MSG_WRMIFWHOLE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		goto dont_forward;
	}

<<<<<<< HEAD
	mrt->vif6_table[vif].pkt_in++;
	mrt->vif6_table[vif].bytes_in += skb->len;
=======
forward:
	WRITE_ONCE(mrt->vif_table[vif].pkt_in,
		   mrt->vif_table[vif].pkt_in + 1);
	WRITE_ONCE(mrt->vif_table[vif].bytes_in,
		   mrt->vif_table[vif].bytes_in + skb->len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 *	Forward the frame
	 */
<<<<<<< HEAD
	for (ct = cache->mfc_un.res.maxvif - 1; ct >= cache->mfc_un.res.minvif; ct--) {
		if (ipv6_hdr(skb)->hop_limit > cache->mfc_un.res.ttls[ct]) {
			if (psend != -1) {
				struct sk_buff *skb2 = skb_clone(skb, GFP_ATOMIC);
				if (skb2)
					ip6mr_forward2(net, mrt, skb2, cache, psend);
=======
	if (ipv6_addr_any(&c->mf6c_origin) &&
	    ipv6_addr_any(&c->mf6c_mcastgrp)) {
		if (true_vifi >= 0 &&
		    true_vifi != c->_c.mfc_parent &&
		    ipv6_hdr(skb)->hop_limit >
				c->_c.mfc_un.res.ttls[c->_c.mfc_parent]) {
			/* It's an (*,*) entry and the packet is not coming from
			 * the upstream: forward the packet to the upstream
			 * only.
			 */
			psend = c->_c.mfc_parent;
			goto last_forward;
		}
		goto dont_forward;
	}
	for (ct = c->_c.mfc_un.res.maxvif - 1;
	     ct >= c->_c.mfc_un.res.minvif; ct--) {
		/* For (*,G) entry, don't forward to the incoming interface */
		if ((!ipv6_addr_any(&c->mf6c_origin) || ct != true_vifi) &&
		    ipv6_hdr(skb)->hop_limit > c->_c.mfc_un.res.ttls[ct]) {
			if (psend != -1) {
				struct sk_buff *skb2 = skb_clone(skb, GFP_ATOMIC);
				if (skb2)
					ip6mr_forward2(net, mrt, skb2, psend);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			}
			psend = ct;
		}
	}
<<<<<<< HEAD
	if (psend != -1) {
		ip6mr_forward2(net, mrt, skb, cache, psend);
		return 0;
=======
last_forward:
	if (psend != -1) {
		ip6mr_forward2(net, mrt, skb, psend);
		return;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

dont_forward:
	kfree_skb(skb);
<<<<<<< HEAD
	return 0;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}


/*
 *	Multicast packets for forwarding arrive here
 */

int ip6_mr_input(struct sk_buff *skb)
{
	struct mfc6_cache *cache;
	struct net *net = dev_net(skb->dev);
<<<<<<< HEAD
	struct mr6_table *mrt;
=======
	struct mr_table *mrt;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct flowi6 fl6 = {
		.flowi6_iif	= skb->dev->ifindex,
		.flowi6_mark	= skb->mark,
	};
	int err;
<<<<<<< HEAD
=======
	struct net_device *dev;

	/* skb->dev passed in is the master dev for vrfs.
	 * Get the proper interface that does have a vif associated with it.
	 */
	dev = skb->dev;
	if (netif_is_l3_master(skb->dev)) {
		dev = dev_get_by_index_rcu(net, IPCB(skb)->iif);
		if (!dev) {
			kfree_skb(skb);
			return -ENODEV;
		}
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	err = ip6mr_fib_lookup(net, &fl6, &mrt);
	if (err < 0) {
		kfree_skb(skb);
		return err;
	}

<<<<<<< HEAD
	read_lock(&mrt_lock);
	cache = ip6mr_cache_find(mrt,
				 &ipv6_hdr(skb)->saddr, &ipv6_hdr(skb)->daddr);
=======
	cache = ip6mr_cache_find(mrt,
				 &ipv6_hdr(skb)->saddr, &ipv6_hdr(skb)->daddr);
	if (!cache) {
		int vif = ip6mr_find_vif(mrt, dev);

		if (vif >= 0)
			cache = ip6mr_cache_find_any(mrt,
						     &ipv6_hdr(skb)->daddr,
						     vif);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 *	No usable cache entry
	 */
<<<<<<< HEAD
	if (cache == NULL) {
		int vif;

		vif = ip6mr_find_vif(mrt, skb->dev);
		if (vif >= 0) {
			int err = ip6mr_cache_unresolved(mrt, vif, skb);
			read_unlock(&mrt_lock);

			return err;
		}
		read_unlock(&mrt_lock);
=======
	if (!cache) {
		int vif;

		vif = ip6mr_find_vif(mrt, dev);
		if (vif >= 0) {
			int err = ip6mr_cache_unresolved(mrt, vif, skb, dev);

			return err;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		kfree_skb(skb);
		return -ENODEV;
	}

<<<<<<< HEAD
	ip6_mr_forward(net, mrt, skb, cache);

	read_unlock(&mrt_lock);
=======
	ip6_mr_forward(net, mrt, dev, skb, cache);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD

static int __ip6mr_fill_mroute(struct mr6_table *mrt, struct sk_buff *skb,
			       struct mfc6_cache *c, struct rtmsg *rtm)
{
	int ct;
	struct rtnexthop *nhp;
	u8 *b = skb_tail_pointer(skb);
	struct rtattr *mp_head;

	/* If cache is unresolved, don't try to parse IIF and OIF */
	if (c->mf6c_parent >= MAXMIFS)
		return -ENOENT;

	if (MIF_EXISTS(mrt, c->mf6c_parent))
		RTA_PUT(skb, RTA_IIF, 4, &mrt->vif6_table[c->mf6c_parent].dev->ifindex);

	mp_head = (struct rtattr *)skb_put(skb, RTA_LENGTH(0));

	for (ct = c->mfc_un.res.minvif; ct < c->mfc_un.res.maxvif; ct++) {
		if (MIF_EXISTS(mrt, ct) && c->mfc_un.res.ttls[ct] < 255) {
			if (skb_tailroom(skb) < RTA_ALIGN(RTA_ALIGN(sizeof(*nhp)) + 4))
				goto rtattr_failure;
			nhp = (struct rtnexthop *)skb_put(skb, RTA_ALIGN(sizeof(*nhp)));
			nhp->rtnh_flags = 0;
			nhp->rtnh_hops = c->mfc_un.res.ttls[ct];
			nhp->rtnh_ifindex = mrt->vif6_table[ct].dev->ifindex;
			nhp->rtnh_len = sizeof(*nhp);
		}
	}
	mp_head->rta_type = RTA_MULTIPATH;
	mp_head->rta_len = skb_tail_pointer(skb) - (u8 *)mp_head;
	rtm->rtm_type = RTN_MULTICAST;
	return 1;

rtattr_failure:
	nlmsg_trim(skb, b);
	return -EMSGSIZE;
}

int ip6mr_get_route(struct net *net,
		    struct sk_buff *skb, struct rtmsg *rtm, int nowait)
{
	int err;
	struct mr6_table *mrt;
=======
int ip6mr_get_route(struct net *net, struct sk_buff *skb, struct rtmsg *rtm,
		    u32 portid)
{
	int err;
	struct mr_table *mrt;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct mfc6_cache *cache;
	struct rt6_info *rt = (struct rt6_info *)skb_dst(skb);

	mrt = ip6mr_get_table(net, RT6_TABLE_DFLT);
<<<<<<< HEAD
	if (mrt == NULL)
		return -ENOENT;

	read_lock(&mrt_lock);
	cache = ip6mr_cache_find(mrt, &rt->rt6i_src.addr, &rt->rt6i_dst.addr);
=======
	if (!mrt)
		return -ENOENT;

	rcu_read_lock();
	cache = ip6mr_cache_find(mrt, &rt->rt6i_src.addr, &rt->rt6i_dst.addr);
	if (!cache && skb->dev) {
		int vif = ip6mr_find_vif(mrt, skb->dev);

		if (vif >= 0)
			cache = ip6mr_cache_find_any(mrt, &rt->rt6i_dst.addr,
						     vif);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!cache) {
		struct sk_buff *skb2;
		struct ipv6hdr *iph;
		struct net_device *dev;
		int vif;

<<<<<<< HEAD
		if (nowait) {
			read_unlock(&mrt_lock);
			return -EAGAIN;
		}

		dev = skb->dev;
		if (dev == NULL || (vif = ip6mr_find_vif(mrt, dev)) < 0) {
			read_unlock(&mrt_lock);
=======
		dev = skb->dev;
		if (!dev || (vif = ip6mr_find_vif(mrt, dev)) < 0) {
			rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -ENODEV;
		}

		/* really correct? */
		skb2 = alloc_skb(sizeof(struct ipv6hdr), GFP_ATOMIC);
		if (!skb2) {
<<<<<<< HEAD
			read_unlock(&mrt_lock);
			return -ENOMEM;
		}

=======
			rcu_read_unlock();
			return -ENOMEM;
		}

		NETLINK_CB(skb2).portid = portid;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		skb_reset_transport_header(skb2);

		skb_put(skb2, sizeof(struct ipv6hdr));
		skb_reset_network_header(skb2);

		iph = ipv6_hdr(skb2);
		iph->version = 0;
		iph->priority = 0;
		iph->flow_lbl[0] = 0;
		iph->flow_lbl[1] = 0;
		iph->flow_lbl[2] = 0;
		iph->payload_len = 0;
		iph->nexthdr = IPPROTO_NONE;
		iph->hop_limit = 0;
		iph->saddr = rt->rt6i_src.addr;
		iph->daddr = rt->rt6i_dst.addr;

<<<<<<< HEAD
		err = ip6mr_cache_unresolved(mrt, vif, skb2);
		read_unlock(&mrt_lock);
=======
		err = ip6mr_cache_unresolved(mrt, vif, skb2, dev);
		rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		return err;
	}

<<<<<<< HEAD
	if (!nowait && (rtm->rtm_flags&RTM_F_NOTIFY))
		cache->mfc_flags |= MFC_NOTIFY;

	err = __ip6mr_fill_mroute(mrt, skb, cache, rtm);
	read_unlock(&mrt_lock);
	return err;
}

static int ip6mr_fill_mroute(struct mr6_table *mrt, struct sk_buff *skb,
			     u32 pid, u32 seq, struct mfc6_cache *c)
{
	struct nlmsghdr *nlh;
	struct rtmsg *rtm;

	nlh = nlmsg_put(skb, pid, seq, RTM_NEWROUTE, sizeof(*rtm), NLM_F_MULTI);
	if (nlh == NULL)
		return -EMSGSIZE;

	rtm = nlmsg_data(nlh);
	rtm->rtm_family   = RTNL_FAMILY_IPMR;
=======
	err = mr_fill_mroute(mrt, skb, &cache->_c, rtm);
	rcu_read_unlock();
	return err;
}

static int ip6mr_fill_mroute(struct mr_table *mrt, struct sk_buff *skb,
			     u32 portid, u32 seq, struct mfc6_cache *c, int cmd,
			     int flags)
{
	struct nlmsghdr *nlh;
	struct rtmsg *rtm;
	int err;

	nlh = nlmsg_put(skb, portid, seq, cmd, sizeof(*rtm), flags);
	if (!nlh)
		return -EMSGSIZE;

	rtm = nlmsg_data(nlh);
	rtm->rtm_family   = RTNL_FAMILY_IP6MR;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rtm->rtm_dst_len  = 128;
	rtm->rtm_src_len  = 128;
	rtm->rtm_tos      = 0;
	rtm->rtm_table    = mrt->id;
<<<<<<< HEAD
	NLA_PUT_U32(skb, RTA_TABLE, mrt->id);
	rtm->rtm_scope    = RT_SCOPE_UNIVERSE;
	rtm->rtm_protocol = RTPROT_UNSPEC;
	rtm->rtm_flags    = 0;

	NLA_PUT(skb, RTA_SRC, 16, &c->mf6c_origin);
	NLA_PUT(skb, RTA_DST, 16, &c->mf6c_mcastgrp);

	if (__ip6mr_fill_mroute(mrt, skb, c, rtm) < 0)
		goto nla_put_failure;

	return nlmsg_end(skb, nlh);
=======
	if (nla_put_u32(skb, RTA_TABLE, mrt->id))
		goto nla_put_failure;
	rtm->rtm_type = RTN_MULTICAST;
	rtm->rtm_scope    = RT_SCOPE_UNIVERSE;
	if (c->_c.mfc_flags & MFC_STATIC)
		rtm->rtm_protocol = RTPROT_STATIC;
	else
		rtm->rtm_protocol = RTPROT_MROUTED;
	rtm->rtm_flags    = 0;

	if (nla_put_in6_addr(skb, RTA_SRC, &c->mf6c_origin) ||
	    nla_put_in6_addr(skb, RTA_DST, &c->mf6c_mcastgrp))
		goto nla_put_failure;
	err = mr_fill_mroute(mrt, skb, &c->_c, rtm);
	/* do not break the dump if cache is unresolved */
	if (err < 0 && err != -ENOENT)
		goto nla_put_failure;

	nlmsg_end(skb, nlh);
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

nla_put_failure:
	nlmsg_cancel(skb, nlh);
	return -EMSGSIZE;
}

<<<<<<< HEAD
static int ip6mr_rtm_dumproute(struct sk_buff *skb, struct netlink_callback *cb)
{
	struct net *net = sock_net(skb->sk);
	struct mr6_table *mrt;
	struct mfc6_cache *mfc;
	unsigned int t = 0, s_t;
	unsigned int h = 0, s_h;
	unsigned int e = 0, s_e;

	s_t = cb->args[0];
	s_h = cb->args[1];
	s_e = cb->args[2];

	read_lock(&mrt_lock);
	ip6mr_for_each_table(mrt, net) {
		if (t < s_t)
			goto next_table;
		if (t > s_t)
			s_h = 0;
		for (h = s_h; h < MFC6_LINES; h++) {
			list_for_each_entry(mfc, &mrt->mfc6_cache_array[h], list) {
				if (e < s_e)
					goto next_entry;
				if (ip6mr_fill_mroute(mrt, skb,
						      NETLINK_CB(cb->skb).pid,
						      cb->nlh->nlmsg_seq,
						      mfc) < 0)
					goto done;
next_entry:
				e++;
			}
			e = s_e = 0;
		}
		s_h = 0;
next_table:
		t++;
	}
done:
	read_unlock(&mrt_lock);

	cb->args[2] = e;
	cb->args[1] = h;
	cb->args[0] = t;

	return skb->len;
=======
static int _ip6mr_fill_mroute(struct mr_table *mrt, struct sk_buff *skb,
			      u32 portid, u32 seq, struct mr_mfc *c,
			      int cmd, int flags)
{
	return ip6mr_fill_mroute(mrt, skb, portid, seq, (struct mfc6_cache *)c,
				 cmd, flags);
}

static int mr6_msgsize(bool unresolved, int maxvif)
{
	size_t len =
		NLMSG_ALIGN(sizeof(struct rtmsg))
		+ nla_total_size(4)	/* RTA_TABLE */
		+ nla_total_size(sizeof(struct in6_addr))	/* RTA_SRC */
		+ nla_total_size(sizeof(struct in6_addr))	/* RTA_DST */
		;

	if (!unresolved)
		len = len
		      + nla_total_size(4)	/* RTA_IIF */
		      + nla_total_size(0)	/* RTA_MULTIPATH */
		      + maxvif * NLA_ALIGN(sizeof(struct rtnexthop))
						/* RTA_MFC_STATS */
		      + nla_total_size_64bit(sizeof(struct rta_mfc_stats))
		;

	return len;
}

static void mr6_netlink_event(struct mr_table *mrt, struct mfc6_cache *mfc,
			      int cmd)
{
	struct net *net = read_pnet(&mrt->net);
	struct sk_buff *skb;
	int err = -ENOBUFS;

	skb = nlmsg_new(mr6_msgsize(mfc->_c.mfc_parent >= MAXMIFS, mrt->maxvif),
			GFP_ATOMIC);
	if (!skb)
		goto errout;

	err = ip6mr_fill_mroute(mrt, skb, 0, 0, mfc, cmd, 0);
	if (err < 0)
		goto errout;

	rtnl_notify(skb, net, 0, RTNLGRP_IPV6_MROUTE, NULL, GFP_ATOMIC);
	return;

errout:
	kfree_skb(skb);
	if (err < 0)
		rtnl_set_sk_err(net, RTNLGRP_IPV6_MROUTE, err);
}

static size_t mrt6msg_netlink_msgsize(size_t payloadlen)
{
	size_t len =
		NLMSG_ALIGN(sizeof(struct rtgenmsg))
		+ nla_total_size(1)	/* IP6MRA_CREPORT_MSGTYPE */
		+ nla_total_size(4)	/* IP6MRA_CREPORT_MIF_ID */
					/* IP6MRA_CREPORT_SRC_ADDR */
		+ nla_total_size(sizeof(struct in6_addr))
					/* IP6MRA_CREPORT_DST_ADDR */
		+ nla_total_size(sizeof(struct in6_addr))
					/* IP6MRA_CREPORT_PKT */
		+ nla_total_size(payloadlen)
		;

	return len;
}

static void mrt6msg_netlink_event(const struct mr_table *mrt, struct sk_buff *pkt)
{
	struct net *net = read_pnet(&mrt->net);
	struct nlmsghdr *nlh;
	struct rtgenmsg *rtgenm;
	struct mrt6msg *msg;
	struct sk_buff *skb;
	struct nlattr *nla;
	int payloadlen;

	payloadlen = pkt->len - sizeof(struct mrt6msg);
	msg = (struct mrt6msg *)skb_transport_header(pkt);

	skb = nlmsg_new(mrt6msg_netlink_msgsize(payloadlen), GFP_ATOMIC);
	if (!skb)
		goto errout;

	nlh = nlmsg_put(skb, 0, 0, RTM_NEWCACHEREPORT,
			sizeof(struct rtgenmsg), 0);
	if (!nlh)
		goto errout;
	rtgenm = nlmsg_data(nlh);
	rtgenm->rtgen_family = RTNL_FAMILY_IP6MR;
	if (nla_put_u8(skb, IP6MRA_CREPORT_MSGTYPE, msg->im6_msgtype) ||
	    nla_put_u32(skb, IP6MRA_CREPORT_MIF_ID, msg->im6_mif) ||
	    nla_put_in6_addr(skb, IP6MRA_CREPORT_SRC_ADDR,
			     &msg->im6_src) ||
	    nla_put_in6_addr(skb, IP6MRA_CREPORT_DST_ADDR,
			     &msg->im6_dst))
		goto nla_put_failure;

	nla = nla_reserve(skb, IP6MRA_CREPORT_PKT, payloadlen);
	if (!nla || skb_copy_bits(pkt, sizeof(struct mrt6msg),
				  nla_data(nla), payloadlen))
		goto nla_put_failure;

	nlmsg_end(skb, nlh);

	rtnl_notify(skb, net, 0, RTNLGRP_IPV6_MROUTE_R, NULL, GFP_ATOMIC);
	return;

nla_put_failure:
	nlmsg_cancel(skb, nlh);
errout:
	kfree_skb(skb);
	rtnl_set_sk_err(net, RTNLGRP_IPV6_MROUTE_R, -ENOBUFS);
}

static const struct nla_policy ip6mr_getroute_policy[RTA_MAX + 1] = {
	[RTA_SRC]		= NLA_POLICY_EXACT_LEN(sizeof(struct in6_addr)),
	[RTA_DST]		= NLA_POLICY_EXACT_LEN(sizeof(struct in6_addr)),
	[RTA_TABLE]		= { .type = NLA_U32 },
};

static int ip6mr_rtm_valid_getroute_req(struct sk_buff *skb,
					const struct nlmsghdr *nlh,
					struct nlattr **tb,
					struct netlink_ext_ack *extack)
{
	struct rtmsg *rtm;
	int err;

	err = nlmsg_parse(nlh, sizeof(*rtm), tb, RTA_MAX, ip6mr_getroute_policy,
			  extack);
	if (err)
		return err;

	rtm = nlmsg_data(nlh);
	if ((rtm->rtm_src_len && rtm->rtm_src_len != 128) ||
	    (rtm->rtm_dst_len && rtm->rtm_dst_len != 128) ||
	    rtm->rtm_tos || rtm->rtm_table || rtm->rtm_protocol ||
	    rtm->rtm_scope || rtm->rtm_type || rtm->rtm_flags) {
		NL_SET_ERR_MSG_MOD(extack,
				   "Invalid values in header for multicast route get request");
		return -EINVAL;
	}

	if ((tb[RTA_SRC] && !rtm->rtm_src_len) ||
	    (tb[RTA_DST] && !rtm->rtm_dst_len)) {
		NL_SET_ERR_MSG_MOD(extack, "rtm_src_len and rtm_dst_len must be 128 for IPv6");
		return -EINVAL;
	}

	return 0;
}

static int ip6mr_rtm_getroute(struct sk_buff *in_skb, struct nlmsghdr *nlh,
			      struct netlink_ext_ack *extack)
{
	struct net *net = sock_net(in_skb->sk);
	struct in6_addr src = {}, grp = {};
	struct nlattr *tb[RTA_MAX + 1];
	struct mfc6_cache *cache;
	struct mr_table *mrt;
	struct sk_buff *skb;
	u32 tableid;
	int err;

	err = ip6mr_rtm_valid_getroute_req(in_skb, nlh, tb, extack);
	if (err < 0)
		return err;

	if (tb[RTA_SRC])
		src = nla_get_in6_addr(tb[RTA_SRC]);
	if (tb[RTA_DST])
		grp = nla_get_in6_addr(tb[RTA_DST]);
	tableid = tb[RTA_TABLE] ? nla_get_u32(tb[RTA_TABLE]) : 0;

	mrt = ip6mr_get_table(net, tableid ?: RT_TABLE_DEFAULT);
	if (!mrt) {
		NL_SET_ERR_MSG_MOD(extack, "MR table does not exist");
		return -ENOENT;
	}

	/* entries are added/deleted only under RTNL */
	rcu_read_lock();
	cache = ip6mr_cache_find(mrt, &src, &grp);
	rcu_read_unlock();
	if (!cache) {
		NL_SET_ERR_MSG_MOD(extack, "MR cache entry not found");
		return -ENOENT;
	}

	skb = nlmsg_new(mr6_msgsize(false, mrt->maxvif), GFP_KERNEL);
	if (!skb)
		return -ENOBUFS;

	err = ip6mr_fill_mroute(mrt, skb, NETLINK_CB(in_skb).portid,
				nlh->nlmsg_seq, cache, RTM_NEWROUTE, 0);
	if (err < 0) {
		kfree_skb(skb);
		return err;
	}

	return rtnl_unicast(skb, net, NETLINK_CB(in_skb).portid);
}

static int ip6mr_rtm_dumproute(struct sk_buff *skb, struct netlink_callback *cb)
{
	const struct nlmsghdr *nlh = cb->nlh;
	struct fib_dump_filter filter = {
		.rtnl_held = true,
	};
	int err;

	if (cb->strict_check) {
		err = ip_valid_fib_dump_req(sock_net(skb->sk), nlh,
					    &filter, cb);
		if (err < 0)
			return err;
	}

	if (filter.table_id) {
		struct mr_table *mrt;

		mrt = ip6mr_get_table(sock_net(skb->sk), filter.table_id);
		if (!mrt) {
			if (rtnl_msg_family(cb->nlh) != RTNL_FAMILY_IP6MR)
				return skb->len;

			NL_SET_ERR_MSG_MOD(cb->extack, "MR table does not exist");
			return -ENOENT;
		}
		err = mr_table_dump(mrt, skb, cb, _ip6mr_fill_mroute,
				    &mfc_unres_lock, &filter);
		return skb->len ? : err;
	}

	return mr_rtm_dumproute(skb, cb, ip6mr_mr_table_iter,
				_ip6mr_fill_mroute, &mfc_unres_lock, &filter);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
