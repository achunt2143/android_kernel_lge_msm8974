<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * linux/fs/nfs/read.c
 *
 * Block I/O for NFS
 *
 * Partial copy of Linus' read cache modifications to fs/nfs/file.c
 * modified for async RPC by okir@monad.swb.de
 */

#include <linux/time.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/fcntl.h>
#include <linux/stat.h>
#include <linux/mm.h>
#include <linux/slab.h>
<<<<<<< HEAD
=======
#include <linux/task_io_accounting_ops.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/pagemap.h>
#include <linux/sunrpc/clnt.h>
#include <linux/nfs_fs.h>
#include <linux/nfs_page.h>
#include <linux/module.h>

<<<<<<< HEAD
#include "pnfs.h"

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "nfs4_fs.h"
#include "internal.h"
#include "iostat.h"
#include "fscache.h"
<<<<<<< HEAD

#define NFSDBG_FACILITY		NFSDBG_PAGECACHE

static const struct nfs_pageio_ops nfs_pageio_read_ops;
static const struct rpc_call_ops nfs_read_partial_ops;
static const struct rpc_call_ops nfs_read_full_ops;

static struct kmem_cache *nfs_rdata_cachep;

struct nfs_read_data *nfs_readdata_alloc(unsigned int pagecount)
{
	struct nfs_read_data *p;

	p = kmem_cache_zalloc(nfs_rdata_cachep, GFP_KERNEL);
	if (p) {
		INIT_LIST_HEAD(&p->pages);
		p->npages = pagecount;
		if (pagecount <= ARRAY_SIZE(p->page_array))
			p->pagevec = p->page_array;
		else {
			p->pagevec = kcalloc(pagecount, sizeof(struct page *), GFP_KERNEL);
			if (!p->pagevec) {
				kmem_cache_free(nfs_rdata_cachep, p);
				p = NULL;
			}
		}
	}
	return p;
}

void nfs_readdata_free(struct nfs_read_data *p)
{
	if (p && (p->pagevec != &p->page_array[0]))
		kfree(p->pagevec);
	kmem_cache_free(nfs_rdata_cachep, p);
}

void nfs_readdata_release(struct nfs_read_data *rdata)
{
	put_nfs_open_context(rdata->args.context);
	nfs_readdata_free(rdata);
}

static
int nfs_return_empty_page(struct page *page)
{
	zero_user(page, 0, PAGE_CACHE_SIZE);
	SetPageUptodate(page);
	unlock_page(page);
	return 0;
}

static void nfs_readpage_truncate_uninitialised_page(struct nfs_read_data *data)
{
	unsigned int remainder = data->args.count - data->res.count;
	unsigned int base = data->args.pgbase + data->res.count;
	unsigned int pglen;
	struct page **pages;

	if (data->res.eof == 0 || remainder == 0)
		return;
	/*
	 * Note: "remainder" can never be negative, since we check for
	 * 	this in the XDR code.
	 */
	pages = &data->args.pages[base >> PAGE_CACHE_SHIFT];
	base &= ~PAGE_CACHE_MASK;
	pglen = PAGE_CACHE_SIZE - base;
	for (;;) {
		if (remainder <= pglen) {
			zero_user(*pages, base, remainder);
			break;
		}
		zero_user(*pages, base, pglen);
		pages++;
		remainder -= pglen;
		pglen = PAGE_CACHE_SIZE;
		base = 0;
	}
}

void nfs_pageio_init_read_mds(struct nfs_pageio_descriptor *pgio,
		struct inode *inode)
{
	nfs_pageio_init(pgio, inode, &nfs_pageio_read_ops,
			NFS_SERVER(inode)->rsize, 0);
}

void nfs_pageio_reset_read_mds(struct nfs_pageio_descriptor *pgio)
{
	pgio->pg_ops = &nfs_pageio_read_ops;
	pgio->pg_bsize = NFS_SERVER(pgio->pg_inode)->rsize;
}
EXPORT_SYMBOL_GPL(nfs_pageio_reset_read_mds);

static void nfs_pageio_init_read(struct nfs_pageio_descriptor *pgio,
		struct inode *inode)
{
	if (!pnfs_pageio_init_read(pgio, inode))
		nfs_pageio_init_read_mds(pgio, inode);
}

int nfs_readpage_async(struct nfs_open_context *ctx, struct inode *inode,
		       struct page *page)
{
	struct nfs_page	*new;
	unsigned int len;
	struct nfs_pageio_descriptor pgio;

