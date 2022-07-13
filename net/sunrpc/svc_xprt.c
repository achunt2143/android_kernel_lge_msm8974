<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * linux/net/sunrpc/svc_xprt.c
 *
 * Author: Tom Tucker <tom@opengridcomputing.com>
 */

#include <linux/sched.h>
<<<<<<< HEAD
#include <linux/errno.h>
#include <linux/freezer.h>
#include <linux/kthread.h>
#include <linux/slab.h>
#include <net/sock.h>
=======
#include <linux/sched/mm.h>
#include <linux/errno.h>
#include <linux/freezer.h>
#include <linux/slab.h>
#include <net/sock.h>
#include <linux/sunrpc/addr.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/sunrpc/stats.h>
#include <linux/sunrpc/svc_xprt.h>
#include <linux/sunrpc/svcsock.h>
#include <linux/sunrpc/xprt.h>
<<<<<<< HEAD
#include <linux/module.h>

#define RPCDBG_FACILITY	RPCDBG_SVCXPRT

static struct svc_deferred_req *svc_deferred_dequeue(struct svc_xprt *xprt);
static int svc_deferred_recv(struct svc_rqst *rqstp);
static struct cache_deferred_req *svc_defer(struct cache_req *req);
static void svc_age_temp_xprts(unsigned long closure);
=======
#include <linux/sunrpc/bc_xprt.h>
#include <linux/module.h>
#include <linux/netdevice.h>
#include <trace/events/sunrpc.h>

#define RPCDBG_FACILITY	RPCDBG_SVCXPRT

static unsigned int svc_rpc_per_connection_limit __read_mostly;
module_param(svc_rpc_per_connection_limit, uint, 0644);


static struct svc_deferred_req *svc_deferred_dequeue(struct svc_xprt *xprt);
static int svc_deferred_recv(struct svc_rqst *rqstp);
static struct cache_deferred_req *svc_defer(struct cache_req *req);
static void svc_age_temp_xprts(struct timer_list *t);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void svc_delete_xprt(struct svc_xprt *xprt);

/* apparently the "standard" is that clients close
 * idle connections after 5 minutes, servers after
 * 6 minutes
<<<<<<< HEAD
 *   http://www.connectathon.org/talks96/nfstcp.pdf
=======
 *   http://nfsv4bat.org/Documents/ConnectAThon/1996/nfstcp.pdf
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
static int svc_conn_age_period = 6*60;

/* List of registered transport classes */
static DEFINE_SPINLOCK(svc_xprt_class_lock);
static LIST_HEAD(svc_xprt_class_list);

/* SMP locking strategy:
 *
 *	svc_pool->sp_lock protects most of the fields of that pool.
 *	svc_serv->sv_lock protects sv_tempsocks, sv_permsocks, sv_tmpcnt.
 *	when both need to be taken (rare), svc_serv->sv_lock is first.
<<<<<<< HEAD
 *	BKL protects svc_serv->sv_nrthread.
=======
 *	The "service mutex" protects svc_serv->sv_nrthread.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *	svc_sock->sk_lock protects the svc_sock->sk_deferred list
 *             and the ->sk_info_authunix cache.
 *
 *	The XPT_BUSY bit in xprt->xpt_flags prevents a transport being
 *	enqueued multiply. During normal transport processing this bit
 *	is set by svc_xprt_enqueue and cleared by svc_xprt_received.
 *	Providers should not manipulate this bit directly.
 *
 *	Some flags can be set to certain values at any time
 *	providing that certain rules are followed:
 *
 *	XPT_CONN, XPT_DATA:
 *		- Can be set or cleared at any time.
 *		- After a set, svc_xprt_enqueue must be called to enqueue
 *		  the transport for processing.
 *		- After a clear, the transport must be read/accepted.
 *		  If this succeeds, it must be set again.
 *	XPT_CLOSE:
 *		- Can set at any time. It is never cleared.
 *      XPT_DEAD:
 *		- Can only be set while XPT_BUSY is held which ensures
 *		  that no other thread will be using the transport or will
 *		  try to set XPT_DEAD.
 */

<<<<<<< HEAD
=======
/**
 * svc_reg_xprt_class - Register a server-side RPC transport class
 * @xcl: New transport class to be registered
 *
 * Returns zero on success; otherwise a negative errno is returned.
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int svc_reg_xprt_class(struct svc_xprt_class *xcl)
{
	struct svc_xprt_class *cl;
	int res = -EEXIST;

<<<<<<< HEAD
	dprintk("svc: Adding svc transport class '%s'\n", xcl->xcl_name);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	INIT_LIST_HEAD(&xcl->xcl_list);
	spin_lock(&svc_xprt_class_lock);
	/* Make sure there isn't already a class with the same name */
	list_for_each_entry(cl, &svc_xprt_class_list, xcl_list) {
		if (strcmp(xcl->xcl_name, cl->xcl_name) == 0)
			goto out;
	}
	list_add_tail(&xcl->xcl_list, &svc_xprt_class_list);
	res = 0;
out:
	spin_unlock(&svc_xprt_class_lock);
	return res;
}
EXPORT_SYMBOL_GPL(svc_reg_xprt_class);

<<<<<<< HEAD
void svc_unreg_xprt_class(struct svc_xprt_class *xcl)
{
	dprintk("svc: Removing svc transport class '%s'\n", xcl->xcl_name);
=======
/**
 * svc_unreg_xprt_class - Unregister a server-side RPC transport class
 * @xcl: Transport class to be unregistered
 *
 */
void svc_unreg_xprt_class(struct svc_xprt_class *xcl)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_lock(&svc_xprt_class_lock);
	list_del_init(&xcl->xcl_list);
	spin_unlock(&svc_xprt_class_lock);
}
EXPORT_SYMBOL_GPL(svc_unreg_xprt_class);

<<<<<<< HEAD
/*
 * Format the transport list for printing
=======
/**
 * svc_print_xprts - Format the transport list for printing
 * @buf: target buffer for formatted address
 * @maxlen: length of target buffer
 *
 * Fills in @buf with a string containing a list of transport names, each name
 * terminated with '\n'. If the buffer is too small, some entries may be
 * missing, but it is guaranteed that all lines in the output buffer are
 * complete.
 *
 * Returns positive length of the filled-in string.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
int svc_print_xprts(char *buf, int maxlen)
{
	struct svc_xprt_class *xcl;
	char tmpstr[80];
	int len = 0;
	buf[0] = '\0';

	spin_lock(&svc_xprt_class_lock);
	list_for_each_entry(xcl, &svc_xprt_class_list, xcl_list) {
		int slen;

<<<<<<< HEAD
		sprintf(tmpstr, "%s %d\n", xcl->xcl_name, xcl->xcl_max_payload);
		slen = strlen(tmpstr);
		if (len + slen > maxlen)
=======
		slen = snprintf(tmpstr, sizeof(tmpstr), "%s %d\n",
				xcl->xcl_name, xcl->xcl_max_payload);
		if (slen >= sizeof(tmpstr) || len + slen >= maxlen)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		len += slen;
		strcat(buf, tmpstr);
	}
	spin_unlock(&svc_xprt_class_lock);

	return len;
}

<<<<<<< HEAD
=======
/**
 * svc_xprt_deferred_close - Close a transport
 * @xprt: transport instance
 *
 * Used in contexts that need to defer the work of shutting down
 * the transport to an nfsd thread.
 */
