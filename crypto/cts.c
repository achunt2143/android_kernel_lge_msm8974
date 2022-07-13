/*
 * CTS: Cipher Text Stealing mode
 *
 * COPYRIGHT (c) 2008
 * The Regents of the University of Michigan
 * ALL RIGHTS RESERVED
 *
 * Permission is granted to use, copy, create derivative works
 * and redistribute this software and such derivative works
 * for any purpose, so long as the name of The University of
 * Michigan is not used in any advertising or publicity
 * pertaining to the use of distribution of this software
 * without specific, written prior authorization.  If the
 * above copyright notice or any other identification of the
 * University of Michigan is included in any copy of any
 * portion of this software, then the disclaimer below must
 * also be included.
 *
 * THIS SOFTWARE IS PROVIDED AS IS, WITHOUT REPRESENTATION
 * FROM THE UNIVERSITY OF MICHIGAN AS TO ITS FITNESS FOR ANY
 * PURPOSE, AND WITHOUT WARRANTY BY THE UNIVERSITY OF
 * MICHIGAN OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING
 * WITHOUT LIMITATION THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE
 * REGENTS OF THE UNIVERSITY OF MICHIGAN SHALL NOT BE LIABLE
 * FOR ANY DAMAGES, INCLUDING SPECIAL, INDIRECT, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, WITH RESPECT TO ANY CLAIM ARISING
 * OUT OF OR IN CONNECTION WITH THE USE OF THE SOFTWARE, EVEN
 * IF IT HAS BEEN OR IS HEREAFTER ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGES.
 */

/* Derived from various:
 *	Copyright (c) 2006 Herbert Xu <herbert@gondor.apana.org.au>
 */

/*
 * This is the Cipher Text Stealing mode as described by
 * Section 8 of rfc2040 and referenced by rfc3962.
 * rfc3962 includes errata information in its Appendix A.
 */

#include <crypto/algapi.h>
<<<<<<< HEAD
=======
#include <crypto/internal/skcipher.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/err.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/log2.h>
#include <linux/module.h>
#include <linux/scatterlist.h>
#include <crypto/scatterwalk.h>
#include <linux/slab.h>
<<<<<<< HEAD

struct crypto_cts_ctx {
	struct crypto_blkcipher *child;
};

static int crypto_cts_setkey(struct crypto_tfm *parent, const u8 *key,
			     unsigned int keylen)
{
	struct crypto_cts_ctx *ctx = crypto_tfm_ctx(parent);
	struct crypto_blkcipher *child = ctx->child;
	int err;

	crypto_blkcipher_clear_flags(child, CRYPTO_TFM_REQ_MASK);
	crypto_blkcipher_set_flags(child, crypto_tfm_get_flags(parent) &
				       CRYPTO_TFM_REQ_MASK);
	err = crypto_blkcipher_setkey(child, key, keylen);
	crypto_tfm_set_flags(parent, crypto_blkcipher_get_flags(child) &
				     CRYPTO_TFM_RES_MASK);
	return err;
}

static int cts_cbc_encrypt(struct crypto_cts_ctx *ctx,
			   struct blkcipher_desc *desc,
			   struct scatterlist *dst,
			   struct scatterlist *src,
			   unsigned int offset,
			   unsigned int nbytes)
{
	int bsize = crypto_blkcipher_blocksize(desc->tfm);
	u8 tmp[bsize], tmp2[bsize];
	struct blkcipher_desc lcldesc;
	struct scatterlist sgsrc[1], sgdst[1];
	int lastn = nbytes - bsize;
	u8 iv[bsize];
	u8 s[bsize * 2], d[bsize * 2];
	int err;

	if (lastn < 0)
		return -EINVAL;

	sg_init_table(sgsrc, 1);
	sg_init_table(sgdst, 1);

	memset(s, 0, sizeof(s));
	scatterwalk_map_and_copy(s, src, offset, nbytes, 0);

	memcpy(iv, desc->info, bsize);

	lcldesc.tfm = ctx->child;
	lcldesc.info = iv;
	lcldesc.flags = desc->flags;

	sg_set_buf(&sgsrc[0], s, bsize);
	sg_set_buf(&sgdst[0], tmp, bsize);
	err = crypto_blkcipher_encrypt_iv(&lcldesc, sgdst, sgsrc, bsize);

	memcpy(d + bsize, tmp, lastn);

	lcldesc.info = tmp;

	sg_set_buf(&sgsrc[0], s + bsize, bsize);
	sg_set_buf(&sgdst[0], tmp2, bsize);
	err = crypto_blkcipher_encrypt_iv(&lcldesc, sgdst, sgsrc, bsize);

	memcpy(d, tmp2, bsize);

	scatterwalk_map_and_copy(d, dst, offset, nbytes, 1);

	memcpy(desc->info, tmp2, bsize);

	return err;
}

