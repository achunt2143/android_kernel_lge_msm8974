<<<<<<< HEAD
/*
 * L2TP internal definitions.
 *
 * Copyright (c) 2008,2009 Katalix Systems Ltd
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/* L2TP internal definitions.
 *
 * Copyright (c) 2008,2009 Katalix Systems Ltd
 */
#include <linux/refcount.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifndef _L2TP_CORE_H_
#define _L2TP_CORE_H_

<<<<<<< HEAD
/* Just some random numbers */
#define L2TP_TUNNEL_MAGIC	0x42114DDA
#define L2TP_SESSION_MAGIC	0x0C04EB7D

/* Per tunnel, session hash table size */
#define L2TP_HASH_BITS	4
#define L2TP_HASH_SIZE	(1 << L2TP_HASH_BITS)

/* System-wide, session hash table size */
#define L2TP_HASH_BITS_2	8
#define L2TP_HASH_SIZE_2	(1 << L2TP_HASH_BITS_2)

/* Debug message categories for the DEBUG socket option */
enum {
	L2TP_MSG_DEBUG		= (1 << 0),	/* verbose debug (if
						 * compiled in) */
	L2TP_MSG_CONTROL	= (1 << 1),	/* userspace - kernel
						 * interface */
	L2TP_MSG_SEQ		= (1 << 2),	/* sequence numbers */
	L2TP_MSG_DATA		= (1 << 3),	/* data packets */
};
=======
#include <net/dst.h>
#include <net/sock.h>

#ifdef CONFIG_XFRM
#include <net/xfrm.h>
#endif

/* Random numbers used for internal consistency checks of tunnel and session structures */
#define L2TP_TUNNEL_MAGIC	0x42114DDA
#define L2TP_SESSION_MAGIC	0x0C04EB7D

/* Per tunnel session hash table size */
#define L2TP_HASH_BITS	4
#define L2TP_HASH_SIZE	BIT(L2TP_HASH_BITS)

/* System-wide session hash table size */
#define L2TP_HASH_BITS_2	8
#define L2TP_HASH_SIZE_2	BIT(L2TP_HASH_BITS_2)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct sk_buff;

struct l2tp_stats {
<<<<<<< HEAD
	u64			tx_packets;
	u64			tx_bytes;
	u64			tx_errors;
	u64			rx_packets;
	u64			rx_bytes;
	u64			rx_seq_discards;
	u64			rx_oos_packets;
	u64			rx_errors;
	u64			rx_cookie_discards;
=======
	atomic_long_t		tx_packets;
	atomic_long_t		tx_bytes;
	atomic_long_t		tx_errors;
	atomic_long_t		rx_packets;
	atomic_long_t		rx_bytes;
	atomic_long_t		rx_seq_discards;
	atomic_long_t		rx_oos_packets;
	atomic_long_t		rx_errors;
	atomic_long_t		rx_cookie_discards;
	atomic_long_t		rx_invalid;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct l2tp_tunnel;

<<<<<<< HEAD
/* Describes a session. Contains information to determine incoming
 * packets and transmit outgoing ones.
 */
struct l2tp_session_cfg {
	enum l2tp_pwtype	pw_type;
	unsigned		data_seq:2;	/* data sequencing level
						 * 0 => none, 1 => IP only,
						 * 2 => all
						 */
	unsigned		recv_seq:1;	/* expect receive packets with
						 * sequence numbers? */
	unsigned		send_seq:1;	/* send packets with sequence
						 * numbers? */
	unsigned		lns_mode:1;	/* behave as LNS? LAC enables
						 * sequence numbers under
						 * control of LNS. */
	int			debug;		/* bitmask of debug message
						 * categories */
	u16			vlan_id;	/* VLAN pseudowire only */
	u16			offset;		/* offset to payload */
	u16			l2specific_len;	/* Layer 2 specific length */
=======
/* L2TP session configuration */
struct l2tp_session_cfg {
	enum l2tp_pwtype	pw_type;
	unsigned int		recv_seq:1;	/* expect receive packets with sequence numbers? */
	unsigned int		send_seq:1;	/* send packets with sequence numbers? */
	unsigned int		lns_mode:1;	/* behave as LNS?
						 * LAC enables sequence numbers under LNS control.
						 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u16			l2specific_type; /* Layer 2 specific type */
	u8			cookie[8];	/* optional cookie */
	int			cookie_len;	/* 0, 4 or 8 bytes */
	u8			peer_cookie[8];	/* peer's cookie */
	int			peer_cookie_len; /* 0, 4 or 8 bytes */
<<<<<<< HEAD
	int			reorder_timeout; /* configured reorder timeout
						  * (in jiffies) */
	int			mtu;
	int			mru;
	char			*ifname;
};

struct l2tp_session {
	int			magic;		/* should be
						 * L2TP_SESSION_MAGIC */

