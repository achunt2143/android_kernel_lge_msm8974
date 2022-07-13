<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Symmetric key ciphers.
 * 
 * Copyright (c) 2007 Herbert Xu <herbert@gondor.apana.org.au>
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

#ifndef _CRYPTO_INTERNAL_SKCIPHER_H
#define _CRYPTO_INTERNAL_SKCIPHER_H

#include <crypto/algapi.h>
<<<<<<< HEAD
#include <crypto/skcipher.h>
#include <linux/types.h>

struct rtattr;

=======
#include <crypto/internal/cipher.h>
#include <crypto/skcipher.h>
#include <linux/list.h>
#include <linux/types.h>

/*
 * Set this if your algorithm is sync but needs a reqsize larger
 * than MAX_SYNC_SKCIPHER_REQSIZE.
 *
 * Reuse bit that is specific to hash algorithms.
 */
#define CRYPTO_ALG_SKCIPHER_REQSIZE_LARGE CRYPTO_ALG_OPTIONAL_KEY

struct aead_request;
struct rtattr;

struct skcipher_instance {
	void (*free)(struct skcipher_instance *inst);
	union {
		struct {
			char head[offsetof(struct skcipher_alg, base)];
			struct crypto_instance base;
		} s;
		struct skcipher_alg alg;
	};
};

struct lskcipher_instance {
	void (*free)(struct lskcipher_instance *inst);
	union {
		struct {
			char head[offsetof(struct lskcipher_alg, co.base)];
			struct crypto_instance base;
		} s;
		struct lskcipher_alg alg;
	};
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct crypto_skcipher_spawn {
	struct crypto_spawn base;
};

<<<<<<< HEAD
extern const struct crypto_type crypto_givcipher_type;

static inline void crypto_set_skcipher_spawn(
	struct crypto_skcipher_spawn *spawn, struct crypto_instance *inst)
{
	crypto_set_spawn(&spawn->base, inst);
}

int crypto_grab_skcipher(struct crypto_skcipher_spawn *spawn, const char *name,
			 u32 type, u32 mask);

struct crypto_alg *crypto_lookup_skcipher(const char *name, u32 type, u32 mask);
=======
struct crypto_lskcipher_spawn {
	struct crypto_spawn base;
};

struct skcipher_walk {
	union {
		struct {
			struct page *page;
			unsigned long offset;
		} phys;

		struct {
			u8 *page;
			void *addr;
		} virt;
	} src, dst;

	struct scatter_walk in;
	unsigned int nbytes;

	struct scatter_walk out;
	unsigned int total;

	struct list_head buffers;

	u8 *page;
	u8 *buffer;
	u8 *oiv;
	void *iv;

	unsigned int ivsize;

