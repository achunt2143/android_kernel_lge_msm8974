<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *	Anycast support for IPv6
 *	Linux INET6 implementation
 *
 *	Authors:
 *	David L Stevens (dlstevens@us.ibm.com)
 *
 *	based heavily on net/ipv6/mcast.c
<<<<<<< HEAD
 *
 *	This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/capability.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/random.h>
#include <linux/string.h>
#include <linux/socket.h>
#include <linux/sockios.h>
#include <linux/net.h>
#include <linux/in6.h>
#include <linux/netdevice.h>
#include <linux/if_arp.h>
#include <linux/route.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/slab.h>

#include <net/net_namespace.h>
#include <net/sock.h>
#include <net/snmp.h>

#include <net/ipv6.h>
#include <net/protocol.h>
#include <net/if_inet6.h>
#include <net/ndisc.h>
#include <net/addrconf.h>
#include <net/ip6_route.h>

#include <net/checksum.h>

<<<<<<< HEAD
static int ipv6_dev_ac_dec(struct net_device *dev, const struct in6_addr *addr);

/* Big ac list lock for all the sockets */
static DEFINE_RWLOCK(ipv6_sk_ac_lock);

=======
#define IN6_ADDR_HSIZE_SHIFT	8
#define IN6_ADDR_HSIZE		BIT(IN6_ADDR_HSIZE_SHIFT)
/*	anycast address hash table
 */
static struct hlist_head inet6_acaddr_lst[IN6_ADDR_HSIZE];
static DEFINE_SPINLOCK(acaddr_hash_lock);

static int ipv6_dev_ac_dec(struct net_device *dev, const struct in6_addr *addr);