static int crypto_cts_encrypt(struct blkcipher_desc *desc,
			      struct scatterlist *dst, struct scatterlist *src,
			      unsigned int nbytes)
{
	struct crypto_cts_ctx *ctx = crypto_blkcipher_ctx(desc->tfm);
	int bsize = crypto_blkcipher_blocksize(desc->tfm);
	int tot_blocks = (nbytes + bsize - 1) / bsize;
	int cbc_blocks = tot_blocks > 2 ? tot_blocks - 2 : 0;
	struct blkcipher_desc lcldesc;
	int err;

	lcldesc.tfm = ctx->child;
	lcldesc.info = desc->info;
	lcldesc.flags = desc->flags;

	if (tot_blocks == 1) {
		err = crypto_blkcipher_encrypt_iv(&lcldesc, dst, src, bsize);
	} else if (nbytes <= bsize * 2) {
		err = cts_cbc_encrypt(ctx, desc, dst, src, 0, nbytes);
	} else {
		/* do normal function for tot_blocks - 2 */
		err = crypto_blkcipher_encrypt_iv(&lcldesc, dst, src,
							cbc_blocks * bsize);
		if (err == 0) {
			/* do cts for final two blocks */
			err = cts_cbc_encrypt(ctx, desc, dst, src,
						cbc_blocks * bsize,
						nbytes - (cbc_blocks * bsize));
		}
	}

	return err;
}

static int cts_cbc_decrypt(struct crypto_cts_ctx *ctx,
			   struct blkcipher_desc *desc,
			   struct scatterlist *dst,
			   struct scatterlist *src,
			   unsigned int offset,
			   unsigned int nbytes)
{
	int bsize = crypto_blkcipher_blocksize(desc->tfm);
	u8 tmp[bsize];
	struct blkcipher_desc lcldesc;
	struct scatterlist sgsrc[1], sgdst[1];
	int lastn = nbytes - bsize;
	u8 iv[bsize];
	u8 s[bsize * 2], d[bsize * 2];
	int err;

	if (lastn < 0)
		return -EINVAL;

	sg_init_table(sgsrc, 1);
	sg_init_table(sgdst, 1);

	scatterwalk_map_and_copy(s, src, offset, nbytes, 0);

	lcldesc.tfm = ctx->child;
	lcldesc.info = iv;
	lcldesc.flags = desc->flags;

	/* 1. Decrypt Cn-1 (s) to create Dn (tmp)*/
	memset(iv, 0, sizeof(iv));
	sg_set_buf(&sgsrc[0], s, bsize);
	sg_set_buf(&sgdst[0], tmp, bsize);
	err = crypto_blkcipher_decrypt_iv(&lcldesc, sgdst, sgsrc, bsize);
	if (err)
		return err;
	/* 2. Pad Cn with zeros at the end to create C of length BB */
	memset(iv, 0, sizeof(iv));
	memcpy(iv, s + bsize, lastn);
	/* 3. Exclusive-or Dn (tmp) with C (iv) to create Xn (tmp) */
	crypto_xor(tmp, iv, bsize);
	/* 4. Select the first Ln bytes of Xn (tmp) to create Pn */
	memcpy(d + bsize, tmp, lastn);

	/* 5. Append the tail (BB - Ln) bytes of Xn (tmp) to Cn to create En */
	memcpy(s + bsize + lastn, tmp + lastn, bsize - lastn);
	/* 6. Decrypt En to create Pn-1 */
	memset(iv, 0, sizeof(iv));
	sg_set_buf(&sgsrc[0], s + bsize, bsize);
	sg_set_buf(&sgdst[0], d, bsize);
	err = crypto_blkcipher_decrypt_iv(&lcldesc, sgdst, sgsrc, bsize);

	/* XOR with previous block */
	crypto_xor(d, desc->info, bsize);

	scatterwalk_map_and_copy(d, dst, offset, nbytes, 1);

	memcpy(desc->info, s, bsize);
	return err;
}

