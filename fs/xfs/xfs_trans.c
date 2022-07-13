<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Copyright (c) 2000-2003,2005 Silicon Graphics, Inc.
 * Copyright (C) 2010 Red Hat, Inc.
 * All Rights Reserved.
<<<<<<< HEAD
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
#include "xfs_error.h"
#include "xfs_da_btree.h"
#include "xfs_bmap_btree.h"
#include "xfs_alloc_btree.h"
#include "xfs_ialloc_btree.h"
#include "xfs_dinode.h"
#include "xfs_inode.h"
#include "xfs_btree.h"
#include "xfs_ialloc.h"
#include "xfs_alloc.h"
#include "xfs_bmap.h"
#include "xfs_quota.h"
#include "xfs_trans_priv.h"
#include "xfs_trans_space.h"
#include "xfs_inode_item.h"
#include "xfs_trace.h"

kmem_zone_t	*xfs_trans_zone;
kmem_zone_t	*xfs_log_item_desc_zone;


/*
 * Various log reservation values.
 *
 * These are based on the size of the file system block because that is what
 * most transactions manipulate.  Each adds in an additional 128 bytes per
 * item logged to try to account for the overhead of the transaction mechanism.
 *
 * Note:  Most of the reservations underestimate the number of allocation
 * groups into which they could free extents in the xfs_bmap_finish() call.
 * This is because the number in the worst case is quite high and quite
 * unusual.  In order to fix this we need to change xfs_bmap_finish() to free
 * extents in only a single AG at a time.  This will require changes to the
 * EFI code as well, however, so that the EFI for the extents not freed is
 * logged again in each transaction.  See SGI PV #261917.
 *
 * Reservation functions here avoid a huge stack in xfs_trans_init due to
 * register overflow from temporaries in the calculations.
 */


/*
 * In a write transaction we can allocate a maximum of 2
 * extents.  This gives:
 *    the inode getting the new extents: inode size
 *    the inode's bmap btree: max depth * block size
 *    the agfs of the ags from which the extents are allocated: 2 * sector
 *    the superblock free block counter: sector size
 *    the allocation btrees: 2 exts * 2 trees * (2 * max depth - 1) * block size
 * And the bmap_finish transaction can free bmap blocks in a join:
 *    the agfs of the ags containing the blocks: 2 * sector size
 *    the agfls of the ags containing the blocks: 2 * sector size
 *    the super block free block counter: sector size
 *    the allocation btrees: 2 exts * 2 trees * (2 * max depth - 1) * block size
 */
STATIC uint
xfs_calc_write_reservation(
	struct xfs_mount	*mp)
{
	return XFS_DQUOT_LOGRES(mp) +
		MAX((mp->m_sb.sb_inodesize +
		     XFS_FSB_TO_B(mp, XFS_BM_MAXLEVELS(mp, XFS_DATA_FORK)) +
		     2 * mp->m_sb.sb_sectsize +
		     mp->m_sb.sb_sectsize +
		     XFS_ALLOCFREE_LOG_RES(mp, 2) +
		     128 * (4 + XFS_BM_MAXLEVELS(mp, XFS_DATA_FORK) +
			    XFS_ALLOCFREE_LOG_COUNT(mp, 2))),
		    (2 * mp->m_sb.sb_sectsize +
		     2 * mp->m_sb.sb_sectsize +
		     mp->m_sb.sb_sectsize +
		     XFS_ALLOCFREE_LOG_RES(mp, 2) +
		     128 * (5 + XFS_ALLOCFREE_LOG_COUNT(mp, 2))));
}

/*
 * In truncating a file we free up to two extents at once.  We can modify:
 *    the inode being truncated: inode size
 *    the inode's bmap btree: (max depth + 1) * block size
 * And the bmap_finish transaction can free the blocks and bmap blocks:
 *    the agf for each of the ags: 4 * sector size
 *    the agfl for each of the ags: 4 * sector size
 *    the super block to reflect the freed blocks: sector size
 *    worst case split in allocation btrees per extent assuming 4 extents:
 *		4 exts * 2 trees * (2 * max depth - 1) * block size
 *    the inode btree: max depth * blocksize
 *    the allocation btrees: 2 trees * (max depth - 1) * block size
 */
STATIC uint
xfs_calc_itruncate_reservation(
	struct xfs_mount	*mp)
{
	return XFS_DQUOT_LOGRES(mp) +
		MAX((mp->m_sb.sb_inodesize +
		     XFS_FSB_TO_B(mp, XFS_BM_MAXLEVELS(mp, XFS_DATA_FORK) + 1) +
		     128 * (2 + XFS_BM_MAXLEVELS(mp, XFS_DATA_FORK))),
		    (4 * mp->m_sb.sb_sectsize +
		     4 * mp->m_sb.sb_sectsize +
		     mp->m_sb.sb_sectsize +
		     XFS_ALLOCFREE_LOG_RES(mp, 4) +
		     128 * (9 + XFS_ALLOCFREE_LOG_COUNT(mp, 4)) +
		     128 * 5 +
		     XFS_ALLOCFREE_LOG_RES(mp, 1) +
		     128 * (2 + XFS_IALLOC_BLOCKS(mp) + mp->m_in_maxlevels +
			    XFS_ALLOCFREE_LOG_COUNT(mp, 1))));
}

/*
 * In renaming a files we can modify:
 *    the four inodes involved: 4 * inode size
 *    the two directory btrees: 2 * (max depth + v2) * dir block size
 *    the two directory bmap btrees: 2 * max depth * block size
 * And the bmap_finish transaction can free dir and bmap blocks (two sets
 *	of bmap blocks) giving:
 *    the agf for the ags in which the blocks live: 3 * sector size
 *    the agfl for the ags in which the blocks live: 3 * sector size
 *    the superblock for the free block count: sector size
 *    the allocation btrees: 3 exts * 2 trees * (2 * max depth - 1) * block size
 */
STATIC uint
xfs_calc_rename_reservation(
	struct xfs_mount	*mp)
{
	return XFS_DQUOT_LOGRES(mp) +
		MAX((4 * mp->m_sb.sb_inodesize +
		     2 * XFS_DIROP_LOG_RES(mp) +
		     128 * (4 + 2 * XFS_DIROP_LOG_COUNT(mp))),
		    (3 * mp->m_sb.sb_sectsize +
		     3 * mp->m_sb.sb_sectsize +
		     mp->m_sb.sb_sectsize +
		     XFS_ALLOCFREE_LOG_RES(mp, 3) +
		     128 * (7 + XFS_ALLOCFREE_LOG_COUNT(mp, 3))));
}

/*
 * For creating a link to an inode:
 *    the parent directory inode: inode size
 *    the linked inode: inode size
 *    the directory btree could split: (max depth + v2) * dir block size
 *    the directory bmap btree could join or split: (max depth + v2) * blocksize
 * And the bmap_finish transaction can free some bmap blocks giving:
 *    the agf for the ag in which the blocks live: sector size
 *    the agfl for the ag in which the blocks live: sector size
 *    the superblock for the free block count: sector size
 *    the allocation btrees: 2 trees * (2 * max depth - 1) * block size
 */
STATIC uint
xfs_calc_link_reservation(
	struct xfs_mount	*mp)
{
	return XFS_DQUOT_LOGRES(mp) +
		MAX((mp->m_sb.sb_inodesize +
		     mp->m_sb.sb_inodesize +
		     XFS_DIROP_LOG_RES(mp) +
		     128 * (2 + XFS_DIROP_LOG_COUNT(mp))),
		    (mp->m_sb.sb_sectsize +
		     mp->m_sb.sb_sectsize +
		     mp->m_sb.sb_sectsize +
		     XFS_ALLOCFREE_LOG_RES(mp, 1) +
		     128 * (3 + XFS_ALLOCFREE_LOG_COUNT(mp, 1))));
}

/*
 * For removing a directory entry we can modify:
 *    the parent directory inode: inode size
 *    the removed inode: inode size
 *    the directory btree could join: (max depth + v2) * dir block size
 *    the directory bmap btree could join or split: (max depth + v2) * blocksize
 * And the bmap_finish transaction can free the dir and bmap blocks giving:
 *    the agf for the ag in which the blocks live: 2 * sector size
 *    the agfl for the ag in which the blocks live: 2 * sector size
 *    the superblock for the free block count: sector size
 *    the allocation btrees: 2 exts * 2 trees * (2 * max depth - 1) * block size
 */
STATIC uint
xfs_calc_remove_reservation(
	struct xfs_mount	*mp)
{
	return XFS_DQUOT_LOGRES(mp) +
		MAX((mp->m_sb.sb_inodesize +
		     mp->m_sb.sb_inodesize +
		     XFS_DIROP_LOG_RES(mp) +
		     128 * (2 + XFS_DIROP_LOG_COUNT(mp))),
		    (2 * mp->m_sb.sb_sectsize +
		     2 * mp->m_sb.sb_sectsize +
		     mp->m_sb.sb_sectsize +
		     XFS_ALLOCFREE_LOG_RES(mp, 2) +
		     128 * (5 + XFS_ALLOCFREE_LOG_COUNT(mp, 2))));
}

/*
 * For symlink we can modify:
 *    the parent directory inode: inode size
 *    the new inode: inode size
 *    the inode btree entry: 1 block
 *    the directory btree: (max depth + v2) * dir block size
 *    the directory inode's bmap btree: (max depth + v2) * block size
 *    the blocks for the symlink: 1 kB
 * Or in the first xact we allocate some inodes giving:
 *    the agi and agf of the ag getting the new inodes: 2 * sectorsize
 *    the inode blocks allocated: XFS_IALLOC_BLOCKS * blocksize
 *    the inode btree: max depth * blocksize
 *    the allocation btrees: 2 trees * (2 * max depth - 1) * block size
 */
STATIC uint
xfs_calc_symlink_reservation(
	struct xfs_mount	*mp)
{
	return XFS_DQUOT_LOGRES(mp) +
		MAX((mp->m_sb.sb_inodesize +
		     mp->m_sb.sb_inodesize +
		     XFS_FSB_TO_B(mp, 1) +
		     XFS_DIROP_LOG_RES(mp) +
		     1024 +
		     128 * (4 + XFS_DIROP_LOG_COUNT(mp))),
		    (2 * mp->m_sb.sb_sectsize +
		     XFS_FSB_TO_B(mp, XFS_IALLOC_BLOCKS(mp)) +
		     XFS_FSB_TO_B(mp, mp->m_in_maxlevels) +
		     XFS_ALLOCFREE_LOG_RES(mp, 1) +
		     128 * (2 + XFS_IALLOC_BLOCKS(mp) + mp->m_in_maxlevels +
			    XFS_ALLOCFREE_LOG_COUNT(mp, 1))));
}

/*
 * For create we can modify:
 *    the parent directory inode: inode size
 *    the new inode: inode size
 *    the inode btree entry: block size
 *    the superblock for the nlink flag: sector size
 *    the directory btree: (max depth + v2) * dir block size
 *    the directory inode's bmap btree: (max depth + v2) * block size
 * Or in the first xact we allocate some inodes giving:
 *    the agi and agf of the ag getting the new inodes: 2 * sectorsize
 *    the superblock for the nlink flag: sector size
 *    the inode blocks allocated: XFS_IALLOC_BLOCKS * blocksize
 *    the inode btree: max depth * blocksize
 *    the allocation btrees: 2 trees * (max depth - 1) * block size
 */
STATIC uint
xfs_calc_create_reservation(
	struct xfs_mount	*mp)
{
	return XFS_DQUOT_LOGRES(mp) +
		MAX((mp->m_sb.sb_inodesize +
		     mp->m_sb.sb_inodesize +
		     mp->m_sb.sb_sectsize +
		     XFS_FSB_TO_B(mp, 1) +
		     XFS_DIROP_LOG_RES(mp) +
		     128 * (3 + XFS_DIROP_LOG_COUNT(mp))),
		    (3 * mp->m_sb.sb_sectsize +
		     XFS_FSB_TO_B(mp, XFS_IALLOC_BLOCKS(mp)) +
		     XFS_FSB_TO_B(mp, mp->m_in_maxlevels) +
		     XFS_ALLOCFREE_LOG_RES(mp, 1) +
		     128 * (2 + XFS_IALLOC_BLOCKS(mp) + mp->m_in_maxlevels +
			    XFS_ALLOCFREE_LOG_COUNT(mp, 1))));
}

