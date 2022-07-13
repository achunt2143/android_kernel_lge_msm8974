<<<<<<< HEAD
/*
 * dir.c - NILFS directory entry operations
 *
 * Copyright (C) 2005-2008 Nippon Telegraph and Telephone Corporation.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Modified for NILFS by Amagai Yoshiji <amagai@osrg.net>
=======
// SPDX-License-Identifier: GPL-2.0+
/*
 * NILFS directory entry operations
 *
 * Copyright (C) 2005-2008 Nippon Telegraph and Telephone Corporation.
 *
 * Modified for NILFS by Amagai Yoshiji.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
/*
 *  linux/fs/ext2/dir.c
 *
 * Copyright (C) 1992, 1993, 1994, 1995
 * Remy Card (card@masi.ibp.fr)
 * Laboratoire MASI - Institut Blaise Pascal
 * Universite Pierre et Marie Curie (Paris VI)
 *
 *  from
 *
 *  linux/fs/minix/dir.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 *
 *  ext2 directory handling functions
 *
 *  Big-endian to little-endian byte-swapping/bitmaps by
 *        David S. Miller (davem@caip.rutgers.edu), 1995
 *
 * All code that works with directory layout had been switched to pagecache
 * and moved here. AV
 */

#include <linux/pagemap.h>
#include "nilfs.h"
#include "page.h"

<<<<<<< HEAD
=======
static inline unsigned int nilfs_rec_len_from_disk(__le16 dlen)
{
	unsigned int len = le16_to_cpu(dlen);

#if (PAGE_SIZE >= 65536)
	if (len == NILFS_MAX_REC_LEN)
		return 1 << 16;
#endif
	return len;
}

static inline __le16 nilfs_rec_len_to_disk(unsigned int len)
{
#if (PAGE_SIZE >= 65536)
	if (len == (1 << 16))
		return cpu_to_le16(NILFS_MAX_REC_LEN);

	BUG_ON(len > (1 << 16));
#endif
	return cpu_to_le16(len);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * nilfs uses block-sized chunks. Arguably, sector-sized ones would be
 * more robust, but we have what we have
 */
<<<<<<< HEAD
static inline unsigned nilfs_chunk_size(struct inode *inode)
=======
static inline unsigned int nilfs_chunk_size(struct inode *inode)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return inode->i_sb->s_blocksize;
}

<<<<<<< HEAD
static inline void nilfs_put_page(struct page *page)
{
	kunmap(page);
	page_cache_release(page);
}