	len = nfs_page_length(page);
	if (len == 0)
		return nfs_return_empty_page(page);
	new = nfs_create_request(ctx, inode, page, 0, len);
	if (IS_ERR(new)) {
		unlock_page(page);
		return PTR_ERR(new);
	}
	if (len < PAGE_CACHE_SIZE)
		zero_user_segment(page, len, PAGE_CACHE_SIZE);

	nfs_pageio_init_read(&pgio, inode);
	nfs_pageio_add_request(&pgio, new);
	nfs_pageio_complete(&pgio);
	return 0;
}

static void nfs_readpage_release(struct nfs_page *req)
{
	struct inode *d_inode = req->wb_context->dentry->d_inode;

	if (PageUptodate(req->wb_page))
		nfs_readpage_to_fscache(d_inode, req->wb_page, 0);

	unlock_page(req->wb_page);

	dprintk("NFS: read done (%s/%Ld %d@%Ld)\n",
			req->wb_context->dentry->d_inode->i_sb->s_id,
			(long long)NFS_FILEID(req->wb_context->dentry->d_inode),
			req->wb_bytes,
			(long long)req_offset(req));
	nfs_release_request(req);
}

int nfs_initiate_read(struct nfs_read_data *data, struct rpc_clnt *clnt,
		      const struct rpc_call_ops *call_ops)
{
	struct inode *inode = data->inode;
	int swap_flags = IS_SWAPFILE(inode) ? NFS_RPC_SWAPFLAGS : 0;
	struct rpc_task *task;
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
		.flags = RPC_TASK_ASYNC | swap_flags,
	};

	/* Set up the initial task struct. */
	NFS_PROTO(inode)->read_setup(data, &msg);

	dprintk("NFS: %5u initiated read call (req %s/%lld, %u bytes @ "
			"offset %llu)\n",
			data->task.tk_pid,
			inode->i_sb->s_id,
			(long long)NFS_FILEID(inode),
			data->args.count,
			(unsigned long long)data->args.offset);

	task = rpc_run_task(&task_setup_data);
	if (IS_ERR(task))
		return PTR_ERR(task);
	rpc_put_task(task);
	return 0;
}
EXPORT_SYMBOL_GPL(nfs_initiate_read);

/*
 * Set up the NFS read request struct
 */
static void nfs_read_rpcsetup(struct nfs_page *req, struct nfs_read_data *data,
		unsigned int count, unsigned int offset)
{
	struct inode *inode = req->wb_context->dentry->d_inode;

	data->req	  = req;
	data->inode	  = inode;
	data->cred	  = req->wb_context->cred;

	data->args.fh     = NFS_FH(inode);
	data->args.offset = req_offset(req) + offset;
	data->args.pgbase = req->wb_pgbase + offset;
	data->args.pages  = data->pagevec;
	data->args.count  = count;
	data->args.context = get_nfs_open_context(req->wb_context);
	data->args.lock_context = req->wb_lock_context;

	data->res.fattr   = &data->fattr;
	data->res.count   = count;
	data->res.eof     = 0;
	nfs_fattr_init(&data->fattr);
}

static int nfs_do_read(struct nfs_read_data *data,
		const struct rpc_call_ops *call_ops)
{
	struct inode *inode = data->args.context->dentry->d_inode;

	return nfs_initiate_read(data, NFS_CLIENT(inode), call_ops);
}

static int
nfs_do_multiple_reads(struct list_head *head,
		const struct rpc_call_ops *call_ops)
{
	struct nfs_read_data *data;
	int ret = 0;

	while (!list_empty(head)) {
		int ret2;

		data = list_entry(head->next, struct nfs_read_data, list);
		list_del_init(&data->list);

		ret2 = nfs_do_read(data, call_ops);
		if (ret == 0)
			ret = ret2;
	}
	return ret;
}

static void
nfs_async_read_error(struct list_head *head)
=======
#include "pnfs.h"
#include "nfstrace.h"

#define NFSDBG_FACILITY		NFSDBG_PAGECACHE

const struct nfs_pgio_completion_ops nfs_async_read_completion_ops;
static const struct nfs_rw_ops nfs_rw_read_ops;

static struct kmem_cache *nfs_rdata_cachep;

static struct nfs_pgio_header *nfs_readhdr_alloc(void)
{
	struct nfs_pgio_header *p = kmem_cache_zalloc(nfs_rdata_cachep, GFP_KERNEL);

