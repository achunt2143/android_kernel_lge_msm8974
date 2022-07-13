<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * This contains encryption functions for per-file encryption.
 *
 * Copyright (C) 2015, Google, Inc.
 * Copyright (C) 2015, Motorola Mobility
 *
 * Written by Michael Halcrow, 2014.
 *
 * Filename encryption additions
 *	Uday Savagaonkar, 2014
 * Encryption policy handling additions
 *	Ildar Muslukhov, 2014
 * Add fscrypt_pullback_bio_page()
 *	Jaegeuk Kim, 2015.
 *
 * This has not yet undergone a rigorous security audit.
 *
 * The usage of AES-XTS should conform to recommendations in NIST
 * Special Publication 800-38E and IEEE P1619/D16.
 */

<<<<<<< HEAD
#include <linux/crypto.h>
#include <linux/ecryptfs.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/pagemap.h>
#include <linux/mempool.h>
#include <linux/module.h>
#include <linux/scatterlist.h>
#include <linux/ratelimit.h>
<<<<<<< HEAD
#include <linux/bio.h>
#include <linux/dcache.h>
#include <linux/namei.h>
#include <linux/fscrypto.h>

static unsigned int num_prealloc_crypto_pages = 32;
static unsigned int num_prealloc_crypto_ctxs = 128;
=======
#include <crypto/skcipher.h>
#include "fscrypt_private.h"

static unsigned int num_prealloc_crypto_pages = 32;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

module_param(num_prealloc_crypto_pages, uint, 0444);
MODULE_PARM_DESC(num_prealloc_crypto_pages,
		"Number of crypto pages to preallocate");
<<<<<<< HEAD
module_param(num_prealloc_crypto_ctxs, uint, 0444);
MODULE_PARM_DESC(num_prealloc_crypto_ctxs,
		"Number of crypto contexts to preallocate");

static mempool_t *fscrypt_bounce_page_pool = NULL;

static LIST_HEAD(fscrypt_free_ctxs);
static DEFINE_SPINLOCK(fscrypt_ctx_lock);

static struct workqueue_struct *fscrypt_read_workqueue;
static DEFINE_MUTEX(fscrypt_init_mutex);

static struct kmem_cache *fscrypt_ctx_cachep;
struct kmem_cache *fscrypt_info_cachep;

/**
 * fscrypt_release_ctx() - Releases an encryption context
 * @ctx: The encryption context to release.
 *
 * If the encryption context was allocated from the pre-allocated pool, returns
 * it to that pool. Else, frees it.
 *
 * If there's a bounce page in the context, this frees that.
 */
void fscrypt_release_ctx(struct fscrypt_ctx *ctx)
{
	unsigned long flags;

	if (ctx->flags & FS_WRITE_PATH_FL && ctx->w.bounce_page) {
		mempool_free(ctx->w.bounce_page, fscrypt_bounce_page_pool);
		ctx->w.bounce_page = NULL;
	}
	ctx->w.control_page = NULL;
	if (ctx->flags & FS_CTX_REQUIRES_FREE_ENCRYPT_FL) {
		kmem_cache_free(fscrypt_ctx_cachep, ctx);
	} else {
		spin_lock_irqsave(&fscrypt_ctx_lock, flags);
		list_add(&ctx->free_list, &fscrypt_free_ctxs);
		spin_unlock_irqrestore(&fscrypt_ctx_lock, flags);
	}
}
EXPORT_SYMBOL(fscrypt_release_ctx);

/**
 * fscrypt_get_ctx() - Gets an encryption context
 * @inode:       The inode for which we are doing the crypto
 * @gfp_flags:   The gfp flag for memory allocation
 *
 * Allocates and initializes an encryption context.
 *
 * Return: An allocated and initialized encryption context on success; error
 * value or NULL otherwise.
 */
struct fscrypt_ctx *fscrypt_get_ctx(struct inode *inode, gfp_t gfp_flags)
{
	struct fscrypt_ctx *ctx = NULL;
	struct fscrypt_info *ci = inode->i_crypt_info;
	unsigned long flags;

	if (ci == NULL)
		return ERR_PTR(-ENOKEY);

