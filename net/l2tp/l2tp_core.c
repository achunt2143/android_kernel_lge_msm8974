<<<<<<< HEAD
/*
 * L2TP core.
=======
// SPDX-License-Identifier: GPL-2.0-only
/* L2TP core.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Copyright (c) 2008,2009,2010 Katalix Systems Ltd
 *
 * This file contains some code of the original L2TPv2 pppol2tp
 * driver, which has the following copyright:
 *
 * Authors:	Martijn van Oosterhout <kleptog@svana.org>
 *		James Chapman (jchapman@katalix.com)
 * Contributors:
 *		Michal Ostrowski <mostrows@speakeasy.net>
 *		Arnaldo Carvalho de Melo <acme@xconectiva.com.br>
 *		David S. Miller (davem@redhat.com)
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

=======
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/module.h>
#include <linux/string.h>
#include <linux/list.h>
#include <linux/rculist.h>
#include <linux/uaccess.h>

#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/jiffies.h>

#include <linux/netdevice.h>
#include <linux/net.h>
#include <linux/inetdevice.h>
#include <linux/skbuff.h>
#include <linux/init.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/l2tp.h>
#include <linux/hash.h>
#include <linux/sort.h>
#include <linux/file.h>
#include <linux/nsproxy.h>
#include <net/net_namespace.h>
#include <net/netns/generic.h>
#include <net/dst.h>
#include <net/ip.h>
#include <net/udp.h>
<<<<<<< HEAD
#include <net/inet_common.h>
#include <net/xfrm.h>
#include <net/protocol.h>
=======
#include <net/udp_tunnel.h>
#include <net/inet_common.h>
#include <net/xfrm.h>
#include <net/protocol.h>
#include <net/inet6_connection_sock.h>
#include <net/inet_ecn.h>
#include <net/ip6_route.h>
#include <net/ip6_checksum.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <asm/byteorder.h>
#include <linux/atomic.h>

#include "l2tp_core.h"
<<<<<<< HEAD
=======
#include "trace.h"

#define CREATE_TRACE_POINTS
#include "trace.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define L2TP_DRV_VERSION	"V2.0"

/* L2TP header constants */
#define L2TP_HDRFLAG_T	   0x8000
#define L2TP_HDRFLAG_L	   0x4000
#define L2TP_HDRFLAG_S	   0x0800
#define L2TP_HDRFLAG_O	   0x0200
#define L2TP_HDRFLAG_P	   0x0100

#define L2TP_HDR_VER_MASK  0x000F
#define L2TP_HDR_VER_2	   0x0002
#define L2TP_HDR_VER_3	   0x0003

/* L2TPv3 default L2-specific sublayer */
#define L2TP_SLFLAG_S	   0x40000000
#define L2TP_SL_SEQ_MASK   0x00ffffff

<<<<<<< HEAD
#define L2TP_HDR_SIZE_SEQ		10
#define L2TP_HDR_SIZE_NOSEQ		6
=======
#define L2TP_HDR_SIZE_MAX		14
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Default trace flags */
#define L2TP_DEFAULT_DEBUG_FLAGS	0

<<<<<<< HEAD
#define PRINTK(_mask, _type, _lvl, _fmt, args...)			\
	do {								\
		if ((_mask) & (_type))					\
			printk(_lvl "L2TP: " _fmt, ##args);		\
	} while (0)

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Private data stored for received packets in the skb.
 */
struct l2tp_skb_cb {
	u32			ns;
	u16			has_seq;
	u16			length;
	unsigned long		expires;
};

<<<<<<< HEAD
#define L2TP_SKB_CB(skb)	((struct l2tp_skb_cb *) &skb->cb[sizeof(struct inet_skb_parm)])

static atomic_t l2tp_tunnel_count;
static atomic_t l2tp_session_count;
=======
#define L2TP_SKB_CB(skb)	((struct l2tp_skb_cb *)&(skb)->cb[sizeof(struct inet_skb_parm)])

static struct workqueue_struct *l2tp_wq;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* per-net private data for this module */
static unsigned int l2tp_net_id;
struct l2tp_net {
<<<<<<< HEAD
	struct list_head l2tp_tunnel_list;
	spinlock_t l2tp_tunnel_list_lock;
	struct hlist_head l2tp_session_hlist[L2TP_HASH_SIZE_2];
	spinlock_t l2tp_session_hlist_lock;
};

static void l2tp_session_set_header_len(struct l2tp_session *session, int version);
static void l2tp_tunnel_free(struct l2tp_tunnel *tunnel);
static void l2tp_tunnel_closeall(struct l2tp_tunnel *tunnel);

static inline struct l2tp_net *l2tp_pernet(struct net *net)
{
	BUG_ON(!net);

	return net_generic(net, l2tp_net_id);
}


/* Tunnel reference counts. Incremented per session that is added to
 * the tunnel.
 */
static inline void l2tp_tunnel_inc_refcount_1(struct l2tp_tunnel *tunnel)
{
	atomic_inc(&tunnel->ref_count);
}

static inline void l2tp_tunnel_dec_refcount_1(struct l2tp_tunnel *tunnel)
{
	if (atomic_dec_and_test(&tunnel->ref_count))
		l2tp_tunnel_free(tunnel);
}
#ifdef L2TP_REFCNT_DEBUG
#define l2tp_tunnel_inc_refcount(_t) do { \
		printk(KERN_DEBUG "l2tp_tunnel_inc_refcount: %s:%d %s: cnt=%d\n", __func__, __LINE__, (_t)->name, atomic_read(&_t->ref_count)); \
		l2tp_tunnel_inc_refcount_1(_t);				\
	} while (0)
#define l2tp_tunnel_dec_refcount(_t) do { \
		printk(KERN_DEBUG "l2tp_tunnel_dec_refcount: %s:%d %s: cnt=%d\n", __func__, __LINE__, (_t)->name, atomic_read(&_t->ref_count)); \
		l2tp_tunnel_dec_refcount_1(_t);				\
	} while (0)
#else
#define l2tp_tunnel_inc_refcount(t) l2tp_tunnel_inc_refcount_1(t)
#define l2tp_tunnel_dec_refcount(t) l2tp_tunnel_dec_refcount_1(t)
#endif

=======
	/* Lock for write access to l2tp_tunnel_idr */
	spinlock_t l2tp_tunnel_idr_lock;
	struct idr l2tp_tunnel_idr;
	struct hlist_head l2tp_session_hlist[L2TP_HASH_SIZE_2];
	/* Lock for write access to l2tp_session_hlist */
	spinlock_t l2tp_session_hlist_lock;
};

#if IS_ENABLED(CONFIG_IPV6)
static bool l2tp_sk_is_v6(struct sock *sk)
{
	return sk->sk_family == PF_INET6 &&
	       !ipv6_addr_v4mapped(&sk->sk_v6_daddr);
}
#endif

