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

#ifndef _INET_HASHTABLES_H
#define _INET_HASHTABLES_H


#include <linux/interrupt.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/socket.h>
#include <linux/spinlock.h>
#include <linux/types.h>
#include <linux/wait.h>
<<<<<<< HEAD
#include <linux/vmalloc.h>

#include <net/inet_connection_sock.h>
#include <net/inet_sock.h>
=======

#include <net/inet_connection_sock.h>
#include <net/inet_sock.h>
#include <net/ip.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <net/sock.h>
#include <net/route.h>
#include <net/tcp_states.h>
#include <net/netns/hash.h>

<<<<<<< HEAD
#include <linux/atomic.h>
#include <asm/byteorder.h>

/* This is for all connections with a full identity, no wildcards.
 * One chain is dedicated to TIME_WAIT sockets.
 * I'll experiment with dynamic table growth later.
 */
struct inet_ehash_bucket {
	struct hlist_nulls_head chain;
	struct hlist_nulls_head twchain;
=======
#include <linux/refcount.h>
#include <asm/byteorder.h>

/* This is for all connections with a full identity, no wildcards.
 * The 'e' prefix stands for Establish, but we really put all sockets
 * but LISTEN ones.
 */
struct inet_ehash_bucket {
	struct hlist_nulls_head chain;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/* There are a few simple rules, which allow for local port reuse by
 * an application.  In essence:
 *
 *	1) Sockets bound to different interfaces may share a local port.
 *	   Failing that, goto test 2.
 *	2) If all sockets have sk->sk_reuse set, and none of them are in
 *	   TCP_LISTEN state, the port may be shared.
 *	   Failing that, goto test 3.
 *	3) If all sockets are bound to a specific inet_sk(sk)->rcv_saddr local
 *	   address, and none of them are the same, the port may be
 *	   shared.
 *	   Failing this, the port cannot be shared.
 *
 * The interesting point, is test #2.  This is what an FTP server does
 * all day.  To optimize this case we use a specific flag bit defined
 * below.  As we add sockets to a bind bucket list, we perform a
 * check of: (newsk->sk_reuse && (newsk->sk_state != TCP_LISTEN))
 * As long as all sockets added to a bind bucket pass this test,
 * the flag bit will be set.
 * The resulting situation is that tcp_v[46]_verify_bind() can just check
 * for this flag bit, if it is set and the socket trying to bind has
 * sk->sk_reuse set, we don't even have to walk the owners list at all,
 * we return that it is ok to bind this socket to the requested local port.
 *
 * Sounds like a lot of work, but it is worth it.  In a more naive
 * implementation (ie. current FreeBSD etc.) the entire list of ports
 * must be walked for each data port opened by an ftp server.  Needless
 * to say, this does not scale at all.  With a couple thousand FTP
 * users logged onto your box, isn't it nice to know that new data
 * ports are created in O(1) time?  I thought so. ;-)	-DaveM
 */
<<<<<<< HEAD
struct inet_bind_bucket {
#ifdef CONFIG_NET_NS
	struct net		*ib_net;
#endif
	unsigned short		port;
	signed short		fastreuse;
	int			num_owners;
	struct hlist_node	node;
	struct hlist_head	owners;
};

static inline struct net *ib_net(struct inet_bind_bucket *ib)
=======
#define FASTREUSEPORT_ANY	1
#define FASTREUSEPORT_STRICT	2

struct inet_bind_bucket {
	possible_net_t		ib_net;
	int			l3mdev;
	unsigned short		port;
	signed char		fastreuse;
	signed char		fastreuseport;
	kuid_t			fastuid;
#if IS_ENABLED(CONFIG_IPV6)
	struct in6_addr		fast_v6_rcv_saddr;
#endif
	__be32			fast_rcv_saddr;
	unsigned short		fast_sk_family;
	bool			fast_ipv6_only;
	struct hlist_node	node;
	struct hlist_head	bhash2;
};

struct inet_bind2_bucket {
	possible_net_t		ib_net;
	int			l3mdev;
	unsigned short		port;
#if IS_ENABLED(CONFIG_IPV6)
	unsigned short		addr_type;
	struct in6_addr		v6_rcv_saddr;
#define rcv_saddr		v6_rcv_saddr.s6_addr32[3]
#else
	__be32			rcv_saddr;
#endif
	/* Node in the bhash2 inet_bind_hashbucket chain */
	struct hlist_node	node;
	struct hlist_node	bhash_node;
	/* List of sockets hashed to this bucket */
	struct hlist_head	owners;
};

static inline struct net *ib_net(const struct inet_bind_bucket *ib)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return read_pnet(&ib->ib_net);
}

<<<<<<< HEAD
#define inet_bind_bucket_for_each(tb, pos, head) \
	hlist_for_each_entry(tb, pos, head, node)
=======
static inline struct net *ib2_net(const struct inet_bind2_bucket *ib)
{
	return read_pnet(&ib->ib_net);
}

#define inet_bind_bucket_for_each(tb, head) \
	hlist_for_each_entry(tb, head, node)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct inet_bind_hashbucket {
	spinlock_t		lock;
	struct hlist_head	chain;
};

<<<<<<< HEAD
/*
 * Sockets can be hashed in established or listening table
 * We must use different 'nulls' end-of-chain value for listening
 * hash table, or we might find a socket that was closed and
 * reallocated/inserted into established hash table
=======
/* Sockets can be hashed in established or listening table.
 * We must use different 'nulls' end-of-chain value for all hash buckets :
 * A socket might transition from ESTABLISH to LISTEN state without
 * RCU grace period. A lookup in ehash table needs to handle this case.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#define LISTENING_NULLS_BASE (1U << 29)
struct inet_listen_hashbucket {
	spinlock_t		lock;
<<<<<<< HEAD
	struct hlist_nulls_head	head;
=======
	struct hlist_nulls_head	nulls_head;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/* This is for listening sockets, thus all sockets which possess wildcards. */
#define INET_LHTABLE_SIZE	32	/* Yes, really, this is all you need. */

struct inet_hashinfo {
	/* This is for sockets with full identity only.  Sockets here will
	 * always be without wildcards and will have the following invariant:
	 *
	 *          TCP_ESTABLISHED <= sk->sk_state < TCP_CLOSE
	 *
<<<<<<< HEAD
	 * TIME_WAIT sockets use a separate chain (twchain).
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 */
	struct inet_ehash_bucket	*ehash;
	spinlock_t			*ehash_locks;
	unsigned int			ehash_mask;
	unsigned int			ehash_locks_mask;

