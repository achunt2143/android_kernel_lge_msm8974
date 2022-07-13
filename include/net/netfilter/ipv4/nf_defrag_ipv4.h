<<<<<<< HEAD
#ifndef _NF_DEFRAG_IPV4_H
#define _NF_DEFRAG_IPV4_H

extern void nf_defrag_ipv4_enable(void);
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _NF_DEFRAG_IPV4_H
#define _NF_DEFRAG_IPV4_H

struct net;
int nf_defrag_ipv4_enable(struct net *net);
void nf_defrag_ipv4_disable(struct net *net);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _NF_DEFRAG_IPV4_H */
