<<<<<<< HEAD
/*
 *   Copyright (C) International Business Machines Corp., 2000-2004
 *   Portions Copyright (C) Christoph Hellwig, 2001-2002
 *
 *   This program is free software;  you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY;  without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 *   the GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program;  if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 *   Copyright (C) International Business Machines Corp., 2000-2004
 *   Portions Copyright (C) Christoph Hellwig, 2001-2002
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/fs.h>
#include <linux/mpage.h>
#include <linux/buffer_head.h>
#include <linux/pagemap.h>
#include <linux/quotaops.h>
<<<<<<< HEAD
=======
#include <linux/uio.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/writeback.h>
#include "jfs_incore.h"
#include "jfs_inode.h"
#include "jfs_filsys.h"
#include "jfs_imap.h"
#include "jfs_extent.h"
#include "jfs_unicode.h"
#include "jfs_debug.h"
<<<<<<< HEAD
=======
#include "jfs_dmap.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)


struct inode *jfs_iget(struct super_block *sb, unsigned long ino)
{
	struct inode *inode;
	int ret;

	inode = iget_locked(sb, ino);
	if (!inode)
		return ERR_PTR(-ENOMEM);
	if (!(inode->i_state & I_NEW))
		return inode;

	ret = diRead(inode);
	if (ret < 0) {
		iget_failed(inode);
		return ERR_PTR(ret);
	}

	if (S_ISREG(inode->i_mode)) {
		inode->i_op = &jfs_file_inode_operations;
		inode->i_fop = &jfs_file_operations;
		inode->i_mapping->a_ops = &jfs_aops;
	} else if (S_ISDIR(inode->i_mode)) {
		inode->i_op = &jfs_dir_inode_operations;
		inode->i_fop = &jfs_dir_operations;
	} else if (S_ISLNK(inode->i_mode)) {
		if (inode->i_size >= IDATASIZE) {
			inode->i_op = &page_symlink_inode_operations;
<<<<<<< HEAD
			inode->i_mapping->a_ops = &jfs_aops;
		} else {
			inode->i_op = &jfs_fast_symlink_inode_operations;
=======
			inode_nohighmem(inode);
			inode->i_mapping->a_ops = &jfs_aops;
		} else {
			inode->i_op = &jfs_fast_symlink_inode_operations;
			inode->i_link = JFS_IP(inode)->i_inline;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/*
			 * The inline data should be null-terminated, but
			 * don't let on-disk corruption crash the kernel
			 */
<<<<<<< HEAD
			JFS_IP(inode)->i_inline[inode->i_size] = '\0';
=======
			inode->i_link[inode->i_size] = '\0';
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	} else {
		inode->i_op = &jfs_file_inode_operations;
		init_special_inode(inode, inode->i_mode, inode->i_rdev);
	}
	unlock_new_inode(inode);
	return inode;
}

/*
 * Workhorse of both fsync & write_inode
 */