	if (p)
		p->rw_mode = FMODE_READ;
	return p;
}

static void nfs_readhdr_free(struct nfs_pgio_header *rhdr)
{
	if (rhdr->res.scratch != NULL)
		kfree(rhdr->res.scratch);
	kmem_cache_free(nfs_rdata_cachep, rhdr);
}

static int nfs_return_empty_folio(struct folio *folio)
{
	folio_zero_segment(folio, 0, folio_size(folio));
	folio_mark_uptodate(folio);
	folio_unlock(folio);
	return 0;
}

void nfs_pageio_init_read(struct nfs_pageio_descriptor *pgio,
			      struct inode *inode, bool force_mds,
			      const struct nfs_pgio_completion_ops *compl_ops)
{
	struct nfs_server *server = NFS_SERVER(inode);
	const struct nfs_pageio_ops *pg_ops = &nfs_pgio_rw_ops;

#ifdef CONFIG_NFS_V4_1
	if (server->pnfs_curr_ld && !force_mds)
		pg_ops = server->pnfs_curr_ld->pg_read_ops;
#endif
	nfs_pageio_init(pgio, inode, pg_ops, compl_ops, &nfs_rw_read_ops,
			server->rsize, 0);
}
EXPORT_SYMBOL_GPL(nfs_pageio_init_read);

void nfs_pageio_complete_read(struct nfs_pageio_descriptor *pgio)
{
	struct nfs_pgio_mirror *pgm;
	unsigned long npages;

	nfs_pageio_complete(pgio);

	/* It doesn't make sense to do mirrored reads! */
	WARN_ON_ONCE(pgio->pg_mirror_count != 1);

	pgm = &pgio->pg_mirrors[0];
	NFS_I(pgio->pg_inode)->read_io += pgm->pg_bytes_written;
	npages = (pgm->pg_bytes_written + PAGE_SIZE - 1) >> PAGE_SHIFT;
	nfs_add_stats(pgio->pg_inode, NFSIOS_READPAGES, npages);
}


void nfs_pageio_reset_read_mds(struct nfs_pageio_descriptor *pgio)
{
	struct nfs_pgio_mirror *mirror;

	if (pgio->pg_ops && pgio->pg_ops->pg_cleanup)
		pgio->pg_ops->pg_cleanup(pgio);

	pgio->pg_ops = &nfs_pgio_rw_ops;

	/* read path should never have more than one mirror */
	WARN_ON_ONCE(pgio->pg_mirror_count != 1);

	mirror = &pgio->pg_mirrors[0];
	mirror->pg_bsize = NFS_SERVER(pgio->pg_inode)->rsize;
}
EXPORT_SYMBOL_GPL(nfs_pageio_reset_read_mds);

bool nfs_read_alloc_scratch(struct nfs_pgio_header *hdr, size_t size)
{
	WARN_ON(hdr->res.scratch != NULL);
	hdr->res.scratch = kmalloc(size, GFP_KERNEL);
	return hdr->res.scratch != NULL;
}
EXPORT_SYMBOL_GPL(nfs_read_alloc_scratch);

static void nfs_readpage_release(struct nfs_page *req, int error)
{
	struct folio *folio = nfs_page_to_folio(req);

	if (nfs_error_is_fatal_on_server(error) && error != -ETIMEDOUT)
		folio_set_error(folio);
	if (nfs_page_group_sync_on_bit(req, PG_UNLOCKPAGE))
		if (nfs_netfs_folio_unlock(folio))
			folio_unlock(folio);

	nfs_release_request(req);
}

static void nfs_page_group_set_uptodate(struct nfs_page *req)
{
	if (nfs_page_group_sync_on_bit(req, PG_UPTODATE))
		folio_mark_uptodate(nfs_page_to_folio(req));
}

