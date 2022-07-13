<<<<<<< HEAD
#ifndef _LINUX_ICMPV6_H
#define _LINUX_ICMPV6_H

#include <linux/types.h>
#include <asm/byteorder.h>

struct icmp6hdr {

	__u8		icmp6_type;
	__u8		icmp6_code;
	__sum16		icmp6_cksum;


	union {
		__be32			un_data32[1];
		__be16			un_data16[2];
		__u8			un_data8[4];

		struct icmpv6_echo {
			__be16		identifier;
			__be16		sequence;
		} u_echo;

                struct icmpv6_nd_advt {
#if defined(__LITTLE_ENDIAN_BITFIELD)
                        __u32		reserved:5,
                        		override:1,
                        		solicited:1,
                        		router:1,
					reserved2:24;
#elif defined(__BIG_ENDIAN_BITFIELD)
                        __u32		router:1,
					solicited:1,
                        		override:1,
                        		reserved:29;
#else
#error	"Please fix <asm/byteorder.h>"
#endif						
                } u_nd_advt;

                struct icmpv6_nd_ra {
			__u8		hop_limit;
#if defined(__LITTLE_ENDIAN_BITFIELD)
			__u8		reserved:3,
					router_pref:2,
					home_agent:1,
					other:1,
					managed:1;

#elif defined(__BIG_ENDIAN_BITFIELD)
			__u8		managed:1,
					other:1,
					home_agent:1,
					router_pref:2,
					reserved:3;
#else
#error	"Please fix <asm/byteorder.h>"
#endif
			__be16		rt_lifetime;
                } u_nd_ra;

	} icmp6_dataun;

#define icmp6_identifier	icmp6_dataun.u_echo.identifier
#define icmp6_sequence		icmp6_dataun.u_echo.sequence
#define icmp6_pointer		icmp6_dataun.un_data32[0]
#define icmp6_mtu		icmp6_dataun.un_data32[0]
#define icmp6_unused		icmp6_dataun.un_data32[0]
#define icmp6_maxdelay		icmp6_dataun.un_data16[0]
#define icmp6_router		icmp6_dataun.u_nd_advt.router
#define icmp6_solicited		icmp6_dataun.u_nd_advt.solicited
#define icmp6_override		icmp6_dataun.u_nd_advt.override
#define icmp6_ndiscreserved	icmp6_dataun.u_nd_advt.reserved
#define icmp6_hop_limit		icmp6_dataun.u_nd_ra.hop_limit
#define icmp6_addrconf_managed	icmp6_dataun.u_nd_ra.managed
#define icmp6_addrconf_other	icmp6_dataun.u_nd_ra.other
#define icmp6_rt_lifetime	icmp6_dataun.u_nd_ra.rt_lifetime
#define icmp6_router_pref	icmp6_dataun.u_nd_ra.router_pref
};

#ifdef __KERNEL__
#include <linux/skbuff.h>
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_ICMPV6_H
#define _LINUX_ICMPV6_H

#include <linux/skbuff.h>
#include <linux/ipv6.h>
#include <uapi/linux/icmpv6.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline struct icmp6hdr *icmp6_hdr(const struct sk_buff *skb)
{
	return (struct icmp6hdr *)skb_transport_header(skb);
}
<<<<<<< HEAD
#endif

#define ICMPV6_ROUTER_PREF_LOW		0x3
#define ICMPV6_ROUTER_PREF_MEDIUM	0x0
#define ICMPV6_ROUTER_PREF_HIGH		0x1
#define ICMPV6_ROUTER_PREF_INVALID	0x2

#define ICMPV6_DEST_UNREACH		1
#define ICMPV6_PKT_TOOBIG		2
#define ICMPV6_TIME_EXCEED		3
#define ICMPV6_PARAMPROB		4

#define ICMPV6_INFOMSG_MASK		0x80

#define ICMPV6_ECHO_REQUEST		128
#define ICMPV6_ECHO_REPLY		129
#define ICMPV6_MGM_QUERY		130
#define ICMPV6_MGM_REPORT       	131
#define ICMPV6_MGM_REDUCTION    	132

#define ICMPV6_NI_QUERY			139
#define ICMPV6_NI_REPLY			140

#define ICMPV6_MLD2_REPORT		143

#define ICMPV6_DHAAD_REQUEST		144
#define ICMPV6_DHAAD_REPLY		145
#define ICMPV6_MOBILE_PREFIX_SOL	146
#define ICMPV6_MOBILE_PREFIX_ADV	147

/*
 *	Codes for Destination Unreachable
 */
#define ICMPV6_NOROUTE			0
#define ICMPV6_ADM_PROHIBITED		1
#define ICMPV6_NOT_NEIGHBOUR		2
#define ICMPV6_ADDR_UNREACH		3
#define ICMPV6_PORT_UNREACH		4
#define ICMPV6_POLICY_FAIL		5
#define ICMPV6_REJECT_ROUTE		6

/*
 *	Codes for Time Exceeded
 */
#define ICMPV6_EXC_HOPLIMIT		0
#define ICMPV6_EXC_FRAGTIME		1

/*
 *	Codes for Parameter Problem
 */
