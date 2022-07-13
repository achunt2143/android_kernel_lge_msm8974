<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * linux/fs/nfs/write.c
 *
 * Write file data over NFS.
 *
 * Copyright (C) 1996, 1997, Olaf Kirch <okir@monad.swb.de>
 */

#include <linux/types.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/pagemap.h>
#include <linux/file.h>
#include <linux/writeback.h>
#include <linux/swap.h>
#include <linux/migrate.h>

#include <linux/sunrpc/clnt.h>
#include <linux/nfs_fs.h>
#include <linux/nfs_mount.h>
#include <linux/nfs_page.h>
#include <linux/backing-dev.h>
#include <linux/export.h>
<<<<<<< HEAD

#include <asm/uaccess.h>
=======
#include <linux/freezer.h>
#include <linux/wait.h>
#include <linux/iversion.h>
#include <linux/filelock.h>

#include <linux/uaccess.h>
#include <linux/sched/mm.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include "delegation.h"
#include "internal.h"
#include "iostat.h"
#include "nfs4_fs.h"
#include "fscache.h"
#include "pnfs.h"

<<<<<<< HEAD
=======
#include "nfstrace.h"

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define NFSDBG_FACILITY		NFSDBG_PAGECACHE

#define MIN_POOL_WRITE		(32)
#define MIN_POOL_COMMIT		(4)

<<<<<<< HEAD
/*
 * Local function declarations
 */
static void nfs_pageio_init_write(struct nfs_pageio_descriptor *desc,
				  struct inode *inode, int ioflags);
static void nfs_redirty_request(struct nfs_page *req);
static const struct rpc_call_ops nfs_write_partial_ops;
static const struct rpc_call_ops nfs_write_full_ops;
static const struct rpc_call_ops nfs_commit_ops;

static struct kmem_cache *nfs_wdata_cachep;
static mempool_t *nfs_wdata_mempool;
static mempool_t *nfs_commit_mempool;

struct nfs_write_data *nfs_commitdata_alloc(void)
{
	struct nfs_write_data *p = mempool_alloc(nfs_commit_mempool, GFP_NOFS);

	if (p) {
		memset(p, 0, sizeof(*p));
		INIT_LIST_HEAD(&p->pages);
	}
=======
struct nfs_io_completion {
	void (*complete)(void *data);
	void *data;
	struct kref refcount;
};

/*
 * Local function declarations
 */
static void nfs_redirty_request(struct nfs_page *req);
static const struct rpc_call_ops nfs_commit_ops;
static const struct nfs_pgio_completion_ops nfs_async_write_completion_ops;
static const struct nfs_commit_completion_ops nfs_commit_completion_ops;
static const struct nfs_rw_ops nfs_rw_write_ops;
static void nfs_inode_remove_request(struct nfs_page *req);
static void nfs_clear_request_commit(struct nfs_commit_info *cinfo,
				     struct nfs_page *req);
static void nfs_init_cinfo_from_inode(struct nfs_commit_info *cinfo,
				      struct inode *inode);
static struct nfs_page *
nfs_page_search_commits_for_head_request_locked(struct nfs_inode *nfsi,
						struct folio *folio);

static struct kmem_cache *nfs_wdata_cachep;
static mempool_t *nfs_wdata_mempool;
static struct kmem_cache *nfs_cdata_cachep;
static mempool_t *nfs_commit_mempool;

struct nfs_commit_data *nfs_commitdata_alloc(void)
{
	struct nfs_commit_data *p;

	p = kmem_cache_zalloc(nfs_cdata_cachep, nfs_io_gfp_mask());
	if (!p) {
		p = mempool_alloc(nfs_commit_mempool, GFP_NOWAIT);
		if (!p)
			return NULL;
		memset(p, 0, sizeof(*p));
	}
	INIT_LIST_HEAD(&p->pages);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return p;
}
EXPORT_SYMBOL_GPL(nfs_commitdata_alloc);

<<<<<<< HEAD
void nfs_commit_free(struct nfs_write_data *p)
{
	if (p && (p->pagevec != &p->page_array[0]))
		kfree(p->pagevec);
=======
void nfs_commit_free(struct nfs_commit_data *p)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mempool_free(p, nfs_commit_mempool);
}
EXPORT_SYMBOL_GPL(nfs_commit_free);

<<<<<<< HEAD
struct nfs_write_data *nfs_writedata_alloc(unsigned int pagecount)
{
	struct nfs_write_data *p = mempool_alloc(nfs_wdata_mempool, GFP_NOFS);

	if (p) {
		memset(p, 0, sizeof(*p));
		INIT_LIST_HEAD(&p->pages);
		p->npages = pagecount;
		if (pagecount <= ARRAY_SIZE(p->page_array))
			p->pagevec = p->page_array;
		else {
			p->pagevec = kcalloc(pagecount, sizeof(struct page *), GFP_NOFS);
			if (!p->pagevec) {
				mempool_free(p, nfs_wdata_mempool);
				p = NULL;
			}
		}
	}
	return p;
}

void nfs_writedata_free(struct nfs_write_data *p)
{
	if (p && (p->pagevec != &p->page_array[0]))
		kfree(p->pagevec);
	mempool_free(p, nfs_wdata_mempool);
}

void nfs_writedata_release(struct nfs_write_data *wdata)
{
	put_nfs_open_context(wdata->args.context);
	nfs_writedata_free(wdata);
}

static void nfs_context_set_write_error(struct nfs_open_context *ctx, int error)
{
	ctx->error = error;
	smp_wmb();
	set_bit(NFS_CONTEXT_ERROR_WRITE, &ctx->flags);
}

static struct nfs_page *nfs_page_find_request_locked(struct page *page)
{
	struct nfs_page *req = NULL;

	if (PagePrivate(page)) {
		req = (struct nfs_page *)page_private(page);
		if (req != NULL)
			kref_get(&req->wb_kref);
	}
	return req;
}

static struct nfs_page *nfs_page_find_request(struct page *page)
{
	struct inode *inode = page->mapping->host;
	struct nfs_page *req = NULL;

