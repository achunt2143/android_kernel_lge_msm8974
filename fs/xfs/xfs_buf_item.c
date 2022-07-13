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
#include "xfs_mount.h"
#include "xfs_buf_item.h"
#include "xfs_trans_priv.h"
#include "xfs_error.h"
#include "xfs_trace.h"


kmem_zone_t	*xfs_buf_item_zone;
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
#include "xfs_trans.h"
#include "xfs_trans_priv.h"
#include "xfs_buf_item.h"
#include "xfs_inode.h"
#include "xfs_inode_item.h"
#include "xfs_quota.h"
#include "xfs_dquot_item.h"
#include "xfs_dquot.h"
#include "xfs_trace.h"
#include "xfs_log.h"
#include "xfs_log_priv.h"


struct kmem_cache	*xfs_buf_item_cache;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline struct xfs_buf_log_item *BUF_ITEM(struct xfs_log_item *lip)
{
	return container_of(lip, struct xfs_buf_log_item, bli_item);
}

<<<<<<< HEAD

#ifdef XFS_TRANS_DEBUG
/*
 * This function uses an alternate strategy for tracking the bytes
 * that the user requests to be logged.  This can then be used
 * in conjunction with the bli_orig array in the buf log item to
 * catch bugs in our callers' code.
 *
 * We also double check the bits set in xfs_buf_item_log using a
 * simple algorithm to check that every byte is accounted for.
 */
STATIC void
xfs_buf_item_log_debug(
	xfs_buf_log_item_t	*bip,
	uint			first,
	uint			last)
{
	uint	x;
	uint	byte;
	uint	nbytes;
	uint	chunk_num;
	uint	word_num;
	uint	bit_num;
	uint	bit_set;
	uint	*wordp;

	ASSERT(bip->bli_logged != NULL);
	byte = first;
	nbytes = last - first + 1;
	bfset(bip->bli_logged, first, nbytes);
	for (x = 0; x < nbytes; x++) {
		chunk_num = byte >> XFS_BLF_SHIFT;
		word_num = chunk_num >> BIT_TO_WORD_SHIFT;
		bit_num = chunk_num & (NBWORD - 1);
		wordp = &(bip->bli_format.blf_data_map[word_num]);
		bit_set = *wordp & (1 << bit_num);
		ASSERT(bit_set);
		byte++;
	}
}

/*
 * This function is called when we flush something into a buffer without
 * logging it.  This happens for things like inodes which are logged
 * separately from the buffer.
 */
void
xfs_buf_item_flush_log_debug(
	xfs_buf_t	*bp,
	uint		first,
	uint		last)
{
	xfs_buf_log_item_t	*bip = bp->b_fspriv;
	uint			nbytes;

	if (bip == NULL || (bip->bli_item.li_type != XFS_LI_BUF))
		return;

	ASSERT(bip->bli_logged != NULL);
	nbytes = last - first + 1;
	bfset(bip->bli_logged, first, nbytes);
}

/*
 * This function is called to verify that our callers have logged
 * all the bytes that they changed.
 *
 * It does this by comparing the original copy of the buffer stored in
 * the buf log item's bli_orig array to the current copy of the buffer
 * and ensuring that all bytes which mismatch are set in the bli_logged
 * array of the buf log item.
 */
STATIC void
xfs_buf_item_log_check(
	xfs_buf_log_item_t	*bip)
{
	char		*orig;
	char		*buffer;
	int		x;
	xfs_buf_t	*bp;

	ASSERT(bip->bli_orig != NULL);
	ASSERT(bip->bli_logged != NULL);

	bp = bip->bli_buf;
	ASSERT(XFS_BUF_COUNT(bp) > 0);
	ASSERT(bp->b_addr != NULL);
	orig = bip->bli_orig;
	buffer = bp->b_addr;
	for (x = 0; x < XFS_BUF_COUNT(bp); x++) {
		if (orig[x] != buffer[x] && !btst(bip->bli_logged, x)) {
			xfs_emerg(bp->b_mount,
				"%s: bip %x buffer %x orig %x index %d",
				__func__, bip, bp, orig, x);
			ASSERT(0);
		}
	}
}
#else
#define		xfs_buf_item_log_debug(x,y,z)
#define		xfs_buf_item_log_check(x)
#endif

STATIC void	xfs_buf_do_callbacks(struct xfs_buf *bp);

/*
 * This returns the number of log iovecs needed to log the
 * given buf log item.
 *
 * It calculates this as 1 iovec for the buf log format structure
 * and 1 for each stretch of non-contiguous chunks to be logged.
 * Contiguous chunks are logged in a single iovec.
 *
 * If the XFS_BLI_STALE flag has been set, then log nothing.
 */
STATIC uint
xfs_buf_item_size(
	struct xfs_log_item	*lip)
{
	struct xfs_buf_log_item	*bip = BUF_ITEM(lip);
	struct xfs_buf		*bp = bip->bli_buf;
	uint			nvecs;
	int			next_bit;
	int			last_bit;

	ASSERT(atomic_read(&bip->bli_refcount) > 0);
	if (bip->bli_flags & XFS_BLI_STALE) {
		/*
		 * The buffer is stale, so all we need to log
		 * is the buf log format structure with the
		 * cancel flag in it.
		 */
		trace_xfs_buf_item_size_stale(bip);
		ASSERT(bip->bli_format.blf_flags & XFS_BLF_CANCEL);
		return 1;
	}

	ASSERT(bip->bli_flags & XFS_BLI_LOGGED);
	nvecs = 1;
	last_bit = xfs_next_bit(bip->bli_format.blf_data_map,
					 bip->bli_format.blf_map_size, 0);
	ASSERT(last_bit != -1);
	nvecs++;
=======
/* Is this log iovec plausibly large enough to contain the buffer log format? */
bool
xfs_buf_log_check_iovec(
	struct xfs_log_iovec		*iovec)
{
	struct xfs_buf_log_format	*blfp = iovec->i_addr;
	char				*bmp_end;
	char				*item_end;

	if (offsetof(struct xfs_buf_log_format, blf_data_map) > iovec->i_len)
		return false;

	item_end = (char *)iovec->i_addr + iovec->i_len;
	bmp_end = (char *)&blfp->blf_data_map[blfp->blf_map_size];
	return bmp_end <= item_end;
}

static inline int
xfs_buf_log_format_size(
	struct xfs_buf_log_format *blfp)
{
	return offsetof(struct xfs_buf_log_format, blf_data_map) +
			(blfp->blf_map_size * sizeof(blfp->blf_data_map[0]));
}

static inline bool
xfs_buf_item_straddle(
	struct xfs_buf		*bp,
	uint			offset,
	int			first_bit,
	int			nbits)
{
	void			*first, *last;

	first = xfs_buf_offset(bp, offset + (first_bit << XFS_BLF_SHIFT));
	last = xfs_buf_offset(bp,
			offset + ((first_bit + nbits) << XFS_BLF_SHIFT));

	if (last - first != nbits * XFS_BLF_CHUNK)
		return true;
	return false;
}

/*
 * Return the number of log iovecs and space needed to log the given buf log
 * item segment.
 *
 * It calculates this as 1 iovec for the buf log format structure and 1 for each
 * stretch of non-contiguous chunks to be logged.  Contiguous chunks are logged
 * in a single iovec.
 */
STATIC void
xfs_buf_item_size_segment(
	struct xfs_buf_log_item		*bip,
	struct xfs_buf_log_format	*blfp,
	uint				offset,
	int				*nvecs,
	int				*nbytes)
{
	struct xfs_buf			*bp = bip->bli_buf;
	int				first_bit;
	int				nbits;
	int				next_bit;
	int				last_bit;

	first_bit = xfs_next_bit(blfp->blf_data_map, blfp->blf_map_size, 0);
	if (first_bit == -1)
		return;

	(*nvecs)++;
	*nbytes += xfs_buf_log_format_size(blfp);

	do {
		nbits = xfs_contig_bits(blfp->blf_data_map,
					blfp->blf_map_size, first_bit);
		ASSERT(nbits > 0);

		/*
		 * Straddling a page is rare because we don't log contiguous
		 * chunks of unmapped buffers anywhere.
		 */
		if (nbits > 1 &&
		    xfs_buf_item_straddle(bp, offset, first_bit, nbits))
			goto slow_scan;

		(*nvecs)++;
		*nbytes += nbits * XFS_BLF_CHUNK;

		/*
		 * This takes the bit number to start looking from and
		 * returns the next set bit from there.  It returns -1
		 * if there are no more bits set or the start bit is
		 * beyond the end of the bitmap.
		 */
		first_bit = xfs_next_bit(blfp->blf_data_map, blfp->blf_map_size,
					(uint)first_bit + nbits + 1);
	} while (first_bit != -1);

	return;

slow_scan:
	/* Count the first bit we jumped out of the above loop from */
	(*nvecs)++;
	*nbytes += XFS_BLF_CHUNK;
	last_bit = first_bit;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	while (last_bit != -1) {
		/*
		 * This takes the bit number to start looking from and
		 * returns the next set bit from there.  It returns -1
		 * if there are no more bits set or the start bit is
		 * beyond the end of the bitmap.
		 */
<<<<<<< HEAD
		next_bit = xfs_next_bit(bip->bli_format.blf_data_map,
						 bip->bli_format.blf_map_size,
						 last_bit + 1);
=======
		next_bit = xfs_next_bit(blfp->blf_data_map, blfp->blf_map_size,
					last_bit + 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * If we run out of bits, leave the loop,
		 * else if we find a new set of bits bump the number of vecs,
		 * else keep scanning the current set of bits.
		 */
		if (next_bit == -1) {
<<<<<<< HEAD
			last_bit = -1;
		} else if (next_bit != last_bit + 1) {
			last_bit = next_bit;
			nvecs++;
		} else if (xfs_buf_offset(bp, next_bit * XFS_BLF_CHUNK) !=
			   (xfs_buf_offset(bp, last_bit * XFS_BLF_CHUNK) +
			    XFS_BLF_CHUNK)) {
			last_bit = next_bit;
			nvecs++;
		} else {
			last_bit++;
		}
	}

	trace_xfs_buf_item_size(bip);
	return nvecs;
=======
			break;
		} else if (next_bit != last_bit + 1 ||
		           xfs_buf_item_straddle(bp, offset, first_bit, nbits)) {
			last_bit = next_bit;
			first_bit = next_bit;
			(*nvecs)++;
			nbits = 1;
		} else {
			last_bit++;
			nbits++;
		}
		*nbytes += XFS_BLF_CHUNK;
	}
}

