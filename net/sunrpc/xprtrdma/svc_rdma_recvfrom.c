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
=======
/* Operation
 *
 * The main entry point is svc_rdma_recvfrom. This is called from
 * svc_recv when the transport indicates there is incoming data to
 * be read. "Data Ready" is signaled when an RDMA Receive completes,
 * or when a set of RDMA Reads complete.
 *
 * An svc_rqst is passed in. This structure contains an array of
 * free pages (rq_pages) that will contain the incoming RPC message.
 *
 * Short messages are moved directly into svc_rqst::rq_arg, and
 * the RPC Call is ready to be processed by the Upper Layer.
 * svc_rdma_recvfrom returns the length of the RPC Call message,
 * completing the reception of the RPC Call.
 *
 * However, when an incoming message has Read chunks,
 * svc_rdma_recvfrom must post RDMA Reads to pull the RPC Call's
 * data payload from the client. svc_rdma_recvfrom sets up the
 * RDMA Reads using pages in svc_rqst::rq_pages, which are
 * transferred to an svc_rdma_recv_ctxt for the duration of the
 * I/O. svc_rdma_recvfrom then returns zero, since the RPC message
 * is still not yet ready.
 *
 * When the Read chunk payloads have become available on the
 * server, "Data Ready" is raised again, and svc_recv calls
 * svc_rdma_recvfrom again. This second call may use a different
 * svc_rqst than the first one, thus any information that needs
 * to be preserved across these two calls is kept in an
 * svc_rdma_recv_ctxt.
 *
 * The second call to svc_rdma_recvfrom performs final assembly
 * of the RPC Call message, using the RDMA Read sink pages kept in
 * the svc_rdma_recv_ctxt. The xdr_buf is copied from the
 * svc_rdma_recv_ctxt to the second svc_rqst. The second call returns
 * the length of the completed RPC Call message.
 *
 * Page Management
 *
 * Pages under I/O must be transferred from the first svc_rqst to an
 * svc_rdma_recv_ctxt before the first svc_rdma_recvfrom call returns.
 *
 * The first svc_rqst supplies pages for RDMA Reads. These are moved
 * from rqstp::rq_pages into ctxt::pages. The consumed elements of
 * the rq_pages array are set to NULL and refilled with the first
 * svc_rdma_recvfrom call returns.
 *
 * During the second svc_rdma_recvfrom call, RDMA Read sink pages
 * are transferred from the svc_rdma_recv_ctxt to the second svc_rqst.
 */

#include <linux/slab.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/spinlock.h>
#include <asm/unaligned.h>
#include <rdma/ib_verbs.h>
#include <rdma/rdma_cm.h>
<<<<<<< HEAD
#include <linux/sunrpc/svc_rdma.h>

#define RPCDBG_FACILITY	RPCDBG_SVCXPRT

/*
 * Replace the pages in the rq_argpages array with the pages from the SGE in
 * the RDMA_RECV completion. The SGL should contain full pages up until the
 * last one.
 */
static void rdma_build_arg_xdr(struct svc_rqst *rqstp,
			       struct svc_rdma_op_ctxt *ctxt,
			       u32 byte_count)
{
	struct page *page;
	u32 bc;
	int sge_no;

	/* Swap the page in the SGE with the page in argpages */
	page = ctxt->pages[0];
	put_page(rqstp->rq_pages[0]);
	rqstp->rq_pages[0] = page;

	/* Set up the XDR head */
	rqstp->rq_arg.head[0].iov_base = page_address(page);
	rqstp->rq_arg.head[0].iov_len = min(byte_count, ctxt->sge[0].length);
	rqstp->rq_arg.len = byte_count;
	rqstp->rq_arg.buflen = byte_count;

	/* Compute bytes past head in the SGL */
	bc = byte_count - rqstp->rq_arg.head[0].iov_len;

	/* If data remains, store it in the pagelist */
	rqstp->rq_arg.page_len = bc;
	rqstp->rq_arg.page_base = 0;
	rqstp->rq_arg.pages = &rqstp->rq_pages[1];
	sge_no = 1;
	while (bc && sge_no < ctxt->count) {
		page = ctxt->pages[sge_no];
		put_page(rqstp->rq_pages[sge_no]);
		rqstp->rq_pages[sge_no] = page;
		bc -= min(bc, ctxt->sge[sge_no].length);
		rqstp->rq_arg.buflen += ctxt->sge[sge_no].length;
		sge_no++;
	}
	rqstp->rq_respages = &rqstp->rq_pages[sge_no];

	/* We should never run out of SGE because the limit is defined to
	 * support the max allowed RPC data length
	 */
	BUG_ON(bc && (sge_no == ctxt->count));
	BUG_ON((rqstp->rq_arg.head[0].iov_len + rqstp->rq_arg.page_len)
	       != byte_count);
	BUG_ON(rqstp->rq_arg.len != byte_count);

	/* If not all pages were used from the SGL, free the remaining ones */
	bc = sge_no;
	while (sge_no < ctxt->count) {
		page = ctxt->pages[sge_no++];
		put_page(page);
	}
	ctxt->count = bc;

	/* Set up tail */
	rqstp->rq_arg.tail[0].iov_base = NULL;
	rqstp->rq_arg.tail[0].iov_len = 0;
}

/* Encode a read-chunk-list as an array of IB SGE
 *
 * Assumptions:
 * - chunk[0]->position points to pages[0] at an offset of 0
 * - pages[] is not physically or virtually contiguous and consists of
 *   PAGE_SIZE elements.
 *
 * Output:
 * - sge array pointing into pages[] array.
 * - chunk_sge array specifying sge index and count for each
 *   chunk in the read list
 *
 */
static int map_read_chunks(struct svcxprt_rdma *xprt,
			   struct svc_rqst *rqstp,
			   struct svc_rdma_op_ctxt *head,
			   struct rpcrdma_msg *rmsgp,
			   struct svc_rdma_req_map *rpl_map,
			   struct svc_rdma_req_map *chl_map,
			   int ch_count,
			   int byte_count)
{
	int sge_no;
	int sge_bytes;
	int page_off;
	int page_no;
	int ch_bytes;
	int ch_no;
	struct rpcrdma_read_chunk *ch;

	sge_no = 0;
	page_no = 0;
	page_off = 0;
	ch = (struct rpcrdma_read_chunk *)&rmsgp->rm_body.rm_chunks[0];
	ch_no = 0;
	ch_bytes = ntohl(ch->rc_target.rs_length);
	head->arg.head[0] = rqstp->rq_arg.head[0];
	head->arg.tail[0] = rqstp->rq_arg.tail[0];
	head->arg.pages = &head->pages[head->count];
	head->hdr_count = head->count; /* save count of hdr pages */
	head->arg.page_base = 0;
	head->arg.page_len = ch_bytes;
	head->arg.len = rqstp->rq_arg.len + ch_bytes;
	head->arg.buflen = rqstp->rq_arg.buflen + ch_bytes;
	head->count++;
	chl_map->ch[0].start = 0;
	while (byte_count) {
		rpl_map->sge[sge_no].iov_base =
			page_address(rqstp->rq_arg.pages[page_no]) + page_off;
		sge_bytes = min_t(int, PAGE_SIZE-page_off, ch_bytes);
		rpl_map->sge[sge_no].iov_len = sge_bytes;
		/*
		 * Don't bump head->count here because the same page
		 * may be used by multiple SGE.
		 */
		head->arg.pages[page_no] = rqstp->rq_arg.pages[page_no];
		rqstp->rq_respages = &rqstp->rq_arg.pages[page_no+1];

		byte_count -= sge_bytes;
		ch_bytes -= sge_bytes;
		sge_no++;
		/*
		 * If all bytes for this chunk have been mapped to an
		 * SGE, move to the next SGE
		 */
		if (ch_bytes == 0) {
			chl_map->ch[ch_no].count =
				sge_no - chl_map->ch[ch_no].start;
			ch_no++;
			ch++;
			chl_map->ch[ch_no].start = sge_no;
			ch_bytes = ntohl(ch->rc_target.rs_length);
			/* If bytes remaining account for next chunk */
			if (byte_count) {
				head->arg.page_len += ch_bytes;
				head->arg.len += ch_bytes;
				head->arg.buflen += ch_bytes;
			}
		}
		/*
		 * If this SGE consumed all of the page, move to the
		 * next page
		 */
		if ((sge_bytes + page_off) == PAGE_SIZE) {
			page_no++;
			page_off = 0;
			/*
			 * If there are still bytes left to map, bump
			 * the page count
			 */
			if (byte_count)
				head->count++;
		} else
			page_off += sge_bytes;
	}
	BUG_ON(byte_count != 0);
	return sge_no;
}

