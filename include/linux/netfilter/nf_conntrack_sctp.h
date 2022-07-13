<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _NF_CONNTRACK_SCTP_H
#define _NF_CONNTRACK_SCTP_H
/* SCTP tracking. */

<<<<<<< HEAD
#include <linux/netfilter/nf_conntrack_tuple_common.h>

enum sctp_conntrack {
	SCTP_CONNTRACK_NONE,
	SCTP_CONNTRACK_CLOSED,
	SCTP_CONNTRACK_COOKIE_WAIT,
	SCTP_CONNTRACK_COOKIE_ECHOED,
	SCTP_CONNTRACK_ESTABLISHED,
	SCTP_CONNTRACK_SHUTDOWN_SENT,
	SCTP_CONNTRACK_SHUTDOWN_RECD,
	SCTP_CONNTRACK_SHUTDOWN_ACK_SENT,
	SCTP_CONNTRACK_MAX
};
=======
#include <uapi/linux/netfilter/nf_conntrack_sctp.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct ip_ct_sctp {
	enum sctp_conntrack state;

	__be32 vtag[IP_CT_DIR_MAX];
<<<<<<< HEAD
=======
	u8 init[IP_CT_DIR_MAX];
	u8 last_dir;
	u8 flags;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

#endif /* _NF_CONNTRACK_SCTP_H */
