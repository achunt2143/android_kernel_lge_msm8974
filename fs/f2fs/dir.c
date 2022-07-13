<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * fs/f2fs/dir.c
 *
 * Copyright (c) 2012 Samsung Electronics Co., Ltd.
 *             http://www.samsung.com/
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/fs.h>
#include <linux/f2fs_fs.h>
=======
 */
#include <asm/unaligned.h>
#include <linux/fs.h>
#include <linux/f2fs_fs.h>
#include <linux/sched/signal.h>
#include <linux/unicode.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "f2fs.h"
#include "node.h"
#include "acl.h"
#include "xattr.h"
<<<<<<< HEAD

static unsigned long dir_blocks(struct inode *inode)
{
	return ((unsigned long long) (i_size_read(inode) + PAGE_CACHE_SIZE - 1))
							>> PAGE_CACHE_SHIFT;
=======
#include <trace/events/f2fs.h>

#if IS_ENABLED(CONFIG_UNICODE)
extern struct kmem_cache *f2fs_cf_name_slab;
#endif

static unsigned long dir_blocks(struct inode *inode)
{
	return ((unsigned long long) (i_size_read(inode) + PAGE_SIZE - 1))
							>> PAGE_SHIFT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static unsigned int dir_buckets(unsigned int level, int dir_level)
{
	if (level + dir_level < MAX_DIR_HASH_DEPTH / 2)
<<<<<<< HEAD
		return 1 << (level + dir_level);
=======
		return BIT(level + dir_level);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	else
		return MAX_DIR_BUCKETS;
}

static unsigned int bucket_blocks(unsigned int level)
{
	if (level < MAX_DIR_HASH_DEPTH / 2)
		return 2;
	else
		return 4;
}

<<<<<<< HEAD
unsigned char f2fs_filetype_table[F2FS_FT_MAX] = {
	[F2FS_FT_UNKNOWN]	= DT_UNKNOWN,
	[F2FS_FT_REG_FILE]	= DT_REG,
	[F2FS_FT_DIR]		= DT_DIR,
	[F2FS_FT_CHRDEV]	= DT_CHR,
	[F2FS_FT_BLKDEV]	= DT_BLK,
	[F2FS_FT_FIFO]		= DT_FIFO,
	[F2FS_FT_SOCK]		= DT_SOCK,
	[F2FS_FT_SYMLINK]	= DT_LNK,
};

#define S_SHIFT 12
static unsigned char f2fs_type_by_mode[S_IFMT >> S_SHIFT] = {
	[S_IFREG >> S_SHIFT]	= F2FS_FT_REG_FILE,
	[S_IFDIR >> S_SHIFT]	= F2FS_FT_DIR,
	[S_IFCHR >> S_SHIFT]	= F2FS_FT_CHRDEV,
	[S_IFBLK >> S_SHIFT]	= F2FS_FT_BLKDEV,
	[S_IFIFO >> S_SHIFT]	= F2FS_FT_FIFO,
	[S_IFSOCK >> S_SHIFT]	= F2FS_FT_SOCK,
	[S_IFLNK >> S_SHIFT]	= F2FS_FT_SYMLINK,
};

void set_de_type(struct f2fs_dir_entry *de, umode_t mode)
{
	de->file_type = f2fs_type_by_mode[(mode & S_IFMT) >> S_SHIFT];
=======
/* If @dir is casefolded, initialize @fname->cf_name from @fname->usr_fname. */
int f2fs_init_casefolded_name(const struct inode *dir,
			      struct f2fs_filename *fname)
{
#if IS_ENABLED(CONFIG_UNICODE)
	struct super_block *sb = dir->i_sb;

	if (IS_CASEFOLDED(dir) &&
	    !is_dot_dotdot(fname->usr_fname->name, fname->usr_fname->len)) {
		fname->cf_name.name = f2fs_kmem_cache_alloc(f2fs_cf_name_slab,
					GFP_NOFS, false, F2FS_SB(sb));
		if (!fname->cf_name.name)
			return -ENOMEM;
		fname->cf_name.len = utf8_casefold(sb->s_encoding,
						   fname->usr_fname,
						   fname->cf_name.name,
						   F2FS_NAME_LEN);
		if ((int)fname->cf_name.len <= 0) {
			kmem_cache_free(f2fs_cf_name_slab, fname->cf_name.name);
			fname->cf_name.name = NULL;
			if (sb_has_strict_encoding(sb))
				return -EINVAL;
			/* fall back to treating name as opaque byte sequence */
		}
	}
#endif
	return 0;
}

static int __f2fs_setup_filename(const struct inode *dir,
				 const struct fscrypt_name *crypt_name,
				 struct f2fs_filename *fname)
{
	int err;

	memset(fname, 0, sizeof(*fname));

	fname->usr_fname = crypt_name->usr_fname;
	fname->disk_name = crypt_name->disk_name;
#ifdef CONFIG_FS_ENCRYPTION
	fname->crypto_buf = crypt_name->crypto_buf;
#endif
	if (crypt_name->is_nokey_name) {
		/* hash was decoded from the no-key name */
		fname->hash = cpu_to_le32(crypt_name->hash);
	} else {
		err = f2fs_init_casefolded_name(dir, fname);
		if (err) {
			f2fs_free_filename(fname);
			return err;
		}
		f2fs_hash_filename(dir, fname);
	}
	return 0;
}

/*
 * Prepare to search for @iname in @dir.  This is similar to
 * fscrypt_setup_filename(), but this also handles computing the casefolded name
 * and the f2fs dirhash if needed, then packing all the information about this
 * filename up into a 'struct f2fs_filename'.
 */
int f2fs_setup_filename(struct inode *dir, const struct qstr *iname,
			int lookup, struct f2fs_filename *fname)
{
	struct fscrypt_name crypt_name;
	int err;

	err = fscrypt_setup_filename(dir, iname, lookup, &crypt_name);
	if (err)
		return err;

	return __f2fs_setup_filename(dir, &crypt_name, fname);
}

/*
 * Prepare to look up @dentry in @dir.  This is similar to
 * fscrypt_prepare_lookup(), but this also handles computing the casefolded name
 * and the f2fs dirhash if needed, then packing all the information about this
 * filename up into a 'struct f2fs_filename'.
 */
int f2fs_prepare_lookup(struct inode *dir, struct dentry *dentry,
			struct f2fs_filename *fname)
{
	struct fscrypt_name crypt_name;
	int err;

	err = fscrypt_prepare_lookup(dir, dentry, &crypt_name);
	if (err)
		return err;

	return __f2fs_setup_filename(dir, &crypt_name, fname);
}

void f2fs_free_filename(struct f2fs_filename *fname)
{
#ifdef CONFIG_FS_ENCRYPTION
	kfree(fname->crypto_buf.name);
	fname->crypto_buf.name = NULL;
#endif
#if IS_ENABLED(CONFIG_UNICODE)
	if (fname->cf_name.name) {
		kmem_cache_free(f2fs_cf_name_slab, fname->cf_name.name);
		fname->cf_name.name = NULL;
	}
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static unsigned long dir_block_index(unsigned int level,
				int dir_level, unsigned int idx)
{
	unsigned long i;
	unsigned long bidx = 0;

	for (i = 0; i < level; i++)
		bidx += dir_buckets(i, dir_level) * bucket_blocks(i);
	bidx += idx * bucket_blocks(level);
	return bidx;
}

<<<<<<< HEAD
static struct f2fs_dir_entry *find_in_block(struct page *dentry_page,
				struct fscrypt_name *fname,
				f2fs_hash_t namehash,
				int *max_slots,
				struct page **res_page)
{
	struct f2fs_dentry_block *dentry_blk;
	struct f2fs_dir_entry *de;
	struct f2fs_dentry_ptr d;

	dentry_blk = (struct f2fs_dentry_block *)kmap(dentry_page);

	make_dentry_ptr(NULL, &d, (void *)dentry_blk, 1);
	de = find_target_dentry(fname, namehash, max_slots, &d);
	if (de)
		*res_page = dentry_page;
	else
		kunmap(dentry_page);

	/*
	 * For the most part, it should be a bug when name_len is zero.
	 * We stop here for figuring out where the bugs has occurred.
	 */
	f2fs_bug_on(F2FS_P_SB(dentry_page), d.max < 0);
	return de;
}

struct f2fs_dir_entry *find_target_dentry(struct fscrypt_name *fname,
			f2fs_hash_t namehash, int *max_slots,
			struct f2fs_dentry_ptr *d)
=======
static struct f2fs_dir_entry *find_in_block(struct inode *dir,
				struct page *dentry_page,
				const struct f2fs_filename *fname,
				int *max_slots)
{
	struct f2fs_dentry_block *dentry_blk;
	struct f2fs_dentry_ptr d;

	dentry_blk = (struct f2fs_dentry_block *)page_address(dentry_page);

	make_dentry_ptr_block(dir, &d, dentry_blk);
	return f2fs_find_target_dentry(&d, fname, max_slots);
}

#if IS_ENABLED(CONFIG_UNICODE)
/*
 * Test whether a case-insensitive directory entry matches the filename
 * being searched for.
 *
 * Returns 1 for a match, 0 for no match, and -errno on an error.
 */
static int f2fs_match_ci_name(const struct inode *dir, const struct qstr *name,
			       const u8 *de_name, u32 de_name_len)
{
	const struct super_block *sb = dir->i_sb;
	const struct unicode_map *um = sb->s_encoding;
	struct fscrypt_str decrypted_name = FSTR_INIT(NULL, de_name_len);
	struct qstr entry = QSTR_INIT(de_name, de_name_len);
	int res;

	if (IS_ENCRYPTED(dir)) {
		const struct fscrypt_str encrypted_name =
			FSTR_INIT((u8 *)de_name, de_name_len);

		if (WARN_ON_ONCE(!fscrypt_has_encryption_key(dir)))
			return -EINVAL;

		decrypted_name.name = kmalloc(de_name_len, GFP_KERNEL);
		if (!decrypted_name.name)
			return -ENOMEM;
		res = fscrypt_fname_disk_to_usr(dir, 0, 0, &encrypted_name,
						&decrypted_name);
		if (res < 0)
			goto out;
		entry.name = decrypted_name.name;
		entry.len = decrypted_name.len;
	}

	res = utf8_strncasecmp_folded(um, name, &entry);
	/*
	 * In strict mode, ignore invalid names.  In non-strict mode,
	 * fall back to treating them as opaque byte sequences.
	 */
	if (res < 0 && !sb_has_strict_encoding(sb)) {
		res = name->len == entry.len &&
				memcmp(name->name, entry.name, name->len) == 0;
	} else {
		/* utf8_strncasecmp_folded returns 0 on match */
		res = (res == 0);
	}
out:
	kfree(decrypted_name.name);
	return res;
}
#endif /* CONFIG_UNICODE */

static inline int f2fs_match_name(const struct inode *dir,
				   const struct f2fs_filename *fname,
				   const u8 *de_name, u32 de_name_len)
{
	struct fscrypt_name f;

#if IS_ENABLED(CONFIG_UNICODE)
	if (fname->cf_name.name) {
		struct qstr cf = FSTR_TO_QSTR(&fname->cf_name);

		return f2fs_match_ci_name(dir, &cf, de_name, de_name_len);
	}
#endif
	f.usr_fname = fname->usr_fname;
	f.disk_name = fname->disk_name;
#ifdef CONFIG_FS_ENCRYPTION
	f.crypto_buf = fname->crypto_buf;
#endif
	return fscrypt_match_name(&f, de_name, de_name_len);
}

struct f2fs_dir_entry *f2fs_find_target_dentry(const struct f2fs_dentry_ptr *d,
			const struct f2fs_filename *fname, int *max_slots)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct f2fs_dir_entry *de;
	unsigned long bit_pos = 0;
	int max_len = 0;
<<<<<<< HEAD
	struct fscrypt_str de_name = FSTR_INIT(NULL, 0);
	struct fscrypt_str *name = &fname->disk_name;
=======
	int res = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (max_slots)
		*max_slots = 0;
	while (bit_pos < d->max) {
		if (!test_bit_le(bit_pos, d->bitmap)) {
			bit_pos++;
			max_len++;
			continue;
		}

		de = &d->dentry[bit_pos];

<<<<<<< HEAD
		/* encrypted case */
		de_name.name = d->filename[bit_pos];
		de_name.len = le16_to_cpu(de->name_len);

		/* show encrypted name */
		if (fname->hash) {
			if (de->hash_code == fname->hash)
				goto found;
		} else if (de_name.len == name->len &&
			de->hash_code == namehash &&
			!memcmp(de_name.name, name->name, name->len))
			goto found;
=======
		if (unlikely(!de->name_len)) {
			bit_pos++;
			continue;
		}

		if (de->hash_code == fname->hash) {
			res = f2fs_match_name(d->inode, fname,
					      d->filename[bit_pos],
					      le16_to_cpu(de->name_len));
			if (res < 0)
				return ERR_PTR(res);
			if (res)
				goto found;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (max_slots && max_len > *max_slots)
			*max_slots = max_len;
		max_len = 0;

<<<<<<< HEAD
		/* remain bug on condition */
		if (unlikely(!de->name_len))
			d->max = -1;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		bit_pos += GET_DENTRY_SLOTS(le16_to_cpu(de->name_len));
	}

	de = NULL;
found:
	if (max_slots && max_len > *max_slots)
		*max_slots = max_len;
	return de;
}

static struct f2fs_dir_entry *find_in_level(struct inode *dir,
					unsigned int level,
<<<<<<< HEAD
					struct fscrypt_name *fname,
					struct page **res_page)
{
	struct qstr name = FSTR_TO_QSTR(&fname->disk_name);
	int s = GET_DENTRY_SLOTS(name.len);
=======
					const struct f2fs_filename *fname,
					struct page **res_page)
{
	int s = GET_DENTRY_SLOTS(fname->disk_name.len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned int nbucket, nblock;
	unsigned int bidx, end_block;
	struct page *dentry_page;
	struct f2fs_dir_entry *de = NULL;
<<<<<<< HEAD
	bool room = false;
	int max_slots;
	f2fs_hash_t namehash;

	namehash = f2fs_dentry_hash(&name);
=======
	pgoff_t next_pgofs;
	bool room = false;
	int max_slots;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	nbucket = dir_buckets(level, F2FS_I(dir)->i_dir_level);
	nblock = bucket_blocks(level);

	bidx = dir_block_index(level, F2FS_I(dir)->i_dir_level,
<<<<<<< HEAD
					le32_to_cpu(namehash) % nbucket);
	end_block = bidx + nblock;

	for (; bidx < end_block; bidx++) {
		/* no need to allocate new dentry pages to all the indices */
		dentry_page = find_data_page(dir, bidx);
		if (IS_ERR(dentry_page)) {
			room = true;
			continue;
		}

		de = find_in_block(dentry_page, fname, namehash, &max_slots,
								res_page);
		if (de)
			break;
=======
			       le32_to_cpu(fname->hash) % nbucket);
	end_block = bidx + nblock;

	while (bidx < end_block) {
		/* no need to allocate new dentry pages to all the indices */
		dentry_page = f2fs_find_data_page(dir, bidx, &next_pgofs);
		if (IS_ERR(dentry_page)) {
			if (PTR_ERR(dentry_page) == -ENOENT) {
				room = true;
				bidx = next_pgofs;
				continue;
			} else {
				*res_page = dentry_page;
				break;
			}
		}

		de = find_in_block(dir, dentry_page, fname, &max_slots);
		if (IS_ERR(de)) {
			*res_page = ERR_CAST(de);
			de = NULL;
			break;
		} else if (de) {
			*res_page = dentry_page;
			break;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (max_slots >= s)
			room = true;
		f2fs_put_page(dentry_page, 0);
<<<<<<< HEAD
	}

	if (!de && room && F2FS_I(dir)->chash != namehash) {
		F2FS_I(dir)->chash = namehash;
=======

		bidx++;
	}

	if (!de && room && F2FS_I(dir)->chash != fname->hash) {
		F2FS_I(dir)->chash = fname->hash;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		F2FS_I(dir)->clevel = level;
	}

	return de;
}

<<<<<<< HEAD
=======
struct f2fs_dir_entry *__f2fs_find_entry(struct inode *dir,
					 const struct f2fs_filename *fname,
					 struct page **res_page)
{
	unsigned long npages = dir_blocks(dir);
	struct f2fs_dir_entry *de = NULL;
	unsigned int max_depth;
	unsigned int level;

	*res_page = NULL;

	if (f2fs_has_inline_dentry(dir)) {
		de = f2fs_find_in_inline_dir(dir, fname, res_page);
		goto out;
	}

	if (npages == 0)
		goto out;

	max_depth = F2FS_I(dir)->i_current_depth;
	if (unlikely(max_depth > MAX_DIR_HASH_DEPTH)) {
		f2fs_warn(F2FS_I_SB(dir), "Corrupted max_depth of %lu: %u",
			  dir->i_ino, max_depth);
		max_depth = MAX_DIR_HASH_DEPTH;
		f2fs_i_depth_write(dir, max_depth);
	}

	for (level = 0; level < max_depth; level++) {
		de = find_in_level(dir, level, fname, res_page);
		if (de || IS_ERR(*res_page))
			break;
	}
out:
	/* This is to increase the speed of f2fs_create */
	if (!de)
		F2FS_I(dir)->task = current;
	return de;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Find an entry in the specified directory with the wanted name.
 * It returns the page where the entry was found (as a parameter - res_page),
 * and the entry itself. Page is returned mapped and unlocked.
 * Entry is guaranteed to be valid.
 */
struct f2fs_dir_entry *f2fs_find_entry(struct inode *dir,
<<<<<<< HEAD
			struct qstr *child, struct page **res_page)
{
	unsigned long npages = dir_blocks(dir);
	struct f2fs_dir_entry *de = NULL;
	unsigned int max_depth;
	unsigned int level;
	struct fscrypt_name fname;
	int err;

	*res_page = NULL;

	err = fscrypt_setup_filename(dir, child, 1, &fname);
	if (err)
		return NULL;

	if (f2fs_has_inline_dentry(dir)) {
		de = find_in_inline_dir(dir, &fname, res_page);
		goto out;
	}

	if (npages == 0)
		goto out;

	max_depth = F2FS_I(dir)->i_current_depth;
	if (unlikely(max_depth > MAX_DIR_HASH_DEPTH)) {
		f2fs_msg(F2FS_I_SB(dir)->sb, KERN_WARNING,
				"Corrupted max_depth of %lu: %u",
				dir->i_ino, max_depth);
		max_depth = MAX_DIR_HASH_DEPTH;
		F2FS_I(dir)->i_current_depth = max_depth;
		mark_inode_dirty(dir);
	}

	for (level = 0; level < max_depth; level++) {
		de = find_in_level(dir, level, &fname, res_page);
		if (de)
			break;
	}
out:
	fscrypt_free_filename(&fname);
=======
			const struct qstr *child, struct page **res_page)
{
	struct f2fs_dir_entry *de = NULL;
	struct f2fs_filename fname;
	int err;

	err = f2fs_setup_filename(dir, child, 1, &fname);
	if (err) {
		if (err == -ENOENT)
			*res_page = NULL;
		else
			*res_page = ERR_PTR(err);
		return NULL;
	}

	de = __f2fs_find_entry(dir, &fname, res_page);

	f2fs_free_filename(&fname);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return de;
}

struct f2fs_dir_entry *f2fs_parent_dir(struct inode *dir, struct page **p)
{
<<<<<<< HEAD
	struct page *page;
	struct f2fs_dir_entry *de;
	struct f2fs_dentry_block *dentry_blk;

	if (f2fs_has_inline_dentry(dir))
		return f2fs_parent_inline_dir(dir, p);

	page = get_lock_data_page(dir, 0, false);
	if (IS_ERR(page))
		return NULL;

	dentry_blk = kmap(page);
	de = &dentry_blk->dentry[1];
	*p = page;
	unlock_page(page);
	return de;
}

ino_t f2fs_inode_by_name(struct inode *dir, struct qstr *qstr)
{
	ino_t res = 0;
	struct f2fs_dir_entry *de;
	struct page *page;

	de = f2fs_find_entry(dir, qstr, &page);
	if (de) {
		res = le32_to_cpu(de->ino);
		f2fs_dentry_kunmap(dir, page);
		f2fs_put_page(page, 0);
=======
	return f2fs_find_entry(dir, &dotdot_name, p);
}

ino_t f2fs_inode_by_name(struct inode *dir, const struct qstr *qstr,
							struct page **page)
{
	ino_t res = 0;
	struct f2fs_dir_entry *de;

	de = f2fs_find_entry(dir, qstr, page);
	if (de) {
		res = le32_to_cpu(de->ino);
		f2fs_put_page(*page, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return res;
}

void f2fs_set_link(struct inode *dir, struct f2fs_dir_entry *de,
		struct page *page, struct inode *inode)
{
	enum page_type type = f2fs_has_inline_dentry(dir) ? NODE : DATA;
<<<<<<< HEAD
	lock_page(page);
	f2fs_wait_on_page_writeback(page, type, true);
	de->ino = cpu_to_le32(inode->i_ino);
	set_de_type(de, inode->i_mode);
	f2fs_dentry_kunmap(dir, page);
	set_page_dirty(page);
	dir->i_mtime = dir->i_ctime = CURRENT_TIME;
	mark_inode_dirty(dir);

	f2fs_put_page(page, 1);
}

static void init_dent_inode(const struct qstr *name, struct page *ipage)
{
	struct f2fs_inode *ri;

	f2fs_wait_on_page_writeback(ipage, NODE, true);

	/* copy name info. to this inode page */
	ri = F2FS_INODE(ipage);
	ri->i_namelen = cpu_to_le32(name->len);
	memcpy(ri->i_name, name->name, name->len);
	set_page_dirty(ipage);
}

int update_dent_inode(struct inode *inode, struct inode *to,
					const struct qstr *name)
{
	struct page *page;

	if (file_enc_name(to))
		return 0;

	page = get_node_page(F2FS_I_SB(inode), inode->i_ino);
	if (IS_ERR(page))
		return PTR_ERR(page);

	init_dent_inode(name, page);
	f2fs_put_page(page, 1);

	return 0;
}

void do_make_empty_dir(struct inode *inode, struct inode *parent,
					struct f2fs_dentry_ptr *d)
{
	struct qstr dot = QSTR_INIT(".", 1);
	struct qstr dotdot = QSTR_INIT("..", 2);
=======

	lock_page(page);
	f2fs_wait_on_page_writeback(page, type, true, true);
	de->ino = cpu_to_le32(inode->i_ino);
	de->file_type = fs_umode_to_ftype(inode->i_mode);
	set_page_dirty(page);

	inode_set_mtime_to_ts(dir, inode_set_ctime_current(dir));
	f2fs_mark_inode_dirty_sync(dir, false);
	f2fs_put_page(page, 1);
}

static void init_dent_inode(struct inode *dir, struct inode *inode,
			    const struct f2fs_filename *fname,
			    struct page *ipage)
{
	struct f2fs_inode *ri;

	if (!fname) /* tmpfile case? */
		return;

	f2fs_wait_on_page_writeback(ipage, NODE, true, true);

	/* copy name info. to this inode page */
	ri = F2FS_INODE(ipage);
	ri->i_namelen = cpu_to_le32(fname->disk_name.len);
	memcpy(ri->i_name, fname->disk_name.name, fname->disk_name.len);
	if (IS_ENCRYPTED(dir)) {
		file_set_enc_name(inode);
		/*
		 * Roll-forward recovery doesn't have encryption keys available,
		 * so it can't compute the dirhash for encrypted+casefolded
		 * filenames.  Append it to i_name if possible.  Else, disable
		 * roll-forward recovery of the dentry (i.e., make fsync'ing the
		 * file force a checkpoint) by setting LOST_PINO.
		 */
		if (IS_CASEFOLDED(dir)) {
			if (fname->disk_name.len + sizeof(f2fs_hash_t) <=
			    F2FS_NAME_LEN)
				put_unaligned(fname->hash, (f2fs_hash_t *)
					&ri->i_name[fname->disk_name.len]);
			else
				file_lost_pino(inode);
		}
	}
	set_page_dirty(ipage);
}

void f2fs_do_make_empty_dir(struct inode *inode, struct inode *parent,
					struct f2fs_dentry_ptr *d)
{
	struct fscrypt_str dot = FSTR_INIT(".", 1);
	struct fscrypt_str dotdot = FSTR_INIT("..", 2);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* update dirent of "." */
	f2fs_update_dentry(inode->i_ino, inode->i_mode, d, &dot, 0, 0);

	/* update dirent of ".." */
	f2fs_update_dentry(parent->i_ino, parent->i_mode, d, &dotdot, 0, 1);
}

static int make_empty_dir(struct inode *inode,
		struct inode *parent, struct page *page)
{
	struct page *dentry_page;
	struct f2fs_dentry_block *dentry_blk;
	struct f2fs_dentry_ptr d;

	if (f2fs_has_inline_dentry(inode))
<<<<<<< HEAD
		return make_empty_inline_dir(inode, parent, page);

	dentry_page = get_new_data_page(inode, page, 0, true);
	if (IS_ERR(dentry_page))
		return PTR_ERR(dentry_page);

	dentry_blk = kmap_atomic(dentry_page);

	make_dentry_ptr(NULL, &d, (void *)dentry_blk, 1);
	do_make_empty_dir(inode, parent, &d);

	kunmap_atomic(dentry_blk);
=======
		return f2fs_make_empty_inline_dir(inode, parent, page);

	dentry_page = f2fs_get_new_data_page(inode, page, 0, true);
	if (IS_ERR(dentry_page))
		return PTR_ERR(dentry_page);

	dentry_blk = page_address(dentry_page);

	make_dentry_ptr_block(NULL, &d, dentry_blk);
	f2fs_do_make_empty_dir(inode, parent, &d);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	set_page_dirty(dentry_page);
	f2fs_put_page(dentry_page, 1);
	return 0;
}

<<<<<<< HEAD
struct page *init_inode_metadata(struct inode *inode, struct inode *dir,
			const struct qstr *name, struct page *dpage)
=======
struct page *f2fs_init_inode_metadata(struct inode *inode, struct inode *dir,
			const struct f2fs_filename *fname, struct page *dpage)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct page *page;
	int err;

<<<<<<< HEAD
	if (is_inode_flag_set(F2FS_I(inode), FI_NEW_INODE)) {
		page = new_inode_page(inode);
=======
	if (is_inode_flag_set(inode, FI_NEW_INODE)) {
		page = f2fs_new_inode_page(inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (IS_ERR(page))
			return page;

		if (S_ISDIR(inode->i_mode)) {
<<<<<<< HEAD
			err = make_empty_dir(inode, dir, page);
			if (err)
				goto error;
=======
			/* in order to handle error case */
			get_page(page);
			err = make_empty_dir(inode, dir, page);
			if (err) {
				lock_page(page);
				goto put_error;
			}
			put_page(page);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}

		err = f2fs_init_acl(inode, dir, page, dpage);
		if (err)
			goto put_error;

<<<<<<< HEAD
		err = f2fs_init_security(inode, dir, name, page);
		if (err)
			goto put_error;

		if (f2fs_encrypted_inode(dir) && f2fs_may_encrypt(inode)) {
			err = fscrypt_inherit_context(dir, inode, page, false);
=======
		err = f2fs_init_security(inode, dir,
					 fname ? fname->usr_fname : NULL, page);
		if (err)
			goto put_error;

		if (IS_ENCRYPTED(inode)) {
			err = fscrypt_set_context(inode, page);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (err)
				goto put_error;
		}
	} else {
<<<<<<< HEAD
		page = get_node_page(F2FS_I_SB(dir), inode->i_ino);
		if (IS_ERR(page))
			return page;

		set_cold_node(inode, page);
	}

	if (name)
		init_dent_inode(name, page);
=======
		page = f2fs_get_node_page(F2FS_I_SB(dir), inode->i_ino);
		if (IS_ERR(page))
			return page;
	}

	init_dent_inode(dir, inode, fname, page);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * This file should be checkpointed during fsync.
	 * We lost i_pino from now on.
	 */
<<<<<<< HEAD
	if (is_inode_flag_set(F2FS_I(inode), FI_INC_LINK)) {
		file_lost_pino(inode);
=======
	if (is_inode_flag_set(inode, FI_INC_LINK)) {
		if (!S_ISDIR(inode->i_mode))
			file_lost_pino(inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * If link the tmpfile to alias through linkat path,
		 * we should remove this inode from orphan list.
		 */
		if (inode->i_nlink == 0)
<<<<<<< HEAD
			remove_orphan_inode(F2FS_I_SB(dir), inode->i_ino);
		inc_nlink(inode);
=======
			f2fs_remove_orphan_inode(F2FS_I_SB(dir), inode->i_ino);
		f2fs_i_links_write(inode, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return page;

put_error:
<<<<<<< HEAD
	f2fs_put_page(page, 1);
error:
	/* once the failed inode becomes a bad inode, i_mode is S_IFREG */
	truncate_inode_pages(&inode->i_data, 0);
	truncate_blocks(inode, 0, false);
	remove_dirty_inode(inode);
	remove_inode_page(inode);
	return ERR_PTR(err);
}

void update_parent_metadata(struct inode *dir, struct inode *inode,
						unsigned int current_depth)
{
	if (inode && is_inode_flag_set(F2FS_I(inode), FI_NEW_INODE)) {
		if (S_ISDIR(inode->i_mode)) {
			inc_nlink(dir);
			set_inode_flag(F2FS_I(dir), FI_UPDATE_DIR);
		}
		clear_inode_flag(F2FS_I(inode), FI_NEW_INODE);
	}
	dir->i_mtime = dir->i_ctime = CURRENT_TIME;
	mark_inode_dirty(dir);

	if (F2FS_I(dir)->i_current_depth != current_depth) {
		F2FS_I(dir)->i_current_depth = current_depth;
		set_inode_flag(F2FS_I(dir), FI_UPDATE_DIR);
	}

	if (inode && is_inode_flag_set(F2FS_I(inode), FI_INC_LINK))
		clear_inode_flag(F2FS_I(inode), FI_INC_LINK);
}

int room_for_filename(const void *bitmap, int slots, int max_slots)
=======
	clear_nlink(inode);
	f2fs_update_inode(inode, page);
	f2fs_put_page(page, 1);
	return ERR_PTR(err);
}

void f2fs_update_parent_metadata(struct inode *dir, struct inode *inode,
						unsigned int current_depth)
{
	if (inode && is_inode_flag_set(inode, FI_NEW_INODE)) {
		if (S_ISDIR(inode->i_mode))
			f2fs_i_links_write(dir, true);
		clear_inode_flag(inode, FI_NEW_INODE);
	}
	inode_set_mtime_to_ts(dir, inode_set_ctime_current(dir));
	f2fs_mark_inode_dirty_sync(dir, false);

	if (F2FS_I(dir)->i_current_depth != current_depth)
		f2fs_i_depth_write(dir, current_depth);

	if (inode && is_inode_flag_set(inode, FI_INC_LINK))
		clear_inode_flag(inode, FI_INC_LINK);
}

int f2fs_room_for_filename(const void *bitmap, int slots, int max_slots)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int bit_start = 0;
	int zero_start, zero_end;
next:
	zero_start = find_next_zero_bit_le(bitmap, max_slots, bit_start);
	if (zero_start >= max_slots)
		return max_slots;

	zero_end = find_next_bit_le(bitmap, max_slots, zero_start);
	if (zero_end - zero_start >= slots)
		return zero_start;

	bit_start = zero_end + 1;

	if (zero_end + 1 >= max_slots)
		return max_slots;
	goto next;
}

<<<<<<< HEAD
void f2fs_update_dentry(nid_t ino, umode_t mode, struct f2fs_dentry_ptr *d,
				const struct qstr *name, f2fs_hash_t name_hash,
				unsigned int bit_pos)
=======
bool f2fs_has_enough_room(struct inode *dir, struct page *ipage,
			  const struct f2fs_filename *fname)
{
	struct f2fs_dentry_ptr d;
	unsigned int bit_pos;
	int slots = GET_DENTRY_SLOTS(fname->disk_name.len);

	make_dentry_ptr_inline(dir, &d, inline_data_addr(dir, ipage));

	bit_pos = f2fs_room_for_filename(d.bitmap, slots, d.max);

	return bit_pos < d.max;
}

void f2fs_update_dentry(nid_t ino, umode_t mode, struct f2fs_dentry_ptr *d,
			const struct fscrypt_str *name, f2fs_hash_t name_hash,
			unsigned int bit_pos)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct f2fs_dir_entry *de;
	int slots = GET_DENTRY_SLOTS(name->len);
	int i;

	de = &d->dentry[bit_pos];
	de->hash_code = name_hash;
	de->name_len = cpu_to_le16(name->len);
	memcpy(d->filename[bit_pos], name->name, name->len);
	de->ino = cpu_to_le32(ino);
<<<<<<< HEAD
	set_de_type(de, mode);
	for (i = 0; i < slots; i++) {
		test_and_set_bit_le(bit_pos + i, (void *)d->bitmap);
=======
	de->file_type = fs_umode_to_ftype(mode);
	for (i = 0; i < slots; i++) {
		__set_bit_le(bit_pos + i, (void *)d->bitmap);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* avoid wrong garbage data for readdir */
		if (i)
			(de + i)->name_len = 0;
	}
}

<<<<<<< HEAD
/*
 * Caller should grab and release a rwsem by calling f2fs_lock_op() and
 * f2fs_unlock_op().
 */
int __f2fs_add_link(struct inode *dir, const struct qstr *name,
				struct inode *inode, nid_t ino, umode_t mode)
=======
int f2fs_add_regular_entry(struct inode *dir, const struct f2fs_filename *fname,
			   struct inode *inode, nid_t ino, umode_t mode)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned int bit_pos;
	unsigned int level;
	unsigned int current_depth;
	unsigned long bidx, block;
<<<<<<< HEAD
	f2fs_hash_t dentry_hash;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned int nbucket, nblock;
	struct page *dentry_page = NULL;
	struct f2fs_dentry_block *dentry_blk = NULL;
	struct f2fs_dentry_ptr d;
	struct page *page = NULL;
<<<<<<< HEAD
	struct fscrypt_name fname;
	struct qstr new_name;
	int slots, err;

	err = fscrypt_setup_filename(dir, name, 0, &fname);
	if (err)
		return err;

	new_name.name = fname_name(&fname);
	new_name.len = fname_len(&fname);

	if (f2fs_has_inline_dentry(dir)) {
		err = f2fs_add_inline_entry(dir, &new_name, inode, ino, mode);
		if (!err || err != -EAGAIN)
			goto out;
		else
			err = 0;
	}

	level = 0;
	slots = GET_DENTRY_SLOTS(new_name.len);
	dentry_hash = f2fs_dentry_hash(&new_name);

	current_depth = F2FS_I(dir)->i_current_depth;
	if (F2FS_I(dir)->chash == dentry_hash) {
=======
	int slots, err = 0;

	level = 0;
	slots = GET_DENTRY_SLOTS(fname->disk_name.len);

	current_depth = F2FS_I(dir)->i_current_depth;
	if (F2FS_I(dir)->chash == fname->hash) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		level = F2FS_I(dir)->clevel;
		F2FS_I(dir)->chash = 0;
	}

start:
<<<<<<< HEAD
	if (unlikely(current_depth == MAX_DIR_HASH_DEPTH)) {
		err = -ENOSPC;
		goto out;
	}
=======
	if (time_to_inject(F2FS_I_SB(dir), FAULT_DIR_DEPTH))
		return -ENOSPC;

	if (unlikely(current_depth == MAX_DIR_HASH_DEPTH))
		return -ENOSPC;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Increase the depth, if required */
	if (level == current_depth)
		++current_depth;

	nbucket = dir_buckets(level, F2FS_I(dir)->i_dir_level);
	nblock = bucket_blocks(level);

	bidx = dir_block_index(level, F2FS_I(dir)->i_dir_level,
<<<<<<< HEAD
				(le32_to_cpu(dentry_hash) % nbucket));

	for (block = bidx; block <= (bidx + nblock - 1); block++) {
		dentry_page = get_new_data_page(dir, NULL, block, true);
		if (IS_ERR(dentry_page)) {
			err = PTR_ERR(dentry_page);
			goto out;
		}

		dentry_blk = kmap(dentry_page);
		bit_pos = room_for_filename(&dentry_blk->dentry_bitmap,
=======
				(le32_to_cpu(fname->hash) % nbucket));

	for (block = bidx; block <= (bidx + nblock - 1); block++) {
		dentry_page = f2fs_get_new_data_page(dir, NULL, block, true);
		if (IS_ERR(dentry_page))
			return PTR_ERR(dentry_page);

		dentry_blk = page_address(dentry_page);
		bit_pos = f2fs_room_for_filename(&dentry_blk->dentry_bitmap,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
						slots, NR_DENTRY_IN_BLOCK);
		if (bit_pos < NR_DENTRY_IN_BLOCK)
			goto add_dentry;

<<<<<<< HEAD
		kunmap(dentry_page);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		f2fs_put_page(dentry_page, 1);
	}

	/* Move to next level to find the empty slot for new dentry */
	++level;
	goto start;
add_dentry:
<<<<<<< HEAD
	f2fs_wait_on_page_writeback(dentry_page, DATA, true);

	if (inode) {
		down_write(&F2FS_I(inode)->i_sem);
		page = init_inode_metadata(inode, dir, &new_name, NULL);
=======
	f2fs_wait_on_page_writeback(dentry_page, DATA, true, true);

	if (inode) {
		f2fs_down_write(&F2FS_I(inode)->i_sem);
		page = f2fs_init_inode_metadata(inode, dir, fname, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (IS_ERR(page)) {
			err = PTR_ERR(page);
			goto fail;
		}
<<<<<<< HEAD
		if (f2fs_encrypted_inode(dir))
			file_set_enc_name(inode);
	}

	make_dentry_ptr(NULL, &d, (void *)dentry_blk, 1);
	f2fs_update_dentry(ino, mode, &d, &new_name, dentry_hash, bit_pos);
=======
	}

	make_dentry_ptr_block(NULL, &d, dentry_blk);
	f2fs_update_dentry(ino, mode, &d, &fname->disk_name, fname->hash,
			   bit_pos);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	set_page_dirty(dentry_page);

	if (inode) {
<<<<<<< HEAD
		/* we don't need to mark_inode_dirty now */
		F2FS_I(inode)->i_pino = dir->i_ino;
		update_inode(inode, page);
		f2fs_put_page(page, 1);
	}

	update_parent_metadata(dir, inode, current_depth);
fail:
	if (inode)
		up_write(&F2FS_I(inode)->i_sem);

	if (is_inode_flag_set(F2FS_I(dir), FI_UPDATE_DIR)) {
		update_inode_page(dir);
		clear_inode_flag(F2FS_I(dir), FI_UPDATE_DIR);
	}
	kunmap(dentry_page);
	f2fs_put_page(dentry_page, 1);
out:
	fscrypt_free_filename(&fname);
=======
		f2fs_i_pino_write(inode, dir->i_ino);

		/* synchronize inode page's data from inode cache */
		if (is_inode_flag_set(inode, FI_NEW_INODE))
			f2fs_update_inode(inode, page);

		f2fs_put_page(page, 1);
	}

	f2fs_update_parent_metadata(dir, inode, current_depth);
fail:
	if (inode)
		f2fs_up_write(&F2FS_I(inode)->i_sem);

	f2fs_put_page(dentry_page, 1);

	return err;
}

int f2fs_add_dentry(struct inode *dir, const struct f2fs_filename *fname,
		    struct inode *inode, nid_t ino, umode_t mode)
{
	int err = -EAGAIN;

	if (f2fs_has_inline_dentry(dir)) {
		/*
		 * Should get i_xattr_sem to keep the lock order:
		 * i_xattr_sem -> inode_page lock used by f2fs_setxattr.
		 */
		f2fs_down_read(&F2FS_I(dir)->i_xattr_sem);
		err = f2fs_add_inline_entry(dir, fname, inode, ino, mode);
		f2fs_up_read(&F2FS_I(dir)->i_xattr_sem);
	}
	if (err == -EAGAIN)
		err = f2fs_add_regular_entry(dir, fname, inode, ino, mode);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	f2fs_update_time(F2FS_I_SB(dir), REQ_TIME);
	return err;
}

<<<<<<< HEAD
int f2fs_do_tmpfile(struct inode *inode, struct inode *dir)
=======
/*
 * Caller should grab and release a rwsem by calling f2fs_lock_op() and
 * f2fs_unlock_op().
 */
int f2fs_do_add_link(struct inode *dir, const struct qstr *name,
				struct inode *inode, nid_t ino, umode_t mode)
{
	struct f2fs_filename fname;
	struct page *page = NULL;
	struct f2fs_dir_entry *de = NULL;
	int err;

	err = f2fs_setup_filename(dir, name, 0, &fname);
	if (err)
		return err;

	/*
	 * An immature stackable filesystem shows a race condition between lookup
	 * and create. If we have same task when doing lookup and create, it's
	 * definitely fine as expected by VFS normally. Otherwise, let's just
	 * verify on-disk dentry one more time, which guarantees filesystem
	 * consistency more.
	 */
	if (current != F2FS_I(dir)->task) {
		de = __f2fs_find_entry(dir, &fname, &page);
		F2FS_I(dir)->task = NULL;
	}
	if (de) {
		f2fs_put_page(page, 0);
		err = -EEXIST;
	} else if (IS_ERR(page)) {
		err = PTR_ERR(page);
	} else {
		err = f2fs_add_dentry(dir, &fname, inode, ino, mode);
	}
	f2fs_free_filename(&fname);
	return err;
}

int f2fs_do_tmpfile(struct inode *inode, struct inode *dir,
					struct f2fs_filename *fname)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct page *page;
	int err = 0;

<<<<<<< HEAD
	down_write(&F2FS_I(inode)->i_sem);
	page = init_inode_metadata(inode, dir, NULL, NULL);
=======
	f2fs_down_write(&F2FS_I(inode)->i_sem);
	page = f2fs_init_inode_metadata(inode, dir, fname, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (IS_ERR(page)) {
		err = PTR_ERR(page);
		goto fail;
	}
<<<<<<< HEAD
	/* we don't need to mark_inode_dirty now */
	update_inode(inode, page);
	f2fs_put_page(page, 1);

	clear_inode_flag(F2FS_I(inode), FI_NEW_INODE);
fail:
	up_write(&F2FS_I(inode)->i_sem);
	f2fs_update_time(F2FS_I_SB(inode), REQ_TIME);
	return err;
}

void f2fs_drop_nlink(struct inode *dir, struct inode *inode, struct page *page)
{
	struct f2fs_sb_info *sbi = F2FS_I_SB(dir);

	down_write(&F2FS_I(inode)->i_sem);

	if (S_ISDIR(inode->i_mode)) {
		drop_nlink(dir);
		if (page)
			update_inode(dir, page);
		else
			update_inode_page(dir);
	}
	inode->i_ctime = CURRENT_TIME;

	drop_nlink(inode);
	if (S_ISDIR(inode->i_mode)) {
		drop_nlink(inode);
		i_size_write(inode, 0);
	}
	up_write(&F2FS_I(inode)->i_sem);
	update_inode_page(inode);

	if (inode->i_nlink == 0)
		add_orphan_inode(sbi, inode->i_ino);
	else
		release_orphan_inode(sbi);
=======
	f2fs_put_page(page, 1);

	clear_inode_flag(inode, FI_NEW_INODE);
	f2fs_update_time(F2FS_I_SB(inode), REQ_TIME);
fail:
	f2fs_up_write(&F2FS_I(inode)->i_sem);
	return err;
}

void f2fs_drop_nlink(struct inode *dir, struct inode *inode)
{
	struct f2fs_sb_info *sbi = F2FS_I_SB(dir);

	f2fs_down_write(&F2FS_I(inode)->i_sem);

	if (S_ISDIR(inode->i_mode))
		f2fs_i_links_write(dir, false);
	inode_set_ctime_current(inode);

	f2fs_i_links_write(inode, false);
	if (S_ISDIR(inode->i_mode)) {
		f2fs_i_links_write(inode, false);
		f2fs_i_size_write(inode, 0);
	}
	f2fs_up_write(&F2FS_I(inode)->i_sem);

	if (inode->i_nlink == 0)
		f2fs_add_orphan_inode(inode);
	else
		f2fs_release_orphan_inode(sbi);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * It only removes the dentry from the dentry page, corresponding name
 * entry in name page does not need to be touched during deletion.
 */
void f2fs_delete_entry(struct f2fs_dir_entry *dentry, struct page *page,
					struct inode *dir, struct inode *inode)
{
	struct	f2fs_dentry_block *dentry_blk;
	unsigned int bit_pos;
	int slots = GET_DENTRY_SLOTS(le16_to_cpu(dentry->name_len));
	int i;

	f2fs_update_time(F2FS_I_SB(dir), REQ_TIME);

<<<<<<< HEAD
=======
	if (F2FS_OPTION(F2FS_I_SB(dir)).fsync_mode == FSYNC_MODE_STRICT)
		f2fs_add_ino_entry(F2FS_I_SB(dir), dir->i_ino, TRANS_DIR_INO);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (f2fs_has_inline_dentry(dir))
		return f2fs_delete_inline_entry(dentry, page, dir, inode);

	lock_page(page);
<<<<<<< HEAD
	f2fs_wait_on_page_writeback(page, DATA, true);
=======
	f2fs_wait_on_page_writeback(page, DATA, true, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	dentry_blk = page_address(page);
	bit_pos = dentry - dentry_blk->dentry;
	for (i = 0; i < slots; i++)
<<<<<<< HEAD
		test_and_clear_bit_le(bit_pos + i, &dentry_blk->dentry_bitmap);
=======
		__clear_bit_le(bit_pos + i, &dentry_blk->dentry_bitmap);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Let's check and deallocate this dentry page */
	bit_pos = find_next_bit_le(&dentry_blk->dentry_bitmap,
			NR_DENTRY_IN_BLOCK,
			0);
<<<<<<< HEAD
	kunmap(page); /* kunmap - pair of f2fs_find_entry */
	set_page_dirty(page);

	dir->i_ctime = dir->i_mtime = CURRENT_TIME;

	if (inode)
		f2fs_drop_nlink(dir, inode, NULL);

	if (bit_pos == NR_DENTRY_IN_BLOCK &&
			!truncate_hole(dir, page->index, page->index + 1)) {
		clear_page_dirty_for_io(page);
		ClearPagePrivate(page);
		ClearPageUptodate(page);
		inode_dec_dirty_pages(dir);
	}
	f2fs_put_page(page, 1);
=======
	set_page_dirty(page);

	if (bit_pos == NR_DENTRY_IN_BLOCK &&
		!f2fs_truncate_hole(dir, page->index, page->index + 1)) {
		f2fs_clear_page_cache_dirty_tag(page);
		clear_page_dirty_for_io(page);
		ClearPageUptodate(page);
		clear_page_private_all(page);

		inode_dec_dirty_pages(dir);
		f2fs_remove_dirty_inode(dir);
	}
	f2fs_put_page(page, 1);

	inode_set_mtime_to_ts(dir, inode_set_ctime_current(dir));
	f2fs_mark_inode_dirty_sync(dir, false);

	if (inode)
		f2fs_drop_nlink(dir, inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

bool f2fs_empty_dir(struct inode *dir)
{
<<<<<<< HEAD
	unsigned long bidx;
=======
	unsigned long bidx = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct page *dentry_page;
	unsigned int bit_pos;
	struct f2fs_dentry_block *dentry_blk;
	unsigned long nblock = dir_blocks(dir);

	if (f2fs_has_inline_dentry(dir))
		return f2fs_empty_inline_dir(dir);

<<<<<<< HEAD
	for (bidx = 0; bidx < nblock; bidx++) {
		dentry_page = get_lock_data_page(dir, bidx, false);
		if (IS_ERR(dentry_page)) {
			if (PTR_ERR(dentry_page) == -ENOENT)
				continue;
			else
				return false;
		}

		dentry_blk = kmap_atomic(dentry_page);
=======
	while (bidx < nblock) {
		pgoff_t next_pgofs;

		dentry_page = f2fs_find_data_page(dir, bidx, &next_pgofs);
		if (IS_ERR(dentry_page)) {
			if (PTR_ERR(dentry_page) == -ENOENT) {
				bidx = next_pgofs;
				continue;
			} else {
				return false;
			}
		}

		dentry_blk = page_address(dentry_page);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (bidx == 0)
			bit_pos = 2;
		else
			bit_pos = 0;
		bit_pos = find_next_bit_le(&dentry_blk->dentry_bitmap,
						NR_DENTRY_IN_BLOCK,
						bit_pos);
<<<<<<< HEAD
		kunmap_atomic(dentry_blk);

		f2fs_put_page(dentry_page, 1);

		if (bit_pos < NR_DENTRY_IN_BLOCK)
			return false;
=======

		f2fs_put_page(dentry_page, 0);

		if (bit_pos < NR_DENTRY_IN_BLOCK)
			return false;

		bidx++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return true;
}

<<<<<<< HEAD
bool f2fs_fill_dentries(struct file *file, void *dirent, filldir_t filldir,
		struct f2fs_dentry_ptr *d, unsigned int n, unsigned int bit_pos,
		struct fscrypt_str *fstr)
{
	unsigned int start_bit_pos = bit_pos;
	unsigned char d_type;
	struct f2fs_dir_entry *de = NULL;
	struct fscrypt_str de_name = FSTR_INIT(NULL, 0);
	unsigned char *types = f2fs_filetype_table;
	int over;

	while (bit_pos < d->max) {
		d_type = DT_UNKNOWN;
=======
int f2fs_fill_dentries(struct dir_context *ctx, struct f2fs_dentry_ptr *d,
			unsigned int start_pos, struct fscrypt_str *fstr)
{
	unsigned char d_type = DT_UNKNOWN;
	unsigned int bit_pos;
	struct f2fs_dir_entry *de = NULL;
	struct fscrypt_str de_name = FSTR_INIT(NULL, 0);
	struct f2fs_sb_info *sbi = F2FS_I_SB(d->inode);
	struct blk_plug plug;
	bool readdir_ra = sbi->readdir_ra;
	bool found_valid_dirent = false;
	int err = 0;

	bit_pos = ((unsigned long)ctx->pos % d->max);

	if (readdir_ra)
		blk_start_plug(&plug);

	while (bit_pos < d->max) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		bit_pos = find_next_bit_le(d->bitmap, d->max, bit_pos);
		if (bit_pos >= d->max)
			break;

		de = &d->dentry[bit_pos];
<<<<<<< HEAD

		if (de->name_len == 0) {
			bit_pos++;
			continue;
		}

		if (types && de->file_type < F2FS_FT_MAX)
			d_type = types[de->file_type];
=======
		if (de->name_len == 0) {
			if (found_valid_dirent || !bit_pos) {
				f2fs_warn_ratelimited(sbi,
					"invalid namelen(0), ino:%u, run fsck to fix.",
					le32_to_cpu(de->ino));
				set_sbi_flag(sbi, SBI_NEED_FSCK);
			}
			bit_pos++;
			ctx->pos = start_pos + bit_pos;
			continue;
		}

		d_type = fs_ftype_to_dtype(de->file_type);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		de_name.name = d->filename[bit_pos];
		de_name.len = le16_to_cpu(de->name_len);

<<<<<<< HEAD
		if (f2fs_encrypted_inode(d->inode)) {
			int save_len = fstr->len;
			int ret;

			de_name.name = kmalloc(de_name.len, GFP_NOFS);
			if (!de_name.name)
				return false;

			memcpy(de_name.name, d->filename[bit_pos], de_name.len);

			ret = fscrypt_fname_disk_to_usr(d->inode,
						(u32)de->hash_code, 0,
						&de_name, fstr);
			kfree(de_name.name);
			if (ret < 0)
				return true;
=======
		/* check memory boundary before moving forward */
		bit_pos += GET_DENTRY_SLOTS(le16_to_cpu(de->name_len));
		if (unlikely(bit_pos > d->max ||
				le16_to_cpu(de->name_len) > F2FS_NAME_LEN)) {
			f2fs_warn(sbi, "%s: corrupted namelen=%d, run fsck to fix.",
				  __func__, le16_to_cpu(de->name_len));
			set_sbi_flag(sbi, SBI_NEED_FSCK);
			err = -EFSCORRUPTED;
			f2fs_handle_error(sbi, ERROR_CORRUPTED_DIRENT);
			goto out;
		}

		if (IS_ENCRYPTED(d->inode)) {
			int save_len = fstr->len;

			err = fscrypt_fname_disk_to_usr(d->inode,
						(u32)le32_to_cpu(de->hash_code),
						0, &de_name, fstr);
			if (err)
				goto out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

			de_name = *fstr;
			fstr->len = save_len;
		}

<<<<<<< HEAD
		over = filldir(dirent, de_name.name, de_name.len,
					(n * d->max) + bit_pos,
					le32_to_cpu(de->ino), d_type);
		if (over) {
			file->f_pos += bit_pos - start_bit_pos;
			return true;
		}

		bit_pos += GET_DENTRY_SLOTS(le16_to_cpu(de->name_len));
	}
	return false;
}

static int f2fs_readdir(struct file *file, void *dirent, filldir_t filldir)
{
	unsigned long pos = file->f_pos;
	unsigned int bit_pos = 0;
=======
		if (!dir_emit(ctx, de_name.name, de_name.len,
					le32_to_cpu(de->ino), d_type)) {
			err = 1;
			goto out;
		}

		if (readdir_ra)
			f2fs_ra_node_page(sbi, le32_to_cpu(de->ino));

		ctx->pos = start_pos + bit_pos;
		found_valid_dirent = true;
	}
out:
	if (readdir_ra)
		blk_finish_plug(&plug);
	return err;
}

static int f2fs_readdir(struct file *file, struct dir_context *ctx)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct inode *inode = file_inode(file);
	unsigned long npages = dir_blocks(inode);
	struct f2fs_dentry_block *dentry_blk = NULL;
	struct page *dentry_page = NULL;
	struct file_ra_state *ra = &file->f_ra;
<<<<<<< HEAD
	struct f2fs_dentry_ptr d;
	struct fscrypt_str fstr = FSTR_INIT(NULL, 0);
	unsigned int n = 0;
	int err = 0;

	if (f2fs_encrypted_inode(inode)) {
		err = fscrypt_get_encryption_info(inode);
		if (err && err != -ENOKEY)
			return err;

		err = fscrypt_fname_alloc_buffer(inode, F2FS_NAME_LEN, &fstr);
		if (err < 0)
			return err;
	}

	if (f2fs_has_inline_dentry(inode)) {
		err = f2fs_read_inline_dir(file, dirent, filldir, &fstr);
		goto out;
	}

	bit_pos = (pos % NR_DENTRY_IN_BLOCK);
	n = (pos / NR_DENTRY_IN_BLOCK);

	/* readahead for multi pages of dir */
	if (npages - n > 1 && !ra_has_index(ra, n))
		page_cache_sync_readahead(inode->i_mapping, ra, file, n,
				min(npages - n, (pgoff_t)MAX_DIR_RA_PAGES));

	for (; n < npages; n++) {
		dentry_page = get_lock_data_page(inode, n, false);
		if (IS_ERR(dentry_page)) {
			err = PTR_ERR(dentry_page);
			if (err == -ENOENT)
				continue;
			else
				goto out;
		}

		dentry_blk = kmap(dentry_page);

		make_dentry_ptr(inode, &d, (void *)dentry_blk, 1);

		if (f2fs_fill_dentries(file, dirent, filldir, &d, n,
							bit_pos, &fstr)) {
			kunmap(dentry_page);
			f2fs_put_page(dentry_page, 1);
			break;
		}

		bit_pos = 0;
		file->f_pos = (n + 1) * NR_DENTRY_IN_BLOCK;
		kunmap(dentry_page);
		f2fs_put_page(dentry_page, 1);
	}
out:
	fscrypt_fname_free_buffer(&fstr);
	return err;
}

static int f2fs_dir_open(struct inode *inode, struct file *filp)
{
	if (f2fs_encrypted_inode(inode))
		return fscrypt_get_encryption_info(inode) ? -EACCES : 0;
	return 0;
=======
	loff_t start_pos = ctx->pos;
	unsigned int n = ((unsigned long)ctx->pos / NR_DENTRY_IN_BLOCK);
	struct f2fs_dentry_ptr d;
	struct fscrypt_str fstr = FSTR_INIT(NULL, 0);
	int err = 0;

	if (IS_ENCRYPTED(inode)) {
		err = fscrypt_prepare_readdir(inode);
		if (err)
			goto out;

		err = fscrypt_fname_alloc_buffer(F2FS_NAME_LEN, &fstr);
		if (err < 0)
			goto out;
	}

	if (f2fs_has_inline_dentry(inode)) {
		err = f2fs_read_inline_dir(file, ctx, &fstr);
		goto out_free;
	}

	for (; n < npages; ctx->pos = n * NR_DENTRY_IN_BLOCK) {
		pgoff_t next_pgofs;

		/* allow readdir() to be interrupted */
		if (fatal_signal_pending(current)) {
			err = -ERESTARTSYS;
			goto out_free;
		}
		cond_resched();

		/* readahead for multi pages of dir */
		if (npages - n > 1 && !ra_has_index(ra, n))
			page_cache_sync_readahead(inode->i_mapping, ra, file, n,
				min(npages - n, (pgoff_t)MAX_DIR_RA_PAGES));

		dentry_page = f2fs_find_data_page(inode, n, &next_pgofs);
		if (IS_ERR(dentry_page)) {
			err = PTR_ERR(dentry_page);
			if (err == -ENOENT) {
				err = 0;
				n = next_pgofs;
				continue;
			} else {
				goto out_free;
			}
		}

		dentry_blk = page_address(dentry_page);

		make_dentry_ptr_block(inode, &d, dentry_blk);

		err = f2fs_fill_dentries(ctx, &d,
				n * NR_DENTRY_IN_BLOCK, &fstr);
		if (err) {
			f2fs_put_page(dentry_page, 0);
			break;
		}

		f2fs_put_page(dentry_page, 0);

		n++;
	}
out_free:
	fscrypt_fname_free_buffer(&fstr);
out:
	trace_f2fs_readdir(inode, start_pos, ctx->pos, err);
	return err < 0 ? err : 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

const struct file_operations f2fs_dir_operations = {
	.llseek		= generic_file_llseek,
	.read		= generic_read_dir,
<<<<<<< HEAD
	.readdir	= f2fs_readdir,
	.fsync		= f2fs_sync_file,
	.open		= f2fs_dir_open,
=======
	.iterate_shared	= f2fs_readdir,
	.fsync		= f2fs_sync_file,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.unlocked_ioctl	= f2fs_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl   = f2fs_compat_ioctl,
#endif
};
