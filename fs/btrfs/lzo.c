<<<<<<< HEAD
/*
 * Copyright (C) 2008 Oracle.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License v2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 021110-1307, USA.
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2008 Oracle.  All rights reserved.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/kernel.h>
#include <linux/slab.h>
<<<<<<< HEAD
#include <linux/vmalloc.h>
=======
#include <linux/mm.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/init.h>
#include <linux/err.h>
#include <linux/sched.h>
#include <linux/pagemap.h>
#include <linux/bio.h>
#include <linux/lzo.h>
<<<<<<< HEAD
#include "compression.h"

#define LZO_LEN	4

struct workspace {
	void *mem;
	void *buf;	/* where compressed data goes */
	void *cbuf;	/* where decompressed data goes */
	struct list_head list;
};

static void lzo_free_workspace(struct list_head *ws)
{
	struct workspace *workspace = list_entry(ws, struct workspace, list);

	vfree(workspace->buf);
	vfree(workspace->cbuf);
	vfree(workspace->mem);
	kfree(workspace);
}

static struct list_head *lzo_alloc_workspace(void)
{
	struct workspace *workspace;

	workspace = kzalloc(sizeof(*workspace), GFP_NOFS);
	if (!workspace)
		return ERR_PTR(-ENOMEM);

	workspace->mem = vmalloc(LZO1X_MEM_COMPRESS);
	workspace->buf = vmalloc(lzo1x_worst_compress(PAGE_CACHE_SIZE));
	workspace->cbuf = vmalloc(lzo1x_worst_compress(PAGE_CACHE_SIZE));
=======
#include <linux/refcount.h>
#include "messages.h"
#include "compression.h"
#include "ctree.h"
#include "super.h"
#include "btrfs_inode.h"

#define LZO_LEN	4

/*
 * Btrfs LZO compression format
 *
 * Regular and inlined LZO compressed data extents consist of:
 *
 * 1.  Header
 *     Fixed size. LZO_LEN (4) bytes long, LE32.
 *     Records the total size (including the header) of compressed data.
 *
 * 2.  Segment(s)
 *     Variable size. Each segment includes one segment header, followed by data
 *     payload.
 *     One regular LZO compressed extent can have one or more segments.
 *     For inlined LZO compressed extent, only one segment is allowed.
 *     One segment represents at most one sector of uncompressed data.
 *
 * 2.1 Segment header
 *     Fixed size. LZO_LEN (4) bytes long, LE32.
 *     Records the total size of the segment (not including the header).
 *     Segment header never crosses sector boundary, thus it's possible to
 *     have at most 3 padding zeros at the end of the sector.
 *
 * 2.2 Data Payload
 *     Variable size. Size up limit should be lzo1x_worst_compress(sectorsize)
 *     which is 4419 for a 4KiB sectorsize.
 *
 * Example with 4K sectorsize:
 * Page 1:
 *          0     0x2   0x4   0x6   0x8   0xa   0xc   0xe     0x10
 * 0x0000   |  Header   | SegHdr 01 | Data payload 01 ...     |
 * ...
 * 0x0ff0   | SegHdr  N | Data payload  N     ...          |00|
 *                                                          ^^ padding zeros
 * Page 2:
 * 0x1000   | SegHdr N+1| Data payload N+1 ...                |
 */

#define WORKSPACE_BUF_LENGTH	(lzo1x_worst_compress(PAGE_SIZE))
#define WORKSPACE_CBUF_LENGTH	(lzo1x_worst_compress(PAGE_SIZE))

struct workspace {
	void *mem;
	void *buf;	/* where decompressed data goes */
	void *cbuf;	/* where compressed data goes */
	struct list_head list;
};

static struct workspace_manager wsm;

void lzo_free_workspace(struct list_head *ws)
{
	struct workspace *workspace = list_entry(ws, struct workspace, list);

	kvfree(workspace->buf);
	kvfree(workspace->cbuf);
	kvfree(workspace->mem);
	kfree(workspace);
}

struct list_head *lzo_alloc_workspace(unsigned int level)
{
	struct workspace *workspace;

