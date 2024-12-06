<<<<<<< HEAD
/*
 * Copyright (C) Sistina Software, Inc.  1997-2003 All rights reserved.
 * Copyright (C) 2004-2007 Red Hat, Inc.  All rights reserved.
 *
 * This copyrighted material is made available to anyone wishing to use,
 * modify, copy, or redistribute it subject to the terms and conditions
 * of the GNU General Public License version 2.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) Sistina Software, Inc.  1997-2003 All rights reserved.
 * Copyright (C) 2004-2007 Red Hat, Inc.  All rights reserved.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/completion.h>
#include <linux/buffer_head.h>
#include <linux/gfs2_ondisk.h>
#include <linux/crc32.h>
<<<<<<< HEAD
=======
#include <linux/crc32c.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/freezer.h>
#include <linux/bio.h>
<<<<<<< HEAD
=======
#include <linux/blkdev.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/writeback.h>
#include <linux/list_sort.h>

#include "gfs2.h"
#include "incore.h"
#include "bmap.h"
#include "glock.h"
#include "log.h"
#include "lops.h"
#include "meta_io.h"
#include "util.h"
#include "dir.h"
#include "trace_gfs2.h"
<<<<<<< HEAD

#define PULL 1
=======
#include "trans.h"

static void gfs2_log_shutdown(struct gfs2_sbd *sdp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * gfs2_struct2blk - compute stuff
 * @sdp: the filesystem
 * @nstruct: the number of structures
<<<<<<< HEAD
 * @ssize: the size of the structures
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Compute the number of log descriptor blocks needed to hold a certain number
 * of structures of a certain size.
 *
 * Returns: the number of blocks needed (minimum is always 1)
 */

<<<<<<< HEAD
unsigned int gfs2_struct2blk(struct gfs2_sbd *sdp, unsigned int nstruct,
			     unsigned int ssize)
=======
unsigned int gfs2_struct2blk(struct gfs2_sbd *sdp, unsigned int nstruct)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned int blks;
	unsigned int first, second;

<<<<<<< HEAD
	blks = 1;
	first = (sdp->sd_sb.sb_bsize - sizeof(struct gfs2_log_descriptor)) / ssize;

	if (nstruct > first) {
		second = (sdp->sd_sb.sb_bsize -
			  sizeof(struct gfs2_meta_header)) / ssize;
=======
	/* The initial struct gfs2_log_descriptor block */
	blks = 1;
	first = sdp->sd_ldptrs;

	if (nstruct > first) {
		/* Subsequent struct gfs2_meta_header blocks */
		second = sdp->sd_inptrs;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		blks += DIV_ROUND_UP(nstruct - first, second);
	}

	return blks;
}

/**
 * gfs2_remove_from_ail - Remove an entry from the ail lists, updating counters
<<<<<<< HEAD
 * @mapping: The associated mapping (maybe NULL)
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @bd: The gfs2_bufdata to remove
 *
 * The ail lock _must_ be held when calling this function
 *
 */

void gfs2_remove_from_ail(struct gfs2_bufdata *bd)
{
<<<<<<< HEAD
	bd->bd_ail = NULL;
=======
	bd->bd_tr = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	list_del_init(&bd->bd_ail_st_list);
	list_del_init(&bd->bd_ail_gl_list);
	atomic_dec(&bd->bd_gl->gl_ail_count);
	brelse(bd->bd_bh);
}

<<<<<<< HEAD
/**
 * gfs2_ail1_start_one - Start I/O on a part of the AIL
 * @sdp: the filesystem
 * @wbc: The writeback control structure
 * @ai: The ail structure
 *
=======
static int __gfs2_writepage(struct folio *folio, struct writeback_control *wbc,
		       void *data)
{
	struct address_space *mapping = data;
	int ret = mapping->a_ops->writepage(&folio->page, wbc);
	mapping_set_error(mapping, ret);
	return ret;
}

/**
 * gfs2_ail1_start_one - Start I/O on a transaction
 * @sdp: The superblock
 * @wbc: The writeback control structure
 * @tr: The transaction to start I/O on
 * @plug: The block plug currently active
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

static int gfs2_ail1_start_one(struct gfs2_sbd *sdp,
			       struct writeback_control *wbc,
<<<<<<< HEAD
			       struct gfs2_ail *ai)
=======
			       struct gfs2_trans *tr, struct blk_plug *plug)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
__releases(&sdp->sd_ail_lock)
__acquires(&sdp->sd_ail_lock)
{
	struct gfs2_glock *gl = NULL;
	struct address_space *mapping;
	struct gfs2_bufdata *bd, *s;
	struct buffer_head *bh;
<<<<<<< HEAD

	list_for_each_entry_safe_reverse(bd, s, &ai->ai_ail1_list, bd_ail_st_list) {
		bh = bd->bd_bh;

		gfs2_assert(sdp, bd->bd_ail == ai);

		if (!buffer_busy(bh)) {
			if (!buffer_uptodate(bh))
				gfs2_io_error_bh(sdp, bh);
			list_move(&bd->bd_ail_st_list, &ai->ai_ail2_list);
			continue;
		}

=======
	int ret = 0;

	list_for_each_entry_safe_reverse(bd, s, &tr->tr_ail1_list, bd_ail_st_list) {
		bh = bd->bd_bh;

		gfs2_assert(sdp, bd->bd_tr == tr);

		if (!buffer_busy(bh)) {
			if (buffer_uptodate(bh)) {
				list_move(&bd->bd_ail_st_list,
					  &tr->tr_ail2_list);
				continue;
			}
			if (!cmpxchg(&sdp->sd_log_error, 0, -EIO)) {
				gfs2_io_error_bh(sdp, bh);
				gfs2_withdraw_delayed(sdp);
			}
		}

		if (gfs2_withdrawing_or_withdrawn(sdp)) {
			gfs2_remove_from_ail(bd);
			continue;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!buffer_dirty(bh))
			continue;
		if (gl == bd->bd_gl)
			continue;
		gl = bd->bd_gl;
<<<<<<< HEAD
		list_move(&bd->bd_ail_st_list, &ai->ai_ail1_list);
		mapping = bh->b_page->mapping;
		if (!mapping)
			continue;
		spin_unlock(&sdp->sd_ail_lock);
		generic_writepages(mapping, wbc);
		spin_lock(&sdp->sd_ail_lock);
		if (wbc->nr_to_write <= 0)
			break;
		return 1;
	}

	return 0;
}

=======
		list_move(&bd->bd_ail_st_list, &tr->tr_ail1_list);
		mapping = bh->b_folio->mapping;
		if (!mapping)
			continue;
		spin_unlock(&sdp->sd_ail_lock);
		ret = write_cache_pages(mapping, wbc, __gfs2_writepage, mapping);
		if (need_resched()) {
			blk_finish_plug(plug);
			cond_resched();
			blk_start_plug(plug);
		}
		spin_lock(&sdp->sd_ail_lock);
		if (ret == -ENODATA) /* if a jdata write into a new hole */
			ret = 0; /* ignore it */
		if (ret || wbc->nr_to_write <= 0)
			break;
		return -EBUSY;
	}

	return ret;
}

