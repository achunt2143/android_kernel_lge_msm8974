<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Cryptographic API.
 *
 * Copyright (c) 2002 James Morris <jmorris@intercode.com.au>
 * Copyright (c) 2005 Herbert Xu <herbert@gondor.apana.org.au>
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
#ifndef _CRYPTO_INTERNAL_H
#define _CRYPTO_INTERNAL_H

#include <crypto/algapi.h>
#include <linux/completion.h>
<<<<<<< HEAD
#include <linux/mm.h>
#include <linux/highmem.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/notifier.h>
#include <linux/rwsem.h>
#include <linux/slab.h>
#include <linux/fips.h>

/*
 * FIPS_CRYPTO_TEST 1 : aes-cbc self-test failure
 * FIPS_CRYPTO_TEST 2 : hmac-sha256 self-test failure
 * FIPS_CRYPTO_TEST 3 : continous PRNG test fail
 * FIPS_CRYPTO_TEST 4 : Add log for algorithm self-test
 * FIPS_CRYPTO_TEST 5 : zeroization test.
*/
//set number what you want test
#define FIPS_CRYPTO_TEST 0

/* Crypto notification events. */
enum {
	CRYPTO_MSG_ALG_REQUEST,
	CRYPTO_MSG_ALG_REGISTER,
	CRYPTO_MSG_ALG_UNREGISTER,
	CRYPTO_MSG_TMPL_REGISTER,
	CRYPTO_MSG_TMPL_UNREGISTER,
};

=======
#include <linux/err.h>
#include <linux/jump_label.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/notifier.h>
#include <linux/numa.h>
#include <linux/refcount.h>
#include <linux/rwsem.h>
#include <linux/scatterlist.h>
#include <linux/sched.h>
#include <linux/types.h>

struct akcipher_request;
struct crypto_akcipher;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct crypto_instance;
struct crypto_template;

struct crypto_larval {
	struct crypto_alg alg;
	struct crypto_alg *adult;
	struct completion completion;
	u32 mask;
<<<<<<< HEAD
};

=======
	bool test_started;
};

struct crypto_akcipher_sync_data {
	struct crypto_akcipher *tfm;
	const void *src;
	void *dst;
	unsigned int slen;
	unsigned int dlen;

	struct akcipher_request *req;
	struct crypto_wait cwait;
	struct scatterlist sg;
	u8 *buf;
};

enum {
	CRYPTOA_UNSPEC,
	CRYPTOA_ALG,
	CRYPTOA_TYPE,
	__CRYPTOA_MAX,
};

#define CRYPTOA_MAX (__CRYPTOA_MAX - 1)

/* Maximum number of (rtattr) parameters for each template. */
#define CRYPTO_MAX_ATTRS 32

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern struct list_head crypto_alg_list;
extern struct rw_semaphore crypto_alg_sem;
extern struct blocking_notifier_head crypto_chain;

<<<<<<< HEAD
#ifdef CONFIG_CRYPTO_FIPS
void set_fips_error(void);
#endif

#ifdef CONFIG_PROC_FS
#ifdef CONFIG_CRYPTO_FIPS
void fips_init_proc(void);
void crypto_init_proc(int * fips_error, int * cc_mode);
#else
void __init crypto_init_proc(void);
#endif
=======
int alg_test(const char *driver, const char *alg, u32 type, u32 mask);

#ifdef CONFIG_CRYPTO_MANAGER_DISABLE_TESTS
static inline bool crypto_boot_test_finished(void)
{
	return true;
}
static inline void set_crypto_boot_test_finished(void)
{
}
#else
DECLARE_STATIC_KEY_FALSE(__crypto_boot_test_finished);
static inline bool crypto_boot_test_finished(void)
{
	return static_branch_likely(&__crypto_boot_test_finished);
}
static inline void set_crypto_boot_test_finished(void)
{
	static_branch_enable(&__crypto_boot_test_finished);
}
#endif /* !CONFIG_CRYPTO_MANAGER_DISABLE_TESTS */

#ifdef CONFIG_PROC_FS
void __init crypto_init_proc(void);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void __exit crypto_exit_proc(void);
#else
static inline void crypto_init_proc(void)
{ }
static inline void crypto_exit_proc(void)
{ }
#endif

static inline unsigned int crypto_cipher_ctxsize(struct crypto_alg *alg)
{
	return alg->cra_ctxsize;
}

static inline unsigned int crypto_compress_ctxsize(struct crypto_alg *alg)
{
	return alg->cra_ctxsize;
}

struct crypto_alg *crypto_mod_get(struct crypto_alg *alg);
<<<<<<< HEAD
struct crypto_alg *crypto_alg_lookup(const char *name, u32 type, u32 mask);
struct crypto_alg *crypto_alg_mod_lookup(const char *name, u32 type, u32 mask);

int crypto_init_cipher_ops(struct crypto_tfm *tfm);
int crypto_init_compress_ops(struct crypto_tfm *tfm);

void crypto_exit_cipher_ops(struct crypto_tfm *tfm);
void crypto_exit_compress_ops(struct crypto_tfm *tfm);

