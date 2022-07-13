<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Cryptographic API.
 *
 * Null algorithms, aka Much Ado About Nothing.
 *
 * These are needed for IPsec, and may be useful in general for
 * testing & debugging.
 *
 * The null cipher is compliant with RFC2410.
 *
 * Copyright (c) 2002 James Morris <jmorris@intercode.com.au>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

=======
 */

#include <crypto/null.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <crypto/internal/hash.h>
#include <crypto/internal/skcipher.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/string.h>

<<<<<<< HEAD
#define NULL_KEY_SIZE		0
#define NULL_BLOCK_SIZE		1
#define NULL_DIGEST_SIZE	0
#define NULL_IV_SIZE		0
=======
static DEFINE_MUTEX(crypto_default_null_skcipher_lock);
static struct crypto_sync_skcipher *crypto_default_null_skcipher;
static int crypto_default_null_skcipher_refcnt;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int null_compress(struct crypto_tfm *tfm, const u8 *src,
			 unsigned int slen, u8 *dst, unsigned int *dlen)
{
	if (slen > *dlen)
		return -EINVAL;
	memcpy(dst, src, slen);
	*dlen = slen;
	return 0;
}

static int null_init(struct shash_desc *desc)
{
	return 0;
}

static int null_update(struct shash_desc *desc, const u8 *data,
		       unsigned int len)
{
	return 0;
}

static int null_final(struct shash_desc *desc, u8 *out)
{
	return 0;
}

static int null_digest(struct shash_desc *desc, const u8 *data,
		       unsigned int len, u8 *out)
{
	return 0;
}

static int null_hash_setkey(struct crypto_shash *tfm, const u8 *key,
			    unsigned int keylen)
{ return 0; }

<<<<<<< HEAD
=======
static int null_skcipher_setkey(struct crypto_skcipher *tfm, const u8 *key,
				unsigned int keylen)
{ return 0; }

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int null_setkey(struct crypto_tfm *tfm, const u8 *key,
		       unsigned int keylen)
{ return 0; }

static void null_crypt(struct crypto_tfm *tfm, u8 *dst, const u8 *src)
{
	memcpy(dst, src, NULL_BLOCK_SIZE);
}

<<<<<<< HEAD
static int skcipher_null_crypt(struct blkcipher_desc *desc,
			       struct scatterlist *dst,
			       struct scatterlist *src, unsigned int nbytes)
{
	struct blkcipher_walk walk;
	int err;

