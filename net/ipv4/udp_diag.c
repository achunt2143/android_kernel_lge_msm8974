<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * udp_diag.c	Module for monitoring UDP transport protocols sockets.
 *
 * Authors:	Pavel Emelyanov, <xemul@parallels.com>
<<<<<<< HEAD
 *
 *	This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */


#include <linux/module.h>
#include <linux/inet_diag.h>
#include <linux/udp.h>
#include <net/udp.h>
#include <net/udplite.h>
#include <linux/sock_diag.h>

static int sk_diag_dump(struct sock *sk, struct sk_buff *skb,
<<<<<<< HEAD
		struct netlink_callback *cb, struct inet_diag_req_v2 *req,
		struct nlattr *bc)
=======
			struct netlink_callback *cb,
			const struct inet_diag_req_v2 *req,
			struct nlattr *bc, bool net_admin)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	if (!inet_diag_bc_sk(bc, sk))
		return 0;

<<<<<<< HEAD
	return inet_sk_diag_fill(sk, NULL, skb, req, NETLINK_CB(cb->skb).pid,
			cb->nlh->nlmsg_seq, NLM_F_MULTI, cb->nlh);
}

static int udp_dump_one(struct udp_table *tbl, struct sk_buff *in_skb,
		const struct nlmsghdr *nlh, struct inet_diag_req_v2 *req)
{
	int err = -EINVAL;
	struct sock *sk;
	struct sk_buff *rep;

	if (req->sdiag_family == AF_INET)
		sk = __udp4_lib_lookup(&init_net,
				req->id.idiag_src[0], req->id.idiag_sport,
				req->id.idiag_dst[0], req->id.idiag_dport,
				req->id.idiag_if, tbl);
#if IS_ENABLED(CONFIG_IPV6)
	else if (req->sdiag_family == AF_INET6)
		sk = __udp6_lib_lookup(&init_net,
=======
	return inet_sk_diag_fill(sk, NULL, skb, cb, req, NLM_F_MULTI,
				 net_admin);
}

static int udp_dump_one(struct udp_table *tbl,
			struct netlink_callback *cb,
			const struct inet_diag_req_v2 *req)
{
	struct sk_buff *in_skb = cb->skb;
	int err;
	struct sock *sk = NULL;
	struct sk_buff *rep;
	struct net *net = sock_net(in_skb->sk);

	rcu_read_lock();
	if (req->sdiag_family == AF_INET)
		/* src and dst are swapped for historical reasons */
		sk = __udp4_lib_lookup(net,
				req->id.idiag_src[0], req->id.idiag_sport,
				req->id.idiag_dst[0], req->id.idiag_dport,
				req->id.idiag_if, 0, tbl, NULL);
#if IS_ENABLED(CONFIG_IPV6)
	else if (req->sdiag_family == AF_INET6)
		sk = __udp6_lib_lookup(net,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				(struct in6_addr *)req->id.idiag_src,
				req->id.idiag_sport,
				(struct in6_addr *)req->id.idiag_dst,
				req->id.idiag_dport,
<<<<<<< HEAD
				req->id.idiag_if, tbl);
#endif
	else
		goto out_nosk;

	err = -ENOENT;
	if (sk == NULL)
=======
				req->id.idiag_if, 0, tbl, NULL);
#endif
	if (sk && !refcount_inc_not_zero(&sk->sk_refcnt))
		sk = NULL;
	rcu_read_unlock();
	err = -ENOENT;
	if (!sk)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out_nosk;

	err = sock_diag_check_cookie(sk, req->id.idiag_cookie);
	if (err)
		goto out;

	err = -ENOMEM;
<<<<<<< HEAD
	rep = alloc_skb(NLMSG_SPACE((sizeof(struct inet_diag_msg) +
				     sizeof(struct inet_diag_meminfo) +
				     64)), GFP_KERNEL);
	if (!rep)
		goto out;

	err = inet_sk_diag_fill(sk, NULL, rep, req,
			   NETLINK_CB(in_skb).pid,
			   nlh->nlmsg_seq, 0, nlh);
=======
	rep = nlmsg_new(nla_total_size(sizeof(struct inet_diag_msg)) +
			inet_diag_msg_attrs_size() +
			nla_total_size(sizeof(struct inet_diag_meminfo)) + 64,
			GFP_KERNEL);
	if (!rep)
		goto out;

	err = inet_sk_diag_fill(sk, NULL, rep, cb, req, 0,
				netlink_net_capable(in_skb, CAP_NET_ADMIN));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err < 0) {
		WARN_ON(err == -EMSGSIZE);
		kfree_skb(rep);
		goto out;
	}
<<<<<<< HEAD
	err = netlink_unicast(sock_diag_nlsk, rep, NETLINK_CB(in_skb).pid,
			      MSG_DONTWAIT);
	if (err > 0)
		err = 0;
