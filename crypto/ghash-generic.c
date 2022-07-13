<<<<<<< HEAD
/*
 * GHASH: digest algorithm for GCM (Galois/Counter Mode).
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * GHASH: hash function for GCM (Galois/Counter Mode).
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Copyright (c) 2007 Nokia Siemens Networks - Mikko Herranen <mh1@iki.fi>
 * Copyright (c) 2009 Intel Corp.
 *   Author: Huang Ying <ying.huang@intel.com>
<<<<<<< HEAD
 *
 * The algorithm implementation is copied from gcm.c.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
=======
 */

/*
 * GHASH is a keyed hash function used in GCM authentication tag generation.
 *
 * The original GCM paper [1] presents GHASH as a function GHASH(H, A, C) which
 * takes a 16-byte hash key H, additional authenticated data A, and a ciphertext
 * C.  It formats A and C into a single byte string X, interprets X as a
 * polynomial over GF(2^128), and evaluates this polynomial at the point H.
 *
 * However, the NIST standard for GCM [2] presents GHASH as GHASH(H, X) where X
 * is the already-formatted byte string containing both A and C.
 *
 * "ghash" in the Linux crypto API uses the 'X' (pre-formatted) convention,
 * since the API supports only a single data stream per hash.  Thus, the
 * formatting of 'A' and 'C' is done in the "gcm" template, not in "ghash".
 *
 * The reason "ghash" is separate from "gcm" is to allow "gcm" to use an
 * accelerated "ghash" when a standalone accelerated "gcm(aes)" is unavailable.
 * It is generally inappropriate to use "ghash" for other purposes, since it is
 * an "ε-almost-XOR-universal hash function", not a cryptographic hash function.
 * It can only be used securely in crypto modes specially designed to use it.
 *
 * [1] The Galois/Counter Mode of Operation (GCM)
 *     (http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.694.695&rep=rep1&type=pdf)
 * [2] Recommendation for Block Cipher Modes of Operation: Galois/Counter Mode (GCM) and GMAC
 *     (https://csrc.nist.gov/publications/detail/sp/800-38d/final)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <crypto/algapi.h>
#include <crypto/gf128mul.h>
<<<<<<< HEAD
=======
#include <crypto/ghash.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <crypto/internal/hash.h>
#include <linux/crypto.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

<<<<<<< HEAD
#define GHASH_BLOCK_SIZE	16
#define GHASH_DIGEST_SIZE	16

struct ghash_ctx {
	struct gf128mul_4k *gf128;
};

struct ghash_desc_ctx {
	u8 buffer[GHASH_BLOCK_SIZE];
	u32 bytes;
};

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int ghash_init(struct shash_desc *desc)
{
	struct ghash_desc_ctx *dctx = shash_desc_ctx(desc);

	memset(dctx, 0, sizeof(*dctx));

	return 0;
}

static int ghash_setkey(struct crypto_shash *tfm,
			const u8 *key, unsigned int keylen)
{
	struct ghash_ctx *ctx = crypto_shash_ctx(tfm);
<<<<<<< HEAD

	if (keylen != GHASH_BLOCK_SIZE) {
		crypto_shash_set_flags(tfm, CRYPTO_TFM_RES_BAD_KEY_LEN);
		return -EINVAL;
	}

	if (ctx->gf128)
		gf128mul_free_4k(ctx->gf128);
	ctx->gf128 = gf128mul_init_4k_lle((be128 *)key);
=======
	be128 k;

	if (keylen != GHASH_BLOCK_SIZE)
		return -EINVAL;

	if (ctx->gf128)
		gf128mul_free_4k(ctx->gf128);

	BUILD_BUG_ON(sizeof(k) != GHASH_BLOCK_SIZE);
	memcpy(&k, key, GHASH_BLOCK_SIZE); /* avoid violating alignment rules */
	ctx->gf128 = gf128mul_init_4k_lle(&k);
	memzero_explicit(&k, GHASH_BLOCK_SIZE);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!ctx->gf128)
		return -ENOMEM;

	return 0;
}

