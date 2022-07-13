<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * INET		An implementation of the TCP/IP protocol suite for the LINUX
 *		operating system.  INET is implemented using the  BSD Socket
 *		interface as the means of communication with the user level.
 *
 *		Definitions for the RAW-IP module.
 *
 * Version:	@(#)raw.h	1.0.2	05/07/93
 *
 * Author:	Fred N. van Kempen, <waltje@uWalt.NL.Mugnet.ORG>
<<<<<<< HEAD
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#ifndef _RAW_H
#define _RAW_H

<<<<<<< HEAD

#include <net/protocol.h>
=======
#include <net/inet_sock.h>
#include <net/protocol.h>
#include <net/netns/hash.h>
#include <linux/hash.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/icmp.h>

extern struct proto raw_prot;

<<<<<<< HEAD
void raw_icmp_error(struct sk_buff *, int, u32);
int raw_local_deliver(struct sk_buff *, int);

extern int 	raw_rcv(struct sock *, struct sk_buff *);

#define RAW_HTABLE_SIZE	MAX_INET_PROTOS

struct raw_hashinfo {
	rwlock_t lock;
	struct hlist_head ht[RAW_HTABLE_SIZE];
};

#ifdef CONFIG_PROC_FS
extern int  raw_proc_init(void);
extern void raw_proc_exit(void);
=======
extern struct raw_hashinfo raw_v4_hashinfo;
bool raw_v4_match(struct net *net, const struct sock *sk, unsigned short num,
		  __be32 raddr, __be32 laddr, int dif, int sdif);

int raw_abort(struct sock *sk, int err);
void raw_icmp_error(struct sk_buff *, int, u32);
int raw_local_deliver(struct sk_buff *, int);

int raw_rcv(struct sock *, struct sk_buff *);

#define RAW_HTABLE_LOG	8
#define RAW_HTABLE_SIZE	(1U << RAW_HTABLE_LOG)

struct raw_hashinfo {
	spinlock_t lock;

	struct hlist_head ht[RAW_HTABLE_SIZE] ____cacheline_aligned;
};

static inline u32 raw_hashfunc(const struct net *net, u32 proto)
{
	return hash_32(net_hash_mix(net) ^ proto, RAW_HTABLE_LOG);
}

static inline void raw_hashinfo_init(struct raw_hashinfo *hashinfo)
{
	int i;

	spin_lock_init(&hashinfo->lock);
	for (i = 0; i < RAW_HTABLE_SIZE; i++)
		INIT_HLIST_HEAD(&hashinfo->ht[i]);
}

#ifdef CONFIG_PROC_FS
int raw_proc_init(void);
void raw_proc_exit(void);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct raw_iter_state {
	struct seq_net_private p;
	int bucket;
<<<<<<< HEAD
	struct raw_hashinfo *h;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static inline struct raw_iter_state *raw_seq_private(struct seq_file *seq)
{
	return seq->private;
}
void *raw_seq_start(struct seq_file *seq, loff_t *pos);
void *raw_seq_next(struct seq_file *seq, void *v, loff_t *pos);
void raw_seq_stop(struct seq_file *seq, void *v);
<<<<<<< HEAD
int raw_seq_open(struct inode *ino, struct file *file,
		 struct raw_hashinfo *h, const struct seq_operations *ops);

#endif

void raw_hash_sk(struct sock *sk);
void raw_unhash_sk(struct sock *sk);
=======
#endif

int raw_hash_sk(struct sock *sk);
void raw_unhash_sk(struct sock *sk);
void raw_init(void);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct raw_sock {
	/* inet_sock has to be the first member */
	struct inet_sock   inet;
	struct icmp_filter filter;
	u32		   ipmr_table;
};

<<<<<<< HEAD
static inline struct raw_sock *raw_sk(const struct sock *sk)
{
	return (struct raw_sock *)sk;
=======
#define raw_sk(ptr) container_of_const(ptr, struct raw_sock, inet.sk)

static inline bool raw_sk_bound_dev_eq(struct net *net, int bound_dev_if,
				       int dif, int sdif)
{
#if IS_ENABLED(CONFIG_NET_L3_MASTER_DEV)
	return inet_bound_dev_eq(READ_ONCE(net->ipv4.sysctl_raw_l3mdev_accept),
				 bound_dev_if, dif, sdif);
#else
	return inet_bound_dev_eq(true, bound_dev_if, dif, sdif);
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#endif	/* _RAW_H */
