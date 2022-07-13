<<<<<<< HEAD
/*
 * Copyright (c) 2000-2002,2005 Silicon Graphics, Inc.
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
#include "xfs_mount.h"
#include "xfs_bmap_btree.h"
#include "xfs_alloc_btree.h"
#include "xfs_ialloc_btree.h"
#include "xfs_dinode.h"
#include "xfs_inode.h"
#include "xfs_ialloc.h"
#include "xfs_itable.h"
#include "xfs_error.h"
#include "xfs_btree.h"
#include "xfs_trace.h"

STATIC int
xfs_internal_inum(
	xfs_mount_t	*mp,
	xfs_ino_t	ino)
{
	return (ino == mp->m_sb.sb_rbmino || ino == mp->m_sb.sb_rsumino ||
		(xfs_sb_version_hasquota(&mp->m_sb) &&
		 (ino == mp->m_sb.sb_uquotino || ino == mp->m_sb.sb_gquotino)));
}

/*
 * Return stat information for one inode.
 * Return 0 if ok, else errno.
 */
int
xfs_bulkstat_one_int(
	struct xfs_mount	*mp,		/* mount point for filesystem */
	xfs_ino_t		ino,		/* inode to get data for */
	void __user		*buffer,	/* buffer to place output in */
	int			ubsize,		/* size of buffer */
	bulkstat_one_fmt_pf	formatter,	/* formatter, copy to user */
	int			*ubused,	/* bytes used by me */
	int			*stat)		/* BULKSTAT_RV_... */
{
	struct xfs_icdinode	*dic;		/* dinode core info pointer */
	struct xfs_inode	*ip;		/* incore inode pointer */
	struct xfs_bstat	*buf;		/* return buffer */
	int			error = 0;	/* error value */

	*stat = BULKSTAT_RV_NOTHING;

	if (!buffer || xfs_internal_inum(mp, ino))
		return XFS_ERROR(EINVAL);

	buf = kmem_alloc(sizeof(*buf), KM_SLEEP | KM_MAYFAIL);
	if (!buf)
		return XFS_ERROR(ENOMEM);

	error = xfs_iget(mp, NULL, ino,
			 (XFS_IGET_DONTCACHE | XFS_IGET_UNTRUSTED),
			 XFS_ILOCK_SHARED, &ip);
	if (error) {
		*stat = BULKSTAT_RV_NOTHING;
		goto out_free;
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2000-2002,2005 Silicon Graphics, Inc.
 * All Rights Reserved.
 */
#include "xfs.h"
#include "xfs_fs.h"
#include "xfs_shared.h"
#include "xfs_format.h"
#include "xfs_log_format.h"
#include "xfs_trans_resv.h"
#include "xfs_mount.h"
#include "xfs_inode.h"
#include "xfs_btree.h"
#include "xfs_ialloc.h"
#include "xfs_ialloc_btree.h"
#include "xfs_iwalk.h"
#include "xfs_itable.h"
#include "xfs_error.h"
#include "xfs_icache.h"
#include "xfs_health.h"
#include "xfs_trans.h"

/*
 * Bulk Stat
 * =========
 *
 * Use the inode walking functions to fill out struct xfs_bulkstat for every
 * allocated inode, then pass the stat information to some externally provided
 * iteration function.
 */

struct xfs_bstat_chunk {
	bulkstat_one_fmt_pf	formatter;
	struct xfs_ibulk	*breq;
	struct xfs_bulkstat	*buf;
};

/*
 * Fill out the bulkstat info for a single inode and report it somewhere.
 *
 * bc->breq->lastino is effectively the inode cursor as we walk through the
 * filesystem.  Therefore, we update it any time we need to move the cursor
 * forward, regardless of whether or not we're sending any bstat information
 * back to userspace.  If the inode is internal metadata or, has been freed
 * out from under us, we just simply keep going.
 *
 * However, if any other type of error happens we want to stop right where we
 * are so that userspace will call back with exact number of the bad inode and
 * we can send back an error code.
 *
 * Note that if the formatter tells us there's no space left in the buffer we
 * move the cursor forward and abort the walk.
 */
STATIC int
xfs_bulkstat_one_int(
	struct xfs_mount	*mp,
	struct mnt_idmap	*idmap,
	struct xfs_trans	*tp,
	xfs_ino_t		ino,
	struct xfs_bstat_chunk	*bc)
{
	struct user_namespace	*sb_userns = mp->m_super->s_user_ns;
	struct xfs_inode	*ip;		/* incore inode pointer */
	struct inode		*inode;
	struct xfs_bulkstat	*buf = bc->buf;
	xfs_extnum_t		nextents;
	int			error = -EINVAL;
	vfsuid_t		vfsuid;
	vfsgid_t		vfsgid;

	if (xfs_internal_inum(mp, ino))
		goto out_advance;

	error = xfs_iget(mp, tp, ino,
			 (XFS_IGET_DONTCACHE | XFS_IGET_UNTRUSTED),
			 XFS_ILOCK_SHARED, &ip);
	if (error == -ENOENT || error == -EINVAL)
		goto out_advance;
	if (error)
		goto out;

	/* Reload the incore unlinked list to avoid failure in inodegc. */
	if (xfs_inode_unlinked_incomplete(ip)) {
		error = xfs_inode_reload_unlinked_bucket(tp, ip);
		if (error) {
			xfs_iunlock(ip, XFS_ILOCK_SHARED);
			xfs_force_shutdown(mp, SHUTDOWN_CORRUPT_INCORE);
			xfs_irele(ip);
			return error;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	ASSERT(ip != NULL);
	ASSERT(ip->i_imap.im_blkno != 0);
<<<<<<< HEAD

	dic = &ip->i_d;
=======
	inode = VFS_I(ip);
	vfsuid = i_uid_into_vfsuid(idmap, inode);
	vfsgid = i_gid_into_vfsgid(idmap, inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* xfs_iget returns the following without needing
	 * further change.
	 */
<<<<<<< HEAD
	buf->bs_nlink = dic->di_nlink;
	buf->bs_projid_lo = dic->di_projid_lo;
	buf->bs_projid_hi = dic->di_projid_hi;
	buf->bs_ino = ino;
	buf->bs_mode = dic->di_mode;
	buf->bs_uid = dic->di_uid;
	buf->bs_gid = dic->di_gid;
	buf->bs_size = dic->di_size;
	buf->bs_atime.tv_sec = dic->di_atime.t_sec;
	buf->bs_atime.tv_nsec = dic->di_atime.t_nsec;
	buf->bs_mtime.tv_sec = dic->di_mtime.t_sec;
	buf->bs_mtime.tv_nsec = dic->di_mtime.t_nsec;
	buf->bs_ctime.tv_sec = dic->di_ctime.t_sec;
	buf->bs_ctime.tv_nsec = dic->di_ctime.t_nsec;
	buf->bs_xflags = xfs_ip2xflags(ip);
	buf->bs_extsize = dic->di_extsize << mp->m_sb.sb_blocklog;
	buf->bs_extents = dic->di_nextents;
	buf->bs_gen = dic->di_gen;
	memset(buf->bs_pad, 0, sizeof(buf->bs_pad));
	buf->bs_dmevmask = dic->di_dmevmask;
	buf->bs_dmstate = dic->di_dmstate;
	buf->bs_aextents = dic->di_anextents;
	buf->bs_forkoff = XFS_IFORK_BOFF(ip);

	switch (dic->di_format) {
	case XFS_DINODE_FMT_DEV:
		buf->bs_rdev = ip->i_df.if_u2.if_rdev;
=======
	buf->bs_projectid = ip->i_projid;
	buf->bs_ino = ino;
	buf->bs_uid = from_kuid(sb_userns, vfsuid_into_kuid(vfsuid));
	buf->bs_gid = from_kgid(sb_userns, vfsgid_into_kgid(vfsgid));
	buf->bs_size = ip->i_disk_size;

	buf->bs_nlink = inode->i_nlink;
	buf->bs_atime = inode_get_atime_sec(inode);
	buf->bs_atime_nsec = inode_get_atime_nsec(inode);
	buf->bs_mtime = inode_get_mtime_sec(inode);
	buf->bs_mtime_nsec = inode_get_mtime_nsec(inode);
	buf->bs_ctime = inode_get_ctime_sec(inode);
	buf->bs_ctime_nsec = inode_get_ctime_nsec(inode);
	buf->bs_gen = inode->i_generation;
	buf->bs_mode = inode->i_mode;

	buf->bs_xflags = xfs_ip2xflags(ip);
	buf->bs_extsize_blks = ip->i_extsize;

	nextents = xfs_ifork_nextents(&ip->i_df);
	if (!(bc->breq->flags & XFS_IBULK_NREXT64))
		buf->bs_extents = min(nextents, XFS_MAX_EXTCNT_DATA_FORK_SMALL);
	else
		buf->bs_extents64 = nextents;

	xfs_bulkstat_health(ip, buf);
	buf->bs_aextents = xfs_ifork_nextents(&ip->i_af);
	buf->bs_forkoff = xfs_inode_fork_boff(ip);
	buf->bs_version = XFS_BULKSTAT_VERSION_V5;

	if (xfs_has_v3inodes(mp)) {
		buf->bs_btime = ip->i_crtime.tv_sec;
		buf->bs_btime_nsec = ip->i_crtime.tv_nsec;
		if (ip->i_diflags2 & XFS_DIFLAG2_COWEXTSIZE)
			buf->bs_cowextsize_blks = ip->i_cowextsize;
	}

	switch (ip->i_df.if_format) {
	case XFS_DINODE_FMT_DEV:
		buf->bs_rdev = sysv_encode_dev(inode->i_rdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		buf->bs_blksize = BLKDEV_IOSIZE;
		buf->bs_blocks = 0;
		break;
	case XFS_DINODE_FMT_LOCAL:
<<<<<<< HEAD
	case XFS_DINODE_FMT_UUID:
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		buf->bs_rdev = 0;
		buf->bs_blksize = mp->m_sb.sb_blocksize;
		buf->bs_blocks = 0;
		break;
	case XFS_DINODE_FMT_EXTENTS:
	case XFS_DINODE_FMT_BTREE:
		buf->bs_rdev = 0;
		buf->bs_blksize = mp->m_sb.sb_blocksize;
<<<<<<< HEAD
		buf->bs_blocks = dic->di_nblocks + ip->i_delayed_blks;
		break;
	}
	xfs_iunlock(ip, XFS_ILOCK_SHARED);
	IRELE(ip);

	error = formatter(buffer, ubsize, ubused, buf);

	if (!error)
		*stat = BULKSTAT_RV_DIDONE;

 out_free:
	kmem_free(buf);
	return error;
}

/* Return 0 on success or positive error */
STATIC int
xfs_bulkstat_one_fmt(
	void			__user *ubuffer,
	int			ubsize,
	int			*ubused,
	const xfs_bstat_t	*buffer)
{
	if (ubsize < sizeof(*buffer))
		return XFS_ERROR(ENOMEM);
	if (copy_to_user(ubuffer, buffer, sizeof(*buffer)))
		return XFS_ERROR(EFAULT);
	if (ubused)
		*ubused = sizeof(*buffer);
	return 0;
}

int
xfs_bulkstat_one(
	xfs_mount_t	*mp,		/* mount point for filesystem */
	xfs_ino_t	ino,		/* inode number to get data for */
	void		__user *buffer,	/* buffer to place output in */
	int		ubsize,		/* size of buffer */
	int		*ubused,	/* bytes used by me */
	int		*stat)		/* BULKSTAT_RV_... */
{
	return xfs_bulkstat_one_int(mp, ino, buffer, ubsize,
				    xfs_bulkstat_one_fmt, ubused, stat);
}

#define XFS_BULKSTAT_UBLEFT(ubleft)	((ubleft) >= statstruct_size)

/*
 * Return stat information in bulk (by-inode) for the filesystem.
 */
int					/* error status */
xfs_bulkstat(
	xfs_mount_t		*mp,	/* mount point for filesystem */
	xfs_ino_t		*lastinop, /* last inode returned */
	int			*ubcountp, /* size of buffer/count returned */
	bulkstat_one_pf		formatter, /* func that'd fill a single buf */
	size_t			statstruct_size, /* sizeof struct filling */
	char			__user *ubuffer, /* buffer with inode stats */
	int			*done)	/* 1 if there are more stats to get */
{
	xfs_agblock_t		agbno=0;/* allocation group block number */
	xfs_buf_t		*agbp;	/* agi header buffer */
	xfs_agi_t		*agi;	/* agi header data */
	xfs_agino_t		agino;	/* inode # in allocation group */
	xfs_agnumber_t		agno;	/* allocation group number */
	int			chunkidx; /* current index into inode chunk */
	int			clustidx; /* current index into inode cluster */
	xfs_btree_cur_t		*cur;	/* btree cursor for ialloc btree */
	int			end_of_ag; /* set if we've seen the ag end */
	int			error;	/* error code */
	int                     fmterror;/* bulkstat formatter result */
	int			i;	/* loop index */
	int			icount;	/* count of inodes good in irbuf */
	size_t			irbsize; /* size of irec buffer in bytes */
	xfs_ino_t		ino;	/* inode number (filesystem) */
	xfs_inobt_rec_incore_t	*irbp;	/* current irec buffer pointer */
	xfs_inobt_rec_incore_t	*irbuf;	/* start of irec buffer */
	xfs_inobt_rec_incore_t	*irbufend; /* end of good irec buffer entries */
	xfs_ino_t		lastino; /* last inode number returned */
	int			nbcluster; /* # of blocks in a cluster */
	int			nicluster; /* # of inodes in a cluster */
	int			nimask;	/* mask for inode clusters */
	int			nirbuf;	/* size of irbuf */
	int			rval;	/* return value error code */
	int			tmp;	/* result value from btree calls */
	int			ubcount; /* size of user's buffer */
	int			ubleft;	/* bytes left in user's buffer */
	char			__user *ubufp;	/* pointer into user's buffer */
	int			ubelem;	/* spaces used in user's buffer */
	int			ubused;	/* bytes used by formatter */
	xfs_buf_t		*bp;	/* ptr to on-disk inode cluster buf */

	/*
	 * Get the last inode value, see if there's nothing to do.
	 */
	ino = (xfs_ino_t)*lastinop;
	lastino = ino;
	agno = XFS_INO_TO_AGNO(mp, ino);
	agino = XFS_INO_TO_AGINO(mp, ino);
	if (agno >= mp->m_sb.sb_agcount ||
	    ino != XFS_AGINO_TO_INO(mp, agno, agino)) {
		*done = 1;
		*ubcountp = 0;
		return 0;
	}
	if (!ubcountp || *ubcountp <= 0) {
		return EINVAL;
	}
	ubcount = *ubcountp; /* statstruct's */
	ubleft = ubcount * statstruct_size; /* bytes */
	*ubcountp = ubelem = 0;
	*done = 0;
	fmterror = 0;
	ubufp = ubuffer;
	nicluster = mp->m_sb.sb_blocksize >= XFS_INODE_CLUSTER_SIZE(mp) ?
		mp->m_sb.sb_inopblock :
		(XFS_INODE_CLUSTER_SIZE(mp) >> mp->m_sb.sb_inodelog);
	nimask = ~(nicluster - 1);
	nbcluster = nicluster >> mp->m_sb.sb_inopblog;
	irbuf = kmem_zalloc_greedy(&irbsize, PAGE_SIZE, PAGE_SIZE * 4);
	if (!irbuf)
		return ENOMEM;

	nirbuf = irbsize / sizeof(*irbuf);

	/*
	 * Loop over the allocation groups, starting from the last
	 * inode returned; 0 means start of the allocation group.
	 */
	rval = 0;
	while (XFS_BULKSTAT_UBLEFT(ubleft) && agno < mp->m_sb.sb_agcount) {
		cond_resched();
		bp = NULL;
		error = xfs_ialloc_read_agi(mp, NULL, agno, &agbp);
		if (error) {
			/*
			 * Skip this allocation group and go to the next one.
			 */
			agno++;
			agino = 0;
			continue;
		}
		agi = XFS_BUF_TO_AGI(agbp);
		/*
		 * Allocate and initialize a btree cursor for ialloc btree.
		 */
		cur = xfs_inobt_init_cursor(mp, NULL, agbp, agno);
		irbp = irbuf;
		irbufend = irbuf + nirbuf;
		end_of_ag = 0;
		/*
		 * If we're returning in the middle of an allocation group,
		 * we need to get the remainder of the chunk we're in.
		 */
		if (agino > 0) {
			xfs_inobt_rec_incore_t r;

			/*
			 * Lookup the inode chunk that this inode lives in.
			 */
			error = xfs_inobt_lookup(cur, agino, XFS_LOOKUP_LE,
						 &tmp);
			if (!error &&	/* no I/O error */
			    tmp &&	/* lookup succeeded */
					/* got the record, should always work */
			    !(error = xfs_inobt_get_rec(cur, &r, &i)) &&
			    i == 1 &&
					/* this is the right chunk */
			    agino < r.ir_startino + XFS_INODES_PER_CHUNK &&
					/* lastino was not last in chunk */
			    (chunkidx = agino - r.ir_startino + 1) <
				    XFS_INODES_PER_CHUNK &&
					/* there are some left allocated */
			    xfs_inobt_maskn(chunkidx,
				    XFS_INODES_PER_CHUNK - chunkidx) &
				    ~r.ir_free) {
				/*
				 * Grab the chunk record.  Mark all the
				 * uninteresting inodes (because they're
				 * before our start point) free.
				 */
				for (i = 0; i < chunkidx; i++) {
					if (XFS_INOBT_MASK(i) & ~r.ir_free)
						r.ir_freecount++;
				}
				r.ir_free |= xfs_inobt_maskn(0, chunkidx);
				irbp->ir_startino = r.ir_startino;
				irbp->ir_freecount = r.ir_freecount;
				irbp->ir_free = r.ir_free;
				irbp++;
				agino = r.ir_startino + XFS_INODES_PER_CHUNK;
				icount = XFS_INODES_PER_CHUNK - r.ir_freecount;
			} else {
				/*
				 * If any of those tests failed, bump the
				 * inode number (just in case).
				 */
				agino++;
				icount = 0;
			}
			/*
			 * In any case, increment to the next record.
			 */
			if (!error)
				error = xfs_btree_increment(cur, 0, &tmp);
		} else {
			/*
			 * Start of ag.  Lookup the first inode chunk.
			 */
			error = xfs_inobt_lookup(cur, 0, XFS_LOOKUP_GE, &tmp);
			icount = 0;
		}
		/*
		 * Loop through inode btree records in this ag,
		 * until we run out of inodes or space in the buffer.
		 */
		while (irbp < irbufend && icount < ubcount) {
			xfs_inobt_rec_incore_t r;

			/*
			 * Loop as long as we're unable to read the
			 * inode btree.
			 */
			while (error) {
				agino += XFS_INODES_PER_CHUNK;
				if (XFS_AGINO_TO_AGBNO(mp, agino) >=
						be32_to_cpu(agi->agi_length))
					break;
				error = xfs_inobt_lookup(cur, agino,
							 XFS_LOOKUP_GE, &tmp);
				cond_resched();
			}
			/*
			 * If ran off the end of the ag either with an error,
			 * or the normal way, set end and stop collecting.
			 */
			if (error) {
				end_of_ag = 1;
				break;
			}

			error = xfs_inobt_get_rec(cur, &r, &i);
			if (error || i == 0) {
				end_of_ag = 1;
				break;
			}

			/*
			 * If this chunk has any allocated inodes, save it.
			 * Also start read-ahead now for this chunk.
			 */
			if (r.ir_freecount < XFS_INODES_PER_CHUNK) {
				/*
				 * Loop over all clusters in the next chunk.
				 * Do a readahead if there are any allocated
				 * inodes in that cluster.
				 */
				agbno = XFS_AGINO_TO_AGBNO(mp, r.ir_startino);
				for (chunkidx = 0;
				     chunkidx < XFS_INODES_PER_CHUNK;
				     chunkidx += nicluster,
				     agbno += nbcluster) {
					if (xfs_inobt_maskn(chunkidx, nicluster)
							& ~r.ir_free)
						xfs_btree_reada_bufs(mp, agno,
							agbno, nbcluster);
				}
				irbp->ir_startino = r.ir_startino;
				irbp->ir_freecount = r.ir_freecount;
				irbp->ir_free = r.ir_free;
				irbp++;
				icount += XFS_INODES_PER_CHUNK - r.ir_freecount;
			}
			/*
			 * Set agino to after this chunk and bump the cursor.
			 */
			agino = r.ir_startino + XFS_INODES_PER_CHUNK;
			error = xfs_btree_increment(cur, 0, &tmp);
			cond_resched();
		}
		/*
		 * Drop the btree buffers and the agi buffer.
		 * We can't hold any of the locks these represent
		 * when calling iget.
		 */
		xfs_btree_del_cursor(cur, XFS_BTREE_NOERROR);
		xfs_buf_relse(agbp);
		/*
		 * Now format all the good inodes into the user's buffer.
		 */
		irbufend = irbp;
		for (irbp = irbuf;
		     irbp < irbufend && XFS_BULKSTAT_UBLEFT(ubleft); irbp++) {
			/*
			 * Now process this chunk of inodes.
			 */
			for (agino = irbp->ir_startino, chunkidx = clustidx = 0;
			     XFS_BULKSTAT_UBLEFT(ubleft) &&
				irbp->ir_freecount < XFS_INODES_PER_CHUNK;
			     chunkidx++, clustidx++, agino++) {
				ASSERT(chunkidx < XFS_INODES_PER_CHUNK);
				/*
				 * Recompute agbno if this is the
				 * first inode of the cluster.
				 *
				 * Careful with clustidx.   There can be
				 * multiple clusters per chunk, a single
				 * cluster per chunk or a cluster that has
				 * inodes represented from several different
				 * chunks (if blocksize is large).
				 *
				 * Because of this, the starting clustidx is
				 * initialized to zero in this loop but must
				 * later be reset after reading in the cluster
				 * buffer.
				 */
				if ((chunkidx & (nicluster - 1)) == 0) {
					agbno = XFS_AGINO_TO_AGBNO(mp,
							irbp->ir_startino) +
						((chunkidx & nimask) >>
						 mp->m_sb.sb_inopblog);
				}
				ino = XFS_AGINO_TO_INO(mp, agno, agino);
				/*
				 * Skip if this inode is free.
				 */
				if (XFS_INOBT_MASK(chunkidx) & irbp->ir_free) {
					lastino = ino;
					continue;
				}
				/*
				 * Count used inodes as free so we can tell
				 * when the chunk is used up.
				 */
				irbp->ir_freecount++;

				/*
				 * Get the inode and fill in a single buffer.
				 */
				ubused = statstruct_size;
				error = formatter(mp, ino, ubufp, ubleft,
						  &ubused, &fmterror);
				if (fmterror == BULKSTAT_RV_NOTHING) {
					if (error && error != ENOENT &&
						error != EINVAL) {
						ubleft = 0;
						rval = error;
						break;
					}
					lastino = ino;
					continue;
				}
				if (fmterror == BULKSTAT_RV_GIVEUP) {
					ubleft = 0;
					ASSERT(error);
					rval = error;
					break;
				}
				if (ubufp)
					ubufp += ubused;
				ubleft -= ubused;
				ubelem++;
				lastino = ino;
			}

			cond_resched();
		}

		if (bp)
			xfs_buf_relse(bp);

		/*
		 * Set up for the next loop iteration.
		 */
		if (XFS_BULKSTAT_UBLEFT(ubleft)) {
			if (end_of_ag) {
				agno++;
				agino = 0;
			} else
				agino = XFS_INO_TO_AGINO(mp, lastino);
		} else
			break;
	}
	/*
	 * Done, we're either out of filesystem or space to put the data.
	 */
	kmem_free_large(irbuf);
	*ubcountp = ubelem;
	/*
	 * Found some inodes, return them now and return the error next time.
	 */
	if (ubelem)
		rval = 0;
	if (agno >= mp->m_sb.sb_agcount) {
		/*
		 * If we ran out of filesystem, mark lastino as off
		 * the end of the filesystem, so the next call
		 * will return immediately.
		 */
		*lastinop = (xfs_ino_t)XFS_AGINO_TO_INO(mp, agno, 0);
		*done = 1;
	} else
		*lastinop = (xfs_ino_t)lastino;

	return rval;
}

/*
 * Return stat information in bulk (by-inode) for the filesystem.
 * Special case for non-sequential one inode bulkstat.
 */
int					/* error status */
xfs_bulkstat_single(
	xfs_mount_t		*mp,	/* mount point for filesystem */
	xfs_ino_t		*lastinop, /* inode to return */
	char			__user *buffer, /* buffer with inode stats */
	int			*done)	/* 1 if there are more stats to get */
{
	int			count;	/* count value for bulkstat call */
	int			error;	/* return value */
	xfs_ino_t		ino;	/* filesystem inode number */
	int			res;	/* result from bs1 */

	/*
	 * note that requesting valid inode numbers which are not allocated
	 * to inodes will most likely cause xfs_itobp to generate warning
	 * messages about bad magic numbers. This is ok. The fact that
	 * the inode isn't actually an inode is handled by the
	 * error check below. Done this way to make the usual case faster
	 * at the expense of the error case.
	 */

	ino = (xfs_ino_t)*lastinop;
	error = xfs_bulkstat_one(mp, ino, buffer, sizeof(xfs_bstat_t), 0, &res);
	if (error) {
		/*
		 * Special case way failed, do it the "long" way
		 * to see if that works.
		 */
		(*lastinop)--;
		count = 1;
		if (xfs_bulkstat(mp, lastinop, &count, xfs_bulkstat_one,
				sizeof(xfs_bstat_t), buffer, done))
			return error;
		if (count == 0 || (xfs_ino_t)*lastinop != ino)
			return error == EFSCORRUPTED ?
				XFS_ERROR(EINVAL) : error;
		else
			return 0;
	}
	*done = 0;
	return 0;
}

int
xfs_inumbers_fmt(
	void			__user *ubuffer, /* buffer to write to */
	const xfs_inogrp_t	*buffer,	/* buffer to read from */
	long			count,		/* # of elements to read */
	long			*written)	/* # of bytes written */
{
	if (copy_to_user(ubuffer, buffer, count * sizeof(*buffer)))
		return -EFAULT;
	*written = count * sizeof(*buffer);
	return 0;
=======
		buf->bs_blocks = ip->i_nblocks + ip->i_delayed_blks;
		break;
	}
	xfs_iunlock(ip, XFS_ILOCK_SHARED);
	xfs_irele(ip);

	error = bc->formatter(bc->breq, buf);
	if (error == -ECANCELED)
		goto out_advance;
	if (error)
		goto out;

out_advance:
	/*
	 * Advance the cursor to the inode that comes after the one we just
	 * looked at.  We want the caller to move along if the bulkstat
	 * information was copied successfully; if we tried to grab the inode
	 * but it's no longer allocated; or if it's internal metadata.
	 */
	bc->breq->startino = ino + 1;
out:
	return error;
}

/* Bulkstat a single inode. */
int
xfs_bulkstat_one(
	struct xfs_ibulk	*breq,
	bulkstat_one_fmt_pf	formatter)
{
	struct xfs_bstat_chunk	bc = {
		.formatter	= formatter,
		.breq		= breq,
	};
	struct xfs_trans	*tp;
	int			error;

	if (breq->idmap != &nop_mnt_idmap) {
		xfs_warn_ratelimited(breq->mp,
			"bulkstat not supported inside of idmapped mounts.");
		return -EINVAL;
	}

	ASSERT(breq->icount == 1);

	bc.buf = kzalloc(sizeof(struct xfs_bulkstat),
			GFP_KERNEL | __GFP_RETRY_MAYFAIL);
	if (!bc.buf)
		return -ENOMEM;

	/*
	 * Grab an empty transaction so that we can use its recursive buffer
	 * locking abilities to detect cycles in the inobt without deadlocking.
	 */
	error = xfs_trans_alloc_empty(breq->mp, &tp);
	if (error)
		goto out;

	error = xfs_bulkstat_one_int(breq->mp, breq->idmap, tp,
			breq->startino, &bc);
	xfs_trans_cancel(tp);
out:
	kfree(bc.buf);

	/*
	 * If we reported one inode to userspace then we abort because we hit
	 * the end of the buffer.  Don't leak that back to userspace.
	 */
	if (error == -ECANCELED)
		error = 0;

	return error;
}

static int
xfs_bulkstat_iwalk(
	struct xfs_mount	*mp,
	struct xfs_trans	*tp,
	xfs_ino_t		ino,
	void			*data)
{
	struct xfs_bstat_chunk	*bc = data;
	int			error;

	error = xfs_bulkstat_one_int(mp, bc->breq->idmap, tp, ino, data);
	/* bulkstat just skips over missing inodes */
	if (error == -ENOENT || error == -EINVAL)
		return 0;
	return error;
}

/*
 * Check the incoming lastino parameter.
 *
 * We allow any inode value that could map to physical space inside the
 * filesystem because if there are no inodes there, bulkstat moves on to the
 * next chunk.  In other words, the magic agino value of zero takes us to the
 * first chunk in the AG, and an agino value past the end of the AG takes us to
 * the first chunk in the next AG.
 *
 * Therefore we can end early if the requested inode is beyond the end of the
 * filesystem or doesn't map properly.
 */
static inline bool
xfs_bulkstat_already_done(
	struct xfs_mount	*mp,
	xfs_ino_t		startino)
{
	xfs_agnumber_t		agno = XFS_INO_TO_AGNO(mp, startino);
	xfs_agino_t		agino = XFS_INO_TO_AGINO(mp, startino);

	return agno >= mp->m_sb.sb_agcount ||
	       startino != XFS_AGINO_TO_INO(mp, agno, agino);
}

/* Return stat information in bulk (by-inode) for the filesystem. */
int
xfs_bulkstat(
	struct xfs_ibulk	*breq,
	bulkstat_one_fmt_pf	formatter)
{
	struct xfs_bstat_chunk	bc = {
		.formatter	= formatter,
		.breq		= breq,
	};
	struct xfs_trans	*tp;
	unsigned int		iwalk_flags = 0;
	int			error;

	if (breq->idmap != &nop_mnt_idmap) {
		xfs_warn_ratelimited(breq->mp,
			"bulkstat not supported inside of idmapped mounts.");
		return -EINVAL;
	}
	if (xfs_bulkstat_already_done(breq->mp, breq->startino))
		return 0;

	bc.buf = kzalloc(sizeof(struct xfs_bulkstat),
			GFP_KERNEL | __GFP_RETRY_MAYFAIL);
	if (!bc.buf)
		return -ENOMEM;

	/*
	 * Grab an empty transaction so that we can use its recursive buffer
	 * locking abilities to detect cycles in the inobt without deadlocking.
	 */
	error = xfs_trans_alloc_empty(breq->mp, &tp);
	if (error)
		goto out;

	if (breq->flags & XFS_IBULK_SAME_AG)
		iwalk_flags |= XFS_IWALK_SAME_AG;

	error = xfs_iwalk(breq->mp, tp, breq->startino, iwalk_flags,
			xfs_bulkstat_iwalk, breq->icount, &bc);
	xfs_trans_cancel(tp);
out:
	kfree(bc.buf);

	/*
	 * We found some inodes, so clear the error status and return them.
	 * The lastino pointer will point directly at the inode that triggered
	 * any error that occurred, so on the next call the error will be
	 * triggered again and propagated to userspace as there will be no
	 * formatted inodes in the buffer.
	 */
	if (breq->ocount > 0)
		error = 0;

	return error;
}

/* Convert bulkstat (v5) to bstat (v1). */
void
xfs_bulkstat_to_bstat(
	struct xfs_mount		*mp,
	struct xfs_bstat		*bs1,
	const struct xfs_bulkstat	*bstat)
{
	/* memset is needed here because of padding holes in the structure. */
	memset(bs1, 0, sizeof(struct xfs_bstat));
	bs1->bs_ino = bstat->bs_ino;
	bs1->bs_mode = bstat->bs_mode;
	bs1->bs_nlink = bstat->bs_nlink;
	bs1->bs_uid = bstat->bs_uid;
	bs1->bs_gid = bstat->bs_gid;
	bs1->bs_rdev = bstat->bs_rdev;
	bs1->bs_blksize = bstat->bs_blksize;
	bs1->bs_size = bstat->bs_size;
	bs1->bs_atime.tv_sec = bstat->bs_atime;
	bs1->bs_mtime.tv_sec = bstat->bs_mtime;
	bs1->bs_ctime.tv_sec = bstat->bs_ctime;
	bs1->bs_atime.tv_nsec = bstat->bs_atime_nsec;
	bs1->bs_mtime.tv_nsec = bstat->bs_mtime_nsec;
	bs1->bs_ctime.tv_nsec = bstat->bs_ctime_nsec;
	bs1->bs_blocks = bstat->bs_blocks;
	bs1->bs_xflags = bstat->bs_xflags;
	bs1->bs_extsize = XFS_FSB_TO_B(mp, bstat->bs_extsize_blks);
	bs1->bs_extents = bstat->bs_extents;
	bs1->bs_gen = bstat->bs_gen;
	bs1->bs_projid_lo = bstat->bs_projectid & 0xFFFF;
	bs1->bs_forkoff = bstat->bs_forkoff;
	bs1->bs_projid_hi = bstat->bs_projectid >> 16;
	bs1->bs_sick = bstat->bs_sick;
	bs1->bs_checked = bstat->bs_checked;
	bs1->bs_cowextsize = XFS_FSB_TO_B(mp, bstat->bs_cowextsize_blks);
	bs1->bs_dmevmask = 0;
	bs1->bs_dmstate = 0;
	bs1->bs_aextents = bstat->bs_aextents;
}

struct xfs_inumbers_chunk {
	inumbers_fmt_pf		formatter;
	struct xfs_ibulk	*breq;
};

/*
 * INUMBERS
 * ========
 * This is how we export inode btree records to userspace, so that XFS tools
 * can figure out where inodes are allocated.
 */

/*
 * Format the inode group structure and report it somewhere.
 *
 * Similar to xfs_bulkstat_one_int, lastino is the inode cursor as we walk
 * through the filesystem so we move it forward unless there was a runtime
 * error.  If the formatter tells us the buffer is now full we also move the
 * cursor forward and abort the walk.
 */
STATIC int
xfs_inumbers_walk(
	struct xfs_mount	*mp,
	struct xfs_trans	*tp,
	xfs_agnumber_t		agno,
	const struct xfs_inobt_rec_incore *irec,
	void			*data)
{
	struct xfs_inumbers	inogrp = {
		.xi_startino	= XFS_AGINO_TO_INO(mp, agno, irec->ir_startino),
		.xi_alloccount	= irec->ir_count - irec->ir_freecount,
		.xi_allocmask	= ~irec->ir_free,
		.xi_version	= XFS_INUMBERS_VERSION_V5,
	};
	struct xfs_inumbers_chunk *ic = data;
	int			error;

	error = ic->formatter(ic->breq, &inogrp);
	if (error && error != -ECANCELED)
		return error;

	ic->breq->startino = XFS_AGINO_TO_INO(mp, agno, irec->ir_startino) +
			XFS_INODES_PER_CHUNK;
	return error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Return inode number table for the filesystem.
 */
<<<<<<< HEAD
int					/* error status */
xfs_inumbers(
	xfs_mount_t	*mp,		/* mount point for filesystem */
	xfs_ino_t	*lastino,	/* last inode returned */
	int		*count,		/* size of buffer/count returned */
	void		__user *ubuffer,/* buffer with inode descriptions */
	inumbers_fmt_pf	formatter)
{
	xfs_buf_t	*agbp;
	xfs_agino_t	agino;
	xfs_agnumber_t	agno;
	int		bcount;
	xfs_inogrp_t	*buffer;
	int		bufidx;
	xfs_btree_cur_t	*cur;
	int		error;
	xfs_inobt_rec_incore_t r;
	int		i;
	xfs_ino_t	ino;
	int		left;
	int		tmp;

	ino = (xfs_ino_t)*lastino;
	agno = XFS_INO_TO_AGNO(mp, ino);
	agino = XFS_INO_TO_AGINO(mp, ino);
	left = *count;
	*count = 0;
	bcount = MIN(left, (int)(PAGE_SIZE / sizeof(*buffer)));
	buffer = kmem_alloc(bcount * sizeof(*buffer), KM_SLEEP);
	error = bufidx = 0;
	cur = NULL;
	agbp = NULL;
	while (left > 0 && agno < mp->m_sb.sb_agcount) {
		if (agbp == NULL) {
			error = xfs_ialloc_read_agi(mp, NULL, agno, &agbp);
			if (error) {
				/*
				 * If we can't read the AGI of this ag,
				 * then just skip to the next one.
				 */
				ASSERT(cur == NULL);
				agbp = NULL;
				agno++;
				agino = 0;
				continue;
			}
			cur = xfs_inobt_init_cursor(mp, NULL, agbp, agno);
			error = xfs_inobt_lookup(cur, agino, XFS_LOOKUP_GE,
						 &tmp);
			if (error) {
				xfs_btree_del_cursor(cur, XFS_BTREE_ERROR);
				cur = NULL;
				xfs_buf_relse(agbp);
				agbp = NULL;
				/*
				 * Move up the last inode in the current
				 * chunk.  The lookup_ge will always get
				 * us the first inode in the next chunk.
				 */
				agino += XFS_INODES_PER_CHUNK - 1;
				continue;
			}
		}
		error = xfs_inobt_get_rec(cur, &r, &i);
		if (error || i == 0) {
			xfs_buf_relse(agbp);
			agbp = NULL;
			xfs_btree_del_cursor(cur, XFS_BTREE_NOERROR);
			cur = NULL;
			agno++;
			agino = 0;
			continue;
		}
		agino = r.ir_startino + XFS_INODES_PER_CHUNK - 1;
		buffer[bufidx].xi_startino =
			XFS_AGINO_TO_INO(mp, agno, r.ir_startino);
		buffer[bufidx].xi_alloccount =
			XFS_INODES_PER_CHUNK - r.ir_freecount;
		buffer[bufidx].xi_allocmask = ~r.ir_free;
		bufidx++;
		left--;
		if (bufidx == bcount) {
			long written;
			if (formatter(ubuffer, buffer, bufidx, &written)) {
				error = XFS_ERROR(EFAULT);
				break;
			}
			ubuffer += written;
			*count += bufidx;
			bufidx = 0;
		}
		if (left) {
			error = xfs_btree_increment(cur, 0, &tmp);
			if (error) {
				xfs_btree_del_cursor(cur, XFS_BTREE_ERROR);
				cur = NULL;
				xfs_buf_relse(agbp);
				agbp = NULL;
				/*
				 * The agino value has already been bumped.
				 * Just try to skip up to it.
				 */
				agino += XFS_INODES_PER_CHUNK;
				continue;
			}
		}
	}
	if (!error) {
		if (bufidx) {
			long written;
			if (formatter(ubuffer, buffer, bufidx, &written))
				error = XFS_ERROR(EFAULT);
			else
				*count += bufidx;
		}
		*lastino = XFS_AGINO_TO_INO(mp, agno, agino);
	}
	kmem_free(buffer);
	if (cur)
		xfs_btree_del_cursor(cur, (error ? XFS_BTREE_ERROR :
					   XFS_BTREE_NOERROR));
	if (agbp)
		xfs_buf_relse(agbp);
	return error;
}
=======
int
xfs_inumbers(
	struct xfs_ibulk	*breq,
	inumbers_fmt_pf		formatter)
{
	struct xfs_inumbers_chunk ic = {
		.formatter	= formatter,
		.breq		= breq,
	};
	struct xfs_trans	*tp;
	int			error = 0;

	if (xfs_bulkstat_already_done(breq->mp, breq->startino))
		return 0;

	/*
	 * Grab an empty transaction so that we can use its recursive buffer
	 * locking abilities to detect cycles in the inobt without deadlocking.
	 */
	error = xfs_trans_alloc_empty(breq->mp, &tp);
	if (error)
		goto out;

	error = xfs_inobt_walk(breq->mp, tp, breq->startino, breq->flags,
			xfs_inumbers_walk, breq->icount, &ic);
	xfs_trans_cancel(tp);
out:

	/*
	 * We found some inode groups, so clear the error status and return
	 * them.  The lastino pointer will point directly at the inode that
	 * triggered any error that occurred, so on the next call the error
	 * will be triggered again and propagated to userspace as there will be
	 * no formatted inode groups in the buffer.
	 */
	if (breq->ocount > 0)
		error = 0;

	return error;
}

/* Convert an inumbers (v5) struct to a inogrp (v1) struct. */
void
xfs_inumbers_to_inogrp(
	struct xfs_inogrp		*ig1,
	const struct xfs_inumbers	*ig)
{
	/* memset is needed here because of padding holes in the structure. */
	memset(ig1, 0, sizeof(struct xfs_inogrp));
	ig1->xi_startino = ig->xi_startino;
	ig1->xi_alloccount = ig->xi_alloccount;
	ig1->xi_allocmask = ig->xi_allocmask;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
