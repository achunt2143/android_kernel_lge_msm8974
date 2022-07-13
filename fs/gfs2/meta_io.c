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
#include <linux/mm.h>
#include <linux/pagemap.h>
#include <linux/writeback.h>
#include <linux/swap.h>
#include <linux/delay.h>
#include <linux/bio.h>
#include <linux/gfs2_ondisk.h>

#include "gfs2.h"
#include "incore.h"
#include "glock.h"
#include "glops.h"
#include "inode.h"
#include "log.h"
#include "lops.h"
#include "meta_io.h"
#include "rgrp.h"
#include "trans.h"
#include "util.h"
#include "trace_gfs2.h"

static int gfs2_aspace_writepage(struct page *page, struct writeback_control *wbc)
{
	struct buffer_head *bh, *head;
	int nr_underway = 0;
<<<<<<< HEAD
	int write_op = REQ_META | REQ_PRIO |
		(wbc->sync_mode == WB_SYNC_ALL ? WRITE_SYNC : WRITE);
=======
	blk_opf_t write_flags = REQ_META | REQ_PRIO | wbc_to_write_flags(wbc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	BUG_ON(!PageLocked(page));
	BUG_ON(!page_has_buffers(page));

	head = page_buffers(page);
	bh = head;

	do {
		if (!buffer_mapped(bh))
			continue;
		/*
		 * If it's a fully non-blocking write attempt and we cannot
		 * lock the buffer then redirty the page.  Note that this can
<<<<<<< HEAD
		 * potentially cause a busy-wait loop from pdflush and kswapd
=======
		 * potentially cause a busy-wait loop from flusher thread and kswapd
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		 * activity, but those code paths have their own higher-level
		 * throttling.
		 */
		if (wbc->sync_mode != WB_SYNC_NONE) {
			lock_buffer(bh);
		} else if (!trylock_buffer(bh)) {
			redirty_page_for_writepage(wbc, page);
			continue;
		}
		if (test_clear_buffer_dirty(bh)) {
			mark_buffer_async_write(bh);
		} else {
			unlock_buffer(bh);
		}
	} while ((bh = bh->b_this_page) != head);

	/*
	 * The page and its buffers are protected by PageWriteback(), so we can
	 * drop the bh refcounts early.
	 */
	BUG_ON(PageWriteback(page));
	set_page_writeback(page);

	do {
		struct buffer_head *next = bh->b_this_page;
		if (buffer_async_write(bh)) {
<<<<<<< HEAD
			submit_bh(write_op, bh);
=======
			submit_bh(REQ_OP_WRITE | write_flags, bh);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			nr_underway++;
		}
		bh = next;
	} while (bh != head);
	unlock_page(page);

	if (nr_underway == 0)
		end_page_writeback(page);

	return 0;
}

const struct address_space_operations gfs2_meta_aops = {
<<<<<<< HEAD
	.writepage = gfs2_aspace_writepage,
	.releasepage = gfs2_releasepage,
};

/**
 * gfs2_meta_sync - Sync all buffers associated with a glock
 * @gl: The glock
 *
 */

void gfs2_meta_sync(struct gfs2_glock *gl)
{
	struct address_space *mapping = gfs2_glock2aspace(gl);
	int error;

	filemap_fdatawrite(mapping);
	error = filemap_fdatawait(mapping);

	if (error)
		gfs2_io_error(gl->gl_sbd);
}
=======
	.dirty_folio	= block_dirty_folio,
	.invalidate_folio = block_invalidate_folio,
	.writepage = gfs2_aspace_writepage,
	.release_folio = gfs2_release_folio,
};

