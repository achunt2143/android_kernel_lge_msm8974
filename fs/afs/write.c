<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* handling of writes to regular files and writing back to the server
 *
 * Copyright (C) 2007 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */
=======
 */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/backing-dev.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/pagemap.h>
#include <linux/writeback.h>
#include <linux/pagevec.h>
<<<<<<< HEAD
#include "internal.h"

static int afs_write_back_from_locked_page(struct afs_writeback *wb,
					   struct page *page);

/*
 * mark a page as having been made dirty and thus needing writeback
 */
int afs_set_page_dirty(struct page *page)
{
	_enter("");
	return __set_page_dirty_nobuffers(page);
}

/*
 * unlink a writeback record because its usage has reached zero
 * - must be called with the wb->vnode->writeback_lock held
 */
static void afs_unlink_writeback(struct afs_writeback *wb)
{
	struct afs_writeback *front;
	struct afs_vnode *vnode = wb->vnode;

	list_del_init(&wb->link);
	if (!list_empty(&vnode->writebacks)) {
		/* if an fsync rises to the front of the queue then wake it
		 * up */
		front = list_entry(vnode->writebacks.next,
				   struct afs_writeback, link);
		if (front->state == AFS_WBACK_SYNCING) {
			_debug("wake up sync");
			front->state = AFS_WBACK_COMPLETE;
			wake_up(&front->waitq);
		}
	}
}

/*
 * free a writeback record
 */
static void afs_free_writeback(struct afs_writeback *wb)
{
	_enter("");
	key_put(wb->key);
	kfree(wb);
}

/*
 * dispose of a reference to a writeback record
 */
void afs_put_writeback(struct afs_writeback *wb)
{
	struct afs_vnode *vnode = wb->vnode;

	_enter("{%d}", wb->usage);

	spin_lock(&vnode->writeback_lock);
	if (--wb->usage == 0)
		afs_unlink_writeback(wb);
	else
		wb = NULL;
	spin_unlock(&vnode->writeback_lock);
	if (wb)
		afs_free_writeback(wb);
}

/*
 * partly or wholly fill a page that's under preparation for writing
 */
static int afs_fill_page(struct afs_vnode *vnode, struct key *key,
			 loff_t pos, struct page *page)
{
	loff_t i_size;
	int ret;
	int len;

	_enter(",,%llu", (unsigned long long)pos);

	i_size = i_size_read(&vnode->vfs_inode);
	if (pos + PAGE_CACHE_SIZE > i_size)
		len = i_size - pos;
	else
		len = PAGE_CACHE_SIZE;

	ret = afs_vnode_fetch_data(vnode, key, pos, len, page);
	if (ret < 0) {
		if (ret == -ENOENT) {
			_debug("got NOENT from server"
			       " - marking file deleted and stale");
			set_bit(AFS_VNODE_DELETED, &vnode->flags);
			ret = -ESTALE;
		}
	}

	_leave(" = %d", ret);
	return ret;
}

/*
 * prepare to perform part of a write to a page
 */
int afs_write_begin(struct file *file, struct address_space *mapping,
		    loff_t pos, unsigned len, unsigned flags,
		    struct page **pagep, void **fsdata)
{
	struct afs_writeback *candidate, *wb;
	struct afs_vnode *vnode = AFS_FS_I(file->f_dentry->d_inode);
	struct page *page;
	struct key *key = file->private_data;
	unsigned from = pos & (PAGE_CACHE_SIZE - 1);
	unsigned to = from + len;
	pgoff_t index = pos >> PAGE_CACHE_SHIFT;
	int ret;

	_enter("{%x:%u},{%lx},%u,%u",
	       vnode->fid.vid, vnode->fid.vnode, index, from, to);