/* Map a read-chunk-list to an XDR and fast register the page-list.
 *
 * Assumptions:
 * - chunk[0]	position points to pages[0] at an offset of 0
 * - pages[]	will be made physically contiguous by creating a one-off memory
 *		region using the fastreg verb.
 * - byte_count is # of bytes in read-chunk-list
 * - ch_count	is # of chunks in read-chunk-list
 *
 * Output:
 * - sge array pointing into pages[] array.
 * - chunk_sge array specifying sge index and count for each
 *   chunk in the read list
 */
static int fast_reg_read_chunks(struct svcxprt_rdma *xprt,
				struct svc_rqst *rqstp,
				struct svc_rdma_op_ctxt *head,
				struct rpcrdma_msg *rmsgp,
				struct svc_rdma_req_map *rpl_map,
				struct svc_rdma_req_map *chl_map,
				int ch_count,
				int byte_count)
{
	int page_no;
	int ch_no;
	u32 offset;
	struct rpcrdma_read_chunk *ch;
	struct svc_rdma_fastreg_mr *frmr;
	int ret = 0;

	frmr = svc_rdma_get_frmr(xprt);
	if (IS_ERR(frmr))
		return -ENOMEM;

	head->frmr = frmr;
	head->arg.head[0] = rqstp->rq_arg.head[0];
	head->arg.tail[0] = rqstp->rq_arg.tail[0];
	head->arg.pages = &head->pages[head->count];
	head->hdr_count = head->count; /* save count of hdr pages */
	head->arg.page_base = 0;
	head->arg.page_len = byte_count;
	head->arg.len = rqstp->rq_arg.len + byte_count;
	head->arg.buflen = rqstp->rq_arg.buflen + byte_count;

	/* Fast register the page list */
	frmr->kva = page_address(rqstp->rq_arg.pages[0]);
	frmr->direction = DMA_FROM_DEVICE;
	frmr->access_flags = (IB_ACCESS_LOCAL_WRITE|IB_ACCESS_REMOTE_WRITE);
	frmr->map_len = byte_count;
	frmr->page_list_len = PAGE_ALIGN(byte_count) >> PAGE_SHIFT;
	for (page_no = 0; page_no < frmr->page_list_len; page_no++) {
		frmr->page_list->page_list[page_no] =
			ib_dma_map_page(xprt->sc_cm_id->device,
					rqstp->rq_arg.pages[page_no], 0,
					PAGE_SIZE, DMA_FROM_DEVICE);
		if (ib_dma_mapping_error(xprt->sc_cm_id->device,
					 frmr->page_list->page_list[page_no]))
			goto fatal_err;
		atomic_inc(&xprt->sc_dma_used);
		head->arg.pages[page_no] = rqstp->rq_arg.pages[page_no];
	}
	head->count += page_no;

	/* rq_respages points one past arg pages */
	rqstp->rq_respages = &rqstp->rq_arg.pages[page_no];

	/* Create the reply and chunk maps */
	offset = 0;
	ch = (struct rpcrdma_read_chunk *)&rmsgp->rm_body.rm_chunks[0];
	for (ch_no = 0; ch_no < ch_count; ch_no++) {
		int len = ntohl(ch->rc_target.rs_length);
		rpl_map->sge[ch_no].iov_base = frmr->kva + offset;
		rpl_map->sge[ch_no].iov_len = len;
		chl_map->ch[ch_no].count = 1;
		chl_map->ch[ch_no].start = ch_no;
		offset += len;
		ch++;
	}

	ret = svc_rdma_fastreg(xprt, frmr);
	if (ret)
		goto fatal_err;

	return ch_no;

 fatal_err:
	printk("svcrdma: error fast registering xdr for xprt %p", xprt);
	svc_rdma_put_frmr(xprt, frmr);
	return -EIO;
}

static int rdma_set_ctxt_sge(struct svcxprt_rdma *xprt,
			     struct svc_rdma_op_ctxt *ctxt,
			     struct svc_rdma_fastreg_mr *frmr,
			     struct kvec *vec,
			     u64 *sgl_offset,
			     int count)
{
	int i;
	unsigned long off;

	ctxt->count = count;
	ctxt->direction = DMA_FROM_DEVICE;
	for (i = 0; i < count; i++) {
		ctxt->sge[i].length = 0; /* in case map fails */
		if (!frmr) {
			BUG_ON(!virt_to_page(vec[i].iov_base));
			off = (unsigned long)vec[i].iov_base & ~PAGE_MASK;
			ctxt->sge[i].addr =
				ib_dma_map_page(xprt->sc_cm_id->device,
						virt_to_page(vec[i].iov_base),
						off,
						vec[i].iov_len,
						DMA_FROM_DEVICE);
			if (ib_dma_mapping_error(xprt->sc_cm_id->device,
						 ctxt->sge[i].addr))
				return -EINVAL;
			ctxt->sge[i].lkey = xprt->sc_dma_lkey;
			atomic_inc(&xprt->sc_dma_used);
		} else {
			ctxt->sge[i].addr = (unsigned long)vec[i].iov_base;
			ctxt->sge[i].lkey = frmr->mr->lkey;
		}
		ctxt->sge[i].length = vec[i].iov_len;
		*sgl_offset = *sgl_offset + vec[i].iov_len;
	}
	return 0;
}

static int rdma_read_max_sge(struct svcxprt_rdma *xprt, int sge_count)
{
	if ((rdma_node_get_transport(xprt->sc_cm_id->device->node_type) ==
	     RDMA_TRANSPORT_IWARP) &&
	    sge_count > 1)
		return 1;
	else
		return min_t(int, sge_count, xprt->sc_max_sge);
}

/*
 * Use RDMA_READ to read data from the advertised client buffer into the
 * XDR stream starting at rq_arg.head[0].iov_base.
 * Each chunk in the array
 * contains the following fields:
 * discrim      - '1', This isn't used for data placement
 * position     - The xdr stream offset (the same for every chunk)
 * handle       - RMR for client memory region
 * length       - data transfer length
 * offset       - 64 bit tagged offset in remote memory region
 *
 * On our side, we need to read into a pagelist. The first page immediately
 * follows the RPC header.
 *
 * This function returns:
 * 0 - No error and no read-list found.
 *
 * 1 - Successful read-list processing. The data is not yet in
 * the pagelist and therefore the RPC request must be deferred. The
 * I/O completion will enqueue the transport again and
 * svc_rdma_recvfrom will complete the request.
 *
 * <0 - Error processing/posting read-list.
 *
 * NOTE: The ctxt must not be touched after the last WR has been posted
 * because the I/O completion processing may occur on another
 * processor and free / modify the context. Ne touche pas!
 */