/*
 * Making a new directory is the same as creating a new file.
 */
STATIC uint
xfs_calc_mkdir_reservation(
	struct xfs_mount	*mp)
{
	return xfs_calc_create_reservation(mp);
}

/*
 * In freeing an inode we can modify:
 *    the inode being freed: inode size
 *    the super block free inode counter: sector size
 *    the agi hash list and counters: sector size
 *    the inode btree entry: block size
 *    the on disk inode before ours in the agi hash list: inode cluster size
 *    the inode btree: max depth * blocksize
 *    the allocation btrees: 2 trees * (max depth - 1) * block size
 */
STATIC uint
xfs_calc_ifree_reservation(
	struct xfs_mount	*mp)
{
	return XFS_DQUOT_LOGRES(mp) +
		mp->m_sb.sb_inodesize +
		mp->m_sb.sb_sectsize +
		mp->m_sb.sb_sectsize +
		XFS_FSB_TO_B(mp, 1) +
		MAX((__uint16_t)XFS_FSB_TO_B(mp, 1),
		    XFS_INODE_CLUSTER_SIZE(mp)) +
		128 * 5 +
		XFS_ALLOCFREE_LOG_RES(mp, 1) +
		128 * (2 + XFS_IALLOC_BLOCKS(mp) + mp->m_in_maxlevels +
		       XFS_ALLOCFREE_LOG_COUNT(mp, 1));
}

/*
 * When only changing the inode we log the inode and possibly the superblock
 * We also add a bit of slop for the transaction stuff.
 */
STATIC uint
xfs_calc_ichange_reservation(
	struct xfs_mount	*mp)
{
	return XFS_DQUOT_LOGRES(mp) +
		mp->m_sb.sb_inodesize +
		mp->m_sb.sb_sectsize +
		512;

}

/*
 * Growing the data section of the filesystem.
 *	superblock
 *	agi and agf
 *	allocation btrees
 */
STATIC uint
xfs_calc_growdata_reservation(
	struct xfs_mount	*mp)
{
	return mp->m_sb.sb_sectsize * 3 +
		XFS_ALLOCFREE_LOG_RES(mp, 1) +
		128 * (3 + XFS_ALLOCFREE_LOG_COUNT(mp, 1));
}

/*
 * Growing the rt section of the filesystem.
 * In the first set of transactions (ALLOC) we allocate space to the
 * bitmap or summary files.
 *	superblock: sector size
 *	agf of the ag from which the extent is allocated: sector size
 *	bmap btree for bitmap/summary inode: max depth * blocksize
 *	bitmap/summary inode: inode size
 *	allocation btrees for 1 block alloc: 2 * (2 * maxdepth - 1) * blocksize
 */
STATIC uint
xfs_calc_growrtalloc_reservation(
	struct xfs_mount	*mp)
{
	return 2 * mp->m_sb.sb_sectsize +
		XFS_FSB_TO_B(mp, XFS_BM_MAXLEVELS(mp, XFS_DATA_FORK)) +
		mp->m_sb.sb_inodesize +
		XFS_ALLOCFREE_LOG_RES(mp, 1) +
		128 * (3 + XFS_BM_MAXLEVELS(mp, XFS_DATA_FORK) +
		       XFS_ALLOCFREE_LOG_COUNT(mp, 1));
}

/*
 * Growing the rt section of the filesystem.
 * In the second set of transactions (ZERO) we zero the new metadata blocks.
 *	one bitmap/summary block: blocksize
 */
STATIC uint
xfs_calc_growrtzero_reservation(
	struct xfs_mount	*mp)
{
	return mp->m_sb.sb_blocksize + 128;
}

/*
 * Growing the rt section of the filesystem.
 * In the third set of transactions (FREE) we update metadata without
 * allocating any new blocks.
 *	superblock: sector size
 *	bitmap inode: inode size
 *	summary inode: inode size
 *	one bitmap block: blocksize
 *	summary blocks: new summary size
 */
STATIC uint
xfs_calc_growrtfree_reservation(
	struct xfs_mount	*mp)
{
	return mp->m_sb.sb_sectsize +
		2 * mp->m_sb.sb_inodesize +
		mp->m_sb.sb_blocksize +
		mp->m_rsumsize +
		128 * 5;
}

/*
 * Logging the inode modification timestamp on a synchronous write.
 *	inode
 */
STATIC uint
xfs_calc_swrite_reservation(
	struct xfs_mount	*mp)
{
	return mp->m_sb.sb_inodesize + 128;
}

/*
 * Logging the inode mode bits when writing a setuid/setgid file
 *	inode
 */
STATIC uint
xfs_calc_writeid_reservation(xfs_mount_t *mp)
{
	return mp->m_sb.sb_inodesize + 128;
}

/*
 * Converting the inode from non-attributed to attributed.
 *	the inode being converted: inode size
 *	agf block and superblock (for block allocation)
 *	the new block (directory sized)
 *	bmap blocks for the new directory block
 *	allocation btrees
 */
STATIC uint
xfs_calc_addafork_reservation(
	struct xfs_mount	*mp)
{
	return XFS_DQUOT_LOGRES(mp) +
		mp->m_sb.sb_inodesize +
		mp->m_sb.sb_sectsize * 2 +
		mp->m_dirblksize +
		XFS_FSB_TO_B(mp, XFS_DAENTER_BMAP1B(mp, XFS_DATA_FORK) + 1) +
		XFS_ALLOCFREE_LOG_RES(mp, 1) +
		128 * (4 + XFS_DAENTER_BMAP1B(mp, XFS_DATA_FORK) + 1 +
		       XFS_ALLOCFREE_LOG_COUNT(mp, 1));
}

/*
 * Removing the attribute fork of a file
 *    the inode being truncated: inode size
 *    the inode's bmap btree: max depth * block size
 * And the bmap_finish transaction can free the blocks and bmap blocks:
 *    the agf for each of the ags: 4 * sector size
 *    the agfl for each of the ags: 4 * sector size
 *    the super block to reflect the freed blocks: sector size
 *    worst case split in allocation btrees per extent assuming 4 extents:
 *		4 exts * 2 trees * (2 * max depth - 1) * block size
 */
STATIC uint
xfs_calc_attrinval_reservation(
	struct xfs_mount	*mp)
{
	return MAX((mp->m_sb.sb_inodesize +
		    XFS_FSB_TO_B(mp, XFS_BM_MAXLEVELS(mp, XFS_ATTR_FORK)) +
		    128 * (1 + XFS_BM_MAXLEVELS(mp, XFS_ATTR_FORK))),
		   (4 * mp->m_sb.sb_sectsize +
		    4 * mp->m_sb.sb_sectsize +
		    mp->m_sb.sb_sectsize +
		    XFS_ALLOCFREE_LOG_RES(mp, 4) +
		    128 * (9 + XFS_ALLOCFREE_LOG_COUNT(mp, 4))));
}

/*
 * Setting an attribute.
 *	the inode getting the attribute
 *	the superblock for allocations
 *	the agfs extents are allocated from
 *	the attribute btree * max depth
 *	the inode allocation btree
 * Since attribute transaction space is dependent on the size of the attribute,
 * the calculation is done partially at mount time and partially at runtime.
 */
STATIC uint
xfs_calc_attrset_reservation(
	struct xfs_mount	*mp)
{
	return XFS_DQUOT_LOGRES(mp) +
		mp->m_sb.sb_inodesize +
		mp->m_sb.sb_sectsize +
		XFS_FSB_TO_B(mp, XFS_DA_NODE_MAXDEPTH) +
		128 * (2 + XFS_DA_NODE_MAXDEPTH);
}

/*
 * Removing an attribute.
 *    the inode: inode size
 *    the attribute btree could join: max depth * block size
 *    the inode bmap btree could join or split: max depth * block size
 * And the bmap_finish transaction can free the attr blocks freed giving:
 *    the agf for the ag in which the blocks live: 2 * sector size
 *    the agfl for the ag in which the blocks live: 2 * sector size
 *    the superblock for the free block count: sector size
 *    the allocation btrees: 2 exts * 2 trees * (2 * max depth - 1) * block size
 */
STATIC uint
xfs_calc_attrrm_reservation(
	struct xfs_mount	*mp)
{
	return XFS_DQUOT_LOGRES(mp) +
		MAX((mp->m_sb.sb_inodesize +
		     XFS_FSB_TO_B(mp, XFS_DA_NODE_MAXDEPTH) +
		     XFS_FSB_TO_B(mp, XFS_BM_MAXLEVELS(mp, XFS_ATTR_FORK)) +
		     128 * (1 + XFS_DA_NODE_MAXDEPTH +
			    XFS_BM_MAXLEVELS(mp, XFS_DATA_FORK))),
		    (2 * mp->m_sb.sb_sectsize +
		     2 * mp->m_sb.sb_sectsize +
		     mp->m_sb.sb_sectsize +
		     XFS_ALLOCFREE_LOG_RES(mp, 2) +
		     128 * (5 + XFS_ALLOCFREE_LOG_COUNT(mp, 2))));
}

/*
 * Clearing a bad agino number in an agi hash bucket.
 */
STATIC uint
xfs_calc_clear_agi_bucket_reservation(
	struct xfs_mount	*mp)
{
	return mp->m_sb.sb_sectsize + 128;
}
=======
 */
#include "xfs.h"
#include "xfs_fs.h"
#include "xfs_shared.h"
#include "xfs_format.h"
#include "xfs_log_format.h"
#include "xfs_trans_resv.h"
#include "xfs_mount.h"
#include "xfs_extent_busy.h"
#include "xfs_quota.h"
#include "xfs_trans.h"
#include "xfs_trans_priv.h"
#include "xfs_log.h"
#include "xfs_log_priv.h"
#include "xfs_trace.h"
#include "xfs_error.h"
#include "xfs_defer.h"
#include "xfs_inode.h"
#include "xfs_dquot_item.h"
#include "xfs_dquot.h"
#include "xfs_icache.h"
#include "xfs_rtbitmap.h"

struct kmem_cache	*xfs_trans_cache;

#if defined(CONFIG_TRACEPOINTS)
static void
xfs_trans_trace_reservations(
	struct xfs_mount	*mp)
{
	struct xfs_trans_res	*res;
	struct xfs_trans_res	*end_res;
	int			i;

	res = (struct xfs_trans_res *)M_RES(mp);
	end_res = (struct xfs_trans_res *)(M_RES(mp) + 1);
	for (i = 0; res < end_res; i++, res++)
		trace_xfs_trans_resv_calc(mp, i, res);
}
#else
# define xfs_trans_trace_reservations(mp)
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Initialize the precomputed transaction reservation values
 * in the mount structure.
 */
void
xfs_trans_init(
	struct xfs_mount	*mp)
{
<<<<<<< HEAD
	struct xfs_trans_reservations *resp = &mp->m_reservations;

	resp->tr_write = xfs_calc_write_reservation(mp);
	resp->tr_itruncate = xfs_calc_itruncate_reservation(mp);
	resp->tr_rename = xfs_calc_rename_reservation(mp);
	resp->tr_link = xfs_calc_link_reservation(mp);
	resp->tr_remove = xfs_calc_remove_reservation(mp);
	resp->tr_symlink = xfs_calc_symlink_reservation(mp);
	resp->tr_create = xfs_calc_create_reservation(mp);
	resp->tr_mkdir = xfs_calc_mkdir_reservation(mp);
	resp->tr_ifree = xfs_calc_ifree_reservation(mp);
	resp->tr_ichange = xfs_calc_ichange_reservation(mp);
	resp->tr_growdata = xfs_calc_growdata_reservation(mp);
	resp->tr_swrite = xfs_calc_swrite_reservation(mp);
	resp->tr_writeid = xfs_calc_writeid_reservation(mp);
	resp->tr_addafork = xfs_calc_addafork_reservation(mp);
	resp->tr_attrinval = xfs_calc_attrinval_reservation(mp);
	resp->tr_attrset = xfs_calc_attrset_reservation(mp);
	resp->tr_attrrm = xfs_calc_attrrm_reservation(mp);
	resp->tr_clearagi = xfs_calc_clear_agi_bucket_reservation(mp);
	resp->tr_growrtalloc = xfs_calc_growrtalloc_reservation(mp);
	resp->tr_growrtzero = xfs_calc_growrtzero_reservation(mp);
	resp->tr_growrtfree = xfs_calc_growrtfree_reservation(mp);
}