static int crypto_cts_decrypt(struct blkcipher_desc *desc,
			      struct scatterlist *dst, struct scatterlist *src,
			      unsigned int nbytes)
{
	struct crypto_cts_ctx *ctx = crypto_blkcipher_ctx(desc->tfm);
	int bsize = crypto_blkcipher_blocksize(desc->tfm);
	int tot_blocks = (nbytes + bsize - 1) / bsize;
	int cbc_blocks = tot_blocks > 2 ? tot_blocks - 2 : 0;
	struct blkcipher_desc lcldesc;
	int err;

	lcldesc.tfm = ctx->child;
	lcldesc.info = desc->info;
	lcldesc.flags = desc->flags;

	if (tot_blocks == 1) {
		err = crypto_blkcipher_decrypt_iv(&lcldesc, dst, src, bsize);
	} else if (nbytes <= bsize * 2) {
		err = cts_cbc_decrypt(ctx, desc, dst, src, 0, nbytes);
	} else {
		/* do normal function for tot_blocks - 2 */
		err = crypto_blkcipher_decrypt_iv(&lcldesc, dst, src,
							cbc_blocks * bsize);
		if (err == 0) {
			/* do cts for final two blocks */
			err = cts_cbc_decrypt(ctx, desc, dst, src,
						cbc_blocks * bsize,
						nbytes - (cbc_blocks * bsize));
		}
	}
	return err;
}