struct crypto_larval *crypto_larval_alloc(const char *name, u32 type, u32 mask);
void crypto_larval_kill(struct crypto_alg *alg);
struct crypto_alg *crypto_larval_lookup(const char *name, u32 type, u32 mask);
void crypto_larval_error(const char *name, u32 type, u32 mask);
=======
struct crypto_alg *crypto_alg_mod_lookup(const char *name, u32 type, u32 mask);

struct crypto_larval *crypto_larval_alloc(const char *name, u32 type, u32 mask);
void crypto_larval_kill(struct crypto_alg *alg);
void crypto_wait_for_test(struct crypto_larval *larval);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void crypto_alg_tested(const char *name, int err);

void crypto_remove_spawns(struct crypto_alg *alg, struct list_head *list,
			  struct crypto_alg *nalg);
void crypto_remove_final(struct list_head *list);
void crypto_shoot_alg(struct crypto_alg *alg);
<<<<<<< HEAD
struct crypto_tfm *__crypto_alloc_tfm(struct crypto_alg *alg, u32 type,
				      u32 mask);
void *crypto_create_tfm(struct crypto_alg *alg,
			const struct crypto_type *frontend);
struct crypto_alg *crypto_find_alg(const char *alg_name,
				   const struct crypto_type *frontend,
				   u32 type, u32 mask);
void *crypto_alloc_tfm(const char *alg_name,
		       const struct crypto_type *frontend, u32 type, u32 mask);

int crypto_register_notifier(struct notifier_block *nb);
int crypto_unregister_notifier(struct notifier_block *nb);
int crypto_probing_notify(unsigned long val, void *v);

static inline void crypto_alg_put(struct crypto_alg *alg)
{
	if (atomic_dec_and_test(&alg->cra_refcnt) && alg->cra_destroy)
=======
struct crypto_tfm *__crypto_alloc_tfmgfp(struct crypto_alg *alg, u32 type,
					 u32 mask, gfp_t gfp);
struct crypto_tfm *__crypto_alloc_tfm(struct crypto_alg *alg, u32 type,
				      u32 mask);
void *crypto_create_tfm_node(struct crypto_alg *alg,
			const struct crypto_type *frontend, int node);
void *crypto_clone_tfm(const struct crypto_type *frontend,
		       struct crypto_tfm *otfm);

int crypto_akcipher_sync_prep(struct crypto_akcipher_sync_data *data);
int crypto_akcipher_sync_post(struct crypto_akcipher_sync_data *data, int err);
int crypto_init_akcipher_ops_sig(struct crypto_tfm *tfm);

static inline void *crypto_create_tfm(struct crypto_alg *alg,
			const struct crypto_type *frontend)
{
	return crypto_create_tfm_node(alg, frontend, NUMA_NO_NODE);
}

struct crypto_alg *crypto_find_alg(const char *alg_name,
				   const struct crypto_type *frontend,
				   u32 type, u32 mask);

void *crypto_alloc_tfm_node(const char *alg_name,
		       const struct crypto_type *frontend, u32 type, u32 mask,
		       int node);

static inline void *crypto_alloc_tfm(const char *alg_name,
		       const struct crypto_type *frontend, u32 type, u32 mask)
{
	return crypto_alloc_tfm_node(alg_name, frontend, type, mask, NUMA_NO_NODE);
}

int crypto_probing_notify(unsigned long val, void *v);

unsigned int crypto_alg_extsize(struct crypto_alg *alg);

int crypto_type_has_alg(const char *name, const struct crypto_type *frontend,
			u32 type, u32 mask);

static inline struct crypto_alg *crypto_alg_get(struct crypto_alg *alg)
{
	refcount_inc(&alg->cra_refcnt);
	return alg;
}

static inline void crypto_alg_put(struct crypto_alg *alg)
{
	if (refcount_dec_and_test(&alg->cra_refcnt) && alg->cra_destroy)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		alg->cra_destroy(alg);
}

static inline int crypto_tmpl_get(struct crypto_template *tmpl)
{
	return try_module_get(tmpl->module);
}

static inline void crypto_tmpl_put(struct crypto_template *tmpl)
{
	module_put(tmpl->module);
}

static inline int crypto_is_larval(struct crypto_alg *alg)
{
	return alg->cra_flags & CRYPTO_ALG_LARVAL;
}

static inline int crypto_is_dead(struct crypto_alg *alg)
{
	return alg->cra_flags & CRYPTO_ALG_DEAD;
}

static inline int crypto_is_moribund(struct crypto_alg *alg)
{
	return alg->cra_flags & (CRYPTO_ALG_DEAD | CRYPTO_ALG_DYING);
}

static inline void crypto_notify(unsigned long val, void *v)
{
	blocking_notifier_call_chain(&crypto_chain, val, v);
}

<<<<<<< HEAD
=======
static inline void crypto_yield(u32 flags)
{
	if (flags & CRYPTO_TFM_REQ_MAY_SLEEP)
		cond_resched();
}

static inline int crypto_is_test_larval(struct crypto_larval *larval)
{
	return larval->alg.cra_driver_name[0];
}

static inline struct crypto_tfm *crypto_tfm_get(struct crypto_tfm *tfm)
{
	return refcount_inc_not_zero(&tfm->refcnt) ? tfm : ERR_PTR(-EOVERFLOW);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif	/* _CRYPTO_INTERNAL_H */

