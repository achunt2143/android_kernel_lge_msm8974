<<<<<<< HEAD
/*
=======
// SPDX-License-Identifier: GPL-2.0 OR BSD-3-Clause
/*
 * Copyright (c) 2014-2017 Oracle.  All rights reserved.
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
 * verbs.c
 *
 * Encapsulates the major functions managing:
 *  o adapters
 *  o endpoints
 *  o connections
 *  o buffer memory
 */

#include <linux/interrupt.h>
<<<<<<< HEAD
#include <linux/pci.h>	/* for Tavor hack below */
#include <linux/slab.h>

#include "xprt_rdma.h"

/*
 * Globals/Macros
 */

#ifdef RPC_DEBUG
# define RPCDBG_FACILITY	RPCDBG_TRANS
#endif

/*
 * internal functions
 */

/*
 * handle replies in tasklet context, using a single, global list
 * rdma tasklet function -- just turn around and call the func
 * for all replies on the list
 */

static DEFINE_SPINLOCK(rpcrdma_tk_lock_g);
static LIST_HEAD(rpcrdma_tasklets_g);

static void
rpcrdma_run_tasklet(unsigned long data)
{
	struct rpcrdma_rep *rep;
	void (*func)(struct rpcrdma_rep *);
	unsigned long flags;

	data = data;
	spin_lock_irqsave(&rpcrdma_tk_lock_g, flags);
	while (!list_empty(&rpcrdma_tasklets_g)) {
		rep = list_entry(rpcrdma_tasklets_g.next,
				 struct rpcrdma_rep, rr_list);
		list_del(&rep->rr_list);
		func = rep->rr_func;
		rep->rr_func = NULL;
		spin_unlock_irqrestore(&rpcrdma_tk_lock_g, flags);

		if (func)
			func(rep);
		else
			rpcrdma_recv_buffer_put(rep);

		spin_lock_irqsave(&rpcrdma_tk_lock_g, flags);
	}
	spin_unlock_irqrestore(&rpcrdma_tk_lock_g, flags);
}

static DECLARE_TASKLET(rpcrdma_tasklet_g, rpcrdma_run_tasklet, 0UL);

static inline void
rpcrdma_schedule_tasklet(struct rpcrdma_rep *rep)
{
	unsigned long flags;

	spin_lock_irqsave(&rpcrdma_tk_lock_g, flags);
	list_add_tail(&rep->rr_list, &rpcrdma_tasklets_g);
	spin_unlock_irqrestore(&rpcrdma_tk_lock_g, flags);
	tasklet_schedule(&rpcrdma_tasklet_g);
}

static void
rpcrdma_qp_async_error_upcall(struct ib_event *event, void *context)
{
	struct rpcrdma_ep *ep = context;

	dprintk("RPC:       %s: QP error %X on device %s ep %p\n",
		__func__, event->event, event->device->name, context);
	if (ep->rep_connected == 1) {
		ep->rep_connected = -EIO;
		ep->rep_func(ep);
		wake_up_all(&ep->rep_connect_wait);
	}
}

static void
rpcrdma_cq_async_error_upcall(struct ib_event *event, void *context)
{
	struct rpcrdma_ep *ep = context;

	dprintk("RPC:       %s: CQ error %X on device %s ep %p\n",
		__func__, event->event, event->device->name, context);
	if (ep->rep_connected == 1) {
		ep->rep_connected = -EIO;
		ep->rep_func(ep);
		wake_up_all(&ep->rep_connect_wait);
	}
}

static inline
void rpcrdma_event_process(struct ib_wc *wc)
{
	struct rpcrdma_mw *frmr;
	struct rpcrdma_rep *rep =
			(struct rpcrdma_rep *)(unsigned long) wc->wr_id;

	dprintk("RPC:       %s: event rep %p status %X opcode %X length %u\n",
		__func__, rep, wc->status, wc->opcode, wc->byte_len);

	if (!rep) /* send or bind completion that we don't care about */
		return;

	if (IB_WC_SUCCESS != wc->status) {
		dprintk("RPC:       %s: WC opcode %d status %X, connection lost\n",
			__func__, wc->opcode, wc->status);
		rep->rr_len = ~0U;
		if (wc->opcode != IB_WC_FAST_REG_MR && wc->opcode != IB_WC_LOCAL_INV)
			rpcrdma_schedule_tasklet(rep);
		return;
	}

	switch (wc->opcode) {
	case IB_WC_FAST_REG_MR:
		frmr = (struct rpcrdma_mw *)(unsigned long)wc->wr_id;
		frmr->r.frmr.state = FRMR_IS_VALID;
		break;
	case IB_WC_LOCAL_INV:
		frmr = (struct rpcrdma_mw *)(unsigned long)wc->wr_id;
		frmr->r.frmr.state = FRMR_IS_INVALID;
		break;
	case IB_WC_RECV:
		rep->rr_len = wc->byte_len;
		ib_dma_sync_single_for_cpu(
			rdmab_to_ia(rep->rr_buffer)->ri_id->device,
			rep->rr_iov.addr, rep->rr_len, DMA_FROM_DEVICE);
		/* Keep (only) the most recent credits, after check validity */
		if (rep->rr_len >= 16) {
			struct rpcrdma_msg *p =
					(struct rpcrdma_msg *) rep->rr_base;
			unsigned int credits = ntohl(p->rm_credit);
			if (credits == 0) {
				dprintk("RPC:       %s: server"
					" dropped credits to 0!\n", __func__);
				/* don't deadlock */
				credits = 1;
			} else if (credits > rep->rr_buffer->rb_max_requests) {
				dprintk("RPC:       %s: server"
					" over-crediting: %d (%d)\n",
					__func__, credits,
					rep->rr_buffer->rb_max_requests);
				credits = rep->rr_buffer->rb_max_requests;
			}
			atomic_set(&rep->rr_buffer->rb_credits, credits);
		}
		/* fall through */
	case IB_WC_BIND_MW:
		rpcrdma_schedule_tasklet(rep);
		break;
	default:
		dprintk("RPC:       %s: unexpected WC event %X\n",
			__func__, wc->opcode);
		break;
	}
}

static inline int
rpcrdma_cq_poll(struct ib_cq *cq)
{
	struct ib_wc wc;
	int rc;

	for (;;) {
		rc = ib_poll_cq(cq, 1, &wc);
		if (rc < 0) {
			dprintk("RPC:       %s: ib_poll_cq failed %i\n",
				__func__, rc);
			return rc;
		}
		if (rc == 0)
			break;

		rpcrdma_event_process(&wc);
	}

	return 0;
}

/*
 * rpcrdma_cq_event_upcall
 *
 * This upcall handles recv, send, bind and unbind events.
 * It is reentrant but processes single events in order to maintain
 * ordering of receives to keep server credits.
 *
 * It is the responsibility of the scheduled tasklet to return
 * recv buffers to the pool. NOTE: this affects synchronization of
 * connection shutdown. That is, the structures required for
 * the completion of the reply handler must remain intact until
 * all memory has been reclaimed.
 *
 * Note that send events are suppressed and do not result in an upcall.
 */
static void
rpcrdma_cq_event_upcall(struct ib_cq *cq, void *context)
{
	int rc;

	rc = rpcrdma_cq_poll(cq);
	if (rc)
		return;

	rc = ib_req_notify_cq(cq, IB_CQ_NEXT_COMP);
	if (rc) {
		dprintk("RPC:       %s: ib_req_notify_cq failed %i\n",
			__func__, rc);
		return;
	}

	rpcrdma_cq_poll(cq);
}

#ifdef RPC_DEBUG
static const char * const conn[] = {
	"address resolved",
	"address error",
	"route resolved",
	"route error",
	"connect request",
	"connect response",
	"connect error",
	"unreachable",
	"rejected",
	"established",
	"disconnected",
	"device removal"
};
#endif