/*
 * This routine is called to allocate a transaction structure.
 * The type parameter indicates the type of the transaction.  These
 * are enumerated in xfs_trans.h.
 *
 * Dynamically allocate the transaction structure from the transaction
 * zone, initialize it, and return it to the caller.
 */
xfs_trans_t *
xfs_trans_alloc(
	xfs_mount_t	*mp,
	uint		type)
{
	xfs_wait_for_freeze(mp, SB_FREEZE_TRANS);
	return _xfs_trans_alloc(mp, type, KM_SLEEP);
}

xfs_trans_t *
_xfs_trans_alloc(
	xfs_mount_t	*mp,
	uint		type,
	uint		memflags)
{
	xfs_trans_t	*tp;

	atomic_inc(&mp->m_active_trans);

	tp = kmem_zone_zalloc(xfs_trans_zone, memflags);
	tp->t_magic = XFS_TRANS_MAGIC;
	tp->t_type = type;
	tp->t_mountp = mp;
	INIT_LIST_HEAD(&tp->t_items);
	INIT_LIST_HEAD(&tp->t_busy);
	return tp;
=======
	xfs_trans_resv_calc(mp, M_RES(mp));
	xfs_trans_trace_reservations(mp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Free the transaction structure.  If there is more clean up
 * to do when the structure is freed, add it here.
 */
STATIC void
xfs_trans_free(
	struct xfs_trans	*tp)
{
<<<<<<< HEAD
	xfs_alloc_busy_sort(&tp->t_busy);
	xfs_alloc_busy_clear(tp->t_mountp, &tp->t_busy, false);

	atomic_dec(&tp->t_mountp->m_active_trans);
	xfs_trans_free_dqinfo(tp);
	kmem_zone_free(xfs_trans_zone, tp);
=======
	xfs_extent_busy_sort(&tp->t_busy);
	xfs_extent_busy_clear(tp->t_mountp, &tp->t_busy, false);

	trace_xfs_trans_free(tp, _RET_IP_);
	xfs_trans_clear_context(tp);
	if (!(tp->t_flags & XFS_TRANS_NO_WRITECOUNT))
		sb_end_intwrite(tp->t_mountp->m_super);
	xfs_trans_free_dqinfo(tp);
	kmem_cache_free(xfs_trans_cache, tp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * This is called to create a new transaction which will share the
 * permanent log reservation of the given transaction.  The remaining
 * unused block and rt extent reservations are also inherited.  This
 * implies that the original transaction is no longer allowed to allocate
 * blocks.  Locks and log items, however, are no inherited.  They must
 * be added to the new transaction explicitly.
 */
<<<<<<< HEAD
xfs_trans_t *
xfs_trans_dup(
	xfs_trans_t	*tp)
{
	xfs_trans_t	*ntp;

	ntp = kmem_zone_zalloc(xfs_trans_zone, KM_SLEEP);
=======
STATIC struct xfs_trans *
xfs_trans_dup(
	struct xfs_trans	*tp)
{
	struct xfs_trans	*ntp;

	trace_xfs_trans_dup(tp, _RET_IP_);

	ntp = kmem_cache_zalloc(xfs_trans_cache, GFP_KERNEL | __GFP_NOFAIL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Initialize the new transaction structure.
	 */
<<<<<<< HEAD
	ntp->t_magic = XFS_TRANS_MAGIC;
	ntp->t_type = tp->t_type;
	ntp->t_mountp = tp->t_mountp;
	INIT_LIST_HEAD(&ntp->t_items);
	INIT_LIST_HEAD(&ntp->t_busy);
=======
	ntp->t_magic = XFS_TRANS_HEADER_MAGIC;
	ntp->t_mountp = tp->t_mountp;
	INIT_LIST_HEAD(&ntp->t_items);
	INIT_LIST_HEAD(&ntp->t_busy);
	INIT_LIST_HEAD(&ntp->t_dfops);
	ntp->t_highest_agno = NULLAGNUMBER;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ASSERT(tp->t_flags & XFS_TRANS_PERM_LOG_RES);
	ASSERT(tp->t_ticket != NULL);

<<<<<<< HEAD
	ntp->t_flags = XFS_TRANS_PERM_LOG_RES | (tp->t_flags & XFS_TRANS_RESERVE);
	ntp->t_ticket = xfs_log_ticket_get(tp->t_ticket);
	ntp->t_blk_res = tp->t_blk_res - tp->t_blk_res_used;
	tp->t_blk_res = tp->t_blk_res_used;
	ntp->t_rtx_res = tp->t_rtx_res - tp->t_rtx_res_used;
	tp->t_rtx_res = tp->t_rtx_res_used;
	ntp->t_pflags = tp->t_pflags;

	xfs_trans_dup_dqinfo(tp, ntp);

	atomic_inc(&tp->t_mountp->m_active_trans);
=======
	ntp->t_flags = XFS_TRANS_PERM_LOG_RES |
		       (tp->t_flags & XFS_TRANS_RESERVE) |
		       (tp->t_flags & XFS_TRANS_NO_WRITECOUNT) |
		       (tp->t_flags & XFS_TRANS_RES_FDBLKS);
	/* We gave our writer reference to the new transaction */
	tp->t_flags |= XFS_TRANS_NO_WRITECOUNT;
	ntp->t_ticket = xfs_log_ticket_get(tp->t_ticket);

	ASSERT(tp->t_blk_res >= tp->t_blk_res_used);
	ntp->t_blk_res = tp->t_blk_res - tp->t_blk_res_used;
	tp->t_blk_res = tp->t_blk_res_used;

	ntp->t_rtx_res = tp->t_rtx_res - tp->t_rtx_res_used;
	tp->t_rtx_res = tp->t_rtx_res_used;

	xfs_trans_switch_context(tp, ntp);

	/* move deferred ops over to the new tp */
	xfs_defer_move(ntp, tp);

	xfs_trans_dup_dqinfo(tp, ntp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ntp;
}

/*
 * This is called to reserve free disk blocks and log space for the
 * given transaction.  This must be done before allocating any resources
 * within the transaction.
 *
 * This will return ENOSPC if there are not enough blocks available.
 * It will sleep waiting for available log space.
 * The only valid value for the flags parameter is XFS_RES_LOG_PERM, which
 * is used by long running transactions.  If any one of the reservations
 * fails then they will all be backed out.
 *
 * This does not do quota reservations. That typically is done by the
 * caller afterwards.
 */
<<<<<<< HEAD
int
xfs_trans_reserve(
	xfs_trans_t	*tp,
	uint		blocks,
	uint		logspace,
	uint		rtextents,
	uint		flags,
	uint		logcount)
{
	int		error = 0;
	int		rsvd = (tp->t_flags & XFS_TRANS_RESERVE) != 0;

	/* Mark this thread as being in a transaction */
	current_set_flags_nested(&tp->t_pflags, PF_FSTRANS);
=======
static int
xfs_trans_reserve(
	struct xfs_trans	*tp,
	struct xfs_trans_res	*resp,
	uint			blocks,
	uint			rtextents)
{
	struct xfs_mount	*mp = tp->t_mountp;
	int			error = 0;
	bool			rsvd = (tp->t_flags & XFS_TRANS_RESERVE) != 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Attempt to reserve the needed disk blocks by decrementing
	 * the number needed from the number available.  This will
	 * fail if the count would go below zero.
	 */
	if (blocks > 0) {
<<<<<<< HEAD
		error = xfs_icsb_modify_counters(tp->t_mountp, XFS_SBS_FDBLOCKS,
					  -((int64_t)blocks), rsvd);
		if (error != 0) {
			current_restore_flags_nested(&tp->t_pflags, PF_FSTRANS);
			return (XFS_ERROR(ENOSPC));
		}
=======
		error = xfs_mod_fdblocks(mp, -((int64_t)blocks), rsvd);
		if (error != 0)
			return -ENOSPC;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		tp->t_blk_res += blocks;
	}

	/*
	 * Reserve the log space needed for this transaction.
	 */
<<<<<<< HEAD
	if (logspace > 0) {
		bool	permanent = false;

		ASSERT(tp->t_log_res == 0 || tp->t_log_res == logspace);
		ASSERT(tp->t_log_count == 0 || tp->t_log_count == logcount);

		if (flags & XFS_TRANS_PERM_LOG_RES) {
=======
	if (resp->tr_logres > 0) {
		bool	permanent = false;

		ASSERT(tp->t_log_res == 0 ||
		       tp->t_log_res == resp->tr_logres);
		ASSERT(tp->t_log_count == 0 ||
		       tp->t_log_count == resp->tr_logcount);

		if (resp->tr_logflags & XFS_TRANS_PERM_LOG_RES) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			tp->t_flags |= XFS_TRANS_PERM_LOG_RES;
			permanent = true;
		} else {
			ASSERT(tp->t_ticket == NULL);
			ASSERT(!(tp->t_flags & XFS_TRANS_PERM_LOG_RES));
		}

		if (tp->t_ticket != NULL) {
<<<<<<< HEAD
			ASSERT(flags & XFS_TRANS_PERM_LOG_RES);
			error = xfs_log_regrant(tp->t_mountp, tp->t_ticket);
		} else {
			error = xfs_log_reserve(tp->t_mountp, logspace,
						logcount, &tp->t_ticket,
						XFS_TRANSACTION, permanent,
						tp->t_type);
=======
			ASSERT(resp->tr_logflags & XFS_TRANS_PERM_LOG_RES);
			error = xfs_log_regrant(mp, tp->t_ticket);
		} else {
			error = xfs_log_reserve(mp, resp->tr_logres,
						resp->tr_logcount,
						&tp->t_ticket, permanent);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}

		if (error)
			goto undo_blocks;

<<<<<<< HEAD
		tp->t_log_res = logspace;
		tp->t_log_count = logcount;
=======
		tp->t_log_res = resp->tr_logres;
		tp->t_log_count = resp->tr_logcount;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/*
	 * Attempt to reserve the needed realtime extents by decrementing
	 * the number needed from the number available.  This will
	 * fail if the count would go below zero.
	 */
	if (rtextents > 0) {
<<<<<<< HEAD
		error = xfs_mod_incore_sb(tp->t_mountp, XFS_SBS_FREXTENTS,
					  -((int64_t)rtextents), rsvd);
		if (error) {
			error = XFS_ERROR(ENOSPC);
=======
		error = xfs_mod_frextents(mp, -((int64_t)rtextents));
		if (error) {
			error = -ENOSPC;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto undo_log;
		}
		tp->t_rtx_res += rtextents;
	}

	return 0;

	/*
	 * Error cases jump to one of these labels to undo any
	 * reservations which have already been performed.
	 */
undo_log:
<<<<<<< HEAD
	if (logspace > 0) {
		int		log_flags;

		if (flags & XFS_TRANS_PERM_LOG_RES) {
			log_flags = XFS_LOG_REL_PERM_RESERV;
		} else {
			log_flags = 0;
		}
		xfs_log_done(tp->t_mountp, tp->t_ticket, NULL, log_flags);
=======
	if (resp->tr_logres > 0) {
		xfs_log_ticket_ungrant(mp->m_log, tp->t_ticket);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		tp->t_ticket = NULL;
		tp->t_log_res = 0;
		tp->t_flags &= ~XFS_TRANS_PERM_LOG_RES;
	}

undo_blocks:
	if (blocks > 0) {
<<<<<<< HEAD
		xfs_icsb_modify_counters(tp->t_mountp, XFS_SBS_FDBLOCKS,
					 (int64_t)blocks, rsvd);
		tp->t_blk_res = 0;
	}

	current_restore_flags_nested(&tp->t_pflags, PF_FSTRANS);

	return error;
}

=======
		xfs_mod_fdblocks(mp, (int64_t)blocks, rsvd);
		tp->t_blk_res = 0;
	}
	return error;
}

int
xfs_trans_alloc(
	struct xfs_mount	*mp,
	struct xfs_trans_res	*resp,
	uint			blocks,
	uint			rtextents,
	uint			flags,
	struct xfs_trans	**tpp)
{
	struct xfs_trans	*tp;
	bool			want_retry = true;
	int			error;

	/*
	 * Allocate the handle before we do our freeze accounting and setting up
	 * GFP_NOFS allocation context so that we avoid lockdep false positives
	 * by doing GFP_KERNEL allocations inside sb_start_intwrite().
	 */
retry:
	tp = kmem_cache_zalloc(xfs_trans_cache, GFP_KERNEL | __GFP_NOFAIL);
	if (!(flags & XFS_TRANS_NO_WRITECOUNT))
		sb_start_intwrite(mp->m_super);
	xfs_trans_set_context(tp);

	/*
	 * Zero-reservation ("empty") transactions can't modify anything, so
	 * they're allowed to run while we're frozen.
	 */
	WARN_ON(resp->tr_logres > 0 &&
		mp->m_super->s_writers.frozen == SB_FREEZE_COMPLETE);
	ASSERT(!(flags & XFS_TRANS_RES_FDBLKS) ||
	       xfs_has_lazysbcount(mp));

	tp->t_magic = XFS_TRANS_HEADER_MAGIC;
	tp->t_flags = flags;
	tp->t_mountp = mp;
	INIT_LIST_HEAD(&tp->t_items);
	INIT_LIST_HEAD(&tp->t_busy);
	INIT_LIST_HEAD(&tp->t_dfops);
	tp->t_highest_agno = NULLAGNUMBER;

	error = xfs_trans_reserve(tp, resp, blocks, rtextents);
	if (error == -ENOSPC && want_retry) {
		xfs_trans_cancel(tp);

		/*
		 * We weren't able to reserve enough space for the transaction.
		 * Flush the other speculative space allocations to free space.
		 * Do not perform a synchronous scan because callers can hold
		 * other locks.
		 */
		error = xfs_blockgc_flush_all(mp);
		if (error)
			return error;
		want_retry = false;
		goto retry;
	}
	if (error) {
		xfs_trans_cancel(tp);
		return error;
	}

	trace_xfs_trans_alloc(tp, _RET_IP_);

	*tpp = tp;
	return 0;
}

/*
 * Create an empty transaction with no reservation.  This is a defensive
 * mechanism for routines that query metadata without actually modifying them --
 * if the metadata being queried is somehow cross-linked (think a btree block
 * pointer that points higher in the tree), we risk deadlock.  However, blocks
 * grabbed as part of a transaction can be re-grabbed.  The verifiers will
 * notice the corrupt block and the operation will fail back to userspace
 * without deadlocking.
 *
 * Note the zero-length reservation; this transaction MUST be cancelled without
 * any dirty data.
 *
 * Callers should obtain freeze protection to avoid a conflict with fs freezing
 * where we can be grabbing buffers at the same time that freeze is trying to
 * drain the buffer LRU list.
 */
int
xfs_trans_alloc_empty(
	struct xfs_mount		*mp,
	struct xfs_trans		**tpp)
{
	struct xfs_trans_res		resv = {0};

	return xfs_trans_alloc(mp, &resv, 0, 0, XFS_TRANS_NO_WRITECOUNT, tpp);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Record the indicated change to the given field for application
 * to the file system's superblock when the transaction commits.
 * For now, just store the change in the transaction structure.
 *
 * Mark the transaction structure to indicate that the superblock
 * needs to be updated before committing.
 *
 * Because we may not be keeping track of allocated/free inodes and
 * used filesystem blocks in the superblock, we do not mark the
 * superblock dirty in this transaction if we modify these fields.
 * We still need to update the transaction deltas so that they get
 * applied to the incore superblock, but we don't want them to
 * cause the superblock to get locked and logged if these are the
 * only fields in the superblock that the transaction modifies.
 */
void
xfs_trans_mod_sb(
	xfs_trans_t	*tp,
	uint		field,
	int64_t		delta)
{
	uint32_t	flags = (XFS_TRANS_DIRTY|XFS_TRANS_SB_DIRTY);
	xfs_mount_t	*mp = tp->t_mountp;

	switch (field) {
	case XFS_TRANS_SB_ICOUNT:
		tp->t_icount_delta += delta;
<<<<<<< HEAD
		if (xfs_sb_version_haslazysbcount(&mp->m_sb))
=======
		if (xfs_has_lazysbcount(mp))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			flags &= ~XFS_TRANS_SB_DIRTY;
		break;
	case XFS_TRANS_SB_IFREE:
		tp->t_ifree_delta += delta;
<<<<<<< HEAD
		if (xfs_sb_version_haslazysbcount(&mp->m_sb))
=======
		if (xfs_has_lazysbcount(mp))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			flags &= ~XFS_TRANS_SB_DIRTY;
		break;
	case XFS_TRANS_SB_FDBLOCKS:
		/*
<<<<<<< HEAD
		 * Track the number of blocks allocated in the
		 * transaction.  Make sure it does not exceed the
		 * number reserved.
		 */
		if (delta < 0) {
			tp->t_blk_res_used += (uint)-delta;
			ASSERT(tp->t_blk_res_used <= tp->t_blk_res);
		}
		tp->t_fdblocks_delta += delta;
		if (xfs_sb_version_haslazysbcount(&mp->m_sb))
=======
		 * Track the number of blocks allocated in the transaction.
		 * Make sure it does not exceed the number reserved. If so,
		 * shutdown as this can lead to accounting inconsistency.
		 */
		if (delta < 0) {
			tp->t_blk_res_used += (uint)-delta;
			if (tp->t_blk_res_used > tp->t_blk_res)
				xfs_force_shutdown(mp, SHUTDOWN_CORRUPT_INCORE);
		} else if (delta > 0 && (tp->t_flags & XFS_TRANS_RES_FDBLKS)) {
			int64_t	blkres_delta;

			/*
			 * Return freed blocks directly to the reservation
			 * instead of the global pool, being careful not to
			 * overflow the trans counter. This is used to preserve
			 * reservation across chains of transaction rolls that
			 * repeatedly free and allocate blocks.
			 */
			blkres_delta = min_t(int64_t, delta,
					     UINT_MAX - tp->t_blk_res);
			tp->t_blk_res += blkres_delta;
			delta -= blkres_delta;
		}
		tp->t_fdblocks_delta += delta;
		if (xfs_has_lazysbcount(mp))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			flags &= ~XFS_TRANS_SB_DIRTY;
		break;
	case XFS_TRANS_SB_RES_FDBLOCKS:
		/*
		 * The allocation has already been applied to the
		 * in-core superblock's counter.  This should only
		 * be applied to the on-disk superblock.
		 */
<<<<<<< HEAD
		ASSERT(delta < 0);
		tp->t_res_fdblocks_delta += delta;
		if (xfs_sb_version_haslazysbcount(&mp->m_sb))
=======
		tp->t_res_fdblocks_delta += delta;
		if (xfs_has_lazysbcount(mp))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			flags &= ~XFS_TRANS_SB_DIRTY;
		break;
	case XFS_TRANS_SB_FREXTENTS:
		/*
		 * Track the number of blocks allocated in the
		 * transaction.  Make sure it does not exceed the
		 * number reserved.
		 */
		if (delta < 0) {
			tp->t_rtx_res_used += (uint)-delta;
			ASSERT(tp->t_rtx_res_used <= tp->t_rtx_res);
		}
		tp->t_frextents_delta += delta;
		break;
	case XFS_TRANS_SB_RES_FREXTENTS:
		/*
		 * The allocation has already been applied to the
		 * in-core superblock's counter.  This should only
		 * be applied to the on-disk superblock.
		 */
		ASSERT(delta < 0);
		tp->t_res_frextents_delta += delta;
		break;
	case XFS_TRANS_SB_DBLOCKS:
<<<<<<< HEAD
		ASSERT(delta > 0);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		tp->t_dblocks_delta += delta;
		break;
	case XFS_TRANS_SB_AGCOUNT:
		ASSERT(delta > 0);
		tp->t_agcount_delta += delta;
		break;
	case XFS_TRANS_SB_IMAXPCT:
		tp->t_imaxpct_delta += delta;
		break;
	case XFS_TRANS_SB_REXTSIZE:
		tp->t_rextsize_delta += delta;
		break;
	case XFS_TRANS_SB_RBMBLOCKS:
		tp->t_rbmblocks_delta += delta;
		break;
	case XFS_TRANS_SB_RBLOCKS:
		tp->t_rblocks_delta += delta;
		break;
	case XFS_TRANS_SB_REXTENTS:
		tp->t_rextents_delta += delta;
		break;
	case XFS_TRANS_SB_REXTSLOG:
		tp->t_rextslog_delta += delta;
		break;
	default:
		ASSERT(0);
		return;
	}

	tp->t_flags |= flags;
}

/*
 * xfs_trans_apply_sb_deltas() is called from the commit code
 * to bring the superblock buffer into the current transaction
 * and modify it as requested by earlier calls to xfs_trans_mod_sb().
 *
 * For now we just look at each field allowed to change and change
 * it if necessary.
 */
STATIC void
xfs_trans_apply_sb_deltas(
	xfs_trans_t	*tp)
{
<<<<<<< HEAD
	xfs_dsb_t	*sbp;
	xfs_buf_t	*bp;
	int		whole = 0;

	bp = xfs_trans_getsb(tp, tp->t_mountp, 0);
	sbp = XFS_BUF_TO_SBP(bp);

	/*
	 * Check that superblock mods match the mods made to AGF counters.
	 */
	ASSERT((tp->t_fdblocks_delta + tp->t_res_fdblocks_delta) ==
	       (tp->t_ag_freeblks_delta + tp->t_ag_flist_delta +
		tp->t_ag_btree_delta));
=======
	struct xfs_dsb	*sbp;
	struct xfs_buf	*bp;
	int		whole = 0;

	bp = xfs_trans_getsb(tp);
	sbp = bp->b_addr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Only update the superblock counters if we are logging them
	 */
<<<<<<< HEAD
	if (!xfs_sb_version_haslazysbcount(&(tp->t_mountp->m_sb))) {
=======
	if (!xfs_has_lazysbcount((tp->t_mountp))) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (tp->t_icount_delta)
			be64_add_cpu(&sbp->sb_icount, tp->t_icount_delta);
		if (tp->t_ifree_delta)
			be64_add_cpu(&sbp->sb_ifree, tp->t_ifree_delta);
		if (tp->t_fdblocks_delta)
			be64_add_cpu(&sbp->sb_fdblocks, tp->t_fdblocks_delta);
		if (tp->t_res_fdblocks_delta)
			be64_add_cpu(&sbp->sb_fdblocks, tp->t_res_fdblocks_delta);
	}

<<<<<<< HEAD
	if (tp->t_frextents_delta)
		be64_add_cpu(&sbp->sb_frextents, tp->t_frextents_delta);
	if (tp->t_res_frextents_delta)
		be64_add_cpu(&sbp->sb_frextents, tp->t_res_frextents_delta);
=======
	/*
	 * Updating frextents requires careful handling because it does not
	 * behave like the lazysb counters because we cannot rely on log
	 * recovery in older kenels to recompute the value from the rtbitmap.
	 * This means that the ondisk frextents must be consistent with the
	 * rtbitmap.
	 *
	 * Therefore, log the frextents change to the ondisk superblock and
	 * update the incore superblock so that future calls to xfs_log_sb
	 * write the correct value ondisk.
	 *
	 * Don't touch m_frextents because it includes incore reservations,
	 * and those are handled by the unreserve function.
	 */
	if (tp->t_frextents_delta || tp->t_res_frextents_delta) {
		struct xfs_mount	*mp = tp->t_mountp;
		int64_t			rtxdelta;

		rtxdelta = tp->t_frextents_delta + tp->t_res_frextents_delta;

		spin_lock(&mp->m_sb_lock);
		be64_add_cpu(&sbp->sb_frextents, rtxdelta);
		mp->m_sb.sb_frextents += rtxdelta;
		spin_unlock(&mp->m_sb_lock);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (tp->t_dblocks_delta) {
		be64_add_cpu(&sbp->sb_dblocks, tp->t_dblocks_delta);
		whole = 1;
	}
	if (tp->t_agcount_delta) {
		be32_add_cpu(&sbp->sb_agcount, tp->t_agcount_delta);
		whole = 1;
	}
	if (tp->t_imaxpct_delta) {
		sbp->sb_imax_pct += tp->t_imaxpct_delta;
		whole = 1;
	}
	if (tp->t_rextsize_delta) {
		be32_add_cpu(&sbp->sb_rextsize, tp->t_rextsize_delta);
		whole = 1;
	}
	if (tp->t_rbmblocks_delta) {
		be32_add_cpu(&sbp->sb_rbmblocks, tp->t_rbmblocks_delta);
		whole = 1;
	}
	if (tp->t_rblocks_delta) {
		be64_add_cpu(&sbp->sb_rblocks, tp->t_rblocks_delta);
		whole = 1;
	}
	if (tp->t_rextents_delta) {
		be64_add_cpu(&sbp->sb_rextents, tp->t_rextents_delta);
		whole = 1;
	}
	if (tp->t_rextslog_delta) {
		sbp->sb_rextslog += tp->t_rextslog_delta;
		whole = 1;
	}

<<<<<<< HEAD
=======
	xfs_trans_buf_set_type(tp, bp, XFS_BLFT_SB_BUF);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (whole)
		/*
		 * Log the whole thing, the fields are noncontiguous.
		 */
<<<<<<< HEAD
		xfs_trans_log_buf(tp, bp, 0, sizeof(xfs_dsb_t) - 1);
=======
		xfs_trans_log_buf(tp, bp, 0, sizeof(struct xfs_dsb) - 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	else
		/*
		 * Since all the modifiable fields are contiguous, we
		 * can get away with this.
		 */
<<<<<<< HEAD
		xfs_trans_log_buf(tp, bp, offsetof(xfs_dsb_t, sb_icount),
				  offsetof(xfs_dsb_t, sb_frextents) +
=======
		xfs_trans_log_buf(tp, bp, offsetof(struct xfs_dsb, sb_icount),
				  offsetof(struct xfs_dsb, sb_frextents) +
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				  sizeof(sbp->sb_frextents) - 1);
}

/*
<<<<<<< HEAD
 * xfs_trans_unreserve_and_mod_sb() is called to release unused reservations
 * and apply superblock counter changes to the in-core superblock.  The
=======
 * xfs_trans_unreserve_and_mod_sb() is called to release unused reservations and
 * apply superblock counter changes to the in-core superblock.  The
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * t_res_fdblocks_delta and t_res_frextents_delta fields are explicitly NOT
 * applied to the in-core superblock.  The idea is that that has already been
 * done.
 *
<<<<<<< HEAD
 * This is done efficiently with a single call to xfs_mod_incore_sb_batch().
 * However, we have to ensure that we only modify each superblock field only
 * once because the application of the delta values may not be atomic. That can
 * lead to ENOSPC races occurring if we have two separate modifcations of the
 * free space counter to put back the entire reservation and then take away
 * what we used.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * If we are not logging superblock counters, then the inode allocated/free and
 * used block counts are not updated in the on disk superblock. In this case,
 * XFS_TRANS_SB_DIRTY will not be set when the transaction is updated but we
 * still need to update the incore superblock with the changes.
<<<<<<< HEAD
 */
void
xfs_trans_unreserve_and_mod_sb(
	xfs_trans_t	*tp)
{
	xfs_mod_sb_t	msb[9];	/* If you add cases, add entries */
	xfs_mod_sb_t	*msbp;
	xfs_mount_t	*mp = tp->t_mountp;
	/* REFERENCED */
	int		error;
	int		rsvd;
	int64_t		blkdelta = 0;
	int64_t		rtxdelta = 0;
	int64_t		idelta = 0;
	int64_t		ifreedelta = 0;

	msbp = msb;
	rsvd = (tp->t_flags & XFS_TRANS_RESERVE) != 0;
=======
 *
 * Deltas for the inode count are +/-64, hence we use a large batch size of 128
 * so we don't need to take the counter lock on every update.
 */
#define XFS_ICOUNT_BATCH	128

void
xfs_trans_unreserve_and_mod_sb(
	struct xfs_trans	*tp)
{
	struct xfs_mount	*mp = tp->t_mountp;
	bool			rsvd = (tp->t_flags & XFS_TRANS_RESERVE) != 0;
	int64_t			blkdelta = 0;
	int64_t			rtxdelta = 0;
	int64_t			idelta = 0;
	int64_t			ifreedelta = 0;
	int			error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* calculate deltas */
	if (tp->t_blk_res > 0)
		blkdelta = tp->t_blk_res;
	if ((tp->t_fdblocks_delta != 0) &&
<<<<<<< HEAD
	    (xfs_sb_version_haslazysbcount(&mp->m_sb) ||
=======
	    (xfs_has_lazysbcount(mp) ||
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	     (tp->t_flags & XFS_TRANS_SB_DIRTY)))
	        blkdelta += tp->t_fdblocks_delta;

	if (tp->t_rtx_res > 0)
		rtxdelta = tp->t_rtx_res;
	if ((tp->t_frextents_delta != 0) &&
	    (tp->t_flags & XFS_TRANS_SB_DIRTY))
		rtxdelta += tp->t_frextents_delta;

<<<<<<< HEAD
	if (xfs_sb_version_haslazysbcount(&mp->m_sb) ||
=======
	if (xfs_has_lazysbcount(mp) ||
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	     (tp->t_flags & XFS_TRANS_SB_DIRTY)) {
		idelta = tp->t_icount_delta;
		ifreedelta = tp->t_ifree_delta;
	}

	/* apply the per-cpu counters */
	if (blkdelta) {
<<<<<<< HEAD
		error = xfs_icsb_modify_counters(mp, XFS_SBS_FDBLOCKS,
						 blkdelta, rsvd);
		if (error)
			goto out;
	}

	if (idelta) {
		error = xfs_icsb_modify_counters(mp, XFS_SBS_ICOUNT,
						 idelta, rsvd);
		if (error)
			goto out_undo_fdblocks;
	}

	if (ifreedelta) {
		error = xfs_icsb_modify_counters(mp, XFS_SBS_IFREE,
						 ifreedelta, rsvd);
		if (error)
			goto out_undo_icount;
	}

	/* apply remaining deltas */
	if (rtxdelta != 0) {
		msbp->msb_field = XFS_SBS_FREXTENTS;
		msbp->msb_delta = rtxdelta;
		msbp++;
	}

	if (tp->t_flags & XFS_TRANS_SB_DIRTY) {
		if (tp->t_dblocks_delta != 0) {
			msbp->msb_field = XFS_SBS_DBLOCKS;
			msbp->msb_delta = tp->t_dblocks_delta;
			msbp++;
		}
		if (tp->t_agcount_delta != 0) {
			msbp->msb_field = XFS_SBS_AGCOUNT;
			msbp->msb_delta = tp->t_agcount_delta;
			msbp++;
		}
		if (tp->t_imaxpct_delta != 0) {
			msbp->msb_field = XFS_SBS_IMAX_PCT;
			msbp->msb_delta = tp->t_imaxpct_delta;
			msbp++;
		}
		if (tp->t_rextsize_delta != 0) {
			msbp->msb_field = XFS_SBS_REXTSIZE;
			msbp->msb_delta = tp->t_rextsize_delta;
			msbp++;
		}
		if (tp->t_rbmblocks_delta != 0) {
			msbp->msb_field = XFS_SBS_RBMBLOCKS;
			msbp->msb_delta = tp->t_rbmblocks_delta;
			msbp++;
		}
		if (tp->t_rblocks_delta != 0) {
			msbp->msb_field = XFS_SBS_RBLOCKS;
			msbp->msb_delta = tp->t_rblocks_delta;
			msbp++;
		}
		if (tp->t_rextents_delta != 0) {
			msbp->msb_field = XFS_SBS_REXTENTS;
			msbp->msb_delta = tp->t_rextents_delta;
			msbp++;
		}
		if (tp->t_rextslog_delta != 0) {
			msbp->msb_field = XFS_SBS_REXTSLOG;
			msbp->msb_delta = tp->t_rextslog_delta;
			msbp++;
		}
	}

	/*
	 * If we need to change anything, do it.
	 */
	if (msbp > msb) {
		error = xfs_mod_incore_sb_batch(tp->t_mountp, msb,
			(uint)(msbp - msb), rsvd);
		if (error)
			goto out_undo_ifreecount;
	}

	return;

out_undo_ifreecount:
	if (ifreedelta)
		xfs_icsb_modify_counters(mp, XFS_SBS_IFREE, -ifreedelta, rsvd);
out_undo_icount:
	if (idelta)
		xfs_icsb_modify_counters(mp, XFS_SBS_ICOUNT, -idelta, rsvd);
out_undo_fdblocks:
	if (blkdelta)
		xfs_icsb_modify_counters(mp, XFS_SBS_FDBLOCKS, -blkdelta, rsvd);
out:
	ASSERT(error == 0);
	return;
}

/*
 * Add the given log item to the transaction's list of log items.
 *
 * The log item will now point to its new descriptor with its li_desc field.
 */
=======
		error = xfs_mod_fdblocks(mp, blkdelta, rsvd);
		ASSERT(!error);
	}

	if (idelta)
		percpu_counter_add_batch(&mp->m_icount, idelta,
					 XFS_ICOUNT_BATCH);

	if (ifreedelta)
		percpu_counter_add(&mp->m_ifree, ifreedelta);

	if (rtxdelta) {
		error = xfs_mod_frextents(mp, rtxdelta);
		ASSERT(!error);
	}

	if (!(tp->t_flags & XFS_TRANS_SB_DIRTY))
		return;

	/* apply remaining deltas */
	spin_lock(&mp->m_sb_lock);
	mp->m_sb.sb_fdblocks += tp->t_fdblocks_delta + tp->t_res_fdblocks_delta;
	mp->m_sb.sb_icount += idelta;
	mp->m_sb.sb_ifree += ifreedelta;
	/*
	 * Do not touch sb_frextents here because we are dealing with incore
	 * reservation.  sb_frextents is not part of the lazy sb counters so it
	 * must be consistent with the ondisk rtbitmap and must never include
	 * incore reservations.
	 */
	mp->m_sb.sb_dblocks += tp->t_dblocks_delta;
	mp->m_sb.sb_agcount += tp->t_agcount_delta;
	mp->m_sb.sb_imax_pct += tp->t_imaxpct_delta;
	mp->m_sb.sb_rextsize += tp->t_rextsize_delta;
	if (tp->t_rextsize_delta) {
		mp->m_rtxblklog = log2_if_power2(mp->m_sb.sb_rextsize);
		mp->m_rtxblkmask = mask64_if_power2(mp->m_sb.sb_rextsize);
	}
	mp->m_sb.sb_rbmblocks += tp->t_rbmblocks_delta;
	mp->m_sb.sb_rblocks += tp->t_rblocks_delta;
	mp->m_sb.sb_rextents += tp->t_rextents_delta;
	mp->m_sb.sb_rextslog += tp->t_rextslog_delta;
	spin_unlock(&mp->m_sb_lock);

	/*
	 * Debug checks outside of the spinlock so they don't lock up the
	 * machine if they fail.
	 */
	ASSERT(mp->m_sb.sb_imax_pct >= 0);
	ASSERT(mp->m_sb.sb_rextslog >= 0);
	return;
}

/* Add the given log item to the transaction's list of log items. */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void
xfs_trans_add_item(
	struct xfs_trans	*tp,
	struct xfs_log_item	*lip)
{
<<<<<<< HEAD
	struct xfs_log_item_desc *lidp;

	ASSERT(lip->li_mountp == tp->t_mountp);
	ASSERT(lip->li_ailp == tp->t_mountp->m_ail);

	lidp = kmem_zone_zalloc(xfs_log_item_desc_zone, KM_SLEEP | KM_NOFS);

	lidp->lid_item = lip;
	lidp->lid_flags = 0;
	list_add_tail(&lidp->lid_trans, &tp->t_items);

	lip->li_desc = lidp;
}

STATIC void
xfs_trans_free_item_desc(
	struct xfs_log_item_desc *lidp)
{
	list_del_init(&lidp->lid_trans);
	kmem_zone_free(xfs_log_item_desc_zone, lidp);
}

/*
 * Unlink and free the given descriptor.
=======
	ASSERT(lip->li_log == tp->t_mountp->m_log);
	ASSERT(lip->li_ailp == tp->t_mountp->m_ail);
	ASSERT(list_empty(&lip->li_trans));
	ASSERT(!test_bit(XFS_LI_DIRTY, &lip->li_flags));

	list_add_tail(&lip->li_trans, &tp->t_items);
	trace_xfs_trans_add_item(tp, _RET_IP_);
}

/*
 * Unlink the log item from the transaction. the log item is no longer
 * considered dirty in this transaction, as the linked transaction has
 * finished, either by abort or commit completion.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
void
xfs_trans_del_item(
	struct xfs_log_item	*lip)
{
<<<<<<< HEAD
	xfs_trans_free_item_desc(lip->li_desc);
	lip->li_desc = NULL;
}

/*
 * Unlock all of the items of a transaction and free all the descriptors
 * of that transaction.
 */
void
xfs_trans_free_items(
	struct xfs_trans	*tp,
	xfs_lsn_t		commit_lsn,
	int			flags)
{
	struct xfs_log_item_desc *lidp, *next;

	list_for_each_entry_safe(lidp, next, &tp->t_items, lid_trans) {
		struct xfs_log_item	*lip = lidp->lid_item;

		lip->li_desc = NULL;

		if (commit_lsn != NULLCOMMITLSN)
			IOP_COMMITTING(lip, commit_lsn);
		if (flags & XFS_TRANS_ABORT)
			lip->li_flags |= XFS_LI_ABORTED;
		IOP_UNLOCK(lip);

		xfs_trans_free_item_desc(lidp);
=======
	clear_bit(XFS_LI_DIRTY, &lip->li_flags);
	list_del_init(&lip->li_trans);
}

/* Detach and unlock all of the items in a transaction */
static void
xfs_trans_free_items(
	struct xfs_trans	*tp,
	bool			abort)
{
	struct xfs_log_item	*lip, *next;

	trace_xfs_trans_free_items(tp, _RET_IP_);

	list_for_each_entry_safe(lip, next, &tp->t_items, li_trans) {
		xfs_trans_del_item(lip);
		if (abort)
			set_bit(XFS_LI_ABORTED, &lip->li_flags);
		if (lip->li_ops->iop_release)
			lip->li_ops->iop_release(lip);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

static inline void
xfs_log_item_batch_insert(
	struct xfs_ail		*ailp,
	struct xfs_ail_cursor	*cur,
	struct xfs_log_item	**log_items,
	int			nr_items,
	xfs_lsn_t		commit_lsn)
{
	int	i;

<<<<<<< HEAD
	spin_lock(&ailp->xa_lock);
	/* xfs_trans_ail_update_bulk drops ailp->xa_lock */
	xfs_trans_ail_update_bulk(ailp, cur, log_items, nr_items, commit_lsn);

	for (i = 0; i < nr_items; i++)
		IOP_UNPIN(log_items[i], 0);
=======
	spin_lock(&ailp->ail_lock);
	/* xfs_trans_ail_update_bulk drops ailp->ail_lock */
	xfs_trans_ail_update_bulk(ailp, cur, log_items, nr_items, commit_lsn);

	for (i = 0; i < nr_items; i++) {
		struct xfs_log_item *lip = log_items[i];

		if (lip->li_ops->iop_unpin)
			lip->li_ops->iop_unpin(lip, 0);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Bulk operation version of xfs_trans_committed that takes a log vector of
 * items to insert into the AIL. This uses bulk AIL insertion techniques to
 * minimise lock traffic.
 *
 * If we are called with the aborted flag set, it is because a log write during
 * a CIL checkpoint commit has failed. In this case, all the items in the
<<<<<<< HEAD
 * checkpoint have already gone through IOP_COMMITED and IOP_UNLOCK, which
 * means that checkpoint commit abort handling is treated exactly the same
 * as an iclog write error even though we haven't started any IO yet. Hence in
 * this case all we need to do is IOP_COMMITTED processing, followed by an
 * IOP_UNPIN(aborted) call.
=======
 * checkpoint have already gone through iop_committed and iop_committing, which
 * means that checkpoint commit abort handling is treated exactly the same
 * as an iclog write error even though we haven't started any IO yet. Hence in
 * this case all we need to do is iop_committed processing, followed by an
 * iop_unpin(aborted) call.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * The AIL cursor is used to optimise the insert process. If commit_lsn is not
 * at the end of the AIL, the insert cursor avoids the need to walk
 * the AIL to find the insertion point on every xfs_log_item_batch_insert()
 * call. This saves a lot of needless list walking and is a net win, even
 * though it slightly increases that amount of AIL lock traffic to set it up
 * and tear it down.
 */
void
xfs_trans_committed_bulk(
	struct xfs_ail		*ailp,
<<<<<<< HEAD
	struct xfs_log_vec	*log_vector,
	xfs_lsn_t		commit_lsn,
	int			aborted)
=======
	struct list_head	*lv_chain,
	xfs_lsn_t		commit_lsn,
	bool			aborted)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
#define LOG_ITEM_BATCH_SIZE	32
	struct xfs_log_item	*log_items[LOG_ITEM_BATCH_SIZE];
	struct xfs_log_vec	*lv;
	struct xfs_ail_cursor	cur;
	int			i = 0;

<<<<<<< HEAD
	spin_lock(&ailp->xa_lock);
	xfs_trans_ail_cursor_last(ailp, &cur, commit_lsn);
	spin_unlock(&ailp->xa_lock);

	/* unpin all the log items */
	for (lv = log_vector; lv; lv = lv->lv_next ) {
=======
	spin_lock(&ailp->ail_lock);
	xfs_trans_ail_cursor_last(ailp, &cur, commit_lsn);
	spin_unlock(&ailp->ail_lock);

	/* unpin all the log items */
	list_for_each_entry(lv, lv_chain, lv_list) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		struct xfs_log_item	*lip = lv->lv_item;
		xfs_lsn_t		item_lsn;

		if (aborted)
<<<<<<< HEAD
			lip->li_flags |= XFS_LI_ABORTED;
		item_lsn = IOP_COMMITTED(lip, commit_lsn);
=======
			set_bit(XFS_LI_ABORTED, &lip->li_flags);

		if (lip->li_ops->flags & XFS_ITEM_RELEASE_WHEN_COMMITTED) {
			lip->li_ops->iop_release(lip);
			continue;
		}

		if (lip->li_ops->iop_committed)
			item_lsn = lip->li_ops->iop_committed(lip, commit_lsn);
		else
			item_lsn = commit_lsn;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/* item_lsn of -1 means the item needs no further processing */
		if (XFS_LSN_CMP(item_lsn, (xfs_lsn_t)-1) == 0)
			continue;

		/*
		 * if we are aborting the operation, no point in inserting the
		 * object into the AIL as we are in a shutdown situation.
		 */
		if (aborted) {
<<<<<<< HEAD
			ASSERT(XFS_FORCED_SHUTDOWN(ailp->xa_mount));
			IOP_UNPIN(lip, 1);
=======
			ASSERT(xlog_is_shutdown(ailp->ail_log));
			if (lip->li_ops->iop_unpin)
				lip->li_ops->iop_unpin(lip, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			continue;
		}

		if (item_lsn != commit_lsn) {

			/*
			 * Not a bulk update option due to unusual item_lsn.
			 * Push into AIL immediately, rechecking the lsn once
			 * we have the ail lock. Then unpin the item. This does
			 * not affect the AIL cursor the bulk insert path is
			 * using.
			 */
<<<<<<< HEAD
			spin_lock(&ailp->xa_lock);
			if (XFS_LSN_CMP(item_lsn, lip->li_lsn) > 0)
				xfs_trans_ail_update(ailp, lip, item_lsn);
			else
				spin_unlock(&ailp->xa_lock);
			IOP_UNPIN(lip, 0);
=======
			spin_lock(&ailp->ail_lock);
			if (XFS_LSN_CMP(item_lsn, lip->li_lsn) > 0)
				xfs_trans_ail_update(ailp, lip, item_lsn);
			else
				spin_unlock(&ailp->ail_lock);
			if (lip->li_ops->iop_unpin)
				lip->li_ops->iop_unpin(lip, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			continue;
		}

		/* Item is a candidate for bulk AIL insert.  */
		log_items[i++] = lv->lv_item;
		if (i >= LOG_ITEM_BATCH_SIZE) {
			xfs_log_item_batch_insert(ailp, &cur, log_items,
					LOG_ITEM_BATCH_SIZE, commit_lsn);
			i = 0;
		}
	}

	/* make sure we insert the remainder! */
	if (i)
		xfs_log_item_batch_insert(ailp, &cur, log_items, i, commit_lsn);

<<<<<<< HEAD
	spin_lock(&ailp->xa_lock);
	xfs_trans_ail_cursor_done(ailp, &cur);
	spin_unlock(&ailp->xa_lock);
=======
	spin_lock(&ailp->ail_lock);
	xfs_trans_ail_cursor_done(&cur);
	spin_unlock(&ailp->ail_lock);
}

/*
 * Sort transaction items prior to running precommit operations. This will
 * attempt to order the items such that they will always be locked in the same
 * order. Items that have no sort function are moved to the end of the list
 * and so are locked last.
 *
 * This may need refinement as different types of objects add sort functions.
 *
 * Function is more complex than it needs to be because we are comparing 64 bit
 * values and the function only returns 32 bit values.
 */
static int
xfs_trans_precommit_sort(
	void			*unused_arg,
	const struct list_head	*a,
	const struct list_head	*b)
{
	struct xfs_log_item	*lia = container_of(a,
					struct xfs_log_item, li_trans);
	struct xfs_log_item	*lib = container_of(b,
					struct xfs_log_item, li_trans);
	int64_t			diff;

	/*
	 * If both items are non-sortable, leave them alone. If only one is
	 * sortable, move the non-sortable item towards the end of the list.
	 */
	if (!lia->li_ops->iop_sort && !lib->li_ops->iop_sort)
		return 0;
	if (!lia->li_ops->iop_sort)
		return 1;
	if (!lib->li_ops->iop_sort)
		return -1;

	diff = lia->li_ops->iop_sort(lia) - lib->li_ops->iop_sort(lib);
	if (diff < 0)
		return -1;
	if (diff > 0)
		return 1;
	return 0;
}

/*
 * Run transaction precommit functions.
 *
 * If there is an error in any of the callouts, then stop immediately and
 * trigger a shutdown to abort the transaction. There is no recovery possible
 * from errors at this point as the transaction is dirty....
 */
static int
xfs_trans_run_precommits(
	struct xfs_trans	*tp)
{
	struct xfs_mount	*mp = tp->t_mountp;
	struct xfs_log_item	*lip, *n;
	int			error = 0;

	/*
	 * Sort the item list to avoid ABBA deadlocks with other transactions
	 * running precommit operations that lock multiple shared items such as
	 * inode cluster buffers.
	 */
	list_sort(NULL, &tp->t_items, xfs_trans_precommit_sort);

	/*
	 * Precommit operations can remove the log item from the transaction
	 * if the log item exists purely to delay modifications until they
	 * can be ordered against other operations. Hence we have to use
	 * list_for_each_entry_safe() here.
	 */
	list_for_each_entry_safe(lip, n, &tp->t_items, li_trans) {
		if (!test_bit(XFS_LI_DIRTY, &lip->li_flags))
			continue;
		if (lip->li_ops->iop_precommit) {
			error = lip->li_ops->iop_precommit(tp, lip);
			if (error)
				break;
		}
	}
	if (error)
		xfs_force_shutdown(mp, SHUTDOWN_CORRUPT_INCORE);
	return error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Commit the given transaction to the log.
 *
 * XFS disk error handling mechanism is not based on a typical
 * transaction abort mechanism. Logically after the filesystem
 * gets marked 'SHUTDOWN', we can't let any new transactions
 * be durable - ie. committed to disk - because some metadata might
 * be inconsistent. In such cases, this returns an error, and the
 * caller may assume that all locked objects joined to the transaction
 * have already been unlocked as if the commit had succeeded.
 * Do not reference the transaction structure after this call.
 */
<<<<<<< HEAD
int
xfs_trans_commit(
	struct xfs_trans	*tp,
	uint			flags)
{
	struct xfs_mount	*mp = tp->t_mountp;
	xfs_lsn_t		commit_lsn = -1;
	int			error = 0;
	int			log_flags = 0;
	int			sync = tp->t_flags & XFS_TRANS_SYNC;

	/*
	 * Determine whether this commit is releasing a permanent
	 * log reservation or not.
	 */
	if (flags & XFS_TRANS_RELEASE_LOG_RES) {
		ASSERT(tp->t_flags & XFS_TRANS_PERM_LOG_RES);
		log_flags = XFS_LOG_REL_PERM_RESERV;
=======
static int
__xfs_trans_commit(
	struct xfs_trans	*tp,
	bool			regrant)
{
	struct xfs_mount	*mp = tp->t_mountp;
	struct xlog		*log = mp->m_log;
	xfs_csn_t		commit_seq = 0;
	int			error = 0;
	int			sync = tp->t_flags & XFS_TRANS_SYNC;

	trace_xfs_trans_commit(tp, _RET_IP_);

	error = xfs_trans_run_precommits(tp);
	if (error) {
		if (tp->t_flags & XFS_TRANS_PERM_LOG_RES)
			xfs_defer_cancel(tp);
		goto out_unreserve;
	}

	/*
	 * Finish deferred items on final commit. Only permanent transactions
	 * should ever have deferred ops.
	 */
	WARN_ON_ONCE(!list_empty(&tp->t_dfops) &&
		     !(tp->t_flags & XFS_TRANS_PERM_LOG_RES));
	if (!regrant && (tp->t_flags & XFS_TRANS_PERM_LOG_RES)) {
		error = xfs_defer_finish_noroll(&tp);
		if (error)
			goto out_unreserve;

		/* Run precommits from final tx in defer chain. */
		error = xfs_trans_run_precommits(tp);
		if (error)
			goto out_unreserve;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/*
	 * If there is nothing to be logged by the transaction,
	 * then unlock all of the items associated with the
	 * transaction and free the transaction structure.
	 * Also make sure to return any reserved blocks to
	 * the free pool.
	 */
	if (!(tp->t_flags & XFS_TRANS_DIRTY))
		goto out_unreserve;

<<<<<<< HEAD
	if (XFS_FORCED_SHUTDOWN(mp)) {
		error = XFS_ERROR(EIO);
=======
	/*
	 * We must check against log shutdown here because we cannot abort log
	 * items and leave them dirty, inconsistent and unpinned in memory while
	 * the log is active. This leaves them open to being written back to
	 * disk, and that will lead to on-disk corruption.
	 */
	if (xlog_is_shutdown(log)) {
		error = -EIO;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out_unreserve;
	}

	ASSERT(tp->t_ticket != NULL);

	/*
	 * If we need to update the superblock, then do it now.
	 */
	if (tp->t_flags & XFS_TRANS_SB_DIRTY)
		xfs_trans_apply_sb_deltas(tp);
	xfs_trans_apply_dquot_deltas(tp);

<<<<<<< HEAD
	error = xfs_log_commit_cil(mp, tp, &commit_lsn, flags);
	if (error == ENOMEM) {
		xfs_force_shutdown(mp, SHUTDOWN_LOG_IO_ERROR);
		error = XFS_ERROR(EIO);
		goto out_unreserve;
	}

	current_restore_flags_nested(&tp->t_pflags, PF_FSTRANS);
=======
	xlog_cil_commit(log, tp, &commit_seq, regrant);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	xfs_trans_free(tp);

	/*
	 * If the transaction needs to be synchronous, then force the
	 * log out now and wait for it.
	 */
	if (sync) {
<<<<<<< HEAD
		if (!error) {
			error = _xfs_log_force_lsn(mp, commit_lsn,
				      XFS_LOG_SYNC, NULL);
		}
		XFS_STATS_INC(xs_trans_sync);
	} else {
		XFS_STATS_INC(xs_trans_async);
=======
		error = xfs_log_force_seq(mp, commit_seq, XFS_LOG_SYNC, NULL);
		XFS_STATS_INC(mp, xs_trans_sync);
	} else {
		XFS_STATS_INC(mp, xs_trans_async);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return error;

out_unreserve:
	xfs_trans_unreserve_and_mod_sb(tp);

	/*
	 * It is indeed possible for the transaction to be not dirty but
	 * the dqinfo portion to be.  All that means is that we have some
	 * (non-persistent) quota reservations that need to be unreserved.
	 */
	xfs_trans_unreserve_and_mod_dquots(tp);
	if (tp->t_ticket) {
<<<<<<< HEAD
		commit_lsn = xfs_log_done(mp, tp->t_ticket, NULL, log_flags);
		if (commit_lsn == -1 && !error)
			error = XFS_ERROR(EIO);
	}
	current_restore_flags_nested(&tp->t_pflags, PF_FSTRANS);
	xfs_trans_free_items(tp, NULLCOMMITLSN, error ? XFS_TRANS_ABORT : 0);
	xfs_trans_free(tp);

	XFS_STATS_INC(xs_trans_empty);
	return error;
}

/*
 * Unlock all of the transaction's items and free the transaction.
 * The transaction must not have modified any of its items, because
 * there is no way to restore them to their previous state.
 *
 * If the transaction has made a log reservation, make sure to release
 * it as well.
 */
void
xfs_trans_cancel(
	xfs_trans_t		*tp,
	int			flags)
{
	int			log_flags;
	xfs_mount_t		*mp = tp->t_mountp;

	/*
	 * See if the caller is being too lazy to figure out if
	 * the transaction really needs an abort.
	 */
	if ((flags & XFS_TRANS_ABORT) && !(tp->t_flags & XFS_TRANS_DIRTY))
		flags &= ~XFS_TRANS_ABORT;
	/*
	 * See if the caller is relying on us to shut down the
	 * filesystem.  This happens in paths where we detect
	 * corruption and decide to give up.
	 */
	if ((tp->t_flags & XFS_TRANS_DIRTY) && !XFS_FORCED_SHUTDOWN(mp)) {
=======
		if (regrant && !xlog_is_shutdown(log))
			xfs_log_ticket_regrant(log, tp->t_ticket);
		else
			xfs_log_ticket_ungrant(log, tp->t_ticket);
		tp->t_ticket = NULL;
	}
	xfs_trans_free_items(tp, !!error);
	xfs_trans_free(tp);

	XFS_STATS_INC(mp, xs_trans_empty);
	return error;
}

int
xfs_trans_commit(
	struct xfs_trans	*tp)
{
	return __xfs_trans_commit(tp, false);
}

/*
 * Unlock all of the transaction's items and free the transaction.  If the
 * transaction is dirty, we must shut down the filesystem because there is no
 * way to restore them to their previous state.
 *
 * If the transaction has made a log reservation, make sure to release it as
 * well.
 *
 * This is a high level function (equivalent to xfs_trans_commit()) and so can
 * be called after the transaction has effectively been aborted due to the mount
 * being shut down. However, if the mount has not been shut down and the
 * transaction is dirty we will shut the mount down and, in doing so, that
 * guarantees that the log is shut down, too. Hence we don't need to be as
 * careful with shutdown state and dirty items here as we need to be in
 * xfs_trans_commit().
 */
void
xfs_trans_cancel(
	struct xfs_trans	*tp)
{
	struct xfs_mount	*mp = tp->t_mountp;
	struct xlog		*log = mp->m_log;
	bool			dirty = (tp->t_flags & XFS_TRANS_DIRTY);

	trace_xfs_trans_cancel(tp, _RET_IP_);

	/*
	 * It's never valid to cancel a transaction with deferred ops attached,
	 * because the transaction is effectively dirty.  Complain about this
	 * loudly before freeing the in-memory defer items and shutting down the
	 * filesystem.
	 */
	if (!list_empty(&tp->t_dfops)) {
		ASSERT(tp->t_flags & XFS_TRANS_PERM_LOG_RES);
		dirty = true;
		xfs_defer_cancel(tp);
	}

	/*
	 * See if the caller is relying on us to shut down the filesystem. We
	 * only want an error report if there isn't already a shutdown in
	 * progress, so we only need to check against the mount shutdown state
	 * here.
	 */
	if (dirty && !xfs_is_shutdown(mp)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		XFS_ERROR_REPORT("xfs_trans_cancel", XFS_ERRLEVEL_LOW, mp);
		xfs_force_shutdown(mp, SHUTDOWN_CORRUPT_INCORE);
	}
#ifdef DEBUG
<<<<<<< HEAD
	if (!(flags & XFS_TRANS_ABORT) && !XFS_FORCED_SHUTDOWN(mp)) {
		struct xfs_log_item_desc *lidp;

		list_for_each_entry(lidp, &tp->t_items, lid_trans)
			ASSERT(!(lidp->lid_item->li_type == XFS_LI_EFD));
=======
	/* Log items need to be consistent until the log is shut down. */
	if (!dirty && !xlog_is_shutdown(log)) {
		struct xfs_log_item *lip;

		list_for_each_entry(lip, &tp->t_items, li_trans)
			ASSERT(!xlog_item_is_intent_done(lip));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
#endif
	xfs_trans_unreserve_and_mod_sb(tp);
	xfs_trans_unreserve_and_mod_dquots(tp);

	if (tp->t_ticket) {
<<<<<<< HEAD
		if (flags & XFS_TRANS_RELEASE_LOG_RES) {
			ASSERT(tp->t_flags & XFS_TRANS_PERM_LOG_RES);
			log_flags = XFS_LOG_REL_PERM_RESERV;
		} else {
			log_flags = 0;
		}
		xfs_log_done(mp, tp->t_ticket, NULL, log_flags);
	}

	/* mark this thread as no longer being in a transaction */
	current_restore_flags_nested(&tp->t_pflags, PF_FSTRANS);

	xfs_trans_free_items(tp, NULLCOMMITLSN, flags);
=======
		xfs_log_ticket_ungrant(log, tp->t_ticket);
		tp->t_ticket = NULL;
	}

	xfs_trans_free_items(tp, dirty);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	xfs_trans_free(tp);
}

/*
 * Roll from one trans in the sequence of PERMANENT transactions to
 * the next: permanent transactions are only flushed out when
<<<<<<< HEAD
 * committed with XFS_TRANS_RELEASE_LOG_RES, but we still want as soon
=======
 * committed with xfs_trans_commit(), but we still want as soon
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * as possible to let chunks of it go to the log. So we commit the
 * chunk we've been working on and get a new transaction to continue.
 */
int
xfs_trans_roll(
<<<<<<< HEAD
	struct xfs_trans	**tpp,
	struct xfs_inode	*dp)
{
	struct xfs_trans	*trans;
	unsigned int		logres, count;
	int			error;

	/*
	 * Ensure that the inode is always logged.
	 */
	trans = *tpp;
	xfs_trans_log_inode(trans, dp, XFS_ILOG_CORE);
=======
	struct xfs_trans	**tpp)
{
	struct xfs_trans	*trans = *tpp;
	struct xfs_trans_res	tres;
	int			error;

	trace_xfs_trans_roll(trans, _RET_IP_);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Copy the critical parameters from one trans to the next.
	 */
<<<<<<< HEAD
	logres = trans->t_log_res;
	count = trans->t_log_count;
=======
	tres.tr_logres = trans->t_log_res;
	tres.tr_logcount = trans->t_log_count;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	*tpp = xfs_trans_dup(trans);

	/*
	 * Commit the current transaction.
	 * If this commit failed, then it'd just unlock those items that
	 * are not marked ihold. That also means that a filesystem shutdown
	 * is in progress. The caller takes the responsibility to cancel
	 * the duplicate transaction that gets returned.
	 */
<<<<<<< HEAD
	error = xfs_trans_commit(trans, 0);
	if (error)
		return (error);

	trans = *tpp;

	/*
	 * transaction commit worked ok so we can drop the extra ticket
	 * reference that we gained in xfs_trans_dup()
	 */
	xfs_log_ticket_put(trans->t_ticket);


	/*
	 * Reserve space in the log for th next transaction.
=======
	error = __xfs_trans_commit(trans, true);
	if (error)
		return error;

	/*
	 * Reserve space in the log for the next transaction.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * This also pushes items in the "AIL", the list of logged items,
	 * out to disk if they are taking up space at the tail of the log
	 * that we want to use.  This requires that either nothing be locked
	 * across this call, or that anything that is locked be logged in
	 * the prior and the next transactions.
	 */
<<<<<<< HEAD
	error = xfs_trans_reserve(trans, 0, logres, 0,
				  XFS_TRANS_PERM_LOG_RES, count);
	/*
	 *  Ensure that the inode is in the new transaction and locked.
	 */
	if (error)
		return error;

	xfs_trans_ijoin(trans, dp, 0);
	return 0;
}
=======
	tres.tr_logflags = XFS_TRANS_PERM_LOG_RES;
	return xfs_trans_reserve(*tpp, &tres, 0, 0);
}

/*
 * Allocate an transaction, lock and join the inode to it, and reserve quota.
 *
 * The caller must ensure that the on-disk dquots attached to this inode have
 * already been allocated and initialized.  The caller is responsible for
 * releasing ILOCK_EXCL if a new transaction is returned.
 */
int
xfs_trans_alloc_inode(
	struct xfs_inode	*ip,
	struct xfs_trans_res	*resv,
	unsigned int		dblocks,
	unsigned int		rblocks,
	bool			force,
	struct xfs_trans	**tpp)
{
	struct xfs_trans	*tp;
	struct xfs_mount	*mp = ip->i_mount;
	bool			retried = false;
	int			error;

retry:
	error = xfs_trans_alloc(mp, resv, dblocks,
			xfs_extlen_to_rtxlen(mp, rblocks),
			force ? XFS_TRANS_RESERVE : 0, &tp);
	if (error)
		return error;

	xfs_ilock(ip, XFS_ILOCK_EXCL);
	xfs_trans_ijoin(tp, ip, 0);

	error = xfs_qm_dqattach_locked(ip, false);
	if (error) {
		/* Caller should have allocated the dquots! */
		ASSERT(error != -ENOENT);
		goto out_cancel;
	}

	error = xfs_trans_reserve_quota_nblks(tp, ip, dblocks, rblocks, force);
	if ((error == -EDQUOT || error == -ENOSPC) && !retried) {
		xfs_trans_cancel(tp);
		xfs_iunlock(ip, XFS_ILOCK_EXCL);
		xfs_blockgc_free_quota(ip, 0);
		retried = true;
		goto retry;
	}
	if (error)
		goto out_cancel;

	*tpp = tp;
	return 0;

out_cancel:
	xfs_trans_cancel(tp);
	xfs_iunlock(ip, XFS_ILOCK_EXCL);
	return error;
}

/*
 * Try to reserve more blocks for a transaction.
 *
 * This is for callers that need to attach resources to a transaction, scan
 * those resources to determine the space reservation requirements, and then
 * modify the attached resources.  In other words, online repair.  This can
 * fail due to ENOSPC, so the caller must be able to cancel the transaction
 * without shutting down the fs.
 */
int
xfs_trans_reserve_more(
	struct xfs_trans	*tp,
	unsigned int		blocks,
	unsigned int		rtextents)
{
	struct xfs_trans_res	resv = { };

	return xfs_trans_reserve(tp, &resv, blocks, rtextents);
}

/*
 * Try to reserve more blocks and file quota for a transaction.  Same
 * conditions of usage as xfs_trans_reserve_more.
 */
int
xfs_trans_reserve_more_inode(
	struct xfs_trans	*tp,
	struct xfs_inode	*ip,
	unsigned int		dblocks,
	unsigned int		rblocks,
	bool			force_quota)
{
	struct xfs_trans_res	resv = { };
	struct xfs_mount	*mp = ip->i_mount;
	unsigned int		rtx = xfs_extlen_to_rtxlen(mp, rblocks);
	int			error;

	xfs_assert_ilocked(ip, XFS_ILOCK_EXCL);

	error = xfs_trans_reserve(tp, &resv, dblocks, rtx);
	if (error)
		return error;

	if (!XFS_IS_QUOTA_ON(mp) || xfs_is_quota_inode(&mp->m_sb, ip->i_ino))
		return 0;

	if (tp->t_flags & XFS_TRANS_RESERVE)
		force_quota = true;

	error = xfs_trans_reserve_quota_nblks(tp, ip, dblocks, rblocks,
			force_quota);
	if (!error)
		return 0;

	/* Quota failed, give back the new reservation. */
	xfs_mod_fdblocks(mp, dblocks, tp->t_flags & XFS_TRANS_RESERVE);
	tp->t_blk_res -= dblocks;
	xfs_mod_frextents(mp, rtx);
	tp->t_rtx_res -= rtx;
	return error;
}

/*
 * Allocate an transaction in preparation for inode creation by reserving quota
 * against the given dquots.  Callers are not required to hold any inode locks.
 */
int
xfs_trans_alloc_icreate(
	struct xfs_mount	*mp,
	struct xfs_trans_res	*resv,
	struct xfs_dquot	*udqp,
	struct xfs_dquot	*gdqp,
	struct xfs_dquot	*pdqp,
	unsigned int		dblocks,
	struct xfs_trans	**tpp)
{
	struct xfs_trans	*tp;
	bool			retried = false;
	int			error;

retry:
	error = xfs_trans_alloc(mp, resv, dblocks, 0, 0, &tp);
	if (error)
		return error;

	error = xfs_trans_reserve_quota_icreate(tp, udqp, gdqp, pdqp, dblocks);
	if ((error == -EDQUOT || error == -ENOSPC) && !retried) {
		xfs_trans_cancel(tp);
		xfs_blockgc_free_dquots(mp, udqp, gdqp, pdqp, 0);
		retried = true;
		goto retry;
	}
	if (error) {
		xfs_trans_cancel(tp);
		return error;
	}

	*tpp = tp;
	return 0;
}

/*
 * Allocate an transaction, lock and join the inode to it, and reserve quota
 * in preparation for inode attribute changes that include uid, gid, or prid
 * changes.
 *
 * The caller must ensure that the on-disk dquots attached to this inode have
 * already been allocated and initialized.  The ILOCK will be dropped when the
 * transaction is committed or cancelled.
 */
int
xfs_trans_alloc_ichange(
	struct xfs_inode	*ip,
	struct xfs_dquot	*new_udqp,
	struct xfs_dquot	*new_gdqp,
	struct xfs_dquot	*new_pdqp,
	bool			force,
	struct xfs_trans	**tpp)
{
	struct xfs_trans	*tp;
	struct xfs_mount	*mp = ip->i_mount;
	struct xfs_dquot	*udqp;
	struct xfs_dquot	*gdqp;
	struct xfs_dquot	*pdqp;
	bool			retried = false;
	int			error;

retry:
	error = xfs_trans_alloc(mp, &M_RES(mp)->tr_ichange, 0, 0, 0, &tp);
	if (error)
		return error;

	xfs_ilock(ip, XFS_ILOCK_EXCL);
	xfs_trans_ijoin(tp, ip, XFS_ILOCK_EXCL);

	error = xfs_qm_dqattach_locked(ip, false);
	if (error) {
		/* Caller should have allocated the dquots! */
		ASSERT(error != -ENOENT);
		goto out_cancel;
	}

	/*
	 * For each quota type, skip quota reservations if the inode's dquots
	 * now match the ones that came from the caller, or the caller didn't
	 * pass one in.  The inode's dquots can change if we drop the ILOCK to
	 * perform a blockgc scan, so we must preserve the caller's arguments.
	 */
	udqp = (new_udqp != ip->i_udquot) ? new_udqp : NULL;
	gdqp = (new_gdqp != ip->i_gdquot) ? new_gdqp : NULL;
	pdqp = (new_pdqp != ip->i_pdquot) ? new_pdqp : NULL;
	if (udqp || gdqp || pdqp) {
		unsigned int	qflags = XFS_QMOPT_RES_REGBLKS;

		if (force)
			qflags |= XFS_QMOPT_FORCE_RES;

		/*
		 * Reserve enough quota to handle blocks on disk and reserved
		 * for a delayed allocation.  We'll actually transfer the
		 * delalloc reservation between dquots at chown time, even
		 * though that part is only semi-transactional.
		 */
		error = xfs_trans_reserve_quota_bydquots(tp, mp, udqp, gdqp,
				pdqp, ip->i_nblocks + ip->i_delayed_blks,
				1, qflags);
		if ((error == -EDQUOT || error == -ENOSPC) && !retried) {
			xfs_trans_cancel(tp);
			xfs_blockgc_free_dquots(mp, udqp, gdqp, pdqp, 0);
			retried = true;
			goto retry;
		}
		if (error)
			goto out_cancel;
	}

	*tpp = tp;
	return 0;

out_cancel:
	xfs_trans_cancel(tp);
	return error;
}

/*
 * Allocate an transaction, lock and join the directory and child inodes to it,
 * and reserve quota for a directory update.  If there isn't sufficient space,
 * @dblocks will be set to zero for a reservationless directory update and
 * @nospace_error will be set to a negative errno describing the space
 * constraint we hit.
 *
 * The caller must ensure that the on-disk dquots attached to this inode have
 * already been allocated and initialized.  The ILOCKs will be dropped when the
 * transaction is committed or cancelled.
 */
int
xfs_trans_alloc_dir(
	struct xfs_inode	*dp,
	struct xfs_trans_res	*resv,
	struct xfs_inode	*ip,
	unsigned int		*dblocks,
	struct xfs_trans	**tpp,
	int			*nospace_error)
{
	struct xfs_trans	*tp;
	struct xfs_mount	*mp = ip->i_mount;
	unsigned int		resblks;
	bool			retried = false;
	int			error;

retry:
	*nospace_error = 0;
	resblks = *dblocks;
	error = xfs_trans_alloc(mp, resv, resblks, 0, 0, &tp);
	if (error == -ENOSPC) {
		*nospace_error = error;
		resblks = 0;
		error = xfs_trans_alloc(mp, resv, resblks, 0, 0, &tp);
	}
	if (error)
		return error;

	xfs_lock_two_inodes(dp, XFS_ILOCK_EXCL, ip, XFS_ILOCK_EXCL);

	xfs_trans_ijoin(tp, dp, XFS_ILOCK_EXCL);
	xfs_trans_ijoin(tp, ip, XFS_ILOCK_EXCL);

	error = xfs_qm_dqattach_locked(dp, false);
	if (error) {
		/* Caller should have allocated the dquots! */
		ASSERT(error != -ENOENT);
		goto out_cancel;
	}

	error = xfs_qm_dqattach_locked(ip, false);
	if (error) {
		/* Caller should have allocated the dquots! */
		ASSERT(error != -ENOENT);
		goto out_cancel;
	}

	if (resblks == 0)
		goto done;

	error = xfs_trans_reserve_quota_nblks(tp, dp, resblks, 0, false);
	if (error == -EDQUOT || error == -ENOSPC) {
		if (!retried) {
			xfs_trans_cancel(tp);
			xfs_blockgc_free_quota(dp, 0);
			retried = true;
			goto retry;
		}

		*nospace_error = error;
		resblks = 0;
		error = 0;
	}
	if (error)
		goto out_cancel;

done:
	*tpp = tp;
	*dblocks = resblks;
	return 0;

out_cancel:
	xfs_trans_cancel(tp);
	return error;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
