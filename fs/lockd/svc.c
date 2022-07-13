<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * linux/fs/lockd/svc.c
 *
 * This is the central lockd service.
 *
 * FIXME: Separate the lockd NFS server functionality from the lockd NFS
 * 	  client functionality. Oh why didn't Sun create two separate
 *	  services in the first place?
 *
 * Authors:	Olaf Kirch (okir@monad.swb.de)
 *
 * Copyright (C) 1995, 1996 Olaf Kirch <okir@monad.swb.de>
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/sysctl.h>
#include <linux/moduleparam.h>

<<<<<<< HEAD
#include <linux/sched.h>
=======
#include <linux/sched/signal.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/errno.h>
#include <linux/in.h>
#include <linux/uio.h>
#include <linux/smp.h>
#include <linux/mutex.h>
<<<<<<< HEAD
#include <linux/kthread.h>
#include <linux/freezer.h>
=======
#include <linux/freezer.h>
#include <linux/inetdevice.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <linux/sunrpc/types.h>
#include <linux/sunrpc/stats.h>
#include <linux/sunrpc/clnt.h>
#include <linux/sunrpc/svc.h>
#include <linux/sunrpc/svcsock.h>
<<<<<<< HEAD
#include <net/ip.h>
=======
#include <linux/sunrpc/svc_xprt.h>
#include <net/ip.h>
#include <net/addrconf.h>
#include <net/ipv6.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/lockd/lockd.h>
#include <linux/nfs.h>

#include "netns.h"
<<<<<<< HEAD

#define NLMDBG_FACILITY		NLMDBG_SVC
#define LOCKD_BUFSIZE		(1024 + NLMSVC_XDRSIZE)
#define ALLOWED_SIGS		(sigmask(SIGKILL))

static struct svc_program	nlmsvc_program;

struct nlmsvc_binding *		nlmsvc_ops;
=======
#include "procfs.h"

#define NLMDBG_FACILITY		NLMDBG_SVC
#define LOCKD_BUFSIZE		(1024 + NLMSVC_XDRSIZE)

static struct svc_program	nlmsvc_program;

const struct nlmsvc_binding	*nlmsvc_ops;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
EXPORT_SYMBOL_GPL(nlmsvc_ops);

static DEFINE_MUTEX(nlmsvc_mutex);
static unsigned int		nlmsvc_users;
<<<<<<< HEAD
static struct task_struct	*nlmsvc_task;
static struct svc_rqst		*nlmsvc_rqst;
unsigned long			nlmsvc_timeout;

int lockd_net_id;
=======
static struct svc_serv		*nlmsvc_serv;
unsigned long			nlmsvc_timeout;

static void nlmsvc_request_retry(struct timer_list *tl)
{
	svc_wake_up(nlmsvc_serv);
}
DEFINE_TIMER(nlmsvc_retry, nlmsvc_request_retry);

unsigned int lockd_net_id;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * These can be set at insmod time (useful for NFS as root filesystem),
 * and also changed through the sysctl interface.  -- Jamie Lokier, Aug 2003
 */
static unsigned long		nlm_grace_period;
static unsigned long		nlm_timeout = LOCKD_DFLT_TIMEO;
static int			nlm_udpport, nlm_tcpport;

/* RLIM_NOFILE defaults to 1024. That seems like a reasonable default here. */
static unsigned int		nlm_max_connections = 1024;

/*
 * Constants needed for the sysctl interface.
 */
static const unsigned long	nlm_grace_period_min = 0;
static const unsigned long	nlm_grace_period_max = 240;
static const unsigned long	nlm_timeout_min = 3;
static const unsigned long	nlm_timeout_max = 20;
<<<<<<< HEAD
static const int		nlm_port_min = 0, nlm_port_max = 65535;

#ifdef CONFIG_SYSCTL
=======

#ifdef CONFIG_SYSCTL
static const int		nlm_port_min = 0, nlm_port_max = 65535;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct ctl_table_header * nlm_sysctl_table;
#endif

static unsigned long get_lockd_grace_period(void)
{
	/* Note: nlm_timeout should always be nonzero */
	if (nlm_grace_period)
		return roundup(nlm_grace_period, nlm_timeout) * HZ;
	else
		return nlm_timeout * 5 * HZ;
}

