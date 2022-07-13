<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * CTR: Counter mode
 *
 * (C) Copyright IBM Corp. 2007 - Joy Latten <latten@us.ibm.com>
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

#include <crypto/algapi.h>
#include <crypto/ctr.h>
<<<<<<< HEAD
=======
#include <crypto/internal/cipher.h>
#include <crypto/internal/skcipher.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/err.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
<<<<<<< HEAD
#include <linux/random.h>
#include <linux/scatterlist.h>
#include <linux/slab.h>

struct crypto_ctr_ctx {
	struct crypto_cipher *child;
};

struct crypto_rfc3686_ctx {
	struct crypto_blkcipher *child;
	u8 nonce[CTR_RFC3686_NONCE_SIZE];
};

static int crypto_ctr_setkey(struct crypto_tfm *parent, const u8 *key,
			     unsigned int keylen)
{
	struct crypto_ctr_ctx *ctx = crypto_tfm_ctx(parent);
	struct crypto_cipher *child = ctx->child;
	int err;

	crypto_cipher_clear_flags(child, CRYPTO_TFM_REQ_MASK);
	crypto_cipher_set_flags(child, crypto_tfm_get_flags(parent) &
				CRYPTO_TFM_REQ_MASK);
	err = crypto_cipher_setkey(child, key, keylen);
	crypto_tfm_set_flags(parent, crypto_cipher_get_flags(child) &
			     CRYPTO_TFM_RES_MASK);

	return err;
}