	candidate = kzalloc(sizeof(*candidate), GFP_KERNEL);
	if (!candidate)
		return -ENOMEM;
	candidate->vnode = vnode;
	candidate->first = candidate->last = index;
	candidate->offset_first = from;
	candidate->to_last = to;
	INIT_LIST_HEAD(&candidate->link);
	candidate->usage = 1;
	candidate->state = AFS_WBACK_PENDING;
	init_waitqueue_head(&candidate->waitq);

	page = grab_cache_page_write_begin(mapping, index, flags);
	if (!page) {
		kfree(candidate);
		return -ENOMEM;
	}
	*pagep = page;
	/* page won't leak in error case: it eventually gets cleaned off LRU */

	if (!PageUptodate(page) && len != PAGE_CACHE_SIZE) {
		ret = afs_fill_page(vnode, key, index << PAGE_CACHE_SHIFT, page);
		if (ret < 0) {
			kfree(candidate);
			_leave(" = %d [prep]", ret);
			return ret;
		}
		SetPageUptodate(page);
	}

try_again:
	spin_lock(&vnode->writeback_lock);

	/* see if this page is already pending a writeback under a suitable key
	 * - if so we can just join onto that one */
	wb = (struct afs_writeback *) page_private(page);
	if (wb) {
		if (wb->key == key && wb->state == AFS_WBACK_PENDING)
			goto subsume_in_current_wb;
		goto flush_conflicting_wb;
	}

	if (index > 0) {
		/* see if we can find an already pending writeback that we can
		 * append this page to */
		list_for_each_entry(wb, &vnode->writebacks, link) {
			if (wb->last == index - 1 && wb->key == key &&
			    wb->state == AFS_WBACK_PENDING)
				goto append_to_previous_wb;
		}
	}

	list_add_tail(&candidate->link, &vnode->writebacks);
	candidate->key = key_get(key);
	spin_unlock(&vnode->writeback_lock);
	SetPagePrivate(page);
	set_page_private(page, (unsigned long) candidate);
	_leave(" = 0 [new]");
	return 0;

subsume_in_current_wb:
	_debug("subsume");
	ASSERTRANGE(wb->first, <=, index, <=, wb->last);
	if (index == wb->first && from < wb->offset_first)
		wb->offset_first = from;
	if (index == wb->last && to > wb->to_last)
		wb->to_last = to;
	spin_unlock(&vnode->writeback_lock);
	kfree(candidate);
	_leave(" = 0 [sub]");
	return 0;

append_to_previous_wb:
	_debug("append into %lx-%lx", wb->first, wb->last);
	wb->usage++;
	wb->last++;
	wb->to_last = to;
	spin_unlock(&vnode->writeback_lock);
	SetPagePrivate(page);
	set_page_private(page, (unsigned long) wb);
	kfree(candidate);
	_leave(" = 0 [app]");
	return 0;

	/* the page is currently bound to another context, so if it's dirty we
	 * need to flush it before we can use the new context */
flush_conflicting_wb:
	_debug("flush conflict");
	if (wb->state == AFS_WBACK_PENDING)
		wb->state = AFS_WBACK_CONFLICTING;
	spin_unlock(&vnode->writeback_lock);
	if (PageDirty(page)) {
		ret = afs_write_back_from_locked_page(wb, page);
		if (ret < 0) {
			afs_put_writeback(candidate);
			_leave(" = %d", ret);
			return ret;
		}
	}

	/* the page holds a ref on the writeback record */
	afs_put_writeback(wb);
	set_page_private(page, 0);
	ClearPagePrivate(page);
	goto try_again;
}

/*
 * finalise part of a write to a page
 */
int afs_write_end(struct file *file, struct address_space *mapping,
		  loff_t pos, unsigned len, unsigned copied,
		  struct page *page, void *fsdata)
{
	struct afs_vnode *vnode = AFS_FS_I(file->f_dentry->d_inode);
	loff_t i_size, maybe_i_size;

	_enter("{%x:%u},{%lx}",
	       vnode->fid.vid, vnode->fid.vnode, page->index);

	maybe_i_size = pos + copied;