/*
 * Return the number of log iovecs and space needed to log the given buf log
 * item.
 *
 * Discontiguous buffers need a format structure per region that is being
 * logged. This makes the changes in the buffer appear to log recovery as though
 * they came from separate buffers, just like would occur if multiple buffers
 * were used instead of a single discontiguous buffer. This enables
 * discontiguous buffers to be in-memory constructs, completely transparent to
 * what ends up on disk.
 *
 * If the XFS_BLI_STALE flag has been set, then log nothing but the buf log
 * format structures. If the item has previously been logged and has dirty
 * regions, we do not relog them in stale buffers. This has the effect of
 * reducing the size of the relogged item by the amount of dirty data tracked
 * by the log item. This can result in the committing transaction reducing the
 * amount of space being consumed by the CIL.
 */
STATIC void
xfs_buf_item_size(
	struct xfs_log_item	*lip,
	int			*nvecs,
	int			*nbytes)
{
	struct xfs_buf_log_item	*bip = BUF_ITEM(lip);
	struct xfs_buf		*bp = bip->bli_buf;
	int			i;
	int			bytes;
	uint			offset = 0;

	ASSERT(atomic_read(&bip->bli_refcount) > 0);
	if (bip->bli_flags & XFS_BLI_STALE) {
		/*
		 * The buffer is stale, so all we need to log is the buf log
		 * format structure with the cancel flag in it as we are never
		 * going to replay the changes tracked in the log item.
		 */
		trace_xfs_buf_item_size_stale(bip);
		ASSERT(bip->__bli_format.blf_flags & XFS_BLF_CANCEL);
		*nvecs += bip->bli_format_count;
		for (i = 0; i < bip->bli_format_count; i++) {
			*nbytes += xfs_buf_log_format_size(&bip->bli_formats[i]);
		}
		return;
	}

	ASSERT(bip->bli_flags & XFS_BLI_LOGGED);

	if (bip->bli_flags & XFS_BLI_ORDERED) {
		/*
		 * The buffer has been logged just to order it. It is not being
		 * included in the transaction commit, so no vectors are used at
		 * all.
		 */
		trace_xfs_buf_item_size_ordered(bip);
		*nvecs = XFS_LOG_VEC_ORDERED;
		return;
	}

	/*
	 * The vector count is based on the number of buffer vectors we have
	 * dirty bits in. This will only be greater than one when we have a
	 * compound buffer with more than one segment dirty. Hence for compound
	 * buffers we need to track which segment the dirty bits correspond to,
	 * and when we move from one segment to the next increment the vector
	 * count for the extra buf log format structure that will need to be
	 * written.
	 */
	bytes = 0;
	for (i = 0; i < bip->bli_format_count; i++) {
		xfs_buf_item_size_segment(bip, &bip->bli_formats[i], offset,
					  nvecs, &bytes);
		offset += BBTOB(bp->b_maps[i].bm_len);
	}

	/*
	 * Round up the buffer size required to minimise the number of memory
	 * allocations that need to be done as this item grows when relogged by
	 * repeated modifications.
	 */
	*nbytes = round_up(bytes, 512);
	trace_xfs_buf_item_size(bip);
}

static inline void
xfs_buf_item_copy_iovec(
	struct xfs_log_vec	*lv,
	struct xfs_log_iovec	**vecp,
	struct xfs_buf		*bp,
	uint			offset,
	int			first_bit,
	uint			nbits)
{
	offset += first_bit * XFS_BLF_CHUNK;
	xlog_copy_iovec(lv, vecp, XLOG_REG_TYPE_BCHUNK,
			xfs_buf_offset(bp, offset),
			nbits * XFS_BLF_CHUNK);
}

static void
xfs_buf_item_format_segment(
	struct xfs_buf_log_item	*bip,
	struct xfs_log_vec	*lv,
	struct xfs_log_iovec	**vecp,
	uint			offset,
	struct xfs_buf_log_format *blfp)
{
	struct xfs_buf		*bp = bip->bli_buf;
	uint			base_size;
	int			first_bit;
	int			last_bit;
	int			next_bit;
	uint			nbits;

	/* copy the flags across from the base format item */
	blfp->blf_flags = bip->__bli_format.blf_flags;

	/*
	 * Base size is the actual size of the ondisk structure - it reflects
	 * the actual size of the dirty bitmap rather than the size of the in
	 * memory structure.
	 */
	base_size = xfs_buf_log_format_size(blfp);

	first_bit = xfs_next_bit(blfp->blf_data_map, blfp->blf_map_size, 0);
	if (!(bip->bli_flags & XFS_BLI_STALE) && first_bit == -1) {
		/*
		 * If the map is not be dirty in the transaction, mark
		 * the size as zero and do not advance the vector pointer.
		 */
		return;
	}

	blfp = xlog_copy_iovec(lv, vecp, XLOG_REG_TYPE_BFORMAT, blfp, base_size);
	blfp->blf_size = 1;

	if (bip->bli_flags & XFS_BLI_STALE) {
		/*
		 * The buffer is stale, so all we need to log
		 * is the buf log format structure with the
		 * cancel flag in it.
		 */
		trace_xfs_buf_item_format_stale(bip);
		ASSERT(blfp->blf_flags & XFS_BLF_CANCEL);
		return;
	}


	/*
	 * Fill in an iovec for each set of contiguous chunks.
	 */
	do {
		ASSERT(first_bit >= 0);
		nbits = xfs_contig_bits(blfp->blf_data_map,
					blfp->blf_map_size, first_bit);
		ASSERT(nbits > 0);

		/*
		 * Straddling a page is rare because we don't log contiguous
		 * chunks of unmapped buffers anywhere.
		 */
		if (nbits > 1 &&
		    xfs_buf_item_straddle(bp, offset, first_bit, nbits))
			goto slow_scan;

		xfs_buf_item_copy_iovec(lv, vecp, bp, offset,
					first_bit, nbits);
		blfp->blf_size++;

		/*
		 * This takes the bit number to start looking from and
		 * returns the next set bit from there.  It returns -1
		 * if there are no more bits set or the start bit is
		 * beyond the end of the bitmap.
		 */
		first_bit = xfs_next_bit(blfp->blf_data_map, blfp->blf_map_size,
					(uint)first_bit + nbits + 1);
	} while (first_bit != -1);

