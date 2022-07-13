<<<<<<< HEAD
/*
 * Copyright (C) Sistina Software, Inc.  1997-2003 All rights reserved.
 * Copyright (C) 2004-2008 Red Hat, Inc.  All rights reserved.
 *
 * This copyrighted material is made available to anyone wishing to use,
 * modify, copy, or redistribute it subject to the terms and conditions
 * of the GNU General Public License version 2.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) Sistina Software, Inc.  1997-2003 All rights reserved.
 * Copyright (C) 2004-2008 Red Hat, Inc.  All rights reserved.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/completion.h>
#include <linux/buffer_head.h>
#include <linux/pagemap.h>
#include <linux/pagevec.h>
#include <linux/mpage.h>
#include <linux/fs.h>
#include <linux/writeback.h>
#include <linux/swap.h>
#include <linux/gfs2_ondisk.h>
#include <linux/backing-dev.h>
<<<<<<< HEAD
=======
#include <linux/uio.h>
#include <trace/events/writeback.h>
#include <linux/sched/signal.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include "gfs2.h"
#include "incore.h"
#include "bmap.h"
#include "glock.h"
#include "inode.h"
#include "log.h"
#include "meta_io.h"
#include "quota.h"
#include "trans.h"
#include "rgrp.h"
#include "super.h"
#include "util.h"
#include "glops.h"
<<<<<<< HEAD


void gfs2_page_add_databufs(struct gfs2_inode *ip, struct page *page,
			    unsigned int from, unsigned int to)
{
	struct buffer_head *head = page_buffers(page);
	unsigned int bsize = head->b_size;
	struct buffer_head *bh;
	unsigned int start, end;
=======
#include "aops.h"


void gfs2_trans_add_databufs(struct gfs2_inode *ip, struct folio *folio,
			     size_t from, size_t len)
{
	struct buffer_head *head = folio_buffers(folio);
	unsigned int bsize = head->b_size;
	struct buffer_head *bh;
	size_t to = from + len;
	size_t start, end;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	for (bh = head, start = 0; bh != head || !start;
	     bh = bh->b_this_page, start = end) {
		end = start + bsize;
<<<<<<< HEAD
		if (end <= from || start >= to)
			continue;
		if (gfs2_is_jdata(ip))
			set_buffer_uptodate(bh);
		gfs2_trans_add_bh(ip->i_gl, bh, 0);
=======
		if (end <= from)
			continue;
		if (start >= to)
			break;
		set_buffer_uptodate(bh);
		gfs2_trans_add_data(ip->i_gl, bh);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

/**
 * gfs2_get_block_noalloc - Fills in a buffer head with details about a block
 * @inode: The inode
 * @lblock: The block number to look up
 * @bh_result: The buffer head to return the result in
 * @create: Non-zero if we may add block to the file
 *
 * Returns: errno
 */

static int gfs2_get_block_noalloc(struct inode *inode, sector_t lblock,
				  struct buffer_head *bh_result, int create)
{
	int error;

	error = gfs2_block_map(inode, lblock, bh_result, 0);
	if (error)
		return error;
	if (!buffer_mapped(bh_result))
<<<<<<< HEAD
		return -EIO;
	return 0;
}

static int gfs2_get_block_direct(struct inode *inode, sector_t lblock,
				 struct buffer_head *bh_result, int create)
{
	return gfs2_block_map(inode, lblock, bh_result, 0);
}

/**
 * gfs2_writepage_common - Common bits of writepage
 * @page: The page to be written
 * @wbc: The writeback control
 *
 * Returns: 1 if writepage is ok, otherwise an error code or zero if no error.
 */

static int gfs2_writepage_common(struct page *page,
				 struct writeback_control *wbc)
{
	struct inode *inode = page->mapping->host;
	struct gfs2_inode *ip = GFS2_I(inode);
	struct gfs2_sbd *sdp = GFS2_SB(inode);
	loff_t i_size = i_size_read(inode);
	pgoff_t end_index = i_size >> PAGE_CACHE_SHIFT;
	unsigned offset;

	if (gfs2_assert_withdraw(sdp, gfs2_glock_is_held_excl(ip->i_gl)))
		goto out;
	if (current->journal_info)
		goto redirty;
	/* Is the page fully outside i_size? (truncate in progress) */
	offset = i_size & (PAGE_CACHE_SIZE-1);
	if (page->index > end_index || (page->index == end_index && !offset)) {
		page->mapping->a_ops->invalidatepage(page, 0);
		goto out;
	}
	return 1;
redirty:
	redirty_page_for_writepage(wbc, page);
out:
	unlock_page(page);
	return 0;
}

/**
 * gfs2_writeback_writepage - Write page for writeback mappings
 * @page: The page
 * @wbc: The writeback control
 *
 */

static int gfs2_writeback_writepage(struct page *page,
				    struct writeback_control *wbc)
{
	int ret;

	ret = gfs2_writepage_common(page, wbc);
	if (ret <= 0)
		return ret;

	return nobh_writepage(page, gfs2_get_block_noalloc, wbc);
}

/**
 * gfs2_ordered_writepage - Write page for ordered data files
 * @page: The page to write
 * @wbc: The writeback control
 *
 */

static int gfs2_ordered_writepage(struct page *page,
				  struct writeback_control *wbc)
{
	struct inode *inode = page->mapping->host;
	struct gfs2_inode *ip = GFS2_I(inode);
	int ret;

	ret = gfs2_writepage_common(page, wbc);
	if (ret <= 0)
		return ret;

	if (!page_has_buffers(page)) {
		create_empty_buffers(page, inode->i_sb->s_blocksize,
				     (1 << BH_Dirty)|(1 << BH_Uptodate));
	}
	gfs2_page_add_databufs(ip, page, 0, inode->i_sb->s_blocksize-1);
	return block_write_full_page(page, gfs2_get_block_noalloc, wbc);
}

/**
 * __gfs2_jdata_writepage - The core of jdata writepage
 * @page: The page to write
=======
		return -ENODATA;
	return 0;
}

/**
 * gfs2_write_jdata_folio - gfs2 jdata-specific version of block_write_full_folio
 * @folio: The folio to write
 * @wbc: The writeback control
 *
 * This is the same as calling block_write_full_folio, but it also
 * writes pages outside of i_size
 */
static int gfs2_write_jdata_folio(struct folio *folio,
				 struct writeback_control *wbc)
{
	struct inode * const inode = folio->mapping->host;
	loff_t i_size = i_size_read(inode);

	/*
	 * The folio straddles i_size.  It must be zeroed out on each and every
	 * writepage invocation because it may be mmapped.  "A file is mapped
	 * in multiples of the page size.  For a file that is not a multiple of
	 * the page size, the remaining memory is zeroed when mapped, and
	 * writes to that region are not written out to the file."
	 */
	if (folio_pos(folio) < i_size &&
	    i_size < folio_pos(folio) + folio_size(folio))
		folio_zero_segment(folio, offset_in_folio(folio, i_size),
				folio_size(folio));

	return __block_write_full_folio(inode, folio, gfs2_get_block_noalloc,
			wbc);
}

/**
 * __gfs2_jdata_write_folio - The core of jdata writepage
 * @folio: The folio to write
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @wbc: The writeback control
 *
 * This is shared between writepage and writepages and implements the
 * core of the writepage operation. If a transaction is required then
<<<<<<< HEAD
 * PageChecked will have been set and the transaction will have
 * already been started before this is called.
 */

