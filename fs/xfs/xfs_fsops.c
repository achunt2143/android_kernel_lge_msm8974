<<<<<<< HEAD
/*
 * Copyright (c) 2000-2005 Silicon Graphics, Inc.
 * All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it would be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write the Free Software Foundation,
 * Inc.,  51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#include "xfs.h"
#include "xfs_fs.h"
#include "xfs_types.h"
#include "xfs_bit.h"
#include "xfs_inum.h"
#include "xfs_log.h"
#include "xfs_trans.h"
#include "xfs_sb.h"
#include "xfs_ag.h"
#include "xfs_mount.h"
#include "xfs_bmap_btree.h"
#include "xfs_alloc_btree.h"
#include "xfs_ialloc_btree.h"
#include "xfs_dinode.h"
#include "xfs_inode.h"
#include "xfs_inode_item.h"
#include "xfs_btree.h"
#include "xfs_error.h"
#include "xfs_alloc.h"
#include "xfs_ialloc.h"
#include "xfs_fsops.h"
#include "xfs_itable.h"
#include "xfs_trans_space.h"
#include "xfs_rtalloc.h"
#include "xfs_rw.h"
#include "xfs_filestream.h"
#include "xfs_trace.h"

/*
 * File system operations
 */

int
xfs_fs_geometry(
	xfs_mount_t		*mp,
	xfs_fsop_geom_t		*geo,
	int			new_version)
{

	memset(geo, 0, sizeof(*geo));

	geo->blocksize = mp->m_sb.sb_blocksize;
	geo->rtextsize = mp->m_sb.sb_rextsize;
	geo->agblocks = mp->m_sb.sb_agblocks;
	geo->agcount = mp->m_sb.sb_agcount;
	geo->logblocks = mp->m_sb.sb_logblocks;
	geo->sectsize = mp->m_sb.sb_sectsize;
	geo->inodesize = mp->m_sb.sb_inodesize;
	geo->imaxpct = mp->m_sb.sb_imax_pct;
	geo->datablocks = mp->m_sb.sb_dblocks;
	geo->rtblocks = mp->m_sb.sb_rblocks;
	geo->rtextents = mp->m_sb.sb_rextents;
	geo->logstart = mp->m_sb.sb_logstart;
	ASSERT(sizeof(geo->uuid)==sizeof(mp->m_sb.sb_uuid));
	memcpy(geo->uuid, &mp->m_sb.sb_uuid, sizeof(mp->m_sb.sb_uuid));
	if (new_version >= 2) {
		geo->sunit = mp->m_sb.sb_unit;
		geo->swidth = mp->m_sb.sb_width;
	}
	if (new_version >= 3) {
		geo->version = XFS_FSOP_GEOM_VERSION;
		geo->flags =
			(xfs_sb_version_hasattr(&mp->m_sb) ?
				XFS_FSOP_GEOM_FLAGS_ATTR : 0) |
			(xfs_sb_version_hasnlink(&mp->m_sb) ?
				XFS_FSOP_GEOM_FLAGS_NLINK : 0) |
			(xfs_sb_version_hasquota(&mp->m_sb) ?
				XFS_FSOP_GEOM_FLAGS_QUOTA : 0) |
			(xfs_sb_version_hasalign(&mp->m_sb) ?
				XFS_FSOP_GEOM_FLAGS_IALIGN : 0) |
			(xfs_sb_version_hasdalign(&mp->m_sb) ?
				XFS_FSOP_GEOM_FLAGS_DALIGN : 0) |
			(xfs_sb_version_hasshared(&mp->m_sb) ?
				XFS_FSOP_GEOM_FLAGS_SHARED : 0) |
			(xfs_sb_version_hasextflgbit(&mp->m_sb) ?
				XFS_FSOP_GEOM_FLAGS_EXTFLG : 0) |
			(xfs_sb_version_hasdirv2(&mp->m_sb) ?
				XFS_FSOP_GEOM_FLAGS_DIRV2 : 0) |
			(xfs_sb_version_hassector(&mp->m_sb) ?
				XFS_FSOP_GEOM_FLAGS_SECTOR : 0) |
			(xfs_sb_version_hasasciici(&mp->m_sb) ?
				XFS_FSOP_GEOM_FLAGS_DIRV2CI : 0) |
			(xfs_sb_version_haslazysbcount(&mp->m_sb) ?
				XFS_FSOP_GEOM_FLAGS_LAZYSB : 0) |
			(xfs_sb_version_hasattr2(&mp->m_sb) ?
				XFS_FSOP_GEOM_FLAGS_ATTR2 : 0);
		geo->logsectsize = xfs_sb_version_hassector(&mp->m_sb) ?
				mp->m_sb.sb_logsectsize : BBSIZE;
		geo->rtsectsize = mp->m_sb.sb_blocksize;
		geo->dirblocksize = mp->m_dirblksize;
	}
	if (new_version >= 4) {
		geo->flags |=
			(xfs_sb_version_haslogv2(&mp->m_sb) ?
				XFS_FSOP_GEOM_FLAGS_LOGV2 : 0);
		geo->logsunit = mp->m_sb.sb_logsunit;
	}
	return 0;
}

