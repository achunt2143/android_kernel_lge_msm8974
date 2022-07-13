<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *	UDP over IPv6
 *	Linux INET6 implementation
 *
 *	Authors:
 *	Pedro Roque		<roque@di.fc.ul.pt>
 *
 *	Based on linux/ipv4/udp.c
 *
 *	Fixes:
 *	Hideaki YOSHIFUJI	:	sin6_scope_id support
 *	YOSHIFUJI Hideaki @USAGI and:	Support IPV6_V6ONLY socket option, which
 *	Alexey Kuznetsov		allow both IPv4 and IPv6 sockets to bind
 *					a single port at the same time.
 *      Kazunori MIYAZAWA @USAGI:       change process style to use ip6_append_data
 *      YOSHIFUJI Hideaki @USAGI:	convert /proc/net/udp6 to seq_file.
<<<<<<< HEAD
 *
 *	This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
 */

=======
 */

#include <linux/bpf-cgroup.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/socket.h>
#include <linux/sockios.h>
#include <linux/net.h>
#include <linux/in6.h>
#include <linux/netdevice.h>
#include <linux/if_arp.h>
#include <linux/ipv6.h>
#include <linux/icmpv6.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/slab.h>
<<<<<<< HEAD
#include <asm/uaccess.h>

=======
#include <linux/uaccess.h>
#include <linux/indirect_call_wrapper.h>

#include <net/addrconf.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <net/ndisc.h>
#include <net/protocol.h>
#include <net/transp_v6.h>
#include <net/ip6_route.h>
#include <net/raw.h>
<<<<<<< HEAD
#include <net/tcp_states.h>
#include <net/ip6_checksum.h>
#include <net/xfrm.h>

#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include "udp_impl.h"

int ipv6_rcv_saddr_equal(const struct sock *sk, const struct sock *sk2)
{
	const struct in6_addr *sk_rcv_saddr6 = &inet6_sk(sk)->rcv_saddr;
	const struct in6_addr *sk2_rcv_saddr6 = inet6_rcv_saddr(sk2);
	__be32 sk1_rcv_saddr = sk_rcv_saddr(sk);
	__be32 sk2_rcv_saddr = sk_rcv_saddr(sk2);
	int sk_ipv6only = ipv6_only_sock(sk);
	int sk2_ipv6only = inet_v6_ipv6only(sk2);
	int addr_type = ipv6_addr_type(sk_rcv_saddr6);
	int addr_type2 = sk2_rcv_saddr6 ? ipv6_addr_type(sk2_rcv_saddr6) : IPV6_ADDR_MAPPED;

	/* if both are mapped, treat as IPv4 */
	if (addr_type == IPV6_ADDR_MAPPED && addr_type2 == IPV6_ADDR_MAPPED)
		return (!sk2_ipv6only &&
			(!sk1_rcv_saddr || !sk2_rcv_saddr ||
			  sk1_rcv_saddr == sk2_rcv_saddr));

	if (addr_type2 == IPV6_ADDR_ANY &&
	    !(sk2_ipv6only && addr_type == IPV6_ADDR_MAPPED))
		return 1;

	if (addr_type == IPV6_ADDR_ANY &&
	    !(sk_ipv6only && addr_type2 == IPV6_ADDR_MAPPED))
		return 1;

	if (sk2_rcv_saddr6 &&
	    ipv6_addr_equal(sk_rcv_saddr6, sk2_rcv_saddr6))
		return 1;

	return 0;
}

static unsigned int udp6_portaddr_hash(struct net *net,
				       const struct in6_addr *addr6,
				       unsigned int port)
{
	unsigned int hash, mix = net_hash_mix(net);

	if (ipv6_addr_any(addr6))
		hash = jhash_1word(0, mix);
	else if (ipv6_addr_v4mapped(addr6))
		hash = jhash_1word((__force u32)addr6->s6_addr32[3], mix);
	else
		hash = jhash2((__force u32 *)addr6->s6_addr32, 4, mix);

	return hash ^ port;
}


int udp_v6_get_port(struct sock *sk, unsigned short snum)
{
	unsigned int hash2_nulladdr =
		udp6_portaddr_hash(sock_net(sk), &in6addr_any, snum);
	unsigned int hash2_partial = 
		udp6_portaddr_hash(sock_net(sk), &inet6_sk(sk)->rcv_saddr, 0);

	/* precompute partial secondary hash */
	udp_sk(sk)->udp_portaddr_hash = hash2_partial;
	return udp_lib_get_port(sk, snum, ipv6_rcv_saddr_equal, hash2_nulladdr);
}