	blkcipher_walk_init(&walk, dst, src, nbytes);
	err = blkcipher_walk_virt(desc, &walk);
=======
static int null_skcipher_crypt(struct skcipher_request *req)
{
	struct skcipher_walk walk;
	int err;

	err = skcipher_walk_virt(&walk, req, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	while (walk.nbytes) {
		if (walk.src.virt.addr != walk.dst.virt.addr)
			memcpy(walk.dst.virt.addr, walk.src.virt.addr,
			       walk.nbytes);
<<<<<<< HEAD
		err = blkcipher_walk_done(desc, &walk, 0);
=======
		err = skcipher_walk_done(&walk, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return err;
}

<<<<<<< HEAD
static struct crypto_alg compress_null = {
	.cra_name		=	"compress_null",
	.cra_flags		=	CRYPTO_ALG_TYPE_COMPRESS,
	.cra_blocksize		=	NULL_BLOCK_SIZE,
	.cra_ctxsize		=	0,
	.cra_module		=	THIS_MODULE,
	.cra_list		=       LIST_HEAD_INIT(compress_null.cra_list),
	.cra_u			=	{ .compress = {
	.coa_compress 		=	null_compress,
	.coa_decompress		=	null_compress } }
};

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct shash_alg digest_null = {
	.digestsize		=	NULL_DIGEST_SIZE,
	.setkey   		=	null_hash_setkey,
	.init   		=	null_init,
	.update 		=	null_update,
	.finup 			=	null_digest,
	.digest 		=	null_digest,
	.final  		=	null_final,
	.base			=	{
		.cra_name		=	"digest_null",
<<<<<<< HEAD
		.cra_flags		=	CRYPTO_ALG_TYPE_SHASH,
=======
		.cra_driver_name	=	"digest_null-generic",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.cra_blocksize		=	NULL_BLOCK_SIZE,
		.cra_module		=	THIS_MODULE,
	}
};

<<<<<<< HEAD
static struct crypto_alg cipher_null = {
	.cra_name		=	"cipher_null",
=======
static struct skcipher_alg skcipher_null = {
	.base.cra_name		=	"ecb(cipher_null)",
	.base.cra_driver_name	=	"ecb-cipher_null",
	.base.cra_priority	=	100,
	.base.cra_blocksize	=	NULL_BLOCK_SIZE,
	.base.cra_ctxsize	=	0,
	.base.cra_module	=	THIS_MODULE,
	.min_keysize		=	NULL_KEY_SIZE,
	.max_keysize		=	NULL_KEY_SIZE,
	.ivsize			=	NULL_IV_SIZE,
	.setkey			=	null_skcipher_setkey,
	.encrypt		=	null_skcipher_crypt,
	.decrypt		=	null_skcipher_crypt,
};

static struct crypto_alg null_algs[] = { {
	.cra_name		=	"cipher_null",
	.cra_driver_name	=	"cipher_null-generic",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.cra_flags		=	CRYPTO_ALG_TYPE_CIPHER,
	.cra_blocksize		=	NULL_BLOCK_SIZE,
	.cra_ctxsize		=	0,
	.cra_module		=	THIS_MODULE,
<<<<<<< HEAD
	.cra_list		=	LIST_HEAD_INIT(cipher_null.cra_list),
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.cra_u			=	{ .cipher = {
	.cia_min_keysize	=	NULL_KEY_SIZE,
	.cia_max_keysize	=	NULL_KEY_SIZE,
	.cia_setkey		= 	null_setkey,
	.cia_encrypt		=	null_crypt,
	.cia_decrypt		=	null_crypt } }
<<<<<<< HEAD
};

static struct crypto_alg skcipher_null = {
	.cra_name		=	"ecb(cipher_null)",
	.cra_driver_name	=	"ecb-cipher_null",
	.cra_priority		=	100,
	.cra_flags		=	CRYPTO_ALG_TYPE_BLKCIPHER,
	.cra_blocksize		=	NULL_BLOCK_SIZE,
	.cra_type		=	&crypto_blkcipher_type,
	.cra_ctxsize		=	0,
	.cra_module		=	THIS_MODULE,
	.cra_list		=	LIST_HEAD_INIT(skcipher_null.cra_list),
	.cra_u			=	{ .blkcipher = {
	.min_keysize		=	NULL_KEY_SIZE,
	.max_keysize		=	NULL_KEY_SIZE,
	.ivsize			=	NULL_IV_SIZE,
	.setkey			= 	null_setkey,
	.encrypt		=	skcipher_null_crypt,
	.decrypt		=	skcipher_null_crypt } }
};

MODULE_ALIAS("compress_null");
MODULE_ALIAS("digest_null");
MODULE_ALIAS("cipher_null");
=======
}, {
	.cra_name		=	"compress_null",
	.cra_driver_name	=	"compress_null-generic",
	.cra_flags		=	CRYPTO_ALG_TYPE_COMPRESS,
	.cra_blocksize		=	NULL_BLOCK_SIZE,
	.cra_ctxsize		=	0,
	.cra_module		=	THIS_MODULE,
	.cra_u			=	{ .compress = {
	.coa_compress		=	null_compress,
	.coa_decompress		=	null_compress } }
} };

MODULE_ALIAS_CRYPTO("compress_null");
MODULE_ALIAS_CRYPTO("digest_null");
MODULE_ALIAS_CRYPTO("cipher_null");

struct crypto_sync_skcipher *crypto_get_default_null_skcipher(void)
{
	struct crypto_sync_skcipher *tfm;

	mutex_lock(&crypto_default_null_skcipher_lock);
	tfm = crypto_default_null_skcipher;

	if (!tfm) {
		tfm = crypto_alloc_sync_skcipher("ecb(cipher_null)", 0, 0);
		if (IS_ERR(tfm))
			goto unlock;

		crypto_default_null_skcipher = tfm;
	}

	crypto_default_null_skcipher_refcnt++;

unlock:
	mutex_unlock(&crypto_default_null_skcipher_lock);

	return tfm;
}
EXPORT_SYMBOL_GPL(crypto_get_default_null_skcipher);

void crypto_put_default_null_skcipher(void)
{
	mutex_lock(&crypto_default_null_skcipher_lock);
	if (!--crypto_default_null_skcipher_refcnt) {
		crypto_free_sync_skcipher(crypto_default_null_skcipher);
		crypto_default_null_skcipher = NULL;
	}
	mutex_unlock(&crypto_default_null_skcipher_lock);
}
EXPORT_SYMBOL_GPL(crypto_put_default_null_skcipher);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int __init crypto_null_mod_init(void)
{
	int ret = 0;

<<<<<<< HEAD
	ret = crypto_register_alg(&cipher_null);
	if (ret < 0)
		goto out;

	ret = crypto_register_alg(&skcipher_null);
	if (ret < 0)
		goto out_unregister_cipher;

	ret = crypto_register_shash(&digest_null);
	if (ret < 0)
		goto out_unregister_skcipher;

	ret = crypto_register_alg(&compress_null);
	if (ret < 0)
		goto out_unregister_digest;

out:
	return ret;

out_unregister_digest:
	crypto_unregister_shash(&digest_null);
out_unregister_skcipher:
	crypto_unregister_alg(&skcipher_null);
out_unregister_cipher:
	crypto_unregister_alg(&cipher_null);
	goto out;
=======
	ret = crypto_register_algs(null_algs, ARRAY_SIZE(null_algs));
	if (ret < 0)
		goto out;

	ret = crypto_register_shash(&digest_null);
	if (ret < 0)
		goto out_unregister_algs;

	ret = crypto_register_skcipher(&skcipher_null);
	if (ret < 0)
		goto out_unregister_shash;

	return 0;

out_unregister_shash:
	crypto_unregister_shash(&digest_null);
out_unregister_algs:
	crypto_unregister_algs(null_algs, ARRAY_SIZE(null_algs));
out:
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void __exit crypto_null_mod_fini(void)
{
<<<<<<< HEAD
	crypto_unregister_alg(&compress_null);
	crypto_unregister_shash(&digest_null);
	crypto_unregister_alg(&skcipher_null);
	crypto_unregister_alg(&cipher_null);
}

module_init(crypto_null_mod_init);
=======
	crypto_unregister_algs(null_algs, ARRAY_SIZE(null_algs));
	crypto_unregister_shash(&digest_null);
	crypto_unregister_skcipher(&skcipher_null);
}

subsys_initcall(crypto_null_mod_init);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
module_exit(crypto_null_mod_fini);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Null Cryptographic Algorithms");
