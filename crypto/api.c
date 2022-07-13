<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Scatterlist Cryptographic API.
 *
 * Copyright (c) 2002 James Morris <jmorris@intercode.com.au>
 * Copyright (c) 2002 David S. Miller (davem@redhat.com)
 * Copyright (c) 2005 Herbert Xu <herbert@gondor.apana.org.au>
 *
 * Portions derived from Cryptoapi, by Alexander Kjeldaas <astor@fast.no>
 * and Nettle, by Niels Möller.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/err.h>
#include <linux/errno.h>
<<<<<<< HEAD
=======
#include <linux/jump_label.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/kernel.h>
#include <linux/kmod.h>
#include <linux/module.h>
#include <linux/param.h>
<<<<<<< HEAD
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/string.h>
=======
#include <linux/sched/signal.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/completion.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "internal.h"

LIST_HEAD(crypto_alg_list);
EXPORT_SYMBOL_GPL(crypto_alg_list);
DECLARE_RWSEM(crypto_alg_sem);
EXPORT_SYMBOL_GPL(crypto_alg_sem);

BLOCKING_NOTIFIER_HEAD(crypto_chain);
EXPORT_SYMBOL_GPL(crypto_chain);

<<<<<<< HEAD
static inline struct crypto_alg *crypto_alg_get(struct crypto_alg *alg)
{
	atomic_inc(&alg->cra_refcnt);
	return alg;
}
=======
#ifndef CONFIG_CRYPTO_MANAGER_DISABLE_TESTS
DEFINE_STATIC_KEY_FALSE(__crypto_boot_test_finished);
EXPORT_SYMBOL_GPL(__crypto_boot_test_finished);
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static struct crypto_alg *crypto_larval_wait(struct crypto_alg *alg);

struct crypto_alg *crypto_mod_get(struct crypto_alg *alg)
{
	return try_module_get(alg->cra_module) ? crypto_alg_get(alg) : NULL;
}
EXPORT_SYMBOL_GPL(crypto_mod_get);

void crypto_mod_put(struct crypto_alg *alg)
{
	struct module *module = alg->cra_module;

	crypto_alg_put(alg);
	module_put(module);
}
EXPORT_SYMBOL_GPL(crypto_mod_put);

