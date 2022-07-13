<<<<<<< HEAD
#ifndef __LINUX_NETFILTER_H
#define __LINUX_NETFILTER_H

#ifdef __KERNEL__
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LINUX_NETFILTER_H
#define __LINUX_NETFILTER_H

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/init.h>
#include <linux/skbuff.h>
#include <linux/net.h>
#include <linux/if.h>
#include <linux/in.h>
#include <linux/in6.h>
#include <linux/wait.h>
#include <linux/list.h>
<<<<<<< HEAD
#endif
#include <linux/types.h>
#include <linux/compiler.h>
#include <linux/sysctl.h>

/* Responses from hook functions. */
#define NF_DROP 0
#define NF_ACCEPT 1
#define NF_STOLEN 2
#define NF_QUEUE 3
#define NF_REPEAT 4
#define NF_STOP 5
#define NF_MAX_VERDICT NF_STOP

/* we overload the higher bits for encoding auxiliary data such as the queue
 * number or errno values. Not nice, but better than additional function
 * arguments. */
#define NF_VERDICT_MASK 0x000000ff

/* extra verdict flags have mask 0x0000ff00 */
#define NF_VERDICT_FLAG_QUEUE_BYPASS	0x00008000

/* queue number (NF_QUEUE) or errno (NF_DROP) */
#define NF_VERDICT_QMASK 0xffff0000
#define NF_VERDICT_QBITS 16

#define NF_QUEUE_NR(x) ((((x) << 16) & NF_VERDICT_QMASK) | NF_QUEUE)

#define NF_DROP_ERR(x) (((-x) << 16) | NF_DROP)

/* only for userspace compatibility */
#ifndef __KERNEL__
/* Generic cache responses from hook functions.
   <= 0x2000 is used for protocol-flags. */
#define NFC_UNKNOWN 0x4000
#define NFC_ALTERED 0x8000

/* NF_VERDICT_BITS should be 8 now, but userspace might break if this changes */
#define NF_VERDICT_BITS 16
#endif

enum nf_inet_hooks {
	NF_INET_PRE_ROUTING,
	NF_INET_LOCAL_IN,
	NF_INET_FORWARD,
	NF_INET_LOCAL_OUT,
	NF_INET_POST_ROUTING,
	NF_INET_NUMHOOKS
};

enum {
	NFPROTO_UNSPEC =  0,
	NFPROTO_IPV4   =  2,
	NFPROTO_ARP    =  3,
	NFPROTO_BRIDGE =  7,
	NFPROTO_IPV6   = 10,
	NFPROTO_DECNET = 12,
	NFPROTO_NUMPROTO,
};

union nf_inet_addr {
	__u32		all[4];
	__be32		ip;
	__be32		ip6[4];
	struct in_addr	in;
	struct in6_addr	in6;
};

#ifdef __KERNEL__
#ifdef CONFIG_NETFILTER
=======
#include <linux/static_key.h>
#include <linux/module.h>
#include <linux/netfilter_defs.h>
#include <linux/netdevice.h>
#include <linux/sockptr.h>
#include <net/net_namespace.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline int NF_DROP_GETERR(int verdict)
{
	return -(verdict >> NF_VERDICT_QBITS);
}

<<<<<<< HEAD
static inline int nf_inet_addr_cmp(const union nf_inet_addr *a1,
				   const union nf_inet_addr *a2)
{
=======
static __always_inline int
NF_DROP_REASON(struct sk_buff *skb, enum skb_drop_reason reason, u32 err)
{
	BUILD_BUG_ON(err > 0xffff);

	kfree_skb_reason(skb, reason);

	return ((err << 16) | NF_STOLEN);
}

static inline int nf_inet_addr_cmp(const union nf_inet_addr *a1,
				   const union nf_inet_addr *a2)
{
#if defined(CONFIG_HAVE_EFFICIENT_UNALIGNED_ACCESS) && BITS_PER_LONG == 64
	const unsigned long *ul1 = (const unsigned long *)a1;
	const unsigned long *ul2 = (const unsigned long *)a2;

	return ((ul1[0] ^ ul2[0]) | (ul1[1] ^ ul2[1])) == 0UL;
#else
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return a1->all[0] == a2->all[0] &&
	       a1->all[1] == a2->all[1] &&
	       a1->all[2] == a2->all[2] &&
	       a1->all[3] == a2->all[3];
<<<<<<< HEAD
}

extern void netfilter_init(void);

/* Largest hook number + 1 */
#define NF_MAX_HOOKS 8

struct sk_buff;

typedef unsigned int nf_hookfn(unsigned int hooknum,
			       struct sk_buff *skb,
			       const struct net_device *in,
			       const struct net_device *out,
			       int (*okfn)(struct sk_buff *));

struct nf_hook_ops {
	struct list_head list;

	/* User fills in from here down. */
	nf_hookfn *hook;
	struct module *owner;
	u_int8_t pf;
	unsigned int hooknum;
	/* Hooks are ordered in ascending priority. */
	int priority;
};

=======
#endif
}

static inline void nf_inet_addr_mask(const union nf_inet_addr *a1,
				     union nf_inet_addr *result,
				     const union nf_inet_addr *mask)
{
#if defined(CONFIG_HAVE_EFFICIENT_UNALIGNED_ACCESS) && BITS_PER_LONG == 64
	const unsigned long *ua = (const unsigned long *)a1;
	unsigned long *ur = (unsigned long *)result;
	const unsigned long *um = (const unsigned long *)mask;

	ur[0] = ua[0] & um[0];
	ur[1] = ua[1] & um[1];
#else
	result->all[0] = a1->all[0] & mask->all[0];
	result->all[1] = a1->all[1] & mask->all[1];
	result->all[2] = a1->all[2] & mask->all[2];
	result->all[3] = a1->all[3] & mask->all[3];
#endif
}

int netfilter_init(void);

struct sk_buff;

struct nf_hook_ops;

struct sock;

struct nf_hook_state {
	u8 hook;
	u8 pf;
	struct net_device *in;
	struct net_device *out;
	struct sock *sk;
	struct net *net;
	int (*okfn)(struct net *, struct sock *, struct sk_buff *);
};

typedef unsigned int nf_hookfn(void *priv,
			       struct sk_buff *skb,
			       const struct nf_hook_state *state);
enum nf_hook_ops_type {
	NF_HOOK_OP_UNDEFINED,
	NF_HOOK_OP_NF_TABLES,
	NF_HOOK_OP_BPF,
};

struct nf_hook_ops {
	/* User fills in from here down. */
	nf_hookfn		*hook;
	struct net_device	*dev;
	void			*priv;
	u8			pf;
	enum nf_hook_ops_type	hook_ops_type:8;
	unsigned int		hooknum;
	/* Hooks are ordered in ascending priority. */
	int			priority;
};

struct nf_hook_entry {
	nf_hookfn			*hook;
	void				*priv;
};

struct nf_hook_entries_rcu_head {
	struct rcu_head head;
	void	*allocation;
};

struct nf_hook_entries {
	u16				num_hook_entries;
	/* padding */
	struct nf_hook_entry		hooks[];

	/* trailer: pointers to original orig_ops of each hook,
	 * followed by rcu_head and scratch space used for freeing
	 * the structure via call_rcu.
	 *
	 *   This is not part of struct nf_hook_entry since its only
	 *   needed in slow path (hook register/unregister):
	 * const struct nf_hook_ops     *orig_ops[]
	 *
	 *   For the same reason, we store this at end -- its
	 *   only needed when a hook is deleted, not during
	 *   packet path processing:
	 * struct nf_hook_entries_rcu_head     head
	 */
};

#ifdef CONFIG_NETFILTER
static inline struct nf_hook_ops **nf_hook_entries_get_hook_ops(const struct nf_hook_entries *e)
{
	unsigned int n = e->num_hook_entries;
	const void *hook_end;

	hook_end = &e->hooks[n]; /* this is *past* ->hooks[]! */

	return (struct nf_hook_ops **)hook_end;
}

static inline int
nf_hook_entry_hookfn(const struct nf_hook_entry *entry, struct sk_buff *skb,
		     struct nf_hook_state *state)
{
	return entry->hook(entry->priv, skb, state);
}

static inline void nf_hook_state_init(struct nf_hook_state *p,
				      unsigned int hook,
				      u_int8_t pf,
				      struct net_device *indev,
				      struct net_device *outdev,
				      struct sock *sk,
				      struct net *net,
				      int (*okfn)(struct net *, struct sock *, struct sk_buff *))
{
	p->hook = hook;
	p->pf = pf;
	p->in = indev;
	p->out = outdev;
	p->sk = sk;
	p->net = net;
	p->okfn = okfn;
}



>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct nf_sockopt_ops {
	struct list_head list;

	u_int8_t pf;

	/* Non-inclusive ranges: use 0/0/NULL to never get called. */
	int set_optmin;
	int set_optmax;
<<<<<<< HEAD
	int (*set)(struct sock *sk, int optval, void __user *user, unsigned int len);
#ifdef CONFIG_COMPAT
	int (*compat_set)(struct sock *sk, int optval,
			void __user *user, unsigned int len);
#endif
	int get_optmin;
	int get_optmax;
	int (*get)(struct sock *sk, int optval, void __user *user, int *len);
#ifdef CONFIG_COMPAT
	int (*compat_get)(struct sock *sk, int optval,
			void __user *user, int *len);
#endif
=======
	int (*set)(struct sock *sk, int optval, sockptr_t arg,
		   unsigned int len);
	int get_optmin;
	int get_optmax;
	int (*get)(struct sock *sk, int optval, void __user *user, int *len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Use the module struct to lock set/get code in place */
	struct module *owner;
};

/* Function to register/unregister hook points. */
<<<<<<< HEAD
int nf_register_hook(struct nf_hook_ops *reg);
void nf_unregister_hook(struct nf_hook_ops *reg);
int nf_register_hooks(struct nf_hook_ops *reg, unsigned int n);
void nf_unregister_hooks(struct nf_hook_ops *reg, unsigned int n);
=======
int nf_register_net_hook(struct net *net, const struct nf_hook_ops *ops);
void nf_unregister_net_hook(struct net *net, const struct nf_hook_ops *ops);
int nf_register_net_hooks(struct net *net, const struct nf_hook_ops *reg,
			  unsigned int n);
void nf_unregister_net_hooks(struct net *net, const struct nf_hook_ops *reg,
			     unsigned int n);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Functions to register get/setsockopt ranges (non-inclusive).  You
   need to check permissions yourself! */
int nf_register_sockopt(struct nf_sockopt_ops *reg);
void nf_unregister_sockopt(struct nf_sockopt_ops *reg);

<<<<<<< HEAD
#ifdef CONFIG_SYSCTL
/* Sysctl registration */
extern struct ctl_path nf_net_netfilter_sysctl_path[];
extern struct ctl_path nf_net_ipv4_netfilter_sysctl_path[];
#endif /* CONFIG_SYSCTL */

extern struct list_head nf_hooks[NFPROTO_NUMPROTO][NF_MAX_HOOKS];

#if defined(CONFIG_JUMP_LABEL)
#include <linux/static_key.h>
extern struct static_key nf_hooks_needed[NFPROTO_NUMPROTO][NF_MAX_HOOKS];
static inline bool nf_hooks_active(u_int8_t pf, unsigned int hook)
{
	if (__builtin_constant_p(pf) &&
	    __builtin_constant_p(hook))
		return static_key_false(&nf_hooks_needed[pf][hook]);

	return !list_empty(&nf_hooks[pf][hook]);
}
#else
static inline bool nf_hooks_active(u_int8_t pf, unsigned int hook)
{
	return !list_empty(&nf_hooks[pf][hook]);
}
#endif

int nf_hook_slow(u_int8_t pf, unsigned int hook, struct sk_buff *skb,
		 struct net_device *indev, struct net_device *outdev,
		 int (*okfn)(struct sk_buff *), int thresh);

/**
 *	nf_hook_thresh - call a netfilter hook
 *	
=======
#ifdef CONFIG_JUMP_LABEL
extern struct static_key nf_hooks_needed[NFPROTO_NUMPROTO][NF_MAX_HOOKS];
#endif

int nf_hook_slow(struct sk_buff *skb, struct nf_hook_state *state,
		 const struct nf_hook_entries *e, unsigned int i);

void nf_hook_slow_list(struct list_head *head, struct nf_hook_state *state,
		       const struct nf_hook_entries *e);
/**
 *	nf_hook - call a netfilter hook
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *	Returns 1 if the hook has allowed the packet to pass.  The function
 *	okfn must be invoked by the caller in this case.  Any other return
 *	value indicates the packet has been consumed by the hook.
 */
<<<<<<< HEAD
static inline int nf_hook_thresh(u_int8_t pf, unsigned int hook,
				 struct sk_buff *skb,
				 struct net_device *indev,
				 struct net_device *outdev,
				 int (*okfn)(struct sk_buff *), int thresh)
{
	if (nf_hooks_active(pf, hook))
		return nf_hook_slow(pf, hook, skb, indev, outdev, okfn, thresh);
	return 1;
}

static inline int nf_hook(u_int8_t pf, unsigned int hook, struct sk_buff *skb,
			  struct net_device *indev, struct net_device *outdev,
			  int (*okfn)(struct sk_buff *))
{
	return nf_hook_thresh(pf, hook, skb, indev, outdev, okfn, INT_MIN);
}
                   
=======
static inline int nf_hook(u_int8_t pf, unsigned int hook, struct net *net,
			  struct sock *sk, struct sk_buff *skb,
			  struct net_device *indev, struct net_device *outdev,
			  int (*okfn)(struct net *, struct sock *, struct sk_buff *))
{
	struct nf_hook_entries *hook_head = NULL;
	int ret = 1;

#ifdef CONFIG_JUMP_LABEL
	if (__builtin_constant_p(pf) &&
	    __builtin_constant_p(hook) &&
	    !static_key_false(&nf_hooks_needed[pf][hook]))
		return 1;
#endif

	rcu_read_lock();
	switch (pf) {
	case NFPROTO_IPV4:
		hook_head = rcu_dereference(net->nf.hooks_ipv4[hook]);
		break;
	case NFPROTO_IPV6:
		hook_head = rcu_dereference(net->nf.hooks_ipv6[hook]);
		break;
	case NFPROTO_ARP:
#ifdef CONFIG_NETFILTER_FAMILY_ARP
		if (WARN_ON_ONCE(hook >= ARRAY_SIZE(net->nf.hooks_arp)))
			break;
		hook_head = rcu_dereference(net->nf.hooks_arp[hook]);
#endif
		break;
	case NFPROTO_BRIDGE:
#ifdef CONFIG_NETFILTER_FAMILY_BRIDGE
		hook_head = rcu_dereference(net->nf.hooks_bridge[hook]);
#endif
		break;
	default:
		WARN_ON_ONCE(1);
		break;
	}

	if (hook_head) {
		struct nf_hook_state state;

		nf_hook_state_init(&state, hook, pf, indev, outdev,
				   sk, net, okfn);

		ret = nf_hook_slow(skb, &state, hook_head, 0);
	}
	rcu_read_unlock();

	return ret;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Activate hook; either okfn or kfree_skb called, unless a hook
   returns NF_STOLEN (in which case, it's up to the hook to deal with
   the consequences).

   Returns -ERRNO if packet dropped.  Zero means queued, stolen or
   accepted.
*/

/* RR:
   > I don't want nf_hook to return anything because people might forget
   > about async and trust the return value to mean "packet was ok".

   AK:
   Just document it clearly, then you can expect some sense from kernel
   coders :)
*/

static inline int
<<<<<<< HEAD
NF_HOOK_THRESH(uint8_t pf, unsigned int hook, struct sk_buff *skb,
	       struct net_device *in, struct net_device *out,
	       int (*okfn)(struct sk_buff *), int thresh)
{
	int ret = nf_hook_thresh(pf, hook, skb, in, out, okfn, thresh);
	if (ret == 1)
		ret = okfn(skb);
	return ret;
}

static inline int
NF_HOOK_COND(uint8_t pf, unsigned int hook, struct sk_buff *skb,
	     struct net_device *in, struct net_device *out,
	     int (*okfn)(struct sk_buff *), bool cond)
=======
NF_HOOK_COND(uint8_t pf, unsigned int hook, struct net *net, struct sock *sk,
	     struct sk_buff *skb, struct net_device *in, struct net_device *out,
	     int (*okfn)(struct net *, struct sock *, struct sk_buff *),
	     bool cond)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int ret;

	if (!cond ||
<<<<<<< HEAD
	    ((ret = nf_hook_thresh(pf, hook, skb, in, out, okfn, INT_MIN)) == 1))
		ret = okfn(skb);
=======
	    ((ret = nf_hook(pf, hook, net, sk, skb, in, out, okfn)) == 1))
		ret = okfn(net, sk, skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

static inline int
<<<<<<< HEAD
NF_HOOK(uint8_t pf, unsigned int hook, struct sk_buff *skb,
	struct net_device *in, struct net_device *out,
	int (*okfn)(struct sk_buff *))
{
	return NF_HOOK_THRESH(pf, hook, skb, in, out, okfn, INT_MIN);
}

/* Call setsockopt() */
int nf_setsockopt(struct sock *sk, u_int8_t pf, int optval, char __user *opt,
		  unsigned int len);
int nf_getsockopt(struct sock *sk, u_int8_t pf, int optval, char __user *opt,
		  int *len);
#ifdef CONFIG_COMPAT
int compat_nf_setsockopt(struct sock *sk, u_int8_t pf, int optval,
		char __user *opt, unsigned int len);
int compat_nf_getsockopt(struct sock *sk, u_int8_t pf, int optval,
		char __user *opt, int *len);
#endif

/* Call this before modifying an existing packet: ensures it is
   modifiable and linear to the point you care about (writable_len).
   Returns true or false. */
extern int skb_make_writable(struct sk_buff *skb, unsigned int writable_len);
=======
NF_HOOK(uint8_t pf, unsigned int hook, struct net *net, struct sock *sk, struct sk_buff *skb,
	struct net_device *in, struct net_device *out,
	int (*okfn)(struct net *, struct sock *, struct sk_buff *))
{
	int ret = nf_hook(pf, hook, net, sk, skb, in, out, okfn);
	if (ret == 1)
		ret = okfn(net, sk, skb);
	return ret;
}

static inline void
NF_HOOK_LIST(uint8_t pf, unsigned int hook, struct net *net, struct sock *sk,
	     struct list_head *head, struct net_device *in, struct net_device *out,
	     int (*okfn)(struct net *, struct sock *, struct sk_buff *))
{
	struct nf_hook_entries *hook_head = NULL;

#ifdef CONFIG_JUMP_LABEL
	if (__builtin_constant_p(pf) &&
	    __builtin_constant_p(hook) &&
	    !static_key_false(&nf_hooks_needed[pf][hook]))
		return;
#endif

	rcu_read_lock();
	switch (pf) {
	case NFPROTO_IPV4:
		hook_head = rcu_dereference(net->nf.hooks_ipv4[hook]);
		break;
	case NFPROTO_IPV6:
		hook_head = rcu_dereference(net->nf.hooks_ipv6[hook]);
		break;
	default:
		WARN_ON_ONCE(1);
		break;
	}

	if (hook_head) {
		struct nf_hook_state state;

		nf_hook_state_init(&state, hook, pf, in, out, sk, net, okfn);

		nf_hook_slow_list(head, &state, hook_head);
	}
	rcu_read_unlock();
}

/* Call setsockopt() */
int nf_setsockopt(struct sock *sk, u_int8_t pf, int optval, sockptr_t opt,
		  unsigned int len);
int nf_getsockopt(struct sock *sk, u_int8_t pf, int optval, char __user *opt,
		  int *len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct flowi;
struct nf_queue_entry;

<<<<<<< HEAD
struct nf_afinfo {
	unsigned short	family;
	__sum16		(*checksum)(struct sk_buff *skb, unsigned int hook,
				    unsigned int dataoff, u_int8_t protocol);
	__sum16		(*checksum_partial)(struct sk_buff *skb,
					    unsigned int hook,
					    unsigned int dataoff,
					    unsigned int len,
					    u_int8_t protocol);
	int		(*route)(struct net *net, struct dst_entry **dst,
				 struct flowi *fl, bool strict);
	void		(*saveroute)(const struct sk_buff *skb,
				     struct nf_queue_entry *entry);
	int		(*reroute)(struct sk_buff *skb,
				   const struct nf_queue_entry *entry);
	int		route_key_size;
};

extern const struct nf_afinfo __rcu *nf_afinfo[NFPROTO_NUMPROTO];
static inline const struct nf_afinfo *nf_get_afinfo(unsigned short family)
{
	return rcu_dereference(nf_afinfo[family]);
}

static inline __sum16
nf_checksum(struct sk_buff *skb, unsigned int hook, unsigned int dataoff,
	    u_int8_t protocol, unsigned short family)
{
	const struct nf_afinfo *afinfo;
	__sum16 csum = 0;

	rcu_read_lock();
	afinfo = nf_get_afinfo(family);
	if (afinfo)
		csum = afinfo->checksum(skb, hook, dataoff, protocol);
	rcu_read_unlock();
	return csum;
}

static inline __sum16
nf_checksum_partial(struct sk_buff *skb, unsigned int hook,
		    unsigned int dataoff, unsigned int len,
		    u_int8_t protocol, unsigned short family)
{
	const struct nf_afinfo *afinfo;
	__sum16 csum = 0;

	rcu_read_lock();
	afinfo = nf_get_afinfo(family);
	if (afinfo)
		csum = afinfo->checksum_partial(skb, hook, dataoff, len,
						protocol);
	rcu_read_unlock();
	return csum;
}

extern int nf_register_afinfo(const struct nf_afinfo *afinfo);
extern void nf_unregister_afinfo(const struct nf_afinfo *afinfo);

#include <net/flow.h>
extern void (*nf_nat_decode_session_hook)(struct sk_buff *, struct flowi *);
=======
__sum16 nf_checksum(struct sk_buff *skb, unsigned int hook,
		    unsigned int dataoff, u_int8_t protocol,
		    unsigned short family);

__sum16 nf_checksum_partial(struct sk_buff *skb, unsigned int hook,
			    unsigned int dataoff, unsigned int len,
			    u_int8_t protocol, unsigned short family);
int nf_route(struct net *net, struct dst_entry **dst, struct flowi *fl,
	     bool strict, unsigned short family);

#include <net/flow.h>

struct nf_conn;
enum nf_nat_manip_type;
struct nlattr;
enum ip_conntrack_dir;

struct nf_nat_hook {
	int (*parse_nat_setup)(struct nf_conn *ct, enum nf_nat_manip_type manip,
			       const struct nlattr *attr);
	void (*decode_session)(struct sk_buff *skb, struct flowi *fl);
	unsigned int (*manip_pkt)(struct sk_buff *skb, struct nf_conn *ct,
				  enum nf_nat_manip_type mtype,
				  enum ip_conntrack_dir dir);
	void (*remove_nat_bysrc)(struct nf_conn *ct);
};

extern const struct nf_nat_hook __rcu *nf_nat_hook;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline void
nf_nat_decode_session(struct sk_buff *skb, struct flowi *fl, u_int8_t family)
{
<<<<<<< HEAD
#ifdef CONFIG_NF_NAT_NEEDED
	void (*decodefn)(struct sk_buff *, struct flowi *);

	rcu_read_lock();
	decodefn = rcu_dereference(nf_nat_decode_session_hook);
	if (decodefn)
		decodefn(skb, fl);
=======
#if IS_ENABLED(CONFIG_NF_NAT)
	const struct nf_nat_hook *nat_hook;

	rcu_read_lock();
	nat_hook = rcu_dereference(nf_nat_hook);
	if (nat_hook && nat_hook->decode_session)
		nat_hook->decode_session(skb, fl);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rcu_read_unlock();
#endif
}

<<<<<<< HEAD
#ifdef CONFIG_PROC_FS
#include <linux/proc_fs.h>
extern struct proc_dir_entry *proc_net_netfilter;
#endif

#else /* !CONFIG_NETFILTER */
#define NF_HOOK(pf, hook, skb, indev, outdev, okfn) (okfn)(skb)
#define NF_HOOK_COND(pf, hook, skb, indev, outdev, okfn, cond) (okfn)(skb)
static inline int nf_hook_thresh(u_int8_t pf, unsigned int hook,
				 struct sk_buff *skb,
				 struct net_device *indev,
				 struct net_device *outdev,
				 int (*okfn)(struct sk_buff *), int thresh)
{
	return okfn(skb);
}
static inline int nf_hook(u_int8_t pf, unsigned int hook, struct sk_buff *skb,
			  struct net_device *indev, struct net_device *outdev,
			  int (*okfn)(struct sk_buff *))
=======
#else /* !CONFIG_NETFILTER */
static inline int
NF_HOOK_COND(uint8_t pf, unsigned int hook, struct net *net, struct sock *sk,
	     struct sk_buff *skb, struct net_device *in, struct net_device *out,
	     int (*okfn)(struct net *, struct sock *, struct sk_buff *),
	     bool cond)
{
	return okfn(net, sk, skb);
}

static inline int
NF_HOOK(uint8_t pf, unsigned int hook, struct net *net, struct sock *sk,
	struct sk_buff *skb, struct net_device *in, struct net_device *out,
	int (*okfn)(struct net *, struct sock *, struct sk_buff *))
{
	return okfn(net, sk, skb);
}

static inline void
NF_HOOK_LIST(uint8_t pf, unsigned int hook, struct net *net, struct sock *sk,
	     struct list_head *head, struct net_device *in, struct net_device *out,
	     int (*okfn)(struct net *, struct sock *, struct sk_buff *))
{
	/* nothing to do */
}

static inline int nf_hook(u_int8_t pf, unsigned int hook, struct net *net,
			  struct sock *sk, struct sk_buff *skb,
			  struct net_device *indev, struct net_device *outdev,
			  int (*okfn)(struct net *, struct sock *, struct sk_buff *))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return 1;
}
struct flowi;
static inline void
nf_nat_decode_session(struct sk_buff *skb, struct flowi *fl, u_int8_t family)
{
}
#endif /*CONFIG_NETFILTER*/

<<<<<<< HEAD
#if defined(CONFIG_NF_CONNTRACK) || defined(CONFIG_NF_CONNTRACK_MODULE)
extern void (*ip_ct_attach)(struct sk_buff *, struct sk_buff *) __rcu;
extern void nf_ct_attach(struct sk_buff *, struct sk_buff *);
extern void (*nf_ct_destroy)(struct nf_conntrack *) __rcu;

struct nf_conn;
struct nlattr;

struct nfq_ct_hook {
	size_t (*build_size)(const struct nf_conn *ct);
	int (*build)(struct sk_buff *skb, struct nf_conn *ct);
	int (*parse)(const struct nlattr *attr, struct nf_conn *ct);
};
extern struct nfq_ct_hook __rcu *nfq_ct_hook;

struct nfq_ct_nat_hook {
	void (*seq_adjust)(struct sk_buff *skb, struct nf_conn *ct,
			   u32 ctinfo, int off);
};
extern struct nfq_ct_nat_hook __rcu *nfq_ct_nat_hook;
#else
static inline void nf_ct_attach(struct sk_buff *new, struct sk_buff *skb) {}
#endif

#endif /*__KERNEL__*/
=======
#if IS_ENABLED(CONFIG_NF_CONNTRACK)
#include <linux/netfilter/nf_conntrack_zones_common.h>

void nf_ct_attach(struct sk_buff *, const struct sk_buff *);
void nf_ct_set_closing(struct nf_conntrack *nfct);
struct nf_conntrack_tuple;
bool nf_ct_get_tuple_skb(struct nf_conntrack_tuple *dst_tuple,
			 const struct sk_buff *skb);
#else
static inline void nf_ct_attach(struct sk_buff *new, struct sk_buff *skb) {}
static inline void nf_ct_set_closing(struct nf_conntrack *nfct) {}
struct nf_conntrack_tuple;
static inline bool nf_ct_get_tuple_skb(struct nf_conntrack_tuple *dst_tuple,
				       const struct sk_buff *skb)
{
	return false;
}
#endif

struct nf_conn;
enum ip_conntrack_info;

struct nf_ct_hook {
	int (*update)(struct net *net, struct sk_buff *skb);
	void (*destroy)(struct nf_conntrack *);
	bool (*get_tuple_skb)(struct nf_conntrack_tuple *,
			      const struct sk_buff *);
	void (*attach)(struct sk_buff *nskb, const struct sk_buff *skb);
	void (*set_closing)(struct nf_conntrack *nfct);
	int (*confirm)(struct sk_buff *skb);
};
extern const struct nf_ct_hook __rcu *nf_ct_hook;

struct nlattr;

struct nfnl_ct_hook {
	size_t (*build_size)(const struct nf_conn *ct);
	int (*build)(struct sk_buff *skb, struct nf_conn *ct,
		     enum ip_conntrack_info ctinfo,
		     u_int16_t ct_attr, u_int16_t ct_info_attr);
	int (*parse)(const struct nlattr *attr, struct nf_conn *ct);
	int (*attach_expect)(const struct nlattr *attr, struct nf_conn *ct,
			     u32 portid, u32 report);
	void (*seq_adjust)(struct sk_buff *skb, struct nf_conn *ct,
			   enum ip_conntrack_info ctinfo, s32 off);
};
extern const struct nfnl_ct_hook __rcu *nfnl_ct_hook;

struct nf_defrag_hook {
	struct module *owner;
	int (*enable)(struct net *net);
	void (*disable)(struct net *net);
};

extern const struct nf_defrag_hook __rcu *nf_defrag_v4_hook;
extern const struct nf_defrag_hook __rcu *nf_defrag_v6_hook;

/*
 * nf_skb_duplicated - TEE target has sent a packet
 *
 * When a xtables target sends a packet, the OUTPUT and POSTROUTING
 * hooks are traversed again, i.e. nft and xtables are invoked recursively.
 *
 * This is used by xtables TEE target to prevent the duplicated skb from
 * being duplicated again.
 */
DECLARE_PER_CPU(bool, nf_skb_duplicated);

/*
 * Contains bitmask of ctnetlink event subscribers, if any.
 * Can't be pernet due to NETLINK_LISTEN_ALL_NSID setsockopt flag.
 */
extern u8 nf_ctnetlink_has_listener;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /*__LINUX_NETFILTER_H*/
