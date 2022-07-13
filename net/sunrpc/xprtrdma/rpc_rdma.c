<<<<<<< HEAD
/*
=======
// SPDX-License-Identifier: GPL-2.0 OR BSD-3-Clause
/*
 * Copyright (c) 2014-2020, Oracle and/or its affiliates.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Copyright (c) 2003-2007 Network Appliance, Inc. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the BSD-type
 * license below:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *      Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *      Redistributions in binary form must reproduce the above
 *      copyright notice, this list of conditions and the following
 *      disclaimer in the documentation and/or other materials provided
 *      with the distribution.
 *
 *      Neither the name of the Network Appliance, Inc. nor the names of
 *      its contributors may be used to endorse or promote products
 *      derived from this software without specific prior written
 *      permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * rpc_rdma.c
 *
 * This file contains the guts of the RPC RDMA protocol, and
 * does marshaling/unmarshaling, etc. It is also where interfacing
 * to the Linux RPC framework lives.
 */

<<<<<<< HEAD
#include "xprt_rdma.h"

#include <linux/highmem.h>

#ifdef RPC_DEBUG
# define RPCDBG_FACILITY	RPCDBG_TRANS
#endif

enum rpcrdma_chunktype {
	rpcrdma_noch = 0,
	rpcrdma_readch,
	rpcrdma_areadch,
	rpcrdma_writech,
	rpcrdma_replych
};

#ifdef RPC_DEBUG
static const char transfertypes[][12] = {
	"pure inline",	/* no chunks */
	" read chunk",	/* some argument via rdma read */
	"*read chunk",	/* entire request via rdma read */
	"write chunk",	/* some result via rdma write */
	"reply chunk"	/* entire reply via rdma write */
};
#endif

/*
 * Chunk assembly from upper layer xdr_buf.
 *
 * Prepare the passed-in xdr_buf into representation as RPC/RDMA chunk
 * elements. Segments are then coalesced when registered, if possible
 * within the selected memreg mode.
 *
 * Note, this routine is never called if the connection's memory
 * registration strategy is 0 (bounce buffers).
 */

static int
rpcrdma_convert_iovs(struct xdr_buf *xdrbuf, unsigned int pos,
	enum rpcrdma_chunktype type, struct rpcrdma_mr_seg *seg, int nsegs)
{
	int len, n = 0, p;
	int page_base;
	struct page **ppages;

	if (pos == 0 && xdrbuf->head[0].iov_len) {
		seg[n].mr_page = NULL;
		seg[n].mr_offset = xdrbuf->head[0].iov_base;
		seg[n].mr_len = xdrbuf->head[0].iov_len;
		++n;
	}

	len = xdrbuf->page_len;
	ppages = xdrbuf->pages + (xdrbuf->page_base >> PAGE_SHIFT);
	page_base = xdrbuf->page_base & ~PAGE_MASK;
	p = 0;
	while (len && n < nsegs) {
		seg[n].mr_page = ppages[p];
		seg[n].mr_offset = (void *)(unsigned long) page_base;
		seg[n].mr_len = min_t(u32, PAGE_SIZE - page_base, len);
		BUG_ON(seg[n].mr_len > PAGE_SIZE);
		len -= seg[n].mr_len;
		++n;
		++p;
		page_base = 0;	/* page offset only applies to first page */
	}

	/* Message overflows the seg array */
	if (len && n == nsegs)
		return 0;

	if (xdrbuf->tail[0].iov_len) {
		/* the rpcrdma protocol allows us to omit any trailing
		 * xdr pad bytes, saving the server an RDMA operation. */
		if (xdrbuf->tail[0].iov_len < 4 && xprt_rdma_pad_optimize)
			return n;
		if (n == nsegs)
			/* Tail remains, but we're out of segments */
			return 0;
		seg[n].mr_page = NULL;
		seg[n].mr_offset = xdrbuf->tail[0].iov_base;
		seg[n].mr_len = xdrbuf->tail[0].iov_len;
		++n;
	}

	return n;
}

/*
 * Create read/write chunk lists, and reply chunks, for RDMA
 *
 *   Assume check against THRESHOLD has been done, and chunks are required.
 *   Assume only encoding one list entry for read|write chunks. The NFSv3
 *     protocol is simple enough to allow this as it only has a single "bulk
 *     result" in each procedure - complicated NFSv4 COMPOUNDs are not. (The
 *     RDMA/Sessions NFSv4 proposal addresses this for future v4 revs.)
 *
 * When used for a single reply chunk (which is a special write
 * chunk used for the entire reply, rather than just the data), it
 * is used primarily for READDIR and READLINK which would otherwise
 * be severely size-limited by a small rdma inline read max. The server
 * response will come back as an RDMA Write, followed by a message
 * of type RDMA_NOMSG carrying the xid and length. As a result, reply
 * chunks do not provide data alignment, however they do not require
 * "fixup" (moving the response to the upper layer buffer) either.
=======
#include <linux/highmem.h>

#include <linux/sunrpc/svc_rdma.h>

#include "xprt_rdma.h"
#include <trace/events/rpcrdma.h>

/* Returns size of largest RPC-over-RDMA header in a Call message
 *
 * The largest Call header contains a full-size Read list and a
 * minimal Reply chunk.
 */
static unsigned int rpcrdma_max_call_header_size(unsigned int maxsegs)
{
	unsigned int size;

	/* Fixed header fields and list discriminators */
	size = RPCRDMA_HDRLEN_MIN;

	/* Maximum Read list size */
	size += maxsegs * rpcrdma_readchunk_maxsz * sizeof(__be32);

	/* Minimal Read chunk size */
	size += sizeof(__be32);	/* segment count */
	size += rpcrdma_segment_maxsz * sizeof(__be32);
	size += sizeof(__be32);	/* list discriminator */

	return size;
}

/* Returns size of largest RPC-over-RDMA header in a Reply message
 *
 * There is only one Write list or one Reply chunk per Reply
 * message.  The larger list is the Write list.
 */
static unsigned int rpcrdma_max_reply_header_size(unsigned int maxsegs)
{
	unsigned int size;

	/* Fixed header fields and list discriminators */
	size = RPCRDMA_HDRLEN_MIN;

	/* Maximum Write list size */
	size += sizeof(__be32);		/* segment count */
	size += maxsegs * rpcrdma_segment_maxsz * sizeof(__be32);
	size += sizeof(__be32);	/* list discriminator */

	return size;
}

/**
 * rpcrdma_set_max_header_sizes - Initialize inline payload sizes
 * @ep: endpoint to initialize
 *
 * The max_inline fields contain the maximum size of an RPC message
 * so the marshaling code doesn't have to repeat this calculation
 * for every RPC.
 */
void rpcrdma_set_max_header_sizes(struct rpcrdma_ep *ep)
{
	unsigned int maxsegs = ep->re_max_rdma_segs;

	ep->re_max_inline_send =
		ep->re_inline_send - rpcrdma_max_call_header_size(maxsegs);
	ep->re_max_inline_recv =
		ep->re_inline_recv - rpcrdma_max_reply_header_size(maxsegs);
}

/* The client can send a request inline as long as the RPCRDMA header
 * plus the RPC call fit under the transport's inline limit. If the
 * combined call message size exceeds that limit, the client must use
 * a Read chunk for this operation.
 *
 * A Read chunk is also required if sending the RPC call inline would
 * exceed this device's max_sge limit.
 */
static bool rpcrdma_args_inline(struct rpcrdma_xprt *r_xprt,
				struct rpc_rqst *rqst)
{
	struct xdr_buf *xdr = &rqst->rq_snd_buf;
	struct rpcrdma_ep *ep = r_xprt->rx_ep;
	unsigned int count, remaining, offset;

	if (xdr->len > ep->re_max_inline_send)
		return false;

	if (xdr->page_len) {
		remaining = xdr->page_len;
		offset = offset_in_page(xdr->page_base);
		count = RPCRDMA_MIN_SEND_SGES;
		while (remaining) {
			remaining -= min_t(unsigned int,
					   PAGE_SIZE - offset, remaining);
			offset = 0;
			if (++count > ep->re_attr.cap.max_send_sge)
				return false;
		}
	}

	return true;
}

/* The client can't know how large the actual reply will be. Thus it
 * plans for the largest possible reply for that particular ULP
 * operation. If the maximum combined reply message size exceeds that
 * limit, the client must provide a write list or a reply chunk for
 * this request.
 */
static bool rpcrdma_results_inline(struct rpcrdma_xprt *r_xprt,
				   struct rpc_rqst *rqst)
{
	return rqst->rq_rcv_buf.buflen <= r_xprt->rx_ep->re_max_inline_recv;
}

/* The client is required to provide a Reply chunk if the maximum
 * size of the non-payload part of the RPC Reply is larger than
 * the inline threshold.
 */
static bool
rpcrdma_nonpayload_inline(const struct rpcrdma_xprt *r_xprt,
			  const struct rpc_rqst *rqst)
{
	const struct xdr_buf *buf = &rqst->rq_rcv_buf;

	return (buf->head[0].iov_len + buf->tail[0].iov_len) <
		r_xprt->rx_ep->re_max_inline_recv;
}

/* ACL likes to be lazy in allocating pages. For TCP, these
 * pages can be allocated during receive processing. Not true
 * for RDMA, which must always provision receive buffers
 * up front.
 */
static noinline int
rpcrdma_alloc_sparse_pages(struct xdr_buf *buf)
{
	struct page **ppages;
	int len;

	len = buf->page_len;
	ppages = buf->pages + (buf->page_base >> PAGE_SHIFT);
	while (len > 0) {
		if (!*ppages)
			*ppages = alloc_page(GFP_NOWAIT | __GFP_NOWARN);
		if (!*ppages)
			return -ENOBUFS;
		ppages++;
		len -= PAGE_SIZE;
	}

	return 0;
}

/* Convert @vec to a single SGL element.
 *
 * Returns pointer to next available SGE, and bumps the total number
 * of SGEs consumed.
 */
static struct rpcrdma_mr_seg *
rpcrdma_convert_kvec(struct kvec *vec, struct rpcrdma_mr_seg *seg,
		     unsigned int *n)
{
	seg->mr_page = virt_to_page(vec->iov_base);
	seg->mr_offset = offset_in_page(vec->iov_base);
	seg->mr_len = vec->iov_len;
	++seg;
	++(*n);
	return seg;
}

/* Convert @xdrbuf into SGEs no larger than a page each. As they
 * are registered, these SGEs are then coalesced into RDMA segments
 * when the selected memreg mode supports it.
 *
 * Returns positive number of SGEs consumed, or a negative errno.
 */

