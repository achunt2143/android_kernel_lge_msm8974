<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *	linux/mm/filemap.c
 *
 * Copyright (C) 1994-1999  Linus Torvalds
 */

/*
 * This file handles the generic file mmap semantics used by
 * most "normal" filesystems (but you don't /have/ to use this:
 * the NFS filesystem used to do this differently, for example)
 */
#include <linux/export.h>
#include <linux/compiler.h>
<<<<<<< HEAD
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/aio.h>
=======
#include <linux/dax.h>
#include <linux/fs.h>
#include <linux/sched/signal.h>
#include <linux/uaccess.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/capability.h>
#include <linux/kernel_stat.h>
#include <linux/gfp.h>
#include <linux/mm.h>
#include <linux/swap.h>
<<<<<<< HEAD
=======
#include <linux/swapops.h>
#include <linux/syscalls.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/mman.h>
#include <linux/pagemap.h>
#include <linux/file.h>
#include <linux/uio.h>
<<<<<<< HEAD
=======
#include <linux/error-injection.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/hash.h>
#include <linux/writeback.h>
#include <linux/backing-dev.h>
#include <linux/pagevec.h>
<<<<<<< HEAD
#include <linux/blkdev.h>
#include <linux/security.h>
#include <linux/syscalls.h>
#include <linux/cpuset.h>
#include <linux/hardirq.h> /* for BUG_ON(!in_atomic()) only */
#include <linux/memcontrol.h>
#include <linux/cleancache.h>
#include "internal.h"
#include "../fs/sreadahead_prof.h"
=======
#include <linux/security.h>
#include <linux/cpuset.h>
#include <linux/hugetlb.h>
#include <linux/memcontrol.h>
#include <linux/shmem_fs.h>
#include <linux/rmap.h>
#include <linux/delayacct.h>
#include <linux/psi.h>
#include <linux/ramfs.h>
#include <linux/page_idle.h>
#include <linux/migrate.h>
#include <linux/pipe_fs_i.h>
#include <linux/splice.h>
#include <linux/rcupdate_wait.h>
#include <asm/pgalloc.h>
#include <asm/tlbflush.h>
#include "internal.h"

#define CREATE_TRACE_POINTS
#include <trace/events/filemap.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * FIXME: remove all knowledge of the buffer layer from the core VM
 */
#include <linux/buffer_head.h> /* for try_to_free_buffers */

#include <asm/mman.h>

<<<<<<< HEAD
=======
#include "swap.h"

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Shared mappings implemented 30.11.1994. It's not fully working yet,
 * though.
 *
 * Shared mappings now work. 15.8.1995  Bruno.
 *
 * finished 'unifying' the page and buffer cache and SMP-threaded the
 * page-cache, 21.05.1999, Ingo Molnar <mingo@redhat.com>
 *
 * SMP-threaded pagemap-LRU 1999, Andrea Arcangeli <andrea@suse.de>
 */

/*
 * Lock ordering:
 *
<<<<<<< HEAD
 *  ->i_mmap_mutex		(truncate_pagecache)
 *    ->private_lock		(__free_pte->__set_page_dirty_buffers)
 *      ->swap_lock		(exclusive_swap_page, others)
 *        ->mapping->tree_lock
 *
 *  ->i_mutex
 *    ->i_mmap_mutex		(truncate->unmap_mapping_range)
 *
 *  ->mmap_sem
 *    ->i_mmap_mutex
 *      ->page_table_lock or pte_lock	(various, mainly in memory.c)
 *        ->mapping->tree_lock	(arch-dependent flush_dcache_mmap_lock)
 *
 *  ->mmap_sem
 *    ->lock_page		(access_process_vm)
 *
 *  ->i_mutex			(generic_file_buffered_write)
 *    ->mmap_sem		(fault_in_pages_readable->do_page_fault)
 *
 *  bdi->wb.list_lock
 *    sb_lock			(fs/fs-writeback.c)
 *    ->mapping->tree_lock	(__sync_single_inode)
 *
 *  ->i_mmap_mutex
 *    ->anon_vma.lock		(vma_adjust)
=======
 *  ->i_mmap_rwsem		(truncate_pagecache)
 *    ->private_lock		(__free_pte->block_dirty_folio)
 *      ->swap_lock		(exclusive_swap_page, others)
 *        ->i_pages lock
 *
 *  ->i_rwsem
 *    ->invalidate_lock		(acquired by fs in truncate path)
 *      ->i_mmap_rwsem		(truncate->unmap_mapping_range)
 *
 *  ->mmap_lock
 *    ->i_mmap_rwsem
 *      ->page_table_lock or pte_lock	(various, mainly in memory.c)
 *        ->i_pages lock	(arch-dependent flush_dcache_mmap_lock)
 *
 *  ->mmap_lock
 *    ->invalidate_lock		(filemap_fault)
 *      ->lock_page		(filemap_fault, access_process_vm)
 *
 *  ->i_rwsem			(generic_perform_write)
 *    ->mmap_lock		(fault_in_readable->do_page_fault)
 *
 *  bdi->wb.list_lock
 *    sb_lock			(fs/fs-writeback.c)
 *    ->i_pages lock		(__sync_single_inode)
 *
 *  ->i_mmap_rwsem
 *    ->anon_vma.lock		(vma_merge)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 *  ->anon_vma.lock
 *    ->page_table_lock or pte_lock	(anon_vma_prepare and various)
 *
 *  ->page_table_lock or pte_lock
 *    ->swap_lock		(try_to_unmap_one)
 *    ->private_lock		(try_to_unmap_one)
<<<<<<< HEAD
 *    ->tree_lock		(try_to_unmap_one)
 *    ->zone.lru_lock		(follow_page->mark_page_accessed)
 *    ->zone.lru_lock		(check_pte_range->isolate_lru_page)
 *    ->private_lock		(page_remove_rmap->set_page_dirty)
 *    ->tree_lock		(page_remove_rmap->set_page_dirty)
 *    bdi.wb->list_lock		(page_remove_rmap->set_page_dirty)
 *    ->inode->i_lock		(page_remove_rmap->set_page_dirty)
 *    bdi.wb->list_lock		(zap_pte_range->set_page_dirty)
 *    ->inode->i_lock		(zap_pte_range->set_page_dirty)
 *    ->private_lock		(zap_pte_range->__set_page_dirty_buffers)
 *
 * ->i_mmap_mutex
 *   ->tasklist_lock            (memory_failure, collect_procs_ao)
 */

/*
 * Delete a page from the page cache and free it. Caller has to make
 * sure the page is locked and that nobody else uses it - or that usage
 * is safe.  The caller must hold the mapping's tree_lock.
 */
void __delete_from_page_cache(struct page *page)
{
	struct address_space *mapping = page->mapping;

	/*
	 * if we're uptodate, flush out into the cleancache, otherwise
	 * invalidate any existing cleancache entries.  We can't leave
	 * stale data around in the cleancache once our page is gone
	 */
	if (PageUptodate(page) && PageMappedToDisk(page))
		cleancache_put_page(page);
	else
		cleancache_invalidate_page(mapping, page);

	radix_tree_delete(&mapping->page_tree, page->index);
	page->mapping = NULL;
	/* Leave page->index set: truncation lookup relies upon it */
	mapping->nrpages--;
	__dec_zone_page_state(page, NR_FILE_PAGES);
	if (PageSwapBacked(page))
		__dec_zone_page_state(page, NR_SHMEM);
	BUG_ON(page_mapped(page));

	/*
	 * Some filesystems seem to re-dirty the page even after
	 * the VM has canceled the dirty bit (eg ext3 journaling).
	 *
	 * Fix it up by doing a final dirty accounting check after
	 * having removed the page entirely.
	 */
	if (PageDirty(page) && mapping_cap_account_dirty(mapping)) {
		dec_zone_page_state(page, NR_FILE_DIRTY);
		dec_bdi_stat(mapping->backing_dev_info, BDI_RECLAIMABLE);
	}
}

/**
 * delete_from_page_cache - delete page from page cache
 * @page: the page which the kernel is trying to remove from page cache
 *
 * This must be called only on pages that have been verified to be in the page
 * cache and locked.  It will never put the page into the free list, the caller
 * has a reference on the page.
 */
void delete_from_page_cache(struct page *page)
{
	struct address_space *mapping = page->mapping;
	void (*freepage)(struct page *);

	BUG_ON(!PageLocked(page));

	freepage = mapping->a_ops->freepage;
	spin_lock_irq(&mapping->tree_lock);
	__delete_from_page_cache(page);
	spin_unlock_irq(&mapping->tree_lock);
	mem_cgroup_uncharge_cache_page(page);

	if (freepage)
		freepage(page);
	page_cache_release(page);
}
EXPORT_SYMBOL(delete_from_page_cache);

static int sleep_on_page(void *word)
{
	io_schedule();
	return 0;
}

static int sleep_on_page_killable(void *word)
{
	sleep_on_page(word);
	return fatal_signal_pending(current) ? -EINTR : 0;
}
=======
 *    ->i_pages lock		(try_to_unmap_one)
 *    ->lruvec->lru_lock	(follow_page->mark_page_accessed)
 *    ->lruvec->lru_lock	(check_pte_range->isolate_lru_page)
 *    ->private_lock		(folio_remove_rmap_pte->set_page_dirty)
 *    ->i_pages lock		(folio_remove_rmap_pte->set_page_dirty)
 *    bdi.wb->list_lock		(folio_remove_rmap_pte->set_page_dirty)
 *    ->inode->i_lock		(folio_remove_rmap_pte->set_page_dirty)
 *    ->memcg->move_lock	(folio_remove_rmap_pte->folio_memcg_lock)
 *    bdi.wb->list_lock		(zap_pte_range->set_page_dirty)
 *    ->inode->i_lock		(zap_pte_range->set_page_dirty)
 *    ->private_lock		(zap_pte_range->block_dirty_folio)
 */

static void mapping_set_update(struct xa_state *xas,
		struct address_space *mapping)
{
	if (dax_mapping(mapping) || shmem_mapping(mapping))
		return;
	xas_set_update(xas, workingset_update_node);
	xas_set_lru(xas, &shadow_nodes);
}

static void page_cache_delete(struct address_space *mapping,
				   struct folio *folio, void *shadow)
{
	XA_STATE(xas, &mapping->i_pages, folio->index);
	long nr = 1;

	mapping_set_update(&xas, mapping);

	xas_set_order(&xas, folio->index, folio_order(folio));
	nr = folio_nr_pages(folio);

	VM_BUG_ON_FOLIO(!folio_test_locked(folio), folio);

	xas_store(&xas, shadow);
	xas_init_marks(&xas);

	folio->mapping = NULL;
	/* Leave page->index set: truncation lookup relies upon it */
	mapping->nrpages -= nr;
}

static void filemap_unaccount_folio(struct address_space *mapping,
		struct folio *folio)
{
	long nr;

	VM_BUG_ON_FOLIO(folio_mapped(folio), folio);
	if (!IS_ENABLED(CONFIG_DEBUG_VM) && unlikely(folio_mapped(folio))) {
		pr_alert("BUG: Bad page cache in process %s  pfn:%05lx\n",
			 current->comm, folio_pfn(folio));
		dump_page(&folio->page, "still mapped when deleted");
		dump_stack();
		add_taint(TAINT_BAD_PAGE, LOCKDEP_NOW_UNRELIABLE);

		if (mapping_exiting(mapping) && !folio_test_large(folio)) {
			int mapcount = page_mapcount(&folio->page);

			if (folio_ref_count(folio) >= mapcount + 2) {
				/*
				 * All vmas have already been torn down, so it's
				 * a good bet that actually the page is unmapped
				 * and we'd rather not leak it: if we're wrong,
				 * another bad page check should catch it later.
				 */
				page_mapcount_reset(&folio->page);
				folio_ref_sub(folio, mapcount);
			}
		}
	}

	/* hugetlb folios do not participate in page cache accounting. */
	if (folio_test_hugetlb(folio))
		return;

	nr = folio_nr_pages(folio);

	__lruvec_stat_mod_folio(folio, NR_FILE_PAGES, -nr);
	if (folio_test_swapbacked(folio)) {
		__lruvec_stat_mod_folio(folio, NR_SHMEM, -nr);
		if (folio_test_pmd_mappable(folio))
			__lruvec_stat_mod_folio(folio, NR_SHMEM_THPS, -nr);
	} else if (folio_test_pmd_mappable(folio)) {
		__lruvec_stat_mod_folio(folio, NR_FILE_THPS, -nr);
		filemap_nr_thps_dec(mapping);
	}

	/*
	 * At this point folio must be either written or cleaned by
	 * truncate.  Dirty folio here signals a bug and loss of
	 * unwritten data - on ordinary filesystems.
	 *
	 * But it's harmless on in-memory filesystems like tmpfs; and can
	 * occur when a driver which did get_user_pages() sets page dirty
	 * before putting it, while the inode is being finally evicted.
	 *
	 * Below fixes dirty accounting after removing the folio entirely
	 * but leaves the dirty flag set: it has no effect for truncated
	 * folio and anyway will be cleared before returning folio to
	 * buddy allocator.
	 */
	if (WARN_ON_ONCE(folio_test_dirty(folio) &&
			 mapping_can_writeback(mapping)))
		folio_account_cleaned(folio, inode_to_wb(mapping->host));
}

/*
 * Delete a page from the page cache and free it. Caller has to make
 * sure the page is locked and that nobody else uses it - or that usage
 * is safe.  The caller must hold the i_pages lock.
 */
void __filemap_remove_folio(struct folio *folio, void *shadow)
{
	struct address_space *mapping = folio->mapping;

	trace_mm_filemap_delete_from_page_cache(folio);
	filemap_unaccount_folio(mapping, folio);
	page_cache_delete(mapping, folio, shadow);
}

void filemap_free_folio(struct address_space *mapping, struct folio *folio)
{
	void (*free_folio)(struct folio *);
	int refs = 1;

	free_folio = mapping->a_ops->free_folio;
	if (free_folio)
		free_folio(folio);

	if (folio_test_large(folio))
		refs = folio_nr_pages(folio);
	folio_put_refs(folio, refs);
}

/**
 * filemap_remove_folio - Remove folio from page cache.
 * @folio: The folio.
 *
 * This must be called only on folios that are locked and have been
 * verified to be in the page cache.  It will never put the folio into
 * the free list because the caller has a reference on the page.
 */
void filemap_remove_folio(struct folio *folio)
{
	struct address_space *mapping = folio->mapping;

	BUG_ON(!folio_test_locked(folio));
	spin_lock(&mapping->host->i_lock);
	xa_lock_irq(&mapping->i_pages);
	__filemap_remove_folio(folio, NULL);
	xa_unlock_irq(&mapping->i_pages);
	if (mapping_shrinkable(mapping))
		inode_add_lru(mapping->host);
	spin_unlock(&mapping->host->i_lock);

	filemap_free_folio(mapping, folio);
}

/*
 * page_cache_delete_batch - delete several folios from page cache
 * @mapping: the mapping to which folios belong
 * @fbatch: batch of folios to delete
 *
 * The function walks over mapping->i_pages and removes folios passed in
 * @fbatch from the mapping. The function expects @fbatch to be sorted
 * by page index and is optimised for it to be dense.
 * It tolerates holes in @fbatch (mapping entries at those indices are not
 * modified).
 *
 * The function expects the i_pages lock to be held.
 */
static void page_cache_delete_batch(struct address_space *mapping,
			     struct folio_batch *fbatch)
{
	XA_STATE(xas, &mapping->i_pages, fbatch->folios[0]->index);
	long total_pages = 0;
	int i = 0;
	struct folio *folio;

	mapping_set_update(&xas, mapping);
	xas_for_each(&xas, folio, ULONG_MAX) {
		if (i >= folio_batch_count(fbatch))
			break;

		/* A swap/dax/shadow entry got inserted? Skip it. */
		if (xa_is_value(folio))
			continue;
		/*
		 * A page got inserted in our range? Skip it. We have our
		 * pages locked so they are protected from being removed.
		 * If we see a page whose index is higher than ours, it
		 * means our page has been removed, which shouldn't be
		 * possible because we're holding the PageLock.
		 */
		if (folio != fbatch->folios[i]) {
			VM_BUG_ON_FOLIO(folio->index >
					fbatch->folios[i]->index, folio);
			continue;
		}

		WARN_ON_ONCE(!folio_test_locked(folio));

		folio->mapping = NULL;
		/* Leave folio->index set: truncation lookup relies on it */

		i++;
		xas_store(&xas, NULL);
		total_pages += folio_nr_pages(folio);
	}
	mapping->nrpages -= total_pages;
}

void delete_from_page_cache_batch(struct address_space *mapping,
				  struct folio_batch *fbatch)
{
	int i;

	if (!folio_batch_count(fbatch))
		return;

	spin_lock(&mapping->host->i_lock);
	xa_lock_irq(&mapping->i_pages);
	for (i = 0; i < folio_batch_count(fbatch); i++) {
		struct folio *folio = fbatch->folios[i];

		trace_mm_filemap_delete_from_page_cache(folio);
		filemap_unaccount_folio(mapping, folio);
	}
	page_cache_delete_batch(mapping, fbatch);
	xa_unlock_irq(&mapping->i_pages);
	if (mapping_shrinkable(mapping))
		inode_add_lru(mapping->host);
	spin_unlock(&mapping->host->i_lock);

	for (i = 0; i < folio_batch_count(fbatch); i++)
		filemap_free_folio(mapping, fbatch->folios[i]);
}

int filemap_check_errors(struct address_space *mapping)
{
	int ret = 0;
	/* Check for outstanding write errors */
	if (test_bit(AS_ENOSPC, &mapping->flags) &&
	    test_and_clear_bit(AS_ENOSPC, &mapping->flags))
		ret = -ENOSPC;
	if (test_bit(AS_EIO, &mapping->flags) &&
	    test_and_clear_bit(AS_EIO, &mapping->flags))
		ret = -EIO;
	return ret;
}
EXPORT_SYMBOL(filemap_check_errors);

static int filemap_check_and_keep_errors(struct address_space *mapping)
{
	/* Check for outstanding write errors */
	if (test_bit(AS_EIO, &mapping->flags))
		return -EIO;
	if (test_bit(AS_ENOSPC, &mapping->flags))
		return -ENOSPC;
	return 0;
}

/**
 * filemap_fdatawrite_wbc - start writeback on mapping dirty pages in range
 * @mapping:	address space structure to write
 * @wbc:	the writeback_control controlling the writeout
 *
 * Call writepages on the mapping using the provided wbc to control the
 * writeout.
 *
 * Return: %0 on success, negative error code otherwise.
 */
int filemap_fdatawrite_wbc(struct address_space *mapping,
			   struct writeback_control *wbc)
{
	int ret;

	if (!mapping_can_writeback(mapping) ||
	    !mapping_tagged(mapping, PAGECACHE_TAG_DIRTY))
		return 0;

