<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Cryptographic API.
 *
 * Compression operations.
 *
 * Copyright (c) 2002 James Morris <jmorris@intercode.com.au>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 */
#include <linux/types.h>
#include <linux/crypto.h>
#include <linux/errno.h>
#include <linux/string.h>
#include "internal.h"

static int crypto_compress(struct crypto_tfm *tfm,
                            const u8 *src, unsigned int slen,
                            u8 *dst, unsigned int *dlen)
{
	return tfm->__crt_alg->cra_compress.coa_compress(tfm, src, slen, dst,
	                                                 dlen);
}

static int crypto_decompress(struct crypto_tfm *tfm,
                             const u8 *src, unsigned int slen,
                             u8 *dst, unsigned int *dlen)
{
	return tfm->__crt_alg->cra_compress.coa_decompress(tfm, src, slen, dst,
	                                                   dlen);
}

int crypto_init_compress_ops(struct crypto_tfm *tfm)
{
	struct compress_tfm *ops = &tfm->crt_compress;

	ops->cot_compress = crypto_compress;
	ops->cot_decompress = crypto_decompress;

	return 0;
}

void crypto_exit_compress_ops(struct crypto_tfm *tfm)
{
}
=======
 */
#include <linux/crypto.h>
#include "internal.h"

int crypto_comp_compress(struct crypto_comp *comp,
			 const u8 *src, unsigned int slen,
			 u8 *dst, unsigned int *dlen)
{
	struct crypto_tfm *tfm = crypto_comp_tfm(comp);

	return tfm->__crt_alg->cra_compress.coa_compress(tfm, src, slen, dst,
	                                                 dlen);
}
EXPORT_SYMBOL_GPL(crypto_comp_compress);

int crypto_comp_decompress(struct crypto_comp *comp,
			   const u8 *src, unsigned int slen,
			   u8 *dst, unsigned int *dlen)
{
	struct crypto_tfm *tfm = crypto_comp_tfm(comp);

	return tfm->__crt_alg->cra_compress.coa_decompress(tfm, src, slen, dst,
	                                                   dlen);
}
EXPORT_SYMBOL_GPL(crypto_comp_decompress);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
