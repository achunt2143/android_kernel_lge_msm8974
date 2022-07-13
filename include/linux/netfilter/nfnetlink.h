<<<<<<< HEAD
#ifndef _NFNETLINK_H
#define _NFNETLINK_H
#include <linux/types.h>
#include <linux/netfilter/nfnetlink_compat.h>

enum nfnetlink_groups {
	NFNLGRP_NONE,
#define NFNLGRP_NONE			NFNLGRP_NONE
	NFNLGRP_CONNTRACK_NEW,
#define NFNLGRP_CONNTRACK_NEW		NFNLGRP_CONNTRACK_NEW
	NFNLGRP_CONNTRACK_UPDATE,
#define NFNLGRP_CONNTRACK_UPDATE	NFNLGRP_CONNTRACK_UPDATE
	NFNLGRP_CONNTRACK_DESTROY,
#define NFNLGRP_CONNTRACK_DESTROY	NFNLGRP_CONNTRACK_DESTROY
	NFNLGRP_CONNTRACK_EXP_NEW,
#define	NFNLGRP_CONNTRACK_EXP_NEW	NFNLGRP_CONNTRACK_EXP_NEW
	NFNLGRP_CONNTRACK_EXP_UPDATE,
#define NFNLGRP_CONNTRACK_EXP_UPDATE	NFNLGRP_CONNTRACK_EXP_UPDATE
	NFNLGRP_CONNTRACK_EXP_DESTROY,
#define NFNLGRP_CONNTRACK_EXP_DESTROY	NFNLGRP_CONNTRACK_EXP_DESTROY
	__NFNLGRP_MAX,
};
#define NFNLGRP_MAX	(__NFNLGRP_MAX - 1)

/* General form of address family dependent message.
 */
struct nfgenmsg {
	__u8  nfgen_family;		/* AF_xxx */
	__u8  version;		/* nfnetlink version */
	__be16    res_id;		/* resource id */
};

#define NFNETLINK_V0	0

/* netfilter netlink message types are split in two pieces:
 * 8 bit subsystem, 8bit operation.
 */

#define NFNL_SUBSYS_ID(x)	((x & 0xff00) >> 8)
#define NFNL_MSG_TYPE(x)	(x & 0x00ff)

/* No enum here, otherwise __stringify() trick of MODULE_ALIAS_NFNL_SUBSYS()
 * won't work anymore */
#define NFNL_SUBSYS_NONE 		0
#define NFNL_SUBSYS_CTNETLINK		1
#define NFNL_SUBSYS_CTNETLINK_EXP	2
#define NFNL_SUBSYS_QUEUE		3
#define NFNL_SUBSYS_ULOG		4
#define NFNL_SUBSYS_OSF			5
#define NFNL_SUBSYS_IPSET		6
#define NFNL_SUBSYS_ACCT		7
#define NFNL_SUBSYS_CTNETLINK_TIMEOUT	8
#define NFNL_SUBSYS_CTHELPER		9
#define NFNL_SUBSYS_COUNT		10

#ifdef __KERNEL__
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _NFNETLINK_H
#define _NFNETLINK_H
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <linux/netlink.h>
#include <linux/capability.h>
#include <net/netlink.h>
<<<<<<< HEAD