static int crypto_cts_init_tfm(struct crypto_tfm *tfm)
{
	struct crypto_instance *inst = (void *)tfm->__crt_alg;
	struct crypto_spawn *spawn = crypto_instance_ctx(inst);
	struct crypto_cts_ctx *ctx = crypto_tfm_ctx(tfm);
	struct crypto_blkcipher *cipher;

	cipher = crypto_spawn_blkcipher(spawn);
=======
#include <linux/compiler.h>

struct crypto_cts_ctx {
	struct crypto_skcipher *child;
};

struct crypto_cts_reqctx {
	struct scatterlist sg[2];
	unsigned offset;
	struct skcipher_request subreq;
};

static inline u8 *crypto_cts_reqctx_space(struct skcipher_request *req)
{
	struct crypto_cts_reqctx *rctx = skcipher_request_ctx(req);
	struct crypto_skcipher *tfm = crypto_skcipher_reqtfm(req);
	struct crypto_cts_ctx *ctx = crypto_skcipher_ctx(tfm);
	struct crypto_skcipher *child = ctx->child;

	return PTR_ALIGN((u8 *)(rctx + 1) + crypto_skcipher_reqsize(child),
			 crypto_skcipher_alignmask(tfm) + 1);
}

static int crypto_cts_setkey(struct crypto_skcipher *parent, const u8 *key,
			     unsigned int keylen)
{
	struct crypto_cts_ctx *ctx = crypto_skcipher_ctx(parent);
	struct crypto_skcipher *child = ctx->child;

	crypto_skcipher_clear_flags(child, CRYPTO_TFM_REQ_MASK);
	crypto_skcipher_set_flags(child, crypto_skcipher_get_flags(parent) &
					 CRYPTO_TFM_REQ_MASK);
	return crypto_skcipher_setkey(child, key, keylen);
}

static void cts_cbc_crypt_done(void *data, int err)
{
	struct skcipher_request *req = data;

	if (err == -EINPROGRESS)
		return;

	skcipher_request_complete(req, err);
}

static int cts_cbc_encrypt(struct skcipher_request *req)
{
	struct crypto_cts_reqctx *rctx = skcipher_request_ctx(req);
	struct crypto_skcipher *tfm = crypto_skcipher_reqtfm(req);
	struct skcipher_request *subreq = &rctx->subreq;
	int bsize = crypto_skcipher_blocksize(tfm);
	u8 d[MAX_CIPHER_BLOCKSIZE * 2] __aligned(__alignof__(u32));
	struct scatterlist *sg;
	unsigned int offset;
	int lastn;

	offset = rctx->offset;
	lastn = req->cryptlen - offset;

	sg = scatterwalk_ffwd(rctx->sg, req->dst, offset - bsize);
	scatterwalk_map_and_copy(d + bsize, sg, 0, bsize, 0);

	memset(d, 0, bsize);
	scatterwalk_map_and_copy(d, req->src, offset, lastn, 0);

	scatterwalk_map_and_copy(d, sg, 0, bsize + lastn, 1);
	memzero_explicit(d, sizeof(d));

	skcipher_request_set_callback(subreq, req->base.flags &
					      CRYPTO_TFM_REQ_MAY_BACKLOG,
				      cts_cbc_crypt_done, req);
	skcipher_request_set_crypt(subreq, sg, sg, bsize, req->iv);
	return crypto_skcipher_encrypt(subreq);
}

static void crypto_cts_encrypt_done(void *data, int err)
{
	struct skcipher_request *req = data;

	if (err)
		goto out;

	err = cts_cbc_encrypt(req);
	if (err == -EINPROGRESS || err == -EBUSY)
		return;

out:
	skcipher_request_complete(req, err);
}

static int crypto_cts_encrypt(struct skcipher_request *req)
{
	struct crypto_skcipher *tfm = crypto_skcipher_reqtfm(req);
	struct crypto_cts_reqctx *rctx = skcipher_request_ctx(req);
	struct crypto_cts_ctx *ctx = crypto_skcipher_ctx(tfm);
	struct skcipher_request *subreq = &rctx->subreq;
	int bsize = crypto_skcipher_blocksize(tfm);
	unsigned int nbytes = req->cryptlen;
	unsigned int offset;

	skcipher_request_set_tfm(subreq, ctx->child);

	if (nbytes < bsize)
		return -EINVAL;

	if (nbytes == bsize) {
		skcipher_request_set_callback(subreq, req->base.flags,
					      req->base.complete,
					      req->base.data);
		skcipher_request_set_crypt(subreq, req->src, req->dst, nbytes,
					   req->iv);
		return crypto_skcipher_encrypt(subreq);
	}

	offset = rounddown(nbytes - 1, bsize);
	rctx->offset = offset;

	skcipher_request_set_callback(subreq, req->base.flags,
				      crypto_cts_encrypt_done, req);
	skcipher_request_set_crypt(subreq, req->src, req->dst,
				   offset, req->iv);

	return crypto_skcipher_encrypt(subreq) ?:
	       cts_cbc_encrypt(req);
}

static int cts_cbc_decrypt(struct skcipher_request *req)
{
	struct crypto_cts_reqctx *rctx = skcipher_request_ctx(req);
	struct crypto_skcipher *tfm = crypto_skcipher_reqtfm(req);
	struct skcipher_request *subreq = &rctx->subreq;
	int bsize = crypto_skcipher_blocksize(tfm);
	u8 d[MAX_CIPHER_BLOCKSIZE * 2] __aligned(__alignof__(u32));
	struct scatterlist *sg;
	unsigned int offset;
	u8 *space;
	int lastn;

	offset = rctx->offset;
	lastn = req->cryptlen - offset;

	sg = scatterwalk_ffwd(rctx->sg, req->dst, offset - bsize);

	/* 1. Decrypt Cn-1 (s) to create Dn */
	scatterwalk_map_and_copy(d + bsize, sg, 0, bsize, 0);
	space = crypto_cts_reqctx_space(req);
	crypto_xor(d + bsize, space, bsize);
	/* 2. Pad Cn with zeros at the end to create C of length BB */
	memset(d, 0, bsize);
	scatterwalk_map_and_copy(d, req->src, offset, lastn, 0);
	/* 3. Exclusive-or Dn with C to create Xn */
	/* 4. Select the first Ln bytes of Xn to create Pn */
	crypto_xor(d + bsize, d, lastn);

	/* 5. Append the tail (BB - Ln) bytes of Xn to Cn to create En */
	memcpy(d + lastn, d + bsize + lastn, bsize - lastn);
	/* 6. Decrypt En to create Pn-1 */

	scatterwalk_map_and_copy(d, sg, 0, bsize + lastn, 1);
	memzero_explicit(d, sizeof(d));

	skcipher_request_set_callback(subreq, req->base.flags &
					      CRYPTO_TFM_REQ_MAY_BACKLOG,
				      cts_cbc_crypt_done, req);

	skcipher_request_set_crypt(subreq, sg, sg, bsize, space);
	return crypto_skcipher_decrypt(subreq);
}

static void crypto_cts_decrypt_done(void *data, int err)
{
	struct skcipher_request *req = data;

	if (err)
		goto out;

	err = cts_cbc_decrypt(req);
	if (err == -EINPROGRESS || err == -EBUSY)
		return;

out:
	skcipher_request_complete(req, err);
}

static int crypto_cts_decrypt(struct skcipher_request *req)
{
	struct crypto_skcipher *tfm = crypto_skcipher_reqtfm(req);
	struct crypto_cts_reqctx *rctx = skcipher_request_ctx(req);
	struct crypto_cts_ctx *ctx = crypto_skcipher_ctx(tfm);
	struct skcipher_request *subreq = &rctx->subreq;
	int bsize = crypto_skcipher_blocksize(tfm);
	unsigned int nbytes = req->cryptlen;
	unsigned int offset;
	u8 *space;

	skcipher_request_set_tfm(subreq, ctx->child);

	if (nbytes < bsize)
		return -EINVAL;

	if (nbytes == bsize) {
		skcipher_request_set_callback(subreq, req->base.flags,
					      req->base.complete,
					      req->base.data);
		skcipher_request_set_crypt(subreq, req->src, req->dst, nbytes,
					   req->iv);
		return crypto_skcipher_decrypt(subreq);
	}

	skcipher_request_set_callback(subreq, req->base.flags,
				      crypto_cts_decrypt_done, req);

	space = crypto_cts_reqctx_space(req);

	offset = rounddown(nbytes - 1, bsize);
	rctx->offset = offset;

	if (offset <= bsize)
		memcpy(space, req->iv, bsize);
	else
		scatterwalk_map_and_copy(space, req->src, offset - 2 * bsize,
					 bsize, 0);

	skcipher_request_set_crypt(subreq, req->src, req->dst,
				   offset, req->iv);

	return crypto_skcipher_decrypt(subreq) ?:
	       cts_cbc_decrypt(req);
}

static int crypto_cts_init_tfm(struct crypto_skcipher *tfm)
{
	struct skcipher_instance *inst = skcipher_alg_instance(tfm);
	struct crypto_skcipher_spawn *spawn = skcipher_instance_ctx(inst);
	struct crypto_cts_ctx *ctx = crypto_skcipher_ctx(tfm);
	struct crypto_skcipher *cipher;
	unsigned reqsize;
	unsigned bsize;
	unsigned align;

	cipher = crypto_spawn_skcipher(spawn);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (IS_ERR(cipher))
		return PTR_ERR(cipher);

	ctx->child = cipher;
<<<<<<< HEAD
	return 0;
}

static void crypto_cts_exit_tfm(struct crypto_tfm *tfm)
{
	struct crypto_cts_ctx *ctx = crypto_tfm_ctx(tfm);
	crypto_free_blkcipher(ctx->child);
}

static struct crypto_instance *crypto_cts_alloc(struct rtattr **tb)
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

	inst = ERR_PTR(-EINVAL);
	if (!is_power_of_2(alg->cra_blocksize))
		goto out_put_alg;

	inst = crypto_alloc_instance("cts", alg);
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
	inst->alg.cra_blkcipher.min_keysize = alg->cra_blkcipher.min_keysize;
	inst->alg.cra_blkcipher.max_keysize = alg->cra_blkcipher.max_keysize;

	inst->alg.cra_blkcipher.geniv = "seqiv";

	inst->alg.cra_ctxsize = sizeof(struct crypto_cts_ctx);

	inst->alg.cra_init = crypto_cts_init_tfm;
	inst->alg.cra_exit = crypto_cts_exit_tfm;

	inst->alg.cra_blkcipher.setkey = crypto_cts_setkey;
	inst->alg.cra_blkcipher.encrypt = crypto_cts_encrypt;
	inst->alg.cra_blkcipher.decrypt = crypto_cts_decrypt;

out_put_alg:
	crypto_mod_put(alg);
	return inst;
}