	workspace = kzalloc(sizeof(*workspace), GFP_KERNEL);
	if (!workspace)
		return ERR_PTR(-ENOMEM);

	workspace->mem = kvmalloc(LZO1X_MEM_COMPRESS, GFP_KERNEL | __GFP_NOWARN);
	workspace->buf = kvmalloc(WORKSPACE_BUF_LENGTH, GFP_KERNEL | __GFP_NOWARN);
	workspace->cbuf = kvmalloc(WORKSPACE_CBUF_LENGTH, GFP_KERNEL | __GFP_NOWARN);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!workspace->mem || !workspace->buf || !workspace->cbuf)
		goto fail;

	INIT_LIST_HEAD(&workspace->list);

	return &workspace->list;
fail:
	lzo_free_workspace(&workspace->list);
	return ERR_PTR(-ENOMEM);
}

static inline void write_compress_length(char *buf, size_t len)
{
	__le32 dlen;

	dlen = cpu_to_le32(len);
	memcpy(buf, &dlen, LZO_LEN);
}

<<<<<<< HEAD
static inline size_t read_compress_length(char *buf)
=======
static inline size_t read_compress_length(const char *buf)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	__le32 dlen;

	memcpy(&dlen, buf, LZO_LEN);
	return le32_to_cpu(dlen);
}

