<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * linux/net/sunrpc/auth.c
 *
 * Generic RPC client authentication API.
 *
 * Copyright (C) 1996, Olaf Kirch <okir@monad.swb.de>
 */

#include <linux/types.h>
#include <linux/sched.h>
<<<<<<< HEAD
=======
#include <linux/cred.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/hash.h>
#include <linux/sunrpc/clnt.h>
<<<<<<< HEAD
#include <linux/spinlock.h>

#ifdef RPC_DEBUG
# define RPCDBG_FACILITY	RPCDBG_AUTH
#endif
=======
#include <linux/sunrpc/gss_api.h>
#include <linux/spinlock.h>

#include <trace/events/sunrpc.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define RPC_CREDCACHE_DEFAULT_HASHBITS	(4)
struct rpc_cred_cache {
	struct hlist_head	*hashtable;
	unsigned int		hashbits;
	spinlock_t		lock;
};

static unsigned int auth_hashbits = RPC_CREDCACHE_DEFAULT_HASHBITS;

<<<<<<< HEAD
static DEFINE_SPINLOCK(rpc_authflavor_lock);
static const struct rpc_authops *auth_flavors[RPC_AUTH_MAXFLAVOR] = {
	&authnull_ops,		/* AUTH_NULL */
	&authunix_ops,		/* AUTH_UNIX */
	NULL,			/* others can be loadable modules */
=======
static const struct rpc_authops __rcu *auth_flavors[RPC_AUTH_MAXFLAVOR] = {
	[RPC_AUTH_NULL] = (const struct rpc_authops __force __rcu *)&authnull_ops,
	[RPC_AUTH_UNIX] = (const struct rpc_authops __force __rcu *)&authunix_ops,
	[RPC_AUTH_TLS]  = (const struct rpc_authops __force __rcu *)&authtls_ops,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static LIST_HEAD(cred_unused);
static unsigned long number_cred_unused;

<<<<<<< HEAD
=======
static struct cred machine_cred = {
	.usage = ATOMIC_INIT(1),
};

/*
 * Return the machine_cred pointer to be used whenever
 * the a generic machine credential is needed.
 */
const struct cred *rpc_machine_cred(void)
{
	return &machine_cred;
}
EXPORT_SYMBOL_GPL(rpc_machine_cred);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define MAX_HASHTABLE_BITS (14)
static int param_set_hashtbl_sz(const char *val, const struct kernel_param *kp)
{
	unsigned long num;
	unsigned int nbits;
	int ret;

	if (!val)
		goto out_inval;
<<<<<<< HEAD
	ret = strict_strtoul(val, 0, &num);
	if (ret == -EINVAL)
		goto out_inval;
	nbits = fls(num);
	if (num > (1U << nbits))
		nbits++;
=======
	ret = kstrtoul(val, 0, &num);
	if (ret)
		goto out_inval;
	nbits = fls(num - 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (nbits > MAX_HASHTABLE_BITS || nbits < 2)
		goto out_inval;
	*(unsigned int *)kp->arg = nbits;
	return 0;
out_inval:
	return -EINVAL;
}

static int param_get_hashtbl_sz(char *buffer, const struct kernel_param *kp)
{
	unsigned int nbits;

	nbits = *(unsigned int *)kp->arg;
<<<<<<< HEAD
	return sprintf(buffer, "%u", 1U << nbits);
=======
	return sprintf(buffer, "%u\n", 1U << nbits);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#define param_check_hashtbl_sz(name, p) __param_check(name, p, unsigned int);

<<<<<<< HEAD
static struct kernel_param_ops param_ops_hashtbl_sz = {
=======
static const struct kernel_param_ops param_ops_hashtbl_sz = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.set = param_set_hashtbl_sz,
	.get = param_get_hashtbl_sz,
};

module_param_named(auth_hashtable_size, auth_hashbits, hashtbl_sz, 0644);
MODULE_PARM_DESC(auth_hashtable_size, "RPC credential cache hashtable size");

<<<<<<< HEAD
static u32
pseudoflavor_to_flavor(u32 flavor) {
	if (flavor >= RPC_AUTH_MAXFLAVOR)
=======
static unsigned long auth_max_cred_cachesize = ULONG_MAX;
module_param(auth_max_cred_cachesize, ulong, 0644);
MODULE_PARM_DESC(auth_max_cred_cachesize, "RPC credential maximum total cache size");

static u32
pseudoflavor_to_flavor(u32 flavor) {
	if (flavor > RPC_AUTH_MAXFLAVOR)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return RPC_AUTH_GSS;
	return flavor;
}

int
rpcauth_register(const struct rpc_authops *ops)
{
<<<<<<< HEAD
	rpc_authflavor_t flavor;
	int ret = -EPERM;

	if ((flavor = ops->au_flavor) >= RPC_AUTH_MAXFLAVOR)
		return -EINVAL;
	spin_lock(&rpc_authflavor_lock);
	if (auth_flavors[flavor] == NULL) {
		auth_flavors[flavor] = ops;
		ret = 0;
	}
	spin_unlock(&rpc_authflavor_lock);
	return ret;
=======
	const struct rpc_authops *old;
	rpc_authflavor_t flavor;

	if ((flavor = ops->au_flavor) >= RPC_AUTH_MAXFLAVOR)
		return -EINVAL;
	old = cmpxchg((const struct rpc_authops ** __force)&auth_flavors[flavor], NULL, ops);
	if (old == NULL || old == ops)
		return 0;
	return -EPERM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(rpcauth_register);

int
rpcauth_unregister(const struct rpc_authops *ops)
{
<<<<<<< HEAD
	rpc_authflavor_t flavor;
	int ret = -EPERM;

	if ((flavor = ops->au_flavor) >= RPC_AUTH_MAXFLAVOR)
		return -EINVAL;
	spin_lock(&rpc_authflavor_lock);
	if (auth_flavors[flavor] == ops) {
		auth_flavors[flavor] = NULL;
		ret = 0;
	}
	spin_unlock(&rpc_authflavor_lock);
	return ret;
}
EXPORT_SYMBOL_GPL(rpcauth_unregister);

struct rpc_auth *
rpcauth_create(rpc_authflavor_t pseudoflavor, struct rpc_clnt *clnt)
{
	struct rpc_auth		*auth;
	const struct rpc_authops *ops;
	u32			flavor = pseudoflavor_to_flavor(pseudoflavor);

	auth = ERR_PTR(-EINVAL);
	if (flavor >= RPC_AUTH_MAXFLAVOR)
		goto out;

	if ((ops = auth_flavors[flavor]) == NULL)
		request_module("rpc-auth-%u", flavor);
	spin_lock(&rpc_authflavor_lock);
	ops = auth_flavors[flavor];
	if (ops == NULL || !try_module_get(ops->owner)) {
		spin_unlock(&rpc_authflavor_lock);
		goto out;
	}
	spin_unlock(&rpc_authflavor_lock);
	auth = ops->create(clnt, pseudoflavor);
	module_put(ops->owner);
=======
	const struct rpc_authops *old;
	rpc_authflavor_t flavor;

	if ((flavor = ops->au_flavor) >= RPC_AUTH_MAXFLAVOR)
		return -EINVAL;

	old = cmpxchg((const struct rpc_authops ** __force)&auth_flavors[flavor], ops, NULL);
	if (old == ops || old == NULL)
		return 0;
	return -EPERM;
}
EXPORT_SYMBOL_GPL(rpcauth_unregister);

static const struct rpc_authops *
rpcauth_get_authops(rpc_authflavor_t flavor)
{
	const struct rpc_authops *ops;

	if (flavor >= RPC_AUTH_MAXFLAVOR)
		return NULL;

	rcu_read_lock();
	ops = rcu_dereference(auth_flavors[flavor]);
	if (ops == NULL) {
		rcu_read_unlock();
		request_module("rpc-auth-%u", flavor);
		rcu_read_lock();
		ops = rcu_dereference(auth_flavors[flavor]);
		if (ops == NULL)
			goto out;
	}
	if (!try_module_get(ops->owner))
		ops = NULL;
out:
	rcu_read_unlock();
	return ops;
}

static void
rpcauth_put_authops(const struct rpc_authops *ops)
{
	module_put(ops->owner);
}

/**
 * rpcauth_get_pseudoflavor - check if security flavor is supported
 * @flavor: a security flavor
 * @info: a GSS mech OID, quality of protection, and service value
 *
 * Verifies that an appropriate kernel module is available or already loaded.
 * Returns an equivalent pseudoflavor, or RPC_AUTH_MAXFLAVOR if "flavor" is
 * not supported locally.
 */
rpc_authflavor_t
rpcauth_get_pseudoflavor(rpc_authflavor_t flavor, struct rpcsec_gss_info *info)
{
	const struct rpc_authops *ops = rpcauth_get_authops(flavor);
	rpc_authflavor_t pseudoflavor;

	if (!ops)
		return RPC_AUTH_MAXFLAVOR;
	pseudoflavor = flavor;
	if (ops->info2flavor != NULL)
		pseudoflavor = ops->info2flavor(info);

	rpcauth_put_authops(ops);
	return pseudoflavor;
}
EXPORT_SYMBOL_GPL(rpcauth_get_pseudoflavor);

/**
 * rpcauth_get_gssinfo - find GSS tuple matching a GSS pseudoflavor
 * @pseudoflavor: GSS pseudoflavor to match
 * @info: rpcsec_gss_info structure to fill in
 *
 * Returns zero and fills in "info" if pseudoflavor matches a
 * supported mechanism.
 */
int
rpcauth_get_gssinfo(rpc_authflavor_t pseudoflavor, struct rpcsec_gss_info *info)
{
	rpc_authflavor_t flavor = pseudoflavor_to_flavor(pseudoflavor);
	const struct rpc_authops *ops;
	int result;

	ops = rpcauth_get_authops(flavor);
	if (ops == NULL)
		return -ENOENT;

	result = -ENOENT;
	if (ops->flavor2info != NULL)
		result = ops->flavor2info(pseudoflavor, info);

	rpcauth_put_authops(ops);
	return result;
}
EXPORT_SYMBOL_GPL(rpcauth_get_gssinfo);

struct rpc_auth *
rpcauth_create(const struct rpc_auth_create_args *args, struct rpc_clnt *clnt)
{
	struct rpc_auth	*auth = ERR_PTR(-EINVAL);
	const struct rpc_authops *ops;
	u32 flavor = pseudoflavor_to_flavor(args->pseudoflavor);

	ops = rpcauth_get_authops(flavor);
	if (ops == NULL)
		goto out;

	auth = ops->create(args, clnt);

	rpcauth_put_authops(ops);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (IS_ERR(auth))
		return auth;
	if (clnt->cl_auth)
		rpcauth_release(clnt->cl_auth);
	clnt->cl_auth = auth;

out:
	return auth;
}
EXPORT_SYMBOL_GPL(rpcauth_create);

void
rpcauth_release(struct rpc_auth *auth)
{
<<<<<<< HEAD
	if (!atomic_dec_and_test(&auth->au_count))
=======
	if (!refcount_dec_and_test(&auth->au_count))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;
	auth->au_ops->destroy(auth);
}

static DEFINE_SPINLOCK(rpc_credcache_lock);

<<<<<<< HEAD
static void
rpcauth_unhash_cred_locked(struct rpc_cred *cred)
{
	hlist_del_rcu(&cred->cr_hash);
	smp_mb__before_clear_bit();
	clear_bit(RPCAUTH_CRED_HASHED, &cred->cr_flags);
}

static int
rpcauth_unhash_cred(struct rpc_cred *cred)
{
	spinlock_t *cache_lock;
	int ret;

	cache_lock = &cred->cr_auth->au_credcache->lock;
	spin_lock(cache_lock);
	ret = atomic_read(&cred->cr_count) == 0;
	if (ret)
		rpcauth_unhash_cred_locked(cred);
=======
/*
 * On success, the caller is responsible for freeing the reference
 * held by the hashtable
 */
static bool
rpcauth_unhash_cred_locked(struct rpc_cred *cred)
{
	if (!test_and_clear_bit(RPCAUTH_CRED_HASHED, &cred->cr_flags))
		return false;
	hlist_del_rcu(&cred->cr_hash);
	return true;
}

static bool
rpcauth_unhash_cred(struct rpc_cred *cred)
{
	spinlock_t *cache_lock;
	bool ret;

	if (!test_bit(RPCAUTH_CRED_HASHED, &cred->cr_flags))
		return false;
	cache_lock = &cred->cr_auth->au_credcache->lock;
	spin_lock(cache_lock);
	ret = rpcauth_unhash_cred_locked(cred);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_unlock(cache_lock);
	return ret;
}

/*
 * Initialize RPC credential cache
 */
int
rpcauth_init_credcache(struct rpc_auth *auth)
{
	struct rpc_cred_cache *new;
	unsigned int hashsize;

	new = kmalloc(sizeof(*new), GFP_KERNEL);
	if (!new)
		goto out_nocache;
	new->hashbits = auth_hashbits;
	hashsize = 1U << new->hashbits;
	new->hashtable = kcalloc(hashsize, sizeof(new->hashtable[0]), GFP_KERNEL);
	if (!new->hashtable)
		goto out_nohashtbl;
	spin_lock_init(&new->lock);
	auth->au_credcache = new;
	return 0;
out_nohashtbl:
	kfree(new);
out_nocache:
	return -ENOMEM;
}
EXPORT_SYMBOL_GPL(rpcauth_init_credcache);

<<<<<<< HEAD
=======
char *
rpcauth_stringify_acceptor(struct rpc_cred *cred)
{
	if (!cred->cr_ops->crstringify_acceptor)
		return NULL;
	return cred->cr_ops->crstringify_acceptor(cred);
}
EXPORT_SYMBOL_GPL(rpcauth_stringify_acceptor);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Destroy a list of credentials
 */
static inline
void rpcauth_destroy_credlist(struct list_head *head)
{
	struct rpc_cred *cred;

	while (!list_empty(head)) {
		cred = list_entry(head->next, struct rpc_cred, cr_lru);
		list_del_init(&cred->cr_lru);
		put_rpccred(cred);
	}
}

<<<<<<< HEAD
=======
static void
rpcauth_lru_add_locked(struct rpc_cred *cred)
{
	if (!list_empty(&cred->cr_lru))
		return;
	number_cred_unused++;
	list_add_tail(&cred->cr_lru, &cred_unused);
}

static void
rpcauth_lru_add(struct rpc_cred *cred)
{
	if (!list_empty(&cred->cr_lru))
		return;
	spin_lock(&rpc_credcache_lock);
	rpcauth_lru_add_locked(cred);
	spin_unlock(&rpc_credcache_lock);
}

static void
rpcauth_lru_remove_locked(struct rpc_cred *cred)
{
	if (list_empty(&cred->cr_lru))
		return;
	number_cred_unused--;
	list_del_init(&cred->cr_lru);
}

static void
rpcauth_lru_remove(struct rpc_cred *cred)
{
	if (list_empty(&cred->cr_lru))
		return;
	spin_lock(&rpc_credcache_lock);
	rpcauth_lru_remove_locked(cred);
	spin_unlock(&rpc_credcache_lock);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Clear the RPC credential cache, and delete those credentials
 * that are not referenced.
 */
void
rpcauth_clear_credcache(struct rpc_cred_cache *cache)
{
	LIST_HEAD(free);
	struct hlist_head *head;
	struct rpc_cred	*cred;
	unsigned int hashsize = 1U << cache->hashbits;
	int		i;

	spin_lock(&rpc_credcache_lock);
	spin_lock(&cache->lock);
	for (i = 0; i < hashsize; i++) {
		head = &cache->hashtable[i];
		while (!hlist_empty(head)) {
			cred = hlist_entry(head->first, struct rpc_cred, cr_hash);
<<<<<<< HEAD
			get_rpccred(cred);
			if (!list_empty(&cred->cr_lru)) {
				list_del(&cred->cr_lru);
				number_cred_unused--;
			}
			list_add_tail(&cred->cr_lru, &free);
			rpcauth_unhash_cred_locked(cred);
=======
			rpcauth_unhash_cred_locked(cred);
			/* Note: We now hold a reference to cred */
			rpcauth_lru_remove_locked(cred);
			list_add_tail(&cred->cr_lru, &free);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}
	spin_unlock(&cache->lock);
	spin_unlock(&rpc_credcache_lock);
	rpcauth_destroy_credlist(&free);
}

/*
 * Destroy the RPC credential cache
 */
void
rpcauth_destroy_credcache(struct rpc_auth *auth)
{
	struct rpc_cred_cache *cache = auth->au_credcache;

	if (cache) {
		auth->au_credcache = NULL;
		rpcauth_clear_credcache(cache);
		kfree(cache->hashtable);
		kfree(cache);
	}
}
EXPORT_SYMBOL_GPL(rpcauth_destroy_credcache);


#define RPC_AUTH_EXPIRY_MORATORIUM (60 * HZ)

/*
 * Remove stale credentials. Avoid sleeping inside the loop.
 */
<<<<<<< HEAD
static int
rpcauth_prune_expired(struct list_head *free, int nr_to_scan)
{
	spinlock_t *cache_lock;
	struct rpc_cred *cred, *next;
	unsigned long expired = jiffies - RPC_AUTH_EXPIRY_MORATORIUM;
=======
static long
rpcauth_prune_expired(struct list_head *free, int nr_to_scan)
{
	struct rpc_cred *cred, *next;
	unsigned long expired = jiffies - RPC_AUTH_EXPIRY_MORATORIUM;
	long freed = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	list_for_each_entry_safe(cred, next, &cred_unused, cr_lru) {

		if (nr_to_scan-- == 0)
			break;
<<<<<<< HEAD
=======
		if (refcount_read(&cred->cr_count) > 1) {
			rpcauth_lru_remove_locked(cred);
			continue;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * Enforce a 60 second garbage collection moratorium
		 * Note that the cred_unused list must be time-ordered.
		 */
<<<<<<< HEAD
		if (time_in_range(cred->cr_expire, expired, jiffies) &&
		    test_bit(RPCAUTH_CRED_HASHED, &cred->cr_flags) != 0)
			return 0;

		list_del_init(&cred->cr_lru);
		number_cred_unused--;
		if (atomic_read(&cred->cr_count) != 0)
			continue;

		cache_lock = &cred->cr_auth->au_credcache->lock;
		spin_lock(cache_lock);
		if (atomic_read(&cred->cr_count) == 0) {
			get_rpccred(cred);
			list_add_tail(&cred->cr_lru, free);
			rpcauth_unhash_cred_locked(cred);
		}
		spin_unlock(cache_lock);
	}
	return (number_cred_unused / 100) * sysctl_vfs_cache_pressure;
=======
		if (time_in_range(cred->cr_expire, expired, jiffies))
			continue;
		if (!rpcauth_unhash_cred(cred))
			continue;

		rpcauth_lru_remove_locked(cred);
		freed++;
		list_add_tail(&cred->cr_lru, free);
	}
	return freed ? freed : SHRINK_STOP;
}

static unsigned long
rpcauth_cache_do_shrink(int nr_to_scan)
{
	LIST_HEAD(free);
	unsigned long freed;

	spin_lock(&rpc_credcache_lock);
	freed = rpcauth_prune_expired(&free, nr_to_scan);
	spin_unlock(&rpc_credcache_lock);
	rpcauth_destroy_credlist(&free);

	return freed;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Run memory cache shrinker.
 */
<<<<<<< HEAD
static int
rpcauth_cache_shrinker(struct shrinker *shrink, struct shrink_control *sc)
{
	LIST_HEAD(free);
	int res;
	int nr_to_scan = sc->nr_to_scan;
	gfp_t gfp_mask = sc->gfp_mask;

	if ((gfp_mask & GFP_KERNEL) != GFP_KERNEL)
		return (nr_to_scan == 0) ? 0 : -1;
	if (list_empty(&cred_unused))
		return 0;
	spin_lock(&rpc_credcache_lock);
	res = rpcauth_prune_expired(&free, nr_to_scan);
	spin_unlock(&rpc_credcache_lock);
	rpcauth_destroy_credlist(&free);
	return res;
=======
static unsigned long
rpcauth_cache_shrink_scan(struct shrinker *shrink, struct shrink_control *sc)

{
	if ((sc->gfp_mask & GFP_KERNEL) != GFP_KERNEL)
		return SHRINK_STOP;

	/* nothing left, don't come back */
	if (list_empty(&cred_unused))
		return SHRINK_STOP;

	return rpcauth_cache_do_shrink(sc->nr_to_scan);
}

static unsigned long
rpcauth_cache_shrink_count(struct shrinker *shrink, struct shrink_control *sc)

{
	return number_cred_unused * sysctl_vfs_cache_pressure / 100;
}

static void
rpcauth_cache_enforce_limit(void)
{
	unsigned long diff;
	unsigned int nr_to_scan;

	if (number_cred_unused <= auth_max_cred_cachesize)
		return;
	diff = number_cred_unused - auth_max_cred_cachesize;
	nr_to_scan = 100;
	if (diff < nr_to_scan)
		nr_to_scan = diff;
	rpcauth_cache_do_shrink(nr_to_scan);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Look up a process' credentials in the authentication cache
 */
struct rpc_cred *
rpcauth_lookup_credcache(struct rpc_auth *auth, struct auth_cred * acred,
<<<<<<< HEAD
		int flags)
{
	LIST_HEAD(free);
	struct rpc_cred_cache *cache = auth->au_credcache;
	struct hlist_node *pos;
=======
		int flags, gfp_t gfp)
{
	LIST_HEAD(free);
	struct rpc_cred_cache *cache = auth->au_credcache;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct rpc_cred	*cred = NULL,
			*entry, *new;
	unsigned int nr;

<<<<<<< HEAD
	nr = hash_long(acred->uid, cache->hashbits);

	rcu_read_lock();
	hlist_for_each_entry_rcu(entry, pos, &cache->hashtable[nr], cr_hash) {
		if (!entry->cr_ops->crmatch(acred, entry, flags))
			continue;
		spin_lock(&cache->lock);
		if (test_bit(RPCAUTH_CRED_HASHED, &entry->cr_flags) == 0) {
			spin_unlock(&cache->lock);
			continue;
		}
		cred = get_rpccred(entry);
		spin_unlock(&cache->lock);
		break;
=======
	nr = auth->au_ops->hash_cred(acred, cache->hashbits);

	rcu_read_lock();
	hlist_for_each_entry_rcu(entry, &cache->hashtable[nr], cr_hash) {
		if (!entry->cr_ops->crmatch(acred, entry, flags))
			continue;
		cred = get_rpccred(entry);
		if (cred)
			break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	rcu_read_unlock();

	if (cred != NULL)
		goto found;

<<<<<<< HEAD
	new = auth->au_ops->crcreate(auth, acred, flags);
=======
	new = auth->au_ops->crcreate(auth, acred, flags, gfp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (IS_ERR(new)) {
		cred = new;
		goto out;
	}

	spin_lock(&cache->lock);
<<<<<<< HEAD
	hlist_for_each_entry(entry, pos, &cache->hashtable[nr], cr_hash) {
		if (!entry->cr_ops->crmatch(acred, entry, flags))
			continue;
		cred = get_rpccred(entry);
		break;
=======
	hlist_for_each_entry(entry, &cache->hashtable[nr], cr_hash) {
		if (!entry->cr_ops->crmatch(acred, entry, flags))
			continue;
		cred = get_rpccred(entry);
		if (cred)
			break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	if (cred == NULL) {
		cred = new;
		set_bit(RPCAUTH_CRED_HASHED, &cred->cr_flags);
<<<<<<< HEAD
=======
		refcount_inc(&cred->cr_count);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		hlist_add_head_rcu(&cred->cr_hash, &cache->hashtable[nr]);
	} else
		list_add_tail(&new->cr_lru, &free);
	spin_unlock(&cache->lock);
<<<<<<< HEAD
=======
	rpcauth_cache_enforce_limit();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
found:
	if (test_bit(RPCAUTH_CRED_NEW, &cred->cr_flags) &&
	    cred->cr_ops->cr_init != NULL &&
	    !(flags & RPCAUTH_LOOKUP_NEW)) {
		int res = cred->cr_ops->cr_init(auth, cred);
		if (res < 0) {
			put_rpccred(cred);
			cred = ERR_PTR(res);
		}
	}
	rpcauth_destroy_credlist(&free);
out:
	return cred;
}
EXPORT_SYMBOL_GPL(rpcauth_lookup_credcache);

struct rpc_cred *
rpcauth_lookupcred(struct rpc_auth *auth, int flags)
{
	struct auth_cred acred;
	struct rpc_cred *ret;
	const struct cred *cred = current_cred();

<<<<<<< HEAD
	dprintk("RPC:       looking up %s cred\n",
		auth->au_ops->au_name);

	memset(&acred, 0, sizeof(acred));
	acred.uid = cred->fsuid;
	acred.gid = cred->fsgid;
	acred.group_info = get_group_info(((struct cred *)cred)->group_info);

	ret = auth->au_ops->lookup_cred(auth, &acred, flags);
	put_group_info(acred.group_info);
	return ret;
}
=======
	memset(&acred, 0, sizeof(acred));
	acred.cred = cred;
	ret = auth->au_ops->lookup_cred(auth, &acred, flags);
	return ret;
}
EXPORT_SYMBOL_GPL(rpcauth_lookupcred);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

void
rpcauth_init_cred(struct rpc_cred *cred, const struct auth_cred *acred,
		  struct rpc_auth *auth, const struct rpc_credops *ops)
{
	INIT_HLIST_NODE(&cred->cr_hash);
	INIT_LIST_HEAD(&cred->cr_lru);
<<<<<<< HEAD
	atomic_set(&cred->cr_count, 1);
	cred->cr_auth = auth;
	cred->cr_ops = ops;
	cred->cr_expire = jiffies;
#ifdef RPC_DEBUG
	cred->cr_magic = RPCAUTH_CRED_MAGIC;
#endif
	cred->cr_uid = acred->uid;
}
EXPORT_SYMBOL_GPL(rpcauth_init_cred);

struct rpc_cred *
rpcauth_generic_bind_cred(struct rpc_task *task, struct rpc_cred *cred, int lookupflags)
{
	dprintk("RPC: %5u holding %s cred %p\n", task->tk_pid,
			cred->cr_auth->au_ops->au_name, cred);
	return get_rpccred(cred);
}
EXPORT_SYMBOL_GPL(rpcauth_generic_bind_cred);

=======
	refcount_set(&cred->cr_count, 1);
	cred->cr_auth = auth;
	cred->cr_flags = 0;
	cred->cr_ops = ops;
	cred->cr_expire = jiffies;
	cred->cr_cred = get_cred(acred->cred);
}
EXPORT_SYMBOL_GPL(rpcauth_init_cred);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct rpc_cred *
rpcauth_bind_root_cred(struct rpc_task *task, int lookupflags)
{
	struct rpc_auth *auth = task->tk_client->cl_auth;
	struct auth_cred acred = {
<<<<<<< HEAD
		.uid = 0,
		.gid = 0,
	};

	dprintk("RPC: %5u looking up %s cred\n",
		task->tk_pid, task->tk_client->cl_auth->au_ops->au_name);
=======
		.cred = get_task_cred(&init_task),
	};
	struct rpc_cred *ret;

	if (RPC_IS_ASYNC(task))
		lookupflags |= RPCAUTH_LOOKUP_ASYNC;
	ret = auth->au_ops->lookup_cred(auth, &acred, lookupflags);
	put_cred(acred.cred);
	return ret;
}

static struct rpc_cred *
rpcauth_bind_machine_cred(struct rpc_task *task, int lookupflags)
{
	struct rpc_auth *auth = task->tk_client->cl_auth;
	struct auth_cred acred = {
		.principal = task->tk_client->cl_principal,
		.cred = init_task.cred,
	};

	if (!acred.principal)
		return NULL;
	if (RPC_IS_ASYNC(task))
		lookupflags |= RPCAUTH_LOOKUP_ASYNC;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return auth->au_ops->lookup_cred(auth, &acred, lookupflags);
}

static struct rpc_cred *
rpcauth_bind_new_cred(struct rpc_task *task, int lookupflags)
{
	struct rpc_auth *auth = task->tk_client->cl_auth;

<<<<<<< HEAD
	dprintk("RPC: %5u looking up %s cred\n",
		task->tk_pid, auth->au_ops->au_name);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return rpcauth_lookupcred(auth, lookupflags);
}

static int
<<<<<<< HEAD
rpcauth_bindcred(struct rpc_task *task, struct rpc_cred *cred, int flags)
{
	struct rpc_rqst *req = task->tk_rqstp;
	struct rpc_cred *new;
	int lookupflags = 0;

	if (flags & RPC_TASK_ASYNC)
		lookupflags |= RPCAUTH_LOOKUP_NEW;
	if (cred != NULL)
		new = cred->cr_ops->crbind(task, cred, lookupflags);
	else if (flags & RPC_TASK_ROOTCREDS)
		new = rpcauth_bind_root_cred(task, lookupflags);
=======
rpcauth_bindcred(struct rpc_task *task, const struct cred *cred, int flags)
{
	struct rpc_rqst *req = task->tk_rqstp;
	struct rpc_cred *new = NULL;
	int lookupflags = 0;
	struct rpc_auth *auth = task->tk_client->cl_auth;
	struct auth_cred acred = {
		.cred = cred,
	};

	if (flags & RPC_TASK_ASYNC)
		lookupflags |= RPCAUTH_LOOKUP_NEW | RPCAUTH_LOOKUP_ASYNC;
	if (task->tk_op_cred)
		/* Task must use exactly this rpc_cred */
		new = get_rpccred(task->tk_op_cred);
	else if (cred != NULL && cred != &machine_cred)
		new = auth->au_ops->lookup_cred(auth, &acred, lookupflags);
	else if (cred == &machine_cred)
		new = rpcauth_bind_machine_cred(task, lookupflags);

	/* If machine cred couldn't be bound, try a root cred */
	if (new)
		;
	else if (cred == &machine_cred)
		new = rpcauth_bind_root_cred(task, lookupflags);
	else if (flags & RPC_TASK_NULLCREDS)
		new = authnull_ops.lookup_cred(NULL, NULL, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	else
		new = rpcauth_bind_new_cred(task, lookupflags);
	if (IS_ERR(new))
		return PTR_ERR(new);
<<<<<<< HEAD
	if (req->rq_cred != NULL)
		put_rpccred(req->rq_cred);
=======
	put_rpccred(req->rq_cred);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	req->rq_cred = new;
	return 0;
}

void
put_rpccred(struct rpc_cred *cred)
{
<<<<<<< HEAD
	/* Fast path for unhashed credentials */
	if (test_bit(RPCAUTH_CRED_HASHED, &cred->cr_flags) == 0) {
		if (atomic_dec_and_test(&cred->cr_count))
			cred->cr_ops->crdestroy(cred);
		return;
	}

	if (!atomic_dec_and_lock(&cred->cr_count, &rpc_credcache_lock))
		return;
	if (!list_empty(&cred->cr_lru)) {
		number_cred_unused--;
		list_del_init(&cred->cr_lru);
	}
	if (test_bit(RPCAUTH_CRED_HASHED, &cred->cr_flags) != 0) {
		if (test_bit(RPCAUTH_CRED_UPTODATE, &cred->cr_flags) != 0) {
			cred->cr_expire = jiffies;
			list_add_tail(&cred->cr_lru, &cred_unused);
			number_cred_unused++;
			goto out_nodestroy;
		}
		if (!rpcauth_unhash_cred(cred)) {
			/* We were hashed and someone looked us up... */
			goto out_nodestroy;
		}
	}
	spin_unlock(&rpc_credcache_lock);
	cred->cr_ops->crdestroy(cred);
	return;
out_nodestroy:
	spin_unlock(&rpc_credcache_lock);
}
EXPORT_SYMBOL_GPL(put_rpccred);

__be32 *
rpcauth_marshcred(struct rpc_task *task, __be32 *p)
{
	struct rpc_cred	*cred = task->tk_rqstp->rq_cred;

	dprintk("RPC: %5u marshaling %s cred %p\n",
		task->tk_pid, cred->cr_auth->au_ops->au_name, cred);

	return cred->cr_ops->crmarshal(task, p);
}

__be32 *
rpcauth_checkverf(struct rpc_task *task, __be32 *p)
{
	struct rpc_cred	*cred = task->tk_rqstp->rq_cred;

	dprintk("RPC: %5u validating %s cred %p\n",
		task->tk_pid, cred->cr_auth->au_ops->au_name, cred);

	return cred->cr_ops->crvalidate(task, p);
}

static void rpcauth_wrap_req_encode(kxdreproc_t encode, struct rpc_rqst *rqstp,
				   __be32 *data, void *obj)
{
	struct xdr_stream xdr;

	xdr_init_encode(&xdr, &rqstp->rq_snd_buf, data);
	encode(rqstp, &xdr, obj);
}

int
rpcauth_wrap_req(struct rpc_task *task, kxdreproc_t encode, void *rqstp,
		__be32 *data, void *obj)
{
	struct rpc_cred *cred = task->tk_rqstp->rq_cred;

	dprintk("RPC: %5u using %s cred %p to wrap rpc data\n",
			task->tk_pid, cred->cr_ops->cr_name, cred);
	if (cred->cr_ops->crwrap_req)
		return cred->cr_ops->crwrap_req(task, encode, rqstp, data, obj);
	/* By default, we encode the arguments normally. */
	rpcauth_wrap_req_encode(encode, rqstp, data, obj);
	return 0;
}

static int
rpcauth_unwrap_req_decode(kxdrdproc_t decode, struct rpc_rqst *rqstp,
			  __be32 *data, void *obj)
{
	struct xdr_stream xdr;

	xdr_init_decode(&xdr, &rqstp->rq_rcv_buf, data);
	return decode(rqstp, &xdr, obj);
}

int
rpcauth_unwrap_resp(struct rpc_task *task, kxdrdproc_t decode, void *rqstp,
		__be32 *data, void *obj)
{
	struct rpc_cred *cred = task->tk_rqstp->rq_cred;

	dprintk("RPC: %5u using %s cred %p to unwrap rpc data\n",
			task->tk_pid, cred->cr_ops->cr_name, cred);
	if (cred->cr_ops->crunwrap_resp)
		return cred->cr_ops->crunwrap_resp(task, decode, rqstp,
						   data, obj);
	/* By default, we decode the arguments normally. */
	return rpcauth_unwrap_req_decode(decode, rqstp, data, obj);
=======
	if (cred == NULL)
		return;
	rcu_read_lock();
	if (refcount_dec_and_test(&cred->cr_count))
		goto destroy;
	if (refcount_read(&cred->cr_count) != 1 ||
	    !test_bit(RPCAUTH_CRED_HASHED, &cred->cr_flags))
		goto out;
	if (test_bit(RPCAUTH_CRED_UPTODATE, &cred->cr_flags) != 0) {
		cred->cr_expire = jiffies;
		rpcauth_lru_add(cred);
		/* Race breaker */
		if (unlikely(!test_bit(RPCAUTH_CRED_HASHED, &cred->cr_flags)))
			rpcauth_lru_remove(cred);
	} else if (rpcauth_unhash_cred(cred)) {
		rpcauth_lru_remove(cred);
		if (refcount_dec_and_test(&cred->cr_count))
			goto destroy;
	}
out:
	rcu_read_unlock();
	return;
destroy:
	rcu_read_unlock();
	cred->cr_ops->crdestroy(cred);
}
EXPORT_SYMBOL_GPL(put_rpccred);

/**
 * rpcauth_marshcred - Append RPC credential to end of @xdr
 * @task: controlling RPC task
 * @xdr: xdr_stream containing initial portion of RPC Call header
 *
 * On success, an appropriate verifier is added to @xdr, @xdr is
 * updated to point past the verifier, and zero is returned.
 * Otherwise, @xdr is in an undefined state and a negative errno
 * is returned.
 */
int rpcauth_marshcred(struct rpc_task *task, struct xdr_stream *xdr)
{
	const struct rpc_credops *ops = task->tk_rqstp->rq_cred->cr_ops;

	return ops->crmarshal(task, xdr);
}

/**
 * rpcauth_wrap_req_encode - XDR encode the RPC procedure
 * @task: controlling RPC task
 * @xdr: stream where on-the-wire bytes are to be marshalled
 *
 * On success, @xdr contains the encoded and wrapped message.
 * Otherwise, @xdr is in an undefined state.
 */
int rpcauth_wrap_req_encode(struct rpc_task *task, struct xdr_stream *xdr)
{
	kxdreproc_t encode = task->tk_msg.rpc_proc->p_encode;

	encode(task->tk_rqstp, xdr, task->tk_msg.rpc_argp);
	return 0;
}
EXPORT_SYMBOL_GPL(rpcauth_wrap_req_encode);

/**
 * rpcauth_wrap_req - XDR encode and wrap the RPC procedure
 * @task: controlling RPC task
 * @xdr: stream where on-the-wire bytes are to be marshalled
 *
 * On success, @xdr contains the encoded and wrapped message,
 * and zero is returned. Otherwise, @xdr is in an undefined
 * state and a negative errno is returned.
 */
int rpcauth_wrap_req(struct rpc_task *task, struct xdr_stream *xdr)
{
	const struct rpc_credops *ops = task->tk_rqstp->rq_cred->cr_ops;

	return ops->crwrap_req(task, xdr);
}

/**
 * rpcauth_checkverf - Validate verifier in RPC Reply header
 * @task: controlling RPC task
 * @xdr: xdr_stream containing RPC Reply header
 *
 * Return values:
 *   %0: Verifier is valid. @xdr now points past the verifier.
 *   %-EIO: Verifier is corrupted or message ended early.
 *   %-EACCES: Verifier is intact but not valid.
 *   %-EPROTONOSUPPORT: Server does not support the requested auth type.
 *
 * When a negative errno is returned, @xdr is left in an undefined
 * state.
 */
int
rpcauth_checkverf(struct rpc_task *task, struct xdr_stream *xdr)
{
	const struct rpc_credops *ops = task->tk_rqstp->rq_cred->cr_ops;

	return ops->crvalidate(task, xdr);
}

/**
 * rpcauth_unwrap_resp_decode - Invoke XDR decode function
 * @task: controlling RPC task
 * @xdr: stream where the Reply message resides
 *
 * Returns zero on success; otherwise a negative errno is returned.
 */
int
rpcauth_unwrap_resp_decode(struct rpc_task *task, struct xdr_stream *xdr)
{
	kxdrdproc_t decode = task->tk_msg.rpc_proc->p_decode;

	return decode(task->tk_rqstp, xdr, task->tk_msg.rpc_resp);
}
EXPORT_SYMBOL_GPL(rpcauth_unwrap_resp_decode);

/**
 * rpcauth_unwrap_resp - Invoke unwrap and decode function for the cred
 * @task: controlling RPC task
 * @xdr: stream where the Reply message resides
 *
 * Returns zero on success; otherwise a negative errno is returned.
 */
int
rpcauth_unwrap_resp(struct rpc_task *task, struct xdr_stream *xdr)
{
	const struct rpc_credops *ops = task->tk_rqstp->rq_cred->cr_ops;

	return ops->crunwrap_resp(task, xdr);
}

bool
rpcauth_xmit_need_reencode(struct rpc_task *task)
{
	struct rpc_cred *cred = task->tk_rqstp->rq_cred;

	if (!cred || !cred->cr_ops->crneed_reencode)
		return false;
	return cred->cr_ops->crneed_reencode(task);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int
rpcauth_refreshcred(struct rpc_task *task)
{
	struct rpc_cred	*cred;
	int err;

	cred = task->tk_rqstp->rq_cred;
	if (cred == NULL) {
		err = rpcauth_bindcred(task, task->tk_msg.rpc_cred, task->tk_flags);
		if (err < 0)
			goto out;
		cred = task->tk_rqstp->rq_cred;
	}
<<<<<<< HEAD
	dprintk("RPC: %5u refreshing %s cred %p\n",
		task->tk_pid, cred->cr_auth->au_ops->au_name, cred);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	err = cred->cr_ops->crrefresh(task);
out:
	if (err < 0)
		task->tk_status = err;
	return err;
}

void
rpcauth_invalcred(struct rpc_task *task)
{
	struct rpc_cred *cred = task->tk_rqstp->rq_cred;

<<<<<<< HEAD
	dprintk("RPC: %5u invalidating %s cred %p\n",
		task->tk_pid, cred->cr_auth->au_ops->au_name, cred);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (cred)
		clear_bit(RPCAUTH_CRED_UPTODATE, &cred->cr_flags);
}

int
rpcauth_uptodatecred(struct rpc_task *task)
{
	struct rpc_cred *cred = task->tk_rqstp->rq_cred;

	return cred == NULL ||
		test_bit(RPCAUTH_CRED_UPTODATE, &cred->cr_flags) != 0;
}

<<<<<<< HEAD
static struct shrinker rpc_cred_shrinker = {
	.shrink = rpcauth_cache_shrinker,
	.seeks = DEFAULT_SEEKS,
};
=======
static struct shrinker *rpc_cred_shrinker;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

int __init rpcauth_init_module(void)
{
	int err;

	err = rpc_init_authunix();
	if (err < 0)
		goto out1;
<<<<<<< HEAD
	err = rpc_init_generic_auth();
	if (err < 0)
		goto out2;
	register_shrinker(&rpc_cred_shrinker);
=======
	rpc_cred_shrinker = shrinker_alloc(0, "sunrpc_cred");
	if (!rpc_cred_shrinker) {
		err = -ENOMEM;
		goto out2;
	}

	rpc_cred_shrinker->count_objects = rpcauth_cache_shrink_count;
	rpc_cred_shrinker->scan_objects = rpcauth_cache_shrink_scan;

	shrinker_register(rpc_cred_shrinker);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
out2:
	rpc_destroy_authunix();
out1:
	return err;
}

void rpcauth_remove_module(void)
{
	rpc_destroy_authunix();
<<<<<<< HEAD
	rpc_destroy_generic_auth();
	unregister_shrinker(&rpc_cred_shrinker);
=======
	shrinker_free(rpc_cred_shrinker);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
