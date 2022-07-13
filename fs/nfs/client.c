<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* client.c: NFS client sharing and management code
 *
 * Copyright (C) 2006 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */


#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/time.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/string.h>
#include <linux/stat.h>
#include <linux/errno.h>
#include <linux/unistd.h>
<<<<<<< HEAD
=======
#include <linux/sunrpc/addr.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/sunrpc/clnt.h>
#include <linux/sunrpc/stats.h>
#include <linux/sunrpc/metrics.h>
#include <linux/sunrpc/xprtsock.h>
#include <linux/sunrpc/xprtrdma.h>
#include <linux/nfs_fs.h>
#include <linux/nfs_mount.h>
#include <linux/nfs4_mount.h>
#include <linux/lockd/bind.h>
#include <linux/seq_file.h>
#include <linux/mount.h>
<<<<<<< HEAD
#include <linux/nfs_idmap.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/vfs.h>
#include <linux/inet.h>
#include <linux/in6.h>
#include <linux/slab.h>
#include <linux/idr.h>
#include <net/ipv6.h>
#include <linux/nfs_xdr.h>
#include <linux/sunrpc/bc_xprt.h>
#include <linux/nsproxy.h>
#include <linux/pid_namespace.h>


#include "nfs4_fs.h"
#include "callback.h"
#include "delegation.h"
#include "iostat.h"
#include "internal.h"
#include "fscache.h"
#include "pnfs.h"
<<<<<<< HEAD
#include "netns.h"
=======
#include "nfs.h"
#include "netns.h"
#include "sysfs.h"
#include "nfs42.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define NFSDBG_FACILITY		NFSDBG_CLIENT

static DECLARE_WAIT_QUEUE_HEAD(nfs_client_active_wq);
<<<<<<< HEAD
#ifdef CONFIG_NFS_V4

/*
 * Get a unique NFSv4.0 callback identifier which will be used
 * by the V4.0 callback service to lookup the nfs_client struct
 */
static int nfs_get_cb_ident_idr(struct nfs_client *clp, int minorversion)
{
	int ret = 0;
	struct nfs_net *nn = net_generic(clp->net, nfs_net_id);

	if (clp->rpc_ops->version != 4 || minorversion != 0)
		return ret;
retry:
	if (!idr_pre_get(&nn->cb_ident_idr, GFP_KERNEL))
		return -ENOMEM;
	spin_lock(&nn->nfs_client_lock);
	ret = idr_get_new(&nn->cb_ident_idr, clp, &clp->cl_cb_ident);
	spin_unlock(&nn->nfs_client_lock);
	if (ret == -EAGAIN)
		goto retry;
	return ret;
}
#endif /* CONFIG_NFS_V4 */

/*
 * Turn off NFSv4 uid/gid mapping when using AUTH_SYS
 */
static bool nfs4_disable_idmapping = true;
=======
static DEFINE_SPINLOCK(nfs_version_lock);
static DEFINE_MUTEX(nfs_version_mutex);
static LIST_HEAD(nfs_versions);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * RPC cruft for NFS
 */
static const struct rpc_version *nfs_version[5] = {
<<<<<<< HEAD
	[2]			= &nfs_version2,
#ifdef CONFIG_NFS_V3
	[3]			= &nfs_version3,
#endif
#ifdef CONFIG_NFS_V4
	[4]			= &nfs_version4,
#endif
=======
	[2] = NULL,
	[3] = NULL,
	[4] = NULL,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

const struct rpc_program nfs_program = {
	.name			= "nfs",
	.number			= NFS_PROGRAM,
	.nrvers			= ARRAY_SIZE(nfs_version),
	.version		= nfs_version,
<<<<<<< HEAD
	.stats			= &nfs_rpcstat,
	.pipe_dir_name		= NFS_PIPE_DIRNAME,
};

struct rpc_stat nfs_rpcstat = {
	.program		= &nfs_program
};


#ifdef CONFIG_NFS_V3_ACL
static struct rpc_stat		nfsacl_rpcstat = { &nfsacl_program };
static const struct rpc_version *nfsacl_version[] = {
	[3]			= &nfsacl_version3,
};

const struct rpc_program nfsacl_program = {
	.name			= "nfsacl",
	.number			= NFS_ACL_PROGRAM,
	.nrvers			= ARRAY_SIZE(nfsacl_version),
	.version		= nfsacl_version,
	.stats			= &nfsacl_rpcstat,
};
#endif  /* CONFIG_NFS_V3_ACL */

struct nfs_client_initdata {
	const char *hostname;
	const struct sockaddr *addr;
	size_t addrlen;
	const struct nfs_rpc_ops *rpc_ops;
	int proto;
	u32 minorversion;
	struct net *net;
};
=======
	.pipe_dir_name		= NFS_PIPE_DIRNAME,
};

static struct nfs_subversion *find_nfs_version(unsigned int version)
{
	struct nfs_subversion *nfs;
	spin_lock(&nfs_version_lock);

	list_for_each_entry(nfs, &nfs_versions, list) {
		if (nfs->rpc_ops->version == version) {
			spin_unlock(&nfs_version_lock);
			return nfs;
		}
	}

	spin_unlock(&nfs_version_lock);
	return ERR_PTR(-EPROTONOSUPPORT);
}

struct nfs_subversion *get_nfs_version(unsigned int version)
{
	struct nfs_subversion *nfs = find_nfs_version(version);

	if (IS_ERR(nfs)) {
		mutex_lock(&nfs_version_mutex);
		request_module("nfsv%d", version);
		nfs = find_nfs_version(version);
		mutex_unlock(&nfs_version_mutex);
	}

	if (!IS_ERR(nfs) && !try_module_get(nfs->owner))
		return ERR_PTR(-EAGAIN);
	return nfs;
}

void put_nfs_version(struct nfs_subversion *nfs)
{
	module_put(nfs->owner);
}

void register_nfs_version(struct nfs_subversion *nfs)
{
	spin_lock(&nfs_version_lock);

	list_add(&nfs->list, &nfs_versions);
	nfs_version[nfs->rpc_ops->version] = nfs->rpc_vers;

	spin_unlock(&nfs_version_lock);
}
EXPORT_SYMBOL_GPL(register_nfs_version);

void unregister_nfs_version(struct nfs_subversion *nfs)
{
	spin_lock(&nfs_version_lock);

	nfs_version[nfs->rpc_ops->version] = NULL;
	list_del(&nfs->list);

	spin_unlock(&nfs_version_lock);
}
EXPORT_SYMBOL_GPL(unregister_nfs_version);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Allocate a shared client record
 *
 * Since these are allocated/deallocated very rarely, we don't
 * bother putting them in a slab cache...
 */
<<<<<<< HEAD
static struct nfs_client *nfs_alloc_client(const struct nfs_client_initdata *cl_init)
{
	struct nfs_client *clp;
	struct rpc_cred *cred;
=======
struct nfs_client *nfs_alloc_client(const struct nfs_client_initdata *cl_init)
{
	struct nfs_client *clp;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int err = -ENOMEM;

	if ((clp = kzalloc(sizeof(*clp), GFP_KERNEL)) == NULL)
		goto error_0;

<<<<<<< HEAD
	clp->rpc_ops = cl_init->rpc_ops;

	atomic_set(&clp->cl_count, 1);
=======
	clp->cl_minorversion = cl_init->minorversion;
	clp->cl_nfs_mod = cl_init->nfs_mod;
	if (!try_module_get(clp->cl_nfs_mod->owner))
		goto error_dealloc;

	clp->rpc_ops = clp->cl_nfs_mod->rpc_ops;

	refcount_set(&clp->cl_count, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	clp->cl_cons_state = NFS_CS_INITING;

	memcpy(&clp->cl_addr, cl_init->addr, cl_init->addrlen);
	clp->cl_addrlen = cl_init->addrlen;

	if (cl_init->hostname) {
		err = -ENOMEM;
		clp->cl_hostname = kstrdup(cl_init->hostname, GFP_KERNEL);
		if (!clp->cl_hostname)
			goto error_cleanup;
	}

	INIT_LIST_HEAD(&clp->cl_superblocks);
	clp->cl_rpcclient = ERR_PTR(-EINVAL);

<<<<<<< HEAD
	clp->cl_proto = cl_init->proto;
	clp->net = get_net(cl_init->net);

#ifdef CONFIG_NFS_V4
	err = nfs_get_cb_ident_idr(clp, cl_init->minorversion);
	if (err)
		goto error_cleanup;

	spin_lock_init(&clp->cl_lock);
	INIT_DELAYED_WORK(&clp->cl_renewd, nfs4_renew_state);
	rpc_init_wait_queue(&clp->cl_rpcwaitq, "NFS client");
	clp->cl_boot_time = CURRENT_TIME;
	clp->cl_state = 1 << NFS4CLNT_LEASE_EXPIRED;
	clp->cl_minorversion = cl_init->minorversion;
	clp->cl_mvops = nfs_v4_minor_ops[cl_init->minorversion];
#endif
	cred = rpc_lookup_machine_cred("*");
	if (!IS_ERR(cred))
		clp->cl_machine_cred = cred;
	nfs_fscache_get_client_cookie(clp);

	return clp;

error_cleanup:
=======
	clp->cl_flags = cl_init->init_flags;
	clp->cl_proto = cl_init->proto;
	clp->cl_nconnect = cl_init->nconnect;
	clp->cl_max_connect = cl_init->max_connect ? cl_init->max_connect : 1;
	clp->cl_net = get_net(cl_init->net);

	clp->cl_principal = "*";
	clp->cl_xprtsec = cl_init->xprtsec;
	return clp;

error_cleanup:
	put_nfs_version(clp->cl_nfs_mod);
error_dealloc:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kfree(clp);
error_0:
	return ERR_PTR(err);
}
<<<<<<< HEAD

#ifdef CONFIG_NFS_V4
#ifdef CONFIG_NFS_V4_1
static void nfs4_shutdown_session(struct nfs_client *clp)
{
	if (nfs4_has_session(clp)) {
		nfs4_deviceid_purge_client(clp);
		nfs4_destroy_session(clp->cl_session);
	}

}
#else /* CONFIG_NFS_V4_1 */
static void nfs4_shutdown_session(struct nfs_client *clp)
{
}
#endif /* CONFIG_NFS_V4_1 */

/*
 * Destroy the NFS4 callback service
 */
static void nfs4_destroy_callback(struct nfs_client *clp)
{
	if (__test_and_clear_bit(NFS_CS_CALLBACK, &clp->cl_res_state))
		nfs_callback_down(clp->cl_mvops->minor_version);
}

static void nfs4_shutdown_client(struct nfs_client *clp)
{
	if (__test_and_clear_bit(NFS_CS_RENEWD, &clp->cl_res_state))
		nfs4_kill_renewd(clp);
	nfs4_shutdown_session(clp);
	nfs4_destroy_callback(clp);
	if (__test_and_clear_bit(NFS_CS_IDMAP, &clp->cl_res_state))
		nfs_idmap_delete(clp);

	rpc_destroy_wait_queue(&clp->cl_rpcwaitq);
}

/* idr_remove_all is not needed as all id's are removed by nfs_put_client */
void nfs_cleanup_cb_ident_idr(struct net *net)
=======
EXPORT_SYMBOL_GPL(nfs_alloc_client);

#if IS_ENABLED(CONFIG_NFS_V4)
static void nfs_cleanup_cb_ident_idr(struct net *net)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct nfs_net *nn = net_generic(net, nfs_net_id);

