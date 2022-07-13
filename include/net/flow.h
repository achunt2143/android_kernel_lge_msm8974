<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *
 *	Generic internet FLOW.
 *
 */

#ifndef _NET_FLOW_H
#define _NET_FLOW_H

<<<<<<< HEAD
#include <linux/socket.h>
#include <linux/in6.h>
#include <linux/atomic.h>
#include <linux/uidgid.h>

=======
#include <linux/in6.h>
#include <linux/atomic.h>
#include <linux/container_of.h>
#include <linux/uidgid.h>

struct flow_keys;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * ifindex generation is per-net namespace, and loopback is
 * always the 1st device in ns (see net_dev_init), thus any
 * loopback device should get ifindex 1
 */

#define LOOPBACK_IFINDEX	1

<<<<<<< HEAD
struct flowi_common {
	int	flowic_oif;
	int	flowic_iif;
=======
struct flowi_tunnel {
	__be64			tun_id;
};

struct flowi_common {
	int	flowic_oif;
	int	flowic_iif;
	int     flowic_l3mdev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	__u32	flowic_mark;
	__u8	flowic_tos;
	__u8	flowic_scope;
	__u8	flowic_proto;
	__u8	flowic_flags;
#define FLOWI_FLAG_ANYSRC		0x01
<<<<<<< HEAD
#define FLOWI_FLAG_PRECOW_METRICS	0x02
#define FLOWI_FLAG_CAN_SLEEP		0x04
	__u32	flowic_secid;
	kuid_t  flowic_uid;
=======
#define FLOWI_FLAG_KNOWN_NH		0x02
	__u32	flowic_secid;
	kuid_t  flowic_uid;
	__u32		flowic_multipath_hash;
	struct flowi_tunnel flowic_tun_key;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

union flowi_uli {
	struct {
		__be16	dport;
		__be16	sport;
	} ports;

	struct {
		__u8	type;
		__u8	code;
	} icmpt;

<<<<<<< HEAD
	struct {
		__le16	dport;
		__le16	sport;
	} dnports;

	__be32		spi;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	__be32		gre_key;

	struct {
		__u8	type;
	} mht;
};

struct flowi4 {
	struct flowi_common	__fl_common;
#define flowi4_oif		__fl_common.flowic_oif
#define flowi4_iif		__fl_common.flowic_iif
<<<<<<< HEAD
=======
#define flowi4_l3mdev		__fl_common.flowic_l3mdev
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define flowi4_mark		__fl_common.flowic_mark
#define flowi4_tos		__fl_common.flowic_tos
#define flowi4_scope		__fl_common.flowic_scope
#define flowi4_proto		__fl_common.flowic_proto
#define flowi4_flags		__fl_common.flowic_flags
#define flowi4_secid		__fl_common.flowic_secid
<<<<<<< HEAD
#define flowi4_uid		__fl_common.flowic_uid
=======
#define flowi4_tun_key		__fl_common.flowic_tun_key
#define flowi4_uid		__fl_common.flowic_uid
#define flowi4_multipath_hash	__fl_common.flowic_multipath_hash
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* (saddr,daddr) must be grouped, same order as in IP header */
	__be32			saddr;
	__be32			daddr;

