<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef __NET_TC_NAT_H
#define __NET_TC_NAT_H

#include <linux/types.h>
#include <net/act_api.h>

<<<<<<< HEAD
struct tcf_nat {
	struct tcf_common common;

=======
struct tcf_nat_parms {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	__be32 old_addr;
	__be32 new_addr;
	__be32 mask;
	u32 flags;
<<<<<<< HEAD
};

static inline struct tcf_nat *to_tcf_nat(struct tcf_common *pc)
{
	return container_of(pc, struct tcf_nat, common);
}
=======
	struct rcu_head rcu;
};

struct tcf_nat {
	struct tc_action common;
	struct tcf_nat_parms __rcu *parms;
};

#define to_tcf_nat(a) ((struct tcf_nat *)a)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* __NET_TC_NAT_H */
