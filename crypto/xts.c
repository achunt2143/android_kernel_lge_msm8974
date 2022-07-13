<<<<<<< HEAD
/* XTS: as defined in IEEE1619/D16
 *	http://grouper.ieee.org/groups/1619/email/pdf00086.pdf
 *	(sector sizes which are not a multiple of 16 bytes are,
 *	however currently unsupported)
 *
 * Copyright (c) 2007 Rik Snel <rsnel@cube.dyndns.org>
 *
 * Based om ecb.c
 * Copyright (c) 2006 Herbert Xu <herbert@gondor.apana.org.au>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 */
#include <crypto/algapi.h>
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/* XTS: as defined in IEEE1619/D16
 *	http://grouper.ieee.org/groups/1619/email/pdf00086.pdf
 *
 * Copyright (c) 2007 Rik Snel <rsnel@cube.dyndns.org>
 *
 * Based on ecb.c
 * Copyright (c) 2006 Herbert Xu <herbert@gondor.apana.org.au>
 */
#include <crypto/internal/cipher.h>
#include <crypto/internal/skcipher.h>
#include <crypto/scatterwalk.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/err.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/scatterlist.h>
#include <linux/slab.h>

#include <crypto/xts.h>
#include <crypto/b128ops.h>
#include <crypto/gf128mul.h>

<<<<<<< HEAD
struct priv {
	struct crypto_cipher *child;
	struct crypto_cipher *tweak;
};