static void nfs_read_completion(struct nfs_pgio_header *hdr)
{
	unsigned long bytes = 0;
	int error;

	if (test_bit(NFS_IOHDR_REDO, &hdr->flags))
		goto out;
	while (!list_empty(&hdr->pages)) {
		struct nfs_page *req = nfs_list_entry(hdr->pages.next);
		struct folio *folio = nfs_page_to_folio(req);
		unsigned long start = req->wb_pgbase;
		unsigned long end = req->wb_pgbase + req->wb_bytes;

		if (test_bit(NFS_IOHDR_EOF, &hdr->flags)) {
			/* note: regions of the page not covered by a
			 * request are zeroed in nfs_read_add_folio
			 */
			if (bytes > hdr->good_bytes) {
				/* nothing in this request was good, so zero
				 * the full extent of the request */
				folio_zero_segment(folio, start, end);

			} else if (hdr->good_bytes - bytes < req->wb_bytes) {
				/* part of this request has good bytes, but
				 * not all. zero the bad bytes */
				start += hdr->good_bytes - bytes;
				WARN_ON(start < req->wb_pgbase);
				folio_zero_segment(folio, start, end);
			}
		}
		error = 0;
		bytes += req->wb_bytes;
		if (test_bit(NFS_IOHDR_ERROR, &hdr->flags)) {
			if (bytes <= hdr->good_bytes)
				nfs_page_group_set_uptodate(req);
			else {
				error = hdr->error;
				xchg(&nfs_req_openctx(req)->error, error);
			}
		} else
			nfs_page_group_set_uptodate(req);
		nfs_list_remove_request(req);
		nfs_readpage_release(req, error);
	}
	nfs_netfs_read_completion(hdr);

out:
	hdr->release(hdr);
}

static void nfs_initiate_read(struct nfs_pgio_header *hdr,
			      struct rpc_message *msg,
			      const struct nfs_rpc_ops *rpc_ops,
			      struct rpc_task_setup *task_setup_data, int how)
{
	rpc_ops->read_setup(hdr, msg);
	nfs_netfs_initiate_read(hdr);
	trace_nfs_initiate_read(hdr);
}

static void
nfs_async_read_error(struct list_head *head, int error)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct nfs_page	*req;

	while (!list_empty(head)) {
		req = nfs_list_entry(head->next);
		nfs_list_remove_request(req);
<<<<<<< HEAD
		nfs_readpage_release(req);
	}
}

/*
 * Generate multiple requests to fill a single page.
 *
 * We optimize to reduce the number of read operations on the wire.  If we
 * detect that we're reading a page, or an area of a page, that is past the
 * end of file, we do not generate NFS read operations but just clear the
 * parts of the page that would have come back zero from the server anyway.
 *
 * We rely on the cached value of i_size to make this determination; another
 * client can fill pages on the server past our cached end-of-file, but we
 * won't see the new data until our attribute cache is updated.  This is more
 * or less conventional NFS client behavior.
 */
static int nfs_pagein_multi(struct nfs_pageio_descriptor *desc, struct list_head *res)
{
	struct nfs_page *req = nfs_list_entry(desc->pg_list.next);
	struct page *page = req->wb_page;
	struct nfs_read_data *data;
	size_t rsize = desc->pg_bsize, nbytes;
	unsigned int offset;
	int requests = 0;
	int ret = 0;

	nfs_list_remove_request(req);

	offset = 0;
	nbytes = desc->pg_count;
	do {
		size_t len = min(nbytes,rsize);

		data = nfs_readdata_alloc(1);
		if (!data)
			goto out_bad;
		data->pagevec[0] = page;
		nfs_read_rpcsetup(req, data, len, offset);
		list_add(&data->list, res);
		requests++;
		nbytes -= len;
		offset += len;
	} while(nbytes != 0);
	atomic_set(&req->wb_complete, requests);
	desc->pg_rpc_callops = &nfs_read_partial_ops;
	return ret;
out_bad:
	while (!list_empty(res)) {
		data = list_entry(res->next, struct nfs_read_data, list);
		list_del(&data->list);
		nfs_readdata_release(data);
	}
	nfs_readpage_release(req);
	return -ENOMEM;
}

static int nfs_pagein_one(struct nfs_pageio_descriptor *desc, struct list_head *res)
{
	struct nfs_page		*req;
	struct page		**pages;
	struct nfs_read_data	*data;
	struct list_head *head = &desc->pg_list;
	int ret = 0;

	data = nfs_readdata_alloc(nfs_page_array_len(desc->pg_base,
						     desc->pg_count));
	if (!data) {
		nfs_async_read_error(head);
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

	nfs_read_rpcsetup(req, data, desc->pg_count, 0);
	list_add(&data->list, res);
	desc->pg_rpc_callops = &nfs_read_full_ops;
out:
	return ret;
}

int nfs_generic_pagein(struct nfs_pageio_descriptor *desc, struct list_head *head)
{
	if (desc->pg_bsize < PAGE_CACHE_SIZE)
		return nfs_pagein_multi(desc, head);
	return nfs_pagein_one(desc, head);
}

static int nfs_generic_pg_readpages(struct nfs_pageio_descriptor *desc)
{
	LIST_HEAD(head);
	int ret;

	ret = nfs_generic_pagein(desc, &head);
	if (ret == 0)
		ret = nfs_do_multiple_reads(&head, desc->pg_rpc_callops);
	return ret;
}

static const struct nfs_pageio_ops nfs_pageio_read_ops = {
	.pg_test = nfs_generic_pg_test,
	.pg_doio = nfs_generic_pg_readpages,
=======
		nfs_readpage_release(req, error);
	}
}

