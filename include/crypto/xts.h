<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _CRYPTO_XTS_H
#define _CRYPTO_XTS_H

#include <crypto/b128ops.h>
<<<<<<< HEAD

struct scatterlist;
struct blkcipher_desc;

#define XTS_BLOCK_SIZE 16

struct xts_crypt_req {
	be128 *tbuf;
	unsigned int tbuflen;

	void *tweak_ctx;
	void (*tweak_fn)(void *ctx, u8* dst, const u8* src);
	void *crypt_ctx;
	void (*crypt_fn)(void *ctx, u8 *blks, unsigned int nbytes);
};

#define XTS_TWEAK_CAST(x) ((void (*)(void *, u8*, const u8*))(x))

int xts_crypt(struct blkcipher_desc *desc, struct scatterlist *dst,
	      struct scatterlist *src, unsigned int nbytes,
	      struct xts_crypt_req *req);
=======
#include <crypto/internal/skcipher.h>
#include <linux/fips.h>

#define XTS_BLOCK_SIZE 16

static inline int xts_verify_key(struct crypto_skcipher *tfm,
				 const u8 *key, unsigned int keylen)
{
	/*
	 * key consists of keys of equal size concatenated, therefore
	 * the length must be even.
	 */
	if (keylen % 2)
		return -EINVAL;

	/*
	 * In FIPS mode only a combined key length of either 256 or
	 * 512 bits is allowed, c.f. FIPS 140-3 IG C.I.
	 */
	if (fips_enabled && keylen != 32 && keylen != 64)
		return -EINVAL;

	/*
	 * Ensure that the AES and tweak key are not identical when
	 * in FIPS mode or the FORBID_WEAK_KEYS flag is set.
	 */
	if ((fips_enabled || (crypto_skcipher_get_flags(tfm) &
			      CRYPTO_TFM_REQ_FORBID_WEAK_KEYS)) &&
	    !crypto_memneq(key, key + (keylen / 2), keylen / 2))
		return -EINVAL;

	return 0;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif  /* _CRYPTO_XTS_H */