	idr_destroy(&nn->cb_ident_idr);
}

/* nfs_client_lock held */
static void nfs_cb_idr_remove_locked(struct nfs_client *clp)
{
<<<<<<< HEAD
	struct nfs_net *nn = net_generic(clp->net, nfs_net_id);
=======
	struct nfs_net *nn = net_generic(clp->cl_net, nfs_net_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (clp->cl_cb_ident)
		idr_remove(&nn->cb_ident_idr, clp->cl_cb_ident);
}

static void pnfs_init_server(struct nfs_server *server)
{
	rpc_init_wait_queue(&server->roc_rpcwaitq, "pNFS ROC");
}

<<<<<<< HEAD
static void nfs4_destroy_server(struct nfs_server *server)
{
	nfs4_purge_state_owners(server);
}

#else
static void nfs4_shutdown_client(struct nfs_client *clp)
{
}

void nfs_cleanup_cb_ident_idr(struct net *net)
=======
#else
static void nfs_cleanup_cb_ident_idr(struct net *net)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
}

static void nfs_cb_idr_remove_locked(struct nfs_client *clp)
{
}

static void pnfs_init_server(struct nfs_server *server)
{
}

#endif /* CONFIG_NFS_V4 */

/*
 * Destroy a shared client record
 */
<<<<<<< HEAD
static void nfs_free_client(struct nfs_client *clp)
{
	dprintk("--> nfs_free_client(%u)\n", clp->rpc_ops->version);

	nfs4_shutdown_client(clp);

	nfs_fscache_release_client_cookie(clp);

=======
void nfs_free_client(struct nfs_client *clp)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* -EIO all pending I/O */
	if (!IS_ERR(clp->cl_rpcclient))
		rpc_shutdown_client(clp->cl_rpcclient);

<<<<<<< HEAD
	if (clp->cl_machine_cred != NULL)
		put_rpccred(clp->cl_machine_cred);

	put_net(clp->net);
	kfree(clp->cl_hostname);
	kfree(clp->server_scope);
	kfree(clp->impl_id);
	kfree(clp);

	dprintk("<-- nfs_free_client()\n");
}
=======
	put_net(clp->cl_net);
	put_nfs_version(clp->cl_nfs_mod);
	kfree(clp->cl_hostname);
	kfree(clp->cl_acceptor);
	kfree_rcu(clp, rcu);
}
EXPORT_SYMBOL_GPL(nfs_free_client);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Release a reference to a shared client record
 */
