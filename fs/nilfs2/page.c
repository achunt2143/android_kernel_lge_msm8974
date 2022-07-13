<<<<<<< HEAD
/*
 * page.c - buffer/page management specific to NILFS
 *
 * Copyright (C) 2005-2008 Nippon Telegraph and Telephone Corporation.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Written by Ryusuke Konishi <ryusuke@osrg.net>,
 *            Seiji Kihara <kihara@osrg.net>.
=======
// SPDX-License-Identifier: GPL-2.0+
/*
 * Buffer/page management specific to NILFS
 *
 * Copyright (C) 2005-2008 Nippon Telegraph and Telephone Corporation.
 *
 * Written by Ryusuke Konishi and Seiji Kihara.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/pagemap.h>
#include <linux/writeback.h>
#include <linux/swap.h>
#include <linux/bitops.h>
#include <linux/page-flags.h>
#include <linux/list.h>
#include <linux/highmem.h>
#include <linux/pagevec.h>
#include <linux/gfp.h>
#include "nilfs.h"
#include "page.h"
#include "mdt.h"


<<<<<<< HEAD
#define NILFS_BUFFER_INHERENT_BITS  \
	((1UL << BH_Uptodate) | (1UL << BH_Mapped) | (1UL << BH_NILFS_Node) | \
	 (1UL << BH_NILFS_Volatile) | (1UL << BH_NILFS_Checked))

static struct buffer_head *
__nilfs_get_page_block(struct page *page, unsigned long block, pgoff_t index,
		       int blkbits, unsigned long b_state)

{
	unsigned long first_block;
	struct buffer_head *bh;

	if (!page_has_buffers(page))
		create_empty_buffers(page, 1 << blkbits, b_state);

	first_block = (unsigned long)index << (PAGE_CACHE_SHIFT - blkbits);
	bh = nilfs_page_get_nth_block(page, block - first_block);
=======
#define NILFS_BUFFER_INHERENT_BITS					\
	(BIT(BH_Uptodate) | BIT(BH_Mapped) | BIT(BH_NILFS_Node) |	\
	 BIT(BH_NILFS_Volatile) | BIT(BH_NILFS_Checked))

static struct buffer_head *__nilfs_get_folio_block(struct folio *folio,
		unsigned long block, pgoff_t index, int blkbits,
		unsigned long b_state)

{
	unsigned long first_block;
	struct buffer_head *bh = folio_buffers(folio);

	if (!bh)
		bh = create_empty_buffers(folio, 1 << blkbits, b_state);

	first_block = (unsigned long)index << (PAGE_SHIFT - blkbits);
	bh = get_nth_bh(bh, block - first_block);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	touch_buffer(bh);
	wait_on_buffer(bh);
	return bh;
}

struct buffer_head *nilfs_grab_buffer(struct inode *inode,
				      struct address_space *mapping,
				      unsigned long blkoff,
				      unsigned long b_state)
{
	int blkbits = inode->i_blkbits;
<<<<<<< HEAD
	pgoff_t index = blkoff >> (PAGE_CACHE_SHIFT - blkbits);
	struct page *page;
	struct buffer_head *bh;

	page = grab_cache_page(mapping, index);
	if (unlikely(!page))
		return NULL;

	bh = __nilfs_get_page_block(page, blkoff, index, blkbits, b_state);
	if (unlikely(!bh)) {
		unlock_page(page);
		page_cache_release(page);
=======
	pgoff_t index = blkoff >> (PAGE_SHIFT - blkbits);
	struct folio *folio;
	struct buffer_head *bh;

	folio = filemap_grab_folio(mapping, index);
	if (IS_ERR(folio))
		return NULL;

	bh = __nilfs_get_folio_block(folio, blkoff, index, blkbits, b_state);
	if (unlikely(!bh)) {
		folio_unlock(folio);
		folio_put(folio);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return NULL;
	}
	return bh;
}

/**
 * nilfs_forget_buffer - discard dirty state
<<<<<<< HEAD
 * @inode: owner inode of the buffer
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @bh: buffer head of the buffer to be discarded
 */
