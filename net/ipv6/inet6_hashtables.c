<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * INET		An implementation of the TCP/IP protocol suite for the LINUX
 *		operating system.  INET is implemented using the BSD Socket
 *		interface as the means of communication with the user level.
 *
 *		Generic INET6 transport hashtables
 *
 * Authors:	Lotsa people, from code originally in tcp, generalised here
<<<<<<< HEAD
 * 		by Arnaldo Carvalho de Melo <acme@mandriva.com>
 *
 *	This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
=======
 *		by Arnaldo Carvalho de Melo <acme@mandriva.com>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/module.h>
#include <linux/random.h>

<<<<<<< HEAD
=======
#include <net/addrconf.h>
#include <net/hotdata.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <net/inet_connection_sock.h>
#include <net/inet_hashtables.h>
#include <net/inet6_hashtables.h>
#include <net/secure_seq.h>
#include <net/ip.h>
<<<<<<< HEAD

int __inet6_hash(struct sock *sk, struct inet_timewait_sock *tw)
{
	struct inet_hashinfo *hashinfo = sk->sk_prot->h.hashinfo;
	int twrefcnt = 0;

	WARN_ON(!sk_unhashed(sk));

	if (sk->sk_state == TCP_LISTEN) {
		struct inet_listen_hashbucket *ilb;

		ilb = &hashinfo->listening_hash[inet_sk_listen_hashfn(sk)];
		spin_lock(&ilb->lock);
		__sk_nulls_add_node_rcu(sk, &ilb->head);
		spin_unlock(&ilb->lock);
	} else {
		unsigned int hash;
		struct hlist_nulls_head *list;
		spinlock_t *lock;

		sk->sk_hash = hash = inet6_sk_ehashfn(sk);
		list = &inet_ehash_bucket(hashinfo, hash)->chain;
		lock = inet_ehash_lockp(hashinfo, hash);
		spin_lock(lock);
		__sk_nulls_add_node_rcu(sk, list);
		if (tw) {
			WARN_ON(sk->sk_hash != tw->tw_hash);
			twrefcnt = inet_twsk_unhash(tw);
		}
		spin_unlock(lock);
	}

	sock_prot_inuse_add(sock_net(sk), sk->sk_prot, 1);
	return twrefcnt;
}
EXPORT_SYMBOL(__inet6_hash);
=======
#include <net/sock_reuseport.h>

u32 inet6_ehashfn(const struct net *net,
		  const struct in6_addr *laddr, const u16 lport,
		  const struct in6_addr *faddr, const __be16 fport)
{
	u32 lhash, fhash;

	net_get_random_once(&inet6_ehash_secret, sizeof(inet6_ehash_secret));
	net_get_random_once(&tcp_ipv6_hash_secret, sizeof(tcp_ipv6_hash_secret));

	lhash = (__force u32)laddr->s6_addr32[3];
	fhash = __ipv6_addr_jhash(faddr, tcp_ipv6_hash_secret);

	return __inet6_ehashfn(lhash, lport, fhash, fport,
			       inet6_ehash_secret + net_hash_mix(net));
}
EXPORT_SYMBOL_GPL(inet6_ehashfn);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Sockets in TCP_CLOSE state are _always_ taken out of the hash, so
 * we need not check it for TCP lookups anymore, thanks Alexey. -DaveM
 *
 * The sockhash lock must be held as a reader here.
 */
struct sock *__inet6_lookup_established(struct net *net,
					struct inet_hashinfo *hashinfo,
					   const struct in6_addr *saddr,
					   const __be16 sport,
					   const struct in6_addr *daddr,
					   const u16 hnum,
<<<<<<< HEAD
					   const int dif)
=======
					   const int dif, const int sdif)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct sock *sk;
	const struct hlist_nulls_node *node;
	const __portpair ports = INET_COMBINED_PORTS(sport, hnum);
	/* Optimize here for direct hit, only listening connections can
	 * have wildcards anyways.
	 */
	unsigned int hash = inet6_ehashfn(net, daddr, hnum, saddr, sport);
	unsigned int slot = hash & hashinfo->ehash_mask;
	struct inet_ehash_bucket *head = &hashinfo->ehash[slot];


