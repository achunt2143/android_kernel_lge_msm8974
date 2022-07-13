<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _NF_CONNTRACK_TIMEOUT_H
#define _NF_CONNTRACK_TIMEOUT_H

#include <net/net_namespace.h>
#include <linux/netfilter/nf_conntrack_common.h>
#include <linux/netfilter/nf_conntrack_tuple_common.h>
<<<<<<< HEAD
=======
#include <linux/refcount.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_extend.h>

#define CTNL_TIMEOUT_NAME_MAX	32

<<<<<<< HEAD
struct ctnl_timeout {
	struct list_head	head;
	struct rcu_head		rcu_head;
	atomic_t		refcnt;
	char			name[CTNL_TIMEOUT_NAME_MAX];
	__u16			l3num;
	struct nf_conntrack_l4proto *l4proto;
	char			data[0];
};

struct nf_conn_timeout {
	struct ctnl_timeout	*timeout;
};

#define NF_CT_TIMEOUT_EXT_DATA(__t) (unsigned int *) &((__t)->timeout->data)
=======
struct nf_ct_timeout {
	__u16			l3num;
	const struct nf_conntrack_l4proto *l4proto;
	char			data[];
};

struct nf_conn_timeout {
	struct nf_ct_timeout __rcu *timeout;
};

static inline unsigned int *
nf_ct_timeout_data(const struct nf_conn_timeout *t)
{
#ifdef CONFIG_NF_CONNTRACK_TIMEOUT
	struct nf_ct_timeout *timeout;

	timeout = rcu_dereference(t->timeout);
	if (timeout == NULL)
		return NULL;

	return (unsigned int *)timeout->data;
#else
	return NULL;
#endif
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline
struct nf_conn_timeout *nf_ct_timeout_find(const struct nf_conn *ct)
{
#ifdef CONFIG_NF_CONNTRACK_TIMEOUT
	return nf_ct_ext_find(ct, NF_CT_EXT_TIMEOUT);
#else
	return NULL;
#endif
}

static inline
struct nf_conn_timeout *nf_ct_timeout_ext_add(struct nf_conn *ct,
<<<<<<< HEAD
					      struct ctnl_timeout *timeout,
=======
					      struct nf_ct_timeout *timeout,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					      gfp_t gfp)
{
#ifdef CONFIG_NF_CONNTRACK_TIMEOUT
	struct nf_conn_timeout *timeout_ext;

	timeout_ext = nf_ct_ext_add(ct, NF_CT_EXT_TIMEOUT, gfp);
	if (timeout_ext == NULL)
		return NULL;

<<<<<<< HEAD
	timeout_ext->timeout = timeout;
=======
	rcu_assign_pointer(timeout_ext->timeout, timeout);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return timeout_ext;
#else
	return NULL;
#endif
};

<<<<<<< HEAD
#ifdef CONFIG_NF_CONNTRACK_TIMEOUT
extern int nf_conntrack_timeout_init(struct net *net);
extern void nf_conntrack_timeout_fini(struct net *net);
#else
static inline int nf_conntrack_timeout_init(struct net *net)
{
        return 0;
}

static inline void nf_conntrack_timeout_fini(struct net *net)
{
        return;
=======
static inline unsigned int *nf_ct_timeout_lookup(const struct nf_conn *ct)
{
	unsigned int *timeouts = NULL;
#ifdef CONFIG_NF_CONNTRACK_TIMEOUT
	struct nf_conn_timeout *timeout_ext;

	timeout_ext = nf_ct_timeout_find(ct);
	if (timeout_ext)
		timeouts = nf_ct_timeout_data(timeout_ext);
#endif
	return timeouts;
}

#ifdef CONFIG_NF_CONNTRACK_TIMEOUT
void nf_ct_untimeout(struct net *net, struct nf_ct_timeout *timeout);
int nf_ct_set_timeout(struct net *net, struct nf_conn *ct, u8 l3num, u8 l4num,
		      const char *timeout_name);
void nf_ct_destroy_timeout(struct nf_conn *ct);
#else
static inline int nf_ct_set_timeout(struct net *net, struct nf_conn *ct,
				    u8 l3num, u8 l4num,
				    const char *timeout_name)
{
	return -EOPNOTSUPP;
}

static inline void nf_ct_destroy_timeout(struct nf_conn *ct)
{
	return;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
#endif /* CONFIG_NF_CONNTRACK_TIMEOUT */

#ifdef CONFIG_NF_CONNTRACK_TIMEOUT
<<<<<<< HEAD
extern struct ctnl_timeout *(*nf_ct_timeout_find_get_hook)(const char *name);
extern void (*nf_ct_timeout_put_hook)(struct ctnl_timeout *timeout);
=======
struct nf_ct_timeout_hooks {
	struct nf_ct_timeout *(*timeout_find_get)(struct net *net, const char *name);
	void (*timeout_put)(struct nf_ct_timeout *timeout);
};

extern const struct nf_ct_timeout_hooks __rcu *nf_ct_timeout_hook;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

#endif /* _NF_CONNTRACK_TIMEOUT_H */
