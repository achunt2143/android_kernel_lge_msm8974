/*
<<<<<<< HEAD
 * Copyright (c) 2009 Oracle.  All rights reserved.
=======
 * Copyright (c) 2009, 2018 Oracle and/or its affiliates. All rights reserved.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */
#include <linux/module.h>
#include <rdma/rdma_cm.h>

<<<<<<< HEAD
#include "rdma_transport.h"

static struct rdma_cm_id *rds_rdma_listen_id;

static char *rds_cm_event_strings[] = {
#define RDS_CM_EVENT_STRING(foo) \
		[RDMA_CM_EVENT_##foo] = __stringify(RDMA_CM_EVENT_##foo)
	RDS_CM_EVENT_STRING(ADDR_RESOLVED),
	RDS_CM_EVENT_STRING(ADDR_ERROR),
	RDS_CM_EVENT_STRING(ROUTE_RESOLVED),
	RDS_CM_EVENT_STRING(ROUTE_ERROR),
	RDS_CM_EVENT_STRING(CONNECT_REQUEST),
	RDS_CM_EVENT_STRING(CONNECT_RESPONSE),
	RDS_CM_EVENT_STRING(CONNECT_ERROR),
	RDS_CM_EVENT_STRING(UNREACHABLE),
	RDS_CM_EVENT_STRING(REJECTED),
	RDS_CM_EVENT_STRING(ESTABLISHED),
	RDS_CM_EVENT_STRING(DISCONNECTED),
	RDS_CM_EVENT_STRING(DEVICE_REMOVAL),
	RDS_CM_EVENT_STRING(MULTICAST_JOIN),
	RDS_CM_EVENT_STRING(MULTICAST_ERROR),
	RDS_CM_EVENT_STRING(ADDR_CHANGE),
	RDS_CM_EVENT_STRING(TIMEWAIT_EXIT),
#undef RDS_CM_EVENT_STRING
};

static char *rds_cm_event_str(enum rdma_cm_event_type type)
{
	return rds_str_array(rds_cm_event_strings,
			     ARRAY_SIZE(rds_cm_event_strings), type);
};

int rds_rdma_cm_event_handler(struct rdma_cm_id *cm_id,
			      struct rdma_cm_event *event)
=======
#include "rds_single_path.h"
#include "rdma_transport.h"
#include "ib.h"

/* Global IPv4 and IPv6 RDS RDMA listener cm_id */
static struct rdma_cm_id *rds_rdma_listen_id;
#if IS_ENABLED(CONFIG_IPV6)
static struct rdma_cm_id *rds6_rdma_listen_id;
#endif

/* Per IB specification 7.7.3, service level is a 4-bit field. */
#define TOS_TO_SL(tos)		((tos) & 0xF)

static int rds_rdma_cm_event_handler_cmn(struct rdma_cm_id *cm_id,
					 struct rdma_cm_event *event,
					 bool isv6)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	/* this can be null in the listening path */
	struct rds_connection *conn = cm_id->context;
	struct rds_transport *trans;
	int ret = 0;
<<<<<<< HEAD

	rdsdebug("conn %p id %p handling event %u (%s)\n", conn, cm_id,
		 event->event, rds_cm_event_str(event->event));

	if (cm_id->device->node_type == RDMA_NODE_RNIC)
		trans = &rds_iw_transport;
	else
=======
	int *err;
	u8 len;

	rdsdebug("conn %p id %p handling event %u (%s)\n", conn, cm_id,
		 event->event, rdma_event_msg(event->event));

	if (cm_id->device->node_type == RDMA_NODE_IB_CA)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		trans = &rds_ib_transport;

	/* Prevent shutdown from tearing down the connection
	 * while we're executing. */
	if (conn) {
		mutex_lock(&conn->c_cm_lock);

		/* If the connection is being shut down, bail out
		 * right away. We return 0 so cm_id doesn't get
		 * destroyed prematurely */
		if (rds_conn_state(conn) == RDS_CONN_DISCONNECTING) {
			/* Reject incoming connections while we're tearing
			 * down an existing one. */
			if (event->event == RDMA_CM_EVENT_CONNECT_REQUEST)
				ret = 1;
			goto out;
		}
	}

	switch (event->event) {
	case RDMA_CM_EVENT_CONNECT_REQUEST:
<<<<<<< HEAD
		ret = trans->cm_handle_connect(cm_id, event);
		break;

	case RDMA_CM_EVENT_ADDR_RESOLVED:
		/* XXX do we need to clean up if this fails? */
		ret = rdma_resolve_route(cm_id,
					 RDS_RDMA_RESOLVE_TIMEOUT_MS);
		break;

	case RDMA_CM_EVENT_ROUTE_RESOLVED:
		/* XXX worry about racing with listen acceptance */
		ret = trans->cm_initiate_connect(cm_id);
		break;

	case RDMA_CM_EVENT_ESTABLISHED:
		trans->cm_connect_complete(conn, event);
		break;

=======
		ret = trans->cm_handle_connect(cm_id, event, isv6);
		break;

	case RDMA_CM_EVENT_ADDR_RESOLVED:
		if (conn) {
			rdma_set_service_type(cm_id, conn->c_tos);
			rdma_set_min_rnr_timer(cm_id, IB_RNR_TIMER_000_32);
			/* XXX do we need to clean up if this fails? */
			ret = rdma_resolve_route(cm_id,
						 RDS_RDMA_RESOLVE_TIMEOUT_MS);
		}
		break;

	case RDMA_CM_EVENT_ROUTE_RESOLVED:
		/* Connection could have been dropped so make sure the
		 * cm_id is valid before proceeding
		 */
		if (conn) {
			struct rds_ib_connection *ibic;

			ibic = conn->c_transport_data;
			if (ibic && ibic->i_cm_id == cm_id) {
				cm_id->route.path_rec[0].sl =
					TOS_TO_SL(conn->c_tos);
				ret = trans->cm_initiate_connect(cm_id, isv6);
			} else {
				rds_conn_drop(conn);
			}
		}
		break;

	case RDMA_CM_EVENT_ESTABLISHED:
		if (conn)
			trans->cm_connect_complete(conn, event);
		break;

	case RDMA_CM_EVENT_REJECTED:
		if (!conn)
			break;
		err = (int *)rdma_consumer_reject_data(cm_id, event, &len);
		if (!err ||
		    (err && len >= sizeof(*err) &&
		     ((*err) <= RDS_RDMA_REJ_INCOMPAT))) {
			pr_warn("RDS/RDMA: conn <%pI6c, %pI6c> rejected, dropping connection\n",
				&conn->c_laddr, &conn->c_faddr);

			if (!conn->c_tos)
				conn->c_proposed_version = RDS_PROTOCOL_COMPAT_VERSION;

			rds_conn_drop(conn);
		}
		rdsdebug("Connection rejected: %s\n",
			 rdma_reject_msg(cm_id, event->status));
		break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case RDMA_CM_EVENT_ADDR_ERROR:
	case RDMA_CM_EVENT_ROUTE_ERROR:
	case RDMA_CM_EVENT_CONNECT_ERROR:
	case RDMA_CM_EVENT_UNREACHABLE:
<<<<<<< HEAD
	case RDMA_CM_EVENT_REJECTED:
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case RDMA_CM_EVENT_DEVICE_REMOVAL:
	case RDMA_CM_EVENT_ADDR_CHANGE:
		if (conn)
			rds_conn_drop(conn);
		break;

	case RDMA_CM_EVENT_DISCONNECTED:
<<<<<<< HEAD
		rdsdebug("DISCONNECT event - dropping connection "
			"%pI4->%pI4\n", &conn->c_laddr,
=======
		if (!conn)
			break;
		rdsdebug("DISCONNECT event - dropping connection "
			 "%pI6c->%pI6c\n", &conn->c_laddr,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			 &conn->c_faddr);
		rds_conn_drop(conn);
		break;

<<<<<<< HEAD
	default:
		/* things like device disconnect? */
		printk(KERN_ERR "RDS: unknown event %u (%s)!\n",
		       event->event, rds_cm_event_str(event->event));
=======
	case RDMA_CM_EVENT_TIMEWAIT_EXIT:
		if (conn) {
			pr_info("RDS: RDMA_CM_EVENT_TIMEWAIT_EXIT event: dropping connection %pI6c->%pI6c\n",
				&conn->c_laddr, &conn->c_faddr);
			rds_conn_drop(conn);
		}
		break;

	default:
		/* things like device disconnect? */
		printk(KERN_ERR "RDS: unknown event %u (%s)!\n",
		       event->event, rdma_event_msg(event->event));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	}

out:
	if (conn)
		mutex_unlock(&conn->c_cm_lock);

	rdsdebug("id %p event %u (%s) handling ret %d\n", cm_id, event->event,
<<<<<<< HEAD
		 rds_cm_event_str(event->event), ret);
=======
		 rdma_event_msg(event->event), ret);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ret;
}