=======
	err = nlmsg_unicast(net->diag_nlsk, rep, NETLINK_CB(in_skb).portid);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	if (sk)
		sock_put(sk);
out_nosk:
	return err;
}

<<<<<<< HEAD
static void udp_dump(struct udp_table *table, struct sk_buff *skb, struct netlink_callback *cb,
		struct inet_diag_req_v2 *r, struct nlattr *bc)
{
	int num, s_num, slot, s_slot;

	s_slot = cb->args[0];
	num = s_num = cb->args[1];

	for (slot = s_slot; slot <= table->mask; num = s_num = 0, slot++) {
		struct sock *sk;
		struct hlist_nulls_node *node;
		struct udp_hslot *hslot = &table->hash[slot];

		if (hlist_nulls_empty(&hslot->head))
			continue;

		spin_lock_bh(&hslot->lock);
		sk_nulls_for_each(sk, node, &hslot->head) {
			struct inet_sock *inet = inet_sk(sk);

=======
static void udp_dump(struct udp_table *table, struct sk_buff *skb,
		     struct netlink_callback *cb,
		     const struct inet_diag_req_v2 *r)
{
	bool net_admin = netlink_net_capable(cb->skb, CAP_NET_ADMIN);
	struct net *net = sock_net(skb->sk);
	struct inet_diag_dump_data *cb_data;
	int num, s_num, slot, s_slot;
	struct nlattr *bc;

	cb_data = cb->data;
	bc = cb_data->inet_diag_nla_bc;
	s_slot = cb->args[0];
	num = s_num = cb->args[1];

	for (slot = s_slot; slot <= table->mask; s_num = 0, slot++) {
		struct udp_hslot *hslot = &table->hash[slot];
		struct sock *sk;

		num = 0;

		if (hlist_empty(&hslot->head))
			continue;

		spin_lock_bh(&hslot->lock);
		sk_for_each(sk, &hslot->head) {
			struct inet_sock *inet = inet_sk(sk);

			if (!net_eq(sock_net(sk), net))
				continue;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (num < s_num)
				goto next;
			if (!(r->idiag_states & (1 << sk->sk_state)))
				goto next;
			if (r->sdiag_family != AF_UNSPEC &&
					sk->sk_family != r->sdiag_family)
				goto next;
			if (r->id.idiag_sport != inet->inet_sport &&
			    r->id.idiag_sport)
				goto next;
			if (r->id.idiag_dport != inet->inet_dport &&
			    r->id.idiag_dport)
				goto next;

<<<<<<< HEAD
			if (sk_diag_dump(sk, skb, cb, r, bc) < 0) {
=======
			if (sk_diag_dump(sk, skb, cb, r, bc, net_admin) < 0) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				spin_unlock_bh(&hslot->lock);
				goto done;
			}
next:
			num++;
		}
		spin_unlock_bh(&hslot->lock);
	}
done:
	cb->args[0] = slot;
	cb->args[1] = num;
}

static void udp_diag_dump(struct sk_buff *skb, struct netlink_callback *cb,
<<<<<<< HEAD
		struct inet_diag_req_v2 *r, struct nlattr *bc)
{
	udp_dump(&udp_table, skb, cb, r, bc);
}

static int udp_diag_dump_one(struct sk_buff *in_skb, const struct nlmsghdr *nlh,
		struct inet_diag_req_v2 *req)
{
	return udp_dump_one(&udp_table, in_skb, nlh, req);
=======
			  const struct inet_diag_req_v2 *r)
{
	udp_dump(sock_net(cb->skb->sk)->ipv4.udp_table, skb, cb, r);
}

static int udp_diag_dump_one(struct netlink_callback *cb,
			     const struct inet_diag_req_v2 *req)
{
	return udp_dump_one(sock_net(cb->skb->sk)->ipv4.udp_table, cb, req);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void udp_diag_get_info(struct sock *sk, struct inet_diag_msg *r,
		void *info)
{
<<<<<<< HEAD
	r->idiag_rqueue = sk_rmem_alloc_get(sk);
	r->idiag_wqueue = sk_wmem_alloc_get(sk);
}

static const struct inet_diag_handler udp_diag_handler = {
=======
	r->idiag_rqueue = udp_rqueue_get(sk);
	r->idiag_wqueue = sk_wmem_alloc_get(sk);
}

#ifdef CONFIG_INET_DIAG_DESTROY
static int __udp_diag_destroy(struct sk_buff *in_skb,
			      const struct inet_diag_req_v2 *req,
			      struct udp_table *tbl)
{
	struct net *net = sock_net(in_skb->sk);
	struct sock *sk;
	int err;

	rcu_read_lock();

	if (req->sdiag_family == AF_INET)
		sk = __udp4_lib_lookup(net,
				req->id.idiag_dst[0], req->id.idiag_dport,
				req->id.idiag_src[0], req->id.idiag_sport,
				req->id.idiag_if, 0, tbl, NULL);
#if IS_ENABLED(CONFIG_IPV6)
	else if (req->sdiag_family == AF_INET6) {
		if (ipv6_addr_v4mapped((struct in6_addr *)req->id.idiag_dst) &&
		    ipv6_addr_v4mapped((struct in6_addr *)req->id.idiag_src))
			sk = __udp4_lib_lookup(net,
					req->id.idiag_dst[3], req->id.idiag_dport,
					req->id.idiag_src[3], req->id.idiag_sport,
					req->id.idiag_if, 0, tbl, NULL);

		else
			sk = __udp6_lib_lookup(net,
					(struct in6_addr *)req->id.idiag_dst,
					req->id.idiag_dport,
					(struct in6_addr *)req->id.idiag_src,
					req->id.idiag_sport,
					req->id.idiag_if, 0, tbl, NULL);
	}
#endif
	else {
		rcu_read_unlock();
		return -EINVAL;
	}

	if (sk && !refcount_inc_not_zero(&sk->sk_refcnt))
		sk = NULL;

	rcu_read_unlock();

	if (!sk)
		return -ENOENT;

	if (sock_diag_check_cookie(sk, req->id.idiag_cookie)) {
		sock_put(sk);
		return -ENOENT;
	}

	err = sock_diag_destroy(sk, ECONNABORTED);

	sock_put(sk);

	return err;
}

static int udp_diag_destroy(struct sk_buff *in_skb,
			    const struct inet_diag_req_v2 *req)
{
	return __udp_diag_destroy(in_skb, req, sock_net(in_skb->sk)->ipv4.udp_table);
}

static int udplite_diag_destroy(struct sk_buff *in_skb,
				const struct inet_diag_req_v2 *req)
{
	return __udp_diag_destroy(in_skb, req, &udplite_table);
}

#endif

static const struct inet_diag_handler udp_diag_handler = {
	.owner		 = THIS_MODULE,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.dump		 = udp_diag_dump,
	.dump_one	 = udp_diag_dump_one,
	.idiag_get_info  = udp_diag_get_info,
	.idiag_type	 = IPPROTO_UDP,
<<<<<<< HEAD
};

static void udplite_diag_dump(struct sk_buff *skb, struct netlink_callback *cb,
		struct inet_diag_req_v2 *r, struct nlattr *bc)
{
	udp_dump(&udplite_table, skb, cb, r, bc);
}

static int udplite_diag_dump_one(struct sk_buff *in_skb, const struct nlmsghdr *nlh,
		struct inet_diag_req_v2 *req)
{
	return udp_dump_one(&udplite_table, in_skb, nlh, req);
}

static const struct inet_diag_handler udplite_diag_handler = {
=======
	.idiag_info_size = 0,
#ifdef CONFIG_INET_DIAG_DESTROY
	.destroy	 = udp_diag_destroy,
#endif
};

static void udplite_diag_dump(struct sk_buff *skb, struct netlink_callback *cb,
			      const struct inet_diag_req_v2 *r)
{
	udp_dump(&udplite_table, skb, cb, r);
}

static int udplite_diag_dump_one(struct netlink_callback *cb,
				 const struct inet_diag_req_v2 *req)
{
	return udp_dump_one(&udplite_table, cb, req);
}

static const struct inet_diag_handler udplite_diag_handler = {
	.owner		 = THIS_MODULE,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.dump		 = udplite_diag_dump,
	.dump_one	 = udplite_diag_dump_one,
	.idiag_get_info  = udp_diag_get_info,
	.idiag_type	 = IPPROTO_UDPLITE,
<<<<<<< HEAD
=======
	.idiag_info_size = 0,
#ifdef CONFIG_INET_DIAG_DESTROY
	.destroy	 = udplite_diag_destroy,
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static int __init udp_diag_init(void)
{
	int err;

	err = inet_diag_register(&udp_diag_handler);
	if (err)
		goto out;
	err = inet_diag_register(&udplite_diag_handler);
	if (err)
		goto out_lite;
out:
	return err;
out_lite:
	inet_diag_unregister(&udp_diag_handler);
	goto out;
}

static void __exit udp_diag_exit(void)
{
	inet_diag_unregister(&udplite_diag_handler);
	inet_diag_unregister(&udp_diag_handler);
}

module_init(udp_diag_init);
module_exit(udp_diag_exit);
MODULE_LICENSE("GPL");
<<<<<<< HEAD
=======
MODULE_DESCRIPTION("UDP socket monitoring via SOCK_DIAG");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_ALIAS_NET_PF_PROTO_TYPE(PF_NETLINK, NETLINK_SOCK_DIAG, 2-17 /* AF_INET - IPPROTO_UDP */);
MODULE_ALIAS_NET_PF_PROTO_TYPE(PF_NETLINK, NETLINK_SOCK_DIAG, 2-136 /* AF_INET - IPPROTO_UDPLITE */);