static int
rpcrdma_convert_iovs(struct rpcrdma_xprt *r_xprt, struct xdr_buf *xdrbuf,
		     unsigned int pos, enum rpcrdma_chunktype type,
		     struct rpcrdma_mr_seg *seg)
{
	unsigned long page_base;
	unsigned int len, n;
	struct page **ppages;

	n = 0;
	if (pos == 0)
		seg = rpcrdma_convert_kvec(&xdrbuf->head[0], seg, &n);

	len = xdrbuf->page_len;
	ppages = xdrbuf->pages + (xdrbuf->page_base >> PAGE_SHIFT);
	page_base = offset_in_page(xdrbuf->page_base);
	while (len) {
		seg->mr_page = *ppages;
		seg->mr_offset = page_base;
		seg->mr_len = min_t(u32, PAGE_SIZE - page_base, len);
		len -= seg->mr_len;
		++ppages;
		++seg;
		++n;
		page_base = 0;
	}

	if (type == rpcrdma_readch || type == rpcrdma_writech)
		goto out;

	if (xdrbuf->tail[0].iov_len)
		rpcrdma_convert_kvec(&xdrbuf->tail[0], seg, &n);

out:
	if (unlikely(n > RPCRDMA_MAX_SEGS))
		return -EIO;
	return n;
}

static int
encode_rdma_segment(struct xdr_stream *xdr, struct rpcrdma_mr *mr)
{
	__be32 *p;

	p = xdr_reserve_space(xdr, 4 * sizeof(*p));
	if (unlikely(!p))
		return -EMSGSIZE;

	xdr_encode_rdma_segment(p, mr->mr_handle, mr->mr_length, mr->mr_offset);
	return 0;
}

static int
encode_read_segment(struct xdr_stream *xdr, struct rpcrdma_mr *mr,
		    u32 position)
{
	__be32 *p;

	p = xdr_reserve_space(xdr, 6 * sizeof(*p));
	if (unlikely(!p))
		return -EMSGSIZE;

	*p++ = xdr_one;			/* Item present */
	xdr_encode_read_segment(p, position, mr->mr_handle, mr->mr_length,
				mr->mr_offset);
	return 0;
}

static struct rpcrdma_mr_seg *rpcrdma_mr_prepare(struct rpcrdma_xprt *r_xprt,
						 struct rpcrdma_req *req,
						 struct rpcrdma_mr_seg *seg,
						 int nsegs, bool writing,
						 struct rpcrdma_mr **mr)
{
	*mr = rpcrdma_mr_pop(&req->rl_free_mrs);
	if (!*mr) {
		*mr = rpcrdma_mr_get(r_xprt);
		if (!*mr)
			goto out_getmr_err;
		(*mr)->mr_req = req;
	}

	rpcrdma_mr_push(*mr, &req->rl_registered);
	return frwr_map(r_xprt, seg, nsegs, writing, req->rl_slot.rq_xid, *mr);

out_getmr_err:
	trace_xprtrdma_nomrs_err(r_xprt, req);
	xprt_wait_for_buffer_space(&r_xprt->rx_xprt);
	rpcrdma_mrs_refresh(r_xprt);
	return ERR_PTR(-EAGAIN);
}

/* Register and XDR encode the Read list. Supports encoding a list of read
 * segments that belong to a single read chunk.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Encoding key for single-list chunks (HLOO = Handle32 Length32 Offset64):
 *
 *  Read chunklist (a linked list):
 *   N elements, position P (same P for all chunks of same arg!):
 *    1 - PHLOO - 1 - PHLOO - ... - 1 - PHLOO - 0
 *
<<<<<<< HEAD
=======
 * Returns zero on success, or a negative errno if a failure occurred.
 * @xdr is advanced to the next position in the stream.
 *
 * Only a single @pos value is currently supported.
 */
static int rpcrdma_encode_read_list(struct rpcrdma_xprt *r_xprt,
				    struct rpcrdma_req *req,
				    struct rpc_rqst *rqst,
				    enum rpcrdma_chunktype rtype)
{
	struct xdr_stream *xdr = &req->rl_stream;
	struct rpcrdma_mr_seg *seg;
	struct rpcrdma_mr *mr;
	unsigned int pos;
	int nsegs;

	if (rtype == rpcrdma_noch_pullup || rtype == rpcrdma_noch_mapped)
		goto done;

	pos = rqst->rq_snd_buf.head[0].iov_len;
	if (rtype == rpcrdma_areadch)
		pos = 0;
	seg = req->rl_segments;
	nsegs = rpcrdma_convert_iovs(r_xprt, &rqst->rq_snd_buf, pos,
				     rtype, seg);
	if (nsegs < 0)
		return nsegs;

	do {
		seg = rpcrdma_mr_prepare(r_xprt, req, seg, nsegs, false, &mr);
		if (IS_ERR(seg))
			return PTR_ERR(seg);

		if (encode_read_segment(xdr, mr, pos) < 0)
			return -EMSGSIZE;

		trace_xprtrdma_chunk_read(rqst->rq_task, pos, mr, nsegs);
		r_xprt->rx_stats.read_chunk_count++;
		nsegs -= mr->mr_nents;
	} while (nsegs);

done:
	if (xdr_stream_encode_item_absent(xdr) < 0)
		return -EMSGSIZE;
	return 0;
}

/* Register and XDR encode the Write list. Supports encoding a list
 * containing one array of plain segments that belong to a single
 * write chunk.
 *
 * Encoding key for single-list chunks (HLOO = Handle32 Length32 Offset64):
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *  Write chunklist (a list of (one) counted array):
 *   N elements:
 *    1 - N - HLOO - HLOO - ... - HLOO - 0
 *
<<<<<<< HEAD
 *  Reply chunk (a counted array):
 *   N elements:
 *    1 - N - HLOO - HLOO - ... - HLOO
 */

static unsigned int
rpcrdma_create_chunks(struct rpc_rqst *rqst, struct xdr_buf *target,
		struct rpcrdma_msg *headerp, enum rpcrdma_chunktype type)
{
	struct rpcrdma_req *req = rpcr_to_rdmar(rqst);
	struct rpcrdma_xprt *r_xprt = rpcx_to_rdmax(rqst->rq_task->tk_xprt);
	int nsegs, nchunks = 0;
	unsigned int pos;
	struct rpcrdma_mr_seg *seg = req->rl_segments;
	struct rpcrdma_read_chunk *cur_rchunk = NULL;
	struct rpcrdma_write_array *warray = NULL;
	struct rpcrdma_write_chunk *cur_wchunk = NULL;
	__be32 *iptr = headerp->rm_body.rm_chunks;

	if (type == rpcrdma_readch || type == rpcrdma_areadch) {
		/* a read chunk - server will RDMA Read our memory */
		cur_rchunk = (struct rpcrdma_read_chunk *) iptr;
	} else {
		/* a write or reply chunk - server will RDMA Write our memory */
		*iptr++ = xdr_zero;	/* encode a NULL read chunk list */
		if (type == rpcrdma_replych)
			*iptr++ = xdr_zero;	/* a NULL write chunk list */
		warray = (struct rpcrdma_write_array *) iptr;
		cur_wchunk = (struct rpcrdma_write_chunk *) (warray + 1);
	}

	if (type == rpcrdma_replych || type == rpcrdma_areadch)
		pos = 0;
	else
		pos = target->head[0].iov_len;

	nsegs = rpcrdma_convert_iovs(target, pos, type, seg, RPCRDMA_MAX_SEGS);
	if (nsegs == 0)
		return 0;

	do {
		/* bind/register the memory, then build chunk from result. */
		int n = rpcrdma_register_external(seg, nsegs,
						cur_wchunk != NULL, r_xprt);
		if (n <= 0)
			goto out;
		if (cur_rchunk) {	/* read */
			cur_rchunk->rc_discrim = xdr_one;
			/* all read chunks have the same "position" */
			cur_rchunk->rc_position = htonl(pos);
			cur_rchunk->rc_target.rs_handle = htonl(seg->mr_rkey);
			cur_rchunk->rc_target.rs_length = htonl(seg->mr_len);
			xdr_encode_hyper(
					(__be32 *)&cur_rchunk->rc_target.rs_offset,
					seg->mr_base);
			dprintk("RPC:       %s: read chunk "
				"elem %d@0x%llx:0x%x pos %u (%s)\n", __func__,
				seg->mr_len, (unsigned long long)seg->mr_base,
				seg->mr_rkey, pos, n < nsegs ? "more" : "last");
			cur_rchunk++;
			r_xprt->rx_stats.read_chunk_count++;
		} else {		/* write/reply */
			cur_wchunk->wc_target.rs_handle = htonl(seg->mr_rkey);
			cur_wchunk->wc_target.rs_length = htonl(seg->mr_len);
			xdr_encode_hyper(
					(__be32 *)&cur_wchunk->wc_target.rs_offset,
					seg->mr_base);
			dprintk("RPC:       %s: %s chunk "
				"elem %d@0x%llx:0x%x (%s)\n", __func__,
				(type == rpcrdma_replych) ? "reply" : "write",
				seg->mr_len, (unsigned long long)seg->mr_base,
				seg->mr_rkey, n < nsegs ? "more" : "last");
			cur_wchunk++;
			if (type == rpcrdma_replych)
				r_xprt->rx_stats.reply_chunk_count++;
			else
				r_xprt->rx_stats.write_chunk_count++;
			r_xprt->rx_stats.total_rdma_request += seg->mr_len;
		}
		nchunks++;
		seg   += n;
		nsegs -= n;
	} while (nsegs);

	/* success. all failures return above */
	req->rl_nchunks = nchunks;

	BUG_ON(nchunks == 0);
	BUG_ON((r_xprt->rx_ia.ri_memreg_strategy == RPCRDMA_FRMR)
	       && (nchunks > 3));

	/*
	 * finish off header. If write, marshal discrim and nchunks.
	 */
	if (cur_rchunk) {
		iptr = (__be32 *) cur_rchunk;
		*iptr++ = xdr_zero;	/* finish the read chunk list */
		*iptr++ = xdr_zero;	/* encode a NULL write chunk list */
		*iptr++ = xdr_zero;	/* encode a NULL reply chunk */
	} else {
		warray->wc_discrim = xdr_one;
		warray->wc_nchunks = htonl(nchunks);
		iptr = (__be32 *) cur_wchunk;
		if (type == rpcrdma_writech) {
			*iptr++ = xdr_zero; /* finish the write chunk list */
			*iptr++ = xdr_zero; /* encode a NULL reply chunk */
		}
	}

