<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * fs/f2fs/checkpoint.c
 *
 * Copyright (c) 2012 Samsung Electronics Co., Ltd.
 *             http://www.samsung.com/
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#include <linux/fs.h>
#include <linux/bio.h>
#include <linux/mpage.h>
#include <linux/writeback.h>
#include <linux/blkdev.h>
#include <linux/f2fs_fs.h>
#include <linux/pagevec.h>
#include <linux/swap.h>
<<<<<<< HEAD
=======
#include <linux/kthread.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include "f2fs.h"
#include "node.h"
#include "segment.h"
<<<<<<< HEAD
#include "trace.h"
#include <trace/events/f2fs.h>

static struct kmem_cache *ino_entry_slab;
struct kmem_cache *inode_entry_slab;

/*
 * We guarantee no failure on the returned page.
 */
struct page *grab_meta_page(struct f2fs_sb_info *sbi, pgoff_t index)
{
	struct address_space *mapping = META_MAPPING(sbi);
	struct page *page = NULL;
repeat:
	page = grab_cache_page(mapping, index);
	if (!page) {
		cond_resched();
		goto repeat;
	}
	f2fs_wait_on_page_writeback(page, META, true);
	SetPageUptodate(page);
	return page;
=======
#include "iostat.h"
#include <trace/events/f2fs.h>

#define DEFAULT_CHECKPOINT_IOPRIO (IOPRIO_PRIO_VALUE(IOPRIO_CLASS_BE, 3))

static struct kmem_cache *ino_entry_slab;
struct kmem_cache *f2fs_inode_entry_slab;

void f2fs_stop_checkpoint(struct f2fs_sb_info *sbi, bool end_io,
						unsigned char reason)
{
	f2fs_build_fault_attr(sbi, 0, 0);
	if (!end_io)
		f2fs_flush_merged_writes(sbi);
	f2fs_handle_critical_error(sbi, reason, end_io);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * We guarantee no failure on the returned page.
 */
<<<<<<< HEAD
=======
struct page *f2fs_grab_meta_page(struct f2fs_sb_info *sbi, pgoff_t index)
{
	struct address_space *mapping = META_MAPPING(sbi);
	struct page *page;
repeat:
	page = f2fs_grab_cache_page(mapping, index, false);
	if (!page) {
		cond_resched();
		goto repeat;
	}
	f2fs_wait_on_page_writeback(page, META, true, true);
	if (!PageUptodate(page))
		SetPageUptodate(page);
	return page;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct page *__get_meta_page(struct f2fs_sb_info *sbi, pgoff_t index,
							bool is_meta)
{
	struct address_space *mapping = META_MAPPING(sbi);
	struct page *page;
	struct f2fs_io_info fio = {
		.sbi = sbi,
		.type = META,
<<<<<<< HEAD
		.rw = READ_SYNC | REQ_META | REQ_PRIO,
		.old_blkaddr = index,
		.new_blkaddr = index,
		.encrypted_page = NULL,
	};

	if (unlikely(!is_meta))
		fio.rw &= ~REQ_META;
repeat:
	page = grab_cache_page(mapping, index);
=======
		.op = REQ_OP_READ,
		.op_flags = REQ_META | REQ_PRIO,
		.old_blkaddr = index,
		.new_blkaddr = index,
		.encrypted_page = NULL,
		.is_por = !is_meta ? 1 : 0,
	};
	int err;

	if (unlikely(!is_meta))
		fio.op_flags &= ~REQ_META;
repeat:
	page = f2fs_grab_cache_page(mapping, index, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!page) {
		cond_resched();
		goto repeat;
	}
	if (PageUptodate(page))
		goto out;

	fio.page = page;

<<<<<<< HEAD
	if (f2fs_submit_page_bio(&fio)) {
		f2fs_put_page(page, 1);
		goto repeat;
	}

=======
	err = f2fs_submit_page_bio(&fio);
	if (err) {
		f2fs_put_page(page, 1);
		return ERR_PTR(err);
	}

	f2fs_update_iostat(sbi, NULL, FS_META_READ_IO, F2FS_BLKSIZE);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	lock_page(page);
	if (unlikely(page->mapping != mapping)) {
		f2fs_put_page(page, 1);
		goto repeat;
	}

<<<<<<< HEAD
	/*
	 * if there is any IO error when accessing device, make our filesystem
	 * readonly and make sure do not write checkpoint with non-uptodate
	 * meta page.
	 */
	if (unlikely(!PageUptodate(page)))
		f2fs_stop_checkpoint(sbi);
out:
	mark_page_accessed(page);
	return page;
}

struct page *get_meta_page(struct f2fs_sb_info *sbi, pgoff_t index)
=======
	if (unlikely(!PageUptodate(page))) {
		f2fs_handle_page_eio(sbi, page->index, META);
		f2fs_put_page(page, 1);
		return ERR_PTR(-EIO);
	}
out:
	return page;
}

struct page *f2fs_get_meta_page(struct f2fs_sb_info *sbi, pgoff_t index)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return __get_meta_page(sbi, index, true);
}

<<<<<<< HEAD
/* for POR only */
struct page *get_tmp_page(struct f2fs_sb_info *sbi, pgoff_t index)
=======
struct page *f2fs_get_meta_page_retry(struct f2fs_sb_info *sbi, pgoff_t index)
{
	struct page *page;
	int count = 0;

retry:
	page = __get_meta_page(sbi, index, true);
	if (IS_ERR(page)) {
		if (PTR_ERR(page) == -EIO &&
				++count <= DEFAULT_RETRY_IO_COUNT)
			goto retry;
		f2fs_stop_checkpoint(sbi, false, STOP_CP_REASON_META_PAGE);
	}
	return page;
}

/* for POR only */
struct page *f2fs_get_tmp_page(struct f2fs_sb_info *sbi, pgoff_t index)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return __get_meta_page(sbi, index, false);
}

<<<<<<< HEAD
bool is_valid_blkaddr(struct f2fs_sb_info *sbi, block_t blkaddr, int type)
=======
static bool __is_bitmap_valid(struct f2fs_sb_info *sbi, block_t blkaddr,
							int type)
{
	struct seg_entry *se;
	unsigned int segno, offset;
	bool exist;

	if (type == DATA_GENERIC)
		return true;

	segno = GET_SEGNO(sbi, blkaddr);
	offset = GET_BLKOFF_FROM_SEG0(sbi, blkaddr);
	se = get_seg_entry(sbi, segno);

	exist = f2fs_test_bit(offset, se->cur_valid_map);

	/* skip data, if we already have an error in checkpoint. */
	if (unlikely(f2fs_cp_error(sbi)))
		return exist;

	if ((exist && type == DATA_GENERIC_ENHANCE_UPDATE) ||
		(!exist && type == DATA_GENERIC_ENHANCE))
		goto out_err;
	if (!exist && type != DATA_GENERIC_ENHANCE_UPDATE)
		goto out_handle;
	return exist;

out_err:
	f2fs_err(sbi, "Inconsistent error blkaddr:%u, sit bitmap:%d",
		 blkaddr, exist);
	set_sbi_flag(sbi, SBI_NEED_FSCK);
	dump_stack();
out_handle:
	f2fs_handle_error(sbi, ERROR_INVALID_BLKADDR);
	return exist;
}

static bool __f2fs_is_valid_blkaddr(struct f2fs_sb_info *sbi,
					block_t blkaddr, int type)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	switch (type) {
	case META_NAT:
		break;
	case META_SIT:
		if (unlikely(blkaddr >= SIT_BLK_CNT(sbi)))
<<<<<<< HEAD
			return false;
=======
			goto err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case META_SSA:
		if (unlikely(blkaddr >= MAIN_BLKADDR(sbi) ||
			blkaddr < SM_I(sbi)->ssa_blkaddr))
<<<<<<< HEAD
			return false;
=======
			goto err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case META_CP:
		if (unlikely(blkaddr >= SIT_I(sbi)->sit_base_addr ||
			blkaddr < __start_cp_addr(sbi)))
<<<<<<< HEAD
			return false;
=======
			goto err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case META_POR:
		if (unlikely(blkaddr >= MAX_BLKADDR(sbi) ||
			blkaddr < MAIN_BLKADDR(sbi)))
<<<<<<< HEAD
			return false;
=======
			goto err;
		break;
	case DATA_GENERIC:
	case DATA_GENERIC_ENHANCE:
	case DATA_GENERIC_ENHANCE_READ:
	case DATA_GENERIC_ENHANCE_UPDATE:
		if (unlikely(blkaddr >= MAX_BLKADDR(sbi) ||
				blkaddr < MAIN_BLKADDR(sbi))) {

			/* Skip to emit an error message. */
			if (unlikely(f2fs_cp_error(sbi)))
				return false;

			f2fs_warn(sbi, "access invalid blkaddr:%u",
				  blkaddr);
			set_sbi_flag(sbi, SBI_NEED_FSCK);
			dump_stack();
			goto err;
		} else {
			return __is_bitmap_valid(sbi, blkaddr, type);
		}
		break;
	case META_GENERIC:
		if (unlikely(blkaddr < SEG0_BLKADDR(sbi) ||
			blkaddr >= MAIN_BLKADDR(sbi)))
			goto err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	default:
		BUG();
	}

	return true;
<<<<<<< HEAD
}

/*
 * Readahead CP/NAT/SIT/SSA pages
 */
int ra_meta_pages(struct f2fs_sb_info *sbi, block_t start, int nrpages,
=======
err:
	f2fs_handle_error(sbi, ERROR_INVALID_BLKADDR);
	return false;
}

bool f2fs_is_valid_blkaddr(struct f2fs_sb_info *sbi,
					block_t blkaddr, int type)
{
	if (time_to_inject(sbi, FAULT_BLKADDR_VALIDITY))
		return false;
	return __f2fs_is_valid_blkaddr(sbi, blkaddr, type);
}

bool f2fs_is_valid_blkaddr_raw(struct f2fs_sb_info *sbi,
					block_t blkaddr, int type)
{
	return __f2fs_is_valid_blkaddr(sbi, blkaddr, type);
}

/*
 * Readahead CP/NAT/SIT/SSA/POR pages
 */
int f2fs_ra_meta_pages(struct f2fs_sb_info *sbi, block_t start, int nrpages,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
							int type, bool sync)
{
	struct page *page;
	block_t blkno = start;
	struct f2fs_io_info fio = {
		.sbi = sbi,
		.type = META,
<<<<<<< HEAD
		.rw = sync ? (READ_SYNC | REQ_META | REQ_PRIO) : READA,
		.encrypted_page = NULL,
	};
	struct blk_plug plug;

	if (unlikely(type == META_POR))
		fio.rw &= ~REQ_META;
=======
		.op = REQ_OP_READ,
		.op_flags = sync ? (REQ_META | REQ_PRIO) : REQ_RAHEAD,
		.encrypted_page = NULL,
		.in_list = 0,
		.is_por = (type == META_POR) ? 1 : 0,
	};
	struct blk_plug plug;
	int err;

	if (unlikely(type == META_POR))
		fio.op_flags &= ~REQ_META;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	blk_start_plug(&plug);
	for (; nrpages-- > 0; blkno++) {

<<<<<<< HEAD
		if (!is_valid_blkaddr(sbi, blkno, type))
=======
		if (!f2fs_is_valid_blkaddr(sbi, blkno, type))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto out;

		switch (type) {
		case META_NAT:
			if (unlikely(blkno >=
					NAT_BLOCK_OFFSET(NM_I(sbi)->max_nid)))
				blkno = 0;
			/* get nat block addr */
			fio.new_blkaddr = current_nat_addr(sbi,
					blkno * NAT_ENTRY_PER_BLOCK);
			break;
		case META_SIT:
<<<<<<< HEAD
=======
			if (unlikely(blkno >= TOTAL_SEGS(sbi)))
				goto out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/* get sit block addr */
			fio.new_blkaddr = current_sit_addr(sbi,
					blkno * SIT_ENTRY_PER_BLOCK);
			break;
		case META_SSA:
		case META_CP:
		case META_POR:
			fio.new_blkaddr = blkno;
			break;
		default:
			BUG();
		}

<<<<<<< HEAD
		page = grab_cache_page(META_MAPPING(sbi), fio.new_blkaddr);
=======
		page = f2fs_grab_cache_page(META_MAPPING(sbi),
						fio.new_blkaddr, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!page)
			continue;
		if (PageUptodate(page)) {
			f2fs_put_page(page, 1);
			continue;
		}

		fio.page = page;
<<<<<<< HEAD
		fio.old_blkaddr = fio.new_blkaddr;
		f2fs_submit_page_mbio(&fio);
		f2fs_put_page(page, 0);
	}
