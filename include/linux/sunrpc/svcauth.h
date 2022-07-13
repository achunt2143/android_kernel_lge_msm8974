<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * linux/include/linux/sunrpc/svcauth.h
 *
 * RPC server-side authentication stuff.
 *
 * Copyright (C) 1995, 1996 Olaf Kirch <okir@monad.swb.de>
 */

#ifndef _LINUX_SUNRPC_SVCAUTH_H_
#define _LINUX_SUNRPC_SVCAUTH_H_

<<<<<<< HEAD
#ifdef __KERNEL__

#include <linux/string.h>
#include <linux/sunrpc/msg_prot.h>
#include <linux/sunrpc/cache.h>
#include <linux/hash.h>

#define SVC_CRED_NGROUPS	32
struct svc_cred {
	uid_t			cr_uid;
	gid_t			cr_gid;
	struct group_info	*cr_group_info;
};

=======
#include <linux/string.h>
#include <linux/sunrpc/msg_prot.h>
#include <linux/sunrpc/cache.h>
#include <linux/sunrpc/gss_api.h>
#include <linux/hash.h>
#include <linux/stringhash.h>
#include <linux/cred.h>

struct svc_cred {
	kuid_t			cr_uid;
	kgid_t			cr_gid;
	struct group_info	*cr_group_info;
	u32			cr_flavor; /* pseudoflavor */
	/* name of form servicetype/hostname@REALM, passed down by
	 * gss-proxy: */
	char			*cr_raw_principal;
	/* name of form servicetype@hostname, passed down by
	 * rpc.svcgssd, or computed from the above: */
	char			*cr_principal;
	char			*cr_targ_princ;
	struct gss_api_mech	*cr_gss_mech;
};

static inline void init_svc_cred(struct svc_cred *cred)
{
	cred->cr_group_info = NULL;
	cred->cr_raw_principal = NULL;
	cred->cr_principal = NULL;
	cred->cr_targ_princ = NULL;
	cred->cr_gss_mech = NULL;
}