	/* Ok, let's try this, I give up, we do need a local binding
	 * TCP hash as well as the others for fast bind/connect.
	 */
<<<<<<< HEAD
	struct inet_bind_hashbucket	*bhash;

	unsigned int			bhash_size;
	/* 4 bytes hole on 64 bit */

	struct kmem_cache		*bind_bucket_cachep;

	/* All the above members are written once at bootup and
	 * never written again _or_ are predominantly read-access.
	 *
	 * Now align to a new cache line as all the following members
	 * might be often dirty.
	 */
	/* All sockets in TCP_LISTEN state will be in here.  This is the only
	 * table where wildcard'd TCP sockets can exist.  Hash function here
	 * is just local port number.
	 */
	struct inet_listen_hashbucket	listening_hash[INET_LHTABLE_SIZE]
					____cacheline_aligned_in_smp;

	atomic_t			bsockets;
};
=======
	struct kmem_cache		*bind_bucket_cachep;
	/* This bind table is hashed by local port */
	struct inet_bind_hashbucket	*bhash;
	struct kmem_cache		*bind2_bucket_cachep;
	/* This bind table is hashed by local port and sk->sk_rcv_saddr (ipv4)
	 * or sk->sk_v6_rcv_saddr (ipv6). This 2nd bind table is used
	 * primarily for expediting bind conflict resolution.
	 */
	struct inet_bind_hashbucket	*bhash2;
	unsigned int			bhash_size;

	/* The 2nd listener table hashed by local port and address */
	unsigned int			lhash2_mask;
	struct inet_listen_hashbucket	*lhash2;

	bool				pernet;
} ____cacheline_aligned_in_smp;

static inline struct inet_hashinfo *tcp_or_dccp_get_hashinfo(const struct sock *sk)
{
#if IS_ENABLED(CONFIG_IP_DCCP)
	return sk->sk_prot->h.hashinfo ? :
		sock_net(sk)->ipv4.tcp_death_row.hashinfo;
#else
	return sock_net(sk)->ipv4.tcp_death_row.hashinfo;
#endif
}