	/*
	 * Return header size.
	 */
	return (unsigned char *)iptr - (unsigned char *)headerp;

out:
	for (pos = 0; nchunks--;)
		pos += rpcrdma_deregister_external(
				&req->rl_segments[pos], r_xprt, NULL);
	return 0;
}

/*
 * Copy write data inline.
 * This function is used for "small" requests. Data which is passed
 * to RPC via iovecs (or page list) is copied directly into the
 * pre-registered memory buffer for this request. For small amounts
 * of data, this is efficient. The cutoff value is tunable.
 */
static int
rpcrdma_inline_pullup(struct rpc_rqst *rqst, int pad)
{
	int i, npages, curlen;
	int copy_len;
	unsigned char *srcp, *destp;
	struct rpcrdma_xprt *r_xprt = rpcx_to_rdmax(rqst->rq_xprt);
	int page_base;
	struct page **ppages;

	destp = rqst->rq_svec[0].iov_base;
	curlen = rqst->rq_svec[0].iov_len;
	destp += curlen;
	/*
	 * Do optional padding where it makes sense. Alignment of write
	 * payload can help the server, if our setting is accurate.
	 */
	pad -= (curlen + 36/*sizeof(struct rpcrdma_msg_padded)*/);
	if (pad < 0 || rqst->rq_slen - curlen < RPCRDMA_INLINE_PAD_THRESH)
		pad = 0;	/* don't pad this request */

	dprintk("RPC:       %s: pad %d destp 0x%p len %d hdrlen %d\n",
		__func__, pad, destp, rqst->rq_slen, curlen);

	copy_len = rqst->rq_snd_buf.page_len;

	if (rqst->rq_snd_buf.tail[0].iov_len) {
		curlen = rqst->rq_snd_buf.tail[0].iov_len;
		if (destp + copy_len != rqst->rq_snd_buf.tail[0].iov_base) {
			memmove(destp + copy_len,
				rqst->rq_snd_buf.tail[0].iov_base, curlen);
			r_xprt->rx_stats.pullup_copy_count += curlen;
		}
		dprintk("RPC:       %s: tail destp 0x%p len %d\n",
			__func__, destp + copy_len, curlen);
		rqst->rq_svec[0].iov_len += curlen;
	}
	r_xprt->rx_stats.pullup_copy_count += copy_len;

	page_base = rqst->rq_snd_buf.page_base;
	ppages = rqst->rq_snd_buf.pages + (page_base >> PAGE_SHIFT);
	page_base &= ~PAGE_MASK;
	npages = PAGE_ALIGN(page_base+copy_len) >> PAGE_SHIFT;
	for (i = 0; copy_len && i < npages; i++) {
		curlen = PAGE_SIZE - page_base;
		if (curlen > copy_len)
			curlen = copy_len;
		dprintk("RPC:       %s: page %d destp 0x%p len %d curlen %d\n",
			__func__, i, destp, copy_len, curlen);
		srcp = kmap_atomic(ppages[i]);
		memcpy(destp, srcp+page_base, curlen);
		kunmap_atomic(srcp);
		rqst->rq_svec[0].iov_len += curlen;
		destp += curlen;
		copy_len -= curlen;
		page_base = 0;
	}
	/* header now contains entire send message */
	return pad;
}

/*
 * Marshal a request: the primary job of this routine is to choose
 * the transfer modes. See comments below.
 *
 * Uses multiple RDMA IOVs for a request:
 *  [0] -- RPC RDMA header, which uses memory from the *start* of the
 *         preregistered buffer that already holds the RPC data in
 *         its middle.
 *  [1] -- the RPC header/data, marshaled by RPC and the NFS protocol.
 *  [2] -- optional padding.
 *  [3] -- if padded, header only in [1] and data here.
 */