static int
rpcrdma_conn_upcall(struct rdma_cm_id *id, struct rdma_cm_event *event)
{
	struct rpcrdma_xprt *xprt = id->context;
	struct rpcrdma_ia *ia = &xprt->rx_ia;
	struct rpcrdma_ep *ep = &xprt->rx_ep;
#ifdef RPC_DEBUG
	struct sockaddr_in *addr = (struct sockaddr_in *) &ep->rep_remote_addr;
#endif
	struct ib_qp_attr attr;
	struct ib_qp_init_attr iattr;
	int connstate = 0;
=======
#include <linux/slab.h>
#include <linux/sunrpc/addr.h>
#include <linux/sunrpc/svc_rdma.h>
#include <linux/log2.h>

#include <asm-generic/barrier.h>
#include <asm/bitops.h>

#include <rdma/ib_cm.h>

#include "xprt_rdma.h"
#include <trace/events/rpcrdma.h>

static int rpcrdma_sendctxs_create(struct rpcrdma_xprt *r_xprt);
static void rpcrdma_sendctxs_destroy(struct rpcrdma_xprt *r_xprt);
static void rpcrdma_sendctx_put_locked(struct rpcrdma_xprt *r_xprt,
				       struct rpcrdma_sendctx *sc);
static int rpcrdma_reqs_setup(struct rpcrdma_xprt *r_xprt);
static void rpcrdma_reqs_reset(struct rpcrdma_xprt *r_xprt);
static void rpcrdma_rep_destroy(struct rpcrdma_rep *rep);
static void rpcrdma_reps_unmap(struct rpcrdma_xprt *r_xprt);
static void rpcrdma_mrs_create(struct rpcrdma_xprt *r_xprt);
static void rpcrdma_mrs_destroy(struct rpcrdma_xprt *r_xprt);
static void rpcrdma_ep_get(struct rpcrdma_ep *ep);
static int rpcrdma_ep_put(struct rpcrdma_ep *ep);
static struct rpcrdma_regbuf *
rpcrdma_regbuf_alloc(size_t size, enum dma_data_direction direction);
static void rpcrdma_regbuf_dma_unmap(struct rpcrdma_regbuf *rb);
static void rpcrdma_regbuf_free(struct rpcrdma_regbuf *rb);

/* Wait for outstanding transport work to finish. ib_drain_qp
 * handles the drains in the wrong order for us, so open code
 * them here.
 */
static void rpcrdma_xprt_drain(struct rpcrdma_xprt *r_xprt)
{
	struct rpcrdma_ep *ep = r_xprt->rx_ep;
	struct rdma_cm_id *id = ep->re_id;

	/* Wait for rpcrdma_post_recvs() to leave its critical
	 * section.
	 */
	if (atomic_inc_return(&ep->re_receiving) > 1)
		wait_for_completion(&ep->re_done);

	/* Flush Receives, then wait for deferred Reply work
	 * to complete.
	 */
	ib_drain_rq(id->qp);

	/* Deferred Reply processing might have scheduled
	 * local invalidations.
	 */
	ib_drain_sq(id->qp);

	rpcrdma_ep_put(ep);
}

/* Ensure xprt_force_disconnect() is invoked exactly once when a
 * connection is closed or lost. (The important thing is it needs
 * to be invoked "at least" once).
 */
void rpcrdma_force_disconnect(struct rpcrdma_ep *ep)
{
	if (atomic_add_unless(&ep->re_force_disconnect, 1, 1))
		xprt_force_disconnect(ep->re_xprt);
}

/**
 * rpcrdma_flush_disconnect - Disconnect on flushed completion
 * @r_xprt: transport to disconnect
 * @wc: work completion entry
 *
 * Must be called in process context.
 */
void rpcrdma_flush_disconnect(struct rpcrdma_xprt *r_xprt, struct ib_wc *wc)
{
	if (wc->status != IB_WC_SUCCESS)
		rpcrdma_force_disconnect(r_xprt->rx_ep);
}

/**
 * rpcrdma_wc_send - Invoked by RDMA provider for each polled Send WC
 * @cq:	completion queue
 * @wc:	WCE for a completed Send WR
 *
 */
static void rpcrdma_wc_send(struct ib_cq *cq, struct ib_wc *wc)
{
	struct ib_cqe *cqe = wc->wr_cqe;
	struct rpcrdma_sendctx *sc =
		container_of(cqe, struct rpcrdma_sendctx, sc_cqe);
	struct rpcrdma_xprt *r_xprt = cq->cq_context;

	/* WARNING: Only wr_cqe and status are reliable at this point */
	trace_xprtrdma_wc_send(wc, &sc->sc_cid);
	rpcrdma_sendctx_put_locked(r_xprt, sc);
	rpcrdma_flush_disconnect(r_xprt, wc);
}

/**
 * rpcrdma_wc_receive - Invoked by RDMA provider for each polled Receive WC
 * @cq:	completion queue
 * @wc:	WCE for a completed Receive WR
 *
 */
static void rpcrdma_wc_receive(struct ib_cq *cq, struct ib_wc *wc)
{
	struct ib_cqe *cqe = wc->wr_cqe;
	struct rpcrdma_rep *rep = container_of(cqe, struct rpcrdma_rep,
					       rr_cqe);
	struct rpcrdma_xprt *r_xprt = cq->cq_context;

	/* WARNING: Only wr_cqe and status are reliable at this point */
	trace_xprtrdma_wc_receive(wc, &rep->rr_cid);
	--r_xprt->rx_ep->re_receive_count;
	if (wc->status != IB_WC_SUCCESS)
		goto out_flushed;

	/* status == SUCCESS means all fields in wc are trustworthy */
	rpcrdma_set_xdrlen(&rep->rr_hdrbuf, wc->byte_len);
	rep->rr_wc_flags = wc->wc_flags;
	rep->rr_inv_rkey = wc->ex.invalidate_rkey;

	ib_dma_sync_single_for_cpu(rdmab_device(rep->rr_rdmabuf),
				   rdmab_addr(rep->rr_rdmabuf),
				   wc->byte_len, DMA_FROM_DEVICE);

	rpcrdma_reply_handler(rep);
	return;

out_flushed:
	rpcrdma_flush_disconnect(r_xprt, wc);
	rpcrdma_rep_put(&r_xprt->rx_buf, rep);
}

static void rpcrdma_update_cm_private(struct rpcrdma_ep *ep,
				      struct rdma_conn_param *param)
{
	const struct rpcrdma_connect_private *pmsg = param->private_data;
	unsigned int rsize, wsize;

	/* Default settings for RPC-over-RDMA Version One */
	rsize = RPCRDMA_V1_DEF_INLINE_SIZE;
	wsize = RPCRDMA_V1_DEF_INLINE_SIZE;

	if (pmsg &&
	    pmsg->cp_magic == rpcrdma_cmp_magic &&
	    pmsg->cp_version == RPCRDMA_CMP_VERSION) {
		rsize = rpcrdma_decode_buffer_size(pmsg->cp_send_size);
		wsize = rpcrdma_decode_buffer_size(pmsg->cp_recv_size);
	}

	if (rsize < ep->re_inline_recv)
		ep->re_inline_recv = rsize;
	if (wsize < ep->re_inline_send)
		ep->re_inline_send = wsize;

	rpcrdma_set_max_header_sizes(ep);
}

/**
 * rpcrdma_cm_event_handler - Handle RDMA CM events
 * @id: rdma_cm_id on which an event has occurred
 * @event: details of the event
 *
 * Called with @id's mutex held. Returns 1 if caller should
 * destroy @id, otherwise 0.
 */
static int
rpcrdma_cm_event_handler(struct rdma_cm_id *id, struct rdma_cm_event *event)
{
	struct sockaddr *sap = (struct sockaddr *)&id->route.addr.dst_addr;
	struct rpcrdma_ep *ep = id->context;

	might_sleep();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (event->event) {
	case RDMA_CM_EVENT_ADDR_RESOLVED:
	case RDMA_CM_EVENT_ROUTE_RESOLVED:
<<<<<<< HEAD
		ia->ri_async_rc = 0;
		complete(&ia->ri_done);
		break;
	case RDMA_CM_EVENT_ADDR_ERROR:
		ia->ri_async_rc = -EHOSTUNREACH;
		dprintk("RPC:       %s: CM address resolution error, ep 0x%p\n",
			__func__, ep);
		complete(&ia->ri_done);
		break;
	case RDMA_CM_EVENT_ROUTE_ERROR:
		ia->ri_async_rc = -ENETUNREACH;
		dprintk("RPC:       %s: CM route resolution error, ep 0x%p\n",
			__func__, ep);
		complete(&ia->ri_done);
		break;
	case RDMA_CM_EVENT_ESTABLISHED:
		connstate = 1;
		ib_query_qp(ia->ri_id->qp, &attr,
			IB_QP_MAX_QP_RD_ATOMIC | IB_QP_MAX_DEST_RD_ATOMIC,
			&iattr);
		dprintk("RPC:       %s: %d responder resources"
			" (%d initiator)\n",
			__func__, attr.max_dest_rd_atomic, attr.max_rd_atomic);
		goto connected;
	case RDMA_CM_EVENT_CONNECT_ERROR:
		connstate = -ENOTCONN;
		goto connected;
	case RDMA_CM_EVENT_UNREACHABLE:
		connstate = -ENETDOWN;
		goto connected;
	case RDMA_CM_EVENT_REJECTED:
		connstate = -ECONNREFUSED;
		goto connected;
	case RDMA_CM_EVENT_DISCONNECTED:
		connstate = -ECONNABORTED;
		goto connected;
	case RDMA_CM_EVENT_DEVICE_REMOVAL:
		connstate = -ENODEV;
connected:
		dprintk("RPC:       %s: %s: %pI4:%u (ep 0x%p event 0x%x)\n",
			__func__,
			(event->event <= 11) ? conn[event->event] :
						"unknown connection error",
			&addr->sin_addr.s_addr,
			ntohs(addr->sin_port),
			ep, event->event);
		atomic_set(&rpcx_to_rdmax(ep->rep_xprt)->rx_buf.rb_credits, 1);
		dprintk("RPC:       %s: %sconnected\n",
					__func__, connstate > 0 ? "" : "dis");
		ep->rep_connected = connstate;
		ep->rep_func(ep);
		wake_up_all(&ep->rep_connect_wait);
		break;
	default:
		dprintk("RPC:       %s: unexpected CM event %d\n",
			__func__, event->event);
		break;
	}

#ifdef RPC_DEBUG
	if (connstate == 1) {
		int ird = attr.max_dest_rd_atomic;
		int tird = ep->rep_remote_cma.responder_resources;
		printk(KERN_INFO "rpcrdma: connection to %pI4:%u "
			"on %s, memreg %d slots %d ird %d%s\n",
			&addr->sin_addr.s_addr,
			ntohs(addr->sin_port),
			ia->ri_id->device->name,
			ia->ri_memreg_strategy,
			xprt->rx_buf.rb_max_requests,
			ird, ird < 4 && ird < tird / 2 ? " (low!)" : "");
	} else if (connstate < 0) {
		printk(KERN_INFO "rpcrdma: connection to %pI4:%u closed (%d)\n",
			&addr->sin_addr.s_addr,
			ntohs(addr->sin_port),
			connstate);
	}
#endif

	return 0;
}

static struct rdma_cm_id *
rpcrdma_create_id(struct rpcrdma_xprt *xprt,
			struct rpcrdma_ia *ia, struct sockaddr *addr)
{
	struct rdma_cm_id *id;
	int rc;

	init_completion(&ia->ri_done);

	id = rdma_create_id(rpcrdma_conn_upcall, xprt, RDMA_PS_TCP, IB_QPT_RC);
	if (IS_ERR(id)) {
		rc = PTR_ERR(id);
		dprintk("RPC:       %s: rdma_create_id() failed %i\n",
			__func__, rc);
		return id;
	}

	ia->ri_async_rc = -ETIMEDOUT;
	rc = rdma_resolve_addr(id, NULL, addr, RDMA_RESOLVE_TIMEOUT);
	if (rc) {
		dprintk("RPC:       %s: rdma_resolve_addr() failed %i\n",
			__func__, rc);
		goto out;
	}
	wait_for_completion_interruptible_timeout(&ia->ri_done,
				msecs_to_jiffies(RDMA_RESOLVE_TIMEOUT) + 1);
	rc = ia->ri_async_rc;
	if (rc)
		goto out;

	ia->ri_async_rc = -ETIMEDOUT;
	rc = rdma_resolve_route(id, RDMA_RESOLVE_TIMEOUT);
	if (rc) {
		dprintk("RPC:       %s: rdma_resolve_route() failed %i\n",
			__func__, rc);
		goto out;
	}
	wait_for_completion_interruptible_timeout(&ia->ri_done,
				msecs_to_jiffies(RDMA_RESOLVE_TIMEOUT) + 1);
	rc = ia->ri_async_rc;
=======
		ep->re_async_rc = 0;
		complete(&ep->re_done);
		return 0;
	case RDMA_CM_EVENT_ADDR_ERROR:
		ep->re_async_rc = -EPROTO;
		complete(&ep->re_done);
		return 0;
	case RDMA_CM_EVENT_ROUTE_ERROR:
		ep->re_async_rc = -ENETUNREACH;
		complete(&ep->re_done);
		return 0;
	case RDMA_CM_EVENT_DEVICE_REMOVAL:
		pr_info("rpcrdma: removing device %s for %pISpc\n",
			ep->re_id->device->name, sap);
		fallthrough;
	case RDMA_CM_EVENT_ADDR_CHANGE:
		ep->re_connect_status = -ENODEV;
		goto disconnected;
	case RDMA_CM_EVENT_ESTABLISHED:
		rpcrdma_ep_get(ep);
		ep->re_connect_status = 1;
		rpcrdma_update_cm_private(ep, &event->param.conn);
		trace_xprtrdma_inline_thresh(ep);
		wake_up_all(&ep->re_connect_wait);
		break;
	case RDMA_CM_EVENT_CONNECT_ERROR:
		ep->re_connect_status = -ENOTCONN;
		goto wake_connect_worker;
	case RDMA_CM_EVENT_UNREACHABLE:
		ep->re_connect_status = -ENETUNREACH;
		goto wake_connect_worker;
	case RDMA_CM_EVENT_REJECTED:
		ep->re_connect_status = -ECONNREFUSED;
		if (event->status == IB_CM_REJ_STALE_CONN)
			ep->re_connect_status = -ENOTCONN;
wake_connect_worker:
		wake_up_all(&ep->re_connect_wait);
		return 0;
	case RDMA_CM_EVENT_DISCONNECTED:
		ep->re_connect_status = -ECONNABORTED;
disconnected:
		rpcrdma_force_disconnect(ep);
		return rpcrdma_ep_put(ep);
	default:
		break;
	}

	return 0;
}

static struct rdma_cm_id *rpcrdma_create_id(struct rpcrdma_xprt *r_xprt,
					    struct rpcrdma_ep *ep)
{
	unsigned long wtimeout = msecs_to_jiffies(RDMA_RESOLVE_TIMEOUT) + 1;
	struct rpc_xprt *xprt = &r_xprt->rx_xprt;
	struct rdma_cm_id *id;
	int rc;

	init_completion(&ep->re_done);

	id = rdma_create_id(xprt->xprt_net, rpcrdma_cm_event_handler, ep,
			    RDMA_PS_TCP, IB_QPT_RC);
	if (IS_ERR(id))
		return id;

	ep->re_async_rc = -ETIMEDOUT;
	rc = rdma_resolve_addr(id, NULL, (struct sockaddr *)&xprt->addr,
			       RDMA_RESOLVE_TIMEOUT);
	if (rc)
		goto out;
	rc = wait_for_completion_interruptible_timeout(&ep->re_done, wtimeout);
	if (rc < 0)
		goto out;

	rc = ep->re_async_rc;
	if (rc)
		goto out;

	ep->re_async_rc = -ETIMEDOUT;
	rc = rdma_resolve_route(id, RDMA_RESOLVE_TIMEOUT);
	if (rc)
		goto out;
	rc = wait_for_completion_interruptible_timeout(&ep->re_done, wtimeout);
	if (rc < 0)
		goto out;
	rc = ep->re_async_rc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (rc)
		goto out;

	return id;

out:
	rdma_destroy_id(id);
	return ERR_PTR(rc);
}

<<<<<<< HEAD
/*
 * Drain any cq, prior to teardown.
 */
static void
rpcrdma_clean_cq(struct ib_cq *cq)
{
	struct ib_wc wc;
	int count = 0;

	while (1 == ib_poll_cq(cq, 1, &wc))
		++count;

	if (count)
		dprintk("RPC:       %s: flushed %d events (last 0x%x)\n",
			__func__, count, wc.opcode);
}

/*
 * Exported functions.
 */

/*
 * Open and initialize an Interface Adapter.
 *  o initializes fields of struct rpcrdma_ia, including
 *    interface and provider attributes and protection zone.
 */
int
rpcrdma_ia_open(struct rpcrdma_xprt *xprt, struct sockaddr *addr, int memreg)
{
	int rc, mem_priv;
	struct ib_device_attr devattr;
	struct rpcrdma_ia *ia = &xprt->rx_ia;

	ia->ri_id = rpcrdma_create_id(xprt, ia, addr);
	if (IS_ERR(ia->ri_id)) {
		rc = PTR_ERR(ia->ri_id);
		goto out1;
	}

	ia->ri_pd = ib_alloc_pd(ia->ri_id->device);
	if (IS_ERR(ia->ri_pd)) {
		rc = PTR_ERR(ia->ri_pd);
		dprintk("RPC:       %s: ib_alloc_pd() failed %i\n",
			__func__, rc);
		goto out2;
	}

	/*
	 * Query the device to determine if the requested memory
	 * registration strategy is supported. If it isn't, set the
	 * strategy to a globally supported model.
	 */
	rc = ib_query_device(ia->ri_id->device, &devattr);
	if (rc) {
		dprintk("RPC:       %s: ib_query_device failed %d\n",
			__func__, rc);
		goto out3;
	}

	if (devattr.device_cap_flags & IB_DEVICE_LOCAL_DMA_LKEY) {
		ia->ri_have_dma_lkey = 1;
		ia->ri_dma_lkey = ia->ri_id->device->local_dma_lkey;
	}

	switch (memreg) {
	case RPCRDMA_MEMWINDOWS:
	case RPCRDMA_MEMWINDOWS_ASYNC:
		if (!(devattr.device_cap_flags & IB_DEVICE_MEM_WINDOW)) {
			dprintk("RPC:       %s: MEMWINDOWS registration "
				"specified but not supported by adapter, "
				"using slower RPCRDMA_REGISTER\n",
				__func__);
			memreg = RPCRDMA_REGISTER;
		}
		break;
	case RPCRDMA_MTHCAFMR:
		if (!ia->ri_id->device->alloc_fmr) {
#if RPCRDMA_PERSISTENT_REGISTRATION
			dprintk("RPC:       %s: MTHCAFMR registration "
				"specified but not supported by adapter, "
				"using riskier RPCRDMA_ALLPHYSICAL\n",
				__func__);
			memreg = RPCRDMA_ALLPHYSICAL;
#else
			dprintk("RPC:       %s: MTHCAFMR registration "
				"specified but not supported by adapter, "
				"using slower RPCRDMA_REGISTER\n",
				__func__);
			memreg = RPCRDMA_REGISTER;
#endif
		}
		break;
	case RPCRDMA_FRMR:
		/* Requires both frmr reg and local dma lkey */
		if ((devattr.device_cap_flags &
		     (IB_DEVICE_MEM_MGT_EXTENSIONS|IB_DEVICE_LOCAL_DMA_LKEY)) !=
		    (IB_DEVICE_MEM_MGT_EXTENSIONS|IB_DEVICE_LOCAL_DMA_LKEY)) {
#if RPCRDMA_PERSISTENT_REGISTRATION
			dprintk("RPC:       %s: FRMR registration "
				"specified but not supported by adapter, "
				"using riskier RPCRDMA_ALLPHYSICAL\n",
				__func__);
			memreg = RPCRDMA_ALLPHYSICAL;
#else
			dprintk("RPC:       %s: FRMR registration "
				"specified but not supported by adapter, "
				"using slower RPCRDMA_REGISTER\n",
				__func__);
			memreg = RPCRDMA_REGISTER;
#endif
		}
		break;
	}

	/*
	 * Optionally obtain an underlying physical identity mapping in
	 * order to do a memory window-based bind. This base registration
	 * is protected from remote access - that is enabled only by binding
	 * for the specific bytes targeted during each RPC operation, and
	 * revoked after the corresponding completion similar to a storage
	 * adapter.
	 */
	switch (memreg) {
	case RPCRDMA_BOUNCEBUFFERS:
	case RPCRDMA_REGISTER:
	case RPCRDMA_FRMR:
		break;
#if RPCRDMA_PERSISTENT_REGISTRATION
	case RPCRDMA_ALLPHYSICAL:
		mem_priv = IB_ACCESS_LOCAL_WRITE |
				IB_ACCESS_REMOTE_WRITE |
				IB_ACCESS_REMOTE_READ;
		goto register_setup;
#endif
	case RPCRDMA_MEMWINDOWS_ASYNC:
	case RPCRDMA_MEMWINDOWS:
		mem_priv = IB_ACCESS_LOCAL_WRITE |
				IB_ACCESS_MW_BIND;
		goto register_setup;
	case RPCRDMA_MTHCAFMR:
		if (ia->ri_have_dma_lkey)
			break;
		mem_priv = IB_ACCESS_LOCAL_WRITE;
	register_setup:
		ia->ri_bind_mem = ib_get_dma_mr(ia->ri_pd, mem_priv);
		if (IS_ERR(ia->ri_bind_mem)) {
			printk(KERN_ALERT "%s: ib_get_dma_mr for "
				"phys register failed with %lX\n\t"
				"Will continue with degraded performance\n",
				__func__, PTR_ERR(ia->ri_bind_mem));
			memreg = RPCRDMA_REGISTER;
			ia->ri_bind_mem = NULL;
		}
		break;
	default:
		printk(KERN_ERR "%s: invalid memory registration mode %d\n",
				__func__, memreg);
		rc = -EINVAL;
		goto out3;
	}
	dprintk("RPC:       %s: memory registration strategy is %d\n",
		__func__, memreg);

	/* Else will do memory reg/dereg for each chunk */
	ia->ri_memreg_strategy = memreg;

	return 0;

out3:
	ib_dealloc_pd(ia->ri_pd);
	ia->ri_pd = NULL;
out2:
	rdma_destroy_id(ia->ri_id);
	ia->ri_id = NULL;
out1:
	return rc;
}