static int rdma_read_xdr(struct svcxprt_rdma *xprt,
			 struct rpcrdma_msg *rmsgp,
			 struct svc_rqst *rqstp,
			 struct svc_rdma_op_ctxt *hdr_ctxt)
{
	struct ib_send_wr read_wr;
	struct ib_send_wr inv_wr;
	int err = 0;
	int ch_no;
	int ch_count;
	int byte_count;
	int sge_count;
	u64 sgl_offset;
	struct rpcrdma_read_chunk *ch;
	struct svc_rdma_op_ctxt *ctxt = NULL;
	struct svc_rdma_req_map *rpl_map;
	struct svc_rdma_req_map *chl_map;

	/* If no read list is present, return 0 */
	ch = svc_rdma_get_read_chunk(rmsgp);
	if (!ch)
		return 0;

	svc_rdma_rcl_chunk_counts(ch, &ch_count, &byte_count);
	if (ch_count > RPCSVC_MAXPAGES)
		return -EINVAL;

	/* Allocate temporary reply and chunk maps */
	rpl_map = svc_rdma_get_req_map();
	chl_map = svc_rdma_get_req_map();

	if (!xprt->sc_frmr_pg_list_len)
		sge_count = map_read_chunks(xprt, rqstp, hdr_ctxt, rmsgp,
					    rpl_map, chl_map, ch_count,
					    byte_count);
	else
		sge_count = fast_reg_read_chunks(xprt, rqstp, hdr_ctxt, rmsgp,
						 rpl_map, chl_map, ch_count,
						 byte_count);
	if (sge_count < 0) {
		err = -EIO;
		goto out;
	}

	sgl_offset = 0;
	ch_no = 0;

	for (ch = (struct rpcrdma_read_chunk *)&rmsgp->rm_body.rm_chunks[0];
	     ch->rc_discrim != 0; ch++, ch_no++) {
		u64 rs_offset;
next_sge:
		ctxt = svc_rdma_get_context(xprt);
		ctxt->direction = DMA_FROM_DEVICE;
		ctxt->frmr = hdr_ctxt->frmr;
		ctxt->read_hdr = NULL;
		clear_bit(RDMACTXT_F_LAST_CTXT, &ctxt->flags);
		clear_bit(RDMACTXT_F_FAST_UNREG, &ctxt->flags);

		/* Prepare READ WR */
		memset(&read_wr, 0, sizeof read_wr);
		read_wr.wr_id = (unsigned long)ctxt;
		read_wr.opcode = IB_WR_RDMA_READ;
		ctxt->wr_op = read_wr.opcode;
		read_wr.send_flags = IB_SEND_SIGNALED;
		read_wr.wr.rdma.rkey = ntohl(ch->rc_target.rs_handle);
		xdr_decode_hyper((__be32 *)&ch->rc_target.rs_offset,
				 &rs_offset);
		read_wr.wr.rdma.remote_addr = rs_offset + sgl_offset;
		read_wr.sg_list = ctxt->sge;
		read_wr.num_sge =
			rdma_read_max_sge(xprt, chl_map->ch[ch_no].count);
		err = rdma_set_ctxt_sge(xprt, ctxt, hdr_ctxt->frmr,
					&rpl_map->sge[chl_map->ch[ch_no].start],
					&sgl_offset,
					read_wr.num_sge);
		if (err) {
			svc_rdma_unmap_dma(ctxt);
			svc_rdma_put_context(ctxt, 0);
			goto out;
		}
		if (((ch+1)->rc_discrim == 0) &&
		    (read_wr.num_sge == chl_map->ch[ch_no].count)) {
			/*
			 * Mark the last RDMA_READ with a bit to
			 * indicate all RPC data has been fetched from
			 * the client and the RPC needs to be enqueued.
			 */
			set_bit(RDMACTXT_F_LAST_CTXT, &ctxt->flags);
			if (hdr_ctxt->frmr) {
				set_bit(RDMACTXT_F_FAST_UNREG, &ctxt->flags);
				/*
				 * Invalidate the local MR used to map the data
				 * sink.
				 */
				if (xprt->sc_dev_caps &
				    SVCRDMA_DEVCAP_READ_W_INV) {
					read_wr.opcode =
						IB_WR_RDMA_READ_WITH_INV;
					ctxt->wr_op = read_wr.opcode;
					read_wr.ex.invalidate_rkey =
						ctxt->frmr->mr->lkey;
				} else {
					/* Prepare INVALIDATE WR */
					memset(&inv_wr, 0, sizeof inv_wr);
					inv_wr.opcode = IB_WR_LOCAL_INV;
					inv_wr.send_flags = IB_SEND_SIGNALED;
					inv_wr.ex.invalidate_rkey =
						hdr_ctxt->frmr->mr->lkey;
					read_wr.next = &inv_wr;
				}
			}
			ctxt->read_hdr = hdr_ctxt;
		}
		/* Post the read */
		err = svc_rdma_send(xprt, &read_wr);
		if (err) {
			printk(KERN_ERR "svcrdma: Error %d posting RDMA_READ\n",
			       err);
			set_bit(XPT_CLOSE, &xprt->sc_xprt.xpt_flags);
			svc_rdma_unmap_dma(ctxt);
			svc_rdma_put_context(ctxt, 0);
			goto out;
		}
		atomic_inc(&rdma_stat_read);

		if (read_wr.num_sge < chl_map->ch[ch_no].count) {
			chl_map->ch[ch_no].count -= read_wr.num_sge;
			chl_map->ch[ch_no].start += read_wr.num_sge;
			goto next_sge;
		}
		sgl_offset = 0;
		err = 1;
	}

 out:
	svc_rdma_put_req_map(rpl_map);
	svc_rdma_put_req_map(chl_map);

	/* Detach arg pages. svc_recv will replenish them */
	for (ch_no = 0; &rqstp->rq_pages[ch_no] < rqstp->rq_respages; ch_no++)
		rqstp->rq_pages[ch_no] = NULL;

	/*
	 * Detach res pages. svc_release must see a resused count of
	 * zero or it will attempt to put them.
	 */
	while (rqstp->rq_resused)
		rqstp->rq_respages[--rqstp->rq_resused] = NULL;

	return err;
}

static int rdma_read_complete(struct svc_rqst *rqstp,
			      struct svc_rdma_op_ctxt *head)
{
	int page_no;
	int ret;

	BUG_ON(!head);

	/* Copy RPC pages */
	for (page_no = 0; page_no < head->count; page_no++) {
		put_page(rqstp->rq_pages[page_no]);
		rqstp->rq_pages[page_no] = head->pages[page_no];
	}
	/* Point rq_arg.pages past header */
	rqstp->rq_arg.pages = &rqstp->rq_pages[head->hdr_count];
	rqstp->rq_arg.page_len = head->arg.page_len;
	rqstp->rq_arg.page_base = head->arg.page_base;

	/* rq_respages starts after the last arg page */
	rqstp->rq_respages = &rqstp->rq_arg.pages[page_no];
	rqstp->rq_resused = 0;

	/* Rebuild rq_arg head and tail. */
	rqstp->rq_arg.head[0] = head->arg.head[0];
	rqstp->rq_arg.tail[0] = head->arg.tail[0];
	rqstp->rq_arg.len = head->arg.len;
	rqstp->rq_arg.buflen = head->arg.buflen;

	/* Free the context */
	svc_rdma_put_context(head, 0);

	/* XXX: What should this be? */
	rqstp->rq_prot = IPPROTO_MAX;
	svc_xprt_copy_addrs(rqstp, rqstp->rq_xprt);

	ret = rqstp->rq_arg.head[0].iov_len
		+ rqstp->rq_arg.page_len
		+ rqstp->rq_arg.tail[0].iov_len;
	dprintk("svcrdma: deferred read ret=%d, rq_arg.len =%d, "
		"rq_arg.head[0].iov_base=%p, rq_arg.head[0].iov_len = %zd\n",
		ret, rqstp->rq_arg.len,	rqstp->rq_arg.head[0].iov_base,
		rqstp->rq_arg.head[0].iov_len);