	struct l2tp_tunnel	*tunnel;	/* back pointer to tunnel
						 * context */
=======
	int			reorder_timeout; /* configured reorder timeout (in jiffies) */
	char			*ifname;
};

/* Represents a session (pseudowire) instance.
 * Tracks runtime state including cookies, dataplane packet sequencing, and IO statistics.
 * Is linked into a per-tunnel session hashlist; and in the case of an L2TPv3 session into
 * an additional per-net ("global") hashlist.
 */
#define L2TP_SESSION_NAME_MAX 32
struct l2tp_session {
	int			magic;		/* should be L2TP_SESSION_MAGIC */
	long			dead;

	struct l2tp_tunnel	*tunnel;	/* back pointer to tunnel context */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u32			session_id;
	u32			peer_session_id;
	u8			cookie[8];
	int			cookie_len;
	u8			peer_cookie[8];
	int			peer_cookie_len;
<<<<<<< HEAD
	u16			offset;		/* offset from end of L2TP header
						   to beginning of data */
	u16			l2specific_len;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u16			l2specific_type;
	u16			hdr_len;
	u32			nr;		/* session NR state (receive) */
	u32			ns;		/* session NR state (send) */
	struct sk_buff_head	reorder_q;	/* receive reorder queue */
<<<<<<< HEAD
	struct hlist_node	hlist;		/* Hash list node */
	atomic_t		ref_count;

	char			name[32];	/* for logging */
	char			ifname[IFNAMSIZ];
	unsigned		data_seq:2;	/* data sequencing level
						 * 0 => none, 1 => IP only,
						 * 2 => all
						 */
	unsigned		recv_seq:1;	/* expect receive packets with
						 * sequence numbers? */
	unsigned		send_seq:1;	/* send packets with sequence
						 * numbers? */
	unsigned		lns_mode:1;	/* behave as LNS? LAC enables
						 * sequence numbers under
						 * control of LNS. */
	int			debug;		/* bitmask of debug message
						 * categories */
	int			reorder_timeout; /* configured reorder timeout
						  * (in jiffies) */
	int			mtu;
	int			mru;
	enum l2tp_pwtype	pwtype;
	struct l2tp_stats	stats;
	struct hlist_node	global_hlist;	/* Global hash list node */

	int (*build_header)(struct l2tp_session *session, void *buf);
	void (*recv_skb)(struct l2tp_session *session, struct sk_buff *skb, int data_len);
	void (*session_close)(struct l2tp_session *session);
	void (*ref)(struct l2tp_session *session);
	void (*deref)(struct l2tp_session *session);
#if defined(CONFIG_L2TP_DEBUGFS) || defined(CONFIG_L2TP_DEBUGFS_MODULE)
	void (*show)(struct seq_file *m, void *priv);
#endif
	uint8_t			priv[0];	/* private data */
};

/* Describes the tunnel. It contains info to track all the associated
 * sessions so incoming packets can be sorted out
 */
struct l2tp_tunnel_cfg {
	int			debug;		/* bitmask of debug message
						 * categories */
=======
	u32			nr_max;		/* max NR. Depends on tunnel */
	u32			nr_window_size;	/* NR window size */
	u32			nr_oos;		/* NR of last OOS packet */
	int			nr_oos_count;	/* for OOS recovery */
	int			nr_oos_count_max;
	struct hlist_node	hlist;		/* hash list node */
	refcount_t		ref_count;