	/*
	 * We first try getting the ctx from a free list because in
	 * the common case the ctx will have an allocated and
	 * initialized crypto tfm, so it's probably a worthwhile
	 * optimization. For the bounce page, we first try getting it
	 * from the kernel allocator because that's just about as fast
	 * as getting it from a list and because a cache of free pages
	 * should generally be a "last resort" option for a filesystem
	 * to be able to do its job.
	 */
	spin_lock_irqsave(&fscrypt_ctx_lock, flags);
	ctx = list_first_entry_or_null(&fscrypt_free_ctxs,
					struct fscrypt_ctx, free_list);
	if (ctx)
		list_del(&ctx->free_list);
	spin_unlock_irqrestore(&fscrypt_ctx_lock, flags);
	if (!ctx) {
		ctx = kmem_cache_zalloc(fscrypt_ctx_cachep, gfp_flags);
		if (!ctx)
			return ERR_PTR(-ENOMEM);
		ctx->flags |= FS_CTX_REQUIRES_FREE_ENCRYPT_FL;
	} else {
		ctx->flags &= ~FS_CTX_REQUIRES_FREE_ENCRYPT_FL;
	}
	ctx->flags &= ~FS_WRITE_PATH_FL;
	return ctx;
}
EXPORT_SYMBOL(fscrypt_get_ctx);

/**
 * fscrypt_complete() - The completion callback for page encryption
 * @req: The asynchronous encryption request context
 * @res: The result of the encryption operation
 */
static void fscrypt_complete(struct crypto_async_request *req, int res)
{
	struct fscrypt_completion_result *ecr = req->data;

	if (res == -EINPROGRESS)
		return;
	ecr->res = res;
	complete(&ecr->completion);
}

typedef enum {
	FS_DECRYPT = 0,
	FS_ENCRYPT,
} fscrypt_direction_t;

