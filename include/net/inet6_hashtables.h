<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * INET		An implementation of the TCP/IP protocol suite for the LINUX
 *		operating system.  INET is implemented using the BSD Socket
 *		interface as the means of communication with the user level.
 *
 * Authors:	Lotsa people, from code originally in tcp
<<<<<<< HEAD
 *
 *	This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef _INET6_HASHTABLES_H
#define _INET6_HASHTABLES_H


#if IS_ENABLED(CONFIG_IPV6)
#include <linux/in6.h>
#include <linux/ipv6.h>
#include <linux/types.h>
#include <linux/jhash.h>

#include <net/inet_sock.h>

#include <net/ipv6.h>
#include <net/netns/hash.h>

struct inet_hashinfo;

<<<<<<< HEAD
static inline unsigned int inet6_ehashfn(struct net *net,
				const struct in6_addr *laddr, const u16 lport,
				const struct in6_addr *faddr, const __be16 fport)
{
	u32 ports = (((u32)lport) << 16) | (__force u32)fport;

	return jhash_3words((__force u32)laddr->s6_addr32[3],
			    ipv6_addr_jhash(faddr),
			    ports,
			    inet_ehash_secret + net_hash_mix(net));
}

static inline int inet6_sk_ehashfn(const struct sock *sk)
{
	const struct inet_sock *inet = inet_sk(sk);
	const struct ipv6_pinfo *np = inet6_sk(sk);
	const struct in6_addr *laddr = &np->rcv_saddr;
	const struct in6_addr *faddr = &np->daddr;
	const __u16 lport = inet->inet_num;
	const __be16 fport = inet->inet_dport;
	struct net *net = sock_net(sk);

	return inet6_ehashfn(net, laddr, lport, faddr, fport);
}

extern int __inet6_hash(struct sock *sk, struct inet_timewait_sock *twp);