	int flags;
	unsigned int blocksize;
	unsigned int stride;
	unsigned int alignmask;
};

static inline struct crypto_instance *skcipher_crypto_instance(
	struct skcipher_instance *inst)
{
	return &inst->s.base;
}

static inline struct crypto_instance *lskcipher_crypto_instance(
	struct lskcipher_instance *inst)
{
	return &inst->s.base;
}

static inline struct skcipher_instance *skcipher_alg_instance(
	struct crypto_skcipher *skcipher)
{
	return container_of(crypto_skcipher_alg(skcipher),
			    struct skcipher_instance, alg);
}

static inline struct lskcipher_instance *lskcipher_alg_instance(
	struct crypto_lskcipher *lskcipher)
{
	return container_of(crypto_lskcipher_alg(lskcipher),
			    struct lskcipher_instance, alg);
}

static inline void *skcipher_instance_ctx(struct skcipher_instance *inst)
{
	return crypto_instance_ctx(skcipher_crypto_instance(inst));
}

static inline void *lskcipher_instance_ctx(struct lskcipher_instance *inst)
{
	return crypto_instance_ctx(lskcipher_crypto_instance(inst));
}

static inline void skcipher_request_complete(struct skcipher_request *req, int err)
{
	crypto_request_complete(&req->base, err);
}

int crypto_grab_skcipher(struct crypto_skcipher_spawn *spawn,
			 struct crypto_instance *inst,
			 const char *name, u32 type, u32 mask);

int crypto_grab_lskcipher(struct crypto_lskcipher_spawn *spawn,
			  struct crypto_instance *inst,
			  const char *name, u32 type, u32 mask);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline void crypto_drop_skcipher(struct crypto_skcipher_spawn *spawn)
{
	crypto_drop_spawn(&spawn->base);
}

<<<<<<< HEAD
static inline struct crypto_alg *crypto_skcipher_spawn_alg(
	struct crypto_skcipher_spawn *spawn)
{
	return spawn->base.alg;
}

static inline struct crypto_ablkcipher *crypto_spawn_skcipher(
	struct crypto_skcipher_spawn *spawn)
{
	return __crypto_ablkcipher_cast(
		crypto_spawn_tfm(&spawn->base, crypto_skcipher_type(0),
				 crypto_skcipher_mask(0)));
}

int skcipher_null_givencrypt(struct skcipher_givcrypt_request *req);
int skcipher_null_givdecrypt(struct skcipher_givcrypt_request *req);
const char *crypto_default_geniv(const struct crypto_alg *alg);

struct crypto_instance *skcipher_geniv_alloc(struct crypto_template *tmpl,
					     struct rtattr **tb, u32 type,
					     u32 mask);
void skcipher_geniv_free(struct crypto_instance *inst);
int skcipher_geniv_init(struct crypto_tfm *tfm);
void skcipher_geniv_exit(struct crypto_tfm *tfm);

static inline struct crypto_ablkcipher *skcipher_geniv_cipher(
	struct crypto_ablkcipher *geniv)
{
	return crypto_ablkcipher_crt(geniv)->base;
}

static inline int skcipher_enqueue_givcrypt(
	struct crypto_queue *queue, struct skcipher_givcrypt_request *request)
{
	return ablkcipher_enqueue_request(queue, &request->creq);
}

static inline struct skcipher_givcrypt_request *skcipher_dequeue_givcrypt(
	struct crypto_queue *queue)
{
	return __crypto_dequeue_request(
		queue, offsetof(struct skcipher_givcrypt_request, creq.base));
}

static inline void *skcipher_givcrypt_reqctx(
	struct skcipher_givcrypt_request *req)
{
	return ablkcipher_request_ctx(&req->creq);
}

static inline void ablkcipher_request_complete(struct ablkcipher_request *req,
					       int err)
{
	req->base.complete(&req->base, err);
}

static inline void skcipher_givcrypt_complete(
	struct skcipher_givcrypt_request *req, int err)
{
	ablkcipher_request_complete(&req->creq, err);
}

static inline u32 ablkcipher_request_flags(struct ablkcipher_request *req)
=======
static inline void crypto_drop_lskcipher(struct crypto_lskcipher_spawn *spawn)
{
	crypto_drop_spawn(&spawn->base);
}

static inline struct lskcipher_alg *crypto_lskcipher_spawn_alg(
	struct crypto_lskcipher_spawn *spawn)
{
	return container_of(spawn->base.alg, struct lskcipher_alg, co.base);
}

static inline struct skcipher_alg_common *crypto_spawn_skcipher_alg_common(
	struct crypto_skcipher_spawn *spawn)
{
	return container_of(spawn->base.alg, struct skcipher_alg_common, base);
}

static inline struct lskcipher_alg *crypto_spawn_lskcipher_alg(
	struct crypto_lskcipher_spawn *spawn)
{
	return crypto_lskcipher_spawn_alg(spawn);
}

static inline struct crypto_skcipher *crypto_spawn_skcipher(
	struct crypto_skcipher_spawn *spawn)
{
	return crypto_spawn_tfm2(&spawn->base);
}

static inline struct crypto_lskcipher *crypto_spawn_lskcipher(
	struct crypto_lskcipher_spawn *spawn)
{
	return crypto_spawn_tfm2(&spawn->base);
}

static inline void crypto_skcipher_set_reqsize(
	struct crypto_skcipher *skcipher, unsigned int reqsize)
{
	skcipher->reqsize = reqsize;
}

static inline void crypto_skcipher_set_reqsize_dma(
	struct crypto_skcipher *skcipher, unsigned int reqsize)
{
	reqsize += crypto_dma_align() & ~(crypto_tfm_ctx_alignment() - 1);
	skcipher->reqsize = reqsize;
}

int crypto_register_skcipher(struct skcipher_alg *alg);
void crypto_unregister_skcipher(struct skcipher_alg *alg);
int crypto_register_skciphers(struct skcipher_alg *algs, int count);
void crypto_unregister_skciphers(struct skcipher_alg *algs, int count);
int skcipher_register_instance(struct crypto_template *tmpl,
			       struct skcipher_instance *inst);

int crypto_register_lskcipher(struct lskcipher_alg *alg);
void crypto_unregister_lskcipher(struct lskcipher_alg *alg);
int crypto_register_lskciphers(struct lskcipher_alg *algs, int count);
void crypto_unregister_lskciphers(struct lskcipher_alg *algs, int count);
int lskcipher_register_instance(struct crypto_template *tmpl,
				struct lskcipher_instance *inst);

int skcipher_walk_done(struct skcipher_walk *walk, int err);
int skcipher_walk_virt(struct skcipher_walk *walk,
		       struct skcipher_request *req,
		       bool atomic);
int skcipher_walk_async(struct skcipher_walk *walk,
			struct skcipher_request *req);
int skcipher_walk_aead_encrypt(struct skcipher_walk *walk,
			       struct aead_request *req, bool atomic);
int skcipher_walk_aead_decrypt(struct skcipher_walk *walk,
			       struct aead_request *req, bool atomic);
void skcipher_walk_complete(struct skcipher_walk *walk, int err);

static inline void skcipher_walk_abort(struct skcipher_walk *walk)
{
	skcipher_walk_done(walk, -ECANCELED);
}

static inline void *crypto_skcipher_ctx(struct crypto_skcipher *tfm)
{
	return crypto_tfm_ctx(&tfm->base);
}

static inline void *crypto_lskcipher_ctx(struct crypto_lskcipher *tfm)
{
	return crypto_tfm_ctx(&tfm->base);
}

static inline void *crypto_skcipher_ctx_dma(struct crypto_skcipher *tfm)
{
	return crypto_tfm_ctx_dma(&tfm->base);
}

static inline void *skcipher_request_ctx(struct skcipher_request *req)
{
	return req->__ctx;
}

static inline void *skcipher_request_ctx_dma(struct skcipher_request *req)
{
	unsigned int align = crypto_dma_align();

	if (align <= crypto_tfm_ctx_alignment())
		align = 1;

	return PTR_ALIGN(skcipher_request_ctx(req), align);
}

static inline u32 skcipher_request_flags(struct skcipher_request *req)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return req->base.flags;
}