	char			name[L2TP_SESSION_NAME_MAX]; /* for logging */
	char			ifname[IFNAMSIZ];
	unsigned int		recv_seq:1;	/* expect receive packets with sequence numbers? */
	unsigned int		send_seq:1;	/* send packets with sequence numbers? */
	unsigned int		lns_mode:1;	/* behave as LNS?
						 * LAC enables sequence numbers under LNS control.
						 */
	int			reorder_timeout; /* configured reorder timeout (in jiffies) */
	int			reorder_skip;	/* set if skip to next nr */
	enum l2tp_pwtype	pwtype;
	struct l2tp_stats	stats;
	struct hlist_node	global_hlist;	/* global hash list node */

	/* Session receive handler for data packets.
	 * Each pseudowire implementation should implement this callback in order to
	 * handle incoming packets.  Packets are passed to the pseudowire handler after
	 * reordering, if data sequence numbers are enabled for the session.
	 */
	void (*recv_skb)(struct l2tp_session *session, struct sk_buff *skb, int data_len);

	/* Session close handler.
	 * Each pseudowire implementation may implement this callback in order to carry
	 * out pseudowire-specific shutdown actions.
	 * The callback is called by core after unhashing the session and purging its
	 * reorder queue.
	 */
	void (*session_close)(struct l2tp_session *session);

	/* Session show handler.
	 * Pseudowire-specific implementation of debugfs session rendering.
	 * The callback is called by l2tp_debugfs.c after rendering core session
	 * information.
	 */
	void (*show)(struct seq_file *m, void *priv);

	u8			priv[];		/* private data */
};

/* L2TP tunnel configuration */
struct l2tp_tunnel_cfg {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	enum l2tp_encap_type	encap;

	/* Used only for kernel-created sockets */
	struct in_addr		local_ip;
	struct in_addr		peer_ip;
<<<<<<< HEAD
	u16			local_udp_port;
	u16			peer_udp_port;
	unsigned int		use_udp_checksums:1;
};

struct l2tp_tunnel {
	int			magic;		/* Should be L2TP_TUNNEL_MAGIC */
	struct rcu_head rcu;
	rwlock_t		hlist_lock;	/* protect session_hlist */
	bool			acpt_newsess;	/* Indicates whether this
						 * tunnel accepts new sessions.
						 * Protected by hlist_lock.
						 */
	struct hlist_head	session_hlist[L2TP_HASH_SIZE];
						/* hashed list of sessions,
						 * hashed by id */
=======
#if IS_ENABLED(CONFIG_IPV6)
	struct in6_addr		*local_ip6;
	struct in6_addr		*peer_ip6;
#endif
	u16			local_udp_port;
	u16			peer_udp_port;
	unsigned int		use_udp_checksums:1,
				udp6_zero_tx_checksums:1,
				udp6_zero_rx_checksums:1;
};

/* Represents a tunnel instance.
 * Tracks runtime state including IO statistics.
 * Holds the tunnel socket (either passed from userspace or directly created by the kernel).
 * Maintains a hashlist of sessions belonging to the tunnel instance.
 * Is linked into a per-net list of tunnels.
 */
#define L2TP_TUNNEL_NAME_MAX 20
struct l2tp_tunnel {
	int			magic;		/* Should be L2TP_TUNNEL_MAGIC */

	unsigned long		dead;

	struct rcu_head rcu;
	spinlock_t		hlist_lock;	/* write-protection for session_hlist */
	bool			acpt_newsess;	/* indicates whether this tunnel accepts
						 * new sessions. Protected by hlist_lock.
						 */
	struct hlist_head	session_hlist[L2TP_HASH_SIZE];
						/* hashed list of sessions, hashed by id */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u32			tunnel_id;
	u32			peer_tunnel_id;
	int			version;	/* 2=>L2TPv2, 3=>L2TPv3 */

<<<<<<< HEAD
	char			name[20];	/* for logging */
	int			debug;		/* bitmask of debug message
						 * categories */
	enum l2tp_encap_type	encap;
	struct l2tp_stats	stats;

