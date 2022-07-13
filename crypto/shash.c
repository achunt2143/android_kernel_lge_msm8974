<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Synchronous Cryptographic Hash operations.
 *
 * Copyright (c) 2008 Herbert Xu <herbert@gondor.apana.org.au>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 */

#include <crypto/scatterwalk.h>
#include <crypto/internal/hash.h>
#include <linux/err.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/seq_file.h>
#include <linux/cryptouser.h>
#include <net/netlink.h>

#include "internal.h"

static const struct crypto_type crypto_shash_type;
=======
 */

#include <crypto/scatterwalk.h>
#include <linux/cryptouser.h>
#include <linux/err.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/seq_file.h>
#include <linux/string.h>
#include <net/netlink.h>

#include "hash.h"

static inline struct crypto_istat_hash *shash_get_stat(struct shash_alg *alg)
{
	return hash_get_stat(&alg->halg);
}

static inline int crypto_shash_errstat(struct shash_alg *alg, int err)
{
	if (IS_ENABLED(CONFIG_CRYPTO_STATS) && err)
		atomic64_inc(&shash_get_stat(alg)->err_cnt);
	return err;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

int shash_no_setkey(struct crypto_shash *tfm, const u8 *key,
		    unsigned int keylen)
{
	return -ENOSYS;
}
EXPORT_SYMBOL_GPL(shash_no_setkey);

<<<<<<< HEAD
static int shash_setkey_unaligned(struct crypto_shash *tfm, const u8 *key,
				  unsigned int keylen)
{
	struct shash_alg *shash = crypto_shash_alg(tfm);
	unsigned long alignmask = crypto_shash_alignmask(tfm);
	unsigned long absize;
	u8 *buffer, *alignbuffer;
	int err;

	absize = keylen + (alignmask & ~(crypto_tfm_ctx_alignment() - 1));
	buffer = kmalloc(absize, GFP_KERNEL);
	if (!buffer)
		return -ENOMEM;

	alignbuffer = (u8 *)ALIGN((unsigned long)buffer, alignmask + 1);
	memcpy(alignbuffer, key, keylen);
	err = shash->setkey(tfm, alignbuffer, keylen);
	kzfree(buffer);
	return err;
=======
static void shash_set_needkey(struct crypto_shash *tfm, struct shash_alg *alg)
{
	if (crypto_shash_alg_needs_key(alg))
		crypto_shash_set_flags(tfm, CRYPTO_TFM_NEED_KEY);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int crypto_shash_setkey(struct crypto_shash *tfm, const u8 *key,
			unsigned int keylen)
{
	struct shash_alg *shash = crypto_shash_alg(tfm);
<<<<<<< HEAD
	unsigned long alignmask = crypto_shash_alignmask(tfm);

	if ((unsigned long)key & alignmask)
		return shash_setkey_unaligned(tfm, key, keylen);

	return shash->setkey(tfm, key, keylen);
}
EXPORT_SYMBOL_GPL(crypto_shash_setkey);

static inline unsigned int shash_align_buffer_size(unsigned len,
						   unsigned long mask)
{
	return len + (mask & ~(__alignof__(u8 __attribute__ ((aligned))) - 1));
}

static int shash_update_unaligned(struct shash_desc *desc, const u8 *data,
				  unsigned int len)
{
	struct crypto_shash *tfm = desc->tfm;
	struct shash_alg *shash = crypto_shash_alg(tfm);
	unsigned long alignmask = crypto_shash_alignmask(tfm);
	unsigned int unaligned_len = alignmask + 1 -
				     ((unsigned long)data & alignmask);
	u8 ubuf[shash_align_buffer_size(unaligned_len, alignmask)]
		__attribute__ ((aligned));
	u8 *buf = PTR_ALIGN(&ubuf[0], alignmask + 1);
	int err;

	if (unaligned_len > len)
		unaligned_len = len;

	memcpy(buf, data, unaligned_len);
	err = shash->update(desc, buf, unaligned_len);
	memset(buf, 0, unaligned_len);

	return err ?:
	       shash->update(desc, data + unaligned_len, len - unaligned_len);
}
=======
	int err;

	err = shash->setkey(tfm, key, keylen);
	if (unlikely(err)) {
		shash_set_needkey(tfm, shash);
		return err;
	}

	crypto_shash_clear_flags(tfm, CRYPTO_TFM_NEED_KEY);
	return 0;
}
EXPORT_SYMBOL_GPL(crypto_shash_setkey);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

int crypto_shash_update(struct shash_desc *desc, const u8 *data,
			unsigned int len)
{
<<<<<<< HEAD
	struct crypto_shash *tfm = desc->tfm;
	struct shash_alg *shash = crypto_shash_alg(tfm);
	unsigned long alignmask = crypto_shash_alignmask(tfm);

	if ((unsigned long)data & alignmask)
		return shash_update_unaligned(desc, data, len);

	return shash->update(desc, data, len);
}
EXPORT_SYMBOL_GPL(crypto_shash_update);

static int shash_final_unaligned(struct shash_desc *desc, u8 *out)
{
	struct crypto_shash *tfm = desc->tfm;
	unsigned long alignmask = crypto_shash_alignmask(tfm);
	struct shash_alg *shash = crypto_shash_alg(tfm);
	unsigned int ds = crypto_shash_digestsize(tfm);
	u8 ubuf[shash_align_buffer_size(ds, alignmask)]
		__attribute__ ((aligned));
	u8 *buf = PTR_ALIGN(&ubuf[0], alignmask + 1);
	int err;

	err = shash->final(desc, buf);
	if (err)
		goto out;

	memcpy(out, buf, ds);

out:
	memset(buf, 0, ds);
	return err;
}

int crypto_shash_final(struct shash_desc *desc, u8 *out)
{
	struct crypto_shash *tfm = desc->tfm;
	struct shash_alg *shash = crypto_shash_alg(tfm);
	unsigned long alignmask = crypto_shash_alignmask(tfm);

	if ((unsigned long)out & alignmask)
		return shash_final_unaligned(desc, out);

	return shash->final(desc, out);
}
EXPORT_SYMBOL_GPL(crypto_shash_final);

static int shash_finup_unaligned(struct shash_desc *desc, const u8 *data,
				 unsigned int len, u8 *out)
{
	return crypto_shash_update(desc, data, len) ?:
	       crypto_shash_final(desc, out);
=======
	struct shash_alg *shash = crypto_shash_alg(desc->tfm);
	int err;

	if (IS_ENABLED(CONFIG_CRYPTO_STATS))
		atomic64_add(len, &shash_get_stat(shash)->hash_tlen);

	err = shash->update(desc, data, len);

	return crypto_shash_errstat(shash, err);
}
EXPORT_SYMBOL_GPL(crypto_shash_update);

int crypto_shash_final(struct shash_desc *desc, u8 *out)
{
	struct shash_alg *shash = crypto_shash_alg(desc->tfm);
	int err;

	if (IS_ENABLED(CONFIG_CRYPTO_STATS))
		atomic64_inc(&shash_get_stat(shash)->hash_cnt);

	err = shash->final(desc, out);

	return crypto_shash_errstat(shash, err);
}
EXPORT_SYMBOL_GPL(crypto_shash_final);

static int shash_default_finup(struct shash_desc *desc, const u8 *data,
			       unsigned int len, u8 *out)
{
	struct shash_alg *shash = crypto_shash_alg(desc->tfm);

	return shash->update(desc, data, len) ?:
	       shash->final(desc, out);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int crypto_shash_finup(struct shash_desc *desc, const u8 *data,
		       unsigned int len, u8 *out)
{
	struct crypto_shash *tfm = desc->tfm;
	struct shash_alg *shash = crypto_shash_alg(tfm);
<<<<<<< HEAD
	unsigned long alignmask = crypto_shash_alignmask(tfm);

	if (((unsigned long)data | (unsigned long)out) & alignmask)
		return shash_finup_unaligned(desc, data, len, out);

	return shash->finup(desc, data, len, out);
}
EXPORT_SYMBOL_GPL(crypto_shash_finup);

static int shash_digest_unaligned(struct shash_desc *desc, const u8 *data,
				  unsigned int len, u8 *out)
{
	return crypto_shash_init(desc) ?:
	       crypto_shash_finup(desc, data, len, out);
=======
	int err;

	if (IS_ENABLED(CONFIG_CRYPTO_STATS)) {
		struct crypto_istat_hash *istat = shash_get_stat(shash);

		atomic64_inc(&istat->hash_cnt);
		atomic64_add(len, &istat->hash_tlen);
	}

	err = shash->finup(desc, data, len, out);

	return crypto_shash_errstat(shash, err);
}
EXPORT_SYMBOL_GPL(crypto_shash_finup);

static int shash_default_digest(struct shash_desc *desc, const u8 *data,
				unsigned int len, u8 *out)
{
	struct shash_alg *shash = crypto_shash_alg(desc->tfm);

	return shash->init(desc) ?:
	       shash->finup(desc, data, len, out);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int crypto_shash_digest(struct shash_desc *desc, const u8 *data,
			unsigned int len, u8 *out)
{
	struct crypto_shash *tfm = desc->tfm;
	struct shash_alg *shash = crypto_shash_alg(tfm);
<<<<<<< HEAD
	unsigned long alignmask = crypto_shash_alignmask(tfm);

	if (((unsigned long)data | (unsigned long)out) & alignmask)
		return shash_digest_unaligned(desc, data, len, out);

	return shash->digest(desc, data, len, out);
}
EXPORT_SYMBOL_GPL(crypto_shash_digest);

static int shash_default_export(struct shash_desc *desc, void *out)
{
	memcpy(out, shash_desc_ctx(desc), crypto_shash_descsize(desc->tfm));
	return 0;
}

static int shash_default_import(struct shash_desc *desc, const void *in)
{
	memcpy(shash_desc_ctx(desc), in, crypto_shash_descsize(desc->tfm));
	return 0;
}

static int shash_async_setkey(struct crypto_ahash *tfm, const u8 *key,
			      unsigned int keylen)
{
	struct crypto_shash **ctx = crypto_ahash_ctx(tfm);

	return crypto_shash_setkey(*ctx, key, keylen);
}

static int shash_async_init(struct ahash_request *req)
{
	struct crypto_shash **ctx = crypto_ahash_ctx(crypto_ahash_reqtfm(req));
	struct shash_desc *desc = ahash_request_ctx(req);

	desc->tfm = *ctx;
	desc->flags = req->base.flags;

	return crypto_shash_init(desc);
}

int shash_ahash_update(struct ahash_request *req, struct shash_desc *desc)
{
	struct crypto_hash_walk walk;
	int nbytes;

	for (nbytes = crypto_hash_walk_first(req, &walk); nbytes > 0;
	     nbytes = crypto_hash_walk_done(&walk, nbytes))
		nbytes = crypto_shash_update(desc, walk.data, nbytes);

	return nbytes;
}
EXPORT_SYMBOL_GPL(shash_ahash_update);

static int shash_async_update(struct ahash_request *req)
{
	return shash_ahash_update(req, ahash_request_ctx(req));
}

static int shash_async_final(struct ahash_request *req)
{
	return crypto_shash_final(ahash_request_ctx(req), req->result);
}

int shash_ahash_finup(struct ahash_request *req, struct shash_desc *desc)
{
	struct crypto_hash_walk walk;
	int nbytes;

	nbytes = crypto_hash_walk_first(req, &walk);
	if (!nbytes)
		return crypto_shash_final(desc, req->result);

	do {
		nbytes = crypto_hash_walk_last(&walk) ?
			 crypto_shash_finup(desc, walk.data, nbytes,
					    req->result) :
			 crypto_shash_update(desc, walk.data, nbytes);
		nbytes = crypto_hash_walk_done(&walk, nbytes);
	} while (nbytes > 0);

	return nbytes;
}
EXPORT_SYMBOL_GPL(shash_ahash_finup);

static int shash_async_finup(struct ahash_request *req)
{
	struct crypto_shash **ctx = crypto_ahash_ctx(crypto_ahash_reqtfm(req));
	struct shash_desc *desc = ahash_request_ctx(req);

	desc->tfm = *ctx;
	desc->flags = req->base.flags;

	return shash_ahash_finup(req, desc);
}

int shash_ahash_digest(struct ahash_request *req, struct shash_desc *desc)
{
	struct scatterlist *sg = req->src;
	unsigned int offset = sg->offset;
	unsigned int nbytes = req->nbytes;
	int err;

	if (nbytes < min(sg->length, ((unsigned int)(PAGE_SIZE)) - offset)) {
		void *data;

		data = kmap_atomic(sg_page(sg));
		err = crypto_shash_digest(desc, data + offset, nbytes,
					  req->result);
		kunmap_atomic(data);
		crypto_yield(desc->flags);
	} else
		err = crypto_shash_init(desc) ?:
		      shash_ahash_finup(req, desc);

	return err;
}
EXPORT_SYMBOL_GPL(shash_ahash_digest);

static int shash_async_digest(struct ahash_request *req)
{
	struct crypto_shash **ctx = crypto_ahash_ctx(crypto_ahash_reqtfm(req));
	struct shash_desc *desc = ahash_request_ctx(req);

	desc->tfm = *ctx;
	desc->flags = req->base.flags;

	return shash_ahash_digest(req, desc);
}

static int shash_async_export(struct ahash_request *req, void *out)
{
	return crypto_shash_export(ahash_request_ctx(req), out);
}

static int shash_async_import(struct ahash_request *req, const void *in)
{
	struct crypto_shash **ctx = crypto_ahash_ctx(crypto_ahash_reqtfm(req));
	struct shash_desc *desc = ahash_request_ctx(req);

	desc->tfm = *ctx;
	desc->flags = req->base.flags;

	return crypto_shash_import(desc, in);
}

static void crypto_exit_shash_ops_async(struct crypto_tfm *tfm)
{
	struct crypto_shash **ctx = crypto_tfm_ctx(tfm);

	crypto_free_shash(*ctx);
}

int crypto_init_shash_ops_async(struct crypto_tfm *tfm)
{
	struct crypto_alg *calg = tfm->__crt_alg;
	struct shash_alg *alg = __crypto_shash_alg(calg);
	struct crypto_ahash *crt = __crypto_ahash_cast(tfm);
	struct crypto_shash **ctx = crypto_tfm_ctx(tfm);
	struct crypto_shash *shash;

	if (!crypto_mod_get(calg))
		return -EAGAIN;

	shash = crypto_create_tfm(calg, &crypto_shash_type);
	if (IS_ERR(shash)) {
		crypto_mod_put(calg);
		return PTR_ERR(shash);
	}

	*ctx = shash;
	tfm->exit = crypto_exit_shash_ops_async;

	crt->init = shash_async_init;
	crt->update = shash_async_update;
	crt->final = shash_async_final;
	crt->finup = shash_async_finup;
	crt->digest = shash_async_digest;

	if (alg->setkey)
		crt->setkey = shash_async_setkey;
	if (alg->export)
		crt->export = shash_async_export;
	if (alg->import)
		crt->import = shash_async_import;

	crt->reqsize = sizeof(struct shash_desc) + crypto_shash_descsize(shash);

	return 0;
}

static int shash_compat_setkey(struct crypto_hash *tfm, const u8 *key,
			       unsigned int keylen)
{
	struct shash_desc **descp = crypto_hash_ctx(tfm);
	struct shash_desc *desc = *descp;

	return crypto_shash_setkey(desc->tfm, key, keylen);
}

static int shash_compat_init(struct hash_desc *hdesc)
{
	struct shash_desc **descp = crypto_hash_ctx(hdesc->tfm);
	struct shash_desc *desc = *descp;

	desc->flags = hdesc->flags;

	return crypto_shash_init(desc);
}

static int shash_compat_update(struct hash_desc *hdesc, struct scatterlist *sg,
			       unsigned int len)
{
	struct shash_desc **descp = crypto_hash_ctx(hdesc->tfm);
	struct shash_desc *desc = *descp;
	struct crypto_hash_walk walk;
	int nbytes;

	for (nbytes = crypto_hash_walk_first_compat(hdesc, &walk, sg, len);
	     nbytes > 0; nbytes = crypto_hash_walk_done(&walk, nbytes))
		nbytes = crypto_shash_update(desc, walk.data, nbytes);

	return nbytes;
}

static int shash_compat_final(struct hash_desc *hdesc, u8 *out)
{
	struct shash_desc **descp = crypto_hash_ctx(hdesc->tfm);

	return crypto_shash_final(*descp, out);
}

static int shash_compat_digest(struct hash_desc *hdesc, struct scatterlist *sg,
			       unsigned int nbytes, u8 *out)
{
	unsigned int offset = sg->offset;
	int err;

	if (nbytes < min(sg->length, ((unsigned int)(PAGE_SIZE)) - offset)) {
		struct shash_desc **descp = crypto_hash_ctx(hdesc->tfm);
		struct shash_desc *desc = *descp;
		void *data;

		desc->flags = hdesc->flags;

		data = kmap_atomic(sg_page(sg));
		err = crypto_shash_digest(desc, data + offset, nbytes, out);
		kunmap_atomic(data);
		crypto_yield(desc->flags);
		goto out;
	}

	err = shash_compat_init(hdesc);
	if (err)
		goto out;

	err = shash_compat_update(hdesc, sg, nbytes);
	if (err)
		goto out;

	err = shash_compat_final(hdesc, out);

out:
	return err;
}

static void crypto_exit_shash_ops_compat(struct crypto_tfm *tfm)
{
	struct shash_desc **descp = crypto_tfm_ctx(tfm);
	struct shash_desc *desc = *descp;

	crypto_free_shash(desc->tfm);
	kzfree(desc);
}

static int crypto_init_shash_ops_compat(struct crypto_tfm *tfm)
{
	struct hash_tfm *crt = &tfm->crt_hash;
	struct crypto_alg *calg = tfm->__crt_alg;
	struct shash_alg *alg = __crypto_shash_alg(calg);
	struct shash_desc **descp = crypto_tfm_ctx(tfm);
	struct crypto_shash *shash;
	struct shash_desc *desc;

	if (!crypto_mod_get(calg))
		return -EAGAIN;

	shash = crypto_create_tfm(calg, &crypto_shash_type);
	if (IS_ERR(shash)) {
		crypto_mod_put(calg);
		return PTR_ERR(shash);
	}

	desc = kmalloc(sizeof(*desc) + crypto_shash_descsize(shash),
		       GFP_KERNEL);
	if (!desc) {
		crypto_free_shash(shash);
		return -ENOMEM;
	}

	*descp = desc;
	desc->tfm = shash;
	tfm->exit = crypto_exit_shash_ops_compat;

	crt->init = shash_compat_init;
	crt->update = shash_compat_update;
	crt->final  = shash_compat_final;
	crt->digest = shash_compat_digest;
	crt->setkey = shash_compat_setkey;

	crt->digestsize = alg->digestsize;

	return 0;
}

static int crypto_init_shash_ops(struct crypto_tfm *tfm, u32 type, u32 mask)
{
	switch (mask & CRYPTO_ALG_TYPE_MASK) {
	case CRYPTO_ALG_TYPE_HASH_MASK:
		return crypto_init_shash_ops_compat(tfm);
	}

	return -EINVAL;
}

static unsigned int crypto_shash_ctxsize(struct crypto_alg *alg, u32 type,
					 u32 mask)
{
	switch (mask & CRYPTO_ALG_TYPE_MASK) {
	case CRYPTO_ALG_TYPE_HASH_MASK:
		return sizeof(struct shash_desc *);
	}

	return 0;
=======
	int err;

	if (IS_ENABLED(CONFIG_CRYPTO_STATS)) {
		struct crypto_istat_hash *istat = shash_get_stat(shash);

		atomic64_inc(&istat->hash_cnt);
		atomic64_add(len, &istat->hash_tlen);
	}

	if (crypto_shash_get_flags(tfm) & CRYPTO_TFM_NEED_KEY)
		err = -ENOKEY;
	else
		err = shash->digest(desc, data, len, out);

	return crypto_shash_errstat(shash, err);
}
EXPORT_SYMBOL_GPL(crypto_shash_digest);

int crypto_shash_tfm_digest(struct crypto_shash *tfm, const u8 *data,
			    unsigned int len, u8 *out)
{
	SHASH_DESC_ON_STACK(desc, tfm);
	int err;

	desc->tfm = tfm;

	err = crypto_shash_digest(desc, data, len, out);

	shash_desc_zero(desc);

	return err;
}
EXPORT_SYMBOL_GPL(crypto_shash_tfm_digest);

int crypto_shash_export(struct shash_desc *desc, void *out)
{
	struct crypto_shash *tfm = desc->tfm;
	struct shash_alg *shash = crypto_shash_alg(tfm);

	if (shash->export)
		return shash->export(desc, out);

	memcpy(out, shash_desc_ctx(desc), crypto_shash_descsize(tfm));
	return 0;
}
EXPORT_SYMBOL_GPL(crypto_shash_export);

int crypto_shash_import(struct shash_desc *desc, const void *in)
{
	struct crypto_shash *tfm = desc->tfm;
	struct shash_alg *shash = crypto_shash_alg(tfm);

	if (crypto_shash_get_flags(tfm) & CRYPTO_TFM_NEED_KEY)
		return -ENOKEY;

	if (shash->import)
		return shash->import(desc, in);

	memcpy(shash_desc_ctx(desc), in, crypto_shash_descsize(tfm));
	return 0;
}
EXPORT_SYMBOL_GPL(crypto_shash_import);

static void crypto_shash_exit_tfm(struct crypto_tfm *tfm)
{
	struct crypto_shash *hash = __crypto_shash_cast(tfm);
	struct shash_alg *alg = crypto_shash_alg(hash);

	alg->exit_tfm(hash);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int crypto_shash_init_tfm(struct crypto_tfm *tfm)
{
	struct crypto_shash *hash = __crypto_shash_cast(tfm);
<<<<<<< HEAD

	hash->descsize = crypto_shash_alg(hash)->descsize;
	return 0;
}

static unsigned int crypto_shash_extsize(struct crypto_alg *alg)
{
	return alg->cra_ctxsize;
}

#ifdef CONFIG_NET
static int crypto_shash_report(struct sk_buff *skb, struct crypto_alg *alg)
=======
	struct shash_alg *alg = crypto_shash_alg(hash);
	int err;

	hash->descsize = alg->descsize;

	shash_set_needkey(hash, alg);

	if (alg->exit_tfm)
		tfm->exit = crypto_shash_exit_tfm;

	if (!alg->init_tfm)
		return 0;

	err = alg->init_tfm(hash);
	if (err)
		return err;

	/* ->init_tfm() may have increased the descsize. */
	if (WARN_ON_ONCE(hash->descsize > HASH_MAX_DESCSIZE)) {
		if (alg->exit_tfm)
			alg->exit_tfm(hash);
		return -EINVAL;
	}

	return 0;
}

static void crypto_shash_free_instance(struct crypto_instance *inst)
{
	struct shash_instance *shash = shash_instance(inst);

	shash->free(shash);
}

static int __maybe_unused crypto_shash_report(
	struct sk_buff *skb, struct crypto_alg *alg)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct crypto_report_hash rhash;
	struct shash_alg *salg = __crypto_shash_alg(alg);

<<<<<<< HEAD
	strncpy(rhash.type, "shash", sizeof(rhash.type));
=======
	memset(&rhash, 0, sizeof(rhash));

	strscpy(rhash.type, "shash", sizeof(rhash.type));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	rhash.blocksize = alg->cra_blocksize;
	rhash.digestsize = salg->digestsize;

<<<<<<< HEAD
	NLA_PUT(skb, CRYPTOCFGA_REPORT_HASH,
		sizeof(struct crypto_report_hash), &rhash);

	return 0;

nla_put_failure:
	return -EMSGSIZE;
}
#else
static int crypto_shash_report(struct sk_buff *skb, struct crypto_alg *alg)
{
	return -ENOSYS;
}
#endif

static void crypto_shash_show(struct seq_file *m, struct crypto_alg *alg)
	__attribute__ ((unused));
=======
	return nla_put(skb, CRYPTOCFGA_REPORT_HASH, sizeof(rhash), &rhash);
}

static void crypto_shash_show(struct seq_file *m, struct crypto_alg *alg)
	__maybe_unused;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void crypto_shash_show(struct seq_file *m, struct crypto_alg *alg)
{
	struct shash_alg *salg = __crypto_shash_alg(alg);

	seq_printf(m, "type         : shash\n");
	seq_printf(m, "blocksize    : %u\n", alg->cra_blocksize);
	seq_printf(m, "digestsize   : %u\n", salg->digestsize);
}

<<<<<<< HEAD
static const struct crypto_type crypto_shash_type = {
	.ctxsize = crypto_shash_ctxsize,
	.extsize = crypto_shash_extsize,
	.init = crypto_init_shash_ops,
	.init_tfm = crypto_shash_init_tfm,
#ifdef CONFIG_PROC_FS
	.show = crypto_shash_show,
#endif
	.report = crypto_shash_report,
=======
static int __maybe_unused crypto_shash_report_stat(
	struct sk_buff *skb, struct crypto_alg *alg)
{
	return crypto_hash_report_stat(skb, alg, "shash");
}

const struct crypto_type crypto_shash_type = {
	.extsize = crypto_alg_extsize,
	.init_tfm = crypto_shash_init_tfm,
	.free = crypto_shash_free_instance,
#ifdef CONFIG_PROC_FS
	.show = crypto_shash_show,
#endif
#if IS_ENABLED(CONFIG_CRYPTO_USER)
	.report = crypto_shash_report,
#endif
#ifdef CONFIG_CRYPTO_STATS
	.report_stat = crypto_shash_report_stat,
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.maskclear = ~CRYPTO_ALG_TYPE_MASK,
	.maskset = CRYPTO_ALG_TYPE_MASK,
	.type = CRYPTO_ALG_TYPE_SHASH,
	.tfmsize = offsetof(struct crypto_shash, base),
};

<<<<<<< HEAD
=======
int crypto_grab_shash(struct crypto_shash_spawn *spawn,
		      struct crypto_instance *inst,
		      const char *name, u32 type, u32 mask)
{
	spawn->base.frontend = &crypto_shash_type;
	return crypto_grab_spawn(&spawn->base, inst, name, type, mask);
}
EXPORT_SYMBOL_GPL(crypto_grab_shash);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct crypto_shash *crypto_alloc_shash(const char *alg_name, u32 type,
					u32 mask)
{
	return crypto_alloc_tfm(alg_name, &crypto_shash_type, type, mask);
}
EXPORT_SYMBOL_GPL(crypto_alloc_shash);

<<<<<<< HEAD
static int shash_prepare_alg(struct shash_alg *alg)
{
	struct crypto_alg *base = &alg->base;

	if (alg->digestsize > PAGE_SIZE / 8 ||
	    alg->descsize > PAGE_SIZE / 8 ||
	    alg->statesize > PAGE_SIZE / 8)
		return -EINVAL;

	base->cra_type = &crypto_shash_type;
	base->cra_flags &= ~CRYPTO_ALG_TYPE_MASK;
	base->cra_flags |= CRYPTO_ALG_TYPE_SHASH;

	if (!alg->finup)
		alg->finup = shash_finup_unaligned;
	if (!alg->digest)
		alg->digest = shash_digest_unaligned;
	if (!alg->export) {
		alg->export = shash_default_export;
		alg->import = shash_default_import;
		alg->statesize = alg->descsize;
	}
=======
int crypto_has_shash(const char *alg_name, u32 type, u32 mask)
{
	return crypto_type_has_alg(alg_name, &crypto_shash_type, type, mask);
}
EXPORT_SYMBOL_GPL(crypto_has_shash);

struct crypto_shash *crypto_clone_shash(struct crypto_shash *hash)
{
	struct crypto_tfm *tfm = crypto_shash_tfm(hash);
	struct shash_alg *alg = crypto_shash_alg(hash);
	struct crypto_shash *nhash;
	int err;

	if (!crypto_shash_alg_has_setkey(alg)) {
		tfm = crypto_tfm_get(tfm);
		if (IS_ERR(tfm))
			return ERR_CAST(tfm);

		return hash;
	}

	if (!alg->clone_tfm && (alg->init_tfm || alg->base.cra_init))
		return ERR_PTR(-ENOSYS);

	nhash = crypto_clone_tfm(&crypto_shash_type, tfm);
	if (IS_ERR(nhash))
		return nhash;

	nhash->descsize = hash->descsize;

	if (alg->clone_tfm) {
		err = alg->clone_tfm(nhash, hash);
		if (err) {
			crypto_free_shash(nhash);
			return ERR_PTR(err);
		}
	}

	return nhash;
}
EXPORT_SYMBOL_GPL(crypto_clone_shash);

int hash_prepare_alg(struct hash_alg_common *alg)
{
	struct crypto_istat_hash *istat = hash_get_stat(alg);
	struct crypto_alg *base = &alg->base;

	if (alg->digestsize > HASH_MAX_DIGESTSIZE)
		return -EINVAL;

	/* alignmask is not useful for hashes, so it is not supported. */
	if (base->cra_alignmask)
		return -EINVAL;

	base->cra_flags &= ~CRYPTO_ALG_TYPE_MASK;

	if (IS_ENABLED(CONFIG_CRYPTO_STATS))
		memset(istat, 0, sizeof(*istat));

	return 0;
}

static int shash_prepare_alg(struct shash_alg *alg)
{
	struct crypto_alg *base = &alg->halg.base;
	int err;

	if (alg->descsize > HASH_MAX_DESCSIZE)
		return -EINVAL;

	if ((alg->export && !alg->import) || (alg->import && !alg->export))
		return -EINVAL;

	err = hash_prepare_alg(&alg->halg);
	if (err)
		return err;

	base->cra_type = &crypto_shash_type;
	base->cra_flags |= CRYPTO_ALG_TYPE_SHASH;

	/*
	 * Handle missing optional functions.  For each one we can either
	 * install a default here, or we can leave the pointer as NULL and check
	 * the pointer for NULL in crypto_shash_*(), avoiding an indirect call
	 * when the default behavior is desired.  For ->finup and ->digest we
	 * install defaults, since for optimal performance algorithms should
	 * implement these anyway.  On the other hand, for ->import and
	 * ->export the common case and best performance comes from the simple
	 * memcpy of the shash_desc_ctx, so when those pointers are NULL we
	 * leave them NULL and provide the memcpy with no indirect call.
	 */
	if (!alg->finup)
		alg->finup = shash_default_finup;
	if (!alg->digest)
		alg->digest = shash_default_digest;
	if (!alg->export)
		alg->halg.statesize = alg->descsize;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!alg->setkey)
		alg->setkey = shash_no_setkey;

	return 0;
}

int crypto_register_shash(struct shash_alg *alg)
{
	struct crypto_alg *base = &alg->base;
	int err;

	err = shash_prepare_alg(alg);
	if (err)
		return err;

	return crypto_register_alg(base);
}
EXPORT_SYMBOL_GPL(crypto_register_shash);

<<<<<<< HEAD
int crypto_unregister_shash(struct shash_alg *alg)
{
	return crypto_unregister_alg(&alg->base);
=======
void crypto_unregister_shash(struct shash_alg *alg)
{
	crypto_unregister_alg(&alg->base);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(crypto_unregister_shash);

int crypto_register_shashes(struct shash_alg *algs, int count)
{
	int i, ret;

	for (i = 0; i < count; i++) {
		ret = crypto_register_shash(&algs[i]);
		if (ret)
			goto err;
	}

	return 0;

err:
	for (--i; i >= 0; --i)
		crypto_unregister_shash(&algs[i]);

	return ret;
}
EXPORT_SYMBOL_GPL(crypto_register_shashes);

<<<<<<< HEAD
int crypto_unregister_shashes(struct shash_alg *algs, int count)
{
	int i, ret;

	for (i = count - 1; i >= 0; --i) {
		ret = crypto_unregister_shash(&algs[i]);
		if (ret)
			pr_err("Failed to unregister %s %s: %d\n",
			       algs[i].base.cra_driver_name,
			       algs[i].base.cra_name, ret);
	}

	return 0;
=======
void crypto_unregister_shashes(struct shash_alg *algs, int count)
{
	int i;

	for (i = count - 1; i >= 0; --i)
		crypto_unregister_shash(&algs[i]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(crypto_unregister_shashes);

int shash_register_instance(struct crypto_template *tmpl,
			    struct shash_instance *inst)
{
	int err;

<<<<<<< HEAD
=======
	if (WARN_ON(!inst->free))
		return -EINVAL;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	err = shash_prepare_alg(&inst->alg);
	if (err)
		return err;

	return crypto_register_instance(tmpl, shash_crypto_instance(inst));
}
EXPORT_SYMBOL_GPL(shash_register_instance);

<<<<<<< HEAD
void shash_free_instance(struct crypto_instance *inst)
{
	crypto_drop_spawn(crypto_instance_ctx(inst));
	kfree(shash_instance(inst));
}
EXPORT_SYMBOL_GPL(shash_free_instance);

int crypto_init_shash_spawn(struct crypto_shash_spawn *spawn,
			    struct shash_alg *alg,
			    struct crypto_instance *inst)
{
	return crypto_init_spawn2(&spawn->base, &alg->base, inst,
				  &crypto_shash_type);
}
EXPORT_SYMBOL_GPL(crypto_init_shash_spawn);

struct shash_alg *shash_attr_alg(struct rtattr *rta, u32 type, u32 mask)
{
	struct crypto_alg *alg;

	alg = crypto_attr_alg2(rta, &crypto_shash_type, type, mask);
	return IS_ERR(alg) ? ERR_CAST(alg) :
	       container_of(alg, struct shash_alg, base);
}
EXPORT_SYMBOL_GPL(shash_attr_alg);
=======
void shash_free_singlespawn_instance(struct shash_instance *inst)
{
	crypto_drop_spawn(shash_instance_ctx(inst));
	kfree(inst);
}
EXPORT_SYMBOL_GPL(shash_free_singlespawn_instance);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Synchronous cryptographic hash type");