static inline struct inet_listen_hashbucket *
inet_lhash2_bucket(struct inet_hashinfo *h, u32 hash)
{
	return &h->lhash2[hash & h->lhash2_mask];
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline struct inet_ehash_bucket *inet_ehash_bucket(
	struct inet_hashinfo *hashinfo,
	unsigned int hash)
{
	return &hashinfo->ehash[hash & hashinfo->ehash_mask];
}

static inline spinlock_t *inet_ehash_lockp(
	struct inet_hashinfo *hashinfo,
	unsigned int hash)
{
	return &hashinfo->ehash_locks[hash & hashinfo->ehash_locks_mask];
}

<<<<<<< HEAD
static inline int inet_ehash_locks_alloc(struct inet_hashinfo *hashinfo)
{
	unsigned int i, size = 256;
#if defined(CONFIG_PROVE_LOCKING)
	unsigned int nr_pcpus = 2;
#else
	unsigned int nr_pcpus = num_possible_cpus();
#endif
	if (nr_pcpus >= 4)
		size = 512;
	if (nr_pcpus >= 8)
		size = 1024;
	if (nr_pcpus >= 16)
		size = 2048;
	if (nr_pcpus >= 32)
		size = 4096;
	if (sizeof(spinlock_t) != 0) {
#ifdef CONFIG_NUMA
		if (size * sizeof(spinlock_t) > PAGE_SIZE)
			hashinfo->ehash_locks = vmalloc(size * sizeof(spinlock_t));
		else
#endif
		hashinfo->ehash_locks =	kmalloc(size * sizeof(spinlock_t),
						GFP_KERNEL);
		if (!hashinfo->ehash_locks)
			return ENOMEM;
		for (i = 0; i < size; i++)
			spin_lock_init(&hashinfo->ehash_locks[i]);
	}
	hashinfo->ehash_locks_mask = size - 1;
	return 0;
=======
int inet_ehash_locks_alloc(struct inet_hashinfo *hashinfo);

static inline void inet_hashinfo2_free_mod(struct inet_hashinfo *h)
{
	kfree(h->lhash2);
	h->lhash2 = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void inet_ehash_locks_free(struct inet_hashinfo *hashinfo)
{
<<<<<<< HEAD
	if (hashinfo->ehash_locks) {
#ifdef CONFIG_NUMA
		unsigned int size = (hashinfo->ehash_locks_mask + 1) *
							sizeof(spinlock_t);
		if (size > PAGE_SIZE)
			vfree(hashinfo->ehash_locks);
		else
#endif
		kfree(hashinfo->ehash_locks);
		hashinfo->ehash_locks = NULL;
	}
}

extern struct inet_bind_bucket *
		    inet_bind_bucket_create(struct kmem_cache *cachep,
					    struct net *net,
					    struct inet_bind_hashbucket *head,
					    const unsigned short snum);
extern void inet_bind_bucket_destroy(struct kmem_cache *cachep,
				     struct inet_bind_bucket *tb);

static inline int inet_bhashfn(struct net *net,
		const __u16 lport, const int bhash_size)
=======
	kvfree(hashinfo->ehash_locks);
	hashinfo->ehash_locks = NULL;
}

struct inet_hashinfo *inet_pernet_hashinfo_alloc(struct inet_hashinfo *hashinfo,
						 unsigned int ehash_entries);
void inet_pernet_hashinfo_free(struct inet_hashinfo *hashinfo);

struct inet_bind_bucket *
inet_bind_bucket_create(struct kmem_cache *cachep, struct net *net,
			struct inet_bind_hashbucket *head,
			const unsigned short snum, int l3mdev);
void inet_bind_bucket_destroy(struct kmem_cache *cachep,
			      struct inet_bind_bucket *tb);

bool inet_bind_bucket_match(const struct inet_bind_bucket *tb,
			    const struct net *net, unsigned short port,
			    int l3mdev);

struct inet_bind2_bucket *
inet_bind2_bucket_create(struct kmem_cache *cachep, struct net *net,
			 struct inet_bind_hashbucket *head,
			 struct inet_bind_bucket *tb,
			 const struct sock *sk);

void inet_bind2_bucket_destroy(struct kmem_cache *cachep,
			       struct inet_bind2_bucket *tb);

struct inet_bind2_bucket *
inet_bind2_bucket_find(const struct inet_bind_hashbucket *head,
		       const struct net *net,
		       unsigned short port, int l3mdev,
		       const struct sock *sk);

bool inet_bind2_bucket_match_addr_any(const struct inet_bind2_bucket *tb,
				      const struct net *net, unsigned short port,
				      int l3mdev, const struct sock *sk);

static inline u32 inet_bhashfn(const struct net *net, const __u16 lport,
			       const u32 bhash_size)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return (lport + net_hash_mix(net)) & (bhash_size - 1);
}

<<<<<<< HEAD
extern void inet_bind_hash(struct sock *sk, struct inet_bind_bucket *tb,
			   const unsigned short snum);

/* These can have wildcards, don't try too hard. */
static inline int inet_lhashfn(struct net *net, const unsigned short num)
{
	return (num + net_hash_mix(net)) & (INET_LHTABLE_SIZE - 1);
}

static inline int inet_sk_listen_hashfn(const struct sock *sk)
{
	return inet_lhashfn(sock_net(sk), inet_sk(sk)->inet_num);
}

/* Caller must disable local BH processing. */
extern int __inet_inherit_port(struct sock *sk, struct sock *child);

extern void inet_put_port(struct sock *sk);

void inet_hashinfo_init(struct inet_hashinfo *h);

extern int __inet_hash_nolisten(struct sock *sk, struct inet_timewait_sock *tw);
extern void inet_hash(struct sock *sk);
extern void inet_unhash(struct sock *sk);

extern struct sock *__inet_lookup_listener(struct net *net,
					   struct inet_hashinfo *hashinfo,
					   const __be32 daddr,
					   const unsigned short hnum,
					   const int dif);

static inline struct sock *inet_lookup_listener(struct net *net,
		struct inet_hashinfo *hashinfo,
		__be32 daddr, __be16 dport, int dif)
{
	return __inet_lookup_listener(net, hashinfo, daddr, ntohs(dport), dif);
=======
static inline struct inet_bind_hashbucket *
inet_bhashfn_portaddr(const struct inet_hashinfo *hinfo, const struct sock *sk,
		      const struct net *net, unsigned short port)
{
	u32 hash;

#if IS_ENABLED(CONFIG_IPV6)
	if (sk->sk_family == AF_INET6)
		hash = ipv6_portaddr_hash(net, &sk->sk_v6_rcv_saddr, port);
	else
#endif
		hash = ipv4_portaddr_hash(net, sk->sk_rcv_saddr, port);
	return &hinfo->bhash2[hash & (hinfo->bhash_size - 1)];
}

struct inet_bind_hashbucket *
inet_bhash2_addr_any_hashbucket(const struct sock *sk, const struct net *net, int port);

/* This should be called whenever a socket's sk_rcv_saddr (ipv4) or
 * sk_v6_rcv_saddr (ipv6) changes after it has been binded. The socket's
 * rcv_saddr field should already have been updated when this is called.
 */
int inet_bhash2_update_saddr(struct sock *sk, void *saddr, int family);
void inet_bhash2_reset_saddr(struct sock *sk);

void inet_bind_hash(struct sock *sk, struct inet_bind_bucket *tb,
		    struct inet_bind2_bucket *tb2, unsigned short port);

/* Caller must disable local BH processing. */
int __inet_inherit_port(const struct sock *sk, struct sock *child);

void inet_put_port(struct sock *sk);

void inet_hashinfo2_init(struct inet_hashinfo *h, const char *name,
			 unsigned long numentries, int scale,
			 unsigned long low_limit,
			 unsigned long high_limit);
int inet_hashinfo2_init_mod(struct inet_hashinfo *h);

bool inet_ehash_insert(struct sock *sk, struct sock *osk, bool *found_dup_sk);
bool inet_ehash_nolisten(struct sock *sk, struct sock *osk,
			 bool *found_dup_sk);
int __inet_hash(struct sock *sk, struct sock *osk);
int inet_hash(struct sock *sk);
void inet_unhash(struct sock *sk);

struct sock *__inet_lookup_listener(struct net *net,
				    struct inet_hashinfo *hashinfo,
				    struct sk_buff *skb, int doff,
				    const __be32 saddr, const __be16 sport,
				    const __be32 daddr,
				    const unsigned short hnum,
				    const int dif, const int sdif);

static inline struct sock *inet_lookup_listener(struct net *net,
		struct inet_hashinfo *hashinfo,
		struct sk_buff *skb, int doff,
		__be32 saddr, __be16 sport,
		__be32 daddr, __be16 dport, int dif, int sdif)
{
	return __inet_lookup_listener(net, hashinfo, skb, doff, saddr, sport,
				      daddr, ntohs(dport), dif, sdif);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Socket demux engine toys. */
/* What happens here is ugly; there's a pair of adjacent fields in
   struct inet_sock; __be16 dport followed by __u16 num.  We want to
   search by pair, so we combine the keys into a single 32bit value
   and compare with 32bit value read from &...->dport.  Let's at least
   make sure that it's not mixed with anything else...
   On 64bit targets we combine comparisons with pair of adjacent __be32
   fields in the same way.
*/
<<<<<<< HEAD
typedef __u32 __bitwise __portpair;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef __BIG_ENDIAN
#define INET_COMBINED_PORTS(__sport, __dport) \
	((__force __portpair)(((__force __u32)(__be16)(__sport) << 16) | (__u32)(__dport)))
#else /* __LITTLE_ENDIAN */
#define INET_COMBINED_PORTS(__sport, __dport) \
	((__force __portpair)(((__u32)(__dport) << 16) | (__force __u32)(__be16)(__sport)))
#endif

<<<<<<< HEAD
#if (BITS_PER_LONG == 64)
typedef __u64 __bitwise __addrpair;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef __BIG_ENDIAN
#define INET_ADDR_COOKIE(__name, __saddr, __daddr) \
	const __addrpair __name = (__force __addrpair) ( \
				   (((__force __u64)(__be32)(__saddr)) << 32) | \
<<<<<<< HEAD
				   ((__force __u64)(__be32)(__daddr)));
=======
				   ((__force __u64)(__be32)(__daddr)))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#else /* __LITTLE_ENDIAN */
#define INET_ADDR_COOKIE(__name, __saddr, __daddr) \
	const __addrpair __name = (__force __addrpair) ( \
				   (((__force __u64)(__be32)(__daddr)) << 32) | \
<<<<<<< HEAD
				   ((__force __u64)(__be32)(__saddr)));
#endif /* __BIG_ENDIAN */
#define INET_MATCH(__sk, __net, __hash, __cookie, __saddr, __daddr, __ports, __dif)\
	(((__sk)->sk_hash == (__hash)) && net_eq(sock_net(__sk), (__net)) &&	\
	 ((*((__addrpair *)&(inet_sk(__sk)->inet_daddr))) == (__cookie))  &&	\
	 ((*((__portpair *)&(inet_sk(__sk)->inet_dport))) == (__ports))   &&	\
	 (!((__sk)->sk_bound_dev_if) || ((__sk)->sk_bound_dev_if == (__dif))))
#define INET_TW_MATCH(__sk, __net, __hash, __cookie, __saddr, __daddr, __ports, __dif)\
	(((__sk)->sk_hash == (__hash)) && net_eq(sock_net(__sk), (__net)) &&	\
	 ((*((__addrpair *)&(inet_twsk(__sk)->tw_daddr))) == (__cookie)) &&	\
	 ((*((__portpair *)&(inet_twsk(__sk)->tw_dport))) == (__ports)) &&	\
	 (!((__sk)->sk_bound_dev_if) || ((__sk)->sk_bound_dev_if == (__dif))))
#else /* 32-bit arch */
#define INET_ADDR_COOKIE(__name, __saddr, __daddr)
#define INET_MATCH(__sk, __net, __hash, __cookie, __saddr, __daddr, __ports, __dif)	\
	(((__sk)->sk_hash == (__hash)) && net_eq(sock_net(__sk), (__net))	&&	\
	 (inet_sk(__sk)->inet_daddr	== (__saddr))		&&	\
	 (inet_sk(__sk)->inet_rcv_saddr	== (__daddr))		&&	\
	 ((*((__portpair *)&(inet_sk(__sk)->inet_dport))) == (__ports))	&&	\
	 (!((__sk)->sk_bound_dev_if) || ((__sk)->sk_bound_dev_if == (__dif))))
#define INET_TW_MATCH(__sk, __net, __hash,__cookie, __saddr, __daddr, __ports, __dif)	\
	(((__sk)->sk_hash == (__hash)) && net_eq(sock_net(__sk), (__net))	&&	\
	 (inet_twsk(__sk)->tw_daddr	== (__saddr))		&&	\
	 (inet_twsk(__sk)->tw_rcv_saddr	== (__daddr))		&&	\
	 ((*((__portpair *)&(inet_twsk(__sk)->tw_dport))) == (__ports)) &&	\
	 (!((__sk)->sk_bound_dev_if) || ((__sk)->sk_bound_dev_if == (__dif))))
#endif /* 64-bit arch */

/*
 * Sockets in TCP_CLOSE state are _always_ taken out of the hash, so we need
 * not check it for lookups anymore, thanks Alexey. -DaveM
 *
 * Local BH must be disabled here.
 */
extern struct sock * __inet_lookup_established(struct net *net,
		struct inet_hashinfo *hashinfo,
		const __be32 saddr, const __be16 sport,
		const __be32 daddr, const u16 hnum, const int dif);
=======
				   ((__force __u64)(__be32)(__saddr)))
#endif /* __BIG_ENDIAN */

static inline bool inet_match(struct net *net, const struct sock *sk,
			      const __addrpair cookie, const __portpair ports,
			      int dif, int sdif)
{
	if (!net_eq(sock_net(sk), net) ||
	    sk->sk_portpair != ports ||
	    sk->sk_addrpair != cookie)
	        return false;

	/* READ_ONCE() paired with WRITE_ONCE() in sock_bindtoindex_locked() */
	return inet_sk_bound_dev_eq(net, READ_ONCE(sk->sk_bound_dev_if), dif,
				    sdif);
}

/* Sockets in TCP_CLOSE state are _always_ taken out of the hash, so we need
 * not check it for lookups anymore, thanks Alexey. -DaveM
 */
struct sock *__inet_lookup_established(struct net *net,
				       struct inet_hashinfo *hashinfo,
				       const __be32 saddr, const __be16 sport,
				       const __be32 daddr, const u16 hnum,
				       const int dif, const int sdif);

typedef u32 (inet_ehashfn_t)(const struct net *net,
			      const __be32 laddr, const __u16 lport,
			      const __be32 faddr, const __be16 fport);

inet_ehashfn_t inet_ehashfn;

INDIRECT_CALLABLE_DECLARE(inet_ehashfn_t udp_ehashfn);

struct sock *inet_lookup_reuseport(struct net *net, struct sock *sk,
				   struct sk_buff *skb, int doff,
				   __be32 saddr, __be16 sport,
				   __be32 daddr, unsigned short hnum,
				   inet_ehashfn_t *ehashfn);

struct sock *inet_lookup_run_sk_lookup(struct net *net,
				       int protocol,
				       struct sk_buff *skb, int doff,
				       __be32 saddr, __be16 sport,
				       __be32 daddr, u16 hnum, const int dif,
				       inet_ehashfn_t *ehashfn);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline struct sock *
	inet_lookup_established(struct net *net, struct inet_hashinfo *hashinfo,
				const __be32 saddr, const __be16 sport,
				const __be32 daddr, const __be16 dport,
				const int dif)
{
	return __inet_lookup_established(net, hashinfo, saddr, sport, daddr,
<<<<<<< HEAD
					 ntohs(dport), dif);
=======
					 ntohs(dport), dif, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline struct sock *__inet_lookup(struct net *net,
					 struct inet_hashinfo *hashinfo,
<<<<<<< HEAD
					 const __be32 saddr, const __be16 sport,
					 const __be32 daddr, const __be16 dport,
					 const int dif)
{
	u16 hnum = ntohs(dport);
	struct sock *sk = __inet_lookup_established(net, hashinfo,
				saddr, sport, daddr, hnum, dif);

	return sk ? : __inet_lookup_listener(net, hashinfo, daddr, hnum, dif);
=======
					 struct sk_buff *skb, int doff,
					 const __be32 saddr, const __be16 sport,
					 const __be32 daddr, const __be16 dport,
					 const int dif, const int sdif,
					 bool *refcounted)
{
	u16 hnum = ntohs(dport);
	struct sock *sk;

	sk = __inet_lookup_established(net, hashinfo, saddr, sport,
				       daddr, hnum, dif, sdif);
	*refcounted = true;
	if (sk)
		return sk;
	*refcounted = false;
	return __inet_lookup_listener(net, hashinfo, skb, doff, saddr,
				      sport, daddr, hnum, dif, sdif);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline struct sock *inet_lookup(struct net *net,
				       struct inet_hashinfo *hashinfo,
<<<<<<< HEAD
=======
				       struct sk_buff *skb, int doff,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				       const __be32 saddr, const __be16 sport,
				       const __be32 daddr, const __be16 dport,
				       const int dif)
{
	struct sock *sk;
<<<<<<< HEAD

	local_bh_disable();
	sk = __inet_lookup(net, hashinfo, saddr, sport, daddr, dport, dif);
	local_bh_enable();

	return sk;
}

static inline struct sock *__inet_lookup_skb(struct inet_hashinfo *hashinfo,
					     struct sk_buff *skb,
					     const __be16 sport,
					     const __be16 dport)
{
	struct sock *sk;
	const struct iphdr *iph = ip_hdr(skb);

	if (unlikely(sk = skb_steal_sock(skb)))
		return sk;
	else
		return __inet_lookup(dev_net(skb_dst(skb)->dev), hashinfo,
				     iph->saddr, sport,
				     iph->daddr, dport, inet_iif(skb));
}

extern int __inet_hash_connect(struct inet_timewait_death_row *death_row,
		struct sock *sk,
		u32 port_offset,
		int (*check_established)(struct inet_timewait_death_row *,
			struct sock *, __u16, struct inet_timewait_sock **),
		int (*hash)(struct sock *sk, struct inet_timewait_sock *twp));

extern int inet_hash_connect(struct inet_timewait_death_row *death_row,
			     struct sock *sk);
=======
	bool refcounted;

	sk = __inet_lookup(net, hashinfo, skb, doff, saddr, sport, daddr,
			   dport, dif, 0, &refcounted);

	if (sk && !refcounted && !refcount_inc_not_zero(&sk->sk_refcnt))
		sk = NULL;
	return sk;
}

static inline
struct sock *inet_steal_sock(struct net *net, struct sk_buff *skb, int doff,
			     const __be32 saddr, const __be16 sport,
			     const __be32 daddr, const __be16 dport,
			     bool *refcounted, inet_ehashfn_t *ehashfn)
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

	reuse_sk = inet_lookup_reuseport(net, sk, skb, doff,
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

static inline struct sock *__inet_lookup_skb(struct inet_hashinfo *hashinfo,
					     struct sk_buff *skb,
					     int doff,
					     const __be16 sport,
					     const __be16 dport,
					     const int sdif,
					     bool *refcounted)
{
	struct net *net = dev_net(skb_dst(skb)->dev);
	const struct iphdr *iph = ip_hdr(skb);
	struct sock *sk;

	sk = inet_steal_sock(net, skb, doff, iph->saddr, sport, iph->daddr, dport,
			     refcounted, inet_ehashfn);
	if (IS_ERR(sk))
		return NULL;
	if (sk)
		return sk;

	return __inet_lookup(net, hashinfo, skb,
			     doff, iph->saddr, sport,
			     iph->daddr, dport, inet_iif(skb), sdif,
			     refcounted);
}

static inline void sk_daddr_set(struct sock *sk, __be32 addr)
{
	sk->sk_daddr = addr; /* alias of inet_daddr */
#if IS_ENABLED(CONFIG_IPV6)
	ipv6_addr_set_v4mapped(addr, &sk->sk_v6_daddr);
#endif
}

static inline void sk_rcv_saddr_set(struct sock *sk, __be32 addr)
{
	sk->sk_rcv_saddr = addr; /* alias of inet_rcv_saddr */
#if IS_ENABLED(CONFIG_IPV6)
	ipv6_addr_set_v4mapped(addr, &sk->sk_v6_rcv_saddr);
#endif
}

int __inet_hash_connect(struct inet_timewait_death_row *death_row,
			struct sock *sk, u64 port_offset,
			int (*check_established)(struct inet_timewait_death_row *,
						 struct sock *, __u16,
						 struct inet_timewait_sock **));

int inet_hash_connect(struct inet_timewait_death_row *death_row,
		      struct sock *sk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _INET_HASHTABLES_H */