<<<<<<< HEAD
static int rds_rdma_listen_init(void)
{
	struct sockaddr_in sin;
	struct rdma_cm_id *cm_id;
	int ret;

	cm_id = rdma_create_id(rds_rdma_cm_event_handler, NULL, RDMA_PS_TCP,
			       IB_QPT_RC);
=======
int rds_rdma_cm_event_handler(struct rdma_cm_id *cm_id,
			      struct rdma_cm_event *event)
{
	return rds_rdma_cm_event_handler_cmn(cm_id, event, false);
}

#if IS_ENABLED(CONFIG_IPV6)
int rds6_rdma_cm_event_handler(struct rdma_cm_id *cm_id,
			       struct rdma_cm_event *event)
{
	return rds_rdma_cm_event_handler_cmn(cm_id, event, true);
}
#endif

static int rds_rdma_listen_init_common(rdma_cm_event_handler handler,
				       struct sockaddr *sa,
				       struct rdma_cm_id **ret_cm_id)
{
	struct rdma_cm_id *cm_id;
	int ret;

	cm_id = rdma_create_id(&init_net, handler, NULL,
			       RDMA_PS_TCP, IB_QPT_RC);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (IS_ERR(cm_id)) {
		ret = PTR_ERR(cm_id);
		printk(KERN_ERR "RDS/RDMA: failed to setup listener, "
		       "rdma_create_id() returned %d\n", ret);
		return ret;
	}

<<<<<<< HEAD
	sin.sin_family = AF_INET,
	sin.sin_addr.s_addr = (__force u32)htonl(INADDR_ANY);
	sin.sin_port = (__force u16)htons(RDS_PORT);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * XXX I bet this binds the cm_id to a device.  If we want to support
	 * fail-over we'll have to take this into consideration.
	 */
<<<<<<< HEAD
	ret = rdma_bind_addr(cm_id, (struct sockaddr *)&sin);
=======
	ret = rdma_bind_addr(cm_id, sa);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ret) {
		printk(KERN_ERR "RDS/RDMA: failed to setup listener, "
		       "rdma_bind_addr() returned %d\n", ret);
		goto out;
	}

	ret = rdma_listen(cm_id, 128);
	if (ret) {
		printk(KERN_ERR "RDS/RDMA: failed to setup listener, "
		       "rdma_listen() returned %d\n", ret);
		goto out;
	}

	rdsdebug("cm %p listening on port %u\n", cm_id, RDS_PORT);

<<<<<<< HEAD
	rds_rdma_listen_id = cm_id;
=======
	*ret_cm_id = cm_id;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	cm_id = NULL;
out:
	if (cm_id)
		rdma_destroy_id(cm_id);
	return ret;
}

<<<<<<< HEAD
=======
/* Initialize the RDS RDMA listeners.  We create two listeners for
 * compatibility reason.  The one on RDS_PORT is used for IPv4
 * requests only.  The one on RDS_CM_PORT is used for IPv6 requests
 * only.  So only IPv6 enabled RDS module will communicate using this
 * port.
 */
static int rds_rdma_listen_init(void)
{
	int ret;
#if IS_ENABLED(CONFIG_IPV6)
	struct sockaddr_in6 sin6;
#endif
	struct sockaddr_in sin;

	sin.sin_family = PF_INET;
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(RDS_PORT);
	ret = rds_rdma_listen_init_common(rds_rdma_cm_event_handler,
					  (struct sockaddr *)&sin,
					  &rds_rdma_listen_id);
	if (ret != 0)
		return ret;

#if IS_ENABLED(CONFIG_IPV6)
	sin6.sin6_family = PF_INET6;
	sin6.sin6_addr = in6addr_any;
	sin6.sin6_port = htons(RDS_CM_PORT);
	sin6.sin6_scope_id = 0;
	sin6.sin6_flowinfo = 0;
	ret = rds_rdma_listen_init_common(rds6_rdma_cm_event_handler,
					  (struct sockaddr *)&sin6,
					  &rds6_rdma_listen_id);
	/* Keep going even when IPv6 is not enabled in the system. */
	if (ret != 0)
		rdsdebug("Cannot set up IPv6 RDMA listener\n");
#endif
	return 0;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void rds_rdma_listen_stop(void)
{
	if (rds_rdma_listen_id) {
		rdsdebug("cm %p\n", rds_rdma_listen_id);
		rdma_destroy_id(rds_rdma_listen_id);
		rds_rdma_listen_id = NULL;
	}
<<<<<<< HEAD
}

static int rds_rdma_init(void)
{
	int ret;

	ret = rds_rdma_listen_init();
	if (ret)
		goto out;

	ret = rds_iw_init();
	if (ret)
		goto err_iw_init;

	ret = rds_ib_init();
	if (ret)
		goto err_ib_init;

	goto out;

err_ib_init:
	rds_iw_exit();
err_iw_init:
	rds_rdma_listen_stop();
=======
#if IS_ENABLED(CONFIG_IPV6)
	if (rds6_rdma_listen_id) {
		rdsdebug("cm %p\n", rds6_rdma_listen_id);
		rdma_destroy_id(rds6_rdma_listen_id);
		rds6_rdma_listen_id = NULL;
	}
#endif
}

static int __init rds_rdma_init(void)
{
	int ret;

	ret = rds_ib_init();
	if (ret)
		goto out;

	ret = rds_rdma_listen_init();
	if (ret)
		rds_ib_exit();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	return ret;
}
module_init(rds_rdma_init);

<<<<<<< HEAD
static void rds_rdma_exit(void)
=======
static void __exit rds_rdma_exit(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	/* stop listening first to ensure no new connections are attempted */
	rds_rdma_listen_stop();
	rds_ib_exit();
<<<<<<< HEAD
	rds_iw_exit();
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
module_exit(rds_rdma_exit);

MODULE_AUTHOR("Oracle Corporation <rds-devel@oss.oracle.com>");
<<<<<<< HEAD
MODULE_DESCRIPTION("RDS: IB/iWARP transport");
MODULE_LICENSE("Dual BSD/GPL");

=======
MODULE_DESCRIPTION("RDS: IB transport");
MODULE_LICENSE("Dual BSD/GPL");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