<<<<<<< HEAD
static int lzo_compress_pages(struct list_head *ws,
			      struct address_space *mapping,
			      u64 start, unsigned long len,
			      struct page **pages,
			      unsigned long nr_dest_pages,
			      unsigned long *out_pages,
			      unsigned long *total_in,
			      unsigned long *total_out,
			      unsigned long max_out)
{
	struct workspace *workspace = list_entry(ws, struct workspace, list);
	int ret = 0;
	char *data_in;
	char *cpage_out;
	int nr_pages = 0;
	struct page *in_page = NULL;
	struct page *out_page = NULL;
	unsigned long bytes_left;

	size_t in_len;
	size_t out_len;
	char *buf;
	unsigned long tot_in = 0;
	unsigned long tot_out = 0;
	unsigned long pg_bytes_left;
	unsigned long out_offset;
	unsigned long bytes;

=======
/*
 * Will do:
 *
 * - Write a segment header into the destination
 * - Copy the compressed buffer into the destination
 * - Make sure we have enough space in the last sector to fit a segment header
 *   If not, we will pad at most (LZO_LEN (4)) - 1 bytes of zeros.
 *
 * Will allocate new pages when needed.
 */
static int copy_compressed_data_to_page(char *compressed_data,
					size_t compressed_size,
					struct page **out_pages,
					unsigned long max_nr_page,
					u32 *cur_out,
					const u32 sectorsize)
{
	u32 sector_bytes_left;
	u32 orig_out;
	struct page *cur_page;
	char *kaddr;

	if ((*cur_out / PAGE_SIZE) >= max_nr_page)
		return -E2BIG;

	/*
	 * We never allow a segment header crossing sector boundary, previous
	 * run should ensure we have enough space left inside the sector.
	 */
	ASSERT((*cur_out / sectorsize) == (*cur_out + LZO_LEN - 1) / sectorsize);

	cur_page = out_pages[*cur_out / PAGE_SIZE];
	/* Allocate a new page */
	if (!cur_page) {
		cur_page = btrfs_alloc_compr_page();
		if (!cur_page)
			return -ENOMEM;
		out_pages[*cur_out / PAGE_SIZE] = cur_page;
	}

	kaddr = kmap_local_page(cur_page);
	write_compress_length(kaddr + offset_in_page(*cur_out),
			      compressed_size);
	*cur_out += LZO_LEN;

	orig_out = *cur_out;

	/* Copy compressed data */
	while (*cur_out - orig_out < compressed_size) {
		u32 copy_len = min_t(u32, sectorsize - *cur_out % sectorsize,
				     orig_out + compressed_size - *cur_out);

		kunmap_local(kaddr);

		if ((*cur_out / PAGE_SIZE) >= max_nr_page)
			return -E2BIG;

		cur_page = out_pages[*cur_out / PAGE_SIZE];
		/* Allocate a new page */
		if (!cur_page) {
			cur_page = btrfs_alloc_compr_page();
			if (!cur_page)
				return -ENOMEM;
			out_pages[*cur_out / PAGE_SIZE] = cur_page;
		}
		kaddr = kmap_local_page(cur_page);

		memcpy(kaddr + offset_in_page(*cur_out),
		       compressed_data + *cur_out - orig_out, copy_len);

		*cur_out += copy_len;
	}

	/*
	 * Check if we can fit the next segment header into the remaining space
	 * of the sector.
	 */
	sector_bytes_left = round_up(*cur_out, sectorsize) - *cur_out;
	if (sector_bytes_left >= LZO_LEN || sector_bytes_left == 0)
		goto out;

	/* The remaining size is not enough, pad it with zeros */
	memset(kaddr + offset_in_page(*cur_out), 0,
	       sector_bytes_left);
	*cur_out += sector_bytes_left;

out:
	kunmap_local(kaddr);
	return 0;
}

int lzo_compress_pages(struct list_head *ws, struct address_space *mapping,
		u64 start, struct page **pages, unsigned long *out_pages,
		unsigned long *total_in, unsigned long *total_out)
{
	struct workspace *workspace = list_entry(ws, struct workspace, list);
	const u32 sectorsize = inode_to_fs_info(mapping->host)->sectorsize;
	struct page *page_in = NULL;
	char *sizes_ptr;
	const unsigned long max_nr_page = *out_pages;
	int ret = 0;
	/* Points to the file offset of input data */
	u64 cur_in = start;
	/* Points to the current output byte */
	u32 cur_out = 0;
	u32 len = *total_out;

	ASSERT(max_nr_page > 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	*out_pages = 0;
	*total_out = 0;
	*total_in = 0;

<<<<<<< HEAD
	in_page = find_get_page(mapping, start >> PAGE_CACHE_SHIFT);
	data_in = kmap(in_page);

	/*
	 * store the size of all chunks of compressed data in
	 * the first 4 bytes
	 */
	out_page = alloc_page(GFP_NOFS | __GFP_HIGHMEM);
	if (out_page == NULL) {
		ret = -ENOMEM;
		goto out;
	}
	cpage_out = kmap(out_page);
	out_offset = LZO_LEN;
	tot_out = LZO_LEN;
	pages[0] = out_page;
	nr_pages = 1;
	pg_bytes_left = PAGE_CACHE_SIZE - LZO_LEN;

	/* compress at most one page of data each time */
	in_len = min(len, PAGE_CACHE_SIZE);
	while (tot_in < len) {
		ret = lzo1x_1_compress(data_in, in_len, workspace->cbuf,
				       &out_len, workspace->mem);
		if (ret != LZO_E_OK) {
			printk(KERN_DEBUG "btrfs deflate in loop returned %d\n",
			       ret);
			ret = -1;
			goto out;
		}

		/* store the size of this chunk of compressed data */
		write_compress_length(cpage_out + out_offset, out_len);
		tot_out += LZO_LEN;
		out_offset += LZO_LEN;
		pg_bytes_left -= LZO_LEN;

		tot_in += in_len;
		tot_out += out_len;

		/* copy bytes from the working buffer into the pages */
		buf = workspace->cbuf;
		while (out_len) {
			bytes = min_t(unsigned long, pg_bytes_left, out_len);

			memcpy(cpage_out + out_offset, buf, bytes);

			out_len -= bytes;
			pg_bytes_left -= bytes;
			buf += bytes;
			out_offset += bytes;

			/*
			 * we need another page for writing out.
			 *
			 * Note if there's less than 4 bytes left, we just
			 * skip to a new page.
			 */
			if ((out_len == 0 && pg_bytes_left < LZO_LEN) ||
			    pg_bytes_left == 0) {
				if (pg_bytes_left) {
					memset(cpage_out + out_offset, 0,
					       pg_bytes_left);
					tot_out += pg_bytes_left;
				}

				/* we're done, don't allocate new page */
				if (out_len == 0 && tot_in >= len)
					break;

				kunmap(out_page);
				if (nr_pages == nr_dest_pages) {
					out_page = NULL;
					ret = -1;
					goto out;
				}

				out_page = alloc_page(GFP_NOFS | __GFP_HIGHMEM);
				if (out_page == NULL) {
					ret = -ENOMEM;
					goto out;
				}
				cpage_out = kmap(out_page);
				pages[nr_pages++] = out_page;

				pg_bytes_left = PAGE_CACHE_SIZE;
				out_offset = 0;
			}
		}

		/* we're making it bigger, give up */
		if (tot_in > 8192 && tot_in < tot_out)
			goto out;

		/* we're all done */
		if (tot_in >= len)
			break;

		if (tot_out > max_out)
			break;

		bytes_left = len - tot_in;
		kunmap(in_page);
		page_cache_release(in_page);

		start += PAGE_CACHE_SIZE;
		in_page = find_get_page(mapping, start >> PAGE_CACHE_SHIFT);
		data_in = kmap(in_page);
		in_len = min(bytes_left, PAGE_CACHE_SIZE);
	}

	if (tot_out > tot_in)
		goto out;

	/* store the size of all chunks of compressed data */
	cpage_out = kmap(pages[0]);
	write_compress_length(cpage_out, tot_out);

	kunmap(pages[0]);

	ret = 0;
	*total_out = tot_out;
	*total_in = tot_in;
out:
	*out_pages = nr_pages;
	if (out_page)
		kunmap(out_page);

	if (in_page) {
		kunmap(in_page);
		page_cache_release(in_page);
	}

	return ret;
}

static int lzo_decompress_biovec(struct list_head *ws,
				 struct page **pages_in,
				 u64 disk_start,
				 struct bio_vec *bvec,
				 int vcnt,
				 size_t srclen)
{
	struct workspace *workspace = list_entry(ws, struct workspace, list);
	int ret = 0, ret2;
	char *data_in;
	unsigned long page_in_index = 0;
	unsigned long page_out_index = 0;
	unsigned long total_pages_in = (srclen + PAGE_CACHE_SIZE - 1) /
					PAGE_CACHE_SIZE;
	unsigned long buf_start;
	unsigned long buf_offset = 0;
	unsigned long bytes;
	unsigned long working_bytes;
	unsigned long pg_offset;

	size_t in_len;
	size_t out_len;
	unsigned long in_offset;
	unsigned long in_page_bytes_left;
	unsigned long tot_in;
	unsigned long tot_out;
	unsigned long tot_len;
	char *buf;
	bool may_late_unmap, need_unmap;

	data_in = kmap(pages_in[0]);
	tot_len = read_compress_length(data_in);

	tot_in = LZO_LEN;
	in_offset = LZO_LEN;
	tot_len = min_t(size_t, srclen, tot_len);
	in_page_bytes_left = PAGE_CACHE_SIZE - LZO_LEN;

	tot_out = 0;
	pg_offset = 0;

	while (tot_in < tot_len) {
		in_len = read_compress_length(data_in + in_offset);
		in_page_bytes_left -= LZO_LEN;
		in_offset += LZO_LEN;
		tot_in += LZO_LEN;

		tot_in += in_len;
		working_bytes = in_len;
		may_late_unmap = need_unmap = false;

		/* fast path: avoid using the working buffer */
		if (in_page_bytes_left >= in_len) {
			buf = data_in + in_offset;
			bytes = in_len;
			may_late_unmap = true;
			goto cont;
		}

		/* copy bytes from the pages into the working buffer */
		buf = workspace->cbuf;
		buf_offset = 0;
		while (working_bytes) {
			bytes = min(working_bytes, in_page_bytes_left);

			memcpy(buf + buf_offset, data_in + in_offset, bytes);
			buf_offset += bytes;
cont:
			working_bytes -= bytes;
			in_page_bytes_left -= bytes;
			in_offset += bytes;

			/* check if we need to pick another page */
			if ((working_bytes == 0 && in_page_bytes_left < LZO_LEN)
			    || in_page_bytes_left == 0) {
				tot_in += in_page_bytes_left;

				if (working_bytes == 0 && tot_in >= tot_len)
					break;

				if (page_in_index + 1 >= total_pages_in) {
					ret = -1;
					goto done;
				}

				if (may_late_unmap)
					need_unmap = true;
				else
					kunmap(pages_in[page_in_index]);

				data_in = kmap(pages_in[++page_in_index]);

				in_page_bytes_left = PAGE_CACHE_SIZE;
				in_offset = 0;
			}
		}

		out_len = lzo1x_worst_compress(PAGE_CACHE_SIZE);
		ret = lzo1x_decompress_safe(buf, in_len, workspace->buf,
					    &out_len);
		if (need_unmap)
			kunmap(pages_in[page_in_index - 1]);
		if (ret != LZO_E_OK) {
			printk(KERN_WARNING "btrfs decompress failed\n");
			ret = -1;
			break;
		}

		buf_start = tot_out;
		tot_out += out_len;

		ret2 = btrfs_decompress_buf2page(workspace->buf, buf_start,
						 tot_out, disk_start,
						 bvec, vcnt,
						 &page_out_index, &pg_offset);
		if (ret2 == 0)
			break;
	}
done:
	kunmap(pages_in[page_in_index]);
	return ret;
}

static int lzo_decompress(struct list_head *ws, unsigned char *data_in,
			  struct page *dest_page,
			  unsigned long start_byte,
			  size_t srclen, size_t destlen)
{
	struct workspace *workspace = list_entry(ws, struct workspace, list);
	size_t in_len;
	size_t out_len;
	size_t tot_len;
	int ret = 0;
	char *kaddr;
	unsigned long bytes;

	BUG_ON(srclen < LZO_LEN);

	tot_len = read_compress_length(data_in);
	data_in += LZO_LEN;

	in_len = read_compress_length(data_in);
	data_in += LZO_LEN;

	out_len = PAGE_CACHE_SIZE;
	ret = lzo1x_decompress_safe(data_in, in_len, workspace->buf, &out_len);
	if (ret != LZO_E_OK) {
		printk(KERN_WARNING "btrfs decompress failed!\n");
		ret = -1;
		goto out;
	}

	if (out_len < start_byte) {
		ret = -1;
		goto out;
	}

	bytes = min_t(unsigned long, destlen, out_len - start_byte);

	kaddr = kmap_atomic(dest_page);
	memcpy(kaddr, workspace->buf + start_byte, bytes);
	kunmap_atomic(kaddr);
=======
	/*
	 * Skip the header for now, we will later come back and write the total
	 * compressed size
	 */
	cur_out += LZO_LEN;
	while (cur_in < start + len) {
		char *data_in;
		const u32 sectorsize_mask = sectorsize - 1;
		u32 sector_off = (cur_in - start) & sectorsize_mask;
		u32 in_len;
		size_t out_len;

		/* Get the input page first */
		if (!page_in) {
			page_in = find_get_page(mapping, cur_in >> PAGE_SHIFT);
			ASSERT(page_in);
		}

		/* Compress at most one sector of data each time */
		in_len = min_t(u32, start + len - cur_in, sectorsize - sector_off);
		ASSERT(in_len);
		data_in = kmap_local_page(page_in);
		ret = lzo1x_1_compress(data_in +
				       offset_in_page(cur_in), in_len,
				       workspace->cbuf, &out_len,
				       workspace->mem);
		kunmap_local(data_in);
		if (ret < 0) {
			pr_debug("BTRFS: lzo in loop returned %d\n", ret);
			ret = -EIO;
			goto out;
		}

		ret = copy_compressed_data_to_page(workspace->cbuf, out_len,
						   pages, max_nr_page,
						   &cur_out, sectorsize);
		if (ret < 0)
			goto out;

		cur_in += in_len;

		/*
		 * Check if we're making it bigger after two sectors.  And if
		 * it is so, give up.
		 */
		if (cur_in - start > sectorsize * 2 && cur_in - start < cur_out) {
			ret = -E2BIG;
			goto out;
		}

		/* Check if we have reached page boundary */
		if (PAGE_ALIGNED(cur_in)) {
			put_page(page_in);
			page_in = NULL;
		}
	}

	/* Store the size of all chunks of compressed data */
	sizes_ptr = kmap_local_page(pages[0]);
	write_compress_length(sizes_ptr, cur_out);
	kunmap_local(sizes_ptr);

	ret = 0;
	*total_out = cur_out;
	*total_in = cur_in - start;
out:
	if (page_in)
		put_page(page_in);
	*out_pages = DIV_ROUND_UP(cur_out, PAGE_SIZE);
	return ret;
}

/*
 * Copy the compressed segment payload into @dest.
 *
 * For the payload there will be no padding, just need to do page switching.
 */
static void copy_compressed_segment(struct compressed_bio *cb,
				    char *dest, u32 len, u32 *cur_in)
{
	u32 orig_in = *cur_in;

	while (*cur_in < orig_in + len) {
		struct page *cur_page;
		u32 copy_len = min_t(u32, PAGE_SIZE - offset_in_page(*cur_in),
					  orig_in + len - *cur_in);

		ASSERT(copy_len);
		cur_page = cb->compressed_pages[*cur_in / PAGE_SIZE];

		memcpy_from_page(dest + *cur_in - orig_in, cur_page,
				 offset_in_page(*cur_in), copy_len);

		*cur_in += copy_len;
	}
}

int lzo_decompress_bio(struct list_head *ws, struct compressed_bio *cb)
{
	struct workspace *workspace = list_entry(ws, struct workspace, list);
	const struct btrfs_fs_info *fs_info = cb->bbio.inode->root->fs_info;
	const u32 sectorsize = fs_info->sectorsize;
	char *kaddr;
	int ret;
	/* Compressed data length, can be unaligned */
	u32 len_in;
	/* Offset inside the compressed data */
	u32 cur_in = 0;
	/* Bytes decompressed so far */
	u32 cur_out = 0;

	kaddr = kmap_local_page(cb->compressed_pages[0]);
	len_in = read_compress_length(kaddr);
	kunmap_local(kaddr);
	cur_in += LZO_LEN;

	/*
	 * LZO header length check
	 *
	 * The total length should not exceed the maximum extent length,
	 * and all sectors should be used.
	 * If this happens, it means the compressed extent is corrupted.
	 */
	if (len_in > min_t(size_t, BTRFS_MAX_COMPRESSED, cb->compressed_len) ||
	    round_up(len_in, sectorsize) < cb->compressed_len) {
		btrfs_err(fs_info,
			"invalid lzo header, lzo len %u compressed len %u",
			len_in, cb->compressed_len);
		return -EUCLEAN;
	}

	/* Go through each lzo segment */
	while (cur_in < len_in) {
		struct page *cur_page;
		/* Length of the compressed segment */
		u32 seg_len;
		u32 sector_bytes_left;
		size_t out_len = lzo1x_worst_compress(sectorsize);

		/*
		 * We should always have enough space for one segment header
		 * inside current sector.
		 */
		ASSERT(cur_in / sectorsize ==
		       (cur_in + LZO_LEN - 1) / sectorsize);
		cur_page = cb->compressed_pages[cur_in / PAGE_SIZE];
		ASSERT(cur_page);
		kaddr = kmap_local_page(cur_page);
		seg_len = read_compress_length(kaddr + offset_in_page(cur_in));
		kunmap_local(kaddr);
		cur_in += LZO_LEN;

		if (seg_len > WORKSPACE_CBUF_LENGTH) {
			/*
			 * seg_len shouldn't be larger than we have allocated
			 * for workspace->cbuf
			 */
			btrfs_err(fs_info, "unexpectedly large lzo segment len %u",
					seg_len);
			return -EIO;
		}

		/* Copy the compressed segment payload into workspace */
		copy_compressed_segment(cb, workspace->cbuf, seg_len, &cur_in);

		/* Decompress the data */
		ret = lzo1x_decompress_safe(workspace->cbuf, seg_len,
					    workspace->buf, &out_len);
		if (ret != LZO_E_OK) {
			btrfs_err(fs_info, "failed to decompress");
			return -EIO;
		}

		/* Copy the data into inode pages */
		ret = btrfs_decompress_buf2page(workspace->buf, out_len, cb, cur_out);
		cur_out += out_len;

		/* All data read, exit */
		if (ret == 0)
			return 0;
		ret = 0;

		/* Check if the sector has enough space for a segment header */
		sector_bytes_left = sectorsize - (cur_in % sectorsize);
		if (sector_bytes_left >= LZO_LEN)
			continue;

		/* Skip the padding zeros */
		cur_in += sector_bytes_left;
	}

	return 0;
}

int lzo_decompress(struct list_head *ws, const u8 *data_in,
		struct page *dest_page, unsigned long dest_pgoff, size_t srclen,
		size_t destlen)
{
	struct workspace *workspace = list_entry(ws, struct workspace, list);
	struct btrfs_fs_info *fs_info = page_to_fs_info(dest_page);
	const u32 sectorsize = fs_info->sectorsize;
	size_t in_len;
	size_t out_len;
	size_t max_segment_len = WORKSPACE_BUF_LENGTH;
	int ret = 0;

	if (srclen < LZO_LEN || srclen > max_segment_len + LZO_LEN * 2)
		return -EUCLEAN;

	in_len = read_compress_length(data_in);
	if (in_len != srclen)
		return -EUCLEAN;
	data_in += LZO_LEN;

	in_len = read_compress_length(data_in);
	if (in_len != srclen - LZO_LEN * 2) {
		ret = -EUCLEAN;
		goto out;
	}
	data_in += LZO_LEN;

	out_len = sectorsize;
	ret = lzo1x_decompress_safe(data_in, in_len, workspace->buf, &out_len);
	if (ret != LZO_E_OK) {
		pr_warn("BTRFS: decompress failed!\n");
		ret = -EIO;
		goto out;
	}

	ASSERT(out_len <= sectorsize);
	memcpy_to_page(dest_page, dest_pgoff, workspace->buf, out_len);
	/* Early end, considered as an error. */
	if (unlikely(out_len < destlen)) {
		ret = -EIO;
		memzero_page(dest_page, dest_pgoff + out_len, destlen - out_len);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	return ret;
}

<<<<<<< HEAD
struct btrfs_compress_op btrfs_lzo_compress = {
	.alloc_workspace	= lzo_alloc_workspace,
	.free_workspace		= lzo_free_workspace,
	.compress_pages		= lzo_compress_pages,
	.decompress_biovec	= lzo_decompress_biovec,
	.decompress		= lzo_decompress,
=======
const struct btrfs_compress_op btrfs_lzo_compress = {
	.workspace_manager	= &wsm,
	.max_level		= 1,
	.default_level		= 1,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};