const struct address_space_operations gfs2_rgrp_aops = {
	.dirty_folio	= block_dirty_folio,
	.invalidate_folio = block_invalidate_folio,
	.writepage = gfs2_aspace_writepage,
	.release_folio = gfs2_release_folio,
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * gfs2_getbuf - Get a buffer with a given address space
 * @gl: the glock
 * @blkno: the block number (filesystem scope)
 * @create: 1 if the buffer should be created
 *
 * Returns: the buffer
 */

struct buffer_head *gfs2_getbuf(struct gfs2_glock *gl, u64 blkno, int create)
{
	struct address_space *mapping = gfs2_glock2aspace(gl);
<<<<<<< HEAD
	struct gfs2_sbd *sdp = gl->gl_sbd;
	struct page *page;
=======
	struct gfs2_sbd *sdp = gl->gl_name.ln_sbd;
	struct folio *folio;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct buffer_head *bh;
	unsigned int shift;
	unsigned long index;
	unsigned int bufnum;

<<<<<<< HEAD
	shift = PAGE_CACHE_SHIFT - sdp->sd_sb.sb_bsize_shift;
=======
	if (mapping == NULL)
		mapping = &sdp->sd_aspace;

	shift = PAGE_SHIFT - sdp->sd_sb.sb_bsize_shift;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	index = blkno >> shift;             /* convert block to page */
	bufnum = blkno - (index << shift);  /* block buf index within page */

	if (create) {
<<<<<<< HEAD
		for (;;) {
			page = grab_cache_page(mapping, index);
			if (page)
				break;
			yield();
		}
	} else {
		page = find_lock_page(mapping, index);
		if (!page)
			return NULL;
	}

	if (!page_has_buffers(page))
		create_empty_buffers(page, sdp->sd_sb.sb_bsize, 0);

	/* Locate header for our buffer within our page */
	for (bh = page_buffers(page); bufnum--; bh = bh->b_this_page)
		/* Do nothing */;
	get_bh(bh);

	if (!buffer_mapped(bh))
		map_bh(bh, sdp->sd_vfs, blkno);

	unlock_page(page);
	mark_page_accessed(page);
	page_cache_release(page);
=======
		folio = __filemap_get_folio(mapping, index,
				FGP_LOCK | FGP_ACCESSED | FGP_CREAT,
				mapping_gfp_mask(mapping) | __GFP_NOFAIL);
		bh = folio_buffers(folio);
		if (!bh)
			bh = create_empty_buffers(folio,
				sdp->sd_sb.sb_bsize, 0);
	} else {
		folio = __filemap_get_folio(mapping, index,
				FGP_LOCK | FGP_ACCESSED, 0);
		if (IS_ERR(folio))
			return NULL;
		bh = folio_buffers(folio);
	}

	if (!bh)
		goto out_unlock;

	bh = get_nth_bh(bh, bufnum);
	if (!buffer_mapped(bh))
		map_bh(bh, sdp->sd_vfs, blkno);

out_unlock:
	folio_unlock(folio);
	folio_put(folio);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return bh;
}

static void meta_prep_new(struct buffer_head *bh)
{
	struct gfs2_meta_header *mh = (struct gfs2_meta_header *)bh->b_data;

	lock_buffer(bh);
	clear_buffer_dirty(bh);
	set_buffer_uptodate(bh);
	unlock_buffer(bh);

	mh->mh_magic = cpu_to_be32(GFS2_MAGIC);
}

/**
 * gfs2_meta_new - Get a block
 * @gl: The glock associated with this block
 * @blkno: The block number
 *
 * Returns: The buffer
 */

struct buffer_head *gfs2_meta_new(struct gfs2_glock *gl, u64 blkno)
{
	struct buffer_head *bh;
	bh = gfs2_getbuf(gl, blkno, CREATE);
	meta_prep_new(bh);
	return bh;
}

<<<<<<< HEAD
=======
static void gfs2_meta_read_endio(struct bio *bio)
{
	struct bio_vec *bvec;
	struct bvec_iter_all iter_all;

	bio_for_each_segment_all(bvec, bio, iter_all) {
		struct page *page = bvec->bv_page;
		struct buffer_head *bh = page_buffers(page);
		unsigned int len = bvec->bv_len;

		while (bh_offset(bh) < bvec->bv_offset)
			bh = bh->b_this_page;
		do {
			struct buffer_head *next = bh->b_this_page;
			len -= bh->b_size;
			bh->b_end_io(bh, !bio->bi_status);
			bh = next;
		} while (bh && len);
	}
	bio_put(bio);
}

/*
 * Submit several consecutive buffer head I/O requests as a single bio I/O
 * request.  (See submit_bh_wbc.)
 */