void nfs_put_client(struct nfs_client *clp)
{
	struct nfs_net *nn;

	if (!clp)
		return;

<<<<<<< HEAD
	dprintk("--> nfs_put_client({%d})\n", atomic_read(&clp->cl_count));
	nn = net_generic(clp->net, nfs_net_id);

	if (atomic_dec_and_lock(&clp->cl_count, &nn->nfs_client_lock)) {
=======
	nn = net_generic(clp->cl_net, nfs_net_id);

	if (refcount_dec_and_lock(&clp->cl_count, &nn->nfs_client_lock)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		list_del(&clp->cl_share_link);
		nfs_cb_idr_remove_locked(clp);
		spin_unlock(&nn->nfs_client_lock);

<<<<<<< HEAD
		BUG_ON(!list_empty(&clp->cl_superblocks));

		nfs_free_client(clp);
=======
		WARN_ON_ONCE(!list_empty(&clp->cl_superblocks));

		clp->rpc_ops->free_client(clp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}
EXPORT_SYMBOL_GPL(nfs_put_client);

<<<<<<< HEAD
#if defined(CONFIG_IPV6) || defined(CONFIG_IPV6_MODULE)
/*
 * Test if two ip6 socket addresses refer to the same socket by
 * comparing relevant fields. The padding bytes specifically, are not
 * compared. sin6_flowinfo is not compared because it only affects QoS
 * and sin6_scope_id is only compared if the address is "link local"
 * because "link local" addresses need only be unique to a specific
 * link. Conversely, ordinary unicast addresses might have different
 * sin6_scope_id.
 *
 * The caller should ensure both socket addresses are AF_INET6.
 */
static int nfs_sockaddr_match_ipaddr6(const struct sockaddr *sa1,
				      const struct sockaddr *sa2)
{
	const struct sockaddr_in6 *sin1 = (const struct sockaddr_in6 *)sa1;
	const struct sockaddr_in6 *sin2 = (const struct sockaddr_in6 *)sa2;

	if (!ipv6_addr_equal(&sin1->sin6_addr, &sin2->sin6_addr))
		return 0;
	else if (ipv6_addr_type(&sin1->sin6_addr) & IPV6_ADDR_LINKLOCAL)
		return sin1->sin6_scope_id == sin2->sin6_scope_id;

	return 1;
}
#else	/* !defined(CONFIG_IPV6) && !defined(CONFIG_IPV6_MODULE) */
static int nfs_sockaddr_match_ipaddr6(const struct sockaddr *sa1,
				      const struct sockaddr *sa2)
{
	return 0;
}
#endif

/*
 * Test if two ip4 socket addresses refer to the same socket, by
 * comparing relevant fields. The padding bytes specifically, are
 * not compared.
 *
 * The caller should ensure both socket addresses are AF_INET.
 */
static int nfs_sockaddr_match_ipaddr4(const struct sockaddr *sa1,
				      const struct sockaddr *sa2)
{
	const struct sockaddr_in *sin1 = (const struct sockaddr_in *)sa1;
	const struct sockaddr_in *sin2 = (const struct sockaddr_in *)sa2;

	return sin1->sin_addr.s_addr == sin2->sin_addr.s_addr;
}

static int nfs_sockaddr_cmp_ip6(const struct sockaddr *sa1,
				const struct sockaddr *sa2)
{
	const struct sockaddr_in6 *sin1 = (const struct sockaddr_in6 *)sa1;
	const struct sockaddr_in6 *sin2 = (const struct sockaddr_in6 *)sa2;

	return nfs_sockaddr_match_ipaddr6(sa1, sa2) &&
		(sin1->sin6_port == sin2->sin6_port);
}

static int nfs_sockaddr_cmp_ip4(const struct sockaddr *sa1,
				const struct sockaddr *sa2)
{
	const struct sockaddr_in *sin1 = (const struct sockaddr_in *)sa1;
	const struct sockaddr_in *sin2 = (const struct sockaddr_in *)sa2;

	return nfs_sockaddr_match_ipaddr4(sa1, sa2) &&
		(sin1->sin_port == sin2->sin_port);
}

#if defined(CONFIG_NFS_V4_1)
/*
 * Test if two socket addresses represent the same actual socket,
 * by comparing (only) relevant fields, excluding the port number.
 */
static int nfs_sockaddr_match_ipaddr(const struct sockaddr *sa1,
				     const struct sockaddr *sa2)
{
	if (sa1->sa_family != sa2->sa_family)
		return 0;

	switch (sa1->sa_family) {
	case AF_INET:
		return nfs_sockaddr_match_ipaddr4(sa1, sa2);
	case AF_INET6:
		return nfs_sockaddr_match_ipaddr6(sa1, sa2);
	}
	return 0;
}
#endif /* CONFIG_NFS_V4_1 */

/*
 * Test if two socket addresses represent the same actual socket,
 * by comparing (only) relevant fields, including the port number.
 */
static int nfs_sockaddr_cmp(const struct sockaddr *sa1,
			    const struct sockaddr *sa2)
{
	if (sa1->sa_family != sa2->sa_family)
		return 0;

	switch (sa1->sa_family) {
	case AF_INET:
		return nfs_sockaddr_cmp_ip4(sa1, sa2);
	case AF_INET6:
		return nfs_sockaddr_cmp_ip6(sa1, sa2);
	}
	return 0;
}

#if defined(CONFIG_NFS_V4_1)
/* Common match routine for v4.0 and v4.1 callback services */
static bool nfs4_cb_match_client(const struct sockaddr *addr,
		struct nfs_client *clp, u32 minorversion)
{
	struct sockaddr *clap = (struct sockaddr *)&clp->cl_addr;

	/* Don't match clients that failed to initialise */
	if (!(clp->cl_cons_state == NFS_CS_READY ||
	    clp->cl_cons_state == NFS_CS_SESSION_INITING))
		return false;

	/* Match the version and minorversion */
	if (clp->rpc_ops->version != 4 ||
	    clp->cl_minorversion != minorversion)
		return false;

	/* Match only the IP address, not the port number */
	if (!nfs_sockaddr_match_ipaddr(addr, clap))
		return false;

	return true;
}
#endif /* CONFIG_NFS_V4_1 */

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Find an nfs_client on the list that matches the initialisation data
 * that is supplied.
 */
static struct nfs_client *nfs_match_client(const struct nfs_client_initdata *data)
{
	struct nfs_client *clp;
<<<<<<< HEAD
	const struct sockaddr *sap = data->addr;
	struct nfs_net *nn = net_generic(data->net, nfs_net_id);

=======
	const struct sockaddr *sap = (struct sockaddr *)data->addr;
	struct nfs_net *nn = net_generic(data->net, nfs_net_id);
	int error;

again:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	list_for_each_entry(clp, &nn->nfs_client_list, cl_share_link) {
	        const struct sockaddr *clap = (struct sockaddr *)&clp->cl_addr;
		/* Don't match clients that failed to initialise properly */
		if (clp->cl_cons_state < 0)
			continue;

<<<<<<< HEAD
		/* Different NFS versions cannot share the same nfs_client */
		if (clp->rpc_ops != data->rpc_ops)
=======
		/* If a client is still initializing then we need to wait */
		if (clp->cl_cons_state > NFS_CS_READY) {
			refcount_inc(&clp->cl_count);
			spin_unlock(&nn->nfs_client_lock);
			error = nfs_wait_client_init_complete(clp);
			nfs_put_client(clp);
			spin_lock(&nn->nfs_client_lock);
			if (error < 0)
				return ERR_PTR(error);
			goto again;
		}

		/* Different NFS versions cannot share the same nfs_client */
		if (clp->rpc_ops != data->nfs_mod->rpc_ops)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			continue;

		if (clp->cl_proto != data->proto)
			continue;
		/* Match nfsv4 minorversion */
		if (clp->cl_minorversion != data->minorversion)
			continue;
<<<<<<< HEAD
		/* Match the full socket address */
		if (!nfs_sockaddr_cmp(sap, clap))
			continue;

		atomic_inc(&clp->cl_count);
=======

		/* Match request for a dedicated DS */
		if (test_bit(NFS_CS_DS, &data->init_flags) !=
		    test_bit(NFS_CS_DS, &clp->cl_flags))
			continue;

		/* Match the full socket address */
		if (!rpc_cmp_addr_port(sap, clap))
			/* Match all xprt_switch full socket addresses */
			if (IS_ERR(clp->cl_rpcclient) ||
                            !rpc_clnt_xprt_switch_has_addr(clp->cl_rpcclient,
							   sap))
				continue;

		/* Match the xprt security policy */
		if (clp->cl_xprtsec.policy != data->xprtsec.policy)
			continue;

		refcount_inc(&clp->cl_count);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return clp;
	}
	return NULL;
}

/*
<<<<<<< HEAD
 * Look up a client by IP address and protocol version
 * - creates a new record if one doesn't yet exist
 */
static struct nfs_client *
nfs_get_client(const struct nfs_client_initdata *cl_init,
	       const struct rpc_timeout *timeparms,
	       const char *ip_addr,
	       rpc_authflavor_t authflavour,
	       int noresvport)
{
	struct nfs_client *clp, *new = NULL;
	int error;
	struct nfs_net *nn = net_generic(cl_init->net, nfs_net_id);

	dprintk("--> nfs_get_client(%s,v%u)\n",
		cl_init->hostname ?: "", cl_init->rpc_ops->version);

	/* see if the client already exists */
	do {
		spin_lock(&nn->nfs_client_lock);

		clp = nfs_match_client(cl_init);
		if (clp)
			goto found_client;
		if (new)
			goto install_client;

		spin_unlock(&nn->nfs_client_lock);

		new = nfs_alloc_client(cl_init);
	} while (!IS_ERR(new));

	dprintk("--> nfs_get_client() = %ld [failed]\n", PTR_ERR(new));
	return new;

	/* install a new client and return with it unready */
install_client:
	clp = new;
	list_add(&clp->cl_share_link, &nn->nfs_client_list);
	spin_unlock(&nn->nfs_client_lock);

	error = cl_init->rpc_ops->init_client(clp, timeparms, ip_addr,
					      authflavour, noresvport);
	if (error < 0) {
		nfs_put_client(clp);
		return ERR_PTR(error);
	}
	dprintk("--> nfs_get_client() = %p [new]\n", clp);
	return clp;

	/* found an existing client
	 * - make sure it's ready before returning
	 */
found_client:
	spin_unlock(&nn->nfs_client_lock);

	if (new)
		nfs_free_client(new);

	error = wait_event_killable(nfs_client_active_wq,
				clp->cl_cons_state < NFS_CS_INITING);
=======
 * Return true if @clp is done initializing, false if still working on it.
 *
 * Use nfs_client_init_status to check if it was successful.
 */
bool nfs_client_init_is_complete(const struct nfs_client *clp)
{
	return clp->cl_cons_state <= NFS_CS_READY;
}
EXPORT_SYMBOL_GPL(nfs_client_init_is_complete);

/*
 * Return 0 if @clp was successfully initialized, -errno otherwise.
 *
 * This must be called *after* nfs_client_init_is_complete() returns true,
 * otherwise it will pop WARN_ON_ONCE and return -EINVAL
 */
int nfs_client_init_status(const struct nfs_client *clp)
{
	/* called without checking nfs_client_init_is_complete */
	if (clp->cl_cons_state > NFS_CS_READY) {
		WARN_ON_ONCE(1);
		return -EINVAL;
	}
	return clp->cl_cons_state;
}
EXPORT_SYMBOL_GPL(nfs_client_init_status);

int nfs_wait_client_init_complete(const struct nfs_client *clp)
{
	return wait_event_killable(nfs_client_active_wq,
			nfs_client_init_is_complete(clp));
}
EXPORT_SYMBOL_GPL(nfs_wait_client_init_complete);

/*
 * Found an existing client.  Make sure it's ready before returning.
 */
static struct nfs_client *
nfs_found_client(const struct nfs_client_initdata *cl_init,
		 struct nfs_client *clp)
{
	int error;

	error = nfs_wait_client_init_complete(clp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (error < 0) {
		nfs_put_client(clp);
		return ERR_PTR(-ERESTARTSYS);
	}

	if (clp->cl_cons_state < NFS_CS_READY) {
		error = clp->cl_cons_state;
		nfs_put_client(clp);
		return ERR_PTR(error);
	}

<<<<<<< HEAD
	BUG_ON(clp->cl_cons_state != NFS_CS_READY);

	dprintk("--> nfs_get_client() = %p [share]\n", clp);
=======
	smp_rmb();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return clp;
}

/*
<<<<<<< HEAD
=======
 * Look up a client by IP address and protocol version
 * - creates a new record if one doesn't yet exist
 */
struct nfs_client *nfs_get_client(const struct nfs_client_initdata *cl_init)
{
	struct nfs_client *clp, *new = NULL;
	struct nfs_net *nn = net_generic(cl_init->net, nfs_net_id);
	const struct nfs_rpc_ops *rpc_ops = cl_init->nfs_mod->rpc_ops;

	if (cl_init->hostname == NULL) {
		WARN_ON(1);
		return ERR_PTR(-EINVAL);
	}

	/* see if the client already exists */
	do {
		spin_lock(&nn->nfs_client_lock);

		clp = nfs_match_client(cl_init);
		if (clp) {
			spin_unlock(&nn->nfs_client_lock);
			if (new)
				new->rpc_ops->free_client(new);
			if (IS_ERR(clp))
				return clp;
			return nfs_found_client(cl_init, clp);
		}
		if (new) {
			list_add_tail(&new->cl_share_link,
					&nn->nfs_client_list);
			spin_unlock(&nn->nfs_client_lock);
			return rpc_ops->init_client(new, cl_init);
		}

		spin_unlock(&nn->nfs_client_lock);

		new = rpc_ops->alloc_client(cl_init);
	} while (!IS_ERR(new));

	return new;
}
EXPORT_SYMBOL_GPL(nfs_get_client);

/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Mark a server as ready or failed
 */
void nfs_mark_client_ready(struct nfs_client *clp, int state)
{
<<<<<<< HEAD
	clp->cl_cons_state = state;
	wake_up_all(&nfs_client_active_wq);
}

/*
 * With sessions, the client is not marked ready until after a
 * successful EXCHANGE_ID and CREATE_SESSION.
 *
 * Map errors cl_cons_state errors to EPROTONOSUPPORT to indicate
 * other versions of NFS can be tried.
 */
int nfs4_check_client_ready(struct nfs_client *clp)
{
	if (!nfs4_has_session(clp))
		return 0;
	if (clp->cl_cons_state < NFS_CS_READY)
		return -EPROTONOSUPPORT;
	return 0;
}
=======
	smp_wmb();
	clp->cl_cons_state = state;
	wake_up_all(&nfs_client_active_wq);
}
EXPORT_SYMBOL_GPL(nfs_mark_client_ready);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Initialise the timeout values for a connection
 */
<<<<<<< HEAD
static void nfs_init_timeout_values(struct rpc_timeout *to, int proto,
				    unsigned int timeo, unsigned int retrans)
=======
void nfs_init_timeout_values(struct rpc_timeout *to, int proto,
				    int timeo, int retrans)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	to->to_initval = timeo * HZ / 10;
	to->to_retries = retrans;

	switch (proto) {
	case XPRT_TRANSPORT_TCP:
<<<<<<< HEAD
	case XPRT_TRANSPORT_RDMA:
		if (to->to_retries == 0)
			to->to_retries = NFS_DEF_TCP_RETRANS;
		if (to->to_initval == 0)
=======
	case XPRT_TRANSPORT_TCP_TLS:
	case XPRT_TRANSPORT_RDMA:
		if (retrans == NFS_UNSPEC_RETRANS)
			to->to_retries = NFS_DEF_TCP_RETRANS;
		if (timeo == NFS_UNSPEC_TIMEO || to->to_initval == 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			to->to_initval = NFS_DEF_TCP_TIMEO * HZ / 10;
		if (to->to_initval > NFS_MAX_TCP_TIMEOUT)
			to->to_initval = NFS_MAX_TCP_TIMEOUT;
		to->to_increment = to->to_initval;
		to->to_maxval = to->to_initval + (to->to_increment * to->to_retries);
		if (to->to_maxval > NFS_MAX_TCP_TIMEOUT)
			to->to_maxval = NFS_MAX_TCP_TIMEOUT;
		if (to->to_maxval < to->to_initval)
			to->to_maxval = to->to_initval;
		to->to_exponential = 0;
		break;
	case XPRT_TRANSPORT_UDP:
<<<<<<< HEAD
		if (to->to_retries == 0)
			to->to_retries = NFS_DEF_UDP_RETRANS;
		if (!to->to_initval)
=======
		if (retrans == NFS_UNSPEC_RETRANS)
			to->to_retries = NFS_DEF_UDP_RETRANS;
		if (timeo == NFS_UNSPEC_TIMEO || to->to_initval == 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			to->to_initval = NFS_DEF_UDP_TIMEO * HZ / 10;
		if (to->to_initval > NFS_MAX_UDP_TIMEOUT)
			to->to_initval = NFS_MAX_UDP_TIMEOUT;
		to->to_maxval = NFS_MAX_UDP_TIMEOUT;
		to->to_exponential = 1;
		break;
	default:
		BUG();
	}
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(nfs_init_timeout_values);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Create an RPC client handle
 */
<<<<<<< HEAD
static int nfs_create_rpc_client(struct nfs_client *clp,
				 const struct rpc_timeout *timeparms,
				 rpc_authflavor_t flavor,
				 int discrtry, int noresvport)
{
	struct rpc_clnt		*clnt = NULL;
	struct rpc_create_args args = {
		.net		= clp->net,
		.protocol	= clp->cl_proto,
		.address	= (struct sockaddr *)&clp->cl_addr,
		.addrsize	= clp->cl_addrlen,
		.timeout	= timeparms,
		.servername	= clp->cl_hostname,
		.program	= &nfs_program,
		.version	= clp->rpc_ops->version,
		.authflavor	= flavor,
	};

	if (discrtry)
		args.flags |= RPC_CLNT_CREATE_DISCRTRY;
	if (noresvport)
		args.flags |= RPC_CLNT_CREATE_NONPRIVPORT;
=======
int nfs_create_rpc_client(struct nfs_client *clp,
			  const struct nfs_client_initdata *cl_init,
			  rpc_authflavor_t flavor)
{
	struct nfs_net		*nn = net_generic(clp->cl_net, nfs_net_id);
	struct rpc_clnt		*clnt = NULL;
	struct rpc_create_args args = {
		.net		= clp->cl_net,
		.protocol	= clp->cl_proto,
		.nconnect	= clp->cl_nconnect,
		.address	= (struct sockaddr *)&clp->cl_addr,
		.addrsize	= clp->cl_addrlen,
		.timeout	= cl_init->timeparms,
		.servername	= clp->cl_hostname,
		.nodename	= cl_init->nodename,
		.program	= &nfs_program,
		.stats		= &nn->rpcstats,
		.version	= clp->rpc_ops->version,
		.authflavor	= flavor,
		.cred		= cl_init->cred,
		.xprtsec	= cl_init->xprtsec,
		.connect_timeout = cl_init->connect_timeout,
		.reconnect_timeout = cl_init->reconnect_timeout,
	};

	if (test_bit(NFS_CS_DISCRTRY, &clp->cl_flags))
		args.flags |= RPC_CLNT_CREATE_DISCRTRY;
	if (test_bit(NFS_CS_NO_RETRANS_TIMEOUT, &clp->cl_flags))
		args.flags |= RPC_CLNT_CREATE_NO_RETRANS_TIMEOUT;
	if (test_bit(NFS_CS_NORESVPORT, &clp->cl_flags))
		args.flags |= RPC_CLNT_CREATE_NONPRIVPORT;
	if (test_bit(NFS_CS_INFINITE_SLOTS, &clp->cl_flags))
		args.flags |= RPC_CLNT_CREATE_INFINITE_SLOTS;
	if (test_bit(NFS_CS_NOPING, &clp->cl_flags))
		args.flags |= RPC_CLNT_CREATE_NOPING;
	if (test_bit(NFS_CS_REUSEPORT, &clp->cl_flags))
		args.flags |= RPC_CLNT_CREATE_REUSEPORT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!IS_ERR(clp->cl_rpcclient))
		return 0;

	clnt = rpc_create(&args);
	if (IS_ERR(clnt)) {
		dprintk("%s: cannot create RPC client. Error = %ld\n",
				__func__, PTR_ERR(clnt));
		return PTR_ERR(clnt);
	}

<<<<<<< HEAD
	clp->cl_rpcclient = clnt;
	return 0;
}
=======
	clnt->cl_principal = clp->cl_principal;
	clp->cl_rpcclient = clnt;
	clnt->cl_max_connect = clp->cl_max_connect;
	return 0;
}
EXPORT_SYMBOL_GPL(nfs_create_rpc_client);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Version 2 or 3 client destruction
 */
static void nfs_destroy_server(struct nfs_server *server)
{
	if (server->nlm_host)
		nlmclnt_done(server->nlm_host);
}

/*
 * Version 2 or 3 lockd setup
 */
static int nfs_start_lockd(struct nfs_server *server)
{
	struct nlm_host *host;
	struct nfs_client *clp = server->nfs_client;
	struct nlmclnt_initdata nlm_init = {
		.hostname	= clp->cl_hostname,
		.address	= (struct sockaddr *)&clp->cl_addr,
		.addrlen	= clp->cl_addrlen,
		.nfs_version	= clp->rpc_ops->version,
		.noresvport	= server->flags & NFS_MOUNT_NORESVPORT ?
					1 : 0,
<<<<<<< HEAD
		.net		= clp->net,
=======
		.net		= clp->cl_net,
		.nlmclnt_ops 	= clp->cl_nfs_mod->rpc_ops->nlmclnt_ops,
		.cred		= server->cred,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	};

	if (nlm_init.nfs_version > 3)
		return 0;
	if ((server->flags & NFS_MOUNT_LOCAL_FLOCK) &&
			(server->flags & NFS_MOUNT_LOCAL_FCNTL))
		return 0;

	switch (clp->cl_proto) {
		default:
			nlm_init.protocol = IPPROTO_TCP;
			break;
<<<<<<< HEAD
		case XPRT_TRANSPORT_UDP:
			nlm_init.protocol = IPPROTO_UDP;
=======
#ifndef CONFIG_NFS_DISABLE_UDP_SUPPORT
		case XPRT_TRANSPORT_UDP:
			nlm_init.protocol = IPPROTO_UDP;
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	host = nlmclnt_init(&nlm_init);
	if (IS_ERR(host))
		return PTR_ERR(host);

	server->nlm_host = host;
	server->destroy = nfs_destroy_server;
<<<<<<< HEAD
=======
	nfs_sysfs_link_rpc_client(server, nlmclnt_rpc_clnt(host), NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

/*
<<<<<<< HEAD
 * Initialise an NFSv3 ACL client connection
 */
#ifdef CONFIG_NFS_V3_ACL
static void nfs_init_server_aclclient(struct nfs_server *server)
{
	if (server->nfs_client->rpc_ops->version != 3)
		goto out_noacl;
	if (server->flags & NFS_MOUNT_NOACL)
		goto out_noacl;

	server->client_acl = rpc_bind_new_program(server->client, &nfsacl_program, 3);
	if (IS_ERR(server->client_acl))
		goto out_noacl;

	/* No errors! Assume that Sun nfsacls are supported */
	server->caps |= NFS_CAP_ACLS;
	return;

out_noacl:
	server->caps &= ~NFS_CAP_ACLS;
}
#else
static inline void nfs_init_server_aclclient(struct nfs_server *server)
{
	server->flags &= ~NFS_MOUNT_NOACL;
	server->caps &= ~NFS_CAP_ACLS;
}
#endif

/*
 * Create a general RPC client
 */
static int nfs_init_server_rpcclient(struct nfs_server *server,
=======
 * Create a general RPC client
 */
int nfs_init_server_rpcclient(struct nfs_server *server,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		const struct rpc_timeout *timeo,
		rpc_authflavor_t pseudoflavour)
{
	struct nfs_client *clp = server->nfs_client;

<<<<<<< HEAD
	server->client = rpc_clone_client(clp->cl_rpcclient);
=======
	server->client = rpc_clone_client_set_auth(clp->cl_rpcclient,
							pseudoflavour);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (IS_ERR(server->client)) {
		dprintk("%s: couldn't create rpc_client!\n", __func__);
		return PTR_ERR(server->client);
	}

	memcpy(&server->client->cl_timeout_default,
			timeo,
			sizeof(server->client->cl_timeout_default));
	server->client->cl_timeout = &server->client->cl_timeout_default;
<<<<<<< HEAD

	if (pseudoflavour != clp->cl_rpcclient->cl_auth->au_flavor) {
		struct rpc_auth *auth;

		auth = rpcauth_create(pseudoflavour, server->client);
		if (IS_ERR(auth)) {
			dprintk("%s: couldn't create credcache!\n", __func__);
			return PTR_ERR(auth);
		}
	}
	server->client->cl_softrtry = 0;
	if (server->flags & NFS_MOUNT_SOFT)
		server->client->cl_softrtry = 1;

	return 0;
}

/*
 * Initialise an NFS2 or NFS3 client
 */
int nfs_init_client(struct nfs_client *clp, const struct rpc_timeout *timeparms,
		    const char *ip_addr, rpc_authflavor_t authflavour,
		    int noresvport)
{
	int error;

	if (clp->cl_cons_state == NFS_CS_READY) {
		/* the client is already initialised */
		dprintk("<-- nfs_init_client() = 0 [already %p]\n", clp);
		return 0;
	}
=======
	server->client->cl_softrtry = 0;
	if (server->flags & NFS_MOUNT_SOFTERR)
		server->client->cl_softerr = 1;
	if (server->flags & NFS_MOUNT_SOFT)
		server->client->cl_softrtry = 1;

	nfs_sysfs_link_rpc_client(server, server->client, NULL);
	return 0;
}
EXPORT_SYMBOL_GPL(nfs_init_server_rpcclient);

/**
 * nfs_init_client - Initialise an NFS2 or NFS3 client
 *
 * @clp: nfs_client to initialise
 * @cl_init: Initialisation parameters
 *
 * Returns pointer to an NFS client, or an ERR_PTR value.
 */
struct nfs_client *nfs_init_client(struct nfs_client *clp,
				   const struct nfs_client_initdata *cl_init)
{
	int error;

	/* the client is already initialised */
	if (clp->cl_cons_state == NFS_CS_READY)
		return clp;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Create a client RPC handle for doing FSSTAT with UNIX auth only
	 * - RFC 2623, sec 2.3.2
	 */
<<<<<<< HEAD
	error = nfs_create_rpc_client(clp, timeparms, RPC_AUTH_UNIX,
				      0, noresvport);
	if (error < 0)
		goto error;
	nfs_mark_client_ready(clp, NFS_CS_READY);
	return 0;

error:
	nfs_mark_client_ready(clp, error);
	dprintk("<-- nfs_init_client() = xerror %d\n", error);
	return error;
}
=======
	error = nfs_create_rpc_client(clp, cl_init, RPC_AUTH_UNIX);
	nfs_mark_client_ready(clp, error == 0 ? NFS_CS_READY : error);
	if (error < 0) {
		nfs_put_client(clp);
		clp = ERR_PTR(error);
	}
	return clp;
}
EXPORT_SYMBOL_GPL(nfs_init_client);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Create a version 2 or 3 client
 */
static int nfs_init_server(struct nfs_server *server,
<<<<<<< HEAD
			   const struct nfs_parsed_mount_data *data)
{
	struct nfs_client_initdata cl_init = {
		.hostname = data->nfs_server.hostname,
		.addr = (const struct sockaddr *)&data->nfs_server.address,
		.addrlen = data->nfs_server.addrlen,
		.rpc_ops = &nfs_v2_clientops,
		.proto = data->nfs_server.protocol,
		.net = data->net,
	};
	struct rpc_timeout timeparms;
	struct nfs_client *clp;
	int error;

	dprintk("--> nfs_init_server()\n");

#ifdef CONFIG_NFS_V3
	if (data->version == 3)
		cl_init.rpc_ops = &nfs_v3_clientops;
#endif

	nfs_init_timeout_values(&timeparms, data->nfs_server.protocol,
			data->timeo, data->retrans);

	/* Allocate or find a client reference we can use */
	clp = nfs_get_client(&cl_init, &timeparms, NULL, RPC_AUTH_UNIX,
			     data->flags & NFS_MOUNT_NORESVPORT);
	if (IS_ERR(clp)) {
		dprintk("<-- nfs_init_server() = error %ld\n", PTR_ERR(clp));
		return PTR_ERR(clp);
	}

	server->nfs_client = clp;

	/* Initialise the client representation from the mount data */
	server->flags = data->flags;
	server->options = data->options;
	server->caps |= NFS_CAP_HARDLINKS|NFS_CAP_SYMLINKS|NFS_CAP_FILEID|
		NFS_CAP_MODE|NFS_CAP_NLINK|NFS_CAP_OWNER|NFS_CAP_OWNER_GROUP|
		NFS_CAP_ATIME|NFS_CAP_CTIME|NFS_CAP_MTIME;

	if (data->rsize)
		server->rsize = nfs_block_size(data->rsize, NULL);
	if (data->wsize)
		server->wsize = nfs_block_size(data->wsize, NULL);

	server->acregmin = data->acregmin * HZ;
	server->acregmax = data->acregmax * HZ;
	server->acdirmin = data->acdirmin * HZ;
	server->acdirmax = data->acdirmax * HZ;
=======
			   const struct fs_context *fc)
{
	const struct nfs_fs_context *ctx = nfs_fc2context(fc);
	struct rpc_timeout timeparms;
	struct nfs_client_initdata cl_init = {
		.hostname = ctx->nfs_server.hostname,
		.addr = &ctx->nfs_server._address,
		.addrlen = ctx->nfs_server.addrlen,
		.nfs_mod = ctx->nfs_mod,
		.proto = ctx->nfs_server.protocol,
		.net = fc->net_ns,
		.timeparms = &timeparms,
		.cred = server->cred,
		.nconnect = ctx->nfs_server.nconnect,
		.init_flags = (1UL << NFS_CS_REUSEPORT),
		.xprtsec = ctx->xprtsec,
	};
	struct nfs_client *clp;
	int error;

	nfs_init_timeout_values(&timeparms, ctx->nfs_server.protocol,
				ctx->timeo, ctx->retrans);
	if (ctx->flags & NFS_MOUNT_NORESVPORT)
		set_bit(NFS_CS_NORESVPORT, &cl_init.init_flags);

	/* Allocate or find a client reference we can use */
	clp = nfs_get_client(&cl_init);
	if (IS_ERR(clp))
		return PTR_ERR(clp);

	server->nfs_client = clp;
	nfs_sysfs_add_server(server);
	nfs_sysfs_link_rpc_client(server, clp->cl_rpcclient, "_state");

	/* Initialise the client representation from the mount data */
	server->flags = ctx->flags;
	server->options = ctx->options;
	server->caps |= NFS_CAP_HARDLINKS | NFS_CAP_SYMLINKS;

	switch (clp->rpc_ops->version) {
	case 2:
		server->fattr_valid = NFS_ATTR_FATTR_V2;
		break;
	case 3:
		server->fattr_valid = NFS_ATTR_FATTR_V3;
		break;
	default:
		server->fattr_valid = NFS_ATTR_FATTR_V4;
	}

	if (ctx->rsize)
		server->rsize = nfs_io_size(ctx->rsize, clp->cl_proto);
	if (ctx->wsize)
		server->wsize = nfs_io_size(ctx->wsize, clp->cl_proto);

	server->acregmin = ctx->acregmin * HZ;
	server->acregmax = ctx->acregmax * HZ;
	server->acdirmin = ctx->acdirmin * HZ;
	server->acdirmax = ctx->acdirmax * HZ;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Start lockd here, before we might error out */
	error = nfs_start_lockd(server);
	if (error < 0)
		goto error;

<<<<<<< HEAD
	server->port = data->nfs_server.port;

	error = nfs_init_server_rpcclient(server, &timeparms, data->auth_flavors[0]);
=======
	server->port = ctx->nfs_server.port;
	server->auth_info = ctx->auth_info;

	error = nfs_init_server_rpcclient(server, &timeparms,
					  ctx->selected_flavor);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (error < 0)
		goto error;

	/* Preserve the values of mount_server-related mount options */
<<<<<<< HEAD
	if (data->mount_server.addrlen) {
		memcpy(&server->mountd_address, &data->mount_server.address,
			data->mount_server.addrlen);
		server->mountd_addrlen = data->mount_server.addrlen;
	}
	server->mountd_version = data->mount_server.version;
	server->mountd_port = data->mount_server.port;
	server->mountd_protocol = data->mount_server.protocol;

	server->namelen  = data->namlen;
	/* Create a client RPC handle for the NFSv3 ACL management interface */
	nfs_init_server_aclclient(server);
	dprintk("<-- nfs_init_server() = 0 [new %p]\n", clp);
=======
	if (ctx->mount_server.addrlen) {
		memcpy(&server->mountd_address, &ctx->mount_server.address,
			ctx->mount_server.addrlen);
		server->mountd_addrlen = ctx->mount_server.addrlen;
	}
	server->mountd_version = ctx->mount_server.version;
	server->mountd_port = ctx->mount_server.port;
	server->mountd_protocol = ctx->mount_server.protocol;

	server->namelen  = ctx->namlen;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;

error:
	server->nfs_client = NULL;
	nfs_put_client(clp);
<<<<<<< HEAD
	dprintk("<-- nfs_init_server() = xerror %d\n", error);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return error;
}

/*
 * Load up the server record from information gained in an fsinfo record
 */
static void nfs_server_set_fsinfo(struct nfs_server *server,
<<<<<<< HEAD
				  struct nfs_fh *mntfh,
				  struct nfs_fsinfo *fsinfo)
{
	unsigned long max_rpc_payload;

	/* Work out a lot of parameters */
	if (server->rsize == 0)
		server->rsize = nfs_block_size(fsinfo->rtpref, NULL);
	if (server->wsize == 0)
		server->wsize = nfs_block_size(fsinfo->wtpref, NULL);

	if (fsinfo->rtmax >= 512 && server->rsize > fsinfo->rtmax)
		server->rsize = nfs_block_size(fsinfo->rtmax, NULL);
	if (fsinfo->wtmax >= 512 && server->wsize > fsinfo->wtmax)
		server->wsize = nfs_block_size(fsinfo->wtmax, NULL);

	max_rpc_payload = nfs_block_size(rpc_max_payload(server->client), NULL);
=======
				  struct nfs_fsinfo *fsinfo)
{
	struct nfs_client *clp = server->nfs_client;
	unsigned long max_rpc_payload, raw_max_rpc_payload;

	/* Work out a lot of parameters */
	if (server->rsize == 0)
		server->rsize = nfs_io_size(fsinfo->rtpref, clp->cl_proto);
	if (server->wsize == 0)
		server->wsize = nfs_io_size(fsinfo->wtpref, clp->cl_proto);

	if (fsinfo->rtmax >= 512 && server->rsize > fsinfo->rtmax)
		server->rsize = nfs_io_size(fsinfo->rtmax, clp->cl_proto);
	if (fsinfo->wtmax >= 512 && server->wsize > fsinfo->wtmax)
		server->wsize = nfs_io_size(fsinfo->wtmax, clp->cl_proto);

	raw_max_rpc_payload = rpc_max_payload(server->client);
	max_rpc_payload = nfs_block_size(raw_max_rpc_payload, NULL);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (server->rsize > max_rpc_payload)
		server->rsize = max_rpc_payload;
	if (server->rsize > NFS_MAX_FILE_IO_SIZE)
		server->rsize = NFS_MAX_FILE_IO_SIZE;
<<<<<<< HEAD
	server->rpages = (server->rsize + PAGE_CACHE_SIZE - 1) >> PAGE_CACHE_SHIFT;

	server->backing_dev_info.name = "nfs";
	server->backing_dev_info.ra_pages = server->rpages * NFS_MAX_READAHEAD;
=======
	server->rpages = (server->rsize + PAGE_SIZE - 1) >> PAGE_SHIFT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (server->wsize > max_rpc_payload)
		server->wsize = max_rpc_payload;
	if (server->wsize > NFS_MAX_FILE_IO_SIZE)
		server->wsize = NFS_MAX_FILE_IO_SIZE;
<<<<<<< HEAD
	server->wpages = (server->wsize + PAGE_CACHE_SIZE - 1) >> PAGE_CACHE_SHIFT;
	server->pnfs_blksize = fsinfo->blksize;
	set_pnfs_layoutdriver(server, mntfh, fsinfo->layouttype);
=======
	server->wpages = (server->wsize + PAGE_SIZE - 1) >> PAGE_SHIFT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	server->wtmult = nfs_block_bits(fsinfo->wtmult, NULL);

	server->dtsize = nfs_block_size(fsinfo->dtpref, NULL);
<<<<<<< HEAD
	if (server->dtsize > PAGE_CACHE_SIZE * NFS_MAX_READDIR_PAGES)
		server->dtsize = PAGE_CACHE_SIZE * NFS_MAX_READDIR_PAGES;
=======
	if (server->dtsize > NFS_MAX_FILE_IO_SIZE)
		server->dtsize = NFS_MAX_FILE_IO_SIZE;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (server->dtsize > server->rsize)
		server->dtsize = server->rsize;

	if (server->flags & NFS_MOUNT_NOAC) {
		server->acregmin = server->acregmax = 0;
		server->acdirmin = server->acdirmax = 0;
	}

	server->maxfilesize = fsinfo->maxfilesize;

	server->time_delta = fsinfo->time_delta;
<<<<<<< HEAD

	/* We're airborne Set socket buffersize */
	rpc_setbufsize(server->client, server->wsize + 100, server->rsize + 100);
=======
	server->change_attr_type = fsinfo->change_attr_type;

	server->clone_blksize = fsinfo->clone_blksize;
	/* We're airborne Set socket buffersize */
	rpc_setbufsize(server->client, server->wsize + 100, server->rsize + 100);

#ifdef CONFIG_NFS_V4_2
	/*
	 * Defaults until limited by the session parameters.
	 */
	server->gxasize = min_t(unsigned int, raw_max_rpc_payload,
				XATTR_SIZE_MAX);
	server->sxasize = min_t(unsigned int, raw_max_rpc_payload,
				XATTR_SIZE_MAX);
	server->lxasize = min_t(unsigned int, raw_max_rpc_payload,
				nfs42_listxattr_xdrsize(XATTR_LIST_MAX));

	if (fsinfo->xattr_support)
		server->caps |= NFS_CAP_XATTR;
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Probe filesystem information, including the FSID on v2/v3
 */
static int nfs_probe_fsinfo(struct nfs_server *server, struct nfs_fh *mntfh, struct nfs_fattr *fattr)
{
	struct nfs_fsinfo fsinfo;
	struct nfs_client *clp = server->nfs_client;
	int error;

<<<<<<< HEAD
	dprintk("--> nfs_probe_fsinfo()\n");

	if (clp->rpc_ops->set_capabilities != NULL) {
		error = clp->rpc_ops->set_capabilities(server, mntfh);
		if (error < 0)
			goto out_error;
	}

	fsinfo.fattr = fattr;
	fsinfo.layouttype = 0;
	error = clp->rpc_ops->fsinfo(server, mntfh, &fsinfo);
	if (error < 0)
		goto out_error;

	nfs_server_set_fsinfo(server, mntfh, &fsinfo);
=======
	if (clp->rpc_ops->set_capabilities != NULL) {
		error = clp->rpc_ops->set_capabilities(server, mntfh);
		if (error < 0)
			return error;
	}

	fsinfo.fattr = fattr;
	fsinfo.nlayouttypes = 0;
	memset(fsinfo.layouttype, 0, sizeof(fsinfo.layouttype));
	error = clp->rpc_ops->fsinfo(server, mntfh, &fsinfo);
	if (error < 0)
		return error;

	nfs_server_set_fsinfo(server, &fsinfo);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Get some general file system info */
	if (server->namelen == 0) {
		struct nfs_pathconf pathinfo;

		pathinfo.fattr = fattr;
		nfs_fattr_init(fattr);

		if (clp->rpc_ops->pathconf(server, mntfh, &pathinfo) >= 0)
			server->namelen = pathinfo.max_namelen;
	}

<<<<<<< HEAD
	dprintk("<-- nfs_probe_fsinfo() = 0\n");
	return 0;

out_error:
	dprintk("nfs_probe_fsinfo: error = %d\n", -error);
	return error;
}
=======
	if (clp->rpc_ops->discover_trunking != NULL &&
			(server->caps & NFS_CAP_FS_LOCATIONS &&
			 (server->flags & NFS_MOUNT_TRUNK_DISCOVERY))) {
		error = clp->rpc_ops->discover_trunking(server, mntfh);
		if (error < 0)
			return error;
	}

	return 0;
}

/*
 * Grab the destination's particulars, including lease expiry time.
 *
 * Returns zero if probe succeeded and retrieved FSID matches the FSID
 * we have cached.
 */
int nfs_probe_server(struct nfs_server *server, struct nfs_fh *mntfh)
{
	struct nfs_fattr *fattr;
	int error;

	fattr = nfs_alloc_fattr();
	if (fattr == NULL)
		return -ENOMEM;

	/* Sanity: the probe won't work if the destination server
	 * does not recognize the migrated FH. */
	error = nfs_probe_fsinfo(server, mntfh, fattr);

	nfs_free_fattr(fattr);
	return error;
}
EXPORT_SYMBOL_GPL(nfs_probe_server);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Copy useful information when duplicating a server record
 */
<<<<<<< HEAD
static void nfs_server_copy_userdata(struct nfs_server *target, struct nfs_server *source)
=======
void nfs_server_copy_userdata(struct nfs_server *target, struct nfs_server *source)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	target->flags = source->flags;
	target->rsize = source->rsize;
	target->wsize = source->wsize;
	target->acregmin = source->acregmin;
	target->acregmax = source->acregmax;
	target->acdirmin = source->acdirmin;
	target->acdirmax = source->acdirmax;
	target->caps = source->caps;
	target->options = source->options;
<<<<<<< HEAD
}

static void nfs_server_insert_lists(struct nfs_server *server)
{
	struct nfs_client *clp = server->nfs_client;
	struct nfs_net *nn = net_generic(clp->net, nfs_net_id);
=======
	target->auth_info = source->auth_info;
	target->port = source->port;
}
EXPORT_SYMBOL_GPL(nfs_server_copy_userdata);

void nfs_server_insert_lists(struct nfs_server *server)
{
	struct nfs_client *clp = server->nfs_client;
	struct nfs_net *nn = net_generic(clp->cl_net, nfs_net_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spin_lock(&nn->nfs_client_lock);
	list_add_tail_rcu(&server->client_link, &clp->cl_superblocks);
	list_add_tail(&server->master_link, &nn->nfs_volume_list);
	clear_bit(NFS_CS_STOP_RENEW, &clp->cl_res_state);
	spin_unlock(&nn->nfs_client_lock);

}
<<<<<<< HEAD

static void nfs_server_remove_lists(struct nfs_server *server)
=======
EXPORT_SYMBOL_GPL(nfs_server_insert_lists);

void nfs_server_remove_lists(struct nfs_server *server)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct nfs_client *clp = server->nfs_client;
	struct nfs_net *nn;

	if (clp == NULL)
		return;
<<<<<<< HEAD
	nn = net_generic(clp->net, nfs_net_id);
=======
	nn = net_generic(clp->cl_net, nfs_net_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_lock(&nn->nfs_client_lock);
	list_del_rcu(&server->client_link);
	if (list_empty(&clp->cl_superblocks))
		set_bit(NFS_CS_STOP_RENEW, &clp->cl_res_state);
	list_del(&server->master_link);
	spin_unlock(&nn->nfs_client_lock);

	synchronize_rcu();
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(nfs_server_remove_lists);

static DEFINE_IDA(s_sysfs_ids);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Allocate and initialise a server record
 */
<<<<<<< HEAD
static struct nfs_server *nfs_alloc_server(void)
=======
struct nfs_server *nfs_alloc_server(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct nfs_server *server;

	server = kzalloc(sizeof(struct nfs_server), GFP_KERNEL);
	if (!server)
		return NULL;

<<<<<<< HEAD
=======
	server->s_sysfs_id = ida_alloc(&s_sysfs_ids, GFP_KERNEL);
	if (server->s_sysfs_id < 0) {
		kfree(server);
		return NULL;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	server->client = server->client_acl = ERR_PTR(-EINVAL);

	/* Zero out the NFS state stuff */
	INIT_LIST_HEAD(&server->client_link);
	INIT_LIST_HEAD(&server->master_link);
	INIT_LIST_HEAD(&server->delegations);
	INIT_LIST_HEAD(&server->layouts);
	INIT_LIST_HEAD(&server->state_owners_lru);
<<<<<<< HEAD
=======
	INIT_LIST_HEAD(&server->ss_copies);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	atomic_set(&server->active, 0);

	server->io_stats = nfs_alloc_iostats();
	if (!server->io_stats) {
		kfree(server);
		return NULL;
	}

<<<<<<< HEAD
	if (bdi_init(&server->backing_dev_info)) {
		nfs_free_iostats(server->io_stats);
		kfree(server);
		return NULL;
	}
=======
	server->change_attr_type = NFS4_CHANGE_TYPE_IS_UNDEFINED;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ida_init(&server->openowner_id);
	ida_init(&server->lockowner_id);
	pnfs_init_server(server);
<<<<<<< HEAD

	return server;
}
=======
	rpc_init_wait_queue(&server->uoc_rpcwaitq, "NFS UOC");

	return server;
}
EXPORT_SYMBOL_GPL(nfs_alloc_server);

static void delayed_free(struct rcu_head *p)
{
	struct nfs_server *server = container_of(p, struct nfs_server, rcu);

	nfs_free_iostats(server->io_stats);
	kfree(server);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Free up a server record
 */
void nfs_free_server(struct nfs_server *server)
{
<<<<<<< HEAD
	dprintk("--> nfs_free_server()\n");

	nfs_server_remove_lists(server);
	unset_pnfs_layoutdriver(server);
=======
	nfs_server_remove_lists(server);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (server->destroy != NULL)
		server->destroy(server);

	if (!IS_ERR(server->client_acl))
		rpc_shutdown_client(server->client_acl);
	if (!IS_ERR(server->client))
		rpc_shutdown_client(server->client);

	nfs_put_client(server->nfs_client);

<<<<<<< HEAD
	ida_destroy(&server->lockowner_id);
	ida_destroy(&server->openowner_id);
	nfs_free_iostats(server->io_stats);
	bdi_destroy(&server->backing_dev_info);
	kfree(server);
	nfs_release_automount_timer();
	dprintk("<-- nfs_free_server()\n");
}
=======
	if (server->kobj.state_initialized) {
		nfs_sysfs_remove_server(server);
		kobject_put(&server->kobj);
	}
	ida_free(&s_sysfs_ids, server->s_sysfs_id);

	ida_destroy(&server->lockowner_id);
	ida_destroy(&server->openowner_id);
	put_cred(server->cred);
	nfs_release_automount_timer();
	call_rcu(&server->rcu, delayed_free);
}
EXPORT_SYMBOL_GPL(nfs_free_server);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Create a version 2 or 3 volume record
 * - keyed on server and FSID
 */
<<<<<<< HEAD
struct nfs_server *nfs_create_server(const struct nfs_parsed_mount_data *data,
				     struct nfs_fh *mntfh)
{
=======
struct nfs_server *nfs_create_server(struct fs_context *fc)
{
	struct nfs_fs_context *ctx = nfs_fc2context(fc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct nfs_server *server;
	struct nfs_fattr *fattr;
	int error;

	server = nfs_alloc_server();
	if (!server)
		return ERR_PTR(-ENOMEM);

<<<<<<< HEAD
=======
	server->cred = get_cred(fc->cred);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	error = -ENOMEM;
	fattr = nfs_alloc_fattr();
	if (fattr == NULL)
		goto error;

	/* Get a client representation */
<<<<<<< HEAD
	error = nfs_init_server(server, data);
	if (error < 0)
		goto error;

	BUG_ON(!server->nfs_client);
	BUG_ON(!server->nfs_client->rpc_ops);
	BUG_ON(!server->nfs_client->rpc_ops->file_inode_ops);

	/* Probe the root fh to retrieve its FSID */
	error = nfs_probe_fsinfo(server, mntfh, fattr);
=======
	error = nfs_init_server(server, fc);
	if (error < 0)
		goto error;

	/* Probe the root fh to retrieve its FSID */
	error = nfs_probe_fsinfo(server, ctx->mntfh, fattr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (error < 0)
		goto error;
	if (server->nfs_client->rpc_ops->version == 3) {
		if (server->namelen == 0 || server->namelen > NFS3_MAXNAMLEN)
			server->namelen = NFS3_MAXNAMLEN;
<<<<<<< HEAD
		if (!(data->flags & NFS_MOUNT_NORDIRPLUS))
=======
		if (!(ctx->flags & NFS_MOUNT_NORDIRPLUS))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			server->caps |= NFS_CAP_READDIRPLUS;
	} else {
		if (server->namelen == 0 || server->namelen > NFS2_MAXNAMLEN)
			server->namelen = NFS2_MAXNAMLEN;
	}

	if (!(fattr->valid & NFS_ATTR_FATTR)) {
<<<<<<< HEAD
		error = server->nfs_client->rpc_ops->getattr(server, mntfh, fattr);
=======
		error = ctx->nfs_mod->rpc_ops->getattr(server, ctx->mntfh,
						       fattr, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (error < 0) {
			dprintk("nfs_create_server: getattr error = %d\n", -error);
			goto error;
		}
	}
	memcpy(&server->fsid, &fattr->fsid, sizeof(server->fsid));

	dprintk("Server FSID: %llx:%llx\n",
		(unsigned long long) server->fsid.major,
		(unsigned long long) server->fsid.minor);

	nfs_server_insert_lists(server);
	server->mount_time = jiffies;
	nfs_free_fattr(fattr);
	return server;

error:
	nfs_free_fattr(fattr);
	nfs_free_server(server);
	return ERR_PTR(error);
}
<<<<<<< HEAD

#ifdef CONFIG_NFS_V4
/*
 * NFSv4.0 callback thread helper
 *
 * Find a client by callback identifier
 */
struct nfs_client *
nfs4_find_client_ident(struct net *net, int cb_ident)
{
	struct nfs_client *clp;
	struct nfs_net *nn = net_generic(net, nfs_net_id);

	spin_lock(&nn->nfs_client_lock);
	clp = idr_find(&nn->cb_ident_idr, cb_ident);
	if (clp)
		atomic_inc(&clp->cl_count);
	spin_unlock(&nn->nfs_client_lock);
	return clp;
}

#if defined(CONFIG_NFS_V4_1)
/*
 * NFSv4.1 callback thread helper
 * For CB_COMPOUND calls, find a client by IP address, protocol version,
 * minorversion, and sessionID
 *
 * Returns NULL if no such client
 */
struct nfs_client *
nfs4_find_client_sessionid(struct net *net, const struct sockaddr *addr,
			   struct nfs4_sessionid *sid)
{
	struct nfs_client *clp;
	struct nfs_net *nn = net_generic(net, nfs_net_id);

	spin_lock(&nn->nfs_client_lock);
	list_for_each_entry(clp, &nn->nfs_client_list, cl_share_link) {
		if (nfs4_cb_match_client(addr, clp, 1) == false)
			continue;

		if (!nfs4_has_session(clp))
			continue;

		/* Match sessionid*/
		if (memcmp(clp->cl_session->sess_id.data,
		    sid->data, NFS4_MAX_SESSIONID_LEN) != 0)
			continue;

		atomic_inc(&clp->cl_count);
		spin_unlock(&nn->nfs_client_lock);
		return clp;
	}
	spin_unlock(&nn->nfs_client_lock);
	return NULL;
}

#else /* CONFIG_NFS_V4_1 */

struct nfs_client *
nfs4_find_client_sessionid(struct net *net, const struct sockaddr *addr,
			   struct nfs4_sessionid *sid)
{
	return NULL;
}
#endif /* CONFIG_NFS_V4_1 */

/*
 * Initialize the NFS4 callback service
 */
static int nfs4_init_callback(struct nfs_client *clp)
{
	int error;

	if (clp->rpc_ops->version == 4) {
		struct rpc_xprt *xprt;

		xprt = rcu_dereference_raw(clp->cl_rpcclient->cl_xprt);

		if (nfs4_has_session(clp)) {
			error = xprt_setup_backchannel(xprt,
						NFS41_BC_MIN_CALLBACKS);
			if (error < 0)
				return error;
		}

		error = nfs_callback_up(clp->cl_mvops->minor_version, xprt);
		if (error < 0) {
			dprintk("%s: failed to start callback. Error = %d\n",
				__func__, error);
			return error;
		}
		__set_bit(NFS_CS_CALLBACK, &clp->cl_res_state);
	}
	return 0;
}

/*
 * Initialize the minor version specific parts of an NFS4 client record
 */
static int nfs4_init_client_minor_version(struct nfs_client *clp)
{
#if defined(CONFIG_NFS_V4_1)
	if (clp->cl_mvops->minor_version) {
		struct nfs4_session *session = NULL;
		/*
		 * Create the session and mark it expired.
		 * When a SEQUENCE operation encounters the expired session
		 * it will do session recovery to initialize it.
		 */
		session = nfs4_alloc_session(clp);
		if (!session)
			return -ENOMEM;

		clp->cl_session = session;
		/*
		 * The create session reply races with the server back
		 * channel probe. Mark the client NFS_CS_SESSION_INITING
		 * so that the client back channel can find the
		 * nfs_client struct
		 */
		clp->cl_cons_state = NFS_CS_SESSION_INITING;
	}
#endif /* CONFIG_NFS_V4_1 */

	return nfs4_init_callback(clp);
}

/*
 * Initialise an NFS4 client record
 */
int nfs4_init_client(struct nfs_client *clp,
		     const struct rpc_timeout *timeparms,
		     const char *ip_addr,
		     rpc_authflavor_t authflavour,
		     int noresvport)
{
	char buf[INET6_ADDRSTRLEN + 1];
	int error;

	if (clp->cl_cons_state == NFS_CS_READY) {
		/* the client is initialised already */
		dprintk("<-- nfs4_init_client() = 0 [already %p]\n", clp);
		return 0;
	}

	/* Check NFS protocol revision and initialize RPC op vector */
	clp->rpc_ops = &nfs_v4_clientops;

	error = nfs_create_rpc_client(clp, timeparms, authflavour,
				      1, noresvport);
	if (error < 0)
		goto error;

	/* If no clientaddr= option was specified, find a usable cb address */
	if (ip_addr == NULL) {
		struct sockaddr_storage cb_addr;
		struct sockaddr *sap = (struct sockaddr *)&cb_addr;

		error = rpc_localaddr(clp->cl_rpcclient, sap, sizeof(cb_addr));
		if (error < 0)
			goto error;
		error = rpc_ntop(sap, buf, sizeof(buf));
		if (error < 0)
			goto error;
		ip_addr = (const char *)buf;
	}
	strlcpy(clp->cl_ipaddr, ip_addr, sizeof(clp->cl_ipaddr));

	error = nfs_idmap_new(clp);
	if (error < 0) {
		dprintk("%s: failed to create idmapper. Error = %d\n",
			__func__, error);
		goto error;
	}
	__set_bit(NFS_CS_IDMAP, &clp->cl_res_state);

	error = nfs4_init_client_minor_version(clp);
	if (error < 0)
		goto error;

	if (!nfs4_has_session(clp))
		nfs_mark_client_ready(clp, NFS_CS_READY);
	return 0;

error:
	nfs_mark_client_ready(clp, error);
	dprintk("<-- nfs4_init_client() = xerror %d\n", error);
	return error;
}

/*
 * Set up an NFS4 client
 */
static int nfs4_set_client(struct nfs_server *server,
		const char *hostname,
		const struct sockaddr *addr,
		const size_t addrlen,
		const char *ip_addr,
		rpc_authflavor_t authflavour,
		int proto, const struct rpc_timeout *timeparms,
		u32 minorversion, struct net *net)
{
	struct nfs_client_initdata cl_init = {
		.hostname = hostname,
		.addr = addr,
		.addrlen = addrlen,
		.rpc_ops = &nfs_v4_clientops,
		.proto = proto,
		.minorversion = minorversion,
		.net = net,
	};
	struct nfs_client *clp;
	int error;

	dprintk("--> nfs4_set_client()\n");

	/* Allocate or find a client reference we can use */
	clp = nfs_get_client(&cl_init, timeparms, ip_addr, authflavour,
			     server->flags & NFS_MOUNT_NORESVPORT);
	if (IS_ERR(clp)) {
		error = PTR_ERR(clp);
		goto error;
	}

	/*
	 * Query for the lease time on clientid setup or renewal
	 *
	 * Note that this will be set on nfs_clients that were created
	 * only for the DS role and did not set this bit, but now will
	 * serve a dual role.
	 */
	set_bit(NFS_CS_CHECK_LEASE_TIME, &clp->cl_res_state);

	server->nfs_client = clp;
	dprintk("<-- nfs4_set_client() = 0 [new %p]\n", clp);
	return 0;
error:
	dprintk("<-- nfs4_set_client() = xerror %d\n", error);
	return error;
}

/*
 * Set up a pNFS Data Server client.
 *
 * Return any existing nfs_client that matches server address,port,version
 * and minorversion.
 *
 * For a new nfs_client, use a soft mount (default), a low retrans and a
 * low timeout interval so that if a connection is lost, we retry through
 * the MDS.
 */
struct nfs_client *nfs4_set_ds_client(struct nfs_client* mds_clp,
		const struct sockaddr *ds_addr,
		int ds_addrlen, int ds_proto)
{
	struct nfs_client_initdata cl_init = {
		.addr = ds_addr,
		.addrlen = ds_addrlen,
		.rpc_ops = &nfs_v4_clientops,
		.proto = ds_proto,
		.minorversion = mds_clp->cl_minorversion,
		.net = mds_clp->net,
	};
	struct rpc_timeout ds_timeout = {
		.to_initval = 15 * HZ,
		.to_maxval = 15 * HZ,
		.to_retries = 1,
		.to_exponential = 1,
	};
	struct nfs_client *clp;

	/*
	 * Set an authflavor equual to the MDS value. Use the MDS nfs_client
	 * cl_ipaddr so as to use the same EXCHANGE_ID co_ownerid as the MDS
	 * (section 13.1 RFC 5661).
	 */
	clp = nfs_get_client(&cl_init, &ds_timeout, mds_clp->cl_ipaddr,
			     mds_clp->cl_rpcclient->cl_auth->au_flavor, 0);

	dprintk("<-- %s %p\n", __func__, clp);
	return clp;
}
EXPORT_SYMBOL_GPL(nfs4_set_ds_client);

/*
 * Session has been established, and the client marked ready.
 * Set the mount rsize and wsize with negotiated fore channel
 * attributes which will be bound checked in nfs_server_set_fsinfo.
 */
static void nfs4_session_set_rwsize(struct nfs_server *server)
{
#ifdef CONFIG_NFS_V4_1
	struct nfs4_session *sess;
	u32 server_resp_sz;
	u32 server_rqst_sz;

	if (!nfs4_has_session(server->nfs_client))
		return;
	sess = server->nfs_client->cl_session;
	server_resp_sz = sess->fc_attrs.max_resp_sz - nfs41_maxread_overhead;
	server_rqst_sz = sess->fc_attrs.max_rqst_sz - nfs41_maxwrite_overhead;

	if (server->rsize > server_resp_sz)
		server->rsize = server_resp_sz;
	if (server->wsize > server_rqst_sz)
		server->wsize = server_rqst_sz;
#endif /* CONFIG_NFS_V4_1 */
}

static int nfs4_server_common_setup(struct nfs_server *server,
		struct nfs_fh *mntfh)
{
	struct nfs_fattr *fattr;
	int error;

	BUG_ON(!server->nfs_client);
	BUG_ON(!server->nfs_client->rpc_ops);
	BUG_ON(!server->nfs_client->rpc_ops->file_inode_ops);

	/* data servers support only a subset of NFSv4.1 */
	if (is_ds_only_client(server->nfs_client))
		return -EPROTONOSUPPORT;

	fattr = nfs_alloc_fattr();
	if (fattr == NULL)
		return -ENOMEM;

	/* We must ensure the session is initialised first */
	error = nfs4_init_session(server);
	if (error < 0)
		goto out;

	/* Probe the root fh to retrieve its FSID and filehandle */
	error = nfs4_get_rootfh(server, mntfh);
	if (error < 0)
		goto out;

	dprintk("Server FSID: %llx:%llx\n",
			(unsigned long long) server->fsid.major,
			(unsigned long long) server->fsid.minor);
	dprintk("Mount FH: %d\n", mntfh->size);

	nfs4_session_set_rwsize(server);

	error = nfs_probe_fsinfo(server, mntfh, fattr);
	if (error < 0)
		goto out;

	if (server->namelen == 0 || server->namelen > NFS4_MAXNAMLEN)
		server->namelen = NFS4_MAXNAMLEN;

	nfs_server_insert_lists(server);
	server->mount_time = jiffies;
	server->destroy = nfs4_destroy_server;
out:
	nfs_free_fattr(fattr);
	return error;
}

/*
 * Create a version 4 volume record
 */
static int nfs4_init_server(struct nfs_server *server,
		const struct nfs_parsed_mount_data *data)
{
	struct rpc_timeout timeparms;
	int error;

	dprintk("--> nfs4_init_server()\n");

	nfs_init_timeout_values(&timeparms, data->nfs_server.protocol,
			data->timeo, data->retrans);

	/* Initialise the client representation from the mount data */
	server->flags = data->flags;
	server->caps |= NFS_CAP_ATOMIC_OPEN|NFS_CAP_CHANGE_ATTR|NFS_CAP_POSIX_LOCK;
	if (!(data->flags & NFS_MOUNT_NORDIRPLUS))
			server->caps |= NFS_CAP_READDIRPLUS;
	server->options = data->options;

	/* Get a client record */
	error = nfs4_set_client(server,
			data->nfs_server.hostname,
			(const struct sockaddr *)&data->nfs_server.address,
			data->nfs_server.addrlen,
			data->client_address,
			data->auth_flavors[0],
			data->nfs_server.protocol,
			&timeparms,
			data->minorversion,
			data->net);
	if (error < 0)
		goto error;

	/*
	 * Don't use NFS uid/gid mapping if we're using AUTH_SYS or lower
	 * authentication.
	 */
	if (nfs4_disable_idmapping && data->auth_flavors[0] == RPC_AUTH_UNIX)
		server->caps |= NFS_CAP_UIDGID_NOMAP;

	if (data->rsize)
		server->rsize = nfs_block_size(data->rsize, NULL);
	if (data->wsize)
		server->wsize = nfs_block_size(data->wsize, NULL);

	server->acregmin = data->acregmin * HZ;
	server->acregmax = data->acregmax * HZ;
	server->acdirmin = data->acdirmin * HZ;
	server->acdirmax = data->acdirmax * HZ;

	server->port = data->nfs_server.port;

	error = nfs_init_server_rpcclient(server, &timeparms, data->auth_flavors[0]);

error:
	/* Done */
	dprintk("<-- nfs4_init_server() = %d\n", error);
	return error;
}

/*
 * Create a version 4 volume record
 * - keyed on server and FSID
 */
struct nfs_server *nfs4_create_server(const struct nfs_parsed_mount_data *data,
				      struct nfs_fh *mntfh)
{
	struct nfs_server *server;
	int error;

	dprintk("--> nfs4_create_server()\n");

	server = nfs_alloc_server();
	if (!server)
		return ERR_PTR(-ENOMEM);

	/* set up the general RPC client */
	error = nfs4_init_server(server, data);
	if (error < 0)
		goto error;

	error = nfs4_server_common_setup(server, mntfh);
	if (error < 0)
		goto error;

	dprintk("<-- nfs4_create_server() = %p\n", server);
	return server;

error:
	nfs_free_server(server);
	dprintk("<-- nfs4_create_server() = error %d\n", error);
	return ERR_PTR(error);
}

/*
 * Create an NFS4 referral server record
 */
struct nfs_server *nfs4_create_referral_server(struct nfs_clone_mount *data,
					       struct nfs_fh *mntfh)
{
	struct nfs_client *parent_client;
	struct nfs_server *server, *parent_server;
	int error;

	dprintk("--> nfs4_create_referral_server()\n");

	server = nfs_alloc_server();
	if (!server)
		return ERR_PTR(-ENOMEM);

	parent_server = NFS_SB(data->sb);
	parent_client = parent_server->nfs_client;

	/* Initialise the client representation from the parent server */
	nfs_server_copy_userdata(server, parent_server);
	server->caps |= NFS_CAP_ATOMIC_OPEN|NFS_CAP_CHANGE_ATTR;

	/* Get a client representation.
	 * Note: NFSv4 always uses TCP, */
	error = nfs4_set_client(server, data->hostname,
				data->addr,
				data->addrlen,
				parent_client->cl_ipaddr,
				data->authflavor,
				rpc_protocol(parent_server->client),
				parent_server->client->cl_timeout,
				parent_client->cl_mvops->minor_version,
				parent_client->net);
	if (error < 0)
		goto error;

	error = nfs_init_server_rpcclient(server, parent_server->client->cl_timeout, data->authflavor);
	if (error < 0)
		goto error;

	error = nfs4_server_common_setup(server, mntfh);
	if (error < 0)
		goto error;

	dprintk("<-- nfs_create_referral_server() = %p\n", server);
	return server;

error:
	nfs_free_server(server);
	dprintk("<-- nfs4_create_referral_server() = error %d\n", error);
	return ERR_PTR(error);
}

#endif /* CONFIG_NFS_V4 */
=======
EXPORT_SYMBOL_GPL(nfs_create_server);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Clone an NFS2, NFS3 or NFS4 server record
 */
struct nfs_server *nfs_clone_server(struct nfs_server *source,
				    struct nfs_fh *fh,
				    struct nfs_fattr *fattr,
				    rpc_authflavor_t flavor)
{
	struct nfs_server *server;
<<<<<<< HEAD
	struct nfs_fattr *fattr_fsinfo;
	int error;

	dprintk("--> nfs_clone_server(,%llx:%llx,)\n",
		(unsigned long long) fattr->fsid.major,
		(unsigned long long) fattr->fsid.minor);

=======
	int error;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	server = nfs_alloc_server();
	if (!server)
		return ERR_PTR(-ENOMEM);

<<<<<<< HEAD
	error = -ENOMEM;
	fattr_fsinfo = nfs_alloc_fattr();
	if (fattr_fsinfo == NULL)
		goto out_free_server;
=======
	server->cred = get_cred(source->cred);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Copy data from the source */
	server->nfs_client = source->nfs_client;
	server->destroy = source->destroy;
<<<<<<< HEAD
	atomic_inc(&server->nfs_client->cl_count);
=======
	refcount_inc(&server->nfs_client->cl_count);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	nfs_server_copy_userdata(server, source);

	server->fsid = fattr->fsid;

<<<<<<< HEAD
=======
	nfs_sysfs_add_server(server);

	nfs_sysfs_link_rpc_client(server,
		server->nfs_client->cl_rpcclient, "_state");

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	error = nfs_init_server_rpcclient(server,
			source->client->cl_timeout,
			flavor);
	if (error < 0)
		goto out_free_server;
<<<<<<< HEAD
	if (!IS_ERR(source->client_acl))
		nfs_init_server_aclclient(server);

	/* probe the filesystem info for this server filesystem */
	error = nfs_probe_fsinfo(server, fh, fattr_fsinfo);
=======

	/* probe the filesystem info for this server filesystem */
	error = nfs_probe_server(server, fh);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (error < 0)
		goto out_free_server;

	if (server->namelen == 0 || server->namelen > NFS4_MAXNAMLEN)
		server->namelen = NFS4_MAXNAMLEN;

<<<<<<< HEAD
	dprintk("Cloned FSID: %llx:%llx\n",
		(unsigned long long) server->fsid.major,
		(unsigned long long) server->fsid.minor);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	error = nfs_start_lockd(server);
	if (error < 0)
		goto out_free_server;

	nfs_server_insert_lists(server);
	server->mount_time = jiffies;

<<<<<<< HEAD
	nfs_free_fattr(fattr_fsinfo);
	dprintk("<-- nfs_clone_server() = %p\n", server);
	return server;

out_free_server:
	nfs_free_fattr(fattr_fsinfo);
	nfs_free_server(server);
	dprintk("<-- nfs_clone_server() = error %d\n", error);
	return ERR_PTR(error);
}
=======
	return server;

out_free_server:
	nfs_free_server(server);
	return ERR_PTR(error);
}
EXPORT_SYMBOL_GPL(nfs_clone_server);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

void nfs_clients_init(struct net *net)
{
	struct nfs_net *nn = net_generic(net, nfs_net_id);

	INIT_LIST_HEAD(&nn->nfs_client_list);
	INIT_LIST_HEAD(&nn->nfs_volume_list);
<<<<<<< HEAD
#ifdef CONFIG_NFS_V4
	idr_init(&nn->cb_ident_idr);
#endif
	spin_lock_init(&nn->nfs_client_lock);
}

#ifdef CONFIG_PROC_FS
static struct proc_dir_entry *proc_fs_nfs;

static int nfs_server_list_open(struct inode *inode, struct file *file);
=======
#if IS_ENABLED(CONFIG_NFS_V4)
	idr_init(&nn->cb_ident_idr);
#endif
	spin_lock_init(&nn->nfs_client_lock);
	nn->boot_time = ktime_get_real();
	memset(&nn->rpcstats, 0, sizeof(nn->rpcstats));
	nn->rpcstats.program = &nfs_program;

	nfs_netns_sysfs_setup(nn, net);
}

void nfs_clients_exit(struct net *net)
{
	struct nfs_net *nn = net_generic(net, nfs_net_id);

	nfs_netns_sysfs_destroy(nn);
	nfs_cleanup_cb_ident_idr(net);
	WARN_ON_ONCE(!list_empty(&nn->nfs_client_list));
	WARN_ON_ONCE(!list_empty(&nn->nfs_volume_list));
}

#ifdef CONFIG_PROC_FS
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void *nfs_server_list_start(struct seq_file *p, loff_t *pos);
static void *nfs_server_list_next(struct seq_file *p, void *v, loff_t *pos);
static void nfs_server_list_stop(struct seq_file *p, void *v);
static int nfs_server_list_show(struct seq_file *m, void *v);

static const struct seq_operations nfs_server_list_ops = {
	.start	= nfs_server_list_start,
	.next	= nfs_server_list_next,
	.stop	= nfs_server_list_stop,
	.show	= nfs_server_list_show,
};

<<<<<<< HEAD
static const struct file_operations nfs_server_list_fops = {
	.open		= nfs_server_list_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release,
	.owner		= THIS_MODULE,
};

static int nfs_volume_list_open(struct inode *inode, struct file *file);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void *nfs_volume_list_start(struct seq_file *p, loff_t *pos);
static void *nfs_volume_list_next(struct seq_file *p, void *v, loff_t *pos);
static void nfs_volume_list_stop(struct seq_file *p, void *v);
static int nfs_volume_list_show(struct seq_file *m, void *v);

static const struct seq_operations nfs_volume_list_ops = {
	.start	= nfs_volume_list_start,
	.next	= nfs_volume_list_next,
	.stop	= nfs_volume_list_stop,
	.show	= nfs_volume_list_show,
};

<<<<<<< HEAD
static const struct file_operations nfs_volume_list_fops = {
	.open		= nfs_volume_list_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release,
	.owner		= THIS_MODULE,
};

/*
 * open "/proc/fs/nfsfs/servers" which provides a summary of servers with which
 * we're dealing
 */
static int nfs_server_list_open(struct inode *inode, struct file *file)
{
	struct seq_file *m;
	int ret;
	struct pid_namespace *pid_ns = file->f_dentry->d_sb->s_fs_info;
	struct net *net = pid_ns->child_reaper->nsproxy->net_ns;

	ret = seq_open(file, &nfs_server_list_ops);
	if (ret < 0)
		return ret;

	m = file->private_data;
	m->private = net;

	return 0;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * set up the iterator to start reading from the server list and return the first item
 */
static void *nfs_server_list_start(struct seq_file *m, loff_t *_pos)
<<<<<<< HEAD
{
	struct nfs_net *nn = net_generic(m->private, nfs_net_id);
=======
				__acquires(&nn->nfs_client_lock)
{
	struct nfs_net *nn = net_generic(seq_file_net(m), nfs_net_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* lock the list against modification */
	spin_lock(&nn->nfs_client_lock);
	return seq_list_start_head(&nn->nfs_client_list, *_pos);
}

/*
 * move to next server
 */
static void *nfs_server_list_next(struct seq_file *p, void *v, loff_t *pos)
{
<<<<<<< HEAD
	struct nfs_net *nn = net_generic(p->private, nfs_net_id);
=======
	struct nfs_net *nn = net_generic(seq_file_net(p), nfs_net_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return seq_list_next(v, &nn->nfs_client_list, pos);
}

/*
 * clean up after reading from the transports list
 */
static void nfs_server_list_stop(struct seq_file *p, void *v)
<<<<<<< HEAD
{
	struct nfs_net *nn = net_generic(p->private, nfs_net_id);
=======
				__releases(&nn->nfs_client_lock)
{
	struct nfs_net *nn = net_generic(seq_file_net(p), nfs_net_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spin_unlock(&nn->nfs_client_lock);
}

/*
 * display a header line followed by a load of call lines
 */
static int nfs_server_list_show(struct seq_file *m, void *v)
{
	struct nfs_client *clp;
<<<<<<< HEAD
	struct nfs_net *nn = net_generic(m->private, nfs_net_id);
=======
	struct nfs_net *nn = net_generic(seq_file_net(m), nfs_net_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* display header on line 1 */
	if (v == &nn->nfs_client_list) {
		seq_puts(m, "NV SERVER   PORT USE HOSTNAME\n");
		return 0;
	}

	/* display one transport per line on subsequent lines */
	clp = list_entry(v, struct nfs_client, cl_share_link);

	/* Check if the client is initialized */
	if (clp->cl_cons_state != NFS_CS_READY)
		return 0;

	rcu_read_lock();
	seq_printf(m, "v%u %s %s %3d %s\n",
		   clp->rpc_ops->version,
		   rpc_peeraddr2str(clp->cl_rpcclient, RPC_DISPLAY_HEX_ADDR),
		   rpc_peeraddr2str(clp->cl_rpcclient, RPC_DISPLAY_HEX_PORT),
<<<<<<< HEAD
		   atomic_read(&clp->cl_count),
=======
		   refcount_read(&clp->cl_count),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		   clp->cl_hostname);
	rcu_read_unlock();

	return 0;
}

/*
<<<<<<< HEAD
 * open "/proc/fs/nfsfs/volumes" which provides a summary of extant volumes
 */
static int nfs_volume_list_open(struct inode *inode, struct file *file)
{
	struct seq_file *m;
	int ret;
	struct pid_namespace *pid_ns = file->f_dentry->d_sb->s_fs_info;
	struct net *net = pid_ns->child_reaper->nsproxy->net_ns;

	ret = seq_open(file, &nfs_volume_list_ops);
	if (ret < 0)
		return ret;

	m = file->private_data;
	m->private = net;

	return 0;
}

/*
 * set up the iterator to start reading from the volume list and return the first item
 */
static void *nfs_volume_list_start(struct seq_file *m, loff_t *_pos)
{
	struct nfs_net *nn = net_generic(m->private, nfs_net_id);
=======
 * set up the iterator to start reading from the volume list and return the first item
 */
static void *nfs_volume_list_start(struct seq_file *m, loff_t *_pos)
				__acquires(&nn->nfs_client_lock)
{
	struct nfs_net *nn = net_generic(seq_file_net(m), nfs_net_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* lock the list against modification */
	spin_lock(&nn->nfs_client_lock);
	return seq_list_start_head(&nn->nfs_volume_list, *_pos);
}

/*
 * move to next volume
 */
static void *nfs_volume_list_next(struct seq_file *p, void *v, loff_t *pos)
{
<<<<<<< HEAD
	struct nfs_net *nn = net_generic(p->private, nfs_net_id);
=======
	struct nfs_net *nn = net_generic(seq_file_net(p), nfs_net_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return seq_list_next(v, &nn->nfs_volume_list, pos);
}

/*
 * clean up after reading from the transports list
 */
static void nfs_volume_list_stop(struct seq_file *p, void *v)
<<<<<<< HEAD
{
	struct nfs_net *nn = net_generic(p->private, nfs_net_id);
=======
				__releases(&nn->nfs_client_lock)
{
	struct nfs_net *nn = net_generic(seq_file_net(p), nfs_net_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spin_unlock(&nn->nfs_client_lock);
}

/*
 * display a header line followed by a load of call lines
 */
static int nfs_volume_list_show(struct seq_file *m, void *v)
{
	struct nfs_server *server;
	struct nfs_client *clp;
<<<<<<< HEAD
	char dev[8], fsid[17];
	struct nfs_net *nn = net_generic(m->private, nfs_net_id);

	/* display header on line 1 */
	if (v == &nn->nfs_volume_list) {
		seq_puts(m, "NV SERVER   PORT DEV     FSID              FSC\n");
=======
	char dev[13];	// 8 for 2^24, 1 for ':', 3 for 2^8, 1 for '\0'
	char fsid[34];	// 2 * 16 for %llx, 1 for ':', 1 for '\0'
	struct nfs_net *nn = net_generic(seq_file_net(m), nfs_net_id);

	/* display header on line 1 */
	if (v == &nn->nfs_volume_list) {
		seq_puts(m, "NV SERVER   PORT DEV          FSID"
			    "                              FSC\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;
	}
	/* display one transport per line on subsequent lines */
	server = list_entry(v, struct nfs_server, master_link);
	clp = server->nfs_client;

<<<<<<< HEAD
	snprintf(dev, 8, "%u:%u",
		 MAJOR(server->s_dev), MINOR(server->s_dev));

	snprintf(fsid, 17, "%llx:%llx",
=======
	snprintf(dev, sizeof(dev), "%u:%u",
		 MAJOR(server->s_dev), MINOR(server->s_dev));

	snprintf(fsid, sizeof(fsid), "%llx:%llx",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		 (unsigned long long) server->fsid.major,
		 (unsigned long long) server->fsid.minor);

	rcu_read_lock();
<<<<<<< HEAD
	seq_printf(m, "v%u %s %s %-7s %-17s %s\n",
=======
	seq_printf(m, "v%u %s %s %-12s %-33s %s\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		   clp->rpc_ops->version,
		   rpc_peeraddr2str(clp->cl_rpcclient, RPC_DISPLAY_HEX_ADDR),
		   rpc_peeraddr2str(clp->cl_rpcclient, RPC_DISPLAY_HEX_PORT),
		   dev,
		   fsid,
		   nfs_server_fscache_state(server));
	rcu_read_unlock();

	return 0;
}

<<<<<<< HEAD
=======
int nfs_fs_proc_net_init(struct net *net)
{
	struct nfs_net *nn = net_generic(net, nfs_net_id);
	struct proc_dir_entry *p;

	nn->proc_nfsfs = proc_net_mkdir(net, "nfsfs", net->proc_net);
	if (!nn->proc_nfsfs)
		goto error_0;

	/* a file of servers with which we're dealing */
	p = proc_create_net("servers", S_IFREG|S_IRUGO, nn->proc_nfsfs,
			&nfs_server_list_ops, sizeof(struct seq_net_private));
	if (!p)
		goto error_1;

	/* a file of volumes that we have mounted */
	p = proc_create_net("volumes", S_IFREG|S_IRUGO, nn->proc_nfsfs,
			&nfs_volume_list_ops, sizeof(struct seq_net_private));
	if (!p)
		goto error_1;
	return 0;

error_1:
	remove_proc_subtree("nfsfs", net->proc_net);
error_0:
	return -ENOMEM;
}

void nfs_fs_proc_net_exit(struct net *net)
{
	remove_proc_subtree("nfsfs", net->proc_net);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * initialise the /proc/fs/nfsfs/ directory
 */
int __init nfs_fs_proc_init(void)
{
<<<<<<< HEAD
	struct proc_dir_entry *p;

	proc_fs_nfs = proc_mkdir("fs/nfsfs", NULL);
	if (!proc_fs_nfs)
		goto error_0;

	/* a file of servers with which we're dealing */
	p = proc_create("servers", S_IFREG|S_IRUGO,
			proc_fs_nfs, &nfs_server_list_fops);
	if (!p)
		goto error_1;

	/* a file of volumes that we have mounted */
	p = proc_create("volumes", S_IFREG|S_IRUGO,
			proc_fs_nfs, &nfs_volume_list_fops);
	if (!p)
		goto error_2;
	return 0;

error_2:
	remove_proc_entry("servers", proc_fs_nfs);
error_1:
	remove_proc_entry("fs/nfsfs", NULL);
=======
	if (!proc_mkdir("fs/nfsfs", NULL))
		goto error_0;

	/* a file of servers with which we're dealing */
	if (!proc_symlink("fs/nfsfs/servers", NULL, "../../net/nfsfs/servers"))
		goto error_1;

	/* a file of volumes that we have mounted */
	if (!proc_symlink("fs/nfsfs/volumes", NULL, "../../net/nfsfs/volumes"))
		goto error_1;

	return 0;
error_1:
	remove_proc_subtree("fs/nfsfs", NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
error_0:
	return -ENOMEM;
}

/*
 * clean up the /proc/fs/nfsfs/ directory
 */
void nfs_fs_proc_exit(void)
{
<<<<<<< HEAD
	remove_proc_entry("volumes", proc_fs_nfs);
	remove_proc_entry("servers", proc_fs_nfs);
	remove_proc_entry("fs/nfsfs", NULL);
}

#endif /* CONFIG_PROC_FS */

module_param(nfs4_disable_idmapping, bool, 0644);
MODULE_PARM_DESC(nfs4_disable_idmapping,
		"Turn off NFSv4 idmapping when using 'sec=sys'");
=======
	remove_proc_subtree("fs/nfsfs", NULL);
	ida_destroy(&s_sysfs_ids);
}

#endif /* CONFIG_PROC_FS */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
