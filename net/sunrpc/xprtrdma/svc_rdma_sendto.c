<<<<<<< HEAD
/*
=======
// SPDX-License-Identifier: GPL-2.0 OR BSD-3-Clause
/*
 * Copyright (c) 2016-2018 Oracle. All rights reserved.
 * Copyright (c) 2014 Open Grid Computing, Inc. All rights reserved.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Copyright (c) 2005-2006 Network Appliance, Inc. All rights reserved.
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
 *
 * Author: Tom Tucker <tom@opengridcomputing.com>
 */

<<<<<<< HEAD
#include <linux/sunrpc/debug.h>
#include <linux/sunrpc/rpc_rdma.h>
#include <linux/spinlock.h>
#include <asm/unaligned.h>
#include <rdma/ib_verbs.h>
#include <rdma/rdma_cm.h>
#include <linux/sunrpc/svc_rdma.h>

#define RPCDBG_FACILITY	RPCDBG_SVCXPRT

/* Encode an XDR as an array of IB SGE
 *
 * Assumptions:
 * - head[0] is physically contiguous.
 * - tail[0] is physically contiguous.
 * - pages[] is not physically or virtually contiguous and consists of
 *   PAGE_SIZE elements.
 *
 * Output:
 * SGE[0]              reserved for RCPRDMA header
 * SGE[1]              data from xdr->head[]
 * SGE[2..sge_count-2] data from xdr->pages[]
 * SGE[sge_count-1]    data from xdr->tail.
 *
 * The max SGE we need is the length of the XDR / pagesize + one for
 * head + one for tail + one for RPCRDMA header. Since RPCSVC_MAXPAGES
 * reserves a page for both the request and the reply header, and this
 * array is only concerned with the reply we are assured that we have
 * on extra page for the RPCRMDA header.
 */
static int fast_reg_xdr(struct svcxprt_rdma *xprt,
			struct xdr_buf *xdr,
			struct svc_rdma_req_map *vec)
{
	int sge_no;
	u32 sge_bytes;
	u32 page_bytes;
	u32 page_off;
	int page_no = 0;
	u8 *frva;
	struct svc_rdma_fastreg_mr *frmr;

	frmr = svc_rdma_get_frmr(xprt);
	if (IS_ERR(frmr))
		return -ENOMEM;
	vec->frmr = frmr;

	/* Skip the RPCRDMA header */
	sge_no = 1;

	/* Map the head. */
	frva = (void *)((unsigned long)(xdr->head[0].iov_base) & PAGE_MASK);
	vec->sge[sge_no].iov_base = xdr->head[0].iov_base;
	vec->sge[sge_no].iov_len = xdr->head[0].iov_len;
	vec->count = 2;
	sge_no++;

	/* Map the XDR head */
	frmr->kva = frva;
	frmr->direction = DMA_TO_DEVICE;
	frmr->access_flags = 0;
	frmr->map_len = PAGE_SIZE;
	frmr->page_list_len = 1;
	page_off = (unsigned long)xdr->head[0].iov_base & ~PAGE_MASK;
	frmr->page_list->page_list[page_no] =
		ib_dma_map_page(xprt->sc_cm_id->device,
				virt_to_page(xdr->head[0].iov_base),
				page_off,
				PAGE_SIZE - page_off,
				DMA_TO_DEVICE);
	if (ib_dma_mapping_error(xprt->sc_cm_id->device,
				 frmr->page_list->page_list[page_no]))
		goto fatal_err;
	atomic_inc(&xprt->sc_dma_used);

	/* Map the XDR page list */
	page_off = xdr->page_base;
	page_bytes = xdr->page_len + page_off;
	if (!page_bytes)
		goto encode_tail;

	/* Map the pages */
	vec->sge[sge_no].iov_base = frva + frmr->map_len + page_off;
	vec->sge[sge_no].iov_len = page_bytes;
	sge_no++;
	while (page_bytes) {
		struct page *page;

		page = xdr->pages[page_no++];
		sge_bytes = min_t(u32, page_bytes, (PAGE_SIZE - page_off));
		page_bytes -= sge_bytes;

		frmr->page_list->page_list[page_no] =
			ib_dma_map_page(xprt->sc_cm_id->device,
					page, page_off,
					sge_bytes, DMA_TO_DEVICE);
		if (ib_dma_mapping_error(xprt->sc_cm_id->device,
					 frmr->page_list->page_list[page_no]))
			goto fatal_err;

		atomic_inc(&xprt->sc_dma_used);
		page_off = 0; /* reset for next time through loop */
		frmr->map_len += PAGE_SIZE;
		frmr->page_list_len++;
	}
	vec->count++;

 encode_tail:
	/* Map tail */
	if (0 == xdr->tail[0].iov_len)
		goto done;

	vec->count++;
	vec->sge[sge_no].iov_len = xdr->tail[0].iov_len;

	if (((unsigned long)xdr->tail[0].iov_base & PAGE_MASK) ==
	    ((unsigned long)xdr->head[0].iov_base & PAGE_MASK)) {
		/*
		 * If head and tail use the same page, we don't need
		 * to map it again.
		 */
		vec->sge[sge_no].iov_base = xdr->tail[0].iov_base;
	} else {
		void *va;

		/* Map another page for the tail */
		page_off = (unsigned long)xdr->tail[0].iov_base & ~PAGE_MASK;
		va = (void *)((unsigned long)xdr->tail[0].iov_base & PAGE_MASK);
		vec->sge[sge_no].iov_base = frva + frmr->map_len + page_off;

		frmr->page_list->page_list[page_no] =
		    ib_dma_map_page(xprt->sc_cm_id->device, virt_to_page(va),
				    page_off,
				    PAGE_SIZE,
				    DMA_TO_DEVICE);
		if (ib_dma_mapping_error(xprt->sc_cm_id->device,
					 frmr->page_list->page_list[page_no]))
			goto fatal_err;
		atomic_inc(&xprt->sc_dma_used);
		frmr->map_len += PAGE_SIZE;
		frmr->page_list_len++;
	}

 done:
	if (svc_rdma_fastreg(xprt, frmr))
		goto fatal_err;

	return 0;

 fatal_err:
	printk("svcrdma: Error fast registering memory for xprt %p\n", xprt);
	vec->frmr = NULL;
	svc_rdma_put_frmr(xprt, frmr);
	return -EIO;
}

static int map_xdr(struct svcxprt_rdma *xprt,
		   struct xdr_buf *xdr,
		   struct svc_rdma_req_map *vec)
{
	int sge_no;
	u32 sge_bytes;
	u32 page_bytes;
	u32 page_off;
	int page_no;

	BUG_ON(xdr->len !=
	       (xdr->head[0].iov_len + xdr->page_len + xdr->tail[0].iov_len));

	if (xprt->sc_frmr_pg_list_len)
		return fast_reg_xdr(xprt, xdr, vec);

	/* Skip the first sge, this is for the RPCRDMA header */
	sge_no = 1;

	/* Head SGE */
	vec->sge[sge_no].iov_base = xdr->head[0].iov_base;
	vec->sge[sge_no].iov_len = xdr->head[0].iov_len;
	sge_no++;

	/* pages SGE */
	page_no = 0;
	page_bytes = xdr->page_len;
	page_off = xdr->page_base;
	while (page_bytes) {
		vec->sge[sge_no].iov_base =
			page_address(xdr->pages[page_no]) + page_off;
		sge_bytes = min_t(u32, page_bytes, (PAGE_SIZE - page_off));
		page_bytes -= sge_bytes;
		vec->sge[sge_no].iov_len = sge_bytes;

		sge_no++;
		page_no++;
		page_off = 0; /* reset for next time through loop */
	}

	/* Tail SGE */
	if (xdr->tail[0].iov_len) {
		vec->sge[sge_no].iov_base = xdr->tail[0].iov_base;
		vec->sge[sge_no].iov_len = xdr->tail[0].iov_len;
		sge_no++;
	}

	dprintk("svcrdma: map_xdr: sge_no %d page_no %d "
		"page_base %u page_len %u head_len %zu tail_len %zu\n",
		sge_no, page_no, xdr->page_base, xdr->page_len,
		xdr->head[0].iov_len, xdr->tail[0].iov_len);

	vec->count = sge_no;
	return 0;
}