static int
xfs_growfs_data_private(
	xfs_mount_t		*mp,		/* mount point for filesystem */
	xfs_growfs_data_t	*in)		/* growfs data input struct */
{
	xfs_agf_t		*agf;
	xfs_agi_t		*agi;
	xfs_agnumber_t		agno;
	xfs_extlen_t		agsize;
	xfs_extlen_t		tmpsize;
	xfs_alloc_rec_t		*arec;
	struct xfs_btree_block	*block;
	xfs_buf_t		*bp;
	int			bucket;
	int			dpct;
	int			error;
	xfs_agnumber_t		nagcount;
	xfs_agnumber_t		nagimax = 0;
	xfs_rfsblock_t		nb, nb_mod;
	xfs_rfsblock_t		new;
	xfs_rfsblock_t		nfree;
	xfs_agnumber_t		oagcount;
	int			pct;
	xfs_trans_t		*tp;

	nb = in->newblocks;
	pct = in->imaxpct;
	if (nb < mp->m_sb.sb_dblocks || pct < 0 || pct > 100)
		return XFS_ERROR(EINVAL);
	if ((error = xfs_sb_validate_fsb_count(&mp->m_sb, nb)))
		return error;
	dpct = pct - mp->m_sb.sb_imax_pct;
	bp = xfs_buf_read_uncached(mp, mp->m_ddev_targp,
				XFS_FSB_TO_BB(mp, nb) - XFS_FSS_TO_BB(mp, 1),
				BBTOB(XFS_FSS_TO_BB(mp, 1)), 0);
	if (!bp)
		return EIO;
	xfs_buf_relse(bp);

	new = nb;	/* use new as a temporary here */
	nb_mod = do_div(new, mp->m_sb.sb_agblocks);
	nagcount = new + (nb_mod != 0);
	if (nb_mod && nb_mod < XFS_MIN_AG_BLOCKS) {
		nagcount--;
		nb = (xfs_rfsblock_t)nagcount * mp->m_sb.sb_agblocks;
		if (nb < mp->m_sb.sb_dblocks)
			return XFS_ERROR(EINVAL);
	}
	new = nb - mp->m_sb.sb_dblocks;
	oagcount = mp->m_sb.sb_agcount;

	/* allocate the new per-ag structures */
	if (nagcount > oagcount) {
		error = xfs_initialize_perag(mp, nagcount, &nagimax);
		if (error)
			return error;
	}

	tp = xfs_trans_alloc(mp, XFS_TRANS_GROWFS);
	tp->t_flags |= XFS_TRANS_RESERVE;
	if ((error = xfs_trans_reserve(tp, XFS_GROWFS_SPACE_RES(mp),
			XFS_GROWDATA_LOG_RES(mp), 0, 0, 0))) {
		xfs_trans_cancel(tp, 0);
		return error;
	}

	/*
	 * Write new AG headers to disk. Non-transactional, but written
	 * synchronously so they are completed prior to the growfs transaction
	 * being logged.
	 */
	nfree = 0;
	for (agno = nagcount - 1; agno >= oagcount; agno--, new -= agsize) {
		/*
		 * AG freelist header block
		 */
		bp = xfs_buf_get(mp->m_ddev_targp,
				 XFS_AG_DADDR(mp, agno, XFS_AGF_DADDR(mp)),
				 XFS_FSS_TO_BB(mp, 1), XBF_LOCK | XBF_MAPPED);
		if (!bp) {
			error = ENOMEM;
			goto error0;
		}
		agf = XFS_BUF_TO_AGF(bp);
		memset(agf, 0, mp->m_sb.sb_sectsize);
		agf->agf_magicnum = cpu_to_be32(XFS_AGF_MAGIC);
		agf->agf_versionnum = cpu_to_be32(XFS_AGF_VERSION);
		agf->agf_seqno = cpu_to_be32(agno);
		if (agno == nagcount - 1)
			agsize =
				nb -
				(agno * (xfs_rfsblock_t)mp->m_sb.sb_agblocks);
		else
			agsize = mp->m_sb.sb_agblocks;
		agf->agf_length = cpu_to_be32(agsize);
		agf->agf_roots[XFS_BTNUM_BNOi] = cpu_to_be32(XFS_BNO_BLOCK(mp));
		agf->agf_roots[XFS_BTNUM_CNTi] = cpu_to_be32(XFS_CNT_BLOCK(mp));
		agf->agf_levels[XFS_BTNUM_BNOi] = cpu_to_be32(1);
		agf->agf_levels[XFS_BTNUM_CNTi] = cpu_to_be32(1);
		agf->agf_flfirst = 0;
		agf->agf_fllast = cpu_to_be32(XFS_AGFL_SIZE(mp) - 1);
		agf->agf_flcount = 0;
		tmpsize = agsize - XFS_PREALLOC_BLOCKS(mp);
		agf->agf_freeblks = cpu_to_be32(tmpsize);
		agf->agf_longest = cpu_to_be32(tmpsize);
		error = xfs_bwrite(bp);
		xfs_buf_relse(bp);
		if (error)
			goto error0;

		/*
		 * AG inode header block
		 */
		bp = xfs_buf_get(mp->m_ddev_targp,
				 XFS_AG_DADDR(mp, agno, XFS_AGI_DADDR(mp)),
				 XFS_FSS_TO_BB(mp, 1), XBF_LOCK | XBF_MAPPED);
		if (!bp) {
			error = ENOMEM;
			goto error0;
		}
		agi = XFS_BUF_TO_AGI(bp);
		memset(agi, 0, mp->m_sb.sb_sectsize);
		agi->agi_magicnum = cpu_to_be32(XFS_AGI_MAGIC);
		agi->agi_versionnum = cpu_to_be32(XFS_AGI_VERSION);
		agi->agi_seqno = cpu_to_be32(agno);
		agi->agi_length = cpu_to_be32(agsize);
		agi->agi_count = 0;
		agi->agi_root = cpu_to_be32(XFS_IBT_BLOCK(mp));
		agi->agi_level = cpu_to_be32(1);
		agi->agi_freecount = 0;
		agi->agi_newino = cpu_to_be32(NULLAGINO);
		agi->agi_dirino = cpu_to_be32(NULLAGINO);
		for (bucket = 0; bucket < XFS_AGI_UNLINKED_BUCKETS; bucket++)
			agi->agi_unlinked[bucket] = cpu_to_be32(NULLAGINO);
		error = xfs_bwrite(bp);
		xfs_buf_relse(bp);
		if (error)
			goto error0;

		/*
		 * BNO btree root block
		 */
		bp = xfs_buf_get(mp->m_ddev_targp,
				 XFS_AGB_TO_DADDR(mp, agno, XFS_BNO_BLOCK(mp)),
				 BTOBB(mp->m_sb.sb_blocksize),
				 XBF_LOCK | XBF_MAPPED);
		if (!bp) {
			error = ENOMEM;
			goto error0;
		}
		block = XFS_BUF_TO_BLOCK(bp);
		memset(block, 0, mp->m_sb.sb_blocksize);
		block->bb_magic = cpu_to_be32(XFS_ABTB_MAGIC);
		block->bb_level = 0;
		block->bb_numrecs = cpu_to_be16(1);
		block->bb_u.s.bb_leftsib = cpu_to_be32(NULLAGBLOCK);
		block->bb_u.s.bb_rightsib = cpu_to_be32(NULLAGBLOCK);
		arec = XFS_ALLOC_REC_ADDR(mp, block, 1);
		arec->ar_startblock = cpu_to_be32(XFS_PREALLOC_BLOCKS(mp));
		arec->ar_blockcount = cpu_to_be32(
			agsize - be32_to_cpu(arec->ar_startblock));
		error = xfs_bwrite(bp);
		xfs_buf_relse(bp);
		if (error)
			goto error0;

		/*
		 * CNT btree root block
		 */
		bp = xfs_buf_get(mp->m_ddev_targp,
				 XFS_AGB_TO_DADDR(mp, agno, XFS_CNT_BLOCK(mp)),
				 BTOBB(mp->m_sb.sb_blocksize),
				 XBF_LOCK | XBF_MAPPED);
		if (!bp) {
			error = ENOMEM;
			goto error0;
		}
		block = XFS_BUF_TO_BLOCK(bp);
		memset(block, 0, mp->m_sb.sb_blocksize);
		block->bb_magic = cpu_to_be32(XFS_ABTC_MAGIC);
		block->bb_level = 0;
		block->bb_numrecs = cpu_to_be16(1);
		block->bb_u.s.bb_leftsib = cpu_to_be32(NULLAGBLOCK);
		block->bb_u.s.bb_rightsib = cpu_to_be32(NULLAGBLOCK);
		arec = XFS_ALLOC_REC_ADDR(mp, block, 1);
		arec->ar_startblock = cpu_to_be32(XFS_PREALLOC_BLOCKS(mp));
		arec->ar_blockcount = cpu_to_be32(
			agsize - be32_to_cpu(arec->ar_startblock));
		nfree += be32_to_cpu(arec->ar_blockcount);
		error = xfs_bwrite(bp);
		xfs_buf_relse(bp);
		if (error)
			goto error0;

		/*
		 * INO btree root block
		 */
		bp = xfs_buf_get(mp->m_ddev_targp,
				 XFS_AGB_TO_DADDR(mp, agno, XFS_IBT_BLOCK(mp)),
				 BTOBB(mp->m_sb.sb_blocksize),
				 XBF_LOCK | XBF_MAPPED);
		if (!bp) {
			error = ENOMEM;
			goto error0;
		}
		block = XFS_BUF_TO_BLOCK(bp);
		memset(block, 0, mp->m_sb.sb_blocksize);
		block->bb_magic = cpu_to_be32(XFS_IBT_MAGIC);
		block->bb_level = 0;
		block->bb_numrecs = 0;
		block->bb_u.s.bb_leftsib = cpu_to_be32(NULLAGBLOCK);
		block->bb_u.s.bb_rightsib = cpu_to_be32(NULLAGBLOCK);
		error = xfs_bwrite(bp);
		xfs_buf_relse(bp);
		if (error)
			goto error0;
	}
	xfs_trans_agblocks_delta(tp, nfree);
	/*
	 * There are new blocks in the old last a.g.
	 */
	if (new) {
		/*
		 * Change the agi length.
		 */
		error = xfs_ialloc_read_agi(mp, tp, agno, &bp);
		if (error) {
			goto error0;
		}
		ASSERT(bp);
		agi = XFS_BUF_TO_AGI(bp);
		be32_add_cpu(&agi->agi_length, new);
		ASSERT(nagcount == oagcount ||
		       be32_to_cpu(agi->agi_length) == mp->m_sb.sb_agblocks);
		xfs_ialloc_log_agi(tp, bp, XFS_AGI_LENGTH);
		/*
		 * Change agf length.
		 */
		error = xfs_alloc_read_agf(mp, tp, agno, 0, &bp);
		if (error) {
			goto error0;
		}
		ASSERT(bp);
		agf = XFS_BUF_TO_AGF(bp);
		be32_add_cpu(&agf->agf_length, new);
		ASSERT(be32_to_cpu(agf->agf_length) ==
		       be32_to_cpu(agi->agi_length));

		xfs_alloc_log_agf(tp, bp, XFS_AGF_LENGTH);
		/*
		 * Free the new space.
		 */
		error = xfs_free_extent(tp, XFS_AGB_TO_FSB(mp, agno,
			be32_to_cpu(agf->agf_length) - new), new);
		if (error) {
			goto error0;
		}
	}
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2000-2005 Silicon Graphics, Inc.
 * All Rights Reserved.
 */
#include "xfs.h"
#include "xfs_fs.h"
#include "xfs_shared.h"
#include "xfs_format.h"
#include "xfs_log_format.h"
#include "xfs_trans_resv.h"
#include "xfs_sb.h"
#include "xfs_mount.h"
#include "xfs_trans.h"
#include "xfs_error.h"
#include "xfs_alloc.h"
#include "xfs_fsops.h"
#include "xfs_trans_space.h"
#include "xfs_log.h"
#include "xfs_log_priv.h"
#include "xfs_ag.h"
#include "xfs_ag_resv.h"
#include "xfs_trace.h"

/*
 * Write new AG headers to disk. Non-transactional, but need to be
 * written and completed prior to the growfs transaction being logged.
 * To do this, we use a delayed write buffer list and wait for
 * submission and IO completion of the list as a whole. This allows the
 * IO subsystem to merge all the AG headers in a single AG into a single
 * IO and hide most of the latency of the IO from us.
 *
 * This also means that if we get an error whilst building the buffer
 * list to write, we can cancel the entire list without having written
 * anything.
 */
static int
xfs_resizefs_init_new_ags(
	struct xfs_trans	*tp,
	struct aghdr_init_data	*id,
	xfs_agnumber_t		oagcount,
	xfs_agnumber_t		nagcount,
	xfs_rfsblock_t		delta,
	struct xfs_perag	*last_pag,
	bool			*lastag_extended)
{
	struct xfs_mount	*mp = tp->t_mountp;
	xfs_rfsblock_t		nb = mp->m_sb.sb_dblocks + delta;
	int			error;

	*lastag_extended = false;

	INIT_LIST_HEAD(&id->buffer_list);
	for (id->agno = nagcount - 1;
	     id->agno >= oagcount;
	     id->agno--, delta -= id->agsize) {

		if (id->agno == nagcount - 1)
			id->agsize = nb - (id->agno *
					(xfs_rfsblock_t)mp->m_sb.sb_agblocks);
		else
			id->agsize = mp->m_sb.sb_agblocks;

		error = xfs_ag_init_headers(mp, id);
		if (error) {
			xfs_buf_delwri_cancel(&id->buffer_list);
			return error;
		}
	}

	error = xfs_buf_delwri_submit(&id->buffer_list);
	if (error)
		return error;

	if (delta) {
		*lastag_extended = true;
		error = xfs_ag_extend_space(last_pag, tp, delta);
	}
	return error;
}

/*
 * growfs operations
 */
static int
xfs_growfs_data_private(
	struct xfs_mount	*mp,		/* mount point for filesystem */
	struct xfs_growfs_data	*in)		/* growfs data input struct */
{
	struct xfs_buf		*bp;
	int			error;
	xfs_agnumber_t		nagcount;
	xfs_agnumber_t		nagimax = 0;
	xfs_rfsblock_t		nb, nb_div, nb_mod;
	int64_t			delta;
	bool			lastag_extended = false;
	xfs_agnumber_t		oagcount;
	struct xfs_trans	*tp;
	struct aghdr_init_data	id = {};
	struct xfs_perag	*last_pag;

	nb = in->newblocks;
	error = xfs_sb_validate_fsb_count(&mp->m_sb, nb);
	if (error)
		return error;

	if (nb > mp->m_sb.sb_dblocks) {
		error = xfs_buf_read_uncached(mp->m_ddev_targp,
				XFS_FSB_TO_BB(mp, nb) - XFS_FSS_TO_BB(mp, 1),
				XFS_FSS_TO_BB(mp, 1), 0, &bp, NULL);
		if (error)
			return error;
		xfs_buf_relse(bp);
	}

	nb_div = nb;
	nb_mod = do_div(nb_div, mp->m_sb.sb_agblocks);
	if (nb_mod && nb_mod >= XFS_MIN_AG_BLOCKS)
		nb_div++;
	else if (nb_mod)
		nb = nb_div * mp->m_sb.sb_agblocks;

	if (nb_div > XFS_MAX_AGNUMBER + 1) {
		nb_div = XFS_MAX_AGNUMBER + 1;
		nb = nb_div * mp->m_sb.sb_agblocks;
	}
	nagcount = nb_div;
	delta = nb - mp->m_sb.sb_dblocks;
	/*
	 * Reject filesystems with a single AG because they are not
	 * supported, and reject a shrink operation that would cause a
	 * filesystem to become unsupported.
	 */
	if (delta < 0 && nagcount < 2)
		return -EINVAL;

	/* No work to do */
	if (delta == 0)
		return 0;

	oagcount = mp->m_sb.sb_agcount;
	/* allocate the new per-ag structures */
	if (nagcount > oagcount) {
		error = xfs_initialize_perag(mp, nagcount, nb, &nagimax);
		if (error)
			return error;
	} else if (nagcount < oagcount) {
		/* TODO: shrinking the entire AGs hasn't yet completed */
		return -EINVAL;
	}

	if (delta > 0)
		error = xfs_trans_alloc(mp, &M_RES(mp)->tr_growdata,
				XFS_GROWFS_SPACE_RES(mp), 0, XFS_TRANS_RESERVE,
				&tp);
	else
		error = xfs_trans_alloc(mp, &M_RES(mp)->tr_growdata, -delta, 0,
				0, &tp);
	if (error)
		goto out_free_unused_perag;

	last_pag = xfs_perag_get(mp, oagcount - 1);
	if (delta > 0) {
		error = xfs_resizefs_init_new_ags(tp, &id, oagcount, nagcount,
				delta, last_pag, &lastag_extended);
	} else {
		xfs_warn_mount(mp, XFS_OPSTATE_WARNED_SHRINK,
	"EXPERIMENTAL online shrink feature in use. Use at your own risk!");

		error = xfs_ag_shrink_space(last_pag, &tp, -delta);
	}
	xfs_perag_put(last_pag);
	if (error)
		goto out_trans_cancel;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Update changed superblock fields transactionally. These are not
	 * seen by the rest of the world until the transaction commit applies
	 * them atomically to the superblock.
	 */
	if (nagcount > oagcount)
		xfs_trans_mod_sb(tp, XFS_TRANS_SB_AGCOUNT, nagcount - oagcount);
<<<<<<< HEAD
	if (nb > mp->m_sb.sb_dblocks)
		xfs_trans_mod_sb(tp, XFS_TRANS_SB_DBLOCKS,
				 nb - mp->m_sb.sb_dblocks);
	if (nfree)
		xfs_trans_mod_sb(tp, XFS_TRANS_SB_FDBLOCKS, nfree);
	if (dpct)
		xfs_trans_mod_sb(tp, XFS_TRANS_SB_IMAXPCT, dpct);
	error = xfs_trans_commit(tp, 0);
=======
	if (delta)
		xfs_trans_mod_sb(tp, XFS_TRANS_SB_DBLOCKS, delta);
	if (id.nfree)
		xfs_trans_mod_sb(tp, XFS_TRANS_SB_FDBLOCKS, id.nfree);

	/*
	 * Sync sb counters now to reflect the updated values. This is
	 * particularly important for shrink because the write verifier
	 * will fail if sb_fdblocks is ever larger than sb_dblocks.
	 */
	if (xfs_has_lazysbcount(mp))
		xfs_log_sb(tp);

	xfs_trans_set_sync(tp);
	error = xfs_trans_commit(tp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (error)
		return error;

	/* New allocation groups fully initialized, so update mount struct */
	if (nagimax)
		mp->m_maxagi = nagimax;
<<<<<<< HEAD
	if (mp->m_sb.sb_imax_pct) {
		__uint64_t icount = mp->m_sb.sb_dblocks * mp->m_sb.sb_imax_pct;
		do_div(icount, 100);
		mp->m_maxicount = icount << mp->m_sb.sb_inopblog;
	} else
		mp->m_maxicount = 0;
	xfs_set_low_space_thresholds(mp);

	/* update secondary superblocks. */
	for (agno = 1; agno < nagcount; agno++) {
		error = xfs_read_buf(mp, mp->m_ddev_targp,
				  XFS_AGB_TO_DADDR(mp, agno, XFS_SB_BLOCK(mp)),
				  XFS_FSS_TO_BB(mp, 1), 0, &bp);
		if (error) {
			xfs_warn(mp,
		"error %d reading secondary superblock for ag %d",
				error, agno);
			break;
		}
		xfs_sb_to_disk(XFS_BUF_TO_SBP(bp), &mp->m_sb, XFS_SB_ALL_BITS);
		/*
		 * If we get an error writing out the alternate superblocks,
		 * just issue a warning and continue.  The real work is
		 * already done and committed.
		 */
		error = xfs_bwrite(bp);
		xfs_buf_relse(bp);
		if (error) {
			xfs_warn(mp,
		"write error %d updating secondary superblock for ag %d",
				error, agno);
			break; /* no point in continuing */
		}
	}
	return 0;

 error0:
	xfs_trans_cancel(tp, XFS_TRANS_ABORT);
=======
	xfs_set_low_space_thresholds(mp);
	mp->m_alloc_set_aside = xfs_alloc_set_aside(mp);

	if (delta > 0) {
		/*
		 * If we expanded the last AG, free the per-AG reservation
		 * so we can reinitialize it with the new size.
		 */
		if (lastag_extended) {
			struct xfs_perag	*pag;

			pag = xfs_perag_get(mp, id.agno);
			error = xfs_ag_resv_free(pag);
			xfs_perag_put(pag);
			if (error)
				return error;
		}
		/*
		 * Reserve AG metadata blocks. ENOSPC here does not mean there
		 * was a growfs failure, just that there still isn't space for
		 * new user data after the grow has been run.
		 */
		error = xfs_fs_reserve_ag_blocks(mp);
		if (error == -ENOSPC)
			error = 0;
	}
	return error;

out_trans_cancel:
	xfs_trans_cancel(tp);
out_free_unused_perag:
	if (nagcount > oagcount)
		xfs_free_unused_perag_range(mp, oagcount, nagcount);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return error;
}

static int
xfs_growfs_log_private(
<<<<<<< HEAD
	xfs_mount_t		*mp,	/* mount point for filesystem */
	xfs_growfs_log_t	*in)	/* growfs log input struct */
=======
	struct xfs_mount	*mp,	/* mount point for filesystem */
	struct xfs_growfs_log	*in)	/* growfs log input struct */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	xfs_extlen_t		nb;

	nb = in->newblocks;
	if (nb < XFS_MIN_LOG_BLOCKS || nb < XFS_B_TO_FSB(mp, XFS_MIN_LOG_BYTES))
<<<<<<< HEAD
		return XFS_ERROR(EINVAL);
	if (nb == mp->m_sb.sb_logblocks &&
	    in->isint == (mp->m_sb.sb_logstart != 0))
		return XFS_ERROR(EINVAL);
=======
		return -EINVAL;
	if (nb == mp->m_sb.sb_logblocks &&
	    in->isint == (mp->m_sb.sb_logstart != 0))
		return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Moving the log is hard, need new interfaces to sync
	 * the log first, hold off all activity while moving it.
	 * Can have shorter or longer log in the same space,
	 * or transform internal to external log or vice versa.
	 */
<<<<<<< HEAD
	return XFS_ERROR(ENOSYS);
=======
	return -ENOSYS;
}

