<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Bridge multicast support.
 *
 * Copyright (c) 2010 Herbert Xu <herbert@gondor.apana.org.au>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 */

#include <linux/err.h>
#include <linux/if_ether.h>
#include <linux/igmp.h>
=======
 */

#include <linux/err.h>
#include <linux/export.h>
#include <linux/if_ether.h>
#include <linux/igmp.h>
#include <linux/in.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/jhash.h>
#include <linux/kernel.h>
#include <linux/log2.h>
#include <linux/netdevice.h>
#include <linux/netfilter_bridge.h>
#include <linux/random.h>
#include <linux/rculist.h>
#include <linux/skbuff.h>
#include <linux/slab.h>
#include <linux/timer.h>
<<<<<<< HEAD
#include <net/ip.h>
#if IS_ENABLED(CONFIG_IPV6)
#include <net/ipv6.h>
#include <net/mld.h>
#include <net/addrconf.h>
#include <net/ip6_checksum.h>
#endif

#include "br_private.h"

#define mlock_dereference(X, br) \
	rcu_dereference_protected(X, lockdep_is_held(&br->multicast_lock))

static void br_multicast_add_router(struct net_bridge *br,
				    struct net_bridge_port *port);

#if IS_ENABLED(CONFIG_IPV6)
static inline int ipv6_is_transient_multicast(const struct in6_addr *addr)
{
	if (ipv6_addr_is_multicast(addr) && IPV6_ADDR_MC_FLAG_TRANSIENT(addr))
		return 1;
	return 0;
}
#endif

static inline int br_ip_equal(const struct br_ip *a, const struct br_ip *b)
{
	if (a->proto != b->proto)
		return 0;
	switch (a->proto) {
	case htons(ETH_P_IP):
		return a->u.ip4 == b->u.ip4;
#if IS_ENABLED(CONFIG_IPV6)
	case htons(ETH_P_IPV6):
		return ipv6_addr_equal(&a->u.ip6, &b->u.ip6);
#endif
	}
	return 0;
}

static inline int __br_ip4_hash(struct net_bridge_mdb_htable *mdb, __be32 ip)
{
	return jhash_1word(mdb->secret, (__force u32)ip) & (mdb->max - 1);
}

#if IS_ENABLED(CONFIG_IPV6)
static inline int __br_ip6_hash(struct net_bridge_mdb_htable *mdb,
				const struct in6_addr *ip)
{
	return jhash2((__force u32 *)ip->s6_addr32, 4, mdb->secret) & (mdb->max - 1);
}
#endif

static inline int br_ip_hash(struct net_bridge_mdb_htable *mdb,
			     struct br_ip *ip)
{
	switch (ip->proto) {
	case htons(ETH_P_IP):
		return __br_ip4_hash(mdb, ip->u.ip4);
#if IS_ENABLED(CONFIG_IPV6)
	case htons(ETH_P_IPV6):
		return __br_ip6_hash(mdb, &ip->u.ip6);
#endif
	}
	return 0;
}

static struct net_bridge_mdb_entry *__br_mdb_ip_get(
	struct net_bridge_mdb_htable *mdb, struct br_ip *dst, int hash)
{
	struct net_bridge_mdb_entry *mp;
	struct hlist_node *p;

	hlist_for_each_entry_rcu(mp, p, &mdb->mhash[hash], hlist[mdb->ver]) {
		if (br_ip_equal(&mp->addr, dst))
			return mp;
	}

	return NULL;
}

static struct net_bridge_mdb_entry *br_mdb_ip_get(
	struct net_bridge_mdb_htable *mdb, struct br_ip *dst)
{
	if (!mdb)
		return NULL;

	return __br_mdb_ip_get(mdb, dst, br_ip_hash(mdb, dst));
}

static struct net_bridge_mdb_entry *br_mdb_ip4_get(
	struct net_bridge_mdb_htable *mdb, __be32 dst)
{
	struct br_ip br_dst;

	br_dst.u.ip4 = dst;
	br_dst.proto = htons(ETH_P_IP);

	return br_mdb_ip_get(mdb, &br_dst);
}

#if IS_ENABLED(CONFIG_IPV6)
static struct net_bridge_mdb_entry *br_mdb_ip6_get(
	struct net_bridge_mdb_htable *mdb, const struct in6_addr *dst)
{
	struct br_ip br_dst;

	br_dst.u.ip6 = *dst;
	br_dst.proto = htons(ETH_P_IPV6);

	return br_mdb_ip_get(mdb, &br_dst);
}
#endif

struct net_bridge_mdb_entry *br_mdb_get(struct net_bridge *br,
					struct sk_buff *skb)
{
	struct net_bridge_mdb_htable *mdb = rcu_dereference(br->mdb);
	struct br_ip ip;

	if (br->multicast_disabled)
=======
#include <linux/inetdevice.h>
#include <linux/mroute.h>
#include <net/ip.h>
#include <net/switchdev.h>
#if IS_ENABLED(CONFIG_IPV6)
#include <linux/icmpv6.h>
#include <net/ipv6.h>
#include <net/mld.h>
#include <net/ip6_checksum.h>
#include <net/addrconf.h>
#endif
#include <trace/events/bridge.h>

#include "br_private.h"
#include "br_private_mcast_eht.h"

static const struct rhashtable_params br_mdb_rht_params = {
	.head_offset = offsetof(struct net_bridge_mdb_entry, rhnode),
	.key_offset = offsetof(struct net_bridge_mdb_entry, addr),
	.key_len = sizeof(struct br_ip),
	.automatic_shrinking = true,
};

static const struct rhashtable_params br_sg_port_rht_params = {
	.head_offset = offsetof(struct net_bridge_port_group, rhnode),
	.key_offset = offsetof(struct net_bridge_port_group, key),
	.key_len = sizeof(struct net_bridge_port_group_sg_key),
	.automatic_shrinking = true,
};

static void br_multicast_start_querier(struct net_bridge_mcast *brmctx,
				       struct bridge_mcast_own_query *query);
static void br_ip4_multicast_add_router(struct net_bridge_mcast *brmctx,
					struct net_bridge_mcast_port *pmctx);
static void br_ip4_multicast_leave_group(struct net_bridge_mcast *brmctx,
					 struct net_bridge_mcast_port *pmctx,
					 __be32 group,
					 __u16 vid,
					 const unsigned char *src);
static void br_multicast_port_group_rexmit(struct timer_list *t);

static void
br_multicast_rport_del_notify(struct net_bridge_mcast_port *pmctx, bool deleted);
static void br_ip6_multicast_add_router(struct net_bridge_mcast *brmctx,
					struct net_bridge_mcast_port *pmctx);
#if IS_ENABLED(CONFIG_IPV6)
static void br_ip6_multicast_leave_group(struct net_bridge_mcast *brmctx,
					 struct net_bridge_mcast_port *pmctx,
					 const struct in6_addr *group,
					 __u16 vid, const unsigned char *src);
#endif
static struct net_bridge_port_group *
__br_multicast_add_group(struct net_bridge_mcast *brmctx,
			 struct net_bridge_mcast_port *pmctx,
			 struct br_ip *group,
			 const unsigned char *src,
			 u8 filter_mode,
			 bool igmpv2_mldv1,
			 bool blocked);
static void br_multicast_find_del_pg(struct net_bridge *br,
				     struct net_bridge_port_group *pg);
static void __br_multicast_stop(struct net_bridge_mcast *brmctx);

static int br_mc_disabled_update(struct net_device *dev, bool value,
				 struct netlink_ext_ack *extack);

static struct net_bridge_port_group *
br_sg_port_find(struct net_bridge *br,
		struct net_bridge_port_group_sg_key *sg_p)
{
	lockdep_assert_held_once(&br->multicast_lock);

	return rhashtable_lookup_fast(&br->sg_port_tbl, sg_p,
				      br_sg_port_rht_params);
}

static struct net_bridge_mdb_entry *br_mdb_ip_get_rcu(struct net_bridge *br,
						      struct br_ip *dst)
{
	return rhashtable_lookup(&br->mdb_hash_tbl, dst, br_mdb_rht_params);
}

struct net_bridge_mdb_entry *br_mdb_ip_get(struct net_bridge *br,
					   struct br_ip *dst)
{
	struct net_bridge_mdb_entry *ent;

	lockdep_assert_held_once(&br->multicast_lock);

	rcu_read_lock();
	ent = rhashtable_lookup(&br->mdb_hash_tbl, dst, br_mdb_rht_params);
	rcu_read_unlock();

	return ent;
}

static struct net_bridge_mdb_entry *br_mdb_ip4_get(struct net_bridge *br,
						   __be32 dst, __u16 vid)
{
	struct br_ip br_dst;

	memset(&br_dst, 0, sizeof(br_dst));
	br_dst.dst.ip4 = dst;
	br_dst.proto = htons(ETH_P_IP);
	br_dst.vid = vid;

	return br_mdb_ip_get(br, &br_dst);
}

#if IS_ENABLED(CONFIG_IPV6)
static struct net_bridge_mdb_entry *br_mdb_ip6_get(struct net_bridge *br,
						   const struct in6_addr *dst,
						   __u16 vid)
{
	struct br_ip br_dst;

	memset(&br_dst, 0, sizeof(br_dst));
	br_dst.dst.ip6 = *dst;
	br_dst.proto = htons(ETH_P_IPV6);
	br_dst.vid = vid;

	return br_mdb_ip_get(br, &br_dst);
}
#endif

struct net_bridge_mdb_entry *
br_mdb_entry_skb_get(struct net_bridge_mcast *brmctx, struct sk_buff *skb,
		     u16 vid)
{
	struct net_bridge *br = brmctx->br;
	struct br_ip ip;

	if (!br_opt_get(br, BROPT_MULTICAST_ENABLED) ||
	    br_multicast_ctx_vlan_global_disabled(brmctx))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return NULL;

	if (BR_INPUT_SKB_CB(skb)->igmp)
		return NULL;

<<<<<<< HEAD
	ip.proto = skb->protocol;

	switch (skb->protocol) {
	case htons(ETH_P_IP):
		ip.u.ip4 = ip_hdr(skb)->daddr;
		break;
#if IS_ENABLED(CONFIG_IPV6)
	case htons(ETH_P_IPV6):
		ip.u.ip6 = ipv6_hdr(skb)->daddr;
		break;
#endif
	default:
		return NULL;
	}

	return br_mdb_ip_get(mdb, &ip);
}

static void br_mdb_free(struct rcu_head *head)
{
	struct net_bridge_mdb_htable *mdb =
		container_of(head, struct net_bridge_mdb_htable, rcu);
	struct net_bridge_mdb_htable *old = mdb->old;

	mdb->old = NULL;
	kfree(old->mhash);
	kfree(old);
}

static int br_mdb_copy(struct net_bridge_mdb_htable *new,
		       struct net_bridge_mdb_htable *old,
		       int elasticity)
{
	struct net_bridge_mdb_entry *mp;
	struct hlist_node *p;
	int maxlen;
	int len;
	int i;

	for (i = 0; i < old->max; i++)
		hlist_for_each_entry(mp, p, &old->mhash[i], hlist[old->ver])
			hlist_add_head(&mp->hlist[new->ver],
				       &new->mhash[br_ip_hash(new, &mp->addr)]);

	if (!elasticity)
		return 0;

	maxlen = 0;
	for (i = 0; i < new->max; i++) {
		len = 0;
		hlist_for_each_entry(mp, p, &new->mhash[i], hlist[new->ver])
			len++;
		if (len > maxlen)
			maxlen = len;
	}

	return maxlen > elasticity ? -EINVAL : 0;
}

static void br_multicast_free_pg(struct rcu_head *head)
{
	struct net_bridge_port_group *p =
		container_of(head, struct net_bridge_port_group, rcu);

	kfree(p);
}

static void br_multicast_free_group(struct rcu_head *head)
{
	struct net_bridge_mdb_entry *mp =
		container_of(head, struct net_bridge_mdb_entry, rcu);

	kfree(mp);
}

static void br_multicast_group_expired(unsigned long data)
{
	struct net_bridge_mdb_entry *mp = (void *)data;
	struct net_bridge *br = mp->br;
	struct net_bridge_mdb_htable *mdb;

	spin_lock(&br->multicast_lock);
	if (!netif_running(br->dev) || timer_pending(&mp->timer))
		goto out;

	mp->mglist = false;

	if (mp->ports)
		goto out;

	mdb = mlock_dereference(br->mdb, br);

	hlist_del_rcu(&mp->hlist[mdb->ver]);
	mdb->size--;

	call_rcu_bh(&mp->rcu, br_multicast_free_group);

=======
	memset(&ip, 0, sizeof(ip));
	ip.proto = skb->protocol;
	ip.vid = vid;

	switch (skb->protocol) {
	case htons(ETH_P_IP):
		ip.dst.ip4 = ip_hdr(skb)->daddr;
		if (brmctx->multicast_igmp_version == 3) {
			struct net_bridge_mdb_entry *mdb;

			ip.src.ip4 = ip_hdr(skb)->saddr;
			mdb = br_mdb_ip_get_rcu(br, &ip);
			if (mdb)
				return mdb;
			ip.src.ip4 = 0;
		}
		break;
#if IS_ENABLED(CONFIG_IPV6)
	case htons(ETH_P_IPV6):
		ip.dst.ip6 = ipv6_hdr(skb)->daddr;
		if (brmctx->multicast_mld_version == 2) {
			struct net_bridge_mdb_entry *mdb;

			ip.src.ip6 = ipv6_hdr(skb)->saddr;
			mdb = br_mdb_ip_get_rcu(br, &ip);
			if (mdb)
				return mdb;
			memset(&ip.src.ip6, 0, sizeof(ip.src.ip6));
		}
		break;
#endif
	default:
		ip.proto = 0;
		ether_addr_copy(ip.dst.mac_addr, eth_hdr(skb)->h_dest);
	}

	return br_mdb_ip_get_rcu(br, &ip);
}

/* IMPORTANT: this function must be used only when the contexts cannot be
 * passed down (e.g. timer) and must be used for read-only purposes because
 * the vlan snooping option can change, so it can return any context
 * (non-vlan or vlan). Its initial intended purpose is to read timer values
 * from the *current* context based on the option. At worst that could lead
 * to inconsistent timers when the contexts are changed, i.e. src timer
 * which needs to re-arm with a specific delay taken from the old context
 */
static struct net_bridge_mcast_port *
br_multicast_pg_to_port_ctx(const struct net_bridge_port_group *pg)
{
	struct net_bridge_mcast_port *pmctx = &pg->key.port->multicast_ctx;
	struct net_bridge_vlan *vlan;

	lockdep_assert_held_once(&pg->key.port->br->multicast_lock);

	/* if vlan snooping is disabled use the port's multicast context */
	if (!pg->key.addr.vid ||
	    !br_opt_get(pg->key.port->br, BROPT_MCAST_VLAN_SNOOPING_ENABLED))
		goto out;

	/* locking is tricky here, due to different rules for multicast and
	 * vlans we need to take rcu to find the vlan and make sure it has
	 * the BR_VLFLAG_MCAST_ENABLED flag set, it can only change under
	 * multicast_lock which must be already held here, so the vlan's pmctx
	 * can safely be used on return
	 */
	rcu_read_lock();
	vlan = br_vlan_find(nbp_vlan_group_rcu(pg->key.port), pg->key.addr.vid);
	if (vlan && !br_multicast_port_ctx_vlan_disabled(&vlan->port_mcast_ctx))
		pmctx = &vlan->port_mcast_ctx;
	else
		pmctx = NULL;
	rcu_read_unlock();
out:
	return pmctx;
}

static struct net_bridge_mcast_port *
br_multicast_port_vid_to_port_ctx(struct net_bridge_port *port, u16 vid)
{
	struct net_bridge_mcast_port *pmctx = NULL;
	struct net_bridge_vlan *vlan;

	lockdep_assert_held_once(&port->br->multicast_lock);

	if (!br_opt_get(port->br, BROPT_MCAST_VLAN_SNOOPING_ENABLED))
		return NULL;

	/* Take RCU to access the vlan. */
	rcu_read_lock();

	vlan = br_vlan_find(nbp_vlan_group_rcu(port), vid);
	if (vlan && !br_multicast_port_ctx_vlan_disabled(&vlan->port_mcast_ctx))
		pmctx = &vlan->port_mcast_ctx;

	rcu_read_unlock();

	return pmctx;
}

/* when snooping we need to check if the contexts should be used
 * in the following order:
 * - if pmctx is non-NULL (port), check if it should be used
 * - if pmctx is NULL (bridge), check if brmctx should be used
 */
static bool
br_multicast_ctx_should_use(const struct net_bridge_mcast *brmctx,
			    const struct net_bridge_mcast_port *pmctx)
{
	if (!netif_running(brmctx->br->dev))
		return false;

	if (pmctx)
		return !br_multicast_port_ctx_state_disabled(pmctx);
	else
		return !br_multicast_ctx_vlan_disabled(brmctx);
}

static bool br_port_group_equal(struct net_bridge_port_group *p,
				struct net_bridge_port *port,
				const unsigned char *src)
{
	if (p->key.port != port)
		return false;

	if (!(port->flags & BR_MULTICAST_TO_UNICAST))
		return true;

	return ether_addr_equal(src, p->eth_addr);
}

static void __fwd_add_star_excl(struct net_bridge_mcast_port *pmctx,
				struct net_bridge_port_group *pg,
				struct br_ip *sg_ip)
{
	struct net_bridge_port_group_sg_key sg_key;
	struct net_bridge_port_group *src_pg;
	struct net_bridge_mcast *brmctx;

	memset(&sg_key, 0, sizeof(sg_key));
	brmctx = br_multicast_port_ctx_get_global(pmctx);
	sg_key.port = pg->key.port;
	sg_key.addr = *sg_ip;
	if (br_sg_port_find(brmctx->br, &sg_key))
		return;

	src_pg = __br_multicast_add_group(brmctx, pmctx,
					  sg_ip, pg->eth_addr,
					  MCAST_INCLUDE, false, false);
	if (IS_ERR_OR_NULL(src_pg) ||
	    src_pg->rt_protocol != RTPROT_KERNEL)
		return;

	src_pg->flags |= MDB_PG_FLAGS_STAR_EXCL;
}

static void __fwd_del_star_excl(struct net_bridge_port_group *pg,
				struct br_ip *sg_ip)
{
	struct net_bridge_port_group_sg_key sg_key;
	struct net_bridge *br = pg->key.port->br;
	struct net_bridge_port_group *src_pg;

	memset(&sg_key, 0, sizeof(sg_key));
	sg_key.port = pg->key.port;
	sg_key.addr = *sg_ip;
	src_pg = br_sg_port_find(br, &sg_key);
	if (!src_pg || !(src_pg->flags & MDB_PG_FLAGS_STAR_EXCL) ||
	    src_pg->rt_protocol != RTPROT_KERNEL)
		return;

	br_multicast_find_del_pg(br, src_pg);
}

/* When a port group transitions to (or is added as) EXCLUDE we need to add it
 * to all other ports' S,G entries which are not blocked by the current group
 * for proper replication, the assumption is that any S,G blocked entries
 * are already added so the S,G,port lookup should skip them.
 * When a port group transitions from EXCLUDE -> INCLUDE mode or is being
 * deleted we need to remove it from all ports' S,G entries where it was
 * automatically installed before (i.e. where it's MDB_PG_FLAGS_STAR_EXCL).
 */
void br_multicast_star_g_handle_mode(struct net_bridge_port_group *pg,
				     u8 filter_mode)
{
	struct net_bridge *br = pg->key.port->br;
	struct net_bridge_port_group *pg_lst;
	struct net_bridge_mcast_port *pmctx;
	struct net_bridge_mdb_entry *mp;
	struct br_ip sg_ip;

	if (WARN_ON(!br_multicast_is_star_g(&pg->key.addr)))
		return;

	mp = br_mdb_ip_get(br, &pg->key.addr);
	if (!mp)
		return;
	pmctx = br_multicast_pg_to_port_ctx(pg);
	if (!pmctx)
		return;

	memset(&sg_ip, 0, sizeof(sg_ip));
	sg_ip = pg->key.addr;

	for (pg_lst = mlock_dereference(mp->ports, br);
	     pg_lst;
	     pg_lst = mlock_dereference(pg_lst->next, br)) {
		struct net_bridge_group_src *src_ent;

		if (pg_lst == pg)
			continue;
		hlist_for_each_entry(src_ent, &pg_lst->src_list, node) {
			if (!(src_ent->flags & BR_SGRP_F_INSTALLED))
				continue;
			sg_ip.src = src_ent->addr.src;
			switch (filter_mode) {
			case MCAST_INCLUDE:
				__fwd_del_star_excl(pg, &sg_ip);
				break;
			case MCAST_EXCLUDE:
				__fwd_add_star_excl(pmctx, pg, &sg_ip);
				break;
			}
		}
	}
}

/* called when adding a new S,G with host_joined == false by default */
static void br_multicast_sg_host_state(struct net_bridge_mdb_entry *star_mp,
				       struct net_bridge_port_group *sg)
{
	struct net_bridge_mdb_entry *sg_mp;

	if (WARN_ON(!br_multicast_is_star_g(&star_mp->addr)))
		return;
	if (!star_mp->host_joined)
		return;

	sg_mp = br_mdb_ip_get(star_mp->br, &sg->key.addr);
	if (!sg_mp)
		return;
	sg_mp->host_joined = true;
}

/* set the host_joined state of all of *,G's S,G entries */
static void br_multicast_star_g_host_state(struct net_bridge_mdb_entry *star_mp)
{
	struct net_bridge *br = star_mp->br;
	struct net_bridge_mdb_entry *sg_mp;
	struct net_bridge_port_group *pg;
	struct br_ip sg_ip;

	if (WARN_ON(!br_multicast_is_star_g(&star_mp->addr)))
		return;

	memset(&sg_ip, 0, sizeof(sg_ip));
	sg_ip = star_mp->addr;
	for (pg = mlock_dereference(star_mp->ports, br);
	     pg;
	     pg = mlock_dereference(pg->next, br)) {
		struct net_bridge_group_src *src_ent;

		hlist_for_each_entry(src_ent, &pg->src_list, node) {
			if (!(src_ent->flags & BR_SGRP_F_INSTALLED))
				continue;
			sg_ip.src = src_ent->addr.src;
			sg_mp = br_mdb_ip_get(br, &sg_ip);
			if (!sg_mp)
				continue;
			sg_mp->host_joined = star_mp->host_joined;
		}
	}
}

static void br_multicast_sg_del_exclude_ports(struct net_bridge_mdb_entry *sgmp)
{
	struct net_bridge_port_group __rcu **pp;
	struct net_bridge_port_group *p;

	/* *,G exclude ports are only added to S,G entries */
	if (WARN_ON(br_multicast_is_star_g(&sgmp->addr)))
		return;

	/* we need the STAR_EXCLUDE ports if there are non-STAR_EXCLUDE ports
	 * we should ignore perm entries since they're managed by user-space
	 */
	for (pp = &sgmp->ports;
	     (p = mlock_dereference(*pp, sgmp->br)) != NULL;
	     pp = &p->next)
		if (!(p->flags & (MDB_PG_FLAGS_STAR_EXCL |
				  MDB_PG_FLAGS_PERMANENT)))
			return;

	/* currently the host can only have joined the *,G which means
	 * we treat it as EXCLUDE {}, so for an S,G it's considered a
	 * STAR_EXCLUDE entry and we can safely leave it
	 */
	sgmp->host_joined = false;

	for (pp = &sgmp->ports;
	     (p = mlock_dereference(*pp, sgmp->br)) != NULL;) {
		if (!(p->flags & MDB_PG_FLAGS_PERMANENT))
			br_multicast_del_pg(sgmp, p, pp);
		else
			pp = &p->next;
	}
}

void br_multicast_sg_add_exclude_ports(struct net_bridge_mdb_entry *star_mp,
				       struct net_bridge_port_group *sg)
{
	struct net_bridge_port_group_sg_key sg_key;
	struct net_bridge *br = star_mp->br;
	struct net_bridge_mcast_port *pmctx;
	struct net_bridge_port_group *pg;
	struct net_bridge_mcast *brmctx;

	if (WARN_ON(br_multicast_is_star_g(&sg->key.addr)))
		return;
	if (WARN_ON(!br_multicast_is_star_g(&star_mp->addr)))
		return;