static void gfs2_submit_bhs(blk_opf_t opf, struct buffer_head *bhs[], int num)
{
	while (num > 0) {
		struct buffer_head *bh = *bhs;
		struct bio *bio;

		bio = bio_alloc(bh->b_bdev, num, opf, GFP_NOIO);
		bio->bi_iter.bi_sector = bh->b_blocknr * (bh->b_size >> 9);
		while (num > 0) {
			bh = *bhs;
			if (!bio_add_page(bio, bh->b_page, bh->b_size, bh_offset(bh))) {
				BUG_ON(bio->bi_iter.bi_size == 0);
				break;
			}
			bhs++;
			num--;
		}
		bio->bi_end_io = gfs2_meta_read_endio;
		submit_bio(bio);
	}
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * gfs2_meta_read - Read a block from disk
 * @gl: The glock covering the block
 * @blkno: The block number
 * @flags: flags
<<<<<<< HEAD
=======
 * @rahead: Do read-ahead
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @bhp: the place where the buffer is returned (NULL on failure)
 *
 * Returns: errno
 */

int gfs2_meta_read(struct gfs2_glock *gl, u64 blkno, int flags,
<<<<<<< HEAD
		   struct buffer_head **bhp)
{
	struct gfs2_sbd *sdp = gl->gl_sbd;
	struct buffer_head *bh;

	if (unlikely(test_bit(SDF_SHUTDOWN, &sdp->sd_flags)))
		return -EIO;
=======
		   int rahead, struct buffer_head **bhp)
{
	struct gfs2_sbd *sdp = gl->gl_name.ln_sbd;
	struct buffer_head *bh, *bhs[2];
	int num = 0;

	if (gfs2_withdrawing_or_withdrawn(sdp) &&
	    !gfs2_withdraw_in_prog(sdp)) {
		*bhp = NULL;
		return -EIO;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	*bhp = bh = gfs2_getbuf(gl, blkno, CREATE);

	lock_buffer(bh);
	if (buffer_uptodate(bh)) {
		unlock_buffer(bh);
<<<<<<< HEAD
		return 0;
	}
	bh->b_end_io = end_buffer_read_sync;
	get_bh(bh);
	submit_bh(READ_SYNC | REQ_META | REQ_PRIO, bh);
	if (!(flags & DIO_WAIT))
		return 0;

	wait_on_buffer(bh);
	if (unlikely(!buffer_uptodate(bh))) {
		struct gfs2_trans *tr = current->journal_info;
		if (tr && tr->tr_touched)
			gfs2_io_error_bh(sdp, bh);
		brelse(bh);
=======
		flags &= ~DIO_WAIT;
	} else {
		bh->b_end_io = end_buffer_read_sync;
		get_bh(bh);
		bhs[num++] = bh;
	}

	if (rahead) {
		bh = gfs2_getbuf(gl, blkno + 1, CREATE);

		lock_buffer(bh);
		if (buffer_uptodate(bh)) {
			unlock_buffer(bh);
			brelse(bh);
		} else {
			bh->b_end_io = end_buffer_read_sync;
			bhs[num++] = bh;
		}
	}

	gfs2_submit_bhs(REQ_OP_READ | REQ_META | REQ_PRIO, bhs, num);
	if (!(flags & DIO_WAIT))
		return 0;

	bh = *bhp;
	wait_on_buffer(bh);
	if (unlikely(!buffer_uptodate(bh))) {
		struct gfs2_trans *tr = current->journal_info;
		if (tr && test_bit(TR_TOUCHED, &tr->tr_flags))
			gfs2_io_error_bh_wd(sdp, bh);
		brelse(bh);
		*bhp = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EIO;
	}

	return 0;
}

/**
 * gfs2_meta_wait - Reread a block from disk
 * @sdp: the filesystem
 * @bh: The block to wait for
 *
 * Returns: errno
 */

int gfs2_meta_wait(struct gfs2_sbd *sdp, struct buffer_head *bh)
{
<<<<<<< HEAD
	if (unlikely(test_bit(SDF_SHUTDOWN, &sdp->sd_flags)))
=======
	if (gfs2_withdrawing_or_withdrawn(sdp) &&
	    !gfs2_withdraw_in_prog(sdp))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EIO;

	wait_on_buffer(bh);

	if (!buffer_uptodate(bh)) {
		struct gfs2_trans *tr = current->journal_info;
<<<<<<< HEAD
		if (tr && tr->tr_touched)
			gfs2_io_error_bh(sdp, bh);
		return -EIO;
	}
	if (unlikely(test_bit(SDF_SHUTDOWN, &sdp->sd_flags)))
=======
		if (tr && test_bit(TR_TOUCHED, &tr->tr_flags))
			gfs2_io_error_bh_wd(sdp, bh);
		return -EIO;
	}
	if (gfs2_withdrawing_or_withdrawn(sdp) &&
	    !gfs2_withdraw_in_prog(sdp))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EIO;

	return 0;
}

<<<<<<< HEAD
/**
 * gfs2_attach_bufdata - attach a struct gfs2_bufdata structure to a buffer
 * @gl: the glock the buffer belongs to
 * @bh: The buffer to be attached to
 * @meta: Flag to indicate whether its metadata or not
 */

void gfs2_attach_bufdata(struct gfs2_glock *gl, struct buffer_head *bh,
			 int meta)
{
	struct gfs2_bufdata *bd;

	if (meta)
		lock_page(bh->b_page);

	if (bh->b_private) {
		if (meta)
			unlock_page(bh->b_page);
		return;
	}

	bd = kmem_cache_zalloc(gfs2_bufdata_cachep, GFP_NOFS | __GFP_NOFAIL);
	bd->bd_bh = bh;
	bd->bd_gl = gl;

	INIT_LIST_HEAD(&bd->bd_list_tr);
	if (meta)
		lops_init_le(&bd->bd_le, &gfs2_buf_lops);
	else
		lops_init_le(&bd->bd_le, &gfs2_databuf_lops);
	bh->b_private = bd;

	if (meta)
		unlock_page(bh->b_page);
}

void gfs2_remove_from_journal(struct buffer_head *bh, struct gfs2_trans *tr, int meta)
{
	struct address_space *mapping = bh->b_page->mapping;
	struct gfs2_sbd *sdp = gfs2_mapping2sbd(mapping);
	struct gfs2_bufdata *bd = bh->b_private;
=======
void gfs2_remove_from_journal(struct buffer_head *bh, int meta)
{
	struct address_space *mapping = bh->b_folio->mapping;
	struct gfs2_sbd *sdp = gfs2_mapping2sbd(mapping);
	struct gfs2_bufdata *bd = bh->b_private;
	struct gfs2_trans *tr = current->journal_info;
	int was_pinned = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (test_clear_buffer_pinned(bh)) {
		trace_gfs2_pin(bd, 0);
		atomic_dec(&sdp->sd_log_pinned);
<<<<<<< HEAD
		list_del_init(&bd->bd_le.le_list);
		if (meta) {
			gfs2_assert_warn(sdp, sdp->sd_log_num_buf);
			sdp->sd_log_num_buf--;
			tr->tr_num_buf_rm++;
		} else {
			gfs2_assert_warn(sdp, sdp->sd_log_num_databuf);
			sdp->sd_log_num_databuf--;
			tr->tr_num_databuf_rm++;
		}
		tr->tr_touched = 1;
		brelse(bh);
	}
	if (bd) {
		spin_lock(&sdp->sd_ail_lock);
		if (bd->bd_ail) {
			gfs2_remove_from_ail(bd);
			bh->b_private = NULL;
			bd->bd_bh = NULL;
			bd->bd_blkno = bh->b_blocknr;
			gfs2_trans_add_revoke(sdp, bd);
		}
		spin_unlock(&sdp->sd_ail_lock);
=======
		list_del_init(&bd->bd_list);
		if (meta == REMOVE_META)
			tr->tr_num_buf_rm++;
		else
			tr->tr_num_databuf_rm++;
		set_bit(TR_TOUCHED, &tr->tr_flags);
		was_pinned = 1;
		brelse(bh);
	}
	if (bd) {
		if (bd->bd_tr) {
			gfs2_trans_add_revoke(sdp, bd);
		} else if (was_pinned) {
			bh->b_private = NULL;
			kmem_cache_free(gfs2_bufdata_cachep, bd);
		} else if (!list_empty(&bd->bd_ail_st_list) &&
					!list_empty(&bd->bd_ail_gl_list)) {
			gfs2_remove_from_ail(bd);
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	clear_buffer_dirty(bh);
	clear_buffer_uptodate(bh);
}

/**
<<<<<<< HEAD
 * gfs2_meta_wipe - make inode's buffers so they aren't dirty/pinned anymore
=======
 * gfs2_ail1_wipe - remove deleted/freed buffers from the ail1 list
 * @sdp: superblock
 * @bstart: starting block address of buffers to remove
 * @blen: length of buffers to be removed
 *
 * This function is called from gfs2_journal wipe, whose job is to remove
 * buffers, corresponding to deleted blocks, from the journal. If we find any
 * bufdata elements on the system ail1 list, they haven't been written to
 * the journal yet. So we remove them.
 */
static void gfs2_ail1_wipe(struct gfs2_sbd *sdp, u64 bstart, u32 blen)
{
	struct gfs2_trans *tr, *s;
	struct gfs2_bufdata *bd, *bs;
	struct buffer_head *bh;
	u64 end = bstart + blen;

	gfs2_log_lock(sdp);
	spin_lock(&sdp->sd_ail_lock);
	list_for_each_entry_safe(tr, s, &sdp->sd_ail1_list, tr_list) {
		list_for_each_entry_safe(bd, bs, &tr->tr_ail1_list,
					 bd_ail_st_list) {
			bh = bd->bd_bh;
			if (bh->b_blocknr < bstart || bh->b_blocknr >= end)
				continue;

			gfs2_remove_from_journal(bh, REMOVE_JDATA);
		}
	}
	spin_unlock(&sdp->sd_ail_lock);
	gfs2_log_unlock(sdp);
}

static struct buffer_head *gfs2_getjdatabuf(struct gfs2_inode *ip, u64 blkno)
{
	struct address_space *mapping = ip->i_inode.i_mapping;
	struct gfs2_sbd *sdp = GFS2_SB(&ip->i_inode);
	struct folio *folio;
	struct buffer_head *bh;
	unsigned int shift = PAGE_SHIFT - sdp->sd_sb.sb_bsize_shift;
	unsigned long index = blkno >> shift; /* convert block to page */
	unsigned int bufnum = blkno - (index << shift);

	folio = __filemap_get_folio(mapping, index, FGP_LOCK | FGP_ACCESSED, 0);
	if (IS_ERR(folio))
		return NULL;
	bh = folio_buffers(folio);
	if (bh)
		bh = get_nth_bh(bh, bufnum);
	folio_unlock(folio);
	folio_put(folio);
	return bh;
}

/**
 * gfs2_journal_wipe - make inode's buffers so they aren't dirty/pinned anymore
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @ip: the inode who owns the buffers
 * @bstart: the first buffer in the run
 * @blen: the number of buffers in the run
 *
 */

<<<<<<< HEAD
void gfs2_meta_wipe(struct gfs2_inode *ip, u64 bstart, u32 blen)
{
	struct gfs2_sbd *sdp = GFS2_SB(&ip->i_inode);
	struct buffer_head *bh;

	while (blen) {
		bh = gfs2_getbuf(ip->i_gl, bstart, NO_CREATE);
		if (bh) {
			lock_buffer(bh);
			gfs2_log_lock(sdp);
			gfs2_remove_from_journal(bh, current->journal_info, 1);
=======
void gfs2_journal_wipe(struct gfs2_inode *ip, u64 bstart, u32 blen)
{
	struct gfs2_sbd *sdp = GFS2_SB(&ip->i_inode);
	struct buffer_head *bh;
	int ty;

	if (!ip->i_gl) {
		/* This can only happen during incomplete inode creation. */
		BUG_ON(!test_bit(GIF_ALLOC_FAILED, &ip->i_flags));
		return;
	}

	gfs2_ail1_wipe(sdp, bstart, blen);
	while (blen) {
		ty = REMOVE_META;
		bh = gfs2_getbuf(ip->i_gl, bstart, NO_CREATE);
		if (!bh && gfs2_is_jdata(ip)) {
			bh = gfs2_getjdatabuf(ip, bstart);
			ty = REMOVE_JDATA;
		}
		if (bh) {
			lock_buffer(bh);
			gfs2_log_lock(sdp);
			spin_lock(&sdp->sd_ail_lock);
			gfs2_remove_from_journal(bh, ty);
			spin_unlock(&sdp->sd_ail_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			gfs2_log_unlock(sdp);
			unlock_buffer(bh);
			brelse(bh);
		}

		bstart++;
		blen--;
	}
}

/**
<<<<<<< HEAD
 * gfs2_meta_indirect_buffer - Get a metadata buffer
 * @ip: The GFS2 inode
 * @height: The level of this buf in the metadata (indir addr) tree (if any)
 * @num: The block number (device relative) of the buffer
 * @new: Non-zero if we may create a new buffer
=======
 * gfs2_meta_buffer - Get a metadata buffer
 * @ip: The GFS2 inode
 * @mtype: The block type (GFS2_METATYPE_*)
 * @num: The block number (device relative) of the buffer
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @bhp: the buffer is returned here
 *
 * Returns: errno
 */

<<<<<<< HEAD
int gfs2_meta_indirect_buffer(struct gfs2_inode *ip, int height, u64 num,
			      int new, struct buffer_head **bhp)
=======
int gfs2_meta_buffer(struct gfs2_inode *ip, u32 mtype, u64 num,
		     struct buffer_head **bhp)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct gfs2_sbd *sdp = GFS2_SB(&ip->i_inode);
	struct gfs2_glock *gl = ip->i_gl;
	struct buffer_head *bh;
	int ret = 0;
<<<<<<< HEAD

	if (new) {
		BUG_ON(height == 0);
		bh = gfs2_meta_new(gl, num);
		gfs2_trans_add_bh(ip->i_gl, bh, 1);
		gfs2_metatype_set(bh, GFS2_METATYPE_IN, GFS2_FORMAT_IN);
		gfs2_buffer_clear_tail(bh, sizeof(struct gfs2_meta_header));
	} else {
		u32 mtype = height ? GFS2_METATYPE_IN : GFS2_METATYPE_DI;
		ret = gfs2_meta_read(gl, num, DIO_WAIT, &bh);
		if (ret == 0 && gfs2_metatype_check(sdp, bh, mtype)) {
			brelse(bh);
			ret = -EIO;
		}
	}
	*bhp = bh;
=======
	int rahead = 0;

	if (num == ip->i_no_addr)
		rahead = ip->i_rahead;

	ret = gfs2_meta_read(gl, num, DIO_WAIT, rahead, &bh);
	if (ret == 0 && gfs2_metatype_check(sdp, bh, mtype)) {
		brelse(bh);
		ret = -EIO;
	} else {
		*bhp = bh;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

/**
 * gfs2_meta_ra - start readahead on an extent of a file
 * @gl: the glock the blocks belong to
 * @dblock: the starting disk block
 * @extlen: the number of blocks in the extent
 *
 * returns: the first buffer in the extent
 */

struct buffer_head *gfs2_meta_ra(struct gfs2_glock *gl, u64 dblock, u32 extlen)
{
<<<<<<< HEAD
	struct gfs2_sbd *sdp = gl->gl_sbd;
=======
	struct gfs2_sbd *sdp = gl->gl_name.ln_sbd;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct buffer_head *first_bh, *bh;
	u32 max_ra = gfs2_tune_get(sdp, gt_max_readahead) >>
			  sdp->sd_sb.sb_bsize_shift;

	BUG_ON(!extlen);

	if (max_ra < 1)
		max_ra = 1;
	if (extlen > max_ra)
		extlen = max_ra;

	first_bh = gfs2_getbuf(gl, dblock, CREATE);

	if (buffer_uptodate(first_bh))
		goto out;
<<<<<<< HEAD
	if (!buffer_locked(first_bh))
		ll_rw_block(READ_SYNC | REQ_META, 1, &first_bh);
=======
	bh_read_nowait(first_bh, REQ_META | REQ_PRIO);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	dblock++;
	extlen--;

	while (extlen) {
		bh = gfs2_getbuf(gl, dblock, CREATE);

<<<<<<< HEAD
		if (!buffer_uptodate(bh) && !buffer_locked(bh))
			ll_rw_block(READA | REQ_META, 1, &bh);
=======
		bh_readahead(bh, REQ_RAHEAD | REQ_META | REQ_PRIO);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		brelse(bh);
		dblock++;
		extlen--;
		if (!buffer_locked(first_bh) && buffer_uptodate(first_bh))
			goto out;
	}

	wait_on_buffer(first_bh);
out:
	return first_bh;
}

