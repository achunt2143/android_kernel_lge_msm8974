<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * mm/readahead.c - address_space-level file readahead.
 *
 * Copyright (C) 2002, Linus Torvalds
 *
 * 09Apr2002	Andrew Morton
 *		Initial version.
 */

<<<<<<< HEAD
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/gfp.h>
#include <linux/mm.h>
#include <linux/export.h>
#include <linux/blkdev.h>
#include <linux/backing-dev.h>
#include <linux/task_io_accounting_ops.h>
#include <linux/pagevec.h>
#include <linux/pagemap.h>
=======
/**
 * DOC: Readahead Overview
 *
 * Readahead is used to read content into the page cache before it is
 * explicitly requested by the application.  Readahead only ever
 * attempts to read folios that are not yet in the page cache.  If a
 * folio is present but not up-to-date, readahead will not try to read
 * it. In that case a simple ->read_folio() will be requested.
 *
 * Readahead is triggered when an application read request (whether a
 * system call or a page fault) finds that the requested folio is not in
 * the page cache, or that it is in the page cache and has the
 * readahead flag set.  This flag indicates that the folio was read
 * as part of a previous readahead request and now that it has been
 * accessed, it is time for the next readahead.
 *
 * Each readahead request is partly synchronous read, and partly async
 * readahead.  This is reflected in the struct file_ra_state which
 * contains ->size being the total number of pages, and ->async_size
 * which is the number of pages in the async section.  The readahead
 * flag will be set on the first folio in this async section to trigger
 * a subsequent readahead.  Once a series of sequential reads has been
 * established, there should be no need for a synchronous component and
 * all readahead request will be fully asynchronous.
 *
 * When either of the triggers causes a readahead, three numbers need
 * to be determined: the start of the region to read, the size of the
 * region, and the size of the async tail.
 *
 * The start of the region is simply the first page address at or after
 * the accessed address, which is not currently populated in the page
 * cache.  This is found with a simple search in the page cache.
 *
 * The size of the async tail is determined by subtracting the size that
 * was explicitly requested from the determined request size, unless
 * this would be less than zero - then zero is used.  NOTE THIS
 * CALCULATION IS WRONG WHEN THE START OF THE REGION IS NOT THE ACCESSED
 * PAGE.  ALSO THIS CALCULATION IS NOT USED CONSISTENTLY.
 *
 * The size of the region is normally determined from the size of the
 * previous readahead which loaded the preceding pages.  This may be
 * discovered from the struct file_ra_state for simple sequential reads,
 * or from examining the state of the page cache when multiple
 * sequential reads are interleaved.  Specifically: where the readahead
 * was triggered by the readahead flag, the size of the previous
 * readahead is assumed to be the number of pages from the triggering
 * page to the start of the new readahead.  In these cases, the size of
 * the previous readahead is scaled, often doubled, for the new
 * readahead, though see get_next_ra_size() for details.
 *
 * If the size of the previous read cannot be determined, the number of
 * preceding pages in the page cache is used to estimate the size of
 * a previous read.  This estimate could easily be misled by random
 * reads being coincidentally adjacent, so it is ignored unless it is
 * larger than the current request, and it is not scaled up, unless it
 * is at the start of file.
 *
 * In general readahead is accelerated at the start of the file, as
 * reads from there are often sequential.  There are other minor
 * adjustments to the readahead size in various special cases and these
 * are best discovered by reading the code.
 *
 * The above calculation, based on the previous readahead size,
 * determines the size of the readahead, to which any requested read
 * size may be added.
 *
 * Readahead requests are sent to the filesystem using the ->readahead()
 * address space operation, for which mpage_readahead() is a canonical
 * implementation.  ->readahead() should normally initiate reads on all
 * folios, but may fail to read any or all folios without causing an I/O
 * error.  The page cache reading code will issue a ->read_folio() request
 * for any folio which ->readahead() did not read, and only an error
 * from this will be final.
 *
 * ->readahead() will generally call readahead_folio() repeatedly to get
 * each folio from those prepared for readahead.  It may fail to read a
 * folio by:
 *
 * * not calling readahead_folio() sufficiently many times, effectively
 *   ignoring some folios, as might be appropriate if the path to
 *   storage is congested.
 *
 * * failing to actually submit a read request for a given folio,
 *   possibly due to insufficient resources, or
 *
 * * getting an error during subsequent processing of a request.
 *
 * In the last two cases, the folio should be unlocked by the filesystem
 * to indicate that the read attempt has failed.  In the first case the
 * folio will be unlocked by the VFS.
 *
 * Those folios not in the final ``async_size`` of the request should be
 * considered to be important and ->readahead() should not fail them due
 * to congestion or temporary resource unavailability, but should wait
 * for necessary resources (e.g.  memory or indexing information) to
 * become available.  Folios in the final ``async_size`` may be
 * considered less urgent and failure to read them is more acceptable.
 * In this case it is best to use filemap_remove_folio() to remove the
 * folios from the page cache as is automatically done for folios that
 * were not fetched with readahead_folio().  This will allow a
 * subsequent synchronous readahead request to try them again.  If they
 * are left in the page cache, then they will be read individually using
 * ->read_folio() which may be less efficient.
 */