static void udp_v6_rehash(struct sock *sk)
{
	u16 new_hash = udp6_portaddr_hash(sock_net(sk),
					  &inet6_sk(sk)->rcv_saddr,
=======
#include <net/seg6.h>
#include <net/tcp_states.h>
#include <net/ip6_checksum.h>
#include <net/ip6_tunnel.h>
#include <trace/events/udp.h>
#include <net/xfrm.h>
#include <net/inet_hashtables.h>
#include <net/inet6_hashtables.h>
#include <net/busy_poll.h>
#include <net/sock_reuseport.h>
#include <net/gro.h>

#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <trace/events/skb.h>
#include "udp_impl.h"

static void udpv6_destruct_sock(struct sock *sk)
{
	udp_destruct_common(sk);
	inet6_sock_destruct(sk);
}

int udpv6_init_sock(struct sock *sk)
{
	udp_lib_init_sock(sk);
	sk->sk_destruct = udpv6_destruct_sock;
	set_bit(SOCK_SUPPORT_ZC, &sk->sk_socket->flags);
	return 0;
}

INDIRECT_CALLABLE_SCOPE
u32 udp6_ehashfn(const struct net *net,
		 const struct in6_addr *laddr,
		 const u16 lport,
		 const struct in6_addr *faddr,
		 const __be16 fport)
{
	u32 lhash, fhash;

	net_get_random_once(&udp6_ehash_secret,
			    sizeof(udp6_ehash_secret));
	net_get_random_once(&udp_ipv6_hash_secret,
			    sizeof(udp_ipv6_hash_secret));

	lhash = (__force u32)laddr->s6_addr32[3];
	fhash = __ipv6_addr_jhash(faddr, udp_ipv6_hash_secret);

	return __inet6_ehashfn(lhash, lport, fhash, fport,
			       udp6_ehash_secret + net_hash_mix(net));
}

int udp_v6_get_port(struct sock *sk, unsigned short snum)
{
	unsigned int hash2_nulladdr =
		ipv6_portaddr_hash(sock_net(sk), &in6addr_any, snum);
	unsigned int hash2_partial =
		ipv6_portaddr_hash(sock_net(sk), &sk->sk_v6_rcv_saddr, 0);

	/* precompute partial secondary hash */
	udp_sk(sk)->udp_portaddr_hash = hash2_partial;
	return udp_lib_get_port(sk, snum, hash2_nulladdr);
}

void udp_v6_rehash(struct sock *sk)
{
	u16 new_hash = ipv6_portaddr_hash(sock_net(sk),
					  &sk->sk_v6_rcv_saddr,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					  inet_sk(sk)->inet_num);

	udp_lib_rehash(sk, new_hash);
}

<<<<<<< HEAD
static inline int compute_score(struct sock *sk, struct net *net,
				unsigned short hnum,
				const struct in6_addr *saddr, __be16 sport,
				const struct in6_addr *daddr, __be16 dport,
				int dif)
{
	int score = -1;

	if (net_eq(sock_net(sk), net) && udp_sk(sk)->udp_port_hash == hnum &&
			sk->sk_family == PF_INET6) {
		struct ipv6_pinfo *np = inet6_sk(sk);
		struct inet_sock *inet = inet_sk(sk);

		score = 0;
		if (inet->inet_dport) {
			if (inet->inet_dport != sport)
				return -1;
			score++;
		}
		if (!ipv6_addr_any(&np->rcv_saddr)) {
			if (!ipv6_addr_equal(&np->rcv_saddr, daddr))
				return -1;
			score++;
		}
		if (!ipv6_addr_any(&np->daddr)) {
			if (!ipv6_addr_equal(&np->daddr, saddr))
				return -1;
			score++;
		}
		if (sk->sk_bound_dev_if) {
			if (sk->sk_bound_dev_if != dif)
				return -1;
			score++;
		}
	}
	return score;
}

#define SCORE2_MAX (1 + 1 + 1)
static inline int compute_score2(struct sock *sk, struct net *net,
				const struct in6_addr *saddr, __be16 sport,
				const struct in6_addr *daddr, unsigned short hnum,
				int dif)
{
	int score = -1;

	if (net_eq(sock_net(sk), net) && udp_sk(sk)->udp_port_hash == hnum &&
			sk->sk_family == PF_INET6) {
		struct ipv6_pinfo *np = inet6_sk(sk);
		struct inet_sock *inet = inet_sk(sk);

		if (!ipv6_addr_equal(&np->rcv_saddr, daddr))
			return -1;
		score = 0;
		if (inet->inet_dport) {
			if (inet->inet_dport != sport)
				return -1;
			score++;
		}
		if (!ipv6_addr_any(&np->daddr)) {
			if (!ipv6_addr_equal(&np->daddr, saddr))
				return -1;
			score++;
		}
		if (sk->sk_bound_dev_if) {
			if (sk->sk_bound_dev_if != dif)
				return -1;
			score++;
		}
	}
	return score;
}


/* called with read_rcu_lock() */
static struct sock *udp6_lib_lookup2(struct net *net,
		const struct in6_addr *saddr, __be16 sport,
		const struct in6_addr *daddr, unsigned int hnum, int dif,
		struct udp_hslot *hslot2, unsigned int slot2)
{
	struct sock *sk, *result;
	struct hlist_nulls_node *node;
	int score, badness;

begin:
	result = NULL;
	badness = -1;
	udp_portaddr_for_each_entry_rcu(sk, node, &hslot2->head) {
		score = compute_score2(sk, net, saddr, sport,
				      daddr, hnum, dif);
		if (score > badness) {
			result = sk;
			badness = score;
			if (score == SCORE2_MAX)
				goto exact_match;
		}
	}
	/*
	 * if the nulls value we got at the end of this lookup is
	 * not the expected one, we must restart lookup.
	 * We probably met an item that was moved to another chain.
	 */
	if (get_nulls_value(node) != slot2)
		goto begin;

	if (result) {
exact_match:
		if (unlikely(!atomic_inc_not_zero_hint(&result->sk_refcnt, 2)))
			result = NULL;
		else if (unlikely(compute_score2(result, net, saddr, sport,
				  daddr, hnum, dif) < badness)) {
			sock_put(result);
			goto begin;
=======
static int compute_score(struct sock *sk, struct net *net,
			 const struct in6_addr *saddr, __be16 sport,
			 const struct in6_addr *daddr, unsigned short hnum,
			 int dif, int sdif)
{
	int bound_dev_if, score;
	struct inet_sock *inet;
	bool dev_match;

	if (!net_eq(sock_net(sk), net) ||
	    udp_sk(sk)->udp_port_hash != hnum ||
	    sk->sk_family != PF_INET6)
		return -1;

	if (!ipv6_addr_equal(&sk->sk_v6_rcv_saddr, daddr))
		return -1;

	score = 0;
	inet = inet_sk(sk);

	if (inet->inet_dport) {
		if (inet->inet_dport != sport)
			return -1;
		score++;
	}

	if (!ipv6_addr_any(&sk->sk_v6_daddr)) {
		if (!ipv6_addr_equal(&sk->sk_v6_daddr, saddr))
			return -1;
		score++;
	}

	bound_dev_if = READ_ONCE(sk->sk_bound_dev_if);
	dev_match = udp_sk_bound_dev_eq(net, bound_dev_if, dif, sdif);
	if (!dev_match)
		return -1;
	if (bound_dev_if)
		score++;

	if (READ_ONCE(sk->sk_incoming_cpu) == raw_smp_processor_id())
		score++;

	return score;
}

/* called with rcu_read_lock() */
static struct sock *udp6_lib_lookup2(struct net *net,
		const struct in6_addr *saddr, __be16 sport,
		const struct in6_addr *daddr, unsigned int hnum,
		int dif, int sdif, struct udp_hslot *hslot2,
		struct sk_buff *skb)
{
	struct sock *sk, *result;
	int score, badness;

	result = NULL;
	badness = -1;
	udp_portaddr_for_each_entry_rcu(sk, &hslot2->head) {
		score = compute_score(sk, net, saddr, sport,
				      daddr, hnum, dif, sdif);
		if (score > badness) {
			badness = score;

			if (sk->sk_state == TCP_ESTABLISHED) {
				result = sk;
				continue;
			}

			result = inet6_lookup_reuseport(net, sk, skb, sizeof(struct udphdr),
							saddr, sport, daddr, hnum, udp6_ehashfn);
			if (!result) {
				result = sk;
				continue;
			}

			/* Fall back to scoring if group has connections */
			if (!reuseport_has_conns(sk))
				return result;

			/* Reuseport logic returned an error, keep original score. */
			if (IS_ERR(result))
				continue;

			badness = compute_score(sk, net, saddr, sport,
						daddr, hnum, dif, sdif);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}
	return result;
}

<<<<<<< HEAD
struct sock *__udp6_lib_lookup(struct net *net,
				      const struct in6_addr *saddr, __be16 sport,
				      const struct in6_addr *daddr, __be16 dport,
				      int dif, struct udp_table *udptable)
{
	struct sock *sk, *result;
	struct hlist_nulls_node *node;
	unsigned short hnum = ntohs(dport);
	unsigned int hash2, slot2, slot = udp_hashfn(net, hnum, udptable->mask);
	struct udp_hslot *hslot2, *hslot = &udptable->hash[slot];
	int score, badness;

	rcu_read_lock();
	if (hslot->count > 10) {
		hash2 = udp6_portaddr_hash(net, daddr, hnum);
		slot2 = hash2 & udptable->mask;
		hslot2 = &udptable->hash2[slot2];
		if (hslot->count < hslot2->count)
			goto begin;

		result = udp6_lib_lookup2(net, saddr, sport,
					  daddr, hnum, dif,
					  hslot2, slot2);
		if (!result) {
			hash2 = udp6_portaddr_hash(net, &in6addr_any, hnum);
			slot2 = hash2 & udptable->mask;
			hslot2 = &udptable->hash2[slot2];
			if (hslot->count < hslot2->count)
				goto begin;

			result = udp6_lib_lookup2(net, saddr, sport,
						  &in6addr_any, hnum, dif,
						  hslot2, slot2);
		}
		rcu_read_unlock();
		return result;
	}
begin:
	result = NULL;
	badness = -1;
	sk_nulls_for_each_rcu(sk, node, &hslot->head) {
		score = compute_score(sk, net, hnum, saddr, sport, daddr, dport, dif);
		if (score > badness) {
			result = sk;
			badness = score;
		}
	}
	/*
	 * if the nulls value we got at the end of this lookup is
	 * not the expected one, we must restart lookup.
	 * We probably met an item that was moved to another chain.
	 */
	if (get_nulls_value(node) != slot)
		goto begin;

	if (result) {
		if (unlikely(!atomic_inc_not_zero_hint(&result->sk_refcnt, 2)))
			result = NULL;
		else if (unlikely(compute_score(result, net, hnum, saddr, sport,
					daddr, dport, dif) < badness)) {
			sock_put(result);
			goto begin;
		}
	}
	rcu_read_unlock();
=======
/* rcu_read_lock() must be held */
struct sock *__udp6_lib_lookup(struct net *net,
			       const struct in6_addr *saddr, __be16 sport,
			       const struct in6_addr *daddr, __be16 dport,
			       int dif, int sdif, struct udp_table *udptable,
			       struct sk_buff *skb)
{
	unsigned short hnum = ntohs(dport);
	unsigned int hash2, slot2;
	struct udp_hslot *hslot2;
	struct sock *result, *sk;

	hash2 = ipv6_portaddr_hash(net, daddr, hnum);
	slot2 = hash2 & udptable->mask;
	hslot2 = &udptable->hash2[slot2];

	/* Lookup connected or non-wildcard sockets */
	result = udp6_lib_lookup2(net, saddr, sport,
				  daddr, hnum, dif, sdif,
				  hslot2, skb);
	if (!IS_ERR_OR_NULL(result) && result->sk_state == TCP_ESTABLISHED)
		goto done;

	/* Lookup redirect from BPF */
	if (static_branch_unlikely(&bpf_sk_lookup_enabled) &&
	    udptable == net->ipv4.udp_table) {
		sk = inet6_lookup_run_sk_lookup(net, IPPROTO_UDP, skb, sizeof(struct udphdr),
						saddr, sport, daddr, hnum, dif,
						udp6_ehashfn);
		if (sk) {
			result = sk;
			goto done;
		}
	}

	/* Got non-wildcard socket or error on first lookup */
	if (result)
		goto done;

	/* Lookup wildcard sockets */
	hash2 = ipv6_portaddr_hash(net, &in6addr_any, hnum);
	slot2 = hash2 & udptable->mask;
	hslot2 = &udptable->hash2[slot2];

	result = udp6_lib_lookup2(net, saddr, sport,
				  &in6addr_any, hnum, dif, sdif,
				  hslot2, skb);
done:
	if (IS_ERR(result))
		return NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return result;
}
EXPORT_SYMBOL_GPL(__udp6_lib_lookup);

static struct sock *__udp6_lib_lookup_skb(struct sk_buff *skb,
					  __be16 sport, __be16 dport,
					  struct udp_table *udptable)
{
<<<<<<< HEAD
	struct sock *sk;
	const struct ipv6hdr *iph = ipv6_hdr(skb);

	if (unlikely(sk = skb_steal_sock(skb)))
		return sk;
	return __udp6_lib_lookup(dev_net(skb_dst(skb)->dev), &iph->saddr, sport,
				 &iph->daddr, dport, inet6_iif(skb),
				 udptable);
}

struct sock *udp6_lib_lookup(struct net *net, const struct in6_addr *saddr, __be16 sport,
			     const struct in6_addr *daddr, __be16 dport, int dif)
{
	return __udp6_lib_lookup(net, saddr, sport, daddr, dport, dif, &udp_table);
}
EXPORT_SYMBOL_GPL(udp6_lib_lookup);


/*
 * 	This should be easy, if there is something there we
 * 	return it, otherwise we block.
 */

int udpv6_recvmsg(struct kiocb *iocb, struct sock *sk,
		  struct msghdr *msg, size_t len,
		  int noblock, int flags, int *addr_len)
=======
	const struct ipv6hdr *iph = ipv6_hdr(skb);

	return __udp6_lib_lookup(dev_net(skb->dev), &iph->saddr, sport,
				 &iph->daddr, dport, inet6_iif(skb),
				 inet6_sdif(skb), udptable, skb);
}

struct sock *udp6_lib_lookup_skb(const struct sk_buff *skb,
				 __be16 sport, __be16 dport)
{
	const u16 offset = NAPI_GRO_CB(skb)->network_offsets[skb->encapsulation];
	const struct ipv6hdr *iph = (struct ipv6hdr *)(skb->data + offset);
	struct net *net = dev_net(skb->dev);
	int iif, sdif;

	inet6_get_iif_sdif(skb, &iif, &sdif);

	return __udp6_lib_lookup(net, &iph->saddr, sport,
				 &iph->daddr, dport, iif,
				 sdif, net->ipv4.udp_table, NULL);
}

/* Must be called under rcu_read_lock().
 * Does increment socket refcount.
 */
#if IS_ENABLED(CONFIG_NF_TPROXY_IPV6) || IS_ENABLED(CONFIG_NF_SOCKET_IPV6)
struct sock *udp6_lib_lookup(struct net *net, const struct in6_addr *saddr, __be16 sport,
			     const struct in6_addr *daddr, __be16 dport, int dif)
{
	struct sock *sk;

	sk =  __udp6_lib_lookup(net, saddr, sport, daddr, dport,
				dif, 0, net->ipv4.udp_table, NULL);
	if (sk && !refcount_inc_not_zero(&sk->sk_refcnt))
		sk = NULL;
	return sk;
}
EXPORT_SYMBOL_GPL(udp6_lib_lookup);
#endif

/* do not use the scratch area len for jumbogram: their length execeeds the
 * scratch area space; note that the IP6CB flags is still in the first
 * cacheline, so checking for jumbograms is cheap
 */
static int udp6_skb_len(struct sk_buff *skb)
{
	return unlikely(inet6_is_jumbogram(skb)) ? skb->len : udp_skb_len(skb);
}

/*
 *	This should be easy, if there is something there we
 *	return it, otherwise we block.
 */

int udpv6_recvmsg(struct sock *sk, struct msghdr *msg, size_t len,
		  int flags, int *addr_len)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ipv6_pinfo *np = inet6_sk(sk);
	struct inet_sock *inet = inet_sk(sk);
	struct sk_buff *skb;
	unsigned int ulen, copied;
<<<<<<< HEAD
	int peeked, off = 0;
	int err;
	int is_udplite = IS_UDPLITE(sk);
	bool checksum_valid = false;
	int is_udp4;
	bool slow;
=======
	int off, err, peeking = flags & MSG_PEEK;
	int is_udplite = IS_UDPLITE(sk);
	struct udp_mib __percpu *mib;
	bool checksum_valid = false;
	int is_udp4;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (flags & MSG_ERRQUEUE)
		return ipv6_recv_error(sk, msg, len, addr_len);

	if (np->rxpmtu && np->rxopt.bits.rxpmtu)
		return ipv6_recv_rxpmtu(sk, msg, len, addr_len);

try_again:
<<<<<<< HEAD
	skb = __skb_recv_datagram(sk, flags | (noblock ? MSG_DONTWAIT : 0),
				  &peeked, &off, &err);
	if (!skb)
		goto out;

	ulen = skb->len - sizeof(struct udphdr);
	copied = len;
	if (copied > ulen)
		copied = ulen;
=======
	off = sk_peek_offset(sk, flags);
	skb = __skb_recv_udp(sk, flags, &off, &err);
	if (!skb)
		return err;

	ulen = udp6_skb_len(skb);
	copied = len;
	if (copied > ulen - off)
		copied = ulen - off;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	else if (copied < ulen)
		msg->msg_flags |= MSG_TRUNC;

	is_udp4 = (skb->protocol == htons(ETH_P_IP));
<<<<<<< HEAD
=======
	mib = __UDPX_MIB(sk, is_udp4);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * If checksum is needed at all, try to do it while copying the
	 * data.  If the data is truncated, or if we only want a partial
	 * coverage checksum (UDP-Lite), do it before the copy.
	 */

<<<<<<< HEAD
	if (copied < ulen || UDP_SKB_CB(skb)->partial_cov) {
		checksum_valid = !udp_lib_checksum_complete(skb);
=======
	if (copied < ulen || peeking ||
	    (is_udplite && UDP_SKB_CB(skb)->partial_cov)) {
		checksum_valid = udp_skb_csum_unnecessary(skb) ||
				!__udp_lib_checksum_complete(skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!checksum_valid)
			goto csum_copy_err;
	}

<<<<<<< HEAD
	if (checksum_valid || skb_csum_unnecessary(skb))
		err = skb_copy_datagram_iovec(skb, sizeof(struct udphdr),
					      msg->msg_iov, copied       );
	else {
		err = skb_copy_and_csum_datagram_iovec(skb, sizeof(struct udphdr),
						       msg->msg_iov, copied);
		if (err == -EINVAL)
			goto csum_copy_err;
	}
	if (err)
		goto out_free;

	if (!peeked) {
		if (is_udp4)
			UDP_INC_STATS_USER(sock_net(sk),
					UDP_MIB_INDATAGRAMS, is_udplite);
		else
			UDP6_INC_STATS_USER(sock_net(sk),
					UDP_MIB_INDATAGRAMS, is_udplite);
	}

	sock_recv_ts_and_drops(msg, sk, skb);

	/* Copy the address. */
	if (msg->msg_name) {
		struct sockaddr_in6 *sin6;

		sin6 = (struct sockaddr_in6 *) msg->msg_name;
		sin6->sin6_family = AF_INET6;
		sin6->sin6_port = udp_hdr(skb)->source;
		sin6->sin6_flowinfo = 0;
		sin6->sin6_scope_id = 0;

		if (is_udp4)
			ipv6_addr_set_v4mapped(ip_hdr(skb)->saddr,
					       &sin6->sin6_addr);
		else {
			sin6->sin6_addr = ipv6_hdr(skb)->saddr;
			if (ipv6_addr_type(&sin6->sin6_addr) & IPV6_ADDR_LINKLOCAL)
				sin6->sin6_scope_id = IP6CB(skb)->iif;
		}
		*addr_len = sizeof(*sin6);
	}
	if (is_udp4) {
		if (inet->cmsg_flags)
			ip_cmsg_recv(msg, skb);
	} else {
		if (np->rxopt.all)
			datagram_recv_ctl(sk, msg, skb);
=======
	if (checksum_valid || udp_skb_csum_unnecessary(skb)) {
		if (udp_skb_is_linear(skb))
			err = copy_linear_skb(skb, copied, off, &msg->msg_iter);
		else
			err = skb_copy_datagram_msg(skb, off, msg, copied);
	} else {
		err = skb_copy_and_csum_datagram_msg(skb, off, msg);
		if (err == -EINVAL)
			goto csum_copy_err;
	}
	if (unlikely(err)) {
		if (!peeking) {
			atomic_inc(&sk->sk_drops);
			SNMP_INC_STATS(mib, UDP_MIB_INERRORS);
		}
		kfree_skb(skb);
		return err;
	}
	if (!peeking)
		SNMP_INC_STATS(mib, UDP_MIB_INDATAGRAMS);

	sock_recv_cmsgs(msg, sk, skb);

	/* Copy the address. */
	if (msg->msg_name) {
		DECLARE_SOCKADDR(struct sockaddr_in6 *, sin6, msg->msg_name);
		sin6->sin6_family = AF_INET6;
		sin6->sin6_port = udp_hdr(skb)->source;
		sin6->sin6_flowinfo = 0;

		if (is_udp4) {
			ipv6_addr_set_v4mapped(ip_hdr(skb)->saddr,
					       &sin6->sin6_addr);
			sin6->sin6_scope_id = 0;
		} else {
			sin6->sin6_addr = ipv6_hdr(skb)->saddr;
			sin6->sin6_scope_id =
				ipv6_iface_scope_id(&sin6->sin6_addr,
						    inet6_iif(skb));
		}
		*addr_len = sizeof(*sin6);

		BPF_CGROUP_RUN_PROG_UDP6_RECVMSG_LOCK(sk,
						      (struct sockaddr *)sin6,
						      addr_len);
	}

	if (udp_test_bit(GRO_ENABLED, sk))
		udp_cmsg_recv(msg, sk, skb);

	if (np->rxopt.all)
		ip6_datagram_recv_common_ctl(sk, msg, skb);

	if (is_udp4) {
		if (inet_cmsg_flags(inet))
			ip_cmsg_recv_offset(msg, sk, skb,
					    sizeof(struct udphdr), off);
	} else {
		if (np->rxopt.all)
			ip6_datagram_recv_specific_ctl(sk, msg, skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	err = copied;
	if (flags & MSG_TRUNC)
		err = ulen;

<<<<<<< HEAD
out_free:
	skb_free_datagram_locked(sk, skb);
out:
	return err;

csum_copy_err:
	slow = lock_sock_fast(sk);
	if (!skb_kill_datagram(sk, skb, flags)) {
		if (is_udp4)
			UDP_INC_STATS_USER(sock_net(sk),
					UDP_MIB_INERRORS, is_udplite);
		else
			UDP6_INC_STATS_USER(sock_net(sk),
					UDP_MIB_INERRORS, is_udplite);
	}
	unlock_sock_fast(sk, slow);
=======
	skb_consume_udp(sk, skb, peeking ? -err : err);
	return err;

csum_copy_err:
	if (!__sk_queue_drop_skb(sk, &udp_sk(sk)->reader_queue, skb, flags,
				 udp_skb_destructor)) {
		SNMP_INC_STATS(mib, UDP_MIB_CSUMERRORS);
		SNMP_INC_STATS(mib, UDP_MIB_INERRORS);
	}
	kfree_skb(skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* starting over for a new packet, but check if we need to yield */
	cond_resched();
	msg->msg_flags &= ~MSG_TRUNC;
	goto try_again;
}

<<<<<<< HEAD
void __udp6_lib_err(struct sk_buff *skb, struct inet6_skb_parm *opt,
		    u8 type, u8 code, int offset, __be32 info,
		    struct udp_table *udptable)
=======
DECLARE_STATIC_KEY_FALSE(udpv6_encap_needed_key);
void udpv6_encap_enable(void)
{
	static_branch_inc(&udpv6_encap_needed_key);
}
EXPORT_SYMBOL(udpv6_encap_enable);

/* Handler for tunnels with arbitrary destination ports: no socket lookup, go
 * through error handlers in encapsulations looking for a match.
 */
static int __udp6_lib_err_encap_no_sk(struct sk_buff *skb,
				      struct inet6_skb_parm *opt,
				      u8 type, u8 code, int offset, __be32 info)
{
	int i;

	for (i = 0; i < MAX_IPTUN_ENCAP_OPS; i++) {
		int (*handler)(struct sk_buff *skb, struct inet6_skb_parm *opt,
			       u8 type, u8 code, int offset, __be32 info);
		const struct ip6_tnl_encap_ops *encap;

		encap = rcu_dereference(ip6tun_encaps[i]);
		if (!encap)
			continue;
		handler = encap->err_handler;
		if (handler && !handler(skb, opt, type, code, offset, info))
			return 0;
	}

	return -ENOENT;
}

/* Try to match ICMP errors to UDP tunnels by looking up a socket without
 * reversing source and destination port: this will match tunnels that force the
 * same destination port on both endpoints (e.g. VXLAN, GENEVE). Note that
 * lwtunnels might actually break this assumption by being configured with
 * different destination ports on endpoints, in this case we won't be able to
 * trace ICMP messages back to them.
 *
 * If this doesn't match any socket, probe tunnels with arbitrary destination
 * ports (e.g. FoU, GUE): there, the receiving socket is useless, as the port
 * we've sent packets to won't necessarily match the local destination port.
 *
 * Then ask the tunnel implementation to match the error against a valid
 * association.
 *
 * Return an error if we can't find a match, the socket if we need further
 * processing, zero otherwise.
 */
static struct sock *__udp6_lib_err_encap(struct net *net,
					 const struct ipv6hdr *hdr, int offset,
					 struct udphdr *uh,
					 struct udp_table *udptable,
					 struct sock *sk,
					 struct sk_buff *skb,
					 struct inet6_skb_parm *opt,
					 u8 type, u8 code, __be32 info)
{
	int (*lookup)(struct sock *sk, struct sk_buff *skb);
	int network_offset, transport_offset;
	struct udp_sock *up;

	network_offset = skb_network_offset(skb);
	transport_offset = skb_transport_offset(skb);

	/* Network header needs to point to the outer IPv6 header inside ICMP */
	skb_reset_network_header(skb);

	/* Transport header needs to point to the UDP header */
	skb_set_transport_header(skb, offset);

	if (sk) {
		up = udp_sk(sk);

		lookup = READ_ONCE(up->encap_err_lookup);
		if (lookup && lookup(sk, skb))
			sk = NULL;

		goto out;
	}

	sk = __udp6_lib_lookup(net, &hdr->daddr, uh->source,
			       &hdr->saddr, uh->dest,
			       inet6_iif(skb), 0, udptable, skb);
	if (sk) {
		up = udp_sk(sk);

		lookup = READ_ONCE(up->encap_err_lookup);
		if (!lookup || lookup(sk, skb))
			sk = NULL;
	}

out:
	if (!sk) {
		sk = ERR_PTR(__udp6_lib_err_encap_no_sk(skb, opt, type, code,
							offset, info));
	}

	skb_set_transport_header(skb, transport_offset);
	skb_set_network_header(skb, network_offset);

	return sk;
}

int __udp6_lib_err(struct sk_buff *skb, struct inet6_skb_parm *opt,
		   u8 type, u8 code, int offset, __be32 info,
		   struct udp_table *udptable)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ipv6_pinfo *np;
	const struct ipv6hdr *hdr = (const struct ipv6hdr *)skb->data;
	const struct in6_addr *saddr = &hdr->saddr;
<<<<<<< HEAD
	const struct in6_addr *daddr = &hdr->daddr;
	struct udphdr *uh = (struct udphdr*)(skb->data+offset);
	struct sock *sk;
	int err;

	sk = __udp6_lib_lookup(dev_net(skb->dev), daddr, uh->dest,
			       saddr, uh->source, inet6_iif(skb), udptable);
	if (sk == NULL)
		return;

	if (type == ICMPV6_PKT_TOOBIG)
		ip6_sk_update_pmtu(skb, sk, info);

	np = inet6_sk(sk);

	if (!icmpv6_err_convert(type, code, &err) && !np->recverr)
		goto out;

	if (sk->sk_state != TCP_ESTABLISHED && !np->recverr)
		goto out;

	if (np->recverr)
		ipv6_icmp_error(sk, skb, err, uh->dest, ntohl(info), (u8 *)(uh+1));

	sk->sk_err = err;
	sk->sk_error_report(sk);
out:
	sock_put(sk);
}

static __inline__ void udpv6_err(struct sk_buff *skb,
				 struct inet6_skb_parm *opt, u8 type,
				 u8 code, int offset, __be32 info     )
{
	__udp6_lib_err(skb, opt, type, code, offset, info, &udp_table);
}

int udpv6_queue_rcv_skb(struct sock * sk, struct sk_buff *skb)
{
	struct udp_sock *up = udp_sk(sk);
	int rc;
	int is_udplite = IS_UDPLITE(sk);

	if (!ipv6_addr_any(&inet6_sk(sk)->daddr))
		sock_rps_save_rxhash(sk, skb);

	if (!xfrm6_policy_check(sk, XFRM_POLICY_IN, skb))
		goto drop;
=======
	const struct in6_addr *daddr = seg6_get_daddr(skb, opt) ? : &hdr->daddr;
	struct udphdr *uh = (struct udphdr *)(skb->data+offset);
	bool tunnel = false;
	struct sock *sk;
	int harderr;
	int err;
	struct net *net = dev_net(skb->dev);

	sk = __udp6_lib_lookup(net, daddr, uh->dest, saddr, uh->source,
			       inet6_iif(skb), inet6_sdif(skb), udptable, NULL);

	if (!sk || READ_ONCE(udp_sk(sk)->encap_type)) {
		/* No socket for error: try tunnels before discarding */
		if (static_branch_unlikely(&udpv6_encap_needed_key)) {
			sk = __udp6_lib_err_encap(net, hdr, offset, uh,
						  udptable, sk, skb,
						  opt, type, code, info);
			if (!sk)
				return 0;
		} else
			sk = ERR_PTR(-ENOENT);

		if (IS_ERR(sk)) {
			__ICMP6_INC_STATS(net, __in6_dev_get(skb->dev),
					  ICMP6_MIB_INERRORS);
			return PTR_ERR(sk);
		}

		tunnel = true;
	}

	harderr = icmpv6_err_convert(type, code, &err);
	np = inet6_sk(sk);

	if (type == ICMPV6_PKT_TOOBIG) {
		if (!ip6_sk_accept_pmtu(sk))
			goto out;
		ip6_sk_update_pmtu(skb, sk, info);
		if (READ_ONCE(np->pmtudisc) != IPV6_PMTUDISC_DONT)
			harderr = 1;
	}
	if (type == NDISC_REDIRECT) {
		if (tunnel) {
			ip6_redirect(skb, sock_net(sk), inet6_iif(skb),
				     READ_ONCE(sk->sk_mark), sk->sk_uid);
		} else {
			ip6_sk_redirect(skb, sk);
		}
		goto out;
	}

	/* Tunnels don't have an application socket: don't pass errors back */
	if (tunnel) {
		if (udp_sk(sk)->encap_err_rcv)
			udp_sk(sk)->encap_err_rcv(sk, skb, err, uh->dest,
						  ntohl(info), (u8 *)(uh+1));
		goto out;
	}

	if (!inet6_test_bit(RECVERR6, sk)) {
		if (!harderr || sk->sk_state != TCP_ESTABLISHED)
			goto out;
	} else {
		ipv6_icmp_error(sk, skb, err, uh->dest, ntohl(info), (u8 *)(uh+1));
	}

	sk->sk_err = err;
	sk_error_report(sk);
out:
	return 0;
}

static int __udpv6_queue_rcv_skb(struct sock *sk, struct sk_buff *skb)
{
	int rc;

	if (!ipv6_addr_any(&sk->sk_v6_daddr)) {
		sock_rps_save_rxhash(sk, skb);
		sk_mark_napi_id(sk, skb);
		sk_incoming_cpu_update(sk);
	} else {
		sk_mark_napi_id_once(sk, skb);
	}

	rc = __udp_enqueue_schedule_skb(sk, skb);
	if (rc < 0) {
		int is_udplite = IS_UDPLITE(sk);
		enum skb_drop_reason drop_reason;

		/* Note that an ENOMEM error is charged twice */
		if (rc == -ENOMEM) {
			UDP6_INC_STATS(sock_net(sk),
					 UDP_MIB_RCVBUFERRORS, is_udplite);
			drop_reason = SKB_DROP_REASON_SOCKET_RCVBUFF;
		} else {
			UDP6_INC_STATS(sock_net(sk),
				       UDP_MIB_MEMERRORS, is_udplite);
			drop_reason = SKB_DROP_REASON_PROTO_MEM;
		}
		UDP6_INC_STATS(sock_net(sk), UDP_MIB_INERRORS, is_udplite);
		kfree_skb_reason(skb, drop_reason);
		trace_udp_fail_queue_rcv_skb(rc, sk);
		return -1;
	}

	return 0;
}

static __inline__ int udpv6_err(struct sk_buff *skb,
				struct inet6_skb_parm *opt, u8 type,
				u8 code, int offset, __be32 info)
{
	return __udp6_lib_err(skb, opt, type, code, offset, info,
			      dev_net(skb->dev)->ipv4.udp_table);
}

static int udpv6_queue_rcv_one_skb(struct sock *sk, struct sk_buff *skb)
{
	enum skb_drop_reason drop_reason = SKB_DROP_REASON_NOT_SPECIFIED;
	struct udp_sock *up = udp_sk(sk);
	int is_udplite = IS_UDPLITE(sk);

	if (!xfrm6_policy_check(sk, XFRM_POLICY_IN, skb)) {
		drop_reason = SKB_DROP_REASON_XFRM_POLICY;
		goto drop;
	}
	nf_reset_ct(skb);

	if (static_branch_unlikely(&udpv6_encap_needed_key) &&
	    READ_ONCE(up->encap_type)) {
		int (*encap_rcv)(struct sock *sk, struct sk_buff *skb);

		/*
		 * This is an encapsulation socket so pass the skb to
		 * the socket's udp_encap_rcv() hook. Otherwise, just
		 * fall through and pass this up the UDP socket.
		 * up->encap_rcv() returns the following value:
		 * =0 if skb was successfully passed to the encap
		 *    handler or was discarded by it.
		 * >0 if skb should be passed on to UDP.
		 * <0 if skb should be resubmitted as proto -N
		 */

		/* if we're overly short, let UDP handle it */
		encap_rcv = READ_ONCE(up->encap_rcv);
		if (encap_rcv) {
			int ret;

			/* Verify checksum before giving to encap */
			if (udp_lib_checksum_complete(skb))
				goto csum_error;

			ret = encap_rcv(sk, skb);
			if (ret <= 0) {
				__UDP6_INC_STATS(sock_net(sk),
						 UDP_MIB_INDATAGRAMS,
						 is_udplite);
				return -ret;
			}
		}

		/* FALLTHROUGH -- it's a UDP Packet */
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * UDP-Lite specific tests, ignored on UDP sockets (see net/ipv4/udp.c).
	 */
<<<<<<< HEAD
	if ((is_udplite & UDPLITE_RECV_CC)  &&  UDP_SKB_CB(skb)->partial_cov) {

		if (up->pcrlen == 0) {          /* full coverage was set  */
			LIMIT_NETDEBUG(KERN_WARNING "UDPLITE6: partial coverage"
				" %d while full coverage %d requested\n",
				UDP_SKB_CB(skb)->cscov, skb->len);
			goto drop;
		}
		if (UDP_SKB_CB(skb)->cscov  <  up->pcrlen) {
			LIMIT_NETDEBUG(KERN_WARNING "UDPLITE6: coverage %d "
						    "too small, need min %d\n",
				       UDP_SKB_CB(skb)->cscov, up->pcrlen);
=======
	if (udp_test_bit(UDPLITE_RECV_CC, sk) && UDP_SKB_CB(skb)->partial_cov) {
		u16 pcrlen = READ_ONCE(up->pcrlen);

		if (pcrlen == 0) {          /* full coverage was set  */
			net_dbg_ratelimited("UDPLITE6: partial coverage %d while full coverage %d requested\n",
					    UDP_SKB_CB(skb)->cscov, skb->len);
			goto drop;
		}
		if (UDP_SKB_CB(skb)->cscov < pcrlen) {
			net_dbg_ratelimited("UDPLITE6: coverage %d too small, need min %d\n",
					    UDP_SKB_CB(skb)->cscov, pcrlen);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto drop;
		}
	}

<<<<<<< HEAD
	if (rcu_access_pointer(sk->sk_filter)) {
		if (udp_lib_checksum_complete(skb))
			goto drop;
	}

	skb_dst_drop(skb);
	rc = sock_queue_rcv_skb(sk, skb);
	if (rc < 0) {
		/* Note that an ENOMEM error is charged twice */
		if (rc == -ENOMEM)
			UDP6_INC_STATS_BH(sock_net(sk),
					UDP_MIB_RCVBUFERRORS, is_udplite);
		goto drop_no_sk_drops_inc;
	}

	return 0;
drop:
	atomic_inc(&sk->sk_drops);
drop_no_sk_drops_inc:
	UDP6_INC_STATS_BH(sock_net(sk), UDP_MIB_INERRORS, is_udplite);
	kfree_skb(skb);
	return -1;
}

static struct sock *udp_v6_mcast_next(struct net *net, struct sock *sk,
				      __be16 loc_port, const struct in6_addr *loc_addr,
				      __be16 rmt_port, const struct in6_addr *rmt_addr,
				      int dif)
{
	struct hlist_nulls_node *node;
	struct sock *s = sk;
	unsigned short num = ntohs(loc_port);

	sk_nulls_for_each_from(s, node) {
		struct inet_sock *inet = inet_sk(s);

		if (!net_eq(sock_net(s), net))
			continue;

		if (udp_sk(s)->udp_port_hash == num &&
		    s->sk_family == PF_INET6) {
			struct ipv6_pinfo *np = inet6_sk(s);
			if (inet->inet_dport) {
				if (inet->inet_dport != rmt_port)
					continue;
			}
			if (!ipv6_addr_any(&np->daddr) &&
			    !ipv6_addr_equal(&np->daddr, rmt_addr))
				continue;

			if (s->sk_bound_dev_if && s->sk_bound_dev_if != dif)
				continue;

			if (!ipv6_addr_any(&np->rcv_saddr)) {
				if (!ipv6_addr_equal(&np->rcv_saddr, loc_addr))
					continue;
			}
			if (!inet6_mc_check(s, loc_addr, rmt_addr))
				continue;
			return s;
		}
	}
	return NULL;
}

static void flush_stack(struct sock **stack, unsigned int count,
			struct sk_buff *skb, unsigned int final)
{
	unsigned int i;
	struct sock *sk;
	struct sk_buff *skb1;

	for (i = 0; i < count; i++) {
		skb1 = (i == final) ? skb : skb_clone(skb, GFP_ATOMIC);

		sk = stack[i];
		if (skb1) {
			if (sk_rcvqueues_full(sk, skb1)) {
				kfree_skb(skb1);
				goto drop;
			}
			bh_lock_sock(sk);
			if (!sock_owned_by_user(sk))
				udpv6_queue_rcv_skb(sk, skb1);
			else if (sk_add_backlog(sk, skb1)) {
				kfree_skb(skb1);
				bh_unlock_sock(sk);
				goto drop;
			}
			bh_unlock_sock(sk);
			continue;
		}
drop:
		atomic_inc(&sk->sk_drops);
		UDP6_INC_STATS_BH(sock_net(sk),
				UDP_MIB_RCVBUFERRORS, IS_UDPLITE(sk));
		UDP6_INC_STATS_BH(sock_net(sk),
				UDP_MIB_INERRORS, IS_UDPLITE(sk));
	}
}
=======
	prefetch(&sk->sk_rmem_alloc);
	if (rcu_access_pointer(sk->sk_filter) &&
	    udp_lib_checksum_complete(skb))
		goto csum_error;

	if (sk_filter_trim_cap(sk, skb, sizeof(struct udphdr))) {
		drop_reason = SKB_DROP_REASON_SOCKET_FILTER;
		goto drop;
	}

	udp_csum_pull_header(skb);

	skb_dst_drop(skb);

	return __udpv6_queue_rcv_skb(sk, skb);

csum_error:
	drop_reason = SKB_DROP_REASON_UDP_CSUM;
	__UDP6_INC_STATS(sock_net(sk), UDP_MIB_CSUMERRORS, is_udplite);
drop:
	__UDP6_INC_STATS(sock_net(sk), UDP_MIB_INERRORS, is_udplite);
	atomic_inc(&sk->sk_drops);
	kfree_skb_reason(skb, drop_reason);
	return -1;
}

static int udpv6_queue_rcv_skb(struct sock *sk, struct sk_buff *skb)
{
	struct sk_buff *next, *segs;
	int ret;

	if (likely(!udp_unexpected_gso(sk, skb)))
		return udpv6_queue_rcv_one_skb(sk, skb);

	__skb_push(skb, -skb_mac_offset(skb));
	segs = udp_rcv_segment(sk, skb, false);
	skb_list_walk_safe(segs, skb, next) {
		__skb_pull(skb, skb_transport_offset(skb));

		udp_post_segment_fix_csum(skb);
		ret = udpv6_queue_rcv_one_skb(sk, skb);
		if (ret > 0)
			ip6_protocol_deliver_rcu(dev_net(skb->dev), skb, ret,
						 true);
	}
	return 0;
}

static bool __udp_v6_is_mcast_sock(struct net *net, const struct sock *sk,
				   __be16 loc_port, const struct in6_addr *loc_addr,
				   __be16 rmt_port, const struct in6_addr *rmt_addr,
				   int dif, int sdif, unsigned short hnum)
{
	const struct inet_sock *inet = inet_sk(sk);

	if (!net_eq(sock_net(sk), net))
		return false;

	if (udp_sk(sk)->udp_port_hash != hnum ||
	    sk->sk_family != PF_INET6 ||
	    (inet->inet_dport && inet->inet_dport != rmt_port) ||
	    (!ipv6_addr_any(&sk->sk_v6_daddr) &&
		    !ipv6_addr_equal(&sk->sk_v6_daddr, rmt_addr)) ||
	    !udp_sk_bound_dev_eq(net, READ_ONCE(sk->sk_bound_dev_if), dif, sdif) ||
	    (!ipv6_addr_any(&sk->sk_v6_rcv_saddr) &&
		    !ipv6_addr_equal(&sk->sk_v6_rcv_saddr, loc_addr)))
		return false;
	if (!inet6_mc_check(sk, loc_addr, rmt_addr))
		return false;
	return true;
}

static void udp6_csum_zero_error(struct sk_buff *skb)
{
	/* RFC 2460 section 8.1 says that we SHOULD log
	 * this error. Well, it is reasonable.
	 */
	net_dbg_ratelimited("IPv6: udp checksum is 0 for [%pI6c]:%u->[%pI6c]:%u\n",
			    &ipv6_hdr(skb)->saddr, ntohs(udp_hdr(skb)->source),
			    &ipv6_hdr(skb)->daddr, ntohs(udp_hdr(skb)->dest));
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Note: called only from the BH handler context,
 * so we don't need to lock the hashes.
 */
static int __udp6_lib_mcast_deliver(struct net *net, struct sk_buff *skb,
		const struct in6_addr *saddr, const struct in6_addr *daddr,
<<<<<<< HEAD
		struct udp_table *udptable)
{
	struct sock *sk, *stack[256 / sizeof(struct sock *)];
	const struct udphdr *uh = udp_hdr(skb);
	struct udp_hslot *hslot = udp_hashslot(udptable, net, ntohs(uh->dest));
	int dif;
	unsigned int i, count = 0;

	spin_lock(&hslot->lock);
	sk = sk_nulls_head(&hslot->head);
	dif = inet6_iif(skb);
	sk = udp_v6_mcast_next(net, sk, uh->dest, daddr, uh->source, saddr, dif);
	while (sk) {
		stack[count++] = sk;
		sk = udp_v6_mcast_next(net, sk_nulls_next(sk), uh->dest, daddr,
				       uh->source, saddr, dif);
		if (unlikely(count == ARRAY_SIZE(stack))) {
			if (!sk)
				break;
			flush_stack(stack, count, skb, ~0);
			count = 0;
		}
	}
	/*
	 * before releasing the lock, we must take reference on sockets
	 */
	for (i = 0; i < count; i++)
		sock_hold(stack[i]);

	spin_unlock(&hslot->lock);

	if (count) {
		flush_stack(stack, count, skb, count - 1);

		for (i = 0; i < count; i++)
			sock_put(stack[i]);
	} else {
		kfree_skb(skb);
=======
		struct udp_table *udptable, int proto)
{
	struct sock *sk, *first = NULL;
	const struct udphdr *uh = udp_hdr(skb);
	unsigned short hnum = ntohs(uh->dest);
	struct udp_hslot *hslot = udp_hashslot(udptable, net, hnum);
	unsigned int offset = offsetof(typeof(*sk), sk_node);
	unsigned int hash2 = 0, hash2_any = 0, use_hash2 = (hslot->count > 10);
	int dif = inet6_iif(skb);
	int sdif = inet6_sdif(skb);
	struct hlist_node *node;
	struct sk_buff *nskb;

	if (use_hash2) {
		hash2_any = ipv6_portaddr_hash(net, &in6addr_any, hnum) &
			    udptable->mask;
		hash2 = ipv6_portaddr_hash(net, daddr, hnum) & udptable->mask;
start_lookup:
		hslot = &udptable->hash2[hash2];
		offset = offsetof(typeof(*sk), __sk_common.skc_portaddr_node);
	}

	sk_for_each_entry_offset_rcu(sk, node, &hslot->head, offset) {
		if (!__udp_v6_is_mcast_sock(net, sk, uh->dest, daddr,
					    uh->source, saddr, dif, sdif,
					    hnum))
			continue;
		/* If zero checksum and no_check is not on for
		 * the socket then skip it.
		 */
		if (!uh->check && !udp_get_no_check6_rx(sk))
			continue;
		if (!first) {
			first = sk;
			continue;
		}
		nskb = skb_clone(skb, GFP_ATOMIC);
		if (unlikely(!nskb)) {
			atomic_inc(&sk->sk_drops);
			__UDP6_INC_STATS(net, UDP_MIB_RCVBUFERRORS,
					 IS_UDPLITE(sk));
			__UDP6_INC_STATS(net, UDP_MIB_INERRORS,
					 IS_UDPLITE(sk));
			continue;
		}

		if (udpv6_queue_rcv_skb(sk, nskb) > 0)
			consume_skb(nskb);
	}

	/* Also lookup *:port if we are using hash2 and haven't done so yet. */
	if (use_hash2 && hash2 != hash2_any) {
		hash2 = hash2_any;
		goto start_lookup;
	}

	if (first) {
		if (udpv6_queue_rcv_skb(first, skb) > 0)
			consume_skb(skb);
	} else {
		kfree_skb(skb);
		__UDP6_INC_STATS(net, UDP_MIB_IGNOREDMULTI,
				 proto == IPPROTO_UDPLITE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return 0;
}

<<<<<<< HEAD
static inline int udp6_csum_init(struct sk_buff *skb, struct udphdr *uh,
				 int proto)
{
	int err;

	UDP_SKB_CB(skb)->partial_cov = 0;
	UDP_SKB_CB(skb)->cscov = skb->len;

	if (proto == IPPROTO_UDPLITE) {
		err = udplite_checksum_init(skb, uh);
		if (err)
			return err;
	}

	if (uh->check == 0) {
		/* RFC 2460 section 8.1 says that we SHOULD log
		   this error. Well, it is reasonable.
		 */
		LIMIT_NETDEBUG(KERN_INFO "IPv6: udp checksum is 0\n");
		return 1;
	}
	if (skb->ip_summed == CHECKSUM_COMPLETE &&
	    !csum_ipv6_magic(&ipv6_hdr(skb)->saddr, &ipv6_hdr(skb)->daddr,
			     skb->len, proto, skb->csum))
		skb->ip_summed = CHECKSUM_UNNECESSARY;

	if (!skb_csum_unnecessary(skb))
		skb->csum = ~csum_unfold(csum_ipv6_magic(&ipv6_hdr(skb)->saddr,
							 &ipv6_hdr(skb)->daddr,
							 skb->len, proto, 0));

=======
static void udp6_sk_rx_dst_set(struct sock *sk, struct dst_entry *dst)
{
	if (udp_sk_rx_dst_set(sk, dst)) {
		const struct rt6_info *rt = (const struct rt6_info *)dst;

		sk->sk_rx_dst_cookie = rt6_get_cookie(rt);
	}
}

/* wrapper for udp_queue_rcv_skb tacking care of csum conversion and
 * return code conversion for ip layer consumption
 */
static int udp6_unicast_rcv_skb(struct sock *sk, struct sk_buff *skb,
				struct udphdr *uh)
{
	int ret;

	if (inet_get_convert_csum(sk) && uh->check && !IS_UDPLITE(sk))
		skb_checksum_try_convert(skb, IPPROTO_UDP, ip6_compute_pseudo);

	ret = udpv6_queue_rcv_skb(sk, skb);

	/* a return value > 0 means to resubmit the input */
	if (ret > 0)
		return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

int __udp6_lib_rcv(struct sk_buff *skb, struct udp_table *udptable,
		   int proto)
{
<<<<<<< HEAD
	struct net *net = dev_net(skb->dev);
	struct sock *sk;
	struct udphdr *uh;
	const struct in6_addr *saddr, *daddr;
=======
	enum skb_drop_reason reason = SKB_DROP_REASON_NOT_SPECIFIED;
	const struct in6_addr *saddr, *daddr;
	struct net *net = dev_net(skb->dev);
	struct udphdr *uh;
	struct sock *sk;
	bool refcounted;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u32 ulen = 0;

	if (!pskb_may_pull(skb, sizeof(struct udphdr)))
		goto discard;

	saddr = &ipv6_hdr(skb)->saddr;
	daddr = &ipv6_hdr(skb)->daddr;
	uh = udp_hdr(skb);

	ulen = ntohs(uh->len);
	if (ulen > skb->len)
		goto short_packet;

	if (proto == IPPROTO_UDP) {
		/* UDP validates ulen. */

		/* Check for jumbo payload */
		if (ulen == 0)
			ulen = skb->len;

		if (ulen < sizeof(*uh))
			goto short_packet;

		if (ulen < skb->len) {
			if (pskb_trim_rcsum(skb, ulen))
				goto short_packet;
			saddr = &ipv6_hdr(skb)->saddr;
			daddr = &ipv6_hdr(skb)->daddr;
			uh = udp_hdr(skb);
		}
	}

	if (udp6_csum_init(skb, uh, proto))
<<<<<<< HEAD
		goto discard;
=======
		goto csum_error;

	/* Check if the socket is already available, e.g. due to early demux */
	sk = inet6_steal_sock(net, skb, sizeof(struct udphdr), saddr, uh->source, daddr, uh->dest,
			      &refcounted, udp6_ehashfn);
	if (IS_ERR(sk))
		goto no_sk;

	if (sk) {
		struct dst_entry *dst = skb_dst(skb);
		int ret;

		if (unlikely(rcu_dereference(sk->sk_rx_dst) != dst))
			udp6_sk_rx_dst_set(sk, dst);

		if (!uh->check && !udp_get_no_check6_rx(sk)) {
			if (refcounted)
				sock_put(sk);
			goto report_csum_error;
		}

		ret = udp6_unicast_rcv_skb(sk, skb, uh);
		if (refcounted)
			sock_put(sk);
		return ret;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 *	Multicast receive code
	 */
	if (ipv6_addr_is_multicast(daddr))
		return __udp6_lib_mcast_deliver(net, skb,
<<<<<<< HEAD
				saddr, daddr, udptable);

	/* Unicast */

	/*
	 * check socket cache ... must talk to Alan about his plans
	 * for sock caches... i'll skip this for now.
	 */
	sk = __udp6_lib_lookup_skb(skb, uh->source, uh->dest, udptable);

	if (sk == NULL) {
		if (!xfrm6_policy_check(NULL, XFRM_POLICY_IN, skb))
			goto discard;

		if (udp_lib_checksum_complete(skb))
			goto discard;
		UDP6_INC_STATS_BH(net, UDP_MIB_NOPORTS,
				proto == IPPROTO_UDPLITE);

		icmpv6_send(skb, ICMPV6_DEST_UNREACH, ICMPV6_PORT_UNREACH, 0);

		kfree_skb(skb);
		return 0;
	}

	/* deliver */

	if (sk_rcvqueues_full(sk, skb)) {
		sock_put(sk);
		goto discard;
	}
	bh_lock_sock(sk);
	if (!sock_owned_by_user(sk))
		udpv6_queue_rcv_skb(sk, skb);
	else if (sk_add_backlog(sk, skb)) {
		atomic_inc(&sk->sk_drops);
		bh_unlock_sock(sk);
		sock_put(sk);
		goto discard;
	}
	bh_unlock_sock(sk);
	sock_put(sk);
	return 0;

short_packet:
	LIMIT_NETDEBUG(KERN_DEBUG "UDP%sv6: short packet: From [%pI6c]:%u %d/%d to [%pI6c]:%u\n",
		       proto == IPPROTO_UDPLITE ? "-Lite" : "",
		       saddr,
		       ntohs(uh->source),
		       ulen,
		       skb->len,
		       daddr,
		       ntohs(uh->dest));

discard:
	UDP6_INC_STATS_BH(net, UDP_MIB_INERRORS, proto == IPPROTO_UDPLITE);
	kfree_skb(skb);
	return 0;
}

static __inline__ int udpv6_rcv(struct sk_buff *skb)
{
	return __udp6_lib_rcv(skb, &udp_table, IPPROTO_UDP);
=======
				saddr, daddr, udptable, proto);

	/* Unicast */
	sk = __udp6_lib_lookup_skb(skb, uh->source, uh->dest, udptable);
	if (sk) {
		if (!uh->check && !udp_get_no_check6_rx(sk))
			goto report_csum_error;
		return udp6_unicast_rcv_skb(sk, skb, uh);
	}
no_sk:
	reason = SKB_DROP_REASON_NO_SOCKET;

	if (!uh->check)
		goto report_csum_error;

	if (!xfrm6_policy_check(NULL, XFRM_POLICY_IN, skb))
		goto discard;
	nf_reset_ct(skb);

	if (udp_lib_checksum_complete(skb))
		goto csum_error;

	__UDP6_INC_STATS(net, UDP_MIB_NOPORTS, proto == IPPROTO_UDPLITE);
	icmpv6_send(skb, ICMPV6_DEST_UNREACH, ICMPV6_PORT_UNREACH, 0);

	kfree_skb_reason(skb, reason);
	return 0;

short_packet:
	if (reason == SKB_DROP_REASON_NOT_SPECIFIED)
		reason = SKB_DROP_REASON_PKT_TOO_SMALL;
	net_dbg_ratelimited("UDP%sv6: short packet: From [%pI6c]:%u %d/%d to [%pI6c]:%u\n",
			    proto == IPPROTO_UDPLITE ? "-Lite" : "",
			    saddr, ntohs(uh->source),
			    ulen, skb->len,
			    daddr, ntohs(uh->dest));
	goto discard;

report_csum_error:
	udp6_csum_zero_error(skb);
csum_error:
	if (reason == SKB_DROP_REASON_NOT_SPECIFIED)
		reason = SKB_DROP_REASON_UDP_CSUM;
	__UDP6_INC_STATS(net, UDP_MIB_CSUMERRORS, proto == IPPROTO_UDPLITE);
discard:
	__UDP6_INC_STATS(net, UDP_MIB_INERRORS, proto == IPPROTO_UDPLITE);
	kfree_skb_reason(skb, reason);
	return 0;
}


static struct sock *__udp6_lib_demux_lookup(struct net *net,
			__be16 loc_port, const struct in6_addr *loc_addr,
			__be16 rmt_port, const struct in6_addr *rmt_addr,
			int dif, int sdif)
{
	struct udp_table *udptable = net->ipv4.udp_table;
	unsigned short hnum = ntohs(loc_port);
	unsigned int hash2, slot2;
	struct udp_hslot *hslot2;
	__portpair ports;
	struct sock *sk;

	hash2 = ipv6_portaddr_hash(net, loc_addr, hnum);
	slot2 = hash2 & udptable->mask;
	hslot2 = &udptable->hash2[slot2];
	ports = INET_COMBINED_PORTS(rmt_port, hnum);

	udp_portaddr_for_each_entry_rcu(sk, &hslot2->head) {
		if (sk->sk_state == TCP_ESTABLISHED &&
		    inet6_match(net, sk, rmt_addr, loc_addr, ports, dif, sdif))
			return sk;
		/* Only check first socket in chain */
		break;
	}
	return NULL;
}

void udp_v6_early_demux(struct sk_buff *skb)
{
	struct net *net = dev_net(skb->dev);
	const struct udphdr *uh;
	struct sock *sk;
	struct dst_entry *dst;
	int dif = skb->dev->ifindex;
	int sdif = inet6_sdif(skb);

	if (!pskb_may_pull(skb, skb_transport_offset(skb) +
	    sizeof(struct udphdr)))
		return;

	uh = udp_hdr(skb);

	if (skb->pkt_type == PACKET_HOST)
		sk = __udp6_lib_demux_lookup(net, uh->dest,
					     &ipv6_hdr(skb)->daddr,
					     uh->source, &ipv6_hdr(skb)->saddr,
					     dif, sdif);
	else
		return;

	if (!sk)
		return;

	skb->sk = sk;
	DEBUG_NET_WARN_ON_ONCE(sk_is_refcounted(sk));
	skb->destructor = sock_pfree;
	dst = rcu_dereference(sk->sk_rx_dst);

	if (dst)
		dst = dst_check(dst, sk->sk_rx_dst_cookie);
	if (dst) {
		/* set noref for now.
		 * any place which wants to hold dst has to call
		 * dst_hold_safe()
		 */
		skb_dst_set_noref(skb, dst);
	}
}

INDIRECT_CALLABLE_SCOPE int udpv6_rcv(struct sk_buff *skb)
{
	return __udp6_lib_rcv(skb, dev_net(skb->dev)->ipv4.udp_table, IPPROTO_UDP);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Throw away all pending data and cancel the corking. Socket is locked.
 */
static void udp_v6_flush_pending_frames(struct sock *sk)
{
	struct udp_sock *up = udp_sk(sk);

	if (up->pending == AF_INET)
		udp_flush_pending_frames(sk);
	else if (up->pending) {
		up->len = 0;
<<<<<<< HEAD
		up->pending = 0;
=======
		WRITE_ONCE(up->pending, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ip6_flush_pending_frames(sk);
	}
}

<<<<<<< HEAD
/**
 * 	udp6_hwcsum_outgoing  -  handle outgoing HW checksumming
 * 	@sk: 	socket we are sending on
 * 	@skb: 	sk_buff containing the filled-in UDP header
 * 	        (checksum field must be zeroed out)
=======
static int udpv6_pre_connect(struct sock *sk, struct sockaddr *uaddr,
			     int addr_len)
{
	if (addr_len < offsetofend(struct sockaddr, sa_family))
		return -EINVAL;
	/* The following checks are replicated from __ip6_datagram_connect()
	 * and intended to prevent BPF program called below from accessing
	 * bytes that are out of the bound specified by user in addr_len.
	 */
	if (uaddr->sa_family == AF_INET) {
		if (ipv6_only_sock(sk))
			return -EAFNOSUPPORT;
		return udp_pre_connect(sk, uaddr, addr_len);
	}

	if (addr_len < SIN6_LEN_RFC2133)
		return -EINVAL;

	return BPF_CGROUP_RUN_PROG_INET6_CONNECT_LOCK(sk, uaddr, &addr_len);
}

/**
 *	udp6_hwcsum_outgoing  -  handle outgoing HW checksumming
 *	@sk:	socket we are sending on
 *	@skb:	sk_buff containing the filled-in UDP header
 *		(checksum field must be zeroed out)
 *	@saddr: source address
 *	@daddr: destination address
 *	@len:	length of packet
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
static void udp6_hwcsum_outgoing(struct sock *sk, struct sk_buff *skb,
				 const struct in6_addr *saddr,
				 const struct in6_addr *daddr, int len)
{
	unsigned int offset;
	struct udphdr *uh = udp_hdr(skb);
<<<<<<< HEAD
	__wsum csum = 0;

	if (skb_queue_len(&sk->sk_write_queue) == 1) {
=======
	struct sk_buff *frags = skb_shinfo(skb)->frag_list;
	__wsum csum = 0;

	if (!frags) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* Only one fragment on the socket.  */
		skb->csum_start = skb_transport_header(skb) - skb->head;
		skb->csum_offset = offsetof(struct udphdr, check);
		uh->check = ~csum_ipv6_magic(saddr, daddr, len, IPPROTO_UDP, 0);
	} else {
		/*
		 * HW-checksum won't work as there are two or more
		 * fragments on the socket so that all csums of sk_buffs
		 * should be together
		 */
		offset = skb_transport_offset(skb);
		skb->csum = skb_checksum(skb, offset, skb->len - offset, 0);
<<<<<<< HEAD

		skb->ip_summed = CHECKSUM_NONE;

		skb_queue_walk(&sk->sk_write_queue, skb) {
			csum = csum_add(csum, skb->csum);
		}
=======
		csum = skb->csum;

		skb->ip_summed = CHECKSUM_NONE;

		do {
			csum = csum_add(csum, frags->csum);
		} while ((frags = frags->next));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		uh->check = csum_ipv6_magic(saddr, daddr, len, IPPROTO_UDP,
					    csum);
		if (uh->check == 0)
			uh->check = CSUM_MANGLED_0;
	}
}

/*
 *	Sending
 */

<<<<<<< HEAD
static int udp_v6_push_pending_frames(struct sock *sk)
{
	struct sk_buff *skb;
	struct udphdr *uh;
	struct udp_sock  *up = udp_sk(sk);
	struct inet_sock *inet = inet_sk(sk);
	struct flowi6 *fl6;
	int err = 0;
	int is_udplite = IS_UDPLITE(sk);
	__wsum csum = 0;

	if (up->pending == AF_INET)
		return udp_push_pending_frames(sk);

	fl6 = &inet->cork.fl.u.ip6;

	/* Grab the skbuff where UDP header space exists. */
	if ((skb = skb_peek(&sk->sk_write_queue)) == NULL)
		goto out;
=======
static int udp_v6_send_skb(struct sk_buff *skb, struct flowi6 *fl6,
			   struct inet_cork *cork)
{
	struct sock *sk = skb->sk;
	struct udphdr *uh;
	int err = 0;
	int is_udplite = IS_UDPLITE(sk);
	__wsum csum = 0;
	int offset = skb_transport_offset(skb);
	int len = skb->len - offset;
	int datalen = len - sizeof(*uh);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Create a UDP header
	 */
	uh = udp_hdr(skb);
	uh->source = fl6->fl6_sport;
	uh->dest = fl6->fl6_dport;
<<<<<<< HEAD
	uh->len = htons(up->len);
	uh->check = 0;

	if (is_udplite)
		csum = udplite_csum_outgoing(sk, skb);
	else if (skb->ip_summed == CHECKSUM_PARTIAL) { /* UDP hardware csum */
		udp6_hwcsum_outgoing(sk, skb, &fl6->saddr, &fl6->daddr,
				     up->len);
		goto send;
	} else
		csum = udp_csum_outgoing(sk, skb);

	/* add protocol-dependent pseudo-header */
	uh->check = csum_ipv6_magic(&fl6->saddr, &fl6->daddr,
				    up->len, fl6->flowi6_proto, csum);
=======
	uh->len = htons(len);
	uh->check = 0;

	if (cork->gso_size) {
		const int hlen = skb_network_header_len(skb) +
				 sizeof(struct udphdr);

		if (hlen + cork->gso_size > cork->fragsize) {
			kfree_skb(skb);
			return -EINVAL;
		}
		if (datalen > cork->gso_size * UDP_MAX_SEGMENTS) {
			kfree_skb(skb);
			return -EINVAL;
		}
		if (udp_get_no_check6_tx(sk)) {
			kfree_skb(skb);
			return -EINVAL;
		}
		if (skb->ip_summed != CHECKSUM_PARTIAL || is_udplite ||
		    dst_xfrm(skb_dst(skb))) {
			kfree_skb(skb);
			return -EIO;
		}

		if (datalen > cork->gso_size) {
			skb_shinfo(skb)->gso_size = cork->gso_size;
			skb_shinfo(skb)->gso_type = SKB_GSO_UDP_L4;
			skb_shinfo(skb)->gso_segs = DIV_ROUND_UP(datalen,
								 cork->gso_size);
		}
		goto csum_partial;
	}

	if (is_udplite)
		csum = udplite_csum(skb);
	else if (udp_get_no_check6_tx(sk)) {   /* UDP csum disabled */
		skb->ip_summed = CHECKSUM_NONE;
		goto send;
	} else if (skb->ip_summed == CHECKSUM_PARTIAL) { /* UDP hardware csum */
csum_partial:
		udp6_hwcsum_outgoing(sk, skb, &fl6->saddr, &fl6->daddr, len);
		goto send;
	} else
		csum = udp_csum(skb);

	/* add protocol-dependent pseudo-header */
	uh->check = csum_ipv6_magic(&fl6->saddr, &fl6->daddr,
				    len, fl6->flowi6_proto, csum);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (uh->check == 0)
		uh->check = CSUM_MANGLED_0;

send:
<<<<<<< HEAD
	err = ip6_push_pending_frames(sk);
	if (err) {
		if (err == -ENOBUFS && !inet6_sk(sk)->recverr) {
			UDP6_INC_STATS_USER(sock_net(sk),
					    UDP_MIB_SNDBUFERRORS, is_udplite);
			err = 0;
		}
	} else
		UDP6_INC_STATS_USER(sock_net(sk),
				    UDP_MIB_OUTDATAGRAMS, is_udplite);
out:
	up->len = 0;
	up->pending = 0;
	return err;
}

int udpv6_sendmsg(struct kiocb *iocb, struct sock *sk,
		  struct msghdr *msg, size_t len)
=======
	err = ip6_send_skb(skb);
	if (err) {
		if (err == -ENOBUFS && !inet6_test_bit(RECVERR6, sk)) {
			UDP6_INC_STATS(sock_net(sk),
				       UDP_MIB_SNDBUFERRORS, is_udplite);
			err = 0;
		}
	} else {
		UDP6_INC_STATS(sock_net(sk),
			       UDP_MIB_OUTDATAGRAMS, is_udplite);
	}
	return err;
}

static int udp_v6_push_pending_frames(struct sock *sk)
{
	struct sk_buff *skb;
	struct udp_sock  *up = udp_sk(sk);
	int err = 0;

	if (up->pending == AF_INET)
		return udp_push_pending_frames(sk);

	skb = ip6_finish_skb(sk);
	if (!skb)
		goto out;

	err = udp_v6_send_skb(skb, &inet_sk(sk)->cork.fl.u.ip6,
			      &inet_sk(sk)->cork.base);
out:
	up->len = 0;
	WRITE_ONCE(up->pending, 0);
	return err;
}

int udpv6_sendmsg(struct sock *sk, struct msghdr *msg, size_t len)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ipv6_txoptions opt_space;
	struct udp_sock *up = udp_sk(sk);
	struct inet_sock *inet = inet_sk(sk);
	struct ipv6_pinfo *np = inet6_sk(sk);
<<<<<<< HEAD
	struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *) msg->msg_name;
	struct in6_addr *daddr, *final_p, final;
	struct ipv6_txoptions *opt = NULL;
	struct ip6_flowlabel *flowlabel = NULL;
	struct flowi6 fl6;
	struct dst_entry *dst;
	int addr_len = msg->msg_namelen;
	int ulen = len;
	int hlimit = -1;
	int tclass = -1;
	int dontfrag = -1;
	int corkreq = up->corkflag || msg->msg_flags&MSG_MORE;
	int err;
	int connected = 0;
	int is_udplite = IS_UDPLITE(sk);
	int (*getfrag)(void *, char *, int, int, int, struct sk_buff *);

=======
	DECLARE_SOCKADDR(struct sockaddr_in6 *, sin6, msg->msg_name);
	struct in6_addr *daddr, *final_p, final;
	struct ipv6_txoptions *opt = NULL;
	struct ipv6_txoptions *opt_to_free = NULL;
	struct ip6_flowlabel *flowlabel = NULL;
	struct inet_cork_full cork;
	struct flowi6 *fl6 = &cork.fl.u.ip6;
	struct dst_entry *dst;
	struct ipcm6_cookie ipc6;
	int addr_len = msg->msg_namelen;
	bool connected = false;
	int ulen = len;
	int corkreq = udp_test_bit(CORK, sk) || msg->msg_flags & MSG_MORE;
	int err;
	int is_udplite = IS_UDPLITE(sk);
	int (*getfrag)(void *, char *, int, int, int, struct sk_buff *);

	ipcm6_init(&ipc6);
	ipc6.gso_size = READ_ONCE(up->gso_size);
	ipc6.sockc.tsflags = READ_ONCE(sk->sk_tsflags);
	ipc6.sockc.mark = READ_ONCE(sk->sk_mark);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* destination address check */
	if (sin6) {
		if (addr_len < offsetof(struct sockaddr, sa_data))
			return -EINVAL;

		switch (sin6->sin6_family) {
		case AF_INET6:
			if (addr_len < SIN6_LEN_RFC2133)
				return -EINVAL;
			daddr = &sin6->sin6_addr;
<<<<<<< HEAD
=======
			if (ipv6_addr_any(daddr) &&
			    ipv6_addr_v4mapped(&np->saddr))
				ipv6_addr_set_v4mapped(htonl(INADDR_LOOPBACK),
						       daddr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		case AF_INET:
			goto do_udp_sendmsg;
		case AF_UNSPEC:
			msg->msg_name = sin6 = NULL;
			msg->msg_namelen = addr_len = 0;
			daddr = NULL;
			break;
		default:
			return -EINVAL;
		}
<<<<<<< HEAD
	} else if (!up->pending) {
		if (sk->sk_state != TCP_ESTABLISHED)
			return -EDESTADDRREQ;
		daddr = &np->daddr;
=======
	} else if (!READ_ONCE(up->pending)) {
		if (sk->sk_state != TCP_ESTABLISHED)
			return -EDESTADDRREQ;
		daddr = &sk->sk_v6_daddr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else
		daddr = NULL;

	if (daddr) {
		if (ipv6_addr_v4mapped(daddr)) {
			struct sockaddr_in sin;
			sin.sin_family = AF_INET;
			sin.sin_port = sin6 ? sin6->sin6_port : inet->inet_dport;
			sin.sin_addr.s_addr = daddr->s6_addr32[3];
			msg->msg_name = &sin;
			msg->msg_namelen = sizeof(sin);
do_udp_sendmsg:
<<<<<<< HEAD
			if (__ipv6_only_sock(sk))
				return -ENETUNREACH;
			return udp_sendmsg(iocb, sk, msg, len);
		}
	}

	if (up->pending == AF_INET)
		return udp_sendmsg(iocb, sk, msg, len);

=======
			err = ipv6_only_sock(sk) ?
				-ENETUNREACH : udp_sendmsg(sk, msg, len);
			msg->msg_name = sin6;
			msg->msg_namelen = addr_len;
			return err;
		}
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Rough check on arithmetic overflow,
	   better check is made in ip6_append_data().
	   */
	if (len > INT_MAX - sizeof(struct udphdr))
		return -EMSGSIZE;

<<<<<<< HEAD
	if (up->pending) {
=======
	getfrag  =  is_udplite ?  udplite_getfrag : ip_generic_getfrag;
	if (READ_ONCE(up->pending)) {
		if (READ_ONCE(up->pending) == AF_INET)
			return udp_sendmsg(sk, msg, len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * There are pending frames.
		 * The socket lock must be held while it's corked.
		 */
		lock_sock(sk);
		if (likely(up->pending)) {
			if (unlikely(up->pending != AF_INET6)) {
				release_sock(sk);
				return -EAFNOSUPPORT;
			}
			dst = NULL;
			goto do_append_data;
		}
		release_sock(sk);
	}
	ulen += sizeof(struct udphdr);

<<<<<<< HEAD
	memset(&fl6, 0, sizeof(fl6));
=======
	memset(fl6, 0, sizeof(*fl6));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (sin6) {
		if (sin6->sin6_port == 0)
			return -EINVAL;

<<<<<<< HEAD
		fl6.fl6_dport = sin6->sin6_port;
		daddr = &sin6->sin6_addr;

		if (np->sndflow) {
			fl6.flowlabel = sin6->sin6_flowinfo&IPV6_FLOWINFO_MASK;
			if (fl6.flowlabel&IPV6_FLOWLABEL_MASK) {
				flowlabel = fl6_sock_lookup(sk, fl6.flowlabel);
				if (flowlabel == NULL)
					return -EINVAL;
				daddr = &flowlabel->dst;
=======
		fl6->fl6_dport = sin6->sin6_port;
		daddr = &sin6->sin6_addr;

		if (inet6_test_bit(SNDFLOW, sk)) {
			fl6->flowlabel = sin6->sin6_flowinfo&IPV6_FLOWINFO_MASK;
			if (fl6->flowlabel & IPV6_FLOWLABEL_MASK) {
				flowlabel = fl6_sock_lookup(sk, fl6->flowlabel);
				if (IS_ERR(flowlabel))
					return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			}
		}

		/*
		 * Otherwise it will be difficult to maintain
		 * sk->sk_dst_cache.
		 */
		if (sk->sk_state == TCP_ESTABLISHED &&
<<<<<<< HEAD
		    ipv6_addr_equal(daddr, &np->daddr))
			daddr = &np->daddr;

		if (addr_len >= sizeof(struct sockaddr_in6) &&
		    sin6->sin6_scope_id &&
		    ipv6_addr_type(daddr)&IPV6_ADDR_LINKLOCAL)
			fl6.flowi6_oif = sin6->sin6_scope_id;
=======
		    ipv6_addr_equal(daddr, &sk->sk_v6_daddr))
			daddr = &sk->sk_v6_daddr;

		if (addr_len >= sizeof(struct sockaddr_in6) &&
		    sin6->sin6_scope_id &&
		    __ipv6_addr_needs_scope_id(__ipv6_addr_type(daddr)))
			fl6->flowi6_oif = sin6->sin6_scope_id;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		if (sk->sk_state != TCP_ESTABLISHED)
			return -EDESTADDRREQ;

<<<<<<< HEAD
		fl6.fl6_dport = inet->inet_dport;
		daddr = &np->daddr;
		fl6.flowlabel = np->flow_label;
		connected = 1;
	}

	if (!fl6.flowi6_oif)
		fl6.flowi6_oif = sk->sk_bound_dev_if;

	if (!fl6.flowi6_oif)
		fl6.flowi6_oif = np->sticky_pktinfo.ipi6_ifindex;

	fl6.flowi6_mark = sk->sk_mark;
	fl6.flowi6_uid = sk->sk_uid;
=======
		fl6->fl6_dport = inet->inet_dport;
		daddr = &sk->sk_v6_daddr;
		fl6->flowlabel = np->flow_label;
		connected = true;
	}

	if (!fl6->flowi6_oif)
		fl6->flowi6_oif = READ_ONCE(sk->sk_bound_dev_if);

	if (!fl6->flowi6_oif)
		fl6->flowi6_oif = np->sticky_pktinfo.ipi6_ifindex;

	fl6->flowi6_uid = sk->sk_uid;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (msg->msg_controllen) {
		opt = &opt_space;
		memset(opt, 0, sizeof(struct ipv6_txoptions));
		opt->tot_len = sizeof(*opt);
<<<<<<< HEAD

		err = datagram_send_ctl(sock_net(sk), sk, msg, &fl6, opt,
					&hlimit, &tclass, &dontfrag);
=======
		ipc6.opt = opt;

		err = udp_cmsg_send(sk, msg, &ipc6.gso_size);
		if (err > 0) {
			err = ip6_datagram_send_ctl(sock_net(sk), sk, msg, fl6,
						    &ipc6);
			connected = false;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (err < 0) {
			fl6_sock_release(flowlabel);
			return err;
		}
<<<<<<< HEAD
		if ((fl6.flowlabel&IPV6_FLOWLABEL_MASK) && !flowlabel) {
			flowlabel = fl6_sock_lookup(sk, fl6.flowlabel);
			if (flowlabel == NULL)
=======
		if ((fl6->flowlabel&IPV6_FLOWLABEL_MASK) && !flowlabel) {
			flowlabel = fl6_sock_lookup(sk, fl6->flowlabel);
			if (IS_ERR(flowlabel))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				return -EINVAL;
		}
		if (!(opt->opt_nflen|opt->opt_flen))
			opt = NULL;
<<<<<<< HEAD
		connected = 0;
	}
	if (opt == NULL)
		opt = np->opt;
	if (flowlabel)
		opt = fl6_merge_options(&opt_space, flowlabel, opt);
	opt = ipv6_fixup_options(&opt_space, opt);

	fl6.flowi6_proto = sk->sk_protocol;
	if (!ipv6_addr_any(daddr))
		fl6.daddr = *daddr;
	else
		fl6.daddr.s6_addr[15] = 0x1; /* :: means loopback (BSD'ism) */
	if (ipv6_addr_any(&fl6.saddr) && !ipv6_addr_any(&np->saddr))
		fl6.saddr = np->saddr;
	fl6.fl6_sport = inet->inet_sport;

	final_p = fl6_update_dst(&fl6, opt, &final);
	if (final_p)
		connected = 0;

	if (!fl6.flowi6_oif && ipv6_addr_is_multicast(&fl6.daddr)) {
		fl6.flowi6_oif = np->mcast_oif;
		connected = 0;
	} else if (!fl6.flowi6_oif)
		fl6.flowi6_oif = np->ucast_oif;

	security_sk_classify_flow(sk, flowi6_to_flowi(&fl6));

	dst = ip6_sk_dst_lookup_flow(sk, &fl6, final_p, true);
=======
	}
	if (!opt) {
		opt = txopt_get(np);
		opt_to_free = opt;
	}
	if (flowlabel)
		opt = fl6_merge_options(&opt_space, flowlabel, opt);
	opt = ipv6_fixup_options(&opt_space, opt);
	ipc6.opt = opt;

	fl6->flowi6_proto = sk->sk_protocol;
	fl6->flowi6_mark = ipc6.sockc.mark;
	fl6->daddr = *daddr;
	if (ipv6_addr_any(&fl6->saddr) && !ipv6_addr_any(&np->saddr))
		fl6->saddr = np->saddr;
	fl6->fl6_sport = inet->inet_sport;

	if (cgroup_bpf_enabled(CGROUP_UDP6_SENDMSG) && !connected) {
		err = BPF_CGROUP_RUN_PROG_UDP6_SENDMSG_LOCK(sk,
					   (struct sockaddr *)sin6,
					   &addr_len,
					   &fl6->saddr);
		if (err)
			goto out_no_dst;
		if (sin6) {
			if (ipv6_addr_v4mapped(&sin6->sin6_addr)) {
				/* BPF program rewrote IPv6-only by IPv4-mapped
				 * IPv6. It's currently unsupported.
				 */
				err = -ENOTSUPP;
				goto out_no_dst;
			}
			if (sin6->sin6_port == 0) {
				/* BPF program set invalid port. Reject it. */
				err = -EINVAL;
				goto out_no_dst;
			}
			fl6->fl6_dport = sin6->sin6_port;
			fl6->daddr = sin6->sin6_addr;
		}
	}

	if (ipv6_addr_any(&fl6->daddr))
		fl6->daddr.s6_addr[15] = 0x1; /* :: means loopback (BSD'ism) */

	final_p = fl6_update_dst(fl6, opt, &final);
	if (final_p)
		connected = false;

	if (!fl6->flowi6_oif && ipv6_addr_is_multicast(&fl6->daddr)) {
		fl6->flowi6_oif = READ_ONCE(np->mcast_oif);
		connected = false;
	} else if (!fl6->flowi6_oif)
		fl6->flowi6_oif = READ_ONCE(np->ucast_oif);

	security_sk_classify_flow(sk, flowi6_to_flowi_common(fl6));

	if (ipc6.tclass < 0)
		ipc6.tclass = np->tclass;

	fl6->flowlabel = ip6_make_flowinfo(ipc6.tclass, fl6->flowlabel);

	dst = ip6_sk_dst_lookup_flow(sk, fl6, final_p, connected);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (IS_ERR(dst)) {
		err = PTR_ERR(dst);
		dst = NULL;
		goto out;
	}

<<<<<<< HEAD
	if (hlimit < 0) {
		if (ipv6_addr_is_multicast(&fl6.daddr))
			hlimit = np->mcast_hops;
		else
			hlimit = np->hop_limit;
		if (hlimit < 0)
			hlimit = ip6_dst_hoplimit(dst);
	}

	if (tclass < 0)
		tclass = np->tclass;

	if (dontfrag < 0)
		dontfrag = np->dontfrag;
=======
	if (ipc6.hlimit < 0)
		ipc6.hlimit = ip6_sk_dst_hoplimit(np, fl6, dst);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (msg->msg_flags&MSG_CONFIRM)
		goto do_confirm;
back_from_confirm:

<<<<<<< HEAD
=======
	/* Lockless fast path for the non-corking case */
	if (!corkreq) {
		struct sk_buff *skb;

		skb = ip6_make_skb(sk, getfrag, msg, ulen,
				   sizeof(struct udphdr), &ipc6,
				   (struct rt6_info *)dst,
				   msg->msg_flags, &cork);
		err = PTR_ERR(skb);
		if (!IS_ERR_OR_NULL(skb))
			err = udp_v6_send_skb(skb, fl6, &cork.base);
		/* ip6_make_skb steals dst reference */
		goto out_no_dst;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	lock_sock(sk);
	if (unlikely(up->pending)) {
		/* The socket is already corked while preparing it. */
		/* ... which is an evident application bug. --ANK */
		release_sock(sk);

<<<<<<< HEAD
		LIMIT_NETDEBUG(KERN_DEBUG "udp cork app bug 2\n");
=======
		net_dbg_ratelimited("udp cork app bug 2\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = -EINVAL;
		goto out;
	}

<<<<<<< HEAD
	up->pending = AF_INET6;

do_append_data:
	up->len += ulen;
	getfrag  =  is_udplite ?  udplite_getfrag : ip_generic_getfrag;
	err = ip6_append_data(sk, getfrag, msg->msg_iov, ulen,
		sizeof(struct udphdr), hlimit, tclass, opt, &fl6,
		(struct rt6_info*)dst,
		corkreq ? msg->msg_flags|MSG_MORE : msg->msg_flags, dontfrag);
=======
	WRITE_ONCE(up->pending, AF_INET6);

do_append_data:
	if (ipc6.dontfrag < 0)
		ipc6.dontfrag = inet6_test_bit(DONTFRAG, sk);
	up->len += ulen;
	err = ip6_append_data(sk, getfrag, msg, ulen, sizeof(struct udphdr),
			      &ipc6, fl6, (struct rt6_info *)dst,
			      corkreq ? msg->msg_flags|MSG_MORE : msg->msg_flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err)
		udp_v6_flush_pending_frames(sk);
	else if (!corkreq)
		err = udp_v6_push_pending_frames(sk);
	else if (unlikely(skb_queue_empty(&sk->sk_write_queue)))
<<<<<<< HEAD
		up->pending = 0;

	if (dst) {
		if (connected) {
			ip6_dst_store(sk, dst,
				      ipv6_addr_equal(&fl6.daddr, &np->daddr) ?
				      &np->daddr : NULL,
#ifdef CONFIG_IPV6_SUBTREES
				      ipv6_addr_equal(&fl6.saddr, &np->saddr) ?
				      &np->saddr :
#endif
				      NULL);
		} else {
			dst_release(dst);
		}
		dst = NULL;
	}

	if (err > 0)
		err = np->recverr ? net_xmit_errno(err) : 0;
	release_sock(sk);
out:
	dst_release(dst);
	fl6_sock_release(flowlabel);
=======
		WRITE_ONCE(up->pending, 0);

	if (err > 0)
		err = inet6_test_bit(RECVERR6, sk) ? net_xmit_errno(err) : 0;
	release_sock(sk);

out:
	dst_release(dst);
out_no_dst:
	fl6_sock_release(flowlabel);
	txopt_put(opt_to_free);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!err)
		return len;
	/*
	 * ENOBUFS = no kernel mem, SOCK_NOSPACE = no sndbuf space.  Reporting
	 * ENOBUFS might not be good (it's not tunable per se), but otherwise
	 * we don't have a good statistic (IpOutDiscards but it can be too many
	 * things).  We could add another new stat but at least for now that
	 * seems like overkill.
	 */
	if (err == -ENOBUFS || test_bit(SOCK_NOSPACE, &sk->sk_socket->flags)) {
<<<<<<< HEAD
		UDP6_INC_STATS_USER(sock_net(sk),
				UDP_MIB_SNDBUFERRORS, is_udplite);
=======
		UDP6_INC_STATS(sock_net(sk),
			       UDP_MIB_SNDBUFERRORS, is_udplite);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return err;

do_confirm:
<<<<<<< HEAD
	dst_confirm(dst);
=======
	if (msg->msg_flags & MSG_PROBE)
		dst_confirm_neigh(dst, &fl6->daddr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!(msg->msg_flags&MSG_PROBE) || len)
		goto back_from_confirm;
	err = 0;
	goto out;
}
<<<<<<< HEAD

void udpv6_destroy_sock(struct sock *sk)
{
	lock_sock(sk);
	udp_v6_flush_pending_frames(sk);
	release_sock(sk);

	inet6_destroy_sock(sk);
=======
EXPORT_SYMBOL(udpv6_sendmsg);

static void udpv6_splice_eof(struct socket *sock)
{
	struct sock *sk = sock->sk;
	struct udp_sock *up = udp_sk(sk);

	if (!READ_ONCE(up->pending) || udp_test_bit(CORK, sk))
		return;

	lock_sock(sk);
	if (up->pending && !udp_test_bit(CORK, sk))
		udp_v6_push_pending_frames(sk);
	release_sock(sk);
}

void udpv6_destroy_sock(struct sock *sk)
{
	struct udp_sock *up = udp_sk(sk);
	lock_sock(sk);

	/* protects from races with udp_abort() */
	sock_set_flag(sk, SOCK_DEAD);
	udp_v6_flush_pending_frames(sk);
	release_sock(sk);

	if (static_branch_unlikely(&udpv6_encap_needed_key)) {
		if (up->encap_type) {
			void (*encap_destroy)(struct sock *sk);
			encap_destroy = READ_ONCE(up->encap_destroy);
			if (encap_destroy)
				encap_destroy(sk);
		}
		if (udp_test_bit(ENCAP_ENABLED, sk)) {
			static_branch_dec(&udpv6_encap_needed_key);
			udp_encap_disable();
		}
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 *	Socket option code for UDP
 */
<<<<<<< HEAD
int udpv6_setsockopt(struct sock *sk, int level, int optname,
		     char __user *optval, unsigned int optlen)
{
	if (level == SOL_UDP  ||  level == SOL_UDPLITE)
		return udp_lib_setsockopt(sk, level, optname, optval, optlen,
=======
int udpv6_setsockopt(struct sock *sk, int level, int optname, sockptr_t optval,
		     unsigned int optlen)
{
	if (level == SOL_UDP  ||  level == SOL_UDPLITE || level == SOL_SOCKET)
		return udp_lib_setsockopt(sk, level, optname,
					  optval, optlen,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					  udp_v6_push_pending_frames);
	return ipv6_setsockopt(sk, level, optname, optval, optlen);
}

<<<<<<< HEAD
#ifdef CONFIG_COMPAT
int compat_udpv6_setsockopt(struct sock *sk, int level, int optname,
			    char __user *optval, unsigned int optlen)
{
	if (level == SOL_UDP  ||  level == SOL_UDPLITE)
		return udp_lib_setsockopt(sk, level, optname, optval, optlen,
					  udp_v6_push_pending_frames);
	return compat_ipv6_setsockopt(sk, level, optname, optval, optlen);
}
#endif

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int udpv6_getsockopt(struct sock *sk, int level, int optname,
		     char __user *optval, int __user *optlen)
{
	if (level == SOL_UDP  ||  level == SOL_UDPLITE)
		return udp_lib_getsockopt(sk, level, optname, optval, optlen);
	return ipv6_getsockopt(sk, level, optname, optval, optlen);
}

<<<<<<< HEAD
#ifdef CONFIG_COMPAT
int compat_udpv6_getsockopt(struct sock *sk, int level, int optname,
			    char __user *optval, int __user *optlen)
{
	if (level == SOL_UDP  ||  level == SOL_UDPLITE)
		return udp_lib_getsockopt(sk, level, optname, optval, optlen);
	return compat_ipv6_getsockopt(sk, level, optname, optval, optlen);
}
#endif

static int udp6_ufo_send_check(struct sk_buff *skb)
{
	const struct ipv6hdr *ipv6h;
	struct udphdr *uh;

	if (!pskb_may_pull(skb, sizeof(*uh)))
		return -EINVAL;

	ipv6h = ipv6_hdr(skb);
	uh = udp_hdr(skb);

	uh->check = ~csum_ipv6_magic(&ipv6h->saddr, &ipv6h->daddr, skb->len,
				     IPPROTO_UDP, 0);
	skb->csum_start = skb_transport_header(skb) - skb->head;
	skb->csum_offset = offsetof(struct udphdr, check);
	skb->ip_summed = CHECKSUM_PARTIAL;
	return 0;
}

static struct sk_buff *udp6_ufo_fragment(struct sk_buff *skb,
	netdev_features_t features)
{
	struct sk_buff *segs = ERR_PTR(-EINVAL);
	unsigned int mss;
	unsigned int unfrag_ip6hlen, unfrag_len;
	struct frag_hdr *fptr;
	u8 *mac_start, *prevhdr;
	u8 nexthdr;
	u8 frag_hdr_sz = sizeof(struct frag_hdr);
	int offset;
	__wsum csum;
	int err;

	mss = skb_shinfo(skb)->gso_size;
	if (unlikely(skb->len <= mss))
		goto out;

	if (skb_gso_ok(skb, features | NETIF_F_GSO_ROBUST)) {
		/* Packet is from an untrusted source, reset gso_segs. */
		int type = skb_shinfo(skb)->gso_type;

		if (unlikely(type & ~(SKB_GSO_UDP | SKB_GSO_DODGY) ||
			     !(type & (SKB_GSO_UDP))))
			goto out;

		skb_shinfo(skb)->gso_segs = DIV_ROUND_UP(skb->len, mss);

		segs = NULL;
		goto out;
	}

	/* Do software UFO. Complete and fill in the UDP checksum as HW cannot
	 * do checksum of UDP packets sent as multiple IP fragments.
	 */
	offset = skb_checksum_start_offset(skb);
	csum = skb_checksum(skb, offset, skb->len- offset, 0);
	offset += skb->csum_offset;
	*(__sum16 *)(skb->data + offset) = csum_fold(csum);
	skb->ip_summed = CHECKSUM_NONE;

	/* Check if there is enough headroom to insert fragment header. */
	if ((skb_mac_header(skb) < skb->head + frag_hdr_sz) &&
	    pskb_expand_head(skb, frag_hdr_sz, 0, GFP_ATOMIC))
		goto out;

	/* Find the unfragmentable header and shift it left by frag_hdr_sz
	 * bytes to insert fragment header.
	 */
	err = ip6_find_1stfragopt(skb, &prevhdr);
	if (err < 0)
		return ERR_PTR(err);
	unfrag_ip6hlen = err;
	nexthdr = *prevhdr;
	*prevhdr = NEXTHDR_FRAGMENT;
	unfrag_len = skb_network_header(skb) - skb_mac_header(skb) +
		     unfrag_ip6hlen;
	mac_start = skb_mac_header(skb);
	memmove(mac_start-frag_hdr_sz, mac_start, unfrag_len);

	skb->mac_header -= frag_hdr_sz;
	skb->network_header -= frag_hdr_sz;

	fptr = (struct frag_hdr *)(skb_network_header(skb) + unfrag_ip6hlen);
	fptr->nexthdr = nexthdr;
	fptr->reserved = 0;
	fptr->identification = skb_shinfo(skb)->ip6_frag_id;

	/* Fragment the skb. ipv6 header and the remaining fields of the
	 * fragment header are updated in ipv6_gso_segment()
	 */
	segs = skb_segment(skb, features);

out:
	return segs;
}

static const struct inet6_protocol udpv6_protocol = {
	.handler	=	udpv6_rcv,
	.err_handler	=	udpv6_err,
	.gso_send_check =	udp6_ufo_send_check,
	.gso_segment	=	udp6_ufo_fragment,
	.flags		=	INET6_PROTO_NOPOLICY|INET6_PROTO_FINAL,
};

/* ------------------------------------------------------------------------ */
#ifdef CONFIG_PROC_FS

static void udp6_sock_seq_show(struct seq_file *seq, struct sock *sp, int bucket)
{
	struct inet_sock *inet = inet_sk(sp);
	struct ipv6_pinfo *np = inet6_sk(sp);
	const struct in6_addr *dest, *src;
	__u16 destp, srcp;

	dest  = &np->daddr;
	src   = &np->rcv_saddr;
	destp = ntohs(inet->inet_dport);
	srcp  = ntohs(inet->inet_sport);
	seq_printf(seq,
		   "%5d: %08X%08X%08X%08X:%04X %08X%08X%08X%08X:%04X "
		   "%02X %08X:%08X %02X:%08lX %08X %5d %8d %lu %d %pK %d\n",
		   bucket,
		   src->s6_addr32[0], src->s6_addr32[1],
		   src->s6_addr32[2], src->s6_addr32[3], srcp,
		   dest->s6_addr32[0], dest->s6_addr32[1],
		   dest->s6_addr32[2], dest->s6_addr32[3], destp,
		   sp->sk_state,
		   sk_wmem_alloc_get(sp),
		   sk_rmem_alloc_get(sp),
		   0, 0L, 0,
		   sock_i_uid(sp), 0,
		   sock_i_ino(sp),
		   atomic_read(&sp->sk_refcnt), sp,
		   atomic_read(&sp->sk_drops));
}

int udp6_seq_show(struct seq_file *seq, void *v)
{
	if (v == SEQ_START_TOKEN)
		seq_printf(seq,
			   "  sl  "
			   "local_address                         "
			   "remote_address                        "
			   "st tx_queue rx_queue tr tm->when retrnsmt"
			   "   uid  timeout inode ref pointer drops\n");
	else
		udp6_sock_seq_show(seq, v, ((struct udp_iter_state *)seq->private)->bucket);
	return 0;
}

static const struct file_operations udp6_afinfo_seq_fops = {
	.owner    = THIS_MODULE,
	.open     = udp_seq_open,
	.read     = seq_read,
	.llseek   = seq_lseek,
	.release  = seq_release_net
};

static struct udp_seq_afinfo udp6_seq_afinfo = {
	.name		= "udp6",
	.family		= AF_INET6,
	.udp_table	= &udp_table,
	.seq_fops	= &udp6_afinfo_seq_fops,
	.seq_ops	= {
		.show		= udp6_seq_show,
	},
=======

/* ------------------------------------------------------------------------ */
#ifdef CONFIG_PROC_FS
int udp6_seq_show(struct seq_file *seq, void *v)
{
	if (v == SEQ_START_TOKEN) {
		seq_puts(seq, IPV6_SEQ_DGRAM_HEADER);
	} else {
		int bucket = ((struct udp_iter_state *)seq->private)->bucket;
		const struct inet_sock *inet = inet_sk((const struct sock *)v);
		__u16 srcp = ntohs(inet->inet_sport);
		__u16 destp = ntohs(inet->inet_dport);
		__ip6_dgram_sock_seq_show(seq, v, srcp, destp,
					  udp_rqueue_get(v), bucket);
	}
	return 0;
}

const struct seq_operations udp6_seq_ops = {
	.start		= udp_seq_start,
	.next		= udp_seq_next,
	.stop		= udp_seq_stop,
	.show		= udp6_seq_show,
};
EXPORT_SYMBOL(udp6_seq_ops);

static struct udp_seq_afinfo udp6_seq_afinfo = {
	.family		= AF_INET6,
	.udp_table	= NULL,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

int __net_init udp6_proc_init(struct net *net)
{
<<<<<<< HEAD
	return udp_proc_register(net, &udp6_seq_afinfo);
}

void udp6_proc_exit(struct net *net) {
	udp_proc_unregister(net, &udp6_seq_afinfo);
}
#endif /* CONFIG_PROC_FS */

void udp_v6_clear_sk(struct sock *sk, int size)
{
	struct inet_sock *inet = inet_sk(sk);

	/* we do not want to clear pinet6 field, because of RCU lookups */
	sk_prot_clear_portaddr_nulls(sk, offsetof(struct inet_sock, pinet6));

	size -= offsetof(struct inet_sock, pinet6) + sizeof(inet->pinet6);
	memset(&inet->pinet6 + 1, 0, size);
}

/* ------------------------------------------------------------------------ */

struct proto udpv6_prot = {
	.name		   = "UDPv6",
	.owner		   = THIS_MODULE,
	.close		   = udp_lib_close,
	.connect	   = ip6_datagram_connect,
	.disconnect	   = udp_disconnect,
	.ioctl		   = udp_ioctl,
	.destroy	   = udpv6_destroy_sock,
	.setsockopt	   = udpv6_setsockopt,
	.getsockopt	   = udpv6_getsockopt,
	.sendmsg	   = udpv6_sendmsg,
	.recvmsg	   = udpv6_recvmsg,
	.backlog_rcv	   = udpv6_queue_rcv_skb,
	.hash		   = udp_lib_hash,
	.unhash		   = udp_lib_unhash,
	.rehash		   = udp_v6_rehash,
	.get_port	   = udp_v6_get_port,
	.memory_allocated  = &udp_memory_allocated,
	.sysctl_mem	   = sysctl_udp_mem,
	.sysctl_wmem	   = &sysctl_udp_wmem_min,
	.sysctl_rmem	   = &sysctl_udp_rmem_min,
	.obj_size	   = sizeof(struct udp6_sock),
	.slab_flags	   = SLAB_DESTROY_BY_RCU,
	.h.udp_table	   = &udp_table,
#ifdef CONFIG_COMPAT
	.compat_setsockopt = compat_udpv6_setsockopt,
	.compat_getsockopt = compat_udpv6_getsockopt,
#endif
	.clear_sk	   = udp_v6_clear_sk,
=======
	if (!proc_create_net_data("udp6", 0444, net->proc_net, &udp6_seq_ops,
			sizeof(struct udp_iter_state), &udp6_seq_afinfo))
		return -ENOMEM;
	return 0;
}

void udp6_proc_exit(struct net *net)
{
	remove_proc_entry("udp6", net->proc_net);
}
#endif /* CONFIG_PROC_FS */

/* ------------------------------------------------------------------------ */

struct proto udpv6_prot = {
	.name			= "UDPv6",
	.owner			= THIS_MODULE,
	.close			= udp_lib_close,
	.pre_connect		= udpv6_pre_connect,
	.connect		= ip6_datagram_connect,
	.disconnect		= udp_disconnect,
	.ioctl			= udp_ioctl,
	.init			= udpv6_init_sock,
	.destroy		= udpv6_destroy_sock,
	.setsockopt		= udpv6_setsockopt,
	.getsockopt		= udpv6_getsockopt,
	.sendmsg		= udpv6_sendmsg,
	.recvmsg		= udpv6_recvmsg,
	.splice_eof		= udpv6_splice_eof,
	.release_cb		= ip6_datagram_release_cb,
	.hash			= udp_lib_hash,
	.unhash			= udp_lib_unhash,
	.rehash			= udp_v6_rehash,
	.get_port		= udp_v6_get_port,
	.put_port		= udp_lib_unhash,
#ifdef CONFIG_BPF_SYSCALL
	.psock_update_sk_prot	= udp_bpf_update_proto,
#endif

	.memory_allocated	= &udp_memory_allocated,
	.per_cpu_fw_alloc	= &udp_memory_per_cpu_fw_alloc,

	.sysctl_mem		= sysctl_udp_mem,
	.sysctl_wmem_offset     = offsetof(struct net, ipv4.sysctl_udp_wmem_min),
	.sysctl_rmem_offset     = offsetof(struct net, ipv4.sysctl_udp_rmem_min),
	.obj_size		= sizeof(struct udp6_sock),
	.ipv6_pinfo_offset = offsetof(struct udp6_sock, inet6),
	.h.udp_table		= NULL,
	.diag_destroy		= udp_abort,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static struct inet_protosw udpv6_protosw = {
	.type =      SOCK_DGRAM,
	.protocol =  IPPROTO_UDP,
	.prot =      &udpv6_prot,
	.ops =       &inet6_dgram_ops,
<<<<<<< HEAD
	.no_check =  UDP_CSUM_DEFAULT,
	.flags =     INET_PROTOSW_PERMANENT,
};


=======
	.flags =     INET_PROTOSW_PERMANENT,
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int __init udpv6_init(void)
{
	int ret;

<<<<<<< HEAD
	ret = inet6_add_protocol(&udpv6_protocol, IPPROTO_UDP);
=======
	net_hotdata.udpv6_protocol = (struct inet6_protocol) {
		.handler     = udpv6_rcv,
		.err_handler = udpv6_err,
		.flags	     = INET6_PROTO_NOPOLICY | INET6_PROTO_FINAL,
	};
	ret = inet6_add_protocol(&net_hotdata.udpv6_protocol, IPPROTO_UDP);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ret)
		goto out;

	ret = inet6_register_protosw(&udpv6_protosw);
	if (ret)
		goto out_udpv6_protocol;
out:
	return ret;

out_udpv6_protocol:
<<<<<<< HEAD
	inet6_del_protocol(&udpv6_protocol, IPPROTO_UDP);
=======
	inet6_del_protocol(&net_hotdata.udpv6_protocol, IPPROTO_UDP);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	goto out;
}

void udpv6_exit(void)
{
	inet6_unregister_protosw(&udpv6_protosw);
<<<<<<< HEAD
	inet6_del_protocol(&udpv6_protocol, IPPROTO_UDP);
=======
	inet6_del_protocol(&net_hotdata.udpv6_protocol, IPPROTO_UDP);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
