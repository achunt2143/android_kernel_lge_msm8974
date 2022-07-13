<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * INET		An implementation of the TCP/IP protocol suite for the LINUX
 *		operating system.  INET is implemented using the  BSD Socket
 *		interface as the means of communication with the user level.
 *
 *		Implementation of the Transmission Control Protocol(TCP).
 *
 *		IPv4 specific functions
 *
<<<<<<< HEAD
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *		code split from:
 *		linux/ipv4/tcp.c
 *		linux/ipv4/tcp_input.c
 *		linux/ipv4/tcp_output.c
 *
 *		See tcp.c for author information
<<<<<<< HEAD
 *
 *	This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

/*
 * Changes:
 *		David S. Miller	:	New socket lookup architecture.
 *					This code is dedicated to John Dyson.
 *		David S. Miller :	Change semantics of established hash,
 *					half is devoted to TIME_WAIT sockets
 *					and the rest go in the other half.
 *		Andi Kleen :		Add support for syncookies and fixed
 *					some bugs: ip options weren't passed to
 *					the TCP layer, missed a check for an
 *					ACK bit.
 *		Andi Kleen :		Implemented fast path mtu discovery.
 *	     				Fixed many serious bugs in the
 *					request_sock handling and moved
 *					most of it into the af independent code.
 *					Added tail drop and some other bugfixes.
 *					Added new listen semantics.
 *		Mike McLagan	:	Routing by source
 *	Juan Jose Ciarlante:		ip_dynaddr bits
 *		Andi Kleen:		various fixes.
 *	Vitaly E. Lavrov	:	Transparent proxy revived after year
 *					coma.
 *	Andi Kleen		:	Fix new listen.
 *	Andi Kleen		:	Fix accept error reporting.
 *	YOSHIFUJI Hideaki @USAGI and:	Support IPV6_V6ONLY socket option, which
 *	Alexey Kuznetsov		allow both IPv4 and IPv6 sockets to bind
 *					a single port at the same time.
 */

#define pr_fmt(fmt) "TCP: " fmt

#include <linux/bottom_half.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/module.h>
#include <linux/random.h>
#include <linux/cache.h>
#include <linux/jhash.h>
#include <linux/init.h>
#include <linux/times.h>
#include <linux/slab.h>
<<<<<<< HEAD
=======
#include <linux/sched.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <net/net_namespace.h>
#include <net/icmp.h>
#include <net/inet_hashtables.h>
#include <net/tcp.h>
#include <net/transp_v6.h>
#include <net/ipv6.h>
#include <net/inet_common.h>
#include <net/timewait_sock.h>
#include <net/xfrm.h>
<<<<<<< HEAD
#include <net/netdma.h>
#include <net/secure_seq.h>
#include <net/tcp_memcontrol.h>
=======
#include <net/secure_seq.h>
#include <net/busy_poll.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <linux/inet.h>
#include <linux/ipv6.h>
#include <linux/stddef.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
<<<<<<< HEAD

#include <linux/crypto.h>
#include <linux/scatterlist.h>

int sysctl_tcp_tw_reuse __read_mostly;
int sysctl_tcp_low_latency __read_mostly;
EXPORT_SYMBOL(sysctl_tcp_low_latency);

=======
#include <linux/inetdevice.h>
#include <linux/btf_ids.h>

#include <crypto/hash.h>
#include <linux/scatterlist.h>

#include <trace/events/tcp.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_TCP_MD5SIG
static int tcp_v4_md5_hash_hdr(char *md5_hash, const struct tcp_md5sig_key *key,
			       __be32 daddr, __be32 saddr, const struct tcphdr *th);
#endif

struct inet_hashinfo tcp_hashinfo;
EXPORT_SYMBOL(tcp_hashinfo);