/*
 * Clean up/close an IA.
 *   o if event handles and PD have been initialized, free them.
 *   o close the IA
 */
void
rpcrdma_ia_close(struct rpcrdma_ia *ia)
{
	int rc;

	dprintk("RPC:       %s: entering\n", __func__);
	if (ia->ri_bind_mem != NULL) {
		rc = ib_dereg_mr(ia->ri_bind_mem);
		dprintk("RPC:       %s: ib_dereg_mr returned %i\n",
			__func__, rc);
	}
	if (ia->ri_id != NULL && !IS_ERR(ia->ri_id)) {
		if (ia->ri_id->qp)
			rdma_destroy_qp(ia->ri_id);
		rdma_destroy_id(ia->ri_id);
		ia->ri_id = NULL;
	}
	if (ia->ri_pd != NULL && !IS_ERR(ia->ri_pd)) {
		rc = ib_dealloc_pd(ia->ri_pd);
		dprintk("RPC:       %s: ib_dealloc_pd returned %i\n",
			__func__, rc);
	}
}

/*
 * Create unconnected endpoint.
 */
int
rpcrdma_ep_create(struct rpcrdma_ep *ep, struct rpcrdma_ia *ia,
				struct rpcrdma_create_data_internal *cdata)
{
	struct ib_device_attr devattr;
	int rc, err;

	rc = ib_query_device(ia->ri_id->device, &devattr);
	if (rc) {
		dprintk("RPC:       %s: ib_query_device failed %d\n",
			__func__, rc);
		return rc;
	}

	/* check provider's send/recv wr limits */
	if (cdata->max_requests > devattr.max_qp_wr)
		cdata->max_requests = devattr.max_qp_wr;

	ep->rep_attr.event_handler = rpcrdma_qp_async_error_upcall;
	ep->rep_attr.qp_context = ep;
	/* send_cq and recv_cq initialized below */
	ep->rep_attr.srq = NULL;
	ep->rep_attr.cap.max_send_wr = cdata->max_requests;
	switch (ia->ri_memreg_strategy) {
	case RPCRDMA_FRMR:
		/* Add room for frmr register and invalidate WRs.
		 * 1. FRMR reg WR for head
		 * 2. FRMR invalidate WR for head
		 * 3. FRMR reg WR for pagelist
		 * 4. FRMR invalidate WR for pagelist
		 * 5. FRMR reg WR for tail
		 * 6. FRMR invalidate WR for tail
		 * 7. The RDMA_SEND WR
		 */
		ep->rep_attr.cap.max_send_wr *= 7;
		if (ep->rep_attr.cap.max_send_wr > devattr.max_qp_wr) {
			cdata->max_requests = devattr.max_qp_wr / 7;
			if (!cdata->max_requests)
				return -EINVAL;
			ep->rep_attr.cap.max_send_wr = cdata->max_requests * 7;
		}
		break;
	case RPCRDMA_MEMWINDOWS_ASYNC:
	case RPCRDMA_MEMWINDOWS:
		/* Add room for mw_binds+unbinds - overkill! */
		ep->rep_attr.cap.max_send_wr++;
		ep->rep_attr.cap.max_send_wr *= (2 * RPCRDMA_MAX_SEGS);
		if (ep->rep_attr.cap.max_send_wr > devattr.max_qp_wr)
			return -EINVAL;
		break;
	default:
		break;
	}
	ep->rep_attr.cap.max_recv_wr = cdata->max_requests;
	ep->rep_attr.cap.max_send_sge = (cdata->padding ? 4 : 2);
	ep->rep_attr.cap.max_recv_sge = 1;
	ep->rep_attr.cap.max_inline_data = 0;
	ep->rep_attr.sq_sig_type = IB_SIGNAL_REQ_WR;
	ep->rep_attr.qp_type = IB_QPT_RC;
	ep->rep_attr.port_num = ~0;

	dprintk("RPC:       %s: requested max: dtos: send %d recv %d; "
		"iovs: send %d recv %d\n",
		__func__,
		ep->rep_attr.cap.max_send_wr,
		ep->rep_attr.cap.max_recv_wr,
		ep->rep_attr.cap.max_send_sge,
		ep->rep_attr.cap.max_recv_sge);

	/* set trigger for requesting send completion */
	ep->rep_cqinit = ep->rep_attr.cap.max_send_wr/2 /*  - 1*/;
	switch (ia->ri_memreg_strategy) {
	case RPCRDMA_MEMWINDOWS_ASYNC:
	case RPCRDMA_MEMWINDOWS:
		ep->rep_cqinit -= RPCRDMA_MAX_SEGS;
		break;
	default:
		break;
	}
	if (ep->rep_cqinit <= 2)
		ep->rep_cqinit = 0;
	INIT_CQCOUNT(ep);
	ep->rep_ia = ia;
	init_waitqueue_head(&ep->rep_connect_wait);

	/*
	 * Create a single cq for receive dto and mw_bind (only ever
	 * care about unbind, really). Send completions are suppressed.
	 * Use single threaded tasklet upcalls to maintain ordering.
	 */
	ep->rep_cq = ib_create_cq(ia->ri_id->device, rpcrdma_cq_event_upcall,
				  rpcrdma_cq_async_error_upcall, NULL,
				  ep->rep_attr.cap.max_recv_wr +
				  ep->rep_attr.cap.max_send_wr + 1, 0);
	if (IS_ERR(ep->rep_cq)) {
		rc = PTR_ERR(ep->rep_cq);
		dprintk("RPC:       %s: ib_create_cq failed: %i\n",
			__func__, rc);
		goto out1;
	}

	rc = ib_req_notify_cq(ep->rep_cq, IB_CQ_NEXT_COMP);
	if (rc) {
		dprintk("RPC:       %s: ib_req_notify_cq failed: %i\n",
			__func__, rc);
		goto out2;
	}

	ep->rep_attr.send_cq = ep->rep_cq;
	ep->rep_attr.recv_cq = ep->rep_cq;

	/* Initialize cma parameters */

	/* RPC/RDMA does not use private data */
	ep->rep_remote_cma.private_data = NULL;
	ep->rep_remote_cma.private_data_len = 0;

	/* Client offers RDMA Read but does not initiate */
	ep->rep_remote_cma.initiator_depth = 0;
	if (ia->ri_memreg_strategy == RPCRDMA_BOUNCEBUFFERS)
		ep->rep_remote_cma.responder_resources = 0;
	else if (devattr.max_qp_rd_atom > 32)	/* arbitrary but <= 255 */
		ep->rep_remote_cma.responder_resources = 32;
	else
		ep->rep_remote_cma.responder_resources = devattr.max_qp_rd_atom;

	ep->rep_remote_cma.retry_count = 7;
	ep->rep_remote_cma.flow_control = 0;
	ep->rep_remote_cma.rnr_retry_count = 0;

	return 0;

out2:
	err = ib_destroy_cq(ep->rep_cq);
	if (err)
		dprintk("RPC:       %s: ib_destroy_cq returned %i\n",
			__func__, err);
out1:
	return rc;
}

/*
 * rpcrdma_ep_destroy
 *
 * Disconnect and destroy endpoint. After this, the only
 * valid operations on the ep are to free it (if dynamically
 * allocated) or re-create it.
 *
 * The caller's error handling must be sure to not leak the endpoint
 * if this function fails.
 */
int
rpcrdma_ep_destroy(struct rpcrdma_ep *ep, struct rpcrdma_ia *ia)
{
	int rc;

	dprintk("RPC:       %s: entering, connected is %d\n",
		__func__, ep->rep_connected);

	if (ia->ri_id->qp) {
		rc = rpcrdma_ep_disconnect(ep, ia);
		if (rc)
			dprintk("RPC:       %s: rpcrdma_ep_disconnect"
				" returned %i\n", __func__, rc);
		rdma_destroy_qp(ia->ri_id);
		ia->ri_id->qp = NULL;
	}

	/* padding - could be done in rpcrdma_buffer_destroy... */
	if (ep->rep_pad_mr) {
		rpcrdma_deregister_internal(ia, ep->rep_pad_mr, &ep->rep_pad);
		ep->rep_pad_mr = NULL;
	}

	rpcrdma_clean_cq(ep->rep_cq);
	rc = ib_destroy_cq(ep->rep_cq);
	if (rc)
		dprintk("RPC:       %s: ib_destroy_cq returned %i\n",
			__func__, rc);

	return rc;
}

/*
 * Connect unconnected endpoint.
 */
int
rpcrdma_ep_connect(struct rpcrdma_ep *ep, struct rpcrdma_ia *ia)
{
	struct rdma_cm_id *id;
	int rc = 0;
	int retry_count = 0;

	if (ep->rep_connected != 0) {
		struct rpcrdma_xprt *xprt;
retry:
		rc = rpcrdma_ep_disconnect(ep, ia);
		if (rc && rc != -ENOTCONN)
			dprintk("RPC:       %s: rpcrdma_ep_disconnect"
				" status %i\n", __func__, rc);
		rpcrdma_clean_cq(ep->rep_cq);

		xprt = container_of(ia, struct rpcrdma_xprt, rx_ia);
		id = rpcrdma_create_id(xprt, ia,
				(struct sockaddr *)&xprt->rx_data.addr);
		if (IS_ERR(id)) {
			rc = PTR_ERR(id);
			goto out;
		}
		/* TEMP TEMP TEMP - fail if new device:
		 * Deregister/remarshal *all* requests!
		 * Close and recreate adapter, pd, etc!
		 * Re-determine all attributes still sane!
		 * More stuff I haven't thought of!
		 * Rrrgh!
		 */
		if (ia->ri_id->device != id->device) {
			printk("RPC:       %s: can't reconnect on "
				"different device!\n", __func__);
			rdma_destroy_id(id);
			rc = -ENETDOWN;
			goto out;
		}
		/* END TEMP */
		rdma_destroy_qp(ia->ri_id);
		rdma_destroy_id(ia->ri_id);
		ia->ri_id = id;
	}

	rc = rdma_create_qp(ia->ri_id, ia->ri_pd, &ep->rep_attr);
	if (rc) {
		dprintk("RPC:       %s: rdma_create_qp failed %i\n",
			__func__, rc);
		goto out;
	}

/* XXX Tavor device performs badly with 2K MTU! */
if (strnicmp(ia->ri_id->device->dma_device->bus->name, "pci", 3) == 0) {
	struct pci_dev *pcid = to_pci_dev(ia->ri_id->device->dma_device);
	if (pcid->device == PCI_DEVICE_ID_MELLANOX_TAVOR &&
	    (pcid->vendor == PCI_VENDOR_ID_MELLANOX ||
	     pcid->vendor == PCI_VENDOR_ID_TOPSPIN)) {
		struct ib_qp_attr attr = {
			.path_mtu = IB_MTU_1024
		};
		rc = ib_modify_qp(ia->ri_id->qp, &attr, IB_QP_PATH_MTU);
	}
}

	ep->rep_connected = 0;

	rc = rdma_connect(ia->ri_id, &ep->rep_remote_cma);
	if (rc) {
		dprintk("RPC:       %s: rdma_connect() failed with %i\n",
				__func__, rc);
		goto out;
	}

	wait_event_interruptible(ep->rep_connect_wait, ep->rep_connected != 0);

	/*
	 * Check state. A non-peer reject indicates no listener
	 * (ECONNREFUSED), which may be a transient state. All
	 * others indicate a transport condition which has already
	 * undergone a best-effort.
	 */
	if (ep->rep_connected == -ECONNREFUSED &&
	    ++retry_count <= RDMA_CONNECT_RETRY_MAX) {
		dprintk("RPC:       %s: non-peer_reject, retry\n", __func__);
		goto retry;
	}
	if (ep->rep_connected <= 0) {
		/* Sometimes, the only way to reliably connect to remote
		 * CMs is to use same nonzero values for ORD and IRD. */
		if (retry_count++ <= RDMA_CONNECT_RETRY_MAX + 1 &&
		    (ep->rep_remote_cma.responder_resources == 0 ||
		     ep->rep_remote_cma.initiator_depth !=
				ep->rep_remote_cma.responder_resources)) {
			if (ep->rep_remote_cma.responder_resources == 0)
				ep->rep_remote_cma.responder_resources = 1;
			ep->rep_remote_cma.initiator_depth =
				ep->rep_remote_cma.responder_resources;
			goto retry;
		}
		rc = ep->rep_connected;
	} else {
		dprintk("RPC:       %s: connected\n", __func__);
	}

out:
	if (rc)
		ep->rep_connected = rc;
	return rc;
}

/*
 * rpcrdma_ep_disconnect
 *
 * This is separate from destroy to facilitate the ability
 * to reconnect without recreating the endpoint.
 *
 * This call is not reentrant, and must not be made in parallel
 * on the same endpoint.
 */
int
rpcrdma_ep_disconnect(struct rpcrdma_ep *ep, struct rpcrdma_ia *ia)
{
	int rc;

	rpcrdma_clean_cq(ep->rep_cq);
	rc = rdma_disconnect(ia->ri_id);
	if (!rc) {
		/* returns without wait if not connected */
		wait_event_interruptible(ep->rep_connect_wait,
							ep->rep_connected != 1);
		dprintk("RPC:       %s: after wait, %sconnected\n", __func__,
			(ep->rep_connected == 1) ? "still " : "dis");
	} else {
		dprintk("RPC:       %s: rdma_disconnect %i\n", __func__, rc);
		ep->rep_connected = rc;
	}
	return rc;
}

/*
 * Initialize buffer memory
 */
int
rpcrdma_buffer_create(struct rpcrdma_buffer *buf, struct rpcrdma_ep *ep,
	struct rpcrdma_ia *ia, struct rpcrdma_create_data_internal *cdata)
{
	char *p;
	size_t len;
	int i, rc;
	struct rpcrdma_mw *r;

	buf->rb_max_requests = cdata->max_requests;
	spin_lock_init(&buf->rb_lock);
	atomic_set(&buf->rb_credits, 1);

	/* Need to allocate:
	 *   1.  arrays for send and recv pointers
	 *   2.  arrays of struct rpcrdma_req to fill in pointers
	 *   3.  array of struct rpcrdma_rep for replies
	 *   4.  padding, if any
	 *   5.  mw's, fmr's or frmr's, if any
	 * Send/recv buffers in req/rep need to be registered
	 */