	struct list_head	list;		/* Keep a list of all tunnels */
	struct net		*l2tp_net;	/* the net we belong to */

	atomic_t		ref_count;
#ifdef CONFIG_DEBUG_FS
	void (*show)(struct seq_file *m, void *arg);
#endif
	int (*recv_payload_hook)(struct sk_buff *skb);
	void (*old_sk_destruct)(struct sock *);
	struct sock		*sock;		/* Parent socket */
	int			fd;

	uint8_t			priv[0];	/* private data */
};

struct l2tp_nl_cmd_ops {
	int (*session_create)(struct net *net, struct l2tp_tunnel *tunnel,
			      u32 session_id, u32 peer_session_id,
			      struct l2tp_session_cfg *cfg);
	int (*session_delete)(struct l2tp_session *session);
};

static inline void *l2tp_tunnel_priv(struct l2tp_tunnel *tunnel)
{
	return &tunnel->priv[0];
}
=======
	char			name[L2TP_TUNNEL_NAME_MAX]; /* for logging */
	enum l2tp_encap_type	encap;
	struct l2tp_stats	stats;

	struct list_head	list;		/* list node on per-namespace list of tunnels */
	struct net		*l2tp_net;	/* the net we belong to */

	refcount_t		ref_count;
	void (*old_sk_destruct)(struct sock *sk);
	struct sock		*sock;		/* parent socket */
	int			fd;		/* parent fd, if tunnel socket was created
						 * by userspace
						 */

	struct work_struct	del_work;
};

/* Pseudowire ops callbacks for use with the l2tp genetlink interface */
struct l2tp_nl_cmd_ops {
	/* The pseudowire session create callback is responsible for creating a session
	 * instance for a specific pseudowire type.
	 * It must call l2tp_session_create and l2tp_session_register to register the
	 * session instance, as well as carry out any pseudowire-specific initialisation.
	 * It must return >= 0 on success, or an appropriate negative errno value on failure.
	 */
	int (*session_create)(struct net *net, struct l2tp_tunnel *tunnel,
			      u32 session_id, u32 peer_session_id,
			      struct l2tp_session_cfg *cfg);