#include <linux/blkdev.h>
#include <linux/kernel.h>
#include <linux/dax.h>
#include <linux/gfp.h>
#include <linux/export.h>
#include <linux/backing-dev.h>
#include <linux/task_io_accounting_ops.h>
#include <linux/pagemap.h>
#include <linux/psi.h>
#include <linux/syscalls.h>
#include <linux/file.h>
#include <linux/mm_inline.h>
#include <linux/blk-cgroup.h>
#include <linux/fadvise.h>
#include <linux/sched/mm.h>

#include "internal.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Initialise a struct file's readahead state.  Assumes that the caller has
 * memset *ra to zero.
 */
void
file_ra_state_init(struct file_ra_state *ra, struct address_space *mapping)
{
<<<<<<< HEAD
	ra->ra_pages = mapping->backing_dev_info->ra_pages;
=======
	ra->ra_pages = inode_to_bdi(mapping->host)->ra_pages;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ra->prev_pos = -1;
}
EXPORT_SYMBOL_GPL(file_ra_state_init);

<<<<<<< HEAD
#define list_to_page(head) (list_entry((head)->prev, struct page, lru))

/*
 * see if a page needs releasing upon read_cache_pages() failure
 * - the caller of read_cache_pages() may have set PG_private or PG_fscache
 *   before calling, such as the NFS fs marking pages that are cached locally
 *   on disk, thus we need to give the fs a chance to clean up in the event of
 *   an error
 */
static void read_cache_pages_invalidate_page(struct address_space *mapping,
					     struct page *page)
{
	if (page_has_private(page)) {
		if (!trylock_page(page))
			BUG();
		page->mapping = mapping;
		do_invalidatepage(page, 0);
		page->mapping = NULL;
		unlock_page(page);
	}
	page_cache_release(page);
}

/*
 * release a list of pages, invalidating them first if need be
 */
static void read_cache_pages_invalidate_pages(struct address_space *mapping,
					      struct list_head *pages)
{
	struct page *victim;

	while (!list_empty(pages)) {
		victim = list_to_page(pages);
		list_del(&victim->lru);
		read_cache_pages_invalidate_page(mapping, victim);
	}
}

/**
 * read_cache_pages - populate an address space with some pages & start reads against them
 * @mapping: the address_space
 * @pages: The address of a list_head which contains the target pages.  These
 *   pages have their ->index populated and are otherwise uninitialised.
 * @filler: callback routine for filling a single page.
 * @data: private data for the callback routine.
 *
 * Hides the details of the LRU cache etc from the filesystems.
 */
int read_cache_pages(struct address_space *mapping, struct list_head *pages,
			int (*filler)(void *, struct page *), void *data)
{
	struct page *page;
	int ret = 0;

	while (!list_empty(pages)) {
		page = list_to_page(pages);
		list_del(&page->lru);
		if (add_to_page_cache_lru(page, mapping,
					page->index, GFP_KERNEL)) {
			read_cache_pages_invalidate_page(mapping, page);
			continue;
		}
		page_cache_release(page);

		ret = filler(data, page);
		if (unlikely(ret)) {
			read_cache_pages_invalidate_pages(mapping, pages);
			break;
		}
		task_io_account_read(PAGE_CACHE_SIZE);
	}
	return ret;
}

EXPORT_SYMBOL(read_cache_pages);

static int read_pages(struct address_space *mapping, struct file *filp,
		struct list_head *pages, unsigned nr_pages)
{
	struct blk_plug plug;
	unsigned page_idx;
	int ret;

	blk_start_plug(&plug);

	if (mapping->a_ops->readpages) {
		ret = mapping->a_ops->readpages(filp, mapping, pages, nr_pages);
		/* Clean up the remaining pages */
		put_pages_list(pages);
		goto out;
	}

	for (page_idx = 0; page_idx < nr_pages; page_idx++) {
		struct page *page = list_to_page(pages);
		list_del(&page->lru);
		if (!add_to_page_cache_lru(page, mapping,
					page->index, GFP_KERNEL)) {
			mapping->a_ops->readpage(filp, page);
		}
		page_cache_release(page);
	}
	ret = 0;

out:
	blk_finish_plug(&plug);

	return ret;
}

/*
 * __do_page_cache_readahead() actually reads a chunk of disk.  It allocates all
 * the pages first, then submits them all for I/O. This avoids the very bad
 * behaviour which would occur if page allocations are causing VM writeback.
 * We really don't want to intermingle reads and writes like that.
 *
 * Returns the number of pages requested, or the maximum amount of I/O allowed.
 */
