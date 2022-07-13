/*
 *  linux/net/sunrpc/gss_krb5_crypto.c
 *
 *  Copyright (c) 2000-2008 The Regents of the University of Michigan.
 *  All rights reserved.
 *
 *  Andy Adamson   <andros@umich.edu>
 *  Bruce Fields   <bfields@umich.edu>
 */

/*
 * Copyright (C) 1998 by the FundsXpress, INC.
 *
 * All rights reserved.
 *
 * Export of this software from the United States of America may require
 * a specific license from the United States Government.  It is the
 * responsibility of any person or organization contemplating export to
 * obtain such a license before exporting.
 *
 * WITHIN THAT CONSTRAINT, permission to use, copy, modify, and
 * distribute this software and its documentation for any purpose and
 * without fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright notice and
 * this permission notice appear in supporting documentation, and that
 * the name of FundsXpress. not be used in advertising or publicity pertaining
 * to distribution of the software without specific, written prior
 * permission.  FundsXpress makes no representations about the suitability of
 * this software for any purpose.  It is provided "as is" without express
 * or implied warranty.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

<<<<<<< HEAD
=======
#include <crypto/hash.h>
#include <crypto/skcipher.h>
#include <crypto/utils.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/err.h>
#include <linux/types.h>
#include <linux/mm.h>
#include <linux/scatterlist.h>
<<<<<<< HEAD
#include <linux/crypto.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/highmem.h>
#include <linux/pagemap.h>
#include <linux/random.h>
#include <linux/sunrpc/gss_krb5.h>
#include <linux/sunrpc/xdr.h>
<<<<<<< HEAD

#ifdef RPC_DEBUG
# define RPCDBG_FACILITY        RPCDBG_AUTH
#endif

u32
krb5_encrypt(
	struct crypto_blkcipher *tfm,
=======
#include <kunit/visibility.h>

#include "gss_krb5_internal.h"

#if IS_ENABLED(CONFIG_SUNRPC_DEBUG)
# define RPCDBG_FACILITY        RPCDBG_AUTH
#endif

/**
 * krb5_make_confounder - Generate a confounder string
 * @p: memory location into which to write the string
 * @conflen: string length to write, in octets
 *
 * RFCs 1964 and 3961 mention only "a random confounder" without going
 * into detail about its function or cryptographic requirements. The
 * assumed purpose is to prevent repeated encryption of a plaintext with
 * the same key from generating the same ciphertext. It is also used to
 * pad minimum plaintext length to at least a single cipher block.
 *
 * However, in situations like the GSS Kerberos 5 mechanism, where the
 * encryption IV is always all zeroes, the confounder also effectively
 * functions like an IV. Thus, not only must it be unique from message
 * to message, but it must also be difficult to predict. Otherwise an
 * attacker can correlate the confounder to previous or future values,
 * making the encryption easier to break.
 *
 * Given that the primary consumer of this encryption mechanism is a
 * network storage protocol, a type of traffic that often carries
 * predictable payloads (eg, all zeroes when reading unallocated blocks
 * from a file), our confounder generation has to be cryptographically
 * strong.
 */
void krb5_make_confounder(u8 *p, int conflen)
{
	get_random_bytes(p, conflen);
}

/**
 * krb5_encrypt - simple encryption of an RPCSEC GSS payload
 * @tfm: initialized cipher transform
 * @iv: pointer to an IV
 * @in: plaintext to encrypt
 * @out: OUT: ciphertext
 * @length: length of input and output buffers, in bytes
 *
 * @iv may be NULL to force the use of an all-zero IV.
 * The buffer containing the IV must be as large as the
 * cipher's ivsize.
 *
 * Return values:
 *   %0: @in successfully encrypted into @out
 *   negative errno: @in not encrypted
 */