static int
xfs_growfs_imaxpct(
	struct xfs_mount	*mp,
	__u32			imaxpct)
{
	struct xfs_trans	*tp;
	int			dpct;
	int			error;

	if (imaxpct > 100)
		return -EINVAL;

	error = xfs_trans_alloc(mp, &M_RES(mp)->tr_growdata,
			XFS_GROWFS_SPACE_RES(mp), 0, XFS_TRANS_RESERVE, &tp);
	if (error)
		return error;

	dpct = imaxpct - mp->m_sb.sb_imax_pct;
	xfs_trans_mod_sb(tp, XFS_TRANS_SB_IMAXPCT, dpct);
	xfs_trans_set_sync(tp);
	return xfs_trans_commit(tp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * protected versions of growfs function acquire and release locks on the mount
 * point - exported through ioctls: XFS_IOC_FSGROWFSDATA, XFS_IOC_FSGROWFSLOG,
 * XFS_IOC_FSGROWFSRT
 */
<<<<<<< HEAD


int
xfs_growfs_data(
	xfs_mount_t		*mp,
	xfs_growfs_data_t	*in)
{
	int error;

	if (!capable(CAP_SYS_ADMIN))
		return XFS_ERROR(EPERM);
	if (!mutex_trylock(&mp->m_growlock))
		return XFS_ERROR(EWOULDBLOCK);
	error = xfs_growfs_data_private(mp, in);
=======
int
xfs_growfs_data(
	struct xfs_mount	*mp,
	struct xfs_growfs_data	*in)
{
	int			error = 0;

	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;
	if (!mutex_trylock(&mp->m_growlock))
		return -EWOULDBLOCK;

	/* update imaxpct separately to the physical grow of the filesystem */
	if (in->imaxpct != mp->m_sb.sb_imax_pct) {
		error = xfs_growfs_imaxpct(mp, in->imaxpct);
		if (error)
			goto out_error;
	}

	if (in->newblocks != mp->m_sb.sb_dblocks) {
		error = xfs_growfs_data_private(mp, in);
		if (error)
			goto out_error;
	}

	/* Post growfs calculations needed to reflect new state in operations */
	if (mp->m_sb.sb_imax_pct) {
		uint64_t icount = mp->m_sb.sb_dblocks * mp->m_sb.sb_imax_pct;
		do_div(icount, 100);
		M_IGEO(mp)->maxicount = XFS_FSB_TO_INO(mp, icount);
	} else
		M_IGEO(mp)->maxicount = 0;

	/* Update secondary superblocks now the physical grow has completed */
	error = xfs_update_secondary_sbs(mp);

out_error:
	/*
	 * Increment the generation unconditionally, the error could be from
	 * updating the secondary superblocks, in which case the new size
	 * is live already.
	 */
	mp->m_generation++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mutex_unlock(&mp->m_growlock);
	return error;
}

int
xfs_growfs_log(
	xfs_mount_t		*mp,
<<<<<<< HEAD
	xfs_growfs_log_t	*in)
=======
	struct xfs_growfs_log	*in)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int error;

	if (!capable(CAP_SYS_ADMIN))
<<<<<<< HEAD
		return XFS_ERROR(EPERM);
	if (!mutex_trylock(&mp->m_growlock))
		return XFS_ERROR(EWOULDBLOCK);
=======
		return -EPERM;
	if (!mutex_trylock(&mp->m_growlock))
		return -EWOULDBLOCK;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	error = xfs_growfs_log_private(mp, in);
	mutex_unlock(&mp->m_growlock);
	return error;
}

/*
<<<<<<< HEAD
 * exported through ioctl XFS_IOC_FSCOUNTS
 */

int
xfs_fs_counts(
	xfs_mount_t		*mp,
	xfs_fsop_counts_t	*cnt)
{
	xfs_icsb_sync_counters(mp, XFS_ICSB_LAZY_COUNT);
	spin_lock(&mp->m_sb_lock);
	cnt->freedata = mp->m_sb.sb_fdblocks - XFS_ALLOC_SET_ASIDE(mp);
	cnt->freertx = mp->m_sb.sb_frextents;
	cnt->freeino = mp->m_sb.sb_ifree;
	cnt->allocino = mp->m_sb.sb_icount;
	spin_unlock(&mp->m_sb_lock);
	return 0;
}

/*
 * exported through ioctl XFS_IOC_SET_RESBLKS & XFS_IOC_GET_RESBLKS
 *
 * xfs_reserve_blocks is called to set m_resblks
 * in the in-core mount table. The number of unused reserved blocks
 * is kept in m_resblks_avail.
 *
 * Reserve the requested number of blocks if available. Otherwise return
 * as many as possible to satisfy the request. The actual number
 * reserved are returned in outval
 *
 * A null inval pointer indicates that only the current reserved blocks
 * available  should  be returned no settings are changed.
 */

int
xfs_reserve_blocks(
	xfs_mount_t             *mp,
	__uint64_t              *inval,
	xfs_fsop_resblks_t      *outval)
{
	__int64_t		lcounter, delta, fdblks_delta;
	__uint64_t		request;

	/* If inval is null, report current values and return */
	if (inval == (__uint64_t *)NULL) {
		if (!outval)
			return EINVAL;
		outval->resblks = mp->m_resblks;
		outval->resblks_avail = mp->m_resblks_avail;
		return 0;
	}

	request = *inval;

	/*
	 * With per-cpu counters, this becomes an interesting
	 * problem. we needto work out if we are freeing or allocation
	 * blocks first, then we can do the modification as necessary.
	 *
	 * We do this under the m_sb_lock so that if we are near
	 * ENOSPC, we will hold out any changes while we work out
	 * what to do. This means that the amount of free space can
	 * change while we do this, so we need to retry if we end up
	 * trying to reserve more space than is available.
	 *
	 * We also use the xfs_mod_incore_sb() interface so that we
	 * don't have to care about whether per cpu counter are
	 * enabled, disabled or even compiled in....
	 */
retry:
	spin_lock(&mp->m_sb_lock);
	xfs_icsb_sync_counters_locked(mp, 0);

	/*
	 * If our previous reservation was larger than the current value,
	 * then move any unused blocks back to the free pool.
	 */
	fdblks_delta = 0;
=======
 * Reserve the requested number of blocks if available. Otherwise return
 * as many as possible to satisfy the request. The actual number
 * reserved are returned in outval.
 */
int
xfs_reserve_blocks(
	struct xfs_mount	*mp,
	uint64_t		request)
{
	int64_t			lcounter, delta;
	int64_t			fdblks_delta = 0;
	int64_t			free;
	int			error = 0;

	/*
	 * With per-cpu counters, this becomes an interesting problem. we need
	 * to work out if we are freeing or allocation blocks first, then we can
	 * do the modification as necessary.
	 *
	 * We do this under the m_sb_lock so that if we are near ENOSPC, we will
	 * hold out any changes while we work out what to do. This means that
	 * the amount of free space can change while we do this, so we need to
	 * retry if we end up trying to reserve more space than is available.
	 */
	spin_lock(&mp->m_sb_lock);

	/*
	 * If our previous reservation was larger than the current value,
	 * then move any unused blocks back to the free pool. Modify the resblks
	 * counters directly since we shouldn't have any problems unreserving
	 * space.
	 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (mp->m_resblks > request) {
		lcounter = mp->m_resblks_avail - request;
		if (lcounter  > 0) {		/* release unused blocks */
			fdblks_delta = lcounter;
			mp->m_resblks_avail -= lcounter;
		}
		mp->m_resblks = request;
<<<<<<< HEAD
	} else {
		__int64_t	free;

		free =  mp->m_sb.sb_fdblocks - XFS_ALLOC_SET_ASIDE(mp);
		if (!free)
			goto out; /* ENOSPC and fdblks_delta = 0 */

		delta = request - mp->m_resblks;
		lcounter = free - delta;
		if (lcounter < 0) {
			/* We can't satisfy the request, just get what we can */
			mp->m_resblks += free;
			mp->m_resblks_avail += free;
			fdblks_delta = -free;
		} else {
			fdblks_delta = -delta;
			mp->m_resblks = request;
			mp->m_resblks_avail += delta;
		}
	}
out:
	if (outval) {
		outval->resblks = mp->m_resblks;
		outval->resblks_avail = mp->m_resblks_avail;
	}
	spin_unlock(&mp->m_sb_lock);

	if (fdblks_delta) {
		/*
		 * If we are putting blocks back here, m_resblks_avail is
		 * already at its max so this will put it in the free pool.
		 *
		 * If we need space, we'll either succeed in getting it
		 * from the free block count or we'll get an enospc. If
		 * we get a ENOSPC, it means things changed while we were
		 * calculating fdblks_delta and so we should try again to
		 * see if there is anything left to reserve.
		 *
		 * Don't set the reserved flag here - we don't want to reserve
		 * the extra reserve blocks from the reserve.....
		 */
		int error;
		error = xfs_icsb_modify_counters(mp, XFS_SBS_FDBLOCKS,
						 fdblks_delta, 0);
		if (error == ENOSPC)
			goto retry;
	}
	return 0;
}