	br_multicast_sg_host_state(star_mp, sg);
	memset(&sg_key, 0, sizeof(sg_key));
	sg_key.addr = sg->key.addr;
	/* we need to add all exclude ports to the S,G */
	for (pg = mlock_dereference(star_mp->ports, br);
	     pg;
	     pg = mlock_dereference(pg->next, br)) {
		struct net_bridge_port_group *src_pg;

		if (pg == sg || pg->filter_mode == MCAST_INCLUDE)
			continue;

		sg_key.port = pg->key.port;
		if (br_sg_port_find(br, &sg_key))
			continue;

		pmctx = br_multicast_pg_to_port_ctx(pg);
		if (!pmctx)
			continue;
		brmctx = br_multicast_port_ctx_get_global(pmctx);

		src_pg = __br_multicast_add_group(brmctx, pmctx,
						  &sg->key.addr,
						  sg->eth_addr,
						  MCAST_INCLUDE, false, false);
		if (IS_ERR_OR_NULL(src_pg) ||
		    src_pg->rt_protocol != RTPROT_KERNEL)
			continue;
		src_pg->flags |= MDB_PG_FLAGS_STAR_EXCL;
	}
}

static void br_multicast_fwd_src_add(struct net_bridge_group_src *src)
{
	struct net_bridge_mdb_entry *star_mp;
	struct net_bridge_mcast_port *pmctx;
	struct net_bridge_port_group *sg;
	struct net_bridge_mcast *brmctx;
	struct br_ip sg_ip;

	if (src->flags & BR_SGRP_F_INSTALLED)
		return;

	memset(&sg_ip, 0, sizeof(sg_ip));
	pmctx = br_multicast_pg_to_port_ctx(src->pg);
	if (!pmctx)
		return;
	brmctx = br_multicast_port_ctx_get_global(pmctx);
	sg_ip = src->pg->key.addr;
	sg_ip.src = src->addr.src;

	sg = __br_multicast_add_group(brmctx, pmctx, &sg_ip,
				      src->pg->eth_addr, MCAST_INCLUDE, false,
				      !timer_pending(&src->timer));
	if (IS_ERR_OR_NULL(sg))
		return;
	src->flags |= BR_SGRP_F_INSTALLED;
	sg->flags &= ~MDB_PG_FLAGS_STAR_EXCL;

	/* if it was added by user-space as perm we can skip next steps */
	if (sg->rt_protocol != RTPROT_KERNEL &&
	    (sg->flags & MDB_PG_FLAGS_PERMANENT))
		return;

	/* the kernel is now responsible for removing this S,G */
	del_timer(&sg->timer);
	star_mp = br_mdb_ip_get(src->br, &src->pg->key.addr);
	if (!star_mp)
		return;

	br_multicast_sg_add_exclude_ports(star_mp, sg);
}

static void br_multicast_fwd_src_remove(struct net_bridge_group_src *src,
					bool fastleave)
{
	struct net_bridge_port_group *p, *pg = src->pg;
	struct net_bridge_port_group __rcu **pp;
	struct net_bridge_mdb_entry *mp;
	struct br_ip sg_ip;

	memset(&sg_ip, 0, sizeof(sg_ip));
	sg_ip = pg->key.addr;
	sg_ip.src = src->addr.src;

	mp = br_mdb_ip_get(src->br, &sg_ip);
	if (!mp)
		return;

	for (pp = &mp->ports;
	     (p = mlock_dereference(*pp, src->br)) != NULL;
	     pp = &p->next) {
		if (!br_port_group_equal(p, pg->key.port, pg->eth_addr))
			continue;

		if (p->rt_protocol != RTPROT_KERNEL &&
		    (p->flags & MDB_PG_FLAGS_PERMANENT) &&
		    !(src->flags & BR_SGRP_F_USER_ADDED))
			break;

		if (fastleave)
			p->flags |= MDB_PG_FLAGS_FAST_LEAVE;
		br_multicast_del_pg(mp, p, pp);
		break;
	}
	src->flags &= ~BR_SGRP_F_INSTALLED;
}

/* install S,G and based on src's timer enable or disable forwarding */
static void br_multicast_fwd_src_handle(struct net_bridge_group_src *src)
{
	struct net_bridge_port_group_sg_key sg_key;
	struct net_bridge_port_group *sg;
	u8 old_flags;

	br_multicast_fwd_src_add(src);

	memset(&sg_key, 0, sizeof(sg_key));
	sg_key.addr = src->pg->key.addr;
	sg_key.addr.src = src->addr.src;
	sg_key.port = src->pg->key.port;

	sg = br_sg_port_find(src->br, &sg_key);
	if (!sg || (sg->flags & MDB_PG_FLAGS_PERMANENT))
		return;

	old_flags = sg->flags;
	if (timer_pending(&src->timer))
		sg->flags &= ~MDB_PG_FLAGS_BLOCKED;
	else
		sg->flags |= MDB_PG_FLAGS_BLOCKED;

	if (old_flags != sg->flags) {
		struct net_bridge_mdb_entry *sg_mp;

		sg_mp = br_mdb_ip_get(src->br, &sg_key.addr);
		if (!sg_mp)
			return;
		br_mdb_notify(src->br->dev, sg_mp, sg, RTM_NEWMDB);
	}
}

static void br_multicast_destroy_mdb_entry(struct net_bridge_mcast_gc *gc)
{
	struct net_bridge_mdb_entry *mp;

	mp = container_of(gc, struct net_bridge_mdb_entry, mcast_gc);
	WARN_ON(!hlist_unhashed(&mp->mdb_node));
	WARN_ON(mp->ports);

	timer_shutdown_sync(&mp->timer);
	kfree_rcu(mp, rcu);
}

static void br_multicast_del_mdb_entry(struct net_bridge_mdb_entry *mp)
{
	struct net_bridge *br = mp->br;

	rhashtable_remove_fast(&br->mdb_hash_tbl, &mp->rhnode,
			       br_mdb_rht_params);
	hlist_del_init_rcu(&mp->mdb_node);
	hlist_add_head(&mp->mcast_gc.gc_node, &br->mcast_gc_list);
	queue_work(system_long_wq, &br->mcast_gc_work);
}

static void br_multicast_group_expired(struct timer_list *t)
{
	struct net_bridge_mdb_entry *mp = from_timer(mp, t, timer);
	struct net_bridge *br = mp->br;

	spin_lock(&br->multicast_lock);
	if (hlist_unhashed(&mp->mdb_node) || !netif_running(br->dev) ||
	    timer_pending(&mp->timer))
		goto out;

	br_multicast_host_leave(mp, true);

	if (mp->ports)
		goto out;
	br_multicast_del_mdb_entry(mp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	spin_unlock(&br->multicast_lock);
}

<<<<<<< HEAD
static void br_multicast_del_pg(struct net_bridge *br,
				struct net_bridge_port_group *pg)
{
	struct net_bridge_mdb_htable *mdb;
	struct net_bridge_mdb_entry *mp;
	struct net_bridge_port_group *p;
	struct net_bridge_port_group __rcu **pp;

	mdb = mlock_dereference(br->mdb, br);

	mp = br_mdb_ip_get(mdb, &pg->addr);
=======
static void br_multicast_destroy_group_src(struct net_bridge_mcast_gc *gc)
{
	struct net_bridge_group_src *src;

	src = container_of(gc, struct net_bridge_group_src, mcast_gc);
	WARN_ON(!hlist_unhashed(&src->node));

	timer_shutdown_sync(&src->timer);
	kfree_rcu(src, rcu);
}

void __br_multicast_del_group_src(struct net_bridge_group_src *src)
{
	struct net_bridge *br = src->pg->key.port->br;

	hlist_del_init_rcu(&src->node);
	src->pg->src_ents--;
	hlist_add_head(&src->mcast_gc.gc_node, &br->mcast_gc_list);
	queue_work(system_long_wq, &br->mcast_gc_work);
}

void br_multicast_del_group_src(struct net_bridge_group_src *src,
				bool fastleave)
{
	br_multicast_fwd_src_remove(src, fastleave);
	__br_multicast_del_group_src(src);
}

static int
br_multicast_port_ngroups_inc_one(struct net_bridge_mcast_port *pmctx,
				  struct netlink_ext_ack *extack,
				  const char *what)
{
	u32 max = READ_ONCE(pmctx->mdb_max_entries);
	u32 n = READ_ONCE(pmctx->mdb_n_entries);

	if (max && n >= max) {
		NL_SET_ERR_MSG_FMT_MOD(extack, "%s is already in %u groups, and mcast_max_groups=%u",
				       what, n, max);
		return -E2BIG;
	}

	WRITE_ONCE(pmctx->mdb_n_entries, n + 1);
	return 0;
}

static void br_multicast_port_ngroups_dec_one(struct net_bridge_mcast_port *pmctx)
{
	u32 n = READ_ONCE(pmctx->mdb_n_entries);

	WARN_ON_ONCE(n == 0);
	WRITE_ONCE(pmctx->mdb_n_entries, n - 1);
}

static int br_multicast_port_ngroups_inc(struct net_bridge_port *port,
					 const struct br_ip *group,
					 struct netlink_ext_ack *extack)
{
	struct net_bridge_mcast_port *pmctx;
	int err;

	lockdep_assert_held_once(&port->br->multicast_lock);

	/* Always count on the port context. */
	err = br_multicast_port_ngroups_inc_one(&port->multicast_ctx, extack,
						"Port");
	if (err) {
		trace_br_mdb_full(port->dev, group);
		return err;
	}

	/* Only count on the VLAN context if VID is given, and if snooping on
	 * that VLAN is enabled.
	 */
	if (!group->vid)
		return 0;

	pmctx = br_multicast_port_vid_to_port_ctx(port, group->vid);
	if (!pmctx)
		return 0;

	err = br_multicast_port_ngroups_inc_one(pmctx, extack, "Port-VLAN");
	if (err) {
		trace_br_mdb_full(port->dev, group);
		goto dec_one_out;
	}

	return 0;

dec_one_out:
	br_multicast_port_ngroups_dec_one(&port->multicast_ctx);
	return err;
}

static void br_multicast_port_ngroups_dec(struct net_bridge_port *port, u16 vid)
{
	struct net_bridge_mcast_port *pmctx;

	lockdep_assert_held_once(&port->br->multicast_lock);

	if (vid) {
		pmctx = br_multicast_port_vid_to_port_ctx(port, vid);
		if (pmctx)
			br_multicast_port_ngroups_dec_one(pmctx);
	}
	br_multicast_port_ngroups_dec_one(&port->multicast_ctx);
}

u32 br_multicast_ngroups_get(const struct net_bridge_mcast_port *pmctx)
{
	return READ_ONCE(pmctx->mdb_n_entries);
}

void br_multicast_ngroups_set_max(struct net_bridge_mcast_port *pmctx, u32 max)
{
	WRITE_ONCE(pmctx->mdb_max_entries, max);
}

u32 br_multicast_ngroups_get_max(const struct net_bridge_mcast_port *pmctx)
{
	return READ_ONCE(pmctx->mdb_max_entries);
}

static void br_multicast_destroy_port_group(struct net_bridge_mcast_gc *gc)
{
	struct net_bridge_port_group *pg;

	pg = container_of(gc, struct net_bridge_port_group, mcast_gc);
	WARN_ON(!hlist_unhashed(&pg->mglist));
	WARN_ON(!hlist_empty(&pg->src_list));

	timer_shutdown_sync(&pg->rexmit_timer);
	timer_shutdown_sync(&pg->timer);
	kfree_rcu(pg, rcu);
}

void br_multicast_del_pg(struct net_bridge_mdb_entry *mp,
			 struct net_bridge_port_group *pg,
			 struct net_bridge_port_group __rcu **pp)
{
	struct net_bridge *br = pg->key.port->br;
	struct net_bridge_group_src *ent;
	struct hlist_node *tmp;

	rcu_assign_pointer(*pp, pg->next);
	hlist_del_init(&pg->mglist);
	br_multicast_eht_clean_sets(pg);
	hlist_for_each_entry_safe(ent, tmp, &pg->src_list, node)
		br_multicast_del_group_src(ent, false);
	br_mdb_notify(br->dev, mp, pg, RTM_DELMDB);
	if (!br_multicast_is_star_g(&mp->addr)) {
		rhashtable_remove_fast(&br->sg_port_tbl, &pg->rhnode,
				       br_sg_port_rht_params);
		br_multicast_sg_del_exclude_ports(mp);
	} else {
		br_multicast_star_g_handle_mode(pg, MCAST_INCLUDE);
	}
	br_multicast_port_ngroups_dec(pg->key.port, pg->key.addr.vid);
	hlist_add_head(&pg->mcast_gc.gc_node, &br->mcast_gc_list);
	queue_work(system_long_wq, &br->mcast_gc_work);

	if (!mp->ports && !mp->host_joined && netif_running(br->dev))
		mod_timer(&mp->timer, jiffies);
}

static void br_multicast_find_del_pg(struct net_bridge *br,
				     struct net_bridge_port_group *pg)
{
	struct net_bridge_port_group __rcu **pp;
	struct net_bridge_mdb_entry *mp;
	struct net_bridge_port_group *p;

	mp = br_mdb_ip_get(br, &pg->key.addr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (WARN_ON(!mp))
		return;

	for (pp = &mp->ports;
	     (p = mlock_dereference(*pp, br)) != NULL;
	     pp = &p->next) {
		if (p != pg)
			continue;

<<<<<<< HEAD
		rcu_assign_pointer(*pp, p->next);
		hlist_del_init(&p->mglist);
		del_timer(&p->timer);
		call_rcu_bh(&p->rcu, br_multicast_free_pg);

		if (!mp->ports && !mp->mglist &&
		    netif_running(br->dev))
			mod_timer(&mp->timer, jiffies);

=======
		br_multicast_del_pg(mp, pg, pp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;
	}

	WARN_ON(1);
}

<<<<<<< HEAD
static void br_multicast_port_group_expired(unsigned long data)
{
	struct net_bridge_port_group *pg = (void *)data;
	struct net_bridge *br = pg->port->br;

	spin_lock(&br->multicast_lock);
	if (!netif_running(br->dev) || timer_pending(&pg->timer) ||
	    hlist_unhashed(&pg->mglist))
		goto out;

	br_multicast_del_pg(br, pg);

=======
static void br_multicast_port_group_expired(struct timer_list *t)
{
	struct net_bridge_port_group *pg = from_timer(pg, t, timer);
	struct net_bridge_group_src *src_ent;
	struct net_bridge *br = pg->key.port->br;
	struct hlist_node *tmp;
	bool changed;

	spin_lock(&br->multicast_lock);
	if (!netif_running(br->dev) || timer_pending(&pg->timer) ||
	    hlist_unhashed(&pg->mglist) || pg->flags & MDB_PG_FLAGS_PERMANENT)
		goto out;

	changed = !!(pg->filter_mode == MCAST_EXCLUDE);
	pg->filter_mode = MCAST_INCLUDE;
	hlist_for_each_entry_safe(src_ent, tmp, &pg->src_list, node) {
		if (!timer_pending(&src_ent->timer)) {
			br_multicast_del_group_src(src_ent, false);
			changed = true;
		}
	}

	if (hlist_empty(&pg->src_list)) {
		br_multicast_find_del_pg(br, pg);
	} else if (changed) {
		struct net_bridge_mdb_entry *mp = br_mdb_ip_get(br, &pg->key.addr);

		if (changed && br_multicast_is_star_g(&pg->key.addr))
			br_multicast_star_g_handle_mode(pg, MCAST_INCLUDE);

		if (WARN_ON(!mp))
			goto out;
		br_mdb_notify(br->dev, mp, pg, RTM_NEWMDB);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	spin_unlock(&br->multicast_lock);
}

<<<<<<< HEAD
static int br_mdb_rehash(struct net_bridge_mdb_htable __rcu **mdbp, int max,
			 int elasticity)
{
	struct net_bridge_mdb_htable *old = rcu_dereference_protected(*mdbp, 1);
	struct net_bridge_mdb_htable *mdb;
	int err;

	mdb = kmalloc(sizeof(*mdb), GFP_ATOMIC);
	if (!mdb)
		return -ENOMEM;

	mdb->max = max;
	mdb->old = old;

	mdb->mhash = kzalloc(max * sizeof(*mdb->mhash), GFP_ATOMIC);
	if (!mdb->mhash) {
		kfree(mdb);
		return -ENOMEM;
	}

	mdb->size = old ? old->size : 0;
	mdb->ver = old ? old->ver ^ 1 : 0;

	if (!old || elasticity)
		get_random_bytes(&mdb->secret, sizeof(mdb->secret));
	else
		mdb->secret = old->secret;

	if (!old)
		goto out;

	err = br_mdb_copy(mdb, old, elasticity);
	if (err) {
		kfree(mdb->mhash);
		kfree(mdb);
		return err;
	}

	call_rcu_bh(&mdb->rcu, br_mdb_free);

out:
	rcu_assign_pointer(*mdbp, mdb);

	return 0;
}

static struct sk_buff *br_ip4_multicast_alloc_query(struct net_bridge *br,
						    __be32 group)
{
	struct sk_buff *skb;
	struct igmphdr *ih;
	struct ethhdr *eth;
	struct iphdr *iph;

	skb = netdev_alloc_skb_ip_align(br->dev, sizeof(*eth) + sizeof(*iph) +
						 sizeof(*ih) + 4);
	if (!skb)
		goto out;

=======
static void br_multicast_gc(struct hlist_head *head)
{
	struct net_bridge_mcast_gc *gcent;
	struct hlist_node *tmp;

	hlist_for_each_entry_safe(gcent, tmp, head, gc_node) {
		hlist_del_init(&gcent->gc_node);
		gcent->destroy(gcent);
	}
}

static void __br_multicast_query_handle_vlan(struct net_bridge_mcast *brmctx,
					     struct net_bridge_mcast_port *pmctx,
					     struct sk_buff *skb)
{
	struct net_bridge_vlan *vlan = NULL;

	if (pmctx && br_multicast_port_ctx_is_vlan(pmctx))
		vlan = pmctx->vlan;
	else if (br_multicast_ctx_is_vlan(brmctx))
		vlan = brmctx->vlan;

	if (vlan && !(vlan->flags & BRIDGE_VLAN_INFO_UNTAGGED)) {
		u16 vlan_proto;

		if (br_vlan_get_proto(brmctx->br->dev, &vlan_proto) != 0)
			return;
		__vlan_hwaccel_put_tag(skb, htons(vlan_proto), vlan->vid);
	}
}

static struct sk_buff *br_ip4_multicast_alloc_query(struct net_bridge_mcast *brmctx,
						    struct net_bridge_mcast_port *pmctx,
						    struct net_bridge_port_group *pg,
						    __be32 ip_dst, __be32 group,
						    bool with_srcs, bool over_lmqt,
						    u8 sflag, u8 *igmp_type,
						    bool *need_rexmit)
{
	struct net_bridge_port *p = pg ? pg->key.port : NULL;
	struct net_bridge_group_src *ent;
	size_t pkt_size, igmp_hdr_size;
	unsigned long now = jiffies;
	struct igmpv3_query *ihv3;
	void *csum_start = NULL;
	__sum16 *csum = NULL;
	struct sk_buff *skb;
	struct igmphdr *ih;
	struct ethhdr *eth;
	unsigned long lmqt;
	struct iphdr *iph;
	u16 lmqt_srcs = 0;

	igmp_hdr_size = sizeof(*ih);
	if (brmctx->multicast_igmp_version == 3) {
		igmp_hdr_size = sizeof(*ihv3);
		if (pg && with_srcs) {
			lmqt = now + (brmctx->multicast_last_member_interval *
				      brmctx->multicast_last_member_count);
			hlist_for_each_entry(ent, &pg->src_list, node) {
				if (over_lmqt == time_after(ent->timer.expires,
							    lmqt) &&
				    ent->src_query_rexmit_cnt > 0)
					lmqt_srcs++;
			}

			if (!lmqt_srcs)
				return NULL;
			igmp_hdr_size += lmqt_srcs * sizeof(__be32);
		}
	}

	pkt_size = sizeof(*eth) + sizeof(*iph) + 4 + igmp_hdr_size;
	if ((p && pkt_size > p->dev->mtu) ||
	    pkt_size > brmctx->br->dev->mtu)
		return NULL;

	skb = netdev_alloc_skb_ip_align(brmctx->br->dev, pkt_size);
	if (!skb)
		goto out;

	__br_multicast_query_handle_vlan(brmctx, pmctx, skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	skb->protocol = htons(ETH_P_IP);

	skb_reset_mac_header(skb);
	eth = eth_hdr(skb);

<<<<<<< HEAD
	memcpy(eth->h_source, br->dev->dev_addr, 6);
	eth->h_dest[0] = 1;
	eth->h_dest[1] = 0;
	eth->h_dest[2] = 0x5e;
	eth->h_dest[3] = 0;
	eth->h_dest[4] = 0;
	eth->h_dest[5] = 1;
=======
	ether_addr_copy(eth->h_source, brmctx->br->dev->dev_addr);
	ip_eth_mc_map(ip_dst, eth->h_dest);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	eth->h_proto = htons(ETH_P_IP);
	skb_put(skb, sizeof(*eth));

	skb_set_network_header(skb, skb->len);
	iph = ip_hdr(skb);
<<<<<<< HEAD
=======
	iph->tot_len = htons(pkt_size - sizeof(*eth));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	iph->version = 4;
	iph->ihl = 6;
	iph->tos = 0xc0;
<<<<<<< HEAD
	iph->tot_len = htons(sizeof(*iph) + sizeof(*ih) + 4);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	iph->id = 0;
	iph->frag_off = htons(IP_DF);
	iph->ttl = 1;
	iph->protocol = IPPROTO_IGMP;
<<<<<<< HEAD
	iph->saddr = 0;
	iph->daddr = htonl(INADDR_ALLHOSTS_GROUP);
=======
	iph->saddr = br_opt_get(brmctx->br, BROPT_MULTICAST_QUERY_USE_IFADDR) ?
		     inet_select_addr(brmctx->br->dev, 0, RT_SCOPE_LINK) : 0;
	iph->daddr = ip_dst;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	((u8 *)&iph[1])[0] = IPOPT_RA;
	((u8 *)&iph[1])[1] = 4;
	((u8 *)&iph[1])[2] = 0;
	((u8 *)&iph[1])[3] = 0;
	ip_send_check(iph);
	skb_put(skb, 24);

	skb_set_transport_header(skb, skb->len);
<<<<<<< HEAD
	ih = igmp_hdr(skb);
	ih->type = IGMP_HOST_MEMBERSHIP_QUERY;
	ih->code = (group ? br->multicast_last_member_interval :
			    br->multicast_query_response_interval) /
		   (HZ / IGMP_TIMER_SCALE);
	ih->group = group;
	ih->csum = 0;
	ih->csum = ip_compute_csum((void *)ih, sizeof(struct igmphdr));
	skb_put(skb, sizeof(*ih));

=======
	*igmp_type = IGMP_HOST_MEMBERSHIP_QUERY;

	switch (brmctx->multicast_igmp_version) {
	case 2:
		ih = igmp_hdr(skb);
		ih->type = IGMP_HOST_MEMBERSHIP_QUERY;
		ih->code = (group ? brmctx->multicast_last_member_interval :
				    brmctx->multicast_query_response_interval) /
			   (HZ / IGMP_TIMER_SCALE);
		ih->group = group;
		ih->csum = 0;
		csum = &ih->csum;
		csum_start = (void *)ih;
		break;
	case 3:
		ihv3 = igmpv3_query_hdr(skb);
		ihv3->type = IGMP_HOST_MEMBERSHIP_QUERY;
		ihv3->code = (group ? brmctx->multicast_last_member_interval :
				      brmctx->multicast_query_response_interval) /
			     (HZ / IGMP_TIMER_SCALE);
		ihv3->group = group;
		ihv3->qqic = brmctx->multicast_query_interval / HZ;
		ihv3->nsrcs = htons(lmqt_srcs);
		ihv3->resv = 0;
		ihv3->suppress = sflag;
		ihv3->qrv = 2;
		ihv3->csum = 0;
		csum = &ihv3->csum;
		csum_start = (void *)ihv3;
		if (!pg || !with_srcs)
			break;

		lmqt_srcs = 0;
		hlist_for_each_entry(ent, &pg->src_list, node) {
			if (over_lmqt == time_after(ent->timer.expires,
						    lmqt) &&
			    ent->src_query_rexmit_cnt > 0) {
				ihv3->srcs[lmqt_srcs++] = ent->addr.src.ip4;
				ent->src_query_rexmit_cnt--;
				if (need_rexmit && ent->src_query_rexmit_cnt)
					*need_rexmit = true;
			}
		}
		if (WARN_ON(lmqt_srcs != ntohs(ihv3->nsrcs))) {
			kfree_skb(skb);
			return NULL;
		}
		break;
	}

	if (WARN_ON(!csum || !csum_start)) {
		kfree_skb(skb);
		return NULL;
	}

	*csum = ip_compute_csum(csum_start, igmp_hdr_size);
	skb_put(skb, igmp_hdr_size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	__skb_pull(skb, sizeof(*eth));

out:
	return skb;
}

#if IS_ENABLED(CONFIG_IPV6)
<<<<<<< HEAD
static struct sk_buff *br_ip6_multicast_alloc_query(struct net_bridge *br,
						    const struct in6_addr *group)
{
	struct sk_buff *skb;
	struct ipv6hdr *ip6h;
	struct mld_msg *mldq;
	struct ethhdr *eth;
	u8 *hopopt;
	unsigned long interval;

	skb = netdev_alloc_skb_ip_align(br->dev, sizeof(*eth) + sizeof(*ip6h) +
						 8 + sizeof(*mldq));
	if (!skb)
		goto out;

=======
static struct sk_buff *br_ip6_multicast_alloc_query(struct net_bridge_mcast *brmctx,
						    struct net_bridge_mcast_port *pmctx,
						    struct net_bridge_port_group *pg,
						    const struct in6_addr *ip6_dst,
						    const struct in6_addr *group,
						    bool with_srcs, bool over_llqt,
						    u8 sflag, u8 *igmp_type,
						    bool *need_rexmit)
{
	struct net_bridge_port *p = pg ? pg->key.port : NULL;
	struct net_bridge_group_src *ent;
	size_t pkt_size, mld_hdr_size;
	unsigned long now = jiffies;
	struct mld2_query *mld2q;
	void *csum_start = NULL;
	unsigned long interval;
	__sum16 *csum = NULL;
	struct ipv6hdr *ip6h;
	struct mld_msg *mldq;
	struct sk_buff *skb;
	unsigned long llqt;
	struct ethhdr *eth;
	u16 llqt_srcs = 0;
	u8 *hopopt;

	mld_hdr_size = sizeof(*mldq);
	if (brmctx->multicast_mld_version == 2) {
		mld_hdr_size = sizeof(*mld2q);
		if (pg && with_srcs) {
			llqt = now + (brmctx->multicast_last_member_interval *
				      brmctx->multicast_last_member_count);
			hlist_for_each_entry(ent, &pg->src_list, node) {
				if (over_llqt == time_after(ent->timer.expires,
							    llqt) &&
				    ent->src_query_rexmit_cnt > 0)
					llqt_srcs++;
			}

			if (!llqt_srcs)
				return NULL;
			mld_hdr_size += llqt_srcs * sizeof(struct in6_addr);
		}
	}

	pkt_size = sizeof(*eth) + sizeof(*ip6h) + 8 + mld_hdr_size;
	if ((p && pkt_size > p->dev->mtu) ||
	    pkt_size > brmctx->br->dev->mtu)
		return NULL;

	skb = netdev_alloc_skb_ip_align(brmctx->br->dev, pkt_size);
	if (!skb)
		goto out;

	__br_multicast_query_handle_vlan(brmctx, pmctx, skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	skb->protocol = htons(ETH_P_IPV6);

	/* Ethernet header */
	skb_reset_mac_header(skb);
	eth = eth_hdr(skb);

<<<<<<< HEAD
	memcpy(eth->h_source, br->dev->dev_addr, 6);
=======
	ether_addr_copy(eth->h_source, brmctx->br->dev->dev_addr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	eth->h_proto = htons(ETH_P_IPV6);
	skb_put(skb, sizeof(*eth));

	/* IPv6 header + HbH option */
	skb_set_network_header(skb, skb->len);
	ip6h = ipv6_hdr(skb);

	*(__force __be32 *)ip6h = htonl(0x60000000);
<<<<<<< HEAD
	ip6h->payload_len = htons(8 + sizeof(*mldq));
	ip6h->nexthdr = IPPROTO_HOPOPTS;
	ip6h->hop_limit = 1;
	ipv6_addr_set(&ip6h->daddr, htonl(0xff020000), 0, 0, htonl(1));
	if (ipv6_dev_get_saddr(dev_net(br->dev), br->dev, &ip6h->daddr, 0,
			       &ip6h->saddr)) {
		kfree_skb(skb);
		return NULL;
	}
=======
	ip6h->payload_len = htons(8 + mld_hdr_size);
	ip6h->nexthdr = IPPROTO_HOPOPTS;
	ip6h->hop_limit = 1;
	ip6h->daddr = *ip6_dst;
	if (ipv6_dev_get_saddr(dev_net(brmctx->br->dev), brmctx->br->dev,
			       &ip6h->daddr, 0, &ip6h->saddr)) {
		kfree_skb(skb);
		br_opt_toggle(brmctx->br, BROPT_HAS_IPV6_ADDR, false);
		return NULL;
	}

	br_opt_toggle(brmctx->br, BROPT_HAS_IPV6_ADDR, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ipv6_eth_mc_map(&ip6h->daddr, eth->h_dest);

	hopopt = (u8 *)(ip6h + 1);
	hopopt[0] = IPPROTO_ICMPV6;		/* next hdr */
	hopopt[1] = 0;				/* length of HbH */
	hopopt[2] = IPV6_TLV_ROUTERALERT;	/* Router Alert */
	hopopt[3] = 2;				/* Length of RA Option */
	hopopt[4] = 0;				/* Type = 0x0000 (MLD) */
	hopopt[5] = 0;
<<<<<<< HEAD
	hopopt[6] = IPV6_TLV_PAD0;		/* Pad0 */
	hopopt[7] = IPV6_TLV_PAD0;		/* Pad0 */
=======
	hopopt[6] = IPV6_TLV_PAD1;		/* Pad1 */
	hopopt[7] = IPV6_TLV_PAD1;		/* Pad1 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	skb_put(skb, sizeof(*ip6h) + 8);

	/* ICMPv6 */
	skb_set_transport_header(skb, skb->len);
<<<<<<< HEAD
	mldq = (struct mld_msg *) icmp6_hdr(skb);

	interval = ipv6_addr_any(group) ?
			br->multicast_query_response_interval :
			br->multicast_last_member_interval;

	mldq->mld_type = ICMPV6_MGM_QUERY;
	mldq->mld_code = 0;
	mldq->mld_cksum = 0;
	mldq->mld_maxdelay = htons((u16)jiffies_to_msecs(interval));
	mldq->mld_reserved = 0;
	mldq->mld_mca = *group;

	/* checksum */
	mldq->mld_cksum = csum_ipv6_magic(&ip6h->saddr, &ip6h->daddr,
					  sizeof(*mldq), IPPROTO_ICMPV6,
					  csum_partial(mldq,
						       sizeof(*mldq), 0));
	skb_put(skb, sizeof(*mldq));

=======
	interval = ipv6_addr_any(group) ?
			brmctx->multicast_query_response_interval :
			brmctx->multicast_last_member_interval;
	*igmp_type = ICMPV6_MGM_QUERY;
	switch (brmctx->multicast_mld_version) {
	case 1:
		mldq = (struct mld_msg *)icmp6_hdr(skb);
		mldq->mld_type = ICMPV6_MGM_QUERY;
		mldq->mld_code = 0;
		mldq->mld_cksum = 0;
		mldq->mld_maxdelay = htons((u16)jiffies_to_msecs(interval));
		mldq->mld_reserved = 0;
		mldq->mld_mca = *group;
		csum = &mldq->mld_cksum;
		csum_start = (void *)mldq;
		break;
	case 2:
		mld2q = (struct mld2_query *)icmp6_hdr(skb);
		mld2q->mld2q_mrc = htons((u16)jiffies_to_msecs(interval));
		mld2q->mld2q_type = ICMPV6_MGM_QUERY;
		mld2q->mld2q_code = 0;
		mld2q->mld2q_cksum = 0;
		mld2q->mld2q_resv1 = 0;
		mld2q->mld2q_resv2 = 0;
		mld2q->mld2q_suppress = sflag;
		mld2q->mld2q_qrv = 2;
		mld2q->mld2q_nsrcs = htons(llqt_srcs);
		mld2q->mld2q_qqic = brmctx->multicast_query_interval / HZ;
		mld2q->mld2q_mca = *group;
		csum = &mld2q->mld2q_cksum;
		csum_start = (void *)mld2q;
		if (!pg || !with_srcs)
			break;

		llqt_srcs = 0;
		hlist_for_each_entry(ent, &pg->src_list, node) {
			if (over_llqt == time_after(ent->timer.expires,
						    llqt) &&
			    ent->src_query_rexmit_cnt > 0) {
				mld2q->mld2q_srcs[llqt_srcs++] = ent->addr.src.ip6;
				ent->src_query_rexmit_cnt--;
				if (need_rexmit && ent->src_query_rexmit_cnt)
					*need_rexmit = true;
			}
		}
		if (WARN_ON(llqt_srcs != ntohs(mld2q->mld2q_nsrcs))) {
			kfree_skb(skb);
			return NULL;
		}
		break;
	}

	if (WARN_ON(!csum || !csum_start)) {
		kfree_skb(skb);
		return NULL;
	}

	*csum = csum_ipv6_magic(&ip6h->saddr, &ip6h->daddr, mld_hdr_size,
				IPPROTO_ICMPV6,
				csum_partial(csum_start, mld_hdr_size, 0));
	skb_put(skb, mld_hdr_size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	__skb_pull(skb, sizeof(*eth));

out:
	return skb;
}
#endif

<<<<<<< HEAD
static struct sk_buff *br_multicast_alloc_query(struct net_bridge *br,
						struct br_ip *addr)
{
	switch (addr->proto) {
	case htons(ETH_P_IP):
		return br_ip4_multicast_alloc_query(br, addr->u.ip4);
#if IS_ENABLED(CONFIG_IPV6)
	case htons(ETH_P_IPV6):
		return br_ip6_multicast_alloc_query(br, &addr->u.ip6);
=======
static struct sk_buff *br_multicast_alloc_query(struct net_bridge_mcast *brmctx,
						struct net_bridge_mcast_port *pmctx,
						struct net_bridge_port_group *pg,
						struct br_ip *ip_dst,
						struct br_ip *group,
						bool with_srcs, bool over_lmqt,
						u8 sflag, u8 *igmp_type,
						bool *need_rexmit)
{
	__be32 ip4_dst;

	switch (group->proto) {
	case htons(ETH_P_IP):
		ip4_dst = ip_dst ? ip_dst->dst.ip4 : htonl(INADDR_ALLHOSTS_GROUP);
		return br_ip4_multicast_alloc_query(brmctx, pmctx, pg,
						    ip4_dst, group->dst.ip4,
						    with_srcs, over_lmqt,
						    sflag, igmp_type,
						    need_rexmit);
#if IS_ENABLED(CONFIG_IPV6)
	case htons(ETH_P_IPV6): {
		struct in6_addr ip6_dst;

		if (ip_dst)
			ip6_dst = ip_dst->dst.ip6;
		else
			ipv6_addr_set(&ip6_dst, htonl(0xff020000), 0, 0,
				      htonl(1));

		return br_ip6_multicast_alloc_query(brmctx, pmctx, pg,
						    &ip6_dst, &group->dst.ip6,
						    with_srcs, over_lmqt,
						    sflag, igmp_type,
						    need_rexmit);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
	}
	return NULL;
}

<<<<<<< HEAD
static struct net_bridge_mdb_entry *br_multicast_get_group(
	struct net_bridge *br, struct net_bridge_port *port,
	struct br_ip *group, int hash)
{
	struct net_bridge_mdb_htable *mdb;
	struct net_bridge_mdb_entry *mp;
	struct hlist_node *p;
	unsigned count = 0;
	unsigned max;
	int elasticity;
	int err;

	mdb = rcu_dereference_protected(br->mdb, 1);
	hlist_for_each_entry(mp, p, &mdb->mhash[hash], hlist[mdb->ver]) {
		count++;
		if (unlikely(br_ip_equal(group, &mp->addr)))
			return mp;
	}

	elasticity = 0;
	max = mdb->max;

	if (unlikely(count > br->hash_elasticity && count)) {
		if (net_ratelimit())
			br_info(br, "Multicast hash table "
				"chain limit reached: %s\n",
				port ? port->dev->name : br->dev->name);

		elasticity = br->hash_elasticity;
	}

	if (mdb->size >= max) {
		max *= 2;
		if (unlikely(max >= br->hash_max)) {
			br_warn(br, "Multicast hash table maximum "
				"reached, disabling snooping: %s, %d\n",
				port ? port->dev->name : br->dev->name, max);
			err = -E2BIG;
disable:
			br->multicast_disabled = 1;
			goto err;
		}
	}

	if (max > mdb->max || elasticity) {
		if (mdb->old) {
			if (net_ratelimit())
				br_info(br, "Multicast hash table "
					"on fire: %s\n",
					port ? port->dev->name : br->dev->name);
			err = -EEXIST;
			goto err;
		}

		err = br_mdb_rehash(&br->mdb, max, elasticity);
		if (err) {
			br_warn(br, "Cannot rehash multicast "
				"hash table, disabling snooping: %s, %d, %d\n",
				port ? port->dev->name : br->dev->name,
				mdb->size, err);
			goto disable;
		}

		err = -EAGAIN;
		goto err;
	}

	return NULL;

err:
	mp = ERR_PTR(err);
	return mp;
}

static struct net_bridge_mdb_entry *br_multicast_new_group(
	struct net_bridge *br, struct net_bridge_port *port,
	struct br_ip *group)
{
	struct net_bridge_mdb_htable *mdb;
	struct net_bridge_mdb_entry *mp;
	int hash;
	int err;

	mdb = rcu_dereference_protected(br->mdb, 1);
	if (!mdb) {
		err = br_mdb_rehash(&br->mdb, BR_HASH_SIZE, 0);
		if (err)
			return ERR_PTR(err);
		goto rehash;
	}

	hash = br_ip_hash(mdb, group);
	mp = br_multicast_get_group(br, port, group, hash);
	switch (PTR_ERR(mp)) {
	case 0:
		break;

	case -EAGAIN:
rehash:
		mdb = rcu_dereference_protected(br->mdb, 1);
		hash = br_ip_hash(mdb, group);
		break;

	default:
		goto out;
=======
struct net_bridge_mdb_entry *br_multicast_new_group(struct net_bridge *br,
						    struct br_ip *group)
{
	struct net_bridge_mdb_entry *mp;
	int err;

	mp = br_mdb_ip_get(br, group);
	if (mp)
		return mp;

	if (atomic_read(&br->mdb_hash_tbl.nelems) >= br->hash_max) {
		trace_br_mdb_full(br->dev, group);
		br_mc_disabled_update(br->dev, false, NULL);
		br_opt_toggle(br, BROPT_MULTICAST_ENABLED, false);
		return ERR_PTR(-E2BIG);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	mp = kzalloc(sizeof(*mp), GFP_ATOMIC);
	if (unlikely(!mp))
		return ERR_PTR(-ENOMEM);

	mp->br = br;
	mp->addr = *group;
<<<<<<< HEAD
	setup_timer(&mp->timer, br_multicast_group_expired,
		    (unsigned long)mp);

	hlist_add_head_rcu(&mp->hlist[mdb->ver], &mdb->mhash[hash]);
	mdb->size++;

out:
	return mp;
}

static int br_multicast_add_group(struct net_bridge *br,
				  struct net_bridge_port *port,
				  struct br_ip *group)
{
	struct net_bridge_mdb_entry *mp;
	struct net_bridge_port_group *p;
	struct net_bridge_port_group __rcu **pp;
	unsigned long now = jiffies;
	int err;

	spin_lock(&br->multicast_lock);
	if (!netif_running(br->dev) ||
	    (port && port->state == BR_STATE_DISABLED))
		goto out;

	mp = br_multicast_new_group(br, port, group);
	err = PTR_ERR(mp);
	if (IS_ERR(mp))
		goto err;

	if (!port) {
		mp->mglist = true;
		mod_timer(&mp->timer, now + br->multicast_membership_interval);
=======
	mp->mcast_gc.destroy = br_multicast_destroy_mdb_entry;
	timer_setup(&mp->timer, br_multicast_group_expired, 0);
	err = rhashtable_lookup_insert_fast(&br->mdb_hash_tbl, &mp->rhnode,
					    br_mdb_rht_params);
	if (err) {
		kfree(mp);
		mp = ERR_PTR(err);
	} else {
		hlist_add_head_rcu(&mp->mdb_node, &br->mdb_list);
	}

	return mp;
}

static void br_multicast_group_src_expired(struct timer_list *t)
{
	struct net_bridge_group_src *src = from_timer(src, t, timer);
	struct net_bridge_port_group *pg;
	struct net_bridge *br = src->br;

	spin_lock(&br->multicast_lock);
	if (hlist_unhashed(&src->node) || !netif_running(br->dev) ||
	    timer_pending(&src->timer))
		goto out;

	pg = src->pg;
	if (pg->filter_mode == MCAST_INCLUDE) {
		br_multicast_del_group_src(src, false);
		if (!hlist_empty(&pg->src_list))
			goto out;
		br_multicast_find_del_pg(br, pg);
	} else {
		br_multicast_fwd_src_handle(src);
	}

out:
	spin_unlock(&br->multicast_lock);
}

struct net_bridge_group_src *
br_multicast_find_group_src(struct net_bridge_port_group *pg, struct br_ip *ip)
{
	struct net_bridge_group_src *ent;

	switch (ip->proto) {
	case htons(ETH_P_IP):
		hlist_for_each_entry(ent, &pg->src_list, node)
			if (ip->src.ip4 == ent->addr.src.ip4)
				return ent;
		break;
#if IS_ENABLED(CONFIG_IPV6)
	case htons(ETH_P_IPV6):
		hlist_for_each_entry(ent, &pg->src_list, node)
			if (!ipv6_addr_cmp(&ent->addr.src.ip6, &ip->src.ip6))
				return ent;
		break;
#endif
	}

	return NULL;
}

struct net_bridge_group_src *
br_multicast_new_group_src(struct net_bridge_port_group *pg, struct br_ip *src_ip)
{
	struct net_bridge_group_src *grp_src;

	if (unlikely(pg->src_ents >= PG_SRC_ENT_LIMIT))
		return NULL;

	switch (src_ip->proto) {
	case htons(ETH_P_IP):
		if (ipv4_is_zeronet(src_ip->src.ip4) ||
		    ipv4_is_multicast(src_ip->src.ip4))
			return NULL;
		break;
#if IS_ENABLED(CONFIG_IPV6)
	case htons(ETH_P_IPV6):
		if (ipv6_addr_any(&src_ip->src.ip6) ||
		    ipv6_addr_is_multicast(&src_ip->src.ip6))
			return NULL;
		break;
#endif
	}

	grp_src = kzalloc(sizeof(*grp_src), GFP_ATOMIC);
	if (unlikely(!grp_src))
		return NULL;

	grp_src->pg = pg;
	grp_src->br = pg->key.port->br;
	grp_src->addr = *src_ip;
	grp_src->mcast_gc.destroy = br_multicast_destroy_group_src;
	timer_setup(&grp_src->timer, br_multicast_group_src_expired, 0);

	hlist_add_head_rcu(&grp_src->node, &pg->src_list);
	pg->src_ents++;

	return grp_src;
}

struct net_bridge_port_group *br_multicast_new_port_group(
			struct net_bridge_port *port,
			const struct br_ip *group,
			struct net_bridge_port_group __rcu *next,
			unsigned char flags,
			const unsigned char *src,
			u8 filter_mode,
			u8 rt_protocol,
			struct netlink_ext_ack *extack)
{
	struct net_bridge_port_group *p;
	int err;

	err = br_multicast_port_ngroups_inc(port, group, extack);
	if (err)
		return NULL;

	p = kzalloc(sizeof(*p), GFP_ATOMIC);
	if (unlikely(!p)) {
		NL_SET_ERR_MSG_MOD(extack, "Couldn't allocate new port group");
		goto dec_out;
	}

	p->key.addr = *group;
	p->key.port = port;
	p->flags = flags;
	p->filter_mode = filter_mode;
	p->rt_protocol = rt_protocol;
	p->eht_host_tree = RB_ROOT;
	p->eht_set_tree = RB_ROOT;
	p->mcast_gc.destroy = br_multicast_destroy_port_group;
	INIT_HLIST_HEAD(&p->src_list);

	if (!br_multicast_is_star_g(group) &&
	    rhashtable_lookup_insert_fast(&port->br->sg_port_tbl, &p->rhnode,
					  br_sg_port_rht_params)) {
		NL_SET_ERR_MSG_MOD(extack, "Couldn't insert new port group");
		goto free_out;
	}

	rcu_assign_pointer(p->next, next);
	timer_setup(&p->timer, br_multicast_port_group_expired, 0);
	timer_setup(&p->rexmit_timer, br_multicast_port_group_rexmit, 0);
	hlist_add_head(&p->mglist, &port->mglist);

	if (src)
		memcpy(p->eth_addr, src, ETH_ALEN);
	else
		eth_broadcast_addr(p->eth_addr);

	return p;

free_out:
	kfree(p);
dec_out:
	br_multicast_port_ngroups_dec(port, group->vid);
	return NULL;
}

void br_multicast_del_port_group(struct net_bridge_port_group *p)
{
	struct net_bridge_port *port = p->key.port;
	__u16 vid = p->key.addr.vid;

	hlist_del_init(&p->mglist);
	if (!br_multicast_is_star_g(&p->key.addr))
		rhashtable_remove_fast(&port->br->sg_port_tbl, &p->rhnode,
				       br_sg_port_rht_params);
	kfree(p);
	br_multicast_port_ngroups_dec(port, vid);
}

void br_multicast_host_join(const struct net_bridge_mcast *brmctx,
			    struct net_bridge_mdb_entry *mp, bool notify)
{
	if (!mp->host_joined) {
		mp->host_joined = true;
		if (br_multicast_is_star_g(&mp->addr))
			br_multicast_star_g_host_state(mp);
		if (notify)
			br_mdb_notify(mp->br->dev, mp, NULL, RTM_NEWMDB);
	}

	if (br_group_is_l2(&mp->addr))
		return;

	mod_timer(&mp->timer, jiffies + brmctx->multicast_membership_interval);
}

void br_multicast_host_leave(struct net_bridge_mdb_entry *mp, bool notify)
{
	if (!mp->host_joined)
		return;

	mp->host_joined = false;
	if (br_multicast_is_star_g(&mp->addr))
		br_multicast_star_g_host_state(mp);
	if (notify)
		br_mdb_notify(mp->br->dev, mp, NULL, RTM_DELMDB);
}

static struct net_bridge_port_group *
__br_multicast_add_group(struct net_bridge_mcast *brmctx,
			 struct net_bridge_mcast_port *pmctx,
			 struct br_ip *group,
			 const unsigned char *src,
			 u8 filter_mode,
			 bool igmpv2_mldv1,
			 bool blocked)
{
	struct net_bridge_port_group __rcu **pp;
	struct net_bridge_port_group *p = NULL;
	struct net_bridge_mdb_entry *mp;
	unsigned long now = jiffies;

	if (!br_multicast_ctx_should_use(brmctx, pmctx))
		goto out;

	mp = br_multicast_new_group(brmctx->br, group);
	if (IS_ERR(mp))
		return ERR_CAST(mp);

	if (!pmctx) {
		br_multicast_host_join(brmctx, mp, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;
	}

	for (pp = &mp->ports;
<<<<<<< HEAD
	     (p = mlock_dereference(*pp, br)) != NULL;
	     pp = &p->next) {
		if (p->port == port)
			goto found;
		if ((unsigned long)p->port < (unsigned long)port)
			break;
	}

	p = kzalloc(sizeof(*p), GFP_ATOMIC);
	err = -ENOMEM;
	if (unlikely(!p))
		goto err;

	p->addr = *group;
	p->port = port;
	p->next = *pp;
	hlist_add_head(&p->mglist, &port->mglist);
	setup_timer(&p->timer, br_multicast_port_group_expired,
		    (unsigned long)p);

	rcu_assign_pointer(*pp, p);

found:
	mod_timer(&p->timer, now + br->multicast_membership_interval);
out:
	err = 0;

err:
	spin_unlock(&br->multicast_lock);
	return err;
}

static int br_ip4_multicast_add_group(struct net_bridge *br,
				      struct net_bridge_port *port,
				      __be32 group)
{
	struct br_ip br_group;
=======
	     (p = mlock_dereference(*pp, brmctx->br)) != NULL;
	     pp = &p->next) {
		if (br_port_group_equal(p, pmctx->port, src))
			goto found;
		if ((unsigned long)p->key.port < (unsigned long)pmctx->port)
			break;
	}

	p = br_multicast_new_port_group(pmctx->port, group, *pp, 0, src,
					filter_mode, RTPROT_KERNEL, NULL);
	if (unlikely(!p)) {
		p = ERR_PTR(-ENOMEM);
		goto out;
	}
	rcu_assign_pointer(*pp, p);
	if (blocked)
		p->flags |= MDB_PG_FLAGS_BLOCKED;
	br_mdb_notify(brmctx->br->dev, mp, p, RTM_NEWMDB);

found:
	if (igmpv2_mldv1)
		mod_timer(&p->timer,
			  now + brmctx->multicast_membership_interval);

out:
	return p;
}

static int br_multicast_add_group(struct net_bridge_mcast *brmctx,
				  struct net_bridge_mcast_port *pmctx,
				  struct br_ip *group,
				  const unsigned char *src,
				  u8 filter_mode,
				  bool igmpv2_mldv1)
{
	struct net_bridge_port_group *pg;
	int err;

	spin_lock(&brmctx->br->multicast_lock);
	pg = __br_multicast_add_group(brmctx, pmctx, group, src, filter_mode,
				      igmpv2_mldv1, false);
	/* NULL is considered valid for host joined groups */
	err = PTR_ERR_OR_ZERO(pg);
	spin_unlock(&brmctx->br->multicast_lock);

	return err;
}

static int br_ip4_multicast_add_group(struct net_bridge_mcast *brmctx,
				      struct net_bridge_mcast_port *pmctx,
				      __be32 group,
				      __u16 vid,
				      const unsigned char *src,
				      bool igmpv2)
{
	struct br_ip br_group;
	u8 filter_mode;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (ipv4_is_local_multicast(group))
		return 0;

<<<<<<< HEAD
	br_group.u.ip4 = group;
	br_group.proto = htons(ETH_P_IP);

	return br_multicast_add_group(br, port, &br_group);
}

#if IS_ENABLED(CONFIG_IPV6)
static int br_ip6_multicast_add_group(struct net_bridge *br,
				      struct net_bridge_port *port,
				      const struct in6_addr *group)
{
	struct br_ip br_group;

	if (!ipv6_is_transient_multicast(group))
		return 0;

	br_group.u.ip6 = *group;
	br_group.proto = htons(ETH_P_IPV6);

	return br_multicast_add_group(br, port, &br_group);
}
#endif

static void br_multicast_router_expired(unsigned long data)
{
	struct net_bridge_port *port = (void *)data;
	struct net_bridge *br = port->br;

	spin_lock(&br->multicast_lock);
	if (port->multicast_router != 1 ||
	    timer_pending(&port->multicast_router_timer) ||
	    hlist_unhashed(&port->rlist))
		goto out;

	hlist_del_init_rcu(&port->rlist);

=======
	memset(&br_group, 0, sizeof(br_group));
	br_group.dst.ip4 = group;
	br_group.proto = htons(ETH_P_IP);
	br_group.vid = vid;
	filter_mode = igmpv2 ? MCAST_EXCLUDE : MCAST_INCLUDE;

	return br_multicast_add_group(brmctx, pmctx, &br_group, src,
				      filter_mode, igmpv2);
}

#if IS_ENABLED(CONFIG_IPV6)
static int br_ip6_multicast_add_group(struct net_bridge_mcast *brmctx,
				      struct net_bridge_mcast_port *pmctx,
				      const struct in6_addr *group,
				      __u16 vid,
				      const unsigned char *src,
				      bool mldv1)
{
	struct br_ip br_group;
	u8 filter_mode;

	if (ipv6_addr_is_ll_all_nodes(group))
		return 0;

	memset(&br_group, 0, sizeof(br_group));
	br_group.dst.ip6 = *group;
	br_group.proto = htons(ETH_P_IPV6);
	br_group.vid = vid;
	filter_mode = mldv1 ? MCAST_EXCLUDE : MCAST_INCLUDE;

	return br_multicast_add_group(brmctx, pmctx, &br_group, src,
				      filter_mode, mldv1);
}
#endif

static bool br_multicast_rport_del(struct hlist_node *rlist)
{
	if (hlist_unhashed(rlist))
		return false;

	hlist_del_init_rcu(rlist);
	return true;
}

static bool br_ip4_multicast_rport_del(struct net_bridge_mcast_port *pmctx)
{
	return br_multicast_rport_del(&pmctx->ip4_rlist);
}

static bool br_ip6_multicast_rport_del(struct net_bridge_mcast_port *pmctx)
{
#if IS_ENABLED(CONFIG_IPV6)
	return br_multicast_rport_del(&pmctx->ip6_rlist);
#else
	return false;
#endif
}

static void br_multicast_router_expired(struct net_bridge_mcast_port *pmctx,
					struct timer_list *t,
					struct hlist_node *rlist)
{
	struct net_bridge *br = pmctx->port->br;
	bool del;

	spin_lock(&br->multicast_lock);
	if (pmctx->multicast_router == MDB_RTR_TYPE_DISABLED ||
	    pmctx->multicast_router == MDB_RTR_TYPE_PERM ||
	    timer_pending(t))
		goto out;

	del = br_multicast_rport_del(rlist);
	br_multicast_rport_del_notify(pmctx, del);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	spin_unlock(&br->multicast_lock);
}

<<<<<<< HEAD
static void br_multicast_local_router_expired(unsigned long data)
{
}

static void __br_multicast_send_query(struct net_bridge *br,
				      struct net_bridge_port *port,
				      struct br_ip *ip)
{
	struct sk_buff *skb;

	skb = br_multicast_alloc_query(br, ip);
	if (!skb)
		return;

	if (port) {
		__skb_push(skb, sizeof(struct ethhdr));
		skb->dev = port->dev;
		NF_HOOK(NFPROTO_BRIDGE, NF_BR_LOCAL_OUT, skb, NULL, skb->dev,
			dev_queue_xmit);
	} else
		netif_rx(skb);
}

static void br_multicast_send_query(struct net_bridge *br,
				    struct net_bridge_port *port, u32 sent)
{
	unsigned long time;
	struct br_ip br_group;

	if (!netif_running(br->dev) || br->multicast_disabled ||
	    timer_pending(&br->multicast_querier_timer))
		return;

	memset(&br_group.u, 0, sizeof(br_group.u));

	br_group.proto = htons(ETH_P_IP);
	__br_multicast_send_query(br, port, &br_group);

#if IS_ENABLED(CONFIG_IPV6)
	br_group.proto = htons(ETH_P_IPV6);
	__br_multicast_send_query(br, port, &br_group);
#endif

	time = jiffies;
	time += sent < br->multicast_startup_query_count ?
		br->multicast_startup_query_interval :
		br->multicast_query_interval;
	mod_timer(port ? &port->multicast_query_timer :
			 &br->multicast_query_timer, time);
}

static void br_multicast_port_query_expired(unsigned long data)
{
	struct net_bridge_port *port = (void *)data;
	struct net_bridge *br = port->br;

	spin_lock(&br->multicast_lock);
	if (port->state == BR_STATE_DISABLED ||
	    port->state == BR_STATE_BLOCKING)
		goto out;

	if (port->multicast_startup_queries_sent <
	    br->multicast_startup_query_count)
		port->multicast_startup_queries_sent++;

	br_multicast_send_query(port->br, port,
				port->multicast_startup_queries_sent);
=======
static void br_ip4_multicast_router_expired(struct timer_list *t)
{
	struct net_bridge_mcast_port *pmctx = from_timer(pmctx, t,
							 ip4_mc_router_timer);

	br_multicast_router_expired(pmctx, t, &pmctx->ip4_rlist);
}

#if IS_ENABLED(CONFIG_IPV6)
static void br_ip6_multicast_router_expired(struct timer_list *t)
{
	struct net_bridge_mcast_port *pmctx = from_timer(pmctx, t,
							 ip6_mc_router_timer);

	br_multicast_router_expired(pmctx, t, &pmctx->ip6_rlist);
}
#endif

static void br_mc_router_state_change(struct net_bridge *p,
				      bool is_mc_router)
{
	struct switchdev_attr attr = {
		.orig_dev = p->dev,
		.id = SWITCHDEV_ATTR_ID_BRIDGE_MROUTER,
		.flags = SWITCHDEV_F_DEFER,
		.u.mrouter = is_mc_router,
	};

	switchdev_port_attr_set(p->dev, &attr, NULL);
}

static void br_multicast_local_router_expired(struct net_bridge_mcast *brmctx,
					      struct timer_list *timer)
{
	spin_lock(&brmctx->br->multicast_lock);
	if (brmctx->multicast_router == MDB_RTR_TYPE_DISABLED ||
	    brmctx->multicast_router == MDB_RTR_TYPE_PERM ||
	    br_ip4_multicast_is_router(brmctx) ||
	    br_ip6_multicast_is_router(brmctx))
		goto out;

	br_mc_router_state_change(brmctx->br, false);
out:
	spin_unlock(&brmctx->br->multicast_lock);
}

static void br_ip4_multicast_local_router_expired(struct timer_list *t)
{
	struct net_bridge_mcast *brmctx = from_timer(brmctx, t,
						     ip4_mc_router_timer);

	br_multicast_local_router_expired(brmctx, t);
}

#if IS_ENABLED(CONFIG_IPV6)
static void br_ip6_multicast_local_router_expired(struct timer_list *t)
{
	struct net_bridge_mcast *brmctx = from_timer(brmctx, t,
						     ip6_mc_router_timer);

	br_multicast_local_router_expired(brmctx, t);
}
#endif

static void br_multicast_querier_expired(struct net_bridge_mcast *brmctx,
					 struct bridge_mcast_own_query *query)
{
	spin_lock(&brmctx->br->multicast_lock);
	if (!netif_running(brmctx->br->dev) ||
	    br_multicast_ctx_vlan_global_disabled(brmctx) ||
	    !br_opt_get(brmctx->br, BROPT_MULTICAST_ENABLED))
		goto out;

	br_multicast_start_querier(brmctx, query);

out:
	spin_unlock(&brmctx->br->multicast_lock);
}

static void br_ip4_multicast_querier_expired(struct timer_list *t)
{
	struct net_bridge_mcast *brmctx = from_timer(brmctx, t,
						     ip4_other_query.timer);

	br_multicast_querier_expired(brmctx, &brmctx->ip4_own_query);
}

#if IS_ENABLED(CONFIG_IPV6)
static void br_ip6_multicast_querier_expired(struct timer_list *t)
{
	struct net_bridge_mcast *brmctx = from_timer(brmctx, t,
						     ip6_other_query.timer);

	br_multicast_querier_expired(brmctx, &brmctx->ip6_own_query);
}
#endif

static void br_multicast_query_delay_expired(struct timer_list *t)
{
}

static void br_multicast_select_own_querier(struct net_bridge_mcast *brmctx,
					    struct br_ip *ip,
					    struct sk_buff *skb)
{
	if (ip->proto == htons(ETH_P_IP))
		brmctx->ip4_querier.addr.src.ip4 = ip_hdr(skb)->saddr;
#if IS_ENABLED(CONFIG_IPV6)
	else
		brmctx->ip6_querier.addr.src.ip6 = ipv6_hdr(skb)->saddr;
#endif
}

static void __br_multicast_send_query(struct net_bridge_mcast *brmctx,
				      struct net_bridge_mcast_port *pmctx,
				      struct net_bridge_port_group *pg,
				      struct br_ip *ip_dst,
				      struct br_ip *group,
				      bool with_srcs,
				      u8 sflag,
				      bool *need_rexmit)
{
	bool over_lmqt = !!sflag;
	struct sk_buff *skb;
	u8 igmp_type;

	if (!br_multicast_ctx_should_use(brmctx, pmctx) ||
	    !br_multicast_ctx_matches_vlan_snooping(brmctx))
		return;

again_under_lmqt:
	skb = br_multicast_alloc_query(brmctx, pmctx, pg, ip_dst, group,
				       with_srcs, over_lmqt, sflag, &igmp_type,
				       need_rexmit);
	if (!skb)
		return;

	if (pmctx) {
		skb->dev = pmctx->port->dev;
		br_multicast_count(brmctx->br, pmctx->port, skb, igmp_type,
				   BR_MCAST_DIR_TX);
		NF_HOOK(NFPROTO_BRIDGE, NF_BR_LOCAL_OUT,
			dev_net(pmctx->port->dev), NULL, skb, NULL, skb->dev,
			br_dev_queue_push_xmit);

		if (over_lmqt && with_srcs && sflag) {
			over_lmqt = false;
			goto again_under_lmqt;
		}
	} else {
		br_multicast_select_own_querier(brmctx, group, skb);
		br_multicast_count(brmctx->br, NULL, skb, igmp_type,
				   BR_MCAST_DIR_RX);
		netif_rx(skb);
	}
}

static void br_multicast_read_querier(const struct bridge_mcast_querier *querier,
				      struct bridge_mcast_querier *dest)
{
	unsigned int seq;

	memset(dest, 0, sizeof(*dest));
	do {
		seq = read_seqcount_begin(&querier->seq);
		dest->port_ifidx = querier->port_ifidx;
		memcpy(&dest->addr, &querier->addr, sizeof(struct br_ip));
	} while (read_seqcount_retry(&querier->seq, seq));
}

static void br_multicast_update_querier(struct net_bridge_mcast *brmctx,
					struct bridge_mcast_querier *querier,
					int ifindex,
					struct br_ip *saddr)
{
	write_seqcount_begin(&querier->seq);
	querier->port_ifidx = ifindex;
	memcpy(&querier->addr, saddr, sizeof(*saddr));
	write_seqcount_end(&querier->seq);
}

static void br_multicast_send_query(struct net_bridge_mcast *brmctx,
				    struct net_bridge_mcast_port *pmctx,
				    struct bridge_mcast_own_query *own_query)
{
	struct bridge_mcast_other_query *other_query = NULL;
	struct bridge_mcast_querier *querier;
	struct br_ip br_group;
	unsigned long time;

	if (!br_multicast_ctx_should_use(brmctx, pmctx) ||
	    !br_opt_get(brmctx->br, BROPT_MULTICAST_ENABLED) ||
	    !brmctx->multicast_querier)
		return;

	memset(&br_group.dst, 0, sizeof(br_group.dst));

	if (pmctx ? (own_query == &pmctx->ip4_own_query) :
		    (own_query == &brmctx->ip4_own_query)) {
		querier = &brmctx->ip4_querier;
		other_query = &brmctx->ip4_other_query;
		br_group.proto = htons(ETH_P_IP);
#if IS_ENABLED(CONFIG_IPV6)
	} else {
		querier = &brmctx->ip6_querier;
		other_query = &brmctx->ip6_other_query;
		br_group.proto = htons(ETH_P_IPV6);
#endif
	}

	if (!other_query || timer_pending(&other_query->timer))
		return;

	/* we're about to select ourselves as querier */
	if (!pmctx && querier->port_ifidx) {
		struct br_ip zeroip = {};

		br_multicast_update_querier(brmctx, querier, 0, &zeroip);
	}

	__br_multicast_send_query(brmctx, pmctx, NULL, NULL, &br_group, false,
				  0, NULL);

	time = jiffies;
	time += own_query->startup_sent < brmctx->multicast_startup_query_count ?
		brmctx->multicast_startup_query_interval :
		brmctx->multicast_query_interval;
	mod_timer(&own_query->timer, time);
}

static void
br_multicast_port_query_expired(struct net_bridge_mcast_port *pmctx,
				struct bridge_mcast_own_query *query)
{
	struct net_bridge *br = pmctx->port->br;
	struct net_bridge_mcast *brmctx;

	spin_lock(&br->multicast_lock);
	if (br_multicast_port_ctx_state_stopped(pmctx))
		goto out;

	brmctx = br_multicast_port_ctx_get_global(pmctx);
	if (query->startup_sent < brmctx->multicast_startup_query_count)
		query->startup_sent++;

	br_multicast_send_query(brmctx, pmctx, query);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

out:
	spin_unlock(&br->multicast_lock);
}

<<<<<<< HEAD
void br_multicast_add_port(struct net_bridge_port *port)
{
	port->multicast_router = 1;

	setup_timer(&port->multicast_router_timer, br_multicast_router_expired,
		    (unsigned long)port);
	setup_timer(&port->multicast_query_timer,
		    br_multicast_port_query_expired, (unsigned long)port);
=======
static void br_ip4_multicast_port_query_expired(struct timer_list *t)
{
	struct net_bridge_mcast_port *pmctx = from_timer(pmctx, t,
							 ip4_own_query.timer);

	br_multicast_port_query_expired(pmctx, &pmctx->ip4_own_query);
}

#if IS_ENABLED(CONFIG_IPV6)
static void br_ip6_multicast_port_query_expired(struct timer_list *t)
{
	struct net_bridge_mcast_port *pmctx = from_timer(pmctx, t,
							 ip6_own_query.timer);

	br_multicast_port_query_expired(pmctx, &pmctx->ip6_own_query);
}
#endif

static void br_multicast_port_group_rexmit(struct timer_list *t)
{
	struct net_bridge_port_group *pg = from_timer(pg, t, rexmit_timer);
	struct bridge_mcast_other_query *other_query = NULL;
	struct net_bridge *br = pg->key.port->br;
	struct net_bridge_mcast_port *pmctx;
	struct net_bridge_mcast *brmctx;
	bool need_rexmit = false;

	spin_lock(&br->multicast_lock);
	if (!netif_running(br->dev) || hlist_unhashed(&pg->mglist) ||
	    !br_opt_get(br, BROPT_MULTICAST_ENABLED))
		goto out;

	pmctx = br_multicast_pg_to_port_ctx(pg);
	if (!pmctx)
		goto out;
	brmctx = br_multicast_port_ctx_get_global(pmctx);
	if (!brmctx->multicast_querier)
		goto out;

	if (pg->key.addr.proto == htons(ETH_P_IP))
		other_query = &brmctx->ip4_other_query;
#if IS_ENABLED(CONFIG_IPV6)
	else
		other_query = &brmctx->ip6_other_query;
#endif

	if (!other_query || timer_pending(&other_query->timer))
		goto out;

	if (pg->grp_query_rexmit_cnt) {
		pg->grp_query_rexmit_cnt--;
		__br_multicast_send_query(brmctx, pmctx, pg, &pg->key.addr,
					  &pg->key.addr, false, 1, NULL);
	}
	__br_multicast_send_query(brmctx, pmctx, pg, &pg->key.addr,
				  &pg->key.addr, true, 0, &need_rexmit);

	if (pg->grp_query_rexmit_cnt || need_rexmit)
		mod_timer(&pg->rexmit_timer, jiffies +
					     brmctx->multicast_last_member_interval);
out:
	spin_unlock(&br->multicast_lock);
}

static int br_mc_disabled_update(struct net_device *dev, bool value,
				 struct netlink_ext_ack *extack)
{
	struct switchdev_attr attr = {
		.orig_dev = dev,
		.id = SWITCHDEV_ATTR_ID_BRIDGE_MC_DISABLED,
		.flags = SWITCHDEV_F_DEFER,
		.u.mc_disabled = !value,
	};

	return switchdev_port_attr_set(dev, &attr, extack);
}

void br_multicast_port_ctx_init(struct net_bridge_port *port,
				struct net_bridge_vlan *vlan,
				struct net_bridge_mcast_port *pmctx)
{
	pmctx->port = port;
	pmctx->vlan = vlan;
	pmctx->multicast_router = MDB_RTR_TYPE_TEMP_QUERY;
	timer_setup(&pmctx->ip4_mc_router_timer,
		    br_ip4_multicast_router_expired, 0);
	timer_setup(&pmctx->ip4_own_query.timer,
		    br_ip4_multicast_port_query_expired, 0);
#if IS_ENABLED(CONFIG_IPV6)
	timer_setup(&pmctx->ip6_mc_router_timer,
		    br_ip6_multicast_router_expired, 0);
	timer_setup(&pmctx->ip6_own_query.timer,
		    br_ip6_multicast_port_query_expired, 0);
#endif
}

void br_multicast_port_ctx_deinit(struct net_bridge_mcast_port *pmctx)
{
#if IS_ENABLED(CONFIG_IPV6)
	del_timer_sync(&pmctx->ip6_mc_router_timer);
#endif
	del_timer_sync(&pmctx->ip4_mc_router_timer);
}

int br_multicast_add_port(struct net_bridge_port *port)
{
	int err;

	port->multicast_eht_hosts_limit = BR_MCAST_DEFAULT_EHT_HOSTS_LIMIT;
	br_multicast_port_ctx_init(port, NULL, &port->multicast_ctx);

	err = br_mc_disabled_update(port->dev,
				    br_opt_get(port->br,
					       BROPT_MULTICAST_ENABLED),
				    NULL);
	if (err && err != -EOPNOTSUPP)
		return err;

	port->mcast_stats = netdev_alloc_pcpu_stats(struct bridge_mcast_stats);
	if (!port->mcast_stats)
		return -ENOMEM;

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void br_multicast_del_port(struct net_bridge_port *port)
{
<<<<<<< HEAD
	del_timer_sync(&port->multicast_router_timer);
}

static void __br_multicast_enable_port(struct net_bridge_port *port)
{
	port->multicast_startup_queries_sent = 0;

	if (try_to_del_timer_sync(&port->multicast_query_timer) >= 0 ||
	    del_timer(&port->multicast_query_timer))
		mod_timer(&port->multicast_query_timer, jiffies);
=======
	struct net_bridge *br = port->br;
	struct net_bridge_port_group *pg;
	HLIST_HEAD(deleted_head);
	struct hlist_node *n;

	/* Take care of the remaining groups, only perm ones should be left */
	spin_lock_bh(&br->multicast_lock);
	hlist_for_each_entry_safe(pg, n, &port->mglist, mglist)
		br_multicast_find_del_pg(br, pg);
	hlist_move_list(&br->mcast_gc_list, &deleted_head);
	spin_unlock_bh(&br->multicast_lock);
	br_multicast_gc(&deleted_head);
	br_multicast_port_ctx_deinit(&port->multicast_ctx);
	free_percpu(port->mcast_stats);
}

static void br_multicast_enable(struct bridge_mcast_own_query *query)
{
	query->startup_sent = 0;

	if (try_to_del_timer_sync(&query->timer) >= 0 ||
	    del_timer(&query->timer))
		mod_timer(&query->timer, jiffies);
}

static void __br_multicast_enable_port_ctx(struct net_bridge_mcast_port *pmctx)
{
	struct net_bridge *br = pmctx->port->br;
	struct net_bridge_mcast *brmctx;

	brmctx = br_multicast_port_ctx_get_global(pmctx);
	if (!br_opt_get(br, BROPT_MULTICAST_ENABLED) ||
	    !netif_running(br->dev))
		return;

	br_multicast_enable(&pmctx->ip4_own_query);
#if IS_ENABLED(CONFIG_IPV6)
	br_multicast_enable(&pmctx->ip6_own_query);
#endif
	if (pmctx->multicast_router == MDB_RTR_TYPE_PERM) {
		br_ip4_multicast_add_router(brmctx, pmctx);
		br_ip6_multicast_add_router(brmctx, pmctx);
	}

	if (br_multicast_port_ctx_is_vlan(pmctx)) {
		struct net_bridge_port_group *pg;
		u32 n = 0;

		/* The mcast_n_groups counter might be wrong. First,
		 * BR_VLFLAG_MCAST_ENABLED is toggled before temporary entries
		 * are flushed, thus mcast_n_groups after the toggle does not
		 * reflect the true values. And second, permanent entries added
		 * while BR_VLFLAG_MCAST_ENABLED was disabled, are not reflected
		 * either. Thus we have to refresh the counter.
		 */

		hlist_for_each_entry(pg, &pmctx->port->mglist, mglist) {
			if (pg->key.addr.vid == pmctx->vlan->vid)
				n++;
		}
		WRITE_ONCE(pmctx->mdb_n_entries, n);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void br_multicast_enable_port(struct net_bridge_port *port)
{
	struct net_bridge *br = port->br;

<<<<<<< HEAD
	spin_lock(&br->multicast_lock);
	if (br->multicast_disabled || !netif_running(br->dev))
		goto out;

	__br_multicast_enable_port(port);
	if (port->multicast_router == 2 && hlist_unhashed(&port->rlist))
		br_multicast_add_router(br, port);

out:
	spin_unlock(&br->multicast_lock);
=======
	spin_lock_bh(&br->multicast_lock);
	__br_multicast_enable_port_ctx(&port->multicast_ctx);
	spin_unlock_bh(&br->multicast_lock);
}

static void __br_multicast_disable_port_ctx(struct net_bridge_mcast_port *pmctx)
{
	struct net_bridge_port_group *pg;
	struct hlist_node *n;
	bool del = false;

	hlist_for_each_entry_safe(pg, n, &pmctx->port->mglist, mglist)
		if (!(pg->flags & MDB_PG_FLAGS_PERMANENT) &&
		    (!br_multicast_port_ctx_is_vlan(pmctx) ||
		     pg->key.addr.vid == pmctx->vlan->vid))
			br_multicast_find_del_pg(pmctx->port->br, pg);

	del |= br_ip4_multicast_rport_del(pmctx);
	del_timer(&pmctx->ip4_mc_router_timer);
	del_timer(&pmctx->ip4_own_query.timer);
	del |= br_ip6_multicast_rport_del(pmctx);
#if IS_ENABLED(CONFIG_IPV6)
	del_timer(&pmctx->ip6_mc_router_timer);
	del_timer(&pmctx->ip6_own_query.timer);
#endif
	br_multicast_rport_del_notify(pmctx, del);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void br_multicast_disable_port(struct net_bridge_port *port)
{
<<<<<<< HEAD
	struct net_bridge *br = port->br;
	struct net_bridge_port_group *pg;
	struct hlist_node *p, *n;

	spin_lock(&br->multicast_lock);
	hlist_for_each_entry_safe(pg, p, n, &port->mglist, mglist)
		br_multicast_del_pg(br, pg);

	if (!hlist_unhashed(&port->rlist))
		hlist_del_init_rcu(&port->rlist);
	del_timer(&port->multicast_router_timer);
	del_timer(&port->multicast_query_timer);
	spin_unlock(&br->multicast_lock);
}

static int br_ip4_multicast_igmp3_report(struct net_bridge *br,
					 struct net_bridge_port *port,
					 struct sk_buff *skb)
{
	struct igmpv3_report *ih;
	struct igmpv3_grec *grec;
	int i;
	int len;
	int num;
	int type;
	int err = 0;
	__be32 group;

	if (!pskb_may_pull(skb, sizeof(*ih)))
		return -EINVAL;

	ih = igmpv3_report_hdr(skb);
	num = ntohs(ih->ngrec);
	len = sizeof(*ih);

	for (i = 0; i < num; i++) {
		len += sizeof(*grec);
		if (!pskb_may_pull(skb, len))
=======
	spin_lock_bh(&port->br->multicast_lock);
	__br_multicast_disable_port_ctx(&port->multicast_ctx);
	spin_unlock_bh(&port->br->multicast_lock);
}

static int __grp_src_delete_marked(struct net_bridge_port_group *pg)
{
	struct net_bridge_group_src *ent;
	struct hlist_node *tmp;
	int deleted = 0;

	hlist_for_each_entry_safe(ent, tmp, &pg->src_list, node)
		if (ent->flags & BR_SGRP_F_DELETE) {
			br_multicast_del_group_src(ent, false);
			deleted++;
		}

	return deleted;
}

static void __grp_src_mod_timer(struct net_bridge_group_src *src,
				unsigned long expires)
{
	mod_timer(&src->timer, expires);
	br_multicast_fwd_src_handle(src);
}

static void __grp_src_query_marked_and_rexmit(struct net_bridge_mcast *brmctx,
					      struct net_bridge_mcast_port *pmctx,
					      struct net_bridge_port_group *pg)
{
	struct bridge_mcast_other_query *other_query = NULL;
	u32 lmqc = brmctx->multicast_last_member_count;
	unsigned long lmqt, lmi, now = jiffies;
	struct net_bridge_group_src *ent;

	if (!netif_running(brmctx->br->dev) ||
	    !br_opt_get(brmctx->br, BROPT_MULTICAST_ENABLED))
		return;

	if (pg->key.addr.proto == htons(ETH_P_IP))
		other_query = &brmctx->ip4_other_query;
#if IS_ENABLED(CONFIG_IPV6)
	else
		other_query = &brmctx->ip6_other_query;
#endif

	lmqt = now + br_multicast_lmqt(brmctx);
	hlist_for_each_entry(ent, &pg->src_list, node) {
		if (ent->flags & BR_SGRP_F_SEND) {
			ent->flags &= ~BR_SGRP_F_SEND;
			if (ent->timer.expires > lmqt) {
				if (brmctx->multicast_querier &&
				    other_query &&
				    !timer_pending(&other_query->timer))
					ent->src_query_rexmit_cnt = lmqc;
				__grp_src_mod_timer(ent, lmqt);
			}
		}
	}

	if (!brmctx->multicast_querier ||
	    !other_query || timer_pending(&other_query->timer))
		return;

	__br_multicast_send_query(brmctx, pmctx, pg, &pg->key.addr,
				  &pg->key.addr, true, 1, NULL);

	lmi = now + brmctx->multicast_last_member_interval;
	if (!timer_pending(&pg->rexmit_timer) ||
	    time_after(pg->rexmit_timer.expires, lmi))
		mod_timer(&pg->rexmit_timer, lmi);
}

static void __grp_send_query_and_rexmit(struct net_bridge_mcast *brmctx,
					struct net_bridge_mcast_port *pmctx,
					struct net_bridge_port_group *pg)
{
	struct bridge_mcast_other_query *other_query = NULL;
	unsigned long now = jiffies, lmi;

	if (!netif_running(brmctx->br->dev) ||
	    !br_opt_get(brmctx->br, BROPT_MULTICAST_ENABLED))
		return;

	if (pg->key.addr.proto == htons(ETH_P_IP))
		other_query = &brmctx->ip4_other_query;
#if IS_ENABLED(CONFIG_IPV6)
	else
		other_query = &brmctx->ip6_other_query;
#endif

	if (brmctx->multicast_querier &&
	    other_query && !timer_pending(&other_query->timer)) {
		lmi = now + brmctx->multicast_last_member_interval;
		pg->grp_query_rexmit_cnt = brmctx->multicast_last_member_count - 1;
		__br_multicast_send_query(brmctx, pmctx, pg, &pg->key.addr,
					  &pg->key.addr, false, 0, NULL);
		if (!timer_pending(&pg->rexmit_timer) ||
		    time_after(pg->rexmit_timer.expires, lmi))
			mod_timer(&pg->rexmit_timer, lmi);
	}

	if (pg->filter_mode == MCAST_EXCLUDE &&
	    (!timer_pending(&pg->timer) ||
	     time_after(pg->timer.expires, now + br_multicast_lmqt(brmctx))))
		mod_timer(&pg->timer, now + br_multicast_lmqt(brmctx));
}

/* State          Msg type      New state                Actions
 * INCLUDE (A)    IS_IN (B)     INCLUDE (A+B)            (B)=GMI
 * INCLUDE (A)    ALLOW (B)     INCLUDE (A+B)            (B)=GMI
 * EXCLUDE (X,Y)  ALLOW (A)     EXCLUDE (X+A,Y-A)        (A)=GMI
 */
static bool br_multicast_isinc_allow(const struct net_bridge_mcast *brmctx,
				     struct net_bridge_port_group *pg, void *h_addr,
				     void *srcs, u32 nsrcs, size_t addr_size,
				     int grec_type)
{
	struct net_bridge_group_src *ent;
	unsigned long now = jiffies;
	bool changed = false;
	struct br_ip src_ip;
	u32 src_idx;

	memset(&src_ip, 0, sizeof(src_ip));
	src_ip.proto = pg->key.addr.proto;
	for (src_idx = 0; src_idx < nsrcs; src_idx++) {
		memcpy(&src_ip.src, srcs + (src_idx * addr_size), addr_size);
		ent = br_multicast_find_group_src(pg, &src_ip);
		if (!ent) {
			ent = br_multicast_new_group_src(pg, &src_ip);
			if (ent)
				changed = true;
		}

		if (ent)
			__grp_src_mod_timer(ent, now + br_multicast_gmi(brmctx));
	}

	if (br_multicast_eht_handle(brmctx, pg, h_addr, srcs, nsrcs, addr_size,
				    grec_type))
		changed = true;

	return changed;
}

/* State          Msg type      New state                Actions
 * INCLUDE (A)    IS_EX (B)     EXCLUDE (A*B,B-A)        (B-A)=0
 *                                                       Delete (A-B)
 *                                                       Group Timer=GMI
 */
static void __grp_src_isexc_incl(const struct net_bridge_mcast *brmctx,
				 struct net_bridge_port_group *pg, void *h_addr,
				 void *srcs, u32 nsrcs, size_t addr_size,
				 int grec_type)
{
	struct net_bridge_group_src *ent;
	struct br_ip src_ip;
	u32 src_idx;

	hlist_for_each_entry(ent, &pg->src_list, node)
		ent->flags |= BR_SGRP_F_DELETE;

	memset(&src_ip, 0, sizeof(src_ip));
	src_ip.proto = pg->key.addr.proto;
	for (src_idx = 0; src_idx < nsrcs; src_idx++) {
		memcpy(&src_ip.src, srcs + (src_idx * addr_size), addr_size);
		ent = br_multicast_find_group_src(pg, &src_ip);
		if (ent)
			ent->flags &= ~BR_SGRP_F_DELETE;
		else
			ent = br_multicast_new_group_src(pg, &src_ip);
		if (ent)
			br_multicast_fwd_src_handle(ent);
	}

	br_multicast_eht_handle(brmctx, pg, h_addr, srcs, nsrcs, addr_size,
				grec_type);

	__grp_src_delete_marked(pg);
}

/* State          Msg type      New state                Actions
 * EXCLUDE (X,Y)  IS_EX (A)     EXCLUDE (A-Y,Y*A)        (A-X-Y)=GMI
 *                                                       Delete (X-A)
 *                                                       Delete (Y-A)
 *                                                       Group Timer=GMI
 */
static bool __grp_src_isexc_excl(const struct net_bridge_mcast *brmctx,
				 struct net_bridge_port_group *pg, void *h_addr,
				 void *srcs, u32 nsrcs, size_t addr_size,
				 int grec_type)
{
	struct net_bridge_group_src *ent;
	unsigned long now = jiffies;
	bool changed = false;
	struct br_ip src_ip;
	u32 src_idx;

	hlist_for_each_entry(ent, &pg->src_list, node)
		ent->flags |= BR_SGRP_F_DELETE;

	memset(&src_ip, 0, sizeof(src_ip));
	src_ip.proto = pg->key.addr.proto;
	for (src_idx = 0; src_idx < nsrcs; src_idx++) {
		memcpy(&src_ip.src, srcs + (src_idx * addr_size), addr_size);
		ent = br_multicast_find_group_src(pg, &src_ip);
		if (ent) {
			ent->flags &= ~BR_SGRP_F_DELETE;
		} else {
			ent = br_multicast_new_group_src(pg, &src_ip);
			if (ent) {
				__grp_src_mod_timer(ent,
						    now + br_multicast_gmi(brmctx));
				changed = true;
			}
		}
	}

	if (br_multicast_eht_handle(brmctx, pg, h_addr, srcs, nsrcs, addr_size,
				    grec_type))
		changed = true;

	if (__grp_src_delete_marked(pg))
		changed = true;

	return changed;
}

static bool br_multicast_isexc(const struct net_bridge_mcast *brmctx,
			       struct net_bridge_port_group *pg, void *h_addr,
			       void *srcs, u32 nsrcs, size_t addr_size,
			       int grec_type)
{
	bool changed = false;

	switch (pg->filter_mode) {
	case MCAST_INCLUDE:
		__grp_src_isexc_incl(brmctx, pg, h_addr, srcs, nsrcs, addr_size,
				     grec_type);
		br_multicast_star_g_handle_mode(pg, MCAST_EXCLUDE);
		changed = true;
		break;
	case MCAST_EXCLUDE:
		changed = __grp_src_isexc_excl(brmctx, pg, h_addr, srcs, nsrcs,
					       addr_size, grec_type);
		break;
	}

	pg->filter_mode = MCAST_EXCLUDE;
	mod_timer(&pg->timer, jiffies + br_multicast_gmi(brmctx));

	return changed;
}

/* State          Msg type      New state                Actions
 * INCLUDE (A)    TO_IN (B)     INCLUDE (A+B)            (B)=GMI
 *                                                       Send Q(G,A-B)
 */
static bool __grp_src_toin_incl(struct net_bridge_mcast *brmctx,
				struct net_bridge_mcast_port *pmctx,
				struct net_bridge_port_group *pg, void *h_addr,
				void *srcs, u32 nsrcs, size_t addr_size,
				int grec_type)
{
	u32 src_idx, to_send = pg->src_ents;
	struct net_bridge_group_src *ent;
	unsigned long now = jiffies;
	bool changed = false;
	struct br_ip src_ip;

	hlist_for_each_entry(ent, &pg->src_list, node)
		ent->flags |= BR_SGRP_F_SEND;

	memset(&src_ip, 0, sizeof(src_ip));
	src_ip.proto = pg->key.addr.proto;
	for (src_idx = 0; src_idx < nsrcs; src_idx++) {
		memcpy(&src_ip.src, srcs + (src_idx * addr_size), addr_size);
		ent = br_multicast_find_group_src(pg, &src_ip);
		if (ent) {
			ent->flags &= ~BR_SGRP_F_SEND;
			to_send--;
		} else {
			ent = br_multicast_new_group_src(pg, &src_ip);
			if (ent)
				changed = true;
		}
		if (ent)
			__grp_src_mod_timer(ent, now + br_multicast_gmi(brmctx));
	}

	if (br_multicast_eht_handle(brmctx, pg, h_addr, srcs, nsrcs, addr_size,
				    grec_type))
		changed = true;

	if (to_send)
		__grp_src_query_marked_and_rexmit(brmctx, pmctx, pg);

	return changed;
}

/* State          Msg type      New state                Actions
 * EXCLUDE (X,Y)  TO_IN (A)     EXCLUDE (X+A,Y-A)        (A)=GMI
 *                                                       Send Q(G,X-A)
 *                                                       Send Q(G)
 */
static bool __grp_src_toin_excl(struct net_bridge_mcast *brmctx,
				struct net_bridge_mcast_port *pmctx,
				struct net_bridge_port_group *pg, void *h_addr,
				void *srcs, u32 nsrcs, size_t addr_size,
				int grec_type)
{
	u32 src_idx, to_send = pg->src_ents;
	struct net_bridge_group_src *ent;
	unsigned long now = jiffies;
	bool changed = false;
	struct br_ip src_ip;

	hlist_for_each_entry(ent, &pg->src_list, node)
		if (timer_pending(&ent->timer))
			ent->flags |= BR_SGRP_F_SEND;

	memset(&src_ip, 0, sizeof(src_ip));
	src_ip.proto = pg->key.addr.proto;
	for (src_idx = 0; src_idx < nsrcs; src_idx++) {
		memcpy(&src_ip.src, srcs + (src_idx * addr_size), addr_size);
		ent = br_multicast_find_group_src(pg, &src_ip);
		if (ent) {
			if (timer_pending(&ent->timer)) {
				ent->flags &= ~BR_SGRP_F_SEND;
				to_send--;
			}
		} else {
			ent = br_multicast_new_group_src(pg, &src_ip);
			if (ent)
				changed = true;
		}
		if (ent)
			__grp_src_mod_timer(ent, now + br_multicast_gmi(brmctx));
	}

	if (br_multicast_eht_handle(brmctx, pg, h_addr, srcs, nsrcs, addr_size,
				    grec_type))
		changed = true;

	if (to_send)
		__grp_src_query_marked_and_rexmit(brmctx, pmctx, pg);

	__grp_send_query_and_rexmit(brmctx, pmctx, pg);

	return changed;
}

static bool br_multicast_toin(struct net_bridge_mcast *brmctx,
			      struct net_bridge_mcast_port *pmctx,
			      struct net_bridge_port_group *pg, void *h_addr,
			      void *srcs, u32 nsrcs, size_t addr_size,
			      int grec_type)
{
	bool changed = false;

	switch (pg->filter_mode) {
	case MCAST_INCLUDE:
		changed = __grp_src_toin_incl(brmctx, pmctx, pg, h_addr, srcs,
					      nsrcs, addr_size, grec_type);
		break;
	case MCAST_EXCLUDE:
		changed = __grp_src_toin_excl(brmctx, pmctx, pg, h_addr, srcs,
					      nsrcs, addr_size, grec_type);
		break;
	}

	if (br_multicast_eht_should_del_pg(pg)) {
		pg->flags |= MDB_PG_FLAGS_FAST_LEAVE;
		br_multicast_find_del_pg(pg->key.port->br, pg);
		/* a notification has already been sent and we shouldn't
		 * access pg after the delete so we have to return false
		 */
		changed = false;
	}

	return changed;
}

/* State          Msg type      New state                Actions
 * INCLUDE (A)    TO_EX (B)     EXCLUDE (A*B,B-A)        (B-A)=0
 *                                                       Delete (A-B)
 *                                                       Send Q(G,A*B)
 *                                                       Group Timer=GMI
 */
static void __grp_src_toex_incl(struct net_bridge_mcast *brmctx,
				struct net_bridge_mcast_port *pmctx,
				struct net_bridge_port_group *pg, void *h_addr,
				void *srcs, u32 nsrcs, size_t addr_size,
				int grec_type)
{
	struct net_bridge_group_src *ent;
	u32 src_idx, to_send = 0;
	struct br_ip src_ip;

	hlist_for_each_entry(ent, &pg->src_list, node)
		ent->flags = (ent->flags & ~BR_SGRP_F_SEND) | BR_SGRP_F_DELETE;

	memset(&src_ip, 0, sizeof(src_ip));
	src_ip.proto = pg->key.addr.proto;
	for (src_idx = 0; src_idx < nsrcs; src_idx++) {
		memcpy(&src_ip.src, srcs + (src_idx * addr_size), addr_size);
		ent = br_multicast_find_group_src(pg, &src_ip);
		if (ent) {
			ent->flags = (ent->flags & ~BR_SGRP_F_DELETE) |
				     BR_SGRP_F_SEND;
			to_send++;
		} else {
			ent = br_multicast_new_group_src(pg, &src_ip);
		}
		if (ent)
			br_multicast_fwd_src_handle(ent);
	}

	br_multicast_eht_handle(brmctx, pg, h_addr, srcs, nsrcs, addr_size,
				grec_type);

	__grp_src_delete_marked(pg);
	if (to_send)
		__grp_src_query_marked_and_rexmit(brmctx, pmctx, pg);
}

/* State          Msg type      New state                Actions
 * EXCLUDE (X,Y)  TO_EX (A)     EXCLUDE (A-Y,Y*A)        (A-X-Y)=Group Timer
 *                                                       Delete (X-A)
 *                                                       Delete (Y-A)
 *                                                       Send Q(G,A-Y)
 *                                                       Group Timer=GMI
 */
static bool __grp_src_toex_excl(struct net_bridge_mcast *brmctx,
				struct net_bridge_mcast_port *pmctx,
				struct net_bridge_port_group *pg, void *h_addr,
				void *srcs, u32 nsrcs, size_t addr_size,
				int grec_type)
{
	struct net_bridge_group_src *ent;
	u32 src_idx, to_send = 0;
	bool changed = false;
	struct br_ip src_ip;

	hlist_for_each_entry(ent, &pg->src_list, node)
		ent->flags = (ent->flags & ~BR_SGRP_F_SEND) | BR_SGRP_F_DELETE;

	memset(&src_ip, 0, sizeof(src_ip));
	src_ip.proto = pg->key.addr.proto;
	for (src_idx = 0; src_idx < nsrcs; src_idx++) {
		memcpy(&src_ip.src, srcs + (src_idx * addr_size), addr_size);
		ent = br_multicast_find_group_src(pg, &src_ip);
		if (ent) {
			ent->flags &= ~BR_SGRP_F_DELETE;
		} else {
			ent = br_multicast_new_group_src(pg, &src_ip);
			if (ent) {
				__grp_src_mod_timer(ent, pg->timer.expires);
				changed = true;
			}
		}
		if (ent && timer_pending(&ent->timer)) {
			ent->flags |= BR_SGRP_F_SEND;
			to_send++;
		}
	}

	if (br_multicast_eht_handle(brmctx, pg, h_addr, srcs, nsrcs, addr_size,
				    grec_type))
		changed = true;

	if (__grp_src_delete_marked(pg))
		changed = true;
	if (to_send)
		__grp_src_query_marked_and_rexmit(brmctx, pmctx, pg);

	return changed;
}

static bool br_multicast_toex(struct net_bridge_mcast *brmctx,
			      struct net_bridge_mcast_port *pmctx,
			      struct net_bridge_port_group *pg, void *h_addr,
			      void *srcs, u32 nsrcs, size_t addr_size,
			      int grec_type)
{
	bool changed = false;

	switch (pg->filter_mode) {
	case MCAST_INCLUDE:
		__grp_src_toex_incl(brmctx, pmctx, pg, h_addr, srcs, nsrcs,
				    addr_size, grec_type);
		br_multicast_star_g_handle_mode(pg, MCAST_EXCLUDE);
		changed = true;
		break;
	case MCAST_EXCLUDE:
		changed = __grp_src_toex_excl(brmctx, pmctx, pg, h_addr, srcs,
					      nsrcs, addr_size, grec_type);
		break;
	}

	pg->filter_mode = MCAST_EXCLUDE;
	mod_timer(&pg->timer, jiffies + br_multicast_gmi(brmctx));

	return changed;
}

/* State          Msg type      New state                Actions
 * INCLUDE (A)    BLOCK (B)     INCLUDE (A)              Send Q(G,A*B)
 */
static bool __grp_src_block_incl(struct net_bridge_mcast *brmctx,
				 struct net_bridge_mcast_port *pmctx,
				 struct net_bridge_port_group *pg, void *h_addr,
				 void *srcs, u32 nsrcs, size_t addr_size, int grec_type)
{
	struct net_bridge_group_src *ent;
	u32 src_idx, to_send = 0;
	bool changed = false;
	struct br_ip src_ip;

	hlist_for_each_entry(ent, &pg->src_list, node)
		ent->flags &= ~BR_SGRP_F_SEND;

	memset(&src_ip, 0, sizeof(src_ip));
	src_ip.proto = pg->key.addr.proto;
	for (src_idx = 0; src_idx < nsrcs; src_idx++) {
		memcpy(&src_ip.src, srcs + (src_idx * addr_size), addr_size);
		ent = br_multicast_find_group_src(pg, &src_ip);
		if (ent) {
			ent->flags |= BR_SGRP_F_SEND;
			to_send++;
		}
	}

	if (br_multicast_eht_handle(brmctx, pg, h_addr, srcs, nsrcs, addr_size,
				    grec_type))
		changed = true;

	if (to_send)
		__grp_src_query_marked_and_rexmit(brmctx, pmctx, pg);

	return changed;
}

/* State          Msg type      New state                Actions
 * EXCLUDE (X,Y)  BLOCK (A)     EXCLUDE (X+(A-Y),Y)      (A-X-Y)=Group Timer
 *                                                       Send Q(G,A-Y)
 */
static bool __grp_src_block_excl(struct net_bridge_mcast *brmctx,
				 struct net_bridge_mcast_port *pmctx,
				 struct net_bridge_port_group *pg, void *h_addr,
				 void *srcs, u32 nsrcs, size_t addr_size, int grec_type)
{
	struct net_bridge_group_src *ent;
	u32 src_idx, to_send = 0;
	bool changed = false;
	struct br_ip src_ip;

	hlist_for_each_entry(ent, &pg->src_list, node)
		ent->flags &= ~BR_SGRP_F_SEND;

	memset(&src_ip, 0, sizeof(src_ip));
	src_ip.proto = pg->key.addr.proto;
	for (src_idx = 0; src_idx < nsrcs; src_idx++) {
		memcpy(&src_ip.src, srcs + (src_idx * addr_size), addr_size);
		ent = br_multicast_find_group_src(pg, &src_ip);
		if (!ent) {
			ent = br_multicast_new_group_src(pg, &src_ip);
			if (ent) {
				__grp_src_mod_timer(ent, pg->timer.expires);
				changed = true;
			}
		}
		if (ent && timer_pending(&ent->timer)) {
			ent->flags |= BR_SGRP_F_SEND;
			to_send++;
		}
	}

	if (br_multicast_eht_handle(brmctx, pg, h_addr, srcs, nsrcs, addr_size,
				    grec_type))
		changed = true;

	if (to_send)
		__grp_src_query_marked_and_rexmit(brmctx, pmctx, pg);

	return changed;
}

static bool br_multicast_block(struct net_bridge_mcast *brmctx,
			       struct net_bridge_mcast_port *pmctx,
			       struct net_bridge_port_group *pg, void *h_addr,
			       void *srcs, u32 nsrcs, size_t addr_size, int grec_type)
{
	bool changed = false;

	switch (pg->filter_mode) {
	case MCAST_INCLUDE:
		changed = __grp_src_block_incl(brmctx, pmctx, pg, h_addr, srcs,
					       nsrcs, addr_size, grec_type);
		break;
	case MCAST_EXCLUDE:
		changed = __grp_src_block_excl(brmctx, pmctx, pg, h_addr, srcs,
					       nsrcs, addr_size, grec_type);
		break;
	}

	if ((pg->filter_mode == MCAST_INCLUDE && hlist_empty(&pg->src_list)) ||
	    br_multicast_eht_should_del_pg(pg)) {
		if (br_multicast_eht_should_del_pg(pg))
			pg->flags |= MDB_PG_FLAGS_FAST_LEAVE;
		br_multicast_find_del_pg(pg->key.port->br, pg);
		/* a notification has already been sent and we shouldn't
		 * access pg after the delete so we have to return false
		 */
		changed = false;
	}

	return changed;
}

static struct net_bridge_port_group *
br_multicast_find_port(struct net_bridge_mdb_entry *mp,
		       struct net_bridge_port *p,
		       const unsigned char *src)
{
	struct net_bridge *br __maybe_unused = mp->br;
	struct net_bridge_port_group *pg;

	for (pg = mlock_dereference(mp->ports, br);
	     pg;
	     pg = mlock_dereference(pg->next, br))
		if (br_port_group_equal(pg, p, src))
			return pg;

	return NULL;
}

static int br_ip4_multicast_igmp3_report(struct net_bridge_mcast *brmctx,
					 struct net_bridge_mcast_port *pmctx,
					 struct sk_buff *skb,
					 u16 vid)
{
	bool igmpv2 = brmctx->multicast_igmp_version == 2;
	struct net_bridge_mdb_entry *mdst;
	struct net_bridge_port_group *pg;
	const unsigned char *src;
	struct igmpv3_report *ih;
	struct igmpv3_grec *grec;
	int i, len, num, type;
	__be32 group, *h_addr;
	bool changed = false;
	int err = 0;
	u16 nsrcs;

	ih = igmpv3_report_hdr(skb);
	num = ntohs(ih->ngrec);
	len = skb_transport_offset(skb) + sizeof(*ih);

	for (i = 0; i < num; i++) {
		len += sizeof(*grec);
		if (!ip_mc_may_pull(skb, len))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -EINVAL;

		grec = (void *)(skb->data + len - sizeof(*grec));
		group = grec->grec_mca;
		type = grec->grec_type;
<<<<<<< HEAD

		len += ntohs(grec->grec_nsrcs) * 4;
		if (!pskb_may_pull(skb, len))
			return -EINVAL;

		/* We treat this as an IGMPv2 report for now. */
=======
		nsrcs = ntohs(grec->grec_nsrcs);

		len += nsrcs * 4;
		if (!ip_mc_may_pull(skb, len))
			return -EINVAL;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		switch (type) {
		case IGMPV3_MODE_IS_INCLUDE:
		case IGMPV3_MODE_IS_EXCLUDE:
		case IGMPV3_CHANGE_TO_INCLUDE:
		case IGMPV3_CHANGE_TO_EXCLUDE:
		case IGMPV3_ALLOW_NEW_SOURCES:
		case IGMPV3_BLOCK_OLD_SOURCES:
			break;

		default:
			continue;
		}

<<<<<<< HEAD
		err = br_ip4_multicast_add_group(br, port, group);
		if (err)
			break;
=======
		src = eth_hdr(skb)->h_source;
		if (nsrcs == 0 &&
		    (type == IGMPV3_CHANGE_TO_INCLUDE ||
		     type == IGMPV3_MODE_IS_INCLUDE)) {
			if (!pmctx || igmpv2) {
				br_ip4_multicast_leave_group(brmctx, pmctx,
							     group, vid, src);
				continue;
			}
		} else {
			err = br_ip4_multicast_add_group(brmctx, pmctx, group,
							 vid, src, igmpv2);
			if (err)
				break;
		}

		if (!pmctx || igmpv2)
			continue;

		spin_lock(&brmctx->br->multicast_lock);
		if (!br_multicast_ctx_should_use(brmctx, pmctx))
			goto unlock_continue;

		mdst = br_mdb_ip4_get(brmctx->br, group, vid);
		if (!mdst)
			goto unlock_continue;
		pg = br_multicast_find_port(mdst, pmctx->port, src);
		if (!pg || (pg->flags & MDB_PG_FLAGS_PERMANENT))
			goto unlock_continue;
		/* reload grec and host addr */
		grec = (void *)(skb->data + len - sizeof(*grec) - (nsrcs * 4));
		h_addr = &ip_hdr(skb)->saddr;
		switch (type) {
		case IGMPV3_ALLOW_NEW_SOURCES:
			changed = br_multicast_isinc_allow(brmctx, pg, h_addr,
							   grec->grec_src,
							   nsrcs, sizeof(__be32), type);
			break;
		case IGMPV3_MODE_IS_INCLUDE:
			changed = br_multicast_isinc_allow(brmctx, pg, h_addr,
							   grec->grec_src,
							   nsrcs, sizeof(__be32), type);
			break;
		case IGMPV3_MODE_IS_EXCLUDE:
			changed = br_multicast_isexc(brmctx, pg, h_addr,
						     grec->grec_src,
						     nsrcs, sizeof(__be32), type);
			break;
		case IGMPV3_CHANGE_TO_INCLUDE:
			changed = br_multicast_toin(brmctx, pmctx, pg, h_addr,
						    grec->grec_src,
						    nsrcs, sizeof(__be32), type);
			break;
		case IGMPV3_CHANGE_TO_EXCLUDE:
			changed = br_multicast_toex(brmctx, pmctx, pg, h_addr,
						    grec->grec_src,
						    nsrcs, sizeof(__be32), type);
			break;
		case IGMPV3_BLOCK_OLD_SOURCES:
			changed = br_multicast_block(brmctx, pmctx, pg, h_addr,
						     grec->grec_src,
						     nsrcs, sizeof(__be32), type);
			break;
		}
		if (changed)
			br_mdb_notify(brmctx->br->dev, mdst, pg, RTM_NEWMDB);
unlock_continue:
		spin_unlock(&brmctx->br->multicast_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return err;
}

#if IS_ENABLED(CONFIG_IPV6)
<<<<<<< HEAD
static int br_ip6_multicast_mld2_report(struct net_bridge *br,
					struct net_bridge_port *port,
					struct sk_buff *skb)
{
	struct icmp6hdr *icmp6h;
	struct mld2_grec *grec;
	int i;
	int len;
	int num;
	int err = 0;

	if (!pskb_may_pull(skb, sizeof(*icmp6h)))
		return -EINVAL;

	icmp6h = icmp6_hdr(skb);
	num = ntohs(icmp6h->icmp6_dataun.un_data16[1]);
	len = sizeof(*icmp6h);

	for (i = 0; i < num; i++) {
		__be16 *nsrcs, _nsrcs;

		nsrcs = skb_header_pointer(skb,
					   len + offsetof(struct mld2_grec,
							  grec_nsrcs),
					   sizeof(_nsrcs), &_nsrcs);
		if (!nsrcs)
			return -EINVAL;

		if (!pskb_may_pull(skb,
				   len + sizeof(*grec) +
				   sizeof(struct in6_addr) * ntohs(*nsrcs)))
			return -EINVAL;

		grec = (struct mld2_grec *)(skb->data + len);
		len += sizeof(*grec) +
		       sizeof(struct in6_addr) * ntohs(*nsrcs);

		/* We treat these as MLDv1 reports for now. */
=======
static int br_ip6_multicast_mld2_report(struct net_bridge_mcast *brmctx,
					struct net_bridge_mcast_port *pmctx,
					struct sk_buff *skb,
					u16 vid)
{
	bool mldv1 = brmctx->multicast_mld_version == 1;
	struct net_bridge_mdb_entry *mdst;
	struct net_bridge_port_group *pg;
	unsigned int nsrcs_offset;
	struct mld2_report *mld2r;
	const unsigned char *src;
	struct in6_addr *h_addr;
	struct mld2_grec *grec;
	unsigned int grec_len;
	bool changed = false;
	int i, len, num;
	int err = 0;

	if (!ipv6_mc_may_pull(skb, sizeof(*mld2r)))
		return -EINVAL;

	mld2r = (struct mld2_report *)icmp6_hdr(skb);
	num = ntohs(mld2r->mld2r_ngrec);
	len = skb_transport_offset(skb) + sizeof(*mld2r);

	for (i = 0; i < num; i++) {
		__be16 *_nsrcs, __nsrcs;
		u16 nsrcs;

		nsrcs_offset = len + offsetof(struct mld2_grec, grec_nsrcs);

		if (skb_transport_offset(skb) + ipv6_transport_len(skb) <
		    nsrcs_offset + sizeof(__nsrcs))
			return -EINVAL;

		_nsrcs = skb_header_pointer(skb, nsrcs_offset,
					    sizeof(__nsrcs), &__nsrcs);
		if (!_nsrcs)
			return -EINVAL;

		nsrcs = ntohs(*_nsrcs);
		grec_len = struct_size(grec, grec_src, nsrcs);

		if (!ipv6_mc_may_pull(skb, len + grec_len))
			return -EINVAL;

		grec = (struct mld2_grec *)(skb->data + len);
		len += grec_len;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		switch (grec->grec_type) {
		case MLD2_MODE_IS_INCLUDE:
		case MLD2_MODE_IS_EXCLUDE:
		case MLD2_CHANGE_TO_INCLUDE:
		case MLD2_CHANGE_TO_EXCLUDE:
		case MLD2_ALLOW_NEW_SOURCES:
		case MLD2_BLOCK_OLD_SOURCES:
			break;

		default:
			continue;
		}

<<<<<<< HEAD
		err = br_ip6_multicast_add_group(br, port, &grec->grec_mca);
		if (err)
			break;
=======
		src = eth_hdr(skb)->h_source;
		if ((grec->grec_type == MLD2_CHANGE_TO_INCLUDE ||
		     grec->grec_type == MLD2_MODE_IS_INCLUDE) &&
		    nsrcs == 0) {
			if (!pmctx || mldv1) {
				br_ip6_multicast_leave_group(brmctx, pmctx,
							     &grec->grec_mca,
							     vid, src);
				continue;
			}
		} else {
			err = br_ip6_multicast_add_group(brmctx, pmctx,
							 &grec->grec_mca, vid,
							 src, mldv1);
			if (err)
				break;
		}

		if (!pmctx || mldv1)
			continue;

		spin_lock(&brmctx->br->multicast_lock);
		if (!br_multicast_ctx_should_use(brmctx, pmctx))
			goto unlock_continue;

		mdst = br_mdb_ip6_get(brmctx->br, &grec->grec_mca, vid);
		if (!mdst)
			goto unlock_continue;
		pg = br_multicast_find_port(mdst, pmctx->port, src);
		if (!pg || (pg->flags & MDB_PG_FLAGS_PERMANENT))
			goto unlock_continue;
		h_addr = &ipv6_hdr(skb)->saddr;
		switch (grec->grec_type) {
		case MLD2_ALLOW_NEW_SOURCES:
			changed = br_multicast_isinc_allow(brmctx, pg, h_addr,
							   grec->grec_src, nsrcs,
							   sizeof(struct in6_addr),
							   grec->grec_type);
			break;
		case MLD2_MODE_IS_INCLUDE:
			changed = br_multicast_isinc_allow(brmctx, pg, h_addr,
							   grec->grec_src, nsrcs,
							   sizeof(struct in6_addr),
							   grec->grec_type);
			break;
		case MLD2_MODE_IS_EXCLUDE:
			changed = br_multicast_isexc(brmctx, pg, h_addr,
						     grec->grec_src, nsrcs,
						     sizeof(struct in6_addr),
						     grec->grec_type);
			break;
		case MLD2_CHANGE_TO_INCLUDE:
			changed = br_multicast_toin(brmctx, pmctx, pg, h_addr,
						    grec->grec_src, nsrcs,
						    sizeof(struct in6_addr),
						    grec->grec_type);
			break;
		case MLD2_CHANGE_TO_EXCLUDE:
			changed = br_multicast_toex(brmctx, pmctx, pg, h_addr,
						    grec->grec_src, nsrcs,
						    sizeof(struct in6_addr),
						    grec->grec_type);
			break;
		case MLD2_BLOCK_OLD_SOURCES:
			changed = br_multicast_block(brmctx, pmctx, pg, h_addr,
						     grec->grec_src, nsrcs,
						     sizeof(struct in6_addr),
						     grec->grec_type);
			break;
		}
		if (changed)
			br_mdb_notify(brmctx->br->dev, mdst, pg, RTM_NEWMDB);
unlock_continue:
		spin_unlock(&brmctx->br->multicast_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return err;
}
#endif

<<<<<<< HEAD
/*
 * Add port to rotuer_list
 *  list is maintained ordered by pointer value
 *  and locked by br->multicast_lock and RCU
 */
static void br_multicast_add_router(struct net_bridge *br,
				    struct net_bridge_port *port)
{
	struct net_bridge_port *p;
	struct hlist_node *n, *slot = NULL;

	if (!hlist_unhashed(&port->rlist))
		return;

	hlist_for_each_entry(p, n, &br->router_list, rlist) {
		if ((unsigned long) port >= (unsigned long) p)
			break;
		slot = n;
	}

	if (slot)
		hlist_add_after_rcu(slot, &port->rlist);
	else
		hlist_add_head_rcu(&port->rlist, &br->router_list);
}

static void br_multicast_mark_router(struct net_bridge *br,
				     struct net_bridge_port *port)
{
	unsigned long now = jiffies;

	if (!port) {
		if (br->multicast_router == 1)
			mod_timer(&br->multicast_router_timer,
				  now + br->multicast_querier_interval);
		return;
	}

	if (port->multicast_router != 1)
		return;

	br_multicast_add_router(br, port);

	mod_timer(&port->multicast_router_timer,
		  now + br->multicast_querier_interval);
}

static void br_multicast_query_received(struct net_bridge *br,
					struct net_bridge_port *port,
					int saddr)
{
	if (saddr)
		mod_timer(&br->multicast_querier_timer,
			  jiffies + br->multicast_querier_interval);
	else if (timer_pending(&br->multicast_querier_timer))
		return;

	br_multicast_mark_router(br, port);
}

static int br_ip4_multicast_query(struct net_bridge *br,
				  struct net_bridge_port *port,
				  struct sk_buff *skb)
{
=======
static bool br_multicast_select_querier(struct net_bridge_mcast *brmctx,
					struct net_bridge_mcast_port *pmctx,
					struct br_ip *saddr)
{
	int port_ifidx = pmctx ? pmctx->port->dev->ifindex : 0;
	struct timer_list *own_timer, *other_timer;
	struct bridge_mcast_querier *querier;

	switch (saddr->proto) {
	case htons(ETH_P_IP):
		querier = &brmctx->ip4_querier;
		own_timer = &brmctx->ip4_own_query.timer;
		other_timer = &brmctx->ip4_other_query.timer;
		if (!querier->addr.src.ip4 ||
		    ntohl(saddr->src.ip4) <= ntohl(querier->addr.src.ip4))
			goto update;
		break;
#if IS_ENABLED(CONFIG_IPV6)
	case htons(ETH_P_IPV6):
		querier = &brmctx->ip6_querier;
		own_timer = &brmctx->ip6_own_query.timer;
		other_timer = &brmctx->ip6_other_query.timer;
		if (ipv6_addr_cmp(&saddr->src.ip6, &querier->addr.src.ip6) <= 0)
			goto update;
		break;
#endif
	default:
		return false;
	}

	if (!timer_pending(own_timer) && !timer_pending(other_timer))
		goto update;

	return false;

update:
	br_multicast_update_querier(brmctx, querier, port_ifidx, saddr);

	return true;
}

static struct net_bridge_port *
__br_multicast_get_querier_port(struct net_bridge *br,
				const struct bridge_mcast_querier *querier)
{
	int port_ifidx = READ_ONCE(querier->port_ifidx);
	struct net_bridge_port *p;
	struct net_device *dev;

	if (port_ifidx == 0)
		return NULL;

	dev = dev_get_by_index_rcu(dev_net(br->dev), port_ifidx);
	if (!dev)
		return NULL;
	p = br_port_get_rtnl_rcu(dev);
	if (!p || p->br != br)
		return NULL;

	return p;
}

size_t br_multicast_querier_state_size(void)
{
	return nla_total_size(0) +		/* nest attribute */
	       nla_total_size(sizeof(__be32)) + /* BRIDGE_QUERIER_IP_ADDRESS */
	       nla_total_size(sizeof(int)) +    /* BRIDGE_QUERIER_IP_PORT */
	       nla_total_size_64bit(sizeof(u64)) + /* BRIDGE_QUERIER_IP_OTHER_TIMER */
#if IS_ENABLED(CONFIG_IPV6)
	       nla_total_size(sizeof(struct in6_addr)) + /* BRIDGE_QUERIER_IPV6_ADDRESS */
	       nla_total_size(sizeof(int)) +		 /* BRIDGE_QUERIER_IPV6_PORT */
	       nla_total_size_64bit(sizeof(u64)) +	 /* BRIDGE_QUERIER_IPV6_OTHER_TIMER */
#endif
	       0;
}

/* protected by rtnl or rcu */
int br_multicast_dump_querier_state(struct sk_buff *skb,
				    const struct net_bridge_mcast *brmctx,
				    int nest_attr)
{
	struct bridge_mcast_querier querier = {};
	struct net_bridge_port *p;
	struct nlattr *nest;

	if (!br_opt_get(brmctx->br, BROPT_MULTICAST_ENABLED) ||
	    br_multicast_ctx_vlan_global_disabled(brmctx))
		return 0;

	nest = nla_nest_start(skb, nest_attr);
	if (!nest)
		return -EMSGSIZE;

	rcu_read_lock();
	if (!brmctx->multicast_querier &&
	    !timer_pending(&brmctx->ip4_other_query.timer))
		goto out_v6;

	br_multicast_read_querier(&brmctx->ip4_querier, &querier);
	if (nla_put_in_addr(skb, BRIDGE_QUERIER_IP_ADDRESS,
			    querier.addr.src.ip4)) {
		rcu_read_unlock();
		goto out_err;
	}

	p = __br_multicast_get_querier_port(brmctx->br, &querier);
	if (timer_pending(&brmctx->ip4_other_query.timer) &&
	    (nla_put_u64_64bit(skb, BRIDGE_QUERIER_IP_OTHER_TIMER,
			       br_timer_value(&brmctx->ip4_other_query.timer),
			       BRIDGE_QUERIER_PAD) ||
	     (p && nla_put_u32(skb, BRIDGE_QUERIER_IP_PORT, p->dev->ifindex)))) {
		rcu_read_unlock();
		goto out_err;
	}

out_v6:
#if IS_ENABLED(CONFIG_IPV6)
	if (!brmctx->multicast_querier &&
	    !timer_pending(&brmctx->ip6_other_query.timer))
		goto out;

	br_multicast_read_querier(&brmctx->ip6_querier, &querier);
	if (nla_put_in6_addr(skb, BRIDGE_QUERIER_IPV6_ADDRESS,
			     &querier.addr.src.ip6)) {
		rcu_read_unlock();
		goto out_err;
	}

	p = __br_multicast_get_querier_port(brmctx->br, &querier);
	if (timer_pending(&brmctx->ip6_other_query.timer) &&
	    (nla_put_u64_64bit(skb, BRIDGE_QUERIER_IPV6_OTHER_TIMER,
			       br_timer_value(&brmctx->ip6_other_query.timer),
			       BRIDGE_QUERIER_PAD) ||
	     (p && nla_put_u32(skb, BRIDGE_QUERIER_IPV6_PORT,
			       p->dev->ifindex)))) {
		rcu_read_unlock();
		goto out_err;
	}
out:
#endif
	rcu_read_unlock();
	nla_nest_end(skb, nest);
	if (!nla_len(nest))
		nla_nest_cancel(skb, nest);

	return 0;

out_err:
	nla_nest_cancel(skb, nest);
	return -EMSGSIZE;
}

static void
br_multicast_update_query_timer(struct net_bridge_mcast *brmctx,
				struct bridge_mcast_other_query *query,
				unsigned long max_delay)
{
	if (!timer_pending(&query->timer))
		mod_timer(&query->delay_timer, jiffies + max_delay);

	mod_timer(&query->timer, jiffies + brmctx->multicast_querier_interval);
}

static void br_port_mc_router_state_change(struct net_bridge_port *p,
					   bool is_mc_router)
{
	struct switchdev_attr attr = {
		.orig_dev = p->dev,
		.id = SWITCHDEV_ATTR_ID_PORT_MROUTER,
		.flags = SWITCHDEV_F_DEFER,
		.u.mrouter = is_mc_router,
	};

	switchdev_port_attr_set(p->dev, &attr, NULL);
}

static struct net_bridge_port *
br_multicast_rport_from_node(struct net_bridge_mcast *brmctx,
			     struct hlist_head *mc_router_list,
			     struct hlist_node *rlist)
{
	struct net_bridge_mcast_port *pmctx;

#if IS_ENABLED(CONFIG_IPV6)
	if (mc_router_list == &brmctx->ip6_mc_router_list)
		pmctx = hlist_entry(rlist, struct net_bridge_mcast_port,
				    ip6_rlist);
	else
#endif
		pmctx = hlist_entry(rlist, struct net_bridge_mcast_port,
				    ip4_rlist);

	return pmctx->port;
}

static struct hlist_node *
br_multicast_get_rport_slot(struct net_bridge_mcast *brmctx,
			    struct net_bridge_port *port,
			    struct hlist_head *mc_router_list)

{
	struct hlist_node *slot = NULL;
	struct net_bridge_port *p;
	struct hlist_node *rlist;

	hlist_for_each(rlist, mc_router_list) {
		p = br_multicast_rport_from_node(brmctx, mc_router_list, rlist);

		if ((unsigned long)port >= (unsigned long)p)
			break;

		slot = rlist;
	}

	return slot;
}

static bool br_multicast_no_router_otherpf(struct net_bridge_mcast_port *pmctx,
					   struct hlist_node *rnode)
{
#if IS_ENABLED(CONFIG_IPV6)
	if (rnode != &pmctx->ip6_rlist)
		return hlist_unhashed(&pmctx->ip6_rlist);
	else
		return hlist_unhashed(&pmctx->ip4_rlist);
#else
	return true;
#endif
}

/* Add port to router_list
 *  list is maintained ordered by pointer value
 *  and locked by br->multicast_lock and RCU
 */
static void br_multicast_add_router(struct net_bridge_mcast *brmctx,
				    struct net_bridge_mcast_port *pmctx,
				    struct hlist_node *rlist,
				    struct hlist_head *mc_router_list)
{
	struct hlist_node *slot;

	if (!hlist_unhashed(rlist))
		return;

	slot = br_multicast_get_rport_slot(brmctx, pmctx->port, mc_router_list);

	if (slot)
		hlist_add_behind_rcu(rlist, slot);
	else
		hlist_add_head_rcu(rlist, mc_router_list);

	/* For backwards compatibility for now, only notify if we
	 * switched from no IPv4/IPv6 multicast router to a new
	 * IPv4 or IPv6 multicast router.
	 */
	if (br_multicast_no_router_otherpf(pmctx, rlist)) {
		br_rtr_notify(pmctx->port->br->dev, pmctx, RTM_NEWMDB);
		br_port_mc_router_state_change(pmctx->port, true);
	}
}

/* Add port to router_list
 *  list is maintained ordered by pointer value
 *  and locked by br->multicast_lock and RCU
 */
static void br_ip4_multicast_add_router(struct net_bridge_mcast *brmctx,
					struct net_bridge_mcast_port *pmctx)
{
	br_multicast_add_router(brmctx, pmctx, &pmctx->ip4_rlist,
				&brmctx->ip4_mc_router_list);
}

/* Add port to router_list
 *  list is maintained ordered by pointer value
 *  and locked by br->multicast_lock and RCU
 */
static void br_ip6_multicast_add_router(struct net_bridge_mcast *brmctx,
					struct net_bridge_mcast_port *pmctx)
{
#if IS_ENABLED(CONFIG_IPV6)
	br_multicast_add_router(brmctx, pmctx, &pmctx->ip6_rlist,
				&brmctx->ip6_mc_router_list);
#endif
}

static void br_multicast_mark_router(struct net_bridge_mcast *brmctx,
				     struct net_bridge_mcast_port *pmctx,
				     struct timer_list *timer,
				     struct hlist_node *rlist,
				     struct hlist_head *mc_router_list)
{
	unsigned long now = jiffies;

	if (!br_multicast_ctx_should_use(brmctx, pmctx))
		return;

	if (!pmctx) {
		if (brmctx->multicast_router == MDB_RTR_TYPE_TEMP_QUERY) {
			if (!br_ip4_multicast_is_router(brmctx) &&
			    !br_ip6_multicast_is_router(brmctx))
				br_mc_router_state_change(brmctx->br, true);
			mod_timer(timer, now + brmctx->multicast_querier_interval);
		}
		return;
	}

	if (pmctx->multicast_router == MDB_RTR_TYPE_DISABLED ||
	    pmctx->multicast_router == MDB_RTR_TYPE_PERM)
		return;

	br_multicast_add_router(brmctx, pmctx, rlist, mc_router_list);
	mod_timer(timer, now + brmctx->multicast_querier_interval);
}

static void br_ip4_multicast_mark_router(struct net_bridge_mcast *brmctx,
					 struct net_bridge_mcast_port *pmctx)
{
	struct timer_list *timer = &brmctx->ip4_mc_router_timer;
	struct hlist_node *rlist = NULL;

	if (pmctx) {
		timer = &pmctx->ip4_mc_router_timer;
		rlist = &pmctx->ip4_rlist;
	}

	br_multicast_mark_router(brmctx, pmctx, timer, rlist,
				 &brmctx->ip4_mc_router_list);
}

static void br_ip6_multicast_mark_router(struct net_bridge_mcast *brmctx,
					 struct net_bridge_mcast_port *pmctx)
{
#if IS_ENABLED(CONFIG_IPV6)
	struct timer_list *timer = &brmctx->ip6_mc_router_timer;
	struct hlist_node *rlist = NULL;

	if (pmctx) {
		timer = &pmctx->ip6_mc_router_timer;
		rlist = &pmctx->ip6_rlist;
	}

	br_multicast_mark_router(brmctx, pmctx, timer, rlist,
				 &brmctx->ip6_mc_router_list);
#endif
}

static void
br_ip4_multicast_query_received(struct net_bridge_mcast *brmctx,
				struct net_bridge_mcast_port *pmctx,
				struct bridge_mcast_other_query *query,
				struct br_ip *saddr,
				unsigned long max_delay)
{
	if (!br_multicast_select_querier(brmctx, pmctx, saddr))
		return;

	br_multicast_update_query_timer(brmctx, query, max_delay);
	br_ip4_multicast_mark_router(brmctx, pmctx);
}

#if IS_ENABLED(CONFIG_IPV6)
static void
br_ip6_multicast_query_received(struct net_bridge_mcast *brmctx,
				struct net_bridge_mcast_port *pmctx,
				struct bridge_mcast_other_query *query,
				struct br_ip *saddr,
				unsigned long max_delay)
{
	if (!br_multicast_select_querier(brmctx, pmctx, saddr))
		return;

	br_multicast_update_query_timer(brmctx, query, max_delay);
	br_ip6_multicast_mark_router(brmctx, pmctx);
}
#endif

static void br_ip4_multicast_query(struct net_bridge_mcast *brmctx,
				   struct net_bridge_mcast_port *pmctx,
				   struct sk_buff *skb,
				   u16 vid)
{
	unsigned int transport_len = ip_transport_len(skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	const struct iphdr *iph = ip_hdr(skb);
	struct igmphdr *ih = igmp_hdr(skb);
	struct net_bridge_mdb_entry *mp;
	struct igmpv3_query *ih3;
	struct net_bridge_port_group *p;
	struct net_bridge_port_group __rcu **pp;
<<<<<<< HEAD
	unsigned long max_delay;
	unsigned long now = jiffies;
	__be32 group;
	int err = 0;

	spin_lock(&br->multicast_lock);
	if (!netif_running(br->dev) ||
	    (port && port->state == BR_STATE_DISABLED))
		goto out;

	br_multicast_query_received(br, port, !!iph->saddr);

	group = ih->group;

	if (skb->len == sizeof(*ih)) {
=======
	struct br_ip saddr = {};
	unsigned long max_delay;
	unsigned long now = jiffies;
	__be32 group;

	spin_lock(&brmctx->br->multicast_lock);
	if (!br_multicast_ctx_should_use(brmctx, pmctx))
		goto out;

	group = ih->group;

	if (transport_len == sizeof(*ih)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		max_delay = ih->code * (HZ / IGMP_TIMER_SCALE);

		if (!max_delay) {
			max_delay = 10 * HZ;
			group = 0;
		}
<<<<<<< HEAD
	} else {
		if (!pskb_may_pull(skb, sizeof(struct igmpv3_query))) {
			err = -EINVAL;
			goto out;
		}

		ih3 = igmpv3_query_hdr(skb);
		if (ih3->nsrcs)
=======
	} else if (transport_len >= sizeof(*ih3)) {
		ih3 = igmpv3_query_hdr(skb);
		if (ih3->nsrcs ||
		    (brmctx->multicast_igmp_version == 3 && group &&
		     ih3->suppress))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto out;

		max_delay = ih3->code ?
			    IGMPV3_MRC(ih3->code) * (HZ / IGMP_TIMER_SCALE) : 1;
<<<<<<< HEAD
	}

	if (!group)
		goto out;

	mp = br_mdb_ip4_get(mlock_dereference(br->mdb, br), group);
	if (!mp)
		goto out;

	max_delay *= br->multicast_last_member_count;

	if (mp->mglist &&
=======
	} else {
		goto out;
	}

	if (!group) {
		saddr.proto = htons(ETH_P_IP);
		saddr.src.ip4 = iph->saddr;

		br_ip4_multicast_query_received(brmctx, pmctx,
						&brmctx->ip4_other_query,
						&saddr, max_delay);
		goto out;
	}

	mp = br_mdb_ip4_get(brmctx->br, group, vid);
	if (!mp)
		goto out;

	max_delay *= brmctx->multicast_last_member_count;

	if (mp->host_joined &&
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	    (timer_pending(&mp->timer) ?
	     time_after(mp->timer.expires, now + max_delay) :
	     try_to_del_timer_sync(&mp->timer) >= 0))
		mod_timer(&mp->timer, now + max_delay);

	for (pp = &mp->ports;
<<<<<<< HEAD
	     (p = mlock_dereference(*pp, br)) != NULL;
	     pp = &p->next) {
		if (timer_pending(&p->timer) ?
		    time_after(p->timer.expires, now + max_delay) :
		    try_to_del_timer_sync(&p->timer) >= 0)
=======
	     (p = mlock_dereference(*pp, brmctx->br)) != NULL;
	     pp = &p->next) {
		if (timer_pending(&p->timer) ?
		    time_after(p->timer.expires, now + max_delay) :
		    try_to_del_timer_sync(&p->timer) >= 0 &&
		    (brmctx->multicast_igmp_version == 2 ||
		     p->filter_mode == MCAST_EXCLUDE))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			mod_timer(&p->timer, now + max_delay);
	}

out:
<<<<<<< HEAD
	spin_unlock(&br->multicast_lock);
	return err;
}

#if IS_ENABLED(CONFIG_IPV6)
static int br_ip6_multicast_query(struct net_bridge *br,
				  struct net_bridge_port *port,
				  struct sk_buff *skb)
{
	const struct ipv6hdr *ip6h = ipv6_hdr(skb);
	struct mld_msg *mld = (struct mld_msg *) icmp6_hdr(skb);
=======
	spin_unlock(&brmctx->br->multicast_lock);
}

#if IS_ENABLED(CONFIG_IPV6)
static int br_ip6_multicast_query(struct net_bridge_mcast *brmctx,
				  struct net_bridge_mcast_port *pmctx,
				  struct sk_buff *skb,
				  u16 vid)
{
	unsigned int transport_len = ipv6_transport_len(skb);
	struct mld_msg *mld;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct net_bridge_mdb_entry *mp;
	struct mld2_query *mld2q;
	struct net_bridge_port_group *p;
	struct net_bridge_port_group __rcu **pp;
<<<<<<< HEAD
	unsigned long max_delay;
	unsigned long now = jiffies;
	const struct in6_addr *group = NULL;
	int err = 0;

	spin_lock(&br->multicast_lock);
	if (!netif_running(br->dev) ||
	    (port && port->state == BR_STATE_DISABLED))
		goto out;

	br_multicast_query_received(br, port, !ipv6_addr_any(&ip6h->saddr));

	/* RFC2710+RFC3810 (MLDv1+MLDv2) require link-local source addresses */
	if (!(ipv6_addr_type(&ip6h->saddr) & IPV6_ADDR_LINKLOCAL)) {
		err = -EINVAL;
		goto out;
	}

	if (skb->len == sizeof(*mld)) {
		if (!pskb_may_pull(skb, sizeof(*mld))) {
=======
	struct br_ip saddr = {};
	unsigned long max_delay;
	unsigned long now = jiffies;
	unsigned int offset = skb_transport_offset(skb);
	const struct in6_addr *group = NULL;
	bool is_general_query;
	int err = 0;

	spin_lock(&brmctx->br->multicast_lock);
	if (!br_multicast_ctx_should_use(brmctx, pmctx))
		goto out;

	if (transport_len == sizeof(*mld)) {
		if (!pskb_may_pull(skb, offset + sizeof(*mld))) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			err = -EINVAL;
			goto out;
		}
		mld = (struct mld_msg *) icmp6_hdr(skb);
<<<<<<< HEAD
		max_delay = msecs_to_jiffies(htons(mld->mld_maxdelay));
		if (max_delay)
			group = &mld->mld_mca;
	} else if (skb->len >= sizeof(*mld2q)) {
		if (!pskb_may_pull(skb, sizeof(*mld2q))) {
=======
		max_delay = msecs_to_jiffies(ntohs(mld->mld_maxdelay));
		if (max_delay)
			group = &mld->mld_mca;
	} else {
		if (!pskb_may_pull(skb, offset + sizeof(*mld2q))) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			err = -EINVAL;
			goto out;
		}
		mld2q = (struct mld2_query *)icmp6_hdr(skb);
		if (!mld2q->mld2q_nsrcs)
			group = &mld2q->mld2q_mca;
<<<<<<< HEAD

		max_delay = max(msecs_to_jiffies(MLDV2_MRC(ntohs(mld2q->mld2q_mrc))), 1UL);
	}

	if (!group)
		goto out;

	mp = br_mdb_ip6_get(mlock_dereference(br->mdb, br), group);
	if (!mp)
		goto out;

	max_delay *= br->multicast_last_member_count;
	if (mp->mglist &&
=======
		if (brmctx->multicast_mld_version == 2 &&
		    !ipv6_addr_any(&mld2q->mld2q_mca) &&
		    mld2q->mld2q_suppress)
			goto out;

		max_delay = max(msecs_to_jiffies(mldv2_mrc(mld2q)), 1UL);
	}

	is_general_query = group && ipv6_addr_any(group);

	if (is_general_query) {
		saddr.proto = htons(ETH_P_IPV6);
		saddr.src.ip6 = ipv6_hdr(skb)->saddr;

		br_ip6_multicast_query_received(brmctx, pmctx,
						&brmctx->ip6_other_query,
						&saddr, max_delay);
		goto out;
	} else if (!group) {
		goto out;
	}

	mp = br_mdb_ip6_get(brmctx->br, group, vid);
	if (!mp)
		goto out;

	max_delay *= brmctx->multicast_last_member_count;
	if (mp->host_joined &&
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	    (timer_pending(&mp->timer) ?
	     time_after(mp->timer.expires, now + max_delay) :
	     try_to_del_timer_sync(&mp->timer) >= 0))
		mod_timer(&mp->timer, now + max_delay);

	for (pp = &mp->ports;
<<<<<<< HEAD
	     (p = mlock_dereference(*pp, br)) != NULL;
	     pp = &p->next) {
		if (timer_pending(&p->timer) ?
		    time_after(p->timer.expires, now + max_delay) :
		    try_to_del_timer_sync(&p->timer) >= 0)
=======
	     (p = mlock_dereference(*pp, brmctx->br)) != NULL;
	     pp = &p->next) {
		if (timer_pending(&p->timer) ?
		    time_after(p->timer.expires, now + max_delay) :
		    try_to_del_timer_sync(&p->timer) >= 0 &&
		    (brmctx->multicast_mld_version == 1 ||
		     p->filter_mode == MCAST_EXCLUDE))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			mod_timer(&p->timer, now + max_delay);
	}

out:
<<<<<<< HEAD
	spin_unlock(&br->multicast_lock);
=======
	spin_unlock(&brmctx->br->multicast_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}
#endif

<<<<<<< HEAD
static void br_multicast_leave_group(struct net_bridge *br,
				     struct net_bridge_port *port,
				     struct br_ip *group)
{
	struct net_bridge_mdb_htable *mdb;
=======
static void
br_multicast_leave_group(struct net_bridge_mcast *brmctx,
			 struct net_bridge_mcast_port *pmctx,
			 struct br_ip *group,
			 struct bridge_mcast_other_query *other_query,
			 struct bridge_mcast_own_query *own_query,
			 const unsigned char *src)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct net_bridge_mdb_entry *mp;
	struct net_bridge_port_group *p;
	unsigned long now;
	unsigned long time;

<<<<<<< HEAD
	spin_lock(&br->multicast_lock);
	if (!netif_running(br->dev) ||
	    (port && port->state == BR_STATE_DISABLED) ||
	    timer_pending(&br->multicast_querier_timer))
		goto out;

	mdb = mlock_dereference(br->mdb, br);
	mp = br_mdb_ip_get(mdb, group);
	if (!mp)
		goto out;

	now = jiffies;
	time = now + br->multicast_last_member_count *
		     br->multicast_last_member_interval;

	if (!port) {
		if (mp->mglist &&
=======
	spin_lock(&brmctx->br->multicast_lock);
	if (!br_multicast_ctx_should_use(brmctx, pmctx))
		goto out;

	mp = br_mdb_ip_get(brmctx->br, group);
	if (!mp)
		goto out;

	if (pmctx && (pmctx->port->flags & BR_MULTICAST_FAST_LEAVE)) {
		struct net_bridge_port_group __rcu **pp;

		for (pp = &mp->ports;
		     (p = mlock_dereference(*pp, brmctx->br)) != NULL;
		     pp = &p->next) {
			if (!br_port_group_equal(p, pmctx->port, src))
				continue;

			if (p->flags & MDB_PG_FLAGS_PERMANENT)
				break;

			p->flags |= MDB_PG_FLAGS_FAST_LEAVE;
			br_multicast_del_pg(mp, p, pp);
		}
		goto out;
	}

	if (timer_pending(&other_query->timer))
		goto out;

	if (brmctx->multicast_querier) {
		__br_multicast_send_query(brmctx, pmctx, NULL, NULL, &mp->addr,
					  false, 0, NULL);

		time = jiffies + brmctx->multicast_last_member_count *
				 brmctx->multicast_last_member_interval;

		mod_timer(&own_query->timer, time);

		for (p = mlock_dereference(mp->ports, brmctx->br);
		     p != NULL && pmctx != NULL;
		     p = mlock_dereference(p->next, brmctx->br)) {
			if (!br_port_group_equal(p, pmctx->port, src))
				continue;

			if (!hlist_unhashed(&p->mglist) &&
			    (timer_pending(&p->timer) ?
			     time_after(p->timer.expires, time) :
			     try_to_del_timer_sync(&p->timer) >= 0)) {
				mod_timer(&p->timer, time);
			}

			break;
		}
	}

	now = jiffies;
	time = now + brmctx->multicast_last_member_count *
		     brmctx->multicast_last_member_interval;

	if (!pmctx) {
		if (mp->host_joined &&
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		    (timer_pending(&mp->timer) ?
		     time_after(mp->timer.expires, time) :
		     try_to_del_timer_sync(&mp->timer) >= 0)) {
			mod_timer(&mp->timer, time);
		}

		goto out;
	}

<<<<<<< HEAD
	for (p = mlock_dereference(mp->ports, br);
	     p != NULL;
	     p = mlock_dereference(p->next, br)) {
		if (p->port != port)
=======
	for (p = mlock_dereference(mp->ports, brmctx->br);
	     p != NULL;
	     p = mlock_dereference(p->next, brmctx->br)) {
		if (p->key.port != pmctx->port)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			continue;

		if (!hlist_unhashed(&p->mglist) &&
		    (timer_pending(&p->timer) ?
		     time_after(p->timer.expires, time) :
		     try_to_del_timer_sync(&p->timer) >= 0)) {
			mod_timer(&p->timer, time);
		}

		break;
	}
<<<<<<< HEAD

out:
	spin_unlock(&br->multicast_lock);
}

static void br_ip4_multicast_leave_group(struct net_bridge *br,
					 struct net_bridge_port *port,
					 __be32 group)
{
	struct br_ip br_group;
=======
out:
	spin_unlock(&brmctx->br->multicast_lock);
}

static void br_ip4_multicast_leave_group(struct net_bridge_mcast *brmctx,
					 struct net_bridge_mcast_port *pmctx,
					 __be32 group,
					 __u16 vid,
					 const unsigned char *src)
{
	struct br_ip br_group;
	struct bridge_mcast_own_query *own_query;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (ipv4_is_local_multicast(group))
		return;

<<<<<<< HEAD
	br_group.u.ip4 = group;
	br_group.proto = htons(ETH_P_IP);

	br_multicast_leave_group(br, port, &br_group);
}

#if IS_ENABLED(CONFIG_IPV6)
static void br_ip6_multicast_leave_group(struct net_bridge *br,
					 struct net_bridge_port *port,
					 const struct in6_addr *group)
{
	struct br_ip br_group;

	if (!ipv6_is_transient_multicast(group))
		return;

	br_group.u.ip6 = *group;
	br_group.proto = htons(ETH_P_IPV6);

	br_multicast_leave_group(br, port, &br_group);
}
#endif

static int br_multicast_ipv4_rcv(struct net_bridge *br,
				 struct net_bridge_port *port,
				 struct sk_buff *skb)
{
	struct sk_buff *skb2 = skb;
	const struct iphdr *iph;
	struct igmphdr *ih;
	unsigned len;
	unsigned offset;
	int err;

	/* We treat OOM as packet loss for now. */
	if (!pskb_may_pull(skb, sizeof(*iph)))
		return -EINVAL;

	iph = ip_hdr(skb);

	if (iph->ihl < 5 || iph->version != 4)
		return -EINVAL;

	if (!pskb_may_pull(skb, ip_hdrlen(skb)))
		return -EINVAL;

	iph = ip_hdr(skb);

	if (unlikely(ip_fast_csum((u8 *)iph, iph->ihl)))
		return -EINVAL;

	if (iph->protocol != IPPROTO_IGMP) {
		if ((iph->daddr & IGMP_LOCAL_GROUP_MASK) != IGMP_LOCAL_GROUP)
			BR_INPUT_SKB_CB(skb)->mrouters_only = 1;
		return 0;
	}

	len = ntohs(iph->tot_len);
	if (skb->len < len || len < ip_hdrlen(skb))
		return -EINVAL;

	if (skb->len > len) {
		skb2 = skb_clone(skb, GFP_ATOMIC);
		if (!skb2)
			return -ENOMEM;

		err = pskb_trim_rcsum(skb2, len);
		if (err)
			goto err_out;
	}

	len -= ip_hdrlen(skb2);
	offset = skb_network_offset(skb2) + ip_hdrlen(skb2);
	__skb_pull(skb2, offset);
	skb_reset_transport_header(skb2);

	err = -EINVAL;
	if (!pskb_may_pull(skb2, sizeof(*ih)))
		goto out;

	switch (skb2->ip_summed) {
	case CHECKSUM_COMPLETE:
		if (!csum_fold(skb2->csum))
			break;
		/* fall through */
	case CHECKSUM_NONE:
		skb2->csum = 0;
		if (skb_checksum_complete(skb2))
			goto out;
	}

	err = 0;

	BR_INPUT_SKB_CB(skb)->igmp = 1;
	ih = igmp_hdr(skb2);
=======
	own_query = pmctx ? &pmctx->ip4_own_query : &brmctx->ip4_own_query;

	memset(&br_group, 0, sizeof(br_group));
	br_group.dst.ip4 = group;
	br_group.proto = htons(ETH_P_IP);
	br_group.vid = vid;

	br_multicast_leave_group(brmctx, pmctx, &br_group,
				 &brmctx->ip4_other_query,
				 own_query, src);
}

#if IS_ENABLED(CONFIG_IPV6)
static void br_ip6_multicast_leave_group(struct net_bridge_mcast *brmctx,
					 struct net_bridge_mcast_port *pmctx,
					 const struct in6_addr *group,
					 __u16 vid,
					 const unsigned char *src)
{
	struct br_ip br_group;
	struct bridge_mcast_own_query *own_query;

	if (ipv6_addr_is_ll_all_nodes(group))
		return;

	own_query = pmctx ? &pmctx->ip6_own_query : &brmctx->ip6_own_query;

	memset(&br_group, 0, sizeof(br_group));
	br_group.dst.ip6 = *group;
	br_group.proto = htons(ETH_P_IPV6);
	br_group.vid = vid;

	br_multicast_leave_group(brmctx, pmctx, &br_group,
				 &brmctx->ip6_other_query,
				 own_query, src);
}
#endif

static void br_multicast_err_count(const struct net_bridge *br,
				   const struct net_bridge_port *p,
				   __be16 proto)
{
	struct bridge_mcast_stats __percpu *stats;
	struct bridge_mcast_stats *pstats;

	if (!br_opt_get(br, BROPT_MULTICAST_STATS_ENABLED))
		return;

	if (p)
		stats = p->mcast_stats;
	else
		stats = br->mcast_stats;
	if (WARN_ON(!stats))
		return;

	pstats = this_cpu_ptr(stats);

	u64_stats_update_begin(&pstats->syncp);
	switch (proto) {
	case htons(ETH_P_IP):
		pstats->mstats.igmp_parse_errors++;
		break;
#if IS_ENABLED(CONFIG_IPV6)
	case htons(ETH_P_IPV6):
		pstats->mstats.mld_parse_errors++;
		break;
#endif
	}
	u64_stats_update_end(&pstats->syncp);
}

static void br_multicast_pim(struct net_bridge_mcast *brmctx,
			     struct net_bridge_mcast_port *pmctx,
			     const struct sk_buff *skb)
{
	unsigned int offset = skb_transport_offset(skb);
	struct pimhdr *pimhdr, _pimhdr;

	pimhdr = skb_header_pointer(skb, offset, sizeof(_pimhdr), &_pimhdr);
	if (!pimhdr || pim_hdr_version(pimhdr) != PIM_VERSION ||
	    pim_hdr_type(pimhdr) != PIM_TYPE_HELLO)
		return;

	spin_lock(&brmctx->br->multicast_lock);
	br_ip4_multicast_mark_router(brmctx, pmctx);
	spin_unlock(&brmctx->br->multicast_lock);
}

static int br_ip4_multicast_mrd_rcv(struct net_bridge_mcast *brmctx,
				    struct net_bridge_mcast_port *pmctx,
				    struct sk_buff *skb)
{
	if (ip_hdr(skb)->protocol != IPPROTO_IGMP ||
	    igmp_hdr(skb)->type != IGMP_MRDISC_ADV)
		return -ENOMSG;

	spin_lock(&brmctx->br->multicast_lock);
	br_ip4_multicast_mark_router(brmctx, pmctx);
	spin_unlock(&brmctx->br->multicast_lock);

	return 0;
}

static int br_multicast_ipv4_rcv(struct net_bridge_mcast *brmctx,
				 struct net_bridge_mcast_port *pmctx,
				 struct sk_buff *skb,
				 u16 vid)
{
	struct net_bridge_port *p = pmctx ? pmctx->port : NULL;
	const unsigned char *src;
	struct igmphdr *ih;
	int err;

	err = ip_mc_check_igmp(skb);

	if (err == -ENOMSG) {
		if (!ipv4_is_local_multicast(ip_hdr(skb)->daddr)) {
			BR_INPUT_SKB_CB(skb)->mrouters_only = 1;
		} else if (pim_ipv4_all_pim_routers(ip_hdr(skb)->daddr)) {
			if (ip_hdr(skb)->protocol == IPPROTO_PIM)
				br_multicast_pim(brmctx, pmctx, skb);
		} else if (ipv4_is_all_snoopers(ip_hdr(skb)->daddr)) {
			br_ip4_multicast_mrd_rcv(brmctx, pmctx, skb);
		}

		return 0;
	} else if (err < 0) {
		br_multicast_err_count(brmctx->br, p, skb->protocol);
		return err;
	}

	ih = igmp_hdr(skb);
	src = eth_hdr(skb)->h_source;
	BR_INPUT_SKB_CB(skb)->igmp = ih->type;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (ih->type) {
	case IGMP_HOST_MEMBERSHIP_REPORT:
	case IGMPV2_HOST_MEMBERSHIP_REPORT:
		BR_INPUT_SKB_CB(skb)->mrouters_only = 1;
<<<<<<< HEAD
		err = br_ip4_multicast_add_group(br, port, ih->group);
		break;
	case IGMPV3_HOST_MEMBERSHIP_REPORT:
		err = br_ip4_multicast_igmp3_report(br, port, skb2);
		break;
	case IGMP_HOST_MEMBERSHIP_QUERY:
		err = br_ip4_multicast_query(br, port, skb2);
		break;
	case IGMP_HOST_LEAVE_MESSAGE:
		br_ip4_multicast_leave_group(br, port, ih->group);
		break;
	}

out:
	__skb_push(skb2, offset);
err_out:
	if (skb2 != skb)
		kfree_skb(skb2);
=======
		err = br_ip4_multicast_add_group(brmctx, pmctx, ih->group, vid,
						 src, true);
		break;
	case IGMPV3_HOST_MEMBERSHIP_REPORT:
		err = br_ip4_multicast_igmp3_report(brmctx, pmctx, skb, vid);
		break;
	case IGMP_HOST_MEMBERSHIP_QUERY:
		br_ip4_multicast_query(brmctx, pmctx, skb, vid);
		break;
	case IGMP_HOST_LEAVE_MESSAGE:
		br_ip4_multicast_leave_group(brmctx, pmctx, ih->group, vid, src);
		break;
	}

	br_multicast_count(brmctx->br, p, skb, BR_INPUT_SKB_CB(skb)->igmp,
			   BR_MCAST_DIR_RX);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

#if IS_ENABLED(CONFIG_IPV6)
<<<<<<< HEAD
static int br_multicast_ipv6_rcv(struct net_bridge *br,
				 struct net_bridge_port *port,
				 struct sk_buff *skb)
{
	struct sk_buff *skb2;
	const struct ipv6hdr *ip6h;
	u8 icmp6_type;
	u8 nexthdr;
	__be16 frag_off;
	unsigned len;
	int offset;
	int err;

	if (!pskb_may_pull(skb, sizeof(*ip6h)))
		return -EINVAL;

	ip6h = ipv6_hdr(skb);

	/*
	 * We're interested in MLD messages only.
	 *  - Version is 6
	 *  - MLD has always Router Alert hop-by-hop option
	 *  - But we do not support jumbrograms.
	 */
	if (ip6h->version != 6 ||
	    ip6h->nexthdr != IPPROTO_HOPOPTS ||
	    ip6h->payload_len == 0)
		return 0;

	len = ntohs(ip6h->payload_len) + sizeof(*ip6h);
	if (skb->len < len)
		return -EINVAL;

	nexthdr = ip6h->nexthdr;
	offset = ipv6_skip_exthdr(skb, sizeof(*ip6h), &nexthdr, &frag_off);

	if (offset < 0 || nexthdr != IPPROTO_ICMPV6)
		return 0;

	/* Okay, we found ICMPv6 header */
	skb2 = skb_clone(skb, GFP_ATOMIC);
	if (!skb2)
		return -ENOMEM;

	err = -EINVAL;
	if (!pskb_may_pull(skb2, offset + sizeof(struct icmp6hdr)))
		goto out;

	len -= offset - skb_network_offset(skb2);

	__skb_pull(skb2, offset);
	skb_reset_transport_header(skb2);
	skb_postpull_rcsum(skb2, skb_network_header(skb2),
			   skb_network_header_len(skb2));

	icmp6_type = icmp6_hdr(skb2)->icmp6_type;

	switch (icmp6_type) {
	case ICMPV6_MGM_QUERY:
	case ICMPV6_MGM_REPORT:
	case ICMPV6_MGM_REDUCTION:
	case ICMPV6_MLD2_REPORT:
		break;
	default:
		err = 0;
		goto out;
	}

	/* Okay, we found MLD message. Check further. */
	if (skb2->len > len) {
		err = pskb_trim_rcsum(skb2, len);
		if (err)
			goto out;
		err = -EINVAL;
	}

	ip6h = ipv6_hdr(skb2);

	switch (skb2->ip_summed) {
	case CHECKSUM_COMPLETE:
		if (!csum_ipv6_magic(&ip6h->saddr, &ip6h->daddr, skb2->len,
					IPPROTO_ICMPV6, skb2->csum))
			break;
		/*FALLTHROUGH*/
	case CHECKSUM_NONE:
		skb2->csum = ~csum_unfold(csum_ipv6_magic(&ip6h->saddr,
							&ip6h->daddr,
							skb2->len,
							IPPROTO_ICMPV6, 0));
		if (__skb_checksum_complete(skb2))
			goto out;
	}

	err = 0;

	BR_INPUT_SKB_CB(skb)->igmp = 1;

	switch (icmp6_type) {
	case ICMPV6_MGM_REPORT:
	    {
		struct mld_msg *mld;
		if (!pskb_may_pull(skb2, sizeof(*mld))) {
			err = -EINVAL;
			goto out;
		}
		mld = (struct mld_msg *)skb_transport_header(skb2);
		BR_INPUT_SKB_CB(skb)->mrouters_only = 1;
		err = br_ip6_multicast_add_group(br, port, &mld->mld_mca);
		break;
	    }
	case ICMPV6_MLD2_REPORT:
		err = br_ip6_multicast_mld2_report(br, port, skb2);
		break;
	case ICMPV6_MGM_QUERY:
		err = br_ip6_multicast_query(br, port, skb2);
		break;
	case ICMPV6_MGM_REDUCTION:
	    {
		struct mld_msg *mld;
		if (!pskb_may_pull(skb2, sizeof(*mld))) {
			err = -EINVAL;
			goto out;
		}
		mld = (struct mld_msg *)skb_transport_header(skb2);
		br_ip6_multicast_leave_group(br, port, &mld->mld_mca);
	    }
	}

out:
	kfree_skb(skb2);
=======
static void br_ip6_multicast_mrd_rcv(struct net_bridge_mcast *brmctx,
				     struct net_bridge_mcast_port *pmctx,
				     struct sk_buff *skb)
{
	if (icmp6_hdr(skb)->icmp6_type != ICMPV6_MRDISC_ADV)
		return;

	spin_lock(&brmctx->br->multicast_lock);
	br_ip6_multicast_mark_router(brmctx, pmctx);
	spin_unlock(&brmctx->br->multicast_lock);
}

static int br_multicast_ipv6_rcv(struct net_bridge_mcast *brmctx,
				 struct net_bridge_mcast_port *pmctx,
				 struct sk_buff *skb,
				 u16 vid)
{
	struct net_bridge_port *p = pmctx ? pmctx->port : NULL;
	const unsigned char *src;
	struct mld_msg *mld;
	int err;

	err = ipv6_mc_check_mld(skb);

	if (err == -ENOMSG || err == -ENODATA) {
		if (!ipv6_addr_is_ll_all_nodes(&ipv6_hdr(skb)->daddr))
			BR_INPUT_SKB_CB(skb)->mrouters_only = 1;
		if (err == -ENODATA &&
		    ipv6_addr_is_all_snoopers(&ipv6_hdr(skb)->daddr))
			br_ip6_multicast_mrd_rcv(brmctx, pmctx, skb);

		return 0;
	} else if (err < 0) {
		br_multicast_err_count(brmctx->br, p, skb->protocol);
		return err;
	}

	mld = (struct mld_msg *)skb_transport_header(skb);
	BR_INPUT_SKB_CB(skb)->igmp = mld->mld_type;

	switch (mld->mld_type) {
	case ICMPV6_MGM_REPORT:
		src = eth_hdr(skb)->h_source;
		BR_INPUT_SKB_CB(skb)->mrouters_only = 1;
		err = br_ip6_multicast_add_group(brmctx, pmctx, &mld->mld_mca,
						 vid, src, true);
		break;
	case ICMPV6_MLD2_REPORT:
		err = br_ip6_multicast_mld2_report(brmctx, pmctx, skb, vid);
		break;
	case ICMPV6_MGM_QUERY:
		err = br_ip6_multicast_query(brmctx, pmctx, skb, vid);
		break;
	case ICMPV6_MGM_REDUCTION:
		src = eth_hdr(skb)->h_source;
		br_ip6_multicast_leave_group(brmctx, pmctx, &mld->mld_mca, vid,
					     src);
		break;
	}

	br_multicast_count(brmctx->br, p, skb, BR_INPUT_SKB_CB(skb)->igmp,
			   BR_MCAST_DIR_RX);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}
#endif

<<<<<<< HEAD
int br_multicast_rcv(struct net_bridge *br, struct net_bridge_port *port,
		     struct sk_buff *skb)
{
	BR_INPUT_SKB_CB(skb)->igmp = 0;
	BR_INPUT_SKB_CB(skb)->mrouters_only = 0;

	if (br->multicast_disabled)
		return 0;

	switch (skb->protocol) {
	case htons(ETH_P_IP):
		return br_multicast_ipv4_rcv(br, port, skb);
#if IS_ENABLED(CONFIG_IPV6)
	case htons(ETH_P_IPV6):
		return br_multicast_ipv6_rcv(br, port, skb);
#endif
	}

	return 0;
}

static void br_multicast_query_expired(unsigned long data)
{
	struct net_bridge *br = (void *)data;

	spin_lock(&br->multicast_lock);
	if (br->multicast_startup_queries_sent <
	    br->multicast_startup_query_count)
		br->multicast_startup_queries_sent++;

	br_multicast_send_query(br, NULL, br->multicast_startup_queries_sent);

	spin_unlock(&br->multicast_lock);
}

void br_multicast_init(struct net_bridge *br)
{
	br->hash_elasticity = 4;
	br->hash_max = 512;

	br->multicast_router = 1;
	br->multicast_last_member_count = 2;
	br->multicast_startup_query_count = 2;

	br->multicast_last_member_interval = HZ;
	br->multicast_query_response_interval = 10 * HZ;
	br->multicast_startup_query_interval = 125 * HZ / 4;
	br->multicast_query_interval = 125 * HZ;
	br->multicast_querier_interval = 255 * HZ;
	br->multicast_membership_interval = 260 * HZ;

	spin_lock_init(&br->multicast_lock);
	setup_timer(&br->multicast_router_timer,
		    br_multicast_local_router_expired, 0);
	setup_timer(&br->multicast_querier_timer,
		    br_multicast_local_router_expired, 0);
	setup_timer(&br->multicast_query_timer, br_multicast_query_expired,
		    (unsigned long)br);
}

void br_multicast_open(struct net_bridge *br)
{
	br->multicast_startup_queries_sent = 0;

	if (br->multicast_disabled)
		return;

	mod_timer(&br->multicast_query_timer, jiffies);
}

void br_multicast_stop(struct net_bridge *br)
{
	struct net_bridge_mdb_htable *mdb;
	struct net_bridge_mdb_entry *mp;
	struct hlist_node *p, *n;
	u32 ver;
	int i;

	del_timer_sync(&br->multicast_router_timer);
	del_timer_sync(&br->multicast_querier_timer);
	del_timer_sync(&br->multicast_query_timer);

	spin_lock_bh(&br->multicast_lock);
	mdb = mlock_dereference(br->mdb, br);
	if (!mdb)
		goto out;

	br->mdb = NULL;

	ver = mdb->ver;
	for (i = 0; i < mdb->max; i++) {
		hlist_for_each_entry_safe(mp, p, n, &mdb->mhash[i],
					  hlist[ver]) {
			del_timer(&mp->timer);
			call_rcu_bh(&mp->rcu, br_multicast_free_group);
		}
	}

	if (mdb->old) {
		spin_unlock_bh(&br->multicast_lock);
		rcu_barrier_bh();
		spin_lock_bh(&br->multicast_lock);
		WARN_ON(mdb->old);
	}

	mdb->old = mdb;
	call_rcu_bh(&mdb->rcu, br_mdb_free);

out:
	spin_unlock_bh(&br->multicast_lock);
}

int br_multicast_set_router(struct net_bridge *br, unsigned long val)
{
	int err = -ENOENT;

	spin_lock_bh(&br->multicast_lock);
	if (!netif_running(br->dev))
		goto unlock;

	switch (val) {
	case 0:
	case 2:
		del_timer(&br->multicast_router_timer);
		/* fall through */
	case 1:
		br->multicast_router = val;
		err = 0;
		break;

	default:
		err = -EINVAL;
		break;
	}

unlock:
	spin_unlock_bh(&br->multicast_lock);

	return err;
}

int br_multicast_set_port_router(struct net_bridge_port *p, unsigned long val)
{
	struct net_bridge *br = p->br;
	int err = -ENOENT;

	spin_lock(&br->multicast_lock);
	if (!netif_running(br->dev) || p->state == BR_STATE_DISABLED)
		goto unlock;

	switch (val) {
	case 0:
	case 1:
	case 2:
		p->multicast_router = val;
		err = 0;

		if (val < 2 && !hlist_unhashed(&p->rlist))
			hlist_del_init_rcu(&p->rlist);

		if (val == 1)
			break;

		del_timer(&p->multicast_router_timer);

		if (val == 0)
			break;

		br_multicast_add_router(br, p);
		break;

	default:
		err = -EINVAL;
		break;
	}

unlock:
	spin_unlock(&br->multicast_lock);

	return err;
}

int br_multicast_toggle(struct net_bridge *br, unsigned long val)
{
	struct net_bridge_port *port;
	int err = 0;
	struct net_bridge_mdb_htable *mdb;

	spin_lock_bh(&br->multicast_lock);
	if (br->multicast_disabled == !val)
		goto unlock;

	br->multicast_disabled = !val;
	if (br->multicast_disabled)
		goto unlock;

	if (!netif_running(br->dev))
		goto unlock;

	mdb = mlock_dereference(br->mdb, br);
	if (mdb) {
		if (mdb->old) {
			err = -EEXIST;
rollback:
			br->multicast_disabled = !!val;
			goto unlock;
		}

		err = br_mdb_rehash(&br->mdb, mdb->max,
				    br->hash_elasticity);
		if (err)
			goto rollback;
	}

	br_multicast_open(br);
	list_for_each_entry(port, &br->port_list, list) {
		if (port->state == BR_STATE_DISABLED ||
		    port->state == BR_STATE_BLOCKING)
			continue;

		__br_multicast_enable_port(port);
	}

unlock:
	spin_unlock_bh(&br->multicast_lock);

	return err;
}

int br_multicast_set_hash_max(struct net_bridge *br, unsigned long val)
{
	int err = -ENOENT;
	u32 old;
	struct net_bridge_mdb_htable *mdb;

	spin_lock_bh(&br->multicast_lock);
	if (!netif_running(br->dev))
		goto unlock;

	err = -EINVAL;
	if (!is_power_of_2(val))
		goto unlock;

	mdb = mlock_dereference(br->mdb, br);
	if (mdb && val < mdb->size)
		goto unlock;

	err = 0;

	old = br->hash_max;
	br->hash_max = val;

	if (mdb) {
		if (mdb->old) {
			err = -EEXIST;
rollback:
			br->hash_max = old;
			goto unlock;
		}

		err = br_mdb_rehash(&br->mdb, br->hash_max,
				    br->hash_elasticity);
		if (err)
			goto rollback;
	}

unlock:
	spin_unlock_bh(&br->multicast_lock);

	return err;
=======
int br_multicast_rcv(struct net_bridge_mcast **brmctx,
		     struct net_bridge_mcast_port **pmctx,
		     struct net_bridge_vlan *vlan,
		     struct sk_buff *skb, u16 vid)
{
	int ret = 0;

	BR_INPUT_SKB_CB(skb)->igmp = 0;
	BR_INPUT_SKB_CB(skb)->mrouters_only = 0;

	if (!br_opt_get((*brmctx)->br, BROPT_MULTICAST_ENABLED))
		return 0;

	if (br_opt_get((*brmctx)->br, BROPT_MCAST_VLAN_SNOOPING_ENABLED) && vlan) {
		const struct net_bridge_vlan *masterv;

		/* the vlan has the master flag set only when transmitting
		 * through the bridge device
		 */
		if (br_vlan_is_master(vlan)) {
			masterv = vlan;
			*brmctx = &vlan->br_mcast_ctx;
			*pmctx = NULL;
		} else {
			masterv = vlan->brvlan;
			*brmctx = &vlan->brvlan->br_mcast_ctx;
			*pmctx = &vlan->port_mcast_ctx;
		}

		if (!(masterv->priv_flags & BR_VLFLAG_GLOBAL_MCAST_ENABLED))
			return 0;
	}

	switch (skb->protocol) {
	case htons(ETH_P_IP):
		ret = br_multicast_ipv4_rcv(*brmctx, *pmctx, skb, vid);
		break;
#if IS_ENABLED(CONFIG_IPV6)
	case htons(ETH_P_IPV6):
		ret = br_multicast_ipv6_rcv(*brmctx, *pmctx, skb, vid);
		break;
#endif
	}

	return ret;
}

static void br_multicast_query_expired(struct net_bridge_mcast *brmctx,
				       struct bridge_mcast_own_query *query,
				       struct bridge_mcast_querier *querier)
{
	spin_lock(&brmctx->br->multicast_lock);
	if (br_multicast_ctx_vlan_disabled(brmctx))
		goto out;

	if (query->startup_sent < brmctx->multicast_startup_query_count)
		query->startup_sent++;

	br_multicast_send_query(brmctx, NULL, query);
out:
	spin_unlock(&brmctx->br->multicast_lock);
}

static void br_ip4_multicast_query_expired(struct timer_list *t)
{
	struct net_bridge_mcast *brmctx = from_timer(brmctx, t,
						     ip4_own_query.timer);

	br_multicast_query_expired(brmctx, &brmctx->ip4_own_query,
				   &brmctx->ip4_querier);
}

#if IS_ENABLED(CONFIG_IPV6)
static void br_ip6_multicast_query_expired(struct timer_list *t)
{
	struct net_bridge_mcast *brmctx = from_timer(brmctx, t,
						     ip6_own_query.timer);

	br_multicast_query_expired(brmctx, &brmctx->ip6_own_query,
				   &brmctx->ip6_querier);
}
#endif

static void br_multicast_gc_work(struct work_struct *work)
{
	struct net_bridge *br = container_of(work, struct net_bridge,
					     mcast_gc_work);
	HLIST_HEAD(deleted_head);

	spin_lock_bh(&br->multicast_lock);
	hlist_move_list(&br->mcast_gc_list, &deleted_head);
	spin_unlock_bh(&br->multicast_lock);

	br_multicast_gc(&deleted_head);
}

void br_multicast_ctx_init(struct net_bridge *br,
			   struct net_bridge_vlan *vlan,
			   struct net_bridge_mcast *brmctx)
{
	brmctx->br = br;
	brmctx->vlan = vlan;
	brmctx->multicast_router = MDB_RTR_TYPE_TEMP_QUERY;
	brmctx->multicast_last_member_count = 2;
	brmctx->multicast_startup_query_count = 2;

	brmctx->multicast_last_member_interval = HZ;
	brmctx->multicast_query_response_interval = 10 * HZ;
	brmctx->multicast_startup_query_interval = 125 * HZ / 4;
	brmctx->multicast_query_interval = 125 * HZ;
	brmctx->multicast_querier_interval = 255 * HZ;
	brmctx->multicast_membership_interval = 260 * HZ;

	brmctx->ip4_querier.port_ifidx = 0;
	seqcount_spinlock_init(&brmctx->ip4_querier.seq, &br->multicast_lock);
	brmctx->multicast_igmp_version = 2;
#if IS_ENABLED(CONFIG_IPV6)
	brmctx->multicast_mld_version = 1;
	brmctx->ip6_querier.port_ifidx = 0;
	seqcount_spinlock_init(&brmctx->ip6_querier.seq, &br->multicast_lock);
#endif

	timer_setup(&brmctx->ip4_mc_router_timer,
		    br_ip4_multicast_local_router_expired, 0);
	timer_setup(&brmctx->ip4_other_query.timer,
		    br_ip4_multicast_querier_expired, 0);
	timer_setup(&brmctx->ip4_other_query.delay_timer,
		    br_multicast_query_delay_expired, 0);
	timer_setup(&brmctx->ip4_own_query.timer,
		    br_ip4_multicast_query_expired, 0);
#if IS_ENABLED(CONFIG_IPV6)
	timer_setup(&brmctx->ip6_mc_router_timer,
		    br_ip6_multicast_local_router_expired, 0);
	timer_setup(&brmctx->ip6_other_query.timer,
		    br_ip6_multicast_querier_expired, 0);
	timer_setup(&brmctx->ip6_other_query.delay_timer,
		    br_multicast_query_delay_expired, 0);
	timer_setup(&brmctx->ip6_own_query.timer,
		    br_ip6_multicast_query_expired, 0);
#endif
}

void br_multicast_ctx_deinit(struct net_bridge_mcast *brmctx)
{
	__br_multicast_stop(brmctx);
}

void br_multicast_init(struct net_bridge *br)
{
	br->hash_max = BR_MULTICAST_DEFAULT_HASH_MAX;

	br_multicast_ctx_init(br, NULL, &br->multicast_ctx);

	br_opt_toggle(br, BROPT_MULTICAST_ENABLED, true);
	br_opt_toggle(br, BROPT_HAS_IPV6_ADDR, true);

	spin_lock_init(&br->multicast_lock);
	INIT_HLIST_HEAD(&br->mdb_list);
	INIT_HLIST_HEAD(&br->mcast_gc_list);
	INIT_WORK(&br->mcast_gc_work, br_multicast_gc_work);
}

static void br_ip4_multicast_join_snoopers(struct net_bridge *br)
{
	struct in_device *in_dev = in_dev_get(br->dev);

	if (!in_dev)
		return;

	__ip_mc_inc_group(in_dev, htonl(INADDR_ALLSNOOPERS_GROUP), GFP_ATOMIC);
	in_dev_put(in_dev);
}

#if IS_ENABLED(CONFIG_IPV6)
static void br_ip6_multicast_join_snoopers(struct net_bridge *br)
{
	struct in6_addr addr;

	ipv6_addr_set(&addr, htonl(0xff020000), 0, 0, htonl(0x6a));
	ipv6_dev_mc_inc(br->dev, &addr);
}
#else
static inline void br_ip6_multicast_join_snoopers(struct net_bridge *br)
{
}
#endif

void br_multicast_join_snoopers(struct net_bridge *br)
{
	br_ip4_multicast_join_snoopers(br);
	br_ip6_multicast_join_snoopers(br);
}

static void br_ip4_multicast_leave_snoopers(struct net_bridge *br)
{
	struct in_device *in_dev = in_dev_get(br->dev);

	if (WARN_ON(!in_dev))
		return;

	__ip_mc_dec_group(in_dev, htonl(INADDR_ALLSNOOPERS_GROUP), GFP_ATOMIC);
	in_dev_put(in_dev);
}

#if IS_ENABLED(CONFIG_IPV6)
static void br_ip6_multicast_leave_snoopers(struct net_bridge *br)
{
	struct in6_addr addr;

	ipv6_addr_set(&addr, htonl(0xff020000), 0, 0, htonl(0x6a));
	ipv6_dev_mc_dec(br->dev, &addr);
}
#else
static inline void br_ip6_multicast_leave_snoopers(struct net_bridge *br)
{
}
#endif

void br_multicast_leave_snoopers(struct net_bridge *br)
{
	br_ip4_multicast_leave_snoopers(br);
	br_ip6_multicast_leave_snoopers(br);
}

static void __br_multicast_open_query(struct net_bridge *br,
				      struct bridge_mcast_own_query *query)
{
	query->startup_sent = 0;

	if (!br_opt_get(br, BROPT_MULTICAST_ENABLED))
		return;

	mod_timer(&query->timer, jiffies);
}

static void __br_multicast_open(struct net_bridge_mcast *brmctx)
{
	__br_multicast_open_query(brmctx->br, &brmctx->ip4_own_query);
#if IS_ENABLED(CONFIG_IPV6)
	__br_multicast_open_query(brmctx->br, &brmctx->ip6_own_query);
#endif
}

void br_multicast_open(struct net_bridge *br)
{
	ASSERT_RTNL();

	if (br_opt_get(br, BROPT_MCAST_VLAN_SNOOPING_ENABLED)) {
		struct net_bridge_vlan_group *vg;
		struct net_bridge_vlan *vlan;

		vg = br_vlan_group(br);
		if (vg) {
			list_for_each_entry(vlan, &vg->vlan_list, vlist) {
				struct net_bridge_mcast *brmctx;

				brmctx = &vlan->br_mcast_ctx;
				if (br_vlan_is_brentry(vlan) &&
				    !br_multicast_ctx_vlan_disabled(brmctx))
					__br_multicast_open(&vlan->br_mcast_ctx);
			}
		}
	} else {
		__br_multicast_open(&br->multicast_ctx);
	}
}

static void __br_multicast_stop(struct net_bridge_mcast *brmctx)
{
	del_timer_sync(&brmctx->ip4_mc_router_timer);
	del_timer_sync(&brmctx->ip4_other_query.timer);
	del_timer_sync(&brmctx->ip4_other_query.delay_timer);
	del_timer_sync(&brmctx->ip4_own_query.timer);
#if IS_ENABLED(CONFIG_IPV6)
	del_timer_sync(&brmctx->ip6_mc_router_timer);
	del_timer_sync(&brmctx->ip6_other_query.timer);
	del_timer_sync(&brmctx->ip6_other_query.delay_timer);
	del_timer_sync(&brmctx->ip6_own_query.timer);
#endif
}

void br_multicast_toggle_one_vlan(struct net_bridge_vlan *vlan, bool on)
{
	struct net_bridge *br;

	/* it's okay to check for the flag without the multicast lock because it
	 * can only change under RTNL -> multicast_lock, we need the latter to
	 * sync with timers and packets
	 */
	if (on == !!(vlan->priv_flags & BR_VLFLAG_MCAST_ENABLED))
		return;

	if (br_vlan_is_master(vlan)) {
		br = vlan->br;

		if (!br_vlan_is_brentry(vlan) ||
		    (on &&
		     br_multicast_ctx_vlan_global_disabled(&vlan->br_mcast_ctx)))
			return;

		spin_lock_bh(&br->multicast_lock);
		vlan->priv_flags ^= BR_VLFLAG_MCAST_ENABLED;
		spin_unlock_bh(&br->multicast_lock);

		if (on)
			__br_multicast_open(&vlan->br_mcast_ctx);
		else
			__br_multicast_stop(&vlan->br_mcast_ctx);
	} else {
		struct net_bridge_mcast *brmctx;

		brmctx = br_multicast_port_ctx_get_global(&vlan->port_mcast_ctx);
		if (on && br_multicast_ctx_vlan_global_disabled(brmctx))
			return;

		br = vlan->port->br;
		spin_lock_bh(&br->multicast_lock);
		vlan->priv_flags ^= BR_VLFLAG_MCAST_ENABLED;
		if (on)
			__br_multicast_enable_port_ctx(&vlan->port_mcast_ctx);
		else
			__br_multicast_disable_port_ctx(&vlan->port_mcast_ctx);
		spin_unlock_bh(&br->multicast_lock);
	}
}

static void br_multicast_toggle_vlan(struct net_bridge_vlan *vlan, bool on)
{
	struct net_bridge_port *p;

	if (WARN_ON_ONCE(!br_vlan_is_master(vlan)))
		return;

	list_for_each_entry(p, &vlan->br->port_list, list) {
		struct net_bridge_vlan *vport;

		vport = br_vlan_find(nbp_vlan_group(p), vlan->vid);
		if (!vport)
			continue;
		br_multicast_toggle_one_vlan(vport, on);
	}

	if (br_vlan_is_brentry(vlan))
		br_multicast_toggle_one_vlan(vlan, on);
}

int br_multicast_toggle_vlan_snooping(struct net_bridge *br, bool on,
				      struct netlink_ext_ack *extack)
{
	struct net_bridge_vlan_group *vg;
	struct net_bridge_vlan *vlan;
	struct net_bridge_port *p;

	if (br_opt_get(br, BROPT_MCAST_VLAN_SNOOPING_ENABLED) == on)
		return 0;

	if (on && !br_opt_get(br, BROPT_VLAN_ENABLED)) {
		NL_SET_ERR_MSG_MOD(extack, "Cannot enable multicast vlan snooping with vlan filtering disabled");
		return -EINVAL;
	}

	vg = br_vlan_group(br);
	if (!vg)
		return 0;

	br_opt_toggle(br, BROPT_MCAST_VLAN_SNOOPING_ENABLED, on);

	/* disable/enable non-vlan mcast contexts based on vlan snooping */
	if (on)
		__br_multicast_stop(&br->multicast_ctx);
	else
		__br_multicast_open(&br->multicast_ctx);
	list_for_each_entry(p, &br->port_list, list) {
		if (on)
			br_multicast_disable_port(p);
		else
			br_multicast_enable_port(p);
	}

	list_for_each_entry(vlan, &vg->vlan_list, vlist)
		br_multicast_toggle_vlan(vlan, on);

	return 0;
}

bool br_multicast_toggle_global_vlan(struct net_bridge_vlan *vlan, bool on)
{
	ASSERT_RTNL();

	/* BR_VLFLAG_GLOBAL_MCAST_ENABLED relies on eventual consistency and
	 * requires only RTNL to change
	 */
	if (on == !!(vlan->priv_flags & BR_VLFLAG_GLOBAL_MCAST_ENABLED))
		return false;

	vlan->priv_flags ^= BR_VLFLAG_GLOBAL_MCAST_ENABLED;
	br_multicast_toggle_vlan(vlan, on);

	return true;
}

void br_multicast_stop(struct net_bridge *br)
{
	ASSERT_RTNL();

	if (br_opt_get(br, BROPT_MCAST_VLAN_SNOOPING_ENABLED)) {
		struct net_bridge_vlan_group *vg;
		struct net_bridge_vlan *vlan;

		vg = br_vlan_group(br);
		if (vg) {
			list_for_each_entry(vlan, &vg->vlan_list, vlist) {
				struct net_bridge_mcast *brmctx;

				brmctx = &vlan->br_mcast_ctx;
				if (br_vlan_is_brentry(vlan) &&
				    !br_multicast_ctx_vlan_disabled(brmctx))
					__br_multicast_stop(&vlan->br_mcast_ctx);
			}
		}
	} else {
		__br_multicast_stop(&br->multicast_ctx);
	}
}

void br_multicast_dev_del(struct net_bridge *br)
{
	struct net_bridge_mdb_entry *mp;
	HLIST_HEAD(deleted_head);
	struct hlist_node *tmp;

	spin_lock_bh(&br->multicast_lock);
	hlist_for_each_entry_safe(mp, tmp, &br->mdb_list, mdb_node)
		br_multicast_del_mdb_entry(mp);
	hlist_move_list(&br->mcast_gc_list, &deleted_head);
	spin_unlock_bh(&br->multicast_lock);

	br_multicast_ctx_deinit(&br->multicast_ctx);
	br_multicast_gc(&deleted_head);
	cancel_work_sync(&br->mcast_gc_work);

	rcu_barrier();
}

int br_multicast_set_router(struct net_bridge_mcast *brmctx, unsigned long val)
{
	int err = -EINVAL;

	spin_lock_bh(&brmctx->br->multicast_lock);

	switch (val) {
	case MDB_RTR_TYPE_DISABLED:
	case MDB_RTR_TYPE_PERM:
		br_mc_router_state_change(brmctx->br, val == MDB_RTR_TYPE_PERM);
		del_timer(&brmctx->ip4_mc_router_timer);
#if IS_ENABLED(CONFIG_IPV6)
		del_timer(&brmctx->ip6_mc_router_timer);
#endif
		brmctx->multicast_router = val;
		err = 0;
		break;
	case MDB_RTR_TYPE_TEMP_QUERY:
		if (brmctx->multicast_router != MDB_RTR_TYPE_TEMP_QUERY)
			br_mc_router_state_change(brmctx->br, false);
		brmctx->multicast_router = val;
		err = 0;
		break;
	}

	spin_unlock_bh(&brmctx->br->multicast_lock);

	return err;
}

static void
br_multicast_rport_del_notify(struct net_bridge_mcast_port *pmctx, bool deleted)
{
	if (!deleted)
		return;

	/* For backwards compatibility for now, only notify if there is
	 * no multicast router anymore for both IPv4 and IPv6.
	 */
	if (!hlist_unhashed(&pmctx->ip4_rlist))
		return;
#if IS_ENABLED(CONFIG_IPV6)
	if (!hlist_unhashed(&pmctx->ip6_rlist))
		return;
#endif

	br_rtr_notify(pmctx->port->br->dev, pmctx, RTM_DELMDB);
	br_port_mc_router_state_change(pmctx->port, false);

	/* don't allow timer refresh */
	if (pmctx->multicast_router == MDB_RTR_TYPE_TEMP)
		pmctx->multicast_router = MDB_RTR_TYPE_TEMP_QUERY;
}

int br_multicast_set_port_router(struct net_bridge_mcast_port *pmctx,
				 unsigned long val)
{
	struct net_bridge_mcast *brmctx;
	unsigned long now = jiffies;
	int err = -EINVAL;
	bool del = false;

	brmctx = br_multicast_port_ctx_get_global(pmctx);
	spin_lock_bh(&brmctx->br->multicast_lock);
	if (pmctx->multicast_router == val) {
		/* Refresh the temp router port timer */
		if (pmctx->multicast_router == MDB_RTR_TYPE_TEMP) {
			mod_timer(&pmctx->ip4_mc_router_timer,
				  now + brmctx->multicast_querier_interval);
#if IS_ENABLED(CONFIG_IPV6)
			mod_timer(&pmctx->ip6_mc_router_timer,
				  now + brmctx->multicast_querier_interval);
#endif
		}
		err = 0;
		goto unlock;
	}
	switch (val) {
	case MDB_RTR_TYPE_DISABLED:
		pmctx->multicast_router = MDB_RTR_TYPE_DISABLED;
		del |= br_ip4_multicast_rport_del(pmctx);
		del_timer(&pmctx->ip4_mc_router_timer);
		del |= br_ip6_multicast_rport_del(pmctx);
#if IS_ENABLED(CONFIG_IPV6)
		del_timer(&pmctx->ip6_mc_router_timer);
#endif
		br_multicast_rport_del_notify(pmctx, del);
		break;
	case MDB_RTR_TYPE_TEMP_QUERY:
		pmctx->multicast_router = MDB_RTR_TYPE_TEMP_QUERY;
		del |= br_ip4_multicast_rport_del(pmctx);
		del |= br_ip6_multicast_rport_del(pmctx);
		br_multicast_rport_del_notify(pmctx, del);
		break;
	case MDB_RTR_TYPE_PERM:
		pmctx->multicast_router = MDB_RTR_TYPE_PERM;
		del_timer(&pmctx->ip4_mc_router_timer);
		br_ip4_multicast_add_router(brmctx, pmctx);
#if IS_ENABLED(CONFIG_IPV6)
		del_timer(&pmctx->ip6_mc_router_timer);
#endif
		br_ip6_multicast_add_router(brmctx, pmctx);
		break;
	case MDB_RTR_TYPE_TEMP:
		pmctx->multicast_router = MDB_RTR_TYPE_TEMP;
		br_ip4_multicast_mark_router(brmctx, pmctx);
		br_ip6_multicast_mark_router(brmctx, pmctx);
		break;
	default:
		goto unlock;
	}
	err = 0;
unlock:
	spin_unlock_bh(&brmctx->br->multicast_lock);

	return err;
}

int br_multicast_set_vlan_router(struct net_bridge_vlan *v, u8 mcast_router)
{
	int err;

	if (br_vlan_is_master(v))
		err = br_multicast_set_router(&v->br_mcast_ctx, mcast_router);
	else
		err = br_multicast_set_port_router(&v->port_mcast_ctx,
						   mcast_router);

	return err;
}

static void br_multicast_start_querier(struct net_bridge_mcast *brmctx,
				       struct bridge_mcast_own_query *query)
{
	struct net_bridge_port *port;

	if (!br_multicast_ctx_matches_vlan_snooping(brmctx))
		return;

	__br_multicast_open_query(brmctx->br, query);

	rcu_read_lock();
	list_for_each_entry_rcu(port, &brmctx->br->port_list, list) {
		struct bridge_mcast_own_query *ip4_own_query;
#if IS_ENABLED(CONFIG_IPV6)
		struct bridge_mcast_own_query *ip6_own_query;
#endif

		if (br_multicast_port_ctx_state_stopped(&port->multicast_ctx))
			continue;

		if (br_multicast_ctx_is_vlan(brmctx)) {
			struct net_bridge_vlan *vlan;

			vlan = br_vlan_find(nbp_vlan_group_rcu(port),
					    brmctx->vlan->vid);
			if (!vlan ||
			    br_multicast_port_ctx_state_stopped(&vlan->port_mcast_ctx))
				continue;

			ip4_own_query = &vlan->port_mcast_ctx.ip4_own_query;
#if IS_ENABLED(CONFIG_IPV6)
			ip6_own_query = &vlan->port_mcast_ctx.ip6_own_query;
#endif
		} else {
			ip4_own_query = &port->multicast_ctx.ip4_own_query;
#if IS_ENABLED(CONFIG_IPV6)
			ip6_own_query = &port->multicast_ctx.ip6_own_query;
#endif
		}

		if (query == &brmctx->ip4_own_query)
			br_multicast_enable(ip4_own_query);
#if IS_ENABLED(CONFIG_IPV6)
		else
			br_multicast_enable(ip6_own_query);
#endif
	}
	rcu_read_unlock();
}

int br_multicast_toggle(struct net_bridge *br, unsigned long val,
			struct netlink_ext_ack *extack)
{
	struct net_bridge_port *port;
	bool change_snoopers = false;
	int err = 0;

	spin_lock_bh(&br->multicast_lock);
	if (!!br_opt_get(br, BROPT_MULTICAST_ENABLED) == !!val)
		goto unlock;

	err = br_mc_disabled_update(br->dev, val, extack);
	if (err == -EOPNOTSUPP)
		err = 0;
	if (err)
		goto unlock;

	br_opt_toggle(br, BROPT_MULTICAST_ENABLED, !!val);
	if (!br_opt_get(br, BROPT_MULTICAST_ENABLED)) {
		change_snoopers = true;
		goto unlock;
	}

	if (!netif_running(br->dev))
		goto unlock;

	br_multicast_open(br);
	list_for_each_entry(port, &br->port_list, list)
		__br_multicast_enable_port_ctx(&port->multicast_ctx);

	change_snoopers = true;

unlock:
	spin_unlock_bh(&br->multicast_lock);

	/* br_multicast_join_snoopers has the potential to cause
	 * an MLD Report/Leave to be delivered to br_multicast_rcv,
	 * which would in turn call br_multicast_add_group, which would
	 * attempt to acquire multicast_lock. This function should be
	 * called after the lock has been released to avoid deadlocks on
	 * multicast_lock.
	 *
	 * br_multicast_leave_snoopers does not have the problem since
	 * br_multicast_rcv first checks BROPT_MULTICAST_ENABLED, and
	 * returns without calling br_multicast_ipv4/6_rcv if it's not
	 * enabled. Moved both functions out just for symmetry.
	 */
	if (change_snoopers) {
		if (br_opt_get(br, BROPT_MULTICAST_ENABLED))
			br_multicast_join_snoopers(br);
		else
			br_multicast_leave_snoopers(br);
	}

	return err;
}

bool br_multicast_enabled(const struct net_device *dev)
{
	struct net_bridge *br = netdev_priv(dev);

	return !!br_opt_get(br, BROPT_MULTICAST_ENABLED);
}
EXPORT_SYMBOL_GPL(br_multicast_enabled);

bool br_multicast_router(const struct net_device *dev)
{
	struct net_bridge *br = netdev_priv(dev);
	bool is_router;

	spin_lock_bh(&br->multicast_lock);
	is_router = br_multicast_is_router(&br->multicast_ctx, NULL);
	spin_unlock_bh(&br->multicast_lock);
	return is_router;
}
EXPORT_SYMBOL_GPL(br_multicast_router);

int br_multicast_set_querier(struct net_bridge_mcast *brmctx, unsigned long val)
{
	unsigned long max_delay;

	val = !!val;

	spin_lock_bh(&brmctx->br->multicast_lock);
	if (brmctx->multicast_querier == val)
		goto unlock;

	WRITE_ONCE(brmctx->multicast_querier, val);
	if (!val)
		goto unlock;

	max_delay = brmctx->multicast_query_response_interval;

	if (!timer_pending(&brmctx->ip4_other_query.timer))
		mod_timer(&brmctx->ip4_other_query.delay_timer,
			  jiffies + max_delay);

	br_multicast_start_querier(brmctx, &brmctx->ip4_own_query);

#if IS_ENABLED(CONFIG_IPV6)
	if (!timer_pending(&brmctx->ip6_other_query.timer))
		mod_timer(&brmctx->ip6_other_query.delay_timer,
			  jiffies + max_delay);

	br_multicast_start_querier(brmctx, &brmctx->ip6_own_query);
#endif

unlock:
	spin_unlock_bh(&brmctx->br->multicast_lock);

	return 0;
}

int br_multicast_set_igmp_version(struct net_bridge_mcast *brmctx,
				  unsigned long val)
{
	/* Currently we support only version 2 and 3 */
	switch (val) {
	case 2:
	case 3:
		break;
	default:
		return -EINVAL;
	}

	spin_lock_bh(&brmctx->br->multicast_lock);
	brmctx->multicast_igmp_version = val;
	spin_unlock_bh(&brmctx->br->multicast_lock);

	return 0;
}

#if IS_ENABLED(CONFIG_IPV6)
int br_multicast_set_mld_version(struct net_bridge_mcast *brmctx,
				 unsigned long val)
{
	/* Currently we support version 1 and 2 */
	switch (val) {
	case 1:
	case 2:
		break;
	default:
		return -EINVAL;
	}

	spin_lock_bh(&brmctx->br->multicast_lock);
	brmctx->multicast_mld_version = val;
	spin_unlock_bh(&brmctx->br->multicast_lock);

	return 0;
}
#endif

void br_multicast_set_query_intvl(struct net_bridge_mcast *brmctx,
				  unsigned long val)
{
	unsigned long intvl_jiffies = clock_t_to_jiffies(val);

	if (intvl_jiffies < BR_MULTICAST_QUERY_INTVL_MIN) {
		br_info(brmctx->br,
			"trying to set multicast query interval below minimum, setting to %lu (%ums)\n",
			jiffies_to_clock_t(BR_MULTICAST_QUERY_INTVL_MIN),
			jiffies_to_msecs(BR_MULTICAST_QUERY_INTVL_MIN));
		intvl_jiffies = BR_MULTICAST_QUERY_INTVL_MIN;
	}

	brmctx->multicast_query_interval = intvl_jiffies;
}

void br_multicast_set_startup_query_intvl(struct net_bridge_mcast *brmctx,
					  unsigned long val)
{
	unsigned long intvl_jiffies = clock_t_to_jiffies(val);

	if (intvl_jiffies < BR_MULTICAST_STARTUP_QUERY_INTVL_MIN) {
		br_info(brmctx->br,
			"trying to set multicast startup query interval below minimum, setting to %lu (%ums)\n",
			jiffies_to_clock_t(BR_MULTICAST_STARTUP_QUERY_INTVL_MIN),
			jiffies_to_msecs(BR_MULTICAST_STARTUP_QUERY_INTVL_MIN));
		intvl_jiffies = BR_MULTICAST_STARTUP_QUERY_INTVL_MIN;
	}

	brmctx->multicast_startup_query_interval = intvl_jiffies;
}

/**
 * br_multicast_list_adjacent - Returns snooped multicast addresses
 * @dev:	The bridge port adjacent to which to retrieve addresses
 * @br_ip_list:	The list to store found, snooped multicast IP addresses in
 *
 * Creates a list of IP addresses (struct br_ip_list) sensed by the multicast
 * snooping feature on all bridge ports of dev's bridge device, excluding
 * the addresses from dev itself.
 *
 * Returns the number of items added to br_ip_list.
 *
 * Notes:
 * - br_ip_list needs to be initialized by caller
 * - br_ip_list might contain duplicates in the end
 *   (needs to be taken care of by caller)
 * - br_ip_list needs to be freed by caller
 */
int br_multicast_list_adjacent(struct net_device *dev,
			       struct list_head *br_ip_list)
{
	struct net_bridge *br;
	struct net_bridge_port *port;
	struct net_bridge_port_group *group;
	struct br_ip_list *entry;
	int count = 0;

	rcu_read_lock();
	if (!br_ip_list || !netif_is_bridge_port(dev))
		goto unlock;

	port = br_port_get_rcu(dev);
	if (!port || !port->br)
		goto unlock;

	br = port->br;

	list_for_each_entry_rcu(port, &br->port_list, list) {
		if (!port->dev || port->dev == dev)
			continue;

		hlist_for_each_entry_rcu(group, &port->mglist, mglist) {
			entry = kmalloc(sizeof(*entry), GFP_ATOMIC);
			if (!entry)
				goto unlock;

			entry->addr = group->key.addr;
			list_add(&entry->list, br_ip_list);
			count++;
		}
	}

unlock:
	rcu_read_unlock();
	return count;
}
EXPORT_SYMBOL_GPL(br_multicast_list_adjacent);

/**
 * br_multicast_has_querier_anywhere - Checks for a querier on a bridge
 * @dev: The bridge port providing the bridge on which to check for a querier
 * @proto: The protocol family to check for: IGMP -> ETH_P_IP, MLD -> ETH_P_IPV6
 *
 * Checks whether the given interface has a bridge on top and if so returns
 * true if a valid querier exists anywhere on the bridged link layer.
 * Otherwise returns false.
 */
bool br_multicast_has_querier_anywhere(struct net_device *dev, int proto)
{
	struct net_bridge *br;
	struct net_bridge_port *port;
	struct ethhdr eth;
	bool ret = false;

	rcu_read_lock();
	if (!netif_is_bridge_port(dev))
		goto unlock;

	port = br_port_get_rcu(dev);
	if (!port || !port->br)
		goto unlock;

	br = port->br;

	memset(&eth, 0, sizeof(eth));
	eth.h_proto = htons(proto);

	ret = br_multicast_querier_exists(&br->multicast_ctx, &eth, NULL);

unlock:
	rcu_read_unlock();
	return ret;
}
EXPORT_SYMBOL_GPL(br_multicast_has_querier_anywhere);

/**
 * br_multicast_has_querier_adjacent - Checks for a querier behind a bridge port
 * @dev: The bridge port adjacent to which to check for a querier
 * @proto: The protocol family to check for: IGMP -> ETH_P_IP, MLD -> ETH_P_IPV6
 *
 * Checks whether the given interface has a bridge on top and if so returns
 * true if a selected querier is behind one of the other ports of this
 * bridge. Otherwise returns false.
 */
bool br_multicast_has_querier_adjacent(struct net_device *dev, int proto)
{
	struct net_bridge_mcast *brmctx;
	struct net_bridge *br;
	struct net_bridge_port *port;
	bool ret = false;
	int port_ifidx;

	rcu_read_lock();
	if (!netif_is_bridge_port(dev))
		goto unlock;

	port = br_port_get_rcu(dev);
	if (!port || !port->br)
		goto unlock;

	br = port->br;
	brmctx = &br->multicast_ctx;

	switch (proto) {
	case ETH_P_IP:
		port_ifidx = brmctx->ip4_querier.port_ifidx;
		if (!timer_pending(&brmctx->ip4_other_query.timer) ||
		    port_ifidx == port->dev->ifindex)
			goto unlock;
		break;
#if IS_ENABLED(CONFIG_IPV6)
	case ETH_P_IPV6:
		port_ifidx = brmctx->ip6_querier.port_ifidx;
		if (!timer_pending(&brmctx->ip6_other_query.timer) ||
		    port_ifidx == port->dev->ifindex)
			goto unlock;
		break;
#endif
	default:
		goto unlock;
	}

	ret = true;
unlock:
	rcu_read_unlock();
	return ret;
}
EXPORT_SYMBOL_GPL(br_multicast_has_querier_adjacent);

/**
 * br_multicast_has_router_adjacent - Checks for a router behind a bridge port
 * @dev: The bridge port adjacent to which to check for a multicast router
 * @proto: The protocol family to check for: IGMP -> ETH_P_IP, MLD -> ETH_P_IPV6
 *
 * Checks whether the given interface has a bridge on top and if so returns
 * true if a multicast router is behind one of the other ports of this
 * bridge. Otherwise returns false.
 */
bool br_multicast_has_router_adjacent(struct net_device *dev, int proto)
{
	struct net_bridge_mcast_port *pmctx;
	struct net_bridge_mcast *brmctx;
	struct net_bridge_port *port;
	bool ret = false;

	rcu_read_lock();
	port = br_port_get_check_rcu(dev);
	if (!port)
		goto unlock;

	brmctx = &port->br->multicast_ctx;
	switch (proto) {
	case ETH_P_IP:
		hlist_for_each_entry_rcu(pmctx, &brmctx->ip4_mc_router_list,
					 ip4_rlist) {
			if (pmctx->port == port)
				continue;

			ret = true;
			goto unlock;
		}
		break;
#if IS_ENABLED(CONFIG_IPV6)
	case ETH_P_IPV6:
		hlist_for_each_entry_rcu(pmctx, &brmctx->ip6_mc_router_list,
					 ip6_rlist) {
			if (pmctx->port == port)
				continue;

			ret = true;
			goto unlock;
		}
		break;
#endif
	default:
		/* when compiled without IPv6 support, be conservative and
		 * always assume presence of an IPv6 multicast router
		 */
		ret = true;
	}

unlock:
	rcu_read_unlock();
	return ret;
}
EXPORT_SYMBOL_GPL(br_multicast_has_router_adjacent);

static void br_mcast_stats_add(struct bridge_mcast_stats __percpu *stats,
			       const struct sk_buff *skb, u8 type, u8 dir)
{
	struct bridge_mcast_stats *pstats = this_cpu_ptr(stats);
	__be16 proto = skb->protocol;
	unsigned int t_len;

	u64_stats_update_begin(&pstats->syncp);
	switch (proto) {
	case htons(ETH_P_IP):
		t_len = ntohs(ip_hdr(skb)->tot_len) - ip_hdrlen(skb);
		switch (type) {
		case IGMP_HOST_MEMBERSHIP_REPORT:
			pstats->mstats.igmp_v1reports[dir]++;
			break;
		case IGMPV2_HOST_MEMBERSHIP_REPORT:
			pstats->mstats.igmp_v2reports[dir]++;
			break;
		case IGMPV3_HOST_MEMBERSHIP_REPORT:
			pstats->mstats.igmp_v3reports[dir]++;
			break;
		case IGMP_HOST_MEMBERSHIP_QUERY:
			if (t_len != sizeof(struct igmphdr)) {
				pstats->mstats.igmp_v3queries[dir]++;
			} else {
				unsigned int offset = skb_transport_offset(skb);
				struct igmphdr *ih, _ihdr;

				ih = skb_header_pointer(skb, offset,
							sizeof(_ihdr), &_ihdr);
				if (!ih)
					break;
				if (!ih->code)
					pstats->mstats.igmp_v1queries[dir]++;
				else
					pstats->mstats.igmp_v2queries[dir]++;
			}
			break;
		case IGMP_HOST_LEAVE_MESSAGE:
			pstats->mstats.igmp_leaves[dir]++;
			break;
		}
		break;
#if IS_ENABLED(CONFIG_IPV6)
	case htons(ETH_P_IPV6):
		t_len = ntohs(ipv6_hdr(skb)->payload_len) +
			sizeof(struct ipv6hdr);
		t_len -= skb_network_header_len(skb);
		switch (type) {
		case ICMPV6_MGM_REPORT:
			pstats->mstats.mld_v1reports[dir]++;
			break;
		case ICMPV6_MLD2_REPORT:
			pstats->mstats.mld_v2reports[dir]++;
			break;
		case ICMPV6_MGM_QUERY:
			if (t_len != sizeof(struct mld_msg))
				pstats->mstats.mld_v2queries[dir]++;
			else
				pstats->mstats.mld_v1queries[dir]++;
			break;
		case ICMPV6_MGM_REDUCTION:
			pstats->mstats.mld_leaves[dir]++;
			break;
		}
		break;
#endif /* CONFIG_IPV6 */
	}
	u64_stats_update_end(&pstats->syncp);
}

void br_multicast_count(struct net_bridge *br,
			const struct net_bridge_port *p,
			const struct sk_buff *skb, u8 type, u8 dir)
{
	struct bridge_mcast_stats __percpu *stats;

	/* if multicast_disabled is true then igmp type can't be set */
	if (!type || !br_opt_get(br, BROPT_MULTICAST_STATS_ENABLED))
		return;

	if (p)
		stats = p->mcast_stats;
	else
		stats = br->mcast_stats;
	if (WARN_ON(!stats))
		return;

	br_mcast_stats_add(stats, skb, type, dir);
}

int br_multicast_init_stats(struct net_bridge *br)
{
	br->mcast_stats = netdev_alloc_pcpu_stats(struct bridge_mcast_stats);
	if (!br->mcast_stats)
		return -ENOMEM;

	return 0;
}

void br_multicast_uninit_stats(struct net_bridge *br)
{
	free_percpu(br->mcast_stats);
}

/* noinline for https://llvm.org/pr45802#c9 */
static noinline_for_stack void mcast_stats_add_dir(u64 *dst, u64 *src)
{
	dst[BR_MCAST_DIR_RX] += src[BR_MCAST_DIR_RX];
	dst[BR_MCAST_DIR_TX] += src[BR_MCAST_DIR_TX];
}

void br_multicast_get_stats(const struct net_bridge *br,
			    const struct net_bridge_port *p,
			    struct br_mcast_stats *dest)
{
	struct bridge_mcast_stats __percpu *stats;
	struct br_mcast_stats tdst;
	int i;

	memset(dest, 0, sizeof(*dest));
	if (p)
		stats = p->mcast_stats;
	else
		stats = br->mcast_stats;
	if (WARN_ON(!stats))
		return;

	memset(&tdst, 0, sizeof(tdst));
	for_each_possible_cpu(i) {
		struct bridge_mcast_stats *cpu_stats = per_cpu_ptr(stats, i);
		struct br_mcast_stats temp;
		unsigned int start;

		do {
			start = u64_stats_fetch_begin(&cpu_stats->syncp);
			memcpy(&temp, &cpu_stats->mstats, sizeof(temp));
		} while (u64_stats_fetch_retry(&cpu_stats->syncp, start));

		mcast_stats_add_dir(tdst.igmp_v1queries, temp.igmp_v1queries);
		mcast_stats_add_dir(tdst.igmp_v2queries, temp.igmp_v2queries);
		mcast_stats_add_dir(tdst.igmp_v3queries, temp.igmp_v3queries);
		mcast_stats_add_dir(tdst.igmp_leaves, temp.igmp_leaves);
		mcast_stats_add_dir(tdst.igmp_v1reports, temp.igmp_v1reports);
		mcast_stats_add_dir(tdst.igmp_v2reports, temp.igmp_v2reports);
		mcast_stats_add_dir(tdst.igmp_v3reports, temp.igmp_v3reports);
		tdst.igmp_parse_errors += temp.igmp_parse_errors;

		mcast_stats_add_dir(tdst.mld_v1queries, temp.mld_v1queries);
		mcast_stats_add_dir(tdst.mld_v2queries, temp.mld_v2queries);
		mcast_stats_add_dir(tdst.mld_leaves, temp.mld_leaves);
		mcast_stats_add_dir(tdst.mld_v1reports, temp.mld_v1reports);
		mcast_stats_add_dir(tdst.mld_v2reports, temp.mld_v2reports);
		tdst.mld_parse_errors += temp.mld_parse_errors;
	}
	memcpy(dest, &tdst, sizeof(*dest));
}

int br_mdb_hash_init(struct net_bridge *br)
{
	int err;

	err = rhashtable_init(&br->sg_port_tbl, &br_sg_port_rht_params);
	if (err)
		return err;

	err = rhashtable_init(&br->mdb_hash_tbl, &br_mdb_rht_params);
	if (err) {
		rhashtable_destroy(&br->sg_port_tbl);
		return err;
	}

	return 0;
}

void br_mdb_hash_fini(struct net_bridge *br)
{
	rhashtable_destroy(&br->sg_port_tbl);
	rhashtable_destroy(&br->mdb_hash_tbl);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
