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
#include "xfs_log.h"
#include "xfs_inum.h"
#include "xfs_trans.h"
#include "xfs_sb.h"
#include "xfs_ag.h"
#include "xfs_dir2.h"
#include "xfs_mount.h"
#include "xfs_bmap_btree.h"
#include "xfs_dinode.h"
#include "xfs_inode.h"
#include "xfs_alloc.h"
#include "xfs_bmap.h"
#include "xfs_rtalloc.h"
#include "xfs_fsops.h"
#include "xfs_error.h"
#include "xfs_rw.h"
#include "xfs_inode_item.h"
#include "xfs_trans_space.h"
#include "xfs_utils.h"
#include "xfs_trace.h"
#include "xfs_buf.h"


/*
 * Prototypes for internal functions.
 */


STATIC int xfs_rtallocate_range(xfs_mount_t *, xfs_trans_t *, xfs_rtblock_t,
		xfs_extlen_t, xfs_buf_t **, xfs_fsblock_t *);
STATIC int xfs_rtany_summary(xfs_mount_t *, xfs_trans_t *, int, int,
		xfs_rtblock_t, xfs_buf_t **, xfs_fsblock_t *, int *);
STATIC int xfs_rtcheck_range(xfs_mount_t *, xfs_trans_t *, xfs_rtblock_t,
		xfs_extlen_t, int, xfs_rtblock_t *, int *);
STATIC int xfs_rtfind_back(xfs_mount_t *, xfs_trans_t *, xfs_rtblock_t,
		xfs_rtblock_t, xfs_rtblock_t *);
STATIC int xfs_rtfind_forw(xfs_mount_t *, xfs_trans_t *, xfs_rtblock_t,
		xfs_rtblock_t, xfs_rtblock_t *);
STATIC int xfs_rtget_summary( xfs_mount_t *, xfs_trans_t *, int,
		xfs_rtblock_t, xfs_buf_t **, xfs_fsblock_t *, xfs_suminfo_t *);
STATIC int xfs_rtmodify_range(xfs_mount_t *, xfs_trans_t *, xfs_rtblock_t,
		xfs_extlen_t, int);
STATIC int xfs_rtmodify_summary(xfs_mount_t *, xfs_trans_t *, int,
		xfs_rtblock_t, int, xfs_buf_t **, xfs_fsblock_t *);

/*
 * Internal functions.
 */

/*
 * Allocate space to the bitmap or summary file, and zero it, for growfs.
 */
