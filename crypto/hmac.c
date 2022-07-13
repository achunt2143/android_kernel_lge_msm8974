<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Cryptographic API.
 *
 * HMAC: Keyed-Hashing for Message Authentication (RFC2104).
 *
 * Copyright (c) 2002 James Morris <jmorris@intercode.com.au>
 * Copyright (c) 2006 Herbert Xu <herbert@gondor.apana.org.au>
 *
 * The HMAC implementation is derived from USAGI.
 * Copyright (c) 2002 Kazunori Miyazawa <miyazawa@linux-ipv6.org> / USAGI
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 */

#include <crypto/internal/hash.h>
#include <crypto/scatterwalk.h>
#include <linux/err.h>
=======
 */

#include <crypto/hmac.h>
#include <crypto/internal/hash.h>
#include <crypto/scatterwalk.h>
#include <linux/err.h>
#include <linux/fips.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/scatterlist.h>
#include <linux/string.h>

struct hmac_ctx {
	struct crypto_shash *hash;
<<<<<<< HEAD
};

static inline void *align_ptr(void *p, unsigned int align)
{
	return (void *)ALIGN((unsigned long)p, align);
}

static inline struct hmac_ctx *hmac_ctx(struct crypto_shash *tfm)
{
	return align_ptr(crypto_shash_ctx_aligned(tfm) +
			 crypto_shash_statesize(tfm) * 2,
			 crypto_tfm_ctx_alignment());
}

=======
	/* Contains 'u8 ipad[statesize];', then 'u8 opad[statesize];' */
	u8 pads[];
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int hmac_setkey(struct crypto_shash *parent,
		       const u8 *inkey, unsigned int keylen)
{
	int bs = crypto_shash_blocksize(parent);
	int ds = crypto_shash_digestsize(parent);
	int ss = crypto_shash_statesize(parent);
<<<<<<< HEAD
	char *ipad = crypto_shash_ctx_aligned(parent);
	char *opad = ipad + ss;
	struct hmac_ctx *ctx = align_ptr(opad + ss,
					 crypto_tfm_ctx_alignment());
	struct crypto_shash *hash = ctx->hash;
	struct {
		struct shash_desc shash;
		char ctx[crypto_shash_descsize(hash)];
	} desc;
	unsigned int i;

	desc.shash.tfm = hash;
	desc.shash.flags = crypto_shash_get_flags(parent) &
			    CRYPTO_TFM_REQ_MAY_SLEEP;
=======
	struct hmac_ctx *tctx = crypto_shash_ctx(parent);
	struct crypto_shash *hash = tctx->hash;
	u8 *ipad = &tctx->pads[0];
	u8 *opad = &tctx->pads[ss];
	SHASH_DESC_ON_STACK(shash, hash);
	unsigned int i;

	if (fips_enabled && (keylen < 112 / 8))
		return -EINVAL;

	shash->tfm = hash;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (keylen > bs) {
		int err;

<<<<<<< HEAD
		err = crypto_shash_digest(&desc.shash, inkey, keylen, ipad);
=======
		err = crypto_shash_digest(shash, inkey, keylen, ipad);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (err)
			return err;

		keylen = ds;
	} else
		memcpy(ipad, inkey, keylen);

	memset(ipad + keylen, 0, bs - keylen);
	memcpy(opad, ipad, bs);

	for (i = 0; i < bs; i++) {
<<<<<<< HEAD
		ipad[i] ^= 0x36;
		opad[i] ^= 0x5c;
	}

	return crypto_shash_init(&desc.shash) ?:
	       crypto_shash_update(&desc.shash, ipad, bs) ?:
	       crypto_shash_export(&desc.shash, ipad) ?:
	       crypto_shash_init(&desc.shash) ?:
	       crypto_shash_update(&desc.shash, opad, bs) ?:
	       crypto_shash_export(&desc.shash, opad);
=======
		ipad[i] ^= HMAC_IPAD_VALUE;
		opad[i] ^= HMAC_OPAD_VALUE;
	}

	return crypto_shash_init(shash) ?:
	       crypto_shash_update(shash, ipad, bs) ?:
	       crypto_shash_export(shash, ipad) ?:
	       crypto_shash_init(shash) ?:
	       crypto_shash_update(shash, opad, bs) ?:
	       crypto_shash_export(shash, opad);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int hmac_export(struct shash_desc *pdesc, void *out)
{
	struct shash_desc *desc = shash_desc_ctx(pdesc);

<<<<<<< HEAD
	desc->flags = pdesc->flags & CRYPTO_TFM_REQ_MAY_SLEEP;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return crypto_shash_export(desc, out);
}