int jfs_commit_inode(struct inode *inode, int wait)
{
	int rc = 0;
	tid_t tid;
	static int noisy = 5;

	jfs_info("In jfs_commit_inode, inode = 0x%p", inode);

	/*
	 * Don't commit if inode has been committed since last being
	 * marked dirty, or if it has been deleted.
	 */
	if (inode->i_nlink == 0 || !test_cflag(COMMIT_Dirty, inode))
		return 0;

	if (isReadOnly(inode)) {
		/* kernel allows writes to devices on read-only
		 * partitions and may think inode is dirty
		 */
		if (!special_file(inode->i_mode) && noisy) {
<<<<<<< HEAD
			jfs_err("jfs_commit_inode(0x%p) called on "
				   "read-only volume", inode);
=======
			jfs_err("jfs_commit_inode(0x%p) called on read-only volume",
				inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			jfs_err("Is remount racy?");
			noisy--;
		}
		return 0;
	}

	tid = txBegin(inode->i_sb, COMMIT_INODE);
	mutex_lock(&JFS_IP(inode)->commit_mutex);

	/*
	 * Retest inode state after taking commit_mutex
	 */
	if (inode->i_nlink && test_cflag(COMMIT_Dirty, inode))
		rc = txCommit(tid, 1, &inode, wait ? COMMIT_SYNC : 0);

	txEnd(tid);
	mutex_unlock(&JFS_IP(inode)->commit_mutex);
	return rc;
}

int jfs_write_inode(struct inode *inode, struct writeback_control *wbc)
{
	int wait = wbc->sync_mode == WB_SYNC_ALL;

	if (inode->i_nlink == 0)
		return 0;
	/*
	 * If COMMIT_DIRTY is not set, the inode isn't really dirty.
	 * It has been committed since the last change, but was still
	 * on the dirty inode list.
	 */
<<<<<<< HEAD
	 if (!test_cflag(COMMIT_Dirty, inode)) {
		/* Make sure committed changes hit the disk */
		jfs_flush_journal(JFS_SBI(inode->i_sb)->log, wait);
		return 0;
	 }
=======
	if (!test_cflag(COMMIT_Dirty, inode)) {
		/* Make sure committed changes hit the disk */
		jfs_flush_journal(JFS_SBI(inode->i_sb)->log, wait);
		return 0;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (jfs_commit_inode(inode, wait)) {
		jfs_err("jfs_write_inode: jfs_commit_inode failed!");
		return -EIO;
	} else
		return 0;
}

void jfs_evict_inode(struct inode *inode)
{
<<<<<<< HEAD
=======
	struct jfs_inode_info *ji = JFS_IP(inode);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	jfs_info("In jfs_evict_inode, inode = 0x%p", inode);

	if (!inode->i_nlink && !is_bad_inode(inode)) {
		dquot_initialize(inode);

		if (JFS_IP(inode)->fileset == FILESYSTEM_I) {
<<<<<<< HEAD
			truncate_inode_pages(&inode->i_data, 0);
=======
			struct inode *ipimap = JFS_SBI(inode->i_sb)->ipimap;
			truncate_inode_pages_final(&inode->i_data);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

			if (test_cflag(COMMIT_Freewmap, inode))
				jfs_free_zero_link(inode);

<<<<<<< HEAD
			diFree(inode);
=======
			if (ipimap && JFS_IP(ipimap)->i_imap)
				diFree(inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

			/*
			 * Free the inode from the quota allocation.
			 */
<<<<<<< HEAD
			dquot_initialize(inode);
			dquot_free_inode(inode);
		}
	} else {
		truncate_inode_pages(&inode->i_data, 0);
	}
	end_writeback(inode);
	dquot_drop(inode);
=======
			dquot_free_inode(inode);
		}
	} else {
		truncate_inode_pages_final(&inode->i_data);
	}
	clear_inode(inode);
	dquot_drop(inode);

	BUG_ON(!list_empty(&ji->anon_inode_list));

	spin_lock_irq(&ji->ag_lock);
	if (ji->active_ag != -1) {
		struct bmap *bmap = JFS_SBI(inode->i_sb)->bmap;
		atomic_dec(&bmap->db_active[ji->active_ag]);
		ji->active_ag = -1;
	}
	spin_unlock_irq(&ji->ag_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void jfs_dirty_inode(struct inode *inode, int flags)
{
	static int noisy = 5;

	if (isReadOnly(inode)) {
		if (!special_file(inode->i_mode) && noisy) {
			/* kernel allows writes to devices on read-only
			 * partitions and may try to mark inode dirty
			 */
			jfs_err("jfs_dirty_inode called on read-only volume");
			jfs_err("Is remount racy?");
			noisy--;
		}
		return;
	}

	set_cflag(COMMIT_Dirty, inode);
}

int jfs_get_block(struct inode *ip, sector_t lblock,
		  struct buffer_head *bh_result, int create)
{
	s64 lblock64 = lblock;
	int rc = 0;
	xad_t xad;
	s64 xaddr;
	int xflag;
	s32 xlen = bh_result->b_size >> ip->i_blkbits;

	/*
	 * Take appropriate lock on inode
	 */
	if (create)
		IWRITE_LOCK(ip, RDWRLOCK_NORMAL);
	else
		IREAD_LOCK(ip, RDWRLOCK_NORMAL);

	if (((lblock64 << ip->i_sb->s_blocksize_bits) < ip->i_size) &&
	    (!xtLookup(ip, lblock64, xlen, &xflag, &xaddr, &xlen, 0)) &&
	    xaddr) {
		if (xflag & XAD_NOTRECORDED) {
			if (!create)
				/*
				 * Allocated but not recorded, read treats
				 * this as a hole
				 */
				goto unlock;
<<<<<<< HEAD
#ifdef _JFS_4K
			XADoffset(&xad, lblock64);
			XADlength(&xad, xlen);
			XADaddress(&xad, xaddr);
#else				/* _JFS_4K */
			/*
			 * As long as block size = 4K, this isn't a problem.
			 * We should mark the whole page not ABNR, but how
			 * will we know to mark the other blocks BH_New?
			 */
			BUG();
#endif				/* _JFS_4K */
=======
			XADoffset(&xad, lblock64);
			XADlength(&xad, xlen);
			XADaddress(&xad, xaddr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			rc = extRecord(ip, &xad);
			if (rc)
				goto unlock;
			set_buffer_new(bh_result);
		}

		map_bh(bh_result, ip->i_sb, xaddr);
		bh_result->b_size = xlen << ip->i_blkbits;
		goto unlock;
	}
	if (!create)
		goto unlock;

	/*
	 * Allocate a new block
	 */
<<<<<<< HEAD
#ifdef _JFS_4K
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if ((rc = extHint(ip, lblock64 << ip->i_sb->s_blocksize_bits, &xad)))
		goto unlock;
	rc = extAlloc(ip, xlen, lblock64, &xad, false);
	if (rc)
		goto unlock;

	set_buffer_new(bh_result);
	map_bh(bh_result, ip->i_sb, addressXAD(&xad));
	bh_result->b_size = lengthXAD(&xad) << ip->i_blkbits;

<<<<<<< HEAD
#else				/* _JFS_4K */
	/*
	 * We need to do whatever it takes to keep all but the last buffers
	 * in 4K pages - see jfs_write.c
	 */
	BUG();
#endif				/* _JFS_4K */

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
      unlock:
	/*
	 * Release lock on inode
	 */
	if (create)
		IWRITE_UNLOCK(ip);
	else
		IREAD_UNLOCK(ip);
	return rc;
}

<<<<<<< HEAD
static int jfs_writepage(struct page *page, struct writeback_control *wbc)
{
	return block_write_full_page(page, jfs_get_block, wbc);
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int jfs_writepages(struct address_space *mapping,
			struct writeback_control *wbc)
{
	return mpage_writepages(mapping, wbc, jfs_get_block);
}

<<<<<<< HEAD
static int jfs_readpage(struct file *file, struct page *page)
{
	return mpage_readpage(page, jfs_get_block);
}

static int jfs_readpages(struct file *file, struct address_space *mapping,
		struct list_head *pages, unsigned nr_pages)
{
	return mpage_readpages(mapping, pages, nr_pages, jfs_get_block);
}

static int jfs_write_begin(struct file *file, struct address_space *mapping,
				loff_t pos, unsigned len, unsigned flags,
=======
static int jfs_read_folio(struct file *file, struct folio *folio)
{
	return mpage_read_folio(folio, jfs_get_block);
}

static void jfs_readahead(struct readahead_control *rac)
{
	mpage_readahead(rac, jfs_get_block);
}

static void jfs_write_failed(struct address_space *mapping, loff_t to)
{
	struct inode *inode = mapping->host;

	if (to > inode->i_size) {
		truncate_pagecache(inode, inode->i_size);
		jfs_truncate(inode);
	}
}

static int jfs_write_begin(struct file *file, struct address_space *mapping,
				loff_t pos, unsigned len,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				struct page **pagep, void **fsdata)
{
	int ret;

<<<<<<< HEAD
	ret = nobh_write_begin(mapping, pos, len, flags, pagep, fsdata,
				jfs_get_block);
	if (unlikely(ret)) {
		loff_t isize = mapping->host->i_size;
		if (pos + len > isize)
			vmtruncate(mapping->host, isize);
	}
=======
	ret = block_write_begin(mapping, pos, len, pagep, jfs_get_block);
	if (unlikely(ret))
		jfs_write_failed(mapping, pos + len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ret;
}

<<<<<<< HEAD
=======
static int jfs_write_end(struct file *file, struct address_space *mapping,
		loff_t pos, unsigned len, unsigned copied, struct page *page,
		void *fsdata)
{
	int ret;

	ret = generic_write_end(file, mapping, pos, len, copied, page, fsdata);
	if (ret < len)
		jfs_write_failed(mapping, pos + len);
	return ret;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static sector_t jfs_bmap(struct address_space *mapping, sector_t block)
{
	return generic_block_bmap(mapping, block, jfs_get_block);
}

<<<<<<< HEAD
static ssize_t jfs_direct_IO(int rw, struct kiocb *iocb,
	const struct iovec *iov, loff_t offset, unsigned long nr_segs)
{
	struct file *file = iocb->ki_filp;
	struct inode *inode = file->f_mapping->host;
	ssize_t ret;

	ret = blockdev_direct_IO(rw, iocb, inode, iov, offset, nr_segs,
				 jfs_get_block);
=======
static ssize_t jfs_direct_IO(struct kiocb *iocb, struct iov_iter *iter)
{
	struct file *file = iocb->ki_filp;
	struct address_space *mapping = file->f_mapping;
	struct inode *inode = file->f_mapping->host;
	size_t count = iov_iter_count(iter);
	ssize_t ret;

	ret = blockdev_direct_IO(iocb, inode, iter, jfs_get_block);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * In case of error extending write may have instantiated a few
	 * blocks outside i_size. Trim these off again.
	 */
<<<<<<< HEAD
	if (unlikely((rw & WRITE) && ret < 0)) {
		loff_t isize = i_size_read(inode);
		loff_t end = offset + iov_length(iov, nr_segs);

		if (end > isize)
			vmtruncate(inode, isize);
=======
	if (unlikely(iov_iter_rw(iter) == WRITE && ret < 0)) {
		loff_t isize = i_size_read(inode);
		loff_t end = iocb->ki_pos + count;

		if (end > isize)
			jfs_write_failed(mapping, end);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return ret;
}

const struct address_space_operations jfs_aops = {
<<<<<<< HEAD
	.readpage	= jfs_readpage,
	.readpages	= jfs_readpages,
	.writepage	= jfs_writepage,
	.writepages	= jfs_writepages,
	.write_begin	= jfs_write_begin,
	.write_end	= nobh_write_end,
	.bmap		= jfs_bmap,
	.direct_IO	= jfs_direct_IO,
=======
	.dirty_folio	= block_dirty_folio,
	.invalidate_folio = block_invalidate_folio,
	.read_folio	= jfs_read_folio,
	.readahead	= jfs_readahead,
	.writepages	= jfs_writepages,
	.write_begin	= jfs_write_begin,
	.write_end	= jfs_write_end,
	.bmap		= jfs_bmap,
	.direct_IO	= jfs_direct_IO,
	.migrate_folio	= buffer_migrate_folio,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/*
 * Guts of jfs_truncate.  Called with locks already held.  Can be called
 * with directory for truncating directory index table.
 */
void jfs_truncate_nolock(struct inode *ip, loff_t length)
{
	loff_t newsize;
	tid_t tid;

	ASSERT(length >= 0);

	if (test_cflag(COMMIT_Nolink, ip)) {
		xtTruncate(0, ip, length, COMMIT_WMAP);
		return;
	}

	do {
		tid = txBegin(ip->i_sb, 0);

		/*
		 * The commit_mutex cannot be taken before txBegin.
		 * txBegin may block and there is a chance the inode
		 * could be marked dirty and need to be committed
		 * before txBegin unblocks
		 */
		mutex_lock(&JFS_IP(ip)->commit_mutex);

		newsize = xtTruncate(tid, ip, length,
				     COMMIT_TRUNCATE | COMMIT_PWMAP);
		if (newsize < 0) {
			txEnd(tid);
			mutex_unlock(&JFS_IP(ip)->commit_mutex);
			break;
		}

<<<<<<< HEAD
		ip->i_mtime = ip->i_ctime = CURRENT_TIME;
=======
		inode_set_mtime_to_ts(ip, inode_set_ctime_current(ip));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		mark_inode_dirty(ip);

		txCommit(tid, 1, &ip, 0);
		txEnd(tid);
		mutex_unlock(&JFS_IP(ip)->commit_mutex);
	} while (newsize > length);	/* Truncate isn't always atomic */
}

void jfs_truncate(struct inode *ip)
{
	jfs_info("jfs_truncate: size = 0x%lx", (ulong) ip->i_size);

<<<<<<< HEAD
	nobh_truncate_page(ip->i_mapping, ip->i_size, jfs_get_block);
=======
	block_truncate_page(ip->i_mapping, ip->i_size, jfs_get_block);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	IWRITE_LOCK(ip, RDWRLOCK_NORMAL);
	jfs_truncate_nolock(ip, ip->i_size);
	IWRITE_UNLOCK(ip);
}
