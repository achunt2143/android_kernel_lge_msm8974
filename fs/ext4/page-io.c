<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * linux/fs/ext4/page-io.c
 *
 * This contains the new page_io functions for ext4
 *
 * Written by Theodore Ts'o, 2010.
 */

#include <linux/fs.h>
#include <linux/time.h>
<<<<<<< HEAD
#include <linux/jbd2.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/highuid.h>
#include <linux/pagemap.h>
#include <linux/quotaops.h>
#include <linux/string.h>
#include <linux/buffer_head.h>
#include <linux/writeback.h>
#include <linux/pagevec.h>
#include <linux/mpage.h>
#include <linux/namei.h>
#include <linux/uio.h>
#include <linux/bio.h>
#include <linux/workqueue.h>
#include <linux/kernel.h>
#include <linux/slab.h>
<<<<<<< HEAD
=======
#include <linux/mm.h>
#include <linux/sched/mm.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include "ext4_jbd2.h"
#include "xattr.h"
#include "acl.h"
<<<<<<< HEAD
#include "ext4_extents.h"

static struct kmem_cache *io_page_cachep, *io_end_cachep;

int __init ext4_init_pageio(void)
{
	io_page_cachep = KMEM_CACHE(ext4_io_page, SLAB_RECLAIM_ACCOUNT);
	if (io_page_cachep == NULL)
		return -ENOMEM;
	io_end_cachep = KMEM_CACHE(ext4_io_end, SLAB_RECLAIM_ACCOUNT);
	if (io_end_cachep == NULL) {
		kmem_cache_destroy(io_page_cachep);
=======

static struct kmem_cache *io_end_cachep;
static struct kmem_cache *io_end_vec_cachep;

int __init ext4_init_pageio(void)
{
	io_end_cachep = KMEM_CACHE(ext4_io_end, SLAB_RECLAIM_ACCOUNT);
	if (io_end_cachep == NULL)
		return -ENOMEM;

	io_end_vec_cachep = KMEM_CACHE(ext4_io_end_vec, 0);
	if (io_end_vec_cachep == NULL) {
		kmem_cache_destroy(io_end_cachep);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ENOMEM;
	}
	return 0;
}

void ext4_exit_pageio(void)
{
	kmem_cache_destroy(io_end_cachep);
<<<<<<< HEAD
	kmem_cache_destroy(io_page_cachep);
}

void ext4_ioend_wait(struct inode *inode)
{
	wait_queue_head_t *wq = ext4_ioend_wq(inode);

	wait_event(*wq, (atomic_read(&EXT4_I(inode)->i_ioend_count) == 0));
}

static void put_io_page(struct ext4_io_page *io_page)
{
	if (atomic_dec_and_test(&io_page->p_count)) {
		end_page_writeback(io_page->p_page);
		put_page(io_page->p_page);
		kmem_cache_free(io_page_cachep, io_page);
	}
}

void ext4_free_io_end(ext4_io_end_t *io)
{
	int i;

	BUG_ON(!io);
	if (io->page)
		put_page(io->page);
	for (i = 0; i < io->num_io_pages; i++)
		put_io_page(io->pages[i]);
	io->num_io_pages = 0;
	if (atomic_dec_and_test(&EXT4_I(io->inode)->i_ioend_count))
		wake_up_all(ext4_ioend_wq(io->inode));
	kmem_cache_free(io_end_cachep, io);
}

/*
 * check a range of space and convert unwritten extents to written.
 *
 * Called with inode->i_mutex; we depend on this when we manipulate
 * io->flag, since we could otherwise race with ext4_flush_completed_IO()
 */
int ext4_end_io_nolock(ext4_io_end_t *io)
{
	struct inode *inode = io->inode;
	loff_t offset = io->offset;
	ssize_t size = io->size;
	int ret = 0;

	ext4_debug("ext4_end_io_nolock: io 0x%p from inode %lu,list->next 0x%p,"
		   "list->prev 0x%p\n",
		   io, inode->i_ino, io->list.next, io->list.prev);

	ret = ext4_convert_unwritten_extents(inode, offset, size);
	if (ret < 0) {
		ext4_msg(inode->i_sb, KERN_EMERG,
			 "failed to convert unwritten extents to written "
			 "extents -- potential data loss!  "
			 "(inode %lu, offset %llu, size %zd, error %d)",
			 inode->i_ino, offset, size, ret);
	}

	/* Wake up anyone waiting on unwritten extent conversion */
	if (atomic_dec_and_test(&EXT4_I(inode)->i_aiodio_unwritten))
		wake_up_all(ext4_ioend_wq(io->inode));
	if (io->flag & EXT4_IO_END_DIRECT)
		inode_dio_done(inode);
	if (io->iocb)
		aio_complete(io->iocb, io->result, 0);
	return ret;
}

/*
 * work on completed aio dio IO, to convert unwritten extents to extents
 */
static void ext4_end_io_work(struct work_struct *work)
{
	ext4_io_end_t		*io = container_of(work, ext4_io_end_t, work);
	struct inode		*inode = io->inode;
	struct ext4_inode_info	*ei = EXT4_I(inode);
	unsigned long		flags;

	spin_lock_irqsave(&ei->i_completed_io_lock, flags);
	if (io->flag & EXT4_IO_END_IN_FSYNC)
		goto requeue;
	if (list_empty(&io->list)) {
		spin_unlock_irqrestore(&ei->i_completed_io_lock, flags);
		goto free;
	}

	if (!mutex_trylock(&inode->i_mutex)) {
		bool was_queued;
requeue:
		was_queued = !!(io->flag & EXT4_IO_END_QUEUED);
		io->flag |= EXT4_IO_END_QUEUED;
		spin_unlock_irqrestore(&ei->i_completed_io_lock, flags);
		/*
		 * Requeue the work instead of waiting so that the work
		 * items queued after this can be processed.
		 */
		queue_work(EXT4_SB(inode->i_sb)->dio_unwritten_wq, &io->work);
		/*
		 * To prevent the ext4-dio-unwritten thread from keeping
		 * requeueing end_io requests and occupying cpu for too long,
		 * yield the cpu if it sees an end_io request that has already
		 * been requeued.
		 */
		if (was_queued)
			yield();
		return;
	}
	list_del_init(&io->list);
	spin_unlock_irqrestore(&ei->i_completed_io_lock, flags);
	(void) ext4_end_io_nolock(io);
	mutex_unlock(&inode->i_mutex);
free:
	ext4_free_io_end(io);
}

ext4_io_end_t *ext4_init_io_end(struct inode *inode, gfp_t flags)
{
	ext4_io_end_t *io = kmem_cache_zalloc(io_end_cachep, flags);
	if (io) {
		atomic_inc(&EXT4_I(inode)->i_ioend_count);
		io->inode = inode;
		INIT_WORK(&io->work, ext4_end_io_work);
		INIT_LIST_HEAD(&io->list);
	}
	return io;
=======
	kmem_cache_destroy(io_end_vec_cachep);
}

struct ext4_io_end_vec *ext4_alloc_io_end_vec(ext4_io_end_t *io_end)
{
	struct ext4_io_end_vec *io_end_vec;

	io_end_vec = kmem_cache_zalloc(io_end_vec_cachep, GFP_NOFS);
	if (!io_end_vec)
		return ERR_PTR(-ENOMEM);
	INIT_LIST_HEAD(&io_end_vec->list);
	list_add_tail(&io_end_vec->list, &io_end->list_vec);
	return io_end_vec;
}

static void ext4_free_io_end_vec(ext4_io_end_t *io_end)
{
	struct ext4_io_end_vec *io_end_vec, *tmp;

	if (list_empty(&io_end->list_vec))
		return;
	list_for_each_entry_safe(io_end_vec, tmp, &io_end->list_vec, list) {
		list_del(&io_end_vec->list);
		kmem_cache_free(io_end_vec_cachep, io_end_vec);
	}
}

struct ext4_io_end_vec *ext4_last_io_end_vec(ext4_io_end_t *io_end)
{
	BUG_ON(list_empty(&io_end->list_vec));
	return list_last_entry(&io_end->list_vec, struct ext4_io_end_vec, list);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Print an buffer I/O error compatible with the fs/buffer.c.  This
 * provides compatibility with dmesg scrapers that look for a specific
 * buffer I/O error message.  We really need a unified error reporting
 * structure to userspace ala Digital Unix's uerf system, but it's
 * probably not going to happen in my lifetime, due to LKML politics...
 */
static void buffer_io_error(struct buffer_head *bh)
{
<<<<<<< HEAD
	char b[BDEVNAME_SIZE];
	printk(KERN_ERR "Buffer I/O error on device %s, logical block %llu\n",
			bdevname(bh->b_bdev, b),
			(unsigned long long)bh->b_blocknr);
}

static void ext4_end_bio(struct bio *bio, int error)
{
	ext4_io_end_t *io_end = bio->bi_private;
	struct workqueue_struct *wq;
	struct inode *inode;
	unsigned long flags;
	int i;
	sector_t bi_sector = bio->bi_sector;

	BUG_ON(!io_end);
	bio->bi_private = NULL;
	bio->bi_end_io = NULL;
	if (test_bit(BIO_UPTODATE, &bio->bi_flags))
		error = 0;
	bio_put(bio);

	for (i = 0; i < io_end->num_io_pages; i++) {
		struct page *page = io_end->pages[i]->p_page;
		struct buffer_head *bh, *head;
		loff_t offset;
		loff_t io_end_offset;

		if (error) {
			SetPageError(page);
			set_bit(AS_EIO, &page->mapping->flags);
			head = page_buffers(page);
			BUG_ON(!head);

			io_end_offset = io_end->offset + io_end->size;

			offset = (sector_t) page->index << PAGE_CACHE_SHIFT;
			bh = head;
			do {
				if ((offset >= io_end->offset) &&
				    (offset+bh->b_size <= io_end_offset))
					buffer_io_error(bh);

				offset += bh->b_size;
				bh = bh->b_this_page;
			} while (bh != head);
		}

		put_io_page(io_end->pages[i]);
	}
	io_end->num_io_pages = 0;
	inode = io_end->inode;

	if (error) {
		io_end->flag |= EXT4_IO_END_ERROR;
		ext4_warning(inode->i_sb, "I/O error writing to inode %lu "
			     "(offset %llu size %ld starting block %llu)",
			     inode->i_ino,
			     (unsigned long long) io_end->offset,
			     (long) io_end->size,
			     (unsigned long long)
			     bi_sector >> (inode->i_blkbits - 9));
	}

	if (!(io_end->flag & EXT4_IO_END_UNWRITTEN)) {
		ext4_free_io_end(io_end);
		return;
	}

	/* Add the io_end to per-inode completed io list*/
	spin_lock_irqsave(&EXT4_I(inode)->i_completed_io_lock, flags);
	list_add_tail(&io_end->list, &EXT4_I(inode)->i_completed_io_list);
	spin_unlock_irqrestore(&EXT4_I(inode)->i_completed_io_lock, flags);

	wq = EXT4_SB(inode->i_sb)->dio_unwritten_wq;
	/* queue the work to convert unwritten extents to written */
	queue_work(wq, &io_end->work);
=======
	printk_ratelimited(KERN_ERR "Buffer I/O error on device %pg, logical block %llu\n",
		       bh->b_bdev,
			(unsigned long long)bh->b_blocknr);
}

static void ext4_finish_bio(struct bio *bio)
{
	struct folio_iter fi;

	bio_for_each_folio_all(fi, bio) {
		struct folio *folio = fi.folio;
		struct folio *io_folio = NULL;
		struct buffer_head *bh, *head;
		size_t bio_start = fi.offset;
		size_t bio_end = bio_start + fi.length;
		unsigned under_io = 0;
		unsigned long flags;

		if (fscrypt_is_bounce_folio(folio)) {
			io_folio = folio;
			folio = fscrypt_pagecache_folio(folio);
		}

		if (bio->bi_status) {
			int err = blk_status_to_errno(bio->bi_status);
			folio_set_error(folio);
			mapping_set_error(folio->mapping, err);
		}
		bh = head = folio_buffers(folio);
		/*
		 * We check all buffers in the folio under b_uptodate_lock
		 * to avoid races with other end io clearing async_write flags
		 */
		spin_lock_irqsave(&head->b_uptodate_lock, flags);
		do {
			if (bh_offset(bh) < bio_start ||
			    bh_offset(bh) + bh->b_size > bio_end) {
				if (buffer_async_write(bh))
					under_io++;
				continue;
			}
			clear_buffer_async_write(bh);
			if (bio->bi_status) {
				set_buffer_write_io_error(bh);
				buffer_io_error(bh);
			}
		} while ((bh = bh->b_this_page) != head);
		spin_unlock_irqrestore(&head->b_uptodate_lock, flags);
		if (!under_io) {
			fscrypt_free_bounce_page(&io_folio->page);
			folio_end_writeback(folio);
		}
	}
}

static void ext4_release_io_end(ext4_io_end_t *io_end)
{
	struct bio *bio, *next_bio;

	BUG_ON(!list_empty(&io_end->list));
	BUG_ON(io_end->flag & EXT4_IO_END_UNWRITTEN);
	WARN_ON(io_end->handle);

	for (bio = io_end->bio; bio; bio = next_bio) {
		next_bio = bio->bi_private;
		ext4_finish_bio(bio);
		bio_put(bio);
	}
	ext4_free_io_end_vec(io_end);
	kmem_cache_free(io_end_cachep, io_end);
}

/*
 * Check a range of space and convert unwritten extents to written. Note that
 * we are protected from truncate touching same part of extent tree by the
 * fact that truncate code waits for all DIO to finish (thus exclusion from
 * direct IO is achieved) and also waits for PageWriteback bits. Thus we
 * cannot get to ext4_ext_truncate() before all IOs overlapping that range are
 * completed (happens from ext4_free_ioend()).
 */
static int ext4_end_io_end(ext4_io_end_t *io_end)
{
	struct inode *inode = io_end->inode;
	handle_t *handle = io_end->handle;
	int ret = 0;

	ext4_debug("ext4_end_io_nolock: io_end 0x%p from inode %lu,list->next 0x%p,"
		   "list->prev 0x%p\n",
		   io_end, inode->i_ino, io_end->list.next, io_end->list.prev);

	io_end->handle = NULL;	/* Following call will use up the handle */
	ret = ext4_convert_unwritten_io_end_vec(handle, io_end);
	if (ret < 0 && !ext4_forced_shutdown(inode->i_sb)) {
		ext4_msg(inode->i_sb, KERN_EMERG,
			 "failed to convert unwritten extents to written "
			 "extents -- potential data loss!  "
			 "(inode %lu, error %d)", inode->i_ino, ret);
	}
	ext4_clear_io_unwritten_flag(io_end);
	ext4_release_io_end(io_end);
	return ret;
}

static void dump_completed_IO(struct inode *inode, struct list_head *head)
{
#ifdef	EXT4FS_DEBUG
	struct list_head *cur, *before, *after;
	ext4_io_end_t *io_end, *io_end0, *io_end1;

	if (list_empty(head))
		return;

	ext4_debug("Dump inode %lu completed io list\n", inode->i_ino);
	list_for_each_entry(io_end, head, list) {
		cur = &io_end->list;
		before = cur->prev;
		io_end0 = container_of(before, ext4_io_end_t, list);
		after = cur->next;
		io_end1 = container_of(after, ext4_io_end_t, list);

		ext4_debug("io 0x%p from inode %lu,prev 0x%p,next 0x%p\n",
			    io_end, inode->i_ino, io_end0, io_end1);
	}
#endif
}

/* Add the io_end to per-inode completed end_io list. */
static void ext4_add_complete_io(ext4_io_end_t *io_end)
{
	struct ext4_inode_info *ei = EXT4_I(io_end->inode);
	struct ext4_sb_info *sbi = EXT4_SB(io_end->inode->i_sb);
	struct workqueue_struct *wq;
	unsigned long flags;

	/* Only reserved conversions from writeback should enter here */
	WARN_ON(!(io_end->flag & EXT4_IO_END_UNWRITTEN));
	WARN_ON(!io_end->handle && sbi->s_journal);
	spin_lock_irqsave(&ei->i_completed_io_lock, flags);
	wq = sbi->rsv_conversion_wq;
	if (list_empty(&ei->i_rsv_conversion_list))
		queue_work(wq, &ei->i_rsv_conversion_work);
	list_add_tail(&io_end->list, &ei->i_rsv_conversion_list);
	spin_unlock_irqrestore(&ei->i_completed_io_lock, flags);
}

static int ext4_do_flush_completed_IO(struct inode *inode,
				      struct list_head *head)
{
	ext4_io_end_t *io_end;
	struct list_head unwritten;
	unsigned long flags;
	struct ext4_inode_info *ei = EXT4_I(inode);
	int err, ret = 0;

	spin_lock_irqsave(&ei->i_completed_io_lock, flags);
	dump_completed_IO(inode, head);
	list_replace_init(head, &unwritten);
	spin_unlock_irqrestore(&ei->i_completed_io_lock, flags);

	while (!list_empty(&unwritten)) {
		io_end = list_entry(unwritten.next, ext4_io_end_t, list);
		BUG_ON(!(io_end->flag & EXT4_IO_END_UNWRITTEN));
		list_del_init(&io_end->list);

		err = ext4_end_io_end(io_end);
		if (unlikely(!ret && err))
			ret = err;
	}
	return ret;
}

/*
 * work on completed IO, to convert unwritten extents to extents
 */
void ext4_end_io_rsv_work(struct work_struct *work)
{
	struct ext4_inode_info *ei = container_of(work, struct ext4_inode_info,
						  i_rsv_conversion_work);
	ext4_do_flush_completed_IO(&ei->vfs_inode, &ei->i_rsv_conversion_list);
}

ext4_io_end_t *ext4_init_io_end(struct inode *inode, gfp_t flags)
{
	ext4_io_end_t *io_end = kmem_cache_zalloc(io_end_cachep, flags);

	if (io_end) {
		io_end->inode = inode;
		INIT_LIST_HEAD(&io_end->list);
		INIT_LIST_HEAD(&io_end->list_vec);
		refcount_set(&io_end->count, 1);
	}
	return io_end;
}

void ext4_put_io_end_defer(ext4_io_end_t *io_end)
{
	if (refcount_dec_and_test(&io_end->count)) {
		if (!(io_end->flag & EXT4_IO_END_UNWRITTEN) ||
				list_empty(&io_end->list_vec)) {
			ext4_release_io_end(io_end);
			return;
		}
		ext4_add_complete_io(io_end);
	}
}

int ext4_put_io_end(ext4_io_end_t *io_end)
{
	int err = 0;

	if (refcount_dec_and_test(&io_end->count)) {
		if (io_end->flag & EXT4_IO_END_UNWRITTEN) {
			err = ext4_convert_unwritten_io_end_vec(io_end->handle,
								io_end);
			io_end->handle = NULL;
			ext4_clear_io_unwritten_flag(io_end);
		}
		ext4_release_io_end(io_end);
	}
	return err;
}

ext4_io_end_t *ext4_get_io_end(ext4_io_end_t *io_end)
{
	refcount_inc(&io_end->count);
	return io_end;
}

/* BIO completion function for page writeback */
static void ext4_end_bio(struct bio *bio)
{
	ext4_io_end_t *io_end = bio->bi_private;
	sector_t bi_sector = bio->bi_iter.bi_sector;

	if (WARN_ONCE(!io_end, "io_end is NULL: %pg: sector %Lu len %u err %d\n",
		      bio->bi_bdev,
		      (long long) bio->bi_iter.bi_sector,
		      (unsigned) bio_sectors(bio),
		      bio->bi_status)) {
		ext4_finish_bio(bio);
		bio_put(bio);
		return;
	}
	bio->bi_end_io = NULL;

	if (bio->bi_status) {
		struct inode *inode = io_end->inode;

		ext4_warning(inode->i_sb, "I/O error %d writing to inode %lu "
			     "starting block %llu)",
			     bio->bi_status, inode->i_ino,
			     (unsigned long long)
			     bi_sector >> (inode->i_blkbits - 9));
		mapping_set_error(inode->i_mapping,
				blk_status_to_errno(bio->bi_status));
	}

	if (io_end->flag & EXT4_IO_END_UNWRITTEN) {
		/*
		 * Link bio into list hanging from io_end. We have to do it
		 * atomically as bio completions can be racing against each
		 * other.
		 */
		bio->bi_private = xchg(&io_end->bio, bio);
		ext4_put_io_end_defer(io_end);
	} else {
		/*
		 * Drop io_end reference early. Inode can get freed once
		 * we finish the bio.
		 */
		ext4_put_io_end_defer(io_end);
		ext4_finish_bio(bio);
		bio_put(bio);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void ext4_io_submit(struct ext4_io_submit *io)
{
	struct bio *bio = io->io_bio;

	if (bio) {
<<<<<<< HEAD
		bio_get(io->io_bio);
		submit_bio(io->io_op, io->io_bio);
		BUG_ON(bio_flagged(io->io_bio, BIO_EOPNOTSUPP));
		bio_put(io->io_bio);
	}
	io->io_bio = NULL;
	io->io_op = 0;
	io->io_end = NULL;
}

static int io_submit_init(struct ext4_io_submit *io,
			  struct inode *inode,
			  struct writeback_control *wbc,
			  struct buffer_head *bh)
{
	ext4_io_end_t *io_end;
	struct page *page = bh->b_page;
	int nvecs = bio_get_nr_vecs(bh->b_bdev);
	struct bio *bio;

	io_end = ext4_init_io_end(inode, GFP_NOFS);
	if (!io_end)
		return -ENOMEM;
	bio = bio_alloc(GFP_NOIO, min(nvecs, BIO_MAX_PAGES));
	bio->bi_sector = bh->b_blocknr * (bh->b_size >> 9);
	bio->bi_bdev = bh->b_bdev;
	bio->bi_private = io->io_end = io_end;
	bio->bi_end_io = ext4_end_bio;

	io_end->offset = (page->index << PAGE_CACHE_SHIFT) + bh_offset(bh);

	io->io_bio = bio;
	io->io_op = (wbc->sync_mode == WB_SYNC_ALL ?  WRITE_SYNC : WRITE);
	io->io_next_block = bh->b_blocknr;
	return 0;
}

static int io_submit_add_bh(struct ext4_io_submit *io,
			    struct ext4_io_page *io_page,
			    struct inode *inode,
			    struct writeback_control *wbc,
			    struct buffer_head *bh)
{
	ext4_io_end_t *io_end;
	int ret;

	if (buffer_new(bh)) {
		clear_buffer_new(bh);
		unmap_underlying_metadata(bh->b_bdev, bh->b_blocknr);
	}

	if (!buffer_mapped(bh) || buffer_delay(bh)) {
		if (!buffer_mapped(bh))
			clear_buffer_dirty(bh);
		if (io->io_bio)
			ext4_io_submit(io);
		return 0;
	}

	if (io->io_bio && bh->b_blocknr != io->io_next_block) {
submit_and_retry:
		ext4_io_submit(io);
	}
	if (io->io_bio == NULL) {
		ret = io_submit_init(io, inode, wbc, bh);
		if (ret)
			return ret;
	}
	io_end = io->io_end;
	if ((io_end->num_io_pages >= MAX_IO_PAGES) &&
	    (io_end->pages[io_end->num_io_pages-1] != io_page))
		goto submit_and_retry;
	if (buffer_uninit(bh))
		ext4_set_io_unwritten_flag(inode, io_end);
	io->io_end->size += bh->b_size;
	io->io_next_block++;
	ret = bio_add_page(io->io_bio, bh->b_page, bh->b_size, bh_offset(bh));
	if (ret != bh->b_size)
		goto submit_and_retry;
	if ((io_end->num_io_pages == 0) ||
	    (io_end->pages[io_end->num_io_pages-1] != io_page)) {
		io_end->pages[io_end->num_io_pages++] = io_page;
		atomic_inc(&io_page->p_count);
	}
	return 0;
}

int ext4_bio_write_page(struct ext4_io_submit *io,
			struct page *page,
			int len,
			struct writeback_control *wbc)
{
	struct inode *inode = page->mapping->host;
	unsigned block_start, block_end, blocksize;
	struct ext4_io_page *io_page;
	struct buffer_head *bh, *head;
	int ret = 0;

	blocksize = 1 << inode->i_blkbits;

	BUG_ON(!PageLocked(page));
	BUG_ON(PageWriteback(page));

	io_page = kmem_cache_alloc(io_page_cachep, GFP_NOFS);
	if (!io_page) {
		set_page_dirty(page);
		unlock_page(page);
		return -ENOMEM;
	}
	io_page->p_page = page;
	atomic_set(&io_page->p_count, 1);
	get_page(page);
	set_page_writeback(page);
	ClearPageError(page);

	for (bh = head = page_buffers(page), block_start = 0;
	     bh != head || !block_start;
	     block_start = block_end, bh = bh->b_this_page) {

		block_end = block_start + blocksize;
		if (block_start >= len) {
			/*
			 * Comments copied from block_write_full_page_endio:
			 *
			 * The page straddles i_size.  It must be zeroed out on
			 * each and every writepage invocation because it may
			 * be mmapped.  "A file is mapped in multiples of the
			 * page size.  For a file that is not a multiple of
			 * the  page size, the remaining memory is zeroed when
			 * mapped, and writes to that region are not written
			 * out to the file."
			 */
			zero_user_segment(page, block_start, block_end);
=======
		if (io->io_wbc->sync_mode == WB_SYNC_ALL)
			io->io_bio->bi_opf |= REQ_SYNC;
		submit_bio(io->io_bio);
	}
	io->io_bio = NULL;
}

void ext4_io_submit_init(struct ext4_io_submit *io,
			 struct writeback_control *wbc)
{
	io->io_wbc = wbc;
	io->io_bio = NULL;
	io->io_end = NULL;
}

static void io_submit_init_bio(struct ext4_io_submit *io,
			       struct buffer_head *bh)
{
	struct bio *bio;

	/*
	 * bio_alloc will _always_ be able to allocate a bio if
	 * __GFP_DIRECT_RECLAIM is set, see comments for bio_alloc_bioset().
	 */
	bio = bio_alloc(bh->b_bdev, BIO_MAX_VECS, REQ_OP_WRITE, GFP_NOIO);
	fscrypt_set_bio_crypt_ctx_bh(bio, bh, GFP_NOIO);
	bio->bi_iter.bi_sector = bh->b_blocknr * (bh->b_size >> 9);
	bio->bi_end_io = ext4_end_bio;
	bio->bi_private = ext4_get_io_end(io->io_end);
	io->io_bio = bio;
	io->io_next_block = bh->b_blocknr;
	wbc_init_bio(io->io_wbc, bio);
}

static void io_submit_add_bh(struct ext4_io_submit *io,
			     struct inode *inode,
			     struct folio *folio,
			     struct folio *io_folio,
			     struct buffer_head *bh)
{
	if (io->io_bio && (bh->b_blocknr != io->io_next_block ||
			   !fscrypt_mergeable_bio_bh(io->io_bio, bh))) {
submit_and_retry:
		ext4_io_submit(io);
	}
	if (io->io_bio == NULL)
		io_submit_init_bio(io, bh);
	if (!bio_add_folio(io->io_bio, io_folio, bh->b_size, bh_offset(bh)))
		goto submit_and_retry;
	wbc_account_cgroup_owner(io->io_wbc, &folio->page, bh->b_size);
	io->io_next_block++;
}

int ext4_bio_write_folio(struct ext4_io_submit *io, struct folio *folio,
		size_t len)
{
	struct folio *io_folio = folio;
	struct inode *inode = folio->mapping->host;
	unsigned block_start;
	struct buffer_head *bh, *head;
	int ret = 0;
	int nr_to_submit = 0;
	struct writeback_control *wbc = io->io_wbc;
	bool keep_towrite = false;

	BUG_ON(!folio_test_locked(folio));
	BUG_ON(folio_test_writeback(folio));

	folio_clear_error(folio);

	/*
	 * Comments copied from block_write_full_folio:
	 *
	 * The folio straddles i_size.  It must be zeroed out on each and every
	 * writepage invocation because it may be mmapped.  "A file is mapped
	 * in multiples of the page size.  For a file that is not a multiple of
	 * the page size, the remaining memory is zeroed when mapped, and
	 * writes to that region are not written out to the file."
	 */
	if (len < folio_size(folio))
		folio_zero_segment(folio, len, folio_size(folio));
	/*
	 * In the first loop we prepare and mark buffers to submit. We have to
	 * mark all buffers in the folio before submitting so that
	 * folio_end_writeback() cannot be called from ext4_end_bio() when IO
	 * on the first buffer finishes and we are still working on submitting
	 * the second buffer.
	 */
	bh = head = folio_buffers(folio);
	do {
		block_start = bh_offset(bh);
		if (block_start >= len) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			clear_buffer_dirty(bh);
			set_buffer_uptodate(bh);
			continue;
		}
<<<<<<< HEAD
		clear_buffer_dirty(bh);
		ret = io_submit_add_bh(io, io_page, inode, wbc, bh);
		if (ret) {
			/*
			 * We only get here on ENOMEM.  Not much else
			 * we can do but mark the page as dirty, and
			 * better luck next time.
			 */
			set_page_dirty(page);
			break;
		}
	}
	unlock_page(page);
	/*
	 * If the page was truncated before we could do the writeback,
	 * or we had a memory allocation error while trying to write
	 * the first buffer head, we won't have submitted any pages for
	 * I/O.  In that case we need to make sure we've cleared the
	 * PageWriteback bit from the page to prevent the system from
	 * wedging later on.
	 */
	put_io_page(io_page);
	return ret;
=======
		if (!buffer_dirty(bh) || buffer_delay(bh) ||
		    !buffer_mapped(bh) || buffer_unwritten(bh)) {
			/* A hole? We can safely clear the dirty bit */
			if (!buffer_mapped(bh))
				clear_buffer_dirty(bh);
			/*
			 * Keeping dirty some buffer we cannot write? Make sure
			 * to redirty the folio and keep TOWRITE tag so that
			 * racing WB_SYNC_ALL writeback does not skip the folio.
			 * This happens e.g. when doing writeout for
			 * transaction commit or when journalled data is not
			 * yet committed.
			 */
			if (buffer_dirty(bh) ||
			    (buffer_jbd(bh) && buffer_jbddirty(bh))) {
				if (!folio_test_dirty(folio))
					folio_redirty_for_writepage(wbc, folio);
				keep_towrite = true;
			}
			continue;
		}
		if (buffer_new(bh))
			clear_buffer_new(bh);
		set_buffer_async_write(bh);
		clear_buffer_dirty(bh);
		nr_to_submit++;
	} while ((bh = bh->b_this_page) != head);

	/* Nothing to submit? Just unlock the folio... */
	if (!nr_to_submit)
		return 0;

	bh = head = folio_buffers(folio);

	/*
	 * If any blocks are being written to an encrypted file, encrypt them
	 * into a bounce page.  For simplicity, just encrypt until the last
	 * block which might be needed.  This may cause some unneeded blocks
	 * (e.g. holes) to be unnecessarily encrypted, but this is rare and
	 * can't happen in the common case of blocksize == PAGE_SIZE.
	 */
	if (fscrypt_inode_uses_fs_layer_crypto(inode)) {
		gfp_t gfp_flags = GFP_NOFS;
		unsigned int enc_bytes = round_up(len, i_blocksize(inode));
		struct page *bounce_page;

		/*
		 * Since bounce page allocation uses a mempool, we can only use
		 * a waiting mask (i.e. request guaranteed allocation) on the
		 * first page of the bio.  Otherwise it can deadlock.
		 */
		if (io->io_bio)
			gfp_flags = GFP_NOWAIT | __GFP_NOWARN;
	retry_encrypt:
		bounce_page = fscrypt_encrypt_pagecache_blocks(&folio->page,
					enc_bytes, 0, gfp_flags);
		if (IS_ERR(bounce_page)) {
			ret = PTR_ERR(bounce_page);
			if (ret == -ENOMEM &&
			    (io->io_bio || wbc->sync_mode == WB_SYNC_ALL)) {
				gfp_t new_gfp_flags = GFP_NOFS;
				if (io->io_bio)
					ext4_io_submit(io);
				else
					new_gfp_flags |= __GFP_NOFAIL;
				memalloc_retry_wait(gfp_flags);
				gfp_flags = new_gfp_flags;
				goto retry_encrypt;
			}

			printk_ratelimited(KERN_ERR "%s: ret = %d\n", __func__, ret);
			folio_redirty_for_writepage(wbc, folio);
			do {
				if (buffer_async_write(bh)) {
					clear_buffer_async_write(bh);
					set_buffer_dirty(bh);
				}
				bh = bh->b_this_page;
			} while (bh != head);

			return ret;
		}
		io_folio = page_folio(bounce_page);
	}

	__folio_start_writeback(folio, keep_towrite);

	/* Now submit buffers to write */
	do {
		if (!buffer_async_write(bh))
			continue;
		io_submit_add_bh(io, inode, folio, io_folio, bh);
	} while ((bh = bh->b_this_page) != head);

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