static int __gfs2_jdata_writepage(struct page *page, struct writeback_control *wbc)
{
	struct inode *inode = page->mapping->host;
	struct gfs2_inode *ip = GFS2_I(inode);
	struct gfs2_sbd *sdp = GFS2_SB(inode);

	if (PageChecked(page)) {
		ClearPageChecked(page);
		if (!page_has_buffers(page)) {
			create_empty_buffers(page, inode->i_sb->s_blocksize,
					     (1 << BH_Dirty)|(1 << BH_Uptodate));
		}
		gfs2_page_add_databufs(ip, page, 0, sdp->sd_vfs->s_blocksize-1);
	}
	return block_write_full_page(page, gfs2_get_block_noalloc, wbc);
=======
 * the checked flag will have been set and the transaction will have
 * already been started before this is called.
 */
static int __gfs2_jdata_write_folio(struct folio *folio,
		struct writeback_control *wbc)
{
	struct inode *inode = folio->mapping->host;
	struct gfs2_inode *ip = GFS2_I(inode);

	if (folio_test_checked(folio)) {
		folio_clear_checked(folio);
		if (!folio_buffers(folio)) {
			create_empty_buffers(folio,
					inode->i_sb->s_blocksize,
					BIT(BH_Dirty)|BIT(BH_Uptodate));
		}
		gfs2_trans_add_databufs(ip, folio, 0, folio_size(folio));
	}
	return gfs2_write_jdata_folio(folio, wbc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * gfs2_jdata_writepage - Write complete page
 * @page: Page to write
<<<<<<< HEAD
=======
 * @wbc: The writeback control
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Returns: errno
 *
 */

static int gfs2_jdata_writepage(struct page *page, struct writeback_control *wbc)
{
<<<<<<< HEAD
	struct inode *inode = page->mapping->host;
	struct gfs2_sbd *sdp = GFS2_SB(inode);
	int ret;
	int done_trans = 0;

	if (PageChecked(page)) {
		if (wbc->sync_mode != WB_SYNC_ALL)
			goto out_ignore;
		ret = gfs2_trans_begin(sdp, RES_DINODE + 1, 0);
		if (ret)
			goto out_ignore;
		done_trans = 1;
	}
	ret = gfs2_writepage_common(page, wbc);
	if (ret > 0)
		ret = __gfs2_jdata_writepage(page, wbc);
	if (done_trans)
		gfs2_trans_end(sdp);
	return ret;

out_ignore:
	redirty_page_for_writepage(wbc, page);
	unlock_page(page);
=======
	struct folio *folio = page_folio(page);
	struct inode *inode = page->mapping->host;
	struct gfs2_inode *ip = GFS2_I(inode);
	struct gfs2_sbd *sdp = GFS2_SB(inode);

	if (gfs2_assert_withdraw(sdp, ip->i_gl->gl_state == LM_ST_EXCLUSIVE))
		goto out;
	if (folio_test_checked(folio) || current->journal_info)
		goto out_ignore;
	return __gfs2_jdata_write_folio(folio, wbc);

out_ignore:
	folio_redirty_for_writepage(wbc, folio);
out:
	folio_unlock(folio);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

/**
<<<<<<< HEAD
 * gfs2_writeback_writepages - Write a bunch of dirty pages back to disk
 * @mapping: The mapping to write
 * @wbc: Write-back control
 *
 * For the data=writeback case we can already ignore buffer heads
 * and write whole extents at once. This is a big reduction in the
 * number of I/O requests we send and the bmap calls we make in this case.
 */
static int gfs2_writeback_writepages(struct address_space *mapping,
				     struct writeback_control *wbc)
{
	return mpage_writepages(mapping, wbc, gfs2_get_block_noalloc);
}

/**
 * gfs2_write_jdata_pagevec - Write back a pagevec's worth of pages
 * @mapping: The mapping
 * @wbc: The writeback control
 * @writepage: The writepage function to call for each page
 * @pvec: The vector of pages
 * @nr_pages: The number of pages to write
=======
 * gfs2_writepages - Write a bunch of dirty pages back to disk
 * @mapping: The mapping to write
 * @wbc: Write-back control
 *
 * Used for both ordered and writeback modes.
 */
static int gfs2_writepages(struct address_space *mapping,
			   struct writeback_control *wbc)
{
	struct gfs2_sbd *sdp = gfs2_mapping2sbd(mapping);
	struct iomap_writepage_ctx wpc = { };
	int ret;

	/*
	 * Even if we didn't write enough pages here, we might still be holding
	 * dirty pages in the ail. We forcibly flush the ail because we don't
	 * want balance_dirty_pages() to loop indefinitely trying to write out
	 * pages held in the ail that it can't find.
	 */
	ret = iomap_writepages(mapping, wbc, &wpc, &gfs2_writeback_ops);
	if (ret == 0 && wbc->nr_to_write > 0)
		set_bit(SDF_FORCE_AIL_FLUSH, &sdp->sd_flags);
	return ret;
}

/**
 * gfs2_write_jdata_batch - Write back a folio batch's worth of folios
 * @mapping: The mapping
 * @wbc: The writeback control
 * @fbatch: The batch of folios
 * @done_index: Page index
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Returns: non-zero if loop should terminate, zero otherwise
 */

<<<<<<< HEAD
static int gfs2_write_jdata_pagevec(struct address_space *mapping,
				    struct writeback_control *wbc,
				    struct pagevec *pvec,
				    int nr_pages, pgoff_t end)
{
	struct inode *inode = mapping->host;
	struct gfs2_sbd *sdp = GFS2_SB(inode);
	loff_t i_size = i_size_read(inode);
	pgoff_t end_index = i_size >> PAGE_CACHE_SHIFT;
	unsigned offset = i_size & (PAGE_CACHE_SIZE-1);
	unsigned nrblocks = nr_pages * (PAGE_CACHE_SIZE/inode->i_sb->s_blocksize);
	int i;
	int ret;
=======
static int gfs2_write_jdata_batch(struct address_space *mapping,
				    struct writeback_control *wbc,
				    struct folio_batch *fbatch,
				    pgoff_t *done_index)
{
	struct inode *inode = mapping->host;
	struct gfs2_sbd *sdp = GFS2_SB(inode);
	unsigned nrblocks;
	int i;
	int ret;
	size_t size = 0;
	int nr_folios = folio_batch_count(fbatch);

	for (i = 0; i < nr_folios; i++)
		size += folio_size(fbatch->folios[i]);
	nrblocks = size >> inode->i_blkbits;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ret = gfs2_trans_begin(sdp, nrblocks, nrblocks);
	if (ret < 0)
		return ret;

<<<<<<< HEAD
	for(i = 0; i < nr_pages; i++) {
		struct page *page = pvec->pages[i];

		lock_page(page);

		if (unlikely(page->mapping != mapping)) {
			unlock_page(page);
			continue;
		}

		if (!wbc->range_cyclic && page->index > end) {
			ret = 1;
			unlock_page(page);
			continue;
		}

		if (wbc->sync_mode != WB_SYNC_NONE)
			wait_on_page_writeback(page);

		if (PageWriteback(page) ||
		    !clear_page_dirty_for_io(page)) {
			unlock_page(page);
			continue;
		}

		/* Is the page fully outside i_size? (truncate in progress) */
		if (page->index > end_index || (page->index == end_index && !offset)) {
			page->mapping->a_ops->invalidatepage(page, 0);
			unlock_page(page);
			continue;
		}

		ret = __gfs2_jdata_writepage(page, wbc);

		if (ret || (--(wbc->nr_to_write) <= 0))
			ret = 1;
=======
	for (i = 0; i < nr_folios; i++) {
		struct folio *folio = fbatch->folios[i];

		*done_index = folio->index;

		folio_lock(folio);

		if (unlikely(folio->mapping != mapping)) {
continue_unlock:
			folio_unlock(folio);
			continue;
		}

		if (!folio_test_dirty(folio)) {
			/* someone wrote it for us */
			goto continue_unlock;
		}

		if (folio_test_writeback(folio)) {
			if (wbc->sync_mode != WB_SYNC_NONE)
				folio_wait_writeback(folio);
			else
				goto continue_unlock;
		}

		BUG_ON(folio_test_writeback(folio));
		if (!folio_clear_dirty_for_io(folio))
			goto continue_unlock;

		trace_wbc_writepage(wbc, inode_to_bdi(inode));

		ret = __gfs2_jdata_write_folio(folio, wbc);
		if (unlikely(ret)) {
			if (ret == AOP_WRITEPAGE_ACTIVATE) {
				folio_unlock(folio);
				ret = 0;
			} else {

				/*
				 * done_index is set past this page,
				 * so media errors will not choke
				 * background writeout for the entire
				 * file. This has consequences for
				 * range_cyclic semantics (ie. it may
				 * not be suitable for data integrity
				 * writeout).
				 */
				*done_index = folio_next_index(folio);
				ret = 1;
				break;
			}
		}

		/*
		 * We stop writing back only if we are not doing
		 * integrity sync. In case of integrity sync we have to
		 * keep going until we have written all the pages
		 * we tagged for writeback prior to entering this loop.
		 */
		if (--wbc->nr_to_write <= 0 && wbc->sync_mode == WB_SYNC_NONE) {
			ret = 1;
			break;
		}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	gfs2_trans_end(sdp);
	return ret;
}

/**
 * gfs2_write_cache_jdata - Like write_cache_pages but different
 * @mapping: The mapping to write
 * @wbc: The writeback control
<<<<<<< HEAD
 * @writepage: The writepage function to call
 * @data: The data to pass to writepage
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * The reason that we use our own function here is that we need to
 * start transactions before we grab page locks. This allows us
 * to get the ordering right.
 */

static int gfs2_write_cache_jdata(struct address_space *mapping,
				  struct writeback_control *wbc)
{
	int ret = 0;
	int done = 0;
<<<<<<< HEAD
	struct pagevec pvec;
	int nr_pages;
	pgoff_t index;
	pgoff_t end;
	int scanned = 0;
	int range_whole = 0;

	pagevec_init(&pvec, 0);
	if (wbc->range_cyclic) {
		index = mapping->writeback_index; /* Start from prev offset */
		end = -1;
	} else {
		index = wbc->range_start >> PAGE_CACHE_SHIFT;
		end = wbc->range_end >> PAGE_CACHE_SHIFT;
		if (wbc->range_start == 0 && wbc->range_end == LLONG_MAX)
			range_whole = 1;
		scanned = 1;
	}

retry:
	 while (!done && (index <= end) &&
		(nr_pages = pagevec_lookup_tag(&pvec, mapping, &index,
					       PAGECACHE_TAG_DIRTY,
					       min(end - index, (pgoff_t)PAGEVEC_SIZE-1) + 1))) {
		scanned = 1;
		ret = gfs2_write_jdata_pagevec(mapping, wbc, &pvec, nr_pages, end);
=======
	struct folio_batch fbatch;
	int nr_folios;
	pgoff_t writeback_index;
	pgoff_t index;
	pgoff_t end;
	pgoff_t done_index;
	int cycled;
	int range_whole = 0;
	xa_mark_t tag;

	folio_batch_init(&fbatch);
	if (wbc->range_cyclic) {
		writeback_index = mapping->writeback_index; /* prev offset */
		index = writeback_index;
		if (index == 0)
			cycled = 1;
		else
			cycled = 0;
		end = -1;
	} else {
		index = wbc->range_start >> PAGE_SHIFT;
		end = wbc->range_end >> PAGE_SHIFT;
		if (wbc->range_start == 0 && wbc->range_end == LLONG_MAX)
			range_whole = 1;
		cycled = 1; /* ignore range_cyclic tests */
	}
	if (wbc->sync_mode == WB_SYNC_ALL || wbc->tagged_writepages)
		tag = PAGECACHE_TAG_TOWRITE;
	else
		tag = PAGECACHE_TAG_DIRTY;

retry:
	if (wbc->sync_mode == WB_SYNC_ALL || wbc->tagged_writepages)
		tag_pages_for_writeback(mapping, index, end);
	done_index = index;
	while (!done && (index <= end)) {
		nr_folios = filemap_get_folios_tag(mapping, &index, end,
				tag, &fbatch);
		if (nr_folios == 0)
			break;

		ret = gfs2_write_jdata_batch(mapping, wbc, &fbatch,
				&done_index);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (ret)
			done = 1;
		if (ret > 0)
			ret = 0;
<<<<<<< HEAD

		pagevec_release(&pvec);
		cond_resched();
	}

	if (!scanned && !done) {
		/*
		 * We hit the last page and there is more work to be done: wrap
		 * back to the start of the file
		 */
		scanned = 1;
		index = 0;
=======
		folio_batch_release(&fbatch);
		cond_resched();
	}

	if (!cycled && !done) {
		/*
		 * range_cyclic:
		 * We hit the last page and there is more work to be done: wrap
		 * back to the start of the file
		 */
		cycled = 1;
		index = 0;
		end = writeback_index - 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto retry;
	}

	if (wbc->range_cyclic || (range_whole && wbc->nr_to_write > 0))
<<<<<<< HEAD
		mapping->writeback_index = index;
=======
		mapping->writeback_index = done_index;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}


/**
 * gfs2_jdata_writepages - Write a bunch of dirty pages back to disk
 * @mapping: The mapping to write
 * @wbc: The writeback control
 * 
 */

static int gfs2_jdata_writepages(struct address_space *mapping,
				 struct writeback_control *wbc)
{
	struct gfs2_inode *ip = GFS2_I(mapping->host);
	struct gfs2_sbd *sdp = GFS2_SB(mapping->host);
	int ret;

	ret = gfs2_write_cache_jdata(mapping, wbc);
	if (ret == 0 && wbc->sync_mode == WB_SYNC_ALL) {
<<<<<<< HEAD
		gfs2_log_flush(sdp, ip->i_gl);
=======
		gfs2_log_flush(sdp, ip->i_gl, GFS2_LOG_HEAD_FLUSH_NORMAL |
			       GFS2_LFC_JDATA_WPAGES);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = gfs2_write_cache_jdata(mapping, wbc);
	}
	return ret;
}

/**
<<<<<<< HEAD
 * stuffed_readpage - Fill in a Linux page with stuffed file data
 * @ip: the inode
 * @page: the page
 *
 * Returns: errno
 */

static int stuffed_readpage(struct gfs2_inode *ip, struct page *page)
{
	struct buffer_head *dibh;
	u64 dsize = i_size_read(&ip->i_inode);
	void *kaddr;
	int error;

	/*
	 * Due to the order of unstuffing files and ->fault(), we can be
	 * asked for a zero page in the case of a stuffed file being extended,
	 * so we need to supply one here. It doesn't happen often.
	 */
	if (unlikely(page->index)) {
		zero_user(page, 0, PAGE_CACHE_SIZE);
		SetPageUptodate(page);
		return 0;
	}

	error = gfs2_meta_inode_buffer(ip, &dibh);
	if (error)
		return error;

	kaddr = kmap_atomic(page);
	if (dsize > (dibh->b_size - sizeof(struct gfs2_dinode)))
		dsize = (dibh->b_size - sizeof(struct gfs2_dinode));
	memcpy(kaddr, dibh->b_data + sizeof(struct gfs2_dinode), dsize);
	memset(kaddr + dsize, 0, PAGE_CACHE_SIZE - dsize);
	kunmap_atomic(kaddr);
	flush_dcache_page(page);
	brelse(dibh);
	SetPageUptodate(page);

	return 0;
}


/**
 * __gfs2_readpage - readpage
 * @file: The file to read a page for
 * @page: The page to read
 *
 * This is the core of gfs2's readpage. Its used by the internal file
 * reading code as in that case we already hold the glock. Also its
 * called by gfs2_readpage() once the required lock has been granted.
 *
 */

static int __gfs2_readpage(void *file, struct page *page)
{
	struct gfs2_inode *ip = GFS2_I(page->mapping->host);
	struct gfs2_sbd *sdp = GFS2_SB(page->mapping->host);
	int error;

	if (gfs2_is_stuffed(ip)) {
		error = stuffed_readpage(ip, page);
		unlock_page(page);
	} else {
		error = mpage_readpage(page, gfs2_block_map);
	}

	if (unlikely(test_bit(SDF_SHUTDOWN, &sdp->sd_flags)))
=======
 * stuffed_read_folio - Fill in a Linux folio with stuffed file data
 * @ip: the inode
 * @folio: the folio
 *
 * Returns: errno
 */
static int stuffed_read_folio(struct gfs2_inode *ip, struct folio *folio)
{
	struct buffer_head *dibh = NULL;
	size_t dsize = i_size_read(&ip->i_inode);
	void *from = NULL;
	int error = 0;

	/*
	 * Due to the order of unstuffing files and ->fault(), we can be
	 * asked for a zero folio in the case of a stuffed file being extended,
	 * so we need to supply one here. It doesn't happen often.
	 */
	if (unlikely(folio->index)) {
		dsize = 0;
	} else {
		error = gfs2_meta_inode_buffer(ip, &dibh);
		if (error)
			goto out;
		from = dibh->b_data + sizeof(struct gfs2_dinode);
	}

	folio_fill_tail(folio, 0, from, dsize);
	brelse(dibh);
out:
	folio_end_read(folio, error == 0);

	return error;
}

/**
 * gfs2_read_folio - read a folio from a file
 * @file: The file to read
 * @folio: The folio in the file
 */
static int gfs2_read_folio(struct file *file, struct folio *folio)
{
	struct inode *inode = folio->mapping->host;
	struct gfs2_inode *ip = GFS2_I(inode);
	struct gfs2_sbd *sdp = GFS2_SB(inode);
	int error;

	if (!gfs2_is_jdata(ip) ||
	    (i_blocksize(inode) == PAGE_SIZE && !folio_buffers(folio))) {
		error = iomap_read_folio(folio, &gfs2_iomap_ops);
	} else if (gfs2_is_stuffed(ip)) {
		error = stuffed_read_folio(ip, folio);
	} else {
		error = mpage_read_folio(folio, gfs2_block_map);
	}

	if (gfs2_withdrawing_or_withdrawn(sdp))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EIO;

	return error;
}

/**
<<<<<<< HEAD
 * gfs2_readpage - read a page of a file
 * @file: The file to read
 * @page: The page of the file
 *
 * This deals with the locking required. We have to unlock and
 * relock the page in order to get the locking in the right
 * order.
 */

static int gfs2_readpage(struct file *file, struct page *page)
{
	struct address_space *mapping = page->mapping;
	struct gfs2_inode *ip = GFS2_I(mapping->host);
	struct gfs2_holder gh;
	int error;

	unlock_page(page);
	gfs2_holder_init(ip->i_gl, LM_ST_SHARED, 0, &gh);
	error = gfs2_glock_nq(&gh);
	if (unlikely(error))
		goto out;
	error = AOP_TRUNCATED_PAGE;
	lock_page(page);
	if (page->mapping == mapping && !PageUptodate(page))
		error = __gfs2_readpage(file, page);
	else
		unlock_page(page);
	gfs2_glock_dq(&gh);
out:
	gfs2_holder_uninit(&gh);
	if (error && error != AOP_TRUNCATED_PAGE)
		lock_page(page);
	return error;
}

/**
 * gfs2_internal_read - read an internal file
 * @ip: The gfs2 inode
 * @ra_state: The readahead state (or NULL for no readahead)
=======
 * gfs2_internal_read - read an internal file
 * @ip: The gfs2 inode
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @buf: The buffer to fill
 * @pos: The file position
 * @size: The amount to read
 *
 */

<<<<<<< HEAD
int gfs2_internal_read(struct gfs2_inode *ip, struct file_ra_state *ra_state,
                       char *buf, loff_t *pos, unsigned size)
{
	struct address_space *mapping = ip->i_inode.i_mapping;
	unsigned long index = *pos / PAGE_CACHE_SIZE;
	unsigned offset = *pos & (PAGE_CACHE_SIZE - 1);
	unsigned copied = 0;
	unsigned amt;
	struct page *page;
	void *p;

	do {
		amt = size - copied;
		if (offset + size > PAGE_CACHE_SIZE)
			amt = PAGE_CACHE_SIZE - offset;
		page = read_cache_page(mapping, index, __gfs2_readpage, NULL);
		if (IS_ERR(page))
			return PTR_ERR(page);
		p = kmap_atomic(page);
		memcpy(buf + copied, p + offset, amt);
		kunmap_atomic(p);
		mark_page_accessed(page);
		page_cache_release(page);
		copied += amt;
		index++;
		offset = 0;
=======
ssize_t gfs2_internal_read(struct gfs2_inode *ip, char *buf, loff_t *pos,
			   size_t size)
{
	struct address_space *mapping = ip->i_inode.i_mapping;
	unsigned long index = *pos >> PAGE_SHIFT;
	size_t copied = 0;

	do {
		size_t offset, chunk;
		struct folio *folio;

		folio = read_cache_folio(mapping, index, gfs2_read_folio, NULL);
		if (IS_ERR(folio)) {
			if (PTR_ERR(folio) == -EINTR)
				continue;
			return PTR_ERR(folio);
		}
		offset = *pos + copied - folio_pos(folio);
		chunk = min(size - copied, folio_size(folio) - offset);
		memcpy_from_folio(buf + copied, folio, offset, chunk);
		index = folio_next_index(folio);
		folio_put(folio);
		copied += chunk;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} while(copied < size);
	(*pos) += size;
	return size;
}

/**
<<<<<<< HEAD
 * gfs2_readpages - Read a bunch of pages at once
=======
 * gfs2_readahead - Read a bunch of pages at once
 * @rac: Read-ahead control structure
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Some notes:
 * 1. This is only for readahead, so we can simply ignore any things
 *    which are slightly inconvenient (such as locking conflicts between
 *    the page lock and the glock) and return having done no I/O. Its
 *    obviously not something we'd want to do on too regular a basis.
 *    Any I/O we ignore at this time will be done via readpage later.
 * 2. We don't handle stuffed files here we let readpage do the honours.
<<<<<<< HEAD
 * 3. mpage_readpages() does most of the heavy lifting in the common case.
 * 4. gfs2_block_map() is relied upon to set BH_Boundary in the right places.
 */

static int gfs2_readpages(struct file *file, struct address_space *mapping,
			  struct list_head *pages, unsigned nr_pages)
{
	struct inode *inode = mapping->host;
	struct gfs2_inode *ip = GFS2_I(inode);
	struct gfs2_sbd *sdp = GFS2_SB(inode);
	struct gfs2_holder gh;
	int ret;

	gfs2_holder_init(ip->i_gl, LM_ST_SHARED, 0, &gh);
	ret = gfs2_glock_nq(&gh);
	if (unlikely(ret))
		goto out_uninit;
	if (!gfs2_is_stuffed(ip))
		ret = mpage_readpages(mapping, pages, nr_pages, gfs2_block_map);
	gfs2_glock_dq(&gh);
out_uninit:
	gfs2_holder_uninit(&gh);
	if (unlikely(test_bit(SDF_SHUTDOWN, &sdp->sd_flags)))
		ret = -EIO;
	return ret;
}

/**
 * gfs2_write_begin - Begin to write to a file
 * @file: The file to write to
 * @mapping: The mapping in which to write
 * @pos: The file offset at which to start writing
 * @len: Length of the write
 * @flags: Various flags
 * @pagep: Pointer to return the page
 * @fsdata: Pointer to return fs data (unused by GFS2)
 *
 * Returns: errno
 */

static int gfs2_write_begin(struct file *file, struct address_space *mapping,
			    loff_t pos, unsigned len, unsigned flags,
			    struct page **pagep, void **fsdata)
{
	struct gfs2_inode *ip = GFS2_I(mapping->host);
	struct gfs2_sbd *sdp = GFS2_SB(mapping->host);
	struct gfs2_inode *m_ip = GFS2_I(sdp->sd_statfs_inode);
	unsigned int data_blocks = 0, ind_blocks = 0, rblocks;
	int alloc_required;
	int error = 0;
	struct gfs2_qadata *qa = NULL;
	pgoff_t index = pos >> PAGE_CACHE_SHIFT;
	unsigned from = pos & (PAGE_CACHE_SIZE - 1);
	struct page *page;

	gfs2_holder_init(ip->i_gl, LM_ST_EXCLUSIVE, 0, &ip->i_gh);
	error = gfs2_glock_nq(&ip->i_gh);
	if (unlikely(error))
		goto out_uninit;
	if (&ip->i_inode == sdp->sd_rindex) {
		error = gfs2_glock_nq_init(m_ip->i_gl, LM_ST_EXCLUSIVE,
					   GL_NOCACHE, &m_ip->i_gh);
		if (unlikely(error)) {
			gfs2_glock_dq(&ip->i_gh);
			goto out_uninit;
		}
	}

	alloc_required = gfs2_write_alloc_required(ip, pos, len);

	if (alloc_required || gfs2_is_jdata(ip))
		gfs2_write_calc_reserv(ip, len, &data_blocks, &ind_blocks);

	if (alloc_required) {
		qa = gfs2_qadata_get(ip);
		if (!qa) {
			error = -ENOMEM;
			goto out_unlock;
		}

		error = gfs2_quota_lock_check(ip);
		if (error)
			goto out_alloc_put;

		error = gfs2_inplace_reserve(ip, data_blocks + ind_blocks);
		if (error)
			goto out_qunlock;
	}

	rblocks = RES_DINODE + ind_blocks;
	if (gfs2_is_jdata(ip))
		rblocks += data_blocks ? data_blocks : 1;
	if (ind_blocks || data_blocks)
		rblocks += RES_STATFS + RES_QUOTA;
	if (&ip->i_inode == sdp->sd_rindex)
		rblocks += 2 * RES_STATFS;
	if (alloc_required)
		rblocks += gfs2_rg_blocks(ip);

	error = gfs2_trans_begin(sdp, rblocks,
				 PAGE_CACHE_SIZE/sdp->sd_sb.sb_bsize);
	if (error)
		goto out_trans_fail;

	error = -ENOMEM;
	flags |= AOP_FLAG_NOFS;
	page = grab_cache_page_write_begin(mapping, index, flags);
	*pagep = page;
	if (unlikely(!page))
		goto out_endtrans;

	if (gfs2_is_stuffed(ip)) {
		error = 0;
		if (pos + len > sdp->sd_sb.sb_bsize - sizeof(struct gfs2_dinode)) {
			error = gfs2_unstuff_dinode(ip, page);
			if (error == 0)
				goto prepare_write;
		} else if (!PageUptodate(page)) {
			error = stuffed_readpage(ip, page);
		}
		goto out;
	}

prepare_write:
	error = __block_write_begin(page, from, len, gfs2_block_map);
out:
	if (error == 0)
		return 0;

	unlock_page(page);
	page_cache_release(page);

	gfs2_trans_end(sdp);
	if (pos + len > ip->i_inode.i_size)
		gfs2_trim_blocks(&ip->i_inode);
	goto out_trans_fail;

out_endtrans:
	gfs2_trans_end(sdp);
out_trans_fail:
	if (alloc_required) {
		gfs2_inplace_release(ip);
out_qunlock:
		gfs2_quota_unlock(ip);
out_alloc_put:
		gfs2_qadata_put(ip);
	}
out_unlock:
	if (&ip->i_inode == sdp->sd_rindex) {
		gfs2_glock_dq(&m_ip->i_gh);
		gfs2_holder_uninit(&m_ip->i_gh);
	}
	gfs2_glock_dq(&ip->i_gh);
out_uninit:
	gfs2_holder_uninit(&ip->i_gh);
	return error;
=======
 * 3. mpage_readahead() does most of the heavy lifting in the common case.
 * 4. gfs2_block_map() is relied upon to set BH_Boundary in the right places.
 */

static void gfs2_readahead(struct readahead_control *rac)
{
	struct inode *inode = rac->mapping->host;
	struct gfs2_inode *ip = GFS2_I(inode);

	if (gfs2_is_stuffed(ip))
		;
	else if (gfs2_is_jdata(ip))
		mpage_readahead(rac, gfs2_block_map);
	else
		iomap_readahead(rac, &gfs2_iomap_ops);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * adjust_fs_space - Adjusts the free space available due to gfs2_grow
 * @inode: the rindex inode
 */
<<<<<<< HEAD
static void adjust_fs_space(struct inode *inode)
{
	struct gfs2_sbd *sdp = inode->i_sb->s_fs_info;
	struct gfs2_inode *m_ip = GFS2_I(sdp->sd_statfs_inode);
	struct gfs2_inode *l_ip = GFS2_I(sdp->sd_sc_inode);
	struct gfs2_statfs_change_host *m_sc = &sdp->sd_statfs_master;
	struct gfs2_statfs_change_host *l_sc = &sdp->sd_statfs_local;
	struct buffer_head *m_bh, *l_bh;
	u64 fs_total, new_free;

	/* Total up the file system space, according to the latest rindex. */
	fs_total = gfs2_ri_total(sdp);
	if (gfs2_meta_inode_buffer(m_ip, &m_bh) != 0)
		return;
=======
void adjust_fs_space(struct inode *inode)
{
	struct gfs2_sbd *sdp = GFS2_SB(inode);
	struct gfs2_inode *m_ip = GFS2_I(sdp->sd_statfs_inode);
	struct gfs2_statfs_change_host *m_sc = &sdp->sd_statfs_master;
	struct gfs2_statfs_change_host *l_sc = &sdp->sd_statfs_local;
	struct buffer_head *m_bh;
	u64 fs_total, new_free;

	if (gfs2_trans_begin(sdp, 2 * RES_STATFS, 0) != 0)
		return;

	/* Total up the file system space, according to the latest rindex. */
	fs_total = gfs2_ri_total(sdp);
	if (gfs2_meta_inode_buffer(m_ip, &m_bh) != 0)
		goto out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spin_lock(&sdp->sd_statfs_spin);
	gfs2_statfs_change_in(m_sc, m_bh->b_data +
			      sizeof(struct gfs2_dinode));
	if (fs_total > (m_sc->sc_total + l_sc->sc_total))
		new_free = fs_total - (m_sc->sc_total + l_sc->sc_total);
	else
		new_free = 0;
	spin_unlock(&sdp->sd_statfs_spin);
	fs_warn(sdp, "File system extended by %llu blocks.\n",
		(unsigned long long)new_free);
	gfs2_statfs_change(sdp, new_free, new_free, 0);

<<<<<<< HEAD
	if (gfs2_meta_inode_buffer(l_ip, &l_bh) != 0)
		goto out;
	update_statfs(sdp, m_bh, l_bh);
	brelse(l_bh);
out:
	brelse(m_bh);
}

/**
 * gfs2_stuffed_write_end - Write end for stuffed files
 * @inode: The inode
 * @dibh: The buffer_head containing the on-disk inode
 * @pos: The file position
 * @len: The length of the write
 * @copied: How much was actually copied by the VFS
 * @page: The page
 *
 * This copies the data from the page into the inode block after
 * the inode data structure itself.
 *
 * Returns: errno
 */
static int gfs2_stuffed_write_end(struct inode *inode, struct buffer_head *dibh,
				  loff_t pos, unsigned len, unsigned copied,
				  struct page *page)
{
	struct gfs2_inode *ip = GFS2_I(inode);
	struct gfs2_sbd *sdp = GFS2_SB(inode);
	struct gfs2_inode *m_ip = GFS2_I(sdp->sd_statfs_inode);
	u64 to = pos + copied;
	void *kaddr;
	unsigned char *buf = dibh->b_data + sizeof(struct gfs2_dinode);

	BUG_ON((pos + len) > (dibh->b_size - sizeof(struct gfs2_dinode)));
	kaddr = kmap_atomic(page);
	memcpy(buf + pos, kaddr + pos, copied);
	memset(kaddr + pos + copied, 0, len - copied);
	flush_dcache_page(page);
	kunmap_atomic(kaddr);

	if (!PageUptodate(page))
		SetPageUptodate(page);
	unlock_page(page);
	page_cache_release(page);

	if (copied) {
		if (inode->i_size < to)
			i_size_write(inode, to);
		mark_inode_dirty(inode);
	}

	if (inode == sdp->sd_rindex) {
		adjust_fs_space(inode);
		sdp->sd_rindex_uptodate = 0;
	}

	brelse(dibh);
	gfs2_trans_end(sdp);
	if (inode == sdp->sd_rindex) {
		gfs2_glock_dq(&m_ip->i_gh);
		gfs2_holder_uninit(&m_ip->i_gh);
	}
	gfs2_glock_dq(&ip->i_gh);
	gfs2_holder_uninit(&ip->i_gh);
	return copied;
}

/**
 * gfs2_write_end
 * @file: The file to write to
 * @mapping: The address space to write to
 * @pos: The file position
 * @len: The length of the data
 * @copied:
 * @page: The page that has been written
 * @fsdata: The fsdata (unused in GFS2)
 *
 * The main write_end function for GFS2. We have a separate one for
 * stuffed files as they are slightly different, otherwise we just
 * put our locking around the VFS provided functions.
 *
 * Returns: errno
 */

static int gfs2_write_end(struct file *file, struct address_space *mapping,
			  loff_t pos, unsigned len, unsigned copied,
			  struct page *page, void *fsdata)
{
	struct inode *inode = page->mapping->host;
	struct gfs2_inode *ip = GFS2_I(inode);
	struct gfs2_sbd *sdp = GFS2_SB(inode);
	struct gfs2_inode *m_ip = GFS2_I(sdp->sd_statfs_inode);
	struct buffer_head *dibh;
	struct gfs2_qadata *qa = ip->i_qadata;
	unsigned int from = pos & (PAGE_CACHE_SIZE - 1);
	unsigned int to = from + len;
	int ret;

	BUG_ON(gfs2_glock_is_locked_by_me(ip->i_gl) == NULL);

	ret = gfs2_meta_inode_buffer(ip, &dibh);
	if (unlikely(ret)) {
		unlock_page(page);
		page_cache_release(page);
		goto failed;
	}

	gfs2_trans_add_bh(ip->i_gl, dibh, 1);

	if (gfs2_is_stuffed(ip))
		return gfs2_stuffed_write_end(inode, dibh, pos, len, copied, page);

	if (!gfs2_is_writeback(ip))
		gfs2_page_add_databufs(ip, page, from, to);

	ret = generic_write_end(file, mapping, pos, len, copied, page, fsdata);

	if (inode == sdp->sd_rindex) {
		adjust_fs_space(inode);
		sdp->sd_rindex_uptodate = 0;
	}

	brelse(dibh);
failed:
	gfs2_trans_end(sdp);
	if (ip->i_res)
		gfs2_inplace_release(ip);
	if (qa) {
		gfs2_quota_unlock(ip);
		gfs2_qadata_put(ip);
	}
	if (inode == sdp->sd_rindex) {
		gfs2_glock_dq(&m_ip->i_gh);
		gfs2_holder_uninit(&m_ip->i_gh);
	}
	gfs2_glock_dq(&ip->i_gh);
	gfs2_holder_uninit(&ip->i_gh);
	return ret;
}

/**
 * gfs2_set_page_dirty - Page dirtying function
 * @page: The page to dirty
 *
 * Returns: 1 if it dirtyed the page, or 0 otherwise
 */
 
static int gfs2_set_page_dirty(struct page *page)
{
	SetPageChecked(page);
	return __set_page_dirty_buffers(page);
=======
	update_statfs(sdp, m_bh);
	brelse(m_bh);
out:
	sdp->sd_rindex_uptodate = 0;
	gfs2_trans_end(sdp);
}

static bool jdata_dirty_folio(struct address_space *mapping,
		struct folio *folio)
{
	if (current->journal_info)
		folio_set_checked(folio);
	return block_dirty_folio(mapping, folio);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * gfs2_bmap - Block map function
 * @mapping: Address space info
 * @lblock: The block to map
 *
 * Returns: The disk address for the block or 0 on hole or error
 */

static sector_t gfs2_bmap(struct address_space *mapping, sector_t lblock)
{
	struct gfs2_inode *ip = GFS2_I(mapping->host);
	struct gfs2_holder i_gh;
	sector_t dblock = 0;
	int error;

	error = gfs2_glock_nq_init(ip->i_gl, LM_ST_SHARED, LM_FLAG_ANY, &i_gh);
	if (error)
		return 0;

	if (!gfs2_is_stuffed(ip))
<<<<<<< HEAD
		dblock = generic_block_bmap(mapping, lblock, gfs2_block_map);
=======
		dblock = iomap_bmap(mapping, lblock, &gfs2_iomap_ops);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	gfs2_glock_dq_uninit(&i_gh);

	return dblock;
}

static void gfs2_discard(struct gfs2_sbd *sdp, struct buffer_head *bh)
{
	struct gfs2_bufdata *bd;

	lock_buffer(bh);
	gfs2_log_lock(sdp);
	clear_buffer_dirty(bh);
	bd = bh->b_private;
	if (bd) {
<<<<<<< HEAD
		if (!list_empty(&bd->bd_le.le_list) && !buffer_pinned(bh))
			list_del_init(&bd->bd_le.le_list);
		else
			gfs2_remove_from_journal(bh, current->journal_info, 0);
=======
		if (!list_empty(&bd->bd_list) && !buffer_pinned(bh))
			list_del_init(&bd->bd_list);
		else {
			spin_lock(&sdp->sd_ail_lock);
			gfs2_remove_from_journal(bh, REMOVE_JDATA);
			spin_unlock(&sdp->sd_ail_lock);
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	bh->b_bdev = NULL;
	clear_buffer_mapped(bh);
	clear_buffer_req(bh);
	clear_buffer_new(bh);
	gfs2_log_unlock(sdp);
	unlock_buffer(bh);
}

<<<<<<< HEAD
static void gfs2_invalidatepage(struct page *page, unsigned long offset)
{
	struct gfs2_sbd *sdp = GFS2_SB(page->mapping->host);
	struct buffer_head *bh, *head;
	unsigned long pos = 0;

	BUG_ON(!PageLocked(page));
	if (offset == 0)
		ClearPageChecked(page);
	if (!page_has_buffers(page))
		goto out;

	bh = head = page_buffers(page);
	do {
=======
static void gfs2_invalidate_folio(struct folio *folio, size_t offset,
				size_t length)
{
	struct gfs2_sbd *sdp = GFS2_SB(folio->mapping->host);
	size_t stop = offset + length;
	int partial_page = (offset || length < folio_size(folio));
	struct buffer_head *bh, *head;
	unsigned long pos = 0;

	BUG_ON(!folio_test_locked(folio));
	if (!partial_page)
		folio_clear_checked(folio);
	head = folio_buffers(folio);
	if (!head)
		goto out;

	bh = head;
	do {
		if (pos + bh->b_size > stop)
			return;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (offset <= pos)
			gfs2_discard(sdp, bh);
		pos += bh->b_size;
		bh = bh->b_this_page;
	} while (bh != head);
out:
<<<<<<< HEAD
	if (offset == 0)
		try_to_release_page(page, 0);
}

/**
 * gfs2_ok_for_dio - check that dio is valid on this file
 * @ip: The inode
 * @rw: READ or WRITE
 * @offset: The offset at which we are reading or writing
 *
 * Returns: 0 (to ignore the i/o request and thus fall back to buffered i/o)
 *          1 (to accept the i/o request)
 */
static int gfs2_ok_for_dio(struct gfs2_inode *ip, int rw, loff_t offset)
{
	/*
	 * Should we return an error here? I can't see that O_DIRECT for
	 * a stuffed file makes any sense. For now we'll silently fall
	 * back to buffered I/O
	 */
	if (gfs2_is_stuffed(ip))
		return 0;

	if (offset >= i_size_read(&ip->i_inode))
		return 0;
	return 1;
}



static ssize_t gfs2_direct_IO(int rw, struct kiocb *iocb,
			      const struct iovec *iov, loff_t offset,
			      unsigned long nr_segs)
{
	struct file *file = iocb->ki_filp;
	struct inode *inode = file->f_mapping->host;
	struct address_space *mapping = inode->i_mapping;
	struct gfs2_inode *ip = GFS2_I(inode);
	struct gfs2_holder gh;
	int rv;

	/*
	 * Deferred lock, even if its a write, since we do no allocation
	 * on this path. All we need change is atime, and this lock mode
	 * ensures that other nodes have flushed their buffered read caches
	 * (i.e. their page cache entries for this inode). We do not,
	 * unfortunately have the option of only flushing a range like
	 * the VFS does.
	 */
	gfs2_holder_init(ip->i_gl, LM_ST_DEFERRED, 0, &gh);
	rv = gfs2_glock_nq(&gh);
	if (rv)
		return rv;
	rv = gfs2_ok_for_dio(ip, rw, offset);
	if (rv != 1)
		goto out; /* dio not valid, fall back to buffered i/o */

	/*
	 * Now since we are holding a deferred (CW) lock at this point, you
	 * might be wondering why this is ever needed. There is a case however
	 * where we've granted a deferred local lock against a cached exclusive
	 * glock. That is ok provided all granted local locks are deferred, but
	 * it also means that it is possible to encounter pages which are
	 * cached and possibly also mapped. So here we check for that and sort
	 * them out ahead of the dio. The glock state machine will take care of
	 * everything else.
	 *
	 * If in fact the cached glock state (gl->gl_state) is deferred (CW) in
	 * the first place, mapping->nr_pages will always be zero.
	 */
	if (mapping->nrpages) {
		loff_t lstart = offset & (PAGE_CACHE_SIZE - 1);
		loff_t len = iov_length(iov, nr_segs);
		loff_t end = PAGE_ALIGN(offset + len) - 1;

		rv = 0;
		if (len == 0)
			goto out;
		if (test_and_clear_bit(GIF_SW_PAGED, &ip->i_flags))
			unmap_shared_mapping_range(ip->i_inode.i_mapping, offset, len);
		rv = filemap_write_and_wait_range(mapping, lstart, end);
		if (rv)
			return rv;
		truncate_inode_pages_range(mapping, lstart, end);
	}

	rv = __blockdev_direct_IO(rw, iocb, inode, inode->i_sb->s_bdev, iov,
				  offset, nr_segs, gfs2_get_block_direct,
				  NULL, NULL, 0);
out:
	gfs2_glock_dq_m(1, &gh);
	gfs2_holder_uninit(&gh);
	return rv;
}

/**
 * gfs2_releasepage - free the metadata associated with a page
 * @page: the page that's being released
 * @gfp_mask: passed from Linux VFS, ignored by us
 *
 * Call try_to_free_buffers() if the buffers in this page can be
 * released.
 *
 * Returns: 0
 */

int gfs2_releasepage(struct page *page, gfp_t gfp_mask)
{
	struct address_space *mapping = page->mapping;
=======
	if (!partial_page)
		filemap_release_folio(folio, 0);
}

/**
 * gfs2_release_folio - free the metadata associated with a folio
 * @folio: the folio that's being released
 * @gfp_mask: passed from Linux VFS, ignored by us
 *
 * Calls try_to_free_buffers() to free the buffers and put the folio if the
 * buffers can be released.
 *
 * Returns: true if the folio was put or else false
 */

bool gfs2_release_folio(struct folio *folio, gfp_t gfp_mask)
{
	struct address_space *mapping = folio->mapping;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct gfs2_sbd *sdp = gfs2_mapping2sbd(mapping);
	struct buffer_head *bh, *head;
	struct gfs2_bufdata *bd;

<<<<<<< HEAD
	if (!page_has_buffers(page))
		return 0;

	gfs2_log_lock(sdp);
	spin_lock(&sdp->sd_ail_lock);
	head = bh = page_buffers(page);
=======
	head = folio_buffers(folio);
	if (!head)
		return false;

	/*
	 * mm accommodates an old ext3 case where clean folios might
	 * not have had the dirty bit cleared.	Thus, it can send actual
	 * dirty folios to ->release_folio() via shrink_active_list().
	 *
	 * As a workaround, we skip folios that contain dirty buffers
	 * below.  Once ->release_folio isn't called on dirty folios
	 * anymore, we can warn on dirty buffers like we used to here
	 * again.
	 */

	gfs2_log_lock(sdp);
	bh = head;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	do {
		if (atomic_read(&bh->b_count))
			goto cannot_release;
		bd = bh->b_private;
<<<<<<< HEAD
		if (bd && bd->bd_ail)
			goto cannot_release;
		if (buffer_pinned(bh) || buffer_dirty(bh))
			goto not_possible;
		bh = bh->b_this_page;
	} while(bh != head);
	spin_unlock(&sdp->sd_ail_lock);
	gfs2_log_unlock(sdp);

	head = bh = page_buffers(page);
	do {
		gfs2_log_lock(sdp);
		bd = bh->b_private;
		if (bd) {
			gfs2_assert_warn(sdp, bd->bd_bh == bh);
			gfs2_assert_warn(sdp, list_empty(&bd->bd_list_tr));
			if (!list_empty(&bd->bd_le.le_list)) {
				if (!buffer_pinned(bh))
					list_del_init(&bd->bd_le.le_list);
				else
					bd = NULL;
			}
			if (bd)
				bd->bd_bh = NULL;
			bh->b_private = NULL;
		}
		gfs2_log_unlock(sdp);
		if (bd)
			kmem_cache_free(gfs2_bufdata_cachep, bd);

		bh = bh->b_this_page;
	} while (bh != head);

	return try_to_free_buffers(page);

not_possible: /* Should never happen */
	WARN_ON(buffer_dirty(bh));
	WARN_ON(buffer_pinned(bh));
cannot_release:
	spin_unlock(&sdp->sd_ail_lock);
	gfs2_log_unlock(sdp);
	return 0;
}

static const struct address_space_operations gfs2_writeback_aops = {
	.writepage = gfs2_writeback_writepage,
	.writepages = gfs2_writeback_writepages,
	.readpage = gfs2_readpage,
	.readpages = gfs2_readpages,
	.write_begin = gfs2_write_begin,
	.write_end = gfs2_write_end,
	.bmap = gfs2_bmap,
	.invalidatepage = gfs2_invalidatepage,
	.releasepage = gfs2_releasepage,
	.direct_IO = gfs2_direct_IO,
	.migratepage = buffer_migrate_page,
	.is_partially_uptodate = block_is_partially_uptodate,
	.error_remove_page = generic_error_remove_page,
};

static const struct address_space_operations gfs2_ordered_aops = {
	.writepage = gfs2_ordered_writepage,
	.readpage = gfs2_readpage,
	.readpages = gfs2_readpages,
	.write_begin = gfs2_write_begin,
	.write_end = gfs2_write_end,
	.set_page_dirty = gfs2_set_page_dirty,
	.bmap = gfs2_bmap,
	.invalidatepage = gfs2_invalidatepage,
	.releasepage = gfs2_releasepage,
	.direct_IO = gfs2_direct_IO,
	.migratepage = buffer_migrate_page,
	.is_partially_uptodate = block_is_partially_uptodate,
	.error_remove_page = generic_error_remove_page,
=======
		if (bd && bd->bd_tr)
			goto cannot_release;
		if (buffer_dirty(bh) || WARN_ON(buffer_pinned(bh)))
			goto cannot_release;
		bh = bh->b_this_page;
	} while (bh != head);

	bh = head;
	do {
		bd = bh->b_private;
		if (bd) {
			gfs2_assert_warn(sdp, bd->bd_bh == bh);
			bd->bd_bh = NULL;
			bh->b_private = NULL;
			/*
			 * The bd may still be queued as a revoke, in which
			 * case we must not dequeue nor free it.
			 */
			if (!bd->bd_blkno && !list_empty(&bd->bd_list))
				list_del_init(&bd->bd_list);
			if (list_empty(&bd->bd_list))
				kmem_cache_free(gfs2_bufdata_cachep, bd);
		}

		bh = bh->b_this_page;
	} while (bh != head);
	gfs2_log_unlock(sdp);

	return try_to_free_buffers(folio);

cannot_release:
	gfs2_log_unlock(sdp);
	return false;
}

static const struct address_space_operations gfs2_aops = {
	.writepages = gfs2_writepages,
	.read_folio = gfs2_read_folio,
	.readahead = gfs2_readahead,
	.dirty_folio = iomap_dirty_folio,
	.release_folio = iomap_release_folio,
	.invalidate_folio = iomap_invalidate_folio,
	.bmap = gfs2_bmap,
	.migrate_folio = filemap_migrate_folio,
	.is_partially_uptodate = iomap_is_partially_uptodate,
	.error_remove_folio = generic_error_remove_folio,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static const struct address_space_operations gfs2_jdata_aops = {
	.writepage = gfs2_jdata_writepage,
	.writepages = gfs2_jdata_writepages,
<<<<<<< HEAD
	.readpage = gfs2_readpage,
	.readpages = gfs2_readpages,
	.write_begin = gfs2_write_begin,
	.write_end = gfs2_write_end,
	.set_page_dirty = gfs2_set_page_dirty,
	.bmap = gfs2_bmap,
	.invalidatepage = gfs2_invalidatepage,
	.releasepage = gfs2_releasepage,
	.is_partially_uptodate = block_is_partially_uptodate,
	.error_remove_page = generic_error_remove_page,
=======
	.read_folio = gfs2_read_folio,
	.readahead = gfs2_readahead,
	.dirty_folio = jdata_dirty_folio,
	.bmap = gfs2_bmap,
	.invalidate_folio = gfs2_invalidate_folio,
	.release_folio = gfs2_release_folio,
	.is_partially_uptodate = block_is_partially_uptodate,
	.error_remove_folio = generic_error_remove_folio,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

void gfs2_set_aops(struct inode *inode)
{
<<<<<<< HEAD
	struct gfs2_inode *ip = GFS2_I(inode);

	if (gfs2_is_writeback(ip))
		inode->i_mapping->a_ops = &gfs2_writeback_aops;
	else if (gfs2_is_ordered(ip))
		inode->i_mapping->a_ops = &gfs2_ordered_aops;
	else if (gfs2_is_jdata(ip))
		inode->i_mapping->a_ops = &gfs2_jdata_aops;
	else
		BUG();
}

=======
	if (gfs2_is_jdata(GFS2_I(inode)))
		inode->i_mapping->a_ops = &gfs2_jdata_aops;
	else
		inode->i_mapping->a_ops = &gfs2_aops;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