	len = buf->rb_max_requests *
		(sizeof(struct rpcrdma_req *) + sizeof(struct rpcrdma_rep *));
	len += cdata->padding;
	switch (ia->ri_memreg_strategy) {
	case RPCRDMA_FRMR:
		len += buf->rb_max_requests * RPCRDMA_MAX_SEGS *
				sizeof(struct rpcrdma_mw);
		break;
	case RPCRDMA_MTHCAFMR:
		/* TBD we are perhaps overallocating here */
		len += (buf->rb_max_requests + 1) * RPCRDMA_MAX_SEGS *
				sizeof(struct rpcrdma_mw);
		break;
	case RPCRDMA_MEMWINDOWS_ASYNC:
	case RPCRDMA_MEMWINDOWS:
		len += (buf->rb_max_requests + 1) * RPCRDMA_MAX_SEGS *
				sizeof(struct rpcrdma_mw);
		break;
	default:
		break;
	}

	/* allocate 1, 4 and 5 in one shot */
	p = kzalloc(len, GFP_KERNEL);
	if (p == NULL) {
		dprintk("RPC:       %s: req_t/rep_t/pad kzalloc(%zd) failed\n",
			__func__, len);
		rc = -ENOMEM;
		goto out;
	}
	buf->rb_pool = p;	/* for freeing it later */

	buf->rb_send_bufs = (struct rpcrdma_req **) p;
	p = (char *) &buf->rb_send_bufs[buf->rb_max_requests];
	buf->rb_recv_bufs = (struct rpcrdma_rep **) p;
	p = (char *) &buf->rb_recv_bufs[buf->rb_max_requests];

	/*
	 * Register the zeroed pad buffer, if any.
	 */
	if (cdata->padding) {
		rc = rpcrdma_register_internal(ia, p, cdata->padding,
					    &ep->rep_pad_mr, &ep->rep_pad);
		if (rc)
			goto out;
	}
	p += cdata->padding;

	/*
	 * Allocate the fmr's, or mw's for mw_bind chunk registration.
	 * We "cycle" the mw's in order to minimize rkey reuse,
	 * and also reduce unbind-to-bind collision.
	 */
	INIT_LIST_HEAD(&buf->rb_mws);
	r = (struct rpcrdma_mw *)p;
	switch (ia->ri_memreg_strategy) {
	case RPCRDMA_FRMR:
		for (i = buf->rb_max_requests * RPCRDMA_MAX_SEGS; i; i--) {
			r->r.frmr.fr_mr = ib_alloc_fast_reg_mr(ia->ri_pd,
							 RPCRDMA_MAX_SEGS);
			if (IS_ERR(r->r.frmr.fr_mr)) {
				rc = PTR_ERR(r->r.frmr.fr_mr);
				dprintk("RPC:       %s: ib_alloc_fast_reg_mr"
					" failed %i\n", __func__, rc);
				goto out;
			}
			r->r.frmr.fr_pgl =
				ib_alloc_fast_reg_page_list(ia->ri_id->device,
							    RPCRDMA_MAX_SEGS);
			if (IS_ERR(r->r.frmr.fr_pgl)) {
				rc = PTR_ERR(r->r.frmr.fr_pgl);
				dprintk("RPC:       %s: "
					"ib_alloc_fast_reg_page_list "
					"failed %i\n", __func__, rc);
				goto out;
			}
			list_add(&r->mw_list, &buf->rb_mws);
			++r;
		}
		break;
	case RPCRDMA_MTHCAFMR:
		/* TBD we are perhaps overallocating here */
		for (i = (buf->rb_max_requests+1) * RPCRDMA_MAX_SEGS; i; i--) {
			static struct ib_fmr_attr fa =
				{ RPCRDMA_MAX_DATA_SEGS, 1, PAGE_SHIFT };
			r->r.fmr = ib_alloc_fmr(ia->ri_pd,
				IB_ACCESS_REMOTE_WRITE | IB_ACCESS_REMOTE_READ,
				&fa);
			if (IS_ERR(r->r.fmr)) {
				rc = PTR_ERR(r->r.fmr);
				dprintk("RPC:       %s: ib_alloc_fmr"
					" failed %i\n", __func__, rc);
				goto out;
			}
			list_add(&r->mw_list, &buf->rb_mws);
			++r;
		}
		break;
	case RPCRDMA_MEMWINDOWS_ASYNC:
	case RPCRDMA_MEMWINDOWS:
		/* Allocate one extra request's worth, for full cycling */
		for (i = (buf->rb_max_requests+1) * RPCRDMA_MAX_SEGS; i; i--) {
			r->r.mw = ib_alloc_mw(ia->ri_pd);
			if (IS_ERR(r->r.mw)) {
				rc = PTR_ERR(r->r.mw);
				dprintk("RPC:       %s: ib_alloc_mw"
					" failed %i\n", __func__, rc);
				goto out;
			}
			list_add(&r->mw_list, &buf->rb_mws);
			++r;
		}
		break;
	default:
		break;
	}

	/*
	 * Allocate/init the request/reply buffers. Doing this
	 * using kmalloc for now -- one for each buf.
	 */
	for (i = 0; i < buf->rb_max_requests; i++) {
		struct rpcrdma_req *req;
		struct rpcrdma_rep *rep;

		len = cdata->inline_wsize + sizeof(struct rpcrdma_req);
		/* RPC layer requests *double* size + 1K RPC_SLACK_SPACE! */
		/* Typical ~2400b, so rounding up saves work later */
		if (len < 4096)
			len = 4096;
		req = kmalloc(len, GFP_KERNEL);
		if (req == NULL) {
			dprintk("RPC:       %s: request buffer %d alloc"
				" failed\n", __func__, i);
			rc = -ENOMEM;
			goto out;
		}
		memset(req, 0, sizeof(struct rpcrdma_req));
		buf->rb_send_bufs[i] = req;
		buf->rb_send_bufs[i]->rl_buffer = buf;

		rc = rpcrdma_register_internal(ia, req->rl_base,
				len - offsetof(struct rpcrdma_req, rl_base),
				&buf->rb_send_bufs[i]->rl_handle,
				&buf->rb_send_bufs[i]->rl_iov);
		if (rc)
			goto out;

		buf->rb_send_bufs[i]->rl_size = len-sizeof(struct rpcrdma_req);

		len = cdata->inline_rsize + sizeof(struct rpcrdma_rep);
		rep = kmalloc(len, GFP_KERNEL);
		if (rep == NULL) {
			dprintk("RPC:       %s: reply buffer %d alloc failed\n",
				__func__, i);
			rc = -ENOMEM;
			goto out;
		}
		memset(rep, 0, sizeof(struct rpcrdma_rep));
		buf->rb_recv_bufs[i] = rep;
		buf->rb_recv_bufs[i]->rr_buffer = buf;
		init_waitqueue_head(&rep->rr_unbind);

		rc = rpcrdma_register_internal(ia, rep->rr_base,
				len - offsetof(struct rpcrdma_rep, rr_base),
				&buf->rb_recv_bufs[i]->rr_handle,
				&buf->rb_recv_bufs[i]->rr_iov);
		if (rc)
			goto out;

	}
	dprintk("RPC:       %s: max_requests %d\n",
		__func__, buf->rb_max_requests);
	/* done */
=======
static void rpcrdma_ep_destroy(struct kref *kref)
{
	struct rpcrdma_ep *ep = container_of(kref, struct rpcrdma_ep, re_kref);

	if (ep->re_id->qp) {
		rdma_destroy_qp(ep->re_id);
		ep->re_id->qp = NULL;
	}

	if (ep->re_attr.recv_cq)
		ib_free_cq(ep->re_attr.recv_cq);
	ep->re_attr.recv_cq = NULL;
	if (ep->re_attr.send_cq)
		ib_free_cq(ep->re_attr.send_cq);
	ep->re_attr.send_cq = NULL;

	if (ep->re_pd)
		ib_dealloc_pd(ep->re_pd);
	ep->re_pd = NULL;

	kfree(ep);
	module_put(THIS_MODULE);
}

static noinline void rpcrdma_ep_get(struct rpcrdma_ep *ep)
{
	kref_get(&ep->re_kref);
}

/* Returns:
 *     %0 if @ep still has a positive kref count, or
 *     %1 if @ep was destroyed successfully.
 */
static noinline int rpcrdma_ep_put(struct rpcrdma_ep *ep)
{
	return kref_put(&ep->re_kref, rpcrdma_ep_destroy);
}

static int rpcrdma_ep_create(struct rpcrdma_xprt *r_xprt)
{
	struct rpcrdma_connect_private *pmsg;
	struct ib_device *device;
	struct rdma_cm_id *id;
	struct rpcrdma_ep *ep;
	int rc;

	ep = kzalloc(sizeof(*ep), XPRTRDMA_GFP_FLAGS);
	if (!ep)
		return -ENOTCONN;
	ep->re_xprt = &r_xprt->rx_xprt;
	kref_init(&ep->re_kref);

	id = rpcrdma_create_id(r_xprt, ep);
	if (IS_ERR(id)) {
		kfree(ep);
		return PTR_ERR(id);
	}
	__module_get(THIS_MODULE);
	device = id->device;
	ep->re_id = id;
	reinit_completion(&ep->re_done);

	ep->re_max_requests = r_xprt->rx_xprt.max_reqs;
	ep->re_inline_send = xprt_rdma_max_inline_write;
	ep->re_inline_recv = xprt_rdma_max_inline_read;
	rc = frwr_query_device(ep, device);
	if (rc)
		goto out_destroy;

	r_xprt->rx_buf.rb_max_requests = cpu_to_be32(ep->re_max_requests);

	ep->re_attr.srq = NULL;
	ep->re_attr.cap.max_inline_data = 0;
	ep->re_attr.sq_sig_type = IB_SIGNAL_REQ_WR;
	ep->re_attr.qp_type = IB_QPT_RC;
	ep->re_attr.port_num = ~0;

	ep->re_send_batch = ep->re_max_requests >> 3;
	ep->re_send_count = ep->re_send_batch;
	init_waitqueue_head(&ep->re_connect_wait);

	ep->re_attr.send_cq = ib_alloc_cq_any(device, r_xprt,
					      ep->re_attr.cap.max_send_wr,
					      IB_POLL_WORKQUEUE);
	if (IS_ERR(ep->re_attr.send_cq)) {
		rc = PTR_ERR(ep->re_attr.send_cq);
		ep->re_attr.send_cq = NULL;
		goto out_destroy;
	}

	ep->re_attr.recv_cq = ib_alloc_cq_any(device, r_xprt,
					      ep->re_attr.cap.max_recv_wr,
					      IB_POLL_WORKQUEUE);
	if (IS_ERR(ep->re_attr.recv_cq)) {
		rc = PTR_ERR(ep->re_attr.recv_cq);
		ep->re_attr.recv_cq = NULL;
		goto out_destroy;
	}
	ep->re_receive_count = 0;

	/* Initialize cma parameters */
	memset(&ep->re_remote_cma, 0, sizeof(ep->re_remote_cma));

	/* Prepare RDMA-CM private message */
	pmsg = &ep->re_cm_private;
	pmsg->cp_magic = rpcrdma_cmp_magic;
	pmsg->cp_version = RPCRDMA_CMP_VERSION;
	pmsg->cp_flags |= RPCRDMA_CMP_F_SND_W_INV_OK;
	pmsg->cp_send_size = rpcrdma_encode_buffer_size(ep->re_inline_send);
	pmsg->cp_recv_size = rpcrdma_encode_buffer_size(ep->re_inline_recv);
	ep->re_remote_cma.private_data = pmsg;
	ep->re_remote_cma.private_data_len = sizeof(*pmsg);

	/* Client offers RDMA Read but does not initiate */
	ep->re_remote_cma.initiator_depth = 0;
	ep->re_remote_cma.responder_resources =
		min_t(int, U8_MAX, device->attrs.max_qp_rd_atom);

	/* Limit transport retries so client can detect server
	 * GID changes quickly. RPC layer handles re-establishing
	 * transport connection and retransmission.
	 */
	ep->re_remote_cma.retry_count = 6;

	/* RPC-over-RDMA handles its own flow control. In addition,
	 * make all RNR NAKs visible so we know that RPC-over-RDMA
	 * flow control is working correctly (no NAKs should be seen).
	 */
	ep->re_remote_cma.flow_control = 0;
	ep->re_remote_cma.rnr_retry_count = 0;

	ep->re_pd = ib_alloc_pd(device, 0);
	if (IS_ERR(ep->re_pd)) {
		rc = PTR_ERR(ep->re_pd);
		ep->re_pd = NULL;
		goto out_destroy;
	}

	rc = rdma_create_qp(id, ep->re_pd, &ep->re_attr);
	if (rc)
		goto out_destroy;

	r_xprt->rx_ep = ep;
	return 0;

out_destroy:
	rpcrdma_ep_put(ep);
	rdma_destroy_id(id);
	return rc;
}

/**
 * rpcrdma_xprt_connect - Connect an unconnected transport
 * @r_xprt: controlling transport instance
 *
 * Returns 0 on success or a negative errno.
 */
int rpcrdma_xprt_connect(struct rpcrdma_xprt *r_xprt)
{
	struct rpc_xprt *xprt = &r_xprt->rx_xprt;
	struct rpcrdma_ep *ep;
	int rc;

	rc = rpcrdma_ep_create(r_xprt);
	if (rc)
		return rc;
	ep = r_xprt->rx_ep;

	xprt_clear_connected(xprt);
	rpcrdma_reset_cwnd(r_xprt);

	/* Bump the ep's reference count while there are
	 * outstanding Receives.
	 */
	rpcrdma_ep_get(ep);
	rpcrdma_post_recvs(r_xprt, 1, true);

	rc = rdma_connect(ep->re_id, &ep->re_remote_cma);
	if (rc)
		goto out;

	if (xprt->reestablish_timeout < RPCRDMA_INIT_REEST_TO)
		xprt->reestablish_timeout = RPCRDMA_INIT_REEST_TO;
	wait_event_interruptible(ep->re_connect_wait,
				 ep->re_connect_status != 0);
	if (ep->re_connect_status <= 0) {
		rc = ep->re_connect_status;
		goto out;
	}

	rc = rpcrdma_sendctxs_create(r_xprt);
	if (rc) {
		rc = -ENOTCONN;
		goto out;
	}

	rc = rpcrdma_reqs_setup(r_xprt);
	if (rc) {
		rc = -ENOTCONN;
		goto out;
	}
	rpcrdma_mrs_create(r_xprt);
	frwr_wp_create(r_xprt);

out:
	trace_xprtrdma_connect(r_xprt, rc);
	return rc;
}

/**
 * rpcrdma_xprt_disconnect - Disconnect underlying transport
 * @r_xprt: controlling transport instance
 *
 * Caller serializes. Either the transport send lock is held,
 * or we're being called to destroy the transport.
 *
 * On return, @r_xprt is completely divested of all hardware
 * resources and prepared for the next ->connect operation.
 */
void rpcrdma_xprt_disconnect(struct rpcrdma_xprt *r_xprt)
{
	struct rpcrdma_ep *ep = r_xprt->rx_ep;
	struct rdma_cm_id *id;
	int rc;

	if (!ep)
		return;

	id = ep->re_id;
	rc = rdma_disconnect(id);
	trace_xprtrdma_disconnect(r_xprt, rc);

	rpcrdma_xprt_drain(r_xprt);
	rpcrdma_reps_unmap(r_xprt);
	rpcrdma_reqs_reset(r_xprt);
	rpcrdma_mrs_destroy(r_xprt);
	rpcrdma_sendctxs_destroy(r_xprt);

	if (rpcrdma_ep_put(ep))
		rdma_destroy_id(id);

	r_xprt->rx_ep = NULL;
}

/* Fixed-size circular FIFO queue. This implementation is wait-free and
 * lock-free.
 *
 * Consumer is the code path that posts Sends. This path dequeues a
 * sendctx for use by a Send operation. Multiple consumer threads
 * are serialized by the RPC transport lock, which allows only one
 * ->send_request call at a time.
 *
 * Producer is the code path that handles Send completions. This path
 * enqueues a sendctx that has been completed. Multiple producer
 * threads are serialized by the ib_poll_cq() function.
 */

/* rpcrdma_sendctxs_destroy() assumes caller has already quiesced
 * queue activity, and rpcrdma_xprt_drain has flushed all remaining
 * Send requests.
 */
static void rpcrdma_sendctxs_destroy(struct rpcrdma_xprt *r_xprt)
{
	struct rpcrdma_buffer *buf = &r_xprt->rx_buf;
	unsigned long i;

	if (!buf->rb_sc_ctxs)
		return;
	for (i = 0; i <= buf->rb_sc_last; i++)
		kfree(buf->rb_sc_ctxs[i]);
	kfree(buf->rb_sc_ctxs);
	buf->rb_sc_ctxs = NULL;
}

static struct rpcrdma_sendctx *rpcrdma_sendctx_create(struct rpcrdma_ep *ep)
{
	struct rpcrdma_sendctx *sc;

