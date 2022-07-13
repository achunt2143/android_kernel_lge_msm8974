<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Cryptographic API
 *
 * ARC4 Cipher Algorithm
 *
 * Jon Oberheide <jon@oberheide.org>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/crypto.h>

#define ARC4_MIN_KEY_SIZE	1
#define ARC4_MAX_KEY_SIZE	256
#define ARC4_BLOCK_SIZE		1

struct arc4_ctx {
	u8 S[256];
	u8 x, y;
};

static int arc4_set_key(struct crypto_tfm *tfm, const u8 *in_key,
			unsigned int key_len)
{
	struct arc4_ctx *ctx = crypto_tfm_ctx(tfm);
	int i, j = 0, k = 0;

	ctx->x = 1;
	ctx->y = 0;

	for (i = 0; i < 256; i++)
		ctx->S[i] = i;

	for (i = 0; i < 256; i++) {
		u8 a = ctx->S[i];
		j = (j + in_key[k] + a) & 0xff;
		ctx->S[i] = ctx->S[j];
		ctx->S[j] = a;
		if (++k >= key_len)
			k = 0;
	}
=======
 */

#include <crypto/arc4.h>
#include <crypto/internal/skcipher.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>

#define ARC4_ALIGN __alignof__(struct arc4_ctx)

static int crypto_arc4_setkey(struct crypto_lskcipher *tfm, const u8 *in_key,
			      unsigned int key_len)
{
	struct arc4_ctx *ctx = crypto_lskcipher_ctx(tfm);

	return arc4_setkey(ctx, in_key, key_len);
}

static int crypto_arc4_crypt(struct crypto_lskcipher *tfm, const u8 *src,
			     u8 *dst, unsigned nbytes, u8 *siv, u32 flags)
{
	struct arc4_ctx *ctx = crypto_lskcipher_ctx(tfm);

	if (!(flags & CRYPTO_LSKCIPHER_FLAG_CONT))
		memcpy(siv, ctx, sizeof(*ctx));

	ctx = (struct arc4_ctx *)siv;

	arc4_crypt(ctx, dst, src, nbytes);
	return 0;
}

static int crypto_arc4_init(struct crypto_lskcipher *tfm)
{
	pr_warn_ratelimited("\"%s\" (%ld) uses obsolete ecb(arc4) skcipher\n",
			    current->comm, (unsigned long)current->pid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static void arc4_crypt(struct crypto_tfm *tfm, u8 *out, const u8 *in)
{
	struct arc4_ctx *ctx = crypto_tfm_ctx(tfm);

	u8 *const S = ctx->S;
	u8 x = ctx->x;
	u8 y = ctx->y;
	u8 a, b;

	a = S[x];
	y = (y + a) & 0xff;
	b = S[y];
	S[x] = b;
	S[y] = a;
	x = (x + 1) & 0xff;
	*out++ = *in ^ S[(a + b) & 0xff];

	ctx->x = x;
	ctx->y = y;
}

static struct crypto_alg arc4_alg = {
	.cra_name		=	"arc4",
	.cra_flags		=	CRYPTO_ALG_TYPE_CIPHER,
	.cra_blocksize		=	ARC4_BLOCK_SIZE,
	.cra_ctxsize		=	sizeof(struct arc4_ctx),
	.cra_module		=	THIS_MODULE,
	.cra_list		=	LIST_HEAD_INIT(arc4_alg.cra_list),
	.cra_u			=	{ .cipher = {
	.cia_min_keysize	=	ARC4_MIN_KEY_SIZE,
	.cia_max_keysize	=	ARC4_MAX_KEY_SIZE,
	.cia_setkey		=	arc4_set_key,
	.cia_encrypt		=	arc4_crypt,
	.cia_decrypt		=	arc4_crypt } }
=======
static struct lskcipher_alg arc4_alg = {
	.co.base.cra_name		=	"arc4",
	.co.base.cra_driver_name	=	"arc4-generic",
	.co.base.cra_priority		=	100,
	.co.base.cra_blocksize		=	ARC4_BLOCK_SIZE,
	.co.base.cra_ctxsize		=	sizeof(struct arc4_ctx),
	.co.base.cra_alignmask		=	ARC4_ALIGN - 1,
	.co.base.cra_module		=	THIS_MODULE,
	.co.min_keysize			=	ARC4_MIN_KEY_SIZE,
	.co.max_keysize			=	ARC4_MAX_KEY_SIZE,
	.co.statesize			=	sizeof(struct arc4_ctx),
	.setkey				=	crypto_arc4_setkey,
	.encrypt			=	crypto_arc4_crypt,
	.decrypt			=	crypto_arc4_crypt,
	.init				=	crypto_arc4_init,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static int __init arc4_init(void)
{
<<<<<<< HEAD
	return crypto_register_alg(&arc4_alg);
}


static void __exit arc4_exit(void)
{
	crypto_unregister_alg(&arc4_alg);
}

module_init(arc4_init);
=======
	return crypto_register_lskcipher(&arc4_alg);
}

static void __exit arc4_exit(void)
{
	crypto_unregister_lskcipher(&arc4_alg);
}

subsys_initcall(arc4_init);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
module_exit(arc4_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("ARC4 Cipher Algorithm");
MODULE_AUTHOR("Jon Oberheide <jon@oberheide.org>");
<<<<<<< HEAD
=======
MODULE_ALIAS_CRYPTO("ecb(arc4)");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