STATIC int				/* error */
xfs_growfs_rt_alloc(
	xfs_mount_t	*mp,		/* file system mount point */
	xfs_extlen_t	oblocks,	/* old count of blocks */
	xfs_extlen_t	nblocks,	/* new count of blocks */
	xfs_inode_t	*ip)		/* inode (bitmap/summary) */
{
	xfs_fileoff_t	bno;		/* block number in file */
	xfs_buf_t	*bp;		/* temporary buffer for zeroing */
	int		committed;	/* transaction committed flag */
	xfs_daddr_t	d;		/* disk block address */
	int		error;		/* error return value */
	xfs_fsblock_t	firstblock;	/* first block allocated in xaction */
	xfs_bmap_free_t	flist;		/* list of freed blocks */
	xfs_fsblock_t	fsbno;		/* filesystem block for bno */
	xfs_bmbt_irec_t	map;		/* block map output */
	int		nmap;		/* number of block maps */
	int		resblks;	/* space reservation */

	/*
	 * Allocate space to the file, as necessary.
	 */
	while (oblocks < nblocks) {
		int		cancelflags = 0;
		xfs_trans_t	*tp;

		tp = xfs_trans_alloc(mp, XFS_TRANS_GROWFSRT_ALLOC);
		resblks = XFS_GROWFSRT_SPACE_RES(mp, nblocks - oblocks);
		/*
		 * Reserve space & log for one extent added to the file.
		 */
		if ((error = xfs_trans_reserve(tp, resblks,
				XFS_GROWRTALLOC_LOG_RES(mp), 0,
				XFS_TRANS_PERM_LOG_RES,
				XFS_DEFAULT_PERM_LOG_COUNT)))
			goto error_cancel;
		cancelflags = XFS_TRANS_RELEASE_LOG_RES;
		/*
		 * Lock the inode.
		 */
		xfs_ilock(ip, XFS_ILOCK_EXCL);
		xfs_trans_ijoin(tp, ip, XFS_ILOCK_EXCL);

		xfs_bmap_init(&flist, &firstblock);
		/*
		 * Allocate blocks to the bitmap file.
		 */
		nmap = 1;
		cancelflags |= XFS_TRANS_ABORT;
		error = xfs_bmapi_write(tp, ip, oblocks, nblocks - oblocks,
					XFS_BMAPI_METADATA, &firstblock,
					resblks, &map, &nmap, &flist);
		if (!error && nmap < 1)
			error = XFS_ERROR(ENOSPC);
		if (error)
			goto error_cancel;
		/*
		 * Free any blocks freed up in the transaction, then commit.
		 */
		error = xfs_bmap_finish(&tp, &flist, &committed);
		if (error)
			goto error_cancel;
		error = xfs_trans_commit(tp, XFS_TRANS_RELEASE_LOG_RES);
		if (error)
			goto error;
		/*
		 * Now we need to clear the allocated blocks.
		 * Do this one block per transaction, to keep it simple.
		 */
		cancelflags = 0;
		for (bno = map.br_startoff, fsbno = map.br_startblock;
		     bno < map.br_startoff + map.br_blockcount;
		     bno++, fsbno++) {
			tp = xfs_trans_alloc(mp, XFS_TRANS_GROWFSRT_ZERO);
			/*
			 * Reserve log for one block zeroing.
			 */
			if ((error = xfs_trans_reserve(tp, 0,
					XFS_GROWRTZERO_LOG_RES(mp), 0, 0, 0)))
				goto error_cancel;
			/*
			 * Lock the bitmap inode.
			 */
			xfs_ilock(ip, XFS_ILOCK_EXCL);
			xfs_trans_ijoin(tp, ip, XFS_ILOCK_EXCL);
			/*
			 * Get a buffer for the block.
			 */
			d = XFS_FSB_TO_DADDR(mp, fsbno);
			bp = xfs_trans_get_buf(tp, mp->m_ddev_targp, d,
				mp->m_bsize, 0);
			if (bp == NULL) {
				error = XFS_ERROR(EIO);
error_cancel:
				xfs_trans_cancel(tp, cancelflags);
				goto error;
			}
			memset(bp->b_addr, 0, mp->m_sb.sb_blocksize);
			xfs_trans_log_buf(tp, bp, 0, mp->m_sb.sb_blocksize - 1);
			/*
			 * Commit the transaction.
			 */
			error = xfs_trans_commit(tp, 0);
			if (error)
				goto error;
		}
		/*
		 * Go on to the next extent, if any.
		 */
		oblocks = map.br_startoff + map.br_blockcount;
	}
	return 0;

error:
	return error;
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
#include "xfs_bit.h"
#include "xfs_mount.h"
#include "xfs_inode.h"
#include "xfs_bmap.h"
#include "xfs_bmap_btree.h"
#include "xfs_bmap_util.h"
#include "xfs_trans.h"
#include "xfs_trans_space.h"
#include "xfs_icache.h"
#include "xfs_rtalloc.h"
#include "xfs_sb.h"
#include "xfs_rtbitmap.h"
#include "xfs_quota.h"
#include "xfs_log_priv.h"
#include "xfs_health.h"

/*
 * Return whether there are any free extents in the size range given
 * by low and high, for the bitmap block bbno.
 */
STATIC int
xfs_rtany_summary(
	struct xfs_rtalloc_args	*args,
	int			low,	/* low log2 extent size */
	int			high,	/* high log2 extent size */
	xfs_fileoff_t		bbno,	/* bitmap block number */
	int			*maxlog) /* out: max log2 extent size free */
{
	struct xfs_mount	*mp = args->mp;
	int			error;
	int			log;	/* loop counter, log2 of ext. size */
	xfs_suminfo_t		sum;	/* summary data */

	/* There are no extents at levels >= m_rsum_cache[bbno]. */
	if (mp->m_rsum_cache) {
		high = min(high, mp->m_rsum_cache[bbno] - 1);
		if (low > high) {
			*maxlog = -1;
			return 0;
		}
	}

	/*
	 * Loop over logs of extent sizes.
	 */
	for (log = high; log >= low; log--) {
		/*
		 * Get one summary datum.
		 */
		error = xfs_rtget_summary(args, log, bbno, &sum);
		if (error) {
			return error;
		}
		/*
		 * If there are any, return success.
		 */
		if (sum) {
			*maxlog = log;
			goto out;
		}
	}
	/*
	 * Found nothing, return failure.
	 */
	*maxlog = -1;
out:
	/* There were no extents at levels > log. */
	if (mp->m_rsum_cache && log + 1 < mp->m_rsum_cache[bbno])
		mp->m_rsum_cache[bbno] = log + 1;
	return 0;
}


/*
 * Copy and transform the summary file, given the old and new
 * parameters in the mount structures.
 */
STATIC int
xfs_rtcopy_summary(
	struct xfs_rtalloc_args	*oargs,
	struct xfs_rtalloc_args	*nargs)
{
	xfs_fileoff_t		bbno;	/* bitmap block number */
	int			error;
	int			log;	/* summary level number (log length) */
	xfs_suminfo_t		sum;	/* summary data */

	for (log = oargs->mp->m_rsumlevels - 1; log >= 0; log--) {
		for (bbno = oargs->mp->m_sb.sb_rbmblocks - 1;
		     (xfs_srtblock_t)bbno >= 0;
		     bbno--) {
			error = xfs_rtget_summary(oargs, log, bbno, &sum);
			if (error)
				goto out;
			if (sum == 0)
				continue;
			error = xfs_rtmodify_summary(oargs, log, bbno, -sum);
			if (error)
				goto out;
			error = xfs_rtmodify_summary(nargs, log, bbno, sum);
			if (error)
				goto out;
			ASSERT(sum > 0);
		}
	}
	error = 0;
out:
	xfs_rtbuf_cache_relse(oargs);
	return 0;
}
/*
 * Mark an extent specified by start and len allocated.
 * Updates all the summary information as well as the bitmap.
 */
STATIC int
xfs_rtallocate_range(
	struct xfs_rtalloc_args	*args,
	xfs_rtxnum_t		start,	/* start rtext to allocate */
	xfs_rtxlen_t		len)	/* in/out: summary block number */
{
	struct xfs_mount	*mp = args->mp;
	xfs_rtxnum_t		end;	/* end of the allocated rtext */
	int			error;
	xfs_rtxnum_t		postblock = 0; /* first rtext allocated > end */
	xfs_rtxnum_t		preblock = 0; /* first rtext allocated < start */

	end = start + len - 1;
	/*
	 * Assume we're allocating out of the middle of a free extent.
	 * We need to find the beginning and end of the extent so we can
	 * properly update the summary.
	 */
	error = xfs_rtfind_back(args, start, 0, &preblock);
	if (error)
		return error;

	/*
	 * Find the next allocated block (end of free extent).
	 */
	error = xfs_rtfind_forw(args, end, mp->m_sb.sb_rextents - 1,
			&postblock);
	if (error)
		return error;

	/*
	 * Decrement the summary information corresponding to the entire
	 * (old) free extent.
	 */
	error = xfs_rtmodify_summary(args,
			xfs_highbit64(postblock + 1 - preblock),
			xfs_rtx_to_rbmblock(mp, preblock), -1);
	if (error)
		return error;

	/*
	 * If there are blocks not being allocated at the front of the
	 * old extent, add summary data for them to be free.
	 */
	if (preblock < start) {
		error = xfs_rtmodify_summary(args,
				xfs_highbit64(start - preblock),
				xfs_rtx_to_rbmblock(mp, preblock), 1);
		if (error)
			return error;
	}

	/*
	 * If there are blocks not being allocated at the end of the
	 * old extent, add summary data for them to be free.
	 */
	if (postblock > end) {
		error = xfs_rtmodify_summary(args,
				xfs_highbit64(postblock - end),
				xfs_rtx_to_rbmblock(mp, end + 1), 1);
		if (error)
			return error;
	}

	/*
	 * Modify the bitmap to mark this extent allocated.
	 */
	return xfs_rtmodify_range(args, start, len, 0);
}

/*
 * Make sure we don't run off the end of the rt volume.  Be careful that
 * adjusting maxlen downwards doesn't cause us to fail the alignment checks.
 */
static inline xfs_rtxlen_t
xfs_rtallocate_clamp_len(
	struct xfs_mount	*mp,
	xfs_rtxnum_t		startrtx,
	xfs_rtxlen_t		rtxlen,
	xfs_rtxlen_t		prod)
{
	xfs_rtxlen_t		ret;

	ret = min(mp->m_sb.sb_rextents, startrtx + rtxlen) - startrtx;
	return rounddown(ret, prod);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Attempt to allocate an extent minlen<=len<=maxlen starting from
 * bitmap block bbno.  If we don't get maxlen then use prod to trim
<<<<<<< HEAD
 * the length, if given.  Returns error; returns starting block in *rtblock.
 * The lengths are all in rtextents.
 */
STATIC int				/* error */
xfs_rtallocate_extent_block(
	xfs_mount_t	*mp,		/* file system mount point */
	xfs_trans_t	*tp,		/* transaction pointer */
	xfs_rtblock_t	bbno,		/* bitmap block number */
	xfs_extlen_t	minlen,		/* minimum length to allocate */
	xfs_extlen_t	maxlen,		/* maximum length to allocate */
	xfs_extlen_t	*len,		/* out: actual length allocated */
	xfs_rtblock_t	*nextp,		/* out: next block to try */
	xfs_buf_t	**rbpp,		/* in/out: summary block buffer */
	xfs_fsblock_t	*rsb,		/* in/out: summary block number */
	xfs_extlen_t	prod,		/* extent product factor */
	xfs_rtblock_t	*rtblock)	/* out: start block allocated */
{
	xfs_rtblock_t	besti;		/* best rtblock found so far */
	xfs_rtblock_t	bestlen;	/* best length found so far */
	xfs_rtblock_t	end;		/* last rtblock in chunk */
	int		error;		/* error value */
	xfs_rtblock_t	i;		/* current rtblock trying */
	xfs_rtblock_t	next;		/* next rtblock to try */
	int		stat;		/* status from internal calls */
=======
 * the length, if given.  Returns error; returns starting block in *rtx.
 * The lengths are all in rtextents.
 */
STATIC int
xfs_rtallocate_extent_block(
	struct xfs_rtalloc_args	*args,
	xfs_fileoff_t		bbno,	/* bitmap block number */
	xfs_rtxlen_t		minlen,	/* minimum length to allocate */
	xfs_rtxlen_t		maxlen,	/* maximum length to allocate */
	xfs_rtxlen_t		*len,	/* out: actual length allocated */
	xfs_rtxnum_t		*nextp,	/* out: next rtext to try */
	xfs_rtxlen_t		prod,	/* extent product factor */
	xfs_rtxnum_t		*rtx)	/* out: start rtext allocated */
{
	struct xfs_mount	*mp = args->mp;
	xfs_rtxnum_t		besti;	/* best rtext found so far */
	xfs_rtxnum_t		bestlen;/* best length found so far */
	xfs_rtxnum_t		end;	/* last rtext in chunk */
	int			error;
	xfs_rtxnum_t		i;	/* current rtext trying */
	xfs_rtxnum_t		next;	/* next rtext to try */
	int			stat;	/* status from internal calls */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Loop over all the extents starting in this bitmap block,
	 * looking for one that's long enough.
	 */
<<<<<<< HEAD
	for (i = XFS_BLOCKTOBIT(mp, bbno), besti = -1, bestlen = 0,
		end = XFS_BLOCKTOBIT(mp, bbno + 1) - 1;
	     i <= end;
	     i++) {
=======
	for (i = xfs_rbmblock_to_rtx(mp, bbno), besti = -1, bestlen = 0,
		end = xfs_rbmblock_to_rtx(mp, bbno + 1) - 1;
	     i <= end;
	     i++) {
		/* Make sure we don't scan off the end of the rt volume. */
		maxlen = xfs_rtallocate_clamp_len(mp, i, maxlen, prod);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * See if there's a free extent of maxlen starting at i.
		 * If it's not so then next will contain the first non-free.
		 */
<<<<<<< HEAD
		error = xfs_rtcheck_range(mp, tp, i, maxlen, 1, &next, &stat);
		if (error) {
			return error;
		}
=======
		error = xfs_rtcheck_range(args, i, maxlen, 1, &next, &stat);
		if (error)
			return error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (stat) {
			/*
			 * i for maxlen is all free, allocate and return that.
			 */
<<<<<<< HEAD
			error = xfs_rtallocate_range(mp, tp, i, maxlen, rbpp,
				rsb);
			if (error) {
				return error;
			}
			*len = maxlen;
			*rtblock = i;
			return 0;
		}
=======
			bestlen = maxlen;
			besti = i;
			goto allocate;
		}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * In the case where we have a variable-sized allocation
		 * request, figure out how big this free piece is,
		 * and if it's big enough for the minimum, and the best
		 * so far, remember it.
		 */
		if (minlen < maxlen) {
<<<<<<< HEAD
			xfs_rtblock_t	thislen;	/* this extent size */
=======
			xfs_rtxnum_t	thislen;	/* this extent size */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

			thislen = next - i;
			if (thislen >= minlen && thislen > bestlen) {
				besti = i;
				bestlen = thislen;
			}
		}
		/*
		 * If not done yet, find the start of the next free space.
		 */
<<<<<<< HEAD
		if (next < end) {
			error = xfs_rtfind_forw(mp, tp, next, end, &i);
			if (error) {
				return error;
			}
		} else
			break;
	}
	/*
	 * Searched the whole thing & didn't find a maxlen free extent.
	 */
	if (minlen < maxlen && besti != -1) {
		xfs_extlen_t	p;	/* amount to trim length by */

		/*
		 * If size should be a multiple of prod, make that so.
		 */
		if (prod > 1 && (p = do_mod(bestlen, prod)))
			bestlen -= p;
		/*
		 * Allocate besti for bestlen & return that.
		 */
		error = xfs_rtallocate_range(mp, tp, besti, bestlen, rbpp, rsb);
		if (error) {
			return error;
		}
		*len = bestlen;
		*rtblock = besti;
		return 0;
	}
	/*
	 * Allocation failed.  Set *nextp to the next block to try.
	 */
	*nextp = next;
	*rtblock = NULLRTBLOCK;
=======
		if (next >= end)
			break;
		error = xfs_rtfind_forw(args, next, end, &i);
		if (error)
			return error;
	}

	/*
	 * Searched the whole thing & didn't find a maxlen free extent.
	 */
	if (minlen > maxlen || besti == -1) {
		/*
		 * Allocation failed.  Set *nextp to the next block to try.
		 */
		*nextp = next;
		return -ENOSPC;
	}

	/*
	 * If size should be a multiple of prod, make that so.
	 */
	if (prod > 1) {
		xfs_rtxlen_t	p;	/* amount to trim length by */

		div_u64_rem(bestlen, prod, &p);
		if (p)
			bestlen -= p;
	}

	/*
	 * Allocate besti for bestlen & return that.
	 */
allocate:
	error = xfs_rtallocate_range(args, besti, bestlen);
	if (error)
		return error;
	*len = bestlen;
	*rtx = besti;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

/*
 * Allocate an extent of length minlen<=len<=maxlen, starting at block
 * bno.  If we don't get maxlen then use prod to trim the length, if given.
<<<<<<< HEAD
 * Returns error; returns starting block in *rtblock.
 * The lengths are all in rtextents.
 */
STATIC int				/* error */
xfs_rtallocate_extent_exact(
	xfs_mount_t	*mp,		/* file system mount point */
	xfs_trans_t	*tp,		/* transaction pointer */
	xfs_rtblock_t	bno,		/* starting block number to allocate */
	xfs_extlen_t	minlen,		/* minimum length to allocate */
	xfs_extlen_t	maxlen,		/* maximum length to allocate */
	xfs_extlen_t	*len,		/* out: actual length allocated */
	xfs_buf_t	**rbpp,		/* in/out: summary block buffer */
	xfs_fsblock_t	*rsb,		/* in/out: summary block number */
	xfs_extlen_t	prod,		/* extent product factor */
	xfs_rtblock_t	*rtblock)	/* out: start block allocated */
{
	int		error;		/* error value */
	xfs_extlen_t	i;		/* extent length trimmed due to prod */
	int		isfree;		/* extent is free */
	xfs_rtblock_t	next;		/* next block to try (dummy) */

	ASSERT(minlen % prod == 0 && maxlen % prod == 0);
	/*
	 * Check if the range in question (for maxlen) is free.
	 */
	error = xfs_rtcheck_range(mp, tp, bno, maxlen, 1, &next, &isfree);
	if (error) {
		return error;
	}
	if (isfree) {
		/*
		 * If it is, allocate it and return success.
		 */
		error = xfs_rtallocate_range(mp, tp, bno, maxlen, rbpp, rsb);
		if (error) {
			return error;
		}
		*len = maxlen;
		*rtblock = bno;
		return 0;
	}
	/*
	 * If not, allocate what there is, if it's at least minlen.
	 */
	maxlen = next - bno;
	if (maxlen < minlen) {
		/*
		 * Failed, return failure status.
		 */
		*rtblock = NULLRTBLOCK;
		return 0;
	}
	/*
	 * Trim off tail of extent, if prod is specified.
	 */
	if (prod > 1 && (i = maxlen % prod)) {
		maxlen -= i;
		if (maxlen < minlen) {
			/*
			 * Now we can't do it, return failure status.
			 */
			*rtblock = NULLRTBLOCK;
			return 0;
		}
	}
	/*
	 * Allocate what we can and return it.
	 */
	error = xfs_rtallocate_range(mp, tp, bno, maxlen, rbpp, rsb);
	if (error) {
		return error;
	}
	*len = maxlen;
	*rtblock = bno;
=======
 * Returns error; returns starting block in *rtx.
 * The lengths are all in rtextents.
 */
STATIC int
xfs_rtallocate_extent_exact(
	struct xfs_rtalloc_args	*args,
	xfs_rtxnum_t		start,	/* starting rtext number to allocate */
	xfs_rtxlen_t		minlen,	/* minimum length to allocate */
	xfs_rtxlen_t		maxlen,	/* maximum length to allocate */
	xfs_rtxlen_t		*len,	/* out: actual length allocated */
	xfs_rtxlen_t		prod,	/* extent product factor */
	xfs_rtxnum_t		*rtx)	/* out: start rtext allocated */
{
	int			error;
	xfs_rtxlen_t		i;	/* extent length trimmed due to prod */
	int			isfree;	/* extent is free */
	xfs_rtxnum_t		next;	/* next rtext to try (dummy) */

	ASSERT(minlen % prod == 0);
	ASSERT(maxlen % prod == 0);
	/*
	 * Check if the range in question (for maxlen) is free.
	 */
	error = xfs_rtcheck_range(args, start, maxlen, 1, &next, &isfree);
	if (error)
		return error;

	if (!isfree) {
		/*
		 * If not, allocate what there is, if it's at least minlen.
		 */
		maxlen = next - start;
		if (maxlen < minlen)
			return -ENOSPC;

		/*
		 * Trim off tail of extent, if prod is specified.
		 */
		if (prod > 1 && (i = maxlen % prod)) {
			maxlen -= i;
			if (maxlen < minlen)
				return -ENOSPC;
		}
	}

	/*
	 * Allocate what we can and return it.
	 */
	error = xfs_rtallocate_range(args, start, maxlen);
	if (error)
		return error;
	*len = maxlen;
	*rtx = start;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

/*
 * Allocate an extent of length minlen<=len<=maxlen, starting as near
<<<<<<< HEAD
 * to bno as possible.  If we don't get maxlen then use prod to trim
 * the length, if given.  The lengths are all in rtextents.
 */
STATIC int				/* error */
xfs_rtallocate_extent_near(
	xfs_mount_t	*mp,		/* file system mount point */
	xfs_trans_t	*tp,		/* transaction pointer */
	xfs_rtblock_t	bno,		/* starting block number to allocate */
	xfs_extlen_t	minlen,		/* minimum length to allocate */
	xfs_extlen_t	maxlen,		/* maximum length to allocate */
	xfs_extlen_t	*len,		/* out: actual length allocated */
	xfs_buf_t	**rbpp,		/* in/out: summary block buffer */
	xfs_fsblock_t	*rsb,		/* in/out: summary block number */
	xfs_extlen_t	prod,		/* extent product factor */
	xfs_rtblock_t	*rtblock)	/* out: start block allocated */
{
	int		any;		/* any useful extents from summary */
	xfs_rtblock_t	bbno;		/* bitmap block number */
	int		error;		/* error value */
	int		i;		/* bitmap block offset (loop control) */
	int		j;		/* secondary loop control */
	int		log2len;	/* log2 of minlen */
	xfs_rtblock_t	n;		/* next block to try */
	xfs_rtblock_t	r;		/* result block */

	ASSERT(minlen % prod == 0 && maxlen % prod == 0);
=======
 * to start as possible.  If we don't get maxlen then use prod to trim
 * the length, if given.  The lengths are all in rtextents.
 */
STATIC int
xfs_rtallocate_extent_near(
	struct xfs_rtalloc_args	*args,
	xfs_rtxnum_t		start,	/* starting rtext number to allocate */
	xfs_rtxlen_t		minlen,	/* minimum length to allocate */
	xfs_rtxlen_t		maxlen,	/* maximum length to allocate */
	xfs_rtxlen_t		*len,	/* out: actual length allocated */
	xfs_rtxlen_t		prod,	/* extent product factor */
	xfs_rtxnum_t		*rtx)	/* out: start rtext allocated */
{
	struct xfs_mount	*mp = args->mp;
	int			maxlog;	/* max useful extent from summary */
	xfs_fileoff_t		bbno;	/* bitmap block number */
	int			error;
	int			i;	/* bitmap block offset (loop control) */
	int			j;	/* secondary loop control */
	int			log2len; /* log2 of minlen */
	xfs_rtxnum_t		n;	/* next rtext to try */

	ASSERT(minlen % prod == 0);
	ASSERT(maxlen % prod == 0);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * If the block number given is off the end, silently set it to
	 * the last block.
	 */
<<<<<<< HEAD
	if (bno >= mp->m_sb.sb_rextents)
		bno = mp->m_sb.sb_rextents - 1;
	/*
	 * Try the exact allocation first.
	 */
	error = xfs_rtallocate_extent_exact(mp, tp, bno, minlen, maxlen, len,
		rbpp, rsb, prod, &r);
	if (error) {
		return error;
	}
	/*
	 * If the exact allocation worked, return that.
	 */
	if (r != NULLRTBLOCK) {
		*rtblock = r;
		return 0;
	}
	bbno = XFS_BITTOBLOCK(mp, bno);
	i = 0;
=======
	if (start >= mp->m_sb.sb_rextents)
		start = mp->m_sb.sb_rextents - 1;

	/* Make sure we don't run off the end of the rt volume. */
	maxlen = xfs_rtallocate_clamp_len(mp, start, maxlen, prod);
	if (maxlen < minlen)
		return -ENOSPC;

	/*
	 * Try the exact allocation first.
	 */
	error = xfs_rtallocate_extent_exact(args, start, minlen, maxlen, len,
			prod, rtx);
	if (error != -ENOSPC)
		return error;


	bbno = xfs_rtx_to_rbmblock(mp, start);
	i = 0;
	j = -1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ASSERT(minlen != 0);
	log2len = xfs_highbit32(minlen);
	/*
	 * Loop over all bitmap blocks (bbno + i is current block).
	 */
	for (;;) {
		/*
		 * Get summary information of extents of all useful levels
		 * starting in this bitmap block.
		 */
<<<<<<< HEAD
		error = xfs_rtany_summary(mp, tp, log2len, mp->m_rsumlevels - 1,
			bbno + i, rbpp, rsb, &any);
		if (error) {
			return error;
		}
=======
		error = xfs_rtany_summary(args, log2len, mp->m_rsumlevels - 1,
				bbno + i, &maxlog);
		if (error)
			return error;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * If there are any useful extents starting here, try
		 * allocating one.
		 */
<<<<<<< HEAD
		if (any) {
=======
		if (maxlog >= 0) {
			xfs_extlen_t maxavail =
				min_t(xfs_rtblock_t, maxlen,
				      (1ULL << (maxlog + 1)) - 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/*
			 * On the positive side of the starting location.
			 */
			if (i >= 0) {
				/*
				 * Try to allocate an extent starting in
				 * this block.
				 */
<<<<<<< HEAD
				error = xfs_rtallocate_extent_block(mp, tp,
					bbno + i, minlen, maxlen, len, &n, rbpp,
					rsb, prod, &r);
				if (error) {
					return error;
				}
				/*
				 * If it worked, return it.
				 */
				if (r != NULLRTBLOCK) {
					*rtblock = r;
					return 0;
				}
=======
				error = xfs_rtallocate_extent_block(args,
						bbno + i, minlen, maxavail, len,
						&n, prod, rtx);
				if (error != -ENOSPC)
					return error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			}
			/*
			 * On the negative side of the starting location.
			 */
			else {		/* i < 0 */
<<<<<<< HEAD
				/*
				 * Loop backwards through the bitmap blocks from
				 * the starting point-1 up to where we are now.
				 * There should be an extent which ends in this
				 * bitmap block and is long enough.
				 */
				for (j = -1; j > i; j--) {
					/*
					 * Grab the summary information for
					 * this bitmap block.
					 */
					error = xfs_rtany_summary(mp, tp,
						log2len, mp->m_rsumlevels - 1,
						bbno + j, rbpp, rsb, &any);
					if (error) {
						return error;
					}
					/*
					 * If there's no extent given in the
					 * summary that means the extent we
					 * found must carry over from an
					 * earlier block.  If there is an
					 * extent given, we've already tried
					 * that allocation, don't do it again.
					 */
					if (any)
						continue;
					error = xfs_rtallocate_extent_block(mp,
						tp, bbno + j, minlen, maxlen,
						len, &n, rbpp, rsb, prod, &r);
					if (error) {
						return error;
					}
					/*
					 * If it works, return the extent.
					 */
					if (r != NULLRTBLOCK) {
						*rtblock = r;
						return 0;
					}
				}
				/*
				 * There weren't intervening bitmap blocks
				 * with a long enough extent, or the
				 * allocation didn't work for some reason
				 * (i.e. it's a little * too short).
				 * Try to allocate from the summary block
				 * that we found.
				 */
				error = xfs_rtallocate_extent_block(mp, tp,
					bbno + i, minlen, maxlen, len, &n, rbpp,
					rsb, prod, &r);
				if (error) {
					return error;
				}
				/*
				 * If it works, return the extent.
				 */
				if (r != NULLRTBLOCK) {
					*rtblock = r;
					return 0;
=======
				int	maxblocks;

				/*
				 * Loop backwards to find the end of the extent
				 * we found in the realtime summary.
				 *
				 * maxblocks is the maximum possible number of
				 * bitmap blocks from the start of the extent
				 * to the end of the extent.
				 */
				if (maxlog == 0)
					maxblocks = 0;
				else if (maxlog < mp->m_blkbit_log)
					maxblocks = 1;
				else
					maxblocks = 2 << (maxlog - mp->m_blkbit_log);

				/*
				 * We need to check bbno + i + maxblocks down to
				 * bbno + i. We already checked bbno down to
				 * bbno + j + 1, so we don't need to check those
				 * again.
				 */
				j = min(i + maxblocks, j);
				for (; j >= i; j--) {
					error = xfs_rtallocate_extent_block(args,
							bbno + j, minlen,
							maxavail, len, &n, prod,
							rtx);
					if (error != -ENOSPC)
						return error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				}
			}
		}
		/*
		 * Loop control.  If we were on the positive side, and there's
		 * still more blocks on the negative side, go there.
		 */
		if (i > 0 && (int)bbno - i >= 0)
			i = -i;
		/*
		 * If positive, and no more negative, but there are more
		 * positive, go there.
		 */
		else if (i > 0 && (int)bbno + i < mp->m_sb.sb_rbmblocks - 1)
			i++;
		/*
		 * If negative or 0 (just started), and there are positive
		 * blocks to go, go there.  The 0 case moves to block 1.
		 */
		else if (i <= 0 && (int)bbno - i < mp->m_sb.sb_rbmblocks - 1)
			i = 1 - i;
		/*
		 * If negative or 0 and there are more negative blocks,
		 * go there.
		 */
		else if (i <= 0 && (int)bbno + i > 0)
			i--;
		/*
		 * Must be done.  Return failure.
		 */
		else
			break;
	}
<<<<<<< HEAD
	*rtblock = NULLRTBLOCK;
	return 0;
=======
	return -ENOSPC;
}

static int
xfs_rtalloc_sumlevel(
	struct xfs_rtalloc_args	*args,
	int			l,	/* level number */
	xfs_rtxlen_t		minlen,	/* minimum length to allocate */
	xfs_rtxlen_t		maxlen,	/* maximum length to allocate */
	xfs_rtxlen_t		prod,	/* extent product factor */
	xfs_rtxlen_t		*len,	/* out: actual length allocated */
	xfs_rtxnum_t		*rtx)	/* out: start rtext allocated */
{
	xfs_fileoff_t		i;	/* bitmap block number */

	for (i = 0; i < args->mp->m_sb.sb_rbmblocks; i++) {
		xfs_suminfo_t	sum;	/* summary information for extents */
		xfs_rtxnum_t	n;	/* next rtext to be tried */
		int		error;

		error = xfs_rtget_summary(args, l, i, &sum);
		if (error)
			return error;

		/*
		 * Nothing there, on to the next block.
		 */
		if (!sum)
			continue;

		/*
		 * Try allocating the extent.
		 */
		error = xfs_rtallocate_extent_block(args, i, minlen, maxlen,
				len, &n, prod, rtx);
		if (error != -ENOSPC)
			return error;

		/*
		 * If the "next block to try" returned from the allocator is
		 * beyond the next bitmap block, skip to that bitmap block.
		 */
		if (xfs_rtx_to_rbmblock(args->mp, n) > i + 1)
			i = xfs_rtx_to_rbmblock(args->mp, n) - 1;
	}

	return -ENOSPC;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Allocate an extent of length minlen<=len<=maxlen, with no position
 * specified.  If we don't get maxlen then use prod to trim
 * the length, if given.  The lengths are all in rtextents.
 */
<<<<<<< HEAD
STATIC int				/* error */
xfs_rtallocate_extent_size(
	xfs_mount_t	*mp,		/* file system mount point */
	xfs_trans_t	*tp,		/* transaction pointer */
	xfs_extlen_t	minlen,		/* minimum length to allocate */
	xfs_extlen_t	maxlen,		/* maximum length to allocate */
	xfs_extlen_t	*len,		/* out: actual length allocated */
	xfs_buf_t	**rbpp,		/* in/out: summary block buffer */
	xfs_fsblock_t	*rsb,		/* in/out: summary block number */
	xfs_extlen_t	prod,		/* extent product factor */
	xfs_rtblock_t	*rtblock)	/* out: start block allocated */
{
	int		error;		/* error value */
	int		i;		/* bitmap block number */
	int		l;		/* level number (loop control) */
	xfs_rtblock_t	n;		/* next block to be tried */
	xfs_rtblock_t	r;		/* result block number */
	xfs_suminfo_t	sum;		/* summary information for extents */

	ASSERT(minlen % prod == 0 && maxlen % prod == 0);
=======
STATIC int
xfs_rtallocate_extent_size(
	struct xfs_rtalloc_args	*args,
	xfs_rtxlen_t		minlen,	/* minimum length to allocate */
	xfs_rtxlen_t		maxlen,	/* maximum length to allocate */
	xfs_rtxlen_t		*len,	/* out: actual length allocated */
	xfs_rtxlen_t		prod,	/* extent product factor */
	xfs_rtxnum_t		*rtx)	/* out: start rtext allocated */
{
	int			error;
	int			l;	/* level number (loop control) */

	ASSERT(minlen % prod == 0);
	ASSERT(maxlen % prod == 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ASSERT(maxlen != 0);

	/*
	 * Loop over all the levels starting with maxlen.
<<<<<<< HEAD
	 * At each level, look at all the bitmap blocks, to see if there
	 * are extents starting there that are long enough (>= maxlen).
	 * Note, only on the initial level can the allocation fail if
	 * the summary says there's an extent.
	 */
	for (l = xfs_highbit32(maxlen); l < mp->m_rsumlevels; l++) {
		/*
		 * Loop over all the bitmap blocks.
		 */
		for (i = 0; i < mp->m_sb.sb_rbmblocks; i++) {
			/*
			 * Get the summary for this level/block.
			 */
			error = xfs_rtget_summary(mp, tp, l, i, rbpp, rsb,
				&sum);
			if (error) {
				return error;
			}
			/*
			 * Nothing there, on to the next block.
			 */
			if (!sum)
				continue;
			/*
			 * Try allocating the extent.
			 */
			error = xfs_rtallocate_extent_block(mp, tp, i, maxlen,
				maxlen, len, &n, rbpp, rsb, prod, &r);
			if (error) {
				return error;
			}
			/*
			 * If it worked, return that.
			 */
			if (r != NULLRTBLOCK) {
				*rtblock = r;
				return 0;
			}
			/*
			 * If the "next block to try" returned from the
			 * allocator is beyond the next bitmap block,
			 * skip to that bitmap block.
			 */
			if (XFS_BITTOBLOCK(mp, n) > i + 1)
				i = XFS_BITTOBLOCK(mp, n) - 1;
		}
	}
	/*
	 * Didn't find any maxlen blocks.  Try smaller ones, unless
	 * we're asking for a fixed size extent.
	 */
	if (minlen > --maxlen) {
		*rtblock = NULLRTBLOCK;
		return 0;
	}
=======
	 *
	 * At each level, look at all the bitmap blocks, to see if there are
	 * extents starting there that are long enough (>= maxlen).
	 *
	 * Note, only on the initial level can the allocation fail if the
	 * summary says there's an extent.
	 */
	for (l = xfs_highbit32(maxlen); l < args->mp->m_rsumlevels; l++) {
		error = xfs_rtalloc_sumlevel(args, l, minlen, maxlen, prod, len,
				rtx);
		if (error != -ENOSPC)
			return error;
	}

	/*
	 * Didn't find any maxlen blocks.  Try smaller ones, unless we are
	 * looking for a fixed size extent.
	 */
	if (minlen > --maxlen)
		return -ENOSPC;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ASSERT(minlen != 0);
	ASSERT(maxlen != 0);

	/*
	 * Loop over sizes, from maxlen down to minlen.
<<<<<<< HEAD
	 * This time, when we do the allocations, allow smaller ones
	 * to succeed.
	 */
	for (l = xfs_highbit32(maxlen); l >= xfs_highbit32(minlen); l--) {
		/*
		 * Loop over all the bitmap blocks, try an allocation
		 * starting in that block.
		 */
		for (i = 0; i < mp->m_sb.sb_rbmblocks; i++) {
			/*
			 * Get the summary information for this level/block.
			 */
			error =	xfs_rtget_summary(mp, tp, l, i, rbpp, rsb,
						  &sum);
			if (error) {
				return error;
			}
			/*
			 * If nothing there, go on to next.
			 */
			if (!sum)
				continue;
			/*
			 * Try the allocation.  Make sure the specified
			 * minlen/maxlen are in the possible range for
			 * this summary level.
			 */
			error = xfs_rtallocate_extent_block(mp, tp, i,
					XFS_RTMAX(minlen, 1 << l),
					XFS_RTMIN(maxlen, (1 << (l + 1)) - 1),
					len, &n, rbpp, rsb, prod, &r);
			if (error) {
				return error;
			}
			/*
			 * If it worked, return that extent.
			 */
			if (r != NULLRTBLOCK) {
				*rtblock = r;
				return 0;
			}
			/*
			 * If the "next block to try" returned from the
			 * allocator is beyond the next bitmap block,
			 * skip to that bitmap block.
			 */
			if (XFS_BITTOBLOCK(mp, n) > i + 1)
				i = XFS_BITTOBLOCK(mp, n) - 1;
		}
	}
	/*
	 * Got nothing, return failure.
	 */
	*rtblock = NULLRTBLOCK;
	return 0;
}

/*
 * Mark an extent specified by start and len allocated.
 * Updates all the summary information as well as the bitmap.
 */
STATIC int				/* error */
xfs_rtallocate_range(
	xfs_mount_t	*mp,		/* file system mount point */
	xfs_trans_t	*tp,		/* transaction pointer */
	xfs_rtblock_t	start,		/* start block to allocate */
	xfs_extlen_t	len,		/* length to allocate */
	xfs_buf_t	**rbpp,		/* in/out: summary block buffer */
	xfs_fsblock_t	*rsb)		/* in/out: summary block number */
{
	xfs_rtblock_t	end;		/* end of the allocated extent */
	int		error;		/* error value */
	xfs_rtblock_t	postblock;	/* first block allocated > end */
	xfs_rtblock_t	preblock;	/* first block allocated < start */

	end = start + len - 1;
	/*
	 * Assume we're allocating out of the middle of a free extent.
	 * We need to find the beginning and end of the extent so we can
	 * properly update the summary.
	 */
	error = xfs_rtfind_back(mp, tp, start, 0, &preblock);
	if (error) {
		return error;
	}
	/*
	 * Find the next allocated block (end of free extent).
	 */
	error = xfs_rtfind_forw(mp, tp, end, mp->m_sb.sb_rextents - 1,
		&postblock);
	if (error) {
		return error;
	}
	/*
	 * Decrement the summary information corresponding to the entire
	 * (old) free extent.
	 */
	error = xfs_rtmodify_summary(mp, tp,
		XFS_RTBLOCKLOG(postblock + 1 - preblock),
		XFS_BITTOBLOCK(mp, preblock), -1, rbpp, rsb);
	if (error) {
		return error;
	}
	/*
	 * If there are blocks not being allocated at the front of the
	 * old extent, add summary data for them to be free.
	 */
	if (preblock < start) {
		error = xfs_rtmodify_summary(mp, tp,
			XFS_RTBLOCKLOG(start - preblock),
			XFS_BITTOBLOCK(mp, preblock), 1, rbpp, rsb);
		if (error) {
			return error;
		}
	}
	/*
	 * If there are blocks not being allocated at the end of the
	 * old extent, add summary data for them to be free.
	 */
	if (postblock > end) {
		error = xfs_rtmodify_summary(mp, tp,
			XFS_RTBLOCKLOG(postblock - end),
			XFS_BITTOBLOCK(mp, end + 1), 1, rbpp, rsb);
		if (error) {
			return error;
		}
	}
	/*
	 * Modify the bitmap to mark this extent allocated.
	 */
	error = xfs_rtmodify_range(mp, tp, start, len, 0);
	return error;
}

/*
 * Return whether there are any free extents in the size range given
 * by low and high, for the bitmap block bbno.
 */
STATIC int				/* error */
xfs_rtany_summary(
	xfs_mount_t	*mp,		/* file system mount structure */
	xfs_trans_t	*tp,		/* transaction pointer */
	int		low,		/* low log2 extent size */
	int		high,		/* high log2 extent size */
	xfs_rtblock_t	bbno,		/* bitmap block number */
	xfs_buf_t	**rbpp,		/* in/out: summary block buffer */
	xfs_fsblock_t	*rsb,		/* in/out: summary block number */
	int		*stat)		/* out: any good extents here? */
{
	int		error;		/* error value */
	int		log;		/* loop counter, log2 of ext. size */
	xfs_suminfo_t	sum;		/* summary data */

	/*
	 * Loop over logs of extent sizes.  Order is irrelevant.
	 */
	for (log = low; log <= high; log++) {
		/*
		 * Get one summary datum.
		 */
		error = xfs_rtget_summary(mp, tp, log, bbno, rbpp, rsb, &sum);
		if (error) {
			return error;
		}
		/*
		 * If there are any, return success.
		 */
		if (sum) {
			*stat = 1;
			return 0;
		}
	}
	/*
	 * Found nothing, return failure.
	 */
	*stat = 0;
	return 0;
}

/*
 * Get a buffer for the bitmap or summary file block specified.
 * The buffer is returned read and locked.
 */
STATIC int				/* error */
xfs_rtbuf_get(
	xfs_mount_t	*mp,		/* file system mount structure */
	xfs_trans_t	*tp,		/* transaction pointer */
	xfs_rtblock_t	block,		/* block number in bitmap or summary */
	int		issum,		/* is summary not bitmap */
	xfs_buf_t	**bpp)		/* output: buffer for the block */
{
	xfs_buf_t	*bp;		/* block buffer, result */
	xfs_inode_t	*ip;		/* bitmap or summary inode */
	xfs_bmbt_irec_t	map;
	int		nmap;
	int		error;		/* error value */

	ip = issum ? mp->m_rsumip : mp->m_rbmip;

	error = xfs_bmapi_read(ip, block, 1, &map, &nmap, XFS_DATA_FORK);
	if (error)
		return error;

	ASSERT(map.br_startblock != NULLFSBLOCK);
	error = xfs_trans_read_buf(mp, tp, mp->m_ddev_targp,
				   XFS_FSB_TO_DADDR(mp, map.br_startblock),
				   mp->m_bsize, 0, &bp);
	if (error)
		return error;
	ASSERT(!xfs_buf_geterror(bp));
	*bpp = bp;
	return 0;
}

#ifdef DEBUG
/*
 * Check that the given extent (block range) is allocated already.
 */
STATIC int				/* error */
xfs_rtcheck_alloc_range(
	xfs_mount_t	*mp,		/* file system mount point */
	xfs_trans_t	*tp,		/* transaction pointer */
	xfs_rtblock_t	bno,		/* starting block number of extent */
	xfs_extlen_t	len,		/* length of extent */
	int		*stat)		/* out: 1 for allocated, 0 for not */
{
	xfs_rtblock_t	new;		/* dummy for xfs_rtcheck_range */

	return xfs_rtcheck_range(mp, tp, bno, len, 0, &new, stat);
}
#endif

/*
 * Check that the given range is either all allocated (val = 0) or
 * all free (val = 1).
 */
STATIC int				/* error */
xfs_rtcheck_range(
	xfs_mount_t	*mp,		/* file system mount point */
	xfs_trans_t	*tp,		/* transaction pointer */
	xfs_rtblock_t	start,		/* starting block number of extent */
	xfs_extlen_t	len,		/* length of extent */
	int		val,		/* 1 for free, 0 for allocated */
	xfs_rtblock_t	*new,		/* out: first block not matching */
	int		*stat)		/* out: 1 for matches, 0 for not */
{
	xfs_rtword_t	*b;		/* current word in buffer */
	int		bit;		/* bit number in the word */
	xfs_rtblock_t	block;		/* bitmap block number */
	xfs_buf_t	*bp;		/* buf for the block */
	xfs_rtword_t	*bufp;		/* starting word in buffer */
	int		error;		/* error value */
	xfs_rtblock_t	i;		/* current bit number rel. to start */
	xfs_rtblock_t	lastbit;	/* last useful bit in word */
	xfs_rtword_t	mask;		/* mask of relevant bits for value */
	xfs_rtword_t	wdiff;		/* difference from wanted value */
	int		word;		/* word number in the buffer */

	/*
	 * Compute starting bitmap block number
	 */
	block = XFS_BITTOBLOCK(mp, start);
	/*
	 * Read the bitmap block.
	 */
	error = xfs_rtbuf_get(mp, tp, block, 0, &bp);
	if (error) {
		return error;
	}
	bufp = bp->b_addr;
	/*
	 * Compute the starting word's address, and starting bit.
	 */
	word = XFS_BITTOWORD(mp, start);
	b = &bufp[word];
	bit = (int)(start & (XFS_NBWORD - 1));
	/*
	 * 0 (allocated) => all zero's; 1 (free) => all one's.
	 */
	val = -val;
	/*
	 * If not starting on a word boundary, deal with the first
	 * (partial) word.
	 */
	if (bit) {
		/*
		 * Compute first bit not examined.
		 */
		lastbit = XFS_RTMIN(bit + len, XFS_NBWORD);
		/*
		 * Mask of relevant bits.
		 */
		mask = (((xfs_rtword_t)1 << (lastbit - bit)) - 1) << bit;
		/*
		 * Compute difference between actual and desired value.
		 */
		if ((wdiff = (*b ^ val) & mask)) {
			/*
			 * Different, compute first wrong bit and return.
			 */
			xfs_trans_brelse(tp, bp);
			i = XFS_RTLOBIT(wdiff) - bit;
			*new = start + i;
			*stat = 0;
			return 0;
		}
		i = lastbit - bit;
		/*
		 * Go on to next block if that's where the next word is
		 * and we need the next word.
		 */
		if (++word == XFS_BLOCKWSIZE(mp) && i < len) {
			/*
			 * If done with this block, get the next one.
			 */
			xfs_trans_brelse(tp, bp);
			error = xfs_rtbuf_get(mp, tp, ++block, 0, &bp);
			if (error) {
				return error;
			}
			b = bufp = bp->b_addr;
			word = 0;
		} else {
			/*
			 * Go on to the next word in the buffer.
			 */
			b++;
		}
	} else {
		/*
		 * Starting on a word boundary, no partial word.
		 */
		i = 0;
	}
	/*
	 * Loop over whole words in buffers.  When we use up one buffer
	 * we move on to the next one.
	 */
	while (len - i >= XFS_NBWORD) {
		/*
		 * Compute difference between actual and desired value.
		 */
		if ((wdiff = *b ^ val)) {
			/*
			 * Different, compute first wrong bit and return.
			 */
			xfs_trans_brelse(tp, bp);
			i += XFS_RTLOBIT(wdiff);
			*new = start + i;
			*stat = 0;
			return 0;
		}
		i += XFS_NBWORD;
		/*
		 * Go on to next block if that's where the next word is
		 * and we need the next word.
		 */
		if (++word == XFS_BLOCKWSIZE(mp) && i < len) {
			/*
			 * If done with this block, get the next one.
			 */
			xfs_trans_brelse(tp, bp);
			error = xfs_rtbuf_get(mp, tp, ++block, 0, &bp);
			if (error) {
				return error;
			}
			b = bufp = bp->b_addr;
			word = 0;
		} else {
			/*
			 * Go on to the next word in the buffer.
			 */
			b++;
		}
	}
	/*
	 * If not ending on a word boundary, deal with the last
	 * (partial) word.
	 */
	if ((lastbit = len - i)) {
		/*
		 * Mask of relevant bits.
		 */
		mask = ((xfs_rtword_t)1 << lastbit) - 1;
		/*
		 * Compute difference between actual and desired value.
		 */
		if ((wdiff = (*b ^ val) & mask)) {
			/*
			 * Different, compute first wrong bit and return.
			 */
			xfs_trans_brelse(tp, bp);
			i += XFS_RTLOBIT(wdiff);
			*new = start + i;
			*stat = 0;
			return 0;
		} else
			i = len;
	}
	/*
	 * Successful, return.
	 */
	xfs_trans_brelse(tp, bp);
	*new = start + i;
	*stat = 1;
	return 0;
}

/*
 * Copy and transform the summary file, given the old and new
 * parameters in the mount structures.
 */
STATIC int				/* error */
xfs_rtcopy_summary(
	xfs_mount_t	*omp,		/* old file system mount point */
	xfs_mount_t	*nmp,		/* new file system mount point */
	xfs_trans_t	*tp)		/* transaction pointer */
{
	xfs_rtblock_t	bbno;		/* bitmap block number */
	xfs_buf_t	*bp;		/* summary buffer */
	int		error;		/* error return value */
	int		log;		/* summary level number (log length) */
	xfs_suminfo_t	sum;		/* summary data */
	xfs_fsblock_t	sumbno;		/* summary block number */

	bp = NULL;
	for (log = omp->m_rsumlevels - 1; log >= 0; log--) {
		for (bbno = omp->m_sb.sb_rbmblocks - 1;
		     (xfs_srtblock_t)bbno >= 0;
		     bbno--) {
			error = xfs_rtget_summary(omp, tp, log, bbno, &bp,
				&sumbno, &sum);
			if (error)
				return error;
			if (sum == 0)
				continue;
			error = xfs_rtmodify_summary(omp, tp, log, bbno, -sum,
				&bp, &sumbno);
			if (error)
				return error;
			error = xfs_rtmodify_summary(nmp, tp, log, bbno, sum,
				&bp, &sumbno);
			if (error)
				return error;
			ASSERT(sum > 0);
		}
	}
	return 0;
}

/*
 * Searching backward from start to limit, find the first block whose
 * allocated/free state is different from start's.
 */
STATIC int				/* error */
xfs_rtfind_back(
	xfs_mount_t	*mp,		/* file system mount point */
	xfs_trans_t	*tp,		/* transaction pointer */
	xfs_rtblock_t	start,		/* starting block to look at */
	xfs_rtblock_t	limit,		/* last block to look at */
	xfs_rtblock_t	*rtblock)	/* out: start block found */
{
	xfs_rtword_t	*b;		/* current word in buffer */
	int		bit;		/* bit number in the word */
	xfs_rtblock_t	block;		/* bitmap block number */
	xfs_buf_t	*bp;		/* buf for the block */
	xfs_rtword_t	*bufp;		/* starting word in buffer */
	int		error;		/* error value */
	xfs_rtblock_t	firstbit;	/* first useful bit in the word */
	xfs_rtblock_t	i;		/* current bit number rel. to start */
	xfs_rtblock_t	len;		/* length of inspected area */
	xfs_rtword_t	mask;		/* mask of relevant bits for value */
	xfs_rtword_t	want;		/* mask for "good" values */
	xfs_rtword_t	wdiff;		/* difference from wanted value */
	int		word;		/* word number in the buffer */

	/*
	 * Compute and read in starting bitmap block for starting block.
	 */
	block = XFS_BITTOBLOCK(mp, start);
	error = xfs_rtbuf_get(mp, tp, block, 0, &bp);
	if (error) {
		return error;
	}
	bufp = bp->b_addr;
	/*
	 * Get the first word's index & point to it.
	 */
	word = XFS_BITTOWORD(mp, start);
	b = &bufp[word];
	bit = (int)(start & (XFS_NBWORD - 1));
	len = start - limit + 1;
	/*
	 * Compute match value, based on the bit at start: if 1 (free)
	 * then all-ones, else all-zeroes.
	 */
	want = (*b & ((xfs_rtword_t)1 << bit)) ? -1 : 0;
	/*
	 * If the starting position is not word-aligned, deal with the
	 * partial word.
	 */
	if (bit < XFS_NBWORD - 1) {
		/*
		 * Calculate first (leftmost) bit number to look at,
		 * and mask for all the relevant bits in this word.
		 */
		firstbit = XFS_RTMAX((xfs_srtblock_t)(bit - len + 1), 0);
		mask = (((xfs_rtword_t)1 << (bit - firstbit + 1)) - 1) <<
			firstbit;
		/*
		 * Calculate the difference between the value there
		 * and what we're looking for.
		 */
		if ((wdiff = (*b ^ want) & mask)) {
			/*
			 * Different.  Mark where we are and return.
			 */
			xfs_trans_brelse(tp, bp);
			i = bit - XFS_RTHIBIT(wdiff);
			*rtblock = start - i + 1;
			return 0;
		}
		i = bit - firstbit + 1;
		/*
		 * Go on to previous block if that's where the previous word is
		 * and we need the previous word.
		 */
		if (--word == -1 && i < len) {
			/*
			 * If done with this block, get the previous one.
			 */
			xfs_trans_brelse(tp, bp);
			error = xfs_rtbuf_get(mp, tp, --block, 0, &bp);
			if (error) {
				return error;
			}
			bufp = bp->b_addr;
			word = XFS_BLOCKWMASK(mp);
			b = &bufp[word];
		} else {
			/*
			 * Go on to the previous word in the buffer.
			 */
			b--;
		}
	} else {
		/*
		 * Starting on a word boundary, no partial word.
		 */
		i = 0;
	}
	/*
	 * Loop over whole words in buffers.  When we use up one buffer
	 * we move on to the previous one.
	 */
	while (len - i >= XFS_NBWORD) {
		/*
		 * Compute difference between actual and desired value.
		 */
		if ((wdiff = *b ^ want)) {
			/*
			 * Different, mark where we are and return.
			 */
			xfs_trans_brelse(tp, bp);
			i += XFS_NBWORD - 1 - XFS_RTHIBIT(wdiff);
			*rtblock = start - i + 1;
			return 0;
		}
		i += XFS_NBWORD;
		/*
		 * Go on to previous block if that's where the previous word is
		 * and we need the previous word.
		 */
		if (--word == -1 && i < len) {
			/*
			 * If done with this block, get the previous one.
			 */
			xfs_trans_brelse(tp, bp);
			error = xfs_rtbuf_get(mp, tp, --block, 0, &bp);
			if (error) {
				return error;
			}
			bufp = bp->b_addr;
			word = XFS_BLOCKWMASK(mp);
			b = &bufp[word];
		} else {
			/*
			 * Go on to the previous word in the buffer.
			 */
			b--;
		}
	}
	/*
	 * If not ending on a word boundary, deal with the last
	 * (partial) word.
	 */
	if (len - i) {
		/*
		 * Calculate first (leftmost) bit number to look at,
		 * and mask for all the relevant bits in this word.
		 */
		firstbit = XFS_NBWORD - (len - i);
		mask = (((xfs_rtword_t)1 << (len - i)) - 1) << firstbit;
		/*
		 * Compute difference between actual and desired value.
		 */
		if ((wdiff = (*b ^ want) & mask)) {
			/*
			 * Different, mark where we are and return.
			 */
			xfs_trans_brelse(tp, bp);
			i += XFS_NBWORD - 1 - XFS_RTHIBIT(wdiff);
			*rtblock = start - i + 1;
			return 0;
		} else
			i = len;
	}
	/*
	 * No match, return that we scanned the whole area.
	 */
	xfs_trans_brelse(tp, bp);
	*rtblock = start - i + 1;
	return 0;
}

/*
 * Searching forward from start to limit, find the first block whose
 * allocated/free state is different from start's.
 */
STATIC int				/* error */
xfs_rtfind_forw(
	xfs_mount_t	*mp,		/* file system mount point */
	xfs_trans_t	*tp,		/* transaction pointer */
	xfs_rtblock_t	start,		/* starting block to look at */
	xfs_rtblock_t	limit,		/* last block to look at */
	xfs_rtblock_t	*rtblock)	/* out: start block found */
{
	xfs_rtword_t	*b;		/* current word in buffer */
	int		bit;		/* bit number in the word */
	xfs_rtblock_t	block;		/* bitmap block number */
	xfs_buf_t	*bp;		/* buf for the block */
	xfs_rtword_t	*bufp;		/* starting word in buffer */
	int		error;		/* error value */
	xfs_rtblock_t	i;		/* current bit number rel. to start */
	xfs_rtblock_t	lastbit;	/* last useful bit in the word */
	xfs_rtblock_t	len;		/* length of inspected area */
	xfs_rtword_t	mask;		/* mask of relevant bits for value */
	xfs_rtword_t	want;		/* mask for "good" values */
	xfs_rtword_t	wdiff;		/* difference from wanted value */
	int		word;		/* word number in the buffer */

	/*
	 * Compute and read in starting bitmap block for starting block.
	 */
	block = XFS_BITTOBLOCK(mp, start);
	error = xfs_rtbuf_get(mp, tp, block, 0, &bp);
	if (error) {
		return error;
	}
	bufp = bp->b_addr;
	/*
	 * Get the first word's index & point to it.
	 */
	word = XFS_BITTOWORD(mp, start);
	b = &bufp[word];
	bit = (int)(start & (XFS_NBWORD - 1));
	len = limit - start + 1;
	/*
	 * Compute match value, based on the bit at start: if 1 (free)
	 * then all-ones, else all-zeroes.
	 */
	want = (*b & ((xfs_rtword_t)1 << bit)) ? -1 : 0;
	/*
	 * If the starting position is not word-aligned, deal with the
	 * partial word.
	 */
	if (bit) {
		/*
		 * Calculate last (rightmost) bit number to look at,
		 * and mask for all the relevant bits in this word.
		 */
		lastbit = XFS_RTMIN(bit + len, XFS_NBWORD);
		mask = (((xfs_rtword_t)1 << (lastbit - bit)) - 1) << bit;
		/*
		 * Calculate the difference between the value there
		 * and what we're looking for.
		 */
		if ((wdiff = (*b ^ want) & mask)) {
			/*
			 * Different.  Mark where we are and return.
			 */
			xfs_trans_brelse(tp, bp);
			i = XFS_RTLOBIT(wdiff) - bit;
			*rtblock = start + i - 1;
			return 0;
		}
		i = lastbit - bit;
		/*
		 * Go on to next block if that's where the next word is
		 * and we need the next word.
		 */
		if (++word == XFS_BLOCKWSIZE(mp) && i < len) {
			/*
			 * If done with this block, get the previous one.
			 */
			xfs_trans_brelse(tp, bp);
			error = xfs_rtbuf_get(mp, tp, ++block, 0, &bp);
			if (error) {
				return error;
			}
			b = bufp = bp->b_addr;
			word = 0;
		} else {
			/*
			 * Go on to the previous word in the buffer.
			 */
			b++;
		}
	} else {
		/*
		 * Starting on a word boundary, no partial word.
		 */
		i = 0;
	}
	/*
	 * Loop over whole words in buffers.  When we use up one buffer
	 * we move on to the next one.
	 */
	while (len - i >= XFS_NBWORD) {
		/*
		 * Compute difference between actual and desired value.
		 */
		if ((wdiff = *b ^ want)) {
			/*
			 * Different, mark where we are and return.
			 */
			xfs_trans_brelse(tp, bp);
			i += XFS_RTLOBIT(wdiff);
			*rtblock = start + i - 1;
			return 0;
		}
		i += XFS_NBWORD;
		/*
		 * Go on to next block if that's where the next word is
		 * and we need the next word.
		 */
		if (++word == XFS_BLOCKWSIZE(mp) && i < len) {
			/*
			 * If done with this block, get the next one.
			 */
			xfs_trans_brelse(tp, bp);
			error = xfs_rtbuf_get(mp, tp, ++block, 0, &bp);
			if (error) {
				return error;
			}
			b = bufp = bp->b_addr;
			word = 0;
		} else {
			/*
			 * Go on to the next word in the buffer.
			 */
			b++;
		}
	}
	/*
	 * If not ending on a word boundary, deal with the last
	 * (partial) word.
	 */
	if ((lastbit = len - i)) {
		/*
		 * Calculate mask for all the relevant bits in this word.
		 */
		mask = ((xfs_rtword_t)1 << lastbit) - 1;
		/*
		 * Compute difference between actual and desired value.
		 */
		if ((wdiff = (*b ^ want) & mask)) {
			/*
			 * Different, mark where we are and return.
			 */
			xfs_trans_brelse(tp, bp);
			i += XFS_RTLOBIT(wdiff);
			*rtblock = start + i - 1;
			return 0;
		} else
			i = len;
	}
	/*
	 * No match, return that we scanned the whole area.
	 */
	xfs_trans_brelse(tp, bp);
	*rtblock = start + i - 1;
	return 0;
}

/*
 * Mark an extent specified by start and len freed.
 * Updates all the summary information as well as the bitmap.
 */
STATIC int				/* error */
xfs_rtfree_range(
	xfs_mount_t	*mp,		/* file system mount point */
	xfs_trans_t	*tp,		/* transaction pointer */
	xfs_rtblock_t	start,		/* starting block to free */
	xfs_extlen_t	len,		/* length to free */
	xfs_buf_t	**rbpp,		/* in/out: summary block buffer */
	xfs_fsblock_t	*rsb)		/* in/out: summary block number */
{
	xfs_rtblock_t	end;		/* end of the freed extent */
	int		error;		/* error value */
	xfs_rtblock_t	postblock;	/* first block freed > end */
	xfs_rtblock_t	preblock;	/* first block freed < start */

	end = start + len - 1;
	/*
	 * Modify the bitmap to mark this extent freed.
	 */
	error = xfs_rtmodify_range(mp, tp, start, len, 1);
	if (error) {
		return error;
	}
	/*
	 * Assume we're freeing out of the middle of an allocated extent.
	 * We need to find the beginning and end of the extent so we can
	 * properly update the summary.
	 */
	error = xfs_rtfind_back(mp, tp, start, 0, &preblock);
	if (error) {
		return error;
	}
	/*
	 * Find the next allocated block (end of allocated extent).
	 */
	error = xfs_rtfind_forw(mp, tp, end, mp->m_sb.sb_rextents - 1,
		&postblock);
	if (error)
		return error;
	/*
	 * If there are blocks not being freed at the front of the
	 * old extent, add summary data for them to be allocated.
	 */
	if (preblock < start) {
		error = xfs_rtmodify_summary(mp, tp,
			XFS_RTBLOCKLOG(start - preblock),
			XFS_BITTOBLOCK(mp, preblock), -1, rbpp, rsb);
		if (error) {
			return error;
		}
	}
	/*
	 * If there are blocks not being freed at the end of the
	 * old extent, add summary data for them to be allocated.
	 */
	if (postblock > end) {
		error = xfs_rtmodify_summary(mp, tp,
			XFS_RTBLOCKLOG(postblock - end),
			XFS_BITTOBLOCK(mp, end + 1), -1, rbpp, rsb);
		if (error) {
			return error;
		}
	}
	/*
	 * Increment the summary information corresponding to the entire
	 * (new) free extent.
	 */
	error = xfs_rtmodify_summary(mp, tp,
		XFS_RTBLOCKLOG(postblock + 1 - preblock),
		XFS_BITTOBLOCK(mp, preblock), 1, rbpp, rsb);
	return error;
}

/*
 * Read and return the summary information for a given extent size,
 * bitmap block combination.
 * Keeps track of a current summary block, so we don't keep reading
 * it from the buffer cache.
 */
STATIC int				/* error */
xfs_rtget_summary(
	xfs_mount_t	*mp,		/* file system mount structure */
	xfs_trans_t	*tp,		/* transaction pointer */
	int		log,		/* log2 of extent size */
	xfs_rtblock_t	bbno,		/* bitmap block number */
	xfs_buf_t	**rbpp,		/* in/out: summary block buffer */
	xfs_fsblock_t	*rsb,		/* in/out: summary block number */
	xfs_suminfo_t	*sum)		/* out: summary info for this block */
{
	xfs_buf_t	*bp;		/* buffer for summary block */
	int		error;		/* error value */
	xfs_fsblock_t	sb;		/* summary fsblock */
	int		so;		/* index into the summary file */
	xfs_suminfo_t	*sp;		/* pointer to returned data */

	/*
	 * Compute entry number in the summary file.
	 */
	so = XFS_SUMOFFS(mp, log, bbno);
	/*
	 * Compute the block number in the summary file.
	 */
	sb = XFS_SUMOFFSTOBLOCK(mp, so);
	/*
	 * If we have an old buffer, and the block number matches, use that.
	 */
	if (rbpp && *rbpp && *rsb == sb)
		bp = *rbpp;
	/*
	 * Otherwise we have to get the buffer.
	 */
	else {
		/*
		 * If there was an old one, get rid of it first.
		 */
		if (rbpp && *rbpp)
			xfs_trans_brelse(tp, *rbpp);
		error = xfs_rtbuf_get(mp, tp, sb, 1, &bp);
		if (error) {
			return error;
		}
		/*
		 * Remember this buffer and block for the next call.
		 */
		if (rbpp) {
			*rbpp = bp;
			*rsb = sb;
		}
	}
	/*
	 * Point to the summary information & copy it out.
	 */
	sp = XFS_SUMPTR(mp, bp, so);
	*sum = *sp;
	/*
	 * Drop the buffer if we're not asked to remember it.
	 */
	if (!rbpp)
		xfs_trans_brelse(tp, bp);
	return 0;
}

/*
 * Set the given range of bitmap bits to the given value.
 * Do whatever I/O and logging is required.
 */
STATIC int				/* error */
xfs_rtmodify_range(
	xfs_mount_t	*mp,		/* file system mount point */
	xfs_trans_t	*tp,		/* transaction pointer */
	xfs_rtblock_t	start,		/* starting block to modify */
	xfs_extlen_t	len,		/* length of extent to modify */
	int		val)		/* 1 for free, 0 for allocated */
{
	xfs_rtword_t	*b;		/* current word in buffer */
	int		bit;		/* bit number in the word */
	xfs_rtblock_t	block;		/* bitmap block number */
	xfs_buf_t	*bp;		/* buf for the block */
	xfs_rtword_t	*bufp;		/* starting word in buffer */
	int		error;		/* error value */
	xfs_rtword_t	*first;		/* first used word in the buffer */
	int		i;		/* current bit number rel. to start */
	int		lastbit;	/* last useful bit in word */
	xfs_rtword_t	mask;		/* mask o frelevant bits for value */
	int		word;		/* word number in the buffer */

	/*
	 * Compute starting bitmap block number.
	 */
	block = XFS_BITTOBLOCK(mp, start);
	/*
	 * Read the bitmap block, and point to its data.
	 */
	error = xfs_rtbuf_get(mp, tp, block, 0, &bp);
	if (error) {
		return error;
	}
	bufp = bp->b_addr;
	/*
	 * Compute the starting word's address, and starting bit.
	 */
	word = XFS_BITTOWORD(mp, start);
	first = b = &bufp[word];
	bit = (int)(start & (XFS_NBWORD - 1));
	/*
	 * 0 (allocated) => all zeroes; 1 (free) => all ones.
	 */
	val = -val;
	/*
	 * If not starting on a word boundary, deal with the first
	 * (partial) word.
	 */
	if (bit) {
		/*
		 * Compute first bit not changed and mask of relevant bits.
		 */
		lastbit = XFS_RTMIN(bit + len, XFS_NBWORD);
		mask = (((xfs_rtword_t)1 << (lastbit - bit)) - 1) << bit;
		/*
		 * Set/clear the active bits.
		 */
		if (val)
			*b |= mask;
		else
			*b &= ~mask;
		i = lastbit - bit;
		/*
		 * Go on to the next block if that's where the next word is
		 * and we need the next word.
		 */
		if (++word == XFS_BLOCKWSIZE(mp) && i < len) {
			/*
			 * Log the changed part of this block.
			 * Get the next one.
			 */
			xfs_trans_log_buf(tp, bp,
				(uint)((char *)first - (char *)bufp),
				(uint)((char *)b - (char *)bufp));
			error = xfs_rtbuf_get(mp, tp, ++block, 0, &bp);
			if (error) {
				return error;
			}
			first = b = bufp = bp->b_addr;
			word = 0;
		} else {
			/*
			 * Go on to the next word in the buffer
			 */
			b++;
		}
	} else {
		/*
		 * Starting on a word boundary, no partial word.
		 */
		i = 0;
	}
	/*
	 * Loop over whole words in buffers.  When we use up one buffer
	 * we move on to the next one.
	 */
	while (len - i >= XFS_NBWORD) {
		/*
		 * Set the word value correctly.
		 */
		*b = val;
		i += XFS_NBWORD;
		/*
		 * Go on to the next block if that's where the next word is
		 * and we need the next word.
		 */
		if (++word == XFS_BLOCKWSIZE(mp) && i < len) {
			/*
			 * Log the changed part of this block.
			 * Get the next one.
			 */
			xfs_trans_log_buf(tp, bp,
				(uint)((char *)first - (char *)bufp),
				(uint)((char *)b - (char *)bufp));
			error = xfs_rtbuf_get(mp, tp, ++block, 0, &bp);
			if (error) {
				return error;
			}
			first = b = bufp = bp->b_addr;
			word = 0;
		} else {
			/*
			 * Go on to the next word in the buffer
			 */
			b++;
		}
	}
	/*
	 * If not ending on a word boundary, deal with the last
	 * (partial) word.
	 */
	if ((lastbit = len - i)) {
		/*
		 * Compute a mask of relevant bits.
		 */
		bit = 0;
		mask = ((xfs_rtword_t)1 << lastbit) - 1;
		/*
		 * Set/clear the active bits.
		 */
		if (val)
			*b |= mask;
		else
			*b &= ~mask;
		b++;
	}
	/*
	 * Log any remaining changed bytes.
	 */
	if (b > first)
		xfs_trans_log_buf(tp, bp, (uint)((char *)first - (char *)bufp),
			(uint)((char *)b - (char *)bufp - 1));
	return 0;
}

/*
 * Read and modify the summary information for a given extent size,
 * bitmap block combination.
 * Keeps track of a current summary block, so we don't keep reading
 * it from the buffer cache.
 */
STATIC int				/* error */
xfs_rtmodify_summary(
	xfs_mount_t	*mp,		/* file system mount point */
	xfs_trans_t	*tp,		/* transaction pointer */
	int		log,		/* log2 of extent size */
	xfs_rtblock_t	bbno,		/* bitmap block number */
	int		delta,		/* change to make to summary info */
	xfs_buf_t	**rbpp,		/* in/out: summary block buffer */
	xfs_fsblock_t	*rsb)		/* in/out: summary block number */
{
	xfs_buf_t	*bp;		/* buffer for the summary block */
	int		error;		/* error value */
	xfs_fsblock_t	sb;		/* summary fsblock */
	int		so;		/* index into the summary file */
	xfs_suminfo_t	*sp;		/* pointer to returned data */

	/*
	 * Compute entry number in the summary file.
	 */
	so = XFS_SUMOFFS(mp, log, bbno);
	/*
	 * Compute the block number in the summary file.
	 */
	sb = XFS_SUMOFFSTOBLOCK(mp, so);
	/*
	 * If we have an old buffer, and the block number matches, use that.
	 */
	if (rbpp && *rbpp && *rsb == sb)
		bp = *rbpp;
	/*
	 * Otherwise we have to get the buffer.
	 */
	else {
		/*
		 * If there was an old one, get rid of it first.
		 */
		if (rbpp && *rbpp)
			xfs_trans_brelse(tp, *rbpp);
		error = xfs_rtbuf_get(mp, tp, sb, 1, &bp);
		if (error) {
			return error;
		}
		/*
		 * Remember this buffer and block for the next call.
		 */
		if (rbpp) {
			*rbpp = bp;
			*rsb = sb;
		}
	}
	/*
	 * Point to the summary information, modify and log it.
	 */
	sp = XFS_SUMPTR(mp, bp, so);
	*sp += delta;
	xfs_trans_log_buf(tp, bp, (uint)((char *)sp - (char *)bp->b_addr),
		(uint)((char *)sp - (char *)bp->b_addr + sizeof(*sp) - 1));
	return 0;
=======
	 *
	 * This time, when we do the allocations, allow smaller ones to succeed,
	 * but make sure the specified minlen/maxlen are in the possible range
	 * for this summary level.
	 */
	for (l = xfs_highbit32(maxlen); l >= xfs_highbit32(minlen); l--) {
		error = xfs_rtalloc_sumlevel(args, l,
				max_t(xfs_rtxlen_t, minlen, 1 << l),
				min_t(xfs_rtxlen_t, maxlen, (1 << (l + 1)) - 1),
				prod, len, rtx);
		if (error != -ENOSPC)
			return error;
	}

	return -ENOSPC;
}

/*
 * Allocate space to the bitmap or summary file, and zero it, for growfs.
 */
STATIC int
xfs_growfs_rt_alloc(
	struct xfs_mount	*mp,		/* file system mount point */
	xfs_extlen_t		oblocks,	/* old count of blocks */
	xfs_extlen_t		nblocks,	/* new count of blocks */
	struct xfs_inode	*ip)		/* inode (bitmap/summary) */
{
	xfs_fileoff_t		bno;		/* block number in file */
	struct xfs_buf		*bp;	/* temporary buffer for zeroing */
	xfs_daddr_t		d;		/* disk block address */
	int			error;		/* error return value */
	xfs_fsblock_t		fsbno;		/* filesystem block for bno */
	struct xfs_bmbt_irec	map;		/* block map output */
	int			nmap;		/* number of block maps */
	int			resblks;	/* space reservation */
	enum xfs_blft		buf_type;
	struct xfs_trans	*tp;

	if (ip == mp->m_rsumip)
		buf_type = XFS_BLFT_RTSUMMARY_BUF;
	else
		buf_type = XFS_BLFT_RTBITMAP_BUF;

	/*
	 * Allocate space to the file, as necessary.
	 */
	while (oblocks < nblocks) {
		resblks = XFS_GROWFSRT_SPACE_RES(mp, nblocks - oblocks);
		/*
		 * Reserve space & log for one extent added to the file.
		 */
		error = xfs_trans_alloc(mp, &M_RES(mp)->tr_growrtalloc, resblks,
				0, 0, &tp);
		if (error)
			return error;
		/*
		 * Lock the inode.
		 */
		xfs_ilock(ip, XFS_ILOCK_EXCL);
		xfs_trans_ijoin(tp, ip, XFS_ILOCK_EXCL);

		error = xfs_iext_count_may_overflow(ip, XFS_DATA_FORK,
				XFS_IEXT_ADD_NOSPLIT_CNT);
		if (error == -EFBIG)
			error = xfs_iext_count_upgrade(tp, ip,
					XFS_IEXT_ADD_NOSPLIT_CNT);
		if (error)
			goto out_trans_cancel;

		/*
		 * Allocate blocks to the bitmap file.
		 */
		nmap = 1;
		error = xfs_bmapi_write(tp, ip, oblocks, nblocks - oblocks,
					XFS_BMAPI_METADATA, 0, &map, &nmap);
		if (!error && nmap < 1)
			error = -ENOSPC;
		if (error)
			goto out_trans_cancel;
		/*
		 * Free any blocks freed up in the transaction, then commit.
		 */
		error = xfs_trans_commit(tp);
		if (error)
			return error;
		/*
		 * Now we need to clear the allocated blocks.
		 * Do this one block per transaction, to keep it simple.
		 */
		for (bno = map.br_startoff, fsbno = map.br_startblock;
		     bno < map.br_startoff + map.br_blockcount;
		     bno++, fsbno++) {
			/*
			 * Reserve log for one block zeroing.
			 */
			error = xfs_trans_alloc(mp, &M_RES(mp)->tr_growrtzero,
					0, 0, 0, &tp);
			if (error)
				return error;
			/*
			 * Lock the bitmap inode.
			 */
			xfs_ilock(ip, XFS_ILOCK_EXCL);
			xfs_trans_ijoin(tp, ip, XFS_ILOCK_EXCL);
			/*
			 * Get a buffer for the block.
			 */
			d = XFS_FSB_TO_DADDR(mp, fsbno);
			error = xfs_trans_get_buf(tp, mp->m_ddev_targp, d,
					mp->m_bsize, 0, &bp);
			if (error)
				goto out_trans_cancel;

			xfs_trans_buf_set_type(tp, bp, buf_type);
			bp->b_ops = &xfs_rtbuf_ops;
			memset(bp->b_addr, 0, mp->m_sb.sb_blocksize);
			xfs_trans_log_buf(tp, bp, 0, mp->m_sb.sb_blocksize - 1);
			/*
			 * Commit the transaction.
			 */
			error = xfs_trans_commit(tp);
			if (error)
				return error;
		}
		/*
		 * Go on to the next extent, if any.
		 */
		oblocks = map.br_startoff + map.br_blockcount;
	}

	return 0;

out_trans_cancel:
	xfs_trans_cancel(tp);
	return error;
}

static void
xfs_alloc_rsum_cache(
	xfs_mount_t	*mp,		/* file system mount structure */
	xfs_extlen_t	rbmblocks)	/* number of rt bitmap blocks */
{
	/*
	 * The rsum cache is initialized to the maximum value, which is
	 * trivially an upper bound on the maximum level with any free extents.
	 * We can continue without the cache if it couldn't be allocated.
	 */
	mp->m_rsum_cache = kvmalloc(rbmblocks, GFP_KERNEL);
	if (mp->m_rsum_cache)
		memset(mp->m_rsum_cache, -1, rbmblocks);
	else
		xfs_warn(mp, "could not allocate realtime summary cache");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Visible (exported) functions.
 */

/*
 * Grow the realtime area of the filesystem.
 */
int
xfs_growfs_rt(
	xfs_mount_t	*mp,		/* mount point for filesystem */
	xfs_growfs_rt_t	*in)		/* growfs rt input struct */
{
<<<<<<< HEAD
	xfs_rtblock_t	bmbno;		/* bitmap block number */
	xfs_buf_t	*bp;		/* temporary buffer */
	int		error;		/* error return value */
	xfs_mount_t	*nmp;		/* new (fake) mount structure */
	xfs_drfsbno_t	nrblocks;	/* new number of realtime blocks */
	xfs_extlen_t	nrbmblocks;	/* new number of rt bitmap blocks */
	xfs_drtbno_t	nrextents;	/* new number of realtime extents */
=======
	xfs_fileoff_t	bmbno;		/* bitmap block number */
	struct xfs_buf	*bp;		/* temporary buffer */
	int		error;		/* error return value */
	xfs_mount_t	*nmp;		/* new (fake) mount structure */
	xfs_rfsblock_t	nrblocks;	/* new number of realtime blocks */
	xfs_extlen_t	nrbmblocks;	/* new number of rt bitmap blocks */
	xfs_rtxnum_t	nrextents;	/* new number of realtime extents */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	uint8_t		nrextslog;	/* new log2 of sb_rextents */
	xfs_extlen_t	nrsumblocks;	/* new number of summary blocks */
	uint		nrsumlevels;	/* new rt summary levels */
	uint		nrsumsize;	/* new size of rt summary, bytes */
	xfs_sb_t	*nsbp;		/* new superblock */
	xfs_extlen_t	rbmblocks;	/* current number of rt bitmap blocks */
	xfs_extlen_t	rsumblocks;	/* current number of rt summary blks */
	xfs_sb_t	*sbp;		/* old superblock */
<<<<<<< HEAD
	xfs_fsblock_t	sumbno;		/* summary block number */

	sbp = &mp->m_sb;
	/*
	 * Initial error checking.
	 */
	if (!capable(CAP_SYS_ADMIN))
		return XFS_ERROR(EPERM);
	if (mp->m_rtdev_targp == NULL || mp->m_rbmip == NULL ||
	    (nrblocks = in->newblocks) <= sbp->sb_rblocks ||
	    (sbp->sb_rblocks && (in->extsize != sbp->sb_rextsize)))
		return XFS_ERROR(EINVAL);
	if ((error = xfs_sb_validate_fsb_count(sbp, nrblocks)))
=======
	uint8_t		*rsum_cache;	/* old summary cache */

	sbp = &mp->m_sb;

	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;

	/* Needs to have been mounted with an rt device. */
	if (!XFS_IS_REALTIME_MOUNT(mp))
		return -EINVAL;
	/*
	 * Mount should fail if the rt bitmap/summary files don't load, but
	 * we'll check anyway.
	 */
	if (!mp->m_rbmip || !mp->m_rsumip)
		return -EINVAL;

	/* Shrink not supported. */
	if (in->newblocks <= sbp->sb_rblocks)
		return -EINVAL;

	/* Can only change rt extent size when adding rt volume. */
	if (sbp->sb_rblocks > 0 && in->extsize != sbp->sb_rextsize)
		return -EINVAL;

	/* Range check the extent size. */
	if (XFS_FSB_TO_B(mp, in->extsize) > XFS_MAX_RTEXTSIZE ||
	    XFS_FSB_TO_B(mp, in->extsize) < XFS_MIN_RTEXTSIZE)
		return -EINVAL;

	/* Unsupported realtime features. */
	if (xfs_has_rmapbt(mp) || xfs_has_reflink(mp) || xfs_has_quota(mp))
		return -EOPNOTSUPP;

	nrblocks = in->newblocks;
	error = xfs_sb_validate_fsb_count(sbp, nrblocks);
	if (error)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return error;
	/*
	 * Read in the last block of the device, make sure it exists.
	 */
<<<<<<< HEAD
	bp = xfs_buf_read_uncached(mp, mp->m_rtdev_targp,
				XFS_FSB_TO_BB(mp, nrblocks - 1),
				XFS_FSB_TO_B(mp, 1), 0);
	if (!bp)
		return EIO;
=======
	error = xfs_buf_read_uncached(mp->m_rtdev_targp,
				XFS_FSB_TO_BB(mp, nrblocks - 1),
				XFS_FSB_TO_BB(mp, 1), 0, &bp, NULL);
	if (error)
		return error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	xfs_buf_relse(bp);

	/*
	 * Calculate new parameters.  These are the final values to be reached.
	 */
	nrextents = nrblocks;
	do_div(nrextents, in->extsize);
<<<<<<< HEAD
	nrbmblocks = howmany_64(nrextents, NBBY * sbp->sb_blocksize);
	nrextslog = xfs_highbit32(nrextents);
	nrsumlevels = nrextslog + 1;
	nrsumsize = (uint)sizeof(xfs_suminfo_t) * nrsumlevels * nrbmblocks;
	nrsumblocks = XFS_B_TO_FSB(mp, nrsumsize);
=======
	if (!xfs_validate_rtextents(nrextents))
		return -EINVAL;
	nrbmblocks = xfs_rtbitmap_blockcount(mp, nrextents);
	nrextslog = xfs_compute_rextslog(nrextents);
	nrsumlevels = nrextslog + 1;
	nrsumblocks = xfs_rtsummary_blockcount(mp, nrsumlevels, nrbmblocks);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	nrsumsize = XFS_FSB_TO_B(mp, nrsumblocks);
	/*
	 * New summary size can't be more than half the size of
	 * the log.  This prevents us from getting a log overflow,
	 * since we'll log basically the whole summary file at once.
	 */
	if (nrsumblocks > (mp->m_sb.sb_logblocks >> 1))
<<<<<<< HEAD
		return XFS_ERROR(EINVAL);
=======
		return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Get the old block counts for bitmap and summary inodes.
	 * These can't change since other growfs callers are locked out.
	 */
<<<<<<< HEAD
	rbmblocks = XFS_B_TO_FSB(mp, mp->m_rbmip->i_d.di_size);
	rsumblocks = XFS_B_TO_FSB(mp, mp->m_rsumip->i_d.di_size);
=======
	rbmblocks = XFS_B_TO_FSB(mp, mp->m_rbmip->i_disk_size);
	rsumblocks = XFS_B_TO_FSB(mp, mp->m_rsumip->i_disk_size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Allocate space to the bitmap and summary files, as necessary.
	 */
	error = xfs_growfs_rt_alloc(mp, rbmblocks, nrbmblocks, mp->m_rbmip);
	if (error)
		return error;
	error = xfs_growfs_rt_alloc(mp, rsumblocks, nrsumblocks, mp->m_rsumip);
	if (error)
		return error;
<<<<<<< HEAD
	/*
	 * Allocate a new (fake) mount/sb.
	 */
	nmp = kmem_alloc(sizeof(*nmp), KM_SLEEP);
=======

	rsum_cache = mp->m_rsum_cache;
	if (nrbmblocks != sbp->sb_rbmblocks)
		xfs_alloc_rsum_cache(mp, nrbmblocks);

	/*
	 * Allocate a new (fake) mount/sb.
	 */
	nmp = kmalloc(sizeof(*nmp), GFP_KERNEL | __GFP_NOFAIL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Loop over the bitmap blocks.
	 * We will do everything one bitmap block at a time.
	 * Skip the current block if it is exactly full.
	 * This also deals with the case where there were no rtextents before.
	 */
	for (bmbno = sbp->sb_rbmblocks -
		     ((sbp->sb_rextents & ((1 << mp->m_blkbit_log) - 1)) != 0);
	     bmbno < nrbmblocks;
	     bmbno++) {
<<<<<<< HEAD
		xfs_trans_t	*tp;
		int		cancelflags = 0;
=======
		struct xfs_rtalloc_args	args = {
			.mp		= mp,
		};
		struct xfs_rtalloc_args	nargs = {
			.mp		= nmp,
		};
		struct xfs_trans	*tp;
		xfs_rfsblock_t		nrblocks_step;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		*nmp = *mp;
		nsbp = &nmp->m_sb;
		/*
		 * Calculate new sb and mount fields for this round.
		 */
		nsbp->sb_rextsize = in->extsize;
<<<<<<< HEAD
		nsbp->sb_rbmblocks = bmbno + 1;
		nsbp->sb_rblocks =
			XFS_RTMIN(nrblocks,
				  nsbp->sb_rbmblocks * NBBY *
				  nsbp->sb_blocksize * nsbp->sb_rextsize);
		nsbp->sb_rextents = nsbp->sb_rblocks;
		do_div(nsbp->sb_rextents, nsbp->sb_rextsize);
		ASSERT(nsbp->sb_rextents != 0);
		nsbp->sb_rextslog = xfs_highbit32(nsbp->sb_rextents);
		nrsumlevels = nmp->m_rsumlevels = nsbp->sb_rextslog + 1;
		nrsumsize =
			(uint)sizeof(xfs_suminfo_t) * nrsumlevels *
			nsbp->sb_rbmblocks;
		nrsumblocks = XFS_B_TO_FSB(mp, nrsumsize);
		nmp->m_rsumsize = nrsumsize = XFS_FSB_TO_B(mp, nrsumblocks);
		/*
		 * Start a transaction, get the log reservation.
		 */
		tp = xfs_trans_alloc(mp, XFS_TRANS_GROWFSRT_FREE);
		if ((error = xfs_trans_reserve(tp, 0,
				XFS_GROWRTFREE_LOG_RES(nmp), 0, 0, 0)))
			goto error_cancel;
		/*
		 * Lock out other callers by grabbing the bitmap inode lock.
		 */
		xfs_ilock(mp->m_rbmip, XFS_ILOCK_EXCL);
		xfs_trans_ijoin(tp, mp->m_rbmip, XFS_ILOCK_EXCL);
		/*
		 * Update the bitmap inode's size.
		 */
		mp->m_rbmip->i_d.di_size =
			nsbp->sb_rbmblocks * nsbp->sb_blocksize;
		xfs_trans_log_inode(tp, mp->m_rbmip, XFS_ILOG_CORE);
		cancelflags |= XFS_TRANS_ABORT;
		/*
		 * Get the summary inode into the transaction.
		 */
		xfs_ilock(mp->m_rsumip, XFS_ILOCK_EXCL);
		xfs_trans_ijoin(tp, mp->m_rsumip, XFS_ILOCK_EXCL);
		/*
		 * Update the summary inode's size.
		 */
		mp->m_rsumip->i_d.di_size = nmp->m_rsumsize;
=======
		nmp->m_rtxblklog = -1; /* don't use shift or masking */
		nsbp->sb_rbmblocks = bmbno + 1;
		nrblocks_step = (bmbno + 1) * NBBY * nsbp->sb_blocksize *
				nsbp->sb_rextsize;
		nsbp->sb_rblocks = min(nrblocks, nrblocks_step);
		nsbp->sb_rextents = xfs_rtb_to_rtx(nmp, nsbp->sb_rblocks);
		ASSERT(nsbp->sb_rextents != 0);
		nsbp->sb_rextslog = xfs_compute_rextslog(nsbp->sb_rextents);
		nrsumlevels = nmp->m_rsumlevels = nsbp->sb_rextslog + 1;
		nrsumblocks = xfs_rtsummary_blockcount(mp, nrsumlevels,
				nsbp->sb_rbmblocks);
		nmp->m_rsumsize = nrsumsize = XFS_FSB_TO_B(mp, nrsumblocks);
		/* recompute growfsrt reservation from new rsumsize */
		xfs_trans_resv_calc(nmp, &nmp->m_resv);

		/*
		 * Start a transaction, get the log reservation.
		 */
		error = xfs_trans_alloc(mp, &M_RES(mp)->tr_growrtfree, 0, 0, 0,
				&tp);
		if (error)
			break;
		args.tp = tp;
		nargs.tp = tp;

		/*
		 * Lock out other callers by grabbing the bitmap inode lock.
		 */
		xfs_ilock(mp->m_rbmip, XFS_ILOCK_EXCL | XFS_ILOCK_RTBITMAP);
		xfs_trans_ijoin(tp, mp->m_rbmip, XFS_ILOCK_EXCL);
		/*
		 * Update the bitmap inode's size ondisk and incore.  We need
		 * to update the incore size so that inode inactivation won't
		 * punch what it thinks are "posteof" blocks.
		 */
		mp->m_rbmip->i_disk_size =
			nsbp->sb_rbmblocks * nsbp->sb_blocksize;
		i_size_write(VFS_I(mp->m_rbmip), mp->m_rbmip->i_disk_size);
		xfs_trans_log_inode(tp, mp->m_rbmip, XFS_ILOG_CORE);
		/*
		 * Get the summary inode into the transaction.
		 */
		xfs_ilock(mp->m_rsumip, XFS_ILOCK_EXCL | XFS_ILOCK_RTSUM);
		xfs_trans_ijoin(tp, mp->m_rsumip, XFS_ILOCK_EXCL);
		/*
		 * Update the summary inode's size.  We need to update the
		 * incore size so that inode inactivation won't punch what it
		 * thinks are "posteof" blocks.
		 */
		mp->m_rsumip->i_disk_size = nmp->m_rsumsize;
		i_size_write(VFS_I(mp->m_rsumip), mp->m_rsumip->i_disk_size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		xfs_trans_log_inode(tp, mp->m_rsumip, XFS_ILOG_CORE);
		/*
		 * Copy summary data from old to new sizes.
		 * Do this when the real size (not block-aligned) changes.
		 */
		if (sbp->sb_rbmblocks != nsbp->sb_rbmblocks ||
		    mp->m_rsumlevels != nmp->m_rsumlevels) {
<<<<<<< HEAD
			error = xfs_rtcopy_summary(mp, nmp, tp);
=======
			error = xfs_rtcopy_summary(&args, &nargs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (error)
				goto error_cancel;
		}
		/*
		 * Update superblock fields.
		 */
		if (nsbp->sb_rextsize != sbp->sb_rextsize)
			xfs_trans_mod_sb(tp, XFS_TRANS_SB_REXTSIZE,
				nsbp->sb_rextsize - sbp->sb_rextsize);
		if (nsbp->sb_rbmblocks != sbp->sb_rbmblocks)
			xfs_trans_mod_sb(tp, XFS_TRANS_SB_RBMBLOCKS,
				nsbp->sb_rbmblocks - sbp->sb_rbmblocks);
		if (nsbp->sb_rblocks != sbp->sb_rblocks)
			xfs_trans_mod_sb(tp, XFS_TRANS_SB_RBLOCKS,
				nsbp->sb_rblocks - sbp->sb_rblocks);
		if (nsbp->sb_rextents != sbp->sb_rextents)
			xfs_trans_mod_sb(tp, XFS_TRANS_SB_REXTENTS,
				nsbp->sb_rextents - sbp->sb_rextents);
		if (nsbp->sb_rextslog != sbp->sb_rextslog)
			xfs_trans_mod_sb(tp, XFS_TRANS_SB_REXTSLOG,
				nsbp->sb_rextslog - sbp->sb_rextslog);
		/*
		 * Free new extent.
		 */
<<<<<<< HEAD
		bp = NULL;
		error = xfs_rtfree_range(nmp, tp, sbp->sb_rextents,
			nsbp->sb_rextents - sbp->sb_rextents, &bp, &sumbno);
		if (error) {
error_cancel:
			xfs_trans_cancel(tp, cancelflags);
=======
		error = xfs_rtfree_range(&nargs, sbp->sb_rextents,
				nsbp->sb_rextents - sbp->sb_rextents);
		xfs_rtbuf_cache_relse(&nargs);
		if (error) {
error_cancel:
			xfs_trans_cancel(tp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		}
		/*
		 * Mark more blocks free in the superblock.
		 */
		xfs_trans_mod_sb(tp, XFS_TRANS_SB_FREXTENTS,
			nsbp->sb_rextents - sbp->sb_rextents);
		/*
		 * Update mp values into the real mp structure.
		 */
		mp->m_rsumlevels = nrsumlevels;
		mp->m_rsumsize = nrsumsize;
<<<<<<< HEAD

		error = xfs_trans_commit(tp, 0);
		if (error)
			break;
	}

	/*
	 * Free the fake mp structure.
	 */
	kmem_free(nmp);
=======
		/* recompute growfsrt reservation from new rsumsize */
		xfs_trans_resv_calc(mp, &mp->m_resv);

		error = xfs_trans_commit(tp);
		if (error)
			break;

		/* Ensure the mount RT feature flag is now set. */
		mp->m_features |= XFS_FEAT_REALTIME;
	}
	if (error)
		goto out_free;

	/* Update secondary superblocks now the physical grow has completed */
	error = xfs_update_secondary_sbs(mp);

out_free:
	/*
	 * Free the fake mp structure.
	 */
	kfree(nmp);

	/*
	 * If we had to allocate a new rsum_cache, we either need to free the
	 * old one (if we succeeded) or free the new one and restore the old one
	 * (if there was an error).
	 */
	if (rsum_cache != mp->m_rsum_cache) {
		if (error) {
			kvfree(mp->m_rsum_cache);
			mp->m_rsum_cache = rsum_cache;
		} else {
			kvfree(rsum_cache);
		}
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return error;
}

/*
<<<<<<< HEAD
 * Allocate an extent in the realtime subvolume, with the usual allocation
 * parameters.  The length units are all in realtime extents, as is the
 * result block number.
 */
int					/* error */
xfs_rtallocate_extent(
	xfs_trans_t	*tp,		/* transaction pointer */
	xfs_rtblock_t	bno,		/* starting block number to allocate */
	xfs_extlen_t	minlen,		/* minimum length to allocate */
	xfs_extlen_t	maxlen,		/* maximum length to allocate */
	xfs_extlen_t	*len,		/* out: actual length allocated */
	xfs_alloctype_t	type,		/* allocation type XFS_ALLOCTYPE... */
	int		wasdel,		/* was a delayed allocation extent */
	xfs_extlen_t	prod,		/* extent product factor */
	xfs_rtblock_t	*rtblock)	/* out: start block allocated */
{
	xfs_mount_t	*mp = tp->t_mountp;
	int		error;		/* error value */
	xfs_rtblock_t	r;		/* result allocated block */
	xfs_fsblock_t	sb;		/* summary file block number */
	xfs_buf_t	*sumbp;		/* summary file block buffer */

	ASSERT(xfs_isilocked(mp->m_rbmip, XFS_ILOCK_EXCL));
	ASSERT(minlen > 0 && minlen <= maxlen);

	/*
	 * If prod is set then figure out what to do to minlen and maxlen.
	 */
	if (prod > 1) {
		xfs_extlen_t	i;

		if ((i = maxlen % prod))
			maxlen -= i;
		if ((i = minlen % prod))
			minlen += prod - i;
		if (maxlen < minlen) {
			*rtblock = NULLRTBLOCK;
			return 0;
		}
	}

	sumbp = NULL;
	/*
	 * Allocate by size, or near another block, or exactly at some block.
	 */
	switch (type) {
	case XFS_ALLOCTYPE_ANY_AG:
		error = xfs_rtallocate_extent_size(mp, tp, minlen, maxlen, len,
				&sumbp,	&sb, prod, &r);
		break;
	case XFS_ALLOCTYPE_NEAR_BNO:
		error = xfs_rtallocate_extent_near(mp, tp, bno, minlen, maxlen,
				len, &sumbp, &sb, prod, &r);
		break;
	case XFS_ALLOCTYPE_THIS_BNO:
		error = xfs_rtallocate_extent_exact(mp, tp, bno, minlen, maxlen,
				len, &sumbp, &sb, prod, &r);
		break;
	default:
		error = EIO;
		ASSERT(0);
	}
	if (error)
		return error;

	/*
	 * If it worked, update the superblock.
	 */
	if (r != NULLRTBLOCK) {
		long	slen = (long)*len;

		ASSERT(*len >= minlen && *len <= maxlen);
		if (wasdel)
			xfs_trans_mod_sb(tp, XFS_TRANS_SB_RES_FREXTENTS, -slen);
		else
			xfs_trans_mod_sb(tp, XFS_TRANS_SB_FREXTENTS, -slen);
	}
	*rtblock = r;
	return 0;
}

/*
 * Free an extent in the realtime subvolume.  Length is expressed in
 * realtime extents, as is the block number.
 */
int					/* error */
xfs_rtfree_extent(
	xfs_trans_t	*tp,		/* transaction pointer */
	xfs_rtblock_t	bno,		/* starting block number to free */
	xfs_extlen_t	len)		/* length of extent freed */
{
	int		error;		/* error value */
	xfs_mount_t	*mp;		/* file system mount structure */
	xfs_fsblock_t	sb;		/* summary file block number */
	xfs_buf_t	*sumbp;		/* summary file block buffer */

	mp = tp->t_mountp;

	ASSERT(mp->m_rbmip->i_itemp != NULL);
	ASSERT(xfs_isilocked(mp->m_rbmip, XFS_ILOCK_EXCL));

#if defined(__KERNEL__) && defined(DEBUG)
	/*
	 * Check to see that this whole range is currently allocated.
	 */
	{
		int	stat;		/* result from checking range */

		error = xfs_rtcheck_alloc_range(mp, tp, bno, len, &stat);
		if (error) {
			return error;
		}
		ASSERT(stat);
	}
#endif
	sumbp = NULL;
	/*
	 * Free the range of realtime blocks.
	 */
	error = xfs_rtfree_range(mp, tp, bno, len, &sumbp, &sb);
	if (error) {
		return error;
	}
	/*
	 * Mark more blocks free in the superblock.
	 */
	xfs_trans_mod_sb(tp, XFS_TRANS_SB_FREXTENTS, (long)len);
	/*
	 * If we've now freed all the blocks, reset the file sequence
	 * number to 0.
	 */
	if (tp->t_frextents_delta + mp->m_sb.sb_frextents ==
	    mp->m_sb.sb_rextents) {
		if (!(mp->m_rbmip->i_d.di_flags & XFS_DIFLAG_NEWRTBM))
			mp->m_rbmip->i_d.di_flags |= XFS_DIFLAG_NEWRTBM;
		*(__uint64_t *)&mp->m_rbmip->i_d.di_atime = 0;
		xfs_trans_log_inode(tp, mp->m_rbmip, XFS_ILOG_CORE);
	}
	return 0;
}

/*
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Initialize realtime fields in the mount structure.
 */
int				/* error */
xfs_rtmount_init(
<<<<<<< HEAD
	xfs_mount_t	*mp)	/* file system mount structure */
{
	xfs_buf_t	*bp;	/* buffer for last block of subvolume */
	xfs_daddr_t	d;	/* address of last block of subvolume */
	xfs_sb_t	*sbp;	/* filesystem superblock copy in mount */
=======
	struct xfs_mount	*mp)	/* file system mount structure */
{
	struct xfs_buf		*bp;	/* buffer for last block of subvolume */
	struct xfs_sb		*sbp;	/* filesystem superblock copy in mount */
	xfs_daddr_t		d;	/* address of last block of subvolume */
	unsigned int		rsumblocks;
	int			error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	sbp = &mp->m_sb;
	if (sbp->sb_rblocks == 0)
		return 0;
	if (mp->m_rtdev_targp == NULL) {
		xfs_warn(mp,
	"Filesystem has a realtime volume, use rtdev=device option");
<<<<<<< HEAD
		return XFS_ERROR(ENODEV);
	}
	mp->m_rsumlevels = sbp->sb_rextslog + 1;
	mp->m_rsumsize =
		(uint)sizeof(xfs_suminfo_t) * mp->m_rsumlevels *
		sbp->sb_rbmblocks;
	mp->m_rsumsize = roundup(mp->m_rsumsize, sbp->sb_blocksize);
=======
		return -ENODEV;
	}
	mp->m_rsumlevels = sbp->sb_rextslog + 1;
	rsumblocks = xfs_rtsummary_blockcount(mp, mp->m_rsumlevels,
			mp->m_sb.sb_rbmblocks);
	mp->m_rsumsize = XFS_FSB_TO_B(mp, rsumblocks);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mp->m_rbmip = mp->m_rsumip = NULL;
	/*
	 * Check that the realtime section is an ok size.
	 */
	d = (xfs_daddr_t)XFS_FSB_TO_BB(mp, mp->m_sb.sb_rblocks);
	if (XFS_BB_TO_FSB(mp, d) != mp->m_sb.sb_rblocks) {
		xfs_warn(mp, "realtime mount -- %llu != %llu",
			(unsigned long long) XFS_BB_TO_FSB(mp, d),
			(unsigned long long) mp->m_sb.sb_rblocks);
<<<<<<< HEAD
		return XFS_ERROR(EFBIG);
	}
	bp = xfs_buf_read_uncached(mp, mp->m_rtdev_targp,
					d - XFS_FSB_TO_BB(mp, 1),
					XFS_FSB_TO_B(mp, 1), 0);
	if (!bp) {
		xfs_warn(mp, "realtime device size check failed");
		return EIO;
=======
		return -EFBIG;
	}
	error = xfs_buf_read_uncached(mp->m_rtdev_targp,
					d - XFS_FSB_TO_BB(mp, 1),
					XFS_FSB_TO_BB(mp, 1), 0, &bp, NULL);
	if (error) {
		xfs_warn(mp, "realtime device size check failed");
		return error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	xfs_buf_relse(bp);
	return 0;
}

<<<<<<< HEAD
/*
 * Get the bitmap and summary inodes into the mount structure
 * at mount time.
=======
static int
xfs_rtalloc_count_frextent(
	struct xfs_mount		*mp,
	struct xfs_trans		*tp,
	const struct xfs_rtalloc_rec	*rec,
	void				*priv)
{
	uint64_t			*valp = priv;

	*valp += rec->ar_extcount;
	return 0;
}

/*
 * Reinitialize the number of free realtime extents from the realtime bitmap.
 * Callers must ensure that there is no other activity in the filesystem.
 */
int
xfs_rtalloc_reinit_frextents(
	struct xfs_mount	*mp)
{
	uint64_t		val = 0;
	int			error;

	xfs_ilock(mp->m_rbmip, XFS_ILOCK_SHARED | XFS_ILOCK_RTBITMAP);
	error = xfs_rtalloc_query_all(mp, NULL, xfs_rtalloc_count_frextent,
			&val);
	xfs_iunlock(mp->m_rbmip, XFS_ILOCK_SHARED | XFS_ILOCK_RTBITMAP);
	if (error)
		return error;

	spin_lock(&mp->m_sb_lock);
	mp->m_sb.sb_frextents = val;
	spin_unlock(&mp->m_sb_lock);
	percpu_counter_set(&mp->m_frextents, mp->m_sb.sb_frextents);
	return 0;
}

/*
 * Read in the bmbt of an rt metadata inode so that we never have to load them
 * at runtime.  This enables the use of shared ILOCKs for rtbitmap scans.  Use
 * an empty transaction to avoid deadlocking on loops in the bmbt.
 */
static inline int
xfs_rtmount_iread_extents(
	struct xfs_inode	*ip,
	unsigned int		lock_class)
{
	struct xfs_trans	*tp;
	int			error;

	error = xfs_trans_alloc_empty(ip->i_mount, &tp);
	if (error)
		return error;

	xfs_ilock(ip, XFS_ILOCK_EXCL | lock_class);

	error = xfs_iread_extents(tp, ip, XFS_DATA_FORK);
	if (error)
		goto out_unlock;

	if (xfs_inode_has_attr_fork(ip)) {
		error = xfs_iread_extents(tp, ip, XFS_ATTR_FORK);
		if (error)
			goto out_unlock;
	}

out_unlock:
	xfs_iunlock(ip, XFS_ILOCK_EXCL | lock_class);
	xfs_trans_cancel(tp);
	return error;
}

/*
 * Get the bitmap and summary inodes and the summary cache into the mount
 * structure at mount time.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
int					/* error */
xfs_rtmount_inodes(
	xfs_mount_t	*mp)		/* file system mount structure */
{
	int		error;		/* error return value */
	xfs_sb_t	*sbp;

	sbp = &mp->m_sb;
<<<<<<< HEAD
	if (sbp->sb_rbmino == NULLFSINO)
		return 0;
	error = xfs_iget(mp, NULL, sbp->sb_rbmino, 0, 0, &mp->m_rbmip);
	if (error)
		return error;
	ASSERT(mp->m_rbmip != NULL);
	ASSERT(sbp->sb_rsumino != NULLFSINO);
	error = xfs_iget(mp, NULL, sbp->sb_rsumino, 0, 0, &mp->m_rsumip);
	if (error) {
		IRELE(mp->m_rbmip);
		return error;
	}
	ASSERT(mp->m_rsumip != NULL);
	return 0;
=======
	error = xfs_iget(mp, NULL, sbp->sb_rbmino, 0, 0, &mp->m_rbmip);
	if (xfs_metadata_is_sick(error))
		xfs_rt_mark_sick(mp, XFS_SICK_RT_BITMAP);
	if (error)
		return error;
	ASSERT(mp->m_rbmip != NULL);

	error = xfs_rtmount_iread_extents(mp->m_rbmip, XFS_ILOCK_RTBITMAP);
	if (error)
		goto out_rele_bitmap;

	error = xfs_iget(mp, NULL, sbp->sb_rsumino, 0, 0, &mp->m_rsumip);
	if (xfs_metadata_is_sick(error))
		xfs_rt_mark_sick(mp, XFS_SICK_RT_SUMMARY);
	if (error)
		goto out_rele_bitmap;
	ASSERT(mp->m_rsumip != NULL);

	error = xfs_rtmount_iread_extents(mp->m_rsumip, XFS_ILOCK_RTSUM);
	if (error)
		goto out_rele_summary;

	xfs_alloc_rsum_cache(mp, sbp->sb_rbmblocks);
	return 0;

out_rele_summary:
	xfs_irele(mp->m_rsumip);
out_rele_bitmap:
	xfs_irele(mp->m_rbmip);
	return error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void
xfs_rtunmount_inodes(
	struct xfs_mount	*mp)
{
<<<<<<< HEAD
	if (mp->m_rbmip)
		IRELE(mp->m_rbmip);
	if (mp->m_rsumip)
		IRELE(mp->m_rsumip);
=======
	kvfree(mp->m_rsum_cache);
	if (mp->m_rbmip)
		xfs_irele(mp->m_rbmip);
	if (mp->m_rsumip)
		xfs_irele(mp->m_rsumip);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Pick an extent for allocation at the start of a new realtime file.
 * Use the sequence number stored in the atime field of the bitmap inode.
 * Translate this to a fraction of the rtextents, and return the product
 * of rtextents and the fraction.
 * The fraction sequence is 0, 1/2, 1/4, 3/4, 1/8, ..., 7/8, 1/16, ...
 */
<<<<<<< HEAD
int					/* error */
xfs_rtpick_extent(
	xfs_mount_t	*mp,		/* file system mount point */
	xfs_trans_t	*tp,		/* transaction pointer */
	xfs_extlen_t	len,		/* allocation length (rtextents) */
	xfs_rtblock_t	*pick)		/* result rt extent */
{
	xfs_rtblock_t	b;		/* result block */
	int		log2;		/* log of sequence number */
	__uint64_t	resid;		/* residual after log removed */
	__uint64_t	seq;		/* sequence number of file creation */
	__uint64_t	*seqp;		/* pointer to seqno in inode */

	ASSERT(xfs_isilocked(mp->m_rbmip, XFS_ILOCK_EXCL));

	seqp = (__uint64_t *)&mp->m_rbmip->i_d.di_atime;
	if (!(mp->m_rbmip->i_d.di_flags & XFS_DIFLAG_NEWRTBM)) {
		mp->m_rbmip->i_d.di_flags |= XFS_DIFLAG_NEWRTBM;
		*seqp = 0;
	}
	seq = *seqp;
=======
static int
xfs_rtpick_extent(
	xfs_mount_t		*mp,		/* file system mount point */
	xfs_trans_t		*tp,		/* transaction pointer */
	xfs_rtxlen_t		len,		/* allocation length (rtextents) */
	xfs_rtxnum_t		*pick)		/* result rt extent */
{
	xfs_rtxnum_t		b;		/* result rtext */
	int			log2;		/* log of sequence number */
	uint64_t		resid;		/* residual after log removed */
	uint64_t		seq;		/* sequence number of file creation */
	struct timespec64	ts;		/* timespec in inode */

	xfs_assert_ilocked(mp->m_rbmip, XFS_ILOCK_EXCL);

	ts = inode_get_atime(VFS_I(mp->m_rbmip));
	if (!(mp->m_rbmip->i_diflags & XFS_DIFLAG_NEWRTBM)) {
		mp->m_rbmip->i_diflags |= XFS_DIFLAG_NEWRTBM;
		seq = 0;
	} else {
		seq = ts.tv_sec;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if ((log2 = xfs_highbit64(seq)) == -1)
		b = 0;
	else {
		resid = seq - (1ULL << log2);
		b = (mp->m_sb.sb_rextents * ((resid << 1) + 1ULL)) >>
		    (log2 + 1);
		if (b >= mp->m_sb.sb_rextents)
<<<<<<< HEAD
			b = do_mod(b, mp->m_sb.sb_rextents);
		if (b + len > mp->m_sb.sb_rextents)
			b = mp->m_sb.sb_rextents - len;
	}
	*seqp = seq + 1;
=======
			div64_u64_rem(b, mp->m_sb.sb_rextents, &b);
		if (b + len > mp->m_sb.sb_rextents)
			b = mp->m_sb.sb_rextents - len;
	}
	ts.tv_sec = seq + 1;
	inode_set_atime_to_ts(VFS_I(mp->m_rbmip), ts);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	xfs_trans_log_inode(tp, mp->m_rbmip, XFS_ILOG_CORE);
	*pick = b;
	return 0;
}
<<<<<<< HEAD
=======

static void
xfs_rtalloc_align_minmax(
	xfs_rtxlen_t		*raminlen,
	xfs_rtxlen_t		*ramaxlen,
	xfs_rtxlen_t		*prod)
{
	xfs_rtxlen_t		newmaxlen = *ramaxlen;
	xfs_rtxlen_t		newminlen = *raminlen;
	xfs_rtxlen_t		slack;

	slack = newmaxlen % *prod;
	if (slack)
		newmaxlen -= slack;
	slack = newminlen % *prod;
	if (slack)
		newminlen += *prod - slack;

	/*
	 * If adjusting for extent size hint alignment produces an invalid
	 * min/max len combination, go ahead without it.
	 */
	if (newmaxlen < newminlen) {
		*prod = 1;
		return;
	}
	*ramaxlen = newmaxlen;
	*raminlen = newminlen;
}

int
xfs_bmap_rtalloc(
	struct xfs_bmalloca	*ap)
{
	struct xfs_mount	*mp = ap->ip->i_mount;
	xfs_fileoff_t		orig_offset = ap->offset;
	xfs_rtxnum_t		start;	   /* allocation hint rtextent no */
	xfs_rtxnum_t		rtx;	   /* actually allocated rtextent no */
	xfs_rtxlen_t		prod = 0;  /* product factor for allocators */
	xfs_extlen_t		mod = 0;   /* product factor for allocators */
	xfs_rtxlen_t		ralen = 0; /* realtime allocation length */
	xfs_extlen_t		align;     /* minimum allocation alignment */
	xfs_extlen_t		orig_length = ap->length;
	xfs_extlen_t		minlen = mp->m_sb.sb_rextsize;
	xfs_rtxlen_t		raminlen;
	bool			rtlocked = false;
	bool			ignore_locality = false;
	struct xfs_rtalloc_args	args = {
		.mp		= mp,
		.tp		= ap->tp,
	};
	int			error;

	align = xfs_get_extsz_hint(ap->ip);
retry:
	error = xfs_bmap_extsize_align(mp, &ap->got, &ap->prev,
					align, 1, ap->eof, 0,
					ap->conv, &ap->offset, &ap->length);
	if (error)
		return error;
	ASSERT(ap->length);
	ASSERT(xfs_extlen_to_rtxmod(mp, ap->length) == 0);

	/*
	 * If we shifted the file offset downward to satisfy an extent size
	 * hint, increase minlen by that amount so that the allocator won't
	 * give us an allocation that's too short to cover at least one of the
	 * blocks that the caller asked for.
	 */
	if (ap->offset != orig_offset)
		minlen += orig_offset - ap->offset;

	/*
	 * Set ralen to be the actual requested length in rtextents.
	 *
	 * If the old value was close enough to XFS_BMBT_MAX_EXTLEN that
	 * we rounded up to it, cut it back so it's valid again.
	 * Note that if it's a really large request (bigger than
	 * XFS_BMBT_MAX_EXTLEN), we don't hear about that number, and can't
	 * adjust the starting point to match it.
	 */
	ralen = xfs_extlen_to_rtxlen(mp, min(ap->length, XFS_MAX_BMBT_EXTLEN));
	raminlen = max_t(xfs_rtxlen_t, 1, xfs_extlen_to_rtxlen(mp, minlen));
	ASSERT(raminlen > 0);
	ASSERT(raminlen <= ralen);

	/*
	 * Lock out modifications to both the RT bitmap and summary inodes
	 */
	if (!rtlocked) {
		xfs_ilock(mp->m_rbmip, XFS_ILOCK_EXCL|XFS_ILOCK_RTBITMAP);
		xfs_trans_ijoin(ap->tp, mp->m_rbmip, XFS_ILOCK_EXCL);
		xfs_ilock(mp->m_rsumip, XFS_ILOCK_EXCL|XFS_ILOCK_RTSUM);
		xfs_trans_ijoin(ap->tp, mp->m_rsumip, XFS_ILOCK_EXCL);
		rtlocked = true;
	}

	if (ignore_locality) {
		start = 0;
	} else if (xfs_bmap_adjacent(ap)) {
		start = xfs_rtb_to_rtx(mp, ap->blkno);
	} else if (ap->eof && ap->offset == 0) {
		/*
		 * If it's an allocation to an empty file at offset 0, pick an
		 * extent that will space things out in the rt area.
		 */
		error = xfs_rtpick_extent(mp, ap->tp, ralen, &start);
		if (error)
			return error;
	} else {
		start = 0;
	}

	/*
	 * Only bother calculating a real prod factor if offset & length are
	 * perfectly aligned, otherwise it will just get us in trouble.
	 */
	div_u64_rem(ap->offset, align, &mod);
	if (mod || ap->length % align) {
		prod = 1;
	} else {
		prod = xfs_extlen_to_rtxlen(mp, align);
		if (prod > 1)
			xfs_rtalloc_align_minmax(&raminlen, &ralen, &prod);
	}

	if (start) {
		error = xfs_rtallocate_extent_near(&args, start, raminlen,
				ralen, &ralen, prod, &rtx);
	} else {
		error = xfs_rtallocate_extent_size(&args, raminlen,
				ralen, &ralen, prod, &rtx);
	}
	xfs_rtbuf_cache_relse(&args);

	if (error == -ENOSPC) {
		if (align > mp->m_sb.sb_rextsize) {
			/*
			 * We previously enlarged the request length to try to
			 * satisfy an extent size hint.  The allocator didn't
			 * return anything, so reset the parameters to the
			 * original values and try again without alignment
			 * criteria.
			 */
			ap->offset = orig_offset;
			ap->length = orig_length;
			minlen = align = mp->m_sb.sb_rextsize;
			goto retry;
		}

		if (!ignore_locality && start != 0) {
			/*
			 * If we can't allocate near a specific rt extent, try
			 * again without locality criteria.
			 */
			ignore_locality = true;
			goto retry;
		}

		ap->blkno = NULLFSBLOCK;
		ap->length = 0;
		return 0;
	}
	if (error)
		return error;

	xfs_trans_mod_sb(ap->tp, ap->wasdel ?
			XFS_TRANS_SB_RES_FREXTENTS : XFS_TRANS_SB_FREXTENTS,
			-(long)ralen);
	ap->blkno = xfs_rtx_to_rtb(mp, rtx);
	ap->length = xfs_rtxlen_to_extlen(mp, ralen);
	xfs_bmap_alloc_account(ap);
	return 0;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