/*
 * Dump a transaction into the log that contains no real change. This is needed
 * to be able to make the log dirty or stamp the current tail LSN into the log
 * during the covering operation.
 *
 * We cannot use an inode here for this - that will push dirty state back up
 * into the VFS and then periodic inode flushing will prevent log covering from
 * making progress. Hence we log a field in the superblock instead and use a
 * synchronous transaction to ensure the superblock is immediately unpinned
 * and can be written back.
 */
int
xfs_fs_log_dummy(
	xfs_mount_t	*mp)
{
	xfs_trans_t	*tp;
	int		error;

	tp = _xfs_trans_alloc(mp, XFS_TRANS_DUMMY1, KM_SLEEP);
	error = xfs_trans_reserve(tp, 0, mp->m_sb.sb_sectsize + 128, 0, 0,
					XFS_DEFAULT_LOG_COUNT);
	if (error) {
		xfs_trans_cancel(tp, 0);
		return error;
	}

	/* log the UUID because it is an unchanging field */
	xfs_mod_sb(tp, XFS_SB_UUID);
	xfs_trans_set_sync(tp);
	return xfs_trans_commit(tp, 0);
=======
		if (fdblks_delta) {
			spin_unlock(&mp->m_sb_lock);
			error = xfs_mod_fdblocks(mp, fdblks_delta, 0);
			spin_lock(&mp->m_sb_lock);
		}

		goto out;
	}

	/*
	 * If the request is larger than the current reservation, reserve the
	 * blocks before we update the reserve counters. Sample m_fdblocks and
	 * perform a partial reservation if the request exceeds free space.
	 *
	 * The code below estimates how many blocks it can request from
	 * fdblocks to stash in the reserve pool.  This is a classic TOCTOU
	 * race since fdblocks updates are not always coordinated via
	 * m_sb_lock.  Set the reserve size even if there's not enough free
	 * space to fill it because mod_fdblocks will refill an undersized
	 * reserve when it can.
	 */
	free = percpu_counter_sum(&mp->m_fdblocks) -
						xfs_fdblocks_unavailable(mp);
	delta = request - mp->m_resblks;
	mp->m_resblks = request;
	if (delta > 0 && free > 0) {
		/*
		 * We'll either succeed in getting space from the free block
		 * count or we'll get an ENOSPC.  Don't set the reserved flag
		 * here - we don't want to reserve the extra reserve blocks
		 * from the reserve.
		 *
		 * The desired reserve size can change after we drop the lock.
		 * Use mod_fdblocks to put the space into the reserve or into
		 * fdblocks as appropriate.
		 */
		fdblks_delta = min(free, delta);
		spin_unlock(&mp->m_sb_lock);
		error = xfs_mod_fdblocks(mp, -fdblks_delta, 0);
		if (!error)
			xfs_mod_fdblocks(mp, fdblks_delta, 0);
		spin_lock(&mp->m_sb_lock);
	}
