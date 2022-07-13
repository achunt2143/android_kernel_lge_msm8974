<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * INET		An implementation of the TCP/IP protocol suite for the LINUX
 *		operating system.  INET is implemented using the  BSD Socket
 *		interface as the means of communication with the user level.
 *
 *		Support for INET connection oriented protocols.
 *
 * Authors:	See the TCP sources
<<<<<<< HEAD
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or(at your option) any later version.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/module.h>
#include <linux/jhash.h>

#include <net/inet_connection_sock.h>
#include <net/inet_hashtables.h>
#include <net/inet_timewait_sock.h>
#include <net/ip.h>
#include <net/route.h>
#include <net/tcp_states.h>
#include <net/xfrm.h>
<<<<<<< HEAD

#ifdef INET_CSK_DEBUG
const char inet_csk_timer_bug_msg[] = "inet_csk BUG: unknown timer value\n";
EXPORT_SYMBOL(inet_csk_timer_bug_msg);
#endif

/*
 * This struct holds the first and last local port number.
 */
struct local_ports sysctl_local_ports __read_mostly = {
	.lock = __SEQLOCK_UNLOCKED(sysctl_local_ports.lock),
	.range = { 32768, 61000 },
};

unsigned long *sysctl_local_reserved_ports;
EXPORT_SYMBOL(sysctl_local_reserved_ports);

void inet_get_local_port_range(int *low, int *high)
{
	unsigned seq;
	do {
		seq = read_seqbegin(&sysctl_local_ports.lock);

		*low = sysctl_local_ports.range[0];
		*high = sysctl_local_ports.range[1];
	} while (read_seqretry(&sysctl_local_ports.lock, seq));
}
EXPORT_SYMBOL(inet_get_local_port_range);

int inet_csk_bind_conflict(const struct sock *sk,
			   const struct inet_bind_bucket *tb)
{
	struct sock *sk2;
	struct hlist_node *node;
	int reuse = sk->sk_reuse;

	/*
	 * Unlike other sk lookup places we do not check
	 * for sk_net here, since _all_ the socks listed
	 * in tb->owners list belong to the same net - the
	 * one this bucket belongs to.
	 */

	sk_for_each_bound(sk2, node, &tb->owners) {
		if (sk != sk2 &&
		    !inet_v6_ipv6only(sk2) &&
		    (!sk->sk_bound_dev_if ||
		     !sk2->sk_bound_dev_if ||
		     sk->sk_bound_dev_if == sk2->sk_bound_dev_if)) {
			if (!reuse || !sk2->sk_reuse ||
			    sk2->sk_state == TCP_LISTEN) {
				const __be32 sk2_rcv_saddr = sk_rcv_saddr(sk2);
				if (!sk2_rcv_saddr || !sk_rcv_saddr(sk) ||
				    sk2_rcv_saddr == sk_rcv_saddr(sk))
					break;
			}
		}
	}
	return node != NULL;
}
EXPORT_SYMBOL_GPL(inet_csk_bind_conflict);

/* Obtain a reference to a local port for the given sock,
 * if snum is zero it means select any available local port.
 */
