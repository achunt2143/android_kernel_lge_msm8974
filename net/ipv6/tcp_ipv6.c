<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *	TCP over IPv6
 *	Linux INET6 implementation
 *
 *	Authors:
 *	Pedro Roque		<roque@di.fc.ul.pt>
 *
 *	Based on:
 *	linux/net/ipv4/tcp.c
 *	linux/net/ipv4/tcp_input.c
 *	linux/net/ipv4/tcp_output.c
 *
 *	Fixes:
 *	Hideaki YOSHIFUJI	:	sin6_scope_id support
 *	YOSHIFUJI Hideaki @USAGI and:	Support IPV6_V6ONLY socket option, which
 *	Alexey Kuznetsov		allow both IPv4 and IPv6 sockets to bind
 *					a single port at the same time.
 *	YOSHIFUJI Hideaki @USAGI:	convert /proc/net/tcp6 to seq_file.
<<<<<<< HEAD
 *
 *	This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/bottom_half.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/socket.h>
#include <linux/sockios.h>
#include <linux/net.h>
#include <linux/jiffies.h>
#include <linux/in.h>
#include <linux/in6.h>
#include <linux/netdevice.h>
#include <linux/init.h>
#include <linux/jhash.h>
#include <linux/ipsec.h>
#include <linux/times.h>
#include <linux/slab.h>
<<<<<<< HEAD

#include <linux/ipv6.h>
#include <linux/icmpv6.h>
#include <linux/random.h>
=======
#include <linux/uaccess.h>
#include <linux/ipv6.h>
#include <linux/icmpv6.h>
#include <linux/random.h>
#include <linux/indirect_call_wrapper.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <net/tcp.h>
#include <net/ndisc.h>
#include <net/inet6_hashtables.h>
#include <net/inet6_connection_sock.h>
#include <net/ipv6.h>
#include <net/transp_v6.h>
#include <net/addrconf.h>
#include <net/ip6_route.h>
#include <net/ip6_checksum.h>
#include <net/inet_ecn.h>
#include <net/protocol.h>
#include <net/xfrm.h>
#include <net/snmp.h>
#include <net/dsfield.h>
#include <net/timewait_sock.h>
<<<<<<< HEAD
#include <net/netdma.h>
#include <net/inet_common.h>
#include <net/secure_seq.h>
#include <net/tcp_memcontrol.h>

#include <asm/uaccess.h>
=======
#include <net/inet_common.h>
#include <net/secure_seq.h>
#include <net/hotdata.h>
#include <net/busy_poll.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <linux/proc_fs.h>
#include <linux/seq_file.h>

<<<<<<< HEAD
#include <linux/crypto.h>
#include <linux/scatterlist.h>

static void	tcp_v6_send_reset(struct sock *sk, struct sk_buff *skb);
static void	tcp_v6_reqsk_send_ack(struct sock *sk, struct sk_buff *skb,
				      struct request_sock *req);

static int	tcp_v6_do_rcv(struct sock *sk, struct sk_buff *skb);
static void	__tcp_v6_send_check(struct sk_buff *skb,
				    const struct in6_addr *saddr,
				    const struct in6_addr *daddr);

static const struct inet_connection_sock_af_ops ipv6_mapped;
static const struct inet_connection_sock_af_ops ipv6_specific;
#ifdef CONFIG_TCP_MD5SIG
static const struct tcp_sock_af_ops tcp_sock_ipv6_specific;
static const struct tcp_sock_af_ops tcp_sock_ipv6_mapped_specific;
#else
static struct tcp_md5sig_key *tcp_v6_md5_do_lookup(struct sock *sk,
						   const struct in6_addr *addr)
{
	return NULL;
}
#endif

static void tcp_v6_hash(struct sock *sk)
{
	if (sk->sk_state != TCP_CLOSE) {
		if (inet_csk(sk)->icsk_af_ops == &ipv6_mapped) {
			tcp_prot.hash(sk);
			return;
		}
		local_bh_disable();
		__inet6_hash(sk, NULL);
		local_bh_enable();
	}
}

static __inline__ __sum16 tcp_v6_check(int len,
				   const struct in6_addr *saddr,
				   const struct in6_addr *daddr,
				   __wsum base)
{
	return csum_ipv6_magic(saddr, daddr, len, IPPROTO_TCP, base);
}

static __u32 tcp_v6_init_sequence(const struct sk_buff *skb)
{
	return secure_tcpv6_sequence_number(ipv6_hdr(skb)->daddr.s6_addr32,
					    ipv6_hdr(skb)->saddr.s6_addr32,
					    tcp_hdr(skb)->dest,
					    tcp_hdr(skb)->source);
=======
#include <crypto/hash.h>
#include <linux/scatterlist.h>

#include <trace/events/tcp.h>

static void	tcp_v6_send_reset(const struct sock *sk, struct sk_buff *skb);
static void	tcp_v6_reqsk_send_ack(const struct sock *sk, struct sk_buff *skb,
				      struct request_sock *req);

INDIRECT_CALLABLE_SCOPE int tcp_v6_do_rcv(struct sock *sk, struct sk_buff *skb);

static const struct inet_connection_sock_af_ops ipv6_mapped;
const struct inet_connection_sock_af_ops ipv6_specific;
#if defined(CONFIG_TCP_MD5SIG) || defined(CONFIG_TCP_AO)
static const struct tcp_sock_af_ops tcp_sock_ipv6_specific;
static const struct tcp_sock_af_ops tcp_sock_ipv6_mapped_specific;
#endif

/* Helper returning the inet6 address from a given tcp socket.
 * It can be used in TCP stack instead of inet6_sk(sk).
 * This avoids a dereference and allow compiler optimizations.
 * It is a specialized version of inet6_sk_generic().
 */
#define tcp_inet6_sk(sk) (&container_of_const(tcp_sk(sk), \
					      struct tcp6_sock, tcp)->inet6)

static void inet6_sk_rx_dst_set(struct sock *sk, const struct sk_buff *skb)
{
	struct dst_entry *dst = skb_dst(skb);

	if (dst && dst_hold_safe(dst)) {
		const struct rt6_info *rt = (const struct rt6_info *)dst;

		rcu_assign_pointer(sk->sk_rx_dst, dst);
		sk->sk_rx_dst_ifindex = skb->skb_iif;
		sk->sk_rx_dst_cookie = rt6_get_cookie(rt);
	}
}

static u32 tcp_v6_init_seq(const struct sk_buff *skb)
{
	return secure_tcpv6_seq(ipv6_hdr(skb)->daddr.s6_addr32,
				ipv6_hdr(skb)->saddr.s6_addr32,
				tcp_hdr(skb)->dest,
				tcp_hdr(skb)->source);
}

static u32 tcp_v6_init_ts_off(const struct net *net, const struct sk_buff *skb)
{
	return secure_tcpv6_ts_off(net, ipv6_hdr(skb)->daddr.s6_addr32,
				   ipv6_hdr(skb)->saddr.s6_addr32);
}