	/* The pseudowire session delete callback is responsible for initiating the deletion
	 * of a session instance.
	 * It must call l2tp_session_delete, as well as carry out any pseudowire-specific
	 * teardown actions.
	 */
	void (*session_delete)(struct l2tp_session *session);
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline void *l2tp_session_priv(struct l2tp_session *session)
{
	return &session->priv[0];
}

<<<<<<< HEAD
static inline struct l2tp_tunnel *l2tp_sock_to_tunnel(struct sock *sk)
{
	struct l2tp_tunnel *tunnel;

	if (sk == NULL)
		return NULL;

	sock_hold(sk);
	tunnel = (struct l2tp_tunnel *)(sk->sk_user_data);
	if (tunnel == NULL) {
		sock_put(sk);
		goto out;
	}

	BUG_ON(tunnel->magic != L2TP_TUNNEL_MAGIC);

out:
	return tunnel;
}

struct l2tp_session *l2tp_session_get(struct net *net,
				      struct l2tp_tunnel *tunnel,
				      u32 session_id, bool do_ref);
extern struct l2tp_session *l2tp_session_find(struct net *net, struct l2tp_tunnel *tunnel, u32 session_id);
extern struct l2tp_session *l2tp_session_find_nth(struct l2tp_tunnel *tunnel, int nth);
extern struct l2tp_session *l2tp_session_find_by_ifname(struct net *net, char *ifname);
extern struct l2tp_tunnel *l2tp_tunnel_find(struct net *net, u32 tunnel_id);
extern struct l2tp_tunnel *l2tp_tunnel_find_nth(struct net *net, int nth);

extern int l2tp_tunnel_create(struct net *net, int fd, int version, u32 tunnel_id, u32 peer_tunnel_id, struct l2tp_tunnel_cfg *cfg, struct l2tp_tunnel **tunnelp);
extern int l2tp_tunnel_delete(struct l2tp_tunnel *tunnel);
extern struct l2tp_session *l2tp_session_create(int priv_size, struct l2tp_tunnel *tunnel, u32 session_id, u32 peer_session_id, struct l2tp_session_cfg *cfg);
extern int l2tp_session_delete(struct l2tp_session *session);
extern void l2tp_session_free(struct l2tp_session *session);
extern void l2tp_recv_common(struct l2tp_session *session, struct sk_buff *skb, unsigned char *ptr, unsigned char *optr, u16 hdrflags, int length, int (*payload_hook)(struct sk_buff *skb));
extern int l2tp_udp_encap_recv(struct sock *sk, struct sk_buff *skb);

extern int l2tp_xmit_skb(struct l2tp_session *session, struct sk_buff *skb, int hdr_len);

extern int l2tp_nl_register_ops(enum l2tp_pwtype pw_type, const struct l2tp_nl_cmd_ops *ops);
extern void l2tp_nl_unregister_ops(enum l2tp_pwtype pw_type);

/* Session reference counts. Incremented when code obtains a reference
 * to a session.
 */
static inline void l2tp_session_inc_refcount_1(struct l2tp_session *session)
{
	atomic_inc(&session->ref_count);
}

static inline void l2tp_session_dec_refcount_1(struct l2tp_session *session)
{
	if (atomic_dec_and_test(&session->ref_count))
		l2tp_session_free(session);
}

#ifdef L2TP_REFCNT_DEBUG
#define l2tp_session_inc_refcount(_s) do { \
		printk(KERN_DEBUG "l2tp_session_inc_refcount: %s:%d %s: cnt=%d\n", __func__, __LINE__, (_s)->name, atomic_read(&_s->ref_count)); \
		l2tp_session_inc_refcount_1(_s);				\
	} while (0)
#define l2tp_session_dec_refcount(_s) do { \
		printk(KERN_DEBUG "l2tp_session_dec_refcount: %s:%d %s: cnt=%d\n", __func__, __LINE__, (_s)->name, atomic_read(&_s->ref_count)); \
		l2tp_session_dec_refcount_1(_s);				\
	} while (0)
#else
#define l2tp_session_inc_refcount(s) l2tp_session_inc_refcount_1(s)
#define l2tp_session_dec_refcount(s) l2tp_session_dec_refcount_1(s)
#endif

=======
/* Tunnel and session refcounts */
void l2tp_tunnel_inc_refcount(struct l2tp_tunnel *tunnel);
void l2tp_tunnel_dec_refcount(struct l2tp_tunnel *tunnel);
void l2tp_session_inc_refcount(struct l2tp_session *session);
void l2tp_session_dec_refcount(struct l2tp_session *session);

/* Tunnel and session lookup.
 * These functions take a reference on the instances they return, so
 * the caller must ensure that the reference is dropped appropriately.
 */
struct l2tp_tunnel *l2tp_tunnel_get(const struct net *net, u32 tunnel_id);
struct l2tp_tunnel *l2tp_tunnel_get_nth(const struct net *net, int nth);
struct l2tp_session *l2tp_tunnel_get_session(struct l2tp_tunnel *tunnel,
					     u32 session_id);

struct l2tp_session *l2tp_session_get(const struct net *net, u32 session_id);
struct l2tp_session *l2tp_session_get_nth(struct l2tp_tunnel *tunnel, int nth);
struct l2tp_session *l2tp_session_get_by_ifname(const struct net *net,
						const char *ifname);

/* Tunnel and session lifetime management.
 * Creation of a new instance is a two-step process: create, then register.
 * Destruction is triggered using the *_delete functions, and completes asynchronously.
 */
