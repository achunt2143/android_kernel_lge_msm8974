<<<<<<< HEAD
#ifndef _NF_DEFRAG_IPV6_H
#define _NF_DEFRAG_IPV6_H

extern void nf_defrag_ipv6_enable(void);

extern int nf_ct_frag6_init(void);
extern void nf_ct_frag6_cleanup(void);
extern struct sk_buff *nf_ct_frag6_gather(struct sk_buff *skb, u32 user);
extern void nf_ct_frag6_output(unsigned int hooknum, struct sk_buff *skb,
			       struct net_device *in,
			       struct net_device *out,
			       int (*okfn)(struct sk_buff *));

struct inet_frags_ctl;

=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _NF_DEFRAG_IPV6_H
#define _NF_DEFRAG_IPV6_H

#include <linux/skbuff.h>
#include <linux/types.h>

int nf_defrag_ipv6_enable(struct net *net);
void nf_defrag_ipv6_disable(struct net *net);

int nf_ct_frag6_init(void);
void nf_ct_frag6_cleanup(void);
int nf_ct_frag6_gather(struct net *net, struct sk_buff *skb, u32 user);

struct inet_frags_ctl;

struct nft_ct_frag6_pernet {
	struct ctl_table_header *nf_frag_frags_hdr;
	struct fqdir	*fqdir;
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _NF_DEFRAG_IPV6_H */
