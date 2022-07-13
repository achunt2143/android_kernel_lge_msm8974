<<<<<<< HEAD
#ifndef _NF_CONNTRACK_SNMP_H
#define _NF_CONNTRACK_SNMP_H

=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _NF_CONNTRACK_SNMP_H
#define _NF_CONNTRACK_SNMP_H

#include <linux/netfilter.h>
#include <linux/skbuff.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern int (*nf_nat_snmp_hook)(struct sk_buff *skb,
				unsigned int protoff,
				struct nf_conn *ct,
				enum ip_conntrack_info ctinfo);

#endif /* _NF_CONNTRACK_SNMP_H */