u32
krb5_encrypt(
	struct crypto_sync_skcipher *tfm,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	void * iv,
	void * in,
	void * out,
	int length)
{
	u32 ret = -EINVAL;
	struct scatterlist sg[1];
	u8 local_iv[GSS_KRB5_MAX_BLOCKSIZE] = {0};
<<<<<<< HEAD
	struct blkcipher_desc desc = { .tfm = tfm, .info = local_iv };

	if (length % crypto_blkcipher_blocksize(tfm) != 0)
		goto out;

	if (crypto_blkcipher_ivsize(tfm) > GSS_KRB5_MAX_BLOCKSIZE) {
		dprintk("RPC:       gss_k5encrypt: tfm iv size too large %d\n",
			crypto_blkcipher_ivsize(tfm));
=======
	SYNC_SKCIPHER_REQUEST_ON_STACK(req, tfm);

	if (length % crypto_sync_skcipher_blocksize(tfm) != 0)
		goto out;

	if (crypto_sync_skcipher_ivsize(tfm) > GSS_KRB5_MAX_BLOCKSIZE) {
		dprintk("RPC:       gss_k5encrypt: tfm iv size too large %d\n",
			crypto_sync_skcipher_ivsize(tfm));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;
	}

	if (iv)
<<<<<<< HEAD
		memcpy(local_iv, iv, crypto_blkcipher_ivsize(tfm));
=======
		memcpy(local_iv, iv, crypto_sync_skcipher_ivsize(tfm));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	memcpy(out, in, length);
	sg_init_one(sg, out, length);

<<<<<<< HEAD
	ret = crypto_blkcipher_encrypt_iv(&desc, sg, sg, length);
=======
	skcipher_request_set_sync_tfm(req, tfm);
	skcipher_request_set_callback(req, 0, NULL, NULL);
	skcipher_request_set_crypt(req, sg, sg, length, local_iv);

	ret = crypto_skcipher_encrypt(req);
	skcipher_request_zero(req);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	dprintk("RPC:       krb5_encrypt returns %d\n", ret);
	return ret;
}

<<<<<<< HEAD
u32
krb5_decrypt(
     struct crypto_blkcipher *tfm,
=======
/**
 * krb5_decrypt - simple decryption of an RPCSEC GSS payload
 * @tfm: initialized cipher transform
 * @iv: pointer to an IV
 * @in: ciphertext to decrypt
 * @out: OUT: plaintext
 * @length: length of input and output buffers, in bytes
 *
 * @iv may be NULL to force the use of an all-zero IV.
 * The buffer containing the IV must be as large as the
 * cipher's ivsize.
 *
 * Return values:
 *   %0: @in successfully decrypted into @out
 *   negative errno: @in not decrypted
 */
u32
krb5_decrypt(
     struct crypto_sync_skcipher *tfm,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
     void * iv,
     void * in,
     void * out,
     int length)
{
	u32 ret = -EINVAL;
	struct scatterlist sg[1];
	u8 local_iv[GSS_KRB5_MAX_BLOCKSIZE] = {0};
<<<<<<< HEAD
	struct blkcipher_desc desc = { .tfm = tfm, .info = local_iv };

	if (length % crypto_blkcipher_blocksize(tfm) != 0)
		goto out;

	if (crypto_blkcipher_ivsize(tfm) > GSS_KRB5_MAX_BLOCKSIZE) {
		dprintk("RPC:       gss_k5decrypt: tfm iv size too large %d\n",
			crypto_blkcipher_ivsize(tfm));
		goto out;
	}
	if (iv)
		memcpy(local_iv,iv, crypto_blkcipher_ivsize(tfm));
=======
	SYNC_SKCIPHER_REQUEST_ON_STACK(req, tfm);

	if (length % crypto_sync_skcipher_blocksize(tfm) != 0)
		goto out;

	if (crypto_sync_skcipher_ivsize(tfm) > GSS_KRB5_MAX_BLOCKSIZE) {
		dprintk("RPC:       gss_k5decrypt: tfm iv size too large %d\n",
			crypto_sync_skcipher_ivsize(tfm));
		goto out;
	}
	if (iv)
		memcpy(local_iv, iv, crypto_sync_skcipher_ivsize(tfm));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	memcpy(out, in, length);
	sg_init_one(sg, out, length);

<<<<<<< HEAD
	ret = crypto_blkcipher_decrypt_iv(&desc, sg, sg, length);
=======
	skcipher_request_set_sync_tfm(req, tfm);
	skcipher_request_set_callback(req, 0, NULL, NULL);
	skcipher_request_set_crypt(req, sg, sg, length, local_iv);

	ret = crypto_skcipher_decrypt(req);
	skcipher_request_zero(req);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	dprintk("RPC:       gss_k5decrypt returns %d\n",ret);
	return ret;
}

static int
checksummer(struct scatterlist *sg, void *data)
{
<<<<<<< HEAD
	struct hash_desc *desc = data;

	return crypto_hash_update(desc, sg, sg->length);
}

static int
arcfour_hmac_md5_usage_to_salt(unsigned int usage, u8 salt[4])
{
	unsigned int ms_usage;

	switch (usage) {
	case KG_USAGE_SIGN:
		ms_usage = 15;
		break;
	case KG_USAGE_SEAL:
		ms_usage = 13;
		break;
	default:
		return -EINVAL;
	}
	salt[0] = (ms_usage >> 0) & 0xff;
	salt[1] = (ms_usage >> 8) & 0xff;
	salt[2] = (ms_usage >> 16) & 0xff;
	salt[3] = (ms_usage >> 24) & 0xff;

	return 0;
}

static u32
make_checksum_hmac_md5(struct krb5_ctx *kctx, char *header, int hdrlen,
		       struct xdr_buf *body, int body_offset, u8 *cksumkey,
		       unsigned int usage, struct xdr_netobj *cksumout)
{
	struct hash_desc                desc;
	struct scatterlist              sg[1];
	int err;
	u8 checksumdata[GSS_KRB5_MAX_CKSUM_LEN];
	u8 rc4salt[4];
	struct crypto_hash *md5;
	struct crypto_hash *hmac_md5;

	if (cksumkey == NULL)
		return GSS_S_FAILURE;

	if (cksumout->len < kctx->gk5e->cksumlength) {
		dprintk("%s: checksum buffer length, %u, too small for %s\n",
			__func__, cksumout->len, kctx->gk5e->name);
		return GSS_S_FAILURE;
	}

	if (arcfour_hmac_md5_usage_to_salt(usage, rc4salt)) {
		dprintk("%s: invalid usage value %u\n", __func__, usage);
		return GSS_S_FAILURE;
	}

	md5 = crypto_alloc_hash("md5", 0, CRYPTO_ALG_ASYNC);
	if (IS_ERR(md5))
		return GSS_S_FAILURE;

	hmac_md5 = crypto_alloc_hash(kctx->gk5e->cksum_name, 0,
				     CRYPTO_ALG_ASYNC);
	if (IS_ERR(hmac_md5)) {
		crypto_free_hash(md5);
		return GSS_S_FAILURE;
	}

	desc.tfm = md5;
	desc.flags = CRYPTO_TFM_REQ_MAY_SLEEP;

	err = crypto_hash_init(&desc);
	if (err)
		goto out;
	sg_init_one(sg, rc4salt, 4);
	err = crypto_hash_update(&desc, sg, 4);
	if (err)
		goto out;

	sg_init_one(sg, header, hdrlen);
	err = crypto_hash_update(&desc, sg, hdrlen);
	if (err)
		goto out;
	err = xdr_process_buf(body, body_offset, body->len - body_offset,
			      checksummer, &desc);
	if (err)
		goto out;
	err = crypto_hash_final(&desc, checksumdata);
	if (err)
		goto out;

	desc.tfm = hmac_md5;
	desc.flags = CRYPTO_TFM_REQ_MAY_SLEEP;

	err = crypto_hash_init(&desc);
	if (err)
		goto out;
	err = crypto_hash_setkey(hmac_md5, cksumkey, kctx->gk5e->keylength);
	if (err)
		goto out;

	sg_init_one(sg, checksumdata, crypto_hash_digestsize(md5));
	err = crypto_hash_digest(&desc, sg, crypto_hash_digestsize(md5),
				 checksumdata);
	if (err)
		goto out;

	memcpy(cksumout->data, checksumdata, kctx->gk5e->cksumlength);
	cksumout->len = kctx->gk5e->cksumlength;
out:
	crypto_free_hash(md5);
	crypto_free_hash(hmac_md5);
	return err ? GSS_S_FAILURE : 0;
=======
	struct ahash_request *req = data;

	ahash_request_set_crypt(req, sg, NULL, sg->length);

	return crypto_ahash_update(req);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * checksum the plaintext data and hdrlen bytes of the token header
 * The checksum is performed over the first 8 bytes of the
 * gss token header and then over the data body
 */
u32
make_checksum(struct krb5_ctx *kctx, char *header, int hdrlen,
	      struct xdr_buf *body, int body_offset, u8 *cksumkey,
	      unsigned int usage, struct xdr_netobj *cksumout)
{
<<<<<<< HEAD
	struct hash_desc                desc;
	struct scatterlist              sg[1];
	int err;
	u8 checksumdata[GSS_KRB5_MAX_CKSUM_LEN];
	unsigned int checksumlen;

	if (kctx->gk5e->ctype == CKSUMTYPE_HMAC_MD5_ARCFOUR)
		return make_checksum_hmac_md5(kctx, header, hdrlen,
					      body, body_offset,
					      cksumkey, usage, cksumout);

=======
	struct crypto_ahash *tfm;
	struct ahash_request *req;
	struct scatterlist              sg[1];
	int err = -1;
	u8 *checksumdata;
	unsigned int checksumlen;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (cksumout->len < kctx->gk5e->cksumlength) {
		dprintk("%s: checksum buffer length, %u, too small for %s\n",
			__func__, cksumout->len, kctx->gk5e->name);
		return GSS_S_FAILURE;
	}

<<<<<<< HEAD
	desc.tfm = crypto_alloc_hash(kctx->gk5e->cksum_name, 0, CRYPTO_ALG_ASYNC);
	if (IS_ERR(desc.tfm))
		return GSS_S_FAILURE;
	desc.flags = CRYPTO_TFM_REQ_MAY_SLEEP;

	checksumlen = crypto_hash_digestsize(desc.tfm);

	if (cksumkey != NULL) {
		err = crypto_hash_setkey(desc.tfm, cksumkey,
					 kctx->gk5e->keylength);
=======
	checksumdata = kmalloc(GSS_KRB5_MAX_CKSUM_LEN, GFP_KERNEL);
	if (checksumdata == NULL)
		return GSS_S_FAILURE;

	tfm = crypto_alloc_ahash(kctx->gk5e->cksum_name, 0, CRYPTO_ALG_ASYNC);
	if (IS_ERR(tfm))
		goto out_free_cksum;

	req = ahash_request_alloc(tfm, GFP_KERNEL);
	if (!req)
		goto out_free_ahash;

	ahash_request_set_callback(req, CRYPTO_TFM_REQ_MAY_SLEEP, NULL, NULL);

	checksumlen = crypto_ahash_digestsize(tfm);

	if (cksumkey != NULL) {
		err = crypto_ahash_setkey(tfm, cksumkey,
					  kctx->gk5e->keylength);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (err)
			goto out;
	}

<<<<<<< HEAD
	err = crypto_hash_init(&desc);
	if (err)
		goto out;
	sg_init_one(sg, header, hdrlen);
	err = crypto_hash_update(&desc, sg, hdrlen);
	if (err)
		goto out;
	err = xdr_process_buf(body, body_offset, body->len - body_offset,
			      checksummer, &desc);
	if (err)
		goto out;
	err = crypto_hash_final(&desc, checksumdata);
=======
	err = crypto_ahash_init(req);
	if (err)
		goto out;
	sg_init_one(sg, header, hdrlen);
	ahash_request_set_crypt(req, sg, NULL, hdrlen);
	err = crypto_ahash_update(req);
	if (err)
		goto out;
	err = xdr_process_buf(body, body_offset, body->len - body_offset,
			      checksummer, req);
	if (err)
		goto out;
	ahash_request_set_crypt(req, NULL, checksumdata, 0);
	err = crypto_ahash_final(req);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err)
		goto out;

	switch (kctx->gk5e->ctype) {
	case CKSUMTYPE_RSA_MD5:
<<<<<<< HEAD
		err = kctx->gk5e->encrypt(kctx->seq, NULL, checksumdata,
					  checksumdata, checksumlen);
=======
		err = krb5_encrypt(kctx->seq, NULL, checksumdata,
				   checksumdata, checksumlen);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (err)
			goto out;
		memcpy(cksumout->data,
		       checksumdata + checksumlen - kctx->gk5e->cksumlength,
		       kctx->gk5e->cksumlength);
		break;
	case CKSUMTYPE_HMAC_SHA1_DES3:
		memcpy(cksumout->data, checksumdata, kctx->gk5e->cksumlength);
		break;
	default:
		BUG();
		break;
	}
	cksumout->len = kctx->gk5e->cksumlength;
out:
<<<<<<< HEAD
	crypto_free_hash(desc.tfm);
	return err ? GSS_S_FAILURE : 0;
}

/*
 * checksum the plaintext data and hdrlen bytes of the token header
 * Per rfc4121, sec. 4.2.4, the checksum is performed over the data
 * body then over the first 16 octets of the MIC token
 * Inclusion of the header data in the calculation of the
 * checksum is optional.
 */
u32
make_checksum_v2(struct krb5_ctx *kctx, char *header, int hdrlen,
		 struct xdr_buf *body, int body_offset, u8 *cksumkey,
		 unsigned int usage, struct xdr_netobj *cksumout)
{
	struct hash_desc desc;
	struct scatterlist sg[1];
	int err;
	u8 checksumdata[GSS_KRB5_MAX_CKSUM_LEN];
	unsigned int checksumlen;

	if (kctx->gk5e->keyed_cksum == 0) {
		dprintk("%s: expected keyed hash for %s\n",
			__func__, kctx->gk5e->name);
		return GSS_S_FAILURE;
	}
	if (cksumkey == NULL) {
		dprintk("%s: no key supplied for %s\n",
			__func__, kctx->gk5e->name);
		return GSS_S_FAILURE;
	}

	desc.tfm = crypto_alloc_hash(kctx->gk5e->cksum_name, 0,
							CRYPTO_ALG_ASYNC);
	if (IS_ERR(desc.tfm))
		return GSS_S_FAILURE;
	checksumlen = crypto_hash_digestsize(desc.tfm);
	desc.flags = CRYPTO_TFM_REQ_MAY_SLEEP;

	err = crypto_hash_setkey(desc.tfm, cksumkey, kctx->gk5e->keylength);
	if (err)
		goto out;

	err = crypto_hash_init(&desc);
	if (err)
		goto out;
	err = xdr_process_buf(body, body_offset, body->len - body_offset,
			      checksummer, &desc);
	if (err)
		goto out;
	if (header != NULL) {
		sg_init_one(sg, header, hdrlen);
		err = crypto_hash_update(&desc, sg, hdrlen);
		if (err)
			goto out;
	}
	err = crypto_hash_final(&desc, checksumdata);
	if (err)
		goto out;

	cksumout->len = kctx->gk5e->cksumlength;

	switch (kctx->gk5e->ctype) {
	case CKSUMTYPE_HMAC_SHA1_96_AES128:
	case CKSUMTYPE_HMAC_SHA1_96_AES256:
		/* note that this truncates the hash */
		memcpy(cksumout->data, checksumdata, kctx->gk5e->cksumlength);
		break;
	default:
		BUG();
		break;
	}
out:
	crypto_free_hash(desc.tfm);
	return err ? GSS_S_FAILURE : 0;
}

struct encryptor_desc {
	u8 iv[GSS_KRB5_MAX_BLOCKSIZE];
	struct blkcipher_desc desc;
=======
	ahash_request_free(req);
out_free_ahash:
	crypto_free_ahash(tfm);
out_free_cksum:
	kfree(checksumdata);
	return err ? GSS_S_FAILURE : 0;
}

/**
 * gss_krb5_checksum - Compute the MAC for a GSS Wrap or MIC token
 * @tfm: an initialized hash transform
 * @header: pointer to a buffer containing the token header, or NULL
 * @hdrlen: number of octets in @header
 * @body: xdr_buf containing an RPC message (body.len is the message length)
 * @body_offset: byte offset into @body to start checksumming
 * @cksumout: OUT: a buffer to be filled in with the computed HMAC
 *
 * Usually expressed as H = HMAC(K, message)[1..h] .
 *
 * Caller provides the truncation length of the output token (h) in
 * cksumout.len.
 *
 * Return values:
 *   %GSS_S_COMPLETE: Digest computed, @cksumout filled in
 *   %GSS_S_FAILURE: Call failed
 */
u32
gss_krb5_checksum(struct crypto_ahash *tfm, char *header, int hdrlen,
		  const struct xdr_buf *body, int body_offset,
		  struct xdr_netobj *cksumout)
{
	struct ahash_request *req;
	int err = -ENOMEM;
	u8 *checksumdata;

	checksumdata = kmalloc(crypto_ahash_digestsize(tfm), GFP_KERNEL);
	if (!checksumdata)
		return GSS_S_FAILURE;

	req = ahash_request_alloc(tfm, GFP_KERNEL);
	if (!req)
		goto out_free_cksum;
	ahash_request_set_callback(req, CRYPTO_TFM_REQ_MAY_SLEEP, NULL, NULL);
	err = crypto_ahash_init(req);
	if (err)
		goto out_free_ahash;

	/*
	 * Per RFC 4121 Section 4.2.4, the checksum is performed over the
	 * data body first, then over the octets in "header".
	 */
	err = xdr_process_buf(body, body_offset, body->len - body_offset,
			      checksummer, req);
	if (err)
		goto out_free_ahash;
	if (header) {
		struct scatterlist sg[1];

		sg_init_one(sg, header, hdrlen);
		ahash_request_set_crypt(req, sg, NULL, hdrlen);
		err = crypto_ahash_update(req);
		if (err)
			goto out_free_ahash;
	}

	ahash_request_set_crypt(req, NULL, checksumdata, 0);
	err = crypto_ahash_final(req);
	if (err)
		goto out_free_ahash;

	memcpy(cksumout->data, checksumdata,
	       min_t(int, cksumout->len, crypto_ahash_digestsize(tfm)));

out_free_ahash:
	ahash_request_free(req);
out_free_cksum:
	kfree_sensitive(checksumdata);
	return err ? GSS_S_FAILURE : GSS_S_COMPLETE;
}
EXPORT_SYMBOL_IF_KUNIT(gss_krb5_checksum);

struct encryptor_desc {
	u8 iv[GSS_KRB5_MAX_BLOCKSIZE];
	struct skcipher_request *req;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int pos;
	struct xdr_buf *outbuf;
	struct page **pages;
	struct scatterlist infrags[4];
	struct scatterlist outfrags[4];
	int fragno;
	int fraglen;
};

static int
encryptor(struct scatterlist *sg, void *data)
{
	struct encryptor_desc *desc = data;
	struct xdr_buf *outbuf = desc->outbuf;
<<<<<<< HEAD
=======
	struct crypto_sync_skcipher *tfm =
		crypto_sync_skcipher_reqtfm(desc->req);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct page *in_page;
	int thislen = desc->fraglen + sg->length;
	int fraglen, ret;
	int page_pos;

	/* Worst case is 4 fragments: head, end of page 1, start
	 * of page 2, tail.  Anything more is a bug. */
	BUG_ON(desc->fragno > 3);

	page_pos = desc->pos - outbuf->head[0].iov_len;
	if (page_pos >= 0 && page_pos < outbuf->page_len) {
		/* pages are not in place: */
<<<<<<< HEAD
		int i = (page_pos + outbuf->page_base) >> PAGE_CACHE_SHIFT;
=======
		int i = (page_pos + outbuf->page_base) >> PAGE_SHIFT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		in_page = desc->pages[i];
	} else {
		in_page = sg_page(sg);
	}
	sg_set_page(&desc->infrags[desc->fragno], in_page, sg->length,
		    sg->offset);
	sg_set_page(&desc->outfrags[desc->fragno], sg_page(sg), sg->length,
		    sg->offset);
	desc->fragno++;
	desc->fraglen += sg->length;
	desc->pos += sg->length;

<<<<<<< HEAD
	fraglen = thislen & (crypto_blkcipher_blocksize(desc->desc.tfm) - 1);
=======
	fraglen = thislen & (crypto_sync_skcipher_blocksize(tfm) - 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	thislen -= fraglen;

	if (thislen == 0)
		return 0;

	sg_mark_end(&desc->infrags[desc->fragno - 1]);
	sg_mark_end(&desc->outfrags[desc->fragno - 1]);

<<<<<<< HEAD
	ret = crypto_blkcipher_encrypt_iv(&desc->desc, desc->outfrags,
					  desc->infrags, thislen);
=======
	skcipher_request_set_crypt(desc->req, desc->infrags, desc->outfrags,
				   thislen, desc->iv);

	ret = crypto_skcipher_encrypt(desc->req);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ret)
		return ret;

	sg_init_table(desc->infrags, 4);
	sg_init_table(desc->outfrags, 4);

	if (fraglen) {
		sg_set_page(&desc->outfrags[0], sg_page(sg), fraglen,
				sg->offset + sg->length - fraglen);
		desc->infrags[0] = desc->outfrags[0];
		sg_assign_page(&desc->infrags[0], in_page);
		desc->fragno = 1;
		desc->fraglen = fraglen;
	} else {
		desc->fragno = 0;
		desc->fraglen = 0;
	}
	return 0;
}

int
<<<<<<< HEAD
gss_encrypt_xdr_buf(struct crypto_blkcipher *tfm, struct xdr_buf *buf,
=======
gss_encrypt_xdr_buf(struct crypto_sync_skcipher *tfm, struct xdr_buf *buf,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		    int offset, struct page **pages)
{
	int ret;
	struct encryptor_desc desc;
<<<<<<< HEAD

	BUG_ON((buf->len - offset) % crypto_blkcipher_blocksize(tfm) != 0);

	memset(desc.iv, 0, sizeof(desc.iv));
	desc.desc.tfm = tfm;
	desc.desc.info = desc.iv;
	desc.desc.flags = 0;
=======
	SYNC_SKCIPHER_REQUEST_ON_STACK(req, tfm);

	BUG_ON((buf->len - offset) % crypto_sync_skcipher_blocksize(tfm) != 0);

	skcipher_request_set_sync_tfm(req, tfm);
	skcipher_request_set_callback(req, 0, NULL, NULL);

	memset(desc.iv, 0, sizeof(desc.iv));
	desc.req = req;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	desc.pos = offset;
	desc.outbuf = buf;
	desc.pages = pages;
	desc.fragno = 0;
	desc.fraglen = 0;

	sg_init_table(desc.infrags, 4);
	sg_init_table(desc.outfrags, 4);

	ret = xdr_process_buf(buf, offset, buf->len - offset, encryptor, &desc);
<<<<<<< HEAD
=======
	skcipher_request_zero(req);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

struct decryptor_desc {
	u8 iv[GSS_KRB5_MAX_BLOCKSIZE];
<<<<<<< HEAD
	struct blkcipher_desc desc;
=======
	struct skcipher_request *req;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct scatterlist frags[4];
	int fragno;
	int fraglen;
};

static int
decryptor(struct scatterlist *sg, void *data)
{
	struct decryptor_desc *desc = data;
	int thislen = desc->fraglen + sg->length;
<<<<<<< HEAD
=======
	struct crypto_sync_skcipher *tfm =
		crypto_sync_skcipher_reqtfm(desc->req);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int fraglen, ret;

	/* Worst case is 4 fragments: head, end of page 1, start
	 * of page 2, tail.  Anything more is a bug. */
	BUG_ON(desc->fragno > 3);
	sg_set_page(&desc->frags[desc->fragno], sg_page(sg), sg->length,
		    sg->offset);
	desc->fragno++;
	desc->fraglen += sg->length;

<<<<<<< HEAD
	fraglen = thislen & (crypto_blkcipher_blocksize(desc->desc.tfm) - 1);
=======
	fraglen = thislen & (crypto_sync_skcipher_blocksize(tfm) - 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	thislen -= fraglen;

	if (thislen == 0)
		return 0;

	sg_mark_end(&desc->frags[desc->fragno - 1]);

<<<<<<< HEAD
	ret = crypto_blkcipher_decrypt_iv(&desc->desc, desc->frags,
					  desc->frags, thislen);
=======
	skcipher_request_set_crypt(desc->req, desc->frags, desc->frags,
				   thislen, desc->iv);

	ret = crypto_skcipher_decrypt(desc->req);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ret)
		return ret;

	sg_init_table(desc->frags, 4);

	if (fraglen) {
		sg_set_page(&desc->frags[0], sg_page(sg), fraglen,
				sg->offset + sg->length - fraglen);
		desc->fragno = 1;
		desc->fraglen = fraglen;
	} else {
		desc->fragno = 0;
		desc->fraglen = 0;
	}
	return 0;
}

int
<<<<<<< HEAD
gss_decrypt_xdr_buf(struct crypto_blkcipher *tfm, struct xdr_buf *buf,
		    int offset)
{
	struct decryptor_desc desc;

	/* XXXJBF: */
	BUG_ON((buf->len - offset) % crypto_blkcipher_blocksize(tfm) != 0);

	memset(desc.iv, 0, sizeof(desc.iv));
	desc.desc.tfm = tfm;
	desc.desc.info = desc.iv;
	desc.desc.flags = 0;
=======
gss_decrypt_xdr_buf(struct crypto_sync_skcipher *tfm, struct xdr_buf *buf,
		    int offset)
{
	int ret;
	struct decryptor_desc desc;
	SYNC_SKCIPHER_REQUEST_ON_STACK(req, tfm);

	/* XXXJBF: */
	BUG_ON((buf->len - offset) % crypto_sync_skcipher_blocksize(tfm) != 0);

	skcipher_request_set_sync_tfm(req, tfm);
	skcipher_request_set_callback(req, 0, NULL, NULL);

	memset(desc.iv, 0, sizeof(desc.iv));
	desc.req = req;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	desc.fragno = 0;
	desc.fraglen = 0;

	sg_init_table(desc.frags, 4);

<<<<<<< HEAD
	return xdr_process_buf(buf, offset, buf->len - offset, decryptor, &desc);
=======
	ret = xdr_process_buf(buf, offset, buf->len - offset, decryptor, &desc);
	skcipher_request_zero(req);
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * This function makes the assumption that it was ultimately called
 * from gss_wrap().
 *
 * The client auth_gss code moves any existing tail data into a
 * separate page before calling gss_wrap.
 * The server svcauth_gss code ensures that both the head and the
 * tail have slack space of RPC_MAX_AUTH_SIZE before calling gss_wrap.
 *
 * Even with that guarantee, this function may be called more than
 * once in the processing of gss_wrap().  The best we can do is
 * verify at compile-time (see GSS_KRB5_SLACK_CHECK) that the
 * largest expected shift will fit within RPC_MAX_AUTH_SIZE.
 * At run-time we can verify that a single invocation of this
 * function doesn't attempt to use more the RPC_MAX_AUTH_SIZE.
 */

int
xdr_extend_head(struct xdr_buf *buf, unsigned int base, unsigned int shiftlen)
{
	u8 *p;

	if (shiftlen == 0)
		return 0;

<<<<<<< HEAD
	BUILD_BUG_ON(GSS_KRB5_MAX_SLACK_NEEDED > RPC_MAX_AUTH_SIZE);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	BUG_ON(shiftlen > RPC_MAX_AUTH_SIZE);

	p = buf->head[0].iov_base + base;

	memmove(p + shiftlen, p, buf->head[0].iov_len - base);

	buf->head[0].iov_len += shiftlen;
	buf->len += shiftlen;

	return 0;
}

static u32
<<<<<<< HEAD
gss_krb5_cts_crypt(struct crypto_blkcipher *cipher, struct xdr_buf *buf,
=======
gss_krb5_cts_crypt(struct crypto_sync_skcipher *cipher, struct xdr_buf *buf,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		   u32 offset, u8 *iv, struct page **pages, int encrypt)
{
	u32 ret;
	struct scatterlist sg[1];
<<<<<<< HEAD
	struct blkcipher_desc desc = { .tfm = cipher, .info = iv };
	u8 data[GSS_KRB5_MAX_BLOCKSIZE * 2];
	struct page **save_pages;
	u32 len = buf->len - offset;

	if (len > ARRAY_SIZE(data)) {
		WARN_ON(0);
		return -ENOMEM;
	}
=======
	SYNC_SKCIPHER_REQUEST_ON_STACK(req, cipher);
	u8 *data;
	struct page **save_pages;
	u32 len = buf->len - offset;

	if (len > GSS_KRB5_MAX_BLOCKSIZE * 2) {
		WARN_ON(0);
		return -ENOMEM;
	}
	data = kmalloc(GSS_KRB5_MAX_BLOCKSIZE * 2, GFP_KERNEL);
	if (!data)
		return -ENOMEM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * For encryption, we want to read from the cleartext
	 * page cache pages, and write the encrypted data to
	 * the supplied xdr_buf pages.
	 */
	save_pages = buf->pages;
	if (encrypt)
		buf->pages = pages;

	ret = read_bytes_from_xdr_buf(buf, offset, data, len);
	buf->pages = save_pages;
	if (ret)
		goto out;

	sg_init_one(sg, data, len);

<<<<<<< HEAD
	if (encrypt)
		ret = crypto_blkcipher_encrypt_iv(&desc, sg, sg, len);
	else
		ret = crypto_blkcipher_decrypt_iv(&desc, sg, sg, len);
=======
	skcipher_request_set_sync_tfm(req, cipher);
	skcipher_request_set_callback(req, 0, NULL, NULL);
	skcipher_request_set_crypt(req, sg, sg, len, iv);

	if (encrypt)
		ret = crypto_skcipher_encrypt(req);
	else
		ret = crypto_skcipher_decrypt(req);

	skcipher_request_zero(req);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (ret)
		goto out;

	ret = write_bytes_to_xdr_buf(buf, offset, data, len);

<<<<<<< HEAD
out:
	return ret;
}

u32
gss_krb5_aes_encrypt(struct krb5_ctx *kctx, u32 offset,
		     struct xdr_buf *buf, int ec, struct page **pages)
{
	u32 err;
	struct xdr_netobj hmac;
	u8 *cksumkey;
	u8 *ecptr;
	struct crypto_blkcipher *cipher, *aux_cipher;
	int blocksize;
	struct page **save_pages;
	int nblocks, nbytes;
	struct encryptor_desc desc;
	u32 cbcbytes;
	unsigned int usage;
=======
#if IS_ENABLED(CONFIG_KUNIT)
	/*
	 * CBC-CTS does not define an output IV but RFC 3962 defines it as the
	 * penultimate block of ciphertext, so copy that into the IV buffer
	 * before returning.
	 */
	if (encrypt)
		memcpy(iv, data, crypto_sync_skcipher_ivsize(cipher));
#endif

out:
	kfree(data);
	return ret;
}

/**
 * krb5_cbc_cts_encrypt - encrypt in CBC mode with CTS
 * @cts_tfm: CBC cipher with CTS
 * @cbc_tfm: base CBC cipher
 * @offset: starting byte offset for plaintext
 * @buf: OUT: output buffer
 * @pages: plaintext
 * @iv: output CBC initialization vector, or NULL
 * @ivsize: size of @iv, in octets
 *
 * To provide confidentiality, encrypt using cipher block chaining
 * with ciphertext stealing. Message integrity is handled separately.
 *
 * Return values:
 *   %0: encryption successful
 *   negative errno: encryption could not be completed
 */
VISIBLE_IF_KUNIT
int krb5_cbc_cts_encrypt(struct crypto_sync_skcipher *cts_tfm,
			 struct crypto_sync_skcipher *cbc_tfm,
			 u32 offset, struct xdr_buf *buf, struct page **pages,
			 u8 *iv, unsigned int ivsize)
{
	u32 blocksize, nbytes, nblocks, cbcbytes;
	struct encryptor_desc desc;
	int err;

	blocksize = crypto_sync_skcipher_blocksize(cts_tfm);
	nbytes = buf->len - offset;
	nblocks = (nbytes + blocksize - 1) / blocksize;
	cbcbytes = 0;
	if (nblocks > 2)
		cbcbytes = (nblocks - 2) * blocksize;

	memset(desc.iv, 0, sizeof(desc.iv));

	/* Handle block-sized chunks of plaintext with CBC. */
	if (cbcbytes) {
		SYNC_SKCIPHER_REQUEST_ON_STACK(req, cbc_tfm);

		desc.pos = offset;
		desc.fragno = 0;
		desc.fraglen = 0;
		desc.pages = pages;
		desc.outbuf = buf;
		desc.req = req;

		skcipher_request_set_sync_tfm(req, cbc_tfm);
		skcipher_request_set_callback(req, 0, NULL, NULL);

		sg_init_table(desc.infrags, 4);
		sg_init_table(desc.outfrags, 4);

		err = xdr_process_buf(buf, offset, cbcbytes, encryptor, &desc);
		skcipher_request_zero(req);
		if (err)
			return err;
	}

	/* Remaining plaintext is handled with CBC-CTS. */
	err = gss_krb5_cts_crypt(cts_tfm, buf, offset + cbcbytes,
				 desc.iv, pages, 1);
	if (err)
		return err;

	if (unlikely(iv))
		memcpy(iv, desc.iv, ivsize);
	return 0;
}
EXPORT_SYMBOL_IF_KUNIT(krb5_cbc_cts_encrypt);

/**
 * krb5_cbc_cts_decrypt - decrypt in CBC mode with CTS
 * @cts_tfm: CBC cipher with CTS
 * @cbc_tfm: base CBC cipher
 * @offset: starting byte offset for plaintext
 * @buf: OUT: output buffer
 *
 * Return values:
 *   %0: decryption successful
 *   negative errno: decryption could not be completed
 */
VISIBLE_IF_KUNIT
int krb5_cbc_cts_decrypt(struct crypto_sync_skcipher *cts_tfm,
			 struct crypto_sync_skcipher *cbc_tfm,
			 u32 offset, struct xdr_buf *buf)
{
	u32 blocksize, nblocks, cbcbytes;
	struct decryptor_desc desc;
	int err;

	blocksize = crypto_sync_skcipher_blocksize(cts_tfm);
	nblocks = (buf->len + blocksize - 1) / blocksize;
	cbcbytes = 0;
	if (nblocks > 2)
		cbcbytes = (nblocks - 2) * blocksize;

	memset(desc.iv, 0, sizeof(desc.iv));

	/* Handle block-sized chunks of plaintext with CBC. */
	if (cbcbytes) {
		SYNC_SKCIPHER_REQUEST_ON_STACK(req, cbc_tfm);

		desc.fragno = 0;
		desc.fraglen = 0;
		desc.req = req;

		skcipher_request_set_sync_tfm(req, cbc_tfm);
		skcipher_request_set_callback(req, 0, NULL, NULL);

		sg_init_table(desc.frags, 4);

		err = xdr_process_buf(buf, 0, cbcbytes, decryptor, &desc);
		skcipher_request_zero(req);
		if (err)
			return err;
	}

	/* Remaining plaintext is handled with CBC-CTS. */
	return gss_krb5_cts_crypt(cts_tfm, buf, cbcbytes, desc.iv, NULL, 0);
}
EXPORT_SYMBOL_IF_KUNIT(krb5_cbc_cts_decrypt);

u32
gss_krb5_aes_encrypt(struct krb5_ctx *kctx, u32 offset,
		     struct xdr_buf *buf, struct page **pages)
{
	u32 err;
	struct xdr_netobj hmac;
	u8 *ecptr;
	struct crypto_sync_skcipher *cipher, *aux_cipher;
	struct crypto_ahash *ahash;
	struct page **save_pages;
	unsigned int conflen;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (kctx->initiate) {
		cipher = kctx->initiator_enc;
		aux_cipher = kctx->initiator_enc_aux;
<<<<<<< HEAD
		cksumkey = kctx->initiator_integ;
		usage = KG_USAGE_INITIATOR_SEAL;
	} else {
		cipher = kctx->acceptor_enc;
		aux_cipher = kctx->acceptor_enc_aux;
		cksumkey = kctx->acceptor_integ;
		usage = KG_USAGE_ACCEPTOR_SEAL;
	}
	blocksize = crypto_blkcipher_blocksize(cipher);

	/* hide the gss token header and insert the confounder */
	offset += GSS_KRB5_TOK_HDR_LEN;
	if (xdr_extend_head(buf, offset, kctx->gk5e->conflen))
		return GSS_S_FAILURE;
	gss_krb5_make_confounder(buf->head[0].iov_base + offset, kctx->gk5e->conflen);
=======
		ahash = kctx->initiator_integ;
	} else {
		cipher = kctx->acceptor_enc;
		aux_cipher = kctx->acceptor_enc_aux;
		ahash = kctx->acceptor_integ;
	}
	conflen = crypto_sync_skcipher_blocksize(cipher);

	/* hide the gss token header and insert the confounder */
	offset += GSS_KRB5_TOK_HDR_LEN;
	if (xdr_extend_head(buf, offset, conflen))
		return GSS_S_FAILURE;
	krb5_make_confounder(buf->head[0].iov_base + offset, conflen);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	offset -= GSS_KRB5_TOK_HDR_LEN;

	if (buf->tail[0].iov_base != NULL) {
		ecptr = buf->tail[0].iov_base + buf->tail[0].iov_len;
	} else {
		buf->tail[0].iov_base = buf->head[0].iov_base
							+ buf->head[0].iov_len;
		buf->tail[0].iov_len = 0;
		ecptr = buf->tail[0].iov_base;
	}

<<<<<<< HEAD
	memset(ecptr, 'X', ec);
	buf->tail[0].iov_len += ec;
	buf->len += ec;

	/* copy plaintext gss token header after filler (if any) */
	memcpy(ecptr + ec, buf->head[0].iov_base + offset,
						GSS_KRB5_TOK_HDR_LEN);
	buf->tail[0].iov_len += GSS_KRB5_TOK_HDR_LEN;
	buf->len += GSS_KRB5_TOK_HDR_LEN;

	/* Do the HMAC */
	hmac.len = GSS_KRB5_MAX_CKSUM_LEN;
=======
	/* copy plaintext gss token header after filler (if any) */
	memcpy(ecptr, buf->head[0].iov_base + offset, GSS_KRB5_TOK_HDR_LEN);
	buf->tail[0].iov_len += GSS_KRB5_TOK_HDR_LEN;
	buf->len += GSS_KRB5_TOK_HDR_LEN;

	hmac.len = kctx->gk5e->cksumlength;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	hmac.data = buf->tail[0].iov_base + buf->tail[0].iov_len;

	/*
	 * When we are called, pages points to the real page cache
	 * data -- which we can't go and encrypt!  buf->pages points
	 * to scratch pages which we are going to send off to the
	 * client/server.  Swap in the plaintext pages to calculate
	 * the hmac.
	 */
	save_pages = buf->pages;
	buf->pages = pages;

<<<<<<< HEAD
	err = make_checksum_v2(kctx, NULL, 0, buf,
			       offset + GSS_KRB5_TOK_HDR_LEN,
			       cksumkey, usage, &hmac);
=======
	err = gss_krb5_checksum(ahash, NULL, 0, buf,
				offset + GSS_KRB5_TOK_HDR_LEN, &hmac);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	buf->pages = save_pages;
	if (err)
		return GSS_S_FAILURE;

<<<<<<< HEAD
	nbytes = buf->len - offset - GSS_KRB5_TOK_HDR_LEN;
	nblocks = (nbytes + blocksize - 1) / blocksize;
	cbcbytes = 0;
	if (nblocks > 2)
		cbcbytes = (nblocks - 2) * blocksize;

	memset(desc.iv, 0, sizeof(desc.iv));

	if (cbcbytes) {
		desc.pos = offset + GSS_KRB5_TOK_HDR_LEN;
		desc.fragno = 0;
		desc.fraglen = 0;
		desc.pages = pages;
		desc.outbuf = buf;
		desc.desc.info = desc.iv;
		desc.desc.flags = 0;
		desc.desc.tfm = aux_cipher;

		sg_init_table(desc.infrags, 4);
		sg_init_table(desc.outfrags, 4);

		err = xdr_process_buf(buf, offset + GSS_KRB5_TOK_HDR_LEN,
				      cbcbytes, encryptor, &desc);
		if (err)
			goto out_err;
	}

	/* Make sure IV carries forward from any CBC results. */
	err = gss_krb5_cts_crypt(cipher, buf,
				 offset + GSS_KRB5_TOK_HDR_LEN + cbcbytes,
				 desc.iv, pages, 1);
	if (err) {
		err = GSS_S_FAILURE;
		goto out_err;
	}
=======
	err = krb5_cbc_cts_encrypt(cipher, aux_cipher,
				   offset + GSS_KRB5_TOK_HDR_LEN,
				   buf, pages, NULL, 0);
	if (err)
		return GSS_S_FAILURE;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Now update buf to account for HMAC */
	buf->tail[0].iov_len += kctx->gk5e->cksumlength;
	buf->len += kctx->gk5e->cksumlength;

<<<<<<< HEAD
out_err:
	if (err)
		err = GSS_S_FAILURE;
	return err;
}

u32
gss_krb5_aes_decrypt(struct krb5_ctx *kctx, u32 offset, struct xdr_buf *buf,
		     u32 *headskip, u32 *tailskip)
{
	struct xdr_buf subbuf;
	u32 ret = 0;
	u8 *cksum_key;
	struct crypto_blkcipher *cipher, *aux_cipher;
	struct xdr_netobj our_hmac_obj;
	u8 our_hmac[GSS_KRB5_MAX_CKSUM_LEN];
	u8 pkt_hmac[GSS_KRB5_MAX_CKSUM_LEN];
	int nblocks, blocksize, cbcbytes;
	struct decryptor_desc desc;
	unsigned int usage;
=======
	return GSS_S_COMPLETE;
}

u32
gss_krb5_aes_decrypt(struct krb5_ctx *kctx, u32 offset, u32 len,
		     struct xdr_buf *buf, u32 *headskip, u32 *tailskip)
{
	struct crypto_sync_skcipher *cipher, *aux_cipher;
	struct crypto_ahash *ahash;
	struct xdr_netobj our_hmac_obj;
	u8 our_hmac[GSS_KRB5_MAX_CKSUM_LEN];
	u8 pkt_hmac[GSS_KRB5_MAX_CKSUM_LEN];
	struct xdr_buf subbuf;
	u32 ret = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (kctx->initiate) {
		cipher = kctx->acceptor_enc;
		aux_cipher = kctx->acceptor_enc_aux;
<<<<<<< HEAD
		cksum_key = kctx->acceptor_integ;
		usage = KG_USAGE_ACCEPTOR_SEAL;
	} else {
		cipher = kctx->initiator_enc;
		aux_cipher = kctx->initiator_enc_aux;
		cksum_key = kctx->initiator_integ;
		usage = KG_USAGE_INITIATOR_SEAL;
	}
	blocksize = crypto_blkcipher_blocksize(cipher);


	/* create a segment skipping the header and leaving out the checksum */
	xdr_buf_subsegment(buf, &subbuf, offset + GSS_KRB5_TOK_HDR_LEN,
				    (buf->len - offset - GSS_KRB5_TOK_HDR_LEN -
				     kctx->gk5e->cksumlength));

	nblocks = (subbuf.len + blocksize - 1) / blocksize;

	cbcbytes = 0;
	if (nblocks > 2)
		cbcbytes = (nblocks - 2) * blocksize;

	memset(desc.iv, 0, sizeof(desc.iv));

	if (cbcbytes) {
		desc.fragno = 0;
		desc.fraglen = 0;
		desc.desc.info = desc.iv;
		desc.desc.flags = 0;
		desc.desc.tfm = aux_cipher;

		sg_init_table(desc.frags, 4);

		ret = xdr_process_buf(&subbuf, 0, cbcbytes, decryptor, &desc);
		if (ret)
			goto out_err;
	}

	/* Make sure IV carries forward from any CBC results. */
	ret = gss_krb5_cts_crypt(cipher, &subbuf, cbcbytes, desc.iv, NULL, 0);
	if (ret)
		goto out_err;


	/* Calculate our hmac over the plaintext data */
	our_hmac_obj.len = sizeof(our_hmac);
	our_hmac_obj.data = our_hmac;

	ret = make_checksum_v2(kctx, NULL, 0, &subbuf, 0,
			       cksum_key, usage, &our_hmac_obj);
=======
		ahash = kctx->acceptor_integ;
	} else {
		cipher = kctx->initiator_enc;
		aux_cipher = kctx->initiator_enc_aux;
		ahash = kctx->initiator_integ;
	}

	/* create a segment skipping the header and leaving out the checksum */
	xdr_buf_subsegment(buf, &subbuf, offset + GSS_KRB5_TOK_HDR_LEN,
				    (len - offset - GSS_KRB5_TOK_HDR_LEN -
				     kctx->gk5e->cksumlength));

	ret = krb5_cbc_cts_decrypt(cipher, aux_cipher, 0, &subbuf);
	if (ret)
		goto out_err;

	our_hmac_obj.len = kctx->gk5e->cksumlength;
	our_hmac_obj.data = our_hmac;
	ret = gss_krb5_checksum(ahash, NULL, 0, &subbuf, 0, &our_hmac_obj);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ret)
		goto out_err;

	/* Get the packet's hmac value */
<<<<<<< HEAD
	ret = read_bytes_from_xdr_buf(buf, buf->len - kctx->gk5e->cksumlength,
=======
	ret = read_bytes_from_xdr_buf(buf, len - kctx->gk5e->cksumlength,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				      pkt_hmac, kctx->gk5e->cksumlength);
	if (ret)
		goto out_err;

<<<<<<< HEAD
	if (memcmp(pkt_hmac, our_hmac, kctx->gk5e->cksumlength) != 0) {
		ret = GSS_S_BAD_SIG;
		goto out_err;
	}
	*headskip = kctx->gk5e->conflen;
=======
	if (crypto_memneq(pkt_hmac, our_hmac, kctx->gk5e->cksumlength) != 0) {
		ret = GSS_S_BAD_SIG;
		goto out_err;
	}
	*headskip = crypto_sync_skcipher_blocksize(cipher);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	*tailskip = kctx->gk5e->cksumlength;
out_err:
	if (ret && ret != GSS_S_BAD_SIG)
		ret = GSS_S_FAILURE;
	return ret;
}

<<<<<<< HEAD
/*
 * Compute Kseq given the initial session key and the checksum.
 * Set the key of the given cipher.
 */
int
krb5_rc4_setup_seq_key(struct krb5_ctx *kctx, struct crypto_blkcipher *cipher,
		       unsigned char *cksum)
{
	struct crypto_hash *hmac;
	struct hash_desc desc;
	struct scatterlist sg[1];
	u8 Kseq[GSS_KRB5_MAX_KEYLEN];
	u32 zeroconstant = 0;
	int err;

	dprintk("%s: entered\n", __func__);

	hmac = crypto_alloc_hash(kctx->gk5e->cksum_name, 0, CRYPTO_ALG_ASYNC);
	if (IS_ERR(hmac)) {
		dprintk("%s: error %ld, allocating hash '%s'\n",
			__func__, PTR_ERR(hmac), kctx->gk5e->cksum_name);
		return PTR_ERR(hmac);
	}

	desc.tfm = hmac;
	desc.flags = 0;

	err = crypto_hash_init(&desc);
	if (err)
		goto out_err;

	/* Compute intermediate Kseq from session key */
	err = crypto_hash_setkey(hmac, kctx->Ksess, kctx->gk5e->keylength);
	if (err)
		goto out_err;

	sg_init_table(sg, 1);
	sg_set_buf(sg, &zeroconstant, 4);

	err = crypto_hash_digest(&desc, sg, 4, Kseq);
	if (err)
		goto out_err;

	/* Compute final Kseq from the checksum and intermediate Kseq */
	err = crypto_hash_setkey(hmac, Kseq, kctx->gk5e->keylength);
	if (err)
		goto out_err;

	sg_set_buf(sg, cksum, 8);

	err = crypto_hash_digest(&desc, sg, 8, Kseq);
	if (err)
		goto out_err;

	err = crypto_blkcipher_setkey(cipher, Kseq, kctx->gk5e->keylength);
	if (err)
		goto out_err;

	err = 0;

out_err:
	crypto_free_hash(hmac);
	dprintk("%s: returning %d\n", __func__, err);
	return err;
}

/*
 * Compute Kcrypt given the initial session key and the plaintext seqnum.
 * Set the key of cipher kctx->enc.
 */
int
krb5_rc4_setup_enc_key(struct krb5_ctx *kctx, struct crypto_blkcipher *cipher,
		       s32 seqnum)
{
	struct crypto_hash *hmac;
	struct hash_desc desc;
	struct scatterlist sg[1];
	u8 Kcrypt[GSS_KRB5_MAX_KEYLEN];
	u8 zeroconstant[4] = {0};
	u8 seqnumarray[4];
	int err, i;

	dprintk("%s: entered, seqnum %u\n", __func__, seqnum);

	hmac = crypto_alloc_hash(kctx->gk5e->cksum_name, 0, CRYPTO_ALG_ASYNC);
	if (IS_ERR(hmac)) {
		dprintk("%s: error %ld, allocating hash '%s'\n",
			__func__, PTR_ERR(hmac), kctx->gk5e->cksum_name);
		return PTR_ERR(hmac);
	}

	desc.tfm = hmac;
	desc.flags = 0;

	err = crypto_hash_init(&desc);
	if (err)
		goto out_err;

	/* Compute intermediate Kcrypt from session key */
	for (i = 0; i < kctx->gk5e->keylength; i++)
		Kcrypt[i] = kctx->Ksess[i] ^ 0xf0;

	err = crypto_hash_setkey(hmac, Kcrypt, kctx->gk5e->keylength);
	if (err)
		goto out_err;

	sg_init_table(sg, 1);
	sg_set_buf(sg, zeroconstant, 4);

	err = crypto_hash_digest(&desc, sg, 4, Kcrypt);
	if (err)
		goto out_err;

	/* Compute final Kcrypt from the seqnum and intermediate Kcrypt */
	err = crypto_hash_setkey(hmac, Kcrypt, kctx->gk5e->keylength);
	if (err)
		goto out_err;

	seqnumarray[0] = (unsigned char) ((seqnum >> 24) & 0xff);
	seqnumarray[1] = (unsigned char) ((seqnum >> 16) & 0xff);
	seqnumarray[2] = (unsigned char) ((seqnum >> 8) & 0xff);
	seqnumarray[3] = (unsigned char) ((seqnum >> 0) & 0xff);

	sg_set_buf(sg, seqnumarray, 4);

	err = crypto_hash_digest(&desc, sg, 4, Kcrypt);
	if (err)
		goto out_err;

	err = crypto_blkcipher_setkey(cipher, Kcrypt, kctx->gk5e->keylength);
	if (err)
		goto out_err;

	err = 0;

out_err:
	crypto_free_hash(hmac);
	dprintk("%s: returning %d\n", __func__, err);
	return err;
}

=======
/**
 * krb5_etm_checksum - Compute a MAC for a GSS Wrap token
 * @cipher: an initialized cipher transform
 * @tfm: an initialized hash transform
 * @body: xdr_buf containing an RPC message (body.len is the message length)
 * @body_offset: byte offset into @body to start checksumming
 * @cksumout: OUT: a buffer to be filled in with the computed HMAC
 *
 * Usually expressed as H = HMAC(K, IV | ciphertext)[1..h] .
 *
 * Caller provides the truncation length of the output token (h) in
 * cksumout.len.
 *
 * Return values:
 *   %GSS_S_COMPLETE: Digest computed, @cksumout filled in
 *   %GSS_S_FAILURE: Call failed
 */
VISIBLE_IF_KUNIT
u32 krb5_etm_checksum(struct crypto_sync_skcipher *cipher,
		      struct crypto_ahash *tfm, const struct xdr_buf *body,
		      int body_offset, struct xdr_netobj *cksumout)
{
	unsigned int ivsize = crypto_sync_skcipher_ivsize(cipher);
	struct ahash_request *req;
	struct scatterlist sg[1];
	u8 *iv, *checksumdata;
	int err = -ENOMEM;

	checksumdata = kmalloc(crypto_ahash_digestsize(tfm), GFP_KERNEL);
	if (!checksumdata)
		return GSS_S_FAILURE;
	/* For RPCSEC, the "initial cipher state" is always all zeroes. */
	iv = kzalloc(ivsize, GFP_KERNEL);
	if (!iv)
		goto out_free_mem;

	req = ahash_request_alloc(tfm, GFP_KERNEL);
	if (!req)
		goto out_free_mem;
	ahash_request_set_callback(req, CRYPTO_TFM_REQ_MAY_SLEEP, NULL, NULL);
	err = crypto_ahash_init(req);
	if (err)
		goto out_free_ahash;

	sg_init_one(sg, iv, ivsize);
	ahash_request_set_crypt(req, sg, NULL, ivsize);
	err = crypto_ahash_update(req);
	if (err)
		goto out_free_ahash;
	err = xdr_process_buf(body, body_offset, body->len - body_offset,
			      checksummer, req);
	if (err)
		goto out_free_ahash;

	ahash_request_set_crypt(req, NULL, checksumdata, 0);
	err = crypto_ahash_final(req);
	if (err)
		goto out_free_ahash;
	memcpy(cksumout->data, checksumdata, cksumout->len);

out_free_ahash:
	ahash_request_free(req);
out_free_mem:
	kfree(iv);
	kfree_sensitive(checksumdata);
	return err ? GSS_S_FAILURE : GSS_S_COMPLETE;
}
EXPORT_SYMBOL_IF_KUNIT(krb5_etm_checksum);

/**
 * krb5_etm_encrypt - Encrypt using the RFC 8009 rules
 * @kctx: Kerberos context
 * @offset: starting offset of the payload, in bytes
 * @buf: OUT: send buffer to contain the encrypted payload
 * @pages: plaintext payload
 *
 * The main difference with aes_encrypt is that "The HMAC is
 * calculated over the cipher state concatenated with the AES
 * output, instead of being calculated over the confounder and
 * plaintext.  This allows the message receiver to verify the
 * integrity of the message before decrypting the message."
 *
 * RFC 8009 Section 5:
 *
 * encryption function: as follows, where E() is AES encryption in
 * CBC-CS3 mode, and h is the size of truncated HMAC (128 bits or
 * 192 bits as described above).
 *
 *    N = random value of length 128 bits (the AES block size)
 *    IV = cipher state
 *    C = E(Ke, N | plaintext, IV)
 *    H = HMAC(Ki, IV | C)
 *    ciphertext = C | H[1..h]
 *
 * This encryption formula provides AEAD EtM with key separation.
 *
 * Return values:
 *   %GSS_S_COMPLETE: Encryption successful
 *   %GSS_S_FAILURE: Encryption failed
 */
u32
krb5_etm_encrypt(struct krb5_ctx *kctx, u32 offset,
		 struct xdr_buf *buf, struct page **pages)
{
	struct crypto_sync_skcipher *cipher, *aux_cipher;
	struct crypto_ahash *ahash;
	struct xdr_netobj hmac;
	unsigned int conflen;
	u8 *ecptr;
	u32 err;

	if (kctx->initiate) {
		cipher = kctx->initiator_enc;
		aux_cipher = kctx->initiator_enc_aux;
		ahash = kctx->initiator_integ;
	} else {
		cipher = kctx->acceptor_enc;
		aux_cipher = kctx->acceptor_enc_aux;
		ahash = kctx->acceptor_integ;
	}
	conflen = crypto_sync_skcipher_blocksize(cipher);

	offset += GSS_KRB5_TOK_HDR_LEN;
	if (xdr_extend_head(buf, offset, conflen))
		return GSS_S_FAILURE;
	krb5_make_confounder(buf->head[0].iov_base + offset, conflen);
	offset -= GSS_KRB5_TOK_HDR_LEN;

	if (buf->tail[0].iov_base) {
		ecptr = buf->tail[0].iov_base + buf->tail[0].iov_len;
	} else {
		buf->tail[0].iov_base = buf->head[0].iov_base
							+ buf->head[0].iov_len;
		buf->tail[0].iov_len = 0;
		ecptr = buf->tail[0].iov_base;
	}

	memcpy(ecptr, buf->head[0].iov_base + offset, GSS_KRB5_TOK_HDR_LEN);
	buf->tail[0].iov_len += GSS_KRB5_TOK_HDR_LEN;
	buf->len += GSS_KRB5_TOK_HDR_LEN;

	err = krb5_cbc_cts_encrypt(cipher, aux_cipher,
				   offset + GSS_KRB5_TOK_HDR_LEN,
				   buf, pages, NULL, 0);
	if (err)
		return GSS_S_FAILURE;

	hmac.data = buf->tail[0].iov_base + buf->tail[0].iov_len;
	hmac.len = kctx->gk5e->cksumlength;
	err = krb5_etm_checksum(cipher, ahash,
				buf, offset + GSS_KRB5_TOK_HDR_LEN, &hmac);
	if (err)
		goto out_err;
	buf->tail[0].iov_len += kctx->gk5e->cksumlength;
	buf->len += kctx->gk5e->cksumlength;

	return GSS_S_COMPLETE;

out_err:
	return GSS_S_FAILURE;
}

/**
 * krb5_etm_decrypt - Decrypt using the RFC 8009 rules
 * @kctx: Kerberos context
 * @offset: starting offset of the ciphertext, in bytes
 * @len:
 * @buf:
 * @headskip: OUT: the enctype's confounder length, in octets
 * @tailskip: OUT: the enctype's HMAC length, in octets
 *
 * RFC 8009 Section 5:
 *
 * decryption function: as follows, where D() is AES decryption in
 * CBC-CS3 mode, and h is the size of truncated HMAC.
 *
 *    (C, H) = ciphertext
 *        (Note: H is the last h bits of the ciphertext.)
 *    IV = cipher state
 *    if H != HMAC(Ki, IV | C)[1..h]
 *        stop, report error
 *    (N, P) = D(Ke, C, IV)
 *
 * Return values:
 *   %GSS_S_COMPLETE: Decryption successful
 *   %GSS_S_BAD_SIG: computed HMAC != received HMAC
 *   %GSS_S_FAILURE: Decryption failed
 */
u32
krb5_etm_decrypt(struct krb5_ctx *kctx, u32 offset, u32 len,
		 struct xdr_buf *buf, u32 *headskip, u32 *tailskip)
{
	struct crypto_sync_skcipher *cipher, *aux_cipher;
	u8 our_hmac[GSS_KRB5_MAX_CKSUM_LEN];
	u8 pkt_hmac[GSS_KRB5_MAX_CKSUM_LEN];
	struct xdr_netobj our_hmac_obj;
	struct crypto_ahash *ahash;
	struct xdr_buf subbuf;
	u32 ret = 0;

	if (kctx->initiate) {
		cipher = kctx->acceptor_enc;
		aux_cipher = kctx->acceptor_enc_aux;
		ahash = kctx->acceptor_integ;
	} else {
		cipher = kctx->initiator_enc;
		aux_cipher = kctx->initiator_enc_aux;
		ahash = kctx->initiator_integ;
	}

	/* Extract the ciphertext into @subbuf. */
	xdr_buf_subsegment(buf, &subbuf, offset + GSS_KRB5_TOK_HDR_LEN,
			   (len - offset - GSS_KRB5_TOK_HDR_LEN -
			    kctx->gk5e->cksumlength));

	our_hmac_obj.data = our_hmac;
	our_hmac_obj.len = kctx->gk5e->cksumlength;
	ret = krb5_etm_checksum(cipher, ahash, &subbuf, 0, &our_hmac_obj);
	if (ret)
		goto out_err;
	ret = read_bytes_from_xdr_buf(buf, len - kctx->gk5e->cksumlength,
				      pkt_hmac, kctx->gk5e->cksumlength);
	if (ret)
		goto out_err;
	if (crypto_memneq(pkt_hmac, our_hmac, kctx->gk5e->cksumlength) != 0) {
		ret = GSS_S_BAD_SIG;
		goto out_err;
	}

	ret = krb5_cbc_cts_decrypt(cipher, aux_cipher, 0, &subbuf);
	if (ret) {
		ret = GSS_S_FAILURE;
		goto out_err;
	}

	*headskip = crypto_sync_skcipher_blocksize(cipher);
	*tailskip = kctx->gk5e->cksumlength;
	return GSS_S_COMPLETE;

out_err:
	if (ret != GSS_S_BAD_SIG)
		ret = GSS_S_FAILURE;
	return ret;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
