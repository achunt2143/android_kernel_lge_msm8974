<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _NF_INTERNALS_H
#define _NF_INTERNALS_H

#include <linux/list.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>

<<<<<<< HEAD
#ifdef CONFIG_NETFILTER_DEBUG
#define NFDEBUG(format, args...)  printk(KERN_DEBUG format , ## args)
#else
#define NFDEBUG(format, args...)
#endif


/* core.c */
extern unsigned int nf_iterate(struct list_head *head,
				struct sk_buff *skb,
				unsigned int hook,
				const struct net_device *indev,
				const struct net_device *outdev,
				struct list_head **i,
				int (*okfn)(struct sk_buff *),
				int hook_thresh);

/* nf_queue.c */
extern int nf_queue(struct sk_buff *skb,
		    struct list_head *elem,
		    u_int8_t pf, unsigned int hook,
		    struct net_device *indev,
		    struct net_device *outdev,
		    int (*okfn)(struct sk_buff *),
		    unsigned int queuenum);
extern int __init netfilter_queue_init(void);

/* nf_log.c */
extern int __init netfilter_log_init(void);

=======
/* nf_conntrack_netlink.c: applied on tuple filters */
#define CTA_FILTER_F_CTA_IP_SRC			(1 << 0)
#define CTA_FILTER_F_CTA_IP_DST			(1 << 1)
#define CTA_FILTER_F_CTA_TUPLE_ZONE		(1 << 2)
#define CTA_FILTER_F_CTA_PROTO_NUM		(1 << 3)
#define CTA_FILTER_F_CTA_PROTO_SRC_PORT		(1 << 4)
#define CTA_FILTER_F_CTA_PROTO_DST_PORT		(1 << 5)
#define CTA_FILTER_F_CTA_PROTO_ICMP_TYPE	(1 << 6)
#define CTA_FILTER_F_CTA_PROTO_ICMP_CODE	(1 << 7)
#define CTA_FILTER_F_CTA_PROTO_ICMP_ID		(1 << 8)
#define CTA_FILTER_F_CTA_PROTO_ICMPV6_TYPE	(1 << 9)
#define CTA_FILTER_F_CTA_PROTO_ICMPV6_CODE	(1 << 10)
#define CTA_FILTER_F_CTA_PROTO_ICMPV6_ID	(1 << 11)
#define CTA_FILTER_F_MAX			(1 << 12)
#define CTA_FILTER_F_ALL			(CTA_FILTER_F_MAX-1)
#define CTA_FILTER_FLAG(ctattr) CTA_FILTER_F_ ## ctattr

/* nf_queue.c */
void nf_queue_nf_hook_drop(struct net *net);

/* nf_log.c */
int __init netfilter_log_init(void);

/* core.c */
void nf_hook_entries_delete_raw(struct nf_hook_entries __rcu **pp,
				const struct nf_hook_ops *reg);
int nf_hook_entries_insert_raw(struct nf_hook_entries __rcu **pp,
				const struct nf_hook_ops *reg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