static int hmac_import(struct shash_desc *pdesc, const void *in)
{
	struct shash_desc *desc = shash_desc_ctx(pdesc);
<<<<<<< HEAD
	struct hmac_ctx *ctx = hmac_ctx(pdesc->tfm);

	desc->tfm = ctx->hash;
	desc->flags = pdesc->flags & CRYPTO_TFM_REQ_MAY_SLEEP;
=======
	const struct hmac_ctx *tctx = crypto_shash_ctx(pdesc->tfm);

	desc->tfm = tctx->hash;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return crypto_shash_import(desc, in);
}

static int hmac_init(struct shash_desc *pdesc)
{
<<<<<<< HEAD
	return hmac_import(pdesc, crypto_shash_ctx_aligned(pdesc->tfm));
=======
	const struct hmac_ctx *tctx = crypto_shash_ctx(pdesc->tfm);

	return hmac_import(pdesc, &tctx->pads[0]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int hmac_update(struct shash_desc *pdesc,
		       const u8 *data, unsigned int nbytes)
{
	struct shash_desc *desc = shash_desc_ctx(pdesc);

<<<<<<< HEAD
	desc->flags = pdesc->flags & CRYPTO_TFM_REQ_MAY_SLEEP;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return crypto_shash_update(desc, data, nbytes);
}

static int hmac_final(struct shash_desc *pdesc, u8 *out)
{
	struct crypto_shash *parent = pdesc->tfm;
	int ds = crypto_shash_digestsize(parent);
	int ss = crypto_shash_statesize(parent);
<<<<<<< HEAD
	char *opad = crypto_shash_ctx_aligned(parent) + ss;
	struct shash_desc *desc = shash_desc_ctx(pdesc);

	desc->flags = pdesc->flags & CRYPTO_TFM_REQ_MAY_SLEEP;

=======
	const struct hmac_ctx *tctx = crypto_shash_ctx(parent);
	const u8 *opad = &tctx->pads[ss];
	struct shash_desc *desc = shash_desc_ctx(pdesc);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return crypto_shash_final(desc, out) ?:
	       crypto_shash_import(desc, opad) ?:
	       crypto_shash_finup(desc, out, ds, out);
}

static int hmac_finup(struct shash_desc *pdesc, const u8 *data,
		      unsigned int nbytes, u8 *out)
{

	struct crypto_shash *parent = pdesc->tfm;
	int ds = crypto_shash_digestsize(parent);
	int ss = crypto_shash_statesize(parent);
<<<<<<< HEAD
	char *opad = crypto_shash_ctx_aligned(parent) + ss;
	struct shash_desc *desc = shash_desc_ctx(pdesc);

	desc->flags = pdesc->flags & CRYPTO_TFM_REQ_MAY_SLEEP;

=======
	const struct hmac_ctx *tctx = crypto_shash_ctx(parent);
	const u8 *opad = &tctx->pads[ss];
	struct shash_desc *desc = shash_desc_ctx(pdesc);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return crypto_shash_finup(desc, data, nbytes, out) ?:
	       crypto_shash_import(desc, opad) ?:
	       crypto_shash_finup(desc, out, ds, out);
}

<<<<<<< HEAD
static int hmac_init_tfm(struct crypto_tfm *tfm)
{
	struct crypto_shash *parent = __crypto_shash_cast(tfm);
	struct crypto_shash *hash;
	struct crypto_instance *inst = (void *)tfm->__crt_alg;
	struct crypto_shash_spawn *spawn = crypto_instance_ctx(inst);
	struct hmac_ctx *ctx = hmac_ctx(parent);
=======
static int hmac_init_tfm(struct crypto_shash *parent)
{
	struct crypto_shash *hash;
	struct shash_instance *inst = shash_alg_instance(parent);
	struct crypto_shash_spawn *spawn = shash_instance_ctx(inst);
	struct hmac_ctx *tctx = crypto_shash_ctx(parent);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	hash = crypto_spawn_shash(spawn);
	if (IS_ERR(hash))
		return PTR_ERR(hash);

	parent->descsize = sizeof(struct shash_desc) +
			   crypto_shash_descsize(hash);

<<<<<<< HEAD
	ctx->hash = hash;
	return 0;
}

static void hmac_exit_tfm(struct crypto_tfm *tfm)
{
	struct hmac_ctx *ctx = hmac_ctx(__crypto_shash_cast(tfm));
	crypto_free_shash(ctx->hash);
=======
	tctx->hash = hash;
	return 0;
}

static int hmac_clone_tfm(struct crypto_shash *dst, struct crypto_shash *src)
{
	struct hmac_ctx *sctx = crypto_shash_ctx(src);
	struct hmac_ctx *dctx = crypto_shash_ctx(dst);
	struct crypto_shash *hash;

	hash = crypto_clone_shash(sctx->hash);
	if (IS_ERR(hash))
		return PTR_ERR(hash);

	dctx->hash = hash;
	return 0;
}

static void hmac_exit_tfm(struct crypto_shash *parent)
{
	struct hmac_ctx *tctx = crypto_shash_ctx(parent);

	crypto_free_shash(tctx->hash);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int hmac_create(struct crypto_template *tmpl, struct rtattr **tb)
{
	struct shash_instance *inst;
<<<<<<< HEAD
	struct crypto_alg *alg;
	struct shash_alg *salg;
=======
	struct crypto_shash_spawn *spawn;
	struct crypto_alg *alg;
	struct shash_alg *salg;
	u32 mask;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int err;
	int ds;
	int ss;

<<<<<<< HEAD
	err = crypto_check_attr_type(tb, CRYPTO_ALG_TYPE_SHASH);
	if (err)
		return err;

	salg = shash_attr_alg(tb[1], 0, 0);
	if (IS_ERR(salg))
		return PTR_ERR(salg);
	alg = &salg->base;

	/* The underlying hash algorithm must be unkeyed */
	err = -EINVAL;
	if (crypto_shash_alg_has_setkey(salg))
		goto out_put_alg;
=======
	err = crypto_check_attr_type(tb, CRYPTO_ALG_TYPE_SHASH, &mask);
	if (err)
		return err;

	inst = kzalloc(sizeof(*inst) + sizeof(*spawn), GFP_KERNEL);
	if (!inst)
		return -ENOMEM;
	spawn = shash_instance_ctx(inst);

	err = crypto_grab_shash(spawn, shash_crypto_instance(inst),
				crypto_attr_alg_name(tb[1]), 0, mask);
	if (err)
		goto err_free_inst;
	salg = crypto_spawn_shash_alg(spawn);
	alg = &salg->base;

	/* The underlying hash algorithm must not require a key */
	err = -EINVAL;
	if (crypto_shash_alg_needs_key(salg))
		goto err_free_inst;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ds = salg->digestsize;
	ss = salg->statesize;
	if (ds > alg->cra_blocksize ||
	    ss < alg->cra_blocksize)
<<<<<<< HEAD
		goto out_put_alg;

	inst = shash_alloc_instance("hmac", alg);
	err = PTR_ERR(inst);
	if (IS_ERR(inst))
		goto out_put_alg;

	err = crypto_init_shash_spawn(shash_instance_ctx(inst), salg,
				      shash_crypto_instance(inst));
	if (err)
		goto out_free_inst;

	inst->alg.base.cra_priority = alg->cra_priority;
	inst->alg.base.cra_blocksize = alg->cra_blocksize;
	inst->alg.base.cra_alignmask = alg->cra_alignmask;

	ss = ALIGN(ss, alg->cra_alignmask + 1);
	inst->alg.digestsize = ds;
	inst->alg.statesize = ss;

	inst->alg.base.cra_ctxsize = sizeof(struct hmac_ctx) +
				     ALIGN(ss * 2, crypto_tfm_ctx_alignment());

	inst->alg.base.cra_init = hmac_init_tfm;
	inst->alg.base.cra_exit = hmac_exit_tfm;

=======
		goto err_free_inst;

	err = crypto_inst_setname(shash_crypto_instance(inst), tmpl->name, alg);
	if (err)
		goto err_free_inst;

	inst->alg.base.cra_priority = alg->cra_priority;
	inst->alg.base.cra_blocksize = alg->cra_blocksize;
	inst->alg.base.cra_ctxsize = sizeof(struct hmac_ctx) + (ss * 2);

	inst->alg.digestsize = ds;
	inst->alg.statesize = ss;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	inst->alg.init = hmac_init;
	inst->alg.update = hmac_update;
	inst->alg.final = hmac_final;
	inst->alg.finup = hmac_finup;
	inst->alg.export = hmac_export;
	inst->alg.import = hmac_import;
	inst->alg.setkey = hmac_setkey;
<<<<<<< HEAD

	err = shash_register_instance(tmpl, inst);
	if (err) {
out_free_inst:
		shash_free_instance(shash_crypto_instance(inst));
	}

out_put_alg:
	crypto_mod_put(alg);
=======
	inst->alg.init_tfm = hmac_init_tfm;
	inst->alg.clone_tfm = hmac_clone_tfm;
	inst->alg.exit_tfm = hmac_exit_tfm;

	inst->free = shash_free_singlespawn_instance;

	err = shash_register_instance(tmpl, inst);
	if (err) {
err_free_inst:
		shash_free_singlespawn_instance(inst);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

static struct crypto_template hmac_tmpl = {
	.name = "hmac",
	.create = hmac_create,
<<<<<<< HEAD
	.free = shash_free_instance,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.module = THIS_MODULE,
};

static int __init hmac_module_init(void)
{
	return crypto_register_template(&hmac_tmpl);
}

static void __exit hmac_module_exit(void)
{
	crypto_unregister_template(&hmac_tmpl);
}

<<<<<<< HEAD
module_init(hmac_module_init);
=======
subsys_initcall(hmac_module_init);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
module_exit(hmac_module_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("HMAC hash algorithm");
<<<<<<< HEAD
=======
MODULE_ALIAS_CRYPTO("hmac");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
