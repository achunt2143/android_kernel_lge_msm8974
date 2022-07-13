/*
<<<<<<< HEAD
 * Copyright (c) 2006 Oracle.  All rights reserved.
=======
 * Copyright (c) 2006, 2019 Oracle and/or its affiliates. All rights reserved.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */
#include <linux/kernel.h>
#include <net/sock.h>
#include <linux/in.h>
<<<<<<< HEAD
=======
#include <linux/ipv6.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/if_arp.h>
#include <linux/jhash.h>
#include <linux/ratelimit.h>
#include "rds.h"

<<<<<<< HEAD
#define BIND_HASH_SIZE 1024
static struct hlist_head bind_hash_table[BIND_HASH_SIZE];
static DEFINE_SPINLOCK(rds_bind_lock);

static struct hlist_head *hash_to_bucket(__be32 addr, __be16 port)
{
	return bind_hash_table + (jhash_2words((u32)addr, (u32)port, 0) &
				  (BIND_HASH_SIZE - 1));
}

static struct rds_sock *rds_bind_lookup(__be32 addr, __be16 port,
					struct rds_sock *insert)
{
	struct rds_sock *rs;
	struct hlist_node *node;
	struct hlist_head *head = hash_to_bucket(addr, port);
	u64 cmp;
	u64 needle = ((u64)be32_to_cpu(addr) << 32) | be16_to_cpu(port);

	rcu_read_lock();
	hlist_for_each_entry_rcu(rs, node, head, rs_bound_node) {
		cmp = ((u64)be32_to_cpu(rs->rs_bound_addr) << 32) |
		      be16_to_cpu(rs->rs_bound_port);

		if (cmp == needle) {
			rcu_read_unlock();
			return rs;
		}
	}
	rcu_read_unlock();