void nilfs_forget_buffer(struct buffer_head *bh)
{
<<<<<<< HEAD
	struct page *page = bh->b_page;

	lock_buffer(bh);
	clear_buffer_nilfs_volatile(bh);
	clear_buffer_nilfs_checked(bh);
	clear_buffer_nilfs_redirected(bh);
	clear_buffer_async_write(bh);
	clear_buffer_dirty(bh);
	if (nilfs_page_buffers_clean(page))
		__nilfs_clear_page_dirty(page);

	clear_buffer_uptodate(bh);
	clear_buffer_mapped(bh);
	bh->b_blocknr = -1;
	ClearPageUptodate(page);
	ClearPageMappedToDisk(page);
=======
	struct folio *folio = bh->b_folio;
	const unsigned long clear_bits =
		(BIT(BH_Uptodate) | BIT(BH_Dirty) | BIT(BH_Mapped) |
		 BIT(BH_Async_Write) | BIT(BH_NILFS_Volatile) |
		 BIT(BH_NILFS_Checked) | BIT(BH_NILFS_Redirected));

	lock_buffer(bh);
	set_mask_bits(&bh->b_state, clear_bits, 0);
	if (nilfs_folio_buffers_clean(folio))
		__nilfs_clear_folio_dirty(folio);

	bh->b_blocknr = -1;
	folio_clear_uptodate(folio);
	folio_clear_mappedtodisk(folio);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unlock_buffer(bh);
	brelse(bh);
}

/**
 * nilfs_copy_buffer -- copy buffer data and flags
 * @dbh: destination buffer
 * @sbh: source buffer
 */