<<<<<<< HEAD
=======
/* Helpers for simple block cipher modes of operation */
struct skcipher_ctx_simple {
	struct crypto_cipher *cipher;	/* underlying block cipher */
};
static inline struct crypto_cipher *
skcipher_cipher_simple(struct crypto_skcipher *tfm)
{
	struct skcipher_ctx_simple *ctx = crypto_skcipher_ctx(tfm);

	return ctx->cipher;
}

struct skcipher_instance *skcipher_alloc_instance_simple(
	struct crypto_template *tmpl, struct rtattr **tb);

static inline struct crypto_alg *skcipher_ialg_simple(
	struct skcipher_instance *inst)
{
	struct crypto_cipher_spawn *spawn = skcipher_instance_ctx(inst);

	return crypto_spawn_cipher_alg(spawn);
}

static inline struct crypto_lskcipher *lskcipher_cipher_simple(
	struct crypto_lskcipher *tfm)
{
	struct crypto_lskcipher **ctx = crypto_lskcipher_ctx(tfm);

	return *ctx;
}

struct lskcipher_instance *lskcipher_alloc_instance_simple(
	struct crypto_template *tmpl, struct rtattr **tb);

static inline struct lskcipher_alg *lskcipher_ialg_simple(
	struct lskcipher_instance *inst)
{
	struct crypto_lskcipher_spawn *spawn = lskcipher_instance_ctx(inst);

	return crypto_lskcipher_spawn_alg(spawn);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif	/* _CRYPTO_INTERNAL_SKCIPHER_H */

