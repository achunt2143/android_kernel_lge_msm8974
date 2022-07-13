<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  linux/include/linux/sunrpc/clnt.h
 *
 *  Declarations for the high-level RPC client interface
 *
 *  Copyright (C) 1995, 1996, Olaf Kirch <okir@monad.swb.de>
 */

#ifndef _LINUX_SUNRPC_CLNT_H
#define _LINUX_SUNRPC_CLNT_H

#include <linux/types.h>
#include <linux/socket.h>
#include <linux/in.h>
#include <linux/in6.h>
<<<<<<< HEAD
=======
#include <linux/refcount.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <linux/sunrpc/msg_prot.h>
#include <linux/sunrpc/sched.h>
#include <linux/sunrpc/xprt.h>
#include <linux/sunrpc/auth.h>
#include <linux/sunrpc/stats.h>
#include <linux/sunrpc/xdr.h>
#include <linux/sunrpc/timer.h>
<<<<<<< HEAD
#include <asm/signal.h>
#include <linux/path.h>
#include <net/ipv6.h>

struct rpc_inode;
=======
#include <linux/sunrpc/rpc_pipe_fs.h>
#include <asm/signal.h>
#include <linux/path.h>
#include <net/ipv6.h>
#include <linux/sunrpc/xprtmultipath.h>

struct rpc_inode;
struct rpc_sysfs_client {
	struct kobject kobject;
	struct net *net;
	struct rpc_clnt *clnt;
	struct rpc_xprt_switch *xprt_switch;
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * The high-level client handle
 */
struct rpc_clnt {
<<<<<<< HEAD
	atomic_t		cl_count;	/* Number of references */
	struct list_head	cl_clients;	/* Global list of clients */
	struct list_head	cl_tasks;	/* List of tasks */
	spinlock_t		cl_lock;	/* spinlock */
	struct rpc_xprt __rcu *	cl_xprt;	/* transport */
	struct rpc_procinfo *	cl_procinfo;	/* procedure info */
=======
	refcount_t		cl_count;	/* Number of references */
	unsigned int		cl_clid;	/* client id */
	struct list_head	cl_clients;	/* Global list of clients */
	struct list_head	cl_tasks;	/* List of tasks */
	atomic_t		cl_pid;		/* task PID counter */
	spinlock_t		cl_lock;	/* spinlock */
	struct rpc_xprt __rcu *	cl_xprt;	/* transport */
	const struct rpc_procinfo *cl_procinfo;	/* procedure info */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u32			cl_prog,	/* RPC program number */
				cl_vers,	/* RPC version number */
				cl_maxproc;	/* max procedure number */

<<<<<<< HEAD
	const char *		cl_protname;	/* protocol name */
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct rpc_auth *	cl_auth;	/* authenticator */
	struct rpc_stat *	cl_stats;	/* per-program statistics */
	struct rpc_iostats *	cl_metrics;	/* per-client statistics */

