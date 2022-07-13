<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *	IP multicast routing support for mrouted 3.6/3.8
 *
 *		(c) 1995 Alan Cox, <alan@lxorguk.ukuu.org.uk>
 *	  Linux Consultancy and Custom Driver Development
 *
<<<<<<< HEAD
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *	Fixes:
 *	Michael Chastain	:	Incorrect size of copying.
 *	Alan Cox		:	Added the cache manager code
 *	Alan Cox		:	Fixed the clone/copy bug and device race.
 *	Mike McLagan		:	Routing by source
 *	Malcolm Beattie		:	Buffer handling fixes.
 *	Alexey Kuznetsov	:	Double buffer free and other fixes.
 *	SVR Anand		:	Fixed several multicast bugs and problems.
 *	Alexey Kuznetsov	:	Status, optimisations and more.
 *	Brad Parker		:	Better behaviour on mrouted upcall
 *					overflow.
 *      Carlos Picoto           :       PIMv1 Support
 *	Pavlin Ivanov Radoslavov:	PIMv2 Registers must checksum only PIM header
 *					Relax this requirement to work with older peers.
<<<<<<< HEAD
 *
 */

#include <asm/uaccess.h>
#include <linux/types.h>
#include <linux/capability.h>
#include <linux/errno.h>
#include <linux/timer.h>
=======
 */

#include <linux/uaccess.h>
#include <linux/types.h>
#include <linux/cache.h>
#include <linux/capability.h>
#include <linux/errno.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/mm.h>
#include <linux/kernel.h>
#include <linux/fcntl.h>
#include <linux/stat.h>
#include <linux/socket.h>
#include <linux/in.h>
#include <linux/inet.h>
#include <linux/netdevice.h>
#include <linux/inetdevice.h>
#include <linux/igmp.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/mroute.h>
#include <linux/init.h>
#include <linux/if_ether.h>
#include <linux/slab.h>
#include <net/net_namespace.h>
#include <net/ip.h>
#include <net/protocol.h>
#include <linux/skbuff.h>
#include <net/route.h>
<<<<<<< HEAD
#include <net/sock.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <net/icmp.h>
#include <net/udp.h>
#include <net/raw.h>
#include <linux/notifier.h>
#include <linux/if_arp.h>
#include <linux/netfilter_ipv4.h>
#include <linux/compat.h>
#include <linux/export.h>
<<<<<<< HEAD
#include <net/ipip.h>
#include <net/checksum.h>
#include <net/netlink.h>
#include <net/fib_rules.h>

#if defined(CONFIG_IP_PIMSM_V1) || defined(CONFIG_IP_PIMSM_V2)
#define CONFIG_IP_PIMSM	1
#endif

struct mr_table {
	struct list_head	list;
#ifdef CONFIG_NET_NS
	struct net		*net;
#endif
	u32			id;
	struct sock __rcu	*mroute_sk;
	struct timer_list	ipmr_expire_timer;
	struct list_head	mfc_unres_queue;
	struct list_head	mfc_cache_array[MFC_LINES];
	struct vif_device	vif_table[MAXVIFS];
	int			maxvif;
	atomic_t		cache_resolve_queue_len;
	int			mroute_do_assert;
	int			mroute_do_pim;
#if defined(CONFIG_IP_PIMSM_V1) || defined(CONFIG_IP_PIMSM_V2)
	int			mroute_reg_vif_num;
#endif
};
=======
#include <linux/rhashtable.h>
#include <net/ip_tunnels.h>
#include <net/checksum.h>
#include <net/netlink.h>
#include <net/fib_rules.h>
#include <linux/netconf.h>
#include <net/rtnh.h>

#include <linux/nospec.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct ipmr_rule {
	struct fib_rule		common;
};

struct ipmr_result {
	struct mr_table		*mrt;
};

/* Big lock, protecting vif table, mrt cache and mroute socket state.
 * Note that the changes are semaphored via rtnl_lock.
 */

<<<<<<< HEAD
static DEFINE_RWLOCK(mrt_lock);

/*
 *	Multicast router control variables
 */

#define VIF_EXISTS(_mrt, _idx) ((_mrt)->vif_table[_idx].dev != NULL)
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
 * entries is changed only in process context and protected
 * with weak lock mrt_lock. Queue of unresolved entries is protected
 * with strong spinlock mfc_unres_lock.
 *
 * In this case data path is free of exclusive locks at all.
 */

<<<<<<< HEAD
static struct kmem_cache *mrt_cachep __read_mostly;
=======
static struct kmem_cache *mrt_cachep __ro_after_init;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static struct mr_table *ipmr_new_table(struct net *net, u32 id);
static void ipmr_free_table(struct mr_table *mrt);

<<<<<<< HEAD
static int ip_mr_forward(struct net *net, struct mr_table *mrt,
			 struct sk_buff *skb, struct mfc_cache *cache,
			 int local);
static int ipmr_cache_report(struct mr_table *mrt,
			     struct sk_buff *pkt, vifi_t vifi, int assert);
static int __ipmr_fill_mroute(struct mr_table *mrt, struct sk_buff *skb,
			      struct mfc_cache *c, struct rtmsg *rtm);
static void mroute_clean_tables(struct mr_table *mrt);
static void ipmr_expire_process(unsigned long arg);

#ifdef CONFIG_IP_MROUTE_MULTIPLE_TABLES
#define ipmr_for_each_table(mrt, net) \
	list_for_each_entry_rcu(mrt, &net->ipv4.mr_tables, list)
=======
static void ip_mr_forward(struct net *net, struct mr_table *mrt,
			  struct net_device *dev, struct sk_buff *skb,
			  struct mfc_cache *cache, int local);
static int ipmr_cache_report(const struct mr_table *mrt,
			     struct sk_buff *pkt, vifi_t vifi, int assert);
static void mroute_netlink_event(struct mr_table *mrt, struct mfc_cache *mfc,
				 int cmd);
static void igmpmsg_netlink_event(const struct mr_table *mrt, struct sk_buff *pkt);
static void mroute_clean_tables(struct mr_table *mrt, int flags);
static void ipmr_expire_process(struct timer_list *t);

#ifdef CONFIG_IP_MROUTE_MULTIPLE_TABLES
#define ipmr_for_each_table(mrt, net)					\
	list_for_each_entry_rcu(mrt, &net->ipv4.mr_tables, list,	\
				lockdep_rtnl_is_held() ||		\
				list_empty(&net->ipv4.mr_tables))

static struct mr_table *ipmr_mr_table_iter(struct net *net,
					   struct mr_table *mrt)
{
	struct mr_table *ret;

	if (!mrt)
		ret = list_entry_rcu(net->ipv4.mr_tables.next,
				     struct mr_table, list);
	else
		ret = list_entry_rcu(mrt->list.next,
				     struct mr_table, list);

	if (&ret->list == &net->ipv4.mr_tables)
		return NULL;
	return ret;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static struct mr_table *ipmr_get_table(struct net *net, u32 id)
{
	struct mr_table *mrt;

	ipmr_for_each_table(mrt, net) {
		if (mrt->id == id)
			return mrt;
	}
	return NULL;
}

static int ipmr_fib_lookup(struct net *net, struct flowi4 *flp4,
			   struct mr_table **mrt)
{
	int err;
	struct ipmr_result res;
	struct fib_lookup_arg arg = {
		.result = &res,
		.flags = FIB_LOOKUP_NOREF,
	};

<<<<<<< HEAD
=======
	/* update flow if oif or iif point to device enslaved to l3mdev */
	l3mdev_update_flow(net, flowi4_to_flowi(flp4));

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	err = fib_rules_lookup(net->ipv4.mr_rules_ops,
			       flowi4_to_flowi(flp4), 0, &arg);
	if (err < 0)
		return err;
	*mrt = res.mrt;
	return 0;
}

static int ipmr_rule_action(struct fib_rule *rule, struct flowi *flp,
			    int flags, struct fib_lookup_arg *arg)
{
	struct ipmr_result *res = arg->result;
	struct mr_table *mrt;

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
	mrt = ipmr_get_table(rule->fr_net, rule->table);
	if (mrt == NULL)
=======
	arg->table = fib_rule_get_table(rule, arg);

	mrt = ipmr_get_table(rule->fr_net, arg->table);
	if (!mrt)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EAGAIN;
	res->mrt = mrt;
	return 0;
}

static int ipmr_rule_match(struct fib_rule *rule, struct flowi *fl, int flags)
{
	return 1;
}

<<<<<<< HEAD
static const struct nla_policy ipmr_rule_policy[FRA_MAX + 1] = {
	FRA_GENERIC_POLICY,
};

static int ipmr_rule_configure(struct fib_rule *rule, struct sk_buff *skb,
			       struct fib_rule_hdr *frh, struct nlattr **tb)
=======
static int ipmr_rule_configure(struct fib_rule *rule, struct sk_buff *skb,
			       struct fib_rule_hdr *frh, struct nlattr **tb,
			       struct netlink_ext_ack *extack)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return 0;
}

static int ipmr_rule_compare(struct fib_rule *rule, struct fib_rule_hdr *frh,
			     struct nlattr **tb)
{
	return 1;
}

static int ipmr_rule_fill(struct fib_rule *rule, struct sk_buff *skb,
			  struct fib_rule_hdr *frh)
{
	frh->dst_len = 0;
	frh->src_len = 0;
	frh->tos     = 0;
	return 0;
}

<<<<<<< HEAD
static const struct fib_rules_ops __net_initdata ipmr_rules_ops_template = {
=======
static const struct fib_rules_ops __net_initconst ipmr_rules_ops_template = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.family		= RTNL_FAMILY_IPMR,
	.rule_size	= sizeof(struct ipmr_rule),
	.addr_size	= sizeof(u32),
	.action		= ipmr_rule_action,
	.match		= ipmr_rule_match,
	.configure	= ipmr_rule_configure,
	.compare	= ipmr_rule_compare,
<<<<<<< HEAD
	.default_pref	= fib_default_rule_pref,
	.fill		= ipmr_rule_fill,
	.nlgroup	= RTNLGRP_IPV4_RULE,
	.policy		= ipmr_rule_policy,
=======
	.fill		= ipmr_rule_fill,
	.nlgroup	= RTNLGRP_IPV4_RULE,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.owner		= THIS_MODULE,
};

static int __net_init ipmr_rules_init(struct net *net)
{
	struct fib_rules_ops *ops;
	struct mr_table *mrt;
	int err;

	ops = fib_rules_register(&ipmr_rules_ops_template, net);
	if (IS_ERR(ops))
		return PTR_ERR(ops);

	INIT_LIST_HEAD(&net->ipv4.mr_tables);

	mrt = ipmr_new_table(net, RT_TABLE_DEFAULT);
<<<<<<< HEAD
	if (mrt == NULL) {
		err = -ENOMEM;
		goto err1;
	}

	err = fib_default_rule_add(ops, 0x7fff, RT_TABLE_DEFAULT, 0);
=======
	if (IS_ERR(mrt)) {
		err = PTR_ERR(mrt);
		goto err1;
	}

	err = fib_default_rule_add(ops, 0x7fff, RT_TABLE_DEFAULT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err < 0)
		goto err2;

	net->ipv4.mr_rules_ops = ops;
	return 0;

err2:
<<<<<<< HEAD
	kfree(mrt);
=======
	rtnl_lock();
	ipmr_free_table(mrt);
	rtnl_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
err1:
	fib_rules_unregister(ops);
	return err;
}

static void __net_exit ipmr_rules_exit(struct net *net)
{
	struct mr_table *mrt, *next;

<<<<<<< HEAD
=======
	ASSERT_RTNL();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	list_for_each_entry_safe(mrt, next, &net->ipv4.mr_tables, list) {
		list_del(&mrt->list);
		ipmr_free_table(mrt);
	}
	fib_rules_unregister(net->ipv4.mr_rules_ops);
}
<<<<<<< HEAD
=======

static int ipmr_rules_dump(struct net *net, struct notifier_block *nb,
			   struct netlink_ext_ack *extack)
{
	return fib_rules_dump(net, nb, RTNL_FAMILY_IPMR, extack);
}

static unsigned int ipmr_rules_seq_read(struct net *net)
{
	return fib_rules_seq_read(net, RTNL_FAMILY_IPMR);
}

bool ipmr_rule_default(const struct fib_rule *rule)
{
	return fib_rule_matchall(rule) && rule->table == RT_TABLE_DEFAULT;
}
EXPORT_SYMBOL(ipmr_rule_default);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#else
#define ipmr_for_each_table(mrt, net) \
	for (mrt = net->ipv4.mrt; mrt; mrt = NULL)

