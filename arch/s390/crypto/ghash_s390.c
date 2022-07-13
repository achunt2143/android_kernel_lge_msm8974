<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Cryptographic API.
 *
 * s390 implementation of the GHASH algorithm for GCM (Galois/Counter Mode).
 *
 * Copyright IBM Corp. 2011
 * Author(s): Gerald Schaefer <gerald.schaefer@de.ibm.com>
 */

#include <crypto/internal/hash.h>
#include <linux/module.h>
<<<<<<< HEAD

#include "crypt_s390.h"
=======
#include <linux/cpufeature.h>
#include <asm/cpacf.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define GHASH_BLOCK_SIZE	16
#define GHASH_DIGEST_SIZE	16

struct ghash_ctx {
	u8 key[GHASH_BLOCK_SIZE];
};

struct ghash_desc_ctx {
	u8 icv[GHASH_BLOCK_SIZE];
	u8 key[GHASH_BLOCK_SIZE];
	u8 buffer[GHASH_BLOCK_SIZE];
	u32 bytes;
};

static int ghash_init(struct shash_desc *desc)
{
	struct ghash_desc_ctx *dctx = shash_desc_ctx(desc);
	struct ghash_ctx *ctx = crypto_shash_ctx(desc->tfm);

	memset(dctx, 0, sizeof(*dctx));
	memcpy(dctx->key, ctx->key, GHASH_BLOCK_SIZE);

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
=======
	if (keylen != GHASH_BLOCK_SIZE)
		return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	memcpy(ctx->key, key, GHASH_BLOCK_SIZE);

	return 0;
}

static int ghash_update(struct shash_desc *desc,
			 const u8 *src, unsigned int srclen)
{
	struct ghash_desc_ctx *dctx = shash_desc_ctx(desc);
	unsigned int n;
	u8 *buf = dctx->buffer;
<<<<<<< HEAD
	int ret;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (dctx->bytes) {
		u8 *pos = buf + (GHASH_BLOCK_SIZE - dctx->bytes);

		n = min(srclen, dctx->bytes);
		dctx->bytes -= n;
		srclen -= n;

		memcpy(pos, src, n);
		src += n;

		if (!dctx->bytes) {
<<<<<<< HEAD
			ret = crypt_s390_kimd(KIMD_GHASH, dctx, buf,
					      GHASH_BLOCK_SIZE);
			BUG_ON(ret != GHASH_BLOCK_SIZE);
=======
			cpacf_kimd(CPACF_KIMD_GHASH, dctx, buf,
				   GHASH_BLOCK_SIZE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}

	n = srclen & ~(GHASH_BLOCK_SIZE - 1);
	if (n) {
<<<<<<< HEAD
		ret = crypt_s390_kimd(KIMD_GHASH, dctx, src, n);
		BUG_ON(ret != n);
=======
		cpacf_kimd(CPACF_KIMD_GHASH, dctx, src, n);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		src += n;
		srclen -= n;
	}

	if (srclen) {
		dctx->bytes = GHASH_BLOCK_SIZE - srclen;
		memcpy(buf, src, srclen);
	}

	return 0;
}

static int ghash_flush(struct ghash_desc_ctx *dctx)
{
	u8 *buf = dctx->buffer;
<<<<<<< HEAD
	int ret;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (dctx->bytes) {
		u8 *pos = buf + (GHASH_BLOCK_SIZE - dctx->bytes);

		memset(pos, 0, dctx->bytes);
<<<<<<< HEAD

		ret = crypt_s390_kimd(KIMD_GHASH, dctx, buf, GHASH_BLOCK_SIZE);
		BUG_ON(ret != GHASH_BLOCK_SIZE);

		dctx->bytes = 0;
	}
=======
		cpacf_kimd(CPACF_KIMD_GHASH, dctx, buf, GHASH_BLOCK_SIZE);
		dctx->bytes = 0;
	}

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int ghash_final(struct shash_desc *desc, u8 *dst)
{
	struct ghash_desc_ctx *dctx = shash_desc_ctx(desc);
<<<<<<< HEAD

	ghash_flush(dctx);
	memcpy(dst, dctx->icv, GHASH_BLOCK_SIZE);

	return 0;
=======
	int ret;

	ret = ghash_flush(dctx);
	if (!ret)
		memcpy(dst, dctx->icv, GHASH_BLOCK_SIZE);
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
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
		.cra_driver_name	= "ghash-s390",
<<<<<<< HEAD
		.cra_priority		= CRYPT_S390_PRIORITY,
		.cra_flags		= CRYPTO_ALG_TYPE_SHASH,
		.cra_blocksize		= GHASH_BLOCK_SIZE,
		.cra_ctxsize		= sizeof(struct ghash_ctx),
		.cra_module		= THIS_MODULE,
		.cra_list		= LIST_HEAD_INIT(ghash_alg.base.cra_list),
=======
		.cra_priority		= 300,
		.cra_blocksize		= GHASH_BLOCK_SIZE,
		.cra_ctxsize		= sizeof(struct ghash_ctx),
		.cra_module		= THIS_MODULE,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
};

static int __init ghash_mod_init(void)
{
<<<<<<< HEAD
	if (!crypt_s390_func_available(KIMD_GHASH,
				       CRYPT_S390_MSA | CRYPT_S390_MSA4))
		return -EOPNOTSUPP;
=======
	if (!cpacf_query_func(CPACF_KIMD, CPACF_KIMD_GHASH))
		return -ENODEV;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return crypto_register_shash(&ghash_alg);
}

static void __exit ghash_mod_exit(void)
{
	crypto_unregister_shash(&ghash_alg);
}

<<<<<<< HEAD
module_init(ghash_mod_init);
module_exit(ghash_mod_exit);

MODULE_ALIAS("ghash");

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("GHASH Message Digest Algorithm, s390 implementation");
=======
module_cpu_feature_match(S390_CPU_FEATURE_MSA, ghash_mod_init);
module_exit(ghash_mod_exit);

MODULE_ALIAS_CRYPTO("ghash");

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("GHASH hash function, s390 implementation");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