static int
__do_page_cache_readahead(struct address_space *mapping, struct file *filp,
			pgoff_t offset, unsigned long nr_to_read,
			unsigned long lookahead_size)
{
	struct inode *inode = mapping->host;
	struct page *page;
	unsigned long end_index;	/* The last page we want to read */
	LIST_HEAD(page_pool);
	int page_idx;
	int ret = 0;
	loff_t isize = i_size_read(inode);

	if (isize == 0)
		goto out;

	end_index = ((isize - 1) >> PAGE_CACHE_SHIFT);

	/*
	 * Preallocate as many pages as we will need.
	 */
	for (page_idx = 0; page_idx < nr_to_read; page_idx++) {
		pgoff_t page_offset = offset + page_idx;

		if (page_offset > end_index)
			break;

		rcu_read_lock();
		page = radix_tree_lookup(&mapping->page_tree, page_offset);
		rcu_read_unlock();
		if (page)
			continue;

		page = page_cache_alloc_readahead(mapping);
		if (!page)
			break;
		page->index = page_offset;

		page->flags |= (1L << PG_readahead);

		list_add(&page->lru, &page_pool);
		if (page_idx == nr_to_read - lookahead_size)
			SetPageReadahead(page);
		ret++;
	}

	/*
	 * Now start the IO.  We ignore I/O errors - if the page is not
	 * uptodate then the caller will launch readpage again, and
	 * will then handle the error.
	 */
	if (ret)
		read_pages(mapping, filp, &page_pool, ret);
	BUG_ON(!list_empty(&page_pool));
out:
	return ret;
=======
static void read_pages(struct readahead_control *rac)
{
	const struct address_space_operations *aops = rac->mapping->a_ops;
	struct folio *folio;
	struct blk_plug plug;

	if (!readahead_count(rac))
		return;

	if (unlikely(rac->_workingset))
		psi_memstall_enter(&rac->_pflags);
	blk_start_plug(&plug);

	if (aops->readahead) {
		aops->readahead(rac);
		/*
		 * Clean up the remaining folios.  The sizes in ->ra
		 * may be used to size the next readahead, so make sure
		 * they accurately reflect what happened.
		 */
		while ((folio = readahead_folio(rac)) != NULL) {
			unsigned long nr = folio_nr_pages(folio);

			folio_get(folio);
			rac->ra->size -= nr;
			if (rac->ra->async_size >= nr) {
				rac->ra->async_size -= nr;
				filemap_remove_folio(folio);
			}
			folio_unlock(folio);
			folio_put(folio);
		}
	} else {
		while ((folio = readahead_folio(rac)) != NULL)
			aops->read_folio(rac->file, folio);
	}

	blk_finish_plug(&plug);
	if (unlikely(rac->_workingset))
		psi_memstall_leave(&rac->_pflags);
	rac->_workingset = false;

	BUG_ON(readahead_count(rac));
}

/**
 * page_cache_ra_unbounded - Start unchecked readahead.
 * @ractl: Readahead control.
 * @nr_to_read: The number of pages to read.
 * @lookahead_size: Where to start the next readahead.
 *
 * This function is for filesystems to call when they want to start
 * readahead beyond a file's stated i_size.  This is almost certainly
 * not the function you want to call.  Use page_cache_async_readahead()
 * or page_cache_sync_readahead() instead.
 *
 * Context: File is referenced by caller.  Mutexes may be held by caller.
 * May sleep, but will not reenter filesystem to reclaim memory.
 */
void page_cache_ra_unbounded(struct readahead_control *ractl,
		unsigned long nr_to_read, unsigned long lookahead_size)
{
	struct address_space *mapping = ractl->mapping;
	unsigned long index = readahead_index(ractl);
	gfp_t gfp_mask = readahead_gfp_mask(mapping);
	unsigned long i;

	/*
	 * Partway through the readahead operation, we will have added
	 * locked pages to the page cache, but will not yet have submitted
	 * them for I/O.  Adding another page may need to allocate memory,
	 * which can trigger memory reclaim.  Telling the VM we're in
	 * the middle of a filesystem operation will cause it to not
	 * touch file-backed pages, preventing a deadlock.  Most (all?)
	 * filesystems already specify __GFP_NOFS in their mapping's
	 * gfp_mask, but let's be explicit here.
	 */
	unsigned int nofs = memalloc_nofs_save();

	filemap_invalidate_lock_shared(mapping);
	/*
	 * Preallocate as many pages as we will need.
	 */
	for (i = 0; i < nr_to_read; i++) {
		struct folio *folio = xa_load(&mapping->i_pages, index + i);

		if (folio && !xa_is_value(folio)) {
			/*
			 * Page already present?  Kick off the current batch
			 * of contiguous pages before continuing with the
			 * next batch.  This page may be the one we would
			 * have intended to mark as Readahead, but we don't
			 * have a stable reference to this page, and it's
			 * not worth getting one just for that.
			 */
			read_pages(ractl);
			ractl->_index++;
			i = ractl->_index + ractl->_nr_pages - index - 1;
			continue;
		}

		folio = filemap_alloc_folio(gfp_mask, 0);
		if (!folio)
			break;
		if (filemap_add_folio(mapping, folio, index + i,
					gfp_mask) < 0) {
			folio_put(folio);
			read_pages(ractl);
			ractl->_index++;
			i = ractl->_index + ractl->_nr_pages - index - 1;
			continue;
		}
		if (i == nr_to_read - lookahead_size)
			folio_set_readahead(folio);
		ractl->_workingset |= folio_test_workingset(folio);
		ractl->_nr_pages++;
	}

	/*
	 * Now start the IO.  We ignore I/O errors - if the folio is not
	 * uptodate then the caller will launch read_folio again, and
	 * will then handle the error.
	 */
	read_pages(ractl);
	filemap_invalidate_unlock_shared(mapping);
	memalloc_nofs_restore(nofs);
}
EXPORT_SYMBOL_GPL(page_cache_ra_unbounded);

/*
 * do_page_cache_ra() actually reads a chunk of disk.  It allocates
 * the pages first, then submits them for I/O. This avoids the very bad
 * behaviour which would occur if page allocations are causing VM writeback.
 * We really don't want to intermingle reads and writes like that.
 */
static void do_page_cache_ra(struct readahead_control *ractl,
		unsigned long nr_to_read, unsigned long lookahead_size)
{
	struct inode *inode = ractl->mapping->host;
	unsigned long index = readahead_index(ractl);
	loff_t isize = i_size_read(inode);
	pgoff_t end_index;	/* The last page we want to read */

	if (isize == 0)
		return;

	end_index = (isize - 1) >> PAGE_SHIFT;
	if (index > end_index)
		return;
	/* Don't read past the page containing the last byte of the file */
	if (nr_to_read > end_index - index)
		nr_to_read = end_index - index + 1;

	page_cache_ra_unbounded(ractl, nr_to_read, lookahead_size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Chunk the readahead into 2 megabyte units, so that we don't pin too much
 * memory at once.
 */
<<<<<<< HEAD
int force_page_cache_readahead(struct address_space *mapping, struct file *filp,
		pgoff_t offset, unsigned long nr_to_read)
{
	int ret = 0;

	if (unlikely(!mapping->a_ops->readpage && !mapping->a_ops->readpages))
		return -EINVAL;

	nr_to_read = max_sane_readahead(nr_to_read);
	while (nr_to_read) {
		int err;

		unsigned long this_chunk = (2 * 1024 * 1024) / PAGE_CACHE_SIZE;

		if (this_chunk > nr_to_read)
			this_chunk = nr_to_read;
		err = __do_page_cache_readahead(mapping, filp,
						offset, this_chunk, 0);
		if (err < 0) {
			ret = err;
			break;
		}
		ret += err;
		offset += this_chunk;
		nr_to_read -= this_chunk;
	}
	return ret;
}

/*
 * Given a desired number of PAGE_CACHE_SIZE readahead pages, return a
 * sensible upper limit.
 */
unsigned long max_sane_readahead(unsigned long nr)
{
	return min(nr, (node_page_state(numa_node_id(), NR_INACTIVE_FILE)
		+ node_page_state(numa_node_id(), NR_FREE_PAGES)) / 2);
}

/*
 * Submit IO for the read-ahead request in file_ra_state.
 */
unsigned long ra_submit(struct file_ra_state *ra,
		       struct address_space *mapping, struct file *filp)
{
	int actual;

	actual = __do_page_cache_readahead(mapping, filp,
					ra->start, ra->size, ra->async_size);

	return actual;
=======
void force_page_cache_ra(struct readahead_control *ractl,
		unsigned long nr_to_read)
{
	struct address_space *mapping = ractl->mapping;
	struct file_ra_state *ra = ractl->ra;
	struct backing_dev_info *bdi = inode_to_bdi(mapping->host);
	unsigned long max_pages, index;

	if (unlikely(!mapping->a_ops->read_folio && !mapping->a_ops->readahead))
		return;

	/*
	 * If the request exceeds the readahead window, allow the read to
	 * be up to the optimal hardware IO size
	 */
	index = readahead_index(ractl);
	max_pages = max_t(unsigned long, bdi->io_pages, ra->ra_pages);
	nr_to_read = min_t(unsigned long, nr_to_read, max_pages);
	while (nr_to_read) {
		unsigned long this_chunk = (2 * 1024 * 1024) / PAGE_SIZE;

		if (this_chunk > nr_to_read)
			this_chunk = nr_to_read;
		ractl->_index = index;
		do_page_cache_ra(ractl, this_chunk, 0);

		index += this_chunk;
		nr_to_read -= this_chunk;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Set the initial window size, round to next power of 2 and square
<<<<<<< HEAD
 * Small size is not dependant on max value - only a one-page read is regarded
 * as small.
 * for small size, x 4 for medium, and x 2 for large
 * for 128k (32 page) max ra
 * 1-8 page = 32k initial, > 8 page = 128k initial
=======
 * for small size, x 4 for medium, and x 2 for large
 * for 128k (32 page) max ra
 * 1-2 page = 16k, 3-4 page 32k, 5-8 page = 64k, > 8 page = 128k initial
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
static unsigned long get_init_ra_size(unsigned long size, unsigned long max)
{
	unsigned long newsize = roundup_pow_of_two(size);

<<<<<<< HEAD
	if (newsize <= 1)
=======
	if (newsize <= max / 32)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		newsize = newsize * 4;
	else if (newsize <= max / 4)
		newsize = newsize * 2;
	else
		newsize = max;

	return newsize;
}

/*
 *  Get the previous window size, ramp it up, and
 *  return it as the new window size.
 */
static unsigned long get_next_ra_size(struct file_ra_state *ra,
<<<<<<< HEAD
						unsigned long max)
{
	unsigned long cur = ra->size;
	unsigned long newsize;

	if (cur < max / 16)
		newsize = 4 * cur;
	else
		newsize = 2 * cur;

	return min(newsize, max);
=======
				      unsigned long max)
{
	unsigned long cur = ra->size;

	if (cur < max / 16)
		return 4 * cur;
	if (cur <= max / 2)
		return 2 * cur;
	return max;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * On-demand readahead design.
 *
 * The fields in struct file_ra_state represent the most-recently-executed
 * readahead attempt:
 *
 *                        |<----- async_size ---------|
 *     |------------------- size -------------------->|
 *     |==================#===========================|
 *     ^start             ^page marked with PG_readahead
 *
 * To overlap application thinking time and disk I/O time, we do
 * `readahead pipelining': Do not wait until the application consumed all
 * readahead pages and stalled on the missing page at readahead_index;
 * Instead, submit an asynchronous readahead I/O as soon as there are
 * only async_size pages left in the readahead window. Normally async_size
 * will be equal to size, for maximum pipelining.
 *
 * In interleaved sequential reads, concurrent streams on the same fd can
 * be invalidating each other's readahead state. So we flag the new readahead
 * page at (start+size-async_size) with PG_readahead, and use it as readahead
 * indicator. The flag won't be set on already cached pages, to avoid the
 * readahead-for-nothing fuss, saving pointless page cache lookups.
 *
 * prev_pos tracks the last visited byte in the _previous_ read request.
 * It should be maintained by the caller, and will be used for detecting
 * small random reads. Note that the readahead algorithm checks loosely
 * for sequential patterns. Hence interleaved reads might be served as
 * sequential ones.
 *
 * There is a special-case: if the first page which the application tries to
 * read happens to be the first page of the file, it is assumed that a linear
 * read is about to happen and the window is immediately set to the initial size
 * based on I/O request size and the max_readahead.
 *
 * The code ramps up the readahead size aggressively at first, but slow down as
 * it approaches max_readhead.
 */

/*
<<<<<<< HEAD
 * Count contiguously cached pages from @offset-1 to @offset-@max,
=======
 * Count contiguously cached pages from @index-1 to @index-@max,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * this count is a conservative estimation of
 * 	- length of the sequential read sequence, or
 * 	- thrashing threshold in memory tight systems
 */
static pgoff_t count_history_pages(struct address_space *mapping,
<<<<<<< HEAD
				   struct file_ra_state *ra,
				   pgoff_t offset, unsigned long max)
=======
				   pgoff_t index, unsigned long max)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	pgoff_t head;

	rcu_read_lock();
<<<<<<< HEAD
	head = radix_tree_prev_hole(&mapping->page_tree, offset - 1, max);
	rcu_read_unlock();

	return offset - 1 - head;
}

/*
 * page cache context based read-ahead
 */
static int try_context_readahead(struct address_space *mapping,
				 struct file_ra_state *ra,
				 pgoff_t offset,
=======
	head = page_cache_prev_miss(mapping, index - 1, max);
	rcu_read_unlock();

	return index - 1 - head;
}

/*
 * page cache context based readahead
 */
static int try_context_readahead(struct address_space *mapping,
				 struct file_ra_state *ra,
				 pgoff_t index,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				 unsigned long req_size,
				 unsigned long max)
{
	pgoff_t size;

<<<<<<< HEAD
	size = count_history_pages(mapping, ra, offset, max);

	/*
	 * no history pages:
	 * it could be a random read
	 */
	if (!size)
=======
	size = count_history_pages(mapping, index, max);

	/*
	 * not enough history pages:
	 * it could be a random read
	 */
	if (size <= req_size)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;

	/*
	 * starts from beginning of file:
	 * it is a strong indication of long-run stream (or whole-file-read)
	 */
<<<<<<< HEAD
	if (size >= offset)
		size *= 2;

	ra->start = offset;
	ra->size = get_init_ra_size(size + req_size, max);
	ra->async_size = ra->size;
=======
	if (size >= index)
		size *= 2;

	ra->start = index;
	ra->size = min(size + req_size, max);
	ra->async_size = 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 1;
}

<<<<<<< HEAD
/*
 * A minimal readahead algorithm for trivial sequential/random reads.
 */
static unsigned long
ondemand_readahead(struct address_space *mapping,
		   struct file_ra_state *ra, struct file *filp,
		   bool hit_readahead_marker, pgoff_t offset,
		   unsigned long req_size)
{
	unsigned long max = max_sane_readahead(ra->ra_pages);
=======
static inline int ra_alloc_folio(struct readahead_control *ractl, pgoff_t index,
		pgoff_t mark, unsigned int order, gfp_t gfp)
{
	int err;
	struct folio *folio = filemap_alloc_folio(gfp, order);

	if (!folio)
		return -ENOMEM;
	mark = round_down(mark, 1UL << order);
	if (index == mark)
		folio_set_readahead(folio);
	err = filemap_add_folio(ractl->mapping, folio, index, gfp);
	if (err) {
		folio_put(folio);
		return err;
	}

	ractl->_nr_pages += 1UL << order;
	ractl->_workingset |= folio_test_workingset(folio);
	return 0;
}

void page_cache_ra_order(struct readahead_control *ractl,
		struct file_ra_state *ra, unsigned int new_order)
{
	struct address_space *mapping = ractl->mapping;
	pgoff_t index = readahead_index(ractl);
	pgoff_t limit = (i_size_read(mapping->host) - 1) >> PAGE_SHIFT;
	pgoff_t mark = index + ra->size - ra->async_size;
	unsigned int nofs;
	int err = 0;
	gfp_t gfp = readahead_gfp_mask(mapping);

	if (!mapping_large_folio_support(mapping) || ra->size < 4)
		goto fallback;

	limit = min(limit, index + ra->size - 1);

	if (new_order < MAX_PAGECACHE_ORDER) {
		new_order += 2;
		new_order = min_t(unsigned int, MAX_PAGECACHE_ORDER, new_order);
		new_order = min_t(unsigned int, new_order, ilog2(ra->size));
	}

	/* See comment in page_cache_ra_unbounded() */
	nofs = memalloc_nofs_save();
	filemap_invalidate_lock_shared(mapping);
	while (index <= limit) {
		unsigned int order = new_order;

		/* Align with smaller pages if needed */
		if (index & ((1UL << order) - 1))
			order = __ffs(index);
		/* Don't allocate pages past EOF */
		while (index + (1UL << order) - 1 > limit)
			order--;
		err = ra_alloc_folio(ractl, index, mark, order, gfp);
		if (err)
			break;
		index += 1UL << order;
	}

	if (index > limit) {
		ra->size += index - limit - 1;
		ra->async_size += index - limit - 1;
	}

	read_pages(ractl);
	filemap_invalidate_unlock_shared(mapping);
	memalloc_nofs_restore(nofs);

	/*
	 * If there were already pages in the page cache, then we may have
	 * left some gaps.  Let the regular readahead code take care of this
	 * situation.
	 */
	if (!err)
		return;
fallback:
	do_page_cache_ra(ractl, ra->size, ra->async_size);
}

/*
 * A minimal readahead algorithm for trivial sequential/random reads.
 */
static void ondemand_readahead(struct readahead_control *ractl,
		struct folio *folio, unsigned long req_size)
{
	struct backing_dev_info *bdi = inode_to_bdi(ractl->mapping->host);
	struct file_ra_state *ra = ractl->ra;
	unsigned long max_pages = ra->ra_pages;
	unsigned long add_pages;
	pgoff_t index = readahead_index(ractl);
	pgoff_t expected, prev_index;
	unsigned int order = folio ? folio_order(folio) : 0;

	/*
	 * If the request exceeds the readahead window, allow the read to
	 * be up to the optimal hardware IO size
	 */
	if (req_size > max_pages && bdi->io_pages > max_pages)
		max_pages = min(req_size, bdi->io_pages);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * start of file
	 */
<<<<<<< HEAD
	if (!offset)
		goto initial_readahead;

	/*
	 * It's the expected callback offset, assume sequential access.
	 * Ramp up sizes, and push forward the readahead window.
	 */
	if ((offset == (ra->start + ra->size - ra->async_size) ||
	     offset == (ra->start + ra->size))) {
		ra->start += ra->size;
		ra->size = get_next_ra_size(ra, max);
=======
	if (!index)
		goto initial_readahead;

	/*
	 * It's the expected callback index, assume sequential access.
	 * Ramp up sizes, and push forward the readahead window.
	 */
	expected = round_down(ra->start + ra->size - ra->async_size,
			1UL << order);
	if (index == expected || index == (ra->start + ra->size)) {
		ra->start += ra->size;
		ra->size = get_next_ra_size(ra, max_pages);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ra->async_size = ra->size;
		goto readit;
	}

	/*
<<<<<<< HEAD
	 * Hit a marked page without valid readahead state.
=======
	 * Hit a marked folio without valid readahead state.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * E.g. interleaved reads.
	 * Query the pagecache for async_size, which normally equals to
	 * readahead size. Ramp it up and use it as the new readahead size.
	 */
<<<<<<< HEAD
	if (hit_readahead_marker) {
		pgoff_t start;

		rcu_read_lock();
		start = radix_tree_next_hole(&mapping->page_tree, offset+1,max);
		rcu_read_unlock();

		if (!start || start - offset > max)
			return 0;

		ra->start = start;
		ra->size = start - offset;	/* old async_size */
		ra->size += req_size;
		ra->size = get_next_ra_size(ra, max);
=======
	if (folio) {
		pgoff_t start;

		rcu_read_lock();
		start = page_cache_next_miss(ractl->mapping, index + 1,
				max_pages);
		rcu_read_unlock();

		if (!start || start - index > max_pages)
			return;

		ra->start = start;
		ra->size = start - index;	/* old async_size */
		ra->size += req_size;
		ra->size = get_next_ra_size(ra, max_pages);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ra->async_size = ra->size;
		goto readit;
	}

	/*
	 * oversize read
	 */
<<<<<<< HEAD
	if (req_size > max)
=======
	if (req_size > max_pages)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto initial_readahead;

	/*
	 * sequential cache miss
<<<<<<< HEAD
	 */
	if (offset - (ra->prev_pos >> PAGE_CACHE_SHIFT) <= 1UL)
=======
	 * trivial case: (index - prev_index) == 1
	 * unaligned reads: (index - prev_index) == 0
	 */
	prev_index = (unsigned long long)ra->prev_pos >> PAGE_SHIFT;
	if (index - prev_index <= 1UL)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto initial_readahead;

	/*
	 * Query the page cache and look for the traces(cached history pages)
	 * that a sequential stream would leave behind.
	 */
<<<<<<< HEAD
	if (try_context_readahead(mapping, ra, offset, req_size, max))
=======
	if (try_context_readahead(ractl->mapping, ra, index, req_size,
			max_pages))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto readit;

	/*
	 * standalone, small random read
	 * Read as is, and do not pollute the readahead state.
	 */
<<<<<<< HEAD
	return __do_page_cache_readahead(mapping, filp, offset, req_size, 0);

initial_readahead:
	ra->start = offset;
	ra->size = get_init_ra_size(req_size, max);
=======
	do_page_cache_ra(ractl, req_size, 0);
	return;

initial_readahead:
	ra->start = index;
	ra->size = get_init_ra_size(req_size, max_pages);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ra->async_size = ra->size > req_size ? ra->size - req_size : ra->size;

readit:
	/*
	 * Will this read hit the readahead marker made by itself?
	 * If so, trigger the readahead marker hit now, and merge
	 * the resulted next readahead window into the current one.
<<<<<<< HEAD
	 */
	if (offset == ra->start && ra->size == ra->async_size) {
		ra->async_size = get_next_ra_size(ra, max);
		ra->size += ra->async_size;
	}

	return ra_submit(ra, mapping, filp);
}

/**
 * page_cache_sync_readahead - generic file readahead
 * @mapping: address_space which holds the pagecache and I/O vectors
 * @ra: file_ra_state which holds the readahead state
 * @filp: passed on to ->readpage() and ->readpages()
 * @offset: start offset into @mapping, in pagecache page-sized units
 * @req_size: hint: total size of the read which the caller is performing in
 *            pagecache pages
 *
 * page_cache_sync_readahead() should be called when a cache miss happened:
 * it will submit the read.  The readahead logic may decide to piggyback more
 * pages onto the read request if access patterns suggest it will improve
 * performance.
 */
void page_cache_sync_readahead(struct address_space *mapping,
			       struct file_ra_state *ra, struct file *filp,
			       pgoff_t offset, unsigned long req_size)
{
	/* no read-ahead */
	if (!ra->ra_pages)
		return;

	/* be dumb */
	if (filp && (filp->f_mode & FMODE_RANDOM)) {
		force_page_cache_readahead(mapping, filp, offset, req_size);
		return;
	}

	/* do read-ahead */
	ondemand_readahead(mapping, ra, filp, false, offset, req_size);
}
EXPORT_SYMBOL_GPL(page_cache_sync_readahead);

/**
 * page_cache_async_readahead - file readahead for marked pages
 * @mapping: address_space which holds the pagecache and I/O vectors
 * @ra: file_ra_state which holds the readahead state
 * @filp: passed on to ->readpage() and ->readpages()
 * @page: the page at @offset which has the PG_readahead flag set
 * @offset: start offset into @mapping, in pagecache page-sized units
 * @req_size: hint: total size of the read which the caller is performing in
 *            pagecache pages
 *
 * page_cache_async_readahead() should be called when a page is used which
 * has the PG_readahead flag; this is a marker to suggest that the application
 * has used up enough of the readahead window that we should start pulling in
 * more pages.
 */
void
page_cache_async_readahead(struct address_space *mapping,
			   struct file_ra_state *ra, struct file *filp,
			   struct page *page, pgoff_t offset,
			   unsigned long req_size)
{
	/* no read-ahead */
	if (!ra->ra_pages)
=======
	 * Take care of maximum IO pages as above.
	 */
	if (index == ra->start && ra->size == ra->async_size) {
		add_pages = get_next_ra_size(ra, max_pages);
		if (ra->size + add_pages <= max_pages) {
			ra->async_size = add_pages;
			ra->size += add_pages;
		} else {
			ra->size = max_pages;
			ra->async_size = max_pages >> 1;
		}
	}

	ractl->_index = ra->start;
	page_cache_ra_order(ractl, ra, order);
}

void page_cache_sync_ra(struct readahead_control *ractl,
		unsigned long req_count)
{
	bool do_forced_ra = ractl->file && (ractl->file->f_mode & FMODE_RANDOM);

	/*
	 * Even if readahead is disabled, issue this request as readahead
	 * as we'll need it to satisfy the requested range. The forced
	 * readahead will do the right thing and limit the read to just the
	 * requested range, which we'll set to 1 page for this case.
	 */
	if (!ractl->ra->ra_pages || blk_cgroup_congested()) {
		if (!ractl->file)
			return;
		req_count = 1;
		do_forced_ra = true;
	}

	/* be dumb */
	if (do_forced_ra) {
		force_page_cache_ra(ractl, req_count);
		return;
	}

	ondemand_readahead(ractl, NULL, req_count);
}
EXPORT_SYMBOL_GPL(page_cache_sync_ra);

void page_cache_async_ra(struct readahead_control *ractl,
		struct folio *folio, unsigned long req_count)
{
	/* no readahead */
	if (!ractl->ra->ra_pages)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;

	/*
	 * Same bit is used for PG_readahead and PG_reclaim.
	 */
<<<<<<< HEAD
	if (PageWriteback(page))
		return;

	ClearPageReadahead(page);

	/*
	 * Defer asynchronous read-ahead on IO congestion.
	 */
	if (bdi_read_congested(mapping->backing_dev_info))
		return;

	/* do read-ahead */
	ondemand_readahead(mapping, ra, filp, true, offset, req_size);
}
EXPORT_SYMBOL_GPL(page_cache_async_readahead);
=======
	if (folio_test_writeback(folio))
		return;

	folio_clear_readahead(folio);

	if (blk_cgroup_congested())
		return;

	ondemand_readahead(ractl, folio, req_count);
}
EXPORT_SYMBOL_GPL(page_cache_async_ra);

ssize_t ksys_readahead(int fd, loff_t offset, size_t count)
{
	ssize_t ret;
	struct fd f;

	ret = -EBADF;
	f = fdget(fd);
	if (!f.file || !(f.file->f_mode & FMODE_READ))
		goto out;

	/*
	 * The readahead() syscall is intended to run only on files
	 * that can execute readahead. If readahead is not possible
	 * on this file, then we must return -EINVAL.
	 */
	ret = -EINVAL;
	if (!f.file->f_mapping || !f.file->f_mapping->a_ops ||
	    (!S_ISREG(file_inode(f.file)->i_mode) &&
	    !S_ISBLK(file_inode(f.file)->i_mode)))
		goto out;

	ret = vfs_fadvise(f.file, offset, count, POSIX_FADV_WILLNEED);
out:
	fdput(f);
	return ret;
}

SYSCALL_DEFINE3(readahead, int, fd, loff_t, offset, size_t, count)
{
	return ksys_readahead(fd, offset, count);
}

#if defined(CONFIG_COMPAT) && defined(__ARCH_WANT_COMPAT_READAHEAD)
COMPAT_SYSCALL_DEFINE4(readahead, int, fd, compat_arg_u64_dual(offset), size_t, count)
{
	return ksys_readahead(fd, compat_arg_u64_glue(offset), count);
}
#endif

/**
 * readahead_expand - Expand a readahead request
 * @ractl: The request to be expanded
 * @new_start: The revised start
 * @new_len: The revised size of the request
 *
 * Attempt to expand a readahead request outwards from the current size to the
 * specified size by inserting locked pages before and after the current window
 * to increase the size to the new window.  This may involve the insertion of
 * THPs, in which case the window may get expanded even beyond what was
 * requested.
 *
 * The algorithm will stop if it encounters a conflicting page already in the
 * pagecache and leave a smaller expansion than requested.
 *
 * The caller must check for this by examining the revised @ractl object for a
 * different expansion than was requested.
 */
void readahead_expand(struct readahead_control *ractl,
		      loff_t new_start, size_t new_len)
{
	struct address_space *mapping = ractl->mapping;
	struct file_ra_state *ra = ractl->ra;
	pgoff_t new_index, new_nr_pages;
	gfp_t gfp_mask = readahead_gfp_mask(mapping);

	new_index = new_start / PAGE_SIZE;

	/* Expand the leading edge downwards */
	while (ractl->_index > new_index) {
		unsigned long index = ractl->_index - 1;
		struct folio *folio = xa_load(&mapping->i_pages, index);

		if (folio && !xa_is_value(folio))
			return; /* Folio apparently present */

		folio = filemap_alloc_folio(gfp_mask, 0);
		if (!folio)
			return;
		if (filemap_add_folio(mapping, folio, index, gfp_mask) < 0) {
			folio_put(folio);
			return;
		}
		if (unlikely(folio_test_workingset(folio)) &&
				!ractl->_workingset) {
			ractl->_workingset = true;
			psi_memstall_enter(&ractl->_pflags);
		}
		ractl->_nr_pages++;
		ractl->_index = folio->index;
	}

	new_len += new_start - readahead_pos(ractl);
	new_nr_pages = DIV_ROUND_UP(new_len, PAGE_SIZE);

	/* Expand the trailing edge upwards */
	while (ractl->_nr_pages < new_nr_pages) {
		unsigned long index = ractl->_index + ractl->_nr_pages;
		struct folio *folio = xa_load(&mapping->i_pages, index);

		if (folio && !xa_is_value(folio))
			return; /* Folio apparently present */

		folio = filemap_alloc_folio(gfp_mask, 0);
		if (!folio)
			return;
		if (filemap_add_folio(mapping, folio, index, gfp_mask) < 0) {
			folio_put(folio);
			return;
		}
		if (unlikely(folio_test_workingset(folio)) &&
				!ractl->_workingset) {
			ractl->_workingset = true;
			psi_memstall_enter(&ractl->_pflags);
		}
		ractl->_nr_pages++;
		if (ra) {
			ra->size++;
			ra->async_size++;
		}
	}
}
EXPORT_SYMBOL(readahead_expand);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