int
rpcrdma_marshal_req(struct rpc_rqst *rqst)
{
	struct rpc_xprt *xprt = rqst->rq_task->tk_xprt;
	struct rpcrdma_xprt *r_xprt = rpcx_to_rdmax(xprt);
	struct rpcrdma_req *req = rpcr_to_rdmar(rqst);
	char *base;
	size_t hdrlen, rpclen, padlen;
	enum rpcrdma_chunktype rtype, wtype;
	struct rpcrdma_msg *headerp;

	/*
	 * rpclen gets amount of data in first buffer, which is the
	 * pre-registered buffer.
	 */
	base = rqst->rq_svec[0].iov_base;
	rpclen = rqst->rq_svec[0].iov_len;

	/* build RDMA header in private area at front */
	headerp = (struct rpcrdma_msg *) req->rl_base;
	/* don't htonl XID, it's already done in request */
	headerp->rm_xid = rqst->rq_xid;
	headerp->rm_vers = xdr_one;
	headerp->rm_credit = htonl(r_xprt->rx_buf.rb_max_requests);
	headerp->rm_type = htonl(RDMA_MSG);
=======
 * Returns zero on success, or a negative errno if a failure occurred.
 * @xdr is advanced to the next position in the stream.
 *
 * Only a single Write chunk is currently supported.
 */
static int rpcrdma_encode_write_list(struct rpcrdma_xprt *r_xprt,
				     struct rpcrdma_req *req,
				     struct rpc_rqst *rqst,
				     enum rpcrdma_chunktype wtype)
{
	struct xdr_stream *xdr = &req->rl_stream;
	struct rpcrdma_ep *ep = r_xprt->rx_ep;
	struct rpcrdma_mr_seg *seg;
	struct rpcrdma_mr *mr;
	int nsegs, nchunks;
	__be32 *segcount;

	if (wtype != rpcrdma_writech)
		goto done;

	seg = req->rl_segments;
	nsegs = rpcrdma_convert_iovs(r_xprt, &rqst->rq_rcv_buf,
				     rqst->rq_rcv_buf.head[0].iov_len,
				     wtype, seg);
	if (nsegs < 0)
		return nsegs;

	if (xdr_stream_encode_item_present(xdr) < 0)
		return -EMSGSIZE;
	segcount = xdr_reserve_space(xdr, sizeof(*segcount));
	if (unlikely(!segcount))
		return -EMSGSIZE;
	/* Actual value encoded below */

	nchunks = 0;
	do {
		seg = rpcrdma_mr_prepare(r_xprt, req, seg, nsegs, true, &mr);
		if (IS_ERR(seg))
			return PTR_ERR(seg);

		if (encode_rdma_segment(xdr, mr) < 0)
			return -EMSGSIZE;

		trace_xprtrdma_chunk_write(rqst->rq_task, mr, nsegs);
		r_xprt->rx_stats.write_chunk_count++;
		r_xprt->rx_stats.total_rdma_request += mr->mr_length;
		nchunks++;
		nsegs -= mr->mr_nents;
	} while (nsegs);

	if (xdr_pad_size(rqst->rq_rcv_buf.page_len)) {
		if (encode_rdma_segment(xdr, ep->re_write_pad_mr) < 0)
			return -EMSGSIZE;

		trace_xprtrdma_chunk_wp(rqst->rq_task, ep->re_write_pad_mr,
					nsegs);
		r_xprt->rx_stats.write_chunk_count++;
		r_xprt->rx_stats.total_rdma_request += mr->mr_length;
		nchunks++;
		nsegs -= mr->mr_nents;
	}

	/* Update count of segments in this Write chunk */
	*segcount = cpu_to_be32(nchunks);

done:
	if (xdr_stream_encode_item_absent(xdr) < 0)
		return -EMSGSIZE;
	return 0;
}

/* Register and XDR encode the Reply chunk. Supports encoding an array
 * of plain segments that belong to a single write (reply) chunk.
 *
 * Encoding key for single-list chunks (HLOO = Handle32 Length32 Offset64):
 *
 *  Reply chunk (a counted array):
 *   N elements:
 *    1 - N - HLOO - HLOO - ... - HLOO
 *
 * Returns zero on success, or a negative errno if a failure occurred.
 * @xdr is advanced to the next position in the stream.
 */
static int rpcrdma_encode_reply_chunk(struct rpcrdma_xprt *r_xprt,
				      struct rpcrdma_req *req,
				      struct rpc_rqst *rqst,
				      enum rpcrdma_chunktype wtype)
{
	struct xdr_stream *xdr = &req->rl_stream;
	struct rpcrdma_mr_seg *seg;
	struct rpcrdma_mr *mr;
	int nsegs, nchunks;
	__be32 *segcount;

	if (wtype != rpcrdma_replych) {
		if (xdr_stream_encode_item_absent(xdr) < 0)
			return -EMSGSIZE;
		return 0;
	}

	seg = req->rl_segments;
	nsegs = rpcrdma_convert_iovs(r_xprt, &rqst->rq_rcv_buf, 0, wtype, seg);
	if (nsegs < 0)
		return nsegs;

	if (xdr_stream_encode_item_present(xdr) < 0)
		return -EMSGSIZE;
	segcount = xdr_reserve_space(xdr, sizeof(*segcount));
	if (unlikely(!segcount))
		return -EMSGSIZE;
	/* Actual value encoded below */

	nchunks = 0;
	do {
		seg = rpcrdma_mr_prepare(r_xprt, req, seg, nsegs, true, &mr);
		if (IS_ERR(seg))
			return PTR_ERR(seg);

		if (encode_rdma_segment(xdr, mr) < 0)
			return -EMSGSIZE;

		trace_xprtrdma_chunk_reply(rqst->rq_task, mr, nsegs);
		r_xprt->rx_stats.reply_chunk_count++;
		r_xprt->rx_stats.total_rdma_request += mr->mr_length;
		nchunks++;
		nsegs -= mr->mr_nents;
	} while (nsegs);

	/* Update count of segments in the Reply chunk */
	*segcount = cpu_to_be32(nchunks);

	return 0;
}

static void rpcrdma_sendctx_done(struct kref *kref)
{
	struct rpcrdma_req *req =
		container_of(kref, struct rpcrdma_req, rl_kref);
	struct rpcrdma_rep *rep = req->rl_reply;

	rpcrdma_complete_rqst(rep);
	rep->rr_rxprt->rx_stats.reply_waits_for_send++;
}

/**
 * rpcrdma_sendctx_unmap - DMA-unmap Send buffer
 * @sc: sendctx containing SGEs to unmap
 *
 */
void rpcrdma_sendctx_unmap(struct rpcrdma_sendctx *sc)
{
	struct rpcrdma_regbuf *rb = sc->sc_req->rl_sendbuf;
	struct ib_sge *sge;

	if (!sc->sc_unmap_count)
		return;

	/* The first two SGEs contain the transport header and
	 * the inline buffer. These are always left mapped so
	 * they can be cheaply re-used.
	 */
	for (sge = &sc->sc_sges[2]; sc->sc_unmap_count;
	     ++sge, --sc->sc_unmap_count)
		ib_dma_unmap_page(rdmab_device(rb), sge->addr, sge->length,
				  DMA_TO_DEVICE);

	kref_put(&sc->sc_req->rl_kref, rpcrdma_sendctx_done);
}

/* Prepare an SGE for the RPC-over-RDMA transport header.
 */
static void rpcrdma_prepare_hdr_sge(struct rpcrdma_xprt *r_xprt,
				    struct rpcrdma_req *req, u32 len)
{
	struct rpcrdma_sendctx *sc = req->rl_sendctx;
	struct rpcrdma_regbuf *rb = req->rl_rdmabuf;
	struct ib_sge *sge = &sc->sc_sges[req->rl_wr.num_sge++];

	sge->addr = rdmab_addr(rb);
	sge->length = len;
	sge->lkey = rdmab_lkey(rb);

	ib_dma_sync_single_for_device(rdmab_device(rb), sge->addr, sge->length,
				      DMA_TO_DEVICE);
}

/* The head iovec is straightforward, as it is usually already
 * DMA-mapped. Sync the content that has changed.
 */
static bool rpcrdma_prepare_head_iov(struct rpcrdma_xprt *r_xprt,
				     struct rpcrdma_req *req, unsigned int len)
{
	struct rpcrdma_sendctx *sc = req->rl_sendctx;
	struct ib_sge *sge = &sc->sc_sges[req->rl_wr.num_sge++];
	struct rpcrdma_regbuf *rb = req->rl_sendbuf;

	if (!rpcrdma_regbuf_dma_map(r_xprt, rb))
		return false;

	sge->addr = rdmab_addr(rb);
	sge->length = len;
	sge->lkey = rdmab_lkey(rb);

	ib_dma_sync_single_for_device(rdmab_device(rb), sge->addr, sge->length,
				      DMA_TO_DEVICE);
	return true;
}

/* If there is a page list present, DMA map and prepare an
 * SGE for each page to be sent.
 */
static bool rpcrdma_prepare_pagelist(struct rpcrdma_req *req,
				     struct xdr_buf *xdr)
{
	struct rpcrdma_sendctx *sc = req->rl_sendctx;
	struct rpcrdma_regbuf *rb = req->rl_sendbuf;
	unsigned int page_base, len, remaining;
	struct page **ppages;
	struct ib_sge *sge;

	ppages = xdr->pages + (xdr->page_base >> PAGE_SHIFT);
	page_base = offset_in_page(xdr->page_base);
	remaining = xdr->page_len;
	while (remaining) {
		sge = &sc->sc_sges[req->rl_wr.num_sge++];
		len = min_t(unsigned int, PAGE_SIZE - page_base, remaining);
		sge->addr = ib_dma_map_page(rdmab_device(rb), *ppages,
					    page_base, len, DMA_TO_DEVICE);
		if (ib_dma_mapping_error(rdmab_device(rb), sge->addr))
			goto out_mapping_err;

		sge->length = len;
		sge->lkey = rdmab_lkey(rb);

		sc->sc_unmap_count++;
		ppages++;
		remaining -= len;
		page_base = 0;
	}

	return true;

out_mapping_err:
	trace_xprtrdma_dma_maperr(sge->addr);
	return false;
}

/* The tail iovec may include an XDR pad for the page list,
 * as well as additional content, and may not reside in the
 * same page as the head iovec.
 */
static bool rpcrdma_prepare_tail_iov(struct rpcrdma_req *req,
				     struct xdr_buf *xdr,
				     unsigned int page_base, unsigned int len)
{
	struct rpcrdma_sendctx *sc = req->rl_sendctx;
	struct ib_sge *sge = &sc->sc_sges[req->rl_wr.num_sge++];
	struct rpcrdma_regbuf *rb = req->rl_sendbuf;
	struct page *page = virt_to_page(xdr->tail[0].iov_base);

	sge->addr = ib_dma_map_page(rdmab_device(rb), page, page_base, len,
				    DMA_TO_DEVICE);
	if (ib_dma_mapping_error(rdmab_device(rb), sge->addr))
		goto out_mapping_err;

	sge->length = len;
	sge->lkey = rdmab_lkey(rb);
	++sc->sc_unmap_count;
	return true;

out_mapping_err:
	trace_xprtrdma_dma_maperr(sge->addr);
	return false;
}

/* Copy the tail to the end of the head buffer.
 */
static void rpcrdma_pullup_tail_iov(struct rpcrdma_xprt *r_xprt,
				    struct rpcrdma_req *req,
				    struct xdr_buf *xdr)
{
	unsigned char *dst;

	dst = (unsigned char *)xdr->head[0].iov_base;
	dst += xdr->head[0].iov_len + xdr->page_len;
	memmove(dst, xdr->tail[0].iov_base, xdr->tail[0].iov_len);
	r_xprt->rx_stats.pullup_copy_count += xdr->tail[0].iov_len;
}

/* Copy pagelist content into the head buffer.
 */
static void rpcrdma_pullup_pagelist(struct rpcrdma_xprt *r_xprt,
				    struct rpcrdma_req *req,
				    struct xdr_buf *xdr)
{
	unsigned int len, page_base, remaining;
	struct page **ppages;
	unsigned char *src, *dst;

	dst = (unsigned char *)xdr->head[0].iov_base;
	dst += xdr->head[0].iov_len;
	ppages = xdr->pages + (xdr->page_base >> PAGE_SHIFT);
	page_base = offset_in_page(xdr->page_base);
	remaining = xdr->page_len;
	while (remaining) {
		src = page_address(*ppages);
		src += page_base;
		len = min_t(unsigned int, PAGE_SIZE - page_base, remaining);
		memcpy(dst, src, len);
		r_xprt->rx_stats.pullup_copy_count += len;

		ppages++;
		dst += len;
		remaining -= len;
		page_base = 0;
	}
}

/* Copy the contents of @xdr into @rl_sendbuf and DMA sync it.
 * When the head, pagelist, and tail are small, a pull-up copy
 * is considerably less costly than DMA mapping the components
 * of @xdr.
 *
 * Assumptions:
 *  - the caller has already verified that the total length
 *    of the RPC Call body will fit into @rl_sendbuf.
 */
static bool rpcrdma_prepare_noch_pullup(struct rpcrdma_xprt *r_xprt,
					struct rpcrdma_req *req,
					struct xdr_buf *xdr)
{
	if (unlikely(xdr->tail[0].iov_len))
		rpcrdma_pullup_tail_iov(r_xprt, req, xdr);

	if (unlikely(xdr->page_len))
		rpcrdma_pullup_pagelist(r_xprt, req, xdr);

	/* The whole RPC message resides in the head iovec now */
	return rpcrdma_prepare_head_iov(r_xprt, req, xdr->len);
}

static bool rpcrdma_prepare_noch_mapped(struct rpcrdma_xprt *r_xprt,
					struct rpcrdma_req *req,
					struct xdr_buf *xdr)
{
	struct kvec *tail = &xdr->tail[0];

	if (!rpcrdma_prepare_head_iov(r_xprt, req, xdr->head[0].iov_len))
		return false;
	if (xdr->page_len)
		if (!rpcrdma_prepare_pagelist(req, xdr))
			return false;
	if (tail->iov_len)
		if (!rpcrdma_prepare_tail_iov(req, xdr,
					      offset_in_page(tail->iov_base),
					      tail->iov_len))
			return false;

	if (req->rl_sendctx->sc_unmap_count)
		kref_get(&req->rl_kref);
	return true;
}

static bool rpcrdma_prepare_readch(struct rpcrdma_xprt *r_xprt,
				   struct rpcrdma_req *req,
				   struct xdr_buf *xdr)
{
	if (!rpcrdma_prepare_head_iov(r_xprt, req, xdr->head[0].iov_len))
		return false;

	/* If there is a Read chunk, the page list is being handled
	 * via explicit RDMA, and thus is skipped here.
	 */

	/* Do not include the tail if it is only an XDR pad */
	if (xdr->tail[0].iov_len > 3) {
		unsigned int page_base, len;

		/* If the content in the page list is an odd length,
		 * xdr_write_pages() adds a pad at the beginning of
		 * the tail iovec. Force the tail's non-pad content to
		 * land at the next XDR position in the Send message.
		 */
		page_base = offset_in_page(xdr->tail[0].iov_base);
		len = xdr->tail[0].iov_len;
		page_base += len & 3;
		len -= len & 3;
		if (!rpcrdma_prepare_tail_iov(req, xdr, page_base, len))
			return false;
		kref_get(&req->rl_kref);
	}

	return true;
}

/**
 * rpcrdma_prepare_send_sges - Construct SGEs for a Send WR
 * @r_xprt: controlling transport
 * @req: context of RPC Call being marshalled
 * @hdrlen: size of transport header, in bytes
 * @xdr: xdr_buf containing RPC Call
 * @rtype: chunk type being encoded
 *
 * Returns 0 on success; otherwise a negative errno is returned.
 */
inline int rpcrdma_prepare_send_sges(struct rpcrdma_xprt *r_xprt,
				     struct rpcrdma_req *req, u32 hdrlen,
				     struct xdr_buf *xdr,
				     enum rpcrdma_chunktype rtype)
{
	int ret;

	ret = -EAGAIN;
	req->rl_sendctx = rpcrdma_sendctx_get_locked(r_xprt);
	if (!req->rl_sendctx)
		goto out_nosc;
	req->rl_sendctx->sc_unmap_count = 0;
	req->rl_sendctx->sc_req = req;
	kref_init(&req->rl_kref);
	req->rl_wr.wr_cqe = &req->rl_sendctx->sc_cqe;
	req->rl_wr.sg_list = req->rl_sendctx->sc_sges;
	req->rl_wr.num_sge = 0;
	req->rl_wr.opcode = IB_WR_SEND;

	rpcrdma_prepare_hdr_sge(r_xprt, req, hdrlen);

	ret = -EIO;
	switch (rtype) {
	case rpcrdma_noch_pullup:
		if (!rpcrdma_prepare_noch_pullup(r_xprt, req, xdr))
			goto out_unmap;
		break;
	case rpcrdma_noch_mapped:
		if (!rpcrdma_prepare_noch_mapped(r_xprt, req, xdr))
			goto out_unmap;
		break;
	case rpcrdma_readch:
		if (!rpcrdma_prepare_readch(r_xprt, req, xdr))
			goto out_unmap;
		break;
	case rpcrdma_areadch:
		break;
	default:
		goto out_unmap;
	}

	return 0;

out_unmap:
	rpcrdma_sendctx_unmap(req->rl_sendctx);
out_nosc:
	trace_xprtrdma_prepsend_failed(&req->rl_slot, ret);
	return ret;
}

/**
 * rpcrdma_marshal_req - Marshal and send one RPC request
 * @r_xprt: controlling transport
 * @rqst: RPC request to be marshaled
 *
 * For the RPC in "rqst", this function:
 *  - Chooses the transfer mode (eg., RDMA_MSG or RDMA_NOMSG)
 *  - Registers Read, Write, and Reply chunks
 *  - Constructs the transport header
 *  - Posts a Send WR to send the transport header and request
 *
 * Returns:
 *	%0 if the RPC was sent successfully,
 *	%-ENOTCONN if the connection was lost,
 *	%-EAGAIN if the caller should call again with the same arguments,
 *	%-ENOBUFS if the caller should call again after a delay,
 *	%-EMSGSIZE if the transport header is too small,
 *	%-EIO if a permanent problem occurred while marshaling.
 */
int
rpcrdma_marshal_req(struct rpcrdma_xprt *r_xprt, struct rpc_rqst *rqst)
{
	struct rpcrdma_req *req = rpcr_to_rdmar(rqst);
	struct xdr_stream *xdr = &req->rl_stream;
	enum rpcrdma_chunktype rtype, wtype;
	struct xdr_buf *buf = &rqst->rq_snd_buf;
	bool ddp_allowed;
	__be32 *p;
	int ret;

	if (unlikely(rqst->rq_rcv_buf.flags & XDRBUF_SPARSE_PAGES)) {
		ret = rpcrdma_alloc_sparse_pages(&rqst->rq_rcv_buf);
		if (ret)
			return ret;
	}

	rpcrdma_set_xdrlen(&req->rl_hdrbuf, 0);
	xdr_init_encode(xdr, &req->rl_hdrbuf, rdmab_data(req->rl_rdmabuf),
			rqst);

	/* Fixed header fields */
	ret = -EMSGSIZE;
	p = xdr_reserve_space(xdr, 4 * sizeof(*p));
	if (!p)
		goto out_err;
	*p++ = rqst->rq_xid;
	*p++ = rpcrdma_version;
	*p++ = r_xprt->rx_buf.rb_max_requests;

	/* When the ULP employs a GSS flavor that guarantees integrity
	 * or privacy, direct data placement of individual data items
	 * is not allowed.
	 */
	ddp_allowed = !test_bit(RPCAUTH_AUTH_DATATOUCH,
				&rqst->rq_cred->cr_auth->au_flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Chunks needed for results?
	 *
	 * o If the expected result is under the inline threshold, all ops
<<<<<<< HEAD
	 *   return as inline (but see later).
	 * o Large non-read ops return as a single reply chunk.
	 * o Large read ops return data as write chunk(s), header as inline.
	 *
	 * Note: the NFS code sending down multiple result segments implies
	 * the op is one of read, readdir[plus], readlink or NFSv4 getacl.
	 */

	/*
	 * This code can handle read chunks, write chunks OR reply
	 * chunks -- only one type. If the request is too big to fit
	 * inline, then we will choose read chunks. If the request is
	 * a READ, then use write chunks to separate the file data
	 * into pages; otherwise use reply chunks.
	 */
	if (rqst->rq_rcv_buf.buflen <= RPCRDMA_INLINE_READ_THRESHOLD(rqst))
		wtype = rpcrdma_noch;
	else if (rqst->rq_rcv_buf.page_len == 0)
		wtype = rpcrdma_replych;
	else if (rqst->rq_rcv_buf.flags & XDRBUF_READ)
=======
	 *   return as inline.
	 * o Large read ops return data as write chunk(s), header as
	 *   inline.
	 * o Large non-read ops return as a single reply chunk.
	 */
	if (rpcrdma_results_inline(r_xprt, rqst))
		wtype = rpcrdma_noch;
	else if ((ddp_allowed && rqst->rq_rcv_buf.flags & XDRBUF_READ) &&
		 rpcrdma_nonpayload_inline(r_xprt, rqst))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		wtype = rpcrdma_writech;
	else
		wtype = rpcrdma_replych;

	/*
	 * Chunks needed for arguments?
	 *
	 * o If the total request is under the inline threshold, all ops
	 *   are sent as inline.
<<<<<<< HEAD
	 * o Large non-write ops are sent with the entire message as a
	 *   single read chunk (protocol 0-position special case).
	 * o Large write ops transmit data as read chunk(s), header as
	 *   inline.
	 *
	 * Note: the NFS code sending down multiple argument segments
	 * implies the op is a write.
	 * TBD check NFSv4 setacl
	 */
	if (rqst->rq_snd_buf.len <= RPCRDMA_INLINE_WRITE_THRESHOLD(rqst))
		rtype = rpcrdma_noch;
	else if (rqst->rq_snd_buf.page_len == 0)
		rtype = rpcrdma_areadch;
	else
		rtype = rpcrdma_readch;

	/* The following simplification is not true forever */
	if (rtype != rpcrdma_noch && wtype == rpcrdma_replych)
		wtype = rpcrdma_noch;
	BUG_ON(rtype != rpcrdma_noch && wtype != rpcrdma_noch);

	if (r_xprt->rx_ia.ri_memreg_strategy == RPCRDMA_BOUNCEBUFFERS &&
	    (rtype != rpcrdma_noch || wtype != rpcrdma_noch)) {
		/* forced to "pure inline"? */
		dprintk("RPC:       %s: too much data (%d/%d) for inline\n",
			__func__, rqst->rq_rcv_buf.len, rqst->rq_snd_buf.len);
		return -1;
	}

	hdrlen = 28; /*sizeof *headerp;*/
	padlen = 0;

	/*
	 * Pull up any extra send data into the preregistered buffer.
	 * When padding is in use and applies to the transfer, insert
	 * it and change the message type.
	 */
	if (rtype == rpcrdma_noch) {

		padlen = rpcrdma_inline_pullup(rqst,
						RPCRDMA_INLINE_PAD_VALUE(rqst));

		if (padlen) {
			headerp->rm_type = htonl(RDMA_MSGP);
			headerp->rm_body.rm_padded.rm_align =
				htonl(RPCRDMA_INLINE_PAD_VALUE(rqst));
			headerp->rm_body.rm_padded.rm_thresh =
				htonl(RPCRDMA_INLINE_PAD_THRESH);
			headerp->rm_body.rm_padded.rm_pempty[0] = xdr_zero;
			headerp->rm_body.rm_padded.rm_pempty[1] = xdr_zero;
			headerp->rm_body.rm_padded.rm_pempty[2] = xdr_zero;
			hdrlen += 2 * sizeof(u32); /* extra words in padhdr */
			BUG_ON(wtype != rpcrdma_noch);

		} else {
			headerp->rm_body.rm_nochunks.rm_empty[0] = xdr_zero;
			headerp->rm_body.rm_nochunks.rm_empty[1] = xdr_zero;
			headerp->rm_body.rm_nochunks.rm_empty[2] = xdr_zero;
			/* new length after pullup */
			rpclen = rqst->rq_svec[0].iov_len;
			/*
			 * Currently we try to not actually use read inline.
			 * Reply chunks have the desirable property that
			 * they land, packed, directly in the target buffers
			 * without headers, so they require no fixup. The
			 * additional RDMA Write op sends the same amount
			 * of data, streams on-the-wire and adds no overhead
			 * on receive. Therefore, we request a reply chunk
			 * for non-writes wherever feasible and efficient.
			 */
			if (wtype == rpcrdma_noch &&
			    r_xprt->rx_ia.ri_memreg_strategy > RPCRDMA_REGISTER)
				wtype = rpcrdma_replych;
		}
	}

	/*
	 * Marshal chunks. This routine will return the header length
	 * consumed by marshaling.
	 */
	if (rtype != rpcrdma_noch) {
		hdrlen = rpcrdma_create_chunks(rqst,
					&rqst->rq_snd_buf, headerp, rtype);
		wtype = rtype;	/* simplify dprintk */

	} else if (wtype != rpcrdma_noch) {
		hdrlen = rpcrdma_create_chunks(rqst,
					&rqst->rq_rcv_buf, headerp, wtype);
	}

	if (hdrlen == 0)
		return -1;

	dprintk("RPC:       %s: %s: hdrlen %zd rpclen %zd padlen %zd"
		" headerp 0x%p base 0x%p lkey 0x%x\n",
		__func__, transfertypes[wtype], hdrlen, rpclen, padlen,
		headerp, base, req->rl_iov.lkey);

	/*
	 * initialize send_iov's - normally only two: rdma chunk header and
	 * single preregistered RPC header buffer, but if padding is present,
	 * then use a preregistered (and zeroed) pad buffer between the RPC
	 * header and any write data. In all non-rdma cases, any following
	 * data has been copied into the RPC header buffer.
	 */
	req->rl_send_iov[0].addr = req->rl_iov.addr;
	req->rl_send_iov[0].length = hdrlen;
	req->rl_send_iov[0].lkey = req->rl_iov.lkey;

	req->rl_send_iov[1].addr = req->rl_iov.addr + (base - req->rl_base);
	req->rl_send_iov[1].length = rpclen;
	req->rl_send_iov[1].lkey = req->rl_iov.lkey;

	req->rl_niovs = 2;

	if (padlen) {
		struct rpcrdma_ep *ep = &r_xprt->rx_ep;

		req->rl_send_iov[2].addr = ep->rep_pad.addr;
		req->rl_send_iov[2].length = padlen;
		req->rl_send_iov[2].lkey = ep->rep_pad.lkey;

		req->rl_send_iov[3].addr = req->rl_send_iov[1].addr + rpclen;
		req->rl_send_iov[3].length = rqst->rq_slen - rpclen;
		req->rl_send_iov[3].lkey = req->rl_iov.lkey;

		req->rl_niovs = 4;
	}

	return 0;
}

/*
 * Chase down a received write or reply chunklist to get length
 * RDMA'd by server. See map at rpcrdma_create_chunks()! :-)
 */
static int
rpcrdma_count_chunks(struct rpcrdma_rep *rep, unsigned int max, int wrchunk, __be32 **iptrp)
{
	unsigned int i, total_len;
	struct rpcrdma_write_chunk *cur_wchunk;

	i = ntohl(**iptrp);	/* get array count */
	if (i > max)
		return -1;
	cur_wchunk = (struct rpcrdma_write_chunk *) (*iptrp + 1);
	total_len = 0;
	while (i--) {
		struct rpcrdma_segment *seg = &cur_wchunk->wc_target;
		ifdebug(FACILITY) {
			u64 off;
			xdr_decode_hyper((__be32 *)&seg->rs_offset, &off);
			dprintk("RPC:       %s: chunk %d@0x%llx:0x%x\n",
				__func__,
				ntohl(seg->rs_length),
				(unsigned long long)off,
				ntohl(seg->rs_handle));
		}
		total_len += ntohl(seg->rs_length);
		++cur_wchunk;
	}
	/* check and adjust for properly terminated write chunk */
	if (wrchunk) {
		__be32 *w = (__be32 *) cur_wchunk;
		if (*w++ != xdr_zero)
			return -1;
		cur_wchunk = (struct rpcrdma_write_chunk *) w;
	}
	if ((char *) cur_wchunk > rep->rr_base + rep->rr_len)
		return -1;

	*iptrp = (__be32 *) cur_wchunk;
	return total_len;
}

/*
 * Scatter inline received data back into provided iov's.
 */
static void
rpcrdma_inline_fixup(struct rpc_rqst *rqst, char *srcp, int copy_len, int pad)
{
	int i, npages, curlen, olen;
=======
	 * o Large write ops transmit data as read chunk(s), header as
	 *   inline.
	 * o Large non-write ops are sent with the entire message as a
	 *   single read chunk (protocol 0-position special case).
	 *
	 * This assumes that the upper layer does not present a request
	 * that both has a data payload, and whose non-data arguments
	 * by themselves are larger than the inline threshold.
	 */
	if (rpcrdma_args_inline(r_xprt, rqst)) {
		*p++ = rdma_msg;
		rtype = buf->len < rdmab_length(req->rl_sendbuf) ?
			rpcrdma_noch_pullup : rpcrdma_noch_mapped;
	} else if (ddp_allowed && buf->flags & XDRBUF_WRITE) {
		*p++ = rdma_msg;
		rtype = rpcrdma_readch;
	} else {
		r_xprt->rx_stats.nomsg_call_count++;
		*p++ = rdma_nomsg;
		rtype = rpcrdma_areadch;
	}

	/* This implementation supports the following combinations
	 * of chunk lists in one RPC-over-RDMA Call message:
	 *
	 *   - Read list
	 *   - Write list
	 *   - Reply chunk
	 *   - Read list + Reply chunk
	 *
	 * It might not yet support the following combinations:
	 *
	 *   - Read list + Write list
	 *
	 * It does not support the following combinations:
	 *
	 *   - Write list + Reply chunk
	 *   - Read list + Write list + Reply chunk
	 *
	 * This implementation supports only a single chunk in each
	 * Read or Write list. Thus for example the client cannot
	 * send a Call message with a Position Zero Read chunk and a
	 * regular Read chunk at the same time.
	 */
	ret = rpcrdma_encode_read_list(r_xprt, req, rqst, rtype);
	if (ret)
		goto out_err;
	ret = rpcrdma_encode_write_list(r_xprt, req, rqst, wtype);
	if (ret)
		goto out_err;
	ret = rpcrdma_encode_reply_chunk(r_xprt, req, rqst, wtype);
	if (ret)
		goto out_err;

	ret = rpcrdma_prepare_send_sges(r_xprt, req, req->rl_hdrbuf.len,
					buf, rtype);
	if (ret)
		goto out_err;

	trace_xprtrdma_marshal(req, rtype, wtype);
	return 0;

out_err:
	trace_xprtrdma_marshal_failed(rqst, ret);
	r_xprt->rx_stats.failed_marshal_count++;
	frwr_reset(req);
	return ret;
}

static void __rpcrdma_update_cwnd_locked(struct rpc_xprt *xprt,
					 struct rpcrdma_buffer *buf,
					 u32 grant)
{
	buf->rb_credits = grant;
	xprt->cwnd = grant << RPC_CWNDSHIFT;
}

static void rpcrdma_update_cwnd(struct rpcrdma_xprt *r_xprt, u32 grant)
{
	struct rpc_xprt *xprt = &r_xprt->rx_xprt;

	spin_lock(&xprt->transport_lock);
	__rpcrdma_update_cwnd_locked(xprt, &r_xprt->rx_buf, grant);
	spin_unlock(&xprt->transport_lock);
}

/**
 * rpcrdma_reset_cwnd - Reset the xprt's congestion window
 * @r_xprt: controlling transport instance
 *
 * Prepare @r_xprt for the next connection by reinitializing
 * its credit grant to one (see RFC 8166, Section 3.3.3).
 */
void rpcrdma_reset_cwnd(struct rpcrdma_xprt *r_xprt)
{
	struct rpc_xprt *xprt = &r_xprt->rx_xprt;

	spin_lock(&xprt->transport_lock);
	xprt->cong = 0;
	__rpcrdma_update_cwnd_locked(xprt, &r_xprt->rx_buf, 1);
	spin_unlock(&xprt->transport_lock);
}

/**
 * rpcrdma_inline_fixup - Scatter inline received data into rqst's iovecs
 * @rqst: controlling RPC request
 * @srcp: points to RPC message payload in receive buffer
 * @copy_len: remaining length of receive buffer content
 * @pad: Write chunk pad bytes needed (zero for pure inline)
 *
 * The upper layer has set the maximum number of bytes it can
 * receive in each component of rq_rcv_buf. These values are set in
 * the head.iov_len, page_len, tail.iov_len, and buflen fields.
 *
 * Unlike the TCP equivalent (xdr_partial_copy_from_skb), in
 * many cases this function simply updates iov_base pointers in
 * rq_rcv_buf to point directly to the received reply data, to
 * avoid copying reply data.
 *
 * Returns the count of bytes which had to be memcopied.
 */
static unsigned long
rpcrdma_inline_fixup(struct rpc_rqst *rqst, char *srcp, int copy_len, int pad)
{
	unsigned long fixup_copy_count;
	int i, npages, curlen;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	char *destp;
	struct page **ppages;
	int page_base;

<<<<<<< HEAD
	curlen = rqst->rq_rcv_buf.head[0].iov_len;
	if (curlen > copy_len) {	/* write chunk header fixup */
		curlen = copy_len;
		rqst->rq_rcv_buf.head[0].iov_len = curlen;
	}

	dprintk("RPC:       %s: srcp 0x%p len %d hdrlen %d\n",
		__func__, srcp, copy_len, curlen);

	/* Shift pointer for first receive segment only */
	rqst->rq_rcv_buf.head[0].iov_base = srcp;
	srcp += curlen;
	copy_len -= curlen;

	olen = copy_len;
	i = 0;
	rpcx_to_rdmax(rqst->rq_xprt)->rx_stats.fixup_copy_count += olen;
	page_base = rqst->rq_rcv_buf.page_base;
	ppages = rqst->rq_rcv_buf.pages + (page_base >> PAGE_SHIFT);
	page_base &= ~PAGE_MASK;

	if (copy_len && rqst->rq_rcv_buf.page_len) {
		npages = PAGE_ALIGN(page_base +
			rqst->rq_rcv_buf.page_len) >> PAGE_SHIFT;
		for (; i < npages; i++) {
			curlen = PAGE_SIZE - page_base;
			if (curlen > copy_len)
				curlen = copy_len;
			dprintk("RPC:       %s: page %d"
				" srcp 0x%p len %d curlen %d\n",
				__func__, i, srcp, copy_len, curlen);
=======
	/* The head iovec is redirected to the RPC reply message
	 * in the receive buffer, to avoid a memcopy.
	 */
	rqst->rq_rcv_buf.head[0].iov_base = srcp;
	rqst->rq_private_buf.head[0].iov_base = srcp;

	/* The contents of the receive buffer that follow
	 * head.iov_len bytes are copied into the page list.
	 */
	curlen = rqst->rq_rcv_buf.head[0].iov_len;
	if (curlen > copy_len)
		curlen = copy_len;
	srcp += curlen;
	copy_len -= curlen;

	ppages = rqst->rq_rcv_buf.pages +
		(rqst->rq_rcv_buf.page_base >> PAGE_SHIFT);
	page_base = offset_in_page(rqst->rq_rcv_buf.page_base);
	fixup_copy_count = 0;
	if (copy_len && rqst->rq_rcv_buf.page_len) {
		int pagelist_len;

		pagelist_len = rqst->rq_rcv_buf.page_len;
		if (pagelist_len > copy_len)
			pagelist_len = copy_len;
		npages = PAGE_ALIGN(page_base + pagelist_len) >> PAGE_SHIFT;
		for (i = 0; i < npages; i++) {
			curlen = PAGE_SIZE - page_base;
			if (curlen > pagelist_len)
				curlen = pagelist_len;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			destp = kmap_atomic(ppages[i]);
			memcpy(destp + page_base, srcp, curlen);
			flush_dcache_page(ppages[i]);
			kunmap_atomic(destp);
			srcp += curlen;
			copy_len -= curlen;
<<<<<<< HEAD
			if (copy_len == 0)
				break;
			page_base = 0;
		}
		rqst->rq_rcv_buf.page_len = olen - copy_len;
	} else
		rqst->rq_rcv_buf.page_len = 0;

	if (copy_len && rqst->rq_rcv_buf.tail[0].iov_len) {
		curlen = copy_len;
		if (curlen > rqst->rq_rcv_buf.tail[0].iov_len)
			curlen = rqst->rq_rcv_buf.tail[0].iov_len;
		if (rqst->rq_rcv_buf.tail[0].iov_base != srcp)
			memmove(rqst->rq_rcv_buf.tail[0].iov_base, srcp, curlen);
		dprintk("RPC:       %s: tail srcp 0x%p len %d curlen %d\n",
			__func__, srcp, copy_len, curlen);
		rqst->rq_rcv_buf.tail[0].iov_len = curlen;
		copy_len -= curlen; ++i;
	} else
		rqst->rq_rcv_buf.tail[0].iov_len = 0;

	if (pad) {
		/* implicit padding on terminal chunk */
		unsigned char *p = rqst->rq_rcv_buf.tail[0].iov_base;
		while (pad--)
			p[rqst->rq_rcv_buf.tail[0].iov_len++] = 0;
	}

	if (copy_len)
		dprintk("RPC:       %s: %d bytes in"
			" %d extra segments (%d lost)\n",
			__func__, olen, i, copy_len);

	/* TBD avoid a warning from call_decode() */
	rqst->rq_private_buf = rqst->rq_rcv_buf;
}

/*
 * This function is called when an async event is posted to
 * the connection which changes the connection state. All it
 * does at this point is mark the connection up/down, the rpc
 * timers do the rest.
 */
void
rpcrdma_conn_func(struct rpcrdma_ep *ep)
{
	struct rpc_xprt *xprt = ep->rep_xprt;

	spin_lock_bh(&xprt->transport_lock);
	if (++xprt->connect_cookie == 0)	/* maintain a reserved value */
		++xprt->connect_cookie;
	if (ep->rep_connected > 0) {
		if (!xprt_test_and_set_connected(xprt))
			xprt_wake_pending_tasks(xprt, 0);
	} else {
		if (xprt_test_and_clear_connected(xprt))
			xprt_wake_pending_tasks(xprt, -ENOTCONN);
	}
	spin_unlock_bh(&xprt->transport_lock);
}

/*
 * This function is called when memory window unbind which we are waiting
 * for completes. Just use rr_func (zeroed by upcall) to signal completion.
 */
static void
rpcrdma_unbind_func(struct rpcrdma_rep *rep)
{
	wake_up(&rep->rr_unbind);
}

/*
 * Called as a tasklet to do req/reply match and complete a request
 * Errors must result in the RPC task either being awakened, or
 * allowed to timeout, to discover the errors at that time.
 */
void
rpcrdma_reply_handler(struct rpcrdma_rep *rep)
{
	struct rpcrdma_msg *headerp;
	struct rpcrdma_req *req;
	struct rpc_rqst *rqst;
	struct rpc_xprt *xprt = rep->rr_xprt;
	struct rpcrdma_xprt *r_xprt = rpcx_to_rdmax(xprt);
	__be32 *iptr;
	int i, rdmalen, status;

	/* Check status. If bad, signal disconnect and return rep to pool */
	if (rep->rr_len == ~0U) {
		rpcrdma_recv_buffer_put(rep);
		if (r_xprt->rx_ep.rep_connected == 1) {
			r_xprt->rx_ep.rep_connected = -EIO;
			rpcrdma_conn_func(&r_xprt->rx_ep);
		}
		return;
	}
	if (rep->rr_len < 28) {
		dprintk("RPC:       %s: short/invalid reply\n", __func__);
		goto repost;
	}
	headerp = (struct rpcrdma_msg *) rep->rr_base;
	if (headerp->rm_vers != xdr_one) {
		dprintk("RPC:       %s: invalid version %d\n",
			__func__, ntohl(headerp->rm_vers));
		goto repost;
	}

	/* Get XID and try for a match. */
	spin_lock(&xprt->transport_lock);
	rqst = xprt_lookup_rqst(xprt, headerp->rm_xid);
	if (rqst == NULL) {
		spin_unlock(&xprt->transport_lock);
		dprintk("RPC:       %s: reply 0x%p failed "
			"to match any request xid 0x%08x len %d\n",
			__func__, rep, headerp->rm_xid, rep->rr_len);
repost:
		r_xprt->rx_stats.bad_reply_count++;
		rep->rr_func = rpcrdma_reply_handler;
		if (rpcrdma_ep_post_recv(&r_xprt->rx_ia, &r_xprt->rx_ep, rep))
			rpcrdma_recv_buffer_put(rep);

		return;
	}

	/* get request object */
	req = rpcr_to_rdmar(rqst);
	if (req->rl_reply) {
		spin_unlock(&xprt->transport_lock);
		dprintk("RPC:       %s: duplicate reply 0x%p to RPC "
			"request 0x%p: xid 0x%08x\n", __func__, rep, req,
			headerp->rm_xid);
		goto repost;
	}

	dprintk("RPC:       %s: reply 0x%p completes request 0x%p\n"
		"                   RPC request 0x%p xid 0x%08x\n",
			__func__, rep, req, rqst, headerp->rm_xid);

	/* from here on, the reply is no longer an orphan */
	req->rl_reply = rep;

	/* check for expected message types */
	/* The order of some of these tests is important. */
	switch (headerp->rm_type) {
	case htonl(RDMA_MSG):
		/* never expect read chunks */
		/* never expect reply chunks (two ways to check) */
		/* never expect write chunks without having offered RDMA */
		if (headerp->rm_body.rm_chunks[0] != xdr_zero ||
		    (headerp->rm_body.rm_chunks[1] == xdr_zero &&
		     headerp->rm_body.rm_chunks[2] != xdr_zero) ||
		    (headerp->rm_body.rm_chunks[1] != xdr_zero &&
		     req->rl_nchunks == 0))
			goto badheader;
		if (headerp->rm_body.rm_chunks[1] != xdr_zero) {
			/* count any expected write chunks in read reply */
			/* start at write chunk array count */
			iptr = &headerp->rm_body.rm_chunks[2];
			rdmalen = rpcrdma_count_chunks(rep,
						req->rl_nchunks, 1, &iptr);
			/* check for validity, and no reply chunk after */
			if (rdmalen < 0 || *iptr++ != xdr_zero)
				goto badheader;
			rep->rr_len -=
			    ((unsigned char *)iptr - (unsigned char *)headerp);
			status = rep->rr_len + rdmalen;
			r_xprt->rx_stats.total_rdma_reply += rdmalen;
			/* special case - last chunk may omit padding */
			if (rdmalen &= 3) {
				rdmalen = 4 - rdmalen;
				status += rdmalen;
			}
		} else {
			/* else ordinary inline */
			rdmalen = 0;
			iptr = (__be32 *)((unsigned char *)headerp + 28);
			rep->rr_len -= 28; /*sizeof *headerp;*/
			status = rep->rr_len;
		}
		/* Fix up the rpc results for upper layer */
		rpcrdma_inline_fixup(rqst, (char *)iptr, rep->rr_len, rdmalen);
		break;

	case htonl(RDMA_NOMSG):
		/* never expect read or write chunks, always reply chunks */
		if (headerp->rm_body.rm_chunks[0] != xdr_zero ||
		    headerp->rm_body.rm_chunks[1] != xdr_zero ||
		    headerp->rm_body.rm_chunks[2] != xdr_one ||
		    req->rl_nchunks == 0)
			goto badheader;
		iptr = (__be32 *)((unsigned char *)headerp + 28);
		rdmalen = rpcrdma_count_chunks(rep, req->rl_nchunks, 0, &iptr);
		if (rdmalen < 0)
			goto badheader;
		r_xprt->rx_stats.total_rdma_reply += rdmalen;
		/* Reply chunk buffer already is the reply vector - no fixup. */
		status = rdmalen;
		break;

badheader:
	default:
		dprintk("%s: invalid rpcrdma reply header (type %d):"
				" chunks[012] == %d %d %d"
				" expected chunks <= %d\n",
				__func__, ntohl(headerp->rm_type),
				headerp->rm_body.rm_chunks[0],
				headerp->rm_body.rm_chunks[1],
				headerp->rm_body.rm_chunks[2],
				req->rl_nchunks);
		status = -EIO;
		r_xprt->rx_stats.bad_reply_count++;
		break;
	}

	/* If using mw bind, start the deregister process now. */
	/* (Note: if mr_free(), cannot perform it here, in tasklet context) */
	if (req->rl_nchunks) switch (r_xprt->rx_ia.ri_memreg_strategy) {
	case RPCRDMA_MEMWINDOWS:
		for (i = 0; req->rl_nchunks-- > 1;)
			i += rpcrdma_deregister_external(
				&req->rl_segments[i], r_xprt, NULL);
		/* Optionally wait (not here) for unbinds to complete */
		rep->rr_func = rpcrdma_unbind_func;
		(void) rpcrdma_deregister_external(&req->rl_segments[i],
						   r_xprt, rep);
		break;
	case RPCRDMA_MEMWINDOWS_ASYNC:
		for (i = 0; req->rl_nchunks--;)
			i += rpcrdma_deregister_external(&req->rl_segments[i],
							 r_xprt, NULL);
		break;
	default:
		break;
	}

	dprintk("RPC:       %s: xprt_complete_rqst(0x%p, 0x%p, %d)\n",
			__func__, xprt, rqst, status);
	xprt_complete_rqst(rqst->rq_task, status);
	spin_unlock(&xprt->transport_lock);
=======
			fixup_copy_count += curlen;
			pagelist_len -= curlen;
			if (!pagelist_len)
				break;
			page_base = 0;
		}

		/* Implicit padding for the last segment in a Write
		 * chunk is inserted inline at the front of the tail
		 * iovec. The upper layer ignores the content of
		 * the pad. Simply ensure inline content in the tail
		 * that follows the Write chunk is properly aligned.
		 */
		if (pad)
			srcp -= pad;
	}

	/* The tail iovec is redirected to the remaining data
	 * in the receive buffer, to avoid a memcopy.
	 */
	if (copy_len || pad) {
		rqst->rq_rcv_buf.tail[0].iov_base = srcp;
		rqst->rq_private_buf.tail[0].iov_base = srcp;
	}

	if (fixup_copy_count)
		trace_xprtrdma_fixup(rqst, fixup_copy_count);
	return fixup_copy_count;
}

/* By convention, backchannel calls arrive via rdma_msg type
 * messages, and never populate the chunk lists. This makes
 * the RPC/RDMA header small and fixed in size, so it is
 * straightforward to check the RPC header's direction field.
 */
static bool
rpcrdma_is_bcall(struct rpcrdma_xprt *r_xprt, struct rpcrdma_rep *rep)
#if defined(CONFIG_SUNRPC_BACKCHANNEL)
{
	struct rpc_xprt *xprt = &r_xprt->rx_xprt;
	struct xdr_stream *xdr = &rep->rr_stream;
	__be32 *p;

	if (rep->rr_proc != rdma_msg)
		return false;

	/* Peek at stream contents without advancing. */
	p = xdr_inline_decode(xdr, 0);

	/* Chunk lists */
	if (xdr_item_is_present(p++))
		return false;
	if (xdr_item_is_present(p++))
		return false;
	if (xdr_item_is_present(p++))
		return false;

	/* RPC header */
	if (*p++ != rep->rr_xid)
		return false;
	if (*p != cpu_to_be32(RPC_CALL))
		return false;

	/* No bc service. */
	if (xprt->bc_serv == NULL)
		return false;

	/* Now that we are sure this is a backchannel call,
	 * advance to the RPC header.
	 */
	p = xdr_inline_decode(xdr, 3 * sizeof(*p));
	if (unlikely(!p))
		return true;

	rpcrdma_bc_receive_call(r_xprt, rep);
	return true;
}
#else	/* CONFIG_SUNRPC_BACKCHANNEL */
{
	return false;
}
#endif	/* CONFIG_SUNRPC_BACKCHANNEL */

static int decode_rdma_segment(struct xdr_stream *xdr, u32 *length)
{
	u32 handle;
	u64 offset;
	__be32 *p;

	p = xdr_inline_decode(xdr, 4 * sizeof(*p));
	if (unlikely(!p))
		return -EIO;

	xdr_decode_rdma_segment(p, &handle, length, &offset);
	trace_xprtrdma_decode_seg(handle, *length, offset);
	return 0;
}

static int decode_write_chunk(struct xdr_stream *xdr, u32 *length)
{
	u32 segcount, seglength;
	__be32 *p;

	p = xdr_inline_decode(xdr, sizeof(*p));
	if (unlikely(!p))
		return -EIO;

	*length = 0;
	segcount = be32_to_cpup(p);
	while (segcount--) {
		if (decode_rdma_segment(xdr, &seglength))
			return -EIO;
		*length += seglength;
	}

	return 0;
}

/* In RPC-over-RDMA Version One replies, a Read list is never
 * expected. This decoder is a stub that returns an error if
 * a Read list is present.
 */
static int decode_read_list(struct xdr_stream *xdr)
{
	__be32 *p;

	p = xdr_inline_decode(xdr, sizeof(*p));
	if (unlikely(!p))
		return -EIO;
	if (unlikely(xdr_item_is_present(p)))
		return -EIO;
	return 0;
}

/* Supports only one Write chunk in the Write list
 */
static int decode_write_list(struct xdr_stream *xdr, u32 *length)
{
	u32 chunklen;
	bool first;
	__be32 *p;

	*length = 0;
	first = true;
	do {
		p = xdr_inline_decode(xdr, sizeof(*p));
		if (unlikely(!p))
			return -EIO;
		if (xdr_item_is_absent(p))
			break;
		if (!first)
			return -EIO;

		if (decode_write_chunk(xdr, &chunklen))
			return -EIO;
		*length += chunklen;
		first = false;
	} while (true);
	return 0;
}

static int decode_reply_chunk(struct xdr_stream *xdr, u32 *length)
{
	__be32 *p;

	p = xdr_inline_decode(xdr, sizeof(*p));
	if (unlikely(!p))
		return -EIO;

	*length = 0;
	if (xdr_item_is_present(p))
		if (decode_write_chunk(xdr, length))
			return -EIO;
	return 0;
}

static int
rpcrdma_decode_msg(struct rpcrdma_xprt *r_xprt, struct rpcrdma_rep *rep,
		   struct rpc_rqst *rqst)
{
	struct xdr_stream *xdr = &rep->rr_stream;
	u32 writelist, replychunk, rpclen;
	char *base;

	/* Decode the chunk lists */
	if (decode_read_list(xdr))
		return -EIO;
	if (decode_write_list(xdr, &writelist))
		return -EIO;
	if (decode_reply_chunk(xdr, &replychunk))
		return -EIO;

	/* RDMA_MSG sanity checks */
	if (unlikely(replychunk))
		return -EIO;

	/* Build the RPC reply's Payload stream in rqst->rq_rcv_buf */
	base = (char *)xdr_inline_decode(xdr, 0);
	rpclen = xdr_stream_remaining(xdr);
	r_xprt->rx_stats.fixup_copy_count +=
		rpcrdma_inline_fixup(rqst, base, rpclen, writelist & 3);

	r_xprt->rx_stats.total_rdma_reply += writelist;
	return rpclen + xdr_align_size(writelist);
}

static noinline int
rpcrdma_decode_nomsg(struct rpcrdma_xprt *r_xprt, struct rpcrdma_rep *rep)
{
	struct xdr_stream *xdr = &rep->rr_stream;
	u32 writelist, replychunk;

	/* Decode the chunk lists */
	if (decode_read_list(xdr))
		return -EIO;
	if (decode_write_list(xdr, &writelist))
		return -EIO;
	if (decode_reply_chunk(xdr, &replychunk))
		return -EIO;

	/* RDMA_NOMSG sanity checks */
	if (unlikely(writelist))
		return -EIO;
	if (unlikely(!replychunk))
		return -EIO;

	/* Reply chunk buffer already is the reply vector */
	r_xprt->rx_stats.total_rdma_reply += replychunk;
	return replychunk;
}

static noinline int
rpcrdma_decode_error(struct rpcrdma_xprt *r_xprt, struct rpcrdma_rep *rep,
		     struct rpc_rqst *rqst)
{
	struct xdr_stream *xdr = &rep->rr_stream;
	__be32 *p;

	p = xdr_inline_decode(xdr, sizeof(*p));
	if (unlikely(!p))
		return -EIO;

	switch (*p) {
	case err_vers:
		p = xdr_inline_decode(xdr, 2 * sizeof(*p));
		if (!p)
			break;
		trace_xprtrdma_err_vers(rqst, p, p + 1);
		break;
	case err_chunk:
		trace_xprtrdma_err_chunk(rqst);
		break;
	default:
		trace_xprtrdma_err_unrecognized(rqst, p);
	}

	return -EIO;
}

/**
 * rpcrdma_unpin_rqst - Release rqst without completing it
 * @rep: RPC/RDMA Receive context
 *
 * This is done when a connection is lost so that a Reply
 * can be dropped and its matching Call can be subsequently
 * retransmitted on a new connection.
 */
void rpcrdma_unpin_rqst(struct rpcrdma_rep *rep)
{
	struct rpc_xprt *xprt = &rep->rr_rxprt->rx_xprt;
	struct rpc_rqst *rqst = rep->rr_rqst;
	struct rpcrdma_req *req = rpcr_to_rdmar(rqst);

	req->rl_reply = NULL;
	rep->rr_rqst = NULL;

	spin_lock(&xprt->queue_lock);
	xprt_unpin_rqst(rqst);
	spin_unlock(&xprt->queue_lock);
}

/**
 * rpcrdma_complete_rqst - Pass completed rqst back to RPC
 * @rep: RPC/RDMA Receive context
 *
 * Reconstruct the RPC reply and complete the transaction
 * while @rqst is still pinned to ensure the rep, rqst, and
 * rq_task pointers remain stable.
 */
void rpcrdma_complete_rqst(struct rpcrdma_rep *rep)
{
	struct rpcrdma_xprt *r_xprt = rep->rr_rxprt;
	struct rpc_xprt *xprt = &r_xprt->rx_xprt;
	struct rpc_rqst *rqst = rep->rr_rqst;
	int status;

	switch (rep->rr_proc) {
	case rdma_msg:
		status = rpcrdma_decode_msg(r_xprt, rep, rqst);
		break;
	case rdma_nomsg:
		status = rpcrdma_decode_nomsg(r_xprt, rep);
		break;
	case rdma_error:
		status = rpcrdma_decode_error(r_xprt, rep, rqst);
		break;
	default:
		status = -EIO;
	}
	if (status < 0)
		goto out_badheader;

out:
	spin_lock(&xprt->queue_lock);
	xprt_complete_rqst(rqst->rq_task, status);
	xprt_unpin_rqst(rqst);
	spin_unlock(&xprt->queue_lock);
	return;

out_badheader:
	trace_xprtrdma_reply_hdr_err(rep);
	r_xprt->rx_stats.bad_reply_count++;
	rqst->rq_task->tk_status = status;
	status = 0;
	goto out;
}

static void rpcrdma_reply_done(struct kref *kref)
{
	struct rpcrdma_req *req =
		container_of(kref, struct rpcrdma_req, rl_kref);

	rpcrdma_complete_rqst(req->rl_reply);
}

/**
 * rpcrdma_reply_handler - Process received RPC/RDMA messages
 * @rep: Incoming rpcrdma_rep object to process
 *
 * Errors must result in the RPC task either being awakened, or
 * allowed to timeout, to discover the errors at that time.
 */
void rpcrdma_reply_handler(struct rpcrdma_rep *rep)
{
	struct rpcrdma_xprt *r_xprt = rep->rr_rxprt;
	struct rpc_xprt *xprt = &r_xprt->rx_xprt;
	struct rpcrdma_buffer *buf = &r_xprt->rx_buf;
	struct rpcrdma_req *req;
	struct rpc_rqst *rqst;
	u32 credits;
	__be32 *p;

	/* Any data means we had a useful conversation, so
	 * then we don't need to delay the next reconnect.
	 */
	if (xprt->reestablish_timeout)
		xprt->reestablish_timeout = 0;

	/* Fixed transport header fields */
	xdr_init_decode(&rep->rr_stream, &rep->rr_hdrbuf,
			rep->rr_hdrbuf.head[0].iov_base, NULL);
	p = xdr_inline_decode(&rep->rr_stream, 4 * sizeof(*p));
	if (unlikely(!p))
		goto out_shortreply;
	rep->rr_xid = *p++;
	rep->rr_vers = *p++;
	credits = be32_to_cpu(*p++);
	rep->rr_proc = *p++;

	if (rep->rr_vers != rpcrdma_version)
		goto out_badversion;

	if (rpcrdma_is_bcall(r_xprt, rep))
		return;

	/* Match incoming rpcrdma_rep to an rpcrdma_req to
	 * get context for handling any incoming chunks.
	 */
	spin_lock(&xprt->queue_lock);
	rqst = xprt_lookup_rqst(xprt, rep->rr_xid);
	if (!rqst)
		goto out_norqst;
	xprt_pin_rqst(rqst);
	spin_unlock(&xprt->queue_lock);

	if (credits == 0)
		credits = 1;	/* don't deadlock */
	else if (credits > r_xprt->rx_ep->re_max_requests)
		credits = r_xprt->rx_ep->re_max_requests;
	rpcrdma_post_recvs(r_xprt, credits + (buf->rb_bc_srv_max_requests << 1),
			   false);
	if (buf->rb_credits != credits)
		rpcrdma_update_cwnd(r_xprt, credits);

	req = rpcr_to_rdmar(rqst);
	if (unlikely(req->rl_reply))
		rpcrdma_rep_put(buf, req->rl_reply);
	req->rl_reply = rep;
	rep->rr_rqst = rqst;

	trace_xprtrdma_reply(rqst->rq_task, rep, credits);

	if (rep->rr_wc_flags & IB_WC_WITH_INVALIDATE)
		frwr_reminv(rep, &req->rl_registered);
	if (!list_empty(&req->rl_registered))
		frwr_unmap_async(r_xprt, req);
		/* LocalInv completion will complete the RPC */
	else
		kref_put(&req->rl_kref, rpcrdma_reply_done);
	return;

out_badversion:
	trace_xprtrdma_reply_vers_err(rep);
	goto out;

out_norqst:
	spin_unlock(&xprt->queue_lock);
	trace_xprtrdma_reply_rqst_err(rep);
	goto out;

out_shortreply:
	trace_xprtrdma_reply_short_err(rep);

out:
	rpcrdma_rep_put(buf, rep);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