	return ret;
}

/*
 * Set up the rqstp thread context to point to the RQ buffer. If
 * necessary, pull additional data from the client with an RDMA_READ
 * request.
=======

#include <linux/sunrpc/xdr.h>
#include <linux/sunrpc/debug.h>
#include <linux/sunrpc/rpc_rdma.h>
#include <linux/sunrpc/svc_rdma.h>

#include "xprt_rdma.h"
#include <trace/events/rpcrdma.h>

static void svc_rdma_wc_receive(struct ib_cq *cq, struct ib_wc *wc);

static inline struct svc_rdma_recv_ctxt *
svc_rdma_next_recv_ctxt(struct list_head *list)
{
	return list_first_entry_or_null(list, struct svc_rdma_recv_ctxt,
					rc_list);
}

static struct svc_rdma_recv_ctxt *
svc_rdma_recv_ctxt_alloc(struct svcxprt_rdma *rdma)
{
	int node = ibdev_to_node(rdma->sc_cm_id->device);
	struct svc_rdma_recv_ctxt *ctxt;
	dma_addr_t addr;
	void *buffer;

	ctxt = kzalloc_node(sizeof(*ctxt), GFP_KERNEL, node);
	if (!ctxt)
		goto fail0;
	buffer = kmalloc_node(rdma->sc_max_req_size, GFP_KERNEL, node);
	if (!buffer)
		goto fail1;
	addr = ib_dma_map_single(rdma->sc_pd->device, buffer,
				 rdma->sc_max_req_size, DMA_FROM_DEVICE);
	if (ib_dma_mapping_error(rdma->sc_pd->device, addr))
		goto fail2;

	svc_rdma_recv_cid_init(rdma, &ctxt->rc_cid);
	pcl_init(&ctxt->rc_call_pcl);
	pcl_init(&ctxt->rc_read_pcl);
	pcl_init(&ctxt->rc_write_pcl);
	pcl_init(&ctxt->rc_reply_pcl);

	ctxt->rc_recv_wr.next = NULL;
	ctxt->rc_recv_wr.wr_cqe = &ctxt->rc_cqe;
	ctxt->rc_recv_wr.sg_list = &ctxt->rc_recv_sge;
	ctxt->rc_recv_wr.num_sge = 1;
	ctxt->rc_cqe.done = svc_rdma_wc_receive;
	ctxt->rc_recv_sge.addr = addr;
	ctxt->rc_recv_sge.length = rdma->sc_max_req_size;
	ctxt->rc_recv_sge.lkey = rdma->sc_pd->local_dma_lkey;
	ctxt->rc_recv_buf = buffer;
	svc_rdma_cc_init(rdma, &ctxt->rc_cc);
	return ctxt;

fail2:
	kfree(buffer);
fail1:
	kfree(ctxt);
fail0:
	return NULL;
}

static void svc_rdma_recv_ctxt_destroy(struct svcxprt_rdma *rdma,
				       struct svc_rdma_recv_ctxt *ctxt)
{
	ib_dma_unmap_single(rdma->sc_pd->device, ctxt->rc_recv_sge.addr,
			    ctxt->rc_recv_sge.length, DMA_FROM_DEVICE);
	kfree(ctxt->rc_recv_buf);
	kfree(ctxt);
}

/**
 * svc_rdma_recv_ctxts_destroy - Release all recv_ctxt's for an xprt
 * @rdma: svcxprt_rdma being torn down
 *
 */
void svc_rdma_recv_ctxts_destroy(struct svcxprt_rdma *rdma)
{
	struct svc_rdma_recv_ctxt *ctxt;
	struct llist_node *node;

	while ((node = llist_del_first(&rdma->sc_recv_ctxts))) {
		ctxt = llist_entry(node, struct svc_rdma_recv_ctxt, rc_node);
		svc_rdma_recv_ctxt_destroy(rdma, ctxt);
	}
}

/**
 * svc_rdma_recv_ctxt_get - Allocate a recv_ctxt
 * @rdma: controlling svcxprt_rdma
 *
 * Returns a recv_ctxt or (rarely) NULL if none are available.
 */
struct svc_rdma_recv_ctxt *svc_rdma_recv_ctxt_get(struct svcxprt_rdma *rdma)
{
	struct svc_rdma_recv_ctxt *ctxt;
	struct llist_node *node;

	node = llist_del_first(&rdma->sc_recv_ctxts);
	if (!node)
		return NULL;

	ctxt = llist_entry(node, struct svc_rdma_recv_ctxt, rc_node);
	ctxt->rc_page_count = 0;
	return ctxt;
}

/**
 * svc_rdma_recv_ctxt_put - Return recv_ctxt to free list
 * @rdma: controlling svcxprt_rdma
 * @ctxt: object to return to the free list
 *
 */
void svc_rdma_recv_ctxt_put(struct svcxprt_rdma *rdma,
			    struct svc_rdma_recv_ctxt *ctxt)
{
	svc_rdma_cc_release(rdma, &ctxt->rc_cc, DMA_FROM_DEVICE);

	/* @rc_page_count is normally zero here, but error flows
	 * can leave pages in @rc_pages.
	 */
	release_pages(ctxt->rc_pages, ctxt->rc_page_count);

	pcl_free(&ctxt->rc_call_pcl);
	pcl_free(&ctxt->rc_read_pcl);
	pcl_free(&ctxt->rc_write_pcl);
	pcl_free(&ctxt->rc_reply_pcl);

	llist_add(&ctxt->rc_node, &rdma->sc_recv_ctxts);
}

/**
 * svc_rdma_release_ctxt - Release transport-specific per-rqst resources
 * @xprt: the transport which owned the context
 * @vctxt: the context from rqstp->rq_xprt_ctxt or dr->xprt_ctxt
 *
 * Ensure that the recv_ctxt is released whether or not a Reply
 * was sent. For example, the client could close the connection,
 * or svc_process could drop an RPC, before the Reply is sent.
 */
void svc_rdma_release_ctxt(struct svc_xprt *xprt, void *vctxt)
{
	struct svc_rdma_recv_ctxt *ctxt = vctxt;
	struct svcxprt_rdma *rdma =
		container_of(xprt, struct svcxprt_rdma, sc_xprt);

	if (ctxt)
		svc_rdma_recv_ctxt_put(rdma, ctxt);
}

static bool svc_rdma_refresh_recvs(struct svcxprt_rdma *rdma,
				   unsigned int wanted)
{
	const struct ib_recv_wr *bad_wr = NULL;
	struct svc_rdma_recv_ctxt *ctxt;
	struct ib_recv_wr *recv_chain;
	int ret;

	if (test_bit(XPT_CLOSE, &rdma->sc_xprt.xpt_flags))
		return false;

	recv_chain = NULL;
	while (wanted--) {
		ctxt = svc_rdma_recv_ctxt_get(rdma);
		if (!ctxt)
			break;

		trace_svcrdma_post_recv(&ctxt->rc_cid);
		ctxt->rc_recv_wr.next = recv_chain;
		recv_chain = &ctxt->rc_recv_wr;
		rdma->sc_pending_recvs++;
	}
	if (!recv_chain)
		return true;

	ret = ib_post_recv(rdma->sc_qp, recv_chain, &bad_wr);
	if (ret)
		goto err_free;
	return true;

err_free:
	trace_svcrdma_rq_post_err(rdma, ret);
	while (bad_wr) {
		ctxt = container_of(bad_wr, struct svc_rdma_recv_ctxt,
				    rc_recv_wr);
		bad_wr = bad_wr->next;
		svc_rdma_recv_ctxt_put(rdma, ctxt);
	}
	/* Since we're destroying the xprt, no need to reset
	 * sc_pending_recvs. */
	return false;
}

