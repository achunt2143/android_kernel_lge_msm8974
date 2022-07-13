<<<<<<< HEAD
/*
 * Cryptographic API.
 *
 * Cipher operations.
 *
 * Copyright (c) 2002 James Morris <jmorris@intercode.com.au>
 * Copyright (c) 2005 Herbert Xu <herbert@gondor.apana.org.au>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 */

=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Cryptographic API.
 *
 * Single-block cipher operations.
 *
 * Copyright (c) 2002 James Morris <jmorris@intercode.com.au>
 * Copyright (c) 2005 Herbert Xu <herbert@gondor.apana.org.au>
 */

#include <crypto/algapi.h>
#include <crypto/internal/cipher.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/kernel.h>
#include <linux/crypto.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/string.h>
#include "internal.h"

<<<<<<< HEAD
static int setkey_unaligned(struct crypto_tfm *tfm, const u8 *key,
			    unsigned int keylen)
{
	struct cipher_alg *cia = &tfm->__crt_alg->cra_cipher;
	unsigned long alignmask = crypto_tfm_alg_alignmask(tfm);
=======
static int setkey_unaligned(struct crypto_cipher *tfm, const u8 *key,
			    unsigned int keylen)
{
	struct cipher_alg *cia = crypto_cipher_alg(tfm);
	unsigned long alignmask = crypto_cipher_alignmask(tfm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int ret;
	u8 *buffer, *alignbuffer;
	unsigned long absize;

	absize = keylen + alignmask;
	buffer = kmalloc(absize, GFP_ATOMIC);
	if (!buffer)
		return -ENOMEM;

	alignbuffer = (u8 *)ALIGN((unsigned long)buffer, alignmask + 1);
	memcpy(alignbuffer, key, keylen);
<<<<<<< HEAD
	ret = cia->cia_setkey(tfm, alignbuffer, keylen);
=======
	ret = cia->cia_setkey(crypto_cipher_tfm(tfm), alignbuffer, keylen);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	memset(alignbuffer, 0, keylen);
	kfree(buffer);
	return ret;

}

<<<<<<< HEAD
static int setkey(struct crypto_tfm *tfm, const u8 *key, unsigned int keylen)
{
	struct cipher_alg *cia = &tfm->__crt_alg->cra_cipher;
	unsigned long alignmask = crypto_tfm_alg_alignmask(tfm);

	tfm->crt_flags &= ~CRYPTO_TFM_RES_MASK;
	if (keylen < cia->cia_min_keysize || keylen > cia->cia_max_keysize) {
		tfm->crt_flags |= CRYPTO_TFM_RES_BAD_KEY_LEN;
		return -EINVAL;
	}
=======
int crypto_cipher_setkey(struct crypto_cipher *tfm,
			 const u8 *key, unsigned int keylen)
{
	struct cipher_alg *cia = crypto_cipher_alg(tfm);
	unsigned long alignmask = crypto_cipher_alignmask(tfm);

	if (keylen < cia->cia_min_keysize || keylen > cia->cia_max_keysize)
		return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if ((unsigned long)key & alignmask)
		return setkey_unaligned(tfm, key, keylen);

<<<<<<< HEAD
	return cia->cia_setkey(tfm, key, keylen);
}

static void cipher_crypt_unaligned(void (*fn)(struct crypto_tfm *, u8 *,
					      const u8 *),
				   struct crypto_tfm *tfm,
				   u8 *dst, const u8 *src)
{
	unsigned long alignmask = crypto_tfm_alg_alignmask(tfm);
	unsigned int size = crypto_tfm_alg_blocksize(tfm);
	u8 buffer[size + alignmask];
	u8 *tmp = (u8 *)ALIGN((unsigned long)buffer, alignmask + 1);

	memcpy(tmp, src, size);
	fn(tfm, tmp, tmp);
	memcpy(dst, tmp, size);
}

static void cipher_encrypt_unaligned(struct crypto_tfm *tfm,
				     u8 *dst, const u8 *src)
{
	unsigned long alignmask = crypto_tfm_alg_alignmask(tfm);
	struct cipher_alg *cipher = &tfm->__crt_alg->cra_cipher;

	if (unlikely(((unsigned long)dst | (unsigned long)src) & alignmask)) {
		cipher_crypt_unaligned(cipher->cia_encrypt, tfm, dst, src);
		return;
	}

	cipher->cia_encrypt(tfm, dst, src);
}

static void cipher_decrypt_unaligned(struct crypto_tfm *tfm,
				     u8 *dst, const u8 *src)
{
	unsigned long alignmask = crypto_tfm_alg_alignmask(tfm);
	struct cipher_alg *cipher = &tfm->__crt_alg->cra_cipher;

	if (unlikely(((unsigned long)dst | (unsigned long)src) & alignmask)) {
		cipher_crypt_unaligned(cipher->cia_decrypt, tfm, dst, src);
		return;
	}

	cipher->cia_decrypt(tfm, dst, src);
}

int crypto_init_cipher_ops(struct crypto_tfm *tfm)
{
	struct cipher_tfm *ops = &tfm->crt_cipher;
	struct cipher_alg *cipher = &tfm->__crt_alg->cra_cipher;

	ops->cit_setkey = setkey;
	ops->cit_encrypt_one = crypto_tfm_alg_alignmask(tfm) ?
		cipher_encrypt_unaligned : cipher->cia_encrypt;
	ops->cit_decrypt_one = crypto_tfm_alg_alignmask(tfm) ?
		cipher_decrypt_unaligned : cipher->cia_decrypt;

	return 0;
}

void crypto_exit_cipher_ops(struct crypto_tfm *tfm)
{
}
=======
	return cia->cia_setkey(crypto_cipher_tfm(tfm), key, keylen);
}
EXPORT_SYMBOL_NS_GPL(crypto_cipher_setkey, CRYPTO_INTERNAL);

static inline void cipher_crypt_one(struct crypto_cipher *tfm,
				    u8 *dst, const u8 *src, bool enc)
{
	unsigned long alignmask = crypto_cipher_alignmask(tfm);
	struct cipher_alg *cia = crypto_cipher_alg(tfm);
	void (*fn)(struct crypto_tfm *, u8 *, const u8 *) =
		enc ? cia->cia_encrypt : cia->cia_decrypt;

	if (unlikely(((unsigned long)dst | (unsigned long)src) & alignmask)) {
		unsigned int bs = crypto_cipher_blocksize(tfm);
		u8 buffer[MAX_CIPHER_BLOCKSIZE + MAX_CIPHER_ALIGNMASK];
		u8 *tmp = (u8 *)ALIGN((unsigned long)buffer, alignmask + 1);

		memcpy(tmp, src, bs);
		fn(crypto_cipher_tfm(tfm), tmp, tmp);
		memcpy(dst, tmp, bs);
	} else {
		fn(crypto_cipher_tfm(tfm), dst, src);
	}
}

void crypto_cipher_encrypt_one(struct crypto_cipher *tfm,
			       u8 *dst, const u8 *src)
{
	cipher_crypt_one(tfm, dst, src, true);
}
EXPORT_SYMBOL_NS_GPL(crypto_cipher_encrypt_one, CRYPTO_INTERNAL);

void crypto_cipher_decrypt_one(struct crypto_cipher *tfm,
			       u8 *dst, const u8 *src)
{
	cipher_crypt_one(tfm, dst, src, false);
}
EXPORT_SYMBOL_NS_GPL(crypto_cipher_decrypt_one, CRYPTO_INTERNAL);

struct crypto_cipher *crypto_clone_cipher(struct crypto_cipher *cipher)
{
	struct crypto_tfm *tfm = crypto_cipher_tfm(cipher);
	struct crypto_alg *alg = tfm->__crt_alg;
	struct crypto_cipher *ncipher;
	struct crypto_tfm *ntfm;

	if (alg->cra_init)
		return ERR_PTR(-ENOSYS);

	if (unlikely(!crypto_mod_get(alg)))
		return ERR_PTR(-ESTALE);

	ntfm = __crypto_alloc_tfmgfp(alg, CRYPTO_ALG_TYPE_CIPHER,
				     CRYPTO_ALG_TYPE_MASK, GFP_ATOMIC);
	if (IS_ERR(ntfm)) {
		crypto_mod_put(alg);
		return ERR_CAST(ntfm);
	}

	ntfm->crt_flags = tfm->crt_flags;

	ncipher = __crypto_cipher_cast(ntfm);

	return ncipher;
}
EXPORT_SYMBOL_GPL(crypto_clone_cipher);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
