<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * "splice": joining two ropes together by interweaving their strands.
 *
 * This is the "extended pipe" functionality, where a pipe is used as
 * an arbitrary in-memory buffer. Think of a pipe as a small kernel
 * buffer that you can use to transfer data from one end to the other.
 *
 * The traditional unix read/write is extended with a "splice()" operation
 * that transfers data buffers to or from a pipe buffer.
 *
 * Named by Larry McVoy, original implementation from Linus, extended by
 * Jens to support splicing to files, network, direct splicing, etc and
 * fixing lots of bugs.
 *
 * Copyright (C) 2005-2006 Jens Axboe <axboe@kernel.dk>
 * Copyright (C) 2005-2006 Linus Torvalds <torvalds@osdl.org>
 * Copyright (C) 2006 Ingo Molnar <mingo@elte.hu>
 *
 */
<<<<<<< HEAD
=======
#include <linux/bvec.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/pagemap.h>
#include <linux/splice.h>
#include <linux/memcontrol.h>
#include <linux/mm_inline.h>
#include <linux/swap.h>
#include <linux/writeback.h>
#include <linux/export.h>
#include <linux/syscalls.h>
#include <linux/uio.h>
<<<<<<< HEAD
#include <linux/security.h>
#include <linux/gfp.h>
#include <linux/socket.h>
=======
#include <linux/fsnotify.h>
#include <linux/security.h>
#include <linux/gfp.h>
#include <linux/net.h>
#include <linux/socket.h>
#include <linux/sched/signal.h>

#include "internal.h"

/*
 * Splice doesn't support FMODE_NOWAIT. Since pipes may set this flag to
 * indicate they support non-blocking reads or writes, we must clear it
 * here if set to avoid blocking other users of this pipe if splice is
 * being done on it.
 */