static dma_addr_t dma_map_xdr(struct svcxprt_rdma *xprt,
			      struct xdr_buf *xdr,
			      u32 xdr_off, size_t len, int dir)
{
	struct page *page;
	dma_addr_t dma_addr;
	if (xdr_off < xdr->head[0].iov_len) {
		/* This offset is in the head */
		xdr_off += (unsigned long)xdr->head[0].iov_base & ~PAGE_MASK;
		page = virt_to_page(xdr->head[0].iov_base);
	} else {
		xdr_off -= xdr->head[0].iov_len;
		if (xdr_off < xdr->page_len) {
			/* This offset is in the page list */
			page = xdr->pages[xdr_off >> PAGE_SHIFT];
			xdr_off &= ~PAGE_MASK;
		} else {
			/* This offset is in the tail */
			xdr_off -= xdr->page_len;
			xdr_off += (unsigned long)
				xdr->tail[0].iov_base & ~PAGE_MASK;
			page = virt_to_page(xdr->tail[0].iov_base);
		}
	}
	dma_addr = ib_dma_map_page(xprt->sc_cm_id->device, page, xdr_off,
				   min_t(size_t, PAGE_SIZE, len), dir);
	return dma_addr;
}

/* Assumptions:
 * - We are using FRMR
 *     - or -
 * - The specified write_len can be represented in sc_max_sge * PAGE_SIZE
 */
static int send_write(struct svcxprt_rdma *xprt, struct svc_rqst *rqstp,
		      u32 rmr, u64 to,
		      u32 xdr_off, int write_len,
		      struct svc_rdma_req_map *vec)
{
	struct ib_send_wr write_wr;
	struct ib_sge *sge;
	int xdr_sge_no;
	int sge_no;
	int sge_bytes;
	int sge_off;
	int bc;
	struct svc_rdma_op_ctxt *ctxt;

	BUG_ON(vec->count > RPCSVC_MAXPAGES);
	dprintk("svcrdma: RDMA_WRITE rmr=%x, to=%llx, xdr_off=%d, "
		"write_len=%d, vec->sge=%p, vec->count=%lu\n",
		rmr, (unsigned long long)to, xdr_off,
		write_len, vec->sge, vec->count);

	ctxt = svc_rdma_get_context(xprt);
	ctxt->direction = DMA_TO_DEVICE;
	sge = ctxt->sge;

	/* Find the SGE associated with xdr_off */
	for (bc = xdr_off, xdr_sge_no = 1; bc && xdr_sge_no < vec->count;
	     xdr_sge_no++) {
		if (vec->sge[xdr_sge_no].iov_len > bc)
			break;
		bc -= vec->sge[xdr_sge_no].iov_len;
	}

	sge_off = bc;
	bc = write_len;
	sge_no = 0;

	/* Copy the remaining SGE */
	while (bc != 0) {
		sge_bytes = min_t(size_t,
			  bc, vec->sge[xdr_sge_no].iov_len-sge_off);
		sge[sge_no].length = sge_bytes;
		if (!vec->frmr) {
			sge[sge_no].addr =
				dma_map_xdr(xprt, &rqstp->rq_res, xdr_off,
					    sge_bytes, DMA_TO_DEVICE);
			xdr_off += sge_bytes;
			if (ib_dma_mapping_error(xprt->sc_cm_id->device,
						 sge[sge_no].addr))
				goto err;
			atomic_inc(&xprt->sc_dma_used);
			sge[sge_no].lkey = xprt->sc_dma_lkey;
		} else {
			sge[sge_no].addr = (unsigned long)
				vec->sge[xdr_sge_no].iov_base + sge_off;
			sge[sge_no].lkey = vec->frmr->mr->lkey;
		}
		ctxt->count++;
		ctxt->frmr = vec->frmr;
		sge_off = 0;
		sge_no++;
		xdr_sge_no++;
		BUG_ON(xdr_sge_no > vec->count);
		bc -= sge_bytes;
	}

	/* Prepare WRITE WR */
	memset(&write_wr, 0, sizeof write_wr);
	ctxt->wr_op = IB_WR_RDMA_WRITE;
	write_wr.wr_id = (unsigned long)ctxt;
	write_wr.sg_list = &sge[0];
	write_wr.num_sge = sge_no;
	write_wr.opcode = IB_WR_RDMA_WRITE;
	write_wr.send_flags = IB_SEND_SIGNALED;
	write_wr.wr.rdma.rkey = rmr;
	write_wr.wr.rdma.remote_addr = to;

	/* Post It */
	atomic_inc(&rdma_stat_write);
	if (svc_rdma_send(xprt, &write_wr))
		goto err;
	return 0;
 err:
	svc_rdma_unmap_dma(ctxt);
	svc_rdma_put_frmr(xprt, vec->frmr);
	svc_rdma_put_context(ctxt, 0);
	/* Fatal error, close transport */
	return -EIO;
}

static int send_write_chunks(struct svcxprt_rdma *xprt,
			     struct rpcrdma_msg *rdma_argp,
			     struct rpcrdma_msg *rdma_resp,
			     struct svc_rqst *rqstp,
			     struct svc_rdma_req_map *vec)
{
	u32 xfer_len = rqstp->rq_res.page_len + rqstp->rq_res.tail[0].iov_len;
	int write_len;
	int max_write;
	u32 xdr_off;
	int chunk_off;
	int chunk_no;
	struct rpcrdma_write_array *arg_ary;
	struct rpcrdma_write_array *res_ary;
	int ret;

	arg_ary = svc_rdma_get_write_array(rdma_argp);
	if (!arg_ary)
		return 0;
	res_ary = (struct rpcrdma_write_array *)
		&rdma_resp->rm_body.rm_chunks[1];

	if (vec->frmr)
		max_write = vec->frmr->map_len;
	else
		max_write = xprt->sc_max_sge * PAGE_SIZE;

	/* Write chunks start at the pagelist */
	for (xdr_off = rqstp->rq_res.head[0].iov_len, chunk_no = 0;
	     xfer_len && chunk_no < arg_ary->wc_nchunks;
	     chunk_no++) {
		struct rpcrdma_segment *arg_ch;
		u64 rs_offset;

		arg_ch = &arg_ary->wc_array[chunk_no].wc_target;
		write_len = min(xfer_len, ntohl(arg_ch->rs_length));

		/* Prepare the response chunk given the length actually
		 * written */
		xdr_decode_hyper((__be32 *)&arg_ch->rs_offset, &rs_offset);
		svc_rdma_xdr_encode_array_chunk(res_ary, chunk_no,
						arg_ch->rs_handle,
						arg_ch->rs_offset,
						write_len);
		chunk_off = 0;
		while (write_len) {
			int this_write;
			this_write = min(write_len, max_write);
			ret = send_write(xprt, rqstp,
					 ntohl(arg_ch->rs_handle),
					 rs_offset + chunk_off,
					 xdr_off,
					 this_write,
					 vec);
			if (ret) {
				dprintk("svcrdma: RDMA_WRITE failed, ret=%d\n",
					ret);
				return -EIO;
			}
			chunk_off += this_write;
			xdr_off += this_write;
			xfer_len -= this_write;
			write_len -= this_write;
		}
	}
	/* Update the req with the number of chunks actually used */
	svc_rdma_xdr_encode_write_list(rdma_resp, chunk_no);

	return rqstp->rq_res.page_len + rqstp->rq_res.tail[0].iov_len;
}

static int send_reply_chunks(struct svcxprt_rdma *xprt,
			     struct rpcrdma_msg *rdma_argp,
			     struct rpcrdma_msg *rdma_resp,
			     struct svc_rqst *rqstp,
			     struct svc_rdma_req_map *vec)
{
	u32 xfer_len = rqstp->rq_res.len;
	int write_len;
	int max_write;
	u32 xdr_off;
	int chunk_no;
	int chunk_off;
	int nchunks;
	struct rpcrdma_segment *ch;
	struct rpcrdma_write_array *arg_ary;
	struct rpcrdma_write_array *res_ary;
	int ret;

	arg_ary = svc_rdma_get_reply_array(rdma_argp);
	if (!arg_ary)
		return 0;
	/* XXX: need to fix when reply lists occur with read-list and or
	 * write-list */
	res_ary = (struct rpcrdma_write_array *)
		&rdma_resp->rm_body.rm_chunks[2];