<<<<<<< HEAD
static struct lock_manager lockd_manager = {
};

static void grace_ender(struct work_struct *not_used)
{
	locks_end_grace(&lockd_manager);
}

static DECLARE_DELAYED_WORK(grace_period_end, grace_ender);

static void set_grace_period(void)
{
	unsigned long grace_period = get_lockd_grace_period();

	locks_start_grace(&lockd_manager);
	cancel_delayed_work_sync(&grace_period_end);
	schedule_delayed_work(&grace_period_end, grace_period);
}

static void restart_grace(void)
{
	if (nlmsvc_ops) {
		cancel_delayed_work_sync(&grace_period_end);
		locks_end_grace(&lockd_manager);
		nlmsvc_invalidate_all();
		set_grace_period();
	}
=======
static void grace_ender(struct work_struct *grace)
{
	struct delayed_work *dwork = to_delayed_work(grace);
	struct lockd_net *ln = container_of(dwork, struct lockd_net,
					    grace_period_end);

	locks_end_grace(&ln->lockd_manager);
}

static void set_grace_period(struct net *net)
{
	unsigned long grace_period = get_lockd_grace_period();
	struct lockd_net *ln = net_generic(net, lockd_net_id);

	locks_start_grace(net, &ln->lockd_manager);
	cancel_delayed_work_sync(&ln->grace_period_end);
	schedule_delayed_work(&ln->grace_period_end, grace_period);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * This is the lockd kernel thread
 */
static int
lockd(void *vrqstp)
{
<<<<<<< HEAD
	int		err = 0, preverr = 0;
	struct svc_rqst *rqstp = vrqstp;
=======
	struct svc_rqst *rqstp = vrqstp;
	struct net *net = &init_net;
	struct lockd_net *ln = net_generic(net, lockd_net_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* try_to_freeze() is called from svc_recv() */
	set_freezable();

<<<<<<< HEAD
	/* Allow SIGKILL to tell lockd to drop all of its locks */
	allow_signal(SIGKILL);

	dprintk("NFS locking service started (ver " LOCKD_VERSION ").\n");

	if (!nlm_timeout)
		nlm_timeout = LOCKD_DFLT_TIMEO;
	nlmsvc_timeout = nlm_timeout * HZ;

	set_grace_period();

=======
	dprintk("NFS locking service started (ver " LOCKD_VERSION ").\n");

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * The main request loop. We don't terminate until the last
	 * NFS mount or NFS daemon has gone away.
	 */
<<<<<<< HEAD
	while (!kthread_should_stop()) {
		long timeout = MAX_SCHEDULE_TIMEOUT;
		RPC_IFDEBUG(char buf[RPC_MAX_ADDRBUFLEN]);

		/* update sv_maxconn if it has changed */
		rqstp->rq_server->sv_maxconn = nlm_max_connections;

		if (signalled()) {
			flush_signals(current);
			restart_grace();
			continue;
		}

		timeout = nlmsvc_retry_blocked();

		/*
		 * Find a socket with data available and call its
		 * recvfrom routine.
		 */
		err = svc_recv(rqstp, timeout);
		if (err == -EAGAIN || err == -EINTR) {
			preverr = err;
			continue;
		}
		if (err < 0) {
			if (err != preverr) {
				printk(KERN_WARNING "%s: unexpected error "
					"from svc_recv (%d)\n", __func__, err);
				preverr = err;
			}
			schedule_timeout_interruptible(HZ);
			continue;
		}
		preverr = err;

		dprintk("lockd: request from %s\n",
				svc_print_addr(rqstp, buf, sizeof(buf)));

		svc_process(rqstp);
	}
	flush_signals(current);
	cancel_delayed_work_sync(&grace_period_end);
	locks_end_grace(&lockd_manager);
	if (nlmsvc_ops)
		nlmsvc_invalidate_all();
	nlm_shutdown_hosts();
=======
	while (!svc_thread_should_stop(rqstp)) {
		/* update sv_maxconn if it has changed */
		rqstp->rq_server->sv_maxconn = nlm_max_connections;

		nlmsvc_retry_blocked(rqstp);
		svc_recv(rqstp);
	}
	if (nlmsvc_ops)
		nlmsvc_invalidate_all();
	nlm_shutdown_hosts();
	cancel_delayed_work_sync(&ln->grace_period_end);
	locks_end_grace(&ln->lockd_manager);

	dprintk("lockd_down: service stopped\n");

	svc_exit_thread(rqstp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static int create_lockd_listener(struct svc_serv *serv, const char *name,
				 struct net *net, const int family,
<<<<<<< HEAD
				 const unsigned short port)
=======
				 const unsigned short port,
				 const struct cred *cred)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct svc_xprt *xprt;

	xprt = svc_find_xprt(serv, name, net, family, 0);
	if (xprt == NULL)
<<<<<<< HEAD
		return svc_create_xprt(serv, name, net, family, port,
						SVC_SOCK_DEFAULTS);
=======
		return svc_xprt_create(serv, name, net, family, port,
				       SVC_SOCK_DEFAULTS, cred);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	svc_xprt_put(xprt);
	return 0;
}

static int create_lockd_family(struct svc_serv *serv, struct net *net,
<<<<<<< HEAD
			       const int family)
{
	int err;

	err = create_lockd_listener(serv, "udp", net, family, nlm_udpport);
	if (err < 0)
		return err;

	return create_lockd_listener(serv, "tcp", net, family, nlm_tcpport);
=======
			       const int family, const struct cred *cred)
{
	int err;

	err = create_lockd_listener(serv, "udp", net, family, nlm_udpport,
			cred);
	if (err < 0)
		return err;

	return create_lockd_listener(serv, "tcp", net, family, nlm_tcpport,
			cred);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Ensure there are active UDP and TCP listeners for lockd.
 *
 * Even if we have only TCP NFS mounts and/or TCP NFSDs, some
 * local services (such as rpc.statd) still require UDP, and
 * some NFS servers do not yet support NLM over TCP.
 *
 * Returns zero if all listeners are available; otherwise a
 * negative errno value is returned.
 */
<<<<<<< HEAD
static int make_socks(struct svc_serv *serv, struct net *net)
=======
static int make_socks(struct svc_serv *serv, struct net *net,
		const struct cred *cred)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	static int warned;
	int err;

<<<<<<< HEAD
	err = create_lockd_family(serv, net, PF_INET);
	if (err < 0)
		goto out_err;

	err = create_lockd_family(serv, net, PF_INET6);
=======
	err = create_lockd_family(serv, net, PF_INET, cred);
	if (err < 0)
		goto out_err;

	err = create_lockd_family(serv, net, PF_INET6, cred);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err < 0 && err != -EAFNOSUPPORT)
		goto out_err;

	warned = 0;
	return 0;

out_err:
	if (warned++ == 0)
		printk(KERN_WARNING
			"lockd_up: makesock failed, error=%d\n", err);
<<<<<<< HEAD
	return err;
}

static int lockd_up_net(struct svc_serv *serv, struct net *net)
=======
	svc_xprt_destroy_all(serv, net);
	svc_rpcb_cleanup(serv, net);
	return err;
}

static int lockd_up_net(struct svc_serv *serv, struct net *net,
		const struct cred *cred)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct lockd_net *ln = net_generic(net, lockd_net_id);
	int error;

	if (ln->nlmsvc_users++)
		return 0;

<<<<<<< HEAD
	error = svc_rpcb_setup(serv, net);
	if (error)
		goto err_rpcb;

	error = make_socks(serv, net);
	if (error < 0)
		goto err_socks;
	return 0;

err_socks:
	svc_rpcb_cleanup(serv, net);
err_rpcb:
=======
	error = svc_bind(serv, net);
	if (error)
		goto err_bind;

	error = make_socks(serv, net, cred);
	if (error < 0)
		goto err_bind;
	set_grace_period(net);
	dprintk("%s: per-net data created; net=%x\n", __func__, net->ns.inum);
	return 0;

err_bind:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ln->nlmsvc_users--;
	return error;
}

