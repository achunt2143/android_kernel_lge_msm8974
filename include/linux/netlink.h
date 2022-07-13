<<<<<<< HEAD
#ifndef __LINUX_NETLINK_H
#define __LINUX_NETLINK_H

#include <linux/socket.h> /* for __kernel_sa_family_t */
#include <linux/types.h>

#define NETLINK_ROUTE		0	/* Routing/device hook				*/
#define NETLINK_UNUSED		1	/* Unused number				*/
#define NETLINK_USERSOCK	2	/* Reserved for user mode socket protocols 	*/
#define NETLINK_FIREWALL	3	/* Unused number, formerly ip_queue		*/
#define NETLINK_SOCK_DIAG	4	/* socket monitoring				*/
#define NETLINK_NFLOG		5	/* netfilter/iptables ULOG */
#define NETLINK_XFRM		6	/* ipsec */
#define NETLINK_SELINUX		7	/* SELinux event notifications */
#define NETLINK_ISCSI		8	/* Open-iSCSI */
#define NETLINK_AUDIT		9	/* auditing */
#define NETLINK_FIB_LOOKUP	10	
#define NETLINK_CONNECTOR	11
#define NETLINK_NETFILTER	12	/* netfilter subsystem */
#define NETLINK_IP6_FW		13
#define NETLINK_DNRTMSG		14	/* DECnet routing messages */
#define NETLINK_KOBJECT_UEVENT	15	/* Kernel messages to userspace */
#define NETLINK_GENERIC		16
/* leave room for NETLINK_DM (DM Events) */
#define NETLINK_SCSITRANSPORT	18	/* SCSI Transports */
#define NETLINK_ECRYPTFS	19
#define NETLINK_RDMA		20
#define NETLINK_CRYPTO		21	/* Crypto layer */
#define NETLINK_SOCKEV		22	/* Socket Administrative Events */

#define NETLINK_INET_DIAG	NETLINK_SOCK_DIAG

#define MAX_LINKS 32		

struct sockaddr_nl {
	__kernel_sa_family_t	nl_family;	/* AF_NETLINK	*/
	unsigned short	nl_pad;		/* zero		*/
	__u32		nl_pid;		/* port ID	*/
       	__u32		nl_groups;	/* multicast groups mask */
};

struct nlmsghdr {
	__u32		nlmsg_len;	/* Length of message including header */
	__u16		nlmsg_type;	/* Message content */
	__u16		nlmsg_flags;	/* Additional flags */
	__u32		nlmsg_seq;	/* Sequence number */
	__u32		nlmsg_pid;	/* Sending process port ID */
};

/* Flags values */

#define NLM_F_REQUEST		1	/* It is request message. 	*/
#define NLM_F_MULTI		2	/* Multipart message, terminated by NLMSG_DONE */
#define NLM_F_ACK		4	/* Reply with ack, with zero or error code */
#define NLM_F_ECHO		8	/* Echo this request 		*/
#define NLM_F_DUMP_INTR		16	/* Dump was inconsistent due to sequence change */

/* Modifiers to GET request */
#define NLM_F_ROOT	0x100	/* specify tree	root	*/
#define NLM_F_MATCH	0x200	/* return all matching	*/
#define NLM_F_ATOMIC	0x400	/* atomic GET		*/
#define NLM_F_DUMP	(NLM_F_ROOT|NLM_F_MATCH)

/* Modifiers to NEW request */
#define NLM_F_REPLACE	0x100	/* Override existing		*/
#define NLM_F_EXCL	0x200	/* Do not touch, if it exists	*/
#define NLM_F_CREATE	0x400	/* Create, if it does not exist	*/
#define NLM_F_APPEND	0x800	/* Add to end of list		*/

/*
   4.4BSD ADD		NLM_F_CREATE|NLM_F_EXCL
   4.4BSD CHANGE	NLM_F_REPLACE

   True CHANGE		NLM_F_CREATE|NLM_F_REPLACE
   Append		NLM_F_CREATE
   Check		NLM_F_EXCL
 */

#define NLMSG_ALIGNTO	4U
#define NLMSG_ALIGN(len) ( ((len)+NLMSG_ALIGNTO-1) & ~(NLMSG_ALIGNTO-1) )
#define NLMSG_HDRLEN	 ((int) NLMSG_ALIGN(sizeof(struct nlmsghdr)))
#define NLMSG_LENGTH(len) ((len)+NLMSG_ALIGN(NLMSG_HDRLEN))
#define NLMSG_SPACE(len) NLMSG_ALIGN(NLMSG_LENGTH(len))
#define NLMSG_DATA(nlh)  ((void*)(((char*)nlh) + NLMSG_LENGTH(0)))
#define NLMSG_NEXT(nlh,len)	 ((len) -= NLMSG_ALIGN((nlh)->nlmsg_len), \
				  (struct nlmsghdr*)(((char*)(nlh)) + NLMSG_ALIGN((nlh)->nlmsg_len)))
#define NLMSG_OK(nlh,len) ((len) >= (int)sizeof(struct nlmsghdr) && \
			   (nlh)->nlmsg_len >= sizeof(struct nlmsghdr) && \
			   (nlh)->nlmsg_len <= (len))
#define NLMSG_PAYLOAD(nlh,len) ((nlh)->nlmsg_len - NLMSG_SPACE((len)))

#define NLMSG_NOOP		0x1	/* Nothing.		*/
#define NLMSG_ERROR		0x2	/* Error		*/
#define NLMSG_DONE		0x3	/* End of a dump	*/
#define NLMSG_OVERRUN		0x4	/* Data lost		*/

#define NLMSG_MIN_TYPE		0x10	/* < 0x10: reserved control messages */

struct nlmsgerr {
	int		error;
	struct nlmsghdr msg;
};

#define NETLINK_ADD_MEMBERSHIP	1
#define NETLINK_DROP_MEMBERSHIP	2
#define NETLINK_PKTINFO		3
#define NETLINK_BROADCAST_ERROR	4
#define NETLINK_NO_ENOBUFS	5

struct nl_pktinfo {
	__u32	group;
};

#define NET_MAJOR 36		/* Major 36 is reserved for networking 						*/

enum {
	NETLINK_UNCONNECTED = 0,
	NETLINK_CONNECTED,
};

/*
 *  <------- NLA_HDRLEN ------> <-- NLA_ALIGN(payload)-->
 * +---------------------+- - -+- - - - - - - - - -+- - -+
 * |        Header       | Pad |     Payload       | Pad |
 * |   (struct nlattr)   | ing |                   | ing |
 * +---------------------+- - -+- - - - - - - - - -+- - -+
 *  <-------------- nlattr->nla_len -------------->
 */

struct nlattr {
	__u16           nla_len;
	__u16           nla_type;
};

/*
 * nla_type (16 bits)
 * +---+---+-------------------------------+
 * | N | O | Attribute Type                |
 * +---+---+-------------------------------+
 * N := Carries nested attributes
 * O := Payload stored in network byte order
 *
 * Note: The N and O flag are mutually exclusive.
 */
#define NLA_F_NESTED		(1 << 15)
#define NLA_F_NET_BYTEORDER	(1 << 14)
#define NLA_TYPE_MASK		~(NLA_F_NESTED | NLA_F_NET_BYTEORDER)

#define NLA_ALIGNTO		4
#define NLA_ALIGN(len)		(((len) + NLA_ALIGNTO - 1) & ~(NLA_ALIGNTO - 1))
#define NLA_HDRLEN		((int) NLA_ALIGN(sizeof(struct nlattr)))

#ifdef __KERNEL__
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LINUX_NETLINK_H
#define __LINUX_NETLINK_H

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <linux/capability.h>
#include <linux/skbuff.h>
#include <linux/export.h>
<<<<<<< HEAD

struct net;

=======
#include <net/scm.h>
#include <uapi/linux/netlink.h>

struct net;

void do_trace_netlink_extack(const char *msg);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline struct nlmsghdr *nlmsg_hdr(const struct sk_buff *skb)
{
	return (struct nlmsghdr *)skb->data;
}

<<<<<<< HEAD
struct netlink_skb_parms {
	struct ucred		creds;		/* Skb credentials	*/
	__u32			pid;
	__u32			dst_group;
=======
enum netlink_skb_flags {
	NETLINK_SKB_DST		= 0x8,	/* Dst set in sendto or sendmsg */
};

struct netlink_skb_parms {
	struct scm_creds	creds;		/* Skb credentials	*/
	__u32			portid;
	__u32			dst_group;
	__u32			flags;
	struct sock		*sk;
	bool			nsid_is_set;
	int			nsid;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

#define NETLINK_CB(skb)		(*(struct netlink_skb_parms*)&((skb)->cb))
#define NETLINK_CREDS(skb)	(&NETLINK_CB((skb)).creds)


<<<<<<< HEAD
extern void netlink_table_grab(void);
extern void netlink_table_ungrab(void);

extern struct sock *netlink_kernel_create(struct net *net,
					  int unit,unsigned int groups,
					  void (*input)(struct sk_buff *skb),
					  struct mutex *cb_mutex,
					  struct module *module);
extern void netlink_kernel_release(struct sock *sk);
extern int __netlink_change_ngroups(struct sock *sk, unsigned int groups);
extern int netlink_change_ngroups(struct sock *sk, unsigned int groups);
extern void __netlink_clear_multicast_users(struct sock *sk, unsigned int group);
extern void netlink_clear_multicast_users(struct sock *sk, unsigned int group);
extern void netlink_ack(struct sk_buff *in_skb, struct nlmsghdr *nlh, int err);
extern int netlink_has_listeners(struct sock *sk, unsigned int group);
extern int netlink_unicast(struct sock *ssk, struct sk_buff *skb, __u32 pid, int nonblock);
extern int netlink_broadcast(struct sock *ssk, struct sk_buff *skb, __u32 pid,
			     __u32 group, gfp_t allocation);
extern int netlink_broadcast_filtered(struct sock *ssk, struct sk_buff *skb,
	__u32 pid, __u32 group, gfp_t allocation,
	int (*filter)(struct sock *dsk, struct sk_buff *skb, void *data),
	void *filter_data);
extern int netlink_set_err(struct sock *ssk, __u32 pid, __u32 group, int code);
extern int netlink_register_notifier(struct notifier_block *nb);
extern int netlink_unregister_notifier(struct notifier_block *nb);
=======
void netlink_table_grab(void);
void netlink_table_ungrab(void);

#define NL_CFG_F_NONROOT_RECV	(1 << 0)
#define NL_CFG_F_NONROOT_SEND	(1 << 1)

/* optional Netlink kernel configuration parameters */
struct netlink_kernel_cfg {
	unsigned int	groups;
	unsigned int	flags;
	void		(*input)(struct sk_buff *skb);
	struct mutex	*cb_mutex;
	int		(*bind)(struct net *net, int group);
	void		(*unbind)(struct net *net, int group);
	void            (*release) (struct sock *sk, unsigned long *groups);
};

struct sock *__netlink_kernel_create(struct net *net, int unit,
					    struct module *module,
					    struct netlink_kernel_cfg *cfg);
static inline struct sock *
netlink_kernel_create(struct net *net, int unit, struct netlink_kernel_cfg *cfg)
{
	return __netlink_kernel_create(net, unit, THIS_MODULE, cfg);
}

/* this can be increased when necessary - don't expose to userland */
#define NETLINK_MAX_COOKIE_LEN	20
#define NETLINK_MAX_FMTMSG_LEN	80

/**
 * struct netlink_ext_ack - netlink extended ACK report struct
 * @_msg: message string to report - don't access directly, use
 *	%NL_SET_ERR_MSG
 * @bad_attr: attribute with error
 * @policy: policy for a bad attribute
 * @miss_type: attribute type which was missing
 * @miss_nest: nest missing an attribute (%NULL if missing top level attr)
 * @cookie: cookie data to return to userspace (for success)
 * @cookie_len: actual cookie data length
 * @_msg_buf: output buffer for formatted message strings - don't access
 *	directly, use %NL_SET_ERR_MSG_FMT
 */
struct netlink_ext_ack {
	const char *_msg;
	const struct nlattr *bad_attr;
	const struct nla_policy *policy;
	const struct nlattr *miss_nest;
	u16 miss_type;
	u8 cookie[NETLINK_MAX_COOKIE_LEN];
	u8 cookie_len;
	char _msg_buf[NETLINK_MAX_FMTMSG_LEN];
};

/* Always use this macro, this allows later putting the
 * message into a separate section or such for things
 * like translation or listing all possible messages.
 * If string formatting is needed use NL_SET_ERR_MSG_FMT.
 */
#define NL_SET_ERR_MSG(extack, msg) do {		\
	static const char __msg[] = msg;		\
	struct netlink_ext_ack *__extack = (extack);	\
							\
	do_trace_netlink_extack(__msg);			\
							\
	if (__extack)					\
		__extack->_msg = __msg;			\
} while (0)

/* We splice fmt with %s at each end even in the snprintf so that both calls
 * can use the same string constant, avoiding its duplication in .ro
 */
#define NL_SET_ERR_MSG_FMT(extack, fmt, args...) do {			       \
	struct netlink_ext_ack *__extack = (extack);			       \
									       \
	if (!__extack)							       \
		break;							       \
	if (snprintf(__extack->_msg_buf, NETLINK_MAX_FMTMSG_LEN,	       \
		     "%s" fmt "%s", "", ##args, "") >=			       \
	    NETLINK_MAX_FMTMSG_LEN)					       \
		net_warn_ratelimited("%s" fmt "%s", "truncated extack: ",      \
				     ##args, "\n");			       \
									       \
	do_trace_netlink_extack(__extack->_msg_buf);			       \
									       \
	__extack->_msg = __extack->_msg_buf;				       \
} while (0)

#define NL_SET_ERR_MSG_MOD(extack, msg)			\
	NL_SET_ERR_MSG((extack), KBUILD_MODNAME ": " msg)

#define NL_SET_ERR_MSG_FMT_MOD(extack, fmt, args...)	\
	NL_SET_ERR_MSG_FMT((extack), KBUILD_MODNAME ": " fmt, ##args)

#define NL_SET_ERR_MSG_WEAK(extack, msg) do {		\
	if ((extack) && !(extack)->_msg)		\
		NL_SET_ERR_MSG((extack), msg);		\
} while (0)

#define NL_SET_ERR_MSG_WEAK_MOD(extack, msg) do {	\
	if ((extack) && !(extack)->_msg)		\
		NL_SET_ERR_MSG_MOD((extack), msg);	\
} while (0)

#define NL_SET_BAD_ATTR_POLICY(extack, attr, pol) do {	\
	if ((extack)) {					\
		(extack)->bad_attr = (attr);		\
		(extack)->policy = (pol);		\
	}						\
} while (0)

#define NL_SET_BAD_ATTR(extack, attr) NL_SET_BAD_ATTR_POLICY(extack, attr, NULL)

#define NL_SET_ERR_MSG_ATTR_POL(extack, attr, pol, msg) do {	\
	static const char __msg[] = msg;			\
	struct netlink_ext_ack *__extack = (extack);		\
								\
	do_trace_netlink_extack(__msg);				\
								\
	if (__extack) {						\
		__extack->_msg = __msg;				\
		__extack->bad_attr = (attr);			\
		__extack->policy = (pol);			\
	}							\
} while (0)

#define NL_SET_ERR_MSG_ATTR_POL_FMT(extack, attr, pol, fmt, args...) do {	\
	struct netlink_ext_ack *__extack = (extack);				\
										\
	if (!__extack)								\
		break;								\
										\
	if (snprintf(__extack->_msg_buf, NETLINK_MAX_FMTMSG_LEN,		\
		     "%s" fmt "%s", "", ##args, "") >=				\
	    NETLINK_MAX_FMTMSG_LEN)						\
		net_warn_ratelimited("%s" fmt "%s", "truncated extack: ",       \
				     ##args, "\n");				\
										\
	do_trace_netlink_extack(__extack->_msg_buf);				\
										\
	__extack->_msg = __extack->_msg_buf;					\
	__extack->bad_attr = (attr);						\
	__extack->policy = (pol);						\
} while (0)

#define NL_SET_ERR_MSG_ATTR(extack, attr, msg)		\
	NL_SET_ERR_MSG_ATTR_POL(extack, attr, NULL, msg)

#define NL_SET_ERR_MSG_ATTR_FMT(extack, attr, msg, args...) \
	NL_SET_ERR_MSG_ATTR_POL_FMT(extack, attr, NULL, msg, ##args)

#define NL_SET_ERR_ATTR_MISS(extack, nest, type)  do {	\
	struct netlink_ext_ack *__extack = (extack);	\
							\
	if (__extack) {					\
		__extack->miss_nest = (nest);		\
		__extack->miss_type = (type);		\
	}						\
} while (0)

#define NL_REQ_ATTR_CHECK(extack, nest, tb, type) ({		\
	struct nlattr **__tb = (tb);				\
	u32 __attr = (type);					\
	int __retval;						\
								\
	__retval = !__tb[__attr];				\
	if (__retval)						\
		NL_SET_ERR_ATTR_MISS((extack), (nest), __attr);	\
	__retval;						\
})

static inline void nl_set_extack_cookie_u64(struct netlink_ext_ack *extack,
					    u64 cookie)
{
	if (!extack)
		return;
	memcpy(extack->cookie, &cookie, sizeof(cookie));
	extack->cookie_len = sizeof(cookie);
}

void netlink_kernel_release(struct sock *sk);
int __netlink_change_ngroups(struct sock *sk, unsigned int groups);
int netlink_change_ngroups(struct sock *sk, unsigned int groups);
void __netlink_clear_multicast_users(struct sock *sk, unsigned int group);
void netlink_ack(struct sk_buff *in_skb, struct nlmsghdr *nlh, int err,
		 const struct netlink_ext_ack *extack);
int netlink_has_listeners(struct sock *sk, unsigned int group);
bool netlink_strict_get_check(struct sk_buff *skb);

int netlink_unicast(struct sock *ssk, struct sk_buff *skb, __u32 portid, int nonblock);
int netlink_broadcast(struct sock *ssk, struct sk_buff *skb, __u32 portid,
		      __u32 group, gfp_t allocation);

typedef int (*netlink_filter_fn)(struct sock *dsk, struct sk_buff *skb, void *data);

int netlink_broadcast_filtered(struct sock *ssk, struct sk_buff *skb,
			       __u32 portid, __u32 group, gfp_t allocation,
			       netlink_filter_fn filter,
			       void *filter_data);
int netlink_set_err(struct sock *ssk, __u32 portid, __u32 group, int code);
int netlink_register_notifier(struct notifier_block *nb);
int netlink_unregister_notifier(struct notifier_block *nb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* finegrained unicast helpers: */
struct sock *netlink_getsockbyfilp(struct file *filp);
int netlink_attachskb(struct sock *sk, struct sk_buff *skb,
		      long *timeo, struct sock *ssk);
void netlink_detachskb(struct sock *sk, struct sk_buff *skb);
int netlink_sendskb(struct sock *sk, struct sk_buff *skb);

<<<<<<< HEAD
=======
static inline struct sk_buff *
netlink_skb_clone(struct sk_buff *skb, gfp_t gfp_mask)
{
	struct sk_buff *nskb;

	nskb = skb_clone(skb, gfp_mask);
	if (!nskb)
		return NULL;

	/* This is a large skb, set destructor callback to release head */
	if (is_vmalloc_addr(skb->head))
		nskb->destructor = skb->destructor;

	return nskb;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *	skb should fit one page. This choice is good for headerless malloc.
 *	But we should limit to 8K so that userspace does not have to
 *	use enormous buffer sizes on recvmsg() calls just to avoid
 *	MSG_TRUNC when PAGE_SIZE is very large.
 */
#if PAGE_SIZE < 8192UL
#define NLMSG_GOODSIZE	SKB_WITH_OVERHEAD(PAGE_SIZE)
#else
#define NLMSG_GOODSIZE	SKB_WITH_OVERHEAD(8192UL)
#endif

#define NLMSG_DEFAULT_SIZE (NLMSG_GOODSIZE - NLMSG_HDRLEN)


struct netlink_callback {
	struct sk_buff		*skb;
	const struct nlmsghdr	*nlh;
	int			(*dump)(struct sk_buff * skb,
					struct netlink_callback *cb);
	int			(*done)(struct netlink_callback *cb);
	void			*data;
	/* the module that dump function belong to */
	struct module		*module;
<<<<<<< HEAD
	u16			family;
	u16			min_dump_alloc;
	unsigned int		prev_seq, seq;
	long			args[6];
};

struct netlink_notify {
	struct net *net;
	int pid;
=======
	struct netlink_ext_ack	*extack;
	u16			family;
	u16			answer_flags;
	u32			min_dump_alloc;
	unsigned int		prev_seq, seq;
	int			flags;
	bool			strict_check;
	union {
		u8		ctx[48];

		/* args is deprecated. Cast a struct over ctx instead
		 * for proper type safety.
		 */
		long		args[6];
	};
};

#define NL_ASSERT_DUMP_CTX_FITS(type_name)				\
	BUILD_BUG_ON(sizeof(type_name) >				\
		     sizeof_field(struct netlink_callback, ctx))

struct netlink_notify {
	struct net *net;
	u32 portid;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int protocol;
};

struct nlmsghdr *
<<<<<<< HEAD
__nlmsg_put(struct sk_buff *skb, u32 pid, u32 seq, int type, int len, int flags);

#define NLMSG_NEW(skb, pid, seq, type, len, flags) \
({	if (unlikely(skb_tailroom(skb) < (int)NLMSG_SPACE(len))) \
		goto nlmsg_failure; \
	__nlmsg_put(skb, pid, seq, type, len, flags); })

#define NLMSG_PUT(skb, pid, seq, type, len) \
	NLMSG_NEW(skb, pid, seq, type, len, 0)

struct netlink_dump_control {
	int (*dump)(struct sk_buff *skb, struct netlink_callback *);
	int (*done)(struct netlink_callback *);
	void *data;
	struct module *module;
	u16 min_dump_alloc;
};

extern int __netlink_dump_start(struct sock *ssk, struct sk_buff *skb,
=======
__nlmsg_put(struct sk_buff *skb, u32 portid, u32 seq, int type, int len, int flags);

struct netlink_dump_control {
	int (*start)(struct netlink_callback *);
	int (*dump)(struct sk_buff *skb, struct netlink_callback *);
	int (*done)(struct netlink_callback *);
	struct netlink_ext_ack *extack;
	void *data;
	struct module *module;
	u32 min_dump_alloc;
	int flags;
};

int __netlink_dump_start(struct sock *ssk, struct sk_buff *skb,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				const struct nlmsghdr *nlh,
				struct netlink_dump_control *control);
static inline int netlink_dump_start(struct sock *ssk, struct sk_buff *skb,
				     const struct nlmsghdr *nlh,
				     struct netlink_dump_control *control)
{
	if (!control->module)
		control->module = THIS_MODULE;

	return __netlink_dump_start(ssk, skb, nlh, control);
}

<<<<<<< HEAD

#define NL_NONROOT_RECV 0x1
#define NL_NONROOT_SEND 0x2
extern void netlink_set_nonroot(int protocol, unsigned flag);

#endif /* __KERNEL__ */
=======
struct netlink_tap {
	struct net_device *dev;
	struct module *module;
	struct list_head list;
};

int netlink_add_tap(struct netlink_tap *nt);
int netlink_remove_tap(struct netlink_tap *nt);

bool __netlink_ns_capable(const struct netlink_skb_parms *nsp,
			  struct user_namespace *ns, int cap);
bool netlink_ns_capable(const struct sk_buff *skb,
			struct user_namespace *ns, int cap);
bool netlink_capable(const struct sk_buff *skb, int cap);
bool netlink_net_capable(const struct sk_buff *skb, int cap);
struct sk_buff *netlink_alloc_large_skb(unsigned int size, int broadcast);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif	/* __LINUX_NETLINK_H */