static inline unsigned long dir_pages(struct inode *inode)
{
	return (inode->i_size+PAGE_CACHE_SIZE-1)>>PAGE_CACHE_SHIFT;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Return the offset into page `page_nr' of the last valid
 * byte in that page, plus one.
 */
<<<<<<< HEAD
static unsigned nilfs_last_byte(struct inode *inode, unsigned long page_nr)
{
	unsigned last_byte = inode->i_size;

	last_byte -= page_nr << PAGE_CACHE_SHIFT;
	if (last_byte > PAGE_CACHE_SIZE)
		last_byte = PAGE_CACHE_SIZE;
	return last_byte;
}

static int nilfs_prepare_chunk(struct page *page, unsigned from, unsigned to)
{
	loff_t pos = page_offset(page) + from;
	return __block_write_begin(page, pos, to - from, nilfs_get_block);
}

static void nilfs_commit_chunk(struct page *page,
			       struct address_space *mapping,
			       unsigned from, unsigned to)
{
	struct inode *dir = mapping->host;
	loff_t pos = page_offset(page) + from;
	unsigned len = to - from;
	unsigned nr_dirty, copied;
	int err;

	nr_dirty = nilfs_page_count_clean_buffers(page, from, to);
	copied = block_write_end(NULL, mapping, pos, len, len, page, NULL);
=======
static unsigned int nilfs_last_byte(struct inode *inode, unsigned long page_nr)
{
	unsigned int last_byte = inode->i_size;

	last_byte -= page_nr << PAGE_SHIFT;
	if (last_byte > PAGE_SIZE)
		last_byte = PAGE_SIZE;
	return last_byte;
}

static int nilfs_prepare_chunk(struct folio *folio, unsigned int from,
			       unsigned int to)
{
	loff_t pos = folio_pos(folio) + from;

	return __block_write_begin(&folio->page, pos, to - from, nilfs_get_block);
}

static void nilfs_commit_chunk(struct folio *folio,
		struct address_space *mapping, size_t from, size_t to)
{
	struct inode *dir = mapping->host;
	loff_t pos = folio_pos(folio) + from;
	size_t copied, len = to - from;
	unsigned int nr_dirty;
	int err;

	nr_dirty = nilfs_page_count_clean_buffers(&folio->page, from, to);
	copied = block_write_end(NULL, mapping, pos, len, len, &folio->page, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (pos + copied > dir->i_size)
		i_size_write(dir, pos + copied);
	if (IS_DIRSYNC(dir))
		nilfs_set_transaction_flag(NILFS_TI_SYNC);
	err = nilfs_set_file_dirty(dir, nr_dirty);
	WARN_ON(err); /* do not happen */
<<<<<<< HEAD
	unlock_page(page);
}

static void nilfs_check_page(struct page *page)
{
	struct inode *dir = page->mapping->host;
	struct super_block *sb = dir->i_sb;
	unsigned chunk_size = nilfs_chunk_size(dir);
	char *kaddr = page_address(page);
	unsigned offs, rec_len;
	unsigned limit = PAGE_CACHE_SIZE;
	struct nilfs_dir_entry *p;
	char *error;

	if ((dir->i_size >> PAGE_CACHE_SHIFT) == page->index) {
		limit = dir->i_size & ~PAGE_CACHE_MASK;
=======
	folio_unlock(folio);
}

static bool nilfs_check_folio(struct folio *folio, char *kaddr)
{
	struct inode *dir = folio->mapping->host;
	struct super_block *sb = dir->i_sb;
	unsigned int chunk_size = nilfs_chunk_size(dir);
	size_t offs, rec_len;
	size_t limit = folio_size(folio);
	struct nilfs_dir_entry *p;
	char *error;

	if (dir->i_size < folio_pos(folio) + limit) {
		limit = dir->i_size - folio_pos(folio);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (limit & (chunk_size - 1))
			goto Ebadsize;
		if (!limit)
			goto out;
	}
	for (offs = 0; offs <= limit - NILFS_DIR_REC_LEN(1); offs += rec_len) {
		p = (struct nilfs_dir_entry *)(kaddr + offs);
		rec_len = nilfs_rec_len_from_disk(p->rec_len);

		if (rec_len < NILFS_DIR_REC_LEN(1))
			goto Eshort;
		if (rec_len & 3)
			goto Ealign;
		if (rec_len < NILFS_DIR_REC_LEN(p->name_len))
			goto Enamelen;
		if (((offs + rec_len - 1) ^ offs) & ~(chunk_size-1))
			goto Espan;
	}
	if (offs != limit)
		goto Eend;
out:
<<<<<<< HEAD
	SetPageChecked(page);
	return;
=======
	folio_set_checked(folio);
	return true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Too bad, we had an error */

Ebadsize:
<<<<<<< HEAD
	nilfs_error(sb, "nilfs_check_page",
		    "size of directory #%lu is not a multiple of chunk size",
		    dir->i_ino
	);
=======
	nilfs_error(sb,
		    "size of directory #%lu is not a multiple of chunk size",
		    dir->i_ino);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	goto fail;
Eshort:
	error = "rec_len is smaller than minimal";
	goto bad_entry;
Ealign:
	error = "unaligned directory entry";
	goto bad_entry;
Enamelen:
	error = "rec_len is too small for name_len";
	goto bad_entry;
Espan:
	error = "directory entry across blocks";
bad_entry:
<<<<<<< HEAD
	nilfs_error(sb, "nilfs_check_page", "bad entry in directory #%lu: %s - "
		    "offset=%lu, inode=%lu, rec_len=%d, name_len=%d",
		    dir->i_ino, error, (page->index<<PAGE_CACHE_SHIFT)+offs,
		    (unsigned long) le64_to_cpu(p->inode),
=======
	nilfs_error(sb,
		    "bad entry in directory #%lu: %s - offset=%lu, inode=%lu, rec_len=%zd, name_len=%d",
		    dir->i_ino, error, (folio->index << PAGE_SHIFT) + offs,
		    (unsigned long)le64_to_cpu(p->inode),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		    rec_len, p->name_len);
	goto fail;
Eend:
	p = (struct nilfs_dir_entry *)(kaddr + offs);
<<<<<<< HEAD
	nilfs_error(sb, "nilfs_check_page",
		    "entry in directory #%lu spans the page boundary"
		    "offset=%lu, inode=%lu",
		    dir->i_ino, (page->index<<PAGE_CACHE_SHIFT)+offs,
		    (unsigned long) le64_to_cpu(p->inode));
fail:
	SetPageChecked(page);
	SetPageError(page);
}

static struct page *nilfs_get_page(struct inode *dir, unsigned long n)
{
	struct address_space *mapping = dir->i_mapping;
	struct page *page = read_mapping_page(mapping, n, NULL);

	if (!IS_ERR(page)) {
		kmap(page);
		if (!PageChecked(page))
			nilfs_check_page(page);
		if (PageError(page))
			goto fail;
	}
	return page;

fail:
	nilfs_put_page(page);
=======
	nilfs_error(sb,
		    "entry in directory #%lu spans the page boundary offset=%lu, inode=%lu",
		    dir->i_ino, (folio->index << PAGE_SHIFT) + offs,
		    (unsigned long)le64_to_cpu(p->inode));
fail:
	folio_set_error(folio);
	return false;
}

static void *nilfs_get_folio(struct inode *dir, unsigned long n,
		struct folio **foliop)
{
	struct address_space *mapping = dir->i_mapping;
	struct folio *folio = read_mapping_folio(mapping, n, NULL);
	void *kaddr;

	if (IS_ERR(folio))
		return folio;

	kaddr = kmap_local_folio(folio, 0);
	if (unlikely(!folio_test_checked(folio))) {
		if (!nilfs_check_folio(folio, kaddr))
			goto fail;
	}

	*foliop = folio;
	return kaddr;

fail:
	folio_release_kmap(folio, kaddr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ERR_PTR(-EIO);
}

/*
 * NOTE! unlike strncmp, nilfs_match returns 1 for success, 0 for failure.
 *
 * len <= NILFS_NAME_LEN and de != NULL are guaranteed by caller.
 */
static int
nilfs_match(int len, const unsigned char *name, struct nilfs_dir_entry *de)
{
	if (len != de->name_len)
		return 0;
	if (!de->inode)
		return 0;
	return !memcmp(name, de->name, len);
}

/*
 * p is at least 6 bytes before the end of page
 */
static struct nilfs_dir_entry *nilfs_next_entry(struct nilfs_dir_entry *p)
{
	return (struct nilfs_dir_entry *)((char *)p +
					  nilfs_rec_len_from_disk(p->rec_len));
}

static unsigned char
nilfs_filetype_table[NILFS_FT_MAX] = {
	[NILFS_FT_UNKNOWN]	= DT_UNKNOWN,
	[NILFS_FT_REG_FILE]	= DT_REG,
	[NILFS_FT_DIR]		= DT_DIR,
	[NILFS_FT_CHRDEV]	= DT_CHR,
	[NILFS_FT_BLKDEV]	= DT_BLK,
	[NILFS_FT_FIFO]		= DT_FIFO,
	[NILFS_FT_SOCK]		= DT_SOCK,
	[NILFS_FT_SYMLINK]	= DT_LNK,
};

#define S_SHIFT 12
static unsigned char
<<<<<<< HEAD
nilfs_type_by_mode[S_IFMT >> S_SHIFT] = {
=======
nilfs_type_by_mode[(S_IFMT >> S_SHIFT) + 1] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	[S_IFREG >> S_SHIFT]	= NILFS_FT_REG_FILE,
	[S_IFDIR >> S_SHIFT]	= NILFS_FT_DIR,
	[S_IFCHR >> S_SHIFT]	= NILFS_FT_CHRDEV,
	[S_IFBLK >> S_SHIFT]	= NILFS_FT_BLKDEV,
	[S_IFIFO >> S_SHIFT]	= NILFS_FT_FIFO,
	[S_IFSOCK >> S_SHIFT]	= NILFS_FT_SOCK,
	[S_IFLNK >> S_SHIFT]	= NILFS_FT_SYMLINK,
};

static void nilfs_set_de_type(struct nilfs_dir_entry *de, struct inode *inode)
{
	umode_t mode = inode->i_mode;

	de->file_type = nilfs_type_by_mode[(mode & S_IFMT)>>S_SHIFT];
}

<<<<<<< HEAD
static int nilfs_readdir(struct file *filp, void *dirent, filldir_t filldir)
{
	loff_t pos = filp->f_pos;
	struct inode *inode = filp->f_dentry->d_inode;
	struct super_block *sb = inode->i_sb;
	unsigned int offset = pos & ~PAGE_CACHE_MASK;
	unsigned long n = pos >> PAGE_CACHE_SHIFT;
	unsigned long npages = dir_pages(inode);
/*	unsigned chunk_mask = ~(nilfs_chunk_size(inode)-1); */
	unsigned char *types = NULL;
	int ret;

	if (pos > inode->i_size - NILFS_DIR_REC_LEN(1))
		goto success;

	types = nilfs_filetype_table;
=======
static int nilfs_readdir(struct file *file, struct dir_context *ctx)
{
	loff_t pos = ctx->pos;
	struct inode *inode = file_inode(file);
	struct super_block *sb = inode->i_sb;
	unsigned int offset = pos & ~PAGE_MASK;
	unsigned long n = pos >> PAGE_SHIFT;
	unsigned long npages = dir_pages(inode);

	if (pos > inode->i_size - NILFS_DIR_REC_LEN(1))
		return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	for ( ; n < npages; n++, offset = 0) {
		char *kaddr, *limit;
		struct nilfs_dir_entry *de;
<<<<<<< HEAD
		struct page *page = nilfs_get_page(inode, n);

		if (IS_ERR(page)) {
			nilfs_error(sb, __func__, "bad page in #%lu",
				    inode->i_ino);
			filp->f_pos += PAGE_CACHE_SIZE - offset;
			ret = -EIO;
			goto done;
		}
		kaddr = page_address(page);
=======
		struct folio *folio;

		kaddr = nilfs_get_folio(inode, n, &folio);
		if (IS_ERR(kaddr)) {
			nilfs_error(sb, "bad page in #%lu", inode->i_ino);
			ctx->pos += PAGE_SIZE - offset;
			return -EIO;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		de = (struct nilfs_dir_entry *)(kaddr + offset);
		limit = kaddr + nilfs_last_byte(inode, n) -
			NILFS_DIR_REC_LEN(1);
		for ( ; (char *)de <= limit; de = nilfs_next_entry(de)) {
			if (de->rec_len == 0) {
<<<<<<< HEAD
				nilfs_error(sb, __func__,
					    "zero-length directory entry");
				ret = -EIO;
				nilfs_put_page(page);
				goto done;
			}
			if (de->inode) {
				int over;
				unsigned char d_type = DT_UNKNOWN;

				if (types && de->file_type < NILFS_FT_MAX)
					d_type = types[de->file_type];

				offset = (char *)de - kaddr;
				over = filldir(dirent, de->name, de->name_len,
						(n<<PAGE_CACHE_SHIFT) | offset,
						le64_to_cpu(de->inode), d_type);
				if (over) {
					nilfs_put_page(page);
					goto success;
				}
			}
			filp->f_pos += nilfs_rec_len_from_disk(de->rec_len);
		}
		nilfs_put_page(page);
	}

success:
	ret = 0;
done:
	return ret;
}

/*
 *	nilfs_find_entry()
 *
 * finds an entry in the specified directory with the wanted name. It
 * returns the page in which the entry was found, and the entry itself
 * (as a parameter - res_dir). Page is returned mapped and unlocked.
 * Entry is guaranteed to be valid.
 */
struct nilfs_dir_entry *
nilfs_find_entry(struct inode *dir, const struct qstr *qstr,
		 struct page **res_page)
{
	const unsigned char *name = qstr->name;
	int namelen = qstr->len;
	unsigned reclen = NILFS_DIR_REC_LEN(namelen);
	unsigned long start, n;
	unsigned long npages = dir_pages(dir);
	struct page *page = NULL;
=======
				nilfs_error(sb, "zero-length directory entry");
				folio_release_kmap(folio, kaddr);
				return -EIO;
			}
			if (de->inode) {
				unsigned char t;

				if (de->file_type < NILFS_FT_MAX)
					t = nilfs_filetype_table[de->file_type];
				else
					t = DT_UNKNOWN;

				if (!dir_emit(ctx, de->name, de->name_len,
						le64_to_cpu(de->inode), t)) {
					folio_release_kmap(folio, kaddr);
					return 0;
				}
			}
			ctx->pos += nilfs_rec_len_from_disk(de->rec_len);
		}
		folio_release_kmap(folio, kaddr);
	}
	return 0;
}

/*
 * nilfs_find_entry()
 *
 * Finds an entry in the specified directory with the wanted name. It
 * returns the folio in which the entry was found, and the entry itself.
 * The folio is mapped and unlocked.  When the caller is finished with
 * the entry, it should call folio_release_kmap().
 *
 * On failure, returns NULL and the caller should ignore foliop.
 */
struct nilfs_dir_entry *nilfs_find_entry(struct inode *dir,
		const struct qstr *qstr, struct folio **foliop)
{
	const unsigned char *name = qstr->name;
	int namelen = qstr->len;
	unsigned int reclen = NILFS_DIR_REC_LEN(namelen);
	unsigned long start, n;
	unsigned long npages = dir_pages(dir);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct nilfs_inode_info *ei = NILFS_I(dir);
	struct nilfs_dir_entry *de;

	if (npages == 0)
		goto out;

<<<<<<< HEAD
	/* OFFSET_CACHE */
	*res_page = NULL;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	start = ei->i_dir_start_lookup;
	if (start >= npages)
		start = 0;
	n = start;
	do {
<<<<<<< HEAD
		char *kaddr;
		page = nilfs_get_page(dir, n);
		if (!IS_ERR(page)) {
			kaddr = page_address(page);
=======
		char *kaddr = nilfs_get_folio(dir, n, foliop);

		if (!IS_ERR(kaddr)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			de = (struct nilfs_dir_entry *)kaddr;
			kaddr += nilfs_last_byte(dir, n) - reclen;
			while ((char *) de <= kaddr) {
				if (de->rec_len == 0) {
<<<<<<< HEAD
					nilfs_error(dir->i_sb, __func__,
						"zero-length directory entry");
					nilfs_put_page(page);
=======
					nilfs_error(dir->i_sb,
						"zero-length directory entry");
					folio_release_kmap(*foliop, kaddr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					goto out;
				}
				if (nilfs_match(namelen, name, de))
					goto found;
				de = nilfs_next_entry(de);
			}
<<<<<<< HEAD
			nilfs_put_page(page);
		}
		if (++n >= npages)
			n = 0;
		/* next page is past the blocks we've got */
		if (unlikely(n > (dir->i_blocks >> (PAGE_CACHE_SHIFT - 9)))) {
			nilfs_error(dir->i_sb, __func__,
=======
			folio_release_kmap(*foliop, kaddr);
		}
		if (++n >= npages)
			n = 0;
		/* next folio is past the blocks we've got */
		if (unlikely(n > (dir->i_blocks >> (PAGE_SHIFT - 9)))) {
			nilfs_error(dir->i_sb,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			       "dir %lu size %lld exceeds block count %llu",
			       dir->i_ino, dir->i_size,
			       (unsigned long long)dir->i_blocks);
			goto out;
		}
	} while (n != start);
out:
	return NULL;

found:
<<<<<<< HEAD
	*res_page = page;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ei->i_dir_start_lookup = n;
	return de;
}

<<<<<<< HEAD
struct nilfs_dir_entry *nilfs_dotdot(struct inode *dir, struct page **p)
{
	struct page *page = nilfs_get_page(dir, 0);
	struct nilfs_dir_entry *de = NULL;

	if (!IS_ERR(page)) {
		de = nilfs_next_entry(
			(struct nilfs_dir_entry *)page_address(page));
		*p = page;
	}
	return de;
=======
struct nilfs_dir_entry *nilfs_dotdot(struct inode *dir, struct folio **foliop)
{
	struct nilfs_dir_entry *de = nilfs_get_folio(dir, 0, foliop);

	if (IS_ERR(de))
		return NULL;
	return nilfs_next_entry(de);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

ino_t nilfs_inode_by_name(struct inode *dir, const struct qstr *qstr)
{
	ino_t res = 0;
	struct nilfs_dir_entry *de;
<<<<<<< HEAD
	struct page *page;

	de = nilfs_find_entry(dir, qstr, &page);
	if (de) {
		res = le64_to_cpu(de->inode);
		kunmap(page);
		page_cache_release(page);
=======
	struct folio *folio;

	de = nilfs_find_entry(dir, qstr, &folio);
	if (de) {
		res = le64_to_cpu(de->inode);
		folio_release_kmap(folio, de);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return res;
}

<<<<<<< HEAD
/* Releases the page */
void nilfs_set_link(struct inode *dir, struct nilfs_dir_entry *de,
		    struct page *page, struct inode *inode)
{
	unsigned from = (char *) de - (char *) page_address(page);
	unsigned to = from + nilfs_rec_len_from_disk(de->rec_len);
	struct address_space *mapping = page->mapping;
	int err;

	lock_page(page);
	err = nilfs_prepare_chunk(page, from, to);
	BUG_ON(err);
	de->inode = cpu_to_le64(inode->i_ino);
	nilfs_set_de_type(de, inode);
	nilfs_commit_chunk(page, mapping, from, to);
	nilfs_put_page(page);
	dir->i_mtime = dir->i_ctime = CURRENT_TIME;
=======
void nilfs_set_link(struct inode *dir, struct nilfs_dir_entry *de,
		    struct folio *folio, struct inode *inode)
{
	size_t from = offset_in_folio(folio, de);
	size_t to = from + nilfs_rec_len_from_disk(de->rec_len);
	struct address_space *mapping = folio->mapping;
	int err;

	folio_lock(folio);
	err = nilfs_prepare_chunk(folio, from, to);
	BUG_ON(err);
	de->inode = cpu_to_le64(inode->i_ino);
	nilfs_set_de_type(de, inode);
	nilfs_commit_chunk(folio, mapping, from, to);
	inode_set_mtime_to_ts(dir, inode_set_ctime_current(dir));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 *	Parent is locked.
 */
int nilfs_add_link(struct dentry *dentry, struct inode *inode)
{
<<<<<<< HEAD
	struct inode *dir = dentry->d_parent->d_inode;
	const unsigned char *name = dentry->d_name.name;
	int namelen = dentry->d_name.len;
	unsigned chunk_size = nilfs_chunk_size(dir);
	unsigned reclen = NILFS_DIR_REC_LEN(namelen);
	unsigned short rec_len, name_len;
	struct page *page = NULL;
	struct nilfs_dir_entry *de;
	unsigned long npages = dir_pages(dir);
	unsigned long n;
	char *kaddr;
	unsigned from, to;
=======
	struct inode *dir = d_inode(dentry->d_parent);
	const unsigned char *name = dentry->d_name.name;
	int namelen = dentry->d_name.len;
	unsigned int chunk_size = nilfs_chunk_size(dir);
	unsigned int reclen = NILFS_DIR_REC_LEN(namelen);
	unsigned short rec_len, name_len;
	struct folio *folio = NULL;
	struct nilfs_dir_entry *de;
	unsigned long npages = dir_pages(dir);
	unsigned long n;
	size_t from, to;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int err;

	/*
	 * We take care of directory expansion in the same loop.
<<<<<<< HEAD
	 * This code plays outside i_size, so it locks the page
	 * to protect that region.
	 */
	for (n = 0; n <= npages; n++) {
		char *dir_end;

		page = nilfs_get_page(dir, n);
		err = PTR_ERR(page);
		if (IS_ERR(page))
			goto out;
		lock_page(page);
		kaddr = page_address(page);
		dir_end = kaddr + nilfs_last_byte(dir, n);
		de = (struct nilfs_dir_entry *)kaddr;
		kaddr += PAGE_CACHE_SIZE - reclen;
=======
	 * This code plays outside i_size, so it locks the folio
	 * to protect that region.
	 */
	for (n = 0; n <= npages; n++) {
		char *kaddr = nilfs_get_folio(dir, n, &folio);
		char *dir_end;

		if (IS_ERR(kaddr))
			return PTR_ERR(kaddr);
		folio_lock(folio);
		dir_end = kaddr + nilfs_last_byte(dir, n);
		de = (struct nilfs_dir_entry *)kaddr;
		kaddr += folio_size(folio) - reclen;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		while ((char *)de <= kaddr) {
			if ((char *)de == dir_end) {
				/* We hit i_size */
				name_len = 0;
				rec_len = chunk_size;
				de->rec_len = nilfs_rec_len_to_disk(chunk_size);
				de->inode = 0;
				goto got_it;
			}
			if (de->rec_len == 0) {
<<<<<<< HEAD
				nilfs_error(dir->i_sb, __func__,
=======
				nilfs_error(dir->i_sb,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					    "zero-length directory entry");
				err = -EIO;
				goto out_unlock;
			}
			err = -EEXIST;
			if (nilfs_match(namelen, name, de))
				goto out_unlock;
			name_len = NILFS_DIR_REC_LEN(de->name_len);
			rec_len = nilfs_rec_len_from_disk(de->rec_len);
			if (!de->inode && rec_len >= reclen)
				goto got_it;
			if (rec_len >= name_len + reclen)
				goto got_it;
			de = (struct nilfs_dir_entry *)((char *)de + rec_len);
		}
<<<<<<< HEAD
		unlock_page(page);
		nilfs_put_page(page);
=======
		folio_unlock(folio);
		folio_release_kmap(folio, kaddr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	BUG();
	return -EINVAL;

got_it:
<<<<<<< HEAD
	from = (char *)de - (char *)page_address(page);
	to = from + rec_len;
	err = nilfs_prepare_chunk(page, from, to);
=======
	from = offset_in_folio(folio, de);
	to = from + rec_len;
	err = nilfs_prepare_chunk(folio, from, to);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err)
		goto out_unlock;
	if (de->inode) {
		struct nilfs_dir_entry *de1;

		de1 = (struct nilfs_dir_entry *)((char *)de + name_len);
		de1->rec_len = nilfs_rec_len_to_disk(rec_len - name_len);
		de->rec_len = nilfs_rec_len_to_disk(name_len);
		de = de1;
	}
	de->name_len = namelen;
	memcpy(de->name, name, namelen);
	de->inode = cpu_to_le64(inode->i_ino);
	nilfs_set_de_type(de, inode);
<<<<<<< HEAD
	nilfs_commit_chunk(page, page->mapping, from, to);
	dir->i_mtime = dir->i_ctime = CURRENT_TIME;
	nilfs_mark_inode_dirty(dir);
	/* OFFSET_CACHE */
out_put:
	nilfs_put_page(page);
out:
	return err;
out_unlock:
	unlock_page(page);
=======
	nilfs_commit_chunk(folio, folio->mapping, from, to);
	inode_set_mtime_to_ts(dir, inode_set_ctime_current(dir));
	nilfs_mark_inode_dirty(dir);
	/* OFFSET_CACHE */
out_put:
	folio_release_kmap(folio, de);
	return err;
out_unlock:
	folio_unlock(folio);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	goto out_put;
}

/*
 * nilfs_delete_entry deletes a directory entry by merging it with the
<<<<<<< HEAD
 * previous entry. Page is up-to-date. Releases the page.
 */
int nilfs_delete_entry(struct nilfs_dir_entry *dir, struct page *page)
{
	struct address_space *mapping = page->mapping;
	struct inode *inode = mapping->host;
	char *kaddr = page_address(page);
	unsigned from = ((char *)dir - kaddr) & ~(nilfs_chunk_size(inode) - 1);
	unsigned to = ((char *)dir - kaddr) +
		nilfs_rec_len_from_disk(dir->rec_len);
	struct nilfs_dir_entry *pde = NULL;
	struct nilfs_dir_entry *de = (struct nilfs_dir_entry *)(kaddr + from);
	int err;

	while ((char *)de < (char *)dir) {
		if (de->rec_len == 0) {
			nilfs_error(inode->i_sb, __func__,
=======
 * previous entry. Folio is up-to-date.
 */
int nilfs_delete_entry(struct nilfs_dir_entry *dir, struct folio *folio)
{
	struct address_space *mapping = folio->mapping;
	struct inode *inode = mapping->host;
	char *kaddr = (char *)((unsigned long)dir & ~(folio_size(folio) - 1));
	size_t from, to;
	struct nilfs_dir_entry *de, *pde = NULL;
	int err;

	from = ((char *)dir - kaddr) & ~(nilfs_chunk_size(inode) - 1);
	to = ((char *)dir - kaddr) + nilfs_rec_len_from_disk(dir->rec_len);
	de = (struct nilfs_dir_entry *)(kaddr + from);

	while ((char *)de < (char *)dir) {
		if (de->rec_len == 0) {
			nilfs_error(inode->i_sb,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				    "zero-length directory entry");
			err = -EIO;
			goto out;
		}
		pde = de;
		de = nilfs_next_entry(de);
	}
	if (pde)
<<<<<<< HEAD
		from = (char *)pde - (char *)page_address(page);
	lock_page(page);
	err = nilfs_prepare_chunk(page, from, to);
=======
		from = (char *)pde - kaddr;
	folio_lock(folio);
	err = nilfs_prepare_chunk(folio, from, to);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	BUG_ON(err);
	if (pde)
		pde->rec_len = nilfs_rec_len_to_disk(to - from);
	dir->inode = 0;
<<<<<<< HEAD
	nilfs_commit_chunk(page, mapping, from, to);
	inode->i_ctime = inode->i_mtime = CURRENT_TIME;
out:
	nilfs_put_page(page);
=======
	nilfs_commit_chunk(folio, mapping, from, to);
	inode_set_mtime_to_ts(inode, inode_set_ctime_current(inode));
out:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

/*
 * Set the first fragment of directory.
 */
int nilfs_make_empty(struct inode *inode, struct inode *parent)
{
	struct address_space *mapping = inode->i_mapping;
<<<<<<< HEAD
	struct page *page = grab_cache_page(mapping, 0);
	unsigned chunk_size = nilfs_chunk_size(inode);
=======
	struct folio *folio = filemap_grab_folio(mapping, 0);
	unsigned int chunk_size = nilfs_chunk_size(inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct nilfs_dir_entry *de;
	int err;
	void *kaddr;

<<<<<<< HEAD
	if (!page)
		return -ENOMEM;

	err = nilfs_prepare_chunk(page, 0, chunk_size);
	if (unlikely(err)) {
		unlock_page(page);
		goto fail;
	}
	kaddr = kmap_atomic(page);
=======
	if (IS_ERR(folio))
		return PTR_ERR(folio);

	err = nilfs_prepare_chunk(folio, 0, chunk_size);
	if (unlikely(err)) {
		folio_unlock(folio);
		goto fail;
	}
	kaddr = kmap_local_folio(folio, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	memset(kaddr, 0, chunk_size);
	de = (struct nilfs_dir_entry *)kaddr;
	de->name_len = 1;
	de->rec_len = nilfs_rec_len_to_disk(NILFS_DIR_REC_LEN(1));
	memcpy(de->name, ".\0\0", 4);
	de->inode = cpu_to_le64(inode->i_ino);
	nilfs_set_de_type(de, inode);

	de = (struct nilfs_dir_entry *)(kaddr + NILFS_DIR_REC_LEN(1));
	de->name_len = 2;
	de->rec_len = nilfs_rec_len_to_disk(chunk_size - NILFS_DIR_REC_LEN(1));
	de->inode = cpu_to_le64(parent->i_ino);
	memcpy(de->name, "..\0", 4);
	nilfs_set_de_type(de, inode);
<<<<<<< HEAD
	kunmap_atomic(kaddr);
	nilfs_commit_chunk(page, mapping, 0, chunk_size);
fail:
	page_cache_release(page);
=======
	kunmap_local(kaddr);
	nilfs_commit_chunk(folio, mapping, 0, chunk_size);
fail:
	folio_put(folio);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

/*
 * routine to check that the specified directory is empty (for rmdir)
 */
int nilfs_empty_dir(struct inode *inode)
{
<<<<<<< HEAD
	struct page *page = NULL;
	unsigned long i, npages = dir_pages(inode);

	for (i = 0; i < npages; i++) {
		char *kaddr;
		struct nilfs_dir_entry *de;

		page = nilfs_get_page(inode, i);
		if (IS_ERR(page))
			continue;

		kaddr = page_address(page);
=======
	struct folio *folio = NULL;
	char *kaddr;
	unsigned long i, npages = dir_pages(inode);

	for (i = 0; i < npages; i++) {
		struct nilfs_dir_entry *de;

		kaddr = nilfs_get_folio(inode, i, &folio);
		if (IS_ERR(kaddr))
			continue;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		de = (struct nilfs_dir_entry *)kaddr;
		kaddr += nilfs_last_byte(inode, i) - NILFS_DIR_REC_LEN(1);

		while ((char *)de <= kaddr) {
			if (de->rec_len == 0) {
<<<<<<< HEAD
				nilfs_error(inode->i_sb, __func__,
					    "zero-length directory entry "
					    "(kaddr=%p, de=%p)\n", kaddr, de);
=======
				nilfs_error(inode->i_sb,
					    "zero-length directory entry (kaddr=%p, de=%p)",
					    kaddr, de);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				goto not_empty;
			}
			if (de->inode != 0) {
				/* check for . and .. */
				if (de->name[0] != '.')
					goto not_empty;
				if (de->name_len > 2)
					goto not_empty;
				if (de->name_len < 2) {
					if (de->inode !=
					    cpu_to_le64(inode->i_ino))
						goto not_empty;
				} else if (de->name[1] != '.')
					goto not_empty;
			}
			de = nilfs_next_entry(de);
		}
<<<<<<< HEAD
		nilfs_put_page(page);
=======
		folio_release_kmap(folio, kaddr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return 1;

not_empty:
<<<<<<< HEAD
	nilfs_put_page(page);
=======
	folio_release_kmap(folio, kaddr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

const struct file_operations nilfs_dir_operations = {
	.llseek		= generic_file_llseek,
	.read		= generic_read_dir,
<<<<<<< HEAD
	.readdir	= nilfs_readdir,
=======
	.iterate_shared	= nilfs_readdir,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.unlocked_ioctl	= nilfs_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl	= nilfs_compat_ioctl,
#endif	/* CONFIG_COMPAT */
	.fsync		= nilfs_sync_file,

};