	if (vec->frmr)
		max_write = vec->frmr->map_len;
	else
		max_write = xprt->sc_max_sge * PAGE_SIZE;

	/* xdr offset starts at RPC message */
	nchunks = ntohl(arg_ary->wc_nchunks);
	for (xdr_off = 0, chunk_no = 0;
	     xfer_len && chunk_no < nchunks;
	     chunk_no++) {
		u64 rs_offset;
		ch = &arg_ary->wc_array[chunk_no].wc_target;
		write_len = min(xfer_len, htonl(ch->rs_length));

		/* Prepare the reply chunk given the length actually
		 * written */
		xdr_decode_hyper((__be32 *)&ch->rs_offset, &rs_offset);
		svc_rdma_xdr_encode_array_chunk(res_ary, chunk_no,
						ch->rs_handle, ch->rs_offset,
						write_len);
		chunk_off = 0;
		while (write_len) {
			int this_write;

			this_write = min(write_len, max_write);
			ret = send_write(xprt, rqstp,
					 ntohl(ch->rs_handle),
					 rs_offset + chunk_off,
					 xdr_off,
					 this_write,
					 vec);
			if (ret) {
				dprintk("svcrdma: RDMA_WRITE failed, ret=%d\n",
					ret);
				return -EIO;
			}
			chunk_off += this_write;
			xdr_off += this_write;
			xfer_len -= this_write;
			write_len -= this_write;
		}
	}
	/* Update the req with the number of chunks actually used */
	svc_rdma_xdr_encode_reply_array(res_ary, chunk_no);

	return rqstp->rq_res.len;
}

/* This function prepares the portion of the RPCRDMA message to be
 * sent in the RDMA_SEND. This function is called after data sent via
 * RDMA has already been transmitted. There are three cases:
 * - The RPCRDMA header, RPC header, and payload are all sent in a
 *   single RDMA_SEND. This is the "inline" case.
 * - The RPCRDMA header and some portion of the RPC header and data
 *   are sent via this RDMA_SEND and another portion of the data is
 *   sent via RDMA.
 * - The RPCRDMA header [NOMSG] is sent in this RDMA_SEND and the RPC
 *   header and data are all transmitted via RDMA.
 * In all three cases, this function prepares the RPCRDMA header in
 * sge[0], the 'type' parameter indicates the type to place in the
 * RPCRDMA header, and the 'byte_count' field indicates how much of
 * the XDR to include in this RDMA_SEND. NB: The offset of the payload
 * to send is zero in the XDR.
 */
static int send_reply(struct svcxprt_rdma *rdma,
		      struct svc_rqst *rqstp,
		      struct page *page,
		      struct rpcrdma_msg *rdma_resp,
		      struct svc_rdma_op_ctxt *ctxt,
		      struct svc_rdma_req_map *vec,
		      int byte_count)
{
	struct ib_send_wr send_wr;
	struct ib_send_wr inv_wr;
	u32 xdr_off;
	int sge_no;
	int sge_bytes;
	int page_no;
	int ret;

	/* Post a recv buffer to handle another request. */
	ret = svc_rdma_post_recv(rdma);
	if (ret) {
		printk(KERN_INFO
		       "svcrdma: could not post a receive buffer, err=%d."
		       "Closing transport %p.\n", ret, rdma);
		set_bit(XPT_CLOSE, &rdma->sc_xprt.xpt_flags);
		svc_rdma_put_frmr(rdma, vec->frmr);
		svc_rdma_put_context(ctxt, 0);
		return -ENOTCONN;
	}

	/* Prepare the context */
	ctxt->pages[0] = page;
	ctxt->count = 1;
	ctxt->frmr = vec->frmr;
	if (vec->frmr)
		set_bit(RDMACTXT_F_FAST_UNREG, &ctxt->flags);
	else
		clear_bit(RDMACTXT_F_FAST_UNREG, &ctxt->flags);

	/* Prepare the SGE for the RPCRDMA Header */
	ctxt->sge[0].lkey = rdma->sc_dma_lkey;
	ctxt->sge[0].length = svc_rdma_xdr_get_reply_hdr_len(rdma_resp);
	ctxt->sge[0].addr =
	    ib_dma_map_page(rdma->sc_cm_id->device, page, 0,
			    ctxt->sge[0].length, DMA_TO_DEVICE);
	if (ib_dma_mapping_error(rdma->sc_cm_id->device, ctxt->sge[0].addr))
		goto err;
	atomic_inc(&rdma->sc_dma_used);

	ctxt->direction = DMA_TO_DEVICE;

	/* Map the payload indicated by 'byte_count' */
	xdr_off = 0;
	for (sge_no = 1; byte_count && sge_no < vec->count; sge_no++) {
		sge_bytes = min_t(size_t, vec->sge[sge_no].iov_len, byte_count);
		byte_count -= sge_bytes;
		if (!vec->frmr) {
			ctxt->sge[sge_no].addr =
				dma_map_xdr(rdma, &rqstp->rq_res, xdr_off,
					    sge_bytes, DMA_TO_DEVICE);
			xdr_off += sge_bytes;
			if (ib_dma_mapping_error(rdma->sc_cm_id->device,
						 ctxt->sge[sge_no].addr))
				goto err;
			atomic_inc(&rdma->sc_dma_used);
			ctxt->sge[sge_no].lkey = rdma->sc_dma_lkey;
		} else {
			ctxt->sge[sge_no].addr = (unsigned long)
				vec->sge[sge_no].iov_base;
			ctxt->sge[sge_no].lkey = vec->frmr->mr->lkey;
		}
		ctxt->sge[sge_no].length = sge_bytes;
	}
	BUG_ON(byte_count != 0);

	/* Save all respages in the ctxt and remove them from the
	 * respages array. They are our pages until the I/O
	 * completes.
	 */
	for (page_no = 0; page_no < rqstp->rq_resused; page_no++) {
		ctxt->pages[page_no+1] = rqstp->rq_respages[page_no];
		ctxt->count++;
		rqstp->rq_respages[page_no] = NULL;
		/*
		 * If there are more pages than SGE, terminate SGE
		 * list so that svc_rdma_unmap_dma doesn't attempt to
		 * unmap garbage.
		 */
		if (page_no+1 >= sge_no)
			ctxt->sge[page_no+1].length = 0;
	}

	/* The loop above bumps sc_dma_used for each sge. The
	 * xdr_buf.tail gets a separate sge, but resides in the
	 * same page as xdr_buf.head. Don't count it twice.
	 */
	if (sge_no > ctxt->count)
		atomic_dec(&rdma->sc_dma_used);

	BUG_ON(sge_no > rdma->sc_max_sge);
	memset(&send_wr, 0, sizeof send_wr);
	ctxt->wr_op = IB_WR_SEND;
	send_wr.wr_id = (unsigned long)ctxt;
	send_wr.sg_list = ctxt->sge;
	send_wr.num_sge = sge_no;
	send_wr.opcode = IB_WR_SEND;
	send_wr.send_flags =  IB_SEND_SIGNALED;
	if (vec->frmr) {
		/* Prepare INVALIDATE WR */
		memset(&inv_wr, 0, sizeof inv_wr);
		inv_wr.opcode = IB_WR_LOCAL_INV;
		inv_wr.send_flags = IB_SEND_SIGNALED;
		inv_wr.ex.invalidate_rkey =
			vec->frmr->mr->lkey;
		send_wr.next = &inv_wr;
	}

	ret = svc_rdma_send(rdma, &send_wr);
	if (ret)
		goto err;

	return 0;

 err:
	svc_rdma_unmap_dma(ctxt);
	svc_rdma_put_frmr(rdma, vec->frmr);
	svc_rdma_put_context(ctxt, 1);
	return -EIO;
}

void svc_rdma_prep_reply_hdr(struct svc_rqst *rqstp)
{
}

/*
 * Return the start of an xdr buffer.
 */
static void *xdr_start(struct xdr_buf *xdr)
{
	return xdr->head[0].iov_base -
		(xdr->len -
		 xdr->page_len -
		 xdr->tail[0].iov_len -
		 xdr->head[0].iov_len);
}

