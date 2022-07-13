<<<<<<< HEAD
#ifndef _IPV6_H
#define _IPV6_H

#include <linux/types.h>
#include <linux/in6.h>
#include <asm/byteorder.h>

/* The latest drafts declared increase in minimal mtu up to 1280. */

#define IPV6_MIN_MTU	1280

/*
 *	Advanced API
 *	source interface/address selection, source routing, etc...
 *	*under construction*
 */


struct in6_pktinfo {
	struct in6_addr	ipi6_addr;
	int		ipi6_ifindex;
};

struct ip6_mtuinfo {
	struct sockaddr_in6	ip6m_addr;
	__u32			ip6m_mtu;
};

struct in6_ifreq {
	struct in6_addr	ifr6_addr;
	__u32		ifr6_prefixlen;
	int		ifr6_ifindex; 
};

#define IPV6_SRCRT_STRICT	0x01	/* Deprecated; will be removed */
#define IPV6_SRCRT_TYPE_0	0	/* Deprecated; will be removed */
#define IPV6_SRCRT_TYPE_2	2	/* IPv6 type 2 Routing Header	*/

/*
 *	routing header
 */
struct ipv6_rt_hdr {
	__u8		nexthdr;
	__u8		hdrlen;
	__u8		type;
	__u8		segments_left;

	/*
	 *	type specific data
	 *	variable length field
	 */
};


struct ipv6_opt_hdr {
	__u8 		nexthdr;
	__u8 		hdrlen;
	/* 
	 * TLV encoded option data follows.
	 */
} __attribute__((packed));	/* required for some archs */

#define ipv6_destopt_hdr ipv6_opt_hdr
#define ipv6_hopopt_hdr  ipv6_opt_hdr

#ifdef __KERNEL__
#define ipv6_optlen(p)  (((p)->hdrlen+1) << 3)
#endif

/*
 *	routing header type 0 (used in cmsghdr struct)
 */

struct rt0_hdr {
	struct ipv6_rt_hdr	rt_hdr;
	__u32			reserved;
	struct in6_addr		addr[0];

#define rt0_type		rt_hdr.type
};

/*
 *	routing header type 2
 */

struct rt2_hdr {
	struct ipv6_rt_hdr	rt_hdr;
	__u32			reserved;
	struct in6_addr		addr;

#define rt2_type		rt_hdr.type
};

/*
 *	home address option in destination options header
 */

struct ipv6_destopt_hao {
	__u8			type;
	__u8			length;
	struct in6_addr		addr;
} __attribute__((packed));

/*
 *	IPv6 fixed header
 *
 *	BEWARE, it is incorrect. The first 4 bits of flow_lbl
 *	are glued to priority now, forming "class".
 */

struct ipv6hdr {
#if defined(__LITTLE_ENDIAN_BITFIELD)
	__u8			priority:4,
				version:4;
#elif defined(__BIG_ENDIAN_BITFIELD)
	__u8			version:4,
				priority:4;
#else
#error	"Please fix <asm/byteorder.h>"
#endif
	__u8			flow_lbl[3];

	__be16			payload_len;
	__u8			nexthdr;
	__u8			hop_limit;

	struct	in6_addr	saddr;
	struct	in6_addr	daddr;
};

#ifdef __KERNEL__
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _IPV6_H
#define _IPV6_H

#include <uapi/linux/ipv6.h>
#include <linux/cache.h>

#define ipv6_optlen(p)  (((p)->hdrlen+1) << 3)
#define ipv6_authlen(p) (((p)->hdrlen+2) << 2)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * This structure contains configuration options per IPv6 link.
 */
