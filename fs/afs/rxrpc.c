<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Maintain an RxRPC server socket to do AFS communications through
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

#include <linux/slab.h>
#include <net/sock.h>
#include <net/af_rxrpc.h>
#include <rxrpc/packet.h>
#include "internal.h"
#include "afs_cm.h"

static struct socket *afs_socket; /* my RxRPC socket */
static struct workqueue_struct *afs_async_calls;
static atomic_t afs_outstanding_calls;
static atomic_t afs_outstanding_skbs;

static void afs_wake_up_call_waiter(struct afs_call *);
static int afs_wait_for_call_to_complete(struct afs_call *);
static void afs_wake_up_async_call(struct afs_call *);
static int afs_dont_wait_for_call_to_complete(struct afs_call *);
static void afs_process_async_call(struct work_struct *);
static void afs_rx_interceptor(struct sock *, unsigned long, struct sk_buff *);
static int afs_deliver_cm_op_id(struct afs_call *, struct sk_buff *, bool);

/* synchronous call management */
const struct afs_wait_mode afs_sync_call = {
	.rx_wakeup	= afs_wake_up_call_waiter,
	.wait		= afs_wait_for_call_to_complete,
};

/* asynchronous call management */
const struct afs_wait_mode afs_async_call = {
	.rx_wakeup	= afs_wake_up_async_call,
	.wait		= afs_dont_wait_for_call_to_complete,
};

/* asynchronous incoming call management */
static const struct afs_wait_mode afs_async_incoming_call = {
	.rx_wakeup	= afs_wake_up_async_call,
};
=======
 */

#include <linux/slab.h>
#include <linux/sched/signal.h>

#include <net/sock.h>
#include <net/af_rxrpc.h>
#include "internal.h"
#include "afs_cm.h"
#include "protocol_yfs.h"
#define RXRPC_TRACE_ONLY_DEFINE_ENUMS
#include <trace/events/rxrpc.h>

struct workqueue_struct *afs_async_calls;

static void afs_wake_up_call_waiter(struct sock *, struct rxrpc_call *, unsigned long);
static void afs_wake_up_async_call(struct sock *, struct rxrpc_call *, unsigned long);
static void afs_process_async_call(struct work_struct *);
static void afs_rx_new_call(struct sock *, struct rxrpc_call *, unsigned long);
static void afs_rx_discard_new_call(struct rxrpc_call *, unsigned long);
static int afs_deliver_cm_op_id(struct afs_call *);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* asynchronous incoming call initial processing */
static const struct afs_call_type afs_RXCMxxxx = {
	.name		= "CB.xxxx",
	.deliver	= afs_deliver_cm_op_id,
<<<<<<< HEAD
	.abort_to_error	= afs_abort_to_error,
};

static void afs_collect_incoming_call(struct work_struct *);

static struct sk_buff_head afs_incoming_calls;
static DECLARE_WORK(afs_collect_incoming_call_work, afs_collect_incoming_call);

=======
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * open an RxRPC socket and bind it to be a server for callback notifications
 * - the socket is left in blocking mode and non-blocking ops use MSG_DONTWAIT
 */
<<<<<<< HEAD
int afs_open_socket(void)
=======
int afs_open_socket(struct afs_net *net)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct sockaddr_rxrpc srx;
	struct socket *socket;
	int ret;

	_enter("");

<<<<<<< HEAD
	skb_queue_head_init(&afs_incoming_calls);

	afs_async_calls = create_singlethread_workqueue("kafsd");
	if (!afs_async_calls) {
		_leave(" = -ENOMEM [wq]");
		return -ENOMEM;
	}

	ret = sock_create_kern(AF_RXRPC, SOCK_DGRAM, PF_INET, &socket);
	if (ret < 0) {
		destroy_workqueue(afs_async_calls);
		_leave(" = %d [socket]", ret);
		return ret;
	}
=======
	ret = sock_create_kern(net->net, AF_RXRPC, SOCK_DGRAM, PF_INET6, &socket);
	if (ret < 0)
		goto error_1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	socket->sk->sk_allocation = GFP_NOFS;

	/* bind the callback manager's address to make this a server socket */
<<<<<<< HEAD
	srx.srx_family			= AF_RXRPC;
	srx.srx_service			= CM_SERVICE;
	srx.transport_type		= SOCK_DGRAM;
	srx.transport_len		= sizeof(srx.transport.sin);
	srx.transport.sin.sin_family	= AF_INET;
	srx.transport.sin.sin_port	= htons(AFS_CM_PORT);
	memset(&srx.transport.sin.sin_addr, 0,
	       sizeof(srx.transport.sin.sin_addr));

	ret = kernel_bind(socket, (struct sockaddr *) &srx, sizeof(srx));
	if (ret < 0) {
		sock_release(socket);
		destroy_workqueue(afs_async_calls);
		_leave(" = %d [bind]", ret);
		return ret;
	}

	rxrpc_kernel_intercept_rx_messages(socket, afs_rx_interceptor);

	afs_socket = socket;
	_leave(" = 0");
	return 0;
=======
	memset(&srx, 0, sizeof(srx));
	srx.srx_family			= AF_RXRPC;
	srx.srx_service			= CM_SERVICE;
	srx.transport_type		= SOCK_DGRAM;
	srx.transport_len		= sizeof(srx.transport.sin6);
	srx.transport.sin6.sin6_family	= AF_INET6;
	srx.transport.sin6.sin6_port	= htons(AFS_CM_PORT);

	ret = rxrpc_sock_set_min_security_level(socket->sk,
						RXRPC_SECURITY_ENCRYPT);
	if (ret < 0)
		goto error_2;

	ret = kernel_bind(socket, (struct sockaddr *) &srx, sizeof(srx));
	if (ret == -EADDRINUSE) {
		srx.transport.sin6.sin6_port = 0;
		ret = kernel_bind(socket, (struct sockaddr *) &srx, sizeof(srx));
	}
	if (ret < 0)
		goto error_2;

	srx.srx_service = YFS_CM_SERVICE;
	ret = kernel_bind(socket, (struct sockaddr *) &srx, sizeof(srx));
	if (ret < 0)
		goto error_2;

	/* Ideally, we'd turn on service upgrade here, but we can't because
	 * OpenAFS is buggy and leaks the userStatus field from packet to
	 * packet and between FS packets and CB packets - so if we try to do an
	 * upgrade on an FS packet, OpenAFS will leak that into the CB packet
	 * it sends back to us.
	 */

	rxrpc_kernel_new_call_notification(socket, afs_rx_new_call,
					   afs_rx_discard_new_call);

	ret = kernel_listen(socket, INT_MAX);
	if (ret < 0)
		goto error_2;

	net->socket = socket;
	afs_charge_preallocation(&net->charge_preallocation_work);
	_leave(" = 0");
	return 0;