int l2tp_tunnel_create(int fd, int version, u32 tunnel_id,
		       u32 peer_tunnel_id, struct l2tp_tunnel_cfg *cfg,
		       struct l2tp_tunnel **tunnelp);
int l2tp_tunnel_register(struct l2tp_tunnel *tunnel, struct net *net,
			 struct l2tp_tunnel_cfg *cfg);
void l2tp_tunnel_delete(struct l2tp_tunnel *tunnel);

struct l2tp_session *l2tp_session_create(int priv_size,
					 struct l2tp_tunnel *tunnel,
					 u32 session_id, u32 peer_session_id,
					 struct l2tp_session_cfg *cfg);
int l2tp_session_register(struct l2tp_session *session,
			  struct l2tp_tunnel *tunnel);
void l2tp_session_delete(struct l2tp_session *session);

/* Receive path helpers.  If data sequencing is enabled for the session these
 * functions handle queuing and reordering prior to passing packets to the
 * pseudowire code to be passed to userspace.
 */
void l2tp_recv_common(struct l2tp_session *session, struct sk_buff *skb,
		      unsigned char *ptr, unsigned char *optr, u16 hdrflags,
		      int length);
int l2tp_udp_encap_recv(struct sock *sk, struct sk_buff *skb);

/* Transmit path helpers for sending packets over the tunnel socket. */
void l2tp_session_set_header_len(struct l2tp_session *session, int version);
int l2tp_xmit_skb(struct l2tp_session *session, struct sk_buff *skb);

/* Pseudowire management.
 * Pseudowires should register with l2tp core on module init, and unregister
 * on module exit.
 */
int l2tp_nl_register_ops(enum l2tp_pwtype pw_type, const struct l2tp_nl_cmd_ops *ops);
void l2tp_nl_unregister_ops(enum l2tp_pwtype pw_type);

/* IOCTL helper for IP encap modules. */
int l2tp_ioctl(struct sock *sk, int cmd, int *karg);

/* Extract the tunnel structure from a socket's sk_user_data pointer,
 * validating the tunnel magic feather.
 */
struct l2tp_tunnel *l2tp_sk_to_tunnel(struct sock *sk);

static inline int l2tp_get_l2specific_len(struct l2tp_session *session)
{
	switch (session->l2specific_type) {
	case L2TP_L2SPECTYPE_DEFAULT:
		return 4;
	case L2TP_L2SPECTYPE_NONE:
	default:
		return 0;
	}
}

static inline u32 l2tp_tunnel_dst_mtu(const struct l2tp_tunnel *tunnel)
{
	struct dst_entry *dst;
	u32 mtu;

	dst = sk_dst_get(tunnel->sock);
	if (!dst)
		return 0;

	mtu = dst_mtu(dst);
	dst_release(dst);

	return mtu;
}

#ifdef CONFIG_XFRM
static inline bool l2tp_tunnel_uses_xfrm(const struct l2tp_tunnel *tunnel)
{
	struct sock *sk = tunnel->sock;

	return sk && (rcu_access_pointer(sk->sk_policy[0]) ||
		      rcu_access_pointer(sk->sk_policy[1]));
}
#else
static inline bool l2tp_tunnel_uses_xfrm(const struct l2tp_tunnel *tunnel)
{
	return false;
}
#endif

static inline int l2tp_v3_ensure_opt_in_linear(struct l2tp_session *session, struct sk_buff *skb,
					       unsigned char **ptr, unsigned char **optr)
{
	int opt_len = session->peer_cookie_len + l2tp_get_l2specific_len(session);

	if (opt_len > 0) {
		int off = *ptr - *optr;

		if (!pskb_may_pull(skb, off + opt_len))
			return -1;

		if (skb->data != *optr) {
			*optr = skb->data;
			*ptr = skb->data + off;
		}
	}

	return 0;
}

#define MODULE_ALIAS_L2TP_PWTYPE(type) \
	MODULE_ALIAS("net-l2tp-type-" __stringify(type))

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _L2TP_CORE_H_ */
