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

#include <linux/spinlock.h>
#include <linux/completion.h>
#include <linux/buffer_head.h>
#include <linux/gfs2_ondisk.h>
#include <linux/bio.h>
#include <linux/posix_acl.h>
<<<<<<< HEAD
=======
#include <linux/security.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include "gfs2.h"
#include "incore.h"
#include "bmap.h"
#include "glock.h"
#include "glops.h"
#include "inode.h"
#include "log.h"
#include "meta_io.h"
#include "recovery.h"
#include "rgrp.h"
#include "util.h"
#include "trans.h"
#include "dir.h"
<<<<<<< HEAD

static void gfs2_ail_error(struct gfs2_glock *gl, const struct buffer_head *bh)
{
	fs_err(gl->gl_sbd, "AIL buffer %p: blocknr %llu state 0x%08lx mapping %p page state 0x%lx\n",
	       bh, (unsigned long long)bh->b_blocknr, bh->b_state,
	       bh->b_page->mapping, bh->b_page->flags);
	fs_err(gl->gl_sbd, "AIL glock %u:%llu mapping %p\n",
	       gl->gl_name.ln_type, gl->gl_name.ln_number,
	       gfs2_glock2aspace(gl));
	gfs2_lm_withdraw(gl->gl_sbd, "AIL error\n");
=======
#include "lops.h"

struct workqueue_struct *gfs2_freeze_wq;

extern struct workqueue_struct *gfs2_control_wq;

static void gfs2_ail_error(struct gfs2_glock *gl, const struct buffer_head *bh)
{
	struct gfs2_sbd *sdp = gl->gl_name.ln_sbd;

	fs_err(sdp,
	       "AIL buffer %p: blocknr %llu state 0x%08lx mapping %p page "
	       "state 0x%lx\n",
	       bh, (unsigned long long)bh->b_blocknr, bh->b_state,
	       bh->b_folio->mapping, bh->b_folio->flags);
	fs_err(sdp, "AIL glock %u:%llu mapping %p\n",
	       gl->gl_name.ln_type, gl->gl_name.ln_number,
	       gfs2_glock2aspace(gl));
	gfs2_lm(sdp, "AIL error\n");
	gfs2_withdraw_delayed(sdp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * __gfs2_ail_flush - remove all buffers for a given lock from the AIL
 * @gl: the glock
 * @fsync: set when called from fsync (not all buffers will be clean)
<<<<<<< HEAD
=======
 * @nr_revokes: Number of buffers to revoke
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * None of the buffers should be dirty, locked, or pinned.
 */

<<<<<<< HEAD
static void __gfs2_ail_flush(struct gfs2_glock *gl, bool fsync)
{
	struct gfs2_sbd *sdp = gl->gl_sbd;
=======
static void __gfs2_ail_flush(struct gfs2_glock *gl, bool fsync,
			     unsigned int nr_revokes)
{
	struct gfs2_sbd *sdp = gl->gl_name.ln_sbd;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct list_head *head = &gl->gl_ail_list;
	struct gfs2_bufdata *bd, *tmp;
	struct buffer_head *bh;
	const unsigned long b_state = (1UL << BH_Dirty)|(1UL << BH_Pinned)|(1UL << BH_Lock);
<<<<<<< HEAD
	sector_t blocknr;

	gfs2_log_lock(sdp);
	spin_lock(&sdp->sd_ail_lock);
	list_for_each_entry_safe(bd, tmp, head, bd_ail_gl_list) {
=======

	gfs2_log_lock(sdp);
	spin_lock(&sdp->sd_ail_lock);
	list_for_each_entry_safe_reverse(bd, tmp, head, bd_ail_gl_list) {
		if (nr_revokes == 0)
			break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		bh = bd->bd_bh;
		if (bh->b_state & b_state) {
			if (fsync)
				continue;
			gfs2_ail_error(gl, bh);
		}
<<<<<<< HEAD
		blocknr = bh->b_blocknr;
		bh->b_private = NULL;
		gfs2_remove_from_ail(bd); /* drops ref on bh */

		bd->bd_bh = NULL;
		bd->bd_blkno = blocknr;

		gfs2_trans_add_revoke(sdp, bd);
	}
	BUG_ON(!fsync && atomic_read(&gl->gl_ail_count));
=======
		gfs2_trans_add_revoke(sdp, bd);
		nr_revokes--;
	}
	GLOCK_BUG_ON(gl, !fsync && atomic_read(&gl->gl_ail_count));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_unlock(&sdp->sd_ail_lock);
	gfs2_log_unlock(sdp);
}


<<<<<<< HEAD
static void gfs2_ail_empty_gl(struct gfs2_glock *gl)
{
	struct gfs2_sbd *sdp = gl->gl_sbd;
	struct gfs2_trans tr;

	memset(&tr, 0, sizeof(tr));
	tr.tr_revokes = atomic_read(&gl->gl_ail_count);

	if (!tr.tr_revokes)
		return;

	/* A shortened, inline version of gfs2_trans_begin() */
	tr.tr_reserved = 1 + gfs2_struct2blk(sdp, tr.tr_revokes, sizeof(u64));
	tr.tr_ip = (unsigned long)__builtin_return_address(0);
	INIT_LIST_HEAD(&tr.tr_list_buf);
	gfs2_log_reserve(sdp, tr.tr_reserved);
	BUG_ON(current->journal_info);
	current->journal_info = &tr;

	__gfs2_ail_flush(gl, 0);

	gfs2_trans_end(sdp);
	gfs2_log_flush(sdp, NULL);
=======
static int gfs2_ail_empty_gl(struct gfs2_glock *gl)
{
	struct gfs2_sbd *sdp = gl->gl_name.ln_sbd;
	struct gfs2_trans tr;
	unsigned int revokes;
	int ret = 0;

	revokes = atomic_read(&gl->gl_ail_count);

	if (!revokes) {
		bool have_revokes;
		bool log_in_flight;

		/*
		 * We have nothing on the ail, but there could be revokes on
		 * the sdp revoke queue, in which case, we still want to flush
		 * the log and wait for it to finish.
		 *
		 * If the sdp revoke list is empty too, we might still have an
		 * io outstanding for writing revokes, so we should wait for
		 * it before returning.
		 *
		 * If none of these conditions are true, our revokes are all
		 * flushed and we can return.
		 */
		gfs2_log_lock(sdp);
		have_revokes = !list_empty(&sdp->sd_log_revokes);
		log_in_flight = atomic_read(&sdp->sd_log_in_flight);
		gfs2_log_unlock(sdp);
		if (have_revokes)
			goto flush;
		if (log_in_flight)
			log_flush_wait(sdp);
		return 0;
	}

	memset(&tr, 0, sizeof(tr));
	set_bit(TR_ONSTACK, &tr.tr_flags);
	ret = __gfs2_trans_begin(&tr, sdp, 0, revokes, _RET_IP_);
	if (ret) {
		fs_err(sdp, "Transaction error %d: Unable to write revokes.", ret);
		goto flush;
	}
	__gfs2_ail_flush(gl, 0, revokes);
	gfs2_trans_end(sdp);

flush:
	if (!ret)
		gfs2_log_flush(sdp, NULL, GFS2_LOG_HEAD_FLUSH_NORMAL |
				GFS2_LFC_AIL_EMPTY_GL);
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void gfs2_ail_flush(struct gfs2_glock *gl, bool fsync)
{
<<<<<<< HEAD
	struct gfs2_sbd *sdp = gl->gl_sbd;
=======
	struct gfs2_sbd *sdp = gl->gl_name.ln_sbd;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned int revokes = atomic_read(&gl->gl_ail_count);
	int ret;

	if (!revokes)
		return;

	ret = gfs2_trans_begin(sdp, 0, revokes);
	if (ret)
		return;
<<<<<<< HEAD
	__gfs2_ail_flush(gl, fsync);
	gfs2_trans_end(sdp);
	gfs2_log_flush(sdp, NULL);
=======
	__gfs2_ail_flush(gl, fsync, revokes);
	gfs2_trans_end(sdp);
	gfs2_log_flush(sdp, NULL, GFS2_LOG_HEAD_FLUSH_NORMAL |
		       GFS2_LFC_AIL_FLUSH);
}

/**
 * gfs2_rgrp_metasync - sync out the metadata of a resource group
 * @gl: the glock protecting the resource group
 *
 */

static int gfs2_rgrp_metasync(struct gfs2_glock *gl)
{
	struct gfs2_sbd *sdp = gl->gl_name.ln_sbd;
	struct address_space *metamapping = &sdp->sd_aspace;
	struct gfs2_rgrpd *rgd = gfs2_glock2rgrp(gl);
	const unsigned bsize = sdp->sd_sb.sb_bsize;
	loff_t start = (rgd->rd_addr * bsize) & PAGE_MASK;
	loff_t end = PAGE_ALIGN((rgd->rd_addr + rgd->rd_length) * bsize) - 1;
	int error;

	filemap_fdatawrite_range(metamapping, start, end);
	error = filemap_fdatawait_range(metamapping, start, end);
	WARN_ON_ONCE(error && !gfs2_withdrawing_or_withdrawn(sdp));
	mapping_set_error(metamapping, error);
	if (error)
		gfs2_io_error(sdp);
	return error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * rgrp_go_sync - sync out the metadata for this glock
 * @gl: the glock
 *
 * Called when demoting or unlocking an EX glock.  We must flush
 * to disk all dirty buffers/pages relating to this glock, and must not
<<<<<<< HEAD
 * not return to caller to demote/unlock the glock until I/O is complete.
 */

static void rgrp_go_sync(struct gfs2_glock *gl)
{
	struct address_space *metamapping = gfs2_glock2aspace(gl);
	struct gfs2_rgrpd *rgd;
	int error;

	if (!test_and_clear_bit(GLF_DIRTY, &gl->gl_flags))
		return;
	BUG_ON(gl->gl_state != LM_ST_EXCLUSIVE);

	gfs2_log_flush(gl->gl_sbd, gl);
	filemap_fdatawrite(metamapping);
	error = filemap_fdatawait(metamapping);
        mapping_set_error(metamapping, error);
	gfs2_ail_empty_gl(gl);

	spin_lock(&gl->gl_spin);
	rgd = gl->gl_object;
	if (rgd)
		gfs2_free_clones(rgd);
	spin_unlock(&gl->gl_spin);
=======
 * return to caller to demote/unlock the glock until I/O is complete.
 */

static int rgrp_go_sync(struct gfs2_glock *gl)
{
	struct gfs2_sbd *sdp = gl->gl_name.ln_sbd;
	struct gfs2_rgrpd *rgd = gfs2_glock2rgrp(gl);
	int error;

	if (!rgd || !test_and_clear_bit(GLF_DIRTY, &gl->gl_flags))
		return 0;
	GLOCK_BUG_ON(gl, gl->gl_state != LM_ST_EXCLUSIVE);

	gfs2_log_flush(sdp, gl, GFS2_LOG_HEAD_FLUSH_NORMAL |
		       GFS2_LFC_RGRP_GO_SYNC);
	error = gfs2_rgrp_metasync(gl);
	if (!error)
		error = gfs2_ail_empty_gl(gl);
	gfs2_free_clones(rgd);
	return error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * rgrp_go_inval - invalidate the metadata for this glock
 * @gl: the glock
 * @flags:
 *
 * We never used LM_ST_DEFERRED with resource groups, so that we
 * should always see the metadata flag set here.
 *
 */

static void rgrp_go_inval(struct gfs2_glock *gl, int flags)
{
<<<<<<< HEAD
	struct address_space *mapping = gfs2_glock2aspace(gl);

	BUG_ON(!(flags & DIO_METADATA));
	gfs2_assert_withdraw(gl->gl_sbd, !atomic_read(&gl->gl_ail_count));
	truncate_inode_pages(mapping, 0);

	if (gl->gl_object) {
		struct gfs2_rgrpd *rgd = (struct gfs2_rgrpd *)gl->gl_object;
		rgd->rd_flags &= ~GFS2_RDF_UPTODATE;
	}
}

/**
 * inode_go_sync - Sync the dirty data and/or metadata for an inode glock
=======
	struct gfs2_sbd *sdp = gl->gl_name.ln_sbd;
	struct address_space *mapping = &sdp->sd_aspace;
	struct gfs2_rgrpd *rgd = gfs2_glock2rgrp(gl);
	const unsigned bsize = sdp->sd_sb.sb_bsize;
	loff_t start, end;

	if (!rgd)
		return;
	start = (rgd->rd_addr * bsize) & PAGE_MASK;
	end = PAGE_ALIGN((rgd->rd_addr + rgd->rd_length) * bsize) - 1;
	gfs2_rgrp_brelse(rgd);
	WARN_ON_ONCE(!(flags & DIO_METADATA));
	truncate_inode_pages_range(mapping, start, end);
}

static void gfs2_rgrp_go_dump(struct seq_file *seq, const struct gfs2_glock *gl,
			      const char *fs_id_buf)
{
	struct gfs2_rgrpd *rgd = gl->gl_object;

	if (rgd)
		gfs2_rgrp_dump(seq, rgd, fs_id_buf);
}

static struct gfs2_inode *gfs2_glock2inode(struct gfs2_glock *gl)
{
	struct gfs2_inode *ip;

	spin_lock(&gl->gl_lockref.lock);
	ip = gl->gl_object;
	if (ip)
		set_bit(GIF_GLOP_PENDING, &ip->i_flags);
	spin_unlock(&gl->gl_lockref.lock);
	return ip;
}

struct gfs2_rgrpd *gfs2_glock2rgrp(struct gfs2_glock *gl)
{
	struct gfs2_rgrpd *rgd;

	spin_lock(&gl->gl_lockref.lock);
	rgd = gl->gl_object;
	spin_unlock(&gl->gl_lockref.lock);

	return rgd;
}

static void gfs2_clear_glop_pending(struct gfs2_inode *ip)
{
	if (!ip)
		return;

	clear_bit_unlock(GIF_GLOP_PENDING, &ip->i_flags);
	wake_up_bit(&ip->i_flags, GIF_GLOP_PENDING);
}

/**
 * gfs2_inode_metasync - sync out the metadata of an inode
 * @gl: the glock protecting the inode
 *
 */
int gfs2_inode_metasync(struct gfs2_glock *gl)
{
	struct address_space *metamapping = gfs2_glock2aspace(gl);
	int error;

	filemap_fdatawrite(metamapping);
	error = filemap_fdatawait(metamapping);
	if (error)
		gfs2_io_error(gl->gl_name.ln_sbd);
	return error;
}

/**
 * inode_go_sync - Sync the dirty metadata of an inode
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @gl: the glock protecting the inode
 *
 */

<<<<<<< HEAD
static void inode_go_sync(struct gfs2_glock *gl)
{
	struct gfs2_inode *ip = gl->gl_object;
	struct address_space *metamapping = gfs2_glock2aspace(gl);
	int error;

	if (ip && !S_ISREG(ip->i_inode.i_mode))
		ip = NULL;
	if (ip && test_and_clear_bit(GIF_SW_PAGED, &ip->i_flags))
		unmap_shared_mapping_range(ip->i_inode.i_mapping, 0, 0);
	if (!test_and_clear_bit(GLF_DIRTY, &gl->gl_flags))
		return;

	BUG_ON(gl->gl_state != LM_ST_EXCLUSIVE);

	gfs2_log_flush(gl->gl_sbd, gl);
	filemap_fdatawrite(metamapping);
	if (ip) {
=======
static int inode_go_sync(struct gfs2_glock *gl)
{
	struct gfs2_inode *ip = gfs2_glock2inode(gl);
	int isreg = ip && S_ISREG(ip->i_inode.i_mode);
	struct address_space *metamapping = gfs2_glock2aspace(gl);
	int error = 0, ret;

	if (isreg) {
		if (test_and_clear_bit(GIF_SW_PAGED, &ip->i_flags))
			unmap_shared_mapping_range(ip->i_inode.i_mapping, 0, 0);
		inode_dio_wait(&ip->i_inode);
	}
	if (!test_and_clear_bit(GLF_DIRTY, &gl->gl_flags))
		goto out;

	GLOCK_BUG_ON(gl, gl->gl_state != LM_ST_EXCLUSIVE);

	gfs2_log_flush(gl->gl_name.ln_sbd, gl, GFS2_LOG_HEAD_FLUSH_NORMAL |
		       GFS2_LFC_INODE_GO_SYNC);
	filemap_fdatawrite(metamapping);
	if (isreg) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		struct address_space *mapping = ip->i_inode.i_mapping;
		filemap_fdatawrite(mapping);
		error = filemap_fdatawait(mapping);
		mapping_set_error(mapping, error);
	}
<<<<<<< HEAD
	error = filemap_fdatawait(metamapping);
	mapping_set_error(metamapping, error);
	gfs2_ail_empty_gl(gl);
=======
	ret = gfs2_inode_metasync(gl);
	if (!error)
		error = ret;
	ret = gfs2_ail_empty_gl(gl);
	if (!error)
		error = ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Writeback of the data mapping may cause the dirty flag to be set
	 * so we have to clear it again here.
	 */
<<<<<<< HEAD
	smp_mb__before_clear_bit();
	clear_bit(GLF_DIRTY, &gl->gl_flags);
=======
	smp_mb__before_atomic();
	clear_bit(GLF_DIRTY, &gl->gl_flags);

out:
	gfs2_clear_glop_pending(ip);
	return error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * inode_go_inval - prepare a inode glock to be released
 * @gl: the glock
 * @flags:
<<<<<<< HEAD
 * 
 * Normally we invlidate everything, but if we are moving into
=======
 *
 * Normally we invalidate everything, but if we are moving into
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * LM_ST_DEFERRED from LM_ST_SHARED or LM_ST_EXCLUSIVE then we
 * can keep hold of the metadata, since it won't have changed.
 *
 */

static void inode_go_inval(struct gfs2_glock *gl, int flags)
{
<<<<<<< HEAD
	struct gfs2_inode *ip = gl->gl_object;

	gfs2_assert_withdraw(gl->gl_sbd, !atomic_read(&gl->gl_ail_count));
=======
	struct gfs2_inode *ip = gfs2_glock2inode(gl);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (flags & DIO_METADATA) {
		struct address_space *mapping = gfs2_glock2aspace(gl);
		truncate_inode_pages(mapping, 0);
		if (ip) {
<<<<<<< HEAD
			set_bit(GIF_INVALID, &ip->i_flags);
			forget_all_cached_acls(&ip->i_inode);
=======
			set_bit(GLF_INSTANTIATE_NEEDED, &gl->gl_flags);
			forget_all_cached_acls(&ip->i_inode);
			security_inode_invalidate_secctx(&ip->i_inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			gfs2_dir_hash_inval(ip);
		}
	}

<<<<<<< HEAD
	if (ip == GFS2_I(gl->gl_sbd->sd_rindex)) {
		gfs2_log_flush(gl->gl_sbd, NULL);
		gl->gl_sbd->sd_rindex_uptodate = 0;
	}
	if (ip && S_ISREG(ip->i_inode.i_mode))
		truncate_inode_pages(ip->i_inode.i_mapping, 0);
=======
	if (ip == GFS2_I(gl->gl_name.ln_sbd->sd_rindex)) {
		gfs2_log_flush(gl->gl_name.ln_sbd, NULL,
			       GFS2_LOG_HEAD_FLUSH_NORMAL |
			       GFS2_LFC_INODE_GO_INVAL);
		gl->gl_name.ln_sbd->sd_rindex_uptodate = 0;
	}
	if (ip && S_ISREG(ip->i_inode.i_mode))
		truncate_inode_pages(ip->i_inode.i_mapping, 0);

	gfs2_clear_glop_pending(ip);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * inode_go_demote_ok - Check to see if it's ok to unlock an inode glock
 * @gl: the glock
 *
 * Returns: 1 if it's ok
 */

static int inode_go_demote_ok(const struct gfs2_glock *gl)
{
<<<<<<< HEAD
	struct gfs2_sbd *sdp = gl->gl_sbd;
	struct gfs2_holder *gh;
=======
	struct gfs2_sbd *sdp = gl->gl_name.ln_sbd;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (sdp->sd_jindex == gl->gl_object || sdp->sd_rindex == gl->gl_object)
		return 0;

<<<<<<< HEAD
	if (!list_empty(&gl->gl_holders)) {
		gh = list_entry(gl->gl_holders.next, struct gfs2_holder, gh_list);
		if (gh->gh_list.next != &gl->gl_holders)
			return 0;
	}

	return 1;
}

/**
 * gfs2_set_nlink - Set the inode's link count based on on-disk info
 * @inode: The inode in question
 * @nlink: The link count
 *
 * If the link count has hit zero, it must never be raised, whatever the
 * on-disk inode might say. When new struct inodes are created the link
 * count is set to 1, so that we can safely use this test even when reading
 * in on disk information for the first time.
 */

static void gfs2_set_nlink(struct inode *inode, u32 nlink)
{
	/*
	 * We will need to review setting the nlink count here in the
	 * light of the forthcoming ro bind mount work. This is a reminder
	 * to do that.
	 */
	if ((inode->i_nlink != nlink) && (inode->i_nlink != 0)) {
		if (nlink == 0)
			clear_nlink(inode);
		else
			set_nlink(inode, nlink);
	}
}

static int gfs2_dinode_in(struct gfs2_inode *ip, const void *buf)
{
	const struct gfs2_dinode *str = buf;
	struct timespec atime;
	u16 height, depth;

	if (unlikely(ip->i_no_addr != be64_to_cpu(str->di_num.no_addr)))
		goto corrupt;
	ip->i_no_formal_ino = be64_to_cpu(str->di_num.no_formal_ino);
	ip->i_inode.i_mode = be32_to_cpu(str->di_mode);
	ip->i_inode.i_rdev = 0;
	switch (ip->i_inode.i_mode & S_IFMT) {
	case S_IFBLK:
	case S_IFCHR:
		ip->i_inode.i_rdev = MKDEV(be32_to_cpu(str->di_major),
					   be32_to_cpu(str->di_minor));
		break;
	};

	ip->i_inode.i_uid = be32_to_cpu(str->di_uid);
	ip->i_inode.i_gid = be32_to_cpu(str->di_gid);
	gfs2_set_nlink(&ip->i_inode, be32_to_cpu(str->di_nlink));
	i_size_write(&ip->i_inode, be64_to_cpu(str->di_size));
	gfs2_set_inode_blocks(&ip->i_inode, be64_to_cpu(str->di_blocks));
	atime.tv_sec = be64_to_cpu(str->di_atime);
	atime.tv_nsec = be32_to_cpu(str->di_atime_nsec);
	if (timespec_compare(&ip->i_inode.i_atime, &atime) < 0)
		ip->i_inode.i_atime = atime;
	ip->i_inode.i_mtime.tv_sec = be64_to_cpu(str->di_mtime);
	ip->i_inode.i_mtime.tv_nsec = be32_to_cpu(str->di_mtime_nsec);
	ip->i_inode.i_ctime.tv_sec = be64_to_cpu(str->di_ctime);
	ip->i_inode.i_ctime.tv_nsec = be32_to_cpu(str->di_ctime_nsec);
=======
	return 1;
}

static int gfs2_dinode_in(struct gfs2_inode *ip, const void *buf)
{
	struct gfs2_sbd *sdp = GFS2_SB(&ip->i_inode);
	const struct gfs2_dinode *str = buf;
	struct timespec64 atime, iatime;
	u16 height, depth;
	umode_t mode = be32_to_cpu(str->di_mode);
	struct inode *inode = &ip->i_inode;
	bool is_new = inode->i_state & I_NEW;

	if (unlikely(ip->i_no_addr != be64_to_cpu(str->di_num.no_addr)))
		goto corrupt;
	if (unlikely(!is_new && inode_wrong_type(inode, mode)))
		goto corrupt;
	ip->i_no_formal_ino = be64_to_cpu(str->di_num.no_formal_ino);
	inode->i_mode = mode;
	if (is_new) {
		inode->i_rdev = 0;
		switch (mode & S_IFMT) {
		case S_IFBLK:
		case S_IFCHR:
			inode->i_rdev = MKDEV(be32_to_cpu(str->di_major),
					      be32_to_cpu(str->di_minor));
			break;
		}
	}

	i_uid_write(inode, be32_to_cpu(str->di_uid));
	i_gid_write(inode, be32_to_cpu(str->di_gid));
	set_nlink(inode, be32_to_cpu(str->di_nlink));
	i_size_write(inode, be64_to_cpu(str->di_size));
	gfs2_set_inode_blocks(inode, be64_to_cpu(str->di_blocks));
	atime.tv_sec = be64_to_cpu(str->di_atime);
	atime.tv_nsec = be32_to_cpu(str->di_atime_nsec);
	iatime = inode_get_atime(inode);
	if (timespec64_compare(&iatime, &atime) < 0)
		inode_set_atime_to_ts(inode, atime);
	inode_set_mtime(inode, be64_to_cpu(str->di_mtime),
			be32_to_cpu(str->di_mtime_nsec));
	inode_set_ctime(inode, be64_to_cpu(str->di_ctime),
			be32_to_cpu(str->di_ctime_nsec));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ip->i_goal = be64_to_cpu(str->di_goal_meta);
	ip->i_generation = be64_to_cpu(str->di_generation);

	ip->i_diskflags = be32_to_cpu(str->di_flags);
	ip->i_eattr = be64_to_cpu(str->di_eattr);
	/* i_diskflags and i_eattr must be set before gfs2_set_inode_flags() */
<<<<<<< HEAD
	gfs2_set_inode_flags(&ip->i_inode);
	height = be16_to_cpu(str->di_height);
	if (unlikely(height > GFS2_MAX_META_HEIGHT))
=======
	gfs2_set_inode_flags(inode);
	height = be16_to_cpu(str->di_height);
	if (unlikely(height > sdp->sd_max_height))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto corrupt;
	ip->i_height = (u8)height;

	depth = be16_to_cpu(str->di_depth);
	if (unlikely(depth > GFS2_DIR_MAX_DEPTH))
		goto corrupt;
	ip->i_depth = (u8)depth;
	ip->i_entries = be32_to_cpu(str->di_entries);

<<<<<<< HEAD
	if (S_ISREG(ip->i_inode.i_mode))
		gfs2_set_aops(&ip->i_inode);
=======
	if (gfs2_is_stuffed(ip) && inode->i_size > gfs2_max_stuffed_size(ip))
		goto corrupt;

	if (S_ISREG(inode->i_mode))
		gfs2_set_aops(inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
corrupt:
	gfs2_consist_inode(ip);
	return -EIO;
}

/**
 * gfs2_inode_refresh - Refresh the incore copy of the dinode
 * @ip: The GFS2 inode
 *
 * Returns: errno
 */

int gfs2_inode_refresh(struct gfs2_inode *ip)
{
	struct buffer_head *dibh;
	int error;

	error = gfs2_meta_inode_buffer(ip, &dibh);
	if (error)
		return error;

<<<<<<< HEAD
	if (gfs2_metatype_check(GFS2_SB(&ip->i_inode), dibh, GFS2_METATYPE_DI)) {
		brelse(dibh);
		return -EIO;
	}

	error = gfs2_dinode_in(ip, dibh->b_data);
	brelse(dibh);
	clear_bit(GIF_INVALID, &ip->i_flags);

=======
	error = gfs2_dinode_in(ip, dibh->b_data);
	brelse(dibh);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return error;
}

/**
<<<<<<< HEAD
 * inode_go_lock - operation done after an inode lock is locked by a process
 * @gl: the glock
 * @flags:
=======
 * inode_go_instantiate - read in an inode if necessary
 * @gl: The glock
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Returns: errno
 */

<<<<<<< HEAD
static int inode_go_lock(struct gfs2_holder *gh)
{
	struct gfs2_glock *gl = gh->gh_gl;
	struct gfs2_sbd *sdp = gl->gl_sbd;
	struct gfs2_inode *ip = gl->gl_object;
	int error = 0;

	if (!ip || (gh->gh_flags & GL_SKIP))
		return 0;

	if (test_bit(GIF_INVALID, &ip->i_flags)) {
		error = gfs2_inode_refresh(ip);
		if (error)
			return error;
	}

	if ((ip->i_diskflags & GFS2_DIF_TRUNC_IN_PROG) &&
	    (gl->gl_state == LM_ST_EXCLUSIVE) &&
	    (gh->gh_state == LM_ST_EXCLUSIVE)) {
		spin_lock(&sdp->sd_trunc_lock);
		if (list_empty(&ip->i_trunc_list))
			list_add(&sdp->sd_trunc_list, &ip->i_trunc_list);
		spin_unlock(&sdp->sd_trunc_lock);
		wake_up(&sdp->sd_quota_wait);
		return 1;
	}
=======
static int inode_go_instantiate(struct gfs2_glock *gl)
{
	struct gfs2_inode *ip = gl->gl_object;

	if (!ip) /* no inode to populate - read it in later */
		return 0;

	return gfs2_inode_refresh(ip);
}

static int inode_go_held(struct gfs2_holder *gh)
{
	struct gfs2_glock *gl = gh->gh_gl;
	struct gfs2_inode *ip = gl->gl_object;
	int error = 0;

	if (!ip) /* no inode to populate - read it in later */
		return 0;

	if (gh->gh_state != LM_ST_DEFERRED)
		inode_dio_wait(&ip->i_inode);

	if ((ip->i_diskflags & GFS2_DIF_TRUNC_IN_PROG) &&
	    (gl->gl_state == LM_ST_EXCLUSIVE) &&
	    (gh->gh_state == LM_ST_EXCLUSIVE))
		error = gfs2_truncatei_resume(ip);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return error;
}

/**
 * inode_go_dump - print information about an inode
 * @seq: The iterator
<<<<<<< HEAD
 * @ip: the inode
 *
 * Returns: 0 on success, -ENOBUFS when we run out of space
 */

static int inode_go_dump(struct seq_file *seq, const struct gfs2_glock *gl)
{
	const struct gfs2_inode *ip = gl->gl_object;
	if (ip == NULL)
		return 0;
	gfs2_print_dbg(seq, " I: n:%llu/%llu t:%u f:0x%02lx d:0x%08x s:%llu\n",
		  (unsigned long long)ip->i_no_formal_ino,
		  (unsigned long long)ip->i_no_addr,
		  IF2DT(ip->i_inode.i_mode), ip->i_flags,
		  (unsigned int)ip->i_diskflags,
		  (unsigned long long)i_size_read(&ip->i_inode));
	return 0;
}

/**
 * trans_go_sync - promote/demote the transaction glock
 * @gl: the glock
 * @state: the requested state
 * @flags:
 *
 */

static void trans_go_sync(struct gfs2_glock *gl)
{
	struct gfs2_sbd *sdp = gl->gl_sbd;

	if (gl->gl_state != LM_ST_UNLOCKED &&
	    test_bit(SDF_JOURNAL_LIVE, &sdp->sd_flags)) {
		gfs2_meta_syncfs(sdp);
		gfs2_log_shutdown(sdp);
=======
 * @gl: The glock
 * @fs_id_buf: file system id (may be empty)
 *
 */

static void inode_go_dump(struct seq_file *seq, const struct gfs2_glock *gl,
			  const char *fs_id_buf)
{
	struct gfs2_inode *ip = gl->gl_object;
	const struct inode *inode = &ip->i_inode;

	if (ip == NULL)
		return;

	gfs2_print_dbg(seq, "%s I: n:%llu/%llu t:%u f:0x%02lx d:0x%08x s:%llu "
		       "p:%lu\n", fs_id_buf,
		  (unsigned long long)ip->i_no_formal_ino,
		  (unsigned long long)ip->i_no_addr,
		  IF2DT(inode->i_mode), ip->i_flags,
		  (unsigned int)ip->i_diskflags,
		  (unsigned long long)i_size_read(inode),
		  inode->i_data.nrpages);
}

/**
 * freeze_go_callback - A cluster node is requesting a freeze
 * @gl: the glock
 * @remote: true if this came from a different cluster node
 */

static void freeze_go_callback(struct gfs2_glock *gl, bool remote)
{
	struct gfs2_sbd *sdp = gl->gl_name.ln_sbd;
	struct super_block *sb = sdp->sd_vfs;

	if (!remote ||
	    (gl->gl_state != LM_ST_SHARED &&
	     gl->gl_state != LM_ST_UNLOCKED) ||
	    gl->gl_demote_state != LM_ST_UNLOCKED)
		return;

	/*
	 * Try to get an active super block reference to prevent racing with
	 * unmount (see super_trylock_shared()).  But note that unmount isn't
	 * the only place where a write lock on s_umount is taken, and we can
	 * fail here because of things like remount as well.
	 */
	if (down_read_trylock(&sb->s_umount)) {
		atomic_inc(&sb->s_active);
		up_read(&sb->s_umount);
		if (!queue_work(gfs2_freeze_wq, &sdp->sd_freeze_work))
			deactivate_super(sb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

/**
<<<<<<< HEAD
 * trans_go_xmote_bh - After promoting/demoting the transaction glock
 * @gl: the glock
 *
 */

static int trans_go_xmote_bh(struct gfs2_glock *gl, struct gfs2_holder *gh)
{
	struct gfs2_sbd *sdp = gl->gl_sbd;
=======
 * freeze_go_xmote_bh - After promoting/demoting the freeze glock
 * @gl: the glock
 */
static int freeze_go_xmote_bh(struct gfs2_glock *gl)
{
	struct gfs2_sbd *sdp = gl->gl_name.ln_sbd;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct gfs2_inode *ip = GFS2_I(sdp->sd_jdesc->jd_inode);
	struct gfs2_glock *j_gl = ip->i_gl;
	struct gfs2_log_header_host head;
	int error;

	if (test_bit(SDF_JOURNAL_LIVE, &sdp->sd_flags)) {
		j_gl->gl_ops->go_inval(j_gl, DIO_METADATA);

<<<<<<< HEAD
		error = gfs2_find_jhead(sdp->sd_jdesc, &head);
		if (error)
			gfs2_consist(sdp);
		if (!(head.lh_flags & GFS2_LOG_HEAD_UNMOUNT))
			gfs2_consist(sdp);

		/*  Initialize some head of the log stuff  */
		if (!test_bit(SDF_SHUTDOWN, &sdp->sd_flags)) {
			sdp->sd_log_sequence = head.lh_sequence + 1;
			gfs2_log_pointers_init(sdp, head.lh_blkno);
		}
=======
		error = gfs2_find_jhead(sdp->sd_jdesc, &head, false);
		if (gfs2_assert_withdraw_delayed(sdp, !error))
			return error;
		if (gfs2_assert_withdraw_delayed(sdp, head.lh_flags &
						 GFS2_LOG_HEAD_UNMOUNT))
			return -EIO;
		sdp->sd_log_sequence = head.lh_sequence + 1;
		gfs2_log_pointers_init(sdp, head.lh_blkno);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return 0;
}

/**
<<<<<<< HEAD
 * trans_go_demote_ok
 * @gl: the glock
 *
 * Always returns 0
 */

static int trans_go_demote_ok(const struct gfs2_glock *gl)
{
	return 0;
}

/**
 * iopen_go_callback - schedule the dcache entry for the inode to be deleted
 * @gl: the glock
 *
 * gl_spin lock is held while calling this
 */
static void iopen_go_callback(struct gfs2_glock *gl)
{
	struct gfs2_inode *ip = (struct gfs2_inode *)gl->gl_object;
	struct gfs2_sbd *sdp = gl->gl_sbd;

	if (sdp->sd_vfs->s_flags & MS_RDONLY)
=======
 * iopen_go_callback - schedule the dcache entry for the inode to be deleted
 * @gl: the glock
 * @remote: true if this came from a different cluster node
 *
 * gl_lockref.lock lock is held while calling this
 */
static void iopen_go_callback(struct gfs2_glock *gl, bool remote)
{
	struct gfs2_inode *ip = gl->gl_object;
	struct gfs2_sbd *sdp = gl->gl_name.ln_sbd;

	if (!remote || sb_rdonly(sdp->sd_vfs) ||
	    test_bit(SDF_KILL, &sdp->sd_flags))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;

	if (gl->gl_demote_state == LM_ST_UNLOCKED &&
	    gl->gl_state == LM_ST_SHARED && ip) {
<<<<<<< HEAD
		gfs2_glock_hold(gl);
		if (queue_work(gfs2_delete_workqueue, &gl->gl_delete) == 0)
			gfs2_glock_put_nolock(gl);
	}
}

const struct gfs2_glock_operations gfs2_meta_glops = {
	.go_type = LM_TYPE_META,
};

const struct gfs2_glock_operations gfs2_inode_glops = {
	.go_xmote_th = inode_go_sync,
	.go_inval = inode_go_inval,
	.go_demote_ok = inode_go_demote_ok,
	.go_lock = inode_go_lock,
	.go_dump = inode_go_dump,
	.go_type = LM_TYPE_INODE,
	.go_flags = GLOF_ASPACE,
};

const struct gfs2_glock_operations gfs2_rgrp_glops = {
	.go_xmote_th = rgrp_go_sync,
	.go_inval = rgrp_go_inval,
	.go_lock = gfs2_rgrp_go_lock,
	.go_unlock = gfs2_rgrp_go_unlock,
	.go_dump = gfs2_rgrp_dump,
	.go_type = LM_TYPE_RGRP,
	.go_flags = GLOF_ASPACE,
};

const struct gfs2_glock_operations gfs2_trans_glops = {
	.go_xmote_th = trans_go_sync,
	.go_xmote_bh = trans_go_xmote_bh,
	.go_demote_ok = trans_go_demote_ok,
	.go_type = LM_TYPE_NONDISK,
=======
		gl->gl_lockref.count++;
		if (!gfs2_queue_try_to_evict(gl))
			gl->gl_lockref.count--;
	}
}

/**
 * inode_go_free - wake up anyone waiting for dlm's unlock ast to free it
 * @gl: glock being freed
 *
 * For now, this is only used for the journal inode glock. In withdraw
 * situations, we need to wait for the glock to be freed so that we know
 * other nodes may proceed with recovery / journal replay.
 */
static void inode_go_free(struct gfs2_glock *gl)
{
	/* Note that we cannot reference gl_object because it's already set
	 * to NULL by this point in its lifecycle. */
	if (!test_bit(GLF_FREEING, &gl->gl_flags))
		return;
	clear_bit_unlock(GLF_FREEING, &gl->gl_flags);
	wake_up_bit(&gl->gl_flags, GLF_FREEING);
}

/**
 * nondisk_go_callback - used to signal when a node did a withdraw
 * @gl: the nondisk glock
 * @remote: true if this came from a different cluster node
 *
 */
static void nondisk_go_callback(struct gfs2_glock *gl, bool remote)
{
	struct gfs2_sbd *sdp = gl->gl_name.ln_sbd;

	/* Ignore the callback unless it's from another node, and it's the
	   live lock. */
	if (!remote || gl->gl_name.ln_number != GFS2_LIVE_LOCK)
		return;

	/* First order of business is to cancel the demote request. We don't
	 * really want to demote a nondisk glock. At best it's just to inform
	 * us of another node's withdraw. We'll keep it in SH mode. */
	clear_bit(GLF_DEMOTE, &gl->gl_flags);
	clear_bit(GLF_PENDING_DEMOTE, &gl->gl_flags);

	/* Ignore the unlock if we're withdrawn, unmounting, or in recovery. */
	if (test_bit(SDF_NORECOVERY, &sdp->sd_flags) ||
	    test_bit(SDF_WITHDRAWN, &sdp->sd_flags) ||
	    test_bit(SDF_REMOTE_WITHDRAW, &sdp->sd_flags))
		return;

	/* We only care when a node wants us to unlock, because that means
	 * they want a journal recovered. */
	if (gl->gl_demote_state != LM_ST_UNLOCKED)
		return;

	if (sdp->sd_args.ar_spectator) {
		fs_warn(sdp, "Spectator node cannot recover journals.\n");
		return;
	}

	fs_warn(sdp, "Some node has withdrawn; checking for recovery.\n");
	set_bit(SDF_REMOTE_WITHDRAW, &sdp->sd_flags);
	/*
	 * We can't call remote_withdraw directly here or gfs2_recover_journal
	 * because this is called from the glock unlock function and the
	 * remote_withdraw needs to enqueue and dequeue the same "live" glock
	 * we were called from. So we queue it to the control work queue in
	 * lock_dlm.
	 */
	queue_delayed_work(gfs2_control_wq, &sdp->sd_control_work, 0);
}

const struct gfs2_glock_operations gfs2_meta_glops = {
	.go_type = LM_TYPE_META,
	.go_flags = GLOF_NONDISK,
};

const struct gfs2_glock_operations gfs2_inode_glops = {
	.go_sync = inode_go_sync,
	.go_inval = inode_go_inval,
	.go_demote_ok = inode_go_demote_ok,
	.go_instantiate = inode_go_instantiate,
	.go_held = inode_go_held,
	.go_dump = inode_go_dump,
	.go_type = LM_TYPE_INODE,
	.go_flags = GLOF_ASPACE | GLOF_LRU | GLOF_LVB,
	.go_free = inode_go_free,
};

const struct gfs2_glock_operations gfs2_rgrp_glops = {
	.go_sync = rgrp_go_sync,
	.go_inval = rgrp_go_inval,
	.go_instantiate = gfs2_rgrp_go_instantiate,
	.go_dump = gfs2_rgrp_go_dump,
	.go_type = LM_TYPE_RGRP,
	.go_flags = GLOF_LVB,
};

const struct gfs2_glock_operations gfs2_freeze_glops = {
	.go_xmote_bh = freeze_go_xmote_bh,
	.go_callback = freeze_go_callback,
	.go_type = LM_TYPE_NONDISK,
	.go_flags = GLOF_NONDISK,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

const struct gfs2_glock_operations gfs2_iopen_glops = {
	.go_type = LM_TYPE_IOPEN,
	.go_callback = iopen_go_callback,
<<<<<<< HEAD
=======
	.go_dump = inode_go_dump,
	.go_flags = GLOF_LRU | GLOF_NONDISK,
	.go_subclass = 1,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

const struct gfs2_glock_operations gfs2_flock_glops = {
	.go_type = LM_TYPE_FLOCK,
<<<<<<< HEAD
=======
	.go_flags = GLOF_LRU | GLOF_NONDISK,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

const struct gfs2_glock_operations gfs2_nondisk_glops = {
	.go_type = LM_TYPE_NONDISK,
<<<<<<< HEAD
=======
	.go_flags = GLOF_NONDISK,
	.go_callback = nondisk_go_callback,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

const struct gfs2_glock_operations gfs2_quota_glops = {
	.go_type = LM_TYPE_QUOTA,
<<<<<<< HEAD
=======
	.go_flags = GLOF_LVB | GLOF_LRU | GLOF_NONDISK,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

const struct gfs2_glock_operations gfs2_journal_glops = {
	.go_type = LM_TYPE_JOURNAL,
<<<<<<< HEAD
=======
	.go_flags = GLOF_NONDISK,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

const struct gfs2_glock_operations *gfs2_glops_list[] = {
	[LM_TYPE_META] = &gfs2_meta_glops,
	[LM_TYPE_INODE] = &gfs2_inode_glops,
	[LM_TYPE_RGRP] = &gfs2_rgrp_glops,
	[LM_TYPE_IOPEN] = &gfs2_iopen_glops,
	[LM_TYPE_FLOCK] = &gfs2_flock_glops,
	[LM_TYPE_NONDISK] = &gfs2_nondisk_glops,
	[LM_TYPE_QUOTA] = &gfs2_quota_glops,
	[LM_TYPE_JOURNAL] = &gfs2_journal_glops,
};