	if (insert) {
		/*
		 * make sure our addr and port are set before
		 * we are added to the list, other people
		 * in rcu will find us as soon as the
		 * hlist_add_head_rcu is done
		 */
		insert->rs_bound_addr = addr;
		insert->rs_bound_port = port;
		rds_sock_addref(insert);

		hlist_add_head_rcu(&insert->rs_bound_node, head);
	}
	return NULL;
=======
static struct rhashtable bind_hash_table;

static const struct rhashtable_params ht_parms = {
	.nelem_hint = 768,
	.key_len = RDS_BOUND_KEY_LEN,
	.key_offset = offsetof(struct rds_sock, rs_bound_key),
	.head_offset = offsetof(struct rds_sock, rs_bound_node),
	.max_size = 16384,
	.min_size = 1024,
};

/* Create a key for the bind hash table manipulation.  Port is in network byte
 * order.
 */
static inline void __rds_create_bind_key(u8 *key, const struct in6_addr *addr,
					 __be16 port, __u32 scope_id)
{
	memcpy(key, addr, sizeof(*addr));
	key += sizeof(*addr);
	memcpy(key, &port, sizeof(port));
	key += sizeof(port);
	memcpy(key, &scope_id, sizeof(scope_id));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Return the rds_sock bound at the given local address.
 *
 * The rx path can race with rds_release.  We notice if rds_release() has
 * marked this socket and don't return a rs ref to the rx path.
 */
<<<<<<< HEAD
struct rds_sock *rds_find_bound(__be32 addr, __be16 port)
{
	struct rds_sock *rs;

	rs = rds_bind_lookup(addr, port, NULL);

	if (rs && !sock_flag(rds_rs_to_sk(rs), SOCK_DEAD))
		rds_sock_addref(rs);
	else
		rs = NULL;

	rdsdebug("returning rs %p for %pI4:%u\n", rs, &addr,
		ntohs(port));
=======
struct rds_sock *rds_find_bound(const struct in6_addr *addr, __be16 port,
				__u32 scope_id)
{
	u8 key[RDS_BOUND_KEY_LEN];
	struct rds_sock *rs;

	__rds_create_bind_key(key, addr, port, scope_id);
	rcu_read_lock();
	rs = rhashtable_lookup(&bind_hash_table, key, ht_parms);
	if (rs && (sock_flag(rds_rs_to_sk(rs), SOCK_DEAD) ||
		   !refcount_inc_not_zero(&rds_rs_to_sk(rs)->sk_refcnt)))
		rs = NULL;

	rcu_read_unlock();

	rdsdebug("returning rs %p for %pI6c:%u\n", rs, addr,
		 ntohs(port));

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return rs;
}

/* returns -ve errno or +ve port */
<<<<<<< HEAD
static int rds_add_bound(struct rds_sock *rs, __be32 addr, __be16 *port)
{
	unsigned long flags;
	int ret = -EADDRINUSE;
	u16 rover, last;

	if (*port != 0) {
		rover = be16_to_cpu(*port);
		last = rover;
	} else {
		rover = max_t(u16, net_random(), 2);
		last = rover - 1;
	}

	spin_lock_irqsave(&rds_bind_lock, flags);

	do {
		if (rover == 0)
			rover++;
		if (!rds_bind_lookup(addr, cpu_to_be16(rover), rs)) {
			*port = rs->rs_bound_port;
			ret = 0;
			rdsdebug("rs %p binding to %pI4:%d\n",
			  rs, &addr, (int)ntohs(*port));
=======
static int rds_add_bound(struct rds_sock *rs, const struct in6_addr *addr,
			 __be16 *port, __u32 scope_id)
{
	int ret = -EADDRINUSE;
	u16 rover, last;
	u8 key[RDS_BOUND_KEY_LEN];

	if (*port != 0) {
		rover = be16_to_cpu(*port);
		if (rover == RDS_FLAG_PROBE_PORT)
			return -EINVAL;
		last = rover;
	} else {
		rover = max_t(u16, get_random_u16(), 2);
		last = rover - 1;
	}

	do {
		if (rover == 0)
			rover++;

		if (rover == RDS_FLAG_PROBE_PORT)
			continue;
		__rds_create_bind_key(key, addr, cpu_to_be16(rover),
				      scope_id);
		if (rhashtable_lookup_fast(&bind_hash_table, key, ht_parms))
			continue;

		memcpy(rs->rs_bound_key, key, sizeof(rs->rs_bound_key));
		rs->rs_bound_addr = *addr;
		net_get_random_once(&rs->rs_hash_initval,
				    sizeof(rs->rs_hash_initval));
		rs->rs_bound_port = cpu_to_be16(rover);
		rs->rs_bound_node.next = NULL;
		rds_sock_addref(rs);
		if (!rhashtable_insert_fast(&bind_hash_table,
					    &rs->rs_bound_node, ht_parms)) {
			*port = rs->rs_bound_port;
			rs->rs_bound_scope_id = scope_id;
			ret = 0;
			rdsdebug("rs %p binding to %pI6c:%d\n",
				 rs, addr, (int)ntohs(*port));
			break;
		} else {
			rs->rs_bound_addr = in6addr_any;
			rds_sock_put(rs);
			ret = -ENOMEM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		}
	} while (rover++ != last);

<<<<<<< HEAD
	spin_unlock_irqrestore(&rds_bind_lock, flags);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

void rds_remove_bound(struct rds_sock *rs)
{
<<<<<<< HEAD
	unsigned long flags;

	spin_lock_irqsave(&rds_bind_lock, flags);

	if (rs->rs_bound_addr) {
		rdsdebug("rs %p unbinding from %pI4:%d\n",
		  rs, &rs->rs_bound_addr,
		  ntohs(rs->rs_bound_port));

		hlist_del_init_rcu(&rs->rs_bound_node);
		rds_sock_put(rs);
		rs->rs_bound_addr = 0;
	}

	spin_unlock_irqrestore(&rds_bind_lock, flags);
=======

	if (ipv6_addr_any(&rs->rs_bound_addr))
		return;

	rdsdebug("rs %p unbinding from %pI6c:%d\n",
		 rs, &rs->rs_bound_addr,
		 ntohs(rs->rs_bound_port));

	rhashtable_remove_fast(&bind_hash_table, &rs->rs_bound_node, ht_parms);
	rds_sock_put(rs);
	rs->rs_bound_addr = in6addr_any;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int rds_bind(struct socket *sock, struct sockaddr *uaddr, int addr_len)
{
	struct sock *sk = sock->sk;
<<<<<<< HEAD
	struct sockaddr_in *sin = (struct sockaddr_in *)uaddr;
	struct rds_sock *rs = rds_sk_to_rs(sk);
	struct rds_transport *trans;
	int ret = 0;

	lock_sock(sk);

	if (addr_len != sizeof(struct sockaddr_in) ||
	    sin->sin_family != AF_INET ||
	    rs->rs_bound_addr ||
	    sin->sin_addr.s_addr == htonl(INADDR_ANY)) {
=======
	struct rds_sock *rs = rds_sk_to_rs(sk);
	struct in6_addr v6addr, *binding_addr;
	struct rds_transport *trans;
	__u32 scope_id = 0;
	int ret = 0;
	__be16 port;

	/* We allow an RDS socket to be bound to either IPv4 or IPv6
	 * address.
	 */
	if (addr_len < offsetofend(struct sockaddr, sa_family))
		return -EINVAL;
	if (uaddr->sa_family == AF_INET) {
		struct sockaddr_in *sin = (struct sockaddr_in *)uaddr;

		if (addr_len < sizeof(struct sockaddr_in) ||
		    sin->sin_addr.s_addr == htonl(INADDR_ANY) ||
		    sin->sin_addr.s_addr == htonl(INADDR_BROADCAST) ||
		    ipv4_is_multicast(sin->sin_addr.s_addr))
			return -EINVAL;
		ipv6_addr_set_v4mapped(sin->sin_addr.s_addr, &v6addr);
		binding_addr = &v6addr;
		port = sin->sin_port;
#if IS_ENABLED(CONFIG_IPV6)
	} else if (uaddr->sa_family == AF_INET6) {
		struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *)uaddr;
		int addr_type;

		if (addr_len < sizeof(struct sockaddr_in6))
			return -EINVAL;
		addr_type = ipv6_addr_type(&sin6->sin6_addr);
		if (!(addr_type & IPV6_ADDR_UNICAST)) {
			__be32 addr4;

			if (!(addr_type & IPV6_ADDR_MAPPED))
				return -EINVAL;

			/* It is a mapped address.  Need to do some sanity
			 * checks.
			 */
			addr4 = sin6->sin6_addr.s6_addr32[3];
			if (addr4 == htonl(INADDR_ANY) ||
			    addr4 == htonl(INADDR_BROADCAST) ||
			    ipv4_is_multicast(addr4))
				return -EINVAL;
		}
		/* The scope ID must be specified for link local address. */
		if (addr_type & IPV6_ADDR_LINKLOCAL) {
			if (sin6->sin6_scope_id == 0)
				return -EINVAL;
			scope_id = sin6->sin6_scope_id;
		}
		binding_addr = &sin6->sin6_addr;
		port = sin6->sin6_port;
#endif
	} else {
		return -EINVAL;
	}
	lock_sock(sk);

	/* RDS socket does not allow re-binding. */
	if (!ipv6_addr_any(&rs->rs_bound_addr)) {
		ret = -EINVAL;
		goto out;
	}
	/* Socket is connected.  The binding address should have the same
	 * scope ID as the connected address, except the case when one is
	 * non-link local address (scope_id is 0).
	 */
	if (!ipv6_addr_any(&rs->rs_conn_addr) && scope_id &&
	    rs->rs_bound_scope_id &&
	    scope_id != rs->rs_bound_scope_id) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = -EINVAL;
		goto out;
	}

<<<<<<< HEAD
	ret = rds_add_bound(rs, sin->sin_addr.s_addr, &sin->sin_port);
	if (ret)
		goto out;

	trans = rds_trans_get_preferred(sin->sin_addr.s_addr);
	if (!trans) {
		ret = -EADDRNOTAVAIL;
		rds_remove_bound(rs);
		printk_ratelimited(KERN_INFO "RDS: rds_bind() could not find a transport, "
				"load rds_tcp or rds_rdma?\n");
		goto out;
	}

	rs->rs_transport = trans;
	ret = 0;

out:
	release_sock(sk);

	/* we might have called rds_remove_bound on error */
	if (ret)
		synchronize_rcu();
	return ret;
}
=======
	/* The transport can be set using SO_RDS_TRANSPORT option before the
	 * socket is bound.
	 */
	if (rs->rs_transport) {
		trans = rs->rs_transport;
		if (!trans->laddr_check ||
		    trans->laddr_check(sock_net(sock->sk),
				       binding_addr, scope_id) != 0) {
			ret = -ENOPROTOOPT;
			goto out;
		}
	} else {
		trans = rds_trans_get_preferred(sock_net(sock->sk),
						binding_addr, scope_id);
		if (!trans) {
			ret = -EADDRNOTAVAIL;
			pr_info_ratelimited("RDS: %s could not find a transport for %pI6c, load rds_tcp or rds_rdma?\n",
					    __func__, binding_addr);
			goto out;
		}
		rs->rs_transport = trans;
	}

	sock_set_flag(sk, SOCK_RCU_FREE);
	ret = rds_add_bound(rs, binding_addr, &port, scope_id);
	if (ret)
		rs->rs_transport = NULL;

out:
	release_sock(sk);
	return ret;
}

void rds_bind_lock_destroy(void)
{
	rhashtable_destroy(&bind_hash_table);
}

int rds_bind_lock_init(void)
{
	return rhashtable_init(&bind_hash_table, &ht_parms);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