<<<<<<< HEAD
static inline __u32 tcp_v4_init_sequence(const struct sk_buff *skb)
{
	return secure_tcp_sequence_number(ip_hdr(skb)->daddr,
					  ip_hdr(skb)->saddr,
					  tcp_hdr(skb)->dest,
					  tcp_hdr(skb)->source);
=======
static DEFINE_PER_CPU(struct sock *, ipv4_tcp_sk);

static u32 tcp_v4_init_seq(const struct sk_buff *skb)
{
	return secure_tcp_seq(ip_hdr(skb)->daddr,
			      ip_hdr(skb)->saddr,
			      tcp_hdr(skb)->dest,
			      tcp_hdr(skb)->source);
}

static u32 tcp_v4_init_ts_off(const struct net *net, const struct sk_buff *skb)
{
	return secure_tcp_ts_off(net, ip_hdr(skb)->daddr, ip_hdr(skb)->saddr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int tcp_twsk_unique(struct sock *sk, struct sock *sktw, void *twp)
{
<<<<<<< HEAD
	const struct tcp_timewait_sock *tcptw = tcp_twsk(sktw);
	struct tcp_sock *tp = tcp_sk(sk);

=======
	int reuse = READ_ONCE(sock_net(sk)->ipv4.sysctl_tcp_tw_reuse);
	const struct inet_timewait_sock *tw = inet_twsk(sktw);
	const struct tcp_timewait_sock *tcptw = tcp_twsk(sktw);
	struct tcp_sock *tp = tcp_sk(sk);

	if (reuse == 2) {
		/* Still does not detect *everything* that goes through
		 * lo, since we require a loopback src or dst address
		 * or direct binding to 'lo' interface.
		 */
		bool loopback = false;
		if (tw->tw_bound_dev_if == LOOPBACK_IFINDEX)
			loopback = true;
#if IS_ENABLED(CONFIG_IPV6)
		if (tw->tw_family == AF_INET6) {
			if (ipv6_addr_loopback(&tw->tw_v6_daddr) ||
			    ipv6_addr_v4mapped_loopback(&tw->tw_v6_daddr) ||
			    ipv6_addr_loopback(&tw->tw_v6_rcv_saddr) ||
			    ipv6_addr_v4mapped_loopback(&tw->tw_v6_rcv_saddr))
				loopback = true;
		} else
#endif
		{
			if (ipv4_is_loopback(tw->tw_daddr) ||
			    ipv4_is_loopback(tw->tw_rcv_saddr))
				loopback = true;
		}
		if (!loopback)
			reuse = 0;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* With PAWS, it is safe from the viewpoint
	   of data integrity. Even without PAWS it is safe provided sequence
	   spaces do not overlap i.e. at data rates <= 80Mbit/sec.

	   Actually, the idea is close to VJ's one, only timestamp cache is
	   held not per host, but per port pair and TW bucket is used as state
	   holder.

	   If TW bucket has been already destroyed we fall back to VJ's scheme
	   and use initial timestamp retrieved from peer table.
	 */
	if (tcptw->tw_ts_recent_stamp &&
<<<<<<< HEAD
	    (twp == NULL || (sysctl_tcp_tw_reuse &&
			     get_seconds() - tcptw->tw_ts_recent_stamp > 1))) {
		tp->write_seq = tcptw->tw_snd_nxt + 65535 + 2;
		if (tp->write_seq == 0)
			tp->write_seq = 1;
		tp->rx_opt.ts_recent	   = tcptw->tw_ts_recent;
		tp->rx_opt.ts_recent_stamp = tcptw->tw_ts_recent_stamp;
		sock_hold(sktw);
=======
	    (!twp || (reuse && time_after32(ktime_get_seconds(),
					    tcptw->tw_ts_recent_stamp)))) {
		/* inet_twsk_hashdance() sets sk_refcnt after putting twsk
		 * and releasing the bucket lock.
		 */
		if (unlikely(!refcount_inc_not_zero(&sktw->sk_refcnt)))
			return 0;

		/* In case of repair and re-using TIME-WAIT sockets we still
		 * want to be sure that it is safe as above but honor the
		 * sequence numbers and time stamps set as part of the repair
		 * process.
		 *
		 * Without this check re-using a TIME-WAIT socket with TCP
		 * repair would accumulate a -1 on the repair assigned
		 * sequence number. The first time it is reused the sequence
		 * is -1, the second time -2, etc. This fixes that issue
		 * without appearing to create any others.
		 */
		if (likely(!tp->repair)) {
			u32 seq = tcptw->tw_snd_nxt + 65535 + 2;

			if (!seq)
				seq = 1;
			WRITE_ONCE(tp->write_seq, seq);
			tp->rx_opt.ts_recent	   = tcptw->tw_ts_recent;
			tp->rx_opt.ts_recent_stamp = tcptw->tw_ts_recent_stamp;
		}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 1;
	}

	return 0;
}
EXPORT_SYMBOL_GPL(tcp_twsk_unique);

<<<<<<< HEAD
=======
static int tcp_v4_pre_connect(struct sock *sk, struct sockaddr *uaddr,
			      int addr_len)
{
	/* This check is replicated from tcp_v4_connect() and intended to
	 * prevent BPF program called below from accessing bytes that are out
	 * of the bound specified by user in addr_len.
	 */
	if (addr_len < sizeof(struct sockaddr_in))
		return -EINVAL;

	sock_owned_by_me(sk);

	return BPF_CGROUP_RUN_PROG_INET4_CONNECT(sk, uaddr, &addr_len);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* This will initiate an outgoing connection. */
int tcp_v4_connect(struct sock *sk, struct sockaddr *uaddr, int addr_len)
{
	struct sockaddr_in *usin = (struct sockaddr_in *)uaddr;
<<<<<<< HEAD
	struct inet_sock *inet = inet_sk(sk);
	struct tcp_sock *tp = tcp_sk(sk);
=======
	struct inet_timewait_death_row *tcp_death_row;
	struct inet_sock *inet = inet_sk(sk);
	struct tcp_sock *tp = tcp_sk(sk);
	struct ip_options_rcu *inet_opt;
	struct net *net = sock_net(sk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	__be16 orig_sport, orig_dport;
	__be32 daddr, nexthop;
	struct flowi4 *fl4;
	struct rtable *rt;
	int err;
<<<<<<< HEAD
	struct ip_options_rcu *inet_opt;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (addr_len < sizeof(struct sockaddr_in))
		return -EINVAL;

	if (usin->sin_family != AF_INET)
		return -EAFNOSUPPORT;

	nexthop = daddr = usin->sin_addr.s_addr;
	inet_opt = rcu_dereference_protected(inet->inet_opt,
<<<<<<< HEAD
					     sock_owned_by_user(sk));
=======
					     lockdep_sock_is_held(sk));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (inet_opt && inet_opt->opt.srr) {
		if (!daddr)
			return -EINVAL;
		nexthop = inet_opt->opt.faddr;
	}

	orig_sport = inet->inet_sport;
	orig_dport = usin->sin_port;
	fl4 = &inet->cork.fl.u.ip4;
	rt = ip_route_connect(fl4, nexthop, inet->inet_saddr,
<<<<<<< HEAD
			      RT_CONN_FLAGS(sk), sk->sk_bound_dev_if,
			      IPPROTO_TCP,
			      orig_sport, orig_dport, sk, true);
	if (IS_ERR(rt)) {
		err = PTR_ERR(rt);
		if (err == -ENETUNREACH)
			IP_INC_STATS(sock_net(sk), IPSTATS_MIB_OUTNOROUTES);
=======
			      sk->sk_bound_dev_if, IPPROTO_TCP, orig_sport,
			      orig_dport, sk);
	if (IS_ERR(rt)) {
		err = PTR_ERR(rt);
		if (err == -ENETUNREACH)
			IP_INC_STATS(net, IPSTATS_MIB_OUTNOROUTES);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return err;
	}

	if (rt->rt_flags & (RTCF_MULTICAST | RTCF_BROADCAST)) {
		ip_rt_put(rt);
		return -ENETUNREACH;
	}

	if (!inet_opt || !inet_opt->opt.srr)
		daddr = fl4->daddr;

<<<<<<< HEAD
	if (!inet->inet_saddr)
		inet->inet_saddr = fl4->saddr;
	inet->inet_rcv_saddr = inet->inet_saddr;
=======
	tcp_death_row = &sock_net(sk)->ipv4.tcp_death_row;

	if (!inet->inet_saddr) {
		err = inet_bhash2_update_saddr(sk,  &fl4->saddr, AF_INET);
		if (err) {
			ip_rt_put(rt);
			return err;
		}
	} else {
		sk_rcv_saddr_set(sk, inet->inet_saddr);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (tp->rx_opt.ts_recent_stamp && inet->inet_daddr != daddr) {
		/* Reset inherited state */
		tp->rx_opt.ts_recent	   = 0;
		tp->rx_opt.ts_recent_stamp = 0;
<<<<<<< HEAD
		tp->write_seq		   = 0;
	}

	if (tcp_death_row.sysctl_tw_recycle &&
	    !tp->rx_opt.ts_recent_stamp && fl4->daddr == daddr) {
		struct inet_peer *peer = rt_get_peer(rt, fl4->daddr);
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

	inet->inet_dport = usin->sin_port;
	inet->inet_daddr = daddr;
=======
		if (likely(!tp->repair))
			WRITE_ONCE(tp->write_seq, 0);
	}

	inet->inet_dport = usin->sin_port;
	sk_daddr_set(sk, daddr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	inet_csk(sk)->icsk_ext_hdr_len = 0;
	if (inet_opt)
		inet_csk(sk)->icsk_ext_hdr_len = inet_opt->opt.optlen;

	tp->rx_opt.mss_clamp = TCP_MSS_DEFAULT;

	/* Socket identity is still unknown (sport may be zero).
	 * However we set state to SYN-SENT and not releasing socket
	 * lock select source port, enter ourselves into the hash tables and
	 * complete initialization after this.
	 */
	tcp_set_state(sk, TCP_SYN_SENT);
<<<<<<< HEAD
	err = inet_hash_connect(&tcp_death_row, sk);
	if (err)
		goto failure;

=======
	err = inet_hash_connect(tcp_death_row, sk);
	if (err)
		goto failure;

	sk_set_txhash(sk);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rt = ip_route_newports(fl4, rt, orig_sport, orig_dport,
			       inet->inet_sport, inet->inet_dport, sk);
	if (IS_ERR(rt)) {
		err = PTR_ERR(rt);
		rt = NULL;
		goto failure;
	}
<<<<<<< HEAD
	/* OK, now commit destination to socket.  */
	sk->sk_gso_type = SKB_GSO_TCPV4;
	sk_setup_caps(sk, &rt->dst);

	if (!tp->write_seq)
		tp->write_seq = secure_tcp_sequence_number(inet->inet_saddr,
							   inet->inet_daddr,
							   inet->inet_sport,
							   usin->sin_port);

	inet->inet_id = tp->write_seq ^ jiffies;

	err = tcp_connect(sk);
	rt = NULL;
=======
	tp->tcp_usec_ts = dst_tcp_usec_ts(&rt->dst);
	/* OK, now commit destination to socket.  */
	sk->sk_gso_type = SKB_GSO_TCPV4;
	sk_setup_caps(sk, &rt->dst);
	rt = NULL;

	if (likely(!tp->repair)) {
		if (!tp->write_seq)
			WRITE_ONCE(tp->write_seq,
				   secure_tcp_seq(inet->inet_saddr,
						  inet->inet_daddr,
						  inet->inet_sport,
						  usin->sin_port));
		WRITE_ONCE(tp->tsoffset,
			   secure_tcp_ts_off(net, inet->inet_saddr,
					     inet->inet_daddr));
	}

	atomic_set(&inet->inet_id, get_random_u16());

	if (tcp_fastopen_defer_connect(sk, &err))
		return err;
	if (err)
		goto failure;

	err = tcp_connect(sk);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err)
		goto failure;

	return 0;

failure:
	/*
	 * This unhashes the socket and releases the local port,
	 * if necessary.
	 */
	tcp_set_state(sk, TCP_CLOSE);
<<<<<<< HEAD
=======
	inet_bhash2_reset_saddr(sk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ip_rt_put(rt);
	sk->sk_route_caps = 0;
	inet->inet_dport = 0;
	return err;
}
EXPORT_SYMBOL(tcp_v4_connect);

/*
<<<<<<< HEAD
 * This routine does path mtu discovery as defined in RFC1191.
 */
static void do_pmtu_discovery(struct sock *sk, const struct iphdr *iph, u32 mtu)
{
	struct dst_entry *dst;
	struct inet_sock *inet = inet_sk(sk);

	/* We are not interested in TCP_LISTEN and open_requests (SYN-ACKs
	 * send out by Linux are always <576bytes so they should go through
	 * unfragmented).
	 */
	if (sk->sk_state == TCP_LISTEN)
		return;

	/* We don't check in the destentry if pmtu discovery is forbidden
	 * on this route. We just assume that no packet_to_big packets
	 * are send back when pmtu discovery is not active.
	 * There is a small race when the user changes this flag in the
	 * route, but I think that's acceptable.
	 */
	if ((dst = __sk_dst_check(sk, 0)) == NULL)
		return;

	dst->ops->update_pmtu(dst, mtu);

=======
 * This routine reacts to ICMP_FRAG_NEEDED mtu indications as defined in RFC1191.
 * It can be called through tcp_release_cb() if socket was owned by user
 * at the time tcp_v4_err() was called to handle ICMP message.
 */
void tcp_v4_mtu_reduced(struct sock *sk)
{
	struct inet_sock *inet = inet_sk(sk);
	struct dst_entry *dst;
	u32 mtu;

	if ((1 << sk->sk_state) & (TCPF_LISTEN | TCPF_CLOSE))
		return;
	mtu = READ_ONCE(tcp_sk(sk)->mtu_info);
	dst = inet_csk_update_pmtu(sk, mtu);
	if (!dst)
		return;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Something is about to be wrong... Remember soft error
	 * for the case, if this connection will not able to recover.
	 */
	if (mtu < dst_mtu(dst) && ip_dont_fragment(sk, dst))
<<<<<<< HEAD
		sk->sk_err_soft = EMSGSIZE;
=======
		WRITE_ONCE(sk->sk_err_soft, EMSGSIZE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	mtu = dst_mtu(dst);

	if (inet->pmtudisc != IP_PMTUDISC_DONT &&
<<<<<<< HEAD
=======
	    ip_sk_accept_pmtu(sk) &&
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	    inet_csk(sk)->icsk_pmtu_cookie > mtu) {
		tcp_sync_mss(sk, mtu);

		/* Resend the TCP packet because it's
		 * clear that the old packet has been
		 * dropped. This is the new "fast" path mtu
		 * discovery.
		 */
		tcp_simple_retransmit(sk);
	} /* else let the usual retransmit timer handle it */
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL(tcp_v4_mtu_reduced);

static void do_redirect(struct sk_buff *skb, struct sock *sk)
{
	struct dst_entry *dst = __sk_dst_check(sk, 0);

	if (dst)
		dst->ops->redirect(dst, sk, skb);
}


/* handle ICMP messages on TCP_NEW_SYN_RECV request sockets */
void tcp_req_err(struct sock *sk, u32 seq, bool abort)
{
	struct request_sock *req = inet_reqsk(sk);
	struct net *net = sock_net(sk);

	/* ICMPs are not backlogged, hence we cannot get
	 * an established socket here.
	 */
	if (seq != tcp_rsk(req)->snt_isn) {
		__NET_INC_STATS(net, LINUX_MIB_OUTOFWINDOWICMPS);
	} else if (abort) {
		/*
		 * Still in SYN_RECV, just remove it silently.
		 * There is no good way to pass the error to the newly
		 * created socket, and POSIX does not want network
		 * errors returned from accept().
		 */
		inet_csk_reqsk_queue_drop(req->rsk_listener, req);
		tcp_listendrop(req->rsk_listener);
	}
	reqsk_put(req);
}
EXPORT_SYMBOL(tcp_req_err);

/* TCP-LD (RFC 6069) logic */
void tcp_ld_RTO_revert(struct sock *sk, u32 seq)
{
	struct inet_connection_sock *icsk = inet_csk(sk);
	struct tcp_sock *tp = tcp_sk(sk);
	struct sk_buff *skb;
	s32 remaining;
	u32 delta_us;

	if (sock_owned_by_user(sk))
		return;

	if (seq != tp->snd_una  || !icsk->icsk_retransmits ||
	    !icsk->icsk_backoff)
		return;

	skb = tcp_rtx_queue_head(sk);
	if (WARN_ON_ONCE(!skb))
		return;

	icsk->icsk_backoff--;
	icsk->icsk_rto = tp->srtt_us ? __tcp_set_rto(tp) : TCP_TIMEOUT_INIT;
	icsk->icsk_rto = inet_csk_rto_backoff(icsk, TCP_RTO_MAX);

	tcp_mstamp_refresh(tp);
	delta_us = (u32)(tp->tcp_mstamp - tcp_skb_timestamp_us(skb));
	remaining = icsk->icsk_rto - usecs_to_jiffies(delta_us);

	if (remaining > 0) {
		inet_csk_reset_xmit_timer(sk, ICSK_TIME_RETRANS,
					  remaining, TCP_RTO_MAX);
	} else {
		/* RTO revert clocked out retransmission.
		 * Will retransmit now.
		 */
		tcp_retransmit_timer(sk);
	}
}
EXPORT_SYMBOL(tcp_ld_RTO_revert);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * This routine is called by the ICMP module when it gets some
 * sort of error condition.  If err < 0 then the socket should
 * be closed and the error returned to the user.  If err > 0
 * it's just the icmp type << 8 | icmp code.  After adjustment
 * header points to the first 8 bytes of the tcp header.  We need
 * to find the appropriate port.
 *
 * The locking strategy used here is very "optimistic". When
 * someone else accesses the socket the ICMP is just dropped
 * and for some paths there is no check at all.
 * A more general error queue to queue errors for later handling
 * is probably better.
 *
 */

<<<<<<< HEAD
void tcp_v4_err(struct sk_buff *icmp_skb, u32 info)
{
	const struct iphdr *iph = (const struct iphdr *)icmp_skb->data;
	struct tcphdr *th = (struct tcphdr *)(icmp_skb->data + (iph->ihl << 2));
	struct inet_connection_sock *icsk;
	struct tcp_sock *tp;
	struct inet_sock *inet;
	const int type = icmp_hdr(icmp_skb)->type;
	const int code = icmp_hdr(icmp_skb)->code;
	struct sock *sk;
	struct sk_buff *skb;
	__u32 seq;
	__u32 remaining;
	int err;
	struct net *net = dev_net(icmp_skb->dev);

	if (icmp_skb->len < (iph->ihl << 2) + 8) {
		ICMP_INC_STATS_BH(net, ICMP_MIB_INERRORS);
		return;
	}

	sk = inet_lookup(net, &tcp_hashinfo, iph->daddr, th->dest,
			iph->saddr, th->source, inet_iif(icmp_skb));
	if (!sk) {
		ICMP_INC_STATS_BH(net, ICMP_MIB_INERRORS);
		return;
	}
	if (sk->sk_state == TCP_TIME_WAIT) {
		inet_twsk_put(inet_twsk(sk));
		return;
=======
int tcp_v4_err(struct sk_buff *skb, u32 info)
{
	const struct iphdr *iph = (const struct iphdr *)skb->data;
	struct tcphdr *th = (struct tcphdr *)(skb->data + (iph->ihl << 2));
	struct tcp_sock *tp;
	const int type = icmp_hdr(skb)->type;
	const int code = icmp_hdr(skb)->code;
	struct sock *sk;
	struct request_sock *fastopen;
	u32 seq, snd_una;
	int err;
	struct net *net = dev_net(skb->dev);

	sk = __inet_lookup_established(net, net->ipv4.tcp_death_row.hashinfo,
				       iph->daddr, th->dest, iph->saddr,
				       ntohs(th->source), inet_iif(skb), 0);
	if (!sk) {
		__ICMP_INC_STATS(net, ICMP_MIB_INERRORS);
		return -ENOENT;
	}
	if (sk->sk_state == TCP_TIME_WAIT) {
		/* To increase the counter of ignored icmps for TCP-AO */
		tcp_ao_ignore_icmp(sk, AF_INET, type, code);
		inet_twsk_put(inet_twsk(sk));
		return 0;
	}
	seq = ntohl(th->seq);
	if (sk->sk_state == TCP_NEW_SYN_RECV) {
		tcp_req_err(sk, seq, type == ICMP_PARAMETERPROB ||
				     type == ICMP_TIME_EXCEEDED ||
				     (type == ICMP_DEST_UNREACH &&
				      (code == ICMP_NET_UNREACH ||
				       code == ICMP_HOST_UNREACH)));
		return 0;
	}

	if (tcp_ao_ignore_icmp(sk, AF_INET, type, code)) {
		sock_put(sk);
		return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	bh_lock_sock(sk);
	/* If too many ICMPs get dropped on busy
	 * servers this needs to be solved differently.
<<<<<<< HEAD
	 */
	if (sock_owned_by_user(sk))
		NET_INC_STATS_BH(net, LINUX_MIB_LOCKDROPPEDICMPS);

	if (sk->sk_state == TCP_CLOSE)
		goto out;

	if (unlikely(iph->ttl < inet_sk(sk)->min_ttl)) {
		NET_INC_STATS_BH(net, LINUX_MIB_TCPMINTTLDROP);
		goto out;
	}

	icsk = inet_csk(sk);
	tp = tcp_sk(sk);
	seq = ntohl(th->seq);
	if (sk->sk_state != TCP_LISTEN &&
	    !between(seq, tp->snd_una, tp->snd_nxt)) {
		NET_INC_STATS_BH(net, LINUX_MIB_OUTOFWINDOWICMPS);
=======
	 * We do take care of PMTU discovery (RFC1191) special case :
	 * we can receive locally generated ICMP messages while socket is held.
	 */
	if (sock_owned_by_user(sk)) {
		if (!(type == ICMP_DEST_UNREACH && code == ICMP_FRAG_NEEDED))
			__NET_INC_STATS(net, LINUX_MIB_LOCKDROPPEDICMPS);
	}
	if (sk->sk_state == TCP_CLOSE)
		goto out;

	if (static_branch_unlikely(&ip4_min_ttl)) {
		/* min_ttl can be changed concurrently from do_ip_setsockopt() */
		if (unlikely(iph->ttl < READ_ONCE(inet_sk(sk)->min_ttl))) {
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
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;
	}

	switch (type) {
<<<<<<< HEAD
=======
	case ICMP_REDIRECT:
		if (!sock_owned_by_user(sk))
			do_redirect(skb, sk);
		goto out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case ICMP_SOURCE_QUENCH:
		/* Just silently ignore these. */
		goto out;
	case ICMP_PARAMETERPROB:
		err = EPROTO;
		break;
	case ICMP_DEST_UNREACH:
		if (code > NR_ICMP_UNREACH)
			goto out;

		if (code == ICMP_FRAG_NEEDED) { /* PMTU discovery (RFC1191) */
<<<<<<< HEAD
			if (!sock_owned_by_user(sk))
				do_pmtu_discovery(sk, iph, info);
=======
			/* We are not interested in TCP_LISTEN and open_requests
			 * (SYN-ACKs send out by Linux are always <576bytes so
			 * they should go through unfragmented).
			 */
			if (sk->sk_state == TCP_LISTEN)
				goto out;

			WRITE_ONCE(tp->mtu_info, info);
			if (!sock_owned_by_user(sk)) {
				tcp_v4_mtu_reduced(sk);
			} else {
				if (!test_and_set_bit(TCP_MTU_REDUCED_DEFERRED, &sk->sk_tsq_flags))
					sock_hold(sk);
			}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto out;
		}

		err = icmp_err_convert[code].errno;
<<<<<<< HEAD
		/* check if icmp_skb allows revert of backoff
		 * (see draft-zimmermann-tcp-lcd) */
		if (code != ICMP_NET_UNREACH && code != ICMP_HOST_UNREACH)
			break;
		if (seq != tp->snd_una  || !icsk->icsk_retransmits ||
		    !icsk->icsk_backoff)
			break;

		if (sock_owned_by_user(sk))
			break;

		icsk->icsk_backoff--;
		inet_csk(sk)->icsk_rto = (tp->srtt ? __tcp_set_rto(tp) :
			TCP_TIMEOUT_INIT) << icsk->icsk_backoff;
		tcp_bound_rto(sk);

		skb = tcp_write_queue_head(sk);
		BUG_ON(!skb);

		remaining = icsk->icsk_rto - min(icsk->icsk_rto,
				tcp_time_stamp - TCP_SKB_CB(skb)->when);

		if (remaining) {
			inet_csk_reset_xmit_timer(sk, ICSK_TIME_RETRANS,
						  remaining, TCP_RTO_MAX);
		} else {
			/* RTO revert clocked out retransmission.
			 * Will retransmit now */
			tcp_retransmit_timer(sk);
		}

=======
		/* check if this ICMP message allows revert of backoff.
		 * (see RFC 6069)
		 */
		if (!fastopen &&
		    (code == ICMP_NET_UNREACH || code == ICMP_HOST_UNREACH))
			tcp_ld_RTO_revert(sk, seq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case ICMP_TIME_EXCEEDED:
		err = EHOSTUNREACH;
		break;
	default:
		goto out;
	}

	switch (sk->sk_state) {
<<<<<<< HEAD
		struct request_sock *req, **prev;
	case TCP_LISTEN:
		if (sock_owned_by_user(sk))
			goto out;

		req = inet_csk_search_req(sk, &prev, th->dest,
					  iph->daddr, iph->saddr);
		if (!req)
			goto out;

		/* ICMPs are not backlogged, hence we cannot get
		   an established socket here.
		 */
		WARN_ON(req->sk);

		if (seq != tcp_rsk(req)->snt_isn) {
			NET_INC_STATS_BH(net, LINUX_MIB_OUTOFWINDOWICMPS);
			goto out;
		}

		/*
		 * Still in SYN_RECV, just remove it silently.
		 * There is no good way to pass the error to the newly
		 * created socket, and POSIX does not want network
		 * errors returned from accept().
		 */
		inet_csk_reqsk_queue_drop(sk, req, prev);
		goto out;

	case TCP_SYN_SENT:
	case TCP_SYN_RECV:  /* Cannot happen.
			       It can f.e. if SYNs crossed.
			     */
		if (!sock_owned_by_user(sk)) {
			sk->sk_err = err;

			sk->sk_error_report(sk);

			tcp_done(sk);
		} else {
			sk->sk_err_soft = err;
=======
	case TCP_SYN_SENT:
	case TCP_SYN_RECV:
		/* Only in fast or simultaneous open. If a fast open socket is
		 * already accepted it is treated as a connected one below.
		 */
		if (fastopen && !fastopen->sk)
			break;

		ip_icmp_error(sk, skb, err, th->dest, info, (u8 *)th);

		if (!sock_owned_by_user(sk)) {
			WRITE_ONCE(sk->sk_err, err);

			sk_error_report(sk);

			tcp_done(sk);
		} else {
			WRITE_ONCE(sk->sk_err_soft, err);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		goto out;
	}

	/* If we've already connected we will keep trying
	 * until we time out, or the user gives up.
	 *
	 * rfc1122 4.2.3.9 allows to consider as hard errors
	 * only PROTO_UNREACH and PORT_UNREACH (well, FRAG_FAILED too,
	 * but it is obsoleted by pmtu discovery).
	 *
	 * Note, that in modern internet, where routing is unreliable
	 * and in each dark corner broken firewalls sit, sending random
	 * errors ordered by their masters even this two messages finally lose
	 * their original sense (even Linux sends invalid PORT_UNREACHs)
	 *
	 * Now we are in compliance with RFCs.
	 *							--ANK (980905)
	 */

<<<<<<< HEAD
	inet = inet_sk(sk);
	if (!sock_owned_by_user(sk) && inet->recverr) {
		sk->sk_err = err;
		sk->sk_error_report(sk);
	} else	{ /* Only an error on timeout */
		sk->sk_err_soft = err;
=======
	if (!sock_owned_by_user(sk) &&
	    inet_test_bit(RECVERR, sk)) {
		WRITE_ONCE(sk->sk_err, err);
		sk_error_report(sk);
	} else	{ /* Only an error on timeout */
		WRITE_ONCE(sk->sk_err_soft, err);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

out:
	bh_unlock_sock(sk);
	sock_put(sk);
<<<<<<< HEAD
}

static void __tcp_v4_send_check(struct sk_buff *skb,
				__be32 saddr, __be32 daddr)
{
	struct tcphdr *th = tcp_hdr(skb);

	if (skb->ip_summed == CHECKSUM_PARTIAL) {
		th->check = ~tcp_v4_check(skb->len, saddr, daddr, 0);
		skb->csum_start = skb_transport_header(skb) - skb->head;
		skb->csum_offset = offsetof(struct tcphdr, check);
	} else {
		th->check = tcp_v4_check(skb->len, saddr, daddr,
					 csum_partial(th,
						      th->doff << 2,
						      skb->csum));
	}
=======
	return 0;
}

void __tcp_v4_send_check(struct sk_buff *skb, __be32 saddr, __be32 daddr)
{
	struct tcphdr *th = tcp_hdr(skb);

	th->check = ~tcp_v4_check(skb->len, saddr, daddr, 0);
	skb->csum_start = skb_transport_header(skb) - skb->head;
	skb->csum_offset = offsetof(struct tcphdr, check);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* This routine computes an IPv4 TCP checksum. */
void tcp_v4_send_check(struct sock *sk, struct sk_buff *skb)
{
	const struct inet_sock *inet = inet_sk(sk);

	__tcp_v4_send_check(skb, inet->inet_saddr, inet->inet_daddr);
}
EXPORT_SYMBOL(tcp_v4_send_check);

<<<<<<< HEAD
int tcp_v4_gso_send_check(struct sk_buff *skb)
{
	const struct iphdr *iph;
	struct tcphdr *th;

	if (!pskb_may_pull(skb, sizeof(*th)))
		return -EINVAL;

	iph = ip_hdr(skb);
	th = tcp_hdr(skb);

	th->check = 0;
	skb->ip_summed = CHECKSUM_PARTIAL;
	__tcp_v4_send_check(skb, iph->saddr, iph->daddr);
	return 0;
=======
#define REPLY_OPTIONS_LEN      (MAX_TCP_OPTION_SPACE / sizeof(__be32))

static bool tcp_v4_ao_sign_reset(const struct sock *sk, struct sk_buff *skb,
				 const struct tcp_ao_hdr *aoh,
				 struct ip_reply_arg *arg, struct tcphdr *reply,
				 __be32 reply_options[REPLY_OPTIONS_LEN])
{
#ifdef CONFIG_TCP_AO
	int sdif = tcp_v4_sdif(skb);
	int dif = inet_iif(skb);
	int l3index = sdif ? dif : 0;
	bool allocated_traffic_key;
	struct tcp_ao_key *key;
	char *traffic_key;
	bool drop = true;
	u32 ao_sne = 0;
	u8 keyid;

	rcu_read_lock();
	if (tcp_ao_prepare_reset(sk, skb, aoh, l3index, ntohl(reply->seq),
				 &key, &traffic_key, &allocated_traffic_key,
				 &keyid, &ao_sne))
		goto out;

	reply_options[0] = htonl((TCPOPT_AO << 24) | (tcp_ao_len(key) << 16) |
				 (aoh->rnext_keyid << 8) | keyid);
	arg->iov[0].iov_len += tcp_ao_len_aligned(key);
	reply->doff = arg->iov[0].iov_len / 4;

	if (tcp_ao_hash_hdr(AF_INET, (char *)&reply_options[1],
			    key, traffic_key,
			    (union tcp_ao_addr *)&ip_hdr(skb)->saddr,
			    (union tcp_ao_addr *)&ip_hdr(skb)->daddr,
			    reply, ao_sne))
		goto out;
	drop = false;
out:
	rcu_read_unlock();
	if (allocated_traffic_key)
		kfree(traffic_key);
	return drop;
#else
	return true;
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 *	This routine will send an RST to the other tcp.
 *
 *	Someone asks: why I NEVER use socket parameters (TOS, TTL etc.)
 *		      for reset.
 *	Answer: if a packet caused RST, it is not for a socket
 *		existing in our system, if it is matched to a socket,
 *		it is just duplicate segment or bug in other side's TCP.
 *		So that we build reply only basing on parameters
 *		arrived with segment.
 *	Exception: precedence violation. We do not implement it in any case.
 */

<<<<<<< HEAD
static void tcp_v4_send_reset(struct sock *sk, struct sk_buff *skb)
=======
static void tcp_v4_send_reset(const struct sock *sk, struct sk_buff *skb)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	const struct tcphdr *th = tcp_hdr(skb);
	struct {
		struct tcphdr th;
<<<<<<< HEAD
#ifdef CONFIG_TCP_MD5SIG
		__be32 opt[(TCPOLEN_MD5SIG_ALIGNED >> 2)];
#endif
	} rep;
	struct ip_reply_arg arg;
#ifdef CONFIG_TCP_MD5SIG
	struct tcp_md5sig_key *key;
	const __u8 *hash_location = NULL;
	unsigned char newhash[16];
	int genhash;
	struct sock *sk1 = NULL;
#endif
	struct net *net;
=======
		__be32 opt[REPLY_OPTIONS_LEN];
	} rep;
	const __u8 *md5_hash_location = NULL;
	const struct tcp_ao_hdr *aoh;
	struct ip_reply_arg arg;
#ifdef CONFIG_TCP_MD5SIG
	struct tcp_md5sig_key *key = NULL;
	unsigned char newhash[16];
	struct sock *sk1 = NULL;
	int genhash;
#endif
	u64 transmit_time = 0;
	struct sock *ctl_sk;
	struct net *net;
	u32 txhash = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Never send a reset in response to a reset. */
	if (th->rst)
		return;

	/* If sk not NULL, it means we did a successful lookup and incoming
	 * route had to be correct. prequeue might have dropped our dst.
	 */
	if (!sk && skb_rtable(skb)->rt_type != RTN_LOCAL)
		return;

	/* Swap the send and the receive. */
	memset(&rep, 0, sizeof(rep));
	rep.th.dest   = th->source;
	rep.th.source = th->dest;
	rep.th.doff   = sizeof(struct tcphdr) / 4;
	rep.th.rst    = 1;

	if (th->ack) {
		rep.th.seq = th->ack_seq;
	} else {
		rep.th.ack = 1;
		rep.th.ack_seq = htonl(ntohl(th->seq) + th->syn + th->fin +
				       skb->len - (th->doff << 2));
	}

	memset(&arg, 0, sizeof(arg));
	arg.iov[0].iov_base = (unsigned char *)&rep;
	arg.iov[0].iov_len  = sizeof(rep.th);

	net = sk ? sock_net(sk) : dev_net(skb_dst(skb)->dev);
<<<<<<< HEAD
#ifdef CONFIG_TCP_MD5SIG
	hash_location = tcp_parse_md5sig_option(th);
	if (!sk && hash_location) {
=======

	/* Invalid TCP option size or twice included auth */
	if (tcp_parse_auth_options(tcp_hdr(skb), &md5_hash_location, &aoh))
		return;

	if (aoh && tcp_v4_ao_sign_reset(sk, skb, aoh, &arg, &rep.th, rep.opt))
		return;

#ifdef CONFIG_TCP_MD5SIG
	rcu_read_lock();
	if (sk && sk_fullsock(sk)) {
		const union tcp_md5_addr *addr;
		int l3index;

		/* sdif set, means packet ingressed via a device
		 * in an L3 domain and inet_iif is set to it.
		 */
		l3index = tcp_v4_sdif(skb) ? inet_iif(skb) : 0;
		addr = (union tcp_md5_addr *)&ip_hdr(skb)->saddr;
		key = tcp_md5_do_lookup(sk, l3index, addr, AF_INET);
	} else if (md5_hash_location) {
		const union tcp_md5_addr *addr;
		int sdif = tcp_v4_sdif(skb);
		int dif = inet_iif(skb);
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
		sk1 = __inet_lookup_listener(net,
					     &tcp_hashinfo, ip_hdr(skb)->daddr,
					     ntohs(th->source), inet_iif(skb));
		/* don't send rst if it can't find key */
		if (!sk1)
			return;
		rcu_read_lock();
		key = tcp_md5_do_lookup(sk1, (union tcp_md5_addr *)
					&ip_hdr(skb)->saddr, AF_INET);
		if (!key)
			goto release_sk1;

		genhash = tcp_v4_md5_hash_skb(newhash, key, NULL, NULL, skb);
		if (genhash || memcmp(hash_location, newhash, 16) != 0)
			goto release_sk1;
	} else {
		key = sk ? tcp_md5_do_lookup(sk, (union tcp_md5_addr *)
					     &ip_hdr(skb)->saddr,
					     AF_INET) : NULL;
=======
		sk1 = __inet_lookup_listener(net, net->ipv4.tcp_death_row.hashinfo,
					     NULL, 0, ip_hdr(skb)->saddr,
					     th->source, ip_hdr(skb)->daddr,
					     ntohs(th->source), dif, sdif);
		/* don't send rst if it can't find key */
		if (!sk1)
			goto out;

		/* sdif set, means packet ingressed via a device
		 * in an L3 domain and dif is set to it.
		 */
		l3index = sdif ? dif : 0;
		addr = (union tcp_md5_addr *)&ip_hdr(skb)->saddr;
		key = tcp_md5_do_lookup(sk1, l3index, addr, AF_INET);
		if (!key)
			goto out;


		genhash = tcp_v4_md5_hash_skb(newhash, key, NULL, skb);
		if (genhash || memcmp(md5_hash_location, newhash, 16) != 0)
			goto out;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (key) {
		rep.opt[0] = htonl((TCPOPT_NOP << 24) |
				   (TCPOPT_NOP << 16) |
				   (TCPOPT_MD5SIG << 8) |
				   TCPOLEN_MD5SIG);
		/* Update length and the length the header thinks exists */
		arg.iov[0].iov_len += TCPOLEN_MD5SIG_ALIGNED;
		rep.th.doff = arg.iov[0].iov_len / 4;

		tcp_v4_md5_hash_hdr((__u8 *) &rep.opt[1],
				     key, ip_hdr(skb)->saddr,
				     ip_hdr(skb)->daddr, &rep.th);
	}
#endif
<<<<<<< HEAD
=======
	/* Can't co-exist with TCPMD5, hence check rep.opt[0] */
	if (rep.opt[0] == 0) {
		__be32 mrst = mptcp_reset_option(skb);

		if (mrst) {
			rep.opt[0] = mrst;
			arg.iov[0].iov_len += sizeof(mrst);
			rep.th.doff = arg.iov[0].iov_len / 4;
		}
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	arg.csum = csum_tcpudp_nofold(ip_hdr(skb)->daddr,
				      ip_hdr(skb)->saddr, /* XXX */
				      arg.iov[0].iov_len, IPPROTO_TCP, 0);
	arg.csumoffset = offsetof(struct tcphdr, check) / 2;
<<<<<<< HEAD
	arg.flags = (sk && inet_sk(sk)->transparent) ? IP_REPLY_ARG_NOSRCCHECK : 0;
=======
	arg.flags = (sk && inet_sk_transparent(sk)) ? IP_REPLY_ARG_NOSRCCHECK : 0;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* When socket is gone, all binding information is lost.
	 * routing might fail in this case. No choice here, if we choose to force
	 * input interface, we will misroute in case of asymmetric route.
	 */
<<<<<<< HEAD
	if (sk)
		arg.bound_dev_if = sk->sk_bound_dev_if;

	arg.tos = ip_hdr(skb)->tos;
	arg.uid = sock_net_uid(net, sk && sk_fullsock(sk) ? sk : NULL);
	ip_send_reply(net->ipv4.tcp_sock, skb, ip_hdr(skb)->saddr,
		      &arg, arg.iov[0].iov_len);

	TCP_INC_STATS_BH(net, TCP_MIB_OUTSEGS);
	TCP_INC_STATS_BH(net, TCP_MIB_OUTRSTS);

#ifdef CONFIG_TCP_MD5SIG
release_sk1:
	if (sk1) {
		rcu_read_unlock();
		sock_put(sk1);
	}
=======
	if (sk) {
		arg.bound_dev_if = sk->sk_bound_dev_if;
		if (sk_fullsock(sk))
			trace_tcp_send_reset(sk, skb);
	}

	BUILD_BUG_ON(offsetof(struct sock, sk_bound_dev_if) !=
		     offsetof(struct inet_timewait_sock, tw_bound_dev_if));

	arg.tos = ip_hdr(skb)->tos;
	arg.uid = sock_net_uid(net, sk && sk_fullsock(sk) ? sk : NULL);
	local_bh_disable();
	ctl_sk = this_cpu_read(ipv4_tcp_sk);
	sock_net_set(ctl_sk, net);
	if (sk) {
		ctl_sk->sk_mark = (sk->sk_state == TCP_TIME_WAIT) ?
				   inet_twsk(sk)->tw_mark : sk->sk_mark;
		ctl_sk->sk_priority = (sk->sk_state == TCP_TIME_WAIT) ?
				   inet_twsk(sk)->tw_priority : READ_ONCE(sk->sk_priority);
		transmit_time = tcp_transmit_time(sk);
		xfrm_sk_clone_policy(ctl_sk, sk);
		txhash = (sk->sk_state == TCP_TIME_WAIT) ?
			 inet_twsk(sk)->tw_txhash : sk->sk_txhash;
	} else {
		ctl_sk->sk_mark = 0;
		ctl_sk->sk_priority = 0;
	}
	ip_send_unicast_reply(ctl_sk,
			      skb, &TCP_SKB_CB(skb)->header.h4.opt,
			      ip_hdr(skb)->saddr, ip_hdr(skb)->daddr,
			      &arg, arg.iov[0].iov_len,
			      transmit_time, txhash);

	xfrm_sk_free_policy(ctl_sk);
	sock_net_set(ctl_sk, &init_net);
	__TCP_INC_STATS(net, TCP_MIB_OUTSEGS);
	__TCP_INC_STATS(net, TCP_MIB_OUTRSTS);
	local_bh_enable();

#ifdef CONFIG_TCP_MD5SIG
out:
	rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
}

/* The code following below sending ACKs in SYN-RECV and TIME-WAIT states
   outside socket context is ugly, certainly. What can I do?
 */

<<<<<<< HEAD
static void tcp_v4_send_ack(const struct sock *sk, struct sk_buff *skb,
			    u32 seq, u32 ack,
			    u32 win, u32 ts, int oif,
			    struct tcp_md5sig_key *key,
			    int reply_flags, u8 tos)
=======
static void tcp_v4_send_ack(const struct sock *sk,
			    struct sk_buff *skb, u32 seq, u32 ack,
			    u32 win, u32 tsval, u32 tsecr, int oif,
			    struct tcp_key *key,
			    int reply_flags, u8 tos, u32 txhash)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	const struct tcphdr *th = tcp_hdr(skb);
	struct {
		struct tcphdr th;
<<<<<<< HEAD
		__be32 opt[(TCPOLEN_TSTAMP_ALIGNED >> 2)
#ifdef CONFIG_TCP_MD5SIG
			   + (TCPOLEN_MD5SIG_ALIGNED >> 2)
#endif
			];
	} rep;
	struct ip_reply_arg arg;
	struct net *net = sock_net(sk);
=======
		__be32 opt[(MAX_TCP_OPTION_SPACE  >> 2)];
	} rep;
	struct net *net = sock_net(sk);
	struct ip_reply_arg arg;
	struct sock *ctl_sk;
	u64 transmit_time;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	memset(&rep.th, 0, sizeof(struct tcphdr));
	memset(&arg, 0, sizeof(arg));

	arg.iov[0].iov_base = (unsigned char *)&rep;
	arg.iov[0].iov_len  = sizeof(rep.th);
<<<<<<< HEAD
	if (ts) {
		rep.opt[0] = htonl((TCPOPT_NOP << 24) | (TCPOPT_NOP << 16) |
				   (TCPOPT_TIMESTAMP << 8) |
				   TCPOLEN_TIMESTAMP);
		rep.opt[1] = htonl(tcp_time_stamp);
		rep.opt[2] = htonl(ts);
=======
	if (tsecr) {
		rep.opt[0] = htonl((TCPOPT_NOP << 24) | (TCPOPT_NOP << 16) |
				   (TCPOPT_TIMESTAMP << 8) |
				   TCPOLEN_TIMESTAMP);
		rep.opt[1] = htonl(tsval);
		rep.opt[2] = htonl(tsecr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		arg.iov[0].iov_len += TCPOLEN_TSTAMP_ALIGNED;
	}

	/* Swap the send and the receive. */
	rep.th.dest    = th->source;
	rep.th.source  = th->dest;
	rep.th.doff    = arg.iov[0].iov_len / 4;
	rep.th.seq     = htonl(seq);
	rep.th.ack_seq = htonl(ack);
	rep.th.ack     = 1;
	rep.th.window  = htons(win);

#ifdef CONFIG_TCP_MD5SIG
<<<<<<< HEAD
	if (key) {
		int offset = (ts) ? 3 : 0;
=======
	if (tcp_key_is_md5(key)) {
		int offset = (tsecr) ? 3 : 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		rep.opt[offset++] = htonl((TCPOPT_NOP << 24) |
					  (TCPOPT_NOP << 16) |
					  (TCPOPT_MD5SIG << 8) |
					  TCPOLEN_MD5SIG);
		arg.iov[0].iov_len += TCPOLEN_MD5SIG_ALIGNED;
		rep.th.doff = arg.iov[0].iov_len/4;

		tcp_v4_md5_hash_hdr((__u8 *) &rep.opt[offset],
<<<<<<< HEAD
				    key, ip_hdr(skb)->saddr,
				    ip_hdr(skb)->daddr, &rep.th);
	}
#endif
=======
				    key->md5_key, ip_hdr(skb)->saddr,
				    ip_hdr(skb)->daddr, &rep.th);
	}
#endif
#ifdef CONFIG_TCP_AO
	if (tcp_key_is_ao(key)) {
		int offset = (tsecr) ? 3 : 0;

		rep.opt[offset++] = htonl((TCPOPT_AO << 24) |
					  (tcp_ao_len(key->ao_key) << 16) |
					  (key->ao_key->sndid << 8) |
					  key->rcv_next);
		arg.iov[0].iov_len += tcp_ao_len_aligned(key->ao_key);
		rep.th.doff = arg.iov[0].iov_len / 4;

		tcp_ao_hash_hdr(AF_INET, (char *)&rep.opt[offset],
				key->ao_key, key->traffic_key,
				(union tcp_ao_addr *)&ip_hdr(skb)->saddr,
				(union tcp_ao_addr *)&ip_hdr(skb)->daddr,
				&rep.th, key->sne);
	}
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	arg.flags = reply_flags;
	arg.csum = csum_tcpudp_nofold(ip_hdr(skb)->daddr,
				      ip_hdr(skb)->saddr, /* XXX */
				      arg.iov[0].iov_len, IPPROTO_TCP, 0);
	arg.csumoffset = offsetof(struct tcphdr, check) / 2;
	if (oif)
		arg.bound_dev_if = oif;
	arg.tos = tos;
	arg.uid = sock_net_uid(net, sk_fullsock(sk) ? sk : NULL);
<<<<<<< HEAD
	ip_send_reply(net->ipv4.tcp_sock, skb, ip_hdr(skb)->saddr,
		      &arg, arg.iov[0].iov_len);

	TCP_INC_STATS_BH(net, TCP_MIB_OUTSEGS);
=======
	local_bh_disable();
	ctl_sk = this_cpu_read(ipv4_tcp_sk);
	sock_net_set(ctl_sk, net);
	ctl_sk->sk_mark = (sk->sk_state == TCP_TIME_WAIT) ?
			   inet_twsk(sk)->tw_mark : READ_ONCE(sk->sk_mark);
	ctl_sk->sk_priority = (sk->sk_state == TCP_TIME_WAIT) ?
			   inet_twsk(sk)->tw_priority : READ_ONCE(sk->sk_priority);
	transmit_time = tcp_transmit_time(sk);
	ip_send_unicast_reply(ctl_sk,
			      skb, &TCP_SKB_CB(skb)->header.h4.opt,
			      ip_hdr(skb)->saddr, ip_hdr(skb)->daddr,
			      &arg, arg.iov[0].iov_len,
			      transmit_time, txhash);

	sock_net_set(ctl_sk, &init_net);
	__TCP_INC_STATS(net, TCP_MIB_OUTSEGS);
	local_bh_enable();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void tcp_v4_timewait_ack(struct sock *sk, struct sk_buff *skb)
{
	struct inet_timewait_sock *tw = inet_twsk(sk);
	struct tcp_timewait_sock *tcptw = tcp_twsk(sk);
<<<<<<< HEAD

	tcp_v4_send_ack(sk, skb, tcptw->tw_snd_nxt, tcptw->tw_rcv_nxt,
			tcptw->tw_rcv_wnd >> tw->tw_rcv_wscale,
			tcptw->tw_ts_recent,
			tw->tw_bound_dev_if,
			tcp_twsk_md5_key(tcptw),
			tw->tw_transparent ? IP_REPLY_ARG_NOSRCCHECK : 0,
			tw->tw_tos
			);
=======
	struct tcp_key key = {};
#ifdef CONFIG_TCP_AO
	struct tcp_ao_info *ao_info;

	if (static_branch_unlikely(&tcp_ao_needed.key)) {
		/* FIXME: the segment to-be-acked is not verified yet */
		ao_info = rcu_dereference(tcptw->ao_info);
		if (ao_info) {
			const struct tcp_ao_hdr *aoh;

			if (tcp_parse_auth_options(tcp_hdr(skb), NULL, &aoh)) {
				inet_twsk_put(tw);
				return;
			}

			if (aoh)
				key.ao_key = tcp_ao_established_key(ao_info, aoh->rnext_keyid, -1);
		}
	}
	if (key.ao_key) {
		struct tcp_ao_key *rnext_key;

		key.traffic_key = snd_other_key(key.ao_key);
		key.sne = READ_ONCE(ao_info->snd_sne);
		rnext_key = READ_ONCE(ao_info->rnext_key);
		key.rcv_next = rnext_key->rcvid;
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

	tcp_v4_send_ack(sk, skb,
			tcptw->tw_snd_nxt, tcptw->tw_rcv_nxt,
			tcptw->tw_rcv_wnd >> tw->tw_rcv_wscale,
			tcp_tw_tsval(tcptw),
			tcptw->tw_ts_recent,
			tw->tw_bound_dev_if, &key,
			tw->tw_transparent ? IP_REPLY_ARG_NOSRCCHECK : 0,
			tw->tw_tos,
			tw->tw_txhash);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	inet_twsk_put(tw);
}

<<<<<<< HEAD
static void tcp_v4_reqsk_send_ack(struct sock *sk, struct sk_buff *skb,
				  struct request_sock *req)
{
	tcp_v4_send_ack(sk, skb, tcp_rsk(req)->snt_isn + 1,
			tcp_rsk(req)->rcv_isn + 1, req->rcv_wnd,
			req->ts_recent,
			0,
			tcp_md5_do_lookup(sk, (union tcp_md5_addr *)&ip_hdr(skb)->daddr,
					  AF_INET),
			inet_rsk(req)->no_srccheck ? IP_REPLY_ARG_NOSRCCHECK : 0,
			ip_hdr(skb)->tos);
=======
static void tcp_v4_reqsk_send_ack(const struct sock *sk, struct sk_buff *skb,
				  struct request_sock *req)
{
	struct tcp_key key = {};

	/* sk->sk_state == TCP_LISTEN -> for regular TCP_SYN_RECV
	 * sk->sk_state == TCP_SYN_RECV -> for Fast Open.
	 */
	u32 seq = (sk->sk_state == TCP_LISTEN) ? tcp_rsk(req)->snt_isn + 1 :
					     tcp_sk(sk)->snd_nxt;

#ifdef CONFIG_TCP_AO
	if (static_branch_unlikely(&tcp_ao_needed.key) &&
	    tcp_rsk_used_ao(req)) {
		const union tcp_md5_addr *addr;
		const struct tcp_ao_hdr *aoh;
		int l3index;

		/* Invalid TCP option size or twice included auth */
		if (tcp_parse_auth_options(tcp_hdr(skb), NULL, &aoh))
			return;
		if (!aoh)
			return;

		addr = (union tcp_md5_addr *)&ip_hdr(skb)->saddr;
		l3index = tcp_v4_sdif(skb) ? inet_iif(skb) : 0;
		key.ao_key = tcp_ao_do_lookup(sk, l3index, addr, AF_INET,
					      aoh->rnext_keyid, -1);
		if (unlikely(!key.ao_key)) {
			/* Send ACK with any matching MKT for the peer */
			key.ao_key = tcp_ao_do_lookup(sk, l3index, addr, AF_INET, -1, -1);
			/* Matching key disappeared (user removed the key?)
			 * let the handshake timeout.
			 */
			if (!key.ao_key) {
				net_info_ratelimited("TCP-AO key for (%pI4, %d)->(%pI4, %d) suddenly disappeared, won't ACK new connection\n",
						     addr,
						     ntohs(tcp_hdr(skb)->source),
						     &ip_hdr(skb)->daddr,
						     ntohs(tcp_hdr(skb)->dest));
				return;
			}
		}
		key.traffic_key = kmalloc(tcp_ao_digest_size(key.ao_key), GFP_ATOMIC);
		if (!key.traffic_key)
			return;

		key.type = TCP_KEY_AO;
		key.rcv_next = aoh->keyid;
		tcp_v4_ao_calc_key_rsk(key.ao_key, key.traffic_key, req);
#else
	if (0) {
#endif
#ifdef CONFIG_TCP_MD5SIG
	} else if (static_branch_unlikely(&tcp_md5_needed.key)) {
		const union tcp_md5_addr *addr;
		int l3index;

		addr = (union tcp_md5_addr *)&ip_hdr(skb)->saddr;
		l3index = tcp_v4_sdif(skb) ? inet_iif(skb) : 0;
		key.md5_key = tcp_md5_do_lookup(sk, l3index, addr, AF_INET);
		if (key.md5_key)
			key.type = TCP_KEY_MD5;
#endif
	}

	/* RFC 7323 2.3
	 * The window field (SEG.WND) of every outgoing segment, with the
	 * exception of <SYN> segments, MUST be right-shifted by
	 * Rcv.Wind.Shift bits:
	 */
	tcp_v4_send_ack(sk, skb, seq,
			tcp_rsk(req)->rcv_nxt,
			req->rsk_rcv_wnd >> inet_rsk(req)->rcv_wscale,
			tcp_rsk_tsval(tcp_rsk(req)),
			READ_ONCE(req->ts_recent),
			0, &key,
			inet_rsk(req)->no_srccheck ? IP_REPLY_ARG_NOSRCCHECK : 0,
			ip_hdr(skb)->tos,
			READ_ONCE(tcp_rsk(req)->txhash));
	if (tcp_key_is_ao(&key))
		kfree(key.traffic_key);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 *	Send a SYN-ACK after having received a SYN.
 *	This still operates on a request_sock only, not on a big
 *	socket.
 */
<<<<<<< HEAD
static int tcp_v4_send_synack(struct sock *sk, struct dst_entry *dst,
			      struct request_sock *req,
			      struct request_values *rvp)
=======
static int tcp_v4_send_synack(const struct sock *sk, struct dst_entry *dst,
			      struct flowi *fl,
			      struct request_sock *req,
			      struct tcp_fastopen_cookie *foc,
			      enum tcp_synack_type synack_type,
			      struct sk_buff *syn_skb)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	const struct inet_request_sock *ireq = inet_rsk(req);
	struct flowi4 fl4;
	int err = -1;
<<<<<<< HEAD
	struct sk_buff * skb;
=======
	struct sk_buff *skb;
	u8 tos;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* First, grab a route. */
	if (!dst && (dst = inet_csk_route_req(sk, &fl4, req)) == NULL)
		return -1;

<<<<<<< HEAD
	skb = tcp_make_synack(sk, dst, req, rvp);

	if (skb) {
		__tcp_v4_send_check(skb, ireq->loc_addr, ireq->rmt_addr);

		err = ip_build_and_send_pkt(skb, sk, ireq->loc_addr,
					    ireq->rmt_addr,
					    ireq->opt);
		err = net_xmit_eval(err);
	}

	dst_release(dst);
	return err;
}

static int tcp_v4_rtx_synack(struct sock *sk, struct request_sock *req,
			      struct request_values *rvp)
{
	TCP_INC_STATS_BH(sock_net(sk), TCP_MIB_RETRANSSEGS);
	return tcp_v4_send_synack(sk, NULL, req, rvp);
}

=======
	skb = tcp_make_synack(sk, dst, req, foc, synack_type, syn_skb);

	if (skb) {
		__tcp_v4_send_check(skb, ireq->ir_loc_addr, ireq->ir_rmt_addr);

		tos = READ_ONCE(inet_sk(sk)->tos);

		if (READ_ONCE(sock_net(sk)->ipv4.sysctl_tcp_reflect_tos))
			tos = (tcp_rsk(req)->syn_tos & ~INET_ECN_MASK) |
			      (tos & INET_ECN_MASK);

		if (!INET_ECN_is_capable(tos) &&
		    tcp_bpf_ca_needs_ecn((struct sock *)req))
			tos |= INET_ECN_ECT_0;

		rcu_read_lock();
		err = ip_build_and_send_pkt(skb, sk, ireq->ir_loc_addr,
					    ireq->ir_rmt_addr,
					    rcu_dereference(ireq->ireq_opt),
					    tos);
		rcu_read_unlock();
		err = net_xmit_eval(err);
	}

	return err;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *	IPv4 request_sock destructor.
 */
static void tcp_v4_reqsk_destructor(struct request_sock *req)
{
<<<<<<< HEAD
	kfree(inet_rsk(req)->opt);
}

/*
 * Return 1 if a syncookie should be sent
 */
int tcp_syn_flood_action(struct sock *sk,
			 const struct sk_buff *skb,
			 const char *proto)
{
	const char *msg = "Dropping request";
	int want_cookie = 0;
	struct listen_sock *lopt;



#ifdef CONFIG_SYN_COOKIES
	if (sysctl_tcp_syncookies) {
		msg = "Sending cookies";
		want_cookie = 1;
		NET_INC_STATS_BH(sock_net(sk), LINUX_MIB_TCPREQQFULLDOCOOKIES);
	} else
#endif
		NET_INC_STATS_BH(sock_net(sk), LINUX_MIB_TCPREQQFULLDROP);

	lopt = inet_csk(sk)->icsk_accept_queue.listen_opt;
	if (!lopt->synflood_warned) {
		lopt->synflood_warned = 1;
		pr_info("%s: Possible SYN flooding on port %d. %s.  Check SNMP counters.\n",
			proto, ntohs(tcp_hdr(skb)->dest), msg);
	}
	return want_cookie;
}
EXPORT_SYMBOL(tcp_syn_flood_action);

/*
 * Save and compile IPv4 options into the request_sock if needed.
 */
static struct ip_options_rcu *tcp_v4_save_options(struct sock *sk,
						  struct sk_buff *skb)
{
	const struct ip_options *opt = &(IPCB(skb)->opt);
	struct ip_options_rcu *dopt = NULL;

	if (opt && opt->optlen) {
		int opt_size = sizeof(*dopt) + opt->optlen;

		dopt = kmalloc(opt_size, GFP_ATOMIC);
		if (dopt) {
			if (ip_options_echo(&dopt->opt, skb)) {
				kfree(dopt);
				dopt = NULL;
			}
		}
	}
	return dopt;
=======
	kfree(rcu_dereference_protected(inet_rsk(req)->ireq_opt, 1));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#ifdef CONFIG_TCP_MD5SIG
/*
 * RFC2385 MD5 checksumming requires a mapping of
 * IP address->MD5 Key.
 * We need to maintain these in the sk structure.
 */

<<<<<<< HEAD
/* Find the Key structure for an address.  */
struct tcp_md5sig_key *tcp_md5_do_lookup(struct sock *sk,
					 const union tcp_md5_addr *addr,
					 int family)
{
	struct tcp_sock *tp = tcp_sk(sk);
	struct tcp_md5sig_key *key;
	struct hlist_node *pos;
	unsigned int size = sizeof(struct in_addr);
	struct tcp_md5sig_info *md5sig;

	/* caller either holds rcu_read_lock() or socket lock */
	md5sig = rcu_dereference_check(tp->md5sig_info,
				       sock_owned_by_user(sk) ||
				       lockdep_is_held(&sk->sk_lock.slock));
=======
DEFINE_STATIC_KEY_DEFERRED_FALSE(tcp_md5_needed, HZ);
EXPORT_SYMBOL(tcp_md5_needed);

static bool better_md5_match(struct tcp_md5sig_key *old, struct tcp_md5sig_key *new)
{
	if (!old)
		return true;

	/* l3index always overrides non-l3index */
	if (old->l3index && new->l3index == 0)
		return false;
	if (old->l3index == 0 && new->l3index)
		return true;

	return old->prefixlen < new->prefixlen;
}

/* Find the Key structure for an address.  */
struct tcp_md5sig_key *__tcp_md5_do_lookup(const struct sock *sk, int l3index,
					   const union tcp_md5_addr *addr,
					   int family, bool any_l3index)
{
	const struct tcp_sock *tp = tcp_sk(sk);
	struct tcp_md5sig_key *key;
	const struct tcp_md5sig_info *md5sig;
	__be32 mask;
	struct tcp_md5sig_key *best_match = NULL;
	bool match;

	/* caller either holds rcu_read_lock() or socket lock */
	md5sig = rcu_dereference_check(tp->md5sig_info,
				       lockdep_sock_is_held(sk));
	if (!md5sig)
		return NULL;

	hlist_for_each_entry_rcu(key, &md5sig->head, node,
				 lockdep_sock_is_held(sk)) {
		if (key->family != family)
			continue;
		if (!any_l3index && key->flags & TCP_MD5SIG_FLAG_IFINDEX &&
		    key->l3index != l3index)
			continue;
		if (family == AF_INET) {
			mask = inet_make_mask(key->prefixlen);
			match = (key->addr.a4.s_addr & mask) ==
				(addr->a4.s_addr & mask);
#if IS_ENABLED(CONFIG_IPV6)
		} else if (family == AF_INET6) {
			match = ipv6_prefix_equal(&key->addr.a6, &addr->a6,
						  key->prefixlen);
#endif
		} else {
			match = false;
		}

		if (match && better_md5_match(best_match, key))
			best_match = key;
	}
	return best_match;
}
EXPORT_SYMBOL(__tcp_md5_do_lookup);

static struct tcp_md5sig_key *tcp_md5_do_lookup_exact(const struct sock *sk,
						      const union tcp_md5_addr *addr,
						      int family, u8 prefixlen,
						      int l3index, u8 flags)
{
	const struct tcp_sock *tp = tcp_sk(sk);
	struct tcp_md5sig_key *key;
	unsigned int size = sizeof(struct in_addr);
	const struct tcp_md5sig_info *md5sig;

	/* caller either holds rcu_read_lock() or socket lock */
	md5sig = rcu_dereference_check(tp->md5sig_info,
				       lockdep_sock_is_held(sk));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!md5sig)
		return NULL;
#if IS_ENABLED(CONFIG_IPV6)
	if (family == AF_INET6)
		size = sizeof(struct in6_addr);
#endif
<<<<<<< HEAD
	hlist_for_each_entry_rcu(key, pos, &md5sig->head, node) {
		if (key->family != family)
			continue;
		if (!memcmp(&key->addr, addr, size))
=======
	hlist_for_each_entry_rcu(key, &md5sig->head, node,
				 lockdep_sock_is_held(sk)) {
		if (key->family != family)
			continue;
		if ((key->flags & TCP_MD5SIG_FLAG_IFINDEX) != (flags & TCP_MD5SIG_FLAG_IFINDEX))
			continue;
		if (key->l3index != l3index)
			continue;
		if (!memcmp(&key->addr, addr, size) &&
		    key->prefixlen == prefixlen)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return key;
	}
	return NULL;
}
<<<<<<< HEAD
EXPORT_SYMBOL(tcp_md5_do_lookup);

struct tcp_md5sig_key *tcp_v4_md5_lookup(struct sock *sk,
					 struct sock *addr_sk)
{
	union tcp_md5_addr *addr;

	addr = (union tcp_md5_addr *)&inet_sk(addr_sk)->inet_daddr;
	return tcp_md5_do_lookup(sk, addr, AF_INET);
}
EXPORT_SYMBOL(tcp_v4_md5_lookup);

static struct tcp_md5sig_key *tcp_v4_reqsk_md5_lookup(struct sock *sk,
						      struct request_sock *req)
{
	union tcp_md5_addr *addr;

	addr = (union tcp_md5_addr *)&inet_rsk(req)->rmt_addr;
	return tcp_md5_do_lookup(sk, addr, AF_INET);
}

/* This can be called on a newly created socket, from other files */
int tcp_md5_do_add(struct sock *sk, const union tcp_md5_addr *addr,
		   int family, const u8 *newkey, u8 newkeylen, gfp_t gfp)
=======

struct tcp_md5sig_key *tcp_v4_md5_lookup(const struct sock *sk,
					 const struct sock *addr_sk)
{
	const union tcp_md5_addr *addr;
	int l3index;

	l3index = l3mdev_master_ifindex_by_index(sock_net(sk),
						 addr_sk->sk_bound_dev_if);
	addr = (const union tcp_md5_addr *)&addr_sk->sk_daddr;
	return tcp_md5_do_lookup(sk, l3index, addr, AF_INET);
}
EXPORT_SYMBOL(tcp_v4_md5_lookup);

static int tcp_md5sig_info_add(struct sock *sk, gfp_t gfp)
{
	struct tcp_sock *tp = tcp_sk(sk);
	struct tcp_md5sig_info *md5sig;

	md5sig = kmalloc(sizeof(*md5sig), gfp);
	if (!md5sig)
		return -ENOMEM;

	sk_gso_disable(sk);
	INIT_HLIST_HEAD(&md5sig->head);
	rcu_assign_pointer(tp->md5sig_info, md5sig);
	return 0;
}

/* This can be called on a newly created socket, from other files */
static int __tcp_md5_do_add(struct sock *sk, const union tcp_md5_addr *addr,
			    int family, u8 prefixlen, int l3index, u8 flags,
			    const u8 *newkey, u8 newkeylen, gfp_t gfp)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	/* Add Key to the list */
	struct tcp_md5sig_key *key;
	struct tcp_sock *tp = tcp_sk(sk);
	struct tcp_md5sig_info *md5sig;

<<<<<<< HEAD
	key = tcp_md5_do_lookup(sk, addr, family);
	if (key) {
		/* Pre-existing entry - just update that one. */
		memcpy(key->key, newkey, newkeylen);
		key->keylen = newkeylen;
=======
	key = tcp_md5_do_lookup_exact(sk, addr, family, prefixlen, l3index, flags);
	if (key) {
		/* Pre-existing entry - just update that one.
		 * Note that the key might be used concurrently.
		 * data_race() is telling kcsan that we do not care of
		 * key mismatches, since changing MD5 key on live flows
		 * can lead to packet drops.
		 */
		data_race(memcpy(key->key, newkey, newkeylen));

		/* Pairs with READ_ONCE() in tcp_md5_hash_key().
		 * Also note that a reader could catch new key->keylen value
		 * but old key->key[], this is the reason we use __GFP_ZERO
		 * at sock_kmalloc() time below these lines.
		 */
		WRITE_ONCE(key->keylen, newkeylen);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;
	}

	md5sig = rcu_dereference_protected(tp->md5sig_info,
<<<<<<< HEAD
					   sock_owned_by_user(sk) ||
					   lockdep_is_held(&sk->sk_lock.slock));
	if (!md5sig) {
		md5sig = kmalloc(sizeof(*md5sig), gfp);
		if (!md5sig)
			return -ENOMEM;

		sk_nocaps_add(sk, NETIF_F_GSO_MASK);
		INIT_HLIST_HEAD(&md5sig->head);
		rcu_assign_pointer(tp->md5sig_info, md5sig);
	}

	key = sock_kmalloc(sk, sizeof(*key), gfp);
	if (!key)
		return -ENOMEM;
	if (hlist_empty(&md5sig->head) && !tcp_alloc_md5sig_pool(sk)) {
		sock_kfree_s(sk, key, sizeof(*key));
		return -ENOMEM;
	}
=======
					   lockdep_sock_is_held(sk));

	key = sock_kmalloc(sk, sizeof(*key), gfp | __GFP_ZERO);
	if (!key)
		return -ENOMEM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	memcpy(key->key, newkey, newkeylen);
	key->keylen = newkeylen;
	key->family = family;
<<<<<<< HEAD
	memcpy(&key->addr, addr,
	       (family == AF_INET6) ? sizeof(struct in6_addr) :
				      sizeof(struct in_addr));
	hlist_add_head_rcu(&key->node, &md5sig->head);
	return 0;
}
EXPORT_SYMBOL(tcp_md5_do_add);

int tcp_md5_do_del(struct sock *sk, const union tcp_md5_addr *addr, int family)
{
	struct tcp_sock *tp = tcp_sk(sk);
	struct tcp_md5sig_key *key;
	struct tcp_md5sig_info *md5sig;

	key = tcp_md5_do_lookup(sk, addr, family);
=======
	key->prefixlen = prefixlen;
	key->l3index = l3index;
	key->flags = flags;
	memcpy(&key->addr, addr,
	       (IS_ENABLED(CONFIG_IPV6) && family == AF_INET6) ? sizeof(struct in6_addr) :
								 sizeof(struct in_addr));
	hlist_add_head_rcu(&key->node, &md5sig->head);
	return 0;
}

int tcp_md5_do_add(struct sock *sk, const union tcp_md5_addr *addr,
		   int family, u8 prefixlen, int l3index, u8 flags,
		   const u8 *newkey, u8 newkeylen)
{
	struct tcp_sock *tp = tcp_sk(sk);

	if (!rcu_dereference_protected(tp->md5sig_info, lockdep_sock_is_held(sk))) {
		if (tcp_md5_alloc_sigpool())
			return -ENOMEM;

		if (tcp_md5sig_info_add(sk, GFP_KERNEL)) {
			tcp_md5_release_sigpool();
			return -ENOMEM;
		}

		if (!static_branch_inc(&tcp_md5_needed.key)) {
			struct tcp_md5sig_info *md5sig;

			md5sig = rcu_dereference_protected(tp->md5sig_info, lockdep_sock_is_held(sk));
			rcu_assign_pointer(tp->md5sig_info, NULL);
			kfree_rcu(md5sig, rcu);
			tcp_md5_release_sigpool();
			return -EUSERS;
		}
	}

	return __tcp_md5_do_add(sk, addr, family, prefixlen, l3index, flags,
				newkey, newkeylen, GFP_KERNEL);
}
EXPORT_SYMBOL(tcp_md5_do_add);

int tcp_md5_key_copy(struct sock *sk, const union tcp_md5_addr *addr,
		     int family, u8 prefixlen, int l3index,
		     struct tcp_md5sig_key *key)
{
	struct tcp_sock *tp = tcp_sk(sk);

	if (!rcu_dereference_protected(tp->md5sig_info, lockdep_sock_is_held(sk))) {
		tcp_md5_add_sigpool();

		if (tcp_md5sig_info_add(sk, sk_gfp_mask(sk, GFP_ATOMIC))) {
			tcp_md5_release_sigpool();
			return -ENOMEM;
		}

		if (!static_key_fast_inc_not_disabled(&tcp_md5_needed.key.key)) {
			struct tcp_md5sig_info *md5sig;

			md5sig = rcu_dereference_protected(tp->md5sig_info, lockdep_sock_is_held(sk));
			net_warn_ratelimited("Too many TCP-MD5 keys in the system\n");
			rcu_assign_pointer(tp->md5sig_info, NULL);
			kfree_rcu(md5sig, rcu);
			tcp_md5_release_sigpool();
			return -EUSERS;
		}
	}

	return __tcp_md5_do_add(sk, addr, family, prefixlen, l3index,
				key->flags, key->key, key->keylen,
				sk_gfp_mask(sk, GFP_ATOMIC));
}
EXPORT_SYMBOL(tcp_md5_key_copy);

int tcp_md5_do_del(struct sock *sk, const union tcp_md5_addr *addr, int family,
		   u8 prefixlen, int l3index, u8 flags)
{
	struct tcp_md5sig_key *key;

	key = tcp_md5_do_lookup_exact(sk, addr, family, prefixlen, l3index, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!key)
		return -ENOENT;
	hlist_del_rcu(&key->node);
	atomic_sub(sizeof(*key), &sk->sk_omem_alloc);
	kfree_rcu(key, rcu);
<<<<<<< HEAD
	md5sig = rcu_dereference_protected(tp->md5sig_info,
					   sock_owned_by_user(sk));
	if (hlist_empty(&md5sig->head))
		tcp_free_md5sig_pool();
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}
EXPORT_SYMBOL(tcp_md5_do_del);

void tcp_clear_md5_list(struct sock *sk)
{
	struct tcp_sock *tp = tcp_sk(sk);
	struct tcp_md5sig_key *key;
<<<<<<< HEAD
	struct hlist_node *pos, *n;
=======
	struct hlist_node *n;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct tcp_md5sig_info *md5sig;

	md5sig = rcu_dereference_protected(tp->md5sig_info, 1);

<<<<<<< HEAD
	if (!hlist_empty(&md5sig->head))
		tcp_free_md5sig_pool();
	hlist_for_each_entry_safe(key, pos, n, &md5sig->head, node) {
=======
	hlist_for_each_entry_safe(key, n, &md5sig->head, node) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		hlist_del_rcu(&key->node);
		atomic_sub(sizeof(*key), &sk->sk_omem_alloc);
		kfree_rcu(key, rcu);
	}
}

<<<<<<< HEAD
static int tcp_v4_parse_md5_keys(struct sock *sk, char __user *optval,
				 int optlen)
{
	struct tcp_md5sig cmd;
	struct sockaddr_in *sin = (struct sockaddr_in *)&cmd.tcpm_addr;
=======
static int tcp_v4_parse_md5_keys(struct sock *sk, int optname,
				 sockptr_t optval, int optlen)
{
	struct tcp_md5sig cmd;
	struct sockaddr_in *sin = (struct sockaddr_in *)&cmd.tcpm_addr;
	const union tcp_md5_addr *addr;
	u8 prefixlen = 32;
	int l3index = 0;
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

	if (sin->sin_family != AF_INET)
		return -EINVAL;

<<<<<<< HEAD
	if (!cmd.tcpm_key || !cmd.tcpm_keylen)
		return tcp_md5_do_del(sk, (union tcp_md5_addr *)&sin->sin_addr.s_addr,
				      AF_INET);
=======
	flags = cmd.tcpm_flags & TCP_MD5SIG_FLAG_IFINDEX;
	l3flag = cmd.tcpm_flags & TCP_MD5SIG_FLAG_IFINDEX;

	if (optname == TCP_MD5SIG_EXT &&
	    cmd.tcpm_flags & TCP_MD5SIG_FLAG_PREFIX) {
		prefixlen = cmd.tcpm_prefixlen;
		if (prefixlen > 32)
			return -EINVAL;
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

	addr = (union tcp_md5_addr *)&sin->sin_addr.s_addr;

	if (!cmd.tcpm_keylen)
		return tcp_md5_do_del(sk, addr, AF_INET, prefixlen, l3index, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (cmd.tcpm_keylen > TCP_MD5SIG_MAXKEYLEN)
		return -EINVAL;

<<<<<<< HEAD
	return tcp_md5_do_add(sk, (union tcp_md5_addr *)&sin->sin_addr.s_addr,
			      AF_INET, cmd.tcpm_key, cmd.tcpm_keylen,
			      GFP_KERNEL);
}

static int tcp_v4_md5_hash_pseudoheader(struct tcp_md5sig_pool *hp,
					__be32 daddr, __be32 saddr, int nbytes)
{
	struct tcp4_pseudohdr *bp;
	struct scatterlist sg;

	bp = &hp->md5_blk.ip4;

	/*
	 * 1. the TCP pseudo-header (in the order: source IP address,
	 * destination IP address, zero-padded protocol number, and
	 * segment length)
	 */
=======
	/* Don't allow keys for peers that have a matching TCP-AO key.
	 * See the comment in tcp_ao_add_cmd()
	 */
	if (tcp_ao_required(sk, addr, AF_INET, l3flag ? l3index : -1, false))
		return -EKEYREJECTED;

	return tcp_md5_do_add(sk, addr, AF_INET, prefixlen, l3index, flags,
			      cmd.tcpm_key, cmd.tcpm_keylen);
}

static int tcp_v4_md5_hash_headers(struct tcp_sigpool *hp,
				   __be32 daddr, __be32 saddr,
				   const struct tcphdr *th, int nbytes)
{
	struct tcp4_pseudohdr *bp;
	struct scatterlist sg;
	struct tcphdr *_th;

	bp = hp->scratch;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	bp->saddr = saddr;
	bp->daddr = daddr;
	bp->pad = 0;
	bp->protocol = IPPROTO_TCP;
	bp->len = cpu_to_be16(nbytes);

<<<<<<< HEAD
	sg_init_one(&sg, bp, sizeof(*bp));
	return crypto_hash_update(&hp->md5_desc, &sg, sizeof(*bp));
=======
	_th = (struct tcphdr *)(bp + 1);
	memcpy(_th, th, sizeof(*th));
	_th->check = 0;

	sg_init_one(&sg, bp, sizeof(*bp) + sizeof(*th));
	ahash_request_set_crypt(hp->req, &sg, NULL,
				sizeof(*bp) + sizeof(*th));
	return crypto_ahash_update(hp->req);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int tcp_v4_md5_hash_hdr(char *md5_hash, const struct tcp_md5sig_key *key,
			       __be32 daddr, __be32 saddr, const struct tcphdr *th)
{
<<<<<<< HEAD
	struct tcp_md5sig_pool *hp;
	struct hash_desc *desc;

	hp = tcp_get_md5sig_pool();
	if (!hp)
		goto clear_hash_noput;
	desc = &hp->md5_desc;

	if (crypto_hash_init(desc))
		goto clear_hash;
	if (tcp_v4_md5_hash_pseudoheader(hp, daddr, saddr, th->doff << 2))
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
	struct tcp_sigpool hp;

	if (tcp_sigpool_start(tcp_md5_sigpool_id, &hp))
		goto clear_hash_nostart;

	if (crypto_ahash_init(hp.req))
		goto clear_hash;
	if (tcp_v4_md5_hash_headers(&hp, daddr, saddr, th, th->doff << 2))
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
int tcp_v4_md5_hash_skb(char *md5_hash, struct tcp_md5sig_key *key,
			const struct sock *sk, const struct request_sock *req,
			const struct sk_buff *skb)
{
	struct tcp_md5sig_pool *hp;
	struct hash_desc *desc;
	const struct tcphdr *th = tcp_hdr(skb);
	__be32 saddr, daddr;

	if (sk) {
		saddr = inet_sk(sk)->inet_saddr;
		daddr = inet_sk(sk)->inet_daddr;
	} else if (req) {
		saddr = inet_rsk(req)->loc_addr;
		daddr = inet_rsk(req)->rmt_addr;
=======
int tcp_v4_md5_hash_skb(char *md5_hash, const struct tcp_md5sig_key *key,
			const struct sock *sk,
			const struct sk_buff *skb)
{
	const struct tcphdr *th = tcp_hdr(skb);
	struct tcp_sigpool hp;
	__be32 saddr, daddr;

	if (sk) { /* valid for establish/request sockets */
		saddr = sk->sk_rcv_saddr;
		daddr = sk->sk_daddr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		const struct iphdr *iph = ip_hdr(skb);
		saddr = iph->saddr;
		daddr = iph->daddr;
	}

<<<<<<< HEAD
	hp = tcp_get_md5sig_pool();
	if (!hp)
		goto clear_hash_noput;
	desc = &hp->md5_desc;

	if (crypto_hash_init(desc))
		goto clear_hash;

	if (tcp_v4_md5_hash_pseudoheader(hp, daddr, saddr, skb->len))
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
=======
	if (tcp_sigpool_start(tcp_md5_sigpool_id, &hp))
		goto clear_hash_nostart;

	if (crypto_ahash_init(hp.req))
		goto clear_hash;

	if (tcp_v4_md5_hash_headers(&hp, daddr, saddr, th, skb->len))
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
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	memset(md5_hash, 0, 16);
	return 1;
}
EXPORT_SYMBOL(tcp_v4_md5_hash_skb);

<<<<<<< HEAD
static int tcp_v4_inbound_md5_hash(struct sock *sk, const struct sk_buff *skb)
{
	/*
	 * This gets called for each TCP segment that arrives
	 * so we want to be efficient.
	 * We have 3 drop cases:
	 * o No MD5 hash and one expected.
	 * o MD5 hash and we're not expecting one.
	 * o MD5 hash and its wrong.
	 */
	const __u8 *hash_location = NULL;
	struct tcp_md5sig_key *hash_expected;
	const struct iphdr *iph = ip_hdr(skb);
	const struct tcphdr *th = tcp_hdr(skb);
	int genhash;
	unsigned char newhash[16];

	hash_expected = tcp_md5_do_lookup(sk, (union tcp_md5_addr *)&iph->saddr,
					  AF_INET);
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

	/* Okay, so this is hash_expected and hash_location -
	 * so we need to calculate the checksum.
	 */
	genhash = tcp_v4_md5_hash_skb(newhash,
				      hash_expected,
				      NULL, NULL, skb);

	if (genhash || memcmp(hash_location, newhash, 16) != 0) {
		if (net_ratelimit()) {
			pr_info("MD5 Hash failed for (%pI4, %d)->(%pI4, %d)%s\n",
				&iph->saddr, ntohs(th->source),
				&iph->daddr, ntohs(th->dest),
				genhash ? " tcp_v4_calc_md5_hash failed" : "");
		}
		return 1;
	}
	return 0;
}

#endif
=======
#endif

static void tcp_v4_init_req(struct request_sock *req,
			    const struct sock *sk_listener,
			    struct sk_buff *skb)
{
	struct inet_request_sock *ireq = inet_rsk(req);
	struct net *net = sock_net(sk_listener);

	sk_rcv_saddr_set(req_to_sk(req), ip_hdr(skb)->daddr);
	sk_daddr_set(req_to_sk(req), ip_hdr(skb)->saddr);
	RCU_INIT_POINTER(ireq->ireq_opt, tcp_v4_save_options(net, skb));
}

static struct dst_entry *tcp_v4_route_req(const struct sock *sk,
					  struct sk_buff *skb,
					  struct flowi *fl,
					  struct request_sock *req)
{
	tcp_v4_init_req(req, sk, skb);

	if (security_inet_conn_request(sk, skb, req))
		return NULL;

	return inet_csk_route_req(sk, &fl->u.ip4, req);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct request_sock_ops tcp_request_sock_ops __read_mostly = {
	.family		=	PF_INET,
	.obj_size	=	sizeof(struct tcp_request_sock),
<<<<<<< HEAD
	.rtx_syn_ack	=	tcp_v4_rtx_synack,
	.send_ack	=	tcp_v4_reqsk_send_ack,
	.destructor	=	tcp_v4_reqsk_destructor,
	.send_reset	=	tcp_v4_send_reset,
	.syn_ack_timeout = 	tcp_syn_ack_timeout,
};

#ifdef CONFIG_TCP_MD5SIG
static const struct tcp_request_sock_ops tcp_request_sock_ipv4_ops = {
	.md5_lookup	=	tcp_v4_reqsk_md5_lookup,
	.calc_md5_hash	=	tcp_v4_md5_hash_skb,
};
#endif

int tcp_v4_conn_request(struct sock *sk, struct sk_buff *skb)
{
	struct tcp_extend_values tmp_ext;
	struct tcp_options_received tmp_opt;
	const u8 *hash_location;
	struct request_sock *req;
	struct inet_request_sock *ireq;
	struct tcp_sock *tp = tcp_sk(sk);
	struct dst_entry *dst = NULL;
	__be32 saddr = ip_hdr(skb)->saddr;
	__be32 daddr = ip_hdr(skb)->daddr;
	__u32 isn = TCP_SKB_CB(skb)->when;
	int want_cookie = 0;

=======
	.rtx_syn_ack	=	tcp_rtx_synack,
	.send_ack	=	tcp_v4_reqsk_send_ack,
	.destructor	=	tcp_v4_reqsk_destructor,
	.send_reset	=	tcp_v4_send_reset,
	.syn_ack_timeout =	tcp_syn_ack_timeout,
};

const struct tcp_request_sock_ops tcp_request_sock_ipv4_ops = {
	.mss_clamp	=	TCP_MSS_DEFAULT,
#ifdef CONFIG_TCP_MD5SIG
	.req_md5_lookup	=	tcp_v4_md5_lookup,
	.calc_md5_hash	=	tcp_v4_md5_hash_skb,
#endif
#ifdef CONFIG_TCP_AO
	.ao_lookup	=	tcp_v4_ao_lookup_rsk,
	.ao_calc_key	=	tcp_v4_ao_calc_key_rsk,
	.ao_synack_hash	=	tcp_v4_ao_synack_hash,
#endif
#ifdef CONFIG_SYN_COOKIES
	.cookie_init_seq =	cookie_v4_init_sequence,
#endif
	.route_req	=	tcp_v4_route_req,
	.init_seq	=	tcp_v4_init_seq,
	.init_ts_off	=	tcp_v4_init_ts_off,
	.send_synack	=	tcp_v4_send_synack,
};

int tcp_v4_conn_request(struct sock *sk, struct sk_buff *skb)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Never answer to SYNs send to broadcast or multicast */
	if (skb_rtable(skb)->rt_flags & (RTCF_BROADCAST | RTCF_MULTICAST))
		goto drop;

<<<<<<< HEAD
	/* TW buckets are converted to open requests without
	 * limitations, they conserve resources and peer is
	 * evidently real one.
	 */
	if (inet_csk_reqsk_queue_is_full(sk) && !isn) {
		want_cookie = tcp_syn_flood_action(sk, skb, "TCP");
		if (!want_cookie)
			goto drop;
	}

	/* Accept backlog is full. If we have already queued enough
	 * of warm entries in syn queue, drop request. It is better than
	 * clogging syn queue with openreqs with exponentially increasing
	 * timeout.
	 */
	if (sk_acceptq_is_full(sk) && inet_csk_reqsk_queue_young(sk) > 1)
		goto drop;

	req = inet_reqsk_alloc(&tcp_request_sock_ops);
	if (!req)
		goto drop;

#ifdef CONFIG_TCP_MD5SIG
	tcp_rsk(req)->af_specific = &tcp_request_sock_ipv4_ops;
#endif

	tcp_clear_options(&tmp_opt);
	tmp_opt.mss_clamp = TCP_MSS_DEFAULT;
	tmp_opt.user_mss  = tp->rx_opt.user_mss;
	tcp_parse_options(skb, &tmp_opt, &hash_location, 0);

	if (tmp_opt.cookie_plus > 0 &&
	    tmp_opt.saw_tstamp &&
	    !tp->rx_opt.cookie_out_never &&
	    (sysctl_tcp_cookie_size > 0 ||
	     (tp->cookie_values != NULL &&
	      tp->cookie_values->cookie_desired > 0))) {
		u8 *c;
		u32 *mess = &tmp_ext.cookie_bakery[COOKIE_DIGEST_WORDS];
		int l = tmp_opt.cookie_plus - TCPOLEN_COOKIE_BASE;

		if (tcp_cookie_generator(&tmp_ext.cookie_bakery[0]) != 0)
			goto drop_and_release;

		/* Secret recipe starts with IP addresses */
		*mess++ ^= (__force u32)daddr;
		*mess++ ^= (__force u32)saddr;

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
		goto drop_and_release;
	}
	tmp_ext.cookie_in_always = tp->rx_opt.cookie_in_always;

	if (want_cookie && !tmp_opt.saw_tstamp)
		tcp_clear_options(&tmp_opt);

	tmp_opt.tstamp_ok = tmp_opt.saw_tstamp;
	tcp_openreq_init(req, &tmp_opt, skb);

	ireq = inet_rsk(req);
	ireq->loc_addr = daddr;
	ireq->rmt_addr = saddr;
	ireq->no_srccheck = inet_sk(sk)->transparent;
	ireq->opt = tcp_v4_save_options(sk, skb);
	ireq->ir_mark = inet_request_mark(sk, skb);

	if (security_inet_conn_request(sk, skb, req))
		goto drop_and_free;

	if (!want_cookie || tmp_opt.tstamp_ok)
		TCP_ECN_create_request(req, tcp_hdr(skb));

	if (want_cookie) {
		isn = cookie_v4_init_sequence(sk, skb, &req->mss);
		req->cookie_ts = tmp_opt.tstamp_ok;
	} else if (!isn) {
		struct inet_peer *peer = NULL;
		struct flowi4 fl4;

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
		    (dst = inet_csk_route_req(sk, &fl4, req)) != NULL &&
		    fl4.daddr == saddr &&
		    (peer = rt_get_peer((struct rtable *)dst, fl4.daddr)) != NULL) {
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
			LIMIT_NETDEBUG(KERN_DEBUG pr_fmt("drop open request from %pI4/%u\n"),
				       &saddr, ntohs(tcp_hdr(skb)->source));
			goto drop_and_release;
		}

		isn = tcp_v4_init_sequence(skb);
	}
	tcp_rsk(req)->snt_isn = isn;
	tcp_rsk(req)->snt_synack = tcp_time_stamp;

	if (tcp_v4_send_synack(sk, dst, req,
			       (struct request_values *)&tmp_ext) ||
	    want_cookie)
		goto drop_and_free;

	inet_csk_reqsk_queue_hash_add(sk, req, TCP_TIMEOUT_INIT);
	return 0;

drop_and_release:
	dst_release(dst);
drop_and_free:
	reqsk_free(req);
drop:
=======
	return tcp_conn_request(&tcp_request_sock_ops,
				&tcp_request_sock_ipv4_ops, sk, skb);

drop:
	tcp_listendrop(sk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}
EXPORT_SYMBOL(tcp_v4_conn_request);


/*
 * The three way handshake has completed - we got a valid synack -
 * now create the new socket.
 */
<<<<<<< HEAD
struct sock *tcp_v4_syn_recv_sock(struct sock *sk, struct sk_buff *skb,
				  struct request_sock *req,
				  struct dst_entry *dst)
{
	struct inet_request_sock *ireq;
=======
struct sock *tcp_v4_syn_recv_sock(const struct sock *sk, struct sk_buff *skb,
				  struct request_sock *req,
				  struct dst_entry *dst,
				  struct request_sock *req_unhash,
				  bool *own_req)
{
	struct inet_request_sock *ireq;
	bool found_dup_sk = false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct inet_sock *newinet;
	struct tcp_sock *newtp;
	struct sock *newsk;
#ifdef CONFIG_TCP_MD5SIG
<<<<<<< HEAD
	struct tcp_md5sig_key *key;
=======
	const union tcp_md5_addr *addr;
	struct tcp_md5sig_key *key;
	int l3index;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
	struct ip_options_rcu *inet_opt;

	if (sk_acceptq_is_full(sk))
		goto exit_overflow;

	newsk = tcp_create_openreq_child(sk, req, skb);
	if (!newsk)
		goto exit_nonewsk;

	newsk->sk_gso_type = SKB_GSO_TCPV4;
<<<<<<< HEAD
=======
	inet_sk_rx_dst_set(newsk, skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	newtp		      = tcp_sk(newsk);
	newinet		      = inet_sk(newsk);
	ireq		      = inet_rsk(req);
<<<<<<< HEAD
	newinet->inet_daddr   = ireq->rmt_addr;
	newinet->inet_rcv_saddr = ireq->loc_addr;
	newinet->inet_saddr	      = ireq->loc_addr;
	inet_opt	      = ireq->opt;
	rcu_assign_pointer(newinet->inet_opt, inet_opt);
	ireq->opt	      = NULL;
=======
	sk_daddr_set(newsk, ireq->ir_rmt_addr);
	sk_rcv_saddr_set(newsk, ireq->ir_loc_addr);
	newsk->sk_bound_dev_if = ireq->ir_iif;
	newinet->inet_saddr   = ireq->ir_loc_addr;
	inet_opt	      = rcu_dereference(ireq->ireq_opt);
	RCU_INIT_POINTER(newinet->inet_opt, inet_opt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	newinet->mc_index     = inet_iif(skb);
	newinet->mc_ttl	      = ip_hdr(skb)->ttl;
	newinet->rcv_tos      = ip_hdr(skb)->tos;
	inet_csk(newsk)->icsk_ext_hdr_len = 0;
	if (inet_opt)
		inet_csk(newsk)->icsk_ext_hdr_len = inet_opt->opt.optlen;
<<<<<<< HEAD
	newinet->inet_id = newtp->write_seq ^ jiffies;
=======
	atomic_set(&newinet->inet_id, get_random_u16());

	/* Set ToS of the new socket based upon the value of incoming SYN.
	 * ECT bits are set later in tcp_init_transfer().
	 */
	if (READ_ONCE(sock_net(sk)->ipv4.sysctl_tcp_reflect_tos))
		newinet->tos = tcp_rsk(req)->syn_tos & ~INET_ECN_MASK;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!dst) {
		dst = inet_csk_route_child_sock(sk, newsk, req);
		if (!dst)
			goto put_and_exit;
	} else {
		/* syncookie case : see end of cookie_v4_check() */
	}
	sk_setup_caps(newsk, dst);

<<<<<<< HEAD
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

#ifdef CONFIG_TCP_MD5SIG
	/* Copy over the MD5 key from the original socket */
	key = tcp_md5_do_lookup(sk, (union tcp_md5_addr *)&newinet->inet_daddr,
				AF_INET);
	if (key != NULL) {
		/*
		 * We're using one, so create a matching key
		 * on the newsk structure. If we fail to get
		 * memory, then we end up not copying the key
		 * across. Shucks.
		 */
		tcp_md5_do_add(newsk, (union tcp_md5_addr *)&newinet->inet_daddr,
			       AF_INET, key->key, key->keylen, GFP_ATOMIC);
		sk_nocaps_add(newsk, NETIF_F_GSO_MASK);
	}
#endif

	if (__inet_inherit_port(sk, newsk) < 0)
		goto put_and_exit;
	__inet_hash_nolisten(newsk, NULL);

	return newsk;

exit_overflow:
	NET_INC_STATS_BH(sock_net(sk), LINUX_MIB_LISTENOVERFLOWS);
exit_nonewsk:
	dst_release(dst);
exit:
	NET_INC_STATS_BH(sock_net(sk), LINUX_MIB_LISTENDROPS);
	return NULL;
put_and_exit:
=======
	tcp_ca_openreq_child(newsk, dst);

	tcp_sync_mss(newsk, dst_mtu(dst));
	newtp->advmss = tcp_mss_clamp(tcp_sk(sk), dst_metric_advmss(dst));

	tcp_initialize_rcv_mss(newsk);

#ifdef CONFIG_TCP_MD5SIG
	l3index = l3mdev_master_ifindex_by_index(sock_net(sk), ireq->ir_iif);
	/* Copy over the MD5 key from the original socket */
	addr = (union tcp_md5_addr *)&newinet->inet_daddr;
	key = tcp_md5_do_lookup(sk, l3index, addr, AF_INET);
	if (key && !tcp_rsk_used_ao(req)) {
		if (tcp_md5_key_copy(newsk, addr, AF_INET, 32, l3index, key))
			goto put_and_exit;
		sk_gso_disable(newsk);
	}
#endif
#ifdef CONFIG_TCP_AO
	if (tcp_ao_copy_all_matching(sk, newsk, req, skb, AF_INET))
		goto put_and_exit; /* OOM, release back memory */
#endif

	if (__inet_inherit_port(sk, newsk) < 0)
		goto put_and_exit;
	*own_req = inet_ehash_nolisten(newsk, req_to_sk(req_unhash),
				       &found_dup_sk);
	if (likely(*own_req)) {
		tcp_move_syn(newtp, req);
		ireq->ireq_opt = NULL;
	} else {
		newinet->inet_opt = NULL;

		if (!req_unhash && found_dup_sk) {
			/* This code path should only be executed in the
			 * syncookie case only
			 */
			bh_unlock_sock(newsk);
			sock_put(newsk);
			newsk = NULL;
		}
	}
	return newsk;

exit_overflow:
	NET_INC_STATS(sock_net(sk), LINUX_MIB_LISTENOVERFLOWS);
exit_nonewsk:
	dst_release(dst);
exit:
	tcp_listendrop(sk);
	return NULL;
put_and_exit:
	newinet->inet_opt = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	inet_csk_prepare_forced_close(newsk);
	tcp_done(newsk);
	goto exit;
}
EXPORT_SYMBOL(tcp_v4_syn_recv_sock);

<<<<<<< HEAD
static struct sock *tcp_v4_hnd_req(struct sock *sk, struct sk_buff *skb)
{
	struct tcphdr *th = tcp_hdr(skb);
	const struct iphdr *iph = ip_hdr(skb);
	struct sock *nsk;
	struct request_sock **prev;
	/* Find possible connection requests. */
	struct request_sock *req = inet_csk_search_req(sk, &prev, th->source,
						       iph->saddr, iph->daddr);
	if (req)
		return tcp_check_req(sk, skb, req, prev);

	nsk = inet_lookup_established(sock_net(sk), &tcp_hashinfo, iph->saddr,
			th->source, iph->daddr, th->dest, inet_iif(skb));

	if (nsk) {
		if (nsk->sk_state != TCP_TIME_WAIT) {
			bh_lock_sock(nsk);
			return nsk;
		}
		inet_twsk_put(inet_twsk(nsk));
		return NULL;
	}

#ifdef CONFIG_SYN_COOKIES
	if (!th->syn)
		sk = cookie_v4_check(sk, skb, &(IPCB(skb)->opt));
=======
static struct sock *tcp_v4_cookie_check(struct sock *sk, struct sk_buff *skb)
{
#ifdef CONFIG_SYN_COOKIES
	const struct tcphdr *th = tcp_hdr(skb);

	if (!th->syn)
		sk = cookie_v4_check(sk, skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
	return sk;
}

<<<<<<< HEAD
static __sum16 tcp_v4_checksum_init(struct sk_buff *skb)
{
	const struct iphdr *iph = ip_hdr(skb);

	if (skb->ip_summed == CHECKSUM_COMPLETE) {
		if (!tcp_v4_check(skb->len, iph->saddr,
				  iph->daddr, skb->csum)) {
			skb->ip_summed = CHECKSUM_UNNECESSARY;
			return 0;
		}
	}

	skb->csum = csum_tcpudp_nofold(iph->saddr, iph->daddr,
				       skb->len, IPPROTO_TCP, 0);

	if (skb->len <= 76) {
		return __skb_checksum_complete(skb);
	}
	return 0;
}


/* The socket must have it's spinlock held when we get
 * here.
=======
u16 tcp_v4_get_syncookie(struct sock *sk, struct iphdr *iph,
			 struct tcphdr *th, u32 *cookie)
{
	u16 mss = 0;
#ifdef CONFIG_SYN_COOKIES
	mss = tcp_get_syncookie_mss(&tcp_request_sock_ops,
				    &tcp_request_sock_ipv4_ops, sk, th);
	if (mss) {
		*cookie = __cookie_v4_init_sequence(iph, th, &mss);
		tcp_synq_overflow(sk);
	}
#endif
	return mss;
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
int tcp_v4_do_rcv(struct sock *sk, struct sk_buff *skb)
{
<<<<<<< HEAD
	struct sock *rsk;
#ifdef CONFIG_TCP_MD5SIG
	/*
	 * We really want to reject the packet as early as possible
	 * if:
	 *  o We're expecting an MD5'd packet and this is no MD5 tcp option
	 *  o There is an MD5 option and we're not expecting one
	 */
	if (tcp_v4_inbound_md5_hash(sk, skb))
		goto discard;
#endif

	if (sk->sk_state == TCP_ESTABLISHED) { /* Fast path */
		sock_rps_save_rxhash(sk, skb);
		if (tcp_rcv_established(sk, skb, tcp_hdr(skb), skb->len)) {
			rsk = sk;
			goto reset;
		}
		return 0;
	}

	if (skb->len < tcp_hdrlen(skb) || tcp_checksum_complete(skb))
		goto csum_err;

	if (sk->sk_state == TCP_LISTEN) {
		struct sock *nsk = tcp_v4_hnd_req(sk, skb);
		if (!nsk)
			goto discard;

		if (nsk != sk) {
			sock_rps_save_rxhash(nsk, skb);
			if (tcp_child_process(sk, nsk, skb)) {
=======
	enum skb_drop_reason reason;
	struct sock *rsk;

	if (sk->sk_state == TCP_ESTABLISHED) { /* Fast path */
		struct dst_entry *dst;

		dst = rcu_dereference_protected(sk->sk_rx_dst,
						lockdep_sock_is_held(sk));

		sock_rps_save_rxhash(sk, skb);
		sk_mark_napi_id(sk, skb);
		if (dst) {
			if (sk->sk_rx_dst_ifindex != skb->skb_iif ||
			    !INDIRECT_CALL_1(dst->ops->check, ipv4_dst_check,
					     dst, 0)) {
				RCU_INIT_POINTER(sk->sk_rx_dst, NULL);
				dst_release(dst);
			}
		}
		tcp_rcv_established(sk, skb);
		return 0;
	}

	if (tcp_checksum_complete(skb))
		goto csum_err;

	if (sk->sk_state == TCP_LISTEN) {
		struct sock *nsk = tcp_v4_cookie_check(sk, skb);

		if (!nsk)
			return 0;
		if (nsk != sk) {
			reason = tcp_child_process(sk, nsk, skb);
			if (reason) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				rsk = nsk;
				goto reset;
			}
			return 0;
		}
	} else
		sock_rps_save_rxhash(sk, skb);

<<<<<<< HEAD
	if (tcp_rcv_state_process(sk, skb, tcp_hdr(skb), skb->len)) {
=======
	reason = tcp_rcv_state_process(sk, skb);
	if (reason) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		rsk = sk;
		goto reset;
	}
	return 0;

reset:
	tcp_v4_send_reset(rsk, skb);
discard:
<<<<<<< HEAD
	kfree_skb(skb);
=======
	kfree_skb_reason(skb, reason);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Be careful here. If this function gets more complicated and
	 * gcc suffers from register pressure on the x86, sk (in %ebx)
	 * might be destroyed here. This current version compiles correctly,
	 * but you have been warned.
	 */
	return 0;

csum_err:
<<<<<<< HEAD
	TCP_INC_STATS_BH(sock_net(sk), TCP_MIB_INERRS);
=======
	reason = SKB_DROP_REASON_TCP_CSUM;
	trace_tcp_bad_csum(skb);
	TCP_INC_STATS(sock_net(sk), TCP_MIB_CSUMERRORS);
	TCP_INC_STATS(sock_net(sk), TCP_MIB_INERRS);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	goto discard;
}
EXPORT_SYMBOL(tcp_v4_do_rcv);

<<<<<<< HEAD
=======
int tcp_v4_early_demux(struct sk_buff *skb)
{
	struct net *net = dev_net(skb->dev);
	const struct iphdr *iph;
	const struct tcphdr *th;
	struct sock *sk;

	if (skb->pkt_type != PACKET_HOST)
		return 0;

	if (!pskb_may_pull(skb, skb_transport_offset(skb) + sizeof(struct tcphdr)))
		return 0;

	iph = ip_hdr(skb);
	th = tcp_hdr(skb);

	if (th->doff < sizeof(struct tcphdr) / 4)
		return 0;

	sk = __inet_lookup_established(net, net->ipv4.tcp_death_row.hashinfo,
				       iph->saddr, th->source,
				       iph->daddr, ntohs(th->dest),
				       skb->skb_iif, inet_sdif(skb));
	if (sk) {
		skb->sk = sk;
		skb->destructor = sock_edemux;
		if (sk_fullsock(sk)) {
			struct dst_entry *dst = rcu_dereference(sk->sk_rx_dst);

			if (dst)
				dst = dst_check(dst, 0);
			if (dst &&
			    sk->sk_rx_dst_ifindex == skb->skb_iif)
				skb_dst_set_noref(skb, dst);
		}
	}
	return 0;
}

bool tcp_add_backlog(struct sock *sk, struct sk_buff *skb,
		     enum skb_drop_reason *reason)
{
	u32 limit, tail_gso_size, tail_gso_segs;
	struct skb_shared_info *shinfo;
	const struct tcphdr *th;
	struct tcphdr *thtail;
	struct sk_buff *tail;
	unsigned int hdrlen;
	bool fragstolen;
	u32 gso_segs;
	u32 gso_size;
	int delta;

	/* In case all data was pulled from skb frags (in __pskb_pull_tail()),
	 * we can fix skb->truesize to its real value to avoid future drops.
	 * This is valid because skb is not yet charged to the socket.
	 * It has been noticed pure SACK packets were sometimes dropped
	 * (if cooked by drivers without copybreak feature).
	 */
	skb_condense(skb);

	skb_dst_drop(skb);

	if (unlikely(tcp_checksum_complete(skb))) {
		bh_unlock_sock(sk);
		trace_tcp_bad_csum(skb);
		*reason = SKB_DROP_REASON_TCP_CSUM;
		__TCP_INC_STATS(sock_net(sk), TCP_MIB_CSUMERRORS);
		__TCP_INC_STATS(sock_net(sk), TCP_MIB_INERRS);
		return true;
	}

	/* Attempt coalescing to last skb in backlog, even if we are
	 * above the limits.
	 * This is okay because skb capacity is limited to MAX_SKB_FRAGS.
	 */
	th = (const struct tcphdr *)skb->data;
	hdrlen = th->doff * 4;

	tail = sk->sk_backlog.tail;
	if (!tail)
		goto no_coalesce;
	thtail = (struct tcphdr *)tail->data;

	if (TCP_SKB_CB(tail)->end_seq != TCP_SKB_CB(skb)->seq ||
	    TCP_SKB_CB(tail)->ip_dsfield != TCP_SKB_CB(skb)->ip_dsfield ||
	    ((TCP_SKB_CB(tail)->tcp_flags |
	      TCP_SKB_CB(skb)->tcp_flags) & (TCPHDR_SYN | TCPHDR_RST | TCPHDR_URG)) ||
	    !((TCP_SKB_CB(tail)->tcp_flags &
	      TCP_SKB_CB(skb)->tcp_flags) & TCPHDR_ACK) ||
	    ((TCP_SKB_CB(tail)->tcp_flags ^
	      TCP_SKB_CB(skb)->tcp_flags) & (TCPHDR_ECE | TCPHDR_CWR)) ||
#ifdef CONFIG_TLS_DEVICE
	    tail->decrypted != skb->decrypted ||
#endif
	    !mptcp_skb_can_collapse(tail, skb) ||
	    thtail->doff != th->doff ||
	    memcmp(thtail + 1, th + 1, hdrlen - sizeof(*th)))
		goto no_coalesce;

	__skb_pull(skb, hdrlen);

	shinfo = skb_shinfo(skb);
	gso_size = shinfo->gso_size ?: skb->len;
	gso_segs = shinfo->gso_segs ?: 1;

	shinfo = skb_shinfo(tail);
	tail_gso_size = shinfo->gso_size ?: (tail->len - hdrlen);
	tail_gso_segs = shinfo->gso_segs ?: 1;

	if (skb_try_coalesce(tail, skb, &fragstolen, &delta)) {
		TCP_SKB_CB(tail)->end_seq = TCP_SKB_CB(skb)->end_seq;

		if (likely(!before(TCP_SKB_CB(skb)->ack_seq, TCP_SKB_CB(tail)->ack_seq))) {
			TCP_SKB_CB(tail)->ack_seq = TCP_SKB_CB(skb)->ack_seq;
			thtail->window = th->window;
		}

		/* We have to update both TCP_SKB_CB(tail)->tcp_flags and
		 * thtail->fin, so that the fast path in tcp_rcv_established()
		 * is not entered if we append a packet with a FIN.
		 * SYN, RST, URG are not present.
		 * ACK is set on both packets.
		 * PSH : we do not really care in TCP stack,
		 *       at least for 'GRO' packets.
		 */
		thtail->fin |= th->fin;
		TCP_SKB_CB(tail)->tcp_flags |= TCP_SKB_CB(skb)->tcp_flags;

		if (TCP_SKB_CB(skb)->has_rxtstamp) {
			TCP_SKB_CB(tail)->has_rxtstamp = true;
			tail->tstamp = skb->tstamp;
			skb_hwtstamps(tail)->hwtstamp = skb_hwtstamps(skb)->hwtstamp;
		}

		/* Not as strict as GRO. We only need to carry mss max value */
		shinfo->gso_size = max(gso_size, tail_gso_size);
		shinfo->gso_segs = min_t(u32, gso_segs + tail_gso_segs, 0xFFFF);

		sk->sk_backlog.len += delta;
		__NET_INC_STATS(sock_net(sk),
				LINUX_MIB_TCPBACKLOGCOALESCE);
		kfree_skb_partial(skb, fragstolen);
		return false;
	}
	__skb_push(skb, hdrlen);

no_coalesce:
	limit = (u32)READ_ONCE(sk->sk_rcvbuf) + (u32)(READ_ONCE(sk->sk_sndbuf) >> 1);

	/* Only socket owner can try to collapse/prune rx queues
	 * to reduce memory overhead, so add a little headroom here.
	 * Few sockets backlog are possibly concurrently non empty.
	 */
	limit += 64 * 1024;

	if (unlikely(sk_add_backlog(sk, skb, limit))) {
		bh_unlock_sock(sk);
		*reason = SKB_DROP_REASON_SOCKET_BACKLOG;
		__NET_INC_STATS(sock_net(sk), LINUX_MIB_TCPBACKLOGDROP);
		return true;
	}
	return false;
}
EXPORT_SYMBOL(tcp_add_backlog);

int tcp_filter(struct sock *sk, struct sk_buff *skb)
{
	struct tcphdr *th = (struct tcphdr *)skb->data;

	return sk_filter_trim_cap(sk, skb, th->doff * 4);
}
EXPORT_SYMBOL(tcp_filter);

static void tcp_v4_restore_cb(struct sk_buff *skb)
{
	memmove(IPCB(skb), &TCP_SKB_CB(skb)->header.h4,
		sizeof(struct inet_skb_parm));
}

static void tcp_v4_fill_cb(struct sk_buff *skb, const struct iphdr *iph,
			   const struct tcphdr *th)
{
	/* This is tricky : We move IPCB at its correct location into TCP_SKB_CB()
	 * barrier() makes sure compiler wont play fool^Waliasing games.
	 */
	memmove(&TCP_SKB_CB(skb)->header.h4, IPCB(skb),
		sizeof(struct inet_skb_parm));
	barrier();

	TCP_SKB_CB(skb)->seq = ntohl(th->seq);
	TCP_SKB_CB(skb)->end_seq = (TCP_SKB_CB(skb)->seq + th->syn + th->fin +
				    skb->len - th->doff * 4);
	TCP_SKB_CB(skb)->ack_seq = ntohl(th->ack_seq);
	TCP_SKB_CB(skb)->tcp_flags = tcp_flag_byte(th);
	TCP_SKB_CB(skb)->tcp_tw_isn = 0;
	TCP_SKB_CB(skb)->ip_dsfield = ipv4_get_dsfield(iph);
	TCP_SKB_CB(skb)->sacked	 = 0;
	TCP_SKB_CB(skb)->has_rxtstamp =
			skb->tstamp || skb_hwtstamps(skb)->hwtstamp;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *	From tcp_input.c
 */

int tcp_v4_rcv(struct sk_buff *skb)
{
<<<<<<< HEAD
	const struct iphdr *iph;
	const struct tcphdr *th;
	struct sock *sk;
	int ret;
	struct net *net = dev_net(skb->dev);

=======
	struct net *net = dev_net(skb->dev);
	enum skb_drop_reason drop_reason;
	int sdif = inet_sdif(skb);
	int dif = inet_iif(skb);
	const struct iphdr *iph;
	const struct tcphdr *th;
	bool refcounted;
	struct sock *sk;
	int ret;

	drop_reason = SKB_DROP_REASON_NOT_SPECIFIED;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (skb->pkt_type != PACKET_HOST)
		goto discard_it;

	/* Count it even if it's bad */
<<<<<<< HEAD
	TCP_INC_STATS_BH(net, TCP_MIB_INSEGS);
=======
	__TCP_INC_STATS(net, TCP_MIB_INSEGS);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!pskb_may_pull(skb, sizeof(struct tcphdr)))
		goto discard_it;

<<<<<<< HEAD
	th = tcp_hdr(skb);

	if (th->doff < sizeof(struct tcphdr) / 4)
		goto bad_packet;
=======
	th = (const struct tcphdr *)skb->data;

	if (unlikely(th->doff < sizeof(struct tcphdr) / 4)) {
		drop_reason = SKB_DROP_REASON_PKT_TOO_SMALL;
		goto bad_packet;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!pskb_may_pull(skb, th->doff * 4))
		goto discard_it;

	/* An explanation is required here, I think.
	 * Packet length and doff are validated by header prediction,
	 * provided case of th->doff==0 is eliminated.
	 * So, we defer the checks. */
<<<<<<< HEAD
	if (!skb_csum_unnecessary(skb) && tcp_v4_checksum_init(skb))
		goto bad_packet;

	th = tcp_hdr(skb);
	iph = ip_hdr(skb);
	TCP_SKB_CB(skb)->seq = ntohl(th->seq);
	TCP_SKB_CB(skb)->end_seq = (TCP_SKB_CB(skb)->seq + th->syn + th->fin +
				    skb->len - th->doff * 4);
	TCP_SKB_CB(skb)->ack_seq = ntohl(th->ack_seq);
	TCP_SKB_CB(skb)->when	 = 0;
	TCP_SKB_CB(skb)->ip_dsfield = ipv4_get_dsfield(iph);
	TCP_SKB_CB(skb)->sacked	 = 0;

	sk = __inet_lookup_skb(&tcp_hashinfo, skb, th->source, th->dest);
=======

	if (skb_checksum_init(skb, IPPROTO_TCP, inet_compute_pseudo))
		goto csum_error;

	th = (const struct tcphdr *)skb->data;
	iph = ip_hdr(skb);
lookup:
	sk = __inet_lookup_skb(net->ipv4.tcp_death_row.hashinfo,
			       skb, __tcp_hdrlen(th), th->source,
			       th->dest, sdif, &refcounted);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!sk)
		goto no_tcp_socket;

process:
	if (sk->sk_state == TCP_TIME_WAIT)
		goto do_time_wait;

<<<<<<< HEAD
	if (unlikely(iph->ttl < inet_sk(sk)->min_ttl)) {
		NET_INC_STATS_BH(net, LINUX_MIB_TCPMINTTLDROP);
		goto discard_and_relse;
	}

	if (!xfrm4_policy_check(sk, XFRM_POLICY_IN, skb))
		goto discard_and_relse;
	nf_reset(skb);

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
			ret = tcp_v4_do_rcv(sk, skb);
		else
#endif
		{
			if (!tcp_prequeue(sk, skb))
				ret = tcp_v4_do_rcv(sk, skb);
		}
	} else if (unlikely(sk_add_backlog(sk, skb))) {
		bh_unlock_sock(sk);
		NET_INC_STATS_BH(net, LINUX_MIB_TCPBACKLOGDROP);
		goto discard_and_relse;
	}
	bh_unlock_sock(sk);

	sock_put(sk);
=======
	if (sk->sk_state == TCP_NEW_SYN_RECV) {
		struct request_sock *req = inet_reqsk(sk);
		bool req_stolen = false;
		struct sock *nsk;

		sk = req->rsk_listener;
		if (!xfrm4_policy_check(sk, XFRM_POLICY_IN, skb))
			drop_reason = SKB_DROP_REASON_XFRM_POLICY;
		else
			drop_reason = tcp_inbound_hash(sk, req, skb,
						       &iph->saddr, &iph->daddr,
						       AF_INET, dif, sdif);
		if (unlikely(drop_reason)) {
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
			/* We own a reference on the listener, increase it again
			 * as we might lose it too soon.
			 */
			sock_hold(sk);
		}
		refcounted = true;
		nsk = NULL;
		if (!tcp_filter(sk, skb)) {
			th = (const struct tcphdr *)skb->data;
			iph = ip_hdr(skb);
			tcp_v4_fill_cb(skb, iph, th);
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
				tcp_v4_restore_cb(skb);
				sock_put(sk);
				goto lookup;
			}
			goto discard_and_relse;
		}
		nf_reset_ct(skb);
		if (nsk == sk) {
			reqsk_put(req);
			tcp_v4_restore_cb(skb);
		} else {
			drop_reason = tcp_child_process(sk, nsk, skb);
			if (drop_reason) {
				tcp_v4_send_reset(nsk, skb);
				goto discard_and_relse;
			}
			sock_put(sk);
			return 0;
		}
	}

	if (static_branch_unlikely(&ip4_min_ttl)) {
		/* min_ttl can be changed concurrently from do_ip_setsockopt() */
		if (unlikely(iph->ttl < READ_ONCE(inet_sk(sk)->min_ttl))) {
			__NET_INC_STATS(net, LINUX_MIB_TCPMINTTLDROP);
			drop_reason = SKB_DROP_REASON_TCP_MINTTL;
			goto discard_and_relse;
		}
	}

	if (!xfrm4_policy_check(sk, XFRM_POLICY_IN, skb)) {
		drop_reason = SKB_DROP_REASON_XFRM_POLICY;
		goto discard_and_relse;
	}

	drop_reason = tcp_inbound_hash(sk, NULL, skb, &iph->saddr, &iph->daddr,
				       AF_INET, dif, sdif);
	if (drop_reason)
		goto discard_and_relse;

	nf_reset_ct(skb);

	if (tcp_filter(sk, skb)) {
		drop_reason = SKB_DROP_REASON_SOCKET_FILTER;
		goto discard_and_relse;
	}
	th = (const struct tcphdr *)skb->data;
	iph = ip_hdr(skb);
	tcp_v4_fill_cb(skb, iph, th);

	skb->dev = NULL;

	if (sk->sk_state == TCP_LISTEN) {
		ret = tcp_v4_do_rcv(sk, skb);
		goto put_and_return;
	}

	sk_incoming_cpu_update(sk);

	bh_lock_sock_nested(sk);
	tcp_segs_in(tcp_sk(sk), skb);
	ret = 0;
	if (!sock_owned_by_user(sk)) {
		ret = tcp_v4_do_rcv(sk, skb);
	} else {
		if (tcp_add_backlog(sk, skb, &drop_reason))
			goto discard_and_relse;
	}
	bh_unlock_sock(sk);

put_and_return:
	if (refcounted)
		sock_put(sk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ret;

no_tcp_socket:
<<<<<<< HEAD
	if (!xfrm4_policy_check(NULL, XFRM_POLICY_IN, skb))
		goto discard_it;

	if (skb->len < (th->doff << 2) || tcp_checksum_complete(skb)) {
bad_packet:
		TCP_INC_STATS_BH(net, TCP_MIB_INERRS);
=======
	drop_reason = SKB_DROP_REASON_NO_SOCKET;
	if (!xfrm4_policy_check(NULL, XFRM_POLICY_IN, skb))
		goto discard_it;

	tcp_v4_fill_cb(skb, iph, th);

	if (tcp_checksum_complete(skb)) {
csum_error:
		drop_reason = SKB_DROP_REASON_TCP_CSUM;
		trace_tcp_bad_csum(skb);
		__TCP_INC_STATS(net, TCP_MIB_CSUMERRORS);
bad_packet:
		__TCP_INC_STATS(net, TCP_MIB_INERRS);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		tcp_v4_send_reset(NULL, skb);
	}

discard_it:
<<<<<<< HEAD
	/* Discard frame. */
	kfree_skb(skb);
	return 0;

discard_and_relse:
	sock_put(sk);
=======
	SKB_DR_OR(drop_reason, NOT_SPECIFIED);
	/* Discard frame. */
	kfree_skb_reason(skb, drop_reason);
	return 0;

discard_and_relse:
	sk_drops_add(sk, skb);
	if (refcounted)
		sock_put(sk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	goto discard_it;

do_time_wait:
	if (!xfrm4_policy_check(NULL, XFRM_POLICY_IN, skb)) {
<<<<<<< HEAD
=======
		drop_reason = SKB_DROP_REASON_XFRM_POLICY;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		inet_twsk_put(inet_twsk(sk));
		goto discard_it;
	}

<<<<<<< HEAD
	if (skb->len < (th->doff << 2) || tcp_checksum_complete(skb)) {
		TCP_INC_STATS_BH(net, TCP_MIB_INERRS);
		inet_twsk_put(inet_twsk(sk));
		goto discard_it;
	}
	switch (tcp_timewait_state_process(inet_twsk(sk), skb, th)) {
	case TCP_TW_SYN: {
		struct sock *sk2 = inet_lookup_listener(dev_net(skb->dev),
							&tcp_hashinfo,
							iph->daddr, th->dest,
							inet_iif(skb));
		if (sk2) {
			inet_twsk_deschedule(inet_twsk(sk), &tcp_death_row);
			inet_twsk_put(inet_twsk(sk));
			sk = sk2;
			goto process;
		}
		/* Fall through to ACK */
	}
=======
	tcp_v4_fill_cb(skb, iph, th);

	if (tcp_checksum_complete(skb)) {
		inet_twsk_put(inet_twsk(sk));
		goto csum_error;
	}
	switch (tcp_timewait_state_process(inet_twsk(sk), skb, th)) {
	case TCP_TW_SYN: {
		struct sock *sk2 = inet_lookup_listener(net,
							net->ipv4.tcp_death_row.hashinfo,
							skb, __tcp_hdrlen(th),
							iph->saddr, th->source,
							iph->daddr, th->dest,
							inet_iif(skb),
							sdif);
		if (sk2) {
			inet_twsk_deschedule_put(inet_twsk(sk));
			sk = sk2;
			tcp_v4_restore_cb(skb);
			refcounted = false;
			goto process;
		}
	}
		/* to ACK */
		fallthrough;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case TCP_TW_ACK:
		tcp_v4_timewait_ack(sk, skb);
		break;
	case TCP_TW_RST:
<<<<<<< HEAD
		goto no_tcp_socket;
=======
		tcp_v4_send_reset(sk, skb);
		inet_twsk_deschedule_put(inet_twsk(sk));
		goto discard_it;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case TCP_TW_SUCCESS:;
	}
	goto discard_it;
}

<<<<<<< HEAD
struct inet_peer *tcp_v4_get_peer(struct sock *sk, bool *release_it)
{
	struct rtable *rt = (struct rtable *) __sk_dst_get(sk);
	struct inet_sock *inet = inet_sk(sk);
	struct inet_peer *peer;

	if (!rt ||
	    inet->cork.fl.u.ip4.daddr != inet->inet_daddr) {
		peer = inet_getpeer_v4(inet->inet_daddr, 1);
		*release_it = true;
	} else {
		if (!rt->peer)
			rt_bind_peer(rt, inet->inet_daddr, 1);
		peer = rt->peer;
		*release_it = false;
	}

	return peer;
}
EXPORT_SYMBOL(tcp_v4_get_peer);

void *tcp_v4_tw_get_peer(struct sock *sk)
{
	const struct inet_timewait_sock *tw = inet_twsk(sk);

	return inet_getpeer_v4(tw->tw_daddr, 1);
}
EXPORT_SYMBOL(tcp_v4_tw_get_peer);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct timewait_sock_ops tcp_timewait_sock_ops = {
	.twsk_obj_size	= sizeof(struct tcp_timewait_sock),
	.twsk_unique	= tcp_twsk_unique,
	.twsk_destructor= tcp_twsk_destructor,
<<<<<<< HEAD
	.twsk_getpeer	= tcp_v4_tw_get_peer,
};

=======
};

void inet_sk_rx_dst_set(struct sock *sk, const struct sk_buff *skb)
{
	struct dst_entry *dst = skb_dst(skb);

	if (dst && dst_hold_safe(dst)) {
		rcu_assign_pointer(sk->sk_rx_dst, dst);
		sk->sk_rx_dst_ifindex = skb->skb_iif;
	}
}
EXPORT_SYMBOL(inet_sk_rx_dst_set);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
const struct inet_connection_sock_af_ops ipv4_specific = {
	.queue_xmit	   = ip_queue_xmit,
	.send_check	   = tcp_v4_send_check,
	.rebuild_header	   = inet_sk_rebuild_header,
<<<<<<< HEAD
	.conn_request	   = tcp_v4_conn_request,
	.syn_recv_sock	   = tcp_v4_syn_recv_sock,
	.get_peer	   = tcp_v4_get_peer,
=======
	.sk_rx_dst_set	   = inet_sk_rx_dst_set,
	.conn_request	   = tcp_v4_conn_request,
	.syn_recv_sock	   = tcp_v4_syn_recv_sock,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.net_header_len	   = sizeof(struct iphdr),
	.setsockopt	   = ip_setsockopt,
	.getsockopt	   = ip_getsockopt,
	.addr2sockaddr	   = inet_csk_addr2sockaddr,
	.sockaddr_len	   = sizeof(struct sockaddr_in),
<<<<<<< HEAD
	.bind_conflict	   = inet_csk_bind_conflict,
#ifdef CONFIG_COMPAT
	.compat_setsockopt = compat_ip_setsockopt,
	.compat_getsockopt = compat_ip_getsockopt,
#endif
};
EXPORT_SYMBOL(ipv4_specific);

#ifdef CONFIG_TCP_MD5SIG
static const struct tcp_sock_af_ops tcp_sock_ipv4_specific = {
	.md5_lookup		= tcp_v4_md5_lookup,
	.calc_md5_hash		= tcp_v4_md5_hash_skb,
	.md5_parse		= tcp_v4_parse_md5_keys,
=======
	.mtu_reduced	   = tcp_v4_mtu_reduced,
};
EXPORT_SYMBOL(ipv4_specific);

#if defined(CONFIG_TCP_MD5SIG) || defined(CONFIG_TCP_AO)
static const struct tcp_sock_af_ops tcp_sock_ipv4_specific = {
#ifdef CONFIG_TCP_MD5SIG
	.md5_lookup		= tcp_v4_md5_lookup,
	.calc_md5_hash		= tcp_v4_md5_hash_skb,
	.md5_parse		= tcp_v4_parse_md5_keys,
#endif
#ifdef CONFIG_TCP_AO
	.ao_lookup		= tcp_v4_ao_lookup,
	.calc_ao_hash		= tcp_v4_ao_hash_skb,
	.ao_parse		= tcp_v4_parse_ao,
	.ao_calc_key_sk		= tcp_v4_ao_calc_key_sk,
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};
#endif

/* NOTE: A lot of things set to zero explicitly by call to
 *       sk_alloc() so need not be done here.
 */
static int tcp_v4_init_sock(struct sock *sk)
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
	tp->snd_cwnd = TCP_INIT_CWND;

	/* See draft-stevens-tcpca-spec-01 for discussion of the
	 * initialization of these values.
	 */
	tp->snd_ssthresh = TCP_INFINITE_SSTHRESH;
	tp->snd_cwnd_clamp = ~0;
	tp->mss_cache = TCP_MSS_DEFAULT;
	u64_stats_init(&tp->syncp);

	tp->reordering = sysctl_tcp_reordering;
	icsk->icsk_ca_ops = &tcp_init_congestion_ops;

	sk->sk_state = TCP_CLOSE;

	sk->sk_write_space = sk_stream_write_space;
	sock_set_flag(sk, SOCK_USE_WRITE_QUEUE);

	icsk->icsk_af_ops = &ipv4_specific;
	icsk->icsk_sync_mss = tcp_sync_mss;
#ifdef CONFIG_TCP_MD5SIG
	tp->af_specific = &tcp_sock_ipv4_specific;
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

=======

	tcp_init_sock(sk);

	icsk->icsk_af_ops = &ipv4_specific;

#if defined(CONFIG_TCP_MD5SIG) || defined(CONFIG_TCP_AO)
	tcp_sk(sk)->af_specific = &tcp_sock_ipv4_specific;
#endif

	return 0;
}

#ifdef CONFIG_TCP_MD5SIG
static void tcp_md5sig_info_free_rcu(struct rcu_head *head)
{
	struct tcp_md5sig_info *md5sig;

	md5sig = container_of(head, struct tcp_md5sig_info, rcu);
	kfree(md5sig);
	static_branch_slow_dec_deferred(&tcp_md5_needed);
	tcp_md5_release_sigpool();
}
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void tcp_v4_destroy_sock(struct sock *sk)
{
	struct tcp_sock *tp = tcp_sk(sk);

<<<<<<< HEAD
=======
	trace_tcp_destroy_sock(sk);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	tcp_clear_xmit_timers(sk);

	tcp_cleanup_congestion_control(sk);

<<<<<<< HEAD
	/* Cleanup up the write buffer. */
	tcp_write_queue_purge(sk);

	/* Cleans up our, hopefully empty, out_of_order_queue. */
	__skb_queue_purge(&tp->out_of_order_queue);
=======
	tcp_cleanup_ulp(sk);

	/* Cleanup up the write buffer. */
	tcp_write_queue_purge(sk);

	/* Check if we want to disable active TFO */
	tcp_fastopen_active_disable_ofo_check(sk);

	/* Cleans up our, hopefully empty, out_of_order_queue. */
	skb_rbtree_purge(&tp->out_of_order_queue);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_TCP_MD5SIG
	/* Clean up the MD5 key list, if any */
	if (tp->md5sig_info) {
<<<<<<< HEAD
		tcp_clear_md5_list(sk);
		kfree_rcu(tp->md5sig_info, rcu);
		tp->md5sig_info = NULL;
	}
#endif

#ifdef CONFIG_NET_DMA
	/* Cleans up our sk_async_wait_queue */
	__skb_queue_purge(&sk->sk_async_wait_queue);
#endif

	/* Clean prequeue, it must be empty really */
	__skb_queue_purge(&tp->ucopy.prequeue);
=======
		struct tcp_md5sig_info *md5sig;

		md5sig = rcu_dereference_protected(tp->md5sig_info, 1);
		tcp_clear_md5_list(sk);
		call_rcu(&md5sig->rcu, tcp_md5sig_info_free_rcu);
		rcu_assign_pointer(tp->md5sig_info, NULL);
	}
#endif
	tcp_ao_destroy_sock(sk, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Clean up a referenced TCP bind bucket. */
	if (inet_csk(sk)->icsk_bind_hash)
		inet_put_port(sk);

<<<<<<< HEAD
	/*
	 * If sendmsg cached page exists, toss it.
	 */
	if (sk->sk_sndmsg_page) {
		__free_page(sk->sk_sndmsg_page);
		sk->sk_sndmsg_page = NULL;
	}

	/* TCP Cookie Transactions */
	if (tp->cookie_values != NULL) {
		kref_put(&tp->cookie_values->kref,
			 tcp_cookie_values_release);
		tp->cookie_values = NULL;
	}

	sk_sockets_allocated_dec(sk);
	sock_release_memcg(sk);
=======
	BUG_ON(rcu_access_pointer(tp->fastopen_rsk));

	/* If socket is aborted during connect operation */
	tcp_free_fastopen_req(tp);
	tcp_fastopen_destroy_cipher(sk);
	tcp_saved_syn_free(tp);

	sk_sockets_allocated_dec(sk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL(tcp_v4_destroy_sock);

#ifdef CONFIG_PROC_FS
/* Proc filesystem TCP sock list dumping. */

<<<<<<< HEAD
static inline struct inet_timewait_sock *tw_head(struct hlist_nulls_head *head)
{
	return hlist_nulls_empty(head) ? NULL :
		list_entry(head->first, struct inet_timewait_sock, tw_node);
}

static inline struct inet_timewait_sock *tw_next(struct inet_timewait_sock *tw)
{
	return !is_a_nulls(tw->tw_node.next) ?
		hlist_nulls_entry(tw->tw_node.next, typeof(*tw), tw_node) : NULL;
}

/*
 * Get next listener socket follow cur.  If cur is NULL, get first socket
 * starting from bucket given in st->bucket; when st->bucket is zero the
 * very first socket in the hash table is returned.
 */
static void *listening_get_next(struct seq_file *seq, void *cur)
{
	struct inet_connection_sock *icsk;
	struct hlist_nulls_node *node;
	struct sock *sk = cur;
	struct inet_listen_hashbucket *ilb;
	struct tcp_iter_state *st = seq->private;
	struct net *net = seq_file_net(seq);

	if (!sk) {
		ilb = &tcp_hashinfo.listening_hash[st->bucket];
		spin_lock_bh(&ilb->lock);
		sk = sk_nulls_head(&ilb->head);
		st->offset = 0;
		goto get_sk;
	}
	ilb = &tcp_hashinfo.listening_hash[st->bucket];
	++st->num;
	++st->offset;

	if (st->state == TCP_SEQ_STATE_OPENREQ) {
		struct request_sock *req = cur;

		icsk = inet_csk(st->syn_wait_sk);
		req = req->dl_next;
		while (1) {
			while (req) {
				if (req->rsk_ops->family == st->family) {
					cur = req;
					goto out;
				}
				req = req->dl_next;
			}
			if (++st->sbucket >= icsk->icsk_accept_queue.listen_opt->nr_table_entries)
				break;
get_req:
			req = icsk->icsk_accept_queue.listen_opt->syn_table[st->sbucket];
		}
		sk	  = sk_nulls_next(st->syn_wait_sk);
		st->state = TCP_SEQ_STATE_LISTENING;
		read_unlock_bh(&icsk->icsk_accept_queue.syn_wait_lock);
	} else {
		icsk = inet_csk(sk);
		read_lock_bh(&icsk->icsk_accept_queue.syn_wait_lock);
		if (reqsk_queue_len(&icsk->icsk_accept_queue))
			goto start_req;
		read_unlock_bh(&icsk->icsk_accept_queue.syn_wait_lock);
		sk = sk_nulls_next(sk);
	}
get_sk:
	sk_nulls_for_each_from(sk, node) {
		if (!net_eq(sock_net(sk), net))
			continue;
		if (sk->sk_family == st->family) {
			cur = sk;
			goto out;
		}
		icsk = inet_csk(sk);
		read_lock_bh(&icsk->icsk_accept_queue.syn_wait_lock);
		if (reqsk_queue_len(&icsk->icsk_accept_queue)) {
start_req:
			st->uid		= sock_i_uid(sk);
			st->syn_wait_sk = sk;
			st->state	= TCP_SEQ_STATE_OPENREQ;
			st->sbucket	= 0;
			goto get_req;
		}
		read_unlock_bh(&icsk->icsk_accept_queue.syn_wait_lock);
	}
	spin_unlock_bh(&ilb->lock);
	st->offset = 0;
	if (++st->bucket < INET_LHTABLE_SIZE) {
		ilb = &tcp_hashinfo.listening_hash[st->bucket];
		spin_lock_bh(&ilb->lock);
		sk = sk_nulls_head(&ilb->head);
		goto get_sk;
	}
	cur = NULL;
out:
	return cur;
=======
static unsigned short seq_file_family(const struct seq_file *seq);

static bool seq_sk_match(struct seq_file *seq, const struct sock *sk)
{
	unsigned short family = seq_file_family(seq);

	/* AF_UNSPEC is used as a match all */
	return ((family == AF_UNSPEC || family == sk->sk_family) &&
		net_eq(sock_net(sk), seq_file_net(seq)));
}

/* Find a non empty bucket (starting from st->bucket)
 * and return the first sk from it.
 */
static void *listening_get_first(struct seq_file *seq)
{
	struct inet_hashinfo *hinfo = seq_file_net(seq)->ipv4.tcp_death_row.hashinfo;
	struct tcp_iter_state *st = seq->private;

	st->offset = 0;
	for (; st->bucket <= hinfo->lhash2_mask; st->bucket++) {
		struct inet_listen_hashbucket *ilb2;
		struct hlist_nulls_node *node;
		struct sock *sk;

		ilb2 = &hinfo->lhash2[st->bucket];
		if (hlist_nulls_empty(&ilb2->nulls_head))
			continue;

		spin_lock(&ilb2->lock);
		sk_nulls_for_each(sk, node, &ilb2->nulls_head) {
			if (seq_sk_match(seq, sk))
				return sk;
		}
		spin_unlock(&ilb2->lock);
	}

	return NULL;
}

/* Find the next sk of "cur" within the same bucket (i.e. st->bucket).
 * If "cur" is the last one in the st->bucket,
 * call listening_get_first() to return the first sk of the next
 * non empty bucket.
 */
static void *listening_get_next(struct seq_file *seq, void *cur)
{
	struct tcp_iter_state *st = seq->private;
	struct inet_listen_hashbucket *ilb2;
	struct hlist_nulls_node *node;
	struct inet_hashinfo *hinfo;
	struct sock *sk = cur;

	++st->num;
	++st->offset;

	sk = sk_nulls_next(sk);
	sk_nulls_for_each_from(sk, node) {
		if (seq_sk_match(seq, sk))
			return sk;
	}

	hinfo = seq_file_net(seq)->ipv4.tcp_death_row.hashinfo;
	ilb2 = &hinfo->lhash2[st->bucket];
	spin_unlock(&ilb2->lock);
	++st->bucket;
	return listening_get_first(seq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void *listening_get_idx(struct seq_file *seq, loff_t *pos)
{
	struct tcp_iter_state *st = seq->private;
	void *rc;

	st->bucket = 0;
	st->offset = 0;
<<<<<<< HEAD
	rc = listening_get_next(seq, NULL);
=======
	rc = listening_get_first(seq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	while (rc && *pos) {
		rc = listening_get_next(seq, rc);
		--*pos;
	}
	return rc;
}

<<<<<<< HEAD
static inline int empty_bucket(struct tcp_iter_state *st)
{
	return hlist_nulls_empty(&tcp_hashinfo.ehash[st->bucket].chain) &&
		hlist_nulls_empty(&tcp_hashinfo.ehash[st->bucket].twchain);
=======
static inline bool empty_bucket(struct inet_hashinfo *hinfo,
				const struct tcp_iter_state *st)
{
	return hlist_nulls_empty(&hinfo->ehash[st->bucket].chain);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Get first established socket starting from bucket given in st->bucket.
 * If st->bucket is zero, the very first socket in the hash is returned.
 */
static void *established_get_first(struct seq_file *seq)
{
<<<<<<< HEAD
	struct tcp_iter_state *st = seq->private;
	struct net *net = seq_file_net(seq);
	void *rc = NULL;

	st->offset = 0;
	for (; st->bucket <= tcp_hashinfo.ehash_mask; ++st->bucket) {
		struct sock *sk;
		struct hlist_nulls_node *node;
		struct inet_timewait_sock *tw;
		spinlock_t *lock = inet_ehash_lockp(&tcp_hashinfo, st->bucket);

		/* Lockless fast path for the common case of empty buckets */
		if (empty_bucket(st))
			continue;

		spin_lock_bh(lock);
		sk_nulls_for_each(sk, node, &tcp_hashinfo.ehash[st->bucket].chain) {
			if (sk->sk_family != st->family ||
			    !net_eq(sock_net(sk), net)) {
				continue;
			}
			rc = sk;
			goto out;
		}
		st->state = TCP_SEQ_STATE_TIME_WAIT;
		inet_twsk_for_each(tw, node,
				   &tcp_hashinfo.ehash[st->bucket].twchain) {
			if (tw->tw_family != st->family ||
			    !net_eq(twsk_net(tw), net)) {
				continue;
			}
			rc = tw;
			goto out;
		}
		spin_unlock_bh(lock);
		st->state = TCP_SEQ_STATE_ESTABLISHED;
	}
out:
	return rc;
=======
	struct inet_hashinfo *hinfo = seq_file_net(seq)->ipv4.tcp_death_row.hashinfo;
	struct tcp_iter_state *st = seq->private;

	st->offset = 0;
	for (; st->bucket <= hinfo->ehash_mask; ++st->bucket) {
		struct sock *sk;
		struct hlist_nulls_node *node;
		spinlock_t *lock = inet_ehash_lockp(hinfo, st->bucket);

		cond_resched();

		/* Lockless fast path for the common case of empty buckets */
		if (empty_bucket(hinfo, st))
			continue;

		spin_lock_bh(lock);
		sk_nulls_for_each(sk, node, &hinfo->ehash[st->bucket].chain) {
			if (seq_sk_match(seq, sk))
				return sk;
		}
		spin_unlock_bh(lock);
	}

	return NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void *established_get_next(struct seq_file *seq, void *cur)
{
<<<<<<< HEAD
	struct sock *sk = cur;
	struct inet_timewait_sock *tw;
	struct hlist_nulls_node *node;
	struct tcp_iter_state *st = seq->private;
	struct net *net = seq_file_net(seq);
=======
	struct inet_hashinfo *hinfo = seq_file_net(seq)->ipv4.tcp_death_row.hashinfo;
	struct tcp_iter_state *st = seq->private;
	struct hlist_nulls_node *node;
	struct sock *sk = cur;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	++st->num;
	++st->offset;

<<<<<<< HEAD
	if (st->state == TCP_SEQ_STATE_TIME_WAIT) {
		tw = cur;
		tw = tw_next(tw);
get_tw:
		while (tw && (tw->tw_family != st->family || !net_eq(twsk_net(tw), net))) {
			tw = tw_next(tw);
		}
		if (tw) {
			cur = tw;
			goto out;
		}
		spin_unlock_bh(inet_ehash_lockp(&tcp_hashinfo, st->bucket));
		st->state = TCP_SEQ_STATE_ESTABLISHED;

		/* Look for next non empty bucket */
		st->offset = 0;
		while (++st->bucket <= tcp_hashinfo.ehash_mask &&
				empty_bucket(st))
			;
		if (st->bucket > tcp_hashinfo.ehash_mask)
			return NULL;

		spin_lock_bh(inet_ehash_lockp(&tcp_hashinfo, st->bucket));
		sk = sk_nulls_head(&tcp_hashinfo.ehash[st->bucket].chain);
	} else
		sk = sk_nulls_next(sk);

	sk_nulls_for_each_from(sk, node) {
		if (sk->sk_family == st->family && net_eq(sock_net(sk), net))
			goto found;
	}

	st->state = TCP_SEQ_STATE_TIME_WAIT;
	tw = tw_head(&tcp_hashinfo.ehash[st->bucket].twchain);
	goto get_tw;
found:
	cur = sk;
out:
	return cur;
=======
	sk = sk_nulls_next(sk);

	sk_nulls_for_each_from(sk, node) {
		if (seq_sk_match(seq, sk))
			return sk;
	}

	spin_unlock_bh(inet_ehash_lockp(hinfo, st->bucket));
	++st->bucket;
	return established_get_first(seq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void *established_get_idx(struct seq_file *seq, loff_t pos)
{
	struct tcp_iter_state *st = seq->private;
	void *rc;

	st->bucket = 0;
	rc = established_get_first(seq);

	while (rc && pos) {
		rc = established_get_next(seq, rc);
		--pos;
	}
	return rc;
}

static void *tcp_get_idx(struct seq_file *seq, loff_t pos)
{
	void *rc;
	struct tcp_iter_state *st = seq->private;

	st->state = TCP_SEQ_STATE_LISTENING;
	rc	  = listening_get_idx(seq, &pos);

	if (!rc) {
		st->state = TCP_SEQ_STATE_ESTABLISHED;
		rc	  = established_get_idx(seq, pos);
	}

	return rc;
}

static void *tcp_seek_last_pos(struct seq_file *seq)
{
<<<<<<< HEAD
	struct tcp_iter_state *st = seq->private;
=======
	struct inet_hashinfo *hinfo = seq_file_net(seq)->ipv4.tcp_death_row.hashinfo;
	struct tcp_iter_state *st = seq->private;
	int bucket = st->bucket;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int offset = st->offset;
	int orig_num = st->num;
	void *rc = NULL;

	switch (st->state) {
<<<<<<< HEAD
	case TCP_SEQ_STATE_OPENREQ:
	case TCP_SEQ_STATE_LISTENING:
		if (st->bucket >= INET_LHTABLE_SIZE)
			break;
		st->state = TCP_SEQ_STATE_LISTENING;
		rc = listening_get_next(seq, NULL);
		while (offset-- && rc)
=======
	case TCP_SEQ_STATE_LISTENING:
		if (st->bucket > hinfo->lhash2_mask)
			break;
		rc = listening_get_first(seq);
		while (offset-- && rc && bucket == st->bucket)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			rc = listening_get_next(seq, rc);
		if (rc)
			break;
		st->bucket = 0;
<<<<<<< HEAD
		/* Fallthrough */
	case TCP_SEQ_STATE_ESTABLISHED:
	case TCP_SEQ_STATE_TIME_WAIT:
		st->state = TCP_SEQ_STATE_ESTABLISHED;
		if (st->bucket > tcp_hashinfo.ehash_mask)
			break;
		rc = established_get_first(seq);
		while (offset-- && rc)
=======
		st->state = TCP_SEQ_STATE_ESTABLISHED;
		fallthrough;
	case TCP_SEQ_STATE_ESTABLISHED:
		if (st->bucket > hinfo->ehash_mask)
			break;
		rc = established_get_first(seq);
		while (offset-- && rc && bucket == st->bucket)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			rc = established_get_next(seq, rc);
	}

	st->num = orig_num;

	return rc;
}

<<<<<<< HEAD
static void *tcp_seq_start(struct seq_file *seq, loff_t *pos)
=======
void *tcp_seq_start(struct seq_file *seq, loff_t *pos)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct tcp_iter_state *st = seq->private;
	void *rc;

	if (*pos && *pos == st->last_pos) {
		rc = tcp_seek_last_pos(seq);
		if (rc)
			goto out;
	}

	st->state = TCP_SEQ_STATE_LISTENING;
	st->num = 0;
	st->bucket = 0;
	st->offset = 0;
	rc = *pos ? tcp_get_idx(seq, *pos - 1) : SEQ_START_TOKEN;

out:
	st->last_pos = *pos;
	return rc;
}
<<<<<<< HEAD

static void *tcp_seq_next(struct seq_file *seq, void *v, loff_t *pos)
=======
EXPORT_SYMBOL(tcp_seq_start);

void *tcp_seq_next(struct seq_file *seq, void *v, loff_t *pos)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct tcp_iter_state *st = seq->private;
	void *rc = NULL;

	if (v == SEQ_START_TOKEN) {
		rc = tcp_get_idx(seq, 0);
		goto out;
	}

	switch (st->state) {
<<<<<<< HEAD
	case TCP_SEQ_STATE_OPENREQ:
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case TCP_SEQ_STATE_LISTENING:
		rc = listening_get_next(seq, v);
		if (!rc) {
			st->state = TCP_SEQ_STATE_ESTABLISHED;
			st->bucket = 0;
			st->offset = 0;
			rc	  = established_get_first(seq);
		}
		break;
	case TCP_SEQ_STATE_ESTABLISHED:
<<<<<<< HEAD
	case TCP_SEQ_STATE_TIME_WAIT:
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		rc = established_get_next(seq, v);
		break;
	}
out:
	++*pos;
	st->last_pos = *pos;
	return rc;
}
<<<<<<< HEAD

static void tcp_seq_stop(struct seq_file *seq, void *v)
{
	struct tcp_iter_state *st = seq->private;

	switch (st->state) {
	case TCP_SEQ_STATE_OPENREQ:
		if (v) {
			struct inet_connection_sock *icsk = inet_csk(st->syn_wait_sk);
			read_unlock_bh(&icsk->icsk_accept_queue.syn_wait_lock);
		}
	case TCP_SEQ_STATE_LISTENING:
		if (v != SEQ_START_TOKEN)
			spin_unlock_bh(&tcp_hashinfo.listening_hash[st->bucket].lock);
		break;
	case TCP_SEQ_STATE_TIME_WAIT:
	case TCP_SEQ_STATE_ESTABLISHED:
		if (v)
			spin_unlock_bh(inet_ehash_lockp(&tcp_hashinfo, st->bucket));
		break;
	}
}

int tcp_seq_open(struct inode *inode, struct file *file)
{
	struct tcp_seq_afinfo *afinfo = PDE(inode)->data;
	struct tcp_iter_state *s;
	int err;

	err = seq_open_net(inode, file, &afinfo->seq_ops,
			  sizeof(struct tcp_iter_state));
	if (err < 0)
		return err;

	s = ((struct seq_file *)file->private_data)->private;
	s->family		= afinfo->family;
	s->last_pos 		= 0;
	return 0;
}
EXPORT_SYMBOL(tcp_seq_open);

int tcp_proc_register(struct net *net, struct tcp_seq_afinfo *afinfo)
{
	int rc = 0;
	struct proc_dir_entry *p;

	afinfo->seq_ops.start		= tcp_seq_start;
	afinfo->seq_ops.next		= tcp_seq_next;
	afinfo->seq_ops.stop		= tcp_seq_stop;

	p = proc_create_data(afinfo->name, S_IRUGO, net->proc_net,
			     afinfo->seq_fops, afinfo);
	if (!p)
		rc = -ENOMEM;
	return rc;
}
EXPORT_SYMBOL(tcp_proc_register);

void tcp_proc_unregister(struct net *net, struct tcp_seq_afinfo *afinfo)
{
	proc_net_remove(net, afinfo->name);
}
EXPORT_SYMBOL(tcp_proc_unregister);

static void get_openreq4(const struct sock *sk, const struct request_sock *req,
			 struct seq_file *f, int i, int uid)
{
	const struct inet_request_sock *ireq = inet_rsk(req);
	int ttd = req->expires - jiffies;

	seq_printf(f, "%4d: %08X:%04X %08X:%04X"
		" %02X %08X:%08X %02X:%08lX %08X %5d %8d %u %d %pK",
		i,
		ireq->loc_addr,
		ntohs(inet_sk(sk)->inet_sport),
		ireq->rmt_addr,
		ntohs(ireq->rmt_port),
		TCP_SYN_RECV,
		0, 0, /* could print option size, but that is af dependent. */
		1,    /* timers active (only the expire timer) */
		jiffies_to_clock_t(ttd),
		req->retrans,
		uid,
		0,  /* non standard timer */
		0, /* open_requests have no inode */
		atomic_read(&sk->sk_refcnt),
=======
EXPORT_SYMBOL(tcp_seq_next);

void tcp_seq_stop(struct seq_file *seq, void *v)
{
	struct inet_hashinfo *hinfo = seq_file_net(seq)->ipv4.tcp_death_row.hashinfo;
	struct tcp_iter_state *st = seq->private;

	switch (st->state) {
	case TCP_SEQ_STATE_LISTENING:
		if (v != SEQ_START_TOKEN)
			spin_unlock(&hinfo->lhash2[st->bucket].lock);
		break;
	case TCP_SEQ_STATE_ESTABLISHED:
		if (v)
			spin_unlock_bh(inet_ehash_lockp(hinfo, st->bucket));
		break;
	}
}
EXPORT_SYMBOL(tcp_seq_stop);

static void get_openreq4(const struct request_sock *req,
			 struct seq_file *f, int i)
{
	const struct inet_request_sock *ireq = inet_rsk(req);
	long delta = req->rsk_timer.expires - jiffies;

	seq_printf(f, "%4d: %08X:%04X %08X:%04X"
		" %02X %08X:%08X %02X:%08lX %08X %5u %8d %u %d %pK",
		i,
		ireq->ir_loc_addr,
		ireq->ir_num,
		ireq->ir_rmt_addr,
		ntohs(ireq->ir_rmt_port),
		TCP_SYN_RECV,
		0, 0, /* could print option size, but that is af dependent. */
		1,    /* timers active (only the expire timer) */
		jiffies_delta_to_clock_t(delta),
		req->num_timeout,
		from_kuid_munged(seq_user_ns(f),
				 sock_i_uid(req->rsk_listener)),
		0,  /* non standard timer */
		0, /* open_requests have no inode */
		0,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		req);
}

static void get_tcp4_sock(struct sock *sk, struct seq_file *f, int i)
{
	int timer_active;
	unsigned long timer_expires;
	const struct tcp_sock *tp = tcp_sk(sk);
	const struct inet_connection_sock *icsk = inet_csk(sk);
	const struct inet_sock *inet = inet_sk(sk);
<<<<<<< HEAD
=======
	const struct fastopen_queue *fastopenq = &icsk->icsk_accept_queue.fastopenq;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	__be32 dest = inet->inet_daddr;
	__be32 src = inet->inet_rcv_saddr;
	__u16 destp = ntohs(inet->inet_dport);
	__u16 srcp = ntohs(inet->inet_sport);
	int rx_queue;
<<<<<<< HEAD

	if (icsk->icsk_pending == ICSK_TIME_RETRANS) {
=======
	int state;

	if (icsk->icsk_pending == ICSK_TIME_RETRANS ||
	    icsk->icsk_pending == ICSK_TIME_REO_TIMEOUT ||
	    icsk->icsk_pending == ICSK_TIME_LOSS_PROBE) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		timer_active	= 1;
		timer_expires	= icsk->icsk_timeout;
	} else if (icsk->icsk_pending == ICSK_TIME_PROBE0) {
		timer_active	= 4;
		timer_expires	= icsk->icsk_timeout;
	} else if (timer_pending(&sk->sk_timer)) {
		timer_active	= 2;
		timer_expires	= sk->sk_timer.expires;
	} else {
		timer_active	= 0;
		timer_expires = jiffies;
	}

<<<<<<< HEAD
	if (sk->sk_state == TCP_LISTEN)
		rx_queue = sk->sk_ack_backlog;
	else
		/*
		 * because we dont lock socket, we might find a transient negative value
		 */
		rx_queue = max_t(int, tp->rcv_nxt - tp->copied_seq, 0);

	seq_printf(f, "%4d: %08X:%04X %08X:%04X %02X %08X:%08X %02X:%08lX "
			"%08X %5d %8d %lu %d %pK %lu %lu %u %u %d",
		i, src, srcp, dest, destp, sk->sk_state,
		tp->write_seq - tp->snd_una,
		rx_queue,
		timer_active,
		jiffies_to_clock_t(timer_expires - jiffies),
		icsk->icsk_retransmits,
		sock_i_uid(sk),
		icsk->icsk_probes_out,
		sock_i_ino(sk),
		atomic_read(&sk->sk_refcnt), sk,
		jiffies_to_clock_t(icsk->icsk_rto),
		jiffies_to_clock_t(icsk->icsk_ack.ato),
		(icsk->icsk_ack.quick << 1) | icsk->icsk_ack.pingpong,
		tp->snd_cwnd,
		tcp_in_initial_slowstart(tp) ? -1 : tp->snd_ssthresh);

=======
	state = inet_sk_state_load(sk);
	if (state == TCP_LISTEN)
		rx_queue = READ_ONCE(sk->sk_ack_backlog);
	else
		/* Because we don't lock the socket,
		 * we might find a transient negative value.
		 */
		rx_queue = max_t(int, READ_ONCE(tp->rcv_nxt) -
				      READ_ONCE(tp->copied_seq), 0);

	seq_printf(f, "%4d: %08X:%04X %08X:%04X %02X %08X:%08X %02X:%08lX "
			"%08X %5u %8d %lu %d %pK %lu %lu %u %u %d",
		i, src, srcp, dest, destp, state,
		READ_ONCE(tp->write_seq) - tp->snd_una,
		rx_queue,
		timer_active,
		jiffies_delta_to_clock_t(timer_expires - jiffies),
		icsk->icsk_retransmits,
		from_kuid_munged(seq_user_ns(f), sock_i_uid(sk)),
		icsk->icsk_probes_out,
		sock_i_ino(sk),
		refcount_read(&sk->sk_refcnt), sk,
		jiffies_to_clock_t(icsk->icsk_rto),
		jiffies_to_clock_t(icsk->icsk_ack.ato),
		(icsk->icsk_ack.quick << 1) | inet_csk_in_pingpong_mode(sk),
		tcp_snd_cwnd(tp),
		state == TCP_LISTEN ?
		    fastopenq->max_qlen :
		    (tcp_in_initial_slowstart(tp) ? -1 : tp->snd_ssthresh));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void get_timewait4_sock(const struct inet_timewait_sock *tw,
			       struct seq_file *f, int i)
{
<<<<<<< HEAD
	__be32 dest, src;
	__u16 destp, srcp;
	int ttd = tw->tw_ttd - jiffies;

	if (ttd < 0)
		ttd = 0;
=======
	long delta = tw->tw_timer.expires - jiffies;
	__be32 dest, src;
	__u16 destp, srcp;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	dest  = tw->tw_daddr;
	src   = tw->tw_rcv_saddr;
	destp = ntohs(tw->tw_dport);
	srcp  = ntohs(tw->tw_sport);

	seq_printf(f, "%4d: %08X:%04X %08X:%04X"
		" %02X %08X:%08X %02X:%08lX %08X %5d %8d %d %d %pK",
		i, src, srcp, dest, destp, tw->tw_substate, 0, 0,
<<<<<<< HEAD
		3, jiffies_to_clock_t(ttd), 0, 0, 0, 0,
		atomic_read(&tw->tw_refcnt), tw);
=======
		3, jiffies_delta_to_clock_t(delta), 0, 0, 0, 0,
		refcount_read(&tw->tw_refcnt), tw);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#define TMPSZ 150

static int tcp4_seq_show(struct seq_file *seq, void *v)
{
	struct tcp_iter_state *st;
<<<<<<< HEAD
=======
	struct sock *sk = v;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	seq_setwidth(seq, TMPSZ - 1);
	if (v == SEQ_START_TOKEN) {
		seq_puts(seq, "  sl  local_address rem_address   st tx_queue "
			   "rx_queue tr tm->when retrnsmt   uid  timeout "
			   "inode");
		goto out;
	}
	st = seq->private;

<<<<<<< HEAD
	switch (st->state) {
	case TCP_SEQ_STATE_LISTENING:
	case TCP_SEQ_STATE_ESTABLISHED:
		get_tcp4_sock(v, seq, st->num);
		break;
	case TCP_SEQ_STATE_OPENREQ:
		get_openreq4(st->syn_wait_sk, v, seq, st->num, st->uid);
		break;
	case TCP_SEQ_STATE_TIME_WAIT:
		get_timewait4_sock(v, seq, st->num);
		break;
	}
=======
	if (sk->sk_state == TCP_TIME_WAIT)
		get_timewait4_sock(v, seq, st->num);
	else if (sk->sk_state == TCP_NEW_SYN_RECV)
		get_openreq4(v, seq, st->num);
	else
		get_tcp4_sock(v, seq, st->num);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	seq_pad(seq, '\n');
	return 0;
}

<<<<<<< HEAD
static const struct file_operations tcp_afinfo_seq_fops = {
	.owner   = THIS_MODULE,
	.open    = tcp_seq_open,
	.read    = seq_read,
	.llseek  = seq_lseek,
	.release = seq_release_net
};

static struct tcp_seq_afinfo tcp4_seq_afinfo = {
	.name		= "tcp",
	.family		= AF_INET,
	.seq_fops	= &tcp_afinfo_seq_fops,
	.seq_ops	= {
		.show		= tcp4_seq_show,
	},
=======
#ifdef CONFIG_BPF_SYSCALL
struct bpf_tcp_iter_state {
	struct tcp_iter_state state;
	unsigned int cur_sk;
	unsigned int end_sk;
	unsigned int max_sk;
	struct sock **batch;
	bool st_bucket_done;
};

struct bpf_iter__tcp {
	__bpf_md_ptr(struct bpf_iter_meta *, meta);
	__bpf_md_ptr(struct sock_common *, sk_common);
	uid_t uid __aligned(8);
};

static int tcp_prog_seq_show(struct bpf_prog *prog, struct bpf_iter_meta *meta,
			     struct sock_common *sk_common, uid_t uid)
{
	struct bpf_iter__tcp ctx;

	meta->seq_num--;  /* skip SEQ_START_TOKEN */
	ctx.meta = meta;
	ctx.sk_common = sk_common;
	ctx.uid = uid;
	return bpf_iter_run_prog(prog, &ctx);
}

static void bpf_iter_tcp_put_batch(struct bpf_tcp_iter_state *iter)
{
	while (iter->cur_sk < iter->end_sk)
		sock_gen_put(iter->batch[iter->cur_sk++]);
}

static int bpf_iter_tcp_realloc_batch(struct bpf_tcp_iter_state *iter,
				      unsigned int new_batch_sz)
{
	struct sock **new_batch;

	new_batch = kvmalloc(sizeof(*new_batch) * new_batch_sz,
			     GFP_USER | __GFP_NOWARN);
	if (!new_batch)
		return -ENOMEM;

	bpf_iter_tcp_put_batch(iter);
	kvfree(iter->batch);
	iter->batch = new_batch;
	iter->max_sk = new_batch_sz;

	return 0;
}

static unsigned int bpf_iter_tcp_listening_batch(struct seq_file *seq,
						 struct sock *start_sk)
{
	struct inet_hashinfo *hinfo = seq_file_net(seq)->ipv4.tcp_death_row.hashinfo;
	struct bpf_tcp_iter_state *iter = seq->private;
	struct tcp_iter_state *st = &iter->state;
	struct hlist_nulls_node *node;
	unsigned int expected = 1;
	struct sock *sk;

	sock_hold(start_sk);
	iter->batch[iter->end_sk++] = start_sk;

	sk = sk_nulls_next(start_sk);
	sk_nulls_for_each_from(sk, node) {
		if (seq_sk_match(seq, sk)) {
			if (iter->end_sk < iter->max_sk) {
				sock_hold(sk);
				iter->batch[iter->end_sk++] = sk;
			}
			expected++;
		}
	}
	spin_unlock(&hinfo->lhash2[st->bucket].lock);

	return expected;
}

static unsigned int bpf_iter_tcp_established_batch(struct seq_file *seq,
						   struct sock *start_sk)
{
	struct inet_hashinfo *hinfo = seq_file_net(seq)->ipv4.tcp_death_row.hashinfo;
	struct bpf_tcp_iter_state *iter = seq->private;
	struct tcp_iter_state *st = &iter->state;
	struct hlist_nulls_node *node;
	unsigned int expected = 1;
	struct sock *sk;

	sock_hold(start_sk);
	iter->batch[iter->end_sk++] = start_sk;

	sk = sk_nulls_next(start_sk);
	sk_nulls_for_each_from(sk, node) {
		if (seq_sk_match(seq, sk)) {
			if (iter->end_sk < iter->max_sk) {
				sock_hold(sk);
				iter->batch[iter->end_sk++] = sk;
			}
			expected++;
		}
	}
	spin_unlock_bh(inet_ehash_lockp(hinfo, st->bucket));

	return expected;
}

static struct sock *bpf_iter_tcp_batch(struct seq_file *seq)
{
	struct inet_hashinfo *hinfo = seq_file_net(seq)->ipv4.tcp_death_row.hashinfo;
	struct bpf_tcp_iter_state *iter = seq->private;
	struct tcp_iter_state *st = &iter->state;
	unsigned int expected;
	bool resized = false;
	struct sock *sk;

	/* The st->bucket is done.  Directly advance to the next
	 * bucket instead of having the tcp_seek_last_pos() to skip
	 * one by one in the current bucket and eventually find out
	 * it has to advance to the next bucket.
	 */
	if (iter->st_bucket_done) {
		st->offset = 0;
		st->bucket++;
		if (st->state == TCP_SEQ_STATE_LISTENING &&
		    st->bucket > hinfo->lhash2_mask) {
			st->state = TCP_SEQ_STATE_ESTABLISHED;
			st->bucket = 0;
		}
	}

again:
	/* Get a new batch */
	iter->cur_sk = 0;
	iter->end_sk = 0;
	iter->st_bucket_done = false;

	sk = tcp_seek_last_pos(seq);
	if (!sk)
		return NULL; /* Done */

	if (st->state == TCP_SEQ_STATE_LISTENING)
		expected = bpf_iter_tcp_listening_batch(seq, sk);
	else
		expected = bpf_iter_tcp_established_batch(seq, sk);

	if (iter->end_sk == expected) {
		iter->st_bucket_done = true;
		return sk;
	}

	if (!resized && !bpf_iter_tcp_realloc_batch(iter, expected * 3 / 2)) {
		resized = true;
		goto again;
	}

	return sk;
}

static void *bpf_iter_tcp_seq_start(struct seq_file *seq, loff_t *pos)
{
	/* bpf iter does not support lseek, so it always
	 * continue from where it was stop()-ped.
	 */
	if (*pos)
		return bpf_iter_tcp_batch(seq);

	return SEQ_START_TOKEN;
}

static void *bpf_iter_tcp_seq_next(struct seq_file *seq, void *v, loff_t *pos)
{
	struct bpf_tcp_iter_state *iter = seq->private;
	struct tcp_iter_state *st = &iter->state;
	struct sock *sk;

	/* Whenever seq_next() is called, the iter->cur_sk is
	 * done with seq_show(), so advance to the next sk in
	 * the batch.
	 */
	if (iter->cur_sk < iter->end_sk) {
		/* Keeping st->num consistent in tcp_iter_state.
		 * bpf_iter_tcp does not use st->num.
		 * meta.seq_num is used instead.
		 */
		st->num++;
		/* Move st->offset to the next sk in the bucket such that
		 * the future start() will resume at st->offset in
		 * st->bucket.  See tcp_seek_last_pos().
		 */
		st->offset++;
		sock_gen_put(iter->batch[iter->cur_sk++]);
	}

	if (iter->cur_sk < iter->end_sk)
		sk = iter->batch[iter->cur_sk];
	else
		sk = bpf_iter_tcp_batch(seq);

	++*pos;
	/* Keeping st->last_pos consistent in tcp_iter_state.
	 * bpf iter does not do lseek, so st->last_pos always equals to *pos.
	 */
	st->last_pos = *pos;
	return sk;
}

static int bpf_iter_tcp_seq_show(struct seq_file *seq, void *v)
{
	struct bpf_iter_meta meta;
	struct bpf_prog *prog;
	struct sock *sk = v;
	uid_t uid;
	int ret;

	if (v == SEQ_START_TOKEN)
		return 0;

	if (sk_fullsock(sk))
		lock_sock(sk);

	if (unlikely(sk_unhashed(sk))) {
		ret = SEQ_SKIP;
		goto unlock;
	}

	if (sk->sk_state == TCP_TIME_WAIT) {
		uid = 0;
	} else if (sk->sk_state == TCP_NEW_SYN_RECV) {
		const struct request_sock *req = v;

		uid = from_kuid_munged(seq_user_ns(seq),
				       sock_i_uid(req->rsk_listener));
	} else {
		uid = from_kuid_munged(seq_user_ns(seq), sock_i_uid(sk));
	}

	meta.seq = seq;
	prog = bpf_iter_get_info(&meta, false);
	ret = tcp_prog_seq_show(prog, &meta, v, uid);

unlock:
	if (sk_fullsock(sk))
		release_sock(sk);
	return ret;

}

static void bpf_iter_tcp_seq_stop(struct seq_file *seq, void *v)
{
	struct bpf_tcp_iter_state *iter = seq->private;
	struct bpf_iter_meta meta;
	struct bpf_prog *prog;

	if (!v) {
		meta.seq = seq;
		prog = bpf_iter_get_info(&meta, true);
		if (prog)
			(void)tcp_prog_seq_show(prog, &meta, v, 0);
	}

	if (iter->cur_sk < iter->end_sk) {
		bpf_iter_tcp_put_batch(iter);
		iter->st_bucket_done = false;
	}
}

static const struct seq_operations bpf_iter_tcp_seq_ops = {
	.show		= bpf_iter_tcp_seq_show,
	.start		= bpf_iter_tcp_seq_start,
	.next		= bpf_iter_tcp_seq_next,
	.stop		= bpf_iter_tcp_seq_stop,
};
#endif
static unsigned short seq_file_family(const struct seq_file *seq)
{
	const struct tcp_seq_afinfo *afinfo;

#ifdef CONFIG_BPF_SYSCALL
	/* Iterated from bpf_iter.  Let the bpf prog to filter instead. */
	if (seq->op == &bpf_iter_tcp_seq_ops)
		return AF_UNSPEC;
#endif

	/* Iterated from proc fs */
	afinfo = pde_data(file_inode(seq->file));
	return afinfo->family;
}

static const struct seq_operations tcp4_seq_ops = {
	.show		= tcp4_seq_show,
	.start		= tcp_seq_start,
	.next		= tcp_seq_next,
	.stop		= tcp_seq_stop,
};

static struct tcp_seq_afinfo tcp4_seq_afinfo = {
	.family		= AF_INET,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static int __net_init tcp4_proc_init_net(struct net *net)
{
<<<<<<< HEAD
	return tcp_proc_register(net, &tcp4_seq_afinfo);
=======
	if (!proc_create_net_data("tcp", 0444, net->proc_net, &tcp4_seq_ops,
			sizeof(struct tcp_iter_state), &tcp4_seq_afinfo))
		return -ENOMEM;
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void __net_exit tcp4_proc_exit_net(struct net *net)
{
<<<<<<< HEAD
	tcp_proc_unregister(net, &tcp4_seq_afinfo);
=======
	remove_proc_entry("tcp", net->proc_net);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct pernet_operations tcp4_net_ops = {
	.init = tcp4_proc_init_net,
	.exit = tcp4_proc_exit_net,
};

int __init tcp4_proc_init(void)
{
	return register_pernet_subsys(&tcp4_net_ops);
}

void tcp4_proc_exit(void)
{
	unregister_pernet_subsys(&tcp4_net_ops);
}
#endif /* CONFIG_PROC_FS */

<<<<<<< HEAD
struct sk_buff **tcp4_gro_receive(struct sk_buff **head, struct sk_buff *skb)
{
	const struct iphdr *iph = skb_gro_network_header(skb);

	switch (skb->ip_summed) {
	case CHECKSUM_COMPLETE:
		if (!tcp_v4_check(skb_gro_len(skb), iph->saddr, iph->daddr,
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

int tcp4_gro_complete(struct sk_buff *skb)
{
	const struct iphdr *iph = ip_hdr(skb);
	struct tcphdr *th = tcp_hdr(skb);

	th->check = ~tcp_v4_check(skb->len - skb_transport_offset(skb),
				  iph->saddr, iph->daddr, 0);
	skb_shinfo(skb)->gso_type = SKB_GSO_TCPV4;

	return tcp_gro_complete(skb);
}
=======
/* @wake is one when sk_stream_write_space() calls us.
 * This sends EPOLLOUT only if notsent_bytes is half the limit.
 * This mimics the strategy used in sock_def_write_space().
 */
bool tcp_stream_memory_free(const struct sock *sk, int wake)
{
	const struct tcp_sock *tp = tcp_sk(sk);
	u32 notsent_bytes = READ_ONCE(tp->write_seq) -
			    READ_ONCE(tp->snd_nxt);

	return (notsent_bytes << wake) < tcp_notsent_lowat(tp);
}
EXPORT_SYMBOL(tcp_stream_memory_free);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct proto tcp_prot = {
	.name			= "TCP",
	.owner			= THIS_MODULE,
	.close			= tcp_close,
<<<<<<< HEAD
=======
	.pre_connect		= tcp_v4_pre_connect,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.connect		= tcp_v4_connect,
	.disconnect		= tcp_disconnect,
	.accept			= inet_csk_accept,
	.ioctl			= tcp_ioctl,
	.init			= tcp_v4_init_sock,
	.destroy		= tcp_v4_destroy_sock,
	.shutdown		= tcp_shutdown,
	.setsockopt		= tcp_setsockopt,
	.getsockopt		= tcp_getsockopt,
<<<<<<< HEAD
	.recvmsg		= tcp_recvmsg,
	.sendmsg		= tcp_sendmsg,
	.sendpage		= tcp_sendpage,
	.backlog_rcv		= tcp_v4_do_rcv,
	.hash			= inet_hash,
	.unhash			= inet_unhash,
	.get_port		= inet_csk_get_port,
	.enter_memory_pressure	= tcp_enter_memory_pressure,
	.sockets_allocated	= &tcp_sockets_allocated,
	.orphan_count		= &tcp_orphan_count,
	.memory_allocated	= &tcp_memory_allocated,
	.memory_pressure	= &tcp_memory_pressure,
	.sysctl_wmem		= sysctl_tcp_wmem,
	.sysctl_rmem		= sysctl_tcp_rmem,
	.max_header		= MAX_TCP_HEADER,
	.obj_size		= sizeof(struct tcp_sock),
	.slab_flags		= SLAB_DESTROY_BY_RCU,
	.twsk_prot		= &tcp_timewait_sock_ops,
	.rsk_prot		= &tcp_request_sock_ops,
	.h.hashinfo		= &tcp_hashinfo,
	.no_autobind		= true,
#ifdef CONFIG_COMPAT
	.compat_setsockopt	= compat_tcp_setsockopt,
	.compat_getsockopt	= compat_tcp_getsockopt,
#endif
#ifdef CONFIG_CGROUP_MEM_RES_CTLR_KMEM
	.init_cgroup		= tcp_init_cgroup,
	.destroy_cgroup		= tcp_destroy_cgroup,
	.proto_cgroup		= tcp_proto_cgroup,
#endif
};
EXPORT_SYMBOL(tcp_prot);

static int __net_init tcp_sk_init(struct net *net)
{
	return inet_ctl_sock_create(&net->ipv4.tcp_sock,
				    PF_INET, SOCK_RAW, IPPROTO_TCP, net);
}

static void __net_exit tcp_sk_exit(struct net *net)
{
	inet_ctl_sock_destroy(net->ipv4.tcp_sock);
=======
	.bpf_bypass_getsockopt	= tcp_bpf_bypass_getsockopt,
	.keepalive		= tcp_set_keepalive,
	.recvmsg		= tcp_recvmsg,
	.sendmsg		= tcp_sendmsg,
	.splice_eof		= tcp_splice_eof,
	.backlog_rcv		= tcp_v4_do_rcv,
	.release_cb		= tcp_release_cb,
	.hash			= inet_hash,
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
	.orphan_count		= &tcp_orphan_count,

	.memory_allocated	= &tcp_memory_allocated,
	.per_cpu_fw_alloc	= &tcp_memory_per_cpu_fw_alloc,

	.memory_pressure	= &tcp_memory_pressure,
	.sysctl_mem		= sysctl_tcp_mem,
	.sysctl_wmem_offset	= offsetof(struct net, ipv4.sysctl_tcp_wmem),
	.sysctl_rmem_offset	= offsetof(struct net, ipv4.sysctl_tcp_rmem),
	.max_header		= MAX_TCP_HEADER,
	.obj_size		= sizeof(struct tcp_sock),
	.slab_flags		= SLAB_TYPESAFE_BY_RCU,
	.twsk_prot		= &tcp_timewait_sock_ops,
	.rsk_prot		= &tcp_request_sock_ops,
	.h.hashinfo		= NULL,
	.no_autobind		= true,
	.diag_destroy		= tcp_abort,
};
EXPORT_SYMBOL(tcp_prot);

static void __net_exit tcp_sk_exit(struct net *net)
{
	if (net->ipv4.tcp_congestion_control)
		bpf_module_put(net->ipv4.tcp_congestion_control,
			       net->ipv4.tcp_congestion_control->owner);
}

static void __net_init tcp_set_hashinfo(struct net *net)
{
	struct inet_hashinfo *hinfo;
	unsigned int ehash_entries;
	struct net *old_net;

	if (net_eq(net, &init_net))
		goto fallback;

	old_net = current->nsproxy->net_ns;
	ehash_entries = READ_ONCE(old_net->ipv4.sysctl_tcp_child_ehash_entries);
	if (!ehash_entries)
		goto fallback;

	ehash_entries = roundup_pow_of_two(ehash_entries);
	hinfo = inet_pernet_hashinfo_alloc(&tcp_hashinfo, ehash_entries);
	if (!hinfo) {
		pr_warn("Failed to allocate TCP ehash (entries: %u) "
			"for a netns, fallback to the global one\n",
			ehash_entries);
fallback:
		hinfo = &tcp_hashinfo;
		ehash_entries = tcp_hashinfo.ehash_mask + 1;
	}

	net->ipv4.tcp_death_row.hashinfo = hinfo;
	net->ipv4.tcp_death_row.sysctl_max_tw_buckets = ehash_entries / 2;
	net->ipv4.sysctl_max_syn_backlog = max(128U, ehash_entries / 128);
}

static int __net_init tcp_sk_init(struct net *net)
{
	net->ipv4.sysctl_tcp_ecn = 2;
	net->ipv4.sysctl_tcp_ecn_fallback = 1;

	net->ipv4.sysctl_tcp_base_mss = TCP_BASE_MSS;
	net->ipv4.sysctl_tcp_min_snd_mss = TCP_MIN_SND_MSS;
	net->ipv4.sysctl_tcp_probe_threshold = TCP_PROBE_THRESHOLD;
	net->ipv4.sysctl_tcp_probe_interval = TCP_PROBE_INTERVAL;
	net->ipv4.sysctl_tcp_mtu_probe_floor = TCP_MIN_SND_MSS;

	net->ipv4.sysctl_tcp_keepalive_time = TCP_KEEPALIVE_TIME;
	net->ipv4.sysctl_tcp_keepalive_probes = TCP_KEEPALIVE_PROBES;
	net->ipv4.sysctl_tcp_keepalive_intvl = TCP_KEEPALIVE_INTVL;

	net->ipv4.sysctl_tcp_syn_retries = TCP_SYN_RETRIES;
	net->ipv4.sysctl_tcp_synack_retries = TCP_SYNACK_RETRIES;
	net->ipv4.sysctl_tcp_syncookies = 1;
	net->ipv4.sysctl_tcp_reordering = TCP_FASTRETRANS_THRESH;
	net->ipv4.sysctl_tcp_retries1 = TCP_RETR1;
	net->ipv4.sysctl_tcp_retries2 = TCP_RETR2;
	net->ipv4.sysctl_tcp_orphan_retries = 0;
	net->ipv4.sysctl_tcp_fin_timeout = TCP_FIN_TIMEOUT;
	net->ipv4.sysctl_tcp_notsent_lowat = UINT_MAX;
	net->ipv4.sysctl_tcp_tw_reuse = 2;
	net->ipv4.sysctl_tcp_no_ssthresh_metrics_save = 1;

	refcount_set(&net->ipv4.tcp_death_row.tw_refcount, 1);
	tcp_set_hashinfo(net);

	net->ipv4.sysctl_tcp_sack = 1;
	net->ipv4.sysctl_tcp_window_scaling = 1;
	net->ipv4.sysctl_tcp_timestamps = 1;
	net->ipv4.sysctl_tcp_early_retrans = 3;
	net->ipv4.sysctl_tcp_recovery = TCP_RACK_LOSS_DETECTION;
	net->ipv4.sysctl_tcp_slow_start_after_idle = 1; /* By default, RFC2861 behavior.  */
	net->ipv4.sysctl_tcp_retrans_collapse = 1;
	net->ipv4.sysctl_tcp_max_reordering = 300;
	net->ipv4.sysctl_tcp_dsack = 1;
	net->ipv4.sysctl_tcp_app_win = 31;
	net->ipv4.sysctl_tcp_adv_win_scale = 1;
	net->ipv4.sysctl_tcp_frto = 2;
	net->ipv4.sysctl_tcp_moderate_rcvbuf = 1;
	/* This limits the percentage of the congestion window which we
	 * will allow a single TSO frame to consume.  Building TSO frames
	 * which are too large can cause TCP streams to be bursty.
	 */
	net->ipv4.sysctl_tcp_tso_win_divisor = 3;
	/* Default TSQ limit of 16 TSO segments */
	net->ipv4.sysctl_tcp_limit_output_bytes = 16 * 65536;

	/* rfc5961 challenge ack rate limiting, per net-ns, disabled by default. */
	net->ipv4.sysctl_tcp_challenge_ack_limit = INT_MAX;

	net->ipv4.sysctl_tcp_min_tso_segs = 2;
	net->ipv4.sysctl_tcp_tso_rtt_log = 9;  /* 2^9 = 512 usec */
	net->ipv4.sysctl_tcp_min_rtt_wlen = 300;
	net->ipv4.sysctl_tcp_autocorking = 1;
	net->ipv4.sysctl_tcp_invalid_ratelimit = HZ/2;
	net->ipv4.sysctl_tcp_pacing_ss_ratio = 200;
	net->ipv4.sysctl_tcp_pacing_ca_ratio = 120;
	if (net != &init_net) {
		memcpy(net->ipv4.sysctl_tcp_rmem,
		       init_net.ipv4.sysctl_tcp_rmem,
		       sizeof(init_net.ipv4.sysctl_tcp_rmem));
		memcpy(net->ipv4.sysctl_tcp_wmem,
		       init_net.ipv4.sysctl_tcp_wmem,
		       sizeof(init_net.ipv4.sysctl_tcp_wmem));
	}
	net->ipv4.sysctl_tcp_comp_sack_delay_ns = NSEC_PER_MSEC;
	net->ipv4.sysctl_tcp_comp_sack_slack_ns = 100 * NSEC_PER_USEC;
	net->ipv4.sysctl_tcp_comp_sack_nr = 44;
	net->ipv4.sysctl_tcp_backlog_ack_defer = 1;
	net->ipv4.sysctl_tcp_fastopen = TFO_CLIENT_ENABLE;
	net->ipv4.sysctl_tcp_fastopen_blackhole_timeout = 0;
	atomic_set(&net->ipv4.tfo_active_disable_times, 0);

	/* Set default values for PLB */
	net->ipv4.sysctl_tcp_plb_enabled = 0; /* Disabled by default */
	net->ipv4.sysctl_tcp_plb_idle_rehash_rounds = 3;
	net->ipv4.sysctl_tcp_plb_rehash_rounds = 12;
	net->ipv4.sysctl_tcp_plb_suspend_rto_sec = 60;
	/* Default congestion threshold for PLB to mark a round is 50% */
	net->ipv4.sysctl_tcp_plb_cong_thresh = (1 << TCP_PLB_SCALE) / 2;

	/* Reno is always built in */
	if (!net_eq(net, &init_net) &&
	    bpf_try_module_get(init_net.ipv4.tcp_congestion_control,
			       init_net.ipv4.tcp_congestion_control->owner))
		net->ipv4.tcp_congestion_control = init_net.ipv4.tcp_congestion_control;
	else
		net->ipv4.tcp_congestion_control = &tcp_reno;

	net->ipv4.sysctl_tcp_syn_linear_timeouts = 4;
	net->ipv4.sysctl_tcp_shrink_window = 0;

	net->ipv4.sysctl_tcp_pingpong_thresh = 1;

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void __net_exit tcp_sk_exit_batch(struct list_head *net_exit_list)
{
<<<<<<< HEAD
	inet_twsk_purge(&tcp_hashinfo, &tcp_death_row, AF_INET);
=======
	struct net *net;

	tcp_twsk_purge(net_exit_list, AF_INET);

	list_for_each_entry(net, net_exit_list, exit_list) {
		inet_pernet_hashinfo_free(net->ipv4.tcp_death_row.hashinfo);
		WARN_ON_ONCE(!refcount_dec_and_test(&net->ipv4.tcp_death_row.tw_refcount));
		tcp_fastopen_ctx_destroy(net);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct pernet_operations __net_initdata tcp_sk_ops = {
       .init	   = tcp_sk_init,
       .exit	   = tcp_sk_exit,
       .exit_batch = tcp_sk_exit_batch,
};

<<<<<<< HEAD
void __init tcp_v4_init(void)
{
	inet_hashinfo_init(&tcp_hashinfo);
	if (register_pernet_subsys(&tcp_sk_ops))
		panic("Failed to create the TCP control socket.\n");
=======
#if defined(CONFIG_BPF_SYSCALL) && defined(CONFIG_PROC_FS)
DEFINE_BPF_ITER_FUNC(tcp, struct bpf_iter_meta *meta,
		     struct sock_common *sk_common, uid_t uid)

#define INIT_BATCH_SZ 16

static int bpf_iter_init_tcp(void *priv_data, struct bpf_iter_aux_info *aux)
{
	struct bpf_tcp_iter_state *iter = priv_data;
	int err;

	err = bpf_iter_init_seq_net(priv_data, aux);
	if (err)
		return err;

	err = bpf_iter_tcp_realloc_batch(iter, INIT_BATCH_SZ);
	if (err) {
		bpf_iter_fini_seq_net(priv_data);
		return err;
	}

	return 0;
}

static void bpf_iter_fini_tcp(void *priv_data)
{
	struct bpf_tcp_iter_state *iter = priv_data;

	bpf_iter_fini_seq_net(priv_data);
	kvfree(iter->batch);
}

static const struct bpf_iter_seq_info tcp_seq_info = {
	.seq_ops		= &bpf_iter_tcp_seq_ops,
	.init_seq_private	= bpf_iter_init_tcp,
	.fini_seq_private	= bpf_iter_fini_tcp,
	.seq_priv_size		= sizeof(struct bpf_tcp_iter_state),
};

static const struct bpf_func_proto *
bpf_iter_tcp_get_func_proto(enum bpf_func_id func_id,
			    const struct bpf_prog *prog)
{
	switch (func_id) {
	case BPF_FUNC_setsockopt:
		return &bpf_sk_setsockopt_proto;
	case BPF_FUNC_getsockopt:
		return &bpf_sk_getsockopt_proto;
	default:
		return NULL;
	}
}

static struct bpf_iter_reg tcp_reg_info = {
	.target			= "tcp",
	.ctx_arg_info_size	= 1,
	.ctx_arg_info		= {
		{ offsetof(struct bpf_iter__tcp, sk_common),
		  PTR_TO_BTF_ID_OR_NULL | PTR_TRUSTED },
	},
	.get_func_proto		= bpf_iter_tcp_get_func_proto,
	.seq_info		= &tcp_seq_info,
};

static void __init bpf_iter_register(void)
{
	tcp_reg_info.ctx_arg_info[0].btf_id = btf_sock_ids[BTF_SOCK_TYPE_SOCK_COMMON];
	if (bpf_iter_reg_target(&tcp_reg_info))
		pr_warn("Warning: could not register bpf iterator tcp\n");
}

#endif

void __init tcp_v4_init(void)
{
	int cpu, res;

	for_each_possible_cpu(cpu) {
		struct sock *sk;

		res = inet_ctl_sock_create(&sk, PF_INET, SOCK_RAW,
					   IPPROTO_TCP, &init_net);
		if (res)
			panic("Failed to create the TCP control socket.\n");
		sock_set_flag(sk, SOCK_USE_WRITE_QUEUE);

		/* Please enforce IP_DF and IPID==0 for RST and
		 * ACK sent in SYN-RECV and TIME-WAIT state.
		 */
		inet_sk(sk)->pmtudisc = IP_PMTUDISC_DO;

		per_cpu(ipv4_tcp_sk, cpu) = sk;
	}
	if (register_pernet_subsys(&tcp_sk_ops))
		panic("Failed to create the TCP control socket.\n");

#if defined(CONFIG_BPF_SYSCALL) && defined(CONFIG_PROC_FS)
	bpf_iter_register();
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