<<<<<<< HEAD
	rcu_read_lock();
begin:
	sk_nulls_for_each_rcu(sk, node, &head->chain) {
		/* For IPV6 do the cheaper port and family tests first. */
		if (INET6_MATCH(sk, net, hash, saddr, daddr, ports, dif)) {
			if (unlikely(!atomic_inc_not_zero(&sk->sk_refcnt)))
				goto begintw;
			if (!INET6_MATCH(sk, net, hash, saddr, daddr, ports, dif)) {
				sock_put(sk);
				goto begin;
			}
		goto out;
		}
	}
	if (get_nulls_value(node) != slot)
		goto begin;

begintw:
	/* Must check for a TIME_WAIT'er before going to listener hash. */
	sk_nulls_for_each_rcu(sk, node, &head->twchain) {
		if (INET6_TW_MATCH(sk, net, hash, saddr, daddr, ports, dif)) {
			if (unlikely(!atomic_inc_not_zero(&sk->sk_refcnt))) {
				sk = NULL;
				goto out;
			}
			if (!INET6_TW_MATCH(sk, net, hash, saddr, daddr, ports, dif)) {
				inet_twsk_put(inet_twsk(sk));
				goto begintw;
			}
			goto out;
		}
	}
	if (get_nulls_value(node) != slot)
		goto begintw;
	sk = NULL;
out:
	rcu_read_unlock();
=======
begin:
	sk_nulls_for_each_rcu(sk, node, &head->chain) {
		if (sk->sk_hash != hash)
			continue;
		if (!inet6_match(net, sk, saddr, daddr, ports, dif, sdif))
			continue;
		if (unlikely(!refcount_inc_not_zero(&sk->sk_refcnt)))
			goto out;

		if (unlikely(!inet6_match(net, sk, saddr, daddr, ports, dif, sdif))) {
			sock_gen_put(sk);
			goto begin;
		}
		goto found;
	}
	if (get_nulls_value(node) != slot)
		goto begin;
out:
	sk = NULL;
found:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return sk;
}
EXPORT_SYMBOL(__inet6_lookup_established);

static inline int compute_score(struct sock *sk, struct net *net,
				const unsigned short hnum,
				const struct in6_addr *daddr,
<<<<<<< HEAD
				const int dif)
=======
				const int dif, const int sdif)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int score = -1;

	if (net_eq(sock_net(sk), net) && inet_sk(sk)->inet_num == hnum &&
	    sk->sk_family == PF_INET6) {
<<<<<<< HEAD
		const struct ipv6_pinfo *np = inet6_sk(sk);

		score = 1;
		if (!ipv6_addr_any(&np->rcv_saddr)) {
			if (!ipv6_addr_equal(&np->rcv_saddr, daddr))
				return -1;
			score++;
		}
		if (sk->sk_bound_dev_if) {
			if (sk->sk_bound_dev_if != dif)
				return -1;
			score++;
		}
=======
		if (!ipv6_addr_equal(&sk->sk_v6_rcv_saddr, daddr))
			return -1;

		if (!inet_sk_bound_dev_eq(net, sk->sk_bound_dev_if, dif, sdif))
			return -1;

		score =  sk->sk_bound_dev_if ? 2 : 1;
		if (READ_ONCE(sk->sk_incoming_cpu) == raw_smp_processor_id())
			score++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return score;
}

<<<<<<< HEAD
struct sock *inet6_lookup_listener(struct net *net,
		struct inet_hashinfo *hashinfo, const struct in6_addr *daddr,
		const unsigned short hnum, const int dif)
{
	struct sock *sk;
	const struct hlist_nulls_node *node;
	struct sock *result;
	int score, hiscore;
	unsigned int hash = inet_lhashfn(net, hnum);
	struct inet_listen_hashbucket *ilb = &hashinfo->listening_hash[hash];

	rcu_read_lock();
begin:
	result = NULL;
	hiscore = -1;
	sk_nulls_for_each(sk, node, &ilb->head) {
		score = compute_score(sk, net, hnum, daddr, dif);
		if (score > hiscore) {
			hiscore = score;
			result = sk;
		}
	}
	/*
	 * if the nulls value we got at the end of this lookup is
	 * not the expected one, we must restart lookup.
	 * We probably met an item that was moved to another chain.
	 */
	if (get_nulls_value(node) != hash + LISTENING_NULLS_BASE)
		goto begin;
	if (result) {
		if (unlikely(!atomic_inc_not_zero(&result->sk_refcnt)))
			result = NULL;
		else if (unlikely(compute_score(result, net, hnum, daddr,
				  dif) < hiscore)) {
			sock_put(result);
			goto begin;
		}
	}
	rcu_read_unlock();
	return result;
}