=======
/* Operation
 *
 * The main entry point is svc_rdma_sendto. This is called by the
 * RPC server when an RPC Reply is ready to be transmitted to a client.
 *
 * The passed-in svc_rqst contains a struct xdr_buf which holds an
 * XDR-encoded RPC Reply message. sendto must construct the RPC-over-RDMA
 * transport header, post all Write WRs needed for this Reply, then post
 * a Send WR conveying the transport header and the RPC message itself to
 * the client.
 *
 * svc_rdma_sendto must fully transmit the Reply before returning, as
 * the svc_rqst will be recycled as soon as sendto returns. Remaining
 * resources referred to by the svc_rqst are also recycled at that time.
 * Therefore any resources that must remain longer must be detached
 * from the svc_rqst and released later.
 *
 * Page Management
 *
 * The I/O that performs Reply transmission is asynchronous, and may
 * complete well after sendto returns. Thus pages under I/O must be
 * removed from the svc_rqst before sendto returns.
 *
 * The logic here depends on Send Queue and completion ordering. Since
 * the Send WR is always posted last, it will always complete last. Thus
 * when it completes, it is guaranteed that all previous Write WRs have
 * also completed.
 *
 * Write WRs are constructed and posted. Each Write segment gets its own
 * svc_rdma_rw_ctxt, allowing the Write completion handler to find and
 * DMA-unmap the pages under I/O for that Write segment. The Write
 * completion handler does not release any pages.
 *
 * When the Send WR is constructed, it also gets its own svc_rdma_send_ctxt.
 * The ownership of all of the Reply's pages are transferred into that
 * ctxt, the Send WR is posted, and sendto returns.
 *
 * The svc_rdma_send_ctxt is presented when the Send WR completes. The
 * Send completion handler finally releases the Reply's pages.
 *
 * This mechanism also assumes that completions on the transport's Send
 * Completion Queue do not run in parallel. Otherwise a Write completion
 * and Send completion running at the same time could release pages that
 * are still DMA-mapped.
 *
 * Error Handling
 *
 * - If the Send WR is posted successfully, it will either complete
 *   successfully, or get flushed. Either way, the Send completion
 *   handler releases the Reply's pages.
 * - If the Send WR cannot be not posted, the forward path releases
 *   the Reply's pages.
 *
 * This handles the case, without the use of page reference counting,
 * where two different Write segments send portions of the same page.
 */

#include <linux/spinlock.h>
#include <asm/unaligned.h>

#include <rdma/ib_verbs.h>
#include <rdma/rdma_cm.h>

#include <linux/sunrpc/debug.h>
#include <linux/sunrpc/svc_rdma.h>

#include "xprt_rdma.h"
#include <trace/events/rpcrdma.h>

static void svc_rdma_wc_send(struct ib_cq *cq, struct ib_wc *wc);

static struct svc_rdma_send_ctxt *
svc_rdma_send_ctxt_alloc(struct svcxprt_rdma *rdma)
{
	int node = ibdev_to_node(rdma->sc_cm_id->device);
	struct svc_rdma_send_ctxt *ctxt;
	dma_addr_t addr;
	void *buffer;
	int i;

	ctxt = kzalloc_node(struct_size(ctxt, sc_sges, rdma->sc_max_send_sges),
			    GFP_KERNEL, node);
	if (!ctxt)
		goto fail0;
	buffer = kmalloc_node(rdma->sc_max_req_size, GFP_KERNEL, node);
	if (!buffer)
		goto fail1;
	addr = ib_dma_map_single(rdma->sc_pd->device, buffer,
				 rdma->sc_max_req_size, DMA_TO_DEVICE);
	if (ib_dma_mapping_error(rdma->sc_pd->device, addr))
		goto fail2;

	svc_rdma_send_cid_init(rdma, &ctxt->sc_cid);

	ctxt->sc_rdma = rdma;
	ctxt->sc_send_wr.next = NULL;
	ctxt->sc_send_wr.wr_cqe = &ctxt->sc_cqe;
	ctxt->sc_send_wr.sg_list = ctxt->sc_sges;
	ctxt->sc_send_wr.send_flags = IB_SEND_SIGNALED;
	ctxt->sc_cqe.done = svc_rdma_wc_send;
	ctxt->sc_xprt_buf = buffer;
	xdr_buf_init(&ctxt->sc_hdrbuf, ctxt->sc_xprt_buf,
		     rdma->sc_max_req_size);
	ctxt->sc_sges[0].addr = addr;

	for (i = 0; i < rdma->sc_max_send_sges; i++)
		ctxt->sc_sges[i].lkey = rdma->sc_pd->local_dma_lkey;
	return ctxt;

fail2:
	kfree(buffer);
fail1:
	kfree(ctxt);
fail0:
	return NULL;
}

/**
 * svc_rdma_send_ctxts_destroy - Release all send_ctxt's for an xprt
 * @rdma: svcxprt_rdma being torn down
 *
 */
void svc_rdma_send_ctxts_destroy(struct svcxprt_rdma *rdma)
{
	struct svc_rdma_send_ctxt *ctxt;
	struct llist_node *node;

	while ((node = llist_del_first(&rdma->sc_send_ctxts)) != NULL) {
		ctxt = llist_entry(node, struct svc_rdma_send_ctxt, sc_node);
		ib_dma_unmap_single(rdma->sc_pd->device,
				    ctxt->sc_sges[0].addr,
				    rdma->sc_max_req_size,
				    DMA_TO_DEVICE);
		kfree(ctxt->sc_xprt_buf);
		kfree(ctxt);
	}
}

/**
 * svc_rdma_send_ctxt_get - Get a free send_ctxt
 * @rdma: controlling svcxprt_rdma
 *
 * Returns a ready-to-use send_ctxt, or NULL if none are
 * available and a fresh one cannot be allocated.
 */
struct svc_rdma_send_ctxt *svc_rdma_send_ctxt_get(struct svcxprt_rdma *rdma)
{
	struct svc_rdma_send_ctxt *ctxt;
	struct llist_node *node;

	spin_lock(&rdma->sc_send_lock);
	node = llist_del_first(&rdma->sc_send_ctxts);
	spin_unlock(&rdma->sc_send_lock);
	if (!node)
		goto out_empty;

	ctxt = llist_entry(node, struct svc_rdma_send_ctxt, sc_node);

out:
	rpcrdma_set_xdrlen(&ctxt->sc_hdrbuf, 0);
	xdr_init_encode(&ctxt->sc_stream, &ctxt->sc_hdrbuf,
			ctxt->sc_xprt_buf, NULL);

	svc_rdma_cc_init(rdma, &ctxt->sc_reply_info.wi_cc);
	ctxt->sc_send_wr.num_sge = 0;
	ctxt->sc_cur_sge_no = 0;
	ctxt->sc_page_count = 0;
	ctxt->sc_wr_chain = &ctxt->sc_send_wr;
	ctxt->sc_sqecount = 1;

	return ctxt;

out_empty:
	ctxt = svc_rdma_send_ctxt_alloc(rdma);
	if (!ctxt)
		return NULL;
	goto out;
}

static void svc_rdma_send_ctxt_release(struct svcxprt_rdma *rdma,
				       struct svc_rdma_send_ctxt *ctxt)
{
	struct ib_device *device = rdma->sc_cm_id->device;
	unsigned int i;

	svc_rdma_reply_chunk_release(rdma, ctxt);

	if (ctxt->sc_page_count)
		release_pages(ctxt->sc_pages, ctxt->sc_page_count);

	/* The first SGE contains the transport header, which
	 * remains mapped until @ctxt is destroyed.
	 */
	for (i = 1; i < ctxt->sc_send_wr.num_sge; i++) {
		trace_svcrdma_dma_unmap_page(&ctxt->sc_cid,
					     ctxt->sc_sges[i].addr,
					     ctxt->sc_sges[i].length);
		ib_dma_unmap_page(device,
				  ctxt->sc_sges[i].addr,
				  ctxt->sc_sges[i].length,
				  DMA_TO_DEVICE);
	}

	llist_add(&ctxt->sc_node, &rdma->sc_send_ctxts);
}

static void svc_rdma_send_ctxt_put_async(struct work_struct *work)
{
	struct svc_rdma_send_ctxt *ctxt;

	ctxt = container_of(work, struct svc_rdma_send_ctxt, sc_work);
	svc_rdma_send_ctxt_release(ctxt->sc_rdma, ctxt);
}