	spin_lock(&inode->i_lock);
	req = nfs_page_find_request_locked(page);
	spin_unlock(&inode->i_lock);
	return req;
}

/* Adjust the file length if we're writing beyond the end */
static void nfs_grow_file(struct page *page, unsigned int offset, unsigned int count)
{
	struct inode *inode = page->mapping->host;
=======
static struct nfs_pgio_header *nfs_writehdr_alloc(void)
{
	struct nfs_pgio_header *p;

	p = kmem_cache_zalloc(nfs_wdata_cachep, nfs_io_gfp_mask());
	if (!p) {
		p = mempool_alloc(nfs_wdata_mempool, GFP_NOWAIT);
		if (!p)
			return NULL;
		memset(p, 0, sizeof(*p));
	}
	p->rw_mode = FMODE_WRITE;
	return p;
}

static void nfs_writehdr_free(struct nfs_pgio_header *hdr)
{
	mempool_free(hdr, nfs_wdata_mempool);
}

static struct nfs_io_completion *nfs_io_completion_alloc(gfp_t gfp_flags)
{
	return kmalloc(sizeof(struct nfs_io_completion), gfp_flags);
}

static void nfs_io_completion_init(struct nfs_io_completion *ioc,
		void (*complete)(void *), void *data)
{
	ioc->complete = complete;
	ioc->data = data;
	kref_init(&ioc->refcount);
}

static void nfs_io_completion_release(struct kref *kref)
{
	struct nfs_io_completion *ioc = container_of(kref,
			struct nfs_io_completion, refcount);
	ioc->complete(ioc->data);
	kfree(ioc);
}

static void nfs_io_completion_get(struct nfs_io_completion *ioc)
{
	if (ioc != NULL)
		kref_get(&ioc->refcount);
}

static void nfs_io_completion_put(struct nfs_io_completion *ioc)
{
	if (ioc != NULL)
		kref_put(&ioc->refcount, nfs_io_completion_release);
}

static void
nfs_page_set_inode_ref(struct nfs_page *req, struct inode *inode)
{
	if (!test_and_set_bit(PG_INODE_REF, &req->wb_flags)) {
		kref_get(&req->wb_kref);
		atomic_long_inc(&NFS_I(inode)->nrequests);
	}
}

static int
nfs_cancel_remove_inode(struct nfs_page *req, struct inode *inode)
{
	int ret;

	if (!test_bit(PG_REMOVE, &req->wb_flags))
		return 0;
	ret = nfs_page_group_lock(req);
	if (ret)
		return ret;
	if (test_and_clear_bit(PG_REMOVE, &req->wb_flags))
		nfs_page_set_inode_ref(req, inode);
	nfs_page_group_unlock(req);
	return 0;
}

static struct nfs_page *nfs_folio_private_request(struct folio *folio)
{
	return folio_get_private(folio);
}

/**
 * nfs_folio_find_private_request - find head request associated with a folio
 * @folio: pointer to folio
 *
 * must be called while holding the inode lock.
 *
 * returns matching head request with reference held, or NULL if not found.
 */
static struct nfs_page *nfs_folio_find_private_request(struct folio *folio)
{
	struct address_space *mapping = folio_file_mapping(folio);
	struct nfs_page *req;

	if (!folio_test_private(folio))
		return NULL;
	spin_lock(&mapping->i_private_lock);
	req = nfs_folio_private_request(folio);
	if (req) {
		WARN_ON_ONCE(req->wb_head != req);
		kref_get(&req->wb_kref);
	}
	spin_unlock(&mapping->i_private_lock);
	return req;
}

static struct nfs_page *nfs_folio_find_swap_request(struct folio *folio)
{
	struct inode *inode = folio_file_mapping(folio)->host;
	struct nfs_inode *nfsi = NFS_I(inode);
	struct nfs_page *req = NULL;
	if (!folio_test_swapcache(folio))
		return NULL;
	mutex_lock(&nfsi->commit_mutex);
	if (folio_test_swapcache(folio)) {
		req = nfs_page_search_commits_for_head_request_locked(nfsi,
								      folio);
		if (req) {
			WARN_ON_ONCE(req->wb_head != req);
			kref_get(&req->wb_kref);
		}
	}
	mutex_unlock(&nfsi->commit_mutex);
	return req;
}

/**
 * nfs_folio_find_head_request - find head request associated with a folio
 * @folio: pointer to folio
 *
 * returns matching head request with reference held, or NULL if not found.
 */
static struct nfs_page *nfs_folio_find_head_request(struct folio *folio)
{
	struct nfs_page *req;

	req = nfs_folio_find_private_request(folio);
	if (!req)
		req = nfs_folio_find_swap_request(folio);
	return req;
}

static struct nfs_page *nfs_folio_find_and_lock_request(struct folio *folio)
{
	struct inode *inode = folio_file_mapping(folio)->host;
	struct nfs_page *req, *head;
	int ret;

	for (;;) {
		req = nfs_folio_find_head_request(folio);
		if (!req)
			return req;
		head = nfs_page_group_lock_head(req);
		if (head != req)
			nfs_release_request(req);
		if (IS_ERR(head))
			return head;
		ret = nfs_cancel_remove_inode(head, inode);
		if (ret < 0) {
			nfs_unlock_and_release_request(head);
			return ERR_PTR(ret);
		}
		/* Ensure that nobody removed the request before we locked it */
		if (head == nfs_folio_private_request(folio))
			break;
		if (folio_test_swapcache(folio))
			break;
		nfs_unlock_and_release_request(head);
	}
	return head;
}

/* Adjust the file length if we're writing beyond the end */
static void nfs_grow_file(struct folio *folio, unsigned int offset,
			  unsigned int count)
{
	struct inode *inode = folio_file_mapping(folio)->host;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	loff_t end, i_size;
	pgoff_t end_index;

	spin_lock(&inode->i_lock);
	i_size = i_size_read(inode);
<<<<<<< HEAD
	end_index = (i_size - 1) >> PAGE_CACHE_SHIFT;
	if (i_size > 0 && page->index < end_index)
		goto out;
	end = ((loff_t)page->index << PAGE_CACHE_SHIFT) + ((loff_t)offset+count);
	if (i_size >= end)
		goto out;
	i_size_write(inode, end);
	nfs_inc_stats(inode, NFSIOS_EXTENDWRITE);
out:
	spin_unlock(&inode->i_lock);
}

/* A writeback failed: mark the page as bad, and invalidate the page cache */
static void nfs_set_pageerror(struct page *page)
{
	SetPageError(page);
	nfs_zap_mapping(page->mapping->host, page->mapping);
=======
	end_index = ((i_size - 1) >> folio_shift(folio)) << folio_order(folio);
	if (i_size > 0 && folio_index(folio) < end_index)
		goto out;
	end = folio_file_pos(folio) + (loff_t)offset + (loff_t)count;
	if (i_size >= end)
		goto out;
	trace_nfs_size_grow(inode, end);
	i_size_write(inode, end);
	NFS_I(inode)->cache_validity &= ~NFS_INO_INVALID_SIZE;
	nfs_inc_stats(inode, NFSIOS_EXTENDWRITE);
out:
	spin_unlock(&inode->i_lock);
	nfs_fscache_invalidate(inode, 0);
}

/* A writeback failed: mark the page as bad, and invalidate the page cache */
static void nfs_set_pageerror(struct address_space *mapping)
{
	struct inode *inode = mapping->host;

	nfs_zap_mapping(mapping->host, mapping);
	/* Force file size revalidation */
	spin_lock(&inode->i_lock);
	nfs_set_cache_invalid(inode, NFS_INO_REVAL_FORCED |
					     NFS_INO_INVALID_CHANGE |
					     NFS_INO_INVALID_SIZE);
	spin_unlock(&inode->i_lock);
}

static void nfs_mapping_set_error(struct folio *folio, int error)
{
	struct address_space *mapping = folio_file_mapping(folio);

	folio_set_error(folio);
	filemap_set_wb_err(mapping, error);
	if (mapping->host)
		errseq_set(&mapping->host->i_sb->s_wb_err,
			   error == -ENOSPC ? -ENOSPC : -EIO);
	nfs_set_pageerror(mapping);
}

/*
 * nfs_page_group_search_locked
 * @head - head request of page group
 * @page_offset - offset into page
 *
 * Search page group with head @head to find a request that contains the
 * page offset @page_offset.
 *
 * Returns a pointer to the first matching nfs request, or NULL if no
 * match is found.
 *
 * Must be called with the page group lock held
 */
static struct nfs_page *
nfs_page_group_search_locked(struct nfs_page *head, unsigned int page_offset)
{
	struct nfs_page *req;

	req = head;
	do {
		if (page_offset >= req->wb_pgbase &&
		    page_offset < (req->wb_pgbase + req->wb_bytes))
			return req;

		req = req->wb_this_page;
	} while (req != head);

	return NULL;
}

/*
 * nfs_page_group_covers_page
 * @head - head request of page group
 *
 * Return true if the page group with head @head covers the whole page,
 * returns false otherwise
 */
static bool nfs_page_group_covers_page(struct nfs_page *req)
{
	unsigned int len = nfs_folio_length(nfs_page_to_folio(req));
	struct nfs_page *tmp;
	unsigned int pos = 0;

	nfs_page_group_lock(req);

	for (;;) {
		tmp = nfs_page_group_search_locked(req->wb_head, pos);
		if (!tmp)
			break;
		pos = tmp->wb_pgbase + tmp->wb_bytes;
	}

	nfs_page_group_unlock(req);
	return pos >= len;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* We can set the PG_uptodate flag if we see that a write request
 * covers the full page.
 */
<<<<<<< HEAD
static void nfs_mark_uptodate(struct page *page, unsigned int base, unsigned int count)
{
	if (PageUptodate(page))
		return;
	if (base != 0)
		return;
	if (count != nfs_page_length(page))
		return;
	SetPageUptodate(page);
=======
static void nfs_mark_uptodate(struct nfs_page *req)
{
	struct folio *folio = nfs_page_to_folio(req);

	if (folio_test_uptodate(folio))
		return;
	if (!nfs_page_group_covers_page(req))
		return;
	folio_mark_uptodate(folio);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int wb_priority(struct writeback_control *wbc)
{
<<<<<<< HEAD
	if (wbc->for_reclaim)
		return FLUSH_HIGHPRI | FLUSH_STABLE;
	if (wbc->for_kupdate || wbc->for_background)
		return FLUSH_LOWPRI | FLUSH_COND_STABLE;
	return FLUSH_COND_STABLE;
=======
	int ret = 0;

	if (wbc->sync_mode == WB_SYNC_ALL)
		ret = FLUSH_COND_STABLE;
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * NFS congestion control
 */

int nfs_congestion_kb;

#define NFS_CONGESTION_ON_THRESH 	(nfs_congestion_kb >> (PAGE_SHIFT-10))
#define NFS_CONGESTION_OFF_THRESH	\
	(NFS_CONGESTION_ON_THRESH - (NFS_CONGESTION_ON_THRESH >> 2))

<<<<<<< HEAD
static int nfs_set_page_writeback(struct page *page)
{
	int ret = test_set_page_writeback(page);

	if (!ret) {
		struct inode *inode = page->mapping->host;
		struct nfs_server *nfss = NFS_SERVER(inode);

		page_cache_get(page);
		if (atomic_long_inc_return(&nfss->writeback) >
				NFS_CONGESTION_ON_THRESH) {
			set_bdi_congested(&nfss->backing_dev_info,
						BLK_RW_ASYNC);
		}
	}
	return ret;
}

static void nfs_end_page_writeback(struct page *page)
{
	struct inode *inode = page->mapping->host;
	struct nfs_server *nfss = NFS_SERVER(inode);

	end_page_writeback(page);
	page_cache_release(page);
	if (atomic_long_dec_return(&nfss->writeback) < NFS_CONGESTION_OFF_THRESH)
		clear_bdi_congested(&nfss->backing_dev_info, BLK_RW_ASYNC);
}

static struct nfs_page *nfs_find_and_lock_request(struct page *page, bool nonblock)
{
	struct inode *inode = page->mapping->host;
	struct nfs_page *req;
	int ret;

	spin_lock(&inode->i_lock);
	for (;;) {
		req = nfs_page_find_request_locked(page);
		if (req == NULL)
			break;
		if (nfs_lock_request_dontget(req))
			break;
		/* Note: If we hold the page lock, as is the case in nfs_writepage,
		 *	 then the call to nfs_lock_request_dontget() will always
		 *	 succeed provided that someone hasn't already marked the
		 *	 request as dirty (in which case we don't care).
		 */
		spin_unlock(&inode->i_lock);
		if (!nonblock)
			ret = nfs_wait_on_request(req);
		else
			ret = -EAGAIN;
		nfs_release_request(req);
		if (ret != 0)
			return ERR_PTR(ret);
		spin_lock(&inode->i_lock);
	}
	spin_unlock(&inode->i_lock);
	return req;
=======
static void nfs_folio_set_writeback(struct folio *folio)
{
	struct nfs_server *nfss = NFS_SERVER(folio_file_mapping(folio)->host);

	folio_start_writeback(folio);
	if (atomic_long_inc_return(&nfss->writeback) > NFS_CONGESTION_ON_THRESH)
		nfss->write_congested = 1;
}

static void nfs_folio_end_writeback(struct folio *folio)
{
	struct nfs_server *nfss = NFS_SERVER(folio_file_mapping(folio)->host);

	folio_end_writeback(folio);
	if (atomic_long_dec_return(&nfss->writeback) <
	    NFS_CONGESTION_OFF_THRESH)
		nfss->write_congested = 0;
}

static void nfs_page_end_writeback(struct nfs_page *req)
{
	if (nfs_page_group_sync_on_bit(req, PG_WB_END)) {
		nfs_unlock_request(req);
		nfs_folio_end_writeback(nfs_page_to_folio(req));
	} else
		nfs_unlock_request(req);
}

/*
 * nfs_destroy_unlinked_subrequests - destroy recently unlinked subrequests
 *
 * @destroy_list - request list (using wb_this_page) terminated by @old_head
 * @old_head - the old head of the list
 *
 * All subrequests must be locked and removed from all lists, so at this point
 * they are only "active" in this function, and possibly in nfs_wait_on_request
 * with a reference held by some other context.
 */
static void
nfs_destroy_unlinked_subrequests(struct nfs_page *destroy_list,
				 struct nfs_page *old_head,
				 struct inode *inode)
{
	while (destroy_list) {
		struct nfs_page *subreq = destroy_list;

		destroy_list = (subreq->wb_this_page == old_head) ?
				   NULL : subreq->wb_this_page;

		/* Note: lock subreq in order to change subreq->wb_head */
		nfs_page_set_headlock(subreq);
		WARN_ON_ONCE(old_head != subreq->wb_head);

		/* make sure old group is not used */
		subreq->wb_this_page = subreq;
		subreq->wb_head = subreq;

		clear_bit(PG_REMOVE, &subreq->wb_flags);

		/* Note: races with nfs_page_group_destroy() */
		if (!kref_read(&subreq->wb_kref)) {
			/* Check if we raced with nfs_page_group_destroy() */
			if (test_and_clear_bit(PG_TEARDOWN, &subreq->wb_flags)) {
				nfs_page_clear_headlock(subreq);
				nfs_free_request(subreq);
			} else
				nfs_page_clear_headlock(subreq);
			continue;
		}
		nfs_page_clear_headlock(subreq);

		nfs_release_request(old_head);

		if (test_and_clear_bit(PG_INODE_REF, &subreq->wb_flags)) {
			nfs_release_request(subreq);
			atomic_long_dec(&NFS_I(inode)->nrequests);
		}

		/* subreq is now totally disconnected from page group or any
		 * write / commit lists. last chance to wake any waiters */
		nfs_unlock_and_release_request(subreq);
	}
}

/*
 * nfs_join_page_group - destroy subrequests of the head req
 * @head: the page used to lookup the "page group" of nfs_page structures
 * @inode: Inode to which the request belongs.
 *
 * This function joins all sub requests to the head request by first
 * locking all requests in the group, cancelling any pending operations
 * and finally updating the head request to cover the whole range covered by
 * the (former) group.  All subrequests are removed from any write or commit
 * lists, unlinked from the group and destroyed.
 */
void nfs_join_page_group(struct nfs_page *head, struct nfs_commit_info *cinfo,
			 struct inode *inode)
{
	struct nfs_page *subreq;
	struct nfs_page *destroy_list = NULL;
	unsigned int pgbase, off, bytes;

	pgbase = head->wb_pgbase;
	bytes = head->wb_bytes;
	off = head->wb_offset;
	for (subreq = head->wb_this_page; subreq != head;
			subreq = subreq->wb_this_page) {
		/* Subrequests should always form a contiguous range */
		if (pgbase > subreq->wb_pgbase) {
			off -= pgbase - subreq->wb_pgbase;
			bytes += pgbase - subreq->wb_pgbase;
			pgbase = subreq->wb_pgbase;
		}
		bytes = max(subreq->wb_pgbase + subreq->wb_bytes
				- pgbase, bytes);
	}

	/* Set the head request's range to cover the former page group */
	head->wb_pgbase = pgbase;
	head->wb_bytes = bytes;
	head->wb_offset = off;

	/* Now that all requests are locked, make sure they aren't on any list.
	 * Commit list removal accounting is done after locks are dropped */
	subreq = head;
	do {
		nfs_clear_request_commit(cinfo, subreq);
		subreq = subreq->wb_this_page;
	} while (subreq != head);

	/* unlink subrequests from head, destroy them later */
	if (head->wb_this_page != head) {
		/* destroy list will be terminated by head */
		destroy_list = head->wb_this_page;
		head->wb_this_page = head;
	}

	nfs_destroy_unlinked_subrequests(destroy_list, head, inode);
}

/*
 * nfs_lock_and_join_requests - join all subreqs to the head req
 * @folio: the folio used to lookup the "page group" of nfs_page structures
 *
 * This function joins all sub requests to the head request by first
 * locking all requests in the group, cancelling any pending operations
 * and finally updating the head request to cover the whole range covered by
 * the (former) group.  All subrequests are removed from any write or commit
 * lists, unlinked from the group and destroyed.
 *
 * Returns a locked, referenced pointer to the head request - which after
 * this call is guaranteed to be the only request associated with the page.
 * Returns NULL if no requests are found for @folio, or a ERR_PTR if an
 * error was encountered.
 */
static struct nfs_page *nfs_lock_and_join_requests(struct folio *folio)
{
	struct inode *inode = folio_file_mapping(folio)->host;
	struct nfs_page *head;
	struct nfs_commit_info cinfo;
	int ret;

	nfs_init_cinfo_from_inode(&cinfo, inode);
	/*
	 * A reference is taken only on the head request which acts as a
	 * reference to the whole page group - the group will not be destroyed
	 * until the head reference is released.
	 */
	head = nfs_folio_find_and_lock_request(folio);
	if (IS_ERR_OR_NULL(head))
		return head;

	/* lock each request in the page group */
	ret = nfs_page_group_lock_subrequests(head);
	if (ret < 0) {
		nfs_unlock_and_release_request(head);
		return ERR_PTR(ret);
	}

	nfs_join_page_group(head, &cinfo, inode);

	return head;
}

static void nfs_write_error(struct nfs_page *req, int error)
{
	trace_nfs_write_error(nfs_page_to_inode(req), req, error);
	nfs_mapping_set_error(nfs_page_to_folio(req), error);
	nfs_inode_remove_request(req);
	nfs_page_end_writeback(req);
	nfs_release_request(req);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Find an associated nfs write request, and prepare to flush it out
 * May return an error if the user signalled nfs_wait_on_request().
 */
<<<<<<< HEAD
static int nfs_page_async_flush(struct nfs_pageio_descriptor *pgio,
				struct page *page, bool nonblock)
=======
static int nfs_page_async_flush(struct folio *folio,
				struct writeback_control *wbc,
				struct nfs_pageio_descriptor *pgio)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct nfs_page *req;
	int ret = 0;

<<<<<<< HEAD
	req = nfs_find_and_lock_request(page, nonblock);
=======
	req = nfs_lock_and_join_requests(folio);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!req)
		goto out;
	ret = PTR_ERR(req);
	if (IS_ERR(req))
		goto out;

<<<<<<< HEAD
	ret = nfs_set_page_writeback(page);
	BUG_ON(ret != 0);
	BUG_ON(test_bit(PG_CLEAN, &req->wb_flags));

	if (!nfs_pageio_add_request(pgio, req)) {
		nfs_redirty_request(req);
		ret = pgio->pg_error;
	}
out:
	return ret;
}

static int nfs_do_writepage(struct page *page, struct writeback_control *wbc, struct nfs_pageio_descriptor *pgio)
{
	struct inode *inode = page->mapping->host;
	int ret;

	nfs_inc_stats(inode, NFSIOS_VFSWRITEPAGE);
	nfs_add_stats(inode, NFSIOS_WRITEPAGES, 1);

	nfs_pageio_cond_complete(pgio, page->index);
	ret = nfs_page_async_flush(pgio, page, wbc->sync_mode == WB_SYNC_NONE);
	if (ret == -EAGAIN) {
		redirty_page_for_writepage(wbc, page);
		ret = 0;
	}
	return ret;
=======
	nfs_folio_set_writeback(folio);
	WARN_ON_ONCE(test_bit(PG_CLEAN, &req->wb_flags));

	/* If there is a fatal error that covers this write, just exit */
	ret = pgio->pg_error;
	if (nfs_error_is_fatal_on_server(ret))
		goto out_launder;

	ret = 0;
	if (!nfs_pageio_add_request(pgio, req)) {
		ret = pgio->pg_error;
		/*
		 * Remove the problematic req upon fatal errors on the server
		 */
		if (nfs_error_is_fatal_on_server(ret))
			goto out_launder;
		if (wbc->sync_mode == WB_SYNC_NONE)
			ret = AOP_WRITEPAGE_ACTIVATE;
		folio_redirty_for_writepage(wbc, folio);
		nfs_redirty_request(req);
		pgio->pg_error = 0;
	} else
		nfs_add_stats(folio_file_mapping(folio)->host,
			      NFSIOS_WRITEPAGES, 1);
out:
	return ret;
out_launder:
	nfs_write_error(req, ret);
	return 0;
}

static int nfs_do_writepage(struct folio *folio, struct writeback_control *wbc,
			    struct nfs_pageio_descriptor *pgio)
{
	nfs_pageio_cond_complete(pgio, folio_index(folio));
	return nfs_page_async_flush(folio, wbc, pgio);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Write an mmapped page to the server.
 */
<<<<<<< HEAD
static int nfs_writepage_locked(struct page *page, struct writeback_control *wbc)
{
	struct nfs_pageio_descriptor pgio;
	int err;

	nfs_pageio_init_write(&pgio, page->mapping->host, wb_priority(wbc));
	err = nfs_do_writepage(page, wbc, &pgio);
	nfs_pageio_complete(&pgio);
	if (err < 0)
		return err;
	if (pgio.pg_error < 0)
		return pgio.pg_error;
	return 0;
}

int nfs_writepage(struct page *page, struct writeback_control *wbc)
{
	int ret;

	ret = nfs_writepage_locked(page, wbc);
	unlock_page(page);
	return ret;
}

static int nfs_writepages_callback(struct page *page, struct writeback_control *wbc, void *data)
{
	int ret;

	ret = nfs_do_writepage(page, wbc, data);
	unlock_page(page);
	return ret;
=======
static int nfs_writepage_locked(struct folio *folio,
				struct writeback_control *wbc)
{
	struct nfs_pageio_descriptor pgio;
	struct inode *inode = folio_file_mapping(folio)->host;
	int err;

	nfs_inc_stats(inode, NFSIOS_VFSWRITEPAGE);
	nfs_pageio_init_write(&pgio, inode, 0, false,
			      &nfs_async_write_completion_ops);
	err = nfs_do_writepage(folio, wbc, &pgio);
	pgio.pg_error = 0;
	nfs_pageio_complete(&pgio);
	return err;
}

static int nfs_writepages_callback(struct folio *folio,
				   struct writeback_control *wbc, void *data)
{
	int ret;

	ret = nfs_do_writepage(folio, wbc, data);
	if (ret != AOP_WRITEPAGE_ACTIVATE)
		folio_unlock(folio);
	return ret;
}

static void nfs_io_completion_commit(void *inode)
{
	nfs_commit_inode(inode, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int nfs_writepages(struct address_space *mapping, struct writeback_control *wbc)
{
	struct inode *inode = mapping->host;
<<<<<<< HEAD
	unsigned long *bitlock = &NFS_I(inode)->flags;
	struct nfs_pageio_descriptor pgio;
	int err;

	/* Stop dirtying of new pages while we sync */
	err = wait_on_bit_lock(bitlock, NFS_INO_FLUSHING,
			nfs_wait_bit_killable, TASK_KILLABLE);
	if (err)
		goto out_err;

	nfs_inc_stats(inode, NFSIOS_VFSWRITEPAGES);

	nfs_pageio_init_write(&pgio, inode, wb_priority(wbc));
	err = write_cache_pages(mapping, wbc, nfs_writepages_callback, &pgio);
	nfs_pageio_complete(&pgio);

	clear_bit_unlock(NFS_INO_FLUSHING, bitlock);
	smp_mb__after_clear_bit();
	wake_up_bit(bitlock, NFS_INO_FLUSHING);

	if (err < 0)
		goto out_err;
	err = pgio.pg_error;
	if (err < 0)
		goto out_err;
=======
	struct nfs_pageio_descriptor pgio;
	struct nfs_io_completion *ioc = NULL;
	unsigned int mntflags = NFS_SERVER(inode)->flags;
	int priority = 0;
	int err;

	if (wbc->sync_mode == WB_SYNC_NONE &&
	    NFS_SERVER(inode)->write_congested)
		return 0;

	nfs_inc_stats(inode, NFSIOS_VFSWRITEPAGES);

	if (!(mntflags & NFS_MOUNT_WRITE_EAGER) || wbc->for_kupdate ||
	    wbc->for_background || wbc->for_sync || wbc->for_reclaim) {
		ioc = nfs_io_completion_alloc(GFP_KERNEL);
		if (ioc)
			nfs_io_completion_init(ioc, nfs_io_completion_commit,
					       inode);
		priority = wb_priority(wbc);
	}

	do {
		nfs_pageio_init_write(&pgio, inode, priority, false,
				      &nfs_async_write_completion_ops);
		pgio.pg_io_completion = ioc;
		err = write_cache_pages(mapping, wbc, nfs_writepages_callback,
					&pgio);
		pgio.pg_error = 0;
		nfs_pageio_complete(&pgio);
		if (err == -EAGAIN && mntflags & NFS_MOUNT_SOFTERR)
			break;
	} while (err < 0 && !nfs_error_is_fatal(err));
	nfs_io_completion_put(ioc);

	if (err < 0)
		goto out_err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
out_err:
	return err;
}

/*
 * Insert a write request into an inode
 */
<<<<<<< HEAD
static void nfs_inode_add_request(struct inode *inode, struct nfs_page *req)
{
	struct nfs_inode *nfsi = NFS_I(inode);

	/* Lock the request! */
	nfs_lock_request_dontget(req);

	spin_lock(&inode->i_lock);
	if (!nfsi->npages && nfs_have_delegation(inode, FMODE_WRITE))
		inode->i_version++;
	set_bit(PG_MAPPED, &req->wb_flags);
	SetPagePrivate(req->wb_page);
	set_page_private(req->wb_page, (unsigned long)req);
	nfsi->npages++;
	kref_get(&req->wb_kref);
	spin_unlock(&inode->i_lock);
=======
static void nfs_inode_add_request(struct nfs_page *req)
{
	struct folio *folio = nfs_page_to_folio(req);
	struct address_space *mapping = folio_file_mapping(folio);
	struct nfs_inode *nfsi = NFS_I(mapping->host);

	WARN_ON_ONCE(req->wb_this_page != req);

	/* Lock the request! */
	nfs_lock_request(req);

	/*
	 * Swap-space should not get truncated. Hence no need to plug the race
	 * with invalidate/truncate.
	 */
	spin_lock(&mapping->i_private_lock);
	if (likely(!folio_test_swapcache(folio))) {
		set_bit(PG_MAPPED, &req->wb_flags);
		folio_set_private(folio);
		folio->private = req;
	}
	spin_unlock(&mapping->i_private_lock);
	atomic_long_inc(&nfsi->nrequests);
	/* this a head request for a page group - mark it as having an
	 * extra reference so sub groups can follow suit.
	 * This flag also informs pgio layer when to bump nrequests when
	 * adding subrequests. */
	WARN_ON(test_and_set_bit(PG_INODE_REF, &req->wb_flags));
	kref_get(&req->wb_kref);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Remove a write request from an inode
 */
static void nfs_inode_remove_request(struct nfs_page *req)
{
<<<<<<< HEAD
	struct inode *inode = req->wb_context->dentry->d_inode;
	struct nfs_inode *nfsi = NFS_I(inode);

	BUG_ON (!NFS_WBACK_BUSY(req));

	spin_lock(&inode->i_lock);
	set_page_private(req->wb_page, 0);
	ClearPagePrivate(req->wb_page);
	clear_bit(PG_MAPPED, &req->wb_flags);
	nfsi->npages--;
	spin_unlock(&inode->i_lock);
	nfs_release_request(req);
}

static void
nfs_mark_request_dirty(struct nfs_page *req)
{
	__set_page_dirty_nobuffers(req->wb_page);
}

#if defined(CONFIG_NFS_V3) || defined(CONFIG_NFS_V4)
/**
 * nfs_request_add_commit_list - add request to a commit list
 * @req: pointer to a struct nfs_page
 * @head: commit list head
 *
 * This sets the PG_CLEAN bit, updates the inode global count of
 * number of outstanding requests requiring a commit as well as
 * the MM page stats.
 *
 * The caller must _not_ hold the inode->i_lock, but must be
 * holding the nfs_page lock.
 */
void
nfs_request_add_commit_list(struct nfs_page *req, struct list_head *head)
{
	struct inode *inode = req->wb_context->dentry->d_inode;

	set_bit(PG_CLEAN, &(req)->wb_flags);
	spin_lock(&inode->i_lock);
	nfs_list_add_request(req, head);
	NFS_I(inode)->ncommit++;
	spin_unlock(&inode->i_lock);
	inc_zone_page_state(req->wb_page, NR_UNSTABLE_NFS);
	inc_bdi_stat(req->wb_page->mapping->backing_dev_info, BDI_RECLAIMABLE);
	__mark_inode_dirty(inode, I_DIRTY_DATASYNC);
=======
	struct nfs_inode *nfsi = NFS_I(nfs_page_to_inode(req));

	if (nfs_page_group_sync_on_bit(req, PG_REMOVE)) {
		struct folio *folio = nfs_page_to_folio(req->wb_head);
		struct address_space *mapping = folio_file_mapping(folio);

		spin_lock(&mapping->i_private_lock);
		if (likely(folio && !folio_test_swapcache(folio))) {
			folio->private = NULL;
			folio_clear_private(folio);
			clear_bit(PG_MAPPED, &req->wb_head->wb_flags);
		}
		spin_unlock(&mapping->i_private_lock);
	}

	if (test_and_clear_bit(PG_INODE_REF, &req->wb_flags)) {
		atomic_long_dec(&nfsi->nrequests);
		nfs_release_request(req);
	}
}

static void nfs_mark_request_dirty(struct nfs_page *req)
{
	struct folio *folio = nfs_page_to_folio(req);
	if (folio)
		filemap_dirty_folio(folio_mapping(folio), folio);
}

/*
 * nfs_page_search_commits_for_head_request_locked
 *
 * Search through commit lists on @inode for the head request for @folio.
 * Must be called while holding the inode (which is cinfo) lock.
 *
 * Returns the head request if found, or NULL if not found.
 */
static struct nfs_page *
nfs_page_search_commits_for_head_request_locked(struct nfs_inode *nfsi,
						struct folio *folio)
{
	struct nfs_page *freq, *t;
	struct nfs_commit_info cinfo;
	struct inode *inode = &nfsi->vfs_inode;

	nfs_init_cinfo_from_inode(&cinfo, inode);

	/* search through pnfs commit lists */
	freq = pnfs_search_commit_reqs(inode, &cinfo, folio);
	if (freq)
		return freq->wb_head;

	/* Linearly search the commit list for the correct request */
	list_for_each_entry_safe(freq, t, &cinfo.mds->list, wb_list) {
		if (nfs_page_to_folio(freq) == folio)
			return freq->wb_head;
	}

	return NULL;
}

/**
 * nfs_request_add_commit_list_locked - add request to a commit list
 * @req: pointer to a struct nfs_page
 * @dst: commit list head
 * @cinfo: holds list lock and accounting info
 *
 * This sets the PG_CLEAN bit, updates the cinfo count of
 * number of outstanding requests requiring a commit as well as
 * the MM page stats.
 *
 * The caller must hold NFS_I(cinfo->inode)->commit_mutex, and the
 * nfs_page lock.
 */
void
nfs_request_add_commit_list_locked(struct nfs_page *req, struct list_head *dst,
			    struct nfs_commit_info *cinfo)
{
	set_bit(PG_CLEAN, &req->wb_flags);
	nfs_list_add_request(req, dst);
	atomic_long_inc(&cinfo->mds->ncommit);
}
EXPORT_SYMBOL_GPL(nfs_request_add_commit_list_locked);

/**
 * nfs_request_add_commit_list - add request to a commit list
 * @req: pointer to a struct nfs_page
 * @cinfo: holds list lock and accounting info
 *
 * This sets the PG_CLEAN bit, updates the cinfo count of
 * number of outstanding requests requiring a commit as well as
 * the MM page stats.
 *
 * The caller must _not_ hold the cinfo->lock, but must be
 * holding the nfs_page lock.
 */
void
nfs_request_add_commit_list(struct nfs_page *req, struct nfs_commit_info *cinfo)
{
	mutex_lock(&NFS_I(cinfo->inode)->commit_mutex);
	nfs_request_add_commit_list_locked(req, &cinfo->mds->list, cinfo);
	mutex_unlock(&NFS_I(cinfo->inode)->commit_mutex);
	nfs_folio_mark_unstable(nfs_page_to_folio(req), cinfo);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(nfs_request_add_commit_list);

/**
 * nfs_request_remove_commit_list - Remove request from a commit list
 * @req: pointer to a nfs_page
<<<<<<< HEAD
 *
 * This clears the PG_CLEAN bit, and updates the inode global count of
 * number of outstanding requests requiring a commit
 * It does not update the MM page stats.
 *
 * The caller _must_ hold the inode->i_lock and the nfs_page lock.
 */
void
nfs_request_remove_commit_list(struct nfs_page *req)
{
	struct inode *inode = req->wb_context->dentry->d_inode;

	if (!test_and_clear_bit(PG_CLEAN, &(req)->wb_flags))
		return;
	nfs_list_remove_request(req);
	NFS_I(inode)->ncommit--;
}
EXPORT_SYMBOL_GPL(nfs_request_remove_commit_list);

=======
 * @cinfo: holds list lock and accounting info
 *
 * This clears the PG_CLEAN bit, and updates the cinfo's count of
 * number of outstanding requests requiring a commit
 * It does not update the MM page stats.
 *
 * The caller _must_ hold the cinfo->lock and the nfs_page lock.
 */
void
nfs_request_remove_commit_list(struct nfs_page *req,
			       struct nfs_commit_info *cinfo)
{
	if (!test_and_clear_bit(PG_CLEAN, &(req)->wb_flags))
		return;
	nfs_list_remove_request(req);
	atomic_long_dec(&cinfo->mds->ncommit);
}
EXPORT_SYMBOL_GPL(nfs_request_remove_commit_list);

static void nfs_init_cinfo_from_inode(struct nfs_commit_info *cinfo,
				      struct inode *inode)
{
	cinfo->inode = inode;
	cinfo->mds = &NFS_I(inode)->commit_info;
	cinfo->ds = pnfs_get_ds_info(inode);
	cinfo->dreq = NULL;
	cinfo->completion_ops = &nfs_commit_completion_ops;
}

void nfs_init_cinfo(struct nfs_commit_info *cinfo,
		    struct inode *inode,
		    struct nfs_direct_req *dreq)
{
	if (dreq)
		nfs_init_cinfo_from_dreq(cinfo, dreq);
	else
		nfs_init_cinfo_from_inode(cinfo, inode);
}
EXPORT_SYMBOL_GPL(nfs_init_cinfo);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Add a request to the inode's commit list.
 */
<<<<<<< HEAD
static void
nfs_mark_request_commit(struct nfs_page *req, struct pnfs_layout_segment *lseg)
{
	struct inode *inode = req->wb_context->dentry->d_inode;

	if (pnfs_mark_request_commit(req, lseg))
		return;
	nfs_request_add_commit_list(req, &NFS_I(inode)->commit_list);
}

static void
nfs_clear_page_commit(struct page *page)
{
	dec_zone_page_state(page, NR_UNSTABLE_NFS);
	dec_bdi_stat(page->mapping->backing_dev_info, BDI_RECLAIMABLE);
}

static void
nfs_clear_request_commit(struct nfs_page *req)
{
	if (test_bit(PG_CLEAN, &req->wb_flags)) {
		struct inode *inode = req->wb_context->dentry->d_inode;

		if (!pnfs_clear_request_commit(req)) {
			spin_lock(&inode->i_lock);
			nfs_request_remove_commit_list(req);
			spin_unlock(&inode->i_lock);
		}
		nfs_clear_page_commit(req->wb_page);
	}
}

static inline
int nfs_write_need_commit(struct nfs_write_data *data)
{
	if (data->verf.committed == NFS_DATA_SYNC)
		return data->lseg == NULL;
	else
		return data->verf.committed != NFS_FILE_SYNC;
}

static inline
int nfs_reschedule_unstable_write(struct nfs_page *req,
				  struct nfs_write_data *data)
{
	if (test_and_clear_bit(PG_NEED_COMMIT, &req->wb_flags)) {
		nfs_mark_request_commit(req, data->lseg);
		return 1;
	}
	if (test_and_clear_bit(PG_NEED_RESCHED, &req->wb_flags)) {
		nfs_mark_request_dirty(req);
		return 1;
	}
	return 0;
}
#else
static void
nfs_mark_request_commit(struct nfs_page *req, struct pnfs_layout_segment *lseg)
{
}

static void
nfs_clear_request_commit(struct nfs_page *req)
{
}

static inline
int nfs_write_need_commit(struct nfs_write_data *data)
{
	return 0;
}

static inline
int nfs_reschedule_unstable_write(struct nfs_page *req,
				  struct nfs_write_data *data)
{
	return 0;
}
#endif

#if defined(CONFIG_NFS_V3) || defined(CONFIG_NFS_V4)
static int
nfs_need_commit(struct nfs_inode *nfsi)
{
	return nfsi->ncommit > 0;
}

/* i_lock held by caller */
static int
nfs_scan_commit_list(struct list_head *src, struct list_head *dst, int max,
		spinlock_t *lock)
=======
void
nfs_mark_request_commit(struct nfs_page *req, struct pnfs_layout_segment *lseg,
			struct nfs_commit_info *cinfo, u32 ds_commit_idx)
{
	if (pnfs_mark_request_commit(req, lseg, cinfo, ds_commit_idx))
		return;
	nfs_request_add_commit_list(req, cinfo);
}

static void nfs_folio_clear_commit(struct folio *folio)
{
	if (folio) {
		long nr = folio_nr_pages(folio);

		node_stat_mod_folio(folio, NR_WRITEBACK, -nr);
		wb_stat_mod(&inode_to_bdi(folio_file_mapping(folio)->host)->wb,
			    WB_WRITEBACK, -nr);
	}
}

/* Called holding the request lock on @req */
static void nfs_clear_request_commit(struct nfs_commit_info *cinfo,
				     struct nfs_page *req)
{
	if (test_bit(PG_CLEAN, &req->wb_flags)) {
		struct nfs_open_context *ctx = nfs_req_openctx(req);
		struct inode *inode = d_inode(ctx->dentry);

		mutex_lock(&NFS_I(inode)->commit_mutex);
		if (!pnfs_clear_request_commit(req, cinfo)) {
			nfs_request_remove_commit_list(req, cinfo);
		}
		mutex_unlock(&NFS_I(inode)->commit_mutex);
		nfs_folio_clear_commit(nfs_page_to_folio(req));
	}
}

int nfs_write_need_commit(struct nfs_pgio_header *hdr)
{
	if (hdr->verf.committed == NFS_DATA_SYNC)
		return hdr->lseg == NULL;
	return hdr->verf.committed != NFS_FILE_SYNC;
}

static void nfs_async_write_init(struct nfs_pgio_header *hdr)
{
	nfs_io_completion_get(hdr->io_completion);
}

static void nfs_write_completion(struct nfs_pgio_header *hdr)
{
	struct nfs_commit_info cinfo;
	unsigned long bytes = 0;

	if (test_bit(NFS_IOHDR_REDO, &hdr->flags))
		goto out;
	nfs_init_cinfo_from_inode(&cinfo, hdr->inode);
	while (!list_empty(&hdr->pages)) {
		struct nfs_page *req = nfs_list_entry(hdr->pages.next);

		bytes += req->wb_bytes;
		nfs_list_remove_request(req);
		if (test_bit(NFS_IOHDR_ERROR, &hdr->flags) &&
		    (hdr->good_bytes < bytes)) {
			trace_nfs_comp_error(hdr->inode, req, hdr->error);
			nfs_mapping_set_error(nfs_page_to_folio(req),
					      hdr->error);
			goto remove_req;
		}
		if (nfs_write_need_commit(hdr)) {
			/* Reset wb_nio, since the write was successful. */
			req->wb_nio = 0;
			memcpy(&req->wb_verf, &hdr->verf.verifier, sizeof(req->wb_verf));
			nfs_mark_request_commit(req, hdr->lseg, &cinfo,
				hdr->pgio_mirror_idx);
			goto next;
		}
remove_req:
		nfs_inode_remove_request(req);
next:
		nfs_page_end_writeback(req);
		nfs_release_request(req);
	}
out:
	nfs_io_completion_put(hdr->io_completion);
	hdr->release(hdr);
}

unsigned long
nfs_reqs_to_commit(struct nfs_commit_info *cinfo)
{
	return atomic_long_read(&cinfo->mds->ncommit);
}

/* NFS_I(cinfo->inode)->commit_mutex held by caller */
int
nfs_scan_commit_list(struct list_head *src, struct list_head *dst,
		     struct nfs_commit_info *cinfo, int max)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct nfs_page *req, *tmp;
	int ret = 0;

	list_for_each_entry_safe(req, tmp, src, wb_list) {
<<<<<<< HEAD
		if (!nfs_lock_request(req))
			continue;
		if (cond_resched_lock(lock))
			list_safe_reset_next(req, tmp, wb_list);
		nfs_request_remove_commit_list(req);
		nfs_list_add_request(req, dst);
		ret++;
		if (ret == max)
			break;
	}
	return ret;
}
=======
		kref_get(&req->wb_kref);
		if (!nfs_lock_request(req)) {
			nfs_release_request(req);
			continue;
		}
		nfs_request_remove_commit_list(req, cinfo);
		clear_bit(PG_COMMIT_TO_DS, &req->wb_flags);
		nfs_list_add_request(req, dst);
		ret++;
		if ((ret == max) && !cinfo->dreq)
			break;
		cond_resched();
	}
	return ret;
}
EXPORT_SYMBOL_GPL(nfs_scan_commit_list);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * nfs_scan_commit - Scan an inode for commit requests
 * @inode: NFS inode to scan
<<<<<<< HEAD
 * @dst: destination list
=======
 * @dst: mds destination list
 * @cinfo: mds and ds lists of reqs ready to commit
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Moves requests from the inode's 'commit' request list.
 * The requests are *not* checked to ensure that they form a contiguous set.
 */
<<<<<<< HEAD
static int
nfs_scan_commit(struct inode *inode, struct list_head *dst)
{
	struct nfs_inode *nfsi = NFS_I(inode);
	int ret = 0;

	spin_lock(&inode->i_lock);
	if (nfsi->ncommit > 0) {
		const int max = INT_MAX;

		ret = nfs_scan_commit_list(&nfsi->commit_list, dst, max,
				&inode->i_lock);
		ret += pnfs_scan_commit_lists(inode, max - ret,
				&inode->i_lock);
	}
	spin_unlock(&inode->i_lock);
	return ret;
}

#else
static inline int nfs_need_commit(struct nfs_inode *nfsi)
{
	return 0;
}

static inline int nfs_scan_commit(struct inode *inode, struct list_head *dst)
{
	return 0;
}
#endif

=======
int
nfs_scan_commit(struct inode *inode, struct list_head *dst,
		struct nfs_commit_info *cinfo)
{
	int ret = 0;

	if (!atomic_long_read(&cinfo->mds->ncommit))
		return 0;
	mutex_lock(&NFS_I(cinfo->inode)->commit_mutex);
	if (atomic_long_read(&cinfo->mds->ncommit) > 0) {
		const int max = INT_MAX;

		ret = nfs_scan_commit_list(&cinfo->mds->list, dst,
					   cinfo, max);
		ret += pnfs_scan_commit_lists(inode, cinfo, max - ret);
	}
	mutex_unlock(&NFS_I(cinfo->inode)->commit_mutex);
	return ret;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Search for an existing write request, and attempt to update
 * it to reflect a new dirty region on a given page.
 *
 * If the attempt fails, then the existing request is flushed out
 * to disk.
 */
<<<<<<< HEAD
static struct nfs_page *nfs_try_to_update_request(struct inode *inode,
		struct page *page,
		unsigned int offset,
		unsigned int bytes)
=======
static struct nfs_page *nfs_try_to_update_request(struct folio *folio,
						  unsigned int offset,
						  unsigned int bytes)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct nfs_page *req;
	unsigned int rqend;
	unsigned int end;
	int error;

<<<<<<< HEAD
	if (!PagePrivate(page))
		return NULL;

	end = offset + bytes;
	spin_lock(&inode->i_lock);

	for (;;) {
		req = nfs_page_find_request_locked(page);
		if (req == NULL)
			goto out_unlock;

		rqend = req->wb_offset + req->wb_bytes;
		/*
		 * Tell the caller to flush out the request if
		 * the offsets are non-contiguous.
		 * Note: nfs_flush_incompatible() will already
		 * have flushed out requests having wrong owners.
		 */
		if (offset > rqend
		    || end < req->wb_offset)
			goto out_flushme;

		if (nfs_lock_request_dontget(req))
			break;

		/* The request is locked, so wait and then retry */
		spin_unlock(&inode->i_lock);
		error = nfs_wait_on_request(req);
		nfs_release_request(req);
		if (error != 0)
			goto out_err;
		spin_lock(&inode->i_lock);
	}
=======
	end = offset + bytes;

	req = nfs_lock_and_join_requests(folio);
	if (IS_ERR_OR_NULL(req))
		return req;

	rqend = req->wb_offset + req->wb_bytes;
	/*
	 * Tell the caller to flush out the request if
	 * the offsets are non-contiguous.
	 * Note: nfs_flush_incompatible() will already
	 * have flushed out requests having wrong owners.
	 */
	if (offset > rqend || end < req->wb_offset)
		goto out_flushme;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Okay, the request matches. Update the region */
	if (offset < req->wb_offset) {
		req->wb_offset = offset;
		req->wb_pgbase = offset;
	}
	if (end > rqend)
		req->wb_bytes = end - req->wb_offset;
	else
		req->wb_bytes = rqend - req->wb_offset;
<<<<<<< HEAD
out_unlock:
	spin_unlock(&inode->i_lock);
	if (req)
		nfs_clear_request_commit(req);
	return req;
out_flushme:
	spin_unlock(&inode->i_lock);
	nfs_release_request(req);
	error = nfs_wb_page(inode, page);
out_err:
	return ERR_PTR(error);
=======
	req->wb_nio = 0;
	return req;
out_flushme:
	/*
	 * Note: we mark the request dirty here because
	 * nfs_lock_and_join_requests() cannot preserve
	 * commit flags, so we have to replay the write.
	 */
	nfs_mark_request_dirty(req);
	nfs_unlock_and_release_request(req);
	error = nfs_wb_folio(folio_file_mapping(folio)->host, folio);
	return (error < 0) ? ERR_PTR(error) : NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Try to update an existing write request, or create one if there is none.
 *
 * Note: Should always be called with the Page Lock held to prevent races
 * if we have to add a new request. Also assumes that the caller has
 * already called nfs_flush_incompatible() if necessary.
 */
<<<<<<< HEAD
static struct nfs_page * nfs_setup_write_request(struct nfs_open_context* ctx,
		struct page *page, unsigned int offset, unsigned int bytes)
{
	struct inode *inode = page->mapping->host;
	struct nfs_page	*req;

	req = nfs_try_to_update_request(inode, page, offset, bytes);
	if (req != NULL)
		goto out;
	req = nfs_create_request(ctx, inode, page, offset, bytes);
	if (IS_ERR(req))
		goto out;
	nfs_inode_add_request(inode, req);
=======
static struct nfs_page *nfs_setup_write_request(struct nfs_open_context *ctx,
						struct folio *folio,
						unsigned int offset,
						unsigned int bytes)
{
	struct nfs_page *req;

	req = nfs_try_to_update_request(folio, offset, bytes);
	if (req != NULL)
		goto out;
	req = nfs_page_create_from_folio(ctx, folio, offset, bytes);
	if (IS_ERR(req))
		goto out;
	nfs_inode_add_request(req);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	return req;
}

<<<<<<< HEAD
static int nfs_writepage_setup(struct nfs_open_context *ctx, struct page *page,
		unsigned int offset, unsigned int count)
{
	struct nfs_page	*req;

	req = nfs_setup_write_request(ctx, page, offset, count);
	if (IS_ERR(req))
		return PTR_ERR(req);
	/* Update file length */
	nfs_grow_file(page, offset, count);
	nfs_mark_uptodate(page, req->wb_pgbase, req->wb_bytes);
	nfs_mark_request_dirty(req);
	nfs_unlock_request(req);
	return 0;
}

int nfs_flush_incompatible(struct file *file, struct page *page)
{
	struct nfs_open_context *ctx = nfs_file_open_context(file);
=======
static int nfs_writepage_setup(struct nfs_open_context *ctx,
			       struct folio *folio, unsigned int offset,
			       unsigned int count)
{
	struct nfs_page *req;

	req = nfs_setup_write_request(ctx, folio, offset, count);
	if (IS_ERR(req))
		return PTR_ERR(req);
	/* Update file length */
	nfs_grow_file(folio, offset, count);
	nfs_mark_uptodate(req);
	nfs_mark_request_dirty(req);
	nfs_unlock_and_release_request(req);
	return 0;
}

int nfs_flush_incompatible(struct file *file, struct folio *folio)
{
	struct nfs_open_context *ctx = nfs_file_open_context(file);
	struct nfs_lock_context *l_ctx;
	struct file_lock_context *flctx = locks_inode_context(file_inode(file));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct nfs_page	*req;
	int do_flush, status;
	/*
	 * Look for a request corresponding to this page. If there
	 * is one, and it belongs to another file, we flush it out
	 * before we try to copy anything into the page. Do this
	 * due to the lack of an ACCESS-type call in NFSv2.
	 * Also do the same if we find a request from an existing
	 * dropped page.
	 */
	do {
<<<<<<< HEAD
		req = nfs_page_find_request(page);
		if (req == NULL)
			return 0;
		do_flush = req->wb_page != page || req->wb_context != ctx ||
			req->wb_lock_context->lockowner != current->files ||
			req->wb_lock_context->pid != current->tgid;
		nfs_release_request(req);
		if (!do_flush)
			return 0;
		status = nfs_wb_page(page->mapping->host, page);
=======
		req = nfs_folio_find_head_request(folio);
		if (req == NULL)
			return 0;
		l_ctx = req->wb_lock_context;
		do_flush = nfs_page_to_folio(req) != folio ||
			   !nfs_match_open_context(nfs_req_openctx(req), ctx);
		if (l_ctx && flctx &&
		    !(list_empty_careful(&flctx->flc_posix) &&
		      list_empty_careful(&flctx->flc_flock))) {
			do_flush |= l_ctx->lockowner != current->files;
		}
		nfs_release_request(req);
		if (!do_flush)
			return 0;
		status = nfs_wb_folio(folio_file_mapping(folio)->host, folio);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} while (status == 0);
	return status;
}

/*
<<<<<<< HEAD
=======
 * Avoid buffered writes when a open context credential's key would
 * expire soon.
 *
 * Returns -EACCES if the key will expire within RPC_KEY_EXPIRE_FAIL.
 *
 * Return 0 and set a credential flag which triggers the inode to flush
 * and performs  NFS_FILE_SYNC writes if the key will expired within
 * RPC_KEY_EXPIRE_TIMEO.
 */
int
nfs_key_timeout_notify(struct file *filp, struct inode *inode)
{
	struct nfs_open_context *ctx = nfs_file_open_context(filp);

	if (nfs_ctx_key_to_expire(ctx, inode) &&
	    !rcu_access_pointer(ctx->ll_cred))
		/* Already expired! */
		return -EACCES;
	return 0;
}

/*
 * Test if the open context credential key is marked to expire soon.
 */
bool nfs_ctx_key_to_expire(struct nfs_open_context *ctx, struct inode *inode)
{
	struct rpc_auth *auth = NFS_SERVER(inode)->client->cl_auth;
	struct rpc_cred *cred, *new, *old = NULL;
	struct auth_cred acred = {
		.cred = ctx->cred,
	};
	bool ret = false;

	rcu_read_lock();
	cred = rcu_dereference(ctx->ll_cred);
	if (cred && !(cred->cr_ops->crkey_timeout &&
		      cred->cr_ops->crkey_timeout(cred)))
		goto out;
	rcu_read_unlock();

	new = auth->au_ops->lookup_cred(auth, &acred, 0);
	if (new == cred) {
		put_rpccred(new);
		return true;
	}
	if (IS_ERR_OR_NULL(new)) {
		new = NULL;
		ret = true;
	} else if (new->cr_ops->crkey_timeout &&
		   new->cr_ops->crkey_timeout(new))
		ret = true;

	rcu_read_lock();
	old = rcu_dereference_protected(xchg(&ctx->ll_cred,
					     RCU_INITIALIZER(new)), 1);
out:
	rcu_read_unlock();
	put_rpccred(old);
	return ret;
}

/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * If the page cache is marked as unsafe or invalid, then we can't rely on
 * the PageUptodate() flag. In this case, we will need to turn off
 * write optimisations that depend on the page contents being correct.
 */
<<<<<<< HEAD
static int nfs_write_pageuptodate(struct page *page, struct inode *inode)
{
	return PageUptodate(page) &&
		!(NFS_I(inode)->cache_validity & (NFS_INO_REVAL_PAGECACHE|NFS_INO_INVALID_DATA));
=======
static bool nfs_folio_write_uptodate(struct folio *folio, unsigned int pagelen)
{
	struct inode *inode = folio_file_mapping(folio)->host;
	struct nfs_inode *nfsi = NFS_I(inode);

	if (nfs_have_delegated_attributes(inode))
		goto out;
	if (nfsi->cache_validity &
	    (NFS_INO_INVALID_CHANGE | NFS_INO_INVALID_SIZE))
		return false;
	smp_rmb();
	if (test_bit(NFS_INO_INVALIDATING, &nfsi->flags) && pagelen != 0)
		return false;
out:
	if (nfsi->cache_validity & NFS_INO_INVALID_DATA && pagelen != 0)
		return false;
	return folio_test_uptodate(folio) != 0;
}

static bool
is_whole_file_wrlock(struct file_lock *fl)
{
	return fl->fl_start == 0 && fl->fl_end == OFFSET_MAX &&
			lock_is_write(fl);
}

/* If we know the page is up to date, and we're not using byte range locks (or
 * if we have the whole file locked for writing), it may be more efficient to
 * extend the write to cover the entire page in order to avoid fragmentation
 * inefficiencies.
 *
 * If the file is opened for synchronous writes then we can just skip the rest
 * of the checks.
 */
static int nfs_can_extend_write(struct file *file, struct folio *folio,
				unsigned int pagelen)
{
	struct inode *inode = file_inode(file);
	struct file_lock_context *flctx = locks_inode_context(inode);
	struct file_lock *fl;
	int ret;

	if (file->f_flags & O_DSYNC)
		return 0;
	if (!nfs_folio_write_uptodate(folio, pagelen))
		return 0;
	if (NFS_PROTO(inode)->have_delegation(inode, FMODE_WRITE))
		return 1;
	if (!flctx || (list_empty_careful(&flctx->flc_flock) &&
		       list_empty_careful(&flctx->flc_posix)))
		return 1;

	/* Check to see if there are whole file write locks */
	ret = 0;
	spin_lock(&flctx->flc_lock);
	if (!list_empty(&flctx->flc_posix)) {
		fl = list_first_entry(&flctx->flc_posix, struct file_lock,
					c.flc_list);
		if (is_whole_file_wrlock(fl))
			ret = 1;
	} else if (!list_empty(&flctx->flc_flock)) {
		fl = list_first_entry(&flctx->flc_flock, struct file_lock,
					c.flc_list);
		if (lock_is_write(fl))
			ret = 1;
	}
	spin_unlock(&flctx->flc_lock);
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Update and possibly write a cached page of an NFS file.
 *
 * XXX: Keep an eye on generic_file_read to make sure it doesn't do bad
 * things with a page scheduled for an RPC call (e.g. invalidate it).
 */
<<<<<<< HEAD
int nfs_updatepage(struct file *file, struct page *page,
		unsigned int offset, unsigned int count)
{
	struct nfs_open_context *ctx = nfs_file_open_context(file);
	struct inode	*inode = page->mapping->host;
=======
int nfs_update_folio(struct file *file, struct folio *folio,
		     unsigned int offset, unsigned int count)
{
	struct nfs_open_context *ctx = nfs_file_open_context(file);
	struct address_space *mapping = folio_file_mapping(folio);
	struct inode *inode = mapping->host;
	unsigned int pagelen = nfs_folio_length(folio);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int		status = 0;

	nfs_inc_stats(inode, NFSIOS_VFSUPDATEPAGE);

<<<<<<< HEAD
	dprintk("NFS:       nfs_updatepage(%s/%s %d@%lld)\n",
		file->f_path.dentry->d_parent->d_name.name,
		file->f_path.dentry->d_name.name, count,
		(long long)(page_offset(page) + offset));

	/* If we're not using byte range locks, and we know the page
	 * is up to date, it may be more efficient to extend the write
	 * to cover the entire page in order to avoid fragmentation
	 * inefficiencies.
	 */
	if (nfs_write_pageuptodate(page, inode) &&
			inode->i_flock == NULL &&
			!(file->f_flags & O_DSYNC)) {
		count = max(count + offset, nfs_page_length(page));
		offset = 0;
	}

	status = nfs_writepage_setup(ctx, page, offset, count);
	if (status < 0)
		nfs_set_pageerror(page);
	else
		__set_page_dirty_nobuffers(page);

	dprintk("NFS:       nfs_updatepage returns %d (isize %lld)\n",
=======
	dprintk("NFS:       nfs_update_folio(%pD2 %d@%lld)\n", file, count,
		(long long)(folio_file_pos(folio) + offset));

	if (!count)
		goto out;

	if (nfs_can_extend_write(file, folio, pagelen)) {
		count = max(count + offset, pagelen);
		offset = 0;
	}

	status = nfs_writepage_setup(ctx, folio, offset, count);
	if (status < 0)
		nfs_set_pageerror(mapping);
out:
	dprintk("NFS:       nfs_update_folio returns %d (isize %lld)\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			status, (long long)i_size_read(inode));
	return status;
}

<<<<<<< HEAD
static void nfs_writepage_release(struct nfs_page *req,
				  struct nfs_write_data *data)
{
	struct page *page = req->wb_page;

	if (PageError(req->wb_page) || !nfs_reschedule_unstable_write(req, data))
		nfs_inode_remove_request(req);
	nfs_unlock_request(req);
	nfs_end_page_writeback(page);
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int flush_task_priority(int how)
{
	switch (how & (FLUSH_HIGHPRI|FLUSH_LOWPRI)) {
		case FLUSH_HIGHPRI:
			return RPC_PRIORITY_HIGH;
		case FLUSH_LOWPRI:
			return RPC_PRIORITY_LOW;
	}
	return RPC_PRIORITY_NORMAL;
}

<<<<<<< HEAD
int nfs_initiate_write(struct nfs_write_data *data,
		       struct rpc_clnt *clnt,
		       const struct rpc_call_ops *call_ops,
		       int how)
{
	struct inode *inode = data->inode;
	int priority = flush_task_priority(how);
	struct rpc_task *task;
	struct rpc_message msg = {
		.rpc_argp = &data->args,
		.rpc_resp = &data->res,
		.rpc_cred = data->cred,
	};
	struct rpc_task_setup task_setup_data = {
		.rpc_client = clnt,
		.task = &data->task,
		.rpc_message = &msg,
		.callback_ops = call_ops,
		.callback_data = data,
		.workqueue = nfsiod_workqueue,
		.flags = RPC_TASK_ASYNC,
		.priority = priority,
	};
	int ret = 0;

	/* Set up the initial task struct.  */
	NFS_PROTO(inode)->write_setup(data, &msg);

	dprintk("NFS: %5u initiated write call "
		"(req %s/%lld, %u bytes @ offset %llu)\n",
		data->task.tk_pid,
		inode->i_sb->s_id,
		(long long)NFS_FILEID(inode),
		data->args.count,
		(unsigned long long)data->args.offset);

	task = rpc_run_task(&task_setup_data);
	if (IS_ERR(task)) {
		ret = PTR_ERR(task);
		goto out;
	}
	if (how & FLUSH_SYNC) {
		ret = rpc_wait_for_completion_task(task);
		if (ret == 0)
			ret = task->tk_status;
	}
	rpc_put_task(task);
out:
	return ret;
}
EXPORT_SYMBOL_GPL(nfs_initiate_write);

/*
 * Set up the argument/result storage required for the RPC call.
 */
static void nfs_write_rpcsetup(struct nfs_page *req,
		struct nfs_write_data *data,
		unsigned int count, unsigned int offset,
		int how)
{
	struct inode *inode = req->wb_context->dentry->d_inode;

	/* Set up the RPC argument and reply structs
	 * NB: take care not to mess about with data->commit et al. */

	data->req = req;
	data->inode = inode = req->wb_context->dentry->d_inode;
	data->cred = req->wb_context->cred;

	data->args.fh     = NFS_FH(inode);
	data->args.offset = req_offset(req) + offset;
	/* pnfs_set_layoutcommit needs this */
	data->mds_offset = data->args.offset;
	data->args.pgbase = req->wb_pgbase + offset;
	data->args.pages  = data->pagevec;
	data->args.count  = count;
	data->args.context = get_nfs_open_context(req->wb_context);
	data->args.lock_context = req->wb_lock_context;
	data->args.stable  = NFS_UNSTABLE;
	switch (how & (FLUSH_STABLE | FLUSH_COND_STABLE)) {
	case 0:
		break;
	case FLUSH_COND_STABLE:
		if (nfs_need_commit(NFS_I(inode)))
			break;
	default:
		data->args.stable = NFS_FILE_SYNC;
	}

	data->res.fattr   = &data->fattr;
	data->res.count   = count;
	data->res.verf    = &data->verf;
	nfs_fattr_init(&data->fattr);
}

static int nfs_do_write(struct nfs_write_data *data,
		const struct rpc_call_ops *call_ops,
		int how)
{
	struct inode *inode = data->args.context->dentry->d_inode;

	return nfs_initiate_write(data, NFS_CLIENT(inode), call_ops, how);
}

static int nfs_do_multiple_writes(struct list_head *head,
		const struct rpc_call_ops *call_ops,
		int how)
{
	struct nfs_write_data *data;
	int ret = 0;

	while (!list_empty(head)) {
		int ret2;

		data = list_entry(head->next, struct nfs_write_data, list);
		list_del_init(&data->list);
		
		ret2 = nfs_do_write(data, call_ops, how);
		 if (ret == 0)
			 ret = ret2;
	}
	return ret;
=======
static void nfs_initiate_write(struct nfs_pgio_header *hdr,
			       struct rpc_message *msg,
			       const struct nfs_rpc_ops *rpc_ops,
			       struct rpc_task_setup *task_setup_data, int how)
{
	int priority = flush_task_priority(how);

	if (IS_SWAPFILE(hdr->inode))
		task_setup_data->flags |= RPC_TASK_SWAPPER;
	task_setup_data->priority = priority;
	rpc_ops->write_setup(hdr, msg, &task_setup_data->rpc_client);
	trace_nfs_initiate_write(hdr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* If a nfs_flush_* function fails, it should remove reqs from @head and
 * call this on each, which will prepare them to be retried on next
 * writeback using standard nfs.
 */
static void nfs_redirty_request(struct nfs_page *req)
{
<<<<<<< HEAD
	struct page *page = req->wb_page;

	nfs_mark_request_dirty(req);
	nfs_unlock_request(req);
	nfs_end_page_writeback(page);
}

/*
 * Generate multiple small requests to write out a single
 * contiguous dirty area on one page.
 */
static int nfs_flush_multi(struct nfs_pageio_descriptor *desc, struct list_head *res)
{
	struct nfs_page *req = nfs_list_entry(desc->pg_list.next);
	struct page *page = req->wb_page;
	struct nfs_write_data *data;
	size_t wsize = desc->pg_bsize, nbytes;
	unsigned int offset;
	int requests = 0;
	int ret = 0;

	nfs_list_remove_request(req);

	if ((desc->pg_ioflags & FLUSH_COND_STABLE) &&
	    (desc->pg_moreio || NFS_I(desc->pg_inode)->ncommit ||
	     desc->pg_count > wsize))
		desc->pg_ioflags &= ~FLUSH_COND_STABLE;


	offset = 0;
	nbytes = desc->pg_count;
	do {
		size_t len = min(nbytes, wsize);

		data = nfs_writedata_alloc(1);
		if (!data)
			goto out_bad;
		data->pagevec[0] = page;
		nfs_write_rpcsetup(req, data, len, offset, desc->pg_ioflags);
		list_add(&data->list, res);
		requests++;
		nbytes -= len;
		offset += len;
	} while (nbytes != 0);
	atomic_set(&req->wb_complete, requests);
	desc->pg_rpc_callops = &nfs_write_partial_ops;
	return ret;

out_bad:
	while (!list_empty(res)) {
		data = list_entry(res->next, struct nfs_write_data, list);
		list_del(&data->list);
		nfs_writedata_release(data);
	}
	nfs_redirty_request(req);
	return -ENOMEM;
}

/*
 * Create an RPC task for the given write request and kick it.
 * The page must have been locked by the caller.
 *
 * It may happen that the page we're passed is not marked dirty.
 * This is the case if nfs_updatepage detects a conflicting request
 * that has been written but not committed.
 */
static int nfs_flush_one(struct nfs_pageio_descriptor *desc, struct list_head *res)
{
	struct nfs_page		*req;
	struct page		**pages;
	struct nfs_write_data	*data;
	struct list_head *head = &desc->pg_list;
	int ret = 0;

	data = nfs_writedata_alloc(nfs_page_array_len(desc->pg_base,
						      desc->pg_count));
	if (!data) {
		while (!list_empty(head)) {
			req = nfs_list_entry(head->next);
			nfs_list_remove_request(req);
			nfs_redirty_request(req);
		}
		ret = -ENOMEM;
		goto out;
	}
	pages = data->pagevec;
	while (!list_empty(head)) {
		req = nfs_list_entry(head->next);
		nfs_list_remove_request(req);
		nfs_list_add_request(req, &data->pages);
		*pages++ = req->wb_page;
	}
	req = nfs_list_entry(data->pages.next);

	if ((desc->pg_ioflags & FLUSH_COND_STABLE) &&
	    (desc->pg_moreio || NFS_I(desc->pg_inode)->ncommit))
		desc->pg_ioflags &= ~FLUSH_COND_STABLE;

	/* Set up the argument struct */
	nfs_write_rpcsetup(req, data, desc->pg_count, 0, desc->pg_ioflags);
	list_add(&data->list, res);
	desc->pg_rpc_callops = &nfs_write_full_ops;
out:
	return ret;
}

int nfs_generic_flush(struct nfs_pageio_descriptor *desc, struct list_head *head)
{
	if (desc->pg_bsize < PAGE_CACHE_SIZE)
		return nfs_flush_multi(desc, head);
	return nfs_flush_one(desc, head);
}

static int nfs_generic_pg_writepages(struct nfs_pageio_descriptor *desc)
{
	LIST_HEAD(head);
	int ret;

	ret = nfs_generic_flush(desc, &head);
	if (ret == 0)
		ret = nfs_do_multiple_writes(&head, desc->pg_rpc_callops,
				desc->pg_ioflags);
	return ret;
}

static const struct nfs_pageio_ops nfs_pageio_write_ops = {
	.pg_test = nfs_generic_pg_test,
	.pg_doio = nfs_generic_pg_writepages,
};

void nfs_pageio_init_write_mds(struct nfs_pageio_descriptor *pgio,
				  struct inode *inode, int ioflags)
{
	nfs_pageio_init(pgio, inode, &nfs_pageio_write_ops,
				NFS_SERVER(inode)->wsize, ioflags);
}

void nfs_pageio_reset_write_mds(struct nfs_pageio_descriptor *pgio)
{
	pgio->pg_ops = &nfs_pageio_write_ops;
	pgio->pg_bsize = NFS_SERVER(pgio->pg_inode)->wsize;
}
EXPORT_SYMBOL_GPL(nfs_pageio_reset_write_mds);

static void nfs_pageio_init_write(struct nfs_pageio_descriptor *pgio,
				  struct inode *inode, int ioflags)
{
	if (!pnfs_pageio_init_write(pgio, inode, ioflags))
		nfs_pageio_init_write_mds(pgio, inode, ioflags);
}

/*
 * Handle a write reply that flushed part of a page.
 */
static void nfs_writeback_done_partial(struct rpc_task *task, void *calldata)
{
	struct nfs_write_data	*data = calldata;

	dprintk("NFS: %5u write(%s/%lld %d@%lld)",
		task->tk_pid,
		data->req->wb_context->dentry->d_inode->i_sb->s_id,
		(long long)
		  NFS_FILEID(data->req->wb_context->dentry->d_inode),
		data->req->wb_bytes, (long long)req_offset(data->req));

	nfs_writeback_done(task, data);
}

static void nfs_writeback_release_partial(void *calldata)
{
	struct nfs_write_data	*data = calldata;
	struct nfs_page		*req = data->req;
	struct page		*page = req->wb_page;
	int status = data->task.tk_status;

	if (status < 0) {
		nfs_set_pageerror(page);
		nfs_context_set_write_error(req->wb_context, status);
		dprintk(", error = %d\n", status);
		goto out;
	}

	if (nfs_write_need_commit(data)) {
		struct inode *inode = page->mapping->host;

		spin_lock(&inode->i_lock);
		if (test_bit(PG_NEED_RESCHED, &req->wb_flags)) {
			/* Do nothing we need to resend the writes */
		} else if (!test_and_set_bit(PG_NEED_COMMIT, &req->wb_flags)) {
			memcpy(&req->wb_verf, &data->verf, sizeof(req->wb_verf));
			dprintk(" defer commit\n");
		} else if (memcmp(&req->wb_verf, &data->verf, sizeof(req->wb_verf))) {
			set_bit(PG_NEED_RESCHED, &req->wb_flags);
			clear_bit(PG_NEED_COMMIT, &req->wb_flags);
			dprintk(" server reboot detected\n");
		}
		spin_unlock(&inode->i_lock);
	} else
		dprintk(" OK\n");

out:
	if (atomic_dec_and_test(&req->wb_complete))
		nfs_writepage_release(req, data);
	nfs_writedata_release(calldata);
}

void nfs_write_prepare(struct rpc_task *task, void *calldata)
{
	struct nfs_write_data *data = calldata;
	NFS_PROTO(data->inode)->write_rpc_prepare(task, data);
}

static const struct rpc_call_ops nfs_write_partial_ops = {
	.rpc_call_prepare = nfs_write_prepare,
	.rpc_call_done = nfs_writeback_done_partial,
	.rpc_release = nfs_writeback_release_partial,
};

/*
 * Handle a write reply that flushes a whole page.
 *
 * FIXME: There is an inherent race with invalidate_inode_pages and
 *	  writebacks since the page->count is kept > 1 for as long
 *	  as the page has a write request pending.
 */
static void nfs_writeback_done_full(struct rpc_task *task, void *calldata)
{
	struct nfs_write_data	*data = calldata;

	nfs_writeback_done(task, data);
}

static void nfs_writeback_release_full(void *calldata)
{
	struct nfs_write_data	*data = calldata;
	int status = data->task.tk_status;

	/* Update attributes as result of writeback. */
	while (!list_empty(&data->pages)) {
		struct nfs_page *req = nfs_list_entry(data->pages.next);
		struct page *page = req->wb_page;

		nfs_list_remove_request(req);

		dprintk("NFS: %5u write (%s/%lld %d@%lld)",
			data->task.tk_pid,
			req->wb_context->dentry->d_inode->i_sb->s_id,
			(long long)NFS_FILEID(req->wb_context->dentry->d_inode),
			req->wb_bytes,
			(long long)req_offset(req));

		if (status < 0) {
			nfs_set_pageerror(page);
			nfs_context_set_write_error(req->wb_context, status);
			dprintk(", error = %d\n", status);
			goto remove_request;
		}

		if (nfs_write_need_commit(data)) {
			memcpy(&req->wb_verf, &data->verf, sizeof(req->wb_verf));
			nfs_mark_request_commit(req, data->lseg);
			dprintk(" marked for commit\n");
			goto next;
		}
		dprintk(" OK\n");
remove_request:
		nfs_inode_remove_request(req);
	next:
		nfs_unlock_request(req);
		nfs_end_page_writeback(page);
	}
	nfs_writedata_release(calldata);
}

static const struct rpc_call_ops nfs_write_full_ops = {
	.rpc_call_prepare = nfs_write_prepare,
	.rpc_call_done = nfs_writeback_done_full,
	.rpc_release = nfs_writeback_release_full,
};

=======
	struct nfs_inode *nfsi = NFS_I(nfs_page_to_inode(req));

	/* Bump the transmission count */
	req->wb_nio++;
	nfs_mark_request_dirty(req);
	atomic_long_inc(&nfsi->redirtied_pages);
	nfs_page_end_writeback(req);
	nfs_release_request(req);
}

static void nfs_async_write_error(struct list_head *head, int error)
{
	struct nfs_page	*req;

	while (!list_empty(head)) {
		req = nfs_list_entry(head->next);
		nfs_list_remove_request(req);
		if (nfs_error_is_fatal_on_server(error))
			nfs_write_error(req, error);
		else
			nfs_redirty_request(req);
	}
}

static void nfs_async_write_reschedule_io(struct nfs_pgio_header *hdr)
{
	nfs_async_write_error(&hdr->pages, 0);
}

static const struct nfs_pgio_completion_ops nfs_async_write_completion_ops = {
	.init_hdr = nfs_async_write_init,
	.error_cleanup = nfs_async_write_error,
	.completion = nfs_write_completion,
	.reschedule_io = nfs_async_write_reschedule_io,
};

void nfs_pageio_init_write(struct nfs_pageio_descriptor *pgio,
			       struct inode *inode, int ioflags, bool force_mds,
			       const struct nfs_pgio_completion_ops *compl_ops)
{
	struct nfs_server *server = NFS_SERVER(inode);
	const struct nfs_pageio_ops *pg_ops = &nfs_pgio_rw_ops;

#ifdef CONFIG_NFS_V4_1
	if (server->pnfs_curr_ld && !force_mds)
		pg_ops = server->pnfs_curr_ld->pg_write_ops;
#endif
	nfs_pageio_init(pgio, inode, pg_ops, compl_ops, &nfs_rw_write_ops,
			server->wsize, ioflags);
}
EXPORT_SYMBOL_GPL(nfs_pageio_init_write);

void nfs_pageio_reset_write_mds(struct nfs_pageio_descriptor *pgio)
{
	struct nfs_pgio_mirror *mirror;

	if (pgio->pg_ops && pgio->pg_ops->pg_cleanup)
		pgio->pg_ops->pg_cleanup(pgio);

	pgio->pg_ops = &nfs_pgio_rw_ops;

	nfs_pageio_stop_mirroring(pgio);

	mirror = &pgio->pg_mirrors[0];
	mirror->pg_bsize = NFS_SERVER(pgio->pg_inode)->wsize;
}
EXPORT_SYMBOL_GPL(nfs_pageio_reset_write_mds);


void nfs_commit_prepare(struct rpc_task *task, void *calldata)
{
	struct nfs_commit_data *data = calldata;

	NFS_PROTO(data->inode)->commit_rpc_prepare(task, data);
}

static void nfs_writeback_check_extend(struct nfs_pgio_header *hdr,
		struct nfs_fattr *fattr)
{
	struct nfs_pgio_args *argp = &hdr->args;
	struct nfs_pgio_res *resp = &hdr->res;
	u64 size = argp->offset + resp->count;

	if (!(fattr->valid & NFS_ATTR_FATTR_SIZE))
		fattr->size = size;
	if (nfs_size_to_loff_t(fattr->size) < i_size_read(hdr->inode)) {
		fattr->valid &= ~NFS_ATTR_FATTR_SIZE;
		return;
	}
	if (size != fattr->size)
		return;
	/* Set attribute barrier */
	nfs_fattr_set_barrier(fattr);
	/* ...and update size */
	fattr->valid |= NFS_ATTR_FATTR_SIZE;
}

void nfs_writeback_update_inode(struct nfs_pgio_header *hdr)
{
	struct nfs_fattr *fattr = &hdr->fattr;
	struct inode *inode = hdr->inode;

	spin_lock(&inode->i_lock);
	nfs_writeback_check_extend(hdr, fattr);
	nfs_post_op_update_inode_force_wcc_locked(inode, fattr);
	spin_unlock(&inode->i_lock);
}
EXPORT_SYMBOL_GPL(nfs_writeback_update_inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * This function is called when the WRITE call is complete.
 */
<<<<<<< HEAD
void nfs_writeback_done(struct rpc_task *task, struct nfs_write_data *data)
{
	struct nfs_writeargs	*argp = &data->args;
	struct nfs_writeres	*resp = &data->res;
	int status;

	dprintk("NFS: %5u nfs_writeback_done (status %d)\n",
		task->tk_pid, task->tk_status);

=======
static int nfs_writeback_done(struct rpc_task *task,
			      struct nfs_pgio_header *hdr,
			      struct inode *inode)
{
	int status;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * ->write_done will attempt to use post-op attributes to detect
	 * conflicting writes by other clients.  A strict interpretation
	 * of close-to-open would allow us to continue caching even if
	 * another writer had changed the file, but some applications
	 * depend on tighter cache coherency when writing.
	 */
<<<<<<< HEAD
	status = NFS_PROTO(data->inode)->write_done(task, data);
	if (status != 0)
		return;
	nfs_add_stats(data->inode, NFSIOS_SERVERWRITTENBYTES, resp->count);

#if defined(CONFIG_NFS_V3) || defined(CONFIG_NFS_V4)
	if (resp->verf->committed < argp->stable && task->tk_status >= 0) {
		/* We tried a write call, but the server did not
		 * commit data to stable storage even though we
		 * requested it.
		 * Note: There is a known bug in Tru64 < 5.0 in which
		 *	 the server reports NFS_DATA_SYNC, but performs
		 *	 NFS_FILE_SYNC. We therefore implement this checking
		 *	 as a dprintk() in order to avoid filling syslog.
		 */
		static unsigned long    complain;

		/* Note this will print the MDS for a DS write */
		if (time_before(complain, jiffies)) {
			dprintk("NFS:       faulty NFS server %s:"
				" (committed = %d) != (stable = %d)\n",
				NFS_SERVER(data->inode)->nfs_client->cl_hostname,
				resp->verf->committed, argp->stable);
			complain = jiffies + 300 * HZ;
		}
	}
#endif
	/* Is this a short write? */
	if (task->tk_status >= 0 && resp->count < argp->count) {
		static unsigned long    complain;

		nfs_inc_stats(data->inode, NFSIOS_SHORTWRITE);

		/* Has the server at least made some progress? */
		if (resp->count != 0) {
			/* Was this an NFSv2 write or an NFSv3 stable write? */
			if (resp->verf->committed != NFS_UNSTABLE) {
				/* Resend from where the server left off */
				data->mds_offset += resp->count;
				argp->offset += resp->count;
				argp->pgbase += resp->count;
				argp->count -= resp->count;
			} else {
				/* Resend as a stable write in order to avoid
				 * headaches in the case of a server crash.
				 */
				argp->stable = NFS_FILE_SYNC;
			}
			rpc_restart_call_prepare(task);
			return;
		}
		if (time_before(complain, jiffies)) {
			printk(KERN_WARNING
			       "NFS: Server wrote zero bytes, expected %u.\n",
					argp->count);
			complain = jiffies + 300 * HZ;
		}
		/* Can't do anything about it except throw an error. */
		task->tk_status = -EIO;
	}
	return;
}


#if defined(CONFIG_NFS_V3) || defined(CONFIG_NFS_V4)
static int nfs_commit_set_lock(struct nfs_inode *nfsi, int may_wait)
{
	int ret;

	if (!test_and_set_bit(NFS_INO_COMMIT, &nfsi->flags))
		return 1;
	if (!may_wait)
		return 0;
	ret = out_of_line_wait_on_bit_lock(&nfsi->flags,
				NFS_INO_COMMIT,
				nfs_wait_bit_killable,
				TASK_KILLABLE);
	return (ret < 0) ? ret : 1;
}

void nfs_commit_clear_lock(struct nfs_inode *nfsi)
{
	clear_bit(NFS_INO_COMMIT, &nfsi->flags);
	smp_mb__after_clear_bit();
	wake_up_bit(&nfsi->flags, NFS_INO_COMMIT);
}
EXPORT_SYMBOL_GPL(nfs_commit_clear_lock);

void nfs_commitdata_release(void *data)
{
	struct nfs_write_data *wdata = data;

	put_nfs_open_context(wdata->args.context);
	nfs_commit_free(wdata);
}
EXPORT_SYMBOL_GPL(nfs_commitdata_release);

int nfs_initiate_commit(struct nfs_write_data *data, struct rpc_clnt *clnt,
			const struct rpc_call_ops *call_ops,
			int how)
=======
	status = NFS_PROTO(inode)->write_done(task, hdr);
	if (status != 0)
		return status;

	nfs_add_stats(inode, NFSIOS_SERVERWRITTENBYTES, hdr->res.count);
	trace_nfs_writeback_done(task, hdr);

	if (task->tk_status >= 0) {
		enum nfs3_stable_how committed = hdr->res.verf->committed;

		if (committed == NFS_UNSTABLE) {
			/*
			 * We have some uncommitted data on the server at
			 * this point, so ensure that we keep track of that
			 * fact irrespective of what later writes do.
			 */
			set_bit(NFS_IOHDR_UNSTABLE_WRITES, &hdr->flags);
		}

		if (committed < hdr->args.stable) {
			/* We tried a write call, but the server did not
			 * commit data to stable storage even though we
			 * requested it.
			 * Note: There is a known bug in Tru64 < 5.0 in which
			 *	 the server reports NFS_DATA_SYNC, but performs
			 *	 NFS_FILE_SYNC. We therefore implement this checking
			 *	 as a dprintk() in order to avoid filling syslog.
			 */
			static unsigned long    complain;

			/* Note this will print the MDS for a DS write */
			if (time_before(complain, jiffies)) {
				dprintk("NFS:       faulty NFS server %s:"
					" (committed = %d) != (stable = %d)\n",
					NFS_SERVER(inode)->nfs_client->cl_hostname,
					committed, hdr->args.stable);
				complain = jiffies + 300 * HZ;
			}
		}
	}

	/* Deal with the suid/sgid bit corner case */
	if (nfs_should_remove_suid(inode)) {
		spin_lock(&inode->i_lock);
		nfs_set_cache_invalid(inode, NFS_INO_INVALID_MODE);
		spin_unlock(&inode->i_lock);
	}
	return 0;
}

/*
 * This function is called when the WRITE call is complete.
 */
static void nfs_writeback_result(struct rpc_task *task,
				 struct nfs_pgio_header *hdr)
{
	struct nfs_pgio_args	*argp = &hdr->args;
	struct nfs_pgio_res	*resp = &hdr->res;

	if (resp->count < argp->count) {
		static unsigned long    complain;

		/* This a short write! */
		nfs_inc_stats(hdr->inode, NFSIOS_SHORTWRITE);

		/* Has the server at least made some progress? */
		if (resp->count == 0) {
			if (time_before(complain, jiffies)) {
				printk(KERN_WARNING
				       "NFS: Server wrote zero bytes, expected %u.\n",
				       argp->count);
				complain = jiffies + 300 * HZ;
			}
			nfs_set_pgio_error(hdr, -EIO, argp->offset);
			task->tk_status = -EIO;
			return;
		}

		/* For non rpc-based layout drivers, retry-through-MDS */
		if (!task->tk_ops) {
			hdr->pnfs_error = -EAGAIN;
			return;
		}

		/* Was this an NFSv2 write or an NFSv3 stable write? */
		if (resp->verf->committed != NFS_UNSTABLE) {
			/* Resend from where the server left off */
			hdr->mds_offset += resp->count;
			argp->offset += resp->count;
			argp->pgbase += resp->count;
			argp->count -= resp->count;
		} else {
			/* Resend as a stable write in order to avoid
			 * headaches in the case of a server crash.
			 */
			argp->stable = NFS_FILE_SYNC;
		}
		resp->count = 0;
		resp->verf->committed = 0;
		rpc_restart_call_prepare(task);
	}
}

static int wait_on_commit(struct nfs_mds_commit_info *cinfo)
{
	return wait_var_event_killable(&cinfo->rpcs_out,
				       !atomic_read(&cinfo->rpcs_out));
}

void nfs_commit_begin(struct nfs_mds_commit_info *cinfo)
{
	atomic_inc(&cinfo->rpcs_out);
}

bool nfs_commit_end(struct nfs_mds_commit_info *cinfo)
{
	if (atomic_dec_and_test(&cinfo->rpcs_out)) {
		wake_up_var(&cinfo->rpcs_out);
		return true;
	}
	return false;
}

void nfs_commitdata_release(struct nfs_commit_data *data)
{
	put_nfs_open_context(data->context);
	nfs_commit_free(data);
}
EXPORT_SYMBOL_GPL(nfs_commitdata_release);

int nfs_initiate_commit(struct rpc_clnt *clnt, struct nfs_commit_data *data,
			const struct nfs_rpc_ops *nfs_ops,
			const struct rpc_call_ops *call_ops,
			int how, int flags)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct rpc_task *task;
	int priority = flush_task_priority(how);
	struct rpc_message msg = {
		.rpc_argp = &data->args,
		.rpc_resp = &data->res,
		.rpc_cred = data->cred,
	};
	struct rpc_task_setup task_setup_data = {
		.task = &data->task,
		.rpc_client = clnt,
		.rpc_message = &msg,
		.callback_ops = call_ops,
		.callback_data = data,
		.workqueue = nfsiod_workqueue,
<<<<<<< HEAD
		.flags = RPC_TASK_ASYNC,
		.priority = priority,
	};
	/* Set up the initial task struct.  */
	NFS_PROTO(data->inode)->commit_setup(data, &msg);

	dprintk("NFS: %5u initiated commit call\n", data->task.tk_pid);
=======
		.flags = RPC_TASK_ASYNC | flags,
		.priority = priority,
	};

	if (nfs_server_capable(data->inode, NFS_CAP_MOVEABLE))
		task_setup_data.flags |= RPC_TASK_MOVEABLE;

	/* Set up the initial task struct.  */
	nfs_ops->commit_setup(data, &msg, &task_setup_data.rpc_client);
	trace_nfs_initiate_commit(data);

	dprintk("NFS: initiated commit call\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	task = rpc_run_task(&task_setup_data);
	if (IS_ERR(task))
		return PTR_ERR(task);
	if (how & FLUSH_SYNC)
		rpc_wait_for_completion_task(task);
	rpc_put_task(task);
	return 0;
}
EXPORT_SYMBOL_GPL(nfs_initiate_commit);

<<<<<<< HEAD
/*
 * Set up the argument/result storage required for the RPC call.
 */
void nfs_init_commit(struct nfs_write_data *data,
			    struct list_head *head,
			    struct pnfs_layout_segment *lseg)
{
	struct nfs_page *first = nfs_list_entry(head->next);
	struct inode *inode = first->wb_context->dentry->d_inode;
=======
static loff_t nfs_get_lwb(struct list_head *head)
{
	loff_t lwb = 0;
	struct nfs_page *req;

	list_for_each_entry(req, head, wb_list)
		if (lwb < (req_offset(req) + req->wb_bytes))
			lwb = req_offset(req) + req->wb_bytes;

	return lwb;
}

/*
 * Set up the argument/result storage required for the RPC call.
 */
void nfs_init_commit(struct nfs_commit_data *data,
		     struct list_head *head,
		     struct pnfs_layout_segment *lseg,
		     struct nfs_commit_info *cinfo)
{
	struct nfs_page *first;
	struct nfs_open_context *ctx;
	struct inode *inode;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Set up the RPC argument and reply structs
	 * NB: take care not to mess about with data->commit et al. */

<<<<<<< HEAD
	list_splice_init(head, &data->pages);

	data->inode	  = inode;
	data->cred	  = first->wb_context->cred;
	data->lseg	  = lseg; /* reference transferred */
	data->mds_ops     = &nfs_commit_ops;
=======
	if (head)
		list_splice_init(head, &data->pages);

	first = nfs_list_entry(data->pages.next);
	ctx = nfs_req_openctx(first);
	inode = d_inode(ctx->dentry);

	data->inode	  = inode;
	data->cred	  = ctx->cred;
	data->lseg	  = lseg; /* reference transferred */
	/* only set lwb for pnfs commit */
	if (lseg)
		data->lwb = nfs_get_lwb(&data->pages);
	data->mds_ops     = &nfs_commit_ops;
	data->completion_ops = cinfo->completion_ops;
	data->dreq	  = cinfo->dreq;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	data->args.fh     = NFS_FH(data->inode);
	/* Note: we always request a commit of the entire inode */
	data->args.offset = 0;
	data->args.count  = 0;
<<<<<<< HEAD
	data->args.context = get_nfs_open_context(first->wb_context);
	data->res.count   = 0;
	data->res.fattr   = &data->fattr;
	data->res.verf    = &data->verf;
	nfs_fattr_init(&data->fattr);
=======
	data->context     = get_nfs_open_context(ctx);
	data->res.fattr   = &data->fattr;
	data->res.verf    = &data->verf;
	nfs_fattr_init(&data->fattr);
	nfs_commit_begin(cinfo->mds);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(nfs_init_commit);

void nfs_retry_commit(struct list_head *page_list,
<<<<<<< HEAD
		      struct pnfs_layout_segment *lseg)
=======
		      struct pnfs_layout_segment *lseg,
		      struct nfs_commit_info *cinfo,
		      u32 ds_commit_idx)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct nfs_page *req;

	while (!list_empty(page_list)) {
		req = nfs_list_entry(page_list->next);
		nfs_list_remove_request(req);
<<<<<<< HEAD
		nfs_mark_request_commit(req, lseg);
		dec_zone_page_state(req->wb_page, NR_UNSTABLE_NFS);
		dec_bdi_stat(req->wb_page->mapping->backing_dev_info,
			     BDI_RECLAIMABLE);
		nfs_unlock_request(req);
=======
		nfs_mark_request_commit(req, lseg, cinfo, ds_commit_idx);
		nfs_folio_clear_commit(nfs_page_to_folio(req));
		nfs_unlock_and_release_request(req);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}
EXPORT_SYMBOL_GPL(nfs_retry_commit);

<<<<<<< HEAD
=======
static void nfs_commit_resched_write(struct nfs_commit_info *cinfo,
				     struct nfs_page *req)
{
	struct folio *folio = nfs_page_to_folio(req);

	filemap_dirty_folio(folio_mapping(folio), folio);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Commit dirty pages
 */
static int
<<<<<<< HEAD
nfs_commit_list(struct inode *inode, struct list_head *head, int how)
{
	struct nfs_write_data	*data;

	data = nfs_commitdata_alloc();

	if (!data)
		goto out_bad;

	/* Set up the argument struct */
	nfs_init_commit(data, head, NULL);
	return nfs_initiate_commit(data, NFS_CLIENT(inode), data->mds_ops, how);
 out_bad:
	nfs_retry_commit(head, NULL);
	nfs_commit_clear_lock(NFS_I(inode));
	return -ENOMEM;
=======
nfs_commit_list(struct inode *inode, struct list_head *head, int how,
		struct nfs_commit_info *cinfo)
{
	struct nfs_commit_data	*data;
	unsigned short task_flags = 0;

	/* another commit raced with us */
	if (list_empty(head))
		return 0;

	data = nfs_commitdata_alloc();
	if (!data) {
		nfs_retry_commit(head, NULL, cinfo, -1);
		return -ENOMEM;
	}

	/* Set up the argument struct */
	nfs_init_commit(data, head, NULL, cinfo);
	if (NFS_SERVER(inode)->nfs_client->cl_minorversion)
		task_flags = RPC_TASK_MOVEABLE;
	return nfs_initiate_commit(NFS_CLIENT(inode), data, NFS_PROTO(inode),
				   data->mds_ops, how,
				   RPC_TASK_CRED_NOREF | task_flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * COMMIT call returned
 */
static void nfs_commit_done(struct rpc_task *task, void *calldata)
{
<<<<<<< HEAD
	struct nfs_write_data	*data = calldata;

        dprintk("NFS: %5u nfs_commit_done (status %d)\n",
                                task->tk_pid, task->tk_status);

	/* Call the NFS version-specific code */
	NFS_PROTO(data->inode)->commit_done(task, data);
}

void nfs_commit_release_pages(struct nfs_write_data *data)
{
	struct nfs_page	*req;
	int status = data->task.tk_status;
=======
	struct nfs_commit_data	*data = calldata;

	/* Call the NFS version-specific code */
	NFS_PROTO(data->inode)->commit_done(task, data);
	trace_nfs_commit_done(task, data);
}

static void nfs_commit_release_pages(struct nfs_commit_data *data)
{
	const struct nfs_writeverf *verf = data->res.verf;
	struct nfs_page	*req;
	int status = data->task.tk_status;
	struct nfs_commit_info cinfo;
	struct nfs_server *nfss;
	struct folio *folio;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	while (!list_empty(&data->pages)) {
		req = nfs_list_entry(data->pages.next);
		nfs_list_remove_request(req);
<<<<<<< HEAD
		nfs_clear_page_commit(req->wb_page);

		dprintk("NFS:       commit (%s/%lld %d@%lld)",
			req->wb_context->dentry->d_sb->s_id,
			(long long)NFS_FILEID(req->wb_context->dentry->d_inode),
			req->wb_bytes,
			(long long)req_offset(req));
		if (status < 0) {
			nfs_context_set_write_error(req->wb_context, status);
			nfs_inode_remove_request(req);
			dprintk(", error = %d\n", status);
=======
		folio = nfs_page_to_folio(req);
		nfs_folio_clear_commit(folio);

		dprintk("NFS:       commit (%s/%llu %d@%lld)",
			nfs_req_openctx(req)->dentry->d_sb->s_id,
			(unsigned long long)NFS_FILEID(d_inode(nfs_req_openctx(req)->dentry)),
			req->wb_bytes,
			(long long)req_offset(req));
		if (status < 0) {
			if (folio) {
				trace_nfs_commit_error(data->inode, req,
						       status);
				nfs_mapping_set_error(folio, status);
				nfs_inode_remove_request(req);
			}
			dprintk_cont(", error = %d\n", status);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto next;
		}

		/* Okay, COMMIT succeeded, apparently. Check the verifier
		 * returned by the server against all stored verfs. */
<<<<<<< HEAD
		if (!memcmp(req->wb_verf.verifier, data->verf.verifier, sizeof(data->verf.verifier))) {
			/* We have a match */
			nfs_inode_remove_request(req);
			dprintk(" OK\n");
			goto next;
		}
		/* We have a mismatch. Write the page again */
		dprintk(" mismatch\n");
		nfs_mark_request_dirty(req);
	next:
		nfs_unlock_request(req);
	}
}
EXPORT_SYMBOL_GPL(nfs_commit_release_pages);

static void nfs_commit_release(void *calldata)
{
	struct nfs_write_data *data = calldata;

	nfs_commit_release_pages(data);
	nfs_commit_clear_lock(NFS_I(data->inode));
=======
		if (nfs_write_match_verf(verf, req)) {
			/* We have a match */
			if (folio)
				nfs_inode_remove_request(req);
			dprintk_cont(" OK\n");
			goto next;
		}
		/* We have a mismatch. Write the page again */
		dprintk_cont(" mismatch\n");
		nfs_mark_request_dirty(req);
		atomic_long_inc(&NFS_I(data->inode)->redirtied_pages);
	next:
		nfs_unlock_and_release_request(req);
		/* Latency breaker */
		cond_resched();
	}
	nfss = NFS_SERVER(data->inode);
	if (atomic_long_read(&nfss->writeback) < NFS_CONGESTION_OFF_THRESH)
		nfss->write_congested = 0;

	nfs_init_cinfo(&cinfo, data->inode, data->dreq);
	nfs_commit_end(cinfo.mds);
}

static void nfs_commit_release(void *calldata)
{
	struct nfs_commit_data *data = calldata;

	data->completion_ops->completion(data);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	nfs_commitdata_release(calldata);
}

static const struct rpc_call_ops nfs_commit_ops = {
<<<<<<< HEAD
	.rpc_call_prepare = nfs_write_prepare,
=======
	.rpc_call_prepare = nfs_commit_prepare,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.rpc_call_done = nfs_commit_done,
	.rpc_release = nfs_commit_release,
};

<<<<<<< HEAD
int nfs_commit_inode(struct inode *inode, int how)
{
	LIST_HEAD(head);
	int may_wait = how & FLUSH_SYNC;
	int res;

	res = nfs_commit_set_lock(NFS_I(inode), may_wait);
	if (res <= 0)
		goto out_mark_dirty;
	res = nfs_scan_commit(inode, &head);
	if (res) {
		int error;

		error = pnfs_commit_list(inode, &head, how);
		if (error == PNFS_NOT_ATTEMPTED)
			error = nfs_commit_list(inode, &head, how);
		if (error < 0)
			return error;
		if (!may_wait)
			goto out_mark_dirty;
		error = wait_on_bit(&NFS_I(inode)->flags,
				NFS_INO_COMMIT,
				nfs_wait_bit_killable,
				TASK_KILLABLE);
		if (error < 0)
			return error;
	} else
		nfs_commit_clear_lock(NFS_I(inode));
	return res;
	/* Note: If we exit without ensuring that the commit is complete,
	 * we must mark the inode as dirty. Otherwise, future calls to
	 * sync_inode() with the WB_SYNC_ALL flag set will fail to ensure
	 * that the data is on the disk.
	 */
out_mark_dirty:
	__mark_inode_dirty(inode, I_DIRTY_DATASYNC);
	return res;
}

static int nfs_commit_unstable_pages(struct inode *inode, struct writeback_control *wbc)
=======
static const struct nfs_commit_completion_ops nfs_commit_completion_ops = {
	.completion = nfs_commit_release_pages,
	.resched_write = nfs_commit_resched_write,
};

int nfs_generic_commit_list(struct inode *inode, struct list_head *head,
			    int how, struct nfs_commit_info *cinfo)
{
	int status;

	status = pnfs_commit_list(inode, head, how, cinfo);
	if (status == PNFS_NOT_ATTEMPTED)
		status = nfs_commit_list(inode, head, how, cinfo);
	return status;
}

static int __nfs_commit_inode(struct inode *inode, int how,
		struct writeback_control *wbc)
{
	LIST_HEAD(head);
	struct nfs_commit_info cinfo;
	int may_wait = how & FLUSH_SYNC;
	int ret, nscan;

	how &= ~FLUSH_SYNC;
	nfs_init_cinfo_from_inode(&cinfo, inode);
	nfs_commit_begin(cinfo.mds);
	for (;;) {
		ret = nscan = nfs_scan_commit(inode, &head, &cinfo);
		if (ret <= 0)
			break;
		ret = nfs_generic_commit_list(inode, &head, how, &cinfo);
		if (ret < 0)
			break;
		ret = 0;
		if (wbc && wbc->sync_mode == WB_SYNC_NONE) {
			if (nscan < wbc->nr_to_write)
				wbc->nr_to_write -= nscan;
			else
				wbc->nr_to_write = 0;
		}
		if (nscan < INT_MAX)
			break;
		cond_resched();
	}
	nfs_commit_end(cinfo.mds);
	if (ret || !may_wait)
		return ret;
	return wait_on_commit(cinfo.mds);
}

int nfs_commit_inode(struct inode *inode, int how)
{
	return __nfs_commit_inode(inode, how, NULL);
}
EXPORT_SYMBOL_GPL(nfs_commit_inode);

int nfs_write_inode(struct inode *inode, struct writeback_control *wbc)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct nfs_inode *nfsi = NFS_I(inode);
	int flags = FLUSH_SYNC;
	int ret = 0;

<<<<<<< HEAD
	/* no commits means nothing needs to be done */
	if (!nfsi->ncommit)
		return ret;

	if (wbc->sync_mode == WB_SYNC_NONE) {
		/* Don't commit yet if this is a non-blocking flush and there
		 * are a lot of outstanding writes for this mapping.
		 */
		if (nfsi->ncommit <= (nfsi->npages >> 1))
=======
	if (wbc->sync_mode == WB_SYNC_NONE) {
		/* no commits means nothing needs to be done */
		if (!atomic_long_read(&nfsi->commit_info.ncommit))
			goto check_requests_outstanding;

		/* Don't commit yet if this is a non-blocking flush and there
		 * are a lot of outstanding writes for this mapping.
		 */
		if (mapping_tagged(inode->i_mapping, PAGECACHE_TAG_WRITEBACK))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto out_mark_dirty;

		/* don't wait for the COMMIT response */
		flags = 0;
	}

<<<<<<< HEAD
	ret = nfs_commit_inode(inode, flags);
	if (ret >= 0) {
		if (wbc->sync_mode == WB_SYNC_NONE) {
			if (ret < wbc->nr_to_write)
				wbc->nr_to_write -= ret;
			else
				wbc->nr_to_write = 0;
		}
		return 0;
	}
=======
	ret = __nfs_commit_inode(inode, flags, wbc);
	if (!ret) {
		if (flags & FLUSH_SYNC)
			return 0;
	} else if (atomic_long_read(&nfsi->commit_info.ncommit))
		goto out_mark_dirty;

check_requests_outstanding:
	if (!atomic_read(&nfsi->commit_info.rpcs_out))
		return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out_mark_dirty:
	__mark_inode_dirty(inode, I_DIRTY_DATASYNC);
	return ret;
}
<<<<<<< HEAD
#else
static int nfs_commit_unstable_pages(struct inode *inode, struct writeback_control *wbc)
{
	return 0;
}
#endif

int nfs_write_inode(struct inode *inode, struct writeback_control *wbc)
{
	int ret;

	ret = nfs_commit_unstable_pages(inode, wbc);
	if (ret >= 0 && test_bit(NFS_INO_LAYOUTCOMMIT, &NFS_I(inode)->flags)) {
		int status;
		bool sync = true;

		if (wbc->sync_mode == WB_SYNC_NONE)
			sync = false;

		status = pnfs_layoutcommit_inode(inode, sync);
		if (status < 0)
			return status;
	}
	return ret;
}
=======
EXPORT_SYMBOL_GPL(nfs_write_inode);

/*
 * Wrapper for filemap_write_and_wait_range()
 *
 * Needed for pNFS in order to ensure data becomes visible to the
 * client.
 */
int nfs_filemap_write_and_wait_range(struct address_space *mapping,
		loff_t lstart, loff_t lend)
{
	int ret;

	ret = filemap_write_and_wait_range(mapping, lstart, lend);
	if (ret == 0)
		ret = pnfs_sync_inode(mapping->host, true);
	return ret;
}
EXPORT_SYMBOL_GPL(nfs_filemap_write_and_wait_range);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * flush the inode to disk.
 */
int nfs_wb_all(struct inode *inode)
{
<<<<<<< HEAD
	struct writeback_control wbc = {
		.sync_mode = WB_SYNC_ALL,
		.nr_to_write = LONG_MAX,
		.range_start = 0,
		.range_end = LLONG_MAX,
	};

	return sync_inode(inode, &wbc);
}

int nfs_wb_page_cancel(struct inode *inode, struct page *page)
=======
	int ret;

	trace_nfs_writeback_inode_enter(inode);

	ret = filemap_write_and_wait(inode->i_mapping);
	if (ret)
		goto out;
	ret = nfs_commit_inode(inode, FLUSH_SYNC);
	if (ret < 0)
		goto out;
	pnfs_sync_inode(inode, true);
	ret = 0;

out:
	trace_nfs_writeback_inode_exit(inode, ret);
	return ret;
}
EXPORT_SYMBOL_GPL(nfs_wb_all);

int nfs_wb_folio_cancel(struct inode *inode, struct folio *folio)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct nfs_page *req;
	int ret = 0;

<<<<<<< HEAD
	BUG_ON(!PageLocked(page));
	for (;;) {
		wait_on_page_writeback(page);
		req = nfs_page_find_request(page);
		if (req == NULL)
			break;
		if (nfs_lock_request_dontget(req)) {
			nfs_clear_request_commit(req);
			nfs_inode_remove_request(req);
			/*
			 * In case nfs_inode_remove_request has marked the
			 * page as being dirty
			 */
			cancel_dirty_page(page, PAGE_CACHE_SIZE);
			nfs_unlock_request(req);
			break;
		}
		ret = nfs_wait_on_request(req);
		nfs_release_request(req);
		if (ret < 0)
			break;
	}
	return ret;
}

/*
 * Write back all requests on one page - we do this before reading it.
 */
int nfs_wb_page(struct inode *inode, struct page *page)
{
	loff_t range_start = page_offset(page);
	loff_t range_end = range_start + (loff_t)(PAGE_CACHE_SIZE - 1);
=======
	folio_wait_writeback(folio);

	/* blocking call to cancel all requests and join to a single (head)
	 * request */
	req = nfs_lock_and_join_requests(folio);

	if (IS_ERR(req)) {
		ret = PTR_ERR(req);
	} else if (req) {
		/* all requests from this folio have been cancelled by
		 * nfs_lock_and_join_requests, so just remove the head
		 * request from the inode / page_private pointer and
		 * release it */
		nfs_inode_remove_request(req);
		nfs_unlock_and_release_request(req);
	}

	return ret;
}

/**
 * nfs_wb_folio - Write back all requests on one page
 * @inode: pointer to page
 * @folio: pointer to folio
 *
 * Assumes that the folio has been locked by the caller, and will
 * not unlock it.
 */
int nfs_wb_folio(struct inode *inode, struct folio *folio)
{
	loff_t range_start = folio_file_pos(folio);
	loff_t range_end = range_start + (loff_t)folio_size(folio) - 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct writeback_control wbc = {
		.sync_mode = WB_SYNC_ALL,
		.nr_to_write = 0,
		.range_start = range_start,
		.range_end = range_end,
	};
	int ret;

<<<<<<< HEAD
	for (;;) {
		wait_on_page_writeback(page);
		if (clear_page_dirty_for_io(page)) {
			ret = nfs_writepage_locked(page, &wbc);
=======
	trace_nfs_writeback_folio(inode, folio);

	for (;;) {
		folio_wait_writeback(folio);
		if (folio_clear_dirty_for_io(folio)) {
			ret = nfs_writepage_locked(folio, &wbc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (ret < 0)
				goto out_error;
			continue;
		}
<<<<<<< HEAD
		if (!PagePrivate(page))
=======
		ret = 0;
		if (!folio_test_private(folio))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		ret = nfs_commit_inode(inode, FLUSH_SYNC);
		if (ret < 0)
			goto out_error;
	}
<<<<<<< HEAD
	return 0;
out_error:
=======
out_error:
	trace_nfs_writeback_folio_done(inode, folio, ret);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

#ifdef CONFIG_MIGRATION
<<<<<<< HEAD
int nfs_migrate_page(struct address_space *mapping, struct page *newpage,
		struct page *page, enum migrate_mode mode)
{
	/*
	 * If PagePrivate is set, then the page is currently associated with
=======
int nfs_migrate_folio(struct address_space *mapping, struct folio *dst,
		struct folio *src, enum migrate_mode mode)
{
	/*
	 * If the private flag is set, the folio is currently associated with
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * an in-progress read or write request. Don't try to migrate it.
	 *
	 * FIXME: we could do this in principle, but we'll need a way to ensure
	 *        that we can safely release the inode reference while holding
<<<<<<< HEAD
	 *        the page lock.
	 */
	if (PagePrivate(page))
		return -EBUSY;

	nfs_fscache_release_page(page, GFP_KERNEL);

	return migrate_page(mapping, newpage, page, mode);
=======
	 *        the folio lock.
	 */
	if (folio_test_private(src))
		return -EBUSY;

	if (folio_test_fscache(src)) {
		if (mode == MIGRATE_ASYNC)
			return -EBUSY;
		folio_wait_fscache(src);
	}

	return migrate_folio(mapping, dst, src, mode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
#endif

int __init nfs_init_writepagecache(void)
{
	nfs_wdata_cachep = kmem_cache_create("nfs_write_data",
<<<<<<< HEAD
					     sizeof(struct nfs_write_data),
=======
					     sizeof(struct nfs_pgio_header),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					     0, SLAB_HWCACHE_ALIGN,
					     NULL);
	if (nfs_wdata_cachep == NULL)
		return -ENOMEM;

	nfs_wdata_mempool = mempool_create_slab_pool(MIN_POOL_WRITE,
						     nfs_wdata_cachep);
	if (nfs_wdata_mempool == NULL)
		goto out_destroy_write_cache;

<<<<<<< HEAD
	nfs_commit_mempool = mempool_create_slab_pool(MIN_POOL_COMMIT,
						      nfs_wdata_cachep);
	if (nfs_commit_mempool == NULL)
		goto out_destroy_write_mempool;

=======
	nfs_cdata_cachep = kmem_cache_create("nfs_commit_data",
					     sizeof(struct nfs_commit_data),
					     0, SLAB_HWCACHE_ALIGN,
					     NULL);
	if (nfs_cdata_cachep == NULL)
		goto out_destroy_write_mempool;

	nfs_commit_mempool = mempool_create_slab_pool(MIN_POOL_COMMIT,
						      nfs_cdata_cachep);
	if (nfs_commit_mempool == NULL)
		goto out_destroy_commit_cache;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * NFS congestion size, scale with available memory.
	 *
	 *  64MB:    8192k
	 * 128MB:   11585k
	 * 256MB:   16384k
	 * 512MB:   23170k
	 *   1GB:   32768k
	 *   2GB:   46340k
	 *   4GB:   65536k
	 *   8GB:   92681k
	 *  16GB:  131072k
	 *
	 * This allows larger machines to have larger/more transfers.
	 * Limit the default to 256M
	 */
<<<<<<< HEAD
	nfs_congestion_kb = (16*int_sqrt(totalram_pages)) << (PAGE_SHIFT-10);
=======
	nfs_congestion_kb = (16*int_sqrt(totalram_pages())) << (PAGE_SHIFT-10);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (nfs_congestion_kb > 256*1024)
		nfs_congestion_kb = 256*1024;

	return 0;

<<<<<<< HEAD
=======
out_destroy_commit_cache:
	kmem_cache_destroy(nfs_cdata_cachep);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out_destroy_write_mempool:
	mempool_destroy(nfs_wdata_mempool);
out_destroy_write_cache:
	kmem_cache_destroy(nfs_wdata_cachep);
	return -ENOMEM;
}

void nfs_destroy_writepagecache(void)
{
	mempool_destroy(nfs_commit_mempool);
<<<<<<< HEAD
=======
	kmem_cache_destroy(nfs_cdata_cachep);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mempool_destroy(nfs_wdata_mempool);
	kmem_cache_destroy(nfs_wdata_cachep);
}

<<<<<<< HEAD
=======
static const struct nfs_rw_ops nfs_rw_write_ops = {
	.rw_alloc_header	= nfs_writehdr_alloc,
	.rw_free_header		= nfs_writehdr_free,
	.rw_done		= nfs_writeback_done,
	.rw_result		= nfs_writeback_result,
	.rw_initiate		= nfs_initiate_write,
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