static inline void free_svc_cred(struct svc_cred *cred)
{
	if (cred->cr_group_info)
		put_group_info(cred->cr_group_info);
	kfree(cred->cr_raw_principal);
	kfree(cred->cr_principal);
	kfree(cred->cr_targ_princ);
	gss_mech_put(cred->cr_gss_mech);
	init_svc_cred(cred);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct svc_rqst;		/* forward decl */
struct in6_addr;

/* Authentication is done in the context of a domain.
 *
 * Currently, the nfs server uses the auth_domain to stand
 * for the "client" listed in /etc/exports.
 *
 * More generally, a domain might represent a group of clients using
 * a common mechanism for authentication and having a common mapping
 * between local identity (uid) and network identity.  All clients
 * in a domain have similar general access rights.  Each domain can
 * contain multiple principals which will have different specific right
 * based on normal Discretionary Access Control.
 *
 * A domain is created by an authentication flavour module based on name
 * only.  Userspace then fills in detail on demand.
 *
 * In the case of auth_unix and auth_null, the auth_domain is also
 * associated with entries in another cache representing the mapping
 * of ip addresses to the given client.
 */
struct auth_domain {
	struct kref		ref;
	struct hlist_node	hash;
	char			*name;
	struct auth_ops		*flavour;
<<<<<<< HEAD
=======
	struct rcu_head		rcu_head;
};

enum svc_auth_status {
	SVC_GARBAGE = 1,
	SVC_SYSERR,
	SVC_VALID,
	SVC_NEGATIVE,
	SVC_OK,
	SVC_DROP,
	SVC_CLOSE,
	SVC_DENIED,
	SVC_PENDING,
	SVC_COMPLETE,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/*
 * Each authentication flavour registers an auth_ops
 * structure.
 * name is simply the name.
 * flavour gives the auth flavour. It determines where the flavour is registered
 * accept() is given a request and should verify it.
 *   It should inspect the authenticator and verifier, and possibly the data.
 *    If there is a problem with the authentication *authp should be set.
 *    The return value of accept() can indicate:
 *      OK - authorised. client and credential are set in rqstp.
 *           reqbuf points to arguments
 *           resbuf points to good place for results.  verfier
 *             is (probably) already in place.  Certainly space is
 *	       reserved for it.
 *      DROP - simply drop the request. It may have been deferred
<<<<<<< HEAD
=======
 *      CLOSE - like SVC_DROP, but request is definitely lost.
 *		If there is a tcp connection, it should be closed.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *      GARBAGE - rpc garbage_args error
 *      SYSERR - rpc system_err error
 *      DENIED - authp holds reason for denial.
 *      COMPLETE - the reply is encoded already and ready to be sent; no
 *		further processing is necessary.  (This is used for processing
 *		null procedure calls which are used to set up encryption
 *		contexts.)
 *
 *   accept is passed the proc number so that it can accept NULL rpc requests
 *   even if it cannot authenticate the client (as is sometimes appropriate).
 *
 * release() is given a request after the procedure has been run.
 *  It should sign/encrypt the results if needed
<<<<<<< HEAD
 * It should return:
 *    OK - the resbuf is ready to be sent
 *    DROP - the reply should be quitely dropped
 *    DENIED - authp holds a reason for MSG_DENIED
 *    SYSERR - rpc system_err
 *
 * domain_release()
 *   This call releases a domain.
 * set_client()
 *   Givens a pending request (struct svc_rqst), finds and assigns
 *   an appropriate 'auth_domain' as the client.
=======
 *
 * domain_release()
 *   This call releases a domain.
 *
 * set_client()
 *   Given a pending request (struct svc_rqst), finds and assigns
 *   an appropriate 'auth_domain' as the client.
 *
 * pseudoflavor()
 *   Returns RPC_AUTH pseudoflavor in use by @rqstp.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
struct auth_ops {
	char *	name;
	struct module *owner;
	int	flavour;
<<<<<<< HEAD
	int	(*accept)(struct svc_rqst *rq, __be32 *authp);
	int	(*release)(struct svc_rqst *rq);
	void	(*domain_release)(struct auth_domain *);
	int	(*set_client)(struct svc_rqst *rq);
};

#define	SVC_GARBAGE	1
#define	SVC_SYSERR	2
#define	SVC_VALID	3
#define	SVC_NEGATIVE	4
#define	SVC_OK		5
#define	SVC_DROP	6
#define	SVC_CLOSE	7	/* Like SVC_DROP, but request is definitely
				 * lost so if there is a tcp connection, it
				 * should be closed
				 */
#define	SVC_DENIED	8
#define	SVC_PENDING	9
#define	SVC_COMPLETE	10

struct svc_xprt;

extern int	svc_authenticate(struct svc_rqst *rqstp, __be32 *authp);
extern int	svc_authorise(struct svc_rqst *rqstp);
extern int	svc_set_client(struct svc_rqst *rqstp);
=======

	enum svc_auth_status	(*accept)(struct svc_rqst *rqstp);
	int			(*release)(struct svc_rqst *rqstp);
	void			(*domain_release)(struct auth_domain *dom);
	enum svc_auth_status	(*set_client)(struct svc_rqst *rqstp);
	rpc_authflavor_t	(*pseudoflavor)(struct svc_rqst *rqstp);
};

struct svc_xprt;

extern enum svc_auth_status svc_authenticate(struct svc_rqst *rqstp);
extern rpc_authflavor_t svc_auth_flavor(struct svc_rqst *rqstp);
extern int	svc_authorise(struct svc_rqst *rqstp);
extern enum svc_auth_status svc_set_client(struct svc_rqst *rqstp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern int	svc_auth_register(rpc_authflavor_t flavor, struct auth_ops *aops);
extern void	svc_auth_unregister(rpc_authflavor_t flavor);

extern struct auth_domain *unix_domain_find(char *name);
extern void auth_domain_put(struct auth_domain *item);
<<<<<<< HEAD
extern int auth_unix_add_addr(struct net *net, struct in6_addr *addr, struct auth_domain *dom);
extern struct auth_domain *auth_domain_lookup(char *name, struct auth_domain *new);
extern struct auth_domain *auth_domain_find(char *name);
extern struct auth_domain *auth_unix_lookup(struct net *net, struct in6_addr *addr);
extern int auth_unix_forget_old(struct auth_domain *dom);
extern void svcauth_unix_purge(void);
extern void svcauth_unix_info_release(struct svc_xprt *xpt);
extern int svcauth_unix_set_client(struct svc_rqst *rqstp);
=======
extern struct auth_domain *auth_domain_lookup(char *name, struct auth_domain *new);
extern struct auth_domain *auth_domain_find(char *name);
extern void svcauth_unix_purge(struct net *net);
extern void svcauth_unix_info_release(struct svc_xprt *xpt);
extern enum svc_auth_status svcauth_unix_set_client(struct svc_rqst *rqstp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

extern int unix_gid_cache_create(struct net *net);
extern void unix_gid_cache_destroy(struct net *net);

<<<<<<< HEAD
static inline unsigned long hash_str(char *name, int bits)
{
	unsigned long hash = 0;
	unsigned long l = 0;
	int len = 0;
	unsigned char c;
	do {
		if (unlikely(!(c = *name++))) {
			c = (char)len; len = -1;
		}
		l = (l << 8) | c;
		len++;
		if ((len & (BITS_PER_LONG/8-1))==0)
			hash = hash_long(hash^l, BITS_PER_LONG);
	} while (len);
	return hash >> (BITS_PER_LONG - bits);
}

static inline unsigned long hash_mem(char *buf, int length, int bits)
{
	unsigned long hash = 0;
	unsigned long l = 0;
	int len = 0;
	unsigned char c;
	do {
		if (len == length) {
			c = (char)len; len = -1;
		} else
			c = *buf++;
		l = (l << 8) | c;
		len++;
		if ((len & (BITS_PER_LONG/8-1))==0)
			hash = hash_long(hash^l, BITS_PER_LONG);
	} while (len);
	return hash >> (BITS_PER_LONG - bits);
}

#endif /* __KERNEL__ */

=======
/*
 * The <stringhash.h> functions are good enough that we don't need to
 * use hash_32() on them; just extracting the high bits is enough.
 */
static inline unsigned long hash_str(char const *name, int bits)
{
	return hashlen_hash(hashlen_string(NULL, name)) >> (32 - bits);
}

static inline unsigned long hash_mem(char const *buf, int length, int bits)
{
	return full_name_hash(NULL, buf, length) >> (32 - bits);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _LINUX_SUNRPC_SVCAUTH_H_ */
