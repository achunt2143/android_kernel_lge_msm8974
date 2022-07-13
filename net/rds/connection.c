/*
<<<<<<< HEAD
 * Copyright (c) 2006 Oracle.  All rights reserved.
=======
 * Copyright (c) 2006, 2018 Oracle and/or its affiliates. All rights reserved.
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
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/export.h>
<<<<<<< HEAD
#include <net/inet_hashtables.h>
=======
#include <net/ipv6.h>
#include <net/inet6_hashtables.h>
#include <net/addrconf.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include "rds.h"
#include "loop.h"

#define RDS_CONNECTION_HASH_BITS 12
#define RDS_CONNECTION_HASH_ENTRIES (1 << RDS_CONNECTION_HASH_BITS)
#define RDS_CONNECTION_HASH_MASK (RDS_CONNECTION_HASH_ENTRIES - 1)

/* converting this to RCU is a chore for another day.. */
static DEFINE_SPINLOCK(rds_conn_lock);
static unsigned long rds_conn_count;
static struct hlist_head rds_conn_hash[RDS_CONNECTION_HASH_ENTRIES];
static struct kmem_cache *rds_conn_slab;

<<<<<<< HEAD
static struct hlist_head *rds_conn_bucket(__be32 laddr, __be32 faddr)
{
	/* Pass NULL, don't need struct net for hash */
	unsigned long hash = inet_ehashfn(NULL,
					  be32_to_cpu(laddr), 0,
					  be32_to_cpu(faddr), 0);
=======
static struct hlist_head *rds_conn_bucket(const struct in6_addr *laddr,
					  const struct in6_addr *faddr)
{
	static u32 rds6_hash_secret __read_mostly;
	static u32 rds_hash_secret __read_mostly;

	u32 lhash, fhash, hash;

	net_get_random_once(&rds_hash_secret, sizeof(rds_hash_secret));
	net_get_random_once(&rds6_hash_secret, sizeof(rds6_hash_secret));

	lhash = (__force u32)laddr->s6_addr32[3];
#if IS_ENABLED(CONFIG_IPV6)
	fhash = __ipv6_addr_jhash(faddr, rds6_hash_secret);
#else
	fhash = (__force u32)faddr->s6_addr32[3];
#endif
	hash = __inet_ehashfn(lhash, 0, fhash, 0, rds_hash_secret);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return &rds_conn_hash[hash & RDS_CONNECTION_HASH_MASK];
}

#define rds_conn_info_set(var, test, suffix) do {		\
	if (test)						\
		var |= RDS_INFO_CONNECTION_FLAG_##suffix;	\
} while (0)

/* rcu read lock must be held or the connection spinlock */
<<<<<<< HEAD
static struct rds_connection *rds_conn_lookup(struct hlist_head *head,
					      __be32 laddr, __be32 faddr,
					      struct rds_transport *trans)
{
	struct rds_connection *conn, *ret = NULL;
	struct hlist_node *pos;