static int tcp_v6_pre_connect(struct sock *sk, struct sockaddr *uaddr,
			      int addr_len)
{
	/* This check is replicated from tcp_v6_connect() and intended to
	 * prevent BPF program called below from accessing bytes that are out
	 * of the bound specified by user in addr_len.
	 */
	if (addr_len < SIN6_LEN_RFC2133)
		return -EINVAL;

	sock_owned_by_me(sk);

	return BPF_CGROUP_RUN_PROG_INET6_CONNECT(sk, uaddr, &addr_len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int tcp_v6_connect(struct sock *sk, struct sockaddr *uaddr,
			  int addr_len)
{
	struct sockaddr_in6 *usin = (struct sockaddr_in6 *) uaddr;
<<<<<<< HEAD
	struct inet_sock *inet = inet_sk(sk);
	struct inet_connection_sock *icsk = inet_csk(sk);
	struct ipv6_pinfo *np = inet6_sk(sk);
	struct tcp_sock *tp = tcp_sk(sk);
	struct in6_addr *saddr = NULL, *final_p, final;
	struct rt6_info *rt;
	struct flowi6 fl6;
	struct dst_entry *dst;
=======
	struct inet_connection_sock *icsk = inet_csk(sk);
	struct in6_addr *saddr = NULL, *final_p, final;
	struct inet_timewait_death_row *tcp_death_row;
	struct ipv6_pinfo *np = tcp_inet6_sk(sk);
	struct inet_sock *inet = inet_sk(sk);
	struct tcp_sock *tp = tcp_sk(sk);
	struct net *net = sock_net(sk);
	struct ipv6_txoptions *opt;
	struct dst_entry *dst;
	struct flowi6 fl6;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int addr_type;
	int err;

	if (addr_len < SIN6_LEN_RFC2133)
		return -EINVAL;

	if (usin->sin6_family != AF_INET6)
		return -EAFNOSUPPORT;

	memset(&fl6, 0, sizeof(fl6));

<<<<<<< HEAD
	if (np->sndflow) {
=======
	if (inet6_test_bit(SNDFLOW, sk)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		fl6.flowlabel = usin->sin6_flowinfo&IPV6_FLOWINFO_MASK;
		IP6_ECN_flow_init(fl6.flowlabel);
		if (fl6.flowlabel&IPV6_FLOWLABEL_MASK) {
			struct ip6_flowlabel *flowlabel;
			flowlabel = fl6_sock_lookup(sk, fl6.flowlabel);
<<<<<<< HEAD
			if (flowlabel == NULL)
				return -EINVAL;
			usin->sin6_addr = flowlabel->dst;
=======
			if (IS_ERR(flowlabel))
				return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			fl6_sock_release(flowlabel);
		}
	}

	/*
	 *	connect() to INADDR_ANY means loopback (BSD'ism).
	 */

<<<<<<< HEAD
	if(ipv6_addr_any(&usin->sin6_addr))
		usin->sin6_addr.s6_addr[15] = 0x1;

	addr_type = ipv6_addr_type(&usin->sin6_addr);

	if(addr_type & IPV6_ADDR_MULTICAST)
=======
	if (ipv6_addr_any(&usin->sin6_addr)) {
		if (ipv6_addr_v4mapped(&sk->sk_v6_rcv_saddr))
			ipv6_addr_set_v4mapped(htonl(INADDR_LOOPBACK),
					       &usin->sin6_addr);
		else
			usin->sin6_addr = in6addr_loopback;
	}

	addr_type = ipv6_addr_type(&usin->sin6_addr);

	if (addr_type & IPV6_ADDR_MULTICAST)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ENETUNREACH;

	if (addr_type&IPV6_ADDR_LINKLOCAL) {
		if (addr_len >= sizeof(struct sockaddr_in6) &&
		    usin->sin6_scope_id) {
			/* If interface is set while binding, indices
			 * must coincide.
			 */
<<<<<<< HEAD
			if (sk->sk_bound_dev_if &&
			    sk->sk_bound_dev_if != usin->sin6_scope_id)
=======
			if (!sk_dev_equal_l3scope(sk, usin->sin6_scope_id))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				return -EINVAL;

			sk->sk_bound_dev_if = usin->sin6_scope_id;
		}

		/* Connect to link-local address requires an interface */
		if (!sk->sk_bound_dev_if)
			return -EINVAL;
	}

	if (tp->rx_opt.ts_recent_stamp &&
<<<<<<< HEAD
	    !ipv6_addr_equal(&np->daddr, &usin->sin6_addr)) {
		tp->rx_opt.ts_recent = 0;
		tp->rx_opt.ts_recent_stamp = 0;
		tp->write_seq = 0;
	}

	np->daddr = usin->sin6_addr;
=======
	    !ipv6_addr_equal(&sk->sk_v6_daddr, &usin->sin6_addr)) {
		tp->rx_opt.ts_recent = 0;
		tp->rx_opt.ts_recent_stamp = 0;
		WRITE_ONCE(tp->write_seq, 0);
	}

	sk->sk_v6_daddr = usin->sin6_addr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	np->flow_label = fl6.flowlabel;

	/*
	 *	TCP over IPv4
	 */

<<<<<<< HEAD
	if (addr_type == IPV6_ADDR_MAPPED) {
		u32 exthdrlen = icsk->icsk_ext_hdr_len;
		struct sockaddr_in sin;

		SOCK_DEBUG(sk, "connect: ipv4 mapped\n");

		if (__ipv6_only_sock(sk))
=======
	if (addr_type & IPV6_ADDR_MAPPED) {
		u32 exthdrlen = icsk->icsk_ext_hdr_len;
		struct sockaddr_in sin;

		if (ipv6_only_sock(sk))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -ENETUNREACH;

		sin.sin_family = AF_INET;
		sin.sin_port = usin->sin6_port;
		sin.sin_addr.s_addr = usin->sin6_addr.s6_addr32[3];

<<<<<<< HEAD
		icsk->icsk_af_ops = &ipv6_mapped;
		sk->sk_backlog_rcv = tcp_v4_do_rcv;
#ifdef CONFIG_TCP_MD5SIG
=======
		/* Paired with READ_ONCE() in tcp_(get|set)sockopt() */
		WRITE_ONCE(icsk->icsk_af_ops, &ipv6_mapped);
		if (sk_is_mptcp(sk))
			mptcpv6_handle_mapped(sk, true);
		sk->sk_backlog_rcv = tcp_v4_do_rcv;
#if defined(CONFIG_TCP_MD5SIG) || defined(CONFIG_TCP_AO)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		tp->af_specific = &tcp_sock_ipv6_mapped_specific;
#endif

		err = tcp_v4_connect(sk, (struct sockaddr *)&sin, sizeof(sin));

		if (err) {
			icsk->icsk_ext_hdr_len = exthdrlen;
<<<<<<< HEAD
			icsk->icsk_af_ops = &ipv6_specific;
			sk->sk_backlog_rcv = tcp_v6_do_rcv;
#ifdef CONFIG_TCP_MD5SIG
			tp->af_specific = &tcp_sock_ipv6_specific;
#endif
			goto failure;
		} else {
			ipv6_addr_set_v4mapped(inet->inet_saddr, &np->saddr);
			ipv6_addr_set_v4mapped(inet->inet_rcv_saddr,
					       &np->rcv_saddr);
		}
=======
			/* Paired with READ_ONCE() in tcp_(get|set)sockopt() */
			WRITE_ONCE(icsk->icsk_af_ops, &ipv6_specific);
			if (sk_is_mptcp(sk))
				mptcpv6_handle_mapped(sk, false);
			sk->sk_backlog_rcv = tcp_v6_do_rcv;
#if defined(CONFIG_TCP_MD5SIG) || defined(CONFIG_TCP_AO)
			tp->af_specific = &tcp_sock_ipv6_specific;
#endif
			goto failure;
		}
		np->saddr = sk->sk_v6_rcv_saddr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		return err;
	}

<<<<<<< HEAD
	if (!ipv6_addr_any(&np->rcv_saddr))
		saddr = &np->rcv_saddr;

	fl6.flowi6_proto = IPPROTO_TCP;
	fl6.daddr = np->daddr;
	fl6.saddr = saddr ? *saddr : np->saddr;
=======
	if (!ipv6_addr_any(&sk->sk_v6_rcv_saddr))
		saddr = &sk->sk_v6_rcv_saddr;

	fl6.flowi6_proto = IPPROTO_TCP;
	fl6.daddr = sk->sk_v6_daddr;
	fl6.saddr = saddr ? *saddr : np->saddr;
	fl6.flowlabel = ip6_make_flowinfo(np->tclass, np->flow_label);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	fl6.flowi6_oif = sk->sk_bound_dev_if;
	fl6.flowi6_mark = sk->sk_mark;
	fl6.fl6_dport = usin->sin6_port;
	fl6.fl6_sport = inet->inet_sport;
	fl6.flowi6_uid = sk->sk_uid;

<<<<<<< HEAD
	final_p = fl6_update_dst(&fl6, np->opt, &final);

	security_sk_classify_flow(sk, flowi6_to_flowi(&fl6));

	dst = ip6_dst_lookup_flow(sk, &fl6, final_p, true);
=======
	opt = rcu_dereference_protected(np->opt, lockdep_sock_is_held(sk));
	final_p = fl6_update_dst(&fl6, opt, &final);

	security_sk_classify_flow(sk, flowi6_to_flowi_common(&fl6));

	dst = ip6_dst_lookup_flow(net, sk, &fl6, final_p);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (IS_ERR(dst)) {
		err = PTR_ERR(dst);
		goto failure;
	}

<<<<<<< HEAD
	if (saddr == NULL) {
		saddr = &fl6.saddr;
		np->rcv_saddr = *saddr;
=======
	tp->tcp_usec_ts = dst_tcp_usec_ts(dst);
	tcp_death_row = &sock_net(sk)->ipv4.tcp_death_row;

	if (!saddr) {
		saddr = &fl6.saddr;

		err = inet_bhash2_update_saddr(sk, saddr, AF_INET6);
		if (err)
			goto failure;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* set the source address */
	np->saddr = *saddr;
	inet->inet_rcv_saddr = LOOPBACK4_IPV6;

	sk->sk_gso_type = SKB_GSO_TCPV6;
<<<<<<< HEAD
	__ip6_dst_store(sk, dst, NULL, NULL);

	rt = (struct rt6_info *) dst;
	if (tcp_death_row.sysctl_tw_recycle &&
	    !tp->rx_opt.ts_recent_stamp &&
	    ipv6_addr_equal(&rt->rt6i_dst.addr, &np->daddr)) {
		struct inet_peer *peer = rt6_get_peer(rt);
		/*
		 * VJ's idea. We save last timestamp seen from
		 * the destination in peer table, when entering state
		 * TIME-WAIT * and initialize rx_opt.ts_recent from it,
		 * when trying new connection.
		 */
		if (peer) {
			inet_peer_refcheck(peer);
			if ((u32)get_seconds() - peer->tcp_ts_stamp <= TCP_PAWS_MSL) {
				tp->rx_opt.ts_recent_stamp = peer->tcp_ts_stamp;
				tp->rx_opt.ts_recent = peer->tcp_ts;
			}
		}
	}

	icsk->icsk_ext_hdr_len = 0;
	if (np->opt)
		icsk->icsk_ext_hdr_len = (np->opt->opt_flen +
					  np->opt->opt_nflen);
=======
	ip6_dst_store(sk, dst, NULL, NULL);

	icsk->icsk_ext_hdr_len = 0;
	if (opt)
		icsk->icsk_ext_hdr_len = opt->opt_flen +
					 opt->opt_nflen;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	tp->rx_opt.mss_clamp = IPV6_MIN_MTU - sizeof(struct tcphdr) - sizeof(struct ipv6hdr);

	inet->inet_dport = usin->sin6_port;

	tcp_set_state(sk, TCP_SYN_SENT);
<<<<<<< HEAD
	err = inet6_hash_connect(&tcp_death_row, sk);
	if (err)
		goto late_failure;

	if (!tp->write_seq)
		tp->write_seq = secure_tcpv6_sequence_number(np->saddr.s6_addr32,
							     np->daddr.s6_addr32,
							     inet->inet_sport,
							     inet->inet_dport);
=======
	err = inet6_hash_connect(tcp_death_row, sk);
	if (err)
		goto late_failure;

	sk_set_txhash(sk);

	if (likely(!tp->repair)) {
		if (!tp->write_seq)
			WRITE_ONCE(tp->write_seq,
				   secure_tcpv6_seq(np->saddr.s6_addr32,
						    sk->sk_v6_daddr.s6_addr32,
						    inet->inet_sport,
						    inet->inet_dport));
		tp->tsoffset = secure_tcpv6_ts_off(net, np->saddr.s6_addr32,
						   sk->sk_v6_daddr.s6_addr32);
	}

	if (tcp_fastopen_defer_connect(sk, &err))
		return err;
	if (err)
		goto late_failure;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	err = tcp_connect(sk);
	if (err)
		goto late_failure;

	return 0;

late_failure:
	tcp_set_state(sk, TCP_CLOSE);
<<<<<<< HEAD
	__sk_dst_reset(sk);
=======
	inet_bhash2_reset_saddr(sk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
failure:
	inet->inet_dport = 0;
	sk->sk_route_caps = 0;
	return err;
}

<<<<<<< HEAD
static void tcp_v6_err(struct sk_buff *skb, struct inet6_skb_parm *opt,
		u8 type, u8 code, int offset, __be32 info)
{
	const struct ipv6hdr *hdr = (const struct ipv6hdr*)skb->data;
	const struct tcphdr *th = (struct tcphdr *)(skb->data+offset);
	struct ipv6_pinfo *np;
	struct sock *sk;
	int err;
	struct tcp_sock *tp;
	__u32 seq;
	struct net *net = dev_net(skb->dev);

	sk = inet6_lookup(net, &tcp_hashinfo, &hdr->daddr,
			th->dest, &hdr->saddr, th->source, skb->dev->ifindex);

	if (sk == NULL) {
		ICMP6_INC_STATS_BH(net, __in6_dev_get(skb->dev),
				   ICMP6_MIB_INERRORS);
		return;
	}

	if (sk->sk_state == TCP_TIME_WAIT) {
		inet_twsk_put(inet_twsk(sk));
		return;
	}

	bh_lock_sock(sk);
	if (sock_owned_by_user(sk))
		NET_INC_STATS_BH(net, LINUX_MIB_LOCKDROPPEDICMPS);
=======
static void tcp_v6_mtu_reduced(struct sock *sk)
{
	struct dst_entry *dst;
	u32 mtu;

	if ((1 << sk->sk_state) & (TCPF_LISTEN | TCPF_CLOSE))
		return;

	mtu = READ_ONCE(tcp_sk(sk)->mtu_info);

	/* Drop requests trying to increase our current mss.
	 * Check done in __ip6_rt_update_pmtu() is too late.
	 */
	if (tcp_mtu_to_mss(sk, mtu) >= tcp_sk(sk)->mss_cache)
		return;

	dst = inet6_csk_update_pmtu(sk, mtu);
	if (!dst)
		return;

	if (inet_csk(sk)->icsk_pmtu_cookie > dst_mtu(dst)) {
		tcp_sync_mss(sk, dst_mtu(dst));
		tcp_simple_retransmit(sk);
	}
}

static int tcp_v6_err(struct sk_buff *skb, struct inet6_skb_parm *opt,
		u8 type, u8 code, int offset, __be32 info)
{
	const struct ipv6hdr *hdr = (const struct ipv6hdr *)skb->data;
	const struct tcphdr *th = (struct tcphdr *)(skb->data+offset);
	struct net *net = dev_net(skb->dev);
	struct request_sock *fastopen;
	struct ipv6_pinfo *np;
	struct tcp_sock *tp;
	__u32 seq, snd_una;
	struct sock *sk;
	bool fatal;
	int err;

	sk = __inet6_lookup_established(net, net->ipv4.tcp_death_row.hashinfo,
					&hdr->daddr, th->dest,
					&hdr->saddr, ntohs(th->source),
					skb->dev->ifindex, inet6_sdif(skb));

	if (!sk) {
		__ICMP6_INC_STATS(net, __in6_dev_get(skb->dev),
				  ICMP6_MIB_INERRORS);
		return -ENOENT;
	}

	if (sk->sk_state == TCP_TIME_WAIT) {
		/* To increase the counter of ignored icmps for TCP-AO */
		tcp_ao_ignore_icmp(sk, AF_INET6, type, code);
		inet_twsk_put(inet_twsk(sk));
		return 0;
	}
	seq = ntohl(th->seq);
	fatal = icmpv6_err_convert(type, code, &err);
	if (sk->sk_state == TCP_NEW_SYN_RECV) {
		tcp_req_err(sk, seq, fatal);
		return 0;
	}

	if (tcp_ao_ignore_icmp(sk, AF_INET6, type, code)) {
		sock_put(sk);
		return 0;
	}

	bh_lock_sock(sk);
	if (sock_owned_by_user(sk) && type != ICMPV6_PKT_TOOBIG)
		__NET_INC_STATS(net, LINUX_MIB_LOCKDROPPEDICMPS);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (sk->sk_state == TCP_CLOSE)
		goto out;

<<<<<<< HEAD
	if (ipv6_hdr(skb)->hop_limit < inet6_sk(sk)->min_hopcount) {
		NET_INC_STATS_BH(net, LINUX_MIB_TCPMINTTLDROP);
		goto out;
	}

	tp = tcp_sk(sk);
	seq = ntohl(th->seq);
	if (sk->sk_state != TCP_LISTEN &&
	    !between(seq, tp->snd_una, tp->snd_nxt)) {
		NET_INC_STATS_BH(net, LINUX_MIB_OUTOFWINDOWICMPS);
		goto out;
	}

	np = inet6_sk(sk);

	if (type == ICMPV6_PKT_TOOBIG) {
		struct dst_entry *dst;

		if (sock_owned_by_user(sk))
			goto out;
		if ((1 << sk->sk_state) & (TCPF_LISTEN | TCPF_CLOSE))
			goto out;

		/* icmp should have updated the destination cache entry */
		dst = __sk_dst_check(sk, np->dst_cookie);

		if (dst == NULL) {
			struct inet_sock *inet = inet_sk(sk);
			struct flowi6 fl6;

			/* BUGGG_FUTURE: Again, it is not clear how
			   to handle rthdr case. Ignore this complexity
			   for now.
			 */
			memset(&fl6, 0, sizeof(fl6));
			fl6.flowi6_proto = IPPROTO_TCP;
			fl6.daddr = np->daddr;
			fl6.saddr = np->saddr;
			fl6.flowi6_oif = sk->sk_bound_dev_if;
			fl6.flowi6_mark = sk->sk_mark;
			fl6.fl6_dport = inet->inet_dport;
			fl6.fl6_sport = inet->inet_sport;
			fl6.flowi6_uid = sk->sk_uid;
			security_skb_classify_flow(skb, flowi6_to_flowi(&fl6));

			dst = ip6_dst_lookup_flow(sk, &fl6, NULL, false);
			if (IS_ERR(dst)) {
				sk->sk_err_soft = -PTR_ERR(dst);
				goto out;
			}

		} else
			dst_hold(dst);

		dst->ops->update_pmtu(dst, ntohl(info));

		if (inet_csk(sk)->icsk_pmtu_cookie > dst_mtu(dst)) {
			tcp_sync_mss(sk, dst_mtu(dst));
			tcp_simple_retransmit(sk);
		} /* else let the usual retransmit timer handle it */
		dst_release(dst);
		goto out;
	}

	icmpv6_err_convert(type, code, &err);

	/* Might be for an request_sock */
	switch (sk->sk_state) {
		struct request_sock *req, **prev;
	case TCP_LISTEN:
		if (sock_owned_by_user(sk))
			goto out;

		req = inet6_csk_search_req(sk, &prev, th->dest, &hdr->daddr,
					   &hdr->saddr, inet6_iif(skb));
		if (!req)
			goto out;

		/* ICMPs are not backlogged, hence we cannot get
		 * an established socket here.
		 */
		WARN_ON(req->sk != NULL);

		if (seq != tcp_rsk(req)->snt_isn) {
			NET_INC_STATS_BH(net, LINUX_MIB_OUTOFWINDOWICMPS);
			goto out;
		}

		inet_csk_reqsk_queue_drop(sk, req, prev);
		goto out;

	case TCP_SYN_SENT:
	case TCP_SYN_RECV:  /* Cannot happen.
			       It can, it SYNs are crossed. --ANK */
		if (!sock_owned_by_user(sk)) {
			sk->sk_err = err;
			sk->sk_error_report(sk);		/* Wake people up to see the error (see connect in sock.c) */

			tcp_done(sk);
		} else
			sk->sk_err_soft = err;
		goto out;
	}

	if (!sock_owned_by_user(sk) && np->recverr) {
		sk->sk_err = err;
		sk->sk_error_report(sk);
	} else
		sk->sk_err_soft = err;

out:
	bh_unlock_sock(sk);
	sock_put(sk);
}


static int tcp_v6_send_synack(struct sock *sk, struct request_sock *req,
			      struct request_values *rvp)
{
	struct inet6_request_sock *treq = inet6_rsk(req);
	struct ipv6_pinfo *np = inet6_sk(sk);
	struct sk_buff * skb;
	struct ipv6_txoptions *opt = NULL;
	struct in6_addr * final_p, final;
	struct flowi6 fl6;
	struct dst_entry *dst;
	int err;

	memset(&fl6, 0, sizeof(fl6));
	fl6.flowi6_proto = IPPROTO_TCP;
	fl6.daddr = treq->rmt_addr;
	fl6.saddr = treq->loc_addr;
	fl6.flowlabel = 0;
	fl6.flowi6_oif = treq->iif;
	fl6.flowi6_mark = inet_rsk(req)->ir_mark;
	fl6.fl6_dport = inet_rsk(req)->rmt_port;
	fl6.fl6_sport = inet_rsk(req)->loc_port;
	security_req_classify_flow(req, flowi6_to_flowi(&fl6));

	opt = np->opt;
	final_p = fl6_update_dst(&fl6, opt, &final);

	dst = ip6_dst_lookup_flow(sk, &fl6, final_p, false);
	if (IS_ERR(dst)) {
		err = PTR_ERR(dst);
		dst = NULL;
		goto done;
	}
	skb = tcp_make_synack(sk, dst, req, rvp);
	err = -ENOMEM;
	if (skb) {
		__tcp_v6_send_check(skb, &treq->loc_addr, &treq->rmt_addr);

		fl6.daddr = treq->rmt_addr;
		err = ip6_xmit(sk, skb, &fl6, opt, np->tclass);
=======
	if (static_branch_unlikely(&ip6_min_hopcount)) {
		/* min_hopcount can be changed concurrently from do_ipv6_setsockopt() */
		if (ipv6_hdr(skb)->hop_limit < READ_ONCE(tcp_inet6_sk(sk)->min_hopcount)) {
			__NET_INC_STATS(net, LINUX_MIB_TCPMINTTLDROP);
			goto out;
		}
	}

	tp = tcp_sk(sk);
	/* XXX (TFO) - tp->snd_una should be ISN (tcp_create_openreq_child() */
	fastopen = rcu_dereference(tp->fastopen_rsk);
	snd_una = fastopen ? tcp_rsk(fastopen)->snt_isn : tp->snd_una;
	if (sk->sk_state != TCP_LISTEN &&
	    !between(seq, snd_una, tp->snd_nxt)) {
		__NET_INC_STATS(net, LINUX_MIB_OUTOFWINDOWICMPS);
		goto out;
	}

	np = tcp_inet6_sk(sk);

	if (type == NDISC_REDIRECT) {
		if (!sock_owned_by_user(sk)) {
			struct dst_entry *dst = __sk_dst_check(sk, np->dst_cookie);

			if (dst)
				dst->ops->redirect(dst, sk, skb);
		}
		goto out;
	}

	if (type == ICMPV6_PKT_TOOBIG) {
		u32 mtu = ntohl(info);

		/* We are not interested in TCP_LISTEN and open_requests
		 * (SYN-ACKs send out by Linux are always <576bytes so
		 * they should go through unfragmented).
		 */
		if (sk->sk_state == TCP_LISTEN)
			goto out;

		if (!ip6_sk_accept_pmtu(sk))
			goto out;

		if (mtu < IPV6_MIN_MTU)
			goto out;

		WRITE_ONCE(tp->mtu_info, mtu);

		if (!sock_owned_by_user(sk))
			tcp_v6_mtu_reduced(sk);
		else if (!test_and_set_bit(TCP_MTU_REDUCED_DEFERRED,
					   &sk->sk_tsq_flags))
			sock_hold(sk);
		goto out;
	}


	/* Might be for an request_sock */
	switch (sk->sk_state) {
	case TCP_SYN_SENT:
	case TCP_SYN_RECV:
		/* Only in fast or simultaneous open. If a fast open socket is
		 * already accepted it is treated as a connected one below.
		 */
		if (fastopen && !fastopen->sk)
			break;

		ipv6_icmp_error(sk, skb, err, th->dest, ntohl(info), (u8 *)th);

		if (!sock_owned_by_user(sk)) {
			WRITE_ONCE(sk->sk_err, err);
			sk_error_report(sk);		/* Wake people up to see the error (see connect in sock.c) */

			tcp_done(sk);
		} else {
			WRITE_ONCE(sk->sk_err_soft, err);
		}
		goto out;
	case TCP_LISTEN:
		break;
	default:
		/* check if this ICMP message allows revert of backoff.
		 * (see RFC 6069)
		 */
		if (!fastopen && type == ICMPV6_DEST_UNREACH &&
		    code == ICMPV6_NOROUTE)
			tcp_ld_RTO_revert(sk, seq);
	}

	if (!sock_owned_by_user(sk) && inet6_test_bit(RECVERR6, sk)) {
		WRITE_ONCE(sk->sk_err, err);
		sk_error_report(sk);
	} else {
		WRITE_ONCE(sk->sk_err_soft, err);
	}
out:
	bh_unlock_sock(sk);
	sock_put(sk);
	return 0;
}


static int tcp_v6_send_synack(const struct sock *sk, struct dst_entry *dst,
			      struct flowi *fl,
			      struct request_sock *req,
			      struct tcp_fastopen_cookie *foc,
			      enum tcp_synack_type synack_type,
			      struct sk_buff *syn_skb)
{
	struct inet_request_sock *ireq = inet_rsk(req);
	const struct ipv6_pinfo *np = tcp_inet6_sk(sk);
	struct ipv6_txoptions *opt;
	struct flowi6 *fl6 = &fl->u.ip6;
	struct sk_buff *skb;
	int err = -ENOMEM;
	u8 tclass;

	/* First, grab a route. */
	if (!dst && (dst = inet6_csk_route_req(sk, fl6, req,
					       IPPROTO_TCP)) == NULL)
		goto done;

	skb = tcp_make_synack(sk, dst, req, foc, synack_type, syn_skb);

	if (skb) {
		__tcp_v6_send_check(skb, &ireq->ir_v6_loc_addr,
				    &ireq->ir_v6_rmt_addr);

		fl6->daddr = ireq->ir_v6_rmt_addr;
		if (inet6_test_bit(REPFLOW, sk) && ireq->pktopts)
			fl6->flowlabel = ip6_flowlabel(ipv6_hdr(ireq->pktopts));

		tclass = READ_ONCE(sock_net(sk)->ipv4.sysctl_tcp_reflect_tos) ?
				(tcp_rsk(req)->syn_tos & ~INET_ECN_MASK) |
				(np->tclass & INET_ECN_MASK) :
				np->tclass;

		if (!INET_ECN_is_capable(tclass) &&
		    tcp_bpf_ca_needs_ecn((struct sock *)req))
			tclass |= INET_ECN_ECT_0;

		rcu_read_lock();
		opt = ireq->ipv6_opt;
		if (!opt)
			opt = rcu_dereference(np->opt);
		err = ip6_xmit(sk, skb, fl6, skb->mark ? : READ_ONCE(sk->sk_mark),
			       opt, tclass, READ_ONCE(sk->sk_priority));
		rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = net_xmit_eval(err);
	}

done:
<<<<<<< HEAD
	if (opt && opt != np->opt)
		sock_kfree_s(sk, opt, opt->tot_len);
	dst_release(dst);
	return err;
}

static int tcp_v6_rtx_synack(struct sock *sk, struct request_sock *req,
			     struct request_values *rvp)
{
	TCP_INC_STATS_BH(sock_net(sk), TCP_MIB_RETRANSSEGS);
	return tcp_v6_send_synack(sk, req, rvp);
}

static void tcp_v6_reqsk_destructor(struct request_sock *req)
{
	kfree_skb(inet6_rsk(req)->pktopts);
}

#ifdef CONFIG_TCP_MD5SIG
static struct tcp_md5sig_key *tcp_v6_md5_do_lookup(struct sock *sk,
						   const struct in6_addr *addr)
{
	return tcp_md5_do_lookup(sk, (union tcp_md5_addr *)addr, AF_INET6);
}

static struct tcp_md5sig_key *tcp_v6_md5_lookup(struct sock *sk,
						struct sock *addr_sk)
{
	return tcp_v6_md5_do_lookup(sk, &inet6_sk(addr_sk)->daddr);
}

static struct tcp_md5sig_key *tcp_v6_reqsk_md5_lookup(struct sock *sk,
						      struct request_sock *req)
{
	return tcp_v6_md5_do_lookup(sk, &inet6_rsk(req)->rmt_addr);
}

static int tcp_v6_parse_md5_keys (struct sock *sk, char __user *optval,
				  int optlen)
{
	struct tcp_md5sig cmd;
	struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *)&cmd.tcpm_addr;
=======
	return err;
}


static void tcp_v6_reqsk_destructor(struct request_sock *req)
{
	kfree(inet_rsk(req)->ipv6_opt);
	consume_skb(inet_rsk(req)->pktopts);
}

#ifdef CONFIG_TCP_MD5SIG
static struct tcp_md5sig_key *tcp_v6_md5_do_lookup(const struct sock *sk,
						   const struct in6_addr *addr,
						   int l3index)
{
	return tcp_md5_do_lookup(sk, l3index,
				 (union tcp_md5_addr *)addr, AF_INET6);
}

static struct tcp_md5sig_key *tcp_v6_md5_lookup(const struct sock *sk,
						const struct sock *addr_sk)
{
	int l3index;

	l3index = l3mdev_master_ifindex_by_index(sock_net(sk),
						 addr_sk->sk_bound_dev_if);
	return tcp_v6_md5_do_lookup(sk, &addr_sk->sk_v6_daddr,
				    l3index);
}

static int tcp_v6_parse_md5_keys(struct sock *sk, int optname,
				 sockptr_t optval, int optlen)
{
	struct tcp_md5sig cmd;
	struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *)&cmd.tcpm_addr;
	union tcp_ao_addr *addr;
	int l3index = 0;
	u8 prefixlen;
	bool l3flag;
	u8 flags;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (optlen < sizeof(cmd))
		return -EINVAL;

<<<<<<< HEAD
	if (copy_from_user(&cmd, optval, sizeof(cmd)))
=======
	if (copy_from_sockptr(&cmd, optval, sizeof(cmd)))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EFAULT;

	if (sin6->sin6_family != AF_INET6)
		return -EINVAL;

<<<<<<< HEAD
	if (!cmd.tcpm_keylen) {
		if (ipv6_addr_v4mapped(&sin6->sin6_addr))
			return tcp_md5_do_del(sk, (union tcp_md5_addr *)&sin6->sin6_addr.s6_addr32[3],
					      AF_INET);
		return tcp_md5_do_del(sk, (union tcp_md5_addr *)&sin6->sin6_addr,
				      AF_INET6);
=======
	flags = cmd.tcpm_flags & TCP_MD5SIG_FLAG_IFINDEX;
	l3flag = cmd.tcpm_flags & TCP_MD5SIG_FLAG_IFINDEX;

	if (optname == TCP_MD5SIG_EXT &&
	    cmd.tcpm_flags & TCP_MD5SIG_FLAG_PREFIX) {
		prefixlen = cmd.tcpm_prefixlen;
		if (prefixlen > 128 || (ipv6_addr_v4mapped(&sin6->sin6_addr) &&
					prefixlen > 32))
			return -EINVAL;
	} else {
		prefixlen = ipv6_addr_v4mapped(&sin6->sin6_addr) ? 32 : 128;
	}

	if (optname == TCP_MD5SIG_EXT && cmd.tcpm_ifindex &&
	    cmd.tcpm_flags & TCP_MD5SIG_FLAG_IFINDEX) {
		struct net_device *dev;

		rcu_read_lock();
		dev = dev_get_by_index_rcu(sock_net(sk), cmd.tcpm_ifindex);
		if (dev && netif_is_l3_master(dev))
			l3index = dev->ifindex;
		rcu_read_unlock();

		/* ok to reference set/not set outside of rcu;
		 * right now device MUST be an L3 master
		 */
		if (!dev || !l3index)
			return -EINVAL;
	}

	if (!cmd.tcpm_keylen) {
		if (ipv6_addr_v4mapped(&sin6->sin6_addr))
			return tcp_md5_do_del(sk, (union tcp_md5_addr *)&sin6->sin6_addr.s6_addr32[3],
					      AF_INET, prefixlen,
					      l3index, flags);
		return tcp_md5_do_del(sk, (union tcp_md5_addr *)&sin6->sin6_addr,
				      AF_INET6, prefixlen, l3index, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (cmd.tcpm_keylen > TCP_MD5SIG_MAXKEYLEN)
		return -EINVAL;

<<<<<<< HEAD
	if (ipv6_addr_v4mapped(&sin6->sin6_addr))
		return tcp_md5_do_add(sk, (union tcp_md5_addr *)&sin6->sin6_addr.s6_addr32[3],
				      AF_INET, cmd.tcpm_key, cmd.tcpm_keylen, GFP_KERNEL);

	return tcp_md5_do_add(sk, (union tcp_md5_addr *)&sin6->sin6_addr,
			      AF_INET6, cmd.tcpm_key, cmd.tcpm_keylen, GFP_KERNEL);
}

static int tcp_v6_md5_hash_pseudoheader(struct tcp_md5sig_pool *hp,
					const struct in6_addr *daddr,
					const struct in6_addr *saddr, int nbytes)
{
	struct tcp6_pseudohdr *bp;
	struct scatterlist sg;

	bp = &hp->md5_blk.ip6;
=======
	if (ipv6_addr_v4mapped(&sin6->sin6_addr)) {
		addr = (union tcp_md5_addr *)&sin6->sin6_addr.s6_addr32[3];

		/* Don't allow keys for peers that have a matching TCP-AO key.
		 * See the comment in tcp_ao_add_cmd()
		 */
		if (tcp_ao_required(sk, addr, AF_INET,
				    l3flag ? l3index : -1, false))
			return -EKEYREJECTED;
		return tcp_md5_do_add(sk, addr,
				      AF_INET, prefixlen, l3index, flags,
				      cmd.tcpm_key, cmd.tcpm_keylen);
	}

	addr = (union tcp_md5_addr *)&sin6->sin6_addr;

	/* Don't allow keys for peers that have a matching TCP-AO key.
	 * See the comment in tcp_ao_add_cmd()
	 */
	if (tcp_ao_required(sk, addr, AF_INET6, l3flag ? l3index : -1, false))
		return -EKEYREJECTED;

	return tcp_md5_do_add(sk, addr, AF_INET6, prefixlen, l3index, flags,
			      cmd.tcpm_key, cmd.tcpm_keylen);
}

static int tcp_v6_md5_hash_headers(struct tcp_sigpool *hp,
				   const struct in6_addr *daddr,
				   const struct in6_addr *saddr,
				   const struct tcphdr *th, int nbytes)
{
	struct tcp6_pseudohdr *bp;
	struct scatterlist sg;
	struct tcphdr *_th;

	bp = hp->scratch;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* 1. TCP pseudo-header (RFC2460) */
	bp->saddr = *saddr;
	bp->daddr = *daddr;
	bp->protocol = cpu_to_be32(IPPROTO_TCP);
	bp->len = cpu_to_be32(nbytes);

<<<<<<< HEAD
	sg_init_one(&sg, bp, sizeof(*bp));
	return crypto_hash_update(&hp->md5_desc, &sg, sizeof(*bp));
}

static int tcp_v6_md5_hash_hdr(char *md5_hash, struct tcp_md5sig_key *key,
			       const struct in6_addr *daddr, struct in6_addr *saddr,
			       const struct tcphdr *th)
{
	struct tcp_md5sig_pool *hp;
	struct hash_desc *desc;

	hp = tcp_get_md5sig_pool();
	if (!hp)
		goto clear_hash_noput;
	desc = &hp->md5_desc;

	if (crypto_hash_init(desc))
		goto clear_hash;
	if (tcp_v6_md5_hash_pseudoheader(hp, daddr, saddr, th->doff << 2))
		goto clear_hash;
	if (tcp_md5_hash_header(hp, th))
		goto clear_hash;
	if (tcp_md5_hash_key(hp, key))
		goto clear_hash;
	if (crypto_hash_final(desc, md5_hash))
		goto clear_hash;

	tcp_put_md5sig_pool();
	return 0;

clear_hash:
	tcp_put_md5sig_pool();
clear_hash_noput:
=======
	_th = (struct tcphdr *)(bp + 1);
	memcpy(_th, th, sizeof(*th));
	_th->check = 0;

	sg_init_one(&sg, bp, sizeof(*bp) + sizeof(*th));
	ahash_request_set_crypt(hp->req, &sg, NULL,
				sizeof(*bp) + sizeof(*th));
	return crypto_ahash_update(hp->req);
}

static int tcp_v6_md5_hash_hdr(char *md5_hash, const struct tcp_md5sig_key *key,
			       const struct in6_addr *daddr, struct in6_addr *saddr,
			       const struct tcphdr *th)
{
	struct tcp_sigpool hp;

	if (tcp_sigpool_start(tcp_md5_sigpool_id, &hp))
		goto clear_hash_nostart;

	if (crypto_ahash_init(hp.req))
		goto clear_hash;
	if (tcp_v6_md5_hash_headers(&hp, daddr, saddr, th, th->doff << 2))
		goto clear_hash;
	if (tcp_md5_hash_key(&hp, key))
		goto clear_hash;
	ahash_request_set_crypt(hp.req, NULL, md5_hash, 0);
	if (crypto_ahash_final(hp.req))
		goto clear_hash;

	tcp_sigpool_end(&hp);
	return 0;

clear_hash:
	tcp_sigpool_end(&hp);
clear_hash_nostart:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	memset(md5_hash, 0, 16);
	return 1;
}

<<<<<<< HEAD
static int tcp_v6_md5_hash_skb(char *md5_hash, struct tcp_md5sig_key *key,
			       const struct sock *sk,
			       const struct request_sock *req,
			       const struct sk_buff *skb)
{
	const struct in6_addr *saddr, *daddr;
	struct tcp_md5sig_pool *hp;
	struct hash_desc *desc;
	const struct tcphdr *th = tcp_hdr(skb);

	if (sk) {
		saddr = &inet6_sk(sk)->saddr;
		daddr = &inet6_sk(sk)->daddr;
	} else if (req) {
		saddr = &inet6_rsk(req)->loc_addr;
		daddr = &inet6_rsk(req)->rmt_addr;
=======
static int tcp_v6_md5_hash_skb(char *md5_hash,
			       const struct tcp_md5sig_key *key,
			       const struct sock *sk,
			       const struct sk_buff *skb)
{
	const struct tcphdr *th = tcp_hdr(skb);
	const struct in6_addr *saddr, *daddr;
	struct tcp_sigpool hp;

	if (sk) { /* valid for establish/request sockets */
		saddr = &sk->sk_v6_rcv_saddr;
		daddr = &sk->sk_v6_daddr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		const struct ipv6hdr *ip6h = ipv6_hdr(skb);
		saddr = &ip6h->saddr;
		daddr = &ip6h->daddr;
	}

<<<<<<< HEAD
	hp = tcp_get_md5sig_pool();
	if (!hp)
		goto clear_hash_noput;
	desc = &hp->md5_desc;

	if (crypto_hash_init(desc))
		goto clear_hash;

	if (tcp_v6_md5_hash_pseudoheader(hp, daddr, saddr, skb->len))
		goto clear_hash;
	if (tcp_md5_hash_header(hp, th))
		goto clear_hash;
	if (tcp_md5_hash_skb_data(hp, skb, th->doff << 2))
		goto clear_hash;
	if (tcp_md5_hash_key(hp, key))
		goto clear_hash;
	if (crypto_hash_final(desc, md5_hash))
		goto clear_hash;

	tcp_put_md5sig_pool();
	return 0;

clear_hash:
	tcp_put_md5sig_pool();
clear_hash_noput:
	memset(md5_hash, 0, 16);
	return 1;
}

static int tcp_v6_inbound_md5_hash(struct sock *sk, const struct sk_buff *skb)
{
	const __u8 *hash_location = NULL;
	struct tcp_md5sig_key *hash_expected;
	const struct ipv6hdr *ip6h = ipv6_hdr(skb);
	const struct tcphdr *th = tcp_hdr(skb);
	int genhash;
	u8 newhash[16];

	hash_expected = tcp_v6_md5_do_lookup(sk, &ip6h->saddr);
	hash_location = tcp_parse_md5sig_option(th);

	/* We've parsed the options - do we have a hash? */
	if (!hash_expected && !hash_location)
		return 0;

	if (hash_expected && !hash_location) {
		NET_INC_STATS_BH(sock_net(sk), LINUX_MIB_TCPMD5NOTFOUND);
		return 1;
	}

	if (!hash_expected && hash_location) {
		NET_INC_STATS_BH(sock_net(sk), LINUX_MIB_TCPMD5UNEXPECTED);
		return 1;
	}

	/* check the signature */
	genhash = tcp_v6_md5_hash_skb(newhash,
				      hash_expected,
				      NULL, NULL, skb);

	if (genhash || memcmp(hash_location, newhash, 16) != 0) {
		if (net_ratelimit()) {
			printk(KERN_INFO "MD5 Hash %s for [%pI6c]:%u->[%pI6c]:%u\n",
			       genhash ? "failed" : "mismatch",
			       &ip6h->saddr, ntohs(th->source),
			       &ip6h->daddr, ntohs(th->dest));
		}
		return 1;
	}
	return 0;
}
#endif

struct request_sock_ops tcp6_request_sock_ops __read_mostly = {
	.family		=	AF_INET6,
	.obj_size	=	sizeof(struct tcp6_request_sock),
	.rtx_syn_ack	=	tcp_v6_rtx_synack,
	.send_ack	=	tcp_v6_reqsk_send_ack,
	.destructor	=	tcp_v6_reqsk_destructor,
	.send_reset	=	tcp_v6_send_reset,
	.syn_ack_timeout = 	tcp_syn_ack_timeout,
};

#ifdef CONFIG_TCP_MD5SIG
static const struct tcp_request_sock_ops tcp_request_sock_ipv6_ops = {
	.md5_lookup	=	tcp_v6_reqsk_md5_lookup,
	.calc_md5_hash	=	tcp_v6_md5_hash_skb,
};
#endif

static void __tcp_v6_send_check(struct sk_buff *skb,
				const struct in6_addr *saddr, const struct in6_addr *daddr)
{
	struct tcphdr *th = tcp_hdr(skb);

	if (skb->ip_summed == CHECKSUM_PARTIAL) {
		th->check = ~tcp_v6_check(skb->len, saddr, daddr, 0);
		skb->csum_start = skb_transport_header(skb) - skb->head;
		skb->csum_offset = offsetof(struct tcphdr, check);
	} else {
		th->check = tcp_v6_check(skb->len, saddr, daddr,
					 csum_partial(th, th->doff << 2,
						      skb->csum));
	}
}

static void tcp_v6_send_check(struct sock *sk, struct sk_buff *skb)
{
	struct ipv6_pinfo *np = inet6_sk(sk);

	__tcp_v6_send_check(skb, &np->saddr, &np->daddr);
}

static int tcp_v6_gso_send_check(struct sk_buff *skb)
{
	const struct ipv6hdr *ipv6h;
	struct tcphdr *th;

	if (!pskb_may_pull(skb, sizeof(*th)))
		return -EINVAL;

	ipv6h = ipv6_hdr(skb);
	th = tcp_hdr(skb);

	th->check = 0;
	skb->ip_summed = CHECKSUM_PARTIAL;
	__tcp_v6_send_check(skb, &ipv6h->saddr, &ipv6h->daddr);
	return 0;
}

static struct sk_buff **tcp6_gro_receive(struct sk_buff **head,
					 struct sk_buff *skb)
{
	const struct ipv6hdr *iph = skb_gro_network_header(skb);

	switch (skb->ip_summed) {
	case CHECKSUM_COMPLETE:
		if (!tcp_v6_check(skb_gro_len(skb), &iph->saddr, &iph->daddr,
				  skb->csum)) {
			skb->ip_summed = CHECKSUM_UNNECESSARY;
			break;
		}

		/* fall through */
	case CHECKSUM_NONE:
		NAPI_GRO_CB(skb)->flush = 1;
		return NULL;
	}

	return tcp_gro_receive(head, skb);
}

static int tcp6_gro_complete(struct sk_buff *skb)
{
	const struct ipv6hdr *iph = ipv6_hdr(skb);
	struct tcphdr *th = tcp_hdr(skb);

	th->check = ~tcp_v6_check(skb->len - skb_transport_offset(skb),
				  &iph->saddr, &iph->daddr, 0);
	skb_shinfo(skb)->gso_type = SKB_GSO_TCPV6;

	return tcp_gro_complete(skb);
}

static void tcp_v6_send_response(struct sock *sk, struct sk_buff *skb, u32 seq,
				 u32 ack, u32 win, u32 ts,
				 struct tcp_md5sig_key *key, int rst, u8 tclass)
=======
	if (tcp_sigpool_start(tcp_md5_sigpool_id, &hp))
		goto clear_hash_nostart;

	if (crypto_ahash_init(hp.req))
		goto clear_hash;

	if (tcp_v6_md5_hash_headers(&hp, daddr, saddr, th, skb->len))
		goto clear_hash;
	if (tcp_sigpool_hash_skb_data(&hp, skb, th->doff << 2))
		goto clear_hash;
	if (tcp_md5_hash_key(&hp, key))
		goto clear_hash;
	ahash_request_set_crypt(hp.req, NULL, md5_hash, 0);
	if (crypto_ahash_final(hp.req))
		goto clear_hash;

	tcp_sigpool_end(&hp);
	return 0;

clear_hash:
	tcp_sigpool_end(&hp);
clear_hash_nostart:
	memset(md5_hash, 0, 16);
	return 1;
}
#endif

static void tcp_v6_init_req(struct request_sock *req,
			    const struct sock *sk_listener,
			    struct sk_buff *skb)
{
	bool l3_slave = ipv6_l3mdev_skb(TCP_SKB_CB(skb)->header.h6.flags);
	struct inet_request_sock *ireq = inet_rsk(req);
	const struct ipv6_pinfo *np = tcp_inet6_sk(sk_listener);

	ireq->ir_v6_rmt_addr = ipv6_hdr(skb)->saddr;
	ireq->ir_v6_loc_addr = ipv6_hdr(skb)->daddr;

	/* So that link locals have meaning */
	if ((!sk_listener->sk_bound_dev_if || l3_slave) &&
	    ipv6_addr_type(&ireq->ir_v6_rmt_addr) & IPV6_ADDR_LINKLOCAL)
		ireq->ir_iif = tcp_v6_iif(skb);

	if (!TCP_SKB_CB(skb)->tcp_tw_isn &&
	    (ipv6_opt_accepted(sk_listener, skb, &TCP_SKB_CB(skb)->header.h6) ||
	     np->rxopt.bits.rxinfo ||
	     np->rxopt.bits.rxoinfo || np->rxopt.bits.rxhlim ||
	     np->rxopt.bits.rxohlim || inet6_test_bit(REPFLOW, sk_listener))) {
		refcount_inc(&skb->users);
		ireq->pktopts = skb;
	}
}

static struct dst_entry *tcp_v6_route_req(const struct sock *sk,
					  struct sk_buff *skb,
					  struct flowi *fl,
					  struct request_sock *req)
{
	tcp_v6_init_req(req, sk, skb);

	if (security_inet_conn_request(sk, skb, req))
		return NULL;

	return inet6_csk_route_req(sk, &fl->u.ip6, req, IPPROTO_TCP);
}

struct request_sock_ops tcp6_request_sock_ops __read_mostly = {
	.family		=	AF_INET6,
	.obj_size	=	sizeof(struct tcp6_request_sock),
	.rtx_syn_ack	=	tcp_rtx_synack,
	.send_ack	=	tcp_v6_reqsk_send_ack,
	.destructor	=	tcp_v6_reqsk_destructor,
	.send_reset	=	tcp_v6_send_reset,
	.syn_ack_timeout =	tcp_syn_ack_timeout,
};

const struct tcp_request_sock_ops tcp_request_sock_ipv6_ops = {
	.mss_clamp	=	IPV6_MIN_MTU - sizeof(struct tcphdr) -
				sizeof(struct ipv6hdr),
#ifdef CONFIG_TCP_MD5SIG
	.req_md5_lookup	=	tcp_v6_md5_lookup,
	.calc_md5_hash	=	tcp_v6_md5_hash_skb,
#endif
#ifdef CONFIG_TCP_AO
	.ao_lookup	=	tcp_v6_ao_lookup_rsk,
	.ao_calc_key	=	tcp_v6_ao_calc_key_rsk,
	.ao_synack_hash =	tcp_v6_ao_synack_hash,
#endif
#ifdef CONFIG_SYN_COOKIES
	.cookie_init_seq =	cookie_v6_init_sequence,
#endif
	.route_req	=	tcp_v6_route_req,
	.init_seq	=	tcp_v6_init_seq,
	.init_ts_off	=	tcp_v6_init_ts_off,
	.send_synack	=	tcp_v6_send_synack,
};

static void tcp_v6_send_response(const struct sock *sk, struct sk_buff *skb, u32 seq,
				 u32 ack, u32 win, u32 tsval, u32 tsecr,
				 int oif, int rst, u8 tclass, __be32 label,
				 u32 priority, u32 txhash, struct tcp_key *key)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	const struct tcphdr *th = tcp_hdr(skb);
	struct tcphdr *t1;
	struct sk_buff *buff;
	struct flowi6 fl6;
	struct net *net = sk ? sock_net(sk) : dev_net(skb_dst(skb)->dev);
	struct sock *ctl_sk = net->ipv6.tcp_sk;
	unsigned int tot_len = sizeof(struct tcphdr);
<<<<<<< HEAD
	struct dst_entry *dst;
	__be32 *topt;

	if (ts)
		tot_len += TCPOLEN_TSTAMP_ALIGNED;
#ifdef CONFIG_TCP_MD5SIG
	if (key)
		tot_len += TCPOLEN_MD5SIG_ALIGNED;
#endif

	buff = alloc_skb(MAX_HEADER + sizeof(struct ipv6hdr) + tot_len,
			 GFP_ATOMIC);
	if (buff == NULL)
		return;

	skb_reserve(buff, MAX_HEADER + sizeof(struct ipv6hdr) + tot_len);

	t1 = (struct tcphdr *) skb_push(buff, tot_len);
=======
	__be32 mrst = 0, *topt;
	struct dst_entry *dst;
	__u32 mark = 0;

	if (tsecr)
		tot_len += TCPOLEN_TSTAMP_ALIGNED;
	if (tcp_key_is_md5(key))
		tot_len += TCPOLEN_MD5SIG_ALIGNED;
	if (tcp_key_is_ao(key))
		tot_len += tcp_ao_len_aligned(key->ao_key);

#ifdef CONFIG_MPTCP
	if (rst && !tcp_key_is_md5(key)) {
		mrst = mptcp_reset_option(skb);

		if (mrst)
			tot_len += sizeof(__be32);
	}
#endif

	buff = alloc_skb(MAX_TCP_HEADER, GFP_ATOMIC);
	if (!buff)
		return;

	skb_reserve(buff, MAX_TCP_HEADER);

	t1 = skb_push(buff, tot_len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	skb_reset_transport_header(buff);

	/* Swap the send and the receive. */
	memset(t1, 0, sizeof(*t1));
	t1->dest = th->source;
	t1->source = th->dest;
	t1->doff = tot_len / 4;
	t1->seq = htonl(seq);
	t1->ack_seq = htonl(ack);
	t1->ack = !rst || !th->ack;
	t1->rst = rst;
	t1->window = htons(win);

	topt = (__be32 *)(t1 + 1);

<<<<<<< HEAD
	if (ts) {
		*topt++ = htonl((TCPOPT_NOP << 24) | (TCPOPT_NOP << 16) |
				(TCPOPT_TIMESTAMP << 8) | TCPOLEN_TIMESTAMP);
		*topt++ = htonl(tcp_time_stamp);
		*topt++ = htonl(ts);
	}

#ifdef CONFIG_TCP_MD5SIG
	if (key) {
		*topt++ = htonl((TCPOPT_NOP << 24) | (TCPOPT_NOP << 16) |
				(TCPOPT_MD5SIG << 8) | TCPOLEN_MD5SIG);
		tcp_v6_md5_hash_hdr((__u8 *)topt, key,
=======
	if (tsecr) {
		*topt++ = htonl((TCPOPT_NOP << 24) | (TCPOPT_NOP << 16) |
				(TCPOPT_TIMESTAMP << 8) | TCPOLEN_TIMESTAMP);
		*topt++ = htonl(tsval);
		*topt++ = htonl(tsecr);
	}

	if (mrst)
		*topt++ = mrst;

#ifdef CONFIG_TCP_MD5SIG
	if (tcp_key_is_md5(key)) {
		*topt++ = htonl((TCPOPT_NOP << 24) | (TCPOPT_NOP << 16) |
				(TCPOPT_MD5SIG << 8) | TCPOLEN_MD5SIG);
		tcp_v6_md5_hash_hdr((__u8 *)topt, key->md5_key,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				    &ipv6_hdr(skb)->saddr,
				    &ipv6_hdr(skb)->daddr, t1);
	}
#endif
<<<<<<< HEAD
=======
#ifdef CONFIG_TCP_AO
	if (tcp_key_is_ao(key)) {
		*topt++ = htonl((TCPOPT_AO << 24) |
				(tcp_ao_len(key->ao_key) << 16) |
				(key->ao_key->sndid << 8) |
				(key->rcv_next));

		tcp_ao_hash_hdr(AF_INET6, (char *)topt, key->ao_key,
				key->traffic_key,
				(union tcp_ao_addr *)&ipv6_hdr(skb)->saddr,
				(union tcp_ao_addr *)&ipv6_hdr(skb)->daddr,
				t1, key->sne);
	}
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	memset(&fl6, 0, sizeof(fl6));
	fl6.daddr = ipv6_hdr(skb)->saddr;
	fl6.saddr = ipv6_hdr(skb)->daddr;
<<<<<<< HEAD

	buff->ip_summed = CHECKSUM_PARTIAL;
	buff->csum = 0;
=======
	fl6.flowlabel = label;

	buff->ip_summed = CHECKSUM_PARTIAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	__tcp_v6_send_check(buff, &fl6.saddr, &fl6.daddr);

	fl6.flowi6_proto = IPPROTO_TCP;
<<<<<<< HEAD
	if (ipv6_addr_type(&fl6.daddr) & IPV6_ADDR_LINKLOCAL)
		fl6.flowi6_oif = inet6_iif(skb);
	fl6.flowi6_mark = IP6_REPLY_MARK(net, skb->mark);
	fl6.fl6_dport = t1->dest;
	fl6.fl6_sport = t1->source;
	fl6.flowi6_uid = sock_net_uid(net, sk && sk_fullsock(sk) ? sk : NULL);
	security_skb_classify_flow(skb, flowi6_to_flowi(&fl6));
=======
	if (rt6_need_strict(&fl6.daddr) && !oif)
		fl6.flowi6_oif = tcp_v6_iif(skb);
	else {
		if (!oif && netif_index_is_l3_master(net, skb->skb_iif))
			oif = skb->skb_iif;

		fl6.flowi6_oif = oif;
	}

	if (sk) {
		if (sk->sk_state == TCP_TIME_WAIT)
			mark = inet_twsk(sk)->tw_mark;
		else
			mark = READ_ONCE(sk->sk_mark);
		skb_set_delivery_time(buff, tcp_transmit_time(sk), true);
	}
	if (txhash) {
		/* autoflowlabel/skb_get_hash_flowi6 rely on buff->hash */
		skb_set_hash(buff, txhash, PKT_HASH_TYPE_L4);
	}
	fl6.flowi6_mark = IP6_REPLY_MARK(net, skb->mark) ?: mark;
	fl6.fl6_dport = t1->dest;
	fl6.fl6_sport = t1->source;
	fl6.flowi6_uid = sock_net_uid(net, sk && sk_fullsock(sk) ? sk : NULL);
	security_skb_classify_flow(skb, flowi6_to_flowi_common(&fl6));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Pass a socket to ip6_dst_lookup either it is for RST
	 * Underlying function will use this to retrieve the network
	 * namespace
	 */
<<<<<<< HEAD
	dst = ip6_dst_lookup_flow(ctl_sk, &fl6, NULL, false);
	if (!IS_ERR(dst)) {
		skb_dst_set(buff, dst);
		ip6_xmit(ctl_sk, buff, &fl6, NULL, tclass);
		TCP_INC_STATS_BH(net, TCP_MIB_OUTSEGS);
		if (rst)
			TCP_INC_STATS_BH(net, TCP_MIB_OUTRSTS);
=======
	if (sk && sk->sk_state != TCP_TIME_WAIT)
		dst = ip6_dst_lookup_flow(net, sk, &fl6, NULL); /*sk's xfrm_policy can be referred*/
	else
		dst = ip6_dst_lookup_flow(net, ctl_sk, &fl6, NULL);
	if (!IS_ERR(dst)) {
		skb_dst_set(buff, dst);
		ip6_xmit(ctl_sk, buff, &fl6, fl6.flowi6_mark, NULL,
			 tclass & ~INET_ECN_MASK, priority);
		TCP_INC_STATS(net, TCP_MIB_OUTSEGS);
		if (rst)
			TCP_INC_STATS(net, TCP_MIB_OUTRSTS);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;
	}

	kfree_skb(buff);
}

<<<<<<< HEAD
static void tcp_v6_send_reset(struct sock *sk, struct sk_buff *skb)
{
	const struct tcphdr *th = tcp_hdr(skb);
	u32 seq = 0, ack_seq = 0;
	struct tcp_md5sig_key *key = NULL;
#ifdef CONFIG_TCP_MD5SIG
	const __u8 *hash_location = NULL;
	struct ipv6hdr *ipv6h = ipv6_hdr(skb);
=======
static void tcp_v6_send_reset(const struct sock *sk, struct sk_buff *skb)
{
	const struct tcphdr *th = tcp_hdr(skb);
	struct ipv6hdr *ipv6h = ipv6_hdr(skb);
	const __u8 *md5_hash_location = NULL;
#if defined(CONFIG_TCP_MD5SIG) || defined(CONFIG_TCP_AO)
	bool allocated_traffic_key = false;
#endif
	const struct tcp_ao_hdr *aoh;
	struct tcp_key key = {};
	u32 seq = 0, ack_seq = 0;
	__be32 label = 0;
	u32 priority = 0;
	struct net *net;
	u32 txhash = 0;
	int oif = 0;
#ifdef CONFIG_TCP_MD5SIG
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned char newhash[16];
	int genhash;
	struct sock *sk1 = NULL;
#endif

	if (th->rst)
		return;

	/* If sk not NULL, it means we did a successful lookup and incoming
	 * route had to be correct. prequeue might have dropped our dst.
	 */
	if (!sk && !ipv6_unicast_destination(skb))
		return;

<<<<<<< HEAD
#ifdef CONFIG_TCP_MD5SIG
	hash_location = tcp_parse_md5sig_option(th);
	if (!sk && hash_location) {
=======
	net = sk ? sock_net(sk) : dev_net(skb_dst(skb)->dev);
	/* Invalid TCP option size or twice included auth */
	if (tcp_parse_auth_options(th, &md5_hash_location, &aoh))
		return;
#if defined(CONFIG_TCP_MD5SIG) || defined(CONFIG_TCP_AO)
	rcu_read_lock();
#endif
#ifdef CONFIG_TCP_MD5SIG
	if (sk && sk_fullsock(sk)) {
		int l3index;

		/* sdif set, means packet ingressed via a device
		 * in an L3 domain and inet_iif is set to it.
		 */
		l3index = tcp_v6_sdif(skb) ? tcp_v6_iif_l3_slave(skb) : 0;
		key.md5_key = tcp_v6_md5_do_lookup(sk, &ipv6h->saddr, l3index);
		if (key.md5_key)
			key.type = TCP_KEY_MD5;
	} else if (md5_hash_location) {
		int dif = tcp_v6_iif_l3_slave(skb);
		int sdif = tcp_v6_sdif(skb);
		int l3index;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * active side is lost. Try to find listening socket through
		 * source port, and then find md5 key through listening socket.
		 * we are not loose security here:
		 * Incoming packet is checked with md5 hash with finding key,
		 * no RST generated if md5 hash doesn't match.
		 */
<<<<<<< HEAD
		sk1 = inet6_lookup_listener(dev_net(skb_dst(skb)->dev),
					   &tcp_hashinfo, &ipv6h->daddr,
					   ntohs(th->source), inet6_iif(skb));
		if (!sk1)
			return;

		rcu_read_lock();
		key = tcp_v6_md5_do_lookup(sk1, &ipv6h->saddr);
		if (!key)
			goto release_sk1;

		genhash = tcp_v6_md5_hash_skb(newhash, key, NULL, NULL, skb);
		if (genhash || memcmp(hash_location, newhash, 16) != 0)
			goto release_sk1;
	} else {
		key = sk ? tcp_v6_md5_do_lookup(sk, &ipv6h->saddr) : NULL;
=======
		sk1 = inet6_lookup_listener(net, net->ipv4.tcp_death_row.hashinfo,
					    NULL, 0, &ipv6h->saddr, th->source,
					    &ipv6h->daddr, ntohs(th->source),
					    dif, sdif);
		if (!sk1)
			goto out;

		/* sdif set, means packet ingressed via a device
		 * in an L3 domain and dif is set to it.
		 */
		l3index = tcp_v6_sdif(skb) ? dif : 0;

		key.md5_key = tcp_v6_md5_do_lookup(sk1, &ipv6h->saddr, l3index);
		if (!key.md5_key)
			goto out;
		key.type = TCP_KEY_MD5;

		genhash = tcp_v6_md5_hash_skb(newhash, key.md5_key, NULL, skb);
		if (genhash || memcmp(md5_hash_location, newhash, 16) != 0)
			goto out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
#endif

	if (th->ack)
		seq = ntohl(th->ack_seq);
	else
		ack_seq = ntohl(th->seq) + th->syn + th->fin + skb->len -
			  (th->doff << 2);

<<<<<<< HEAD
	tcp_v6_send_response(sk, skb, seq, ack_seq, 0, 0, key, 1, 0);

#ifdef CONFIG_TCP_MD5SIG
release_sk1:
	if (sk1) {
		rcu_read_unlock();
		sock_put(sk1);
	}
#endif
}

static void tcp_v6_send_ack(struct sock *sk, struct sk_buff *skb,
			    u32 seq, u32 ack, u32 win, u32 ts,
			    struct tcp_md5sig_key *key, u8 tclass)
{
	tcp_v6_send_response(sk, skb, seq, ack, win, ts, key, 0, tclass);
=======
#ifdef CONFIG_TCP_AO
	if (aoh) {
		int l3index;

		l3index = tcp_v6_sdif(skb) ? tcp_v6_iif_l3_slave(skb) : 0;
		if (tcp_ao_prepare_reset(sk, skb, aoh, l3index, seq,
					 &key.ao_key, &key.traffic_key,
					 &allocated_traffic_key,
					 &key.rcv_next, &key.sne))
			goto out;
		key.type = TCP_KEY_AO;
	}
#endif

	if (sk) {
		oif = sk->sk_bound_dev_if;
		if (sk_fullsock(sk)) {
			trace_tcp_send_reset(sk, skb);
			if (inet6_test_bit(REPFLOW, sk))
				label = ip6_flowlabel(ipv6h);
			priority = READ_ONCE(sk->sk_priority);
			txhash = sk->sk_txhash;
		}
		if (sk->sk_state == TCP_TIME_WAIT) {
			label = cpu_to_be32(inet_twsk(sk)->tw_flowlabel);
			priority = inet_twsk(sk)->tw_priority;
			txhash = inet_twsk(sk)->tw_txhash;
		}
	} else {
		if (net->ipv6.sysctl.flowlabel_reflect & FLOWLABEL_REFLECT_TCP_RESET)
			label = ip6_flowlabel(ipv6h);
	}

	tcp_v6_send_response(sk, skb, seq, ack_seq, 0, 0, 0, oif, 1,
			     ipv6_get_dsfield(ipv6h), label, priority, txhash,
			     &key);

#if defined(CONFIG_TCP_MD5SIG) || defined(CONFIG_TCP_AO)
out:
	if (allocated_traffic_key)
		kfree(key.traffic_key);
	rcu_read_unlock();
#endif
}

static void tcp_v6_send_ack(const struct sock *sk, struct sk_buff *skb, u32 seq,
			    u32 ack, u32 win, u32 tsval, u32 tsecr, int oif,
			    struct tcp_key *key, u8 tclass,
			    __be32 label, u32 priority, u32 txhash)
{
	tcp_v6_send_response(sk, skb, seq, ack, win, tsval, tsecr, oif, 0,
			     tclass, label, priority, txhash, key);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void tcp_v6_timewait_ack(struct sock *sk, struct sk_buff *skb)
{
	struct inet_timewait_sock *tw = inet_twsk(sk);
	struct tcp_timewait_sock *tcptw = tcp_twsk(sk);
<<<<<<< HEAD

	tcp_v6_send_ack(sk, skb, tcptw->tw_snd_nxt, tcptw->tw_rcv_nxt,
			tcptw->tw_rcv_wnd >> tw->tw_rcv_wscale,
			tcptw->tw_ts_recent, tcp_twsk_md5_key(tcptw),
			tw->tw_tclass);

	inet_twsk_put(tw);
}

static void tcp_v6_reqsk_send_ack(struct sock *sk, struct sk_buff *skb,
				  struct request_sock *req)
{
	tcp_v6_send_ack(sk, skb, tcp_rsk(req)->snt_isn + 1, tcp_rsk(req)->rcv_isn + 1, req->rcv_wnd, req->ts_recent,
			tcp_v6_md5_do_lookup(sk, &ipv6_hdr(skb)->daddr), 0);
}


static struct sock *tcp_v6_hnd_req(struct sock *sk,struct sk_buff *skb)
{
	struct request_sock *req, **prev;
	const struct tcphdr *th = tcp_hdr(skb);
	struct sock *nsk;

	/* Find possible connection requests. */
	req = inet6_csk_search_req(sk, &prev, th->source,
				   &ipv6_hdr(skb)->saddr,
				   &ipv6_hdr(skb)->daddr, inet6_iif(skb));
	if (req)
		return tcp_check_req(sk, skb, req, prev);

	nsk = __inet6_lookup_established(sock_net(sk), &tcp_hashinfo,
			&ipv6_hdr(skb)->saddr, th->source,
			&ipv6_hdr(skb)->daddr, ntohs(th->dest), inet6_iif(skb));

	if (nsk) {
		if (nsk->sk_state != TCP_TIME_WAIT) {
			bh_lock_sock(nsk);
			return nsk;
		}
		inet_twsk_put(inet_twsk(nsk));
		return NULL;
	}

#ifdef CONFIG_SYN_COOKIES
=======
	struct tcp_key key = {};
#ifdef CONFIG_TCP_AO
	struct tcp_ao_info *ao_info;

	if (static_branch_unlikely(&tcp_ao_needed.key)) {

		/* FIXME: the segment to-be-acked is not verified yet */
		ao_info = rcu_dereference(tcptw->ao_info);
		if (ao_info) {
			const struct tcp_ao_hdr *aoh;

			/* Invalid TCP option size or twice included auth */
			if (tcp_parse_auth_options(tcp_hdr(skb), NULL, &aoh))
				goto out;
			if (aoh)
				key.ao_key = tcp_ao_established_key(ao_info,
						aoh->rnext_keyid, -1);
		}
	}
	if (key.ao_key) {
		struct tcp_ao_key *rnext_key;

		key.traffic_key = snd_other_key(key.ao_key);
		/* rcv_next switches to our rcv_next */
		rnext_key = READ_ONCE(ao_info->rnext_key);
		key.rcv_next = rnext_key->rcvid;
		key.sne = READ_ONCE(ao_info->snd_sne);
		key.type = TCP_KEY_AO;
#else
	if (0) {
#endif
#ifdef CONFIG_TCP_MD5SIG
	} else if (static_branch_unlikely(&tcp_md5_needed.key)) {
		key.md5_key = tcp_twsk_md5_key(tcptw);
		if (key.md5_key)
			key.type = TCP_KEY_MD5;
#endif
	}

	tcp_v6_send_ack(sk, skb, tcptw->tw_snd_nxt, tcptw->tw_rcv_nxt,
			tcptw->tw_rcv_wnd >> tw->tw_rcv_wscale,
			tcp_tw_tsval(tcptw),
			tcptw->tw_ts_recent, tw->tw_bound_dev_if, &key,
			tw->tw_tclass, cpu_to_be32(tw->tw_flowlabel), tw->tw_priority,
			tw->tw_txhash);

#ifdef CONFIG_TCP_AO
out:
#endif
	inet_twsk_put(tw);
}

static void tcp_v6_reqsk_send_ack(const struct sock *sk, struct sk_buff *skb,
				  struct request_sock *req)
{
	struct tcp_key key = {};

#ifdef CONFIG_TCP_AO
	if (static_branch_unlikely(&tcp_ao_needed.key) &&
	    tcp_rsk_used_ao(req)) {
		const struct in6_addr *addr = &ipv6_hdr(skb)->saddr;
		const struct tcp_ao_hdr *aoh;
		int l3index;

		l3index = tcp_v6_sdif(skb) ? tcp_v6_iif_l3_slave(skb) : 0;
		/* Invalid TCP option size or twice included auth */
		if (tcp_parse_auth_options(tcp_hdr(skb), NULL, &aoh))
			return;
		if (!aoh)
			return;
		key.ao_key = tcp_ao_do_lookup(sk, l3index,
					      (union tcp_ao_addr *)addr,
					      AF_INET6, aoh->rnext_keyid, -1);
		if (unlikely(!key.ao_key)) {
			/* Send ACK with any matching MKT for the peer */
			key.ao_key = tcp_ao_do_lookup(sk, l3index,
						      (union tcp_ao_addr *)addr,
						      AF_INET6, -1, -1);
			/* Matching key disappeared (user removed the key?)
			 * let the handshake timeout.
			 */
			if (!key.ao_key) {
				net_info_ratelimited("TCP-AO key for (%pI6, %d)->(%pI6, %d) suddenly disappeared, won't ACK new connection\n",
						     addr,
						     ntohs(tcp_hdr(skb)->source),
						     &ipv6_hdr(skb)->daddr,
						     ntohs(tcp_hdr(skb)->dest));
				return;
			}
		}
		key.traffic_key = kmalloc(tcp_ao_digest_size(key.ao_key), GFP_ATOMIC);
		if (!key.traffic_key)
			return;

		key.type = TCP_KEY_AO;
		key.rcv_next = aoh->keyid;
		tcp_v6_ao_calc_key_rsk(key.ao_key, key.traffic_key, req);
#else
	if (0) {
#endif
#ifdef CONFIG_TCP_MD5SIG
	} else if (static_branch_unlikely(&tcp_md5_needed.key)) {
		int l3index = tcp_v6_sdif(skb) ? tcp_v6_iif_l3_slave(skb) : 0;

		key.md5_key = tcp_v6_md5_do_lookup(sk, &ipv6_hdr(skb)->saddr,
						   l3index);
		if (key.md5_key)
			key.type = TCP_KEY_MD5;
#endif
	}

	/* sk->sk_state == TCP_LISTEN -> for regular TCP_SYN_RECV
	 * sk->sk_state == TCP_SYN_RECV -> for Fast Open.
	 */
	/* RFC 7323 2.3
	 * The window field (SEG.WND) of every outgoing segment, with the
	 * exception of <SYN> segments, MUST be right-shifted by
	 * Rcv.Wind.Shift bits:
	 */
	tcp_v6_send_ack(sk, skb, (sk->sk_state == TCP_LISTEN) ?
			tcp_rsk(req)->snt_isn + 1 : tcp_sk(sk)->snd_nxt,
			tcp_rsk(req)->rcv_nxt,
			req->rsk_rcv_wnd >> inet_rsk(req)->rcv_wscale,
			tcp_rsk_tsval(tcp_rsk(req)),
			READ_ONCE(req->ts_recent), sk->sk_bound_dev_if,
			&key, ipv6_get_dsfield(ipv6_hdr(skb)), 0,
			READ_ONCE(sk->sk_priority),
			READ_ONCE(tcp_rsk(req)->txhash));
	if (tcp_key_is_ao(&key))
		kfree(key.traffic_key);
}


static struct sock *tcp_v6_cookie_check(struct sock *sk, struct sk_buff *skb)
{
#ifdef CONFIG_SYN_COOKIES
	const struct tcphdr *th = tcp_hdr(skb);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!th->syn)
		sk = cookie_v6_check(sk, skb);
#endif
	return sk;
}

<<<<<<< HEAD
/* FIXME: this is substantially similar to the ipv4 code.
 * Can some kind of merge be done? -- erics
 */
static int tcp_v6_conn_request(struct sock *sk, struct sk_buff *skb)
{
	struct tcp_extend_values tmp_ext;
	struct tcp_options_received tmp_opt;
	const u8 *hash_location;
	struct request_sock *req;
	struct inet6_request_sock *treq;
	struct ipv6_pinfo *np = inet6_sk(sk);
	struct tcp_sock *tp = tcp_sk(sk);
	__u32 isn = TCP_SKB_CB(skb)->when;
	struct dst_entry *dst = NULL;
	int want_cookie = 0;

=======
u16 tcp_v6_get_syncookie(struct sock *sk, struct ipv6hdr *iph,
			 struct tcphdr *th, u32 *cookie)
{
	u16 mss = 0;
#ifdef CONFIG_SYN_COOKIES
	mss = tcp_get_syncookie_mss(&tcp6_request_sock_ops,
				    &tcp_request_sock_ipv6_ops, sk, th);
	if (mss) {
		*cookie = __cookie_v6_init_sequence(iph, th, &mss);
		tcp_synq_overflow(sk);
	}
#endif
	return mss;
}

static int tcp_v6_conn_request(struct sock *sk, struct sk_buff *skb)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (skb->protocol == htons(ETH_P_IP))
		return tcp_v4_conn_request(sk, skb);

	if (!ipv6_unicast_destination(skb))
		goto drop;

<<<<<<< HEAD
	if (inet_csk_reqsk_queue_is_full(sk) && !isn) {
		want_cookie = tcp_syn_flood_action(sk, skb, "TCPv6");
		if (!want_cookie)
			goto drop;
	}

	if (sk_acceptq_is_full(sk) && inet_csk_reqsk_queue_young(sk) > 1)
		goto drop;

	req = inet6_reqsk_alloc(&tcp6_request_sock_ops);
	if (req == NULL)
		goto drop;

#ifdef CONFIG_TCP_MD5SIG
	tcp_rsk(req)->af_specific = &tcp_request_sock_ipv6_ops;
#endif

	tcp_clear_options(&tmp_opt);
	tmp_opt.mss_clamp = IPV6_MIN_MTU - sizeof(struct tcphdr) - sizeof(struct ipv6hdr);
	tmp_opt.user_mss = tp->rx_opt.user_mss;
	tcp_parse_options(skb, &tmp_opt, &hash_location, 0);

	if (tmp_opt.cookie_plus > 0 &&
	    tmp_opt.saw_tstamp &&
	    !tp->rx_opt.cookie_out_never &&
	    (sysctl_tcp_cookie_size > 0 ||
	     (tp->cookie_values != NULL &&
	      tp->cookie_values->cookie_desired > 0))) {
		u8 *c;
		u32 *d;
		u32 *mess = &tmp_ext.cookie_bakery[COOKIE_DIGEST_WORDS];
		int l = tmp_opt.cookie_plus - TCPOLEN_COOKIE_BASE;

		if (tcp_cookie_generator(&tmp_ext.cookie_bakery[0]) != 0)
			goto drop_and_free;

		/* Secret recipe starts with IP addresses */
		d = (__force u32 *)&ipv6_hdr(skb)->daddr.s6_addr32[0];
		*mess++ ^= *d++;
		*mess++ ^= *d++;
		*mess++ ^= *d++;
		*mess++ ^= *d++;
		d = (__force u32 *)&ipv6_hdr(skb)->saddr.s6_addr32[0];
		*mess++ ^= *d++;
		*mess++ ^= *d++;
		*mess++ ^= *d++;
		*mess++ ^= *d++;

		/* plus variable length Initiator Cookie */
		c = (u8 *)mess;
		while (l-- > 0)
			*c++ ^= *hash_location++;

		want_cookie = 0;	/* not our kind of cookie */
		tmp_ext.cookie_out_never = 0; /* false */
		tmp_ext.cookie_plus = tmp_opt.cookie_plus;
	} else if (!tp->rx_opt.cookie_in_always) {
		/* redundant indications, but ensure initialization. */
		tmp_ext.cookie_out_never = 1; /* true */
		tmp_ext.cookie_plus = 0;
	} else {
		goto drop_and_free;
	}
	tmp_ext.cookie_in_always = tp->rx_opt.cookie_in_always;

	if (want_cookie && !tmp_opt.saw_tstamp)
		tcp_clear_options(&tmp_opt);

	tmp_opt.tstamp_ok = tmp_opt.saw_tstamp;
	tcp_openreq_init(req, &tmp_opt, skb);

	treq = inet6_rsk(req);
	treq->rmt_addr = ipv6_hdr(skb)->saddr;
	treq->loc_addr = ipv6_hdr(skb)->daddr;
	if (!want_cookie || tmp_opt.tstamp_ok)
		TCP_ECN_create_request(req, tcp_hdr(skb));

	treq->iif = sk->sk_bound_dev_if;
	inet_rsk(req)->ir_mark = inet_request_mark(sk, skb);

	/* So that link locals have meaning */
	if (!sk->sk_bound_dev_if &&
	    ipv6_addr_type(&treq->rmt_addr) & IPV6_ADDR_LINKLOCAL)
		treq->iif = inet6_iif(skb);

	if (!isn) {
		struct inet_peer *peer = NULL;

		if (ipv6_opt_accepted(sk, skb) ||
		    np->rxopt.bits.rxinfo || np->rxopt.bits.rxoinfo ||
		    np->rxopt.bits.rxhlim || np->rxopt.bits.rxohlim) {
			atomic_inc(&skb->users);
			treq->pktopts = skb;
		}

		if (want_cookie) {
			isn = cookie_v6_init_sequence(sk, skb, &req->mss);
			req->cookie_ts = tmp_opt.tstamp_ok;
			goto have_isn;
		}

		/* VJ's idea. We save last timestamp seen
		 * from the destination in peer table, when entering
		 * state TIME-WAIT, and check against it before
		 * accepting new connection request.
		 *
		 * If "isn" is not zero, this request hit alive
		 * timewait bucket, so that all the necessary checks
		 * are made in the function processing timewait state.
		 */
		if (tmp_opt.saw_tstamp &&
		    tcp_death_row.sysctl_tw_recycle &&
		    (dst = inet6_csk_route_req(sk, req)) != NULL &&
		    (peer = rt6_get_peer((struct rt6_info *)dst)) != NULL &&
		    ipv6_addr_equal((struct in6_addr *)peer->daddr.addr.a6,
				    &treq->rmt_addr)) {
			inet_peer_refcheck(peer);
			if ((u32)get_seconds() - peer->tcp_ts_stamp < TCP_PAWS_MSL &&
			    (s32)(peer->tcp_ts - req->ts_recent) >
							TCP_PAWS_WINDOW) {
				NET_INC_STATS_BH(sock_net(sk), LINUX_MIB_PAWSPASSIVEREJECTED);
				goto drop_and_release;
			}
		}
		/* Kill the following clause, if you dislike this way. */
		else if (!sysctl_tcp_syncookies &&
			 (sysctl_max_syn_backlog - inet_csk_reqsk_queue_len(sk) <
			  (sysctl_max_syn_backlog >> 2)) &&
			 (!peer || !peer->tcp_ts_stamp) &&
			 (!dst || !dst_metric(dst, RTAX_RTT))) {
			/* Without syncookies last quarter of
			 * backlog is filled with destinations,
			 * proven to be alive.
			 * It means that we continue to communicate
			 * to destinations, already remembered
			 * to the moment of synflood.
			 */
			LIMIT_NETDEBUG(KERN_DEBUG "TCP: drop open request from %pI6/%u\n",
				       &treq->rmt_addr, ntohs(tcp_hdr(skb)->source));
			goto drop_and_release;
		}

		isn = tcp_v6_init_sequence(skb);
	}
have_isn:
	tcp_rsk(req)->snt_isn = isn;
	tcp_rsk(req)->snt_synack = tcp_time_stamp;

	security_inet_conn_request(sk, skb, req);

	if (tcp_v6_send_synack(sk, req,
			       (struct request_values *)&tmp_ext) ||
	    want_cookie)
		goto drop_and_free;

	inet6_csk_reqsk_queue_hash_add(sk, req, TCP_TIMEOUT_INIT);
	return 0;

drop_and_release:
	dst_release(dst);
drop_and_free:
	reqsk_free(req);
drop:
	return 0; /* don't send reset */
}

static struct sock * tcp_v6_syn_recv_sock(struct sock *sk, struct sk_buff *skb,
					  struct request_sock *req,
					  struct dst_entry *dst)
{
	struct inet6_request_sock *treq;
	struct ipv6_pinfo *newnp, *np = inet6_sk(sk);
	struct tcp6_sock *newtcp6sk;
	struct inet_sock *newinet;
	struct tcp_sock *newtp;
	struct sock *newsk;
	struct ipv6_txoptions *opt;
#ifdef CONFIG_TCP_MD5SIG
	struct tcp_md5sig_key *key;
#endif
=======
	if (ipv6_addr_v4mapped(&ipv6_hdr(skb)->saddr)) {
		__IP6_INC_STATS(sock_net(sk), NULL, IPSTATS_MIB_INHDRERRORS);
		return 0;
	}

	return tcp_conn_request(&tcp6_request_sock_ops,
				&tcp_request_sock_ipv6_ops, sk, skb);

drop:
	tcp_listendrop(sk);
	return 0; /* don't send reset */
}

static void tcp_v6_restore_cb(struct sk_buff *skb)
{
	/* We need to move header back to the beginning if xfrm6_policy_check()
	 * and tcp_v6_fill_cb() are going to be called again.
	 * ip6_datagram_recv_specific_ctl() also expects IP6CB to be there.
	 */
	memmove(IP6CB(skb), &TCP_SKB_CB(skb)->header.h6,
		sizeof(struct inet6_skb_parm));
}

static struct sock *tcp_v6_syn_recv_sock(const struct sock *sk, struct sk_buff *skb,
					 struct request_sock *req,
					 struct dst_entry *dst,
					 struct request_sock *req_unhash,
					 bool *own_req)
{
	struct inet_request_sock *ireq;
	struct ipv6_pinfo *newnp;
	const struct ipv6_pinfo *np = tcp_inet6_sk(sk);
	struct ipv6_txoptions *opt;
	struct inet_sock *newinet;
	bool found_dup_sk = false;
	struct tcp_sock *newtp;
	struct sock *newsk;
#ifdef CONFIG_TCP_MD5SIG
	struct tcp_md5sig_key *key;
	int l3index;
#endif
	struct flowi6 fl6;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (skb->protocol == htons(ETH_P_IP)) {
		/*
		 *	v6 mapped
		 */

<<<<<<< HEAD
		newsk = tcp_v4_syn_recv_sock(sk, skb, req, dst);

		if (newsk == NULL)
			return NULL;

		newtcp6sk = (struct tcp6_sock *)newsk;
		inet_sk(newsk)->pinet6 = &newtcp6sk->inet6;

		newinet = inet_sk(newsk);
		newnp = inet6_sk(newsk);
=======
		newsk = tcp_v4_syn_recv_sock(sk, skb, req, dst,
					     req_unhash, own_req);

		if (!newsk)
			return NULL;

		inet_sk(newsk)->pinet6 = tcp_inet6_sk(newsk);

		newnp = tcp_inet6_sk(newsk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		newtp = tcp_sk(newsk);

		memcpy(newnp, np, sizeof(struct ipv6_pinfo));

<<<<<<< HEAD
		ipv6_addr_set_v4mapped(newinet->inet_daddr, &newnp->daddr);

		ipv6_addr_set_v4mapped(newinet->inet_saddr, &newnp->saddr);

		newnp->rcv_saddr = newnp->saddr;

		inet_csk(newsk)->icsk_af_ops = &ipv6_mapped;
		newsk->sk_backlog_rcv = tcp_v4_do_rcv;
#ifdef CONFIG_TCP_MD5SIG
=======
		newnp->saddr = newsk->sk_v6_rcv_saddr;

		inet_csk(newsk)->icsk_af_ops = &ipv6_mapped;
		if (sk_is_mptcp(newsk))
			mptcpv6_handle_mapped(newsk, true);
		newsk->sk_backlog_rcv = tcp_v4_do_rcv;
#if defined(CONFIG_TCP_MD5SIG) || defined(CONFIG_TCP_AO)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		newtp->af_specific = &tcp_sock_ipv6_mapped_specific;
#endif

		newnp->ipv6_mc_list = NULL;
		newnp->ipv6_ac_list = NULL;
		newnp->ipv6_fl_list = NULL;
		newnp->pktoptions  = NULL;
		newnp->opt	   = NULL;
<<<<<<< HEAD
		newnp->mcast_oif   = inet6_iif(skb);
		newnp->mcast_hops  = ipv6_hdr(skb)->hop_limit;
		newnp->rcv_tclass  = ipv6_tclass(ipv6_hdr(skb));
=======
		newnp->mcast_oif   = inet_iif(skb);
		newnp->mcast_hops  = ip_hdr(skb)->ttl;
		newnp->rcv_flowinfo = 0;
		if (inet6_test_bit(REPFLOW, sk))
			newnp->flow_label = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/*
		 * No need to charge this sock to the relevant IPv6 refcnt debug socks count
		 * here, tcp_create_openreq_child now does this for us, see the comment in
		 * that function for the gory details. -acme
		 */

		/* It is tricky place. Until this moment IPv4 tcp
		   worked with IPv6 icsk.icsk_af_ops.
		   Sync it now.
		 */
		tcp_sync_mss(newsk, inet_csk(newsk)->icsk_pmtu_cookie);

		return newsk;
	}

<<<<<<< HEAD
	treq = inet6_rsk(req);
	opt = np->opt;
=======
	ireq = inet_rsk(req);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (sk_acceptq_is_full(sk))
		goto out_overflow;

	if (!dst) {
<<<<<<< HEAD
		dst = inet6_csk_route_req(sk, req);
=======
		dst = inet6_csk_route_req(sk, &fl6, req, IPPROTO_TCP);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!dst)
			goto out;
	}

	newsk = tcp_create_openreq_child(sk, req, skb);
<<<<<<< HEAD
	if (newsk == NULL)
=======
	if (!newsk)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out_nonewsk;

	/*
	 * No need to charge this sock to the relevant IPv6 refcnt debug socks
	 * count here, tcp_create_openreq_child now does this for us, see the
	 * comment in that function for the gory details. -acme
	 */

	newsk->sk_gso_type = SKB_GSO_TCPV6;
<<<<<<< HEAD
	__ip6_dst_store(newsk, dst, NULL, NULL);

	newtcp6sk = (struct tcp6_sock *)newsk;
	inet_sk(newsk)->pinet6 = &newtcp6sk->inet6;

	newtp = tcp_sk(newsk);
	newinet = inet_sk(newsk);
	newnp = inet6_sk(newsk);

	memcpy(newnp, np, sizeof(struct ipv6_pinfo));

	newnp->daddr = treq->rmt_addr;
	newnp->saddr = treq->loc_addr;
	newnp->rcv_saddr = treq->loc_addr;
	newsk->sk_bound_dev_if = treq->iif;
=======
	ip6_dst_store(newsk, dst, NULL, NULL);
	inet6_sk_rx_dst_set(newsk, skb);

	inet_sk(newsk)->pinet6 = tcp_inet6_sk(newsk);

	newtp = tcp_sk(newsk);
	newinet = inet_sk(newsk);
	newnp = tcp_inet6_sk(newsk);

	memcpy(newnp, np, sizeof(struct ipv6_pinfo));

	newsk->sk_v6_daddr = ireq->ir_v6_rmt_addr;
	newnp->saddr = ireq->ir_v6_loc_addr;
	newsk->sk_v6_rcv_saddr = ireq->ir_v6_loc_addr;
	newsk->sk_bound_dev_if = ireq->ir_iif;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Now IPv6 options...

	   First: no IPv4 options.
	 */
	newinet->inet_opt = NULL;
	newnp->ipv6_mc_list = NULL;
	newnp->ipv6_ac_list = NULL;
	newnp->ipv6_fl_list = NULL;

	/* Clone RX bits */
	newnp->rxopt.all = np->rxopt.all;

<<<<<<< HEAD
	/* Clone pktoptions received with SYN */
	newnp->pktoptions = NULL;
	if (treq->pktopts != NULL) {
		newnp->pktoptions = skb_clone(treq->pktopts, GFP_ATOMIC);
		kfree_skb(treq->pktopts);
		treq->pktopts = NULL;
		if (newnp->pktoptions)
			skb_set_owner_r(newnp->pktoptions, newsk);
	}
	newnp->opt	  = NULL;
	newnp->mcast_oif  = inet6_iif(skb);
	newnp->mcast_hops = ipv6_hdr(skb)->hop_limit;
	newnp->rcv_tclass = ipv6_tclass(ipv6_hdr(skb));
=======
	newnp->pktoptions = NULL;
	newnp->opt	  = NULL;
	newnp->mcast_oif  = tcp_v6_iif(skb);
	newnp->mcast_hops = ipv6_hdr(skb)->hop_limit;
	newnp->rcv_flowinfo = ip6_flowinfo(ipv6_hdr(skb));
	if (inet6_test_bit(REPFLOW, sk))
		newnp->flow_label = ip6_flowlabel(ipv6_hdr(skb));

	/* Set ToS of the new socket based upon the value of incoming SYN.
	 * ECT bits are set later in tcp_init_transfer().
	 */
	if (READ_ONCE(sock_net(sk)->ipv4.sysctl_tcp_reflect_tos))
		newnp->tclass = tcp_rsk(req)->syn_tos & ~INET_ECN_MASK;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Clone native IPv6 options from listening socket (if any)

	   Yes, keeping reference count would be much more clever,
	   but we make one more one thing there: reattach optmem
	   to newsk.
	 */
<<<<<<< HEAD
	if (opt) {
		newnp->opt = ipv6_dup_options(newsk, opt);
		if (opt != np->opt)
			sock_kfree_s(sk, opt, opt->tot_len);
	}

	inet_csk(newsk)->icsk_ext_hdr_len = 0;
	if (newnp->opt)
		inet_csk(newsk)->icsk_ext_hdr_len = (newnp->opt->opt_nflen +
						     newnp->opt->opt_flen);

	tcp_mtup_init(newsk);
	tcp_sync_mss(newsk, dst_mtu(dst));
	newtp->advmss = dst_metric_advmss(dst);
	if (tcp_sk(sk)->rx_opt.user_mss &&
	    tcp_sk(sk)->rx_opt.user_mss < newtp->advmss)
		newtp->advmss = tcp_sk(sk)->rx_opt.user_mss;

	tcp_initialize_rcv_mss(newsk);
	if (tcp_rsk(req)->snt_synack)
		tcp_valid_rtt_meas(newsk,
		    tcp_time_stamp - tcp_rsk(req)->snt_synack);
	newtp->total_retrans = req->retrans;
=======
	opt = ireq->ipv6_opt;
	if (!opt)
		opt = rcu_dereference(np->opt);
	if (opt) {
		opt = ipv6_dup_options(newsk, opt);
		RCU_INIT_POINTER(newnp->opt, opt);
	}
	inet_csk(newsk)->icsk_ext_hdr_len = 0;
	if (opt)
		inet_csk(newsk)->icsk_ext_hdr_len = opt->opt_nflen +
						    opt->opt_flen;

	tcp_ca_openreq_child(newsk, dst);

	tcp_sync_mss(newsk, dst_mtu(dst));
	newtp->advmss = tcp_mss_clamp(tcp_sk(sk), dst_metric_advmss(dst));

	tcp_initialize_rcv_mss(newsk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	newinet->inet_daddr = newinet->inet_saddr = LOOPBACK4_IPV6;
	newinet->inet_rcv_saddr = LOOPBACK4_IPV6;

#ifdef CONFIG_TCP_MD5SIG
<<<<<<< HEAD
	/* Copy over the MD5 key from the original socket */
	if ((key = tcp_v6_md5_do_lookup(sk, &newnp->daddr)) != NULL) {
		/* We're using one, so create a matching key
		 * on the newsk structure. If we fail to get
		 * memory, then we end up not copying the key
		 * across. Shucks.
		 */
		tcp_md5_do_add(newsk, (union tcp_md5_addr *)&newnp->daddr,
			       AF_INET6, key->key, key->keylen, GFP_ATOMIC);
	}
#endif
=======
	l3index = l3mdev_master_ifindex_by_index(sock_net(sk), ireq->ir_iif);

	if (!tcp_rsk_used_ao(req)) {
		/* Copy over the MD5 key from the original socket */
		key = tcp_v6_md5_do_lookup(sk, &newsk->sk_v6_daddr, l3index);
		if (key) {
			const union tcp_md5_addr *addr;

			addr = (union tcp_md5_addr *)&newsk->sk_v6_daddr;
			if (tcp_md5_key_copy(newsk, addr, AF_INET6, 128, l3index, key)) {
				inet_csk_prepare_forced_close(newsk);
				tcp_done(newsk);
				goto out;
			}
		}
	}
#endif
#ifdef CONFIG_TCP_AO
	/* Copy over tcp_ao_info if any */
	if (tcp_ao_copy_all_matching(sk, newsk, req, skb, AF_INET6))
		goto out; /* OOM */
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (__inet_inherit_port(sk, newsk) < 0) {
		inet_csk_prepare_forced_close(newsk);
		tcp_done(newsk);
		goto out;
	}
<<<<<<< HEAD
	__inet6_hash(newsk, NULL);
=======
	*own_req = inet_ehash_nolisten(newsk, req_to_sk(req_unhash),
				       &found_dup_sk);
	if (*own_req) {
		tcp_move_syn(newtp, req);

		/* Clone pktoptions received with SYN, if we own the req */
		if (ireq->pktopts) {
			newnp->pktoptions = skb_clone_and_charge_r(ireq->pktopts, newsk);
			consume_skb(ireq->pktopts);
			ireq->pktopts = NULL;
			if (newnp->pktoptions)
				tcp_v6_restore_cb(newnp->pktoptions);
		}
	} else {
		if (!req_unhash && found_dup_sk) {
			/* This code path should only be executed in the
			 * syncookie case only
			 */
			bh_unlock_sock(newsk);
			sock_put(newsk);
			newsk = NULL;
		}
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return newsk;

out_overflow:
<<<<<<< HEAD
	NET_INC_STATS_BH(sock_net(sk), LINUX_MIB_LISTENOVERFLOWS);
out_nonewsk:
	if (opt && opt != np->opt)
		sock_kfree_s(sk, opt, opt->tot_len);
	dst_release(dst);
out:
	NET_INC_STATS_BH(sock_net(sk), LINUX_MIB_LISTENDROPS);
	return NULL;
}

static __sum16 tcp_v6_checksum_init(struct sk_buff *skb)
{
	if (skb->ip_summed == CHECKSUM_COMPLETE) {
		if (!tcp_v6_check(skb->len, &ipv6_hdr(skb)->saddr,
				  &ipv6_hdr(skb)->daddr, skb->csum)) {
			skb->ip_summed = CHECKSUM_UNNECESSARY;
			return 0;
		}
	}

	skb->csum = ~csum_unfold(tcp_v6_check(skb->len,
					      &ipv6_hdr(skb)->saddr,
					      &ipv6_hdr(skb)->daddr, 0));

	if (skb->len <= 76) {
		return __skb_checksum_complete(skb);
	}
	return 0;
}

/* The socket must have it's spinlock held when we get
 * here.
=======
	__NET_INC_STATS(sock_net(sk), LINUX_MIB_LISTENOVERFLOWS);
out_nonewsk:
	dst_release(dst);
out:
	tcp_listendrop(sk);
	return NULL;
}

INDIRECT_CALLABLE_DECLARE(struct dst_entry *ipv4_dst_check(struct dst_entry *,
							   u32));
/* The socket must have it's spinlock held when we get
 * here, unless it is a TCP_LISTEN socket.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * We have a potential double-lock case here, so even when
 * doing backlog processing we use the BH locking scheme.
 * This is because we cannot sleep with the original spinlock
 * held.
 */
<<<<<<< HEAD
static int tcp_v6_do_rcv(struct sock *sk, struct sk_buff *skb)
{
	struct ipv6_pinfo *np = inet6_sk(sk);
	struct tcp_sock *tp;
	struct sk_buff *opt_skb = NULL;
=======
INDIRECT_CALLABLE_SCOPE
int tcp_v6_do_rcv(struct sock *sk, struct sk_buff *skb)
{
	struct ipv6_pinfo *np = tcp_inet6_sk(sk);
	struct sk_buff *opt_skb = NULL;
	enum skb_drop_reason reason;
	struct tcp_sock *tp;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Imagine: socket is IPv6. IPv4 packet arrives,
	   goes to IPv4 receive handler and backlogged.
	   From backlog it always goes here. Kerboom...
	   Fortunately, tcp_rcv_established and rcv_established
	   handle them correctly, but it is not case with
	   tcp_v6_hnd_req and tcp_v6_send_reset().   --ANK
	 */

	if (skb->protocol == htons(ETH_P_IP))
		return tcp_v4_do_rcv(sk, skb);

<<<<<<< HEAD
#ifdef CONFIG_TCP_MD5SIG
	if (tcp_v6_inbound_md5_hash (sk, skb))
		goto discard;
#endif

	if (sk_filter(sk, skb))
		goto discard;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 *	socket locking is here for SMP purposes as backlog rcv
	 *	is currently called with bh processing disabled.
	 */

	/* Do Stevens' IPV6_PKTOPTIONS.

	   Yes, guys, it is the only place in our code, where we
	   may make it not affecting IPv4.
	   The rest of code is protocol independent,
	   and I do not like idea to uglify IPv4.

	   Actually, all the idea behind IPV6_PKTOPTIONS
	   looks not very well thought. For now we latch
	   options, received in the last packet, enqueued
	   by tcp. Feel free to propose better solution.
					       --ANK (980728)
	 */
	if (np->rxopt.all)
<<<<<<< HEAD
		opt_skb = skb_clone(skb, GFP_ATOMIC);

	if (sk->sk_state == TCP_ESTABLISHED) { /* Fast path */
		sock_rps_save_rxhash(sk, skb);
		if (tcp_rcv_established(sk, skb, tcp_hdr(skb), skb->len))
			goto reset;
=======
		opt_skb = skb_clone_and_charge_r(skb, sk);

	if (sk->sk_state == TCP_ESTABLISHED) { /* Fast path */
		struct dst_entry *dst;

		dst = rcu_dereference_protected(sk->sk_rx_dst,
						lockdep_sock_is_held(sk));

		sock_rps_save_rxhash(sk, skb);
		sk_mark_napi_id(sk, skb);
		if (dst) {
			if (sk->sk_rx_dst_ifindex != skb->skb_iif ||
			    INDIRECT_CALL_1(dst->ops->check, ip6_dst_check,
					    dst, sk->sk_rx_dst_cookie) == NULL) {
				RCU_INIT_POINTER(sk->sk_rx_dst, NULL);
				dst_release(dst);
			}
		}

		tcp_rcv_established(sk, skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (opt_skb)
			goto ipv6_pktoptions;
		return 0;
	}

<<<<<<< HEAD
	if (skb->len < tcp_hdrlen(skb) || tcp_checksum_complete(skb))
		goto csum_err;

	if (sk->sk_state == TCP_LISTEN) {
		struct sock *nsk = tcp_v6_hnd_req(sk, skb);
		if (!nsk)
			goto discard;

		/*
		 * Queue it on the new socket if the new socket is active,
		 * otherwise we just shortcircuit this and continue with
		 * the new socket..
		 */
		if(nsk != sk) {
			sock_rps_save_rxhash(nsk, skb);
			if (tcp_child_process(sk, nsk, skb))
				goto reset;
=======
	if (tcp_checksum_complete(skb))
		goto csum_err;

	if (sk->sk_state == TCP_LISTEN) {
		struct sock *nsk = tcp_v6_cookie_check(sk, skb);

		if (nsk != sk) {
			if (nsk) {
				reason = tcp_child_process(sk, nsk, skb);
				if (reason)
					goto reset;
			}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (opt_skb)
				__kfree_skb(opt_skb);
			return 0;
		}
	} else
		sock_rps_save_rxhash(sk, skb);

<<<<<<< HEAD
	if (tcp_rcv_state_process(sk, skb, tcp_hdr(skb), skb->len))
=======
	reason = tcp_rcv_state_process(sk, skb);
	if (reason)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto reset;
	if (opt_skb)
		goto ipv6_pktoptions;
	return 0;

reset:
	tcp_v6_send_reset(sk, skb);
discard:
	if (opt_skb)
		__kfree_skb(opt_skb);
<<<<<<< HEAD
	kfree_skb(skb);
	return 0;
csum_err:
	TCP_INC_STATS_BH(sock_net(sk), TCP_MIB_INERRS);
=======
	kfree_skb_reason(skb, reason);
	return 0;
csum_err:
	reason = SKB_DROP_REASON_TCP_CSUM;
	trace_tcp_bad_csum(skb);
	TCP_INC_STATS(sock_net(sk), TCP_MIB_CSUMERRORS);
	TCP_INC_STATS(sock_net(sk), TCP_MIB_INERRS);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	goto discard;


ipv6_pktoptions:
	/* Do you ask, what is it?

	   1. skb was enqueued by tcp.
	   2. skb is added to tail of read queue, rather than out of order.
	   3. socket is not in passive state.
	   4. Finally, it really contains options, which user wants to receive.
	 */
	tp = tcp_sk(sk);
	if (TCP_SKB_CB(opt_skb)->end_seq == tp->rcv_nxt &&
	    !((1 << sk->sk_state) & (TCPF_CLOSE | TCPF_LISTEN))) {
		if (np->rxopt.bits.rxinfo || np->rxopt.bits.rxoinfo)
<<<<<<< HEAD
			np->mcast_oif = inet6_iif(opt_skb);
		if (np->rxopt.bits.rxhlim || np->rxopt.bits.rxohlim)
			np->mcast_hops = ipv6_hdr(opt_skb)->hop_limit;
		if (np->rxopt.bits.rxtclass)
			np->rcv_tclass = ipv6_tclass(ipv6_hdr(opt_skb));
		if (ipv6_opt_accepted(sk, opt_skb)) {
			skb_set_owner_r(opt_skb, sk);
=======
			WRITE_ONCE(np->mcast_oif, tcp_v6_iif(opt_skb));
		if (np->rxopt.bits.rxhlim || np->rxopt.bits.rxohlim)
			WRITE_ONCE(np->mcast_hops,
				   ipv6_hdr(opt_skb)->hop_limit);
		if (np->rxopt.bits.rxflow || np->rxopt.bits.rxtclass)
			np->rcv_flowinfo = ip6_flowinfo(ipv6_hdr(opt_skb));
		if (inet6_test_bit(REPFLOW, sk))
			np->flow_label = ip6_flowlabel(ipv6_hdr(opt_skb));
		if (ipv6_opt_accepted(sk, opt_skb, &TCP_SKB_CB(opt_skb)->header.h6)) {
			tcp_v6_restore_cb(opt_skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			opt_skb = xchg(&np->pktoptions, opt_skb);
		} else {
			__kfree_skb(opt_skb);
			opt_skb = xchg(&np->pktoptions, NULL);
		}
	}

<<<<<<< HEAD
	kfree_skb(opt_skb);
	return 0;
}

static int tcp_v6_rcv(struct sk_buff *skb)
{
	const struct tcphdr *th;
	const struct ipv6hdr *hdr;
=======
	consume_skb(opt_skb);
	return 0;
}

static void tcp_v6_fill_cb(struct sk_buff *skb, const struct ipv6hdr *hdr,
			   const struct tcphdr *th)
{
	/* This is tricky: we move IP6CB at its correct location into
	 * TCP_SKB_CB(). It must be done after xfrm6_policy_check(), because
	 * _decode_session6() uses IP6CB().
	 * barrier() makes sure compiler won't play aliasing games.
	 */
	memmove(&TCP_SKB_CB(skb)->header.h6, IP6CB(skb),
		sizeof(struct inet6_skb_parm));
	barrier();

	TCP_SKB_CB(skb)->seq = ntohl(th->seq);
	TCP_SKB_CB(skb)->end_seq = (TCP_SKB_CB(skb)->seq + th->syn + th->fin +
				    skb->len - th->doff*4);
	TCP_SKB_CB(skb)->ack_seq = ntohl(th->ack_seq);
	TCP_SKB_CB(skb)->tcp_flags = tcp_flag_byte(th);
	TCP_SKB_CB(skb)->tcp_tw_isn = 0;
	TCP_SKB_CB(skb)->ip_dsfield = ipv6_get_dsfield(hdr);
	TCP_SKB_CB(skb)->sacked = 0;
	TCP_SKB_CB(skb)->has_rxtstamp =
			skb->tstamp || skb_hwtstamps(skb)->hwtstamp;
}

INDIRECT_CALLABLE_SCOPE int tcp_v6_rcv(struct sk_buff *skb)
{
	enum skb_drop_reason drop_reason;
	int sdif = inet6_sdif(skb);
	int dif = inet6_iif(skb);
	const struct tcphdr *th;
	const struct ipv6hdr *hdr;
	bool refcounted;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct sock *sk;
	int ret;
	struct net *net = dev_net(skb->dev);

<<<<<<< HEAD
=======
	drop_reason = SKB_DROP_REASON_NOT_SPECIFIED;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (skb->pkt_type != PACKET_HOST)
		goto discard_it;

	/*
	 *	Count it even if it's bad.
	 */
<<<<<<< HEAD
	TCP_INC_STATS_BH(net, TCP_MIB_INSEGS);
=======
	__TCP_INC_STATS(net, TCP_MIB_INSEGS);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!pskb_may_pull(skb, sizeof(struct tcphdr)))
		goto discard_it;

<<<<<<< HEAD
	th = tcp_hdr(skb);

	if (th->doff < sizeof(struct tcphdr)/4)
		goto bad_packet;
	if (!pskb_may_pull(skb, th->doff*4))
		goto discard_it;

	if (!skb_csum_unnecessary(skb) && tcp_v6_checksum_init(skb))
		goto bad_packet;

	th = tcp_hdr(skb);
	hdr = ipv6_hdr(skb);
	TCP_SKB_CB(skb)->seq = ntohl(th->seq);
	TCP_SKB_CB(skb)->end_seq = (TCP_SKB_CB(skb)->seq + th->syn + th->fin +
				    skb->len - th->doff*4);
	TCP_SKB_CB(skb)->ack_seq = ntohl(th->ack_seq);
	TCP_SKB_CB(skb)->when = 0;
	TCP_SKB_CB(skb)->ip_dsfield = ipv6_get_dsfield(hdr);
	TCP_SKB_CB(skb)->sacked = 0;

	sk = __inet6_lookup_skb(&tcp_hashinfo, skb, th->source, th->dest);
=======
	th = (const struct tcphdr *)skb->data;

	if (unlikely(th->doff < sizeof(struct tcphdr) / 4)) {
		drop_reason = SKB_DROP_REASON_PKT_TOO_SMALL;
		goto bad_packet;
	}
	if (!pskb_may_pull(skb, th->doff*4))
		goto discard_it;

	if (skb_checksum_init(skb, IPPROTO_TCP, ip6_compute_pseudo))
		goto csum_error;

	th = (const struct tcphdr *)skb->data;
	hdr = ipv6_hdr(skb);

lookup:
	sk = __inet6_lookup_skb(net->ipv4.tcp_death_row.hashinfo, skb, __tcp_hdrlen(th),
				th->source, th->dest, inet6_iif(skb), sdif,
				&refcounted);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!sk)
		goto no_tcp_socket;

process:
	if (sk->sk_state == TCP_TIME_WAIT)
		goto do_time_wait;

<<<<<<< HEAD
	if (hdr->hop_limit < inet6_sk(sk)->min_hopcount) {
		NET_INC_STATS_BH(net, LINUX_MIB_TCPMINTTLDROP);
		goto discard_and_relse;
	}

	if (!xfrm6_policy_check(sk, XFRM_POLICY_IN, skb))
		goto discard_and_relse;

	if (sk_filter(sk, skb))
		goto discard_and_relse;

	skb->dev = NULL;

	bh_lock_sock_nested(sk);
	tcp_sk(sk)->segs_in += max_t(u16, 1, skb_shinfo(skb)->gso_segs);
	ret = 0;
	if (!sock_owned_by_user(sk)) {
#ifdef CONFIG_NET_DMA
		struct tcp_sock *tp = tcp_sk(sk);
		if (!tp->ucopy.dma_chan && tp->ucopy.pinned_list)
			tp->ucopy.dma_chan = net_dma_find_channel();
		if (tp->ucopy.dma_chan)
			ret = tcp_v6_do_rcv(sk, skb);
		else
#endif
		{
			if (!tcp_prequeue(sk, skb))
				ret = tcp_v6_do_rcv(sk, skb);
		}
	} else if (unlikely(sk_add_backlog(sk, skb))) {
		bh_unlock_sock(sk);
		NET_INC_STATS_BH(net, LINUX_MIB_TCPBACKLOGDROP);
		goto discard_and_relse;
	}
	bh_unlock_sock(sk);

	sock_put(sk);
	return ret ? -1 : 0;

no_tcp_socket:
	if (!xfrm6_policy_check(NULL, XFRM_POLICY_IN, skb))
		goto discard_it;

	if (skb->len < (th->doff<<2) || tcp_checksum_complete(skb)) {
bad_packet:
		TCP_INC_STATS_BH(net, TCP_MIB_INERRS);
=======
	if (sk->sk_state == TCP_NEW_SYN_RECV) {
		struct request_sock *req = inet_reqsk(sk);
		bool req_stolen = false;
		struct sock *nsk;

		sk = req->rsk_listener;
		if (!xfrm6_policy_check(sk, XFRM_POLICY_IN, skb))
			drop_reason = SKB_DROP_REASON_XFRM_POLICY;
		else
			drop_reason = tcp_inbound_hash(sk, req, skb,
						       &hdr->saddr, &hdr->daddr,
						       AF_INET6, dif, sdif);
		if (drop_reason) {
			sk_drops_add(sk, skb);
			reqsk_put(req);
			goto discard_it;
		}
		if (tcp_checksum_complete(skb)) {
			reqsk_put(req);
			goto csum_error;
		}
		if (unlikely(sk->sk_state != TCP_LISTEN)) {
			nsk = reuseport_migrate_sock(sk, req_to_sk(req), skb);
			if (!nsk) {
				inet_csk_reqsk_queue_drop_and_put(sk, req);
				goto lookup;
			}
			sk = nsk;
			/* reuseport_migrate_sock() has already held one sk_refcnt
			 * before returning.
			 */
		} else {
			sock_hold(sk);
		}
		refcounted = true;
		nsk = NULL;
		if (!tcp_filter(sk, skb)) {
			th = (const struct tcphdr *)skb->data;
			hdr = ipv6_hdr(skb);
			tcp_v6_fill_cb(skb, hdr, th);
			nsk = tcp_check_req(sk, skb, req, false, &req_stolen);
		} else {
			drop_reason = SKB_DROP_REASON_SOCKET_FILTER;
		}
		if (!nsk) {
			reqsk_put(req);
			if (req_stolen) {
				/* Another cpu got exclusive access to req
				 * and created a full blown socket.
				 * Try to feed this packet to this socket
				 * instead of discarding it.
				 */
				tcp_v6_restore_cb(skb);
				sock_put(sk);
				goto lookup;
			}
			goto discard_and_relse;
		}
		nf_reset_ct(skb);
		if (nsk == sk) {
			reqsk_put(req);
			tcp_v6_restore_cb(skb);
		} else {
			drop_reason = tcp_child_process(sk, nsk, skb);
			if (drop_reason) {
				tcp_v6_send_reset(nsk, skb);
				goto discard_and_relse;
			}
			sock_put(sk);
			return 0;
		}
	}

	if (static_branch_unlikely(&ip6_min_hopcount)) {
		/* min_hopcount can be changed concurrently from do_ipv6_setsockopt() */
		if (unlikely(hdr->hop_limit < READ_ONCE(tcp_inet6_sk(sk)->min_hopcount))) {
			__NET_INC_STATS(net, LINUX_MIB_TCPMINTTLDROP);
			drop_reason = SKB_DROP_REASON_TCP_MINTTL;
			goto discard_and_relse;
		}
	}

	if (!xfrm6_policy_check(sk, XFRM_POLICY_IN, skb)) {
		drop_reason = SKB_DROP_REASON_XFRM_POLICY;
		goto discard_and_relse;
	}

	drop_reason = tcp_inbound_hash(sk, NULL, skb, &hdr->saddr, &hdr->daddr,
				       AF_INET6, dif, sdif);
	if (drop_reason)
		goto discard_and_relse;

	nf_reset_ct(skb);

	if (tcp_filter(sk, skb)) {
		drop_reason = SKB_DROP_REASON_SOCKET_FILTER;
		goto discard_and_relse;
	}
	th = (const struct tcphdr *)skb->data;
	hdr = ipv6_hdr(skb);
	tcp_v6_fill_cb(skb, hdr, th);

	skb->dev = NULL;

	if (sk->sk_state == TCP_LISTEN) {
		ret = tcp_v6_do_rcv(sk, skb);
		goto put_and_return;
	}

	sk_incoming_cpu_update(sk);

	bh_lock_sock_nested(sk);
	tcp_segs_in(tcp_sk(sk), skb);
	ret = 0;
	if (!sock_owned_by_user(sk)) {
		ret = tcp_v6_do_rcv(sk, skb);
	} else {
		if (tcp_add_backlog(sk, skb, &drop_reason))
			goto discard_and_relse;
	}
	bh_unlock_sock(sk);
put_and_return:
	if (refcounted)
		sock_put(sk);
	return ret ? -1 : 0;

no_tcp_socket:
	drop_reason = SKB_DROP_REASON_NO_SOCKET;
	if (!xfrm6_policy_check(NULL, XFRM_POLICY_IN, skb))
		goto discard_it;

	tcp_v6_fill_cb(skb, hdr, th);

	if (tcp_checksum_complete(skb)) {
csum_error:
		drop_reason = SKB_DROP_REASON_TCP_CSUM;
		trace_tcp_bad_csum(skb);
		__TCP_INC_STATS(net, TCP_MIB_CSUMERRORS);
bad_packet:
		__TCP_INC_STATS(net, TCP_MIB_INERRS);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		tcp_v6_send_reset(NULL, skb);
	}

discard_it:
<<<<<<< HEAD

	/*
	 *	Discard frame
	 */

	kfree_skb(skb);
	return 0;

discard_and_relse:
	sock_put(sk);
=======
	SKB_DR_OR(drop_reason, NOT_SPECIFIED);
	kfree_skb_reason(skb, drop_reason);
	return 0;

discard_and_relse:
	sk_drops_add(sk, skb);
	if (refcounted)
		sock_put(sk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	goto discard_it;

do_time_wait:
	if (!xfrm6_policy_check(NULL, XFRM_POLICY_IN, skb)) {
<<<<<<< HEAD
=======
		drop_reason = SKB_DROP_REASON_XFRM_POLICY;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		inet_twsk_put(inet_twsk(sk));
		goto discard_it;
	}

<<<<<<< HEAD
	if (skb->len < (th->doff<<2) || tcp_checksum_complete(skb)) {
		TCP_INC_STATS_BH(net, TCP_MIB_INERRS);
		inet_twsk_put(inet_twsk(sk));
		goto discard_it;
=======
	tcp_v6_fill_cb(skb, hdr, th);

	if (tcp_checksum_complete(skb)) {
		inet_twsk_put(inet_twsk(sk));
		goto csum_error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	switch (tcp_timewait_state_process(inet_twsk(sk), skb, th)) {
	case TCP_TW_SYN:
	{
		struct sock *sk2;

<<<<<<< HEAD
		sk2 = inet6_lookup_listener(dev_net(skb->dev), &tcp_hashinfo,
					    &ipv6_hdr(skb)->daddr,
					    ntohs(th->dest), inet6_iif(skb));
		if (sk2 != NULL) {
			struct inet_timewait_sock *tw = inet_twsk(sk);
			inet_twsk_deschedule(tw, &tcp_death_row);
			inet_twsk_put(tw);
			sk = sk2;
			goto process;
		}
		/* Fall through to ACK */
	}
=======
		sk2 = inet6_lookup_listener(net, net->ipv4.tcp_death_row.hashinfo,
					    skb, __tcp_hdrlen(th),
					    &ipv6_hdr(skb)->saddr, th->source,
					    &ipv6_hdr(skb)->daddr,
					    ntohs(th->dest),
					    tcp_v6_iif_l3_slave(skb),
					    sdif);
		if (sk2) {
			struct inet_timewait_sock *tw = inet_twsk(sk);
			inet_twsk_deschedule_put(tw);
			sk = sk2;
			tcp_v6_restore_cb(skb);
			refcounted = false;
			goto process;
		}
	}
		/* to ACK */
		fallthrough;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case TCP_TW_ACK:
		tcp_v6_timewait_ack(sk, skb);
		break;
	case TCP_TW_RST:
<<<<<<< HEAD
		goto no_tcp_socket;
	case TCP_TW_SUCCESS:;
=======
		tcp_v6_send_reset(sk, skb);
		inet_twsk_deschedule_put(inet_twsk(sk));
		goto discard_it;
	case TCP_TW_SUCCESS:
		;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	goto discard_it;
}

<<<<<<< HEAD
static struct inet_peer *tcp_v6_get_peer(struct sock *sk, bool *release_it)
{
	struct rt6_info *rt = (struct rt6_info *) __sk_dst_get(sk);
	struct ipv6_pinfo *np = inet6_sk(sk);
	struct inet_peer *peer;

	if (!rt ||
	    !ipv6_addr_equal(&np->daddr, &rt->rt6i_dst.addr)) {
		peer = inet_getpeer_v6(&np->daddr, 1);
		*release_it = true;
	} else {
		if (!rt->rt6i_peer)
			rt6_bind_peer(rt, 1);
		peer = rt->rt6i_peer;
		*release_it = false;
	}

	return peer;
}

static void *tcp_v6_tw_get_peer(struct sock *sk)
{
	const struct inet6_timewait_sock *tw6 = inet6_twsk(sk);
	const struct inet_timewait_sock *tw = inet_twsk(sk);

	if (tw->tw_family == AF_INET)
		return tcp_v4_tw_get_peer(sk);

	return inet_getpeer_v6(&tw6->tw_v6_daddr, 1);
=======
void tcp_v6_early_demux(struct sk_buff *skb)
{
	struct net *net = dev_net(skb->dev);
	const struct ipv6hdr *hdr;
	const struct tcphdr *th;
	struct sock *sk;

	if (skb->pkt_type != PACKET_HOST)
		return;

	if (!pskb_may_pull(skb, skb_transport_offset(skb) + sizeof(struct tcphdr)))
		return;

	hdr = ipv6_hdr(skb);
	th = tcp_hdr(skb);

	if (th->doff < sizeof(struct tcphdr) / 4)
		return;

	/* Note : We use inet6_iif() here, not tcp_v6_iif() */
	sk = __inet6_lookup_established(net, net->ipv4.tcp_death_row.hashinfo,
					&hdr->saddr, th->source,
					&hdr->daddr, ntohs(th->dest),
					inet6_iif(skb), inet6_sdif(skb));
	if (sk) {
		skb->sk = sk;
		skb->destructor = sock_edemux;
		if (sk_fullsock(sk)) {
			struct dst_entry *dst = rcu_dereference(sk->sk_rx_dst);

			if (dst)
				dst = dst_check(dst, sk->sk_rx_dst_cookie);
			if (dst &&
			    sk->sk_rx_dst_ifindex == skb->skb_iif)
				skb_dst_set_noref(skb, dst);
		}
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct timewait_sock_ops tcp6_timewait_sock_ops = {
	.twsk_obj_size	= sizeof(struct tcp6_timewait_sock),
	.twsk_unique	= tcp_twsk_unique,
<<<<<<< HEAD
	.twsk_destructor= tcp_twsk_destructor,
	.twsk_getpeer	= tcp_v6_tw_get_peer,
};

static const struct inet_connection_sock_af_ops ipv6_specific = {
	.queue_xmit	   = inet6_csk_xmit,
	.send_check	   = tcp_v6_send_check,
	.rebuild_header	   = inet6_sk_rebuild_header,
	.conn_request	   = tcp_v6_conn_request,
	.syn_recv_sock	   = tcp_v6_syn_recv_sock,
	.get_peer	   = tcp_v6_get_peer,
=======
	.twsk_destructor = tcp_twsk_destructor,
};

INDIRECT_CALLABLE_SCOPE void tcp_v6_send_check(struct sock *sk, struct sk_buff *skb)
{
	__tcp_v6_send_check(skb, &sk->sk_v6_rcv_saddr, &sk->sk_v6_daddr);
}

const struct inet_connection_sock_af_ops ipv6_specific = {
	.queue_xmit	   = inet6_csk_xmit,
	.send_check	   = tcp_v6_send_check,
	.rebuild_header	   = inet6_sk_rebuild_header,
	.sk_rx_dst_set	   = inet6_sk_rx_dst_set,
	.conn_request	   = tcp_v6_conn_request,
	.syn_recv_sock	   = tcp_v6_syn_recv_sock,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.net_header_len	   = sizeof(struct ipv6hdr),
	.setsockopt	   = ipv6_setsockopt,
	.getsockopt	   = ipv6_getsockopt,
	.addr2sockaddr	   = inet6_csk_addr2sockaddr,
	.sockaddr_len	   = sizeof(struct sockaddr_in6),
<<<<<<< HEAD
	.bind_conflict	   = inet6_csk_bind_conflict,
#ifdef CONFIG_COMPAT
	.compat_setsockopt = compat_ipv6_setsockopt,
	.compat_getsockopt = compat_ipv6_getsockopt,
#endif
};

#ifdef CONFIG_TCP_MD5SIG
static const struct tcp_sock_af_ops tcp_sock_ipv6_specific = {
	.md5_lookup	=	tcp_v6_md5_lookup,
	.calc_md5_hash	=	tcp_v6_md5_hash_skb,
	.md5_parse	=	tcp_v6_parse_md5_keys,
=======
	.mtu_reduced	   = tcp_v6_mtu_reduced,
};

#if defined(CONFIG_TCP_MD5SIG) || defined(CONFIG_TCP_AO)
static const struct tcp_sock_af_ops tcp_sock_ipv6_specific = {
#ifdef CONFIG_TCP_MD5SIG
	.md5_lookup	=	tcp_v6_md5_lookup,
	.calc_md5_hash	=	tcp_v6_md5_hash_skb,
	.md5_parse	=	tcp_v6_parse_md5_keys,
#endif
#ifdef CONFIG_TCP_AO
	.ao_lookup	=	tcp_v6_ao_lookup,
	.calc_ao_hash	=	tcp_v6_ao_hash_skb,
	.ao_parse	=	tcp_v6_parse_ao,
	.ao_calc_key_sk	=	tcp_v6_ao_calc_key_sk,
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};
#endif

/*
 *	TCP over IPv4 via INET6 API
 */
<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static const struct inet_connection_sock_af_ops ipv6_mapped = {
	.queue_xmit	   = ip_queue_xmit,
	.send_check	   = tcp_v4_send_check,
	.rebuild_header	   = inet_sk_rebuild_header,
<<<<<<< HEAD
	.conn_request	   = tcp_v6_conn_request,
	.syn_recv_sock	   = tcp_v6_syn_recv_sock,
	.get_peer	   = tcp_v4_get_peer,
=======
	.sk_rx_dst_set	   = inet_sk_rx_dst_set,
	.conn_request	   = tcp_v6_conn_request,
	.syn_recv_sock	   = tcp_v6_syn_recv_sock,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.net_header_len	   = sizeof(struct iphdr),
	.setsockopt	   = ipv6_setsockopt,
	.getsockopt	   = ipv6_getsockopt,
	.addr2sockaddr	   = inet6_csk_addr2sockaddr,
	.sockaddr_len	   = sizeof(struct sockaddr_in6),
<<<<<<< HEAD
	.bind_conflict	   = inet6_csk_bind_conflict,
#ifdef CONFIG_COMPAT
	.compat_setsockopt = compat_ipv6_setsockopt,
	.compat_getsockopt = compat_ipv6_getsockopt,
#endif
};

#ifdef CONFIG_TCP_MD5SIG
static const struct tcp_sock_af_ops tcp_sock_ipv6_mapped_specific = {
	.md5_lookup	=	tcp_v4_md5_lookup,
	.calc_md5_hash	=	tcp_v4_md5_hash_skb,
	.md5_parse	=	tcp_v6_parse_md5_keys,
=======
	.mtu_reduced	   = tcp_v4_mtu_reduced,
};

#if defined(CONFIG_TCP_MD5SIG) || defined(CONFIG_TCP_AO)
static const struct tcp_sock_af_ops tcp_sock_ipv6_mapped_specific = {
#ifdef CONFIG_TCP_MD5SIG
	.md5_lookup	=	tcp_v4_md5_lookup,
	.calc_md5_hash	=	tcp_v4_md5_hash_skb,
	.md5_parse	=	tcp_v6_parse_md5_keys,
#endif
#ifdef CONFIG_TCP_AO
	.ao_lookup	=	tcp_v6_ao_lookup,
	.calc_ao_hash	=	tcp_v4_ao_hash_skb,
	.ao_parse	=	tcp_v6_parse_ao,
	.ao_calc_key_sk	=	tcp_v4_ao_calc_key_sk,
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};
#endif

/* NOTE: A lot of things set to zero explicitly by call to
 *       sk_alloc() so need not be done here.
 */
static int tcp_v6_init_sock(struct sock *sk)
{
	struct inet_connection_sock *icsk = inet_csk(sk);
<<<<<<< HEAD
	struct tcp_sock *tp = tcp_sk(sk);

	skb_queue_head_init(&tp->out_of_order_queue);
	tcp_init_xmit_timers(sk);
	tcp_prequeue_init(tp);

	icsk->icsk_rto = TCP_TIMEOUT_INIT;
	tp->mdev = TCP_TIMEOUT_INIT;

	/* So many TCP implementations out there (incorrectly) count the
	 * initial SYN frame in their delayed-ACK and congestion control
	 * algorithms that we must have the following bandaid to talk
	 * efficiently to them.  -DaveM
	 */
	tp->snd_cwnd = 2;

	/* See draft-stevens-tcpca-spec-01 for discussion of the
	 * initialization of these values.
	 */
	tp->snd_ssthresh = TCP_INFINITE_SSTHRESH;
	tp->snd_cwnd_clamp = ~0;
	tp->mss_cache = TCP_MSS_DEFAULT;

	tp->reordering = sysctl_tcp_reordering;

	sk->sk_state = TCP_CLOSE;

	icsk->icsk_af_ops = &ipv6_specific;
	icsk->icsk_ca_ops = &tcp_init_congestion_ops;
	icsk->icsk_sync_mss = tcp_sync_mss;
	sk->sk_write_space = sk_stream_write_space;
	sock_set_flag(sk, SOCK_USE_WRITE_QUEUE);

#ifdef CONFIG_TCP_MD5SIG
	tp->af_specific = &tcp_sock_ipv6_specific;
#endif

	/* TCP Cookie Transactions */
	if (sysctl_tcp_cookie_size > 0) {
		/* Default, cookies without s_data_payload. */
		tp->cookie_values =
			kzalloc(sizeof(*tp->cookie_values),
				sk->sk_allocation);
		if (tp->cookie_values != NULL)
			kref_init(&tp->cookie_values->kref);
	}
	/* Presumed zeroed, in order of appearance:
	 *	cookie_in_always, cookie_out_never,
	 *	s_data_constant, s_data_in, s_data_out
	 */
	sk->sk_sndbuf = sysctl_tcp_wmem[1];
	sk->sk_rcvbuf = sysctl_tcp_rmem[1];

	local_bh_disable();
	sock_update_memcg(sk);
	sk_sockets_allocated_inc(sk);
	local_bh_enable();

	return 0;
}

static void tcp_v6_destroy_sock(struct sock *sk)
{
	tcp_v4_destroy_sock(sk);
	inet6_destroy_sock(sk);
}

#ifdef CONFIG_PROC_FS
/* Proc filesystem TCPv6 sock list dumping. */
static void get_openreq6(struct seq_file *seq,
			 const struct sock *sk, struct request_sock *req, int i, int uid)
{
	int ttd = req->expires - jiffies;
	const struct in6_addr *src = &inet6_rsk(req)->loc_addr;
	const struct in6_addr *dest = &inet6_rsk(req)->rmt_addr;
=======

	tcp_init_sock(sk);

	icsk->icsk_af_ops = &ipv6_specific;

#if defined(CONFIG_TCP_MD5SIG) || defined(CONFIG_TCP_AO)
	tcp_sk(sk)->af_specific = &tcp_sock_ipv6_specific;
#endif

	return 0;
}

#ifdef CONFIG_PROC_FS
/* Proc filesystem TCPv6 sock list dumping. */
static void get_openreq6(struct seq_file *seq,
			 const struct request_sock *req, int i)
{
	long ttd = req->rsk_timer.expires - jiffies;
	const struct in6_addr *src = &inet_rsk(req)->ir_v6_loc_addr;
	const struct in6_addr *dest = &inet_rsk(req)->ir_v6_rmt_addr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (ttd < 0)
		ttd = 0;

	seq_printf(seq,
		   "%4d: %08X%08X%08X%08X:%04X %08X%08X%08X%08X:%04X "
<<<<<<< HEAD
		   "%02X %08X:%08X %02X:%08lX %08X %5d %8d %d %d %pK\n",
		   i,
		   src->s6_addr32[0], src->s6_addr32[1],
		   src->s6_addr32[2], src->s6_addr32[3],
		   ntohs(inet_rsk(req)->loc_port),
		   dest->s6_addr32[0], dest->s6_addr32[1],
		   dest->s6_addr32[2], dest->s6_addr32[3],
		   ntohs(inet_rsk(req)->rmt_port),
		   TCP_SYN_RECV,
		   0,0, /* could print option size, but that is af dependent. */
		   1,   /* timers active (only the expire timer) */
		   jiffies_to_clock_t(ttd),
		   req->retrans,
		   uid,
=======
		   "%02X %08X:%08X %02X:%08lX %08X %5u %8d %d %d %pK\n",
		   i,
		   src->s6_addr32[0], src->s6_addr32[1],
		   src->s6_addr32[2], src->s6_addr32[3],
		   inet_rsk(req)->ir_num,
		   dest->s6_addr32[0], dest->s6_addr32[1],
		   dest->s6_addr32[2], dest->s6_addr32[3],
		   ntohs(inet_rsk(req)->ir_rmt_port),
		   TCP_SYN_RECV,
		   0, 0, /* could print option size, but that is af dependent. */
		   1,   /* timers active (only the expire timer) */
		   jiffies_to_clock_t(ttd),
		   req->num_timeout,
		   from_kuid_munged(seq_user_ns(seq),
				    sock_i_uid(req->rsk_listener)),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		   0,  /* non standard timer */
		   0, /* open_requests have no inode */
		   0, req);
}

static void get_tcp6_sock(struct seq_file *seq, struct sock *sp, int i)
{
	const struct in6_addr *dest, *src;
	__u16 destp, srcp;
	int timer_active;
	unsigned long timer_expires;
	const struct inet_sock *inet = inet_sk(sp);
	const struct tcp_sock *tp = tcp_sk(sp);
	const struct inet_connection_sock *icsk = inet_csk(sp);
<<<<<<< HEAD
	const struct ipv6_pinfo *np = inet6_sk(sp);

	dest  = &np->daddr;
	src   = &np->rcv_saddr;
	destp = ntohs(inet->inet_dport);
	srcp  = ntohs(inet->inet_sport);

	if (icsk->icsk_pending == ICSK_TIME_RETRANS) {
=======
	const struct fastopen_queue *fastopenq = &icsk->icsk_accept_queue.fastopenq;
	int rx_queue;
	int state;

	dest  = &sp->sk_v6_daddr;
	src   = &sp->sk_v6_rcv_saddr;
	destp = ntohs(inet->inet_dport);
	srcp  = ntohs(inet->inet_sport);

	if (icsk->icsk_pending == ICSK_TIME_RETRANS ||
	    icsk->icsk_pending == ICSK_TIME_REO_TIMEOUT ||
	    icsk->icsk_pending == ICSK_TIME_LOSS_PROBE) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		timer_active	= 1;
		timer_expires	= icsk->icsk_timeout;
	} else if (icsk->icsk_pending == ICSK_TIME_PROBE0) {
		timer_active	= 4;
		timer_expires	= icsk->icsk_timeout;
	} else if (timer_pending(&sp->sk_timer)) {
		timer_active	= 2;
		timer_expires	= sp->sk_timer.expires;
	} else {
		timer_active	= 0;
		timer_expires = jiffies;
	}

<<<<<<< HEAD
	seq_printf(seq,
		   "%4d: %08X%08X%08X%08X:%04X %08X%08X%08X%08X:%04X "
		   "%02X %08X:%08X %02X:%08lX %08X %5d %8d %lu %d %pK %lu %lu %u %u %d\n",
=======
	state = inet_sk_state_load(sp);
	if (state == TCP_LISTEN)
		rx_queue = READ_ONCE(sp->sk_ack_backlog);
	else
		/* Because we don't lock the socket,
		 * we might find a transient negative value.
		 */
		rx_queue = max_t(int, READ_ONCE(tp->rcv_nxt) -
				      READ_ONCE(tp->copied_seq), 0);

	seq_printf(seq,
		   "%4d: %08X%08X%08X%08X:%04X %08X%08X%08X%08X:%04X "
		   "%02X %08X:%08X %02X:%08lX %08X %5u %8d %lu %d %pK %lu %lu %u %u %d\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		   i,
		   src->s6_addr32[0], src->s6_addr32[1],
		   src->s6_addr32[2], src->s6_addr32[3], srcp,
		   dest->s6_addr32[0], dest->s6_addr32[1],
		   dest->s6_addr32[2], dest->s6_addr32[3], destp,
<<<<<<< HEAD
		   sp->sk_state,
		   tp->write_seq-tp->snd_una,
		   (sp->sk_state == TCP_LISTEN) ? sp->sk_ack_backlog : (tp->rcv_nxt - tp->copied_seq),
		   timer_active,
		   jiffies_to_clock_t(timer_expires - jiffies),
		   icsk->icsk_retransmits,
		   sock_i_uid(sp),
		   icsk->icsk_probes_out,
		   sock_i_ino(sp),
		   atomic_read(&sp->sk_refcnt), sp,
		   jiffies_to_clock_t(icsk->icsk_rto),
		   jiffies_to_clock_t(icsk->icsk_ack.ato),
		   (icsk->icsk_ack.quick << 1 ) | icsk->icsk_ack.pingpong,
		   tp->snd_cwnd,
		   tcp_in_initial_slowstart(tp) ? -1 : tp->snd_ssthresh
=======
		   state,
		   READ_ONCE(tp->write_seq) - tp->snd_una,
		   rx_queue,
		   timer_active,
		   jiffies_delta_to_clock_t(timer_expires - jiffies),
		   icsk->icsk_retransmits,
		   from_kuid_munged(seq_user_ns(seq), sock_i_uid(sp)),
		   icsk->icsk_probes_out,
		   sock_i_ino(sp),
		   refcount_read(&sp->sk_refcnt), sp,
		   jiffies_to_clock_t(icsk->icsk_rto),
		   jiffies_to_clock_t(icsk->icsk_ack.ato),
		   (icsk->icsk_ack.quick << 1) | inet_csk_in_pingpong_mode(sp),
		   tcp_snd_cwnd(tp),
		   state == TCP_LISTEN ?
			fastopenq->max_qlen :
			(tcp_in_initial_slowstart(tp) ? -1 : tp->snd_ssthresh)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		   );
}

static void get_timewait6_sock(struct seq_file *seq,
			       struct inet_timewait_sock *tw, int i)
{
<<<<<<< HEAD
	const struct in6_addr *dest, *src;
	__u16 destp, srcp;
	const struct inet6_timewait_sock *tw6 = inet6_twsk((struct sock *)tw);
	int ttd = tw->tw_ttd - jiffies;

	if (ttd < 0)
		ttd = 0;

	dest = &tw6->tw_v6_daddr;
	src  = &tw6->tw_v6_rcv_saddr;
=======
	long delta = tw->tw_timer.expires - jiffies;
	const struct in6_addr *dest, *src;
	__u16 destp, srcp;

	dest = &tw->tw_v6_daddr;
	src  = &tw->tw_v6_rcv_saddr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	destp = ntohs(tw->tw_dport);
	srcp  = ntohs(tw->tw_sport);

	seq_printf(seq,
		   "%4d: %08X%08X%08X%08X:%04X %08X%08X%08X%08X:%04X "
		   "%02X %08X:%08X %02X:%08lX %08X %5d %8d %d %d %pK\n",
		   i,
		   src->s6_addr32[0], src->s6_addr32[1],
		   src->s6_addr32[2], src->s6_addr32[3], srcp,
		   dest->s6_addr32[0], dest->s6_addr32[1],
		   dest->s6_addr32[2], dest->s6_addr32[3], destp,
		   tw->tw_substate, 0, 0,
<<<<<<< HEAD
		   3, jiffies_to_clock_t(ttd), 0, 0, 0, 0,
		   atomic_read(&tw->tw_refcnt), tw);
=======
		   3, jiffies_delta_to_clock_t(delta), 0, 0, 0, 0,
		   refcount_read(&tw->tw_refcnt), tw);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int tcp6_seq_show(struct seq_file *seq, void *v)
{
	struct tcp_iter_state *st;
<<<<<<< HEAD
=======
	struct sock *sk = v;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (v == SEQ_START_TOKEN) {
		seq_puts(seq,
			 "  sl  "
			 "local_address                         "
			 "remote_address                        "
			 "st tx_queue rx_queue tr tm->when retrnsmt"
			 "   uid  timeout inode\n");
		goto out;
	}
	st = seq->private;

<<<<<<< HEAD
	switch (st->state) {
	case TCP_SEQ_STATE_LISTENING:
	case TCP_SEQ_STATE_ESTABLISHED:
		get_tcp6_sock(seq, v, st->num);
		break;
	case TCP_SEQ_STATE_OPENREQ:
		get_openreq6(seq, st->syn_wait_sk, v, st->num, st->uid);
		break;
	case TCP_SEQ_STATE_TIME_WAIT:
		get_timewait6_sock(seq, v, st->num);
		break;
	}
=======
	if (sk->sk_state == TCP_TIME_WAIT)
		get_timewait6_sock(seq, v, st->num);
	else if (sk->sk_state == TCP_NEW_SYN_RECV)
		get_openreq6(seq, v, st->num);
	else
		get_tcp6_sock(seq, v, st->num);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	return 0;
}

<<<<<<< HEAD
static const struct file_operations tcp6_afinfo_seq_fops = {
	.owner   = THIS_MODULE,
	.open    = tcp_seq_open,
	.read    = seq_read,
	.llseek  = seq_lseek,
	.release = seq_release_net
};

static struct tcp_seq_afinfo tcp6_seq_afinfo = {
	.name		= "tcp6",
	.family		= AF_INET6,
	.seq_fops	= &tcp6_afinfo_seq_fops,
	.seq_ops	= {
		.show		= tcp6_seq_show,
	},
=======
static const struct seq_operations tcp6_seq_ops = {
	.show		= tcp6_seq_show,
	.start		= tcp_seq_start,
	.next		= tcp_seq_next,
	.stop		= tcp_seq_stop,
};

static struct tcp_seq_afinfo tcp6_seq_afinfo = {
	.family		= AF_INET6,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

int __net_init tcp6_proc_init(struct net *net)
{
<<<<<<< HEAD
	return tcp_proc_register(net, &tcp6_seq_afinfo);
=======
	if (!proc_create_net_data("tcp6", 0444, net->proc_net, &tcp6_seq_ops,
			sizeof(struct tcp_iter_state), &tcp6_seq_afinfo))
		return -ENOMEM;
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void tcp6_proc_exit(struct net *net)
{
<<<<<<< HEAD
	tcp_proc_unregister(net, &tcp6_seq_afinfo);
}
#endif

static void tcp_v6_clear_sk(struct sock *sk, int size)
{
	struct inet_sock *inet = inet_sk(sk);

	/* we do not want to clear pinet6 field, because of RCU lookups */
	sk_prot_clear_nulls(sk, offsetof(struct inet_sock, pinet6));

	size -= offsetof(struct inet_sock, pinet6) + sizeof(inet->pinet6);
	memset(&inet->pinet6 + 1, 0, size);
}

=======
	remove_proc_entry("tcp6", net->proc_net);
}
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct proto tcpv6_prot = {
	.name			= "TCPv6",
	.owner			= THIS_MODULE,
	.close			= tcp_close,
<<<<<<< HEAD
=======
	.pre_connect		= tcp_v6_pre_connect,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.connect		= tcp_v6_connect,
	.disconnect		= tcp_disconnect,
	.accept			= inet_csk_accept,
	.ioctl			= tcp_ioctl,
	.init			= tcp_v6_init_sock,
<<<<<<< HEAD
	.destroy		= tcp_v6_destroy_sock,
	.shutdown		= tcp_shutdown,
	.setsockopt		= tcp_setsockopt,
	.getsockopt		= tcp_getsockopt,
	.recvmsg		= tcp_recvmsg,
	.sendmsg		= tcp_sendmsg,
	.sendpage		= tcp_sendpage,
	.backlog_rcv		= tcp_v6_do_rcv,
	.hash			= tcp_v6_hash,
	.unhash			= inet_unhash,
	.get_port		= inet_csk_get_port,
	.enter_memory_pressure	= tcp_enter_memory_pressure,
	.sockets_allocated	= &tcp_sockets_allocated,
	.memory_allocated	= &tcp_memory_allocated,
	.memory_pressure	= &tcp_memory_pressure,
	.orphan_count		= &tcp_orphan_count,
	.sysctl_wmem		= sysctl_tcp_wmem,
	.sysctl_rmem		= sysctl_tcp_rmem,
	.max_header		= MAX_TCP_HEADER,
	.obj_size		= sizeof(struct tcp6_sock),
	.slab_flags		= SLAB_DESTROY_BY_RCU,
	.twsk_prot		= &tcp6_timewait_sock_ops,
	.rsk_prot		= &tcp6_request_sock_ops,
	.h.hashinfo		= &tcp_hashinfo,
	.no_autobind		= true,
#ifdef CONFIG_COMPAT
	.compat_setsockopt	= compat_tcp_setsockopt,
	.compat_getsockopt	= compat_tcp_getsockopt,
#endif
#ifdef CONFIG_CGROUP_MEM_RES_CTLR_KMEM
	.proto_cgroup		= tcp_proto_cgroup,
#endif
	.clear_sk		= tcp_v6_clear_sk,
};

static const struct inet6_protocol tcpv6_protocol = {
	.handler	=	tcp_v6_rcv,
	.err_handler	=	tcp_v6_err,
	.gso_send_check	=	tcp_v6_gso_send_check,
	.gso_segment	=	tcp_tso_segment,
	.gro_receive	=	tcp6_gro_receive,
	.gro_complete	=	tcp6_gro_complete,
	.flags		=	INET6_PROTO_NOPOLICY|INET6_PROTO_FINAL,
};
=======
	.destroy		= tcp_v4_destroy_sock,
	.shutdown		= tcp_shutdown,
	.setsockopt		= tcp_setsockopt,
	.getsockopt		= tcp_getsockopt,
	.bpf_bypass_getsockopt	= tcp_bpf_bypass_getsockopt,
	.keepalive		= tcp_set_keepalive,
	.recvmsg		= tcp_recvmsg,
	.sendmsg		= tcp_sendmsg,
	.splice_eof		= tcp_splice_eof,
	.backlog_rcv		= tcp_v6_do_rcv,
	.release_cb		= tcp_release_cb,
	.hash			= inet6_hash,
	.unhash			= inet_unhash,
	.get_port		= inet_csk_get_port,
	.put_port		= inet_put_port,
#ifdef CONFIG_BPF_SYSCALL
	.psock_update_sk_prot	= tcp_bpf_update_proto,
#endif
	.enter_memory_pressure	= tcp_enter_memory_pressure,
	.leave_memory_pressure	= tcp_leave_memory_pressure,
	.stream_memory_free	= tcp_stream_memory_free,
	.sockets_allocated	= &tcp_sockets_allocated,

	.memory_allocated	= &tcp_memory_allocated,
	.per_cpu_fw_alloc	= &tcp_memory_per_cpu_fw_alloc,

	.memory_pressure	= &tcp_memory_pressure,
	.orphan_count		= &tcp_orphan_count,
	.sysctl_mem		= sysctl_tcp_mem,
	.sysctl_wmem_offset	= offsetof(struct net, ipv4.sysctl_tcp_wmem),
	.sysctl_rmem_offset	= offsetof(struct net, ipv4.sysctl_tcp_rmem),
	.max_header		= MAX_TCP_HEADER,
	.obj_size		= sizeof(struct tcp6_sock),
	.ipv6_pinfo_offset = offsetof(struct tcp6_sock, inet6),
	.slab_flags		= SLAB_TYPESAFE_BY_RCU,
	.twsk_prot		= &tcp6_timewait_sock_ops,
	.rsk_prot		= &tcp6_request_sock_ops,
	.h.hashinfo		= NULL,
	.no_autobind		= true,
	.diag_destroy		= tcp_abort,
};
EXPORT_SYMBOL_GPL(tcpv6_prot);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static struct inet_protosw tcpv6_protosw = {
	.type		=	SOCK_STREAM,
	.protocol	=	IPPROTO_TCP,
	.prot		=	&tcpv6_prot,
	.ops		=	&inet6_stream_ops,
<<<<<<< HEAD
	.no_check	=	0,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.flags		=	INET_PROTOSW_PERMANENT |
				INET_PROTOSW_ICSK,
};

static int __net_init tcpv6_net_init(struct net *net)
{
	return inet_ctl_sock_create(&net->ipv6.tcp_sk, PF_INET6,
				    SOCK_RAW, IPPROTO_TCP, net);
}

static void __net_exit tcpv6_net_exit(struct net *net)
{
	inet_ctl_sock_destroy(net->ipv6.tcp_sk);
}

static void __net_exit tcpv6_net_exit_batch(struct list_head *net_exit_list)
{
<<<<<<< HEAD
	inet_twsk_purge(&tcp_hashinfo, &tcp_death_row, AF_INET6);
=======
	tcp_twsk_purge(net_exit_list, AF_INET6);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct pernet_operations tcpv6_net_ops = {
	.init	    = tcpv6_net_init,
	.exit	    = tcpv6_net_exit,
	.exit_batch = tcpv6_net_exit_batch,
};

int __init tcpv6_init(void)
{
	int ret;

<<<<<<< HEAD
	ret = inet6_add_protocol(&tcpv6_protocol, IPPROTO_TCP);
=======
	net_hotdata.tcpv6_protocol = (struct inet6_protocol) {
		.handler     = tcp_v6_rcv,
		.err_handler = tcp_v6_err,
		.flags	     = INET6_PROTO_NOPOLICY | INET6_PROTO_FINAL,
	};
	ret = inet6_add_protocol(&net_hotdata.tcpv6_protocol, IPPROTO_TCP);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ret)
		goto out;

	/* register inet6 protocol */
	ret = inet6_register_protosw(&tcpv6_protosw);
	if (ret)
		goto out_tcpv6_protocol;

	ret = register_pernet_subsys(&tcpv6_net_ops);
	if (ret)
		goto out_tcpv6_protosw;
<<<<<<< HEAD
out:
	return ret;

out_tcpv6_protocol:
	inet6_del_protocol(&tcpv6_protocol, IPPROTO_TCP);
out_tcpv6_protosw:
	inet6_unregister_protosw(&tcpv6_protosw);
=======

	ret = mptcpv6_init();
	if (ret)
		goto out_tcpv6_pernet_subsys;

out:
	return ret;

out_tcpv6_pernet_subsys:
	unregister_pernet_subsys(&tcpv6_net_ops);
out_tcpv6_protosw:
	inet6_unregister_protosw(&tcpv6_protosw);
out_tcpv6_protocol:
	inet6_del_protocol(&net_hotdata.tcpv6_protocol, IPPROTO_TCP);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	goto out;
}

void tcpv6_exit(void)
{
	unregister_pernet_subsys(&tcpv6_net_ops);
	inet6_unregister_protosw(&tcpv6_protosw);
<<<<<<< HEAD
	inet6_del_protocol(&tcpv6_protocol, IPPROTO_TCP);
=======
	inet6_del_protocol(&net_hotdata.tcpv6_protocol, IPPROTO_TCP);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
