<<<<<<< HEAD
#ifndef _NF_CONNTRACK_IPV6_H
#define _NF_CONNTRACK_IPV6_H

extern struct nf_conntrack_l3proto nf_conntrack_l3proto_ipv6;

extern struct nf_conntrack_l4proto nf_conntrack_l4proto_tcp6;
extern struct nf_conntrack_l4proto nf_conntrack_l4proto_udp6;
extern struct nf_conntrack_l4proto nf_conntrack_l4proto_icmpv6;

#include <linux/sysctl.h>
extern struct ctl_table nf_ct_ipv6_sysctl_table[];
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _NF_CONNTRACK_IPV6_H
#define _NF_CONNTRACK_IPV6_H

extern const struct nf_conntrack_l4proto nf_conntrack_l4proto_icmpv6;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _NF_CONNTRACK_IPV6_H*/