=======
static inline unsigned int __inet6_ehashfn(const u32 lhash,
				    const u16 lport,
				    const u32 fhash,
				    const __be16 fport,
				    const u32 initval)
{
	const u32 ports = (((u32)lport) << 16) | (__force u32)fport;
	return jhash_3words(lhash, fhash, ports, initval);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Sockets in TCP_CLOSE state are _always_ taken out of the hash, so
 * we need not check it for TCP lookups anymore, thanks Alexey. -DaveM
 *
 * The sockhash lock must be held as a reader here.
 */
<<<<<<< HEAD
extern struct sock *__inet6_lookup_established(struct net *net,
					   struct inet_hashinfo *hashinfo,
					   const struct in6_addr *saddr,
					   const __be16 sport,
					   const struct in6_addr *daddr,
					   const u16 hnum,
					   const int dif);

extern struct sock *inet6_lookup_listener(struct net *net,
					  struct inet_hashinfo *hashinfo,
					  const struct in6_addr *daddr,
					  const unsigned short hnum,
					  const int dif);

static inline struct sock *__inet6_lookup(struct net *net,
					  struct inet_hashinfo *hashinfo,
=======
struct sock *__inet6_lookup_established(struct net *net,
					struct inet_hashinfo *hashinfo,
					const struct in6_addr *saddr,
					const __be16 sport,
					const struct in6_addr *daddr,
					const u16 hnum, const int dif,
					const int sdif);

typedef u32 (inet6_ehashfn_t)(const struct net *net,
			       const struct in6_addr *laddr, const u16 lport,
			       const struct in6_addr *faddr, const __be16 fport);

inet6_ehashfn_t inet6_ehashfn;

INDIRECT_CALLABLE_DECLARE(inet6_ehashfn_t udp6_ehashfn);

struct sock *inet6_lookup_reuseport(struct net *net, struct sock *sk,
				    struct sk_buff *skb, int doff,
				    const struct in6_addr *saddr,
				    __be16 sport,
				    const struct in6_addr *daddr,
				    unsigned short hnum,
				    inet6_ehashfn_t *ehashfn);

struct sock *inet6_lookup_listener(struct net *net,
				   struct inet_hashinfo *hashinfo,
				   struct sk_buff *skb, int doff,
				   const struct in6_addr *saddr,
				   const __be16 sport,
				   const struct in6_addr *daddr,
				   const unsigned short hnum,
				   const int dif, const int sdif);

struct sock *inet6_lookup_run_sk_lookup(struct net *net,
					int protocol,
					struct sk_buff *skb, int doff,
					const struct in6_addr *saddr,
					const __be16 sport,
					const struct in6_addr *daddr,
					const u16 hnum, const int dif,
					inet6_ehashfn_t *ehashfn);

static inline struct sock *__inet6_lookup(struct net *net,
					  struct inet_hashinfo *hashinfo,
					  struct sk_buff *skb, int doff,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					  const struct in6_addr *saddr,
					  const __be16 sport,
					  const struct in6_addr *daddr,
					  const u16 hnum,
<<<<<<< HEAD
					  const int dif)
{
	struct sock *sk = __inet6_lookup_established(net, hashinfo, saddr,
						sport, daddr, hnum, dif);
	if (sk)
		return sk;

	return inet6_lookup_listener(net, hashinfo, daddr, hnum, dif);
}

static inline struct sock *__inet6_lookup_skb(struct inet_hashinfo *hashinfo,
					      struct sk_buff *skb,
					      const __be16 sport,
					      const __be16 dport)
{
	struct sock *sk;

	if (unlikely(sk = skb_steal_sock(skb)))
		return sk;
	else return __inet6_lookup(dev_net(skb_dst(skb)->dev), hashinfo,
				   &ipv6_hdr(skb)->saddr, sport,
				   &ipv6_hdr(skb)->daddr, ntohs(dport),
				   inet6_iif(skb));
}

extern struct sock *inet6_lookup(struct net *net, struct inet_hashinfo *hashinfo,
				 const struct in6_addr *saddr, const __be16 sport,
				 const struct in6_addr *daddr, const __be16 dport,
				 const int dif);
#endif /* IS_ENABLED(CONFIG_IPV6) */
=======
					  const int dif, const int sdif,
					  bool *refcounted)
{
	struct sock *sk = __inet6_lookup_established(net, hashinfo, saddr,
						     sport, daddr, hnum,
						     dif, sdif);
	*refcounted = true;
	if (sk)
		return sk;
	*refcounted = false;
	return inet6_lookup_listener(net, hashinfo, skb, doff, saddr, sport,
				     daddr, hnum, dif, sdif);
}

static inline
struct sock *inet6_steal_sock(struct net *net, struct sk_buff *skb, int doff,
			      const struct in6_addr *saddr, const __be16 sport,
			      const struct in6_addr *daddr, const __be16 dport,
			      bool *refcounted, inet6_ehashfn_t *ehashfn)
{
	struct sock *sk, *reuse_sk;
	bool prefetched;

	sk = skb_steal_sock(skb, refcounted, &prefetched);
	if (!sk)
		return NULL;

	if (!prefetched || !sk_fullsock(sk))
		return sk;

	if (sk->sk_protocol == IPPROTO_TCP) {
		if (sk->sk_state != TCP_LISTEN)
			return sk;
	} else if (sk->sk_protocol == IPPROTO_UDP) {
		if (sk->sk_state != TCP_CLOSE)
			return sk;
	} else {
		return sk;
	}

	reuse_sk = inet6_lookup_reuseport(net, sk, skb, doff,
					  saddr, sport, daddr, ntohs(dport),
					  ehashfn);
	if (!reuse_sk)
		return sk;

	/* We've chosen a new reuseport sock which is never refcounted. This
	 * implies that sk also isn't refcounted.
	 */
	WARN_ON_ONCE(*refcounted);

	return reuse_sk;
}

static inline struct sock *__inet6_lookup_skb(struct inet_hashinfo *hashinfo,
					      struct sk_buff *skb, int doff,
					      const __be16 sport,
					      const __be16 dport,
					      int iif, int sdif,
					      bool *refcounted)
{
	struct net *net = dev_net(skb_dst(skb)->dev);
	const struct ipv6hdr *ip6h = ipv6_hdr(skb);
	struct sock *sk;

	sk = inet6_steal_sock(net, skb, doff, &ip6h->saddr, sport, &ip6h->daddr, dport,
			      refcounted, inet6_ehashfn);
	if (IS_ERR(sk))
		return NULL;
	if (sk)
		return sk;

	return __inet6_lookup(net, hashinfo, skb,
			      doff, &ip6h->saddr, sport,
			      &ip6h->daddr, ntohs(dport),
			      iif, sdif, refcounted);
}

struct sock *inet6_lookup(struct net *net, struct inet_hashinfo *hashinfo,
			  struct sk_buff *skb, int doff,
			  const struct in6_addr *saddr, const __be16 sport,
			  const struct in6_addr *daddr, const __be16 dport,
			  const int dif);

int inet6_hash(struct sock *sk);

static inline bool inet6_match(struct net *net, const struct sock *sk,
			       const struct in6_addr *saddr,
			       const struct in6_addr *daddr,
			       const __portpair ports,
			       const int dif, const int sdif)
{
	if (!net_eq(sock_net(sk), net) ||
	    sk->sk_family != AF_INET6 ||
	    sk->sk_portpair != ports ||
	    !ipv6_addr_equal(&sk->sk_v6_daddr, saddr) ||
	    !ipv6_addr_equal(&sk->sk_v6_rcv_saddr, daddr))
		return false;

	/* READ_ONCE() paired with WRITE_ONCE() in sock_bindtoindex_locked() */
	return inet_sk_bound_dev_eq(net, READ_ONCE(sk->sk_bound_dev_if), dif,
				    sdif);
}
#endif /* IS_ENABLED(CONFIG_IPV6) */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _INET6_HASHTABLES_H */