out:
	f2fs_submit_merged_bio(sbi, META, READ);
=======
		err = f2fs_submit_page_bio(&fio);
		f2fs_put_page(page, err ? 1 : 0);

		if (!err)
			f2fs_update_iostat(sbi, NULL, FS_META_READ_IO,
							F2FS_BLKSIZE);
	}
out:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	blk_finish_plug(&plug);
	return blkno - start;
}

<<<<<<< HEAD
void ra_meta_pages_cond(struct f2fs_sb_info *sbi, pgoff_t index)
=======
void f2fs_ra_meta_pages_cond(struct f2fs_sb_info *sbi, pgoff_t index,
							unsigned int ra_blocks)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct page *page;
	bool readahead = false;

<<<<<<< HEAD
	page = find_get_page(META_MAPPING(sbi), index);
	if (!page || (page && !PageUptodate(page)))
=======
	if (ra_blocks == RECOVERY_MIN_RA_BLOCKS)
		return;

	page = find_get_page(META_MAPPING(sbi), index);
	if (!page || !PageUptodate(page))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		readahead = true;
	f2fs_put_page(page, 0);

	if (readahead)
<<<<<<< HEAD
		ra_meta_pages(sbi, index, MAX_BIO_BLOCKS(sbi), META_POR, true);
}

static int f2fs_write_meta_page(struct page *page,
				struct writeback_control *wbc)
=======
		f2fs_ra_meta_pages(sbi, index, ra_blocks, META_POR, true);
}

static int __f2fs_write_meta_page(struct page *page,
				struct writeback_control *wbc,
				enum iostat_type io_type)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct f2fs_sb_info *sbi = F2FS_P_SB(page);

	trace_f2fs_writepage(page, META);

<<<<<<< HEAD
=======
	if (unlikely(f2fs_cp_error(sbi))) {
		if (is_sbi_flag_set(sbi, SBI_IS_CLOSE)) {
			ClearPageUptodate(page);
			dec_page_count(sbi, F2FS_DIRTY_META);
			unlock_page(page);
			return 0;
		}
		goto redirty_out;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (unlikely(is_sbi_flag_set(sbi, SBI_POR_DOING)))
		goto redirty_out;
	if (wbc->for_reclaim && page->index < GET_SUM_BLOCK(sbi, 0))
		goto redirty_out;
<<<<<<< HEAD
	if (unlikely(f2fs_cp_error(sbi)))
		goto redirty_out;

	write_meta_page(sbi, page);
	dec_page_count(sbi, F2FS_DIRTY_META);

	if (wbc->for_reclaim)
		f2fs_submit_merged_bio_cond(sbi, NULL, page, 0, META, WRITE);
=======

	f2fs_do_write_meta_page(sbi, page, io_type);
	dec_page_count(sbi, F2FS_DIRTY_META);

	if (wbc->for_reclaim)
		f2fs_submit_merged_write_cond(sbi, NULL, page, 0, META);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	unlock_page(page);

	if (unlikely(f2fs_cp_error(sbi)))
<<<<<<< HEAD
		f2fs_submit_merged_bio(sbi, META, WRITE);
=======
		f2fs_submit_merged_write(sbi, META);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;

redirty_out:
	redirty_page_for_writepage(wbc, page);
	return AOP_WRITEPAGE_ACTIVATE;
}

<<<<<<< HEAD
=======
static int f2fs_write_meta_page(struct page *page,
				struct writeback_control *wbc)
{
	return __f2fs_write_meta_page(page, wbc, FS_META_IO);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int f2fs_write_meta_pages(struct address_space *mapping,
				struct writeback_control *wbc)
{
	struct f2fs_sb_info *sbi = F2FS_M_SB(mapping);
	long diff, written;

<<<<<<< HEAD
	/* collect a number of dirty meta pages and write together */
	if (wbc->for_kupdate ||
		get_pages(sbi, F2FS_DIRTY_META) < nr_pages_to_skip(sbi, META))
		goto skip_write;

	trace_f2fs_writepages(mapping->host, wbc, META);

	/* if mounting is failed, skip writing node pages */
	mutex_lock(&sbi->cp_mutex);
	diff = nr_pages_to_write(sbi, META, wbc);
	written = sync_meta_pages(sbi, META, wbc->nr_to_write);
	mutex_unlock(&sbi->cp_mutex);
=======
	if (unlikely(is_sbi_flag_set(sbi, SBI_POR_DOING)))
		goto skip_write;

	/* collect a number of dirty meta pages and write together */
	if (wbc->sync_mode != WB_SYNC_ALL &&
			get_pages(sbi, F2FS_DIRTY_META) <
					nr_pages_to_skip(sbi, META))
		goto skip_write;

	/* if locked failed, cp will flush dirty pages instead */
	if (!f2fs_down_write_trylock(&sbi->cp_global_sem))
		goto skip_write;