void nilfs_copy_buffer(struct buffer_head *dbh, struct buffer_head *sbh)
{
	void *kaddr0, *kaddr1;
	unsigned long bits;
	struct page *spage = sbh->b_page, *dpage = dbh->b_page;
	struct buffer_head *bh;

<<<<<<< HEAD
	kaddr0 = kmap_atomic(spage);
	kaddr1 = kmap_atomic(dpage);
	memcpy(kaddr1 + bh_offset(dbh), kaddr0 + bh_offset(sbh), sbh->b_size);
	kunmap_atomic(kaddr1);
	kunmap_atomic(kaddr0);
=======
	kaddr0 = kmap_local_page(spage);
	kaddr1 = kmap_local_page(dpage);
	memcpy(kaddr1 + bh_offset(dbh), kaddr0 + bh_offset(sbh), sbh->b_size);
	kunmap_local(kaddr1);
	kunmap_local(kaddr0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	dbh->b_state = sbh->b_state & NILFS_BUFFER_INHERENT_BITS;
	dbh->b_blocknr = sbh->b_blocknr;
	dbh->b_bdev = sbh->b_bdev;

	bh = dbh;
<<<<<<< HEAD
	bits = sbh->b_state & ((1UL << BH_Uptodate) | (1UL << BH_Mapped));
=======
	bits = sbh->b_state & (BIT(BH_Uptodate) | BIT(BH_Mapped));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	while ((bh = bh->b_this_page) != dbh) {
		lock_buffer(bh);
		bits &= bh->b_state;
		unlock_buffer(bh);
	}
<<<<<<< HEAD
	if (bits & (1UL << BH_Uptodate))
		SetPageUptodate(dpage);
	else
		ClearPageUptodate(dpage);
	if (bits & (1UL << BH_Mapped))
=======
	if (bits & BIT(BH_Uptodate))
		SetPageUptodate(dpage);
	else
		ClearPageUptodate(dpage);
	if (bits & BIT(BH_Mapped))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		SetPageMappedToDisk(dpage);
	else
		ClearPageMappedToDisk(dpage);
}

/**
<<<<<<< HEAD
 * nilfs_page_buffers_clean - check if a page has dirty buffers or not.
 * @page: page to be checked
 *
 * nilfs_page_buffers_clean() returns zero if the page has dirty buffers.
 * Otherwise, it returns non-zero value.
 */
int nilfs_page_buffers_clean(struct page *page)
{
	struct buffer_head *bh, *head;

	bh = head = page_buffers(page);
	do {
		if (buffer_dirty(bh))
			return 0;
		bh = bh->b_this_page;
	} while (bh != head);
	return 1;
}

void nilfs_page_bug(struct page *page)
{
	struct address_space *m;
	unsigned long ino;

	if (unlikely(!page)) {
		printk(KERN_CRIT "NILFS_PAGE_BUG(NULL)\n");
		return;
	}

	m = page->mapping;
	ino = m ? m->host->i_ino : 0;

	printk(KERN_CRIT "NILFS_PAGE_BUG(%p): cnt=%d index#=%llu flags=0x%lx "
	       "mapping=%p ino=%lu\n",
	       page, atomic_read(&page->_count),
	       (unsigned long long)page->index, page->flags, m, ino);

	if (page_has_buffers(page)) {
		struct buffer_head *bh, *head;
		int i = 0;

		bh = head = page_buffers(page);
=======
 * nilfs_folio_buffers_clean - Check if a folio has dirty buffers or not.
 * @folio: Folio to be checked.
 *
 * nilfs_folio_buffers_clean() returns false if the folio has dirty buffers.
 * Otherwise, it returns true.
 */
bool nilfs_folio_buffers_clean(struct folio *folio)
{
	struct buffer_head *bh, *head;

	bh = head = folio_buffers(folio);
	do {
		if (buffer_dirty(bh))
			return false;
		bh = bh->b_this_page;
	} while (bh != head);
	return true;
}

void nilfs_folio_bug(struct folio *folio)
{
	struct buffer_head *bh, *head;
	struct address_space *m;
	unsigned long ino;

	if (unlikely(!folio)) {
		printk(KERN_CRIT "NILFS_FOLIO_BUG(NULL)\n");
		return;
	}

	m = folio->mapping;
	ino = m ? m->host->i_ino : 0;

	printk(KERN_CRIT "NILFS_FOLIO_BUG(%p): cnt=%d index#=%llu flags=0x%lx "
	       "mapping=%p ino=%lu\n",
	       folio, folio_ref_count(folio),
	       (unsigned long long)folio->index, folio->flags, m, ino);

	head = folio_buffers(folio);
	if (head) {
		int i = 0;

		bh = head;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		do {
			printk(KERN_CRIT
			       " BH[%d] %p: cnt=%d block#=%llu state=0x%lx\n",
			       i++, bh, atomic_read(&bh->b_count),
			       (unsigned long long)bh->b_blocknr, bh->b_state);
			bh = bh->b_this_page;
		} while (bh != head);
	}
}

/**
<<<<<<< HEAD
 * nilfs_copy_page -- copy the page with buffers
 * @dst: destination page
 * @src: source page
 * @copy_dirty: flag whether to copy dirty states on the page's buffer heads.
 *
 * This function is for both data pages and btnode pages.  The dirty flag
 * should be treated by caller.  The page must not be under i/o.
 * Both src and dst page must be locked
 */
static void nilfs_copy_page(struct page *dst, struct page *src, int copy_dirty)
{
	struct buffer_head *dbh, *dbufs, *sbh, *sbufs;
	unsigned long mask = NILFS_BUFFER_INHERENT_BITS;

	BUG_ON(PageWriteback(dst));

	sbh = sbufs = page_buffers(src);
	if (!page_has_buffers(dst))
		create_empty_buffers(dst, sbh->b_size, 0);

	if (copy_dirty)
		mask |= (1UL << BH_Dirty);

	dbh = dbufs = page_buffers(dst);
=======
 * nilfs_copy_folio -- copy the folio with buffers
 * @dst: destination folio
 * @src: source folio
 * @copy_dirty: flag whether to copy dirty states on the folio's buffer heads.
 *
 * This function is for both data folios and btnode folios.  The dirty flag
 * should be treated by caller.  The folio must not be under i/o.
 * Both src and dst folio must be locked
 */
static void nilfs_copy_folio(struct folio *dst, struct folio *src,
		bool copy_dirty)
{
	struct buffer_head *dbh, *dbufs, *sbh;
	unsigned long mask = NILFS_BUFFER_INHERENT_BITS;

	BUG_ON(folio_test_writeback(dst));

	sbh = folio_buffers(src);
	dbh = folio_buffers(dst);
	if (!dbh)
		dbh = create_empty_buffers(dst, sbh->b_size, 0);

	if (copy_dirty)
		mask |= BIT(BH_Dirty);

	dbufs = dbh;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	do {
		lock_buffer(sbh);
		lock_buffer(dbh);
		dbh->b_state = sbh->b_state & mask;
		dbh->b_blocknr = sbh->b_blocknr;
		dbh->b_bdev = sbh->b_bdev;
		sbh = sbh->b_this_page;
		dbh = dbh->b_this_page;
	} while (dbh != dbufs);

<<<<<<< HEAD
	copy_highpage(dst, src);

	if (PageUptodate(src) && !PageUptodate(dst))
		SetPageUptodate(dst);
	else if (!PageUptodate(src) && PageUptodate(dst))
		ClearPageUptodate(dst);
	if (PageMappedToDisk(src) && !PageMappedToDisk(dst))
		SetPageMappedToDisk(dst);
	else if (!PageMappedToDisk(src) && PageMappedToDisk(dst))
		ClearPageMappedToDisk(dst);
=======
	folio_copy(dst, src);

	if (folio_test_uptodate(src) && !folio_test_uptodate(dst))
		folio_mark_uptodate(dst);
	else if (!folio_test_uptodate(src) && folio_test_uptodate(dst))
		folio_clear_uptodate(dst);
	if (folio_test_mappedtodisk(src) && !folio_test_mappedtodisk(dst))
		folio_set_mappedtodisk(dst);
	else if (!folio_test_mappedtodisk(src) && folio_test_mappedtodisk(dst))
		folio_clear_mappedtodisk(dst);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	do {
		unlock_buffer(sbh);
		unlock_buffer(dbh);
		sbh = sbh->b_this_page;
		dbh = dbh->b_this_page;
	} while (dbh != dbufs);
}

int nilfs_copy_dirty_pages(struct address_space *dmap,
			   struct address_space *smap)
{
<<<<<<< HEAD
	struct pagevec pvec;
=======
	struct folio_batch fbatch;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned int i;
	pgoff_t index = 0;
	int err = 0;

<<<<<<< HEAD
	pagevec_init(&pvec, 0);
repeat:
	if (!pagevec_lookup_tag(&pvec, smap, &index, PAGECACHE_TAG_DIRTY,
				PAGEVEC_SIZE))
		return 0;

	for (i = 0; i < pagevec_count(&pvec); i++) {
		struct page *page = pvec.pages[i], *dpage;

		lock_page(page);
		if (unlikely(!PageDirty(page)))
			NILFS_PAGE_BUG(page, "inconsistent dirty state");

		dpage = grab_cache_page(dmap, page->index);
		if (unlikely(!dpage)) {
			/* No empty page is added to the page cache */
			err = -ENOMEM;
			unlock_page(page);
			break;
		}
		if (unlikely(!page_has_buffers(page)))
			NILFS_PAGE_BUG(page,
				       "found empty page in dat page cache");

		nilfs_copy_page(dpage, page, 1);
		__set_page_dirty_nobuffers(dpage);

		unlock_page(dpage);
		page_cache_release(dpage);
		unlock_page(page);
	}
	pagevec_release(&pvec);
=======
	folio_batch_init(&fbatch);
repeat:
	if (!filemap_get_folios_tag(smap, &index, (pgoff_t)-1,
				PAGECACHE_TAG_DIRTY, &fbatch))
		return 0;

	for (i = 0; i < folio_batch_count(&fbatch); i++) {
		struct folio *folio = fbatch.folios[i], *dfolio;

		folio_lock(folio);
		if (unlikely(!folio_test_dirty(folio)))
			NILFS_FOLIO_BUG(folio, "inconsistent dirty state");

		dfolio = filemap_grab_folio(dmap, folio->index);
		if (unlikely(IS_ERR(dfolio))) {
			/* No empty page is added to the page cache */
			folio_unlock(folio);
			err = PTR_ERR(dfolio);
			break;
		}
		if (unlikely(!folio_buffers(folio)))
			NILFS_FOLIO_BUG(folio,
				       "found empty page in dat page cache");

		nilfs_copy_folio(dfolio, folio, true);
		filemap_dirty_folio(folio_mapping(dfolio), dfolio);

		folio_unlock(dfolio);
		folio_put(dfolio);
		folio_unlock(folio);
	}
	folio_batch_release(&fbatch);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	cond_resched();

	if (likely(!err))
		goto repeat;
	return err;
}

/**
 * nilfs_copy_back_pages -- copy back pages to original cache from shadow cache
 * @dmap: destination page cache
 * @smap: source page cache
 *
<<<<<<< HEAD
 * No pages must no be added to the cache during this process.
=======
 * No pages must be added to the cache during this process.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * This must be ensured by the caller.
 */
void nilfs_copy_back_pages(struct address_space *dmap,
			   struct address_space *smap)
{
<<<<<<< HEAD
	struct pagevec pvec;
	unsigned int i, n;
	pgoff_t index = 0;
	int err;

	pagevec_init(&pvec, 0);
repeat:
	n = pagevec_lookup(&pvec, smap, index, PAGEVEC_SIZE);
	if (!n)
		return;
	index = pvec.pages[n - 1]->index + 1;

	for (i = 0; i < pagevec_count(&pvec); i++) {
		struct page *page = pvec.pages[i], *dpage;
		pgoff_t offset = page->index;

		lock_page(page);
		dpage = find_lock_page(dmap, offset);
		if (dpage) {
			/* override existing page on the destination cache */
			WARN_ON(PageDirty(dpage));
			nilfs_copy_page(dpage, page, 0);
			unlock_page(dpage);
			page_cache_release(dpage);
		} else {
			struct page *page2;

			/* move the page to the destination cache */
			spin_lock_irq(&smap->tree_lock);
			page2 = radix_tree_delete(&smap->page_tree, offset);
			WARN_ON(page2 != page);

			smap->nrpages--;
			spin_unlock_irq(&smap->tree_lock);

			spin_lock_irq(&dmap->tree_lock);
			err = radix_tree_insert(&dmap->page_tree, offset, page);
			if (unlikely(err < 0)) {
				WARN_ON(err == -EEXIST);
				page->mapping = NULL;
				page_cache_release(page); /* for cache */
			} else {
				page->mapping = dmap;
				dmap->nrpages++;
				if (PageDirty(page))
					radix_tree_tag_set(&dmap->page_tree,
							   offset,
							   PAGECACHE_TAG_DIRTY);
			}
			spin_unlock_irq(&dmap->tree_lock);
		}
		unlock_page(page);
	}
	pagevec_release(&pvec);
=======
	struct folio_batch fbatch;
	unsigned int i, n;
	pgoff_t start = 0;

	folio_batch_init(&fbatch);
repeat:
	n = filemap_get_folios(smap, &start, ~0UL, &fbatch);
	if (!n)
		return;

	for (i = 0; i < folio_batch_count(&fbatch); i++) {
		struct folio *folio = fbatch.folios[i], *dfolio;
		pgoff_t index = folio->index;

		folio_lock(folio);
		dfolio = filemap_lock_folio(dmap, index);
		if (!IS_ERR(dfolio)) {
			/* overwrite existing folio in the destination cache */
			WARN_ON(folio_test_dirty(dfolio));
			nilfs_copy_folio(dfolio, folio, false);
			folio_unlock(dfolio);
			folio_put(dfolio);
			/* Do we not need to remove folio from smap here? */
		} else {
			struct folio *f;

			/* move the folio to the destination cache */
			xa_lock_irq(&smap->i_pages);
			f = __xa_erase(&smap->i_pages, index);
			WARN_ON(folio != f);
			smap->nrpages--;
			xa_unlock_irq(&smap->i_pages);

			xa_lock_irq(&dmap->i_pages);
			f = __xa_store(&dmap->i_pages, index, folio, GFP_NOFS);
			if (unlikely(f)) {
				/* Probably -ENOMEM */
				folio->mapping = NULL;
				folio_put(folio);
			} else {
				folio->mapping = dmap;
				dmap->nrpages++;
				if (folio_test_dirty(folio))
					__xa_set_mark(&dmap->i_pages, index,
							PAGECACHE_TAG_DIRTY);
			}
			xa_unlock_irq(&dmap->i_pages);
		}
		folio_unlock(folio);
	}
	folio_batch_release(&fbatch);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	cond_resched();

	goto repeat;
}

<<<<<<< HEAD
void nilfs_clear_dirty_pages(struct address_space *mapping)
{
	struct pagevec pvec;
	unsigned int i;
	pgoff_t index = 0;

	pagevec_init(&pvec, 0);

	while (pagevec_lookup_tag(&pvec, mapping, &index, PAGECACHE_TAG_DIRTY,
				  PAGEVEC_SIZE)) {
		for (i = 0; i < pagevec_count(&pvec); i++) {
			struct page *page = pvec.pages[i];
			struct buffer_head *bh, *head;

			lock_page(page);
			ClearPageUptodate(page);
			ClearPageMappedToDisk(page);
			bh = head = page_buffers(page);
			do {
				lock_buffer(bh);
				clear_buffer_async_write(bh);
				clear_buffer_dirty(bh);
				clear_buffer_nilfs_volatile(bh);
				clear_buffer_nilfs_checked(bh);
				clear_buffer_nilfs_redirected(bh);
				clear_buffer_uptodate(bh);
				clear_buffer_mapped(bh);
				unlock_buffer(bh);
				bh = bh->b_this_page;
			} while (bh != head);

			__nilfs_clear_page_dirty(page);
			unlock_page(page);
		}
		pagevec_release(&pvec);
=======
/**
 * nilfs_clear_dirty_pages - discard dirty pages in address space
 * @mapping: address space with dirty pages for discarding
 * @silent: suppress [true] or print [false] warning messages
 */
void nilfs_clear_dirty_pages(struct address_space *mapping, bool silent)
{
	struct folio_batch fbatch;
	unsigned int i;
	pgoff_t index = 0;

	folio_batch_init(&fbatch);

	while (filemap_get_folios_tag(mapping, &index, (pgoff_t)-1,
				PAGECACHE_TAG_DIRTY, &fbatch)) {
		for (i = 0; i < folio_batch_count(&fbatch); i++) {
			struct folio *folio = fbatch.folios[i];

			folio_lock(folio);

			/*
			 * This folio may have been removed from the address
			 * space by truncation or invalidation when the lock
			 * was acquired.  Skip processing in that case.
			 */
			if (likely(folio->mapping == mapping))
				nilfs_clear_folio_dirty(folio, silent);

			folio_unlock(folio);
		}
		folio_batch_release(&fbatch);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		cond_resched();
	}
}

<<<<<<< HEAD
unsigned nilfs_page_count_clean_buffers(struct page *page,
					unsigned from, unsigned to)
{
	unsigned block_start, block_end;
	struct buffer_head *bh, *head;
	unsigned nc = 0;
=======
/**
 * nilfs_clear_folio_dirty - discard dirty folio
 * @folio: dirty folio that will be discarded
 * @silent: suppress [true] or print [false] warning messages
 */
void nilfs_clear_folio_dirty(struct folio *folio, bool silent)
{
	struct inode *inode = folio->mapping->host;
	struct super_block *sb = inode->i_sb;
	struct buffer_head *bh, *head;

	BUG_ON(!folio_test_locked(folio));

	if (!silent)
		nilfs_warn(sb, "discard dirty page: offset=%lld, ino=%lu",
			   folio_pos(folio), inode->i_ino);

	folio_clear_uptodate(folio);
	folio_clear_mappedtodisk(folio);

	head = folio_buffers(folio);
	if (head) {
		const unsigned long clear_bits =
			(BIT(BH_Uptodate) | BIT(BH_Dirty) | BIT(BH_Mapped) |
			 BIT(BH_Async_Write) | BIT(BH_NILFS_Volatile) |
			 BIT(BH_NILFS_Checked) | BIT(BH_NILFS_Redirected));

		bh = head;
		do {
			lock_buffer(bh);
			if (!silent)
				nilfs_warn(sb,
					   "discard dirty block: blocknr=%llu, size=%zu",
					   (u64)bh->b_blocknr, bh->b_size);

			set_mask_bits(&bh->b_state, clear_bits, 0);
			unlock_buffer(bh);
		} while (bh = bh->b_this_page, bh != head);
	}

	__nilfs_clear_folio_dirty(folio);
}

unsigned int nilfs_page_count_clean_buffers(struct page *page,
					    unsigned int from, unsigned int to)
{
	unsigned int block_start, block_end;
	struct buffer_head *bh, *head;
	unsigned int nc = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	for (bh = head = page_buffers(page), block_start = 0;
	     bh != head || !block_start;
	     block_start = block_end, bh = bh->b_this_page) {
		block_end = block_start + bh->b_size;
		if (block_end > from && block_start < to && !buffer_dirty(bh))
			nc++;
	}
	return nc;
}

<<<<<<< HEAD
void nilfs_mapping_init(struct address_space *mapping, struct inode *inode,
			struct backing_dev_info *bdi)
{
	mapping->host = inode;
	mapping->flags = 0;
	mapping_set_gfp_mask(mapping, GFP_NOFS);
	mapping->assoc_mapping = NULL;
	mapping->backing_dev_info = bdi;
	mapping->a_ops = &empty_aops;
}

/*
 * NILFS2 needs clear_page_dirty() in the following two cases:
 *
 * 1) For B-tree node pages and data pages of the dat/gcdat, NILFS2 clears
 *    page dirty flags when it copies back pages from the shadow cache
 *    (gcdat->{i_mapping,i_btnode_cache}) to its original cache
 *    (dat->{i_mapping,i_btnode_cache}).
=======
/*
 * NILFS2 needs clear_page_dirty() in the following two cases:
 *
 * 1) For B-tree node pages and data pages of DAT file, NILFS2 clears dirty
 *    flag of pages when it copies back pages from shadow cache to the
 *    original cache.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * 2) Some B-tree operations like insertion or deletion may dispose buffers
 *    in dirty state, and this needs to cancel the dirty state of their pages.
 */
<<<<<<< HEAD
int __nilfs_clear_page_dirty(struct page *page)
{
	struct address_space *mapping = page->mapping;

	if (mapping) {
		spin_lock_irq(&mapping->tree_lock);
		if (test_bit(PG_dirty, &page->flags)) {
			radix_tree_tag_clear(&mapping->page_tree,
					     page_index(page),
					     PAGECACHE_TAG_DIRTY);
			spin_unlock_irq(&mapping->tree_lock);
			return clear_page_dirty_for_io(page);
		}
		spin_unlock_irq(&mapping->tree_lock);
		return 0;
	}
	return TestClearPageDirty(page);
=======
void __nilfs_clear_folio_dirty(struct folio *folio)
{
	struct address_space *mapping = folio->mapping;

	if (mapping) {
		xa_lock_irq(&mapping->i_pages);
		if (folio_test_dirty(folio)) {
			__xa_clear_mark(&mapping->i_pages, folio->index,
					     PAGECACHE_TAG_DIRTY);
			xa_unlock_irq(&mapping->i_pages);
			folio_clear_dirty_for_io(folio);
			return;
		}
		xa_unlock_irq(&mapping->i_pages);
		return;
	}
	folio_clear_dirty(folio);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * nilfs_find_uncommitted_extent - find extent of uncommitted data
 * @inode: inode
 * @start_blk: start block offset (in)
 * @blkoff: start offset of the found extent (out)
 *
 * This function searches an extent of buffers marked "delayed" which
 * starts from a block offset equal to or larger than @start_blk.  If
 * such an extent was found, this will store the start offset in
 * @blkoff and return its length in blocks.  Otherwise, zero is
 * returned.
 */
unsigned long nilfs_find_uncommitted_extent(struct inode *inode,
					    sector_t start_blk,
					    sector_t *blkoff)
{
<<<<<<< HEAD
	unsigned int i;
	pgoff_t index;
	unsigned int nblocks_in_page;
	unsigned long length = 0;
	sector_t b;
	struct pagevec pvec;
	struct page *page;
=======
	unsigned int i, nr_folios;
	pgoff_t index;
	unsigned long length = 0;
	struct folio_batch fbatch;
	struct folio *folio;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (inode->i_mapping->nrpages == 0)
		return 0;

<<<<<<< HEAD
	index = start_blk >> (PAGE_CACHE_SHIFT - inode->i_blkbits);
	nblocks_in_page = 1U << (PAGE_CACHE_SHIFT - inode->i_blkbits);

	pagevec_init(&pvec, 0);

repeat:
	pvec.nr = find_get_pages_contig(inode->i_mapping, index, PAGEVEC_SIZE,
					pvec.pages);
	if (pvec.nr == 0)
		return length;

	if (length > 0 && pvec.pages[0]->index > index)
		goto out;

	b = pvec.pages[0]->index << (PAGE_CACHE_SHIFT - inode->i_blkbits);
	i = 0;
	do {
		page = pvec.pages[i];

		lock_page(page);
		if (page_has_buffers(page)) {
			struct buffer_head *bh, *head;

			bh = head = page_buffers(page);
=======
	index = start_blk >> (PAGE_SHIFT - inode->i_blkbits);

	folio_batch_init(&fbatch);

repeat:
	nr_folios = filemap_get_folios_contig(inode->i_mapping, &index, ULONG_MAX,
			&fbatch);
	if (nr_folios == 0)
		return length;

	i = 0;
	do {
		folio = fbatch.folios[i];

		folio_lock(folio);
		if (folio_buffers(folio)) {
			struct buffer_head *bh, *head;
			sector_t b;

			b = folio->index << (PAGE_SHIFT - inode->i_blkbits);
			bh = head = folio_buffers(folio);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			do {
				if (b < start_blk)
					continue;
				if (buffer_delay(bh)) {
					if (length == 0)
						*blkoff = b;
					length++;
				} else if (length > 0) {
					goto out_locked;
				}
			} while (++b, bh = bh->b_this_page, bh != head);
		} else {
			if (length > 0)
				goto out_locked;
<<<<<<< HEAD

			b += nblocks_in_page;
		}
		unlock_page(page);

	} while (++i < pagevec_count(&pvec));

	index = page->index + 1;
	pagevec_release(&pvec);
=======
		}
		folio_unlock(folio);

	} while (++i < nr_folios);

	folio_batch_release(&fbatch);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	cond_resched();
	goto repeat;

out_locked:
<<<<<<< HEAD
	unlock_page(page);
out:
	pagevec_release(&pvec);
=======
	folio_unlock(folio);
	folio_batch_release(&fbatch);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return length;
}