struct nfnl_callback {
	int (*call)(struct sock *nl, struct sk_buff *skb, 
		    const struct nlmsghdr *nlh,
		    const struct nlattr * const cda[]);
	int (*call_rcu)(struct sock *nl, struct sk_buff *skb, 
		    const struct nlmsghdr *nlh,
		    const struct nlattr * const cda[]);
	const struct nla_policy *policy;	/* netlink attribute policy */
	const u_int16_t attr_count;		/* number of nlattr's */
=======
#include <uapi/linux/netfilter/nfnetlink.h>

struct nfnl_info {
	struct net		*net;
	struct sock		*sk;
	const struct nlmsghdr	*nlh;
	const struct nfgenmsg	*nfmsg;
	struct netlink_ext_ack	*extack;
};

enum nfnl_callback_type {
	NFNL_CB_UNSPEC	= 0,
	NFNL_CB_MUTEX,
	NFNL_CB_RCU,
	NFNL_CB_BATCH,
};

struct nfnl_callback {
	int (*call)(struct sk_buff *skb, const struct nfnl_info *info,
		    const struct nlattr * const cda[]);
	const struct nla_policy	*policy;
	enum nfnl_callback_type	type;
	__u16			attr_count;
};

enum nfnl_abort_action {
	NFNL_ABORT_NONE		= 0,
	NFNL_ABORT_AUTOLOAD,
	NFNL_ABORT_VALIDATE,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct nfnetlink_subsystem {
	const char *name;
	__u8 subsys_id;			/* nfnetlink subsystem ID */
	__u8 cb_count;			/* number of callbacks */
	const struct nfnl_callback *cb;	/* callback for individual types */
<<<<<<< HEAD
};

extern int nfnetlink_subsys_register(const struct nfnetlink_subsystem *n);
extern int nfnetlink_subsys_unregister(const struct nfnetlink_subsystem *n);

extern int nfnetlink_has_listeners(struct net *net, unsigned int group);
extern int nfnetlink_send(struct sk_buff *skb, struct net *net, u32 pid, unsigned group,
			  int echo, gfp_t flags);
extern int nfnetlink_set_err(struct net *net, u32 pid, u32 group, int error);
extern int nfnetlink_unicast(struct sk_buff *skb, struct net *net, u_int32_t pid, int flags);

extern void nfnl_lock(void);
extern void nfnl_unlock(void);
=======
	struct module *owner;
	int (*commit)(struct net *net, struct sk_buff *skb);
	int (*abort)(struct net *net, struct sk_buff *skb,
		     enum nfnl_abort_action action);
	bool (*valid_genid)(struct net *net, u32 genid);
};

int nfnetlink_subsys_register(const struct nfnetlink_subsystem *n);
int nfnetlink_subsys_unregister(const struct nfnetlink_subsystem *n);

int nfnetlink_has_listeners(struct net *net, unsigned int group);
int nfnetlink_send(struct sk_buff *skb, struct net *net, u32 portid,
		   unsigned int group, int echo, gfp_t flags);
int nfnetlink_set_err(struct net *net, u32 portid, u32 group, int error);
int nfnetlink_unicast(struct sk_buff *skb, struct net *net, u32 portid);
void nfnetlink_broadcast(struct net *net, struct sk_buff *skb, __u32 portid,
			 __u32 group, gfp_t allocation);

static inline u16 nfnl_msg_type(u8 subsys, u8 msg_type)
{
	return subsys << 8 | msg_type;
}

static inline void nfnl_fill_hdr(struct nlmsghdr *nlh, u8 family, u8 version,
				 __be16 res_id)
{
	struct nfgenmsg *nfmsg;

	nfmsg = nlmsg_data(nlh);
	nfmsg->nfgen_family = family;
	nfmsg->version = version;
	nfmsg->res_id = res_id;
}

static inline struct nlmsghdr *nfnl_msg_put(struct sk_buff *skb, u32 portid,
					    u32 seq, int type, int flags,
					    u8 family, u8 version,
					    __be16 res_id)
{
	struct nlmsghdr *nlh;

	nlh = nlmsg_put(skb, portid, seq, type, sizeof(struct nfgenmsg), flags);
	if (!nlh)
		return NULL;

	nfnl_fill_hdr(nlh, family, version, res_id);

	return nlh;
}

void nfnl_lock(__u8 subsys_id);
void nfnl_unlock(__u8 subsys_id);
#ifdef CONFIG_PROVE_LOCKING
bool lockdep_nfnl_is_held(__u8 subsys_id);
#else
static inline bool lockdep_nfnl_is_held(__u8 subsys_id)
{
	return true;
}
#endif /* CONFIG_PROVE_LOCKING */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define MODULE_ALIAS_NFNL_SUBSYS(subsys) \
	MODULE_ALIAS("nfnetlink-subsys-" __stringify(subsys))

<<<<<<< HEAD
#endif	/* __KERNEL__ */
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif	/* _NFNETLINK_H */