int inet_csk_get_port(struct sock *sk, unsigned short snum)
{
	struct inet_hashinfo *hashinfo = sk->sk_prot->h.hashinfo;
	struct inet_bind_hashbucket *head;
	struct hlist_node *node;
	struct inet_bind_bucket *tb;
	int ret, attempts = 5;
	struct net *net = sock_net(sk);
	int smallest_size = -1, smallest_rover;

	local_bh_disable();
	if (!snum) {
		int remaining, rover, low, high;

again:
		inet_get_local_port_range(&low, &high);
		remaining = (high - low) + 1;
		smallest_rover = rover = net_random() % remaining + low;

		smallest_size = -1;
		do {
			if (inet_is_reserved_local_port(rover))
				goto next_nolock;
			head = &hashinfo->bhash[inet_bhashfn(net, rover,
					hashinfo->bhash_size)];
			spin_lock(&head->lock);
			inet_bind_bucket_for_each(tb, node, &head->chain)
				if (net_eq(ib_net(tb), net) && tb->port == rover) {
					if (tb->fastreuse > 0 &&
					    sk->sk_reuse &&
					    sk->sk_state != TCP_LISTEN &&
					    (tb->num_owners < smallest_size || smallest_size == -1)) {
						smallest_size = tb->num_owners;
						smallest_rover = rover;
						if (atomic_read(&hashinfo->bsockets) > (high - low) + 1) {
							snum = smallest_rover;
							goto tb_found;
						}
					}
					if (!inet_csk(sk)->icsk_af_ops->bind_conflict(sk, tb)) {
						snum = rover;
						goto tb_found;
					}
					goto next;
				}
			break;
		next:
			spin_unlock(&head->lock);
		next_nolock:
			if (++rover > high)
				rover = low;
		} while (--remaining > 0);

		/* Exhausted local port range during search?  It is not
		 * possible for us to be holding one of the bind hash
		 * locks if this test triggers, because if 'remaining'
		 * drops to zero, we broke out of the do/while loop at
		 * the top level, not from the 'break;' statement.
		 */
		ret = 1;
		if (remaining <= 0) {
			if (smallest_size != -1) {
				snum = smallest_rover;
				goto have_snum;
			}
			goto fail;
		}
		/* OK, here is the one we will use.  HEAD is
		 * non-NULL and we hold it's mutex.
		 */
		snum = rover;
	} else {
have_snum:
		head = &hashinfo->bhash[inet_bhashfn(net, snum,
				hashinfo->bhash_size)];
		spin_lock(&head->lock);
		inet_bind_bucket_for_each(tb, node, &head->chain)
			if (net_eq(ib_net(tb), net) && tb->port == snum)
				goto tb_found;
	}
	tb = NULL;
	goto tb_not_found;
tb_found:
	if (!hlist_empty(&tb->owners)) {
		if (tb->fastreuse > 0 &&
		    sk->sk_reuse && sk->sk_state != TCP_LISTEN &&
		    smallest_size == -1) {
			goto success;
		} else {
			ret = 1;
			if (inet_csk(sk)->icsk_af_ops->bind_conflict(sk, tb)) {
				if (sk->sk_reuse && sk->sk_state != TCP_LISTEN &&
				    smallest_size != -1 && --attempts >= 0) {
					spin_unlock(&head->lock);
					goto again;
				}
				goto fail_unlock;
			}
		}
	}
tb_not_found:
	ret = 1;
	if (!tb && (tb = inet_bind_bucket_create(hashinfo->bind_bucket_cachep,
					net, head, snum)) == NULL)
		goto fail_unlock;
	if (hlist_empty(&tb->owners)) {
		if (sk->sk_reuse && sk->sk_state != TCP_LISTEN)
			tb->fastreuse = 1;
		else
			tb->fastreuse = 0;
	} else if (tb->fastreuse &&
		   (!sk->sk_reuse || sk->sk_state == TCP_LISTEN))
		tb->fastreuse = 0;
success:
	if (!inet_csk(sk)->icsk_bind_hash)
		inet_bind_hash(sk, tb, snum);
	WARN_ON(inet_csk(sk)->icsk_bind_hash != tb);
	ret = 0;

fail_unlock:
	spin_unlock(&head->lock);
fail:
	local_bh_enable();
=======
#include <net/tcp.h>
#include <net/sock_reuseport.h>
#include <net/addrconf.h>

#if IS_ENABLED(CONFIG_IPV6)
/* match_sk*_wildcard == true:  IPV6_ADDR_ANY equals to any IPv6 addresses
 *				if IPv6 only, and any IPv4 addresses
 *				if not IPv6 only
 * match_sk*_wildcard == false: addresses must be exactly the same, i.e.
 *				IPV6_ADDR_ANY only equals to IPV6_ADDR_ANY,
 *				and 0.0.0.0 equals to 0.0.0.0 only
 */
static bool ipv6_rcv_saddr_equal(const struct in6_addr *sk1_rcv_saddr6,
				 const struct in6_addr *sk2_rcv_saddr6,
				 __be32 sk1_rcv_saddr, __be32 sk2_rcv_saddr,
				 bool sk1_ipv6only, bool sk2_ipv6only,
				 bool match_sk1_wildcard,
				 bool match_sk2_wildcard)
{
	int addr_type = ipv6_addr_type(sk1_rcv_saddr6);
	int addr_type2 = sk2_rcv_saddr6 ? ipv6_addr_type(sk2_rcv_saddr6) : IPV6_ADDR_MAPPED;

	/* if both are mapped, treat as IPv4 */
	if (addr_type == IPV6_ADDR_MAPPED && addr_type2 == IPV6_ADDR_MAPPED) {
		if (!sk2_ipv6only) {
			if (sk1_rcv_saddr == sk2_rcv_saddr)
				return true;
			return (match_sk1_wildcard && !sk1_rcv_saddr) ||
				(match_sk2_wildcard && !sk2_rcv_saddr);
		}
		return false;
	}

	if (addr_type == IPV6_ADDR_ANY && addr_type2 == IPV6_ADDR_ANY)
		return true;

	if (addr_type2 == IPV6_ADDR_ANY && match_sk2_wildcard &&
	    !(sk2_ipv6only && addr_type == IPV6_ADDR_MAPPED))
		return true;

	if (addr_type == IPV6_ADDR_ANY && match_sk1_wildcard &&
	    !(sk1_ipv6only && addr_type2 == IPV6_ADDR_MAPPED))
		return true;

	if (sk2_rcv_saddr6 &&
	    ipv6_addr_equal(sk1_rcv_saddr6, sk2_rcv_saddr6))
		return true;

	return false;
}
#endif

/* match_sk*_wildcard == true:  0.0.0.0 equals to any IPv4 addresses
 * match_sk*_wildcard == false: addresses must be exactly the same, i.e.
 *				0.0.0.0 only equals to 0.0.0.0
 */
static bool ipv4_rcv_saddr_equal(__be32 sk1_rcv_saddr, __be32 sk2_rcv_saddr,
				 bool sk2_ipv6only, bool match_sk1_wildcard,
				 bool match_sk2_wildcard)
{
	if (!sk2_ipv6only) {
		if (sk1_rcv_saddr == sk2_rcv_saddr)
			return true;
		return (match_sk1_wildcard && !sk1_rcv_saddr) ||
			(match_sk2_wildcard && !sk2_rcv_saddr);
	}
	return false;
}

bool inet_rcv_saddr_equal(const struct sock *sk, const struct sock *sk2,
			  bool match_wildcard)
{
#if IS_ENABLED(CONFIG_IPV6)
	if (sk->sk_family == AF_INET6)
		return ipv6_rcv_saddr_equal(&sk->sk_v6_rcv_saddr,
					    inet6_rcv_saddr(sk2),
					    sk->sk_rcv_saddr,
					    sk2->sk_rcv_saddr,
					    ipv6_only_sock(sk),
					    ipv6_only_sock(sk2),
					    match_wildcard,
					    match_wildcard);
#endif
	return ipv4_rcv_saddr_equal(sk->sk_rcv_saddr, sk2->sk_rcv_saddr,
				    ipv6_only_sock(sk2), match_wildcard,
				    match_wildcard);
}
EXPORT_SYMBOL(inet_rcv_saddr_equal);

bool inet_rcv_saddr_any(const struct sock *sk)
{
#if IS_ENABLED(CONFIG_IPV6)
	if (sk->sk_family == AF_INET6)
		return ipv6_addr_any(&sk->sk_v6_rcv_saddr);
#endif
	return !sk->sk_rcv_saddr;
}

/**
 *	inet_sk_get_local_port_range - fetch ephemeral ports range
 *	@sk: socket
 *	@low: pointer to low port
 *	@high: pointer to high port
 *
 *	Fetch netns port range (/proc/sys/net/ipv4/ip_local_port_range)
 *	Range can be overridden if socket got IP_LOCAL_PORT_RANGE option.
 *	Returns true if IP_LOCAL_PORT_RANGE was set on this socket.
 */
bool inet_sk_get_local_port_range(const struct sock *sk, int *low, int *high)
{
	int lo, hi, sk_lo, sk_hi;
	bool local_range = false;
	u32 sk_range;

	inet_get_local_port_range(sock_net(sk), &lo, &hi);

	sk_range = READ_ONCE(inet_sk(sk)->local_port_range);
	if (unlikely(sk_range)) {
		sk_lo = sk_range & 0xffff;
		sk_hi = sk_range >> 16;

		if (lo <= sk_lo && sk_lo <= hi)
			lo = sk_lo;
		if (lo <= sk_hi && sk_hi <= hi)
			hi = sk_hi;
		local_range = true;
	}

	*low = lo;
	*high = hi;
	return local_range;
}
EXPORT_SYMBOL(inet_sk_get_local_port_range);

static bool inet_use_bhash2_on_bind(const struct sock *sk)
{
#if IS_ENABLED(CONFIG_IPV6)
	if (sk->sk_family == AF_INET6) {
		int addr_type = ipv6_addr_type(&sk->sk_v6_rcv_saddr);

		if (addr_type == IPV6_ADDR_ANY)
			return false;

		if (addr_type != IPV6_ADDR_MAPPED)
			return true;
	}
#endif
	return sk->sk_rcv_saddr != htonl(INADDR_ANY);
}

static bool inet_bind_conflict(const struct sock *sk, struct sock *sk2,
			       kuid_t sk_uid, bool relax,
			       bool reuseport_cb_ok, bool reuseport_ok)
{
	int bound_dev_if2;

	if (sk == sk2)
		return false;

	bound_dev_if2 = READ_ONCE(sk2->sk_bound_dev_if);

	if (!sk->sk_bound_dev_if || !bound_dev_if2 ||
	    sk->sk_bound_dev_if == bound_dev_if2) {
		if (sk->sk_reuse && sk2->sk_reuse &&
		    sk2->sk_state != TCP_LISTEN) {
			if (!relax || (!reuseport_ok && sk->sk_reuseport &&
				       sk2->sk_reuseport && reuseport_cb_ok &&
				       (sk2->sk_state == TCP_TIME_WAIT ||
					uid_eq(sk_uid, sock_i_uid(sk2)))))
				return true;
		} else if (!reuseport_ok || !sk->sk_reuseport ||
			   !sk2->sk_reuseport || !reuseport_cb_ok ||
			   (sk2->sk_state != TCP_TIME_WAIT &&
			    !uid_eq(sk_uid, sock_i_uid(sk2)))) {
			return true;
		}
	}
	return false;
}

static bool __inet_bhash2_conflict(const struct sock *sk, struct sock *sk2,
				   kuid_t sk_uid, bool relax,
				   bool reuseport_cb_ok, bool reuseport_ok)
{
	if (ipv6_only_sock(sk2)) {
		if (sk->sk_family == AF_INET)
			return false;

#if IS_ENABLED(CONFIG_IPV6)
		if (ipv6_addr_v4mapped(&sk->sk_v6_rcv_saddr))
			return false;
#endif
	}

	return inet_bind_conflict(sk, sk2, sk_uid, relax,
				  reuseport_cb_ok, reuseport_ok);
}

static bool inet_bhash2_conflict(const struct sock *sk,
				 const struct inet_bind2_bucket *tb2,
				 kuid_t sk_uid,
				 bool relax, bool reuseport_cb_ok,
				 bool reuseport_ok)
{
	struct sock *sk2;

	sk_for_each_bound(sk2, &tb2->owners) {
		if (__inet_bhash2_conflict(sk, sk2, sk_uid, relax,
					   reuseport_cb_ok, reuseport_ok))
			return true;
	}

	return false;
}

#define sk_for_each_bound_bhash(__sk, __tb2, __tb)			\
	hlist_for_each_entry(__tb2, &(__tb)->bhash2, bhash_node)	\
		sk_for_each_bound(sk2, &(__tb2)->owners)

/* This should be called only when the tb and tb2 hashbuckets' locks are held */
static int inet_csk_bind_conflict(const struct sock *sk,
				  const struct inet_bind_bucket *tb,
				  const struct inet_bind2_bucket *tb2, /* may be null */
				  bool relax, bool reuseport_ok)
{
	kuid_t uid = sock_i_uid((struct sock *)sk);
	struct sock_reuseport *reuseport_cb;
	bool reuseport_cb_ok;
	struct sock *sk2;

	rcu_read_lock();
	reuseport_cb = rcu_dereference(sk->sk_reuseport_cb);
	/* paired with WRITE_ONCE() in __reuseport_(add|detach)_closed_sock */
	reuseport_cb_ok = !reuseport_cb || READ_ONCE(reuseport_cb->num_closed_socks);
	rcu_read_unlock();

	/* Conflicts with an existing IPV6_ADDR_ANY (if ipv6) or INADDR_ANY (if
	 * ipv4) should have been checked already. We need to do these two
	 * checks separately because their spinlocks have to be acquired/released
	 * independently of each other, to prevent possible deadlocks
	 */
	if (inet_use_bhash2_on_bind(sk))
		return tb2 && inet_bhash2_conflict(sk, tb2, uid, relax,
						   reuseport_cb_ok, reuseport_ok);

	/* Unlike other sk lookup places we do not check
	 * for sk_net here, since _all_ the socks listed
	 * in tb->owners and tb2->owners list belong
	 * to the same net - the one this bucket belongs to.
	 */
	sk_for_each_bound_bhash(sk2, tb2, tb) {
		if (!inet_bind_conflict(sk, sk2, uid, relax, reuseport_cb_ok, reuseport_ok))
			continue;

		if (inet_rcv_saddr_equal(sk, sk2, true))
			return true;
	}

	return false;
}

/* Determine if there is a bind conflict with an existing IPV6_ADDR_ANY (if ipv6) or
 * INADDR_ANY (if ipv4) socket.
 *
 * Caller must hold bhash hashbucket lock with local bh disabled, to protect
 * against concurrent binds on the port for addr any
 */
static bool inet_bhash2_addr_any_conflict(const struct sock *sk, int port, int l3mdev,
					  bool relax, bool reuseport_ok)
{
	kuid_t uid = sock_i_uid((struct sock *)sk);
	const struct net *net = sock_net(sk);
	struct sock_reuseport *reuseport_cb;
	struct inet_bind_hashbucket *head2;
	struct inet_bind2_bucket *tb2;
	bool conflict = false;
	bool reuseport_cb_ok;

	rcu_read_lock();
	reuseport_cb = rcu_dereference(sk->sk_reuseport_cb);
	/* paired with WRITE_ONCE() in __reuseport_(add|detach)_closed_sock */
	reuseport_cb_ok = !reuseport_cb || READ_ONCE(reuseport_cb->num_closed_socks);
	rcu_read_unlock();

	head2 = inet_bhash2_addr_any_hashbucket(sk, net, port);

	spin_lock(&head2->lock);

	inet_bind_bucket_for_each(tb2, &head2->chain) {
		if (!inet_bind2_bucket_match_addr_any(tb2, net, port, l3mdev, sk))
			continue;

		if (!inet_bhash2_conflict(sk, tb2, uid, relax, reuseport_cb_ok,	reuseport_ok))
			continue;

		conflict = true;
		break;
	}

	spin_unlock(&head2->lock);

	return conflict;
}

/*
 * Find an open port number for the socket.  Returns with the
 * inet_bind_hashbucket locks held if successful.
 */
static struct inet_bind_hashbucket *
inet_csk_find_open_port(const struct sock *sk, struct inet_bind_bucket **tb_ret,
			struct inet_bind2_bucket **tb2_ret,
			struct inet_bind_hashbucket **head2_ret, int *port_ret)
{
	struct inet_hashinfo *hinfo = tcp_or_dccp_get_hashinfo(sk);
	int i, low, high, attempt_half, port, l3mdev;
	struct inet_bind_hashbucket *head, *head2;
	struct net *net = sock_net(sk);
	struct inet_bind2_bucket *tb2;
	struct inet_bind_bucket *tb;
	u32 remaining, offset;
	bool relax = false;

	l3mdev = inet_sk_bound_l3mdev(sk);
ports_exhausted:
	attempt_half = (sk->sk_reuse == SK_CAN_REUSE) ? 1 : 0;
other_half_scan:
	inet_sk_get_local_port_range(sk, &low, &high);
	high++; /* [32768, 60999] -> [32768, 61000[ */
	if (high - low < 4)
		attempt_half = 0;
	if (attempt_half) {
		int half = low + (((high - low) >> 2) << 1);

		if (attempt_half == 1)
			high = half;
		else
			low = half;
	}
	remaining = high - low;
	if (likely(remaining > 1))
		remaining &= ~1U;

	offset = get_random_u32_below(remaining);
	/* __inet_hash_connect() favors ports having @low parity
	 * We do the opposite to not pollute connect() users.
	 */
	offset |= 1U;

other_parity_scan:
	port = low + offset;
	for (i = 0; i < remaining; i += 2, port += 2) {
		if (unlikely(port >= high))
			port -= remaining;
		if (inet_is_local_reserved_port(net, port))
			continue;
		head = &hinfo->bhash[inet_bhashfn(net, port,
						  hinfo->bhash_size)];
		spin_lock_bh(&head->lock);
		if (inet_use_bhash2_on_bind(sk)) {
			if (inet_bhash2_addr_any_conflict(sk, port, l3mdev, relax, false))
				goto next_port;
		}

		head2 = inet_bhashfn_portaddr(hinfo, sk, net, port);
		spin_lock(&head2->lock);
		tb2 = inet_bind2_bucket_find(head2, net, port, l3mdev, sk);
		inet_bind_bucket_for_each(tb, &head->chain)
			if (inet_bind_bucket_match(tb, net, port, l3mdev)) {
				if (!inet_csk_bind_conflict(sk, tb, tb2,
							    relax, false))
					goto success;
				spin_unlock(&head2->lock);
				goto next_port;
			}
		tb = NULL;
		goto success;
next_port:
		spin_unlock_bh(&head->lock);
		cond_resched();
	}

	offset--;
	if (!(offset & 1))
		goto other_parity_scan;

	if (attempt_half == 1) {
		/* OK we now try the upper half of the range */
		attempt_half = 2;
		goto other_half_scan;
	}

	if (READ_ONCE(net->ipv4.sysctl_ip_autobind_reuse) && !relax) {
		/* We still have a chance to connect to different destinations */
		relax = true;
		goto ports_exhausted;
	}
	return NULL;
success:
	*port_ret = port;
	*tb_ret = tb;
	*tb2_ret = tb2;
	*head2_ret = head2;
	return head;
}

static inline int sk_reuseport_match(struct inet_bind_bucket *tb,
				     struct sock *sk)
{
	kuid_t uid = sock_i_uid(sk);

	if (tb->fastreuseport <= 0)
		return 0;
	if (!sk->sk_reuseport)
		return 0;
	if (rcu_access_pointer(sk->sk_reuseport_cb))
		return 0;
	if (!uid_eq(tb->fastuid, uid))
		return 0;
	/* We only need to check the rcv_saddr if this tb was once marked
	 * without fastreuseport and then was reset, as we can only know that
	 * the fast_*rcv_saddr doesn't have any conflicts with the socks on the
	 * owners list.
	 */
	if (tb->fastreuseport == FASTREUSEPORT_ANY)
		return 1;
#if IS_ENABLED(CONFIG_IPV6)
	if (tb->fast_sk_family == AF_INET6)
		return ipv6_rcv_saddr_equal(&tb->fast_v6_rcv_saddr,
					    inet6_rcv_saddr(sk),
					    tb->fast_rcv_saddr,
					    sk->sk_rcv_saddr,
					    tb->fast_ipv6_only,
					    ipv6_only_sock(sk), true, false);
#endif
	return ipv4_rcv_saddr_equal(tb->fast_rcv_saddr, sk->sk_rcv_saddr,
				    ipv6_only_sock(sk), true, false);
}

void inet_csk_update_fastreuse(struct inet_bind_bucket *tb,
			       struct sock *sk)
{
	kuid_t uid = sock_i_uid(sk);
	bool reuse = sk->sk_reuse && sk->sk_state != TCP_LISTEN;

	if (hlist_empty(&tb->bhash2)) {
		tb->fastreuse = reuse;
		if (sk->sk_reuseport) {
			tb->fastreuseport = FASTREUSEPORT_ANY;
			tb->fastuid = uid;
			tb->fast_rcv_saddr = sk->sk_rcv_saddr;
			tb->fast_ipv6_only = ipv6_only_sock(sk);
			tb->fast_sk_family = sk->sk_family;
#if IS_ENABLED(CONFIG_IPV6)
			tb->fast_v6_rcv_saddr = sk->sk_v6_rcv_saddr;
#endif
		} else {
			tb->fastreuseport = 0;
		}
	} else {
		if (!reuse)
			tb->fastreuse = 0;
		if (sk->sk_reuseport) {
			/* We didn't match or we don't have fastreuseport set on
			 * the tb, but we have sk_reuseport set on this socket
			 * and we know that there are no bind conflicts with
			 * this socket in this tb, so reset our tb's reuseport
			 * settings so that any subsequent sockets that match
			 * our current socket will be put on the fast path.
			 *
			 * If we reset we need to set FASTREUSEPORT_STRICT so we
			 * do extra checking for all subsequent sk_reuseport
			 * socks.
			 */
			if (!sk_reuseport_match(tb, sk)) {
				tb->fastreuseport = FASTREUSEPORT_STRICT;
				tb->fastuid = uid;
				tb->fast_rcv_saddr = sk->sk_rcv_saddr;
				tb->fast_ipv6_only = ipv6_only_sock(sk);
				tb->fast_sk_family = sk->sk_family;
#if IS_ENABLED(CONFIG_IPV6)
				tb->fast_v6_rcv_saddr = sk->sk_v6_rcv_saddr;
#endif
			}
		} else {
			tb->fastreuseport = 0;
		}
	}
}

/* Obtain a reference to a local port for the given sock,
 * if snum is zero it means select any available local port.
 * We try to allocate an odd port (and leave even ports for connect())
 */
int inet_csk_get_port(struct sock *sk, unsigned short snum)
{
	struct inet_hashinfo *hinfo = tcp_or_dccp_get_hashinfo(sk);
	bool reuse = sk->sk_reuse && sk->sk_state != TCP_LISTEN;
	bool found_port = false, check_bind_conflict = true;
	bool bhash_created = false, bhash2_created = false;
	int ret = -EADDRINUSE, port = snum, l3mdev;
	struct inet_bind_hashbucket *head, *head2;
	struct inet_bind2_bucket *tb2 = NULL;
	struct inet_bind_bucket *tb = NULL;
	bool head2_lock_acquired = false;
	struct net *net = sock_net(sk);

	l3mdev = inet_sk_bound_l3mdev(sk);

	if (!port) {
		head = inet_csk_find_open_port(sk, &tb, &tb2, &head2, &port);
		if (!head)
			return ret;

		head2_lock_acquired = true;

		if (tb && tb2)
			goto success;
		found_port = true;
	} else {
		head = &hinfo->bhash[inet_bhashfn(net, port,
						  hinfo->bhash_size)];
		spin_lock_bh(&head->lock);
		inet_bind_bucket_for_each(tb, &head->chain)
			if (inet_bind_bucket_match(tb, net, port, l3mdev))
				break;
	}

	if (!tb) {
		tb = inet_bind_bucket_create(hinfo->bind_bucket_cachep, net,
					     head, port, l3mdev);
		if (!tb)
			goto fail_unlock;
		bhash_created = true;
	}

	if (!found_port) {
		if (!hlist_empty(&tb->bhash2)) {
			if (sk->sk_reuse == SK_FORCE_REUSE ||
			    (tb->fastreuse > 0 && reuse) ||
			    sk_reuseport_match(tb, sk))
				check_bind_conflict = false;
		}

		if (check_bind_conflict && inet_use_bhash2_on_bind(sk)) {
			if (inet_bhash2_addr_any_conflict(sk, port, l3mdev, true, true))
				goto fail_unlock;
		}

		head2 = inet_bhashfn_portaddr(hinfo, sk, net, port);
		spin_lock(&head2->lock);
		head2_lock_acquired = true;
		tb2 = inet_bind2_bucket_find(head2, net, port, l3mdev, sk);
	}

	if (!tb2) {
		tb2 = inet_bind2_bucket_create(hinfo->bind2_bucket_cachep,
					       net, head2, tb, sk);
		if (!tb2)
			goto fail_unlock;
		bhash2_created = true;
	}

	if (!found_port && check_bind_conflict) {
		if (inet_csk_bind_conflict(sk, tb, tb2, true, true))
			goto fail_unlock;
	}

success:
	inet_csk_update_fastreuse(tb, sk);

	if (!inet_csk(sk)->icsk_bind_hash)
		inet_bind_hash(sk, tb, tb2, port);
	WARN_ON(inet_csk(sk)->icsk_bind_hash != tb);
	WARN_ON(inet_csk(sk)->icsk_bind2_hash != tb2);
	ret = 0;

fail_unlock:
	if (ret) {
		if (bhash2_created)
			inet_bind2_bucket_destroy(hinfo->bind2_bucket_cachep, tb2);
		if (bhash_created)
			inet_bind_bucket_destroy(hinfo->bind_bucket_cachep, tb);
	}
	if (head2_lock_acquired)
		spin_unlock(&head2->lock);
	spin_unlock_bh(&head->lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}
EXPORT_SYMBOL_GPL(inet_csk_get_port);

/*
 * Wait for an incoming connection, avoid race conditions. This must be called
 * with the socket locked.
 */
static int inet_csk_wait_for_connect(struct sock *sk, long timeo)
{
	struct inet_connection_sock *icsk = inet_csk(sk);
	DEFINE_WAIT(wait);
	int err;

	/*
	 * True wake-one mechanism for incoming connections: only
	 * one process gets woken up, not the 'whole herd'.
	 * Since we do not 'race & poll' for established sockets
	 * anymore, the common case will execute the loop only once.
	 *
	 * Subtle issue: "add_wait_queue_exclusive()" will be added
	 * after any current non-exclusive waiters, and we know that
	 * it will always _stay_ after any new non-exclusive waiters
	 * because all non-exclusive waiters are added at the
	 * beginning of the wait-queue. As such, it's ok to "drop"
	 * our exclusiveness temporarily when we get woken up without
	 * having to remove and re-insert us on the wait queue.
	 */
	for (;;) {
		prepare_to_wait_exclusive(sk_sleep(sk), &wait,
					  TASK_INTERRUPTIBLE);
		release_sock(sk);
		if (reqsk_queue_empty(&icsk->icsk_accept_queue))
			timeo = schedule_timeout(timeo);
<<<<<<< HEAD
=======
		sched_annotate_sleep();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		lock_sock(sk);
		err = 0;
		if (!reqsk_queue_empty(&icsk->icsk_accept_queue))
			break;
		err = -EINVAL;
		if (sk->sk_state != TCP_LISTEN)
			break;
		err = sock_intr_errno(timeo);
		if (signal_pending(current))
			break;
		err = -EAGAIN;
		if (!timeo)
			break;
	}
	finish_wait(sk_sleep(sk), &wait);
	return err;
}

/*
 * This will accept the next outstanding connection.
 */
<<<<<<< HEAD
struct sock *inet_csk_accept(struct sock *sk, int flags, int *err)
{
	struct inet_connection_sock *icsk = inet_csk(sk);
=======
struct sock *inet_csk_accept(struct sock *sk, int flags, int *err, bool kern)
{
	struct inet_connection_sock *icsk = inet_csk(sk);
	struct request_sock_queue *queue = &icsk->icsk_accept_queue;
	struct request_sock *req;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct sock *newsk;
	int error;

	lock_sock(sk);

	/* We need to make sure that this socket is listening,
	 * and that it has something pending.
	 */
	error = -EINVAL;
	if (sk->sk_state != TCP_LISTEN)
		goto out_err;

	/* Find already established connection */
<<<<<<< HEAD
	if (reqsk_queue_empty(&icsk->icsk_accept_queue)) {
=======
	if (reqsk_queue_empty(queue)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		long timeo = sock_rcvtimeo(sk, flags & O_NONBLOCK);

		/* If this is a non blocking socket don't sleep */
		error = -EAGAIN;
		if (!timeo)
			goto out_err;

		error = inet_csk_wait_for_connect(sk, timeo);
		if (error)
			goto out_err;
	}
<<<<<<< HEAD

	newsk = reqsk_queue_get_child(&icsk->icsk_accept_queue, sk);
	WARN_ON(newsk->sk_state == TCP_SYN_RECV);
out:
	release_sock(sk);
	return newsk;
out_err:
	newsk = NULL;
=======
	req = reqsk_queue_remove(queue, sk);
	newsk = req->sk;

	if (sk->sk_protocol == IPPROTO_TCP &&
	    tcp_rsk(req)->tfo_listener) {
		spin_lock_bh(&queue->fastopenq.lock);
		if (tcp_rsk(req)->tfo_listener) {
			/* We are still waiting for the final ACK from 3WHS
			 * so can't free req now. Instead, we set req->sk to
			 * NULL to signify that the child socket is taken
			 * so reqsk_fastopen_remove() will free the req
			 * when 3WHS finishes (or is aborted).
			 */
			req->sk = NULL;
			req = NULL;
		}
		spin_unlock_bh(&queue->fastopenq.lock);
	}

out:
	release_sock(sk);
	if (newsk && mem_cgroup_sockets_enabled) {
		int amt = 0;

		/* atomically get the memory usage, set and charge the
		 * newsk->sk_memcg.
		 */
		lock_sock(newsk);

		mem_cgroup_sk_alloc(newsk);
		if (newsk->sk_memcg) {
			/* The socket has not been accepted yet, no need
			 * to look at newsk->sk_wmem_queued.
			 */
			amt = sk_mem_pages(newsk->sk_forward_alloc +
					   atomic_read(&newsk->sk_rmem_alloc));
		}

		if (amt)
			mem_cgroup_charge_skmem(newsk->sk_memcg, amt,
						GFP_KERNEL | __GFP_NOFAIL);

		release_sock(newsk);
	}
	if (req)
		reqsk_put(req);

	if (newsk)
		inet_init_csk_locks(newsk);

	return newsk;
out_err:
	newsk = NULL;
	req = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	*err = error;
	goto out;
}
EXPORT_SYMBOL(inet_csk_accept);

/*
 * Using different timers for retransmit, delayed acks and probes
 * We may wish use just one timer maintaining a list of expire jiffies
 * to optimize.
 */
void inet_csk_init_xmit_timers(struct sock *sk,
<<<<<<< HEAD
			       void (*retransmit_handler)(unsigned long),
			       void (*delack_handler)(unsigned long),
			       void (*keepalive_handler)(unsigned long))
{
	struct inet_connection_sock *icsk = inet_csk(sk);

	setup_timer(&icsk->icsk_retransmit_timer, retransmit_handler,
			(unsigned long)sk);
	setup_timer(&icsk->icsk_delack_timer, delack_handler,
			(unsigned long)sk);
	setup_timer(&sk->sk_timer, keepalive_handler, (unsigned long)sk);
=======
			       void (*retransmit_handler)(struct timer_list *t),
			       void (*delack_handler)(struct timer_list *t),
			       void (*keepalive_handler)(struct timer_list *t))
{
	struct inet_connection_sock *icsk = inet_csk(sk);

	timer_setup(&icsk->icsk_retransmit_timer, retransmit_handler, 0);
	timer_setup(&icsk->icsk_delack_timer, delack_handler, 0);
	timer_setup(&sk->sk_timer, keepalive_handler, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	icsk->icsk_pending = icsk->icsk_ack.pending = 0;
}
EXPORT_SYMBOL(inet_csk_init_xmit_timers);

void inet_csk_clear_xmit_timers(struct sock *sk)
{
	struct inet_connection_sock *icsk = inet_csk(sk);

<<<<<<< HEAD
	icsk->icsk_pending = icsk->icsk_ack.pending = icsk->icsk_ack.blocked = 0;
=======
	icsk->icsk_pending = icsk->icsk_ack.pending = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	sk_stop_timer(sk, &icsk->icsk_retransmit_timer);
	sk_stop_timer(sk, &icsk->icsk_delack_timer);
	sk_stop_timer(sk, &sk->sk_timer);
}
EXPORT_SYMBOL(inet_csk_clear_xmit_timers);

<<<<<<< HEAD
=======
void inet_csk_clear_xmit_timers_sync(struct sock *sk)
{
	struct inet_connection_sock *icsk = inet_csk(sk);

	/* ongoing timer handlers need to acquire socket lock. */
	sock_not_owned_by_me(sk);

	icsk->icsk_pending = icsk->icsk_ack.pending = 0;

	sk_stop_timer_sync(sk, &icsk->icsk_retransmit_timer);
	sk_stop_timer_sync(sk, &icsk->icsk_delack_timer);
	sk_stop_timer_sync(sk, &sk->sk_timer);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void inet_csk_delete_keepalive_timer(struct sock *sk)
{
	sk_stop_timer(sk, &sk->sk_timer);
}
EXPORT_SYMBOL(inet_csk_delete_keepalive_timer);

void inet_csk_reset_keepalive_timer(struct sock *sk, unsigned long len)
{
	sk_reset_timer(sk, &sk->sk_timer, jiffies + len);
}
EXPORT_SYMBOL(inet_csk_reset_keepalive_timer);

<<<<<<< HEAD
struct dst_entry *inet_csk_route_req(struct sock *sk,
				     struct flowi4 *fl4,
				     const struct request_sock *req)
{
	struct rtable *rt;
	const struct inet_request_sock *ireq = inet_rsk(req);
	struct ip_options_rcu *opt = inet_rsk(req)->opt;
	struct net *net = sock_net(sk);

	flowi4_init_output(fl4, sk->sk_bound_dev_if, ireq->ir_mark,
			   RT_CONN_FLAGS(sk), RT_SCOPE_UNIVERSE,
			   sk->sk_protocol, inet_sk_flowi_flags(sk),
			   (opt && opt->opt.srr) ? opt->opt.faddr : ireq->rmt_addr,
			   ireq->loc_addr, ireq->rmt_port, inet_sk(sk)->inet_sport,
			   sk->sk_uid);
	security_req_classify_flow(req, flowi4_to_flowi(fl4));
	rt = ip_route_output_flow(net, fl4, sk);
	if (IS_ERR(rt))
		goto no_route;
	if (opt && opt->opt.is_strictroute && fl4->daddr != rt->rt_gateway)
		goto route_err;
=======
struct dst_entry *inet_csk_route_req(const struct sock *sk,
				     struct flowi4 *fl4,
				     const struct request_sock *req)
{
	const struct inet_request_sock *ireq = inet_rsk(req);
	struct net *net = read_pnet(&ireq->ireq_net);
	struct ip_options_rcu *opt;
	struct rtable *rt;

	rcu_read_lock();
	opt = rcu_dereference(ireq->ireq_opt);

	flowi4_init_output(fl4, ireq->ir_iif, ireq->ir_mark,
			   ip_sock_rt_tos(sk), ip_sock_rt_scope(sk),
			   sk->sk_protocol, inet_sk_flowi_flags(sk),
			   (opt && opt->opt.srr) ? opt->opt.faddr : ireq->ir_rmt_addr,
			   ireq->ir_loc_addr, ireq->ir_rmt_port,
			   htons(ireq->ir_num), sk->sk_uid);
	security_req_classify_flow(req, flowi4_to_flowi_common(fl4));
	rt = ip_route_output_flow(net, fl4, sk);
	if (IS_ERR(rt))
		goto no_route;
	if (opt && opt->opt.is_strictroute && rt->rt_uses_gateway)
		goto route_err;
	rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return &rt->dst;

route_err:
	ip_rt_put(rt);
no_route:
<<<<<<< HEAD
	IP_INC_STATS_BH(net, IPSTATS_MIB_OUTNOROUTES);
=======
	rcu_read_unlock();
	__IP_INC_STATS(net, IPSTATS_MIB_OUTNOROUTES);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return NULL;
}
EXPORT_SYMBOL_GPL(inet_csk_route_req);

<<<<<<< HEAD
struct dst_entry *inet_csk_route_child_sock(struct sock *sk,
=======
struct dst_entry *inet_csk_route_child_sock(const struct sock *sk,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					    struct sock *newsk,
					    const struct request_sock *req)
{
	const struct inet_request_sock *ireq = inet_rsk(req);
<<<<<<< HEAD
	struct inet_sock *newinet = inet_sk(newsk);
	struct ip_options_rcu *opt = ireq->opt;
	struct net *net = sock_net(sk);
	struct flowi4 *fl4;
	struct rtable *rt;

	fl4 = &newinet->cork.fl.u.ip4;
	flowi4_init_output(fl4, sk->sk_bound_dev_if, ireq->ir_mark,
			   RT_CONN_FLAGS(sk), RT_SCOPE_UNIVERSE,
			   sk->sk_protocol, inet_sk_flowi_flags(sk),
			   (opt && opt->opt.srr) ? opt->opt.faddr : ireq->rmt_addr,
			   ireq->loc_addr, ireq->rmt_port, inet_sk(sk)->inet_sport,
			   sk->sk_uid);
	security_req_classify_flow(req, flowi4_to_flowi(fl4));
	rt = ip_route_output_flow(net, fl4, sk);
	if (IS_ERR(rt))
		goto no_route;
	if (opt && opt->opt.is_strictroute && fl4->daddr != rt->rt_gateway)
=======
	struct net *net = read_pnet(&ireq->ireq_net);
	struct inet_sock *newinet = inet_sk(newsk);
	struct ip_options_rcu *opt;
	struct flowi4 *fl4;
	struct rtable *rt;

	opt = rcu_dereference(ireq->ireq_opt);
	fl4 = &newinet->cork.fl.u.ip4;

	flowi4_init_output(fl4, ireq->ir_iif, ireq->ir_mark,
			   ip_sock_rt_tos(sk), ip_sock_rt_scope(sk),
			   sk->sk_protocol, inet_sk_flowi_flags(sk),
			   (opt && opt->opt.srr) ? opt->opt.faddr : ireq->ir_rmt_addr,
			   ireq->ir_loc_addr, ireq->ir_rmt_port,
			   htons(ireq->ir_num), sk->sk_uid);
	security_req_classify_flow(req, flowi4_to_flowi_common(fl4));
	rt = ip_route_output_flow(net, fl4, sk);
	if (IS_ERR(rt))
		goto no_route;
	if (opt && opt->opt.is_strictroute && rt->rt_uses_gateway)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto route_err;
	return &rt->dst;

route_err:
	ip_rt_put(rt);
no_route:
<<<<<<< HEAD
	IP_INC_STATS_BH(net, IPSTATS_MIB_OUTNOROUTES);
=======
	__IP_INC_STATS(net, IPSTATS_MIB_OUTNOROUTES);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return NULL;
}
EXPORT_SYMBOL_GPL(inet_csk_route_child_sock);

<<<<<<< HEAD
static inline u32 inet_synq_hash(const __be32 raddr, const __be16 rport,
				 const u32 rnd, const u32 synq_hsize)
{
	return jhash_2words((__force u32)raddr, (__force u32)rport, rnd) & (synq_hsize - 1);
}

#if IS_ENABLED(CONFIG_IPV6)
#define AF_INET_FAMILY(fam) ((fam) == AF_INET)
#else
#define AF_INET_FAMILY(fam) 1
#endif

struct request_sock *inet_csk_search_req(const struct sock *sk,
					 struct request_sock ***prevp,
					 const __be16 rport, const __be32 raddr,
					 const __be32 laddr)
{
	const struct inet_connection_sock *icsk = inet_csk(sk);
	struct listen_sock *lopt = icsk->icsk_accept_queue.listen_opt;
	struct request_sock *req, **prev;

	for (prev = &lopt->syn_table[inet_synq_hash(raddr, rport, lopt->hash_rnd,
						    lopt->nr_table_entries)];
	     (req = *prev) != NULL;
	     prev = &req->dl_next) {
		const struct inet_request_sock *ireq = inet_rsk(req);

		if (ireq->rmt_port == rport &&
		    ireq->rmt_addr == raddr &&
		    ireq->loc_addr == laddr &&
		    AF_INET_FAMILY(req->rsk_ops->family)) {
			WARN_ON(req->sk);
			*prevp = prev;
			break;
		}
	}

	return req;
}
EXPORT_SYMBOL_GPL(inet_csk_search_req);

void inet_csk_reqsk_queue_hash_add(struct sock *sk, struct request_sock *req,
				   unsigned long timeout)
{
	struct inet_connection_sock *icsk = inet_csk(sk);
	struct listen_sock *lopt = icsk->icsk_accept_queue.listen_opt;
	const u32 h = inet_synq_hash(inet_rsk(req)->rmt_addr, inet_rsk(req)->rmt_port,
				     lopt->hash_rnd, lopt->nr_table_entries);

	reqsk_queue_hash_req(&icsk->icsk_accept_queue, h, req, timeout);
	inet_csk_reqsk_queue_added(sk, timeout);
}
EXPORT_SYMBOL_GPL(inet_csk_reqsk_queue_hash_add);

/* Only thing we need from tcp.h */
extern int sysctl_tcp_synack_retries;


/* Decide when to expire the request and when to resend SYN-ACK */
static inline void syn_ack_recalc(struct request_sock *req, const int thresh,
				  const int max_retries,
				  const u8 rskq_defer_accept,
				  int *expire, int *resend)
{
	if (!rskq_defer_accept) {
		*expire = req->retrans >= thresh;
		*resend = 1;
		return;
	}
	*expire = req->retrans >= thresh &&
		  (!inet_rsk(req)->acked || req->retrans >= max_retries);
	/*
	 * Do not resend while waiting for data after ACK,
=======
/* Decide when to expire the request and when to resend SYN-ACK */
static void syn_ack_recalc(struct request_sock *req,
			   const int max_syn_ack_retries,
			   const u8 rskq_defer_accept,
			   int *expire, int *resend)
{
	if (!rskq_defer_accept) {
		*expire = req->num_timeout >= max_syn_ack_retries;
		*resend = 1;
		return;
	}
	*expire = req->num_timeout >= max_syn_ack_retries &&
		  (!inet_rsk(req)->acked || req->num_timeout >= rskq_defer_accept);
	/* Do not resend while waiting for data after ACK,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * start to resend on end of deferring period to give
	 * last chance for data or ACK to create established socket.
	 */
	*resend = !inet_rsk(req)->acked ||
<<<<<<< HEAD
		  req->retrans >= rskq_defer_accept - 1;
}

void inet_csk_reqsk_queue_prune(struct sock *parent,
				const unsigned long interval,
				const unsigned long timeout,
				const unsigned long max_rto)
{
	struct inet_connection_sock *icsk = inet_csk(parent);
	struct request_sock_queue *queue = &icsk->icsk_accept_queue;
	struct listen_sock *lopt = queue->listen_opt;
	int max_retries = icsk->icsk_syn_retries ? : sysctl_tcp_synack_retries;
	int thresh = max_retries;
	unsigned long now = jiffies;
	struct request_sock **reqp, *req;
	int i, budget;

	if (lopt == NULL || lopt->qlen == 0)
		return;

	/* Normally all the openreqs are young and become mature
	 * (i.e. converted to established socket) for first timeout.
	 * If synack was not acknowledged for 3 seconds, it means
=======
		  req->num_timeout >= rskq_defer_accept - 1;
}

int inet_rtx_syn_ack(const struct sock *parent, struct request_sock *req)
{
	int err = req->rsk_ops->rtx_syn_ack(parent, req);

	if (!err)
		req->num_retrans++;
	return err;
}
EXPORT_SYMBOL(inet_rtx_syn_ack);

static struct request_sock *inet_reqsk_clone(struct request_sock *req,
					     struct sock *sk)
{
	struct sock *req_sk, *nreq_sk;
	struct request_sock *nreq;

	nreq = kmem_cache_alloc(req->rsk_ops->slab, GFP_ATOMIC | __GFP_NOWARN);
	if (!nreq) {
		__NET_INC_STATS(sock_net(sk), LINUX_MIB_TCPMIGRATEREQFAILURE);

		/* paired with refcount_inc_not_zero() in reuseport_migrate_sock() */
		sock_put(sk);
		return NULL;
	}

	req_sk = req_to_sk(req);
	nreq_sk = req_to_sk(nreq);

	memcpy(nreq_sk, req_sk,
	       offsetof(struct sock, sk_dontcopy_begin));
	unsafe_memcpy(&nreq_sk->sk_dontcopy_end, &req_sk->sk_dontcopy_end,
		      req->rsk_ops->obj_size - offsetof(struct sock, sk_dontcopy_end),
		      /* alloc is larger than struct, see above */);

	sk_node_init(&nreq_sk->sk_node);
	nreq_sk->sk_tx_queue_mapping = req_sk->sk_tx_queue_mapping;
#ifdef CONFIG_SOCK_RX_QUEUE_MAPPING
	nreq_sk->sk_rx_queue_mapping = req_sk->sk_rx_queue_mapping;
#endif
	nreq_sk->sk_incoming_cpu = req_sk->sk_incoming_cpu;

	nreq->rsk_listener = sk;

	/* We need not acquire fastopenq->lock
	 * because the child socket is locked in inet_csk_listen_stop().
	 */
	if (sk->sk_protocol == IPPROTO_TCP && tcp_rsk(nreq)->tfo_listener)
		rcu_assign_pointer(tcp_sk(nreq->sk)->fastopen_rsk, nreq);

	return nreq;
}

static void reqsk_queue_migrated(struct request_sock_queue *queue,
				 const struct request_sock *req)
{
	if (req->num_timeout == 0)
		atomic_inc(&queue->young);
	atomic_inc(&queue->qlen);
}

static void reqsk_migrate_reset(struct request_sock *req)
{
	req->saved_syn = NULL;
#if IS_ENABLED(CONFIG_IPV6)
	inet_rsk(req)->ipv6_opt = NULL;
	inet_rsk(req)->pktopts = NULL;
#else
	inet_rsk(req)->ireq_opt = NULL;
#endif
}

/* return true if req was found in the ehash table */
static bool reqsk_queue_unlink(struct request_sock *req)
{
	struct sock *sk = req_to_sk(req);
	bool found = false;

	if (sk_hashed(sk)) {
		struct inet_hashinfo *hashinfo = tcp_or_dccp_get_hashinfo(sk);
		spinlock_t *lock = inet_ehash_lockp(hashinfo, req->rsk_hash);

		spin_lock(lock);
		found = __sk_nulls_del_node_init_rcu(sk);
		spin_unlock(lock);
	}
	if (timer_pending(&req->rsk_timer) && del_timer_sync(&req->rsk_timer))
		reqsk_put(req);
	return found;
}

bool inet_csk_reqsk_queue_drop(struct sock *sk, struct request_sock *req)
{
	bool unlinked = reqsk_queue_unlink(req);

	if (unlinked) {
		reqsk_queue_removed(&inet_csk(sk)->icsk_accept_queue, req);
		reqsk_put(req);
	}
	return unlinked;
}
EXPORT_SYMBOL(inet_csk_reqsk_queue_drop);

void inet_csk_reqsk_queue_drop_and_put(struct sock *sk, struct request_sock *req)
{
	inet_csk_reqsk_queue_drop(sk, req);
	reqsk_put(req);
}
EXPORT_SYMBOL(inet_csk_reqsk_queue_drop_and_put);

static void reqsk_timer_handler(struct timer_list *t)
{
	struct request_sock *req = from_timer(req, t, rsk_timer);
	struct request_sock *nreq = NULL, *oreq = req;
	struct sock *sk_listener = req->rsk_listener;
	struct inet_connection_sock *icsk;
	struct request_sock_queue *queue;
	struct net *net;
	int max_syn_ack_retries, qlen, expire = 0, resend = 0;

	if (inet_sk_state_load(sk_listener) != TCP_LISTEN) {
		struct sock *nsk;

		nsk = reuseport_migrate_sock(sk_listener, req_to_sk(req), NULL);
		if (!nsk)
			goto drop;

		nreq = inet_reqsk_clone(req, nsk);
		if (!nreq)
			goto drop;

		/* The new timer for the cloned req can decrease the 2
		 * by calling inet_csk_reqsk_queue_drop_and_put(), so
		 * hold another count to prevent use-after-free and
		 * call reqsk_put() just before return.
		 */
		refcount_set(&nreq->rsk_refcnt, 2 + 1);
		timer_setup(&nreq->rsk_timer, reqsk_timer_handler, TIMER_PINNED);
		reqsk_queue_migrated(&inet_csk(nsk)->icsk_accept_queue, req);

		req = nreq;
		sk_listener = nsk;
	}

	icsk = inet_csk(sk_listener);
	net = sock_net(sk_listener);
	max_syn_ack_retries = READ_ONCE(icsk->icsk_syn_retries) ? :
		READ_ONCE(net->ipv4.sysctl_tcp_synack_retries);
	/* Normally all the openreqs are young and become mature
	 * (i.e. converted to established socket) for first timeout.
	 * If synack was not acknowledged for 1 second, it means
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * one of the following things: synack was lost, ack was lost,
	 * rtt is high or nobody planned to ack (i.e. synflood).
	 * When server is a bit loaded, queue is populated with old
	 * open requests, reducing effective size of queue.
	 * When server is well loaded, queue size reduces to zero
	 * after several minutes of work. It is not synflood,
	 * it is normal operation. The solution is pruning
	 * too old entries overriding normal timeout, when
	 * situation becomes dangerous.
	 *
	 * Essentially, we reserve half of room for young
	 * embrions; and abort old ones without pity, if old
	 * ones are about to clog our table.
	 */
<<<<<<< HEAD
	if (lopt->qlen>>(lopt->max_qlen_log-1)) {
		int young = (lopt->qlen_young<<1);

		while (thresh > 2) {
			if (lopt->qlen < young)
				break;
			thresh--;
			young <<= 1;
		}
	}

	if (queue->rskq_defer_accept)
		max_retries = queue->rskq_defer_accept;

	budget = 2 * (lopt->nr_table_entries / (timeout / interval));
	i = lopt->clock_hand;

	do {
		reqp=&lopt->syn_table[i];
		while ((req = *reqp) != NULL) {
			if (time_after_eq(now, req->expires)) {
				int expire = 0, resend = 0;

				syn_ack_recalc(req, thresh, max_retries,
					       queue->rskq_defer_accept,
					       &expire, &resend);
				if (req->rsk_ops->syn_ack_timeout)
					req->rsk_ops->syn_ack_timeout(parent, req);
				if (!expire &&
				    (!resend ||
				     !req->rsk_ops->rtx_syn_ack(parent, req, NULL) ||
				     inet_rsk(req)->acked)) {
					unsigned long timeo;

					if (req->retrans++ == 0)
						lopt->qlen_young--;
					timeo = min((timeout << req->retrans), max_rto);
					req->expires = now + timeo;
					reqp = &req->dl_next;
					continue;
				}

				/* Drop this request */
				inet_csk_reqsk_queue_unlink(parent, req, reqp);
				reqsk_queue_removed(queue, req);
				reqsk_free(req);
				continue;
			}
			reqp = &req->dl_next;
		}

		i = (i + 1) & (lopt->nr_table_entries - 1);

	} while (--budget > 0);

	lopt->clock_hand = i;

	if (lopt->qlen)
		inet_csk_reset_keepalive_timer(parent, interval);
}
EXPORT_SYMBOL_GPL(inet_csk_reqsk_queue_prune);
=======
	queue = &icsk->icsk_accept_queue;
	qlen = reqsk_queue_len(queue);
	if ((qlen << 1) > max(8U, READ_ONCE(sk_listener->sk_max_ack_backlog))) {
		int young = reqsk_queue_len_young(queue) << 1;

		while (max_syn_ack_retries > 2) {
			if (qlen < young)
				break;
			max_syn_ack_retries--;
			young <<= 1;
		}
	}
	syn_ack_recalc(req, max_syn_ack_retries, READ_ONCE(queue->rskq_defer_accept),
		       &expire, &resend);
	req->rsk_ops->syn_ack_timeout(req);
	if (!expire &&
	    (!resend ||
	     !inet_rtx_syn_ack(sk_listener, req) ||
	     inet_rsk(req)->acked)) {
		if (req->num_timeout++ == 0)
			atomic_dec(&queue->young);
		mod_timer(&req->rsk_timer, jiffies + reqsk_timeout(req, TCP_RTO_MAX));

		if (!nreq)
			return;

		if (!inet_ehash_insert(req_to_sk(nreq), req_to_sk(oreq), NULL)) {
			/* delete timer */
			inet_csk_reqsk_queue_drop(sk_listener, nreq);
			goto no_ownership;
		}

		__NET_INC_STATS(net, LINUX_MIB_TCPMIGRATEREQSUCCESS);
		reqsk_migrate_reset(oreq);
		reqsk_queue_removed(&inet_csk(oreq->rsk_listener)->icsk_accept_queue, oreq);
		reqsk_put(oreq);

		reqsk_put(nreq);
		return;
	}

	/* Even if we can clone the req, we may need not retransmit any more
	 * SYN+ACKs (nreq->num_timeout > max_syn_ack_retries, etc), or another
	 * CPU may win the "own_req" race so that inet_ehash_insert() fails.
	 */
	if (nreq) {
		__NET_INC_STATS(net, LINUX_MIB_TCPMIGRATEREQFAILURE);
no_ownership:
		reqsk_migrate_reset(nreq);
		reqsk_queue_removed(queue, nreq);
		__reqsk_free(nreq);
	}

drop:
	inet_csk_reqsk_queue_drop_and_put(oreq->rsk_listener, oreq);
}

static void reqsk_queue_hash_req(struct request_sock *req,
				 unsigned long timeout)
{
	timer_setup(&req->rsk_timer, reqsk_timer_handler, TIMER_PINNED);
	mod_timer(&req->rsk_timer, jiffies + timeout);

	inet_ehash_insert(req_to_sk(req), NULL, NULL);
	/* before letting lookups find us, make sure all req fields
	 * are committed to memory and refcnt initialized.
	 */
	smp_wmb();
	refcount_set(&req->rsk_refcnt, 2 + 1);
}

void inet_csk_reqsk_queue_hash_add(struct sock *sk, struct request_sock *req,
				   unsigned long timeout)
{
	reqsk_queue_hash_req(req, timeout);
	inet_csk_reqsk_queue_added(sk);
}
EXPORT_SYMBOL_GPL(inet_csk_reqsk_queue_hash_add);

static void inet_clone_ulp(const struct request_sock *req, struct sock *newsk,
			   const gfp_t priority)
{
	struct inet_connection_sock *icsk = inet_csk(newsk);

	if (!icsk->icsk_ulp_ops)
		return;

	icsk->icsk_ulp_ops->clone(req, newsk, priority);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 *	inet_csk_clone_lock - clone an inet socket, and lock its clone
 *	@sk: the socket to clone
 *	@req: request_sock
 *	@priority: for allocation (%GFP_KERNEL, %GFP_ATOMIC, etc)
 *
 *	Caller must unlock socket even in error path (bh_unlock_sock(newsk))
 */
struct sock *inet_csk_clone_lock(const struct sock *sk,
				 const struct request_sock *req,
				 const gfp_t priority)
{
	struct sock *newsk = sk_clone_lock(sk, priority);

<<<<<<< HEAD
	if (newsk != NULL) {
		struct inet_connection_sock *newicsk = inet_csk(newsk);

		newsk->sk_state = TCP_SYN_RECV;
		newicsk->icsk_bind_hash = NULL;

		inet_sk(newsk)->inet_dport = inet_rsk(req)->rmt_port;
		inet_sk(newsk)->inet_num = ntohs(inet_rsk(req)->loc_port);
		inet_sk(newsk)->inet_sport = inet_rsk(req)->loc_port;
		newsk->sk_write_space = sk_stream_write_space;
=======
	if (newsk) {
		struct inet_connection_sock *newicsk = inet_csk(newsk);

		inet_sk_set_state(newsk, TCP_SYN_RECV);
		newicsk->icsk_bind_hash = NULL;
		newicsk->icsk_bind2_hash = NULL;

		inet_sk(newsk)->inet_dport = inet_rsk(req)->ir_rmt_port;
		inet_sk(newsk)->inet_num = inet_rsk(req)->ir_num;
		inet_sk(newsk)->inet_sport = htons(inet_rsk(req)->ir_num);

		/* listeners have SOCK_RCU_FREE, not the children */
		sock_reset_flag(newsk, SOCK_RCU_FREE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		inet_sk(newsk)->mc_list = NULL;

		newsk->sk_mark = inet_rsk(req)->ir_mark;
<<<<<<< HEAD
=======
		atomic64_set(&newsk->sk_cookie,
			     atomic64_read(&inet_rsk(req)->ir_cookie));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		newicsk->icsk_retransmits = 0;
		newicsk->icsk_backoff	  = 0;
		newicsk->icsk_probes_out  = 0;
<<<<<<< HEAD
=======
		newicsk->icsk_probes_tstamp = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/* Deinitialize accept_queue to trap illegal accesses. */
		memset(&newicsk->icsk_accept_queue, 0, sizeof(newicsk->icsk_accept_queue));

<<<<<<< HEAD
=======
		inet_clone_ulp(req, newsk, priority);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		security_inet_csk_clone(newsk, req);
	}
	return newsk;
}
EXPORT_SYMBOL_GPL(inet_csk_clone_lock);

/*
 * At this point, there should be no process reference to this
 * socket, and thus no user references at all.  Therefore we
 * can assume the socket waitqueue is inactive and nobody will
 * try to jump onto it.
 */
void inet_csk_destroy_sock(struct sock *sk)
{
	WARN_ON(sk->sk_state != TCP_CLOSE);
	WARN_ON(!sock_flag(sk, SOCK_DEAD));

	/* It cannot be in hash table! */
	WARN_ON(!sk_unhashed(sk));

	/* If it has not 0 inet_sk(sk)->inet_num, it must be bound */
	WARN_ON(inet_sk(sk)->inet_num && !inet_csk(sk)->icsk_bind_hash);

	sk->sk_prot->destroy(sk);

	sk_stream_kill_queues(sk);

	xfrm_sk_free_policy(sk);

<<<<<<< HEAD
	sk_refcnt_debug_release(sk);

	percpu_counter_dec(sk->sk_prot->orphan_count);
=======
	this_cpu_dec(*sk->sk_prot->orphan_count);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	sock_put(sk);
}
EXPORT_SYMBOL(inet_csk_destroy_sock);

/* This function allows to force a closure of a socket after the call to
 * tcp/dccp_create_openreq_child().
 */
void inet_csk_prepare_forced_close(struct sock *sk)
<<<<<<< HEAD
=======
	__releases(&sk->sk_lock.slock)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	/* sk_clone_lock locked the socket and set refcnt to 2 */
	bh_unlock_sock(sk);
	sock_put(sk);
<<<<<<< HEAD

	/* The below has to be done to allow calling inet_csk_destroy_sock */
	sock_set_flag(sk, SOCK_DEAD);
	percpu_counter_inc(sk->sk_prot->orphan_count);
=======
	inet_csk_prepare_for_destroy_sock(sk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	inet_sk(sk)->inet_num = 0;
}
EXPORT_SYMBOL(inet_csk_prepare_forced_close);

<<<<<<< HEAD
int inet_csk_listen_start(struct sock *sk, const int nr_table_entries)
{
	struct inet_sock *inet = inet_sk(sk);
	struct inet_connection_sock *icsk = inet_csk(sk);
	int rc = reqsk_queue_alloc(&icsk->icsk_accept_queue, nr_table_entries);

	if (rc != 0)
		return rc;

	sk->sk_max_ack_backlog = 0;
=======
static int inet_ulp_can_listen(const struct sock *sk)
{
	const struct inet_connection_sock *icsk = inet_csk(sk);

	if (icsk->icsk_ulp_ops && !icsk->icsk_ulp_ops->clone)
		return -EINVAL;

	return 0;
}

int inet_csk_listen_start(struct sock *sk)
{
	struct inet_connection_sock *icsk = inet_csk(sk);
	struct inet_sock *inet = inet_sk(sk);
	int err;

	err = inet_ulp_can_listen(sk);
	if (unlikely(err))
		return err;

	reqsk_queue_alloc(&icsk->icsk_accept_queue);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	sk->sk_ack_backlog = 0;
	inet_csk_delack_init(sk);

	/* There is race window here: we announce ourselves listening,
	 * but this transition is still not validated by get_port().
	 * It is OK, because this socket enters to hash table only
	 * after validation is complete.
	 */
<<<<<<< HEAD
	sk->sk_state = TCP_LISTEN;
	if (!sk->sk_prot->get_port(sk, inet->inet_num)) {
		inet->inet_sport = htons(inet->inet_num);

		sk_dst_reset(sk);
		sk->sk_prot->hash(sk);

		return 0;
	}

	sk->sk_state = TCP_CLOSE;
	__reqsk_queue_destroy(&icsk->icsk_accept_queue);
	return -EADDRINUSE;
}
EXPORT_SYMBOL_GPL(inet_csk_listen_start);

=======
	inet_sk_state_store(sk, TCP_LISTEN);
	err = sk->sk_prot->get_port(sk, inet->inet_num);
	if (!err) {
		inet->inet_sport = htons(inet->inet_num);

		sk_dst_reset(sk);
		err = sk->sk_prot->hash(sk);

		if (likely(!err))
			return 0;
	}

	inet_sk_set_state(sk, TCP_CLOSE);
	return err;
}
EXPORT_SYMBOL_GPL(inet_csk_listen_start);

static void inet_child_forget(struct sock *sk, struct request_sock *req,
			      struct sock *child)
{
	sk->sk_prot->disconnect(child, O_NONBLOCK);

	sock_orphan(child);

	this_cpu_inc(*sk->sk_prot->orphan_count);

	if (sk->sk_protocol == IPPROTO_TCP && tcp_rsk(req)->tfo_listener) {
		BUG_ON(rcu_access_pointer(tcp_sk(child)->fastopen_rsk) != req);
		BUG_ON(sk != req->rsk_listener);

		/* Paranoid, to prevent race condition if
		 * an inbound pkt destined for child is
		 * blocked by sock lock in tcp_v4_rcv().
		 * Also to satisfy an assertion in
		 * tcp_v4_destroy_sock().
		 */
		RCU_INIT_POINTER(tcp_sk(child)->fastopen_rsk, NULL);
	}
	inet_csk_destroy_sock(child);
}

struct sock *inet_csk_reqsk_queue_add(struct sock *sk,
				      struct request_sock *req,
				      struct sock *child)
{
	struct request_sock_queue *queue = &inet_csk(sk)->icsk_accept_queue;

	spin_lock(&queue->rskq_lock);
	if (unlikely(sk->sk_state != TCP_LISTEN)) {
		inet_child_forget(sk, req, child);
		child = NULL;
	} else {
		req->sk = child;
		req->dl_next = NULL;
		if (queue->rskq_accept_head == NULL)
			WRITE_ONCE(queue->rskq_accept_head, req);
		else
			queue->rskq_accept_tail->dl_next = req;
		queue->rskq_accept_tail = req;
		sk_acceptq_added(sk);
	}
	spin_unlock(&queue->rskq_lock);
	return child;
}
EXPORT_SYMBOL(inet_csk_reqsk_queue_add);

struct sock *inet_csk_complete_hashdance(struct sock *sk, struct sock *child,
					 struct request_sock *req, bool own_req)
{
	if (own_req) {
		inet_csk_reqsk_queue_drop(req->rsk_listener, req);
		reqsk_queue_removed(&inet_csk(req->rsk_listener)->icsk_accept_queue, req);

		if (sk != req->rsk_listener) {
			/* another listening sk has been selected,
			 * migrate the req to it.
			 */
			struct request_sock *nreq;

			/* hold a refcnt for the nreq->rsk_listener
			 * which is assigned in inet_reqsk_clone()
			 */
			sock_hold(sk);
			nreq = inet_reqsk_clone(req, sk);
			if (!nreq) {
				inet_child_forget(sk, req, child);
				goto child_put;
			}

			refcount_set(&nreq->rsk_refcnt, 1);
			if (inet_csk_reqsk_queue_add(sk, nreq, child)) {
				__NET_INC_STATS(sock_net(sk), LINUX_MIB_TCPMIGRATEREQSUCCESS);
				reqsk_migrate_reset(req);
				reqsk_put(req);
				return child;
			}

			__NET_INC_STATS(sock_net(sk), LINUX_MIB_TCPMIGRATEREQFAILURE);
			reqsk_migrate_reset(nreq);
			__reqsk_free(nreq);
		} else if (inet_csk_reqsk_queue_add(sk, req, child)) {
			return child;
		}
	}
	/* Too bad, another child took ownership of the request, undo. */
child_put:
	bh_unlock_sock(child);
	sock_put(child);
	return NULL;
}
EXPORT_SYMBOL(inet_csk_complete_hashdance);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *	This routine closes sockets which have been at least partially
 *	opened, but not yet accepted.
 */
void inet_csk_listen_stop(struct sock *sk)
{
	struct inet_connection_sock *icsk = inet_csk(sk);
<<<<<<< HEAD
	struct request_sock *acc_req;
	struct request_sock *req;

	inet_csk_delete_keepalive_timer(sk);

	/* make all the listen_opt local to us */
	acc_req = reqsk_queue_yank_acceptq(&icsk->icsk_accept_queue);
=======
	struct request_sock_queue *queue = &icsk->icsk_accept_queue;
	struct request_sock *next, *req;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Following specs, it would be better either to send FIN
	 * (and enter FIN-WAIT-1, it is normal close)
	 * or to send active reset (abort).
	 * Certainly, it is pretty dangerous while synflood, but it is
	 * bad justification for our negligence 8)
	 * To be honest, we are not able to make either
	 * of the variants now.			--ANK
	 */
<<<<<<< HEAD
	reqsk_queue_destroy(&icsk->icsk_accept_queue);

	while ((req = acc_req) != NULL) {
		struct sock *child = req->sk;

		acc_req = req->dl_next;
=======
	while ((req = reqsk_queue_remove(queue, sk)) != NULL) {
		struct sock *child = req->sk, *nsk;
		struct request_sock *nreq;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		local_bh_disable();
		bh_lock_sock(child);
		WARN_ON(sock_owned_by_user(child));
		sock_hold(child);

<<<<<<< HEAD
		sk->sk_prot->disconnect(child, O_NONBLOCK);

		sock_orphan(child);

		percpu_counter_inc(sk->sk_prot->orphan_count);

		inet_csk_destroy_sock(child);

=======
		nsk = reuseport_migrate_sock(sk, child, NULL);
		if (nsk) {
			nreq = inet_reqsk_clone(req, nsk);
			if (nreq) {
				refcount_set(&nreq->rsk_refcnt, 1);

				if (inet_csk_reqsk_queue_add(nsk, nreq, child)) {
					__NET_INC_STATS(sock_net(nsk),
							LINUX_MIB_TCPMIGRATEREQSUCCESS);
					reqsk_migrate_reset(req);
				} else {
					__NET_INC_STATS(sock_net(nsk),
							LINUX_MIB_TCPMIGRATEREQFAILURE);
					reqsk_migrate_reset(nreq);
					__reqsk_free(nreq);
				}

				/* inet_csk_reqsk_queue_add() has already
				 * called inet_child_forget() on failure case.
				 */
				goto skip_child_forget;
			}
		}

		inet_child_forget(sk, req, child);
skip_child_forget:
		reqsk_put(req);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		bh_unlock_sock(child);
		local_bh_enable();
		sock_put(child);

<<<<<<< HEAD
		sk_acceptq_removed(sk);
		__reqsk_free(req);
	}
	WARN_ON(sk->sk_ack_backlog);
=======
		cond_resched();
	}
	if (queue->fastopenq.rskq_rst_head) {
		/* Free all the reqs queued in rskq_rst_head. */
		spin_lock_bh(&queue->fastopenq.lock);
		req = queue->fastopenq.rskq_rst_head;
		queue->fastopenq.rskq_rst_head = NULL;
		spin_unlock_bh(&queue->fastopenq.lock);
		while (req != NULL) {
			next = req->dl_next;
			reqsk_put(req);
			req = next;
		}
	}
	WARN_ON_ONCE(sk->sk_ack_backlog);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(inet_csk_listen_stop);

void inet_csk_addr2sockaddr(struct sock *sk, struct sockaddr *uaddr)
{
	struct sockaddr_in *sin = (struct sockaddr_in *)uaddr;
	const struct inet_sock *inet = inet_sk(sk);

	sin->sin_family		= AF_INET;
	sin->sin_addr.s_addr	= inet->inet_daddr;
	sin->sin_port		= inet->inet_dport;
}
EXPORT_SYMBOL_GPL(inet_csk_addr2sockaddr);

<<<<<<< HEAD
#ifdef CONFIG_COMPAT
int inet_csk_compat_getsockopt(struct sock *sk, int level, int optname,
			       char __user *optval, int __user *optlen)
{
	const struct inet_connection_sock *icsk = inet_csk(sk);

	if (icsk->icsk_af_ops->compat_getsockopt != NULL)
		return icsk->icsk_af_ops->compat_getsockopt(sk, level, optname,
							    optval, optlen);
	return icsk->icsk_af_ops->getsockopt(sk, level, optname,
					     optval, optlen);
}
EXPORT_SYMBOL_GPL(inet_csk_compat_getsockopt);

int inet_csk_compat_setsockopt(struct sock *sk, int level, int optname,
			       char __user *optval, unsigned int optlen)
{
	const struct inet_connection_sock *icsk = inet_csk(sk);

	if (icsk->icsk_af_ops->compat_setsockopt != NULL)
		return icsk->icsk_af_ops->compat_setsockopt(sk, level, optname,
							    optval, optlen);
	return icsk->icsk_af_ops->setsockopt(sk, level, optname,
					     optval, optlen);
}
EXPORT_SYMBOL_GPL(inet_csk_compat_setsockopt);
#endif
=======
static struct dst_entry *inet_csk_rebuild_route(struct sock *sk, struct flowi *fl)
{
	const struct inet_sock *inet = inet_sk(sk);
	const struct ip_options_rcu *inet_opt;
	__be32 daddr = inet->inet_daddr;
	struct flowi4 *fl4;
	struct rtable *rt;

	rcu_read_lock();
	inet_opt = rcu_dereference(inet->inet_opt);
	if (inet_opt && inet_opt->opt.srr)
		daddr = inet_opt->opt.faddr;
	fl4 = &fl->u.ip4;
	rt = ip_route_output_ports(sock_net(sk), fl4, sk, daddr,
				   inet->inet_saddr, inet->inet_dport,
				   inet->inet_sport, sk->sk_protocol,
				   ip_sock_rt_tos(sk), sk->sk_bound_dev_if);
	if (IS_ERR(rt))
		rt = NULL;
	if (rt)
		sk_setup_caps(sk, &rt->dst);
	rcu_read_unlock();

	return &rt->dst;
}

struct dst_entry *inet_csk_update_pmtu(struct sock *sk, u32 mtu)
{
	struct dst_entry *dst = __sk_dst_check(sk, 0);
	struct inet_sock *inet = inet_sk(sk);

	if (!dst) {
		dst = inet_csk_rebuild_route(sk, &inet->cork.fl);
		if (!dst)
			goto out;
	}
	dst->ops->update_pmtu(dst, sk, NULL, mtu, true);

	dst = __sk_dst_check(sk, 0);
	if (!dst)
		dst = inet_csk_rebuild_route(sk, &inet->cork.fl);
out:
	return dst;
}
EXPORT_SYMBOL_GPL(inet_csk_update_pmtu);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
