<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0+
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Cryptographic API.
 *
 * s390 generic implementation of the SHA Secure Hash Algorithms.
 *
 * Copyright IBM Corp. 2007
 * Author(s): Jan Glauber (jang@de.ibm.com)
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <crypto/internal/hash.h>
#include <linux/module.h>
<<<<<<< HEAD
#include "sha.h"
#include "crypt_s390.h"
=======
#include <asm/cpacf.h>
#include "sha.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

int s390_sha_update(struct shash_desc *desc, const u8 *data, unsigned int len)
{
	struct s390_sha_ctx *ctx = shash_desc_ctx(desc);
	unsigned int bsize = crypto_shash_blocksize(desc->tfm);
<<<<<<< HEAD
	unsigned int index;
	int ret;

	/* how much is already in the buffer? */
	index = ctx->count & (bsize - 1);
=======
	unsigned int index, n;

	/* how much is already in the buffer? */
	index = ctx->count % bsize;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ctx->count += len;

	if ((index + len) < bsize)
		goto store;

	/* process one stored block */
	if (index) {
		memcpy(ctx->buf + index, data, bsize - index);
<<<<<<< HEAD
		ret = crypt_s390_kimd(ctx->func, ctx->state, ctx->buf, bsize);
		BUG_ON(ret != bsize);
=======
		cpacf_kimd(ctx->func, ctx->state, ctx->buf, bsize);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		data += bsize - index;
		len -= bsize - index;
		index = 0;
	}

	/* process as many blocks as possible */
	if (len >= bsize) {
<<<<<<< HEAD
		ret = crypt_s390_kimd(ctx->func, ctx->state, data,
				      len & ~(bsize - 1));
		BUG_ON(ret != (len & ~(bsize - 1)));
		data += ret;
		len -= ret;
=======
		n = (len / bsize) * bsize;
		cpacf_kimd(ctx->func, ctx->state, data, n);
		data += n;
		len -= n;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
store:
	if (len)
		memcpy(ctx->buf + index , data, len);

	return 0;
}
EXPORT_SYMBOL_GPL(s390_sha_update);

<<<<<<< HEAD
=======
static int s390_crypto_shash_parmsize(int func)
{
	switch (func) {
	case CPACF_KLMD_SHA_1:
		return 20;
	case CPACF_KLMD_SHA_256:
		return 32;
	case CPACF_KLMD_SHA_512:
		return 64;
	case CPACF_KLMD_SHA3_224:
	case CPACF_KLMD_SHA3_256:
	case CPACF_KLMD_SHA3_384:
	case CPACF_KLMD_SHA3_512:
		return 200;
	default:
		return -EINVAL;
	}
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int s390_sha_final(struct shash_desc *desc, u8 *out)
{
	struct s390_sha_ctx *ctx = shash_desc_ctx(desc);
	unsigned int bsize = crypto_shash_blocksize(desc->tfm);
	u64 bits;
<<<<<<< HEAD
	unsigned int index, end, plen;
	int ret;

	/* SHA-512 uses 128 bit padding length */
	plen = (bsize > SHA256_BLOCK_SIZE) ? 16 : 8;

	/* must perform manual padding */
	index = ctx->count & (bsize - 1);
	end = (index < bsize - plen) ? bsize : (2 * bsize);

	/* start pad with 1 */
	ctx->buf[index] = 0x80;
	index++;

	/* pad with zeros */
	memset(ctx->buf + index, 0x00, end - index - 8);

	/*
	 * Append message length. Well, SHA-512 wants a 128 bit length value,
	 * nevertheless we use u64, should be enough for now...
	 */
	bits = ctx->count * 8;
	memcpy(ctx->buf + end - 8, &bits, sizeof(bits));

	ret = crypt_s390_kimd(ctx->func, ctx->state, ctx->buf, end);
	BUG_ON(ret != end);
=======
	unsigned int n;
	int mbl_offset;

	n = ctx->count % bsize;
	bits = ctx->count * 8;
	mbl_offset = s390_crypto_shash_parmsize(ctx->func);
	if (mbl_offset < 0)
		return -EINVAL;

	mbl_offset = mbl_offset / sizeof(u32);

	/* set total msg bit length (mbl) in CPACF parmblock */
	switch (ctx->func) {
	case CPACF_KLMD_SHA_1:
	case CPACF_KLMD_SHA_256:
		memcpy(ctx->state + mbl_offset, &bits, sizeof(bits));
		break;
	case CPACF_KLMD_SHA_512:
		/*
		 * the SHA512 parmblock has a 128-bit mbl field, clear
		 * high-order u64 field, copy bits to low-order u64 field
		 */
		memset(ctx->state + mbl_offset, 0x00, sizeof(bits));
		mbl_offset += sizeof(u64) / sizeof(u32);
		memcpy(ctx->state + mbl_offset, &bits, sizeof(bits));
		break;
	case CPACF_KLMD_SHA3_224:
	case CPACF_KLMD_SHA3_256:
	case CPACF_KLMD_SHA3_384:
	case CPACF_KLMD_SHA3_512:
		break;
	default:
		return -EINVAL;
	}

	cpacf_klmd(ctx->func, ctx->state, ctx->buf, n);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* copy digest to out */
	memcpy(out, ctx->state, crypto_shash_digestsize(desc->tfm));
	/* wipe context */
	memset(ctx, 0, sizeof *ctx);

	return 0;
}
EXPORT_SYMBOL_GPL(s390_sha_final);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("s390 SHA cipher common functions");
