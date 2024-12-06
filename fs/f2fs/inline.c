<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * fs/f2fs/inline.c
 * Copyright (c) 2013, Intel Corporation
 * Authors: Huajun Li <huajun.li@intel.com>
 *          Haicheng Li <haicheng.li@intel.com>
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/fs.h>
#include <linux/f2fs_fs.h>
<<<<<<< HEAD

#include "f2fs.h"
#include "node.h"

bool f2fs_may_inline_data(struct inode *inode)
{
	if (f2fs_is_atomic_file(inode))
		return false;

	if (!S_ISREG(inode->i_mode) && !S_ISLNK(inode->i_mode))
		return false;

	if (i_size_read(inode) > MAX_INLINE_DATA)
		return false;

	if (f2fs_encrypted_inode(inode) && S_ISREG(inode->i_mode))
		return false;

	return true;
}

=======
#include <linux/fiemap.h>

#include "f2fs.h"
#include "node.h"
#include <trace/events/f2fs.h>

static bool support_inline_data(struct inode *inode)
{
	if (f2fs_is_atomic_file(inode))
		return false;
	if (!S_ISREG(inode->i_mode) && !S_ISLNK(inode->i_mode))
		return false;
	if (i_size_read(inode) > MAX_INLINE_DATA(inode))
		return false;
	return true;
}

bool f2fs_may_inline_data(struct inode *inode)
{
	if (!support_inline_data(inode))
		return false;

	return !f2fs_post_read_required(inode);
}