	return;

slow_scan:
	ASSERT(bp->b_addr == NULL);
	last_bit = first_bit;
	nbits = 1;
	for (;;) {
		/*
		 * This takes the bit number to start looking from and
		 * returns the next set bit from there.  It returns -1
		 * if there are no more bits set or the start bit is
		 * beyond the end of the bitmap.
		 */
		next_bit = xfs_next_bit(blfp->blf_data_map, blfp->blf_map_size,
					(uint)last_bit + 1);
		/*
		 * If we run out of bits fill in the last iovec and get out of
		 * the loop.  Else if we start a new set of bits then fill in
		 * the iovec for the series we were looking at and start
		 * counting the bits in the new one.  Else we're still in the
		 * same set of bits so just keep counting and scanning.
		 */
		if (next_bit == -1) {
			xfs_buf_item_copy_iovec(lv, vecp, bp, offset,
						first_bit, nbits);
			blfp->blf_size++;
			break;
		} else if (next_bit != last_bit + 1 ||
		           xfs_buf_item_straddle(bp, offset, first_bit, nbits)) {
			xfs_buf_item_copy_iovec(lv, vecp, bp, offset,
						first_bit, nbits);
			blfp->blf_size++;
			first_bit = next_bit;
			last_bit = next_bit;
			nbits = 1;
		} else {
			last_bit++;
			nbits++;
		}
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * This is called to fill in the vector of log iovecs for the
 * given log buf item.  It fills the first entry with a buf log
 * format structure, and the rest point to contiguous chunks
 * within the buffer.
 */
STATIC void
xfs_buf_item_format(
	struct xfs_log_item	*lip,
<<<<<<< HEAD
	struct xfs_log_iovec	*vecp)
{
	struct xfs_buf_log_item	*bip = BUF_ITEM(lip);
	struct xfs_buf	*bp = bip->bli_buf;
	uint		base_size;
	uint		nvecs;
	int		first_bit;
	int		last_bit;
	int		next_bit;
	uint		nbits;
	uint		buffer_offset;
=======
	struct xfs_log_vec	*lv)
{
	struct xfs_buf_log_item	*bip = BUF_ITEM(lip);
	struct xfs_buf		*bp = bip->bli_buf;
	struct xfs_log_iovec	*vecp = NULL;
	uint			offset = 0;
	int			i;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ASSERT(atomic_read(&bip->bli_refcount) > 0);
	ASSERT((bip->bli_flags & XFS_BLI_LOGGED) ||
	       (bip->bli_flags & XFS_BLI_STALE));
<<<<<<< HEAD

	/*
	 * The size of the base structure is the size of the
	 * declared structure plus the space for the extra words
	 * of the bitmap.  We subtract one from the map size, because
	 * the first element of the bitmap is accounted for in the
	 * size of the base structure.
	 */
	base_size =
		(uint)(sizeof(xfs_buf_log_format_t) +
		       ((bip->bli_format.blf_map_size - 1) * sizeof(uint)));
	vecp->i_addr = &bip->bli_format;
	vecp->i_len = base_size;
	vecp->i_type = XLOG_REG_TYPE_BFORMAT;
	vecp++;
	nvecs = 1;

	/*
	 * If it is an inode buffer, transfer the in-memory state to the
	 * format flags and clear the in-memory state. We do not transfer
	 * this state if the inode buffer allocation has not yet been committed
	 * to the log as setting the XFS_BLI_INODE_BUF flag will prevent
	 * correct replay of the inode allocation.
	 */
	if (bip->bli_flags & XFS_BLI_INODE_BUF) {
		if (!((bip->bli_flags & XFS_BLI_INODE_ALLOC_BUF) &&
		      xfs_log_item_in_current_chkpt(lip)))
			bip->bli_format.blf_flags |= XFS_BLF_INODE_BUF;
		bip->bli_flags &= ~XFS_BLI_INODE_BUF;
	}

	if (bip->bli_flags & XFS_BLI_STALE) {
		/*
		 * The buffer is stale, so all we need to log
		 * is the buf log format structure with the
		 * cancel flag in it.
		 */
		trace_xfs_buf_item_format_stale(bip);
		ASSERT(bip->bli_format.blf_flags & XFS_BLF_CANCEL);
		bip->bli_format.blf_size = nvecs;
		return;
	}

	/*
	 * Fill in an iovec for each set of contiguous chunks.
	 */
	first_bit = xfs_next_bit(bip->bli_format.blf_data_map,
					 bip->bli_format.blf_map_size, 0);
	ASSERT(first_bit != -1);
	last_bit = first_bit;
	nbits = 1;
	for (;;) {
		/*
		 * This takes the bit number to start looking from and
		 * returns the next set bit from there.  It returns -1
		 * if there are no more bits set or the start bit is
		 * beyond the end of the bitmap.
		 */
		next_bit = xfs_next_bit(bip->bli_format.blf_data_map,
						 bip->bli_format.blf_map_size,
						 (uint)last_bit + 1);
		/*
		 * If we run out of bits fill in the last iovec and get
		 * out of the loop.
		 * Else if we start a new set of bits then fill in the
		 * iovec for the series we were looking at and start
		 * counting the bits in the new one.
		 * Else we're still in the same set of bits so just
		 * keep counting and scanning.
		 */
		if (next_bit == -1) {
			buffer_offset = first_bit * XFS_BLF_CHUNK;
			vecp->i_addr = xfs_buf_offset(bp, buffer_offset);
			vecp->i_len = nbits * XFS_BLF_CHUNK;
			vecp->i_type = XLOG_REG_TYPE_BCHUNK;
			nvecs++;
			break;
		} else if (next_bit != last_bit + 1) {
			buffer_offset = first_bit * XFS_BLF_CHUNK;
			vecp->i_addr = xfs_buf_offset(bp, buffer_offset);
			vecp->i_len = nbits * XFS_BLF_CHUNK;
			vecp->i_type = XLOG_REG_TYPE_BCHUNK;
			nvecs++;
			vecp++;
			first_bit = next_bit;
			last_bit = next_bit;
			nbits = 1;
		} else if (xfs_buf_offset(bp, next_bit << XFS_BLF_SHIFT) !=
			   (xfs_buf_offset(bp, last_bit << XFS_BLF_SHIFT) +
			    XFS_BLF_CHUNK)) {
			buffer_offset = first_bit * XFS_BLF_CHUNK;
			vecp->i_addr = xfs_buf_offset(bp, buffer_offset);
			vecp->i_len = nbits * XFS_BLF_CHUNK;
			vecp->i_type = XLOG_REG_TYPE_BCHUNK;
/* You would think we need to bump the nvecs here too, but we do not
 * this number is used by recovery, and it gets confused by the boundary
 * split here
 *			nvecs++;
 */
			vecp++;
			first_bit = next_bit;
			last_bit = next_bit;
			nbits = 1;
		} else {
			last_bit++;
			nbits++;
		}
	}
	bip->bli_format.blf_size = nvecs;

	/*
	 * Check to make sure everything is consistent.
	 */
	trace_xfs_buf_item_format(bip);
	xfs_buf_item_log_check(bip);
=======
	ASSERT((bip->bli_flags & XFS_BLI_STALE) ||
	       (xfs_blft_from_flags(&bip->__bli_format) > XFS_BLFT_UNKNOWN_BUF
	        && xfs_blft_from_flags(&bip->__bli_format) < XFS_BLFT_MAX_BUF));
	ASSERT(!(bip->bli_flags & XFS_BLI_ORDERED) ||
	       (bip->bli_flags & XFS_BLI_STALE));


	/*
	 * If it is an inode buffer, transfer the in-memory state to the
	 * format flags and clear the in-memory state.
	 *
	 * For buffer based inode allocation, we do not transfer
	 * this state if the inode buffer allocation has not yet been committed
	 * to the log as setting the XFS_BLI_INODE_BUF flag will prevent
	 * correct replay of the inode allocation.
	 *
	 * For icreate item based inode allocation, the buffers aren't written
	 * to the journal during allocation, and hence we should always tag the
	 * buffer as an inode buffer so that the correct unlinked list replay
	 * occurs during recovery.
	 */
	if (bip->bli_flags & XFS_BLI_INODE_BUF) {
		if (xfs_has_v3inodes(lip->li_log->l_mp) ||
		    !((bip->bli_flags & XFS_BLI_INODE_ALLOC_BUF) &&
		      xfs_log_item_in_current_chkpt(lip)))
			bip->__bli_format.blf_flags |= XFS_BLF_INODE_BUF;
		bip->bli_flags &= ~XFS_BLI_INODE_BUF;
	}

	for (i = 0; i < bip->bli_format_count; i++) {
		xfs_buf_item_format_segment(bip, lv, &vecp, offset,
					    &bip->bli_formats[i]);
		offset += BBTOB(bp->b_maps[i].bm_len);
	}

	/*
	 * Check to make sure everything is consistent.
	 */
	trace_xfs_buf_item_format(bip);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * This is called to pin the buffer associated with the buf log item in memory
 * so it cannot be written out.
 *
<<<<<<< HEAD
 * We also always take a reference to the buffer log item here so that the bli
 * is held while the item is pinned in memory. This means that we can
 * unconditionally drop the reference count a transaction holds when the
 * transaction is completed.
=======
 * We take a reference to the buffer log item here so that the BLI life cycle
 * extends at least until the buffer is unpinned via xfs_buf_item_unpin() and
 * inserted into the AIL.
 *
 * We also need to take a reference to the buffer itself as the BLI unpin
 * processing requires accessing the buffer after the BLI has dropped the final
 * BLI reference. See xfs_buf_item_unpin() for an explanation.
 * If unpins race to drop the final BLI reference and only the
 * BLI owns a reference to the buffer, then the loser of the race can have the
 * buffer fgreed from under it (e.g. on shutdown). Taking a buffer reference per
 * pin count ensures the life cycle of the buffer extends for as
 * long as we hold the buffer pin reference in xfs_buf_item_unpin().
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
STATIC void
xfs_buf_item_pin(
	struct xfs_log_item	*lip)
{
	struct xfs_buf_log_item	*bip = BUF_ITEM(lip);

	ASSERT(atomic_read(&bip->bli_refcount) > 0);
	ASSERT((bip->bli_flags & XFS_BLI_LOGGED) ||
<<<<<<< HEAD
=======
	       (bip->bli_flags & XFS_BLI_ORDERED) ||
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	       (bip->bli_flags & XFS_BLI_STALE));

	trace_xfs_buf_item_pin(bip);

<<<<<<< HEAD
=======
	xfs_buf_hold(bip->bli_buf);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	atomic_inc(&bip->bli_refcount);
	atomic_inc(&bip->bli_buf->b_pin_count);
}

/*
<<<<<<< HEAD
 * This is called to unpin the buffer associated with the buf log
 * item which was previously pinned with a call to xfs_buf_item_pin().
 *
 * Also drop the reference to the buf item for the current transaction.
 * If the XFS_BLI_STALE flag is set and we are the last reference,
 * then free up the buf log item and unlock the buffer.
 *
 * If the remove flag is set we are called from uncommit in the
 * forced-shutdown path.  If that is true and the reference count on
 * the log item is going to drop to zero we need to free the item's
 * descriptor in the transaction.
=======
 * This is called to unpin the buffer associated with the buf log item which was
 * previously pinned with a call to xfs_buf_item_pin().  We enter this function
 * with a buffer pin count, a buffer reference and a BLI reference.
 *
 * We must drop the BLI reference before we unpin the buffer because the AIL
 * doesn't acquire a BLI reference whenever it accesses it. Therefore if the
 * refcount drops to zero, the bli could still be AIL resident and the buffer
 * submitted for I/O at any point before we return. This can result in IO
 * completion freeing the buffer while we are still trying to access it here.
 * This race condition can also occur in shutdown situations where we abort and
 * unpin buffers from contexts other that journal IO completion.
 *
 * Hence we have to hold a buffer reference per pin count to ensure that the
 * buffer cannot be freed until we have finished processing the unpin operation.
 * The reference is taken in xfs_buf_item_pin(), and we must hold it until we
 * are done processing the buffer state. In the case of an abort (remove =
 * true) then we re-use the current pin reference as the IO reference we hand
 * off to IO failure handling.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
STATIC void
xfs_buf_item_unpin(
	struct xfs_log_item	*lip,
	int			remove)
{
	struct xfs_buf_log_item	*bip = BUF_ITEM(lip);
<<<<<<< HEAD
	xfs_buf_t	*bp = bip->bli_buf;
	struct xfs_ail	*ailp = lip->li_ailp;
	int		stale = bip->bli_flags & XFS_BLI_STALE;
	int		freed;

	ASSERT(bp->b_fspriv == bip);
=======
	struct xfs_buf		*bp = bip->bli_buf;
	int			stale = bip->bli_flags & XFS_BLI_STALE;
	int			freed;

	ASSERT(bp->b_log_item == bip);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ASSERT(atomic_read(&bip->bli_refcount) > 0);

	trace_xfs_buf_item_unpin(bip);

	freed = atomic_dec_and_test(&bip->bli_refcount);
<<<<<<< HEAD

	if (atomic_dec_and_test(&bp->b_pin_count))
		wake_up_all(&bp->b_waiters);

	if (freed && stale) {
		ASSERT(bip->bli_flags & XFS_BLI_STALE);
		ASSERT(xfs_buf_islocked(bp));
		ASSERT(!(XFS_BUF_ISDELAYWRITE(bp)));
		ASSERT(XFS_BUF_ISSTALE(bp));
		ASSERT(bip->bli_format.blf_flags & XFS_BLF_CANCEL);

		trace_xfs_buf_item_unpin_stale(bip);

		if (remove) {
			/*
			 * If we are in a transaction context, we have to
			 * remove the log item from the transaction as we are
			 * about to release our reference to the buffer.  If we
			 * don't, the unlock that occurs later in
			 * xfs_trans_uncommit() will try to reference the
			 * buffer which we no longer have a hold on.
			 */
			if (lip->li_desc)
				xfs_trans_del_item(lip);

			/*
			 * Since the transaction no longer refers to the buffer,
			 * the buffer should no longer refer to the transaction.
			 */
			bp->b_transp = NULL;
		}

		/*
		 * If we get called here because of an IO error, we may
		 * or may not have the item on the AIL. xfs_trans_ail_delete()
		 * will take care of that situation.
		 * xfs_trans_ail_delete() drops the AIL lock.
		 */
		if (bip->bli_flags & XFS_BLI_STALE_INODE) {
			xfs_buf_do_callbacks(bp);
			bp->b_fspriv = NULL;
			bp->b_iodone = NULL;
		} else {
			spin_lock(&ailp->xa_lock);
			xfs_trans_ail_delete(ailp, (xfs_log_item_t *)bip);
			xfs_buf_item_relse(bp);
			ASSERT(bp->b_fspriv == NULL);
		}
		xfs_buf_relse(bp);
	}
}