	unsigned int		cl_softrtry : 1,/* soft timeouts */
<<<<<<< HEAD
				cl_discrtry : 1,/* disconnect before retry */
				cl_autobind : 1,/* use getport() */
				cl_chatty   : 1;/* be verbose */
=======
				cl_softerr  : 1,/* Timeouts return errors */
				cl_discrtry : 1,/* disconnect before retry */
				cl_noretranstimeo: 1,/* No retransmit timeouts */
				cl_autobind : 1,/* use getport() */
				cl_chatty   : 1,/* be verbose */
				cl_shutdown : 1;/* rpc immediate -EIO */
	struct xprtsec_parms	cl_xprtsec;	/* transport security policy */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	struct rpc_rtt *	cl_rtt;		/* RTO estimator data */
	const struct rpc_timeout *cl_timeout;	/* Timeout strategy */

<<<<<<< HEAD
	int			cl_nodelen;	/* nodename length */
	char 			cl_nodename[UNX_MAXNODENAME];
	struct dentry *		cl_dentry;
=======
	atomic_t		cl_swapper;	/* swapfile count */
	int			cl_nodelen;	/* nodename length */
	char 			cl_nodename[UNX_MAXNODENAME+1];
	struct rpc_pipe_dir_head cl_pipedir_objects;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct rpc_clnt *	cl_parent;	/* Points to parent of clones */
	struct rpc_rtt		cl_rtt_default;
	struct rpc_timeout	cl_timeout_default;
	const struct rpc_program *cl_program;
<<<<<<< HEAD
	char			*cl_principal;	/* target to authenticate to */
=======
	const char *		cl_principal;	/* use for machine cred */
#if IS_ENABLED(CONFIG_SUNRPC_DEBUG)
	struct dentry		*cl_debugfs;	/* debugfs directory */
#endif
	struct rpc_sysfs_client *cl_sysfs;	/* sysfs directory */
	/* cl_work is only needed after cl_xpi is no longer used,
	 * and that are of similar size
	 */
	union {
		struct rpc_xprt_iter	cl_xpi;
		struct work_struct	cl_work;
	};
	const struct cred	*cl_cred;
	unsigned int		cl_max_connect; /* max number of transports not to the same IP */
	struct super_block *pipefs_sb;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/*
 * General RPC program info
 */
#define RPC_MAXVERSION		4
struct rpc_program {
	const char *		name;		/* protocol name */
	u32			number;		/* program number */
	unsigned int		nrvers;		/* number of versions */
	const struct rpc_version **	version;	/* version array */
	struct rpc_stat *	stats;		/* statistics */
	const char *		pipe_dir_name;	/* path to rpc_pipefs dir */
};

struct rpc_version {
	u32			number;		/* version number */
	unsigned int		nrprocs;	/* number of procs */
<<<<<<< HEAD
	struct rpc_procinfo *	procs;		/* procedure array */
=======
	const struct rpc_procinfo *procs;	/* procedure array */
	unsigned int		*counts;	/* call counts */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/*
 * Procedure information
 */
struct rpc_procinfo {
	u32			p_proc;		/* RPC procedure number */
	kxdreproc_t		p_encode;	/* XDR encode function */
	kxdrdproc_t		p_decode;	/* XDR decode function */
	unsigned int		p_arglen;	/* argument hdr length (u32) */
	unsigned int		p_replen;	/* reply hdr length (u32) */
<<<<<<< HEAD
	unsigned int		p_count;	/* call count */
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned int		p_timer;	/* Which RTT timer to use */
	u32			p_statidx;	/* Which procedure to account */
	const char *		p_name;		/* name of procedure */
};

<<<<<<< HEAD
#ifdef __KERNEL__

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct rpc_create_args {
	struct net		*net;
	int			protocol;
	struct sockaddr		*address;
	size_t			addrsize;
	struct sockaddr		*saddress;
	const struct rpc_timeout *timeout;
	const char		*servername;
<<<<<<< HEAD
	const struct rpc_program *program;
	u32			prognumber;	/* overrides program->number */
	u32			version;
	rpc_authflavor_t	authflavor;
	unsigned long		flags;
	char			*client_name;
	struct svc_xprt		*bc_xprt;	/* NFSv4.1 backchannel */
=======
	const char		*nodename;
	const struct rpc_program *program;
	struct rpc_stat		*stats;
	u32			prognumber;	/* overrides program->number */
	u32			version;
	rpc_authflavor_t	authflavor;
	u32			nconnect;
	unsigned long		flags;
	char			*client_name;
	struct svc_xprt		*bc_xprt;	/* NFSv4.1 backchannel */
	const struct cred	*cred;
	unsigned int		max_connect;
	struct xprtsec_parms	xprtsec;
	unsigned long		connect_timeout;
	unsigned long		reconnect_timeout;
};

struct rpc_add_xprt_test {
	void (*add_xprt_test)(struct rpc_clnt *clnt,
		struct rpc_xprt *xprt,
		void *calldata);
	void *data;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/* Values for "flags" field */
#define RPC_CLNT_CREATE_HARDRTRY	(1UL << 0)
#define RPC_CLNT_CREATE_AUTOBIND	(1UL << 2)
#define RPC_CLNT_CREATE_NONPRIVPORT	(1UL << 3)
#define RPC_CLNT_CREATE_NOPING		(1UL << 4)
#define RPC_CLNT_CREATE_DISCRTRY	(1UL << 5)
#define RPC_CLNT_CREATE_QUIET		(1UL << 6)
<<<<<<< HEAD
=======
#define RPC_CLNT_CREATE_INFINITE_SLOTS	(1UL << 7)
#define RPC_CLNT_CREATE_NO_IDLE_TIMEOUT	(1UL << 8)
#define RPC_CLNT_CREATE_NO_RETRANS_TIMEOUT	(1UL << 9)
#define RPC_CLNT_CREATE_SOFTERR		(1UL << 10)
#define RPC_CLNT_CREATE_REUSEPORT	(1UL << 11)
#define RPC_CLNT_CREATE_CONNECTED	(1UL << 12)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct rpc_clnt *rpc_create(struct rpc_create_args *args);
struct rpc_clnt	*rpc_bind_new_program(struct rpc_clnt *,
				const struct rpc_program *, u32);
<<<<<<< HEAD
void rpc_task_reset_client(struct rpc_task *task, struct rpc_clnt *clnt);
struct rpc_clnt *rpc_clone_client(struct rpc_clnt *);
void		rpc_shutdown_client(struct rpc_clnt *);
void		rpc_release_client(struct rpc_clnt *);
void		rpc_task_release_client(struct rpc_task *);
=======
struct rpc_clnt *rpc_clone_client(struct rpc_clnt *);
struct rpc_clnt *rpc_clone_client_set_auth(struct rpc_clnt *,
				rpc_authflavor_t);
int		rpc_switch_client_transport(struct rpc_clnt *,
				struct xprt_create *,
				const struct rpc_timeout *);

void		rpc_shutdown_client(struct rpc_clnt *);
void		rpc_release_client(struct rpc_clnt *);
void		rpc_task_release_transport(struct rpc_task *);
void		rpc_task_release_client(struct rpc_task *);
struct rpc_xprt	*rpc_task_get_xprt(struct rpc_clnt *clnt,
		struct rpc_xprt *xprt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

int		rpcb_create_local(struct net *);
void		rpcb_put_local(struct net *);
int		rpcb_register(struct net *, u32, u32, int, unsigned short);
int		rpcb_v4_register(struct net *net, const u32 program,
				 const u32 version,
				 const struct sockaddr *address,
				 const char *netid);
void		rpcb_getport_async(struct rpc_task *);

<<<<<<< HEAD
=======
void rpc_prepare_reply_pages(struct rpc_rqst *req, struct page **pages,
			     unsigned int base, unsigned int len,
			     unsigned int hdrsize);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void		rpc_call_start(struct rpc_task *);
int		rpc_call_async(struct rpc_clnt *clnt,
			       const struct rpc_message *msg, int flags,
			       const struct rpc_call_ops *tk_ops,
			       void *calldata);
int		rpc_call_sync(struct rpc_clnt *clnt,
			      const struct rpc_message *msg, int flags);
struct rpc_task *rpc_call_null(struct rpc_clnt *clnt, struct rpc_cred *cred,
			       int flags);
int		rpc_restart_call_prepare(struct rpc_task *);
int		rpc_restart_call(struct rpc_task *);
void		rpc_setbufsize(struct rpc_clnt *, unsigned int, unsigned int);
<<<<<<< HEAD
int		rpc_protocol(struct rpc_clnt *);
struct net *	rpc_net_ns(struct rpc_clnt *);
size_t		rpc_max_payload(struct rpc_clnt *);
=======
struct net *	rpc_net_ns(struct rpc_clnt *);
size_t		rpc_max_payload(struct rpc_clnt *);
size_t		rpc_max_bc_payload(struct rpc_clnt *);
unsigned int	rpc_num_bc_slots(struct rpc_clnt *);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void		rpc_force_rebind(struct rpc_clnt *);
size_t		rpc_peeraddr(struct rpc_clnt *, struct sockaddr *, size_t);
const char	*rpc_peeraddr2str(struct rpc_clnt *, enum rpc_display_format_t);
int		rpc_localaddr(struct rpc_clnt *, struct sockaddr *, size_t);

<<<<<<< HEAD
size_t		rpc_ntop(const struct sockaddr *, char *, const size_t);
size_t		rpc_pton(struct net *, const char *, const size_t,
			 struct sockaddr *, const size_t);
char *		rpc_sockaddr2uaddr(const struct sockaddr *, gfp_t);
size_t		rpc_uaddr2sockaddr(struct net *, const char *, const size_t,
				   struct sockaddr *, const size_t);

static inline unsigned short rpc_get_port(const struct sockaddr *sap)
{
	switch (sap->sa_family) {
	case AF_INET:
		return ntohs(((struct sockaddr_in *)sap)->sin_port);
	case AF_INET6:
		return ntohs(((struct sockaddr_in6 *)sap)->sin6_port);
	}
	return 0;
}

static inline void rpc_set_port(struct sockaddr *sap,
				const unsigned short port)
{
	switch (sap->sa_family) {
	case AF_INET:
		((struct sockaddr_in *)sap)->sin_port = htons(port);
		break;
	case AF_INET6:
		((struct sockaddr_in6 *)sap)->sin6_port = htons(port);
		break;
	}
}

#define IPV6_SCOPE_DELIMITER		'%'
#define IPV6_SCOPE_ID_LEN		sizeof("%nnnnnnnnnn")

static inline bool __rpc_cmp_addr4(const struct sockaddr *sap1,
				   const struct sockaddr *sap2)
{
	const struct sockaddr_in *sin1 = (const struct sockaddr_in *)sap1;
	const struct sockaddr_in *sin2 = (const struct sockaddr_in *)sap2;

	return sin1->sin_addr.s_addr == sin2->sin_addr.s_addr;
}

static inline bool __rpc_copy_addr4(struct sockaddr *dst,
				    const struct sockaddr *src)
{
	const struct sockaddr_in *ssin = (struct sockaddr_in *) src;
	struct sockaddr_in *dsin = (struct sockaddr_in *) dst;

	dsin->sin_family = ssin->sin_family;
	dsin->sin_addr.s_addr = ssin->sin_addr.s_addr;
	return true;
}

#if IS_ENABLED(CONFIG_IPV6)
static inline bool __rpc_cmp_addr6(const struct sockaddr *sap1,
				   const struct sockaddr *sap2)
{
	const struct sockaddr_in6 *sin1 = (const struct sockaddr_in6 *)sap1;
	const struct sockaddr_in6 *sin2 = (const struct sockaddr_in6 *)sap2;

	if (!ipv6_addr_equal(&sin1->sin6_addr, &sin2->sin6_addr))
		return false;
	else if (ipv6_addr_type(&sin1->sin6_addr) & IPV6_ADDR_LINKLOCAL)
		return sin1->sin6_scope_id == sin2->sin6_scope_id;

	return true;
}

static inline bool __rpc_copy_addr6(struct sockaddr *dst,
				    const struct sockaddr *src)
{
	const struct sockaddr_in6 *ssin6 = (const struct sockaddr_in6 *) src;
	struct sockaddr_in6 *dsin6 = (struct sockaddr_in6 *) dst;

	dsin6->sin6_family = ssin6->sin6_family;
	dsin6->sin6_addr = ssin6->sin6_addr;
	return true;
}
#else	/* !(IS_ENABLED(CONFIG_IPV6) */
static inline bool __rpc_cmp_addr6(const struct sockaddr *sap1,
				   const struct sockaddr *sap2)
{
	return false;
}

static inline bool __rpc_copy_addr6(struct sockaddr *dst,
				    const struct sockaddr *src)
{
	return false;
}
#endif	/* !(IS_ENABLED(CONFIG_IPV6) */

/**
 * rpc_cmp_addr - compare the address portion of two sockaddrs.
 * @sap1: first sockaddr
 * @sap2: second sockaddr
 *
 * Just compares the family and address portion. Ignores port, scope, etc.
 * Returns true if the addrs are equal, false if they aren't.
 */
static inline bool rpc_cmp_addr(const struct sockaddr *sap1,
				const struct sockaddr *sap2)
{
	if (sap1->sa_family == sap2->sa_family) {
		switch (sap1->sa_family) {
		case AF_INET:
			return __rpc_cmp_addr4(sap1, sap2);
		case AF_INET6:
			return __rpc_cmp_addr6(sap1, sap2);
		}
	}
	return false;
}

/**
 * rpc_copy_addr - copy the address portion of one sockaddr to another
 * @dst: destination sockaddr
 * @src: source sockaddr
 *
 * Just copies the address portion and family. Ignores port, scope, etc.
 * Caller is responsible for making certain that dst is large enough to hold
 * the address in src. Returns true if address family is supported. Returns
 * false otherwise.
 */
static inline bool rpc_copy_addr(struct sockaddr *dst,
				 const struct sockaddr *src)
{
	switch (src->sa_family) {
	case AF_INET:
		return __rpc_copy_addr4(dst, src);
	case AF_INET6:
		return __rpc_copy_addr6(dst, src);
	}
	return false;
}

/**
 * rpc_get_scope_id - return scopeid for a given sockaddr
 * @sa: sockaddr to get scopeid from
 *
 * Returns the value of the sin6_scope_id for AF_INET6 addrs, or 0 if
 * not an AF_INET6 address.
 */
static inline u32 rpc_get_scope_id(const struct sockaddr *sa)
{
	if (sa->sa_family != AF_INET6)
		return 0;

	return ((struct sockaddr_in6 *) sa)->sin6_scope_id;
}

#endif /* __KERNEL__ */
=======
int 		rpc_clnt_iterate_for_each_xprt(struct rpc_clnt *clnt,
			int (*fn)(struct rpc_clnt *, struct rpc_xprt *, void *),
			void *data);

int 		rpc_clnt_test_and_add_xprt(struct rpc_clnt *clnt,
			struct rpc_xprt_switch *xps,
			struct rpc_xprt *xprt,
			void *dummy);
int		rpc_clnt_add_xprt(struct rpc_clnt *, struct xprt_create *,
			int (*setup)(struct rpc_clnt *,
				struct rpc_xprt_switch *,
				struct rpc_xprt *,
				void *),
			void *data);
void		rpc_set_connect_timeout(struct rpc_clnt *clnt,
			unsigned long connect_timeout,
			unsigned long reconnect_timeout);

int		rpc_clnt_setup_test_and_add_xprt(struct rpc_clnt *,
			struct rpc_xprt_switch *,
			struct rpc_xprt *,
			void *);
void		rpc_clnt_manage_trunked_xprts(struct rpc_clnt *);
void		rpc_clnt_probe_trunked_xprts(struct rpc_clnt *,
			struct rpc_add_xprt_test *);

const char *rpc_proc_name(const struct rpc_task *task);

void rpc_clnt_xprt_switch_add_xprt(struct rpc_clnt *, struct rpc_xprt *);
void rpc_clnt_xprt_switch_remove_xprt(struct rpc_clnt *, struct rpc_xprt *);
bool rpc_clnt_xprt_switch_has_addr(struct rpc_clnt *clnt,
			const struct sockaddr *sap);
void rpc_clnt_xprt_set_online(struct rpc_clnt *clnt, struct rpc_xprt *xprt);
void rpc_clnt_disconnect(struct rpc_clnt *clnt);
void rpc_cleanup_clids(void);

static inline int rpc_reply_expected(struct rpc_task *task)
{
	return (task->tk_msg.rpc_proc != NULL) &&
		(task->tk_msg.rpc_proc->p_decode != NULL);
}

static inline void rpc_task_close_connection(struct rpc_task *task)
{
	if (task->tk_xprt)
		xprt_force_disconnect(task->tk_xprt);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _LINUX_SUNRPC_CLNT_H */
