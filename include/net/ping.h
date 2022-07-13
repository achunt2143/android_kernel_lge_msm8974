<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * INET		An implementation of the TCP/IP protocol suite for the LINUX
 *		operating system.  INET is implemented using the  BSD Socket
 *		interface as the means of communication with the user level.
 *
 *		Definitions for the "ping" module.
<<<<<<< HEAD
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#ifndef _PING_H
#define _PING_H

#include <net/icmp.h>
#include <net/netns/hash.h>

/* PING_HTABLE_SIZE must be power of 2 */
#define PING_HTABLE_SIZE 	64
#define PING_HTABLE_MASK 	(PING_HTABLE_SIZE-1)

<<<<<<< HEAD
#define ping_portaddr_for_each_entry(__sk, node, list) \
	hlist_nulls_for_each_entry(__sk, node, list, sk_nulls_node)

/*
 * gid_t is either uint or ushort.  We want to pass it to
 * proc_dointvec_minmax(), so it must not be larger than MAX_INT
 */
#define GID_T_MAX (((gid_t)~0U) >> 1)
=======
#define GID_T_MAX (((gid_t)~0U) - 1)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Compatibility glue so we can support IPv6 when it's compiled as a module */
struct pingv6_ops {
	int (*ipv6_recv_error)(struct sock *sk, struct msghdr *msg, int len,
<<<<<<< HEAD
				 int *addr_len);
	int (*datagram_recv_ctl)(struct sock *sk, struct msghdr *msg,
				 struct sk_buff *skb);
=======
			       int *addr_len);
	void (*ip6_datagram_recv_common_ctl)(struct sock *sk,
					     struct msghdr *msg,
					     struct sk_buff *skb);
	void (*ip6_datagram_recv_specific_ctl)(struct sock *sk,
					       struct msghdr *msg,
					       struct sk_buff *skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int (*icmpv6_err_convert)(u8 type, u8 code, int *err);
	void (*ipv6_icmp_error)(struct sock *sk, struct sk_buff *skb, int err,
				__be16 port, u32 info, u8 *payload);
	int (*ipv6_chk_addr)(struct net *net, const struct in6_addr *addr,
<<<<<<< HEAD
			     struct net_device *dev, int strict);
};

struct ping_table {
	struct hlist_nulls_head	hash[PING_HTABLE_SIZE];
	rwlock_t		lock;
=======
			     const struct net_device *dev, int strict);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct ping_iter_state {
	struct seq_net_private  p;
	int			bucket;
<<<<<<< HEAD
};

extern struct proto ping_prot;
extern struct ping_table ping_table;
=======
	sa_family_t		family;
};

extern struct proto ping_prot;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#if IS_ENABLED(CONFIG_IPV6)
extern struct pingv6_ops pingv6_ops;
#endif

struct pingfakehdr {
	struct icmphdr icmph;
<<<<<<< HEAD
	struct iovec *iov;
=======
	struct msghdr *msg;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	sa_family_t family;
	__wsum wcheck;
};

int  ping_get_port(struct sock *sk, unsigned short ident);
<<<<<<< HEAD
void ping_hash(struct sock *sk);
=======
int ping_hash(struct sock *sk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void ping_unhash(struct sock *sk);

int  ping_init_sock(struct sock *sk);
void ping_close(struct sock *sk, long timeout);
int  ping_bind(struct sock *sk, struct sockaddr *uaddr, int addr_len);
void ping_err(struct sk_buff *skb, int offset, u32 info);
<<<<<<< HEAD
void ping_v4_err(struct sk_buff *skb, u32 info);
int  ping_getfrag(void *from, char *to, int offset, int fraglen, int odd,
		  struct sk_buff *);

int  ping_recvmsg(struct kiocb *iocb, struct sock *sk, struct msghdr *msg,
		  size_t len, int noblock, int flags, int *addr_len);
int  ping_common_sendmsg(int family, struct msghdr *msg, size_t len,
			 void *user_icmph, size_t icmph_len);
int  ping_v4_sendmsg(struct kiocb *iocb, struct sock *sk, struct msghdr *msg,
		     size_t len);
int  ping_v6_sendmsg(struct kiocb *iocb, struct sock *sk, struct msghdr *msg,
		     size_t len);
int  ping_queue_rcv_skb(struct sock *sk, struct sk_buff *skb);
void ping_rcv(struct sk_buff *skb);

#ifdef CONFIG_PROC_FS
extern int __init ping_proc_init(void);
extern void ping_proc_exit(void);
=======
int  ping_getfrag(void *from, char *to, int offset, int fraglen, int odd,
		  struct sk_buff *);

int  ping_recvmsg(struct sock *sk, struct msghdr *msg, size_t len,
		  int flags, int *addr_len);
int  ping_common_sendmsg(int family, struct msghdr *msg, size_t len,
			 void *user_icmph, size_t icmph_len);
int  ping_queue_rcv_skb(struct sock *sk, struct sk_buff *skb);
enum skb_drop_reason ping_rcv(struct sk_buff *skb);

#ifdef CONFIG_PROC_FS
void *ping_seq_start(struct seq_file *seq, loff_t *pos, sa_family_t family);
void *ping_seq_next(struct seq_file *seq, void *v, loff_t *pos);
void ping_seq_stop(struct seq_file *seq, void *v);

int __init ping_proc_init(void);
void ping_proc_exit(void);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

void __init ping_init(void);
int  __init pingv6_init(void);
void pingv6_exit(void);

#endif /* _PING_H */