/*
 * This is called to attempt to lock the buffer associated with this
 * buf log item.  Don't sleep on the buffer lock.  If we can't get
 * the lock right away, return 0.  If we can get the lock, take a
 * reference to the buffer. If this is a delayed write buffer that
 * needs AIL help to be written back, invoke the pushbuf routine
 * rather than the normal success path.
 */
STATIC uint
xfs_buf_item_trylock(
	struct xfs_log_item	*lip)
{
	struct xfs_buf_log_item	*bip = BUF_ITEM(lip);
	struct xfs_buf		*bp = bip->bli_buf;

	if (xfs_buf_ispinned(bp))
		return XFS_ITEM_PINNED;
	if (!xfs_buf_trylock(bp))
		return XFS_ITEM_LOCKED;

	/* take a reference to the buffer.  */
	xfs_buf_hold(bp);

	ASSERT(!(bip->bli_flags & XFS_BLI_STALE));
	trace_xfs_buf_item_trylock(bip);
	if (XFS_BUF_ISDELAYWRITE(bp))
		return XFS_ITEM_PUSHBUF;
	return XFS_ITEM_SUCCESS;
=======
	if (atomic_dec_and_test(&bp->b_pin_count))
		wake_up_all(&bp->b_waiters);

	/*
	 * Nothing to do but drop the buffer pin reference if the BLI is
	 * still active.
	 */
	if (!freed) {
		xfs_buf_rele(bp);
		return;
	}