void svc_xprt_deferred_close(struct svc_xprt *xprt)
{
	if (!test_and_set_bit(XPT_CLOSE, &xprt->xpt_flags))
		svc_xprt_enqueue(xprt);
}
EXPORT_SYMBOL_GPL(svc_xprt_deferred_close);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void svc_xprt_free(struct kref *kref)
{
	struct svc_xprt *xprt =
		container_of(kref, struct svc_xprt, xpt_ref);
	struct module *owner = xprt->xpt_class->xcl_owner;
	if (test_bit(XPT_CACHE_AUTH, &xprt->xpt_flags))
		svcauth_unix_info_release(xprt);
<<<<<<< HEAD
	put_net(xprt->xpt_net);
	/* See comment on corresponding get in xs_setup_bc_tcp(): */
	if (xprt->xpt_bc_xprt)
		xprt_put(xprt->xpt_bc_xprt);
=======
	put_cred(xprt->xpt_cred);
	put_net_track(xprt->xpt_net, &xprt->ns_tracker);
	/* See comment on corresponding get in xs_setup_bc_tcp(): */
	if (xprt->xpt_bc_xprt)
		xprt_put(xprt->xpt_bc_xprt);
	if (xprt->xpt_bc_xps)
		xprt_switch_put(xprt->xpt_bc_xps);
	trace_svc_xprt_free(xprt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	xprt->xpt_ops->xpo_free(xprt);
	module_put(owner);
}

void svc_xprt_put(struct svc_xprt *xprt)
{
	kref_put(&xprt->xpt_ref, svc_xprt_free);
}
EXPORT_SYMBOL_GPL(svc_xprt_put);

/*
 * Called by transport drivers to initialize the transport independent
 * portion of the transport instance.
 */
void svc_xprt_init(struct net *net, struct svc_xprt_class *xcl,
		   struct svc_xprt *xprt, struct svc_serv *serv)
{
	memset(xprt, 0, sizeof(*xprt));
	xprt->xpt_class = xcl;
	xprt->xpt_ops = xcl->xcl_ops;
	kref_init(&xprt->xpt_ref);
	xprt->xpt_server = serv;
	INIT_LIST_HEAD(&xprt->xpt_list);
<<<<<<< HEAD
	INIT_LIST_HEAD(&xprt->xpt_ready);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	INIT_LIST_HEAD(&xprt->xpt_deferred);
	INIT_LIST_HEAD(&xprt->xpt_users);
	mutex_init(&xprt->xpt_mutex);
	spin_lock_init(&xprt->xpt_lock);
	set_bit(XPT_BUSY, &xprt->xpt_flags);
<<<<<<< HEAD
	rpc_init_wait_queue(&xprt->xpt_bc_pending, "xpt_bc_pending");
	xprt->xpt_net = get_net(net);
=======
	xprt->xpt_net = get_net_track(net, &xprt->ns_tracker, GFP_ATOMIC);
	strcpy(xprt->xpt_remotebuf, "uninitialized");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(svc_xprt_init);

static struct svc_xprt *__svc_xpo_create(struct svc_xprt_class *xcl,
					 struct svc_serv *serv,
					 struct net *net,
					 const int family,
					 const unsigned short port,
					 int flags)
{
	struct sockaddr_in sin = {
		.sin_family		= AF_INET,
		.sin_addr.s_addr	= htonl(INADDR_ANY),
		.sin_port		= htons(port),
	};
#if IS_ENABLED(CONFIG_IPV6)
	struct sockaddr_in6 sin6 = {
		.sin6_family		= AF_INET6,
		.sin6_addr		= IN6ADDR_ANY_INIT,
		.sin6_port		= htons(port),
	};
#endif
<<<<<<< HEAD
=======
	struct svc_xprt *xprt;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct sockaddr *sap;
	size_t len;

	switch (family) {
	case PF_INET:
		sap = (struct sockaddr *)&sin;
		len = sizeof(sin);
		break;
#if IS_ENABLED(CONFIG_IPV6)
	case PF_INET6:
		sap = (struct sockaddr *)&sin6;
		len = sizeof(sin6);
		break;
#endif
	default:
		return ERR_PTR(-EAFNOSUPPORT);
	}

<<<<<<< HEAD
	return xcl->xcl_ops->xpo_create(serv, net, sap, len, flags);
}

int svc_create_xprt(struct svc_serv *serv, const char *xprt_name,
		    struct net *net, const int family,
		    const unsigned short port, int flags)
{
	struct svc_xprt_class *xcl;

	dprintk("svc: creating transport %s[%d]\n", xprt_name, port);
=======
	xprt = xcl->xcl_ops->xpo_create(serv, net, sap, len, flags);
	if (IS_ERR(xprt))
		trace_svc_xprt_create_err(serv->sv_program->pg_name,
					  xcl->xcl_name, sap, len, xprt);
	return xprt;
}

/**
 * svc_xprt_received - start next receiver thread
 * @xprt: controlling transport
 *
 * The caller must hold the XPT_BUSY bit and must
 * not thereafter touch transport data.
 *
 * Note: XPT_DATA only gets cleared when a read-attempt finds no (or
 * insufficient) data.
 */
void svc_xprt_received(struct svc_xprt *xprt)
{
	if (!test_bit(XPT_BUSY, &xprt->xpt_flags)) {
		WARN_ONCE(1, "xprt=0x%p already busy!", xprt);
		return;
	}

	/* As soon as we clear busy, the xprt could be closed and
	 * 'put', so we need a reference to call svc_xprt_enqueue with:
	 */
	svc_xprt_get(xprt);
	smp_mb__before_atomic();
	clear_bit(XPT_BUSY, &xprt->xpt_flags);
	svc_xprt_enqueue(xprt);
	svc_xprt_put(xprt);
}
EXPORT_SYMBOL_GPL(svc_xprt_received);

void svc_add_new_perm_xprt(struct svc_serv *serv, struct svc_xprt *new)
{
	clear_bit(XPT_TEMP, &new->xpt_flags);
	spin_lock_bh(&serv->sv_lock);
	list_add(&new->xpt_list, &serv->sv_permsocks);
	spin_unlock_bh(&serv->sv_lock);
	svc_xprt_received(new);
}

static int _svc_xprt_create(struct svc_serv *serv, const char *xprt_name,
			    struct net *net, const int family,
			    const unsigned short port, int flags,
			    const struct cred *cred)
{
	struct svc_xprt_class *xcl;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_lock(&svc_xprt_class_lock);
	list_for_each_entry(xcl, &svc_xprt_class_list, xcl_list) {
		struct svc_xprt *newxprt;
		unsigned short newport;

		if (strcmp(xprt_name, xcl->xcl_name))
			continue;

		if (!try_module_get(xcl->xcl_owner))
			goto err;

		spin_unlock(&svc_xprt_class_lock);
		newxprt = __svc_xpo_create(xcl, serv, net, family, port, flags);
		if (IS_ERR(newxprt)) {
			module_put(xcl->xcl_owner);
			return PTR_ERR(newxprt);
		}
<<<<<<< HEAD

		clear_bit(XPT_TEMP, &newxprt->xpt_flags);
		spin_lock_bh(&serv->sv_lock);
		list_add(&newxprt->xpt_list, &serv->sv_permsocks);
		spin_unlock_bh(&serv->sv_lock);
		newport = svc_xprt_local_port(newxprt);
		clear_bit(XPT_BUSY, &newxprt->xpt_flags);
=======
		newxprt->xpt_cred = get_cred(cred);
		svc_add_new_perm_xprt(serv, newxprt);
		newport = svc_xprt_local_port(newxprt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return newport;
	}
 err:
	spin_unlock(&svc_xprt_class_lock);
<<<<<<< HEAD
	dprintk("svc: transport %s not found\n", xprt_name);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* This errno is exposed to user space.  Provide a reasonable
	 * perror msg for a bad transport. */
	return -EPROTONOSUPPORT;
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(svc_create_xprt);
=======

/**
 * svc_xprt_create - Add a new listener to @serv
 * @serv: target RPC service
 * @xprt_name: transport class name
 * @net: network namespace
 * @family: network address family
 * @port: listener port
 * @flags: SVC_SOCK flags
 * @cred: credential to bind to this transport
 *
 * Return values:
 *   %0: New listener added successfully
 *   %-EPROTONOSUPPORT: Requested transport type not supported
 */
int svc_xprt_create(struct svc_serv *serv, const char *xprt_name,
		    struct net *net, const int family,
		    const unsigned short port, int flags,
		    const struct cred *cred)
{
	int err;

	err = _svc_xprt_create(serv, xprt_name, net, family, port, flags, cred);
	if (err == -EPROTONOSUPPORT) {
		request_module("svc%s", xprt_name);
		err = _svc_xprt_create(serv, xprt_name, net, family, port, flags, cred);
	}
	return err;
}
EXPORT_SYMBOL_GPL(svc_xprt_create);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Copy the local and remote xprt addresses to the rqstp structure
 */
void svc_xprt_copy_addrs(struct svc_rqst *rqstp, struct svc_xprt *xprt)
{
	memcpy(&rqstp->rq_addr, &xprt->xpt_remote, xprt->xpt_remotelen);
	rqstp->rq_addrlen = xprt->xpt_remotelen;

	/*
	 * Destination address in request is needed for binding the
	 * source address in RPC replies/callbacks later.
	 */
	memcpy(&rqstp->rq_daddr, &xprt->xpt_local, xprt->xpt_locallen);
	rqstp->rq_daddrlen = xprt->xpt_locallen;
}
EXPORT_SYMBOL_GPL(svc_xprt_copy_addrs);

/**
 * svc_print_addr - Format rq_addr field for printing
 * @rqstp: svc_rqst struct containing address to print
 * @buf: target buffer for formatted address
 * @len: length of target buffer
 *
 */
char *svc_print_addr(struct svc_rqst *rqstp, char *buf, size_t len)
{
	return __svc_print_addr(svc_addr(rqstp), buf, len);
}
EXPORT_SYMBOL_GPL(svc_print_addr);

<<<<<<< HEAD
/*
 * Queue up an idle server thread.  Must have pool->sp_lock held.
 * Note: this is really a stack rather than a queue, so that we only
 * use as many different threads as we need, and the rest don't pollute
 * the cache.
 */
static void svc_thread_enqueue(struct svc_pool *pool, struct svc_rqst *rqstp)
{
	list_add(&rqstp->rq_list, &pool->sp_threads);
}

/*
 * Dequeue an nfsd thread.  Must have pool->sp_lock held.
 */
static void svc_thread_dequeue(struct svc_pool *pool, struct svc_rqst *rqstp)
{
	list_del(&rqstp->rq_list);
}

static bool svc_xprt_has_something_to_do(struct svc_xprt *xprt)
{
	if (xprt->xpt_flags & ((1<<XPT_CONN)|(1<<XPT_CLOSE)))
		return true;
	if (xprt->xpt_flags & ((1<<XPT_DATA)|(1<<XPT_DEFERRED)))
		return xprt->xpt_ops->xpo_has_wspace(xprt);
	return false;
}

/*
 * Queue up a transport with data pending. If there are idle nfsd
 * processes, wake 'em up.
=======
static bool svc_xprt_slots_in_range(struct svc_xprt *xprt)
{
	unsigned int limit = svc_rpc_per_connection_limit;
	int nrqsts = atomic_read(&xprt->xpt_nr_rqsts);

	return limit == 0 || (nrqsts >= 0 && nrqsts < limit);
}

static bool svc_xprt_reserve_slot(struct svc_rqst *rqstp, struct svc_xprt *xprt)
{
	if (!test_bit(RQ_DATA, &rqstp->rq_flags)) {
		if (!svc_xprt_slots_in_range(xprt))
			return false;
		atomic_inc(&xprt->xpt_nr_rqsts);
		set_bit(RQ_DATA, &rqstp->rq_flags);
	}
	return true;
}

static void svc_xprt_release_slot(struct svc_rqst *rqstp)
{
	struct svc_xprt	*xprt = rqstp->rq_xprt;
	if (test_and_clear_bit(RQ_DATA, &rqstp->rq_flags)) {
		atomic_dec(&xprt->xpt_nr_rqsts);
		smp_wmb(); /* See smp_rmb() in svc_xprt_ready() */
		svc_xprt_enqueue(xprt);
	}
}

static bool svc_xprt_ready(struct svc_xprt *xprt)
{
	unsigned long xpt_flags;

	/*
	 * If another cpu has recently updated xpt_flags,
	 * sk_sock->flags, xpt_reserved, or xpt_nr_rqsts, we need to
	 * know about it; otherwise it's possible that both that cpu and
	 * this one could call svc_xprt_enqueue() without either
	 * svc_xprt_enqueue() recognizing that the conditions below
	 * are satisfied, and we could stall indefinitely:
	 */
	smp_rmb();
	xpt_flags = READ_ONCE(xprt->xpt_flags);

	trace_svc_xprt_enqueue(xprt, xpt_flags);
	if (xpt_flags & BIT(XPT_BUSY))
		return false;
	if (xpt_flags & (BIT(XPT_CONN) | BIT(XPT_CLOSE) | BIT(XPT_HANDSHAKE)))
		return true;
	if (xpt_flags & (BIT(XPT_DATA) | BIT(XPT_DEFERRED))) {
		if (xprt->xpt_ops->xpo_has_wspace(xprt) &&
		    svc_xprt_slots_in_range(xprt))
			return true;
		trace_svc_xprt_no_write_space(xprt);
		return false;
	}
	return false;
}

/**
 * svc_xprt_enqueue - Queue a transport on an idle nfsd thread
 * @xprt: transport with data pending
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 */
void svc_xprt_enqueue(struct svc_xprt *xprt)
{
	struct svc_pool *pool;
<<<<<<< HEAD
	struct svc_rqst	*rqstp;
	int cpu;

	if (!svc_xprt_has_something_to_do(xprt))
		return;

	cpu = get_cpu();
	pool = svc_pool_for_cpu(xprt->xpt_server, cpu);
	put_cpu();

	spin_lock_bh(&pool->sp_lock);

	if (!list_empty(&pool->sp_threads) &&
	    !list_empty(&pool->sp_sockets))
		printk(KERN_ERR
		       "svc_xprt_enqueue: "
		       "threads and transports both waiting??\n");

	pool->sp_stats.packets++;

=======

	if (!svc_xprt_ready(xprt))
		return;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Mark transport as busy. It will remain in this state until
	 * the provider calls svc_xprt_received. We update XPT_BUSY
	 * atomically because it also guards against trying to enqueue
	 * the transport twice.
	 */
<<<<<<< HEAD
	if (test_and_set_bit(XPT_BUSY, &xprt->xpt_flags)) {
		/* Don't enqueue transport while already enqueued */
		dprintk("svc: transport %p busy, not enqueued\n", xprt);
		goto out_unlock;
	}

	if (!list_empty(&pool->sp_threads)) {
		rqstp = list_entry(pool->sp_threads.next,
				   struct svc_rqst,
				   rq_list);
		dprintk("svc: transport %p served by daemon %p\n",
			xprt, rqstp);
		svc_thread_dequeue(pool, rqstp);
		if (rqstp->rq_xprt)
			printk(KERN_ERR
				"svc_xprt_enqueue: server %p, rq_xprt=%p!\n",
				rqstp, rqstp->rq_xprt);
		rqstp->rq_xprt = xprt;
		svc_xprt_get(xprt);
		pool->sp_stats.threads_woken++;
		wake_up(&rqstp->rq_wait);
	} else {
		dprintk("svc: transport %p put into queue\n", xprt);
		list_add_tail(&xprt->xpt_ready, &pool->sp_sockets);
		pool->sp_stats.sockets_queued++;
	}

out_unlock:
	spin_unlock_bh(&pool->sp_lock);
=======
	if (test_and_set_bit(XPT_BUSY, &xprt->xpt_flags))
		return;

	pool = svc_pool_for_cpu(xprt->xpt_server);

	percpu_counter_inc(&pool->sp_sockets_queued);
	lwq_enqueue(&xprt->xpt_ready, &pool->sp_xprts);

	svc_pool_wake_idle_thread(pool);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(svc_xprt_enqueue);

/*
<<<<<<< HEAD
 * Dequeue the first transport.  Must be called with the pool->sp_lock held.
 */
static struct svc_xprt *svc_xprt_dequeue(struct svc_pool *pool)
{
	struct svc_xprt	*xprt;

	if (list_empty(&pool->sp_sockets))
		return NULL;

	xprt = list_entry(pool->sp_sockets.next,
			  struct svc_xprt, xpt_ready);
	list_del_init(&xprt->xpt_ready);

	dprintk("svc: transport %p dequeued, inuse=%d\n",
		xprt, atomic_read(&xprt->xpt_ref.refcount));

	return xprt;
}

/*
 * svc_xprt_received conditionally queues the transport for processing
 * by another thread. The caller must hold the XPT_BUSY bit and must
 * not thereafter touch transport data.
 *
 * Note: XPT_DATA only gets cleared when a read-attempt finds no (or
 * insufficient) data.
 */
void svc_xprt_received(struct svc_xprt *xprt)
{
	BUG_ON(!test_bit(XPT_BUSY, &xprt->xpt_flags));
	/* As soon as we clear busy, the xprt could be closed and
	 * 'put', so we need a reference to call svc_xprt_enqueue with:
	 */
	svc_xprt_get(xprt);
	clear_bit(XPT_BUSY, &xprt->xpt_flags);
	svc_xprt_enqueue(xprt);
	svc_xprt_put(xprt);
}
EXPORT_SYMBOL_GPL(svc_xprt_received);

=======
 * Dequeue the first transport, if there is one.
 */
static struct svc_xprt *svc_xprt_dequeue(struct svc_pool *pool)
{
	struct svc_xprt	*xprt = NULL;

	xprt = lwq_dequeue(&pool->sp_xprts, struct svc_xprt, xpt_ready);
	if (xprt)
		svc_xprt_get(xprt);
	return xprt;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * svc_reserve - change the space reserved for the reply to a request.
 * @rqstp:  The request in question
 * @space: new max space to reserve
 *
 * Each request reserves some space on the output queue of the transport
 * to make sure the reply fits.  This function reduces that reserved
 * space to be the amount of space used already, plus @space.
 *
 */
void svc_reserve(struct svc_rqst *rqstp, int space)
{
<<<<<<< HEAD
	space += rqstp->rq_res.head[0].iov_len;

	if (space < rqstp->rq_reserved) {
		struct svc_xprt *xprt = rqstp->rq_xprt;
		atomic_sub((rqstp->rq_reserved - space), &xprt->xpt_reserved);
		rqstp->rq_reserved = space;

=======
	struct svc_xprt *xprt = rqstp->rq_xprt;

	space += rqstp->rq_res.head[0].iov_len;

	if (xprt && space < rqstp->rq_reserved) {
		atomic_sub((rqstp->rq_reserved - space), &xprt->xpt_reserved);
		rqstp->rq_reserved = space;
		smp_wmb(); /* See smp_rmb() in svc_xprt_ready() */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		svc_xprt_enqueue(xprt);
	}
}
EXPORT_SYMBOL_GPL(svc_reserve);

<<<<<<< HEAD
=======
static void free_deferred(struct svc_xprt *xprt, struct svc_deferred_req *dr)
{
	if (!dr)
		return;

	xprt->xpt_ops->xpo_release_ctxt(xprt, dr->xprt_ctxt);
	kfree(dr);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void svc_xprt_release(struct svc_rqst *rqstp)
{
	struct svc_xprt	*xprt = rqstp->rq_xprt;

<<<<<<< HEAD
	rqstp->rq_xprt->xpt_ops->xpo_release_rqst(rqstp);

	kfree(rqstp->rq_deferred);
	rqstp->rq_deferred = NULL;

	svc_free_res_pages(rqstp);
=======
	xprt->xpt_ops->xpo_release_ctxt(xprt, rqstp->rq_xprt_ctxt);
	rqstp->rq_xprt_ctxt = NULL;

	free_deferred(xprt, rqstp->rq_deferred);
	rqstp->rq_deferred = NULL;

	svc_rqst_release_pages(rqstp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rqstp->rq_res.page_len = 0;
	rqstp->rq_res.page_base = 0;

	/* Reset response buffer and release
	 * the reservation.
	 * But first, check that enough space was reserved
	 * for the reply, otherwise we have a bug!
	 */
	if ((rqstp->rq_res.len) >  rqstp->rq_reserved)
		printk(KERN_ERR "RPC request reserved %d but used %d\n",
		       rqstp->rq_reserved,
		       rqstp->rq_res.len);

	rqstp->rq_res.head[0].iov_len = 0;
	svc_reserve(rqstp, 0);
<<<<<<< HEAD
	rqstp->rq_xprt = NULL;

	svc_xprt_put(xprt);
}

/*
 * External function to wake up a server waiting for data
 * This really only makes sense for services like lockd
 * which have exactly one thread anyway.
 */
void svc_wake_up(struct svc_serv *serv)
{
	struct svc_rqst	*rqstp;
	unsigned int i;
	struct svc_pool *pool;

	for (i = 0; i < serv->sv_nrpools; i++) {
		pool = &serv->sv_pools[i];

		spin_lock_bh(&pool->sp_lock);
		if (!list_empty(&pool->sp_threads)) {
			rqstp = list_entry(pool->sp_threads.next,
					   struct svc_rqst,
					   rq_list);
			dprintk("svc: daemon %p woken up.\n", rqstp);
			/*
			svc_thread_dequeue(pool, rqstp);
			rqstp->rq_xprt = NULL;
			 */
			wake_up(&rqstp->rq_wait);
		}
		spin_unlock_bh(&pool->sp_lock);
	}
=======
	svc_xprt_release_slot(rqstp);
	rqstp->rq_xprt = NULL;
	svc_xprt_put(xprt);
}

/**
 * svc_wake_up - Wake up a service thread for non-transport work
 * @serv: RPC service
 *
 * Some svc_serv's will have occasional work to do, even when a xprt is not
 * waiting to be serviced. This function is there to "kick" a task in one of
 * those services so that it can wake up and do that work. Note that we only
 * bother with pool 0 as we don't need to wake up more than one thread for
 * this purpose.
 */
void svc_wake_up(struct svc_serv *serv)
{
	struct svc_pool *pool = &serv->sv_pools[0];

	set_bit(SP_TASK_PENDING, &pool->sp_flags);
	svc_pool_wake_idle_thread(pool);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(svc_wake_up);

int svc_port_is_privileged(struct sockaddr *sin)
{
	switch (sin->sa_family) {
	case AF_INET:
		return ntohs(((struct sockaddr_in *)sin)->sin_port)
			< PROT_SOCK;
	case AF_INET6:
		return ntohs(((struct sockaddr_in6 *)sin)->sin6_port)
			< PROT_SOCK;
	default:
		return 0;
	}
}

/*
 * Make sure that we don't have too many active connections. If we have,
 * something must be dropped. It's not clear what will happen if we allow
 * "too many" connections, but when dealing with network-facing software,
 * we have to code defensively. Here we do that by imposing hard limits.
 *
 * There's no point in trying to do random drop here for DoS
 * prevention. The NFS clients does 1 reconnect in 15 seconds. An
 * attacker can easily beat that.
 *
 * The only somewhat efficient mechanism would be if drop old
 * connections from the same IP first. But right now we don't even
 * record the client IP in svc_sock.
 *
 * single-threaded services that expect a lot of clients will probably
 * need to set sv_maxconn to override the default value which is based
 * on the number of threads
 */
static void svc_check_conn_limits(struct svc_serv *serv)
{
	unsigned int limit = serv->sv_maxconn ? serv->sv_maxconn :
				(serv->sv_nrthreads+3) * 20;

	if (serv->sv_tmpcnt > limit) {
		struct svc_xprt *xprt = NULL;
		spin_lock_bh(&serv->sv_lock);
		if (!list_empty(&serv->sv_tempsocks)) {
<<<<<<< HEAD
			if (net_ratelimit()) {
				/* Try to help the admin */
				printk(KERN_NOTICE "%s: too many open  "
				       "connections, consider increasing %s\n",
				       serv->sv_name, serv->sv_maxconn ?
				       "the max number of connections." :
				       "the number of threads.");
			}
=======
			/* Try to help the admin */
			net_notice_ratelimited("%s: too many open connections, consider increasing the %s\n",
					       serv->sv_name, serv->sv_maxconn ?
					       "max number of connections" :
					       "number of threads");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/*
			 * Always select the oldest connection. It's not fair,
			 * but so is life
			 */
			xprt = list_entry(serv->sv_tempsocks.prev,
					  struct svc_xprt,
					  xpt_list);
			set_bit(XPT_CLOSE, &xprt->xpt_flags);
			svc_xprt_get(xprt);
		}
		spin_unlock_bh(&serv->sv_lock);

		if (xprt) {
			svc_xprt_enqueue(xprt);
			svc_xprt_put(xprt);
		}
	}
}

<<<<<<< HEAD
/*
 * Receive the next request on any transport.  This code is carefully
 * organised not to touch any cachelines in the shared svc_serv
 * structure, only cachelines in the local svc_pool.
 */
int svc_recv(struct svc_rqst *rqstp, long timeout)
{
	struct svc_xprt		*xprt = NULL;
	struct svc_serv		*serv = rqstp->rq_server;
	struct svc_pool		*pool = rqstp->rq_pool;
	int			len, i;
	int			pages;
	struct xdr_buf		*arg;
	DECLARE_WAITQUEUE(wait, current);
	long			time_left;

	dprintk("svc: server %p waiting for data (to = %ld)\n",
		rqstp, timeout);

	if (rqstp->rq_xprt)
		printk(KERN_ERR
			"svc_recv: service %p, transport not NULL!\n",
			 rqstp);
	if (waitqueue_active(&rqstp->rq_wait))
		printk(KERN_ERR
			"svc_recv: service %p, wait queue active!\n",
			 rqstp);

	/* now allocate needed pages.  If we get a failure, sleep briefly */
	pages = (serv->sv_max_mesg + PAGE_SIZE) / PAGE_SIZE;
	for (i = 0; i < pages ; i++)
		while (rqstp->rq_pages[i] == NULL) {
			struct page *p = alloc_page(GFP_KERNEL);
			if (!p) {
				set_current_state(TASK_INTERRUPTIBLE);
				if (signalled() || kthread_should_stop()) {
					set_current_state(TASK_RUNNING);
					return -EINTR;
				}
				schedule_timeout(msecs_to_jiffies(500));
			}
			rqstp->rq_pages[i] = p;
		}
	rqstp->rq_pages[i++] = NULL; /* this might be seen in nfs_read_actor */
	BUG_ON(pages >= RPCSVC_MAXPAGES);

	/* Make arg->head point to first page and arg->pages point to rest */
	arg = &rqstp->rq_arg;
=======
static bool svc_alloc_arg(struct svc_rqst *rqstp)
{
	struct svc_serv *serv = rqstp->rq_server;
	struct xdr_buf *arg = &rqstp->rq_arg;
	unsigned long pages, filled, ret;

	pages = (serv->sv_max_mesg + 2 * PAGE_SIZE) >> PAGE_SHIFT;
	if (pages > RPCSVC_MAXPAGES) {
		pr_warn_once("svc: warning: pages=%lu > RPCSVC_MAXPAGES=%lu\n",
			     pages, RPCSVC_MAXPAGES);
		/* use as many pages as possible */
		pages = RPCSVC_MAXPAGES;
	}

	for (filled = 0; filled < pages; filled = ret) {
		ret = alloc_pages_bulk_array(GFP_KERNEL, pages,
					     rqstp->rq_pages);
		if (ret > filled)
			/* Made progress, don't sleep yet */
			continue;

		set_current_state(TASK_IDLE);
		if (svc_thread_should_stop(rqstp)) {
			set_current_state(TASK_RUNNING);
			return false;
		}
		trace_svc_alloc_arg_err(pages, ret);
		memalloc_retry_wait(GFP_KERNEL);
	}
	rqstp->rq_page_end = &rqstp->rq_pages[pages];
	rqstp->rq_pages[pages] = NULL; /* this might be seen in nfsd_splice_actor() */

	/* Make arg->head point to first page and arg->pages point to rest */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	arg->head[0].iov_base = page_address(rqstp->rq_pages[0]);
	arg->head[0].iov_len = PAGE_SIZE;
	arg->pages = rqstp->rq_pages + 1;
	arg->page_base = 0;
	/* save at least one page for response */
	arg->page_len = (pages-2)*PAGE_SIZE;
	arg->len = (pages-1)*PAGE_SIZE;
	arg->tail[0].iov_len = 0;

<<<<<<< HEAD
	try_to_freeze();
	cond_resched();
	if (signalled() || kthread_should_stop())
		return -EINTR;

	/* Normally we will wait up to 5 seconds for any required
	 * cache information to be provided.
	 */
	rqstp->rq_chandle.thread_wait = 5*HZ;

	spin_lock_bh(&pool->sp_lock);
	xprt = svc_xprt_dequeue(pool);
	if (xprt) {
		rqstp->rq_xprt = xprt;
		svc_xprt_get(xprt);

		/* As there is a shortage of threads and this request
		 * had to be queued, don't allow the thread to wait so
		 * long for cache updates.
		 */
		rqstp->rq_chandle.thread_wait = 1*HZ;
	} else {
		/* No data pending. Go to sleep */
		svc_thread_enqueue(pool, rqstp);

		/*
		 * We have to be able to interrupt this wait
		 * to bring down the daemons ...
		 */
		set_current_state(TASK_INTERRUPTIBLE);

		/*
		 * checking kthread_should_stop() here allows us to avoid
		 * locking and signalling when stopping kthreads that call
		 * svc_recv. If the thread has already been woken up, then
		 * we can exit here without sleeping. If not, then it
		 * it'll be woken up quickly during the schedule_timeout
		 */
		if (kthread_should_stop()) {
			set_current_state(TASK_RUNNING);
			spin_unlock_bh(&pool->sp_lock);
			return -EINTR;
		}

		add_wait_queue(&rqstp->rq_wait, &wait);
		spin_unlock_bh(&pool->sp_lock);

		time_left = schedule_timeout(timeout);

		try_to_freeze();

		spin_lock_bh(&pool->sp_lock);
		remove_wait_queue(&rqstp->rq_wait, &wait);
		if (!time_left)
			pool->sp_stats.threads_timedout++;

		xprt = rqstp->rq_xprt;
		if (!xprt) {
			svc_thread_dequeue(pool, rqstp);
			spin_unlock_bh(&pool->sp_lock);
			dprintk("svc: server %p, no data yet\n", rqstp);
			if (signalled() || kthread_should_stop())
				return -EINTR;
			else
				return -EAGAIN;
		}
	}
	spin_unlock_bh(&pool->sp_lock);

	len = 0;
	if (test_bit(XPT_CLOSE, &xprt->xpt_flags)) {
		dprintk("svc_recv: found XPT_CLOSE\n");
=======
	rqstp->rq_xid = xdr_zero;
	return true;
}

static bool
svc_thread_should_sleep(struct svc_rqst *rqstp)
{
	struct svc_pool		*pool = rqstp->rq_pool;

	/* did someone call svc_wake_up? */
	if (test_bit(SP_TASK_PENDING, &pool->sp_flags))
		return false;

	/* was a socket queued? */
	if (!lwq_empty(&pool->sp_xprts))
		return false;

	/* are we shutting down? */
	if (svc_thread_should_stop(rqstp))
		return false;

#if defined(CONFIG_SUNRPC_BACKCHANNEL)
	if (svc_is_backchannel(rqstp)) {
		if (!lwq_empty(&rqstp->rq_server->sv_cb_list))
			return false;
	}
#endif

	return true;
}

static void svc_thread_wait_for_work(struct svc_rqst *rqstp)
{
	struct svc_pool *pool = rqstp->rq_pool;

	if (svc_thread_should_sleep(rqstp)) {
		set_current_state(TASK_IDLE | TASK_FREEZABLE);
		llist_add(&rqstp->rq_idle, &pool->sp_idle_threads);
		if (likely(svc_thread_should_sleep(rqstp)))
			schedule();

		while (!llist_del_first_this(&pool->sp_idle_threads,
					     &rqstp->rq_idle)) {
			/* Work just became available.  This thread can only
			 * handle it after removing rqstp from the idle
			 * list. If that attempt failed, some other thread
			 * must have queued itself after finding no
			 * work to do, so that thread has taken responsibly
			 * for this new work.  This thread can safely sleep
			 * until woken again.
			 */
			schedule();
			set_current_state(TASK_IDLE | TASK_FREEZABLE);
		}
		__set_current_state(TASK_RUNNING);
	} else {
		cond_resched();
	}
	try_to_freeze();
}

static void svc_add_new_temp_xprt(struct svc_serv *serv, struct svc_xprt *newxpt)
{
	spin_lock_bh(&serv->sv_lock);
	set_bit(XPT_TEMP, &newxpt->xpt_flags);
	list_add(&newxpt->xpt_list, &serv->sv_tempsocks);
	serv->sv_tmpcnt++;
	if (serv->sv_temptimer.function == NULL) {
		/* setup timer to age temp transports */
		serv->sv_temptimer.function = svc_age_temp_xprts;
		mod_timer(&serv->sv_temptimer,
			  jiffies + svc_conn_age_period * HZ);
	}
	spin_unlock_bh(&serv->sv_lock);
	svc_xprt_received(newxpt);
}

static void svc_handle_xprt(struct svc_rqst *rqstp, struct svc_xprt *xprt)
{
	struct svc_serv *serv = rqstp->rq_server;
	int len = 0;

	if (test_bit(XPT_CLOSE, &xprt->xpt_flags)) {
		if (test_and_clear_bit(XPT_KILL_TEMP, &xprt->xpt_flags))
			xprt->xpt_ops->xpo_kill_temp_xprt(xprt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		svc_delete_xprt(xprt);
		/* Leave XPT_BUSY set on the dead xprt: */
		goto out;
	}
	if (test_bit(XPT_LISTENER, &xprt->xpt_flags)) {
		struct svc_xprt *newxpt;
<<<<<<< HEAD
		newxpt = xprt->xpt_ops->xpo_accept(xprt);
		if (newxpt) {
			/*
			 * We know this module_get will succeed because the
			 * listener holds a reference too
			 */
			__module_get(newxpt->xpt_class->xcl_owner);
			svc_check_conn_limits(xprt->xpt_server);
			spin_lock_bh(&serv->sv_lock);
			set_bit(XPT_TEMP, &newxpt->xpt_flags);
			list_add(&newxpt->xpt_list, &serv->sv_tempsocks);
			serv->sv_tmpcnt++;
			if (serv->sv_temptimer.function == NULL) {
				/* setup timer to age temp transports */
				setup_timer(&serv->sv_temptimer,
					    svc_age_temp_xprts,
					    (unsigned long)serv);
				mod_timer(&serv->sv_temptimer,
					  jiffies + svc_conn_age_period * HZ);
			}
			spin_unlock_bh(&serv->sv_lock);
			svc_xprt_received(newxpt);
		}
	} else if (xprt->xpt_ops->xpo_has_wspace(xprt)) {
		dprintk("svc: server %p, pool %u, transport %p, inuse=%d\n",
			rqstp, pool->sp_id, xprt,
			atomic_read(&xprt->xpt_ref.refcount));
=======
		/*
		 * We know this module_get will succeed because the
		 * listener holds a reference too
		 */
		__module_get(xprt->xpt_class->xcl_owner);
		svc_check_conn_limits(xprt->xpt_server);
		newxpt = xprt->xpt_ops->xpo_accept(xprt);
		if (newxpt) {
			newxpt->xpt_cred = get_cred(xprt->xpt_cred);
			svc_add_new_temp_xprt(serv, newxpt);
			trace_svc_xprt_accept(newxpt, serv->sv_name);
		} else {
			module_put(xprt->xpt_class->xcl_owner);
		}
		svc_xprt_received(xprt);
	} else if (test_bit(XPT_HANDSHAKE, &xprt->xpt_flags)) {
		xprt->xpt_ops->xpo_handshake(xprt);
		svc_xprt_received(xprt);
	} else if (svc_xprt_reserve_slot(rqstp, xprt)) {
		/* XPT_DATA|XPT_DEFERRED case: */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		rqstp->rq_deferred = svc_deferred_dequeue(xprt);
		if (rqstp->rq_deferred)
			len = svc_deferred_recv(rqstp);
		else
			len = xprt->xpt_ops->xpo_recvfrom(rqstp);
<<<<<<< HEAD
		dprintk("svc: got len=%d\n", len);
		rqstp->rq_reserved = serv->sv_max_mesg;
		atomic_add(rqstp->rq_reserved, &xprt->xpt_reserved);
	}
	svc_xprt_received(xprt);

	/* No data, incomplete (TCP) read, or accept() */
	if (len == 0 || len == -EAGAIN)
		goto out;

	clear_bit(XPT_OLD, &xprt->xpt_flags);

	rqstp->rq_secure = svc_port_is_privileged(svc_addr(rqstp));
	rqstp->rq_chandle.defer = svc_defer;

	if (serv->sv_stats)
		serv->sv_stats->netcnt++;
	return len;
out:
	rqstp->rq_res.len = 0;
	svc_xprt_release(rqstp);
	return -EAGAIN;
=======
		rqstp->rq_reserved = serv->sv_max_mesg;
		atomic_add(rqstp->rq_reserved, &xprt->xpt_reserved);
		if (len <= 0)
			goto out;

		trace_svc_xdr_recvfrom(&rqstp->rq_arg);

		clear_bit(XPT_OLD, &xprt->xpt_flags);

		rqstp->rq_chandle.defer = svc_defer;

		if (serv->sv_stats)
			serv->sv_stats->netcnt++;
		percpu_counter_inc(&rqstp->rq_pool->sp_messages_arrived);
		rqstp->rq_stime = ktime_get();
		svc_process(rqstp);
	} else
		svc_xprt_received(xprt);

out:
	rqstp->rq_res.len = 0;
	svc_xprt_release(rqstp);
}

static void svc_thread_wake_next(struct svc_rqst *rqstp)
{
	if (!svc_thread_should_sleep(rqstp))
		/* More work pending after I dequeued some,
		 * wake another worker
		 */
		svc_pool_wake_idle_thread(rqstp->rq_pool);
}

/**
 * svc_recv - Receive and process the next request on any transport
 * @rqstp: an idle RPC service thread
 *
 * This code is carefully organised not to touch any cachelines in
 * the shared svc_serv structure, only cachelines in the local
 * svc_pool.
 */
void svc_recv(struct svc_rqst *rqstp)
{
	struct svc_pool *pool = rqstp->rq_pool;

	if (!svc_alloc_arg(rqstp))
		return;

	svc_thread_wait_for_work(rqstp);

	clear_bit(SP_TASK_PENDING, &pool->sp_flags);

	if (svc_thread_should_stop(rqstp)) {
		svc_thread_wake_next(rqstp);
		return;
	}

	rqstp->rq_xprt = svc_xprt_dequeue(pool);
	if (rqstp->rq_xprt) {
		struct svc_xprt *xprt = rqstp->rq_xprt;

		svc_thread_wake_next(rqstp);
		/* Normally we will wait up to 5 seconds for any required
		 * cache information to be provided.  When there are no
		 * idle threads, we reduce the wait time.
		 */
		if (pool->sp_idle_threads.first)
			rqstp->rq_chandle.thread_wait = 5 * HZ;
		else
			rqstp->rq_chandle.thread_wait = 1 * HZ;

		trace_svc_xprt_dequeue(rqstp);
		svc_handle_xprt(rqstp, xprt);
	}

#if defined(CONFIG_SUNRPC_BACKCHANNEL)
	if (svc_is_backchannel(rqstp)) {
		struct svc_serv *serv = rqstp->rq_server;
		struct rpc_rqst *req;

		req = lwq_dequeue(&serv->sv_cb_list,
				  struct rpc_rqst, rq_bc_list);
		if (req) {
			svc_thread_wake_next(rqstp);
			svc_process_bc(req, rqstp);
		}
	}
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(svc_recv);

/*
 * Drop request
 */
void svc_drop(struct svc_rqst *rqstp)
{
<<<<<<< HEAD
	dprintk("svc: xprt %p dropped request\n", rqstp->rq_xprt);
	svc_xprt_release(rqstp);
}
EXPORT_SYMBOL_GPL(svc_drop);

/*
 * Return reply to client.
 */
int svc_send(struct svc_rqst *rqstp)
{
	struct svc_xprt	*xprt;
	int		len;
	struct xdr_buf	*xb;

	xprt = rqstp->rq_xprt;
	if (!xprt)
		return -EFAULT;

	/* release the receive skb before sending the reply */
	rqstp->rq_xprt->xpt_ops->xpo_release_rqst(rqstp);
=======
	trace_svc_drop(rqstp);
}
EXPORT_SYMBOL_GPL(svc_drop);

/**
 * svc_send - Return reply to client
 * @rqstp: RPC transaction context
 *
 */
void svc_send(struct svc_rqst *rqstp)
{
	struct svc_xprt	*xprt;
	struct xdr_buf	*xb;
	int status;

	xprt = rqstp->rq_xprt;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* calculate over-all length */
	xb = &rqstp->rq_res;
	xb->len = xb->head[0].iov_len +
		xb->page_len +
		xb->tail[0].iov_len;
<<<<<<< HEAD

	/* Grab mutex to serialize outgoing data. */
	mutex_lock(&xprt->xpt_mutex);
	if (test_bit(XPT_DEAD, &xprt->xpt_flags)
			|| test_bit(XPT_CLOSE, &xprt->xpt_flags))
		len = -ENOTCONN;
	else
		len = xprt->xpt_ops->xpo_sendto(rqstp);
	mutex_unlock(&xprt->xpt_mutex);
	rpc_wake_up(&xprt->xpt_bc_pending);
	svc_xprt_release(rqstp);

	if (len == -ECONNREFUSED || len == -ENOTCONN || len == -EAGAIN)
		return 0;
	return len;
=======
	trace_svc_xdr_sendto(rqstp->rq_xid, xb);
	trace_svc_stats_latency(rqstp);

	status = xprt->xpt_ops->xpo_sendto(rqstp);

	trace_svc_send(rqstp, status);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Timer function to close old temporary transports, using
 * a mark-and-sweep algorithm.
 */
<<<<<<< HEAD
static void svc_age_temp_xprts(unsigned long closure)
{
	struct svc_serv *serv = (struct svc_serv *)closure;
=======
static void svc_age_temp_xprts(struct timer_list *t)
{
	struct svc_serv *serv = from_timer(serv, t, sv_temptimer);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct svc_xprt *xprt;
	struct list_head *le, *next;

	dprintk("svc_age_temp_xprts\n");

	if (!spin_trylock_bh(&serv->sv_lock)) {
		/* busy, try again 1 sec later */
		dprintk("svc_age_temp_xprts: busy\n");
		mod_timer(&serv->sv_temptimer, jiffies + HZ);
		return;
	}

	list_for_each_safe(le, next, &serv->sv_tempsocks) {
		xprt = list_entry(le, struct svc_xprt, xpt_list);

		/* First time through, just mark it OLD. Second time
		 * through, close it. */
		if (!test_and_set_bit(XPT_OLD, &xprt->xpt_flags))
			continue;
<<<<<<< HEAD
		if (atomic_read(&xprt->xpt_ref.refcount) > 1 ||
=======
		if (kref_read(&xprt->xpt_ref) > 1 ||
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		    test_bit(XPT_BUSY, &xprt->xpt_flags))
			continue;
		list_del_init(le);
		set_bit(XPT_CLOSE, &xprt->xpt_flags);
<<<<<<< HEAD
		set_bit(XPT_DETACHED, &xprt->xpt_flags);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		dprintk("queuing xprt %p for closing\n", xprt);

		/* a thread will dequeue and close it soon */
		svc_xprt_enqueue(xprt);
	}
	spin_unlock_bh(&serv->sv_lock);

	mod_timer(&serv->sv_temptimer, jiffies + svc_conn_age_period * HZ);
}

<<<<<<< HEAD
=======
/* Close temporary transports whose xpt_local matches server_addr immediately
 * instead of waiting for them to be picked up by the timer.
 *
 * This is meant to be called from a notifier_block that runs when an ip
 * address is deleted.
 */
void svc_age_temp_xprts_now(struct svc_serv *serv, struct sockaddr *server_addr)
{
	struct svc_xprt *xprt;
	struct list_head *le, *next;
	LIST_HEAD(to_be_closed);

	spin_lock_bh(&serv->sv_lock);
	list_for_each_safe(le, next, &serv->sv_tempsocks) {
		xprt = list_entry(le, struct svc_xprt, xpt_list);
		if (rpc_cmp_addr(server_addr, (struct sockaddr *)
				&xprt->xpt_local)) {
			dprintk("svc_age_temp_xprts_now: found %p\n", xprt);
			list_move(le, &to_be_closed);
		}
	}
	spin_unlock_bh(&serv->sv_lock);

	while (!list_empty(&to_be_closed)) {
		le = to_be_closed.next;
		list_del_init(le);
		xprt = list_entry(le, struct svc_xprt, xpt_list);
		set_bit(XPT_CLOSE, &xprt->xpt_flags);
		set_bit(XPT_KILL_TEMP, &xprt->xpt_flags);
		dprintk("svc_age_temp_xprts_now: queuing xprt %p for closing\n",
				xprt);
		svc_xprt_enqueue(xprt);
	}
}
EXPORT_SYMBOL_GPL(svc_age_temp_xprts_now);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void call_xpt_users(struct svc_xprt *xprt)
{
	struct svc_xpt_user *u;

	spin_lock(&xprt->xpt_lock);
	while (!list_empty(&xprt->xpt_users)) {
		u = list_first_entry(&xprt->xpt_users, struct svc_xpt_user, list);
<<<<<<< HEAD
		list_del(&u->list);
=======
		list_del_init(&u->list);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		u->callback(u);
	}
	spin_unlock(&xprt->xpt_lock);
}

/*
 * Remove a dead transport
 */
static void svc_delete_xprt(struct svc_xprt *xprt)
{
	struct svc_serv	*serv = xprt->xpt_server;
	struct svc_deferred_req *dr;

<<<<<<< HEAD
	/* Only do this once */
	if (test_and_set_bit(XPT_DEAD, &xprt->xpt_flags))
		BUG();

	dprintk("svc: svc_delete_xprt(%p)\n", xprt);
	xprt->xpt_ops->xpo_detach(xprt);

	spin_lock_bh(&serv->sv_lock);
	if (!test_and_set_bit(XPT_DETACHED, &xprt->xpt_flags))
		list_del_init(&xprt->xpt_list);
	BUG_ON(!list_empty(&xprt->xpt_ready));
=======
	if (test_and_set_bit(XPT_DEAD, &xprt->xpt_flags))
		return;

	trace_svc_xprt_detach(xprt);
	xprt->xpt_ops->xpo_detach(xprt);
	if (xprt->xpt_bc_xprt)
		xprt->xpt_bc_xprt->ops->close(xprt->xpt_bc_xprt);

	spin_lock_bh(&serv->sv_lock);
	list_del_init(&xprt->xpt_list);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (test_bit(XPT_TEMP, &xprt->xpt_flags))
		serv->sv_tmpcnt--;
	spin_unlock_bh(&serv->sv_lock);

	while ((dr = svc_deferred_dequeue(xprt)) != NULL)
<<<<<<< HEAD
		kfree(dr);
=======
		free_deferred(xprt, dr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	call_xpt_users(xprt);
	svc_xprt_put(xprt);
}

<<<<<<< HEAD
void svc_close_xprt(struct svc_xprt *xprt)
{
=======
/**
 * svc_xprt_close - Close a client connection
 * @xprt: transport to disconnect
 *
 */
void svc_xprt_close(struct svc_xprt *xprt)
{
	trace_svc_xprt_close(xprt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	set_bit(XPT_CLOSE, &xprt->xpt_flags);
	if (test_and_set_bit(XPT_BUSY, &xprt->xpt_flags))
		/* someone else will have to effect the close */
		return;
	/*
	 * We expect svc_close_xprt() to work even when no threads are
	 * running (e.g., while configuring the server before starting
	 * any threads), so if the transport isn't busy, we delete
	 * it ourself:
	 */
	svc_delete_xprt(xprt);
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(svc_close_xprt);

static void svc_close_list(struct list_head *xprt_list, struct net *net)
{
	struct svc_xprt *xprt;

	list_for_each_entry(xprt, xprt_list, xpt_list) {
		if (xprt->xpt_net != net)
			continue;
		set_bit(XPT_CLOSE, &xprt->xpt_flags);
		set_bit(XPT_BUSY, &xprt->xpt_flags);
	}
}

static void svc_clear_pools(struct svc_serv *serv, struct net *net)
{
	struct svc_pool *pool;
	struct svc_xprt *xprt;
	struct svc_xprt *tmp;
	int i;

	for (i = 0; i < serv->sv_nrpools; i++) {
		pool = &serv->sv_pools[i];

		spin_lock_bh(&pool->sp_lock);
		list_for_each_entry_safe(xprt, tmp, &pool->sp_sockets, xpt_ready) {
			if (xprt->xpt_net != net)
				continue;
			list_del_init(&xprt->xpt_ready);
		}
		spin_unlock_bh(&pool->sp_lock);
	}
}

static void svc_clear_list(struct list_head *xprt_list, struct net *net)
{
	struct svc_xprt *xprt;
	struct svc_xprt *tmp;

	list_for_each_entry_safe(xprt, tmp, xprt_list, xpt_list) {
		if (xprt->xpt_net != net)
			continue;
		svc_delete_xprt(xprt);
	}
	list_for_each_entry(xprt, xprt_list, xpt_list)
		BUG_ON(xprt->xpt_net == net);
}

void svc_close_net(struct svc_serv *serv, struct net *net)
{
	svc_close_list(&serv->sv_tempsocks, net);
	svc_close_list(&serv->sv_permsocks, net);

	svc_clear_pools(serv, net);
	/*
	 * At this point the sp_sockets lists will stay empty, since
	 * svc_enqueue will not add new entries without taking the
	 * sp_lock and checking XPT_BUSY.
	 */
	svc_clear_list(&serv->sv_tempsocks, net);
	svc_clear_list(&serv->sv_permsocks, net);
}
=======
EXPORT_SYMBOL_GPL(svc_xprt_close);

static int svc_close_list(struct svc_serv *serv, struct list_head *xprt_list, struct net *net)
{
	struct svc_xprt *xprt;
	int ret = 0;

	spin_lock_bh(&serv->sv_lock);
	list_for_each_entry(xprt, xprt_list, xpt_list) {
		if (xprt->xpt_net != net)
			continue;
		ret++;
		set_bit(XPT_CLOSE, &xprt->xpt_flags);
		svc_xprt_enqueue(xprt);
	}
	spin_unlock_bh(&serv->sv_lock);
	return ret;
}

static void svc_clean_up_xprts(struct svc_serv *serv, struct net *net)
{
	struct svc_xprt *xprt;
	int i;

	for (i = 0; i < serv->sv_nrpools; i++) {
		struct svc_pool *pool = &serv->sv_pools[i];
		struct llist_node *q, **t1, *t2;

		q = lwq_dequeue_all(&pool->sp_xprts);
		lwq_for_each_safe(xprt, t1, t2, &q, xpt_ready) {
			if (xprt->xpt_net == net) {
				set_bit(XPT_CLOSE, &xprt->xpt_flags);
				svc_delete_xprt(xprt);
				xprt = NULL;
			}
		}

		if (q)
			lwq_enqueue_batch(q, &pool->sp_xprts);
	}
}

/**
 * svc_xprt_destroy_all - Destroy transports associated with @serv
 * @serv: RPC service to be shut down
 * @net: target network namespace
 *
 * Server threads may still be running (especially in the case where the
 * service is still running in other network namespaces).
 *
 * So we shut down sockets the same way we would on a running server, by
 * setting XPT_CLOSE, enqueuing, and letting a thread pick it up to do
 * the close.  In the case there are no such other threads,
 * threads running, svc_clean_up_xprts() does a simple version of a
 * server's main event loop, and in the case where there are other
 * threads, we may need to wait a little while and then check again to
 * see if they're done.
 */
void svc_xprt_destroy_all(struct svc_serv *serv, struct net *net)
{
	int delay = 0;

	while (svc_close_list(serv, &serv->sv_permsocks, net) +
	       svc_close_list(serv, &serv->sv_tempsocks, net)) {

		svc_clean_up_xprts(serv, net);
		msleep(delay++);
	}
}
EXPORT_SYMBOL_GPL(svc_xprt_destroy_all);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Handle defer and revisit of requests
 */

static void svc_revisit(struct cache_deferred_req *dreq, int too_many)
{
	struct svc_deferred_req *dr =
		container_of(dreq, struct svc_deferred_req, handle);
	struct svc_xprt *xprt = dr->xprt;

	spin_lock(&xprt->xpt_lock);
	set_bit(XPT_DEFERRED, &xprt->xpt_flags);
	if (too_many || test_bit(XPT_DEAD, &xprt->xpt_flags)) {
		spin_unlock(&xprt->xpt_lock);
<<<<<<< HEAD
		dprintk("revisit canceled\n");
		svc_xprt_put(xprt);
		kfree(dr);
		return;
	}
	dprintk("revisit queued\n");
	dr->xprt = NULL;
	list_add(&dr->handle.recent, &xprt->xpt_deferred);
	spin_unlock(&xprt->xpt_lock);
=======
		trace_svc_defer_drop(dr);
		free_deferred(xprt, dr);
		svc_xprt_put(xprt);
		return;
	}
	dr->xprt = NULL;
	list_add(&dr->handle.recent, &xprt->xpt_deferred);
	spin_unlock(&xprt->xpt_lock);
	trace_svc_defer_queue(dr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	svc_xprt_enqueue(xprt);
	svc_xprt_put(xprt);
}

/*
 * Save the request off for later processing. The request buffer looks
 * like this:
 *
 * <xprt-header><rpc-header><rpc-pagelist><rpc-tail>
 *
 * This code can only handle requests that consist of an xprt-header
 * and rpc-header.
 */
static struct cache_deferred_req *svc_defer(struct cache_req *req)
{
	struct svc_rqst *rqstp = container_of(req, struct svc_rqst, rq_chandle);
	struct svc_deferred_req *dr;

<<<<<<< HEAD
	if (rqstp->rq_arg.page_len || !rqstp->rq_usedeferral)
=======
	if (rqstp->rq_arg.page_len || !test_bit(RQ_USEDEFERRAL, &rqstp->rq_flags))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return NULL; /* if more than a page, give up FIXME */
	if (rqstp->rq_deferred) {
		dr = rqstp->rq_deferred;
		rqstp->rq_deferred = NULL;
	} else {
		size_t skip;
		size_t size;
		/* FIXME maybe discard if size too large */
		size = sizeof(struct svc_deferred_req) + rqstp->rq_arg.len;
		dr = kmalloc(size, GFP_KERNEL);
		if (dr == NULL)
			return NULL;

		dr->handle.owner = rqstp->rq_server;
		dr->prot = rqstp->rq_prot;
		memcpy(&dr->addr, &rqstp->rq_addr, rqstp->rq_addrlen);
		dr->addrlen = rqstp->rq_addrlen;
		dr->daddr = rqstp->rq_daddr;
		dr->argslen = rqstp->rq_arg.len >> 2;
<<<<<<< HEAD
		dr->xprt_hlen = rqstp->rq_xprt_hlen;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/* back up head to the start of the buffer and copy */
		skip = rqstp->rq_arg.len - rqstp->rq_arg.head[0].iov_len;
		memcpy(dr->args, rqstp->rq_arg.head[0].iov_base - skip,
		       dr->argslen << 2);
	}
<<<<<<< HEAD
	svc_xprt_get(rqstp->rq_xprt);
	dr->xprt = rqstp->rq_xprt;
	rqstp->rq_dropme = true;
=======
	dr->xprt_ctxt = rqstp->rq_xprt_ctxt;
	rqstp->rq_xprt_ctxt = NULL;
	trace_svc_defer(rqstp);
	svc_xprt_get(rqstp->rq_xprt);
	dr->xprt = rqstp->rq_xprt;
	set_bit(RQ_DROPME, &rqstp->rq_flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	dr->handle.revisit = svc_revisit;
	return &dr->handle;
}

/*
 * recv data from a deferred request into an active one
 */
<<<<<<< HEAD
static int svc_deferred_recv(struct svc_rqst *rqstp)
{
	struct svc_deferred_req *dr = rqstp->rq_deferred;

	/* setup iov_base past transport header */
	rqstp->rq_arg.head[0].iov_base = dr->args + (dr->xprt_hlen>>2);
	/* The iov_len does not include the transport header bytes */
	rqstp->rq_arg.head[0].iov_len = (dr->argslen<<2) - dr->xprt_hlen;
	rqstp->rq_arg.page_len = 0;
	/* The rq_arg.len includes the transport header bytes */
	rqstp->rq_arg.len     = dr->argslen<<2;
=======
static noinline int svc_deferred_recv(struct svc_rqst *rqstp)
{
	struct svc_deferred_req *dr = rqstp->rq_deferred;

	trace_svc_defer_recv(dr);

	/* setup iov_base past transport header */
	rqstp->rq_arg.head[0].iov_base = dr->args;
	/* The iov_len does not include the transport header bytes */
	rqstp->rq_arg.head[0].iov_len = dr->argslen << 2;
	rqstp->rq_arg.page_len = 0;
	/* The rq_arg.len includes the transport header bytes */
	rqstp->rq_arg.len     = dr->argslen << 2;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rqstp->rq_prot        = dr->prot;
	memcpy(&rqstp->rq_addr, &dr->addr, dr->addrlen);
	rqstp->rq_addrlen     = dr->addrlen;
	/* Save off transport header len in case we get deferred again */
<<<<<<< HEAD
	rqstp->rq_xprt_hlen   = dr->xprt_hlen;
	rqstp->rq_daddr       = dr->daddr;
	rqstp->rq_respages    = rqstp->rq_pages;
	return (dr->argslen<<2) - dr->xprt_hlen;
=======
	rqstp->rq_daddr       = dr->daddr;
	rqstp->rq_respages    = rqstp->rq_pages;
	rqstp->rq_xprt_ctxt   = dr->xprt_ctxt;

	dr->xprt_ctxt = NULL;
	svc_xprt_received(rqstp->rq_xprt);
	return dr->argslen << 2;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}


static struct svc_deferred_req *svc_deferred_dequeue(struct svc_xprt *xprt)
{
	struct svc_deferred_req *dr = NULL;

	if (!test_bit(XPT_DEFERRED, &xprt->xpt_flags))
		return NULL;
	spin_lock(&xprt->xpt_lock);
	if (!list_empty(&xprt->xpt_deferred)) {
		dr = list_entry(xprt->xpt_deferred.next,
				struct svc_deferred_req,
				handle.recent);
		list_del_init(&dr->handle.recent);
	} else
		clear_bit(XPT_DEFERRED, &xprt->xpt_flags);
	spin_unlock(&xprt->xpt_lock);
	return dr;
}

/**
 * svc_find_xprt - find an RPC transport instance
 * @serv: pointer to svc_serv to search
 * @xcl_name: C string containing transport's class name
 * @net: owner net pointer
 * @af: Address family of transport's local address
 * @port: transport's IP port number
 *
 * Return the transport instance pointer for the endpoint accepting
 * connections/peer traffic from the specified transport class,
 * address family and port.
 *
 * Specifying 0 for the address family or port is effectively a
 * wild-card, and will result in matching the first transport in the
 * service's list that has a matching class name.
 */
struct svc_xprt *svc_find_xprt(struct svc_serv *serv, const char *xcl_name,
			       struct net *net, const sa_family_t af,
			       const unsigned short port)
{
	struct svc_xprt *xprt;
	struct svc_xprt *found = NULL;

	/* Sanity check the args */
	if (serv == NULL || xcl_name == NULL)
		return found;

	spin_lock_bh(&serv->sv_lock);
	list_for_each_entry(xprt, &serv->sv_permsocks, xpt_list) {
		if (xprt->xpt_net != net)
			continue;
		if (strcmp(xprt->xpt_class->xcl_name, xcl_name))
			continue;
		if (af != AF_UNSPEC && af != xprt->xpt_local.ss_family)
			continue;
		if (port != 0 && port != svc_xprt_local_port(xprt))
			continue;
		found = xprt;
		svc_xprt_get(xprt);
		break;
	}
	spin_unlock_bh(&serv->sv_lock);
	return found;
}
EXPORT_SYMBOL_GPL(svc_find_xprt);

static int svc_one_xprt_name(const struct svc_xprt *xprt,
			     char *pos, int remaining)
{
	int len;

	len = snprintf(pos, remaining, "%s %u\n",
			xprt->xpt_class->xcl_name,
			svc_xprt_local_port(xprt));
	if (len >= remaining)
		return -ENAMETOOLONG;
	return len;
}

/**
 * svc_xprt_names - format a buffer with a list of transport names
 * @serv: pointer to an RPC service
 * @buf: pointer to a buffer to be filled in
 * @buflen: length of buffer to be filled in
 *
 * Fills in @buf with a string containing a list of transport names,
 * each name terminated with '\n'.
 *
 * Returns positive length of the filled-in string on success; otherwise
 * a negative errno value is returned if an error occurs.
 */
int svc_xprt_names(struct svc_serv *serv, char *buf, const int buflen)
{
	struct svc_xprt *xprt;
	int len, totlen;
	char *pos;

	/* Sanity check args */
	if (!serv)
		return 0;

	spin_lock_bh(&serv->sv_lock);

	pos = buf;
	totlen = 0;
	list_for_each_entry(xprt, &serv->sv_permsocks, xpt_list) {
		len = svc_one_xprt_name(xprt, pos, buflen - totlen);
		if (len < 0) {
			*buf = '\0';
			totlen = len;
		}
		if (len <= 0)
			break;

		pos += len;
		totlen += len;
	}

	spin_unlock_bh(&serv->sv_lock);
	return totlen;
}
EXPORT_SYMBOL_GPL(svc_xprt_names);

<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*----------------------------------------------------------------------------*/

static void *svc_pool_stats_start(struct seq_file *m, loff_t *pos)
{
	unsigned int pidx = (unsigned int)*pos;
<<<<<<< HEAD
	struct svc_serv *serv = m->private;

	dprintk("svc_pool_stats_start, *pidx=%u\n", pidx);

	if (!pidx)
		return SEQ_START_TOKEN;
	return (pidx > serv->sv_nrpools ? NULL : &serv->sv_pools[pidx-1]);
=======
	struct svc_info *si = m->private;

	dprintk("svc_pool_stats_start, *pidx=%u\n", pidx);

	mutex_lock(si->mutex);

	if (!pidx)
		return SEQ_START_TOKEN;
	if (!si->serv)
		return NULL;
	return pidx > si->serv->sv_nrpools ? NULL
		: &si->serv->sv_pools[pidx - 1];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void *svc_pool_stats_next(struct seq_file *m, void *p, loff_t *pos)
{
	struct svc_pool *pool = p;
<<<<<<< HEAD
	struct svc_serv *serv = m->private;

	dprintk("svc_pool_stats_next, *pos=%llu\n", *pos);

	if (p == SEQ_START_TOKEN) {
=======
	struct svc_info *si = m->private;
	struct svc_serv *serv = si->serv;

	dprintk("svc_pool_stats_next, *pos=%llu\n", *pos);

	if (!serv) {
		pool = NULL;
	} else if (p == SEQ_START_TOKEN) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		pool = &serv->sv_pools[0];
	} else {
		unsigned int pidx = (pool - &serv->sv_pools[0]);
		if (pidx < serv->sv_nrpools-1)
			pool = &serv->sv_pools[pidx+1];
		else
			pool = NULL;
	}
	++*pos;
	return pool;
}

static void svc_pool_stats_stop(struct seq_file *m, void *p)
{
<<<<<<< HEAD
=======
	struct svc_info *si = m->private;

	mutex_unlock(si->mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int svc_pool_stats_show(struct seq_file *m, void *p)
{
	struct svc_pool *pool = p;

	if (p == SEQ_START_TOKEN) {
		seq_puts(m, "# pool packets-arrived sockets-enqueued threads-woken threads-timedout\n");
		return 0;
	}

<<<<<<< HEAD
	seq_printf(m, "%u %lu %lu %lu %lu\n",
		pool->sp_id,
		pool->sp_stats.packets,
		pool->sp_stats.sockets_queued,
		pool->sp_stats.threads_woken,
		pool->sp_stats.threads_timedout);
=======
	seq_printf(m, "%u %llu %llu %llu 0\n",
		   pool->sp_id,
		   percpu_counter_sum_positive(&pool->sp_messages_arrived),
		   percpu_counter_sum_positive(&pool->sp_sockets_queued),
		   percpu_counter_sum_positive(&pool->sp_threads_woken));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static const struct seq_operations svc_pool_stats_seq_ops = {
	.start	= svc_pool_stats_start,
	.next	= svc_pool_stats_next,
	.stop	= svc_pool_stats_stop,
	.show	= svc_pool_stats_show,
};

<<<<<<< HEAD
int svc_pool_stats_open(struct svc_serv *serv, struct file *file)
{
	int err;

	err = seq_open(file, &svc_pool_stats_seq_ops);
	if (!err)
		((struct seq_file *) file->private_data)->private = serv;
	return err;
=======
int svc_pool_stats_open(struct svc_info *info, struct file *file)
{
	struct seq_file *seq;
	int err;

	err = seq_open(file, &svc_pool_stats_seq_ops);
	if (err)
		return err;
	seq = file->private_data;
	seq->private = info;

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL(svc_pool_stats_open);

/*----------------------------------------------------------------------------*/