struct ipv6_devconf {
<<<<<<< HEAD
	__s32		forwarding;
	__s32		hop_limit;
	__s32		mtu6;
=======
	/* RX & TX fastpath fields. */
	__cacheline_group_begin(ipv6_devconf_read_txrx);
	__s32		disable_ipv6;
	__s32		hop_limit;
	__s32		mtu6;
	__s32		forwarding;
	__s32		disable_policy;
	__s32		proxy_ndp;
	__cacheline_group_end(ipv6_devconf_read_txrx);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	__s32		accept_ra;
	__s32		accept_redirects;
	__s32		autoconf;
	__s32		dad_transmits;
	__s32		rtr_solicits;
	__s32		rtr_solicit_interval;
<<<<<<< HEAD
	__s32		rtr_solicit_delay;
	__s32		force_mld_version;
#ifdef CONFIG_IPV6_PRIVACY
	__s32		use_tempaddr;
	__s32		temp_valid_lft;
	__s32		temp_prefered_lft;
	__s32		regen_max_retry;
	__s32		max_desync_factor;
#endif
	__s32		max_addresses;
	__s32		accept_ra_defrtr;
	__s32		accept_ra_pinfo;
#ifdef CONFIG_LGE_DHCPV6_WIFI
	__s32		ra_info_flag;
#endif	
=======
	__s32		rtr_solicit_max_interval;
	__s32		rtr_solicit_delay;
	__s32		force_mld_version;
	__s32		mldv1_unsolicited_report_interval;
	__s32		mldv2_unsolicited_report_interval;
	__s32		use_tempaddr;
	__s32		temp_valid_lft;
	__s32		temp_prefered_lft;
	__s32		regen_min_advance;
	__s32		regen_max_retry;
	__s32		max_desync_factor;
	__s32		max_addresses;
	__s32		accept_ra_defrtr;
	__u32		ra_defrtr_metric;
	__s32		accept_ra_min_hop_limit;
	__s32		accept_ra_min_lft;
	__s32		accept_ra_pinfo;
	__s32		ignore_routes_with_linkdown;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_IPV6_ROUTER_PREF
	__s32		accept_ra_rtr_pref;
	__s32		rtr_probe_interval;
#ifdef CONFIG_IPV6_ROUTE_INFO
<<<<<<< HEAD
	__s32		accept_ra_rt_info_max_plen;
#endif
#endif
	__s32		accept_ra_rt_table;
	__s32		proxy_ndp;
	__s32		accept_source_route;
=======
	__s32		accept_ra_rt_info_min_plen;
	__s32		accept_ra_rt_info_max_plen;
#endif
#endif
	__s32		accept_source_route;
	__s32		accept_ra_from_local;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_IPV6_OPTIMISTIC_DAD
	__s32		optimistic_dad;
	__s32		use_optimistic;
#endif
#ifdef CONFIG_IPV6_MROUTE
<<<<<<< HEAD
	__s32		mc_forwarding;
#endif
	__s32		disable_ipv6;
	__s32		accept_dad;
	__s32		force_tllao;
	__s32		accept_ra_prefix_route;
	__s32		accept_ra_mtu;
	void		*sysctl;
=======
	atomic_t	mc_forwarding;
#endif
	__s32		drop_unicast_in_l2_multicast;
	__s32		accept_dad;
	__s32		force_tllao;
	__s32           ndisc_notify;
	__s32		suppress_frag_ndisc;
	__s32		accept_ra_mtu;
	__s32		drop_unsolicited_na;
	__s32		accept_untracked_na;
	struct ipv6_stable_secret {
		bool initialized;
		struct in6_addr secret;
	} stable_secret;
	__s32		use_oif_addrs_only;
	__s32		keep_addr_on_down;
	__s32		seg6_enabled;
#ifdef CONFIG_IPV6_SEG6_HMAC
	__s32		seg6_require_hmac;
#endif
	__u32		enhanced_dad;
	__u32		addr_gen_mode;
	__s32           ndisc_tclass;
	__s32		rpl_seg_enabled;
	__u32		ioam6_id;
	__u32		ioam6_id_wide;
	__u8		ioam6_enabled;
	__u8		ndisc_evict_nocarrier;
	__u8		ra_honor_pio_life;