	if (stale) {
		ASSERT(bip->bli_flags & XFS_BLI_STALE);
		ASSERT(xfs_buf_islocked(bp));
		ASSERT(bp->b_flags & XBF_STALE);
		ASSERT(bip->__bli_format.blf_flags & XFS_BLF_CANCEL);
		ASSERT(list_empty(&lip->li_trans));
		ASSERT(!bp->b_transp);

		trace_xfs_buf_item_unpin_stale(bip);

		/*
		 * The buffer has been locked and referenced since it was marked
		 * stale so we own both lock and reference exclusively here. We
		 * do not need the pin reference any more, so drop it now so
		 * that we only have one reference to drop once item completion
		 * processing is complete.
		 */
		xfs_buf_rele(bp);

		/*
		 * If we get called here because of an IO error, we may or may
		 * not have the item on the AIL. xfs_trans_ail_delete() will
		 * take care of that situation. xfs_trans_ail_delete() drops
		 * the AIL lock.
		 */
		if (bip->bli_flags & XFS_BLI_STALE_INODE) {
			xfs_buf_item_done(bp);
			xfs_buf_inode_iodone(bp);
			ASSERT(list_empty(&bp->b_li_list));
		} else {
			xfs_trans_ail_delete(lip, SHUTDOWN_LOG_IO_ERROR);
			xfs_buf_item_relse(bp);
			ASSERT(bp->b_log_item == NULL);
		}
		xfs_buf_relse(bp);
		return;
	}

	if (remove) {
		/*
		 * We need to simulate an async IO failures here to ensure that
		 * the correct error completion is run on this buffer. This
		 * requires a reference to the buffer and for the buffer to be
		 * locked. We can safely pass ownership of the pin reference to
		 * the IO to ensure that nothing can free the buffer while we
		 * wait for the lock and then run the IO failure completion.
		 */
		xfs_buf_lock(bp);
		bp->b_flags |= XBF_ASYNC;
		xfs_buf_ioend_fail(bp);
		return;
	}

	/*
	 * BLI has no more active references - it will be moved to the AIL to
	 * manage the remaining BLI/buffer life cycle. There is nothing left for
	 * us to do here so drop the pin reference to the buffer.
	 */
	xfs_buf_rele(bp);
}

STATIC uint
xfs_buf_item_push(
	struct xfs_log_item	*lip,
	struct list_head	*buffer_list)
{
	struct xfs_buf_log_item	*bip = BUF_ITEM(lip);
	struct xfs_buf		*bp = bip->bli_buf;
	uint			rval = XFS_ITEM_SUCCESS;

	if (xfs_buf_ispinned(bp))
		return XFS_ITEM_PINNED;
	if (!xfs_buf_trylock(bp)) {
		/*
		 * If we have just raced with a buffer being pinned and it has
		 * been marked stale, we could end up stalling until someone else
		 * issues a log force to unpin the stale buffer. Check for the
		 * race condition here so xfsaild recognizes the buffer is pinned
		 * and queues a log force to move it along.
		 */
		if (xfs_buf_ispinned(bp))
			return XFS_ITEM_PINNED;
		return XFS_ITEM_LOCKED;
	}

	ASSERT(!(bip->bli_flags & XFS_BLI_STALE));

	trace_xfs_buf_item_push(bip);

	/* has a previous flush failed due to IO errors? */
	if (bp->b_flags & XBF_WRITE_FAIL) {
		xfs_buf_alert_ratelimited(bp, "XFS: Failing async write",
	    "Failing async write on buffer block 0x%llx. Retrying async write.",
					  (long long)xfs_buf_daddr(bp));
	}

	if (!xfs_buf_delwri_queue(bp, buffer_list))
		rval = XFS_ITEM_FLUSHING;
	xfs_buf_unlock(bp);
	return rval;
}

/*
 * Drop the buffer log item refcount and take appropriate action. This helper
 * determines whether the bli must be freed or not, since a decrement to zero
 * does not necessarily mean the bli is unused.
 *
 * Return true if the bli is freed, false otherwise.
 */
bool
xfs_buf_item_put(
	struct xfs_buf_log_item	*bip)
{
	struct xfs_log_item	*lip = &bip->bli_item;
	bool			aborted;
	bool			dirty;

	/* drop the bli ref and return if it wasn't the last one */
	if (!atomic_dec_and_test(&bip->bli_refcount))
		return false;

	/*
	 * We dropped the last ref and must free the item if clean or aborted.
	 * If the bli is dirty and non-aborted, the buffer was clean in the
	 * transaction but still awaiting writeback from previous changes. In
	 * that case, the bli is freed on buffer writeback completion.
	 */
	aborted = test_bit(XFS_LI_ABORTED, &lip->li_flags) ||
			xlog_is_shutdown(lip->li_log);
	dirty = bip->bli_flags & XFS_BLI_DIRTY;
	if (dirty && !aborted)
		return false;