	sc = kzalloc(struct_size(sc, sc_sges, ep->re_attr.cap.max_send_sge),
		     XPRTRDMA_GFP_FLAGS);
	if (!sc)
		return NULL;

	sc->sc_cqe.done = rpcrdma_wc_send;
	sc->sc_cid.ci_queue_id = ep->re_attr.send_cq->res.id;
	sc->sc_cid.ci_completion_id =
		atomic_inc_return(&ep->re_completion_ids);
	return sc;
}

static int rpcrdma_sendctxs_create(struct rpcrdma_xprt *r_xprt)
{
	struct rpcrdma_buffer *buf = &r_xprt->rx_buf;
	struct rpcrdma_sendctx *sc;
	unsigned long i;

	/* Maximum number of concurrent outstanding Send WRs. Capping
	 * the circular queue size stops Send Queue overflow by causing
	 * the ->send_request call to fail temporarily before too many
	 * Sends are posted.
	 */
	i = r_xprt->rx_ep->re_max_requests + RPCRDMA_MAX_BC_REQUESTS;
	buf->rb_sc_ctxs = kcalloc(i, sizeof(sc), XPRTRDMA_GFP_FLAGS);
	if (!buf->rb_sc_ctxs)
		return -ENOMEM;

	buf->rb_sc_last = i - 1;
	for (i = 0; i <= buf->rb_sc_last; i++) {
		sc = rpcrdma_sendctx_create(r_xprt->rx_ep);
		if (!sc)
			return -ENOMEM;

		buf->rb_sc_ctxs[i] = sc;
	}

	buf->rb_sc_head = 0;
	buf->rb_sc_tail = 0;
	return 0;
}

/* The sendctx queue is not guaranteed to have a size that is a
 * power of two, thus the helpers in circ_buf.h cannot be used.
 * The other option is to use modulus (%), which can be expensive.
 */
static unsigned long rpcrdma_sendctx_next(struct rpcrdma_buffer *buf,
					  unsigned long item)
{
	return likely(item < buf->rb_sc_last) ? item + 1 : 0;
}

/**
 * rpcrdma_sendctx_get_locked - Acquire a send context
 * @r_xprt: controlling transport instance
 *
 * Returns pointer to a free send completion context; or NULL if
 * the queue is empty.
 *
 * Usage: Called to acquire an SGE array before preparing a Send WR.
 *
 * The caller serializes calls to this function (per transport), and
 * provides an effective memory barrier that flushes the new value
 * of rb_sc_head.
 */
struct rpcrdma_sendctx *rpcrdma_sendctx_get_locked(struct rpcrdma_xprt *r_xprt)
{
	struct rpcrdma_buffer *buf = &r_xprt->rx_buf;
	struct rpcrdma_sendctx *sc;
	unsigned long next_head;

	next_head = rpcrdma_sendctx_next(buf, buf->rb_sc_head);

	if (next_head == READ_ONCE(buf->rb_sc_tail))
		goto out_emptyq;

	/* ORDER: item must be accessed _before_ head is updated */
	sc = buf->rb_sc_ctxs[next_head];

	/* Releasing the lock in the caller acts as a memory
	 * barrier that flushes rb_sc_head.
	 */
	buf->rb_sc_head = next_head;

	return sc;

out_emptyq:
	/* The queue is "empty" if there have not been enough Send
	 * completions recently. This is a sign the Send Queue is
	 * backing up. Cause the caller to pause and try again.
	 */
	xprt_wait_for_buffer_space(&r_xprt->rx_xprt);
	r_xprt->rx_stats.empty_sendctx_q++;
	return NULL;
}

/**
 * rpcrdma_sendctx_put_locked - Release a send context
 * @r_xprt: controlling transport instance
 * @sc: send context to release
 *
 * Usage: Called from Send completion to return a sendctxt
 * to the queue.
 *
 * The caller serializes calls to this function (per transport).
 */
static void rpcrdma_sendctx_put_locked(struct rpcrdma_xprt *r_xprt,
				       struct rpcrdma_sendctx *sc)
{
	struct rpcrdma_buffer *buf = &r_xprt->rx_buf;
	unsigned long next_tail;

	/* Unmap SGEs of previously completed but unsignaled
	 * Sends by walking up the queue until @sc is found.
	 */
	next_tail = buf->rb_sc_tail;
	do {
		next_tail = rpcrdma_sendctx_next(buf, next_tail);

		/* ORDER: item must be accessed _before_ tail is updated */
		rpcrdma_sendctx_unmap(buf->rb_sc_ctxs[next_tail]);

	} while (buf->rb_sc_ctxs[next_tail] != sc);

	/* Paired with READ_ONCE */
	smp_store_release(&buf->rb_sc_tail, next_tail);

	xprt_write_space(&r_xprt->rx_xprt);
}

static void
rpcrdma_mrs_create(struct rpcrdma_xprt *r_xprt)
{
	struct rpcrdma_buffer *buf = &r_xprt->rx_buf;
	struct rpcrdma_ep *ep = r_xprt->rx_ep;
	struct ib_device *device = ep->re_id->device;
	unsigned int count;

	/* Try to allocate enough to perform one full-sized I/O */
	for (count = 0; count < ep->re_max_rdma_segs; count++) {
		struct rpcrdma_mr *mr;
		int rc;

		mr = kzalloc_node(sizeof(*mr), XPRTRDMA_GFP_FLAGS,
				  ibdev_to_node(device));
		if (!mr)
			break;

		rc = frwr_mr_init(r_xprt, mr);
		if (rc) {
			kfree(mr);
			break;
		}

		spin_lock(&buf->rb_lock);
		rpcrdma_mr_push(mr, &buf->rb_mrs);
		list_add(&mr->mr_all, &buf->rb_all_mrs);
		spin_unlock(&buf->rb_lock);
	}

	r_xprt->rx_stats.mrs_allocated += count;
	trace_xprtrdma_createmrs(r_xprt, count);
}

static void
rpcrdma_mr_refresh_worker(struct work_struct *work)
{
	struct rpcrdma_buffer *buf = container_of(work, struct rpcrdma_buffer,
						  rb_refresh_worker);
	struct rpcrdma_xprt *r_xprt = container_of(buf, struct rpcrdma_xprt,
						   rx_buf);

	rpcrdma_mrs_create(r_xprt);
	xprt_write_space(&r_xprt->rx_xprt);
}

/**
 * rpcrdma_mrs_refresh - Wake the MR refresh worker
 * @r_xprt: controlling transport instance
 *
 */
void rpcrdma_mrs_refresh(struct rpcrdma_xprt *r_xprt)
{
	struct rpcrdma_buffer *buf = &r_xprt->rx_buf;
	struct rpcrdma_ep *ep = r_xprt->rx_ep;

	/* If there is no underlying connection, it's no use
	 * to wake the refresh worker.
	 */
	if (ep->re_connect_status != 1)
		return;
	queue_work(system_highpri_wq, &buf->rb_refresh_worker);
}

/**
 * rpcrdma_req_create - Allocate an rpcrdma_req object
 * @r_xprt: controlling r_xprt
 * @size: initial size, in bytes, of send and receive buffers
 *
 * Returns an allocated and fully initialized rpcrdma_req or NULL.
 */
struct rpcrdma_req *rpcrdma_req_create(struct rpcrdma_xprt *r_xprt,
				       size_t size)
{
	struct rpcrdma_buffer *buffer = &r_xprt->rx_buf;
	struct rpcrdma_req *req;

	req = kzalloc(sizeof(*req), XPRTRDMA_GFP_FLAGS);
	if (req == NULL)
		goto out1;

	req->rl_sendbuf = rpcrdma_regbuf_alloc(size, DMA_TO_DEVICE);
	if (!req->rl_sendbuf)
		goto out2;

	req->rl_recvbuf = rpcrdma_regbuf_alloc(size, DMA_NONE);
	if (!req->rl_recvbuf)
		goto out3;

	INIT_LIST_HEAD(&req->rl_free_mrs);
	INIT_LIST_HEAD(&req->rl_registered);
	spin_lock(&buffer->rb_lock);
	list_add(&req->rl_all, &buffer->rb_allreqs);
	spin_unlock(&buffer->rb_lock);
	return req;

out3:
	rpcrdma_regbuf_free(req->rl_sendbuf);
out2:
	kfree(req);
out1:
	return NULL;
}

/**
 * rpcrdma_req_setup - Per-connection instance setup of an rpcrdma_req object
 * @r_xprt: controlling transport instance
 * @req: rpcrdma_req object to set up
 *
 * Returns zero on success, and a negative errno on failure.
 */
int rpcrdma_req_setup(struct rpcrdma_xprt *r_xprt, struct rpcrdma_req *req)
{
	struct rpcrdma_regbuf *rb;
	size_t maxhdrsize;

	/* Compute maximum header buffer size in bytes */
	maxhdrsize = rpcrdma_fixed_maxsz + 3 +
		     r_xprt->rx_ep->re_max_rdma_segs * rpcrdma_readchunk_maxsz;
	maxhdrsize *= sizeof(__be32);
	rb = rpcrdma_regbuf_alloc(__roundup_pow_of_two(maxhdrsize),
				  DMA_TO_DEVICE);
	if (!rb)
		goto out;

	if (!__rpcrdma_regbuf_dma_map(r_xprt, rb))
		goto out_free;

	req->rl_rdmabuf = rb;
	xdr_buf_init(&req->rl_hdrbuf, rdmab_data(rb), rdmab_length(rb));
	return 0;

out_free:
	rpcrdma_regbuf_free(rb);
out:
	return -ENOMEM;
}

/* ASSUMPTION: the rb_allreqs list is stable for the duration,
 * and thus can be walked without holding rb_lock. Eg. the
 * caller is holding the transport send lock to exclude
 * device removal or disconnection.
 */
static int rpcrdma_reqs_setup(struct rpcrdma_xprt *r_xprt)
{
	struct rpcrdma_buffer *buf = &r_xprt->rx_buf;
	struct rpcrdma_req *req;
	int rc;

	list_for_each_entry(req, &buf->rb_allreqs, rl_all) {
		rc = rpcrdma_req_setup(r_xprt, req);
		if (rc)
			return rc;
	}
	return 0;
}

static void rpcrdma_req_reset(struct rpcrdma_req *req)
{
	/* Credits are valid for only one connection */
	req->rl_slot.rq_cong = 0;

	rpcrdma_regbuf_free(req->rl_rdmabuf);
	req->rl_rdmabuf = NULL;

	rpcrdma_regbuf_dma_unmap(req->rl_sendbuf);
	rpcrdma_regbuf_dma_unmap(req->rl_recvbuf);

	frwr_reset(req);
}

/* ASSUMPTION: the rb_allreqs list is stable for the duration,
 * and thus can be walked without holding rb_lock. Eg. the
 * caller is holding the transport send lock to exclude
 * device removal or disconnection.
 */
static void rpcrdma_reqs_reset(struct rpcrdma_xprt *r_xprt)
{
	struct rpcrdma_buffer *buf = &r_xprt->rx_buf;
	struct rpcrdma_req *req;

	list_for_each_entry(req, &buf->rb_allreqs, rl_all)
		rpcrdma_req_reset(req);
}

static noinline
struct rpcrdma_rep *rpcrdma_rep_create(struct rpcrdma_xprt *r_xprt,
				       bool temp)
{
	struct rpcrdma_buffer *buf = &r_xprt->rx_buf;
	struct rpcrdma_rep *rep;

	rep = kzalloc(sizeof(*rep), XPRTRDMA_GFP_FLAGS);
	if (rep == NULL)
		goto out;

	rep->rr_rdmabuf = rpcrdma_regbuf_alloc(r_xprt->rx_ep->re_inline_recv,
					       DMA_FROM_DEVICE);
	if (!rep->rr_rdmabuf)
		goto out_free;

	rep->rr_cid.ci_completion_id =
		atomic_inc_return(&r_xprt->rx_ep->re_completion_ids);

	xdr_buf_init(&rep->rr_hdrbuf, rdmab_data(rep->rr_rdmabuf),
		     rdmab_length(rep->rr_rdmabuf));
	rep->rr_cqe.done = rpcrdma_wc_receive;
	rep->rr_rxprt = r_xprt;
	rep->rr_recv_wr.next = NULL;
	rep->rr_recv_wr.wr_cqe = &rep->rr_cqe;
	rep->rr_recv_wr.sg_list = &rep->rr_rdmabuf->rg_iov;
	rep->rr_recv_wr.num_sge = 1;
	rep->rr_temp = temp;

	spin_lock(&buf->rb_lock);
	list_add(&rep->rr_all, &buf->rb_all_reps);
	spin_unlock(&buf->rb_lock);
	return rep;

out_free:
	kfree(rep);
out:
	return NULL;
}

static void rpcrdma_rep_free(struct rpcrdma_rep *rep)
{
	rpcrdma_regbuf_free(rep->rr_rdmabuf);
	kfree(rep);
}

static void rpcrdma_rep_destroy(struct rpcrdma_rep *rep)
{
	struct rpcrdma_buffer *buf = &rep->rr_rxprt->rx_buf;

	spin_lock(&buf->rb_lock);
	list_del(&rep->rr_all);
	spin_unlock(&buf->rb_lock);

	rpcrdma_rep_free(rep);
}

static struct rpcrdma_rep *rpcrdma_rep_get_locked(struct rpcrdma_buffer *buf)
{
	struct llist_node *node;