static int ghash_update(struct shash_desc *desc,
			 const u8 *src, unsigned int srclen)
{
	struct ghash_desc_ctx *dctx = shash_desc_ctx(desc);
	struct ghash_ctx *ctx = crypto_shash_ctx(desc->tfm);
	u8 *dst = dctx->buffer;

<<<<<<< HEAD
	if (!ctx->gf128)
		return -ENOKEY;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (dctx->bytes) {
		int n = min(srclen, dctx->bytes);
		u8 *pos = dst + (GHASH_BLOCK_SIZE - dctx->bytes);

		dctx->bytes -= n;
		srclen -= n;

		while (n--)
			*pos++ ^= *src++;

		if (!dctx->bytes)
			gf128mul_4k_lle((be128 *)dst, ctx->gf128);
	}

	while (srclen >= GHASH_BLOCK_SIZE) {
		crypto_xor(dst, src, GHASH_BLOCK_SIZE);
		gf128mul_4k_lle((be128 *)dst, ctx->gf128);
		src += GHASH_BLOCK_SIZE;
		srclen -= GHASH_BLOCK_SIZE;
	}

	if (srclen) {
		dctx->bytes = GHASH_BLOCK_SIZE - srclen;
		while (srclen--)
			*dst++ ^= *src++;
	}

	return 0;
}

static void ghash_flush(struct ghash_ctx *ctx, struct ghash_desc_ctx *dctx)
{
	u8 *dst = dctx->buffer;

	if (dctx->bytes) {
		u8 *tmp = dst + (GHASH_BLOCK_SIZE - dctx->bytes);

		while (dctx->bytes--)
			*tmp++ ^= 0;

		gf128mul_4k_lle((be128 *)dst, ctx->gf128);
	}

	dctx->bytes = 0;
}

static int ghash_final(struct shash_desc *desc, u8 *dst)
{
	struct ghash_desc_ctx *dctx = shash_desc_ctx(desc);
	struct ghash_ctx *ctx = crypto_shash_ctx(desc->tfm);
	u8 *buf = dctx->buffer;

<<<<<<< HEAD
	if (!ctx->gf128)
		return -ENOKEY;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ghash_flush(ctx, dctx);
	memcpy(dst, buf, GHASH_BLOCK_SIZE);

	return 0;
}

static void ghash_exit_tfm(struct crypto_tfm *tfm)
{
	struct ghash_ctx *ctx = crypto_tfm_ctx(tfm);
	if (ctx->gf128)
		gf128mul_free_4k(ctx->gf128);
}

static struct shash_alg ghash_alg = {
	.digestsize	= GHASH_DIGEST_SIZE,
	.init		= ghash_init,
	.update		= ghash_update,
	.final		= ghash_final,
	.setkey		= ghash_setkey,
	.descsize	= sizeof(struct ghash_desc_ctx),
	.base		= {
		.cra_name		= "ghash",
		.cra_driver_name	= "ghash-generic",
		.cra_priority		= 100,
<<<<<<< HEAD
		.cra_flags		= CRYPTO_ALG_TYPE_SHASH,
		.cra_blocksize		= GHASH_BLOCK_SIZE,
		.cra_ctxsize		= sizeof(struct ghash_ctx),
		.cra_module		= THIS_MODULE,
		.cra_list		= LIST_HEAD_INIT(ghash_alg.base.cra_list),
=======
		.cra_blocksize		= GHASH_BLOCK_SIZE,
		.cra_ctxsize		= sizeof(struct ghash_ctx),
		.cra_module		= THIS_MODULE,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.cra_exit		= ghash_exit_tfm,
	},
};

static int __init ghash_mod_init(void)
{
	return crypto_register_shash(&ghash_alg);
}

static void __exit ghash_mod_exit(void)
{
	crypto_unregister_shash(&ghash_alg);
}

<<<<<<< HEAD
module_init(ghash_mod_init);
module_exit(ghash_mod_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("GHASH Message Digest Algorithm");
MODULE_ALIAS("ghash");
=======
subsys_initcall(ghash_mod_init);
module_exit(ghash_mod_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("GHASH hash function");
MODULE_ALIAS_CRYPTO("ghash");
MODULE_ALIAS_CRYPTO("ghash-generic");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