static void lockd_down_net(struct svc_serv *serv, struct net *net)
{
	struct lockd_net *ln = net_generic(net, lockd_net_id);

	if (ln->nlmsvc_users) {
		if (--ln->nlmsvc_users == 0) {
			nlm_shutdown_hosts_net(net);
<<<<<<< HEAD
			svc_shutdown_net(serv, net);
		}
	} else {
		printk(KERN_ERR "lockd_down_net: no users! task=%p, net=%p\n",
				nlmsvc_task, net);
=======
			cancel_delayed_work_sync(&ln->grace_period_end);
			locks_end_grace(&ln->lockd_manager);
			svc_xprt_destroy_all(serv, net);
			svc_rpcb_cleanup(serv, net);
		}
	} else {
		pr_err("%s: no users! net=%x\n",
			__func__, net->ns.inum);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		BUG();
	}
}

<<<<<<< HEAD
/*
 * Bring up the lockd process if it's not already up.
 */
int lockd_up(struct net *net)
{
	struct svc_serv *serv;
	int		error = 0;
	struct lockd_net *ln = net_generic(net, lockd_net_id);

	mutex_lock(&nlmsvc_mutex);
	/*
	 * Check whether we're already up and running.
	 */
	if (nlmsvc_rqst) {
		error = lockd_up_net(nlmsvc_rqst->rq_server, net);
		goto out;
=======
static int lockd_inetaddr_event(struct notifier_block *this,
	unsigned long event, void *ptr)
{
	struct in_ifaddr *ifa = (struct in_ifaddr *)ptr;
	struct sockaddr_in sin;

	if (event != NETDEV_DOWN)
		goto out;

	if (nlmsvc_serv) {
		dprintk("lockd_inetaddr_event: removed %pI4\n",
			&ifa->ifa_local);
		sin.sin_family = AF_INET;
		sin.sin_addr.s_addr = ifa->ifa_local;
		svc_age_temp_xprts_now(nlmsvc_serv, (struct sockaddr *)&sin);
	}

out:
	return NOTIFY_DONE;
}

static struct notifier_block lockd_inetaddr_notifier = {
	.notifier_call = lockd_inetaddr_event,
};

#if IS_ENABLED(CONFIG_IPV6)
static int lockd_inet6addr_event(struct notifier_block *this,
	unsigned long event, void *ptr)
{
	struct inet6_ifaddr *ifa = (struct inet6_ifaddr *)ptr;
	struct sockaddr_in6 sin6;

	if (event != NETDEV_DOWN)
		goto out;

	if (nlmsvc_serv) {
		dprintk("lockd_inet6addr_event: removed %pI6\n", &ifa->addr);
		sin6.sin6_family = AF_INET6;
		sin6.sin6_addr = ifa->addr;
		if (ipv6_addr_type(&sin6.sin6_addr) & IPV6_ADDR_LINKLOCAL)
			sin6.sin6_scope_id = ifa->idev->dev->ifindex;
		svc_age_temp_xprts_now(nlmsvc_serv, (struct sockaddr *)&sin6);
	}

out:
	return NOTIFY_DONE;
}

static struct notifier_block lockd_inet6addr_notifier = {
	.notifier_call = lockd_inet6addr_event,
};
#endif

static int lockd_get(void)
{
	struct svc_serv *serv;
	int error;

	if (nlmsvc_serv) {
		nlmsvc_users++;
		return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/*
	 * Sanity check: if there's no pid,
	 * we should be the first user ...
	 */
	if (nlmsvc_users)
		printk(KERN_WARNING
			"lockd_up: no pid, %d users??\n", nlmsvc_users);

<<<<<<< HEAD
	error = -ENOMEM;
	serv = svc_create(&nlmsvc_program, LOCKD_BUFSIZE, NULL);
	if (!serv) {
		printk(KERN_WARNING "lockd_up: create service failed\n");
		goto out;
	}

	error = svc_bind(serv, net);
	if (error < 0) {
		printk(KERN_WARNING "lockd_up: bind service failed\n");
		goto destroy_and_out;
	}

	ln->nlmsvc_users++;

	error = make_socks(serv, net);
	if (error < 0)
		goto err_start;

	/*
	 * Create the kernel thread and wait for it to start.
	 */
	nlmsvc_rqst = svc_prepare_thread(serv, &serv->sv_pools[0], NUMA_NO_NODE);
	if (IS_ERR(nlmsvc_rqst)) {
		error = PTR_ERR(nlmsvc_rqst);
		nlmsvc_rqst = NULL;
		printk(KERN_WARNING
			"lockd_up: svc_rqst allocation failed, error=%d\n",
			error);
		goto err_start;
	}

	svc_sock_update_bufs(serv);
	serv->sv_maxconn = nlm_max_connections;

	nlmsvc_task = kthread_run(lockd, nlmsvc_rqst, serv->sv_name);
	if (IS_ERR(nlmsvc_task)) {
		error = PTR_ERR(nlmsvc_task);
		svc_exit_thread(nlmsvc_rqst);
		nlmsvc_task = NULL;
		nlmsvc_rqst = NULL;
		printk(KERN_WARNING
			"lockd_up: kthread_run failed, error=%d\n", error);
		goto err_start;
	}

	/*
	 * Note: svc_serv structures have an initial use count of 1,
	 * so we exit through here on both success and failure.
	 */
destroy_and_out:
	svc_destroy(serv);
out:
	if (!error)
		nlmsvc_users++;
	mutex_unlock(&nlmsvc_mutex);
	return error;

err_start:
	lockd_down_net(serv, net);
	goto destroy_and_out;
=======
	if (!nlm_timeout)
		nlm_timeout = LOCKD_DFLT_TIMEO;
	nlmsvc_timeout = nlm_timeout * HZ;

	serv = svc_create(&nlmsvc_program, LOCKD_BUFSIZE, lockd);
	if (!serv) {
		printk(KERN_WARNING "lockd_up: create service failed\n");
		return -ENOMEM;
	}

	serv->sv_maxconn = nlm_max_connections;
	error = svc_set_num_threads(serv, NULL, 1);
	if (error < 0) {
		svc_destroy(&serv);
		return error;
	}

	nlmsvc_serv = serv;
	register_inetaddr_notifier(&lockd_inetaddr_notifier);
#if IS_ENABLED(CONFIG_IPV6)
	register_inet6addr_notifier(&lockd_inet6addr_notifier);
#endif
	dprintk("lockd_up: service created\n");
	nlmsvc_users++;
	return 0;
}

static void lockd_put(void)
{
	if (WARN(nlmsvc_users <= 0, "lockd_down: no users!\n"))
		return;
	if (--nlmsvc_users)
		return;

	unregister_inetaddr_notifier(&lockd_inetaddr_notifier);
#if IS_ENABLED(CONFIG_IPV6)
	unregister_inet6addr_notifier(&lockd_inet6addr_notifier);
#endif

	svc_set_num_threads(nlmsvc_serv, NULL, 0);
	timer_delete_sync(&nlmsvc_retry);
	svc_destroy(&nlmsvc_serv);
	dprintk("lockd_down: service destroyed\n");
}

/*
 * Bring up the lockd process if it's not already up.
 */
int lockd_up(struct net *net, const struct cred *cred)
{
	int error;

	mutex_lock(&nlmsvc_mutex);

	error = lockd_get();
	if (error)
		goto err;

	error = lockd_up_net(nlmsvc_serv, net, cred);
	if (error < 0) {
		lockd_put();
		goto err;
	}

err:
	mutex_unlock(&nlmsvc_mutex);
	return error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(lockd_up);

/*
 * Decrement the user count and bring down lockd if we're the last.
 */
void
lockd_down(struct net *net)
{
	mutex_lock(&nlmsvc_mutex);
<<<<<<< HEAD
	lockd_down_net(nlmsvc_rqst->rq_server, net);
	if (nlmsvc_users) {
		if (--nlmsvc_users)
			goto out;
	} else {
		printk(KERN_ERR "lockd_down: no users! task=%p\n",
			nlmsvc_task);
		BUG();
	}

	if (!nlmsvc_task) {
		printk(KERN_ERR "lockd_down: no lockd running.\n");
		BUG();
	}
	kthread_stop(nlmsvc_task);
	svc_exit_thread(nlmsvc_rqst);
	nlmsvc_task = NULL;
	nlmsvc_rqst = NULL;
out:
=======
	lockd_down_net(nlmsvc_serv, net);
	lockd_put();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mutex_unlock(&nlmsvc_mutex);
}
EXPORT_SYMBOL_GPL(lockd_down);

#ifdef CONFIG_SYSCTL

/*
 * Sysctl parameters (same as module parameters, different interface).
 */

<<<<<<< HEAD
static ctl_table nlm_sysctls[] = {
=======
static struct ctl_table nlm_sysctls[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{
		.procname	= "nlm_grace_period",
		.data		= &nlm_grace_period,
		.maxlen		= sizeof(unsigned long),
		.mode		= 0644,
		.proc_handler	= proc_doulongvec_minmax,
		.extra1		= (unsigned long *) &nlm_grace_period_min,
		.extra2		= (unsigned long *) &nlm_grace_period_max,
	},
	{
		.procname	= "nlm_timeout",
		.data		= &nlm_timeout,
		.maxlen		= sizeof(unsigned long),
		.mode		= 0644,
		.proc_handler	= proc_doulongvec_minmax,
		.extra1		= (unsigned long *) &nlm_timeout_min,
		.extra2		= (unsigned long *) &nlm_timeout_max,
	},
	{
		.procname	= "nlm_udpport",
		.data		= &nlm_udpport,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= (int *) &nlm_port_min,
		.extra2		= (int *) &nlm_port_max,
	},
	{
		.procname	= "nlm_tcpport",
		.data		= &nlm_tcpport,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= (int *) &nlm_port_min,
		.extra2		= (int *) &nlm_port_max,
	},
	{
		.procname	= "nsm_use_hostnames",
		.data		= &nsm_use_hostnames,
<<<<<<< HEAD
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
=======
		.maxlen		= sizeof(bool),
		.mode		= 0644,
		.proc_handler	= proc_dobool,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
	{
		.procname	= "nsm_local_state",
		.data		= &nsm_local_state,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
<<<<<<< HEAD
	{ }
};

static ctl_table nlm_sysctl_dir[] = {
	{
		.procname	= "nfs",
		.mode		= 0555,
		.child		= nlm_sysctls,
	},
	{ }
};

static ctl_table nlm_sysctl_root[] = {
	{
		.procname	= "fs",
		.mode		= 0555,
		.child		= nlm_sysctl_dir,
	},
	{ }
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

#endif	/* CONFIG_SYSCTL */

/*
 * Module (and sysfs) parameters.
 */

#define param_set_min_max(name, type, which_strtol, min, max)		\
<<<<<<< HEAD
static int param_set_##name(const char *val, struct kernel_param *kp)	\
=======
static int param_set_##name(const char *val, const struct kernel_param *kp) \
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{									\
	char *endp;							\
	__typeof__(type) num = which_strtol(val, &endp, 0);		\
	if (endp == val || *endp || num < (min) || num > (max))		\
		return -EINVAL;						\
	*((type *) kp->arg) = num;					\
	return 0;							\
}

static inline int is_callback(u32 proc)
{
	return proc == NLMPROC_GRANTED
		|| proc == NLMPROC_GRANTED_MSG
		|| proc == NLMPROC_TEST_RES
		|| proc == NLMPROC_LOCK_RES
		|| proc == NLMPROC_CANCEL_RES
		|| proc == NLMPROC_UNLOCK_RES
		|| proc == NLMPROC_NSM_NOTIFY;
}


<<<<<<< HEAD
static int lockd_authenticate(struct svc_rqst *rqstp)
=======
static enum svc_auth_status lockd_authenticate(struct svc_rqst *rqstp)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	rqstp->rq_client = NULL;
	switch (rqstp->rq_authop->flavour) {
		case RPC_AUTH_NULL:
		case RPC_AUTH_UNIX:
<<<<<<< HEAD
=======
			rqstp->rq_auth_stat = rpc_auth_ok;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (rqstp->rq_proc == 0)
				return SVC_OK;
			if (is_callback(rqstp->rq_proc)) {
				/* Leave it to individual procedures to
				 * call nlmsvc_lookup_host(rqstp)
				 */
				return SVC_OK;
			}
			return svc_set_client(rqstp);
	}
<<<<<<< HEAD
=======
	rqstp->rq_auth_stat = rpc_autherr_badcred;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return SVC_DENIED;
}


param_set_min_max(port, int, simple_strtol, 0, 65535)
param_set_min_max(grace_period, unsigned long, simple_strtoul,
		  nlm_grace_period_min, nlm_grace_period_max)
param_set_min_max(timeout, unsigned long, simple_strtoul,
		  nlm_timeout_min, nlm_timeout_max)

MODULE_AUTHOR("Olaf Kirch <okir@monad.swb.de>");
MODULE_DESCRIPTION("NFS file locking service version " LOCKD_VERSION ".");
MODULE_LICENSE("GPL");

module_param_call(nlm_grace_period, param_set_grace_period, param_get_ulong,
		  &nlm_grace_period, 0644);
module_param_call(nlm_timeout, param_set_timeout, param_get_ulong,
		  &nlm_timeout, 0644);
module_param_call(nlm_udpport, param_set_port, param_get_int,
		  &nlm_udpport, 0644);
module_param_call(nlm_tcpport, param_set_port, param_get_int,
		  &nlm_tcpport, 0644);
module_param(nsm_use_hostnames, bool, 0644);
module_param(nlm_max_connections, uint, 0644);

static int lockd_init_net(struct net *net)
{
<<<<<<< HEAD
=======
	struct lockd_net *ln = net_generic(net, lockd_net_id);

	INIT_DELAYED_WORK(&ln->grace_period_end, grace_ender);
	INIT_LIST_HEAD(&ln->lockd_manager.list);
	ln->lockd_manager.block_opens = false;
	INIT_LIST_HEAD(&ln->nsm_handles);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static void lockd_exit_net(struct net *net)
{
<<<<<<< HEAD
=======
	struct lockd_net *ln = net_generic(net, lockd_net_id);

	WARN_ONCE(!list_empty(&ln->lockd_manager.list),
		  "net %x %s: lockd_manager.list is not empty\n",
		  net->ns.inum, __func__);
	WARN_ONCE(!list_empty(&ln->nsm_handles),
		  "net %x %s: nsm_handles list is not empty\n",
		  net->ns.inum, __func__);
	WARN_ONCE(delayed_work_pending(&ln->grace_period_end),
		  "net %x %s: grace_period_end was not cancelled\n",
		  net->ns.inum, __func__);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct pernet_operations lockd_net_ops = {
	.init = lockd_init_net,
	.exit = lockd_exit_net,
	.id = &lockd_net_id,
	.size = sizeof(struct lockd_net),
};


/*
 * Initialising and terminating the module.
 */

static int __init init_nlm(void)
{
	int err;

#ifdef CONFIG_SYSCTL
	err = -ENOMEM;
<<<<<<< HEAD
	nlm_sysctl_table = register_sysctl_table(nlm_sysctl_root);
=======
	nlm_sysctl_table = register_sysctl("fs/nfs", nlm_sysctls);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (nlm_sysctl_table == NULL)
		goto err_sysctl;
#endif
	err = register_pernet_subsys(&lockd_net_ops);
	if (err)
		goto err_pernet;
<<<<<<< HEAD
	return 0;

err_pernet:
#ifdef CONFIG_SYSCTL
	unregister_sysctl_table(nlm_sysctl_table);
#endif
err_sysctl:
=======

	err = lockd_create_procfs();
	if (err)
		goto err_procfs;

	return 0;

err_procfs:
	unregister_pernet_subsys(&lockd_net_ops);
err_pernet:
#ifdef CONFIG_SYSCTL
	unregister_sysctl_table(nlm_sysctl_table);
err_sysctl:
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

static void __exit exit_nlm(void)
{
	/* FIXME: delete all NLM clients */
	nlm_shutdown_hosts();
<<<<<<< HEAD
=======
	lockd_remove_procfs();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unregister_pernet_subsys(&lockd_net_ops);
#ifdef CONFIG_SYSCTL
	unregister_sysctl_table(nlm_sysctl_table);
#endif
}

module_init(init_nlm);
module_exit(exit_nlm);

<<<<<<< HEAD
/*
 * Define NLM program and procedures
 */
static struct svc_version	nlmsvc_version1 = {
		.vs_vers	= 1,
		.vs_nproc	= 17,
		.vs_proc	= nlmsvc_procedures,
		.vs_xdrsize	= NLMSVC_XDRSIZE,
};
static struct svc_version	nlmsvc_version3 = {
		.vs_vers	= 3,
		.vs_nproc	= 24,
		.vs_proc	= nlmsvc_procedures,
		.vs_xdrsize	= NLMSVC_XDRSIZE,
};
#ifdef CONFIG_LOCKD_V4
static struct svc_version	nlmsvc_version4 = {
		.vs_vers	= 4,
		.vs_nproc	= 24,
		.vs_proc	= nlmsvc_procedures4,
		.vs_xdrsize	= NLMSVC_XDRSIZE,
};
#endif
static struct svc_version *	nlmsvc_version[] = {
=======
/**
 * nlmsvc_dispatch - Process an NLM Request
 * @rqstp: incoming request
 *
 * Return values:
 *  %0: Processing complete; do not send a Reply
 *  %1: Processing complete; send Reply in rqstp->rq_res
 */
static int nlmsvc_dispatch(struct svc_rqst *rqstp)
{
	const struct svc_procedure *procp = rqstp->rq_procinfo;
	__be32 *statp = rqstp->rq_accept_statp;

	if (!procp->pc_decode(rqstp, &rqstp->rq_arg_stream))
		goto out_decode_err;

	*statp = procp->pc_func(rqstp);
	if (*statp == rpc_drop_reply)
		return 0;
	if (*statp != rpc_success)
		return 1;

	if (!procp->pc_encode(rqstp, &rqstp->rq_res_stream))
		goto out_encode_err;

	return 1;

out_decode_err:
	*statp = rpc_garbage_args;
	return 1;

out_encode_err:
	*statp = rpc_system_err;
	return 1;
}

/*
 * Define NLM program and procedures
 */
static DEFINE_PER_CPU_ALIGNED(unsigned long, nlmsvc_version1_count[17]);
static const struct svc_version	nlmsvc_version1 = {
	.vs_vers	= 1,
	.vs_nproc	= 17,
	.vs_proc	= nlmsvc_procedures,
	.vs_count	= nlmsvc_version1_count,
	.vs_dispatch	= nlmsvc_dispatch,
	.vs_xdrsize	= NLMSVC_XDRSIZE,
};

static DEFINE_PER_CPU_ALIGNED(unsigned long,
			      nlmsvc_version3_count[ARRAY_SIZE(nlmsvc_procedures)]);
static const struct svc_version	nlmsvc_version3 = {
	.vs_vers	= 3,
	.vs_nproc	= ARRAY_SIZE(nlmsvc_procedures),
	.vs_proc	= nlmsvc_procedures,
	.vs_count	= nlmsvc_version3_count,
	.vs_dispatch	= nlmsvc_dispatch,
	.vs_xdrsize	= NLMSVC_XDRSIZE,
};

#ifdef CONFIG_LOCKD_V4
static DEFINE_PER_CPU_ALIGNED(unsigned long,
			      nlmsvc_version4_count[ARRAY_SIZE(nlmsvc_procedures4)]);
static const struct svc_version	nlmsvc_version4 = {
	.vs_vers	= 4,
	.vs_nproc	= ARRAY_SIZE(nlmsvc_procedures4),
	.vs_proc	= nlmsvc_procedures4,
	.vs_count	= nlmsvc_version4_count,
	.vs_dispatch	= nlmsvc_dispatch,
	.vs_xdrsize	= NLMSVC_XDRSIZE,
};
#endif

static const struct svc_version *nlmsvc_version[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	[1] = &nlmsvc_version1,
	[3] = &nlmsvc_version3,
#ifdef CONFIG_LOCKD_V4
	[4] = &nlmsvc_version4,
#endif
};

<<<<<<< HEAD
static struct svc_stat		nlmsvc_stats;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define NLM_NRVERS	ARRAY_SIZE(nlmsvc_version)
static struct svc_program	nlmsvc_program = {
	.pg_prog		= NLM_PROGRAM,		/* program number */
	.pg_nvers		= NLM_NRVERS,		/* number of entries in nlmsvc_version */
	.pg_vers		= nlmsvc_version,	/* version table */
	.pg_name		= "lockd",		/* service name */
	.pg_class		= "nfsd",		/* share authentication with nfsd */
<<<<<<< HEAD
	.pg_stats		= &nlmsvc_stats,	/* stats table */
	.pg_authenticate = &lockd_authenticate	/* export authentication */
=======
	.pg_authenticate	= &lockd_authenticate,	/* export authentication */
	.pg_init_request	= svc_generic_init_request,
	.pg_rpcbind_set		= svc_generic_rpcbind_set,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};