#define ICMPV6_HDR_FIELD		0
#define ICMPV6_UNK_NEXTHDR		1
#define ICMPV6_UNK_OPTION		2

/*
 *	constants for (set|get)sockopt
 */

#define ICMPV6_FILTER			1

/*
 *	ICMPV6 filter
 */

#define ICMPV6_FILTER_BLOCK		1
#define ICMPV6_FILTER_PASS		2
#define ICMPV6_FILTER_BLOCKOTHERS	3
#define ICMPV6_FILTER_PASSONLY		4

struct icmp6_filter {
	__u32		data[8];
};

/*
 *	Definitions for MLDv2
 */
#define MLD2_MODE_IS_INCLUDE	1
#define MLD2_MODE_IS_EXCLUDE	2
#define MLD2_CHANGE_TO_INCLUDE	3
#define MLD2_CHANGE_TO_EXCLUDE	4
#define MLD2_ALLOW_NEW_SOURCES	5
#define MLD2_BLOCK_OLD_SOURCES	6

#define MLD2_ALL_MCR_INIT { { { 0xff,0x02,0,0,0,0,0,0,0,0,0,0,0,0,0,0x16 } } }

#ifdef __KERNEL__

#include <linux/netdevice.h>

extern void				icmpv6_send(struct sk_buff *skb,
						    u8 type, u8 code,
						    __u32 info);
=======

#include <linux/netdevice.h>

#if IS_ENABLED(CONFIG_IPV6)

typedef void ip6_icmp_send_t(struct sk_buff *skb, u8 type, u8 code, __u32 info,
			     const struct in6_addr *force_saddr,
			     const struct inet6_skb_parm *parm);
void icmp6_send(struct sk_buff *skb, u8 type, u8 code, __u32 info,
		const struct in6_addr *force_saddr,
		const struct inet6_skb_parm *parm);
#if IS_BUILTIN(CONFIG_IPV6)
static inline void __icmpv6_send(struct sk_buff *skb, u8 type, u8 code, __u32 info,
				 const struct inet6_skb_parm *parm)
{
	icmp6_send(skb, type, code, info, NULL, parm);
}
static inline int inet6_register_icmp_sender(ip6_icmp_send_t *fn)
{
	BUILD_BUG_ON(fn != icmp6_send);
	return 0;
}
static inline int inet6_unregister_icmp_sender(ip6_icmp_send_t *fn)
{
	BUILD_BUG_ON(fn != icmp6_send);
	return 0;
}
#else
extern void __icmpv6_send(struct sk_buff *skb, u8 type, u8 code, __u32 info,
			  const struct inet6_skb_parm *parm);
extern int inet6_register_icmp_sender(ip6_icmp_send_t *fn);
extern int inet6_unregister_icmp_sender(ip6_icmp_send_t *fn);
#endif

static inline void icmpv6_send(struct sk_buff *skb, u8 type, u8 code, __u32 info)
{
	__icmpv6_send(skb, type, code, info, IP6CB(skb));
}

int ip6_err_gen_icmpv6_unreach(struct sk_buff *skb, int nhs, int type,
			       unsigned int data_len);

#if IS_ENABLED(CONFIG_NF_NAT)
void icmpv6_ndo_send(struct sk_buff *skb_in, u8 type, u8 code, __u32 info);
#else
static inline void icmpv6_ndo_send(struct sk_buff *skb_in, u8 type, u8 code, __u32 info)
{
	struct inet6_skb_parm parm = { 0 };
	__icmpv6_send(skb_in, type, code, info, &parm);
}
#endif

#else

static inline void icmpv6_send(struct sk_buff *skb,
			       u8 type, u8 code, __u32 info)
{
}

static inline void icmpv6_ndo_send(struct sk_buff *skb,
				   u8 type, u8 code, __u32 info)
{
}
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

extern int				icmpv6_init(void);
extern int				icmpv6_err_convert(u8 type, u8 code,
							   int *err);
extern void				icmpv6_cleanup(void);
<<<<<<< HEAD
extern void				icmpv6_param_prob(struct sk_buff *skb,
							  u8 code, int pos);

struct flowi6;
struct in6_addr;
extern void				icmpv6_flow_init(struct sock *sk,
							 struct flowi6 *fl6,
							 u8 type,
							 const struct in6_addr *saddr,
							 const struct in6_addr *daddr,
							 int oif);
#endif
=======
extern void				icmpv6_param_prob_reason(struct sk_buff *skb,
								 u8 code, int pos,
								 enum skb_drop_reason reason);

struct flowi6;
struct in6_addr;

void icmpv6_flow_init(const struct sock *sk, struct flowi6 *fl6, u8 type,
		      const struct in6_addr *saddr,
		      const struct in6_addr *daddr, int oif);

static inline void icmpv6_param_prob(struct sk_buff *skb, u8 code, int pos)
{
	icmpv6_param_prob_reason(skb, code, pos,
				 SKB_DROP_REASON_NOT_SPECIFIED);
}

static inline bool icmpv6_is_err(int type)
{
	switch (type) {
	case ICMPV6_DEST_UNREACH:
	case ICMPV6_PKT_TOOBIG:
	case ICMPV6_TIME_EXCEED:
	case ICMPV6_PARAMPROB:
		return true;
	}

	return false;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif
