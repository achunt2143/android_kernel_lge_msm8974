<<<<<<< HEAD
#ifndef _INET_DIAG_H_
#define _INET_DIAG_H_ 1

#include <linux/types.h>

/* Just some random number */
#define TCPDIAG_GETSOCK 18
#define DCCPDIAG_GETSOCK 19

#define INET_DIAG_GETSOCK_MAX 24

/* Socket identity */
struct inet_diag_sockid {
	__be16	idiag_sport;
	__be16	idiag_dport;
	__be32	idiag_src[4];
	__be32	idiag_dst[4];
	__u32	idiag_if;
	__u32	idiag_cookie[2];
#define INET_DIAG_NOCOOKIE (~0U)
};

/* Request structure */

struct inet_diag_req {
	__u8	idiag_family;		/* Family of addresses. */
	__u8	idiag_src_len;
	__u8	idiag_dst_len;
	__u8	idiag_ext;		/* Query extended information */

	struct inet_diag_sockid id;

	__u32	idiag_states;		/* States to dump */
	__u32	idiag_dbs;		/* Tables to dump (NI) */
};

struct inet_diag_req_v2 {
	__u8	sdiag_family;
	__u8	sdiag_protocol;
	__u8	idiag_ext;
	__u8	pad;
	__u32	idiag_states;
	struct inet_diag_sockid id;
};

enum {
	INET_DIAG_REQ_NONE,
	INET_DIAG_REQ_BYTECODE,
};

#define INET_DIAG_REQ_MAX INET_DIAG_REQ_BYTECODE

/* Bytecode is sequence of 4 byte commands followed by variable arguments.
 * All the commands identified by "code" are conditional jumps forward:
 * to offset cc+"yes" or to offset cc+"no". "yes" is supposed to be
 * length of the command and its arguments.
 */
 
struct inet_diag_bc_op {
	unsigned char	code;
	unsigned char	yes;
	unsigned short	no;
};

enum {
	INET_DIAG_BC_NOP,
	INET_DIAG_BC_JMP,
	INET_DIAG_BC_S_GE,
	INET_DIAG_BC_S_LE,
	INET_DIAG_BC_D_GE,
	INET_DIAG_BC_D_LE,
	INET_DIAG_BC_AUTO,
	INET_DIAG_BC_S_COND,
	INET_DIAG_BC_D_COND,
};

struct inet_diag_hostcond {
	__u8	family;
	__u8	prefix_len;
	int	port;
	__be32	addr[0];
};

/* Base info structure. It contains socket identity (addrs/ports/cookie)
 * and, alas, the information shown by netstat. */
struct inet_diag_msg {
	__u8	idiag_family;
	__u8	idiag_state;
	__u8	idiag_timer;
	__u8	idiag_retrans;

	struct inet_diag_sockid id;

	__u32	idiag_expires;
	__u32	idiag_rqueue;
	__u32	idiag_wqueue;
	__u32	idiag_uid;
	__u32	idiag_inode;
};

/* Extensions */

enum {
	INET_DIAG_NONE,
	INET_DIAG_MEMINFO,
	INET_DIAG_INFO,
	INET_DIAG_VEGASINFO,
	INET_DIAG_CONG,
	INET_DIAG_TOS,
	INET_DIAG_TCLASS,
	INET_DIAG_SKMEMINFO,
};

#define INET_DIAG_MAX INET_DIAG_SKMEMINFO


/* INET_DIAG_MEM */

struct inet_diag_meminfo {
	__u32	idiag_rmem;
	__u32	idiag_wmem;
	__u32	idiag_fmem;
	__u32	idiag_tmem;
};

/* INET_DIAG_VEGASINFO */

struct tcpvegas_info {
	__u32	tcpv_enabled;
	__u32	tcpv_rttcnt;
	__u32	tcpv_rtt;
	__u32	tcpv_minrtt;
};

#ifdef __KERNEL__
struct sock;
struct inet_hashinfo;
struct nlattr;
struct nlmsghdr;
struct sk_buff;
struct netlink_callback;

struct inet_diag_handler {
	void			(*dump)(struct sk_buff *skb,
					struct netlink_callback *cb,
					struct inet_diag_req_v2 *r,
					struct nlattr *bc);

	int			(*dump_one)(struct sk_buff *in_skb,
					const struct nlmsghdr *nlh,
					struct inet_diag_req_v2 *req);

	void			(*idiag_get_info)(struct sock *sk,
						  struct inet_diag_msg *r,
						  void *info);
	__u16                   idiag_type;
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _INET_DIAG_H_
#define _INET_DIAG_H_ 1

#include <net/netlink.h>
#include <uapi/linux/inet_diag.h>

struct inet_hashinfo;

struct inet_diag_handler {
	struct module	*owner;
	void		(*dump)(struct sk_buff *skb,
				struct netlink_callback *cb,
				const struct inet_diag_req_v2 *r);

	int		(*dump_one)(struct netlink_callback *cb,
				    const struct inet_diag_req_v2 *req);

	void		(*idiag_get_info)(struct sock *sk,
					  struct inet_diag_msg *r,
					  void *info);

	int		(*idiag_get_aux)(struct sock *sk,
					 bool net_admin,
					 struct sk_buff *skb);

	size_t		(*idiag_get_aux_size)(struct sock *sk,
					      bool net_admin);

	int		(*destroy)(struct sk_buff *in_skb,
				   const struct inet_diag_req_v2 *req);

	__u16		idiag_type;
	__u16		idiag_info_size;
};

struct bpf_sk_storage_diag;
struct inet_diag_dump_data {
	struct nlattr *req_nlas[__INET_DIAG_REQ_MAX];
#define inet_diag_nla_bc req_nlas[INET_DIAG_REQ_BYTECODE]
#define inet_diag_nla_bpf_stgs req_nlas[INET_DIAG_REQ_SK_BPF_STORAGES]

	struct bpf_sk_storage_diag *bpf_stg_diag;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct inet_connection_sock;
int inet_sk_diag_fill(struct sock *sk, struct inet_connection_sock *icsk,
<<<<<<< HEAD
			      struct sk_buff *skb, struct inet_diag_req_v2 *req,
			      u32 pid, u32 seq, u16 nlmsg_flags,
			      const struct nlmsghdr *unlh);
void inet_diag_dump_icsk(struct inet_hashinfo *h, struct sk_buff *skb,
		struct netlink_callback *cb, struct inet_diag_req_v2 *r,
		struct nlattr *bc);
int inet_diag_dump_one_icsk(struct inet_hashinfo *hashinfo,
		struct sk_buff *in_skb, const struct nlmsghdr *nlh,
		struct inet_diag_req_v2 *req);

int inet_diag_bc_sk(const struct nlattr *_bc, struct sock *sk);

extern int  inet_diag_register(const struct inet_diag_handler *handler);
extern void inet_diag_unregister(const struct inet_diag_handler *handler);
#endif /* __KERNEL__ */

=======
		      struct sk_buff *skb, struct netlink_callback *cb,
		      const struct inet_diag_req_v2 *req,
		      u16 nlmsg_flags, bool net_admin);
void inet_diag_dump_icsk(struct inet_hashinfo *h, struct sk_buff *skb,
			 struct netlink_callback *cb,
			 const struct inet_diag_req_v2 *r);
int inet_diag_dump_one_icsk(struct inet_hashinfo *hashinfo,
			    struct netlink_callback *cb,
			    const struct inet_diag_req_v2 *req);

struct sock *inet_diag_find_one_icsk(struct net *net,
				     struct inet_hashinfo *hashinfo,
				     const struct inet_diag_req_v2 *req);

int inet_diag_bc_sk(const struct nlattr *_bc, struct sock *sk);

void inet_diag_msg_common_fill(struct inet_diag_msg *r, struct sock *sk);

static inline size_t inet_diag_msg_attrs_size(void)
{
	return	  nla_total_size(1)  /* INET_DIAG_SHUTDOWN */
		+ nla_total_size(1)  /* INET_DIAG_TOS */
#if IS_ENABLED(CONFIG_IPV6)
		+ nla_total_size(1)  /* INET_DIAG_TCLASS */
		+ nla_total_size(1)  /* INET_DIAG_SKV6ONLY */
#endif
		+ nla_total_size(4)  /* INET_DIAG_MARK */
		+ nla_total_size(4)  /* INET_DIAG_CLASS_ID */
#ifdef CONFIG_SOCK_CGROUP_DATA
		+ nla_total_size_64bit(sizeof(u64))  /* INET_DIAG_CGROUP_ID */
#endif
		+ nla_total_size(sizeof(struct inet_diag_sockopt))
						     /* INET_DIAG_SOCKOPT */
		;
}
int inet_diag_msg_attrs_fill(struct sock *sk, struct sk_buff *skb,
			     struct inet_diag_msg *r, int ext,
			     struct user_namespace *user_ns, bool net_admin);

extern int  inet_diag_register(const struct inet_diag_handler *handler);
extern void inet_diag_unregister(const struct inet_diag_handler *handler);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _INET_DIAG_H_ */