<<<<<<< HEAD
static inline int crypto_is_test_larval(struct crypto_larval *larval)
{
	return larval->alg.cra_driver_name[0];
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct crypto_alg *__crypto_alg_lookup(const char *name, u32 type,
					      u32 mask)
{
	struct crypto_alg *q, *alg = NULL;
	int best = -2;

	list_for_each_entry(q, &crypto_alg_list, cra_list) {
		int exact, fuzzy;

		if (crypto_is_moribund(q))
			continue;

		if ((q->cra_flags ^ type) & mask)
			continue;

		if (crypto_is_larval(q) &&
		    !crypto_is_test_larval((struct crypto_larval *)q) &&
		    ((struct crypto_larval *)q)->mask != mask)
			continue;

		exact = !strcmp(q->cra_driver_name, name);
		fuzzy = !strcmp(q->cra_name, name);
		if (!exact && !(fuzzy && q->cra_priority > best))
			continue;

		if (unlikely(!crypto_mod_get(q)))
			continue;

		best = q->cra_priority;
		if (alg)
			crypto_mod_put(alg);
		alg = q;

		if (exact)
			break;
	}

	return alg;
}

static void crypto_larval_destroy(struct crypto_alg *alg)
{
	struct crypto_larval *larval = (void *)alg;

	BUG_ON(!crypto_is_larval(alg));
<<<<<<< HEAD
	if (larval->adult)
=======
	if (!IS_ERR_OR_NULL(larval->adult))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		crypto_mod_put(larval->adult);
	kfree(larval);
}

struct crypto_larval *crypto_larval_alloc(const char *name, u32 type, u32 mask)
{
	struct crypto_larval *larval;

	larval = kzalloc(sizeof(*larval), GFP_KERNEL);
	if (!larval)
		return ERR_PTR(-ENOMEM);

	larval->mask = mask;
	larval->alg.cra_flags = CRYPTO_ALG_LARVAL | type;
	larval->alg.cra_priority = -1;
	larval->alg.cra_destroy = crypto_larval_destroy;

<<<<<<< HEAD
	strlcpy(larval->alg.cra_name, name, CRYPTO_MAX_ALG_NAME);
=======
	strscpy(larval->alg.cra_name, name, CRYPTO_MAX_ALG_NAME);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	init_completion(&larval->completion);

	return larval;
}
EXPORT_SYMBOL_GPL(crypto_larval_alloc);

static struct crypto_alg *crypto_larval_add(const char *name, u32 type,
					    u32 mask)
{
	struct crypto_alg *alg;
	struct crypto_larval *larval;

	larval = crypto_larval_alloc(name, type, mask);
	if (IS_ERR(larval))
		return ERR_CAST(larval);

<<<<<<< HEAD
	atomic_set(&larval->alg.cra_refcnt, 2);
=======
	refcount_set(&larval->alg.cra_refcnt, 2);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	down_write(&crypto_alg_sem);
	alg = __crypto_alg_lookup(name, type, mask);
	if (!alg) {
		alg = &larval->alg;
		list_add(&alg->cra_list, &crypto_alg_list);
	}
	up_write(&crypto_alg_sem);

	if (alg != &larval->alg) {
		kfree(larval);
		if (crypto_is_larval(alg))
			alg = crypto_larval_wait(alg);
	}

	return alg;
}

void crypto_larval_kill(struct crypto_alg *alg)
{
	struct crypto_larval *larval = (void *)alg;

	down_write(&crypto_alg_sem);
	list_del(&alg->cra_list);
	up_write(&crypto_alg_sem);
	complete_all(&larval->completion);
	crypto_alg_put(alg);
}
EXPORT_SYMBOL_GPL(crypto_larval_kill);

<<<<<<< HEAD
=======
void crypto_wait_for_test(struct crypto_larval *larval)
{
	int err;

	err = crypto_probing_notify(CRYPTO_MSG_ALG_REGISTER, larval->adult);
	if (WARN_ON_ONCE(err != NOTIFY_STOP))
		goto out;

	err = wait_for_completion_killable(&larval->completion);
	WARN_ON(err);
out:
	crypto_larval_kill(&larval->alg);
}
EXPORT_SYMBOL_GPL(crypto_wait_for_test);

static void crypto_start_test(struct crypto_larval *larval)
{
	if (!crypto_is_test_larval(larval))
		return;

	if (larval->test_started)
		return;

	down_write(&crypto_alg_sem);
	if (larval->test_started) {
		up_write(&crypto_alg_sem);
		return;
	}

	larval->test_started = true;
	up_write(&crypto_alg_sem);

	crypto_wait_for_test(larval);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct crypto_alg *crypto_larval_wait(struct crypto_alg *alg)
{
	struct crypto_larval *larval = (void *)alg;
	long timeout;

<<<<<<< HEAD
=======
	if (!crypto_boot_test_finished())
		crypto_start_test(larval);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	timeout = wait_for_completion_killable_timeout(
		&larval->completion, 60 * HZ);

	alg = larval->adult;
	if (timeout < 0)
		alg = ERR_PTR(-EINTR);
	else if (!timeout)
		alg = ERR_PTR(-ETIMEDOUT);
	else if (!alg)
		alg = ERR_PTR(-ENOENT);
<<<<<<< HEAD
	else if (crypto_is_test_larval(larval) &&
		 !(alg->cra_flags & CRYPTO_ALG_TESTED))
		alg = ERR_PTR(-EAGAIN);
=======
	else if (IS_ERR(alg))
		;
	else if (crypto_is_test_larval(larval) &&
		 !(alg->cra_flags & CRYPTO_ALG_TESTED))
		alg = ERR_PTR(-EAGAIN);
	else if (alg->cra_flags & CRYPTO_ALG_FIPS_INTERNAL)
		alg = ERR_PTR(-EAGAIN);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	else if (!crypto_mod_get(alg))
		alg = ERR_PTR(-EAGAIN);
	crypto_mod_put(&larval->alg);

	return alg;
}

<<<<<<< HEAD
struct crypto_alg *crypto_alg_lookup(const char *name, u32 type, u32 mask)
{
	struct crypto_alg *alg;

	down_read(&crypto_alg_sem);
	alg = __crypto_alg_lookup(name, type, mask);
=======
static struct crypto_alg *crypto_alg_lookup(const char *name, u32 type,
					    u32 mask)
{
	const u32 fips = CRYPTO_ALG_FIPS_INTERNAL;
	struct crypto_alg *alg;
	u32 test = 0;

	if (!((type | mask) & CRYPTO_ALG_TESTED))
		test |= CRYPTO_ALG_TESTED;

	down_read(&crypto_alg_sem);
	alg = __crypto_alg_lookup(name, (type | test) & ~fips,
				  (mask | test) & ~fips);
	if (alg) {
		if (((type | mask) ^ fips) & fips)
			mask |= fips;
		mask &= fips;

		if (!crypto_is_larval(alg) &&
		    ((type ^ alg->cra_flags) & mask)) {
			/* Algorithm is disallowed in FIPS mode. */
			crypto_mod_put(alg);
			alg = ERR_PTR(-ENOENT);
		}
	} else if (test) {
		alg = __crypto_alg_lookup(name, type, mask);
		if (alg && !crypto_is_larval(alg)) {
			/* Test failed */
			crypto_mod_put(alg);
			alg = ERR_PTR(-ELIBBAD);
		}
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	up_read(&crypto_alg_sem);

	return alg;
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(crypto_alg_lookup);

struct crypto_alg *crypto_larval_lookup(const char *name, u32 type, u32 mask)
=======

static struct crypto_alg *crypto_larval_lookup(const char *name, u32 type,
					       u32 mask)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct crypto_alg *alg;

	if (!name)
		return ERR_PTR(-ENOENT);

<<<<<<< HEAD
	mask &= ~(CRYPTO_ALG_LARVAL | CRYPTO_ALG_DEAD);
	type &= mask;

	alg = crypto_alg_lookup(name, type, mask);
	if (!alg) {
		request_module("%s", name);

		if (!((type ^ CRYPTO_ALG_NEED_FALLBACK) & mask &
		      CRYPTO_ALG_NEED_FALLBACK))
			request_module("%s-all", name);
=======
	type &= ~(CRYPTO_ALG_LARVAL | CRYPTO_ALG_DEAD);
	mask &= ~(CRYPTO_ALG_LARVAL | CRYPTO_ALG_DEAD);

	alg = crypto_alg_lookup(name, type, mask);
	if (!alg && !(mask & CRYPTO_NOLOAD)) {
		request_module("crypto-%s", name);

		if (!((type ^ CRYPTO_ALG_NEED_FALLBACK) & mask &
		      CRYPTO_ALG_NEED_FALLBACK))
			request_module("crypto-%s-all", name);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		alg = crypto_alg_lookup(name, type, mask);
	}

<<<<<<< HEAD
	if (alg)
		return crypto_is_larval(alg) ? crypto_larval_wait(alg) : alg;

	return crypto_larval_add(name, type, mask);
}
EXPORT_SYMBOL_GPL(crypto_larval_lookup);
=======
	if (!IS_ERR_OR_NULL(alg) && crypto_is_larval(alg))
		alg = crypto_larval_wait(alg);
	else if (!alg)
		alg = crypto_larval_add(name, type, mask);

	return alg;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

int crypto_probing_notify(unsigned long val, void *v)
{
	int ok;

	ok = blocking_notifier_call_chain(&crypto_chain, val, v);
	if (ok == NOTIFY_DONE) {
		request_module("cryptomgr");
		ok = blocking_notifier_call_chain(&crypto_chain, val, v);
	}

	return ok;
}
EXPORT_SYMBOL_GPL(crypto_probing_notify);

struct crypto_alg *crypto_alg_mod_lookup(const char *name, u32 type, u32 mask)
{
	struct crypto_alg *alg;
	struct crypto_alg *larval;
	int ok;

<<<<<<< HEAD
	if (!((type | mask) & CRYPTO_ALG_TESTED)) {
		type |= CRYPTO_ALG_TESTED;
		mask |= CRYPTO_ALG_TESTED;
	}
=======
	/*
	 * If the internal flag is set for a cipher, require a caller to
	 * invoke the cipher with the internal flag to use that cipher.
	 * Also, if a caller wants to allocate a cipher that may or may
	 * not be an internal cipher, use type | CRYPTO_ALG_INTERNAL and
	 * !(mask & CRYPTO_ALG_INTERNAL).
	 */
	if (!((type | mask) & CRYPTO_ALG_INTERNAL))
		mask |= CRYPTO_ALG_INTERNAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	larval = crypto_larval_lookup(name, type, mask);
	if (IS_ERR(larval) || !crypto_is_larval(larval))
		return larval;

	ok = crypto_probing_notify(CRYPTO_MSG_ALG_REQUEST, larval);

	if (ok == NOTIFY_STOP)
		alg = crypto_larval_wait(larval);
	else {
		crypto_mod_put(larval);
		alg = ERR_PTR(-ENOENT);
	}
	crypto_larval_kill(larval);
	return alg;
}
EXPORT_SYMBOL_GPL(crypto_alg_mod_lookup);

<<<<<<< HEAD
static int crypto_init_ops(struct crypto_tfm *tfm, u32 type, u32 mask)
{
	const struct crypto_type *type_obj = tfm->__crt_alg->cra_type;

	if (type_obj)
		return type_obj->init(tfm, type, mask);

	switch (crypto_tfm_alg_type(tfm)) {
	case CRYPTO_ALG_TYPE_CIPHER:
		return crypto_init_cipher_ops(tfm);

	case CRYPTO_ALG_TYPE_COMPRESS:
		return crypto_init_compress_ops(tfm);

	default:
		break;
	}

	BUG();
	return -EINVAL;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void crypto_exit_ops(struct crypto_tfm *tfm)
{
	const struct crypto_type *type = tfm->__crt_alg->cra_type;

<<<<<<< HEAD
	if (type) {
		if (tfm->exit)
			tfm->exit(tfm);
		return;
	}

	switch (crypto_tfm_alg_type(tfm)) {
	case CRYPTO_ALG_TYPE_CIPHER:
		crypto_exit_cipher_ops(tfm);
		break;

	case CRYPTO_ALG_TYPE_COMPRESS:
		crypto_exit_compress_ops(tfm);
		break;

	default:
		BUG();
	}
=======
	if (type && tfm->exit)
		tfm->exit(tfm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static unsigned int crypto_ctxsize(struct crypto_alg *alg, u32 type, u32 mask)
{
	const struct crypto_type *type_obj = alg->cra_type;
	unsigned int len;

	len = alg->cra_alignmask & ~(crypto_tfm_ctx_alignment() - 1);
	if (type_obj)
		return len + type_obj->ctxsize(alg, type, mask);

	switch (alg->cra_flags & CRYPTO_ALG_TYPE_MASK) {
	default:
		BUG();

	case CRYPTO_ALG_TYPE_CIPHER:
		len += crypto_cipher_ctxsize(alg);
		break;

	case CRYPTO_ALG_TYPE_COMPRESS:
		len += crypto_compress_ctxsize(alg);
		break;
	}

	return len;
}

void crypto_shoot_alg(struct crypto_alg *alg)
{
	down_write(&crypto_alg_sem);
	alg->cra_flags |= CRYPTO_ALG_DYING;
	up_write(&crypto_alg_sem);
}
EXPORT_SYMBOL_GPL(crypto_shoot_alg);

<<<<<<< HEAD
#if FIPS_CRYPTO_TEST == 5
int g_tfm_sz = 0;
#endif
struct crypto_tfm *__crypto_alloc_tfm(struct crypto_alg *alg, u32 type,
				      u32 mask)
{
	struct crypto_tfm *tfm = NULL;
=======
struct crypto_tfm *__crypto_alloc_tfmgfp(struct crypto_alg *alg, u32 type,
					 u32 mask, gfp_t gfp)
{
	struct crypto_tfm *tfm;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned int tfm_size;
	int err = -ENOMEM;

	tfm_size = sizeof(*tfm) + crypto_ctxsize(alg, type, mask);
<<<<<<< HEAD
	tfm = kzalloc(tfm_size, GFP_KERNEL);
	if (tfm == NULL)
		goto out_err;

#if FIPS_CRYPTO_TEST == 5
    g_tfm_sz = tfm_size;
#endif
	tfm->__crt_alg = alg;

	err = crypto_init_ops(tfm, type, mask);
	if (err)
		goto out_free_tfm;
=======
	tfm = kzalloc(tfm_size, gfp);
	if (tfm == NULL)
		goto out_err;

	tfm->__crt_alg = alg;
	refcount_set(&tfm->refcnt, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!tfm->exit && alg->cra_init && (err = alg->cra_init(tfm)))
		goto cra_init_failed;

	goto out;

cra_init_failed:
	crypto_exit_ops(tfm);
<<<<<<< HEAD
out_free_tfm:
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err == -EAGAIN)
		crypto_shoot_alg(alg);
	kfree(tfm);
out_err:
	tfm = ERR_PTR(err);
out:
	return tfm;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(__crypto_alloc_tfmgfp);

struct crypto_tfm *__crypto_alloc_tfm(struct crypto_alg *alg, u32 type,
				      u32 mask)
{
	return __crypto_alloc_tfmgfp(alg, type, mask, GFP_KERNEL);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
EXPORT_SYMBOL_GPL(__crypto_alloc_tfm);

/*
 *	crypto_alloc_base - Locate algorithm and allocate transform
 *	@alg_name: Name of algorithm
 *	@type: Type of algorithm
 *	@mask: Mask for type comparison
 *
 *	This function should not be used by new algorithm types.
<<<<<<< HEAD
 *	Plesae use crypto_alloc_tfm instead.
=======
 *	Please use crypto_alloc_tfm instead.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 *	crypto_alloc_base() will first attempt to locate an already loaded
 *	algorithm.  If that fails and the kernel supports dynamically loadable
 *	modules, it will then attempt to load a module of the same name or
 *	alias.  If that fails it will send a query to any loaded crypto manager
 *	to construct an algorithm on the fly.  A refcount is grabbed on the
 *	algorithm which is then associated with the new transform.
 *
 *	The returned transform is of a non-determinate type.  Most people
 *	should use one of the more specific allocation functions such as
<<<<<<< HEAD
 *	crypto_alloc_blkcipher.
=======
 *	crypto_alloc_skcipher().
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 *	In case of error the return value is an error pointer.
 */
struct crypto_tfm *crypto_alloc_base(const char *alg_name, u32 type, u32 mask)
{
	struct crypto_tfm *tfm;
	int err;

	for (;;) {
		struct crypto_alg *alg;

		alg = crypto_alg_mod_lookup(alg_name, type, mask);
		if (IS_ERR(alg)) {
			err = PTR_ERR(alg);
			goto err;
		}

		tfm = __crypto_alloc_tfm(alg, type, mask);
		if (!IS_ERR(tfm))
			return tfm;

		crypto_mod_put(alg);
		err = PTR_ERR(tfm);

err:
		if (err != -EAGAIN)
			break;
		if (fatal_signal_pending(current)) {
			err = -EINTR;
			break;
		}
	}

	return ERR_PTR(err);
}
EXPORT_SYMBOL_GPL(crypto_alloc_base);

<<<<<<< HEAD
void *crypto_create_tfm(struct crypto_alg *alg,
			const struct crypto_type *frontend)
{
	char *mem;
	struct crypto_tfm *tfm = NULL;
	unsigned int tfmsize;
	unsigned int total;
	int err = -ENOMEM;
=======
static void *crypto_alloc_tfmmem(struct crypto_alg *alg,
				 const struct crypto_type *frontend, int node,
				 gfp_t gfp)
{
	struct crypto_tfm *tfm;
	unsigned int tfmsize;
	unsigned int total;
	char *mem;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	tfmsize = frontend->tfmsize;
	total = tfmsize + sizeof(*tfm) + frontend->extsize(alg);

<<<<<<< HEAD
	mem = kzalloc(total, GFP_KERNEL);
	if (mem == NULL)
		goto out_err;

	tfm = (struct crypto_tfm *)(mem + tfmsize);
	tfm->__crt_alg = alg;
=======
	mem = kzalloc_node(total, gfp, node);
	if (mem == NULL)
		return ERR_PTR(-ENOMEM);

	tfm = (struct crypto_tfm *)(mem + tfmsize);
	tfm->__crt_alg = alg;
	tfm->node = node;
	refcount_set(&tfm->refcnt, 1);

	return mem;
}

void *crypto_create_tfm_node(struct crypto_alg *alg,
			     const struct crypto_type *frontend,
			     int node)
{
	struct crypto_tfm *tfm;
	char *mem;
	int err;

	mem = crypto_alloc_tfmmem(alg, frontend, node, GFP_KERNEL);
	if (IS_ERR(mem))
		goto out;

	tfm = (struct crypto_tfm *)(mem + frontend->tfmsize);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	err = frontend->init_tfm(tfm);
	if (err)
		goto out_free_tfm;

	if (!tfm->exit && alg->cra_init && (err = alg->cra_init(tfm)))
		goto cra_init_failed;

	goto out;

cra_init_failed:
	crypto_exit_ops(tfm);
out_free_tfm:
	if (err == -EAGAIN)
		crypto_shoot_alg(alg);
	kfree(mem);
<<<<<<< HEAD
out_err:
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mem = ERR_PTR(err);
out:
	return mem;
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(crypto_create_tfm);
=======
EXPORT_SYMBOL_GPL(crypto_create_tfm_node);

void *crypto_clone_tfm(const struct crypto_type *frontend,
		       struct crypto_tfm *otfm)
{
	struct crypto_alg *alg = otfm->__crt_alg;
	struct crypto_tfm *tfm;
	char *mem;

	mem = ERR_PTR(-ESTALE);
	if (unlikely(!crypto_mod_get(alg)))
		goto out;

	mem = crypto_alloc_tfmmem(alg, frontend, otfm->node, GFP_ATOMIC);
	if (IS_ERR(mem)) {
		crypto_mod_put(alg);
		goto out;
	}

	tfm = (struct crypto_tfm *)(mem + frontend->tfmsize);
	tfm->crt_flags = otfm->crt_flags;
	tfm->exit = otfm->exit;

out:
	return mem;
}
EXPORT_SYMBOL_GPL(crypto_clone_tfm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct crypto_alg *crypto_find_alg(const char *alg_name,
				   const struct crypto_type *frontend,
				   u32 type, u32 mask)
{
<<<<<<< HEAD
	struct crypto_alg *(*lookup)(const char *name, u32 type, u32 mask) =
		crypto_alg_mod_lookup;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (frontend) {
		type &= frontend->maskclear;
		mask &= frontend->maskclear;
		type |= frontend->type;
		mask |= frontend->maskset;
<<<<<<< HEAD

		if (frontend->lookup)
			lookup = frontend->lookup;
	}

	return lookup(alg_name, type, mask);
=======
	}

	return crypto_alg_mod_lookup(alg_name, type, mask);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(crypto_find_alg);

/*
<<<<<<< HEAD
 *	crypto_alloc_tfm - Locate algorithm and allocate transform
=======
 *	crypto_alloc_tfm_node - Locate algorithm and allocate transform
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *	@alg_name: Name of algorithm
 *	@frontend: Frontend algorithm type
 *	@type: Type of algorithm
 *	@mask: Mask for type comparison
<<<<<<< HEAD
=======
 *	@node: NUMA node in which users desire to put requests, if node is
 *		NUMA_NO_NODE, it means users have no special requirement.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 *	crypto_alloc_tfm() will first attempt to locate an already loaded
 *	algorithm.  If that fails and the kernel supports dynamically loadable
 *	modules, it will then attempt to load a module of the same name or
 *	alias.  If that fails it will send a query to any loaded crypto manager
 *	to construct an algorithm on the fly.  A refcount is grabbed on the
 *	algorithm which is then associated with the new transform.
 *
 *	The returned transform is of a non-determinate type.  Most people
 *	should use one of the more specific allocation functions such as
<<<<<<< HEAD
 *	crypto_alloc_blkcipher.
 *
 *	In case of error the return value is an error pointer.
 */
void *crypto_alloc_tfm(const char *alg_name,
		       const struct crypto_type *frontend, u32 type, u32 mask)
=======
 *	crypto_alloc_skcipher().
 *
 *	In case of error the return value is an error pointer.
 */

void *crypto_alloc_tfm_node(const char *alg_name,
		       const struct crypto_type *frontend, u32 type, u32 mask,
		       int node)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	void *tfm;
	int err;

	for (;;) {
		struct crypto_alg *alg;

		alg = crypto_find_alg(alg_name, frontend, type, mask);
		if (IS_ERR(alg)) {
			err = PTR_ERR(alg);
			goto err;
		}

<<<<<<< HEAD
		tfm = crypto_create_tfm(alg, frontend);
=======
		tfm = crypto_create_tfm_node(alg, frontend, node);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!IS_ERR(tfm))
			return tfm;

		crypto_mod_put(alg);
		err = PTR_ERR(tfm);

err:
		if (err != -EAGAIN)
			break;
		if (fatal_signal_pending(current)) {
			err = -EINTR;
			break;
		}
	}

	return ERR_PTR(err);
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(crypto_alloc_tfm);
=======
EXPORT_SYMBOL_GPL(crypto_alloc_tfm_node);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 *	crypto_destroy_tfm - Free crypto transform
 *	@mem: Start of tfm slab
 *	@tfm: Transform to free
 *
 *	This function frees up the transform and any associated resources,
 *	then drops the refcount on the associated algorithm.
 */
void crypto_destroy_tfm(void *mem, struct crypto_tfm *tfm)
{
	struct crypto_alg *alg;

<<<<<<< HEAD
	if (unlikely(!mem))
		return;

=======
	if (IS_ERR_OR_NULL(mem))
		return;

	if (!refcount_dec_and_test(&tfm->refcnt))
		return;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	alg = tfm->__crt_alg;

	if (!tfm->exit && alg->cra_exit)
		alg->cra_exit(tfm);
	crypto_exit_ops(tfm);
	crypto_mod_put(alg);
<<<<<<< HEAD
#if FIPS_CRYPTO_TEST == 5
    {
        extern void hexdump(unsigned char *, unsigned int);
        int t = ksize(mem);
        printk(KERN_ERR "FIPS: Before zeroize crypto tfm size:%d\n", t);
        hexdump(mem, t);
#endif
	kzfree(mem);
#if FIPS_CRYPTO_TEST == 5
        printk(KERN_ERR "FIPS: After zeroize crypto tfm\n");
        hexdump(mem, t);
    }
#endif
=======
	kfree_sensitive(mem);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(crypto_destroy_tfm);

int crypto_has_alg(const char *name, u32 type, u32 mask)
{
	int ret = 0;
	struct crypto_alg *alg = crypto_alg_mod_lookup(name, type, mask);

	if (!IS_ERR(alg)) {
		crypto_mod_put(alg);
		ret = 1;
	}

	return ret;
}
EXPORT_SYMBOL_GPL(crypto_has_alg);

<<<<<<< HEAD
=======
void crypto_req_done(void *data, int err)
{
	struct crypto_wait *wait = data;

	if (err == -EINPROGRESS)
		return;

	wait->err = err;
	complete(&wait->completion);
}
EXPORT_SYMBOL_GPL(crypto_req_done);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_DESCRIPTION("Cryptographic core API");
MODULE_LICENSE("GPL");