static void crypto_ctr_crypt_final(struct blkcipher_walk *walk,
=======
#include <linux/slab.h>

struct crypto_rfc3686_ctx {
	struct crypto_skcipher *child;
	u8 nonce[CTR_RFC3686_NONCE_SIZE];
};

struct crypto_rfc3686_req_ctx {
	u8 iv[CTR_RFC3686_BLOCK_SIZE];
	struct skcipher_request subreq CRYPTO_MINALIGN_ATTR;
};

static void crypto_ctr_crypt_final(struct skcipher_walk *walk,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				   struct crypto_cipher *tfm)
{
	unsigned int bsize = crypto_cipher_blocksize(tfm);
	unsigned long alignmask = crypto_cipher_alignmask(tfm);
	u8 *ctrblk = walk->iv;
<<<<<<< HEAD
	u8 tmp[bsize + alignmask];
=======
	u8 tmp[MAX_CIPHER_BLOCKSIZE + MAX_CIPHER_ALIGNMASK];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u8 *keystream = PTR_ALIGN(tmp + 0, alignmask + 1);
	u8 *src = walk->src.virt.addr;
	u8 *dst = walk->dst.virt.addr;
	unsigned int nbytes = walk->nbytes;

	crypto_cipher_encrypt_one(tfm, keystream, ctrblk);
<<<<<<< HEAD
	crypto_xor(keystream, src, nbytes);
	memcpy(dst, keystream, nbytes);
=======
	crypto_xor_cpy(dst, keystream, src, nbytes);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	crypto_inc(ctrblk, bsize);
}

<<<<<<< HEAD
static int crypto_ctr_crypt_segment(struct blkcipher_walk *walk,
=======
static int crypto_ctr_crypt_segment(struct skcipher_walk *walk,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				    struct crypto_cipher *tfm)
{
	void (*fn)(struct crypto_tfm *, u8 *, const u8 *) =
		   crypto_cipher_alg(tfm)->cia_encrypt;
	unsigned int bsize = crypto_cipher_blocksize(tfm);
	u8 *ctrblk = walk->iv;
	u8 *src = walk->src.virt.addr;
	u8 *dst = walk->dst.virt.addr;
	unsigned int nbytes = walk->nbytes;

	do {
		/* create keystream */
		fn(crypto_cipher_tfm(tfm), dst, ctrblk);
		crypto_xor(dst, src, bsize);

		/* increment counter in counterblock */
		crypto_inc(ctrblk, bsize);

		src += bsize;
		dst += bsize;
	} while ((nbytes -= bsize) >= bsize);

	return nbytes;
}

<<<<<<< HEAD
static int crypto_ctr_crypt_inplace(struct blkcipher_walk *walk,
=======
static int crypto_ctr_crypt_inplace(struct skcipher_walk *walk,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				    struct crypto_cipher *tfm)
{
	void (*fn)(struct crypto_tfm *, u8 *, const u8 *) =
		   crypto_cipher_alg(tfm)->cia_encrypt;
	unsigned int bsize = crypto_cipher_blocksize(tfm);
	unsigned long alignmask = crypto_cipher_alignmask(tfm);
	unsigned int nbytes = walk->nbytes;
	u8 *ctrblk = walk->iv;
	u8 *src = walk->src.virt.addr;
<<<<<<< HEAD
	u8 tmp[bsize + alignmask];
=======
	u8 tmp[MAX_CIPHER_BLOCKSIZE + MAX_CIPHER_ALIGNMASK];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u8 *keystream = PTR_ALIGN(tmp + 0, alignmask + 1);

	do {
		/* create keystream */
		fn(crypto_cipher_tfm(tfm), keystream, ctrblk);
		crypto_xor(src, keystream, bsize);

		/* increment counter in counterblock */
		crypto_inc(ctrblk, bsize);

		src += bsize;
	} while ((nbytes -= bsize) >= bsize);

	return nbytes;
}

<<<<<<< HEAD
static int crypto_ctr_crypt(struct blkcipher_desc *desc,
			      struct scatterlist *dst, struct scatterlist *src,
			      unsigned int nbytes)
{
	struct blkcipher_walk walk;
	struct crypto_blkcipher *tfm = desc->tfm;
	struct crypto_ctr_ctx *ctx = crypto_blkcipher_ctx(tfm);
	struct crypto_cipher *child = ctx->child;
	unsigned int bsize = crypto_cipher_blocksize(child);
	int err;

	blkcipher_walk_init(&walk, dst, src, nbytes);
	err = blkcipher_walk_virt_block(desc, &walk, bsize);

	while (walk.nbytes >= bsize) {
		if (walk.src.virt.addr == walk.dst.virt.addr)
			nbytes = crypto_ctr_crypt_inplace(&walk, child);
		else
			nbytes = crypto_ctr_crypt_segment(&walk, child);

		err = blkcipher_walk_done(desc, &walk, nbytes);
	}

	if (walk.nbytes) {
		crypto_ctr_crypt_final(&walk, child);
		err = blkcipher_walk_done(desc, &walk, 0);
=======
static int crypto_ctr_crypt(struct skcipher_request *req)
{
	struct crypto_skcipher *tfm = crypto_skcipher_reqtfm(req);
	struct crypto_cipher *cipher = skcipher_cipher_simple(tfm);
	const unsigned int bsize = crypto_cipher_blocksize(cipher);
	struct skcipher_walk walk;
	unsigned int nbytes;
	int err;

	err = skcipher_walk_virt(&walk, req, false);

	while (walk.nbytes >= bsize) {
		if (walk.src.virt.addr == walk.dst.virt.addr)
			nbytes = crypto_ctr_crypt_inplace(&walk, cipher);
		else
			nbytes = crypto_ctr_crypt_segment(&walk, cipher);

		err = skcipher_walk_done(&walk, nbytes);
	}

	if (walk.nbytes) {
		crypto_ctr_crypt_final(&walk, cipher);
		err = skcipher_walk_done(&walk, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return err;
}

<<<<<<< HEAD
static int crypto_ctr_init_tfm(struct crypto_tfm *tfm)
{
	struct crypto_instance *inst = (void *)tfm->__crt_alg;
	struct crypto_spawn *spawn = crypto_instance_ctx(inst);
	struct crypto_ctr_ctx *ctx = crypto_tfm_ctx(tfm);
	struct crypto_cipher *cipher;

	cipher = crypto_spawn_cipher(spawn);
	if (IS_ERR(cipher))
		return PTR_ERR(cipher);

	ctx->child = cipher;

	return 0;
}

static void crypto_ctr_exit_tfm(struct crypto_tfm *tfm)
{
	struct crypto_ctr_ctx *ctx = crypto_tfm_ctx(tfm);

	crypto_free_cipher(ctx->child);
}

static struct crypto_instance *crypto_ctr_alloc(struct rtattr **tb)
{
	struct crypto_instance *inst;
	struct crypto_alg *alg;
	int err;

	err = crypto_check_attr_type(tb, CRYPTO_ALG_TYPE_BLKCIPHER);
	if (err)
		return ERR_PTR(err);

	alg = crypto_attr_alg(tb[1], CRYPTO_ALG_TYPE_CIPHER,
				  CRYPTO_ALG_TYPE_MASK);
	if (IS_ERR(alg))
		return ERR_CAST(alg);
=======
static int crypto_ctr_create(struct crypto_template *tmpl, struct rtattr **tb)
{
	struct skcipher_instance *inst;
	struct crypto_alg *alg;
	int err;

	inst = skcipher_alloc_instance_simple(tmpl, tb);
	if (IS_ERR(inst))
		return PTR_ERR(inst);

	alg = skcipher_ialg_simple(inst);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Block size must be >= 4 bytes. */
	err = -EINVAL;
	if (alg->cra_blocksize < 4)
<<<<<<< HEAD
		goto out_put_alg;

	/* If this is false we'd fail the alignment of crypto_inc. */
	if (alg->cra_blocksize % 4)
		goto out_put_alg;

	inst = crypto_alloc_instance("ctr", alg);
	if (IS_ERR(inst))
		goto out;

	inst->alg.cra_flags = CRYPTO_ALG_TYPE_BLKCIPHER;
	inst->alg.cra_priority = alg->cra_priority;
	inst->alg.cra_blocksize = 1;
	inst->alg.cra_alignmask = alg->cra_alignmask | (__alignof__(u32) - 1);
	inst->alg.cra_type = &crypto_blkcipher_type;

	inst->alg.cra_blkcipher.ivsize = alg->cra_blocksize;
	inst->alg.cra_blkcipher.min_keysize = alg->cra_cipher.cia_min_keysize;
	inst->alg.cra_blkcipher.max_keysize = alg->cra_cipher.cia_max_keysize;

	inst->alg.cra_ctxsize = sizeof(struct crypto_ctr_ctx);

	inst->alg.cra_init = crypto_ctr_init_tfm;
	inst->alg.cra_exit = crypto_ctr_exit_tfm;

	inst->alg.cra_blkcipher.setkey = crypto_ctr_setkey;
	inst->alg.cra_blkcipher.encrypt = crypto_ctr_crypt;
	inst->alg.cra_blkcipher.decrypt = crypto_ctr_crypt;

	inst->alg.cra_blkcipher.geniv = "chainiv";

out:
	crypto_mod_put(alg);
	return inst;

out_put_alg:
	inst = ERR_PTR(err);
	goto out;
}

static void crypto_ctr_free(struct crypto_instance *inst)
{
	crypto_drop_spawn(crypto_instance_ctx(inst));
	kfree(inst);
}

static struct crypto_template crypto_ctr_tmpl = {
	.name = "ctr",
	.alloc = crypto_ctr_alloc,
	.free = crypto_ctr_free,
	.module = THIS_MODULE,
};

static int crypto_rfc3686_setkey(struct crypto_tfm *parent, const u8 *key,
				 unsigned int keylen)
{
	struct crypto_rfc3686_ctx *ctx = crypto_tfm_ctx(parent);
	struct crypto_blkcipher *child = ctx->child;
	int err;
=======
		goto out_free_inst;

	/* If this is false we'd fail the alignment of crypto_inc. */
	if (alg->cra_blocksize % 4)
		goto out_free_inst;

	/* CTR mode is a stream cipher. */
	inst->alg.base.cra_blocksize = 1;

	/*
	 * To simplify the implementation, configure the skcipher walk to only
	 * give a partial block at the very end, never earlier.
	 */
	inst->alg.chunksize = alg->cra_blocksize;

	inst->alg.encrypt = crypto_ctr_crypt;
	inst->alg.decrypt = crypto_ctr_crypt;

	err = skcipher_register_instance(tmpl, inst);
	if (err) {
out_free_inst:
		inst->free(inst);
	}

	return err;
}

static int crypto_rfc3686_setkey(struct crypto_skcipher *parent,
				 const u8 *key, unsigned int keylen)
{
	struct crypto_rfc3686_ctx *ctx = crypto_skcipher_ctx(parent);
	struct crypto_skcipher *child = ctx->child;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* the nonce is stored in bytes at end of key */
	if (keylen < CTR_RFC3686_NONCE_SIZE)
		return -EINVAL;

	memcpy(ctx->nonce, key + (keylen - CTR_RFC3686_NONCE_SIZE),
	       CTR_RFC3686_NONCE_SIZE);

	keylen -= CTR_RFC3686_NONCE_SIZE;

<<<<<<< HEAD
	crypto_blkcipher_clear_flags(child, CRYPTO_TFM_REQ_MASK);
	crypto_blkcipher_set_flags(child, crypto_tfm_get_flags(parent) &
					  CRYPTO_TFM_REQ_MASK);
	err = crypto_blkcipher_setkey(child, key, keylen);
	crypto_tfm_set_flags(parent, crypto_blkcipher_get_flags(child) &
				     CRYPTO_TFM_RES_MASK);

	return err;
}

static int crypto_rfc3686_crypt(struct blkcipher_desc *desc,
				struct scatterlist *dst,
				struct scatterlist *src, unsigned int nbytes)
{
	struct crypto_blkcipher *tfm = desc->tfm;
	struct crypto_rfc3686_ctx *ctx = crypto_blkcipher_ctx(tfm);
	struct crypto_blkcipher *child = ctx->child;
	unsigned long alignmask = crypto_blkcipher_alignmask(tfm);
	u8 ivblk[CTR_RFC3686_BLOCK_SIZE + alignmask];
	u8 *iv = PTR_ALIGN(ivblk + 0, alignmask + 1);
	u8 *info = desc->info;
	int err;

	/* set up counter block */
	memcpy(iv, ctx->nonce, CTR_RFC3686_NONCE_SIZE);
	memcpy(iv + CTR_RFC3686_NONCE_SIZE, info, CTR_RFC3686_IV_SIZE);
=======
	crypto_skcipher_clear_flags(child, CRYPTO_TFM_REQ_MASK);
	crypto_skcipher_set_flags(child, crypto_skcipher_get_flags(parent) &
					 CRYPTO_TFM_REQ_MASK);
	return crypto_skcipher_setkey(child, key, keylen);
}

static int crypto_rfc3686_crypt(struct skcipher_request *req)
{
	struct crypto_skcipher *tfm = crypto_skcipher_reqtfm(req);
	struct crypto_rfc3686_ctx *ctx = crypto_skcipher_ctx(tfm);
	struct crypto_skcipher *child = ctx->child;
	unsigned long align = crypto_skcipher_alignmask(tfm);
	struct crypto_rfc3686_req_ctx *rctx =
		(void *)PTR_ALIGN((u8 *)skcipher_request_ctx(req), align + 1);
	struct skcipher_request *subreq = &rctx->subreq;
	u8 *iv = rctx->iv;

	/* set up counter block */
	memcpy(iv, ctx->nonce, CTR_RFC3686_NONCE_SIZE);
	memcpy(iv + CTR_RFC3686_NONCE_SIZE, req->iv, CTR_RFC3686_IV_SIZE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* initialize counter portion of counter block */
	*(__be32 *)(iv + CTR_RFC3686_NONCE_SIZE + CTR_RFC3686_IV_SIZE) =
		cpu_to_be32(1);

<<<<<<< HEAD
	desc->tfm = child;
	desc->info = iv;
	err = crypto_blkcipher_encrypt_iv(desc, dst, src, nbytes);
	desc->tfm = tfm;
	desc->info = info;

	return err;
}

static int crypto_rfc3686_init_tfm(struct crypto_tfm *tfm)
{
	struct crypto_instance *inst = (void *)tfm->__crt_alg;
	struct crypto_spawn *spawn = crypto_instance_ctx(inst);
	struct crypto_rfc3686_ctx *ctx = crypto_tfm_ctx(tfm);
	struct crypto_blkcipher *cipher;

	cipher = crypto_spawn_blkcipher(spawn);
=======
	skcipher_request_set_tfm(subreq, child);
	skcipher_request_set_callback(subreq, req->base.flags,
				      req->base.complete, req->base.data);
	skcipher_request_set_crypt(subreq, req->src, req->dst,
				   req->cryptlen, iv);

	return crypto_skcipher_encrypt(subreq);
}

static int crypto_rfc3686_init_tfm(struct crypto_skcipher *tfm)
{
	struct skcipher_instance *inst = skcipher_alg_instance(tfm);
	struct crypto_skcipher_spawn *spawn = skcipher_instance_ctx(inst);
	struct crypto_rfc3686_ctx *ctx = crypto_skcipher_ctx(tfm);
	struct crypto_skcipher *cipher;
	unsigned long align;
	unsigned int reqsize;

	cipher = crypto_spawn_skcipher(spawn);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (IS_ERR(cipher))
		return PTR_ERR(cipher);

	ctx->child = cipher;

<<<<<<< HEAD
	return 0;
}

static void crypto_rfc3686_exit_tfm(struct crypto_tfm *tfm)
{
	struct crypto_rfc3686_ctx *ctx = crypto_tfm_ctx(tfm);

	crypto_free_blkcipher(ctx->child);
}

static struct crypto_instance *crypto_rfc3686_alloc(struct rtattr **tb)
{
	struct crypto_instance *inst;
	struct crypto_alg *alg;
	int err;

	err = crypto_check_attr_type(tb, CRYPTO_ALG_TYPE_BLKCIPHER);
	if (err)
		return ERR_PTR(err);

	alg = crypto_attr_alg(tb[1], CRYPTO_ALG_TYPE_BLKCIPHER,
				  CRYPTO_ALG_TYPE_MASK);
	err = PTR_ERR(alg);
	if (IS_ERR(alg))
		return ERR_PTR(err);

	/* We only support 16-byte blocks. */
	err = -EINVAL;
	if (alg->cra_blkcipher.ivsize != CTR_RFC3686_BLOCK_SIZE)
		goto out_put_alg;

	/* Not a stream cipher? */
	if (alg->cra_blocksize != 1)
		goto out_put_alg;

	inst = crypto_alloc_instance("rfc3686", alg);
	if (IS_ERR(inst))
		goto out;

	inst->alg.cra_flags = CRYPTO_ALG_TYPE_BLKCIPHER;
	inst->alg.cra_priority = alg->cra_priority;
	inst->alg.cra_blocksize = 1;
	inst->alg.cra_alignmask = alg->cra_alignmask;
	inst->alg.cra_type = &crypto_blkcipher_type;

	inst->alg.cra_blkcipher.ivsize = CTR_RFC3686_IV_SIZE;
	inst->alg.cra_blkcipher.min_keysize = alg->cra_blkcipher.min_keysize
					      + CTR_RFC3686_NONCE_SIZE;
	inst->alg.cra_blkcipher.max_keysize = alg->cra_blkcipher.max_keysize
					      + CTR_RFC3686_NONCE_SIZE;

	inst->alg.cra_blkcipher.geniv = "seqiv";

	inst->alg.cra_ctxsize = sizeof(struct crypto_rfc3686_ctx);

	inst->alg.cra_init = crypto_rfc3686_init_tfm;
	inst->alg.cra_exit = crypto_rfc3686_exit_tfm;

	inst->alg.cra_blkcipher.setkey = crypto_rfc3686_setkey;
	inst->alg.cra_blkcipher.encrypt = crypto_rfc3686_crypt;
	inst->alg.cra_blkcipher.decrypt = crypto_rfc3686_crypt;

out:
	crypto_mod_put(alg);
	return inst;

out_put_alg:
	inst = ERR_PTR(err);
	goto out;
}

static struct crypto_template crypto_rfc3686_tmpl = {
	.name = "rfc3686",
	.alloc = crypto_rfc3686_alloc,
	.free = crypto_ctr_free,
	.module = THIS_MODULE,
=======
	align = crypto_skcipher_alignmask(tfm);
	align &= ~(crypto_tfm_ctx_alignment() - 1);
	reqsize = align + sizeof(struct crypto_rfc3686_req_ctx) +
		  crypto_skcipher_reqsize(cipher);
	crypto_skcipher_set_reqsize(tfm, reqsize);

	return 0;
}

static void crypto_rfc3686_exit_tfm(struct crypto_skcipher *tfm)
{
	struct crypto_rfc3686_ctx *ctx = crypto_skcipher_ctx(tfm);

	crypto_free_skcipher(ctx->child);
}

static void crypto_rfc3686_free(struct skcipher_instance *inst)
{
	struct crypto_skcipher_spawn *spawn = skcipher_instance_ctx(inst);

	crypto_drop_skcipher(spawn);
	kfree(inst);
}

static int crypto_rfc3686_create(struct crypto_template *tmpl,
				 struct rtattr **tb)
{
	struct skcipher_instance *inst;
	struct crypto_skcipher_spawn *spawn;
	struct skcipher_alg_common *alg;
	u32 mask;
	int err;

	err = crypto_check_attr_type(tb, CRYPTO_ALG_TYPE_SKCIPHER, &mask);
	if (err)
		return err;

	inst = kzalloc(sizeof(*inst) + sizeof(*spawn), GFP_KERNEL);
	if (!inst)
		return -ENOMEM;

	spawn = skcipher_instance_ctx(inst);

	err = crypto_grab_skcipher(spawn, skcipher_crypto_instance(inst),
				   crypto_attr_alg_name(tb[1]), 0, mask);
	if (err)
		goto err_free_inst;

	alg = crypto_spawn_skcipher_alg_common(spawn);

	/* We only support 16-byte blocks. */
	err = -EINVAL;
	if (alg->ivsize != CTR_RFC3686_BLOCK_SIZE)
		goto err_free_inst;

	/* Not a stream cipher? */
	if (alg->base.cra_blocksize != 1)
		goto err_free_inst;

	err = -ENAMETOOLONG;
	if (snprintf(inst->alg.base.cra_name, CRYPTO_MAX_ALG_NAME,
		     "rfc3686(%s)", alg->base.cra_name) >= CRYPTO_MAX_ALG_NAME)
		goto err_free_inst;
	if (snprintf(inst->alg.base.cra_driver_name, CRYPTO_MAX_ALG_NAME,
		     "rfc3686(%s)", alg->base.cra_driver_name) >=
	    CRYPTO_MAX_ALG_NAME)
		goto err_free_inst;

	inst->alg.base.cra_priority = alg->base.cra_priority;
	inst->alg.base.cra_blocksize = 1;
	inst->alg.base.cra_alignmask = alg->base.cra_alignmask;

	inst->alg.ivsize = CTR_RFC3686_IV_SIZE;
	inst->alg.chunksize = alg->chunksize;
	inst->alg.min_keysize = alg->min_keysize + CTR_RFC3686_NONCE_SIZE;
	inst->alg.max_keysize = alg->max_keysize + CTR_RFC3686_NONCE_SIZE;

	inst->alg.setkey = crypto_rfc3686_setkey;
	inst->alg.encrypt = crypto_rfc3686_crypt;
	inst->alg.decrypt = crypto_rfc3686_crypt;

	inst->alg.base.cra_ctxsize = sizeof(struct crypto_rfc3686_ctx);

	inst->alg.init = crypto_rfc3686_init_tfm;
	inst->alg.exit = crypto_rfc3686_exit_tfm;

	inst->free = crypto_rfc3686_free;

	err = skcipher_register_instance(tmpl, inst);
	if (err) {
err_free_inst:
		crypto_rfc3686_free(inst);
	}
	return err;
}

static struct crypto_template crypto_ctr_tmpls[] = {
	{
		.name = "ctr",
		.create = crypto_ctr_create,
		.module = THIS_MODULE,
	}, {
		.name = "rfc3686",
		.create = crypto_rfc3686_create,
		.module = THIS_MODULE,
	},
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static int __init crypto_ctr_module_init(void)
{
<<<<<<< HEAD
	int err;

	err = crypto_register_template(&crypto_ctr_tmpl);
	if (err)
		goto out;

	err = crypto_register_template(&crypto_rfc3686_tmpl);
	if (err)
		goto out_drop_ctr;

out:
	return err;

out_drop_ctr:
	crypto_unregister_template(&crypto_ctr_tmpl);
	goto out;
=======
	return crypto_register_templates(crypto_ctr_tmpls,
					 ARRAY_SIZE(crypto_ctr_tmpls));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void __exit crypto_ctr_module_exit(void)
{
<<<<<<< HEAD
	crypto_unregister_template(&crypto_rfc3686_tmpl);
	crypto_unregister_template(&crypto_ctr_tmpl);
}

module_init(crypto_ctr_module_init);
module_exit(crypto_ctr_module_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("CTR Counter block mode");
MODULE_ALIAS("rfc3686");
=======
	crypto_unregister_templates(crypto_ctr_tmpls,
				    ARRAY_SIZE(crypto_ctr_tmpls));
}

subsys_initcall(crypto_ctr_module_init);
module_exit(crypto_ctr_module_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("CTR block cipher mode of operation");
MODULE_ALIAS_CRYPTO("rfc3686");
MODULE_ALIAS_CRYPTO("ctr");
MODULE_IMPORT_NS(CRYPTO_INTERNAL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
