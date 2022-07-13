<<<<<<< HEAD
/*
 * CBC: Cipher Block Chaining mode
 *
 * Copyright (c) 2006 Herbert Xu <herbert@gondor.apana.org.au>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 */

#include <crypto/algapi.h>
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * CBC: Cipher Block Chaining mode
 *
 * Copyright (c) 2006-2016 Herbert Xu <herbert@gondor.apana.org.au>
 */

#include <crypto/internal/skcipher.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/err.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/log2.h>
#include <linux/module.h>
<<<<<<< HEAD
#include <linux/scatterlist.h>
#include <linux/slab.h>

struct crypto_cbc_ctx {
	struct crypto_cipher *child;
};

static int crypto_cbc_setkey(struct crypto_tfm *parent, const u8 *key,
			     unsigned int keylen)
{
	struct crypto_cbc_ctx *ctx = crypto_tfm_ctx(parent);
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

static int crypto_cbc_encrypt_segment(struct blkcipher_desc *desc,
				      struct blkcipher_walk *walk,
				      struct crypto_cipher *tfm)
{
	void (*fn)(struct crypto_tfm *, u8 *, const u8 *) =
		crypto_cipher_alg(tfm)->cia_encrypt;
	int bsize = crypto_cipher_blocksize(tfm);
	unsigned int nbytes = walk->nbytes;
	u8 *src = walk->src.virt.addr;
	u8 *dst = walk->dst.virt.addr;
	u8 *iv = walk->iv;

	do {
		crypto_xor(iv, src, bsize);
		fn(crypto_cipher_tfm(tfm), dst, iv);
		memcpy(iv, dst, bsize);

		src += bsize;
		dst += bsize;
	} while ((nbytes -= bsize) >= bsize);
=======

static int crypto_cbc_encrypt_segment(struct crypto_lskcipher *tfm,
				      const u8 *src, u8 *dst, unsigned nbytes,
				      u8 *iv)
{
	unsigned int bsize = crypto_lskcipher_blocksize(tfm);

	for (; nbytes >= bsize; src += bsize, dst += bsize, nbytes -= bsize) {
		crypto_xor(iv, src, bsize);
		crypto_lskcipher_encrypt(tfm, iv, dst, bsize, NULL);
		memcpy(iv, dst, bsize);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return nbytes;
}

<<<<<<< HEAD
static int crypto_cbc_encrypt_inplace(struct blkcipher_desc *desc,
				      struct blkcipher_walk *walk,
				      struct crypto_cipher *tfm)
{
	void (*fn)(struct crypto_tfm *, u8 *, const u8 *) =
		crypto_cipher_alg(tfm)->cia_encrypt;
	int bsize = crypto_cipher_blocksize(tfm);
	unsigned int nbytes = walk->nbytes;
	u8 *src = walk->src.virt.addr;
	u8 *iv = walk->iv;

	do {
		crypto_xor(src, iv, bsize);
		fn(crypto_cipher_tfm(tfm), src, src);
=======
static int crypto_cbc_encrypt_inplace(struct crypto_lskcipher *tfm,
				      u8 *src, unsigned nbytes, u8 *oiv)
{
	unsigned int bsize = crypto_lskcipher_blocksize(tfm);
	u8 *iv = oiv;

	if (nbytes < bsize)
		goto out;

	do {
		crypto_xor(src, iv, bsize);
		crypto_lskcipher_encrypt(tfm, src, src, bsize, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		iv = src;

		src += bsize;
	} while ((nbytes -= bsize) >= bsize);

<<<<<<< HEAD
	memcpy(walk->iv, iv, bsize);

	return nbytes;
}

static int crypto_cbc_encrypt(struct blkcipher_desc *desc,
			      struct scatterlist *dst, struct scatterlist *src,
			      unsigned int nbytes)
{
	struct blkcipher_walk walk;
	struct crypto_blkcipher *tfm = desc->tfm;
	struct crypto_cbc_ctx *ctx = crypto_blkcipher_ctx(tfm);
	struct crypto_cipher *child = ctx->child;
	int err;

	blkcipher_walk_init(&walk, dst, src, nbytes);
	err = blkcipher_walk_virt(desc, &walk);

	while ((nbytes = walk.nbytes)) {
		if (walk.src.virt.addr == walk.dst.virt.addr)
			nbytes = crypto_cbc_encrypt_inplace(desc, &walk, child);
		else
			nbytes = crypto_cbc_encrypt_segment(desc, &walk, child);
		err = blkcipher_walk_done(desc, &walk, nbytes);
	}

	return err;
}

static int crypto_cbc_decrypt_segment(struct blkcipher_desc *desc,
				      struct blkcipher_walk *walk,
				      struct crypto_cipher *tfm)
{
	void (*fn)(struct crypto_tfm *, u8 *, const u8 *) =
		crypto_cipher_alg(tfm)->cia_decrypt;
	int bsize = crypto_cipher_blocksize(tfm);
	unsigned int nbytes = walk->nbytes;
	u8 *src = walk->src.virt.addr;
	u8 *dst = walk->dst.virt.addr;
	u8 *iv = walk->iv;

	do {
		fn(crypto_cipher_tfm(tfm), dst, src);
=======
	memcpy(oiv, iv, bsize);

out:
	return nbytes;
}

static int crypto_cbc_encrypt(struct crypto_lskcipher *tfm, const u8 *src,
			      u8 *dst, unsigned len, u8 *iv, u32 flags)
{
	struct crypto_lskcipher **ctx = crypto_lskcipher_ctx(tfm);
	bool final = flags & CRYPTO_LSKCIPHER_FLAG_FINAL;
	struct crypto_lskcipher *cipher = *ctx;
	int rem;

	if (src == dst)
		rem = crypto_cbc_encrypt_inplace(cipher, dst, len, iv);
	else
		rem = crypto_cbc_encrypt_segment(cipher, src, dst, len, iv);

	return rem && final ? -EINVAL : rem;
}

static int crypto_cbc_decrypt_segment(struct crypto_lskcipher *tfm,
				      const u8 *src, u8 *dst, unsigned nbytes,
				      u8 *oiv)
{
	unsigned int bsize = crypto_lskcipher_blocksize(tfm);
	const u8 *iv = oiv;

	if (nbytes < bsize)
		goto out;

	do {
		crypto_lskcipher_decrypt(tfm, src, dst, bsize, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		crypto_xor(dst, iv, bsize);
		iv = src;

		src += bsize;
		dst += bsize;
	} while ((nbytes -= bsize) >= bsize);

<<<<<<< HEAD
	memcpy(walk->iv, iv, bsize);

	return nbytes;
}

static int crypto_cbc_decrypt_inplace(struct blkcipher_desc *desc,
				      struct blkcipher_walk *walk,
				      struct crypto_cipher *tfm)
{
	void (*fn)(struct crypto_tfm *, u8 *, const u8 *) =
		crypto_cipher_alg(tfm)->cia_decrypt;
	int bsize = crypto_cipher_blocksize(tfm);
	unsigned int nbytes = walk->nbytes;
	u8 *src = walk->src.virt.addr;
	u8 last_iv[bsize];
=======
	memcpy(oiv, iv, bsize);

out:
	return nbytes;
}

static int crypto_cbc_decrypt_inplace(struct crypto_lskcipher *tfm,
				      u8 *src, unsigned nbytes, u8 *iv)
{
	unsigned int bsize = crypto_lskcipher_blocksize(tfm);
	u8 last_iv[MAX_CIPHER_BLOCKSIZE];

	if (nbytes < bsize)
		goto out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Start of the last block. */
	src += nbytes - (nbytes & (bsize - 1)) - bsize;
	memcpy(last_iv, src, bsize);

	for (;;) {
<<<<<<< HEAD
		fn(crypto_cipher_tfm(tfm), src, src);
=======
		crypto_lskcipher_decrypt(tfm, src, src, bsize, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if ((nbytes -= bsize) < bsize)
			break;
		crypto_xor(src, src - bsize, bsize);
		src -= bsize;
	}

<<<<<<< HEAD
	crypto_xor(src, walk->iv, bsize);
	memcpy(walk->iv, last_iv, bsize);

	return nbytes;
}

static int crypto_cbc_decrypt(struct blkcipher_desc *desc,
			      struct scatterlist *dst, struct scatterlist *src,
			      unsigned int nbytes)
{
	struct blkcipher_walk walk;
	struct crypto_blkcipher *tfm = desc->tfm;
	struct crypto_cbc_ctx *ctx = crypto_blkcipher_ctx(tfm);
	struct crypto_cipher *child = ctx->child;
	int err;

	blkcipher_walk_init(&walk, dst, src, nbytes);
	err = blkcipher_walk_virt(desc, &walk);

	while ((nbytes = walk.nbytes)) {
		if (walk.src.virt.addr == walk.dst.virt.addr)
			nbytes = crypto_cbc_decrypt_inplace(desc, &walk, child);
		else
			nbytes = crypto_cbc_decrypt_segment(desc, &walk, child);
		err = blkcipher_walk_done(desc, &walk, nbytes);
=======
	crypto_xor(src, iv, bsize);
	memcpy(iv, last_iv, bsize);

out:
	return nbytes;
}

static int crypto_cbc_decrypt(struct crypto_lskcipher *tfm, const u8 *src,
			      u8 *dst, unsigned len, u8 *iv, u32 flags)
{
	struct crypto_lskcipher **ctx = crypto_lskcipher_ctx(tfm);
	bool final = flags & CRYPTO_LSKCIPHER_FLAG_FINAL;
	struct crypto_lskcipher *cipher = *ctx;
	int rem;

	if (src == dst)
		rem = crypto_cbc_decrypt_inplace(cipher, dst, len, iv);
	else
		rem = crypto_cbc_decrypt_segment(cipher, src, dst, len, iv);

	return rem && final ? -EINVAL : rem;
}

static int crypto_cbc_create(struct crypto_template *tmpl, struct rtattr **tb)
{
	struct lskcipher_instance *inst;
	int err;

	inst = lskcipher_alloc_instance_simple(tmpl, tb);
	if (IS_ERR(inst))
		return PTR_ERR(inst);

	err = -EINVAL;
	if (!is_power_of_2(inst->alg.co.base.cra_blocksize))
		goto out_free_inst;

	if (inst->alg.co.statesize)
		goto out_free_inst;

	inst->alg.encrypt = crypto_cbc_encrypt;
	inst->alg.decrypt = crypto_cbc_decrypt;

	err = lskcipher_register_instance(tmpl, inst);
	if (err) {
out_free_inst:
		inst->free(inst);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return err;
}

<<<<<<< HEAD
static int crypto_cbc_init_tfm(struct crypto_tfm *tfm)
{
	struct crypto_instance *inst = (void *)tfm->__crt_alg;
	struct crypto_spawn *spawn = crypto_instance_ctx(inst);
	struct crypto_cbc_ctx *ctx = crypto_tfm_ctx(tfm);
	struct crypto_cipher *cipher;

	cipher = crypto_spawn_cipher(spawn);
	if (IS_ERR(cipher))
		return PTR_ERR(cipher);

	ctx->child = cipher;
	return 0;
}

static void crypto_cbc_exit_tfm(struct crypto_tfm *tfm)
{
	struct crypto_cbc_ctx *ctx = crypto_tfm_ctx(tfm);
	crypto_free_cipher(ctx->child);
}

static struct crypto_instance *crypto_cbc_alloc(struct rtattr **tb)
{
	struct crypto_instance *inst;
	struct crypto_alg *alg;
	int err;

	err = crypto_check_attr_type(tb, CRYPTO_ALG_TYPE_BLKCIPHER);
	if (err)
		return ERR_PTR(err);

	alg = crypto_get_attr_alg(tb, CRYPTO_ALG_TYPE_CIPHER,
				  CRYPTO_ALG_TYPE_MASK);
	if (IS_ERR(alg))
		return ERR_CAST(alg);

	inst = ERR_PTR(-EINVAL);
	if (!is_power_of_2(alg->cra_blocksize))
		goto out_put_alg;

	inst = crypto_alloc_instance("cbc", alg);
	if (IS_ERR(inst))
		goto out_put_alg;

	inst->alg.cra_flags = CRYPTO_ALG_TYPE_BLKCIPHER;
	inst->alg.cra_priority = alg->cra_priority;
	inst->alg.cra_blocksize = alg->cra_blocksize;
	inst->alg.cra_alignmask = alg->cra_alignmask;
	inst->alg.cra_type = &crypto_blkcipher_type;

	/* We access the data as u32s when xoring. */
	inst->alg.cra_alignmask |= __alignof__(u32) - 1;

	inst->alg.cra_blkcipher.ivsize = alg->cra_blocksize;
	inst->alg.cra_blkcipher.min_keysize = alg->cra_cipher.cia_min_keysize;
	inst->alg.cra_blkcipher.max_keysize = alg->cra_cipher.cia_max_keysize;

	inst->alg.cra_ctxsize = sizeof(struct crypto_cbc_ctx);

	inst->alg.cra_init = crypto_cbc_init_tfm;
	inst->alg.cra_exit = crypto_cbc_exit_tfm;

	inst->alg.cra_blkcipher.setkey = crypto_cbc_setkey;
	inst->alg.cra_blkcipher.encrypt = crypto_cbc_encrypt;
	inst->alg.cra_blkcipher.decrypt = crypto_cbc_decrypt;

out_put_alg:
	crypto_mod_put(alg);
	return inst;
}

static void crypto_cbc_free(struct crypto_instance *inst)
{
	crypto_drop_spawn(crypto_instance_ctx(inst));
	kfree(inst);
}

static struct crypto_template crypto_cbc_tmpl = {
	.name = "cbc",
	.alloc = crypto_cbc_alloc,
	.free = crypto_cbc_free,
=======
static struct crypto_template crypto_cbc_tmpl = {
	.name = "cbc",
	.create = crypto_cbc_create,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.module = THIS_MODULE,
};

static int __init crypto_cbc_module_init(void)
{
	return crypto_register_template(&crypto_cbc_tmpl);
}

static void __exit crypto_cbc_module_exit(void)
{
	crypto_unregister_template(&crypto_cbc_tmpl);
}

<<<<<<< HEAD
module_init(crypto_cbc_module_init);
module_exit(crypto_cbc_module_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("CBC block cipher algorithm");
=======
subsys_initcall(crypto_cbc_module_init);
module_exit(crypto_cbc_module_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("CBC block cipher mode of operation");
MODULE_ALIAS_CRYPTO("cbc");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