	struct ctl_table_header *sysctl_header;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct ipv6_params {
	__s32 disable_ipv6;
	__s32 autoconf;
};
extern struct ipv6_params ipv6_defaults;
<<<<<<< HEAD
#endif

/* index values for the variables in ipv6_devconf */
enum {
	DEVCONF_FORWARDING = 0,
	DEVCONF_HOPLIMIT,
	DEVCONF_MTU6,
	DEVCONF_ACCEPT_RA,
	DEVCONF_ACCEPT_REDIRECTS,
	DEVCONF_AUTOCONF,
	DEVCONF_DAD_TRANSMITS,
	DEVCONF_RTR_SOLICITS,
	DEVCONF_RTR_SOLICIT_INTERVAL,
	DEVCONF_RTR_SOLICIT_DELAY,
	DEVCONF_USE_TEMPADDR,
	DEVCONF_TEMP_VALID_LFT,
	DEVCONF_TEMP_PREFERED_LFT,
	DEVCONF_REGEN_MAX_RETRY,
	DEVCONF_MAX_DESYNC_FACTOR,
	DEVCONF_MAX_ADDRESSES,
	DEVCONF_FORCE_MLD_VERSION,
	DEVCONF_ACCEPT_RA_DEFRTR,
	DEVCONF_ACCEPT_RA_PINFO,
	DEVCONF_ACCEPT_RA_RTR_PREF,
	DEVCONF_RTR_PROBE_INTERVAL,
	DEVCONF_ACCEPT_RA_RT_INFO_MAX_PLEN,
	DEVCONF_PROXY_NDP,
	DEVCONF_OPTIMISTIC_DAD,
	DEVCONF_ACCEPT_SOURCE_ROUTE,
	DEVCONF_MC_FORWARDING,
	DEVCONF_DISABLE_IPV6,
	DEVCONF_ACCEPT_DAD,
	DEVCONF_FORCE_TLLAO,
	DEVCONF_ACCEPT_RA_PREFIX_ROUTE,
	DEVCONF_ACCEPT_RA_RT_TABLE,
#ifdef CONFIG_LGE_DHCPV6_WIFI
	DEVCONF_RA_INFO_FLAG,
#endif
	DEVCONF_USE_OPTIMISTIC,
	DEVCONF_ACCEPT_RA_MTU,
	DEVCONF_MAX
};

#ifdef __KERNEL__
#include <linux/icmpv6.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/tcp.h>
#include <linux/udp.h>

#include <net/inet_sock.h>

static inline struct ipv6hdr *ipv6_hdr(const struct sk_buff *skb)
{
	return (struct ipv6hdr *)skb_network_header(skb);
}

<<<<<<< HEAD
=======
static inline struct ipv6hdr *inner_ipv6_hdr(const struct sk_buff *skb)
{
	return (struct ipv6hdr *)skb_inner_network_header(skb);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline struct ipv6hdr *ipipv6_hdr(const struct sk_buff *skb)
{
	return (struct ipv6hdr *)skb_transport_header(skb);
}

<<<<<<< HEAD
static inline __u8 ipv6_tclass(const struct ipv6hdr *iph)
{
	return (ntohl(*(__be32 *)iph) >> 20) & 0xff;
=======
static inline unsigned int ipv6_transport_len(const struct sk_buff *skb)
{
	return ntohs(ipv6_hdr(skb)->payload_len) + sizeof(struct ipv6hdr) -
	       skb_network_header_len(skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* 
   This structure contains results of exthdrs parsing
   as offsets from skb->nh.
 */

struct inet6_skb_parm {
	int			iif;
<<<<<<< HEAD
	__u16			ra;
	__u16			hop;
=======
	__be16			ra;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	__u16			dst0;
	__u16			srcrt;
	__u16			dst1;
	__u16			lastopt;
	__u16			nhoff;
	__u16			flags;
#if defined(CONFIG_IPV6_MIP6) || defined(CONFIG_IPV6_MIP6_MODULE)
	__u16			dsthao;
#endif
	__u16			frag_max_size;
<<<<<<< HEAD
=======
	__u16			srhoff;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define IP6SKB_XFRM_TRANSFORMED	1
#define IP6SKB_FORWARDED	2
#define IP6SKB_REROUTED		4
<<<<<<< HEAD
#define IP6SKB_FRAGMENTED      16
};

=======
#define IP6SKB_ROUTERALERT	8
#define IP6SKB_FRAGMENTED      16
#define IP6SKB_HOPBYHOP        32
#define IP6SKB_L3SLAVE         64
#define IP6SKB_JUMBOGRAM      128
#define IP6SKB_SEG6	      256
#define IP6SKB_FAKEJUMBO      512
#define IP6SKB_MULTIPATH      1024
};

#if defined(CONFIG_NET_L3_MASTER_DEV)
static inline bool ipv6_l3mdev_skb(__u16 flags)
{
	return flags & IP6SKB_L3SLAVE;
}
#else
static inline bool ipv6_l3mdev_skb(__u16 flags)
{
	return false;
}
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define IP6CB(skb)	((struct inet6_skb_parm*)((skb)->cb))
#define IP6CBMTU(skb)	((struct ip6_mtuinfo *)((skb)->cb))

static inline int inet6_iif(const struct sk_buff *skb)
{
<<<<<<< HEAD
	return IP6CB(skb)->iif;
}

struct inet6_request_sock {
	struct in6_addr		loc_addr;
	struct in6_addr		rmt_addr;
	struct sk_buff		*pktopts;
	int			iif;
};

struct tcp6_request_sock {
	struct tcp_request_sock	  tcp6rsk_tcp;
	struct inet6_request_sock tcp6rsk_inet6;
=======
	bool l3_slave = ipv6_l3mdev_skb(IP6CB(skb)->flags);

	return l3_slave ? skb->skb_iif : IP6CB(skb)->iif;
}

static inline bool inet6_is_jumbogram(const struct sk_buff *skb)
{
	return !!(IP6CB(skb)->flags & IP6SKB_JUMBOGRAM);
}

/* can not be used in TCP layer after tcp_v6_fill_cb */
static inline int inet6_sdif(const struct sk_buff *skb)
{
#if IS_ENABLED(CONFIG_NET_L3_MASTER_DEV)
	if (skb && ipv6_l3mdev_skb(IP6CB(skb)->flags))
		return IP6CB(skb)->iif;
#endif
	return 0;
}

struct tcp6_request_sock {
	struct tcp_request_sock	  tcp6rsk_tcp;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct ipv6_mc_socklist;
struct ipv6_ac_socklist;
struct ipv6_fl_socklist;

<<<<<<< HEAD
/**
 * struct ipv6_pinfo - ipv6 private area
 *
 * In the struct sock hierarchy (tcp6_sock, upd6_sock, etc)
 * this _must_ be the last member, so that inet6_sk_generic
 * is able to calculate its offset from the base struct sock
 * by using the struct proto->slab_obj_size member. -acme
 */
struct ipv6_pinfo {
	struct in6_addr 	saddr;
	struct in6_addr 	rcv_saddr;
	struct in6_addr		daddr;
	struct in6_pktinfo	sticky_pktinfo;
	struct in6_addr		*daddr_cache;
#ifdef CONFIG_IPV6_SUBTREES
	struct in6_addr		*saddr_cache;
=======
struct inet6_cork {
	struct ipv6_txoptions *opt;
	u8 hop_limit;
	u8 tclass;
};

/* struct ipv6_pinfo - ipv6 private area */
struct ipv6_pinfo {
	struct in6_addr 	saddr;
	struct in6_pktinfo	sticky_pktinfo;
	const struct in6_addr		*daddr_cache;
#ifdef CONFIG_IPV6_SUBTREES
	const struct in6_addr		*saddr_cache;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

	__be32			flow_label;
	__u32			frag_size;

<<<<<<< HEAD
	/*
	 * Packed in 16bits.
	 * Omit one shift by by putting the signed field at MSB.
	 */
#if defined(__BIG_ENDIAN_BITFIELD)
	__s16			hop_limit:9;
	__u16			__unused_1:7;
#else
	__u16			__unused_1:7;
	__s16			hop_limit:9;
#endif

#if defined(__BIG_ENDIAN_BITFIELD)
	/* Packed in 16bits. */
	__s16			mcast_hops:9;
	__u16			__unused_2:6,
				mc_loop:1;
#else
	__u16			mc_loop:1,
				__unused_2:6;
	__s16			mcast_hops:9;
#endif
=======
	s16			hop_limit;
	u8			mcast_hops;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int			ucast_oif;
	int			mcast_oif;

	/* pktoption flags */
	union {
		struct {
			__u16	srcrt:1,
				osrcrt:1,
			        rxinfo:1,
			        rxoinfo:1,
				rxhlim:1,
				rxohlim:1,
				hopopts:1,
				ohopopts:1,
				dstopts:1,
				odstopts:1,
                                rxflow:1,
				rxtclass:1,
				rxpmtu:1,
<<<<<<< HEAD
				rxorigdstaddr:1;
				/* 2 bits hole */
=======
				rxorigdstaddr:1,
				recvfragsize:1;
				/* 1 bits hole */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		} bits;
		__u16		all;
	} rxopt;

	/* sockopt flags */
<<<<<<< HEAD
	__u16			recverr:1,
	                        sndflow:1,
				pmtudisc:2,
				ipv6only:1,
				srcprefs:3,	/* 001: prefer temporary address
						 * 010: prefer public address
						 * 100: prefer care-of address
						 */
				dontfrag:1;
	__u8			min_hopcount;
	__u8			tclass;
	__u8			rcv_tclass;
=======
	__u8			srcprefs;	/* 001: prefer temporary address
						 * 010: prefer public address
						 * 100: prefer care-of address
						 */
	__u8			pmtudisc;
	__u8			min_hopcount;
	__u8			tclass;
	__be32			rcv_flowinfo;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	__u32			dst_cookie;

	struct ipv6_mc_socklist	__rcu *ipv6_mc_list;
	struct ipv6_ac_socklist	*ipv6_ac_list;
<<<<<<< HEAD
	struct ipv6_fl_socklist *ipv6_fl_list;

	struct ipv6_txoptions	*opt;
	struct sk_buff		*pktoptions;
	struct sk_buff		*rxpmtu;
	struct {
		struct ipv6_txoptions *opt;
		u8 hop_limit;
		u8 tclass;
	} cork;
};

=======
	struct ipv6_fl_socklist __rcu *ipv6_fl_list;

	struct ipv6_txoptions __rcu	*opt;
	struct sk_buff		*pktoptions;
	struct sk_buff		*rxpmtu;
	struct inet6_cork	cork;
};

/* We currently use available bits from inet_sk(sk)->inet_flags,
 * this could change in the future.
 */
#define inet6_test_bit(nr, sk)			\
	test_bit(INET_FLAGS_##nr, &inet_sk(sk)->inet_flags)
#define inet6_set_bit(nr, sk)			\
	set_bit(INET_FLAGS_##nr, &inet_sk(sk)->inet_flags)
#define inet6_clear_bit(nr, sk)			\
	clear_bit(INET_FLAGS_##nr, &inet_sk(sk)->inet_flags)
#define inet6_assign_bit(nr, sk, val)		\
	assign_bit(INET_FLAGS_##nr, &inet_sk(sk)->inet_flags, val)

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* WARNING: don't change the layout of the members in {raw,udp,tcp}6_sock! */
struct raw6_sock {
	/* inet_sock has to be the first member of raw6_sock */
	struct inet_sock	inet;
	__u32			checksum;	/* perform checksum */
	__u32			offset;		/* checksum offset  */
	struct icmp6_filter	filter;
	__u32			ip6mr_table;
<<<<<<< HEAD
	/* ipv6_pinfo has to be the last member of raw6_sock, see inet6_sk_generic */
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct ipv6_pinfo	inet6;
};

struct udp6_sock {
	struct udp_sock	  udp;
<<<<<<< HEAD
	/* ipv6_pinfo has to be the last member of udp6_sock, see inet6_sk_generic */
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct ipv6_pinfo inet6;
};

struct tcp6_sock {
	struct tcp_sock	  tcp;
<<<<<<< HEAD
	/* ipv6_pinfo has to be the last member of tcp6_sock, see inet6_sk_generic */
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct ipv6_pinfo inet6;
};

extern int inet6_sk_rebuild_header(struct sock *sk);

<<<<<<< HEAD
#if IS_ENABLED(CONFIG_IPV6)
static inline struct ipv6_pinfo * inet6_sk(const struct sock *__sk)
{
	return inet_sk(__sk)->pinet6;
}

static inline struct inet6_request_sock *
			inet6_rsk(const struct request_sock *rsk)
{
	return (struct inet6_request_sock *)(((u8 *)rsk) +
					     inet_rsk(rsk)->inet6_rsk_offset);
}

static inline u32 inet6_rsk_offset(struct request_sock *rsk)
{
	return rsk->rsk_ops->obj_size - sizeof(struct inet6_request_sock);
}

static inline struct request_sock *inet6_reqsk_alloc(struct request_sock_ops *ops)
{
	struct request_sock *req = reqsk_alloc(ops);

	if (req != NULL) {
		inet_rsk(req)->inet6_rsk_offset = inet6_rsk_offset(req);
		inet6_rsk(req)->pktopts = NULL;
	}

	return req;
}

static inline struct raw6_sock *raw6_sk(const struct sock *sk)
{
	return (struct raw6_sock *)sk;
}

static inline void inet_sk_copy_descendant(struct sock *sk_to,
					   const struct sock *sk_from)
{
	int ancestor_size = sizeof(struct inet_sock);

	if (sk_from->sk_family == PF_INET6)
		ancestor_size += sizeof(struct ipv6_pinfo);

	__inet_sk_copy_descendant(sk_to, sk_from, ancestor_size);
}

#define __ipv6_only_sock(sk)	(inet6_sk(sk)->ipv6only)
#define ipv6_only_sock(sk)	((sk)->sk_family == PF_INET6 && __ipv6_only_sock(sk))

struct inet6_timewait_sock {
	struct in6_addr tw_v6_daddr;
	struct in6_addr	tw_v6_rcv_saddr;
};

struct tcp6_timewait_sock {
	struct tcp_timewait_sock   tcp6tw_tcp;
	struct inet6_timewait_sock tcp6tw_inet6;
};

static inline u16 inet6_tw_offset(const struct proto *prot)
{
	return prot->twsk_prot->twsk_obj_size -
			sizeof(struct inet6_timewait_sock);
}

static inline struct inet6_timewait_sock *inet6_twsk(const struct sock *sk)
{
	return (struct inet6_timewait_sock *)(((u8 *)sk) +
					      inet_twsk(sk)->tw_ipv6_offset);
}

static inline struct in6_addr *__inet6_rcv_saddr(const struct sock *sk)
{
	return likely(sk->sk_state != TCP_TIME_WAIT) ?
		&inet6_sk(sk)->rcv_saddr : &inet6_twsk(sk)->tw_v6_rcv_saddr;
}

static inline struct in6_addr *inet6_rcv_saddr(const struct sock *sk)
{
	return sk->sk_family == AF_INET6 ? __inet6_rcv_saddr(sk) : NULL;
=======
struct tcp6_timewait_sock {
	struct tcp_timewait_sock   tcp6tw_tcp;
};

#if IS_ENABLED(CONFIG_IPV6)
bool ipv6_mod_enabled(void);

static inline struct ipv6_pinfo *inet6_sk(const struct sock *__sk)
{
	return sk_fullsock(__sk) ? inet_sk(__sk)->pinet6 : NULL;
}

#define raw6_sk(ptr) container_of_const(ptr, struct raw6_sock, inet.sk)

#define ipv6_only_sock(sk)	(sk->sk_ipv6only)
#define ipv6_sk_rxinfo(sk)	((sk)->sk_family == PF_INET6 && \
				 inet6_sk(sk)->rxopt.bits.rxinfo)

static inline const struct in6_addr *inet6_rcv_saddr(const struct sock *sk)
{
	if (sk->sk_family == AF_INET6)
		return &sk->sk_v6_rcv_saddr;
	return NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline int inet_v6_ipv6only(const struct sock *sk)
{
<<<<<<< HEAD
	return likely(sk->sk_state != TCP_TIME_WAIT) ?
		ipv6_only_sock(sk) : inet_twsk(sk)->tw_ipv6only;
}
#else
#define __ipv6_only_sock(sk)	0
#define ipv6_only_sock(sk)	0

static inline struct ipv6_pinfo * inet6_sk(const struct sock *__sk)
{
	return NULL;
}

static inline struct inet6_request_sock *
			inet6_rsk(const struct request_sock *rsk)
=======
	/* ipv6only field is at same position for timewait and other sockets */
	return ipv6_only_sock(sk);
}
#else
#define ipv6_only_sock(sk)	0
#define ipv6_sk_rxinfo(sk)	0

static inline bool ipv6_mod_enabled(void)
{
	return false;
}

static inline struct ipv6_pinfo * inet6_sk(const struct sock *__sk)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return NULL;
}

static inline struct raw6_sock *raw6_sk(const struct sock *sk)
{
	return NULL;
}

<<<<<<< HEAD
#define __inet6_rcv_saddr(__sk)	NULL
#define inet6_rcv_saddr(__sk)	NULL
#define tcp_twsk_ipv6only(__sk)		0
#define inet_v6_ipv6only(__sk)		0
#endif /* IS_ENABLED(CONFIG_IPV6) */

#define INET6_MATCH(__sk, __net, __hash, __saddr, __daddr, __ports, __dif)\
	(((__sk)->sk_hash == (__hash)) && sock_net((__sk)) == (__net)	&& \
	 ((*((__portpair *)&(inet_sk(__sk)->inet_dport))) == (__ports)) && \
	 ((__sk)->sk_family		== AF_INET6)		&& \
	 ipv6_addr_equal(&inet6_sk(__sk)->daddr, (__saddr))	&& \
	 ipv6_addr_equal(&inet6_sk(__sk)->rcv_saddr, (__daddr))	&& \
	 (!((__sk)->sk_bound_dev_if) || ((__sk)->sk_bound_dev_if == (__dif))))

#define INET6_TW_MATCH(__sk, __net, __hash, __saddr, __daddr, __ports, __dif) \
	(((__sk)->sk_hash == (__hash)) && sock_net((__sk)) == (__net)	&& \
	 (*((__portpair *)&(inet_twsk(__sk)->tw_dport)) == (__ports))	&& \
	 ((__sk)->sk_family	       == PF_INET6)			&& \
	 (ipv6_addr_equal(&inet6_twsk(__sk)->tw_v6_daddr, (__saddr)))	&& \
	 (ipv6_addr_equal(&inet6_twsk(__sk)->tw_v6_rcv_saddr, (__daddr))) && \
	 (!((__sk)->sk_bound_dev_if) || ((__sk)->sk_bound_dev_if == (__dif))))

#endif /* __KERNEL__ */

=======
#define inet6_rcv_saddr(__sk)	NULL
#define inet_v6_ipv6only(__sk)		0
#endif /* IS_ENABLED(CONFIG_IPV6) */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _IPV6_H */