EXPORT_SYMBOL_GPL(inet6_lookup_listener);

struct sock *inet6_lookup(struct net *net, struct inet_hashinfo *hashinfo,
=======
/**
 * inet6_lookup_reuseport() - execute reuseport logic on AF_INET6 socket if necessary.
 * @net: network namespace.
 * @sk: AF_INET6 socket, must be in TCP_LISTEN state for TCP or TCP_CLOSE for UDP.
 * @skb: context for a potential SK_REUSEPORT program.
 * @doff: header offset.
 * @saddr: source address.
 * @sport: source port.
 * @daddr: destination address.
 * @hnum: destination port in host byte order.
 * @ehashfn: hash function used to generate the fallback hash.
 *
 * Return: NULL if sk doesn't have SO_REUSEPORT set, otherwise a pointer to
 *         the selected sock or an error.
 */
struct sock *inet6_lookup_reuseport(struct net *net, struct sock *sk,
				    struct sk_buff *skb, int doff,
				    const struct in6_addr *saddr,
				    __be16 sport,
				    const struct in6_addr *daddr,
				    unsigned short hnum,
				    inet6_ehashfn_t *ehashfn)
{
	struct sock *reuse_sk = NULL;
	u32 phash;

	if (sk->sk_reuseport) {
		phash = INDIRECT_CALL_INET(ehashfn, udp6_ehashfn, inet6_ehashfn,
					   net, daddr, hnum, saddr, sport);
		reuse_sk = reuseport_select_sock(sk, phash, skb, doff);
	}
	return reuse_sk;
}
EXPORT_SYMBOL_GPL(inet6_lookup_reuseport);

/* called with rcu_read_lock() */
static struct sock *inet6_lhash2_lookup(struct net *net,
		struct inet_listen_hashbucket *ilb2,
		struct sk_buff *skb, int doff,
		const struct in6_addr *saddr,
		const __be16 sport, const struct in6_addr *daddr,
		const unsigned short hnum, const int dif, const int sdif)
{
	struct sock *sk, *result = NULL;
	struct hlist_nulls_node *node;
	int score, hiscore = 0;

	sk_nulls_for_each_rcu(sk, node, &ilb2->nulls_head) {
		score = compute_score(sk, net, hnum, daddr, dif, sdif);
		if (score > hiscore) {
			result = inet6_lookup_reuseport(net, sk, skb, doff,
							saddr, sport, daddr, hnum, inet6_ehashfn);
			if (result)
				return result;

			result = sk;
			hiscore = score;
		}
	}

	return result;
}

struct sock *inet6_lookup_run_sk_lookup(struct net *net,
					int protocol,
					struct sk_buff *skb, int doff,
					const struct in6_addr *saddr,
					const __be16 sport,
					const struct in6_addr *daddr,
					const u16 hnum, const int dif,
					inet6_ehashfn_t *ehashfn)
{
	struct sock *sk, *reuse_sk;
	bool no_reuseport;

	no_reuseport = bpf_sk_lookup_run_v6(net, protocol, saddr, sport,
					    daddr, hnum, dif, &sk);
	if (no_reuseport || IS_ERR_OR_NULL(sk))
		return sk;

	reuse_sk = inet6_lookup_reuseport(net, sk, skb, doff,
					  saddr, sport, daddr, hnum, ehashfn);
	if (reuse_sk)
		sk = reuse_sk;
	return sk;
}
EXPORT_SYMBOL_GPL(inet6_lookup_run_sk_lookup);

struct sock *inet6_lookup_listener(struct net *net,
		struct inet_hashinfo *hashinfo,
		struct sk_buff *skb, int doff,
		const struct in6_addr *saddr,
		const __be16 sport, const struct in6_addr *daddr,
		const unsigned short hnum, const int dif, const int sdif)
{
	struct inet_listen_hashbucket *ilb2;
	struct sock *result = NULL;
	unsigned int hash2;

	/* Lookup redirect from BPF */
	if (static_branch_unlikely(&bpf_sk_lookup_enabled) &&
	    hashinfo == net->ipv4.tcp_death_row.hashinfo) {
		result = inet6_lookup_run_sk_lookup(net, IPPROTO_TCP, skb, doff,
						    saddr, sport, daddr, hnum, dif,
						    inet6_ehashfn);
		if (result)
			goto done;
	}

	hash2 = ipv6_portaddr_hash(net, daddr, hnum);
	ilb2 = inet_lhash2_bucket(hashinfo, hash2);

	result = inet6_lhash2_lookup(net, ilb2, skb, doff,
				     saddr, sport, daddr, hnum,
				     dif, sdif);
	if (result)
		goto done;

	/* Lookup lhash2 with in6addr_any */
	hash2 = ipv6_portaddr_hash(net, &in6addr_any, hnum);
	ilb2 = inet_lhash2_bucket(hashinfo, hash2);

	result = inet6_lhash2_lookup(net, ilb2, skb, doff,
				     saddr, sport, &in6addr_any, hnum,
				     dif, sdif);
done:
	if (IS_ERR(result))
		return NULL;
	return result;
}
EXPORT_SYMBOL_GPL(inet6_lookup_listener);

struct sock *inet6_lookup(struct net *net, struct inet_hashinfo *hashinfo,
			  struct sk_buff *skb, int doff,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			  const struct in6_addr *saddr, const __be16 sport,
			  const struct in6_addr *daddr, const __be16 dport,
			  const int dif)
{
	struct sock *sk;
<<<<<<< HEAD

	local_bh_disable();
	sk = __inet6_lookup(net, hashinfo, saddr, sport, daddr, ntohs(dport), dif);
	local_bh_enable();

	return sk;
}

=======
	bool refcounted;

	sk = __inet6_lookup(net, hashinfo, skb, doff, saddr, sport, daddr,
			    ntohs(dport), dif, 0, &refcounted);
	if (sk && !refcounted && !refcount_inc_not_zero(&sk->sk_refcnt))
		sk = NULL;
	return sk;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
EXPORT_SYMBOL_GPL(inet6_lookup);

static int __inet6_check_established(struct inet_timewait_death_row *death_row,
				     struct sock *sk, const __u16 lport,
				     struct inet_timewait_sock **twp)
{
	struct inet_hashinfo *hinfo = death_row->hashinfo;
	struct inet_sock *inet = inet_sk(sk);
<<<<<<< HEAD
	const struct ipv6_pinfo *np = inet6_sk(sk);
	const struct in6_addr *daddr = &np->rcv_saddr;
	const struct in6_addr *saddr = &np->daddr;
	const int dif = sk->sk_bound_dev_if;
	const __portpair ports = INET_COMBINED_PORTS(inet->inet_dport, lport);
	struct net *net = sock_net(sk);
=======
	const struct in6_addr *daddr = &sk->sk_v6_rcv_saddr;
	const struct in6_addr *saddr = &sk->sk_v6_daddr;
	const int dif = sk->sk_bound_dev_if;
	struct net *net = sock_net(sk);
	const int sdif = l3mdev_master_ifindex_by_index(net, dif);
	const __portpair ports = INET_COMBINED_PORTS(inet->inet_dport, lport);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	const unsigned int hash = inet6_ehashfn(net, daddr, lport, saddr,
						inet->inet_dport);
	struct inet_ehash_bucket *head = inet_ehash_bucket(hinfo, hash);
	spinlock_t *lock = inet_ehash_lockp(hinfo, hash);
	struct sock *sk2;
	const struct hlist_nulls_node *node;
<<<<<<< HEAD
	struct inet_timewait_sock *tw;
	int twrefcnt = 0;

	spin_lock(lock);

	/* Check TIME-WAIT sockets first. */
	sk_nulls_for_each(sk2, node, &head->twchain) {
		tw = inet_twsk(sk2);

		if (INET6_TW_MATCH(sk2, net, hash, saddr, daddr, ports, dif)) {
			if (twsk_unique(sk, sk2, twp))
				goto unique;
			else
				goto not_unique;
		}
	}
	tw = NULL;

	/* And established part... */
	sk_nulls_for_each(sk2, node, &head->chain) {
		if (INET6_MATCH(sk2, net, hash, saddr, daddr, ports, dif))
			goto not_unique;
	}

unique:
	/* Must record num and sport now. Otherwise we will see
	 * in hash table socket with a funny identity. */
=======
	struct inet_timewait_sock *tw = NULL;

	spin_lock(lock);

	sk_nulls_for_each(sk2, node, &head->chain) {
		if (sk2->sk_hash != hash)
			continue;

		if (likely(inet6_match(net, sk2, saddr, daddr, ports,
				       dif, sdif))) {
			if (sk2->sk_state == TCP_TIME_WAIT) {
				tw = inet_twsk(sk2);
				if (twsk_unique(sk, sk2, twp))
					break;
			}
			goto not_unique;
		}
	}

	/* Must record num and sport now. Otherwise we will see
	 * in hash table socket with a funny identity.
	 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	inet->inet_num = lport;
	inet->inet_sport = htons(lport);
	sk->sk_hash = hash;
	WARN_ON(!sk_unhashed(sk));
	__sk_nulls_add_node_rcu(sk, &head->chain);
	if (tw) {
<<<<<<< HEAD
		twrefcnt = inet_twsk_unhash(tw);
		NET_INC_STATS_BH(net, LINUX_MIB_TIMEWAITRECYCLED);
	}
	spin_unlock(lock);
	if (twrefcnt)
		inet_twsk_put(tw);
=======
		sk_nulls_del_node_init_rcu((struct sock *)tw);
		__NET_INC_STATS(net, LINUX_MIB_TIMEWAITRECYCLED);
	}
	spin_unlock(lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	sock_prot_inuse_add(sock_net(sk), sk->sk_prot, 1);

	if (twp) {
		*twp = tw;
	} else if (tw) {
		/* Silly. Should hash-dance instead... */
<<<<<<< HEAD
		inet_twsk_deschedule(tw, death_row);

		inet_twsk_put(tw);
=======
		inet_twsk_deschedule_put(tw);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return 0;

not_unique:
	spin_unlock(lock);
	return -EADDRNOTAVAIL;
}

<<<<<<< HEAD
static inline u32 inet6_sk_port_offset(const struct sock *sk)
{
	const struct inet_sock *inet = inet_sk(sk);
	const struct ipv6_pinfo *np = inet6_sk(sk);
	return secure_ipv6_port_ephemeral(np->rcv_saddr.s6_addr32,
					  np->daddr.s6_addr32,
=======
static u64 inet6_sk_port_offset(const struct sock *sk)
{
	const struct inet_sock *inet = inet_sk(sk);

	return secure_ipv6_port_ephemeral(sk->sk_v6_rcv_saddr.s6_addr32,
					  sk->sk_v6_daddr.s6_addr32,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					  inet->inet_dport);
}

int inet6_hash_connect(struct inet_timewait_death_row *death_row,
		       struct sock *sk)
{
<<<<<<< HEAD
	return __inet_hash_connect(death_row, sk, inet6_sk_port_offset(sk),
			__inet6_check_established, __inet6_hash);
}

EXPORT_SYMBOL_GPL(inet6_hash_connect);
=======
	u64 port_offset = 0;

	if (!inet_sk(sk)->inet_num)
		port_offset = inet6_sk_port_offset(sk);
	return __inet_hash_connect(death_row, sk, port_offset,
				   __inet6_check_established);
}
EXPORT_SYMBOL_GPL(inet6_hash_connect);

int inet6_hash(struct sock *sk)
{
	int err = 0;

	if (sk->sk_state != TCP_CLOSE)
		err = __inet_hash(sk, NULL);

	return err;
}
EXPORT_SYMBOL_GPL(inet6_hash);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
