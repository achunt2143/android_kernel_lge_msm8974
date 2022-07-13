<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/******************************************************************************
*******************************************************************************
**
**  Copyright (C) Sistina Software, Inc.  1997-2003  All rights reserved.
**  Copyright (C) 2004-2009 Red Hat, Inc.  All rights reserved.
**
<<<<<<< HEAD
**  This copyrighted material is made available to anyone wishing to use,
**  modify, copy, or redistribute it subject to the terms and conditions
**  of the GNU General Public License v.2.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
**
*******************************************************************************
******************************************************************************/

/*
 * lowcomms.c
 *
 * This is the "low-level" comms layer.
 *
 * It is responsible for sending/receiving messages
 * from other nodes in the cluster.
 *
 * Cluster nodes are referred to by their nodeids. nodeids are
 * simply 32 bit numbers to the locking module - if they need to
 * be expanded for the cluster infrastructure then that is its
 * responsibility. It is this layer's
 * responsibility to resolve these into IP address or
 * whatever it needs for inter-node communication.
 *
 * The comms level is two kernel threads that deal mainly with
 * the receiving of messages from other nodes and passing them
 * up to the mid-level comms layer (which understands the
 * message format) for execution by the locking core, and
 * a send thread which does all the setting up of connections
 * to remote nodes and the sending of data. Threads are not allowed
 * to send their own data because it may cause them to wait in times
 * of high load. Also, this way, the sending thread can collect together
 * messages bound for one node and send them in one block.
 *
 * lowcomms will choose to use either TCP or SCTP as its transport layer
 * depending on the configuration variable 'protocol'. This should be set
 * to 0 (default) for TCP or 1 for SCTP. It should be configured using a
 * cluster-wide mechanism as it must be the same on all nodes of the cluster
 * for the DLM to function.
 *
 */

#include <asm/ioctls.h>
#include <net/sock.h>
#include <net/tcp.h>
#include <linux/pagemap.h>
#include <linux/file.h>
#include <linux/mutex.h>
#include <linux/sctp.h>
#include <linux/slab.h>
#include <net/sctp/sctp.h>
<<<<<<< HEAD
#include <net/sctp/user.h>
#include <net/ipv6.h>

#include "dlm_internal.h"
#include "lowcomms.h"
#include "midcomms.h"
#include "config.h"

#define NEEDED_RMEM (4*1024*1024)
#define CONN_HASH_SIZE 32

/* Number of messages to send before rescheduling */
#define MAX_SEND_MSG_COUNT 25

struct cbuf {
	unsigned int base;
	unsigned int len;
	unsigned int mask;
};

static void cbuf_add(struct cbuf *cb, int n)
{
	cb->len += n;
}

static int cbuf_data(struct cbuf *cb)
{
	return ((cb->base + cb->len) & cb->mask);
}

static void cbuf_init(struct cbuf *cb, int size)
{
	cb->base = cb->len = 0;
	cb->mask = size-1;
}

static void cbuf_eat(struct cbuf *cb, int n)
{
	cb->len  -= n;
	cb->base += n;
	cb->base &= cb->mask;
}

static bool cbuf_empty(struct cbuf *cb)
{
	return cb->len == 0;
}
=======
#include <net/ipv6.h>

#include <trace/events/dlm.h>
#include <trace/events/sock.h>

#include "dlm_internal.h"
#include "lowcomms.h"
#include "midcomms.h"
#include "memory.h"
#include "config.h"

#define DLM_SHUTDOWN_WAIT_TIMEOUT msecs_to_jiffies(5000)
#define DLM_MAX_PROCESS_BUFFERS 24
#define NEEDED_RMEM (4*1024*1024)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct connection {
	struct socket *sock;	/* NULL if not connected */
	uint32_t nodeid;	/* So we know who we are in the list */
<<<<<<< HEAD
	struct mutex sock_mutex;
	unsigned long flags;
#define CF_READ_PENDING 1
#define CF_WRITE_PENDING 2
#define CF_CONNECT_PENDING 3
#define CF_INIT_PENDING 4
#define CF_IS_OTHERCON 5
#define CF_CLOSE 6
#define CF_APP_LIMITED 7
	struct list_head writequeue;  /* List of outgoing writequeue_entries */
	spinlock_t writequeue_lock;
	int (*rx_action) (struct connection *);	/* What to do when active */
	void (*connect_action) (struct connection *);	/* What to do to connect */
	struct page *rx_page;
	struct cbuf cb;
	int retries;
#define MAX_CONNECT_RETRIES 3
	int sctp_assoc;
	struct hlist_node list;
	struct connection *othercon;
	struct work_struct rwork; /* Receive workqueue */
	struct work_struct swork; /* Send workqueue */
};
#define sock2con(x) ((struct connection *)(x)->sk_user_data)

=======
	/* this semaphore is used to allow parallel recv/send in read
	 * lock mode. When we release a sock we need to held the write lock.
	 *
	 * However this is locking code and not nice. When we remove the
	 * othercon handling we can look into other mechanism to synchronize
	 * io handling to call sock_release() at the right time.
	 */
	struct rw_semaphore sock_lock;
	unsigned long flags;
#define CF_APP_LIMITED 0
#define CF_RECV_PENDING 1
#define CF_SEND_PENDING 2
#define CF_RECV_INTR 3
#define CF_IO_STOP 4
#define CF_IS_OTHERCON 5
	struct list_head writequeue;  /* List of outgoing writequeue_entries */
	spinlock_t writequeue_lock;
	int retries;
	struct hlist_node list;
	/* due some connect()/accept() races we currently have this cross over
	 * connection attempt second connection for one node.
	 *
	 * There is a solution to avoid the race by introducing a connect
	 * rule as e.g. our_nodeid > nodeid_to_connect who is allowed to
	 * connect. Otherside can connect but will only be considered that
	 * the other side wants to have a reconnect.
	 *
	 * However changing to this behaviour will break backwards compatible.
	 * In a DLM protocol major version upgrade we should remove this!
	 */
	struct connection *othercon;
	struct work_struct rwork; /* receive worker */
	struct work_struct swork; /* send worker */
	wait_queue_head_t shutdown_wait;
	unsigned char rx_leftover_buf[DLM_MAX_SOCKET_BUFSIZE];
	int rx_leftover;
	int mark;
	int addr_count;
	int curr_addr_index;
	struct sockaddr_storage addr[DLM_MAX_ADDR_COUNT];
	spinlock_t addrs_lock;
	struct rcu_head rcu;
};
#define sock2con(x) ((struct connection *)(x)->sk_user_data)

struct listen_connection {
	struct socket *sock;
	struct work_struct rwork;
};

#define DLM_WQ_REMAIN_BYTES(e) (PAGE_SIZE - e->end)
#define DLM_WQ_LENGTH_BYTES(e) (e->end - e->offset)

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* An entry waiting to be sent */
struct writequeue_entry {
	struct list_head list;
	struct page *page;
	int offset;
	int len;
	int end;
	int users;
<<<<<<< HEAD
	struct connection *con;
};

static struct sockaddr_storage *dlm_local_addr[DLM_MAX_ADDR_COUNT];
static int dlm_local_count;

/* Work queues */
static struct workqueue_struct *recv_workqueue;
static struct workqueue_struct *send_workqueue;

static struct hlist_head connection_hash[CONN_HASH_SIZE];
static DEFINE_MUTEX(connections_lock);
static struct kmem_cache *con_cache;

static void process_recv_sockets(struct work_struct *work);
static void process_send_sockets(struct work_struct *work);


/* This is deliberately very simple because most clusters have simple
   sequential nodeids, so we should be able to go straight to a connection
   struct in the array */
static inline int nodeid_hash(int nodeid)
{
	return nodeid & (CONN_HASH_SIZE-1);
}

static struct connection *__find_con(int nodeid)
{
	int r;
	struct hlist_node *h;
	struct connection *con;

	r = nodeid_hash(nodeid);

	hlist_for_each_entry(con, h, &connection_hash[r], list) {
		if (con->nodeid == nodeid)
			return con;
	}
	return NULL;
}

=======
	bool dirty;
	struct connection *con;
	struct list_head msgs;
	struct kref ref;
};

struct dlm_msg {
	struct writequeue_entry *entry;
	struct dlm_msg *orig_msg;
	bool retransmit;
	void *ppc;
	int len;
	int idx; /* new()/commit() idx exchange */

	struct list_head list;
	struct kref ref;
};

struct processqueue_entry {
	unsigned char *buf;
	int nodeid;
	int buflen;

	struct list_head list;
};

struct dlm_proto_ops {
	bool try_new_addr;
	const char *name;
	int proto;

	int (*connect)(struct connection *con, struct socket *sock,
		       struct sockaddr *addr, int addr_len);
	void (*sockopts)(struct socket *sock);
	int (*bind)(struct socket *sock);
	int (*listen_validate)(void);
	void (*listen_sockopts)(struct socket *sock);
	int (*listen_bind)(struct socket *sock);
};

static struct listen_sock_callbacks {
	void (*sk_error_report)(struct sock *);
	void (*sk_data_ready)(struct sock *);
	void (*sk_state_change)(struct sock *);
	void (*sk_write_space)(struct sock *);
} listen_sock;

static struct listen_connection listen_con;
static struct sockaddr_storage dlm_local_addr[DLM_MAX_ADDR_COUNT];
static int dlm_local_count;

/* Work queues */
static struct workqueue_struct *io_workqueue;
static struct workqueue_struct *process_workqueue;

static struct hlist_head connection_hash[CONN_HASH_SIZE];
static DEFINE_SPINLOCK(connections_lock);
DEFINE_STATIC_SRCU(connections_srcu);

static const struct dlm_proto_ops *dlm_proto_ops;

#define DLM_IO_SUCCESS 0
#define DLM_IO_END 1
#define DLM_IO_EOF 2
#define DLM_IO_RESCHED 3
#define DLM_IO_FLUSH 4

static void process_recv_sockets(struct work_struct *work);
static void process_send_sockets(struct work_struct *work);
static void process_dlm_messages(struct work_struct *work);

static DECLARE_WORK(process_work, process_dlm_messages);
static DEFINE_SPINLOCK(processqueue_lock);
static bool process_dlm_messages_pending;
static atomic_t processqueue_count;
static LIST_HEAD(processqueue);

bool dlm_lowcomms_is_running(void)
{
	return !!listen_con.sock;
}

static void lowcomms_queue_swork(struct connection *con)
{
	assert_spin_locked(&con->writequeue_lock);

	if (!test_bit(CF_IO_STOP, &con->flags) &&
	    !test_bit(CF_APP_LIMITED, &con->flags) &&
	    !test_and_set_bit(CF_SEND_PENDING, &con->flags))
		queue_work(io_workqueue, &con->swork);
}

static void lowcomms_queue_rwork(struct connection *con)
{
#ifdef CONFIG_LOCKDEP
	WARN_ON_ONCE(!lockdep_sock_is_held(con->sock->sk));
#endif

	if (!test_bit(CF_IO_STOP, &con->flags) &&
	    !test_and_set_bit(CF_RECV_PENDING, &con->flags))
		queue_work(io_workqueue, &con->rwork);
}

static void writequeue_entry_ctor(void *data)
{
	struct writequeue_entry *entry = data;

	INIT_LIST_HEAD(&entry->msgs);
}

struct kmem_cache *dlm_lowcomms_writequeue_cache_create(void)
{
	return kmem_cache_create("dlm_writequeue", sizeof(struct writequeue_entry),
				 0, 0, writequeue_entry_ctor);
}

struct kmem_cache *dlm_lowcomms_msg_cache_create(void)
{
	return kmem_cache_create("dlm_msg", sizeof(struct dlm_msg), 0, 0, NULL);
}

/* need to held writequeue_lock */
static struct writequeue_entry *con_next_wq(struct connection *con)
{
	struct writequeue_entry *e;

	e = list_first_entry_or_null(&con->writequeue, struct writequeue_entry,
				     list);
	/* if len is zero nothing is to send, if there are users filling
	 * buffers we wait until the users are done so we can send more.
	 */
	if (!e || e->users || e->len == 0)
		return NULL;

	return e;
}

static struct connection *__find_con(int nodeid, int r)
{
	struct connection *con;

	hlist_for_each_entry_rcu(con, &connection_hash[r], list) {
		if (con->nodeid == nodeid)
			return con;
	}

	return NULL;
}