bool f2fs_sanity_check_inline_data(struct inode *inode)
{
	if (!f2fs_has_inline_data(inode))
		return false;

	if (!support_inline_data(inode))
		return true;

	/*
	 * used by sanity_check_inode(), when disk layout fields has not
	 * been synchronized to inmem fields.
	 */
	return (S_ISREG(inode->i_mode) &&
		(file_is_encrypt(inode) || file_is_verity(inode) ||
		(F2FS_I(inode)->i_flags & F2FS_COMPR_FL)));
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
bool f2fs_may_inline_dentry(struct inode *inode)
{
	if (!test_opt(F2FS_I_SB(inode), INLINE_DENTRY))
		return false;

	if (!S_ISDIR(inode->i_mode))
		return false;

	return true;
}

<<<<<<< HEAD
void read_inline_data(struct page *page, struct page *ipage)
{
	void *src_addr, *dst_addr;
=======
void f2fs_do_read_inline_data(struct page *page, struct page *ipage)
{
	struct inode *inode = page->mapping->host;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (PageUptodate(page))
		return;

	f2fs_bug_on(F2FS_P_SB(page), page->index);

<<<<<<< HEAD
	zero_user_segment(page, MAX_INLINE_DATA, PAGE_CACHE_SIZE);

	/* Copy the whole inline data block */
	src_addr = inline_data_addr(ipage);
	dst_addr = kmap_atomic(page);
	memcpy(dst_addr, src_addr, MAX_INLINE_DATA);
	flush_dcache_page(page);
	kunmap_atomic(dst_addr);
	SetPageUptodate(page);
}

bool truncate_inline_inode(struct page *ipage, u64 from)
{
	void *addr;

	if (from >= MAX_INLINE_DATA)
		return false;

	addr = inline_data_addr(ipage);

	f2fs_wait_on_page_writeback(ipage, NODE, true);
	memset(addr + from, 0, MAX_INLINE_DATA - from);

	return true;
=======
	zero_user_segment(page, MAX_INLINE_DATA(inode), PAGE_SIZE);

	/* Copy the whole inline data block */
	memcpy_to_page(page, 0, inline_data_addr(inode, ipage),
		       MAX_INLINE_DATA(inode));
	if (!PageUptodate(page))
		SetPageUptodate(page);
}

void f2fs_truncate_inline_inode(struct inode *inode,
					struct page *ipage, u64 from)
{
	void *addr;

	if (from >= MAX_INLINE_DATA(inode))
		return;

	addr = inline_data_addr(inode, ipage);

	f2fs_wait_on_page_writeback(ipage, NODE, true, true);
	memset(addr + from, 0, MAX_INLINE_DATA(inode) - from);
	set_page_dirty(ipage);

	if (from == 0)
		clear_inode_flag(inode, FI_DATA_EXIST);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int f2fs_read_inline_data(struct inode *inode, struct page *page)
{
	struct page *ipage;

<<<<<<< HEAD
	ipage = get_node_page(F2FS_I_SB(inode), inode->i_ino);
=======
	ipage = f2fs_get_node_page(F2FS_I_SB(inode), inode->i_ino);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (IS_ERR(ipage)) {
		unlock_page(page);
		return PTR_ERR(ipage);
	}

	if (!f2fs_has_inline_data(inode)) {
		f2fs_put_page(ipage, 1);
		return -EAGAIN;
	}

	if (page->index)
<<<<<<< HEAD
		zero_user_segment(page, 0, PAGE_CACHE_SIZE);
	else
		read_inline_data(page, ipage);

	SetPageUptodate(page);
=======
		zero_user_segment(page, 0, PAGE_SIZE);
	else
		f2fs_do_read_inline_data(page, ipage);

	if (!PageUptodate(page))
		SetPageUptodate(page);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	f2fs_put_page(ipage, 1);
	unlock_page(page);
	return 0;
}

int f2fs_convert_inline_page(struct dnode_of_data *dn, struct page *page)
{
	struct f2fs_io_info fio = {
		.sbi = F2FS_I_SB(dn->inode),
<<<<<<< HEAD
		.type = DATA,
		.rw = WRITE_SYNC | REQ_PRIO,
		.page = page,
		.encrypted_page = NULL,
	};
=======
		.ino = dn->inode->i_ino,
		.type = DATA,
		.op = REQ_OP_WRITE,
		.op_flags = REQ_SYNC | REQ_PRIO,
		.page = page,
		.encrypted_page = NULL,
		.io_type = FS_DATA_IO,
	};
	struct node_info ni;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int dirty, err;

	if (!f2fs_exist_data(dn->inode))
		goto clear_out;

	err = f2fs_reserve_block(dn, 0);
	if (err)
		return err;

<<<<<<< HEAD
	if (unlikely(dn->data_blkaddr != NEW_ADDR)) {
		f2fs_put_dnode(dn);
		set_sbi_flag(fio.sbi, SBI_NEED_FSCK);
		f2fs_msg(fio.sbi->sb, KERN_WARNING,
			"%s: corrupted inline inode ino=%lx, i_addr[0]:0x%x, "
			"run fsck to fix.",
			__func__, dn->inode->i_ino, dn->data_blkaddr);
		return -EINVAL;
=======
	err = f2fs_get_node_info(fio.sbi, dn->nid, &ni, false);
	if (err) {
		f2fs_truncate_data_blocks_range(dn, 1);
		f2fs_put_dnode(dn);
		return err;
	}

	fio.version = ni.version;

	if (unlikely(dn->data_blkaddr != NEW_ADDR)) {
		f2fs_put_dnode(dn);
		set_sbi_flag(fio.sbi, SBI_NEED_FSCK);
		f2fs_warn(fio.sbi, "%s: corrupted inline inode ino=%lx, i_addr[0]:0x%x, run fsck to fix.",
			  __func__, dn->inode->i_ino, dn->data_blkaddr);
		f2fs_handle_error(fio.sbi, ERROR_INVALID_BLKADDR);
		return -EFSCORRUPTED;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	f2fs_bug_on(F2FS_P_SB(page), PageWriteback(page));

<<<<<<< HEAD
	read_inline_data(page, dn->inode_page);
=======
	f2fs_do_read_inline_data(page, dn->inode_page);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	set_page_dirty(page);

	/* clear dirty state */
	dirty = clear_page_dirty_for_io(page);

	/* write data page to try to make data consistent */
	set_page_writeback(page);
	fio.old_blkaddr = dn->data_blkaddr;
<<<<<<< HEAD
	write_data_page(dn, &fio);
	f2fs_wait_on_page_writeback(page, DATA, true);
	if (dirty)
		inode_dec_dirty_pages(dn->inode);

	/* this converted inline_data should be recovered. */
	set_inode_flag(F2FS_I(dn->inode), FI_APPEND_WRITE);

	/* clear inline data and flag after data writeback */
	truncate_inline_inode(dn->inode_page, 0);
	clear_inline_node(dn->inode_page);
clear_out:
	stat_dec_inline_inode(dn->inode);
	f2fs_clear_inline_inode(dn->inode);
	sync_inode_page(dn);
=======
	set_inode_flag(dn->inode, FI_HOT_DATA);
	f2fs_outplace_write_data(dn, &fio);
	f2fs_wait_on_page_writeback(page, DATA, true, true);
	if (dirty) {
		inode_dec_dirty_pages(dn->inode);
		f2fs_remove_dirty_inode(dn->inode);
	}

	/* this converted inline_data should be recovered. */
	set_inode_flag(dn->inode, FI_APPEND_WRITE);

	/* clear inline data and flag after data writeback */
	f2fs_truncate_inline_inode(dn->inode, dn->inode_page, 0);
	clear_page_private_inline(dn->inode_page);
clear_out:
	stat_dec_inline_inode(dn->inode);
	clear_inode_flag(dn->inode, FI_INLINE_DATA);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	f2fs_put_dnode(dn);
	return 0;
}

int f2fs_convert_inline_inode(struct inode *inode)
{
	struct f2fs_sb_info *sbi = F2FS_I_SB(inode);
	struct dnode_of_data dn;
	struct page *ipage, *page;
	int err = 0;

<<<<<<< HEAD
	if (!f2fs_has_inline_data(inode))
		return 0;

	page = grab_cache_page(inode->i_mapping, 0);
=======
	if (!f2fs_has_inline_data(inode) ||
			f2fs_hw_is_readonly(sbi) || f2fs_readonly(sbi->sb))
		return 0;

	err = f2fs_dquot_initialize(inode);
	if (err)
		return err;

	page = f2fs_grab_cache_page(inode->i_mapping, 0, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!page)
		return -ENOMEM;

	f2fs_lock_op(sbi);

<<<<<<< HEAD
	ipage = get_node_page(sbi, inode->i_ino);
=======
	ipage = f2fs_get_node_page(sbi, inode->i_ino);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (IS_ERR(ipage)) {
		err = PTR_ERR(ipage);
		goto out;
	}

	set_new_dnode(&dn, inode, ipage, ipage, 0);

	if (f2fs_has_inline_data(inode))
		err = f2fs_convert_inline_page(&dn, page);

	f2fs_put_dnode(&dn);
out:
	f2fs_unlock_op(sbi);

	f2fs_put_page(page, 1);

<<<<<<< HEAD
	f2fs_balance_fs(sbi, dn.node_changed);
=======
	if (!err)
		f2fs_balance_fs(sbi, dn.node_changed);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return err;
}

int f2fs_write_inline_data(struct inode *inode, struct page *page)
{
<<<<<<< HEAD
	void *src_addr, *dst_addr;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct dnode_of_data dn;
	int err;

	set_new_dnode(&dn, inode, NULL, NULL, 0);
<<<<<<< HEAD
	err = get_dnode_of_data(&dn, 0, LOOKUP_NODE);
=======
	err = f2fs_get_dnode_of_data(&dn, 0, LOOKUP_NODE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err)
		return err;

	if (!f2fs_has_inline_data(inode)) {
		f2fs_put_dnode(&dn);
		return -EAGAIN;
	}

	f2fs_bug_on(F2FS_I_SB(inode), page->index);

<<<<<<< HEAD
	f2fs_wait_on_page_writeback(dn.inode_page, NODE, true);
	src_addr = kmap_atomic(page);
	dst_addr = inline_data_addr(dn.inode_page);
	memcpy(dst_addr, src_addr, MAX_INLINE_DATA);
	kunmap_atomic(src_addr);

	set_inode_flag(F2FS_I(inode), FI_APPEND_WRITE);
	set_inode_flag(F2FS_I(inode), FI_DATA_EXIST);

	sync_inode_page(&dn);
	clear_inline_node(dn.inode_page);
=======
	f2fs_wait_on_page_writeback(dn.inode_page, NODE, true, true);
	memcpy_from_page(inline_data_addr(inode, dn.inode_page),
			 page, 0, MAX_INLINE_DATA(inode));
	set_page_dirty(dn.inode_page);

	f2fs_clear_page_cache_dirty_tag(page);

	set_inode_flag(inode, FI_APPEND_WRITE);
	set_inode_flag(inode, FI_DATA_EXIST);

	clear_page_private_inline(dn.inode_page);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	f2fs_put_dnode(&dn);
	return 0;
}

<<<<<<< HEAD
bool recover_inline_data(struct inode *inode, struct page *npage)
=======
int f2fs_recover_inline_data(struct inode *inode, struct page *npage)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct f2fs_sb_info *sbi = F2FS_I_SB(inode);
	struct f2fs_inode *ri = NULL;
	void *src_addr, *dst_addr;
	struct page *ipage;

	/*
	 * The inline_data recovery policy is as follows.
	 * [prev.] [next] of inline_data flag
	 *    o       o  -> recover inline_data
	 *    o       x  -> remove inline_data, and then recover data blocks
<<<<<<< HEAD
	 *    x       o  -> remove inline_data, and then recover inline_data
=======
	 *    x       o  -> remove data blocks, and then recover inline_data
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 *    x       x  -> recover data blocks
	 */
	if (IS_INODE(npage))
		ri = F2FS_INODE(npage);

	if (f2fs_has_inline_data(inode) &&
			ri && (ri->i_inline & F2FS_INLINE_DATA)) {
process_inline:
<<<<<<< HEAD
		ipage = get_node_page(sbi, inode->i_ino);
		f2fs_bug_on(sbi, IS_ERR(ipage));

		f2fs_wait_on_page_writeback(ipage, NODE, true);

		src_addr = inline_data_addr(npage);
		dst_addr = inline_data_addr(ipage);
		memcpy(dst_addr, src_addr, MAX_INLINE_DATA);

		set_inode_flag(F2FS_I(inode), FI_INLINE_DATA);
		set_inode_flag(F2FS_I(inode), FI_DATA_EXIST);

		update_inode(inode, ipage);
		f2fs_put_page(ipage, 1);
		return true;
	}

	if (f2fs_has_inline_data(inode)) {
		ipage = get_node_page(sbi, inode->i_ino);
		f2fs_bug_on(sbi, IS_ERR(ipage));
		if (!truncate_inline_inode(ipage, 0))
			return false;
		f2fs_clear_inline_inode(inode);
		update_inode(inode, ipage);
		f2fs_put_page(ipage, 1);
	} else if (ri && (ri->i_inline & F2FS_INLINE_DATA)) {
		if (truncate_blocks(inode, 0, false))
			return false;
		goto process_inline;
	}
	return false;
}

struct f2fs_dir_entry *find_in_inline_dir(struct inode *dir,
			struct fscrypt_name *fname, struct page **res_page)
{
	struct f2fs_sb_info *sbi = F2FS_SB(dir->i_sb);
	struct f2fs_inline_dentry *inline_dentry;
	struct qstr name = FSTR_TO_QSTR(&fname->disk_name);
	struct f2fs_dir_entry *de;
	struct f2fs_dentry_ptr d;
	struct page *ipage;
	f2fs_hash_t namehash;

	ipage = get_node_page(sbi, dir->i_ino);
	if (IS_ERR(ipage))
		return NULL;

	namehash = f2fs_dentry_hash(&name);

	inline_dentry = inline_data_addr(ipage);

	make_dentry_ptr(NULL, &d, (void *)inline_dentry, 2);
	de = find_target_dentry(fname, namehash, NULL, &d);
	unlock_page(ipage);
=======
		ipage = f2fs_get_node_page(sbi, inode->i_ino);
		if (IS_ERR(ipage))
			return PTR_ERR(ipage);

		f2fs_wait_on_page_writeback(ipage, NODE, true, true);

		src_addr = inline_data_addr(inode, npage);
		dst_addr = inline_data_addr(inode, ipage);
		memcpy(dst_addr, src_addr, MAX_INLINE_DATA(inode));

		set_inode_flag(inode, FI_INLINE_DATA);
		set_inode_flag(inode, FI_DATA_EXIST);

		set_page_dirty(ipage);
		f2fs_put_page(ipage, 1);
		return 1;
	}

	if (f2fs_has_inline_data(inode)) {
		ipage = f2fs_get_node_page(sbi, inode->i_ino);
		if (IS_ERR(ipage))
			return PTR_ERR(ipage);
		f2fs_truncate_inline_inode(inode, ipage, 0);
		stat_dec_inline_inode(inode);
		clear_inode_flag(inode, FI_INLINE_DATA);
		f2fs_put_page(ipage, 1);
	} else if (ri && (ri->i_inline & F2FS_INLINE_DATA)) {
		int ret;

		ret = f2fs_truncate_blocks(inode, 0, false);
		if (ret)
			return ret;
		stat_inc_inline_inode(inode);
		goto process_inline;
	}
	return 0;
}

struct f2fs_dir_entry *f2fs_find_in_inline_dir(struct inode *dir,
					const struct f2fs_filename *fname,
					struct page **res_page)
{
	struct f2fs_sb_info *sbi = F2FS_SB(dir->i_sb);
	struct f2fs_dir_entry *de;
	struct f2fs_dentry_ptr d;
	struct page *ipage;
	void *inline_dentry;

	ipage = f2fs_get_node_page(sbi, dir->i_ino);
	if (IS_ERR(ipage)) {
		*res_page = ipage;
		return NULL;
	}

	inline_dentry = inline_data_addr(dir, ipage);

	make_dentry_ptr_inline(dir, &d, inline_dentry);
	de = f2fs_find_target_dentry(&d, fname, NULL);
	unlock_page(ipage);
	if (IS_ERR(de)) {
		*res_page = ERR_CAST(de);
		de = NULL;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (de)
		*res_page = ipage;
	else
		f2fs_put_page(ipage, 0);

<<<<<<< HEAD
	/*
	 * For the most part, it should be a bug when name_len is zero.
	 * We stop here for figuring out where the bugs has occurred.
	 */
	f2fs_bug_on(sbi, d.max < 0);
	return de;
}

struct f2fs_dir_entry *f2fs_parent_inline_dir(struct inode *dir,
							struct page **p)
{
	struct f2fs_sb_info *sbi = F2FS_I_SB(dir);
	struct page *ipage;
	struct f2fs_dir_entry *de;
	struct f2fs_inline_dentry *dentry_blk;

	ipage = get_node_page(sbi, dir->i_ino);
	if (IS_ERR(ipage))
		return NULL;

	dentry_blk = inline_data_addr(ipage);
	de = &dentry_blk->dentry[1];
	*p = ipage;
	unlock_page(ipage);
	return de;
}

int make_empty_inline_dir(struct inode *inode, struct inode *parent,
							struct page *ipage)
{
	struct f2fs_inline_dentry *dentry_blk;
	struct f2fs_dentry_ptr d;

	dentry_blk = inline_data_addr(ipage);

	make_dentry_ptr(NULL, &d, (void *)dentry_blk, 2);
	do_make_empty_dir(inode, parent, &d);
=======
	return de;
}

int f2fs_make_empty_inline_dir(struct inode *inode, struct inode *parent,
							struct page *ipage)
{
	struct f2fs_dentry_ptr d;
	void *inline_dentry;

	inline_dentry = inline_data_addr(inode, ipage);

	make_dentry_ptr_inline(inode, &d, inline_dentry);
	f2fs_do_make_empty_dir(inode, parent, &d);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	set_page_dirty(ipage);

	/* update i_size to MAX_INLINE_DATA */
<<<<<<< HEAD
	if (i_size_read(inode) < MAX_INLINE_DATA) {
		i_size_write(inode, MAX_INLINE_DATA);
		set_inode_flag(F2FS_I(inode), FI_UPDATE_DIR);
	}
=======
	if (i_size_read(inode) < MAX_INLINE_DATA(inode))
		f2fs_i_size_write(inode, MAX_INLINE_DATA(inode));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

/*
 * NOTE: ipage is grabbed by caller, but if any error occurs, we should
 * release ipage in this function.
 */
<<<<<<< HEAD
static int f2fs_convert_inline_dir(struct inode *dir, struct page *ipage,
				struct f2fs_inline_dentry *inline_dentry)
=======
static int f2fs_move_inline_dirents(struct inode *dir, struct page *ipage,
							void *inline_dentry)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct page *page;
	struct dnode_of_data dn;
	struct f2fs_dentry_block *dentry_blk;
<<<<<<< HEAD
	int err;

	page = grab_cache_page(dir->i_mapping, 0);
=======
	struct f2fs_dentry_ptr src, dst;
	int err;

	page = f2fs_grab_cache_page(dir->i_mapping, 0, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!page) {
		f2fs_put_page(ipage, 1);
		return -ENOMEM;
	}

	set_new_dnode(&dn, dir, ipage, NULL, 0);
	err = f2fs_reserve_block(&dn, 0);
	if (err)
		goto out;

	if (unlikely(dn.data_blkaddr != NEW_ADDR)) {
		f2fs_put_dnode(&dn);
		set_sbi_flag(F2FS_P_SB(page), SBI_NEED_FSCK);
<<<<<<< HEAD
		f2fs_msg(F2FS_P_SB(page)->sb, KERN_WARNING,
			"%s: corrupted inline inode ino=%lx, i_addr[0]:0x%x, "
			"run fsck to fix.",
			__func__, dir->i_ino, dn.data_blkaddr);
		err = -EINVAL;
		goto out;
	}

	f2fs_wait_on_page_writeback(page, DATA, true);
	zero_user_segment(page, MAX_INLINE_DATA, PAGE_CACHE_SIZE);

	dentry_blk = kmap_atomic(page);

	/* copy data from inline dentry block to new dentry block */
	memcpy(dentry_blk->dentry_bitmap, inline_dentry->dentry_bitmap,
					INLINE_DENTRY_BITMAP_SIZE);
	memset(dentry_blk->dentry_bitmap + INLINE_DENTRY_BITMAP_SIZE, 0,
			SIZE_OF_DENTRY_BITMAP - INLINE_DENTRY_BITMAP_SIZE);
	/*
	 * we do not need to zero out remainder part of dentry and filename
	 * field, since we have used bitmap for marking the usage status of
	 * them, besides, we can also ignore copying/zeroing reserved space
	 * of dentry block, because them haven't been used so far.
	 */
	memcpy(dentry_blk->dentry, inline_dentry->dentry,
			sizeof(struct f2fs_dir_entry) * NR_INLINE_DENTRY);
	memcpy(dentry_blk->filename, inline_dentry->filename,
					NR_INLINE_DENTRY * F2FS_SLOT_LEN);

	kunmap_atomic(dentry_blk);
	SetPageUptodate(page);
	set_page_dirty(page);

	/* clear inline dir and flag after data writeback */
	truncate_inline_inode(ipage, 0);

	stat_dec_inline_dir(dir);
	clear_inode_flag(F2FS_I(dir), FI_INLINE_DENTRY);

	if (i_size_read(dir) < PAGE_CACHE_SIZE) {
		i_size_write(dir, PAGE_CACHE_SIZE);
		set_inode_flag(F2FS_I(dir), FI_UPDATE_DIR);
	}

	sync_inode_page(&dn);
=======
		f2fs_warn(F2FS_P_SB(page), "%s: corrupted inline inode ino=%lx, i_addr[0]:0x%x, run fsck to fix.",
			  __func__, dir->i_ino, dn.data_blkaddr);
		f2fs_handle_error(F2FS_P_SB(page), ERROR_INVALID_BLKADDR);
		err = -EFSCORRUPTED;
		goto out;
	}

	f2fs_wait_on_page_writeback(page, DATA, true, true);

	dentry_blk = page_address(page);

	/*
	 * Start by zeroing the full block, to ensure that all unused space is
	 * zeroed and no uninitialized memory is leaked to disk.
	 */
	memset(dentry_blk, 0, F2FS_BLKSIZE);

	make_dentry_ptr_inline(dir, &src, inline_dentry);
	make_dentry_ptr_block(dir, &dst, dentry_blk);

	/* copy data from inline dentry block to new dentry block */
	memcpy(dst.bitmap, src.bitmap, src.nr_bitmap);
	memcpy(dst.dentry, src.dentry, SIZE_OF_DIR_ENTRY * src.max);
	memcpy(dst.filename, src.filename, src.max * F2FS_SLOT_LEN);

	if (!PageUptodate(page))
		SetPageUptodate(page);
	set_page_dirty(page);

	/* clear inline dir and flag after data writeback */
	f2fs_truncate_inline_inode(dir, ipage, 0);

	stat_dec_inline_dir(dir);
	clear_inode_flag(dir, FI_INLINE_DENTRY);

	/*
	 * should retrieve reserved space which was used to keep
	 * inline_dentry's structure for backward compatibility.
	 */
	if (!f2fs_sb_has_flexible_inline_xattr(F2FS_I_SB(dir)) &&
			!f2fs_has_inline_xattr(dir))
		F2FS_I(dir)->i_inline_xattr_size = 0;

	f2fs_i_depth_write(dir, 1);
	if (i_size_read(dir) < PAGE_SIZE)
		f2fs_i_size_write(dir, PAGE_SIZE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	f2fs_put_page(page, 1);
	return err;
}

<<<<<<< HEAD
int f2fs_add_inline_entry(struct inode *dir, const struct qstr *name,
			struct inode *inode, nid_t ino, umode_t mode)
=======
static int f2fs_add_inline_entries(struct inode *dir, void *inline_dentry)
{
	struct f2fs_dentry_ptr d;
	unsigned long bit_pos = 0;
	int err = 0;

	make_dentry_ptr_inline(dir, &d, inline_dentry);

	while (bit_pos < d.max) {
		struct f2fs_dir_entry *de;
		struct f2fs_filename fname;
		nid_t ino;
		umode_t fake_mode;

		if (!test_bit_le(bit_pos, d.bitmap)) {
			bit_pos++;
			continue;
		}

		de = &d.dentry[bit_pos];

		if (unlikely(!de->name_len)) {
			bit_pos++;
			continue;
		}

		/*
		 * We only need the disk_name and hash to move the dentry.
		 * We don't need the original or casefolded filenames.
		 */
		memset(&fname, 0, sizeof(fname));
		fname.disk_name.name = d.filename[bit_pos];
		fname.disk_name.len = le16_to_cpu(de->name_len);
		fname.hash = de->hash_code;

		ino = le32_to_cpu(de->ino);
		fake_mode = fs_ftype_to_dtype(de->file_type) << S_DT_SHIFT;

		err = f2fs_add_regular_entry(dir, &fname, NULL, ino, fake_mode);
		if (err)
			goto punch_dentry_pages;

		bit_pos += GET_DENTRY_SLOTS(le16_to_cpu(de->name_len));
	}
	return 0;
punch_dentry_pages:
	truncate_inode_pages(&dir->i_data, 0);
	f2fs_truncate_blocks(dir, 0, false);
	f2fs_remove_dirty_inode(dir);
	return err;
}

static int f2fs_move_rehashed_dirents(struct inode *dir, struct page *ipage,
							void *inline_dentry)
{
	void *backup_dentry;
	int err;

	backup_dentry = f2fs_kmalloc(F2FS_I_SB(dir),
				MAX_INLINE_DATA(dir), GFP_F2FS_ZERO);
	if (!backup_dentry) {
		f2fs_put_page(ipage, 1);
		return -ENOMEM;
	}

	memcpy(backup_dentry, inline_dentry, MAX_INLINE_DATA(dir));
	f2fs_truncate_inline_inode(dir, ipage, 0);

	unlock_page(ipage);

	err = f2fs_add_inline_entries(dir, backup_dentry);
	if (err)
		goto recover;

	lock_page(ipage);

	stat_dec_inline_dir(dir);
	clear_inode_flag(dir, FI_INLINE_DENTRY);

	/*
	 * should retrieve reserved space which was used to keep
	 * inline_dentry's structure for backward compatibility.
	 */
	if (!f2fs_sb_has_flexible_inline_xattr(F2FS_I_SB(dir)) &&
			!f2fs_has_inline_xattr(dir))
		F2FS_I(dir)->i_inline_xattr_size = 0;

	kfree(backup_dentry);
	return 0;
recover:
	lock_page(ipage);
	f2fs_wait_on_page_writeback(ipage, NODE, true, true);
	memcpy(inline_dentry, backup_dentry, MAX_INLINE_DATA(dir));
	f2fs_i_depth_write(dir, 0);
	f2fs_i_size_write(dir, MAX_INLINE_DATA(dir));
	set_page_dirty(ipage);
	f2fs_put_page(ipage, 1);

	kfree(backup_dentry);
	return err;
}

static int do_convert_inline_dir(struct inode *dir, struct page *ipage,
							void *inline_dentry)
{
	if (!F2FS_I(dir)->i_dir_level)
		return f2fs_move_inline_dirents(dir, ipage, inline_dentry);
	else
		return f2fs_move_rehashed_dirents(dir, ipage, inline_dentry);
}

int f2fs_try_convert_inline_dir(struct inode *dir, struct dentry *dentry)
{
	struct f2fs_sb_info *sbi = F2FS_I_SB(dir);
	struct page *ipage;
	struct f2fs_filename fname;
	void *inline_dentry = NULL;
	int err = 0;

	if (!f2fs_has_inline_dentry(dir))
		return 0;

	f2fs_lock_op(sbi);

	err = f2fs_setup_filename(dir, &dentry->d_name, 0, &fname);
	if (err)
		goto out;

	ipage = f2fs_get_node_page(sbi, dir->i_ino);
	if (IS_ERR(ipage)) {
		err = PTR_ERR(ipage);
		goto out_fname;
	}

	if (f2fs_has_enough_room(dir, ipage, &fname)) {
		f2fs_put_page(ipage, 1);
		goto out_fname;
	}

	inline_dentry = inline_data_addr(dir, ipage);

	err = do_convert_inline_dir(dir, ipage, inline_dentry);
	if (!err)
		f2fs_put_page(ipage, 1);
out_fname:
	f2fs_free_filename(&fname);
out:
	f2fs_unlock_op(sbi);
	return err;
}

int f2fs_add_inline_entry(struct inode *dir, const struct f2fs_filename *fname,
			  struct inode *inode, nid_t ino, umode_t mode)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct f2fs_sb_info *sbi = F2FS_I_SB(dir);
	struct page *ipage;
	unsigned int bit_pos;
<<<<<<< HEAD
	f2fs_hash_t name_hash;
	size_t namelen = name->len;
	struct f2fs_inline_dentry *dentry_blk = NULL;
	struct f2fs_dentry_ptr d;
	int slots = GET_DENTRY_SLOTS(namelen);
	struct page *page = NULL;
	int err = 0;

	ipage = get_node_page(sbi, dir->i_ino);
	if (IS_ERR(ipage))
		return PTR_ERR(ipage);

	dentry_blk = inline_data_addr(ipage);
	bit_pos = room_for_filename(&dentry_blk->dentry_bitmap,
						slots, NR_INLINE_DENTRY);
	if (bit_pos >= NR_INLINE_DENTRY) {
		err = f2fs_convert_inline_dir(dir, ipage, dentry_blk);
=======
	void *inline_dentry = NULL;
	struct f2fs_dentry_ptr d;
	int slots = GET_DENTRY_SLOTS(fname->disk_name.len);
	struct page *page = NULL;
	int err = 0;

	ipage = f2fs_get_node_page(sbi, dir->i_ino);
	if (IS_ERR(ipage))
		return PTR_ERR(ipage);

	inline_dentry = inline_data_addr(dir, ipage);
	make_dentry_ptr_inline(dir, &d, inline_dentry);

	bit_pos = f2fs_room_for_filename(d.bitmap, slots, d.max);
	if (bit_pos >= d.max) {
		err = do_convert_inline_dir(dir, ipage, inline_dentry);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (err)
			return err;
		err = -EAGAIN;
		goto out;
	}

	if (inode) {
<<<<<<< HEAD
		down_write(&F2FS_I(inode)->i_sem);
		page = init_inode_metadata(inode, dir, name, ipage);
=======
		f2fs_down_write_nested(&F2FS_I(inode)->i_sem,
						SINGLE_DEPTH_NESTING);
		page = f2fs_init_inode_metadata(inode, dir, fname, ipage);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (IS_ERR(page)) {
			err = PTR_ERR(page);
			goto fail;
		}
	}

<<<<<<< HEAD
	f2fs_wait_on_page_writeback(ipage, NODE, true);

	name_hash = f2fs_dentry_hash(name);
	make_dentry_ptr(NULL, &d, (void *)dentry_blk, 2);
	f2fs_update_dentry(ino, mode, &d, name, name_hash, bit_pos);
=======
	f2fs_wait_on_page_writeback(ipage, NODE, true, true);

	f2fs_update_dentry(ino, mode, &d, &fname->disk_name, fname->hash,
			   bit_pos);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	set_page_dirty(ipage);

	/* we don't need to mark_inode_dirty now */
	if (inode) {
<<<<<<< HEAD
		F2FS_I(inode)->i_pino = dir->i_ino;
		update_inode(inode, page);
		f2fs_put_page(page, 1);
	}

	update_parent_metadata(dir, inode, 0);
fail:
	if (inode)
		up_write(&F2FS_I(inode)->i_sem);

	if (is_inode_flag_set(F2FS_I(dir), FI_UPDATE_DIR)) {
		update_inode(dir, ipage);
		clear_inode_flag(F2FS_I(dir), FI_UPDATE_DIR);
	}
=======
		f2fs_i_pino_write(inode, dir->i_ino);

		/* synchronize inode page's data from inode cache */
		if (is_inode_flag_set(inode, FI_NEW_INODE))
			f2fs_update_inode(inode, page);

		f2fs_put_page(page, 1);
	}

	f2fs_update_parent_metadata(dir, inode, 0);
fail:
	if (inode)
		f2fs_up_write(&F2FS_I(inode)->i_sem);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	f2fs_put_page(ipage, 1);
	return err;
}

void f2fs_delete_inline_entry(struct f2fs_dir_entry *dentry, struct page *page,
					struct inode *dir, struct inode *inode)
{
<<<<<<< HEAD
	struct f2fs_inline_dentry *inline_dentry;
=======
	struct f2fs_dentry_ptr d;
	void *inline_dentry;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int slots = GET_DENTRY_SLOTS(le16_to_cpu(dentry->name_len));
	unsigned int bit_pos;
	int i;

	lock_page(page);
<<<<<<< HEAD
	f2fs_wait_on_page_writeback(page, NODE, true);

	inline_dentry = inline_data_addr(page);
	bit_pos = dentry - inline_dentry->dentry;
	for (i = 0; i < slots; i++)
		test_and_clear_bit_le(bit_pos + i,
				&inline_dentry->dentry_bitmap);

	set_page_dirty(page);

	dir->i_ctime = dir->i_mtime = CURRENT_TIME;

	if (inode)
		f2fs_drop_nlink(dir, inode, page);

	f2fs_put_page(page, 1);
=======
	f2fs_wait_on_page_writeback(page, NODE, true, true);

	inline_dentry = inline_data_addr(dir, page);
	make_dentry_ptr_inline(dir, &d, inline_dentry);

	bit_pos = dentry - d.dentry;
	for (i = 0; i < slots; i++)
		__clear_bit_le(bit_pos + i, d.bitmap);

	set_page_dirty(page);
	f2fs_put_page(page, 1);

	inode_set_mtime_to_ts(dir, inode_set_ctime_current(dir));
	f2fs_mark_inode_dirty_sync(dir, false);

	if (inode)
		f2fs_drop_nlink(dir, inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

bool f2fs_empty_inline_dir(struct inode *dir)
{
	struct f2fs_sb_info *sbi = F2FS_I_SB(dir);
	struct page *ipage;
	unsigned int bit_pos = 2;
<<<<<<< HEAD
	struct f2fs_inline_dentry *dentry_blk;

	ipage = get_node_page(sbi, dir->i_ino);
	if (IS_ERR(ipage))
		return false;

	dentry_blk = inline_data_addr(ipage);
	bit_pos = find_next_bit_le(&dentry_blk->dentry_bitmap,
					NR_INLINE_DENTRY,
					bit_pos);

	f2fs_put_page(ipage, 1);

	if (bit_pos < NR_INLINE_DENTRY)
=======
	void *inline_dentry;
	struct f2fs_dentry_ptr d;

	ipage = f2fs_get_node_page(sbi, dir->i_ino);
	if (IS_ERR(ipage))
		return false;

	inline_dentry = inline_data_addr(dir, ipage);
	make_dentry_ptr_inline(dir, &d, inline_dentry);

	bit_pos = find_next_bit_le(d.bitmap, d.max, bit_pos);

	f2fs_put_page(ipage, 1);

	if (bit_pos < d.max)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return false;

	return true;
}

<<<<<<< HEAD
int f2fs_read_inline_dir(struct file *file, void *dirent, filldir_t filldir,
						struct fscrypt_str *fstr)
{
	unsigned long pos = file->f_pos;
	unsigned int bit_pos = 0;
	struct inode *inode = file_inode(file);
	struct f2fs_inline_dentry *inline_dentry = NULL;
	struct page *ipage = NULL;
	struct f2fs_dentry_ptr d;

	if (pos >= NR_INLINE_DENTRY)
		return 0;

	bit_pos = (pos % NR_INLINE_DENTRY);

	ipage = get_node_page(F2FS_I_SB(inode), inode->i_ino);
	if (IS_ERR(ipage))
		return PTR_ERR(ipage);

	inline_dentry = inline_data_addr(ipage);

	make_dentry_ptr(inode, &d, (void *)inline_dentry, 2);

	if (!f2fs_fill_dentries(file, dirent, filldir, &d, 0, bit_pos, fstr))
		file->f_pos = NR_INLINE_DENTRY;

	f2fs_put_page(ipage, 1);
	return 0;
=======
int f2fs_read_inline_dir(struct file *file, struct dir_context *ctx,
				struct fscrypt_str *fstr)
{
	struct inode *inode = file_inode(file);
	struct page *ipage = NULL;
	struct f2fs_dentry_ptr d;
	void *inline_dentry = NULL;
	int err;

	make_dentry_ptr_inline(inode, &d, inline_dentry);

	if (ctx->pos == d.max)
		return 0;

	ipage = f2fs_get_node_page(F2FS_I_SB(inode), inode->i_ino);
	if (IS_ERR(ipage))
		return PTR_ERR(ipage);

	/*
	 * f2fs_readdir was protected by inode.i_rwsem, it is safe to access
	 * ipage without page's lock held.
	 */
	unlock_page(ipage);

	inline_dentry = inline_data_addr(inode, ipage);

	make_dentry_ptr_inline(inode, &d, inline_dentry);

	err = f2fs_fill_dentries(ctx, &d, 0, fstr);
	if (!err)
		ctx->pos = d.max;

	f2fs_put_page(ipage, 0);
	return err < 0 ? err : 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int f2fs_inline_data_fiemap(struct inode *inode,
		struct fiemap_extent_info *fieinfo, __u64 start, __u64 len)
{
	__u64 byteaddr, ilen;
	__u32 flags = FIEMAP_EXTENT_DATA_INLINE | FIEMAP_EXTENT_NOT_ALIGNED |
		FIEMAP_EXTENT_LAST;
	struct node_info ni;
	struct page *ipage;
	int err = 0;

<<<<<<< HEAD
	ipage = get_node_page(F2FS_I_SB(inode), inode->i_ino);
	if (IS_ERR(ipage))
		return PTR_ERR(ipage);

	if (!f2fs_has_inline_data(inode)) {
=======
	ipage = f2fs_get_node_page(F2FS_I_SB(inode), inode->i_ino);
	if (IS_ERR(ipage))
		return PTR_ERR(ipage);

	if ((S_ISREG(inode->i_mode) || S_ISLNK(inode->i_mode)) &&
				!f2fs_has_inline_data(inode)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = -EAGAIN;
		goto out;
	}

<<<<<<< HEAD
	ilen = min_t(size_t, MAX_INLINE_DATA, i_size_read(inode));
=======
	if (S_ISDIR(inode->i_mode) && !f2fs_has_inline_dentry(inode)) {
		err = -EAGAIN;
		goto out;
	}

	ilen = min_t(size_t, MAX_INLINE_DATA(inode), i_size_read(inode));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (start >= ilen)
		goto out;
	if (start + len < ilen)
		ilen = start + len;
	ilen -= start;

<<<<<<< HEAD
	get_node_info(F2FS_I_SB(inode), inode->i_ino, &ni);
	byteaddr = (__u64)ni.blk_addr << inode->i_sb->s_blocksize_bits;
	byteaddr += (char *)inline_data_addr(ipage) - (char *)F2FS_INODE(ipage);
	err = fiemap_fill_next_extent(fieinfo, start, byteaddr, ilen, flags);
=======
	err = f2fs_get_node_info(F2FS_I_SB(inode), inode->i_ino, &ni, false);
	if (err)
		goto out;

	byteaddr = (__u64)ni.blk_addr << inode->i_sb->s_blocksize_bits;
	byteaddr += (char *)inline_data_addr(inode, ipage) -
					(char *)F2FS_INODE(ipage);
	err = fiemap_fill_next_extent(fieinfo, start, byteaddr, ilen, flags);
	trace_f2fs_fiemap(inode, start, byteaddr, ilen, flags, err);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	f2fs_put_page(ipage, 1);
	return err;
}