	i_size = i_size_read(&vnode->vfs_inode);
	if (maybe_i_size > i_size) {
		spin_lock(&vnode->writeback_lock);
		i_size = i_size_read(&vnode->vfs_inode);
		if (maybe_i_size > i_size)
			i_size_write(&vnode->vfs_inode, maybe_i_size);
		spin_unlock(&vnode->writeback_lock);
	}

	set_page_dirty(page);
	if (PageDirty(page))
		_debug("dirtied");
	unlock_page(page);
	page_cache_release(page);

	return copied;
}

/*
 * kill all the pages in the given range
 */
static void afs_kill_pages(struct afs_vnode *vnode, bool error,
			   pgoff_t first, pgoff_t last)
{
	struct pagevec pv;
	unsigned count, loop;

	_enter("{%x:%u},%lx-%lx",
	       vnode->fid.vid, vnode->fid.vnode, first, last);

	pagevec_init(&pv, 0);

	do {
		_debug("kill %lx-%lx", first, last);

		count = last - first + 1;
		if (count > PAGEVEC_SIZE)
			count = PAGEVEC_SIZE;
		pv.nr = find_get_pages_contig(vnode->vfs_inode.i_mapping,
					      first, count, pv.pages);
		ASSERTCMP(pv.nr, ==, count);

		for (loop = 0; loop < count; loop++) {
			ClearPageUptodate(pv.pages[loop]);
			if (error)
				SetPageError(pv.pages[loop]);
			end_page_writeback(pv.pages[loop]);
		}

		__pagevec_release(&pv);
	} while (first < last);

=======
#include <linux/netfs.h>
#include <trace/events/netfs.h>
#include "internal.h"

/*
 * completion of write to server
 */
static void afs_pages_written_back(struct afs_vnode *vnode, loff_t start, unsigned int len)
{
	_enter("{%llx:%llu},{%x @%llx}",
	       vnode->fid.vid, vnode->fid.vnode, len, start);

	afs_prune_wb_keys(vnode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	_leave("");
}

/*
<<<<<<< HEAD
 * synchronously write back the locked page and any subsequent non-locked dirty
 * pages also covered by the same writeback record
 */
static int afs_write_back_from_locked_page(struct afs_writeback *wb,
					   struct page *primary_page)
{
	struct page *pages[8], *page;
	unsigned long count;
	unsigned n, offset, to;
	pgoff_t start, first, last;
	int loop, ret;

	_enter(",%lx", primary_page->index);

	count = 1;
	if (!clear_page_dirty_for_io(primary_page))
		BUG();
	if (test_set_page_writeback(primary_page))
		BUG();

	/* find all consecutive lockable dirty pages, stopping when we find a
	 * page that is not immediately lockable, is not dirty or is missing,
	 * or we reach the end of the range */
	start = primary_page->index;
	if (start >= wb->last)
		goto no_more;
	start++;
	do {
		_debug("more %lx [%lx]", start, count);
		n = wb->last - start + 1;
		if (n > ARRAY_SIZE(pages))
			n = ARRAY_SIZE(pages);
		n = find_get_pages_contig(wb->vnode->vfs_inode.i_mapping,
					  start, n, pages);
		_debug("fgpc %u", n);
		if (n == 0)
			goto no_more;
		if (pages[0]->index != start) {
			do {
				put_page(pages[--n]);
			} while (n > 0);
			goto no_more;
		}

		for (loop = 0; loop < n; loop++) {
			page = pages[loop];
			if (page->index > wb->last)
				break;
			if (!trylock_page(page))
				break;
			if (!PageDirty(page) ||
			    page_private(page) != (unsigned long) wb) {
				unlock_page(page);
				break;
			}
			if (!clear_page_dirty_for_io(page))
				BUG();
			if (test_set_page_writeback(page))
				BUG();
			unlock_page(page);
			put_page(page);
		}
		count += loop;
		if (loop < n) {
			for (; loop < n; loop++)
				put_page(pages[loop]);
			goto no_more;
		}

		start += loop;
	} while (start <= wb->last && count < 65536);

no_more:
	/* we now have a contiguous set of dirty pages, each with writeback set
	 * and the dirty mark cleared; the first page is locked and must remain
	 * so, all the rest are unlocked */
	first = primary_page->index;
	last = first + count - 1;

	offset = (first == wb->first) ? wb->offset_first : 0;
	to = (last == wb->last) ? wb->to_last : PAGE_SIZE;

	_debug("write back %lx[%u..] to %lx[..%u]", first, offset, last, to);

	ret = afs_vnode_store_data(wb, first, last, offset, to);
	if (ret < 0) {
		switch (ret) {
		case -EDQUOT:
		case -ENOSPC:
			set_bit(AS_ENOSPC,
				&wb->vnode->vfs_inode.i_mapping->flags);
			break;
		case -EROFS:
		case -EIO:
		case -EREMOTEIO:
		case -EFBIG:
		case -ENOENT:
		case -ENOMEDIUM:
		case -ENXIO:
			afs_kill_pages(wb->vnode, true, first, last);
			set_bit(AS_EIO, &wb->vnode->vfs_inode.i_mapping->flags);
			break;
		case -EACCES:
		case -EPERM:
		case -ENOKEY:
		case -EKEYEXPIRED:
		case -EKEYREJECTED:
		case -EKEYREVOKED:
			afs_kill_pages(wb->vnode, false, first, last);
			break;
		default:
			break;
		}
	} else {
		ret = count;
	}

	_leave(" = %d", ret);
	return ret;
}

/*
 * write a page back to the server
 * - the caller locked the page for us
 */
int afs_writepage(struct page *page, struct writeback_control *wbc)
{
	struct afs_writeback *wb;
	int ret;

	_enter("{%lx},", page->index);

	wb = (struct afs_writeback *) page_private(page);
	ASSERT(wb != NULL);

	ret = afs_write_back_from_locked_page(wb, page);
	unlock_page(page);
	if (ret < 0) {
		_leave(" = %d", ret);
		return 0;
	}

	wbc->nr_to_write -= ret;

	_leave(" = 0");
	return 0;
}

/*
 * write a region of pages back to the server
 */
static int afs_writepages_region(struct address_space *mapping,
				 struct writeback_control *wbc,
				 pgoff_t index, pgoff_t end, pgoff_t *_next)
{
	struct afs_writeback *wb;
	struct page *page;
	int ret, n;

	_enter(",,%lx,%lx,", index, end);

	do {
		n = find_get_pages_tag(mapping, &index, PAGECACHE_TAG_DIRTY,
				       1, &page);
		if (!n)
			break;

		_debug("wback %lx", page->index);

		if (page->index > end) {
			*_next = index;
			page_cache_release(page);
			_leave(" = 0 [%lx]", *_next);
			return 0;
		}

		/* at this point we hold neither mapping->tree_lock nor lock on
		 * the page itself: the page may be truncated or invalidated
		 * (changing page->mapping to NULL), or even swizzled back from
		 * swapper_space to tmpfs file mapping
		 */
		lock_page(page);

		if (page->mapping != mapping) {
			unlock_page(page);
			page_cache_release(page);
			continue;
		}

		if (wbc->sync_mode != WB_SYNC_NONE)
			wait_on_page_writeback(page);

		if (PageWriteback(page) || !PageDirty(page)) {
			unlock_page(page);
			continue;
		}

		wb = (struct afs_writeback *) page_private(page);
		ASSERT(wb != NULL);

		spin_lock(&wb->vnode->writeback_lock);
		wb->state = AFS_WBACK_WRITING;
		spin_unlock(&wb->vnode->writeback_lock);

		ret = afs_write_back_from_locked_page(wb, page);
		unlock_page(page);
		page_cache_release(page);
		if (ret < 0) {
			_leave(" = %d", ret);
			return ret;
		}

		wbc->nr_to_write -= ret;

		cond_resched();
	} while (index < end && wbc->nr_to_write > 0);

	*_next = index;
	_leave(" = 0 [%lx]", *_next);
	return 0;
=======
 * Find a key to use for the writeback.  We cached the keys used to author the
 * writes on the vnode.  *_wbk will contain the last writeback key used or NULL
 * and we need to start from there if it's set.
 */
static int afs_get_writeback_key(struct afs_vnode *vnode,
				 struct afs_wb_key **_wbk)
{
	struct afs_wb_key *wbk = NULL;
	struct list_head *p;
	int ret = -ENOKEY, ret2;

	spin_lock(&vnode->wb_lock);
	if (*_wbk)
		p = (*_wbk)->vnode_link.next;
	else
		p = vnode->wb_keys.next;

	while (p != &vnode->wb_keys) {
		wbk = list_entry(p, struct afs_wb_key, vnode_link);
		_debug("wbk %u", key_serial(wbk->key));
		ret2 = key_validate(wbk->key);
		if (ret2 == 0) {
			refcount_inc(&wbk->usage);
			_debug("USE WB KEY %u", key_serial(wbk->key));
			break;
		}

		wbk = NULL;
		if (ret == -ENOKEY)
			ret = ret2;
		p = p->next;
	}

	spin_unlock(&vnode->wb_lock);
	if (*_wbk)
		afs_put_wb_key(*_wbk);
	*_wbk = wbk;
	return 0;
}

static void afs_store_data_success(struct afs_operation *op)
{
	struct afs_vnode *vnode = op->file[0].vnode;

	op->ctime = op->file[0].scb.status.mtime_client;
	afs_vnode_commit_status(op, &op->file[0]);
	if (!afs_op_error(op)) {
		if (!op->store.laundering)
			afs_pages_written_back(vnode, op->store.pos, op->store.size);
		afs_stat_v(vnode, n_stores);
		atomic_long_add(op->store.size, &afs_v2net(vnode)->n_store_bytes);
	}
}

static const struct afs_operation_ops afs_store_data_operation = {
	.issue_afs_rpc	= afs_fs_store_data,
	.issue_yfs_rpc	= yfs_fs_store_data,
	.success	= afs_store_data_success,
};

/*
 * write to a file
 */
static int afs_store_data(struct afs_vnode *vnode, struct iov_iter *iter, loff_t pos,
			  bool laundering)
{
	struct afs_operation *op;
	struct afs_wb_key *wbk = NULL;
	loff_t size = iov_iter_count(iter);
	int ret = -ENOKEY;

	_enter("%s{%llx:%llu.%u},%llx,%llx",
	       vnode->volume->name,
	       vnode->fid.vid,
	       vnode->fid.vnode,
	       vnode->fid.unique,
	       size, pos);

	ret = afs_get_writeback_key(vnode, &wbk);
	if (ret) {
		_leave(" = %d [no keys]", ret);
		return ret;
	}

	op = afs_alloc_operation(wbk->key, vnode->volume);
	if (IS_ERR(op)) {
		afs_put_wb_key(wbk);
		return -ENOMEM;
	}

	afs_op_set_vnode(op, 0, vnode);
	op->file[0].dv_delta = 1;
	op->file[0].modification = true;
	op->store.pos = pos;
	op->store.size = size;
	op->store.laundering = laundering;
	op->flags |= AFS_OPERATION_UNINTR;
	op->ops = &afs_store_data_operation;

try_next_key:
	afs_begin_vnode_operation(op);

	op->store.write_iter = iter;
	op->store.i_size = max(pos + size, vnode->netfs.remote_i_size);
	op->mtime = inode_get_mtime(&vnode->netfs.inode);

	afs_wait_for_operation(op);

	switch (afs_op_error(op)) {
	case -EACCES:
	case -EPERM:
	case -ENOKEY:
	case -EKEYEXPIRED:
	case -EKEYREJECTED:
	case -EKEYREVOKED:
		_debug("next");

		ret = afs_get_writeback_key(vnode, &wbk);
		if (ret == 0) {
			key_put(op->key);
			op->key = key_get(wbk->key);
			goto try_next_key;
		}
		break;
	}

	afs_put_wb_key(wbk);
	_leave(" = %d", afs_op_error(op));
	return afs_put_operation(op);
}

static void afs_upload_to_server(struct netfs_io_subrequest *subreq)
{
	struct afs_vnode *vnode = AFS_FS_I(subreq->rreq->inode);
	ssize_t ret;

	_enter("%x[%x],%zx",
	       subreq->rreq->debug_id, subreq->debug_index, subreq->io_iter.count);

	trace_netfs_sreq(subreq, netfs_sreq_trace_submit);
	ret = afs_store_data(vnode, &subreq->io_iter, subreq->start,
			     subreq->rreq->origin == NETFS_LAUNDER_WRITE);
	netfs_write_subrequest_terminated(subreq, ret < 0 ? ret : subreq->len,
					  false);
}

static void afs_upload_to_server_worker(struct work_struct *work)
{
	struct netfs_io_subrequest *subreq =
		container_of(work, struct netfs_io_subrequest, work);

	afs_upload_to_server(subreq);
}

/*
 * Set up write requests for a writeback slice.  We need to add a write request
 * for each write we want to make.
 */
void afs_create_write_requests(struct netfs_io_request *wreq, loff_t start, size_t len)
{
	struct netfs_io_subrequest *subreq;

	_enter("%x,%llx-%llx", wreq->debug_id, start, start + len);

	subreq = netfs_create_write_request(wreq, NETFS_UPLOAD_TO_SERVER,
					    start, len, afs_upload_to_server_worker);
	if (subreq)
		netfs_queue_write_request(subreq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * write some of the pending data back to the server
 */
<<<<<<< HEAD
int afs_writepages(struct address_space *mapping,
		   struct writeback_control *wbc)
{
	pgoff_t start, end, next;
	int ret;

	_enter("");

	if (wbc->range_cyclic) {
		start = mapping->writeback_index;
		end = -1;
		ret = afs_writepages_region(mapping, wbc, start, end, &next);
		if (start > 0 && wbc->nr_to_write > 0 && ret == 0)
			ret = afs_writepages_region(mapping, wbc, 0, start,
						    &next);
		mapping->writeback_index = next;
	} else if (wbc->range_start == 0 && wbc->range_end == LLONG_MAX) {
		end = (pgoff_t)(LLONG_MAX >> PAGE_CACHE_SHIFT);
		ret = afs_writepages_region(mapping, wbc, 0, end, &next);
		if (wbc->nr_to_write > 0)
			mapping->writeback_index = next;
	} else {
		start = wbc->range_start >> PAGE_CACHE_SHIFT;
		end = wbc->range_end >> PAGE_CACHE_SHIFT;
		ret = afs_writepages_region(mapping, wbc, start, end, &next);
	}

	_leave(" = %d", ret);
	return ret;
}

/*
 * completion of write to server
 */
void afs_pages_written_back(struct afs_vnode *vnode, struct afs_call *call)
{
	struct afs_writeback *wb = call->wb;
	struct pagevec pv;
	unsigned count, loop;
	pgoff_t first = call->first, last = call->last;
	bool free_wb;

	_enter("{%x:%u},{%lx-%lx}",
	       vnode->fid.vid, vnode->fid.vnode, first, last);

	ASSERT(wb != NULL);

	pagevec_init(&pv, 0);

	do {
		_debug("done %lx-%lx", first, last);

		count = last - first + 1;
		if (count > PAGEVEC_SIZE)
			count = PAGEVEC_SIZE;
		pv.nr = find_get_pages_contig(call->mapping, first, count,
					      pv.pages);
		ASSERTCMP(pv.nr, ==, count);

		spin_lock(&vnode->writeback_lock);
		for (loop = 0; loop < count; loop++) {
			struct page *page = pv.pages[loop];
			end_page_writeback(page);
			if (page_private(page) == (unsigned long) wb) {
				set_page_private(page, 0);
				ClearPagePrivate(page);
				wb->usage--;
			}
		}
		free_wb = false;
		if (wb->usage == 0) {
			afs_unlink_writeback(wb);
			free_wb = true;
		}
		spin_unlock(&vnode->writeback_lock);
		first += count;
		if (free_wb) {
			afs_free_writeback(wb);
			wb = NULL;
		}

		__pagevec_release(&pv);
	} while (first <= last);

	_leave("");
}

/*
 * write to an AFS file
 */
ssize_t afs_file_write(struct kiocb *iocb, const struct iovec *iov,
		       unsigned long nr_segs, loff_t pos)
{
	struct dentry *dentry = iocb->ki_filp->f_path.dentry;
	struct afs_vnode *vnode = AFS_FS_I(dentry->d_inode);
	ssize_t result;
	size_t count = iov_length(iov, nr_segs);

	_enter("{%x.%u},{%zu},%lu,",
	       vnode->fid.vid, vnode->fid.vnode, count, nr_segs);

	if (IS_SWAPFILE(&vnode->vfs_inode)) {
		printk(KERN_INFO
		       "AFS: Attempt to write to active swap file!\n");
		return -EBUSY;
	}

	if (!count)
		return 0;

	result = generic_file_aio_write(iocb, iov, nr_segs, pos);
	if (IS_ERR_VALUE(result)) {
		_leave(" = %zd", result);
		return result;
	}

	_leave(" = %zd", result);
	return result;
}

/*
 * flush the vnode to the fileserver
 */
int afs_writeback_all(struct afs_vnode *vnode)
{
	struct address_space *mapping = vnode->vfs_inode.i_mapping;
	struct writeback_control wbc = {
		.sync_mode	= WB_SYNC_ALL,
		.nr_to_write	= LONG_MAX,
		.range_cyclic	= 1,
	};
	int ret;

	_enter("");

	ret = mapping->a_ops->writepages(mapping, &wbc);
	__mark_inode_dirty(mapping->host, I_DIRTY_PAGES);

	_leave(" = %d", ret);
=======
int afs_writepages(struct address_space *mapping, struct writeback_control *wbc)
{
	struct afs_vnode *vnode = AFS_FS_I(mapping->host);
	int ret;

	/* We have to be careful as we can end up racing with setattr()
	 * truncating the pagecache since the caller doesn't take a lock here
	 * to prevent it.
	 */
	if (wbc->sync_mode == WB_SYNC_ALL)
		down_read(&vnode->validate_lock);
	else if (!down_read_trylock(&vnode->validate_lock))
		return 0;

	ret = netfs_writepages(mapping, wbc);
	up_read(&vnode->validate_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

/*
 * flush any dirty pages for this process, and check for write errors.
 * - the return status from this call provides a reliable indication of
 *   whether any write errors occurred for this process.
 */
int afs_fsync(struct file *file, loff_t start, loff_t end, int datasync)
{
<<<<<<< HEAD
	struct dentry *dentry = file->f_path.dentry;
	struct inode *inode = file->f_mapping->host;
	struct afs_writeback *wb, *xwb;
	struct afs_vnode *vnode = AFS_FS_I(dentry->d_inode);
	int ret;

	_enter("{%x:%u},{n=%s},%d",
	       vnode->fid.vid, vnode->fid.vnode, dentry->d_name.name,
	       datasync);

	ret = filemap_write_and_wait_range(inode->i_mapping, start, end);
	if (ret)
		return ret;
	mutex_lock(&inode->i_mutex);

	/* use a writeback record as a marker in the queue - when this reaches
	 * the front of the queue, all the outstanding writes are either
	 * completed or rejected */
	wb = kzalloc(sizeof(*wb), GFP_KERNEL);
	if (!wb) {
		ret = -ENOMEM;
		goto out;
	}
	wb->vnode = vnode;
	wb->first = 0;
	wb->last = -1;
	wb->offset_first = 0;
	wb->to_last = PAGE_SIZE;
	wb->usage = 1;
	wb->state = AFS_WBACK_SYNCING;
	init_waitqueue_head(&wb->waitq);

	spin_lock(&vnode->writeback_lock);
	list_for_each_entry(xwb, &vnode->writebacks, link) {
		if (xwb->state == AFS_WBACK_PENDING)
			xwb->state = AFS_WBACK_CONFLICTING;
	}
	list_add_tail(&wb->link, &vnode->writebacks);
	spin_unlock(&vnode->writeback_lock);

	/* push all the outstanding writebacks to the server */
	ret = afs_writeback_all(vnode);
	if (ret < 0) {
		afs_put_writeback(wb);
		_leave(" = %d [wb]", ret);
		goto out;
	}

	/* wait for the preceding writes to actually complete */
	ret = wait_event_interruptible(wb->waitq,
				       wb->state == AFS_WBACK_COMPLETE ||
				       vnode->writebacks.next == &wb->link);
	afs_put_writeback(wb);
	_leave(" = %d", ret);
out:
	mutex_unlock(&inode->i_mutex);
	return ret;
=======
	struct afs_vnode *vnode = AFS_FS_I(file_inode(file));
	struct afs_file *af = file->private_data;
	int ret;

	_enter("{%llx:%llu},{n=%pD},%d",
	       vnode->fid.vid, vnode->fid.vnode, file,
	       datasync);

	ret = afs_validate(vnode, af->key);
	if (ret < 0)
		return ret;

	return file_write_and_wait_range(file, start, end);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * notification that a previously read-only page is about to become writable
 * - if it returns an error, the caller will deliver a bus error signal
 */
<<<<<<< HEAD
int afs_page_mkwrite(struct vm_area_struct *vma, struct page *page)
{
	struct afs_vnode *vnode = AFS_FS_I(vma->vm_file->f_mapping->host);

	_enter("{{%x:%u}},{%lx}",
	       vnode->fid.vid, vnode->fid.vnode, page->index);

	/* wait for the page to be written to the cache before we allow it to
	 * be modified */
#ifdef CONFIG_AFS_FSCACHE
	fscache_wait_on_page_write(vnode->cache, page);
#endif

	_leave(" = 0");
	return 0;
=======
vm_fault_t afs_page_mkwrite(struct vm_fault *vmf)
{
	struct file *file = vmf->vma->vm_file;

	if (afs_validate(AFS_FS_I(file_inode(file)), afs_file_key(file)) < 0)
		return VM_FAULT_SIGBUS;
	return netfs_page_mkwrite(vmf, NULL);
}

/*
 * Prune the keys cached for writeback.  The caller must hold vnode->wb_lock.
 */
void afs_prune_wb_keys(struct afs_vnode *vnode)
{
	LIST_HEAD(graveyard);
	struct afs_wb_key *wbk, *tmp;

	/* Discard unused keys */
	spin_lock(&vnode->wb_lock);

	if (!mapping_tagged(&vnode->netfs.inode.i_data, PAGECACHE_TAG_WRITEBACK) &&
	    !mapping_tagged(&vnode->netfs.inode.i_data, PAGECACHE_TAG_DIRTY)) {
		list_for_each_entry_safe(wbk, tmp, &vnode->wb_keys, vnode_link) {
			if (refcount_read(&wbk->usage) == 1)
				list_move(&wbk->vnode_link, &graveyard);
		}
	}

	spin_unlock(&vnode->wb_lock);

	while (!list_empty(&graveyard)) {
		wbk = list_entry(graveyard.next, struct afs_wb_key, vnode_link);
		list_del(&wbk->vnode_link);
		afs_put_wb_key(wbk);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
