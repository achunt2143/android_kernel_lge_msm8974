<<<<<<< HEAD
/*
 * Copyright (C)2006 USAGI/WIDE Project
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C)2006 USAGI/WIDE Project
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Author:
 * 	Kazunori Miyazawa <miyazawa@linux-ipv6.org>
 */

<<<<<<< HEAD
=======
#include <crypto/internal/cipher.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <crypto/internal/hash.h>
#include <linux/err.h>
#include <linux/kernel.h>
#include <linux/module.h>

static u_int32_t ks[12] = {0x01010101, 0x01010101, 0x01010101, 0x01010101,
			   0x02020202, 0x02020202, 0x02020202, 0x02020202,
			   0x03030303, 0x03030303, 0x03030303, 0x03030303};

/*
 * +------------------------
 * | <parent tfm>
 * +------------------------
 * | xcbc_tfm_ctx
 * +------------------------
 * | consts (block size * 2)
 * +------------------------
 */
struct xcbc_tfm_ctx {
	struct crypto_cipher *child;
<<<<<<< HEAD
	u8 ctx[];
=======
	u8 consts[];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/*
 * +------------------------
 * | <shash desc>
 * +------------------------
 * | xcbc_desc_ctx
 * +------------------------
 * | odds (block size)
 * +------------------------
 * | prev (block size)
 * +------------------------
 */
struct xcbc_desc_ctx {
	unsigned int len;
<<<<<<< HEAD
	u8 ctx[];
};

static int crypto_xcbc_digest_setkey(struct crypto_shash *parent,
				     const u8 *inkey, unsigned int keylen)
{
	unsigned long alignmask = crypto_shash_alignmask(parent);
	struct xcbc_tfm_ctx *ctx = crypto_shash_ctx(parent);
	int bs = crypto_shash_blocksize(parent);
	u8 *consts = PTR_ALIGN(&ctx->ctx[0], alignmask + 1);
	int err = 0;
	u8 key1[bs];
=======
	u8 odds[];
};

#define XCBC_BLOCKSIZE	16

static int crypto_xcbc_digest_setkey(struct crypto_shash *parent,
				     const u8 *inkey, unsigned int keylen)
{
	struct xcbc_tfm_ctx *ctx = crypto_shash_ctx(parent);
	u8 *consts = ctx->consts;
	int err = 0;
	u8 key1[XCBC_BLOCKSIZE];
	int bs = sizeof(key1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if ((err = crypto_cipher_setkey(ctx->child, inkey, keylen)))
		return err;

	crypto_cipher_encrypt_one(ctx->child, consts, (u8 *)ks + bs);
	crypto_cipher_encrypt_one(ctx->child, consts + bs, (u8 *)ks + bs * 2);
	crypto_cipher_encrypt_one(ctx->child, key1, (u8 *)ks);

	return crypto_cipher_setkey(ctx->child, key1, bs);

}

static int crypto_xcbc_digest_init(struct shash_desc *pdesc)
{
<<<<<<< HEAD
	unsigned long alignmask = crypto_shash_alignmask(pdesc->tfm);
	struct xcbc_desc_ctx *ctx = shash_desc_ctx(pdesc);
	int bs = crypto_shash_blocksize(pdesc->tfm);
	u8 *prev = PTR_ALIGN(&ctx->ctx[0], alignmask + 1) + bs;
=======
	struct xcbc_desc_ctx *ctx = shash_desc_ctx(pdesc);
	int bs = crypto_shash_blocksize(pdesc->tfm);
	u8 *prev = &ctx->odds[bs];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ctx->len = 0;
	memset(prev, 0, bs);

	return 0;
}

static int crypto_xcbc_digest_update(struct shash_desc *pdesc, const u8 *p,
				     unsigned int len)
{
	struct crypto_shash *parent = pdesc->tfm;
<<<<<<< HEAD
	unsigned long alignmask = crypto_shash_alignmask(parent);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct xcbc_tfm_ctx *tctx = crypto_shash_ctx(parent);
	struct xcbc_desc_ctx *ctx = shash_desc_ctx(pdesc);
	struct crypto_cipher *tfm = tctx->child;
	int bs = crypto_shash_blocksize(parent);
<<<<<<< HEAD
	u8 *odds = PTR_ALIGN(&ctx->ctx[0], alignmask + 1);
=======
	u8 *odds = ctx->odds;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u8 *prev = odds + bs;

	/* checking the data can fill the block */
	if ((ctx->len + len) <= bs) {
		memcpy(odds + ctx->len, p, len);
		ctx->len += len;
		return 0;
	}

	/* filling odds with new data and encrypting it */
	memcpy(odds + ctx->len, p, bs - ctx->len);
	len -= bs - ctx->len;
	p += bs - ctx->len;

	crypto_xor(prev, odds, bs);
	crypto_cipher_encrypt_one(tfm, prev, prev);

	/* clearing the length */
	ctx->len = 0;

	/* encrypting the rest of data */
	while (len > bs) {
		crypto_xor(prev, p, bs);
		crypto_cipher_encrypt_one(tfm, prev, prev);
		p += bs;
		len -= bs;
	}

	/* keeping the surplus of blocksize */
	if (len) {
		memcpy(odds, p, len);
		ctx->len = len;
	}

	return 0;
}

static int crypto_xcbc_digest_final(struct shash_desc *pdesc, u8 *out)
{
	struct crypto_shash *parent = pdesc->tfm;
<<<<<<< HEAD
	unsigned long alignmask = crypto_shash_alignmask(parent);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct xcbc_tfm_ctx *tctx = crypto_shash_ctx(parent);
	struct xcbc_desc_ctx *ctx = shash_desc_ctx(pdesc);
	struct crypto_cipher *tfm = tctx->child;
	int bs = crypto_shash_blocksize(parent);
<<<<<<< HEAD
	u8 *consts = PTR_ALIGN(&tctx->ctx[0], alignmask + 1);
	u8 *odds = PTR_ALIGN(&ctx->ctx[0], alignmask + 1);
=======
	u8 *odds = ctx->odds;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u8 *prev = odds + bs;
	unsigned int offset = 0;

	if (ctx->len != bs) {
		unsigned int rlen;
		u8 *p = odds + ctx->len;

		*p = 0x80;
		p++;

		rlen = bs - ctx->len -1;
		if (rlen)
			memset(p, 0, rlen);

		offset += bs;
	}

	crypto_xor(prev, odds, bs);
<<<<<<< HEAD
	crypto_xor(prev, consts + offset, bs);
=======
	crypto_xor(prev, &tctx->consts[offset], bs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	crypto_cipher_encrypt_one(tfm, out, prev);

	return 0;
}

static int xcbc_init_tfm(struct crypto_tfm *tfm)
{
	struct crypto_cipher *cipher;
	struct crypto_instance *inst = (void *)tfm->__crt_alg;
<<<<<<< HEAD
	struct crypto_spawn *spawn = crypto_instance_ctx(inst);
=======
	struct crypto_cipher_spawn *spawn = crypto_instance_ctx(inst);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct xcbc_tfm_ctx *ctx = crypto_tfm_ctx(tfm);

	cipher = crypto_spawn_cipher(spawn);
	if (IS_ERR(cipher))
		return PTR_ERR(cipher);

	ctx->child = cipher;

	return 0;
};

static void xcbc_exit_tfm(struct crypto_tfm *tfm)
{
	struct xcbc_tfm_ctx *ctx = crypto_tfm_ctx(tfm);
	crypto_free_cipher(ctx->child);
}

static int xcbc_create(struct crypto_template *tmpl, struct rtattr **tb)
{
	struct shash_instance *inst;
<<<<<<< HEAD
	struct crypto_alg *alg;
	unsigned long alignmask;
	int err;

	err = crypto_check_attr_type(tb, CRYPTO_ALG_TYPE_SHASH);
	if (err)
		return err;

	alg = crypto_get_attr_alg(tb, CRYPTO_ALG_TYPE_CIPHER,
				  CRYPTO_ALG_TYPE_MASK);
	if (IS_ERR(alg))
		return PTR_ERR(alg);

	switch(alg->cra_blocksize) {
	case 16:
		break;
	default:
		goto out_put_alg;
	}

	inst = shash_alloc_instance("xcbc", alg);
	err = PTR_ERR(inst);
	if (IS_ERR(inst))
		goto out_put_alg;

	err = crypto_init_spawn(shash_instance_ctx(inst), alg,
				shash_crypto_instance(inst),
				CRYPTO_ALG_TYPE_MASK);
	if (err)
		goto out_free_inst;

	alignmask = alg->cra_alignmask | 3;
	inst->alg.base.cra_alignmask = alignmask;
	inst->alg.base.cra_priority = alg->cra_priority;
	inst->alg.base.cra_blocksize = alg->cra_blocksize;

	inst->alg.digestsize = alg->cra_blocksize;
	inst->alg.descsize = ALIGN(sizeof(struct xcbc_desc_ctx),
				   crypto_tfm_ctx_alignment()) +
			     (alignmask &
			      ~(crypto_tfm_ctx_alignment() - 1)) +
			     alg->cra_blocksize * 2;

	inst->alg.base.cra_ctxsize = ALIGN(sizeof(struct xcbc_tfm_ctx),
					   alignmask + 1) +
				     alg->cra_blocksize * 2;
=======
	struct crypto_cipher_spawn *spawn;
	struct crypto_alg *alg;
	u32 mask;
	int err;

	err = crypto_check_attr_type(tb, CRYPTO_ALG_TYPE_SHASH, &mask);
	if (err)
		return err;

	inst = kzalloc(sizeof(*inst) + sizeof(*spawn), GFP_KERNEL);
	if (!inst)
		return -ENOMEM;
	spawn = shash_instance_ctx(inst);

	err = crypto_grab_cipher(spawn, shash_crypto_instance(inst),
				 crypto_attr_alg_name(tb[1]), 0, mask);
	if (err)
		goto err_free_inst;
	alg = crypto_spawn_cipher_alg(spawn);

	err = -EINVAL;
	if (alg->cra_blocksize != XCBC_BLOCKSIZE)
		goto err_free_inst;

	err = crypto_inst_setname(shash_crypto_instance(inst), tmpl->name, alg);
	if (err)
		goto err_free_inst;

	inst->alg.base.cra_priority = alg->cra_priority;
	inst->alg.base.cra_blocksize = alg->cra_blocksize;
	inst->alg.base.cra_ctxsize = sizeof(struct xcbc_tfm_ctx) +
				     alg->cra_blocksize * 2;

	inst->alg.digestsize = alg->cra_blocksize;
	inst->alg.descsize = sizeof(struct xcbc_desc_ctx) +
			     alg->cra_blocksize * 2;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	inst->alg.base.cra_init = xcbc_init_tfm;
	inst->alg.base.cra_exit = xcbc_exit_tfm;

	inst->alg.init = crypto_xcbc_digest_init;
	inst->alg.update = crypto_xcbc_digest_update;
	inst->alg.final = crypto_xcbc_digest_final;
	inst->alg.setkey = crypto_xcbc_digest_setkey;

<<<<<<< HEAD
	err = shash_register_instance(tmpl, inst);
	if (err) {
out_free_inst:
		shash_free_instance(shash_crypto_instance(inst));
	}

out_put_alg:
	crypto_mod_put(alg);
=======
	inst->free = shash_free_singlespawn_instance;

	err = shash_register_instance(tmpl, inst);
	if (err) {
err_free_inst:
		shash_free_singlespawn_instance(inst);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

static struct crypto_template crypto_xcbc_tmpl = {
	.name = "xcbc",
	.create = xcbc_create,
<<<<<<< HEAD
	.free = shash_free_instance,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.module = THIS_MODULE,
};

static int __init crypto_xcbc_module_init(void)
{
	return crypto_register_template(&crypto_xcbc_tmpl);
}

static void __exit crypto_xcbc_module_exit(void)
{
	crypto_unregister_template(&crypto_xcbc_tmpl);
}

<<<<<<< HEAD
module_init(crypto_xcbc_module_init);
=======
subsys_initcall(crypto_xcbc_module_init);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
module_exit(crypto_xcbc_module_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("XCBC keyed hash algorithm");
<<<<<<< HEAD
=======
MODULE_ALIAS_CRYPTO("xcbc");
MODULE_IMPORT_NS(CRYPTO_INTERNAL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