	wbc_attach_fdatawrite_inode(wbc, mapping->host);
	ret = do_writepages(mapping, wbc);
	wbc_detach_inode(wbc);
	return ret;
}
EXPORT_SYMBOL(filemap_fdatawrite_wbc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * __filemap_fdatawrite_range - start writeback on mapping dirty pages in range
 * @mapping:	address space structure to write
 * @start:	offset in bytes where the range starts
 * @end:	offset in bytes where the range ends (inclusive)
 * @sync_mode:	enable synchronous operation
 *
 * Start writeback against all of a mapping's dirty pages that lie
 * within the byte offsets <start, end> inclusive.
 *
 * If sync_mode is WB_SYNC_ALL then this is a "data integrity" operation, as
 * opposed to a regular memory cleansing writeback.  The difference between
 * these two operations is that if a dirty page/buffer is encountered, it must
 * be waited upon, and not just skipped over.
<<<<<<< HEAD
=======
 *
 * Return: %0 on success, negative error code otherwise.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
int __filemap_fdatawrite_range(struct address_space *mapping, loff_t start,
				loff_t end, int sync_mode)
{
<<<<<<< HEAD
	int ret;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct writeback_control wbc = {
		.sync_mode = sync_mode,
		.nr_to_write = LONG_MAX,
		.range_start = start,
		.range_end = end,
	};

<<<<<<< HEAD
	if (!mapping_cap_writeback_dirty(mapping))
		return 0;

	ret = do_writepages(mapping, &wbc);
	return ret;
=======
	return filemap_fdatawrite_wbc(mapping, &wbc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline int __filemap_fdatawrite(struct address_space *mapping,
	int sync_mode)
{
	return __filemap_fdatawrite_range(mapping, 0, LLONG_MAX, sync_mode);
}

int filemap_fdatawrite(struct address_space *mapping)
{
	return __filemap_fdatawrite(mapping, WB_SYNC_ALL);
}
EXPORT_SYMBOL(filemap_fdatawrite);

int filemap_fdatawrite_range(struct address_space *mapping, loff_t start,
				loff_t end)
{
	return __filemap_fdatawrite_range(mapping, start, end, WB_SYNC_ALL);
}
EXPORT_SYMBOL(filemap_fdatawrite_range);

/**
 * filemap_flush - mostly a non-blocking flush
 * @mapping:	target address_space
 *
 * This is a mostly non-blocking flush.  Not suitable for data-integrity
 * purposes - I/O may not be started against all dirty pages.
<<<<<<< HEAD
=======
 *
 * Return: %0 on success, negative error code otherwise.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
int filemap_flush(struct address_space *mapping)
{
	return __filemap_fdatawrite(mapping, WB_SYNC_NONE);
}
EXPORT_SYMBOL(filemap_flush);

/**
<<<<<<< HEAD
=======
 * filemap_range_has_page - check if a page exists in range.
 * @mapping:           address space within which to check
 * @start_byte:        offset in bytes where the range starts
 * @end_byte:          offset in bytes where the range ends (inclusive)
 *
 * Find at least one page in the range supplied, usually used to check if
 * direct writing in this range will trigger a writeback.
 *
 * Return: %true if at least one page exists in the specified range,
 * %false otherwise.
 */
bool filemap_range_has_page(struct address_space *mapping,
			   loff_t start_byte, loff_t end_byte)
{
	struct folio *folio;
	XA_STATE(xas, &mapping->i_pages, start_byte >> PAGE_SHIFT);
	pgoff_t max = end_byte >> PAGE_SHIFT;

	if (end_byte < start_byte)
		return false;

	rcu_read_lock();
	for (;;) {
		folio = xas_find(&xas, max);
		if (xas_retry(&xas, folio))
			continue;
		/* Shadow entries don't count */
		if (xa_is_value(folio))
			continue;
		/*
		 * We don't need to try to pin this page; we're about to
		 * release the RCU lock anyway.  It is enough to know that
		 * there was a page here recently.
		 */
		break;
	}
	rcu_read_unlock();

	return folio != NULL;
}
EXPORT_SYMBOL(filemap_range_has_page);

static void __filemap_fdatawait_range(struct address_space *mapping,
				     loff_t start_byte, loff_t end_byte)
{
	pgoff_t index = start_byte >> PAGE_SHIFT;
	pgoff_t end = end_byte >> PAGE_SHIFT;
	struct folio_batch fbatch;
	unsigned nr_folios;

	folio_batch_init(&fbatch);

	while (index <= end) {
		unsigned i;

		nr_folios = filemap_get_folios_tag(mapping, &index, end,
				PAGECACHE_TAG_WRITEBACK, &fbatch);

		if (!nr_folios)
			break;

		for (i = 0; i < nr_folios; i++) {
			struct folio *folio = fbatch.folios[i];

			folio_wait_writeback(folio);
			folio_clear_error(folio);
		}
		folio_batch_release(&fbatch);
		cond_resched();
	}
}

/**
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * filemap_fdatawait_range - wait for writeback to complete
 * @mapping:		address space structure to wait for
 * @start_byte:		offset in bytes where the range starts
 * @end_byte:		offset in bytes where the range ends (inclusive)
 *
 * Walk the list of under-writeback pages of the given address space
<<<<<<< HEAD
 * in the given range and wait for all of them.
=======
 * in the given range and wait for all of them.  Check error status of
 * the address space and return it.
 *
 * Since the error status of the address space is cleared by this function,
 * callers are responsible for checking the return value and handling and/or
 * reporting the error.
 *
 * Return: error status of the address space.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
int filemap_fdatawait_range(struct address_space *mapping, loff_t start_byte,
			    loff_t end_byte)
{
<<<<<<< HEAD
	pgoff_t index = start_byte >> PAGE_CACHE_SHIFT;
	pgoff_t end = end_byte >> PAGE_CACHE_SHIFT;
	struct pagevec pvec;
	int nr_pages;
	int ret = 0;

	if (end_byte < start_byte)
		return 0;

	pagevec_init(&pvec, 0);
	while ((index <= end) &&
			(nr_pages = pagevec_lookup_tag(&pvec, mapping, &index,
			PAGECACHE_TAG_WRITEBACK,
			min(end - index, (pgoff_t)PAGEVEC_SIZE-1) + 1)) != 0) {
		unsigned i;

		for (i = 0; i < nr_pages; i++) {
			struct page *page = pvec.pages[i];

			/* until radix tree lookup accepts end_index */
			if (page->index > end)
				continue;

			wait_on_page_writeback(page);
			if (TestClearPageError(page))
				ret = -EIO;
		}
		pagevec_release(&pvec);
		cond_resched();
	}

	/* Check for outstanding write errors */
	if (test_and_clear_bit(AS_ENOSPC, &mapping->flags))
		ret = -ENOSPC;
	if (test_and_clear_bit(AS_EIO, &mapping->flags))
		ret = -EIO;

	return ret;
=======
	__filemap_fdatawait_range(mapping, start_byte, end_byte);
	return filemap_check_errors(mapping);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL(filemap_fdatawait_range);

/**
<<<<<<< HEAD
 * filemap_fdatawait - wait for all under-writeback pages to complete
 * @mapping: address space structure to wait for
 *
 * Walk the list of under-writeback pages of the given address space
 * and wait for all of them.
 */
int filemap_fdatawait(struct address_space *mapping)
{
	loff_t i_size = i_size_read(mapping->host);

	if (i_size == 0)
		return 0;

	return filemap_fdatawait_range(mapping, 0, i_size - 1);
}
EXPORT_SYMBOL(filemap_fdatawait);

int filemap_write_and_wait(struct address_space *mapping)
{
	int err = 0;

	if (mapping->nrpages) {
		err = filemap_fdatawrite(mapping);
		/*
		 * Even if the above returned error, the pages may be
		 * written partially (e.g. -ENOSPC), so we wait for it.
		 * But the -EIO is special case, it may indicate the worst
		 * thing (e.g. bug) happened, so we avoid waiting for it.
		 */
		if (err != -EIO) {
			int err2 = filemap_fdatawait(mapping);
			if (!err)
				err = err2;
		}
	}
	return err;
}
EXPORT_SYMBOL(filemap_write_and_wait);
=======
 * filemap_fdatawait_range_keep_errors - wait for writeback to complete
 * @mapping:		address space structure to wait for
 * @start_byte:		offset in bytes where the range starts
 * @end_byte:		offset in bytes where the range ends (inclusive)
 *
 * Walk the list of under-writeback pages of the given address space in the
 * given range and wait for all of them.  Unlike filemap_fdatawait_range(),
 * this function does not clear error status of the address space.
 *
 * Use this function if callers don't handle errors themselves.  Expected
 * call sites are system-wide / filesystem-wide data flushers: e.g. sync(2),
 * fsfreeze(8)
 */
int filemap_fdatawait_range_keep_errors(struct address_space *mapping,
		loff_t start_byte, loff_t end_byte)
{
	__filemap_fdatawait_range(mapping, start_byte, end_byte);
	return filemap_check_and_keep_errors(mapping);
}
EXPORT_SYMBOL(filemap_fdatawait_range_keep_errors);

/**
 * file_fdatawait_range - wait for writeback to complete
 * @file:		file pointing to address space structure to wait for
 * @start_byte:		offset in bytes where the range starts
 * @end_byte:		offset in bytes where the range ends (inclusive)
 *
 * Walk the list of under-writeback pages of the address space that file
 * refers to, in the given range and wait for all of them.  Check error
 * status of the address space vs. the file->f_wb_err cursor and return it.
 *
 * Since the error status of the file is advanced by this function,
 * callers are responsible for checking the return value and handling and/or
 * reporting the error.
 *
 * Return: error status of the address space vs. the file->f_wb_err cursor.
 */
int file_fdatawait_range(struct file *file, loff_t start_byte, loff_t end_byte)
{
	struct address_space *mapping = file->f_mapping;

	__filemap_fdatawait_range(mapping, start_byte, end_byte);
	return file_check_and_advance_wb_err(file);
}
EXPORT_SYMBOL(file_fdatawait_range);

/**
 * filemap_fdatawait_keep_errors - wait for writeback without clearing errors
 * @mapping: address space structure to wait for
 *
 * Walk the list of under-writeback pages of the given address space
 * and wait for all of them.  Unlike filemap_fdatawait(), this function
 * does not clear error status of the address space.
 *
 * Use this function if callers don't handle errors themselves.  Expected
 * call sites are system-wide / filesystem-wide data flushers: e.g. sync(2),
 * fsfreeze(8)
 *
 * Return: error status of the address space.
 */
int filemap_fdatawait_keep_errors(struct address_space *mapping)
{
	__filemap_fdatawait_range(mapping, 0, LLONG_MAX);
	return filemap_check_and_keep_errors(mapping);
}
EXPORT_SYMBOL(filemap_fdatawait_keep_errors);

/* Returns true if writeback might be needed or already in progress. */
static bool mapping_needs_writeback(struct address_space *mapping)
{
	return mapping->nrpages;
}

bool filemap_range_has_writeback(struct address_space *mapping,
				 loff_t start_byte, loff_t end_byte)
{
	XA_STATE(xas, &mapping->i_pages, start_byte >> PAGE_SHIFT);
	pgoff_t max = end_byte >> PAGE_SHIFT;
	struct folio *folio;

	if (end_byte < start_byte)
		return false;

	rcu_read_lock();
	xas_for_each(&xas, folio, max) {
		if (xas_retry(&xas, folio))
			continue;
		if (xa_is_value(folio))
			continue;
		if (folio_test_dirty(folio) || folio_test_locked(folio) ||
				folio_test_writeback(folio))
			break;
	}
	rcu_read_unlock();
	return folio != NULL;
}
EXPORT_SYMBOL_GPL(filemap_range_has_writeback);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * filemap_write_and_wait_range - write out & wait on a file range
 * @mapping:	the address_space for the pages
 * @lstart:	offset in bytes where the range starts
 * @lend:	offset in bytes where the range ends (inclusive)
 *
 * Write out and wait upon file offsets lstart->lend, inclusive.
 *
<<<<<<< HEAD
 * Note that `lend' is inclusive (describes the last byte to be written) so
 * that this function can be used to write to the very end-of-file (end = -1).
=======
 * Note that @lend is inclusive (describes the last byte to be written) so
 * that this function can be used to write to the very end-of-file (end = -1).
 *
 * Return: error status of the address space.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
int filemap_write_and_wait_range(struct address_space *mapping,
				 loff_t lstart, loff_t lend)
{
<<<<<<< HEAD
	int err = 0;

	if (mapping->nrpages) {
		err = __filemap_fdatawrite_range(mapping, lstart, lend,
						 WB_SYNC_ALL);
		/* See comment of filemap_write_and_wait() */
		if (err != -EIO) {
			int err2 = filemap_fdatawait_range(mapping,
						lstart, lend);
			if (!err)
				err = err2;
		}
	}
=======
	int err = 0, err2;

	if (lend < lstart)
		return 0;

	if (mapping_needs_writeback(mapping)) {
		err = __filemap_fdatawrite_range(mapping, lstart, lend,
						 WB_SYNC_ALL);
		/*
		 * Even if the above returned error, the pages may be
		 * written partially (e.g. -ENOSPC), so we wait for it.
		 * But the -EIO is special case, it may indicate the worst
		 * thing (e.g. bug) happened, so we avoid waiting for it.
		 */
		if (err != -EIO)
			__filemap_fdatawait_range(mapping, lstart, lend);
	}
	err2 = filemap_check_errors(mapping);
	if (!err)
		err = err2;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}
EXPORT_SYMBOL(filemap_write_and_wait_range);

<<<<<<< HEAD
/**
 * replace_page_cache_page - replace a pagecache page with a new one
 * @old:	page to be replaced
 * @new:	page to replace with
 * @gfp_mask:	allocation mode
 *
 * This function replaces a page in the pagecache with a new one.  On
 * success it acquires the pagecache reference for the new page and
 * drops it for the old page.  Both the old and new pages must be
 * locked.  This function does not add the new page to the LRU, the
 * caller must do that.
 *
 * The remove + add is atomic.  The only way this function can fail is
 * memory allocation failure.
 */
int replace_page_cache_page(struct page *old, struct page *new, gfp_t gfp_mask)
{
	int error;

	VM_BUG_ON(!PageLocked(old));
	VM_BUG_ON(!PageLocked(new));
	VM_BUG_ON(new->mapping);

	error = radix_tree_preload(gfp_mask & ~__GFP_HIGHMEM);
	if (!error) {
		struct address_space *mapping = old->mapping;
		void (*freepage)(struct page *);

		pgoff_t offset = old->index;
		freepage = mapping->a_ops->freepage;

		page_cache_get(new);
		new->mapping = mapping;
		new->index = offset;

		spin_lock_irq(&mapping->tree_lock);
		__delete_from_page_cache(old);
		error = radix_tree_insert(&mapping->page_tree, offset, new);
		BUG_ON(error);
		mapping->nrpages++;
		__inc_zone_page_state(new, NR_FILE_PAGES);
		if (PageSwapBacked(new))
			__inc_zone_page_state(new, NR_SHMEM);
		spin_unlock_irq(&mapping->tree_lock);
		/* mem_cgroup codes must not be called under tree_lock */
		mem_cgroup_replace_page_cache(old, new);
		radix_tree_preload_end();
		if (freepage)
			freepage(old);
		page_cache_release(old);
	}

	return error;
}
EXPORT_SYMBOL_GPL(replace_page_cache_page);

/**
 * add_to_page_cache_locked - add a locked page to the pagecache
 * @page:	page to add
 * @mapping:	the page's address_space
 * @offset:	page index
 * @gfp_mask:	page allocation mode
 *
 * This function is used to add a page to the pagecache. It must be locked.
 * This function does not add the page to the LRU.  The caller must do that.
 */
int add_to_page_cache_locked(struct page *page, struct address_space *mapping,
		pgoff_t offset, gfp_t gfp_mask)
{
	int error;

	VM_BUG_ON(!PageLocked(page));
	VM_BUG_ON(PageSwapBacked(page));

	error = mem_cgroup_cache_charge(page, current->mm,
					gfp_mask & GFP_RECLAIM_MASK);
	if (error)
		goto out;

	error = radix_tree_preload(gfp_mask & ~__GFP_HIGHMEM);
	if (error == 0) {
		page_cache_get(page);
		page->mapping = mapping;
		page->index = offset;

		spin_lock_irq(&mapping->tree_lock);
		error = radix_tree_insert(&mapping->page_tree, offset, page);
		if (likely(!error)) {
			mapping->nrpages++;
			__inc_zone_page_state(page, NR_FILE_PAGES);
			spin_unlock_irq(&mapping->tree_lock);
		} else {
			page->mapping = NULL;
			/* Leave page->index set: truncation relies upon it */
			spin_unlock_irq(&mapping->tree_lock);
			mem_cgroup_uncharge_cache_page(page);
			page_cache_release(page);
		}
		radix_tree_preload_end();
	} else
		mem_cgroup_uncharge_cache_page(page);
out:
	return error;
}
EXPORT_SYMBOL(add_to_page_cache_locked);

int add_to_page_cache_lru(struct page *page, struct address_space *mapping,
				pgoff_t offset, gfp_t gfp_mask)
{
	int ret;

	ret = add_to_page_cache(page, mapping, offset, gfp_mask);
	if (ret == 0)
		lru_cache_add_file(page);
	return ret;
}
EXPORT_SYMBOL_GPL(add_to_page_cache_lru);

#ifdef CONFIG_NUMA
struct page *__page_cache_alloc(gfp_t gfp)
{
	int n;
	struct page *page;
=======
void __filemap_set_wb_err(struct address_space *mapping, int err)
{
	errseq_t eseq = errseq_set(&mapping->wb_err, err);

	trace_filemap_set_wb_err(mapping, eseq);
}
EXPORT_SYMBOL(__filemap_set_wb_err);

/**
 * file_check_and_advance_wb_err - report wb error (if any) that was previously
 * 				   and advance wb_err to current one
 * @file: struct file on which the error is being reported
 *
 * When userland calls fsync (or something like nfsd does the equivalent), we
 * want to report any writeback errors that occurred since the last fsync (or
 * since the file was opened if there haven't been any).
 *
 * Grab the wb_err from the mapping. If it matches what we have in the file,
 * then just quickly return 0. The file is all caught up.
 *
 * If it doesn't match, then take the mapping value, set the "seen" flag in
 * it and try to swap it into place. If it works, or another task beat us
 * to it with the new value, then update the f_wb_err and return the error
 * portion. The error at this point must be reported via proper channels
 * (a'la fsync, or NFS COMMIT operation, etc.).
 *
 * While we handle mapping->wb_err with atomic operations, the f_wb_err
 * value is protected by the f_lock since we must ensure that it reflects
 * the latest value swapped in for this file descriptor.
 *
 * Return: %0 on success, negative error code otherwise.
 */
int file_check_and_advance_wb_err(struct file *file)
{
	int err = 0;
	errseq_t old = READ_ONCE(file->f_wb_err);
	struct address_space *mapping = file->f_mapping;

	/* Locklessly handle the common case where nothing has changed */
	if (errseq_check(&mapping->wb_err, old)) {
		/* Something changed, must use slow path */
		spin_lock(&file->f_lock);
		old = file->f_wb_err;
		err = errseq_check_and_advance(&mapping->wb_err,
						&file->f_wb_err);
		trace_file_check_and_advance_wb_err(file, old);
		spin_unlock(&file->f_lock);
	}

	/*
	 * We're mostly using this function as a drop in replacement for
	 * filemap_check_errors. Clear AS_EIO/AS_ENOSPC to emulate the effect
	 * that the legacy code would have had on these flags.
	 */
	clear_bit(AS_EIO, &mapping->flags);
	clear_bit(AS_ENOSPC, &mapping->flags);
	return err;
}
EXPORT_SYMBOL(file_check_and_advance_wb_err);

/**
 * file_write_and_wait_range - write out & wait on a file range
 * @file:	file pointing to address_space with pages
 * @lstart:	offset in bytes where the range starts
 * @lend:	offset in bytes where the range ends (inclusive)
 *
 * Write out and wait upon file offsets lstart->lend, inclusive.
 *
 * Note that @lend is inclusive (describes the last byte to be written) so
 * that this function can be used to write to the very end-of-file (end = -1).
 *
 * After writing out and waiting on the data, we check and advance the
 * f_wb_err cursor to the latest value, and return any errors detected there.
 *
 * Return: %0 on success, negative error code otherwise.
 */
int file_write_and_wait_range(struct file *file, loff_t lstart, loff_t lend)
{
	int err = 0, err2;
	struct address_space *mapping = file->f_mapping;

	if (lend < lstart)
		return 0;

	if (mapping_needs_writeback(mapping)) {
		err = __filemap_fdatawrite_range(mapping, lstart, lend,
						 WB_SYNC_ALL);
		/* See comment of filemap_write_and_wait() */
		if (err != -EIO)
			__filemap_fdatawait_range(mapping, lstart, lend);
	}
	err2 = file_check_and_advance_wb_err(file);
	if (!err)
		err = err2;
	return err;
}
EXPORT_SYMBOL(file_write_and_wait_range);

/**
 * replace_page_cache_folio - replace a pagecache folio with a new one
 * @old:	folio to be replaced
 * @new:	folio to replace with
 *
 * This function replaces a folio in the pagecache with a new one.  On
 * success it acquires the pagecache reference for the new folio and
 * drops it for the old folio.  Both the old and new folios must be
 * locked.  This function does not add the new folio to the LRU, the
 * caller must do that.
 *
 * The remove + add is atomic.  This function cannot fail.
 */
void replace_page_cache_folio(struct folio *old, struct folio *new)
{
	struct address_space *mapping = old->mapping;
	void (*free_folio)(struct folio *) = mapping->a_ops->free_folio;
	pgoff_t offset = old->index;
	XA_STATE(xas, &mapping->i_pages, offset);

	VM_BUG_ON_FOLIO(!folio_test_locked(old), old);
	VM_BUG_ON_FOLIO(!folio_test_locked(new), new);
	VM_BUG_ON_FOLIO(new->mapping, new);

	folio_get(new);
	new->mapping = mapping;
	new->index = offset;

	mem_cgroup_replace_folio(old, new);

	xas_lock_irq(&xas);
	xas_store(&xas, new);

	old->mapping = NULL;
	/* hugetlb pages do not participate in page cache accounting. */
	if (!folio_test_hugetlb(old))
		__lruvec_stat_sub_folio(old, NR_FILE_PAGES);
	if (!folio_test_hugetlb(new))
		__lruvec_stat_add_folio(new, NR_FILE_PAGES);
	if (folio_test_swapbacked(old))
		__lruvec_stat_sub_folio(old, NR_SHMEM);
	if (folio_test_swapbacked(new))
		__lruvec_stat_add_folio(new, NR_SHMEM);
	xas_unlock_irq(&xas);
	if (free_folio)
		free_folio(old);
	folio_put(old);
}
EXPORT_SYMBOL_GPL(replace_page_cache_folio);

noinline int __filemap_add_folio(struct address_space *mapping,
		struct folio *folio, pgoff_t index, gfp_t gfp, void **shadowp)
{
	XA_STATE(xas, &mapping->i_pages, index);
	bool huge = folio_test_hugetlb(folio);
	bool charged = false;
	long nr = 1;

	VM_BUG_ON_FOLIO(!folio_test_locked(folio), folio);
	VM_BUG_ON_FOLIO(folio_test_swapbacked(folio), folio);
	mapping_set_update(&xas, mapping);

	if (!huge) {
		int error = mem_cgroup_charge(folio, NULL, gfp);
		if (error)
			return error;
		charged = true;
	}

	VM_BUG_ON_FOLIO(index & (folio_nr_pages(folio) - 1), folio);
	xas_set_order(&xas, index, folio_order(folio));
	nr = folio_nr_pages(folio);

	gfp &= GFP_RECLAIM_MASK;
	folio_ref_add(folio, nr);
	folio->mapping = mapping;
	folio->index = xas.xa_index;

	do {
		unsigned int order = xa_get_order(xas.xa, xas.xa_index);
		void *entry, *old = NULL;

		if (order > folio_order(folio))
			xas_split_alloc(&xas, xa_load(xas.xa, xas.xa_index),
					order, gfp);
		xas_lock_irq(&xas);
		xas_for_each_conflict(&xas, entry) {
			old = entry;
			if (!xa_is_value(entry)) {
				xas_set_err(&xas, -EEXIST);
				goto unlock;
			}
		}

		if (old) {
			if (shadowp)
				*shadowp = old;
			/* entry may have been split before we acquired lock */
			order = xa_get_order(xas.xa, xas.xa_index);
			if (order > folio_order(folio)) {
				/* How to handle large swap entries? */
				BUG_ON(shmem_mapping(mapping));
				xas_split(&xas, old, order);
				xas_reset(&xas);
			}
		}

		xas_store(&xas, folio);
		if (xas_error(&xas))
			goto unlock;

		mapping->nrpages += nr;

		/* hugetlb pages do not participate in page cache accounting */
		if (!huge) {
			__lruvec_stat_mod_folio(folio, NR_FILE_PAGES, nr);
			if (folio_test_pmd_mappable(folio))
				__lruvec_stat_mod_folio(folio,
						NR_FILE_THPS, nr);
		}
unlock:
		xas_unlock_irq(&xas);
	} while (xas_nomem(&xas, gfp));

	if (xas_error(&xas))
		goto error;

	trace_mm_filemap_add_to_page_cache(folio);
	return 0;
error:
	if (charged)
		mem_cgroup_uncharge(folio);
	folio->mapping = NULL;
	/* Leave page->index set: truncation relies upon it */
	folio_put_refs(folio, nr);
	return xas_error(&xas);
}
ALLOW_ERROR_INJECTION(__filemap_add_folio, ERRNO);

int filemap_add_folio(struct address_space *mapping, struct folio *folio,
				pgoff_t index, gfp_t gfp)
{
	void *shadow = NULL;
	int ret;

	__folio_set_locked(folio);
	ret = __filemap_add_folio(mapping, folio, index, gfp, &shadow);
	if (unlikely(ret))
		__folio_clear_locked(folio);
	else {
		/*
		 * The folio might have been evicted from cache only
		 * recently, in which case it should be activated like
		 * any other repeatedly accessed folio.
		 * The exception is folios getting rewritten; evicting other
		 * data from the working set, only to cache data that will
		 * get overwritten with something else, is a waste of memory.
		 */
		WARN_ON_ONCE(folio_test_active(folio));
		if (!(gfp & __GFP_WRITE) && shadow)
			workingset_refault(folio, shadow);
		folio_add_lru(folio);
	}
	return ret;
}
EXPORT_SYMBOL_GPL(filemap_add_folio);

#ifdef CONFIG_NUMA
struct folio *filemap_alloc_folio(gfp_t gfp, unsigned int order)
{
	int n;
	struct folio *folio;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (cpuset_do_page_mem_spread()) {
		unsigned int cpuset_mems_cookie;
		do {
<<<<<<< HEAD
			cpuset_mems_cookie = get_mems_allowed();
			n = cpuset_mem_spread_node();
			page = alloc_pages_exact_node(n, gfp, 0);
		} while (!put_mems_allowed(cpuset_mems_cookie) && !page);

		return page;
	}
	return alloc_pages(gfp, 0);
}
EXPORT_SYMBOL(__page_cache_alloc);
#endif

/*
=======
			cpuset_mems_cookie = read_mems_allowed_begin();
			n = cpuset_mem_spread_node();
			folio = __folio_alloc_node(gfp, order, n);
		} while (!folio && read_mems_allowed_retry(cpuset_mems_cookie));

		return folio;
	}
	return folio_alloc(gfp, order);
}
EXPORT_SYMBOL(filemap_alloc_folio);
#endif

/*
 * filemap_invalidate_lock_two - lock invalidate_lock for two mappings
 *
 * Lock exclusively invalidate_lock of any passed mapping that is not NULL.
 *
 * @mapping1: the first mapping to lock
 * @mapping2: the second mapping to lock
 */
void filemap_invalidate_lock_two(struct address_space *mapping1,
				 struct address_space *mapping2)
{
	if (mapping1 > mapping2)
		swap(mapping1, mapping2);
	if (mapping1)
		down_write(&mapping1->invalidate_lock);
	if (mapping2 && mapping1 != mapping2)
		down_write_nested(&mapping2->invalidate_lock, 1);
}
EXPORT_SYMBOL(filemap_invalidate_lock_two);

/*
 * filemap_invalidate_unlock_two - unlock invalidate_lock for two mappings
 *
 * Unlock exclusive invalidate_lock of any passed mapping that is not NULL.
 *
 * @mapping1: the first mapping to unlock
 * @mapping2: the second mapping to unlock
 */
void filemap_invalidate_unlock_two(struct address_space *mapping1,
				   struct address_space *mapping2)
{
	if (mapping1)
		up_write(&mapping1->invalidate_lock);
	if (mapping2 && mapping1 != mapping2)
		up_write(&mapping2->invalidate_lock);
}
EXPORT_SYMBOL(filemap_invalidate_unlock_two);

/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * In order to wait for pages to become available there must be
 * waitqueues associated with pages. By using a hash table of
 * waitqueues where the bucket discipline is to maintain all
 * waiters on the same queue and wake all when any of the pages
 * become available, and for the woken contexts to check to be
 * sure the appropriate page became available, this saves space
 * at a cost of "thundering herd" phenomena during rare hash
 * collisions.
 */
<<<<<<< HEAD
static wait_queue_head_t *page_waitqueue(struct page *page)
{
	const struct zone *zone = page_zone(page);

	return &zone->wait_table[hash_ptr(page, zone->wait_table_bits)];
}

static inline void wake_up_page(struct page *page, int bit)
{
	__wake_up_bit(page_waitqueue(page), &page->flags, bit);
}

void wait_on_page_bit(struct page *page, int bit_nr)
{
	DEFINE_WAIT_BIT(wait, &page->flags, bit_nr);

	if (test_bit(bit_nr, &page->flags))
		__wait_on_bit(page_waitqueue(page), &wait, sleep_on_page,
							TASK_UNINTERRUPTIBLE);
}
EXPORT_SYMBOL(wait_on_page_bit);

int wait_on_page_bit_killable(struct page *page, int bit_nr)
{
	DEFINE_WAIT_BIT(wait, &page->flags, bit_nr);

	if (!test_bit(bit_nr, &page->flags))
		return 0;

	return __wait_on_bit(page_waitqueue(page), &wait,
			     sleep_on_page_killable, TASK_KILLABLE);
}

/**
 * add_page_wait_queue - Add an arbitrary waiter to a page's wait queue
 * @page: Page defining the wait queue of interest
 * @waiter: Waiter to add to the queue
 *
 * Add an arbitrary @waiter to the wait queue for the nominated @page.
 */
void add_page_wait_queue(struct page *page, wait_queue_t *waiter)
{
	wait_queue_head_t *q = page_waitqueue(page);
	unsigned long flags;

	spin_lock_irqsave(&q->lock, flags);
	__add_wait_queue(q, waiter);
	spin_unlock_irqrestore(&q->lock, flags);
}
EXPORT_SYMBOL_GPL(add_page_wait_queue);

/**
 * unlock_page - unlock a locked page
 * @page: the page
 *
 * Unlocks the page and wakes up sleepers in ___wait_on_page_locked().
 * Also wakes sleepers in wait_on_page_writeback() because the wakeup
 * mechananism between PageLocked pages and PageWriteback pages is shared.
 * But that's OK - sleepers in wait_on_page_writeback() just go back to sleep.
 *
 * The mb is necessary to enforce ordering between the clear_bit and the read
 * of the waitqueue (to avoid SMP races with a parallel wait_on_page_locked()).
 */
void unlock_page(struct page *page)
{
	VM_BUG_ON(!PageLocked(page));
	clear_bit_unlock(PG_locked, &page->flags);
	smp_mb__after_clear_bit();
	wake_up_page(page, PG_locked);
}
EXPORT_SYMBOL(unlock_page);

/**
 * end_page_writeback - end writeback against a page
 * @page: the page
 */
void end_page_writeback(struct page *page)
{
	if (TestClearPageReclaim(page))
		rotate_reclaimable_page(page);

	if (!test_clear_page_writeback(page))
		BUG();

	smp_mb__after_clear_bit();
	wake_up_page(page, PG_writeback);
}
EXPORT_SYMBOL(end_page_writeback);

/**
 * __lock_page - get a lock on the page, assuming we need to sleep to get it
 * @page: the page to lock
 */
void __lock_page(struct page *page)
{
	DEFINE_WAIT_BIT(wait, &page->flags, PG_locked);

	__wait_on_bit_lock(page_waitqueue(page), &wait, sleep_on_page,
							TASK_UNINTERRUPTIBLE);
}
EXPORT_SYMBOL(__lock_page);

int __lock_page_killable(struct page *page)
{
	DEFINE_WAIT_BIT(wait, &page->flags, PG_locked);

	return __wait_on_bit_lock(page_waitqueue(page), &wait,
					sleep_on_page_killable, TASK_KILLABLE);
}
EXPORT_SYMBOL_GPL(__lock_page_killable);

int __lock_page_or_retry(struct page *page, struct mm_struct *mm,
			 unsigned int flags)
{
	if (flags & FAULT_FLAG_ALLOW_RETRY) {
		/*
		 * CAUTION! In this case, mmap_sem is not released
		 * even though return 0.
		 */
		if (flags & FAULT_FLAG_RETRY_NOWAIT)
			return 0;

		up_read(&mm->mmap_sem);
		if (flags & FAULT_FLAG_KILLABLE)
			wait_on_page_locked_killable(page);
		else
			wait_on_page_locked(page);
		return 0;
	} else {
		if (flags & FAULT_FLAG_KILLABLE) {
			int ret;

			ret = __lock_page_killable(page);
			if (ret) {
				up_read(&mm->mmap_sem);
				return 0;
			}
		} else
			__lock_page(page);
		return 1;
	}
}

/**
 * find_get_page - find and get a page reference
 * @mapping: the address_space to search
 * @offset: the page index
 *
 * Is there a pagecache struct page at the given (mapping, offset) tuple?
 * If yes, increment its refcount and return it; if no, return NULL.
 */
struct page *find_get_page(struct address_space *mapping, pgoff_t offset)
{
	void **pagep;
	struct page *page;

	rcu_read_lock();
repeat:
	page = NULL;
	pagep = radix_tree_lookup_slot(&mapping->page_tree, offset);
	if (pagep) {
		page = radix_tree_deref_slot(pagep);
		if (unlikely(!page))
			goto out;
		if (radix_tree_exception(page)) {
			if (radix_tree_deref_retry(page))
				goto repeat;
			/*
			 * Otherwise, shmem/tmpfs must be storing a swap entry
			 * here as an exceptional entry: so return it without
			 * attempting to raise page count.
			 */
			goto out;
		}
		if (!page_cache_get_speculative(page))
			goto repeat;

		/*
		 * Has the page moved?
		 * This is part of the lockless pagecache protocol. See
		 * include/linux/pagemap.h for details.
		 */
		if (unlikely(page != *pagep)) {
			page_cache_release(page);
			goto repeat;
		}
=======
#define PAGE_WAIT_TABLE_BITS 8
#define PAGE_WAIT_TABLE_SIZE (1 << PAGE_WAIT_TABLE_BITS)
static wait_queue_head_t folio_wait_table[PAGE_WAIT_TABLE_SIZE] __cacheline_aligned;

static wait_queue_head_t *folio_waitqueue(struct folio *folio)
{
	return &folio_wait_table[hash_ptr(folio, PAGE_WAIT_TABLE_BITS)];
}

void __init pagecache_init(void)
{
	int i;

	for (i = 0; i < PAGE_WAIT_TABLE_SIZE; i++)
		init_waitqueue_head(&folio_wait_table[i]);

	page_writeback_init();
}

/*
 * The page wait code treats the "wait->flags" somewhat unusually, because
 * we have multiple different kinds of waits, not just the usual "exclusive"
 * one.
 *
 * We have:
 *
 *  (a) no special bits set:
 *
 *	We're just waiting for the bit to be released, and when a waker
 *	calls the wakeup function, we set WQ_FLAG_WOKEN and wake it up,
 *	and remove it from the wait queue.
 *
 *	Simple and straightforward.
 *
 *  (b) WQ_FLAG_EXCLUSIVE:
 *
 *	The waiter is waiting to get the lock, and only one waiter should
 *	be woken up to avoid any thundering herd behavior. We'll set the
 *	WQ_FLAG_WOKEN bit, wake it up, and remove it from the wait queue.
 *
 *	This is the traditional exclusive wait.
 *
 *  (c) WQ_FLAG_EXCLUSIVE | WQ_FLAG_CUSTOM:
 *
 *	The waiter is waiting to get the bit, and additionally wants the
 *	lock to be transferred to it for fair lock behavior. If the lock
 *	cannot be taken, we stop walking the wait queue without waking
 *	the waiter.
 *
 *	This is the "fair lock handoff" case, and in addition to setting
 *	WQ_FLAG_WOKEN, we set WQ_FLAG_DONE to let the waiter easily see
 *	that it now has the lock.
 */
static int wake_page_function(wait_queue_entry_t *wait, unsigned mode, int sync, void *arg)
{
	unsigned int flags;
	struct wait_page_key *key = arg;
	struct wait_page_queue *wait_page
		= container_of(wait, struct wait_page_queue, wait);

	if (!wake_page_match(wait_page, key))
		return 0;

	/*
	 * If it's a lock handoff wait, we get the bit for it, and
	 * stop walking (and do not wake it up) if we can't.
	 */
	flags = wait->flags;
	if (flags & WQ_FLAG_EXCLUSIVE) {
		if (test_bit(key->bit_nr, &key->folio->flags))
			return -1;
		if (flags & WQ_FLAG_CUSTOM) {
			if (test_and_set_bit(key->bit_nr, &key->folio->flags))
				return -1;
			flags |= WQ_FLAG_DONE;
		}
	}

	/*
	 * We are holding the wait-queue lock, but the waiter that
	 * is waiting for this will be checking the flags without
	 * any locking.
	 *
	 * So update the flags atomically, and wake up the waiter
	 * afterwards to avoid any races. This store-release pairs
	 * with the load-acquire in folio_wait_bit_common().
	 */
	smp_store_release(&wait->flags, flags | WQ_FLAG_WOKEN);
	wake_up_state(wait->private, mode);

	/*
	 * Ok, we have successfully done what we're waiting for,
	 * and we can unconditionally remove the wait entry.
	 *
	 * Note that this pairs with the "finish_wait()" in the
	 * waiter, and has to be the absolute last thing we do.
	 * After this list_del_init(&wait->entry) the wait entry
	 * might be de-allocated and the process might even have
	 * exited.
	 */
	list_del_init_careful(&wait->entry);
	return (flags & WQ_FLAG_EXCLUSIVE) != 0;
}

static void folio_wake_bit(struct folio *folio, int bit_nr)
{
	wait_queue_head_t *q = folio_waitqueue(folio);
	struct wait_page_key key;
	unsigned long flags;

	key.folio = folio;
	key.bit_nr = bit_nr;
	key.page_match = 0;

	spin_lock_irqsave(&q->lock, flags);
	__wake_up_locked_key(q, TASK_NORMAL, &key);

	/*
	 * It's possible to miss clearing waiters here, when we woke our page
	 * waiters, but the hashed waitqueue has waiters for other pages on it.
	 * That's okay, it's a rare case. The next waker will clear it.
	 *
	 * Note that, depending on the page pool (buddy, hugetlb, ZONE_DEVICE,
	 * other), the flag may be cleared in the course of freeing the page;
	 * but that is not required for correctness.
	 */
	if (!waitqueue_active(q) || !key.page_match)
		folio_clear_waiters(folio);

	spin_unlock_irqrestore(&q->lock, flags);
}

/*
 * A choice of three behaviors for folio_wait_bit_common():
 */
enum behavior {
	EXCLUSIVE,	/* Hold ref to page and take the bit when woken, like
			 * __folio_lock() waiting on then setting PG_locked.
			 */
	SHARED,		/* Hold ref to page and check the bit when woken, like
			 * folio_wait_writeback() waiting on PG_writeback.
			 */
	DROP,		/* Drop ref to page before wait, no check when woken,
			 * like folio_put_wait_locked() on PG_locked.
			 */
};

/*
 * Attempt to check (or get) the folio flag, and mark us done
 * if successful.
 */
static inline bool folio_trylock_flag(struct folio *folio, int bit_nr,
					struct wait_queue_entry *wait)
{
	if (wait->flags & WQ_FLAG_EXCLUSIVE) {
		if (test_and_set_bit(bit_nr, &folio->flags))
			return false;
	} else if (test_bit(bit_nr, &folio->flags))
		return false;

	wait->flags |= WQ_FLAG_WOKEN | WQ_FLAG_DONE;
	return true;
}

/* How many times do we accept lock stealing from under a waiter? */
int sysctl_page_lock_unfairness = 5;

static inline int folio_wait_bit_common(struct folio *folio, int bit_nr,
		int state, enum behavior behavior)
{
	wait_queue_head_t *q = folio_waitqueue(folio);
	int unfairness = sysctl_page_lock_unfairness;
	struct wait_page_queue wait_page;
	wait_queue_entry_t *wait = &wait_page.wait;
	bool thrashing = false;
	unsigned long pflags;
	bool in_thrashing;

	if (bit_nr == PG_locked &&
	    !folio_test_uptodate(folio) && folio_test_workingset(folio)) {
		delayacct_thrashing_start(&in_thrashing);
		psi_memstall_enter(&pflags);
		thrashing = true;
	}

	init_wait(wait);
	wait->func = wake_page_function;
	wait_page.folio = folio;
	wait_page.bit_nr = bit_nr;

repeat:
	wait->flags = 0;
	if (behavior == EXCLUSIVE) {
		wait->flags = WQ_FLAG_EXCLUSIVE;
		if (--unfairness < 0)
			wait->flags |= WQ_FLAG_CUSTOM;
	}

	/*
	 * Do one last check whether we can get the
	 * page bit synchronously.
	 *
	 * Do the folio_set_waiters() marking before that
	 * to let any waker we _just_ missed know they
	 * need to wake us up (otherwise they'll never
	 * even go to the slow case that looks at the
	 * page queue), and add ourselves to the wait
	 * queue if we need to sleep.
	 *
	 * This part needs to be done under the queue
	 * lock to avoid races.
	 */
	spin_lock_irq(&q->lock);
	folio_set_waiters(folio);
	if (!folio_trylock_flag(folio, bit_nr, wait))
		__add_wait_queue_entry_tail(q, wait);
	spin_unlock_irq(&q->lock);

	/*
	 * From now on, all the logic will be based on
	 * the WQ_FLAG_WOKEN and WQ_FLAG_DONE flag, to
	 * see whether the page bit testing has already
	 * been done by the wake function.
	 *
	 * We can drop our reference to the folio.
	 */
	if (behavior == DROP)
		folio_put(folio);

	/*
	 * Note that until the "finish_wait()", or until
	 * we see the WQ_FLAG_WOKEN flag, we need to
	 * be very careful with the 'wait->flags', because
	 * we may race with a waker that sets them.
	 */
	for (;;) {
		unsigned int flags;

		set_current_state(state);

		/* Loop until we've been woken or interrupted */
		flags = smp_load_acquire(&wait->flags);
		if (!(flags & WQ_FLAG_WOKEN)) {
			if (signal_pending_state(state, current))
				break;

			io_schedule();
			continue;
		}

		/* If we were non-exclusive, we're done */
		if (behavior != EXCLUSIVE)
			break;

		/* If the waker got the lock for us, we're done */
		if (flags & WQ_FLAG_DONE)
			break;

		/*
		 * Otherwise, if we're getting the lock, we need to
		 * try to get it ourselves.
		 *
		 * And if that fails, we'll have to retry this all.
		 */
		if (unlikely(test_and_set_bit(bit_nr, folio_flags(folio, 0))))
			goto repeat;

		wait->flags |= WQ_FLAG_DONE;
		break;
	}

	/*
	 * If a signal happened, this 'finish_wait()' may remove the last
	 * waiter from the wait-queues, but the folio waiters bit will remain
	 * set. That's ok. The next wakeup will take care of it, and trying
	 * to do it here would be difficult and prone to races.
	 */
	finish_wait(q, wait);

	if (thrashing) {
		delayacct_thrashing_end(&in_thrashing);
		psi_memstall_leave(&pflags);
	}

	/*
	 * NOTE! The wait->flags weren't stable until we've done the
	 * 'finish_wait()', and we could have exited the loop above due
	 * to a signal, and had a wakeup event happen after the signal
	 * test but before the 'finish_wait()'.
	 *
	 * So only after the finish_wait() can we reliably determine
	 * if we got woken up or not, so we can now figure out the final
	 * return value based on that state without races.
	 *
	 * Also note that WQ_FLAG_WOKEN is sufficient for a non-exclusive
	 * waiter, but an exclusive one requires WQ_FLAG_DONE.
	 */
	if (behavior == EXCLUSIVE)
		return wait->flags & WQ_FLAG_DONE ? 0 : -EINTR;

	return wait->flags & WQ_FLAG_WOKEN ? 0 : -EINTR;
}

#ifdef CONFIG_MIGRATION
/**
 * migration_entry_wait_on_locked - Wait for a migration entry to be removed
 * @entry: migration swap entry.
 * @ptl: already locked ptl. This function will drop the lock.
 *
 * Wait for a migration entry referencing the given page to be removed. This is
 * equivalent to put_and_wait_on_page_locked(page, TASK_UNINTERRUPTIBLE) except
 * this can be called without taking a reference on the page. Instead this
 * should be called while holding the ptl for the migration entry referencing
 * the page.
 *
 * Returns after unlocking the ptl.
 *
 * This follows the same logic as folio_wait_bit_common() so see the comments
 * there.
 */
void migration_entry_wait_on_locked(swp_entry_t entry, spinlock_t *ptl)
	__releases(ptl)
{
	struct wait_page_queue wait_page;
	wait_queue_entry_t *wait = &wait_page.wait;
	bool thrashing = false;
	unsigned long pflags;
	bool in_thrashing;
	wait_queue_head_t *q;
	struct folio *folio = pfn_swap_entry_folio(entry);

	q = folio_waitqueue(folio);
	if (!folio_test_uptodate(folio) && folio_test_workingset(folio)) {
		delayacct_thrashing_start(&in_thrashing);
		psi_memstall_enter(&pflags);
		thrashing = true;
	}

	init_wait(wait);
	wait->func = wake_page_function;
	wait_page.folio = folio;
	wait_page.bit_nr = PG_locked;
	wait->flags = 0;

	spin_lock_irq(&q->lock);
	folio_set_waiters(folio);
	if (!folio_trylock_flag(folio, PG_locked, wait))
		__add_wait_queue_entry_tail(q, wait);
	spin_unlock_irq(&q->lock);

	/*
	 * If a migration entry exists for the page the migration path must hold
	 * a valid reference to the page, and it must take the ptl to remove the
	 * migration entry. So the page is valid until the ptl is dropped.
	 */
	spin_unlock(ptl);

	for (;;) {
		unsigned int flags;

		set_current_state(TASK_UNINTERRUPTIBLE);

		/* Loop until we've been woken or interrupted */
		flags = smp_load_acquire(&wait->flags);
		if (!(flags & WQ_FLAG_WOKEN)) {
			if (signal_pending_state(TASK_UNINTERRUPTIBLE, current))
				break;

			io_schedule();
			continue;
		}
		break;
	}

	finish_wait(q, wait);

	if (thrashing) {
		delayacct_thrashing_end(&in_thrashing);
		psi_memstall_leave(&pflags);
	}
}
#endif

void folio_wait_bit(struct folio *folio, int bit_nr)
{
	folio_wait_bit_common(folio, bit_nr, TASK_UNINTERRUPTIBLE, SHARED);
}
EXPORT_SYMBOL(folio_wait_bit);

int folio_wait_bit_killable(struct folio *folio, int bit_nr)
{
	return folio_wait_bit_common(folio, bit_nr, TASK_KILLABLE, SHARED);
}
EXPORT_SYMBOL(folio_wait_bit_killable);

/**
 * folio_put_wait_locked - Drop a reference and wait for it to be unlocked
 * @folio: The folio to wait for.
 * @state: The sleep state (TASK_KILLABLE, TASK_UNINTERRUPTIBLE, etc).
 *
 * The caller should hold a reference on @folio.  They expect the page to
 * become unlocked relatively soon, but do not wish to hold up migration
 * (for example) by holding the reference while waiting for the folio to
 * come unlocked.  After this function returns, the caller should not
 * dereference @folio.
 *
 * Return: 0 if the folio was unlocked or -EINTR if interrupted by a signal.
 */
static int folio_put_wait_locked(struct folio *folio, int state)
{
	return folio_wait_bit_common(folio, PG_locked, state, DROP);
}

/**
 * folio_add_wait_queue - Add an arbitrary waiter to a folio's wait queue
 * @folio: Folio defining the wait queue of interest
 * @waiter: Waiter to add to the queue
 *
 * Add an arbitrary @waiter to the wait queue for the nominated @folio.
 */
void folio_add_wait_queue(struct folio *folio, wait_queue_entry_t *waiter)
{
	wait_queue_head_t *q = folio_waitqueue(folio);
	unsigned long flags;

	spin_lock_irqsave(&q->lock, flags);
	__add_wait_queue_entry_tail(q, waiter);
	folio_set_waiters(folio);
	spin_unlock_irqrestore(&q->lock, flags);
}
EXPORT_SYMBOL_GPL(folio_add_wait_queue);

/**
 * folio_unlock - Unlock a locked folio.
 * @folio: The folio.
 *
 * Unlocks the folio and wakes up any thread sleeping on the page lock.
 *
 * Context: May be called from interrupt or process context.  May not be
 * called from NMI context.
 */
void folio_unlock(struct folio *folio)
{
	/* Bit 7 allows x86 to check the byte's sign bit */
	BUILD_BUG_ON(PG_waiters != 7);
	BUILD_BUG_ON(PG_locked > 7);
	VM_BUG_ON_FOLIO(!folio_test_locked(folio), folio);
	if (folio_xor_flags_has_waiters(folio, 1 << PG_locked))
		folio_wake_bit(folio, PG_locked);
}
EXPORT_SYMBOL(folio_unlock);

/**
 * folio_end_read - End read on a folio.
 * @folio: The folio.
 * @success: True if all reads completed successfully.
 *
 * When all reads against a folio have completed, filesystems should
 * call this function to let the pagecache know that no more reads
 * are outstanding.  This will unlock the folio and wake up any thread
 * sleeping on the lock.  The folio will also be marked uptodate if all
 * reads succeeded.
 *
 * Context: May be called from interrupt or process context.  May not be
 * called from NMI context.
 */
void folio_end_read(struct folio *folio, bool success)
{
	unsigned long mask = 1 << PG_locked;

	/* Must be in bottom byte for x86 to work */
	BUILD_BUG_ON(PG_uptodate > 7);
	VM_BUG_ON_FOLIO(!folio_test_locked(folio), folio);
	VM_BUG_ON_FOLIO(folio_test_uptodate(folio), folio);

	if (likely(success))
		mask |= 1 << PG_uptodate;
	if (folio_xor_flags_has_waiters(folio, mask))
		folio_wake_bit(folio, PG_locked);
}
EXPORT_SYMBOL(folio_end_read);

/**
 * folio_end_private_2 - Clear PG_private_2 and wake any waiters.
 * @folio: The folio.
 *
 * Clear the PG_private_2 bit on a folio and wake up any sleepers waiting for
 * it.  The folio reference held for PG_private_2 being set is released.
 *
 * This is, for example, used when a netfs folio is being written to a local
 * disk cache, thereby allowing writes to the cache for the same folio to be
 * serialised.
 */
void folio_end_private_2(struct folio *folio)
{
	VM_BUG_ON_FOLIO(!folio_test_private_2(folio), folio);
	clear_bit_unlock(PG_private_2, folio_flags(folio, 0));
	folio_wake_bit(folio, PG_private_2);
	folio_put(folio);
}
EXPORT_SYMBOL(folio_end_private_2);

/**
 * folio_wait_private_2 - Wait for PG_private_2 to be cleared on a folio.
 * @folio: The folio to wait on.
 *
 * Wait for PG_private_2 (aka PG_fscache) to be cleared on a folio.
 */
void folio_wait_private_2(struct folio *folio)
{
	while (folio_test_private_2(folio))
		folio_wait_bit(folio, PG_private_2);
}
EXPORT_SYMBOL(folio_wait_private_2);

/**
 * folio_wait_private_2_killable - Wait for PG_private_2 to be cleared on a folio.
 * @folio: The folio to wait on.
 *
 * Wait for PG_private_2 (aka PG_fscache) to be cleared on a folio or until a
 * fatal signal is received by the calling task.
 *
 * Return:
 * - 0 if successful.
 * - -EINTR if a fatal signal was encountered.
 */
int folio_wait_private_2_killable(struct folio *folio)
{
	int ret = 0;

	while (folio_test_private_2(folio)) {
		ret = folio_wait_bit_killable(folio, PG_private_2);
		if (ret < 0)
			break;
	}

	return ret;
}
EXPORT_SYMBOL(folio_wait_private_2_killable);

/**
 * folio_end_writeback - End writeback against a folio.
 * @folio: The folio.
 *
 * The folio must actually be under writeback.
 *
 * Context: May be called from process or interrupt context.
 */
void folio_end_writeback(struct folio *folio)
{
	VM_BUG_ON_FOLIO(!folio_test_writeback(folio), folio);

	/*
	 * folio_test_clear_reclaim() could be used here but it is an
	 * atomic operation and overkill in this particular case. Failing
	 * to shuffle a folio marked for immediate reclaim is too mild
	 * a gain to justify taking an atomic operation penalty at the
	 * end of every folio writeback.
	 */
	if (folio_test_reclaim(folio)) {
		folio_clear_reclaim(folio);
		folio_rotate_reclaimable(folio);
	}

	/*
	 * Writeback does not hold a folio reference of its own, relying
	 * on truncation to wait for the clearing of PG_writeback.
	 * But here we must make sure that the folio is not freed and
	 * reused before the folio_wake_bit().
	 */
	folio_get(folio);
	if (__folio_end_writeback(folio))
		folio_wake_bit(folio, PG_writeback);
	acct_reclaim_writeback(folio);
	folio_put(folio);
}
EXPORT_SYMBOL(folio_end_writeback);

/**
 * __folio_lock - Get a lock on the folio, assuming we need to sleep to get it.
 * @folio: The folio to lock
 */
void __folio_lock(struct folio *folio)
{
	folio_wait_bit_common(folio, PG_locked, TASK_UNINTERRUPTIBLE,
				EXCLUSIVE);
}
EXPORT_SYMBOL(__folio_lock);

int __folio_lock_killable(struct folio *folio)
{
	return folio_wait_bit_common(folio, PG_locked, TASK_KILLABLE,
					EXCLUSIVE);
}
EXPORT_SYMBOL_GPL(__folio_lock_killable);

static int __folio_lock_async(struct folio *folio, struct wait_page_queue *wait)
{
	struct wait_queue_head *q = folio_waitqueue(folio);
	int ret;

	wait->folio = folio;
	wait->bit_nr = PG_locked;

	spin_lock_irq(&q->lock);
	__add_wait_queue_entry_tail(q, &wait->wait);
	folio_set_waiters(folio);
	ret = !folio_trylock(folio);
	/*
	 * If we were successful now, we know we're still on the
	 * waitqueue as we're still under the lock. This means it's
	 * safe to remove and return success, we know the callback
	 * isn't going to trigger.
	 */
	if (!ret)
		__remove_wait_queue(q, &wait->wait);
	else
		ret = -EIOCBQUEUED;
	spin_unlock_irq(&q->lock);
	return ret;
}

/*
 * Return values:
 * 0 - folio is locked.
 * non-zero - folio is not locked.
 *     mmap_lock or per-VMA lock has been released (mmap_read_unlock() or
 *     vma_end_read()), unless flags had both FAULT_FLAG_ALLOW_RETRY and
 *     FAULT_FLAG_RETRY_NOWAIT set, in which case the lock is still held.
 *
 * If neither ALLOW_RETRY nor KILLABLE are set, will always return 0
 * with the folio locked and the mmap_lock/per-VMA lock is left unperturbed.
 */
vm_fault_t __folio_lock_or_retry(struct folio *folio, struct vm_fault *vmf)
{
	unsigned int flags = vmf->flags;

	if (fault_flag_allow_retry_first(flags)) {
		/*
		 * CAUTION! In this case, mmap_lock/per-VMA lock is not
		 * released even though returning VM_FAULT_RETRY.
		 */
		if (flags & FAULT_FLAG_RETRY_NOWAIT)
			return VM_FAULT_RETRY;

		release_fault_lock(vmf);
		if (flags & FAULT_FLAG_KILLABLE)
			folio_wait_locked_killable(folio);
		else
			folio_wait_locked(folio);
		return VM_FAULT_RETRY;
	}
	if (flags & FAULT_FLAG_KILLABLE) {
		bool ret;

		ret = __folio_lock_killable(folio);
		if (ret) {
			release_fault_lock(vmf);
			return VM_FAULT_RETRY;
		}
	} else {
		__folio_lock(folio);
	}

	return 0;
}

/**
 * page_cache_next_miss() - Find the next gap in the page cache.
 * @mapping: Mapping.
 * @index: Index.
 * @max_scan: Maximum range to search.
 *
 * Search the range [index, min(index + max_scan - 1, ULONG_MAX)] for the
 * gap with the lowest index.
 *
 * This function may be called under the rcu_read_lock.  However, this will
 * not atomically search a snapshot of the cache at a single point in time.
 * For example, if a gap is created at index 5, then subsequently a gap is
 * created at index 10, page_cache_next_miss covering both indices may
 * return 10 if called under the rcu_read_lock.
 *
 * Return: The index of the gap if found, otherwise an index outside the
 * range specified (in which case 'return - index >= max_scan' will be true).
 * In the rare case of index wrap-around, 0 will be returned.
 */
pgoff_t page_cache_next_miss(struct address_space *mapping,
			     pgoff_t index, unsigned long max_scan)
{
	XA_STATE(xas, &mapping->i_pages, index);

	while (max_scan--) {
		void *entry = xas_next(&xas);
		if (!entry || xa_is_value(entry))
			break;
		if (xas.xa_index == 0)
			break;
	}

	return xas.xa_index;
}
EXPORT_SYMBOL(page_cache_next_miss);

/**
 * page_cache_prev_miss() - Find the previous gap in the page cache.
 * @mapping: Mapping.
 * @index: Index.
 * @max_scan: Maximum range to search.
 *
 * Search the range [max(index - max_scan + 1, 0), index] for the
 * gap with the highest index.
 *
 * This function may be called under the rcu_read_lock.  However, this will
 * not atomically search a snapshot of the cache at a single point in time.
 * For example, if a gap is created at index 10, then subsequently a gap is
 * created at index 5, page_cache_prev_miss() covering both indices may
 * return 5 if called under the rcu_read_lock.
 *
 * Return: The index of the gap if found, otherwise an index outside the
 * range specified (in which case 'index - return >= max_scan' will be true).
 * In the rare case of wrap-around, ULONG_MAX will be returned.
 */
pgoff_t page_cache_prev_miss(struct address_space *mapping,
			     pgoff_t index, unsigned long max_scan)
{
	XA_STATE(xas, &mapping->i_pages, index);

	while (max_scan--) {
		void *entry = xas_prev(&xas);
		if (!entry || xa_is_value(entry))
			break;
		if (xas.xa_index == ULONG_MAX)
			break;
	}

	return xas.xa_index;
}
EXPORT_SYMBOL(page_cache_prev_miss);

/*
 * Lockless page cache protocol:
 * On the lookup side:
 * 1. Load the folio from i_pages
 * 2. Increment the refcount if it's not zero
 * 3. If the folio is not found by xas_reload(), put the refcount and retry
 *
 * On the removal side:
 * A. Freeze the page (by zeroing the refcount if nobody else has a reference)
 * B. Remove the page from i_pages
 * C. Return the page to the page allocator
 *
 * This means that any page may have its reference count temporarily
 * increased by a speculative page cache (or fast GUP) lookup as it can
 * be allocated by another user before the RCU grace period expires.
 * Because the refcount temporarily acquired here may end up being the
 * last refcount on the page, any page allocation must be freeable by
 * folio_put().
 */

/*
 * filemap_get_entry - Get a page cache entry.
 * @mapping: the address_space to search
 * @index: The page cache index.
 *
 * Looks up the page cache entry at @mapping & @index.  If it is a folio,
 * it is returned with an increased refcount.  If it is a shadow entry
 * of a previously evicted folio, or a swap entry from shmem/tmpfs,
 * it is returned without further action.
 *
 * Return: The folio, swap or shadow entry, %NULL if nothing is found.
 */
void *filemap_get_entry(struct address_space *mapping, pgoff_t index)
{
	XA_STATE(xas, &mapping->i_pages, index);
	struct folio *folio;

	rcu_read_lock();
repeat:
	xas_reset(&xas);
	folio = xas_load(&xas);
	if (xas_retry(&xas, folio))
		goto repeat;
	/*
	 * A shadow entry of a recently evicted page, or a swap entry from
	 * shmem/tmpfs.  Return it without attempting to raise page count.
	 */
	if (!folio || xa_is_value(folio))
		goto out;

	if (!folio_try_get_rcu(folio))
		goto repeat;

	if (unlikely(folio != xas_reload(&xas))) {
		folio_put(folio);
		goto repeat;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
out:
	rcu_read_unlock();

<<<<<<< HEAD
	return page;
}
EXPORT_SYMBOL(find_get_page);

/**
 * find_lock_page - locate, pin and lock a pagecache page
 * @mapping: the address_space to search
 * @offset: the page index
 *
 * Locates the desired pagecache page, locks it, increments its reference
 * count and returns its address.
 *
 * Returns zero if the page was not present. find_lock_page() may sleep.
 */
struct page *find_lock_page(struct address_space *mapping, pgoff_t offset)
{
	struct page *page;

repeat:
	page = find_get_page(mapping, offset);
	if (page && !radix_tree_exception(page)) {
		lock_page(page);
		/* Has the page been truncated? */
		if (unlikely(page->mapping != mapping)) {
			unlock_page(page);
			page_cache_release(page);
			goto repeat;
		}
		VM_BUG_ON(page->index != offset);
	}
	return page;
}
EXPORT_SYMBOL(find_lock_page);

/**
 * find_or_create_page - locate or add a pagecache page
 * @mapping: the page's address_space
 * @index: the page's index into the mapping
 * @gfp_mask: page allocation mode
 *
 * Locates a page in the pagecache.  If the page is not present, a new page
 * is allocated using @gfp_mask and is added to the pagecache and to the VM's
 * LRU list.  The returned page is locked and has its reference count
 * incremented.
 *
 * find_or_create_page() may sleep, even if @gfp_flags specifies an atomic
 * allocation!
 *
 * find_or_create_page() returns the desired page's address, or zero on
 * memory exhaustion.
 */
struct page *find_or_create_page(struct address_space *mapping,
		pgoff_t index, gfp_t gfp_mask)
{
	struct page *page;
	int err;
	//[LGE_S] (2014.04.15) Avoid ion memory allocation failure when Miracast connects
	gfp_t gfp_notmask = 0;
	//[LGE_E] (2014.04.15) Avoid ion memory allocation failure when Miracast connects

repeat:
	page = find_lock_page(mapping, index);
	//[LGE_S] (2014.04.15) Avoid ion memory allocation failure when Miracast connects
	if (!page) {
	retry:
		page = __page_cache_alloc(gfp_mask & ~gfp_notmask);
		if (!page)
			return NULL;
		if (is_cma_pageblock(page)) {
			__free_page(page);
			gfp_notmask |= __GFP_MOVABLE;
			goto retry;
		}
 	//[LGE_E] (2014.04.15) Avoid ion memory allocation failure when Miracast connects

		/*
		 * We want a regular kernel memory (not highmem or DMA etc)
		 * allocation for the radix tree nodes, but we need to honour
		 * the context-specific requirements the caller has asked for.
		 * GFP_RECLAIM_MASK collects those requirements.
		 */
		err = add_to_page_cache_lru(page, mapping, index,
			(gfp_mask & GFP_RECLAIM_MASK));
		if (unlikely(err)) {
			page_cache_release(page);
			page = NULL;
			if (err == -EEXIST)
				goto repeat;
		}
	}
	return page;
}
EXPORT_SYMBOL(find_or_create_page);

/**
 * find_get_pages - gang pagecache lookup
 * @mapping:	The address_space to search
 * @start:	The starting page index
 * @nr_pages:	The maximum number of pages
 * @pages:	Where the resulting pages are placed
 *
 * find_get_pages() will search for and return a group of up to
 * @nr_pages pages in the mapping.  The pages are placed at @pages.
 * find_get_pages() takes a reference against the returned pages.
 *
 * The search returns a group of mapping-contiguous pages with ascending
 * indexes.  There may be holes in the indices due to not-present pages.
 *
 * find_get_pages() returns the number of pages which were found.
 */
unsigned find_get_pages(struct address_space *mapping, pgoff_t start,
			    unsigned int nr_pages, struct page **pages)
{
	struct radix_tree_iter iter;
	void **slot;
	unsigned ret = 0;

	if (unlikely(!nr_pages))
		return 0;

	rcu_read_lock();
restart:
	radix_tree_for_each_slot(slot, &mapping->page_tree, &iter, start) {
		struct page *page;
repeat:
		page = radix_tree_deref_slot(slot);
		if (unlikely(!page))
			continue;

		if (radix_tree_exception(page)) {
			if (radix_tree_deref_retry(page)) {
				/*
				 * Transient condition which can only trigger
				 * when entry at index 0 moves out of or back
				 * to root: none yet gotten, safe to restart.
				 */
				WARN_ON(iter.index);
				goto restart;
			}
			/*
			 * Otherwise, shmem/tmpfs must be storing a swap entry
			 * here as an exceptional entry: so skip over it -
			 * we only reach this from invalidate_mapping_pages().
			 */
			continue;
		}

		if (!page_cache_get_speculative(page))
			goto repeat;

		/* Has the page moved? */
		if (unlikely(page != *slot)) {
			page_cache_release(page);
			goto repeat;
		}

		pages[ret] = page;
		if (++ret == nr_pages)
			break;
	}

	rcu_read_unlock();
	return ret;
}

/**
 * find_get_pages_contig - gang contiguous pagecache lookup
 * @mapping:	The address_space to search
 * @index:	The starting page index
 * @nr_pages:	The maximum number of pages
 * @pages:	Where the resulting pages are placed
 *
 * find_get_pages_contig() works exactly like find_get_pages(), except
 * that the returned number of pages are guaranteed to be contiguous.
 *
 * find_get_pages_contig() returns the number of pages which were found.
 */
unsigned find_get_pages_contig(struct address_space *mapping, pgoff_t index,
			       unsigned int nr_pages, struct page **pages)
{
	struct radix_tree_iter iter;
	void **slot;
	unsigned int ret = 0;

	if (unlikely(!nr_pages))
		return 0;

	rcu_read_lock();
restart:
	radix_tree_for_each_contig(slot, &mapping->page_tree, &iter, index) {
		struct page *page;
repeat:
		page = radix_tree_deref_slot(slot);
		/* The hole, there no reason to continue */
		if (unlikely(!page))
			break;

		if (radix_tree_exception(page)) {
			if (radix_tree_deref_retry(page)) {
				/*
				 * Transient condition which can only trigger
				 * when entry at index 0 moves out of or back
				 * to root: none yet gotten, safe to restart.
				 */
				goto restart;
			}
			/*
			 * Otherwise, shmem/tmpfs must be storing a swap entry
			 * here as an exceptional entry: so stop looking for
			 * contiguous pages.
			 */
			break;
		}

		if (!page_cache_get_speculative(page))
			goto repeat;

		/* Has the page moved? */
		if (unlikely(page != *slot)) {
			page_cache_release(page);
			goto repeat;
		}

		/*
		 * must check mapping and index after taking the ref.
		 * otherwise we can get both false positives and false
		 * negatives, which is just confusing to the caller.
		 */
		if (page->mapping == NULL || page->index != iter.index) {
			page_cache_release(page);
			break;
		}

		pages[ret] = page;
		if (++ret == nr_pages)
			break;
	}
	rcu_read_unlock();
	return ret;
}
EXPORT_SYMBOL(find_get_pages_contig);

/**
 * find_get_pages_tag - find and return pages that match @tag
 * @mapping:	the address_space to search
 * @index:	the starting page index
 * @tag:	the tag index
 * @nr_pages:	the maximum number of pages
 * @pages:	where the resulting pages are placed
 *
 * Like find_get_pages, except we only return pages which are tagged with
 * @tag.   We update @index to index the next page for the traversal.
 */
unsigned find_get_pages_tag(struct address_space *mapping, pgoff_t *index,
			int tag, unsigned int nr_pages, struct page **pages)
{
	struct radix_tree_iter iter;
	void **slot;
	unsigned ret = 0;

	if (unlikely(!nr_pages))
		return 0;

	rcu_read_lock();
restart:
	radix_tree_for_each_tagged(slot, &mapping->page_tree,
				   &iter, *index, tag) {
		struct page *page;
repeat:
		page = radix_tree_deref_slot(slot);
		if (unlikely(!page))
			continue;

		if (radix_tree_exception(page)) {
			if (radix_tree_deref_retry(page)) {
				/*
				 * Transient condition which can only trigger
				 * when entry at index 0 moves out of or back
				 * to root: none yet gotten, safe to restart.
				 */
				goto restart;
			}
			/*
			 * This function is never used on a shmem/tmpfs
			 * mapping, so a swap entry won't be found here.
			 */
			BUG();
		}

		if (!page_cache_get_speculative(page))
			goto repeat;

		/* Has the page moved? */
		if (unlikely(page != *slot)) {
			page_cache_release(page);
			goto repeat;
		}

		pages[ret] = page;
		if (++ret == nr_pages)
			break;
	}

	rcu_read_unlock();

	if (ret)
		*index = pages[ret - 1]->index + 1;

	return ret;
}
EXPORT_SYMBOL(find_get_pages_tag);

/**
 * grab_cache_page_nowait - returns locked page at given index in given cache
 * @mapping: target address_space
 * @index: the page index
 *
 * Same as grab_cache_page(), but do not wait if the page is unavailable.
 * This is intended for speculative data generators, where the data can
 * be regenerated if the page couldn't be grabbed.  This routine should
 * be safe to call while holding the lock for another page.
 *
 * Clear __GFP_FS when allocating the page to avoid recursion into the fs
 * and deadlock against the caller's locked page.
 */
struct page *
grab_cache_page_nowait(struct address_space *mapping, pgoff_t index)
{
	struct page *page = find_get_page(mapping, index);

	if (page) {
		if (trylock_page(page))
			return page;
		page_cache_release(page);
		return NULL;
	}
	page = __page_cache_alloc(mapping_gfp_mask(mapping) & ~__GFP_FS);
	if (page && add_to_page_cache_lru(page, mapping, index, GFP_NOFS)) {
		page_cache_release(page);
		page = NULL;
	}
	return page;
}
EXPORT_SYMBOL(grab_cache_page_nowait);
=======
	return folio;
}

/**
 * __filemap_get_folio - Find and get a reference to a folio.
 * @mapping: The address_space to search.
 * @index: The page index.
 * @fgp_flags: %FGP flags modify how the folio is returned.
 * @gfp: Memory allocation flags to use if %FGP_CREAT is specified.
 *
 * Looks up the page cache entry at @mapping & @index.
 *
 * If %FGP_LOCK or %FGP_CREAT are specified then the function may sleep even
 * if the %GFP flags specified for %FGP_CREAT are atomic.
 *
 * If this function returns a folio, it is returned with an increased refcount.
 *
 * Return: The found folio or an ERR_PTR() otherwise.
 */
struct folio *__filemap_get_folio(struct address_space *mapping, pgoff_t index,
		fgf_t fgp_flags, gfp_t gfp)
{
	struct folio *folio;

repeat:
	folio = filemap_get_entry(mapping, index);
	if (xa_is_value(folio))
		folio = NULL;
	if (!folio)
		goto no_page;

	if (fgp_flags & FGP_LOCK) {
		if (fgp_flags & FGP_NOWAIT) {
			if (!folio_trylock(folio)) {
				folio_put(folio);
				return ERR_PTR(-EAGAIN);
			}
		} else {
			folio_lock(folio);
		}

		/* Has the page been truncated? */
		if (unlikely(folio->mapping != mapping)) {
			folio_unlock(folio);
			folio_put(folio);
			goto repeat;
		}
		VM_BUG_ON_FOLIO(!folio_contains(folio, index), folio);
	}

	if (fgp_flags & FGP_ACCESSED)
		folio_mark_accessed(folio);
	else if (fgp_flags & FGP_WRITE) {
		/* Clear idle flag for buffer write */
		if (folio_test_idle(folio))
			folio_clear_idle(folio);
	}

	if (fgp_flags & FGP_STABLE)
		folio_wait_stable(folio);
no_page:
	if (!folio && (fgp_flags & FGP_CREAT)) {
		unsigned order = FGF_GET_ORDER(fgp_flags);
		int err;

		if ((fgp_flags & FGP_WRITE) && mapping_can_writeback(mapping))
			gfp |= __GFP_WRITE;
		if (fgp_flags & FGP_NOFS)
			gfp &= ~__GFP_FS;
		if (fgp_flags & FGP_NOWAIT) {
			gfp &= ~GFP_KERNEL;
			gfp |= GFP_NOWAIT | __GFP_NOWARN;
		}
		if (WARN_ON_ONCE(!(fgp_flags & (FGP_LOCK | FGP_FOR_MMAP))))
			fgp_flags |= FGP_LOCK;

		if (!mapping_large_folio_support(mapping))
			order = 0;
		if (order > MAX_PAGECACHE_ORDER)
			order = MAX_PAGECACHE_ORDER;
		/* If we're not aligned, allocate a smaller folio */
		if (index & ((1UL << order) - 1))
			order = __ffs(index);

		do {
			gfp_t alloc_gfp = gfp;

			err = -ENOMEM;
			if (order > 0)
				alloc_gfp |= __GFP_NORETRY | __GFP_NOWARN;
			folio = filemap_alloc_folio(alloc_gfp, order);
			if (!folio)
				continue;

			/* Init accessed so avoid atomic mark_page_accessed later */
			if (fgp_flags & FGP_ACCESSED)
				__folio_set_referenced(folio);

			err = filemap_add_folio(mapping, folio, index, gfp);
			if (!err)
				break;
			folio_put(folio);
			folio = NULL;
		} while (order-- > 0);

		if (err == -EEXIST)
			goto repeat;
		if (err)
			return ERR_PTR(err);
		/*
		 * filemap_add_folio locks the page, and for mmap
		 * we expect an unlocked page.
		 */
		if (folio && (fgp_flags & FGP_FOR_MMAP))
			folio_unlock(folio);
	}

	if (!folio)
		return ERR_PTR(-ENOENT);
	return folio;
}
EXPORT_SYMBOL(__filemap_get_folio);

static inline struct folio *find_get_entry(struct xa_state *xas, pgoff_t max,
		xa_mark_t mark)
{
	struct folio *folio;

retry:
	if (mark == XA_PRESENT)
		folio = xas_find(xas, max);
	else
		folio = xas_find_marked(xas, max, mark);

	if (xas_retry(xas, folio))
		goto retry;
	/*
	 * A shadow entry of a recently evicted page, a swap
	 * entry from shmem/tmpfs or a DAX entry.  Return it
	 * without attempting to raise page count.
	 */
	if (!folio || xa_is_value(folio))
		return folio;

	if (!folio_try_get_rcu(folio))
		goto reset;

	if (unlikely(folio != xas_reload(xas))) {
		folio_put(folio);
		goto reset;
	}

	return folio;
reset:
	xas_reset(xas);
	goto retry;
}

/**
 * find_get_entries - gang pagecache lookup
 * @mapping:	The address_space to search
 * @start:	The starting page cache index
 * @end:	The final page index (inclusive).
 * @fbatch:	Where the resulting entries are placed.
 * @indices:	The cache indices corresponding to the entries in @entries
 *
 * find_get_entries() will search for and return a batch of entries in
 * the mapping.  The entries are placed in @fbatch.  find_get_entries()
 * takes a reference on any actual folios it returns.
 *
 * The entries have ascending indexes.  The indices may not be consecutive
 * due to not-present entries or large folios.
 *
 * Any shadow entries of evicted folios, or swap entries from
 * shmem/tmpfs, are included in the returned array.
 *
 * Return: The number of entries which were found.
 */
unsigned find_get_entries(struct address_space *mapping, pgoff_t *start,
		pgoff_t end, struct folio_batch *fbatch, pgoff_t *indices)
{
	XA_STATE(xas, &mapping->i_pages, *start);
	struct folio *folio;

	rcu_read_lock();
	while ((folio = find_get_entry(&xas, end, XA_PRESENT)) != NULL) {
		indices[fbatch->nr] = xas.xa_index;
		if (!folio_batch_add(fbatch, folio))
			break;
	}
	rcu_read_unlock();

	if (folio_batch_count(fbatch)) {
		unsigned long nr = 1;
		int idx = folio_batch_count(fbatch) - 1;

		folio = fbatch->folios[idx];
		if (!xa_is_value(folio))
			nr = folio_nr_pages(folio);
		*start = indices[idx] + nr;
	}
	return folio_batch_count(fbatch);
}

/**
 * find_lock_entries - Find a batch of pagecache entries.
 * @mapping:	The address_space to search.
 * @start:	The starting page cache index.
 * @end:	The final page index (inclusive).
 * @fbatch:	Where the resulting entries are placed.
 * @indices:	The cache indices of the entries in @fbatch.
 *
 * find_lock_entries() will return a batch of entries from @mapping.
 * Swap, shadow and DAX entries are included.  Folios are returned
 * locked and with an incremented refcount.  Folios which are locked
 * by somebody else or under writeback are skipped.  Folios which are
 * partially outside the range are not returned.
 *
 * The entries have ascending indexes.  The indices may not be consecutive
 * due to not-present entries, large folios, folios which could not be
 * locked or folios under writeback.
 *
 * Return: The number of entries which were found.
 */
unsigned find_lock_entries(struct address_space *mapping, pgoff_t *start,
		pgoff_t end, struct folio_batch *fbatch, pgoff_t *indices)
{
	XA_STATE(xas, &mapping->i_pages, *start);
	struct folio *folio;

	rcu_read_lock();
	while ((folio = find_get_entry(&xas, end, XA_PRESENT))) {
		if (!xa_is_value(folio)) {
			if (folio->index < *start)
				goto put;
			if (folio_next_index(folio) - 1 > end)
				goto put;
			if (!folio_trylock(folio))
				goto put;
			if (folio->mapping != mapping ||
			    folio_test_writeback(folio))
				goto unlock;
			VM_BUG_ON_FOLIO(!folio_contains(folio, xas.xa_index),
					folio);
		}
		indices[fbatch->nr] = xas.xa_index;
		if (!folio_batch_add(fbatch, folio))
			break;
		continue;
unlock:
		folio_unlock(folio);
put:
		folio_put(folio);
	}
	rcu_read_unlock();

	if (folio_batch_count(fbatch)) {
		unsigned long nr = 1;
		int idx = folio_batch_count(fbatch) - 1;

		folio = fbatch->folios[idx];
		if (!xa_is_value(folio))
			nr = folio_nr_pages(folio);
		*start = indices[idx] + nr;
	}
	return folio_batch_count(fbatch);
}

/**
 * filemap_get_folios - Get a batch of folios
 * @mapping:	The address_space to search
 * @start:	The starting page index
 * @end:	The final page index (inclusive)
 * @fbatch:	The batch to fill.
 *
 * Search for and return a batch of folios in the mapping starting at
 * index @start and up to index @end (inclusive).  The folios are returned
 * in @fbatch with an elevated reference count.
 *
 * Return: The number of folios which were found.
 * We also update @start to index the next folio for the traversal.
 */
unsigned filemap_get_folios(struct address_space *mapping, pgoff_t *start,
		pgoff_t end, struct folio_batch *fbatch)
{
	return filemap_get_folios_tag(mapping, start, end, XA_PRESENT, fbatch);
}
EXPORT_SYMBOL(filemap_get_folios);

/**
 * filemap_get_folios_contig - Get a batch of contiguous folios
 * @mapping:	The address_space to search
 * @start:	The starting page index
 * @end:	The final page index (inclusive)
 * @fbatch:	The batch to fill
 *
 * filemap_get_folios_contig() works exactly like filemap_get_folios(),
 * except the returned folios are guaranteed to be contiguous. This may
 * not return all contiguous folios if the batch gets filled up.
 *
 * Return: The number of folios found.
 * Also update @start to be positioned for traversal of the next folio.
 */

unsigned filemap_get_folios_contig(struct address_space *mapping,
		pgoff_t *start, pgoff_t end, struct folio_batch *fbatch)
{
	XA_STATE(xas, &mapping->i_pages, *start);
	unsigned long nr;
	struct folio *folio;

	rcu_read_lock();

	for (folio = xas_load(&xas); folio && xas.xa_index <= end;
			folio = xas_next(&xas)) {
		if (xas_retry(&xas, folio))
			continue;
		/*
		 * If the entry has been swapped out, we can stop looking.
		 * No current caller is looking for DAX entries.
		 */
		if (xa_is_value(folio))
			goto update_start;

		if (!folio_try_get_rcu(folio))
			goto retry;

		if (unlikely(folio != xas_reload(&xas)))
			goto put_folio;

		if (!folio_batch_add(fbatch, folio)) {
			nr = folio_nr_pages(folio);
			*start = folio->index + nr;
			goto out;
		}
		continue;
put_folio:
		folio_put(folio);

retry:
		xas_reset(&xas);
	}

update_start:
	nr = folio_batch_count(fbatch);

	if (nr) {
		folio = fbatch->folios[nr - 1];
		*start = folio_next_index(folio);
	}
out:
	rcu_read_unlock();
	return folio_batch_count(fbatch);
}
EXPORT_SYMBOL(filemap_get_folios_contig);

/**
 * filemap_get_folios_tag - Get a batch of folios matching @tag
 * @mapping:    The address_space to search
 * @start:      The starting page index
 * @end:        The final page index (inclusive)
 * @tag:        The tag index
 * @fbatch:     The batch to fill
 *
 * The first folio may start before @start; if it does, it will contain
 * @start.  The final folio may extend beyond @end; if it does, it will
 * contain @end.  The folios have ascending indices.  There may be gaps
 * between the folios if there are indices which have no folio in the
 * page cache.  If folios are added to or removed from the page cache
 * while this is running, they may or may not be found by this call.
 * Only returns folios that are tagged with @tag.
 *
 * Return: The number of folios found.
 * Also update @start to index the next folio for traversal.
 */
unsigned filemap_get_folios_tag(struct address_space *mapping, pgoff_t *start,
			pgoff_t end, xa_mark_t tag, struct folio_batch *fbatch)
{
	XA_STATE(xas, &mapping->i_pages, *start);
	struct folio *folio;

	rcu_read_lock();
	while ((folio = find_get_entry(&xas, end, tag)) != NULL) {
		/*
		 * Shadow entries should never be tagged, but this iteration
		 * is lockless so there is a window for page reclaim to evict
		 * a page we saw tagged. Skip over it.
		 */
		if (xa_is_value(folio))
			continue;
		if (!folio_batch_add(fbatch, folio)) {
			unsigned long nr = folio_nr_pages(folio);
			*start = folio->index + nr;
			goto out;
		}
	}
	/*
	 * We come here when there is no page beyond @end. We take care to not
	 * overflow the index @start as it confuses some of the callers. This
	 * breaks the iteration when there is a page at index -1 but that is
	 * already broke anyway.
	 */
	if (end == (pgoff_t)-1)
		*start = (pgoff_t)-1;
	else
		*start = end + 1;
out:
	rcu_read_unlock();

	return folio_batch_count(fbatch);
}
EXPORT_SYMBOL(filemap_get_folios_tag);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * CD/DVDs are error prone. When a medium error occurs, the driver may fail
 * a _large_ part of the i/o request. Imagine the worst scenario:
 *
 *      ---R__________________________________________B__________
 *         ^ reading here                             ^ bad block(assume 4k)
 *
 * read(R) => miss => readahead(R...B) => media error => frustrating retries
 * => failing the whole request => read(R) => read(R+1) =>
 * readahead(R+1...B+1) => bang => read(R+2) => read(R+3) =>
 * readahead(R+3...B+2) => bang => read(R+3) => read(R+4) =>
 * readahead(R+4...B+3) => bang => read(R+4) => read(R+5) => ......
 *
 * It is going insane. Fix it by quickly scaling down the readahead size.
 */
<<<<<<< HEAD
static void shrink_readahead_size_eio(struct file *filp,
					struct file_ra_state *ra)
=======
static void shrink_readahead_size_eio(struct file_ra_state *ra)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	ra->ra_pages /= 4;
}

<<<<<<< HEAD
/**
 * do_generic_file_read - generic file read routine
 * @filp:	the file to read
 * @ppos:	current file position
 * @desc:	read_descriptor
 * @actor:	read method
 *
 * This is a generic file read routine, and uses the
 * mapping->a_ops->readpage() function for the actual low-level stuff.
 *
 * This is really ugly. But the goto's actually try to clarify some
 * of the logic when it comes to error handling etc.
 */
static void do_generic_file_read(struct file *filp, loff_t *ppos,
		read_descriptor_t *desc, read_actor_t actor)
{
	struct address_space *mapping = filp->f_mapping;
	struct inode *inode = mapping->host;
	struct file_ra_state *ra = &filp->f_ra;
	pgoff_t index;
	pgoff_t last_index;
	pgoff_t prev_index;
	unsigned long offset;      /* offset into pagecache page */
	unsigned int prev_offset;
	int error;

	index = *ppos >> PAGE_CACHE_SHIFT;
	prev_index = ra->prev_pos >> PAGE_CACHE_SHIFT;
	prev_offset = ra->prev_pos & (PAGE_CACHE_SIZE-1);
	last_index = (*ppos + desc->count + PAGE_CACHE_SIZE-1) >> PAGE_CACHE_SHIFT;
	offset = *ppos & ~PAGE_CACHE_MASK;

	for (;;) {
		struct page *page;
		pgoff_t end_index;
		loff_t isize;
		unsigned long nr, ret;

		cond_resched();
find_page:
		page = find_get_page(mapping, index);
		if (!page) {
			page_cache_sync_readahead(mapping,
					ra, filp,
					index, last_index - index);
			page = find_get_page(mapping, index);
			if (unlikely(page == NULL))
				goto no_cached_page;
		}
		if (PageReadahead(page)) {
			page_cache_async_readahead(mapping,
					ra, filp, page,
					index, last_index - index);
		}
		if (!PageUptodate(page)) {
			if (inode->i_blkbits == PAGE_CACHE_SHIFT ||
					!mapping->a_ops->is_partially_uptodate)
				goto page_not_up_to_date;
			if (!trylock_page(page))
				goto page_not_up_to_date;
			/* Did it get truncated before we got the lock? */
			if (!page->mapping)
				goto page_not_up_to_date_locked;
			if (!mapping->a_ops->is_partially_uptodate(page,
								desc, offset))
				goto page_not_up_to_date_locked;
			unlock_page(page);
		}
page_ok:
		/*
		 * i_size must be checked after we know the page is Uptodate.
=======
/*
 * filemap_get_read_batch - Get a batch of folios for read
 *
 * Get a batch of folios which represent a contiguous range of bytes in
 * the file.  No exceptional entries will be returned.  If @index is in
 * the middle of a folio, the entire folio will be returned.  The last
 * folio in the batch may have the readahead flag set or the uptodate flag
 * clear so that the caller can take the appropriate action.
 */
static void filemap_get_read_batch(struct address_space *mapping,
		pgoff_t index, pgoff_t max, struct folio_batch *fbatch)
{
	XA_STATE(xas, &mapping->i_pages, index);
	struct folio *folio;

	rcu_read_lock();
	for (folio = xas_load(&xas); folio; folio = xas_next(&xas)) {
		if (xas_retry(&xas, folio))
			continue;
		if (xas.xa_index > max || xa_is_value(folio))
			break;
		if (xa_is_sibling(folio))
			break;
		if (!folio_try_get_rcu(folio))
			goto retry;

		if (unlikely(folio != xas_reload(&xas)))
			goto put_folio;

		if (!folio_batch_add(fbatch, folio))
			break;
		if (!folio_test_uptodate(folio))
			break;
		if (folio_test_readahead(folio))
			break;
		xas_advance(&xas, folio_next_index(folio) - 1);
		continue;
put_folio:
		folio_put(folio);
retry:
		xas_reset(&xas);
	}
	rcu_read_unlock();
}

static int filemap_read_folio(struct file *file, filler_t filler,
		struct folio *folio)
{
	bool workingset = folio_test_workingset(folio);
	unsigned long pflags;
	int error;

	/*
	 * A previous I/O error may have been due to temporary failures,
	 * eg. multipath errors.  PG_error will be set again if read_folio
	 * fails.
	 */
	folio_clear_error(folio);

	/* Start the actual read. The read will unlock the page. */
	if (unlikely(workingset))
		psi_memstall_enter(&pflags);
	error = filler(file, folio);
	if (unlikely(workingset))
		psi_memstall_leave(&pflags);
	if (error)
		return error;

	error = folio_wait_locked_killable(folio);
	if (error)
		return error;
	if (folio_test_uptodate(folio))
		return 0;
	if (file)
		shrink_readahead_size_eio(&file->f_ra);
	return -EIO;
}

static bool filemap_range_uptodate(struct address_space *mapping,
		loff_t pos, size_t count, struct folio *folio,
		bool need_uptodate)
{
	if (folio_test_uptodate(folio))
		return true;
	/* pipes can't handle partially uptodate pages */
	if (need_uptodate)
		return false;
	if (!mapping->a_ops->is_partially_uptodate)
		return false;
	if (mapping->host->i_blkbits >= folio_shift(folio))
		return false;

	if (folio_pos(folio) > pos) {
		count -= folio_pos(folio) - pos;
		pos = 0;
	} else {
		pos -= folio_pos(folio);
	}

	return mapping->a_ops->is_partially_uptodate(folio, pos, count);
}

static int filemap_update_page(struct kiocb *iocb,
		struct address_space *mapping, size_t count,
		struct folio *folio, bool need_uptodate)
{
	int error;

	if (iocb->ki_flags & IOCB_NOWAIT) {
		if (!filemap_invalidate_trylock_shared(mapping))
			return -EAGAIN;
	} else {
		filemap_invalidate_lock_shared(mapping);
	}

	if (!folio_trylock(folio)) {
		error = -EAGAIN;
		if (iocb->ki_flags & (IOCB_NOWAIT | IOCB_NOIO))
			goto unlock_mapping;
		if (!(iocb->ki_flags & IOCB_WAITQ)) {
			filemap_invalidate_unlock_shared(mapping);
			/*
			 * This is where we usually end up waiting for a
			 * previously submitted readahead to finish.
			 */
			folio_put_wait_locked(folio, TASK_KILLABLE);
			return AOP_TRUNCATED_PAGE;
		}
		error = __folio_lock_async(folio, iocb->ki_waitq);
		if (error)
			goto unlock_mapping;
	}

	error = AOP_TRUNCATED_PAGE;
	if (!folio->mapping)
		goto unlock;

	error = 0;
	if (filemap_range_uptodate(mapping, iocb->ki_pos, count, folio,
				   need_uptodate))
		goto unlock;

	error = -EAGAIN;
	if (iocb->ki_flags & (IOCB_NOIO | IOCB_NOWAIT | IOCB_WAITQ))
		goto unlock;

	error = filemap_read_folio(iocb->ki_filp, mapping->a_ops->read_folio,
			folio);
	goto unlock_mapping;
unlock:
	folio_unlock(folio);
unlock_mapping:
	filemap_invalidate_unlock_shared(mapping);
	if (error == AOP_TRUNCATED_PAGE)
		folio_put(folio);
	return error;
}

static int filemap_create_folio(struct file *file,
		struct address_space *mapping, pgoff_t index,
		struct folio_batch *fbatch)
{
	struct folio *folio;
	int error;

	folio = filemap_alloc_folio(mapping_gfp_mask(mapping), 0);
	if (!folio)
		return -ENOMEM;

	/*
	 * Protect against truncate / hole punch. Grabbing invalidate_lock
	 * here assures we cannot instantiate and bring uptodate new
	 * pagecache folios after evicting page cache during truncate
	 * and before actually freeing blocks.	Note that we could
	 * release invalidate_lock after inserting the folio into
	 * the page cache as the locked folio would then be enough to
	 * synchronize with hole punching. But there are code paths
	 * such as filemap_update_page() filling in partially uptodate
	 * pages or ->readahead() that need to hold invalidate_lock
	 * while mapping blocks for IO so let's hold the lock here as
	 * well to keep locking rules simple.
	 */
	filemap_invalidate_lock_shared(mapping);
	error = filemap_add_folio(mapping, folio, index,
			mapping_gfp_constraint(mapping, GFP_KERNEL));
	if (error == -EEXIST)
		error = AOP_TRUNCATED_PAGE;
	if (error)
		goto error;

	error = filemap_read_folio(file, mapping->a_ops->read_folio, folio);
	if (error)
		goto error;

	filemap_invalidate_unlock_shared(mapping);
	folio_batch_add(fbatch, folio);
	return 0;
error:
	filemap_invalidate_unlock_shared(mapping);
	folio_put(folio);
	return error;
}

static int filemap_readahead(struct kiocb *iocb, struct file *file,
		struct address_space *mapping, struct folio *folio,
		pgoff_t last_index)
{
	DEFINE_READAHEAD(ractl, file, &file->f_ra, mapping, folio->index);

	if (iocb->ki_flags & IOCB_NOIO)
		return -EAGAIN;
	page_cache_async_ra(&ractl, folio, last_index - folio->index);
	return 0;
}

static int filemap_get_pages(struct kiocb *iocb, size_t count,
		struct folio_batch *fbatch, bool need_uptodate)
{
	struct file *filp = iocb->ki_filp;
	struct address_space *mapping = filp->f_mapping;
	struct file_ra_state *ra = &filp->f_ra;
	pgoff_t index = iocb->ki_pos >> PAGE_SHIFT;
	pgoff_t last_index;
	struct folio *folio;
	int err = 0;

	/* "last_index" is the index of the page beyond the end of the read */
	last_index = DIV_ROUND_UP(iocb->ki_pos + count, PAGE_SIZE);
retry:
	if (fatal_signal_pending(current))
		return -EINTR;

	filemap_get_read_batch(mapping, index, last_index - 1, fbatch);
	if (!folio_batch_count(fbatch)) {
		if (iocb->ki_flags & IOCB_NOIO)
			return -EAGAIN;
		page_cache_sync_readahead(mapping, ra, filp, index,
				last_index - index);
		filemap_get_read_batch(mapping, index, last_index - 1, fbatch);
	}
	if (!folio_batch_count(fbatch)) {
		if (iocb->ki_flags & (IOCB_NOWAIT | IOCB_WAITQ))
			return -EAGAIN;
		err = filemap_create_folio(filp, mapping,
				iocb->ki_pos >> PAGE_SHIFT, fbatch);
		if (err == AOP_TRUNCATED_PAGE)
			goto retry;
		return err;
	}

	folio = fbatch->folios[folio_batch_count(fbatch) - 1];
	if (folio_test_readahead(folio)) {
		err = filemap_readahead(iocb, filp, mapping, folio, last_index);
		if (err)
			goto err;
	}
	if (!folio_test_uptodate(folio)) {
		if ((iocb->ki_flags & IOCB_WAITQ) &&
		    folio_batch_count(fbatch) > 1)
			iocb->ki_flags |= IOCB_NOWAIT;
		err = filemap_update_page(iocb, mapping, count, folio,
					  need_uptodate);
		if (err)
			goto err;
	}

	return 0;
err:
	if (err < 0)
		folio_put(folio);
	if (likely(--fbatch->nr))
		return 0;
	if (err == AOP_TRUNCATED_PAGE)
		goto retry;
	return err;
}

static inline bool pos_same_folio(loff_t pos1, loff_t pos2, struct folio *folio)
{
	unsigned int shift = folio_shift(folio);

	return (pos1 >> shift == pos2 >> shift);
}

/**
 * filemap_read - Read data from the page cache.
 * @iocb: The iocb to read.
 * @iter: Destination for the data.
 * @already_read: Number of bytes already read by the caller.
 *
 * Copies data from the page cache.  If the data is not currently present,
 * uses the readahead and read_folio address_space operations to fetch it.
 *
 * Return: Total number of bytes copied, including those already read by
 * the caller.  If an error happens before any bytes are copied, returns
 * a negative error number.
 */
ssize_t filemap_read(struct kiocb *iocb, struct iov_iter *iter,
		ssize_t already_read)
{
	struct file *filp = iocb->ki_filp;
	struct file_ra_state *ra = &filp->f_ra;
	struct address_space *mapping = filp->f_mapping;
	struct inode *inode = mapping->host;
	struct folio_batch fbatch;
	int i, error = 0;
	bool writably_mapped;
	loff_t isize, end_offset;
	loff_t last_pos = ra->prev_pos;

	if (unlikely(iocb->ki_pos >= inode->i_sb->s_maxbytes))
		return 0;
	if (unlikely(!iov_iter_count(iter)))
		return 0;

	iov_iter_truncate(iter, inode->i_sb->s_maxbytes);
	folio_batch_init(&fbatch);

	do {
		cond_resched();

		/*
		 * If we've already successfully copied some data, then we
		 * can no longer safely return -EIOCBQUEUED. Hence mark
		 * an async read NOWAIT at that point.
		 */
		if ((iocb->ki_flags & IOCB_WAITQ) && already_read)
			iocb->ki_flags |= IOCB_NOWAIT;

		if (unlikely(iocb->ki_pos >= i_size_read(inode)))
			break;

		error = filemap_get_pages(iocb, iter->count, &fbatch, false);
		if (error < 0)
			break;

		/*
		 * i_size must be checked after we know the pages are Uptodate.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		 *
		 * Checking i_size after the check allows us to calculate
		 * the correct value for "nr", which means the zero-filled
		 * part of the page is not copied back to userspace (unless
		 * another truncate extends the file - this is desired though).
		 */
<<<<<<< HEAD

		isize = i_size_read(inode);
		end_index = (isize - 1) >> PAGE_CACHE_SHIFT;
		if (unlikely(!isize || index > end_index)) {
			page_cache_release(page);
			goto out;
		}

		/* nr is the maximum number of bytes to copy from this page */
		nr = PAGE_CACHE_SIZE;
		if (index == end_index) {
			nr = ((isize - 1) & ~PAGE_CACHE_MASK) + 1;
			if (nr <= offset) {
				page_cache_release(page);
				goto out;
			}
		}
		nr = nr - offset;

		/* If users can be writing to this page using arbitrary
		 * virtual addresses, take care about potential aliasing
		 * before reading the page on the kernel side.
		 */
		if (mapping_writably_mapped(mapping))
			flush_dcache_page(page);

		/*
		 * When a sequential read accesses a page several times,
		 * only mark it as accessed the first time.
		 */
		if (prev_index != index || offset != prev_offset)
			mark_page_accessed(page);
		prev_index = index;

		/*
		 * Ok, we have the page, and it's up-to-date, so
		 * now we can copy it to user space...
		 *
		 * The actor routine returns how many bytes were actually used..
		 * NOTE! This may not be the same as how much of a user buffer
		 * we filled up (we may be padding etc), so we can only update
		 * "pos" here (the actor routine has to update the user buffer
		 * pointers and the remaining count).
		 */
		ret = actor(desc, page, offset, nr);
		offset += ret;
		index += offset >> PAGE_CACHE_SHIFT;
		offset &= ~PAGE_CACHE_MASK;
		prev_offset = offset;

		page_cache_release(page);
		if (ret == nr && desc->count)
			continue;
		goto out;

page_not_up_to_date:
		/* Get exclusive access to the page ... */
		error = lock_page_killable(page);
		if (unlikely(error))
			goto readpage_error;

page_not_up_to_date_locked:
		/* Did it get truncated before we got the lock? */
		if (!page->mapping) {
			unlock_page(page);
			page_cache_release(page);
			continue;
		}

		/* Did somebody else fill it already? */
		if (PageUptodate(page)) {
			unlock_page(page);
			goto page_ok;
		}

readpage:
		/*
		 * A previous I/O error may have been due to temporary
		 * failures, eg. multipath errors.
		 * PG_error will be set again if readpage fails.
		 */
		ClearPageError(page);
		/* Start the actual read. The read will unlock the page. */
		error = mapping->a_ops->readpage(filp, page);

		if (unlikely(error)) {
			if (error == AOP_TRUNCATED_PAGE) {
				page_cache_release(page);
				goto find_page;
			}
			goto readpage_error;
		}

		if (!PageUptodate(page)) {
			error = lock_page_killable(page);
			if (unlikely(error))
				goto readpage_error;
			if (!PageUptodate(page)) {
				if (page->mapping == NULL) {
					/*
					 * invalidate_mapping_pages got it
					 */
					unlock_page(page);
					page_cache_release(page);
					goto find_page;
				}
				unlock_page(page);
				shrink_readahead_size_eio(filp, ra);
				error = -EIO;
				goto readpage_error;
			}
			unlock_page(page);
		}

		goto page_ok;

readpage_error:
		/* UHHUH! A synchronous read error occurred. Report it */
		desc->error = error;
		page_cache_release(page);
		goto out;

no_cached_page:
		/*
		 * Ok, it wasn't cached, so we need to create a new
		 * page..
		 */
		page = page_cache_alloc_cold(mapping);
		if (!page) {
			desc->error = -ENOMEM;
			goto out;
		}
		error = add_to_page_cache_lru(page, mapping,
						index, GFP_KERNEL);
		if (error) {
			page_cache_release(page);
			if (error == -EEXIST)
				goto find_page;
			desc->error = error;
			goto out;
		}
		goto readpage;
	}

out:
	ra->prev_pos = prev_index;
	ra->prev_pos <<= PAGE_CACHE_SHIFT;
	ra->prev_pos |= prev_offset;

	*ppos = ((loff_t)index << PAGE_CACHE_SHIFT) + offset;
	file_accessed(filp);
}

int file_read_actor(read_descriptor_t *desc, struct page *page,
			unsigned long offset, unsigned long size)
{
	char *kaddr;
	unsigned long left, count = desc->count;

	if (size > count)
		size = count;

	/*
	 * Faults on the destination of a read are common, so do it before
	 * taking the kmap.
	 */
	if (!fault_in_pages_writeable(desc->arg.buf, size)) {
		kaddr = kmap_atomic(page);
		left = __copy_to_user_inatomic(desc->arg.buf,
						kaddr + offset, size);
		kunmap_atomic(kaddr);
		if (left == 0)
			goto success;
	}

	/* Do it the slow way */
	kaddr = kmap(page);
	left = __copy_to_user(desc->arg.buf, kaddr + offset, size);
	kunmap(page);

	if (left) {
		size -= left;
		desc->error = -EFAULT;
	}
success:
	desc->count = count - size;
	desc->written += size;
	desc->arg.buf += size;
	return size;
}

/*
 * Performs necessary checks before doing a write
 * @iov:	io vector request
 * @nr_segs:	number of segments in the iovec
 * @count:	number of bytes to write
 * @access_flags: type of access: %VERIFY_READ or %VERIFY_WRITE
 *
 * Adjust number of segments and amount of bytes to write (nr_segs should be
 * properly initialized first). Returns appropriate error code that caller
 * should return or zero in case that write should be allowed.
 */
int generic_segment_checks(const struct iovec *iov,
			unsigned long *nr_segs, size_t *count, int access_flags)
{
	unsigned long   seg;
	size_t cnt = 0;
	for (seg = 0; seg < *nr_segs; seg++) {
		const struct iovec *iv = &iov[seg];

		/*
		 * If any segment has a negative length, or the cumulative
		 * length ever wraps negative then return -EINVAL.
		 */
		cnt += iv->iov_len;
		if (unlikely((ssize_t)(cnt|iv->iov_len) < 0))
			return -EINVAL;
		if (access_ok(access_flags, iv->iov_base, iv->iov_len))
			continue;
		if (seg == 0)
			return -EFAULT;
		*nr_segs = seg;
		cnt -= iv->iov_len;	/* This segment is no good */
		break;
	}
	*count = cnt;
	return 0;
}
EXPORT_SYMBOL(generic_segment_checks);

/**
 * generic_file_aio_read - generic filesystem read routine
 * @iocb:	kernel I/O control block
 * @iov:	io vector request
 * @nr_segs:	number of segments in the iovec
 * @pos:	current file position
 *
 * This is the "read()" routine for all filesystems
 * that can use the page cache directly.
 */
ssize_t
generic_file_aio_read(struct kiocb *iocb, const struct iovec *iov,
		unsigned long nr_segs, loff_t pos)
{
	struct file *filp = iocb->ki_filp;
	ssize_t retval;
	unsigned long seg = 0;
	size_t count;
	loff_t *ppos = &iocb->ki_pos;

	count = 0;
	retval = generic_segment_checks(iov, &nr_segs, &count, VERIFY_WRITE);
	if (retval)
		return retval;

	/* coalesce the iovecs and go direct-to-BIO for O_DIRECT */
	if (filp->f_flags & O_DIRECT) {
		loff_t size;
		struct address_space *mapping;
		struct inode *inode;

		mapping = filp->f_mapping;
		inode = mapping->host;
		if (!count)
			goto out; /* skip atime */
		size = i_size_read(inode);
		if (pos < size) {
			retval = filemap_write_and_wait_range(mapping, pos,
					pos + iov_length(iov, nr_segs) - 1);
			if (!retval) {
				struct blk_plug plug;

				blk_start_plug(&plug);
				retval = mapping->a_ops->direct_IO(READ, iocb,
							iov, pos, nr_segs);
				blk_finish_plug(&plug);
			}
			if (retval > 0) {
				*ppos = pos + retval;
				count -= retval;
			}

			/*
			 * Btrfs can have a short DIO read if we encounter
			 * compressed extents, so if there was an error, or if
			 * we've already read everything we wanted to, or if
			 * there was a short read because we hit EOF, go ahead
			 * and return.  Otherwise fallthrough to buffered io for
			 * the rest of the read.
			 */
			if (retval < 0 || !count || *ppos >= size) {
				file_accessed(filp);
				goto out;
			}
		}
	}

	count = retval;
	for (seg = 0; seg < nr_segs; seg++) {
		read_descriptor_t desc;
		loff_t offset = 0;

		/*
		 * If we did a short DIO read we need to skip the section of the
		 * iov that we've already read data into.
		 */
		if (count) {
			if (count > iov[seg].iov_len) {
				count -= iov[seg].iov_len;
				continue;
			}
			offset = count;
			count = 0;
		}

		desc.written = 0;
		desc.arg.buf = iov[seg].iov_base + offset;
		desc.count = iov[seg].iov_len - offset;
		if (desc.count == 0)
			continue;
		desc.error = 0;
		do_generic_file_read(filp, ppos, &desc, file_read_actor);
		retval += desc.written;
		if (desc.error) {
			retval = retval ?: desc.error;
			break;
		}
		if (desc.count > 0)
			break;
	}
out:
	return retval;
}
EXPORT_SYMBOL(generic_file_aio_read);

static ssize_t
do_readahead(struct address_space *mapping, struct file *filp,
	     pgoff_t index, unsigned long nr)
{
	if (!mapping || !mapping->a_ops || !mapping->a_ops->readpage)
		return -EINVAL;

	force_page_cache_readahead(mapping, filp, index, nr);
	return 0;
}

SYSCALL_DEFINE(readahead)(int fd, loff_t offset, size_t count)
{
	ssize_t ret;
	struct file *file;

	ret = -EBADF;
	file = fget(fd);
	if (file) {
		if (file->f_mode & FMODE_READ) {
			struct address_space *mapping = file->f_mapping;
			pgoff_t start = offset >> PAGE_CACHE_SHIFT;
			pgoff_t end = (offset + count - 1) >> PAGE_CACHE_SHIFT;
			unsigned long len = end - start + 1;
			ret = do_readahead(mapping, file, start, len);
		}
		fput(file);
	}
	return ret;
}
#ifdef CONFIG_HAVE_SYSCALL_WRAPPERS
asmlinkage long SyS_readahead(long fd, loff_t offset, long count)
{
	return SYSC_readahead((int) fd, offset, (size_t) count);
}
SYSCALL_ALIAS(sys_readahead, SyS_readahead);
#endif

#ifdef CONFIG_MMU
/**
 * page_cache_read - adds requested page to the page cache if not already there
 * @file:	file to read
 * @offset:	page index
 *
 * This adds the requested page to the page cache if it isn't already there,
 * and schedules an I/O to read in its contents from disk.
 */
static int page_cache_read(struct file *file, pgoff_t offset)
{
	struct address_space *mapping = file->f_mapping;
	struct page *page; 
	int ret;

	do {
		page = page_cache_alloc_cold(mapping);
		if (!page)
			return -ENOMEM;

		ret = add_to_page_cache_lru(page, mapping, offset, GFP_KERNEL);
		if (ret == 0)
			ret = mapping->a_ops->readpage(file, page);
		else if (ret == -EEXIST)
			ret = 0; /* losing race to add is OK */

		page_cache_release(page);

	} while (ret == AOP_TRUNCATED_PAGE);
		
	return ret;
}

#define MMAP_LOTSAMISS  (100)

/*
 * Synchronous readahead happens when we don't even find
 * a page in the page cache at all.
 */
static void do_sync_mmap_readahead(struct vm_area_struct *vma,
				   struct file_ra_state *ra,
				   struct file *file,
				   pgoff_t offset)
{
	unsigned long ra_pages;
	struct address_space *mapping = file->f_mapping;

	/* If we don't want any read-ahead, don't bother */
	if (VM_RandomReadHint(vma))
		return;
	if (!ra->ra_pages)
		return;

	if (VM_SequentialReadHint(vma)) {
		page_cache_sync_readahead(mapping, ra, file, offset,
					  ra->ra_pages);
		return;
	}

	/* Avoid banging the cache line if not needed */
	if (ra->mmap_miss < MMAP_LOTSAMISS * 10)
		ra->mmap_miss++;
=======
		isize = i_size_read(inode);
		if (unlikely(iocb->ki_pos >= isize))
			goto put_folios;
		end_offset = min_t(loff_t, isize, iocb->ki_pos + iter->count);

		/*
		 * Once we start copying data, we don't want to be touching any
		 * cachelines that might be contended:
		 */
		writably_mapped = mapping_writably_mapped(mapping);

		/*
		 * When a read accesses the same folio several times, only
		 * mark it as accessed the first time.
		 */
		if (!pos_same_folio(iocb->ki_pos, last_pos - 1,
				    fbatch.folios[0]))
			folio_mark_accessed(fbatch.folios[0]);

		for (i = 0; i < folio_batch_count(&fbatch); i++) {
			struct folio *folio = fbatch.folios[i];
			size_t fsize = folio_size(folio);
			size_t offset = iocb->ki_pos & (fsize - 1);
			size_t bytes = min_t(loff_t, end_offset - iocb->ki_pos,
					     fsize - offset);
			size_t copied;

			if (end_offset < folio_pos(folio))
				break;
			if (i > 0)
				folio_mark_accessed(folio);
			/*
			 * If users can be writing to this folio using arbitrary
			 * virtual addresses, take care of potential aliasing
			 * before reading the folio on the kernel side.
			 */
			if (writably_mapped)
				flush_dcache_folio(folio);

			copied = copy_folio_to_iter(folio, offset, bytes, iter);

			already_read += copied;
			iocb->ki_pos += copied;
			last_pos = iocb->ki_pos;

			if (copied < bytes) {
				error = -EFAULT;
				break;
			}
		}
put_folios:
		for (i = 0; i < folio_batch_count(&fbatch); i++)
			folio_put(fbatch.folios[i]);
		folio_batch_init(&fbatch);
	} while (iov_iter_count(iter) && iocb->ki_pos < isize && !error);

	file_accessed(filp);
	ra->prev_pos = last_pos;
	return already_read ? already_read : error;
}
EXPORT_SYMBOL_GPL(filemap_read);

int kiocb_write_and_wait(struct kiocb *iocb, size_t count)
{
	struct address_space *mapping = iocb->ki_filp->f_mapping;
	loff_t pos = iocb->ki_pos;
	loff_t end = pos + count - 1;

	if (iocb->ki_flags & IOCB_NOWAIT) {
		if (filemap_range_needs_writeback(mapping, pos, end))
			return -EAGAIN;
		return 0;
	}

	return filemap_write_and_wait_range(mapping, pos, end);
}
EXPORT_SYMBOL_GPL(kiocb_write_and_wait);

int kiocb_invalidate_pages(struct kiocb *iocb, size_t count)
{
	struct address_space *mapping = iocb->ki_filp->f_mapping;
	loff_t pos = iocb->ki_pos;
	loff_t end = pos + count - 1;
	int ret;

	if (iocb->ki_flags & IOCB_NOWAIT) {
		/* we could block if there are any pages in the range */
		if (filemap_range_has_page(mapping, pos, end))
			return -EAGAIN;
	} else {
		ret = filemap_write_and_wait_range(mapping, pos, end);
		if (ret)
			return ret;
	}

	/*
	 * After a write we want buffered reads to be sure to go to disk to get
	 * the new data.  We invalidate clean cached page from the region we're
	 * about to write.  We do this *before* the write so that we can return
	 * without clobbering -EIOCBQUEUED from ->direct_IO().
	 */
	return invalidate_inode_pages2_range(mapping, pos >> PAGE_SHIFT,
					     end >> PAGE_SHIFT);
}
EXPORT_SYMBOL_GPL(kiocb_invalidate_pages);

/**
 * generic_file_read_iter - generic filesystem read routine
 * @iocb:	kernel I/O control block
 * @iter:	destination for the data read
 *
 * This is the "read_iter()" routine for all filesystems
 * that can use the page cache directly.
 *
 * The IOCB_NOWAIT flag in iocb->ki_flags indicates that -EAGAIN shall
 * be returned when no data can be read without waiting for I/O requests
 * to complete; it doesn't prevent readahead.
 *
 * The IOCB_NOIO flag in iocb->ki_flags indicates that no new I/O
 * requests shall be made for the read or for readahead.  When no data
 * can be read, -EAGAIN shall be returned.  When readahead would be
 * triggered, a partial, possibly empty read shall be returned.
 *
 * Return:
 * * number of bytes copied, even for partial reads
 * * negative error code (or 0 if IOCB_NOIO) if nothing was read
 */
ssize_t
generic_file_read_iter(struct kiocb *iocb, struct iov_iter *iter)
{
	size_t count = iov_iter_count(iter);
	ssize_t retval = 0;

	if (!count)
		return 0; /* skip atime */

	if (iocb->ki_flags & IOCB_DIRECT) {
		struct file *file = iocb->ki_filp;
		struct address_space *mapping = file->f_mapping;
		struct inode *inode = mapping->host;

		retval = kiocb_write_and_wait(iocb, count);
		if (retval < 0)
			return retval;
		file_accessed(file);

		retval = mapping->a_ops->direct_IO(iocb, iter);
		if (retval >= 0) {
			iocb->ki_pos += retval;
			count -= retval;
		}
		if (retval != -EIOCBQUEUED)
			iov_iter_revert(iter, count - iov_iter_count(iter));

		/*
		 * Btrfs can have a short DIO read if we encounter
		 * compressed extents, so if there was an error, or if
		 * we've already read everything we wanted to, or if
		 * there was a short read because we hit EOF, go ahead
		 * and return.  Otherwise fallthrough to buffered io for
		 * the rest of the read.  Buffered reads will not work for
		 * DAX files, so don't bother trying.
		 */
		if (retval < 0 || !count || IS_DAX(inode))
			return retval;
		if (iocb->ki_pos >= i_size_read(inode))
			return retval;
	}

	return filemap_read(iocb, iter, retval);
}
EXPORT_SYMBOL(generic_file_read_iter);

/*
 * Splice subpages from a folio into a pipe.
 */
size_t splice_folio_into_pipe(struct pipe_inode_info *pipe,
			      struct folio *folio, loff_t fpos, size_t size)
{
	struct page *page;
	size_t spliced = 0, offset = offset_in_folio(folio, fpos);

	page = folio_page(folio, offset / PAGE_SIZE);
	size = min(size, folio_size(folio) - offset);
	offset %= PAGE_SIZE;

	while (spliced < size &&
	       !pipe_full(pipe->head, pipe->tail, pipe->max_usage)) {
		struct pipe_buffer *buf = pipe_head_buf(pipe);
		size_t part = min_t(size_t, PAGE_SIZE - offset, size - spliced);

		*buf = (struct pipe_buffer) {
			.ops	= &page_cache_pipe_buf_ops,
			.page	= page,
			.offset	= offset,
			.len	= part,
		};
		folio_get(folio);
		pipe->head++;
		page++;
		spliced += part;
		offset = 0;
	}

	return spliced;
}

/**
 * filemap_splice_read -  Splice data from a file's pagecache into a pipe
 * @in: The file to read from
 * @ppos: Pointer to the file position to read from
 * @pipe: The pipe to splice into
 * @len: The amount to splice
 * @flags: The SPLICE_F_* flags
 *
 * This function gets folios from a file's pagecache and splices them into the
 * pipe.  Readahead will be called as necessary to fill more folios.  This may
 * be used for blockdevs also.
 *
 * Return: On success, the number of bytes read will be returned and *@ppos
 * will be updated if appropriate; 0 will be returned if there is no more data
 * to be read; -EAGAIN will be returned if the pipe had no space, and some
 * other negative error code will be returned on error.  A short read may occur
 * if the pipe has insufficient space, we reach the end of the data or we hit a
 * hole.
 */
ssize_t filemap_splice_read(struct file *in, loff_t *ppos,
			    struct pipe_inode_info *pipe,
			    size_t len, unsigned int flags)
{
	struct folio_batch fbatch;
	struct kiocb iocb;
	size_t total_spliced = 0, used, npages;
	loff_t isize, end_offset;
	bool writably_mapped;
	int i, error = 0;

	if (unlikely(*ppos >= in->f_mapping->host->i_sb->s_maxbytes))
		return 0;

	init_sync_kiocb(&iocb, in);
	iocb.ki_pos = *ppos;

	/* Work out how much data we can actually add into the pipe */
	used = pipe_occupancy(pipe->head, pipe->tail);
	npages = max_t(ssize_t, pipe->max_usage - used, 0);
	len = min_t(size_t, len, npages * PAGE_SIZE);

	folio_batch_init(&fbatch);

	do {
		cond_resched();

		if (*ppos >= i_size_read(in->f_mapping->host))
			break;

		iocb.ki_pos = *ppos;
		error = filemap_get_pages(&iocb, len, &fbatch, true);
		if (error < 0)
			break;

		/*
		 * i_size must be checked after we know the pages are Uptodate.
		 *
		 * Checking i_size after the check allows us to calculate
		 * the correct value for "nr", which means the zero-filled
		 * part of the page is not copied back to userspace (unless
		 * another truncate extends the file - this is desired though).
		 */
		isize = i_size_read(in->f_mapping->host);
		if (unlikely(*ppos >= isize))
			break;
		end_offset = min_t(loff_t, isize, *ppos + len);

		/*
		 * Once we start copying data, we don't want to be touching any
		 * cachelines that might be contended:
		 */
		writably_mapped = mapping_writably_mapped(in->f_mapping);

		for (i = 0; i < folio_batch_count(&fbatch); i++) {
			struct folio *folio = fbatch.folios[i];
			size_t n;

			if (folio_pos(folio) >= end_offset)
				goto out;
			folio_mark_accessed(folio);

			/*
			 * If users can be writing to this folio using arbitrary
			 * virtual addresses, take care of potential aliasing
			 * before reading the folio on the kernel side.
			 */
			if (writably_mapped)
				flush_dcache_folio(folio);

			n = min_t(loff_t, len, isize - *ppos);
			n = splice_folio_into_pipe(pipe, folio, *ppos, n);
			if (!n)
				goto out;
			len -= n;
			total_spliced += n;
			*ppos += n;
			in->f_ra.prev_pos = *ppos;
			if (pipe_full(pipe->head, pipe->tail, pipe->max_usage))
				goto out;
		}

		folio_batch_release(&fbatch);
	} while (len);

out:
	folio_batch_release(&fbatch);
	file_accessed(in);

	return total_spliced ? total_spliced : error;
}
EXPORT_SYMBOL(filemap_splice_read);

static inline loff_t folio_seek_hole_data(struct xa_state *xas,
		struct address_space *mapping, struct folio *folio,
		loff_t start, loff_t end, bool seek_data)
{
	const struct address_space_operations *ops = mapping->a_ops;
	size_t offset, bsz = i_blocksize(mapping->host);

	if (xa_is_value(folio) || folio_test_uptodate(folio))
		return seek_data ? start : end;
	if (!ops->is_partially_uptodate)
		return seek_data ? end : start;

	xas_pause(xas);
	rcu_read_unlock();
	folio_lock(folio);
	if (unlikely(folio->mapping != mapping))
		goto unlock;

	offset = offset_in_folio(folio, start) & ~(bsz - 1);

	do {
		if (ops->is_partially_uptodate(folio, offset, bsz) ==
							seek_data)
			break;
		start = (start + bsz) & ~(bsz - 1);
		offset += bsz;
	} while (offset < folio_size(folio));
unlock:
	folio_unlock(folio);
	rcu_read_lock();
	return start;
}

static inline size_t seek_folio_size(struct xa_state *xas, struct folio *folio)
{
	if (xa_is_value(folio))
		return PAGE_SIZE << xa_get_order(xas->xa, xas->xa_index);
	return folio_size(folio);
}

/**
 * mapping_seek_hole_data - Seek for SEEK_DATA / SEEK_HOLE in the page cache.
 * @mapping: Address space to search.
 * @start: First byte to consider.
 * @end: Limit of search (exclusive).
 * @whence: Either SEEK_HOLE or SEEK_DATA.
 *
 * If the page cache knows which blocks contain holes and which blocks
 * contain data, your filesystem can use this function to implement
 * SEEK_HOLE and SEEK_DATA.  This is useful for filesystems which are
 * entirely memory-based such as tmpfs, and filesystems which support
 * unwritten extents.
 *
 * Return: The requested offset on success, or -ENXIO if @whence specifies
 * SEEK_DATA and there is no data after @start.  There is an implicit hole
 * after @end - 1, so SEEK_HOLE returns @end if all the bytes between @start
 * and @end contain data.
 */
loff_t mapping_seek_hole_data(struct address_space *mapping, loff_t start,
		loff_t end, int whence)
{
	XA_STATE(xas, &mapping->i_pages, start >> PAGE_SHIFT);
	pgoff_t max = (end - 1) >> PAGE_SHIFT;
	bool seek_data = (whence == SEEK_DATA);
	struct folio *folio;

	if (end <= start)
		return -ENXIO;

	rcu_read_lock();
	while ((folio = find_get_entry(&xas, max, XA_PRESENT))) {
		loff_t pos = (u64)xas.xa_index << PAGE_SHIFT;
		size_t seek_size;

		if (start < pos) {
			if (!seek_data)
				goto unlock;
			start = pos;
		}

		seek_size = seek_folio_size(&xas, folio);
		pos = round_up((u64)pos + 1, seek_size);
		start = folio_seek_hole_data(&xas, mapping, folio, start, pos,
				seek_data);
		if (start < pos)
			goto unlock;
		if (start >= end)
			break;
		if (seek_size > PAGE_SIZE)
			xas_set(&xas, pos >> PAGE_SHIFT);
		if (!xa_is_value(folio))
			folio_put(folio);
	}
	if (seek_data)
		start = -ENXIO;
unlock:
	rcu_read_unlock();
	if (folio && !xa_is_value(folio))
		folio_put(folio);
	if (start > end)
		return end;
	return start;
}

#ifdef CONFIG_MMU
#define MMAP_LOTSAMISS  (100)
/*
 * lock_folio_maybe_drop_mmap - lock the page, possibly dropping the mmap_lock
 * @vmf - the vm_fault for this fault.
 * @folio - the folio to lock.
 * @fpin - the pointer to the file we may pin (or is already pinned).
 *
 * This works similar to lock_folio_or_retry in that it can drop the
 * mmap_lock.  It differs in that it actually returns the folio locked
 * if it returns 1 and 0 if it couldn't lock the folio.  If we did have
 * to drop the mmap_lock then fpin will point to the pinned file and
 * needs to be fput()'ed at a later point.
 */
static int lock_folio_maybe_drop_mmap(struct vm_fault *vmf, struct folio *folio,
				     struct file **fpin)
{
	if (folio_trylock(folio))
		return 1;

	/*
	 * NOTE! This will make us return with VM_FAULT_RETRY, but with
	 * the fault lock still held. That's how FAULT_FLAG_RETRY_NOWAIT
	 * is supposed to work. We have way too many special cases..
	 */
	if (vmf->flags & FAULT_FLAG_RETRY_NOWAIT)
		return 0;

	*fpin = maybe_unlock_mmap_for_io(vmf, *fpin);
	if (vmf->flags & FAULT_FLAG_KILLABLE) {
		if (__folio_lock_killable(folio)) {
			/*
			 * We didn't have the right flags to drop the
			 * fault lock, but all fault_handlers only check
			 * for fatal signals if we return VM_FAULT_RETRY,
			 * so we need to drop the fault lock here and
			 * return 0 if we don't have a fpin.
			 */
			if (*fpin == NULL)
				release_fault_lock(vmf);
			return 0;
		}
	} else
		__folio_lock(folio);

	return 1;
}

/*
 * Synchronous readahead happens when we don't even find a page in the page
 * cache at all.  We don't want to perform IO under the mmap sem, so if we have
 * to drop the mmap sem we return the file that was pinned in order for us to do
 * that.  If we didn't pin a file then we return NULL.  The file that is
 * returned needs to be fput()'ed when we're done with it.
 */
static struct file *do_sync_mmap_readahead(struct vm_fault *vmf)
{
	struct file *file = vmf->vma->vm_file;
	struct file_ra_state *ra = &file->f_ra;
	struct address_space *mapping = file->f_mapping;
	DEFINE_READAHEAD(ractl, file, ra, mapping, vmf->pgoff);
	struct file *fpin = NULL;
	unsigned long vm_flags = vmf->vma->vm_flags;
	unsigned int mmap_miss;

#ifdef CONFIG_TRANSPARENT_HUGEPAGE
	/* Use the readahead code, even if readahead is disabled */
	if (vm_flags & VM_HUGEPAGE) {
		fpin = maybe_unlock_mmap_for_io(vmf, fpin);
		ractl._index &= ~((unsigned long)HPAGE_PMD_NR - 1);
		ra->size = HPAGE_PMD_NR;
		/*
		 * Fetch two PMD folios, so we get the chance to actually
		 * readahead, unless we've been told not to.
		 */
		if (!(vm_flags & VM_RAND_READ))
			ra->size *= 2;
		ra->async_size = HPAGE_PMD_NR;
		page_cache_ra_order(&ractl, ra, HPAGE_PMD_ORDER);
		return fpin;
	}
#endif

	/* If we don't want any read-ahead, don't bother */
	if (vm_flags & VM_RAND_READ)
		return fpin;
	if (!ra->ra_pages)
		return fpin;

	if (vm_flags & VM_SEQ_READ) {
		fpin = maybe_unlock_mmap_for_io(vmf, fpin);
		page_cache_sync_ra(&ractl, ra->ra_pages);
		return fpin;
	}

	/* Avoid banging the cache line if not needed */
	mmap_miss = READ_ONCE(ra->mmap_miss);
	if (mmap_miss < MMAP_LOTSAMISS * 10)
		WRITE_ONCE(ra->mmap_miss, ++mmap_miss);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Do we miss much more than hit in this file? If so,
	 * stop bothering with read-ahead. It will only hurt.
	 */
<<<<<<< HEAD
	if (ra->mmap_miss > MMAP_LOTSAMISS)
		return;
=======
	if (mmap_miss > MMAP_LOTSAMISS)
		return fpin;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * mmap read-around
	 */
<<<<<<< HEAD
	ra_pages = max_sane_readahead(ra->ra_pages);
	ra->start = max_t(long, 0, offset - ra_pages / 2);
	ra->size = ra_pages;
	ra->async_size = ra_pages / 4;
	ra_submit(ra, mapping, file);
=======
	fpin = maybe_unlock_mmap_for_io(vmf, fpin);
	ra->start = max_t(long, 0, vmf->pgoff - ra->ra_pages / 2);
	ra->size = ra->ra_pages;
	ra->async_size = ra->ra_pages / 4;
	ractl._index = ra->start;
	page_cache_ra_order(&ractl, ra, 0);
	return fpin;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Asynchronous readahead happens when we find the page and PG_readahead,
<<<<<<< HEAD
 * so we want to possibly extend the readahead further..
 */
static void do_async_mmap_readahead(struct vm_area_struct *vma,
				    struct file_ra_state *ra,
				    struct file *file,
				    struct page *page,
				    pgoff_t offset)
{
	struct address_space *mapping = file->f_mapping;

	/* If we don't want any read-ahead, don't bother */
	if (VM_RandomReadHint(vma))
		return;
	if (ra->mmap_miss > 0)
		ra->mmap_miss--;
	if (PageReadahead(page))
		page_cache_async_readahead(mapping, ra, file,
					   page, offset, ra->ra_pages);
=======
 * so we want to possibly extend the readahead further.  We return the file that
 * was pinned if we have to drop the mmap_lock in order to do IO.
 */
static struct file *do_async_mmap_readahead(struct vm_fault *vmf,
					    struct folio *folio)
{
	struct file *file = vmf->vma->vm_file;
	struct file_ra_state *ra = &file->f_ra;
	DEFINE_READAHEAD(ractl, file, ra, file->f_mapping, vmf->pgoff);
	struct file *fpin = NULL;
	unsigned int mmap_miss;

	/* If we don't want any read-ahead, don't bother */
	if (vmf->vma->vm_flags & VM_RAND_READ || !ra->ra_pages)
		return fpin;

	mmap_miss = READ_ONCE(ra->mmap_miss);
	if (mmap_miss)
		WRITE_ONCE(ra->mmap_miss, --mmap_miss);

	if (folio_test_readahead(folio)) {
		fpin = maybe_unlock_mmap_for_io(vmf, fpin);
		page_cache_async_ra(&ractl, folio, ra->ra_pages);
	}
	return fpin;
}

static vm_fault_t filemap_fault_recheck_pte_none(struct vm_fault *vmf)
{
	struct vm_area_struct *vma = vmf->vma;
	vm_fault_t ret = 0;
	pte_t *ptep;

	/*
	 * We might have COW'ed a pagecache folio and might now have an mlocked
	 * anon folio mapped. The original pagecache folio is not mlocked and
	 * might have been evicted. During a read+clear/modify/write update of
	 * the PTE, such as done in do_numa_page()/change_pte_range(), we
	 * temporarily clear the PTE under PT lock and might detect it here as
	 * "none" when not holding the PT lock.
	 *
	 * Not rechecking the PTE under PT lock could result in an unexpected
	 * major fault in an mlock'ed region. Recheck only for this special
	 * scenario while holding the PT lock, to not degrade non-mlocked
	 * scenarios. Recheck the PTE without PT lock firstly, thereby reducing
	 * the number of times we hold PT lock.
	 */
	if (!(vma->vm_flags & VM_LOCKED))
		return 0;

	if (!(vmf->flags & FAULT_FLAG_ORIG_PTE_VALID))
		return 0;

	ptep = pte_offset_map(vmf->pmd, vmf->address);
	if (unlikely(!ptep))
		return VM_FAULT_NOPAGE;

	if (unlikely(!pte_none(ptep_get_lockless(ptep)))) {
		ret = VM_FAULT_NOPAGE;
	} else {
		spin_lock(vmf->ptl);
		if (unlikely(!pte_none(ptep_get(ptep))))
			ret = VM_FAULT_NOPAGE;
		spin_unlock(vmf->ptl);
	}
	pte_unmap(ptep);
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * filemap_fault - read in file data for page fault handling
<<<<<<< HEAD
 * @vma:	vma in which the fault was taken
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @vmf:	struct vm_fault containing details of the fault
 *
 * filemap_fault() is invoked via the vma operations vector for a
 * mapped memory region to read in file data during a page fault.
 *
 * The goto's are kind of ugly, but this streamlines the normal case of having
 * it in the page cache, and handles the special cases reasonably without
 * having a lot of duplicated code.
<<<<<<< HEAD
 */
int filemap_fault(struct vm_area_struct *vma, struct vm_fault *vmf)
{
	int error;
	struct file *file = vma->vm_file;
	struct address_space *mapping = file->f_mapping;
	struct file_ra_state *ra = &file->f_ra;
	struct inode *inode = mapping->host;
	pgoff_t offset = vmf->pgoff;
	struct page *page;
	pgoff_t size;
	int ret = 0;

	size = (i_size_read(inode) + PAGE_CACHE_SIZE - 1) >> PAGE_CACHE_SHIFT;
	if (offset >= size)
=======
 *
 * vma->vm_mm->mmap_lock must be held on entry.
 *
 * If our return value has VM_FAULT_RETRY set, it's because the mmap_lock
 * may be dropped before doing I/O or by lock_folio_maybe_drop_mmap().
 *
 * If our return value does not have VM_FAULT_RETRY set, the mmap_lock
 * has not been released.
 *
 * We never return with VM_FAULT_RETRY and a bit from VM_FAULT_ERROR set.
 *
 * Return: bitwise-OR of %VM_FAULT_ codes.
 */
vm_fault_t filemap_fault(struct vm_fault *vmf)
{
	int error;
	struct file *file = vmf->vma->vm_file;
	struct file *fpin = NULL;
	struct address_space *mapping = file->f_mapping;
	struct inode *inode = mapping->host;
	pgoff_t max_idx, index = vmf->pgoff;
	struct folio *folio;
	vm_fault_t ret = 0;
	bool mapping_locked = false;

	max_idx = DIV_ROUND_UP(i_size_read(inode), PAGE_SIZE);
	if (unlikely(index >= max_idx))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return VM_FAULT_SIGBUS;

	/*
	 * Do we have something in the page cache already?
	 */
<<<<<<< HEAD
	page = find_get_page(mapping, offset);
	if (likely(page)) {
		/*
		 * We found the page, so try async readahead before
		 * waiting for the lock.
		 */
		do_async_mmap_readahead(vma, ra, file, page, offset);
	} else {
		/* No page in the page cache at all */
		do_sync_mmap_readahead(vma, ra, file, offset);
		count_vm_event(PGMAJFAULT);
		/* LGE_CHANGE_S
		*
		* Profile files related to pgmajfault during 1st booting
		* in order to use the data as readahead args
		*
		* matia.kim@lge.com 20130612
		*/
		sreadahead_prof(file, 0, 0);
		/* LGE_CHANGE_E */
		mem_cgroup_count_vm_event(vma->vm_mm, PGMAJFAULT);
		ret = VM_FAULT_MAJOR;
retry_find:
		page = find_get_page(mapping, offset);
		if (!page)
			goto no_cached_page;
	}

	if (!lock_page_or_retry(page, vma->vm_mm, vmf->flags)) {
		page_cache_release(page);
		return ret | VM_FAULT_RETRY;
	}

	/* Did it get truncated? */
	if (unlikely(page->mapping != mapping)) {
		unlock_page(page);
		put_page(page);
		goto retry_find;
	}
	VM_BUG_ON(page->index != offset);

	/*
	 * We have a locked page in the page cache, now we need to check
	 * that it's up-to-date. If not, it is going to be due to an error.
	 */
	if (unlikely(!PageUptodate(page)))
		goto page_not_uptodate;
=======
	folio = filemap_get_folio(mapping, index);
	if (likely(!IS_ERR(folio))) {
		/*
		 * We found the page, so try async readahead before waiting for
		 * the lock.
		 */
		if (!(vmf->flags & FAULT_FLAG_TRIED))
			fpin = do_async_mmap_readahead(vmf, folio);
		if (unlikely(!folio_test_uptodate(folio))) {
			filemap_invalidate_lock_shared(mapping);
			mapping_locked = true;
		}
	} else {
		ret = filemap_fault_recheck_pte_none(vmf);
		if (unlikely(ret))
			return ret;

		/* No page in the page cache at all */
		count_vm_event(PGMAJFAULT);
		count_memcg_event_mm(vmf->vma->vm_mm, PGMAJFAULT);
		ret = VM_FAULT_MAJOR;
		fpin = do_sync_mmap_readahead(vmf);
retry_find:
		/*
		 * See comment in filemap_create_folio() why we need
		 * invalidate_lock
		 */
		if (!mapping_locked) {
			filemap_invalidate_lock_shared(mapping);
			mapping_locked = true;
		}
		folio = __filemap_get_folio(mapping, index,
					  FGP_CREAT|FGP_FOR_MMAP,
					  vmf->gfp_mask);
		if (IS_ERR(folio)) {
			if (fpin)
				goto out_retry;
			filemap_invalidate_unlock_shared(mapping);
			return VM_FAULT_OOM;
		}
	}

	if (!lock_folio_maybe_drop_mmap(vmf, folio, &fpin))
		goto out_retry;

	/* Did it get truncated? */
	if (unlikely(folio->mapping != mapping)) {
		folio_unlock(folio);
		folio_put(folio);
		goto retry_find;
	}
	VM_BUG_ON_FOLIO(!folio_contains(folio, index), folio);

	/*
	 * We have a locked folio in the page cache, now we need to check
	 * that it's up-to-date. If not, it is going to be due to an error,
	 * or because readahead was otherwise unable to retrieve it.
	 */
	if (unlikely(!folio_test_uptodate(folio))) {
		/*
		 * If the invalidate lock is not held, the folio was in cache
		 * and uptodate and now it is not. Strange but possible since we
		 * didn't hold the page lock all the time. Let's drop
		 * everything, get the invalidate lock and try again.
		 */
		if (!mapping_locked) {
			folio_unlock(folio);
			folio_put(folio);
			goto retry_find;
		}

		/*
		 * OK, the folio is really not uptodate. This can be because the
		 * VMA has the VM_RAND_READ flag set, or because an error
		 * arose. Let's read it in directly.
		 */
		goto page_not_uptodate;
	}

	/*
	 * We've made it this far and we had to drop our mmap_lock, now is the
	 * time to return to the upper layer and have it re-find the vma and
	 * redo the fault.
	 */
	if (fpin) {
		folio_unlock(folio);
		goto out_retry;
	}
	if (mapping_locked)
		filemap_invalidate_unlock_shared(mapping);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Found the page and have a reference on it.
	 * We must recheck i_size under page lock.
	 */
<<<<<<< HEAD
	size = (i_size_read(inode) + PAGE_CACHE_SIZE - 1) >> PAGE_CACHE_SHIFT;
	if (unlikely(offset >= size)) {
		unlock_page(page);
		page_cache_release(page);
		return VM_FAULT_SIGBUS;
	}

	vmf->page = page;
	return ret | VM_FAULT_LOCKED;

no_cached_page:
	/*
	 * We're only likely to ever get here if MADV_RANDOM is in
	 * effect.
	 */
	error = page_cache_read(file, offset);

	/*
	 * The page we want has now been added to the page cache.
	 * In the unlikely event that someone removed it in the
	 * meantime, we'll just come back here and read it again.
	 */
	if (error >= 0)
		goto retry_find;

	/*
	 * An error return from page_cache_read can result if the
	 * system is low on memory, or a problem occurs while trying
	 * to schedule I/O.
	 */
	if (error == -ENOMEM)
		return VM_FAULT_OOM;
	return VM_FAULT_SIGBUS;

=======
	max_idx = DIV_ROUND_UP(i_size_read(inode), PAGE_SIZE);
	if (unlikely(index >= max_idx)) {
		folio_unlock(folio);
		folio_put(folio);
		return VM_FAULT_SIGBUS;
	}

	vmf->page = folio_file_page(folio, index);
	return ret | VM_FAULT_LOCKED;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
page_not_uptodate:
	/*
	 * Umm, take care of errors if the page isn't up-to-date.
	 * Try to re-read it _once_. We do this synchronously,
	 * because there really aren't any performance issues here
	 * and we need to check for errors.
	 */
<<<<<<< HEAD
	ClearPageError(page);
	error = mapping->a_ops->readpage(file, page);
	if (!error) {
		wait_on_page_locked(page);
		if (!PageUptodate(page))
			error = -EIO;
	}
	page_cache_release(page);

	if (!error || error == AOP_TRUNCATED_PAGE)
		goto retry_find;

	/* Things didn't work out. Return zero to tell the mm layer so. */
	shrink_readahead_size_eio(file, ra);
	return VM_FAULT_SIGBUS;
}
EXPORT_SYMBOL(filemap_fault);

const struct vm_operations_struct generic_file_vm_ops = {
	.fault		= filemap_fault,
=======
	fpin = maybe_unlock_mmap_for_io(vmf, fpin);
	error = filemap_read_folio(file, mapping->a_ops->read_folio, folio);
	if (fpin)
		goto out_retry;
	folio_put(folio);

	if (!error || error == AOP_TRUNCATED_PAGE)
		goto retry_find;
	filemap_invalidate_unlock_shared(mapping);

	return VM_FAULT_SIGBUS;

out_retry:
	/*
	 * We dropped the mmap_lock, we need to return to the fault handler to
	 * re-find the vma and come back and find our hopefully still populated
	 * page.
	 */
	if (!IS_ERR(folio))
		folio_put(folio);
	if (mapping_locked)
		filemap_invalidate_unlock_shared(mapping);
	if (fpin)
		fput(fpin);
	return ret | VM_FAULT_RETRY;
}
EXPORT_SYMBOL(filemap_fault);

static bool filemap_map_pmd(struct vm_fault *vmf, struct folio *folio,
		pgoff_t start)
{
	struct mm_struct *mm = vmf->vma->vm_mm;

	/* Huge page is mapped? No need to proceed. */
	if (pmd_trans_huge(*vmf->pmd)) {
		folio_unlock(folio);
		folio_put(folio);
		return true;
	}

	if (pmd_none(*vmf->pmd) && folio_test_pmd_mappable(folio)) {
		struct page *page = folio_file_page(folio, start);
		vm_fault_t ret = do_set_pmd(vmf, page);
		if (!ret) {
			/* The page is mapped successfully, reference consumed. */
			folio_unlock(folio);
			return true;
		}
	}

	if (pmd_none(*vmf->pmd) && vmf->prealloc_pte)
		pmd_install(mm, vmf->pmd, &vmf->prealloc_pte);

	return false;
}

static struct folio *next_uptodate_folio(struct xa_state *xas,
		struct address_space *mapping, pgoff_t end_pgoff)
{
	struct folio *folio = xas_next_entry(xas, end_pgoff);
	unsigned long max_idx;

	do {
		if (!folio)
			return NULL;
		if (xas_retry(xas, folio))
			continue;
		if (xa_is_value(folio))
			continue;
		if (folio_test_locked(folio))
			continue;
		if (!folio_try_get_rcu(folio))
			continue;
		/* Has the page moved or been split? */
		if (unlikely(folio != xas_reload(xas)))
			goto skip;
		if (!folio_test_uptodate(folio) || folio_test_readahead(folio))
			goto skip;
		if (!folio_trylock(folio))
			goto skip;
		if (folio->mapping != mapping)
			goto unlock;
		if (!folio_test_uptodate(folio))
			goto unlock;
		max_idx = DIV_ROUND_UP(i_size_read(mapping->host), PAGE_SIZE);
		if (xas->xa_index >= max_idx)
			goto unlock;
		return folio;
unlock:
		folio_unlock(folio);
skip:
		folio_put(folio);
	} while ((folio = xas_next_entry(xas, end_pgoff)) != NULL);

	return NULL;
}

/*
 * Map page range [start_page, start_page + nr_pages) of folio.
 * start_page is gotten from start by folio_page(folio, start)
 */
static vm_fault_t filemap_map_folio_range(struct vm_fault *vmf,
			struct folio *folio, unsigned long start,
			unsigned long addr, unsigned int nr_pages,
			unsigned int *mmap_miss)
{
	vm_fault_t ret = 0;
	struct page *page = folio_page(folio, start);
	unsigned int count = 0;
	pte_t *old_ptep = vmf->pte;

	do {
		if (PageHWPoison(page + count))
			goto skip;

		(*mmap_miss)++;

		/*
		 * NOTE: If there're PTE markers, we'll leave them to be
		 * handled in the specific fault path, and it'll prohibit the
		 * fault-around logic.
		 */
		if (!pte_none(ptep_get(&vmf->pte[count])))
			goto skip;

		count++;
		continue;
skip:
		if (count) {
			set_pte_range(vmf, folio, page, count, addr);
			folio_ref_add(folio, count);
			if (in_range(vmf->address, addr, count * PAGE_SIZE))
				ret = VM_FAULT_NOPAGE;
		}

		count++;
		page += count;
		vmf->pte += count;
		addr += count * PAGE_SIZE;
		count = 0;
	} while (--nr_pages > 0);

	if (count) {
		set_pte_range(vmf, folio, page, count, addr);
		folio_ref_add(folio, count);
		if (in_range(vmf->address, addr, count * PAGE_SIZE))
			ret = VM_FAULT_NOPAGE;
	}

	vmf->pte = old_ptep;

	return ret;
}

static vm_fault_t filemap_map_order0_folio(struct vm_fault *vmf,
		struct folio *folio, unsigned long addr,
		unsigned int *mmap_miss)
{
	vm_fault_t ret = 0;
	struct page *page = &folio->page;

	if (PageHWPoison(page))
		return ret;

	(*mmap_miss)++;

	/*
	 * NOTE: If there're PTE markers, we'll leave them to be
	 * handled in the specific fault path, and it'll prohibit
	 * the fault-around logic.
	 */
	if (!pte_none(ptep_get(vmf->pte)))
		return ret;

	if (vmf->address == addr)
		ret = VM_FAULT_NOPAGE;

	set_pte_range(vmf, folio, page, 1, addr);
	folio_ref_inc(folio);

	return ret;
}

vm_fault_t filemap_map_pages(struct vm_fault *vmf,
			     pgoff_t start_pgoff, pgoff_t end_pgoff)
{
	struct vm_area_struct *vma = vmf->vma;
	struct file *file = vma->vm_file;
	struct address_space *mapping = file->f_mapping;
	pgoff_t last_pgoff = start_pgoff;
	unsigned long addr;
	XA_STATE(xas, &mapping->i_pages, start_pgoff);
	struct folio *folio;
	vm_fault_t ret = 0;
	unsigned int nr_pages = 0, mmap_miss = 0, mmap_miss_saved;

	rcu_read_lock();
	folio = next_uptodate_folio(&xas, mapping, end_pgoff);
	if (!folio)
		goto out;

	if (filemap_map_pmd(vmf, folio, start_pgoff)) {
		ret = VM_FAULT_NOPAGE;
		goto out;
	}

	addr = vma->vm_start + ((start_pgoff - vma->vm_pgoff) << PAGE_SHIFT);
	vmf->pte = pte_offset_map_lock(vma->vm_mm, vmf->pmd, addr, &vmf->ptl);
	if (!vmf->pte) {
		folio_unlock(folio);
		folio_put(folio);
		goto out;
	}
	do {
		unsigned long end;

		addr += (xas.xa_index - last_pgoff) << PAGE_SHIFT;
		vmf->pte += xas.xa_index - last_pgoff;
		last_pgoff = xas.xa_index;
		end = folio_next_index(folio) - 1;
		nr_pages = min(end, end_pgoff) - xas.xa_index + 1;

		if (!folio_test_large(folio))
			ret |= filemap_map_order0_folio(vmf,
					folio, addr, &mmap_miss);
		else
			ret |= filemap_map_folio_range(vmf, folio,
					xas.xa_index - folio->index, addr,
					nr_pages, &mmap_miss);

		folio_unlock(folio);
		folio_put(folio);
	} while ((folio = next_uptodate_folio(&xas, mapping, end_pgoff)) != NULL);
	pte_unmap_unlock(vmf->pte, vmf->ptl);
out:
	rcu_read_unlock();

	mmap_miss_saved = READ_ONCE(file->f_ra.mmap_miss);
	if (mmap_miss >= mmap_miss_saved)
		WRITE_ONCE(file->f_ra.mmap_miss, 0);
	else
		WRITE_ONCE(file->f_ra.mmap_miss, mmap_miss_saved - mmap_miss);

	return ret;
}
EXPORT_SYMBOL(filemap_map_pages);

vm_fault_t filemap_page_mkwrite(struct vm_fault *vmf)
{
	struct address_space *mapping = vmf->vma->vm_file->f_mapping;
	struct folio *folio = page_folio(vmf->page);
	vm_fault_t ret = VM_FAULT_LOCKED;

	sb_start_pagefault(mapping->host->i_sb);
	file_update_time(vmf->vma->vm_file);
	folio_lock(folio);
	if (folio->mapping != mapping) {
		folio_unlock(folio);
		ret = VM_FAULT_NOPAGE;
		goto out;
	}
	/*
	 * We mark the folio dirty already here so that when freeze is in
	 * progress, we are guaranteed that writeback during freezing will
	 * see the dirty folio and writeprotect it again.
	 */
	folio_mark_dirty(folio);
	folio_wait_stable(folio);
out:
	sb_end_pagefault(mapping->host->i_sb);
	return ret;
}

const struct vm_operations_struct generic_file_vm_ops = {
	.fault		= filemap_fault,
	.map_pages	= filemap_map_pages,
	.page_mkwrite	= filemap_page_mkwrite,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/* This is used for a general mmap of a disk file */

<<<<<<< HEAD
int generic_file_mmap(struct file * file, struct vm_area_struct * vma)
{
	struct address_space *mapping = file->f_mapping;

	if (!mapping->a_ops->readpage)
		return -ENOEXEC;
	file_accessed(file);
	vma->vm_ops = &generic_file_vm_ops;
	vma->vm_flags |= VM_CAN_NONLINEAR;
=======
int generic_file_mmap(struct file *file, struct vm_area_struct *vma)
{
	struct address_space *mapping = file->f_mapping;

	if (!mapping->a_ops->read_folio)
		return -ENOEXEC;
	file_accessed(file);
	vma->vm_ops = &generic_file_vm_ops;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

/*
 * This is for filesystems which do not implement ->writepage.
 */
int generic_file_readonly_mmap(struct file *file, struct vm_area_struct *vma)
{
<<<<<<< HEAD
	if ((vma->vm_flags & VM_SHARED) && (vma->vm_flags & VM_MAYWRITE))
=======
	if (vma_is_shared_maywrite(vma))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;
	return generic_file_mmap(file, vma);
}
#else
<<<<<<< HEAD
int generic_file_mmap(struct file * file, struct vm_area_struct * vma)
{
	return -ENOSYS;
}
int generic_file_readonly_mmap(struct file * file, struct vm_area_struct * vma)
=======
vm_fault_t filemap_page_mkwrite(struct vm_fault *vmf)
{
	return VM_FAULT_SIGBUS;
}
int generic_file_mmap(struct file *file, struct vm_area_struct *vma)
{
	return -ENOSYS;
}
int generic_file_readonly_mmap(struct file *file, struct vm_area_struct *vma)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return -ENOSYS;
}
#endif /* CONFIG_MMU */

<<<<<<< HEAD
EXPORT_SYMBOL(generic_file_mmap);
EXPORT_SYMBOL(generic_file_readonly_mmap);

static struct page *__read_cache_page(struct address_space *mapping,
				pgoff_t index,
				int (*filler)(void *, struct page *),
				void *data,
				gfp_t gfp)
{
	struct page *page;
	int err;
repeat:
	page = find_get_page(mapping, index);
	if (!page) {
		page = __page_cache_alloc(gfp | __GFP_COLD);
		if (!page)
			return ERR_PTR(-ENOMEM);
		err = add_to_page_cache_lru(page, mapping, index, gfp);
		if (unlikely(err)) {
			page_cache_release(page);
			if (err == -EEXIST)
				goto repeat;
			/* Presumably ENOMEM for radix tree node */
			return ERR_PTR(err);
		}
		err = filler(data, page);
		if (err < 0) {
			page_cache_release(page);
			page = ERR_PTR(err);
		}
	}
	return page;
}

static struct page *do_read_cache_page(struct address_space *mapping,
				pgoff_t index,
				int (*filler)(void *, struct page *),
				void *data,
				gfp_t gfp)

{
	struct page *page;
	int err;

retry:
	page = __read_cache_page(mapping, index, filler, data, gfp);
	if (IS_ERR(page))
		return page;
	if (PageUptodate(page))
		goto out;

	lock_page(page);
	if (!page->mapping) {
		unlock_page(page);
		page_cache_release(page);
		goto retry;
	}
	if (PageUptodate(page)) {
		unlock_page(page);
		goto out;
	}
	err = filler(data, page);
	if (err < 0) {
		page_cache_release(page);
		return ERR_PTR(err);
	}
out:
	mark_page_accessed(page);
	return page;
}

/**
 * read_cache_page_async - read into page cache, fill it if needed
 * @mapping:	the page's address_space
 * @index:	the page index
 * @filler:	function to perform the read
 * @data:	first arg to filler(data, page) function, often left as NULL
 *
 * Same as read_cache_page, but don't wait for page to become unlocked
 * after submitting it to the filler.
 *
 * Read into the page cache. If a page already exists, and PageUptodate() is
 * not set, try to fill the page but don't wait for it to become unlocked.
 *
 * If the page does not get brought uptodate, return -EIO.
 */
struct page *read_cache_page_async(struct address_space *mapping,
				pgoff_t index,
				int (*filler)(void *, struct page *),
				void *data)
{
	return do_read_cache_page(mapping, index, filler, data, mapping_gfp_mask(mapping));
}
EXPORT_SYMBOL(read_cache_page_async);

static struct page *wait_on_page_read(struct page *page)
{
	if (!IS_ERR(page)) {
		wait_on_page_locked(page);
		if (!PageUptodate(page)) {
			page_cache_release(page);
			page = ERR_PTR(-EIO);
		}
	}
	return page;
}
=======
EXPORT_SYMBOL(filemap_page_mkwrite);
EXPORT_SYMBOL(generic_file_mmap);
EXPORT_SYMBOL(generic_file_readonly_mmap);

static struct folio *do_read_cache_folio(struct address_space *mapping,
		pgoff_t index, filler_t filler, struct file *file, gfp_t gfp)
{
	struct folio *folio;
	int err;

	if (!filler)
		filler = mapping->a_ops->read_folio;
repeat:
	folio = filemap_get_folio(mapping, index);
	if (IS_ERR(folio)) {
		folio = filemap_alloc_folio(gfp, 0);
		if (!folio)
			return ERR_PTR(-ENOMEM);
		err = filemap_add_folio(mapping, folio, index, gfp);
		if (unlikely(err)) {
			folio_put(folio);
			if (err == -EEXIST)
				goto repeat;
			/* Presumably ENOMEM for xarray node */
			return ERR_PTR(err);
		}

		goto filler;
	}
	if (folio_test_uptodate(folio))
		goto out;

	if (!folio_trylock(folio)) {
		folio_put_wait_locked(folio, TASK_UNINTERRUPTIBLE);
		goto repeat;
	}

	/* Folio was truncated from mapping */
	if (!folio->mapping) {
		folio_unlock(folio);
		folio_put(folio);
		goto repeat;
	}

	/* Someone else locked and filled the page in a very small window */
	if (folio_test_uptodate(folio)) {
		folio_unlock(folio);
		goto out;
	}

filler:
	err = filemap_read_folio(file, filler, folio);
	if (err) {
		folio_put(folio);
		if (err == AOP_TRUNCATED_PAGE)
			goto repeat;
		return ERR_PTR(err);
	}

out:
	folio_mark_accessed(folio);
	return folio;
}

/**
 * read_cache_folio - Read into page cache, fill it if needed.
 * @mapping: The address_space to read from.
 * @index: The index to read.
 * @filler: Function to perform the read, or NULL to use aops->read_folio().
 * @file: Passed to filler function, may be NULL if not required.
 *
 * Read one page into the page cache.  If it succeeds, the folio returned
 * will contain @index, but it may not be the first page of the folio.
 *
 * If the filler function returns an error, it will be returned to the
 * caller.
 *
 * Context: May sleep.  Expects mapping->invalidate_lock to be held.
 * Return: An uptodate folio on success, ERR_PTR() on failure.
 */
struct folio *read_cache_folio(struct address_space *mapping, pgoff_t index,
		filler_t filler, struct file *file)
{
	return do_read_cache_folio(mapping, index, filler, file,
			mapping_gfp_mask(mapping));
}
EXPORT_SYMBOL(read_cache_folio);

/**
 * mapping_read_folio_gfp - Read into page cache, using specified allocation flags.
 * @mapping:	The address_space for the folio.
 * @index:	The index that the allocated folio will contain.
 * @gfp:	The page allocator flags to use if allocating.
 *
 * This is the same as "read_cache_folio(mapping, index, NULL, NULL)", but with
 * any new memory allocations done using the specified allocation flags.
 *
 * The most likely error from this function is EIO, but ENOMEM is
 * possible and so is EINTR.  If ->read_folio returns another error,
 * that will be returned to the caller.
 *
 * The function expects mapping->invalidate_lock to be already held.
 *
 * Return: Uptodate folio on success, ERR_PTR() on failure.
 */
struct folio *mapping_read_folio_gfp(struct address_space *mapping,
		pgoff_t index, gfp_t gfp)
{
	return do_read_cache_folio(mapping, index, NULL, NULL, gfp);
}
EXPORT_SYMBOL(mapping_read_folio_gfp);

static struct page *do_read_cache_page(struct address_space *mapping,
		pgoff_t index, filler_t *filler, struct file *file, gfp_t gfp)
{
	struct folio *folio;

	folio = do_read_cache_folio(mapping, index, filler, file, gfp);
	if (IS_ERR(folio))
		return &folio->page;
	return folio_file_page(folio, index);
}

struct page *read_cache_page(struct address_space *mapping,
			pgoff_t index, filler_t *filler, struct file *file)
{
	return do_read_cache_page(mapping, index, filler, file,
			mapping_gfp_mask(mapping));
}
EXPORT_SYMBOL(read_cache_page);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * read_cache_page_gfp - read into page cache, using specified page allocation flags.
 * @mapping:	the page's address_space
 * @index:	the page index
 * @gfp:	the page allocator flags to use if allocating
 *
 * This is the same as "read_mapping_page(mapping, index, NULL)", but with
 * any new page allocations done using the specified allocation flags.
 *
 * If the page does not get brought uptodate, return -EIO.
<<<<<<< HEAD
=======
 *
 * The function expects mapping->invalidate_lock to be already held.
 *
 * Return: up to date page on success, ERR_PTR() on failure.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
struct page *read_cache_page_gfp(struct address_space *mapping,
				pgoff_t index,
				gfp_t gfp)
{
<<<<<<< HEAD
	filler_t *filler = (filler_t *)mapping->a_ops->readpage;

	return wait_on_page_read(do_read_cache_page(mapping, index, filler, NULL, gfp));
}
EXPORT_SYMBOL(read_cache_page_gfp);

/**
 * read_cache_page - read into page cache, fill it if needed
 * @mapping:	the page's address_space
 * @index:	the page index
 * @filler:	function to perform the read
 * @data:	first arg to filler(data, page) function, often left as NULL
 *
 * Read into the page cache. If a page already exists, and PageUptodate() is
 * not set, try to fill the page then wait for it to become unlocked.
 *
 * If the page does not get brought uptodate, return -EIO.
 */
struct page *read_cache_page(struct address_space *mapping,
				pgoff_t index,
				int (*filler)(void *, struct page *),
				void *data)
{
	return wait_on_page_read(read_cache_page_async(mapping, index, filler, data));
}
EXPORT_SYMBOL(read_cache_page);

/*
 * The logic we want is
 *
 *	if suid or (sgid and xgrp)
 *		remove privs
 */
int should_remove_suid(struct dentry *dentry)
{
	umode_t mode = dentry->d_inode->i_mode;
	int kill = 0;

	/* suid always must be killed */
	if (unlikely(mode & S_ISUID))
		kill = ATTR_KILL_SUID;

	/*
	 * sgid without any exec bits is just a mandatory locking mark; leave
	 * it alone.  If some exec bits are set, it's a real sgid; kill it.
	 */
	if (unlikely((mode & S_ISGID) && (mode & S_IXGRP)))
		kill |= ATTR_KILL_SGID;

	if (unlikely(kill && !capable(CAP_FSETID) && S_ISREG(mode)))
		return kill;

	return 0;
}
EXPORT_SYMBOL(should_remove_suid);

static int __remove_suid(struct dentry *dentry, int kill)
{
	struct iattr newattrs;

	newattrs.ia_valid = ATTR_FORCE | kill;
	return notify_change(dentry, &newattrs);
}

int file_remove_suid(struct file *file)
{
	struct dentry *dentry = file->f_path.dentry;
	struct inode *inode = dentry->d_inode;
	int killsuid;
	int killpriv;
	int error = 0;

	/* Fast path for nothing security related */
	if (IS_NOSEC(inode))
		return 0;

	killsuid = should_remove_suid(dentry);
	killpriv = security_inode_need_killpriv(dentry);

	if (killpriv < 0)
		return killpriv;
	if (killpriv)
		error = security_inode_killpriv(dentry);
	if (!error && killsuid)
		error = __remove_suid(dentry, killsuid);
	if (!error && (inode->i_sb->s_flags & MS_NOSEC))
		inode->i_flags |= S_NOSEC;

	return error;
}
EXPORT_SYMBOL(file_remove_suid);

static size_t __iovec_copy_from_user_inatomic(char *vaddr,
			const struct iovec *iov, size_t base, size_t bytes)
{
	size_t copied = 0, left = 0;

	while (bytes) {
		char __user *buf = iov->iov_base + base;
		int copy = min(bytes, iov->iov_len - base);

		base = 0;
		left = __copy_from_user_inatomic(vaddr, buf, copy);
		copied += copy;
		bytes -= copy;
		vaddr += copy;
		iov++;

		if (unlikely(left))
			break;
	}
	return copied - left;
}

/*
 * Copy as much as we can into the page and return the number of bytes which
 * were successfully copied.  If a fault is encountered then return the number of
 * bytes which were copied.
 */
size_t iov_iter_copy_from_user_atomic(struct page *page,
		struct iov_iter *i, unsigned long offset, size_t bytes)
{
	char *kaddr;
	size_t copied;

	BUG_ON(!in_atomic());
	kaddr = kmap_atomic(page);
	if (likely(i->nr_segs == 1)) {
		int left;
		char __user *buf = i->iov->iov_base + i->iov_offset;
		left = __copy_from_user_inatomic(kaddr + offset, buf, bytes);
		copied = bytes - left;
	} else {
		copied = __iovec_copy_from_user_inatomic(kaddr + offset,
						i->iov, i->iov_offset, bytes);
	}
	kunmap_atomic(kaddr);

	return copied;
}
EXPORT_SYMBOL(iov_iter_copy_from_user_atomic);

/*
 * This has the same sideeffects and return value as
 * iov_iter_copy_from_user_atomic().
 * The difference is that it attempts to resolve faults.
 * Page must not be locked.
 */
size_t iov_iter_copy_from_user(struct page *page,
		struct iov_iter *i, unsigned long offset, size_t bytes)
{
	char *kaddr;
	size_t copied;

	kaddr = kmap(page);
	if (likely(i->nr_segs == 1)) {
		int left;
		char __user *buf = i->iov->iov_base + i->iov_offset;
		left = __copy_from_user(kaddr + offset, buf, bytes);
		copied = bytes - left;
	} else {
		copied = __iovec_copy_from_user_inatomic(kaddr + offset,
						i->iov, i->iov_offset, bytes);
	}
	kunmap(page);
	return copied;
}
EXPORT_SYMBOL(iov_iter_copy_from_user);

void iov_iter_advance(struct iov_iter *i, size_t bytes)
{
	BUG_ON(i->count < bytes);

	if (likely(i->nr_segs == 1)) {
		i->iov_offset += bytes;
		i->count -= bytes;
	} else {
		const struct iovec *iov = i->iov;
		size_t base = i->iov_offset;
		unsigned long nr_segs = i->nr_segs;

		/*
		 * The !iov->iov_len check ensures we skip over unlikely
		 * zero-length segments (without overruning the iovec).
		 */
		while (bytes || unlikely(i->count && !iov->iov_len)) {
			int copy;

			copy = min(bytes, iov->iov_len - base);
			BUG_ON(!i->count || i->count < copy);
			i->count -= copy;
			bytes -= copy;
			base += copy;
			if (iov->iov_len == base) {
				iov++;
				nr_segs--;
				base = 0;
			}
		}
		i->iov = iov;
		i->iov_offset = base;
		i->nr_segs = nr_segs;
	}
}
EXPORT_SYMBOL(iov_iter_advance);

/*
 * Fault in the first iovec of the given iov_iter, to a maximum length
 * of bytes. Returns 0 on success, or non-zero if the memory could not be
 * accessed (ie. because it is an invalid address).
 *
 * writev-intensive code may want this to prefault several iovecs -- that
 * would be possible (callers must not rely on the fact that _only_ the
 * first iovec will be faulted with the current implementation).
 */
int iov_iter_fault_in_readable(struct iov_iter *i, size_t bytes)
{
	char __user *buf = i->iov->iov_base + i->iov_offset;
	bytes = min(bytes, i->iov->iov_len - i->iov_offset);
	return fault_in_pages_readable(buf, bytes);
}
EXPORT_SYMBOL(iov_iter_fault_in_readable);

/*
 * Return the count of just the current iov_iter segment.
 */
size_t iov_iter_single_seg_count(struct iov_iter *i)
{
	const struct iovec *iov = i->iov;
	if (i->nr_segs == 1)
		return i->count;
	else
		return min(i->count, iov->iov_len - i->iov_offset);
}
EXPORT_SYMBOL(iov_iter_single_seg_count);

/*
 * Performs necessary checks before doing a write
 *
 * Can adjust writing position or amount of bytes to write.
 * Returns appropriate error code that caller should return or
 * zero in case that write should be allowed.
 */
inline int generic_write_checks(struct file *file, loff_t *pos, size_t *count, int isblk)
{
	struct inode *inode = file->f_mapping->host;
	unsigned long limit = rlimit(RLIMIT_FSIZE);

        if (unlikely(*pos < 0))
                return -EINVAL;

	if (!isblk) {
		/* FIXME: this is for backwards compatibility with 2.4 */
		if (file->f_flags & O_APPEND)
                        *pos = i_size_read(inode);

		if (limit != RLIM_INFINITY) {
			if (*pos >= limit) {
				send_sig(SIGXFSZ, current, 0);
				return -EFBIG;
			}
			if (*count > limit - (typeof(limit))*pos) {
				*count = limit - (typeof(limit))*pos;
			}
		}
	}

	/*
	 * LFS rule
	 */
	if (unlikely(*pos + *count > MAX_NON_LFS &&
				!(file->f_flags & O_LARGEFILE))) {
		if (*pos >= MAX_NON_LFS) {
			return -EFBIG;
		}
		if (*count > MAX_NON_LFS - (unsigned long)*pos) {
			*count = MAX_NON_LFS - (unsigned long)*pos;
		}
	}

	/*
	 * Are we about to exceed the fs block limit ?
	 *
	 * If we have written data it becomes a short write.  If we have
	 * exceeded without writing data we send a signal and return EFBIG.
	 * Linus frestrict idea will clean these up nicely..
	 */
	if (likely(!isblk)) {
		if (unlikely(*pos >= inode->i_sb->s_maxbytes)) {
			if (*count || *pos > inode->i_sb->s_maxbytes) {
				return -EFBIG;
			}
			/* zero-length writes at ->s_maxbytes are OK */
		}

		if (unlikely(*pos + *count > inode->i_sb->s_maxbytes))
			*count = inode->i_sb->s_maxbytes - *pos;
	} else {
#ifdef CONFIG_BLOCK
		loff_t isize;
		if (bdev_read_only(I_BDEV(inode)))
			return -EPERM;
		isize = i_size_read(inode);
		if (*pos >= isize) {
			if (*count || *pos > isize)
				return -ENOSPC;
		}

		if (*pos + *count > isize)
			*count = isize - *pos;
#else
		return -EPERM;
#endif
	}
	return 0;
}
EXPORT_SYMBOL(generic_write_checks);

int pagecache_write_begin(struct file *file, struct address_space *mapping,
				loff_t pos, unsigned len, unsigned flags,
				struct page **pagep, void **fsdata)
{
	const struct address_space_operations *aops = mapping->a_ops;

	return aops->write_begin(file, mapping, pos, len, flags,
							pagep, fsdata);
}
EXPORT_SYMBOL(pagecache_write_begin);

int pagecache_write_end(struct file *file, struct address_space *mapping,
				loff_t pos, unsigned len, unsigned copied,
				struct page *page, void *fsdata)
{
	const struct address_space_operations *aops = mapping->a_ops;

	mark_page_accessed(page);
	return aops->write_end(file, mapping, pos, len, copied, page, fsdata);
}
EXPORT_SYMBOL(pagecache_write_end);

ssize_t
generic_file_direct_write(struct kiocb *iocb, const struct iovec *iov,
		unsigned long *nr_segs, loff_t pos, loff_t *ppos,
		size_t count, size_t ocount)
{
	struct file	*file = iocb->ki_filp;
	struct address_space *mapping = file->f_mapping;
	struct inode	*inode = mapping->host;
	ssize_t		written;
	size_t		write_len;
	pgoff_t		end;

	if (count != ocount)
		*nr_segs = iov_shorten((struct iovec *)iov, *nr_segs, count);

	write_len = iov_length(iov, *nr_segs);
	end = (pos + write_len - 1) >> PAGE_CACHE_SHIFT;

	written = filemap_write_and_wait_range(mapping, pos, pos + write_len - 1);
	if (written)
		goto out;

	/*
	 * After a write we want buffered reads to be sure to go to disk to get
	 * the new data.  We invalidate clean cached page from the region we're
	 * about to write.  We do this *before* the write so that we can return
	 * without clobbering -EIOCBQUEUED from ->direct_IO().
	 */
	if (mapping->nrpages) {
		written = invalidate_inode_pages2_range(mapping,
					pos >> PAGE_CACHE_SHIFT, end);
		/*
		 * If a page can not be invalidated, return 0 to fall back
		 * to buffered write.
		 */
		if (written) {
			if (written == -EBUSY)
				return 0;
			goto out;
		}
	}

	written = mapping->a_ops->direct_IO(WRITE, iocb, iov, pos, *nr_segs);
=======
	return do_read_cache_page(mapping, index, NULL, NULL, gfp);
}
EXPORT_SYMBOL(read_cache_page_gfp);

/*
 * Warn about a page cache invalidation failure during a direct I/O write.
 */
static void dio_warn_stale_pagecache(struct file *filp)
{
	static DEFINE_RATELIMIT_STATE(_rs, 86400 * HZ, DEFAULT_RATELIMIT_BURST);
	char pathname[128];
	char *path;

	errseq_set(&filp->f_mapping->wb_err, -EIO);
	if (__ratelimit(&_rs)) {
		path = file_path(filp, pathname, sizeof(pathname));
		if (IS_ERR(path))
			path = "(unknown)";
		pr_crit("Page cache invalidation failure on direct I/O.  Possible data corruption due to collision with buffered I/O!\n");
		pr_crit("File: %s PID: %d Comm: %.20s\n", path, current->pid,
			current->comm);
	}
}

void kiocb_invalidate_post_direct_write(struct kiocb *iocb, size_t count)
{
	struct address_space *mapping = iocb->ki_filp->f_mapping;

	if (mapping->nrpages &&
	    invalidate_inode_pages2_range(mapping,
			iocb->ki_pos >> PAGE_SHIFT,
			(iocb->ki_pos + count - 1) >> PAGE_SHIFT))
		dio_warn_stale_pagecache(iocb->ki_filp);
}

ssize_t
generic_file_direct_write(struct kiocb *iocb, struct iov_iter *from)
{
	struct address_space *mapping = iocb->ki_filp->f_mapping;
	size_t write_len = iov_iter_count(from);
	ssize_t written;

	/*
	 * If a page can not be invalidated, return 0 to fall back
	 * to buffered write.
	 */
	written = kiocb_invalidate_pages(iocb, write_len);
	if (written) {
		if (written == -EBUSY)
			return 0;
		return written;
	}

	written = mapping->a_ops->direct_IO(iocb, from);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Finally, try again to invalidate clean pages which might have been
	 * cached by non-direct readahead, or faulted in by get_user_pages()
	 * if the source of the write was an mmap'ed region of the file
	 * we're writing.  Either one is a pretty crazy thing to do,
	 * so we don't support it 100%.  If this invalidation
	 * fails, tough, the write still worked...
<<<<<<< HEAD
	 */
	if (mapping->nrpages) {
		invalidate_inode_pages2_range(mapping,
					      pos >> PAGE_CACHE_SHIFT, end);
	}

	if (written > 0) {
		pos += written;
=======
	 *
	 * Most of the time we do not need this since dio_complete() will do
	 * the invalidation for us. However there are some file systems that
	 * do not end up with dio_complete() being called, so let's not break
	 * them by removing it completely.
	 *
	 * Noticeable example is a blkdev_direct_IO().
	 *
	 * Skip invalidation for async writes or if mapping has no pages.
	 */
	if (written > 0) {
		struct inode *inode = mapping->host;
		loff_t pos = iocb->ki_pos;

		kiocb_invalidate_post_direct_write(iocb, written);
		pos += written;
		write_len -= written;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (pos > i_size_read(inode) && !S_ISBLK(inode->i_mode)) {
			i_size_write(inode, pos);
			mark_inode_dirty(inode);
		}
<<<<<<< HEAD
		*ppos = pos;
	}
out:
=======
		iocb->ki_pos = pos;
	}
	if (written != -EIOCBQUEUED)
		iov_iter_revert(from, write_len - iov_iter_count(from));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return written;
}
EXPORT_SYMBOL(generic_file_direct_write);

<<<<<<< HEAD
/*
 * Find or create a page at the given pagecache position. Return the locked
 * page. This function is specifically for buffered writes.
 */
struct page *grab_cache_page_write_begin(struct address_space *mapping,
					pgoff_t index, unsigned flags)
{
	int status;
	gfp_t gfp_mask;
	struct page *page;
	gfp_t gfp_notmask = 0;

	gfp_mask = mapping_gfp_mask(mapping);
	if (mapping_cap_account_dirty(mapping))
		gfp_mask |= __GFP_WRITE;
	if (flags & AOP_FLAG_NOFS)
		gfp_notmask = __GFP_FS;
repeat:
	page = find_lock_page(mapping, index);
	if (page)
		goto found;

retry:
	page = __page_cache_alloc(gfp_mask & ~gfp_notmask);
	if (!page)
		return NULL;

	if (is_cma_pageblock(page)) {
		__free_page(page);
		gfp_notmask |= __GFP_MOVABLE;
		goto retry;
	}

	status = add_to_page_cache_lru(page, mapping, index,
						GFP_KERNEL & ~gfp_notmask);
	if (unlikely(status)) {
		page_cache_release(page);
		if (status == -EEXIST)
			goto repeat;
		return NULL;
	}
found:
	wait_on_page_writeback(page);
	return page;
}
EXPORT_SYMBOL(grab_cache_page_write_begin);

static ssize_t generic_perform_write(struct file *file,
				struct iov_iter *i, loff_t pos)
{
=======
ssize_t generic_perform_write(struct kiocb *iocb, struct iov_iter *i)
{
	struct file *file = iocb->ki_filp;
	loff_t pos = iocb->ki_pos;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct address_space *mapping = file->f_mapping;
	const struct address_space_operations *a_ops = mapping->a_ops;
	long status = 0;
	ssize_t written = 0;
<<<<<<< HEAD
	unsigned int flags = 0;

	/*
	 * Copies from kernel address space cannot fail (NFSD is a big user).
	 */
	if (segment_eq(get_fs(), KERNEL_DS))
		flags |= AOP_FLAG_UNINTERRUPTIBLE;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	do {
		struct page *page;
		unsigned long offset;	/* Offset into pagecache page */
		unsigned long bytes;	/* Bytes to write to page */
		size_t copied;		/* Bytes copied from user */
<<<<<<< HEAD
		void *fsdata;

		offset = (pos & (PAGE_CACHE_SIZE - 1));
		bytes = min_t(unsigned long, PAGE_CACHE_SIZE - offset,
=======
		void *fsdata = NULL;

		offset = (pos & (PAGE_SIZE - 1));
		bytes = min_t(unsigned long, PAGE_SIZE - offset,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
						iov_iter_count(i));

again:
		/*
		 * Bring in the user page that we will copy from _first_.
		 * Otherwise there's a nasty deadlock on copying from the
		 * same page as we're writing to, without it being marked
		 * up-to-date.
<<<<<<< HEAD
		 *
		 * Not only is this an optimisation, but it is also required
		 * to check that the address is actually valid, when atomic
		 * usercopies are used, below.
		 */
		if (unlikely(iov_iter_fault_in_readable(i, bytes))) {
=======
		 */
		if (unlikely(fault_in_iov_iter_readable(i, bytes) == bytes)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			status = -EFAULT;
			break;
		}

		if (fatal_signal_pending(current)) {
			status = -EINTR;
			break;
		}

<<<<<<< HEAD
		status = a_ops->write_begin(file, mapping, pos, bytes, flags,
						&page, &fsdata);
		if (unlikely(status))
=======
		status = a_ops->write_begin(file, mapping, pos, bytes,
						&page, &fsdata);
		if (unlikely(status < 0))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;

		if (mapping_writably_mapped(mapping))
			flush_dcache_page(page);

<<<<<<< HEAD
		pagefault_disable();
		copied = iov_iter_copy_from_user_atomic(page, i, offset, bytes);
		pagefault_enable();
		flush_dcache_page(page);

		mark_page_accessed(page);
		status = a_ops->write_end(file, mapping, pos, bytes, copied,
						page, fsdata);
		if (unlikely(status < 0))
			break;
		copied = status;

		cond_resched();

		iov_iter_advance(i, copied);
		if (unlikely(copied == 0)) {
			/*
			 * If we were unable to copy any data at all, we must
			 * fall back to a single segment length write.
			 *
			 * If we didn't fallback here, we could livelock
			 * because not all segments in the iov can be copied at
			 * once without a pagefault.
			 */
			bytes = min_t(unsigned long, PAGE_CACHE_SIZE - offset,
						iov_iter_single_seg_count(i));
			goto again;
		}
		pos += copied;
		written += copied;
=======
		copied = copy_page_from_iter_atomic(page, offset, bytes, i);
		flush_dcache_page(page);

		status = a_ops->write_end(file, mapping, pos, bytes, copied,
						page, fsdata);
		if (unlikely(status != copied)) {
			iov_iter_revert(i, copied - max(status, 0L));
			if (unlikely(status < 0))
				break;
		}
		cond_resched();

		if (unlikely(status == 0)) {
			/*
			 * A short copy made ->write_end() reject the
			 * thing entirely.  Might be memory poisoning
			 * halfway through, might be a race with munmap,
			 * might be severe memory pressure.
			 */
			if (copied)
				bytes = copied;
			goto again;
		}
		pos += status;
		written += status;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		balance_dirty_pages_ratelimited(mapping);
	} while (iov_iter_count(i));

<<<<<<< HEAD
	return written ? written : status;
}

ssize_t
generic_file_buffered_write(struct kiocb *iocb, const struct iovec *iov,
		unsigned long nr_segs, loff_t pos, loff_t *ppos,
		size_t count, ssize_t written)
{
	struct file *file = iocb->ki_filp;
	ssize_t status;
	struct iov_iter i;

	iov_iter_init(&i, iov, nr_segs, count, written);
	status = generic_perform_write(file, &i, pos);

	if (likely(status >= 0)) {
		written += status;
		*ppos = pos + status;
  	}
	
	return written ? written : status;
}
EXPORT_SYMBOL(generic_file_buffered_write);

/**
 * __generic_file_aio_write - write data to a file
 * @iocb:	IO state structure (file, offset, etc.)
 * @iov:	vector with data to write
 * @nr_segs:	number of segments in the vector
 * @ppos:	position where to write
=======
	if (!written)
		return status;
	iocb->ki_pos += written;
	return written;
}
EXPORT_SYMBOL(generic_perform_write);

/**
 * __generic_file_write_iter - write data to a file
 * @iocb:	IO state structure (file, offset, etc.)
 * @from:	iov_iter with data to write
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * This function does all the work needed for actually writing data to a
 * file. It does all basic checks, removes SUID from the file, updates
 * modification times and calls proper subroutines depending on whether we
 * do direct IO or a standard buffered write.
 *
<<<<<<< HEAD
 * It expects i_mutex to be grabbed unless we work on a block device or similar
=======
 * It expects i_rwsem to be grabbed unless we work on a block device or similar
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * object which does not need locking at all.
 *
 * This function does *not* take care of syncing data in case of O_SYNC write.
 * A caller has to handle it. This is mainly due to the fact that we want to
<<<<<<< HEAD
 * avoid syncing under i_mutex.
 */
ssize_t __generic_file_aio_write(struct kiocb *iocb, const struct iovec *iov,
				 unsigned long nr_segs, loff_t *ppos)
{
	struct file *file = iocb->ki_filp;
	struct address_space * mapping = file->f_mapping;
	size_t ocount;		/* original count */
	size_t count;		/* after file limit checks */
	struct inode 	*inode = mapping->host;
	loff_t		pos;
	ssize_t		written;
	ssize_t		err;

	ocount = 0;
	err = generic_segment_checks(iov, &nr_segs, &ocount, VERIFY_READ);
	if (err)
		return err;

	count = ocount;
	pos = *ppos;

	vfs_check_frozen(inode->i_sb, SB_FREEZE_WRITE);

	/* We can write back this queue in page reclaim */
	current->backing_dev_info = mapping->backing_dev_info;
	written = 0;

	err = generic_write_checks(file, &pos, &count, S_ISBLK(inode->i_mode));
	if (err)
		goto out;

	if (count == 0)
		goto out;

	err = file_remove_suid(file);
	if (err)
		goto out;

	err = file_update_time(file);
	if (err)
		goto out;

	/* coalesce the iovecs and go direct-to-BIO for O_DIRECT */
	if (unlikely(file->f_flags & O_DIRECT)) {
		loff_t endbyte;
		ssize_t written_buffered;

		written = generic_file_direct_write(iocb, iov, &nr_segs, pos,
							ppos, count, ocount);
		if (written < 0 || written == count)
			goto out;
		/*
		 * direct-io write to a hole: fall through to buffered I/O
		 * for completing the rest of the request.
		 */
		pos += written;
		count -= written;
		written_buffered = generic_file_buffered_write(iocb, iov,
						nr_segs, pos, ppos, count,
						written);
		/*
		 * If generic_file_buffered_write() retuned a synchronous error
		 * then we want to return the number of bytes which were
		 * direct-written, or the error code if that was zero.  Note
		 * that this differs from normal direct-io semantics, which
		 * will return -EFOO even if some bytes were written.
		 */
		if (written_buffered < 0) {
			err = written_buffered;
			goto out;
		}

		/*
		 * We need to ensure that the page cache pages are written to
		 * disk and invalidated to preserve the expected O_DIRECT
		 * semantics.
		 */
		endbyte = pos + written_buffered - written - 1;
		err = filemap_write_and_wait_range(file->f_mapping, pos, endbyte);
		if (err == 0) {
			written = written_buffered;
			invalidate_mapping_pages(mapping,
						 pos >> PAGE_CACHE_SHIFT,
						 endbyte >> PAGE_CACHE_SHIFT);
		} else {
			/*
			 * We don't know how much we wrote, so just return
			 * the number of bytes which were direct-written
			 */
		}
	} else {
		written = generic_file_buffered_write(iocb, iov, nr_segs,
				pos, ppos, count, written);
	}
out:
	current->backing_dev_info = NULL;
	return written ? written : err;
}
EXPORT_SYMBOL(__generic_file_aio_write);

/**
 * generic_file_aio_write - write data to a file
 * @iocb:	IO state structure
 * @iov:	vector with data to write
 * @nr_segs:	number of segments in the vector
 * @pos:	position in file where to write
 *
 * This is a wrapper around __generic_file_aio_write() to be used by most
 * filesystems. It takes care of syncing the file in case of O_SYNC file
 * and acquires i_mutex as needed.
 */
ssize_t generic_file_aio_write(struct kiocb *iocb, const struct iovec *iov,
		unsigned long nr_segs, loff_t pos)
{
	struct file *file = iocb->ki_filp;
	struct inode *inode = file->f_mapping->host;
	struct blk_plug plug;
	ssize_t ret;

	BUG_ON(iocb->ki_pos != pos);

	mutex_lock(&inode->i_mutex);
	blk_start_plug(&plug);
	ret = __generic_file_aio_write(iocb, iov, nr_segs, &iocb->ki_pos);
	mutex_unlock(&inode->i_mutex);

	if (ret > 0 || ret == -EIOCBQUEUED) {
		ssize_t err;

		err = generic_write_sync(file, pos, ret);
		if (err < 0 && ret > 0)
			ret = err;
	}
	blk_finish_plug(&plug);
	return ret;
}
EXPORT_SYMBOL(generic_file_aio_write);

/**
 * try_to_release_page() - release old fs-specific metadata on a page
 *
 * @page: the page which the kernel is trying to free
 * @gfp_mask: memory allocation flags (and I/O mode)
 *
 * The address_space is to try to release any data against the page
 * (presumably at page->private).  If the release was successful, return `1'.
 * Otherwise return zero.
 *
 * This may also be called if PG_fscache is set on a page, indicating that the
 * page is known to the local caching routines.
 *
 * The @gfp_mask argument specifies whether I/O may be performed to release
 * this page (__GFP_IO), and whether the call may block (__GFP_WAIT & __GFP_FS).
 *
 */
int try_to_release_page(struct page *page, gfp_t gfp_mask)
{
	struct address_space * const mapping = page->mapping;

	BUG_ON(!PageLocked(page));
	if (PageWriteback(page))
		return 0;

	if (mapping && mapping->a_ops->releasepage)
		return mapping->a_ops->releasepage(page, gfp_mask);
	return try_to_free_buffers(page);
}

EXPORT_SYMBOL(try_to_release_page);
=======
 * avoid syncing under i_rwsem.
 *
 * Return:
 * * number of bytes written, even for truncated writes
 * * negative error code if no data has been written at all
 */
ssize_t __generic_file_write_iter(struct kiocb *iocb, struct iov_iter *from)
{
	struct file *file = iocb->ki_filp;
	struct address_space *mapping = file->f_mapping;
	struct inode *inode = mapping->host;
	ssize_t ret;

	ret = file_remove_privs(file);
	if (ret)
		return ret;

	ret = file_update_time(file);
	if (ret)
		return ret;

	if (iocb->ki_flags & IOCB_DIRECT) {
		ret = generic_file_direct_write(iocb, from);
		/*
		 * If the write stopped short of completing, fall back to
		 * buffered writes.  Some filesystems do this for writes to
		 * holes, for example.  For DAX files, a buffered write will
		 * not succeed (even if it did, DAX does not handle dirty
		 * page-cache pages correctly).
		 */
		if (ret < 0 || !iov_iter_count(from) || IS_DAX(inode))
			return ret;
		return direct_write_fallback(iocb, from, ret,
				generic_perform_write(iocb, from));
	}

	return generic_perform_write(iocb, from);
}
EXPORT_SYMBOL(__generic_file_write_iter);

/**
 * generic_file_write_iter - write data to a file
 * @iocb:	IO state structure
 * @from:	iov_iter with data to write
 *
 * This is a wrapper around __generic_file_write_iter() to be used by most
 * filesystems. It takes care of syncing the file in case of O_SYNC file
 * and acquires i_rwsem as needed.
 * Return:
 * * negative error code if no data has been written at all of
 *   vfs_fsync_range() failed for a synchronous write
 * * number of bytes written, even for truncated writes
 */
ssize_t generic_file_write_iter(struct kiocb *iocb, struct iov_iter *from)
{
	struct file *file = iocb->ki_filp;
	struct inode *inode = file->f_mapping->host;
	ssize_t ret;

	inode_lock(inode);
	ret = generic_write_checks(iocb, from);
	if (ret > 0)
		ret = __generic_file_write_iter(iocb, from);
	inode_unlock(inode);

	if (ret > 0)
		ret = generic_write_sync(iocb, ret);
	return ret;
}
EXPORT_SYMBOL(generic_file_write_iter);

/**
 * filemap_release_folio() - Release fs-specific metadata on a folio.
 * @folio: The folio which the kernel is trying to free.
 * @gfp: Memory allocation flags (and I/O mode).
 *
 * The address_space is trying to release any data attached to a folio
 * (presumably at folio->private).
 *
 * This will also be called if the private_2 flag is set on a page,
 * indicating that the folio has other metadata associated with it.
 *
 * The @gfp argument specifies whether I/O may be performed to release
 * this page (__GFP_IO), and whether the call may block
 * (__GFP_RECLAIM & __GFP_FS).
 *
 * Return: %true if the release was successful, otherwise %false.
 */
bool filemap_release_folio(struct folio *folio, gfp_t gfp)
{
	struct address_space * const mapping = folio->mapping;

	BUG_ON(!folio_test_locked(folio));
	if (!folio_needs_release(folio))
		return true;
	if (folio_test_writeback(folio))
		return false;

	if (mapping && mapping->a_ops->release_folio)
		return mapping->a_ops->release_folio(folio, gfp);
	return try_to_free_buffers(folio);
}
EXPORT_SYMBOL(filemap_release_folio);

#ifdef CONFIG_CACHESTAT_SYSCALL
/**
 * filemap_cachestat() - compute the page cache statistics of a mapping
 * @mapping:	The mapping to compute the statistics for.
 * @first_index:	The starting page cache index.
 * @last_index:	The final page index (inclusive).
 * @cs:	the cachestat struct to write the result to.
 *
 * This will query the page cache statistics of a mapping in the
 * page range of [first_index, last_index] (inclusive). The statistics
 * queried include: number of dirty pages, number of pages marked for
 * writeback, and the number of (recently) evicted pages.
 */
static void filemap_cachestat(struct address_space *mapping,
		pgoff_t first_index, pgoff_t last_index, struct cachestat *cs)
{
	XA_STATE(xas, &mapping->i_pages, first_index);
	struct folio *folio;

	rcu_read_lock();
	xas_for_each(&xas, folio, last_index) {
		int order;
		unsigned long nr_pages;
		pgoff_t folio_first_index, folio_last_index;

		/*
		 * Don't deref the folio. It is not pinned, and might
		 * get freed (and reused) underneath us.
		 *
		 * We *could* pin it, but that would be expensive for
		 * what should be a fast and lightweight syscall.
		 *
		 * Instead, derive all information of interest from
		 * the rcu-protected xarray.
		 */

		if (xas_retry(&xas, folio))
			continue;

		order = xa_get_order(xas.xa, xas.xa_index);
		nr_pages = 1 << order;
		folio_first_index = round_down(xas.xa_index, 1 << order);
		folio_last_index = folio_first_index + nr_pages - 1;

		/* Folios might straddle the range boundaries, only count covered pages */
		if (folio_first_index < first_index)
			nr_pages -= first_index - folio_first_index;

		if (folio_last_index > last_index)
			nr_pages -= folio_last_index - last_index;

		if (xa_is_value(folio)) {
			/* page is evicted */
			void *shadow = (void *)folio;
			bool workingset; /* not used */

			cs->nr_evicted += nr_pages;

#ifdef CONFIG_SWAP /* implies CONFIG_MMU */
			if (shmem_mapping(mapping)) {
				/* shmem file - in swap cache */
				swp_entry_t swp = radix_to_swp_entry(folio);

				/* swapin error results in poisoned entry */
				if (non_swap_entry(swp))
					goto resched;

				/*
				 * Getting a swap entry from the shmem
				 * inode means we beat
				 * shmem_unuse(). rcu_read_lock()
				 * ensures swapoff waits for us before
				 * freeing the swapper space. However,
				 * we can race with swapping and
				 * invalidation, so there might not be
				 * a shadow in the swapcache (yet).
				 */
				shadow = get_shadow_from_swap_cache(swp);
				if (!shadow)
					goto resched;
			}
#endif
			if (workingset_test_recent(shadow, true, &workingset))
				cs->nr_recently_evicted += nr_pages;

			goto resched;
		}

		/* page is in cache */
		cs->nr_cache += nr_pages;

		if (xas_get_mark(&xas, PAGECACHE_TAG_DIRTY))
			cs->nr_dirty += nr_pages;

		if (xas_get_mark(&xas, PAGECACHE_TAG_WRITEBACK))
			cs->nr_writeback += nr_pages;

resched:
		if (need_resched()) {
			xas_pause(&xas);
			cond_resched_rcu();
		}
	}
	rcu_read_unlock();
}

/*
 * The cachestat(2) system call.
 *
 * cachestat() returns the page cache statistics of a file in the
 * bytes range specified by `off` and `len`: number of cached pages,
 * number of dirty pages, number of pages marked for writeback,
 * number of evicted pages, and number of recently evicted pages.
 *
 * An evicted page is a page that is previously in the page cache
 * but has been evicted since. A page is recently evicted if its last
 * eviction was recent enough that its reentry to the cache would
 * indicate that it is actively being used by the system, and that
 * there is memory pressure on the system.
 *
 * `off` and `len` must be non-negative integers. If `len` > 0,
 * the queried range is [`off`, `off` + `len`]. If `len` == 0,
 * we will query in the range from `off` to the end of the file.
 *
 * The `flags` argument is unused for now, but is included for future
 * extensibility. User should pass 0 (i.e no flag specified).
 *
 * Currently, hugetlbfs is not supported.
 *
 * Because the status of a page can change after cachestat() checks it
 * but before it returns to the application, the returned values may
 * contain stale information.
 *
 * return values:
 *  zero        - success
 *  -EFAULT     - cstat or cstat_range points to an illegal address
 *  -EINVAL     - invalid flags
 *  -EBADF      - invalid file descriptor
 *  -EOPNOTSUPP - file descriptor is of a hugetlbfs file
 */
SYSCALL_DEFINE4(cachestat, unsigned int, fd,
		struct cachestat_range __user *, cstat_range,
		struct cachestat __user *, cstat, unsigned int, flags)
{
	struct fd f = fdget(fd);
	struct address_space *mapping;
	struct cachestat_range csr;
	struct cachestat cs;
	pgoff_t first_index, last_index;

	if (!f.file)
		return -EBADF;

	if (copy_from_user(&csr, cstat_range,
			sizeof(struct cachestat_range))) {
		fdput(f);
		return -EFAULT;
	}

	/* hugetlbfs is not supported */
	if (is_file_hugepages(f.file)) {
		fdput(f);
		return -EOPNOTSUPP;
	}

	if (flags != 0) {
		fdput(f);
		return -EINVAL;
	}

	first_index = csr.off >> PAGE_SHIFT;
	last_index =
		csr.len == 0 ? ULONG_MAX : (csr.off + csr.len - 1) >> PAGE_SHIFT;
	memset(&cs, 0, sizeof(struct cachestat));
	mapping = f.file->f_mapping;
	filemap_cachestat(mapping, first_index, last_index, &cs);
	fdput(f);

	if (copy_to_user(cstat, &cs, sizeof(struct cachestat)))
		return -EFAULT;

	return 0;
}
#endif /* CONFIG_CACHESTAT_SYSCALL */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