/**
 * svc_rdma_send_ctxt_put - Return send_ctxt to free list
 * @rdma: controlling svcxprt_rdma
 * @ctxt: object to return to the free list
 *
 * Pages left in sc_pages are DMA unmapped and released.
 */
void svc_rdma_send_ctxt_put(struct svcxprt_rdma *rdma,
			    struct svc_rdma_send_ctxt *ctxt)
{
	INIT_WORK(&ctxt->sc_work, svc_rdma_send_ctxt_put_async);
	queue_work(svcrdma_wq, &ctxt->sc_work);
}

/**
 * svc_rdma_wake_send_waiters - manage Send Queue accounting
 * @rdma: controlling transport
 * @avail: Number of additional SQEs that are now available
 *
 */
void svc_rdma_wake_send_waiters(struct svcxprt_rdma *rdma, int avail)
{
	atomic_add(avail, &rdma->sc_sq_avail);
	smp_mb__after_atomic();
	if (unlikely(waitqueue_active(&rdma->sc_send_wait)))
		wake_up(&rdma->sc_send_wait);
}

/**
 * svc_rdma_wc_send - Invoked by RDMA provider for each polled Send WC
 * @cq: Completion Queue context
 * @wc: Work Completion object
 *
 * NB: The svc_xprt/svcxprt_rdma is pinned whenever it's possible that
 * the Send completion handler could be running.
 */
static void svc_rdma_wc_send(struct ib_cq *cq, struct ib_wc *wc)
{
	struct svcxprt_rdma *rdma = cq->cq_context;
	struct ib_cqe *cqe = wc->wr_cqe;
	struct svc_rdma_send_ctxt *ctxt =
		container_of(cqe, struct svc_rdma_send_ctxt, sc_cqe);

	svc_rdma_wake_send_waiters(rdma, ctxt->sc_sqecount);

	if (unlikely(wc->status != IB_WC_SUCCESS))
		goto flushed;

	trace_svcrdma_wc_send(&ctxt->sc_cid);
	svc_rdma_send_ctxt_put(rdma, ctxt);
	return;

flushed:
	if (wc->status != IB_WC_WR_FLUSH_ERR)
		trace_svcrdma_wc_send_err(wc, &ctxt->sc_cid);
	else
		trace_svcrdma_wc_send_flush(wc, &ctxt->sc_cid);
	svc_rdma_send_ctxt_put(rdma, ctxt);
	svc_xprt_deferred_close(&rdma->sc_xprt);
}

/**
 * svc_rdma_post_send - Post a WR chain to the Send Queue
 * @rdma: transport context
 * @ctxt: WR chain to post
 *
 * Copy fields in @ctxt to stack variables in order to guarantee
 * that these values remain available after the ib_post_send() call.
 * In some error flow cases, svc_rdma_wc_send() releases @ctxt.
 *
 * Note there is potential for starvation when the Send Queue is
 * full because there is no order to when waiting threads are
 * awoken. The transport is typically provisioned with a deep
 * enough Send Queue that SQ exhaustion should be a rare event.
 *
 * Return values:
 *   %0: @ctxt's WR chain was posted successfully
 *   %-ENOTCONN: The connection was lost
 */
int svc_rdma_post_send(struct svcxprt_rdma *rdma,
		       struct svc_rdma_send_ctxt *ctxt)
{
	struct ib_send_wr *first_wr = ctxt->sc_wr_chain;
	struct ib_send_wr *send_wr = &ctxt->sc_send_wr;
	const struct ib_send_wr *bad_wr = first_wr;
	struct rpc_rdma_cid cid = ctxt->sc_cid;
	int ret, sqecount = ctxt->sc_sqecount;

	might_sleep();

	/* Sync the transport header buffer */
	ib_dma_sync_single_for_device(rdma->sc_pd->device,
				      send_wr->sg_list[0].addr,
				      send_wr->sg_list[0].length,
				      DMA_TO_DEVICE);

	/* If the SQ is full, wait until an SQ entry is available */
	while (!test_bit(XPT_CLOSE, &rdma->sc_xprt.xpt_flags)) {
		if (atomic_sub_return(sqecount, &rdma->sc_sq_avail) < 0) {
			svc_rdma_wake_send_waiters(rdma, sqecount);

			/* When the transport is torn down, assume
			 * ib_drain_sq() will trigger enough Send
			 * completions to wake us. The XPT_CLOSE test
			 * above should then cause the while loop to
			 * exit.
			 */
			percpu_counter_inc(&svcrdma_stat_sq_starve);
			trace_svcrdma_sq_full(rdma, &cid);
			wait_event(rdma->sc_send_wait,
				   atomic_read(&rdma->sc_sq_avail) > 0);
			trace_svcrdma_sq_retry(rdma, &cid);
			continue;
		}

		trace_svcrdma_post_send(ctxt);
		ret = ib_post_send(rdma->sc_qp, first_wr, &bad_wr);
		if (ret) {
			trace_svcrdma_sq_post_err(rdma, &cid, ret);
			svc_xprt_deferred_close(&rdma->sc_xprt);

			/* If even one WR was posted, there will be a
			 * Send completion that bumps sc_sq_avail.
			 */
			if (bad_wr == first_wr) {
				svc_rdma_wake_send_waiters(rdma, sqecount);
				break;
			}
		}
		return 0;
	}
	return -ENOTCONN;
}

/**
 * svc_rdma_encode_read_list - Encode RPC Reply's Read chunk list
 * @sctxt: Send context for the RPC Reply
 *
 * Return values:
 *   On success, returns length in bytes of the Reply XDR buffer
 *   that was consumed by the Reply Read list
 *   %-EMSGSIZE on XDR buffer overflow
 */
static ssize_t svc_rdma_encode_read_list(struct svc_rdma_send_ctxt *sctxt)
{
	/* RPC-over-RDMA version 1 replies never have a Read list. */
	return xdr_stream_encode_item_absent(&sctxt->sc_stream);
}

/**
 * svc_rdma_encode_write_segment - Encode one Write segment
 * @sctxt: Send context for the RPC Reply
 * @chunk: Write chunk to push
 * @remaining: remaining bytes of the payload left in the Write chunk
 * @segno: which segment in the chunk
 *
 * Return values:
 *   On success, returns length in bytes of the Reply XDR buffer
 *   that was consumed by the Write segment, and updates @remaining
 *   %-EMSGSIZE on XDR buffer overflow
 */
static ssize_t svc_rdma_encode_write_segment(struct svc_rdma_send_ctxt *sctxt,
					     const struct svc_rdma_chunk *chunk,
					     u32 *remaining, unsigned int segno)
{
	const struct svc_rdma_segment *segment = &chunk->ch_segments[segno];
	const size_t len = rpcrdma_segment_maxsz * sizeof(__be32);
	u32 length;
	__be32 *p;

	p = xdr_reserve_space(&sctxt->sc_stream, len);
	if (!p)
		return -EMSGSIZE;

	length = min_t(u32, *remaining, segment->rs_length);
	*remaining -= length;
	xdr_encode_rdma_segment(p, segment->rs_handle, length,
				segment->rs_offset);
	trace_svcrdma_encode_wseg(sctxt, segno, segment->rs_handle, length,
				  segment->rs_offset);
	return len;
}

/**
 * svc_rdma_encode_write_chunk - Encode one Write chunk
 * @sctxt: Send context for the RPC Reply
 * @chunk: Write chunk to push
 *
 * Copy a Write chunk from the Call transport header to the
 * Reply transport header. Update each segment's length field
 * to reflect the number of bytes written in that segment.
 *
 * Return values:
 *   On success, returns length in bytes of the Reply XDR buffer
 *   that was consumed by the Write chunk
 *   %-EMSGSIZE on XDR buffer overflow
 */
static ssize_t svc_rdma_encode_write_chunk(struct svc_rdma_send_ctxt *sctxt,
					   const struct svc_rdma_chunk *chunk)
{
	u32 remaining = chunk->ch_payload_length;
	unsigned int segno;
	ssize_t len, ret;

	len = 0;
	ret = xdr_stream_encode_item_present(&sctxt->sc_stream);
	if (ret < 0)
		return ret;
	len += ret;

	ret = xdr_stream_encode_u32(&sctxt->sc_stream, chunk->ch_segcount);
	if (ret < 0)
		return ret;
	len += ret;

	for (segno = 0; segno < chunk->ch_segcount; segno++) {
		ret = svc_rdma_encode_write_segment(sctxt, chunk, &remaining, segno);
		if (ret < 0)
			return ret;
		len += ret;
	}

	return len;
}

/**
 * svc_rdma_encode_write_list - Encode RPC Reply's Write chunk list
 * @rctxt: Reply context with information about the RPC Call
 * @sctxt: Send context for the RPC Reply
 *
 * Return values:
 *   On success, returns length in bytes of the Reply XDR buffer
 *   that was consumed by the Reply's Write list
 *   %-EMSGSIZE on XDR buffer overflow
 */
static ssize_t svc_rdma_encode_write_list(struct svc_rdma_recv_ctxt *rctxt,
					  struct svc_rdma_send_ctxt *sctxt)
{
	struct svc_rdma_chunk *chunk;
	ssize_t len, ret;

	len = 0;
	pcl_for_each_chunk(chunk, &rctxt->rc_write_pcl) {
		ret = svc_rdma_encode_write_chunk(sctxt, chunk);
		if (ret < 0)
			return ret;
		len += ret;
	}

	/* Terminate the Write list */
	ret = xdr_stream_encode_item_absent(&sctxt->sc_stream);
	if (ret < 0)
		return ret;

	return len + ret;
}

/**
 * svc_rdma_encode_reply_chunk - Encode RPC Reply's Reply chunk
 * @rctxt: Reply context with information about the RPC Call
 * @sctxt: Send context for the RPC Reply
 * @length: size in bytes of the payload in the Reply chunk
 *
 * Return values:
 *   On success, returns length in bytes of the Reply XDR buffer
 *   that was consumed by the Reply's Reply chunk
 *   %-EMSGSIZE on XDR buffer overflow
 *   %-E2BIG if the RPC message is larger than the Reply chunk
 */
static ssize_t
svc_rdma_encode_reply_chunk(struct svc_rdma_recv_ctxt *rctxt,
			    struct svc_rdma_send_ctxt *sctxt,
			    unsigned int length)
{
	struct svc_rdma_chunk *chunk;

	if (pcl_is_empty(&rctxt->rc_reply_pcl))
		return xdr_stream_encode_item_absent(&sctxt->sc_stream);

	chunk = pcl_first_chunk(&rctxt->rc_reply_pcl);
	if (length > chunk->ch_length)
		return -E2BIG;

	chunk->ch_payload_length = length;
	return svc_rdma_encode_write_chunk(sctxt, chunk);
}

struct svc_rdma_map_data {
	struct svcxprt_rdma		*md_rdma;
	struct svc_rdma_send_ctxt	*md_ctxt;
};

/**
 * svc_rdma_page_dma_map - DMA map one page
 * @data: pointer to arguments
 * @page: struct page to DMA map
 * @offset: offset into the page
 * @len: number of bytes to map
 *
 * Returns:
 *   %0 if DMA mapping was successful
 *   %-EIO if the page cannot be DMA mapped
 */
static int svc_rdma_page_dma_map(void *data, struct page *page,
				 unsigned long offset, unsigned int len)
{
	struct svc_rdma_map_data *args = data;
	struct svcxprt_rdma *rdma = args->md_rdma;
	struct svc_rdma_send_ctxt *ctxt = args->md_ctxt;
	struct ib_device *dev = rdma->sc_cm_id->device;
	dma_addr_t dma_addr;

	++ctxt->sc_cur_sge_no;

	dma_addr = ib_dma_map_page(dev, page, offset, len, DMA_TO_DEVICE);
	if (ib_dma_mapping_error(dev, dma_addr))
		goto out_maperr;

	trace_svcrdma_dma_map_page(&ctxt->sc_cid, dma_addr, len);
	ctxt->sc_sges[ctxt->sc_cur_sge_no].addr = dma_addr;
	ctxt->sc_sges[ctxt->sc_cur_sge_no].length = len;
	ctxt->sc_send_wr.num_sge++;
	return 0;

out_maperr:
	trace_svcrdma_dma_map_err(&ctxt->sc_cid, dma_addr, len);
	return -EIO;
}

/**
 * svc_rdma_iov_dma_map - DMA map an iovec
 * @data: pointer to arguments
 * @iov: kvec to DMA map
 *
 * ib_dma_map_page() is used here because svc_rdma_dma_unmap()
 * handles DMA-unmap and it uses ib_dma_unmap_page() exclusively.
 *
 * Returns:
 *   %0 if DMA mapping was successful
 *   %-EIO if the iovec cannot be DMA mapped
 */
static int svc_rdma_iov_dma_map(void *data, const struct kvec *iov)
{
	if (!iov->iov_len)
		return 0;
	return svc_rdma_page_dma_map(data, virt_to_page(iov->iov_base),
				     offset_in_page(iov->iov_base),
				     iov->iov_len);
}

/**
 * svc_rdma_xb_dma_map - DMA map all segments of an xdr_buf
 * @xdr: xdr_buf containing portion of an RPC message to transmit
 * @data: pointer to arguments
 *
 * Returns:
 *   %0 if DMA mapping was successful
 *   %-EIO if DMA mapping failed
 *
 * On failure, any DMA mappings that have been already done must be
 * unmapped by the caller.
 */
static int svc_rdma_xb_dma_map(const struct xdr_buf *xdr, void *data)
{
	unsigned int len, remaining;
	unsigned long pageoff;
	struct page **ppages;
	int ret;

	ret = svc_rdma_iov_dma_map(data, &xdr->head[0]);
	if (ret < 0)
		return ret;

	ppages = xdr->pages + (xdr->page_base >> PAGE_SHIFT);
	pageoff = offset_in_page(xdr->page_base);
	remaining = xdr->page_len;
	while (remaining) {
		len = min_t(u32, PAGE_SIZE - pageoff, remaining);

		ret = svc_rdma_page_dma_map(data, *ppages++, pageoff, len);
		if (ret < 0)
			return ret;

		remaining -= len;
		pageoff = 0;
	}

	ret = svc_rdma_iov_dma_map(data, &xdr->tail[0]);
	if (ret < 0)
		return ret;

	return xdr->len;
}

struct svc_rdma_pullup_data {
	u8		*pd_dest;
	unsigned int	pd_length;
	unsigned int	pd_num_sges;
};

/**
 * svc_rdma_xb_count_sges - Count how many SGEs will be needed
 * @xdr: xdr_buf containing portion of an RPC message to transmit
 * @data: pointer to arguments
 *
 * Returns:
 *   Number of SGEs needed to Send the contents of @xdr inline
 */
static int svc_rdma_xb_count_sges(const struct xdr_buf *xdr,
				  void *data)
{
	struct svc_rdma_pullup_data *args = data;
	unsigned int remaining;
	unsigned long offset;

	if (xdr->head[0].iov_len)
		++args->pd_num_sges;

	offset = offset_in_page(xdr->page_base);
	remaining = xdr->page_len;
	while (remaining) {
		++args->pd_num_sges;
		remaining -= min_t(u32, PAGE_SIZE - offset, remaining);
		offset = 0;
	}

	if (xdr->tail[0].iov_len)
		++args->pd_num_sges;

	args->pd_length += xdr->len;
	return 0;
}

/**
 * svc_rdma_pull_up_needed - Determine whether to use pull-up
 * @rdma: controlling transport
 * @sctxt: send_ctxt for the Send WR
 * @write_pcl: Write chunk list provided by client
 * @xdr: xdr_buf containing RPC message to transmit
 *
 * Returns:
 *   %true if pull-up must be used
 *   %false otherwise
 */
static bool svc_rdma_pull_up_needed(const struct svcxprt_rdma *rdma,
				    const struct svc_rdma_send_ctxt *sctxt,
				    const struct svc_rdma_pcl *write_pcl,
				    const struct xdr_buf *xdr)
{
	/* Resources needed for the transport header */
	struct svc_rdma_pullup_data args = {
		.pd_length	= sctxt->sc_hdrbuf.len,
		.pd_num_sges	= 1,
	};
	int ret;

	ret = pcl_process_nonpayloads(write_pcl, xdr,
				      svc_rdma_xb_count_sges, &args);
	if (ret < 0)
		return false;

	if (args.pd_length < RPCRDMA_PULLUP_THRESH)
		return true;
	return args.pd_num_sges >= rdma->sc_max_send_sges;
}