	union flowi_uli		uli;
#define fl4_sport		uli.ports.sport
#define fl4_dport		uli.ports.dport
#define fl4_icmp_type		uli.icmpt.type
#define fl4_icmp_code		uli.icmpt.code
<<<<<<< HEAD
#define fl4_ipsec_spi		uli.spi
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define fl4_mh_type		uli.mht.type
#define fl4_gre_key		uli.gre_key
} __attribute__((__aligned__(BITS_PER_LONG/8)));

static inline void flowi4_init_output(struct flowi4 *fl4, int oif,
				      __u32 mark, __u8 tos, __u8 scope,
				      __u8 proto, __u8 flags,
				      __be32 daddr, __be32 saddr,
				      __be16 dport, __be16 sport,
				      kuid_t uid)
{
	fl4->flowi4_oif = oif;
	fl4->flowi4_iif = LOOPBACK_IFINDEX;
<<<<<<< HEAD
=======
	fl4->flowi4_l3mdev = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	fl4->flowi4_mark = mark;
	fl4->flowi4_tos = tos;
	fl4->flowi4_scope = scope;
	fl4->flowi4_proto = proto;
	fl4->flowi4_flags = flags;
	fl4->flowi4_secid = 0;
<<<<<<< HEAD
=======
	fl4->flowi4_tun_key.tun_id = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	fl4->flowi4_uid = uid;
	fl4->daddr = daddr;
	fl4->saddr = saddr;
	fl4->fl4_dport = dport;
	fl4->fl4_sport = sport;
<<<<<<< HEAD
}

/* Reset some input parameters after previous lookup */
static inline void flowi4_update_output(struct flowi4 *fl4, int oif, __u8 tos,
					__be32 daddr, __be32 saddr)
{
	fl4->flowi4_oif = oif;
	fl4->flowi4_tos = tos;
	fl4->daddr = daddr;
	fl4->saddr = saddr;
}
				      
=======
	fl4->flowi4_multipath_hash = 0;
}

/* Reset some input parameters after previous lookup */
static inline void flowi4_update_output(struct flowi4 *fl4, int oif,
					__be32 daddr, __be32 saddr)
{
	fl4->flowi4_oif = oif;
	fl4->daddr = daddr;
	fl4->saddr = saddr;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct flowi6 {
	struct flowi_common	__fl_common;
#define flowi6_oif		__fl_common.flowic_oif
#define flowi6_iif		__fl_common.flowic_iif
<<<<<<< HEAD
#define flowi6_mark		__fl_common.flowic_mark
#define flowi6_tos		__fl_common.flowic_tos
=======
#define flowi6_l3mdev		__fl_common.flowic_l3mdev
#define flowi6_mark		__fl_common.flowic_mark
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define flowi6_scope		__fl_common.flowic_scope
#define flowi6_proto		__fl_common.flowic_proto
#define flowi6_flags		__fl_common.flowic_flags
#define flowi6_secid		__fl_common.flowic_secid
<<<<<<< HEAD
#define flowi6_uid		__fl_common.flowic_uid
	struct in6_addr		daddr;
	struct in6_addr		saddr;
=======
#define flowi6_tun_key		__fl_common.flowic_tun_key
#define flowi6_uid		__fl_common.flowic_uid
	struct in6_addr		daddr;
	struct in6_addr		saddr;
	/* Note: flowi6_tos is encoded in flowlabel, too. */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	__be32			flowlabel;
	union flowi_uli		uli;
#define fl6_sport		uli.ports.sport
#define fl6_dport		uli.ports.dport
#define fl6_icmp_type		uli.icmpt.type
#define fl6_icmp_code		uli.icmpt.code
<<<<<<< HEAD
#define fl6_ipsec_spi		uli.spi
#define fl6_mh_type		uli.mht.type
#define fl6_gre_key		uli.gre_key
} __attribute__((__aligned__(BITS_PER_LONG/8)));

struct flowidn {
	struct flowi_common	__fl_common;
#define flowidn_oif		__fl_common.flowic_oif
#define flowidn_iif		__fl_common.flowic_iif
#define flowidn_mark		__fl_common.flowic_mark
#define flowidn_scope		__fl_common.flowic_scope
#define flowidn_proto		__fl_common.flowic_proto
#define flowidn_flags		__fl_common.flowic_flags
	__le16			daddr;
	__le16			saddr;
	union flowi_uli		uli;
#define fld_sport		uli.ports.sport
#define fld_dport		uli.ports.dport
=======
#define fl6_mh_type		uli.mht.type
#define fl6_gre_key		uli.gre_key
	__u32			mp_hash;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
} __attribute__((__aligned__(BITS_PER_LONG/8)));

struct flowi {
	union {
		struct flowi_common	__fl_common;
		struct flowi4		ip4;
		struct flowi6		ip6;
<<<<<<< HEAD
		struct flowidn		dn;
	} u;
#define flowi_oif	u.__fl_common.flowic_oif
#define flowi_iif	u.__fl_common.flowic_iif
=======
	} u;
#define flowi_oif	u.__fl_common.flowic_oif
#define flowi_iif	u.__fl_common.flowic_iif
#define flowi_l3mdev	u.__fl_common.flowic_l3mdev
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define flowi_mark	u.__fl_common.flowic_mark
#define flowi_tos	u.__fl_common.flowic_tos
#define flowi_scope	u.__fl_common.flowic_scope
#define flowi_proto	u.__fl_common.flowic_proto
#define flowi_flags	u.__fl_common.flowic_flags
#define flowi_secid	u.__fl_common.flowic_secid
<<<<<<< HEAD
=======
#define flowi_tun_key	u.__fl_common.flowic_tun_key
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define flowi_uid	u.__fl_common.flowic_uid
} __attribute__((__aligned__(BITS_PER_LONG/8)));

static inline struct flowi *flowi4_to_flowi(struct flowi4 *fl4)
{
	return container_of(fl4, struct flowi, u.ip4);
}

<<<<<<< HEAD
=======
static inline struct flowi_common *flowi4_to_flowi_common(struct flowi4 *fl4)
{
	return &(fl4->__fl_common);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline struct flowi *flowi6_to_flowi(struct flowi6 *fl6)
{
	return container_of(fl6, struct flowi, u.ip6);
}

<<<<<<< HEAD
static inline struct flowi *flowidn_to_flowi(struct flowidn *fldn)
{
	return container_of(fldn, struct flowi, u.dn);
}

typedef unsigned long flow_compare_t;

static inline size_t flow_key_size(u16 family)
{
	switch (family) {
	case AF_INET:
		BUILD_BUG_ON(sizeof(struct flowi4) % sizeof(flow_compare_t));
		return sizeof(struct flowi4) / sizeof(flow_compare_t);
	case AF_INET6:
		BUILD_BUG_ON(sizeof(struct flowi6) % sizeof(flow_compare_t));
		return sizeof(struct flowi6) / sizeof(flow_compare_t);
	case AF_DECnet:
		BUILD_BUG_ON(sizeof(struct flowidn) % sizeof(flow_compare_t));
		return sizeof(struct flowidn) / sizeof(flow_compare_t);
	}
	return 0;
}

#define FLOW_DIR_IN	0
#define FLOW_DIR_OUT	1
#define FLOW_DIR_FWD	2

struct net;
struct sock;
struct flow_cache_ops;

struct flow_cache_object {
	const struct flow_cache_ops *ops;
};

struct flow_cache_ops {
	struct flow_cache_object *(*get)(struct flow_cache_object *);
	int (*check)(struct flow_cache_object *);
	void (*delete)(struct flow_cache_object *);
};

typedef struct flow_cache_object *(*flow_resolve_t)(
		struct net *net, const struct flowi *key, u16 family,
		u8 dir, struct flow_cache_object *oldobj, void *ctx);

extern struct flow_cache_object *flow_cache_lookup(
		struct net *net, const struct flowi *key, u16 family,
		u8 dir, flow_resolve_t resolver, void *ctx);

extern void flow_cache_flush(void);
extern void flow_cache_flush_deferred(void);
extern atomic_t flow_cache_genid;
=======
static inline struct flowi_common *flowi6_to_flowi_common(struct flowi6 *fl6)
{
	return &(fl6->__fl_common);
}

__u32 __get_hash_from_flowi6(const struct flowi6 *fl6, struct flow_keys *keys);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif
