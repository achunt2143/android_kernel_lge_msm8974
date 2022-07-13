<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Header for use in defining a given L4 protocol for connection tracking.
 *
 * 16 Dec 2003: Yasuyuki Kozakai @USAGI <yasuyuki.kozakai@toshiba.co.jp>
 *	- generalized L3 protocol dependent part.
 *
 * Derived from include/linux/netfiter_ipv4/ip_conntrack_protcol.h
 */

#ifndef _NF_CONNTRACK_L4PROTO_H
#define _NF_CONNTRACK_L4PROTO_H
#include <linux/netlink.h>
#include <net/netlink.h>
#include <net/netfilter/nf_conntrack.h>
<<<<<<< HEAD
=======
#include <net/netns/generic.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct seq_file;

struct nf_conntrack_l4proto {
<<<<<<< HEAD
	/* L3 Protocol number. */
	u_int16_t l3proto;

	/* L4 Protocol number. */
	u_int8_t l4proto;

	/* Try to fill in the third arg: dataoff is offset past network protocol
           hdr.  Return true if possible. */
	bool (*pkt_to_tuple)(const struct sk_buff *skb, unsigned int dataoff,
			     struct nf_conntrack_tuple *tuple);

	/* Invert the per-proto part of the tuple: ie. turn xmit into reply.
	 * Some packets can't be inverted: return 0 in that case.
	 */
	bool (*invert_tuple)(struct nf_conntrack_tuple *inverse,
			     const struct nf_conntrack_tuple *orig);

	/* Returns verdict for packet, or -1 for invalid. */
	int (*packet)(struct nf_conn *ct,
		      const struct sk_buff *skb,
		      unsigned int dataoff,
		      enum ip_conntrack_info ctinfo,
		      u_int8_t pf,
		      unsigned int hooknum,
		      unsigned int *timeouts);

	/* Called when a new connection for this protocol found;
	 * returns TRUE if it's OK.  If so, packet() called next. */
	bool (*new)(struct nf_conn *ct, const struct sk_buff *skb,
		    unsigned int dataoff, unsigned int *timeouts);

	/* Called when a conntrack entry is destroyed */
	void (*destroy)(struct nf_conn *ct);

	int (*error)(struct net *net, struct nf_conn *tmpl, struct sk_buff *skb,
		     unsigned int dataoff, enum ip_conntrack_info *ctinfo,
		     u_int8_t pf, unsigned int hooknum);

	/* Print out the per-protocol part of the tuple. Return like seq_* */
	int (*print_tuple)(struct seq_file *s,
			   const struct nf_conntrack_tuple *);

	/* Print out the private part of the conntrack. */
	int (*print_conntrack)(struct seq_file *s, struct nf_conn *);

	/* Return the array of timeouts for this protocol. */
	unsigned int *(*get_timeouts)(struct net *net);

	/* convert protoinfo to nfnetink attributes */
	int (*to_nlattr)(struct sk_buff *skb, struct nlattr *nla,
			 struct nf_conn *ct);
	/* Calculate protoinfo nlattr size */
	int (*nlattr_size)(void);
=======
	/* L4 Protocol number. */
	u_int8_t l4proto;

	/* Resolve clashes on insertion races. */
	bool allow_clash;

	/* protoinfo nlattr size, closes a hole */
	u16 nlattr_size;

	/* called by gc worker if table is full */
	bool (*can_early_drop)(const struct nf_conn *ct);

	/* convert protoinfo to nfnetink attributes */
	int (*to_nlattr)(struct sk_buff *skb, struct nlattr *nla,
			 struct nf_conn *ct, bool destroy);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* convert nfnetlink attributes to protoinfo */
	int (*from_nlattr)(struct nlattr *tb[], struct nf_conn *ct);

	int (*tuple_to_nlattr)(struct sk_buff *skb,
			       const struct nf_conntrack_tuple *t);
	/* Calculate tuple nlattr size */
<<<<<<< HEAD
	int (*nlattr_tuple_size)(void);
	int (*nlattr_to_tuple)(struct nlattr *tb[],
			       struct nf_conntrack_tuple *t);
	const struct nla_policy *nla_policy;

	size_t nla_size;

#if IS_ENABLED(CONFIG_NF_CT_NETLINK_TIMEOUT)
	struct {
		size_t obj_size;
		int (*nlattr_to_obj)(struct nlattr *tb[], void *data);
		int (*obj_to_nlattr)(struct sk_buff *skb, const void *data);

		unsigned int nlattr_max;
		const struct nla_policy *nla_policy;
	} ctnl_timeout;
#endif

#ifdef CONFIG_SYSCTL
	struct ctl_table_header	**ctl_table_header;
	struct ctl_table	*ctl_table;
	unsigned int		*ctl_table_users;
#ifdef CONFIG_NF_CONNTRACK_PROC_COMPAT
	struct ctl_table_header	*ctl_compat_table_header;
	struct ctl_table	*ctl_compat_table;
#endif
#endif
	/* Protocol name */
	const char *name;

	/* Module (if any) which this is connected to. */
	struct module *me;
};

/* Existing built-in generic protocol */
extern struct nf_conntrack_l4proto nf_conntrack_l4proto_generic;

#define MAX_NF_CT_PROTO 256

extern struct nf_conntrack_l4proto *
__nf_ct_l4proto_find(u_int16_t l3proto, u_int8_t l4proto);

extern struct nf_conntrack_l4proto *
nf_ct_l4proto_find_get(u_int16_t l3proto, u_int8_t l4proto);
extern void nf_ct_l4proto_put(struct nf_conntrack_l4proto *p);

/* Protocol registration. */
extern int nf_conntrack_l4proto_register(struct nf_conntrack_l4proto *proto);
extern void nf_conntrack_l4proto_unregister(struct nf_conntrack_l4proto *proto);

/* Generic netlink helpers */
extern int nf_ct_port_tuple_to_nlattr(struct sk_buff *skb,
				      const struct nf_conntrack_tuple *tuple);
extern int nf_ct_port_nlattr_to_tuple(struct nlattr *tb[],
				      struct nf_conntrack_tuple *t);
extern int nf_ct_port_nlattr_tuple_size(void);
extern const struct nla_policy nf_ct_port_nla_policy[];

#ifdef CONFIG_SYSCTL
#ifdef DEBUG_INVALID_PACKETS
#define LOG_INVALID(net, proto)				\
	((net)->ct.sysctl_log_invalid == (proto) ||	\
	 (net)->ct.sysctl_log_invalid == IPPROTO_RAW)
#else
#define LOG_INVALID(net, proto)				\
	(((net)->ct.sysctl_log_invalid == (proto) ||	\
	  (net)->ct.sysctl_log_invalid == IPPROTO_RAW)	\
	 && net_ratelimit())
#endif
#else
static inline int LOG_INVALID(struct net *net, int proto) { return 0; }
#endif /* CONFIG_SYSCTL */

=======
	unsigned int (*nlattr_tuple_size)(void);
	int (*nlattr_to_tuple)(struct nlattr *tb[],
			       struct nf_conntrack_tuple *t,
			       u_int32_t flags);
	const struct nla_policy *nla_policy;

	struct {
		int (*nlattr_to_obj)(struct nlattr *tb[],
				     struct net *net, void *data);
		int (*obj_to_nlattr)(struct sk_buff *skb, const void *data);

		u16 obj_size;
		u16 nlattr_max;
		const struct nla_policy *nla_policy;
	} ctnl_timeout;
#ifdef CONFIG_NF_CONNTRACK_PROCFS
	/* Print out the private part of the conntrack. */
	void (*print_conntrack)(struct seq_file *s, struct nf_conn *);
#endif
};

bool icmp_pkt_to_tuple(const struct sk_buff *skb,
		       unsigned int dataoff,
		       struct net *net,
		       struct nf_conntrack_tuple *tuple);

bool icmpv6_pkt_to_tuple(const struct sk_buff *skb,
			 unsigned int dataoff,
			 struct net *net,
			 struct nf_conntrack_tuple *tuple);

bool nf_conntrack_invert_icmp_tuple(struct nf_conntrack_tuple *tuple,
				    const struct nf_conntrack_tuple *orig);
bool nf_conntrack_invert_icmpv6_tuple(struct nf_conntrack_tuple *tuple,
				      const struct nf_conntrack_tuple *orig);

int nf_conntrack_inet_error(struct nf_conn *tmpl, struct sk_buff *skb,
			    unsigned int dataoff,
			    const struct nf_hook_state *state,
			    u8 l4proto,
			    union nf_inet_addr *outer_daddr);

int nf_conntrack_icmpv4_error(struct nf_conn *tmpl,
			      struct sk_buff *skb,
			      unsigned int dataoff,
			      const struct nf_hook_state *state);

int nf_conntrack_icmpv6_error(struct nf_conn *tmpl,
			      struct sk_buff *skb,
			      unsigned int dataoff,
			      const struct nf_hook_state *state);

int nf_conntrack_icmp_packet(struct nf_conn *ct,
			     struct sk_buff *skb,
			     enum ip_conntrack_info ctinfo,
			     const struct nf_hook_state *state);

int nf_conntrack_icmpv6_packet(struct nf_conn *ct,
			       struct sk_buff *skb,
			       enum ip_conntrack_info ctinfo,
			       const struct nf_hook_state *state);

int nf_conntrack_udp_packet(struct nf_conn *ct,
			    struct sk_buff *skb,
			    unsigned int dataoff,
			    enum ip_conntrack_info ctinfo,
			    const struct nf_hook_state *state);
int nf_conntrack_udplite_packet(struct nf_conn *ct,
				struct sk_buff *skb,
				unsigned int dataoff,
				enum ip_conntrack_info ctinfo,
				const struct nf_hook_state *state);
int nf_conntrack_tcp_packet(struct nf_conn *ct,
			    struct sk_buff *skb,
			    unsigned int dataoff,
			    enum ip_conntrack_info ctinfo,
			    const struct nf_hook_state *state);
int nf_conntrack_dccp_packet(struct nf_conn *ct,
			     struct sk_buff *skb,
			     unsigned int dataoff,
			     enum ip_conntrack_info ctinfo,
			     const struct nf_hook_state *state);
int nf_conntrack_sctp_packet(struct nf_conn *ct,
			     struct sk_buff *skb,
			     unsigned int dataoff,
			     enum ip_conntrack_info ctinfo,
			     const struct nf_hook_state *state);
int nf_conntrack_gre_packet(struct nf_conn *ct,
			    struct sk_buff *skb,
			    unsigned int dataoff,
			    enum ip_conntrack_info ctinfo,
			    const struct nf_hook_state *state);

void nf_conntrack_generic_init_net(struct net *net);
void nf_conntrack_tcp_init_net(struct net *net);
void nf_conntrack_udp_init_net(struct net *net);
void nf_conntrack_gre_init_net(struct net *net);
void nf_conntrack_dccp_init_net(struct net *net);
void nf_conntrack_sctp_init_net(struct net *net);
void nf_conntrack_icmp_init_net(struct net *net);
void nf_conntrack_icmpv6_init_net(struct net *net);

/* Existing built-in generic protocol */
extern const struct nf_conntrack_l4proto nf_conntrack_l4proto_generic;

#define MAX_NF_CT_PROTO IPPROTO_UDPLITE

const struct nf_conntrack_l4proto *nf_ct_l4proto_find(u8 l4proto);

/* Generic netlink helpers */
int nf_ct_port_tuple_to_nlattr(struct sk_buff *skb,
			       const struct nf_conntrack_tuple *tuple);
int nf_ct_port_nlattr_to_tuple(struct nlattr *tb[],
			       struct nf_conntrack_tuple *t,
			       u_int32_t flags);
unsigned int nf_ct_port_nlattr_tuple_size(void);
extern const struct nla_policy nf_ct_port_nla_policy[];

#ifdef CONFIG_SYSCTL
__printf(4, 5) __cold
void nf_ct_l4proto_log_invalid(const struct sk_buff *skb,
			       const struct nf_conn *ct,
			       const struct nf_hook_state *state,
			       const char *fmt, ...);
__printf(4, 5) __cold
void nf_l4proto_log_invalid(const struct sk_buff *skb,
			    const struct nf_hook_state *state,
			    u8 protonum,
			    const char *fmt, ...);
#else
static inline __printf(4, 5) __cold
void nf_l4proto_log_invalid(const struct sk_buff *skb,
			    const struct nf_hook_state *state,
			    u8 protonum,
			    const char *fmt, ...) {}
static inline __printf(4, 5) __cold
void nf_ct_l4proto_log_invalid(const struct sk_buff *skb,
			       const struct nf_conn *ct,
			       const struct nf_hook_state *state,
			       const char *fmt, ...) { }
#endif /* CONFIG_SYSCTL */

#if IS_ENABLED(CONFIG_NF_CONNTRACK)
static inline struct nf_generic_net *nf_generic_pernet(struct net *net)
{
	return &net->ct.nf_ct_proto.generic;
}

static inline struct nf_tcp_net *nf_tcp_pernet(struct net *net)
{
	return &net->ct.nf_ct_proto.tcp;
}

static inline struct nf_udp_net *nf_udp_pernet(struct net *net)
{
	return &net->ct.nf_ct_proto.udp;
}

static inline struct nf_icmp_net *nf_icmp_pernet(struct net *net)
{
	return &net->ct.nf_ct_proto.icmp;
}

static inline struct nf_icmp_net *nf_icmpv6_pernet(struct net *net)
{
	return &net->ct.nf_ct_proto.icmpv6;
}

/* Caller must check nf_ct_protonum(ct) is IPPROTO_TCP before calling. */
static inline void nf_ct_set_tcp_be_liberal(struct nf_conn *ct)
{
	ct->proto.tcp.seen[0].flags |= IP_CT_TCP_FLAG_BE_LIBERAL;
	ct->proto.tcp.seen[1].flags |= IP_CT_TCP_FLAG_BE_LIBERAL;
}

/* Caller must check nf_ct_protonum(ct) is IPPROTO_TCP before calling. */
static inline bool nf_conntrack_tcp_established(const struct nf_conn *ct)
{
	return ct->proto.tcp.state == TCP_CONNTRACK_ESTABLISHED &&
	       test_bit(IPS_ASSURED_BIT, &ct->status);
}
#endif

#ifdef CONFIG_NF_CT_PROTO_DCCP
static inline struct nf_dccp_net *nf_dccp_pernet(struct net *net)
{
	return &net->ct.nf_ct_proto.dccp;
}
#endif

#ifdef CONFIG_NF_CT_PROTO_SCTP
static inline struct nf_sctp_net *nf_sctp_pernet(struct net *net)
{
	return &net->ct.nf_ct_proto.sctp;
}
#endif

#ifdef CONFIG_NF_CT_PROTO_GRE
static inline struct nf_gre_net *nf_gre_pernet(struct net *net)
{
	return &net->ct.nf_ct_proto.gre;
}
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /*_NF_CONNTRACK_PROTOCOL_H*/