/**
 * svc_rdma_xb_linearize - Copy region of xdr_buf to flat buffer
 * @xdr: xdr_buf containing portion of an RPC message to copy
 * @data: pointer to arguments
 *
 * Returns:
 *   Always zero.
 */
static int svc_rdma_xb_linearize(const struct xdr_buf *xdr,
				 void *data)
{
	struct svc_rdma_pullup_data *args = data;
	unsigned int len, remaining;
	unsigned long pageoff;
	struct page **ppages;

	if (xdr->head[0].iov_len) {
		memcpy(args->pd_dest, xdr->head[0].iov_base, xdr->head[0].iov_len);
		args->pd_dest += xdr->head[0].iov_len;
	}

	ppages = xdr->pages + (xdr->page_base >> PAGE_SHIFT);
	pageoff = offset_in_page(xdr->page_base);
	remaining = xdr->page_len;
	while (remaining) {
		len = min_t(u32, PAGE_SIZE - pageoff, remaining);
		memcpy(args->pd_dest, page_address(*ppages) + pageoff, len);
		remaining -= len;
		args->pd_dest += len;
		pageoff = 0;
		ppages++;
	}

	if (xdr->tail[0].iov_len) {
		memcpy(args->pd_dest, xdr->tail[0].iov_base, xdr->tail[0].iov_len);
		args->pd_dest += xdr->tail[0].iov_len;
	}

	args->pd_length += xdr->len;
	return 0;
}

/**
 * svc_rdma_pull_up_reply_msg - Copy Reply into a single buffer
 * @rdma: controlling transport
 * @sctxt: send_ctxt for the Send WR; xprt hdr is already prepared
 * @write_pcl: Write chunk list provided by client
 * @xdr: prepared xdr_buf containing RPC message
 *
 * The device is not capable of sending the reply directly.
 * Assemble the elements of @xdr into the transport header buffer.
 *
 * Assumptions:
 *  pull_up_needed has determined that @xdr will fit in the buffer.
 *
 * Returns:
 *   %0 if pull-up was successful
 *   %-EMSGSIZE if a buffer manipulation problem occurred
 */
static int svc_rdma_pull_up_reply_msg(const struct svcxprt_rdma *rdma,
				      struct svc_rdma_send_ctxt *sctxt,
				      const struct svc_rdma_pcl *write_pcl,
				      const struct xdr_buf *xdr)
{
	struct svc_rdma_pullup_data args = {
		.pd_dest	= sctxt->sc_xprt_buf + sctxt->sc_hdrbuf.len,
	};
	int ret;

	ret = pcl_process_nonpayloads(write_pcl, xdr,
				      svc_rdma_xb_linearize, &args);
	if (ret < 0)
		return ret;

	sctxt->sc_sges[0].length = sctxt->sc_hdrbuf.len + args.pd_length;
	trace_svcrdma_send_pullup(sctxt, args.pd_length);
	return 0;
}

/* svc_rdma_map_reply_msg - DMA map the buffer holding RPC message
 * @rdma: controlling transport
 * @sctxt: send_ctxt for the Send WR
 * @write_pcl: Write chunk list provided by client
 * @reply_pcl: Reply chunk provided by client
 * @xdr: prepared xdr_buf containing RPC message
 *
 * Returns:
 *   %0 if DMA mapping was successful.
 *   %-EMSGSIZE if a buffer manipulation problem occurred
 *   %-EIO if DMA mapping failed
 *
 * The Send WR's num_sge field is set in all cases.
 */
int svc_rdma_map_reply_msg(struct svcxprt_rdma *rdma,
			   struct svc_rdma_send_ctxt *sctxt,
			   const struct svc_rdma_pcl *write_pcl,
			   const struct svc_rdma_pcl *reply_pcl,
			   const struct xdr_buf *xdr)
{
	struct svc_rdma_map_data args = {
		.md_rdma	= rdma,
		.md_ctxt	= sctxt,
	};

	/* Set up the (persistently-mapped) transport header SGE. */
	sctxt->sc_send_wr.num_sge = 1;
	sctxt->sc_sges[0].length = sctxt->sc_hdrbuf.len;

	/* If there is a Reply chunk, nothing follows the transport
	 * header, so there is nothing to map.
	 */
	if (!pcl_is_empty(reply_pcl))
		return 0;

	/* For pull-up, svc_rdma_send() will sync the transport header.
	 * No additional DMA mapping is necessary.
	 */
	if (svc_rdma_pull_up_needed(rdma, sctxt, write_pcl, xdr))
		return svc_rdma_pull_up_reply_msg(rdma, sctxt, write_pcl, xdr);

	return pcl_process_nonpayloads(write_pcl, xdr,
				       svc_rdma_xb_dma_map, &args);
}

/* The svc_rqst and all resources it owns are released as soon as
 * svc_rdma_sendto returns. Transfer pages under I/O to the ctxt
 * so they are released by the Send completion handler.
 */
static void svc_rdma_save_io_pages(struct svc_rqst *rqstp,
				   struct svc_rdma_send_ctxt *ctxt)
{
	int i, pages = rqstp->rq_next_page - rqstp->rq_respages;

	ctxt->sc_page_count += pages;
	for (i = 0; i < pages; i++) {
		ctxt->sc_pages[i] = rqstp->rq_respages[i];
		rqstp->rq_respages[i] = NULL;
	}

	/* Prevent svc_xprt_release from releasing pages in rq_pages */
	rqstp->rq_next_page = rqstp->rq_respages;
}

/* Prepare the portion of the RPC Reply that will be transmitted
 * via RDMA Send. The RPC-over-RDMA transport header is prepared
 * in sc_sges[0], and the RPC xdr_buf is prepared in following sges.
 *
 * Depending on whether a Write list or Reply chunk is present,
 * the server may Send all, a portion of, or none of the xdr_buf.
 * In the latter case, only the transport header (sc_sges[0]) is
 * transmitted.
 *
 * Assumptions:
 * - The Reply's transport header will never be larger than a page.
 */
static int svc_rdma_send_reply_msg(struct svcxprt_rdma *rdma,
				   struct svc_rdma_send_ctxt *sctxt,
				   const struct svc_rdma_recv_ctxt *rctxt,
				   struct svc_rqst *rqstp)
{
	struct ib_send_wr *send_wr = &sctxt->sc_send_wr;
	int ret;

	ret = svc_rdma_map_reply_msg(rdma, sctxt, &rctxt->rc_write_pcl,
				     &rctxt->rc_reply_pcl, &rqstp->rq_res);
	if (ret < 0)
		return ret;

	/* Transfer pages involved in RDMA Writes to the sctxt's
	 * page array. Completion handling releases these pages.
	 */
	svc_rdma_save_io_pages(rqstp, sctxt);

	if (rctxt->rc_inv_rkey) {
		send_wr->opcode = IB_WR_SEND_WITH_INV;
		send_wr->ex.invalidate_rkey = rctxt->rc_inv_rkey;
	} else {
		send_wr->opcode = IB_WR_SEND;
	}

	return svc_rdma_post_send(rdma, sctxt);
}

/**
 * svc_rdma_send_error_msg - Send an RPC/RDMA v1 error response
 * @rdma: controlling transport context
 * @sctxt: Send context for the response
 * @rctxt: Receive context for incoming bad message
 * @status: negative errno indicating error that occurred
 *
 * Given the client-provided Read, Write, and Reply chunks, the
 * server was not able to parse the Call or form a complete Reply.
 * Return an RDMA_ERROR message so the client can retire the RPC
 * transaction.
 *
 * The caller does not have to release @sctxt. It is released by
 * Send completion, or by this function on error.
 */
