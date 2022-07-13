<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * IPv6 Address Label subsystem
 * for the IPv6 "Default" Source Address Selection
 *
 * Copyright (C)2007 USAGI/WIDE Project
 */
/*
 * Author:
<<<<<<< HEAD
 * 	YOSHIFUJI Hideaki @ USAGI/WIDE Project <yoshfuji@linux-ipv6.org>
=======
 *	YOSHIFUJI Hideaki @ USAGI/WIDE Project <yoshfuji@linux-ipv6.org>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/rcupdate.h>
#include <linux/in6.h>
#include <linux/slab.h>
#include <net/addrconf.h>
#include <linux/if_addrlabel.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>

#if 0
#define ADDRLABEL(x...) printk(x)
#else
<<<<<<< HEAD
#define ADDRLABEL(x...) do { ; } while(0)
=======
#define ADDRLABEL(x...) do { ; } while (0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

/*
 * Policy Table
 */
<<<<<<< HEAD
struct ip6addrlbl_entry
{
#ifdef CONFIG_NET_NS
	struct net *lbl_net;
#endif
=======
struct ip6addrlbl_entry {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct in6_addr prefix;
	int prefixlen;
	int ifindex;
	int addrtype;
	u32 label;
	struct hlist_node list;
<<<<<<< HEAD
	atomic_t refcnt;
	struct rcu_head rcu;
};

static struct ip6addrlbl_table
{
	struct hlist_head head;
	spinlock_t lock;
	u32 seq;
} ip6addrlbl_table;

static inline
struct net *ip6addrlbl_net(const struct ip6addrlbl_entry *lbl)
{
	return read_pnet(&lbl->lbl_net);
}

/*
 * Default policy table (RFC3484 + extensions)
=======
	struct rcu_head rcu;
};

/*
 * Default policy table (RFC6724 + extensions)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * prefix		addr_type	label
 * -------------------------------------------------------------------------
 * ::1/128		LOOPBACK	0
 * ::/0			N/A		1
 * 2002::/16		N/A		2
 * ::/96		COMPATv4	3
 * ::ffff:0:0/96	V4MAPPED	4
 * fc00::/7		N/A		5		ULA (RFC 4193)
 * 2001::/32		N/A		6		Teredo (RFC 4380)
 * 2001:10::/28		N/A		7		ORCHID (RFC 4843)
<<<<<<< HEAD
 *
 * Note: 0xffffffff is used if we do not have any policies.
=======
 * fec0::/10		N/A		11		Site-local
 *							(deprecated by RFC3879)
 * 3ffe::/16		N/A		12		6bone
 *
 * Note: 0xffffffff is used if we do not have any policies.
 * Note: Labels for ULA and 6to4 are different from labels listed in RFC6724.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#define IPV6_ADDR_LABEL_DEFAULT	0xffffffffUL

<<<<<<< HEAD
static const __net_initdata struct ip6addrlbl_init_table
=======
static const __net_initconst struct ip6addrlbl_init_table
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	const struct in6_addr *prefix;
	int prefixlen;
	u32 label;
} ip6addrlbl_init_table[] = {
	{	/* ::/0 */
		.prefix = &in6addr_any,
		.label = 1,
<<<<<<< HEAD
	},{	/* fc00::/7 */
		.prefix = &(struct in6_addr){{{ 0xfc }}},
		.prefixlen = 7,
		.label = 5,
	},{	/* 2002::/16 */
		.prefix = &(struct in6_addr){{{ 0x20, 0x02 }}},
		.prefixlen = 16,
		.label = 2,
	},{	/* 2001::/32 */
		.prefix = &(struct in6_addr){{{ 0x20, 0x01 }}},
		.prefixlen = 32,
		.label = 6,
	},{	/* 2001:10::/28 */
		.prefix = &(struct in6_addr){{{ 0x20, 0x01, 0x00, 0x10 }}},
		.prefixlen = 28,
		.label = 7,
	},{	/* ::ffff:0:0 */
		.prefix = &(struct in6_addr){{{ [10] = 0xff, [11] = 0xff }}},
		.prefixlen = 96,
		.label = 4,
	},{	/* ::/96 */
		.prefix = &in6addr_any,
		.prefixlen = 96,
		.label = 3,
	},{	/* ::1/128 */
=======
	}, {	/* fc00::/7 */
		.prefix = &(struct in6_addr){ { { 0xfc } } } ,
		.prefixlen = 7,
		.label = 5,
	}, {	/* fec0::/10 */
		.prefix = &(struct in6_addr){ { { 0xfe, 0xc0 } } },
		.prefixlen = 10,
		.label = 11,
	}, {	/* 2002::/16 */
		.prefix = &(struct in6_addr){ { { 0x20, 0x02 } } },
		.prefixlen = 16,
		.label = 2,
	}, {	/* 3ffe::/16 */
		.prefix = &(struct in6_addr){ { { 0x3f, 0xfe } } },
		.prefixlen = 16,
		.label = 12,
	}, {	/* 2001::/32 */
		.prefix = &(struct in6_addr){ { { 0x20, 0x01 } } },
		.prefixlen = 32,
		.label = 6,
	}, {	/* 2001:10::/28 */
		.prefix = &(struct in6_addr){ { { 0x20, 0x01, 0x00, 0x10 } } },
		.prefixlen = 28,
		.label = 7,
	}, {	/* ::ffff:0:0 */
		.prefix = &(struct in6_addr){ { { [10] = 0xff, [11] = 0xff } } },
		.prefixlen = 96,
		.label = 4,
	}, {	/* ::/96 */
		.prefix = &in6addr_any,
		.prefixlen = 96,
		.label = 3,
	}, {	/* ::1/128 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.prefix = &in6addr_loopback,
		.prefixlen = 128,
		.label = 0,
	}
};

<<<<<<< HEAD
/* Object management */
static inline void ip6addrlbl_free(struct ip6addrlbl_entry *p)
{
#ifdef CONFIG_NET_NS
	release_net(p->lbl_net);
#endif
	kfree(p);
}

static void ip6addrlbl_free_rcu(struct rcu_head *h)
{
	ip6addrlbl_free(container_of(h, struct ip6addrlbl_entry, rcu));
}

static inline int ip6addrlbl_hold(struct ip6addrlbl_entry *p)
{
	return atomic_inc_not_zero(&p->refcnt);
}

static inline void ip6addrlbl_put(struct ip6addrlbl_entry *p)
{
	if (atomic_dec_and_test(&p->refcnt))
		call_rcu(&p->rcu, ip6addrlbl_free_rcu);
}

/* Find label */
static int __ip6addrlbl_match(struct net *net,
			      struct ip6addrlbl_entry *p,
			      const struct in6_addr *addr,
			      int addrtype, int ifindex)
{
	if (!net_eq(ip6addrlbl_net(p), net))
		return 0;
	if (p->ifindex && p->ifindex != ifindex)
		return 0;
	if (p->addrtype && p->addrtype != addrtype)
		return 0;
	if (!ipv6_prefix_equal(addr, &p->prefix, p->prefixlen))
		return 0;
	return 1;
=======
/* Find label */
static bool __ip6addrlbl_match(const struct ip6addrlbl_entry *p,
			       const struct in6_addr *addr,
			       int addrtype, int ifindex)
{
	if (p->ifindex && p->ifindex != ifindex)
		return false;
	if (p->addrtype && p->addrtype != addrtype)
		return false;
	if (!ipv6_prefix_equal(addr, &p->prefix, p->prefixlen))
		return false;
	return true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct ip6addrlbl_entry *__ipv6_addr_label(struct net *net,
						  const struct in6_addr *addr,
						  int type, int ifindex)
{
<<<<<<< HEAD
	struct hlist_node *pos;
	struct ip6addrlbl_entry *p;
	hlist_for_each_entry_rcu(p, pos, &ip6addrlbl_table.head, list) {
		if (__ip6addrlbl_match(net, p, addr, type, ifindex))
=======
	struct ip6addrlbl_entry *p;

	hlist_for_each_entry_rcu(p, &net->ipv6.ip6addrlbl_table.head, list) {
		if (__ip6addrlbl_match(p, addr, type, ifindex))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return p;
	}
	return NULL;
}

u32 ipv6_addr_label(struct net *net,
		    const struct in6_addr *addr, int type, int ifindex)
{
	u32 label;
	struct ip6addrlbl_entry *p;

	type &= IPV6_ADDR_MAPPED | IPV6_ADDR_COMPATv4 | IPV6_ADDR_LOOPBACK;

	rcu_read_lock();
	p = __ipv6_addr_label(net, addr, type, ifindex);
	label = p ? p->label : IPV6_ADDR_LABEL_DEFAULT;
	rcu_read_unlock();

	ADDRLABEL(KERN_DEBUG "%s(addr=%pI6, type=%d, ifindex=%d) => %08x\n",
		  __func__, addr, type, ifindex, label);

	return label;
}

/* allocate one entry */
<<<<<<< HEAD
static struct ip6addrlbl_entry *ip6addrlbl_alloc(struct net *net,
						 const struct in6_addr *prefix,
=======
static struct ip6addrlbl_entry *ip6addrlbl_alloc(const struct in6_addr *prefix,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
						 int prefixlen, int ifindex,
						 u32 label)
{
	struct ip6addrlbl_entry *newp;
	int addrtype;

	ADDRLABEL(KERN_DEBUG "%s(prefix=%pI6, prefixlen=%d, ifindex=%d, label=%u)\n",
		  __func__, prefix, prefixlen, ifindex, (unsigned int)label);

	addrtype = ipv6_addr_type(prefix) & (IPV6_ADDR_MAPPED | IPV6_ADDR_COMPATv4 | IPV6_ADDR_LOOPBACK);

	switch (addrtype) {
	case IPV6_ADDR_MAPPED:
		if (prefixlen > 96)
			return ERR_PTR(-EINVAL);
		if (prefixlen < 96)
			addrtype = 0;
		break;
	case IPV6_ADDR_COMPATv4:
		if (prefixlen != 96)
			addrtype = 0;
		break;
	case IPV6_ADDR_LOOPBACK:
		if (prefixlen != 128)
			addrtype = 0;
		break;
	}

	newp = kmalloc(sizeof(*newp), GFP_KERNEL);
	if (!newp)
		return ERR_PTR(-ENOMEM);

	ipv6_addr_prefix(&newp->prefix, prefix, prefixlen);
	newp->prefixlen = prefixlen;
	newp->ifindex = ifindex;
	newp->addrtype = addrtype;
	newp->label = label;
	INIT_HLIST_NODE(&newp->list);
<<<<<<< HEAD
#ifdef CONFIG_NET_NS
	newp->lbl_net = hold_net(net);
#endif
	atomic_set(&newp->refcnt, 1);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return newp;
}

/* add a label */
<<<<<<< HEAD
static int __ip6addrlbl_add(struct ip6addrlbl_entry *newp, int replace)
{
	int ret = 0;

	ADDRLABEL(KERN_DEBUG "%s(newp=%p, replace=%d)\n",
			__func__,
			newp, replace);

	if (hlist_empty(&ip6addrlbl_table.head)) {
		hlist_add_head_rcu(&newp->list, &ip6addrlbl_table.head);
	} else {
		struct hlist_node *pos, *n;
		struct ip6addrlbl_entry *p = NULL;
		hlist_for_each_entry_safe(p, pos, n,
					  &ip6addrlbl_table.head, list) {
			if (p->prefixlen == newp->prefixlen &&
			    net_eq(ip6addrlbl_net(p), ip6addrlbl_net(newp)) &&
			    p->ifindex == newp->ifindex &&
			    ipv6_addr_equal(&p->prefix, &newp->prefix)) {
				if (!replace) {
					ret = -EEXIST;
					goto out;
				}
				hlist_replace_rcu(&p->list, &newp->list);
				ip6addrlbl_put(p);
				goto out;
			} else if ((p->prefixlen == newp->prefixlen && !p->ifindex) ||
				   (p->prefixlen < newp->prefixlen)) {
				hlist_add_before_rcu(&newp->list, &p->list);
				goto out;
			}
		}
		hlist_add_after_rcu(&p->list, &newp->list);
	}
out:
	if (!ret)
		ip6addrlbl_table.seq++;
=======
static int __ip6addrlbl_add(struct net *net, struct ip6addrlbl_entry *newp,
			    int replace)
{
	struct ip6addrlbl_entry *last = NULL, *p = NULL;
	struct hlist_node *n;
	int ret = 0;

	ADDRLABEL(KERN_DEBUG "%s(newp=%p, replace=%d)\n", __func__, newp,
		  replace);

	hlist_for_each_entry_safe(p, n,	&net->ipv6.ip6addrlbl_table.head, list) {
		if (p->prefixlen == newp->prefixlen &&
		    p->ifindex == newp->ifindex &&
		    ipv6_addr_equal(&p->prefix, &newp->prefix)) {
			if (!replace) {
				ret = -EEXIST;
				goto out;
			}
			hlist_replace_rcu(&p->list, &newp->list);
			kfree_rcu(p, rcu);
			goto out;
		} else if ((p->prefixlen == newp->prefixlen && !p->ifindex) ||
			   (p->prefixlen < newp->prefixlen)) {
			hlist_add_before_rcu(&newp->list, &p->list);
			goto out;
		}
		last = p;
	}
	if (last)
		hlist_add_behind_rcu(&newp->list, &last->list);
	else
		hlist_add_head_rcu(&newp->list, &net->ipv6.ip6addrlbl_table.head);
out:
	if (!ret)
		net->ipv6.ip6addrlbl_table.seq++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

/* add a label */
static int ip6addrlbl_add(struct net *net,
			  const struct in6_addr *prefix, int prefixlen,
			  int ifindex, u32 label, int replace)
{
	struct ip6addrlbl_entry *newp;
	int ret = 0;

	ADDRLABEL(KERN_DEBUG "%s(prefix=%pI6, prefixlen=%d, ifindex=%d, label=%u, replace=%d)\n",
		  __func__, prefix, prefixlen, ifindex, (unsigned int)label,
		  replace);

<<<<<<< HEAD
	newp = ip6addrlbl_alloc(net, prefix, prefixlen, ifindex, label);
	if (IS_ERR(newp))
		return PTR_ERR(newp);
	spin_lock(&ip6addrlbl_table.lock);
	ret = __ip6addrlbl_add(newp, replace);
	spin_unlock(&ip6addrlbl_table.lock);
	if (ret)
		ip6addrlbl_free(newp);
=======
	newp = ip6addrlbl_alloc(prefix, prefixlen, ifindex, label);
	if (IS_ERR(newp))
		return PTR_ERR(newp);
	spin_lock(&net->ipv6.ip6addrlbl_table.lock);
	ret = __ip6addrlbl_add(net, newp, replace);
	spin_unlock(&net->ipv6.ip6addrlbl_table.lock);
	if (ret)
		kfree(newp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

/* remove a label */
static int __ip6addrlbl_del(struct net *net,
			    const struct in6_addr *prefix, int prefixlen,
			    int ifindex)
{
	struct ip6addrlbl_entry *p = NULL;
<<<<<<< HEAD
	struct hlist_node *pos, *n;
=======
	struct hlist_node *n;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int ret = -ESRCH;

	ADDRLABEL(KERN_DEBUG "%s(prefix=%pI6, prefixlen=%d, ifindex=%d)\n",
		  __func__, prefix, prefixlen, ifindex);

<<<<<<< HEAD
	hlist_for_each_entry_safe(p, pos, n, &ip6addrlbl_table.head, list) {
		if (p->prefixlen == prefixlen &&
		    net_eq(ip6addrlbl_net(p), net) &&
		    p->ifindex == ifindex &&
		    ipv6_addr_equal(&p->prefix, prefix)) {
			hlist_del_rcu(&p->list);
			ip6addrlbl_put(p);
=======
	hlist_for_each_entry_safe(p, n, &net->ipv6.ip6addrlbl_table.head, list) {
		if (p->prefixlen == prefixlen &&
		    p->ifindex == ifindex &&
		    ipv6_addr_equal(&p->prefix, prefix)) {
			hlist_del_rcu(&p->list);
			kfree_rcu(p, rcu);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			ret = 0;
			break;
		}
	}
	return ret;
}

static int ip6addrlbl_del(struct net *net,
			  const struct in6_addr *prefix, int prefixlen,
			  int ifindex)
{
	struct in6_addr prefix_buf;
	int ret;

	ADDRLABEL(KERN_DEBUG "%s(prefix=%pI6, prefixlen=%d, ifindex=%d)\n",
		  __func__, prefix, prefixlen, ifindex);

	ipv6_addr_prefix(&prefix_buf, prefix, prefixlen);
<<<<<<< HEAD
	spin_lock(&ip6addrlbl_table.lock);
	ret = __ip6addrlbl_del(net, &prefix_buf, prefixlen, ifindex);
	spin_unlock(&ip6addrlbl_table.lock);
=======
	spin_lock(&net->ipv6.ip6addrlbl_table.lock);
	ret = __ip6addrlbl_del(net, &prefix_buf, prefixlen, ifindex);
	spin_unlock(&net->ipv6.ip6addrlbl_table.lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

/* add default label */
static int __net_init ip6addrlbl_net_init(struct net *net)
{
<<<<<<< HEAD
	int err = 0;
	int i;

	ADDRLABEL(KERN_DEBUG "%s()\n", __func__);

	for (i = 0; i < ARRAY_SIZE(ip6addrlbl_init_table); i++) {
		int ret = ip6addrlbl_add(net,
					 ip6addrlbl_init_table[i].prefix,
					 ip6addrlbl_init_table[i].prefixlen,
					 0,
					 ip6addrlbl_init_table[i].label, 0);
		/* XXX: should we free all rules when we catch an error? */
		if (ret && (!err || err != -ENOMEM))
			err = ret;
=======
	struct ip6addrlbl_entry *p = NULL;
	struct hlist_node *n;
	int err;
	int i;

	ADDRLABEL(KERN_DEBUG "%s\n", __func__);

	spin_lock_init(&net->ipv6.ip6addrlbl_table.lock);
	INIT_HLIST_HEAD(&net->ipv6.ip6addrlbl_table.head);

	for (i = 0; i < ARRAY_SIZE(ip6addrlbl_init_table); i++) {
		err = ip6addrlbl_add(net,
				     ip6addrlbl_init_table[i].prefix,
				     ip6addrlbl_init_table[i].prefixlen,
				     0,
				     ip6addrlbl_init_table[i].label, 0);
		if (err)
			goto err_ip6addrlbl_add;
	}
	return 0;

err_ip6addrlbl_add:
	hlist_for_each_entry_safe(p, n, &net->ipv6.ip6addrlbl_table.head, list) {
		hlist_del_rcu(&p->list);
		kfree_rcu(p, rcu);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return err;
}

static void __net_exit ip6addrlbl_net_exit(struct net *net)
{
	struct ip6addrlbl_entry *p = NULL;
<<<<<<< HEAD
	struct hlist_node *pos, *n;

	/* Remove all labels belonging to the exiting net */
	spin_lock(&ip6addrlbl_table.lock);
	hlist_for_each_entry_safe(p, pos, n, &ip6addrlbl_table.head, list) {
		if (net_eq(ip6addrlbl_net(p), net)) {
			hlist_del_rcu(&p->list);
			ip6addrlbl_put(p);
		}
	}
	spin_unlock(&ip6addrlbl_table.lock);
=======
	struct hlist_node *n;

	/* Remove all labels belonging to the exiting net */
	spin_lock(&net->ipv6.ip6addrlbl_table.lock);
	hlist_for_each_entry_safe(p, n, &net->ipv6.ip6addrlbl_table.head, list) {
		hlist_del_rcu(&p->list);
		kfree_rcu(p, rcu);
	}
	spin_unlock(&net->ipv6.ip6addrlbl_table.lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct pernet_operations ipv6_addr_label_ops = {
	.init = ip6addrlbl_net_init,
	.exit = ip6addrlbl_net_exit,
};

int __init ipv6_addr_label_init(void)
{
<<<<<<< HEAD
	spin_lock_init(&ip6addrlbl_table.lock);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return register_pernet_subsys(&ipv6_addr_label_ops);
}

void ipv6_addr_label_cleanup(void)
{
	unregister_pernet_subsys(&ipv6_addr_label_ops);
}

static const struct nla_policy ifal_policy[IFAL_MAX+1] = {
	[IFAL_ADDRESS]		= { .len = sizeof(struct in6_addr), },
	[IFAL_LABEL]		= { .len = sizeof(u32), },
};

<<<<<<< HEAD
static int ip6addrlbl_newdel(struct sk_buff *skb, struct nlmsghdr *nlh,
			     void *arg)
=======
static bool addrlbl_ifindex_exists(struct net *net, int ifindex)
{

	struct net_device *dev;

	rcu_read_lock();
	dev = dev_get_by_index_rcu(net, ifindex);
	rcu_read_unlock();

	return dev != NULL;
}

static int ip6addrlbl_newdel(struct sk_buff *skb, struct nlmsghdr *nlh,
			     struct netlink_ext_ack *extack)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct net *net = sock_net(skb->sk);
	struct ifaddrlblmsg *ifal;
	struct nlattr *tb[IFAL_MAX+1];
	struct in6_addr *pfx;
	u32 label;
	int err = 0;

<<<<<<< HEAD
	err = nlmsg_parse(nlh, sizeof(*ifal), tb, IFAL_MAX, ifal_policy);
=======
	err = nlmsg_parse_deprecated(nlh, sizeof(*ifal), tb, IFAL_MAX,
				     ifal_policy, extack);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err < 0)
		return err;

	ifal = nlmsg_data(nlh);

	if (ifal->ifal_family != AF_INET6 ||
	    ifal->ifal_prefixlen > 128)
		return -EINVAL;

	if (!tb[IFAL_ADDRESS])
		return -EINVAL;
<<<<<<< HEAD

	pfx = nla_data(tb[IFAL_ADDRESS]);
	if (!pfx)
		return -EINVAL;
=======
	pfx = nla_data(tb[IFAL_ADDRESS]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!tb[IFAL_LABEL])
		return -EINVAL;
	label = nla_get_u32(tb[IFAL_LABEL]);
	if (label == IPV6_ADDR_LABEL_DEFAULT)
		return -EINVAL;

<<<<<<< HEAD
	switch(nlh->nlmsg_type) {
	case RTM_NEWADDRLABEL:
		if (ifal->ifal_index &&
		    !__dev_get_by_index(net, ifal->ifal_index))
=======
	switch (nlh->nlmsg_type) {
	case RTM_NEWADDRLABEL:
		if (ifal->ifal_index &&
		    !addrlbl_ifindex_exists(net, ifal->ifal_index))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -EINVAL;

		err = ip6addrlbl_add(net, pfx, ifal->ifal_prefixlen,
				     ifal->ifal_index, label,
				     nlh->nlmsg_flags & NLM_F_REPLACE);
		break;
	case RTM_DELADDRLABEL:
		err = ip6addrlbl_del(net, pfx, ifal->ifal_prefixlen,
				     ifal->ifal_index);
		break;
	default:
		err = -EOPNOTSUPP;
	}
	return err;
}

<<<<<<< HEAD
static inline void ip6addrlbl_putmsg(struct nlmsghdr *nlh,
				     int prefixlen, int ifindex, u32 lseq)
{
	struct ifaddrlblmsg *ifal = nlmsg_data(nlh);
	ifal->ifal_family = AF_INET6;
=======
static void ip6addrlbl_putmsg(struct nlmsghdr *nlh,
			      int prefixlen, int ifindex, u32 lseq)
{
	struct ifaddrlblmsg *ifal = nlmsg_data(nlh);
	ifal->ifal_family = AF_INET6;
	ifal->__ifal_reserved = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ifal->ifal_prefixlen = prefixlen;
	ifal->ifal_flags = 0;
	ifal->ifal_index = ifindex;
	ifal->ifal_seq = lseq;
};

static int ip6addrlbl_fill(struct sk_buff *skb,
			   struct ip6addrlbl_entry *p,
			   u32 lseq,
<<<<<<< HEAD
			   u32 pid, u32 seq, int event,
			   unsigned int flags)
{
	struct nlmsghdr *nlh = nlmsg_put(skb, pid, seq, event,
=======
			   u32 portid, u32 seq, int event,
			   unsigned int flags)
{
	struct nlmsghdr *nlh = nlmsg_put(skb, portid, seq, event,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					 sizeof(struct ifaddrlblmsg), flags);
	if (!nlh)
		return -EMSGSIZE;

	ip6addrlbl_putmsg(nlh, p->prefixlen, p->ifindex, lseq);

<<<<<<< HEAD
	if (nla_put(skb, IFAL_ADDRESS, 16, &p->prefix) < 0 ||
=======
	if (nla_put_in6_addr(skb, IFAL_ADDRESS, &p->prefix) < 0 ||
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	    nla_put_u32(skb, IFAL_LABEL, p->label) < 0) {
		nlmsg_cancel(skb, nlh);
		return -EMSGSIZE;
	}

<<<<<<< HEAD
	return nlmsg_end(skb, nlh);
=======
	nlmsg_end(skb, nlh);
	return 0;
}

static int ip6addrlbl_valid_dump_req(const struct nlmsghdr *nlh,
				     struct netlink_ext_ack *extack)
{
	struct ifaddrlblmsg *ifal;

	if (nlh->nlmsg_len < nlmsg_msg_size(sizeof(*ifal))) {
		NL_SET_ERR_MSG_MOD(extack, "Invalid header for address label dump request");
		return -EINVAL;
	}

	ifal = nlmsg_data(nlh);
	if (ifal->__ifal_reserved || ifal->ifal_prefixlen ||
	    ifal->ifal_flags || ifal->ifal_index || ifal->ifal_seq) {
		NL_SET_ERR_MSG_MOD(extack, "Invalid values in header for address label dump request");
		return -EINVAL;
	}

	if (nlmsg_attrlen(nlh, sizeof(*ifal))) {
		NL_SET_ERR_MSG_MOD(extack, "Invalid data after header for address label dump request");
		return -EINVAL;
	}

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int ip6addrlbl_dump(struct sk_buff *skb, struct netlink_callback *cb)
{
<<<<<<< HEAD
	struct net *net = sock_net(skb->sk);
	struct ip6addrlbl_entry *p;
	struct hlist_node *pos;
	int idx = 0, s_idx = cb->args[0];
	int err;

	rcu_read_lock();
	hlist_for_each_entry_rcu(p, pos, &ip6addrlbl_table.head, list) {
		if (idx >= s_idx &&
		    net_eq(ip6addrlbl_net(p), net)) {
			if ((err = ip6addrlbl_fill(skb, p,
						   ip6addrlbl_table.seq,
						   NETLINK_CB(cb->skb).pid,
						   cb->nlh->nlmsg_seq,
						   RTM_NEWADDRLABEL,
						   NLM_F_MULTI)) <= 0)
=======
	const struct nlmsghdr *nlh = cb->nlh;
	struct net *net = sock_net(skb->sk);
	struct ip6addrlbl_entry *p;
	int idx = 0, s_idx = cb->args[0];
	int err;

	if (cb->strict_check) {
		err = ip6addrlbl_valid_dump_req(nlh, cb->extack);
		if (err < 0)
			return err;
	}

	rcu_read_lock();
	hlist_for_each_entry_rcu(p, &net->ipv6.ip6addrlbl_table.head, list) {
		if (idx >= s_idx) {
			err = ip6addrlbl_fill(skb, p,
					      net->ipv6.ip6addrlbl_table.seq,
					      NETLINK_CB(cb->skb).portid,
					      nlh->nlmsg_seq,
					      RTM_NEWADDRLABEL,
					      NLM_F_MULTI);
			if (err < 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				break;
		}
		idx++;
	}
	rcu_read_unlock();
	cb->args[0] = idx;
	return skb->len;
}

static inline int ip6addrlbl_msgsize(void)
{
	return NLMSG_ALIGN(sizeof(struct ifaddrlblmsg))
		+ nla_total_size(16)	/* IFAL_ADDRESS */
		+ nla_total_size(4);	/* IFAL_LABEL */
}

<<<<<<< HEAD
static int ip6addrlbl_get(struct sk_buff *in_skb, struct nlmsghdr* nlh,
			  void *arg)
=======
static int ip6addrlbl_valid_get_req(struct sk_buff *skb,
				    const struct nlmsghdr *nlh,
				    struct nlattr **tb,
				    struct netlink_ext_ack *extack)
{
	struct ifaddrlblmsg *ifal;
	int i, err;

	if (nlh->nlmsg_len < nlmsg_msg_size(sizeof(*ifal))) {
		NL_SET_ERR_MSG_MOD(extack, "Invalid header for addrlabel get request");
		return -EINVAL;
	}

	if (!netlink_strict_get_check(skb))
		return nlmsg_parse_deprecated(nlh, sizeof(*ifal), tb,
					      IFAL_MAX, ifal_policy, extack);

	ifal = nlmsg_data(nlh);
	if (ifal->__ifal_reserved || ifal->ifal_flags || ifal->ifal_seq) {
		NL_SET_ERR_MSG_MOD(extack, "Invalid values in header for addrlabel get request");
		return -EINVAL;
	}

	err = nlmsg_parse_deprecated_strict(nlh, sizeof(*ifal), tb, IFAL_MAX,
					    ifal_policy, extack);
	if (err)
		return err;

	for (i = 0; i <= IFAL_MAX; i++) {
		if (!tb[i])
			continue;

		switch (i) {
		case IFAL_ADDRESS:
			break;
		default:
			NL_SET_ERR_MSG_MOD(extack, "Unsupported attribute in addrlabel get request");
			return -EINVAL;
		}
	}

	return 0;
}

static int ip6addrlbl_get(struct sk_buff *in_skb, struct nlmsghdr *nlh,
			  struct netlink_ext_ack *extack)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct net *net = sock_net(in_skb->sk);
	struct ifaddrlblmsg *ifal;
	struct nlattr *tb[IFAL_MAX+1];
	struct in6_addr *addr;
	u32 lseq;
	int err = 0;
	struct ip6addrlbl_entry *p;
	struct sk_buff *skb;

<<<<<<< HEAD
	err = nlmsg_parse(nlh, sizeof(*ifal), tb, IFAL_MAX, ifal_policy);
=======
	err = ip6addrlbl_valid_get_req(in_skb, nlh, tb, extack);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err < 0)
		return err;

	ifal = nlmsg_data(nlh);

	if (ifal->ifal_family != AF_INET6 ||
	    ifal->ifal_prefixlen != 128)
		return -EINVAL;

	if (ifal->ifal_index &&
<<<<<<< HEAD
	    !__dev_get_by_index(net, ifal->ifal_index))
=======
	    !addrlbl_ifindex_exists(net, ifal->ifal_index))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;

	if (!tb[IFAL_ADDRESS])
		return -EINVAL;
<<<<<<< HEAD

	addr = nla_data(tb[IFAL_ADDRESS]);
	if (!addr)
		return -EINVAL;

	rcu_read_lock();
	p = __ipv6_addr_label(net, addr, ipv6_addr_type(addr), ifal->ifal_index);
	if (p && !ip6addrlbl_hold(p))
		p = NULL;
	lseq = ip6addrlbl_table.seq;
	rcu_read_unlock();

	if (!p) {
		err = -ESRCH;
		goto out;
	}

	if (!(skb = nlmsg_new(ip6addrlbl_msgsize(), GFP_KERNEL))) {
		ip6addrlbl_put(p);
		return -ENOBUFS;
	}

	err = ip6addrlbl_fill(skb, p, lseq,
			      NETLINK_CB(in_skb).pid, nlh->nlmsg_seq,
			      RTM_NEWADDRLABEL, 0);

	ip6addrlbl_put(p);

	if (err < 0) {
		WARN_ON(err == -EMSGSIZE);
		kfree_skb(skb);
		goto out;
	}

	err = rtnl_unicast(skb, net, NETLINK_CB(in_skb).pid);
out:
	return err;
}

void __init ipv6_addr_label_rtnl_register(void)
{
	__rtnl_register(PF_INET6, RTM_NEWADDRLABEL, ip6addrlbl_newdel,
			NULL, NULL);
	__rtnl_register(PF_INET6, RTM_DELADDRLABEL, ip6addrlbl_newdel,
			NULL, NULL);
	__rtnl_register(PF_INET6, RTM_GETADDRLABEL, ip6addrlbl_get,
			ip6addrlbl_dump, NULL);
}

=======
	addr = nla_data(tb[IFAL_ADDRESS]);

	skb = nlmsg_new(ip6addrlbl_msgsize(), GFP_KERNEL);
	if (!skb)
		return -ENOBUFS;

	err = -ESRCH;

	rcu_read_lock();
	p = __ipv6_addr_label(net, addr, ipv6_addr_type(addr), ifal->ifal_index);
	lseq = net->ipv6.ip6addrlbl_table.seq;
	if (p)
		err = ip6addrlbl_fill(skb, p, lseq,
				      NETLINK_CB(in_skb).portid,
				      nlh->nlmsg_seq,
				      RTM_NEWADDRLABEL, 0);
	rcu_read_unlock();

	if (err < 0) {
		WARN_ON(err == -EMSGSIZE);
		kfree_skb(skb);
	} else {
		err = rtnl_unicast(skb, net, NETLINK_CB(in_skb).portid);
	}
	return err;
}

int __init ipv6_addr_label_rtnl_register(void)
{
	int ret;

	ret = rtnl_register_module(THIS_MODULE, PF_INET6, RTM_NEWADDRLABEL,
				   ip6addrlbl_newdel,
				   NULL, RTNL_FLAG_DOIT_UNLOCKED);
	if (ret < 0)
		return ret;
	ret = rtnl_register_module(THIS_MODULE, PF_INET6, RTM_DELADDRLABEL,
				   ip6addrlbl_newdel,
				   NULL, RTNL_FLAG_DOIT_UNLOCKED);
	if (ret < 0)
		return ret;
	ret = rtnl_register_module(THIS_MODULE, PF_INET6, RTM_GETADDRLABEL,
				   ip6addrlbl_get,
				   ip6addrlbl_dump, RTNL_FLAG_DOIT_UNLOCKED);
	return ret;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