static void crypto_cts_free(struct crypto_instance *inst)
{
	crypto_drop_spawn(crypto_instance_ctx(inst));
	kfree(inst);
=======

	align = crypto_skcipher_alignmask(tfm);
	bsize = crypto_skcipher_blocksize(cipher);
	reqsize = ALIGN(sizeof(struct crypto_cts_reqctx) +
			crypto_skcipher_reqsize(cipher),
			crypto_tfm_ctx_alignment()) +
		  (align & ~(crypto_tfm_ctx_alignment() - 1)) + bsize;

	crypto_skcipher_set_reqsize(tfm, reqsize);

	return 0;
}

static void crypto_cts_exit_tfm(struct crypto_skcipher *tfm)
{
	struct crypto_cts_ctx *ctx = crypto_skcipher_ctx(tfm);

	crypto_free_skcipher(ctx->child);
}

static void crypto_cts_free(struct skcipher_instance *inst)
{
	crypto_drop_skcipher(skcipher_instance_ctx(inst));
	kfree(inst);
}

static int crypto_cts_create(struct crypto_template *tmpl, struct rtattr **tb)
{
	struct crypto_skcipher_spawn *spawn;
	struct skcipher_alg_common *alg;
	struct skcipher_instance *inst;
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

	err = -EINVAL;
	if (alg->ivsize != alg->base.cra_blocksize)
		goto err_free_inst;

	if (strncmp(alg->base.cra_name, "cbc(", 4))
		goto err_free_inst;

	err = crypto_inst_setname(skcipher_crypto_instance(inst), "cts",
				  &alg->base);
	if (err)
		goto err_free_inst;

	inst->alg.base.cra_priority = alg->base.cra_priority;
	inst->alg.base.cra_blocksize = alg->base.cra_blocksize;
	inst->alg.base.cra_alignmask = alg->base.cra_alignmask;

	inst->alg.ivsize = alg->base.cra_blocksize;
	inst->alg.chunksize = alg->chunksize;
	inst->alg.min_keysize = alg->min_keysize;
	inst->alg.max_keysize = alg->max_keysize;

	inst->alg.base.cra_ctxsize = sizeof(struct crypto_cts_ctx);

	inst->alg.init = crypto_cts_init_tfm;
	inst->alg.exit = crypto_cts_exit_tfm;

	inst->alg.setkey = crypto_cts_setkey;
	inst->alg.encrypt = crypto_cts_encrypt;
	inst->alg.decrypt = crypto_cts_decrypt;

	inst->free = crypto_cts_free;

	err = skcipher_register_instance(tmpl, inst);
	if (err) {
err_free_inst:
		crypto_cts_free(inst);
	}
	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct crypto_template crypto_cts_tmpl = {
	.name = "cts",
<<<<<<< HEAD
	.alloc = crypto_cts_alloc,
	.free = crypto_cts_free,
=======
	.create = crypto_cts_create,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.module = THIS_MODULE,
};

static int __init crypto_cts_module_init(void)
{
	return crypto_register_template(&crypto_cts_tmpl);
}

static void __exit crypto_cts_module_exit(void)
{
	crypto_unregister_template(&crypto_cts_tmpl);
}

<<<<<<< HEAD
module_init(crypto_cts_module_init);
=======
subsys_initcall(crypto_cts_module_init);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
module_exit(crypto_cts_module_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("CTS-CBC CipherText Stealing for CBC");
<<<<<<< HEAD
=======
MODULE_ALIAS_CRYPTO("cts");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