	/* Calls to llist_del_first are required to be serialized */
	node = llist_del_first(&buf->rb_free_reps);
	if (!node)
		return NULL;
	return llist_entry(node, struct rpcrdma_rep, rr_node);
}

/**
 * rpcrdma_rep_put - Release rpcrdma_rep back to free list
 * @buf: buffer pool
 * @rep: rep to release
 *
 */
void rpcrdma_rep_put(struct rpcrdma_buffer *buf, struct rpcrdma_rep *rep)
{
	llist_add(&rep->rr_node, &buf->rb_free_reps);
}

/* Caller must ensure the QP is quiescent (RQ is drained) before
 * invoking this function, to guarantee rb_all_reps is not
 * changing.
 */
static void rpcrdma_reps_unmap(struct rpcrdma_xprt *r_xprt)
{
	struct rpcrdma_buffer *buf = &r_xprt->rx_buf;
	struct rpcrdma_rep *rep;

	list_for_each_entry(rep, &buf->rb_all_reps, rr_all) {
		rpcrdma_regbuf_dma_unmap(rep->rr_rdmabuf);
		rep->rr_temp = true;	/* Mark this rep for destruction */
	}
}

static void rpcrdma_reps_destroy(struct rpcrdma_buffer *buf)
{
	struct rpcrdma_rep *rep;

	spin_lock(&buf->rb_lock);
	while ((rep = list_first_entry_or_null(&buf->rb_all_reps,
					       struct rpcrdma_rep,
					       rr_all)) != NULL) {
		list_del(&rep->rr_all);
		spin_unlock(&buf->rb_lock);

		rpcrdma_rep_free(rep);

		spin_lock(&buf->rb_lock);
	}
	spin_unlock(&buf->rb_lock);
}

/**
 * rpcrdma_buffer_create - Create initial set of req/rep objects
 * @r_xprt: transport instance to (re)initialize
 *
 * Returns zero on success, otherwise a negative errno.
 */
int rpcrdma_buffer_create(struct rpcrdma_xprt *r_xprt)
{
	struct rpcrdma_buffer *buf = &r_xprt->rx_buf;
	int i, rc;

	buf->rb_bc_srv_max_requests = 0;
	spin_lock_init(&buf->rb_lock);
	INIT_LIST_HEAD(&buf->rb_mrs);
	INIT_LIST_HEAD(&buf->rb_all_mrs);
	INIT_WORK(&buf->rb_refresh_worker, rpcrdma_mr_refresh_worker);

	INIT_LIST_HEAD(&buf->rb_send_bufs);
	INIT_LIST_HEAD(&buf->rb_allreqs);
	INIT_LIST_HEAD(&buf->rb_all_reps);

	rc = -ENOMEM;
	for (i = 0; i < r_xprt->rx_xprt.max_reqs; i++) {
		struct rpcrdma_req *req;

		req = rpcrdma_req_create(r_xprt,
					 RPCRDMA_V1_DEF_INLINE_SIZE * 2);
		if (!req)
			goto out;
		list_add(&req->rl_list, &buf->rb_send_bufs);
	}

	init_llist_head(&buf->rb_free_reps);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
out:
	rpcrdma_buffer_destroy(buf);
	return rc;
}

<<<<<<< HEAD
/*
 * Unregister and destroy buffer memory. Need to deal with
 * partial initialization, so it's callable from failed create.
 * Must be called before destroying endpoint, as registrations
 * reference it.
=======
/**
 * rpcrdma_req_destroy - Destroy an rpcrdma_req object
 * @req: unused object to be destroyed
 *
 * Relies on caller holding the transport send lock to protect
 * removing req->rl_all from buf->rb_all_reqs safely.
 */
void rpcrdma_req_destroy(struct rpcrdma_req *req)
{
	struct rpcrdma_mr *mr;

	list_del(&req->rl_all);

	while ((mr = rpcrdma_mr_pop(&req->rl_free_mrs))) {
		struct rpcrdma_buffer *buf = &mr->mr_xprt->rx_buf;

		spin_lock(&buf->rb_lock);
		list_del(&mr->mr_all);
		spin_unlock(&buf->rb_lock);

		frwr_mr_release(mr);
	}

	rpcrdma_regbuf_free(req->rl_recvbuf);
	rpcrdma_regbuf_free(req->rl_sendbuf);
	rpcrdma_regbuf_free(req->rl_rdmabuf);
	kfree(req);
}

/**
 * rpcrdma_mrs_destroy - Release all of a transport's MRs
 * @r_xprt: controlling transport instance
 *
 * Relies on caller holding the transport send lock to protect
 * removing mr->mr_list from req->rl_free_mrs safely.
 */
static void rpcrdma_mrs_destroy(struct rpcrdma_xprt *r_xprt)
{
	struct rpcrdma_buffer *buf = &r_xprt->rx_buf;
	struct rpcrdma_mr *mr;

	cancel_work_sync(&buf->rb_refresh_worker);

	spin_lock(&buf->rb_lock);
	while ((mr = list_first_entry_or_null(&buf->rb_all_mrs,
					      struct rpcrdma_mr,
					      mr_all)) != NULL) {
		list_del(&mr->mr_list);
		list_del(&mr->mr_all);
		spin_unlock(&buf->rb_lock);

		frwr_mr_release(mr);

		spin_lock(&buf->rb_lock);
	}
	spin_unlock(&buf->rb_lock);
}

/**
 * rpcrdma_buffer_destroy - Release all hw resources
 * @buf: root control block for resources
 *
 * ORDERING: relies on a prior rpcrdma_xprt_drain :
 * - No more Send or Receive completions can occur
 * - All MRs, reps, and reqs are returned to their free lists
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
void
rpcrdma_buffer_destroy(struct rpcrdma_buffer *buf)
{
<<<<<<< HEAD
	int rc, i;
	struct rpcrdma_ia *ia = rdmab_to_ia(buf);
	struct rpcrdma_mw *r;

	/* clean up in reverse order from create
	 *   1.  recv mr memory (mr free, then kfree)
	 *   1a. bind mw memory
	 *   2.  send mr memory (mr free, then kfree)
	 *   3.  padding (if any) [moved to rpcrdma_ep_destroy]
	 *   4.  arrays
	 */
	dprintk("RPC:       %s: entering\n", __func__);

	for (i = 0; i < buf->rb_max_requests; i++) {
		if (buf->rb_recv_bufs && buf->rb_recv_bufs[i]) {
			rpcrdma_deregister_internal(ia,
					buf->rb_recv_bufs[i]->rr_handle,
					&buf->rb_recv_bufs[i]->rr_iov);
			kfree(buf->rb_recv_bufs[i]);
		}
		if (buf->rb_send_bufs && buf->rb_send_bufs[i]) {
			while (!list_empty(&buf->rb_mws)) {
				r = list_entry(buf->rb_mws.next,
					struct rpcrdma_mw, mw_list);
				list_del(&r->mw_list);
				switch (ia->ri_memreg_strategy) {
				case RPCRDMA_FRMR:
					rc = ib_dereg_mr(r->r.frmr.fr_mr);
					if (rc)
						dprintk("RPC:       %s:"
							" ib_dereg_mr"
							" failed %i\n",
							__func__, rc);
					ib_free_fast_reg_page_list(r->r.frmr.fr_pgl);
					break;
				case RPCRDMA_MTHCAFMR:
					rc = ib_dealloc_fmr(r->r.fmr);
					if (rc)
						dprintk("RPC:       %s:"
							" ib_dealloc_fmr"
							" failed %i\n",
							__func__, rc);
					break;
				case RPCRDMA_MEMWINDOWS_ASYNC:
				case RPCRDMA_MEMWINDOWS:
					rc = ib_dealloc_mw(r->r.mw);
					if (rc)
						dprintk("RPC:       %s:"
							" ib_dealloc_mw"
							" failed %i\n",
							__func__, rc);
					break;
				default:
					break;
				}
			}
			rpcrdma_deregister_internal(ia,
					buf->rb_send_bufs[i]->rl_handle,
					&buf->rb_send_bufs[i]->rl_iov);
			kfree(buf->rb_send_bufs[i]);
		}
	}

	kfree(buf->rb_pool);
}

/*
 * Get a set of request/reply buffers.
 *
 * Reply buffer (if needed) is attached to send buffer upon return.
 * Rule:
 *    rb_send_index and rb_recv_index MUST always be pointing to the
 *    *next* available buffer (non-NULL). They are incremented after
 *    removing buffers, and decremented *before* returning them.
=======
	rpcrdma_reps_destroy(buf);

	while (!list_empty(&buf->rb_send_bufs)) {
		struct rpcrdma_req *req;

		req = list_first_entry(&buf->rb_send_bufs,
				       struct rpcrdma_req, rl_list);
		list_del(&req->rl_list);
		rpcrdma_req_destroy(req);
	}
}

/**
 * rpcrdma_mr_get - Allocate an rpcrdma_mr object
 * @r_xprt: controlling transport
 *
 * Returns an initialized rpcrdma_mr or NULL if no free
 * rpcrdma_mr objects are available.
 */
struct rpcrdma_mr *
rpcrdma_mr_get(struct rpcrdma_xprt *r_xprt)
{
	struct rpcrdma_buffer *buf = &r_xprt->rx_buf;
	struct rpcrdma_mr *mr;

	spin_lock(&buf->rb_lock);
	mr = rpcrdma_mr_pop(&buf->rb_mrs);
	spin_unlock(&buf->rb_lock);
	return mr;
}

/**
 * rpcrdma_reply_put - Put reply buffers back into pool
 * @buffers: buffer pool
 * @req: object to return
 *
 */
void rpcrdma_reply_put(struct rpcrdma_buffer *buffers, struct rpcrdma_req *req)
{
	if (req->rl_reply) {
		rpcrdma_rep_put(buffers, req->rl_reply);
		req->rl_reply = NULL;
	}
}

/**
 * rpcrdma_buffer_get - Get a request buffer
 * @buffers: Buffer pool from which to obtain a buffer
 *
 * Returns a fresh rpcrdma_req, or NULL if none are available.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
struct rpcrdma_req *
rpcrdma_buffer_get(struct rpcrdma_buffer *buffers)
{
	struct rpcrdma_req *req;
<<<<<<< HEAD
	unsigned long flags;
	int i;
	struct rpcrdma_mw *r;

	spin_lock_irqsave(&buffers->rb_lock, flags);
	if (buffers->rb_send_index == buffers->rb_max_requests) {
		spin_unlock_irqrestore(&buffers->rb_lock, flags);
		dprintk("RPC:       %s: out of request buffers\n", __func__);
		return ((struct rpcrdma_req *)NULL);
	}

	req = buffers->rb_send_bufs[buffers->rb_send_index];
	if (buffers->rb_send_index < buffers->rb_recv_index) {
		dprintk("RPC:       %s: %d extra receives outstanding (ok)\n",
			__func__,
			buffers->rb_recv_index - buffers->rb_send_index);
		req->rl_reply = NULL;
	} else {
		req->rl_reply = buffers->rb_recv_bufs[buffers->rb_recv_index];
		buffers->rb_recv_bufs[buffers->rb_recv_index++] = NULL;
	}
	buffers->rb_send_bufs[buffers->rb_send_index++] = NULL;
	if (!list_empty(&buffers->rb_mws)) {
		i = RPCRDMA_MAX_SEGS - 1;
		do {
			r = list_entry(buffers->rb_mws.next,
					struct rpcrdma_mw, mw_list);
			list_del(&r->mw_list);
			req->rl_segments[i].mr_chunk.rl_mw = r;
		} while (--i >= 0);
	}
	spin_unlock_irqrestore(&buffers->rb_lock, flags);
	return req;
}

/*
 * Put request/reply buffers back into pool.
 * Pre-decrement counter/array index.
 */
void
rpcrdma_buffer_put(struct rpcrdma_req *req)
{
	struct rpcrdma_buffer *buffers = req->rl_buffer;
	struct rpcrdma_ia *ia = rdmab_to_ia(buffers);
	int i;
	unsigned long flags;

	BUG_ON(req->rl_nchunks != 0);
	spin_lock_irqsave(&buffers->rb_lock, flags);
	buffers->rb_send_bufs[--buffers->rb_send_index] = req;
	req->rl_niovs = 0;
	if (req->rl_reply) {
		buffers->rb_recv_bufs[--buffers->rb_recv_index] = req->rl_reply;
		init_waitqueue_head(&req->rl_reply->rr_unbind);
		req->rl_reply->rr_func = NULL;
		req->rl_reply = NULL;
	}
	switch (ia->ri_memreg_strategy) {
	case RPCRDMA_FRMR:
	case RPCRDMA_MTHCAFMR:
	case RPCRDMA_MEMWINDOWS_ASYNC:
	case RPCRDMA_MEMWINDOWS:
		/*
		 * Cycle mw's back in reverse order, and "spin" them.
		 * This delays and scrambles reuse as much as possible.
		 */
		i = 1;
		do {
			struct rpcrdma_mw **mw;
			mw = &req->rl_segments[i].mr_chunk.rl_mw;
			list_add_tail(&(*mw)->mw_list, &buffers->rb_mws);
			*mw = NULL;
		} while (++i < RPCRDMA_MAX_SEGS);
		list_add_tail(&req->rl_segments[0].mr_chunk.rl_mw->mw_list,
					&buffers->rb_mws);
		req->rl_segments[0].mr_chunk.rl_mw = NULL;
		break;
	default:
		break;
	}
	spin_unlock_irqrestore(&buffers->rb_lock, flags);
}

/*
 * Recover reply buffers from pool.
 * This happens when recovering from error conditions.
 * Post-increment counter/array index.
 */
void
rpcrdma_recv_buffer_get(struct rpcrdma_req *req)
{
	struct rpcrdma_buffer *buffers = req->rl_buffer;
	unsigned long flags;

	if (req->rl_iov.length == 0)	/* special case xprt_rdma_allocate() */
		buffers = ((struct rpcrdma_req *) buffers)->rl_buffer;
	spin_lock_irqsave(&buffers->rb_lock, flags);
	if (buffers->rb_recv_index < buffers->rb_max_requests) {
		req->rl_reply = buffers->rb_recv_bufs[buffers->rb_recv_index];
		buffers->rb_recv_bufs[buffers->rb_recv_index++] = NULL;
	}
	spin_unlock_irqrestore(&buffers->rb_lock, flags);
}

/*
 * Put reply buffers back into pool when not attached to
 * request. This happens in error conditions, and when
 * aborting unbinds. Pre-decrement counter/array index.
 */
void
rpcrdma_recv_buffer_put(struct rpcrdma_rep *rep)
{
	struct rpcrdma_buffer *buffers = rep->rr_buffer;
	unsigned long flags;

	rep->rr_func = NULL;
	spin_lock_irqsave(&buffers->rb_lock, flags);
	buffers->rb_recv_bufs[--buffers->rb_recv_index] = rep;
	spin_unlock_irqrestore(&buffers->rb_lock, flags);
}

/*
 * Wrappers for internal-use kmalloc memory registration, used by buffer code.
 */

int
rpcrdma_register_internal(struct rpcrdma_ia *ia, void *va, int len,
				struct ib_mr **mrp, struct ib_sge *iov)
{
	struct ib_phys_buf ipb;
	struct ib_mr *mr;
	int rc;

	/*
	 * All memory passed here was kmalloc'ed, therefore phys-contiguous.
	 */
	iov->addr = ib_dma_map_single(ia->ri_id->device,
			va, len, DMA_BIDIRECTIONAL);
	iov->length = len;

	if (ia->ri_have_dma_lkey) {
		*mrp = NULL;
		iov->lkey = ia->ri_dma_lkey;
		return 0;
	} else if (ia->ri_bind_mem != NULL) {
		*mrp = NULL;
		iov->lkey = ia->ri_bind_mem->lkey;
		return 0;
	}

	ipb.addr = iov->addr;
	ipb.size = iov->length;
	mr = ib_reg_phys_mr(ia->ri_pd, &ipb, 1,
			IB_ACCESS_LOCAL_WRITE, &iov->addr);

	dprintk("RPC:       %s: phys convert: 0x%llx "
			"registered 0x%llx length %d\n",
			__func__, (unsigned long long)ipb.addr,
			(unsigned long long)iov->addr, len);

	if (IS_ERR(mr)) {
		*mrp = NULL;
		rc = PTR_ERR(mr);
		dprintk("RPC:       %s: failed with %i\n", __func__, rc);
	} else {
		*mrp = mr;
		iov->lkey = mr->lkey;
		rc = 0;
	}

	return rc;
}

int
rpcrdma_deregister_internal(struct rpcrdma_ia *ia,
				struct ib_mr *mr, struct ib_sge *iov)
{
	int rc;

	ib_dma_unmap_single(ia->ri_id->device,
			iov->addr, iov->length, DMA_BIDIRECTIONAL);

	if (NULL == mr)
		return 0;

	rc = ib_dereg_mr(mr);
	if (rc)
		dprintk("RPC:       %s: ib_dereg_mr failed %i\n", __func__, rc);
	return rc;
}

/*
 * Wrappers for chunk registration, shared by read/write chunk code.
 */

static void
rpcrdma_map_one(struct rpcrdma_ia *ia, struct rpcrdma_mr_seg *seg, int writing)
{
	seg->mr_dir = writing ? DMA_FROM_DEVICE : DMA_TO_DEVICE;
	seg->mr_dmalen = seg->mr_len;
	if (seg->mr_page)
		seg->mr_dma = ib_dma_map_page(ia->ri_id->device,
				seg->mr_page, offset_in_page(seg->mr_offset),
				seg->mr_dmalen, seg->mr_dir);
	else
		seg->mr_dma = ib_dma_map_single(ia->ri_id->device,
				seg->mr_offset,
				seg->mr_dmalen, seg->mr_dir);
	if (ib_dma_mapping_error(ia->ri_id->device, seg->mr_dma)) {
		dprintk("RPC:       %s: mr_dma %llx mr_offset %p mr_dma_len %zu\n",
			__func__,
			(unsigned long long)seg->mr_dma,
			seg->mr_offset, seg->mr_dmalen);
	}
}

static void
rpcrdma_unmap_one(struct rpcrdma_ia *ia, struct rpcrdma_mr_seg *seg)
{
	if (seg->mr_page)
		ib_dma_unmap_page(ia->ri_id->device,
				seg->mr_dma, seg->mr_dmalen, seg->mr_dir);
	else
		ib_dma_unmap_single(ia->ri_id->device,
				seg->mr_dma, seg->mr_dmalen, seg->mr_dir);
}

static int
rpcrdma_register_frmr_external(struct rpcrdma_mr_seg *seg,
			int *nsegs, int writing, struct rpcrdma_ia *ia,
			struct rpcrdma_xprt *r_xprt)
{
	struct rpcrdma_mr_seg *seg1 = seg;
	struct ib_send_wr invalidate_wr, frmr_wr, *bad_wr, *post_wr;

	u8 key;
	int len, pageoff;
	int i, rc;
	int seg_len;
	u64 pa;
	int page_no;

	pageoff = offset_in_page(seg1->mr_offset);
	seg1->mr_offset -= pageoff;	/* start of page */
	seg1->mr_len += pageoff;
	len = -pageoff;
	if (*nsegs > RPCRDMA_MAX_DATA_SEGS)
		*nsegs = RPCRDMA_MAX_DATA_SEGS;
	for (page_no = i = 0; i < *nsegs;) {
		rpcrdma_map_one(ia, seg, writing);
		pa = seg->mr_dma;
		for (seg_len = seg->mr_len; seg_len > 0; seg_len -= PAGE_SIZE) {
			seg1->mr_chunk.rl_mw->r.frmr.fr_pgl->
				page_list[page_no++] = pa;
			pa += PAGE_SIZE;
		}
		len += seg->mr_len;
		++seg;
		++i;
		/* Check for holes */
		if ((i < *nsegs && offset_in_page(seg->mr_offset)) ||
		    offset_in_page((seg-1)->mr_offset + (seg-1)->mr_len))
			break;
	}
	dprintk("RPC:       %s: Using frmr %p to map %d segments\n",
		__func__, seg1->mr_chunk.rl_mw, i);

	if (unlikely(seg1->mr_chunk.rl_mw->r.frmr.state == FRMR_IS_VALID)) {
		dprintk("RPC:       %s: frmr %x left valid, posting invalidate.\n",
			__func__,
			seg1->mr_chunk.rl_mw->r.frmr.fr_mr->rkey);
		/* Invalidate before using. */
		memset(&invalidate_wr, 0, sizeof invalidate_wr);
		invalidate_wr.wr_id = (unsigned long)(void *)seg1->mr_chunk.rl_mw;
		invalidate_wr.next = &frmr_wr;
		invalidate_wr.opcode = IB_WR_LOCAL_INV;
		invalidate_wr.send_flags = IB_SEND_SIGNALED;
		invalidate_wr.ex.invalidate_rkey =
			seg1->mr_chunk.rl_mw->r.frmr.fr_mr->rkey;
		DECR_CQCOUNT(&r_xprt->rx_ep);
		post_wr = &invalidate_wr;
	} else
		post_wr = &frmr_wr;

	/* Bump the key */
	key = (u8)(seg1->mr_chunk.rl_mw->r.frmr.fr_mr->rkey & 0x000000FF);
	ib_update_fast_reg_key(seg1->mr_chunk.rl_mw->r.frmr.fr_mr, ++key);

	/* Prepare FRMR WR */
	memset(&frmr_wr, 0, sizeof frmr_wr);
	frmr_wr.wr_id = (unsigned long)(void *)seg1->mr_chunk.rl_mw;
	frmr_wr.opcode = IB_WR_FAST_REG_MR;
	frmr_wr.send_flags = IB_SEND_SIGNALED;
	frmr_wr.wr.fast_reg.iova_start = seg1->mr_dma;
	frmr_wr.wr.fast_reg.page_list = seg1->mr_chunk.rl_mw->r.frmr.fr_pgl;
	frmr_wr.wr.fast_reg.page_list_len = page_no;
	frmr_wr.wr.fast_reg.page_shift = PAGE_SHIFT;
	frmr_wr.wr.fast_reg.length = page_no << PAGE_SHIFT;
	BUG_ON(frmr_wr.wr.fast_reg.length < len);
	frmr_wr.wr.fast_reg.access_flags = (writing ?
				IB_ACCESS_REMOTE_WRITE | IB_ACCESS_LOCAL_WRITE :
				IB_ACCESS_REMOTE_READ);
	frmr_wr.wr.fast_reg.rkey = seg1->mr_chunk.rl_mw->r.frmr.fr_mr->rkey;
	DECR_CQCOUNT(&r_xprt->rx_ep);

	rc = ib_post_send(ia->ri_id->qp, post_wr, &bad_wr);

	if (rc) {
		dprintk("RPC:       %s: failed ib_post_send for register,"
			" status %i\n", __func__, rc);
		while (i--)
			rpcrdma_unmap_one(ia, --seg);
	} else {
		seg1->mr_rkey = seg1->mr_chunk.rl_mw->r.frmr.fr_mr->rkey;
		seg1->mr_base = seg1->mr_dma + pageoff;
		seg1->mr_nsegs = i;
		seg1->mr_len = len;
	}
	*nsegs = i;
	return rc;
}

static int
rpcrdma_deregister_frmr_external(struct rpcrdma_mr_seg *seg,
			struct rpcrdma_ia *ia, struct rpcrdma_xprt *r_xprt)
{
	struct rpcrdma_mr_seg *seg1 = seg;
	struct ib_send_wr invalidate_wr, *bad_wr;
	int rc;

	while (seg1->mr_nsegs--)
		rpcrdma_unmap_one(ia, seg++);

	memset(&invalidate_wr, 0, sizeof invalidate_wr);
	invalidate_wr.wr_id = (unsigned long)(void *)seg1->mr_chunk.rl_mw;
	invalidate_wr.opcode = IB_WR_LOCAL_INV;
	invalidate_wr.send_flags = IB_SEND_SIGNALED;
	invalidate_wr.ex.invalidate_rkey = seg1->mr_chunk.rl_mw->r.frmr.fr_mr->rkey;
	DECR_CQCOUNT(&r_xprt->rx_ep);

	rc = ib_post_send(ia->ri_id->qp, &invalidate_wr, &bad_wr);
	if (rc)
		dprintk("RPC:       %s: failed ib_post_send for invalidate,"
			" status %i\n", __func__, rc);
	return rc;
}

static int
rpcrdma_register_fmr_external(struct rpcrdma_mr_seg *seg,
			int *nsegs, int writing, struct rpcrdma_ia *ia)
{
	struct rpcrdma_mr_seg *seg1 = seg;
	u64 physaddrs[RPCRDMA_MAX_DATA_SEGS];
	int len, pageoff, i, rc;

	pageoff = offset_in_page(seg1->mr_offset);
	seg1->mr_offset -= pageoff;	/* start of page */
	seg1->mr_len += pageoff;
	len = -pageoff;
	if (*nsegs > RPCRDMA_MAX_DATA_SEGS)
		*nsegs = RPCRDMA_MAX_DATA_SEGS;
	for (i = 0; i < *nsegs;) {
		rpcrdma_map_one(ia, seg, writing);
		physaddrs[i] = seg->mr_dma;
		len += seg->mr_len;
		++seg;
		++i;
		/* Check for holes */
		if ((i < *nsegs && offset_in_page(seg->mr_offset)) ||
		    offset_in_page((seg-1)->mr_offset + (seg-1)->mr_len))
			break;
	}
	rc = ib_map_phys_fmr(seg1->mr_chunk.rl_mw->r.fmr,
				physaddrs, i, seg1->mr_dma);
	if (rc) {
		dprintk("RPC:       %s: failed ib_map_phys_fmr "
			"%u@0x%llx+%i (%d)... status %i\n", __func__,
			len, (unsigned long long)seg1->mr_dma,
			pageoff, i, rc);
		while (i--)
			rpcrdma_unmap_one(ia, --seg);
	} else {
		seg1->mr_rkey = seg1->mr_chunk.rl_mw->r.fmr->rkey;
		seg1->mr_base = seg1->mr_dma + pageoff;
		seg1->mr_nsegs = i;
		seg1->mr_len = len;
	}
	*nsegs = i;
	return rc;
}

static int
rpcrdma_deregister_fmr_external(struct rpcrdma_mr_seg *seg,
			struct rpcrdma_ia *ia)
{
	struct rpcrdma_mr_seg *seg1 = seg;
	LIST_HEAD(l);
	int rc;

	list_add(&seg1->mr_chunk.rl_mw->r.fmr->list, &l);
	rc = ib_unmap_fmr(&l);
	while (seg1->mr_nsegs--)
		rpcrdma_unmap_one(ia, seg++);
	if (rc)
		dprintk("RPC:       %s: failed ib_unmap_fmr,"
			" status %i\n", __func__, rc);
	return rc;
}

static int
rpcrdma_register_memwin_external(struct rpcrdma_mr_seg *seg,
			int *nsegs, int writing, struct rpcrdma_ia *ia,
			struct rpcrdma_xprt *r_xprt)
{
	int mem_priv = (writing ? IB_ACCESS_REMOTE_WRITE :
				  IB_ACCESS_REMOTE_READ);
	struct ib_mw_bind param;
	int rc;

	*nsegs = 1;
	rpcrdma_map_one(ia, seg, writing);
	param.mr = ia->ri_bind_mem;
	param.wr_id = 0ULL;	/* no send cookie */
	param.addr = seg->mr_dma;
	param.length = seg->mr_len;
	param.send_flags = 0;
	param.mw_access_flags = mem_priv;

	DECR_CQCOUNT(&r_xprt->rx_ep);
	rc = ib_bind_mw(ia->ri_id->qp, seg->mr_chunk.rl_mw->r.mw, &param);
	if (rc) {
		dprintk("RPC:       %s: failed ib_bind_mw "
			"%u@0x%llx status %i\n",
			__func__, seg->mr_len,
			(unsigned long long)seg->mr_dma, rc);
		rpcrdma_unmap_one(ia, seg);
	} else {
		seg->mr_rkey = seg->mr_chunk.rl_mw->r.mw->rkey;
		seg->mr_base = param.addr;
		seg->mr_nsegs = 1;
	}
	return rc;
}

static int
rpcrdma_deregister_memwin_external(struct rpcrdma_mr_seg *seg,
			struct rpcrdma_ia *ia,
			struct rpcrdma_xprt *r_xprt, void **r)
{
	struct ib_mw_bind param;
	LIST_HEAD(l);
	int rc;

	BUG_ON(seg->mr_nsegs != 1);
	param.mr = ia->ri_bind_mem;
	param.addr = 0ULL;	/* unbind */
	param.length = 0;
	param.mw_access_flags = 0;
	if (*r) {
		param.wr_id = (u64) (unsigned long) *r;
		param.send_flags = IB_SEND_SIGNALED;
		INIT_CQCOUNT(&r_xprt->rx_ep);
	} else {
		param.wr_id = 0ULL;
		param.send_flags = 0;
		DECR_CQCOUNT(&r_xprt->rx_ep);
	}
	rc = ib_bind_mw(ia->ri_id->qp, seg->mr_chunk.rl_mw->r.mw, &param);
	rpcrdma_unmap_one(ia, seg);
	if (rc)
		dprintk("RPC:       %s: failed ib_(un)bind_mw,"
			" status %i\n", __func__, rc);
	else
		*r = NULL;	/* will upcall on completion */
	return rc;
}

