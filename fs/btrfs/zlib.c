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
 *
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2008 Oracle.  All rights reserved.
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Based on jffs2 zlib code:
 * Copyright © 2001-2007 Red Hat, Inc.
 * Created by David Woodhouse <dwmw2@infradead.org>
 */

#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/zlib.h>
#include <linux/zutil.h>
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
<<<<<<< HEAD
#include "compression.h"

struct workspace {
	z_stream inf_strm;
	z_stream def_strm;
	char *buf;
	struct list_head list;
};

static void zlib_free_workspace(struct list_head *ws)
{
	struct workspace *workspace = list_entry(ws, struct workspace, list);

	vfree(workspace->def_strm.workspace);
	vfree(workspace->inf_strm.workspace);
=======
#include <linux/refcount.h>
#include "compression.h"

/* workspace buffer size for s390 zlib hardware support */
#define ZLIB_DFLTCC_BUF_SIZE    (4 * PAGE_SIZE)

struct workspace {
	z_stream strm;
	char *buf;
	unsigned int buf_size;
	struct list_head list;
	int level;
};

static struct workspace_manager wsm;

struct list_head *zlib_get_workspace(unsigned int level)
{
	struct list_head *ws = btrfs_get_workspace(BTRFS_COMPRESS_ZLIB, level);
	struct workspace *workspace = list_entry(ws, struct workspace, list);

	workspace->level = level;

	return ws;
}

void zlib_free_workspace(struct list_head *ws)
{
	struct workspace *workspace = list_entry(ws, struct workspace, list);

	kvfree(workspace->strm.workspace);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kfree(workspace->buf);
	kfree(workspace);
}

<<<<<<< HEAD
static struct list_head *zlib_alloc_workspace(void)
{
	struct workspace *workspace;

	workspace = kzalloc(sizeof(*workspace), GFP_NOFS);
	if (!workspace)
		return ERR_PTR(-ENOMEM);

	workspace->def_strm.workspace = vmalloc(zlib_deflate_workspacesize(
						MAX_WBITS, MAX_MEM_LEVEL));
	workspace->inf_strm.workspace = vmalloc(zlib_inflate_workspacesize());
	workspace->buf = kmalloc(PAGE_CACHE_SIZE, GFP_NOFS);
	if (!workspace->def_strm.workspace ||
	    !workspace->inf_strm.workspace || !workspace->buf)
=======
struct list_head *zlib_alloc_workspace(unsigned int level)
{
	struct workspace *workspace;
	int workspacesize;

	workspace = kzalloc(sizeof(*workspace), GFP_KERNEL);
	if (!workspace)
		return ERR_PTR(-ENOMEM);

	workspacesize = max(zlib_deflate_workspacesize(MAX_WBITS, MAX_MEM_LEVEL),
			zlib_inflate_workspacesize());
	workspace->strm.workspace = kvzalloc(workspacesize, GFP_KERNEL | __GFP_NOWARN);
	workspace->level = level;
	workspace->buf = NULL;
	/*
	 * In case of s390 zlib hardware support, allocate lager workspace
	 * buffer. If allocator fails, fall back to a single page buffer.
	 */
	if (zlib_deflate_dfltcc_enabled()) {
		workspace->buf = kmalloc(ZLIB_DFLTCC_BUF_SIZE,
					 __GFP_NOMEMALLOC | __GFP_NORETRY |
					 __GFP_NOWARN | GFP_NOIO);
		workspace->buf_size = ZLIB_DFLTCC_BUF_SIZE;
	}
	if (!workspace->buf) {
		workspace->buf = kmalloc(PAGE_SIZE, GFP_KERNEL);
		workspace->buf_size = PAGE_SIZE;
	}
	if (!workspace->strm.workspace || !workspace->buf)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto fail;

	INIT_LIST_HEAD(&workspace->list);

	return &workspace->list;
fail:
	zlib_free_workspace(&workspace->list);
	return ERR_PTR(-ENOMEM);
}

<<<<<<< HEAD
static int zlib_compress_pages(struct list_head *ws,
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
	int ret;
	char *data_in;
=======
int zlib_compress_pages(struct list_head *ws, struct address_space *mapping,
		u64 start, struct page **pages, unsigned long *out_pages,
		unsigned long *total_in, unsigned long *total_out)
{
	struct workspace *workspace = list_entry(ws, struct workspace, list);
	int ret;
	char *data_in = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	char *cpage_out;
	int nr_pages = 0;
	struct page *in_page = NULL;
	struct page *out_page = NULL;
	unsigned long bytes_left;
<<<<<<< HEAD
=======
	unsigned int in_buf_pages;
	unsigned long len = *total_out;
	unsigned long nr_dest_pages = *out_pages;
	const unsigned long max_out = nr_dest_pages * PAGE_SIZE;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	*out_pages = 0;
	*total_out = 0;
	*total_in = 0;

<<<<<<< HEAD
	if (Z_OK != zlib_deflateInit(&workspace->def_strm, 3)) {
		printk(KERN_WARNING "deflateInit failed\n");
		ret = -1;
		goto out;
	}

	workspace->def_strm.total_in = 0;
	workspace->def_strm.total_out = 0;

	in_page = find_get_page(mapping, start >> PAGE_CACHE_SHIFT);
	data_in = kmap(in_page);

	out_page = alloc_page(GFP_NOFS | __GFP_HIGHMEM);
	if (out_page == NULL) {
		ret = -1;
		goto out;
	}
	cpage_out = kmap(out_page);
	pages[0] = out_page;
	nr_pages = 1;

	workspace->def_strm.next_in = data_in;
	workspace->def_strm.next_out = cpage_out;
	workspace->def_strm.avail_out = PAGE_CACHE_SIZE;
	workspace->def_strm.avail_in = min(len, PAGE_CACHE_SIZE);

	while (workspace->def_strm.total_in < len) {
		ret = zlib_deflate(&workspace->def_strm, Z_SYNC_FLUSH);
		if (ret != Z_OK) {
			printk(KERN_DEBUG "btrfs deflate in loop returned %d\n",
			       ret);
			zlib_deflateEnd(&workspace->def_strm);
			ret = -1;
=======
	if (Z_OK != zlib_deflateInit(&workspace->strm, workspace->level)) {
		pr_warn("BTRFS: deflateInit failed\n");
		ret = -EIO;
		goto out;
	}

	workspace->strm.total_in = 0;
	workspace->strm.total_out = 0;

	out_page = btrfs_alloc_compr_page();
	if (out_page == NULL) {
		ret = -ENOMEM;
		goto out;
	}
	cpage_out = page_address(out_page);
	pages[0] = out_page;
	nr_pages = 1;

	workspace->strm.next_in = workspace->buf;
	workspace->strm.avail_in = 0;
	workspace->strm.next_out = cpage_out;
	workspace->strm.avail_out = PAGE_SIZE;

	while (workspace->strm.total_in < len) {
		/*
		 * Get next input pages and copy the contents to
		 * the workspace buffer if required.
		 */
		if (workspace->strm.avail_in == 0) {
			bytes_left = len - workspace->strm.total_in;
			in_buf_pages = min(DIV_ROUND_UP(bytes_left, PAGE_SIZE),
					   workspace->buf_size / PAGE_SIZE);
			if (in_buf_pages > 1) {
				int i;

				for (i = 0; i < in_buf_pages; i++) {
					if (data_in) {
						kunmap_local(data_in);
						put_page(in_page);
					}
					in_page = find_get_page(mapping,
								start >> PAGE_SHIFT);
					data_in = kmap_local_page(in_page);
					copy_page(workspace->buf + i * PAGE_SIZE,
						  data_in);
					start += PAGE_SIZE;
				}
				workspace->strm.next_in = workspace->buf;
			} else {
				if (data_in) {
					kunmap_local(data_in);
					put_page(in_page);
				}
				in_page = find_get_page(mapping,
							start >> PAGE_SHIFT);
				data_in = kmap_local_page(in_page);
				start += PAGE_SIZE;
				workspace->strm.next_in = data_in;
			}
			workspace->strm.avail_in = min(bytes_left,
						       (unsigned long) workspace->buf_size);
		}

		ret = zlib_deflate(&workspace->strm, Z_SYNC_FLUSH);
		if (ret != Z_OK) {
			pr_debug("BTRFS: deflate in loop returned %d\n",
			       ret);
			zlib_deflateEnd(&workspace->strm);
			ret = -EIO;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto out;
		}

		/* we're making it bigger, give up */
<<<<<<< HEAD
		if (workspace->def_strm.total_in > 8192 &&
		    workspace->def_strm.total_in <
		    workspace->def_strm.total_out) {
			ret = -1;
=======
		if (workspace->strm.total_in > 8192 &&
		    workspace->strm.total_in <
		    workspace->strm.total_out) {
			ret = -E2BIG;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto out;
		}
		/* we need another page for writing out.  Test this
		 * before the total_in so we will pull in a new page for
		 * the stream end if required
		 */
<<<<<<< HEAD
		if (workspace->def_strm.avail_out == 0) {
			kunmap(out_page);
			if (nr_pages == nr_dest_pages) {
				out_page = NULL;
				ret = -1;
				goto out;
			}
			out_page = alloc_page(GFP_NOFS | __GFP_HIGHMEM);
			if (out_page == NULL) {
				ret = -1;
				goto out;
			}
			cpage_out = kmap(out_page);
			pages[nr_pages] = out_page;
			nr_pages++;
			workspace->def_strm.avail_out = PAGE_CACHE_SIZE;
			workspace->def_strm.next_out = cpage_out;
		}
		/* we're all done */
		if (workspace->def_strm.total_in >= len)
			break;

		/* we've read in a full page, get a new one */
		if (workspace->def_strm.avail_in == 0) {
			if (workspace->def_strm.total_out > max_out)
				break;

			bytes_left = len - workspace->def_strm.total_in;
			kunmap(in_page);
			page_cache_release(in_page);

			start += PAGE_CACHE_SIZE;
			in_page = find_get_page(mapping,
						start >> PAGE_CACHE_SHIFT);
			data_in = kmap(in_page);
			workspace->def_strm.avail_in = min(bytes_left,
							   PAGE_CACHE_SIZE);
			workspace->def_strm.next_in = data_in;
		}
	}
	workspace->def_strm.avail_in = 0;
	ret = zlib_deflate(&workspace->def_strm, Z_FINISH);
	zlib_deflateEnd(&workspace->def_strm);

	if (ret != Z_STREAM_END) {
		ret = -1;
		goto out;
	}

	if (workspace->def_strm.total_out >= workspace->def_strm.total_in) {
		ret = -1;
=======
		if (workspace->strm.avail_out == 0) {
			if (nr_pages == nr_dest_pages) {
				ret = -E2BIG;
				goto out;
			}
			out_page = btrfs_alloc_compr_page();
			if (out_page == NULL) {
				ret = -ENOMEM;
				goto out;
			}
			cpage_out = page_address(out_page);
			pages[nr_pages] = out_page;
			nr_pages++;
			workspace->strm.avail_out = PAGE_SIZE;
			workspace->strm.next_out = cpage_out;
		}
		/* we're all done */
		if (workspace->strm.total_in >= len)
			break;
		if (workspace->strm.total_out > max_out)
			break;
	}
	workspace->strm.avail_in = 0;
	/*
	 * Call deflate with Z_FINISH flush parameter providing more output
	 * space but no more input data, until it returns with Z_STREAM_END.
	 */
	while (ret != Z_STREAM_END) {
		ret = zlib_deflate(&workspace->strm, Z_FINISH);
		if (ret == Z_STREAM_END)
			break;
		if (ret != Z_OK && ret != Z_BUF_ERROR) {
			zlib_deflateEnd(&workspace->strm);
			ret = -EIO;
			goto out;
		} else if (workspace->strm.avail_out == 0) {
			/* get another page for the stream end */
			if (nr_pages == nr_dest_pages) {
				ret = -E2BIG;
				goto out;
			}
			out_page = btrfs_alloc_compr_page();
			if (out_page == NULL) {
				ret = -ENOMEM;
				goto out;
			}
			cpage_out = page_address(out_page);
			pages[nr_pages] = out_page;
			nr_pages++;
			workspace->strm.avail_out = PAGE_SIZE;
			workspace->strm.next_out = cpage_out;
		}
	}
	zlib_deflateEnd(&workspace->strm);

	if (workspace->strm.total_out >= workspace->strm.total_in) {
		ret = -E2BIG;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;
	}

	ret = 0;
<<<<<<< HEAD
	*total_out = workspace->def_strm.total_out;
	*total_in = workspace->def_strm.total_in;
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

static int zlib_decompress_biovec(struct list_head *ws, struct page **pages_in,
				  u64 disk_start,
				  struct bio_vec *bvec,
				  int vcnt,
				  size_t srclen)
=======
	*total_out = workspace->strm.total_out;
	*total_in = workspace->strm.total_in;
out:
	*out_pages = nr_pages;
	if (data_in) {
		kunmap_local(data_in);
		put_page(in_page);
	}

	return ret;
}

int zlib_decompress_bio(struct list_head *ws, struct compressed_bio *cb)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct workspace *workspace = list_entry(ws, struct workspace, list);
	int ret = 0, ret2;
	int wbits = MAX_WBITS;
	char *data_in;
	size_t total_out = 0;
	unsigned long page_in_index = 0;
<<<<<<< HEAD
	unsigned long page_out_index = 0;
	unsigned long total_pages_in = (srclen + PAGE_CACHE_SIZE - 1) /
					PAGE_CACHE_SIZE;
	unsigned long buf_start;
	unsigned long pg_offset;

	data_in = kmap(pages_in[page_in_index]);
	workspace->inf_strm.next_in = data_in;
	workspace->inf_strm.avail_in = min_t(size_t, srclen, PAGE_CACHE_SIZE);
	workspace->inf_strm.total_in = 0;

	workspace->inf_strm.total_out = 0;
	workspace->inf_strm.next_out = workspace->buf;
	workspace->inf_strm.avail_out = PAGE_CACHE_SIZE;
	pg_offset = 0;
=======
	size_t srclen = cb->compressed_len;
	unsigned long total_pages_in = DIV_ROUND_UP(srclen, PAGE_SIZE);
	unsigned long buf_start;
	struct page **pages_in = cb->compressed_pages;

	data_in = kmap_local_page(pages_in[page_in_index]);
	workspace->strm.next_in = data_in;
	workspace->strm.avail_in = min_t(size_t, srclen, PAGE_SIZE);
	workspace->strm.total_in = 0;

	workspace->strm.total_out = 0;
	workspace->strm.next_out = workspace->buf;
	workspace->strm.avail_out = workspace->buf_size;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* If it's deflate, and it's got no preset dictionary, then
	   we can tell zlib to skip the adler32 check. */
	if (srclen > 2 && !(data_in[1] & PRESET_DICT) &&
	    ((data_in[0] & 0x0f) == Z_DEFLATED) &&
	    !(((data_in[0]<<8) + data_in[1]) % 31)) {

		wbits = -((data_in[0] >> 4) + 8);
<<<<<<< HEAD
		workspace->inf_strm.next_in += 2;
		workspace->inf_strm.avail_in -= 2;
	}

	if (Z_OK != zlib_inflateInit2(&workspace->inf_strm, wbits)) {
		printk(KERN_WARNING "inflateInit failed\n");
		return -1;
	}
	while (workspace->inf_strm.total_in < srclen) {
		ret = zlib_inflate(&workspace->inf_strm, Z_NO_FLUSH);
=======
		workspace->strm.next_in += 2;
		workspace->strm.avail_in -= 2;
	}

	if (Z_OK != zlib_inflateInit2(&workspace->strm, wbits)) {
		pr_warn("BTRFS: inflateInit failed\n");
		kunmap_local(data_in);
		return -EIO;
	}
	while (workspace->strm.total_in < srclen) {
		ret = zlib_inflate(&workspace->strm, Z_NO_FLUSH);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (ret != Z_OK && ret != Z_STREAM_END)
			break;

		buf_start = total_out;
<<<<<<< HEAD
		total_out = workspace->inf_strm.total_out;
=======
		total_out = workspace->strm.total_out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/* we didn't make progress in this inflate call, we're done */
		if (buf_start == total_out)
			break;

<<<<<<< HEAD
		ret2 = btrfs_decompress_buf2page(workspace->buf, buf_start,
						 total_out, disk_start,
						 bvec, vcnt,
						 &page_out_index, &pg_offset);
=======
		ret2 = btrfs_decompress_buf2page(workspace->buf,
				total_out - buf_start, cb, buf_start);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (ret2 == 0) {
			ret = 0;
			goto done;
		}

<<<<<<< HEAD
		workspace->inf_strm.next_out = workspace->buf;
		workspace->inf_strm.avail_out = PAGE_CACHE_SIZE;

		if (workspace->inf_strm.avail_in == 0) {
			unsigned long tmp;
			kunmap(pages_in[page_in_index]);
=======
		workspace->strm.next_out = workspace->buf;
		workspace->strm.avail_out = workspace->buf_size;

		if (workspace->strm.avail_in == 0) {
			unsigned long tmp;
			kunmap_local(data_in);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			page_in_index++;
			if (page_in_index >= total_pages_in) {
				data_in = NULL;
				break;
			}
<<<<<<< HEAD
			data_in = kmap(pages_in[page_in_index]);
			workspace->inf_strm.next_in = data_in;
			tmp = srclen - workspace->inf_strm.total_in;
			workspace->inf_strm.avail_in = min(tmp,
							   PAGE_CACHE_SIZE);
		}
	}
	if (ret != Z_STREAM_END)
		ret = -1;
	else
		ret = 0;
done:
	zlib_inflateEnd(&workspace->inf_strm);
	if (data_in)
		kunmap(pages_in[page_in_index]);
	return ret;
}

static int zlib_decompress(struct list_head *ws, unsigned char *data_in,
			   struct page *dest_page,
			   unsigned long start_byte,
			   size_t srclen, size_t destlen)
=======
			data_in = kmap_local_page(pages_in[page_in_index]);
			workspace->strm.next_in = data_in;
			tmp = srclen - workspace->strm.total_in;
			workspace->strm.avail_in = min(tmp, PAGE_SIZE);
		}
	}
	if (ret != Z_STREAM_END)
		ret = -EIO;
	else
		ret = 0;
done:
	zlib_inflateEnd(&workspace->strm);
	if (data_in)
		kunmap_local(data_in);
	return ret;
}

int zlib_decompress(struct list_head *ws, const u8 *data_in,
		struct page *dest_page, unsigned long dest_pgoff, size_t srclen,
		size_t destlen)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct workspace *workspace = list_entry(ws, struct workspace, list);
	int ret = 0;
	int wbits = MAX_WBITS;
<<<<<<< HEAD
	unsigned long bytes_left = destlen;
	unsigned long total_out = 0;
	char *kaddr;

	workspace->inf_strm.next_in = data_in;
	workspace->inf_strm.avail_in = srclen;
	workspace->inf_strm.total_in = 0;

	workspace->inf_strm.next_out = workspace->buf;
	workspace->inf_strm.avail_out = PAGE_CACHE_SIZE;
	workspace->inf_strm.total_out = 0;
=======
	unsigned long to_copy;

	workspace->strm.next_in = data_in;
	workspace->strm.avail_in = srclen;
	workspace->strm.total_in = 0;

	workspace->strm.next_out = workspace->buf;
	workspace->strm.avail_out = workspace->buf_size;
	workspace->strm.total_out = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* If it's deflate, and it's got no preset dictionary, then
	   we can tell zlib to skip the adler32 check. */
	if (srclen > 2 && !(data_in[1] & PRESET_DICT) &&
	    ((data_in[0] & 0x0f) == Z_DEFLATED) &&
	    !(((data_in[0]<<8) + data_in[1]) % 31)) {

		wbits = -((data_in[0] >> 4) + 8);
<<<<<<< HEAD
		workspace->inf_strm.next_in += 2;
		workspace->inf_strm.avail_in -= 2;
	}

	if (Z_OK != zlib_inflateInit2(&workspace->inf_strm, wbits)) {
		printk(KERN_WARNING "inflateInit failed\n");
		return -1;
	}

	while (bytes_left > 0) {
		unsigned long buf_start;
		unsigned long buf_offset;
		unsigned long bytes;
		unsigned long pg_offset = 0;

		ret = zlib_inflate(&workspace->inf_strm, Z_NO_FLUSH);
		if (ret != Z_OK && ret != Z_STREAM_END)
			break;

		buf_start = total_out;
		total_out = workspace->inf_strm.total_out;

		if (total_out == buf_start) {
			ret = -1;
			break;
		}

		if (total_out <= start_byte)
			goto next;

		if (total_out > start_byte && buf_start < start_byte)
			buf_offset = start_byte - buf_start;
		else
			buf_offset = 0;

		bytes = min(PAGE_CACHE_SIZE - pg_offset,
			    PAGE_CACHE_SIZE - buf_offset);
		bytes = min(bytes, bytes_left);

		kaddr = kmap_atomic(dest_page);
		memcpy(kaddr + pg_offset, workspace->buf + buf_offset, bytes);
		kunmap_atomic(kaddr);

		pg_offset += bytes;
		bytes_left -= bytes;
next:
		workspace->inf_strm.next_out = workspace->buf;
		workspace->inf_strm.avail_out = PAGE_CACHE_SIZE;
	}

	if (ret != Z_STREAM_END && bytes_left != 0)
		ret = -1;
	else
		ret = 0;

	zlib_inflateEnd(&workspace->inf_strm);
	return ret;
}

struct btrfs_compress_op btrfs_zlib_compress = {
	.alloc_workspace	= zlib_alloc_workspace,
	.free_workspace		= zlib_free_workspace,
	.compress_pages		= zlib_compress_pages,
	.decompress_biovec	= zlib_decompress_biovec,
	.decompress		= zlib_decompress,
=======
		workspace->strm.next_in += 2;
		workspace->strm.avail_in -= 2;
	}

	if (Z_OK != zlib_inflateInit2(&workspace->strm, wbits)) {
		pr_warn("BTRFS: inflateInit failed\n");
		return -EIO;
	}

	/*
	 * Everything (in/out buf) should be at most one sector, there should
	 * be no need to switch any input/output buffer.
	 */
	ret = zlib_inflate(&workspace->strm, Z_FINISH);
	to_copy = min(workspace->strm.total_out, destlen);
	if (ret != Z_STREAM_END)
		goto out;

	memcpy_to_page(dest_page, dest_pgoff, workspace->buf, to_copy);

out:
	if (unlikely(to_copy != destlen)) {
		pr_warn_ratelimited("BTRFS: inflate failed, decompressed=%lu expected=%zu\n",
					to_copy, destlen);
		ret = -EIO;
	} else {
		ret = 0;
	}

	zlib_inflateEnd(&workspace->strm);

	if (unlikely(to_copy < destlen))
		memzero_page(dest_page, dest_pgoff + to_copy, destlen - to_copy);
	return ret;
}

const struct btrfs_compress_op btrfs_zlib_compress = {
	.workspace_manager	= &wsm,
	.max_level		= 9,
	.default_level		= BTRFS_ZLIB_DEFAULT_LEVEL,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};