static void dlm_con_init(struct connection *con, int nodeid)
{
	con->nodeid = nodeid;
	init_rwsem(&con->sock_lock);
	INIT_LIST_HEAD(&con->writequeue);
	spin_lock_init(&con->writequeue_lock);
	INIT_WORK(&con->swork, process_send_sockets);
	INIT_WORK(&con->rwork, process_recv_sockets);
	spin_lock_init(&con->addrs_lock);
	init_waitqueue_head(&con->shutdown_wait);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * If 'allocation' is zero then we don't attempt to create a new
 * connection structure for this node.
 */
<<<<<<< HEAD
static struct connection *__nodeid2con(int nodeid, gfp_t alloc)
{
	struct connection *con = NULL;
	int r;

	con = __find_con(nodeid);
	if (con || !alloc)
		return con;

	con = kmem_cache_zalloc(con_cache, alloc);
	if (!con)
		return NULL;

	r = nodeid_hash(nodeid);
	hlist_add_head(&con->list, &connection_hash[r]);

	con->nodeid = nodeid;
	mutex_init(&con->sock_mutex);
	INIT_LIST_HEAD(&con->writequeue);
	spin_lock_init(&con->writequeue_lock);
	INIT_WORK(&con->swork, process_send_sockets);
	INIT_WORK(&con->rwork, process_recv_sockets);

	/* Setup action pointers for child sockets */
	if (con->nodeid) {
		struct connection *zerocon = __find_con(0);

		con->connect_action = zerocon->connect_action;
		if (!con->rx_action)
			con->rx_action = zerocon->rx_action;
	}

	return con;
}

/* Loop round all connections */
static void foreach_conn(void (*conn_func)(struct connection *c))
{
	int i;
	struct hlist_node *h, *n;
	struct connection *con;

	for (i = 0; i < CONN_HASH_SIZE; i++) {
		hlist_for_each_entry_safe(con, h, n, &connection_hash[i], list){
			conn_func(con);
		}
	}
}

static struct connection *nodeid2con(int nodeid, gfp_t allocation)
{
	struct connection *con;

	mutex_lock(&connections_lock);
	con = __nodeid2con(nodeid, allocation);
	mutex_unlock(&connections_lock);

	return con;
}

/* This is a bit drastic, but only called when things go wrong */
static struct connection *assoc2con(int assoc_id)
{
	int i;
	struct hlist_node *h;
	struct connection *con;

	mutex_lock(&connections_lock);

	for (i = 0 ; i < CONN_HASH_SIZE; i++) {
		hlist_for_each_entry(con, h, &connection_hash[i], list) {
			if (con->sctp_assoc == assoc_id) {
				mutex_unlock(&connections_lock);
				return con;
			}
		}
	}
	mutex_unlock(&connections_lock);
	return NULL;
}

static int nodeid_to_addr(int nodeid, struct sockaddr *retaddr)
{
	struct sockaddr_storage addr;
	int error;
=======
static struct connection *nodeid2con(int nodeid, gfp_t alloc)
{
	struct connection *con, *tmp;
	int r;

	r = nodeid_hash(nodeid);
	con = __find_con(nodeid, r);
	if (con || !alloc)
		return con;

	con = kzalloc(sizeof(*con), alloc);
	if (!con)
		return NULL;

	dlm_con_init(con, nodeid);

	spin_lock(&connections_lock);
	/* Because multiple workqueues/threads calls this function it can
	 * race on multiple cpu's. Instead of locking hot path __find_con()
	 * we just check in rare cases of recently added nodes again
	 * under protection of connections_lock. If this is the case we
	 * abort our connection creation and return the existing connection.
	 */
	tmp = __find_con(nodeid, r);
	if (tmp) {
		spin_unlock(&connections_lock);
		kfree(con);
		return tmp;
	}

	hlist_add_head_rcu(&con->list, &connection_hash[r]);
	spin_unlock(&connections_lock);

	return con;
}

static int addr_compare(const struct sockaddr_storage *x,
			const struct sockaddr_storage *y)
{
	switch (x->ss_family) {
	case AF_INET: {
		struct sockaddr_in *sinx = (struct sockaddr_in *)x;
		struct sockaddr_in *siny = (struct sockaddr_in *)y;
		if (sinx->sin_addr.s_addr != siny->sin_addr.s_addr)
			return 0;
		if (sinx->sin_port != siny->sin_port)
			return 0;
		break;
	}
	case AF_INET6: {
		struct sockaddr_in6 *sinx = (struct sockaddr_in6 *)x;
		struct sockaddr_in6 *siny = (struct sockaddr_in6 *)y;
		if (!ipv6_addr_equal(&sinx->sin6_addr, &siny->sin6_addr))
			return 0;
		if (sinx->sin6_port != siny->sin6_port)
			return 0;
		break;
	}
	default:
		return 0;
	}
	return 1;
}

static int nodeid_to_addr(int nodeid, struct sockaddr_storage *sas_out,
			  struct sockaddr *sa_out, bool try_new_addr,
			  unsigned int *mark)
{
	struct sockaddr_storage sas;
	struct connection *con;
	int idx;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!dlm_local_count)
		return -1;

<<<<<<< HEAD
	error = dlm_nodeid_to_addr(nodeid, &addr);
	if (error)
		return error;

	if (dlm_local_addr[0]->ss_family == AF_INET) {
		struct sockaddr_in *in4  = (struct sockaddr_in *) &addr;
		struct sockaddr_in *ret4 = (struct sockaddr_in *) retaddr;
		ret4->sin_addr.s_addr = in4->sin_addr.s_addr;
	} else {
		struct sockaddr_in6 *in6  = (struct sockaddr_in6 *) &addr;
		struct sockaddr_in6 *ret6 = (struct sockaddr_in6 *) retaddr;
		ret6->sin6_addr = in6->sin6_addr;
	}

=======
	idx = srcu_read_lock(&connections_srcu);
	con = nodeid2con(nodeid, 0);
	if (!con) {
		srcu_read_unlock(&connections_srcu, idx);
		return -ENOENT;
	}

	spin_lock(&con->addrs_lock);
	if (!con->addr_count) {
		spin_unlock(&con->addrs_lock);
		srcu_read_unlock(&connections_srcu, idx);
		return -ENOENT;
	}

	memcpy(&sas, &con->addr[con->curr_addr_index],
	       sizeof(struct sockaddr_storage));

	if (try_new_addr) {
		con->curr_addr_index++;
		if (con->curr_addr_index == con->addr_count)
			con->curr_addr_index = 0;
	}

	*mark = con->mark;
	spin_unlock(&con->addrs_lock);

	if (sas_out)
		memcpy(sas_out, &sas, sizeof(struct sockaddr_storage));

	if (!sa_out) {
		srcu_read_unlock(&connections_srcu, idx);
		return 0;
	}

	if (dlm_local_addr[0].ss_family == AF_INET) {
		struct sockaddr_in *in4  = (struct sockaddr_in *) &sas;
		struct sockaddr_in *ret4 = (struct sockaddr_in *) sa_out;
		ret4->sin_addr.s_addr = in4->sin_addr.s_addr;
	} else {
		struct sockaddr_in6 *in6  = (struct sockaddr_in6 *) &sas;
		struct sockaddr_in6 *ret6 = (struct sockaddr_in6 *) sa_out;
		ret6->sin6_addr = in6->sin6_addr;
	}

	srcu_read_unlock(&connections_srcu, idx);
	return 0;
}

static int addr_to_nodeid(struct sockaddr_storage *addr, int *nodeid,
			  unsigned int *mark)
{
	struct connection *con;
	int i, idx, addr_i;

	idx = srcu_read_lock(&connections_srcu);
	for (i = 0; i < CONN_HASH_SIZE; i++) {
		hlist_for_each_entry_rcu(con, &connection_hash[i], list) {
			WARN_ON_ONCE(!con->addr_count);

			spin_lock(&con->addrs_lock);
			for (addr_i = 0; addr_i < con->addr_count; addr_i++) {
				if (addr_compare(&con->addr[addr_i], addr)) {
					*nodeid = con->nodeid;
					*mark = con->mark;
					spin_unlock(&con->addrs_lock);
					srcu_read_unlock(&connections_srcu, idx);
					return 0;
				}
			}
			spin_unlock(&con->addrs_lock);
		}
	}
	srcu_read_unlock(&connections_srcu, idx);

	return -ENOENT;
}

static bool dlm_lowcomms_con_has_addr(const struct connection *con,
				      const struct sockaddr_storage *addr)
{
	int i;

	for (i = 0; i < con->addr_count; i++) {
		if (addr_compare(&con->addr[i], addr))
			return true;
	}

	return false;
}

int dlm_lowcomms_addr(int nodeid, struct sockaddr_storage *addr, int len)
{
	struct connection *con;
	bool ret, idx;

	idx = srcu_read_lock(&connections_srcu);
	con = nodeid2con(nodeid, GFP_NOFS);
	if (!con) {
		srcu_read_unlock(&connections_srcu, idx);
		return -ENOMEM;
	}

	spin_lock(&con->addrs_lock);
	if (!con->addr_count) {
		memcpy(&con->addr[0], addr, sizeof(*addr));
		con->addr_count = 1;
		con->mark = dlm_config.ci_mark;
		spin_unlock(&con->addrs_lock);
		srcu_read_unlock(&connections_srcu, idx);
		return 0;
	}

	ret = dlm_lowcomms_con_has_addr(con, addr);
	if (ret) {
		spin_unlock(&con->addrs_lock);
		srcu_read_unlock(&connections_srcu, idx);
		return -EEXIST;
	}

	if (con->addr_count >= DLM_MAX_ADDR_COUNT) {
		spin_unlock(&con->addrs_lock);
		srcu_read_unlock(&connections_srcu, idx);
		return -ENOSPC;
	}

	memcpy(&con->addr[con->addr_count++], addr, sizeof(*addr));
	srcu_read_unlock(&connections_srcu, idx);
	spin_unlock(&con->addrs_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

/* Data available on socket or listen socket received a connect */
<<<<<<< HEAD
static void lowcomms_data_ready(struct sock *sk, int count_unused)
{
	struct connection *con = sock2con(sk);
	if (con && !test_and_set_bit(CF_READ_PENDING, &con->flags))
		queue_work(recv_workqueue, &con->rwork);
=======
static void lowcomms_data_ready(struct sock *sk)
{
	struct connection *con = sock2con(sk);

	trace_sk_data_ready(sk);

	set_bit(CF_RECV_INTR, &con->flags);
	lowcomms_queue_rwork(con);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void lowcomms_write_space(struct sock *sk)
{
	struct connection *con = sock2con(sk);

<<<<<<< HEAD
	if (!con)
		return;

	clear_bit(SOCK_NOSPACE, &con->sock->flags);

	if (test_and_clear_bit(CF_APP_LIMITED, &con->flags)) {
		con->sock->sk->sk_write_pending--;
		clear_bit(SOCK_ASYNC_NOSPACE, &con->sock->flags);
	}

	if (!test_and_set_bit(CF_WRITE_PENDING, &con->flags))
		queue_work(send_workqueue, &con->swork);
}

static inline void lowcomms_connect_sock(struct connection *con)
{
	if (test_bit(CF_CLOSE, &con->flags))
		return;
	if (!test_and_set_bit(CF_CONNECT_PENDING, &con->flags))
		queue_work(send_workqueue, &con->swork);
=======
	clear_bit(SOCK_NOSPACE, &con->sock->flags);

	spin_lock_bh(&con->writequeue_lock);
	if (test_and_clear_bit(CF_APP_LIMITED, &con->flags)) {
		con->sock->sk->sk_write_pending--;
		clear_bit(SOCKWQ_ASYNC_NOSPACE, &con->sock->flags);
	}

	lowcomms_queue_swork(con);
	spin_unlock_bh(&con->writequeue_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void lowcomms_state_change(struct sock *sk)
{
<<<<<<< HEAD
	if (sk->sk_state == TCP_ESTABLISHED)
		lowcomms_write_space(sk);
=======
	/* SCTP layer is not calling sk_data_ready when the connection
	 * is done, so we catch the signal through here.
	 */
	if (sk->sk_shutdown == RCV_SHUTDOWN)
		lowcomms_data_ready(sk);
}

static void lowcomms_listen_data_ready(struct sock *sk)
{
	trace_sk_data_ready(sk);

	queue_work(io_workqueue, &listen_con.rwork);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int dlm_lowcomms_connect_node(int nodeid)
{
	struct connection *con;
<<<<<<< HEAD

	/* with sctp there's no connecting without sending */
	if (dlm_config.ci_protocol != 0)
		return 0;

	if (nodeid == dlm_our_nodeid())
		return 0;

	con = nodeid2con(nodeid, GFP_NOFS);
	if (!con)
		return -ENOMEM;
	lowcomms_connect_sock(con);
	return 0;
}

/* Make a socket active */
static int add_sock(struct socket *sock, struct connection *con)
{
	con->sock = sock;

	/* Install a data_ready callback */
	con->sock->sk->sk_data_ready = lowcomms_data_ready;
	con->sock->sk->sk_write_space = lowcomms_write_space;
	con->sock->sk->sk_state_change = lowcomms_state_change;
	con->sock->sk->sk_user_data = con;
	con->sock->sk->sk_allocation = GFP_NOFS;
	return 0;
=======
	int idx;

	idx = srcu_read_lock(&connections_srcu);
	con = nodeid2con(nodeid, 0);
	if (WARN_ON_ONCE(!con)) {
		srcu_read_unlock(&connections_srcu, idx);
		return -ENOENT;
	}

	down_read(&con->sock_lock);
	if (!con->sock) {
		spin_lock_bh(&con->writequeue_lock);
		lowcomms_queue_swork(con);
		spin_unlock_bh(&con->writequeue_lock);
	}
	up_read(&con->sock_lock);
	srcu_read_unlock(&connections_srcu, idx);

	cond_resched();
	return 0;
}

int dlm_lowcomms_nodes_set_mark(int nodeid, unsigned int mark)
{
	struct connection *con;
	int idx;

	idx = srcu_read_lock(&connections_srcu);
	con = nodeid2con(nodeid, 0);
	if (!con) {
		srcu_read_unlock(&connections_srcu, idx);
		return -ENOENT;
	}

	spin_lock(&con->addrs_lock);
	con->mark = mark;
	spin_unlock(&con->addrs_lock);
	srcu_read_unlock(&connections_srcu, idx);
	return 0;
}

static void lowcomms_error_report(struct sock *sk)
{
	struct connection *con = sock2con(sk);
	struct inet_sock *inet;

	inet = inet_sk(sk);
	switch (sk->sk_family) {
	case AF_INET:
		printk_ratelimited(KERN_ERR "dlm: node %d: socket error "
				   "sending to node %d at %pI4, dport %d, "
				   "sk_err=%d/%d\n", dlm_our_nodeid(),
				   con->nodeid, &inet->inet_daddr,
				   ntohs(inet->inet_dport), sk->sk_err,
				   READ_ONCE(sk->sk_err_soft));
		break;
#if IS_ENABLED(CONFIG_IPV6)
	case AF_INET6:
		printk_ratelimited(KERN_ERR "dlm: node %d: socket error "
				   "sending to node %d at %pI6c, "
				   "dport %d, sk_err=%d/%d\n", dlm_our_nodeid(),
				   con->nodeid, &sk->sk_v6_daddr,
				   ntohs(inet->inet_dport), sk->sk_err,
				   READ_ONCE(sk->sk_err_soft));
		break;
#endif
	default:
		printk_ratelimited(KERN_ERR "dlm: node %d: socket error "
				   "invalid socket family %d set, "
				   "sk_err=%d/%d\n", dlm_our_nodeid(),
				   sk->sk_family, sk->sk_err,
				   READ_ONCE(sk->sk_err_soft));
		break;
	}

	dlm_midcomms_unack_msg_resend(con->nodeid);

	listen_sock.sk_error_report(sk);
}

static void restore_callbacks(struct sock *sk)
{
#ifdef CONFIG_LOCKDEP
	WARN_ON_ONCE(!lockdep_sock_is_held(sk));
#endif

	sk->sk_user_data = NULL;
	sk->sk_data_ready = listen_sock.sk_data_ready;
	sk->sk_state_change = listen_sock.sk_state_change;
	sk->sk_write_space = listen_sock.sk_write_space;
	sk->sk_error_report = listen_sock.sk_error_report;
}

/* Make a socket active */
static void add_sock(struct socket *sock, struct connection *con)
{
	struct sock *sk = sock->sk;

	lock_sock(sk);
	con->sock = sock;

	sk->sk_user_data = con;
	sk->sk_data_ready = lowcomms_data_ready;
	sk->sk_write_space = lowcomms_write_space;
	if (dlm_config.ci_protocol == DLM_PROTO_SCTP)
		sk->sk_state_change = lowcomms_state_change;
	sk->sk_allocation = GFP_NOFS;
	sk->sk_use_task_frag = false;
	sk->sk_error_report = lowcomms_error_report;
	release_sock(sk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Add the port number to an IPv6 or 4 sockaddr and return the address
   length */
static void make_sockaddr(struct sockaddr_storage *saddr, uint16_t port,
			  int *addr_len)
{
<<<<<<< HEAD
	saddr->ss_family =  dlm_local_addr[0]->ss_family;
=======
	saddr->ss_family =  dlm_local_addr[0].ss_family;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (saddr->ss_family == AF_INET) {
		struct sockaddr_in *in4_addr = (struct sockaddr_in *)saddr;
		in4_addr->sin_port = cpu_to_be16(port);
		*addr_len = sizeof(struct sockaddr_in);
		memset(&in4_addr->sin_zero, 0, sizeof(in4_addr->sin_zero));
	} else {
		struct sockaddr_in6 *in6_addr = (struct sockaddr_in6 *)saddr;
		in6_addr->sin6_port = cpu_to_be16(port);
		*addr_len = sizeof(struct sockaddr_in6);
	}
	memset((char *)saddr + *addr_len, 0, sizeof(struct sockaddr_storage) - *addr_len);
}

<<<<<<< HEAD
/* Close a remote connection and tidy up */
static void close_connection(struct connection *con, bool and_other)
{
	mutex_lock(&con->sock_mutex);

	if (con->sock) {
		sock_release(con->sock);
		con->sock = NULL;
	}
	if (con->othercon && and_other) {
		/* Will only re-enter once. */
		close_connection(con->othercon, false);
	}
	if (con->rx_page) {
		__free_page(con->rx_page);
		con->rx_page = NULL;
	}

	con->retries = 0;
	mutex_unlock(&con->sock_mutex);
}

/* We only send shutdown messages to nodes that are not part of the cluster */
static void sctp_send_shutdown(sctp_assoc_t associd)
{
	static char outcmsg[CMSG_SPACE(sizeof(struct sctp_sndrcvinfo))];
	struct msghdr outmessage;
	struct cmsghdr *cmsg;
	struct sctp_sndrcvinfo *sinfo;
	int ret;
	struct connection *con;

	con = nodeid2con(0,0);
	BUG_ON(con == NULL);

	outmessage.msg_name = NULL;
	outmessage.msg_namelen = 0;
	outmessage.msg_control = outcmsg;
	outmessage.msg_controllen = sizeof(outcmsg);
	outmessage.msg_flags = MSG_EOR;

	cmsg = CMSG_FIRSTHDR(&outmessage);
	cmsg->cmsg_level = IPPROTO_SCTP;
	cmsg->cmsg_type = SCTP_SNDRCV;
	cmsg->cmsg_len = CMSG_LEN(sizeof(struct sctp_sndrcvinfo));
	outmessage.msg_controllen = cmsg->cmsg_len;
	sinfo = CMSG_DATA(cmsg);
	memset(sinfo, 0x00, sizeof(struct sctp_sndrcvinfo));

	sinfo->sinfo_flags |= MSG_EOF;
	sinfo->sinfo_assoc_id = associd;

	ret = kernel_sendmsg(con->sock, &outmessage, NULL, 0, 0);

	if (ret != 0)
		log_print("send EOF to node failed: %d", ret);
}

static void sctp_init_failed_foreach(struct connection *con)
{
	con->sctp_assoc = 0;
	if (test_and_clear_bit(CF_CONNECT_PENDING, &con->flags)) {
		if (!test_and_set_bit(CF_WRITE_PENDING, &con->flags))
			queue_work(send_workqueue, &con->swork);
	}
}

/* INIT failed but we don't know which node...
   restart INIT on all pending nodes */
static void sctp_init_failed(void)
{
	mutex_lock(&connections_lock);

	foreach_conn(sctp_init_failed_foreach);

	mutex_unlock(&connections_lock);
}

/* Something happened to an association */
static void process_sctp_notification(struct connection *con,
				      struct msghdr *msg, char *buf)
{
	union sctp_notification *sn = (union sctp_notification *)buf;

	if (sn->sn_header.sn_type == SCTP_ASSOC_CHANGE) {
		switch (sn->sn_assoc_change.sac_state) {

		case SCTP_COMM_UP:
		case SCTP_RESTART:
		{
			/* Check that the new node is in the lockspace */
			struct sctp_prim prim;
			int nodeid;
			int prim_len, ret;
			int addr_len;
			struct connection *new_con;

			/*
			 * We get this before any data for an association.
			 * We verify that the node is in the cluster and
			 * then peel off a socket for it.
			 */
			if ((int)sn->sn_assoc_change.sac_assoc_id <= 0) {
				log_print("COMM_UP for invalid assoc ID %d",
					 (int)sn->sn_assoc_change.sac_assoc_id);
				sctp_init_failed();
				return;
			}
			memset(&prim, 0, sizeof(struct sctp_prim));
			prim_len = sizeof(struct sctp_prim);
			prim.ssp_assoc_id = sn->sn_assoc_change.sac_assoc_id;

			ret = kernel_getsockopt(con->sock,
						IPPROTO_SCTP,
						SCTP_PRIMARY_ADDR,
						(char*)&prim,
						&prim_len);
			if (ret < 0) {
				log_print("getsockopt/sctp_primary_addr on "
					  "new assoc %d failed : %d",
					  (int)sn->sn_assoc_change.sac_assoc_id,
					  ret);

				/* Retry INIT later */
				new_con = assoc2con(sn->sn_assoc_change.sac_assoc_id);
				if (new_con)
					clear_bit(CF_CONNECT_PENDING, &con->flags);
				return;
			}
			make_sockaddr(&prim.ssp_addr, 0, &addr_len);
			if (dlm_addr_to_nodeid(&prim.ssp_addr, &nodeid)) {
				unsigned char *b=(unsigned char *)&prim.ssp_addr;
				log_print("reject connect from unknown addr");
				print_hex_dump_bytes("ss: ", DUMP_PREFIX_NONE, 
						     b, sizeof(struct sockaddr_storage));
				sctp_send_shutdown(prim.ssp_assoc_id);
				return;
			}

			new_con = nodeid2con(nodeid, GFP_NOFS);
			if (!new_con)
				return;

			/* Peel off a new sock */
			sctp_lock_sock(con->sock->sk);
			ret = sctp_do_peeloff(con->sock->sk,
				sn->sn_assoc_change.sac_assoc_id,
				&new_con->sock);
			sctp_release_sock(con->sock->sk);
			if (ret < 0) {
				log_print("Can't peel off a socket for "
					  "connection %d to node %d: err=%d",
					  (int)sn->sn_assoc_change.sac_assoc_id,
					  nodeid, ret);
				return;
			}
			add_sock(new_con->sock, new_con);

			log_print("connecting to %d sctp association %d",
				 nodeid, (int)sn->sn_assoc_change.sac_assoc_id);

			/* Send any pending writes */
			clear_bit(CF_CONNECT_PENDING, &new_con->flags);
			clear_bit(CF_INIT_PENDING, &con->flags);
			if (!test_and_set_bit(CF_WRITE_PENDING, &new_con->flags)) {
				queue_work(send_workqueue, &new_con->swork);
			}
			if (!test_and_set_bit(CF_READ_PENDING, &new_con->flags))
				queue_work(recv_workqueue, &new_con->rwork);
		}
		break;

		case SCTP_COMM_LOST:
		case SCTP_SHUTDOWN_COMP:
		{
			con = assoc2con(sn->sn_assoc_change.sac_assoc_id);
			if (con) {
				con->sctp_assoc = 0;
			}
		}
		break;

		/* We don't know which INIT failed, so clear the PENDING flags
		 * on them all.  if assoc_id is zero then it will then try
		 * again */

		case SCTP_CANT_STR_ASSOC:
		{
			log_print("Can't start SCTP association - retrying");
			sctp_init_failed();
		}
		break;

		default:
			log_print("unexpected SCTP assoc change id=%d state=%d",
				  (int)sn->sn_assoc_change.sac_assoc_id,
				  sn->sn_assoc_change.sac_state);
		}
=======
static void dlm_page_release(struct kref *kref)
{
	struct writequeue_entry *e = container_of(kref, struct writequeue_entry,
						  ref);

	__free_page(e->page);
	dlm_free_writequeue(e);
}

static void dlm_msg_release(struct kref *kref)
{
	struct dlm_msg *msg = container_of(kref, struct dlm_msg, ref);

	kref_put(&msg->entry->ref, dlm_page_release);
	dlm_free_msg(msg);
}

static void free_entry(struct writequeue_entry *e)
{
	struct dlm_msg *msg, *tmp;

	list_for_each_entry_safe(msg, tmp, &e->msgs, list) {
		if (msg->orig_msg) {
			msg->orig_msg->retransmit = false;
			kref_put(&msg->orig_msg->ref, dlm_msg_release);
		}

		list_del(&msg->list);
		kref_put(&msg->ref, dlm_msg_release);
	}

	list_del(&e->list);
	kref_put(&e->ref, dlm_page_release);
}

static void dlm_close_sock(struct socket **sock)
{
	lock_sock((*sock)->sk);
	restore_callbacks((*sock)->sk);
	release_sock((*sock)->sk);

	sock_release(*sock);
	*sock = NULL;
}

static void allow_connection_io(struct connection *con)
{
	if (con->othercon)
		clear_bit(CF_IO_STOP, &con->othercon->flags);
	clear_bit(CF_IO_STOP, &con->flags);
}

static void stop_connection_io(struct connection *con)
{
	if (con->othercon)
		stop_connection_io(con->othercon);

	spin_lock_bh(&con->writequeue_lock);
	set_bit(CF_IO_STOP, &con->flags);
	spin_unlock_bh(&con->writequeue_lock);

	down_write(&con->sock_lock);
	if (con->sock) {
		lock_sock(con->sock->sk);
		restore_callbacks(con->sock->sk);
		release_sock(con->sock->sk);
	}
	up_write(&con->sock_lock);

	cancel_work_sync(&con->swork);
	cancel_work_sync(&con->rwork);
}

/* Close a remote connection and tidy up */
static void close_connection(struct connection *con, bool and_other)
{
	struct writequeue_entry *e;

	if (con->othercon && and_other)
		close_connection(con->othercon, false);

	down_write(&con->sock_lock);
	if (!con->sock) {
		up_write(&con->sock_lock);
		return;
	}

	dlm_close_sock(&con->sock);

	/* if we send a writequeue entry only a half way, we drop the
	 * whole entry because reconnection and that we not start of the
	 * middle of a msg which will confuse the other end.
	 *
	 * we can always drop messages because retransmits, but what we
	 * cannot allow is to transmit half messages which may be processed
	 * at the other side.
	 *
	 * our policy is to start on a clean state when disconnects, we don't
	 * know what's send/received on transport layer in this case.
	 */
	spin_lock_bh(&con->writequeue_lock);
	if (!list_empty(&con->writequeue)) {
		e = list_first_entry(&con->writequeue, struct writequeue_entry,
				     list);
		if (e->dirty)
			free_entry(e);
	}
	spin_unlock_bh(&con->writequeue_lock);

	con->rx_leftover = 0;
	con->retries = 0;
	clear_bit(CF_APP_LIMITED, &con->flags);
	clear_bit(CF_RECV_PENDING, &con->flags);
	clear_bit(CF_SEND_PENDING, &con->flags);
	up_write(&con->sock_lock);
}

static void shutdown_connection(struct connection *con, bool and_other)
{
	int ret;

	if (con->othercon && and_other)
		shutdown_connection(con->othercon, false);

	flush_workqueue(io_workqueue);
	down_read(&con->sock_lock);
	/* nothing to shutdown */
	if (!con->sock) {
		up_read(&con->sock_lock);
		return;
	}

	ret = kernel_sock_shutdown(con->sock, SHUT_WR);
	up_read(&con->sock_lock);
	if (ret) {
		log_print("Connection %p failed to shutdown: %d will force close",
			  con, ret);
		goto force_close;
	} else {
		ret = wait_event_timeout(con->shutdown_wait, !con->sock,
					 DLM_SHUTDOWN_WAIT_TIMEOUT);
		if (ret == 0) {
			log_print("Connection %p shutdown timed out, will force close",
				  con);
			goto force_close;
		}
	}

	return;

force_close:
	close_connection(con, false);
}

static struct processqueue_entry *new_processqueue_entry(int nodeid,
							 int buflen)
{
	struct processqueue_entry *pentry;

	pentry = kmalloc(sizeof(*pentry), GFP_NOFS);
	if (!pentry)
		return NULL;

	pentry->buf = kmalloc(buflen, GFP_NOFS);
	if (!pentry->buf) {
		kfree(pentry);
		return NULL;
	}

	pentry->nodeid = nodeid;
	return pentry;
}

static void free_processqueue_entry(struct processqueue_entry *pentry)
{
	kfree(pentry->buf);
	kfree(pentry);
}

struct dlm_processed_nodes {
	int nodeid;

	struct list_head list;
};

static void process_dlm_messages(struct work_struct *work)
{
	struct processqueue_entry *pentry;

	spin_lock(&processqueue_lock);
	pentry = list_first_entry_or_null(&processqueue,
					  struct processqueue_entry, list);
	if (WARN_ON_ONCE(!pentry)) {
		process_dlm_messages_pending = false;
		spin_unlock(&processqueue_lock);
		return;
	}

	list_del(&pentry->list);
	atomic_dec(&processqueue_count);
	spin_unlock(&processqueue_lock);

	for (;;) {
		dlm_process_incoming_buffer(pentry->nodeid, pentry->buf,
					    pentry->buflen);
		free_processqueue_entry(pentry);

		spin_lock(&processqueue_lock);
		pentry = list_first_entry_or_null(&processqueue,
						  struct processqueue_entry, list);
		if (!pentry) {
			process_dlm_messages_pending = false;
			spin_unlock(&processqueue_lock);
			break;
		}

		list_del(&pentry->list);
		atomic_dec(&processqueue_count);
		spin_unlock(&processqueue_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

/* Data received from remote end */
<<<<<<< HEAD
static int receive_from_sock(struct connection *con)
{
	int ret = 0;
	struct msghdr msg = {};
	struct kvec iov[2];
	unsigned len;
	int r;
	int call_again_soon = 0;
	int nvec;
	char incmsg[CMSG_SPACE(sizeof(struct sctp_sndrcvinfo))];

	mutex_lock(&con->sock_mutex);

	if (con->sock == NULL) {
		ret = -EAGAIN;
		goto out_close;
	}

	if (con->rx_page == NULL) {
		/*
		 * This doesn't need to be atomic, but I think it should
		 * improve performance if it is.
		 */
		con->rx_page = alloc_page(GFP_ATOMIC);
		if (con->rx_page == NULL)
			goto out_resched;
		cbuf_init(&con->cb, PAGE_CACHE_SIZE);
	}

	/* Only SCTP needs these really */
	memset(&incmsg, 0, sizeof(incmsg));
	msg.msg_control = incmsg;
	msg.msg_controllen = sizeof(incmsg);

	/*
	 * iov[0] is the bit of the circular buffer between the current end
	 * point (cb.base + cb.len) and the end of the buffer.
	 */
	iov[0].iov_len = con->cb.base - cbuf_data(&con->cb);
	iov[0].iov_base = page_address(con->rx_page) + cbuf_data(&con->cb);
	iov[1].iov_len = 0;
	nvec = 1;

	/*
	 * iov[1] is the bit of the circular buffer between the start of the
	 * buffer and the start of the currently used section (cb.base)
	 */
	if (cbuf_data(&con->cb) >= con->cb.base) {
		iov[0].iov_len = PAGE_CACHE_SIZE - cbuf_data(&con->cb);
		iov[1].iov_len = con->cb.base;
		iov[1].iov_base = page_address(con->rx_page);
		nvec = 2;
	}
	len = iov[0].iov_len + iov[1].iov_len;

	r = ret = kernel_recvmsg(con->sock, &msg, iov, nvec, len,
			       MSG_DONTWAIT | MSG_NOSIGNAL);
	if (ret <= 0)
		goto out_close;

	/* Process SCTP notifications */
	if (msg.msg_flags & MSG_NOTIFICATION) {
		msg.msg_control = incmsg;
		msg.msg_controllen = sizeof(incmsg);

		process_sctp_notification(con, &msg,
				page_address(con->rx_page) + con->cb.base);
		mutex_unlock(&con->sock_mutex);
		return 0;
	}
	BUG_ON(con->nodeid == 0);

	if (ret == len)
		call_again_soon = 1;
	cbuf_add(&con->cb, ret);
	ret = dlm_process_incoming_buffer(con->nodeid,
					  page_address(con->rx_page),
					  con->cb.base, con->cb.len,
					  PAGE_CACHE_SIZE);
	if (ret == -EBADMSG) {
		log_print("lowcomms: addr=%p, base=%u, len=%u, "
			  "iov_len=%u, iov_base[0]=%p, read=%d",
			  page_address(con->rx_page), con->cb.base, con->cb.len,
			  len, iov[0].iov_base, r);
	}
	if (ret < 0)
		goto out_close;
	cbuf_eat(&con->cb, ret);

	if (cbuf_empty(&con->cb) && !call_again_soon) {
		__free_page(con->rx_page);
		con->rx_page = NULL;
	}

	if (call_again_soon)
		goto out_resched;
	mutex_unlock(&con->sock_mutex);
	return 0;

out_resched:
	if (!test_and_set_bit(CF_READ_PENDING, &con->flags))
		queue_work(recv_workqueue, &con->rwork);
	mutex_unlock(&con->sock_mutex);
	return -EAGAIN;

out_close:
	mutex_unlock(&con->sock_mutex);
	if (ret != -EAGAIN) {
		close_connection(con, false);
		/* Reconnect when there is something to send */
	}
	/* Don't return success if we really got EOF */
	if (ret == 0)
		ret = -EAGAIN;

	return ret;
}

/* Listening socket is busy, accept a connection */
static int tcp_accept_from_sock(struct connection *con)
{
	int result;
	struct sockaddr_storage peeraddr;
	struct socket *newsock;
	int len;
	int nodeid;
	struct connection *newcon;
	struct connection *addcon;

	memset(&peeraddr, 0, sizeof(peeraddr));
	result = sock_create_kern(dlm_local_addr[0]->ss_family, SOCK_STREAM,
				  IPPROTO_TCP, &newsock);
	if (result < 0)
		return -ENOMEM;

	mutex_lock_nested(&con->sock_mutex, 0);

	result = -ENOTCONN;
	if (con->sock == NULL)
		goto accept_err;

	newsock->type = con->sock->type;
	newsock->ops = con->sock->ops;

	result = con->sock->ops->accept(con->sock, newsock, O_NONBLOCK);
	if (result < 0)
=======
static int receive_from_sock(struct connection *con, int buflen)
{
	struct processqueue_entry *pentry;
	int ret, buflen_real;
	struct msghdr msg;
	struct kvec iov;

	pentry = new_processqueue_entry(con->nodeid, buflen);
	if (!pentry)
		return DLM_IO_RESCHED;

	memcpy(pentry->buf, con->rx_leftover_buf, con->rx_leftover);

	/* calculate new buffer parameter regarding last receive and
	 * possible leftover bytes
	 */
	iov.iov_base = pentry->buf + con->rx_leftover;
	iov.iov_len = buflen - con->rx_leftover;

	memset(&msg, 0, sizeof(msg));
	msg.msg_flags = MSG_DONTWAIT | MSG_NOSIGNAL;
	clear_bit(CF_RECV_INTR, &con->flags);
again:
	ret = kernel_recvmsg(con->sock, &msg, &iov, 1, iov.iov_len,
			     msg.msg_flags);
	trace_dlm_recv(con->nodeid, ret);
	if (ret == -EAGAIN) {
		lock_sock(con->sock->sk);
		if (test_and_clear_bit(CF_RECV_INTR, &con->flags)) {
			release_sock(con->sock->sk);
			goto again;
		}

		clear_bit(CF_RECV_PENDING, &con->flags);
		release_sock(con->sock->sk);
		free_processqueue_entry(pentry);
		return DLM_IO_END;
	} else if (ret == 0) {
		/* close will clear CF_RECV_PENDING */
		free_processqueue_entry(pentry);
		return DLM_IO_EOF;
	} else if (ret < 0) {
		free_processqueue_entry(pentry);
		return ret;
	}

	/* new buflen according readed bytes and leftover from last receive */
	buflen_real = ret + con->rx_leftover;
	ret = dlm_validate_incoming_buffer(con->nodeid, pentry->buf,
					   buflen_real);
	if (ret < 0) {
		free_processqueue_entry(pentry);
		return ret;
	}

	pentry->buflen = ret;

	/* calculate leftover bytes from process and put it into begin of
	 * the receive buffer, so next receive we have the full message
	 * at the start address of the receive buffer.
	 */
	con->rx_leftover = buflen_real - ret;
	memmove(con->rx_leftover_buf, pentry->buf + ret,
		con->rx_leftover);

	spin_lock(&processqueue_lock);
	ret = atomic_inc_return(&processqueue_count);
	list_add_tail(&pentry->list, &processqueue);
	if (!process_dlm_messages_pending) {
		process_dlm_messages_pending = true;
		queue_work(process_workqueue, &process_work);
	}
	spin_unlock(&processqueue_lock);

	if (ret > DLM_MAX_PROCESS_BUFFERS)
		return DLM_IO_FLUSH;

	return DLM_IO_SUCCESS;
}

/* Listening socket is busy, accept a connection */
static int accept_from_sock(void)
{
	struct sockaddr_storage peeraddr;
	int len, idx, result, nodeid;
	struct connection *newcon;
	struct socket *newsock;
	unsigned int mark;

	result = kernel_accept(listen_con.sock, &newsock, O_NONBLOCK);
	if (result == -EAGAIN)
		return DLM_IO_END;
	else if (result < 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto accept_err;

	/* Get the connected socket's peer */
	memset(&peeraddr, 0, sizeof(peeraddr));
<<<<<<< HEAD
	if (newsock->ops->getname(newsock, (struct sockaddr *)&peeraddr,
				  &len, 2)) {
=======
	len = newsock->ops->getname(newsock, (struct sockaddr *)&peeraddr, 2);
	if (len < 0) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		result = -ECONNABORTED;
		goto accept_err;
	}

	/* Get the new node's NODEID */
	make_sockaddr(&peeraddr, 0, &len);
<<<<<<< HEAD
	if (dlm_addr_to_nodeid(&peeraddr, &nodeid)) {
		unsigned char *b=(unsigned char *)&peeraddr;
		log_print("connect from non cluster node");
		print_hex_dump_bytes("ss: ", DUMP_PREFIX_NONE, 
				     b, sizeof(struct sockaddr_storage));
		sock_release(newsock);
		mutex_unlock(&con->sock_mutex);
=======
	if (addr_to_nodeid(&peeraddr, &nodeid, &mark)) {
		switch (peeraddr.ss_family) {
		case AF_INET: {
			struct sockaddr_in *sin = (struct sockaddr_in *)&peeraddr;

			log_print("connect from non cluster IPv4 node %pI4",
				  &sin->sin_addr);
			break;
		}
#if IS_ENABLED(CONFIG_IPV6)
		case AF_INET6: {
			struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *)&peeraddr;

			log_print("connect from non cluster IPv6 node %pI6c",
				  &sin6->sin6_addr);
			break;
		}
#endif
		default:
			log_print("invalid family from non cluster node");
			break;
		}

		sock_release(newsock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -1;
	}

	log_print("got connection from %d", nodeid);

	/*  Check to see if we already have a connection to this node. This
	 *  could happen if the two nodes initiate a connection at roughly
	 *  the same time and the connections cross on the wire.
	 *  In this case we store the incoming one in "othercon"
	 */
<<<<<<< HEAD
	newcon = nodeid2con(nodeid, GFP_NOFS);
	if (!newcon) {
		result = -ENOMEM;
		goto accept_err;
	}
	mutex_lock_nested(&newcon->sock_mutex, 1);
=======
	idx = srcu_read_lock(&connections_srcu);
	newcon = nodeid2con(nodeid, 0);
	if (WARN_ON_ONCE(!newcon)) {
		srcu_read_unlock(&connections_srcu, idx);
		result = -ENOENT;
		goto accept_err;
	}

	sock_set_mark(newsock->sk, mark);

	down_write(&newcon->sock_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (newcon->sock) {
		struct connection *othercon = newcon->othercon;

		if (!othercon) {
<<<<<<< HEAD
			othercon = kmem_cache_zalloc(con_cache, GFP_NOFS);
			if (!othercon) {
				log_print("failed to allocate incoming socket");
				mutex_unlock(&newcon->sock_mutex);
				result = -ENOMEM;
				goto accept_err;
			}
			othercon->nodeid = nodeid;
			othercon->rx_action = receive_from_sock;
			mutex_init(&othercon->sock_mutex);
			INIT_WORK(&othercon->swork, process_send_sockets);
			INIT_WORK(&othercon->rwork, process_recv_sockets);
			set_bit(CF_IS_OTHERCON, &othercon->flags);
		}
		if (!othercon->sock) {
			newcon->othercon = othercon;
			othercon->sock = newsock;
			newsock->sk->sk_user_data = othercon;
			add_sock(newsock, othercon);
			addcon = othercon;
		}
		else {
			printk("Extra connection from node %d attempted\n", nodeid);
			result = -EAGAIN;
			mutex_unlock(&newcon->sock_mutex);
			goto accept_err;
		}
	}
	else {
		newsock->sk->sk_user_data = newcon;
		newcon->rx_action = receive_from_sock;
		add_sock(newsock, newcon);
		addcon = newcon;
	}

	mutex_unlock(&newcon->sock_mutex);

	/*
	 * Add it to the active queue in case we got data
	 * between processing the accept adding the socket
	 * to the read_sockets list
	 */
	if (!test_and_set_bit(CF_READ_PENDING, &addcon->flags))
		queue_work(recv_workqueue, &addcon->rwork);
	mutex_unlock(&con->sock_mutex);

	return 0;

accept_err:
	mutex_unlock(&con->sock_mutex);
	sock_release(newsock);

	if (result != -EAGAIN)
		log_print("error accepting connection from node: %d", result);
	return result;
}

static void free_entry(struct writequeue_entry *e)
{
	__free_page(e->page);
	kfree(e);
}

/* Initiate an SCTP association.
   This is a special case of send_to_sock() in that we don't yet have a
   peeled-off socket for this association, so we use the listening socket
   and add the primary IP address of the remote node.
 */
static void sctp_init_assoc(struct connection *con)
{
	struct sockaddr_storage rem_addr;
	char outcmsg[CMSG_SPACE(sizeof(struct sctp_sndrcvinfo))];
	struct msghdr outmessage;
	struct cmsghdr *cmsg;
	struct sctp_sndrcvinfo *sinfo;
	struct connection *base_con;
	struct writequeue_entry *e;
	int len, offset;
	int ret;
	int addrlen;
	struct kvec iov[1];

	if (test_and_set_bit(CF_INIT_PENDING, &con->flags))
		return;

	if (con->retries++ > MAX_CONNECT_RETRIES)
		return;

	if (nodeid_to_addr(con->nodeid, (struct sockaddr *)&rem_addr)) {
		log_print("no address for nodeid %d", con->nodeid);
		return;
	}
	base_con = nodeid2con(0, 0);
	BUG_ON(base_con == NULL);

	make_sockaddr(&rem_addr, dlm_config.ci_tcp_port, &addrlen);

	outmessage.msg_name = &rem_addr;
	outmessage.msg_namelen = addrlen;
	outmessage.msg_control = outcmsg;
	outmessage.msg_controllen = sizeof(outcmsg);
	outmessage.msg_flags = MSG_EOR;

	spin_lock(&con->writequeue_lock);

	if (list_empty(&con->writequeue)) {
		spin_unlock(&con->writequeue_lock);
		log_print("writequeue empty for nodeid %d", con->nodeid);
		return;
	}

	e = list_first_entry(&con->writequeue, struct writequeue_entry, list);
	len = e->len;
	offset = e->offset;
	spin_unlock(&con->writequeue_lock);

	/* Send the first block off the write queue */
	iov[0].iov_base = page_address(e->page)+offset;
	iov[0].iov_len = len;

	cmsg = CMSG_FIRSTHDR(&outmessage);
	cmsg->cmsg_level = IPPROTO_SCTP;
	cmsg->cmsg_type = SCTP_SNDRCV;
	cmsg->cmsg_len = CMSG_LEN(sizeof(struct sctp_sndrcvinfo));
	sinfo = CMSG_DATA(cmsg);
	memset(sinfo, 0x00, sizeof(struct sctp_sndrcvinfo));
	sinfo->sinfo_ppid = cpu_to_le32(dlm_our_nodeid());
	outmessage.msg_controllen = cmsg->cmsg_len;

	ret = kernel_sendmsg(base_con->sock, &outmessage, iov, 1, len);
	if (ret < 0) {
		log_print("Send first packet to node %d failed: %d",
			  con->nodeid, ret);

		/* Try again later */
		clear_bit(CF_CONNECT_PENDING, &con->flags);
		clear_bit(CF_INIT_PENDING, &con->flags);
	}
	else {
		spin_lock(&con->writequeue_lock);
		e->offset += ret;
		e->len -= ret;

		if (e->len == 0 && e->users == 0) {
			list_del(&e->list);
			free_entry(e);
		}
		spin_unlock(&con->writequeue_lock);
	}
}

/* Connect a new socket to its peer */
static void tcp_connect_to_sock(struct connection *con)
{
	int result = -EHOSTUNREACH;
	struct sockaddr_storage saddr, src_addr;
	int addr_len;
	struct socket *sock = NULL;
	int one = 1;

	if (con->nodeid == 0) {
		log_print("attempt to connect sock 0 foiled");
		return;
	}

	mutex_lock(&con->sock_mutex);
	if (con->retries++ > MAX_CONNECT_RETRIES)
		goto out;

	/* Some odd races can cause double-connects, ignore them */
	if (con->sock) {
		result = 0;
		goto out;
	}

	/* Create a socket to communicate with */
	result = sock_create_kern(dlm_local_addr[0]->ss_family, SOCK_STREAM,
				  IPPROTO_TCP, &sock);
	if (result < 0)
		goto out_err;

	memset(&saddr, 0, sizeof(saddr));
	if (dlm_nodeid_to_addr(con->nodeid, &saddr))
		goto out_err;

	sock->sk->sk_user_data = con;
	con->rx_action = receive_from_sock;
	con->connect_action = tcp_connect_to_sock;
	add_sock(sock, con);

	/* Bind to our cluster-known address connecting to avoid
	   routing problems */
	memcpy(&src_addr, dlm_local_addr[0], sizeof(src_addr));
	make_sockaddr(&src_addr, 0, &addr_len);
	result = sock->ops->bind(sock, (struct sockaddr *) &src_addr,
				 addr_len);
	if (result < 0) {
		log_print("could not bind for connect: %d", result);
		/* This *may* not indicate a critical error */
	}

	make_sockaddr(&saddr, dlm_config.ci_tcp_port, &addr_len);

	log_print("connecting to %d", con->nodeid);

	/* Turn off Nagle's algorithm */
	kernel_setsockopt(sock, SOL_TCP, TCP_NODELAY, (char *)&one,
			  sizeof(one));

	result =
		sock->ops->connect(sock, (struct sockaddr *)&saddr, addr_len,
				   O_NONBLOCK);
	if (result == -EINPROGRESS)
		result = 0;
	if (result == 0)
		goto out;

out_err:
	if (con->sock) {
		sock_release(con->sock);
		con->sock = NULL;
	} else if (sock) {
		sock_release(sock);
	}
	/*
	 * Some errors are fatal and this list might need adjusting. For other
	 * errors we try again until the max number of retries is reached.
	 */
	if (result != -EHOSTUNREACH && result != -ENETUNREACH &&
	    result != -ENETDOWN && result != -EINVAL
	    && result != -EPROTONOSUPPORT) {
		lowcomms_connect_sock(con);
		result = 0;
	}
out:
	mutex_unlock(&con->sock_mutex);
	return;
}

static struct socket *tcp_create_listen_sock(struct connection *con,
					     struct sockaddr_storage *saddr)
{
	struct socket *sock = NULL;
	int result = 0;
	int one = 1;
	int addr_len;

	if (dlm_local_addr[0]->ss_family == AF_INET)
		addr_len = sizeof(struct sockaddr_in);
	else
		addr_len = sizeof(struct sockaddr_in6);

	/* Create a socket to communicate with */
	result = sock_create_kern(dlm_local_addr[0]->ss_family, SOCK_STREAM,
				  IPPROTO_TCP, &sock);
	if (result < 0) {
		log_print("Can't create listening comms socket");
		goto create_out;
	}

	/* Turn off Nagle's algorithm */
	kernel_setsockopt(sock, SOL_TCP, TCP_NODELAY, (char *)&one,
			  sizeof(one));

	result = kernel_setsockopt(sock, SOL_SOCKET, SO_REUSEADDR,
				   (char *)&one, sizeof(one));

	if (result < 0) {
		log_print("Failed to set SO_REUSEADDR on socket: %d", result);
	}
	sock->sk->sk_user_data = con;
	con->rx_action = tcp_accept_from_sock;
	con->connect_action = tcp_connect_to_sock;
	con->sock = sock;

	/* Bind to our port */
	make_sockaddr(saddr, dlm_config.ci_tcp_port, &addr_len);
	result = sock->ops->bind(sock, (struct sockaddr *) saddr, addr_len);
	if (result < 0) {
		log_print("Can't bind to port %d", dlm_config.ci_tcp_port);
		sock_release(sock);
		sock = NULL;
		con->sock = NULL;
		goto create_out;
	}
	result = kernel_setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE,
				 (char *)&one, sizeof(one));
	if (result < 0) {
		log_print("Set keepalive failed: %d", result);
	}

	result = sock->ops->listen(sock, 5);
	if (result < 0) {
		log_print("Can't listen on port %d", dlm_config.ci_tcp_port);
		sock_release(sock);
		sock = NULL;
		goto create_out;
	}

create_out:
	return sock;
=======
			othercon = kzalloc(sizeof(*othercon), GFP_NOFS);
			if (!othercon) {
				log_print("failed to allocate incoming socket");
				up_write(&newcon->sock_lock);
				srcu_read_unlock(&connections_srcu, idx);
				result = -ENOMEM;
				goto accept_err;
			}

			dlm_con_init(othercon, nodeid);
			lockdep_set_subclass(&othercon->sock_lock, 1);
			newcon->othercon = othercon;
			set_bit(CF_IS_OTHERCON, &othercon->flags);
		} else {
			/* close other sock con if we have something new */
			close_connection(othercon, false);
		}

		down_write(&othercon->sock_lock);
		add_sock(newsock, othercon);

		/* check if we receved something while adding */
		lock_sock(othercon->sock->sk);
		lowcomms_queue_rwork(othercon);
		release_sock(othercon->sock->sk);
		up_write(&othercon->sock_lock);
	}
	else {
		/* accept copies the sk after we've saved the callbacks, so we
		   don't want to save them a second time or comm errors will
		   result in calling sk_error_report recursively. */
		add_sock(newsock, newcon);

		/* check if we receved something while adding */
		lock_sock(newcon->sock->sk);
		lowcomms_queue_rwork(newcon);
		release_sock(newcon->sock->sk);
	}
	up_write(&newcon->sock_lock);
	srcu_read_unlock(&connections_srcu, idx);

	return DLM_IO_SUCCESS;

accept_err:
	if (newsock)
		sock_release(newsock);

	return result;
}

/*
 * writequeue_entry_complete - try to delete and free write queue entry
 * @e: write queue entry to try to delete
 * @completed: bytes completed
 *
 * writequeue_lock must be held.
 */
static void writequeue_entry_complete(struct writequeue_entry *e, int completed)
{
	e->offset += completed;
	e->len -= completed;
	/* signal that page was half way transmitted */
	e->dirty = true;

	if (e->len == 0 && e->users == 0)
		free_entry(e);
}

/*
 * sctp_bind_addrs - bind a SCTP socket to all our addresses
 */
static int sctp_bind_addrs(struct socket *sock, uint16_t port)
{
	struct sockaddr_storage localaddr;
	struct sockaddr *addr = (struct sockaddr *)&localaddr;
	int i, addr_len, result = 0;

	for (i = 0; i < dlm_local_count; i++) {
		memcpy(&localaddr, &dlm_local_addr[i], sizeof(localaddr));
		make_sockaddr(&localaddr, port, &addr_len);

		if (!i)
			result = kernel_bind(sock, addr, addr_len);
		else
			result = sock_bind_add(sock->sk, addr, addr_len);

		if (result < 0) {
			log_print("Can't bind to %d addr number %d, %d.\n",
				  port, i + 1, result);
			break;
		}
	}
	return result;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Get local addresses */
static void init_local(void)
{
<<<<<<< HEAD
	struct sockaddr_storage sas, *addr;
=======
	struct sockaddr_storage sas;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int i;

	dlm_local_count = 0;
	for (i = 0; i < DLM_MAX_ADDR_COUNT; i++) {
		if (dlm_our_addr(&sas, i))
			break;

<<<<<<< HEAD
		addr = kmalloc(sizeof(*addr), GFP_NOFS);
		if (!addr)
			break;
		memcpy(addr, &sas, sizeof(*addr));
		dlm_local_addr[dlm_local_count++] = addr;
	}
}

/* Bind to an IP address. SCTP allows multiple address so it can do
   multi-homing */
static int add_sctp_bind_addr(struct connection *sctp_con,
			      struct sockaddr_storage *addr,
			      int addr_len, int num)
{
	int result = 0;

	if (num == 1)
		result = kernel_bind(sctp_con->sock,
				     (struct sockaddr *) addr,
				     addr_len);
	else
		result = kernel_setsockopt(sctp_con->sock, SOL_SCTP,
					   SCTP_SOCKOPT_BINDX_ADD,
					   (char *)addr, addr_len);

	if (result < 0)
		log_print("Can't bind to port %d addr number %d",
			  dlm_config.ci_tcp_port, num);

	return result;
}

/* Initialise SCTP socket and bind to all interfaces */
static int sctp_listen_for_all(void)
{
	struct socket *sock = NULL;
	struct sockaddr_storage localaddr;
	struct sctp_event_subscribe subscribe;
	int result = -EINVAL, num = 1, i, addr_len;
	struct connection *con = nodeid2con(0, GFP_NOFS);
	int bufsize = NEEDED_RMEM;

	if (!con)
		return -ENOMEM;

	log_print("Using SCTP for communications");

	result = sock_create_kern(dlm_local_addr[0]->ss_family, SOCK_SEQPACKET,
				  IPPROTO_SCTP, &sock);
	if (result < 0) {
		log_print("Can't create comms socket, check SCTP is loaded");
		goto out;
	}

	/* Listen for events */
	memset(&subscribe, 0, sizeof(subscribe));
	subscribe.sctp_data_io_event = 1;
	subscribe.sctp_association_event = 1;
	subscribe.sctp_send_failure_event = 1;
	subscribe.sctp_shutdown_event = 1;
	subscribe.sctp_partial_delivery_event = 1;

	result = kernel_setsockopt(sock, SOL_SOCKET, SO_RCVBUFFORCE,
				 (char *)&bufsize, sizeof(bufsize));
	if (result)
		log_print("Error increasing buffer space on socket %d", result);

	result = kernel_setsockopt(sock, SOL_SCTP, SCTP_EVENTS,
				   (char *)&subscribe, sizeof(subscribe));
	if (result < 0) {
		log_print("Failed to set SCTP_EVENTS on socket: result=%d",
			  result);
		goto create_delsock;
	}

	/* Init con struct */
	sock->sk->sk_user_data = con;
	con->sock = sock;
	con->sock->sk->sk_data_ready = lowcomms_data_ready;
	con->rx_action = receive_from_sock;
	con->connect_action = sctp_init_assoc;

	/* Bind to all interfaces. */
	for (i = 0; i < dlm_local_count; i++) {
		memcpy(&localaddr, dlm_local_addr[i], sizeof(localaddr));
		make_sockaddr(&localaddr, dlm_config.ci_tcp_port, &addr_len);

		result = add_sctp_bind_addr(con, &localaddr, addr_len, num);
		if (result)
			goto create_delsock;
		++num;
	}

	result = sock->ops->listen(sock, 5);
	if (result < 0) {
		log_print("Can't set socket listening");
		goto create_delsock;
	}

	return 0;

create_delsock:
	sock_release(sock);
	con->sock = NULL;
out:
	return result;
}

static int tcp_listen_for_all(void)
{
	struct socket *sock = NULL;
	struct connection *con = nodeid2con(0, GFP_NOFS);
	int result = -EINVAL;

	if (!con)
		return -ENOMEM;

	/* We don't support multi-homed hosts */
	if (dlm_local_addr[1] != NULL) {
		log_print("TCP protocol can't handle multi-homed hosts, "
			  "try SCTP");
		return -EINVAL;
	}

	log_print("Using TCP for communications");

	sock = tcp_create_listen_sock(con, dlm_local_addr[0]);
	if (sock) {
		add_sock(sock, con);
		result = 0;
	}
	else {
		result = -EADDRINUSE;
	}

	return result;
}



static struct writequeue_entry *new_writequeue_entry(struct connection *con,
						     gfp_t allocation)
{
	struct writequeue_entry *entry;

	entry = kmalloc(sizeof(struct writequeue_entry), allocation);
	if (!entry)
		return NULL;

	entry->page = alloc_page(allocation);
	if (!entry->page) {
		kfree(entry);
=======
		memcpy(&dlm_local_addr[dlm_local_count++], &sas, sizeof(sas));
	}
}

static struct writequeue_entry *new_writequeue_entry(struct connection *con)
{
	struct writequeue_entry *entry;

	entry = dlm_allocate_writequeue();
	if (!entry)
		return NULL;

	entry->page = alloc_page(GFP_ATOMIC | __GFP_ZERO);
	if (!entry->page) {
		dlm_free_writequeue(entry);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return NULL;
	}

	entry->offset = 0;
	entry->len = 0;
	entry->end = 0;
<<<<<<< HEAD
	entry->users = 0;
	entry->con = con;

	return entry;
}

void *dlm_lowcomms_get_buffer(int nodeid, int len, gfp_t allocation, char **ppc)
{
	struct connection *con;
	struct writequeue_entry *e;
	int offset = 0;
	int users = 0;

	con = nodeid2con(nodeid, allocation);
	if (!con)
		return NULL;

	spin_lock(&con->writequeue_lock);
	e = list_entry(con->writequeue.prev, struct writequeue_entry, list);
	if ((&e->list == &con->writequeue) ||
	    (PAGE_CACHE_SIZE - e->end < len)) {
		e = NULL;
	} else {
		offset = e->end;
		e->end += len;
		users = e->users++;
	}
	spin_unlock(&con->writequeue_lock);

	if (e) {
	got_one:
		*ppc = page_address(e->page) + offset;
		return e;
	}

	e = new_writequeue_entry(con, allocation);
	if (e) {
		spin_lock(&con->writequeue_lock);
		offset = e->end;
		e->end += len;
		users = e->users++;
		list_add_tail(&e->list, &con->writequeue);
		spin_unlock(&con->writequeue_lock);
		goto got_one;
	}
	return NULL;
}

void dlm_lowcomms_commit_buffer(void *mh)
{
	struct writequeue_entry *e = (struct writequeue_entry *)mh;
	struct connection *con = e->con;
	int users;

	spin_lock(&con->writequeue_lock);
	users = --e->users;
	if (users)
		goto out;
	e->len = e->end - e->offset;
	spin_unlock(&con->writequeue_lock);

	if (!test_and_set_bit(CF_WRITE_PENDING, &con->flags)) {
		queue_work(send_workqueue, &con->swork);
	}
	return;

out:
	spin_unlock(&con->writequeue_lock);
	return;
}

/* Send a message */
static void send_to_sock(struct connection *con)
{
	int ret = 0;
	const int msg_flags = MSG_DONTWAIT | MSG_NOSIGNAL;
	struct writequeue_entry *e;
	int len, offset;
	int count = 0;

	mutex_lock(&con->sock_mutex);
	if (con->sock == NULL)
		goto out_connect;

	spin_lock(&con->writequeue_lock);
	for (;;) {
		e = list_entry(con->writequeue.next, struct writequeue_entry,
			       list);
		if ((struct list_head *) e == &con->writequeue)
			break;

		len = e->len;
		offset = e->offset;
		BUG_ON(len == 0 && e->users == 0);
		spin_unlock(&con->writequeue_lock);

		ret = 0;
		if (len) {
			ret = kernel_sendpage(con->sock, e->page, offset, len,
					      msg_flags);
			if (ret == -EAGAIN || ret == 0) {
				if (ret == -EAGAIN &&
				    test_bit(SOCK_ASYNC_NOSPACE, &con->sock->flags) &&
				    !test_and_set_bit(CF_APP_LIMITED, &con->flags)) {
					/* Notify TCP that we're limited by the
					 * application window size.
					 */
					set_bit(SOCK_NOSPACE, &con->sock->flags);
					con->sock->sk->sk_write_pending++;
				}
				cond_resched();
				goto out;
			}
			if (ret <= 0)
				goto send_error;
		}

		/* Don't starve people filling buffers */
		if (++count >= MAX_SEND_MSG_COUNT) {
			cond_resched();
			count = 0;
		}

		spin_lock(&con->writequeue_lock);
		e->offset += ret;
		e->len -= ret;

		if (e->len == 0 && e->users == 0) {
			list_del(&e->list);
			free_entry(e);
			continue;
		}
	}
	spin_unlock(&con->writequeue_lock);
out:
	mutex_unlock(&con->sock_mutex);
	return;

send_error:
	mutex_unlock(&con->sock_mutex);
	close_connection(con, false);
	lowcomms_connect_sock(con);
	return;

out_connect:
	mutex_unlock(&con->sock_mutex);
	if (!test_bit(CF_INIT_PENDING, &con->flags))
		lowcomms_connect_sock(con);
	return;
=======
	entry->dirty = false;
	entry->con = con;
	entry->users = 1;
	kref_init(&entry->ref);
	return entry;
}

static struct writequeue_entry *new_wq_entry(struct connection *con, int len,
					     char **ppc, void (*cb)(void *data),
					     void *data)
{
	struct writequeue_entry *e;

	spin_lock_bh(&con->writequeue_lock);
	if (!list_empty(&con->writequeue)) {
		e = list_last_entry(&con->writequeue, struct writequeue_entry, list);
		if (DLM_WQ_REMAIN_BYTES(e) >= len) {
			kref_get(&e->ref);

			*ppc = page_address(e->page) + e->end;
			if (cb)
				cb(data);

			e->end += len;
			e->users++;
			goto out;
		}
	}

	e = new_writequeue_entry(con);
	if (!e)
		goto out;

	kref_get(&e->ref);
	*ppc = page_address(e->page);
	e->end += len;
	if (cb)
		cb(data);

	list_add_tail(&e->list, &con->writequeue);

out:
	spin_unlock_bh(&con->writequeue_lock);
	return e;
};

static struct dlm_msg *dlm_lowcomms_new_msg_con(struct connection *con, int len,
						gfp_t allocation, char **ppc,
						void (*cb)(void *data),
						void *data)
{
	struct writequeue_entry *e;
	struct dlm_msg *msg;

	msg = dlm_allocate_msg(allocation);
	if (!msg)
		return NULL;

	kref_init(&msg->ref);

	e = new_wq_entry(con, len, ppc, cb, data);
	if (!e) {
		dlm_free_msg(msg);
		return NULL;
	}

	msg->retransmit = false;
	msg->orig_msg = NULL;
	msg->ppc = *ppc;
	msg->len = len;
	msg->entry = e;

	return msg;
}

/* avoid false positive for nodes_srcu, unlock happens in
 * dlm_lowcomms_commit_msg which is a must call if success
 */
#ifndef __CHECKER__
struct dlm_msg *dlm_lowcomms_new_msg(int nodeid, int len, gfp_t allocation,
				     char **ppc, void (*cb)(void *data),
				     void *data)
{
	struct connection *con;
	struct dlm_msg *msg;
	int idx;

	if (len > DLM_MAX_SOCKET_BUFSIZE ||
	    len < sizeof(struct dlm_header)) {
		BUILD_BUG_ON(PAGE_SIZE < DLM_MAX_SOCKET_BUFSIZE);
		log_print("failed to allocate a buffer of size %d", len);
		WARN_ON_ONCE(1);
		return NULL;
	}

	idx = srcu_read_lock(&connections_srcu);
	con = nodeid2con(nodeid, 0);
	if (WARN_ON_ONCE(!con)) {
		srcu_read_unlock(&connections_srcu, idx);
		return NULL;
	}

	msg = dlm_lowcomms_new_msg_con(con, len, allocation, ppc, cb, data);
	if (!msg) {
		srcu_read_unlock(&connections_srcu, idx);
		return NULL;
	}

	/* for dlm_lowcomms_commit_msg() */
	kref_get(&msg->ref);
	/* we assume if successful commit must called */
	msg->idx = idx;
	return msg;
}
#endif

static void _dlm_lowcomms_commit_msg(struct dlm_msg *msg)
{
	struct writequeue_entry *e = msg->entry;
	struct connection *con = e->con;
	int users;

	spin_lock_bh(&con->writequeue_lock);
	kref_get(&msg->ref);
	list_add(&msg->list, &e->msgs);

	users = --e->users;
	if (users)
		goto out;

	e->len = DLM_WQ_LENGTH_BYTES(e);

	lowcomms_queue_swork(con);

out:
	spin_unlock_bh(&con->writequeue_lock);
	return;
}

/* avoid false positive for nodes_srcu, lock was happen in
 * dlm_lowcomms_new_msg
 */
#ifndef __CHECKER__
void dlm_lowcomms_commit_msg(struct dlm_msg *msg)
{
	_dlm_lowcomms_commit_msg(msg);
	srcu_read_unlock(&connections_srcu, msg->idx);
	/* because dlm_lowcomms_new_msg() */
	kref_put(&msg->ref, dlm_msg_release);
}
#endif

void dlm_lowcomms_put_msg(struct dlm_msg *msg)
{
	kref_put(&msg->ref, dlm_msg_release);
}

/* does not held connections_srcu, usage lowcomms_error_report only */
int dlm_lowcomms_resend_msg(struct dlm_msg *msg)
{
	struct dlm_msg *msg_resend;
	char *ppc;

	if (msg->retransmit)
		return 1;

	msg_resend = dlm_lowcomms_new_msg_con(msg->entry->con, msg->len,
					      GFP_ATOMIC, &ppc, NULL, NULL);
	if (!msg_resend)
		return -ENOMEM;

	msg->retransmit = true;
	kref_get(&msg->ref);
	msg_resend->orig_msg = msg;

	memcpy(ppc, msg->ppc, msg->len);
	_dlm_lowcomms_commit_msg(msg_resend);
	dlm_lowcomms_put_msg(msg_resend);

	return 0;
}

/* Send a message */
static int send_to_sock(struct connection *con)
{
	struct writequeue_entry *e;
	struct bio_vec bvec;
	struct msghdr msg = {
		.msg_flags = MSG_SPLICE_PAGES | MSG_DONTWAIT | MSG_NOSIGNAL,
	};
	int len, offset, ret;

	spin_lock_bh(&con->writequeue_lock);
	e = con_next_wq(con);
	if (!e) {
		clear_bit(CF_SEND_PENDING, &con->flags);
		spin_unlock_bh(&con->writequeue_lock);
		return DLM_IO_END;
	}

	len = e->len;
	offset = e->offset;
	WARN_ON_ONCE(len == 0 && e->users == 0);
	spin_unlock_bh(&con->writequeue_lock);

	bvec_set_page(&bvec, e->page, len, offset);
	iov_iter_bvec(&msg.msg_iter, ITER_SOURCE, &bvec, 1, len);
	ret = sock_sendmsg(con->sock, &msg);
	trace_dlm_send(con->nodeid, ret);
	if (ret == -EAGAIN || ret == 0) {
		lock_sock(con->sock->sk);
		spin_lock_bh(&con->writequeue_lock);
		if (test_bit(SOCKWQ_ASYNC_NOSPACE, &con->sock->flags) &&
		    !test_and_set_bit(CF_APP_LIMITED, &con->flags)) {
			/* Notify TCP that we're limited by the
			 * application window size.
			 */
			set_bit(SOCK_NOSPACE, &con->sock->sk->sk_socket->flags);
			con->sock->sk->sk_write_pending++;

			clear_bit(CF_SEND_PENDING, &con->flags);
			spin_unlock_bh(&con->writequeue_lock);
			release_sock(con->sock->sk);

			/* wait for write_space() event */
			return DLM_IO_END;
		}
		spin_unlock_bh(&con->writequeue_lock);
		release_sock(con->sock->sk);

		return DLM_IO_RESCHED;
	} else if (ret < 0) {
		return ret;
	}

	spin_lock_bh(&con->writequeue_lock);
	writequeue_entry_complete(e, ret);
	spin_unlock_bh(&con->writequeue_lock);

	return DLM_IO_SUCCESS;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void clean_one_writequeue(struct connection *con)
{
	struct writequeue_entry *e, *safe;

<<<<<<< HEAD
	spin_lock(&con->writequeue_lock);
	list_for_each_entry_safe(e, safe, &con->writequeue, list) {
		list_del(&e->list);
		free_entry(e);
	}
	spin_unlock(&con->writequeue_lock);
=======
	spin_lock_bh(&con->writequeue_lock);
	list_for_each_entry_safe(e, safe, &con->writequeue, list) {
		free_entry(e);
	}
	spin_unlock_bh(&con->writequeue_lock);
}

static void connection_release(struct rcu_head *rcu)
{
	struct connection *con = container_of(rcu, struct connection, rcu);

	WARN_ON_ONCE(!list_empty(&con->writequeue));
	WARN_ON_ONCE(con->sock);
	kfree(con);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Called from recovery when it knows that a node has
   left the cluster */
int dlm_lowcomms_close(int nodeid)
{
	struct connection *con;
<<<<<<< HEAD

	log_print("closing connection to node %d", nodeid);
	con = nodeid2con(nodeid, 0);
	if (con) {
		clear_bit(CF_CONNECT_PENDING, &con->flags);
		clear_bit(CF_WRITE_PENDING, &con->flags);
		set_bit(CF_CLOSE, &con->flags);
		if (cancel_work_sync(&con->swork))
			log_print("canceled swork for node %d", nodeid);
		if (cancel_work_sync(&con->rwork))
			log_print("canceled rwork for node %d", nodeid);
		clean_one_writequeue(con);
		close_connection(con, true);
	}
	return 0;
}

/* Receive workqueue function */
static void process_recv_sockets(struct work_struct *work)
{
	struct connection *con = container_of(work, struct connection, rwork);
	int err;

	clear_bit(CF_READ_PENDING, &con->flags);
	do {
		err = con->rx_action(con);
	} while (!err);
}

/* Send workqueue function */
static void process_send_sockets(struct work_struct *work)
{
	struct connection *con = container_of(work, struct connection, swork);

	if (test_and_clear_bit(CF_CONNECT_PENDING, &con->flags)) {
		con->connect_action(con);
		set_bit(CF_WRITE_PENDING, &con->flags);
	}
	if (test_and_clear_bit(CF_WRITE_PENDING, &con->flags))
		send_to_sock(con);
}


/* Discard all entries on the write queues */
static void clean_writequeues(void)
{
	foreach_conn(clean_one_writequeue);
=======
	int idx;

	log_print("closing connection to node %d", nodeid);

	idx = srcu_read_lock(&connections_srcu);
	con = nodeid2con(nodeid, 0);
	if (WARN_ON_ONCE(!con)) {
		srcu_read_unlock(&connections_srcu, idx);
		return -ENOENT;
	}

	stop_connection_io(con);
	log_print("io handling for node: %d stopped", nodeid);
	close_connection(con, true);

	spin_lock(&connections_lock);
	hlist_del_rcu(&con->list);
	spin_unlock(&connections_lock);

	clean_one_writequeue(con);
	call_srcu(&connections_srcu, &con->rcu, connection_release);
	if (con->othercon) {
		clean_one_writequeue(con->othercon);
		call_srcu(&connections_srcu, &con->othercon->rcu, connection_release);
	}
	srcu_read_unlock(&connections_srcu, idx);

	/* for debugging we print when we are done to compare with other
	 * messages in between. This function need to be correctly synchronized
	 * with io handling
	 */
	log_print("closing connection to node %d done", nodeid);

	return 0;
}

/* Receive worker function */
static void process_recv_sockets(struct work_struct *work)
{
	struct connection *con = container_of(work, struct connection, rwork);
	int ret, buflen;

	down_read(&con->sock_lock);
	if (!con->sock) {
		up_read(&con->sock_lock);
		return;
	}

	buflen = READ_ONCE(dlm_config.ci_buffer_size);
	do {
		ret = receive_from_sock(con, buflen);
	} while (ret == DLM_IO_SUCCESS);
	up_read(&con->sock_lock);

	switch (ret) {
	case DLM_IO_END:
		/* CF_RECV_PENDING cleared */
		break;
	case DLM_IO_EOF:
		close_connection(con, false);
		wake_up(&con->shutdown_wait);
		/* CF_RECV_PENDING cleared */
		break;
	case DLM_IO_FLUSH:
		flush_workqueue(process_workqueue);
		fallthrough;
	case DLM_IO_RESCHED:
		cond_resched();
		queue_work(io_workqueue, &con->rwork);
		/* CF_RECV_PENDING not cleared */
		break;
	default:
		if (ret < 0) {
			if (test_bit(CF_IS_OTHERCON, &con->flags)) {
				close_connection(con, false);
			} else {
				spin_lock_bh(&con->writequeue_lock);
				lowcomms_queue_swork(con);
				spin_unlock_bh(&con->writequeue_lock);
			}

			/* CF_RECV_PENDING cleared for othercon
			 * we trigger send queue if not already done
			 * and process_send_sockets will handle it
			 */
			break;
		}

		WARN_ON_ONCE(1);
		break;
	}
}

static void process_listen_recv_socket(struct work_struct *work)
{
	int ret;

	if (WARN_ON_ONCE(!listen_con.sock))
		return;

	do {
		ret = accept_from_sock();
	} while (ret == DLM_IO_SUCCESS);

	if (ret < 0)
		log_print("critical error accepting connection: %d", ret);
}

static int dlm_connect(struct connection *con)
{
	struct sockaddr_storage addr;
	int result, addr_len;
	struct socket *sock;
	unsigned int mark;

	memset(&addr, 0, sizeof(addr));
	result = nodeid_to_addr(con->nodeid, &addr, NULL,
				dlm_proto_ops->try_new_addr, &mark);
	if (result < 0) {
		log_print("no address for nodeid %d", con->nodeid);
		return result;
	}

	/* Create a socket to communicate with */
	result = sock_create_kern(&init_net, dlm_local_addr[0].ss_family,
				  SOCK_STREAM, dlm_proto_ops->proto, &sock);
	if (result < 0)
		return result;

	sock_set_mark(sock->sk, mark);
	dlm_proto_ops->sockopts(sock);

	result = dlm_proto_ops->bind(sock);
	if (result < 0) {
		sock_release(sock);
		return result;
	}

	add_sock(sock, con);

	log_print_ratelimited("connecting to %d", con->nodeid);
	make_sockaddr(&addr, dlm_config.ci_tcp_port, &addr_len);
	result = dlm_proto_ops->connect(con, sock, (struct sockaddr *)&addr,
					addr_len);
	switch (result) {
	case -EINPROGRESS:
		/* not an error */
		fallthrough;
	case 0:
		break;
	default:
		if (result < 0)
			dlm_close_sock(&con->sock);

		break;
	}

	return result;
}

/* Send worker function */
static void process_send_sockets(struct work_struct *work)
{
	struct connection *con = container_of(work, struct connection, swork);
	int ret;

	WARN_ON_ONCE(test_bit(CF_IS_OTHERCON, &con->flags));

	down_read(&con->sock_lock);
	if (!con->sock) {
		up_read(&con->sock_lock);
		down_write(&con->sock_lock);
		if (!con->sock) {
			ret = dlm_connect(con);
			switch (ret) {
			case 0:
				break;
			case -EINPROGRESS:
				/* avoid spamming resched on connection
				 * we might can switch to a state_change
				 * event based mechanism if established
				 */
				msleep(100);
				break;
			default:
				/* CF_SEND_PENDING not cleared */
				up_write(&con->sock_lock);
				log_print("connect to node %d try %d error %d",
					  con->nodeid, con->retries++, ret);
				msleep(1000);
				/* For now we try forever to reconnect. In
				 * future we should send a event to cluster
				 * manager to fence itself after certain amount
				 * of retries.
				 */
				queue_work(io_workqueue, &con->swork);
				return;
			}
		}
		downgrade_write(&con->sock_lock);
	}

	do {
		ret = send_to_sock(con);
	} while (ret == DLM_IO_SUCCESS);
	up_read(&con->sock_lock);

	switch (ret) {
	case DLM_IO_END:
		/* CF_SEND_PENDING cleared */
		break;
	case DLM_IO_RESCHED:
		/* CF_SEND_PENDING not cleared */
		cond_resched();
		queue_work(io_workqueue, &con->swork);
		break;
	default:
		if (ret < 0) {
			close_connection(con, false);

			/* CF_SEND_PENDING cleared */
			spin_lock_bh(&con->writequeue_lock);
			lowcomms_queue_swork(con);
			spin_unlock_bh(&con->writequeue_lock);
			break;
		}

		WARN_ON_ONCE(1);
		break;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void work_stop(void)
{
<<<<<<< HEAD
	destroy_workqueue(recv_workqueue);
	destroy_workqueue(send_workqueue);
=======
	if (io_workqueue) {
		destroy_workqueue(io_workqueue);
		io_workqueue = NULL;
	}

	if (process_workqueue) {
		destroy_workqueue(process_workqueue);
		process_workqueue = NULL;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int work_start(void)
{
<<<<<<< HEAD
	recv_workqueue = alloc_workqueue("dlm_recv",
					 WQ_UNBOUND | WQ_MEM_RECLAIM, 1);
	if (!recv_workqueue) {
		log_print("can't start dlm_recv");
		return -ENOMEM;
	}

	send_workqueue = alloc_workqueue("dlm_send",
					 WQ_UNBOUND | WQ_MEM_RECLAIM, 1);
	if (!send_workqueue) {
		log_print("can't start dlm_send");
		destroy_workqueue(recv_workqueue);
=======
	io_workqueue = alloc_workqueue("dlm_io", WQ_HIGHPRI | WQ_MEM_RECLAIM |
				       WQ_UNBOUND, 0);
	if (!io_workqueue) {
		log_print("can't start dlm_io");
		return -ENOMEM;
	}

	/* ordered dlm message process queue,
	 * should be converted to a tasklet
	 */
	process_workqueue = alloc_ordered_workqueue("dlm_process",
						    WQ_HIGHPRI | WQ_MEM_RECLAIM);
	if (!process_workqueue) {
		log_print("can't start dlm_process");
		destroy_workqueue(io_workqueue);
		io_workqueue = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ENOMEM;
	}

	return 0;
}

<<<<<<< HEAD
static void stop_conn(struct connection *con)
{
	con->flags |= 0x0F;
	if (con->sock && con->sock->sk)
		con->sock->sk->sk_user_data = NULL;
}

static void free_conn(struct connection *con)
{
	close_connection(con, true);
	if (con->othercon)
		kmem_cache_free(con_cache, con->othercon);
	hlist_del(&con->list);
	kmem_cache_free(con_cache, con);
=======
void dlm_lowcomms_shutdown(void)
{
	struct connection *con;
	int i, idx;

	/* stop lowcomms_listen_data_ready calls */
	lock_sock(listen_con.sock->sk);
	listen_con.sock->sk->sk_data_ready = listen_sock.sk_data_ready;
	release_sock(listen_con.sock->sk);

	cancel_work_sync(&listen_con.rwork);
	dlm_close_sock(&listen_con.sock);

	idx = srcu_read_lock(&connections_srcu);
	for (i = 0; i < CONN_HASH_SIZE; i++) {
		hlist_for_each_entry_rcu(con, &connection_hash[i], list) {
			shutdown_connection(con, true);
			stop_connection_io(con);
			flush_workqueue(process_workqueue);
			close_connection(con, true);

			clean_one_writequeue(con);
			if (con->othercon)
				clean_one_writequeue(con->othercon);
			allow_connection_io(con);
		}
	}
	srcu_read_unlock(&connections_srcu, idx);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void dlm_lowcomms_stop(void)
{
<<<<<<< HEAD
	/* Set all the flags to prevent any
	   socket activity.
	*/
	mutex_lock(&connections_lock);
	foreach_conn(stop_conn);
	mutex_unlock(&connections_lock);

	work_stop();

	mutex_lock(&connections_lock);
	clean_writequeues();

	foreach_conn(free_conn);

	mutex_unlock(&connections_lock);
	kmem_cache_destroy(con_cache);
}

int dlm_lowcomms_start(void)
{
	int error = -EINVAL;
	struct connection *con;
	int i;

	for (i = 0; i < CONN_HASH_SIZE; i++)
		INIT_HLIST_HEAD(&connection_hash[i]);
=======
	work_stop();
	dlm_proto_ops = NULL;
}

static int dlm_listen_for_all(void)
{
	struct socket *sock;
	int result;

	log_print("Using %s for communications",
		  dlm_proto_ops->name);

	result = dlm_proto_ops->listen_validate();
	if (result < 0)
		return result;

	result = sock_create_kern(&init_net, dlm_local_addr[0].ss_family,
				  SOCK_STREAM, dlm_proto_ops->proto, &sock);
	if (result < 0) {
		log_print("Can't create comms socket: %d", result);
		return result;
	}

	sock_set_mark(sock->sk, dlm_config.ci_mark);
	dlm_proto_ops->listen_sockopts(sock);

	result = dlm_proto_ops->listen_bind(sock);
	if (result < 0)
		goto out;

	lock_sock(sock->sk);
	listen_sock.sk_data_ready = sock->sk->sk_data_ready;
	listen_sock.sk_write_space = sock->sk->sk_write_space;
	listen_sock.sk_error_report = sock->sk->sk_error_report;
	listen_sock.sk_state_change = sock->sk->sk_state_change;

	listen_con.sock = sock;

	sock->sk->sk_allocation = GFP_NOFS;
	sock->sk->sk_use_task_frag = false;
	sock->sk->sk_data_ready = lowcomms_listen_data_ready;
	release_sock(sock->sk);

	result = sock->ops->listen(sock, 128);
	if (result < 0) {
		dlm_close_sock(&listen_con.sock);
		return result;
	}

	return 0;

out:
	sock_release(sock);
	return result;
}

static int dlm_tcp_bind(struct socket *sock)
{
	struct sockaddr_storage src_addr;
	int result, addr_len;

	/* Bind to our cluster-known address connecting to avoid
	 * routing problems.
	 */
	memcpy(&src_addr, &dlm_local_addr[0], sizeof(src_addr));
	make_sockaddr(&src_addr, 0, &addr_len);

	result = kernel_bind(sock, (struct sockaddr *)&src_addr,
			     addr_len);
	if (result < 0) {
		/* This *may* not indicate a critical error */
		log_print("could not bind for connect: %d", result);
	}

	return 0;
}

static int dlm_tcp_connect(struct connection *con, struct socket *sock,
			   struct sockaddr *addr, int addr_len)
{
	return kernel_connect(sock, addr, addr_len, O_NONBLOCK);
}

static int dlm_tcp_listen_validate(void)
{
	/* We don't support multi-homed hosts */
	if (dlm_local_count > 1) {
		log_print("TCP protocol can't handle multi-homed hosts, try SCTP");
		return -EINVAL;
	}

	return 0;
}

static void dlm_tcp_sockopts(struct socket *sock)
{
	/* Turn off Nagle's algorithm */
	tcp_sock_set_nodelay(sock->sk);
}

static void dlm_tcp_listen_sockopts(struct socket *sock)
{
	dlm_tcp_sockopts(sock);
	sock_set_reuseaddr(sock->sk);
}

static int dlm_tcp_listen_bind(struct socket *sock)
{
	int addr_len;

	/* Bind to our port */
	make_sockaddr(&dlm_local_addr[0], dlm_config.ci_tcp_port, &addr_len);
	return kernel_bind(sock, (struct sockaddr *)&dlm_local_addr[0],
			   addr_len);
}

static const struct dlm_proto_ops dlm_tcp_ops = {
	.name = "TCP",
	.proto = IPPROTO_TCP,
	.connect = dlm_tcp_connect,
	.sockopts = dlm_tcp_sockopts,
	.bind = dlm_tcp_bind,
	.listen_validate = dlm_tcp_listen_validate,
	.listen_sockopts = dlm_tcp_listen_sockopts,
	.listen_bind = dlm_tcp_listen_bind,
};

static int dlm_sctp_bind(struct socket *sock)
{
	return sctp_bind_addrs(sock, 0);
}

static int dlm_sctp_connect(struct connection *con, struct socket *sock,
			    struct sockaddr *addr, int addr_len)
{
	int ret;

	/*
	 * Make kernel_connect() function return in specified time,
	 * since O_NONBLOCK argument in connect() function does not work here,
	 * then, we should restore the default value of this attribute.
	 */
	sock_set_sndtimeo(sock->sk, 5);
	ret = kernel_connect(sock, addr, addr_len, 0);
	sock_set_sndtimeo(sock->sk, 0);
	return ret;
}

static int dlm_sctp_listen_validate(void)
{
	if (!IS_ENABLED(CONFIG_IP_SCTP)) {
		log_print("SCTP is not enabled by this kernel");
		return -EOPNOTSUPP;
	}

	request_module("sctp");
	return 0;
}

static int dlm_sctp_bind_listen(struct socket *sock)
{
	return sctp_bind_addrs(sock, dlm_config.ci_tcp_port);
}

static void dlm_sctp_sockopts(struct socket *sock)
{
	/* Turn off Nagle's algorithm */
	sctp_sock_set_nodelay(sock->sk);
	sock_set_rcvbuf(sock->sk, NEEDED_RMEM);
}

static const struct dlm_proto_ops dlm_sctp_ops = {
	.name = "SCTP",
	.proto = IPPROTO_SCTP,
	.try_new_addr = true,
	.connect = dlm_sctp_connect,
	.sockopts = dlm_sctp_sockopts,
	.bind = dlm_sctp_bind,
	.listen_validate = dlm_sctp_listen_validate,
	.listen_sockopts = dlm_sctp_sockopts,
	.listen_bind = dlm_sctp_bind_listen,
};

int dlm_lowcomms_start(void)
{
	int error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	init_local();
	if (!dlm_local_count) {
		error = -ENOTCONN;
		log_print("no local IP address has been set");
<<<<<<< HEAD
		goto out;
	}

	error = -ENOMEM;
	con_cache = kmem_cache_create("dlm_conn", sizeof(struct connection),
				      __alignof__(struct connection), 0,
				      NULL);
	if (!con_cache)
		goto out;

	/* Start listening */
	if (dlm_config.ci_protocol == 0)
		error = tcp_listen_for_all();
	else
		error = sctp_listen_for_all();
	if (error)
		goto fail_unlisten;

	error = work_start();
	if (error)
		goto fail_unlisten;

	return 0;

fail_unlisten:
	con = nodeid2con(0,0);
	if (con) {
		close_connection(con, false);
		kmem_cache_free(con_cache, con);
	}
	kmem_cache_destroy(con_cache);

out:
	return error;
}
=======
		goto fail;
	}

	error = work_start();
	if (error)
		goto fail;

	/* Start listening */
	switch (dlm_config.ci_protocol) {
	case DLM_PROTO_TCP:
		dlm_proto_ops = &dlm_tcp_ops;
		break;
	case DLM_PROTO_SCTP:
		dlm_proto_ops = &dlm_sctp_ops;
		break;
	default:
		log_print("Invalid protocol identifier %d set",
			  dlm_config.ci_protocol);
		error = -EINVAL;
		goto fail_proto_ops;
	}

	error = dlm_listen_for_all();
	if (error)
		goto fail_listen;

	return 0;

fail_listen:
	dlm_proto_ops = NULL;
fail_proto_ops:
	work_stop();
fail:
	return error;
}

void dlm_lowcomms_init(void)
{
	int i;

	for (i = 0; i < CONN_HASH_SIZE; i++)
		INIT_HLIST_HEAD(&connection_hash[i]);

	INIT_WORK(&listen_con.rwork, process_listen_recv_socket);
}

void dlm_lowcomms_exit(void)
{
	struct connection *con;
	int i, idx;

	idx = srcu_read_lock(&connections_srcu);
	for (i = 0; i < CONN_HASH_SIZE; i++) {
		hlist_for_each_entry_rcu(con, &connection_hash[i], list) {
			spin_lock(&connections_lock);
			hlist_del_rcu(&con->list);
			spin_unlock(&connections_lock);

			if (con->othercon)
				call_srcu(&connections_srcu, &con->othercon->rcu,
					  connection_release);
			call_srcu(&connections_srcu, &con->rcu, connection_release);
		}
	}
	srcu_read_unlock(&connections_srcu, idx);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