error_2:
	sock_release(socket);
error_1:
	_leave(" = %d", ret);
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * close the RxRPC socket AFS was using
 */
<<<<<<< HEAD
void afs_close_socket(void)
{
	_enter("");

	sock_release(afs_socket);

	_debug("dework");
	destroy_workqueue(afs_async_calls);

	ASSERTCMP(atomic_read(&afs_outstanding_skbs), ==, 0);
	ASSERTCMP(atomic_read(&afs_outstanding_calls), ==, 0);
=======
void afs_close_socket(struct afs_net *net)
{
	_enter("");

	kernel_listen(net->socket, 0);
	flush_workqueue(afs_async_calls);

	if (net->spare_incoming_call) {
		afs_put_call(net->spare_incoming_call);
		net->spare_incoming_call = NULL;
	}

	_debug("outstanding %u", atomic_read(&net->nr_outstanding_calls));
	wait_var_event(&net->nr_outstanding_calls,
		       !atomic_read(&net->nr_outstanding_calls));
	_debug("no outstanding calls");

	kernel_sock_shutdown(net->socket, SHUT_RDWR);
	flush_workqueue(afs_async_calls);
	sock_release(net->socket);

	_debug("dework");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	_leave("");
}

/*
<<<<<<< HEAD
 * note that the data in a socket buffer is now delivered and that the buffer
 * should be freed
 */
static void afs_data_delivered(struct sk_buff *skb)
{
	if (!skb) {
		_debug("DLVR NULL [%d]", atomic_read(&afs_outstanding_skbs));
		dump_stack();
	} else {
		_debug("DLVR %p{%u} [%d]",
		       skb, skb->mark, atomic_read(&afs_outstanding_skbs));
		if (atomic_dec_return(&afs_outstanding_skbs) == -1)
			BUG();
		rxrpc_kernel_data_delivered(skb);
	}
}

/*
 * free a socket buffer
 */
static void afs_free_skb(struct sk_buff *skb)
{
	if (!skb) {
		_debug("FREE NULL [%d]", atomic_read(&afs_outstanding_skbs));
		dump_stack();
	} else {
		_debug("FREE %p{%u} [%d]",
		       skb, skb->mark, atomic_read(&afs_outstanding_skbs));
		if (atomic_dec_return(&afs_outstanding_skbs) == -1)
			BUG();
		rxrpc_kernel_free_skb(skb);
	}
}

/*
 * free a call
 */
static void afs_free_call(struct afs_call *call)
{
	_debug("DONE %p{%s} [%d]",
	       call, call->type->name, atomic_read(&afs_outstanding_calls));
	if (atomic_dec_return(&afs_outstanding_calls) == -1)
		BUG();

	ASSERTCMP(call->rxcall, ==, NULL);
	ASSERT(!work_pending(&call->async_work));
	ASSERT(skb_queue_empty(&call->rx_queue));
	ASSERT(call->type->name != NULL);

	kfree(call->request);
	kfree(call);
=======
 * Allocate a call.
 */
static struct afs_call *afs_alloc_call(struct afs_net *net,
				       const struct afs_call_type *type,
				       gfp_t gfp)
{
	struct afs_call *call;
	int o;

	call = kzalloc(sizeof(*call), gfp);
	if (!call)
		return NULL;

	call->type = type;
	call->net = net;
	call->debug_id = atomic_inc_return(&rxrpc_debug_id);
	refcount_set(&call->ref, 1);
	INIT_WORK(&call->async_work, afs_process_async_call);
	init_waitqueue_head(&call->waitq);
	spin_lock_init(&call->state_lock);
	call->iter = &call->def_iter;

	o = atomic_inc_return(&net->nr_outstanding_calls);
	trace_afs_call(call->debug_id, afs_call_trace_alloc, 1, o,
		       __builtin_return_address(0));
	return call;
}

/*
 * Dispose of a reference on a call.
 */
void afs_put_call(struct afs_call *call)
{
	struct afs_net *net = call->net;
	unsigned int debug_id = call->debug_id;
	bool zero;
	int r, o;

	zero = __refcount_dec_and_test(&call->ref, &r);
	o = atomic_read(&net->nr_outstanding_calls);
	trace_afs_call(debug_id, afs_call_trace_put, r - 1, o,
		       __builtin_return_address(0));

	if (zero) {
		ASSERT(!work_pending(&call->async_work));
		ASSERT(call->type->name != NULL);

		rxrpc_kernel_put_peer(call->peer);

		if (call->rxcall) {
			rxrpc_kernel_shutdown_call(net->socket, call->rxcall);
			rxrpc_kernel_put_call(net->socket, call->rxcall);
			call->rxcall = NULL;
		}
		if (call->type->destructor)
			call->type->destructor(call);

		afs_unuse_server_notime(call->net, call->server, afs_server_trace_put_call);
		kfree(call->request);

		trace_afs_call(call->debug_id, afs_call_trace_free, 0, o,
			       __builtin_return_address(0));
		kfree(call);

		o = atomic_dec_return(&net->nr_outstanding_calls);
		if (o == 0)
			wake_up_var(&net->nr_outstanding_calls);
	}
}

static struct afs_call *afs_get_call(struct afs_call *call,
				     enum afs_call_trace why)
{
	int r;

	__refcount_inc(&call->ref, &r);

	trace_afs_call(call->debug_id, why, r + 1,
		       atomic_read(&call->net->nr_outstanding_calls),
		       __builtin_return_address(0));
	return call;
}

/*
 * Queue the call for actual work.
 */
static void afs_queue_call_work(struct afs_call *call)
{
	if (call->type->work) {
		INIT_WORK(&call->work, call->type->work);

		afs_get_call(call, afs_call_trace_work);
		if (!queue_work(afs_wq, &call->work))
			afs_put_call(call);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * allocate a call with flat request and reply buffers
 */
<<<<<<< HEAD
struct afs_call *afs_alloc_flat_call(const struct afs_call_type *type,
				     size_t request_size, size_t reply_size)
{
	struct afs_call *call;

	call = kzalloc(sizeof(*call), GFP_NOFS);
	if (!call)
		goto nomem_call;

	_debug("CALL %p{%s} [%d]",
	       call, type->name, atomic_read(&afs_outstanding_calls));
	atomic_inc(&afs_outstanding_calls);

	call->type = type;
	call->request_size = request_size;
	call->reply_max = reply_size;

	if (request_size) {
=======
struct afs_call *afs_alloc_flat_call(struct afs_net *net,
				     const struct afs_call_type *type,
				     size_t request_size, size_t reply_max)
{
	struct afs_call *call;

	call = afs_alloc_call(net, type, GFP_NOFS);
	if (!call)
		goto nomem_call;

	if (request_size) {
		call->request_size = request_size;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		call->request = kmalloc(request_size, GFP_NOFS);
		if (!call->request)
			goto nomem_free;
	}

<<<<<<< HEAD
	if (reply_size) {
		call->buffer = kmalloc(reply_size, GFP_NOFS);
=======
	if (reply_max) {
		call->reply_max = reply_max;
		call->buffer = kmalloc(reply_max, GFP_NOFS);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!call->buffer)
			goto nomem_free;
	}

<<<<<<< HEAD
	init_waitqueue_head(&call->waitq);
	skb_queue_head_init(&call->rx_queue);
	return call;

nomem_free:
	afs_free_call(call);
=======
	afs_extract_to_buf(call, call->reply_max);
	call->operation_ID = type->op;
	init_waitqueue_head(&call->waitq);
	return call;

nomem_free:
	afs_put_call(call);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
nomem_call:
	return NULL;
}

/*
 * clean up a call with flat buffer
 */
void afs_flat_call_destructor(struct afs_call *call)
{
	_enter("");

	kfree(call->request);
	call->request = NULL;
	kfree(call->buffer);
	call->buffer = NULL;
}

/*
<<<<<<< HEAD
 * attach the data from a bunch of pages on an inode to a call
 */
static int afs_send_pages(struct afs_call *call, struct msghdr *msg,
			  struct kvec *iov)
{
	struct page *pages[8];
	unsigned count, n, loop, offset, to;
	pgoff_t first = call->first, last = call->last;
	int ret;

	_enter("");

	offset = call->first_offset;
	call->first_offset = 0;

	do {
		_debug("attach %lx-%lx", first, last);

		count = last - first + 1;
		if (count > ARRAY_SIZE(pages))
			count = ARRAY_SIZE(pages);
		n = find_get_pages_contig(call->mapping, first, count, pages);
		ASSERTCMP(n, ==, count);

		loop = 0;
		do {
			msg->msg_flags = 0;
			to = PAGE_SIZE;
			if (first + loop >= last)
				to = call->last_to;
			else
				msg->msg_flags = MSG_MORE;
			iov->iov_base = kmap(pages[loop]) + offset;
			iov->iov_len = to - offset;
			offset = 0;

			_debug("- range %u-%u%s",
			       offset, to, msg->msg_flags ? " [more]" : "");
			msg->msg_iov = (struct iovec *) iov;
			msg->msg_iovlen = 1;

			/* have to change the state *before* sending the last
			 * packet as RxRPC might give us the reply before it
			 * returns from sending the request */
			if (first + loop >= last)
				call->state = AFS_CALL_AWAIT_REPLY;
			ret = rxrpc_kernel_send_data(call->rxcall, msg,
						     to - offset);
			kunmap(pages[loop]);
			if (ret < 0)
				break;
		} while (++loop < count);
		first += count;

		for (loop = 0; loop < count; loop++)
			put_page(pages[loop]);
		if (ret < 0)
			break;
	} while (first <= last);

	_leave(" = %d", ret);
	return ret;
}

/*
 * initiate a call
 */
int afs_make_call(struct in_addr *addr, struct afs_call *call, gfp_t gfp,
		  const struct afs_wait_mode *wait_mode)
{
	struct sockaddr_rxrpc srx;
	struct rxrpc_call *rxcall;
	struct msghdr msg;
	struct kvec iov[1];
	int ret;
	struct sk_buff *skb;

	_enter("%x,{%d},", addr->s_addr, ntohs(call->port));
=======
 * Advance the AFS call state when the RxRPC call ends the transmit phase.
 */
static void afs_notify_end_request_tx(struct sock *sock,
				      struct rxrpc_call *rxcall,
				      unsigned long call_user_ID)
{
	struct afs_call *call = (struct afs_call *)call_user_ID;

	afs_set_call_state(call, AFS_CALL_CL_REQUESTING, AFS_CALL_CL_AWAIT_REPLY);
}

/*
 * Initiate a call and synchronously queue up the parameters for dispatch.  Any
 * error is stored into the call struct, which the caller must check for.
 */
void afs_make_call(struct afs_call *call, gfp_t gfp)
{
	struct rxrpc_call *rxcall;
	struct msghdr msg;
	struct kvec iov[1];
	size_t len;
	s64 tx_total_len;
	int ret;

	_enter(",{%pISp+%u},", rxrpc_kernel_remote_addr(call->peer), call->service_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ASSERT(call->type != NULL);
	ASSERT(call->type->name != NULL);

	_debug("____MAKE %p{%s,%x} [%d]____",
	       call, call->type->name, key_serial(call->key),
<<<<<<< HEAD
	       atomic_read(&afs_outstanding_calls));

	call->wait_mode = wait_mode;
	INIT_WORK(&call->async_work, afs_process_async_call);

	memset(&srx, 0, sizeof(srx));
	srx.srx_family = AF_RXRPC;
	srx.srx_service = call->service_id;
	srx.transport_type = SOCK_DGRAM;
	srx.transport_len = sizeof(srx.transport.sin);
	srx.transport.sin.sin_family = AF_INET;
	srx.transport.sin.sin_port = call->port;
	memcpy(&srx.transport.sin.sin_addr, addr, 4);

	/* create a call */
	rxcall = rxrpc_kernel_begin_call(afs_socket, &srx, call->key,
					 (unsigned long) call, gfp);
	call->key = NULL;
	if (IS_ERR(rxcall)) {
		ret = PTR_ERR(rxcall);
=======
	       atomic_read(&call->net->nr_outstanding_calls));

	trace_afs_make_call(call);

	/* Work out the length we're going to transmit.  This is awkward for
	 * calls such as FS.StoreData where there's an extra injection of data
	 * after the initial fixed part.
	 */
	tx_total_len = call->request_size;
	if (call->write_iter)
		tx_total_len += iov_iter_count(call->write_iter);

	/* If the call is going to be asynchronous, we need an extra ref for
	 * the call to hold itself so the caller need not hang on to its ref.
	 */
	if (call->async) {
		afs_get_call(call, afs_call_trace_get);
		call->drop_ref = true;
	}

	/* create a call */
	rxcall = rxrpc_kernel_begin_call(call->net->socket, call->peer, call->key,
					 (unsigned long)call,
					 tx_total_len,
					 call->max_lifespan,
					 gfp,
					 (call->async ?
					  afs_wake_up_async_call :
					  afs_wake_up_call_waiter),
					 call->service_id,
					 call->upgrade,
					 (call->intr ? RXRPC_PREINTERRUPTIBLE :
					  RXRPC_UNINTERRUPTIBLE),
					 call->debug_id);
	if (IS_ERR(rxcall)) {
		ret = PTR_ERR(rxcall);
		call->error = ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto error_kill_call;
	}

	call->rxcall = rxcall;
<<<<<<< HEAD
=======
	call->issue_time = ktime_get_real();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* send the request */
	iov[0].iov_base	= call->request;
	iov[0].iov_len	= call->request_size;

	msg.msg_name		= NULL;
	msg.msg_namelen		= 0;
<<<<<<< HEAD
	msg.msg_iov		= (struct iovec *) iov;
	msg.msg_iovlen		= 1;
	msg.msg_control		= NULL;
	msg.msg_controllen	= 0;
	msg.msg_flags		= (call->send_pages ? MSG_MORE : 0);

	/* have to change the state *before* sending the last packet as RxRPC
	 * might give us the reply before it returns from sending the
	 * request */
	if (!call->send_pages)
		call->state = AFS_CALL_AWAIT_REPLY;
	ret = rxrpc_kernel_send_data(rxcall, &msg, call->request_size);
	if (ret < 0)
		goto error_do_abort;

	if (call->send_pages) {
		ret = afs_send_pages(call, &msg, iov);
=======
	iov_iter_kvec(&msg.msg_iter, ITER_SOURCE, iov, 1, call->request_size);
	msg.msg_control		= NULL;
	msg.msg_controllen	= 0;
	msg.msg_flags		= MSG_WAITALL | (call->write_iter ? MSG_MORE : 0);

	ret = rxrpc_kernel_send_data(call->net->socket, rxcall,
				     &msg, call->request_size,
				     afs_notify_end_request_tx);
	if (ret < 0)
		goto error_do_abort;

	if (call->write_iter) {
		msg.msg_iter = *call->write_iter;
		msg.msg_flags &= ~MSG_MORE;
		trace_afs_send_data(call, &msg);

		ret = rxrpc_kernel_send_data(call->net->socket,
					     call->rxcall, &msg,
					     iov_iter_count(&msg.msg_iter),
					     afs_notify_end_request_tx);
		*call->write_iter = msg.msg_iter;

		trace_afs_sent_data(call, &msg, ret);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (ret < 0)
			goto error_do_abort;
	}

<<<<<<< HEAD
	/* at this point, an async call may no longer exist as it may have
	 * already completed */
	return wait_mode->wait(call);

error_do_abort:
	rxrpc_kernel_abort_call(rxcall, RX_USER_ABORT);
	while ((skb = skb_dequeue(&call->rx_queue)))
		afs_free_skb(skb);
	rxrpc_kernel_end_call(rxcall);
	call->rxcall = NULL;
error_kill_call:
	call->type->destructor(call);
	afs_free_call(call);
	_leave(" = %d", ret);
	return ret;
}

/*
 * handles intercepted messages that were arriving in the socket's Rx queue
 * - called with the socket receive queue lock held to ensure message ordering
 * - called with softirqs disabled
 */
static void afs_rx_interceptor(struct sock *sk, unsigned long user_call_ID,
			       struct sk_buff *skb)
{
	struct afs_call *call = (struct afs_call *) user_call_ID;

	_enter("%p,,%u", call, skb->mark);

	_debug("ICPT %p{%u} [%d]",
	       skb, skb->mark, atomic_read(&afs_outstanding_skbs));

	ASSERTCMP(sk, ==, afs_socket->sk);
	atomic_inc(&afs_outstanding_skbs);

	if (!call) {
		/* its an incoming call for our callback service */
		skb_queue_tail(&afs_incoming_calls, skb);
		queue_work(afs_wq, &afs_collect_incoming_call_work);
	} else {
		/* route the messages directly to the appropriate call */
		skb_queue_tail(&call->rx_queue, skb);
		call->wait_mode->rx_wakeup(call);
	}

	_leave("");
=======
	/* Note that at this point, we may have received the reply or an abort
	 * - and an asynchronous call may already have completed.
	 *
	 * afs_wait_for_call_to_complete(call)
	 * must be called to synchronously clean up.
	 */
	return;

error_do_abort:
	if (ret != -ECONNABORTED) {
		rxrpc_kernel_abort_call(call->net->socket, rxcall,
					RX_USER_ABORT, ret,
					afs_abort_send_data_error);
	} else {
		len = 0;
		iov_iter_kvec(&msg.msg_iter, ITER_DEST, NULL, 0, 0);
		rxrpc_kernel_recv_data(call->net->socket, rxcall,
				       &msg.msg_iter, &len, false,
				       &call->abort_code, &call->service_id);
		call->responded = true;
	}
	call->error = ret;
	trace_afs_call_done(call);
error_kill_call:
	if (call->type->done)
		call->type->done(call);

	/* We need to dispose of the extra ref we grabbed for an async call.
	 * The call, however, might be queued on afs_async_calls and we need to
	 * make sure we don't get any more notifications that might requeue it.
	 */
	if (call->rxcall)
		rxrpc_kernel_shutdown_call(call->net->socket, call->rxcall);
	if (call->async) {
		if (cancel_work_sync(&call->async_work))
			afs_put_call(call);
		afs_set_call_complete(call, ret, 0);
	}

	call->error = ret;
	call->state = AFS_CALL_COMPLETE;
	_leave(" = %d", ret);
}

/*
 * Log remote abort codes that indicate that we have a protocol disagreement
 * with the server.
 */
static void afs_log_error(struct afs_call *call, s32 remote_abort)
{
	static int max = 0;
	const char *msg;
	int m;

	switch (remote_abort) {
	case RX_EOF:		 msg = "unexpected EOF";	break;
	case RXGEN_CC_MARSHAL:	 msg = "client marshalling";	break;
	case RXGEN_CC_UNMARSHAL: msg = "client unmarshalling";	break;
	case RXGEN_SS_MARSHAL:	 msg = "server marshalling";	break;
	case RXGEN_SS_UNMARSHAL: msg = "server unmarshalling";	break;
	case RXGEN_DECODE:	 msg = "opcode decode";		break;
	case RXGEN_SS_XDRFREE:	 msg = "server XDR cleanup";	break;
	case RXGEN_CC_XDRFREE:	 msg = "client XDR cleanup";	break;
	case -32:		 msg = "insufficient data";	break;
	default:
		return;
	}

	m = max;
	if (m < 3) {
		max = m + 1;
		pr_notice("kAFS: Peer reported %s failure on %s [%pISp]\n",
			  msg, call->type->name,
			  rxrpc_kernel_remote_addr(call->peer));
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * deliver messages to a call
 */
static void afs_deliver_to_call(struct afs_call *call)
{
<<<<<<< HEAD
	struct sk_buff *skb;
	bool last;
	u32 abort_code;
	int ret;

	_enter("");

	while ((call->state == AFS_CALL_AWAIT_REPLY ||
		call->state == AFS_CALL_AWAIT_OP_ID ||
		call->state == AFS_CALL_AWAIT_REQUEST ||
		call->state == AFS_CALL_AWAIT_ACK) &&
	       (skb = skb_dequeue(&call->rx_queue))) {
		switch (skb->mark) {
		case RXRPC_SKB_MARK_DATA:
			_debug("Rcv DATA");
			last = rxrpc_kernel_is_data_last(skb);
			ret = call->type->deliver(call, skb, last);
			switch (ret) {
			case 0:
				if (last &&
				    call->state == AFS_CALL_AWAIT_REPLY)
					call->state = AFS_CALL_COMPLETE;
				break;
			case -ENOTCONN:
				abort_code = RX_CALL_DEAD;
				goto do_abort;
			case -ENOTSUPP:
				abort_code = RX_INVALID_OPERATION;
				goto do_abort;
			default:
				abort_code = RXGEN_CC_UNMARSHAL;
				if (call->state != AFS_CALL_AWAIT_REPLY)
					abort_code = RXGEN_SS_UNMARSHAL;
			do_abort:
				rxrpc_kernel_abort_call(call->rxcall,
							abort_code);
				call->error = ret;
				call->state = AFS_CALL_ERROR;
				break;
			}
			afs_data_delivered(skb);
			skb = NULL;
			continue;
		case RXRPC_SKB_MARK_FINAL_ACK:
			_debug("Rcv ACK");
			call->state = AFS_CALL_COMPLETE;
			break;
		case RXRPC_SKB_MARK_BUSY:
			_debug("Rcv BUSY");
			call->error = -EBUSY;
			call->state = AFS_CALL_BUSY;
			break;
		case RXRPC_SKB_MARK_REMOTE_ABORT:
			abort_code = rxrpc_kernel_get_abort_code(skb);
			call->error = call->type->abort_to_error(abort_code);
			call->state = AFS_CALL_ABORTED;
			_debug("Rcv ABORT %u -> %d", abort_code, call->error);
			break;
		case RXRPC_SKB_MARK_NET_ERROR:
			call->error = -rxrpc_kernel_get_error_number(skb);
			call->state = AFS_CALL_ERROR;
			_debug("Rcv NET ERROR %d", call->error);
			break;
		case RXRPC_SKB_MARK_LOCAL_ERROR:
			call->error = -rxrpc_kernel_get_error_number(skb);
			call->state = AFS_CALL_ERROR;
			_debug("Rcv LOCAL ERROR %d", call->error);
			break;
		default:
			BUG();
			break;
		}

		afs_free_skb(skb);
	}

	/* make sure the queue is empty if the call is done with (we might have
	 * aborted the call early because of an unmarshalling error) */
	if (call->state >= AFS_CALL_COMPLETE) {
		while ((skb = skb_dequeue(&call->rx_queue)))
			afs_free_skb(skb);
		if (call->incoming) {
			rxrpc_kernel_end_call(call->rxcall);
			call->rxcall = NULL;
			call->type->destructor(call);
			afs_free_call(call);
		}
	}

	_leave("");
}

/*
 * wait synchronously for a call to complete
 */
static int afs_wait_for_call_to_complete(struct afs_call *call)
{
	struct sk_buff *skb;
	int ret;

	DECLARE_WAITQUEUE(myself, current);

	_enter("");

	add_wait_queue(&call->waitq, &myself);
	for (;;) {
		set_current_state(TASK_INTERRUPTIBLE);

		/* deliver any messages that are in the queue */
		if (!skb_queue_empty(&call->rx_queue)) {
			__set_current_state(TASK_RUNNING);
			afs_deliver_to_call(call);
			continue;
		}

		ret = call->error;
		if (call->state >= AFS_CALL_COMPLETE)
			break;
		ret = -EINTR;
		if (signal_pending(current))
			break;
		schedule();
	}

	remove_wait_queue(&call->waitq, &myself);
	__set_current_state(TASK_RUNNING);

	/* kill the call */
	if (call->state < AFS_CALL_COMPLETE) {
		_debug("call incomplete");
		rxrpc_kernel_abort_call(call->rxcall, RX_CALL_DEAD);
		while ((skb = skb_dequeue(&call->rx_queue)))
			afs_free_skb(skb);
	}

	_debug("call complete");
	rxrpc_kernel_end_call(call->rxcall);
	call->rxcall = NULL;
	call->type->destructor(call);
	afs_free_call(call);
	_leave(" = %d", ret);
	return ret;
=======
	enum afs_call_state state;
	size_t len;
	u32 abort_code, remote_abort = 0;
	int ret;

	_enter("%s", call->type->name);

	while (state = READ_ONCE(call->state),
	       state == AFS_CALL_CL_AWAIT_REPLY ||
	       state == AFS_CALL_SV_AWAIT_OP_ID ||
	       state == AFS_CALL_SV_AWAIT_REQUEST ||
	       state == AFS_CALL_SV_AWAIT_ACK
	       ) {
		if (state == AFS_CALL_SV_AWAIT_ACK) {
			len = 0;
			iov_iter_kvec(&call->def_iter, ITER_DEST, NULL, 0, 0);
			ret = rxrpc_kernel_recv_data(call->net->socket,
						     call->rxcall, &call->def_iter,
						     &len, false, &remote_abort,
						     &call->service_id);
			trace_afs_receive_data(call, &call->def_iter, false, ret);

			if (ret == -EINPROGRESS || ret == -EAGAIN)
				return;
			if (ret < 0 || ret == 1) {
				if (ret == 1)
					ret = 0;
				goto call_complete;
			}
			return;
		}

		ret = call->type->deliver(call);
		state = READ_ONCE(call->state);
		if (ret == 0 && call->unmarshalling_error)
			ret = -EBADMSG;
		switch (ret) {
		case 0:
			call->responded = true;
			afs_queue_call_work(call);
			if (state == AFS_CALL_CL_PROC_REPLY) {
				if (call->op)
					set_bit(AFS_SERVER_FL_MAY_HAVE_CB,
						&call->op->server->flags);
				goto call_complete;
			}
			ASSERTCMP(state, >, AFS_CALL_CL_PROC_REPLY);
			goto done;
		case -EINPROGRESS:
		case -EAGAIN:
			goto out;
		case -ECONNABORTED:
			ASSERTCMP(state, ==, AFS_CALL_COMPLETE);
			call->responded = true;
			afs_log_error(call, call->abort_code);
			goto done;
		case -ENOTSUPP:
			call->responded = true;
			abort_code = RXGEN_OPCODE;
			rxrpc_kernel_abort_call(call->net->socket, call->rxcall,
						abort_code, ret,
						afs_abort_op_not_supported);
			goto local_abort;
		case -EIO:
			pr_err("kAFS: Call %u in bad state %u\n",
			       call->debug_id, state);
			fallthrough;
		case -ENODATA:
		case -EBADMSG:
		case -EMSGSIZE:
		case -ENOMEM:
		case -EFAULT:
			abort_code = RXGEN_CC_UNMARSHAL;
			if (state != AFS_CALL_CL_AWAIT_REPLY)
				abort_code = RXGEN_SS_UNMARSHAL;
			rxrpc_kernel_abort_call(call->net->socket, call->rxcall,
						abort_code, ret,
						afs_abort_unmarshal_error);
			goto local_abort;
		default:
			abort_code = RX_CALL_DEAD;
			rxrpc_kernel_abort_call(call->net->socket, call->rxcall,
						abort_code, ret,
						afs_abort_general_error);
			goto local_abort;
		}
	}

done:
	if (call->type->done)
		call->type->done(call);
out:
	_leave("");
	return;

local_abort:
	abort_code = 0;
call_complete:
	afs_set_call_complete(call, ret, remote_abort);
	state = AFS_CALL_COMPLETE;
	goto done;
}

/*
 * Wait synchronously for a call to complete.
 */
void afs_wait_for_call_to_complete(struct afs_call *call)
{
	bool rxrpc_complete = false;

	_enter("");

	if (!afs_check_call_state(call, AFS_CALL_COMPLETE)) {
		DECLARE_WAITQUEUE(myself, current);

		add_wait_queue(&call->waitq, &myself);
		for (;;) {
			set_current_state(TASK_UNINTERRUPTIBLE);

			/* deliver any messages that are in the queue */
			if (!afs_check_call_state(call, AFS_CALL_COMPLETE) &&
			    call->need_attention) {
				call->need_attention = false;
				__set_current_state(TASK_RUNNING);
				afs_deliver_to_call(call);
				continue;
			}

			if (afs_check_call_state(call, AFS_CALL_COMPLETE))
				break;

			if (!rxrpc_kernel_check_life(call->net->socket, call->rxcall)) {
				/* rxrpc terminated the call. */
				rxrpc_complete = true;
				break;
			}

			schedule();
		}

		remove_wait_queue(&call->waitq, &myself);
		__set_current_state(TASK_RUNNING);
	}

	if (!afs_check_call_state(call, AFS_CALL_COMPLETE)) {
		if (rxrpc_complete) {
			afs_set_call_complete(call, call->error, call->abort_code);
		} else {
			/* Kill off the call if it's still live. */
			_debug("call interrupted");
			if (rxrpc_kernel_abort_call(call->net->socket, call->rxcall,
						    RX_USER_ABORT, -EINTR,
						    afs_abort_interrupted))
				afs_set_call_complete(call, -EINTR, 0);
		}
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * wake up a waiting call
 */
<<<<<<< HEAD
static void afs_wake_up_call_waiter(struct afs_call *call)
{
=======
static void afs_wake_up_call_waiter(struct sock *sk, struct rxrpc_call *rxcall,
				    unsigned long call_user_ID)
{
	struct afs_call *call = (struct afs_call *)call_user_ID;

	call->need_attention = true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	wake_up(&call->waitq);
}

/*
 * wake up an asynchronous call
 */
<<<<<<< HEAD
static void afs_wake_up_async_call(struct afs_call *call)
{
	_enter("");
	queue_work(afs_async_calls, &call->async_work);
}

/*
 * put a call into asynchronous mode
 * - mustn't touch the call descriptor as the call my have completed by the
 *   time we get here
 */
static int afs_dont_wait_for_call_to_complete(struct afs_call *call)
{
	_enter("");
	return -EINPROGRESS;
}

/*
 * delete an asynchronous call
 */
static void afs_delete_async_call(struct work_struct *work)
{
	struct afs_call *call =
		container_of(work, struct afs_call, async_work);

	_enter("");

	afs_free_call(call);

	_leave("");
}

/*
 * perform processing on an asynchronous call
 * - on a multiple-thread workqueue this work item may try to run on several
 *   CPUs at the same time
 */
static void afs_process_async_call(struct work_struct *work)
{
	struct afs_call *call =
		container_of(work, struct afs_call, async_work);

	_enter("");

	if (!skb_queue_empty(&call->rx_queue))
		afs_deliver_to_call(call);

	if (call->state >= AFS_CALL_COMPLETE && call->wait_mode) {
		if (call->wait_mode->async_complete)
			call->wait_mode->async_complete(call->reply,
							call->error);
		call->reply = NULL;

		/* kill the call */
		rxrpc_kernel_end_call(call->rxcall);
		call->rxcall = NULL;
		if (call->type->destructor)
			call->type->destructor(call);

		/* we can't just delete the call because the work item may be
		 * queued */
		PREPARE_WORK(&call->async_work, afs_delete_async_call);
		queue_work(afs_async_calls, &call->async_work);
	}

	_leave("");
}

/*
 * empty a socket buffer into a flat reply buffer
 */
void afs_transfer_reply(struct afs_call *call, struct sk_buff *skb)
{
	size_t len = skb->len;

	if (skb_copy_bits(skb, 0, call->buffer + call->reply_size, len) < 0)
		BUG();
	call->reply_size += len;
}

/*
 * accept the backlog of incoming calls
 */
static void afs_collect_incoming_call(struct work_struct *work)
{
	struct rxrpc_call *rxcall;
	struct afs_call *call = NULL;
	struct sk_buff *skb;

	while ((skb = skb_dequeue(&afs_incoming_calls))) {
		_debug("new call");

		/* don't need the notification */
		afs_free_skb(skb);

		if (!call) {
			call = kzalloc(sizeof(struct afs_call), GFP_KERNEL);
			if (!call) {
				rxrpc_kernel_reject_call(afs_socket);
				return;
			}

			INIT_WORK(&call->async_work, afs_process_async_call);
			call->wait_mode = &afs_async_incoming_call;
			call->type = &afs_RXCMxxxx;
			init_waitqueue_head(&call->waitq);
			skb_queue_head_init(&call->rx_queue);
			call->state = AFS_CALL_AWAIT_OP_ID;

			_debug("CALL %p{%s} [%d]",
			       call, call->type->name,
			       atomic_read(&afs_outstanding_calls));
			atomic_inc(&afs_outstanding_calls);
		}

		rxcall = rxrpc_kernel_accept_call(afs_socket,
						  (unsigned long) call);
		if (!IS_ERR(rxcall)) {
			call->rxcall = rxcall;
			call = NULL;
		}
	}

	if (call)
		afs_free_call(call);
}

/*
 * grab the operation ID from an incoming cache manager call
 */
static int afs_deliver_cm_op_id(struct afs_call *call, struct sk_buff *skb,
				bool last)
{
	size_t len = skb->len;
	void *oibuf = (void *) &call->operation_ID;

	_enter("{%u},{%zu},%d", call->offset, len, last);

	ASSERTCMP(call->offset, <, 4);

	/* the operation ID forms the first four bytes of the request data */
	len = min_t(size_t, len, 4 - call->offset);
	if (skb_copy_bits(skb, 0, oibuf + call->offset, len) < 0)
		BUG();
	if (!pskb_pull(skb, len))
		BUG();
	call->offset += len;

	if (call->offset < 4) {
		if (last) {
			_leave(" = -EBADMSG [op ID short]");
			return -EBADMSG;
		}
		_leave(" = 0 [incomplete]");
		return 0;
	}

	call->state = AFS_CALL_AWAIT_REQUEST;
=======
static void afs_wake_up_async_call(struct sock *sk, struct rxrpc_call *rxcall,
				   unsigned long call_user_ID)
{
	struct afs_call *call = (struct afs_call *)call_user_ID;
	int r;

	trace_afs_notify_call(rxcall, call);
	call->need_attention = true;

	if (__refcount_inc_not_zero(&call->ref, &r)) {
		trace_afs_call(call->debug_id, afs_call_trace_wake, r + 1,
			       atomic_read(&call->net->nr_outstanding_calls),
			       __builtin_return_address(0));

		if (!queue_work(afs_async_calls, &call->async_work))
			afs_put_call(call);
	}
}

/*
 * Perform I/O processing on an asynchronous call.  The work item carries a ref
 * to the call struct that we either need to release or to pass on.
 */
static void afs_process_async_call(struct work_struct *work)
{
	struct afs_call *call = container_of(work, struct afs_call, async_work);

	_enter("");

	if (call->state < AFS_CALL_COMPLETE && call->need_attention) {
		call->need_attention = false;
		afs_deliver_to_call(call);
	}

	afs_put_call(call);
	_leave("");
}

static void afs_rx_attach(struct rxrpc_call *rxcall, unsigned long user_call_ID)
{
	struct afs_call *call = (struct afs_call *)user_call_ID;

	call->rxcall = rxcall;
}

/*
 * Charge the incoming call preallocation.
 */
void afs_charge_preallocation(struct work_struct *work)
{
	struct afs_net *net =
		container_of(work, struct afs_net, charge_preallocation_work);
	struct afs_call *call = net->spare_incoming_call;

	for (;;) {
		if (!call) {
			call = afs_alloc_call(net, &afs_RXCMxxxx, GFP_KERNEL);
			if (!call)
				break;

			call->drop_ref = true;
			call->async = true;
			call->state = AFS_CALL_SV_AWAIT_OP_ID;
			init_waitqueue_head(&call->waitq);
			afs_extract_to_tmp(call);
		}

		if (rxrpc_kernel_charge_accept(net->socket,
					       afs_wake_up_async_call,
					       afs_rx_attach,
					       (unsigned long)call,
					       GFP_KERNEL,
					       call->debug_id) < 0)
			break;
		call = NULL;
	}
	net->spare_incoming_call = call;
}

/*
 * Discard a preallocated call when a socket is shut down.
 */
static void afs_rx_discard_new_call(struct rxrpc_call *rxcall,
				    unsigned long user_call_ID)
{
	struct afs_call *call = (struct afs_call *)user_call_ID;

	call->rxcall = NULL;
	afs_put_call(call);
}

/*
 * Notification of an incoming call.
 */
static void afs_rx_new_call(struct sock *sk, struct rxrpc_call *rxcall,
			    unsigned long user_call_ID)
{
	struct afs_net *net = afs_sock2net(sk);

	queue_work(afs_wq, &net->charge_preallocation_work);
}

/*
 * Grab the operation ID from an incoming cache manager call.  The socket
 * buffer is discarded on error or if we don't yet have sufficient data.
 */
static int afs_deliver_cm_op_id(struct afs_call *call)
{
	int ret;

	_enter("{%zu}", iov_iter_count(call->iter));

	/* the operation ID forms the first four bytes of the request data */
	ret = afs_extract_data(call, true);
	if (ret < 0)
		return ret;

	call->operation_ID = ntohl(call->tmp);
	afs_set_call_state(call, AFS_CALL_SV_AWAIT_OP_ID, AFS_CALL_SV_AWAIT_REQUEST);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* ask the cache manager to route the call (it'll change the call type
	 * if successful) */
	if (!afs_cm_incoming_call(call))
		return -ENOTSUPP;

<<<<<<< HEAD
	/* pass responsibility for the remainer of this message off to the
	 * cache manager op */
	return call->type->deliver(call, skb, last);
=======
	trace_afs_cb_call(call);

	/* pass responsibility for the remainer of this message off to the
	 * cache manager op */
	return call->type->deliver(call);
}

/*
 * Advance the AFS call state when an RxRPC service call ends the transmit
 * phase.
 */
static void afs_notify_end_reply_tx(struct sock *sock,
				    struct rxrpc_call *rxcall,
				    unsigned long call_user_ID)
{
	struct afs_call *call = (struct afs_call *)call_user_ID;

	afs_set_call_state(call, AFS_CALL_SV_REPLYING, AFS_CALL_SV_AWAIT_ACK);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * send an empty reply
 */
void afs_send_empty_reply(struct afs_call *call)
{
<<<<<<< HEAD
	struct msghdr msg;
	struct iovec iov[1];

	_enter("");

	iov[0].iov_base		= NULL;
	iov[0].iov_len		= 0;
	msg.msg_name		= NULL;
	msg.msg_namelen		= 0;
	msg.msg_iov		= iov;
	msg.msg_iovlen		= 0;
=======
	struct afs_net *net = call->net;
	struct msghdr msg;

	_enter("");

	rxrpc_kernel_set_tx_length(net->socket, call->rxcall, 0);

	msg.msg_name		= NULL;
	msg.msg_namelen		= 0;
	iov_iter_kvec(&msg.msg_iter, ITER_SOURCE, NULL, 0, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	msg.msg_control		= NULL;
	msg.msg_controllen	= 0;
	msg.msg_flags		= 0;

<<<<<<< HEAD
	call->state = AFS_CALL_AWAIT_ACK;
	switch (rxrpc_kernel_send_data(call->rxcall, &msg, 0)) {
=======
	switch (rxrpc_kernel_send_data(net->socket, call->rxcall, &msg, 0,
				       afs_notify_end_reply_tx)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case 0:
		_leave(" [replied]");
		return;

	case -ENOMEM:
		_debug("oom");
<<<<<<< HEAD
		rxrpc_kernel_abort_call(call->rxcall, RX_USER_ABORT);
	default:
		rxrpc_kernel_end_call(call->rxcall);
		call->rxcall = NULL;
		call->type->destructor(call);
		afs_free_call(call);
=======
		rxrpc_kernel_abort_call(net->socket, call->rxcall,
					RXGEN_SS_MARSHAL, -ENOMEM,
					afs_abort_oom);
		fallthrough;
	default:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		_leave(" [error]");
		return;
	}
}

/*
 * send a simple reply
 */
void afs_send_simple_reply(struct afs_call *call, const void *buf, size_t len)
{
<<<<<<< HEAD
	struct msghdr msg;
	struct iovec iov[1];
=======
	struct afs_net *net = call->net;
	struct msghdr msg;
	struct kvec iov[1];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int n;

	_enter("");

<<<<<<< HEAD
=======
	rxrpc_kernel_set_tx_length(net->socket, call->rxcall, len);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	iov[0].iov_base		= (void *) buf;
	iov[0].iov_len		= len;
	msg.msg_name		= NULL;
	msg.msg_namelen		= 0;
<<<<<<< HEAD
	msg.msg_iov		= iov;
	msg.msg_iovlen		= 1;
=======
	iov_iter_kvec(&msg.msg_iter, ITER_SOURCE, iov, 1, len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	msg.msg_control		= NULL;
	msg.msg_controllen	= 0;
	msg.msg_flags		= 0;

<<<<<<< HEAD
	call->state = AFS_CALL_AWAIT_ACK;
	n = rxrpc_kernel_send_data(call->rxcall, &msg, len);
	if (n >= 0) {
		_leave(" [replied]");
		return;
	}
	if (n == -ENOMEM) {
		_debug("oom");
		rxrpc_kernel_abort_call(call->rxcall, RX_USER_ABORT);
	}
	rxrpc_kernel_end_call(call->rxcall);
	call->rxcall = NULL;
	call->type->destructor(call);
	afs_free_call(call);
=======
	n = rxrpc_kernel_send_data(net->socket, call->rxcall, &msg, len,
				   afs_notify_end_reply_tx);
	if (n >= 0) {
		/* Success */
		_leave(" [replied]");
		return;
	}

	if (n == -ENOMEM) {
		_debug("oom");
		rxrpc_kernel_abort_call(net->socket, call->rxcall,
					RXGEN_SS_MARSHAL, -ENOMEM,
					afs_abort_oom);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	_leave(" [error]");
}

/*
<<<<<<< HEAD
 * extract a piece of data from the received data socket buffers
 */
int afs_extract_data(struct afs_call *call, struct sk_buff *skb,
		     bool last, void *buf, size_t count)
{
	size_t len = skb->len;

	_enter("{%u},{%zu},%d,,%zu", call->offset, len, last, count);

	ASSERTCMP(call->offset, <, count);

	len = min_t(size_t, len, count - call->offset);
	if (skb_copy_bits(skb, 0, buf + call->offset, len) < 0 ||
	    !pskb_pull(skb, len))
		BUG();
	call->offset += len;

	if (call->offset < count) {
		if (last) {
			_leave(" = -EBADMSG [%d < %zu]", call->offset, count);
			return -EBADMSG;
		}
		_leave(" = -EAGAIN");
		return -EAGAIN;
	}
	return 0;
=======
 * Extract a piece of data from the received data socket buffers.
 */
int afs_extract_data(struct afs_call *call, bool want_more)
{
	struct afs_net *net = call->net;
	struct iov_iter *iter = call->iter;
	enum afs_call_state state;
	u32 remote_abort = 0;
	int ret;

	_enter("{%s,%zu,%zu},%d",
	       call->type->name, call->iov_len, iov_iter_count(iter), want_more);

	ret = rxrpc_kernel_recv_data(net->socket, call->rxcall, iter,
				     &call->iov_len, want_more, &remote_abort,
				     &call->service_id);
	trace_afs_receive_data(call, call->iter, want_more, ret);
	if (ret == 0 || ret == -EAGAIN)
		return ret;

	state = READ_ONCE(call->state);
	if (ret == 1) {
		switch (state) {
		case AFS_CALL_CL_AWAIT_REPLY:
			afs_set_call_state(call, state, AFS_CALL_CL_PROC_REPLY);
			break;
		case AFS_CALL_SV_AWAIT_REQUEST:
			afs_set_call_state(call, state, AFS_CALL_SV_REPLYING);
			break;
		case AFS_CALL_COMPLETE:
			kdebug("prem complete %d", call->error);
			return afs_io_error(call, afs_io_error_extract);
		default:
			break;
		}
		return 0;
	}

	afs_set_call_complete(call, ret, remote_abort);
	return ret;
}

/*
 * Log protocol error production.
 */
noinline int afs_protocol_error(struct afs_call *call,
				enum afs_eproto_cause cause)
{
	trace_afs_protocol_error(call, cause);
	if (call)
		call->unmarshalling_error = true;
	return -EBADMSG;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