static void dump_ail_list(struct gfs2_sbd *sdp)
{
	struct gfs2_trans *tr;
	struct gfs2_bufdata *bd;
	struct buffer_head *bh;

	list_for_each_entry_reverse(tr, &sdp->sd_ail1_list, tr_list) {
		list_for_each_entry_reverse(bd, &tr->tr_ail1_list,
					    bd_ail_st_list) {
			bh = bd->bd_bh;
			fs_err(sdp, "bd %p: blk:0x%llx bh=%p ", bd,
			       (unsigned long long)bd->bd_blkno, bh);
			if (!bh) {
				fs_err(sdp, "\n");
				continue;
			}
			fs_err(sdp, "0x%llx up2:%d dirt:%d lkd:%d req:%d "
			       "map:%d new:%d ar:%d aw:%d delay:%d "
			       "io err:%d unwritten:%d dfr:%d pin:%d esc:%d\n",
			       (unsigned long long)bh->b_blocknr,
			       buffer_uptodate(bh), buffer_dirty(bh),
			       buffer_locked(bh), buffer_req(bh),
			       buffer_mapped(bh), buffer_new(bh),
			       buffer_async_read(bh), buffer_async_write(bh),
			       buffer_delay(bh), buffer_write_io_error(bh),
			       buffer_unwritten(bh),
			       buffer_defer_completion(bh),
			       buffer_pinned(bh), buffer_escaped(bh));
		}
	}
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * gfs2_ail1_flush - start writeback of some ail1 entries 
 * @sdp: The super block
 * @wbc: The writeback control structure
 *
 * Writes back some ail1 entries, according to the limits in the
 * writeback control structure
 */

void gfs2_ail1_flush(struct gfs2_sbd *sdp, struct writeback_control *wbc)
{
	struct list_head *head = &sdp->sd_ail1_list;
<<<<<<< HEAD
	struct gfs2_ail *ai;

	trace_gfs2_ail_flush(sdp, wbc, 1);
	spin_lock(&sdp->sd_ail_lock);
restart:
	list_for_each_entry_reverse(ai, head, ai_list) {
		if (wbc->nr_to_write <= 0)
			break;
		if (gfs2_ail1_start_one(sdp, wbc, ai))
			goto restart;
	}
	spin_unlock(&sdp->sd_ail_lock);
=======
	struct gfs2_trans *tr;
	struct blk_plug plug;
	int ret;
	unsigned long flush_start = jiffies;

	trace_gfs2_ail_flush(sdp, wbc, 1);
	blk_start_plug(&plug);
	spin_lock(&sdp->sd_ail_lock);
restart:
	ret = 0;
	if (time_after(jiffies, flush_start + (HZ * 600))) {
		fs_err(sdp, "Error: In %s for ten minutes! t=%d\n",
		       __func__, current->journal_info ? 1 : 0);
		dump_ail_list(sdp);
		goto out;
	}
	list_for_each_entry_reverse(tr, head, tr_list) {
		if (wbc->nr_to_write <= 0)
			break;
		ret = gfs2_ail1_start_one(sdp, wbc, tr, &plug);
		if (ret) {
			if (ret == -EBUSY)
				goto restart;
			break;
		}
	}
out:
	spin_unlock(&sdp->sd_ail_lock);
	blk_finish_plug(&plug);
	if (ret) {
		gfs2_lm(sdp, "gfs2_ail1_start_one returned: %d\n", ret);
		gfs2_withdraw(sdp);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	trace_gfs2_ail_flush(sdp, wbc, 0);
}

/**
 * gfs2_ail1_start - start writeback of all ail1 entries
 * @sdp: The superblock
 */

static void gfs2_ail1_start(struct gfs2_sbd *sdp)
{
	struct writeback_control wbc = {
		.sync_mode = WB_SYNC_NONE,
		.nr_to_write = LONG_MAX,
		.range_start = 0,
		.range_end = LLONG_MAX,
	};

	return gfs2_ail1_flush(sdp, &wbc);
}

<<<<<<< HEAD
/**
 * gfs2_ail1_empty_one - Check whether or not a trans in the AIL has been synced
 * @sdp: the filesystem
 * @ai: the AIL entry
 *
 */

static void gfs2_ail1_empty_one(struct gfs2_sbd *sdp, struct gfs2_ail *ai)
{
	struct gfs2_bufdata *bd, *s;
	struct buffer_head *bh;

	list_for_each_entry_safe_reverse(bd, s, &ai->ai_ail1_list,
					 bd_ail_st_list) {
		bh = bd->bd_bh;
		gfs2_assert(sdp, bd->bd_ail == ai);
		if (buffer_busy(bh))
			continue;
		if (!buffer_uptodate(bh))
			gfs2_io_error_bh(sdp, bh);
		list_move(&bd->bd_ail_st_list, &ai->ai_ail2_list);
	}

=======
static void gfs2_log_update_flush_tail(struct gfs2_sbd *sdp)
{
	unsigned int new_flush_tail = sdp->sd_log_head;
	struct gfs2_trans *tr;

	if (!list_empty(&sdp->sd_ail1_list)) {
		tr = list_last_entry(&sdp->sd_ail1_list,
				     struct gfs2_trans, tr_list);
		new_flush_tail = tr->tr_first;
	}
	sdp->sd_log_flush_tail = new_flush_tail;
}

static void gfs2_log_update_head(struct gfs2_sbd *sdp)
{
	unsigned int new_head = sdp->sd_log_flush_head;

	if (sdp->sd_log_flush_tail == sdp->sd_log_head)
		sdp->sd_log_flush_tail = new_head;
	sdp->sd_log_head = new_head;
}

/*
 * gfs2_ail_empty_tr - empty one of the ail lists of a transaction
 */

static void gfs2_ail_empty_tr(struct gfs2_sbd *sdp, struct gfs2_trans *tr,
			      struct list_head *head)
{
	struct gfs2_bufdata *bd;

	while (!list_empty(head)) {
		bd = list_first_entry(head, struct gfs2_bufdata,
				      bd_ail_st_list);
		gfs2_assert(sdp, bd->bd_tr == tr);
		gfs2_remove_from_ail(bd);
	}
}

/**
 * gfs2_ail1_empty_one - Check whether or not a trans in the AIL has been synced
 * @sdp: the filesystem
 * @tr: the transaction
 * @max_revokes: If nonzero, issue revokes for the bd items for written buffers
 *
 * returns: the transaction's count of remaining active items
 */

static int gfs2_ail1_empty_one(struct gfs2_sbd *sdp, struct gfs2_trans *tr,
				int *max_revokes)
{
	struct gfs2_bufdata *bd, *s;
	struct buffer_head *bh;
	int active_count = 0;

	list_for_each_entry_safe_reverse(bd, s, &tr->tr_ail1_list,
					 bd_ail_st_list) {
		bh = bd->bd_bh;
		gfs2_assert(sdp, bd->bd_tr == tr);
		/*
		 * If another process flagged an io error, e.g. writing to the
		 * journal, error all other bhs and move them off the ail1 to
		 * prevent a tight loop when unmount tries to flush ail1,
		 * regardless of whether they're still busy. If no outside
		 * errors were found and the buffer is busy, move to the next.
		 * If the ail buffer is not busy and caught an error, flag it
		 * for others.
		 */
		if (!sdp->sd_log_error && buffer_busy(bh)) {
			active_count++;
			continue;
		}
		if (!buffer_uptodate(bh) &&
		    !cmpxchg(&sdp->sd_log_error, 0, -EIO)) {
			gfs2_io_error_bh(sdp, bh);
			gfs2_withdraw_delayed(sdp);
		}
		/*
		 * If we have space for revokes and the bd is no longer on any
		 * buf list, we can just add a revoke for it immediately and
		 * avoid having to put it on the ail2 list, where it would need
		 * to be revoked later.
		 */
		if (*max_revokes && list_empty(&bd->bd_list)) {
			gfs2_add_revoke(sdp, bd);
			(*max_revokes)--;
			continue;
		}
		list_move(&bd->bd_ail_st_list, &tr->tr_ail2_list);
	}
	return active_count;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * gfs2_ail1_empty - Try to empty the ail1 lists
 * @sdp: The superblock
<<<<<<< HEAD
 *
 * Tries to empty the ail1 lists, starting with the oldest first
 */

static int gfs2_ail1_empty(struct gfs2_sbd *sdp)
{
	struct gfs2_ail *ai, *s;
	int ret;

	spin_lock(&sdp->sd_ail_lock);
	list_for_each_entry_safe_reverse(ai, s, &sdp->sd_ail1_list, ai_list) {
		gfs2_ail1_empty_one(sdp, ai);
		if (list_empty(&ai->ai_ail1_list))
			list_move(&ai->ai_list, &sdp->sd_ail2_list);
		else
			break;
	}
	ret = list_empty(&sdp->sd_ail1_list);
	spin_unlock(&sdp->sd_ail_lock);

	return ret;
=======
 * @max_revokes: If non-zero, add revokes where appropriate
 *
 * Tries to empty the ail1 lists, starting with the oldest first.
 * Returns %true if the ail1 list is now empty.
 */

static bool gfs2_ail1_empty(struct gfs2_sbd *sdp, int max_revokes)
{
	struct gfs2_trans *tr, *s;
	int oldest_tr = 1;
	bool empty;

	spin_lock(&sdp->sd_ail_lock);
	list_for_each_entry_safe_reverse(tr, s, &sdp->sd_ail1_list, tr_list) {
		if (!gfs2_ail1_empty_one(sdp, tr, &max_revokes) && oldest_tr)
			list_move(&tr->tr_list, &sdp->sd_ail2_list);
		else
			oldest_tr = 0;
	}
	gfs2_log_update_flush_tail(sdp);
	empty = list_empty(&sdp->sd_ail1_list);
	spin_unlock(&sdp->sd_ail_lock);

	return empty;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void gfs2_ail1_wait(struct gfs2_sbd *sdp)
{
<<<<<<< HEAD
	struct gfs2_ail *ai;
=======
	struct gfs2_trans *tr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct gfs2_bufdata *bd;
	struct buffer_head *bh;

	spin_lock(&sdp->sd_ail_lock);
<<<<<<< HEAD
	list_for_each_entry_reverse(ai, &sdp->sd_ail1_list, ai_list) {
		list_for_each_entry(bd, &ai->ai_ail1_list, bd_ail_st_list) {
=======
	list_for_each_entry_reverse(tr, &sdp->sd_ail1_list, tr_list) {
		list_for_each_entry(bd, &tr->tr_ail1_list, bd_ail_st_list) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			bh = bd->bd_bh;
			if (!buffer_locked(bh))
				continue;
			get_bh(bh);
			spin_unlock(&sdp->sd_ail_lock);
			wait_on_buffer(bh);
			brelse(bh);
			return;
		}
	}
	spin_unlock(&sdp->sd_ail_lock);
}

<<<<<<< HEAD
/**
 * gfs2_ail2_empty_one - Check whether or not a trans in the AIL has been synced
 * @sdp: the filesystem
 * @ai: the AIL entry
 *
 */

static void gfs2_ail2_empty_one(struct gfs2_sbd *sdp, struct gfs2_ail *ai)
{
	struct list_head *head = &ai->ai_ail2_list;
	struct gfs2_bufdata *bd;

	while (!list_empty(head)) {
		bd = list_entry(head->prev, struct gfs2_bufdata,
				bd_ail_st_list);
		gfs2_assert(sdp, bd->bd_ail == ai);
		gfs2_remove_from_ail(bd);
	}
=======
static void __ail2_empty(struct gfs2_sbd *sdp, struct gfs2_trans *tr)
{
	gfs2_ail_empty_tr(sdp, tr, &tr->tr_ail2_list);
	list_del(&tr->tr_list);
	gfs2_assert_warn(sdp, list_empty(&tr->tr_ail1_list));
	gfs2_assert_warn(sdp, list_empty(&tr->tr_ail2_list));
	gfs2_trans_free(sdp, tr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void ail2_empty(struct gfs2_sbd *sdp, unsigned int new_tail)
{
<<<<<<< HEAD
	struct gfs2_ail *ai, *safe;
	unsigned int old_tail = sdp->sd_log_tail;
	int wrap = (new_tail < old_tail);
	int a, b, rm;

	spin_lock(&sdp->sd_ail_lock);

	list_for_each_entry_safe(ai, safe, &sdp->sd_ail2_list, ai_list) {
		a = (old_tail <= ai->ai_first);
		b = (ai->ai_first < new_tail);
		rm = (wrap) ? (a || b) : (a && b);
		if (!rm)
			continue;

		gfs2_ail2_empty_one(sdp, ai);
		list_del(&ai->ai_list);
		gfs2_assert_warn(sdp, list_empty(&ai->ai_ail1_list));
		gfs2_assert_warn(sdp, list_empty(&ai->ai_ail2_list));
		kfree(ai);
	}

=======
	struct list_head *ail2_list = &sdp->sd_ail2_list;
	unsigned int old_tail = sdp->sd_log_tail;
	struct gfs2_trans *tr, *safe;

	spin_lock(&sdp->sd_ail_lock);
	if (old_tail <= new_tail) {
		list_for_each_entry_safe(tr, safe, ail2_list, tr_list) {
			if (old_tail <= tr->tr_first && tr->tr_first < new_tail)
				__ail2_empty(sdp, tr);
		}
	} else {
		list_for_each_entry_safe(tr, safe, ail2_list, tr_list) {
			if (old_tail <= tr->tr_first || tr->tr_first < new_tail)
				__ail2_empty(sdp, tr);
		}
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_unlock(&sdp->sd_ail_lock);
}

/**
<<<<<<< HEAD
 * gfs2_log_reserve - Make a log reservation
 * @sdp: The GFS2 superblock
 * @blks: The number of blocks to reserve
 *
 * Note that we never give out the last few blocks of the journal. Thats
 * due to the fact that there is a small number of header blocks
 * associated with each log flush. The exact number can't be known until
 * flush time, so we ensure that we have just enough free blocks at all
 * times to avoid running out during a log flush.
=======
 * gfs2_log_is_empty - Check if the log is empty
 * @sdp: The GFS2 superblock
 */

bool gfs2_log_is_empty(struct gfs2_sbd *sdp) {
	return atomic_read(&sdp->sd_log_blks_free) == sdp->sd_jdesc->jd_blocks;
}

static bool __gfs2_log_try_reserve_revokes(struct gfs2_sbd *sdp, unsigned int revokes)
{
	unsigned int available;

	available = atomic_read(&sdp->sd_log_revokes_available);
	while (available >= revokes) {
		if (atomic_try_cmpxchg(&sdp->sd_log_revokes_available,
				       &available, available - revokes))
			return true;
	}
	return false;
}

/**
 * gfs2_log_release_revokes - Release a given number of revokes
 * @sdp: The GFS2 superblock
 * @revokes: The number of revokes to release
 *
 * sdp->sd_log_flush_lock must be held.
 */
void gfs2_log_release_revokes(struct gfs2_sbd *sdp, unsigned int revokes)
{
	if (revokes)
		atomic_add(revokes, &sdp->sd_log_revokes_available);
}

/**
 * gfs2_log_release - Release a given number of log blocks
 * @sdp: The GFS2 superblock
 * @blks: The number of blocks
 *
 */

void gfs2_log_release(struct gfs2_sbd *sdp, unsigned int blks)
{
	atomic_add(blks, &sdp->sd_log_blks_free);
	trace_gfs2_log_blocks(sdp, blks);
	gfs2_assert_withdraw(sdp, atomic_read(&sdp->sd_log_blks_free) <=
				  sdp->sd_jdesc->jd_blocks);
	if (atomic_read(&sdp->sd_log_blks_needed))
		wake_up(&sdp->sd_log_waitq);
}

/**
 * __gfs2_log_try_reserve - Try to make a log reservation
 * @sdp: The GFS2 superblock
 * @blks: The number of blocks to reserve
 * @taboo_blks: The number of blocks to leave free
 *
 * Try to do the same as __gfs2_log_reserve(), but fail if no more log
 * space is immediately available.
 */
static bool __gfs2_log_try_reserve(struct gfs2_sbd *sdp, unsigned int blks,
				   unsigned int taboo_blks)
{
	unsigned wanted = blks + taboo_blks;
	unsigned int free_blocks;

	free_blocks = atomic_read(&sdp->sd_log_blks_free);
	while (free_blocks >= wanted) {
		if (atomic_try_cmpxchg(&sdp->sd_log_blks_free, &free_blocks,
				       free_blocks - blks)) {
			trace_gfs2_log_blocks(sdp, -blks);
			return true;
		}
	}
	return false;
}

/**
 * __gfs2_log_reserve - Make a log reservation
 * @sdp: The GFS2 superblock
 * @blks: The number of blocks to reserve
 * @taboo_blks: The number of blocks to leave free
 *
 * @taboo_blks is set to 0 for logd, and to GFS2_LOG_FLUSH_MIN_BLOCKS
 * for all other processes.  This ensures that when the log is almost full,
 * logd will still be able to call gfs2_log_flush one more time  without
 * blocking, which will advance the tail and make some more log space
 * available.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * We no longer flush the log here, instead we wake up logd to do that
 * for us. To avoid the thundering herd and to ensure that we deal fairly
 * with queued waiters, we use an exclusive wait. This means that when we
 * get woken with enough journal space to get our reservation, we need to
 * wake the next waiter on the list.
<<<<<<< HEAD
 *
 * Returns: errno
 */

int gfs2_log_reserve(struct gfs2_sbd *sdp, unsigned int blks)
{
	unsigned reserved_blks = 6 * (4096 / sdp->sd_vfs->s_blocksize);
	unsigned wanted = blks + reserved_blks;
	DEFINE_WAIT(wait);
	int did_wait = 0;
	unsigned int free_blocks;

	if (gfs2_assert_warn(sdp, blks) ||
	    gfs2_assert_warn(sdp, blks <= sdp->sd_jdesc->jd_blocks))
		return -EINVAL;
retry:
	free_blocks = atomic_read(&sdp->sd_log_blks_free);
	if (unlikely(free_blocks <= wanted)) {
		do {
			prepare_to_wait_exclusive(&sdp->sd_log_waitq, &wait,
					TASK_UNINTERRUPTIBLE);
			wake_up(&sdp->sd_logd_waitq);
			did_wait = 1;
			if (atomic_read(&sdp->sd_log_blks_free) <= wanted)
				io_schedule();
			free_blocks = atomic_read(&sdp->sd_log_blks_free);
		} while(free_blocks <= wanted);
		finish_wait(&sdp->sd_log_waitq, &wait);
	}
	if (atomic_cmpxchg(&sdp->sd_log_blks_free, free_blocks,
				free_blocks - blks) != free_blocks)
		goto retry;
	trace_gfs2_log_blocks(sdp, -blks);

	/*
	 * If we waited, then so might others, wake them up _after_ we get
	 * our share of the log.
	 */
	if (unlikely(did_wait))
		wake_up(&sdp->sd_log_waitq);

	down_read(&sdp->sd_log_flush_lock);

	return 0;
}

u64 gfs2_log_bmap(struct gfs2_sbd *sdp, unsigned int lbn)
{
	struct gfs2_journal_extent *je;

	list_for_each_entry(je, &sdp->sd_jdesc->extent_list, extent_list) {
		if (lbn >= je->lblock && lbn < je->lblock + je->blocks)
			return je->dblock + lbn - je->lblock;
	}

	return -1;
=======
 */

static void __gfs2_log_reserve(struct gfs2_sbd *sdp, unsigned int blks,
			       unsigned int taboo_blks)
{
	unsigned wanted = blks + taboo_blks;
	unsigned int free_blocks;

	atomic_add(blks, &sdp->sd_log_blks_needed);
	for (;;) {
		if (current != sdp->sd_logd_process)
			wake_up(&sdp->sd_logd_waitq);
		io_wait_event(sdp->sd_log_waitq,
			(free_blocks = atomic_read(&sdp->sd_log_blks_free),
			 free_blocks >= wanted));
		do {
			if (atomic_try_cmpxchg(&sdp->sd_log_blks_free,
					       &free_blocks,
					       free_blocks - blks))
				goto reserved;
		} while (free_blocks >= wanted);
	}

reserved:
	trace_gfs2_log_blocks(sdp, -blks);
	if (atomic_sub_return(blks, &sdp->sd_log_blks_needed))
		wake_up(&sdp->sd_log_waitq);
}

/**
 * gfs2_log_try_reserve - Try to make a log reservation
 * @sdp: The GFS2 superblock
 * @tr: The transaction
 * @extra_revokes: The number of additional revokes reserved (output)
 *
 * This is similar to gfs2_log_reserve, but sdp->sd_log_flush_lock must be
 * held for correct revoke accounting.
 */

bool gfs2_log_try_reserve(struct gfs2_sbd *sdp, struct gfs2_trans *tr,
			  unsigned int *extra_revokes)
{
	unsigned int blks = tr->tr_reserved;
	unsigned int revokes = tr->tr_revokes;
	unsigned int revoke_blks = 0;

	*extra_revokes = 0;
	if (revokes && !__gfs2_log_try_reserve_revokes(sdp, revokes)) {
		revoke_blks = DIV_ROUND_UP(revokes, sdp->sd_inptrs);
		*extra_revokes = revoke_blks * sdp->sd_inptrs - revokes;
		blks += revoke_blks;
	}
	if (!blks)
		return true;
	if (__gfs2_log_try_reserve(sdp, blks, GFS2_LOG_FLUSH_MIN_BLOCKS))
		return true;
	if (!revoke_blks)
		gfs2_log_release_revokes(sdp, revokes);
	return false;
}

/**
 * gfs2_log_reserve - Make a log reservation
 * @sdp: The GFS2 superblock
 * @tr: The transaction
 * @extra_revokes: The number of additional revokes reserved (output)
 *
 * sdp->sd_log_flush_lock must not be held.
 */

void gfs2_log_reserve(struct gfs2_sbd *sdp, struct gfs2_trans *tr,
		      unsigned int *extra_revokes)
{
	unsigned int blks = tr->tr_reserved;
	unsigned int revokes = tr->tr_revokes;
	unsigned int revoke_blks;

	*extra_revokes = 0;
	if (revokes) {
		revoke_blks = DIV_ROUND_UP(revokes, sdp->sd_inptrs);
		*extra_revokes = revoke_blks * sdp->sd_inptrs - revokes;
		blks += revoke_blks;
	}
	__gfs2_log_reserve(sdp, blks, GFS2_LOG_FLUSH_MIN_BLOCKS);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * log_distance - Compute distance between two journal blocks
 * @sdp: The GFS2 superblock
 * @newer: The most recent journal block of the pair
 * @older: The older journal block of the pair
 *
 *   Compute the distance (in the journal direction) between two
 *   blocks in the journal
 *
 * Returns: the distance in blocks
 */

static inline unsigned int log_distance(struct gfs2_sbd *sdp, unsigned int newer,
					unsigned int older)
{
	int dist;

	dist = newer - older;
	if (dist < 0)
		dist += sdp->sd_jdesc->jd_blocks;

	return dist;
}

/**
<<<<<<< HEAD
 * calc_reserved - Calculate the number of blocks to reserve when
 *                 refunding a transaction's unused buffers.
 * @sdp: The GFS2 superblock
 *
 * This is complex.  We need to reserve room for all our currently used
 * metadata buffers (e.g. normal file I/O rewriting file time stamps) and 
 * all our journaled data buffers for journaled files (e.g. files in the 
 * meta_fs like rindex, or files for which chattr +j was done.)
 * If we don't reserve enough space, gfs2_log_refund and gfs2_log_flush
 * will count it as free space (sd_log_blks_free) and corruption will follow.
 *
 * We can have metadata bufs and jdata bufs in the same journal.  So each
 * type gets its own log header, for which we need to reserve a block.
 * In fact, each type has the potential for needing more than one header 
 * in cases where we have more buffers than will fit on a journal page.
 * Metadata journal entries take up half the space of journaled buffer entries.
 * Thus, metadata entries have buf_limit (502) and journaled buffers have
 * databuf_limit (251) before they cause a wrap around.
=======
 * calc_reserved - Calculate the number of blocks to keep reserved
 * @sdp: The GFS2 superblock
 *
 * This is complex.  We need to reserve room for all our currently used
 * metadata blocks (e.g. normal file I/O rewriting file time stamps) and
 * all our journaled data blocks for journaled files (e.g. files in the
 * meta_fs like rindex, or files for which chattr +j was done.)
 * If we don't reserve enough space, corruption will follow.
 *
 * We can have metadata blocks and jdata blocks in the same journal.  Each
 * type gets its own log descriptor, for which we need to reserve a block.
 * In fact, each type has the potential for needing more than one log descriptor
 * in cases where we have more blocks than will fit in a log descriptor.
 * Metadata journal entries take up half the space of journaled buffer entries.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Also, we need to reserve blocks for revoke journal entries and one for an
 * overall header for the lot.
 *
 * Returns: the number of blocks reserved
 */
static unsigned int calc_reserved(struct gfs2_sbd *sdp)
{
<<<<<<< HEAD
	unsigned int reserved = 0;
	unsigned int mbuf_limit, metabufhdrs_needed;
	unsigned int dbuf_limit, databufhdrs_needed;
	unsigned int revokes = 0;

	mbuf_limit = buf_limit(sdp);
	metabufhdrs_needed = (sdp->sd_log_commited_buf +
			      (mbuf_limit - 1)) / mbuf_limit;
	dbuf_limit = databuf_limit(sdp);
	databufhdrs_needed = (sdp->sd_log_commited_databuf +
			      (dbuf_limit - 1)) / dbuf_limit;

	if (sdp->sd_log_commited_revoke > 0)
		revokes = gfs2_struct2blk(sdp, sdp->sd_log_commited_revoke,
					  sizeof(u64));

	reserved = sdp->sd_log_commited_buf + metabufhdrs_needed +
		sdp->sd_log_commited_databuf + databufhdrs_needed +
		revokes;
	/* One for the overall header */
	if (reserved)
		reserved++;
	return reserved;
}

static unsigned int current_tail(struct gfs2_sbd *sdp)
{
	struct gfs2_ail *ai;
	unsigned int tail;

	spin_lock(&sdp->sd_ail_lock);

	if (list_empty(&sdp->sd_ail1_list)) {
		tail = sdp->sd_log_head;
	} else {
		ai = list_entry(sdp->sd_ail1_list.prev, struct gfs2_ail, ai_list);
		tail = ai->ai_first;
	}

	spin_unlock(&sdp->sd_ail_lock);

	return tail;
}

void gfs2_log_incr_head(struct gfs2_sbd *sdp)
{
	BUG_ON((sdp->sd_log_flush_head == sdp->sd_log_tail) &&
	       (sdp->sd_log_flush_head != sdp->sd_log_head));

	if (++sdp->sd_log_flush_head == sdp->sd_jdesc->jd_blocks) {
		sdp->sd_log_flush_head = 0;
		sdp->sd_log_flush_wrapped = 1;
	}
}

static void log_pull_tail(struct gfs2_sbd *sdp, unsigned int new_tail)
{
	unsigned int dist = log_distance(sdp, new_tail, sdp->sd_log_tail);

	ail2_empty(sdp, new_tail);

	atomic_add(dist, &sdp->sd_log_blks_free);
	trace_gfs2_log_blocks(sdp, dist);
	gfs2_assert_withdraw(sdp, atomic_read(&sdp->sd_log_blks_free) <=
			     sdp->sd_jdesc->jd_blocks);

=======
	unsigned int reserved = GFS2_LOG_FLUSH_MIN_BLOCKS;
	unsigned int blocks;
	struct gfs2_trans *tr = sdp->sd_log_tr;

	if (tr) {
		blocks = tr->tr_num_buf_new - tr->tr_num_buf_rm;
		reserved += blocks + DIV_ROUND_UP(blocks, buf_limit(sdp));
		blocks = tr->tr_num_databuf_new - tr->tr_num_databuf_rm;
		reserved += blocks + DIV_ROUND_UP(blocks, databuf_limit(sdp));
	}
	return reserved;
}

static void log_pull_tail(struct gfs2_sbd *sdp)
{
	unsigned int new_tail = sdp->sd_log_flush_tail;
	unsigned int dist;

	if (new_tail == sdp->sd_log_tail)
		return;
	dist = log_distance(sdp, new_tail, sdp->sd_log_tail);
	ail2_empty(sdp, new_tail);
	gfs2_log_release(sdp, dist);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	sdp->sd_log_tail = new_tail;
}


<<<<<<< HEAD
static void log_flush_wait(struct gfs2_sbd *sdp)
=======
void log_flush_wait(struct gfs2_sbd *sdp)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	DEFINE_WAIT(wait);

	if (atomic_read(&sdp->sd_log_in_flight)) {
		do {
			prepare_to_wait(&sdp->sd_log_flush_wait, &wait,
					TASK_UNINTERRUPTIBLE);
			if (atomic_read(&sdp->sd_log_in_flight))
				io_schedule();
		} while(atomic_read(&sdp->sd_log_in_flight));
		finish_wait(&sdp->sd_log_flush_wait, &wait);
	}
}

<<<<<<< HEAD
static int bd_cmp(void *priv, struct list_head *a, struct list_head *b)
{
	struct gfs2_bufdata *bda, *bdb;

	bda = list_entry(a, struct gfs2_bufdata, bd_le.le_list);
	bdb = list_entry(b, struct gfs2_bufdata, bd_le.le_list);

	if (bda->bd_bh->b_blocknr < bdb->bd_bh->b_blocknr)
		return -1;
	if (bda->bd_bh->b_blocknr > bdb->bd_bh->b_blocknr)
=======
static int ip_cmp(void *priv, const struct list_head *a, const struct list_head *b)
{
	struct gfs2_inode *ipa, *ipb;

	ipa = list_entry(a, struct gfs2_inode, i_ordered);
	ipb = list_entry(b, struct gfs2_inode, i_ordered);

	if (ipa->i_no_addr < ipb->i_no_addr)
		return -1;
	if (ipa->i_no_addr > ipb->i_no_addr)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 1;
	return 0;
}

<<<<<<< HEAD
static void gfs2_ordered_write(struct gfs2_sbd *sdp)
{
	struct gfs2_bufdata *bd;
	struct buffer_head *bh;
	LIST_HEAD(written);

	gfs2_log_lock(sdp);
	list_sort(NULL, &sdp->sd_log_le_ordered, &bd_cmp);
	while (!list_empty(&sdp->sd_log_le_ordered)) {
		bd = list_entry(sdp->sd_log_le_ordered.next, struct gfs2_bufdata, bd_le.le_list);
		list_move(&bd->bd_le.le_list, &written);
		bh = bd->bd_bh;
		if (!buffer_dirty(bh))
			continue;
		get_bh(bh);
		gfs2_log_unlock(sdp);
		lock_buffer(bh);
		if (buffer_mapped(bh) && test_clear_buffer_dirty(bh)) {
			bh->b_end_io = end_buffer_write_sync;
			submit_bh(WRITE_SYNC, bh);
		} else {
			unlock_buffer(bh);
			brelse(bh);
		}
		gfs2_log_lock(sdp);
	}
	list_splice(&written, &sdp->sd_log_le_ordered);
	gfs2_log_unlock(sdp);
=======
static void __ordered_del_inode(struct gfs2_inode *ip)
{
	if (!list_empty(&ip->i_ordered))
		list_del_init(&ip->i_ordered);
}

static void gfs2_ordered_write(struct gfs2_sbd *sdp)
{
	struct gfs2_inode *ip;
	LIST_HEAD(written);

	spin_lock(&sdp->sd_ordered_lock);
	list_sort(NULL, &sdp->sd_log_ordered, &ip_cmp);
	while (!list_empty(&sdp->sd_log_ordered)) {
		ip = list_first_entry(&sdp->sd_log_ordered, struct gfs2_inode, i_ordered);
		if (ip->i_inode.i_mapping->nrpages == 0) {
			__ordered_del_inode(ip);
			continue;
		}
		list_move(&ip->i_ordered, &written);
		spin_unlock(&sdp->sd_ordered_lock);
		filemap_fdatawrite(ip->i_inode.i_mapping);
		spin_lock(&sdp->sd_ordered_lock);
	}
	list_splice(&written, &sdp->sd_log_ordered);
	spin_unlock(&sdp->sd_ordered_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void gfs2_ordered_wait(struct gfs2_sbd *sdp)
{
<<<<<<< HEAD
	struct gfs2_bufdata *bd;
	struct buffer_head *bh;

	gfs2_log_lock(sdp);
	while (!list_empty(&sdp->sd_log_le_ordered)) {
		bd = list_entry(sdp->sd_log_le_ordered.prev, struct gfs2_bufdata, bd_le.le_list);
		bh = bd->bd_bh;
		if (buffer_locked(bh)) {
			get_bh(bh);
			gfs2_log_unlock(sdp);
			wait_on_buffer(bh);
			brelse(bh);
			gfs2_log_lock(sdp);
			continue;
		}
		list_del_init(&bd->bd_le.le_list);
	}
	gfs2_log_unlock(sdp);
}

/**
 * log_write_header - Get and initialize a journal header buffer
 * @sdp: The GFS2 superblock
 *
 * Returns: the initialized log buffer descriptor
 */

static void log_write_header(struct gfs2_sbd *sdp, u32 flags, int pull)
{
	u64 blkno = gfs2_log_bmap(sdp, sdp->sd_log_flush_head);
	struct buffer_head *bh;
	struct gfs2_log_header *lh;
	unsigned int tail;
	u32 hash;

	bh = sb_getblk(sdp->sd_vfs, blkno);
	lock_buffer(bh);
	memset(bh->b_data, 0, bh->b_size);
	set_buffer_uptodate(bh);
	clear_buffer_dirty(bh);

	gfs2_ail1_empty(sdp);
	tail = current_tail(sdp);

	lh = (struct gfs2_log_header *)bh->b_data;
	memset(lh, 0, sizeof(struct gfs2_log_header));
=======
	struct gfs2_inode *ip;

	spin_lock(&sdp->sd_ordered_lock);
	while (!list_empty(&sdp->sd_log_ordered)) {
		ip = list_first_entry(&sdp->sd_log_ordered, struct gfs2_inode, i_ordered);
		__ordered_del_inode(ip);
		if (ip->i_inode.i_mapping->nrpages == 0)
			continue;
		spin_unlock(&sdp->sd_ordered_lock);
		filemap_fdatawait(ip->i_inode.i_mapping);
		spin_lock(&sdp->sd_ordered_lock);
	}
	spin_unlock(&sdp->sd_ordered_lock);
}

void gfs2_ordered_del_inode(struct gfs2_inode *ip)
{
	struct gfs2_sbd *sdp = GFS2_SB(&ip->i_inode);

	spin_lock(&sdp->sd_ordered_lock);
	__ordered_del_inode(ip);
	spin_unlock(&sdp->sd_ordered_lock);
}

void gfs2_add_revoke(struct gfs2_sbd *sdp, struct gfs2_bufdata *bd)
{
	struct buffer_head *bh = bd->bd_bh;
	struct gfs2_glock *gl = bd->bd_gl;

	sdp->sd_log_num_revoke++;
	if (atomic_inc_return(&gl->gl_revokes) == 1)
		gfs2_glock_hold(gl);
	bh->b_private = NULL;
	bd->bd_blkno = bh->b_blocknr;
	gfs2_remove_from_ail(bd); /* drops ref on bh */
	bd->bd_bh = NULL;
	set_bit(GLF_LFLUSH, &gl->gl_flags);
	list_add(&bd->bd_list, &sdp->sd_log_revokes);
}

void gfs2_glock_remove_revoke(struct gfs2_glock *gl)
{
	if (atomic_dec_return(&gl->gl_revokes) == 0) {
		clear_bit(GLF_LFLUSH, &gl->gl_flags);
		gfs2_glock_queue_put(gl);
	}
}

/**
 * gfs2_flush_revokes - Add as many revokes to the system transaction as we can
 * @sdp: The GFS2 superblock
 *
 * Our usual strategy is to defer writing revokes as much as we can in the hope
 * that we'll eventually overwrite the journal, which will make those revokes
 * go away.  This changes when we flush the log: at that point, there will
 * likely be some left-over space in the last revoke block of that transaction.
 * We can fill that space with additional revokes for blocks that have already
 * been written back.  This will basically come at no cost now, and will save
 * us from having to keep track of those blocks on the AIL2 list later.
 */
void gfs2_flush_revokes(struct gfs2_sbd *sdp)
{
	/* number of revokes we still have room for */
	unsigned int max_revokes = atomic_read(&sdp->sd_log_revokes_available);

	gfs2_log_lock(sdp);
	gfs2_ail1_empty(sdp, max_revokes);
	gfs2_log_unlock(sdp);

	if (gfs2_withdrawing(sdp))
		gfs2_withdraw(sdp);
}

/**
 * gfs2_write_log_header - Write a journal log header buffer at lblock
 * @sdp: The GFS2 superblock
 * @jd: journal descriptor of the journal to which we are writing
 * @seq: sequence number
 * @tail: tail of the log
 * @lblock: value for lh_blkno (block number relative to start of journal)
 * @flags: log header flags GFS2_LOG_HEAD_*
 * @op_flags: flags to pass to the bio
 *
 * Returns: the initialized log buffer descriptor
 */

void gfs2_write_log_header(struct gfs2_sbd *sdp, struct gfs2_jdesc *jd,
			   u64 seq, u32 tail, u32 lblock, u32 flags,
			   blk_opf_t op_flags)
{
	struct gfs2_log_header *lh;
	u32 hash, crc;
	struct page *page;
	struct gfs2_statfs_change_host *l_sc = &sdp->sd_statfs_local;
	struct timespec64 tv;
	struct super_block *sb = sdp->sd_vfs;
	u64 dblock;

	if (gfs2_withdrawing_or_withdrawn(sdp))
		return;

	page = mempool_alloc(gfs2_page_pool, GFP_NOIO);
	lh = page_address(page);
	clear_page(lh);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	lh->lh_header.mh_magic = cpu_to_be32(GFS2_MAGIC);
	lh->lh_header.mh_type = cpu_to_be32(GFS2_METATYPE_LH);
	lh->lh_header.__pad0 = cpu_to_be64(0);
	lh->lh_header.mh_format = cpu_to_be32(GFS2_FORMAT_LH);
	lh->lh_header.mh_jid = cpu_to_be32(sdp->sd_jdesc->jd_jid);
<<<<<<< HEAD
	lh->lh_sequence = cpu_to_be64(sdp->sd_log_sequence++);
	lh->lh_flags = cpu_to_be32(flags);
	lh->lh_tail = cpu_to_be32(tail);
	lh->lh_blkno = cpu_to_be32(sdp->sd_log_flush_head);
	hash = gfs2_disk_hash(bh->b_data, sizeof(struct gfs2_log_header));
	lh->lh_hash = cpu_to_be32(hash);

	bh->b_end_io = end_buffer_write_sync;
	get_bh(bh);
	if (test_bit(SDF_NOBARRIERS, &sdp->sd_flags)) {
		gfs2_ordered_wait(sdp);
		log_flush_wait(sdp);
		submit_bh(WRITE_SYNC | REQ_META | REQ_PRIO, bh);
	} else {
		submit_bh(WRITE_FLUSH_FUA | REQ_META, bh);
	}
	wait_on_buffer(bh);

	if (!buffer_uptodate(bh))
		gfs2_io_error_bh(sdp, bh);
	brelse(bh);

	if (sdp->sd_log_tail != tail)
		log_pull_tail(sdp, tail);
	else
		gfs2_assert_withdraw(sdp, !pull);

	sdp->sd_log_idle = (tail == sdp->sd_log_flush_head);
	gfs2_log_incr_head(sdp);
=======
	lh->lh_sequence = cpu_to_be64(seq);
	lh->lh_flags = cpu_to_be32(flags);
	lh->lh_tail = cpu_to_be32(tail);
	lh->lh_blkno = cpu_to_be32(lblock);
	hash = ~crc32(~0, lh, LH_V1_SIZE);
	lh->lh_hash = cpu_to_be32(hash);

	ktime_get_coarse_real_ts64(&tv);
	lh->lh_nsec = cpu_to_be32(tv.tv_nsec);
	lh->lh_sec = cpu_to_be64(tv.tv_sec);
	if (!list_empty(&jd->extent_list))
		dblock = gfs2_log_bmap(jd, lblock);
	else {
		unsigned int extlen;
		int ret;

		extlen = 1;
		ret = gfs2_get_extent(jd->jd_inode, lblock, &dblock, &extlen);
		if (gfs2_assert_withdraw(sdp, ret == 0))
			return;
	}
	lh->lh_addr = cpu_to_be64(dblock);
	lh->lh_jinode = cpu_to_be64(GFS2_I(jd->jd_inode)->i_no_addr);

	/* We may only write local statfs, quota, etc., when writing to our
	   own journal. The values are left 0 when recovering a journal
	   different from our own. */
	if (!(flags & GFS2_LOG_HEAD_RECOVERY)) {
		lh->lh_statfs_addr =
			cpu_to_be64(GFS2_I(sdp->sd_sc_inode)->i_no_addr);
		lh->lh_quota_addr =
			cpu_to_be64(GFS2_I(sdp->sd_qc_inode)->i_no_addr);

		spin_lock(&sdp->sd_statfs_spin);
		lh->lh_local_total = cpu_to_be64(l_sc->sc_total);
		lh->lh_local_free = cpu_to_be64(l_sc->sc_free);
		lh->lh_local_dinodes = cpu_to_be64(l_sc->sc_dinodes);
		spin_unlock(&sdp->sd_statfs_spin);
	}

	BUILD_BUG_ON(offsetof(struct gfs2_log_header, lh_crc) != LH_V1_SIZE);

	crc = crc32c(~0, (void *)lh + LH_V1_SIZE + 4,
		     sb->s_blocksize - LH_V1_SIZE - 4);
	lh->lh_crc = cpu_to_be32(crc);

	gfs2_log_write(sdp, jd, page, sb->s_blocksize, 0, dblock);
	gfs2_log_submit_bio(&jd->jd_log_bio, REQ_OP_WRITE | op_flags);
}

/**
 * log_write_header - Get and initialize a journal header buffer
 * @sdp: The GFS2 superblock
 * @flags: The log header flags, including log header origin
 *
 * Returns: the initialized log buffer descriptor
 */

static void log_write_header(struct gfs2_sbd *sdp, u32 flags)
{
	blk_opf_t op_flags = REQ_PREFLUSH | REQ_FUA | REQ_META | REQ_SYNC;
	struct super_block *sb = sdp->sd_vfs;

	gfs2_assert_withdraw(sdp, sb->s_writers.frozen != SB_FREEZE_COMPLETE);

	if (test_bit(SDF_NOBARRIERS, &sdp->sd_flags)) {
		gfs2_ordered_wait(sdp);
		log_flush_wait(sdp);
		op_flags = REQ_SYNC | REQ_META | REQ_PRIO;
	}
	sdp->sd_log_idle = (sdp->sd_log_flush_tail == sdp->sd_log_flush_head);
	gfs2_write_log_header(sdp, sdp->sd_jdesc, sdp->sd_log_sequence++,
			      sdp->sd_log_flush_tail, sdp->sd_log_flush_head,
			      flags, op_flags);
	gfs2_log_incr_head(sdp);
	log_flush_wait(sdp);
	log_pull_tail(sdp);
	gfs2_log_update_head(sdp);
}

/**
 * gfs2_ail_drain - drain the ail lists after a withdraw
 * @sdp: Pointer to GFS2 superblock
 */
void gfs2_ail_drain(struct gfs2_sbd *sdp)
{
	struct gfs2_trans *tr;

	spin_lock(&sdp->sd_ail_lock);
	/*
	 * For transactions on the sd_ail1_list we need to drain both the
	 * ail1 and ail2 lists. That's because function gfs2_ail1_start_one
	 * (temporarily) moves items from its tr_ail1 list to tr_ail2 list
	 * before revokes are sent for that block. Items on the sd_ail2_list
	 * should have already gotten beyond that point, so no need.
	 */
	while (!list_empty(&sdp->sd_ail1_list)) {
		tr = list_first_entry(&sdp->sd_ail1_list, struct gfs2_trans,
				      tr_list);
		gfs2_ail_empty_tr(sdp, tr, &tr->tr_ail1_list);
		gfs2_ail_empty_tr(sdp, tr, &tr->tr_ail2_list);
		list_del(&tr->tr_list);
		gfs2_trans_free(sdp, tr);
	}
	while (!list_empty(&sdp->sd_ail2_list)) {
		tr = list_first_entry(&sdp->sd_ail2_list, struct gfs2_trans,
				      tr_list);
		gfs2_ail_empty_tr(sdp, tr, &tr->tr_ail2_list);
		list_del(&tr->tr_list);
		gfs2_trans_free(sdp, tr);
	}
	gfs2_drain_revokes(sdp);
	spin_unlock(&sdp->sd_ail_lock);
}

/**
 * empty_ail1_list - try to start IO and empty the ail1 list
 * @sdp: Pointer to GFS2 superblock
 */
static void empty_ail1_list(struct gfs2_sbd *sdp)
{
	unsigned long start = jiffies;
	bool empty = false;

	while (!empty) {
		if (time_after(jiffies, start + (HZ * 600))) {
			fs_err(sdp, "Error: In %s for 10 minutes! t=%d\n",
			       __func__, current->journal_info ? 1 : 0);
			dump_ail_list(sdp);
			return;
		}
		gfs2_ail1_start(sdp);
		gfs2_ail1_wait(sdp);
		empty = gfs2_ail1_empty(sdp, 0);

		if (gfs2_withdrawing_or_withdrawn(sdp))
			break;
	}

	if (gfs2_withdrawing(sdp))
		gfs2_withdraw(sdp);
}

/**
 * trans_drain - drain the buf and databuf queue for a failed transaction
 * @tr: the transaction to drain
 *
 * When this is called, we're taking an error exit for a log write that failed
 * but since we bypassed the after_commit functions, we need to remove the
 * items from the buf and databuf queue.
 */
static void trans_drain(struct gfs2_trans *tr)
{
	struct gfs2_bufdata *bd;
	struct list_head *head;

	if (!tr)
		return;

	head = &tr->tr_buf;
	while (!list_empty(head)) {
		bd = list_first_entry(head, struct gfs2_bufdata, bd_list);
		list_del_init(&bd->bd_list);
		if (!list_empty(&bd->bd_ail_st_list))
			gfs2_remove_from_ail(bd);
		kmem_cache_free(gfs2_bufdata_cachep, bd);
	}
	head = &tr->tr_databuf;
	while (!list_empty(head)) {
		bd = list_first_entry(head, struct gfs2_bufdata, bd_list);
		list_del_init(&bd->bd_list);
		if (!list_empty(&bd->bd_ail_st_list))
			gfs2_remove_from_ail(bd);
		kmem_cache_free(gfs2_bufdata_cachep, bd);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * gfs2_log_flush - flush incore transaction(s)
<<<<<<< HEAD
 * @sdp: the filesystem
 * @gl: The glock structure to flush.  If NULL, flush the whole incore log
 *
 */

void gfs2_log_flush(struct gfs2_sbd *sdp, struct gfs2_glock *gl)
{
	struct gfs2_ail *ai;

	down_write(&sdp->sd_log_flush_lock);

	/* Log might have been flushed while we waited for the flush lock */
	if (gl && !test_bit(GLF_LFLUSH, &gl->gl_flags)) {
		up_write(&sdp->sd_log_flush_lock);
		return;
	}
	trace_gfs2_log_flush(sdp, 1);

	ai = kzalloc(sizeof(struct gfs2_ail), GFP_NOFS | __GFP_NOFAIL);
	INIT_LIST_HEAD(&ai->ai_ail1_list);
	INIT_LIST_HEAD(&ai->ai_ail2_list);

	if (sdp->sd_log_num_buf != sdp->sd_log_commited_buf) {
		printk(KERN_INFO "GFS2: log buf %u %u\n", sdp->sd_log_num_buf,
		       sdp->sd_log_commited_buf);
		gfs2_assert_withdraw(sdp, 0);
	}
	if (sdp->sd_log_num_databuf != sdp->sd_log_commited_databuf) {
		printk(KERN_INFO "GFS2: log databuf %u %u\n",
		       sdp->sd_log_num_databuf, sdp->sd_log_commited_databuf);
		gfs2_assert_withdraw(sdp, 0);
	}
	gfs2_assert_withdraw(sdp,
			sdp->sd_log_num_revoke == sdp->sd_log_commited_revoke);

	sdp->sd_log_flush_head = sdp->sd_log_head;
	sdp->sd_log_flush_wrapped = 0;
	ai->ai_first = sdp->sd_log_flush_head;

	gfs2_ordered_write(sdp);
	lops_before_commit(sdp);

	if (sdp->sd_log_head != sdp->sd_log_flush_head) {
		log_write_header(sdp, 0, 0);
	} else if (sdp->sd_log_tail != current_tail(sdp) && !sdp->sd_log_idle){
		gfs2_log_lock(sdp);
		atomic_dec(&sdp->sd_log_blks_free); /* Adjust for unreserved buffer */
		trace_gfs2_log_blocks(sdp, -1);
		gfs2_log_unlock(sdp);
		log_write_header(sdp, 0, PULL);
	}
	lops_after_commit(sdp, ai);

	gfs2_log_lock(sdp);
	sdp->sd_log_head = sdp->sd_log_flush_head;
	sdp->sd_log_blks_reserved = 0;
	sdp->sd_log_commited_buf = 0;
	sdp->sd_log_commited_databuf = 0;
	sdp->sd_log_commited_revoke = 0;

	spin_lock(&sdp->sd_ail_lock);
	if (!list_empty(&ai->ai_ail1_list)) {
		list_add(&ai->ai_list, &sdp->sd_ail1_list);
		ai = NULL;
	}
	spin_unlock(&sdp->sd_ail_lock);
	gfs2_log_unlock(sdp);
	trace_gfs2_log_flush(sdp, 0);
	up_write(&sdp->sd_log_flush_lock);

	kfree(ai);
=======
 * @sdp: The filesystem
 * @gl: The glock structure to flush.  If NULL, flush the whole incore log
 * @flags: The log header flags: GFS2_LOG_HEAD_FLUSH_* and debug flags
 *
 */

void gfs2_log_flush(struct gfs2_sbd *sdp, struct gfs2_glock *gl, u32 flags)
{
	struct gfs2_trans *tr = NULL;
	unsigned int reserved_blocks = 0, used_blocks = 0;
	bool frozen = test_bit(SDF_FROZEN, &sdp->sd_flags);
	unsigned int first_log_head;
	unsigned int reserved_revokes = 0;

	down_write(&sdp->sd_log_flush_lock);
	trace_gfs2_log_flush(sdp, 1, flags);

repeat:
	/*
	 * Do this check while holding the log_flush_lock to prevent new
	 * buffers from being added to the ail via gfs2_pin()
	 */
	if (gfs2_withdrawing_or_withdrawn(sdp) ||
	    !test_bit(SDF_JOURNAL_LIVE, &sdp->sd_flags))
		goto out;

	/* Log might have been flushed while we waited for the flush lock */
	if (gl && !test_bit(GLF_LFLUSH, &gl->gl_flags))
		goto out;

	first_log_head = sdp->sd_log_head;
	sdp->sd_log_flush_head = first_log_head;

	tr = sdp->sd_log_tr;
	if (tr || sdp->sd_log_num_revoke) {
		if (reserved_blocks)
			gfs2_log_release(sdp, reserved_blocks);
		reserved_blocks = sdp->sd_log_blks_reserved;
		reserved_revokes = sdp->sd_log_num_revoke;
		if (tr) {
			sdp->sd_log_tr = NULL;
			tr->tr_first = first_log_head;
			if (unlikely(frozen)) {
				if (gfs2_assert_withdraw_delayed(sdp,
				       !tr->tr_num_buf_new && !tr->tr_num_databuf_new))
					goto out_withdraw;
			}
		}
	} else if (!reserved_blocks) {
		unsigned int taboo_blocks = GFS2_LOG_FLUSH_MIN_BLOCKS;

		reserved_blocks = GFS2_LOG_FLUSH_MIN_BLOCKS;
		if (current == sdp->sd_logd_process)
			taboo_blocks = 0;

		if (!__gfs2_log_try_reserve(sdp, reserved_blocks, taboo_blocks)) {
			up_write(&sdp->sd_log_flush_lock);
			__gfs2_log_reserve(sdp, reserved_blocks, taboo_blocks);
			down_write(&sdp->sd_log_flush_lock);
			goto repeat;
		}
		BUG_ON(sdp->sd_log_num_revoke);
	}

	if (flags & GFS2_LOG_HEAD_FLUSH_SHUTDOWN)
		clear_bit(SDF_JOURNAL_LIVE, &sdp->sd_flags);

	if (unlikely(frozen))
		if (gfs2_assert_withdraw_delayed(sdp, !reserved_revokes))
			goto out_withdraw;

	gfs2_ordered_write(sdp);
	if (gfs2_withdrawing_or_withdrawn(sdp))
		goto out_withdraw;
	lops_before_commit(sdp, tr);
	if (gfs2_withdrawing_or_withdrawn(sdp))
		goto out_withdraw;
	gfs2_log_submit_bio(&sdp->sd_jdesc->jd_log_bio, REQ_OP_WRITE);
	if (gfs2_withdrawing_or_withdrawn(sdp))
		goto out_withdraw;

	if (sdp->sd_log_head != sdp->sd_log_flush_head) {
		log_write_header(sdp, flags);
	} else if (sdp->sd_log_tail != sdp->sd_log_flush_tail && !sdp->sd_log_idle) {
		log_write_header(sdp, flags);
	}
	if (gfs2_withdrawing_or_withdrawn(sdp))
		goto out_withdraw;
	lops_after_commit(sdp, tr);

	gfs2_log_lock(sdp);
	sdp->sd_log_blks_reserved = 0;

	spin_lock(&sdp->sd_ail_lock);
	if (tr && !list_empty(&tr->tr_ail1_list)) {
		list_add(&tr->tr_list, &sdp->sd_ail1_list);
		tr = NULL;
	}
	spin_unlock(&sdp->sd_ail_lock);
	gfs2_log_unlock(sdp);

	if (!(flags & GFS2_LOG_HEAD_FLUSH_NORMAL)) {
		if (!sdp->sd_log_idle) {
			empty_ail1_list(sdp);
			if (gfs2_withdrawing_or_withdrawn(sdp))
				goto out_withdraw;
			log_write_header(sdp, flags);
		}
		if (flags & (GFS2_LOG_HEAD_FLUSH_SHUTDOWN |
			     GFS2_LOG_HEAD_FLUSH_FREEZE))
			gfs2_log_shutdown(sdp);
	}

out_end:
	used_blocks = log_distance(sdp, sdp->sd_log_flush_head, first_log_head);
	reserved_revokes += atomic_read(&sdp->sd_log_revokes_available);
	atomic_set(&sdp->sd_log_revokes_available, sdp->sd_ldptrs);
	gfs2_assert_withdraw(sdp, reserved_revokes % sdp->sd_inptrs == sdp->sd_ldptrs);
	if (reserved_revokes > sdp->sd_ldptrs)
		reserved_blocks += (reserved_revokes - sdp->sd_ldptrs) / sdp->sd_inptrs;
out:
	if (used_blocks != reserved_blocks) {
		gfs2_assert_withdraw_delayed(sdp, used_blocks < reserved_blocks);
		gfs2_log_release(sdp, reserved_blocks - used_blocks);
	}
	up_write(&sdp->sd_log_flush_lock);
	gfs2_trans_free(sdp, tr);
	if (gfs2_withdrawing(sdp))
		gfs2_withdraw(sdp);
	trace_gfs2_log_flush(sdp, 0, flags);
	return;

out_withdraw:
	trans_drain(tr);
	/**
	 * If the tr_list is empty, we're withdrawing during a log
	 * flush that targets a transaction, but the transaction was
	 * never queued onto any of the ail lists. Here we add it to
	 * ail1 just so that ail_drain() will find and free it.
	 */
	spin_lock(&sdp->sd_ail_lock);
	if (tr && list_empty(&tr->tr_list))
		list_add(&tr->tr_list, &sdp->sd_ail1_list);
	spin_unlock(&sdp->sd_ail_lock);
	tr = NULL;
	goto out_end;
}

/**
 * gfs2_merge_trans - Merge a new transaction into a cached transaction
 * @sdp: the filesystem
 * @new: New transaction to be merged
 */

static void gfs2_merge_trans(struct gfs2_sbd *sdp, struct gfs2_trans *new)
{
	struct gfs2_trans *old = sdp->sd_log_tr;

	WARN_ON_ONCE(!test_bit(TR_ATTACHED, &old->tr_flags));

	old->tr_num_buf_new	+= new->tr_num_buf_new;
	old->tr_num_databuf_new	+= new->tr_num_databuf_new;
	old->tr_num_buf_rm	+= new->tr_num_buf_rm;
	old->tr_num_databuf_rm	+= new->tr_num_databuf_rm;
	old->tr_revokes		+= new->tr_revokes;
	old->tr_num_revoke	+= new->tr_num_revoke;

	list_splice_tail_init(&new->tr_databuf, &old->tr_databuf);
	list_splice_tail_init(&new->tr_buf, &old->tr_buf);

	spin_lock(&sdp->sd_ail_lock);
	list_splice_tail_init(&new->tr_ail1_list, &old->tr_ail1_list);
	list_splice_tail_init(&new->tr_ail2_list, &old->tr_ail2_list);
	spin_unlock(&sdp->sd_ail_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void log_refund(struct gfs2_sbd *sdp, struct gfs2_trans *tr)
{
	unsigned int reserved;
	unsigned int unused;
<<<<<<< HEAD

	gfs2_log_lock(sdp);

	sdp->sd_log_commited_buf += tr->tr_num_buf_new - tr->tr_num_buf_rm;
	sdp->sd_log_commited_databuf += tr->tr_num_databuf_new -
		tr->tr_num_databuf_rm;
	gfs2_assert_withdraw(sdp, (((int)sdp->sd_log_commited_buf) >= 0) ||
			     (((int)sdp->sd_log_commited_databuf) >= 0));
	sdp->sd_log_commited_revoke += tr->tr_num_revoke - tr->tr_num_revoke_rm;
	reserved = calc_reserved(sdp);
	gfs2_assert_withdraw(sdp, sdp->sd_log_blks_reserved + tr->tr_reserved >= reserved);
	unused = sdp->sd_log_blks_reserved - reserved + tr->tr_reserved;
	atomic_add(unused, &sdp->sd_log_blks_free);
	trace_gfs2_log_blocks(sdp, unused);
	gfs2_assert_withdraw(sdp, atomic_read(&sdp->sd_log_blks_free) <=
			     sdp->sd_jdesc->jd_blocks);
=======
	unsigned int maxres;

	gfs2_log_lock(sdp);

	if (sdp->sd_log_tr) {
		gfs2_merge_trans(sdp, tr);
	} else if (tr->tr_num_buf_new || tr->tr_num_databuf_new) {
		gfs2_assert_withdraw(sdp, !test_bit(TR_ONSTACK, &tr->tr_flags));
		sdp->sd_log_tr = tr;
		set_bit(TR_ATTACHED, &tr->tr_flags);
	}

	reserved = calc_reserved(sdp);
	maxres = sdp->sd_log_blks_reserved + tr->tr_reserved;
	gfs2_assert_withdraw(sdp, maxres >= reserved);
	unused = maxres - reserved;
	if (unused)
		gfs2_log_release(sdp, unused);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	sdp->sd_log_blks_reserved = reserved;

	gfs2_log_unlock(sdp);
}

<<<<<<< HEAD
static void buf_lo_incore_commit(struct gfs2_sbd *sdp, struct gfs2_trans *tr)
{
	struct list_head *head = &tr->tr_list_buf;
	struct gfs2_bufdata *bd;

	gfs2_log_lock(sdp);
	while (!list_empty(head)) {
		bd = list_entry(head->next, struct gfs2_bufdata, bd_list_tr);
		list_del_init(&bd->bd_list_tr);
		tr->tr_num_buf--;
	}
	gfs2_log_unlock(sdp);
	gfs2_assert_warn(sdp, !tr->tr_num_buf);
=======
static inline int gfs2_jrnl_flush_reqd(struct gfs2_sbd *sdp)
{
	return atomic_read(&sdp->sd_log_pinned) +
	       atomic_read(&sdp->sd_log_blks_needed) >=
	       atomic_read(&sdp->sd_log_thresh1);
}

static inline int gfs2_ail_flush_reqd(struct gfs2_sbd *sdp)
{
	return sdp->sd_jdesc->jd_blocks -
	       atomic_read(&sdp->sd_log_blks_free) +
	       atomic_read(&sdp->sd_log_blks_needed) >=
	       atomic_read(&sdp->sd_log_thresh2);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * gfs2_log_commit - Commit a transaction to the log
 * @sdp: the filesystem
 * @tr: the transaction
 *
 * We wake up gfs2_logd if the number of pinned blocks exceed thresh1
 * or the total number of used blocks (pinned blocks plus AIL blocks)
 * is greater than thresh2.
 *
<<<<<<< HEAD
 * At mount time thresh1 is 1/3rd of journal size, thresh2 is 2/3rd of
=======
 * At mount time thresh1 is 2/5ths of journal size, thresh2 is 4/5ths of
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * journal size.
 *
 * Returns: errno
 */

void gfs2_log_commit(struct gfs2_sbd *sdp, struct gfs2_trans *tr)
{
	log_refund(sdp, tr);
<<<<<<< HEAD
	buf_lo_incore_commit(sdp, tr);

	up_read(&sdp->sd_log_flush_lock);

	if (atomic_read(&sdp->sd_log_pinned) > atomic_read(&sdp->sd_log_thresh1) ||
	    ((sdp->sd_jdesc->jd_blocks - atomic_read(&sdp->sd_log_blks_free)) >
	    atomic_read(&sdp->sd_log_thresh2)))
=======

	if (gfs2_ail_flush_reqd(sdp) || gfs2_jrnl_flush_reqd(sdp))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		wake_up(&sdp->sd_logd_waitq);
}

/**
 * gfs2_log_shutdown - write a shutdown header into a journal
 * @sdp: the filesystem
 *
 */

<<<<<<< HEAD
void gfs2_log_shutdown(struct gfs2_sbd *sdp)
{
	down_write(&sdp->sd_log_flush_lock);

	gfs2_assert_withdraw(sdp, !sdp->sd_log_blks_reserved);
	gfs2_assert_withdraw(sdp, !sdp->sd_log_num_buf);
	gfs2_assert_withdraw(sdp, !sdp->sd_log_num_revoke);
	gfs2_assert_withdraw(sdp, !sdp->sd_log_num_rg);
	gfs2_assert_withdraw(sdp, !sdp->sd_log_num_databuf);
	gfs2_assert_withdraw(sdp, list_empty(&sdp->sd_ail1_list));

	sdp->sd_log_flush_head = sdp->sd_log_head;
	sdp->sd_log_flush_wrapped = 0;

	log_write_header(sdp, GFS2_LOG_HEAD_UNMOUNT,
			 (sdp->sd_log_tail == current_tail(sdp)) ? 0 : PULL);

	gfs2_assert_warn(sdp, atomic_read(&sdp->sd_log_blks_free) == sdp->sd_jdesc->jd_blocks);
	gfs2_assert_warn(sdp, sdp->sd_log_head == sdp->sd_log_tail);
	gfs2_assert_warn(sdp, list_empty(&sdp->sd_ail2_list));

	sdp->sd_log_head = sdp->sd_log_flush_head;
	sdp->sd_log_tail = sdp->sd_log_head;

	up_write(&sdp->sd_log_flush_lock);
}


/**
 * gfs2_meta_syncfs - sync all the buffers in a filesystem
 * @sdp: the filesystem
 *
 */

void gfs2_meta_syncfs(struct gfs2_sbd *sdp)
{
	gfs2_log_flush(sdp, NULL);
	for (;;) {
		gfs2_ail1_start(sdp);
		gfs2_ail1_wait(sdp);
		if (gfs2_ail1_empty(sdp))
			break;
	}
	gfs2_log_flush(sdp, NULL);
}

static inline int gfs2_jrnl_flush_reqd(struct gfs2_sbd *sdp)
{
	return (atomic_read(&sdp->sd_log_pinned) >= atomic_read(&sdp->sd_log_thresh1));
}

static inline int gfs2_ail_flush_reqd(struct gfs2_sbd *sdp)
{
	unsigned int used_blocks = sdp->sd_jdesc->jd_blocks - atomic_read(&sdp->sd_log_blks_free);
	return used_blocks >= atomic_read(&sdp->sd_log_thresh2);
=======
static void gfs2_log_shutdown(struct gfs2_sbd *sdp)
{
	gfs2_assert_withdraw(sdp, !sdp->sd_log_blks_reserved);
	gfs2_assert_withdraw(sdp, !sdp->sd_log_num_revoke);
	gfs2_assert_withdraw(sdp, list_empty(&sdp->sd_ail1_list));

	log_write_header(sdp, GFS2_LOG_HEAD_UNMOUNT | GFS2_LFC_SHUTDOWN);
	log_pull_tail(sdp);

	gfs2_assert_warn(sdp, sdp->sd_log_head == sdp->sd_log_tail);
	gfs2_assert_warn(sdp, list_empty(&sdp->sd_ail2_list));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * gfs2_logd - Update log tail as Active Items get flushed to in-place blocks
<<<<<<< HEAD
 * @sdp: Pointer to GFS2 superblock
=======
 * @data: Pointer to GFS2 superblock
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Also, periodically check to make sure that we're using the most recent
 * journal index.
 */

int gfs2_logd(void *data)
{
	struct gfs2_sbd *sdp = data;
	unsigned long t = 1;
<<<<<<< HEAD
	DEFINE_WAIT(wait);
	unsigned preflush;

	while (!kthread_should_stop()) {

		preflush = atomic_read(&sdp->sd_log_pinned);
		if (gfs2_jrnl_flush_reqd(sdp) || t == 0) {
			gfs2_ail1_empty(sdp);
			gfs2_log_flush(sdp, NULL);
		}

		if (gfs2_ail_flush_reqd(sdp)) {
			gfs2_ail1_start(sdp);
			gfs2_ail1_wait(sdp);
			gfs2_ail1_empty(sdp);
			gfs2_log_flush(sdp, NULL);
		}

		if (!gfs2_ail_flush_reqd(sdp))
			wake_up(&sdp->sd_log_waitq);

		t = gfs2_tune_get(sdp, gt_logd_secs) * HZ;

		try_to_freeze();

		do {
			prepare_to_wait(&sdp->sd_logd_waitq, &wait,
					TASK_INTERRUPTIBLE);
			if (!gfs2_ail_flush_reqd(sdp) &&
			    !gfs2_jrnl_flush_reqd(sdp) &&
			    !kthread_should_stop())
				t = schedule_timeout(t);
		} while(t && !gfs2_ail_flush_reqd(sdp) &&
			!gfs2_jrnl_flush_reqd(sdp) &&
			!kthread_should_stop());
		finish_wait(&sdp->sd_logd_waitq, &wait);
	}

=======

	set_freezable();
	while (!kthread_should_stop()) {
		if (gfs2_withdrawing_or_withdrawn(sdp))
			break;

		/* Check for errors writing to the journal */
		if (sdp->sd_log_error) {
			gfs2_lm(sdp,
				"GFS2: fsid=%s: error %d: "
				"withdrawing the file system to "
				"prevent further damage.\n",
				sdp->sd_fsname, sdp->sd_log_error);
			gfs2_withdraw(sdp);
			break;
		}

		if (gfs2_jrnl_flush_reqd(sdp) || t == 0) {
			gfs2_ail1_empty(sdp, 0);
			gfs2_log_flush(sdp, NULL, GFS2_LOG_HEAD_FLUSH_NORMAL |
						  GFS2_LFC_LOGD_JFLUSH_REQD);
		}

		if (test_bit(SDF_FORCE_AIL_FLUSH, &sdp->sd_flags) ||
		    gfs2_ail_flush_reqd(sdp)) {
			clear_bit(SDF_FORCE_AIL_FLUSH, &sdp->sd_flags);
			gfs2_ail1_start(sdp);
			gfs2_ail1_wait(sdp);
			gfs2_ail1_empty(sdp, 0);
			gfs2_log_flush(sdp, NULL, GFS2_LOG_HEAD_FLUSH_NORMAL |
						  GFS2_LFC_LOGD_AIL_FLUSH_REQD);
		}

		t = gfs2_tune_get(sdp, gt_logd_secs) * HZ;

		t = wait_event_freezable_timeout(sdp->sd_logd_waitq,
				test_bit(SDF_FORCE_AIL_FLUSH, &sdp->sd_flags) ||
				gfs2_ail_flush_reqd(sdp) ||
				gfs2_jrnl_flush_reqd(sdp) ||
				sdp->sd_log_error ||
				gfs2_withdrawing_or_withdrawn(sdp) ||
				kthread_should_stop(),
				t);
	}

	if (gfs2_withdrawing(sdp))
		gfs2_withdraw(sdp);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