static inline struct l2tp_net *l2tp_pernet(const struct net *net)
{
	return net_generic(net, l2tp_net_id);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Session hash global list for L2TPv3.
 * The session_id SHOULD be random according to RFC3931, but several
 * L2TP implementations use incrementing session_ids.  So we do a real
 * hash on the session_id, rather than a simple bitmask.
 */
static inline struct hlist_head *
l2tp_session_id_hash_2(struct l2tp_net *pn, u32 session_id)
{
	return &pn->l2tp_session_hlist[hash_32(session_id, L2TP_HASH_BITS_2)];
<<<<<<< HEAD

}

/* Lookup a session by id in the global session list
 */
static struct l2tp_session *l2tp_session_find_2(struct net *net, u32 session_id)
{
	struct l2tp_net *pn = l2tp_pernet(net);
	struct hlist_head *session_list =
		l2tp_session_id_hash_2(pn, session_id);
	struct l2tp_session *session;
	struct hlist_node *walk;

	rcu_read_lock_bh();
	hlist_for_each_entry_rcu(session, walk, session_list, global_hlist) {
		if (session->session_id == session_id) {
			rcu_read_unlock_bh();
			return session;
		}
	}
	rcu_read_unlock_bh();

	return NULL;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Session hash list.
 * The session_id SHOULD be random according to RFC2661, but several
 * L2TP implementations (Cisco and Microsoft) use incrementing
 * session_ids.  So we do a real hash on the session_id, rather than a
 * simple bitmask.
 */
static inline struct hlist_head *
l2tp_session_id_hash(struct l2tp_tunnel *tunnel, u32 session_id)
{
	return &tunnel->session_hlist[hash_32(session_id, L2TP_HASH_BITS)];
}

<<<<<<< HEAD
/* Lookup a session by id
 */
struct l2tp_session *l2tp_session_find(struct net *net, struct l2tp_tunnel *tunnel, u32 session_id)
{
	struct hlist_head *session_list;
	struct l2tp_session *session;
	struct hlist_node *walk;

	/* In L2TPv3, session_ids are unique over all tunnels and we
	 * sometimes need to look them up before we know the
	 * tunnel.
	 */
	if (tunnel == NULL)
		return l2tp_session_find_2(net, session_id);

	session_list = l2tp_session_id_hash(tunnel, session_id);
	read_lock_bh(&tunnel->hlist_lock);
	hlist_for_each_entry(session, walk, session_list, hlist) {
		if (session->session_id == session_id) {
			read_unlock_bh(&tunnel->hlist_lock);
			return session;
		}
	}
	read_unlock_bh(&tunnel->hlist_lock);

	return NULL;
}
EXPORT_SYMBOL_GPL(l2tp_session_find);

/* Like l2tp_session_find() but takes a reference on the returned session.
 * Optionally calls session->ref() too if do_ref is true.
 */
struct l2tp_session *l2tp_session_get(struct net *net,
				      struct l2tp_tunnel *tunnel,
				      u32 session_id, bool do_ref)
{
	struct hlist_head *session_list;
	struct l2tp_session *session;
	struct hlist_node *walk;

	if (!tunnel) {
		struct l2tp_net *pn = l2tp_pernet(net);

		session_list = l2tp_session_id_hash_2(pn, session_id);

		rcu_read_lock_bh();
		hlist_for_each_entry_rcu(session, walk, session_list, global_hlist) {
			if (session->session_id == session_id) {
				l2tp_session_inc_refcount(session);
				if (do_ref && session->ref)
					session->ref(session);
				rcu_read_unlock_bh();

				return session;
			}
		}
		rcu_read_unlock_bh();

		return NULL;
	}

	session_list = l2tp_session_id_hash(tunnel, session_id);
	read_lock_bh(&tunnel->hlist_lock);
	hlist_for_each_entry(session, walk, session_list, hlist) {
		if (session->session_id == session_id) {
			l2tp_session_inc_refcount(session);
			if (do_ref && session->ref)
				session->ref(session);
			read_unlock_bh(&tunnel->hlist_lock);

			return session;
		}
	}
	read_unlock_bh(&tunnel->hlist_lock);
=======
static void l2tp_tunnel_free(struct l2tp_tunnel *tunnel)
{
	trace_free_tunnel(tunnel);
	sock_put(tunnel->sock);
	/* the tunnel is freed in the socket destructor */
}

static void l2tp_session_free(struct l2tp_session *session)
{
	trace_free_session(session);
	if (session->tunnel)
		l2tp_tunnel_dec_refcount(session->tunnel);
	kfree(session);
}

struct l2tp_tunnel *l2tp_sk_to_tunnel(struct sock *sk)
{
	struct l2tp_tunnel *tunnel = sk->sk_user_data;

	if (tunnel)
		if (WARN_ON(tunnel->magic != L2TP_TUNNEL_MAGIC))
			return NULL;

	return tunnel;
}
EXPORT_SYMBOL_GPL(l2tp_sk_to_tunnel);

void l2tp_tunnel_inc_refcount(struct l2tp_tunnel *tunnel)
{
	refcount_inc(&tunnel->ref_count);
}
EXPORT_SYMBOL_GPL(l2tp_tunnel_inc_refcount);

void l2tp_tunnel_dec_refcount(struct l2tp_tunnel *tunnel)
{
	if (refcount_dec_and_test(&tunnel->ref_count))
		l2tp_tunnel_free(tunnel);
}
EXPORT_SYMBOL_GPL(l2tp_tunnel_dec_refcount);

void l2tp_session_inc_refcount(struct l2tp_session *session)
{
	refcount_inc(&session->ref_count);
}
EXPORT_SYMBOL_GPL(l2tp_session_inc_refcount);

void l2tp_session_dec_refcount(struct l2tp_session *session)
{
	if (refcount_dec_and_test(&session->ref_count))
		l2tp_session_free(session);
}
EXPORT_SYMBOL_GPL(l2tp_session_dec_refcount);

/* Lookup a tunnel. A new reference is held on the returned tunnel. */
struct l2tp_tunnel *l2tp_tunnel_get(const struct net *net, u32 tunnel_id)
{
	const struct l2tp_net *pn = l2tp_pernet(net);
	struct l2tp_tunnel *tunnel;

	rcu_read_lock_bh();
	tunnel = idr_find(&pn->l2tp_tunnel_idr, tunnel_id);
	if (tunnel && refcount_inc_not_zero(&tunnel->ref_count)) {
		rcu_read_unlock_bh();
		return tunnel;
	}
	rcu_read_unlock_bh();

	return NULL;
}
EXPORT_SYMBOL_GPL(l2tp_tunnel_get);

struct l2tp_tunnel *l2tp_tunnel_get_nth(const struct net *net, int nth)
{
	struct l2tp_net *pn = l2tp_pernet(net);
	unsigned long tunnel_id, tmp;
	struct l2tp_tunnel *tunnel;
	int count = 0;

	rcu_read_lock_bh();
	idr_for_each_entry_ul(&pn->l2tp_tunnel_idr, tunnel, tmp, tunnel_id) {
		if (tunnel && ++count > nth &&
		    refcount_inc_not_zero(&tunnel->ref_count)) {
			rcu_read_unlock_bh();
			return tunnel;
		}
	}
	rcu_read_unlock_bh();

	return NULL;
}
EXPORT_SYMBOL_GPL(l2tp_tunnel_get_nth);

struct l2tp_session *l2tp_tunnel_get_session(struct l2tp_tunnel *tunnel,
					     u32 session_id)
{
	struct hlist_head *session_list;
	struct l2tp_session *session;

	session_list = l2tp_session_id_hash(tunnel, session_id);

	rcu_read_lock_bh();
	hlist_for_each_entry_rcu(session, session_list, hlist)
		if (session->session_id == session_id) {
			l2tp_session_inc_refcount(session);
			rcu_read_unlock_bh();

			return session;
		}
	rcu_read_unlock_bh();

	return NULL;
}
EXPORT_SYMBOL_GPL(l2tp_tunnel_get_session);

struct l2tp_session *l2tp_session_get(const struct net *net, u32 session_id)
{
	struct hlist_head *session_list;
	struct l2tp_session *session;

	session_list = l2tp_session_id_hash_2(l2tp_pernet(net), session_id);

	rcu_read_lock_bh();
	hlist_for_each_entry_rcu(session, session_list, global_hlist)
		if (session->session_id == session_id) {
			l2tp_session_inc_refcount(session);
			rcu_read_unlock_bh();

			return session;
		}
	rcu_read_unlock_bh();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return NULL;
}
EXPORT_SYMBOL_GPL(l2tp_session_get);

<<<<<<< HEAD
struct l2tp_session *l2tp_session_find_nth(struct l2tp_tunnel *tunnel, int nth)
{
	int hash;
	struct hlist_node *walk;
	struct l2tp_session *session;
	int count = 0;

	read_lock_bh(&tunnel->hlist_lock);
	for (hash = 0; hash < L2TP_HASH_SIZE; hash++) {
		hlist_for_each_entry(session, walk, &tunnel->session_hlist[hash], hlist) {
			if (++count > nth) {
				read_unlock_bh(&tunnel->hlist_lock);
				return session;
			}
		}
	}

	read_unlock_bh(&tunnel->hlist_lock);

	return NULL;
}
EXPORT_SYMBOL_GPL(l2tp_session_find_nth);

/* Lookup a session by interface name.
 * This is very inefficient but is only used by management interfaces.
 */
struct l2tp_session *l2tp_session_find_by_ifname(struct net *net, char *ifname)
{
	struct l2tp_net *pn = l2tp_pernet(net);
	int hash;
	struct hlist_node *walk;
	struct l2tp_session *session;

	rcu_read_lock_bh();
	for (hash = 0; hash < L2TP_HASH_SIZE_2; hash++) {
		hlist_for_each_entry_rcu(session, walk, &pn->l2tp_session_hlist[hash], global_hlist) {
			if (!strcmp(session->ifname, ifname)) {
=======
struct l2tp_session *l2tp_session_get_nth(struct l2tp_tunnel *tunnel, int nth)
{
	int hash;
	struct l2tp_session *session;
	int count = 0;

	rcu_read_lock_bh();
	for (hash = 0; hash < L2TP_HASH_SIZE; hash++) {
		hlist_for_each_entry_rcu(session, &tunnel->session_hlist[hash], hlist) {
			if (++count > nth) {
				l2tp_session_inc_refcount(session);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				rcu_read_unlock_bh();
				return session;
			}
		}
	}

	rcu_read_unlock_bh();

	return NULL;
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(l2tp_session_find_by_ifname);

static int l2tp_session_add_to_tunnel(struct l2tp_tunnel *tunnel,
				      struct l2tp_session *session)
=======
EXPORT_SYMBOL_GPL(l2tp_session_get_nth);

/* Lookup a session by interface name.
 * This is very inefficient but is only used by management interfaces.
 */
struct l2tp_session *l2tp_session_get_by_ifname(const struct net *net,
						const char *ifname)
{
	struct l2tp_net *pn = l2tp_pernet(net);
	int hash;
	struct l2tp_session *session;

	rcu_read_lock_bh();
	for (hash = 0; hash < L2TP_HASH_SIZE_2; hash++) {
		hlist_for_each_entry_rcu(session, &pn->l2tp_session_hlist[hash], global_hlist) {
			if (!strcmp(session->ifname, ifname)) {
				l2tp_session_inc_refcount(session);
				rcu_read_unlock_bh();

				return session;
			}
		}
	}

	rcu_read_unlock_bh();

	return NULL;
}
EXPORT_SYMBOL_GPL(l2tp_session_get_by_ifname);

int l2tp_session_register(struct l2tp_session *session,
			  struct l2tp_tunnel *tunnel)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct l2tp_session *session_walk;
	struct hlist_head *g_head;
	struct hlist_head *head;
	struct l2tp_net *pn;
<<<<<<< HEAD
	struct hlist_node *walk;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int err;

	head = l2tp_session_id_hash(tunnel, session->session_id);

<<<<<<< HEAD
	write_lock_bh(&tunnel->hlist_lock);
=======
	spin_lock_bh(&tunnel->hlist_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!tunnel->acpt_newsess) {
		err = -ENODEV;
		goto err_tlock;
	}

<<<<<<< HEAD
	hlist_for_each_entry(session_walk, walk, head, hlist)
=======
	hlist_for_each_entry(session_walk, head, hlist)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (session_walk->session_id == session->session_id) {
			err = -EEXIST;
			goto err_tlock;
		}

	if (tunnel->version == L2TP_HDR_VER_3) {
		pn = l2tp_pernet(tunnel->l2tp_net);
<<<<<<< HEAD
		g_head = l2tp_session_id_hash_2(l2tp_pernet(tunnel->l2tp_net),
						session->session_id);

		spin_lock_bh(&pn->l2tp_session_hlist_lock);

		hlist_for_each_entry(session_walk, walk, g_head, global_hlist)
			if (session_walk->session_id == session->session_id) {
=======
		g_head = l2tp_session_id_hash_2(pn, session->session_id);

		spin_lock_bh(&pn->l2tp_session_hlist_lock);

		/* IP encap expects session IDs to be globally unique, while
		 * UDP encap doesn't.
		 */
		hlist_for_each_entry(session_walk, g_head, global_hlist)
			if (session_walk->session_id == session->session_id &&
			    (session_walk->tunnel->encap == L2TP_ENCAPTYPE_IP ||
			     tunnel->encap == L2TP_ENCAPTYPE_IP)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				err = -EEXIST;
				goto err_tlock_pnlock;
			}

		l2tp_tunnel_inc_refcount(tunnel);
<<<<<<< HEAD
		sock_hold(tunnel->sock);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		hlist_add_head_rcu(&session->global_hlist, g_head);

		spin_unlock_bh(&pn->l2tp_session_hlist_lock);
	} else {
		l2tp_tunnel_inc_refcount(tunnel);
<<<<<<< HEAD
		sock_hold(tunnel->sock);
	}

	hlist_add_head(&session->hlist, head);
	write_unlock_bh(&tunnel->hlist_lock);
=======
	}

	hlist_add_head_rcu(&session->hlist, head);
	spin_unlock_bh(&tunnel->hlist_lock);

	trace_register_session(session);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;

err_tlock_pnlock:
	spin_unlock_bh(&pn->l2tp_session_hlist_lock);
err_tlock:
<<<<<<< HEAD
	write_unlock_bh(&tunnel->hlist_lock);

	return err;
}

/* Lookup a tunnel by id
 */
struct l2tp_tunnel *l2tp_tunnel_find(struct net *net, u32 tunnel_id)
{
	struct l2tp_tunnel *tunnel;
	struct l2tp_net *pn = l2tp_pernet(net);

	rcu_read_lock_bh();
	list_for_each_entry_rcu(tunnel, &pn->l2tp_tunnel_list, list) {
		if (tunnel->tunnel_id == tunnel_id) {
			rcu_read_unlock_bh();
			return tunnel;
		}
	}
	rcu_read_unlock_bh();

	return NULL;
}
EXPORT_SYMBOL_GPL(l2tp_tunnel_find);

struct l2tp_tunnel *l2tp_tunnel_find_nth(struct net *net, int nth)
{
	struct l2tp_net *pn = l2tp_pernet(net);
	struct l2tp_tunnel *tunnel;
	int count = 0;

	rcu_read_lock_bh();
	list_for_each_entry_rcu(tunnel, &pn->l2tp_tunnel_list, list) {
		if (++count > nth) {
			rcu_read_unlock_bh();
			return tunnel;
		}
	}

	rcu_read_unlock_bh();

	return NULL;
}
EXPORT_SYMBOL_GPL(l2tp_tunnel_find_nth);
=======
	spin_unlock_bh(&tunnel->hlist_lock);

	return err;
}
EXPORT_SYMBOL_GPL(l2tp_session_register);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*****************************************************************************
 * Receive data handling
 *****************************************************************************/

/* Queue a skb in order. We come here only if the skb has an L2TP sequence
 * number.
 */
static void l2tp_recv_queue_skb(struct l2tp_session *session, struct sk_buff *skb)
{
	struct sk_buff *skbp;
	struct sk_buff *tmp;
	u32 ns = L2TP_SKB_CB(skb)->ns;

	spin_lock_bh(&session->reorder_q.lock);
	skb_queue_walk_safe(&session->reorder_q, skbp, tmp) {
		if (L2TP_SKB_CB(skbp)->ns > ns) {
			__skb_queue_before(&session->reorder_q, skbp, skb);
<<<<<<< HEAD
			PRINTK(session->debug, L2TP_MSG_SEQ, KERN_DEBUG,
			       "%s: pkt %hu, inserted before %hu, reorder_q len=%d\n",
			       session->name, ns, L2TP_SKB_CB(skbp)->ns,
			       skb_queue_len(&session->reorder_q));
			session->stats.rx_oos_packets++;
=======
			atomic_long_inc(&session->stats.rx_oos_packets);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto out;
		}
	}

	__skb_queue_tail(&session->reorder_q, skb);

out:
	spin_unlock_bh(&session->reorder_q.lock);
}

/* Dequeue a single skb.
 */
static void l2tp_recv_dequeue_skb(struct l2tp_session *session, struct sk_buff *skb)
{
	struct l2tp_tunnel *tunnel = session->tunnel;
	int length = L2TP_SKB_CB(skb)->length;

	/* We're about to requeue the skb, so return resources
	 * to its current owner (a socket receive buffer).
	 */
	skb_orphan(skb);

<<<<<<< HEAD
	tunnel->stats.rx_packets++;
	tunnel->stats.rx_bytes += length;
	session->stats.rx_packets++;
	session->stats.rx_bytes += length;
=======
	atomic_long_inc(&tunnel->stats.rx_packets);
	atomic_long_add(length, &tunnel->stats.rx_bytes);
	atomic_long_inc(&session->stats.rx_packets);
	atomic_long_add(length, &session->stats.rx_bytes);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (L2TP_SKB_CB(skb)->has_seq) {
		/* Bump our Nr */
		session->nr++;
<<<<<<< HEAD
		if (tunnel->version == L2TP_HDR_VER_2)
			session->nr &= 0xffff;
		else
			session->nr &= 0xffffff;

		PRINTK(session->debug, L2TP_MSG_SEQ, KERN_DEBUG,
		       "%s: updated nr to %hu\n", session->name, session->nr);
	}

	/* call private receive handler */
	if (session->recv_skb != NULL)
		(*session->recv_skb)(session, skb, L2TP_SKB_CB(skb)->length);
	else
		kfree_skb(skb);

	if (session->deref)
		(*session->deref)(session);
=======
		session->nr &= session->nr_max;
		trace_session_seqnum_update(session);
	}

	/* call private receive handler */
	if (session->recv_skb)
		(*session->recv_skb)(session, skb, L2TP_SKB_CB(skb)->length);
	else
		kfree_skb(skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Dequeue skbs from the session's reorder_q, subject to packet order.
 * Skbs that have been in the queue for too long are simply discarded.
 */
static void l2tp_recv_dequeue(struct l2tp_session *session)
{
	struct sk_buff *skb;
	struct sk_buff *tmp;

	/* If the pkt at the head of the queue has the nr that we
	 * expect to send up next, dequeue it and any other
	 * in-sequence packets behind it.
	 */
start:
	spin_lock_bh(&session->reorder_q.lock);
	skb_queue_walk_safe(&session->reorder_q, skb, tmp) {
<<<<<<< HEAD
		if (time_after(jiffies, L2TP_SKB_CB(skb)->expires)) {
			session->stats.rx_seq_discards++;
			session->stats.rx_errors++;
			PRINTK(session->debug, L2TP_MSG_SEQ, KERN_DEBUG,
			       "%s: oos pkt %u len %d discarded (too old), "
			       "waiting for %u, reorder_q_len=%d\n",
			       session->name, L2TP_SKB_CB(skb)->ns,
			       L2TP_SKB_CB(skb)->length, session->nr,
			       skb_queue_len(&session->reorder_q));
			__skb_unlink(skb, &session->reorder_q);
			kfree_skb(skb);
			if (session->deref)
				(*session->deref)(session);
			continue;
		}

		if (L2TP_SKB_CB(skb)->has_seq) {
			if (L2TP_SKB_CB(skb)->ns != session->nr) {
				PRINTK(session->debug, L2TP_MSG_SEQ, KERN_DEBUG,
				       "%s: holding oos pkt %u len %d, "
				       "waiting for %u, reorder_q_len=%d\n",
				       session->name, L2TP_SKB_CB(skb)->ns,
				       L2TP_SKB_CB(skb)->length, session->nr,
				       skb_queue_len(&session->reorder_q));
				goto out;
			}
=======
		struct l2tp_skb_cb *cb = L2TP_SKB_CB(skb);

		/* If the packet has been pending on the queue for too long, discard it */
		if (time_after(jiffies, cb->expires)) {
			atomic_long_inc(&session->stats.rx_seq_discards);
			atomic_long_inc(&session->stats.rx_errors);
			trace_session_pkt_expired(session, cb->ns);
			session->reorder_skip = 1;
			__skb_unlink(skb, &session->reorder_q);
			kfree_skb(skb);
			continue;
		}

		if (cb->has_seq) {
			if (session->reorder_skip) {
				session->reorder_skip = 0;
				session->nr = cb->ns;
				trace_session_seqnum_reset(session);
			}
			if (cb->ns != session->nr)
				goto out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		__skb_unlink(skb, &session->reorder_q);

		/* Process the skb. We release the queue lock while we
		 * do so to let other contexts process the queue.
		 */
		spin_unlock_bh(&session->reorder_q.lock);
		l2tp_recv_dequeue_skb(session, skb);
		goto start;
	}

out:
	spin_unlock_bh(&session->reorder_q.lock);
}

<<<<<<< HEAD
static inline int l2tp_verify_udp_checksum(struct sock *sk,
					   struct sk_buff *skb)
{
	struct udphdr *uh = udp_hdr(skb);
	u16 ulen = ntohs(uh->len);
	struct inet_sock *inet;
	__wsum psum;

	if (sk->sk_no_check || skb_csum_unnecessary(skb) || !uh->check)
		return 0;

	inet = inet_sk(sk);
	psum = csum_tcpudp_nofold(inet->inet_saddr, inet->inet_daddr, ulen,
				  IPPROTO_UDP, 0);

	if ((skb->ip_summed == CHECKSUM_COMPLETE) &&
	    !csum_fold(csum_add(psum, skb->csum)))
		return 0;

	skb->csum = psum;

	return __skb_checksum_complete(skb);
=======
static int l2tp_seq_check_rx_window(struct l2tp_session *session, u32 nr)
{
	u32 nws;

	if (nr >= session->nr)
		nws = nr - session->nr;
	else
		nws = (session->nr_max + 1) - (session->nr - nr);

	return nws < session->nr_window_size;
}

/* If packet has sequence numbers, queue it if acceptable. Returns 0 if
 * acceptable, else non-zero.
 */
static int l2tp_recv_data_seq(struct l2tp_session *session, struct sk_buff *skb)
{
	struct l2tp_skb_cb *cb = L2TP_SKB_CB(skb);

	if (!l2tp_seq_check_rx_window(session, cb->ns)) {
		/* Packet sequence number is outside allowed window.
		 * Discard it.
		 */
		trace_session_pkt_outside_rx_window(session, cb->ns);
		goto discard;
	}

	if (session->reorder_timeout != 0) {
		/* Packet reordering enabled. Add skb to session's
		 * reorder queue, in order of ns.
		 */
		l2tp_recv_queue_skb(session, skb);
		goto out;
	}

	/* Packet reordering disabled. Discard out-of-sequence packets, while
	 * tracking the number if in-sequence packets after the first OOS packet
	 * is seen. After nr_oos_count_max in-sequence packets, reset the
	 * sequence number to re-enable packet reception.
	 */
	if (cb->ns == session->nr) {
		skb_queue_tail(&session->reorder_q, skb);
	} else {
		u32 nr_oos = cb->ns;
		u32 nr_next = (session->nr_oos + 1) & session->nr_max;

		if (nr_oos == nr_next)
			session->nr_oos_count++;
		else
			session->nr_oos_count = 0;

		session->nr_oos = nr_oos;
		if (session->nr_oos_count > session->nr_oos_count_max) {
			session->reorder_skip = 1;
		}
		if (!session->reorder_skip) {
			atomic_long_inc(&session->stats.rx_seq_discards);
			trace_session_pkt_oos(session, cb->ns);
			goto discard;
		}
		skb_queue_tail(&session->reorder_q, skb);
	}

out:
	return 0;

discard:
	return 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Do receive processing of L2TP data frames. We handle both L2TPv2
 * and L2TPv3 data frames here.
 *
 * L2TPv2 Data Message Header
 *
 *  0                   1                   2                   3
 *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |T|L|x|x|S|x|O|P|x|x|x|x|  Ver  |          Length (opt)         |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |           Tunnel ID           |           Session ID          |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |             Ns (opt)          |             Nr (opt)          |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |      Offset Size (opt)        |    Offset pad... (opt)
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * Data frames are marked by T=0. All other fields are the same as
 * those in L2TP control frames.
 *
 * L2TPv3 Data Message Header
 *
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |                      L2TP Session Header                      |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |                      L2-Specific Sublayer                     |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |                        Tunnel Payload                      ...
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * L2TPv3 Session Header Over IP
 *
 *  0                   1                   2                   3
 *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |                           Session ID                          |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |               Cookie (optional, maximum 64 bits)...
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *                                                                 |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * L2TPv3 L2-Specific Sublayer Format
 *
 *  0                   1                   2                   3
 *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |x|S|x|x|x|x|x|x|              Sequence Number                  |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
<<<<<<< HEAD
 * Cookie value, sublayer format and offset (pad) are negotiated with
 * the peer when the session is set up. Unlike L2TPv2, we do not need
 * to parse the packet header to determine if optional fields are
 * present.
=======
 * Cookie value and sublayer format are negotiated with the peer when
 * the session is set up. Unlike L2TPv2, we do not need to parse the
 * packet header to determine if optional fields are present.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Caller must already have parsed the frame and determined that it is
 * a data (not control) frame before coming here. Fields up to the
 * session-id have already been parsed and ptr points to the data
 * after the session-id.
<<<<<<< HEAD
 *
 * session->ref() must have been called prior to l2tp_recv_common().
 * session->deref() will be called automatically after skb is processed.
 */
void l2tp_recv_common(struct l2tp_session *session, struct sk_buff *skb,
		      unsigned char *ptr, unsigned char *optr, u16 hdrflags,
		      int length, int (*payload_hook)(struct sk_buff *skb))
{
	struct l2tp_tunnel *tunnel = session->tunnel;
	int offset;
	u32 ns, nr;
=======
 */
void l2tp_recv_common(struct l2tp_session *session, struct sk_buff *skb,
		      unsigned char *ptr, unsigned char *optr, u16 hdrflags,
		      int length)
{
	struct l2tp_tunnel *tunnel = session->tunnel;
	int offset;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Parse and check optional cookie */
	if (session->peer_cookie_len > 0) {
		if (memcmp(ptr, &session->peer_cookie[0], session->peer_cookie_len)) {
<<<<<<< HEAD
			PRINTK(tunnel->debug, L2TP_MSG_DATA, KERN_INFO,
			       "%s: cookie mismatch (%u/%u). Discarding.\n",
			       tunnel->name, tunnel->tunnel_id, session->session_id);
			session->stats.rx_cookie_discards++;
=======
			pr_debug_ratelimited("%s: cookie mismatch (%u/%u). Discarding.\n",
					     tunnel->name, tunnel->tunnel_id,
					     session->session_id);
			atomic_long_inc(&session->stats.rx_cookie_discards);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto discard;
		}
		ptr += session->peer_cookie_len;
	}

	/* Handle the optional sequence numbers. Sequence numbers are
	 * in different places for L2TPv2 and L2TPv3.
	 *
	 * If we are the LAC, enable/disable sequence numbers under
	 * the control of the LNS.  If no sequence numbers present but
	 * we were expecting them, discard frame.
	 */
<<<<<<< HEAD
	ns = nr = 0;
	L2TP_SKB_CB(skb)->has_seq = 0;
	if (tunnel->version == L2TP_HDR_VER_2) {
		if (hdrflags & L2TP_HDRFLAG_S) {
			ns = ntohs(*(__be16 *) ptr);
			ptr += 2;
			nr = ntohs(*(__be16 *) ptr);
			ptr += 2;

			/* Store L2TP info in the skb */
			L2TP_SKB_CB(skb)->ns = ns;
			L2TP_SKB_CB(skb)->has_seq = 1;

			PRINTK(session->debug, L2TP_MSG_SEQ, KERN_DEBUG,
			       "%s: recv data ns=%u, nr=%u, session nr=%u\n",
			       session->name, ns, nr, session->nr);
		}
	} else if (session->l2specific_type == L2TP_L2SPECTYPE_DEFAULT) {
		u32 l2h = ntohl(*(__be32 *) ptr);

		if (l2h & 0x40000000) {
			ns = l2h & 0x00ffffff;

			/* Store L2TP info in the skb */
			L2TP_SKB_CB(skb)->ns = ns;
			L2TP_SKB_CB(skb)->has_seq = 1;

			PRINTK(session->debug, L2TP_MSG_SEQ, KERN_DEBUG,
			       "%s: recv data ns=%u, session nr=%u\n",
			       session->name, ns, session->nr);
		}
	}

	/* Advance past L2-specific header, if present */
	ptr += session->l2specific_len;

	if (L2TP_SKB_CB(skb)->has_seq) {
		/* Received a packet with sequence numbers. If we're the LNS,
		 * check if we sre sending sequence numbers and if not,
		 * configure it so.
		 */
		if ((!session->lns_mode) && (!session->send_seq)) {
			PRINTK(session->debug, L2TP_MSG_SEQ, KERN_INFO,
			       "%s: requested to enable seq numbers by LNS\n",
			       session->name);
			session->send_seq = -1;
=======
	L2TP_SKB_CB(skb)->has_seq = 0;
	if (tunnel->version == L2TP_HDR_VER_2) {
		if (hdrflags & L2TP_HDRFLAG_S) {
			/* Store L2TP info in the skb */
			L2TP_SKB_CB(skb)->ns = ntohs(*(__be16 *)ptr);
			L2TP_SKB_CB(skb)->has_seq = 1;
			ptr += 2;
			/* Skip past nr in the header */
			ptr += 2;

		}
	} else if (session->l2specific_type == L2TP_L2SPECTYPE_DEFAULT) {
		u32 l2h = ntohl(*(__be32 *)ptr);

		if (l2h & 0x40000000) {
			/* Store L2TP info in the skb */
			L2TP_SKB_CB(skb)->ns = l2h & 0x00ffffff;
			L2TP_SKB_CB(skb)->has_seq = 1;
		}
		ptr += 4;
	}

	if (L2TP_SKB_CB(skb)->has_seq) {
		/* Received a packet with sequence numbers. If we're the LAC,
		 * check if we sre sending sequence numbers and if not,
		 * configure it so.
		 */
		if (!session->lns_mode && !session->send_seq) {
			trace_session_seqnum_lns_enable(session);
			session->send_seq = 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			l2tp_session_set_header_len(session, tunnel->version);
		}
	} else {
		/* No sequence numbers.
		 * If user has configured mandatory sequence numbers, discard.
		 */
		if (session->recv_seq) {
<<<<<<< HEAD
			PRINTK(session->debug, L2TP_MSG_SEQ, KERN_WARNING,
			       "%s: recv data has no seq numbers when required. "
			       "Discarding\n", session->name);
			session->stats.rx_seq_discards++;
=======
			pr_debug_ratelimited("%s: recv data has no seq numbers when required. Discarding.\n",
					     session->name);
			atomic_long_inc(&session->stats.rx_seq_discards);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto discard;
		}

		/* If we're the LAC and we're sending sequence numbers, the
		 * LNS has requested that we no longer send sequence numbers.
		 * If we're the LNS and we're sending sequence numbers, the
		 * LAC is broken. Discard the frame.
		 */
<<<<<<< HEAD
		if ((!session->lns_mode) && (session->send_seq)) {
			PRINTK(session->debug, L2TP_MSG_SEQ, KERN_INFO,
			       "%s: requested to disable seq numbers by LNS\n",
			       session->name);
			session->send_seq = 0;
			l2tp_session_set_header_len(session, tunnel->version);
		} else if (session->send_seq) {
			PRINTK(session->debug, L2TP_MSG_SEQ, KERN_WARNING,
			       "%s: recv data has no seq numbers when required. "
			       "Discarding\n", session->name);
			session->stats.rx_seq_discards++;
=======
		if (!session->lns_mode && session->send_seq) {
			trace_session_seqnum_lns_disable(session);
			session->send_seq = 0;
			l2tp_session_set_header_len(session, tunnel->version);
		} else if (session->send_seq) {
			pr_debug_ratelimited("%s: recv data has no seq numbers when required. Discarding.\n",
					     session->name);
			atomic_long_inc(&session->stats.rx_seq_discards);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto discard;
		}
	}

<<<<<<< HEAD
	/* Session data offset is handled differently for L2TPv2 and
	 * L2TPv3. For L2TPv2, there is an optional 16-bit value in
	 * the header. For L2TPv3, the offset is negotiated using AVPs
	 * in the session setup control protocol.
=======
	/* Session data offset is defined only for L2TPv2 and is
	 * indicated by an optional 16-bit value in the header.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 */
	if (tunnel->version == L2TP_HDR_VER_2) {
		/* If offset bit set, skip it. */
		if (hdrflags & L2TP_HDRFLAG_O) {
			offset = ntohs(*(__be16 *)ptr);
			ptr += 2 + offset;
		}
<<<<<<< HEAD
	} else
		ptr += session->offset;
=======
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	offset = ptr - optr;
	if (!pskb_may_pull(skb, offset))
		goto discard;

	__skb_pull(skb, offset);

<<<<<<< HEAD
	/* If caller wants to process the payload before we queue the
	 * packet, do so now.
	 */
	if (payload_hook)
		if ((*payload_hook)(skb))
			goto discard;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Prepare skb for adding to the session's reorder_q.  Hold
	 * packets for max reorder_timeout or 1 second if not
	 * reordering.
	 */
	L2TP_SKB_CB(skb)->length = length;
	L2TP_SKB_CB(skb)->expires = jiffies +
		(session->reorder_timeout ? session->reorder_timeout : HZ);

	/* Add packet to the session's receive queue. Reordering is done here, if
	 * enabled. Saved L2TP protocol info is stored in skb->sb[].
	 */
	if (L2TP_SKB_CB(skb)->has_seq) {
<<<<<<< HEAD
		if (session->reorder_timeout != 0) {
			/* Packet reordering enabled. Add skb to session's
			 * reorder queue, in order of ns.
			 */
			l2tp_recv_queue_skb(session, skb);
		} else {
			/* Packet reordering disabled. Discard out-of-sequence
			 * packets
			 */
			if (L2TP_SKB_CB(skb)->ns != session->nr) {
				session->stats.rx_seq_discards++;
				PRINTK(session->debug, L2TP_MSG_SEQ, KERN_DEBUG,
				       "%s: oos pkt %u len %d discarded, "
				       "waiting for %u, reorder_q_len=%d\n",
				       session->name, L2TP_SKB_CB(skb)->ns,
				       L2TP_SKB_CB(skb)->length, session->nr,
				       skb_queue_len(&session->reorder_q));
				goto discard;
			}
			skb_queue_tail(&session->reorder_q, skb);
		}
=======
		if (l2tp_recv_data_seq(session, skb))
			goto discard;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		/* No sequence numbers. Add the skb to the tail of the
		 * reorder queue. This ensures that it will be
		 * delivered after all previous sequenced skbs.
		 */
		skb_queue_tail(&session->reorder_q, skb);
	}

	/* Try to dequeue as many skbs from reorder_q as we can. */
	l2tp_recv_dequeue(session);

	return;

discard:
<<<<<<< HEAD
	session->stats.rx_errors++;
	kfree_skb(skb);

	if (session->deref)
		(*session->deref)(session);
}
EXPORT_SYMBOL(l2tp_recv_common);
=======
	atomic_long_inc(&session->stats.rx_errors);
	kfree_skb(skb);
}
EXPORT_SYMBOL_GPL(l2tp_recv_common);

/* Drop skbs from the session's reorder_q
 */
static void l2tp_session_queue_purge(struct l2tp_session *session)
{
	struct sk_buff *skb = NULL;

	while ((skb = skb_dequeue(&session->reorder_q))) {
		atomic_long_inc(&session->stats.rx_errors);
		kfree_skb(skb);
	}
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Internal UDP receive frame. Do the real work of receiving an L2TP data frame
 * here. The skb is not on a list when we get here.
 * Returns 0 if the packet was a data packet and was successfully passed on.
 * Returns 1 if the packet was not a good data packet and could not be
 * forwarded.  All such packets are passed up to userspace to deal with.
 */
<<<<<<< HEAD
static int l2tp_udp_recv_core(struct l2tp_tunnel *tunnel, struct sk_buff *skb,
			      int (*payload_hook)(struct sk_buff *skb))
=======
static int l2tp_udp_recv_core(struct l2tp_tunnel *tunnel, struct sk_buff *skb)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct l2tp_session *session = NULL;
	unsigned char *ptr, *optr;
	u16 hdrflags;
	u32 tunnel_id, session_id;
<<<<<<< HEAD
	int offset;
	u16 version;
	int length;

	if (tunnel->sock && l2tp_verify_udp_checksum(tunnel->sock, skb))
		goto discard_bad_csum;
=======
	u16 version;
	int length;

	/* UDP has verified checksum */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* UDP always verifies the packet length. */
	__skb_pull(skb, sizeof(struct udphdr));

	/* Short packet? */
<<<<<<< HEAD
	if (!pskb_may_pull(skb, L2TP_HDR_SIZE_SEQ)) {
		PRINTK(tunnel->debug, L2TP_MSG_DATA, KERN_INFO,
		       "%s: recv short packet (len=%d)\n", tunnel->name, skb->len);
		goto error;
	}

	/* Trace packet contents, if enabled */
	if (tunnel->debug & L2TP_MSG_DATA) {
		length = min(32u, skb->len);
		if (!pskb_may_pull(skb, length))
			goto error;

		printk(KERN_DEBUG "%s: recv: ", tunnel->name);

		offset = 0;
		do {
			printk(" %02X", skb->data[offset]);
		} while (++offset < length);

		printk("\n");
	}

	/* Point to L2TP header */
	optr = ptr = skb->data;

	/* Get L2TP header flags */
	hdrflags = ntohs(*(__be16 *) ptr);
=======
	if (!pskb_may_pull(skb, L2TP_HDR_SIZE_MAX)) {
		pr_debug_ratelimited("%s: recv short packet (len=%d)\n",
				     tunnel->name, skb->len);
		goto invalid;
	}

	/* Point to L2TP header */
	optr = skb->data;
	ptr = skb->data;

	/* Get L2TP header flags */
	hdrflags = ntohs(*(__be16 *)ptr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Check protocol version */
	version = hdrflags & L2TP_HDR_VER_MASK;
	if (version != tunnel->version) {
<<<<<<< HEAD
		PRINTK(tunnel->debug, L2TP_MSG_DATA, KERN_INFO,
		       "%s: recv protocol version mismatch: got %d expected %d\n",
		       tunnel->name, version, tunnel->version);
		goto error;
=======
		pr_debug_ratelimited("%s: recv protocol version mismatch: got %d expected %d\n",
				     tunnel->name, version, tunnel->version);
		goto invalid;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* Get length of L2TP packet */
	length = skb->len;

	/* If type is control packet, it is handled by userspace. */
<<<<<<< HEAD
	if (hdrflags & L2TP_HDRFLAG_T) {
		PRINTK(tunnel->debug, L2TP_MSG_DATA, KERN_DEBUG,
		       "%s: recv control packet, len=%d\n", tunnel->name, length);
		goto error;
	}
=======
	if (hdrflags & L2TP_HDRFLAG_T)
		goto pass;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Skip flags */
	ptr += 2;

	if (tunnel->version == L2TP_HDR_VER_2) {
		/* If length is present, skip it */
		if (hdrflags & L2TP_HDRFLAG_L)
			ptr += 2;

		/* Extract tunnel and session ID */
<<<<<<< HEAD
		tunnel_id = ntohs(*(__be16 *) ptr);
		ptr += 2;
		session_id = ntohs(*(__be16 *) ptr);
=======
		tunnel_id = ntohs(*(__be16 *)ptr);
		ptr += 2;
		session_id = ntohs(*(__be16 *)ptr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ptr += 2;
	} else {
		ptr += 2;	/* skip reserved bits */
		tunnel_id = tunnel->tunnel_id;
<<<<<<< HEAD
		session_id = ntohl(*(__be32 *) ptr);
=======
		session_id = ntohl(*(__be32 *)ptr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ptr += 4;
	}

	/* Find the session context */
<<<<<<< HEAD
	session = l2tp_session_get(tunnel->l2tp_net, tunnel, session_id, true);
	if (!session || !session->recv_skb) {
		if (session) {
			if (session->deref)
				session->deref(session);
			l2tp_session_dec_refcount(session);
		}

		/* Not found? Pass to userspace to deal with */
		PRINTK(tunnel->debug, L2TP_MSG_DATA, KERN_INFO,
		       "%s: no session found (%u/%u). Passing up.\n",
		       tunnel->name, tunnel_id, session_id);
		goto error;
	}

	l2tp_recv_common(session, skb, ptr, optr, hdrflags, length, payload_hook);
=======
	session = l2tp_tunnel_get_session(tunnel, session_id);
	if (!session || !session->recv_skb) {
		if (session)
			l2tp_session_dec_refcount(session);

		/* Not found? Pass to userspace to deal with */
		pr_debug_ratelimited("%s: no session found (%u/%u). Passing up.\n",
				     tunnel->name, tunnel_id, session_id);
		goto pass;
	}

	if (tunnel->version == L2TP_HDR_VER_3 &&
	    l2tp_v3_ensure_opt_in_linear(session, skb, &ptr, &optr)) {
		l2tp_session_dec_refcount(session);
		goto invalid;
	}

	l2tp_recv_common(session, skb, ptr, optr, hdrflags, length);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	l2tp_session_dec_refcount(session);

	return 0;

<<<<<<< HEAD
discard_bad_csum:
	LIMIT_NETDEBUG("%s: UDP: bad checksum\n", tunnel->name);
	UDP_INC_STATS_USER(tunnel->l2tp_net, UDP_MIB_INERRORS, 0);
	tunnel->stats.rx_errors++;
	kfree_skb(skb);

	return 0;

error:
=======
invalid:
	atomic_long_inc(&tunnel->stats.rx_invalid);

pass:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Put UDP header back */
	__skb_push(skb, sizeof(struct udphdr));

	return 1;
}

/* UDP encapsulation receive handler. See net/ipv4/udp.c.
 * Return codes:
 * 0 : success.
 * <0: error
 * >0: skb should be passed up to userspace as UDP.
 */
int l2tp_udp_encap_recv(struct sock *sk, struct sk_buff *skb)
{
	struct l2tp_tunnel *tunnel;

<<<<<<< HEAD
	tunnel = l2tp_sock_to_tunnel(sk);
	if (tunnel == NULL)
		goto pass_up;

	PRINTK(tunnel->debug, L2TP_MSG_DATA, KERN_DEBUG,
	       "%s: received %d bytes\n", tunnel->name, skb->len);

	if (l2tp_udp_recv_core(tunnel, skb, tunnel->recv_payload_hook))
		goto pass_up_put;

	sock_put(sk);
	return 0;

pass_up_put:
	sock_put(sk);
=======
	/* Note that this is called from the encap_rcv hook inside an
	 * RCU-protected region, but without the socket being locked.
	 * Hence we use rcu_dereference_sk_user_data to access the
	 * tunnel data structure rather the usual l2tp_sk_to_tunnel
	 * accessor function.
	 */
	tunnel = rcu_dereference_sk_user_data(sk);
	if (!tunnel)
		goto pass_up;
	if (WARN_ON(tunnel->magic != L2TP_TUNNEL_MAGIC))
		goto pass_up;

	if (l2tp_udp_recv_core(tunnel, skb))
		goto pass_up;

	return 0;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
pass_up:
	return 1;
}
EXPORT_SYMBOL_GPL(l2tp_udp_encap_recv);

/************************************************************************
 * Transmit handling
 ***********************************************************************/

/* Build an L2TP header for the session into the buffer provided.
 */
static int l2tp_build_l2tpv2_header(struct l2tp_session *session, void *buf)
{
	struct l2tp_tunnel *tunnel = session->tunnel;
	__be16 *bufp = buf;
	__be16 *optr = buf;
	u16 flags = L2TP_HDR_VER_2;
	u32 tunnel_id = tunnel->peer_tunnel_id;
	u32 session_id = session->peer_session_id;

	if (session->send_seq)
		flags |= L2TP_HDRFLAG_S;

	/* Setup L2TP header. */
	*bufp++ = htons(flags);
	*bufp++ = htons(tunnel_id);
	*bufp++ = htons(session_id);
	if (session->send_seq) {
		*bufp++ = htons(session->ns);
		*bufp++ = 0;
		session->ns++;
		session->ns &= 0xffff;
<<<<<<< HEAD
		PRINTK(session->debug, L2TP_MSG_SEQ, KERN_DEBUG,
		       "%s: updated ns to %u\n", session->name, session->ns);
=======
		trace_session_seqnum_update(session);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return bufp - optr;
}

static int l2tp_build_l2tpv3_header(struct l2tp_session *session, void *buf)
{
	struct l2tp_tunnel *tunnel = session->tunnel;
	char *bufp = buf;
	char *optr = bufp;

	/* Setup L2TP header. The header differs slightly for UDP and
	 * IP encapsulations. For UDP, there is 4 bytes of flags.
	 */
	if (tunnel->encap == L2TP_ENCAPTYPE_UDP) {
		u16 flags = L2TP_HDR_VER_3;
<<<<<<< HEAD
		*((__be16 *) bufp) = htons(flags);
		bufp += 2;
		*((__be16 *) bufp) = 0;
		bufp += 2;
	}

	*((__be32 *) bufp) = htonl(session->peer_session_id);
=======
		*((__be16 *)bufp) = htons(flags);
		bufp += 2;
		*((__be16 *)bufp) = 0;
		bufp += 2;
	}

	*((__be32 *)bufp) = htonl(session->peer_session_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	bufp += 4;
	if (session->cookie_len) {
		memcpy(bufp, &session->cookie[0], session->cookie_len);
		bufp += session->cookie_len;
	}
<<<<<<< HEAD
	if (session->l2specific_len) {
		if (session->l2specific_type == L2TP_L2SPECTYPE_DEFAULT) {
			u32 l2h = 0;
			if (session->send_seq) {
				l2h = 0x40000000 | session->ns;
				session->ns++;
				session->ns &= 0xffffff;
				PRINTK(session->debug, L2TP_MSG_SEQ, KERN_DEBUG,
				       "%s: updated ns to %u\n", session->name, session->ns);
			}

			*((__be32 *) bufp) = htonl(l2h);
		}
		bufp += session->l2specific_len;
	}
	if (session->offset)
		bufp += session->offset;
=======
	if (session->l2specific_type == L2TP_L2SPECTYPE_DEFAULT) {
		u32 l2h = 0;

		if (session->send_seq) {
			l2h = 0x40000000 | session->ns;
			session->ns++;
			session->ns &= 0xffffff;
			trace_session_seqnum_update(session);
		}

		*((__be32 *)bufp) = htonl(l2h);
		bufp += 4;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return bufp - optr;
}

<<<<<<< HEAD
static int l2tp_xmit_core(struct l2tp_session *session, struct sk_buff *skb,
			  struct flowi *fl, size_t data_len)
{
	struct l2tp_tunnel *tunnel = session->tunnel;
	unsigned int len = skb->len;
	int error;

	/* Debug */
	if (session->send_seq)
		PRINTK(session->debug, L2TP_MSG_DATA, KERN_DEBUG,
		       "%s: send %Zd bytes, ns=%u\n", session->name,
		       data_len, session->ns - 1);
	else
		PRINTK(session->debug, L2TP_MSG_DATA, KERN_DEBUG,
		       "%s: send %Zd bytes\n", session->name, data_len);

	if (session->debug & L2TP_MSG_DATA) {
		int i;
		int uhlen = (tunnel->encap == L2TP_ENCAPTYPE_UDP) ? sizeof(struct udphdr) : 0;
		unsigned char *datap = skb->data + uhlen;

		printk(KERN_DEBUG "%s: xmit:", session->name);
		for (i = 0; i < (len - uhlen); i++) {
			printk(" %02X", *datap++);
			if (i == 31) {
				printk(" ...");
				break;
			}
		}
		printk("\n");
	}

	/* Queue the packet to IP for output */
	skb->local_df = 1;
	error = ip_queue_xmit(skb, fl);

	/* Update stats */
	if (error >= 0) {
		tunnel->stats.tx_packets++;
		tunnel->stats.tx_bytes += len;
		session->stats.tx_packets++;
		session->stats.tx_bytes += len;
	} else {
		tunnel->stats.tx_errors++;
		session->stats.tx_errors++;
	}

	return 0;
}

/* Automatically called when the skb is freed.
 */
static void l2tp_sock_wfree(struct sk_buff *skb)
{
	sock_put(skb->sk);
}

/* For data skbs that we transmit, we associate with the tunnel socket
 * but don't do accounting.
 */
static inline void l2tp_skb_set_owner_w(struct sk_buff *skb, struct sock *sk)
{
	sock_hold(sk);
	skb->sk = sk;
	skb->destructor = l2tp_sock_wfree;
}

/* If caller requires the skb to have a ppp header, the header must be
 * inserted in the skb data before calling this function.
 */
int l2tp_xmit_skb(struct l2tp_session *session, struct sk_buff *skb, int hdr_len)
{
	int data_len = skb->len;
	struct l2tp_tunnel *tunnel = session->tunnel;
	struct sock *sk = tunnel->sock;
	struct flowi *fl;
	struct udphdr *uh;
	struct inet_sock *inet;
	__wsum csum;
	int old_headroom;
	int new_headroom;
	int headroom;
	int uhlen = (tunnel->encap == L2TP_ENCAPTYPE_UDP) ? sizeof(struct udphdr) : 0;
	int udp_len;
=======
/* Queue the packet to IP for output: tunnel socket lock must be held */
static int l2tp_xmit_queue(struct l2tp_tunnel *tunnel, struct sk_buff *skb, struct flowi *fl)
{
	int err;

	skb->ignore_df = 1;
	skb_dst_drop(skb);
#if IS_ENABLED(CONFIG_IPV6)
	if (l2tp_sk_is_v6(tunnel->sock))
		err = inet6_csk_xmit(tunnel->sock, skb, NULL);
	else
#endif
		err = ip_queue_xmit(tunnel->sock, skb, fl);

	return err >= 0 ? NET_XMIT_SUCCESS : NET_XMIT_DROP;
}

static int l2tp_xmit_core(struct l2tp_session *session, struct sk_buff *skb, unsigned int *len)
{
	struct l2tp_tunnel *tunnel = session->tunnel;
	unsigned int data_len = skb->len;
	struct sock *sk = tunnel->sock;
	int headroom, uhlen, udp_len;
	int ret = NET_XMIT_SUCCESS;
	struct inet_sock *inet;
	struct udphdr *uh;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Check that there's enough headroom in the skb to insert IP,
	 * UDP and L2TP headers. If not enough, expand it to
	 * make room. Adjust truesize.
	 */
<<<<<<< HEAD
	headroom = NET_SKB_PAD + sizeof(struct iphdr) +
		uhlen + hdr_len;
	old_headroom = skb_headroom(skb);
	if (skb_cow_head(skb, headroom)) {
		dev_kfree_skb(skb);
		goto abort;
	}

	new_headroom = skb_headroom(skb);
	skb_orphan(skb);
	skb->truesize += new_headroom - old_headroom;

	/* Setup L2TP header */
	session->build_header(session, __skb_push(skb, hdr_len));

	/* Reset skb netfilter state */
	memset(&(IPCB(skb)->opt), 0, sizeof(IPCB(skb)->opt));
	IPCB(skb)->flags &= ~(IPSKB_XFRM_TUNNEL_SIZE | IPSKB_XFRM_TRANSFORMED |
			      IPSKB_REROUTED);
	nf_reset(skb);

	bh_lock_sock(sk);
	if (sock_owned_by_user(sk)) {
		dev_kfree_skb(skb);
		goto out_unlock;
	}

	/* Get routing info from the tunnel socket */
	skb_dst_drop(skb);
	skb_dst_set(skb, dst_clone(__sk_dst_check(sk, 0)));

	inet = inet_sk(sk);
	fl = &inet->cork.fl;
=======
	uhlen = (tunnel->encap == L2TP_ENCAPTYPE_UDP) ? sizeof(*uh) : 0;
	headroom = NET_SKB_PAD + sizeof(struct iphdr) + uhlen + session->hdr_len;
	if (skb_cow_head(skb, headroom)) {
		kfree_skb(skb);
		return NET_XMIT_DROP;
	}

	/* Setup L2TP header */
	if (tunnel->version == L2TP_HDR_VER_2)
		l2tp_build_l2tpv2_header(session, __skb_push(skb, session->hdr_len));
	else
		l2tp_build_l2tpv3_header(session, __skb_push(skb, session->hdr_len));

	/* Reset skb netfilter state */
	memset(&(IPCB(skb)->opt), 0, sizeof(IPCB(skb)->opt));
	IPCB(skb)->flags &= ~(IPSKB_XFRM_TUNNEL_SIZE | IPSKB_XFRM_TRANSFORMED | IPSKB_REROUTED);
	nf_reset_ct(skb);

	bh_lock_sock_nested(sk);
	if (sock_owned_by_user(sk)) {
		kfree_skb(skb);
		ret = NET_XMIT_DROP;
		goto out_unlock;
	}

	/* The user-space may change the connection status for the user-space
	 * provided socket at run time: we must check it under the socket lock
	 */
	if (tunnel->fd >= 0 && sk->sk_state != TCP_ESTABLISHED) {
		kfree_skb(skb);
		ret = NET_XMIT_DROP;
		goto out_unlock;
	}

	/* Report transmitted length before we add encap header, which keeps
	 * statistics consistent for both UDP and IP encap tx/rx paths.
	 */
	*len = skb->len;

	inet = inet_sk(sk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	switch (tunnel->encap) {
	case L2TP_ENCAPTYPE_UDP:
		/* Setup UDP header */
		__skb_push(skb, sizeof(*uh));
		skb_reset_transport_header(skb);
		uh = udp_hdr(skb);
		uh->source = inet->inet_sport;
		uh->dest = inet->inet_dport;
<<<<<<< HEAD
		udp_len = uhlen + hdr_len + data_len;
		uh->len = htons(udp_len);
		uh->check = 0;

		/* Calculate UDP checksum if configured to do so */
		if (sk->sk_no_check == UDP_CSUM_NOXMIT)
			skb->ip_summed = CHECKSUM_NONE;
		else if ((skb_dst(skb) && skb_dst(skb)->dev) &&
			 (!(skb_dst(skb)->dev->features & NETIF_F_V4_CSUM))) {
			skb->ip_summed = CHECKSUM_COMPLETE;
			csum = skb_checksum(skb, 0, udp_len, 0);
			uh->check = csum_tcpudp_magic(inet->inet_saddr,
						      inet->inet_daddr,
						      udp_len, IPPROTO_UDP, csum);
			if (uh->check == 0)
				uh->check = CSUM_MANGLED_0;
		} else {
			skb->ip_summed = CHECKSUM_PARTIAL;
			skb->csum_start = skb_transport_header(skb) - skb->head;
			skb->csum_offset = offsetof(struct udphdr, check);
			uh->check = ~csum_tcpudp_magic(inet->inet_saddr,
						       inet->inet_daddr,
						       udp_len, IPPROTO_UDP, 0);
		}
=======
		udp_len = uhlen + session->hdr_len + data_len;
		uh->len = htons(udp_len);

		/* Calculate UDP checksum if configured to do so */
#if IS_ENABLED(CONFIG_IPV6)
		if (l2tp_sk_is_v6(sk))
			udp6_set_csum(udp_get_no_check6_tx(sk),
				      skb, &inet6_sk(sk)->saddr,
				      &sk->sk_v6_daddr, udp_len);
		else
#endif
			udp_set_csum(sk->sk_no_check_tx, skb, inet->inet_saddr,
				     inet->inet_daddr, udp_len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;

	case L2TP_ENCAPTYPE_IP:
		break;
	}

<<<<<<< HEAD
	l2tp_skb_set_owner_w(skb, sk);

	l2tp_xmit_core(session, skb, fl, data_len);
out_unlock:
	bh_unlock_sock(sk);

abort:
	return 0;
=======
	ret = l2tp_xmit_queue(tunnel, skb, &inet->cork.fl);

out_unlock:
	bh_unlock_sock(sk);

	return ret;
}

/* If caller requires the skb to have a ppp header, the header must be
 * inserted in the skb data before calling this function.
 */
int l2tp_xmit_skb(struct l2tp_session *session, struct sk_buff *skb)
{
	unsigned int len = 0;
	int ret;

	ret = l2tp_xmit_core(session, skb, &len);
	if (ret == NET_XMIT_SUCCESS) {
		atomic_long_inc(&session->tunnel->stats.tx_packets);
		atomic_long_add(len, &session->tunnel->stats.tx_bytes);
		atomic_long_inc(&session->stats.tx_packets);
		atomic_long_add(len, &session->stats.tx_bytes);
	} else {
		atomic_long_inc(&session->tunnel->stats.tx_errors);
		atomic_long_inc(&session->stats.tx_errors);
	}
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(l2tp_xmit_skb);

/*****************************************************************************
 * Tinnel and session create/destroy.
 *****************************************************************************/

/* Tunnel socket destruct hook.
 * The tunnel context is deleted only when all session sockets have been
 * closed.
 */
static void l2tp_tunnel_destruct(struct sock *sk)
{
<<<<<<< HEAD
	struct l2tp_tunnel *tunnel;

	tunnel = sk->sk_user_data;
	if (tunnel == NULL)
		goto end;

	PRINTK(tunnel->debug, L2TP_MSG_CONTROL, KERN_INFO,
	       "%s: closing...\n", tunnel->name);

	/* Close all sessions */
	l2tp_tunnel_closeall(tunnel);

	switch (tunnel->encap) {
	case L2TP_ENCAPTYPE_UDP:
		/* No longer an encapsulation socket. See net/ipv4/udp.c */
		(udp_sk(sk))->encap_type = 0;
		(udp_sk(sk))->encap_rcv = NULL;
=======
	struct l2tp_tunnel *tunnel = l2tp_sk_to_tunnel(sk);

	if (!tunnel)
		goto end;

	/* Disable udp encapsulation */
	switch (tunnel->encap) {
	case L2TP_ENCAPTYPE_UDP:
		/* No longer an encapsulation socket. See net/ipv4/udp.c */
		WRITE_ONCE(udp_sk(sk)->encap_type, 0);
		udp_sk(sk)->encap_rcv = NULL;
		udp_sk(sk)->encap_destroy = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case L2TP_ENCAPTYPE_IP:
		break;
	}

	/* Remove hooks into tunnel socket */
<<<<<<< HEAD
	tunnel->sock = NULL;
	sk->sk_destruct = tunnel->old_sk_destruct;
	sk->sk_user_data = NULL;
=======
	write_lock_bh(&sk->sk_callback_lock);
	sk->sk_destruct = tunnel->old_sk_destruct;
	sk->sk_user_data = NULL;
	write_unlock_bh(&sk->sk_callback_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Call the original destructor */
	if (sk->sk_destruct)
		(*sk->sk_destruct)(sk);

<<<<<<< HEAD
	/* We're finished with the socket */
	l2tp_tunnel_dec_refcount(tunnel);

=======
	kfree_rcu(tunnel, rcu);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
end:
	return;
}

<<<<<<< HEAD
=======
/* Remove an l2tp session from l2tp_core's hash lists. */
static void l2tp_session_unhash(struct l2tp_session *session)
{
	struct l2tp_tunnel *tunnel = session->tunnel;

	/* Remove the session from core hashes */
	if (tunnel) {
		/* Remove from the per-tunnel hash */
		spin_lock_bh(&tunnel->hlist_lock);
		hlist_del_init_rcu(&session->hlist);
		spin_unlock_bh(&tunnel->hlist_lock);

		/* For L2TPv3 we have a per-net hash: remove from there, too */
		if (tunnel->version != L2TP_HDR_VER_2) {
			struct l2tp_net *pn = l2tp_pernet(tunnel->l2tp_net);

			spin_lock_bh(&pn->l2tp_session_hlist_lock);
			hlist_del_init_rcu(&session->global_hlist);
			spin_unlock_bh(&pn->l2tp_session_hlist_lock);
		}

		synchronize_rcu();
	}
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* When the tunnel is closed, all the attached sessions need to go too.
 */
static void l2tp_tunnel_closeall(struct l2tp_tunnel *tunnel)
{
<<<<<<< HEAD
	int hash;
	struct hlist_node *walk;
	struct hlist_node *tmp;
	struct l2tp_session *session;

	BUG_ON(tunnel == NULL);

	PRINTK(tunnel->debug, L2TP_MSG_CONTROL, KERN_INFO,
	       "%s: closing all sessions...\n", tunnel->name);

	write_lock_bh(&tunnel->hlist_lock);
	tunnel->acpt_newsess = false;
	for (hash = 0; hash < L2TP_HASH_SIZE; hash++) {
again:
		hlist_for_each_safe(walk, tmp, &tunnel->session_hlist[hash]) {
			session = hlist_entry(walk, struct l2tp_session, hlist);

			PRINTK(session->debug, L2TP_MSG_CONTROL, KERN_INFO,
			       "%s: closing session\n", session->name);

			hlist_del_init(&session->hlist);

			/* Since we should hold the sock lock while
			 * doing any unbinding, we need to release the
			 * lock we're holding before taking that lock.
			 * Hold a reference to the sock so it doesn't
			 * disappear as we're jumping between locks.
			 */
			if (session->ref != NULL)
				(*session->ref)(session);

			write_unlock_bh(&tunnel->hlist_lock);

			if (tunnel->version != L2TP_HDR_VER_2) {
				struct l2tp_net *pn = l2tp_pernet(tunnel->l2tp_net);

				spin_lock_bh(&pn->l2tp_session_hlist_lock);
				hlist_del_init_rcu(&session->global_hlist);
				spin_unlock_bh(&pn->l2tp_session_hlist_lock);
				synchronize_rcu();
			}

			if (session->session_close != NULL)
				(*session->session_close)(session);

			if (session->deref != NULL)
				(*session->deref)(session);

			write_lock_bh(&tunnel->hlist_lock);
=======
	struct l2tp_session *session;
	int hash;

	spin_lock_bh(&tunnel->hlist_lock);
	tunnel->acpt_newsess = false;
	for (hash = 0; hash < L2TP_HASH_SIZE; hash++) {
again:
		hlist_for_each_entry_rcu(session, &tunnel->session_hlist[hash], hlist) {
			hlist_del_init_rcu(&session->hlist);

			spin_unlock_bh(&tunnel->hlist_lock);
			l2tp_session_delete(session);
			spin_lock_bh(&tunnel->hlist_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

			/* Now restart from the beginning of this hash
			 * chain.  We always remove a session from the
			 * list so we are guaranteed to make forward
			 * progress.
			 */
			goto again;
		}
	}
<<<<<<< HEAD
	write_unlock_bh(&tunnel->hlist_lock);
}

/* Really kill the tunnel.
 * Come here only when all sessions have been cleared from the tunnel.
 */
static void l2tp_tunnel_free(struct l2tp_tunnel *tunnel)
{
	struct l2tp_net *pn = l2tp_pernet(tunnel->l2tp_net);

	BUG_ON(atomic_read(&tunnel->ref_count) != 0);
	BUG_ON(tunnel->sock != NULL);

	PRINTK(tunnel->debug, L2TP_MSG_CONTROL, KERN_INFO,
	       "%s: free...\n", tunnel->name);

	/* Remove from tunnel list */
	spin_lock_bh(&pn->l2tp_tunnel_list_lock);
	list_del_rcu(&tunnel->list);
	kfree_rcu(tunnel, rcu);
	spin_unlock_bh(&pn->l2tp_tunnel_list_lock);

	atomic_dec(&l2tp_tunnel_count);
=======
	spin_unlock_bh(&tunnel->hlist_lock);
}

/* Tunnel socket destroy hook for UDP encapsulation */
static void l2tp_udp_encap_destroy(struct sock *sk)
{
	struct l2tp_tunnel *tunnel = l2tp_sk_to_tunnel(sk);

	if (tunnel)
		l2tp_tunnel_delete(tunnel);
}

static void l2tp_tunnel_remove(struct net *net, struct l2tp_tunnel *tunnel)
{
	struct l2tp_net *pn = l2tp_pernet(net);

	spin_lock_bh(&pn->l2tp_tunnel_idr_lock);
	idr_remove(&pn->l2tp_tunnel_idr, tunnel->tunnel_id);
	spin_unlock_bh(&pn->l2tp_tunnel_idr_lock);
}

/* Workqueue tunnel deletion function */
static void l2tp_tunnel_del_work(struct work_struct *work)
{
	struct l2tp_tunnel *tunnel = container_of(work, struct l2tp_tunnel,
						  del_work);
	struct sock *sk = tunnel->sock;
	struct socket *sock = sk->sk_socket;

	l2tp_tunnel_closeall(tunnel);

	/* If the tunnel socket was created within the kernel, use
	 * the sk API to release it here.
	 */
	if (tunnel->fd < 0) {
		if (sock) {
			kernel_sock_shutdown(sock, SHUT_RDWR);
			sock_release(sock);
		}
	}

	l2tp_tunnel_remove(tunnel->l2tp_net, tunnel);
	/* drop initial ref */
	l2tp_tunnel_dec_refcount(tunnel);

	/* drop workqueue ref */
	l2tp_tunnel_dec_refcount(tunnel);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Create a socket for the tunnel, if one isn't set up by
 * userspace. This is used for static tunnels where there is no
 * managing L2TP daemon.
<<<<<<< HEAD
 */
static int l2tp_tunnel_sock_create(u32 tunnel_id, u32 peer_tunnel_id, struct l2tp_tunnel_cfg *cfg, struct socket **sockp)
{
	int err = -EINVAL;
	struct sockaddr_in udp_addr;
	struct sockaddr_l2tpip ip_addr;
	struct socket *sock = NULL;

	switch (cfg->encap) {
	case L2TP_ENCAPTYPE_UDP:
		err = sock_create(AF_INET, SOCK_DGRAM, 0, sockp);
		if (err < 0)
			goto out;

		sock = *sockp;

		memset(&udp_addr, 0, sizeof(udp_addr));
		udp_addr.sin_family = AF_INET;
		udp_addr.sin_addr = cfg->local_ip;
		udp_addr.sin_port = htons(cfg->local_udp_port);
		err = kernel_bind(sock, (struct sockaddr *) &udp_addr, sizeof(udp_addr));
		if (err < 0)
			goto out;

		udp_addr.sin_family = AF_INET;
		udp_addr.sin_addr = cfg->peer_ip;
		udp_addr.sin_port = htons(cfg->peer_udp_port);
		err = kernel_connect(sock, (struct sockaddr *) &udp_addr, sizeof(udp_addr), 0);
		if (err < 0)
			goto out;

		if (!cfg->use_udp_checksums)
			sock->sk->sk_no_check = UDP_CSUM_NOXMIT;

		break;

	case L2TP_ENCAPTYPE_IP:
		err = sock_create(AF_INET, SOCK_DGRAM, IPPROTO_L2TP, sockp);
		if (err < 0)
			goto out;

		sock = *sockp;

		memset(&ip_addr, 0, sizeof(ip_addr));
		ip_addr.l2tp_family = AF_INET;
		ip_addr.l2tp_addr = cfg->local_ip;
		ip_addr.l2tp_conn_id = tunnel_id;
		err = kernel_bind(sock, (struct sockaddr *) &ip_addr, sizeof(ip_addr));
		if (err < 0)
			goto out;

		ip_addr.l2tp_family = AF_INET;
		ip_addr.l2tp_addr = cfg->peer_ip;
		ip_addr.l2tp_conn_id = peer_tunnel_id;
		err = kernel_connect(sock, (struct sockaddr *) &ip_addr, sizeof(ip_addr), 0);
		if (err < 0)
			goto out;

=======
 *
 * Since we don't want these sockets to keep a namespace alive by
 * themselves, we drop the socket's namespace refcount after creation.
 * These sockets are freed when the namespace exits using the pernet
 * exit hook.
 */
static int l2tp_tunnel_sock_create(struct net *net,
				   u32 tunnel_id,
				   u32 peer_tunnel_id,
				   struct l2tp_tunnel_cfg *cfg,
				   struct socket **sockp)
{
	int err = -EINVAL;
	struct socket *sock = NULL;
	struct udp_port_cfg udp_conf;

	switch (cfg->encap) {
	case L2TP_ENCAPTYPE_UDP:
		memset(&udp_conf, 0, sizeof(udp_conf));

#if IS_ENABLED(CONFIG_IPV6)
		if (cfg->local_ip6 && cfg->peer_ip6) {
			udp_conf.family = AF_INET6;
			memcpy(&udp_conf.local_ip6, cfg->local_ip6,
			       sizeof(udp_conf.local_ip6));
			memcpy(&udp_conf.peer_ip6, cfg->peer_ip6,
			       sizeof(udp_conf.peer_ip6));
			udp_conf.use_udp6_tx_checksums =
			  !cfg->udp6_zero_tx_checksums;
			udp_conf.use_udp6_rx_checksums =
			  !cfg->udp6_zero_rx_checksums;
		} else
#endif
		{
			udp_conf.family = AF_INET;
			udp_conf.local_ip = cfg->local_ip;
			udp_conf.peer_ip = cfg->peer_ip;
			udp_conf.use_udp_checksums = cfg->use_udp_checksums;
		}

		udp_conf.local_udp_port = htons(cfg->local_udp_port);
		udp_conf.peer_udp_port = htons(cfg->peer_udp_port);

		err = udp_sock_create(net, &udp_conf, &sock);
		if (err < 0)
			goto out;

		break;

	case L2TP_ENCAPTYPE_IP:
#if IS_ENABLED(CONFIG_IPV6)
		if (cfg->local_ip6 && cfg->peer_ip6) {
			struct sockaddr_l2tpip6 ip6_addr = {0};

			err = sock_create_kern(net, AF_INET6, SOCK_DGRAM,
					       IPPROTO_L2TP, &sock);
			if (err < 0)
				goto out;

			ip6_addr.l2tp_family = AF_INET6;
			memcpy(&ip6_addr.l2tp_addr, cfg->local_ip6,
			       sizeof(ip6_addr.l2tp_addr));
			ip6_addr.l2tp_conn_id = tunnel_id;
			err = kernel_bind(sock, (struct sockaddr *)&ip6_addr,
					  sizeof(ip6_addr));
			if (err < 0)
				goto out;

			ip6_addr.l2tp_family = AF_INET6;
			memcpy(&ip6_addr.l2tp_addr, cfg->peer_ip6,
			       sizeof(ip6_addr.l2tp_addr));
			ip6_addr.l2tp_conn_id = peer_tunnel_id;
			err = kernel_connect(sock,
					     (struct sockaddr *)&ip6_addr,
					     sizeof(ip6_addr), 0);
			if (err < 0)
				goto out;
		} else
#endif
		{
			struct sockaddr_l2tpip ip_addr = {0};

			err = sock_create_kern(net, AF_INET, SOCK_DGRAM,
					       IPPROTO_L2TP, &sock);
			if (err < 0)
				goto out;

			ip_addr.l2tp_family = AF_INET;
			ip_addr.l2tp_addr = cfg->local_ip;
			ip_addr.l2tp_conn_id = tunnel_id;
			err = kernel_bind(sock, (struct sockaddr *)&ip_addr,
					  sizeof(ip_addr));
			if (err < 0)
				goto out;

			ip_addr.l2tp_family = AF_INET;
			ip_addr.l2tp_addr = cfg->peer_ip;
			ip_addr.l2tp_conn_id = peer_tunnel_id;
			err = kernel_connect(sock, (struct sockaddr *)&ip_addr,
					     sizeof(ip_addr), 0);
			if (err < 0)
				goto out;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;

	default:
		goto out;
	}

out:
<<<<<<< HEAD
	if ((err < 0) && sock) {
=======
	*sockp = sock;
	if (err < 0 && sock) {
		kernel_sock_shutdown(sock, SHUT_RDWR);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		sock_release(sock);
		*sockp = NULL;
	}

	return err;
}

<<<<<<< HEAD
int l2tp_tunnel_create(struct net *net, int fd, int version, u32 tunnel_id, u32 peer_tunnel_id, struct l2tp_tunnel_cfg *cfg, struct l2tp_tunnel **tunnelp)
{
	struct l2tp_tunnel *tunnel = NULL;
	int err;
	struct socket *sock = NULL;
	struct sock *sk = NULL;
	struct l2tp_net *pn;
	enum l2tp_encap_type encap = L2TP_ENCAPTYPE_UDP;

	/* Get the tunnel socket from the fd, which was opened by
	 * the userspace L2TP daemon. If not specified, create a
	 * kernel socket.
	 */
	if (fd < 0) {
		err = l2tp_tunnel_sock_create(tunnel_id, peer_tunnel_id, cfg, &sock);
		if (err < 0)
			goto err;
	} else {
		err = -EBADF;
		sock = sockfd_lookup(fd, &err);
		if (!sock) {
			printk(KERN_ERR "tunl %hu: sockfd_lookup(fd=%d) returned %d\n",
			       tunnel_id, fd, err);
			goto err;
		}
	}

	sk = sock->sk;

	if (cfg != NULL)
		encap = cfg->encap;

	/* Quick sanity checks */
	switch (encap) {
	case L2TP_ENCAPTYPE_UDP:
		err = -EPROTONOSUPPORT;
		if (sk->sk_protocol != IPPROTO_UDP) {
			printk(KERN_ERR "tunl %hu: fd %d wrong protocol, got %d, expected %d\n",
			       tunnel_id, fd, sk->sk_protocol, IPPROTO_UDP);
			goto err;
		}
		break;
	case L2TP_ENCAPTYPE_IP:
		err = -EPROTONOSUPPORT;
		if (sk->sk_protocol != IPPROTO_L2TP) {
			printk(KERN_ERR "tunl %hu: fd %d wrong protocol, got %d, expected %d\n",
			       tunnel_id, fd, sk->sk_protocol, IPPROTO_L2TP);
			goto err;
		}
		break;
	}

	/* Check if this socket has already been prepped */
	tunnel = (struct l2tp_tunnel *)sk->sk_user_data;
	if (tunnel != NULL) {
		/* This socket has already been prepped */
		err = -EBUSY;
		goto err;
	}

	tunnel = kzalloc(sizeof(struct l2tp_tunnel), GFP_KERNEL);
	if (tunnel == NULL) {
=======
int l2tp_tunnel_create(int fd, int version, u32 tunnel_id, u32 peer_tunnel_id,
		       struct l2tp_tunnel_cfg *cfg, struct l2tp_tunnel **tunnelp)
{
	struct l2tp_tunnel *tunnel = NULL;
	int err;
	enum l2tp_encap_type encap = L2TP_ENCAPTYPE_UDP;

	if (cfg)
		encap = cfg->encap;

	tunnel = kzalloc(sizeof(*tunnel), GFP_KERNEL);
	if (!tunnel) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = -ENOMEM;
		goto err;
	}

	tunnel->version = version;
	tunnel->tunnel_id = tunnel_id;
	tunnel->peer_tunnel_id = peer_tunnel_id;
<<<<<<< HEAD
	tunnel->debug = L2TP_DEFAULT_DEBUG_FLAGS;

	tunnel->magic = L2TP_TUNNEL_MAGIC;
	sprintf(&tunnel->name[0], "tunl %u", tunnel_id);
	rwlock_init(&tunnel->hlist_lock);
	tunnel->acpt_newsess = true;

	/* The net we belong to */
	tunnel->l2tp_net = net;
	pn = l2tp_pernet(net);

	if (cfg != NULL)
		tunnel->debug = cfg->debug;

	/* Mark socket as an encapsulation socket. See net/ipv4/udp.c */
	tunnel->encap = encap;
	if (encap == L2TP_ENCAPTYPE_UDP) {
		/* Mark socket as an encapsulation socket. See net/ipv4/udp.c */
		udp_sk(sk)->encap_type = UDP_ENCAP_L2TPINUDP;
		udp_sk(sk)->encap_rcv = l2tp_udp_encap_recv;
	}

	sk->sk_user_data = tunnel;

	/* Hook on the tunnel socket destructor so that we can cleanup
	 * if the tunnel socket goes away.
	 */
	tunnel->old_sk_destruct = sk->sk_destruct;
	sk->sk_destruct = &l2tp_tunnel_destruct;
	tunnel->sock = sk;
	sk->sk_allocation = GFP_ATOMIC;

	/* Add tunnel to our list */
	INIT_LIST_HEAD(&tunnel->list);
	atomic_inc(&l2tp_tunnel_count);

	/* Bump the reference count. The tunnel context is deleted
	 * only when this drops to zero. Must be done before list insertion
	 */
	l2tp_tunnel_inc_refcount(tunnel);
	spin_lock_bh(&pn->l2tp_tunnel_list_lock);
	list_add_rcu(&tunnel->list, &pn->l2tp_tunnel_list);
	spin_unlock_bh(&pn->l2tp_tunnel_list_lock);
=======

	tunnel->magic = L2TP_TUNNEL_MAGIC;
	sprintf(&tunnel->name[0], "tunl %u", tunnel_id);
	spin_lock_init(&tunnel->hlist_lock);
	tunnel->acpt_newsess = true;

	tunnel->encap = encap;

	refcount_set(&tunnel->ref_count, 1);
	tunnel->fd = fd;

	/* Init delete workqueue struct */
	INIT_WORK(&tunnel->del_work, l2tp_tunnel_del_work);

	INIT_LIST_HEAD(&tunnel->list);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	err = 0;
err:
	if (tunnelp)
		*tunnelp = tunnel;

<<<<<<< HEAD
	/* If tunnel's socket was created by the kernel, it doesn't
	 *  have a file.
	 */
	if (sock && sock->file)
		sockfd_put(sock);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}
EXPORT_SYMBOL_GPL(l2tp_tunnel_create);

<<<<<<< HEAD
/* This function is used by the netlink TUNNEL_DELETE command.
 */
int l2tp_tunnel_delete(struct l2tp_tunnel *tunnel)
{
	int err = 0;
	struct socket *sock = tunnel->sock ? tunnel->sock->sk_socket : NULL;

	/* Force the tunnel socket to close. This will eventually
	 * cause the tunnel to be deleted via the normal socket close
	 * mechanisms when userspace closes the tunnel socket.
	 */
	if (sock != NULL) {
		err = inet_shutdown(sock, 2);

		/* If the tunnel's socket was created by the kernel,
		 * close the socket here since the socket was not
		 * created by userspace.
		 */
		if (sock->file == NULL)
			err = inet_release(sock);
	}

	return err;
}
EXPORT_SYMBOL_GPL(l2tp_tunnel_delete);

/* Really kill the session.
 */
void l2tp_session_free(struct l2tp_session *session)
{
	struct l2tp_tunnel *tunnel;

	BUG_ON(atomic_read(&session->ref_count) != 0);

	tunnel = session->tunnel;
	if (tunnel != NULL) {
		BUG_ON(tunnel->magic != L2TP_TUNNEL_MAGIC);

		/* Delete the session from the hash */
		write_lock_bh(&tunnel->hlist_lock);
		hlist_del_init(&session->hlist);
		write_unlock_bh(&tunnel->hlist_lock);

		/* Unlink from the global hash if not L2TPv2 */
		if (tunnel->version != L2TP_HDR_VER_2) {
			struct l2tp_net *pn = l2tp_pernet(tunnel->l2tp_net);

			spin_lock_bh(&pn->l2tp_session_hlist_lock);
			hlist_del_init_rcu(&session->global_hlist);
			spin_unlock_bh(&pn->l2tp_session_hlist_lock);
			synchronize_rcu();
		}

		if (session->session_id != 0)
			atomic_dec(&l2tp_session_count);

		sock_put(tunnel->sock);

		/* This will delete the tunnel context if this
		 * is the last session on the tunnel.
		 */
		session->tunnel = NULL;
		l2tp_tunnel_dec_refcount(tunnel);
	}

	kfree(session);

	return;
}
EXPORT_SYMBOL_GPL(l2tp_session_free);

/* This function is used by the netlink SESSION_DELETE command and by
   pseudowire modules.
 */
int l2tp_session_delete(struct l2tp_session *session)
{
	if (session->session_close != NULL)
		(*session->session_close)(session);

	l2tp_session_dec_refcount(session);

	return 0;
}
EXPORT_SYMBOL_GPL(l2tp_session_delete);


/* We come here whenever a session's send_seq, cookie_len or
 * l2specific_len parameters are set.
 */
static void l2tp_session_set_header_len(struct l2tp_session *session, int version)
=======
static int l2tp_validate_socket(const struct sock *sk, const struct net *net,
				enum l2tp_encap_type encap)
{
	if (!net_eq(sock_net(sk), net))
		return -EINVAL;

	if (sk->sk_type != SOCK_DGRAM)
		return -EPROTONOSUPPORT;

	if (sk->sk_family != PF_INET && sk->sk_family != PF_INET6)
		return -EPROTONOSUPPORT;

	if ((encap == L2TP_ENCAPTYPE_UDP && sk->sk_protocol != IPPROTO_UDP) ||
	    (encap == L2TP_ENCAPTYPE_IP && sk->sk_protocol != IPPROTO_L2TP))
		return -EPROTONOSUPPORT;

	if (sk->sk_user_data)
		return -EBUSY;

	return 0;
}

int l2tp_tunnel_register(struct l2tp_tunnel *tunnel, struct net *net,
			 struct l2tp_tunnel_cfg *cfg)
{
	struct l2tp_net *pn = l2tp_pernet(net);
	u32 tunnel_id = tunnel->tunnel_id;
	struct socket *sock;
	struct sock *sk;
	int ret;

	spin_lock_bh(&pn->l2tp_tunnel_idr_lock);
	ret = idr_alloc_u32(&pn->l2tp_tunnel_idr, NULL, &tunnel_id, tunnel_id,
			    GFP_ATOMIC);
	spin_unlock_bh(&pn->l2tp_tunnel_idr_lock);
	if (ret)
		return ret == -ENOSPC ? -EEXIST : ret;

	if (tunnel->fd < 0) {
		ret = l2tp_tunnel_sock_create(net, tunnel->tunnel_id,
					      tunnel->peer_tunnel_id, cfg,
					      &sock);
		if (ret < 0)
			goto err;
	} else {
		sock = sockfd_lookup(tunnel->fd, &ret);
		if (!sock)
			goto err;
	}

	sk = sock->sk;
	lock_sock(sk);
	write_lock_bh(&sk->sk_callback_lock);
	ret = l2tp_validate_socket(sk, net, tunnel->encap);
	if (ret < 0)
		goto err_inval_sock;
	rcu_assign_sk_user_data(sk, tunnel);
	write_unlock_bh(&sk->sk_callback_lock);

	if (tunnel->encap == L2TP_ENCAPTYPE_UDP) {
		struct udp_tunnel_sock_cfg udp_cfg = {
			.sk_user_data = tunnel,
			.encap_type = UDP_ENCAP_L2TPINUDP,
			.encap_rcv = l2tp_udp_encap_recv,
			.encap_destroy = l2tp_udp_encap_destroy,
		};

		setup_udp_tunnel_sock(net, sock, &udp_cfg);
	}

	tunnel->old_sk_destruct = sk->sk_destruct;
	sk->sk_destruct = &l2tp_tunnel_destruct;
	sk->sk_allocation = GFP_ATOMIC;
	release_sock(sk);

	sock_hold(sk);
	tunnel->sock = sk;
	tunnel->l2tp_net = net;

	spin_lock_bh(&pn->l2tp_tunnel_idr_lock);
	idr_replace(&pn->l2tp_tunnel_idr, tunnel, tunnel->tunnel_id);
	spin_unlock_bh(&pn->l2tp_tunnel_idr_lock);

	trace_register_tunnel(tunnel);

	if (tunnel->fd >= 0)
		sockfd_put(sock);

	return 0;

err_inval_sock:
	write_unlock_bh(&sk->sk_callback_lock);
	release_sock(sk);

	if (tunnel->fd < 0)
		sock_release(sock);
	else
		sockfd_put(sock);
err:
	l2tp_tunnel_remove(net, tunnel);
	return ret;
}
EXPORT_SYMBOL_GPL(l2tp_tunnel_register);

/* This function is used by the netlink TUNNEL_DELETE command.
 */
void l2tp_tunnel_delete(struct l2tp_tunnel *tunnel)
{
	if (!test_and_set_bit(0, &tunnel->dead)) {
		trace_delete_tunnel(tunnel);
		l2tp_tunnel_inc_refcount(tunnel);
		queue_work(l2tp_wq, &tunnel->del_work);
	}
}
EXPORT_SYMBOL_GPL(l2tp_tunnel_delete);

void l2tp_session_delete(struct l2tp_session *session)
{
	if (test_and_set_bit(0, &session->dead))
		return;

	trace_delete_session(session);
	l2tp_session_unhash(session);
	l2tp_session_queue_purge(session);
	if (session->session_close)
		(*session->session_close)(session);

	l2tp_session_dec_refcount(session);
}
EXPORT_SYMBOL_GPL(l2tp_session_delete);

/* We come here whenever a session's send_seq, cookie_len or
 * l2specific_type parameters are set.
 */
void l2tp_session_set_header_len(struct l2tp_session *session, int version)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	if (version == L2TP_HDR_VER_2) {
		session->hdr_len = 6;
		if (session->send_seq)
			session->hdr_len += 4;
	} else {
<<<<<<< HEAD
		session->hdr_len = 4 + session->cookie_len + session->l2specific_len + session->offset;
		if (session->tunnel->encap == L2TP_ENCAPTYPE_UDP)
			session->hdr_len += 4;
	}

}

struct l2tp_session *l2tp_session_create(int priv_size, struct l2tp_tunnel *tunnel, u32 session_id, u32 peer_session_id, struct l2tp_session_cfg *cfg)
{
	struct l2tp_session *session;
	int err;

	session = kzalloc(sizeof(struct l2tp_session) + priv_size, GFP_KERNEL);
	if (session != NULL) {
=======
		session->hdr_len = 4 + session->cookie_len;
		session->hdr_len += l2tp_get_l2specific_len(session);
		if (session->tunnel->encap == L2TP_ENCAPTYPE_UDP)
			session->hdr_len += 4;
	}
}
EXPORT_SYMBOL_GPL(l2tp_session_set_header_len);

struct l2tp_session *l2tp_session_create(int priv_size, struct l2tp_tunnel *tunnel, u32 session_id,
					 u32 peer_session_id, struct l2tp_session_cfg *cfg)
{
	struct l2tp_session *session;

	session = kzalloc(sizeof(*session) + priv_size, GFP_KERNEL);
	if (session) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		session->magic = L2TP_SESSION_MAGIC;
		session->tunnel = tunnel;

		session->session_id = session_id;
		session->peer_session_id = peer_session_id;
<<<<<<< HEAD
		session->nr = 1;
=======
		session->nr = 0;
		if (tunnel->version == L2TP_HDR_VER_2)
			session->nr_max = 0xffff;
		else
			session->nr_max = 0xffffff;
		session->nr_window_size = session->nr_max / 2;
		session->nr_oos_count_max = 4;

		/* Use NR of first received packet */
		session->reorder_skip = 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		sprintf(&session->name[0], "sess %u/%u",
			tunnel->tunnel_id, session->session_id);

		skb_queue_head_init(&session->reorder_q);

		INIT_HLIST_NODE(&session->hlist);
		INIT_HLIST_NODE(&session->global_hlist);

<<<<<<< HEAD
		/* Inherit debug options from tunnel */
		session->debug = tunnel->debug;

		if (cfg) {
			session->pwtype = cfg->pw_type;
			session->debug = cfg->debug;
			session->mtu = cfg->mtu;
			session->mru = cfg->mru;
=======
		if (cfg) {
			session->pwtype = cfg->pw_type;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			session->send_seq = cfg->send_seq;
			session->recv_seq = cfg->recv_seq;
			session->lns_mode = cfg->lns_mode;
			session->reorder_timeout = cfg->reorder_timeout;
<<<<<<< HEAD
			session->offset = cfg->offset;
			session->l2specific_type = cfg->l2specific_type;
			session->l2specific_len = cfg->l2specific_len;
=======
			session->l2specific_type = cfg->l2specific_type;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			session->cookie_len = cfg->cookie_len;
			memcpy(&session->cookie[0], &cfg->cookie[0], cfg->cookie_len);
			session->peer_cookie_len = cfg->peer_cookie_len;
			memcpy(&session->peer_cookie[0], &cfg->peer_cookie[0], cfg->peer_cookie_len);
		}

<<<<<<< HEAD
		if (tunnel->version == L2TP_HDR_VER_2)
			session->build_header = l2tp_build_l2tpv2_header;
		else
			session->build_header = l2tp_build_l2tpv3_header;

		l2tp_session_set_header_len(session, tunnel->version);

		l2tp_session_inc_refcount(session);

		err = l2tp_session_add_to_tunnel(tunnel, session);
		if (err) {
			kfree(session);

			return ERR_PTR(err);
		}

		/* Ignore management session in session count value */
		if (session->session_id != 0)
			atomic_inc(&l2tp_session_count);
=======
		l2tp_session_set_header_len(session, tunnel->version);

		refcount_set(&session->ref_count, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		return session;
	}

	return ERR_PTR(-ENOMEM);
}
EXPORT_SYMBOL_GPL(l2tp_session_create);

/*****************************************************************************
 * Init and cleanup
 *****************************************************************************/

static __net_init int l2tp_init_net(struct net *net)
{
	struct l2tp_net *pn = net_generic(net, l2tp_net_id);
	int hash;

<<<<<<< HEAD
	INIT_LIST_HEAD(&pn->l2tp_tunnel_list);
	spin_lock_init(&pn->l2tp_tunnel_list_lock);
=======
	idr_init(&pn->l2tp_tunnel_idr);
	spin_lock_init(&pn->l2tp_tunnel_idr_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	for (hash = 0; hash < L2TP_HASH_SIZE_2; hash++)
		INIT_HLIST_HEAD(&pn->l2tp_session_hlist[hash]);

	spin_lock_init(&pn->l2tp_session_hlist_lock);

	return 0;
}

<<<<<<< HEAD
static struct pernet_operations l2tp_net_ops = {
	.init = l2tp_init_net,
=======
static __net_exit void l2tp_exit_net(struct net *net)
{
	struct l2tp_net *pn = l2tp_pernet(net);
	struct l2tp_tunnel *tunnel = NULL;
	unsigned long tunnel_id, tmp;
	int hash;

	rcu_read_lock_bh();
	idr_for_each_entry_ul(&pn->l2tp_tunnel_idr, tunnel, tmp, tunnel_id) {
		if (tunnel)
			l2tp_tunnel_delete(tunnel);
	}
	rcu_read_unlock_bh();

	if (l2tp_wq)
		flush_workqueue(l2tp_wq);
	rcu_barrier();

	for (hash = 0; hash < L2TP_HASH_SIZE_2; hash++)
		WARN_ON_ONCE(!hlist_empty(&pn->l2tp_session_hlist[hash]));
	idr_destroy(&pn->l2tp_tunnel_idr);
}

static struct pernet_operations l2tp_net_ops = {
	.init = l2tp_init_net,
	.exit = l2tp_exit_net,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.id   = &l2tp_net_id,
	.size = sizeof(struct l2tp_net),
};

static int __init l2tp_init(void)
{
	int rc = 0;

	rc = register_pernet_device(&l2tp_net_ops);
	if (rc)
		goto out;

<<<<<<< HEAD
	printk(KERN_INFO "L2TP core driver, %s\n", L2TP_DRV_VERSION);
=======
	l2tp_wq = alloc_workqueue("l2tp", WQ_UNBOUND, 0);
	if (!l2tp_wq) {
		pr_err("alloc_workqueue failed\n");
		unregister_pernet_device(&l2tp_net_ops);
		rc = -ENOMEM;
		goto out;
	}

	pr_info("L2TP core driver, %s\n", L2TP_DRV_VERSION);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

out:
	return rc;
}

static void __exit l2tp_exit(void)
{
	unregister_pernet_device(&l2tp_net_ops);
<<<<<<< HEAD
=======
	if (l2tp_wq) {
		destroy_workqueue(l2tp_wq);
		l2tp_wq = NULL;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

module_init(l2tp_init);
module_exit(l2tp_exit);

MODULE_AUTHOR("James Chapman <jchapman@katalix.com>");
MODULE_DESCRIPTION("L2TP core");
MODULE_LICENSE("GPL");
MODULE_VERSION(L2TP_DRV_VERSION);
<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
