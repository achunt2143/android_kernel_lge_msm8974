<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *	common UDP/RAW code
 *	Linux INET implementation
 *
 * Authors:
 * 	Hideaki YOSHIFUJI <yoshfuji@linux-ipv6.org>
<<<<<<< HEAD
 *
 * 	This program is free software; you can redistribute it and/or
 * 	modify it under the terms of the GNU General Public License
 * 	as published by the Free Software Foundation; either version
 * 	2 of the License, or (at your option) any later version.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/types.h>
#include <linux/module.h>
<<<<<<< HEAD
#include <linux/ip.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/in.h>
#include <net/ip.h>
#include <net/sock.h>
#include <net/route.h>
#include <net/tcp_states.h>
<<<<<<< HEAD

int ip4_datagram_connect(struct sock *sk, struct sockaddr *uaddr, int addr_len)
=======
#include <net/sock_reuseport.h>

int __ip4_datagram_connect(struct sock *sk, struct sockaddr *uaddr, int addr_len)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct inet_sock *inet = inet_sk(sk);
	struct sockaddr_in *usin = (struct sockaddr_in *) uaddr;
	struct flowi4 *fl4;
	struct rtable *rt;
	__be32 saddr;
	int oif;
	int err;


	if (addr_len < sizeof(*usin))
		return -EINVAL;

	if (usin->sin_family != AF_INET)
		return -EAFNOSUPPORT;

	sk_dst_reset(sk);

<<<<<<< HEAD
	lock_sock(sk);

	oif = sk->sk_bound_dev_if;
	saddr = inet->inet_saddr;
	if (ipv4_is_multicast(usin->sin_addr.s_addr)) {
		if (!oif)
			oif = inet->mc_index;
		if (!saddr)
			saddr = inet->mc_addr;
	}
	fl4 = &inet->cork.fl.u.ip4;
	rt = ip_route_connect(fl4, usin->sin_addr.s_addr, saddr,
			      RT_CONN_FLAGS(sk), oif,
			      sk->sk_protocol,
			      inet->inet_sport, usin->sin_port, sk, true);
=======
	oif = sk->sk_bound_dev_if;
	saddr = inet->inet_saddr;
	if (ipv4_is_multicast(usin->sin_addr.s_addr)) {
		if (!oif || netif_index_is_l3_master(sock_net(sk), oif))
			oif = READ_ONCE(inet->mc_index);
		if (!saddr)
			saddr = READ_ONCE(inet->mc_addr);
	} else if (!oif) {
		oif = READ_ONCE(inet->uc_index);
	}
	fl4 = &inet->cork.fl.u.ip4;
	rt = ip_route_connect(fl4, usin->sin_addr.s_addr, saddr, oif,
			      sk->sk_protocol, inet->inet_sport,
			      usin->sin_port, sk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (IS_ERR(rt)) {
		err = PTR_ERR(rt);
		if (err == -ENETUNREACH)
			IP_INC_STATS(sock_net(sk), IPSTATS_MIB_OUTNOROUTES);
		goto out;
	}

	if ((rt->rt_flags & RTCF_BROADCAST) && !sock_flag(sk, SOCK_BROADCAST)) {
		ip_rt_put(rt);
		err = -EACCES;
		goto out;
	}
	if (!inet->inet_saddr)
		inet->inet_saddr = fl4->saddr;	/* Update source address */
	if (!inet->inet_rcv_saddr) {
		inet->inet_rcv_saddr = fl4->saddr;
		if (sk->sk_prot->rehash)
			sk->sk_prot->rehash(sk);
	}
	inet->inet_daddr = fl4->daddr;
	inet->inet_dport = usin->sin_port;
<<<<<<< HEAD
	sk->sk_state = TCP_ESTABLISHED;
	inet->inet_id = jiffies;
=======
	reuseport_has_conns_set(sk);
	sk->sk_state = TCP_ESTABLISHED;
	sk_set_txhash(sk);
	atomic_set(&inet->inet_id, get_random_u16());
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	sk_dst_set(sk, &rt->dst);
	err = 0;
out:
<<<<<<< HEAD
	release_sock(sk);
	return err;
}
EXPORT_SYMBOL(ip4_datagram_connect);
=======
	return err;
}
EXPORT_SYMBOL(__ip4_datagram_connect);

int ip4_datagram_connect(struct sock *sk, struct sockaddr *uaddr, int addr_len)
{
	int res;

	lock_sock(sk);
	res = __ip4_datagram_connect(sk, uaddr, addr_len);
	release_sock(sk);
	return res;
}
EXPORT_SYMBOL(ip4_datagram_connect);

/* Because UDP xmit path can manipulate sk_dst_cache without holding
 * socket lock, we need to use sk_dst_set() here,
 * even if we own the socket lock.
 */
void ip4_datagram_release_cb(struct sock *sk)
{
	const struct inet_sock *inet = inet_sk(sk);
	const struct ip_options_rcu *inet_opt;
	__be32 daddr = inet->inet_daddr;
	struct dst_entry *dst;
	struct flowi4 fl4;
	struct rtable *rt;

	rcu_read_lock();

	dst = __sk_dst_get(sk);
	if (!dst || !dst->obsolete || dst->ops->check(dst, 0)) {
		rcu_read_unlock();
		return;
	}
	inet_opt = rcu_dereference(inet->inet_opt);
	if (inet_opt && inet_opt->opt.srr)
		daddr = inet_opt->opt.faddr;
	rt = ip_route_output_ports(sock_net(sk), &fl4, sk, daddr,
				   inet->inet_saddr, inet->inet_dport,
				   inet->inet_sport, sk->sk_protocol,
				   ip_sock_rt_tos(sk), sk->sk_bound_dev_if);

	dst = !IS_ERR(rt) ? &rt->dst : NULL;
	sk_dst_set(sk, dst);

	rcu_read_unlock();
}
EXPORT_SYMBOL_GPL(ip4_datagram_release_cb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