static int setkey(struct crypto_tfm *parent, const u8 *key,
		  unsigned int keylen)
{
	struct priv *ctx = crypto_tfm_ctx(parent);
	struct crypto_cipher *child = ctx->tweak;
	u32 *flags = &parent->crt_flags;
	int err;

	/* key consists of keys of equal size concatenated, therefore
	 * the length must be even */
	if (keylen % 2) {
		/* tell the user why there was an error */
		*flags |= CRYPTO_TFM_RES_BAD_KEY_LEN;
		return -EINVAL;
	}
=======
struct xts_tfm_ctx {
	struct crypto_skcipher *child;
	struct crypto_cipher *tweak;
};

struct xts_instance_ctx {
	struct crypto_skcipher_spawn spawn;
	struct crypto_cipher_spawn tweak_spawn;
};

struct xts_request_ctx {
	le128 t;
	struct scatterlist *tail;
	struct scatterlist sg[2];
	struct skcipher_request subreq;
};

static int xts_setkey(struct crypto_skcipher *parent, const u8 *key,
		      unsigned int keylen)
{
	struct xts_tfm_ctx *ctx = crypto_skcipher_ctx(parent);
	struct crypto_skcipher *child;
	struct crypto_cipher *tweak;
	int err;

	err = xts_verify_key(parent, key, keylen);
	if (err)
		return err;

	keylen /= 2;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* we need two cipher instances: one to compute the initial 'tweak'
	 * by encrypting the IV (usually the 'plain' iv) and the other
	 * one to encrypt and decrypt the data */

	/* tweak cipher, uses Key2 i.e. the second half of *key */
<<<<<<< HEAD
	crypto_cipher_clear_flags(child, CRYPTO_TFM_REQ_MASK);
	crypto_cipher_set_flags(child, crypto_tfm_get_flags(parent) &
				       CRYPTO_TFM_REQ_MASK);
	err = crypto_cipher_setkey(child, key + keylen/2, keylen/2);
	if (err)
		return err;

	crypto_tfm_set_flags(parent, crypto_cipher_get_flags(child) &
				     CRYPTO_TFM_RES_MASK);

	child = ctx->child;

	/* data cipher, uses Key1 i.e. the first half of *key */
	crypto_cipher_clear_flags(child, CRYPTO_TFM_REQ_MASK);
	crypto_cipher_set_flags(child, crypto_tfm_get_flags(parent) &
				       CRYPTO_TFM_REQ_MASK);
	err = crypto_cipher_setkey(child, key, keylen/2);
	if (err)
		return err;

	crypto_tfm_set_flags(parent, crypto_cipher_get_flags(child) &
				     CRYPTO_TFM_RES_MASK);
=======
	tweak = ctx->tweak;
	crypto_cipher_clear_flags(tweak, CRYPTO_TFM_REQ_MASK);
	crypto_cipher_set_flags(tweak, crypto_skcipher_get_flags(parent) &
				       CRYPTO_TFM_REQ_MASK);
	err = crypto_cipher_setkey(tweak, key + keylen, keylen);
	if (err)
		return err;

	/* data cipher, uses Key1 i.e. the first half of *key */
	child = ctx->child;
	crypto_skcipher_clear_flags(child, CRYPTO_TFM_REQ_MASK);
	crypto_skcipher_set_flags(child, crypto_skcipher_get_flags(parent) &
					 CRYPTO_TFM_REQ_MASK);
	return crypto_skcipher_setkey(child, key, keylen);
}

/*
 * We compute the tweak masks twice (both before and after the ECB encryption or
 * decryption) to avoid having to allocate a temporary buffer and/or make
 * mutliple calls to the 'ecb(..)' instance, which usually would be slower than
 * just doing the gf128mul_x_ble() calls again.
 */
static int xts_xor_tweak(struct skcipher_request *req, bool second_pass,
			 bool enc)
{
	struct xts_request_ctx *rctx = skcipher_request_ctx(req);
	struct crypto_skcipher *tfm = crypto_skcipher_reqtfm(req);
	const bool cts = (req->cryptlen % XTS_BLOCK_SIZE);
	const int bs = XTS_BLOCK_SIZE;
	struct skcipher_walk w;
	le128 t = rctx->t;
	int err;

	if (second_pass) {
		req = &rctx->subreq;
		/* set to our TFM to enforce correct alignment: */
		skcipher_request_set_tfm(req, tfm);
	}
	err = skcipher_walk_virt(&w, req, false);

	while (w.nbytes) {
		unsigned int avail = w.nbytes;
		le128 *wsrc;
		le128 *wdst;

		wsrc = w.src.virt.addr;
		wdst = w.dst.virt.addr;

		do {
			if (unlikely(cts) &&
			    w.total - w.nbytes + avail < 2 * XTS_BLOCK_SIZE) {
				if (!enc) {
					if (second_pass)
						rctx->t = t;
					gf128mul_x_ble(&t, &t);
				}
				le128_xor(wdst, &t, wsrc);
				if (enc && second_pass)
					gf128mul_x_ble(&rctx->t, &t);
				skcipher_walk_done(&w, avail - bs);
				return 0;
			}

			le128_xor(wdst++, &t, wsrc++);
			gf128mul_x_ble(&t, &t);
		} while ((avail -= bs) >= bs);

		err = skcipher_walk_done(&w, avail);
	}

	return err;
}

static int xts_xor_tweak_pre(struct skcipher_request *req, bool enc)
{
	return xts_xor_tweak(req, false, enc);
}

static int xts_xor_tweak_post(struct skcipher_request *req, bool enc)
{
	return xts_xor_tweak(req, true, enc);
}

static void xts_cts_done(void *data, int err)
{
	struct skcipher_request *req = data;
	le128 b;

	if (!err) {
		struct xts_request_ctx *rctx = skcipher_request_ctx(req);

		scatterwalk_map_and_copy(&b, rctx->tail, 0, XTS_BLOCK_SIZE, 0);
		le128_xor(&b, &rctx->t, &b);
		scatterwalk_map_and_copy(&b, rctx->tail, 0, XTS_BLOCK_SIZE, 1);
	}

	skcipher_request_complete(req, err);
}

static int xts_cts_final(struct skcipher_request *req,
			 int (*crypt)(struct skcipher_request *req))
{
	const struct xts_tfm_ctx *ctx =
		crypto_skcipher_ctx(crypto_skcipher_reqtfm(req));
	int offset = req->cryptlen & ~(XTS_BLOCK_SIZE - 1);
	struct xts_request_ctx *rctx = skcipher_request_ctx(req);
	struct skcipher_request *subreq = &rctx->subreq;
	int tail = req->cryptlen % XTS_BLOCK_SIZE;
	le128 b[2];
	int err;

	rctx->tail = scatterwalk_ffwd(rctx->sg, req->dst,
				      offset - XTS_BLOCK_SIZE);

	scatterwalk_map_and_copy(b, rctx->tail, 0, XTS_BLOCK_SIZE, 0);
	b[1] = b[0];
	scatterwalk_map_and_copy(b, req->src, offset, tail, 0);

	le128_xor(b, &rctx->t, b);

	scatterwalk_map_and_copy(b, rctx->tail, 0, XTS_BLOCK_SIZE + tail, 1);

	skcipher_request_set_tfm(subreq, ctx->child);
	skcipher_request_set_callback(subreq, req->base.flags, xts_cts_done,
				      req);
	skcipher_request_set_crypt(subreq, rctx->tail, rctx->tail,
				   XTS_BLOCK_SIZE, NULL);

	err = crypt(subreq);
	if (err)
		return err;

	scatterwalk_map_and_copy(b, rctx->tail, 0, XTS_BLOCK_SIZE, 0);
	le128_xor(b, &rctx->t, b);
	scatterwalk_map_and_copy(b, rctx->tail, 0, XTS_BLOCK_SIZE, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
struct sinfo {
	be128 *t;
	struct crypto_tfm *tfm;
	void (*fn)(struct crypto_tfm *, u8 *, const u8 *);
};

static inline void xts_round(struct sinfo *s, void *dst, const void *src)
{
	be128_xor(dst, s->t, src);		/* PP <- T xor P */
	s->fn(s->tfm, dst, dst);		/* CC <- E(Key1,PP) */
	be128_xor(dst, dst, s->t);		/* C <- T xor CC */
}

static int crypt(struct blkcipher_desc *d,
		 struct blkcipher_walk *w, struct priv *ctx,
		 void (*tw)(struct crypto_tfm *, u8 *, const u8 *),
		 void (*fn)(struct crypto_tfm *, u8 *, const u8 *))
{
	int err;
	unsigned int avail;
	const int bs = XTS_BLOCK_SIZE;
	struct sinfo s = {
		.tfm = crypto_cipher_tfm(ctx->child),
		.fn = fn
	};
	u8 *wsrc;
	u8 *wdst;

	err = blkcipher_walk_virt(d, w);
	if (!w->nbytes)
		return err;

	s.t = (be128 *)w->iv;
	avail = w->nbytes;

	wsrc = w->src.virt.addr;
	wdst = w->dst.virt.addr;

	/* calculate first value of T */
	tw(crypto_cipher_tfm(ctx->tweak), w->iv, w->iv);

	goto first;

	for (;;) {
		do {
			gf128mul_x_ble(s.t, s.t);

first:
			xts_round(&s, wdst, wsrc);

			wsrc += bs;
			wdst += bs;
		} while ((avail -= bs) >= bs);

		err = blkcipher_walk_done(d, w, avail);
		if (!w->nbytes)
			break;

		avail = w->nbytes;

		wsrc = w->src.virt.addr;
		wdst = w->dst.virt.addr;
	}

	return err;
}

static int encrypt(struct blkcipher_desc *desc, struct scatterlist *dst,
		   struct scatterlist *src, unsigned int nbytes)
{
	struct priv *ctx = crypto_blkcipher_ctx(desc->tfm);
	struct blkcipher_walk w;

	blkcipher_walk_init(&w, dst, src, nbytes);
	return crypt(desc, &w, ctx, crypto_cipher_alg(ctx->tweak)->cia_encrypt,
		     crypto_cipher_alg(ctx->child)->cia_encrypt);
}

static int decrypt(struct blkcipher_desc *desc, struct scatterlist *dst,
		   struct scatterlist *src, unsigned int nbytes)
{
	struct priv *ctx = crypto_blkcipher_ctx(desc->tfm);
	struct blkcipher_walk w;

	blkcipher_walk_init(&w, dst, src, nbytes);
	return crypt(desc, &w, ctx, crypto_cipher_alg(ctx->tweak)->cia_encrypt,
		     crypto_cipher_alg(ctx->child)->cia_decrypt);
}

int xts_crypt(struct blkcipher_desc *desc, struct scatterlist *sdst,
	      struct scatterlist *ssrc, unsigned int nbytes,
	      struct xts_crypt_req *req)
{
	const unsigned int bsize = XTS_BLOCK_SIZE;
	const unsigned int max_blks = req->tbuflen / bsize;
	struct blkcipher_walk walk;
	unsigned int nblocks;
	be128 *src, *dst, *t;
	be128 *t_buf = req->tbuf;
	int err, i;

	BUG_ON(max_blks < 1);

	blkcipher_walk_init(&walk, sdst, ssrc, nbytes);

	err = blkcipher_walk_virt(desc, &walk);
	nbytes = walk.nbytes;
	if (!nbytes)
		return err;

	nblocks = min(nbytes / bsize, max_blks);
	src = (be128 *)walk.src.virt.addr;
	dst = (be128 *)walk.dst.virt.addr;

	/* calculate first value of T */
	req->tweak_fn(req->tweak_ctx, (u8 *)&t_buf[0], walk.iv);

	i = 0;
	goto first;

	for (;;) {
		do {
			for (i = 0; i < nblocks; i++) {
				gf128mul_x_ble(&t_buf[i], t);
first:
				t = &t_buf[i];

				/* PP <- T xor P */
				be128_xor(dst + i, t, src + i);
			}

			/* CC <- E(Key2,PP) */
			req->crypt_fn(req->crypt_ctx, (u8 *)dst,
				      nblocks * bsize);

			/* C <- T xor CC */
			for (i = 0; i < nblocks; i++)
				be128_xor(dst + i, dst + i, &t_buf[i]);

			src += nblocks;
			dst += nblocks;
			nbytes -= nblocks * bsize;
			nblocks = min(nbytes / bsize, max_blks);
		} while (nblocks > 0);

		*(be128 *)walk.iv = *t;

		err = blkcipher_walk_done(desc, &walk, nbytes);
		nbytes = walk.nbytes;
		if (!nbytes)
			break;

		nblocks = min(nbytes / bsize, max_blks);
		src = (be128 *)walk.src.virt.addr;
		dst = (be128 *)walk.dst.virt.addr;
	}

	return err;
}
EXPORT_SYMBOL_GPL(xts_crypt);

static int init_tfm(struct crypto_tfm *tfm)
{
	struct crypto_cipher *cipher;
	struct crypto_instance *inst = (void *)tfm->__crt_alg;
	struct crypto_spawn *spawn = crypto_instance_ctx(inst);
	struct priv *ctx = crypto_tfm_ctx(tfm);
	u32 *flags = &tfm->crt_flags;

	cipher = crypto_spawn_cipher(spawn);
	if (IS_ERR(cipher))
		return PTR_ERR(cipher);

	if (crypto_cipher_blocksize(cipher) != XTS_BLOCK_SIZE) {
		*flags |= CRYPTO_TFM_RES_BAD_BLOCK_LEN;
		crypto_free_cipher(cipher);
		return -EINVAL;
	}

	ctx->child = cipher;

	cipher = crypto_spawn_cipher(spawn);
	if (IS_ERR(cipher)) {
		crypto_free_cipher(ctx->child);
		return PTR_ERR(cipher);
	}

	/* this check isn't really needed, leave it here just in case */
	if (crypto_cipher_blocksize(cipher) != XTS_BLOCK_SIZE) {
		crypto_free_cipher(cipher);
		crypto_free_cipher(ctx->child);
		*flags |= CRYPTO_TFM_RES_BAD_BLOCK_LEN;
		return -EINVAL;
	}

	ctx->tweak = cipher;
=======
static void xts_encrypt_done(void *data, int err)
{
	struct skcipher_request *req = data;

	if (!err) {
		struct xts_request_ctx *rctx = skcipher_request_ctx(req);

		rctx->subreq.base.flags &= CRYPTO_TFM_REQ_MAY_BACKLOG;
		err = xts_xor_tweak_post(req, true);

		if (!err && unlikely(req->cryptlen % XTS_BLOCK_SIZE)) {
			err = xts_cts_final(req, crypto_skcipher_encrypt);
			if (err == -EINPROGRESS || err == -EBUSY)
				return;
		}
	}

	skcipher_request_complete(req, err);
}

static void xts_decrypt_done(void *data, int err)
{
	struct skcipher_request *req = data;

	if (!err) {
		struct xts_request_ctx *rctx = skcipher_request_ctx(req);

		rctx->subreq.base.flags &= CRYPTO_TFM_REQ_MAY_BACKLOG;
		err = xts_xor_tweak_post(req, false);

		if (!err && unlikely(req->cryptlen % XTS_BLOCK_SIZE)) {
			err = xts_cts_final(req, crypto_skcipher_decrypt);
			if (err == -EINPROGRESS || err == -EBUSY)
				return;
		}
	}

	skcipher_request_complete(req, err);
}

static int xts_init_crypt(struct skcipher_request *req,
			  crypto_completion_t compl)
{
	const struct xts_tfm_ctx *ctx =
		crypto_skcipher_ctx(crypto_skcipher_reqtfm(req));
	struct xts_request_ctx *rctx = skcipher_request_ctx(req);
	struct skcipher_request *subreq = &rctx->subreq;

	if (req->cryptlen < XTS_BLOCK_SIZE)
		return -EINVAL;

	skcipher_request_set_tfm(subreq, ctx->child);
	skcipher_request_set_callback(subreq, req->base.flags, compl, req);
	skcipher_request_set_crypt(subreq, req->dst, req->dst,
				   req->cryptlen & ~(XTS_BLOCK_SIZE - 1), NULL);

	/* calculate first value of T */
	crypto_cipher_encrypt_one(ctx->tweak, (u8 *)&rctx->t, req->iv);

	return 0;
}

static int xts_encrypt(struct skcipher_request *req)
{
	struct xts_request_ctx *rctx = skcipher_request_ctx(req);
	struct skcipher_request *subreq = &rctx->subreq;
	int err;

	err = xts_init_crypt(req, xts_encrypt_done) ?:
	      xts_xor_tweak_pre(req, true) ?:
	      crypto_skcipher_encrypt(subreq) ?:
	      xts_xor_tweak_post(req, true);

	if (err || likely((req->cryptlen % XTS_BLOCK_SIZE) == 0))
		return err;

	return xts_cts_final(req, crypto_skcipher_encrypt);
}

static int xts_decrypt(struct skcipher_request *req)
{
	struct xts_request_ctx *rctx = skcipher_request_ctx(req);
	struct skcipher_request *subreq = &rctx->subreq;
	int err;

	err = xts_init_crypt(req, xts_decrypt_done) ?:
	      xts_xor_tweak_pre(req, false) ?:
	      crypto_skcipher_decrypt(subreq) ?:
	      xts_xor_tweak_post(req, false);

	if (err || likely((req->cryptlen % XTS_BLOCK_SIZE) == 0))
		return err;

	return xts_cts_final(req, crypto_skcipher_decrypt);
}

static int xts_init_tfm(struct crypto_skcipher *tfm)
{
	struct skcipher_instance *inst = skcipher_alg_instance(tfm);
	struct xts_instance_ctx *ictx = skcipher_instance_ctx(inst);
	struct xts_tfm_ctx *ctx = crypto_skcipher_ctx(tfm);
	struct crypto_skcipher *child;
	struct crypto_cipher *tweak;

	child = crypto_spawn_skcipher(&ictx->spawn);
	if (IS_ERR(child))
		return PTR_ERR(child);

	ctx->child = child;

	tweak = crypto_spawn_cipher(&ictx->tweak_spawn);
	if (IS_ERR(tweak)) {
		crypto_free_skcipher(ctx->child);
		return PTR_ERR(tweak);
	}

	ctx->tweak = tweak;

	crypto_skcipher_set_reqsize(tfm, crypto_skcipher_reqsize(child) +
					 sizeof(struct xts_request_ctx));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static void exit_tfm(struct crypto_tfm *tfm)
{
	struct priv *ctx = crypto_tfm_ctx(tfm);
	crypto_free_cipher(ctx->child);
	crypto_free_cipher(ctx->tweak);
}

static struct crypto_instance *alloc(struct rtattr **tb)
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

	inst = crypto_alloc_instance("xts", alg);
	if (IS_ERR(inst))
		goto out_put_alg;

	inst->alg.cra_flags = CRYPTO_ALG_TYPE_BLKCIPHER;
	inst->alg.cra_priority = alg->cra_priority;
	inst->alg.cra_blocksize = alg->cra_blocksize;

	if (alg->cra_alignmask < 7)
		inst->alg.cra_alignmask = 7;
	else
		inst->alg.cra_alignmask = alg->cra_alignmask;

	inst->alg.cra_type = &crypto_blkcipher_type;

	inst->alg.cra_blkcipher.ivsize = alg->cra_blocksize;
	inst->alg.cra_blkcipher.min_keysize =
		2 * alg->cra_cipher.cia_min_keysize;
	inst->alg.cra_blkcipher.max_keysize =
		2 * alg->cra_cipher.cia_max_keysize;

	inst->alg.cra_ctxsize = sizeof(struct priv);

	inst->alg.cra_init = init_tfm;
	inst->alg.cra_exit = exit_tfm;

	inst->alg.cra_blkcipher.setkey = setkey;
	inst->alg.cra_blkcipher.encrypt = encrypt;
	inst->alg.cra_blkcipher.decrypt = decrypt;

out_put_alg:
	crypto_mod_put(alg);
	return inst;
}

static void free(struct crypto_instance *inst)
{
	crypto_drop_spawn(crypto_instance_ctx(inst));
	kfree(inst);
}

static struct crypto_template crypto_tmpl = {
	.name = "xts",
	.alloc = alloc,
	.free = free,
	.module = THIS_MODULE,
};

static int __init crypto_module_init(void)
{
	return crypto_register_template(&crypto_tmpl);
}

static void __exit crypto_module_exit(void)
{
	crypto_unregister_template(&crypto_tmpl);
}

module_init(crypto_module_init);
module_exit(crypto_module_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("XTS block cipher mode");
=======
static void xts_exit_tfm(struct crypto_skcipher *tfm)
{
	struct xts_tfm_ctx *ctx = crypto_skcipher_ctx(tfm);

	crypto_free_skcipher(ctx->child);
	crypto_free_cipher(ctx->tweak);
}

static void xts_free_instance(struct skcipher_instance *inst)
{
	struct xts_instance_ctx *ictx = skcipher_instance_ctx(inst);

	crypto_drop_skcipher(&ictx->spawn);
	crypto_drop_cipher(&ictx->tweak_spawn);
	kfree(inst);
}

static int xts_create(struct crypto_template *tmpl, struct rtattr **tb)
{
	struct skcipher_alg_common *alg;
	char name[CRYPTO_MAX_ALG_NAME];
	struct skcipher_instance *inst;
	struct xts_instance_ctx *ctx;
	const char *cipher_name;
	u32 mask;
	int err;

	err = crypto_check_attr_type(tb, CRYPTO_ALG_TYPE_SKCIPHER, &mask);
	if (err)
		return err;

	cipher_name = crypto_attr_alg_name(tb[1]);
	if (IS_ERR(cipher_name))
		return PTR_ERR(cipher_name);

	inst = kzalloc(sizeof(*inst) + sizeof(*ctx), GFP_KERNEL);
	if (!inst)
		return -ENOMEM;

	ctx = skcipher_instance_ctx(inst);

	err = crypto_grab_skcipher(&ctx->spawn, skcipher_crypto_instance(inst),
				   cipher_name, 0, mask);
	if (err == -ENOENT) {
		err = -ENAMETOOLONG;
		if (snprintf(name, CRYPTO_MAX_ALG_NAME, "ecb(%s)",
			     cipher_name) >= CRYPTO_MAX_ALG_NAME)
			goto err_free_inst;

		err = crypto_grab_skcipher(&ctx->spawn,
					   skcipher_crypto_instance(inst),
					   name, 0, mask);
	}

	if (err)
		goto err_free_inst;

	alg = crypto_spawn_skcipher_alg_common(&ctx->spawn);

	err = -EINVAL;
	if (alg->base.cra_blocksize != XTS_BLOCK_SIZE)
		goto err_free_inst;

	if (alg->ivsize)
		goto err_free_inst;

	err = crypto_inst_setname(skcipher_crypto_instance(inst), "xts",
				  &alg->base);
	if (err)
		goto err_free_inst;

	err = -EINVAL;
	cipher_name = alg->base.cra_name;

	/* Alas we screwed up the naming so we have to mangle the
	 * cipher name.
	 */
	if (!strncmp(cipher_name, "ecb(", 4)) {
		int len;

		len = strscpy(name, cipher_name + 4, sizeof(name));
		if (len < 2)
			goto err_free_inst;

		if (name[len - 1] != ')')
			goto err_free_inst;

		name[len - 1] = 0;

		if (snprintf(inst->alg.base.cra_name, CRYPTO_MAX_ALG_NAME,
			     "xts(%s)", name) >= CRYPTO_MAX_ALG_NAME) {
			err = -ENAMETOOLONG;
			goto err_free_inst;
		}
	} else
		goto err_free_inst;

	err = crypto_grab_cipher(&ctx->tweak_spawn,
				 skcipher_crypto_instance(inst), name, 0, mask);
	if (err)
		goto err_free_inst;

	inst->alg.base.cra_priority = alg->base.cra_priority;
	inst->alg.base.cra_blocksize = XTS_BLOCK_SIZE;
	inst->alg.base.cra_alignmask = alg->base.cra_alignmask |
				       (__alignof__(u64) - 1);

	inst->alg.ivsize = XTS_BLOCK_SIZE;
	inst->alg.min_keysize = alg->min_keysize * 2;
	inst->alg.max_keysize = alg->max_keysize * 2;

	inst->alg.base.cra_ctxsize = sizeof(struct xts_tfm_ctx);

	inst->alg.init = xts_init_tfm;
	inst->alg.exit = xts_exit_tfm;

	inst->alg.setkey = xts_setkey;
	inst->alg.encrypt = xts_encrypt;
	inst->alg.decrypt = xts_decrypt;

	inst->free = xts_free_instance;

	err = skcipher_register_instance(tmpl, inst);
	if (err) {
err_free_inst:
		xts_free_instance(inst);
	}
	return err;
}

static struct crypto_template xts_tmpl = {
	.name = "xts",
	.create = xts_create,
	.module = THIS_MODULE,
};

static int __init xts_module_init(void)
{
	return crypto_register_template(&xts_tmpl);
}

static void __exit xts_module_exit(void)
{
	crypto_unregister_template(&xts_tmpl);
}

subsys_initcall(xts_module_init);
module_exit(xts_module_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("XTS block cipher mode");
MODULE_ALIAS_CRYPTO("xts");
MODULE_IMPORT_NS(CRYPTO_INTERNAL);
MODULE_SOFTDEP("pre: ecb");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