static u32 inet6_acaddr_hash(struct net *net, const struct in6_addr *addr)
{
	u32 val = ipv6_addr_hash(addr) ^ net_hash_mix(net);

	return hash_32(val, IN6_ADDR_HSIZE_SHIFT);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 *	socket join an anycast group
 */

int ipv6_sock_ac_join(struct sock *sk, int ifindex, const struct in6_addr *addr)
{
	struct ipv6_pinfo *np = inet6_sk(sk);
	struct net_device *dev = NULL;
	struct inet6_dev *idev;
	struct ipv6_ac_socklist *pac;
	struct net *net = sock_net(sk);
	int	ishost = !net->ipv6.devconf_all->forwarding;
	int	err = 0;

<<<<<<< HEAD
	if (!capable(CAP_NET_ADMIN))
		return -EPERM;
	if (ipv6_addr_is_multicast(addr))
		return -EINVAL;
	if (ipv6_chk_addr(net, addr, NULL, 0))
		return -EINVAL;

	pac = sock_kmalloc(sk, sizeof(struct ipv6_ac_socklist), GFP_KERNEL);
	if (pac == NULL)
=======
	ASSERT_RTNL();

	if (!ns_capable(net->user_ns, CAP_NET_ADMIN))
		return -EPERM;
	if (ipv6_addr_is_multicast(addr))
		return -EINVAL;

	if (ifindex)
		dev = __dev_get_by_index(net, ifindex);

	if (ipv6_chk_addr_and_flags(net, addr, dev, true, 0, IFA_F_TENTATIVE))
		return -EINVAL;

	pac = sock_kmalloc(sk, sizeof(struct ipv6_ac_socklist), GFP_KERNEL);
	if (!pac)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ENOMEM;
	pac->acl_next = NULL;
	pac->acl_addr = *addr;

<<<<<<< HEAD
	rcu_read_lock();
	if (ifindex == 0) {
		struct rt6_info *rt;

		rt = rt6_lookup(net, addr, NULL, 0, 0);
		if (rt) {
			dev = rt->dst.dev;
			dst_release(&rt->dst);
=======
	if (ifindex == 0) {
		struct rt6_info *rt;

		rt = rt6_lookup(net, addr, NULL, 0, NULL, 0);
		if (rt) {
			dev = rt->dst.dev;
			ip6_rt_put(rt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		} else if (ishost) {
			err = -EADDRNOTAVAIL;
			goto error;
		} else {
			/* router, no matching interface: just pick one */
<<<<<<< HEAD
			dev = dev_get_by_flags_rcu(net, IFF_UP,
						   IFF_UP | IFF_LOOPBACK);
		}
	} else
		dev = dev_get_by_index_rcu(net, ifindex);

	if (dev == NULL) {
=======
			dev = __dev_get_by_flags(net, IFF_UP,
						 IFF_UP | IFF_LOOPBACK);
		}
	}

	if (!dev) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = -ENODEV;
		goto error;
	}

	idev = __in6_dev_get(dev);
	if (!idev) {
		if (ifindex)
			err = -ENODEV;
		else
			err = -EADDRNOTAVAIL;
		goto error;
	}
	/* reset ishost, now that we have a specific device */
	ishost = !idev->cnf.forwarding;

	pac->acl_ifindex = dev->ifindex;

	/* XXX
	 * For hosts, allow link-local or matching prefix anycasts.
	 * This obviates the need for propagating anycast routes while
	 * still allowing some non-router anycast participation.
	 */
	if (!ipv6_chk_prefix(addr, dev)) {
		if (ishost)
			err = -EADDRNOTAVAIL;
		if (err)
			goto error;
	}

<<<<<<< HEAD
	err = ipv6_dev_ac_inc(dev, addr);
	if (!err) {
		write_lock_bh(&ipv6_sk_ac_lock);
		pac->acl_next = np->ipv6_ac_list;
		np->ipv6_ac_list = pac;
		write_unlock_bh(&ipv6_sk_ac_lock);
=======
	err = __ipv6_dev_ac_inc(idev, addr);
	if (!err) {
		pac->acl_next = np->ipv6_ac_list;
		np->ipv6_ac_list = pac;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		pac = NULL;
	}

error:
<<<<<<< HEAD
	rcu_read_unlock();
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (pac)
		sock_kfree_s(sk, pac, sizeof(*pac));
	return err;
}

/*
 *	socket leave an anycast group
 */
int ipv6_sock_ac_drop(struct sock *sk, int ifindex, const struct in6_addr *addr)
{
	struct ipv6_pinfo *np = inet6_sk(sk);
	struct net_device *dev;
	struct ipv6_ac_socklist *pac, *prev_pac;
	struct net *net = sock_net(sk);

<<<<<<< HEAD
	write_lock_bh(&ipv6_sk_ac_lock);
=======
	ASSERT_RTNL();

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	prev_pac = NULL;
	for (pac = np->ipv6_ac_list; pac; pac = pac->acl_next) {
		if ((ifindex == 0 || pac->acl_ifindex == ifindex) &&
		     ipv6_addr_equal(&pac->acl_addr, addr))
			break;
		prev_pac = pac;
	}
<<<<<<< HEAD
	if (!pac) {
		write_unlock_bh(&ipv6_sk_ac_lock);
		return -ENOENT;
	}
=======
	if (!pac)
		return -ENOENT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (prev_pac)
		prev_pac->acl_next = pac->acl_next;
	else
		np->ipv6_ac_list = pac->acl_next;

<<<<<<< HEAD
	write_unlock_bh(&ipv6_sk_ac_lock);

	rcu_read_lock();
	dev = dev_get_by_index_rcu(net, pac->acl_ifindex);
	if (dev)
		ipv6_dev_ac_dec(dev, &pac->acl_addr);
	rcu_read_unlock();
=======
	dev = __dev_get_by_index(net, pac->acl_ifindex);
	if (dev)
		ipv6_dev_ac_dec(dev, &pac->acl_addr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	sock_kfree_s(sk, pac, sizeof(*pac));
	return 0;
}

<<<<<<< HEAD
void ipv6_sock_ac_close(struct sock *sk)
=======
void __ipv6_sock_ac_close(struct sock *sk)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ipv6_pinfo *np = inet6_sk(sk);
	struct net_device *dev = NULL;
	struct ipv6_ac_socklist *pac;
	struct net *net = sock_net(sk);
	int	prev_index;

<<<<<<< HEAD
	write_lock_bh(&ipv6_sk_ac_lock);
	pac = np->ipv6_ac_list;
	np->ipv6_ac_list = NULL;
	write_unlock_bh(&ipv6_sk_ac_lock);

	prev_index = 0;
	rcu_read_lock();
=======
	ASSERT_RTNL();
	pac = np->ipv6_ac_list;
	np->ipv6_ac_list = NULL;

	prev_index = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	while (pac) {
		struct ipv6_ac_socklist *next = pac->acl_next;

		if (pac->acl_ifindex != prev_index) {
<<<<<<< HEAD
			dev = dev_get_by_index_rcu(net, pac->acl_ifindex);
=======
			dev = __dev_get_by_index(net, pac->acl_ifindex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			prev_index = pac->acl_ifindex;
		}
		if (dev)
			ipv6_dev_ac_dec(dev, &pac->acl_addr);
		sock_kfree_s(sk, pac, sizeof(*pac));
		pac = next;
	}
<<<<<<< HEAD
	rcu_read_unlock();
=======
}

void ipv6_sock_ac_close(struct sock *sk)
{
	struct ipv6_pinfo *np = inet6_sk(sk);

	if (!np->ipv6_ac_list)
		return;
	rtnl_lock();
	__ipv6_sock_ac_close(sk);
	rtnl_unlock();
}

static void ipv6_add_acaddr_hash(struct net *net, struct ifacaddr6 *aca)
{
	unsigned int hash = inet6_acaddr_hash(net, &aca->aca_addr);

	spin_lock(&acaddr_hash_lock);
	hlist_add_head_rcu(&aca->aca_addr_lst, &inet6_acaddr_lst[hash]);
	spin_unlock(&acaddr_hash_lock);
}

static void ipv6_del_acaddr_hash(struct ifacaddr6 *aca)
{
	spin_lock(&acaddr_hash_lock);
	hlist_del_init_rcu(&aca->aca_addr_lst);
	spin_unlock(&acaddr_hash_lock);
}

static void aca_get(struct ifacaddr6 *aca)
{
	refcount_inc(&aca->aca_refcnt);
}

static void aca_free_rcu(struct rcu_head *h)
{
	struct ifacaddr6 *aca = container_of(h, struct ifacaddr6, rcu);

	fib6_info_release(aca->aca_rt);
	kfree(aca);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void aca_put(struct ifacaddr6 *ac)
{
<<<<<<< HEAD
	if (atomic_dec_and_test(&ac->aca_refcnt)) {
		in6_dev_put(ac->aca_idev);
		dst_release(&ac->aca_rt->dst);
		kfree(ac);
	}
}

/*
 *	device anycast group inc (add if not found)
 */
int ipv6_dev_ac_inc(struct net_device *dev, const struct in6_addr *addr)
{
	struct ifacaddr6 *aca;
	struct inet6_dev *idev;
	struct rt6_info *rt;
	int err;

	idev = in6_dev_get(dev);

	if (idev == NULL)
		return -EINVAL;
=======
	if (refcount_dec_and_test(&ac->aca_refcnt)) {
		call_rcu(&ac->rcu, aca_free_rcu);
	}
}

static struct ifacaddr6 *aca_alloc(struct fib6_info *f6i,
				   const struct in6_addr *addr)
{
	struct ifacaddr6 *aca;

	aca = kzalloc(sizeof(*aca), GFP_ATOMIC);
	if (!aca)
		return NULL;

	aca->aca_addr = *addr;
	fib6_info_hold(f6i);
	aca->aca_rt = f6i;
	INIT_HLIST_NODE(&aca->aca_addr_lst);
	aca->aca_users = 1;
	/* aca_tstamp should be updated upon changes */
	aca->aca_cstamp = aca->aca_tstamp = jiffies;
	refcount_set(&aca->aca_refcnt, 1);

	return aca;
}

/*
 *	device anycast group inc (add if not found)
 */
int __ipv6_dev_ac_inc(struct inet6_dev *idev, const struct in6_addr *addr)
{
	struct ifacaddr6 *aca;
	struct fib6_info *f6i;
	struct net *net;
	int err;

	ASSERT_RTNL();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	write_lock_bh(&idev->lock);
	if (idev->dead) {
		err = -ENODEV;
		goto out;
	}

<<<<<<< HEAD
	for (aca = idev->ac_list; aca; aca = aca->aca_next) {
=======
	for (aca = rtnl_dereference(idev->ac_list); aca;
	     aca = rtnl_dereference(aca->aca_next)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (ipv6_addr_equal(&aca->aca_addr, addr)) {
			aca->aca_users++;
			err = 0;
			goto out;
		}
	}

<<<<<<< HEAD
	/*
	 *	not found: create a new one.
	 */

	aca = kzalloc(sizeof(struct ifacaddr6), GFP_ATOMIC);

	if (aca == NULL) {
=======
	net = dev_net(idev->dev);
	f6i = addrconf_f6i_alloc(net, idev, addr, true, GFP_ATOMIC, NULL);
	if (IS_ERR(f6i)) {
		err = PTR_ERR(f6i);
		goto out;
	}
	aca = aca_alloc(f6i, addr);
	if (!aca) {
		fib6_info_release(f6i);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = -ENOMEM;
		goto out;
	}

<<<<<<< HEAD
	rt = addrconf_dst_alloc(idev, addr, true);
	if (IS_ERR(rt)) {
		kfree(aca);
		err = PTR_ERR(rt);
		goto out;
	}

	aca->aca_addr = *addr;
	aca->aca_idev = idev;
	aca->aca_rt = rt;
	aca->aca_users = 1;
	/* aca_tstamp should be updated upon changes */
	aca->aca_cstamp = aca->aca_tstamp = jiffies;
	atomic_set(&aca->aca_refcnt, 2);
	spin_lock_init(&aca->aca_lock);

	aca->aca_next = idev->ac_list;
	idev->ac_list = aca;
	write_unlock_bh(&idev->lock);

	ip6_ins_rt(rt);

	addrconf_join_solict(dev, &aca->aca_addr);
=======
	/* Hold this for addrconf_join_solict() below before we unlock,
	 * it is already exposed via idev->ac_list.
	 */
	aca_get(aca);
	aca->aca_next = idev->ac_list;
	rcu_assign_pointer(idev->ac_list, aca);

	write_unlock_bh(&idev->lock);

	ipv6_add_acaddr_hash(net, aca);

	ip6_ins_rt(net, f6i);

	addrconf_join_solict(idev->dev, &aca->aca_addr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	aca_put(aca);
	return 0;
out:
	write_unlock_bh(&idev->lock);
<<<<<<< HEAD
	in6_dev_put(idev);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

/*
 *	device anycast group decrement
 */
int __ipv6_dev_ac_dec(struct inet6_dev *idev, const struct in6_addr *addr)
{
	struct ifacaddr6 *aca, *prev_aca;

<<<<<<< HEAD
	write_lock_bh(&idev->lock);
	prev_aca = NULL;
	for (aca = idev->ac_list; aca; aca = aca->aca_next) {
=======
	ASSERT_RTNL();

	write_lock_bh(&idev->lock);
	prev_aca = NULL;
	for (aca = rtnl_dereference(idev->ac_list); aca;
	     aca = rtnl_dereference(aca->aca_next)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (ipv6_addr_equal(&aca->aca_addr, addr))
			break;
		prev_aca = aca;
	}
	if (!aca) {
		write_unlock_bh(&idev->lock);
		return -ENOENT;
	}
	if (--aca->aca_users > 0) {
		write_unlock_bh(&idev->lock);
		return 0;
	}
	if (prev_aca)
<<<<<<< HEAD
		prev_aca->aca_next = aca->aca_next;
	else
		idev->ac_list = aca->aca_next;
	write_unlock_bh(&idev->lock);
	addrconf_leave_solict(idev, &aca->aca_addr);

	dst_hold(&aca->aca_rt->dst);
	ip6_del_rt(aca->aca_rt);
=======
		rcu_assign_pointer(prev_aca->aca_next, aca->aca_next);
	else
		rcu_assign_pointer(idev->ac_list, aca->aca_next);
	write_unlock_bh(&idev->lock);
	ipv6_del_acaddr_hash(aca);
	addrconf_leave_solict(idev, &aca->aca_addr);

	ip6_del_rt(dev_net(idev->dev), aca->aca_rt, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	aca_put(aca);
	return 0;
}

<<<<<<< HEAD
/* called with rcu_read_lock() */
=======
/* called with rtnl_lock() */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int ipv6_dev_ac_dec(struct net_device *dev, const struct in6_addr *addr)
{
	struct inet6_dev *idev = __in6_dev_get(dev);

<<<<<<< HEAD
	if (idev == NULL)
=======
	if (!idev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ENODEV;
	return __ipv6_dev_ac_dec(idev, addr);
}

<<<<<<< HEAD
=======
void ipv6_ac_destroy_dev(struct inet6_dev *idev)
{
	struct ifacaddr6 *aca;

	write_lock_bh(&idev->lock);
	while ((aca = rtnl_dereference(idev->ac_list)) != NULL) {
		rcu_assign_pointer(idev->ac_list, aca->aca_next);
		write_unlock_bh(&idev->lock);

		ipv6_del_acaddr_hash(aca);

		addrconf_leave_solict(idev, &aca->aca_addr);

		ip6_del_rt(dev_net(idev->dev), aca->aca_rt, false);

		aca_put(aca);

		write_lock_bh(&idev->lock);
	}
	write_unlock_bh(&idev->lock);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *	check if the interface has this anycast address
 *	called with rcu_read_lock()
 */
<<<<<<< HEAD
static int ipv6_chk_acast_dev(struct net_device *dev, const struct in6_addr *addr)
=======
static bool ipv6_chk_acast_dev(struct net_device *dev, const struct in6_addr *addr)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct inet6_dev *idev;
	struct ifacaddr6 *aca;

	idev = __in6_dev_get(dev);
	if (idev) {
<<<<<<< HEAD
		read_lock_bh(&idev->lock);
		for (aca = idev->ac_list; aca; aca = aca->aca_next)
			if (ipv6_addr_equal(&aca->aca_addr, addr))
				break;
		read_unlock_bh(&idev->lock);
		return aca != NULL;
	}
	return 0;
=======
		for (aca = rcu_dereference(idev->ac_list); aca;
		     aca = rcu_dereference(aca->aca_next))
			if (ipv6_addr_equal(&aca->aca_addr, addr))
				break;
		return aca != NULL;
	}
	return false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 *	check if given interface (or any, if dev==0) has this anycast address
 */
<<<<<<< HEAD
int ipv6_chk_acast_addr(struct net *net, struct net_device *dev,
			const struct in6_addr *addr)
{
	int found = 0;
=======
bool ipv6_chk_acast_addr(struct net *net, struct net_device *dev,
			 const struct in6_addr *addr)
{
	struct net_device *nh_dev;
	struct ifacaddr6 *aca;
	bool found = false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	rcu_read_lock();
	if (dev)
		found = ipv6_chk_acast_dev(dev, addr);
<<<<<<< HEAD
	else
		for_each_netdev_rcu(net, dev)
			if (ipv6_chk_acast_dev(dev, addr)) {
				found = 1;
				break;
			}
=======
	else {
		unsigned int hash = inet6_acaddr_hash(net, addr);

		hlist_for_each_entry_rcu(aca, &inet6_acaddr_lst[hash],
					 aca_addr_lst) {
			nh_dev = fib6_info_nh_dev(aca->aca_rt);
			if (!nh_dev || !net_eq(dev_net(nh_dev), net))
				continue;
			if (ipv6_addr_equal(&aca->aca_addr, addr)) {
				found = true;
				break;
			}
		}
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rcu_read_unlock();
	return found;
}

<<<<<<< HEAD
=======
/*	check if this anycast address is link-local on given interface or
 *	is global
 */
bool ipv6_chk_acast_addr_src(struct net *net, struct net_device *dev,
			     const struct in6_addr *addr)
{
	return ipv6_chk_acast_addr(net,
				   (ipv6_addr_type(addr) & IPV6_ADDR_LINKLOCAL ?
				    dev : NULL),
				   addr);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_PROC_FS
struct ac6_iter_state {
	struct seq_net_private p;
	struct net_device *dev;
<<<<<<< HEAD
	struct inet6_dev *idev;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

#define ac6_seq_private(seq)	((struct ac6_iter_state *)(seq)->private)

static inline struct ifacaddr6 *ac6_get_first(struct seq_file *seq)
{
<<<<<<< HEAD
	struct ifacaddr6 *im = NULL;
	struct ac6_iter_state *state = ac6_seq_private(seq);
	struct net *net = seq_file_net(seq);

	state->idev = NULL;
	for_each_netdev_rcu(net, state->dev) {
		struct inet6_dev *idev;
		idev = __in6_dev_get(state->dev);
		if (!idev)
			continue;
		read_lock_bh(&idev->lock);
		im = idev->ac_list;
		if (im) {
			state->idev = idev;
			break;
		}
		read_unlock_bh(&idev->lock);
=======
	struct ac6_iter_state *state = ac6_seq_private(seq);
	struct net *net = seq_file_net(seq);
	struct ifacaddr6 *im = NULL;

	for_each_netdev_rcu(net, state->dev) {
		struct inet6_dev *idev;

		idev = __in6_dev_get(state->dev);
		if (!idev)
			continue;
		im = rcu_dereference(idev->ac_list);
		if (im)
			break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return im;
}

static struct ifacaddr6 *ac6_get_next(struct seq_file *seq, struct ifacaddr6 *im)
{
	struct ac6_iter_state *state = ac6_seq_private(seq);
<<<<<<< HEAD

	im = im->aca_next;
	while (!im) {
		if (likely(state->idev != NULL))
			read_unlock_bh(&state->idev->lock);

		state->dev = next_net_device_rcu(state->dev);
		if (!state->dev) {
			state->idev = NULL;
			break;
		}
		state->idev = __in6_dev_get(state->dev);
		if (!state->idev)
			continue;
		read_lock_bh(&state->idev->lock);
		im = state->idev->ac_list;
=======
	struct inet6_dev *idev;

	im = rcu_dereference(im->aca_next);
	while (!im) {
		state->dev = next_net_device_rcu(state->dev);
		if (!state->dev)
			break;
		idev = __in6_dev_get(state->dev);
		if (!idev)
			continue;
		im = rcu_dereference(idev->ac_list);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return im;
}

static struct ifacaddr6 *ac6_get_idx(struct seq_file *seq, loff_t pos)
{
	struct ifacaddr6 *im = ac6_get_first(seq);
	if (im)
		while (pos && (im = ac6_get_next(seq, im)) != NULL)
			--pos;
	return pos ? NULL : im;
}

static void *ac6_seq_start(struct seq_file *seq, loff_t *pos)
	__acquires(RCU)
{
	rcu_read_lock();
	return ac6_get_idx(seq, *pos);
}

static void *ac6_seq_next(struct seq_file *seq, void *v, loff_t *pos)
{
	struct ifacaddr6 *im = ac6_get_next(seq, v);

	++*pos;
	return im;
}

static void ac6_seq_stop(struct seq_file *seq, void *v)
	__releases(RCU)
{
<<<<<<< HEAD
	struct ac6_iter_state *state = ac6_seq_private(seq);

	if (likely(state->idev != NULL)) {
		read_unlock_bh(&state->idev->lock);
		state->idev = NULL;
	}
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rcu_read_unlock();
}

static int ac6_seq_show(struct seq_file *seq, void *v)
{
	struct ifacaddr6 *im = (struct ifacaddr6 *)v;
	struct ac6_iter_state *state = ac6_seq_private(seq);

	seq_printf(seq, "%-4d %-15s %pi6 %5d\n",
		   state->dev->ifindex, state->dev->name,
		   &im->aca_addr, im->aca_users);
	return 0;
}

static const struct seq_operations ac6_seq_ops = {
	.start	=	ac6_seq_start,
	.next	=	ac6_seq_next,
	.stop	=	ac6_seq_stop,
	.show	=	ac6_seq_show,
};

<<<<<<< HEAD
static int ac6_seq_open(struct inode *inode, struct file *file)
{
	return seq_open_net(inode, file, &ac6_seq_ops,
			    sizeof(struct ac6_iter_state));
}

static const struct file_operations ac6_seq_fops = {
	.owner		=	THIS_MODULE,
	.open		=	ac6_seq_open,
	.read		=	seq_read,
	.llseek		=	seq_lseek,
	.release	=	seq_release_net,
};

int __net_init ac6_proc_init(struct net *net)
{
	if (!proc_net_fops_create(net, "anycast6", S_IRUGO, &ac6_seq_fops))
=======
int __net_init ac6_proc_init(struct net *net)
{
	if (!proc_create_net("anycast6", 0444, net->proc_net, &ac6_seq_ops,
			sizeof(struct ac6_iter_state)))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ENOMEM;

	return 0;
}

void ac6_proc_exit(struct net *net)
{
<<<<<<< HEAD
	proc_net_remove(net, "anycast6");
}
#endif

=======
	remove_proc_entry("anycast6", net->proc_net);
}
#endif

/*	Init / cleanup code
 */
int __init ipv6_anycast_init(void)
{
	int i;

	for (i = 0; i < IN6_ADDR_HSIZE; i++)
		INIT_HLIST_HEAD(&inet6_acaddr_lst[i]);
	return 0;
}

void ipv6_anycast_cleanup(void)
{
	int i;

	spin_lock(&acaddr_hash_lock);
	for (i = 0; i < IN6_ADDR_HSIZE; i++)
		WARN_ON(!hlist_empty(&inet6_acaddr_lst[i]));
	spin_unlock(&acaddr_hash_lock);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