static noinline void noinline pipe_clear_nowait(struct file *file)
{
	fmode_t fmode = READ_ONCE(file->f_mode);

	do {
		if (!(fmode & FMODE_NOWAIT))
			break;
	} while (!try_cmpxchg(&file->f_mode, &fmode, fmode & ~FMODE_NOWAIT));
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Attempt to steal a page from a pipe buffer. This should perhaps go into
 * a vm helper function, it's already simplified quite a bit by the
 * addition of remove_mapping(). If success is returned, the caller may
 * attempt to reuse this page for another destination.
 */
<<<<<<< HEAD
static int page_cache_pipe_buf_steal(struct pipe_inode_info *pipe,
				     struct pipe_buffer *buf)
{
	struct page *page = buf->page;
	struct address_space *mapping;

	lock_page(page);

	mapping = page_mapping(page);
	if (mapping) {
		WARN_ON(!PageUptodate(page));

		/*
		 * At least for ext2 with nobh option, we need to wait on
		 * writeback completing on this page, since we'll remove it
		 * from the pagecache.  Otherwise truncate wont wait on the
		 * page, allowing the disk blocks to be reused by someone else
		 * before we actually wrote our data to them. fs corruption
		 * ensues.
		 */
		wait_on_page_writeback(page);

		if (page_has_private(page) &&
		    !try_to_release_page(page, GFP_KERNEL))
=======
static bool page_cache_pipe_buf_try_steal(struct pipe_inode_info *pipe,
		struct pipe_buffer *buf)
{
	struct folio *folio = page_folio(buf->page);
	struct address_space *mapping;

	folio_lock(folio);

	mapping = folio_mapping(folio);
	if (mapping) {
		WARN_ON(!folio_test_uptodate(folio));

		/*
		 * At least for ext2 with nobh option, we need to wait on
		 * writeback completing on this folio, since we'll remove it
		 * from the pagecache.  Otherwise truncate wont wait on the
		 * folio, allowing the disk blocks to be reused by someone else
		 * before we actually wrote our data to them. fs corruption
		 * ensues.
		 */
		folio_wait_writeback(folio);

		if (!filemap_release_folio(folio, GFP_KERNEL))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto out_unlock;

		/*
		 * If we succeeded in removing the mapping, set LRU flag
		 * and return good.
		 */
<<<<<<< HEAD
		if (remove_mapping(mapping, page)) {
			buf->flags |= PIPE_BUF_FLAG_LRU;
			return 0;
=======
		if (remove_mapping(mapping, folio)) {
			buf->flags |= PIPE_BUF_FLAG_LRU;
			return true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}

	/*
<<<<<<< HEAD
	 * Raced with truncate or failed to remove page from current
	 * address space, unlock and return failure.
	 */
out_unlock:
	unlock_page(page);
	return 1;
=======
	 * Raced with truncate or failed to remove folio from current
	 * address space, unlock and return failure.
	 */
out_unlock:
	folio_unlock(folio);
	return false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void page_cache_pipe_buf_release(struct pipe_inode_info *pipe,
					struct pipe_buffer *buf)
{
<<<<<<< HEAD
	page_cache_release(buf->page);
=======
	put_page(buf->page);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	buf->flags &= ~PIPE_BUF_FLAG_LRU;
}

/*
 * Check whether the contents of buf is OK to access. Since the content
 * is a page cache page, IO may be in flight.
 */
static int page_cache_pipe_buf_confirm(struct pipe_inode_info *pipe,
				       struct pipe_buffer *buf)
{
<<<<<<< HEAD
	struct page *page = buf->page;
	int err;

	if (!PageUptodate(page)) {
		lock_page(page);

		/*
		 * Page got truncated/unhashed. This will cause a 0-byte
		 * splice, if this is the first page.
		 */
		if (!page->mapping) {
=======
	struct folio *folio = page_folio(buf->page);
	int err;

	if (!folio_test_uptodate(folio)) {
		folio_lock(folio);

		/*
		 * Folio got truncated/unhashed. This will cause a 0-byte
		 * splice, if this is the first page.
		 */
		if (!folio->mapping) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			err = -ENODATA;
			goto error;
		}

		/*
		 * Uh oh, read-error from disk.
		 */
<<<<<<< HEAD
		if (!PageUptodate(page)) {
=======
		if (!folio_test_uptodate(folio)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			err = -EIO;
			goto error;
		}

<<<<<<< HEAD
		/*
		 * Page is ok afterall, we are done.
		 */
		unlock_page(page);
=======
		/* Folio is ok after all, we are done */
		folio_unlock(folio);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return 0;
error:
<<<<<<< HEAD
	unlock_page(page);
=======
	folio_unlock(folio);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

const struct pipe_buf_operations page_cache_pipe_buf_ops = {
<<<<<<< HEAD
	.can_merge = 0,
	.map = generic_pipe_buf_map,
	.unmap = generic_pipe_buf_unmap,
	.confirm = page_cache_pipe_buf_confirm,
	.release = page_cache_pipe_buf_release,
	.steal = page_cache_pipe_buf_steal,
	.get = generic_pipe_buf_get,
};

static int user_page_pipe_buf_steal(struct pipe_inode_info *pipe,
				    struct pipe_buffer *buf)
{
	if (!(buf->flags & PIPE_BUF_FLAG_GIFT))
		return 1;

	buf->flags |= PIPE_BUF_FLAG_LRU;
	return generic_pipe_buf_steal(pipe, buf);
}

static const struct pipe_buf_operations user_page_pipe_buf_ops = {
	.can_merge = 0,
	.map = generic_pipe_buf_map,
	.unmap = generic_pipe_buf_unmap,
	.confirm = generic_pipe_buf_confirm,
	.release = page_cache_pipe_buf_release,
	.steal = user_page_pipe_buf_steal,
	.get = generic_pipe_buf_get,
=======
	.confirm	= page_cache_pipe_buf_confirm,
	.release	= page_cache_pipe_buf_release,
	.try_steal	= page_cache_pipe_buf_try_steal,
	.get		= generic_pipe_buf_get,
};

static bool user_page_pipe_buf_try_steal(struct pipe_inode_info *pipe,
		struct pipe_buffer *buf)
{
	if (!(buf->flags & PIPE_BUF_FLAG_GIFT))
		return false;

	buf->flags |= PIPE_BUF_FLAG_LRU;
	return generic_pipe_buf_try_steal(pipe, buf);
}

static const struct pipe_buf_operations user_page_pipe_buf_ops = {
	.release	= page_cache_pipe_buf_release,
	.try_steal	= user_page_pipe_buf_try_steal,
	.get		= generic_pipe_buf_get,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static void wakeup_pipe_readers(struct pipe_inode_info *pipe)
{
	smp_mb();
<<<<<<< HEAD
	if (waitqueue_active(&pipe->wait))
		wake_up_interruptible(&pipe->wait);
=======
	if (waitqueue_active(&pipe->rd_wait))
		wake_up_interruptible(&pipe->rd_wait);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kill_fasync(&pipe->fasync_readers, SIGIO, POLL_IN);
}

/**
 * splice_to_pipe - fill passed data into a pipe
 * @pipe:	pipe to fill
 * @spd:	data to fill
 *
 * Description:
 *    @spd contains a map of pages and len/offset tuples, along with
 *    the struct pipe_buf_operations associated with these pages. This
 *    function will link that data to the pipe.
 *
 */
ssize_t splice_to_pipe(struct pipe_inode_info *pipe,
		       struct splice_pipe_desc *spd)
{
	unsigned int spd_pages = spd->nr_pages;
<<<<<<< HEAD
	int ret, do_wakeup, page_nr;

	ret = 0;
	do_wakeup = 0;
	page_nr = 0;

	pipe_lock(pipe);

	for (;;) {
		if (!pipe->readers) {
			send_sig(SIGPIPE, current, 0);
			if (!ret)
				ret = -EPIPE;
			break;
		}

		if (pipe->nrbufs < pipe->buffers) {
			int newbuf = (pipe->curbuf + pipe->nrbufs) & (pipe->buffers - 1);
			struct pipe_buffer *buf = pipe->bufs + newbuf;

			buf->page = spd->pages[page_nr];
			buf->offset = spd->partial[page_nr].offset;
			buf->len = spd->partial[page_nr].len;
			buf->private = spd->partial[page_nr].private;
			buf->ops = spd->ops;
			if (spd->flags & SPLICE_F_GIFT)
				buf->flags |= PIPE_BUF_FLAG_GIFT;

			pipe->nrbufs++;
			page_nr++;
			ret += buf->len;

			if (pipe->inode)
				do_wakeup = 1;

			if (!--spd->nr_pages)
				break;
			if (pipe->nrbufs < pipe->buffers)
				continue;

			break;
		}

		if (spd->flags & SPLICE_F_NONBLOCK) {
			if (!ret)
				ret = -EAGAIN;
			break;
		}

		if (signal_pending(current)) {
			if (!ret)
				ret = -ERESTARTSYS;
			break;
		}

		if (do_wakeup) {
			smp_mb();
			if (waitqueue_active(&pipe->wait))
				wake_up_interruptible_sync(&pipe->wait);
			kill_fasync(&pipe->fasync_readers, SIGIO, POLL_IN);
			do_wakeup = 0;
		}

		pipe->waiting_writers++;
		pipe_wait(pipe);
		pipe->waiting_writers--;
	}

	pipe_unlock(pipe);

	if (do_wakeup)
		wakeup_pipe_readers(pipe);

=======
	unsigned int tail = pipe->tail;
	unsigned int head = pipe->head;
	unsigned int mask = pipe->ring_size - 1;
	ssize_t ret = 0;
	int page_nr = 0;

	if (!spd_pages)
		return 0;

	if (unlikely(!pipe->readers)) {
		send_sig(SIGPIPE, current, 0);
		ret = -EPIPE;
		goto out;
	}

	while (!pipe_full(head, tail, pipe->max_usage)) {
		struct pipe_buffer *buf = &pipe->bufs[head & mask];

		buf->page = spd->pages[page_nr];
		buf->offset = spd->partial[page_nr].offset;
		buf->len = spd->partial[page_nr].len;
		buf->private = spd->partial[page_nr].private;
		buf->ops = spd->ops;
		buf->flags = 0;

		head++;
		pipe->head = head;
		page_nr++;
		ret += buf->len;

		if (!--spd->nr_pages)
			break;
	}

	if (!ret)
		ret = -EAGAIN;

out:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	while (page_nr < spd_pages)
		spd->spd_release(spd, page_nr++);

	return ret;
}
<<<<<<< HEAD

void spd_release_page(struct splice_pipe_desc *spd, unsigned int i)
{
	page_cache_release(spd->pages[i]);
}
=======
EXPORT_SYMBOL_GPL(splice_to_pipe);

ssize_t add_to_pipe(struct pipe_inode_info *pipe, struct pipe_buffer *buf)
{
	unsigned int head = pipe->head;
	unsigned int tail = pipe->tail;
	unsigned int mask = pipe->ring_size - 1;
	int ret;

	if (unlikely(!pipe->readers)) {
		send_sig(SIGPIPE, current, 0);
		ret = -EPIPE;
	} else if (pipe_full(head, tail, pipe->max_usage)) {
		ret = -EAGAIN;
	} else {
		pipe->bufs[head & mask] = *buf;
		pipe->head = head + 1;
		return buf->len;
	}
	pipe_buf_release(pipe, buf);
	return ret;
}
EXPORT_SYMBOL(add_to_pipe);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Check if we need to grow the arrays holding pages and partial page
 * descriptions.
 */
int splice_grow_spd(const struct pipe_inode_info *pipe, struct splice_pipe_desc *spd)
{
<<<<<<< HEAD
	unsigned int buffers = ACCESS_ONCE(pipe->buffers);

	spd->nr_pages_max = buffers;
	if (buffers <= PIPE_DEF_BUFFERS)
		return 0;

	spd->pages = kmalloc(buffers * sizeof(struct page *), GFP_KERNEL);
	spd->partial = kmalloc(buffers * sizeof(struct partial_page), GFP_KERNEL);
=======
	unsigned int max_usage = READ_ONCE(pipe->max_usage);

	spd->nr_pages_max = max_usage;
	if (max_usage <= PIPE_DEF_BUFFERS)
		return 0;

	spd->pages = kmalloc_array(max_usage, sizeof(struct page *), GFP_KERNEL);
	spd->partial = kmalloc_array(max_usage, sizeof(struct partial_page),
				     GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (spd->pages && spd->partial)
		return 0;

	kfree(spd->pages);
	kfree(spd->partial);
	return -ENOMEM;
}

void splice_shrink_spd(struct splice_pipe_desc *spd)
{
	if (spd->nr_pages_max <= PIPE_DEF_BUFFERS)
		return;

	kfree(spd->pages);
	kfree(spd->partial);
}

<<<<<<< HEAD
static int
__generic_file_splice_read(struct file *in, loff_t *ppos,
			   struct pipe_inode_info *pipe, size_t len,
			   unsigned int flags)
{
	struct address_space *mapping = in->f_mapping;
	unsigned int loff, nr_pages, req_pages;
	struct page *pages[PIPE_DEF_BUFFERS];
	struct partial_page partial[PIPE_DEF_BUFFERS];
	struct page *page;
	pgoff_t index, end_index;
	loff_t isize;
	int error, page_nr;
	struct splice_pipe_desc spd = {
		.pages = pages,
		.partial = partial,
		.nr_pages_max = PIPE_DEF_BUFFERS,
		.flags = flags,
		.ops = &page_cache_pipe_buf_ops,
		.spd_release = spd_release_page,
	};

	if (splice_grow_spd(pipe, &spd))
		return -ENOMEM;

	index = *ppos >> PAGE_CACHE_SHIFT;
	loff = *ppos & ~PAGE_CACHE_MASK;
	req_pages = (len + loff + PAGE_CACHE_SIZE - 1) >> PAGE_CACHE_SHIFT;
	nr_pages = min(req_pages, spd.nr_pages_max);

	/*
	 * Lookup the (hopefully) full range of pages we need.
	 */
	spd.nr_pages = find_get_pages_contig(mapping, index, nr_pages, spd.pages);
	index += spd.nr_pages;

	/*
	 * If find_get_pages_contig() returned fewer pages than we needed,
	 * readahead/allocate the rest and fill in the holes.
	 */
	if (spd.nr_pages < nr_pages)
		page_cache_sync_readahead(mapping, &in->f_ra, in,
				index, req_pages - spd.nr_pages);

	error = 0;
	while (spd.nr_pages < nr_pages) {
		/*
		 * Page could be there, find_get_pages_contig() breaks on
		 * the first hole.
		 */
		page = find_get_page(mapping, index);
		if (!page) {
			/*
			 * page didn't exist, allocate one.
			 */
			page = page_cache_alloc_cold(mapping);
			if (!page)
				break;

			error = add_to_page_cache_lru(page, mapping, index,
						GFP_KERNEL);
			if (unlikely(error)) {
				page_cache_release(page);
				if (error == -EEXIST)
					continue;
				break;
			}
			/*
			 * add_to_page_cache() locks the page, unlock it
			 * to avoid convoluting the logic below even more.
			 */
			unlock_page(page);
		}

		spd.pages[spd.nr_pages++] = page;
		index++;
	}

	if (unlikely(!(in->f_mode & FMODE_SPLICE_READ)))
		return -EINVAL;

	/*
	 * Now loop over the map and see if we need to start IO on any
	 * pages, fill in the partial map, etc.
	 */
	index = *ppos >> PAGE_CACHE_SHIFT;
	nr_pages = spd.nr_pages;
	spd.nr_pages = 0;
	for (page_nr = 0; page_nr < nr_pages; page_nr++) {
		unsigned int this_len;

		if (!len)
			break;

		/*
		 * this_len is the max we'll use from this page
		 */
		this_len = min_t(unsigned long, len, PAGE_CACHE_SIZE - loff);
		page = spd.pages[page_nr];

		if (PageReadahead(page))
			page_cache_async_readahead(mapping, &in->f_ra, in,
					page, index, req_pages - page_nr);

		/*
		 * If the page isn't uptodate, we may need to start io on it
		 */
		if (!PageUptodate(page)) {
			lock_page(page);

			/*
			 * Page was truncated, or invalidated by the
			 * filesystem.  Redo the find/create, but this time the
			 * page is kept locked, so there's no chance of another
			 * race with truncate/invalidate.
			 */
			if (!page->mapping) {
				unlock_page(page);
				page = find_or_create_page(mapping, index,
						mapping_gfp_mask(mapping));

				if (!page) {
					error = -ENOMEM;
					break;
				}
				page_cache_release(spd.pages[page_nr]);
				spd.pages[page_nr] = page;
			}
			/*
			 * page was already under io and is now done, great
			 */
			if (PageUptodate(page)) {
				unlock_page(page);
				goto fill_it;
			}

			/*
			 * need to read in the page
			 */
			error = mapping->a_ops->readpage(in, page);
			if (unlikely(error)) {
				/*
				 * We really should re-lookup the page here,
				 * but it complicates things a lot. Instead
				 * lets just do what we already stored, and
				 * we'll get it the next time we are called.
				 */
				if (error == AOP_TRUNCATED_PAGE)
					error = 0;

				break;
			}
		}
fill_it:
		/*
		 * i_size must be checked after PageUptodate.
		 */
		isize = i_size_read(mapping->host);
		end_index = (isize - 1) >> PAGE_CACHE_SHIFT;
		if (unlikely(!isize || index > end_index))
			break;

		/*
		 * if this is the last page, see if we need to shrink
		 * the length and stop
		 */
		if (end_index == index) {
			unsigned int plen;

			/*
			 * max good bytes in this page
			 */
			plen = ((isize - 1) & ~PAGE_CACHE_MASK) + 1;
			if (plen <= loff)
				break;

			/*
			 * force quit after adding this page
			 */
			this_len = min(this_len, plen - loff);
			len = this_len;
		}

		spd.partial[page_nr].offset = loff;
		spd.partial[page_nr].len = this_len;
		len -= this_len;
		loff = 0;
		spd.nr_pages++;
		index++;
	}

	/*
	 * Release any pages at the end, if we quit early. 'page_nr' is how far
	 * we got, 'nr_pages' is how many pages are in the map.
	 */
	while (page_nr < nr_pages)
		page_cache_release(spd.pages[page_nr++]);
	in->f_ra.prev_pos = (loff_t)index << PAGE_CACHE_SHIFT;

	if (spd.nr_pages)
		error = splice_to_pipe(pipe, &spd);

	splice_shrink_spd(&spd);
	return error;
}

/**
 * generic_file_splice_read - splice data from file to a pipe
 * @in:		file to splice from
 * @ppos:	position in @in
 * @pipe:	pipe to splice to
 * @len:	number of bytes to splice
 * @flags:	splice modifier flags
 *
 * Description:
 *    Will read pages from given file and fill them into a pipe. Can be
 *    used as long as the address_space operations for the source implements
 *    a readpage() hook.
 *
 */
ssize_t generic_file_splice_read(struct file *in, loff_t *ppos,
				 struct pipe_inode_info *pipe, size_t len,
				 unsigned int flags)
{
	loff_t isize, left;
	int ret;

	isize = i_size_read(in->f_mapping->host);
	if (unlikely(*ppos >= isize))
		return 0;

	left = isize - *ppos;
	if (unlikely(left < len))
		len = left;

	ret = __generic_file_splice_read(in, ppos, pipe, len, flags);
	if (ret > 0) {
		*ppos += ret;
		file_accessed(in);
	}

	return ret;
}
EXPORT_SYMBOL(generic_file_splice_read);

static const struct pipe_buf_operations default_pipe_buf_ops = {
	.can_merge = 0,
	.map = generic_pipe_buf_map,
	.unmap = generic_pipe_buf_unmap,
	.confirm = generic_pipe_buf_confirm,
	.release = generic_pipe_buf_release,
	.steal = generic_pipe_buf_steal,
	.get = generic_pipe_buf_get,
};

static ssize_t kernel_readv(struct file *file, const struct iovec *vec,
			    unsigned long vlen, loff_t offset)
{
	mm_segment_t old_fs;
	loff_t pos = offset;
	ssize_t res;

	old_fs = get_fs();
	set_fs(get_ds());
	/* The cast to a user pointer is valid due to the set_fs() */
	res = vfs_readv(file, (const struct iovec __user *)vec, vlen, &pos);
	set_fs(old_fs);

	return res;
}

static ssize_t kernel_write(struct file *file, const char *buf, size_t count,
			    loff_t pos)
{
	mm_segment_t old_fs;
	ssize_t res;

	old_fs = get_fs();
	set_fs(get_ds());
	/* The cast to a user pointer is valid due to the set_fs() */
	res = vfs_write(file, (const char __user *)buf, count, &pos);
	set_fs(old_fs);

	return res;
}

ssize_t default_file_splice_read(struct file *in, loff_t *ppos,
				 struct pipe_inode_info *pipe, size_t len,
				 unsigned int flags)
{
	unsigned int nr_pages;
	unsigned int nr_freed;
	size_t offset;
	struct page *pages[PIPE_DEF_BUFFERS];
	struct partial_page partial[PIPE_DEF_BUFFERS];
	struct iovec *vec, __vec[PIPE_DEF_BUFFERS];
	ssize_t res;
	size_t this_len;
	int error;
	int i;
	struct splice_pipe_desc spd = {
		.pages = pages,
		.partial = partial,
		.nr_pages_max = PIPE_DEF_BUFFERS,
		.flags = flags,
		.ops = &default_pipe_buf_ops,
		.spd_release = spd_release_page,
	};

	if (splice_grow_spd(pipe, &spd))
		return -ENOMEM;

	res = -ENOMEM;
	vec = __vec;
	if (spd.nr_pages_max > PIPE_DEF_BUFFERS) {
		vec = kmalloc(spd.nr_pages_max * sizeof(struct iovec), GFP_KERNEL);
		if (!vec)
			goto shrink_ret;
	}

	offset = *ppos & ~PAGE_CACHE_MASK;
	nr_pages = (len + offset + PAGE_CACHE_SIZE - 1) >> PAGE_CACHE_SHIFT;

	for (i = 0; i < nr_pages && i < spd.nr_pages_max && len; i++) {
		struct page *page;

		page = alloc_page(GFP_USER);
		error = -ENOMEM;
		if (!page)
			goto err;

		this_len = min_t(size_t, len, PAGE_CACHE_SIZE - offset);
		vec[i].iov_base = (void __user *) page_address(page);
		vec[i].iov_len = this_len;
		spd.pages[i] = page;
		spd.nr_pages++;
		len -= this_len;
		offset = 0;
	}

	res = kernel_readv(in, vec, spd.nr_pages, *ppos);
	if (res < 0) {
		error = res;
		goto err;
	}

	error = 0;
	if (!res)
		goto err;

	nr_freed = 0;
	for (i = 0; i < spd.nr_pages; i++) {
		this_len = min_t(size_t, vec[i].iov_len, res);
		spd.partial[i].offset = 0;
		spd.partial[i].len = this_len;
		if (!this_len) {
			__free_page(spd.pages[i]);
			spd.pages[i] = NULL;
			nr_freed++;
		}
		res -= this_len;
	}
	spd.nr_pages -= nr_freed;

	res = splice_to_pipe(pipe, &spd);
	if (res > 0)
		*ppos += res;

shrink_ret:
	if (vec != __vec)
		kfree(vec);
	splice_shrink_spd(&spd);
	return res;

err:
	for (i = 0; i < spd.nr_pages; i++)
		__free_page(spd.pages[i]);

	res = error;
	goto shrink_ret;
}
EXPORT_SYMBOL(default_file_splice_read);

/*
 * Send 'sd->len' bytes to socket from 'sd->file' at position 'sd->pos'
 * using sendpage(). Return the number of bytes sent.
 */
static int pipe_to_sendpage(struct pipe_inode_info *pipe,
			    struct pipe_buffer *buf, struct splice_desc *sd)
{
	struct file *file = sd->u.file;
	loff_t pos = sd->pos;
	int more;

	if (!likely(file->f_op && file->f_op->sendpage))
		return -EINVAL;

	more = (sd->flags & SPLICE_F_MORE) ? MSG_MORE : 0;

	if (sd->len < sd->total_len && pipe->nrbufs > 1)
		more |= MSG_SENDPAGE_NOTLAST;

	return file->f_op->sendpage(file, buf->page, buf->offset,
				    sd->len, &pos, more);
}

/*
 * This is a little more tricky than the file -> pipe splicing. There are
 * basically three cases:
 *
 *	- Destination page already exists in the address space and there
 *	  are users of it. For that case we have no other option that
 *	  copying the data. Tough luck.
 *	- Destination page already exists in the address space, but there
 *	  are no users of it. Make sure it's uptodate, then drop it. Fall
 *	  through to last case.
 *	- Destination page does not exist, we can add the pipe page to
 *	  the page cache and avoid the copy.
 *
 * If asked to move pages to the output file (SPLICE_F_MOVE is set in
 * sd->flags), we attempt to migrate pages from the pipe to the output
 * file address space page cache. This is possible if no one else has
 * the pipe page referenced outside of the pipe and page cache. If
 * SPLICE_F_MOVE isn't set, or we cannot move the page, we simply create
 * a new page in the output file page cache and fill/dirty that.
 */
int pipe_to_file(struct pipe_inode_info *pipe, struct pipe_buffer *buf,
		 struct splice_desc *sd)
{
	struct file *file = sd->u.file;
	struct address_space *mapping = file->f_mapping;
	unsigned int offset, this_len;
	struct page *page;
	void *fsdata;
	int ret;

	offset = sd->pos & ~PAGE_CACHE_MASK;

	this_len = sd->len;
	if (this_len + offset > PAGE_CACHE_SIZE)
		this_len = PAGE_CACHE_SIZE - offset;

	ret = pagecache_write_begin(file, mapping, sd->pos, this_len,
				AOP_FLAG_UNINTERRUPTIBLE, &page, &fsdata);
	if (unlikely(ret))
		goto out;

	if (buf->page != page) {
		char *src = buf->ops->map(pipe, buf, 1);
		char *dst = kmap_atomic(page);

		memcpy(dst + offset, src + buf->offset, this_len);
		flush_dcache_page(page);
		kunmap_atomic(dst);
		buf->ops->unmap(pipe, buf, src);
	}
	ret = pagecache_write_end(file, mapping, sd->pos, this_len, this_len,
				page, fsdata);
out:
	return ret;
}
EXPORT_SYMBOL(pipe_to_file);
=======
/**
 * copy_splice_read -  Copy data from a file and splice the copy into a pipe
 * @in: The file to read from
 * @ppos: Pointer to the file position to read from
 * @pipe: The pipe to splice into
 * @len: The amount to splice
 * @flags: The SPLICE_F_* flags
 *
 * This function allocates a bunch of pages sufficient to hold the requested
 * amount of data (but limited by the remaining pipe capacity), passes it to
 * the file's ->read_iter() to read into and then splices the used pages into
 * the pipe.
 *
 * Return: On success, the number of bytes read will be returned and *@ppos
 * will be updated if appropriate; 0 will be returned if there is no more data
 * to be read; -EAGAIN will be returned if the pipe had no space, and some
 * other negative error code will be returned on error.  A short read may occur
 * if the pipe has insufficient space, we reach the end of the data or we hit a
 * hole.
 */
ssize_t copy_splice_read(struct file *in, loff_t *ppos,
			 struct pipe_inode_info *pipe,
			 size_t len, unsigned int flags)
{
	struct iov_iter to;
	struct bio_vec *bv;
	struct kiocb kiocb;
	struct page **pages;
	ssize_t ret;
	size_t used, npages, chunk, remain, keep = 0;
	int i;

	/* Work out how much data we can actually add into the pipe */
	used = pipe_occupancy(pipe->head, pipe->tail);
	npages = max_t(ssize_t, pipe->max_usage - used, 0);
	len = min_t(size_t, len, npages * PAGE_SIZE);
	npages = DIV_ROUND_UP(len, PAGE_SIZE);

	bv = kzalloc(array_size(npages, sizeof(bv[0])) +
		     array_size(npages, sizeof(struct page *)), GFP_KERNEL);
	if (!bv)
		return -ENOMEM;

	pages = (struct page **)(bv + npages);
	npages = alloc_pages_bulk_array(GFP_USER, npages, pages);
	if (!npages) {
		kfree(bv);
		return -ENOMEM;
	}

	remain = len = min_t(size_t, len, npages * PAGE_SIZE);

	for (i = 0; i < npages; i++) {
		chunk = min_t(size_t, PAGE_SIZE, remain);
		bv[i].bv_page = pages[i];
		bv[i].bv_offset = 0;
		bv[i].bv_len = chunk;
		remain -= chunk;
	}

	/* Do the I/O */
	iov_iter_bvec(&to, ITER_DEST, bv, npages, len);
	init_sync_kiocb(&kiocb, in);
	kiocb.ki_pos = *ppos;
	ret = call_read_iter(in, &kiocb, &to);

	if (ret > 0) {
		keep = DIV_ROUND_UP(ret, PAGE_SIZE);
		*ppos = kiocb.ki_pos;
	}

	/*
	 * Callers of ->splice_read() expect -EAGAIN on "can't put anything in
	 * there", rather than -EFAULT.
	 */
	if (ret == -EFAULT)
		ret = -EAGAIN;

	/* Free any pages that didn't get touched at all. */
	if (keep < npages)
		release_pages(pages + keep, npages - keep);

	/* Push the remaining pages into the pipe. */
	remain = ret;
	for (i = 0; i < keep; i++) {
		struct pipe_buffer *buf = pipe_head_buf(pipe);

		chunk = min_t(size_t, remain, PAGE_SIZE);
		*buf = (struct pipe_buffer) {
			.ops	= &default_pipe_buf_ops,
			.page	= bv[i].bv_page,
			.offset	= 0,
			.len	= chunk,
		};
		pipe->head++;
		remain -= chunk;
	}

	kfree(bv);
	return ret;
}
EXPORT_SYMBOL(copy_splice_read);

const struct pipe_buf_operations default_pipe_buf_ops = {
	.release	= generic_pipe_buf_release,
	.try_steal	= generic_pipe_buf_try_steal,
	.get		= generic_pipe_buf_get,
};

/* Pipe buffer operations for a socket and similar. */
const struct pipe_buf_operations nosteal_pipe_buf_ops = {
	.release	= generic_pipe_buf_release,
	.get		= generic_pipe_buf_get,
};
EXPORT_SYMBOL(nosteal_pipe_buf_ops);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static void wakeup_pipe_writers(struct pipe_inode_info *pipe)
{
	smp_mb();
<<<<<<< HEAD
	if (waitqueue_active(&pipe->wait))
		wake_up_interruptible(&pipe->wait);
=======
	if (waitqueue_active(&pipe->wr_wait))
		wake_up_interruptible(&pipe->wr_wait);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kill_fasync(&pipe->fasync_writers, SIGIO, POLL_OUT);
}

/**
 * splice_from_pipe_feed - feed available data from a pipe to a file
 * @pipe:	pipe to splice from
 * @sd:		information to @actor
 * @actor:	handler that splices the data
 *
 * Description:
 *    This function loops over the pipe and calls @actor to do the
 *    actual moving of a single struct pipe_buffer to the desired
 *    destination.  It returns when there's no more buffers left in
 *    the pipe or if the requested number of bytes (@sd->total_len)
 *    have been copied.  It returns a positive number (one) if the
 *    pipe needs to be filled with more data, zero if the required
 *    number of bytes have been copied and -errno on error.
 *
 *    This, together with splice_from_pipe_{begin,end,next}, may be
 *    used to implement the functionality of __splice_from_pipe() when
 *    locking is required around copying the pipe buffers to the
 *    destination.
 */
<<<<<<< HEAD
int splice_from_pipe_feed(struct pipe_inode_info *pipe, struct splice_desc *sd,
			  splice_actor *actor)
{
	int ret;

	while (pipe->nrbufs) {
		struct pipe_buffer *buf = pipe->bufs + pipe->curbuf;
		const struct pipe_buf_operations *ops = buf->ops;
=======
static int splice_from_pipe_feed(struct pipe_inode_info *pipe, struct splice_desc *sd,
			  splice_actor *actor)
{
	unsigned int head = pipe->head;
	unsigned int tail = pipe->tail;
	unsigned int mask = pipe->ring_size - 1;
	int ret;

	while (!pipe_empty(head, tail)) {
		struct pipe_buffer *buf = &pipe->bufs[tail & mask];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		sd->len = buf->len;
		if (sd->len > sd->total_len)
			sd->len = sd->total_len;

<<<<<<< HEAD
		ret = buf->ops->confirm(pipe, buf);
=======
		ret = pipe_buf_confirm(pipe, buf);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (unlikely(ret)) {
			if (ret == -ENODATA)
				ret = 0;
			return ret;
		}

		ret = actor(pipe, buf, sd);
		if (ret <= 0)
			return ret;

		buf->offset += ret;
		buf->len -= ret;

		sd->num_spliced += ret;
		sd->len -= ret;
		sd->pos += ret;
		sd->total_len -= ret;

		if (!buf->len) {
<<<<<<< HEAD
			buf->ops = NULL;
			ops->release(pipe, buf);
			pipe->curbuf = (pipe->curbuf + 1) & (pipe->buffers - 1);
			pipe->nrbufs--;
			if (pipe->inode)
=======
			pipe_buf_release(pipe, buf);
			tail++;
			pipe->tail = tail;
			if (pipe->files)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				sd->need_wakeup = true;
		}

		if (!sd->total_len)
			return 0;
	}

	return 1;
}
<<<<<<< HEAD
EXPORT_SYMBOL(splice_from_pipe_feed);
=======

/* We know we have a pipe buffer, but maybe it's empty? */
static inline bool eat_empty_buffer(struct pipe_inode_info *pipe)
{
	unsigned int tail = pipe->tail;
	unsigned int mask = pipe->ring_size - 1;
	struct pipe_buffer *buf = &pipe->bufs[tail & mask];

	if (unlikely(!buf->len)) {
		pipe_buf_release(pipe, buf);
		pipe->tail = tail+1;
		return true;
	}

	return false;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * splice_from_pipe_next - wait for some data to splice from
 * @pipe:	pipe to splice from
 * @sd:		information about the splice operation
 *
 * Description:
 *    This function will wait for some data and return a positive
 *    value (one) if pipe buffers are available.  It will return zero
 *    or -errno if no more data needs to be spliced.
 */
<<<<<<< HEAD
int splice_from_pipe_next(struct pipe_inode_info *pipe, struct splice_desc *sd)
=======
static int splice_from_pipe_next(struct pipe_inode_info *pipe, struct splice_desc *sd)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	/*
	 * Check for signal early to make process killable when there are
	 * always buffers available
	 */
	if (signal_pending(current))
		return -ERESTARTSYS;

<<<<<<< HEAD
	while (!pipe->nrbufs) {
		if (!pipe->writers)
			return 0;

		if (!pipe->waiting_writers && sd->num_spliced)
=======
repeat:
	while (pipe_empty(pipe->head, pipe->tail)) {
		if (!pipe->writers)
			return 0;

		if (sd->num_spliced)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return 0;

		if (sd->flags & SPLICE_F_NONBLOCK)
			return -EAGAIN;

		if (signal_pending(current))
			return -ERESTARTSYS;

		if (sd->need_wakeup) {
			wakeup_pipe_writers(pipe);
			sd->need_wakeup = false;
		}

<<<<<<< HEAD
		pipe_wait(pipe);
	}

	return 1;
}
EXPORT_SYMBOL(splice_from_pipe_next);
=======
		pipe_wait_readable(pipe);
	}

	if (eat_empty_buffer(pipe))
		goto repeat;

	return 1;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * splice_from_pipe_begin - start splicing from pipe
 * @sd:		information about the splice operation
 *
 * Description:
 *    This function should be called before a loop containing
 *    splice_from_pipe_next() and splice_from_pipe_feed() to
 *    initialize the necessary fields of @sd.
 */
<<<<<<< HEAD
void splice_from_pipe_begin(struct splice_desc *sd)
=======
static void splice_from_pipe_begin(struct splice_desc *sd)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	sd->num_spliced = 0;
	sd->need_wakeup = false;
}
<<<<<<< HEAD
EXPORT_SYMBOL(splice_from_pipe_begin);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * splice_from_pipe_end - finish splicing from pipe
 * @pipe:	pipe to splice from
 * @sd:		information about the splice operation
 *
 * Description:
 *    This function will wake up pipe writers if necessary.  It should
 *    be called after a loop containing splice_from_pipe_next() and
 *    splice_from_pipe_feed().
 */
<<<<<<< HEAD
void splice_from_pipe_end(struct pipe_inode_info *pipe, struct splice_desc *sd)
=======
static void splice_from_pipe_end(struct pipe_inode_info *pipe, struct splice_desc *sd)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	if (sd->need_wakeup)
		wakeup_pipe_writers(pipe);
}
<<<<<<< HEAD
EXPORT_SYMBOL(splice_from_pipe_end);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * __splice_from_pipe - splice data from a pipe to given actor
 * @pipe:	pipe to splice from
 * @sd:		information to @actor
 * @actor:	handler that splices the data
 *
 * Description:
 *    This function does little more than loop over the pipe and call
 *    @actor to do the actual moving of a single struct pipe_buffer to
<<<<<<< HEAD
 *    the desired destination. See pipe_to_file, pipe_to_sendpage, or
=======
 *    the desired destination. See pipe_to_file, pipe_to_sendmsg, or
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *    pipe_to_user.
 *
 */
ssize_t __splice_from_pipe(struct pipe_inode_info *pipe, struct splice_desc *sd,
			   splice_actor *actor)
{
	int ret;

	splice_from_pipe_begin(sd);
	do {
		cond_resched();
		ret = splice_from_pipe_next(pipe, sd);
		if (ret > 0)
			ret = splice_from_pipe_feed(pipe, sd, actor);
	} while (ret > 0);
	splice_from_pipe_end(pipe, sd);

	return sd->num_spliced ? sd->num_spliced : ret;
}
EXPORT_SYMBOL(__splice_from_pipe);

/**
 * splice_from_pipe - splice data from a pipe to a file
 * @pipe:	pipe to splice from
 * @out:	file to splice to
 * @ppos:	position in @out
 * @len:	how many bytes to splice
 * @flags:	splice modifier flags
 * @actor:	handler that splices the data
 *
 * Description:
 *    See __splice_from_pipe. This function locks the pipe inode,
 *    otherwise it's identical to __splice_from_pipe().
 *
 */
ssize_t splice_from_pipe(struct pipe_inode_info *pipe, struct file *out,
			 loff_t *ppos, size_t len, unsigned int flags,
			 splice_actor *actor)
{
	ssize_t ret;
	struct splice_desc sd = {
		.total_len = len,
		.flags = flags,
		.pos = *ppos,
		.u.file = out,
	};

	pipe_lock(pipe);
	ret = __splice_from_pipe(pipe, &sd, actor);
	pipe_unlock(pipe);

	return ret;
}

/**
<<<<<<< HEAD
 * generic_file_splice_write - splice data from a pipe to a file
=======
 * iter_file_splice_write - splice data from a pipe to a file
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @pipe:	pipe info
 * @out:	file to write to
 * @ppos:	position in @out
 * @len:	number of bytes to splice
 * @flags:	splice modifier flags
 *
 * Description:
 *    Will either move or copy pages (determined by @flags options) from
 *    the given pipe inode to the given file.
<<<<<<< HEAD
 *
 */
ssize_t
generic_file_splice_write(struct pipe_inode_info *pipe, struct file *out,
			  loff_t *ppos, size_t len, unsigned int flags)
{
	struct address_space *mapping = out->f_mapping;
	struct inode *inode = mapping->host;
	struct splice_desc sd = {
		.flags = flags,
		.u.file = out,
	};
	ssize_t ret;

	ret = generic_write_checks(out, ppos, &len, S_ISBLK(inode->i_mode));
	if (ret)
		return ret;
	sd.total_len = len;
	sd.pos = *ppos;
=======
 *    This one is ->write_iter-based.
 *
 */
ssize_t
iter_file_splice_write(struct pipe_inode_info *pipe, struct file *out,
			  loff_t *ppos, size_t len, unsigned int flags)
{
	struct splice_desc sd = {
		.total_len = len,
		.flags = flags,
		.pos = *ppos,
		.u.file = out,
	};
	int nbufs = pipe->max_usage;
	struct bio_vec *array;
	ssize_t ret;

	if (!out->f_op->write_iter)
		return -EINVAL;

	array = kcalloc(nbufs, sizeof(struct bio_vec), GFP_KERNEL);
	if (unlikely(!array))
		return -ENOMEM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	pipe_lock(pipe);

	splice_from_pipe_begin(&sd);
<<<<<<< HEAD
	do {
=======
	while (sd.total_len) {
		struct kiocb kiocb;
		struct iov_iter from;
		unsigned int head, tail, mask;
		size_t left;
		int n;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = splice_from_pipe_next(pipe, &sd);
		if (ret <= 0)
			break;

<<<<<<< HEAD
		mutex_lock_nested(&inode->i_mutex, I_MUTEX_CHILD);
		ret = file_remove_suid(out);
		if (!ret) {
			ret = file_update_time(out);
			if (!ret)
				ret = splice_from_pipe_feed(pipe, &sd,
							    pipe_to_file);
		}
		mutex_unlock(&inode->i_mutex);
	} while (ret > 0);
=======
		if (unlikely(nbufs < pipe->max_usage)) {
			kfree(array);
			nbufs = pipe->max_usage;
			array = kcalloc(nbufs, sizeof(struct bio_vec),
					GFP_KERNEL);
			if (!array) {
				ret = -ENOMEM;
				break;
			}
		}

		head = pipe->head;
		tail = pipe->tail;
		mask = pipe->ring_size - 1;

		/* build the vector */
		left = sd.total_len;
		for (n = 0; !pipe_empty(head, tail) && left && n < nbufs; tail++) {
			struct pipe_buffer *buf = &pipe->bufs[tail & mask];
			size_t this_len = buf->len;

			/* zero-length bvecs are not supported, skip them */
			if (!this_len)
				continue;
			this_len = min(this_len, left);

			ret = pipe_buf_confirm(pipe, buf);
			if (unlikely(ret)) {
				if (ret == -ENODATA)
					ret = 0;
				goto done;
			}

			bvec_set_page(&array[n], buf->page, this_len,
				      buf->offset);
			left -= this_len;
			n++;
		}

		iov_iter_bvec(&from, ITER_SOURCE, array, n, sd.total_len - left);
		init_sync_kiocb(&kiocb, out);
		kiocb.ki_pos = sd.pos;
		ret = call_write_iter(out, &kiocb, &from);
		sd.pos = kiocb.ki_pos;
		if (ret <= 0)
			break;

		sd.num_spliced += ret;
		sd.total_len -= ret;
		*ppos = sd.pos;

		/* dismiss the fully eaten buffers, adjust the partial one */
		tail = pipe->tail;
		while (ret) {
			struct pipe_buffer *buf = &pipe->bufs[tail & mask];
			if (ret >= buf->len) {
				ret -= buf->len;
				buf->len = 0;
				pipe_buf_release(pipe, buf);
				tail++;
				pipe->tail = tail;
				if (pipe->files)
					sd.need_wakeup = true;
			} else {
				buf->offset += ret;
				buf->len -= ret;
				ret = 0;
			}
		}
	}
done:
	kfree(array);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	splice_from_pipe_end(pipe, &sd);

	pipe_unlock(pipe);

	if (sd.num_spliced)
		ret = sd.num_spliced;

<<<<<<< HEAD
	if (ret > 0) {
		unsigned long nr_pages;
		int err;

		nr_pages = (ret + PAGE_CACHE_SIZE - 1) >> PAGE_CACHE_SHIFT;

		err = generic_write_sync(out, *ppos, ret);
		if (err)
			ret = err;
		else
			*ppos += ret;
		balance_dirty_pages_ratelimited_nr(mapping, nr_pages);
	}

	return ret;
}

EXPORT_SYMBOL(generic_file_splice_write);

static int write_pipe_buf(struct pipe_inode_info *pipe, struct pipe_buffer *buf,
			  struct splice_desc *sd)
{
	int ret;
	void *data;

	data = buf->ops->map(pipe, buf, 0);
	ret = kernel_write(sd->u.file, data + buf->offset, sd->len, sd->pos);
	buf->ops->unmap(pipe, buf, data);

	return ret;
}

static ssize_t default_file_splice_write(struct pipe_inode_info *pipe,
					 struct file *out, loff_t *ppos,
					 size_t len, unsigned int flags)
{
	ssize_t ret;

	if (unlikely(!(out->f_mode & FMODE_SPLICE_WRITE)))
		return -EINVAL;

	ret = splice_from_pipe(pipe, out, ppos, len, flags, write_pipe_buf);
	if (ret > 0)
		*ppos += ret;

	return ret;
}

/**
 * generic_splice_sendpage - splice data from a pipe to a socket
=======
	return ret;
}

EXPORT_SYMBOL(iter_file_splice_write);

#ifdef CONFIG_NET
/**
 * splice_to_socket - splice data from a pipe to a socket
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @pipe:	pipe to splice from
 * @out:	socket to write to
 * @ppos:	position in @out
 * @len:	number of bytes to splice
 * @flags:	splice modifier flags
 *
 * Description:
 *    Will send @len bytes from the pipe to a network socket. No data copying
 *    is involved.
 *
 */
<<<<<<< HEAD
ssize_t generic_splice_sendpage(struct pipe_inode_info *pipe, struct file *out,
				loff_t *ppos, size_t len, unsigned int flags)
{
	return splice_from_pipe(pipe, out, ppos, len, flags, pipe_to_sendpage);
}

EXPORT_SYMBOL(generic_splice_sendpage);
=======
ssize_t splice_to_socket(struct pipe_inode_info *pipe, struct file *out,
			 loff_t *ppos, size_t len, unsigned int flags)
{
	struct socket *sock = sock_from_file(out);
	struct bio_vec bvec[16];
	struct msghdr msg = {};
	ssize_t ret = 0;
	size_t spliced = 0;
	bool need_wakeup = false;

	pipe_lock(pipe);

	while (len > 0) {
		unsigned int head, tail, mask, bc = 0;
		size_t remain = len;

		/*
		 * Check for signal early to make process killable when there
		 * are always buffers available
		 */
		ret = -ERESTARTSYS;
		if (signal_pending(current))
			break;

		while (pipe_empty(pipe->head, pipe->tail)) {
			ret = 0;
			if (!pipe->writers)
				goto out;

			if (spliced)
				goto out;

			ret = -EAGAIN;
			if (flags & SPLICE_F_NONBLOCK)
				goto out;

			ret = -ERESTARTSYS;
			if (signal_pending(current))
				goto out;

			if (need_wakeup) {
				wakeup_pipe_writers(pipe);
				need_wakeup = false;
			}

			pipe_wait_readable(pipe);
		}

		head = pipe->head;
		tail = pipe->tail;
		mask = pipe->ring_size - 1;

		while (!pipe_empty(head, tail)) {
			struct pipe_buffer *buf = &pipe->bufs[tail & mask];
			size_t seg;

			if (!buf->len) {
				tail++;
				continue;
			}

			seg = min_t(size_t, remain, buf->len);

			ret = pipe_buf_confirm(pipe, buf);
			if (unlikely(ret)) {
				if (ret == -ENODATA)
					ret = 0;
				break;
			}

			bvec_set_page(&bvec[bc++], buf->page, seg, buf->offset);
			remain -= seg;
			if (remain == 0 || bc >= ARRAY_SIZE(bvec))
				break;
			tail++;
		}

		if (!bc)
			break;

		msg.msg_flags = MSG_SPLICE_PAGES;
		if (flags & SPLICE_F_MORE)
			msg.msg_flags |= MSG_MORE;
		if (remain && pipe_occupancy(pipe->head, tail) > 0)
			msg.msg_flags |= MSG_MORE;
		if (out->f_flags & O_NONBLOCK)
			msg.msg_flags |= MSG_DONTWAIT;

		iov_iter_bvec(&msg.msg_iter, ITER_SOURCE, bvec, bc,
			      len - remain);
		ret = sock_sendmsg(sock, &msg);
		if (ret <= 0)
			break;

		spliced += ret;
		len -= ret;
		tail = pipe->tail;
		while (ret > 0) {
			struct pipe_buffer *buf = &pipe->bufs[tail & mask];
			size_t seg = min_t(size_t, ret, buf->len);

			buf->offset += seg;
			buf->len -= seg;
			ret -= seg;

			if (!buf->len) {
				pipe_buf_release(pipe, buf);
				tail++;
			}
		}

		if (tail != pipe->tail) {
			pipe->tail = tail;
			if (pipe->files)
				need_wakeup = true;
		}
	}

out:
	pipe_unlock(pipe);
	if (need_wakeup)
		wakeup_pipe_writers(pipe);
	return spliced ?: ret;
}
#endif

static int warn_unsupported(struct file *file, const char *op)
{
	pr_debug_ratelimited(
		"splice %s not supported for file %pD4 (pid: %d comm: %.20s)\n",
		op, file, current->pid, current->comm);
	return -EINVAL;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Attempt to initiate a splice from pipe to file.
 */
<<<<<<< HEAD
static long do_splice_from(struct pipe_inode_info *pipe, struct file *out,
			   loff_t *ppos, size_t len, unsigned int flags)
{
	ssize_t (*splice_write)(struct pipe_inode_info *, struct file *,
				loff_t *, size_t, unsigned int);
	int ret;

	if (unlikely(!(out->f_mode & FMODE_WRITE)))
		return -EBADF;

	if (unlikely(out->f_flags & O_APPEND))
		return -EINVAL;

	ret = rw_verify_area(WRITE, out, ppos, len);
	if (unlikely(ret < 0))
		return ret;

	if (out->f_op && out->f_op->splice_write)
		splice_write = out->f_op->splice_write;
	else
		splice_write = default_file_splice_write;

	return splice_write(pipe, out, ppos, len, flags);
}

/*
 * Attempt to initiate a splice from a file to a pipe.
 */
static long do_splice_to(struct file *in, loff_t *ppos,
			 struct pipe_inode_info *pipe, size_t len,
			 unsigned int flags)
{
	ssize_t (*splice_read)(struct file *, loff_t *,
			       struct pipe_inode_info *, size_t, unsigned int);
	int ret;

	if (unlikely(!(in->f_mode & FMODE_READ)))
		return -EBADF;
=======
static ssize_t do_splice_from(struct pipe_inode_info *pipe, struct file *out,
			      loff_t *ppos, size_t len, unsigned int flags)
{
	if (unlikely(!out->f_op->splice_write))
		return warn_unsupported(out, "write");
	return out->f_op->splice_write(pipe, out, ppos, len, flags);
}

/*
 * Indicate to the caller that there was a premature EOF when reading from the
 * source and the caller didn't indicate they would be sending more data after
 * this.
 */
static void do_splice_eof(struct splice_desc *sd)
{
	if (sd->splice_eof)
		sd->splice_eof(sd);
}

/*
 * Callers already called rw_verify_area() on the entire range.
 * No need to call it for sub ranges.
 */
static ssize_t do_splice_read(struct file *in, loff_t *ppos,
			      struct pipe_inode_info *pipe, size_t len,
			      unsigned int flags)
{
	unsigned int p_space;

	if (unlikely(!(in->f_mode & FMODE_READ)))
		return -EBADF;
	if (!len)
		return 0;

	/* Don't try to read more the pipe has space for. */
	p_space = pipe->max_usage - pipe_occupancy(pipe->head, pipe->tail);
	len = min_t(size_t, len, p_space << PAGE_SHIFT);

	if (unlikely(len > MAX_RW_COUNT))
		len = MAX_RW_COUNT;

	if (unlikely(!in->f_op->splice_read))
		return warn_unsupported(in, "read");
	/*
	 * O_DIRECT and DAX don't deal with the pagecache, so we allocate a
	 * buffer, copy into it and splice that into the pipe.
	 */
	if ((in->f_flags & O_DIRECT) || IS_DAX(in->f_mapping->host))
		return copy_splice_read(in, ppos, pipe, len, flags);
	return in->f_op->splice_read(in, ppos, pipe, len, flags);
}

/**
 * vfs_splice_read - Read data from a file and splice it into a pipe
 * @in:		File to splice from
 * @ppos:	Input file offset
 * @pipe:	Pipe to splice to
 * @len:	Number of bytes to splice
 * @flags:	Splice modifier flags (SPLICE_F_*)
 *
 * Splice the requested amount of data from the input file to the pipe.  This
 * is synchronous as the caller must hold the pipe lock across the entire
 * operation.
 *
 * If successful, it returns the amount of data spliced, 0 if it hit the EOF or
 * a hole and a negative error code otherwise.
 */
ssize_t vfs_splice_read(struct file *in, loff_t *ppos,
			struct pipe_inode_info *pipe, size_t len,
			unsigned int flags)
{
	ssize_t ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ret = rw_verify_area(READ, in, ppos, len);
	if (unlikely(ret < 0))
		return ret;

<<<<<<< HEAD
	if (in->f_op && in->f_op->splice_read)
		splice_read = in->f_op->splice_read;
	else
		splice_read = default_file_splice_read;

	return splice_read(in, ppos, pipe, len, flags);
}
=======
	return do_splice_read(in, ppos, pipe, len, flags);
}
EXPORT_SYMBOL_GPL(vfs_splice_read);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * splice_direct_to_actor - splices data directly between two non-pipes
 * @in:		file to splice from
 * @sd:		actor information on where to splice to
 * @actor:	handles the data splicing
 *
 * Description:
 *    This is a special case helper to splice directly between two
 *    points, without requiring an explicit pipe. Internally an allocated
 *    pipe is cached in the process, and reused during the lifetime of
 *    that process.
 *
 */
ssize_t splice_direct_to_actor(struct file *in, struct splice_desc *sd,
			       splice_direct_actor *actor)
{
	struct pipe_inode_info *pipe;
<<<<<<< HEAD
	long ret, bytes;
	umode_t i_mode;
=======
	ssize_t ret, bytes;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	size_t len;
	int i, flags, more;

	/*
<<<<<<< HEAD
	 * We require the input being a regular file, as we don't want to
	 * randomly drop data for eg socket -> socket splicing. Use the
	 * piped splicing for that!
	 */
	i_mode = in->f_path.dentry->d_inode->i_mode;
	if (unlikely(!S_ISREG(i_mode) && !S_ISBLK(i_mode)))
=======
	 * We require the input to be seekable, as we don't want to randomly
	 * drop data for eg socket -> socket splicing. Use the piped splicing
	 * for that!
	 */
	if (unlikely(!(in->f_mode & FMODE_LSEEK)))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;

	/*
	 * neither in nor out is a pipe, setup an internal pipe attached to
	 * 'out' and transfer the wanted data from 'in' to 'out' through that
	 */
	pipe = current->splice_pipe;
	if (unlikely(!pipe)) {
<<<<<<< HEAD
		pipe = alloc_pipe_info(NULL);
=======
		pipe = alloc_pipe_info();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!pipe)
			return -ENOMEM;

		/*
		 * We don't have an immediate reader, but we'll read the stuff
		 * out of the pipe right after the splice_to_pipe(). So set
		 * PIPE_READERS appropriately.
		 */
		pipe->readers = 1;

		current->splice_pipe = pipe;
	}

	/*
	 * Do the splice.
	 */
<<<<<<< HEAD
	ret = 0;
	bytes = 0;
	len = sd->total_len;
	flags = sd->flags;

	/*
	 * Don't block on output, we have to drain the direct pipe.
	 */
	sd->flags &= ~SPLICE_F_NONBLOCK;
	more = sd->flags & SPLICE_F_MORE;
=======
	bytes = 0;
	len = sd->total_len;

	/* Don't block on output, we have to drain the direct pipe. */
	flags = sd->flags;
	sd->flags &= ~SPLICE_F_NONBLOCK;

	/*
	 * We signal MORE until we've read sufficient data to fulfill the
	 * request and we keep signalling it if the caller set it.
	 */
	more = sd->flags & SPLICE_F_MORE;
	sd->flags |= SPLICE_F_MORE;

	WARN_ON_ONCE(!pipe_empty(pipe->head, pipe->tail));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	while (len) {
		size_t read_len;
		loff_t pos = sd->pos, prev_pos = pos;

<<<<<<< HEAD
		ret = do_splice_to(in, &pos, pipe, len, flags);
		if (unlikely(ret <= 0))
			goto out_release;
=======
		ret = do_splice_read(in, &pos, pipe, len, flags);
		if (unlikely(ret <= 0))
			goto read_failure;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		read_len = ret;
		sd->total_len = read_len;

		/*
<<<<<<< HEAD
		 * If more data is pending, set SPLICE_F_MORE
		 * If this is the last data and SPLICE_F_MORE was not set
		 * initially, clears it.
		 */
		if (read_len < len)
			sd->flags |= SPLICE_F_MORE;
		else if (!more)
			sd->flags &= ~SPLICE_F_MORE;
=======
		 * If we now have sufficient data to fulfill the request then
		 * we clear SPLICE_F_MORE if it was not set initially.
		 */
		if (read_len >= len && !more)
			sd->flags &= ~SPLICE_F_MORE;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * NOTE: nonblocking mode only applies to the input. We
		 * must not do the output in nonblocking mode as then we
		 * could get stuck data in the internal pipe:
		 */
		ret = actor(pipe, sd);
		if (unlikely(ret <= 0)) {
			sd->pos = prev_pos;
			goto out_release;
		}

		bytes += ret;
		len -= ret;
		sd->pos = pos;

		if (ret < read_len) {
			sd->pos = prev_pos + ret;
			goto out_release;
		}
	}

done:
<<<<<<< HEAD
	pipe->nrbufs = pipe->curbuf = 0;
	file_accessed(in);
	return bytes;

=======
	pipe->tail = pipe->head = 0;
	file_accessed(in);
	return bytes;

read_failure:
	/*
	 * If the user did *not* set SPLICE_F_MORE *and* we didn't hit that
	 * "use all of len" case that cleared SPLICE_F_MORE, *and* we did a
	 * "->splice_in()" that returned EOF (ie zero) *and* we have sent at
	 * least 1 byte *then* we will also do the ->splice_eof() call.
	 */
	if (ret == 0 && !more && len > 0 && bytes)
		do_splice_eof(sd);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out_release:
	/*
	 * If we did an incomplete transfer we must release
	 * the pipe buffers in question:
	 */
<<<<<<< HEAD
	for (i = 0; i < pipe->buffers; i++) {
		struct pipe_buffer *buf = pipe->bufs + i;

		if (buf->ops) {
			buf->ops->release(pipe, buf);
			buf->ops = NULL;
		}
=======
	for (i = 0; i < pipe->ring_size; i++) {
		struct pipe_buffer *buf = &pipe->bufs[i];

		if (buf->ops)
			pipe_buf_release(pipe, buf);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (!bytes)
		bytes = ret;

	goto done;
}
EXPORT_SYMBOL(splice_direct_to_actor);

static int direct_splice_actor(struct pipe_inode_info *pipe,
			       struct splice_desc *sd)
{
	struct file *file = sd->u.file;
<<<<<<< HEAD

	return do_splice_from(pipe, file, &file->f_pos, sd->total_len,
			      sd->flags);
}

=======
	long ret;

	file_start_write(file);
	ret = do_splice_from(pipe, file, sd->opos, sd->total_len, sd->flags);
	file_end_write(file);
	return ret;
}

static int splice_file_range_actor(struct pipe_inode_info *pipe,
					struct splice_desc *sd)
{
	struct file *file = sd->u.file;

	return do_splice_from(pipe, file, sd->opos, sd->total_len, sd->flags);
}

static void direct_file_splice_eof(struct splice_desc *sd)
{
	struct file *file = sd->u.file;

	if (file->f_op->splice_eof)
		file->f_op->splice_eof(file);
}

static ssize_t do_splice_direct_actor(struct file *in, loff_t *ppos,
				      struct file *out, loff_t *opos,
				      size_t len, unsigned int flags,
				      splice_direct_actor *actor)
{
	struct splice_desc sd = {
		.len		= len,
		.total_len	= len,
		.flags		= flags,
		.pos		= *ppos,
		.u.file		= out,
		.splice_eof	= direct_file_splice_eof,
		.opos		= opos,
	};
	ssize_t ret;

	if (unlikely(!(out->f_mode & FMODE_WRITE)))
		return -EBADF;

	if (unlikely(out->f_flags & O_APPEND))
		return -EINVAL;

	ret = splice_direct_to_actor(in, &sd, actor);
	if (ret > 0)
		*ppos = sd.pos;

	return ret;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * do_splice_direct - splices data directly between two files
 * @in:		file to splice from
 * @ppos:	input file offset
 * @out:	file to splice to
<<<<<<< HEAD
=======
 * @opos:	output file offset
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @len:	number of bytes to splice
 * @flags:	splice modifier flags
 *
 * Description:
 *    For use by do_sendfile(). splice can easily emulate sendfile, but
 *    doing it in the application would incur an extra system call
 *    (splice in + splice out, as compared to just sendfile()). So this helper
 *    can splice directly through a process-private pipe.
 *
<<<<<<< HEAD
 */
long do_splice_direct(struct file *in, loff_t *ppos, struct file *out,
		      size_t len, unsigned int flags)
{
	struct splice_desc sd = {
		.len		= len,
		.total_len	= len,
		.flags		= flags,
		.pos		= *ppos,
		.u.file		= out,
	};
	long ret;

	ret = splice_direct_to_actor(in, &sd, direct_splice_actor);
	if (ret > 0)
		*ppos = sd.pos;

	return ret;
=======
 * Callers already called rw_verify_area() on the entire range.
 */
ssize_t do_splice_direct(struct file *in, loff_t *ppos, struct file *out,
			 loff_t *opos, size_t len, unsigned int flags)
{
	return do_splice_direct_actor(in, ppos, out, opos, len, flags,
				      direct_splice_actor);
}
EXPORT_SYMBOL(do_splice_direct);

/**
 * splice_file_range - splices data between two files for copy_file_range()
 * @in:		file to splice from
 * @ppos:	input file offset
 * @out:	file to splice to
 * @opos:	output file offset
 * @len:	number of bytes to splice
 *
 * Description:
 *    For use by ->copy_file_range() methods.
 *    Like do_splice_direct(), but vfs_copy_file_range() already holds
 *    start_file_write() on @out file.
 *
 * Callers already called rw_verify_area() on the entire range.
 */
ssize_t splice_file_range(struct file *in, loff_t *ppos, struct file *out,
			  loff_t *opos, size_t len)
{
	lockdep_assert(file_write_started(out));

	return do_splice_direct_actor(in, ppos, out, opos,
				      min_t(size_t, len, MAX_RW_COUNT),
				      0, splice_file_range_actor);
}
EXPORT_SYMBOL(splice_file_range);

static int wait_for_space(struct pipe_inode_info *pipe, unsigned flags)
{
	for (;;) {
		if (unlikely(!pipe->readers)) {
			send_sig(SIGPIPE, current, 0);
			return -EPIPE;
		}
		if (!pipe_full(pipe->head, pipe->tail, pipe->max_usage))
			return 0;
		if (flags & SPLICE_F_NONBLOCK)
			return -EAGAIN;
		if (signal_pending(current))
			return -ERESTARTSYS;
		pipe_wait_writable(pipe);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int splice_pipe_to_pipe(struct pipe_inode_info *ipipe,
			       struct pipe_inode_info *opipe,
			       size_t len, unsigned int flags);

<<<<<<< HEAD
/*
 * Determine where to splice to/from.
 */
static long do_splice(struct file *in, loff_t __user *off_in,
		      struct file *out, loff_t __user *off_out,
		      size_t len, unsigned int flags)
{
	struct pipe_inode_info *ipipe;
	struct pipe_inode_info *opipe;
	loff_t offset, *off;
	long ret;

	ipipe = get_pipe_info(in);
	opipe = get_pipe_info(out);
=======
ssize_t splice_file_to_pipe(struct file *in,
			    struct pipe_inode_info *opipe,
			    loff_t *offset,
			    size_t len, unsigned int flags)
{
	ssize_t ret;

	pipe_lock(opipe);
	ret = wait_for_space(opipe, flags);
	if (!ret)
		ret = do_splice_read(in, offset, opipe, len, flags);
	pipe_unlock(opipe);
	if (ret > 0)
		wakeup_pipe_readers(opipe);
	return ret;
}

/*
 * Determine where to splice to/from.
 */
ssize_t do_splice(struct file *in, loff_t *off_in, struct file *out,
		  loff_t *off_out, size_t len, unsigned int flags)
{
	struct pipe_inode_info *ipipe;
	struct pipe_inode_info *opipe;
	loff_t offset;
	ssize_t ret;

	if (unlikely(!(in->f_mode & FMODE_READ) ||
		     !(out->f_mode & FMODE_WRITE)))
		return -EBADF;

	ipipe = get_pipe_info(in, true);
	opipe = get_pipe_info(out, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (ipipe && opipe) {
		if (off_in || off_out)
			return -ESPIPE;

<<<<<<< HEAD
		if (!(in->f_mode & FMODE_READ))
			return -EBADF;

		if (!(out->f_mode & FMODE_WRITE))
			return -EBADF;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* Splicing to self would be fun, but... */
		if (ipipe == opipe)
			return -EINVAL;

<<<<<<< HEAD
		return splice_pipe_to_pipe(ipipe, opipe, len, flags);
	}

	if (ipipe) {
=======
		if ((in->f_flags | out->f_flags) & O_NONBLOCK)
			flags |= SPLICE_F_NONBLOCK;

		ret = splice_pipe_to_pipe(ipipe, opipe, len, flags);
	} else if (ipipe) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (off_in)
			return -ESPIPE;
		if (off_out) {
			if (!(out->f_mode & FMODE_PWRITE))
				return -EINVAL;
<<<<<<< HEAD
			if (copy_from_user(&offset, off_out, sizeof(loff_t)))
				return -EFAULT;
			off = &offset;
		} else
			off = &out->f_pos;

		ret = do_splice_from(ipipe, out, off, len, flags);

		if (off_out && copy_to_user(off_out, off, sizeof(loff_t)))
			ret = -EFAULT;

		return ret;
	}

	if (opipe) {
=======
			offset = *off_out;
		} else {
			offset = out->f_pos;
		}

		if (unlikely(out->f_flags & O_APPEND))
			return -EINVAL;

		ret = rw_verify_area(WRITE, out, &offset, len);
		if (unlikely(ret < 0))
			return ret;

		if (in->f_flags & O_NONBLOCK)
			flags |= SPLICE_F_NONBLOCK;

		file_start_write(out);
		ret = do_splice_from(ipipe, out, &offset, len, flags);
		file_end_write(out);

		if (!off_out)
			out->f_pos = offset;
		else
			*off_out = offset;
	} else if (opipe) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (off_out)
			return -ESPIPE;
		if (off_in) {
			if (!(in->f_mode & FMODE_PREAD))
				return -EINVAL;
<<<<<<< HEAD
			if (copy_from_user(&offset, off_in, sizeof(loff_t)))
				return -EFAULT;
			off = &offset;
		} else
			off = &in->f_pos;

		ret = do_splice_to(in, off, opipe, len, flags);

		if (off_in && copy_to_user(off_in, off, sizeof(loff_t)))
			ret = -EFAULT;

		return ret;
	}

	return -EINVAL;
}

/*
 * Map an iov into an array of pages and offset/length tupples. With the
 * partial_page structure, we can map several non-contiguous ranges into
 * our ones pages[] map instead of splitting that operation into pieces.
 * Could easily be exported as a generic helper for other users, in which
 * case one would probably want to add a 'max_nr_pages' parameter as well.
 */
static int get_iovec_page_array(const struct iovec __user *iov,
				unsigned int nr_vecs, struct page **pages,
				struct partial_page *partial, int aligned,
				unsigned int pipe_buffers)
{
	int buffers = 0, error = 0;

	while (nr_vecs) {
		unsigned long off, npages;
		struct iovec entry;
		void __user *base;
		size_t len;
		int i;

		error = -EFAULT;
		if (copy_from_user(&entry, iov, sizeof(entry)))
			break;

		base = entry.iov_base;
		len = entry.iov_len;

		/*
		 * Sanity check this iovec. 0 read succeeds.
		 */
		error = 0;
		if (unlikely(!len))
			break;
		error = -EFAULT;
		if (!access_ok(VERIFY_READ, base, len))
			break;

		/*
		 * Get this base offset and number of pages, then map
		 * in the user pages.
		 */
		off = (unsigned long) base & ~PAGE_MASK;

		/*
		 * If asked for alignment, the offset must be zero and the
		 * length a multiple of the PAGE_SIZE.
		 */
		error = -EINVAL;
		if (aligned && (off || len & ~PAGE_MASK))
			break;

		npages = (off + len + PAGE_SIZE - 1) >> PAGE_SHIFT;
		if (npages > pipe_buffers - buffers)
			npages = pipe_buffers - buffers;

		error = get_user_pages_fast((unsigned long)base, npages,
					0, &pages[buffers]);

		if (unlikely(error <= 0))
			break;

		/*
		 * Fill this contiguous range into the partial page map.
		 */
		for (i = 0; i < error; i++) {
			const int plen = min_t(size_t, len, PAGE_SIZE - off);

			partial[buffers].offset = off;
			partial[buffers].len = plen;

			off = 0;
			len -= plen;
			buffers++;
		}

		/*
		 * We didn't complete this iov, stop here since it probably
		 * means we have to move some of this into a pipe to
		 * be able to continue.
		 */
		if (len)
			break;

		/*
		 * Don't continue if we mapped fewer pages than we asked for,
		 * or if we mapped the max number of pages that we have
		 * room for.
		 */
		if (error < npages || buffers == pipe_buffers)
			break;

		nr_vecs--;
		iov++;
	}

	if (buffers)
		return buffers;

	return error;
=======
			offset = *off_in;
		} else {
			offset = in->f_pos;
		}

		ret = rw_verify_area(READ, in, &offset, len);
		if (unlikely(ret < 0))
			return ret;

		if (out->f_flags & O_NONBLOCK)
			flags |= SPLICE_F_NONBLOCK;

		ret = splice_file_to_pipe(in, opipe, &offset, len, flags);

		if (!off_in)
			in->f_pos = offset;
		else
			*off_in = offset;
	} else {
		ret = -EINVAL;
	}

	if (ret > 0) {
		/*
		 * Generate modify out before access in:
		 * do_splice_from() may've already sent modify out,
		 * and this ensures the events get merged.
		 */
		fsnotify_modify(out);
		fsnotify_access(in);
	}

	return ret;
}

static ssize_t __do_splice(struct file *in, loff_t __user *off_in,
			   struct file *out, loff_t __user *off_out,
			   size_t len, unsigned int flags)
{
	struct pipe_inode_info *ipipe;
	struct pipe_inode_info *opipe;
	loff_t offset, *__off_in = NULL, *__off_out = NULL;
	ssize_t ret;

	ipipe = get_pipe_info(in, true);
	opipe = get_pipe_info(out, true);

	if (ipipe) {
		if (off_in)
			return -ESPIPE;
		pipe_clear_nowait(in);
	}
	if (opipe) {
		if (off_out)
			return -ESPIPE;
		pipe_clear_nowait(out);
	}

	if (off_out) {
		if (copy_from_user(&offset, off_out, sizeof(loff_t)))
			return -EFAULT;
		__off_out = &offset;
	}
	if (off_in) {
		if (copy_from_user(&offset, off_in, sizeof(loff_t)))
			return -EFAULT;
		__off_in = &offset;
	}

	ret = do_splice(in, __off_in, out, __off_out, len, flags);
	if (ret < 0)
		return ret;

	if (__off_out && copy_to_user(off_out, __off_out, sizeof(loff_t)))
		return -EFAULT;
	if (__off_in && copy_to_user(off_in, __off_in, sizeof(loff_t)))
		return -EFAULT;

	return ret;
}

static ssize_t iter_to_pipe(struct iov_iter *from,
			    struct pipe_inode_info *pipe,
			    unsigned int flags)
{
	struct pipe_buffer buf = {
		.ops = &user_page_pipe_buf_ops,
		.flags = flags
	};
	size_t total = 0;
	ssize_t ret = 0;

	while (iov_iter_count(from)) {
		struct page *pages[16];
		ssize_t left;
		size_t start;
		int i, n;

		left = iov_iter_get_pages2(from, pages, ~0UL, 16, &start);
		if (left <= 0) {
			ret = left;
			break;
		}

		n = DIV_ROUND_UP(left + start, PAGE_SIZE);
		for (i = 0; i < n; i++) {
			int size = min_t(int, left, PAGE_SIZE - start);

			buf.page = pages[i];
			buf.offset = start;
			buf.len = size;
			ret = add_to_pipe(pipe, &buf);
			if (unlikely(ret < 0)) {
				iov_iter_revert(from, left);
				// this one got dropped by add_to_pipe()
				while (++i < n)
					put_page(pages[i]);
				goto out;
			}
			total += ret;
			left -= size;
			start = 0;
		}
	}
out:
	return total ? total : ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int pipe_to_user(struct pipe_inode_info *pipe, struct pipe_buffer *buf,
			struct splice_desc *sd)
{
<<<<<<< HEAD
	char *src;
	int ret;

	/*
	 * See if we can use the atomic maps, by prefaulting in the
	 * pages and doing an atomic copy
	 */
	if (!fault_in_pages_writeable(sd->u.userptr, sd->len)) {
		src = buf->ops->map(pipe, buf, 1);
		ret = __copy_to_user_inatomic(sd->u.userptr, src + buf->offset,
							sd->len);
		buf->ops->unmap(pipe, buf, src);
		if (!ret) {
			ret = sd->len;
			goto out;
		}
	}

	/*
	 * No dice, use slow non-atomic map and copy
 	 */
	src = buf->ops->map(pipe, buf, 0);

	ret = sd->len;
	if (copy_to_user(sd->u.userptr, src + buf->offset, sd->len))
		ret = -EFAULT;

	buf->ops->unmap(pipe, buf, src);
out:
	if (ret > 0)
		sd->u.userptr += ret;
	return ret;
=======
	int n = copy_page_to_iter(buf->page, buf->offset, sd->len, sd->u.data);
	return n == sd->len ? n : -EFAULT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * For lack of a better implementation, implement vmsplice() to userspace
 * as a simple copy of the pipes pages to the user iov.
 */
<<<<<<< HEAD
static long vmsplice_to_user(struct file *file, const struct iovec __user *iov,
			     unsigned long nr_segs, unsigned int flags)
{
	struct pipe_inode_info *pipe;
	struct splice_desc sd;
	ssize_t size;
	int error;
	long ret;

	pipe = get_pipe_info(file);
	if (!pipe)
		return -EBADF;

	pipe_lock(pipe);

	error = ret = 0;
	while (nr_segs) {
		void __user *base;
		size_t len;

		/*
		 * Get user address base and length for this iovec.
		 */
		error = get_user(base, &iov->iov_base);
		if (unlikely(error))
			break;
		error = get_user(len, &iov->iov_len);
		if (unlikely(error))
			break;

		/*
		 * Sanity check this iovec. 0 read succeeds.
		 */
		if (unlikely(!len))
			break;
		if (unlikely(!base)) {
			error = -EFAULT;
			break;
		}

		if (unlikely(!access_ok(VERIFY_WRITE, base, len))) {
			error = -EFAULT;
			break;
		}

		sd.len = 0;
		sd.total_len = len;
		sd.flags = flags;
		sd.u.userptr = base;
		sd.pos = 0;

		size = __splice_from_pipe(pipe, &sd, pipe_to_user);
		if (size < 0) {
			if (!ret)
				ret = size;

			break;
		}

		ret += size;

		if (size < len)
			break;

		nr_segs--;
		iov++;
	}

	pipe_unlock(pipe);

	if (!ret)
		ret = error;
=======
static ssize_t vmsplice_to_user(struct file *file, struct iov_iter *iter,
				unsigned int flags)
{
	struct pipe_inode_info *pipe = get_pipe_info(file, true);
	struct splice_desc sd = {
		.total_len = iov_iter_count(iter),
		.flags = flags,
		.u.data = iter
	};
	ssize_t ret = 0;

	if (!pipe)
		return -EBADF;

	pipe_clear_nowait(file);

	if (sd.total_len) {
		pipe_lock(pipe);
		ret = __splice_from_pipe(pipe, &sd, pipe_to_user);
		pipe_unlock(pipe);
	}

	if (ret > 0)
		fsnotify_access(file);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ret;
}

/*
 * vmsplice splices a user address range into a pipe. It can be thought of
 * as splice-from-memory, where the regular splice is splice-from-file (or
 * to file). In both cases the output is a pipe, naturally.
 */
<<<<<<< HEAD
static long vmsplice_to_pipe(struct file *file, const struct iovec __user *iov,
			     unsigned long nr_segs, unsigned int flags)
{
	struct pipe_inode_info *pipe;
	struct page *pages[PIPE_DEF_BUFFERS];
	struct partial_page partial[PIPE_DEF_BUFFERS];
	struct splice_pipe_desc spd = {
		.pages = pages,
		.partial = partial,
		.nr_pages_max = PIPE_DEF_BUFFERS,
		.flags = flags,
		.ops = &user_page_pipe_buf_ops,
		.spd_release = spd_release_page,
	};
	long ret;

	pipe = get_pipe_info(file);
	if (!pipe)
		return -EBADF;

	if (splice_grow_spd(pipe, &spd))
		return -ENOMEM;

	spd.nr_pages = get_iovec_page_array(iov, nr_segs, spd.pages,
					    spd.partial, flags & SPLICE_F_GIFT,
					    spd.nr_pages_max);
	if (spd.nr_pages <= 0)
		ret = spd.nr_pages;
	else
		ret = splice_to_pipe(pipe, &spd);

	splice_shrink_spd(&spd);
	return ret;
}

=======
static ssize_t vmsplice_to_pipe(struct file *file, struct iov_iter *iter,
				unsigned int flags)
{
	struct pipe_inode_info *pipe;
	ssize_t ret = 0;
	unsigned buf_flag = 0;

	if (flags & SPLICE_F_GIFT)
		buf_flag = PIPE_BUF_FLAG_GIFT;

	pipe = get_pipe_info(file, true);
	if (!pipe)
		return -EBADF;

	pipe_clear_nowait(file);

	pipe_lock(pipe);
	ret = wait_for_space(pipe, flags);
	if (!ret)
		ret = iter_to_pipe(iter, pipe, buf_flag);
	pipe_unlock(pipe);
	if (ret > 0) {
		wakeup_pipe_readers(pipe);
		fsnotify_modify(file);
	}
	return ret;
}

static int vmsplice_type(struct fd f, int *type)
{
	if (!f.file)
		return -EBADF;
	if (f.file->f_mode & FMODE_WRITE) {
		*type = ITER_SOURCE;
	} else if (f.file->f_mode & FMODE_READ) {
		*type = ITER_DEST;
	} else {
		fdput(f);
		return -EBADF;
	}
	return 0;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Note that vmsplice only really supports true splicing _from_ user memory
 * to a pipe, not the other way around. Splicing from user memory is a simple
 * operation that can be supported without any funky alignment restrictions
 * or nasty vm tricks. We simply map in the user memory and fill them into
 * a pipe. The reverse isn't quite as easy, though. There are two possible
 * solutions for that:
 *
 *	- memcpy() the data internally, at which point we might as well just
 *	  do a regular read() on the buffer anyway.
 *	- Lots of nasty vm tricks, that are neither fast nor flexible (it
 *	  has restriction limitations on both ends of the pipe).
 *
 * Currently we punt and implement it as a normal copy, see pipe_to_user().
 *
 */
<<<<<<< HEAD
SYSCALL_DEFINE4(vmsplice, int, fd, const struct iovec __user *, iov,
		unsigned long, nr_segs, unsigned int, flags)
{
	struct file *file;
	long error;
	int fput;

	if (unlikely(nr_segs > UIO_MAXIOV))
		return -EINVAL;
	else if (unlikely(!nr_segs))
		return 0;

	error = -EBADF;
	file = fget_light(fd, &fput);
	if (file) {
		if (file->f_mode & FMODE_WRITE)
			error = vmsplice_to_pipe(file, iov, nr_segs, flags);
		else if (file->f_mode & FMODE_READ)
			error = vmsplice_to_user(file, iov, nr_segs, flags);

		fput_light(file, fput);
	}

=======
SYSCALL_DEFINE4(vmsplice, int, fd, const struct iovec __user *, uiov,
		unsigned long, nr_segs, unsigned int, flags)
{
	struct iovec iovstack[UIO_FASTIOV];
	struct iovec *iov = iovstack;
	struct iov_iter iter;
	ssize_t error;
	struct fd f;
	int type;

	if (unlikely(flags & ~SPLICE_F_ALL))
		return -EINVAL;

	f = fdget(fd);
	error = vmsplice_type(f, &type);
	if (error)
		return error;

	error = import_iovec(type, uiov, nr_segs,
			     ARRAY_SIZE(iovstack), &iov, &iter);
	if (error < 0)
		goto out_fdput;

	if (!iov_iter_count(&iter))
		error = 0;
	else if (type == ITER_SOURCE)
		error = vmsplice_to_pipe(f.file, &iter, flags);
	else
		error = vmsplice_to_user(f.file, &iter, flags);

	kfree(iov);
out_fdput:
	fdput(f);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return error;
}

SYSCALL_DEFINE6(splice, int, fd_in, loff_t __user *, off_in,
		int, fd_out, loff_t __user *, off_out,
		size_t, len, unsigned int, flags)
{
<<<<<<< HEAD
	long error;
	struct file *in, *out;
	int fput_in, fput_out;
=======
	struct fd in, out;
	ssize_t error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (unlikely(!len))
		return 0;

<<<<<<< HEAD
	error = -EBADF;
	in = fget_light(fd_in, &fput_in);
	if (in) {
		if (in->f_mode & FMODE_READ) {
			out = fget_light(fd_out, &fput_out);
			if (out) {
				if (out->f_mode & FMODE_WRITE)
					error = do_splice(in, off_in,
							  out, off_out,
							  len, flags);
				fput_light(out, fput_out);
			}
		}

		fput_light(in, fput_in);
	}

=======
	if (unlikely(flags & ~SPLICE_F_ALL))
		return -EINVAL;

	error = -EBADF;
	in = fdget(fd_in);
	if (in.file) {
		out = fdget(fd_out);
		if (out.file) {
			error = __do_splice(in.file, off_in, out.file, off_out,
					    len, flags);
			fdput(out);
		}
		fdput(in);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return error;
}

/*
 * Make sure there's data to read. Wait for input if we can, otherwise
 * return an appropriate error.
 */
static int ipipe_prep(struct pipe_inode_info *pipe, unsigned int flags)
{
	int ret;

	/*
<<<<<<< HEAD
	 * Check ->nrbufs without the inode lock first. This function
	 * is speculative anyways, so missing one is ok.
	 */
	if (pipe->nrbufs)
=======
	 * Check the pipe occupancy without the inode lock first. This function
	 * is speculative anyways, so missing one is ok.
	 */
	if (!pipe_empty(pipe->head, pipe->tail))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;

	ret = 0;
	pipe_lock(pipe);

<<<<<<< HEAD
	while (!pipe->nrbufs) {
=======
	while (pipe_empty(pipe->head, pipe->tail)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (signal_pending(current)) {
			ret = -ERESTARTSYS;
			break;
		}
		if (!pipe->writers)
			break;
<<<<<<< HEAD
		if (!pipe->waiting_writers) {
			if (flags & SPLICE_F_NONBLOCK) {
				ret = -EAGAIN;
				break;
			}
		}
		pipe_wait(pipe);
=======
		if (flags & SPLICE_F_NONBLOCK) {
			ret = -EAGAIN;
			break;
		}
		pipe_wait_readable(pipe);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	pipe_unlock(pipe);
	return ret;
}

/*
 * Make sure there's writeable room. Wait for room if we can, otherwise
 * return an appropriate error.
 */
static int opipe_prep(struct pipe_inode_info *pipe, unsigned int flags)
{
	int ret;

	/*
<<<<<<< HEAD
	 * Check ->nrbufs without the inode lock first. This function
	 * is speculative anyways, so missing one is ok.
	 */
	if (pipe->nrbufs < pipe->buffers)
=======
	 * Check pipe occupancy without the inode lock first. This function
	 * is speculative anyways, so missing one is ok.
	 */
	if (!pipe_full(pipe->head, pipe->tail, pipe->max_usage))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;

	ret = 0;
	pipe_lock(pipe);

<<<<<<< HEAD
	while (pipe->nrbufs >= pipe->buffers) {
=======
	while (pipe_full(pipe->head, pipe->tail, pipe->max_usage)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!pipe->readers) {
			send_sig(SIGPIPE, current, 0);
			ret = -EPIPE;
			break;
		}
		if (flags & SPLICE_F_NONBLOCK) {
			ret = -EAGAIN;
			break;
		}
		if (signal_pending(current)) {
			ret = -ERESTARTSYS;
			break;
		}
<<<<<<< HEAD
		pipe->waiting_writers++;
		pipe_wait(pipe);
		pipe->waiting_writers--;
=======
		pipe_wait_writable(pipe);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	pipe_unlock(pipe);
	return ret;
}

/*
 * Splice contents of ipipe to opipe.
 */
static int splice_pipe_to_pipe(struct pipe_inode_info *ipipe,
			       struct pipe_inode_info *opipe,
			       size_t len, unsigned int flags)
{
	struct pipe_buffer *ibuf, *obuf;
<<<<<<< HEAD
	int ret = 0, nbuf;
=======
	unsigned int i_head, o_head;
	unsigned int i_tail, o_tail;
	unsigned int i_mask, o_mask;
	int ret = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	bool input_wakeup = false;


retry:
	ret = ipipe_prep(ipipe, flags);
	if (ret)
		return ret;

	ret = opipe_prep(opipe, flags);
	if (ret)
		return ret;

	/*
	 * Potential ABBA deadlock, work around it by ordering lock
	 * grabbing by pipe info address. Otherwise two different processes
	 * could deadlock (one doing tee from A -> B, the other from B -> A).
	 */
	pipe_double_lock(ipipe, opipe);

<<<<<<< HEAD
	do {
=======
	i_tail = ipipe->tail;
	i_mask = ipipe->ring_size - 1;
	o_head = opipe->head;
	o_mask = opipe->ring_size - 1;

	do {
		size_t o_len;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!opipe->readers) {
			send_sig(SIGPIPE, current, 0);
			if (!ret)
				ret = -EPIPE;
			break;
		}

<<<<<<< HEAD
		if (!ipipe->nrbufs && !ipipe->writers)
=======
		i_head = ipipe->head;
		o_tail = opipe->tail;

		if (pipe_empty(i_head, i_tail) && !ipipe->writers)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;

		/*
		 * Cannot make any progress, because either the input
		 * pipe is empty or the output pipe is full.
		 */
<<<<<<< HEAD
		if (!ipipe->nrbufs || opipe->nrbufs >= opipe->buffers) {
=======
		if (pipe_empty(i_head, i_tail) ||
		    pipe_full(o_head, o_tail, opipe->max_usage)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/* Already processed some buffers, break */
			if (ret)
				break;

			if (flags & SPLICE_F_NONBLOCK) {
				ret = -EAGAIN;
				break;
			}

			/*
			 * We raced with another reader/writer and haven't
			 * managed to process any buffers.  A zero return
			 * value means EOF, so retry instead.
			 */
			pipe_unlock(ipipe);
			pipe_unlock(opipe);
			goto retry;
		}

<<<<<<< HEAD
		ibuf = ipipe->bufs + ipipe->curbuf;
		nbuf = (opipe->curbuf + opipe->nrbufs) & (opipe->buffers - 1);
		obuf = opipe->bufs + nbuf;
=======
		ibuf = &ipipe->bufs[i_tail & i_mask];
		obuf = &opipe->bufs[o_head & o_mask];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (len >= ibuf->len) {
			/*
			 * Simply move the whole buffer from ipipe to opipe
			 */
			*obuf = *ibuf;
			ibuf->ops = NULL;
<<<<<<< HEAD
			opipe->nrbufs++;
			ipipe->curbuf = (ipipe->curbuf + 1) & (ipipe->buffers - 1);
			ipipe->nrbufs--;
			input_wakeup = true;
=======
			i_tail++;
			ipipe->tail = i_tail;
			input_wakeup = true;
			o_len = obuf->len;
			o_head++;
			opipe->head = o_head;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		} else {
			/*
			 * Get a reference to this pipe buffer,
			 * so we can copy the contents over.
			 */
<<<<<<< HEAD
			ibuf->ops->get(ipipe, ibuf);
			*obuf = *ibuf;

			/*
			 * Don't inherit the gift flag, we need to
			 * prevent multiple steals of this page.
			 */
			obuf->flags &= ~PIPE_BUF_FLAG_GIFT;

			obuf->len = len;
			opipe->nrbufs++;
			ibuf->offset += obuf->len;
			ibuf->len -= obuf->len;
		}
		ret += obuf->len;
		len -= obuf->len;
=======
			if (!pipe_buf_get(ipipe, ibuf)) {
				if (ret == 0)
					ret = -EFAULT;
				break;
			}
			*obuf = *ibuf;

			/*
			 * Don't inherit the gift and merge flags, we need to
			 * prevent multiple steals of this page.
			 */
			obuf->flags &= ~PIPE_BUF_FLAG_GIFT;
			obuf->flags &= ~PIPE_BUF_FLAG_CAN_MERGE;

			obuf->len = len;
			ibuf->offset += len;
			ibuf->len -= len;
			o_len = len;
			o_head++;
			opipe->head = o_head;
		}
		ret += o_len;
		len -= o_len;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} while (len);

	pipe_unlock(ipipe);
	pipe_unlock(opipe);

	/*
	 * If we put data in the output pipe, wakeup any potential readers.
	 */
	if (ret > 0)
		wakeup_pipe_readers(opipe);

	if (input_wakeup)
		wakeup_pipe_writers(ipipe);

	return ret;
}

/*
 * Link contents of ipipe to opipe.
 */
<<<<<<< HEAD
static int link_pipe(struct pipe_inode_info *ipipe,
		     struct pipe_inode_info *opipe,
		     size_t len, unsigned int flags)
{
	struct pipe_buffer *ibuf, *obuf;
	int ret = 0, i = 0, nbuf;
=======
static ssize_t link_pipe(struct pipe_inode_info *ipipe,
			 struct pipe_inode_info *opipe,
			 size_t len, unsigned int flags)
{
	struct pipe_buffer *ibuf, *obuf;
	unsigned int i_head, o_head;
	unsigned int i_tail, o_tail;
	unsigned int i_mask, o_mask;
	ssize_t ret = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Potential ABBA deadlock, work around it by ordering lock
	 * grabbing by pipe info address. Otherwise two different processes
	 * could deadlock (one doing tee from A -> B, the other from B -> A).
	 */
	pipe_double_lock(ipipe, opipe);

<<<<<<< HEAD
=======
	i_tail = ipipe->tail;
	i_mask = ipipe->ring_size - 1;
	o_head = opipe->head;
	o_mask = opipe->ring_size - 1;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	do {
		if (!opipe->readers) {
			send_sig(SIGPIPE, current, 0);
			if (!ret)
				ret = -EPIPE;
			break;
		}

<<<<<<< HEAD
		/*
		 * If we have iterated all input buffers or ran out of
		 * output room, break.
		 */
		if (i >= ipipe->nrbufs || opipe->nrbufs >= opipe->buffers)
			break;

		ibuf = ipipe->bufs + ((ipipe->curbuf + i) & (ipipe->buffers-1));
		nbuf = (opipe->curbuf + opipe->nrbufs) & (opipe->buffers - 1);
=======
		i_head = ipipe->head;
		o_tail = opipe->tail;

		/*
		 * If we have iterated all input buffers or run out of
		 * output room, break.
		 */
		if (pipe_empty(i_head, i_tail) ||
		    pipe_full(o_head, o_tail, opipe->max_usage))
			break;

		ibuf = &ipipe->bufs[i_tail & i_mask];
		obuf = &opipe->bufs[o_head & o_mask];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/*
		 * Get a reference to this pipe buffer,
		 * so we can copy the contents over.
		 */
<<<<<<< HEAD
		ibuf->ops->get(ipipe, ibuf);

		obuf = opipe->bufs + nbuf;
		*obuf = *ibuf;

		/*
		 * Don't inherit the gift flag, we need to
		 * prevent multiple steals of this page.
		 */
		obuf->flags &= ~PIPE_BUF_FLAG_GIFT;

		if (obuf->len > len)
			obuf->len = len;

		opipe->nrbufs++;
		ret += obuf->len;
		len -= obuf->len;
		i++;
	} while (len);

	/*
	 * return EAGAIN if we have the potential of some data in the
	 * future, otherwise just return 0
	 */
	if (!ret && ipipe->waiting_writers && (flags & SPLICE_F_NONBLOCK))
		ret = -EAGAIN;
=======
		if (!pipe_buf_get(ipipe, ibuf)) {
			if (ret == 0)
				ret = -EFAULT;
			break;
		}

		*obuf = *ibuf;

		/*
		 * Don't inherit the gift and merge flag, we need to prevent
		 * multiple steals of this page.
		 */
		obuf->flags &= ~PIPE_BUF_FLAG_GIFT;
		obuf->flags &= ~PIPE_BUF_FLAG_CAN_MERGE;

		if (obuf->len > len)
			obuf->len = len;
		ret += obuf->len;
		len -= obuf->len;

		o_head++;
		opipe->head = o_head;
		i_tail++;
	} while (len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	pipe_unlock(ipipe);
	pipe_unlock(opipe);

	/*
	 * If we put data in the output pipe, wakeup any potential readers.
	 */
	if (ret > 0)
		wakeup_pipe_readers(opipe);

	return ret;
}

/*
 * This is a tee(1) implementation that works on pipes. It doesn't copy
 * any data, it simply references the 'in' pages on the 'out' pipe.
 * The 'flags' used are the SPLICE_F_* variants, currently the only
 * applicable one is SPLICE_F_NONBLOCK.
 */
<<<<<<< HEAD
static long do_tee(struct file *in, struct file *out, size_t len,
		   unsigned int flags)
{
	struct pipe_inode_info *ipipe = get_pipe_info(in);
	struct pipe_inode_info *opipe = get_pipe_info(out);
	int ret = -EINVAL;
=======
ssize_t do_tee(struct file *in, struct file *out, size_t len,
	       unsigned int flags)
{
	struct pipe_inode_info *ipipe = get_pipe_info(in, true);
	struct pipe_inode_info *opipe = get_pipe_info(out, true);
	ssize_t ret = -EINVAL;

	if (unlikely(!(in->f_mode & FMODE_READ) ||
		     !(out->f_mode & FMODE_WRITE)))
		return -EBADF;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Duplicate the contents of ipipe to opipe without actually
	 * copying the data.
	 */
	if (ipipe && opipe && ipipe != opipe) {
<<<<<<< HEAD
=======
		if ((in->f_flags | out->f_flags) & O_NONBLOCK)
			flags |= SPLICE_F_NONBLOCK;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * Keep going, unless we encounter an error. The ipipe/opipe
		 * ordering doesn't really matter.
		 */
		ret = ipipe_prep(ipipe, flags);
		if (!ret) {
			ret = opipe_prep(opipe, flags);
			if (!ret)
				ret = link_pipe(ipipe, opipe, len, flags);
		}
	}

<<<<<<< HEAD
=======
	if (ret > 0) {
		fsnotify_access(in);
		fsnotify_modify(out);
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

SYSCALL_DEFINE4(tee, int, fdin, int, fdout, size_t, len, unsigned int, flags)
{
<<<<<<< HEAD
	struct file *in;
	int error, fput_in;
=======
	struct fd in, out;
	ssize_t error;

	if (unlikely(flags & ~SPLICE_F_ALL))
		return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (unlikely(!len))
		return 0;

	error = -EBADF;
<<<<<<< HEAD
	in = fget_light(fdin, &fput_in);
	if (in) {
		if (in->f_mode & FMODE_READ) {
			int fput_out;
			struct file *out = fget_light(fdout, &fput_out);

			if (out) {
				if (out->f_mode & FMODE_WRITE)
					error = do_tee(in, out, len, flags);
				fput_light(out, fput_out);
			}
		}
 		fput_light(in, fput_in);
=======
	in = fdget(fdin);
	if (in.file) {
		out = fdget(fdout);
		if (out.file) {
			error = do_tee(in.file, out.file, len, flags);
			fdput(out);
		}
 		fdput(in);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 	}

	return error;
}
