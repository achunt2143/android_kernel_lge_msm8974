<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0+
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Cryptographic API.
 *
 * s390 implementation of the DES Cipher Algorithm.
 *
<<<<<<< HEAD
 * Copyright IBM Corp. 2003,2011
 * Author(s): Thomas Spatzier
 *	      Jan Glauber (jan.glauber@de.ibm.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
=======
 * Copyright IBM Corp. 2003, 2011
 * Author(s): Thomas Spatzier
 *	      Jan Glauber (jan.glauber@de.ibm.com)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/init.h>
#include <linux/module.h>
<<<<<<< HEAD
#include <linux/crypto.h>
#include <crypto/algapi.h>
#include <crypto/des.h>

#include "crypt_s390.h"
=======
#include <linux/cpufeature.h>
#include <linux/crypto.h>
#include <linux/fips.h>
#include <linux/mutex.h>
#include <crypto/algapi.h>
#include <crypto/internal/des.h>
#include <crypto/internal/skcipher.h>
#include <asm/cpacf.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define DES3_KEY_SIZE	(3 * DES_KEY_SIZE)

static u8 *ctrblk;
<<<<<<< HEAD
=======
static DEFINE_MUTEX(ctrblk_lock);

static cpacf_mask_t km_functions, kmc_functions, kmctr_functions;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct s390_des_ctx {
	u8 iv[DES_BLOCK_SIZE];
	u8 key[DES3_KEY_SIZE];
};

static int des_setkey(struct crypto_tfm *tfm, const u8 *key,
		      unsigned int key_len)
{
	struct s390_des_ctx *ctx = crypto_tfm_ctx(tfm);
<<<<<<< HEAD
	u32 *flags = &tfm->crt_flags;
	u32 tmp[DES_EXPKEY_WORDS];

	/* check for weak keys */
	if (!des_ekey(tmp, key) && (*flags & CRYPTO_TFM_REQ_WEAK_KEY)) {
		*flags |= CRYPTO_TFM_RES_WEAK_KEY;
		return -EINVAL;
	}
=======
	int err;

	err = crypto_des_verify_key(tfm, key);
	if (err)
		return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	memcpy(ctx->key, key, key_len);
	return 0;
}

<<<<<<< HEAD
static void des_encrypt(struct crypto_tfm *tfm, u8 *out, const u8 *in)
{
	struct s390_des_ctx *ctx = crypto_tfm_ctx(tfm);

	crypt_s390_km(KM_DEA_ENCRYPT, ctx->key, out, in, DES_BLOCK_SIZE);
}

static void des_decrypt(struct crypto_tfm *tfm, u8 *out, const u8 *in)
{
	struct s390_des_ctx *ctx = crypto_tfm_ctx(tfm);

	crypt_s390_km(KM_DEA_DECRYPT, ctx->key, out, in, DES_BLOCK_SIZE);
=======
static int des_setkey_skcipher(struct crypto_skcipher *tfm, const u8 *key,
			       unsigned int key_len)
{
	return des_setkey(crypto_skcipher_tfm(tfm), key, key_len);
}

static void s390_des_encrypt(struct crypto_tfm *tfm, u8 *out, const u8 *in)
{
	struct s390_des_ctx *ctx = crypto_tfm_ctx(tfm);

	cpacf_km(CPACF_KM_DEA, ctx->key, out, in, DES_BLOCK_SIZE);
}

static void s390_des_decrypt(struct crypto_tfm *tfm, u8 *out, const u8 *in)
{
	struct s390_des_ctx *ctx = crypto_tfm_ctx(tfm);

	cpacf_km(CPACF_KM_DEA | CPACF_DECRYPT,
		 ctx->key, out, in, DES_BLOCK_SIZE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct crypto_alg des_alg = {
	.cra_name		=	"des",
	.cra_driver_name	=	"des-s390",
<<<<<<< HEAD
	.cra_priority		=	CRYPT_S390_PRIORITY,
=======
	.cra_priority		=	300,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.cra_flags		=	CRYPTO_ALG_TYPE_CIPHER,
	.cra_blocksize		=	DES_BLOCK_SIZE,
	.cra_ctxsize		=	sizeof(struct s390_des_ctx),
	.cra_module		=	THIS_MODULE,
<<<<<<< HEAD
	.cra_list		=	LIST_HEAD_INIT(des_alg.cra_list),
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.cra_u			=	{
		.cipher = {
			.cia_min_keysize	=	DES_KEY_SIZE,
			.cia_max_keysize	=	DES_KEY_SIZE,
			.cia_setkey		=	des_setkey,
<<<<<<< HEAD
			.cia_encrypt		=	des_encrypt,
			.cia_decrypt		=	des_decrypt,
=======
			.cia_encrypt		=	s390_des_encrypt,
			.cia_decrypt		=	s390_des_decrypt,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}
};

<<<<<<< HEAD
static int ecb_desall_crypt(struct blkcipher_desc *desc, long func,
			    u8 *key, struct blkcipher_walk *walk)
{
	int ret = blkcipher_walk_virt(desc, walk);
	unsigned int nbytes;

	while ((nbytes = walk->nbytes)) {
		/* only use complete blocks */
		unsigned int n = nbytes & ~(DES_BLOCK_SIZE - 1);
		u8 *out = walk->dst.virt.addr;
		u8 *in = walk->src.virt.addr;

		ret = crypt_s390_km(func, key, out, in, n);
		BUG_ON((ret < 0) || (ret != n));

		nbytes &= DES_BLOCK_SIZE - 1;
		ret = blkcipher_walk_done(desc, walk, nbytes);
	}

	return ret;
}

static int cbc_desall_crypt(struct blkcipher_desc *desc, long func,
			    u8 *iv, struct blkcipher_walk *walk)
{
	int ret = blkcipher_walk_virt(desc, walk);
	unsigned int nbytes = walk->nbytes;

	if (!nbytes)
		goto out;

	memcpy(iv, walk->iv, DES_BLOCK_SIZE);
	do {
		/* only use complete blocks */
		unsigned int n = nbytes & ~(DES_BLOCK_SIZE - 1);
		u8 *out = walk->dst.virt.addr;
		u8 *in = walk->src.virt.addr;

		ret = crypt_s390_kmc(func, iv, out, in, n);
		BUG_ON((ret < 0) || (ret != n));

		nbytes &= DES_BLOCK_SIZE - 1;
		ret = blkcipher_walk_done(desc, walk, nbytes);
	} while ((nbytes = walk->nbytes));
	memcpy(walk->iv, iv, DES_BLOCK_SIZE);

out:
	return ret;
}

static int ecb_des_encrypt(struct blkcipher_desc *desc,
			   struct scatterlist *dst, struct scatterlist *src,
			   unsigned int nbytes)
{
	struct s390_des_ctx *ctx = crypto_blkcipher_ctx(desc->tfm);
	struct blkcipher_walk walk;

	blkcipher_walk_init(&walk, dst, src, nbytes);
	return ecb_desall_crypt(desc, KM_DEA_ENCRYPT, ctx->key, &walk);
}

static int ecb_des_decrypt(struct blkcipher_desc *desc,
			   struct scatterlist *dst, struct scatterlist *src,
			   unsigned int nbytes)
{
	struct s390_des_ctx *ctx = crypto_blkcipher_ctx(desc->tfm);
	struct blkcipher_walk walk;

	blkcipher_walk_init(&walk, dst, src, nbytes);
	return ecb_desall_crypt(desc, KM_DEA_DECRYPT, ctx->key, &walk);
}

static struct crypto_alg ecb_des_alg = {
	.cra_name		=	"ecb(des)",
	.cra_driver_name	=	"ecb-des-s390",
	.cra_priority		=	CRYPT_S390_COMPOSITE_PRIORITY,
	.cra_flags		=	CRYPTO_ALG_TYPE_BLKCIPHER,
	.cra_blocksize		=	DES_BLOCK_SIZE,
	.cra_ctxsize		=	sizeof(struct s390_des_ctx),
	.cra_type		=	&crypto_blkcipher_type,
	.cra_module		=	THIS_MODULE,
	.cra_list		=	LIST_HEAD_INIT(ecb_des_alg.cra_list),
	.cra_u			=	{
		.blkcipher = {
			.min_keysize		=	DES_KEY_SIZE,
			.max_keysize		=	DES_KEY_SIZE,
			.setkey			=	des_setkey,
			.encrypt		=	ecb_des_encrypt,
			.decrypt		=	ecb_des_decrypt,
		}
	}
};

static int cbc_des_encrypt(struct blkcipher_desc *desc,
			   struct scatterlist *dst, struct scatterlist *src,
			   unsigned int nbytes)
{
	struct s390_des_ctx *ctx = crypto_blkcipher_ctx(desc->tfm);
	struct blkcipher_walk walk;

	blkcipher_walk_init(&walk, dst, src, nbytes);
	return cbc_desall_crypt(desc, KMC_DEA_ENCRYPT, ctx->iv, &walk);
}

static int cbc_des_decrypt(struct blkcipher_desc *desc,
			   struct scatterlist *dst, struct scatterlist *src,
			   unsigned int nbytes)
{
	struct s390_des_ctx *ctx = crypto_blkcipher_ctx(desc->tfm);
	struct blkcipher_walk walk;

	blkcipher_walk_init(&walk, dst, src, nbytes);
	return cbc_desall_crypt(desc, KMC_DEA_DECRYPT, ctx->iv, &walk);
}

static struct crypto_alg cbc_des_alg = {
	.cra_name		=	"cbc(des)",
	.cra_driver_name	=	"cbc-des-s390",
	.cra_priority		=	CRYPT_S390_COMPOSITE_PRIORITY,
	.cra_flags		=	CRYPTO_ALG_TYPE_BLKCIPHER,
	.cra_blocksize		=	DES_BLOCK_SIZE,
	.cra_ctxsize		=	sizeof(struct s390_des_ctx),
	.cra_type		=	&crypto_blkcipher_type,
	.cra_module		=	THIS_MODULE,
	.cra_list		=	LIST_HEAD_INIT(cbc_des_alg.cra_list),
	.cra_u			=	{
		.blkcipher = {
			.min_keysize		=	DES_KEY_SIZE,
			.max_keysize		=	DES_KEY_SIZE,
			.ivsize			=	DES_BLOCK_SIZE,
			.setkey			=	des_setkey,
			.encrypt		=	cbc_des_encrypt,
			.decrypt		=	cbc_des_decrypt,
		}
	}
=======
static int ecb_desall_crypt(struct skcipher_request *req, unsigned long fc)
{
	struct crypto_skcipher *tfm = crypto_skcipher_reqtfm(req);
	struct s390_des_ctx *ctx = crypto_skcipher_ctx(tfm);
	struct skcipher_walk walk;
	unsigned int nbytes, n;
	int ret;

	ret = skcipher_walk_virt(&walk, req, false);
	while ((nbytes = walk.nbytes) != 0) {
		/* only use complete blocks */
		n = nbytes & ~(DES_BLOCK_SIZE - 1);
		cpacf_km(fc, ctx->key, walk.dst.virt.addr,
			 walk.src.virt.addr, n);
		ret = skcipher_walk_done(&walk, nbytes - n);
	}
	return ret;
}

static int cbc_desall_crypt(struct skcipher_request *req, unsigned long fc)
{
	struct crypto_skcipher *tfm = crypto_skcipher_reqtfm(req);
	struct s390_des_ctx *ctx = crypto_skcipher_ctx(tfm);
	struct skcipher_walk walk;
	unsigned int nbytes, n;
	int ret;
	struct {
		u8 iv[DES_BLOCK_SIZE];
		u8 key[DES3_KEY_SIZE];
	} param;

	ret = skcipher_walk_virt(&walk, req, false);
	if (ret)
		return ret;
	memcpy(param.iv, walk.iv, DES_BLOCK_SIZE);
	memcpy(param.key, ctx->key, DES3_KEY_SIZE);
	while ((nbytes = walk.nbytes) != 0) {
		/* only use complete blocks */
		n = nbytes & ~(DES_BLOCK_SIZE - 1);
		cpacf_kmc(fc, &param, walk.dst.virt.addr,
			  walk.src.virt.addr, n);
		memcpy(walk.iv, param.iv, DES_BLOCK_SIZE);
		ret = skcipher_walk_done(&walk, nbytes - n);
	}
	return ret;
}

static int ecb_des_encrypt(struct skcipher_request *req)
{
	return ecb_desall_crypt(req, CPACF_KM_DEA);
}

static int ecb_des_decrypt(struct skcipher_request *req)
{
	return ecb_desall_crypt(req, CPACF_KM_DEA | CPACF_DECRYPT);
}

static struct skcipher_alg ecb_des_alg = {
	.base.cra_name		=	"ecb(des)",
	.base.cra_driver_name	=	"ecb-des-s390",
	.base.cra_priority	=	400,	/* combo: des + ecb */
	.base.cra_blocksize	=	DES_BLOCK_SIZE,
	.base.cra_ctxsize	=	sizeof(struct s390_des_ctx),
	.base.cra_module	=	THIS_MODULE,
	.min_keysize		=	DES_KEY_SIZE,
	.max_keysize		=	DES_KEY_SIZE,
	.setkey			=	des_setkey_skcipher,
	.encrypt		=	ecb_des_encrypt,
	.decrypt		=	ecb_des_decrypt,
};

static int cbc_des_encrypt(struct skcipher_request *req)
{
	return cbc_desall_crypt(req, CPACF_KMC_DEA);
}

static int cbc_des_decrypt(struct skcipher_request *req)
{
	return cbc_desall_crypt(req, CPACF_KMC_DEA | CPACF_DECRYPT);
}

static struct skcipher_alg cbc_des_alg = {
	.base.cra_name		=	"cbc(des)",
	.base.cra_driver_name	=	"cbc-des-s390",
	.base.cra_priority	=	400,	/* combo: des + cbc */
	.base.cra_blocksize	=	DES_BLOCK_SIZE,
	.base.cra_ctxsize	=	sizeof(struct s390_des_ctx),
	.base.cra_module	=	THIS_MODULE,
	.min_keysize		=	DES_KEY_SIZE,
	.max_keysize		=	DES_KEY_SIZE,
	.ivsize			=	DES_BLOCK_SIZE,
	.setkey			=	des_setkey_skcipher,
	.encrypt		=	cbc_des_encrypt,
	.decrypt		=	cbc_des_decrypt,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/*
 * RFC2451:
 *
 *   For DES-EDE3, there is no known need to reject weak or
 *   complementation keys.  Any weakness is obviated by the use of
 *   multiple keys.
 *
 *   However, if the first two or last two independent 64-bit keys are
 *   equal (k1 == k2 or k2 == k3), then the DES3 operation is simply the
 *   same as DES.  Implementers MUST reject keys that exhibit this
 *   property.
 *
<<<<<<< HEAD
=======
 *   In fips mode additionally check for all 3 keys are unique.
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
static int des3_setkey(struct crypto_tfm *tfm, const u8 *key,
		       unsigned int key_len)
{
	struct s390_des_ctx *ctx = crypto_tfm_ctx(tfm);
<<<<<<< HEAD
	u32 *flags = &tfm->crt_flags;

	if (!(memcmp(key, &key[DES_KEY_SIZE], DES_KEY_SIZE) &&
	    memcmp(&key[DES_KEY_SIZE], &key[DES_KEY_SIZE * 2],
		   DES_KEY_SIZE)) &&
	    (*flags & CRYPTO_TFM_REQ_WEAK_KEY)) {
		*flags |= CRYPTO_TFM_RES_WEAK_KEY;
		return -EINVAL;
	}
=======
	int err;

	err = crypto_des3_ede_verify_key(tfm, key);
	if (err)
		return err;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	memcpy(ctx->key, key, key_len);
	return 0;
}

<<<<<<< HEAD
=======
static int des3_setkey_skcipher(struct crypto_skcipher *tfm, const u8 *key,
				unsigned int key_len)
{
	return des3_setkey(crypto_skcipher_tfm(tfm), key, key_len);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void des3_encrypt(struct crypto_tfm *tfm, u8 *dst, const u8 *src)
{
	struct s390_des_ctx *ctx = crypto_tfm_ctx(tfm);

<<<<<<< HEAD
	crypt_s390_km(KM_TDEA_192_ENCRYPT, ctx->key, dst, src, DES_BLOCK_SIZE);
=======
	cpacf_km(CPACF_KM_TDEA_192, ctx->key, dst, src, DES_BLOCK_SIZE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void des3_decrypt(struct crypto_tfm *tfm, u8 *dst, const u8 *src)
{
	struct s390_des_ctx *ctx = crypto_tfm_ctx(tfm);

<<<<<<< HEAD
	crypt_s390_km(KM_TDEA_192_DECRYPT, ctx->key, dst, src, DES_BLOCK_SIZE);
=======
	cpacf_km(CPACF_KM_TDEA_192 | CPACF_DECRYPT,
		 ctx->key, dst, src, DES_BLOCK_SIZE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct crypto_alg des3_alg = {
	.cra_name		=	"des3_ede",
	.cra_driver_name	=	"des3_ede-s390",
<<<<<<< HEAD
	.cra_priority		=	CRYPT_S390_PRIORITY,
=======
	.cra_priority		=	300,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.cra_flags		=	CRYPTO_ALG_TYPE_CIPHER,
	.cra_blocksize		=	DES_BLOCK_SIZE,
	.cra_ctxsize		=	sizeof(struct s390_des_ctx),
	.cra_module		=	THIS_MODULE,
<<<<<<< HEAD
	.cra_list		=	LIST_HEAD_INIT(des3_alg.cra_list),
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.cra_u			=	{
		.cipher = {
			.cia_min_keysize	=	DES3_KEY_SIZE,
			.cia_max_keysize	=	DES3_KEY_SIZE,
			.cia_setkey		=	des3_setkey,
			.cia_encrypt		=	des3_encrypt,
			.cia_decrypt		=	des3_decrypt,
		}
	}
};

<<<<<<< HEAD
static int ecb_des3_encrypt(struct blkcipher_desc *desc,
			    struct scatterlist *dst, struct scatterlist *src,
			    unsigned int nbytes)
{
	struct s390_des_ctx *ctx = crypto_blkcipher_ctx(desc->tfm);
	struct blkcipher_walk walk;

	blkcipher_walk_init(&walk, dst, src, nbytes);
	return ecb_desall_crypt(desc, KM_TDEA_192_ENCRYPT, ctx->key, &walk);
}

static int ecb_des3_decrypt(struct blkcipher_desc *desc,
			    struct scatterlist *dst, struct scatterlist *src,
			    unsigned int nbytes)
{
	struct s390_des_ctx *ctx = crypto_blkcipher_ctx(desc->tfm);
	struct blkcipher_walk walk;

	blkcipher_walk_init(&walk, dst, src, nbytes);
	return ecb_desall_crypt(desc, KM_TDEA_192_DECRYPT, ctx->key, &walk);
}

static struct crypto_alg ecb_des3_alg = {
	.cra_name		=	"ecb(des3_ede)",
	.cra_driver_name	=	"ecb-des3_ede-s390",
	.cra_priority		=	CRYPT_S390_COMPOSITE_PRIORITY,
	.cra_flags		=	CRYPTO_ALG_TYPE_BLKCIPHER,
	.cra_blocksize		=	DES_BLOCK_SIZE,
	.cra_ctxsize		=	sizeof(struct s390_des_ctx),
	.cra_type		=	&crypto_blkcipher_type,
	.cra_module		=	THIS_MODULE,
	.cra_list		=	LIST_HEAD_INIT(
						ecb_des3_alg.cra_list),
	.cra_u			=	{
		.blkcipher = {
			.min_keysize		=	DES3_KEY_SIZE,
			.max_keysize		=	DES3_KEY_SIZE,
			.setkey			=	des3_setkey,
			.encrypt		=	ecb_des3_encrypt,
			.decrypt		=	ecb_des3_decrypt,
		}
	}
};

static int cbc_des3_encrypt(struct blkcipher_desc *desc,
			    struct scatterlist *dst, struct scatterlist *src,
			    unsigned int nbytes)
{
	struct s390_des_ctx *ctx = crypto_blkcipher_ctx(desc->tfm);
	struct blkcipher_walk walk;

	blkcipher_walk_init(&walk, dst, src, nbytes);
	return cbc_desall_crypt(desc, KMC_TDEA_192_ENCRYPT, ctx->iv, &walk);
}

static int cbc_des3_decrypt(struct blkcipher_desc *desc,
			    struct scatterlist *dst, struct scatterlist *src,
			    unsigned int nbytes)
{
	struct s390_des_ctx *ctx = crypto_blkcipher_ctx(desc->tfm);
	struct blkcipher_walk walk;

	blkcipher_walk_init(&walk, dst, src, nbytes);
	return cbc_desall_crypt(desc, KMC_TDEA_192_DECRYPT, ctx->iv, &walk);
}

static struct crypto_alg cbc_des3_alg = {
	.cra_name		=	"cbc(des3_ede)",
	.cra_driver_name	=	"cbc-des3_ede-s390",
	.cra_priority		=	CRYPT_S390_COMPOSITE_PRIORITY,
	.cra_flags		=	CRYPTO_ALG_TYPE_BLKCIPHER,
	.cra_blocksize		=	DES_BLOCK_SIZE,
	.cra_ctxsize		=	sizeof(struct s390_des_ctx),
	.cra_type		=	&crypto_blkcipher_type,
	.cra_module		=	THIS_MODULE,
	.cra_list		=	LIST_HEAD_INIT(
						cbc_des3_alg.cra_list),
	.cra_u			=	{
		.blkcipher = {
			.min_keysize		=	DES3_KEY_SIZE,
			.max_keysize		=	DES3_KEY_SIZE,
			.ivsize			=	DES_BLOCK_SIZE,
			.setkey			=	des3_setkey,
			.encrypt		=	cbc_des3_encrypt,
			.decrypt		=	cbc_des3_decrypt,
		}
	}
};

static int ctr_desall_crypt(struct blkcipher_desc *desc, long func,
			    struct s390_des_ctx *ctx, struct blkcipher_walk *walk)
{
	int ret = blkcipher_walk_virt_block(desc, walk, DES_BLOCK_SIZE);
	unsigned int i, n, nbytes;
	u8 buf[DES_BLOCK_SIZE];
	u8 *out, *in;

	memcpy(ctrblk, walk->iv, DES_BLOCK_SIZE);
	while ((nbytes = walk->nbytes) >= DES_BLOCK_SIZE) {
		out = walk->dst.virt.addr;
		in = walk->src.virt.addr;
		while (nbytes >= DES_BLOCK_SIZE) {
			/* align to block size, max. PAGE_SIZE */
			n = (nbytes > PAGE_SIZE) ? PAGE_SIZE :
				nbytes & ~(DES_BLOCK_SIZE - 1);
			for (i = DES_BLOCK_SIZE; i < n; i += DES_BLOCK_SIZE) {
				memcpy(ctrblk + i, ctrblk + i - DES_BLOCK_SIZE,
				       DES_BLOCK_SIZE);
				crypto_inc(ctrblk + i, DES_BLOCK_SIZE);
			}
			ret = crypt_s390_kmctr(func, ctx->key, out, in, n, ctrblk);
			BUG_ON((ret < 0) || (ret != n));
			if (n > DES_BLOCK_SIZE)
				memcpy(ctrblk, ctrblk + n - DES_BLOCK_SIZE,
				       DES_BLOCK_SIZE);
			crypto_inc(ctrblk, DES_BLOCK_SIZE);
			out += n;
			in += n;
			nbytes -= n;
		}
		ret = blkcipher_walk_done(desc, walk, nbytes);
	}

	/* final block may be < DES_BLOCK_SIZE, copy only nbytes */
	if (nbytes) {
		out = walk->dst.virt.addr;
		in = walk->src.virt.addr;
		ret = crypt_s390_kmctr(func, ctx->key, buf, in,
				       DES_BLOCK_SIZE, ctrblk);
		BUG_ON(ret < 0 || ret != DES_BLOCK_SIZE);
		memcpy(out, buf, nbytes);
		crypto_inc(ctrblk, DES_BLOCK_SIZE);
		ret = blkcipher_walk_done(desc, walk, 0);
	}
	memcpy(walk->iv, ctrblk, DES_BLOCK_SIZE);
	return ret;
}

static int ctr_des_encrypt(struct blkcipher_desc *desc,
			   struct scatterlist *dst, struct scatterlist *src,
			   unsigned int nbytes)
{
	struct s390_des_ctx *ctx = crypto_blkcipher_ctx(desc->tfm);
	struct blkcipher_walk walk;

	blkcipher_walk_init(&walk, dst, src, nbytes);
	return ctr_desall_crypt(desc, KMCTR_DEA_ENCRYPT, ctx, &walk);
}

static int ctr_des_decrypt(struct blkcipher_desc *desc,
			   struct scatterlist *dst, struct scatterlist *src,
			   unsigned int nbytes)
{
	struct s390_des_ctx *ctx = crypto_blkcipher_ctx(desc->tfm);
	struct blkcipher_walk walk;

	blkcipher_walk_init(&walk, dst, src, nbytes);
	return ctr_desall_crypt(desc, KMCTR_DEA_DECRYPT, ctx, &walk);
}

static struct crypto_alg ctr_des_alg = {
	.cra_name		=	"ctr(des)",
	.cra_driver_name	=	"ctr-des-s390",
	.cra_priority		=	CRYPT_S390_COMPOSITE_PRIORITY,
	.cra_flags		=	CRYPTO_ALG_TYPE_BLKCIPHER,
	.cra_blocksize		=	1,
	.cra_ctxsize		=	sizeof(struct s390_des_ctx),
	.cra_type		=	&crypto_blkcipher_type,
	.cra_module		=	THIS_MODULE,
	.cra_list		=	LIST_HEAD_INIT(ctr_des_alg.cra_list),
	.cra_u			=	{
		.blkcipher = {
			.min_keysize		=	DES_KEY_SIZE,
			.max_keysize		=	DES_KEY_SIZE,
			.ivsize			=	DES_BLOCK_SIZE,
			.setkey			=	des_setkey,
			.encrypt		=	ctr_des_encrypt,
			.decrypt		=	ctr_des_decrypt,
		}
	}
};

static int ctr_des3_encrypt(struct blkcipher_desc *desc,
			    struct scatterlist *dst, struct scatterlist *src,
			    unsigned int nbytes)
{
	struct s390_des_ctx *ctx = crypto_blkcipher_ctx(desc->tfm);
	struct blkcipher_walk walk;

	blkcipher_walk_init(&walk, dst, src, nbytes);
	return ctr_desall_crypt(desc, KMCTR_TDEA_192_ENCRYPT, ctx, &walk);
}

static int ctr_des3_decrypt(struct blkcipher_desc *desc,
			    struct scatterlist *dst, struct scatterlist *src,
			    unsigned int nbytes)
{
	struct s390_des_ctx *ctx = crypto_blkcipher_ctx(desc->tfm);
	struct blkcipher_walk walk;

	blkcipher_walk_init(&walk, dst, src, nbytes);
	return ctr_desall_crypt(desc, KMCTR_TDEA_192_DECRYPT, ctx, &walk);
}

static struct crypto_alg ctr_des3_alg = {
	.cra_name		=	"ctr(des3_ede)",
	.cra_driver_name	=	"ctr-des3_ede-s390",
	.cra_priority		=	CRYPT_S390_COMPOSITE_PRIORITY,
	.cra_flags		=	CRYPTO_ALG_TYPE_BLKCIPHER,
	.cra_blocksize		=	1,
	.cra_ctxsize		=	sizeof(struct s390_des_ctx),
	.cra_type		=	&crypto_blkcipher_type,
	.cra_module		=	THIS_MODULE,
	.cra_list		=	LIST_HEAD_INIT(ctr_des3_alg.cra_list),
	.cra_u			=	{
		.blkcipher = {
			.min_keysize		=	DES3_KEY_SIZE,
			.max_keysize		=	DES3_KEY_SIZE,
			.ivsize			=	DES_BLOCK_SIZE,
			.setkey			=	des3_setkey,
			.encrypt		=	ctr_des3_encrypt,
			.decrypt		=	ctr_des3_decrypt,
		}
	}
};

=======
static int ecb_des3_encrypt(struct skcipher_request *req)
{
	return ecb_desall_crypt(req, CPACF_KM_TDEA_192);
}

static int ecb_des3_decrypt(struct skcipher_request *req)
{
	return ecb_desall_crypt(req, CPACF_KM_TDEA_192 | CPACF_DECRYPT);
}

static struct skcipher_alg ecb_des3_alg = {
	.base.cra_name		=	"ecb(des3_ede)",
	.base.cra_driver_name	=	"ecb-des3_ede-s390",
	.base.cra_priority	=	400,	/* combo: des3 + ecb */
	.base.cra_blocksize	=	DES_BLOCK_SIZE,
	.base.cra_ctxsize	=	sizeof(struct s390_des_ctx),
	.base.cra_module	=	THIS_MODULE,
	.min_keysize		=	DES3_KEY_SIZE,
	.max_keysize		=	DES3_KEY_SIZE,
	.setkey			=	des3_setkey_skcipher,
	.encrypt		=	ecb_des3_encrypt,
	.decrypt		=	ecb_des3_decrypt,
};

static int cbc_des3_encrypt(struct skcipher_request *req)
{
	return cbc_desall_crypt(req, CPACF_KMC_TDEA_192);
}

static int cbc_des3_decrypt(struct skcipher_request *req)
{
	return cbc_desall_crypt(req, CPACF_KMC_TDEA_192 | CPACF_DECRYPT);
}

static struct skcipher_alg cbc_des3_alg = {
	.base.cra_name		=	"cbc(des3_ede)",
	.base.cra_driver_name	=	"cbc-des3_ede-s390",
	.base.cra_priority	=	400,	/* combo: des3 + cbc */
	.base.cra_blocksize	=	DES_BLOCK_SIZE,
	.base.cra_ctxsize	=	sizeof(struct s390_des_ctx),
	.base.cra_module	=	THIS_MODULE,
	.min_keysize		=	DES3_KEY_SIZE,
	.max_keysize		=	DES3_KEY_SIZE,
	.ivsize			=	DES_BLOCK_SIZE,
	.setkey			=	des3_setkey_skcipher,
	.encrypt		=	cbc_des3_encrypt,
	.decrypt		=	cbc_des3_decrypt,
};

static unsigned int __ctrblk_init(u8 *ctrptr, u8 *iv, unsigned int nbytes)
{
	unsigned int i, n;

	/* align to block size, max. PAGE_SIZE */
	n = (nbytes > PAGE_SIZE) ? PAGE_SIZE : nbytes & ~(DES_BLOCK_SIZE - 1);
	memcpy(ctrptr, iv, DES_BLOCK_SIZE);
	for (i = (n / DES_BLOCK_SIZE) - 1; i > 0; i--) {
		memcpy(ctrptr + DES_BLOCK_SIZE, ctrptr, DES_BLOCK_SIZE);
		crypto_inc(ctrptr + DES_BLOCK_SIZE, DES_BLOCK_SIZE);
		ctrptr += DES_BLOCK_SIZE;
	}
	return n;
}

static int ctr_desall_crypt(struct skcipher_request *req, unsigned long fc)
{
	struct crypto_skcipher *tfm = crypto_skcipher_reqtfm(req);
	struct s390_des_ctx *ctx = crypto_skcipher_ctx(tfm);
	u8 buf[DES_BLOCK_SIZE], *ctrptr;
	struct skcipher_walk walk;
	unsigned int n, nbytes;
	int ret, locked;

	locked = mutex_trylock(&ctrblk_lock);

	ret = skcipher_walk_virt(&walk, req, false);
	while ((nbytes = walk.nbytes) >= DES_BLOCK_SIZE) {
		n = DES_BLOCK_SIZE;
		if (nbytes >= 2*DES_BLOCK_SIZE && locked)
			n = __ctrblk_init(ctrblk, walk.iv, nbytes);
		ctrptr = (n > DES_BLOCK_SIZE) ? ctrblk : walk.iv;
		cpacf_kmctr(fc, ctx->key, walk.dst.virt.addr,
			    walk.src.virt.addr, n, ctrptr);
		if (ctrptr == ctrblk)
			memcpy(walk.iv, ctrptr + n - DES_BLOCK_SIZE,
				DES_BLOCK_SIZE);
		crypto_inc(walk.iv, DES_BLOCK_SIZE);
		ret = skcipher_walk_done(&walk, nbytes - n);
	}
	if (locked)
		mutex_unlock(&ctrblk_lock);
	/* final block may be < DES_BLOCK_SIZE, copy only nbytes */
	if (nbytes) {
		cpacf_kmctr(fc, ctx->key, buf, walk.src.virt.addr,
			    DES_BLOCK_SIZE, walk.iv);
		memcpy(walk.dst.virt.addr, buf, nbytes);
		crypto_inc(walk.iv, DES_BLOCK_SIZE);
		ret = skcipher_walk_done(&walk, 0);
	}
	return ret;
}

static int ctr_des_crypt(struct skcipher_request *req)
{
	return ctr_desall_crypt(req, CPACF_KMCTR_DEA);
}

static struct skcipher_alg ctr_des_alg = {
	.base.cra_name		=	"ctr(des)",
	.base.cra_driver_name	=	"ctr-des-s390",
	.base.cra_priority	=	400,	/* combo: des + ctr */
	.base.cra_blocksize	=	1,
	.base.cra_ctxsize	=	sizeof(struct s390_des_ctx),
	.base.cra_module	=	THIS_MODULE,
	.min_keysize		=	DES_KEY_SIZE,
	.max_keysize		=	DES_KEY_SIZE,
	.ivsize			=	DES_BLOCK_SIZE,
	.setkey			=	des_setkey_skcipher,
	.encrypt		=	ctr_des_crypt,
	.decrypt		=	ctr_des_crypt,
	.chunksize		=	DES_BLOCK_SIZE,
};

static int ctr_des3_crypt(struct skcipher_request *req)
{
	return ctr_desall_crypt(req, CPACF_KMCTR_TDEA_192);
}

static struct skcipher_alg ctr_des3_alg = {
	.base.cra_name		=	"ctr(des3_ede)",
	.base.cra_driver_name	=	"ctr-des3_ede-s390",
	.base.cra_priority	=	400,	/* combo: des3 + ede */
	.base.cra_blocksize	=	1,
	.base.cra_ctxsize	=	sizeof(struct s390_des_ctx),
	.base.cra_module	=	THIS_MODULE,
	.min_keysize		=	DES3_KEY_SIZE,
	.max_keysize		=	DES3_KEY_SIZE,
	.ivsize			=	DES_BLOCK_SIZE,
	.setkey			=	des3_setkey_skcipher,
	.encrypt		=	ctr_des3_crypt,
	.decrypt		=	ctr_des3_crypt,
	.chunksize		=	DES_BLOCK_SIZE,
};

static struct crypto_alg *des_s390_algs_ptr[2];
static int des_s390_algs_num;
static struct skcipher_alg *des_s390_skciphers_ptr[6];
static int des_s390_skciphers_num;

static int des_s390_register_alg(struct crypto_alg *alg)
{
	int ret;

	ret = crypto_register_alg(alg);
	if (!ret)
		des_s390_algs_ptr[des_s390_algs_num++] = alg;
	return ret;
}

static int des_s390_register_skcipher(struct skcipher_alg *alg)
{
	int ret;

	ret = crypto_register_skcipher(alg);
	if (!ret)
		des_s390_skciphers_ptr[des_s390_skciphers_num++] = alg;
	return ret;
}

static void des_s390_exit(void)
{
	while (des_s390_algs_num--)
		crypto_unregister_alg(des_s390_algs_ptr[des_s390_algs_num]);
	while (des_s390_skciphers_num--)
		crypto_unregister_skcipher(des_s390_skciphers_ptr[des_s390_skciphers_num]);
	if (ctrblk)
		free_page((unsigned long) ctrblk);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int __init des_s390_init(void)
{
	int ret;

<<<<<<< HEAD
	if (!crypt_s390_func_available(KM_DEA_ENCRYPT, CRYPT_S390_MSA) ||
	    !crypt_s390_func_available(KM_TDEA_192_ENCRYPT, CRYPT_S390_MSA))
		return -EOPNOTSUPP;

	ret = crypto_register_alg(&des_alg);
	if (ret)
		goto des_err;
	ret = crypto_register_alg(&ecb_des_alg);
	if (ret)
		goto ecb_des_err;
	ret = crypto_register_alg(&cbc_des_alg);
	if (ret)
		goto cbc_des_err;
	ret = crypto_register_alg(&des3_alg);
	if (ret)
		goto des3_err;
	ret = crypto_register_alg(&ecb_des3_alg);
	if (ret)
		goto ecb_des3_err;
	ret = crypto_register_alg(&cbc_des3_alg);
	if (ret)
		goto cbc_des3_err;

	if (crypt_s390_func_available(KMCTR_DEA_ENCRYPT,
			CRYPT_S390_MSA | CRYPT_S390_MSA4) &&
	    crypt_s390_func_available(KMCTR_TDEA_192_ENCRYPT,
			CRYPT_S390_MSA | CRYPT_S390_MSA4)) {
		ret = crypto_register_alg(&ctr_des_alg);
		if (ret)
			goto ctr_des_err;
		ret = crypto_register_alg(&ctr_des3_alg);
		if (ret)
			goto ctr_des3_err;
		ctrblk = (u8 *) __get_free_page(GFP_KERNEL);
		if (!ctrblk) {
			ret = -ENOMEM;
			goto ctr_mem_err;
		}
	}
out:
	return ret;

ctr_mem_err:
	crypto_unregister_alg(&ctr_des3_alg);
ctr_des3_err:
	crypto_unregister_alg(&ctr_des_alg);
ctr_des_err:
	crypto_unregister_alg(&cbc_des3_alg);
cbc_des3_err:
	crypto_unregister_alg(&ecb_des3_alg);
ecb_des3_err:
	crypto_unregister_alg(&des3_alg);
des3_err:
	crypto_unregister_alg(&cbc_des_alg);
cbc_des_err:
	crypto_unregister_alg(&ecb_des_alg);
ecb_des_err:
	crypto_unregister_alg(&des_alg);
des_err:
	goto out;
}

static void __exit des_s390_exit(void)
{
	if (ctrblk) {
		crypto_unregister_alg(&ctr_des_alg);
		crypto_unregister_alg(&ctr_des3_alg);
		free_page((unsigned long) ctrblk);
	}
	crypto_unregister_alg(&cbc_des3_alg);
	crypto_unregister_alg(&ecb_des3_alg);
	crypto_unregister_alg(&des3_alg);
	crypto_unregister_alg(&cbc_des_alg);
	crypto_unregister_alg(&ecb_des_alg);
	crypto_unregister_alg(&des_alg);
}

module_init(des_s390_init);
module_exit(des_s390_exit);

MODULE_ALIAS("des");
MODULE_ALIAS("des3_ede");
=======
	/* Query available functions for KM, KMC and KMCTR */
	cpacf_query(CPACF_KM, &km_functions);
	cpacf_query(CPACF_KMC, &kmc_functions);
	cpacf_query(CPACF_KMCTR, &kmctr_functions);

	if (cpacf_test_func(&km_functions, CPACF_KM_DEA)) {
		ret = des_s390_register_alg(&des_alg);
		if (ret)
			goto out_err;
		ret = des_s390_register_skcipher(&ecb_des_alg);
		if (ret)
			goto out_err;
	}
	if (cpacf_test_func(&kmc_functions, CPACF_KMC_DEA)) {
		ret = des_s390_register_skcipher(&cbc_des_alg);
		if (ret)
			goto out_err;
	}
	if (cpacf_test_func(&km_functions, CPACF_KM_TDEA_192)) {
		ret = des_s390_register_alg(&des3_alg);
		if (ret)
			goto out_err;
		ret = des_s390_register_skcipher(&ecb_des3_alg);
		if (ret)
			goto out_err;
	}
	if (cpacf_test_func(&kmc_functions, CPACF_KMC_TDEA_192)) {
		ret = des_s390_register_skcipher(&cbc_des3_alg);
		if (ret)
			goto out_err;
	}

	if (cpacf_test_func(&kmctr_functions, CPACF_KMCTR_DEA) ||
	    cpacf_test_func(&kmctr_functions, CPACF_KMCTR_TDEA_192)) {
		ctrblk = (u8 *) __get_free_page(GFP_KERNEL);
		if (!ctrblk) {
			ret = -ENOMEM;
			goto out_err;
		}
	}

	if (cpacf_test_func(&kmctr_functions, CPACF_KMCTR_DEA)) {
		ret = des_s390_register_skcipher(&ctr_des_alg);
		if (ret)
			goto out_err;
	}
	if (cpacf_test_func(&kmctr_functions, CPACF_KMCTR_TDEA_192)) {
		ret = des_s390_register_skcipher(&ctr_des3_alg);
		if (ret)
			goto out_err;
	}

	return 0;
out_err:
	des_s390_exit();
	return ret;
}

module_cpu_feature_match(S390_CPU_FEATURE_MSA, des_s390_init);
module_exit(des_s390_exit);

MODULE_ALIAS_CRYPTO("des");
MODULE_ALIAS_CRYPTO("des3_ede");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("DES & Triple DES EDE Cipher Algorithms");