out:
	spin_unlock(&mp->m_sb_lock);
	return error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int
xfs_fs_goingdown(
	xfs_mount_t	*mp,
<<<<<<< HEAD
	__uint32_t	inflags)
{
	switch (inflags) {
	case XFS_FSOP_GOING_FLAGS_DEFAULT: {
		struct super_block *sb = freeze_bdev(mp->m_super->s_bdev);

		if (sb && !IS_ERR(sb)) {
			xfs_force_shutdown(mp, SHUTDOWN_FORCE_UMOUNT);
			thaw_bdev(sb->s_bdev, sb);
		}

=======
	uint32_t	inflags)
{
	switch (inflags) {
	case XFS_FSOP_GOING_FLAGS_DEFAULT: {
		if (!bdev_freeze(mp->m_super->s_bdev)) {
			xfs_force_shutdown(mp, SHUTDOWN_FORCE_UMOUNT);
			bdev_thaw(mp->m_super->s_bdev);
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	}
	case XFS_FSOP_GOING_FLAGS_LOGFLUSH:
		xfs_force_shutdown(mp, SHUTDOWN_FORCE_UMOUNT);
		break;
	case XFS_FSOP_GOING_FLAGS_NOLOGFLUSH:
		xfs_force_shutdown(mp,
				SHUTDOWN_FORCE_UMOUNT | SHUTDOWN_LOG_IO_ERROR);
		break;
	default:
<<<<<<< HEAD
		return XFS_ERROR(EINVAL);
=======
		return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return 0;
}
<<<<<<< HEAD
=======

/*
 * Force a shutdown of the filesystem instantly while keeping the filesystem
 * consistent. We don't do an unmount here; just shutdown the shop, make sure
 * that absolutely nothing persistent happens to this filesystem after this
 * point.
 *
 * The shutdown state change is atomic, resulting in the first and only the
 * first shutdown call processing the shutdown. This means we only shutdown the
 * log once as it requires, and we don't spam the logs when multiple concurrent
 * shutdowns race to set the shutdown flags.
 */
void
xfs_do_force_shutdown(
	struct xfs_mount *mp,
	uint32_t	flags,
	char		*fname,
	int		lnnum)
{
	int		tag;
	const char	*why;


	if (test_and_set_bit(XFS_OPSTATE_SHUTDOWN, &mp->m_opstate)) {
		xlog_shutdown_wait(mp->m_log);
		return;
	}
	if (mp->m_sb_bp)
		mp->m_sb_bp->b_flags |= XBF_DONE;

	if (flags & SHUTDOWN_FORCE_UMOUNT)
		xfs_alert(mp, "User initiated shutdown received.");

	if (xlog_force_shutdown(mp->m_log, flags)) {
		tag = XFS_PTAG_SHUTDOWN_LOGERROR;
		why = "Log I/O Error";
	} else if (flags & SHUTDOWN_CORRUPT_INCORE) {
		tag = XFS_PTAG_SHUTDOWN_CORRUPT;
		why = "Corruption of in-memory data";
	} else if (flags & SHUTDOWN_CORRUPT_ONDISK) {
		tag = XFS_PTAG_SHUTDOWN_CORRUPT;
		why = "Corruption of on-disk metadata";
	} else if (flags & SHUTDOWN_DEVICE_REMOVED) {
		tag = XFS_PTAG_SHUTDOWN_IOERROR;
		why = "Block device removal";
	} else {
		tag = XFS_PTAG_SHUTDOWN_IOERROR;
		why = "Metadata I/O Error";
	}

	trace_xfs_force_shutdown(mp, tag, flags, fname, lnnum);

	xfs_alert_tag(mp, tag,
"%s (0x%x) detected at %pS (%s:%d).  Shutting down filesystem.",
			why, flags, __return_address, fname, lnnum);
	xfs_alert(mp,
		"Please unmount the filesystem and rectify the problem(s)");
	if (xfs_error_level >= XFS_ERRLEVEL_HIGH)
		xfs_stack_trace();
}

/*
 * Reserve free space for per-AG metadata.
 */
int
xfs_fs_reserve_ag_blocks(
	struct xfs_mount	*mp)
{
	xfs_agnumber_t		agno;
	struct xfs_perag	*pag;
	int			error = 0;
	int			err2;

	mp->m_finobt_nores = false;
	for_each_perag(mp, agno, pag) {
		err2 = xfs_ag_resv_init(pag, NULL);
		if (err2 && !error)
			error = err2;
	}

	if (error && error != -ENOSPC) {
		xfs_warn(mp,
	"Error %d reserving per-AG metadata reserve pool.", error);
		xfs_force_shutdown(mp, SHUTDOWN_CORRUPT_INCORE);
	}

	return error;
}

/*
 * Free space reserved for per-AG metadata.
 */
int
xfs_fs_unreserve_ag_blocks(
	struct xfs_mount	*mp)
{
	xfs_agnumber_t		agno;
	struct xfs_perag	*pag;
	int			error = 0;
	int			err2;

	for_each_perag(mp, agno, pag) {
		err2 = xfs_ag_resv_free(pag);
		if (err2 && !error)
			error = err2;
	}

	if (error)
		xfs_warn(mp,
	"Error %d freeing per-AG metadata reserve pool.", error);

	return error;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