static int
rpcrdma_register_default_external(struct rpcrdma_mr_seg *seg,
			int *nsegs, int writing, struct rpcrdma_ia *ia)
{
	int mem_priv = (writing ? IB_ACCESS_REMOTE_WRITE :
				  IB_ACCESS_REMOTE_READ);
	struct rpcrdma_mr_seg *seg1 = seg;
	struct ib_phys_buf ipb[RPCRDMA_MAX_DATA_SEGS];
	int len, i, rc = 0;

	if (*nsegs > RPCRDMA_MAX_DATA_SEGS)
		*nsegs = RPCRDMA_MAX_DATA_SEGS;
	for (len = 0, i = 0; i < *nsegs;) {
		rpcrdma_map_one(ia, seg, writing);
		ipb[i].addr = seg->mr_dma;
		ipb[i].size = seg->mr_len;
		len += seg->mr_len;
		++seg;
		++i;
		/* Check for holes */
		if ((i < *nsegs && offset_in_page(seg->mr_offset)) ||
		    offset_in_page((seg-1)->mr_offset+(seg-1)->mr_len))
			break;
	}
	seg1->mr_base = seg1->mr_dma;
	seg1->mr_chunk.rl_mr = ib_reg_phys_mr(ia->ri_pd,
				ipb, i, mem_priv, &seg1->mr_base);
	if (IS_ERR(seg1->mr_chunk.rl_mr)) {
		rc = PTR_ERR(seg1->mr_chunk.rl_mr);
		dprintk("RPC:       %s: failed ib_reg_phys_mr "
			"%u@0x%llx (%d)... status %i\n",
			__func__, len,
			(unsigned long long)seg1->mr_dma, i, rc);
		while (i--)
			rpcrdma_unmap_one(ia, --seg);
	} else {
		seg1->mr_rkey = seg1->mr_chunk.rl_mr->rkey;
		seg1->mr_nsegs = i;
		seg1->mr_len = len;
	}
	*nsegs = i;
	return rc;
}

static int
rpcrdma_deregister_default_external(struct rpcrdma_mr_seg *seg,
			struct rpcrdma_ia *ia)
{
	struct rpcrdma_mr_seg *seg1 = seg;
	int rc;

	rc = ib_dereg_mr(seg1->mr_chunk.rl_mr);
	seg1->mr_chunk.rl_mr = NULL;
	while (seg1->mr_nsegs--)
		rpcrdma_unmap_one(ia, seg++);
	if (rc)
		dprintk("RPC:       %s: failed ib_dereg_mr,"
			" status %i\n", __func__, rc);
	return rc;
}

int
rpcrdma_register_external(struct rpcrdma_mr_seg *seg,
			int nsegs, int writing, struct rpcrdma_xprt *r_xprt)
{
	struct rpcrdma_ia *ia = &r_xprt->rx_ia;
	int rc = 0;

	switch (ia->ri_memreg_strategy) {

#if RPCRDMA_PERSISTENT_REGISTRATION
	case RPCRDMA_ALLPHYSICAL:
		rpcrdma_map_one(ia, seg, writing);
		seg->mr_rkey = ia->ri_bind_mem->rkey;
		seg->mr_base = seg->mr_dma;
		seg->mr_nsegs = 1;
		nsegs = 1;
		break;
#endif

	/* Registration using frmr registration */
	case RPCRDMA_FRMR:
		rc = rpcrdma_register_frmr_external(seg, &nsegs, writing, ia, r_xprt);
		break;

	/* Registration using fmr memory registration */
	case RPCRDMA_MTHCAFMR:
		rc = rpcrdma_register_fmr_external(seg, &nsegs, writing, ia);
		break;

	/* Registration using memory windows */
	case RPCRDMA_MEMWINDOWS_ASYNC:
	case RPCRDMA_MEMWINDOWS:
		rc = rpcrdma_register_memwin_external(seg, &nsegs, writing, ia, r_xprt);
		break;

	/* Default registration each time */
	default:
		rc = rpcrdma_register_default_external(seg, &nsegs, writing, ia);
		break;
	}
	if (rc)
		return -1;

	return nsegs;
}

int
rpcrdma_deregister_external(struct rpcrdma_mr_seg *seg,
		struct rpcrdma_xprt *r_xprt, void *r)
{
	struct rpcrdma_ia *ia = &r_xprt->rx_ia;
	int nsegs = seg->mr_nsegs, rc;

	switch (ia->ri_memreg_strategy) {

#if RPCRDMA_PERSISTENT_REGISTRATION
	case RPCRDMA_ALLPHYSICAL:
		BUG_ON(nsegs != 1);
		rpcrdma_unmap_one(ia, seg);
		rc = 0;
		break;
#endif

	case RPCRDMA_FRMR:
		rc = rpcrdma_deregister_frmr_external(seg, ia, r_xprt);
		break;

	case RPCRDMA_MTHCAFMR:
		rc = rpcrdma_deregister_fmr_external(seg, ia);
		break;

	case RPCRDMA_MEMWINDOWS_ASYNC:
	case RPCRDMA_MEMWINDOWS:
		rc = rpcrdma_deregister_memwin_external(seg, ia, r_xprt, &r);
		break;

	default:
		rc = rpcrdma_deregister_default_external(seg, ia);
		break;
	}
	if (r) {
		struct rpcrdma_rep *rep = r;
		void (*func)(struct rpcrdma_rep *) = rep->rr_func;
		rep->rr_func = NULL;
		func(rep);	/* dereg done, callback now */
	}
	return nsegs;
}

/*
 * Prepost any receive buffer, then post send.
 *
 * Receive buffer is donated to hardware, reclaimed upon recv completion.
 */
int
rpcrdma_ep_post(struct rpcrdma_ia *ia,
		struct rpcrdma_ep *ep,
		struct rpcrdma_req *req)
{
	struct ib_send_wr send_wr, *send_wr_fail;
	struct rpcrdma_rep *rep = req->rl_reply;
	int rc;

	if (rep) {
		rc = rpcrdma_ep_post_recv(ia, ep, rep);
		if (rc)
			goto out;
		req->rl_reply = NULL;
	}

	send_wr.next = NULL;
	send_wr.wr_id = 0ULL;	/* no send cookie */
	send_wr.sg_list = req->rl_send_iov;
	send_wr.num_sge = req->rl_niovs;
	send_wr.opcode = IB_WR_SEND;
	if (send_wr.num_sge == 4)	/* no need to sync any pad (constant) */
		ib_dma_sync_single_for_device(ia->ri_id->device,
			req->rl_send_iov[3].addr, req->rl_send_iov[3].length,
			DMA_TO_DEVICE);
	ib_dma_sync_single_for_device(ia->ri_id->device,
		req->rl_send_iov[1].addr, req->rl_send_iov[1].length,
		DMA_TO_DEVICE);
	ib_dma_sync_single_for_device(ia->ri_id->device,
		req->rl_send_iov[0].addr, req->rl_send_iov[0].length,
		DMA_TO_DEVICE);

	if (DECR_CQCOUNT(ep) > 0)
		send_wr.send_flags = 0;
	else { /* Provider must take a send completion every now and then */
		INIT_CQCOUNT(ep);
		send_wr.send_flags = IB_SEND_SIGNALED;
	}

	rc = ib_post_send(ia->ri_id->qp, &send_wr, &send_wr_fail);
	if (rc)
		dprintk("RPC:       %s: ib_post_send returned %i\n", __func__,
			rc);
out:
	return rc;
}

/*
 * (Re)post a receive buffer.
 */
int
rpcrdma_ep_post_recv(struct rpcrdma_ia *ia,
		     struct rpcrdma_ep *ep,
		     struct rpcrdma_rep *rep)
{
	struct ib_recv_wr recv_wr, *recv_wr_fail;
	int rc;

	recv_wr.next = NULL;
	recv_wr.wr_id = (u64) (unsigned long) rep;
	recv_wr.sg_list = &rep->rr_iov;
	recv_wr.num_sge = 1;

	ib_dma_sync_single_for_cpu(ia->ri_id->device,
		rep->rr_iov.addr, rep->rr_iov.length, DMA_BIDIRECTIONAL);

	DECR_CQCOUNT(ep);
	rc = ib_post_recv(ia->ri_id->qp, &recv_wr, &recv_wr_fail);

	if (rc)
		dprintk("RPC:       %s: ib_post_recv returned %i\n", __func__,
			rc);
	return rc;
=======

	spin_lock(&buffers->rb_lock);
	req = list_first_entry_or_null(&buffers->rb_send_bufs,
				       struct rpcrdma_req, rl_list);
	if (req)
		list_del_init(&req->rl_list);
	spin_unlock(&buffers->rb_lock);
	return req;
}

/**
 * rpcrdma_buffer_put - Put request/reply buffers back into pool
 * @buffers: buffer pool
 * @req: object to return
 *
 */
void rpcrdma_buffer_put(struct rpcrdma_buffer *buffers, struct rpcrdma_req *req)
{
	rpcrdma_reply_put(buffers, req);

	spin_lock(&buffers->rb_lock);
	list_add(&req->rl_list, &buffers->rb_send_bufs);
	spin_unlock(&buffers->rb_lock);
}

/* Returns a pointer to a rpcrdma_regbuf object, or NULL.
 *
 * xprtrdma uses a regbuf for posting an outgoing RDMA SEND, or for
 * receiving the payload of RDMA RECV operations. During Long Calls
 * or Replies they may be registered externally via frwr_map.
 */
static struct rpcrdma_regbuf *
rpcrdma_regbuf_alloc(size_t size, enum dma_data_direction direction)
{
	struct rpcrdma_regbuf *rb;

	rb = kmalloc(sizeof(*rb), XPRTRDMA_GFP_FLAGS);
	if (!rb)
		return NULL;
	rb->rg_data = kmalloc(size, XPRTRDMA_GFP_FLAGS);
	if (!rb->rg_data) {
		kfree(rb);
		return NULL;
	}

	rb->rg_device = NULL;
	rb->rg_direction = direction;
	rb->rg_iov.length = size;
	return rb;
}

/**
 * rpcrdma_regbuf_realloc - re-allocate a SEND/RECV buffer
 * @rb: regbuf to reallocate
 * @size: size of buffer to be allocated, in bytes
 * @flags: GFP flags
 *
 * Returns true if reallocation was successful. If false is
 * returned, @rb is left untouched.
 */
bool rpcrdma_regbuf_realloc(struct rpcrdma_regbuf *rb, size_t size, gfp_t flags)
{
	void *buf;

	buf = kmalloc(size, flags);
	if (!buf)
		return false;

	rpcrdma_regbuf_dma_unmap(rb);
	kfree(rb->rg_data);

	rb->rg_data = buf;
	rb->rg_iov.length = size;
	return true;
}

/**
 * __rpcrdma_regbuf_dma_map - DMA-map a regbuf
 * @r_xprt: controlling transport instance
 * @rb: regbuf to be mapped
 *
 * Returns true if the buffer is now DMA mapped to @r_xprt's device
 */
bool __rpcrdma_regbuf_dma_map(struct rpcrdma_xprt *r_xprt,
			      struct rpcrdma_regbuf *rb)
{
	struct ib_device *device = r_xprt->rx_ep->re_id->device;

	if (rb->rg_direction == DMA_NONE)
		return false;

	rb->rg_iov.addr = ib_dma_map_single(device, rdmab_data(rb),
					    rdmab_length(rb), rb->rg_direction);
	if (ib_dma_mapping_error(device, rdmab_addr(rb))) {
		trace_xprtrdma_dma_maperr(rdmab_addr(rb));
		return false;
	}

	rb->rg_device = device;
	rb->rg_iov.lkey = r_xprt->rx_ep->re_pd->local_dma_lkey;
	return true;
}

static void rpcrdma_regbuf_dma_unmap(struct rpcrdma_regbuf *rb)
{
	if (!rb)
		return;

	if (!rpcrdma_regbuf_is_mapped(rb))
		return;

	ib_dma_unmap_single(rb->rg_device, rdmab_addr(rb), rdmab_length(rb),
			    rb->rg_direction);
	rb->rg_device = NULL;
}

static void rpcrdma_regbuf_free(struct rpcrdma_regbuf *rb)
{
	rpcrdma_regbuf_dma_unmap(rb);
	if (rb)
		kfree(rb->rg_data);
	kfree(rb);
}

/**
 * rpcrdma_post_recvs - Refill the Receive Queue
 * @r_xprt: controlling transport instance
 * @needed: current credit grant
 * @temp: mark Receive buffers to be deleted after one use
 *
 */
void rpcrdma_post_recvs(struct rpcrdma_xprt *r_xprt, int needed, bool temp)
{
	struct rpcrdma_buffer *buf = &r_xprt->rx_buf;
	struct rpcrdma_ep *ep = r_xprt->rx_ep;
	struct ib_recv_wr *wr, *bad_wr;
	struct rpcrdma_rep *rep;
	int count, rc;

	rc = 0;
	count = 0;

	if (likely(ep->re_receive_count > needed))
		goto out;
	needed -= ep->re_receive_count;
	if (!temp)
		needed += RPCRDMA_MAX_RECV_BATCH;

	if (atomic_inc_return(&ep->re_receiving) > 1)
		goto out;

	/* fast path: all needed reps can be found on the free list */
	wr = NULL;
	while (needed) {
		rep = rpcrdma_rep_get_locked(buf);
		if (rep && rep->rr_temp) {
			rpcrdma_rep_destroy(rep);
			continue;
		}
		if (!rep)
			rep = rpcrdma_rep_create(r_xprt, temp);
		if (!rep)
			break;
		if (!rpcrdma_regbuf_dma_map(r_xprt, rep->rr_rdmabuf)) {
			rpcrdma_rep_put(buf, rep);
			break;
		}

		rep->rr_cid.ci_queue_id = ep->re_attr.recv_cq->res.id;
		trace_xprtrdma_post_recv(&rep->rr_cid);
		rep->rr_recv_wr.next = wr;
		wr = &rep->rr_recv_wr;
		--needed;
		++count;
	}
	if (!wr)
		goto out;

	rc = ib_post_recv(ep->re_id->qp, wr,
			  (const struct ib_recv_wr **)&bad_wr);
	if (rc) {
		trace_xprtrdma_post_recvs_err(r_xprt, rc);
		for (wr = bad_wr; wr;) {
			struct rpcrdma_rep *rep;

			rep = container_of(wr, struct rpcrdma_rep, rr_recv_wr);
			wr = wr->next;
			rpcrdma_rep_put(buf, rep);
			--count;
		}
	}
	if (atomic_dec_return(&ep->re_receiving) > 0)
		complete(&ep->re_done);

out:
	trace_xprtrdma_post_recvs(r_xprt, count);
	ep->re_receive_count += count;
	return;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