/**
 * svc_rdma_post_recvs - Post initial set of Recv WRs
 * @rdma: fresh svcxprt_rdma
 *
 * Return values:
 *   %true: Receive Queue initialization successful
 *   %false: memory allocation or DMA error
 */
bool svc_rdma_post_recvs(struct svcxprt_rdma *rdma)
{
	unsigned int total;

	/* For each credit, allocate enough recv_ctxts for one
	 * posted Receive and one RPC in process.
	 */
	total = (rdma->sc_max_requests * 2) + rdma->sc_recv_batch;
	while (total--) {
		struct svc_rdma_recv_ctxt *ctxt;

		ctxt = svc_rdma_recv_ctxt_alloc(rdma);
		if (!ctxt)
			return false;
		llist_add(&ctxt->rc_node, &rdma->sc_recv_ctxts);
	}

	return svc_rdma_refresh_recvs(rdma, rdma->sc_max_requests);
}

/**
 * svc_rdma_wc_receive - Invoked by RDMA provider for each polled Receive WC
 * @cq: Completion Queue context
 * @wc: Work Completion object
 *
 */
static void svc_rdma_wc_receive(struct ib_cq *cq, struct ib_wc *wc)
{
	struct svcxprt_rdma *rdma = cq->cq_context;
	struct ib_cqe *cqe = wc->wr_cqe;
	struct svc_rdma_recv_ctxt *ctxt;

	rdma->sc_pending_recvs--;

	/* WARNING: Only wc->wr_cqe and wc->status are reliable */
	ctxt = container_of(cqe, struct svc_rdma_recv_ctxt, rc_cqe);

	if (wc->status != IB_WC_SUCCESS)
		goto flushed;
	trace_svcrdma_wc_recv(wc, &ctxt->rc_cid);

	/* If receive posting fails, the connection is about to be
	 * lost anyway. The server will not be able to send a reply
	 * for this RPC, and the client will retransmit this RPC
	 * anyway when it reconnects.
	 *
	 * Therefore we drop the Receive, even if status was SUCCESS
	 * to reduce the likelihood of replayed requests once the
	 * client reconnects.
	 */
	if (rdma->sc_pending_recvs < rdma->sc_max_requests)
		if (!svc_rdma_refresh_recvs(rdma, rdma->sc_recv_batch))
			goto dropped;

	/* All wc fields are now known to be valid */
	ctxt->rc_byte_len = wc->byte_len;

	spin_lock(&rdma->sc_rq_dto_lock);
	list_add_tail(&ctxt->rc_list, &rdma->sc_rq_dto_q);
	/* Note the unlock pairs with the smp_rmb in svc_xprt_ready: */
	set_bit(XPT_DATA, &rdma->sc_xprt.xpt_flags);
	spin_unlock(&rdma->sc_rq_dto_lock);
	if (!test_bit(RDMAXPRT_CONN_PENDING, &rdma->sc_flags))
		svc_xprt_enqueue(&rdma->sc_xprt);
	return;

flushed:
	if (wc->status == IB_WC_WR_FLUSH_ERR)
		trace_svcrdma_wc_recv_flush(wc, &ctxt->rc_cid);
	else
		trace_svcrdma_wc_recv_err(wc, &ctxt->rc_cid);
dropped:
	svc_rdma_recv_ctxt_put(rdma, ctxt);
	svc_xprt_deferred_close(&rdma->sc_xprt);
}

/**
 * svc_rdma_flush_recv_queues - Drain pending Receive work
 * @rdma: svcxprt_rdma being shut down
 *
 */
void svc_rdma_flush_recv_queues(struct svcxprt_rdma *rdma)
{
	struct svc_rdma_recv_ctxt *ctxt;

	while ((ctxt = svc_rdma_next_recv_ctxt(&rdma->sc_read_complete_q))) {
		list_del(&ctxt->rc_list);
		svc_rdma_recv_ctxt_put(rdma, ctxt);
	}
	while ((ctxt = svc_rdma_next_recv_ctxt(&rdma->sc_rq_dto_q))) {
		list_del(&ctxt->rc_list);
		svc_rdma_recv_ctxt_put(rdma, ctxt);
	}
}

static void svc_rdma_build_arg_xdr(struct svc_rqst *rqstp,
				   struct svc_rdma_recv_ctxt *ctxt)
{
	struct xdr_buf *arg = &rqstp->rq_arg;

	arg->head[0].iov_base = ctxt->rc_recv_buf;
	arg->head[0].iov_len = ctxt->rc_byte_len;
	arg->tail[0].iov_base = NULL;
	arg->tail[0].iov_len = 0;
	arg->page_len = 0;
	arg->page_base = 0;
	arg->buflen = ctxt->rc_byte_len;
	arg->len = ctxt->rc_byte_len;
}

/**
 * xdr_count_read_segments - Count number of Read segments in Read list
 * @rctxt: Ingress receive context
 * @p: Start of an un-decoded Read list
 *
 * Before allocating anything, ensure the ingress Read list is safe
 * to use.
 *
 * The segment count is limited to how many segments can fit in the
 * transport header without overflowing the buffer. That's about 40
 * Read segments for a 1KB inline threshold.
 *
 * Return values:
 *   %true: Read list is valid. @rctxt's xdr_stream is updated to point
 *	    to the first byte past the Read list. rc_read_pcl and
 *	    rc_call_pcl cl_count fields are set to the number of
 *	    Read segments in the list.
 *  %false: Read list is corrupt. @rctxt's xdr_stream is left in an
 *	    unknown state.
 */
static bool xdr_count_read_segments(struct svc_rdma_recv_ctxt *rctxt, __be32 *p)
{
	rctxt->rc_call_pcl.cl_count = 0;
	rctxt->rc_read_pcl.cl_count = 0;
	while (xdr_item_is_present(p)) {
		u32 position, handle, length;
		u64 offset;

		p = xdr_inline_decode(&rctxt->rc_stream,
				      rpcrdma_readseg_maxsz * sizeof(*p));
		if (!p)
			return false;

		xdr_decode_read_segment(p, &position, &handle,
					    &length, &offset);
		if (position) {
			if (position & 3)
				return false;
			++rctxt->rc_read_pcl.cl_count;
		} else {
			++rctxt->rc_call_pcl.cl_count;
		}

		p = xdr_inline_decode(&rctxt->rc_stream, sizeof(*p));
		if (!p)
			return false;
	}
	return true;
}

/* Sanity check the Read list.
 *
 * Sanity checks:
 * - Read list does not overflow Receive buffer.
 * - Chunk size limited by largest NFS data payload.
 *
 * Return values:
 *   %true: Read list is valid. @rctxt's xdr_stream is updated
 *	    to point to the first byte past the Read list.
 *  %false: Read list is corrupt. @rctxt's xdr_stream is left
 *	    in an unknown state.
 */
static bool xdr_check_read_list(struct svc_rdma_recv_ctxt *rctxt)
{
	__be32 *p;

	p = xdr_inline_decode(&rctxt->rc_stream, sizeof(*p));
	if (!p)
		return false;
	if (!xdr_count_read_segments(rctxt, p))
		return false;
	if (!pcl_alloc_call(rctxt, p))
		return false;
	return pcl_alloc_read(rctxt, p);
}

static bool xdr_check_write_chunk(struct svc_rdma_recv_ctxt *rctxt)
{
	u32 segcount;
	__be32 *p;

	if (xdr_stream_decode_u32(&rctxt->rc_stream, &segcount))
		return false;

	/* A bogus segcount causes this buffer overflow check to fail. */
	p = xdr_inline_decode(&rctxt->rc_stream,
			      segcount * rpcrdma_segment_maxsz * sizeof(*p));
	return p != NULL;
}