	hlist_for_each_entry_rcu(conn, pos, head, c_hash_node) {
		if (conn->c_faddr == faddr && conn->c_laddr == laddr &&
				conn->c_trans == trans) {
=======
static struct rds_connection *rds_conn_lookup(struct net *net,
					      struct hlist_head *head,
					      const struct in6_addr *laddr,
					      const struct in6_addr *faddr,
					      struct rds_transport *trans,
					      u8 tos, int dev_if)
{
	struct rds_connection *conn, *ret = NULL;

	hlist_for_each_entry_rcu(conn, head, c_hash_node) {
		if (ipv6_addr_equal(&conn->c_faddr, faddr) &&
		    ipv6_addr_equal(&conn->c_laddr, laddr) &&
		    conn->c_trans == trans &&
		    conn->c_tos == tos &&
		    net == rds_conn_net(conn) &&
		    conn->c_dev_if == dev_if) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			ret = conn;
			break;
		}
	}
<<<<<<< HEAD
	rdsdebug("returning conn %p for %pI4 -> %pI4\n", ret,
		 &laddr, &faddr);
=======
	rdsdebug("returning conn %p for %pI6c -> %pI6c\n", ret,
		 laddr, faddr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

/*
 * This is called by transports as they're bringing down a connection.
 * It clears partial message state so that the transport can start sending
 * and receiving over this connection again in the future.  It is up to
 * the transport to have serialized this call with its send and recv.
 */
<<<<<<< HEAD
static void rds_conn_reset(struct rds_connection *conn)
{
	rdsdebug("connection %pI4 to %pI4 reset\n",
	  &conn->c_laddr, &conn->c_faddr);

	rds_stats_inc(s_conn_reset);
	rds_send_reset(conn);
	conn->c_flags = 0;
=======
static void rds_conn_path_reset(struct rds_conn_path *cp)
{
	struct rds_connection *conn = cp->cp_conn;

	rdsdebug("connection %pI6c to %pI6c reset\n",
		 &conn->c_laddr, &conn->c_faddr);

	rds_stats_inc(s_conn_reset);
	rds_send_path_reset(cp);
	cp->cp_flags = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Do not clear next_rx_seq here, else we cannot distinguish
	 * retransmitted packets from new packets, and will hand all
	 * of them to the application. That is not consistent with the
	 * reliability guarantees of RDS. */
}

<<<<<<< HEAD
=======
static void __rds_conn_path_init(struct rds_connection *conn,
				 struct rds_conn_path *cp, bool is_outgoing)
{
	spin_lock_init(&cp->cp_lock);
	cp->cp_next_tx_seq = 1;
	init_waitqueue_head(&cp->cp_waitq);
	INIT_LIST_HEAD(&cp->cp_send_queue);
	INIT_LIST_HEAD(&cp->cp_retrans);

	cp->cp_conn = conn;
	atomic_set(&cp->cp_state, RDS_CONN_DOWN);
	cp->cp_send_gen = 0;
	cp->cp_reconnect_jiffies = 0;
	cp->cp_conn->c_proposed_version = RDS_PROTOCOL_VERSION;
	INIT_DELAYED_WORK(&cp->cp_send_w, rds_send_worker);
	INIT_DELAYED_WORK(&cp->cp_recv_w, rds_recv_worker);
	INIT_DELAYED_WORK(&cp->cp_conn_w, rds_connect_worker);
	INIT_WORK(&cp->cp_down_w, rds_shutdown_worker);
	mutex_init(&cp->cp_cm_lock);
	cp->cp_flags = 0;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * There is only every one 'conn' for a given pair of addresses in the
 * system at a time.  They contain messages to be retransmitted and so
 * span the lifetime of the actual underlying transport connections.
 *
 * For now they are not garbage collected once they're created.  They
 * are torn down as the module is removed, if ever.
 */
<<<<<<< HEAD
static struct rds_connection *__rds_conn_create(__be32 laddr, __be32 faddr,
				       struct rds_transport *trans, gfp_t gfp,
				       int is_outgoing)
=======
static struct rds_connection *__rds_conn_create(struct net *net,
						const struct in6_addr *laddr,
						const struct in6_addr *faddr,
						struct rds_transport *trans,
						gfp_t gfp, u8 tos,
						int is_outgoing,
						int dev_if)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct rds_connection *conn, *parent = NULL;
	struct hlist_head *head = rds_conn_bucket(laddr, faddr);
	struct rds_transport *loop_trans;
	unsigned long flags;
<<<<<<< HEAD
	int ret;

	rcu_read_lock();
	conn = rds_conn_lookup(head, laddr, faddr, trans);
	if (conn && conn->c_loopback && conn->c_trans != &rds_loop_transport &&
=======
	int ret, i;
	int npaths = (trans->t_mp_capable ? RDS_MPATH_WORKERS : 1);

	rcu_read_lock();
	conn = rds_conn_lookup(net, head, laddr, faddr, trans, tos, dev_if);
	if (conn &&
	    conn->c_loopback &&
	    conn->c_trans != &rds_loop_transport &&
	    ipv6_addr_equal(laddr, faddr) &&
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	    !is_outgoing) {
		/* This is a looped back IB connection, and we're
		 * called by the code handling the incoming connect.
		 * We need a second connection object into which we
		 * can stick the other QP. */
		parent = conn;
		conn = parent->c_passive;
	}
	rcu_read_unlock();
	if (conn)
		goto out;

	conn = kmem_cache_zalloc(rds_conn_slab, gfp);
	if (!conn) {
		conn = ERR_PTR(-ENOMEM);
		goto out;
	}
<<<<<<< HEAD

	INIT_HLIST_NODE(&conn->c_hash_node);
	conn->c_laddr = laddr;
	conn->c_faddr = faddr;
	spin_lock_init(&conn->c_lock);
	conn->c_next_tx_seq = 1;

	init_waitqueue_head(&conn->c_waitq);
	INIT_LIST_HEAD(&conn->c_send_queue);
	INIT_LIST_HEAD(&conn->c_retrans);

	ret = rds_cong_get_maps(conn);
	if (ret) {
=======
	conn->c_path = kcalloc(npaths, sizeof(struct rds_conn_path), gfp);
	if (!conn->c_path) {
		kmem_cache_free(rds_conn_slab, conn);
		conn = ERR_PTR(-ENOMEM);
		goto out;
	}

	INIT_HLIST_NODE(&conn->c_hash_node);
	conn->c_laddr = *laddr;
	conn->c_isv6 = !ipv6_addr_v4mapped(laddr);
	conn->c_faddr = *faddr;
	conn->c_dev_if = dev_if;
	conn->c_tos = tos;

#if IS_ENABLED(CONFIG_IPV6)
	/* If the local address is link local, set c_bound_if to be the
	 * index used for this connection.  Otherwise, set it to 0 as
	 * the socket is not bound to an interface.  c_bound_if is used
	 * to look up a socket when a packet is received
	 */
	if (ipv6_addr_type(laddr) & IPV6_ADDR_LINKLOCAL)
		conn->c_bound_if = dev_if;
	else
#endif
		conn->c_bound_if = 0;

	rds_conn_net_set(conn, net);

	ret = rds_cong_get_maps(conn);
	if (ret) {
		kfree(conn->c_path);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		kmem_cache_free(rds_conn_slab, conn);
		conn = ERR_PTR(ret);
		goto out;
	}

	/*
	 * This is where a connection becomes loopback.  If *any* RDS sockets
	 * can bind to the destination address then we'd rather the messages
	 * flow through loopback rather than either transport.
	 */
<<<<<<< HEAD
	loop_trans = rds_trans_get_preferred(faddr);
	if (loop_trans) {
		rds_trans_put(loop_trans);
		conn->c_loopback = 1;
		if (is_outgoing && trans->t_prefer_loopback) {
			/* "outgoing" connection - and the transport
			 * says it wants the connection handled by the
			 * loopback transport. This is what TCP does.
			 */
			trans = &rds_loop_transport;
=======
	loop_trans = rds_trans_get_preferred(net, faddr, conn->c_dev_if);
	if (loop_trans) {
		rds_trans_put(loop_trans);
		conn->c_loopback = 1;
		if (trans->t_prefer_loopback) {
			if (likely(is_outgoing)) {
				/* "outgoing" connection to local address.
				 * Protocol says it wants the connection
				 * handled by the loopback transport.
				 * This is what TCP does.
				 */
				trans = &rds_loop_transport;
			} else {
				/* No transport currently in use
				 * should end up here, but if it
				 * does, reset/destroy the connection.
				 */
				kfree(conn->c_path);
				kmem_cache_free(rds_conn_slab, conn);
				conn = ERR_PTR(-EOPNOTSUPP);
				goto out;
			}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}

	conn->c_trans = trans;

<<<<<<< HEAD
	ret = trans->conn_alloc(conn, gfp);
	if (ret) {
=======
	init_waitqueue_head(&conn->c_hs_waitq);
	for (i = 0; i < npaths; i++) {
		__rds_conn_path_init(conn, &conn->c_path[i],
				     is_outgoing);
		conn->c_path[i].cp_index = i;
	}
	rcu_read_lock();
	if (rds_destroy_pending(conn))
		ret = -ENETDOWN;
	else
		ret = trans->conn_alloc(conn, GFP_ATOMIC);
	if (ret) {
		rcu_read_unlock();
		kfree(conn->c_path);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		kmem_cache_free(rds_conn_slab, conn);
		conn = ERR_PTR(ret);
		goto out;
	}

<<<<<<< HEAD
	atomic_set(&conn->c_state, RDS_CONN_DOWN);
	conn->c_reconnect_jiffies = 0;
	INIT_DELAYED_WORK(&conn->c_send_w, rds_send_worker);
	INIT_DELAYED_WORK(&conn->c_recv_w, rds_recv_worker);
	INIT_DELAYED_WORK(&conn->c_conn_w, rds_connect_worker);
	INIT_WORK(&conn->c_down_w, rds_shutdown_worker);
	mutex_init(&conn->c_cm_lock);
	conn->c_flags = 0;

	rdsdebug("allocated conn %p for %pI4 -> %pI4 over %s %s\n",
	  conn, &laddr, &faddr,
	  trans->t_name ? trans->t_name : "[unknown]",
	  is_outgoing ? "(outgoing)" : "");
=======
	rdsdebug("allocated conn %p for %pI6c -> %pI6c over %s %s\n",
		 conn, laddr, faddr,
		 strnlen(trans->t_name, sizeof(trans->t_name)) ?
		 trans->t_name : "[unknown]", is_outgoing ? "(outgoing)" : "");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Since we ran without holding the conn lock, someone could
	 * have created the same conn (either normal or passive) in the
	 * interim. We check while holding the lock. If we won, we complete
	 * init and return our conn. If we lost, we rollback and return the
	 * other one.
	 */
	spin_lock_irqsave(&rds_conn_lock, flags);
	if (parent) {
		/* Creating passive conn */
		if (parent->c_passive) {
<<<<<<< HEAD
			trans->conn_free(conn->c_transport_data);
=======
			trans->conn_free(conn->c_path[0].cp_transport_data);
			kfree(conn->c_path);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			kmem_cache_free(rds_conn_slab, conn);
			conn = parent->c_passive;
		} else {
			parent->c_passive = conn;
			rds_cong_add_conn(conn);
			rds_conn_count++;
		}
	} else {
		/* Creating normal conn */
		struct rds_connection *found;

<<<<<<< HEAD
		found = rds_conn_lookup(head, laddr, faddr, trans);
		if (found) {
			trans->conn_free(conn->c_transport_data);
			kmem_cache_free(rds_conn_slab, conn);
			conn = found;
		} else {
=======
		found = rds_conn_lookup(net, head, laddr, faddr, trans,
					tos, dev_if);
		if (found) {
			struct rds_conn_path *cp;
			int i;

			for (i = 0; i < npaths; i++) {
				cp = &conn->c_path[i];
				/* The ->conn_alloc invocation may have
				 * allocated resource for all paths, so all
				 * of them may have to be freed here.
				 */
				if (cp->cp_transport_data)
					trans->conn_free(cp->cp_transport_data);
			}
			kfree(conn->c_path);
			kmem_cache_free(rds_conn_slab, conn);
			conn = found;
		} else {
			conn->c_my_gen_num = rds_gen_num;
			conn->c_peer_gen_num = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			hlist_add_head_rcu(&conn->c_hash_node, head);
			rds_cong_add_conn(conn);
			rds_conn_count++;
		}
	}
	spin_unlock_irqrestore(&rds_conn_lock, flags);
<<<<<<< HEAD
=======
	rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

out:
	return conn;
}

<<<<<<< HEAD
struct rds_connection *rds_conn_create(__be32 laddr, __be32 faddr,
				       struct rds_transport *trans, gfp_t gfp)
{
	return __rds_conn_create(laddr, faddr, trans, gfp, 0);
}
EXPORT_SYMBOL_GPL(rds_conn_create);

struct rds_connection *rds_conn_create_outgoing(__be32 laddr, __be32 faddr,
				       struct rds_transport *trans, gfp_t gfp)
{
	return __rds_conn_create(laddr, faddr, trans, gfp, 1);
}
EXPORT_SYMBOL_GPL(rds_conn_create_outgoing);

void rds_conn_shutdown(struct rds_connection *conn)
{
	/* shut it down unless it's down already */
	if (!rds_conn_transition(conn, RDS_CONN_DOWN, RDS_CONN_DOWN)) {
=======
struct rds_connection *rds_conn_create(struct net *net,
				       const struct in6_addr *laddr,
				       const struct in6_addr *faddr,
				       struct rds_transport *trans, u8 tos,
				       gfp_t gfp, int dev_if)
{
	return __rds_conn_create(net, laddr, faddr, trans, gfp, tos, 0, dev_if);
}
EXPORT_SYMBOL_GPL(rds_conn_create);

struct rds_connection *rds_conn_create_outgoing(struct net *net,
						const struct in6_addr *laddr,
						const struct in6_addr *faddr,
						struct rds_transport *trans,
						u8 tos, gfp_t gfp, int dev_if)
{
	return __rds_conn_create(net, laddr, faddr, trans, gfp, tos, 1, dev_if);
}
EXPORT_SYMBOL_GPL(rds_conn_create_outgoing);

void rds_conn_shutdown(struct rds_conn_path *cp)
{
	struct rds_connection *conn = cp->cp_conn;

	/* shut it down unless it's down already */
	if (!rds_conn_path_transition(cp, RDS_CONN_DOWN, RDS_CONN_DOWN)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * Quiesce the connection mgmt handlers before we start tearing
		 * things down. We don't hold the mutex for the entire
		 * duration of the shutdown operation, else we may be
		 * deadlocking with the CM handler. Instead, the CM event
		 * handler is supposed to check for state DISCONNECTING
		 */
<<<<<<< HEAD
		mutex_lock(&conn->c_cm_lock);
		if (!rds_conn_transition(conn, RDS_CONN_UP, RDS_CONN_DISCONNECTING)
		 && !rds_conn_transition(conn, RDS_CONN_ERROR, RDS_CONN_DISCONNECTING)) {
			rds_conn_error(conn, "shutdown called in state %d\n",
					atomic_read(&conn->c_state));
			mutex_unlock(&conn->c_cm_lock);
			return;
		}
		mutex_unlock(&conn->c_cm_lock);

		wait_event(conn->c_waitq,
			   !test_bit(RDS_IN_XMIT, &conn->c_flags));

		conn->c_trans->conn_shutdown(conn);
		rds_conn_reset(conn);

		if (!rds_conn_transition(conn, RDS_CONN_DISCONNECTING, RDS_CONN_DOWN)) {
			/* This can happen - eg when we're in the middle of tearing
			 * down the connection, and someone unloads the rds module.
			 * Quite reproduceable with loopback connections.
			 * Mostly harmless.
			 */
			rds_conn_error(conn,
				"%s: failed to transition to state DOWN, "
				"current state is %d\n",
				__func__,
				atomic_read(&conn->c_state));
=======
		mutex_lock(&cp->cp_cm_lock);
		if (!rds_conn_path_transition(cp, RDS_CONN_UP,
					      RDS_CONN_DISCONNECTING) &&
		    !rds_conn_path_transition(cp, RDS_CONN_ERROR,
					      RDS_CONN_DISCONNECTING)) {
			rds_conn_path_error(cp,
					    "shutdown called in state %d\n",
					    atomic_read(&cp->cp_state));
			mutex_unlock(&cp->cp_cm_lock);
			return;
		}
		mutex_unlock(&cp->cp_cm_lock);

		wait_event(cp->cp_waitq,
			   !test_bit(RDS_IN_XMIT, &cp->cp_flags));
		wait_event(cp->cp_waitq,
			   !test_bit(RDS_RECV_REFILL, &cp->cp_flags));

		conn->c_trans->conn_path_shutdown(cp);
		rds_conn_path_reset(cp);

		if (!rds_conn_path_transition(cp, RDS_CONN_DISCONNECTING,
					      RDS_CONN_DOWN) &&
		    !rds_conn_path_transition(cp, RDS_CONN_ERROR,
					      RDS_CONN_DOWN)) {
			/* This can happen - eg when we're in the middle of tearing
			 * down the connection, and someone unloads the rds module.
			 * Quite reproducible with loopback connections.
			 * Mostly harmless.
			 *
			 * Note that this also happens with rds-tcp because
			 * we could have triggered rds_conn_path_drop in irq
			 * mode from rds_tcp_state change on the receipt of
			 * a FIN, thus we need to recheck for RDS_CONN_ERROR
			 * here.
			 */
			rds_conn_path_error(cp, "%s: failed to transition "
					    "to state DOWN, current state "
					    "is %d\n", __func__,
					    atomic_read(&cp->cp_state));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return;
		}
	}

	/* Then reconnect if it's still live.
	 * The passive side of an IB loopback connection is never added
	 * to the conn hash, so we never trigger a reconnect on this
	 * conn - the reconnect is always triggered by the active peer. */
<<<<<<< HEAD
	cancel_delayed_work_sync(&conn->c_conn_w);
	rcu_read_lock();
	if (!hlist_unhashed(&conn->c_hash_node)) {
		rcu_read_unlock();
		rds_queue_reconnect(conn);
=======
	cancel_delayed_work_sync(&cp->cp_conn_w);
	rcu_read_lock();
	if (!hlist_unhashed(&conn->c_hash_node)) {
		rcu_read_unlock();
		rds_queue_reconnect(cp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		rcu_read_unlock();
	}
}

<<<<<<< HEAD
=======
/* destroy a single rds_conn_path. rds_conn_destroy() iterates over
 * all paths using rds_conn_path_destroy()
 */
static void rds_conn_path_destroy(struct rds_conn_path *cp)
{
	struct rds_message *rm, *rtmp;

	if (!cp->cp_transport_data)
		return;

	/* make sure lingering queued work won't try to ref the conn */
	cancel_delayed_work_sync(&cp->cp_send_w);
	cancel_delayed_work_sync(&cp->cp_recv_w);

	rds_conn_path_drop(cp, true);
	flush_work(&cp->cp_down_w);

	/* tear down queued messages */
	list_for_each_entry_safe(rm, rtmp,
				 &cp->cp_send_queue,
				 m_conn_item) {
		list_del_init(&rm->m_conn_item);
		BUG_ON(!list_empty(&rm->m_sock_item));
		rds_message_put(rm);
	}
	if (cp->cp_xmit_rm)
		rds_message_put(cp->cp_xmit_rm);

	WARN_ON(delayed_work_pending(&cp->cp_send_w));
	WARN_ON(delayed_work_pending(&cp->cp_recv_w));
	WARN_ON(delayed_work_pending(&cp->cp_conn_w));
	WARN_ON(work_pending(&cp->cp_down_w));

	cp->cp_conn->c_trans->conn_free(cp->cp_transport_data);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Stop and free a connection.
 *
 * This can only be used in very limited circumstances.  It assumes that once
 * the conn has been shutdown that no one else is referencing the connection.
 * We can only ensure this in the rmmod path in the current code.
 */
void rds_conn_destroy(struct rds_connection *conn)
{
<<<<<<< HEAD
	struct rds_message *rm, *rtmp;
	unsigned long flags;
=======
	unsigned long flags;
	int i;
	struct rds_conn_path *cp;
	int npaths = (conn->c_trans->t_mp_capable ? RDS_MPATH_WORKERS : 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	rdsdebug("freeing conn %p for %pI4 -> "
		 "%pI4\n", conn, &conn->c_laddr,
		 &conn->c_faddr);

	/* Ensure conn will not be scheduled for reconnect */
	spin_lock_irq(&rds_conn_lock);
	hlist_del_init_rcu(&conn->c_hash_node);
	spin_unlock_irq(&rds_conn_lock);
	synchronize_rcu();

	/* shut the connection down */
<<<<<<< HEAD
	rds_conn_drop(conn);
	flush_work(&conn->c_down_w);

	/* make sure lingering queued work won't try to ref the conn */
	cancel_delayed_work_sync(&conn->c_send_w);
	cancel_delayed_work_sync(&conn->c_recv_w);

	/* tear down queued messages */
	list_for_each_entry_safe(rm, rtmp,
				 &conn->c_send_queue,
				 m_conn_item) {
		list_del_init(&rm->m_conn_item);
		BUG_ON(!list_empty(&rm->m_sock_item));
		rds_message_put(rm);
	}
	if (conn->c_xmit_rm)
		rds_message_put(conn->c_xmit_rm);

	conn->c_trans->conn_free(conn->c_transport_data);
=======
	for (i = 0; i < npaths; i++) {
		cp = &conn->c_path[i];
		rds_conn_path_destroy(cp);
		BUG_ON(!list_empty(&cp->cp_retrans));
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * The congestion maps aren't freed up here.  They're
	 * freed by rds_cong_exit() after all the connections
	 * have been freed.
	 */
	rds_cong_remove_conn(conn);

<<<<<<< HEAD
	BUG_ON(!list_empty(&conn->c_retrans));
=======
	kfree(conn->c_path);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kmem_cache_free(rds_conn_slab, conn);

	spin_lock_irqsave(&rds_conn_lock, flags);
	rds_conn_count--;
	spin_unlock_irqrestore(&rds_conn_lock, flags);
}
EXPORT_SYMBOL_GPL(rds_conn_destroy);

<<<<<<< HEAD
static void rds_conn_message_info(struct socket *sock, unsigned int len,
				  struct rds_info_iterator *iter,
				  struct rds_info_lengths *lens,
				  int want_send)
{
	struct hlist_head *head;
	struct hlist_node *pos;
=======
static void __rds_inc_msg_cp(struct rds_incoming *inc,
			     struct rds_info_iterator *iter,
			     void *saddr, void *daddr, int flip, bool isv6)
{
#if IS_ENABLED(CONFIG_IPV6)
	if (isv6)
		rds6_inc_info_copy(inc, iter, saddr, daddr, flip);
	else
#endif
		rds_inc_info_copy(inc, iter, *(__be32 *)saddr,
				  *(__be32 *)daddr, flip);
}

static void rds_conn_message_info_cmn(struct socket *sock, unsigned int len,
				      struct rds_info_iterator *iter,
				      struct rds_info_lengths *lens,
				      int want_send, bool isv6)
{
	struct hlist_head *head;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct list_head *list;
	struct rds_connection *conn;
	struct rds_message *rm;
	unsigned int total = 0;
	unsigned long flags;
	size_t i;
<<<<<<< HEAD

	len /= sizeof(struct rds_info_message);
=======
	int j;

	if (isv6)
		len /= sizeof(struct rds6_info_message);
	else
		len /= sizeof(struct rds_info_message);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	rcu_read_lock();

	for (i = 0, head = rds_conn_hash; i < ARRAY_SIZE(rds_conn_hash);
	     i++, head++) {
<<<<<<< HEAD
		hlist_for_each_entry_rcu(conn, pos, head, c_hash_node) {
			if (want_send)
				list = &conn->c_send_queue;
			else
				list = &conn->c_retrans;

			spin_lock_irqsave(&conn->c_lock, flags);

			/* XXX too lazy to maintain counts.. */
			list_for_each_entry(rm, list, m_conn_item) {
				total++;
				if (total <= len)
					rds_inc_info_copy(&rm->m_inc, iter,
							  conn->c_laddr,
							  conn->c_faddr, 0);
			}

			spin_unlock_irqrestore(&conn->c_lock, flags);
=======
		hlist_for_each_entry_rcu(conn, head, c_hash_node) {
			struct rds_conn_path *cp;
			int npaths;

			if (!isv6 && conn->c_isv6)
				continue;

			npaths = (conn->c_trans->t_mp_capable ?
				 RDS_MPATH_WORKERS : 1);

			for (j = 0; j < npaths; j++) {
				cp = &conn->c_path[j];
				if (want_send)
					list = &cp->cp_send_queue;
				else
					list = &cp->cp_retrans;

				spin_lock_irqsave(&cp->cp_lock, flags);

				/* XXX too lazy to maintain counts.. */
				list_for_each_entry(rm, list, m_conn_item) {
					total++;
					if (total <= len)
						__rds_inc_msg_cp(&rm->m_inc,
								 iter,
								 &conn->c_laddr,
								 &conn->c_faddr,
								 0, isv6);
				}

				spin_unlock_irqrestore(&cp->cp_lock, flags);
			}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}
	rcu_read_unlock();

	lens->nr = total;
<<<<<<< HEAD
	lens->each = sizeof(struct rds_info_message);
}

=======
	if (isv6)
		lens->each = sizeof(struct rds6_info_message);
	else
		lens->each = sizeof(struct rds_info_message);
}

static void rds_conn_message_info(struct socket *sock, unsigned int len,
				  struct rds_info_iterator *iter,
				  struct rds_info_lengths *lens,
				  int want_send)
{
	rds_conn_message_info_cmn(sock, len, iter, lens, want_send, false);
}

#if IS_ENABLED(CONFIG_IPV6)
static void rds6_conn_message_info(struct socket *sock, unsigned int len,
				   struct rds_info_iterator *iter,
				   struct rds_info_lengths *lens,
				   int want_send)
{
	rds_conn_message_info_cmn(sock, len, iter, lens, want_send, true);
}
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void rds_conn_message_info_send(struct socket *sock, unsigned int len,
				       struct rds_info_iterator *iter,
				       struct rds_info_lengths *lens)
{
	rds_conn_message_info(sock, len, iter, lens, 1);
}

<<<<<<< HEAD
=======
#if IS_ENABLED(CONFIG_IPV6)
static void rds6_conn_message_info_send(struct socket *sock, unsigned int len,
					struct rds_info_iterator *iter,
					struct rds_info_lengths *lens)
{
	rds6_conn_message_info(sock, len, iter, lens, 1);
}
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void rds_conn_message_info_retrans(struct socket *sock,
					  unsigned int len,
					  struct rds_info_iterator *iter,
					  struct rds_info_lengths *lens)
{
	rds_conn_message_info(sock, len, iter, lens, 0);
}

<<<<<<< HEAD
=======
#if IS_ENABLED(CONFIG_IPV6)
static void rds6_conn_message_info_retrans(struct socket *sock,
					   unsigned int len,
					   struct rds_info_iterator *iter,
					   struct rds_info_lengths *lens)
{
	rds6_conn_message_info(sock, len, iter, lens, 0);
}
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void rds_for_each_conn_info(struct socket *sock, unsigned int len,
			  struct rds_info_iterator *iter,
			  struct rds_info_lengths *lens,
			  int (*visitor)(struct rds_connection *, void *),
<<<<<<< HEAD
			  size_t item_len)
{
	uint64_t buffer[(item_len + 7) / 8];
	struct hlist_head *head;
	struct hlist_node *pos;
=======
			  u64 *buffer,
			  size_t item_len)
{
	struct hlist_head *head;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct rds_connection *conn;
	size_t i;

	rcu_read_lock();

	lens->nr = 0;
	lens->each = item_len;

	for (i = 0, head = rds_conn_hash; i < ARRAY_SIZE(rds_conn_hash);
	     i++, head++) {
<<<<<<< HEAD
		hlist_for_each_entry_rcu(conn, pos, head, c_hash_node) {
=======
		hlist_for_each_entry_rcu(conn, head, c_hash_node) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

			/* XXX no c_lock usage.. */
			if (!visitor(conn, buffer))
				continue;

			/* We copy as much as we can fit in the buffer,
			 * but we count all items so that the caller
			 * can resize the buffer. */
			if (len >= item_len) {
				rds_info_copy(iter, buffer, item_len);
				len -= item_len;
			}
			lens->nr++;
		}
	}
	rcu_read_unlock();
}
EXPORT_SYMBOL_GPL(rds_for_each_conn_info);

<<<<<<< HEAD
static int rds_conn_info_visitor(struct rds_connection *conn,
				  void *buffer)
{
	struct rds_info_connection *cinfo = buffer;

	cinfo->next_tx_seq = conn->c_next_tx_seq;
	cinfo->next_rx_seq = conn->c_next_rx_seq;
	cinfo->laddr = conn->c_laddr;
	cinfo->faddr = conn->c_faddr;
=======
static void rds_walk_conn_path_info(struct socket *sock, unsigned int len,
				    struct rds_info_iterator *iter,
				    struct rds_info_lengths *lens,
				    int (*visitor)(struct rds_conn_path *, void *),
				    u64 *buffer,
				    size_t item_len)
{
	struct hlist_head *head;
	struct rds_connection *conn;
	size_t i;

	rcu_read_lock();

	lens->nr = 0;
	lens->each = item_len;

	for (i = 0, head = rds_conn_hash; i < ARRAY_SIZE(rds_conn_hash);
	     i++, head++) {
		hlist_for_each_entry_rcu(conn, head, c_hash_node) {
			struct rds_conn_path *cp;

			/* XXX We only copy the information from the first
			 * path for now.  The problem is that if there are
			 * more than one underlying paths, we cannot report
			 * information of all of them using the existing
			 * API.  For example, there is only one next_tx_seq,
			 * which path's next_tx_seq should we report?  It is
			 * a bug in the design of MPRDS.
			 */
			cp = conn->c_path;

			/* XXX no cp_lock usage.. */
			if (!visitor(cp, buffer))
				continue;

			/* We copy as much as we can fit in the buffer,
			 * but we count all items so that the caller
			 * can resize the buffer.
			 */
			if (len >= item_len) {
				rds_info_copy(iter, buffer, item_len);
				len -= item_len;
			}
			lens->nr++;
		}
	}
	rcu_read_unlock();
}

static int rds_conn_info_visitor(struct rds_conn_path *cp, void *buffer)
{
	struct rds_info_connection *cinfo = buffer;
	struct rds_connection *conn = cp->cp_conn;

	if (conn->c_isv6)
		return 0;

	cinfo->next_tx_seq = cp->cp_next_tx_seq;
	cinfo->next_rx_seq = cp->cp_next_rx_seq;
	cinfo->laddr = conn->c_laddr.s6_addr32[3];
	cinfo->faddr = conn->c_faddr.s6_addr32[3];
	cinfo->tos = conn->c_tos;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	strncpy(cinfo->transport, conn->c_trans->t_name,
		sizeof(cinfo->transport));
	cinfo->flags = 0;

<<<<<<< HEAD
	rds_conn_info_set(cinfo->flags, test_bit(RDS_IN_XMIT, &conn->c_flags),
			  SENDING);
	/* XXX Future: return the state rather than these funky bits */
	rds_conn_info_set(cinfo->flags,
			  atomic_read(&conn->c_state) == RDS_CONN_CONNECTING,
			  CONNECTING);
	rds_conn_info_set(cinfo->flags,
			  atomic_read(&conn->c_state) == RDS_CONN_UP,
=======
	rds_conn_info_set(cinfo->flags, test_bit(RDS_IN_XMIT, &cp->cp_flags),
			  SENDING);
	/* XXX Future: return the state rather than these funky bits */
	rds_conn_info_set(cinfo->flags,
			  atomic_read(&cp->cp_state) == RDS_CONN_CONNECTING,
			  CONNECTING);
	rds_conn_info_set(cinfo->flags,
			  atomic_read(&cp->cp_state) == RDS_CONN_UP,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			  CONNECTED);
	return 1;
}

<<<<<<< HEAD
=======
#if IS_ENABLED(CONFIG_IPV6)
static int rds6_conn_info_visitor(struct rds_conn_path *cp, void *buffer)
{
	struct rds6_info_connection *cinfo6 = buffer;
	struct rds_connection *conn = cp->cp_conn;

	cinfo6->next_tx_seq = cp->cp_next_tx_seq;
	cinfo6->next_rx_seq = cp->cp_next_rx_seq;
	cinfo6->laddr = conn->c_laddr;
	cinfo6->faddr = conn->c_faddr;
	strncpy(cinfo6->transport, conn->c_trans->t_name,
		sizeof(cinfo6->transport));
	cinfo6->flags = 0;

	rds_conn_info_set(cinfo6->flags, test_bit(RDS_IN_XMIT, &cp->cp_flags),
			  SENDING);
	/* XXX Future: return the state rather than these funky bits */
	rds_conn_info_set(cinfo6->flags,
			  atomic_read(&cp->cp_state) == RDS_CONN_CONNECTING,
			  CONNECTING);
	rds_conn_info_set(cinfo6->flags,
			  atomic_read(&cp->cp_state) == RDS_CONN_UP,
			  CONNECTED);
	/* Just return 1 as there is no error case. This is a helper function
	 * for rds_walk_conn_path_info() and it wants a return value.
	 */
	return 1;
}
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void rds_conn_info(struct socket *sock, unsigned int len,
			  struct rds_info_iterator *iter,
			  struct rds_info_lengths *lens)
{
<<<<<<< HEAD
	rds_for_each_conn_info(sock, len, iter, lens,
				rds_conn_info_visitor,
				sizeof(struct rds_info_connection));
}

int rds_conn_init(void)
{
	rds_conn_slab = kmem_cache_create("rds_connection",
					  sizeof(struct rds_connection),
					  0, 0, NULL);
	if (!rds_conn_slab)
		return -ENOMEM;
=======
	u64 buffer[(sizeof(struct rds_info_connection) + 7) / 8];

	rds_walk_conn_path_info(sock, len, iter, lens,
				rds_conn_info_visitor,
				buffer,
				sizeof(struct rds_info_connection));
}

#if IS_ENABLED(CONFIG_IPV6)
static void rds6_conn_info(struct socket *sock, unsigned int len,
			   struct rds_info_iterator *iter,
			   struct rds_info_lengths *lens)
{
	u64 buffer[(sizeof(struct rds6_info_connection) + 7) / 8];

	rds_walk_conn_path_info(sock, len, iter, lens,
				rds6_conn_info_visitor,
				buffer,
				sizeof(struct rds6_info_connection));
}
#endif

int rds_conn_init(void)
{
	int ret;

	ret = rds_loop_net_init(); /* register pernet callback */
	if (ret)
		return ret;

	rds_conn_slab = KMEM_CACHE(rds_connection, 0);
	if (!rds_conn_slab) {
		rds_loop_net_exit();
		return -ENOMEM;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	rds_info_register_func(RDS_INFO_CONNECTIONS, rds_conn_info);
	rds_info_register_func(RDS_INFO_SEND_MESSAGES,
			       rds_conn_message_info_send);
	rds_info_register_func(RDS_INFO_RETRANS_MESSAGES,
			       rds_conn_message_info_retrans);
<<<<<<< HEAD

=======
#if IS_ENABLED(CONFIG_IPV6)
	rds_info_register_func(RDS6_INFO_CONNECTIONS, rds6_conn_info);
	rds_info_register_func(RDS6_INFO_SEND_MESSAGES,
			       rds6_conn_message_info_send);
	rds_info_register_func(RDS6_INFO_RETRANS_MESSAGES,
			       rds6_conn_message_info_retrans);
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

void rds_conn_exit(void)
{
<<<<<<< HEAD
=======
	rds_loop_net_exit(); /* unregister pernet callback */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rds_loop_exit();

	WARN_ON(!hlist_empty(rds_conn_hash));

	kmem_cache_destroy(rds_conn_slab);

	rds_info_deregister_func(RDS_INFO_CONNECTIONS, rds_conn_info);
	rds_info_deregister_func(RDS_INFO_SEND_MESSAGES,
				 rds_conn_message_info_send);
	rds_info_deregister_func(RDS_INFO_RETRANS_MESSAGES,
				 rds_conn_message_info_retrans);
<<<<<<< HEAD
=======
#if IS_ENABLED(CONFIG_IPV6)
	rds_info_deregister_func(RDS6_INFO_CONNECTIONS, rds6_conn_info);
	rds_info_deregister_func(RDS6_INFO_SEND_MESSAGES,
				 rds6_conn_message_info_send);
	rds_info_deregister_func(RDS6_INFO_RETRANS_MESSAGES,
				 rds6_conn_message_info_retrans);
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Force a disconnect
 */
<<<<<<< HEAD
void rds_conn_drop(struct rds_connection *conn)
{
	atomic_set(&conn->c_state, RDS_CONN_ERROR);
	queue_work(rds_wq, &conn->c_down_w);
=======
void rds_conn_path_drop(struct rds_conn_path *cp, bool destroy)
{
	atomic_set(&cp->cp_state, RDS_CONN_ERROR);

	rcu_read_lock();
	if (!destroy && rds_destroy_pending(cp->cp_conn)) {
		rcu_read_unlock();
		return;
	}
	queue_work(rds_wq, &cp->cp_down_w);
	rcu_read_unlock();
}
EXPORT_SYMBOL_GPL(rds_conn_path_drop);

void rds_conn_drop(struct rds_connection *conn)
{
	WARN_ON(conn->c_trans->t_mp_capable);
	rds_conn_path_drop(&conn->c_path[0], false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(rds_conn_drop);

/*
 * If the connection is down, trigger a connect. We may have scheduled a
 * delayed reconnect however - in this case we should not interfere.
 */
<<<<<<< HEAD
void rds_conn_connect_if_down(struct rds_connection *conn)
{
	if (rds_conn_state(conn) == RDS_CONN_DOWN &&
	    !test_and_set_bit(RDS_RECONNECT_PENDING, &conn->c_flags))
		queue_delayed_work(rds_wq, &conn->c_conn_w, 0);
}
EXPORT_SYMBOL_GPL(rds_conn_connect_if_down);

/*
 * An error occurred on the connection
 */
void
__rds_conn_error(struct rds_connection *conn, const char *fmt, ...)
=======
void rds_conn_path_connect_if_down(struct rds_conn_path *cp)
{
	rcu_read_lock();
	if (rds_destroy_pending(cp->cp_conn)) {
		rcu_read_unlock();
		return;
	}
	if (rds_conn_path_state(cp) == RDS_CONN_DOWN &&
	    !test_and_set_bit(RDS_RECONNECT_PENDING, &cp->cp_flags))
		queue_delayed_work(rds_wq, &cp->cp_conn_w, 0);
	rcu_read_unlock();
}
EXPORT_SYMBOL_GPL(rds_conn_path_connect_if_down);

/* Check connectivity of all paths
 */
void rds_check_all_paths(struct rds_connection *conn)
{
	int i = 0;

	do {
		rds_conn_path_connect_if_down(&conn->c_path[i]);
	} while (++i < conn->c_npaths);
}

void rds_conn_connect_if_down(struct rds_connection *conn)
{
	WARN_ON(conn->c_trans->t_mp_capable);
	rds_conn_path_connect_if_down(&conn->c_path[0]);
}
EXPORT_SYMBOL_GPL(rds_conn_connect_if_down);

void
__rds_conn_path_error(struct rds_conn_path *cp, const char *fmt, ...)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	va_list ap;

	va_start(ap, fmt);
	vprintk(fmt, ap);
	va_end(ap);

<<<<<<< HEAD
	rds_conn_drop(conn);
=======
	rds_conn_path_drop(cp, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
