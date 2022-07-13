<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * This header is used to share core functionality between the
 * standalone connection tracking module, and the compatibility layer's use
 * of connection tracking.
 *
 * 16 Dec 2003: Yasuyuki Kozakai @USAGI <yasuyuki.kozakai@toshiba.co.jp>
 *	- generalize L3 protocol dependent part.
 *
 * Derived from include/linux/netfiter_ipv4/ip_conntrack_core.h
 */

#ifndef _NF_CONNTRACK_CORE_H
#define _NF_CONNTRACK_CORE_H

#include <linux/netfilter.h>
<<<<<<< HEAD
#include <net/netfilter/nf_conntrack_l3proto.h>
#include <net/netfilter/nf_conntrack_l4proto.h>
#include <net/netfilter/nf_conntrack_ecache.h>
=======
#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_ecache.h>
#include <net/netfilter/nf_conntrack_l4proto.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* This header is used to share core functionality between the
   standalone connection tracking module, and the compatibility layer's use
   of connection tracking. */
<<<<<<< HEAD
extern unsigned int nf_conntrack_in(struct net *net,
				    u_int8_t pf,
				    unsigned int hooknum,
				    struct sk_buff *skb);

extern int nf_conntrack_init(struct net *net);
extern void nf_conntrack_cleanup(struct net *net);

extern int nf_conntrack_proto_init(void);
extern void nf_conntrack_proto_fini(void);

extern bool
nf_ct_get_tuple(const struct sk_buff *skb,
		unsigned int nhoff,
		unsigned int dataoff,
		u_int16_t l3num,
		u_int8_t protonum,
		struct nf_conntrack_tuple *tuple,
		const struct nf_conntrack_l3proto *l3proto,
		const struct nf_conntrack_l4proto *l4proto);

extern bool
nf_ct_invert_tuple(struct nf_conntrack_tuple *inverse,
		   const struct nf_conntrack_tuple *orig,
		   const struct nf_conntrack_l3proto *l3proto,
		   const struct nf_conntrack_l4proto *l4proto);

/* Find a connection corresponding to a tuple. */
extern struct nf_conntrack_tuple_hash *
nf_conntrack_find_get(struct net *net, u16 zone,
		      const struct nf_conntrack_tuple *tuple);

extern int __nf_conntrack_confirm(struct sk_buff *skb);
=======

unsigned int nf_conntrack_in(struct sk_buff *skb,
			     const struct nf_hook_state *state);

int nf_conntrack_init_net(struct net *net);
void nf_conntrack_cleanup_net(struct net *net);
void nf_conntrack_cleanup_net_list(struct list_head *net_exit_list);

void nf_conntrack_proto_pernet_init(struct net *net);

int nf_conntrack_proto_init(void);
void nf_conntrack_proto_fini(void);

int nf_conntrack_init_start(void);
void nf_conntrack_cleanup_start(void);

void nf_conntrack_init_end(void);
void nf_conntrack_cleanup_end(void);

bool nf_ct_invert_tuple(struct nf_conntrack_tuple *inverse,
			const struct nf_conntrack_tuple *orig);

/* Find a connection corresponding to a tuple. */
struct nf_conntrack_tuple_hash *
nf_conntrack_find_get(struct net *net,
		      const struct nf_conntrack_zone *zone,
		      const struct nf_conntrack_tuple *tuple);

int __nf_conntrack_confirm(struct sk_buff *skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Confirm a connection: returns NF_DROP if packet must be dropped. */
static inline int nf_conntrack_confirm(struct sk_buff *skb)
{
<<<<<<< HEAD
	struct nf_conn *ct = (struct nf_conn *)skb->nfct;
	int ret = NF_ACCEPT;

	if (ct && !nf_ct_is_untracked(ct)) {
		if (!nf_ct_is_confirmed(ct))
			ret = __nf_conntrack_confirm(skb);
		if (likely(ret == NF_ACCEPT))
=======
	struct nf_conn *ct = (struct nf_conn *)skb_nfct(skb);
	int ret = NF_ACCEPT;

	if (ct) {
		if (!nf_ct_is_confirmed(ct)) {
			ret = __nf_conntrack_confirm(skb);

			if (ret == NF_ACCEPT)
				ct = (struct nf_conn *)skb_nfct(skb);
		}

		if (ret == NF_ACCEPT && nf_ct_ecache_exist(ct))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			nf_ct_deliver_cached_events(ct);
	}
	return ret;
}

<<<<<<< HEAD
int
print_tuple(struct seq_file *s, const struct nf_conntrack_tuple *tuple,
            const struct nf_conntrack_l3proto *l3proto,
            const struct nf_conntrack_l4proto *proto);

extern spinlock_t nf_conntrack_lock ;
=======
unsigned int nf_confirm(void *priv, struct sk_buff *skb, const struct nf_hook_state *state);

void print_tuple(struct seq_file *s, const struct nf_conntrack_tuple *tuple,
		 const struct nf_conntrack_l4proto *proto);

#define CONNTRACK_LOCKS 1024

extern spinlock_t nf_conntrack_locks[CONNTRACK_LOCKS];
void nf_conntrack_lock(spinlock_t *lock);

extern spinlock_t nf_conntrack_expect_lock;

/* ctnetlink code shared by both ctnetlink and nf_conntrack_bpf */

static inline void __nf_ct_set_timeout(struct nf_conn *ct, u64 timeout)
{
	if (timeout > INT_MAX)
		timeout = INT_MAX;

	if (nf_ct_is_confirmed(ct))
		WRITE_ONCE(ct->timeout, nfct_time_stamp + (u32)timeout);
	else
		ct->timeout = (u32)timeout;
}

int __nf_ct_change_timeout(struct nf_conn *ct, u64 cta_timeout);
void __nf_ct_change_status(struct nf_conn *ct, unsigned long on, unsigned long off);
int nf_ct_change_status_common(struct nf_conn *ct, unsigned int status);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _NF_CONNTRACK_CORE_H */