	/*
	 * The bli is aborted or clean. An aborted item may be in the AIL
	 * regardless of dirty state.  For example, consider an aborted
	 * transaction that invalidated a dirty bli and cleared the dirty
	 * state.
	 */
	if (aborted)
		xfs_trans_ail_delete(lip, 0);
	xfs_buf_item_relse(bip->bli_buf);
	return true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Release the buffer associated with the buf log item.  If there is no dirty
 * logged data associated with the buffer recorded in the buf log item, then
 * free the buf log item and remove the reference to it in the buffer.
 *
 * This call ignores the recursion count.  It is only called when the buffer
 * should REALLY be unlocked, regardless of the recursion count.
 *
 * We unconditionally drop the transaction's reference to the log item. If the
 * item was logged, then another reference was taken when it was pinned, so we
 * can safely drop the transaction reference now.  This also allows us to avoid
 * potential races with the unpin code freeing the bli by not referencing the
 * bli after we've dropped the reference count.
 *
 * If the XFS_BLI_HOLD flag is set in the buf log item, then free the log item
 * if necessary but do not unlock the buffer.  This is for support of
 * xfs_trans_bhold(). Make sure the XFS_BLI_HOLD field is cleared if we don't
 * free the item.
 */
STATIC void
<<<<<<< HEAD
xfs_buf_item_unlock(
=======
xfs_buf_item_release(
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct xfs_log_item	*lip)
{
	struct xfs_buf_log_item	*bip = BUF_ITEM(lip);
	struct xfs_buf		*bp = bip->bli_buf;
<<<<<<< HEAD
	int			aborted;
	uint			hold;

	/* Clear the buffer's association with this transaction. */
	bp->b_transp = NULL;

	/*
	 * If this is a transaction abort, don't return early.  Instead, allow
	 * the brelse to happen.  Normally it would be done for stale
	 * (cancelled) buffers at unpin time, but we'll never go through the
	 * pin/unpin cycle if we abort inside commit.
	 */
	aborted = (lip->li_flags & XFS_LI_ABORTED) != 0;

	/*
	 * Before possibly freeing the buf item, determine if we should
	 * release the buffer at the end of this routine.
	 */
	hold = bip->bli_flags & XFS_BLI_HOLD;

	/* Clear the per transaction state. */
	bip->bli_flags &= ~(XFS_BLI_LOGGED | XFS_BLI_HOLD);

	/*
	 * If the buf item is marked stale, then don't do anything.  We'll
	 * unlock the buffer and free the buf item when the buffer is unpinned
	 * for the last time.
	 */
	if (bip->bli_flags & XFS_BLI_STALE) {
		trace_xfs_buf_item_unlock_stale(bip);
		ASSERT(bip->bli_format.blf_flags & XFS_BLF_CANCEL);
		if (!aborted) {
			atomic_dec(&bip->bli_refcount);
			return;
		}
	}

	trace_xfs_buf_item_unlock(bip);

	/*
	 * If the buf item isn't tracking any data, free it, otherwise drop the
	 * reference we hold to it.
	 */
	if (xfs_bitmap_empty(bip->bli_format.blf_data_map,
			     bip->bli_format.blf_map_size))
		xfs_buf_item_relse(bp);
	else
		atomic_dec(&bip->bli_refcount);

	if (!hold)
		xfs_buf_relse(bp);
=======
	bool			released;
	bool			hold = bip->bli_flags & XFS_BLI_HOLD;
	bool			stale = bip->bli_flags & XFS_BLI_STALE;
#if defined(DEBUG) || defined(XFS_WARN)
	bool			ordered = bip->bli_flags & XFS_BLI_ORDERED;
	bool			dirty = bip->bli_flags & XFS_BLI_DIRTY;
	bool			aborted = test_bit(XFS_LI_ABORTED,
						   &lip->li_flags);
#endif

	trace_xfs_buf_item_release(bip);

	/*
	 * The bli dirty state should match whether the blf has logged segments
	 * except for ordered buffers, where only the bli should be dirty.
	 */
	ASSERT((!ordered && dirty == xfs_buf_item_dirty_format(bip)) ||
	       (ordered && dirty && !xfs_buf_item_dirty_format(bip)));
	ASSERT(!stale || (bip->__bli_format.blf_flags & XFS_BLF_CANCEL));

	/*
	 * Clear the buffer's association with this transaction and
	 * per-transaction state from the bli, which has been copied above.
	 */
	bp->b_transp = NULL;
	bip->bli_flags &= ~(XFS_BLI_LOGGED | XFS_BLI_HOLD | XFS_BLI_ORDERED);

	/*
	 * Unref the item and unlock the buffer unless held or stale. Stale
	 * buffers remain locked until final unpin unless the bli is freed by
	 * the unref call. The latter implies shutdown because buffer
	 * invalidation dirties the bli and transaction.
	 */
	released = xfs_buf_item_put(bip);
	if (hold || (stale && !released))
		return;
	ASSERT(!stale || aborted);
	xfs_buf_relse(bp);
}

STATIC void
xfs_buf_item_committing(
	struct xfs_log_item	*lip,
	xfs_csn_t		seq)
{
	return xfs_buf_item_release(lip);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * This is called to find out where the oldest active copy of the
 * buf log item in the on disk log resides now that the last log
 * write of it completed at the given lsn.
 * We always re-log all the dirty data in a buffer, so usually the
 * latest copy in the on disk log is the only one that matters.  For
 * those cases we simply return the given lsn.
 *
 * The one exception to this is for buffers full of newly allocated
 * inodes.  These buffers are only relogged with the XFS_BLI_INODE_BUF
 * flag set, indicating that only the di_next_unlinked fields from the
 * inodes in the buffers will be replayed during recovery.  If the
 * original newly allocated inode images have not yet been flushed
 * when the buffer is so relogged, then we need to make sure that we
 * keep the old images in the 'active' portion of the log.  We do this
 * by returning the original lsn of that transaction here rather than
 * the current one.
 */
STATIC xfs_lsn_t
xfs_buf_item_committed(
	struct xfs_log_item	*lip,
	xfs_lsn_t		lsn)
{
	struct xfs_buf_log_item	*bip = BUF_ITEM(lip);

	trace_xfs_buf_item_committed(bip);

	if ((bip->bli_flags & XFS_BLI_INODE_ALLOC_BUF) && lip->li_lsn != 0)
		return lip->li_lsn;
	return lsn;
}

<<<<<<< HEAD
/*
 * The buffer is locked, but is not a delayed write buffer. This happens
 * if we race with IO completion and hence we don't want to try to write it
 * again. Just release the buffer.
 */
STATIC void
xfs_buf_item_push(
	struct xfs_log_item	*lip)
{
	struct xfs_buf_log_item	*bip = BUF_ITEM(lip);
	struct xfs_buf		*bp = bip->bli_buf;

	ASSERT(!(bip->bli_flags & XFS_BLI_STALE));
	ASSERT(!XFS_BUF_ISDELAYWRITE(bp));

	trace_xfs_buf_item_push(bip);

	xfs_buf_relse(bp);
}

/*
 * The buffer is locked and is a delayed write buffer. Promote the buffer
 * in the delayed write queue as the caller knows that they must invoke
 * the xfsbufd to get this buffer written. We have to unlock the buffer
 * to allow the xfsbufd to write it, too.
 */
STATIC bool
xfs_buf_item_pushbuf(
	struct xfs_log_item	*lip)
{
	struct xfs_buf_log_item	*bip = BUF_ITEM(lip);
	struct xfs_buf		*bp = bip->bli_buf;

	ASSERT(!(bip->bli_flags & XFS_BLI_STALE));
	ASSERT(XFS_BUF_ISDELAYWRITE(bp));

	trace_xfs_buf_item_pushbuf(bip);

	xfs_buf_delwri_promote(bp);
	xfs_buf_relse(bp);
	return true;
}

STATIC void
xfs_buf_item_committing(
	struct xfs_log_item	*lip,
	xfs_lsn_t		commit_lsn)
{
}

/*
 * This is the ops vector shared by all buf log items.
 */
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static const struct xfs_item_ops xfs_buf_item_ops = {
	.iop_size	= xfs_buf_item_size,
	.iop_format	= xfs_buf_item_format,
	.iop_pin	= xfs_buf_item_pin,
	.iop_unpin	= xfs_buf_item_unpin,
<<<<<<< HEAD
	.iop_trylock	= xfs_buf_item_trylock,
	.iop_unlock	= xfs_buf_item_unlock,
	.iop_committed	= xfs_buf_item_committed,
	.iop_push	= xfs_buf_item_push,
	.iop_pushbuf	= xfs_buf_item_pushbuf,
	.iop_committing = xfs_buf_item_committing
};


/*
 * Allocate a new buf log item to go with the given buffer.
 * Set the buffer's b_fsprivate field to point to the new
 * buf log item.  If there are other item's attached to the
 * buffer (see xfs_buf_attach_iodone() below), then put the
 * buf log item at the front.
 */
void
xfs_buf_item_init(
	xfs_buf_t	*bp,
	xfs_mount_t	*mp)
{
	xfs_log_item_t		*lip = bp->b_fspriv;
	xfs_buf_log_item_t	*bip;
	int			chunks;
	int			map_size;

	/*
	 * Check to see if there is already a buf log item for
	 * this buffer.  If there is, it is guaranteed to be
	 * the first.  If we do already have one, there is
	 * nothing to do here so return.
	 */
	ASSERT(bp->b_target->bt_mount == mp);
	if (lip != NULL && lip->li_type == XFS_LI_BUF)
		return;

	/*
	 * chunks is the number of XFS_BLF_CHUNK size pieces
	 * the buffer can be divided into. Make sure not to
	 * truncate any pieces.  map_size is the size of the
	 * bitmap needed to describe the chunks of the buffer.
	 */
	chunks = (int)((XFS_BUF_COUNT(bp) + (XFS_BLF_CHUNK - 1)) >> XFS_BLF_SHIFT);
	map_size = (int)((chunks + NBWORD) >> BIT_TO_WORD_SHIFT);

	bip = (xfs_buf_log_item_t*)kmem_zone_zalloc(xfs_buf_item_zone,
						    KM_SLEEP);
	xfs_log_item_init(mp, &bip->bli_item, XFS_LI_BUF, &xfs_buf_item_ops);
	bip->bli_buf = bp;
	xfs_buf_hold(bp);
	bip->bli_format.blf_type = XFS_LI_BUF;
	bip->bli_format.blf_blkno = (__int64_t)XFS_BUF_ADDR(bp);
	bip->bli_format.blf_len = (ushort)BTOBB(XFS_BUF_COUNT(bp));
	bip->bli_format.blf_map_size = map_size;

#ifdef XFS_TRANS_DEBUG
	/*
	 * Allocate the arrays for tracking what needs to be logged
	 * and what our callers request to be logged.  bli_orig
	 * holds a copy of the original, clean buffer for comparison
	 * against, and bli_logged keeps a 1 bit flag per byte in
	 * the buffer to indicate which bytes the callers have asked
	 * to have logged.
	 */
	bip->bli_orig = (char *)kmem_alloc(XFS_BUF_COUNT(bp), KM_SLEEP);
	memcpy(bip->bli_orig, bp->b_addr, XFS_BUF_COUNT(bp));
	bip->bli_logged = (char *)kmem_zalloc(XFS_BUF_COUNT(bp) / NBBY, KM_SLEEP);
#endif

	/*
	 * Put the buf item into the list of items attached to the
	 * buffer at the front.
	 */
	if (bp->b_fspriv)
		bip->bli_item.li_bio_list = bp->b_fspriv;
	bp->b_fspriv = bip;
=======
	.iop_release	= xfs_buf_item_release,
	.iop_committing	= xfs_buf_item_committing,
	.iop_committed	= xfs_buf_item_committed,
	.iop_push	= xfs_buf_item_push,
};

STATIC void
xfs_buf_item_get_format(
	struct xfs_buf_log_item	*bip,
	int			count)
{
	ASSERT(bip->bli_formats == NULL);
	bip->bli_format_count = count;

	if (count == 1) {
		bip->bli_formats = &bip->__bli_format;
		return;
	}

	bip->bli_formats = kzalloc(count * sizeof(struct xfs_buf_log_format),
				GFP_KERNEL | __GFP_NOFAIL);
}

STATIC void
xfs_buf_item_free_format(
	struct xfs_buf_log_item	*bip)
{
	if (bip->bli_formats != &bip->__bli_format) {
		kfree(bip->bli_formats);
		bip->bli_formats = NULL;
	}
}

/*
 * Allocate a new buf log item to go with the given buffer.
 * Set the buffer's b_log_item field to point to the new
 * buf log item.
 */
int
xfs_buf_item_init(
	struct xfs_buf	*bp,
	struct xfs_mount *mp)
{
	struct xfs_buf_log_item	*bip = bp->b_log_item;
	int			chunks;
	int			map_size;
	int			i;

	/*
	 * Check to see if there is already a buf log item for
	 * this buffer. If we do already have one, there is
	 * nothing to do here so return.
	 */
	ASSERT(bp->b_mount == mp);
	if (bip) {
		ASSERT(bip->bli_item.li_type == XFS_LI_BUF);
		ASSERT(!bp->b_transp);
		ASSERT(bip->bli_buf == bp);
		return 0;
	}

	bip = kmem_cache_zalloc(xfs_buf_item_cache, GFP_KERNEL | __GFP_NOFAIL);
	xfs_log_item_init(mp, &bip->bli_item, XFS_LI_BUF, &xfs_buf_item_ops);
	bip->bli_buf = bp;

	/*
	 * chunks is the number of XFS_BLF_CHUNK size pieces the buffer
	 * can be divided into. Make sure not to truncate any pieces.
	 * map_size is the size of the bitmap needed to describe the
	 * chunks of the buffer.
	 *
	 * Discontiguous buffer support follows the layout of the underlying
	 * buffer. This makes the implementation as simple as possible.
	 */
	xfs_buf_item_get_format(bip, bp->b_map_count);

	for (i = 0; i < bip->bli_format_count; i++) {
		chunks = DIV_ROUND_UP(BBTOB(bp->b_maps[i].bm_len),
				      XFS_BLF_CHUNK);
		map_size = DIV_ROUND_UP(chunks, NBWORD);

		if (map_size > XFS_BLF_DATAMAP_SIZE) {
			kmem_cache_free(xfs_buf_item_cache, bip);
			xfs_err(mp,
	"buffer item dirty bitmap (%u uints) too small to reflect %u bytes!",
					map_size,
					BBTOB(bp->b_maps[i].bm_len));
			return -EFSCORRUPTED;
		}

		bip->bli_formats[i].blf_type = XFS_LI_BUF;
		bip->bli_formats[i].blf_blkno = bp->b_maps[i].bm_bn;
		bip->bli_formats[i].blf_len = bp->b_maps[i].bm_len;
		bip->bli_formats[i].blf_map_size = map_size;
	}

	bp->b_log_item = bip;
	xfs_buf_hold(bp);
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}


/*
 * Mark bytes first through last inclusive as dirty in the buf
 * item's bitmap.
 */
<<<<<<< HEAD
void
xfs_buf_item_log(
	xfs_buf_log_item_t	*bip,
	uint			first,
	uint			last)
=======
static void
xfs_buf_item_log_segment(
	uint			first,
	uint			last,
	uint			*map)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	uint		first_bit;
	uint		last_bit;
	uint		bits_to_set;
	uint		bits_set;
	uint		word_num;
	uint		*wordp;
	uint		bit;
	uint		end_bit;
	uint		mask;

<<<<<<< HEAD
	/*
	 * Mark the item as having some dirty data for
	 * quick reference in xfs_buf_item_dirty.
	 */
	bip->bli_flags |= XFS_BLI_DIRTY;
=======
	ASSERT(first < XFS_BLF_DATAMAP_SIZE * XFS_BLF_CHUNK * NBWORD);
	ASSERT(last < XFS_BLF_DATAMAP_SIZE * XFS_BLF_CHUNK * NBWORD);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Convert byte offsets to bit numbers.
	 */
	first_bit = first >> XFS_BLF_SHIFT;
	last_bit = last >> XFS_BLF_SHIFT;

	/*
	 * Calculate the total number of bits to be set.
	 */
	bits_to_set = last_bit - first_bit + 1;

	/*
	 * Get a pointer to the first word in the bitmap
	 * to set a bit in.
	 */
	word_num = first_bit >> BIT_TO_WORD_SHIFT;
<<<<<<< HEAD
	wordp = &(bip->bli_format.blf_data_map[word_num]);
=======
	wordp = &map[word_num];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Calculate the starting bit in the first word.
	 */
	bit = first_bit & (uint)(NBWORD - 1);

	/*
	 * First set any bits in the first word of our range.
	 * If it starts at bit 0 of the word, it will be
	 * set below rather than here.  That is what the variable
	 * bit tells us. The variable bits_set tracks the number
	 * of bits that have been set so far.  End_bit is the number
	 * of the last bit to be set in this word plus one.
	 */
	if (bit) {
<<<<<<< HEAD
		end_bit = MIN(bit + bits_to_set, (uint)NBWORD);
		mask = ((1 << (end_bit - bit)) - 1) << bit;
=======
		end_bit = min(bit + bits_to_set, (uint)NBWORD);
		mask = ((1U << (end_bit - bit)) - 1) << bit;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		*wordp |= mask;
		wordp++;
		bits_set = end_bit - bit;
	} else {
		bits_set = 0;
	}

	/*
	 * Now set bits a whole word at a time that are between
	 * first_bit and last_bit.
	 */
	while ((bits_to_set - bits_set) >= NBWORD) {
<<<<<<< HEAD
		*wordp |= 0xffffffff;
=======
		*wordp = 0xffffffff;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		bits_set += NBWORD;
		wordp++;
	}

	/*
	 * Finally, set any bits left to be set in one last partial word.
	 */
	end_bit = bits_to_set - bits_set;
	if (end_bit) {
<<<<<<< HEAD
		mask = (1 << end_bit) - 1;
		*wordp |= mask;
	}

	xfs_buf_item_log_debug(bip, first, last);
=======
		mask = (1U << end_bit) - 1;
		*wordp |= mask;
	}
}

/*
 * Mark bytes first through last inclusive as dirty in the buf
 * item's bitmap.
 */
void
xfs_buf_item_log(
	struct xfs_buf_log_item	*bip,
	uint			first,
	uint			last)
{
	int			i;
	uint			start;
	uint			end;
	struct xfs_buf		*bp = bip->bli_buf;

	/*
	 * walk each buffer segment and mark them dirty appropriately.
	 */
	start = 0;
	for (i = 0; i < bip->bli_format_count; i++) {
		if (start > last)
			break;
		end = start + BBTOB(bp->b_maps[i].bm_len) - 1;

		/* skip to the map that includes the first byte to log */
		if (first > end) {
			start += BBTOB(bp->b_maps[i].bm_len);
			continue;
		}

		/*
		 * Trim the range to this segment and mark it in the bitmap.
		 * Note that we must convert buffer offsets to segment relative
		 * offsets (e.g., the first byte of each segment is byte 0 of
		 * that segment).
		 */
		if (first < start)
			first = start;
		if (end > last)
			end = last;
		xfs_buf_item_log_segment(first - start, end - start,
					 &bip->bli_formats[i].blf_data_map[0]);

		start += BBTOB(bp->b_maps[i].bm_len);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}


/*
<<<<<<< HEAD
 * Return 1 if the buffer has some data that has been logged (at any
 * point, not just the current transaction) and 0 if not.
 */
uint
xfs_buf_item_dirty(
	xfs_buf_log_item_t	*bip)
{
	return (bip->bli_flags & XFS_BLI_DIRTY);
=======
 * Return true if the buffer has any ranges logged/dirtied by a transaction,
 * false otherwise.
 */
bool
xfs_buf_item_dirty_format(
	struct xfs_buf_log_item	*bip)
{
	int			i;

	for (i = 0; i < bip->bli_format_count; i++) {
		if (!xfs_bitmap_empty(bip->bli_formats[i].blf_data_map,
			     bip->bli_formats[i].blf_map_size))
			return true;
	}

	return false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

STATIC void
xfs_buf_item_free(
<<<<<<< HEAD
	xfs_buf_log_item_t	*bip)
{
#ifdef XFS_TRANS_DEBUG
	kmem_free(bip->bli_orig);
	kmem_free(bip->bli_logged);
#endif /* XFS_TRANS_DEBUG */

	kmem_zone_free(xfs_buf_item_zone, bip);
}

/*
 * This is called when the buf log item is no longer needed.  It should
 * free the buf log item associated with the given buffer and clear
 * the buffer's pointer to the buf log item.  If there are no more
 * items in the list, clear the b_iodone field of the buffer (see
 * xfs_buf_attach_iodone() below).
 */
void
xfs_buf_item_relse(
	xfs_buf_t	*bp)
{
	xfs_buf_log_item_t	*bip;

	trace_xfs_buf_item_relse(bp, _RET_IP_);

	bip = bp->b_fspriv;
	bp->b_fspriv = bip->bli_item.li_bio_list;
	if (bp->b_fspriv == NULL)
		bp->b_iodone = NULL;

=======
	struct xfs_buf_log_item	*bip)
{
	xfs_buf_item_free_format(bip);
	kvfree(bip->bli_item.li_lv_shadow);
	kmem_cache_free(xfs_buf_item_cache, bip);
}

/*
 * xfs_buf_item_relse() is called when the buf log item is no longer needed.
 */
void
xfs_buf_item_relse(
	struct xfs_buf	*bp)
{
	struct xfs_buf_log_item	*bip = bp->b_log_item;

	trace_xfs_buf_item_relse(bp, _RET_IP_);
	ASSERT(!test_bit(XFS_LI_IN_AIL, &bip->bli_item.li_flags));

	if (atomic_read(&bip->bli_refcount))
		return;
	bp->b_log_item = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	xfs_buf_rele(bp);
	xfs_buf_item_free(bip);
}

<<<<<<< HEAD

/*
 * Add the given log item with its callback to the list of callbacks
 * to be called when the buffer's I/O completes.  If it is not set
 * already, set the buffer's b_iodone() routine to be
 * xfs_buf_iodone_callbacks() and link the log item into the list of
 * items rooted at b_fsprivate.  Items are always added as the second
 * entry in the list if there is a first, because the buf item code
 * assumes that the buf log item is first.
 */
void
xfs_buf_attach_iodone(
	xfs_buf_t	*bp,
	void		(*cb)(xfs_buf_t *, xfs_log_item_t *),
	xfs_log_item_t	*lip)
{
	xfs_log_item_t	*head_lip;

	ASSERT(xfs_buf_islocked(bp));

	lip->li_cb = cb;
	head_lip = bp->b_fspriv;
	if (head_lip) {
		lip->li_bio_list = head_lip->li_bio_list;
		head_lip->li_bio_list = lip;
	} else {
		bp->b_fspriv = lip;
	}

	ASSERT(bp->b_iodone == NULL ||
	       bp->b_iodone == xfs_buf_iodone_callbacks);
	bp->b_iodone = xfs_buf_iodone_callbacks;
}

/*
 * We can have many callbacks on a buffer. Running the callbacks individually
 * can cause a lot of contention on the AIL lock, so we allow for a single
 * callback to be able to scan the remaining lip->li_bio_list for other items
 * of the same type and callback to be processed in the first call.
 *
 * As a result, the loop walking the callback list below will also modify the
 * list. it removes the first item from the list and then runs the callback.
 * The loop then restarts from the new head of the list. This allows the
 * callback to scan and modify the list attached to the buffer and we don't
 * have to care about maintaining a next item pointer.
 */
STATIC void
xfs_buf_do_callbacks(
	struct xfs_buf		*bp)
{
	struct xfs_log_item	*lip;

	while ((lip = bp->b_fspriv) != NULL) {
		bp->b_fspriv = lip->li_bio_list;
		ASSERT(lip->li_cb != NULL);
		/*
		 * Clear the next pointer so we don't have any
		 * confusion if the item is added to another buf.
		 * Don't touch the log item after calling its
		 * callback, because it could have freed itself.
		 */
		lip->li_bio_list = NULL;
		lip->li_cb(bp, lip);
	}
}

/*
 * This is the iodone() function for buffers which have had callbacks
 * attached to them by xfs_buf_attach_iodone().  It should remove each
 * log item from the buffer's list and call the callback of each in turn.
 * When done, the buffer's fsprivate field is set to NULL and the buffer
 * is unlocked with a call to iodone().
 */
void
xfs_buf_iodone_callbacks(
	struct xfs_buf		*bp)
{
	struct xfs_log_item	*lip = bp->b_fspriv;
	struct xfs_mount	*mp = lip->li_mountp;
	static ulong		lasttime;
	static xfs_buftarg_t	*lasttarg;

	if (likely(!xfs_buf_geterror(bp)))
		goto do_callbacks;

	/*
	 * If we've already decided to shutdown the filesystem because of
	 * I/O errors, there's no point in giving this a retry.
	 */
	if (XFS_FORCED_SHUTDOWN(mp)) {
		xfs_buf_stale(bp);
		XFS_BUF_DONE(bp);
		trace_xfs_buf_item_iodone(bp, _RET_IP_);
		goto do_callbacks;
	}

	if (bp->b_target != lasttarg ||
	    time_after(jiffies, (lasttime + 5*HZ))) {
		lasttime = jiffies;
		xfs_buf_ioerror_alert(bp, __func__);
	}
	lasttarg = bp->b_target;

	/*
	 * If the write was asynchronous then no one will be looking for the
	 * error.  Clear the error state and write the buffer out again.
	 *
	 * During sync or umount we'll write all pending buffers again
	 * synchronous, which will catch these errors if they keep hanging
	 * around.
	 */
	if (XFS_BUF_ISASYNC(bp)) {
		xfs_buf_ioerror(bp, 0); /* errno of 0 unsets the flag */

		if (!XFS_BUF_ISSTALE(bp)) {
			xfs_buf_delwri_queue(bp);
			XFS_BUF_DONE(bp);
		}
		ASSERT(bp->b_iodone != NULL);
		trace_xfs_buf_item_iodone_async(bp, _RET_IP_);
		xfs_buf_relse(bp);
		return;
	}

	/*
	 * If the write of the buffer was synchronous, we want to make
	 * sure to return the error to the caller of xfs_bwrite().
	 */
	xfs_buf_stale(bp);
	XFS_BUF_DONE(bp);

	trace_xfs_buf_error_relse(bp, _RET_IP_);

do_callbacks:
	xfs_buf_do_callbacks(bp);
	bp->b_fspriv = NULL;
	bp->b_iodone = NULL;
	xfs_buf_ioend(bp, 0);
}

/*
 * This is the iodone() function for buffers which have been
 * logged.  It is called when they are eventually flushed out.
 * It should remove the buf item from the AIL, and free the buf item.
 * It is called by xfs_buf_iodone_callbacks() above which will take
 * care of cleaning up the buffer itself.
 */
void
xfs_buf_iodone(
	struct xfs_buf		*bp,
	struct xfs_log_item	*lip)
{
	struct xfs_ail		*ailp = lip->li_ailp;

	ASSERT(BUF_ITEM(lip)->bli_buf == bp);

	xfs_buf_rele(bp);

	/*
	 * If we are forcibly shutting down, this may well be
	 * off the AIL already. That's because we simulate the
	 * log-committed callbacks to unpin these buffers. Or we may never
	 * have put this item on AIL because of the transaction was
	 * aborted forcibly. xfs_trans_ail_delete() takes care of these.
	 *
	 * Either way, AIL is useless if we're forcing a shutdown.
	 */
	spin_lock(&ailp->xa_lock);
	xfs_trans_ail_delete(ailp, lip);
	xfs_buf_item_free(BUF_ITEM(lip));
=======
void
xfs_buf_item_done(
	struct xfs_buf		*bp)
{
	/*
	 * If we are forcibly shutting down, this may well be off the AIL
	 * already. That's because we simulate the log-committed callbacks to
	 * unpin these buffers. Or we may never have put this item on AIL
	 * because of the transaction was aborted forcibly.
	 * xfs_trans_ail_delete() takes care of these.
	 *
	 * Either way, AIL is useless if we're forcing a shutdown.
	 *
	 * Note that log recovery writes might have buffer items that are not on
	 * the AIL even when the file system is not shut down.
	 */
	xfs_trans_ail_delete(&bp->b_log_item->bli_item,
			     (bp->b_flags & _XBF_LOGRECOVERY) ? 0 :
			     SHUTDOWN_CORRUPT_INCORE);
	xfs_buf_item_relse(bp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