static int do_page_crypto(struct inode *inode,
			fscrypt_direction_t rw, pgoff_t index,
			struct page *src_page, struct page *dest_page,
			gfp_t gfp_flags)
{
	u8 xts_tweak[FS_XTS_TWEAK_SIZE];
	struct ablkcipher_request *req = NULL;
	DECLARE_FS_COMPLETION_RESULT(ecr);
	struct scatterlist dst, src;
	struct fscrypt_info *ci = inode->i_crypt_info;
	struct crypto_ablkcipher *tfm = ci->ci_ctfm;
	int res = 0;

	req = ablkcipher_request_alloc(tfm, gfp_flags);
	if (!req) {
		printk_ratelimited(KERN_ERR
				"%s: crypto_request_alloc() failed\n",
				__func__);
		return -ENOMEM;
	}

	ablkcipher_request_set_callback(
		req, CRYPTO_TFM_REQ_MAY_BACKLOG | CRYPTO_TFM_REQ_MAY_SLEEP,
		fscrypt_complete, &ecr);

	BUILD_BUG_ON(FS_XTS_TWEAK_SIZE < sizeof(index));
	memcpy(xts_tweak, &index, sizeof(index));
	memset(&xts_tweak[sizeof(index)], 0,
			FS_XTS_TWEAK_SIZE - sizeof(index));

	sg_init_table(&dst, 1);
	sg_set_page(&dst, dest_page, PAGE_CACHE_SIZE, 0);
	sg_init_table(&src, 1);
	sg_set_page(&src, src_page, PAGE_CACHE_SIZE, 0);
	ablkcipher_request_set_crypt(req, &src, &dst, PAGE_CACHE_SIZE,
					xts_tweak);
	if (rw == FS_DECRYPT)
		res = crypto_ablkcipher_decrypt(req);
	else
		res = crypto_ablkcipher_encrypt(req);
	if (res == -EINPROGRESS || res == -EBUSY) {
		BUG_ON(req->base.data != &ecr);
		wait_for_completion(&ecr.completion);
		res = ecr.res;
	}
	ablkcipher_request_free(req);
	if (res) {
		printk_ratelimited(KERN_ERR
			"%s: crypto_ablkcipher_encrypt() returned %d\n",
			__func__, res);
=======

static mempool_t *fscrypt_bounce_page_pool = NULL;

static struct workqueue_struct *fscrypt_read_workqueue;
static DEFINE_MUTEX(fscrypt_init_mutex);

struct kmem_cache *fscrypt_inode_info_cachep;

void fscrypt_enqueue_decrypt_work(struct work_struct *work)
{
	queue_work(fscrypt_read_workqueue, work);
}
EXPORT_SYMBOL(fscrypt_enqueue_decrypt_work);

struct page *fscrypt_alloc_bounce_page(gfp_t gfp_flags)
{
	if (WARN_ON_ONCE(!fscrypt_bounce_page_pool)) {
		/*
		 * Oops, the filesystem called a function that uses the bounce
		 * page pool, but it didn't set needs_bounce_pages.
		 */
		return NULL;
	}
	return mempool_alloc(fscrypt_bounce_page_pool, gfp_flags);
}

/**
 * fscrypt_free_bounce_page() - free a ciphertext bounce page
 * @bounce_page: the bounce page to free, or NULL
 *
 * Free a bounce page that was allocated by fscrypt_encrypt_pagecache_blocks(),
 * or by fscrypt_alloc_bounce_page() directly.
 */
void fscrypt_free_bounce_page(struct page *bounce_page)
{
	if (!bounce_page)
		return;
	set_page_private(bounce_page, (unsigned long)NULL);
	ClearPagePrivate(bounce_page);
	mempool_free(bounce_page, fscrypt_bounce_page_pool);
}
EXPORT_SYMBOL(fscrypt_free_bounce_page);

/*
 * Generate the IV for the given data unit index within the given file.
 * For filenames encryption, index == 0.
 *
 * Keep this in sync with fscrypt_limit_io_blocks().  fscrypt_limit_io_blocks()
 * needs to know about any IV generation methods where the low bits of IV don't
 * simply contain the data unit index (e.g., IV_INO_LBLK_32).
 */
void fscrypt_generate_iv(union fscrypt_iv *iv, u64 index,
			 const struct fscrypt_inode_info *ci)
{
	u8 flags = fscrypt_policy_flags(&ci->ci_policy);

	memset(iv, 0, ci->ci_mode->ivsize);

	if (flags & FSCRYPT_POLICY_FLAG_IV_INO_LBLK_64) {
		WARN_ON_ONCE(index > U32_MAX);
		WARN_ON_ONCE(ci->ci_inode->i_ino > U32_MAX);
		index |= (u64)ci->ci_inode->i_ino << 32;
	} else if (flags & FSCRYPT_POLICY_FLAG_IV_INO_LBLK_32) {
		WARN_ON_ONCE(index > U32_MAX);
		index = (u32)(ci->ci_hashed_ino + index);
	} else if (flags & FSCRYPT_POLICY_FLAG_DIRECT_KEY) {
		memcpy(iv->nonce, ci->ci_nonce, FSCRYPT_FILE_NONCE_SIZE);
	}
	iv->index = cpu_to_le64(index);
}

/* Encrypt or decrypt a single "data unit" of file contents. */
int fscrypt_crypt_data_unit(const struct fscrypt_inode_info *ci,
			    fscrypt_direction_t rw, u64 index,
			    struct page *src_page, struct page *dest_page,
			    unsigned int len, unsigned int offs,
			    gfp_t gfp_flags)
{
	union fscrypt_iv iv;
	struct skcipher_request *req = NULL;
	DECLARE_CRYPTO_WAIT(wait);
	struct scatterlist dst, src;
	struct crypto_skcipher *tfm = ci->ci_enc_key.tfm;
	int res = 0;

	if (WARN_ON_ONCE(len <= 0))
		return -EINVAL;
	if (WARN_ON_ONCE(len % FSCRYPT_CONTENTS_ALIGNMENT != 0))
		return -EINVAL;

	fscrypt_generate_iv(&iv, index, ci);

	req = skcipher_request_alloc(tfm, gfp_flags);
	if (!req)
		return -ENOMEM;

	skcipher_request_set_callback(
		req, CRYPTO_TFM_REQ_MAY_BACKLOG | CRYPTO_TFM_REQ_MAY_SLEEP,
		crypto_req_done, &wait);

	sg_init_table(&dst, 1);
	sg_set_page(&dst, dest_page, len, offs);
	sg_init_table(&src, 1);
	sg_set_page(&src, src_page, len, offs);
	skcipher_request_set_crypt(req, &src, &dst, len, &iv);
	if (rw == FS_DECRYPT)
		res = crypto_wait_req(crypto_skcipher_decrypt(req), &wait);
	else
		res = crypto_wait_req(crypto_skcipher_encrypt(req), &wait);
	skcipher_request_free(req);
	if (res) {
		fscrypt_err(ci->ci_inode,
			    "%scryption failed for data unit %llu: %d",
			    (rw == FS_DECRYPT ? "De" : "En"), index, res);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return res;
	}
	return 0;
}

<<<<<<< HEAD
static struct page *alloc_bounce_page(struct fscrypt_ctx *ctx, gfp_t gfp_flags)
{
	ctx->w.bounce_page = mempool_alloc(fscrypt_bounce_page_pool, gfp_flags);
	if (ctx->w.bounce_page == NULL)
		return ERR_PTR(-ENOMEM);
	ctx->flags |= FS_WRITE_PATH_FL;
	return ctx->w.bounce_page;
}

/**
 * fscypt_encrypt_page() - Encrypts a page
 * @inode:          The inode for which the encryption should take place
 * @plaintext_page: The page to encrypt. Must be locked.
 * @gfp_flags:      The gfp flag for memory allocation
 *
 * Allocates a ciphertext page and encrypts plaintext_page into it using the ctx
 * encryption context.
 *
 * Called on the page write path.  The caller must call
 * fscrypt_restore_control_page() on the returned ciphertext page to
 * release the bounce buffer and the encryption context.
 *
 * Return: An allocated page with the encrypted content on success. Else, an
 * error value or NULL.
 */
struct page *fscrypt_encrypt_page(struct inode *inode,
				struct page *plaintext_page, gfp_t gfp_flags)
{
	struct fscrypt_ctx *ctx;
	struct page *ciphertext_page = NULL;
	int err;

	BUG_ON(!PageLocked(plaintext_page));

	ctx = fscrypt_get_ctx(inode, gfp_flags);
	if (IS_ERR(ctx))
		return (struct page *)ctx;

	/* The encryption operation will require a bounce page. */
	ciphertext_page = alloc_bounce_page(ctx, gfp_flags);
	if (IS_ERR(ciphertext_page))
		goto errout;

	ctx->w.control_page = plaintext_page;
	err = do_page_crypto(inode, FS_ENCRYPT, plaintext_page->index,
					plaintext_page, ciphertext_page,
					gfp_flags);
	if (err) {
		ciphertext_page = ERR_PTR(err);
		goto errout;
	}
	SetPagePrivate(ciphertext_page);
	set_page_private(ciphertext_page, (unsigned long)ctx);
	lock_page(ciphertext_page);
	return ciphertext_page;

errout:
	fscrypt_release_ctx(ctx);
	return ciphertext_page;
}
EXPORT_SYMBOL(fscrypt_encrypt_page);

/**
 * f2crypt_decrypt_page() - Decrypts a page in-place
 * @page: The page to decrypt. Must be locked.
 *
 * Decrypts page in-place using the ctx encryption context.
 *
 * Called from the read completion callback.
 *
 * Return: Zero on success, non-zero otherwise.
 */
int fscrypt_decrypt_page(struct page *page)
{
	BUG_ON(!PageLocked(page));

	return do_page_crypto(page->mapping->host,
			FS_DECRYPT, page->index, page, page, GFP_NOFS);
}
EXPORT_SYMBOL(fscrypt_decrypt_page);

int fscrypt_zeroout_range(struct inode *inode, pgoff_t lblk,
				sector_t pblk, unsigned int len)
{
	struct fscrypt_ctx *ctx;
	struct page *ciphertext_page = NULL;
	struct bio *bio;
	int ret, err = 0;

	BUG_ON(inode->i_sb->s_blocksize != PAGE_CACHE_SIZE);

	ctx = fscrypt_get_ctx(inode, GFP_NOFS);
	if (IS_ERR(ctx))
		return PTR_ERR(ctx);

	ciphertext_page = alloc_bounce_page(ctx, GFP_NOWAIT);
	if (IS_ERR(ciphertext_page)) {
		err = PTR_ERR(ciphertext_page);
		goto errout;
	}

	while (len--) {
		err = do_page_crypto(inode, FS_ENCRYPT, lblk,
					ZERO_PAGE(0), ciphertext_page,
					GFP_NOFS);
		if (err)
			goto errout;

		bio = bio_alloc(GFP_NOWAIT, 1);
		if (!bio) {
			err = -ENOMEM;
			goto errout;
		}
		bio->bi_bdev = inode->i_sb->s_bdev;
		bio->bi_sector =
			pblk << (inode->i_sb->s_blocksize_bits - 9);
		ret = bio_add_page(bio, ciphertext_page,
					inode->i_sb->s_blocksize, 0);
		if (ret != inode->i_sb->s_blocksize) {
			/* should never happen! */
			WARN_ON(1);
			bio_put(bio);
			err = -EIO;
			goto errout;
		}
		err = submit_bio_wait(WRITE, bio);
		bio_put(bio);
		if (err)
			goto errout;
		lblk++;
		pblk++;
	}
	err = 0;
errout:
	fscrypt_release_ctx(ctx);
	return err;
}
EXPORT_SYMBOL(fscrypt_zeroout_range);

/*
 * Validate dentries for encrypted directories to make sure we aren't
 * potentially caching stale data after a key has been added or
 * removed.
 */
static int fscrypt_d_revalidate(struct dentry *dentry, unsigned int flags)
{
	struct dentry *dir;
	int dir_has_key, cached_with_key;

	if (flags & LOOKUP_RCU)
		return -ECHILD;

	dir = dget_parent(dentry);
	if (!d_inode(dir)->i_sb->s_cop->is_encrypted(d_inode(dir))) {
		dput(dir);
		return 0;
	}

	/* this should eventually be an flag in d_flags */
	spin_lock(&dentry->d_lock);
	cached_with_key = dentry->d_flags & DCACHE_ENCRYPTED_WITH_KEY;
	spin_unlock(&dentry->d_lock);
	dir_has_key = (d_inode(dir)->i_crypt_info != NULL);
	dput(dir);

	/*
	 * If the dentry was cached without the key, and it is a
	 * negative dentry, it might be a valid name.  We can't check
	 * if the key has since been made available due to locking
	 * reasons, so we fail the validation so ext4_lookup() can do
	 * this check.
	 *
	 * We also fail the validation if the dentry was created with
	 * the key present, but we no longer have the key, or vice versa.
	 */
	if (!cached_with_key ||
			(!cached_with_key && dir_has_key) ||
			(cached_with_key && !dir_has_key))
		return 0;
	return 1;
}

const struct dentry_operations fscrypt_d_ops = {
	.d_revalidate = fscrypt_d_revalidate,
};
EXPORT_SYMBOL(fscrypt_d_ops);

/*
 * Call fscrypt_decrypt_page on every single page, reusing the encryption
 * context.
 */
static void completion_pages(struct work_struct *work)
{
	struct fscrypt_ctx *ctx =
		container_of(work, struct fscrypt_ctx, r.work);
	struct bio *bio = ctx->r.bio;
	struct bio_vec *bv;
	int i;

	bio_for_each_segment_all(bv, bio, i) {
		struct page *page = bv->bv_page;
		int ret = fscrypt_decrypt_page(page);

		if (ret) {
			WARN_ON_ONCE(1);
			SetPageError(page);
		} else {
			SetPageUptodate(page);
		}
		unlock_page(page);
	}
	fscrypt_release_ctx(ctx);
	bio_put(bio);
}

void fscrypt_decrypt_bio_pages(struct fscrypt_ctx *ctx, struct bio *bio)
{
	INIT_WORK(&ctx->r.work, completion_pages);
	ctx->r.bio = bio;
	queue_work(fscrypt_read_workqueue, &ctx->r.work);
}
EXPORT_SYMBOL(fscrypt_decrypt_bio_pages);

void fscrypt_pullback_bio_page(struct page **page, bool restore)
{
	struct fscrypt_ctx *ctx;
	struct page *bounce_page;

	/* The bounce data pages are unmapped. */
	if ((*page)->mapping)
		return;

	/* The bounce data page is unmapped. */
	bounce_page = *page;
	ctx = (struct fscrypt_ctx *)page_private(bounce_page);

	/* restore control page */
	*page = ctx->w.control_page;

	if (restore)
		fscrypt_restore_control_page(bounce_page);
}
EXPORT_SYMBOL(fscrypt_pullback_bio_page);

void fscrypt_restore_control_page(struct page *page)
{
	struct fscrypt_ctx *ctx;

	ctx = (struct fscrypt_ctx *)page_private(page);
	set_page_private(page, (unsigned long)NULL);
	ClearPagePrivate(page);
	unlock_page(page);
	fscrypt_release_ctx(ctx);
}
EXPORT_SYMBOL(fscrypt_restore_control_page);

static void fscrypt_destroy(void)
{
	struct fscrypt_ctx *pos, *n;

	list_for_each_entry_safe(pos, n, &fscrypt_free_ctxs, free_list)
		kmem_cache_free(fscrypt_ctx_cachep, pos);
	INIT_LIST_HEAD(&fscrypt_free_ctxs);
	mempool_destroy(fscrypt_bounce_page_pool);
	fscrypt_bounce_page_pool = NULL;
}

/**
 * fscrypt_initialize() - allocate major buffers for fs encryption.
=======
/**
 * fscrypt_encrypt_pagecache_blocks() - Encrypt data from a pagecache page
 * @page: the locked pagecache page containing the data to encrypt
 * @len: size of the data to encrypt, in bytes
 * @offs: offset within @page of the data to encrypt, in bytes
 * @gfp_flags: memory allocation flags; see details below
 *
 * This allocates a new bounce page and encrypts the given data into it.  The
 * length and offset of the data must be aligned to the file's crypto data unit
 * size.  Alignment to the filesystem block size fulfills this requirement, as
 * the filesystem block size is always a multiple of the data unit size.
 *
 * In the bounce page, the ciphertext data will be located at the same offset at
 * which the plaintext data was located in the source page.  Any other parts of
 * the bounce page will be left uninitialized.
 *
 * This is for use by the filesystem's ->writepages() method.
 *
 * The bounce page allocation is mempool-backed, so it will always succeed when
 * @gfp_flags includes __GFP_DIRECT_RECLAIM, e.g. when it's GFP_NOFS.  However,
 * only the first page of each bio can be allocated this way.  To prevent
 * deadlocks, for any additional pages a mask like GFP_NOWAIT must be used.
 *
 * Return: the new encrypted bounce page on success; an ERR_PTR() on failure
 */
struct page *fscrypt_encrypt_pagecache_blocks(struct page *page,
					      unsigned int len,
					      unsigned int offs,
					      gfp_t gfp_flags)

{
	const struct inode *inode = page->mapping->host;
	const struct fscrypt_inode_info *ci = inode->i_crypt_info;
	const unsigned int du_bits = ci->ci_data_unit_bits;
	const unsigned int du_size = 1U << du_bits;
	struct page *ciphertext_page;
	u64 index = ((u64)page->index << (PAGE_SHIFT - du_bits)) +
		    (offs >> du_bits);
	unsigned int i;
	int err;

	if (WARN_ON_ONCE(!PageLocked(page)))
		return ERR_PTR(-EINVAL);

	if (WARN_ON_ONCE(len <= 0 || !IS_ALIGNED(len | offs, du_size)))
		return ERR_PTR(-EINVAL);

	ciphertext_page = fscrypt_alloc_bounce_page(gfp_flags);
	if (!ciphertext_page)
		return ERR_PTR(-ENOMEM);

	for (i = offs; i < offs + len; i += du_size, index++) {
		err = fscrypt_crypt_data_unit(ci, FS_ENCRYPT, index,
					      page, ciphertext_page,
					      du_size, i, gfp_flags);
		if (err) {
			fscrypt_free_bounce_page(ciphertext_page);
			return ERR_PTR(err);
		}
	}
	SetPagePrivate(ciphertext_page);
	set_page_private(ciphertext_page, (unsigned long)page);
	return ciphertext_page;
}
EXPORT_SYMBOL(fscrypt_encrypt_pagecache_blocks);

/**
 * fscrypt_encrypt_block_inplace() - Encrypt a filesystem block in-place
 * @inode:     The inode to which this block belongs
 * @page:      The page containing the block to encrypt
 * @len:       Size of block to encrypt.  This must be a multiple of
 *		FSCRYPT_CONTENTS_ALIGNMENT.
 * @offs:      Byte offset within @page at which the block to encrypt begins
 * @lblk_num:  Filesystem logical block number of the block, i.e. the 0-based
 *		number of the block within the file
 * @gfp_flags: Memory allocation flags
 *
 * Encrypt a possibly-compressed filesystem block that is located in an
 * arbitrary page, not necessarily in the original pagecache page.  The @inode
 * and @lblk_num must be specified, as they can't be determined from @page.
 *
 * This is not compatible with fscrypt_operations::supports_subblock_data_units.
 *
 * Return: 0 on success; -errno on failure
 */
int fscrypt_encrypt_block_inplace(const struct inode *inode, struct page *page,
				  unsigned int len, unsigned int offs,
				  u64 lblk_num, gfp_t gfp_flags)
{
	if (WARN_ON_ONCE(inode->i_sb->s_cop->supports_subblock_data_units))
		return -EOPNOTSUPP;
	return fscrypt_crypt_data_unit(inode->i_crypt_info, FS_ENCRYPT,
				       lblk_num, page, page, len, offs,
				       gfp_flags);
}
EXPORT_SYMBOL(fscrypt_encrypt_block_inplace);

/**
 * fscrypt_decrypt_pagecache_blocks() - Decrypt data from a pagecache folio
 * @folio: the pagecache folio containing the data to decrypt
 * @len: size of the data to decrypt, in bytes
 * @offs: offset within @folio of the data to decrypt, in bytes
 *
 * Decrypt data that has just been read from an encrypted file.  The data must
 * be located in a pagecache folio that is still locked and not yet uptodate.
 * The length and offset of the data must be aligned to the file's crypto data
 * unit size.  Alignment to the filesystem block size fulfills this requirement,
 * as the filesystem block size is always a multiple of the data unit size.
 *
 * Return: 0 on success; -errno on failure
 */
int fscrypt_decrypt_pagecache_blocks(struct folio *folio, size_t len,
				     size_t offs)
{
	const struct inode *inode = folio->mapping->host;
	const struct fscrypt_inode_info *ci = inode->i_crypt_info;
	const unsigned int du_bits = ci->ci_data_unit_bits;
	const unsigned int du_size = 1U << du_bits;
	u64 index = ((u64)folio->index << (PAGE_SHIFT - du_bits)) +
		    (offs >> du_bits);
	size_t i;
	int err;

	if (WARN_ON_ONCE(!folio_test_locked(folio)))
		return -EINVAL;

	if (WARN_ON_ONCE(len <= 0 || !IS_ALIGNED(len | offs, du_size)))
		return -EINVAL;

	for (i = offs; i < offs + len; i += du_size, index++) {
		struct page *page = folio_page(folio, i >> PAGE_SHIFT);

		err = fscrypt_crypt_data_unit(ci, FS_DECRYPT, index, page,
					      page, du_size, i & ~PAGE_MASK,
					      GFP_NOFS);
		if (err)
			return err;
	}
	return 0;
}
EXPORT_SYMBOL(fscrypt_decrypt_pagecache_blocks);

/**
 * fscrypt_decrypt_block_inplace() - Decrypt a filesystem block in-place
 * @inode:     The inode to which this block belongs
 * @page:      The page containing the block to decrypt
 * @len:       Size of block to decrypt.  This must be a multiple of
 *		FSCRYPT_CONTENTS_ALIGNMENT.
 * @offs:      Byte offset within @page at which the block to decrypt begins
 * @lblk_num:  Filesystem logical block number of the block, i.e. the 0-based
 *		number of the block within the file
 *
 * Decrypt a possibly-compressed filesystem block that is located in an
 * arbitrary page, not necessarily in the original pagecache page.  The @inode
 * and @lblk_num must be specified, as they can't be determined from @page.
 *
 * This is not compatible with fscrypt_operations::supports_subblock_data_units.
 *
 * Return: 0 on success; -errno on failure
 */
int fscrypt_decrypt_block_inplace(const struct inode *inode, struct page *page,
				  unsigned int len, unsigned int offs,
				  u64 lblk_num)
{
	if (WARN_ON_ONCE(inode->i_sb->s_cop->supports_subblock_data_units))
		return -EOPNOTSUPP;
	return fscrypt_crypt_data_unit(inode->i_crypt_info, FS_DECRYPT,
				       lblk_num, page, page, len, offs,
				       GFP_NOFS);
}
EXPORT_SYMBOL(fscrypt_decrypt_block_inplace);

/**
 * fscrypt_initialize() - allocate major buffers for fs encryption.
 * @sb: the filesystem superblock
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * We only call this when we start accessing encrypted files, since it
 * results in memory getting allocated that wouldn't otherwise be used.
 *
<<<<<<< HEAD
 * Return: Zero on success, non-zero otherwise.
 */
int fscrypt_initialize(void)
{
	int i, res = -ENOMEM;

	if (fscrypt_bounce_page_pool)
=======
 * Return: 0 on success; -errno on failure
 */
int fscrypt_initialize(struct super_block *sb)
{
	int err = 0;
	mempool_t *pool;

	/* pairs with smp_store_release() below */
	if (likely(smp_load_acquire(&fscrypt_bounce_page_pool)))
		return 0;

	/* No need to allocate a bounce page pool if this FS won't use it. */
	if (!sb->s_cop->needs_bounce_pages)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;

	mutex_lock(&fscrypt_init_mutex);
	if (fscrypt_bounce_page_pool)
<<<<<<< HEAD
		goto already_initialized;

	for (i = 0; i < num_prealloc_crypto_ctxs; i++) {
		struct fscrypt_ctx *ctx;

		ctx = kmem_cache_zalloc(fscrypt_ctx_cachep, GFP_NOFS);
		if (!ctx)
			goto fail;
		list_add(&ctx->free_list, &fscrypt_free_ctxs);
	}

	fscrypt_bounce_page_pool =
		mempool_create_page_pool(num_prealloc_crypto_pages, 0);
	if (!fscrypt_bounce_page_pool)
		goto fail;

already_initialized:
	mutex_unlock(&fscrypt_init_mutex);
	return 0;
fail:
	fscrypt_destroy();
	mutex_unlock(&fscrypt_init_mutex);
	return res;
}
EXPORT_SYMBOL(fscrypt_initialize);

/**
 * fscrypt_init() - Set up for fs encryption.
 */
static int __init fscrypt_init(void)
{
	fscrypt_read_workqueue = alloc_workqueue("fscrypt_read_queue",
							WQ_HIGHPRI, 0);
	if (!fscrypt_read_workqueue)
		goto fail;

	fscrypt_ctx_cachep = KMEM_CACHE(fscrypt_ctx, SLAB_RECLAIM_ACCOUNT);
	if (!fscrypt_ctx_cachep)
		goto fail_free_queue;

	fscrypt_info_cachep = KMEM_CACHE(fscrypt_info, SLAB_RECLAIM_ACCOUNT);
	if (!fscrypt_info_cachep)
		goto fail_free_ctx;

	return 0;

fail_free_ctx:
	kmem_cache_destroy(fscrypt_ctx_cachep);
fail_free_queue:
	destroy_workqueue(fscrypt_read_workqueue);
fail:
	return -ENOMEM;
}
module_init(fscrypt_init)

/**
 * fscrypt_exit() - Shutdown the fs encryption system
 */
static void __exit fscrypt_exit(void)
{
	fscrypt_destroy();

	if (fscrypt_read_workqueue)
		destroy_workqueue(fscrypt_read_workqueue);
	kmem_cache_destroy(fscrypt_ctx_cachep);
	kmem_cache_destroy(fscrypt_info_cachep);
}
module_exit(fscrypt_exit);

MODULE_LICENSE("GPL");
=======
		goto out_unlock;

	err = -ENOMEM;
	pool = mempool_create_page_pool(num_prealloc_crypto_pages, 0);
	if (!pool)
		goto out_unlock;
	/* pairs with smp_load_acquire() above */
	smp_store_release(&fscrypt_bounce_page_pool, pool);
	err = 0;
out_unlock:
	mutex_unlock(&fscrypt_init_mutex);
	return err;
}

void fscrypt_msg(const struct inode *inode, const char *level,
		 const char *fmt, ...)
{
	static DEFINE_RATELIMIT_STATE(rs, DEFAULT_RATELIMIT_INTERVAL,
				      DEFAULT_RATELIMIT_BURST);
	struct va_format vaf;
	va_list args;

	if (!__ratelimit(&rs))
		return;

	va_start(args, fmt);
	vaf.fmt = fmt;
	vaf.va = &args;
	if (inode && inode->i_ino)
		printk("%sfscrypt (%s, inode %lu): %pV\n",
		       level, inode->i_sb->s_id, inode->i_ino, &vaf);
	else if (inode)
		printk("%sfscrypt (%s): %pV\n", level, inode->i_sb->s_id, &vaf);
	else
		printk("%sfscrypt: %pV\n", level, &vaf);
	va_end(args);
}

/**
 * fscrypt_init() - Set up for fs encryption.
 *
 * Return: 0 on success; -errno on failure
 */
static int __init fscrypt_init(void)
{
	int err = -ENOMEM;

	/*
	 * Use an unbound workqueue to allow bios to be decrypted in parallel
	 * even when they happen to complete on the same CPU.  This sacrifices
	 * locality, but it's worthwhile since decryption is CPU-intensive.
	 *
	 * Also use a high-priority workqueue to prioritize decryption work,
	 * which blocks reads from completing, over regular application tasks.
	 */
	fscrypt_read_workqueue = alloc_workqueue("fscrypt_read_queue",
						 WQ_UNBOUND | WQ_HIGHPRI,
						 num_online_cpus());
	if (!fscrypt_read_workqueue)
		goto fail;

	fscrypt_inode_info_cachep = KMEM_CACHE(fscrypt_inode_info,
					       SLAB_RECLAIM_ACCOUNT);
	if (!fscrypt_inode_info_cachep)
		goto fail_free_queue;

	err = fscrypt_init_keyring();
	if (err)
		goto fail_free_inode_info;

	return 0;

fail_free_inode_info:
	kmem_cache_destroy(fscrypt_inode_info_cachep);
fail_free_queue:
	destroy_workqueue(fscrypt_read_workqueue);
fail:
	return err;
}
late_initcall(fscrypt_init)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