/**
 * xdr_count_write_chunks - Count number of Write chunks in Write list
 * @rctxt: Received header and decoding state
 * @p: start of an un-decoded Write list
 *
 * Before allocating anything, ensure the ingress Write list is
 * safe to use.
 *
 * Return values:
 *       %true: Write list is valid. @rctxt's xdr_stream is updated
 *		to point to the first byte past the Write list, and
 *		the number of Write chunks is in rc_write_pcl.cl_count.
 *      %false: Write list is corrupt. @rctxt's xdr_stream is left
 *		in an indeterminate state.
 */
static bool xdr_count_write_chunks(struct svc_rdma_recv_ctxt *rctxt, __be32 *p)
{
	rctxt->rc_write_pcl.cl_count = 0;
	while (xdr_item_is_present(p)) {
		if (!xdr_check_write_chunk(rctxt))
			return false;
		++rctxt->rc_write_pcl.cl_count;
		p = xdr_inline_decode(&rctxt->rc_stream, sizeof(*p));
		if (!p)
			return false;
	}
	return true;
}

/* Sanity check the Write list.
 *
 * Implementation limits:
 * - This implementation currently supports only one Write chunk.
 *
 * Sanity checks:
 * - Write list does not overflow Receive buffer.
 * - Chunk size limited by largest NFS data payload.
 *
 * Return values:
 *       %true: Write list is valid. @rctxt's xdr_stream is updated
 *		to point to the first byte past the Write list.
 *      %false: Write list is corrupt. @rctxt's xdr_stream is left
 *		in an unknown state.
 */
static bool xdr_check_write_list(struct svc_rdma_recv_ctxt *rctxt)
{
	__be32 *p;

	p = xdr_inline_decode(&rctxt->rc_stream, sizeof(*p));
	if (!p)
		return false;
	if (!xdr_count_write_chunks(rctxt, p))
		return false;
	if (!pcl_alloc_write(rctxt, &rctxt->rc_write_pcl, p))
		return false;

	rctxt->rc_cur_result_payload = pcl_first_chunk(&rctxt->rc_write_pcl);
	return true;
}

/* Sanity check the Reply chunk.
 *
 * Sanity checks:
 * - Reply chunk does not overflow Receive buffer.
 * - Chunk size limited by largest NFS data payload.
 *
 * Return values:
 *       %true: Reply chunk is valid. @rctxt's xdr_stream is updated
 *		to point to the first byte past the Reply chunk.
 *      %false: Reply chunk is corrupt. @rctxt's xdr_stream is left
 *		in an unknown state.
 */
static bool xdr_check_reply_chunk(struct svc_rdma_recv_ctxt *rctxt)
{
	__be32 *p;

	p = xdr_inline_decode(&rctxt->rc_stream, sizeof(*p));
	if (!p)
		return false;

	if (!xdr_item_is_present(p))
		return true;
	if (!xdr_check_write_chunk(rctxt))
		return false;

	rctxt->rc_reply_pcl.cl_count = 1;
	return pcl_alloc_write(rctxt, &rctxt->rc_reply_pcl, p);
}

/* RPC-over-RDMA Version One private extension: Remote Invalidation.
 * Responder's choice: requester signals it can handle Send With
 * Invalidate, and responder chooses one R_key to invalidate.
 *
 * If there is exactly one distinct R_key in the received transport
 * header, set rc_inv_rkey to that R_key. Otherwise, set it to zero.
 */
static void svc_rdma_get_inv_rkey(struct svcxprt_rdma *rdma,
				  struct svc_rdma_recv_ctxt *ctxt)
{
	struct svc_rdma_segment *segment;
	struct svc_rdma_chunk *chunk;
	u32 inv_rkey;

	ctxt->rc_inv_rkey = 0;

	if (!rdma->sc_snd_w_inv)
		return;

	inv_rkey = 0;
	pcl_for_each_chunk(chunk, &ctxt->rc_call_pcl) {
		pcl_for_each_segment(segment, chunk) {
			if (inv_rkey == 0)
				inv_rkey = segment->rs_handle;
			else if (inv_rkey != segment->rs_handle)
				return;
		}
	}
	pcl_for_each_chunk(chunk, &ctxt->rc_read_pcl) {
		pcl_for_each_segment(segment, chunk) {
			if (inv_rkey == 0)
				inv_rkey = segment->rs_handle;
			else if (inv_rkey != segment->rs_handle)
				return;
		}
	}
	pcl_for_each_chunk(chunk, &ctxt->rc_write_pcl) {
		pcl_for_each_segment(segment, chunk) {
			if (inv_rkey == 0)
				inv_rkey = segment->rs_handle;
			else if (inv_rkey != segment->rs_handle)
				return;
		}
	}
	pcl_for_each_chunk(chunk, &ctxt->rc_reply_pcl) {
		pcl_for_each_segment(segment, chunk) {
			if (inv_rkey == 0)
				inv_rkey = segment->rs_handle;
			else if (inv_rkey != segment->rs_handle)
				return;
		}
	}
	ctxt->rc_inv_rkey = inv_rkey;
}

/**
 * svc_rdma_xdr_decode_req - Decode the transport header
 * @rq_arg: xdr_buf containing ingress RPC/RDMA message
 * @rctxt: state of decoding
 *
 * On entry, xdr->head[0].iov_base points to first byte of the
 * RPC-over-RDMA transport header.
 *
 * On successful exit, head[0] points to first byte past the
 * RPC-over-RDMA header. For RDMA_MSG, this is the RPC message.
 *
 * The length of the RPC-over-RDMA header is returned.
 *
 * Assumptions:
 * - The transport header is entirely contained in the head iovec.
 */
static int svc_rdma_xdr_decode_req(struct xdr_buf *rq_arg,
				   struct svc_rdma_recv_ctxt *rctxt)
{
	__be32 *p, *rdma_argp;
	unsigned int hdr_len;

	rdma_argp = rq_arg->head[0].iov_base;
	xdr_init_decode(&rctxt->rc_stream, rq_arg, rdma_argp, NULL);

	p = xdr_inline_decode(&rctxt->rc_stream,
			      rpcrdma_fixed_maxsz * sizeof(*p));
	if (unlikely(!p))
		goto out_short;
	p++;
	if (*p != rpcrdma_version)
		goto out_version;
	p += 2;
	rctxt->rc_msgtype = *p;
	switch (rctxt->rc_msgtype) {
	case rdma_msg:
		break;
	case rdma_nomsg:
		break;
	case rdma_done:
		goto out_drop;
	case rdma_error:
		goto out_drop;
	default:
		goto out_proc;
	}

	if (!xdr_check_read_list(rctxt))
		goto out_inval;
	if (!xdr_check_write_list(rctxt))
		goto out_inval;
	if (!xdr_check_reply_chunk(rctxt))
		goto out_inval;

	rq_arg->head[0].iov_base = rctxt->rc_stream.p;
	hdr_len = xdr_stream_pos(&rctxt->rc_stream);
	rq_arg->head[0].iov_len -= hdr_len;
	rq_arg->len -= hdr_len;
	trace_svcrdma_decode_rqst(rctxt, rdma_argp, hdr_len);
	return hdr_len;

out_short:
	trace_svcrdma_decode_short_err(rctxt, rq_arg->len);
	return -EINVAL;

out_version:
	trace_svcrdma_decode_badvers_err(rctxt, rdma_argp);
	return -EPROTONOSUPPORT;

out_drop:
	trace_svcrdma_decode_drop_err(rctxt, rdma_argp);
	return 0;

out_proc:
	trace_svcrdma_decode_badproc_err(rctxt, rdma_argp);
	return -EINVAL;

out_inval:
	trace_svcrdma_decode_parse_err(rctxt, rdma_argp);
	return -EINVAL;
}

static void svc_rdma_send_error(struct svcxprt_rdma *rdma,
				struct svc_rdma_recv_ctxt *rctxt,
				int status)
{
	struct svc_rdma_send_ctxt *sctxt;

	sctxt = svc_rdma_send_ctxt_get(rdma);
	if (!sctxt)
		return;
	svc_rdma_send_error_msg(rdma, sctxt, rctxt, status);
}

/* By convention, backchannel calls arrive via rdma_msg type
 * messages, and never populate the chunk lists. This makes
 * the RPC/RDMA header small and fixed in size, so it is
 * straightforward to check the RPC header's direction field.
 */
static bool svc_rdma_is_reverse_direction_reply(struct svc_xprt *xprt,
						struct svc_rdma_recv_ctxt *rctxt)
{
	__be32 *p = rctxt->rc_recv_buf;

	if (!xprt->xpt_bc_xprt)
		return false;

	if (rctxt->rc_msgtype != rdma_msg)
		return false;

	if (!pcl_is_empty(&rctxt->rc_call_pcl))
		return false;
	if (!pcl_is_empty(&rctxt->rc_read_pcl))
		return false;
	if (!pcl_is_empty(&rctxt->rc_write_pcl))
		return false;
	if (!pcl_is_empty(&rctxt->rc_reply_pcl))
		return false;

	/* RPC call direction */
	if (*(p + 8) == cpu_to_be32(RPC_CALL))
		return false;

	return true;
}

/* Finish constructing the RPC Call message in rqstp::rq_arg.
 *
 * The incoming RPC/RDMA message is an RDMA_MSG type message
 * with a single Read chunk (only the upper layer data payload
 * was conveyed via RDMA Read).
 */
static void svc_rdma_read_complete_one(struct svc_rqst *rqstp,
				       struct svc_rdma_recv_ctxt *ctxt)
{
	struct svc_rdma_chunk *chunk = pcl_first_chunk(&ctxt->rc_read_pcl);
	struct xdr_buf *buf = &rqstp->rq_arg;
	unsigned int length;

	/* Split the Receive buffer between the head and tail
	 * buffers at Read chunk's position. XDR roundup of the
	 * chunk is not included in either the pagelist or in
	 * the tail.
	 */
	buf->tail[0].iov_base = buf->head[0].iov_base + chunk->ch_position;
	buf->tail[0].iov_len = buf->head[0].iov_len - chunk->ch_position;
	buf->head[0].iov_len = chunk->ch_position;

	/* Read chunk may need XDR roundup (see RFC 8166, s. 3.4.5.2).
	 *
	 * If the client already rounded up the chunk length, the
	 * length does not change. Otherwise, the length of the page
	 * list is increased to include XDR round-up.
	 *
	 * Currently these chunks always start at page offset 0,
	 * thus the rounded-up length never crosses a page boundary.
	 */
	buf->pages = &rqstp->rq_pages[0];
	length = xdr_align_size(chunk->ch_length);
	buf->page_len = length;
	buf->len += length;
	buf->buflen += length;
}

/* Finish constructing the RPC Call message in rqstp::rq_arg.
 *
 * The incoming RPC/RDMA message is an RDMA_MSG type message
 * with payload in multiple Read chunks and no PZRC.
 */
static void svc_rdma_read_complete_multiple(struct svc_rqst *rqstp,
					    struct svc_rdma_recv_ctxt *ctxt)
{
	struct xdr_buf *buf = &rqstp->rq_arg;

	buf->len += ctxt->rc_readbytes;
	buf->buflen += ctxt->rc_readbytes;

	buf->head[0].iov_base = page_address(rqstp->rq_pages[0]);
	buf->head[0].iov_len = min_t(size_t, PAGE_SIZE, ctxt->rc_readbytes);
	buf->pages = &rqstp->rq_pages[1];
	buf->page_len = ctxt->rc_readbytes - buf->head[0].iov_len;
}

/* Finish constructing the RPC Call message in rqstp::rq_arg.
 *
 * The incoming RPC/RDMA message is an RDMA_NOMSG type message
 * (the RPC message body was conveyed via RDMA Read).
 */
static void svc_rdma_read_complete_pzrc(struct svc_rqst *rqstp,
					struct svc_rdma_recv_ctxt *ctxt)
{
	struct xdr_buf *buf = &rqstp->rq_arg;

	buf->len += ctxt->rc_readbytes;
	buf->buflen += ctxt->rc_readbytes;

	buf->head[0].iov_base = page_address(rqstp->rq_pages[0]);
	buf->head[0].iov_len = min_t(size_t, PAGE_SIZE, ctxt->rc_readbytes);
	buf->pages = &rqstp->rq_pages[1];
	buf->page_len = ctxt->rc_readbytes - buf->head[0].iov_len;
}

static noinline void svc_rdma_read_complete(struct svc_rqst *rqstp,
					    struct svc_rdma_recv_ctxt *ctxt)
{
	unsigned int i;

	/* Transfer the Read chunk pages into @rqstp.rq_pages, replacing
	 * the rq_pages that were already allocated for this rqstp.
	 */
	release_pages(rqstp->rq_respages, ctxt->rc_page_count);
	for (i = 0; i < ctxt->rc_page_count; i++)
		rqstp->rq_pages[i] = ctxt->rc_pages[i];

	/* Update @rqstp's result send buffer to start after the
	 * last page in the RDMA Read payload.
	 */
	rqstp->rq_respages = &rqstp->rq_pages[ctxt->rc_page_count];
	rqstp->rq_next_page = rqstp->rq_respages + 1;

	/* Prevent svc_rdma_recv_ctxt_put() from releasing the
	 * pages in ctxt::rc_pages a second time.
	 */
	ctxt->rc_page_count = 0;

	/* Finish constructing the RPC Call message. The exact
	 * procedure for that depends on what kind of RPC/RDMA
	 * chunks were provided by the client.
	 */
	rqstp->rq_arg = ctxt->rc_saved_arg;
	if (pcl_is_empty(&ctxt->rc_call_pcl)) {
		if (ctxt->rc_read_pcl.cl_count == 1)
			svc_rdma_read_complete_one(rqstp, ctxt);
		else
			svc_rdma_read_complete_multiple(rqstp, ctxt);
	} else {
		svc_rdma_read_complete_pzrc(rqstp, ctxt);
	}

	trace_svcrdma_read_finished(&ctxt->rc_cid);
}

/**
 * svc_rdma_recvfrom - Receive an RPC call
 * @rqstp: request structure into which to receive an RPC Call
 *
 * Returns:
 *	The positive number of bytes in the RPC Call message,
 *	%0 if there were no Calls ready to return,
 *	%-EINVAL if the Read chunk data is too large,
 *	%-ENOMEM if rdma_rw context pool was exhausted,
 *	%-ENOTCONN if posting failed (connection is lost),
 *	%-EIO if rdma_rw initialization failed (DMA mapping, etc).
 *
 * Called in a loop when XPT_DATA is set. XPT_DATA is cleared only
 * when there are no remaining ctxt's to process.
 *
 * The next ctxt is removed from the "receive" lists.
 *
 * - If the ctxt completes a Receive, then construct the Call
 *   message from the contents of the Receive buffer.
 *
 *   - If there are no Read chunks in this message, then finish
 *     assembling the Call message and return the number of bytes
 *     in the message.
 *
 *   - If there are Read chunks in this message, post Read WRs to
 *     pull that payload. When the Read WRs complete, build the
 *     full message and return the number of bytes in it.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
int svc_rdma_recvfrom(struct svc_rqst *rqstp)
{
	struct svc_xprt *xprt = rqstp->rq_xprt;
	struct svcxprt_rdma *rdma_xprt =
		container_of(xprt, struct svcxprt_rdma, sc_xprt);
<<<<<<< HEAD
	struct svc_rdma_op_ctxt *ctxt = NULL;
	struct rpcrdma_msg *rmsgp;
	int ret = 0;
	int len;

	dprintk("svcrdma: rqstp=%p\n", rqstp);

	spin_lock_bh(&rdma_xprt->sc_rq_dto_lock);
	if (!list_empty(&rdma_xprt->sc_read_complete_q)) {
		ctxt = list_entry(rdma_xprt->sc_read_complete_q.next,
				  struct svc_rdma_op_ctxt,
				  dto_q);
		list_del_init(&ctxt->dto_q);
	}
	if (ctxt) {
		spin_unlock_bh(&rdma_xprt->sc_rq_dto_lock);
		return rdma_read_complete(rqstp, ctxt);
	}

	if (!list_empty(&rdma_xprt->sc_rq_dto_q)) {
		ctxt = list_entry(rdma_xprt->sc_rq_dto_q.next,
				  struct svc_rdma_op_ctxt,
				  dto_q);
		list_del_init(&ctxt->dto_q);
	} else {
		atomic_inc(&rdma_stat_rq_starve);
		clear_bit(XPT_DATA, &xprt->xpt_flags);
		ctxt = NULL;
	}
	spin_unlock_bh(&rdma_xprt->sc_rq_dto_lock);
	if (!ctxt) {
		/* This is the EAGAIN path. The svc_recv routine will
		 * return -EAGAIN, the nfsd thread will go to call into
		 * svc_recv again and we shouldn't be on the active
		 * transport list
		 */
		if (test_bit(XPT_CLOSE, &xprt->xpt_flags))
			goto close_out;

		BUG_ON(ret);
		goto out;
	}
	dprintk("svcrdma: processing ctxt=%p on xprt=%p, rqstp=%p, status=%d\n",
		ctxt, rdma_xprt, rqstp, ctxt->wc_status);
	BUG_ON(ctxt->wc_status != IB_WC_SUCCESS);
	atomic_inc(&rdma_stat_recv);

	/* Build up the XDR from the receive buffers. */
	rdma_build_arg_xdr(rqstp, ctxt, ctxt->byte_len);

	/* Decode the RDMA header. */
	len = svc_rdma_xdr_decode_req(&rmsgp, rqstp);
	rqstp->rq_xprt_hlen = len;

	/* If the request is invalid, reply with an error */
	if (len < 0) {
		if (len == -ENOSYS)
			svc_rdma_send_error(rdma_xprt, rmsgp, ERR_VERS);
		goto close_out;
	}

	/* Read read-list data. */
	ret = rdma_read_xdr(rdma_xprt, rmsgp, rqstp, ctxt);
	if (ret > 0) {
		/* read-list posted, defer until data received from client. */
		goto defer;
	}
	if (ret < 0) {
		/* Post of read-list failed, free context. */
		svc_rdma_put_context(ctxt, 1);
		return 0;
	}

	ret = rqstp->rq_arg.head[0].iov_len
		+ rqstp->rq_arg.page_len
		+ rqstp->rq_arg.tail[0].iov_len;
	svc_rdma_put_context(ctxt, 0);
 out:
	dprintk("svcrdma: ret = %d, rq_arg.len =%d, "
		"rq_arg.head[0].iov_base=%p, rq_arg.head[0].iov_len = %zd\n",
		ret, rqstp->rq_arg.len,
		rqstp->rq_arg.head[0].iov_base,
		rqstp->rq_arg.head[0].iov_len);
	rqstp->rq_prot = IPPROTO_MAX;
	svc_xprt_copy_addrs(rqstp, xprt);
	return ret;

 close_out:
	if (ctxt)
		svc_rdma_put_context(ctxt, 1);
	dprintk("svcrdma: transport %p is closing\n", xprt);
	/*
	 * Set the close bit and enqueue it. svc_recv will see the
	 * close bit and call svc_xprt_delete
	 */
	set_bit(XPT_CLOSE, &xprt->xpt_flags);
defer:
=======
	struct svc_rdma_recv_ctxt *ctxt;
	int ret;

	/* Prevent svc_xprt_release() from releasing pages in rq_pages
	 * when returning 0 or an error.
	 */
	rqstp->rq_respages = rqstp->rq_pages;
	rqstp->rq_next_page = rqstp->rq_respages;

	rqstp->rq_xprt_ctxt = NULL;

	spin_lock(&rdma_xprt->sc_rq_dto_lock);
	ctxt = svc_rdma_next_recv_ctxt(&rdma_xprt->sc_read_complete_q);
	if (ctxt) {
		list_del(&ctxt->rc_list);
		spin_unlock(&rdma_xprt->sc_rq_dto_lock);
		svc_xprt_received(xprt);
		svc_rdma_read_complete(rqstp, ctxt);
		goto complete;
	}
	ctxt = svc_rdma_next_recv_ctxt(&rdma_xprt->sc_rq_dto_q);
	if (ctxt)
		list_del(&ctxt->rc_list);
	else
		/* No new incoming requests, terminate the loop */
		clear_bit(XPT_DATA, &xprt->xpt_flags);
	spin_unlock(&rdma_xprt->sc_rq_dto_lock);

	/* Unblock the transport for the next receive */
	svc_xprt_received(xprt);
	if (!ctxt)
		return 0;

	percpu_counter_inc(&svcrdma_stat_recv);
	ib_dma_sync_single_for_cpu(rdma_xprt->sc_pd->device,
				   ctxt->rc_recv_sge.addr, ctxt->rc_byte_len,
				   DMA_FROM_DEVICE);
	svc_rdma_build_arg_xdr(rqstp, ctxt);

	ret = svc_rdma_xdr_decode_req(&rqstp->rq_arg, ctxt);
	if (ret < 0)
		goto out_err;
	if (ret == 0)
		goto out_drop;

	if (svc_rdma_is_reverse_direction_reply(xprt, ctxt))
		goto out_backchannel;

	svc_rdma_get_inv_rkey(rdma_xprt, ctxt);

	if (!pcl_is_empty(&ctxt->rc_read_pcl) ||
	    !pcl_is_empty(&ctxt->rc_call_pcl))
		goto out_readlist;

complete:
	rqstp->rq_xprt_ctxt = ctxt;
	rqstp->rq_prot = IPPROTO_MAX;
	svc_xprt_copy_addrs(rqstp, xprt);
	set_bit(RQ_SECURE, &rqstp->rq_flags);
	return rqstp->rq_arg.len;

out_err:
	svc_rdma_send_error(rdma_xprt, ctxt, ret);
	svc_rdma_recv_ctxt_put(rdma_xprt, ctxt);
	return 0;

out_readlist:
	/* This @rqstp is about to be recycled. Save the work
	 * already done constructing the Call message in rq_arg
	 * so it can be restored when the RDMA Reads have
	 * completed.
	 */
	ctxt->rc_saved_arg = rqstp->rq_arg;

	ret = svc_rdma_process_read_list(rdma_xprt, rqstp, ctxt);
	if (ret < 0) {
		if (ret == -EINVAL)
			svc_rdma_send_error(rdma_xprt, ctxt, ret);
		svc_rdma_recv_ctxt_put(rdma_xprt, ctxt);
		svc_xprt_deferred_close(xprt);
		return ret;
	}
	return 0;

out_backchannel:
	svc_rdma_handle_bc_reply(rqstp, ctxt);
out_drop:
	svc_rdma_recv_ctxt_put(rdma_xprt, ctxt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}