void svc_rdma_send_error_msg(struct svcxprt_rdma *rdma,
			     struct svc_rdma_send_ctxt *sctxt,
			     struct svc_rdma_recv_ctxt *rctxt,
			     int status)
{
	__be32 *rdma_argp = rctxt->rc_recv_buf;
	__be32 *p;

	rpcrdma_set_xdrlen(&sctxt->sc_hdrbuf, 0);
	xdr_init_encode(&sctxt->sc_stream, &sctxt->sc_hdrbuf,
			sctxt->sc_xprt_buf, NULL);

	p = xdr_reserve_space(&sctxt->sc_stream,
			      rpcrdma_fixed_maxsz * sizeof(*p));
	if (!p)
		goto put_ctxt;

	*p++ = *rdma_argp;
	*p++ = *(rdma_argp + 1);
	*p++ = rdma->sc_fc_credits;
	*p = rdma_error;

	switch (status) {
	case -EPROTONOSUPPORT:
		p = xdr_reserve_space(&sctxt->sc_stream, 3 * sizeof(*p));
		if (!p)
			goto put_ctxt;

		*p++ = err_vers;
		*p++ = rpcrdma_version;
		*p = rpcrdma_version;
		trace_svcrdma_err_vers(*rdma_argp);
		break;
	default:
		p = xdr_reserve_space(&sctxt->sc_stream, sizeof(*p));
		if (!p)
			goto put_ctxt;

		*p = err_chunk;
		trace_svcrdma_err_chunk(*rdma_argp);
	}

	/* Remote Invalidation is skipped for simplicity. */
	sctxt->sc_send_wr.num_sge = 1;
	sctxt->sc_send_wr.opcode = IB_WR_SEND;
	sctxt->sc_sges[0].length = sctxt->sc_hdrbuf.len;
	if (svc_rdma_post_send(rdma, sctxt))
		goto put_ctxt;
	return;

put_ctxt:
	svc_rdma_send_ctxt_put(rdma, sctxt);
}

/**
 * svc_rdma_sendto - Transmit an RPC reply
 * @rqstp: processed RPC request, reply XDR already in ::rq_res
 *
 * Any resources still associated with @rqstp are released upon return.
 * If no reply message was possible, the connection is closed.
 *
 * Returns:
 *	%0 if an RPC reply has been successfully posted,
 *	%-ENOMEM if a resource shortage occurred (connection is lost),
 *	%-ENOTCONN if posting failed (connection is lost).
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int svc_rdma_sendto(struct svc_rqst *rqstp)
{
	struct svc_xprt *xprt = rqstp->rq_xprt;
	struct svcxprt_rdma *rdma =
		container_of(xprt, struct svcxprt_rdma, sc_xprt);
<<<<<<< HEAD
	struct rpcrdma_msg *rdma_argp;
	struct rpcrdma_msg *rdma_resp;
	struct rpcrdma_write_array *reply_ary;
	enum rpcrdma_proc reply_type;
	int ret;
	int inline_bytes;
	struct page *res_page;
	struct svc_rdma_op_ctxt *ctxt;
	struct svc_rdma_req_map *vec;

	dprintk("svcrdma: sending response for rqstp=%p\n", rqstp);

	/* Get the RDMA request header. */
	rdma_argp = xdr_start(&rqstp->rq_arg);

	/* Build an req vec for the XDR */
	ctxt = svc_rdma_get_context(rdma);
	ctxt->direction = DMA_TO_DEVICE;
	vec = svc_rdma_get_req_map();
	ret = map_xdr(rdma, &rqstp->rq_res, vec);
	if (ret)
		goto err0;
	inline_bytes = rqstp->rq_res.len;

	/* Create the RDMA response header */
	res_page = svc_rdma_get_page();
	rdma_resp = page_address(res_page);
	reply_ary = svc_rdma_get_reply_array(rdma_argp);
	if (reply_ary)
		reply_type = RDMA_NOMSG;
	else
		reply_type = RDMA_MSG;
	svc_rdma_xdr_encode_reply_header(rdma, rdma_argp,
					 rdma_resp, reply_type);

	/* Send any write-chunk data and build resp write-list */
	ret = send_write_chunks(rdma, rdma_argp, rdma_resp,
				rqstp, vec);
	if (ret < 0) {
		printk(KERN_ERR "svcrdma: failed to send write chunks, rc=%d\n",
		       ret);
		goto err1;
	}
	inline_bytes -= ret;

	/* Send any reply-list data and update resp reply-list */
	ret = send_reply_chunks(rdma, rdma_argp, rdma_resp,
				rqstp, vec);
	if (ret < 0) {
		printk(KERN_ERR "svcrdma: failed to send reply chunks, rc=%d\n",
		       ret);
		goto err1;
	}
	inline_bytes -= ret;

	ret = send_reply(rdma, rqstp, res_page, rdma_resp, ctxt, vec,
			 inline_bytes);
	svc_rdma_put_req_map(vec);
	dprintk("svcrdma: send_reply returns %d\n", ret);
	return ret;

 err1:
	put_page(res_page);
 err0:
	svc_rdma_put_req_map(vec);
	svc_rdma_put_context(ctxt, 0);
	return ret;
=======
	struct svc_rdma_recv_ctxt *rctxt = rqstp->rq_xprt_ctxt;
	__be32 *rdma_argp = rctxt->rc_recv_buf;
	struct svc_rdma_send_ctxt *sctxt;
	unsigned int rc_size;
	__be32 *p;
	int ret;

	ret = -ENOTCONN;
	if (svc_xprt_is_dead(xprt))
		goto drop_connection;

	ret = -ENOMEM;
	sctxt = svc_rdma_send_ctxt_get(rdma);
	if (!sctxt)
		goto drop_connection;

	ret = -EMSGSIZE;
	p = xdr_reserve_space(&sctxt->sc_stream,
			      rpcrdma_fixed_maxsz * sizeof(*p));
	if (!p)
		goto put_ctxt;

	ret = svc_rdma_send_write_list(rdma, rctxt, &rqstp->rq_res);
	if (ret < 0)
		goto put_ctxt;

	rc_size = 0;
	if (!pcl_is_empty(&rctxt->rc_reply_pcl)) {
		ret = svc_rdma_prepare_reply_chunk(rdma, &rctxt->rc_write_pcl,
						   &rctxt->rc_reply_pcl, sctxt,
						   &rqstp->rq_res);
		if (ret < 0)
			goto reply_chunk;
		rc_size = ret;
	}

	*p++ = *rdma_argp;
	*p++ = *(rdma_argp + 1);
	*p++ = rdma->sc_fc_credits;
	*p = pcl_is_empty(&rctxt->rc_reply_pcl) ? rdma_msg : rdma_nomsg;

	ret = svc_rdma_encode_read_list(sctxt);
	if (ret < 0)
		goto put_ctxt;
	ret = svc_rdma_encode_write_list(rctxt, sctxt);
	if (ret < 0)
		goto put_ctxt;
	ret = svc_rdma_encode_reply_chunk(rctxt, sctxt, rc_size);
	if (ret < 0)
		goto put_ctxt;

	ret = svc_rdma_send_reply_msg(rdma, sctxt, rctxt, rqstp);
	if (ret < 0)
		goto put_ctxt;
	return 0;

reply_chunk:
	if (ret != -E2BIG && ret != -EINVAL)
		goto put_ctxt;

	/* Send completion releases payload pages that were part
	 * of previously posted RDMA Writes.
	 */
	svc_rdma_save_io_pages(rqstp, sctxt);
	svc_rdma_send_error_msg(rdma, sctxt, rctxt, ret);
	return 0;

put_ctxt:
	svc_rdma_send_ctxt_put(rdma, sctxt);
drop_connection:
	trace_svcrdma_send_err(rqstp, ret);
	svc_xprt_deferred_close(&rdma->sc_xprt);
	return -ENOTCONN;
}

/**
 * svc_rdma_result_payload - special processing for a result payload
 * @rqstp: RPC transaction context
 * @offset: payload's byte offset in @rqstp->rq_res
 * @length: size of payload, in bytes
 *
 * Assign the passed-in result payload to the current Write chunk,
 * and advance to cur_result_payload to the next Write chunk, if
 * there is one.
 *
 * Return values:
 *   %0 if successful or nothing needed to be done
 *   %-E2BIG if the payload was larger than the Write chunk
 */
int svc_rdma_result_payload(struct svc_rqst *rqstp, unsigned int offset,
			    unsigned int length)
{
	struct svc_rdma_recv_ctxt *rctxt = rqstp->rq_xprt_ctxt;
	struct svc_rdma_chunk *chunk;

	chunk = rctxt->rc_cur_result_payload;
	if (!length || !chunk)
		return 0;
	rctxt->rc_cur_result_payload =
		pcl_next_chunk(&rctxt->rc_write_pcl, chunk);

	if (length > chunk->ch_length)
		return -E2BIG;
	chunk->ch_position = offset;
	chunk->ch_payload_length = length;
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