<<<<<<< HEAD
=======
static struct mr_table *ipmr_mr_table_iter(struct net *net,
					   struct mr_table *mrt)
{
	if (!mrt)
		return net->ipv4.mrt;
	return NULL;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct mr_table *ipmr_get_table(struct net *net, u32 id)
{
	return net->ipv4.mrt;
}

static int ipmr_fib_lookup(struct net *net, struct flowi4 *flp4,
			   struct mr_table **mrt)
{
	*mrt = net->ipv4.mrt;
	return 0;
}

static int __net_init ipmr_rules_init(struct net *net)
{
<<<<<<< HEAD
	net->ipv4.mrt = ipmr_new_table(net, RT_TABLE_DEFAULT);
	return net->ipv4.mrt ? 0 : -ENOMEM;
=======
	struct mr_table *mrt;

	mrt = ipmr_new_table(net, RT_TABLE_DEFAULT);
	if (IS_ERR(mrt))
		return PTR_ERR(mrt);
	net->ipv4.mrt = mrt;
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void __net_exit ipmr_rules_exit(struct net *net)
{
<<<<<<< HEAD
	ipmr_free_table(net->ipv4.mrt);
}
#endif

static struct mr_table *ipmr_new_table(struct net *net, u32 id)
{
	struct mr_table *mrt;
	unsigned int i;

	mrt = ipmr_get_table(net, id);
	if (mrt != NULL)
		return mrt;

	mrt = kzalloc(sizeof(*mrt), GFP_KERNEL);
	if (mrt == NULL)
		return NULL;
	write_pnet(&mrt->net, net);
	mrt->id = id;

	/* Forwarding cache */
	for (i = 0; i < MFC_LINES; i++)
		INIT_LIST_HEAD(&mrt->mfc_cache_array[i]);

	INIT_LIST_HEAD(&mrt->mfc_unres_queue);

	setup_timer(&mrt->ipmr_expire_timer, ipmr_expire_process,
		    (unsigned long)mrt);

#ifdef CONFIG_IP_PIMSM
	mrt->mroute_reg_vif_num = -1;
#endif
#ifdef CONFIG_IP_MROUTE_MULTIPLE_TABLES
	list_add_tail_rcu(&mrt->list, &net->ipv4.mr_tables);
#endif
	return mrt;
=======
	ASSERT_RTNL();
	ipmr_free_table(net->ipv4.mrt);
	net->ipv4.mrt = NULL;
}

static int ipmr_rules_dump(struct net *net, struct notifier_block *nb,
			   struct netlink_ext_ack *extack)
{
	return 0;
}

static unsigned int ipmr_rules_seq_read(struct net *net)
{
	return 0;
}

bool ipmr_rule_default(const struct fib_rule *rule)
{
	return true;
}
EXPORT_SYMBOL(ipmr_rule_default);
#endif

static inline int ipmr_hash_cmp(struct rhashtable_compare_arg *arg,
				const void *ptr)
{
	const struct mfc_cache_cmp_arg *cmparg = arg->key;
	const struct mfc_cache *c = ptr;

	return cmparg->mfc_mcastgrp != c->mfc_mcastgrp ||
	       cmparg->mfc_origin != c->mfc_origin;
}

static const struct rhashtable_params ipmr_rht_params = {
	.head_offset = offsetof(struct mr_mfc, mnode),
	.key_offset = offsetof(struct mfc_cache, cmparg),
	.key_len = sizeof(struct mfc_cache_cmp_arg),
	.nelem_hint = 3,
	.obj_cmpfn = ipmr_hash_cmp,
	.automatic_shrinking = true,
};

static void ipmr_new_table_set(struct mr_table *mrt,
			       struct net *net)
{
#ifdef CONFIG_IP_MROUTE_MULTIPLE_TABLES
	list_add_tail_rcu(&mrt->list, &net->ipv4.mr_tables);
#endif
}

static struct mfc_cache_cmp_arg ipmr_mr_table_ops_cmparg_any = {
	.mfc_mcastgrp = htonl(INADDR_ANY),
	.mfc_origin = htonl(INADDR_ANY),
};

static struct mr_table_ops ipmr_mr_table_ops = {
	.rht_params = &ipmr_rht_params,
	.cmparg_any = &ipmr_mr_table_ops_cmparg_any,
};

static struct mr_table *ipmr_new_table(struct net *net, u32 id)
{
	struct mr_table *mrt;

	/* "pimreg%u" should not exceed 16 bytes (IFNAMSIZ) */
	if (id != RT_TABLE_DEFAULT && id >= 1000000000)
		return ERR_PTR(-EINVAL);

	mrt = ipmr_get_table(net, id);
	if (mrt)
		return mrt;

	return mr_table_alloc(net, id, &ipmr_mr_table_ops,
			      ipmr_expire_process, ipmr_new_table_set);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void ipmr_free_table(struct mr_table *mrt)
{
<<<<<<< HEAD
	del_timer_sync(&mrt->ipmr_expire_timer);
	mroute_clean_tables(mrt);
=======
	timer_shutdown_sync(&mrt->ipmr_expire_timer);
	mroute_clean_tables(mrt, MRT_FLUSH_VIFS | MRT_FLUSH_VIFS_STATIC |
				 MRT_FLUSH_MFC | MRT_FLUSH_MFC_STATIC);
	rhltable_destroy(&mrt->mfc_hash);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kfree(mrt);
}

/* Service routines creating virtual interfaces: DVMRP tunnels and PIMREG */

<<<<<<< HEAD
static void ipmr_del_tunnel(struct net_device *dev, struct vifctl *v)
{
	struct net *net = dev_net(dev);

	dev_close(dev);

	dev = __dev_get_by_name(net, "tunl0");
	if (dev) {
		const struct net_device_ops *ops = dev->netdev_ops;
		struct ifreq ifr;
		struct ip_tunnel_parm p;

		memset(&p, 0, sizeof(p));
		p.iph.daddr = v->vifc_rmt_addr.s_addr;
		p.iph.saddr = v->vifc_lcl_addr.s_addr;
		p.iph.version = 4;
		p.iph.ihl = 5;
		p.iph.protocol = IPPROTO_IPIP;
		sprintf(p.name, "dvmrp%d", v->vifc_vifi);
		ifr.ifr_ifru.ifru_data = (__force void __user *)&p;

		if (ops->ndo_do_ioctl) {
			mm_segment_t oldfs = get_fs();

			set_fs(KERNEL_DS);
			ops->ndo_do_ioctl(dev, &ifr, SIOCDELTUNNEL);
			set_fs(oldfs);
		}
	}
}

static
struct net_device *ipmr_new_tunnel(struct net *net, struct vifctl *v)
{
	struct net_device  *dev;

	dev = __dev_get_by_name(net, "tunl0");

	if (dev) {
		const struct net_device_ops *ops = dev->netdev_ops;
		int err;
		struct ifreq ifr;
		struct ip_tunnel_parm p;
		struct in_device  *in_dev;

		memset(&p, 0, sizeof(p));
		p.iph.daddr = v->vifc_rmt_addr.s_addr;
		p.iph.saddr = v->vifc_lcl_addr.s_addr;
		p.iph.version = 4;
		p.iph.ihl = 5;
		p.iph.protocol = IPPROTO_IPIP;
		sprintf(p.name, "dvmrp%d", v->vifc_vifi);
		ifr.ifr_ifru.ifru_data = (__force void __user *)&p;

		if (ops->ndo_do_ioctl) {
			mm_segment_t oldfs = get_fs();

			set_fs(KERNEL_DS);
			err = ops->ndo_do_ioctl(dev, &ifr, SIOCADDTUNNEL);
			set_fs(oldfs);
		} else {
			err = -EOPNOTSUPP;
		}
		dev = NULL;

		if (err == 0 &&
		    (dev = __dev_get_by_name(net, p.name)) != NULL) {
			dev->flags |= IFF_MULTICAST;

			in_dev = __in_dev_get_rtnl(dev);
			if (in_dev == NULL)
				goto failure;

			ipv4_devconf_setall(in_dev);
			IPV4_DEVCONF(in_dev->cnf, RP_FILTER) = 0;

			if (dev_open(dev))
				goto failure;
			dev_hold(dev);
		}
	}
	return dev;

failure:
	/* allow the register to be completed before unregistering. */
	rtnl_unlock();
	rtnl_lock();

	unregister_netdevice(dev);
	return NULL;
}

#ifdef CONFIG_IP_PIMSM

=======
/* Initialize ipmr pimreg/tunnel in_device */
static bool ipmr_init_vif_indev(const struct net_device *dev)
{
	struct in_device *in_dev;

	ASSERT_RTNL();

	in_dev = __in_dev_get_rtnl(dev);
	if (!in_dev)
		return false;
	ipv4_devconf_setall(in_dev);
	neigh_parms_data_state_setall(in_dev->arp_parms);
	IPV4_DEVCONF(in_dev->cnf, RP_FILTER) = 0;

	return true;
}

static struct net_device *ipmr_new_tunnel(struct net *net, struct vifctl *v)
{
	struct net_device *tunnel_dev, *new_dev;
	struct ip_tunnel_parm p = { };
	int err;

	tunnel_dev = __dev_get_by_name(net, "tunl0");
	if (!tunnel_dev)
		goto out;

	p.iph.daddr = v->vifc_rmt_addr.s_addr;
	p.iph.saddr = v->vifc_lcl_addr.s_addr;
	p.iph.version = 4;
	p.iph.ihl = 5;
	p.iph.protocol = IPPROTO_IPIP;
	sprintf(p.name, "dvmrp%d", v->vifc_vifi);

	if (!tunnel_dev->netdev_ops->ndo_tunnel_ctl)
		goto out;
	err = tunnel_dev->netdev_ops->ndo_tunnel_ctl(tunnel_dev, &p,
			SIOCADDTUNNEL);
	if (err)
		goto out;

	new_dev = __dev_get_by_name(net, p.name);
	if (!new_dev)
		goto out;

	new_dev->flags |= IFF_MULTICAST;
	if (!ipmr_init_vif_indev(new_dev))
		goto out_unregister;
	if (dev_open(new_dev, NULL))
		goto out_unregister;
	dev_hold(new_dev);
	err = dev_set_allmulti(new_dev, 1);
	if (err) {
		dev_close(new_dev);
		tunnel_dev->netdev_ops->ndo_tunnel_ctl(tunnel_dev, &p,
				SIOCDELTUNNEL);
		dev_put(new_dev);
		new_dev = ERR_PTR(err);
	}
	return new_dev;

out_unregister:
	unregister_netdevice(new_dev);
out:
	return ERR_PTR(-ENOBUFS);
}

#if defined(CONFIG_IP_PIMSM_V1) || defined(CONFIG_IP_PIMSM_V2)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static netdev_tx_t reg_vif_xmit(struct sk_buff *skb, struct net_device *dev)
{
	struct net *net = dev_net(dev);
	struct mr_table *mrt;
	struct flowi4 fl4 = {
		.flowi4_oif	= dev->ifindex,
		.flowi4_iif	= skb->skb_iif ? : LOOPBACK_IFINDEX,
		.flowi4_mark	= skb->mark,
	};
	int err;

	err = ipmr_fib_lookup(net, &fl4, &mrt);
	if (err < 0) {
		kfree_skb(skb);
		return err;
	}

<<<<<<< HEAD
	read_lock(&mrt_lock);
	dev->stats.tx_bytes += skb->len;
	dev->stats.tx_packets++;
	ipmr_cache_report(mrt, skb, mrt->mroute_reg_vif_num, IGMPMSG_WHOLEPKT);
	read_unlock(&mrt_lock);
=======
	DEV_STATS_ADD(dev, tx_bytes, skb->len);
	DEV_STATS_INC(dev, tx_packets);
	rcu_read_lock();

	/* Pairs with WRITE_ONCE() in vif_add() and vif_delete() */
	ipmr_cache_report(mrt, skb, READ_ONCE(mrt->mroute_reg_vif_num),
			  IGMPMSG_WHOLEPKT);

	rcu_read_unlock();
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
	dev->mtu		= ETH_DATA_LEN - sizeof(struct iphdr) - 8;
	dev->flags		= IFF_NOARP;
<<<<<<< HEAD
	dev->netdev_ops		= &reg_vif_netdev_ops,
	dev->destructor		= free_netdev;
=======
	dev->netdev_ops		= &reg_vif_netdev_ops;
	dev->needs_free_netdev	= true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dev->features		|= NETIF_F_NETNS_LOCAL;
}

static struct net_device *ipmr_reg_vif(struct net *net, struct mr_table *mrt)
{
	struct net_device *dev;
<<<<<<< HEAD
	struct in_device *in_dev;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	char name[IFNAMSIZ];

	if (mrt->id == RT_TABLE_DEFAULT)
		sprintf(name, "pimreg");
	else
		sprintf(name, "pimreg%u", mrt->id);

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

	rcu_read_lock();
	in_dev = __in_dev_get_rcu(dev);
	if (!in_dev) {
		rcu_read_unlock();
		goto failure;
	}

	ipv4_devconf_setall(in_dev);
	IPV4_DEVCONF(in_dev->cnf, RP_FILTER) = 0;
	rcu_read_unlock();

	if (dev_open(dev))
=======

	if (!ipmr_init_vif_indev(dev))
		goto failure;
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

	unregister_netdevice(dev);
	return NULL;
}
#endif

/*
 *	Delete a VIF entry
 *	@notify: Set to 1, if the caller is a notifier_call
 */

static int vif_delete(struct mr_table *mrt, int vifi, int notify,
		      struct list_head *head)
{
=======
	unregister_netdevice(dev);
	return NULL;
}

/* called with rcu_read_lock() */
static int __pim_rcv(struct mr_table *mrt, struct sk_buff *skb,
		     unsigned int pimlen)
{
	struct net_device *reg_dev = NULL;
	struct iphdr *encap;
	int vif_num;

	encap = (struct iphdr *)(skb_transport_header(skb) + pimlen);
	/* Check that:
	 * a. packet is really sent to a multicast group
	 * b. packet is not a NULL-REGISTER
	 * c. packet is not truncated
	 */
	if (!ipv4_is_multicast(encap->daddr) ||
	    encap->tot_len == 0 ||
	    ntohs(encap->tot_len) + pimlen > skb->len)
		return 1;

	/* Pairs with WRITE_ONCE() in vif_add()/vid_delete() */
	vif_num = READ_ONCE(mrt->mroute_reg_vif_num);
	if (vif_num >= 0)
		reg_dev = vif_dev_read(&mrt->vif_table[vif_num]);
	if (!reg_dev)
		return 1;

	skb->mac_header = skb->network_header;
	skb_pull(skb, (u8 *)encap - skb->data);
	skb_reset_network_header(skb);
	skb->protocol = htons(ETH_P_IP);
	skb->ip_summed = CHECKSUM_NONE;

	skb_tunnel_rx(skb, reg_dev, dev_net(reg_dev));

	netif_rx(skb);

	return NET_RX_SUCCESS;
}
#else
static struct net_device *ipmr_reg_vif(struct net *net, struct mr_table *mrt)
{
	return NULL;
}
#endif

static int call_ipmr_vif_entry_notifiers(struct net *net,
					 enum fib_event_type event_type,
					 struct vif_device *vif,
					 struct net_device *vif_dev,
					 vifi_t vif_index, u32 tb_id)
{
	return mr_call_vif_notifiers(net, RTNL_FAMILY_IPMR, event_type,
				     vif, vif_dev, vif_index, tb_id,
				     &net->ipv4.ipmr_seq);
}

static int call_ipmr_mfc_entry_notifiers(struct net *net,
					 enum fib_event_type event_type,
					 struct mfc_cache *mfc, u32 tb_id)
{
	return mr_call_mfc_notifiers(net, RTNL_FAMILY_IPMR, event_type,
				     &mfc->_c, tb_id, &net->ipv4.ipmr_seq);
}

/**
 *	vif_delete - Delete a VIF entry
 *	@mrt: Table to delete from
 *	@vifi: VIF identifier to delete
 *	@notify: Set to 1, if the caller is a notifier_call
 *	@head: if unregistering the VIF, place it on this queue
 */
static int vif_delete(struct mr_table *mrt, int vifi, int notify,
		      struct list_head *head)
{
	struct net *net = read_pnet(&mrt->net);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct vif_device *v;
	struct net_device *dev;
	struct in_device *in_dev;

	if (vifi < 0 || vifi >= mrt->maxvif)
		return -EADDRNOTAVAIL;

	v = &mrt->vif_table[vifi];

<<<<<<< HEAD
	write_lock_bh(&mrt_lock);
	dev = v->dev;
	v->dev = NULL;

	if (!dev) {
		write_unlock_bh(&mrt_lock);
		return -EADDRNOTAVAIL;
	}

#ifdef CONFIG_IP_PIMSM
	if (vifi == mrt->mroute_reg_vif_num)
		mrt->mroute_reg_vif_num = -1;
#endif

=======
	dev = rtnl_dereference(v->dev);
	if (!dev)
		return -EADDRNOTAVAIL;

	spin_lock(&mrt_lock);
	call_ipmr_vif_entry_notifiers(net, FIB_EVENT_VIF_DEL, v, dev,
				      vifi, mrt->id);
	RCU_INIT_POINTER(v->dev, NULL);

	if (vifi == mrt->mroute_reg_vif_num) {
		/* Pairs with READ_ONCE() in ipmr_cache_report() and reg_vif_xmit() */
		WRITE_ONCE(mrt->mroute_reg_vif_num, -1);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (vifi + 1 == mrt->maxvif) {
		int tmp;

		for (tmp = vifi - 1; tmp >= 0; tmp--) {
			if (VIF_EXISTS(mrt, tmp))
				break;
		}
<<<<<<< HEAD
		mrt->maxvif = tmp+1;
	}

	write_unlock_bh(&mrt_lock);
=======
		WRITE_ONCE(mrt->maxvif, tmp + 1);
	}

	spin_unlock(&mrt_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	dev_set_allmulti(dev, -1);

	in_dev = __in_dev_get_rtnl(dev);
	if (in_dev) {
		IPV4_DEVCONF(in_dev->cnf, MC_FORWARDING)--;
<<<<<<< HEAD
=======
		inet_netconf_notify_devconf(dev_net(dev), RTM_NEWNETCONF,
					    NETCONFA_MC_FORWARDING,
					    dev->ifindex, &in_dev->cnf);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ip_rt_multicast_event(in_dev);
	}

	if (v->flags & (VIFF_TUNNEL | VIFF_REGISTER) && !notify)
		unregister_netdevice_queue(dev, head);

<<<<<<< HEAD
	dev_put(dev);
=======
	netdev_put(dev, &v->dev_tracker);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static void ipmr_cache_free_rcu(struct rcu_head *head)
{
<<<<<<< HEAD
	struct mfc_cache *c = container_of(head, struct mfc_cache, rcu);

	kmem_cache_free(mrt_cachep, c);
}

static inline void ipmr_cache_free(struct mfc_cache *c)
{
	call_rcu(&c->rcu, ipmr_cache_free_rcu);
=======
	struct mr_mfc *c = container_of(head, struct mr_mfc, rcu);

	kmem_cache_free(mrt_cachep, (struct mfc_cache *)c);
}

static void ipmr_cache_free(struct mfc_cache *c)
{
	call_rcu(&c->_c.rcu, ipmr_cache_free_rcu);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Destroy an unresolved cache entry, killing queued skbs
 * and reporting error to netlink readers.
 */
<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void ipmr_destroy_unres(struct mr_table *mrt, struct mfc_cache *c)
{
	struct net *net = read_pnet(&mrt->net);
	struct sk_buff *skb;
	struct nlmsgerr *e;

	atomic_dec(&mrt->cache_resolve_queue_len);

<<<<<<< HEAD
	while ((skb = skb_dequeue(&c->mfc_un.unres.unresolved))) {
		if (ip_hdr(skb)->version == 0) {
			struct nlmsghdr *nlh = (struct nlmsghdr *)skb_pull(skb, sizeof(struct iphdr));
			nlh->nlmsg_type = NLMSG_ERROR;
			nlh->nlmsg_len = NLMSG_LENGTH(sizeof(struct nlmsgerr));
			skb_trim(skb, nlh->nlmsg_len);
			e = NLMSG_DATA(nlh);
			e->error = -ETIMEDOUT;
			memset(&e->msg, 0, sizeof(e->msg));

			rtnl_unicast(skb, net, NETLINK_CB(skb).pid);
=======
	while ((skb = skb_dequeue(&c->_c.mfc_un.unres.unresolved))) {
		if (ip_hdr(skb)->version == 0) {
			struct nlmsghdr *nlh = skb_pull(skb,
							sizeof(struct iphdr));
			nlh->nlmsg_type = NLMSG_ERROR;
			nlh->nlmsg_len = nlmsg_msg_size(sizeof(struct nlmsgerr));
			skb_trim(skb, nlh->nlmsg_len);
			e = nlmsg_data(nlh);
			e->error = -ETIMEDOUT;
			memset(&e->msg, 0, sizeof(e->msg));

			rtnl_unicast(skb, net, NETLINK_CB(skb).portid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		} else {
			kfree_skb(skb);
		}
	}

	ipmr_cache_free(c);
}

<<<<<<< HEAD

/* Timer process for the unresolved queue. */

static void ipmr_expire_process(unsigned long arg)
{
	struct mr_table *mrt = (struct mr_table *)arg;
	unsigned long now;
	unsigned long expires;
	struct mfc_cache *c, *next;
=======
/* Timer process for the unresolved queue. */
static void ipmr_expire_process(struct timer_list *t)
{
	struct mr_table *mrt = from_timer(mrt, t, ipmr_expire_timer);
	struct mr_mfc *c, *next;
	unsigned long expires;
	unsigned long now;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!spin_trylock(&mfc_unres_lock)) {
		mod_timer(&mrt->ipmr_expire_timer, jiffies+HZ/10);
		return;
	}

	if (list_empty(&mrt->mfc_unres_queue))
		goto out;

	now = jiffies;
	expires = 10*HZ;

	list_for_each_entry_safe(c, next, &mrt->mfc_unres_queue, list) {
		if (time_after(c->mfc_un.unres.expires, now)) {
			unsigned long interval = c->mfc_un.unres.expires - now;
			if (interval < expires)
				expires = interval;
			continue;
		}

		list_del(&c->list);
<<<<<<< HEAD
		ipmr_destroy_unres(mrt, c);
=======
		mroute_netlink_event(mrt, (struct mfc_cache *)c, RTM_DELROUTE);
		ipmr_destroy_unres(mrt, (struct mfc_cache *)c);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (!list_empty(&mrt->mfc_unres_queue))
		mod_timer(&mrt->ipmr_expire_timer, jiffies + expires);

out:
	spin_unlock(&mfc_unres_lock);
}

<<<<<<< HEAD
/* Fill oifs list. It is called under write locked mrt_lock. */

static void ipmr_update_thresholds(struct mr_table *mrt, struct mfc_cache *cache,
=======
/* Fill oifs list. It is called under locked mrt_lock. */
static void ipmr_update_thresholds(struct mr_table *mrt, struct mr_mfc *cache,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				   unsigned char *ttls)
{
	int vifi;

	cache->mfc_un.res.minvif = MAXVIFS;
	cache->mfc_un.res.maxvif = 0;
	memset(cache->mfc_un.res.ttls, 255, MAXVIFS);

	for (vifi = 0; vifi < mrt->maxvif; vifi++) {
		if (VIF_EXISTS(mrt, vifi) &&
		    ttls[vifi] && ttls[vifi] < 255) {
			cache->mfc_un.res.ttls[vifi] = ttls[vifi];
			if (cache->mfc_un.res.minvif > vifi)
				cache->mfc_un.res.minvif = vifi;
			if (cache->mfc_un.res.maxvif <= vifi)
				cache->mfc_un.res.maxvif = vifi + 1;
		}
	}
<<<<<<< HEAD
=======
	cache->mfc_un.res.lastuse = jiffies;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int vif_add(struct net *net, struct mr_table *mrt,
		   struct vifctl *vifc, int mrtsock)
{
<<<<<<< HEAD
=======
	struct netdev_phys_item_id ppid = { };
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int vifi = vifc->vifc_vifi;
	struct vif_device *v = &mrt->vif_table[vifi];
	struct net_device *dev;
	struct in_device *in_dev;
	int err;

	/* Is vif busy ? */
	if (VIF_EXISTS(mrt, vifi))
		return -EADDRINUSE;

	switch (vifc->vifc_flags) {
<<<<<<< HEAD
#ifdef CONFIG_IP_PIMSM
	case VIFF_REGISTER:
		/*
		 * Special Purpose VIF in PIM
=======
	case VIFF_REGISTER:
		if (!ipmr_pimsm_enabled())
			return -EINVAL;
		/* Special Purpose VIF in PIM
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		 * All the packets will be sent to the daemon
		 */
		if (mrt->mroute_reg_vif_num >= 0)
			return -EADDRINUSE;
		dev = ipmr_reg_vif(net, mrt);
		if (!dev)
			return -ENOBUFS;
		err = dev_set_allmulti(dev, 1);
		if (err) {
			unregister_netdevice(dev);
			dev_put(dev);
			return err;
		}
		break;
<<<<<<< HEAD
#endif
	case VIFF_TUNNEL:
		dev = ipmr_new_tunnel(net, vifc);
		if (!dev)
			return -ENOBUFS;
		err = dev_set_allmulti(dev, 1);
		if (err) {
			ipmr_del_tunnel(dev, vifc);
			dev_put(dev);
			return err;
		}
		break;

=======
	case VIFF_TUNNEL:
		dev = ipmr_new_tunnel(net, vifc);
		if (IS_ERR(dev))
			return PTR_ERR(dev);
		break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case VIFF_USE_IFINDEX:
	case 0:
		if (vifc->vifc_flags == VIFF_USE_IFINDEX) {
			dev = dev_get_by_index(net, vifc->vifc_lcl_ifindex);
<<<<<<< HEAD
			if (dev && __in_dev_get_rtnl(dev) == NULL) {
=======
			if (dev && !__in_dev_get_rtnl(dev)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				dev_put(dev);
				return -EADDRNOTAVAIL;
			}
		} else {
			dev = ip_dev_find(net, vifc->vifc_lcl_addr.s_addr);
		}
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

	in_dev = __in_dev_get_rtnl(dev);
	if (!in_dev) {
		dev_put(dev);
		return -EADDRNOTAVAIL;
	}
	IPV4_DEVCONF(in_dev->cnf, MC_FORWARDING)++;
<<<<<<< HEAD
	ip_rt_multicast_event(in_dev);

	/* Fill in the VIF structures */

	v->rate_limit = vifc->vifc_rate_limit;
	v->local = vifc->vifc_lcl_addr.s_addr;
	v->remote = vifc->vifc_rmt_addr.s_addr;
	v->flags = vifc->vifc_flags;
	if (!mrtsock)
		v->flags |= VIFF_STATIC;
	v->threshold = vifc->vifc_threshold;
	v->bytes_in = 0;
	v->bytes_out = 0;
	v->pkt_in = 0;
	v->pkt_out = 0;
	v->link = dev->ifindex;
	if (v->flags & (VIFF_TUNNEL | VIFF_REGISTER))
		v->link = dev->iflink;

	/* And finish update writing critical data */
	write_lock_bh(&mrt_lock);
	v->dev = dev;
#ifdef CONFIG_IP_PIMSM
	if (v->flags & VIFF_REGISTER)
		mrt->mroute_reg_vif_num = vifi;
#endif
	if (vifi+1 > mrt->maxvif)
		mrt->maxvif = vifi+1;
	write_unlock_bh(&mrt_lock);
=======
	inet_netconf_notify_devconf(net, RTM_NEWNETCONF, NETCONFA_MC_FORWARDING,
				    dev->ifindex, &in_dev->cnf);
	ip_rt_multicast_event(in_dev);

	/* Fill in the VIF structures */
	vif_device_init(v, dev, vifc->vifc_rate_limit,
			vifc->vifc_threshold,
			vifc->vifc_flags | (!mrtsock ? VIFF_STATIC : 0),
			(VIFF_TUNNEL | VIFF_REGISTER));

	err = dev_get_port_parent_id(dev, &ppid, true);
	if (err == 0) {
		memcpy(v->dev_parent_id.id, ppid.id, ppid.id_len);
		v->dev_parent_id.id_len = ppid.id_len;
	} else {
		v->dev_parent_id.id_len = 0;
	}

	v->local = vifc->vifc_lcl_addr.s_addr;
	v->remote = vifc->vifc_rmt_addr.s_addr;

	/* And finish update writing critical data */
	spin_lock(&mrt_lock);
	rcu_assign_pointer(v->dev, dev);
	netdev_tracker_alloc(dev, &v->dev_tracker, GFP_ATOMIC);
	if (v->flags & VIFF_REGISTER) {
		/* Pairs with READ_ONCE() in ipmr_cache_report() and reg_vif_xmit() */
		WRITE_ONCE(mrt->mroute_reg_vif_num, vifi);
	}
	if (vifi+1 > mrt->maxvif)
		WRITE_ONCE(mrt->maxvif, vifi + 1);
	spin_unlock(&mrt_lock);
	call_ipmr_vif_entry_notifiers(net, FIB_EVENT_VIF_ADD, v, dev,
				      vifi, mrt->id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

/* called with rcu_read_lock() */
static struct mfc_cache *ipmr_cache_find(struct mr_table *mrt,
					 __be32 origin,
					 __be32 mcastgrp)
{
<<<<<<< HEAD
	int line = MFC_HASH(mcastgrp, origin);
	struct mfc_cache *c;

	list_for_each_entry_rcu(c, &mrt->mfc_cache_array[line], list) {
		if (c->mfc_origin == origin && c->mfc_mcastgrp == mcastgrp)
			return c;
	}
	return NULL;
}

/*
 *	Allocate a multicast cache entry
 */
=======
	struct mfc_cache_cmp_arg arg = {
			.mfc_mcastgrp = mcastgrp,
			.mfc_origin = origin
	};

	return mr_mfc_find(mrt, &arg);
}

/* Look for a (*,G) entry */
static struct mfc_cache *ipmr_cache_find_any(struct mr_table *mrt,
					     __be32 mcastgrp, int vifi)
{
	struct mfc_cache_cmp_arg arg = {
			.mfc_mcastgrp = mcastgrp,
			.mfc_origin = htonl(INADDR_ANY)
	};

	if (mcastgrp == htonl(INADDR_ANY))
		return mr_mfc_find_any_parent(mrt, vifi);
	return mr_mfc_find_any(mrt, vifi, &arg);
}

/* Look for a (S,G,iif) entry if parent != -1 */
static struct mfc_cache *ipmr_cache_find_parent(struct mr_table *mrt,
						__be32 origin, __be32 mcastgrp,
						int parent)
{
	struct mfc_cache_cmp_arg arg = {
			.mfc_mcastgrp = mcastgrp,
			.mfc_origin = origin,
	};

	return mr_mfc_find_parent(mrt, &arg, parent);
}

/* Allocate a multicast cache entry */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct mfc_cache *ipmr_cache_alloc(void)
{
	struct mfc_cache *c = kmem_cache_zalloc(mrt_cachep, GFP_KERNEL);

<<<<<<< HEAD
	if (c)
		c->mfc_un.res.minvif = MAXVIFS;
=======
	if (c) {
		c->_c.mfc_un.res.last_assert = jiffies - MFC_ASSERT_THRESH - 1;
		c->_c.mfc_un.res.minvif = MAXVIFS;
		c->_c.free = ipmr_cache_free_rcu;
		refcount_set(&c->_c.mfc_un.res.refcount, 1);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return c;
}

static struct mfc_cache *ipmr_cache_alloc_unres(void)
{
	struct mfc_cache *c = kmem_cache_zalloc(mrt_cachep, GFP_ATOMIC);

	if (c) {
<<<<<<< HEAD
		skb_queue_head_init(&c->mfc_un.unres.unresolved);
		c->mfc_un.unres.expires = jiffies + 10*HZ;
=======
		skb_queue_head_init(&c->_c.mfc_un.unres.unresolved);
		c->_c.mfc_un.unres.expires = jiffies + 10 * HZ;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return c;
}

<<<<<<< HEAD
/*
 *	A cache entry has gone into a resolved state from queued
 */

=======
/* A cache entry has gone into a resolved state from queued */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void ipmr_cache_resolve(struct net *net, struct mr_table *mrt,
			       struct mfc_cache *uc, struct mfc_cache *c)
{
	struct sk_buff *skb;
	struct nlmsgerr *e;

	/* Play the pending entries through our router */
<<<<<<< HEAD

	while ((skb = __skb_dequeue(&uc->mfc_un.unres.unresolved))) {
		if (ip_hdr(skb)->version == 0) {
			struct nlmsghdr *nlh = (struct nlmsghdr *)skb_pull(skb, sizeof(struct iphdr));

			if (__ipmr_fill_mroute(mrt, skb, c, NLMSG_DATA(nlh)) > 0) {
=======
	while ((skb = __skb_dequeue(&uc->_c.mfc_un.unres.unresolved))) {
		if (ip_hdr(skb)->version == 0) {
			struct nlmsghdr *nlh = skb_pull(skb,
							sizeof(struct iphdr));

			if (mr_fill_mroute(mrt, skb, &c->_c,
					   nlmsg_data(nlh)) > 0) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				nlh->nlmsg_len = skb_tail_pointer(skb) -
						 (u8 *)nlh;
			} else {
				nlh->nlmsg_type = NLMSG_ERROR;
<<<<<<< HEAD
				nlh->nlmsg_len = NLMSG_LENGTH(sizeof(struct nlmsgerr));
				skb_trim(skb, nlh->nlmsg_len);
				e = NLMSG_DATA(nlh);
=======
				nlh->nlmsg_len = nlmsg_msg_size(sizeof(struct nlmsgerr));
				skb_trim(skb, nlh->nlmsg_len);
				e = nlmsg_data(nlh);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				e->error = -EMSGSIZE;
				memset(&e->msg, 0, sizeof(e->msg));
			}

<<<<<<< HEAD
			rtnl_unicast(skb, net, NETLINK_CB(skb).pid);
		} else {
			ip_mr_forward(net, mrt, skb, c, 0);
=======
			rtnl_unicast(skb, net, NETLINK_CB(skb).portid);
		} else {
			rcu_read_lock();
			ip_mr_forward(net, mrt, skb->dev, skb, c, 0);
			rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}
}

<<<<<<< HEAD
/*
 *	Bounce a cache query up to mrouted. We could use netlink for this but mrouted
 *	expects the following bizarre scheme.
 *
 *	Called under mrt_lock.
 */

static int ipmr_cache_report(struct mr_table *mrt,
			     struct sk_buff *pkt, vifi_t vifi, int assert)
{
	struct sk_buff *skb;
	const int ihl = ip_hdrlen(pkt);
	struct igmphdr *igmp;
	struct igmpmsg *msg;
	struct sock *mroute_sk;
	int ret;

#ifdef CONFIG_IP_PIMSM
	if (assert == IGMPMSG_WHOLEPKT)
		skb = skb_realloc_headroom(pkt, sizeof(struct iphdr));
	else
#endif
=======
/* Bounce a cache query up to mrouted and netlink.
 *
 * Called under rcu_read_lock().
 */
static int ipmr_cache_report(const struct mr_table *mrt,
			     struct sk_buff *pkt, vifi_t vifi, int assert)
{
	const int ihl = ip_hdrlen(pkt);
	struct sock *mroute_sk;
	struct igmphdr *igmp;
	struct igmpmsg *msg;
	struct sk_buff *skb;
	int ret;

	mroute_sk = rcu_dereference(mrt->mroute_sk);
	if (!mroute_sk)
		return -EINVAL;

	if (assert == IGMPMSG_WHOLEPKT || assert == IGMPMSG_WRVIFWHOLE)
		skb = skb_realloc_headroom(pkt, sizeof(struct iphdr));
	else
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		skb = alloc_skb(128, GFP_ATOMIC);

	if (!skb)
		return -ENOBUFS;

<<<<<<< HEAD
#ifdef CONFIG_IP_PIMSM
	if (assert == IGMPMSG_WHOLEPKT) {
=======
	if (assert == IGMPMSG_WHOLEPKT || assert == IGMPMSG_WRVIFWHOLE) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* Ugly, but we have no choice with this interface.
		 * Duplicate old header, fix ihl, length etc.
		 * And all this only to mangle msg->im_msgtype and
		 * to set msg->im_mbz to "mbz" :-)
		 */
		skb_push(skb, sizeof(struct iphdr));
		skb_reset_network_header(skb);
		skb_reset_transport_header(skb);
		msg = (struct igmpmsg *)skb_network_header(skb);
		memcpy(msg, skb_network_header(pkt), sizeof(struct iphdr));
<<<<<<< HEAD
		msg->im_msgtype = IGMPMSG_WHOLEPKT;
		msg->im_mbz = 0;
		msg->im_vif = mrt->mroute_reg_vif_num;
		ip_hdr(skb)->ihl = sizeof(struct iphdr) >> 2;
		ip_hdr(skb)->tot_len = htons(ntohs(ip_hdr(pkt)->tot_len) +
					     sizeof(struct iphdr));
	} else
#endif
	{

	/* Copy the IP header */

	skb->network_header = skb->tail;
	skb_put(skb, ihl);
	skb_copy_to_linear_data(skb, pkt->data, ihl);
	ip_hdr(skb)->protocol = 0;	/* Flag to the kernel this is a route add */
	msg = (struct igmpmsg *)skb_network_header(skb);
	msg->im_vif = vifi;
	skb_dst_set(skb, dst_clone(skb_dst(pkt)));

	/* Add our header */

	igmp = (struct igmphdr *)skb_put(skb, sizeof(struct igmphdr));
	igmp->type	=
	msg->im_msgtype = assert;
	igmp->code	= 0;
	ip_hdr(skb)->tot_len = htons(skb->len);		/* Fix the length */
	skb->transport_header = skb->network_header;
	}

	rcu_read_lock();
	mroute_sk = rcu_dereference(mrt->mroute_sk);
	if (mroute_sk == NULL) {
		rcu_read_unlock();
		kfree_skb(skb);
		return -EINVAL;
	}

	/* Deliver to mrouted */

	ret = sock_queue_rcv_skb(mroute_sk, skb);
	rcu_read_unlock();
	if (ret < 0) {
		if (net_ratelimit())
			pr_warn("mroute: pending queue full, dropping entries\n");
=======
		msg->im_msgtype = assert;
		msg->im_mbz = 0;
		if (assert == IGMPMSG_WRVIFWHOLE) {
			msg->im_vif = vifi;
			msg->im_vif_hi = vifi >> 8;
		} else {
			/* Pairs with WRITE_ONCE() in vif_add() and vif_delete() */
			int vif_num = READ_ONCE(mrt->mroute_reg_vif_num);

			msg->im_vif = vif_num;
			msg->im_vif_hi = vif_num >> 8;
		}
		ip_hdr(skb)->ihl = sizeof(struct iphdr) >> 2;
		ip_hdr(skb)->tot_len = htons(ntohs(ip_hdr(pkt)->tot_len) +
					     sizeof(struct iphdr));
	} else {
		/* Copy the IP header */
		skb_set_network_header(skb, skb->len);
		skb_put(skb, ihl);
		skb_copy_to_linear_data(skb, pkt->data, ihl);
		/* Flag to the kernel this is a route add */
		ip_hdr(skb)->protocol = 0;
		msg = (struct igmpmsg *)skb_network_header(skb);
		msg->im_vif = vifi;
		msg->im_vif_hi = vifi >> 8;
		ipv4_pktinfo_prepare(mroute_sk, pkt, false);
		memcpy(skb->cb, pkt->cb, sizeof(skb->cb));
		/* Add our header */
		igmp = skb_put(skb, sizeof(struct igmphdr));
		igmp->type = assert;
		msg->im_msgtype = assert;
		igmp->code = 0;
		ip_hdr(skb)->tot_len = htons(skb->len);	/* Fix the length */
		skb->transport_header = skb->network_header;
	}

	igmpmsg_netlink_event(mrt, skb);

	/* Deliver to mrouted */
	ret = sock_queue_rcv_skb(mroute_sk, skb);

	if (ret < 0) {
		net_warn_ratelimited("mroute: pending queue full, dropping entries\n");
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
ipmr_cache_unresolved(struct mr_table *mrt, vifi_t vifi, struct sk_buff *skb)
{
	bool found = false;
	int err;
	struct mfc_cache *c;
	const struct iphdr *iph = ip_hdr(skb);

	spin_lock_bh(&mfc_unres_lock);
	list_for_each_entry(c, &mrt->mfc_unres_queue, list) {
=======
/* Queue a packet for resolution. It gets locked cache entry! */
/* Called under rcu_read_lock() */
static int ipmr_cache_unresolved(struct mr_table *mrt, vifi_t vifi,
				 struct sk_buff *skb, struct net_device *dev)
{
	const struct iphdr *iph = ip_hdr(skb);
	struct mfc_cache *c;
	bool found = false;
	int err;

	spin_lock_bh(&mfc_unres_lock);
	list_for_each_entry(c, &mrt->mfc_unres_queue, _c.list) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (c->mfc_mcastgrp == iph->daddr &&
		    c->mfc_origin == iph->saddr) {
			found = true;
			break;
		}
	}

	if (!found) {
		/* Create a new entry if allowable */
<<<<<<< HEAD

		if (atomic_read(&mrt->cache_resolve_queue_len) >= 10 ||
		    (c = ipmr_cache_alloc_unres()) == NULL) {
=======
		c = ipmr_cache_alloc_unres();
		if (!c) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			spin_unlock_bh(&mfc_unres_lock);

			kfree_skb(skb);
			return -ENOBUFS;
		}

		/* Fill in the new cache entry */
<<<<<<< HEAD

		c->mfc_parent	= -1;
=======
		c->_c.mfc_parent = -1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		c->mfc_origin	= iph->saddr;
		c->mfc_mcastgrp	= iph->daddr;

		/* Reflect first query at mrouted. */
<<<<<<< HEAD

		err = ipmr_cache_report(mrt, skb, vifi, IGMPMSG_NOCACHE);
=======
		err = ipmr_cache_report(mrt, skb, vifi, IGMPMSG_NOCACHE);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (err < 0) {
			/* If the report failed throw the cache entry
			   out - Brad Parker
			 */
			spin_unlock_bh(&mfc_unres_lock);

			ipmr_cache_free(c);
			kfree_skb(skb);
			return err;
		}

		atomic_inc(&mrt->cache_resolve_queue_len);
<<<<<<< HEAD
		list_add(&c->list, &mrt->mfc_unres_queue);

		if (atomic_read(&mrt->cache_resolve_queue_len) == 1)
			mod_timer(&mrt->ipmr_expire_timer, c->mfc_un.unres.expires);
	}

	/* See if we can append the packet */

	if (c->mfc_un.unres.unresolved.qlen > 3) {
		kfree_skb(skb);
		err = -ENOBUFS;
	} else {
		skb_queue_tail(&c->mfc_un.unres.unresolved, skb);
=======
		list_add(&c->_c.list, &mrt->mfc_unres_queue);
		mroute_netlink_event(mrt, c, RTM_NEWROUTE);

		if (atomic_read(&mrt->cache_resolve_queue_len) == 1)
			mod_timer(&mrt->ipmr_expire_timer,
				  c->_c.mfc_un.unres.expires);
	}

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

<<<<<<< HEAD
/*
 *	MFC cache manipulation by user space mroute daemon
 */

static int ipmr_mfc_delete(struct mr_table *mrt, struct mfcctl *mfc)
{
	int line;
	struct mfc_cache *c, *next;

	line = MFC_HASH(mfc->mfcc_mcastgrp.s_addr, mfc->mfcc_origin.s_addr);

	list_for_each_entry_safe(c, next, &mrt->mfc_cache_array[line], list) {
		if (c->mfc_origin == mfc->mfcc_origin.s_addr &&
		    c->mfc_mcastgrp == mfc->mfcc_mcastgrp.s_addr) {
			list_del_rcu(&c->list);

			ipmr_cache_free(c);
			return 0;
		}
	}
	return -ENOENT;
}

static int ipmr_mfc_add(struct net *net, struct mr_table *mrt,
			struct mfcctl *mfc, int mrtsock)
{
	bool found = false;
	int line;
	struct mfc_cache *uc, *c;
=======
/* MFC cache manipulation by user space mroute daemon */

static int ipmr_mfc_delete(struct mr_table *mrt, struct mfcctl *mfc, int parent)
{
	struct net *net = read_pnet(&mrt->net);
	struct mfc_cache *c;

	/* The entries are added/deleted only under RTNL */
	rcu_read_lock();
	c = ipmr_cache_find_parent(mrt, mfc->mfcc_origin.s_addr,
				   mfc->mfcc_mcastgrp.s_addr, parent);
	rcu_read_unlock();
	if (!c)
		return -ENOENT;
	rhltable_remove(&mrt->mfc_hash, &c->_c.mnode, ipmr_rht_params);
	list_del_rcu(&c->_c.list);
	call_ipmr_mfc_entry_notifiers(net, FIB_EVENT_ENTRY_DEL, c, mrt->id);
	mroute_netlink_event(mrt, c, RTM_DELROUTE);
	mr_cache_put(&c->_c);

	return 0;
}

static int ipmr_mfc_add(struct net *net, struct mr_table *mrt,
			struct mfcctl *mfc, int mrtsock, int parent)
{
	struct mfc_cache *uc, *c;
	struct mr_mfc *_uc;
	bool found;
	int ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (mfc->mfcc_parent >= MAXVIFS)
		return -ENFILE;

<<<<<<< HEAD
	line = MFC_HASH(mfc->mfcc_mcastgrp.s_addr, mfc->mfcc_origin.s_addr);

	list_for_each_entry(c, &mrt->mfc_cache_array[line], list) {
		if (c->mfc_origin == mfc->mfcc_origin.s_addr &&
		    c->mfc_mcastgrp == mfc->mfcc_mcastgrp.s_addr) {
			found = true;
			break;
		}
	}

	if (found) {
		write_lock_bh(&mrt_lock);
		c->mfc_parent = mfc->mfcc_parent;
		ipmr_update_thresholds(mrt, c, mfc->mfcc_ttls);
		if (!mrtsock)
			c->mfc_flags |= MFC_STATIC;
		write_unlock_bh(&mrt_lock);
		return 0;
	}

	if (!ipv4_is_multicast(mfc->mfcc_mcastgrp.s_addr))
		return -EINVAL;

	c = ipmr_cache_alloc();
	if (c == NULL)
=======
	/* The entries are added/deleted only under RTNL */
	rcu_read_lock();
	c = ipmr_cache_find_parent(mrt, mfc->mfcc_origin.s_addr,
				   mfc->mfcc_mcastgrp.s_addr, parent);
	rcu_read_unlock();
	if (c) {
		spin_lock(&mrt_lock);
		c->_c.mfc_parent = mfc->mfcc_parent;
		ipmr_update_thresholds(mrt, &c->_c, mfc->mfcc_ttls);
		if (!mrtsock)
			c->_c.mfc_flags |= MFC_STATIC;
		spin_unlock(&mrt_lock);
		call_ipmr_mfc_entry_notifiers(net, FIB_EVENT_ENTRY_REPLACE, c,
					      mrt->id);
		mroute_netlink_event(mrt, c, RTM_NEWROUTE);
		return 0;
	}

	if (mfc->mfcc_mcastgrp.s_addr != htonl(INADDR_ANY) &&
	    !ipv4_is_multicast(mfc->mfcc_mcastgrp.s_addr))
		return -EINVAL;

	c = ipmr_cache_alloc();
	if (!c)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ENOMEM;

	c->mfc_origin = mfc->mfcc_origin.s_addr;
	c->mfc_mcastgrp = mfc->mfcc_mcastgrp.s_addr;
<<<<<<< HEAD
	c->mfc_parent = mfc->mfcc_parent;
	ipmr_update_thresholds(mrt, c, mfc->mfcc_ttls);
	if (!mrtsock)
		c->mfc_flags |= MFC_STATIC;

	list_add_rcu(&c->list, &mrt->mfc_cache_array[line]);

	/*
	 *	Check to see if we resolved a queued list. If so we
	 *	need to send on the frames and tidy up.
	 */
	found = false;
	spin_lock_bh(&mfc_unres_lock);
	list_for_each_entry(uc, &mrt->mfc_unres_queue, list) {
		if (uc->mfc_origin == c->mfc_origin &&
		    uc->mfc_mcastgrp == c->mfc_mcastgrp) {
			list_del(&uc->list);
=======
	c->_c.mfc_parent = mfc->mfcc_parent;
	ipmr_update_thresholds(mrt, &c->_c, mfc->mfcc_ttls);
	if (!mrtsock)
		c->_c.mfc_flags |= MFC_STATIC;

	ret = rhltable_insert_key(&mrt->mfc_hash, &c->cmparg, &c->_c.mnode,
				  ipmr_rht_params);
	if (ret) {
		pr_err("ipmr: rhtable insert error %d\n", ret);
		ipmr_cache_free(c);
		return ret;
	}
	list_add_tail_rcu(&c->_c.list, &mrt->mfc_cache_list);
	/* Check to see if we resolved a queued list. If so we
	 * need to send on the frames and tidy up.
	 */
	found = false;
	spin_lock_bh(&mfc_unres_lock);
	list_for_each_entry(_uc, &mrt->mfc_unres_queue, list) {
		uc = (struct mfc_cache *)_uc;
		if (uc->mfc_origin == c->mfc_origin &&
		    uc->mfc_mcastgrp == c->mfc_mcastgrp) {
			list_del(&_uc->list);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			atomic_dec(&mrt->cache_resolve_queue_len);
			found = true;
			break;
		}
	}
	if (list_empty(&mrt->mfc_unres_queue))
		del_timer(&mrt->ipmr_expire_timer);
	spin_unlock_bh(&mfc_unres_lock);

	if (found) {
		ipmr_cache_resolve(net, mrt, uc, c);
		ipmr_cache_free(uc);
	}
<<<<<<< HEAD
	return 0;
}

/*
 *	Close the multicast socket, and clear the vif tables etc
 */

static void mroute_clean_tables(struct mr_table *mrt)
{
	int i;
	LIST_HEAD(list);
	struct mfc_cache *c, *next;

	/* Shut down all active vif entries */

	for (i = 0; i < mrt->maxvif; i++) {
		if (!(mrt->vif_table[i].flags & VIFF_STATIC))
			vif_delete(mrt, i, 0, &list);
	}
	unregister_netdevice_many(&list);

	/* Wipe the cache */

	for (i = 0; i < MFC_LINES; i++) {
		list_for_each_entry_safe(c, next, &mrt->mfc_cache_array[i], list) {
			if (c->mfc_flags & MFC_STATIC)
				continue;
			list_del_rcu(&c->list);
			ipmr_cache_free(c);
		}
	}

	if (atomic_read(&mrt->cache_resolve_queue_len) != 0) {
		spin_lock_bh(&mfc_unres_lock);
		list_for_each_entry_safe(c, next, &mrt->mfc_unres_queue, list) {
			list_del(&c->list);
			ipmr_destroy_unres(mrt, c);
		}
		spin_unlock_bh(&mfc_unres_lock);
=======
	call_ipmr_mfc_entry_notifiers(net, FIB_EVENT_ENTRY_ADD, c, mrt->id);
	mroute_netlink_event(mrt, c, RTM_NEWROUTE);
	return 0;
}

/* Close the multicast socket, and clear the vif tables etc */
static void mroute_clean_tables(struct mr_table *mrt, int flags)
{
	struct net *net = read_pnet(&mrt->net);
	struct mr_mfc *c, *tmp;
	struct mfc_cache *cache;
	LIST_HEAD(list);
	int i;

	/* Shut down all active vif entries */
	if (flags & (MRT_FLUSH_VIFS | MRT_FLUSH_VIFS_STATIC)) {
		for (i = 0; i < mrt->maxvif; i++) {
			if (((mrt->vif_table[i].flags & VIFF_STATIC) &&
			     !(flags & MRT_FLUSH_VIFS_STATIC)) ||
			    (!(mrt->vif_table[i].flags & VIFF_STATIC) && !(flags & MRT_FLUSH_VIFS)))
				continue;
			vif_delete(mrt, i, 0, &list);
		}
		unregister_netdevice_many(&list);
	}

	/* Wipe the cache */
	if (flags & (MRT_FLUSH_MFC | MRT_FLUSH_MFC_STATIC)) {
		list_for_each_entry_safe(c, tmp, &mrt->mfc_cache_list, list) {
			if (((c->mfc_flags & MFC_STATIC) && !(flags & MRT_FLUSH_MFC_STATIC)) ||
			    (!(c->mfc_flags & MFC_STATIC) && !(flags & MRT_FLUSH_MFC)))
				continue;
			rhltable_remove(&mrt->mfc_hash, &c->mnode, ipmr_rht_params);
			list_del_rcu(&c->list);
			cache = (struct mfc_cache *)c;
			call_ipmr_mfc_entry_notifiers(net, FIB_EVENT_ENTRY_DEL, cache,
						      mrt->id);
			mroute_netlink_event(mrt, cache, RTM_DELROUTE);
			mr_cache_put(c);
		}
	}

	if (flags & MRT_FLUSH_MFC) {
		if (atomic_read(&mrt->cache_resolve_queue_len) != 0) {
			spin_lock_bh(&mfc_unres_lock);
			list_for_each_entry_safe(c, tmp, &mrt->mfc_unres_queue, list) {
				list_del(&c->list);
				cache = (struct mfc_cache *)c;
				mroute_netlink_event(mrt, cache, RTM_DELROUTE);
				ipmr_destroy_unres(mrt, cache);
			}
			spin_unlock_bh(&mfc_unres_lock);
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

/* called from ip_ra_control(), before an RCU grace period,
<<<<<<< HEAD
 * we dont need to call synchronize_rcu() here
=======
 * we don't need to call synchronize_rcu() here
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
static void mrtsock_destruct(struct sock *sk)
{
	struct net *net = sock_net(sk);
	struct mr_table *mrt;

	rtnl_lock();
	ipmr_for_each_table(mrt, net) {
		if (sk == rtnl_dereference(mrt->mroute_sk)) {
			IPV4_DEVCONF_ALL(net, MC_FORWARDING)--;
<<<<<<< HEAD
			RCU_INIT_POINTER(mrt->mroute_sk, NULL);
			mroute_clean_tables(mrt);
=======
			inet_netconf_notify_devconf(net, RTM_NEWNETCONF,
						    NETCONFA_MC_FORWARDING,
						    NETCONFA_IFINDEX_ALL,
						    net->ipv4.devconf_all);
			RCU_INIT_POINTER(mrt->mroute_sk, NULL);
			mroute_clean_tables(mrt, MRT_FLUSH_VIFS | MRT_FLUSH_MFC);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}
	rtnl_unlock();
}

<<<<<<< HEAD
/*
 *	Socket options and virtual interface manipulation. The whole
 *	virtual interface system is a complete heap, but unfortunately
 *	that's how BSD mrouted happens to think. Maybe one day with a proper
 *	MOSPF/PIM router set up we can clean this up.
 */

int ip_mroute_setsockopt(struct sock *sk, int optname, char __user *optval, unsigned int optlen)
{
	int ret;
	struct vifctl vif;
	struct mfcctl mfc;
	struct net *net = sock_net(sk);
	struct mr_table *mrt;

	mrt = ipmr_get_table(net, raw_sk(sk)->ipmr_table ? : RT_TABLE_DEFAULT);
	if (mrt == NULL)
		return -ENOENT;

	if (optname != MRT_INIT) {
		if (sk != rcu_access_pointer(mrt->mroute_sk) &&
		    !ns_capable(net->user_ns, CAP_NET_ADMIN))
			return -EACCES;
=======
/* Socket options and virtual interface manipulation. The whole
 * virtual interface system is a complete heap, but unfortunately
 * that's how BSD mrouted happens to think. Maybe one day with a proper
 * MOSPF/PIM router set up we can clean this up.
 */

int ip_mroute_setsockopt(struct sock *sk, int optname, sockptr_t optval,
			 unsigned int optlen)
{
	struct net *net = sock_net(sk);
	int val, ret = 0, parent = 0;
	struct mr_table *mrt;
	struct vifctl vif;
	struct mfcctl mfc;
	bool do_wrvifwhole;
	u32 uval;

	/* There's one exception to the lock - MRT_DONE which needs to unlock */
	rtnl_lock();
	if (sk->sk_type != SOCK_RAW ||
	    inet_sk(sk)->inet_num != IPPROTO_IGMP) {
		ret = -EOPNOTSUPP;
		goto out_unlock;
	}

	mrt = ipmr_get_table(net, raw_sk(sk)->ipmr_table ? : RT_TABLE_DEFAULT);
	if (!mrt) {
		ret = -ENOENT;
		goto out_unlock;
	}
	if (optname != MRT_INIT) {
		if (sk != rcu_access_pointer(mrt->mroute_sk) &&
		    !ns_capable(net->user_ns, CAP_NET_ADMIN)) {
			ret = -EACCES;
			goto out_unlock;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	switch (optname) {
	case MRT_INIT:
<<<<<<< HEAD
		if (sk->sk_type != SOCK_RAW ||
		    inet_sk(sk)->inet_num != IPPROTO_IGMP)
			return -EOPNOTSUPP;
		if (optlen != sizeof(int))
			return -ENOPROTOOPT;

		rtnl_lock();
		if (rtnl_dereference(mrt->mroute_sk)) {
			rtnl_unlock();
			return -EADDRINUSE;
=======
		if (optlen != sizeof(int)) {
			ret = -EINVAL;
			break;
		}
		if (rtnl_dereference(mrt->mroute_sk)) {
			ret = -EADDRINUSE;
			break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}

		ret = ip_ra_control(sk, 1, mrtsock_destruct);
		if (ret == 0) {
			rcu_assign_pointer(mrt->mroute_sk, sk);
			IPV4_DEVCONF_ALL(net, MC_FORWARDING)++;
<<<<<<< HEAD
		}
		rtnl_unlock();
		return ret;
	case MRT_DONE:
		if (sk != rcu_access_pointer(mrt->mroute_sk))
			return -EACCES;
		return ip_ra_control(sk, 0, NULL);
	case MRT_ADD_VIF:
	case MRT_DEL_VIF:
		if (optlen != sizeof(vif))
			return -EINVAL;
		if (copy_from_user(&vif, optval, sizeof(vif)))
			return -EFAULT;
		if (vif.vifc_vifi >= MAXVIFS)
			return -ENFILE;
		rtnl_lock();
=======
			inet_netconf_notify_devconf(net, RTM_NEWNETCONF,
						    NETCONFA_MC_FORWARDING,
						    NETCONFA_IFINDEX_ALL,
						    net->ipv4.devconf_all);
		}
		break;
	case MRT_DONE:
		if (sk != rcu_access_pointer(mrt->mroute_sk)) {
			ret = -EACCES;
		} else {
			/* We need to unlock here because mrtsock_destruct takes
			 * care of rtnl itself and we can't change that due to
			 * the IP_ROUTER_ALERT setsockopt which runs without it.
			 */
			rtnl_unlock();
			ret = ip_ra_control(sk, 0, NULL);
			goto out;
		}
		break;
	case MRT_ADD_VIF:
	case MRT_DEL_VIF:
		if (optlen != sizeof(vif)) {
			ret = -EINVAL;
			break;
		}
		if (copy_from_sockptr(&vif, optval, sizeof(vif))) {
			ret = -EFAULT;
			break;
		}
		if (vif.vifc_vifi >= MAXVIFS) {
			ret = -ENFILE;
			break;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (optname == MRT_ADD_VIF) {
			ret = vif_add(net, mrt, &vif,
				      sk == rtnl_dereference(mrt->mroute_sk));
		} else {
			ret = vif_delete(mrt, vif.vifc_vifi, 0, NULL);
		}
<<<<<<< HEAD
		rtnl_unlock();
		return ret;

		/*
		 *	Manipulate the forwarding caches. These live
		 *	in a sort of kernel/user symbiosis.
		 */
	case MRT_ADD_MFC:
	case MRT_DEL_MFC:
		if (optlen != sizeof(mfc))
			return -EINVAL;
		if (copy_from_user(&mfc, optval, sizeof(mfc)))
			return -EFAULT;
		rtnl_lock();
		if (optname == MRT_DEL_MFC)
			ret = ipmr_mfc_delete(mrt, &mfc);
		else
			ret = ipmr_mfc_add(net, mrt, &mfc,
					   sk == rtnl_dereference(mrt->mroute_sk));
		rtnl_unlock();
		return ret;
		/*
		 *	Control PIM assert.
		 */
	case MRT_ASSERT:
	{
		int v;
		if (get_user(v, (int __user *)optval))
			return -EFAULT;
		mrt->mroute_do_assert = (v) ? 1 : 0;
		return 0;
	}
#ifdef CONFIG_IP_PIMSM
	case MRT_PIM:
	{
		int v;

		if (get_user(v, (int __user *)optval))
			return -EFAULT;
		v = (v) ? 1 : 0;

		rtnl_lock();
		ret = 0;
		if (v != mrt->mroute_do_pim) {
			mrt->mroute_do_pim = v;
			mrt->mroute_do_assert = v;
		}
		rtnl_unlock();
		return ret;
	}
#endif
#ifdef CONFIG_IP_MROUTE_MULTIPLE_TABLES
	case MRT_TABLE:
	{
		u32 v;

		if (optlen != sizeof(u32))
			return -EINVAL;
		if (get_user(v, (u32 __user *)optval))
			return -EFAULT;

		rtnl_lock();
		ret = 0;
		if (sk == rtnl_dereference(mrt->mroute_sk)) {
			ret = -EBUSY;
		} else {
			if (!ipmr_new_table(net, v))
				ret = -ENOMEM;
			raw_sk(sk)->ipmr_table = v;
		}
		rtnl_unlock();
		return ret;
	}
#endif
	/*
	 *	Spurious command, or MRT_VERSION which you cannot
	 *	set.
	 */
	default:
		return -ENOPROTOOPT;
	}
}

/*
 *	Getsock opt support for the multicast routing system.
 */

int ip_mroute_getsockopt(struct sock *sk, int optname, char __user *optval, int __user *optlen)
=======
		break;
	/* Manipulate the forwarding caches. These live
	 * in a sort of kernel/user symbiosis.
	 */
	case MRT_ADD_MFC:
	case MRT_DEL_MFC:
		parent = -1;
		fallthrough;
	case MRT_ADD_MFC_PROXY:
	case MRT_DEL_MFC_PROXY:
		if (optlen != sizeof(mfc)) {
			ret = -EINVAL;
			break;
		}
		if (copy_from_sockptr(&mfc, optval, sizeof(mfc))) {
			ret = -EFAULT;
			break;
		}
		if (parent == 0)
			parent = mfc.mfcc_parent;
		if (optname == MRT_DEL_MFC || optname == MRT_DEL_MFC_PROXY)
			ret = ipmr_mfc_delete(mrt, &mfc, parent);
		else
			ret = ipmr_mfc_add(net, mrt, &mfc,
					   sk == rtnl_dereference(mrt->mroute_sk),
					   parent);
		break;
	case MRT_FLUSH:
		if (optlen != sizeof(val)) {
			ret = -EINVAL;
			break;
		}
		if (copy_from_sockptr(&val, optval, sizeof(val))) {
			ret = -EFAULT;
			break;
		}
		mroute_clean_tables(mrt, val);
		break;
	/* Control PIM assert. */
	case MRT_ASSERT:
		if (optlen != sizeof(val)) {
			ret = -EINVAL;
			break;
		}
		if (copy_from_sockptr(&val, optval, sizeof(val))) {
			ret = -EFAULT;
			break;
		}
		mrt->mroute_do_assert = val;
		break;
	case MRT_PIM:
		if (!ipmr_pimsm_enabled()) {
			ret = -ENOPROTOOPT;
			break;
		}
		if (optlen != sizeof(val)) {
			ret = -EINVAL;
			break;
		}
		if (copy_from_sockptr(&val, optval, sizeof(val))) {
			ret = -EFAULT;
			break;
		}

		do_wrvifwhole = (val == IGMPMSG_WRVIFWHOLE);
		val = !!val;
		if (val != mrt->mroute_do_pim) {
			mrt->mroute_do_pim = val;
			mrt->mroute_do_assert = val;
			mrt->mroute_do_wrvifwhole = do_wrvifwhole;
		}
		break;
	case MRT_TABLE:
		if (!IS_BUILTIN(CONFIG_IP_MROUTE_MULTIPLE_TABLES)) {
			ret = -ENOPROTOOPT;
			break;
		}
		if (optlen != sizeof(uval)) {
			ret = -EINVAL;
			break;
		}
		if (copy_from_sockptr(&uval, optval, sizeof(uval))) {
			ret = -EFAULT;
			break;
		}

		if (sk == rtnl_dereference(mrt->mroute_sk)) {
			ret = -EBUSY;
		} else {
			mrt = ipmr_new_table(net, uval);
			if (IS_ERR(mrt))
				ret = PTR_ERR(mrt);
			else
				raw_sk(sk)->ipmr_table = uval;
		}
		break;
	/* Spurious command, or MRT_VERSION which you cannot set. */
	default:
		ret = -ENOPROTOOPT;
	}
out_unlock:
	rtnl_unlock();
out:
	return ret;
}

/* Execute if this ioctl is a special mroute ioctl */
int ipmr_sk_ioctl(struct sock *sk, unsigned int cmd, void __user *arg)
{
	switch (cmd) {
	/* These userspace buffers will be consumed by ipmr_ioctl() */
	case SIOCGETVIFCNT: {
		struct sioc_vif_req buffer;

		return sock_ioctl_inout(sk, cmd, arg, &buffer,
				      sizeof(buffer));
		}
	case SIOCGETSGCNT: {
		struct sioc_sg_req buffer;

		return sock_ioctl_inout(sk, cmd, arg, &buffer,
				      sizeof(buffer));
		}
	}
	/* return code > 0 means that the ioctl was not executed */
	return 1;
}

/* Getsock opt support for the multicast routing system. */
int ip_mroute_getsockopt(struct sock *sk, int optname, sockptr_t optval,
			 sockptr_t optlen)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int olr;
	int val;
	struct net *net = sock_net(sk);
	struct mr_table *mrt;

<<<<<<< HEAD
	mrt = ipmr_get_table(net, raw_sk(sk)->ipmr_table ? : RT_TABLE_DEFAULT);
	if (mrt == NULL)
		return -ENOENT;

	if (optname != MRT_VERSION &&
#ifdef CONFIG_IP_PIMSM
	   optname != MRT_PIM &&
#endif
	   optname != MRT_ASSERT)
		return -ENOPROTOOPT;

	if (get_user(olr, optlen))
		return -EFAULT;

	olr = min_t(unsigned int, olr, sizeof(int));
	if (olr < 0)
		return -EINVAL;

	if (put_user(olr, optlen))
		return -EFAULT;
	if (optname == MRT_VERSION)
		val = 0x0305;
#ifdef CONFIG_IP_PIMSM
	else if (optname == MRT_PIM)
		val = mrt->mroute_do_pim;
#endif
	else
		val = mrt->mroute_do_assert;
	if (copy_to_user(optval, &val, olr))
=======
	if (sk->sk_type != SOCK_RAW ||
	    inet_sk(sk)->inet_num != IPPROTO_IGMP)
		return -EOPNOTSUPP;

	mrt = ipmr_get_table(net, raw_sk(sk)->ipmr_table ? : RT_TABLE_DEFAULT);
	if (!mrt)
		return -ENOENT;

	switch (optname) {
	case MRT_VERSION:
		val = 0x0305;
		break;
	case MRT_PIM:
		if (!ipmr_pimsm_enabled())
			return -ENOPROTOOPT;
		val = mrt->mroute_do_pim;
		break;
	case MRT_ASSERT:
		val = mrt->mroute_do_assert;
		break;
	default:
		return -ENOPROTOOPT;
	}

	if (copy_from_sockptr(&olr, optlen, sizeof(int)))
		return -EFAULT;
	if (olr < 0)
		return -EINVAL;

	olr = min_t(unsigned int, olr, sizeof(int));

	if (copy_to_sockptr(optlen, &olr, sizeof(int)))
		return -EFAULT;
	if (copy_to_sockptr(optval, &val, olr))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EFAULT;
	return 0;
}

<<<<<<< HEAD
/*
 *	The IP multicast ioctl support routines.
 */

int ipmr_ioctl(struct sock *sk, int cmd, void __user *arg)
{
	struct sioc_sg_req sr;
	struct sioc_vif_req vr;
	struct vif_device *vif;
	struct mfc_cache *c;
	struct net *net = sock_net(sk);
	struct mr_table *mrt;

	mrt = ipmr_get_table(net, raw_sk(sk)->ipmr_table ? : RT_TABLE_DEFAULT);
	if (mrt == NULL)
=======
/* The IP multicast ioctl support routines. */
int ipmr_ioctl(struct sock *sk, int cmd, void *arg)
{
	struct vif_device *vif;
	struct mfc_cache *c;
	struct net *net = sock_net(sk);
	struct sioc_vif_req *vr;
	struct sioc_sg_req *sr;
	struct mr_table *mrt;

	mrt = ipmr_get_table(net, raw_sk(sk)->ipmr_table ? : RT_TABLE_DEFAULT);
	if (!mrt)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ENOENT;

	switch (cmd) {
	case SIOCGETVIFCNT:
<<<<<<< HEAD
		if (copy_from_user(&vr, arg, sizeof(vr)))
			return -EFAULT;
		if (vr.vifi >= mrt->maxvif)
			return -EINVAL;
		read_lock(&mrt_lock);
		vif = &mrt->vif_table[vr.vifi];
		if (VIF_EXISTS(mrt, vr.vifi)) {
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
	case SIOCGETSGCNT:
		if (copy_from_user(&sr, arg, sizeof(sr)))
			return -EFAULT;

		rcu_read_lock();
		c = ipmr_cache_find(mrt, sr.src.s_addr, sr.grp.s_addr);
		if (c) {
			sr.pktcnt = c->mfc_un.res.pkt;
			sr.bytecnt = c->mfc_un.res.bytes;
			sr.wrong_if = c->mfc_un.res.wrong_if;
			rcu_read_unlock();

			if (copy_to_user(arg, &sr, sizeof(sr)))
				return -EFAULT;
=======
		vr = (struct sioc_vif_req *)arg;
		if (vr->vifi >= mrt->maxvif)
			return -EINVAL;
		vr->vifi = array_index_nospec(vr->vifi, mrt->maxvif);
		rcu_read_lock();
		vif = &mrt->vif_table[vr->vifi];
		if (VIF_EXISTS(mrt, vr->vifi)) {
			vr->icount = READ_ONCE(vif->pkt_in);
			vr->ocount = READ_ONCE(vif->pkt_out);
			vr->ibytes = READ_ONCE(vif->bytes_in);
			vr->obytes = READ_ONCE(vif->bytes_out);
			rcu_read_unlock();

			return 0;
		}
		rcu_read_unlock();
		return -EADDRNOTAVAIL;
	case SIOCGETSGCNT:
		sr = (struct sioc_sg_req *)arg;

		rcu_read_lock();
		c = ipmr_cache_find(mrt, sr->src.s_addr, sr->grp.s_addr);
		if (c) {
			sr->pktcnt = c->_c.mfc_un.res.pkt;
			sr->bytecnt = c->_c.mfc_un.res.bytes;
			sr->wrong_if = c->_c.mfc_un.res.wrong_if;
			rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return 0;
		}
		rcu_read_unlock();
		return -EADDRNOTAVAIL;
	default:
		return -ENOIOCTLCMD;
	}
}

#ifdef CONFIG_COMPAT
struct compat_sioc_sg_req {
	struct in_addr src;
	struct in_addr grp;
	compat_ulong_t pktcnt;
	compat_ulong_t bytecnt;
	compat_ulong_t wrong_if;
};

struct compat_sioc_vif_req {
	vifi_t	vifi;		/* Which iface */
	compat_ulong_t icount;
	compat_ulong_t ocount;
	compat_ulong_t ibytes;
	compat_ulong_t obytes;
};

int ipmr_compat_ioctl(struct sock *sk, unsigned int cmd, void __user *arg)
{
	struct compat_sioc_sg_req sr;
	struct compat_sioc_vif_req vr;
	struct vif_device *vif;
	struct mfc_cache *c;
	struct net *net = sock_net(sk);
	struct mr_table *mrt;

	mrt = ipmr_get_table(net, raw_sk(sk)->ipmr_table ? : RT_TABLE_DEFAULT);
<<<<<<< HEAD
	if (mrt == NULL)
=======
	if (!mrt)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ENOENT;

	switch (cmd) {
	case SIOCGETVIFCNT:
		if (copy_from_user(&vr, arg, sizeof(vr)))
			return -EFAULT;
		if (vr.vifi >= mrt->maxvif)
			return -EINVAL;
<<<<<<< HEAD
		read_lock(&mrt_lock);
		vif = &mrt->vif_table[vr.vifi];
		if (VIF_EXISTS(mrt, vr.vifi)) {
			vr.icount = vif->pkt_in;
			vr.ocount = vif->pkt_out;
			vr.ibytes = vif->bytes_in;
			vr.obytes = vif->bytes_out;
			read_unlock(&mrt_lock);
=======
		vr.vifi = array_index_nospec(vr.vifi, mrt->maxvif);
		rcu_read_lock();
		vif = &mrt->vif_table[vr.vifi];
		if (VIF_EXISTS(mrt, vr.vifi)) {
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
	case SIOCGETSGCNT:
		if (copy_from_user(&sr, arg, sizeof(sr)))
			return -EFAULT;

		rcu_read_lock();
		c = ipmr_cache_find(mrt, sr.src.s_addr, sr.grp.s_addr);
		if (c) {
<<<<<<< HEAD
			sr.pktcnt = c->mfc_un.res.pkt;
			sr.bytecnt = c->mfc_un.res.bytes;
			sr.wrong_if = c->mfc_un.res.wrong_if;
=======
			sr.pktcnt = c->_c.mfc_un.res.pkt;
			sr.bytecnt = c->_c.mfc_un.res.bytes;
			sr.wrong_if = c->_c.mfc_un.res.wrong_if;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			rcu_read_unlock();

			if (copy_to_user(arg, &sr, sizeof(sr)))
				return -EFAULT;
			return 0;
		}
		rcu_read_unlock();
		return -EADDRNOTAVAIL;
	default:
		return -ENOIOCTLCMD;
	}
}
#endif

<<<<<<< HEAD

static int ipmr_device_event(struct notifier_block *this, unsigned long event, void *ptr)
{
	struct net_device *dev = ptr;
=======
static int ipmr_device_event(struct notifier_block *this, unsigned long event, void *ptr)
{
	struct net_device *dev = netdev_notifier_info_to_dev(ptr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct net *net = dev_net(dev);
	struct mr_table *mrt;
	struct vif_device *v;
	int ct;

	if (event != NETDEV_UNREGISTER)
		return NOTIFY_DONE;

	ipmr_for_each_table(mrt, net) {
		v = &mrt->vif_table[0];
		for (ct = 0; ct < mrt->maxvif; ct++, v++) {
<<<<<<< HEAD
			if (v->dev == dev)
=======
			if (rcu_access_pointer(v->dev) == dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				vif_delete(mrt, ct, 1, NULL);
		}
	}
	return NOTIFY_DONE;
}

<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct notifier_block ip_mr_notifier = {
	.notifier_call = ipmr_device_event,
};

<<<<<<< HEAD
/*
 *	Encapsulate a packet by attaching a valid IPIP header to it.
 *	This avoids tunnel drivers and other mess and gives us the speed so
 *	important for multicast video.
 */

static void ip_encap(struct sk_buff *skb, __be32 saddr, __be32 daddr)
=======
/* Encapsulate a packet by attaching a valid IPIP header to it.
 * This avoids tunnel drivers and other mess and gives us the speed so
 * important for multicast video.
 */
static void ip_encap(struct net *net, struct sk_buff *skb,
		     __be32 saddr, __be32 daddr)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct iphdr *iph;
	const struct iphdr *old_iph = ip_hdr(skb);

	skb_push(skb, sizeof(struct iphdr));
	skb->transport_header = skb->network_header;
	skb_reset_network_header(skb);
	iph = ip_hdr(skb);

	iph->version	=	4;
	iph->tos	=	old_iph->tos;
	iph->ttl	=	old_iph->ttl;
	iph->frag_off	=	0;
	iph->daddr	=	daddr;
	iph->saddr	=	saddr;
	iph->protocol	=	IPPROTO_IPIP;
	iph->ihl	=	5;
	iph->tot_len	=	htons(skb->len);
<<<<<<< HEAD
	ip_select_ident(skb, NULL);
	ip_send_check(iph);

	memset(&(IPCB(skb)->opt), 0, sizeof(IPCB(skb)->opt));
	nf_reset(skb);
}

static inline int ipmr_forward_finish(struct sk_buff *skb)
{
	struct ip_options *opt = &(IPCB(skb)->opt);

	IP_INC_STATS_BH(dev_net(skb_dst(skb)->dev), IPSTATS_MIB_OUTFORWDATAGRAMS);
=======
	ip_select_ident(net, skb, NULL);
	ip_send_check(iph);

	memset(&(IPCB(skb)->opt), 0, sizeof(IPCB(skb)->opt));
	nf_reset_ct(skb);
}

static inline int ipmr_forward_finish(struct net *net, struct sock *sk,
				      struct sk_buff *skb)
{
	struct ip_options *opt = &(IPCB(skb)->opt);

	IP_INC_STATS(net, IPSTATS_MIB_OUTFORWDATAGRAMS);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (unlikely(opt->optlen))
		ip_forward_options(skb);

<<<<<<< HEAD
	return dst_output(skb);
}

/*
 *	Processing handlers for ipmr_forward
 */

static void ipmr_queue_xmit(struct net *net, struct mr_table *mrt,
			    struct sk_buff *skb, struct mfc_cache *c, int vifi)
{
	const struct iphdr *iph = ip_hdr(skb);
	struct vif_device *vif = &mrt->vif_table[vifi];
=======
	return dst_output(net, sk, skb);
}

#ifdef CONFIG_NET_SWITCHDEV
static bool ipmr_forward_offloaded(struct sk_buff *skb, struct mr_table *mrt,
				   int in_vifi, int out_vifi)
{
	struct vif_device *out_vif = &mrt->vif_table[out_vifi];
	struct vif_device *in_vif = &mrt->vif_table[in_vifi];

	if (!skb->offload_l3_fwd_mark)
		return false;
	if (!out_vif->dev_parent_id.id_len || !in_vif->dev_parent_id.id_len)
		return false;
	return netdev_phys_item_id_same(&out_vif->dev_parent_id,
					&in_vif->dev_parent_id);
}
#else
static bool ipmr_forward_offloaded(struct sk_buff *skb, struct mr_table *mrt,
				   int in_vifi, int out_vifi)
{
	return false;
}
#endif

/* Processing handlers for ipmr_forward, under rcu_read_lock() */

static void ipmr_queue_xmit(struct net *net, struct mr_table *mrt,
			    int in_vifi, struct sk_buff *skb, int vifi)
{
	const struct iphdr *iph = ip_hdr(skb);
	struct vif_device *vif = &mrt->vif_table[vifi];
	struct net_device *vif_dev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct net_device *dev;
	struct rtable *rt;
	struct flowi4 fl4;
	int    encap = 0;

<<<<<<< HEAD
	if (vif->dev == NULL)
		goto out_free;

#ifdef CONFIG_IP_PIMSM
	if (vif->flags & VIFF_REGISTER) {
		vif->pkt_out++;
		vif->bytes_out += skb->len;
		vif->dev->stats.tx_bytes += skb->len;
		vif->dev->stats.tx_packets++;
		ipmr_cache_report(mrt, skb, vifi, IGMPMSG_WHOLEPKT);
		goto out_free;
	}
#endif
=======
	vif_dev = vif_dev_read(vif);
	if (!vif_dev)
		goto out_free;

	if (vif->flags & VIFF_REGISTER) {
		WRITE_ONCE(vif->pkt_out, vif->pkt_out + 1);
		WRITE_ONCE(vif->bytes_out, vif->bytes_out + skb->len);
		DEV_STATS_ADD(vif_dev, tx_bytes, skb->len);
		DEV_STATS_INC(vif_dev, tx_packets);
		ipmr_cache_report(mrt, skb, vifi, IGMPMSG_WHOLEPKT);
		goto out_free;
	}

	if (ipmr_forward_offloaded(skb, mrt, in_vifi, vifi))
		goto out_free;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (vif->flags & VIFF_TUNNEL) {
		rt = ip_route_output_ports(net, &fl4, NULL,
					   vif->remote, vif->local,
					   0, 0,
					   IPPROTO_IPIP,
					   RT_TOS(iph->tos), vif->link);
		if (IS_ERR(rt))
			goto out_free;
		encap = sizeof(struct iphdr);
	} else {
		rt = ip_route_output_ports(net, &fl4, NULL, iph->daddr, 0,
					   0, 0,
					   IPPROTO_IPIP,
					   RT_TOS(iph->tos), vif->link);
		if (IS_ERR(rt))
			goto out_free;
	}

	dev = rt->dst.dev;

	if (skb->len+encap > dst_mtu(&rt->dst) && (ntohs(iph->frag_off) & IP_DF)) {
		/* Do not fragment multicasts. Alas, IPv4 does not
		 * allow to send ICMP, so that packets will disappear
		 * to blackhole.
		 */
<<<<<<< HEAD

		IP_INC_STATS_BH(dev_net(dev), IPSTATS_MIB_FRAGFAILS);
=======
		IP_INC_STATS(net, IPSTATS_MIB_FRAGFAILS);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ip_rt_put(rt);
		goto out_free;
	}

	encap += LL_RESERVED_SPACE(dev) + rt->dst.header_len;

	if (skb_cow(skb, encap)) {
		ip_rt_put(rt);
		goto out_free;
	}

<<<<<<< HEAD
	vif->pkt_out++;
	vif->bytes_out += skb->len;
=======
	WRITE_ONCE(vif->pkt_out, vif->pkt_out + 1);
	WRITE_ONCE(vif->bytes_out, vif->bytes_out + skb->len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	skb_dst_drop(skb);
	skb_dst_set(skb, &rt->dst);
	ip_decrease_ttl(ip_hdr(skb));

	/* FIXME: forward and output firewalls used to be called here.
	 * What do we do with netfilter? -- RR
	 */
	if (vif->flags & VIFF_TUNNEL) {
<<<<<<< HEAD
		ip_encap(skb, vif->local, vif->remote);
		/* FIXME: extra output firewall step used to be here. --RR */
		vif->dev->stats.tx_packets++;
		vif->dev->stats.tx_bytes += skb->len;
=======
		ip_encap(net, skb, vif->local, vif->remote);
		/* FIXME: extra output firewall step used to be here. --RR */
		DEV_STATS_INC(vif_dev, tx_packets);
		DEV_STATS_ADD(vif_dev, tx_bytes, skb->len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	IPCB(skb)->flags |= IPSKB_FORWARDED;

<<<<<<< HEAD
	/*
	 * RFC1584 teaches, that DVMRP/PIM router must deliver packets locally
=======
	/* RFC1584 teaches, that DVMRP/PIM router must deliver packets locally
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
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
	NF_HOOK(NFPROTO_IPV4, NF_INET_FORWARD, skb, skb->dev, dev,
=======
	NF_HOOK(NFPROTO_IPV4, NF_INET_FORWARD,
		net, NULL, skb, skb->dev, dev,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ipmr_forward_finish);
	return;

out_free:
	kfree_skb(skb);
}

<<<<<<< HEAD
static int ipmr_find_vif(struct mr_table *mrt, struct net_device *dev)
{
	int ct;

	for (ct = mrt->maxvif-1; ct >= 0; ct--) {
		if (mrt->vif_table[ct].dev == dev)
=======
/* Called with mrt_lock or rcu_read_lock() */
static int ipmr_find_vif(const struct mr_table *mrt, struct net_device *dev)
{
	int ct;
	/* Pairs with WRITE_ONCE() in vif_delete()/vif_add() */
	for (ct = READ_ONCE(mrt->maxvif) - 1; ct >= 0; ct--) {
		if (rcu_access_pointer(mrt->vif_table[ct].dev) == dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
	}
	return ct;
}

/* "local" means that we should preserve one skb (for local delivery) */
<<<<<<< HEAD

static int ip_mr_forward(struct net *net, struct mr_table *mrt,
			 struct sk_buff *skb, struct mfc_cache *cache,
			 int local)
{
	int psend = -1;
	int vif, ct;

	vif = cache->mfc_parent;
	cache->mfc_un.res.pkt++;
	cache->mfc_un.res.bytes += skb->len;

	/*
	 * Wrong interface: drop packet and (maybe) send PIM assert.
	 */
	if (mrt->vif_table[vif].dev != skb->dev) {
		int true_vifi;

=======
/* Called uner rcu_read_lock() */
static void ip_mr_forward(struct net *net, struct mr_table *mrt,
			  struct net_device *dev, struct sk_buff *skb,
			  struct mfc_cache *c, int local)
{
	int true_vifi = ipmr_find_vif(mrt, dev);
	int psend = -1;
	int vif, ct;

	vif = c->_c.mfc_parent;
	c->_c.mfc_un.res.pkt++;
	c->_c.mfc_un.res.bytes += skb->len;
	c->_c.mfc_un.res.lastuse = jiffies;

	if (c->mfc_origin == htonl(INADDR_ANY) && true_vifi >= 0) {
		struct mfc_cache *cache_proxy;

		/* For an (*,G) entry, we only check that the incoming
		 * interface is part of the static tree.
		 */
		cache_proxy = mr_mfc_find_any_parent(mrt, vif);
		if (cache_proxy &&
		    cache_proxy->_c.mfc_un.res.ttls[true_vifi] < 255)
			goto forward;
	}

	/* Wrong interface: drop packet and (maybe) send PIM assert. */
	if (rcu_access_pointer(mrt->vif_table[vif].dev) != dev) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (rt_is_output_route(skb_rtable(skb))) {
			/* It is our own packet, looped back.
			 * Very complicated situation...
			 *
			 * The best workaround until routing daemons will be
			 * fixed is not to redistribute packet, if it was
			 * send through wrong interface. It means, that
			 * multicast applications WILL NOT work for
			 * (S,G), which have default multicast route pointing
			 * to wrong oif. In any case, it is not a good
			 * idea to use multicasting applications on router.
			 */
			goto dont_forward;
		}

<<<<<<< HEAD
		cache->mfc_un.res.wrong_if++;
		true_vifi = ipmr_find_vif(mrt, skb->dev);
=======
		c->_c.mfc_un.res.wrong_if++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (true_vifi >= 0 && mrt->mroute_do_assert &&
		    /* pimsm uses asserts, when switching from RPT to SPT,
		     * so that we cannot check that packet arrived on an oif.
		     * It is bad, but otherwise we would need to move pretty
		     * large chunk of pimd to kernel. Ough... --ANK
		     */
		    (mrt->mroute_do_pim ||
<<<<<<< HEAD
		     cache->mfc_un.res.ttls[true_vifi] < 255) &&
		    time_after(jiffies,
			       cache->mfc_un.res.last_assert + MFC_ASSERT_THRESH)) {
			cache->mfc_un.res.last_assert = jiffies;
			ipmr_cache_report(mrt, skb, true_vifi, IGMPMSG_WRONGVIF);
=======
		     c->_c.mfc_un.res.ttls[true_vifi] < 255) &&
		    time_after(jiffies,
			       c->_c.mfc_un.res.last_assert +
			       MFC_ASSERT_THRESH)) {
			c->_c.mfc_un.res.last_assert = jiffies;
			ipmr_cache_report(mrt, skb, true_vifi, IGMPMSG_WRONGVIF);
			if (mrt->mroute_do_wrvifwhole)
				ipmr_cache_report(mrt, skb, true_vifi,
						  IGMPMSG_WRVIFWHOLE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		goto dont_forward;
	}

<<<<<<< HEAD
	mrt->vif_table[vif].pkt_in++;
	mrt->vif_table[vif].bytes_in += skb->len;

	/*
	 *	Forward the frame
	 */
	for (ct = cache->mfc_un.res.maxvif - 1;
	     ct >= cache->mfc_un.res.minvif; ct--) {
		if (ip_hdr(skb)->ttl > cache->mfc_un.res.ttls[ct]) {
=======
forward:
	WRITE_ONCE(mrt->vif_table[vif].pkt_in,
		   mrt->vif_table[vif].pkt_in + 1);
	WRITE_ONCE(mrt->vif_table[vif].bytes_in,
		   mrt->vif_table[vif].bytes_in + skb->len);

	/* Forward the frame */
	if (c->mfc_origin == htonl(INADDR_ANY) &&
	    c->mfc_mcastgrp == htonl(INADDR_ANY)) {
		if (true_vifi >= 0 &&
		    true_vifi != c->_c.mfc_parent &&
		    ip_hdr(skb)->ttl >
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
		if ((c->mfc_origin != htonl(INADDR_ANY) ||
		     ct != true_vifi) &&
		    ip_hdr(skb)->ttl > c->_c.mfc_un.res.ttls[ct]) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (psend != -1) {
				struct sk_buff *skb2 = skb_clone(skb, GFP_ATOMIC);

				if (skb2)
<<<<<<< HEAD
					ipmr_queue_xmit(net, mrt, skb2, cache,
							psend);
=======
					ipmr_queue_xmit(net, mrt, true_vifi,
							skb2, psend);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			}
			psend = ct;
		}
	}
<<<<<<< HEAD
=======
last_forward:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (psend != -1) {
		if (local) {
			struct sk_buff *skb2 = skb_clone(skb, GFP_ATOMIC);

			if (skb2)
<<<<<<< HEAD
				ipmr_queue_xmit(net, mrt, skb2, cache, psend);
		} else {
			ipmr_queue_xmit(net, mrt, skb, cache, psend);
			return 0;
=======
				ipmr_queue_xmit(net, mrt, true_vifi, skb2,
						psend);
		} else {
			ipmr_queue_xmit(net, mrt, true_vifi, skb, psend);
			return;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}

dont_forward:
	if (!local)
		kfree_skb(skb);
<<<<<<< HEAD
	return 0;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct mr_table *ipmr_rt_fib_lookup(struct net *net, struct sk_buff *skb)
{
	struct rtable *rt = skb_rtable(skb);
	struct iphdr *iph = ip_hdr(skb);
	struct flowi4 fl4 = {
		.daddr = iph->daddr,
		.saddr = iph->saddr,
		.flowi4_tos = RT_TOS(iph->tos),
<<<<<<< HEAD
		.flowi4_oif = rt->rt_oif,
		.flowi4_iif = rt->rt_iif,
		.flowi4_mark = rt->rt_mark,
=======
		.flowi4_oif = (rt_is_output_route(rt) ?
			       skb->dev->ifindex : 0),
		.flowi4_iif = (rt_is_output_route(rt) ?
			       LOOPBACK_IFINDEX :
			       skb->dev->ifindex),
		.flowi4_mark = skb->mark,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	};
	struct mr_table *mrt;
	int err;

	err = ipmr_fib_lookup(net, &fl4, &mrt);
	if (err)
		return ERR_PTR(err);
	return mrt;
}

<<<<<<< HEAD
/*
 *	Multicast packets for forwarding arrive here
 *	Called with rcu_read_lock();
 */

=======
/* Multicast packets for forwarding arrive here
 * Called with rcu_read_lock();
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int ip_mr_input(struct sk_buff *skb)
{
	struct mfc_cache *cache;
	struct net *net = dev_net(skb->dev);
	int local = skb_rtable(skb)->rt_flags & RTCF_LOCAL;
	struct mr_table *mrt;
<<<<<<< HEAD
=======
	struct net_device *dev;

	/* skb->dev passed in is the loX master dev for vrfs.
	 * As there are no vifs associated with loopback devices,
	 * get the proper interface that does have a vif associated with it.
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

	/* Packet is looped back after forward, it should not be
	 * forwarded second time, but still can be delivered locally.
	 */
	if (IPCB(skb)->flags & IPSKB_FORWARDED)
		goto dont_forward;

	mrt = ipmr_rt_fib_lookup(net, skb);
	if (IS_ERR(mrt)) {
		kfree_skb(skb);
		return PTR_ERR(mrt);
	}
	if (!local) {
		if (IPCB(skb)->opt.router_alert) {
			if (ip_call_ra_chain(skb))
				return 0;
		} else if (ip_hdr(skb)->protocol == IPPROTO_IGMP) {
			/* IGMPv1 (and broken IGMPv2 implementations sort of
			 * Cisco IOS <= 11.2(8)) do not put router alert
			 * option to IGMP packets destined to routable
			 * groups. It is very bad, because it means
			 * that we can forward NO IGMP messages.
			 */
			struct sock *mroute_sk;

			mroute_sk = rcu_dereference(mrt->mroute_sk);
			if (mroute_sk) {
<<<<<<< HEAD
				nf_reset(skb);
				raw_rcv(mroute_sk, skb);
				return 0;
			}
		    }
=======
				nf_reset_ct(skb);
				raw_rcv(mroute_sk, skb);
				return 0;
			}
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* already under rcu_read_lock() */
	cache = ipmr_cache_find(mrt, ip_hdr(skb)->saddr, ip_hdr(skb)->daddr);
<<<<<<< HEAD

	/*
	 *	No usable cache entry
	 */
	if (cache == NULL) {
=======
	if (!cache) {
		int vif = ipmr_find_vif(mrt, dev);

		if (vif >= 0)
			cache = ipmr_cache_find_any(mrt, ip_hdr(skb)->daddr,
						    vif);
	}

	/* No usable cache entry */
	if (!cache) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		int vif;

		if (local) {
			struct sk_buff *skb2 = skb_clone(skb, GFP_ATOMIC);
			ip_local_deliver(skb);
<<<<<<< HEAD
			if (skb2 == NULL)
=======
			if (!skb2)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				return -ENOBUFS;
			skb = skb2;
		}

<<<<<<< HEAD
		read_lock(&mrt_lock);
		vif = ipmr_find_vif(mrt, skb->dev);
		if (vif >= 0) {
			int err2 = ipmr_cache_unresolved(mrt, vif, skb);
			read_unlock(&mrt_lock);

			return err2;
		}
		read_unlock(&mrt_lock);
=======
		vif = ipmr_find_vif(mrt, dev);
		if (vif >= 0)
			return ipmr_cache_unresolved(mrt, vif, skb, dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		kfree_skb(skb);
		return -ENODEV;
	}

<<<<<<< HEAD
	read_lock(&mrt_lock);
	ip_mr_forward(net, mrt, skb, cache, local);
	read_unlock(&mrt_lock);
=======
	ip_mr_forward(net, mrt, dev, skb, cache, local);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (local)
		return ip_local_deliver(skb);

	return 0;

dont_forward:
	if (local)
		return ip_local_deliver(skb);
	kfree_skb(skb);
	return 0;
}

<<<<<<< HEAD
#ifdef CONFIG_IP_PIMSM
/* called with rcu_read_lock() */
static int __pim_rcv(struct mr_table *mrt, struct sk_buff *skb,
		     unsigned int pimlen)
{
	struct net_device *reg_dev = NULL;
	struct iphdr *encap;

	encap = (struct iphdr *)(skb_transport_header(skb) + pimlen);
	/*
	 * Check that:
	 * a. packet is really sent to a multicast group
	 * b. packet is not a NULL-REGISTER
	 * c. packet is not truncated
	 */
	if (!ipv4_is_multicast(encap->daddr) ||
	    encap->tot_len == 0 ||
	    ntohs(encap->tot_len) + pimlen > skb->len)
		return 1;

	read_lock(&mrt_lock);
	if (mrt->mroute_reg_vif_num >= 0)
		reg_dev = mrt->vif_table[mrt->mroute_reg_vif_num].dev;
	read_unlock(&mrt_lock);

	if (reg_dev == NULL)
		return 1;

	skb->mac_header = skb->network_header;
	skb_pull(skb, (u8 *)encap - skb->data);
	skb_reset_network_header(skb);
	skb->protocol = htons(ETH_P_IP);
	skb->ip_summed = CHECKSUM_NONE;
	skb->pkt_type = PACKET_HOST;

	skb_tunnel_rx(skb, reg_dev);

	netif_rx(skb);

	return NET_RX_SUCCESS;
}
#endif

#ifdef CONFIG_IP_PIMSM_V1
/*
 * Handle IGMP messages of PIMv1
 */

=======
#ifdef CONFIG_IP_PIMSM_V1
/* Handle IGMP messages of PIMv1 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int pim_rcv_v1(struct sk_buff *skb)
{
	struct igmphdr *pim;
	struct net *net = dev_net(skb->dev);
	struct mr_table *mrt;

	if (!pskb_may_pull(skb, sizeof(*pim) + sizeof(struct iphdr)))
		goto drop;

	pim = igmp_hdr(skb);

	mrt = ipmr_rt_fib_lookup(net, skb);
	if (IS_ERR(mrt))
		goto drop;
	if (!mrt->mroute_do_pim ||
	    pim->group != PIM_V1_VERSION || pim->code != PIM_V1_REGISTER)
		goto drop;

	if (__pim_rcv(mrt, skb, sizeof(*pim))) {
drop:
		kfree_skb(skb);
	}
	return 0;
}
#endif

#ifdef CONFIG_IP_PIMSM_V2
static int pim_rcv(struct sk_buff *skb)
{
	struct pimreghdr *pim;
	struct net *net = dev_net(skb->dev);
	struct mr_table *mrt;

	if (!pskb_may_pull(skb, sizeof(*pim) + sizeof(struct iphdr)))
		goto drop;

	pim = (struct pimreghdr *)skb_transport_header(skb);
<<<<<<< HEAD
	if (pim->type != ((PIM_VERSION << 4) | (PIM_REGISTER)) ||
=======
	if (pim->type != ((PIM_VERSION << 4) | (PIM_TYPE_REGISTER)) ||
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	    (pim->flags & PIM_NULL_REGISTER) ||
	    (ip_compute_csum((void *)pim, sizeof(*pim)) != 0 &&
	     csum_fold(skb_checksum(skb, 0, skb->len, 0))))
		goto drop;

	mrt = ipmr_rt_fib_lookup(net, skb);
	if (IS_ERR(mrt))
		goto drop;
	if (__pim_rcv(mrt, skb, sizeof(*pim))) {
drop:
		kfree_skb(skb);
	}
	return 0;
}
#endif

<<<<<<< HEAD
static int __ipmr_fill_mroute(struct mr_table *mrt, struct sk_buff *skb,
			      struct mfc_cache *c, struct rtmsg *rtm)
{
	int ct;
	struct rtnexthop *nhp;
	u8 *b = skb_tail_pointer(skb);
	struct rtattr *mp_head;

	/* If cache is unresolved, don't try to parse IIF and OIF */
	if (c->mfc_parent >= MAXVIFS)
		return -ENOENT;

	if (VIF_EXISTS(mrt, c->mfc_parent))
		RTA_PUT(skb, RTA_IIF, 4, &mrt->vif_table[c->mfc_parent].dev->ifindex);

	mp_head = (struct rtattr *)skb_put(skb, RTA_LENGTH(0));

	for (ct = c->mfc_un.res.minvif; ct < c->mfc_un.res.maxvif; ct++) {
		if (VIF_EXISTS(mrt, ct) && c->mfc_un.res.ttls[ct] < 255) {
			if (skb_tailroom(skb) < RTA_ALIGN(RTA_ALIGN(sizeof(*nhp)) + 4))
				goto rtattr_failure;
			nhp = (struct rtnexthop *)skb_put(skb, RTA_ALIGN(sizeof(*nhp)));
			nhp->rtnh_flags = 0;
			nhp->rtnh_hops = c->mfc_un.res.ttls[ct];
			nhp->rtnh_ifindex = mrt->vif_table[ct].dev->ifindex;
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

int ipmr_get_route(struct net *net, struct sk_buff *skb,
		   __be32 saddr, __be32 daddr,
		   struct rtmsg *rtm, int nowait)
=======
int ipmr_get_route(struct net *net, struct sk_buff *skb,
		   __be32 saddr, __be32 daddr,
		   struct rtmsg *rtm, u32 portid)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct mfc_cache *cache;
	struct mr_table *mrt;
	int err;

	mrt = ipmr_get_table(net, RT_TABLE_DEFAULT);
<<<<<<< HEAD
	if (mrt == NULL)
=======
	if (!mrt)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ENOENT;

	rcu_read_lock();
	cache = ipmr_cache_find(mrt, saddr, daddr);
<<<<<<< HEAD

	if (cache == NULL) {
=======
	if (!cache && skb->dev) {
		int vif = ipmr_find_vif(mrt, skb->dev);

		if (vif >= 0)
			cache = ipmr_cache_find_any(mrt, daddr, vif);
	}
	if (!cache) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		struct sk_buff *skb2;
		struct iphdr *iph;
		struct net_device *dev;
		int vif = -1;

<<<<<<< HEAD
		if (nowait) {
			rcu_read_unlock();
			return -EAGAIN;
		}

		dev = skb->dev;
		read_lock(&mrt_lock);
		if (dev)
			vif = ipmr_find_vif(mrt, dev);
		if (vif < 0) {
			read_unlock(&mrt_lock);
			rcu_read_unlock();
			return -ENODEV;
		}
		skb2 = skb_clone(skb, GFP_ATOMIC);
		if (!skb2) {
			read_unlock(&mrt_lock);
=======
		dev = skb->dev;
		if (dev)
			vif = ipmr_find_vif(mrt, dev);
		if (vif < 0) {
			rcu_read_unlock();
			return -ENODEV;
		}

		skb2 = skb_realloc_headroom(skb, sizeof(struct iphdr));
		if (!skb2) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			rcu_read_unlock();
			return -ENOMEM;
		}

<<<<<<< HEAD
=======
		NETLINK_CB(skb2).portid = portid;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		skb_push(skb2, sizeof(struct iphdr));
		skb_reset_network_header(skb2);
		iph = ip_hdr(skb2);
		iph->ihl = sizeof(struct iphdr) >> 2;
		iph->saddr = saddr;
		iph->daddr = daddr;
		iph->version = 0;
<<<<<<< HEAD
		err = ipmr_cache_unresolved(mrt, vif, skb2);
		read_unlock(&mrt_lock);
=======
		err = ipmr_cache_unresolved(mrt, vif, skb2, dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		rcu_read_unlock();
		return err;
	}

<<<<<<< HEAD
	read_lock(&mrt_lock);
	if (!nowait && (rtm->rtm_flags & RTM_F_NOTIFY))
		cache->mfc_flags |= MFC_NOTIFY;
	err = __ipmr_fill_mroute(mrt, skb, cache, rtm);
	read_unlock(&mrt_lock);
=======
	err = mr_fill_mroute(mrt, skb, &cache->_c, rtm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rcu_read_unlock();
	return err;
}

static int ipmr_fill_mroute(struct mr_table *mrt, struct sk_buff *skb,
<<<<<<< HEAD
			    u32 pid, u32 seq, struct mfc_cache *c)
{
	struct nlmsghdr *nlh;
	struct rtmsg *rtm;

	nlh = nlmsg_put(skb, pid, seq, RTM_NEWROUTE, sizeof(*rtm), NLM_F_MULTI);
	if (nlh == NULL)
=======
			    u32 portid, u32 seq, struct mfc_cache *c, int cmd,
			    int flags)
{
	struct nlmsghdr *nlh;
	struct rtmsg *rtm;
	int err;

	nlh = nlmsg_put(skb, portid, seq, cmd, sizeof(*rtm), flags);
	if (!nlh)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EMSGSIZE;

	rtm = nlmsg_data(nlh);
	rtm->rtm_family   = RTNL_FAMILY_IPMR;
	rtm->rtm_dst_len  = 32;
	rtm->rtm_src_len  = 32;
	rtm->rtm_tos      = 0;
	rtm->rtm_table    = mrt->id;
<<<<<<< HEAD
	NLA_PUT_U32(skb, RTA_TABLE, mrt->id);
	rtm->rtm_type     = RTN_MULTICAST;
	rtm->rtm_scope    = RT_SCOPE_UNIVERSE;
	rtm->rtm_protocol = RTPROT_UNSPEC;
	rtm->rtm_flags    = 0;

	NLA_PUT_BE32(skb, RTA_SRC, c->mfc_origin);
	NLA_PUT_BE32(skb, RTA_DST, c->mfc_mcastgrp);

	if (__ipmr_fill_mroute(mrt, skb, c, rtm) < 0)
		goto nla_put_failure;

	return nlmsg_end(skb, nlh);
=======
	if (nla_put_u32(skb, RTA_TABLE, mrt->id))
		goto nla_put_failure;
	rtm->rtm_type     = RTN_MULTICAST;
	rtm->rtm_scope    = RT_SCOPE_UNIVERSE;
	if (c->_c.mfc_flags & MFC_STATIC)
		rtm->rtm_protocol = RTPROT_STATIC;
	else
		rtm->rtm_protocol = RTPROT_MROUTED;
	rtm->rtm_flags    = 0;

	if (nla_put_in_addr(skb, RTA_SRC, c->mfc_origin) ||
	    nla_put_in_addr(skb, RTA_DST, c->mfc_mcastgrp))
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
static int ipmr_rtm_dumproute(struct sk_buff *skb, struct netlink_callback *cb)
{
	struct net *net = sock_net(skb->sk);
	struct mr_table *mrt;
	struct mfc_cache *mfc;
	unsigned int t = 0, s_t;
	unsigned int h = 0, s_h;
	unsigned int e = 0, s_e;

	s_t = cb->args[0];
	s_h = cb->args[1];
	s_e = cb->args[2];

	rcu_read_lock();
	ipmr_for_each_table(mrt, net) {
		if (t < s_t)
			goto next_table;
		if (t > s_t)
			s_h = 0;
		for (h = s_h; h < MFC_LINES; h++) {
			list_for_each_entry_rcu(mfc, &mrt->mfc_cache_array[h], list) {
				if (e < s_e)
					goto next_entry;
				if (ipmr_fill_mroute(mrt, skb,
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
	rcu_read_unlock();

	cb->args[2] = e;
	cb->args[1] = h;
=======
static int _ipmr_fill_mroute(struct mr_table *mrt, struct sk_buff *skb,
			     u32 portid, u32 seq, struct mr_mfc *c, int cmd,
			     int flags)
{
	return ipmr_fill_mroute(mrt, skb, portid, seq, (struct mfc_cache *)c,
				cmd, flags);
}

static size_t mroute_msgsize(bool unresolved, int maxvif)
{
	size_t len =
		NLMSG_ALIGN(sizeof(struct rtmsg))
		+ nla_total_size(4)	/* RTA_TABLE */
		+ nla_total_size(4)	/* RTA_SRC */
		+ nla_total_size(4)	/* RTA_DST */
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

static void mroute_netlink_event(struct mr_table *mrt, struct mfc_cache *mfc,
				 int cmd)
{
	struct net *net = read_pnet(&mrt->net);
	struct sk_buff *skb;
	int err = -ENOBUFS;

	skb = nlmsg_new(mroute_msgsize(mfc->_c.mfc_parent >= MAXVIFS,
				       mrt->maxvif),
			GFP_ATOMIC);
	if (!skb)
		goto errout;

	err = ipmr_fill_mroute(mrt, skb, 0, 0, mfc, cmd, 0);
	if (err < 0)
		goto errout;

	rtnl_notify(skb, net, 0, RTNLGRP_IPV4_MROUTE, NULL, GFP_ATOMIC);
	return;

errout:
	kfree_skb(skb);
	if (err < 0)
		rtnl_set_sk_err(net, RTNLGRP_IPV4_MROUTE, err);
}

static size_t igmpmsg_netlink_msgsize(size_t payloadlen)
{
	size_t len =
		NLMSG_ALIGN(sizeof(struct rtgenmsg))
		+ nla_total_size(1)	/* IPMRA_CREPORT_MSGTYPE */
		+ nla_total_size(4)	/* IPMRA_CREPORT_VIF_ID */
		+ nla_total_size(4)	/* IPMRA_CREPORT_SRC_ADDR */
		+ nla_total_size(4)	/* IPMRA_CREPORT_DST_ADDR */
		+ nla_total_size(4)	/* IPMRA_CREPORT_TABLE */
					/* IPMRA_CREPORT_PKT */
		+ nla_total_size(payloadlen)
		;

	return len;
}

static void igmpmsg_netlink_event(const struct mr_table *mrt, struct sk_buff *pkt)
{
	struct net *net = read_pnet(&mrt->net);
	struct nlmsghdr *nlh;
	struct rtgenmsg *rtgenm;
	struct igmpmsg *msg;
	struct sk_buff *skb;
	struct nlattr *nla;
	int payloadlen;

	payloadlen = pkt->len - sizeof(struct igmpmsg);
	msg = (struct igmpmsg *)skb_network_header(pkt);

	skb = nlmsg_new(igmpmsg_netlink_msgsize(payloadlen), GFP_ATOMIC);
	if (!skb)
		goto errout;

	nlh = nlmsg_put(skb, 0, 0, RTM_NEWCACHEREPORT,
			sizeof(struct rtgenmsg), 0);
	if (!nlh)
		goto errout;
	rtgenm = nlmsg_data(nlh);
	rtgenm->rtgen_family = RTNL_FAMILY_IPMR;
	if (nla_put_u8(skb, IPMRA_CREPORT_MSGTYPE, msg->im_msgtype) ||
	    nla_put_u32(skb, IPMRA_CREPORT_VIF_ID, msg->im_vif | (msg->im_vif_hi << 8)) ||
	    nla_put_in_addr(skb, IPMRA_CREPORT_SRC_ADDR,
			    msg->im_src.s_addr) ||
	    nla_put_in_addr(skb, IPMRA_CREPORT_DST_ADDR,
			    msg->im_dst.s_addr) ||
	    nla_put_u32(skb, IPMRA_CREPORT_TABLE, mrt->id))
		goto nla_put_failure;

	nla = nla_reserve(skb, IPMRA_CREPORT_PKT, payloadlen);
	if (!nla || skb_copy_bits(pkt, sizeof(struct igmpmsg),
				  nla_data(nla), payloadlen))
		goto nla_put_failure;

	nlmsg_end(skb, nlh);

	rtnl_notify(skb, net, 0, RTNLGRP_IPV4_MROUTE_R, NULL, GFP_ATOMIC);
	return;

nla_put_failure:
	nlmsg_cancel(skb, nlh);
errout:
	kfree_skb(skb);
	rtnl_set_sk_err(net, RTNLGRP_IPV4_MROUTE_R, -ENOBUFS);
}

static int ipmr_rtm_valid_getroute_req(struct sk_buff *skb,
				       const struct nlmsghdr *nlh,
				       struct nlattr **tb,
				       struct netlink_ext_ack *extack)
{
	struct rtmsg *rtm;
	int i, err;

	if (nlh->nlmsg_len < nlmsg_msg_size(sizeof(*rtm))) {
		NL_SET_ERR_MSG(extack, "ipv4: Invalid header for multicast route get request");
		return -EINVAL;
	}

	if (!netlink_strict_get_check(skb))
		return nlmsg_parse_deprecated(nlh, sizeof(*rtm), tb, RTA_MAX,
					      rtm_ipv4_policy, extack);

	rtm = nlmsg_data(nlh);
	if ((rtm->rtm_src_len && rtm->rtm_src_len != 32) ||
	    (rtm->rtm_dst_len && rtm->rtm_dst_len != 32) ||
	    rtm->rtm_tos || rtm->rtm_table || rtm->rtm_protocol ||
	    rtm->rtm_scope || rtm->rtm_type || rtm->rtm_flags) {
		NL_SET_ERR_MSG(extack, "ipv4: Invalid values in header for multicast route get request");
		return -EINVAL;
	}

	err = nlmsg_parse_deprecated_strict(nlh, sizeof(*rtm), tb, RTA_MAX,
					    rtm_ipv4_policy, extack);
	if (err)
		return err;

	if ((tb[RTA_SRC] && !rtm->rtm_src_len) ||
	    (tb[RTA_DST] && !rtm->rtm_dst_len)) {
		NL_SET_ERR_MSG(extack, "ipv4: rtm_src_len and rtm_dst_len must be 32 for IPv4");
		return -EINVAL;
	}

	for (i = 0; i <= RTA_MAX; i++) {
		if (!tb[i])
			continue;

		switch (i) {
		case RTA_SRC:
		case RTA_DST:
		case RTA_TABLE:
			break;
		default:
			NL_SET_ERR_MSG(extack, "ipv4: Unsupported attribute in multicast route get request");
			return -EINVAL;
		}
	}

	return 0;
}

static int ipmr_rtm_getroute(struct sk_buff *in_skb, struct nlmsghdr *nlh,
			     struct netlink_ext_ack *extack)
{
	struct net *net = sock_net(in_skb->sk);
	struct nlattr *tb[RTA_MAX + 1];
	struct sk_buff *skb = NULL;
	struct mfc_cache *cache;
	struct mr_table *mrt;
	__be32 src, grp;
	u32 tableid;
	int err;

	err = ipmr_rtm_valid_getroute_req(in_skb, nlh, tb, extack);
	if (err < 0)
		goto errout;

	src = tb[RTA_SRC] ? nla_get_in_addr(tb[RTA_SRC]) : 0;
	grp = tb[RTA_DST] ? nla_get_in_addr(tb[RTA_DST]) : 0;
	tableid = tb[RTA_TABLE] ? nla_get_u32(tb[RTA_TABLE]) : 0;

	mrt = ipmr_get_table(net, tableid ? tableid : RT_TABLE_DEFAULT);
	if (!mrt) {
		err = -ENOENT;
		goto errout_free;
	}

	/* entries are added/deleted only under RTNL */
	rcu_read_lock();
	cache = ipmr_cache_find(mrt, src, grp);
	rcu_read_unlock();
	if (!cache) {
		err = -ENOENT;
		goto errout_free;
	}

	skb = nlmsg_new(mroute_msgsize(false, mrt->maxvif), GFP_KERNEL);
	if (!skb) {
		err = -ENOBUFS;
		goto errout_free;
	}

	err = ipmr_fill_mroute(mrt, skb, NETLINK_CB(in_skb).portid,
			       nlh->nlmsg_seq, cache,
			       RTM_NEWROUTE, 0);
	if (err < 0)
		goto errout_free;

	err = rtnl_unicast(skb, net, NETLINK_CB(in_skb).portid);

errout:
	return err;

errout_free:
	kfree_skb(skb);
	goto errout;
}

static int ipmr_rtm_dumproute(struct sk_buff *skb, struct netlink_callback *cb)
{
	struct fib_dump_filter filter = {
		.rtnl_held = true,
	};
	int err;

	if (cb->strict_check) {
		err = ip_valid_fib_dump_req(sock_net(skb->sk), cb->nlh,
					    &filter, cb);
		if (err < 0)
			return err;
	}

	if (filter.table_id) {
		struct mr_table *mrt;

		mrt = ipmr_get_table(sock_net(skb->sk), filter.table_id);
		if (!mrt) {
			if (rtnl_msg_family(cb->nlh) != RTNL_FAMILY_IPMR)
				return skb->len;

			NL_SET_ERR_MSG(cb->extack, "ipv4: MR table does not exist");
			return -ENOENT;
		}
		err = mr_table_dump(mrt, skb, cb, _ipmr_fill_mroute,
				    &mfc_unres_lock, &filter);
		return skb->len ? : err;
	}

	return mr_rtm_dumproute(skb, cb, ipmr_mr_table_iter,
				_ipmr_fill_mroute, &mfc_unres_lock, &filter);
}

static const struct nla_policy rtm_ipmr_policy[RTA_MAX + 1] = {
	[RTA_SRC]	= { .type = NLA_U32 },
	[RTA_DST]	= { .type = NLA_U32 },
	[RTA_IIF]	= { .type = NLA_U32 },
	[RTA_TABLE]	= { .type = NLA_U32 },
	[RTA_MULTIPATH]	= { .len = sizeof(struct rtnexthop) },
};

static bool ipmr_rtm_validate_proto(unsigned char rtm_protocol)
{
	switch (rtm_protocol) {
	case RTPROT_STATIC:
	case RTPROT_MROUTED:
		return true;
	}
	return false;
}

static int ipmr_nla_get_ttls(const struct nlattr *nla, struct mfcctl *mfcc)
{
	struct rtnexthop *rtnh = nla_data(nla);
	int remaining = nla_len(nla), vifi = 0;

	while (rtnh_ok(rtnh, remaining)) {
		mfcc->mfcc_ttls[vifi] = rtnh->rtnh_hops;
		if (++vifi == MAXVIFS)
			break;
		rtnh = rtnh_next(rtnh, &remaining);
	}

	return remaining > 0 ? -EINVAL : vifi;
}

/* returns < 0 on error, 0 for ADD_MFC and 1 for ADD_MFC_PROXY */
static int rtm_to_ipmr_mfcc(struct net *net, struct nlmsghdr *nlh,
			    struct mfcctl *mfcc, int *mrtsock,
			    struct mr_table **mrtret,
			    struct netlink_ext_ack *extack)
{
	struct net_device *dev = NULL;
	u32 tblid = RT_TABLE_DEFAULT;
	struct mr_table *mrt;
	struct nlattr *attr;
	struct rtmsg *rtm;
	int ret, rem;

	ret = nlmsg_validate_deprecated(nlh, sizeof(*rtm), RTA_MAX,
					rtm_ipmr_policy, extack);
	if (ret < 0)
		goto out;
	rtm = nlmsg_data(nlh);

	ret = -EINVAL;
	if (rtm->rtm_family != RTNL_FAMILY_IPMR || rtm->rtm_dst_len != 32 ||
	    rtm->rtm_type != RTN_MULTICAST ||
	    rtm->rtm_scope != RT_SCOPE_UNIVERSE ||
	    !ipmr_rtm_validate_proto(rtm->rtm_protocol))
		goto out;

	memset(mfcc, 0, sizeof(*mfcc));
	mfcc->mfcc_parent = -1;
	ret = 0;
	nlmsg_for_each_attr(attr, nlh, sizeof(struct rtmsg), rem) {
		switch (nla_type(attr)) {
		case RTA_SRC:
			mfcc->mfcc_origin.s_addr = nla_get_be32(attr);
			break;
		case RTA_DST:
			mfcc->mfcc_mcastgrp.s_addr = nla_get_be32(attr);
			break;
		case RTA_IIF:
			dev = __dev_get_by_index(net, nla_get_u32(attr));
			if (!dev) {
				ret = -ENODEV;
				goto out;
			}
			break;
		case RTA_MULTIPATH:
			if (ipmr_nla_get_ttls(attr, mfcc) < 0) {
				ret = -EINVAL;
				goto out;
			}
			break;
		case RTA_PREFSRC:
			ret = 1;
			break;
		case RTA_TABLE:
			tblid = nla_get_u32(attr);
			break;
		}
	}
	mrt = ipmr_get_table(net, tblid);
	if (!mrt) {
		ret = -ENOENT;
		goto out;
	}
	*mrtret = mrt;
	*mrtsock = rtm->rtm_protocol == RTPROT_MROUTED ? 1 : 0;
	if (dev)
		mfcc->mfcc_parent = ipmr_find_vif(mrt, dev);

out:
	return ret;
}

/* takes care of both newroute and delroute */
static int ipmr_rtm_route(struct sk_buff *skb, struct nlmsghdr *nlh,
			  struct netlink_ext_ack *extack)
{
	struct net *net = sock_net(skb->sk);
	int ret, mrtsock, parent;
	struct mr_table *tbl;
	struct mfcctl mfcc;

	mrtsock = 0;
	tbl = NULL;
	ret = rtm_to_ipmr_mfcc(net, nlh, &mfcc, &mrtsock, &tbl, extack);
	if (ret < 0)
		return ret;

	parent = ret ? mfcc.mfcc_parent : -1;
	if (nlh->nlmsg_type == RTM_NEWROUTE)
		return ipmr_mfc_add(net, tbl, &mfcc, mrtsock, parent);
	else
		return ipmr_mfc_delete(tbl, &mfcc, parent);
}

static bool ipmr_fill_table(struct mr_table *mrt, struct sk_buff *skb)
{
	u32 queue_len = atomic_read(&mrt->cache_resolve_queue_len);

	if (nla_put_u32(skb, IPMRA_TABLE_ID, mrt->id) ||
	    nla_put_u32(skb, IPMRA_TABLE_CACHE_RES_QUEUE_LEN, queue_len) ||
	    nla_put_s32(skb, IPMRA_TABLE_MROUTE_REG_VIF_NUM,
			mrt->mroute_reg_vif_num) ||
	    nla_put_u8(skb, IPMRA_TABLE_MROUTE_DO_ASSERT,
		       mrt->mroute_do_assert) ||
	    nla_put_u8(skb, IPMRA_TABLE_MROUTE_DO_PIM, mrt->mroute_do_pim) ||
	    nla_put_u8(skb, IPMRA_TABLE_MROUTE_DO_WRVIFWHOLE,
		       mrt->mroute_do_wrvifwhole))
		return false;

	return true;
}

static bool ipmr_fill_vif(struct mr_table *mrt, u32 vifid, struct sk_buff *skb)
{
	struct net_device *vif_dev;
	struct nlattr *vif_nest;
	struct vif_device *vif;

	vif = &mrt->vif_table[vifid];
	vif_dev = rtnl_dereference(vif->dev);
	/* if the VIF doesn't exist just continue */
	if (!vif_dev)
		return true;

	vif_nest = nla_nest_start_noflag(skb, IPMRA_VIF);
	if (!vif_nest)
		return false;

	if (nla_put_u32(skb, IPMRA_VIFA_IFINDEX, vif_dev->ifindex) ||
	    nla_put_u32(skb, IPMRA_VIFA_VIF_ID, vifid) ||
	    nla_put_u16(skb, IPMRA_VIFA_FLAGS, vif->flags) ||
	    nla_put_u64_64bit(skb, IPMRA_VIFA_BYTES_IN, vif->bytes_in,
			      IPMRA_VIFA_PAD) ||
	    nla_put_u64_64bit(skb, IPMRA_VIFA_BYTES_OUT, vif->bytes_out,
			      IPMRA_VIFA_PAD) ||
	    nla_put_u64_64bit(skb, IPMRA_VIFA_PACKETS_IN, vif->pkt_in,
			      IPMRA_VIFA_PAD) ||
	    nla_put_u64_64bit(skb, IPMRA_VIFA_PACKETS_OUT, vif->pkt_out,
			      IPMRA_VIFA_PAD) ||
	    nla_put_be32(skb, IPMRA_VIFA_LOCAL_ADDR, vif->local) ||
	    nla_put_be32(skb, IPMRA_VIFA_REMOTE_ADDR, vif->remote)) {
		nla_nest_cancel(skb, vif_nest);
		return false;
	}
	nla_nest_end(skb, vif_nest);

	return true;
}

static int ipmr_valid_dumplink(const struct nlmsghdr *nlh,
			       struct netlink_ext_ack *extack)
{
	struct ifinfomsg *ifm;

	if (nlh->nlmsg_len < nlmsg_msg_size(sizeof(*ifm))) {
		NL_SET_ERR_MSG(extack, "ipv4: Invalid header for ipmr link dump");
		return -EINVAL;
	}

	if (nlmsg_attrlen(nlh, sizeof(*ifm))) {
		NL_SET_ERR_MSG(extack, "Invalid data after header in ipmr link dump");
		return -EINVAL;
	}

	ifm = nlmsg_data(nlh);
	if (ifm->__ifi_pad || ifm->ifi_type || ifm->ifi_flags ||
	    ifm->ifi_change || ifm->ifi_index) {
		NL_SET_ERR_MSG(extack, "Invalid values in header for ipmr link dump request");
		return -EINVAL;
	}

	return 0;
}

static int ipmr_rtm_dumplink(struct sk_buff *skb, struct netlink_callback *cb)
{
	struct net *net = sock_net(skb->sk);
	struct nlmsghdr *nlh = NULL;
	unsigned int t = 0, s_t;
	unsigned int e = 0, s_e;
	struct mr_table *mrt;

	if (cb->strict_check) {
		int err = ipmr_valid_dumplink(cb->nlh, cb->extack);

		if (err < 0)
			return err;
	}

	s_t = cb->args[0];
	s_e = cb->args[1];

	ipmr_for_each_table(mrt, net) {
		struct nlattr *vifs, *af;
		struct ifinfomsg *hdr;
		u32 i;

		if (t < s_t)
			goto skip_table;
		nlh = nlmsg_put(skb, NETLINK_CB(cb->skb).portid,
				cb->nlh->nlmsg_seq, RTM_NEWLINK,
				sizeof(*hdr), NLM_F_MULTI);
		if (!nlh)
			break;

		hdr = nlmsg_data(nlh);
		memset(hdr, 0, sizeof(*hdr));
		hdr->ifi_family = RTNL_FAMILY_IPMR;

		af = nla_nest_start_noflag(skb, IFLA_AF_SPEC);
		if (!af) {
			nlmsg_cancel(skb, nlh);
			goto out;
		}

		if (!ipmr_fill_table(mrt, skb)) {
			nlmsg_cancel(skb, nlh);
			goto out;
		}

		vifs = nla_nest_start_noflag(skb, IPMRA_TABLE_VIFS);
		if (!vifs) {
			nla_nest_end(skb, af);
			nlmsg_end(skb, nlh);
			goto out;
		}
		for (i = 0; i < mrt->maxvif; i++) {
			if (e < s_e)
				goto skip_entry;
			if (!ipmr_fill_vif(mrt, i, skb)) {
				nla_nest_end(skb, vifs);
				nla_nest_end(skb, af);
				nlmsg_end(skb, nlh);
				goto out;
			}
skip_entry:
			e++;
		}
		s_e = 0;
		e = 0;
		nla_nest_end(skb, vifs);
		nla_nest_end(skb, af);
		nlmsg_end(skb, nlh);
skip_table:
		t++;
	}

out:
	cb->args[1] = e;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	cb->args[0] = t;

	return skb->len;
}

#ifdef CONFIG_PROC_FS
<<<<<<< HEAD
/*
 *	The /proc interfaces to multicast routing :
 *	/proc/net/ip_mr_cache & /proc/net/ip_mr_vif
 */
struct ipmr_vif_iter {
	struct seq_net_private p;
	struct mr_table *mrt;
	int ct;
};

static struct vif_device *ipmr_vif_seq_idx(struct net *net,
					   struct ipmr_vif_iter *iter,
					   loff_t pos)
{
	struct mr_table *mrt = iter->mrt;

	for (iter->ct = 0; iter->ct < mrt->maxvif; ++iter->ct) {
		if (!VIF_EXISTS(mrt, iter->ct))
			continue;
		if (pos-- == 0)
			return &mrt->vif_table[iter->ct];
	}
	return NULL;
}

static void *ipmr_vif_seq_start(struct seq_file *seq, loff_t *pos)
	__acquires(mrt_lock)
{
	struct ipmr_vif_iter *iter = seq->private;
=======
/* The /proc interfaces to multicast routing :
 * /proc/net/ip_mr_cache & /proc/net/ip_mr_vif
 */

static void *ipmr_vif_seq_start(struct seq_file *seq, loff_t *pos)
	__acquires(RCU)
{
	struct mr_vif_iter *iter = seq->private;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct net *net = seq_file_net(seq);
	struct mr_table *mrt;

	mrt = ipmr_get_table(net, RT_TABLE_DEFAULT);
<<<<<<< HEAD
	if (mrt == NULL)
=======
	if (!mrt)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return ERR_PTR(-ENOENT);

	iter->mrt = mrt;

<<<<<<< HEAD
	read_lock(&mrt_lock);
	return *pos ? ipmr_vif_seq_idx(net, seq->private, *pos - 1)
		: SEQ_START_TOKEN;
}

static void *ipmr_vif_seq_next(struct seq_file *seq, void *v, loff_t *pos)
{
	struct ipmr_vif_iter *iter = seq->private;
	struct net *net = seq_file_net(seq);
	struct mr_table *mrt = iter->mrt;

	++*pos;
	if (v == SEQ_START_TOKEN)
		return ipmr_vif_seq_idx(net, iter, 0);

	while (++iter->ct < mrt->maxvif) {
		if (!VIF_EXISTS(mrt, iter->ct))
			continue;
		return &mrt->vif_table[iter->ct];
	}
	return NULL;
}

static void ipmr_vif_seq_stop(struct seq_file *seq, void *v)
	__releases(mrt_lock)
{
	read_unlock(&mrt_lock);
=======
	rcu_read_lock();
	return mr_vif_seq_start(seq, pos);
}

static void ipmr_vif_seq_stop(struct seq_file *seq, void *v)
	__releases(RCU)
{
	rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int ipmr_vif_seq_show(struct seq_file *seq, void *v)
{
<<<<<<< HEAD
	struct ipmr_vif_iter *iter = seq->private;
=======
	struct mr_vif_iter *iter = seq->private;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct mr_table *mrt = iter->mrt;

	if (v == SEQ_START_TOKEN) {
		seq_puts(seq,
			 "Interface      BytesIn  PktsIn  BytesOut PktsOut Flags Local    Remote\n");
	} else {
		const struct vif_device *vif = v;
<<<<<<< HEAD
		const char *name =  vif->dev ? vif->dev->name : "none";

		seq_printf(seq,
			   "%2Zd %-10s %8ld %7ld  %8ld %7ld %05X %08X %08X\n",
=======
		const struct net_device *vif_dev;
		const char *name;

		vif_dev = vif_dev_read(vif);
		name = vif_dev ? vif_dev->name : "none";
		seq_printf(seq,
			   "%2td %-10s %8ld %7ld  %8ld %7ld %05X %08X %08X\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			   vif - mrt->vif_table,
			   name, vif->bytes_in, vif->pkt_in,
			   vif->bytes_out, vif->pkt_out,
			   vif->flags, vif->local, vif->remote);
	}
	return 0;
}

static const struct seq_operations ipmr_vif_seq_ops = {
	.start = ipmr_vif_seq_start,
<<<<<<< HEAD
	.next  = ipmr_vif_seq_next,
=======
	.next  = mr_vif_seq_next,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.stop  = ipmr_vif_seq_stop,
	.show  = ipmr_vif_seq_show,
};

<<<<<<< HEAD
static int ipmr_vif_open(struct inode *inode, struct file *file)
{
	return seq_open_net(inode, file, &ipmr_vif_seq_ops,
			    sizeof(struct ipmr_vif_iter));
}

static const struct file_operations ipmr_vif_fops = {
	.owner	 = THIS_MODULE,
	.open    = ipmr_vif_open,
	.read    = seq_read,
	.llseek  = seq_lseek,
	.release = seq_release_net,
};

struct ipmr_mfc_iter {
	struct seq_net_private p;
	struct mr_table *mrt;
	struct list_head *cache;
	int ct;
};


static struct mfc_cache *ipmr_mfc_seq_idx(struct net *net,
					  struct ipmr_mfc_iter *it, loff_t pos)
{
	struct mr_table *mrt = it->mrt;
	struct mfc_cache *mfc;

	rcu_read_lock();
	for (it->ct = 0; it->ct < MFC_LINES; it->ct++) {
		it->cache = &mrt->mfc_cache_array[it->ct];
		list_for_each_entry_rcu(mfc, it->cache, list)
			if (pos-- == 0)
				return mfc;
	}
	rcu_read_unlock();

	spin_lock_bh(&mfc_unres_lock);
	it->cache = &mrt->mfc_unres_queue;
	list_for_each_entry(mfc, it->cache, list)
		if (pos-- == 0)
			return mfc;
	spin_unlock_bh(&mfc_unres_lock);

	it->cache = NULL;
	return NULL;
}


static void *ipmr_mfc_seq_start(struct seq_file *seq, loff_t *pos)
{
	struct ipmr_mfc_iter *it = seq->private;
=======
static void *ipmr_mfc_seq_start(struct seq_file *seq, loff_t *pos)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct net *net = seq_file_net(seq);
	struct mr_table *mrt;

	mrt = ipmr_get_table(net, RT_TABLE_DEFAULT);
<<<<<<< HEAD
	if (mrt == NULL)
		return ERR_PTR(-ENOENT);

	it->mrt = mrt;
	it->cache = NULL;
	it->ct = 0;
	return *pos ? ipmr_mfc_seq_idx(net, seq->private, *pos - 1)
		: SEQ_START_TOKEN;
}

static void *ipmr_mfc_seq_next(struct seq_file *seq, void *v, loff_t *pos)
{
	struct mfc_cache *mfc = v;
	struct ipmr_mfc_iter *it = seq->private;
	struct net *net = seq_file_net(seq);
	struct mr_table *mrt = it->mrt;

	++*pos;

	if (v == SEQ_START_TOKEN)
		return ipmr_mfc_seq_idx(net, seq->private, 0);

	if (mfc->list.next != it->cache)
		return list_entry(mfc->list.next, struct mfc_cache, list);

	if (it->cache == &mrt->mfc_unres_queue)
		goto end_of_list;

	BUG_ON(it->cache != &mrt->mfc_cache_array[it->ct]);

	while (++it->ct < MFC_LINES) {
		it->cache = &mrt->mfc_cache_array[it->ct];
		if (list_empty(it->cache))
			continue;
		return list_first_entry(it->cache, struct mfc_cache, list);
	}

	/* exhausted cache_array, show unresolved */
	rcu_read_unlock();
	it->cache = &mrt->mfc_unres_queue;
	it->ct = 0;

	spin_lock_bh(&mfc_unres_lock);
	if (!list_empty(it->cache))
		return list_first_entry(it->cache, struct mfc_cache, list);

end_of_list:
	spin_unlock_bh(&mfc_unres_lock);
	it->cache = NULL;

	return NULL;
}

static void ipmr_mfc_seq_stop(struct seq_file *seq, void *v)
{
	struct ipmr_mfc_iter *it = seq->private;
	struct mr_table *mrt = it->mrt;

	if (it->cache == &mrt->mfc_unres_queue)
		spin_unlock_bh(&mfc_unres_lock);
	else if (it->cache == &mrt->mfc_cache_array[it->ct])
		rcu_read_unlock();
=======
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
		 "Group    Origin   Iif     Pkts    Bytes    Wrong Oifs\n");
	} else {
		const struct mfc_cache *mfc = v;
<<<<<<< HEAD
		const struct ipmr_mfc_iter *it = seq->private;
=======
		const struct mr_mfc_iter *it = seq->private;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		const struct mr_table *mrt = it->mrt;

		seq_printf(seq, "%08X %08X %-3hd",
			   (__force u32) mfc->mfc_mcastgrp,
			   (__force u32) mfc->mfc_origin,
<<<<<<< HEAD
			   mfc->mfc_parent);

		if (it->cache != &mrt->mfc_unres_queue) {
			seq_printf(seq, " %8lu %8lu %8lu",
				   mfc->mfc_un.res.pkt,
				   mfc->mfc_un.res.bytes,
				   mfc->mfc_un.res.wrong_if);
			for (n = mfc->mfc_un.res.minvif;
			     n < mfc->mfc_un.res.maxvif; n++) {
				if (VIF_EXISTS(mrt, n) &&
				    mfc->mfc_un.res.ttls[n] < 255)
					seq_printf(seq,
					   " %2d:%-3d",
					   n, mfc->mfc_un.res.ttls[n]);
=======
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
					   " %2d:%-3d",
					   n, mfc->_c.mfc_un.res.ttls[n]);
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

static const struct file_operations ipmr_mfc_fops = {
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

#ifdef CONFIG_IP_PIMSM_V2
static const struct net_protocol pim_protocol = {
	.handler	=	pim_rcv,
<<<<<<< HEAD
	.netns_ok	=	1,
};
#endif


/*
 *	Setup for IP multicast routing
 */
=======
};
#endif

static unsigned int ipmr_seq_read(struct net *net)
{
	ASSERT_RTNL();

	return net->ipv4.ipmr_seq + ipmr_rules_seq_read(net);
}

static int ipmr_dump(struct net *net, struct notifier_block *nb,
		     struct netlink_ext_ack *extack)
{
	return mr_dump(net, nb, RTNL_FAMILY_IPMR, ipmr_rules_dump,
		       ipmr_mr_table_iter, extack);
}

static const struct fib_notifier_ops ipmr_notifier_ops_template = {
	.family		= RTNL_FAMILY_IPMR,
	.fib_seq_read	= ipmr_seq_read,
	.fib_dump	= ipmr_dump,
	.owner		= THIS_MODULE,
};

static int __net_init ipmr_notifier_init(struct net *net)
{
	struct fib_notifier_ops *ops;

	net->ipv4.ipmr_seq = 0;

	ops = fib_notifier_ops_register(&ipmr_notifier_ops_template, net);
	if (IS_ERR(ops))
		return PTR_ERR(ops);
	net->ipv4.ipmr_notifier_ops = ops;

	return 0;
}

static void __net_exit ipmr_notifier_exit(struct net *net)
{
	fib_notifier_ops_unregister(net->ipv4.ipmr_notifier_ops);
	net->ipv4.ipmr_notifier_ops = NULL;
}

/* Setup for IP multicast routing */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int __net_init ipmr_net_init(struct net *net)
{
	int err;

<<<<<<< HEAD
	err = ipmr_rules_init(net);
	if (err < 0)
		goto fail;

#ifdef CONFIG_PROC_FS
	err = -ENOMEM;
	if (!proc_net_fops_create(net, "ip_mr_vif", 0, &ipmr_vif_fops))
		goto proc_vif_fail;
	if (!proc_net_fops_create(net, "ip_mr_cache", 0, &ipmr_mfc_fops))
=======
	err = ipmr_notifier_init(net);
	if (err)
		goto ipmr_notifier_fail;

	err = ipmr_rules_init(net);
	if (err < 0)
		goto ipmr_rules_fail;

#ifdef CONFIG_PROC_FS
	err = -ENOMEM;
	if (!proc_create_net("ip_mr_vif", 0, net->proc_net, &ipmr_vif_seq_ops,
			sizeof(struct mr_vif_iter)))
		goto proc_vif_fail;
	if (!proc_create_net("ip_mr_cache", 0, net->proc_net, &ipmr_mfc_seq_ops,
			sizeof(struct mr_mfc_iter)))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto proc_cache_fail;
#endif
	return 0;

#ifdef CONFIG_PROC_FS
proc_cache_fail:
<<<<<<< HEAD
	proc_net_remove(net, "ip_mr_vif");
proc_vif_fail:
	ipmr_rules_exit(net);
#endif
fail:
=======
	remove_proc_entry("ip_mr_vif", net->proc_net);
proc_vif_fail:
	rtnl_lock();
	ipmr_rules_exit(net);
	rtnl_unlock();
#endif
ipmr_rules_fail:
	ipmr_notifier_exit(net);
ipmr_notifier_fail:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

static void __net_exit ipmr_net_exit(struct net *net)
{
#ifdef CONFIG_PROC_FS
<<<<<<< HEAD
	proc_net_remove(net, "ip_mr_cache");
	proc_net_remove(net, "ip_mr_vif");
#endif
	ipmr_rules_exit(net);
=======
	remove_proc_entry("ip_mr_cache", net->proc_net);
	remove_proc_entry("ip_mr_vif", net->proc_net);
#endif
	ipmr_notifier_exit(net);
}

static void __net_exit ipmr_net_exit_batch(struct list_head *net_list)
{
	struct net *net;

	rtnl_lock();
	list_for_each_entry(net, net_list, exit_list)
		ipmr_rules_exit(net);
	rtnl_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct pernet_operations ipmr_net_ops = {
	.init = ipmr_net_init,
	.exit = ipmr_net_exit,
<<<<<<< HEAD
=======
	.exit_batch = ipmr_net_exit_batch,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

int __init ip_mr_init(void)
{
	int err;

<<<<<<< HEAD
	mrt_cachep = kmem_cache_create("ip_mrt_cache",
				       sizeof(struct mfc_cache),
				       0, SLAB_HWCACHE_ALIGN | SLAB_PANIC,
				       NULL);
	if (!mrt_cachep)
		return -ENOMEM;
=======
	mrt_cachep = KMEM_CACHE(mfc_cache, SLAB_HWCACHE_ALIGN | SLAB_PANIC);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	err = register_pernet_subsys(&ipmr_net_ops);
	if (err)
		goto reg_pernet_fail;

	err = register_netdevice_notifier(&ip_mr_notifier);
	if (err)
		goto reg_notif_fail;
#ifdef CONFIG_IP_PIMSM_V2
	if (inet_add_protocol(&pim_protocol, IPPROTO_PIM) < 0) {
		pr_err("%s: can't add PIM protocol\n", __func__);
		err = -EAGAIN;
		goto add_proto_fail;
	}
#endif
	rtnl_register(RTNL_FAMILY_IPMR, RTM_GETROUTE,
<<<<<<< HEAD
		      NULL, ipmr_rtm_dumproute, NULL);
=======
		      ipmr_rtm_getroute, ipmr_rtm_dumproute, 0);
	rtnl_register(RTNL_FAMILY_IPMR, RTM_NEWROUTE,
		      ipmr_rtm_route, NULL, 0);
	rtnl_register(RTNL_FAMILY_IPMR, RTM_DELROUTE,
		      ipmr_rtm_route, NULL, 0);

	rtnl_register(RTNL_FAMILY_IPMR, RTM_GETLINK,
		      NULL, ipmr_rtm_dumplink, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;

#ifdef CONFIG_IP_PIMSM_V2
add_proto_fail:
	unregister_netdevice_notifier(&ip_mr_notifier);
#endif
reg_notif_fail:
	unregister_pernet_subsys(&ipmr_net_ops);
reg_pernet_fail:
	kmem_cache_destroy(mrt_cachep);
	return err;
}