const struct nfs_pgio_completion_ops nfs_async_read_completion_ops = {
	.error_cleanup = nfs_async_read_error,
	.completion = nfs_read_completion,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/*
 * This is the callback from RPC telling us whether a reply was
 * received or some error occurred (timeout or socket shutdown).
 */
<<<<<<< HEAD
int nfs_readpage_result(struct rpc_task *task, struct nfs_read_data *data)
{
	int status;

	dprintk("NFS: %s: %5u, (status %d)\n", __func__, task->tk_pid,
			task->tk_status);

	status = NFS_PROTO(data->inode)->read_done(task, data);
	if (status != 0)
		return status;

	nfs_add_stats(data->inode, NFSIOS_SERVERREADBYTES, data->res.count);

	if (task->tk_status == -ESTALE) {
		set_bit(NFS_INO_STALE, &NFS_I(data->inode)->flags);
		nfs_mark_for_revalidate(data->inode);
=======
static int nfs_readpage_done(struct rpc_task *task,
			     struct nfs_pgio_header *hdr,
			     struct inode *inode)
{
	int status = NFS_PROTO(inode)->read_done(task, hdr);
	if (status != 0)
		return status;

	nfs_add_stats(inode, NFSIOS_SERVERREADBYTES, hdr->res.count);
	trace_nfs_readpage_done(task, hdr);

	if (task->tk_status == -ESTALE) {
		nfs_set_inode_stale(inode);
		nfs_mark_for_revalidate(inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return 0;
}

<<<<<<< HEAD
static void nfs_readpage_retry(struct rpc_task *task, struct nfs_read_data *data)
{
	struct nfs_readargs *argp = &data->args;
	struct nfs_readres *resp = &data->res;

	if (resp->eof || resp->count == argp->count)
		return;

	/* This is a short read! */
	nfs_inc_stats(data->inode, NFSIOS_SHORTREAD);
	/* Has the server at least made some progress? */
	if (resp->count == 0)
		return;

	/* Yes, so retry the read at the end of the data */
	data->mds_offset += resp->count;
	argp->offset += resp->count;
	argp->pgbase += resp->count;
	argp->count -= resp->count;
	rpc_restart_call_prepare(task);
}

/*
 * Handle a read reply that fills part of a page.
 */
static void nfs_readpage_result_partial(struct rpc_task *task, void *calldata)
{
	struct nfs_read_data *data = calldata;
 
	if (nfs_readpage_result(task, data) != 0)
		return;
	if (task->tk_status < 0)
		return;

	nfs_readpage_truncate_uninitialised_page(data);
	nfs_readpage_retry(task, data);
}

static void nfs_readpage_release_partial(void *calldata)
{
	struct nfs_read_data *data = calldata;
	struct nfs_page *req = data->req;
	struct page *page = req->wb_page;
	int status = data->task.tk_status;

	if (status < 0)
		set_bit(PG_PARTIAL_READ_FAILED, &req->wb_flags);

	if (atomic_dec_and_test(&req->wb_complete)) {
		if (!test_bit(PG_PARTIAL_READ_FAILED, &req->wb_flags))
			SetPageUptodate(page);
		nfs_readpage_release(req);
	}
	nfs_readdata_release(calldata);
}

void nfs_read_prepare(struct rpc_task *task, void *calldata)
{
	struct nfs_read_data *data = calldata;
	NFS_PROTO(data->inode)->read_rpc_prepare(task, data);
}

static const struct rpc_call_ops nfs_read_partial_ops = {
	.rpc_call_prepare = nfs_read_prepare,
	.rpc_call_done = nfs_readpage_result_partial,
	.rpc_release = nfs_readpage_release_partial,
};

static void nfs_readpage_set_pages_uptodate(struct nfs_read_data *data)
{
	unsigned int count = data->res.count;
	unsigned int base = data->args.pgbase;
	struct page **pages;

	if (data->res.eof)
		count = data->args.count;
	if (unlikely(count == 0))
		return;
	pages = &data->args.pages[base >> PAGE_CACHE_SHIFT];
	base &= ~PAGE_CACHE_MASK;
	count += base;
	for (;count >= PAGE_CACHE_SIZE; count -= PAGE_CACHE_SIZE, pages++)
		SetPageUptodate(*pages);
	if (count == 0)
		return;
	/* Was this a short read? */
	if (data->res.eof || data->res.count == data->args.count)
		SetPageUptodate(*pages);
}

/*
 * This is the callback from RPC telling us whether a reply was
 * received or some error occurred (timeout or socket shutdown).
 */
static void nfs_readpage_result_full(struct rpc_task *task, void *calldata)
{
	struct nfs_read_data *data = calldata;

	if (nfs_readpage_result(task, data) != 0)
		return;
	if (task->tk_status < 0)
		return;
	/*
	 * Note: nfs_readpage_retry may change the values of
	 * data->args. In the multi-page case, we therefore need
	 * to ensure that we call nfs_readpage_set_pages_uptodate()
	 * first.
	 */
	nfs_readpage_truncate_uninitialised_page(data);
	nfs_readpage_set_pages_uptodate(data);
	nfs_readpage_retry(task, data);
}

static void nfs_readpage_release_full(void *calldata)
{
	struct nfs_read_data *data = calldata;

	while (!list_empty(&data->pages)) {
		struct nfs_page *req = nfs_list_entry(data->pages.next);

		nfs_list_remove_request(req);
		nfs_readpage_release(req);
	}
	nfs_readdata_release(calldata);
}

static const struct rpc_call_ops nfs_read_full_ops = {
	.rpc_call_prepare = nfs_read_prepare,
	.rpc_call_done = nfs_readpage_result_full,
	.rpc_release = nfs_readpage_release_full,
};

=======
static void nfs_readpage_retry(struct rpc_task *task,
			       struct nfs_pgio_header *hdr)
{
	struct nfs_pgio_args *argp = &hdr->args;
	struct nfs_pgio_res  *resp = &hdr->res;

	/* This is a short read! */
	nfs_inc_stats(hdr->inode, NFSIOS_SHORTREAD);
	trace_nfs_readpage_short(task, hdr);

	/* Has the server at least made some progress? */
	if (resp->count == 0) {
		nfs_set_pgio_error(hdr, -EIO, argp->offset);
		return;
	}

	/* For non rpc-based layout drivers, retry-through-MDS */
	if (!task->tk_ops) {
		hdr->pnfs_error = -EAGAIN;
		return;
	}

	/* Yes, so retry the read at the end of the hdr */
	hdr->mds_offset += resp->count;
	argp->offset += resp->count;
	argp->pgbase += resp->count;
	argp->count -= resp->count;
	resp->count = 0;
	resp->eof = 0;
	rpc_restart_call_prepare(task);
}

static void nfs_readpage_result(struct rpc_task *task,
				struct nfs_pgio_header *hdr)
{
	if (hdr->res.eof) {
		loff_t pos = hdr->args.offset + hdr->res.count;
		unsigned int new = pos - hdr->io_start;

		if (hdr->good_bytes > new) {
			hdr->good_bytes = new;
			set_bit(NFS_IOHDR_EOF, &hdr->flags);
			clear_bit(NFS_IOHDR_ERROR, &hdr->flags);
		}
	} else if (hdr->res.count < hdr->args.count)
		nfs_readpage_retry(task, hdr);
}

int nfs_read_add_folio(struct nfs_pageio_descriptor *pgio,
		       struct nfs_open_context *ctx,
		       struct folio *folio)
{
	struct inode *inode = folio_file_mapping(folio)->host;
	struct nfs_server *server = NFS_SERVER(inode);
	size_t fsize = folio_size(folio);
	unsigned int rsize = server->rsize;
	struct nfs_page *new;
	unsigned int len, aligned_len;
	int error;

	len = nfs_folio_length(folio);
	if (len == 0)
		return nfs_return_empty_folio(folio);

	aligned_len = min_t(unsigned int, ALIGN(len, rsize), fsize);

	new = nfs_page_create_from_folio(ctx, folio, 0, aligned_len);
	if (IS_ERR(new)) {
		error = PTR_ERR(new);
		if (nfs_netfs_folio_unlock(folio))
			folio_unlock(folio);
		goto out;
	}

	if (len < fsize)
		folio_zero_segment(folio, len, fsize);
	if (!nfs_pageio_add_request(pgio, new)) {
		nfs_list_remove_request(new);
		error = pgio->pg_error;
		nfs_readpage_release(new, error);
		goto out;
	}
	return 0;
out:
	return error;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Read a page over NFS.
 * We read the page synchronously in the following case:
 *  -	The error flag is set for this page. This happens only when a
 *	previous async read operation failed.
 */
<<<<<<< HEAD
int nfs_readpage(struct file *file, struct page *page)
{
	struct nfs_open_context *ctx;
	struct inode *inode = page->mapping->host;
	int		error;

	dprintk("NFS: nfs_readpage (%p %ld@%lu)\n",
		page, PAGE_CACHE_SIZE, page->index);
	nfs_inc_stats(inode, NFSIOS_VFSREADPAGE);
	nfs_add_stats(inode, NFSIOS_READPAGES, 1);
=======
int nfs_read_folio(struct file *file, struct folio *folio)
{
	struct inode *inode = file_inode(file);
	struct nfs_pageio_descriptor pgio;
	struct nfs_open_context *ctx;
	int ret;

	trace_nfs_aop_readpage(inode, folio);
	nfs_inc_stats(inode, NFSIOS_VFSREADPAGE);
	task_io_account_read(folio_size(folio));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Try to flush any pending writes to the file..
	 *
<<<<<<< HEAD
	 * NOTE! Because we own the page lock, there cannot
	 * be any new pending writes generated at this point
	 * for this page (other pages can be written to).
	 */
	error = nfs_wb_page(inode, page);
	if (error)
		goto out_unlock;
	if (PageUptodate(page))
		goto out_unlock;

	error = -ESTALE;
	if (NFS_STALE(inode))
		goto out_unlock;

	if (file == NULL) {
		error = -EBADF;
		ctx = nfs_find_open_context(inode, NULL, FMODE_READ);
		if (ctx == NULL)
			goto out_unlock;
	} else
		ctx = get_nfs_open_context(nfs_file_open_context(file));

	if (!IS_SYNC(inode)) {
		error = nfs_readpage_from_fscache(ctx, inode, page);
		if (error == 0)
			goto out;
	}

	error = nfs_readpage_async(ctx, inode, page);

out:
	put_nfs_open_context(ctx);
	return error;
out_unlock:
	unlock_page(page);
	return error;
}

struct nfs_readdesc {
	struct nfs_pageio_descriptor *pgio;
	struct nfs_open_context *ctx;
};

static int
readpage_async_filler(void *data, struct page *page)
{
	struct nfs_readdesc *desc = (struct nfs_readdesc *)data;
	struct inode *inode = page->mapping->host;
	struct nfs_page *new;
	unsigned int len;
	int error;

	len = nfs_page_length(page);
	if (len == 0)
		return nfs_return_empty_page(page);

	new = nfs_create_request(desc->ctx, inode, page, 0, len);
	if (IS_ERR(new))
		goto out_error;

	if (len < PAGE_CACHE_SIZE)
		zero_user_segment(page, len, PAGE_CACHE_SIZE);
	if (!nfs_pageio_add_request(desc->pgio, new)) {
		error = desc->pgio->pg_error;
		goto out_unlock;
	}
	return 0;
out_error:
	error = PTR_ERR(new);
out_unlock:
	unlock_page(page);
	return error;
}

int nfs_readpages(struct file *filp, struct address_space *mapping,
		struct list_head *pages, unsigned nr_pages)
{
	struct nfs_pageio_descriptor pgio;
	struct nfs_readdesc desc = {
		.pgio = &pgio,
	};
	struct inode *inode = mapping->host;
	unsigned long npages;
	int ret = -ESTALE;

	dprintk("NFS: nfs_readpages (%s/%Ld %d)\n",
			inode->i_sb->s_id,
			(long long)NFS_FILEID(inode),
			nr_pages);
	nfs_inc_stats(inode, NFSIOS_VFSREADPAGES);

	if (NFS_STALE(inode))
		goto out;

	if (filp == NULL) {
		desc.ctx = nfs_find_open_context(inode, NULL, FMODE_READ);
		if (desc.ctx == NULL)
			return -EBADF;
	} else
		desc.ctx = get_nfs_open_context(nfs_file_open_context(filp));

	/* attempt to read as many of the pages as possible from the cache
	 * - this returns -ENOBUFS immediately if the cookie is negative
	 */
	ret = nfs_readpages_from_fscache(desc.ctx, inode, mapping,
					 pages, &nr_pages);
	if (ret == 0)
		goto read_complete; /* all pages were read */

	nfs_pageio_init_read(&pgio, inode);

	ret = read_cache_pages(mapping, pages, readpage_async_filler, &desc);

	nfs_pageio_complete(&pgio);
	npages = (pgio.pg_bytes_written + PAGE_CACHE_SIZE - 1) >> PAGE_CACHE_SHIFT;
	nfs_add_stats(inode, NFSIOS_READPAGES, npages);
read_complete:
	put_nfs_open_context(desc.ctx);
out:
	return ret;
=======
	 * NOTE! Because we own the folio lock, there cannot
	 * be any new pending writes generated at this point
	 * for this folio (other folios can be written to).
	 */
	ret = nfs_wb_folio(inode, folio);
	if (ret)
		goto out_unlock;
	if (folio_test_uptodate(folio))
		goto out_unlock;

	ret = -ESTALE;
	if (NFS_STALE(inode))
		goto out_unlock;

	ret = nfs_netfs_read_folio(file, folio);
	if (!ret)
		goto out;

	ctx = get_nfs_open_context(nfs_file_open_context(file));

	xchg(&ctx->error, 0);
	nfs_pageio_init_read(&pgio, inode, false,
			     &nfs_async_read_completion_ops);

	ret = nfs_read_add_folio(&pgio, ctx, folio);
	if (ret)
		goto out_put;

	nfs_pageio_complete_read(&pgio);
	ret = pgio.pg_error < 0 ? pgio.pg_error : 0;
	if (!ret) {
		ret = folio_wait_locked_killable(folio);
		if (!folio_test_uptodate(folio) && !ret)
			ret = xchg(&ctx->error, 0);
	}
out_put:
	put_nfs_open_context(ctx);
out:
	trace_nfs_aop_readpage_done(inode, folio, ret);
	return ret;
out_unlock:
	folio_unlock(folio);
	goto out;
}

void nfs_readahead(struct readahead_control *ractl)
{
	struct nfs_pageio_descriptor pgio;
	struct nfs_open_context *ctx;
	unsigned int nr_pages = readahead_count(ractl);
	struct file *file = ractl->file;
	struct inode *inode = ractl->mapping->host;
	struct folio *folio;
	int ret;

	trace_nfs_aop_readahead(inode, readahead_pos(ractl), nr_pages);
	nfs_inc_stats(inode, NFSIOS_VFSREADPAGES);
	task_io_account_read(readahead_length(ractl));

	ret = -ESTALE;
	if (NFS_STALE(inode))
		goto out;

	ret = nfs_netfs_readahead(ractl);
	if (!ret)
		goto out;

	if (file == NULL) {
		ret = -EBADF;
		ctx = nfs_find_open_context(inode, NULL, FMODE_READ);
		if (ctx == NULL)
			goto out;
	} else
		ctx = get_nfs_open_context(nfs_file_open_context(file));

	nfs_pageio_init_read(&pgio, inode, false,
			     &nfs_async_read_completion_ops);

	while ((folio = readahead_folio(ractl)) != NULL) {
		ret = nfs_read_add_folio(&pgio, ctx, folio);
		if (ret)
			break;
	}

	nfs_pageio_complete_read(&pgio);

	put_nfs_open_context(ctx);
out:
	trace_nfs_aop_readahead_done(inode, nr_pages, ret);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int __init nfs_init_readpagecache(void)
{
	nfs_rdata_cachep = kmem_cache_create("nfs_read_data",
<<<<<<< HEAD
					     sizeof(struct nfs_read_data),
=======
					     sizeof(struct nfs_pgio_header),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					     0, SLAB_HWCACHE_ALIGN,
					     NULL);
	if (nfs_rdata_cachep == NULL)
		return -ENOMEM;

	return 0;
}

void nfs_destroy_readpagecache(void)
{
	kmem_cache_destroy(nfs_rdata_cachep);
}
<<<<<<< HEAD
=======

static const struct nfs_rw_ops nfs_rw_read_ops = {
	.rw_alloc_header	= nfs_readhdr_alloc,
	.rw_free_header		= nfs_readhdr_free,
	.rw_done		= nfs_readpage_done,
	.rw_result		= nfs_readpage_result,
	.rw_initiate		= nfs_initiate_read,
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