	trace_f2fs_writepages(mapping->host, wbc, META);
	diff = nr_pages_to_write(sbi, META, wbc);
	written = f2fs_sync_meta_pages(sbi, META, wbc->nr_to_write, FS_META_IO);
	f2fs_up_write(&sbi->cp_global_sem);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	wbc->nr_to_write = max((long)0, wbc->nr_to_write - written - diff);
	return 0;

skip_write:
	wbc->pages_skipped += get_pages(sbi, F2FS_DIRTY_META);
	trace_f2fs_writepages(mapping->host, wbc, META);
	return 0;
}

<<<<<<< HEAD
long sync_meta_pages(struct f2fs_sb_info *sbi, enum page_type type,
						long nr_to_write)
{
	struct address_space *mapping = META_MAPPING(sbi);
	pgoff_t index = 0, end = ULONG_MAX, prev = ULONG_MAX;
	struct pagevec pvec;
	long nwritten = 0;
=======
long f2fs_sync_meta_pages(struct f2fs_sb_info *sbi, enum page_type type,
				long nr_to_write, enum iostat_type io_type)
{
	struct address_space *mapping = META_MAPPING(sbi);
	pgoff_t index = 0, prev = ULONG_MAX;
	struct folio_batch fbatch;
	long nwritten = 0;
	int nr_folios;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct writeback_control wbc = {
		.for_reclaim = 0,
	};
	struct blk_plug plug;

<<<<<<< HEAD
	pagevec_init(&pvec, 0);

	blk_start_plug(&plug);

	while (index <= end) {
		int i, nr_pages;
		nr_pages = pagevec_lookup_tag(&pvec, mapping, &index,
				PAGECACHE_TAG_DIRTY,
				min(end - index, (pgoff_t)PAGEVEC_SIZE-1) + 1);
		if (unlikely(nr_pages == 0))
			break;

		for (i = 0; i < nr_pages; i++) {
			struct page *page = pvec.pages[i];

			if (prev == ULONG_MAX)
				prev = page->index - 1;
			if (nr_to_write != LONG_MAX && page->index != prev + 1) {
				pagevec_release(&pvec);
				goto stop;
			}

			lock_page(page);

			if (unlikely(page->mapping != mapping)) {
continue_unlock:
				unlock_page(page);
				continue;
			}
			if (!PageDirty(page)) {
=======
	folio_batch_init(&fbatch);

	blk_start_plug(&plug);

	while ((nr_folios = filemap_get_folios_tag(mapping, &index,
					(pgoff_t)-1,
					PAGECACHE_TAG_DIRTY, &fbatch))) {
		int i;

		for (i = 0; i < nr_folios; i++) {
			struct folio *folio = fbatch.folios[i];

			if (nr_to_write != LONG_MAX && i != 0 &&
					folio->index != prev +
					folio_nr_pages(fbatch.folios[i-1])) {
				folio_batch_release(&fbatch);
				goto stop;
			}

			folio_lock(folio);

			if (unlikely(folio->mapping != mapping)) {
continue_unlock:
				folio_unlock(folio);
				continue;
			}
			if (!folio_test_dirty(folio)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				/* someone wrote it for us */
				goto continue_unlock;
			}

<<<<<<< HEAD
			f2fs_wait_on_page_writeback(page, META, true);

			BUG_ON(PageWriteback(page));
			if (!clear_page_dirty_for_io(page))
				goto continue_unlock;

			if (mapping->a_ops->writepage(page, &wbc)) {
				unlock_page(page);
				break;
			}
			nwritten++;
			prev = page->index;
			if (unlikely(nwritten >= nr_to_write))
				break;
		}
		pagevec_release(&pvec);
=======
			f2fs_wait_on_page_writeback(&folio->page, META,
					true, true);

			if (!folio_clear_dirty_for_io(folio))
				goto continue_unlock;

			if (__f2fs_write_meta_page(&folio->page, &wbc,
						io_type)) {
				folio_unlock(folio);
				break;
			}
			nwritten += folio_nr_pages(folio);
			prev = folio->index;
			if (unlikely(nwritten >= nr_to_write))
				break;
		}
		folio_batch_release(&fbatch);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		cond_resched();
	}
stop:
	if (nwritten)
<<<<<<< HEAD
		f2fs_submit_merged_bio(sbi, type, WRITE);
=======
		f2fs_submit_merged_write(sbi, type);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	blk_finish_plug(&plug);

	return nwritten;
}

<<<<<<< HEAD
static int f2fs_set_meta_page_dirty(struct page *page)
{
	trace_f2fs_set_page_dirty(page, META);

	SetPageUptodate(page);
	if (!PageDirty(page)) {
		__set_page_dirty_nobuffers(page);
		inc_page_count(F2FS_P_SB(page), F2FS_DIRTY_META);
		SetPagePrivate(page);
		f2fs_trace_pid(page);
		return 1;
	}
	return 0;
=======
static bool f2fs_dirty_meta_folio(struct address_space *mapping,
		struct folio *folio)
{
	trace_f2fs_set_page_dirty(&folio->page, META);

	if (!folio_test_uptodate(folio))
		folio_mark_uptodate(folio);
	if (filemap_dirty_folio(mapping, folio)) {
		inc_page_count(F2FS_M_SB(mapping), F2FS_DIRTY_META);
		set_page_private_reference(&folio->page);
		return true;
	}
	return false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

const struct address_space_operations f2fs_meta_aops = {
	.writepage	= f2fs_write_meta_page,
	.writepages	= f2fs_write_meta_pages,
<<<<<<< HEAD
	.set_page_dirty	= f2fs_set_meta_page_dirty,
	.invalidatepage = f2fs_invalidate_page,
	.releasepage	= f2fs_release_page,
};

static void __add_ino_entry(struct f2fs_sb_info *sbi, nid_t ino, int type)
{
	struct inode_management *im = &sbi->im[type];
	struct ino_entry *e, *tmp;

	tmp = f2fs_kmem_cache_alloc(ino_entry_slab, GFP_NOFS);
retry:
=======
	.dirty_folio	= f2fs_dirty_meta_folio,
	.invalidate_folio = f2fs_invalidate_folio,
	.release_folio	= f2fs_release_folio,
	.migrate_folio	= filemap_migrate_folio,
};

static void __add_ino_entry(struct f2fs_sb_info *sbi, nid_t ino,
						unsigned int devidx, int type)
{
	struct inode_management *im = &sbi->im[type];
	struct ino_entry *e = NULL, *new = NULL;

	if (type == FLUSH_INO) {
		rcu_read_lock();
		e = radix_tree_lookup(&im->ino_root, ino);
		rcu_read_unlock();
	}

retry:
	if (!e)
		new = f2fs_kmem_cache_alloc(ino_entry_slab,
						GFP_NOFS, true, NULL);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	radix_tree_preload(GFP_NOFS | __GFP_NOFAIL);

	spin_lock(&im->ino_lock);
	e = radix_tree_lookup(&im->ino_root, ino);
	if (!e) {
<<<<<<< HEAD
		e = tmp;
		if (radix_tree_insert(&im->ino_root, ino, e)) {
=======
		if (!new) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			spin_unlock(&im->ino_lock);
			radix_tree_preload_end();
			goto retry;
		}
<<<<<<< HEAD
=======
		e = new;
		if (unlikely(radix_tree_insert(&im->ino_root, ino, e)))
			f2fs_bug_on(sbi, 1);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		memset(e, 0, sizeof(struct ino_entry));
		e->ino = ino;

		list_add_tail(&e->list, &im->ino_list);
		if (type != ORPHAN_INO)
			im->ino_num++;
	}
<<<<<<< HEAD
	spin_unlock(&im->ino_lock);
	radix_tree_preload_end();

	if (e != tmp)
		kmem_cache_free(ino_entry_slab, tmp);
=======

	if (type == FLUSH_INO)
		f2fs_set_bit(devidx, (char *)&e->dirty_device);

	spin_unlock(&im->ino_lock);
	radix_tree_preload_end();

	if (new && e != new)
		kmem_cache_free(ino_entry_slab, new);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void __remove_ino_entry(struct f2fs_sb_info *sbi, nid_t ino, int type)
{
	struct inode_management *im = &sbi->im[type];
	struct ino_entry *e;

	spin_lock(&im->ino_lock);
	e = radix_tree_lookup(&im->ino_root, ino);
	if (e) {
		list_del(&e->list);
		radix_tree_delete(&im->ino_root, ino);
		im->ino_num--;
		spin_unlock(&im->ino_lock);
		kmem_cache_free(ino_entry_slab, e);
		return;
	}
	spin_unlock(&im->ino_lock);
}

<<<<<<< HEAD
void add_ino_entry(struct f2fs_sb_info *sbi, nid_t ino, int type)
{
	/* add new dirty ino entry into list */
	__add_ino_entry(sbi, ino, type);
}

void remove_ino_entry(struct f2fs_sb_info *sbi, nid_t ino, int type)
=======
void f2fs_add_ino_entry(struct f2fs_sb_info *sbi, nid_t ino, int type)
{
	/* add new dirty ino entry into list */
	__add_ino_entry(sbi, ino, 0, type);
}

void f2fs_remove_ino_entry(struct f2fs_sb_info *sbi, nid_t ino, int type)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	/* remove dirty ino entry from list */
	__remove_ino_entry(sbi, ino, type);
}

<<<<<<< HEAD
/* mode should be APPEND_INO or UPDATE_INO */
bool exist_written_data(struct f2fs_sb_info *sbi, nid_t ino, int mode)
=======
/* mode should be APPEND_INO, UPDATE_INO or TRANS_DIR_INO */
bool f2fs_exist_written_data(struct f2fs_sb_info *sbi, nid_t ino, int mode)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct inode_management *im = &sbi->im[mode];
	struct ino_entry *e;

	spin_lock(&im->ino_lock);
	e = radix_tree_lookup(&im->ino_root, ino);
	spin_unlock(&im->ino_lock);
	return e ? true : false;
}

<<<<<<< HEAD
void release_ino_entry(struct f2fs_sb_info *sbi)
=======
void f2fs_release_ino_entry(struct f2fs_sb_info *sbi, bool all)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ino_entry *e, *tmp;
	int i;

<<<<<<< HEAD
	for (i = APPEND_INO; i <= UPDATE_INO; i++) {
=======
	for (i = all ? ORPHAN_INO : APPEND_INO; i < MAX_INO_ENTRY; i++) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		struct inode_management *im = &sbi->im[i];

		spin_lock(&im->ino_lock);
		list_for_each_entry_safe(e, tmp, &im->ino_list, list) {
			list_del(&e->list);
			radix_tree_delete(&im->ino_root, e->ino);
			kmem_cache_free(ino_entry_slab, e);
			im->ino_num--;
		}
		spin_unlock(&im->ino_lock);
	}
}

<<<<<<< HEAD
int acquire_orphan_inode(struct f2fs_sb_info *sbi)
=======
void f2fs_set_dirty_device(struct f2fs_sb_info *sbi, nid_t ino,
					unsigned int devidx, int type)
{
	__add_ino_entry(sbi, ino, devidx, type);
}

bool f2fs_is_dirty_device(struct f2fs_sb_info *sbi, nid_t ino,
					unsigned int devidx, int type)
{
	struct inode_management *im = &sbi->im[type];
	struct ino_entry *e;
	bool is_dirty = false;

	spin_lock(&im->ino_lock);
	e = radix_tree_lookup(&im->ino_root, ino);
	if (e && f2fs_test_bit(devidx, (char *)&e->dirty_device))
		is_dirty = true;
	spin_unlock(&im->ino_lock);
	return is_dirty;
}

int f2fs_acquire_orphan_inode(struct f2fs_sb_info *sbi)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct inode_management *im = &sbi->im[ORPHAN_INO];
	int err = 0;

	spin_lock(&im->ino_lock);
<<<<<<< HEAD
=======

	if (time_to_inject(sbi, FAULT_ORPHAN)) {
		spin_unlock(&im->ino_lock);
		return -ENOSPC;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (unlikely(im->ino_num >= sbi->max_orphans))
		err = -ENOSPC;
	else
		im->ino_num++;
	spin_unlock(&im->ino_lock);

	return err;
}

<<<<<<< HEAD
void release_orphan_inode(struct f2fs_sb_info *sbi)
=======
void f2fs_release_orphan_inode(struct f2fs_sb_info *sbi)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct inode_management *im = &sbi->im[ORPHAN_INO];

	spin_lock(&im->ino_lock);
	f2fs_bug_on(sbi, im->ino_num == 0);
	im->ino_num--;
	spin_unlock(&im->ino_lock);
}

<<<<<<< HEAD
void add_orphan_inode(struct f2fs_sb_info *sbi, nid_t ino)
{
	/* add new orphan ino entry into list */
	__add_ino_entry(sbi, ino, ORPHAN_INO);
}

void remove_orphan_inode(struct f2fs_sb_info *sbi, nid_t ino)
=======
void f2fs_add_orphan_inode(struct inode *inode)
{
	/* add new orphan ino entry into list */
	__add_ino_entry(F2FS_I_SB(inode), inode->i_ino, 0, ORPHAN_INO);
	f2fs_update_inode_page(inode);
}

void f2fs_remove_orphan_inode(struct f2fs_sb_info *sbi, nid_t ino)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	/* remove orphan entry from orphan list */
	__remove_ino_entry(sbi, ino, ORPHAN_INO);
}

static int recover_orphan_inode(struct f2fs_sb_info *sbi, nid_t ino)
{
	struct inode *inode;
<<<<<<< HEAD

	inode = f2fs_iget(sbi->sb, ino);
=======
	struct node_info ni;
	int err;

	inode = f2fs_iget_retry(sbi->sb, ino);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (IS_ERR(inode)) {
		/*
		 * there should be a bug that we can't find the entry
		 * to orphan inode.
		 */
		f2fs_bug_on(sbi, PTR_ERR(inode) == -ENOENT);
		return PTR_ERR(inode);
	}

<<<<<<< HEAD
=======
	err = f2fs_dquot_initialize(inode);
	if (err) {
		iput(inode);
		goto err_out;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	clear_nlink(inode);

	/* truncate all the data during iput */
	iput(inode);
<<<<<<< HEAD
	return 0;
}

int recover_orphan_inodes(struct f2fs_sb_info *sbi)
{
	block_t start_blk, orphan_blocks, i, j;
	int err;

	if (!is_set_ckpt_flags(F2FS_CKPT(sbi), CP_ORPHAN_PRESENT_FLAG))
		return 0;

	start_blk = __start_cp_addr(sbi) + 1 + __cp_payload(sbi);
	orphan_blocks = __start_sum_addr(sbi) - 1 - __cp_payload(sbi);

	ra_meta_pages(sbi, start_blk, orphan_blocks, META_CP, true);

	for (i = 0; i < orphan_blocks; i++) {
		struct page *page = get_meta_page(sbi, start_blk + i);
		struct f2fs_orphan_block *orphan_blk;

		orphan_blk = (struct f2fs_orphan_block *)page_address(page);
		for (j = 0; j < le32_to_cpu(orphan_blk->entry_count); j++) {
			nid_t ino = le32_to_cpu(orphan_blk->ino[j]);
			err = recover_orphan_inode(sbi, ino);
			if (err) {
				f2fs_put_page(page, 1);
				return err;
=======

	err = f2fs_get_node_info(sbi, ino, &ni, false);
	if (err)
		goto err_out;

	/* ENOMEM was fully retried in f2fs_evict_inode. */
	if (ni.blk_addr != NULL_ADDR) {
		err = -EIO;
		goto err_out;
	}
	return 0;

err_out:
	set_sbi_flag(sbi, SBI_NEED_FSCK);
	f2fs_warn(sbi, "%s: orphan failed (ino=%x), run fsck to fix.",
		  __func__, ino);
	return err;
}

int f2fs_recover_orphan_inodes(struct f2fs_sb_info *sbi)
{
	block_t start_blk, orphan_blocks, i, j;
	int err = 0;

	if (!is_set_ckpt_flags(sbi, CP_ORPHAN_PRESENT_FLAG))
		return 0;

	if (f2fs_hw_is_readonly(sbi)) {
		f2fs_info(sbi, "write access unavailable, skipping orphan cleanup");
		return 0;
	}

	if (is_sbi_flag_set(sbi, SBI_IS_WRITABLE))
		f2fs_info(sbi, "orphan cleanup on readonly fs");

	start_blk = __start_cp_addr(sbi) + 1 + __cp_payload(sbi);
	orphan_blocks = __start_sum_addr(sbi) - 1 - __cp_payload(sbi);

	f2fs_ra_meta_pages(sbi, start_blk, orphan_blocks, META_CP, true);

	for (i = 0; i < orphan_blocks; i++) {
		struct page *page;
		struct f2fs_orphan_block *orphan_blk;

		page = f2fs_get_meta_page(sbi, start_blk + i);
		if (IS_ERR(page)) {
			err = PTR_ERR(page);
			goto out;
		}

		orphan_blk = (struct f2fs_orphan_block *)page_address(page);
		for (j = 0; j < le32_to_cpu(orphan_blk->entry_count); j++) {
			nid_t ino = le32_to_cpu(orphan_blk->ino[j]);

			err = recover_orphan_inode(sbi, ino);
			if (err) {
				f2fs_put_page(page, 1);
				goto out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			}
		}
		f2fs_put_page(page, 1);
	}
	/* clear Orphan Flag */
<<<<<<< HEAD
	clear_ckpt_flags(F2FS_CKPT(sbi), CP_ORPHAN_PRESENT_FLAG);
	return 0;
=======
	clear_ckpt_flags(sbi, CP_ORPHAN_PRESENT_FLAG);
out:
	set_sbi_flag(sbi, SBI_IS_RECOVERED);

	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void write_orphan_inodes(struct f2fs_sb_info *sbi, block_t start_blk)
{
	struct list_head *head;
	struct f2fs_orphan_block *orphan_blk = NULL;
	unsigned int nentries = 0;
	unsigned short index = 1;
	unsigned short orphan_blocks;
	struct page *page = NULL;
	struct ino_entry *orphan = NULL;
	struct inode_management *im = &sbi->im[ORPHAN_INO];

	orphan_blocks = GET_ORPHAN_BLOCKS(im->ino_num);

	/*
	 * we don't need to do spin_lock(&im->ino_lock) here, since all the
	 * orphan inode operations are covered under f2fs_lock_op().
	 * And, spin_lock should be avoided due to page operations below.
	 */
	head = &im->ino_list;

<<<<<<< HEAD
	/* loop for each orphan inode entry and write them in Jornal block */
	list_for_each_entry(orphan, head, list) {
		if (!page) {
			page = grab_meta_page(sbi, start_blk++);
=======
	/* loop for each orphan inode entry and write them in journal block */
	list_for_each_entry(orphan, head, list) {
		if (!page) {
			page = f2fs_grab_meta_page(sbi, start_blk++);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			orphan_blk =
				(struct f2fs_orphan_block *)page_address(page);
			memset(orphan_blk, 0, sizeof(*orphan_blk));
		}

		orphan_blk->ino[nentries++] = cpu_to_le32(orphan->ino);

		if (nentries == F2FS_ORPHANS_PER_BLOCK) {
			/*
			 * an orphan block is full of 1020 entries,
			 * then we need to flush current orphan blocks
			 * and bring another one in memory
			 */
			orphan_blk->blk_addr = cpu_to_le16(index);
			orphan_blk->blk_count = cpu_to_le16(orphan_blocks);
			orphan_blk->entry_count = cpu_to_le32(nentries);
			set_page_dirty(page);
			f2fs_put_page(page, 1);
			index++;
			nentries = 0;
			page = NULL;
		}
	}

	if (page) {
		orphan_blk->blk_addr = cpu_to_le16(index);
		orphan_blk->blk_count = cpu_to_le16(orphan_blocks);
		orphan_blk->entry_count = cpu_to_le32(nentries);
		set_page_dirty(page);
		f2fs_put_page(page, 1);
	}
}

<<<<<<< HEAD
static struct page *validate_checkpoint(struct f2fs_sb_info *sbi,
				block_t cp_addr, unsigned long long *version)
{
	struct page *cp_page_1, *cp_page_2 = NULL;
	unsigned long blk_size = sbi->blocksize;
	struct f2fs_checkpoint *cp_block;
	unsigned long long cur_version = 0, pre_version = 0;
	size_t crc_offset;
	__u32 crc = 0;

	/* Read the 1st cp block in this CP pack */
	cp_page_1 = get_meta_page(sbi, cp_addr);

	/* get the version number */
	cp_block = (struct f2fs_checkpoint *)page_address(cp_page_1);
	crc_offset = le32_to_cpu(cp_block->checksum_offset);
	if (crc_offset >= blk_size)
		goto invalid_cp1;

	crc = le32_to_cpu(*((__le32 *)((unsigned char *)cp_block + crc_offset)));
	if (!f2fs_crc_valid(crc, cp_block, crc_offset))
		goto invalid_cp1;

	pre_version = cur_cp_version(cp_block);

	/* Read the 2nd cp block in this CP pack */
	cp_addr += le32_to_cpu(cp_block->cp_pack_total_block_count) - 1;
	cp_page_2 = get_meta_page(sbi, cp_addr);

	cp_block = (struct f2fs_checkpoint *)page_address(cp_page_2);
	crc_offset = le32_to_cpu(cp_block->checksum_offset);
	if (crc_offset >= blk_size)
		goto invalid_cp2;

	crc = le32_to_cpu(*((__le32 *)((unsigned char *)cp_block + crc_offset)));
	if (!f2fs_crc_valid(crc, cp_block, crc_offset))
		goto invalid_cp2;

	cur_version = cur_cp_version(cp_block);
=======
static __u32 f2fs_checkpoint_chksum(struct f2fs_sb_info *sbi,
						struct f2fs_checkpoint *ckpt)
{
	unsigned int chksum_ofs = le32_to_cpu(ckpt->checksum_offset);
	__u32 chksum;

	chksum = f2fs_crc32(sbi, ckpt, chksum_ofs);
	if (chksum_ofs < CP_CHKSUM_OFFSET) {
		chksum_ofs += sizeof(chksum);
		chksum = f2fs_chksum(sbi, chksum, (__u8 *)ckpt + chksum_ofs,
						F2FS_BLKSIZE - chksum_ofs);
	}
	return chksum;
}

static int get_checkpoint_version(struct f2fs_sb_info *sbi, block_t cp_addr,
		struct f2fs_checkpoint **cp_block, struct page **cp_page,
		unsigned long long *version)
{
	size_t crc_offset = 0;
	__u32 crc;

	*cp_page = f2fs_get_meta_page(sbi, cp_addr);
	if (IS_ERR(*cp_page))
		return PTR_ERR(*cp_page);

	*cp_block = (struct f2fs_checkpoint *)page_address(*cp_page);

	crc_offset = le32_to_cpu((*cp_block)->checksum_offset);
	if (crc_offset < CP_MIN_CHKSUM_OFFSET ||
			crc_offset > CP_CHKSUM_OFFSET) {
		f2fs_put_page(*cp_page, 1);
		f2fs_warn(sbi, "invalid crc_offset: %zu", crc_offset);
		return -EINVAL;
	}

	crc = f2fs_checkpoint_chksum(sbi, *cp_block);
	if (crc != cur_cp_crc(*cp_block)) {
		f2fs_put_page(*cp_page, 1);
		f2fs_warn(sbi, "invalid crc value");
		return -EINVAL;
	}

	*version = cur_cp_version(*cp_block);
	return 0;
}

static struct page *validate_checkpoint(struct f2fs_sb_info *sbi,
				block_t cp_addr, unsigned long long *version)
{
	struct page *cp_page_1 = NULL, *cp_page_2 = NULL;
	struct f2fs_checkpoint *cp_block = NULL;
	unsigned long long cur_version = 0, pre_version = 0;
	unsigned int cp_blocks;
	int err;

	err = get_checkpoint_version(sbi, cp_addr, &cp_block,
					&cp_page_1, version);
	if (err)
		return NULL;

	cp_blocks = le32_to_cpu(cp_block->cp_pack_total_block_count);

	if (cp_blocks > BLKS_PER_SEG(sbi) || cp_blocks <= F2FS_CP_PACKS) {
		f2fs_warn(sbi, "invalid cp_pack_total_block_count:%u",
			  le32_to_cpu(cp_block->cp_pack_total_block_count));
		goto invalid_cp;
	}
	pre_version = *version;

	cp_addr += cp_blocks - 1;
	err = get_checkpoint_version(sbi, cp_addr, &cp_block,
					&cp_page_2, version);
	if (err)
		goto invalid_cp;
	cur_version = *version;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (cur_version == pre_version) {
		*version = cur_version;
		f2fs_put_page(cp_page_2, 1);
		return cp_page_1;
	}
<<<<<<< HEAD
invalid_cp2:
	f2fs_put_page(cp_page_2, 1);
invalid_cp1:
=======
	f2fs_put_page(cp_page_2, 1);
invalid_cp:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	f2fs_put_page(cp_page_1, 1);
	return NULL;
}

<<<<<<< HEAD
int get_valid_checkpoint(struct f2fs_sb_info *sbi)
=======
int f2fs_get_valid_checkpoint(struct f2fs_sb_info *sbi)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct f2fs_checkpoint *cp_block;
	struct f2fs_super_block *fsb = sbi->raw_super;
	struct page *cp1, *cp2, *cur_page;
	unsigned long blk_size = sbi->blocksize;
	unsigned long long cp1_version = 0, cp2_version = 0;
	unsigned long long cp_start_blk_no;
	unsigned int cp_blks = 1 + __cp_payload(sbi);
	block_t cp_blk_no;
	int i;
<<<<<<< HEAD

	sbi->ckpt = kzalloc(cp_blks * blk_size, GFP_KERNEL);
=======
	int err;

	sbi->ckpt = f2fs_kvzalloc(sbi, array_size(blk_size, cp_blks),
				  GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!sbi->ckpt)
		return -ENOMEM;
	/*
	 * Finding out valid cp block involves read both
<<<<<<< HEAD
	 * sets( cp pack1 and cp pack 2)
=======
	 * sets( cp pack 1 and cp pack 2)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 */
	cp_start_blk_no = le32_to_cpu(fsb->cp_blkaddr);
	cp1 = validate_checkpoint(sbi, cp_start_blk_no, &cp1_version);

	/* The second checkpoint pack should start at the next segment */
	cp_start_blk_no += ((unsigned long long)1) <<
				le32_to_cpu(fsb->log_blocks_per_seg);
	cp2 = validate_checkpoint(sbi, cp_start_blk_no, &cp2_version);

	if (cp1 && cp2) {
		if (ver_after(cp2_version, cp1_version))
			cur_page = cp2;
		else
			cur_page = cp1;
	} else if (cp1) {
		cur_page = cp1;
	} else if (cp2) {
		cur_page = cp2;
	} else {
<<<<<<< HEAD
=======
		err = -EFSCORRUPTED;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto fail_no_cp;
	}

	cp_block = (struct f2fs_checkpoint *)page_address(cur_page);
	memcpy(sbi->ckpt, cp_block, blk_size);

<<<<<<< HEAD
	/* Sanity checking of checkpoint */
	if (sanity_check_ckpt(sbi))
		goto fail_no_cp;
=======
	if (cur_page == cp1)
		sbi->cur_cp_pack = 1;
	else
		sbi->cur_cp_pack = 2;

	/* Sanity checking of checkpoint */
	if (f2fs_sanity_check_ckpt(sbi)) {
		err = -EFSCORRUPTED;
		goto free_fail_no_cp;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (cp_blks <= 1)
		goto done;

	cp_blk_no = le32_to_cpu(fsb->cp_blkaddr);
	if (cur_page == cp2)
<<<<<<< HEAD
		cp_blk_no += 1 << le32_to_cpu(fsb->log_blocks_per_seg);
=======
		cp_blk_no += BIT(le32_to_cpu(fsb->log_blocks_per_seg));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	for (i = 1; i < cp_blks; i++) {
		void *sit_bitmap_ptr;
		unsigned char *ckpt = (unsigned char *)sbi->ckpt;

<<<<<<< HEAD
		cur_page = get_meta_page(sbi, cp_blk_no + i);
=======
		cur_page = f2fs_get_meta_page(sbi, cp_blk_no + i);
		if (IS_ERR(cur_page)) {
			err = PTR_ERR(cur_page);
			goto free_fail_no_cp;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		sit_bitmap_ptr = page_address(cur_page);
		memcpy(ckpt + i * blk_size, sit_bitmap_ptr, blk_size);
		f2fs_put_page(cur_page, 1);
	}
done:
	f2fs_put_page(cp1, 1);
	f2fs_put_page(cp2, 1);
	return 0;

<<<<<<< HEAD
fail_no_cp:
	kfree(sbi->ckpt);
	return -EINVAL;
=======
free_fail_no_cp:
	f2fs_put_page(cp1, 1);
	f2fs_put_page(cp2, 1);
fail_no_cp:
	kvfree(sbi->ckpt);
	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void __add_dirty_inode(struct inode *inode, enum inode_type type)
{
	struct f2fs_sb_info *sbi = F2FS_I_SB(inode);
<<<<<<< HEAD
	struct f2fs_inode_info *fi = F2FS_I(inode);
	int flag = (type == DIR_INODE) ? FI_DIRTY_DIR : FI_DIRTY_FILE;

	if (is_inode_flag_set(fi, flag))
		return;

	set_inode_flag(fi, flag);
	list_add_tail(&fi->dirty_list, &sbi->inode_list[type]);
=======
	int flag = (type == DIR_INODE) ? FI_DIRTY_DIR : FI_DIRTY_FILE;

	if (is_inode_flag_set(inode, flag))
		return;

	set_inode_flag(inode, flag);
	list_add_tail(&F2FS_I(inode)->dirty_list, &sbi->inode_list[type]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	stat_inc_dirty_inode(sbi, type);
}

static void __remove_dirty_inode(struct inode *inode, enum inode_type type)
{
<<<<<<< HEAD
	struct f2fs_inode_info *fi = F2FS_I(inode);
	int flag = (type == DIR_INODE) ? FI_DIRTY_DIR : FI_DIRTY_FILE;

	if (get_dirty_pages(inode) ||
			!is_inode_flag_set(F2FS_I(inode), flag))
		return;

	list_del_init(&fi->dirty_list);
	clear_inode_flag(fi, flag);
	stat_dec_dirty_inode(F2FS_I_SB(inode), type);
}

void update_dirty_page(struct inode *inode, struct page *page)
=======
	int flag = (type == DIR_INODE) ? FI_DIRTY_DIR : FI_DIRTY_FILE;

	if (get_dirty_pages(inode) || !is_inode_flag_set(inode, flag))
		return;

	list_del_init(&F2FS_I(inode)->dirty_list);
	clear_inode_flag(inode, flag);
	stat_dec_dirty_inode(F2FS_I_SB(inode), type);
}

void f2fs_update_dirty_folio(struct inode *inode, struct folio *folio)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct f2fs_sb_info *sbi = F2FS_I_SB(inode);
	enum inode_type type = S_ISDIR(inode->i_mode) ? DIR_INODE : FILE_INODE;

	if (!S_ISDIR(inode->i_mode) && !S_ISREG(inode->i_mode) &&
			!S_ISLNK(inode->i_mode))
		return;

	spin_lock(&sbi->inode_lock[type]);
<<<<<<< HEAD
	__add_dirty_inode(inode, type);
	inode_inc_dirty_pages(inode);
	spin_unlock(&sbi->inode_lock[type]);

	SetPagePrivate(page);
	f2fs_trace_pid(page);
}

void add_dirty_dir_inode(struct inode *inode)
{
	struct f2fs_sb_info *sbi = F2FS_I_SB(inode);

	spin_lock(&sbi->inode_lock[DIR_INODE]);
	__add_dirty_inode(inode, DIR_INODE);
	spin_unlock(&sbi->inode_lock[DIR_INODE]);
}

void remove_dirty_inode(struct inode *inode)
{
	struct f2fs_sb_info *sbi = F2FS_I_SB(inode);
	struct f2fs_inode_info *fi = F2FS_I(inode);
=======
	if (type != FILE_INODE || test_opt(sbi, DATA_FLUSH))
		__add_dirty_inode(inode, type);
	inode_inc_dirty_pages(inode);
	spin_unlock(&sbi->inode_lock[type]);

	set_page_private_reference(&folio->page);
}

void f2fs_remove_dirty_inode(struct inode *inode)
{
	struct f2fs_sb_info *sbi = F2FS_I_SB(inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	enum inode_type type = S_ISDIR(inode->i_mode) ? DIR_INODE : FILE_INODE;

	if (!S_ISDIR(inode->i_mode) && !S_ISREG(inode->i_mode) &&
			!S_ISLNK(inode->i_mode))
		return;

<<<<<<< HEAD
	spin_lock(&sbi->inode_lock[type]);
	__remove_dirty_inode(inode, type);
	spin_unlock(&sbi->inode_lock[type]);

	/* Only from the recovery routine */
	if (is_inode_flag_set(fi, FI_DELAY_IPUT)) {
		clear_inode_flag(fi, FI_DELAY_IPUT);
		iput(inode);
	}
}

int sync_dirty_inodes(struct f2fs_sb_info *sbi, enum inode_type type)
=======
	if (type == FILE_INODE && !test_opt(sbi, DATA_FLUSH))
		return;

	spin_lock(&sbi->inode_lock[type]);
	__remove_dirty_inode(inode, type);
	spin_unlock(&sbi->inode_lock[type]);
}

int f2fs_sync_dirty_inodes(struct f2fs_sb_info *sbi, enum inode_type type,
						bool from_cp)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct list_head *head;
	struct inode *inode;
	struct f2fs_inode_info *fi;
	bool is_dir = (type == DIR_INODE);
<<<<<<< HEAD
=======
	unsigned long ino = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	trace_f2fs_sync_dirty_inodes_enter(sbi->sb, is_dir,
				get_pages(sbi, is_dir ?
				F2FS_DIRTY_DENTS : F2FS_DIRTY_DATA));
retry:
<<<<<<< HEAD
	if (unlikely(f2fs_cp_error(sbi)))
		return -EIO;
=======
	if (unlikely(f2fs_cp_error(sbi))) {
		trace_f2fs_sync_dirty_inodes_exit(sbi->sb, is_dir,
				get_pages(sbi, is_dir ?
				F2FS_DIRTY_DENTS : F2FS_DIRTY_DATA));
		return -EIO;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spin_lock(&sbi->inode_lock[type]);

	head = &sbi->inode_list[type];
	if (list_empty(head)) {
		spin_unlock(&sbi->inode_lock[type]);
		trace_f2fs_sync_dirty_inodes_exit(sbi->sb, is_dir,
				get_pages(sbi, is_dir ?
				F2FS_DIRTY_DENTS : F2FS_DIRTY_DATA));
		return 0;
	}
<<<<<<< HEAD
	fi = list_entry(head->next, struct f2fs_inode_info, dirty_list);
	inode = igrab(&fi->vfs_inode);
	spin_unlock(&sbi->inode_lock[type]);
	if (inode) {
		filemap_fdatawrite(inode->i_mapping);
		iput(inode);
	} else {
		/*
		 * We should submit bio, since it exists several
		 * wribacking dentry pages in the freeing inode.
		 */
		f2fs_submit_merged_bio(sbi, DATA, WRITE);
=======
	fi = list_first_entry(head, struct f2fs_inode_info, dirty_list);
	inode = igrab(&fi->vfs_inode);
	spin_unlock(&sbi->inode_lock[type]);
	if (inode) {
		unsigned long cur_ino = inode->i_ino;

		if (from_cp)
			F2FS_I(inode)->cp_task = current;
		F2FS_I(inode)->wb_task = current;

		filemap_fdatawrite(inode->i_mapping);

		F2FS_I(inode)->wb_task = NULL;
		if (from_cp)
			F2FS_I(inode)->cp_task = NULL;

		iput(inode);
		/* We need to give cpu to another writers. */
		if (ino == cur_ino)
			cond_resched();
		else
			ino = cur_ino;
	} else {
		/*
		 * We should submit bio, since it exists several
		 * writebacking dentry pages in the freeing inode.
		 */
		f2fs_submit_merged_write(sbi, DATA);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		cond_resched();
	}
	goto retry;
}

<<<<<<< HEAD
=======
static int f2fs_sync_inode_meta(struct f2fs_sb_info *sbi)
{
	struct list_head *head = &sbi->inode_list[DIRTY_META];
	struct inode *inode;
	struct f2fs_inode_info *fi;
	s64 total = get_pages(sbi, F2FS_DIRTY_IMETA);

	while (total--) {
		if (unlikely(f2fs_cp_error(sbi)))
			return -EIO;

		spin_lock(&sbi->inode_lock[DIRTY_META]);
		if (list_empty(head)) {
			spin_unlock(&sbi->inode_lock[DIRTY_META]);
			return 0;
		}
		fi = list_first_entry(head, struct f2fs_inode_info,
							gdirty_list);
		inode = igrab(&fi->vfs_inode);
		spin_unlock(&sbi->inode_lock[DIRTY_META]);
		if (inode) {
			sync_inode_metadata(inode, 0);

			/* it's on eviction */
			if (is_inode_flag_set(inode, FI_DIRTY_INODE))
				f2fs_update_inode_page(inode);
			iput(inode);
		}
	}
	return 0;
}

static void __prepare_cp_block(struct f2fs_sb_info *sbi)
{
	struct f2fs_checkpoint *ckpt = F2FS_CKPT(sbi);
	struct f2fs_nm_info *nm_i = NM_I(sbi);
	nid_t last_nid = nm_i->next_scan_nid;

	next_free_nid(sbi, &last_nid);
	ckpt->valid_block_count = cpu_to_le64(valid_user_blocks(sbi));
	ckpt->valid_node_count = cpu_to_le32(valid_node_count(sbi));
	ckpt->valid_inode_count = cpu_to_le32(valid_inode_count(sbi));
	ckpt->next_free_nid = cpu_to_le32(last_nid);
}

static bool __need_flush_quota(struct f2fs_sb_info *sbi)
{
	bool ret = false;

	if (!is_journalled_quota(sbi))
		return false;

	if (!f2fs_down_write_trylock(&sbi->quota_sem))
		return true;
	if (is_sbi_flag_set(sbi, SBI_QUOTA_SKIP_FLUSH)) {
		ret = false;
	} else if (is_sbi_flag_set(sbi, SBI_QUOTA_NEED_REPAIR)) {
		ret = false;
	} else if (is_sbi_flag_set(sbi, SBI_QUOTA_NEED_FLUSH)) {
		clear_sbi_flag(sbi, SBI_QUOTA_NEED_FLUSH);
		ret = true;
	} else if (get_pages(sbi, F2FS_DIRTY_QDATA)) {
		ret = true;
	}
	f2fs_up_write(&sbi->quota_sem);
	return ret;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Freeze all the FS-operations for checkpoint.
 */
static int block_operations(struct f2fs_sb_info *sbi)
{
	struct writeback_control wbc = {
		.sync_mode = WB_SYNC_ALL,
		.nr_to_write = LONG_MAX,
		.for_reclaim = 0,
	};
<<<<<<< HEAD
	struct blk_plug plug;
	int err = 0;

	blk_start_plug(&plug);

retry_flush_dents:
	f2fs_lock_all(sbi);
	/* write all the dirty dentry pages */
	if (get_pages(sbi, F2FS_DIRTY_DENTS)) {
		f2fs_unlock_all(sbi);
		err = sync_dirty_inodes(sbi, DIR_INODE);
		if (err)
			goto out;
		goto retry_flush_dents;
=======
	int err = 0, cnt = 0;

	/*
	 * Let's flush inline_data in dirty node pages.
	 */
	f2fs_flush_inline_data(sbi);

retry_flush_quotas:
	f2fs_lock_all(sbi);
	if (__need_flush_quota(sbi)) {
		int locked;

		if (++cnt > DEFAULT_RETRY_QUOTA_FLUSH_COUNT) {
			set_sbi_flag(sbi, SBI_QUOTA_SKIP_FLUSH);
			set_sbi_flag(sbi, SBI_QUOTA_NEED_FLUSH);
			goto retry_flush_dents;
		}
		f2fs_unlock_all(sbi);

		/* only failed during mount/umount/freeze/quotactl */
		locked = down_read_trylock(&sbi->sb->s_umount);
		f2fs_quota_sync(sbi->sb, -1);
		if (locked)
			up_read(&sbi->sb->s_umount);
		cond_resched();
		goto retry_flush_quotas;
	}

retry_flush_dents:
	/* write all the dirty dentry pages */
	if (get_pages(sbi, F2FS_DIRTY_DENTS)) {
		f2fs_unlock_all(sbi);
		err = f2fs_sync_dirty_inodes(sbi, DIR_INODE, true);
		if (err)
			return err;
		cond_resched();
		goto retry_flush_quotas;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/*
	 * POR: we should ensure that there are no dirty node pages
<<<<<<< HEAD
	 * until finishing nat/sit flush.
	 */
retry_flush_nodes:
	down_write(&sbi->node_write);

	if (get_pages(sbi, F2FS_DIRTY_NODES)) {
		up_write(&sbi->node_write);
		err = sync_node_pages(sbi, 0, &wbc);
		if (err) {
			f2fs_unlock_all(sbi);
			goto out;
		}
		goto retry_flush_nodes;
	}
out:
	blk_finish_plug(&plug);
=======
	 * until finishing nat/sit flush. inode->i_blocks can be updated.
	 */
	f2fs_down_write(&sbi->node_change);

	if (get_pages(sbi, F2FS_DIRTY_IMETA)) {
		f2fs_up_write(&sbi->node_change);
		f2fs_unlock_all(sbi);
		err = f2fs_sync_inode_meta(sbi);
		if (err)
			return err;
		cond_resched();
		goto retry_flush_quotas;
	}

retry_flush_nodes:
	f2fs_down_write(&sbi->node_write);

	if (get_pages(sbi, F2FS_DIRTY_NODES)) {
		f2fs_up_write(&sbi->node_write);
		atomic_inc(&sbi->wb_sync_req[NODE]);
		err = f2fs_sync_node_pages(sbi, &wbc, false, FS_CP_NODE_IO);
		atomic_dec(&sbi->wb_sync_req[NODE]);
		if (err) {
			f2fs_up_write(&sbi->node_change);
			f2fs_unlock_all(sbi);
			return err;
		}
		cond_resched();
		goto retry_flush_nodes;
	}

	/*
	 * sbi->node_change is used only for AIO write_begin path which produces
	 * dirty node blocks and some checkpoint values by block allocation.
	 */
	__prepare_cp_block(sbi);
	f2fs_up_write(&sbi->node_change);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

static void unblock_operations(struct f2fs_sb_info *sbi)
{
<<<<<<< HEAD
	up_write(&sbi->node_write);
	f2fs_unlock_all(sbi);
}

static void wait_on_all_pages_writeback(struct f2fs_sb_info *sbi)
=======
	f2fs_up_write(&sbi->node_write);
	f2fs_unlock_all(sbi);
}

void f2fs_wait_on_all_pages(struct f2fs_sb_info *sbi, int type)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	DEFINE_WAIT(wait);

	for (;;) {
<<<<<<< HEAD
		prepare_to_wait(&sbi->cp_wait, &wait, TASK_UNINTERRUPTIBLE);

		if (!get_pages(sbi, F2FS_WRITEBACK))
			break;

		io_schedule_timeout(5*HZ);
=======
		if (!get_pages(sbi, type))
			break;

		if (unlikely(f2fs_cp_error(sbi) &&
			!is_sbi_flag_set(sbi, SBI_IS_CLOSE)))
			break;

		if (type == F2FS_DIRTY_META)
			f2fs_sync_meta_pages(sbi, META, LONG_MAX,
							FS_CP_META_IO);
		else if (type == F2FS_WB_CP_DATA)
			f2fs_submit_merged_write(sbi, DATA);

		prepare_to_wait(&sbi->cp_wait, &wait, TASK_UNINTERRUPTIBLE);
		io_schedule_timeout(DEFAULT_IO_TIMEOUT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	finish_wait(&sbi->cp_wait, &wait);
}

<<<<<<< HEAD
static int do_checkpoint(struct f2fs_sb_info *sbi, struct cp_control *cpc)
{
	struct f2fs_checkpoint *ckpt = F2FS_CKPT(sbi);
	struct curseg_info *curseg = CURSEG_I(sbi, CURSEG_WARM_NODE);
	struct f2fs_nm_info *nm_i = NM_I(sbi);
	unsigned long orphan_num = sbi->im[ORPHAN_INO].ino_num;
	nid_t last_nid = nm_i->next_scan_nid;
=======
static void update_ckpt_flags(struct f2fs_sb_info *sbi, struct cp_control *cpc)
{
	unsigned long orphan_num = sbi->im[ORPHAN_INO].ino_num;
	struct f2fs_checkpoint *ckpt = F2FS_CKPT(sbi);
	unsigned long flags;

	if (cpc->reason & CP_UMOUNT) {
		if (le32_to_cpu(ckpt->cp_pack_total_block_count) +
			NM_I(sbi)->nat_bits_blocks > BLKS_PER_SEG(sbi)) {
			clear_ckpt_flags(sbi, CP_NAT_BITS_FLAG);
			f2fs_notice(sbi, "Disable nat_bits due to no space");
		} else if (!is_set_ckpt_flags(sbi, CP_NAT_BITS_FLAG) &&
						f2fs_nat_bitmap_enabled(sbi)) {
			f2fs_enable_nat_bits(sbi);
			set_ckpt_flags(sbi, CP_NAT_BITS_FLAG);
			f2fs_notice(sbi, "Rebuild and enable nat_bits");
		}
	}

	spin_lock_irqsave(&sbi->cp_lock, flags);

	if (cpc->reason & CP_TRIMMED)
		__set_ckpt_flags(ckpt, CP_TRIMMED_FLAG);
	else
		__clear_ckpt_flags(ckpt, CP_TRIMMED_FLAG);

	if (cpc->reason & CP_UMOUNT)
		__set_ckpt_flags(ckpt, CP_UMOUNT_FLAG);
	else
		__clear_ckpt_flags(ckpt, CP_UMOUNT_FLAG);

	if (cpc->reason & CP_FASTBOOT)
		__set_ckpt_flags(ckpt, CP_FASTBOOT_FLAG);
	else
		__clear_ckpt_flags(ckpt, CP_FASTBOOT_FLAG);

	if (orphan_num)
		__set_ckpt_flags(ckpt, CP_ORPHAN_PRESENT_FLAG);
	else
		__clear_ckpt_flags(ckpt, CP_ORPHAN_PRESENT_FLAG);

	if (is_sbi_flag_set(sbi, SBI_NEED_FSCK))
		__set_ckpt_flags(ckpt, CP_FSCK_FLAG);

	if (is_sbi_flag_set(sbi, SBI_IS_RESIZEFS))
		__set_ckpt_flags(ckpt, CP_RESIZEFS_FLAG);
	else
		__clear_ckpt_flags(ckpt, CP_RESIZEFS_FLAG);

	if (is_sbi_flag_set(sbi, SBI_CP_DISABLED))
		__set_ckpt_flags(ckpt, CP_DISABLED_FLAG);
	else
		__clear_ckpt_flags(ckpt, CP_DISABLED_FLAG);

	if (is_sbi_flag_set(sbi, SBI_CP_DISABLED_QUICK))
		__set_ckpt_flags(ckpt, CP_DISABLED_QUICK_FLAG);
	else
		__clear_ckpt_flags(ckpt, CP_DISABLED_QUICK_FLAG);

	if (is_sbi_flag_set(sbi, SBI_QUOTA_SKIP_FLUSH))
		__set_ckpt_flags(ckpt, CP_QUOTA_NEED_FSCK_FLAG);
	else
		__clear_ckpt_flags(ckpt, CP_QUOTA_NEED_FSCK_FLAG);

	if (is_sbi_flag_set(sbi, SBI_QUOTA_NEED_REPAIR))
		__set_ckpt_flags(ckpt, CP_QUOTA_NEED_FSCK_FLAG);

	/* set this flag to activate crc|cp_ver for recovery */
	__set_ckpt_flags(ckpt, CP_CRC_RECOVERY_FLAG);
	__clear_ckpt_flags(ckpt, CP_NOCRC_RECOVERY_FLAG);

	spin_unlock_irqrestore(&sbi->cp_lock, flags);
}

static void commit_checkpoint(struct f2fs_sb_info *sbi,
	void *src, block_t blk_addr)
{
	struct writeback_control wbc = {
		.for_reclaim = 0,
	};

	/*
	 * filemap_get_folios_tag and lock_page again will take
	 * some extra time. Therefore, f2fs_update_meta_pages and
	 * f2fs_sync_meta_pages are combined in this function.
	 */
	struct page *page = f2fs_grab_meta_page(sbi, blk_addr);
	int err;

	f2fs_wait_on_page_writeback(page, META, true, true);

	memcpy(page_address(page), src, PAGE_SIZE);

	set_page_dirty(page);
	if (unlikely(!clear_page_dirty_for_io(page)))
		f2fs_bug_on(sbi, 1);

	/* writeout cp pack 2 page */
	err = __f2fs_write_meta_page(page, &wbc, FS_CP_META_IO);
	if (unlikely(err && f2fs_cp_error(sbi))) {
		f2fs_put_page(page, 1);
		return;
	}

	f2fs_bug_on(sbi, err);
	f2fs_put_page(page, 0);

	/* submit checkpoint (with barrier if NOBARRIER is not set) */
	f2fs_submit_merged_write(sbi, META_FLUSH);
}

static inline u64 get_sectors_written(struct block_device *bdev)
{
	return (u64)part_stat_read(bdev, sectors[STAT_WRITE]);
}

u64 f2fs_get_sectors_written(struct f2fs_sb_info *sbi)
{
	if (f2fs_is_multi_device(sbi)) {
		u64 sectors = 0;
		int i;

		for (i = 0; i < sbi->s_ndevs; i++)
			sectors += get_sectors_written(FDEV(i).bdev);

		return sectors;
	}

	return get_sectors_written(sbi->sb->s_bdev);
}

static int do_checkpoint(struct f2fs_sb_info *sbi, struct cp_control *cpc)
{
	struct f2fs_checkpoint *ckpt = F2FS_CKPT(sbi);
	struct f2fs_nm_info *nm_i = NM_I(sbi);
	unsigned long orphan_num = sbi->im[ORPHAN_INO].ino_num, flags;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	block_t start_blk;
	unsigned int data_sum_blocks, orphan_blocks;
	__u32 crc32 = 0;
	int i;
	int cp_payload_blks = __cp_payload(sbi);
<<<<<<< HEAD
	block_t discard_blk = NEXT_FREE_BLKADDR(sbi, curseg);
	bool invalidate = false;
	struct super_block *sb = sbi->sb;
	struct curseg_info *seg_i = CURSEG_I(sbi, CURSEG_HOT_NODE);
	u64 kbytes_written;

	/*
	 * This avoids to conduct wrong roll-forward operations and uses
	 * metapages, so should be called prior to sync_meta_pages below.
	 */
	if (discard_next_dnode(sbi, discard_blk))
		invalidate = true;

	/* Flush all the NAT/SIT pages */
	while (get_pages(sbi, F2FS_DIRTY_META)) {
		sync_meta_pages(sbi, META, LONG_MAX);
		if (unlikely(f2fs_cp_error(sbi)))
			return -EIO;
	}

	next_free_nid(sbi, &last_nid);

	/*
	 * modify checkpoint
	 * version number is already updated
	 */
	ckpt->elapsed_time = cpu_to_le64(get_mtime(sbi));
	ckpt->valid_block_count = cpu_to_le64(valid_user_blocks(sbi));
	ckpt->free_segment_count = cpu_to_le32(free_segments(sbi));
	for (i = 0; i < NR_CURSEG_NODE_TYPE; i++) {
		ckpt->cur_node_segno[i] =
			cpu_to_le32(curseg_segno(sbi, i + CURSEG_HOT_NODE));
		ckpt->cur_node_blkoff[i] =
			cpu_to_le16(curseg_blkoff(sbi, i + CURSEG_HOT_NODE));
		ckpt->alloc_type[i + CURSEG_HOT_NODE] =
				curseg_alloc_type(sbi, i + CURSEG_HOT_NODE);
	}
	for (i = 0; i < NR_CURSEG_DATA_TYPE; i++) {
		ckpt->cur_data_segno[i] =
			cpu_to_le32(curseg_segno(sbi, i + CURSEG_HOT_DATA));
		ckpt->cur_data_blkoff[i] =
			cpu_to_le16(curseg_blkoff(sbi, i + CURSEG_HOT_DATA));
		ckpt->alloc_type[i + CURSEG_HOT_DATA] =
				curseg_alloc_type(sbi, i + CURSEG_HOT_DATA);
	}

	ckpt->valid_node_count = cpu_to_le32(valid_node_count(sbi));
	ckpt->valid_inode_count = cpu_to_le32(valid_inode_count(sbi));
	ckpt->next_free_nid = cpu_to_le32(last_nid);

	/* 2 cp  + n data seg summary + orphan inode blocks */
	data_sum_blocks = npages_for_summary_flush(sbi, false);
	if (data_sum_blocks < NR_CURSEG_DATA_TYPE)
		set_ckpt_flags(ckpt, CP_COMPACT_SUM_FLAG);
	else
		clear_ckpt_flags(ckpt, CP_COMPACT_SUM_FLAG);
=======
	struct curseg_info *seg_i = CURSEG_I(sbi, CURSEG_HOT_NODE);
	u64 kbytes_written;
	int err;

	/* Flush all the NAT/SIT pages */
	f2fs_sync_meta_pages(sbi, META, LONG_MAX, FS_CP_META_IO);

	/* start to update checkpoint, cp ver is already updated previously */
	ckpt->elapsed_time = cpu_to_le64(get_mtime(sbi, true));
	ckpt->free_segment_count = cpu_to_le32(free_segments(sbi));
	for (i = 0; i < NR_CURSEG_NODE_TYPE; i++) {
		struct curseg_info *curseg = CURSEG_I(sbi, i + CURSEG_HOT_NODE);

		ckpt->cur_node_segno[i] = cpu_to_le32(curseg->segno);
		ckpt->cur_node_blkoff[i] = cpu_to_le16(curseg->next_blkoff);
		ckpt->alloc_type[i + CURSEG_HOT_NODE] = curseg->alloc_type;
	}
	for (i = 0; i < NR_CURSEG_DATA_TYPE; i++) {
		struct curseg_info *curseg = CURSEG_I(sbi, i + CURSEG_HOT_DATA);

		ckpt->cur_data_segno[i] = cpu_to_le32(curseg->segno);
		ckpt->cur_data_blkoff[i] = cpu_to_le16(curseg->next_blkoff);
		ckpt->alloc_type[i + CURSEG_HOT_DATA] = curseg->alloc_type;
	}

	/* 2 cp + n data seg summary + orphan inode blocks */
	data_sum_blocks = f2fs_npages_for_summary_flush(sbi, false);
	spin_lock_irqsave(&sbi->cp_lock, flags);
	if (data_sum_blocks < NR_CURSEG_DATA_TYPE)
		__set_ckpt_flags(ckpt, CP_COMPACT_SUM_FLAG);
	else
		__clear_ckpt_flags(ckpt, CP_COMPACT_SUM_FLAG);
	spin_unlock_irqrestore(&sbi->cp_lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	orphan_blocks = GET_ORPHAN_BLOCKS(orphan_num);
	ckpt->cp_pack_start_sum = cpu_to_le32(1 + cp_payload_blks +
			orphan_blocks);

	if (__remain_node_summaries(cpc->reason))
<<<<<<< HEAD
		ckpt->cp_pack_total_block_count = cpu_to_le32(F2FS_CP_PACKS+
=======
		ckpt->cp_pack_total_block_count = cpu_to_le32(F2FS_CP_PACKS +
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				cp_payload_blks + data_sum_blocks +
				orphan_blocks + NR_CURSEG_NODE_TYPE);
	else
		ckpt->cp_pack_total_block_count = cpu_to_le32(F2FS_CP_PACKS +
				cp_payload_blks + data_sum_blocks +
				orphan_blocks);

<<<<<<< HEAD
	if (cpc->reason == CP_UMOUNT)
		set_ckpt_flags(ckpt, CP_UMOUNT_FLAG);
	else
		clear_ckpt_flags(ckpt, CP_UMOUNT_FLAG);

	if (cpc->reason == CP_FASTBOOT)
		set_ckpt_flags(ckpt, CP_FASTBOOT_FLAG);
	else
		clear_ckpt_flags(ckpt, CP_FASTBOOT_FLAG);

	if (orphan_num)
		set_ckpt_flags(ckpt, CP_ORPHAN_PRESENT_FLAG);
	else
		clear_ckpt_flags(ckpt, CP_ORPHAN_PRESENT_FLAG);

	if (is_sbi_flag_set(sbi, SBI_NEED_FSCK))
		set_ckpt_flags(ckpt, CP_FSCK_FLAG);
=======
	/* update ckpt flag for checkpoint */
	update_ckpt_flags(sbi, cpc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* update SIT/NAT bitmap */
	get_sit_bitmap(sbi, __bitmap_ptr(sbi, SIT_BITMAP));
	get_nat_bitmap(sbi, __bitmap_ptr(sbi, NAT_BITMAP));

<<<<<<< HEAD
	crc32 = f2fs_crc32(ckpt, le32_to_cpu(ckpt->checksum_offset));
=======
	crc32 = f2fs_checkpoint_chksum(sbi, ckpt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	*((__le32 *)((unsigned char *)ckpt +
				le32_to_cpu(ckpt->checksum_offset)))
				= cpu_to_le32(crc32);

<<<<<<< HEAD
	start_blk = __start_cp_addr(sbi);

	/* need to wait for end_io results */
	wait_on_all_pages_writeback(sbi);
	if (unlikely(f2fs_cp_error(sbi)))
		return -EIO;

	/* write out checkpoint buffer at block 0 */
	update_meta_page(sbi, ckpt, start_blk++);

	for (i = 1; i < 1 + cp_payload_blks; i++)
		update_meta_page(sbi, (char *)ckpt + i * F2FS_BLKSIZE,
=======
	start_blk = __start_cp_next_addr(sbi);

	/* write nat bits */
	if ((cpc->reason & CP_UMOUNT) &&
			is_set_ckpt_flags(sbi, CP_NAT_BITS_FLAG)) {
		__u64 cp_ver = cur_cp_version(ckpt);
		block_t blk;

		cp_ver |= ((__u64)crc32 << 32);
		*(__le64 *)nm_i->nat_bits = cpu_to_le64(cp_ver);

		blk = start_blk + BLKS_PER_SEG(sbi) - nm_i->nat_bits_blocks;
		for (i = 0; i < nm_i->nat_bits_blocks; i++)
			f2fs_update_meta_page(sbi, nm_i->nat_bits +
					(i << F2FS_BLKSIZE_BITS), blk + i);
	}

	/* write out checkpoint buffer at block 0 */
	f2fs_update_meta_page(sbi, ckpt, start_blk++);

	for (i = 1; i < 1 + cp_payload_blks; i++)
		f2fs_update_meta_page(sbi, (char *)ckpt + i * F2FS_BLKSIZE,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
							start_blk++);

	if (orphan_num) {
		write_orphan_inodes(sbi, start_blk);
		start_blk += orphan_blocks;
	}

<<<<<<< HEAD
	write_data_summaries(sbi, start_blk);
=======
	f2fs_write_data_summaries(sbi, start_blk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	start_blk += data_sum_blocks;

	/* Record write statistics in the hot node summary */
	kbytes_written = sbi->kbytes_written;
<<<<<<< HEAD
	if (sb->s_bdev->bd_part)
		kbytes_written += BD_PART_WRITTEN(sbi);

	seg_i->journal->info.kbytes_written = cpu_to_le64(kbytes_written);

	if (__remain_node_summaries(cpc->reason)) {
		write_node_summaries(sbi, start_blk);
		start_blk += NR_CURSEG_NODE_TYPE;
	}

	/* writeout checkpoint block */
	update_meta_page(sbi, ckpt, start_blk);

	/* wait for previous submitted node/meta pages writeback */
	wait_on_all_pages_writeback(sbi);

	if (unlikely(f2fs_cp_error(sbi)))
		return -EIO;

	filemap_fdatawait_range(NODE_MAPPING(sbi), 0, LLONG_MAX);
	filemap_fdatawait_range(META_MAPPING(sbi), 0, LLONG_MAX);

	/* update user_block_counts */
	sbi->last_valid_block_count = sbi->total_valid_block_count;
	sbi->alloc_valid_block_count = 0;

	/* Here, we only have one bio having CP pack */
	sync_meta_pages(sbi, META_FLUSH, LONG_MAX);

	/* wait for previous submitted meta pages writeback */
	wait_on_all_pages_writeback(sbi);

	/*
	 * invalidate meta page which is used temporarily for zeroing out
	 * block at the end of warm node chain.
	 */
	if (invalidate)
		invalidate_mapping_pages(META_MAPPING(sbi), discard_blk,
								discard_blk);

	release_ino_entry(sbi);

	if (unlikely(f2fs_cp_error(sbi)))
		return -EIO;

	clear_prefree_segments(sbi, cpc);
	clear_sbi_flag(sbi, SBI_IS_DIRTY);

	return 0;
}

/*
 * We guarantee that this checkpoint procedure will not fail.
 */
int write_checkpoint(struct f2fs_sb_info *sbi, struct cp_control *cpc)
=======
	kbytes_written += (f2fs_get_sectors_written(sbi) -
				sbi->sectors_written_start) >> 1;
	seg_i->journal->info.kbytes_written = cpu_to_le64(kbytes_written);

	if (__remain_node_summaries(cpc->reason)) {
		f2fs_write_node_summaries(sbi, start_blk);
		start_blk += NR_CURSEG_NODE_TYPE;
	}

	/* update user_block_counts */
	sbi->last_valid_block_count = sbi->total_valid_block_count;
	percpu_counter_set(&sbi->alloc_valid_block_count, 0);
	percpu_counter_set(&sbi->rf_node_block_count, 0);

	/* Here, we have one bio having CP pack except cp pack 2 page */
	f2fs_sync_meta_pages(sbi, META, LONG_MAX, FS_CP_META_IO);
	/* Wait for all dirty meta pages to be submitted for IO */
	f2fs_wait_on_all_pages(sbi, F2FS_DIRTY_META);

	/* wait for previous submitted meta pages writeback */
	f2fs_wait_on_all_pages(sbi, F2FS_WB_CP_DATA);

	/* flush all device cache */
	err = f2fs_flush_device_cache(sbi);
	if (err)
		return err;

	/* barrier and flush checkpoint cp pack 2 page if it can */
	commit_checkpoint(sbi, ckpt, start_blk);
	f2fs_wait_on_all_pages(sbi, F2FS_WB_CP_DATA);

	/*
	 * invalidate intermediate page cache borrowed from meta inode which are
	 * used for migration of encrypted, verity or compressed inode's blocks.
	 */
	if (f2fs_sb_has_encrypt(sbi) || f2fs_sb_has_verity(sbi) ||
		f2fs_sb_has_compression(sbi))
		f2fs_bug_on(sbi,
			invalidate_inode_pages2_range(META_MAPPING(sbi),
				MAIN_BLKADDR(sbi), MAX_BLKADDR(sbi) - 1));

	f2fs_release_ino_entry(sbi, false);

	f2fs_reset_fsync_node_info(sbi);

	clear_sbi_flag(sbi, SBI_IS_DIRTY);
	clear_sbi_flag(sbi, SBI_NEED_CP);
	clear_sbi_flag(sbi, SBI_QUOTA_SKIP_FLUSH);

	spin_lock(&sbi->stat_lock);
	sbi->unusable_block_count = 0;
	spin_unlock(&sbi->stat_lock);

	__set_cp_next_pack(sbi);

	/*
	 * redirty superblock if metadata like node page or inode cache is
	 * updated during writing checkpoint.
	 */
	if (get_pages(sbi, F2FS_DIRTY_NODES) ||
			get_pages(sbi, F2FS_DIRTY_IMETA))
		set_sbi_flag(sbi, SBI_IS_DIRTY);

	f2fs_bug_on(sbi, get_pages(sbi, F2FS_DIRTY_DENTS));

	return unlikely(f2fs_cp_error(sbi)) ? -EIO : 0;
}

int f2fs_write_checkpoint(struct f2fs_sb_info *sbi, struct cp_control *cpc)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct f2fs_checkpoint *ckpt = F2FS_CKPT(sbi);
	unsigned long long ckpt_ver;
	int err = 0;

<<<<<<< HEAD
	mutex_lock(&sbi->cp_mutex);

	if (!is_sbi_flag_set(sbi, SBI_IS_DIRTY) &&
		(cpc->reason == CP_FASTBOOT || cpc->reason == CP_SYNC ||
		(cpc->reason == CP_DISCARD && !sbi->discard_blks)))
=======
	if (f2fs_readonly(sbi->sb) || f2fs_hw_is_readonly(sbi))
		return -EROFS;

	if (unlikely(is_sbi_flag_set(sbi, SBI_CP_DISABLED))) {
		if (cpc->reason != CP_PAUSE)
			return 0;
		f2fs_warn(sbi, "Start checkpoint disabled!");
	}
	if (cpc->reason != CP_RESIZE)
		f2fs_down_write(&sbi->cp_global_sem);

	if (!is_sbi_flag_set(sbi, SBI_IS_DIRTY) &&
		((cpc->reason & CP_FASTBOOT) || (cpc->reason & CP_SYNC) ||
		((cpc->reason & CP_DISCARD) && !sbi->discard_blks)))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;
	if (unlikely(f2fs_cp_error(sbi))) {
		err = -EIO;
		goto out;
	}
<<<<<<< HEAD
	if (f2fs_readonly(sbi->sb)) {
		err = -EROFS;
		goto out;
	}
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	trace_f2fs_write_checkpoint(sbi->sb, cpc->reason, "start block_ops");

	err = block_operations(sbi);
	if (err)
		goto out;

	trace_f2fs_write_checkpoint(sbi->sb, cpc->reason, "finish block_ops");

<<<<<<< HEAD
	f2fs_flush_merged_bios(sbi);
=======
	f2fs_flush_merged_writes(sbi);

	/* this is the case of multiple fstrims without any changes */
	if (cpc->reason & CP_DISCARD) {
		if (!f2fs_exist_trim_candidates(sbi, cpc)) {
			unblock_operations(sbi);
			goto out;
		}

		if (NM_I(sbi)->nat_cnt[DIRTY_NAT] == 0 &&
				SIT_I(sbi)->dirty_sentries == 0 &&
				prefree_segments(sbi) == 0) {
			f2fs_flush_sit_entries(sbi, cpc);
			f2fs_clear_prefree_segments(sbi, cpc);
			unblock_operations(sbi);
			goto out;
		}
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * update checkpoint pack index
	 * Increase the version number so that
	 * SIT entries and seg summaries are written at correct place
	 */
	ckpt_ver = cur_cp_version(ckpt);
	ckpt->checkpoint_ver = cpu_to_le64(++ckpt_ver);

	/* write cached NAT/SIT entries to NAT/SIT area */
<<<<<<< HEAD
	flush_nat_entries(sbi);
	flush_sit_entries(sbi, cpc);

	/* unlock all the fs_lock[] in do_checkpoint() */
	err = do_checkpoint(sbi, cpc);

	unblock_operations(sbi);
	stat_inc_cp_count(sbi->stat_info);

	if (cpc->reason == CP_RECOVERY)
		f2fs_msg(sbi->sb, KERN_NOTICE,
			"checkpoint: version = %llx", ckpt_ver);

	/* do checkpoint periodically */
	f2fs_update_time(sbi, CP_TIME);
	trace_f2fs_write_checkpoint(sbi->sb, cpc->reason, "finish checkpoint");
out:
	mutex_unlock(&sbi->cp_mutex);
	return err;
}

void init_ino_entry_info(struct f2fs_sb_info *sbi)
=======
	err = f2fs_flush_nat_entries(sbi, cpc);
	if (err) {
		f2fs_err(sbi, "f2fs_flush_nat_entries failed err:%d, stop checkpoint", err);
		f2fs_bug_on(sbi, !f2fs_cp_error(sbi));
		goto stop;
	}

	f2fs_flush_sit_entries(sbi, cpc);

	/* save inmem log status */
	f2fs_save_inmem_curseg(sbi);

	err = do_checkpoint(sbi, cpc);
	if (err) {
		f2fs_err(sbi, "do_checkpoint failed err:%d, stop checkpoint", err);
		f2fs_bug_on(sbi, !f2fs_cp_error(sbi));
		f2fs_release_discard_addrs(sbi);
	} else {
		f2fs_clear_prefree_segments(sbi, cpc);
	}

	f2fs_restore_inmem_curseg(sbi);
	stat_inc_cp_count(sbi);
stop:
	unblock_operations(sbi);

	if (cpc->reason & CP_RECOVERY)
		f2fs_notice(sbi, "checkpoint: version = %llx", ckpt_ver);

	/* update CP_TIME to trigger checkpoint periodically */
	f2fs_update_time(sbi, CP_TIME);
	trace_f2fs_write_checkpoint(sbi->sb, cpc->reason, "finish checkpoint");
out:
	if (cpc->reason != CP_RESIZE)
		f2fs_up_write(&sbi->cp_global_sem);
	return err;
}

void f2fs_init_ino_entry_info(struct f2fs_sb_info *sbi)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int i;

	for (i = 0; i < MAX_INO_ENTRY; i++) {
		struct inode_management *im = &sbi->im[i];

		INIT_RADIX_TREE(&im->ino_root, GFP_ATOMIC);
		spin_lock_init(&im->ino_lock);
		INIT_LIST_HEAD(&im->ino_list);
		im->ino_num = 0;
	}

<<<<<<< HEAD
	sbi->max_orphans = (sbi->blocks_per_seg - F2FS_CP_PACKS -
			NR_CURSEG_TYPE - __cp_payload(sbi)) *
				F2FS_ORPHANS_PER_BLOCK;
}

int __init create_checkpoint_caches(void)
=======
	sbi->max_orphans = (BLKS_PER_SEG(sbi) - F2FS_CP_PACKS -
			NR_CURSEG_PERSIST_TYPE - __cp_payload(sbi)) *
			F2FS_ORPHANS_PER_BLOCK;
}

int __init f2fs_create_checkpoint_caches(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	ino_entry_slab = f2fs_kmem_cache_create("f2fs_ino_entry",
			sizeof(struct ino_entry));
	if (!ino_entry_slab)
		return -ENOMEM;
<<<<<<< HEAD
	inode_entry_slab = f2fs_kmem_cache_create("f2fs_inode_entry",
			sizeof(struct inode_entry));
	if (!inode_entry_slab) {
=======
	f2fs_inode_entry_slab = f2fs_kmem_cache_create("f2fs_inode_entry",
			sizeof(struct inode_entry));
	if (!f2fs_inode_entry_slab) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		kmem_cache_destroy(ino_entry_slab);
		return -ENOMEM;
	}
	return 0;
}

<<<<<<< HEAD
void destroy_checkpoint_caches(void)
{
	kmem_cache_destroy(ino_entry_slab);
	kmem_cache_destroy(inode_entry_slab);
=======
void f2fs_destroy_checkpoint_caches(void)
{
	kmem_cache_destroy(ino_entry_slab);
	kmem_cache_destroy(f2fs_inode_entry_slab);
}

static int __write_checkpoint_sync(struct f2fs_sb_info *sbi)
{
	struct cp_control cpc = { .reason = CP_SYNC, };
	int err;

	f2fs_down_write(&sbi->gc_lock);
	err = f2fs_write_checkpoint(sbi, &cpc);
	f2fs_up_write(&sbi->gc_lock);

	return err;
}

static void __checkpoint_and_complete_reqs(struct f2fs_sb_info *sbi)
{
	struct ckpt_req_control *cprc = &sbi->cprc_info;
	struct ckpt_req *req, *next;
	struct llist_node *dispatch_list;
	u64 sum_diff = 0, diff, count = 0;
	int ret;

	dispatch_list = llist_del_all(&cprc->issue_list);
	if (!dispatch_list)
		return;
	dispatch_list = llist_reverse_order(dispatch_list);

	ret = __write_checkpoint_sync(sbi);
	atomic_inc(&cprc->issued_ckpt);

	llist_for_each_entry_safe(req, next, dispatch_list, llnode) {
		diff = (u64)ktime_ms_delta(ktime_get(), req->queue_time);
		req->ret = ret;
		complete(&req->wait);

		sum_diff += diff;
		count++;
	}
	atomic_sub(count, &cprc->queued_ckpt);
	atomic_add(count, &cprc->total_ckpt);

	spin_lock(&cprc->stat_lock);
	cprc->cur_time = (unsigned int)div64_u64(sum_diff, count);
	if (cprc->peak_time < cprc->cur_time)
		cprc->peak_time = cprc->cur_time;
	spin_unlock(&cprc->stat_lock);
}

static int issue_checkpoint_thread(void *data)
{
	struct f2fs_sb_info *sbi = data;
	struct ckpt_req_control *cprc = &sbi->cprc_info;
	wait_queue_head_t *q = &cprc->ckpt_wait_queue;
repeat:
	if (kthread_should_stop())
		return 0;

	if (!llist_empty(&cprc->issue_list))
		__checkpoint_and_complete_reqs(sbi);

	wait_event_interruptible(*q,
		kthread_should_stop() || !llist_empty(&cprc->issue_list));
	goto repeat;
}

static void flush_remained_ckpt_reqs(struct f2fs_sb_info *sbi,
		struct ckpt_req *wait_req)
{
	struct ckpt_req_control *cprc = &sbi->cprc_info;

	if (!llist_empty(&cprc->issue_list)) {
		__checkpoint_and_complete_reqs(sbi);
	} else {
		/* already dispatched by issue_checkpoint_thread */
		if (wait_req)
			wait_for_completion(&wait_req->wait);
	}
}

static void init_ckpt_req(struct ckpt_req *req)
{
	memset(req, 0, sizeof(struct ckpt_req));

	init_completion(&req->wait);
	req->queue_time = ktime_get();
}

int f2fs_issue_checkpoint(struct f2fs_sb_info *sbi)
{
	struct ckpt_req_control *cprc = &sbi->cprc_info;
	struct ckpt_req req;
	struct cp_control cpc;

	cpc.reason = __get_cp_reason(sbi);
	if (!test_opt(sbi, MERGE_CHECKPOINT) || cpc.reason != CP_SYNC) {
		int ret;

		f2fs_down_write(&sbi->gc_lock);
		ret = f2fs_write_checkpoint(sbi, &cpc);
		f2fs_up_write(&sbi->gc_lock);

		return ret;
	}

	if (!cprc->f2fs_issue_ckpt)
		return __write_checkpoint_sync(sbi);

	init_ckpt_req(&req);

	llist_add(&req.llnode, &cprc->issue_list);
	atomic_inc(&cprc->queued_ckpt);

	/*
	 * update issue_list before we wake up issue_checkpoint thread,
	 * this smp_mb() pairs with another barrier in ___wait_event(),
	 * see more details in comments of waitqueue_active().
	 */
	smp_mb();

	if (waitqueue_active(&cprc->ckpt_wait_queue))
		wake_up(&cprc->ckpt_wait_queue);

	if (cprc->f2fs_issue_ckpt)
		wait_for_completion(&req.wait);
	else
		flush_remained_ckpt_reqs(sbi, &req);

	return req.ret;
}

int f2fs_start_ckpt_thread(struct f2fs_sb_info *sbi)
{
	dev_t dev = sbi->sb->s_bdev->bd_dev;
	struct ckpt_req_control *cprc = &sbi->cprc_info;

	if (cprc->f2fs_issue_ckpt)
		return 0;

	cprc->f2fs_issue_ckpt = kthread_run(issue_checkpoint_thread, sbi,
			"f2fs_ckpt-%u:%u", MAJOR(dev), MINOR(dev));
	if (IS_ERR(cprc->f2fs_issue_ckpt)) {
		int err = PTR_ERR(cprc->f2fs_issue_ckpt);

		cprc->f2fs_issue_ckpt = NULL;
		return err;
	}

	set_task_ioprio(cprc->f2fs_issue_ckpt, cprc->ckpt_thread_ioprio);

	return 0;
}

void f2fs_stop_ckpt_thread(struct f2fs_sb_info *sbi)
{
	struct ckpt_req_control *cprc = &sbi->cprc_info;
	struct task_struct *ckpt_task;

	if (!cprc->f2fs_issue_ckpt)
		return;

	ckpt_task = cprc->f2fs_issue_ckpt;
	cprc->f2fs_issue_ckpt = NULL;
	kthread_stop(ckpt_task);

	f2fs_flush_ckpt_thread(sbi);
}

void f2fs_flush_ckpt_thread(struct f2fs_sb_info *sbi)
{
	struct ckpt_req_control *cprc = &sbi->cprc_info;

	flush_remained_ckpt_reqs(sbi, NULL);

	/* Let's wait for the previous dispatched checkpoint. */
	while (atomic_read(&cprc->queued_ckpt))
		io_schedule_timeout(DEFAULT_IO_TIMEOUT);
}

void f2fs_init_ckpt_req_control(struct f2fs_sb_info *sbi)
{
	struct ckpt_req_control *cprc = &sbi->cprc_info;

	atomic_set(&cprc->issued_ckpt, 0);
	atomic_set(&cprc->total_ckpt, 0);
	atomic_set(&cprc->queued_ckpt, 0);
	cprc->ckpt_thread_ioprio = DEFAULT_CHECKPOINT_IOPRIO;
	init_waitqueue_head(&cprc->ckpt_wait_queue);
	init_llist_head(&cprc->issue_list);
	spin_lock_init(&cprc->stat_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
