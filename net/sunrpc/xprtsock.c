<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * linux/net/sunrpc/xprtsock.c
 *
 * Client-side transport implementation for sockets.
 *
 * TCP callback races fixes (C) 1998 Red Hat
 * TCP send fixes (C) 1998 Red Hat
 * TCP NFS related read + write fixes
 *  (C) 1999 Dave Airlie, University of Limerick, Ireland <airlied@linux.ie>
 *
 * Rewrite of larges part of the code in order to stabilize TCP stuff.
 * Fix behaviour when socket buffer is full.
 *  (C) 1999 Trond Myklebust <trond.myklebust@fys.uio.no>
 *
 * IP socket transport implementation, (C) 2005 Chuck Lever <cel@netapp.com>
 *
 * IPv6 support contributed by Gilles Quillard, Bull Open Source, 2005.
 *   <gilles.quillard@bull.net>
 */

#include <linux/types.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/capability.h>
#include <linux/pagemap.h>
#include <linux/errno.h>
#include <linux/socket.h>
#include <linux/in.h>
#include <linux/net.h>
#include <linux/mm.h>
#include <linux/un.h>
#include <linux/udp.h>
#include <linux/tcp.h>
#include <linux/sunrpc/clnt.h>
<<<<<<< HEAD
=======
#include <linux/sunrpc/addr.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/sunrpc/sched.h>
#include <linux/sunrpc/svcsock.h>
#include <linux/sunrpc/xprtsock.h>
#include <linux/file.h>
#ifdef CONFIG_SUNRPC_BACKCHANNEL
#include <linux/sunrpc/bc_xprt.h>
#endif

#include <net/sock.h>
#include <net/checksum.h>
#include <net/udp.h>
#include <net/tcp.h>
<<<<<<< HEAD

#include "sunrpc.h"

static void xs_close(struct rpc_xprt *xprt);
=======
#include <net/tls_prot.h>
#include <net/handshake.h>

#include <linux/bvec.h>
#include <linux/highmem.h>
#include <linux/uio.h>
#include <linux/sched/mm.h>

#include <trace/events/sock.h>
#include <trace/events/sunrpc.h>

#include "socklib.h"
#include "sunrpc.h"

static void xs_close(struct rpc_xprt *xprt);
static void xs_reset_srcport(struct sock_xprt *transport);
static void xs_set_srcport(struct sock_xprt *transport, struct socket *sock);
static void xs_tcp_set_socket_timeouts(struct rpc_xprt *xprt,
		struct socket *sock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * xprtsock tunables
 */
static unsigned int xprt_udp_slot_table_entries = RPC_DEF_SLOT_TABLE;
static unsigned int xprt_tcp_slot_table_entries = RPC_MIN_SLOT_TABLE;
static unsigned int xprt_max_tcp_slot_table_entries = RPC_MAX_SLOT_TABLE;

static unsigned int xprt_min_resvport = RPC_DEF_MIN_RESVPORT;
static unsigned int xprt_max_resvport = RPC_DEF_MAX_RESVPORT;

#define XS_TCP_LINGER_TO	(15U * HZ)
static unsigned int xs_tcp_fin_timeout __read_mostly = XS_TCP_LINGER_TO;

/*
 * We can register our own files under /proc/sys/sunrpc by
<<<<<<< HEAD
 * calling register_sysctl_table() again.  The files in that
=======
 * calling register_sysctl() again.  The files in that
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * directory become the union of all files registered there.
 *
 * We simply need to make sure that we don't collide with
 * someone else's file names!
 */

<<<<<<< HEAD
#ifdef RPC_DEBUG

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static unsigned int min_slot_table_size = RPC_MIN_SLOT_TABLE;
static unsigned int max_slot_table_size = RPC_MAX_SLOT_TABLE;
static unsigned int max_tcp_slot_table_limit = RPC_MAX_SLOT_TABLE_LIMIT;
static unsigned int xprt_min_resvport_limit = RPC_MIN_RESVPORT;
static unsigned int xprt_max_resvport_limit = RPC_MAX_RESVPORT;

static struct ctl_table_header *sunrpc_table_header;

<<<<<<< HEAD
=======
static struct xprt_class xs_local_transport;
static struct xprt_class xs_udp_transport;
static struct xprt_class xs_tcp_transport;
static struct xprt_class xs_tcp_tls_transport;
static struct xprt_class xs_bc_tcp_transport;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * FIXME: changing the UDP slot table size should also resize the UDP
 *        socket buffers for existing UDP transports
 */
<<<<<<< HEAD
static ctl_table xs_tunables_table[] = {
=======
static struct ctl_table xs_tunables_table[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{
		.procname	= "udp_slot_table_entries",
		.data		= &xprt_udp_slot_table_entries,
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &min_slot_table_size,
		.extra2		= &max_slot_table_size
	},
	{
		.procname	= "tcp_slot_table_entries",
		.data		= &xprt_tcp_slot_table_entries,
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &min_slot_table_size,
		.extra2		= &max_slot_table_size
	},
	{
		.procname	= "tcp_max_slot_table_entries",
		.data		= &xprt_max_tcp_slot_table_entries,
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &min_slot_table_size,
		.extra2		= &max_tcp_slot_table_limit
	},
	{
		.procname	= "min_resvport",
		.data		= &xprt_min_resvport,
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &xprt_min_resvport_limit,
		.extra2		= &xprt_max_resvport_limit
	},
	{
		.procname	= "max_resvport",
		.data		= &xprt_max_resvport,
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &xprt_min_resvport_limit,
		.extra2		= &xprt_max_resvport_limit
	},
	{
		.procname	= "tcp_fin_timeout",
		.data		= &xs_tcp_fin_timeout,
		.maxlen		= sizeof(xs_tcp_fin_timeout),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	{ },
};

<<<<<<< HEAD
static ctl_table sunrpc_table[] = {
	{
		.procname	= "sunrpc",
		.mode		= 0555,
		.child		= xs_tunables_table
	},
	{ },
};

#endif

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Wait duration for a reply from the RPC portmapper.
 */
#define XS_BIND_TO		(60U * HZ)

/*
 * Delay if a UDP socket connect error occurs.  This is most likely some
 * kind of resource problem on the local host.
 */
#define XS_UDP_REEST_TO		(2U * HZ)

/*
 * The reestablish timeout allows clients to delay for a bit before attempting
 * to reconnect to a server that just dropped our connection.
 *
 * We implement an exponential backoff when trying to reestablish a TCP
 * transport connection with the server.  Some servers like to drop a TCP
 * connection when they are overworked, so we start with a short timeout and
 * increase over time if the server is down or not responding.
 */
#define XS_TCP_INIT_REEST_TO	(3U * HZ)
<<<<<<< HEAD
#define XS_TCP_MAX_REEST_TO	(5U * 60 * HZ)
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * TCP idle timeout; client drops the transport socket if it is idle
 * for this long.  Note that we also timeout UDP sockets to prevent
 * holding port numbers when there is no RPC traffic.
 */
#define XS_IDLE_DISC_TO		(5U * 60 * HZ)

<<<<<<< HEAD
#ifdef RPC_DEBUG
=======
/*
 * TLS handshake timeout.
 */
#define XS_TLS_HANDSHAKE_TO	(10U * HZ)

#if IS_ENABLED(CONFIG_SUNRPC_DEBUG)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
# undef  RPC_DEBUG_DATA
# define RPCDBG_FACILITY	RPCDBG_TRANS
#endif

#ifdef RPC_DEBUG_DATA
static void xs_pktdump(char *msg, u32 *packet, unsigned int count)
{
	u8 *buf = (u8 *) packet;
	int j;

	dprintk("RPC:       %s\n", msg);
	for (j = 0; j < count && j < 128; j += 4) {
		if (!(j & 31)) {
			if (j)
				dprintk("\n");
			dprintk("0x%04x ", j);
		}
		dprintk("%02x%02x%02x%02x ",
			buf[j], buf[j+1], buf[j+2], buf[j+3]);
	}
	dprintk("\n");
}
#else
static inline void xs_pktdump(char *msg, u32 *packet, unsigned int count)
{
	/* NOP */
}
#endif

<<<<<<< HEAD
struct sock_xprt {
	struct rpc_xprt		xprt;

	/*
	 * Network layer
	 */
	struct socket *		sock;
	struct sock *		inet;

	/*
	 * State of TCP reply receive
	 */
	__be32			tcp_fraghdr,
				tcp_xid,
				tcp_calldir;

	u32			tcp_offset,
				tcp_reclen;

	unsigned long		tcp_copied,
				tcp_flags;

	/*
	 * Connection of transports
	 */
	struct delayed_work	connect_worker;
	struct sockaddr_storage	srcaddr;
	unsigned short		srcport;

	/*
	 * UDP socket buffer size parameters
	 */
	size_t			rcvsize,
				sndsize;

	/*
	 * Saved socket callback addresses
	 */
	void			(*old_data_ready)(struct sock *, int);
	void			(*old_state_change)(struct sock *);
	void			(*old_write_space)(struct sock *);
};

/*
 * TCP receive state flags
 */
#define TCP_RCV_LAST_FRAG	(1UL << 0)
#define TCP_RCV_COPY_FRAGHDR	(1UL << 1)
#define TCP_RCV_COPY_XID	(1UL << 2)
#define TCP_RCV_COPY_DATA	(1UL << 3)
#define TCP_RCV_READ_CALLDIR	(1UL << 4)
#define TCP_RCV_COPY_CALLDIR	(1UL << 5)

/*
 * TCP RPC flags
 */
#define TCP_RPC_REPLY		(1UL << 6)
=======
static inline struct rpc_xprt *xprt_from_sock(struct sock *sk)
{
	return (struct rpc_xprt *) sk->sk_user_data;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline struct sockaddr *xs_addr(struct rpc_xprt *xprt)
{
	return (struct sockaddr *) &xprt->addr;
}

static inline struct sockaddr_un *xs_addr_un(struct rpc_xprt *xprt)
{
	return (struct sockaddr_un *) &xprt->addr;
}

static inline struct sockaddr_in *xs_addr_in(struct rpc_xprt *xprt)
{
	return (struct sockaddr_in *) &xprt->addr;
}

static inline struct sockaddr_in6 *xs_addr_in6(struct rpc_xprt *xprt)
{
	return (struct sockaddr_in6 *) &xprt->addr;
}

static void xs_format_common_peer_addresses(struct rpc_xprt *xprt)
{
	struct sockaddr *sap = xs_addr(xprt);
	struct sockaddr_in6 *sin6;
	struct sockaddr_in *sin;
	struct sockaddr_un *sun;
	char buf[128];

	switch (sap->sa_family) {
	case AF_LOCAL:
		sun = xs_addr_un(xprt);
<<<<<<< HEAD
		strlcpy(buf, sun->sun_path, sizeof(buf));
=======
		if (sun->sun_path[0]) {
			strscpy(buf, sun->sun_path, sizeof(buf));
		} else {
			buf[0] = '@';
			strscpy(buf+1, sun->sun_path+1, sizeof(buf)-1);
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		xprt->address_strings[RPC_DISPLAY_ADDR] =
						kstrdup(buf, GFP_KERNEL);
		break;
	case AF_INET:
		(void)rpc_ntop(sap, buf, sizeof(buf));
		xprt->address_strings[RPC_DISPLAY_ADDR] =
						kstrdup(buf, GFP_KERNEL);
		sin = xs_addr_in(xprt);
		snprintf(buf, sizeof(buf), "%08x", ntohl(sin->sin_addr.s_addr));
		break;
	case AF_INET6:
		(void)rpc_ntop(sap, buf, sizeof(buf));
		xprt->address_strings[RPC_DISPLAY_ADDR] =
						kstrdup(buf, GFP_KERNEL);
		sin6 = xs_addr_in6(xprt);
		snprintf(buf, sizeof(buf), "%pi6", &sin6->sin6_addr);
		break;
	default:
		BUG();
	}

	xprt->address_strings[RPC_DISPLAY_HEX_ADDR] = kstrdup(buf, GFP_KERNEL);
}

static void xs_format_common_peer_ports(struct rpc_xprt *xprt)
{
	struct sockaddr *sap = xs_addr(xprt);
	char buf[128];

	snprintf(buf, sizeof(buf), "%u", rpc_get_port(sap));
	xprt->address_strings[RPC_DISPLAY_PORT] = kstrdup(buf, GFP_KERNEL);

	snprintf(buf, sizeof(buf), "%4hx", rpc_get_port(sap));
	xprt->address_strings[RPC_DISPLAY_HEX_PORT] = kstrdup(buf, GFP_KERNEL);
}

static void xs_format_peer_addresses(struct rpc_xprt *xprt,
				     const char *protocol,
				     const char *netid)
{
	xprt->address_strings[RPC_DISPLAY_PROTO] = protocol;
	xprt->address_strings[RPC_DISPLAY_NETID] = netid;
	xs_format_common_peer_addresses(xprt);
	xs_format_common_peer_ports(xprt);
}

static void xs_update_peer_port(struct rpc_xprt *xprt)
{
	kfree(xprt->address_strings[RPC_DISPLAY_HEX_PORT]);
	kfree(xprt->address_strings[RPC_DISPLAY_PORT]);

	xs_format_common_peer_ports(xprt);
}

static void xs_free_peer_addresses(struct rpc_xprt *xprt)
{
	unsigned int i;

	for (i = 0; i < RPC_DISPLAY_MAX; i++)
		switch (i) {
		case RPC_DISPLAY_PROTO:
		case RPC_DISPLAY_NETID:
			continue;
		default:
			kfree(xprt->address_strings[i]);
		}
}

<<<<<<< HEAD
#define XS_SENDMSG_FLAGS	(MSG_DONTWAIT | MSG_NOSIGNAL)

static int xs_send_kvec(struct socket *sock, struct sockaddr *addr, int addrlen, struct kvec *vec, unsigned int base, int more)
{
	struct msghdr msg = {
		.msg_name	= addr,
		.msg_namelen	= addrlen,
		.msg_flags	= XS_SENDMSG_FLAGS | (more ? MSG_MORE : 0),
	};
	struct kvec iov = {
		.iov_base	= vec->iov_base + base,
		.iov_len	= vec->iov_len - base,
	};

	if (iov.iov_len != 0)
		return kernel_sendmsg(sock, &msg, &iov, 1, iov.iov_len);
	return kernel_sendmsg(sock, &msg, NULL, 0, 0);
}

static int xs_send_pagedata(struct socket *sock, struct xdr_buf *xdr, unsigned int base, int more, bool zerocopy)
{
	ssize_t (*do_sendpage)(struct socket *sock, struct page *page,
			int offset, size_t size, int flags);
	struct page **ppage;
	unsigned int remainder;
	int err, sent = 0;

	remainder = xdr->page_len - base;
	base += xdr->page_base;
	ppage = xdr->pages + (base >> PAGE_SHIFT);
	base &= ~PAGE_MASK;
	do_sendpage = sock->ops->sendpage;
	if (!zerocopy)
		do_sendpage = sock_no_sendpage;
	for(;;) {
		unsigned int len = min_t(unsigned int, PAGE_SIZE - base, remainder);
		int flags = XS_SENDMSG_FLAGS;

		remainder -= len;
		if (remainder != 0 || more)
			flags |= MSG_MORE;
		err = do_sendpage(sock, *ppage, base, len, flags);
		if (remainder == 0 || err != len)
			break;
		sent += err;
		ppage++;
		base = 0;
	}
	if (sent == 0)
		return err;
	if (err > 0)
		sent += err;
	return sent;
}

/**
 * xs_sendpages - write pages directly to a socket
 * @sock: socket to send on
 * @addr: UDP only -- address of destination
 * @addrlen: UDP only -- length of destination address
 * @xdr: buffer containing this request
 * @base: starting position in the buffer
 * @zerocopy: true if it is safe to use sendpage()
 *
 */
static int xs_sendpages(struct socket *sock, struct sockaddr *addr, int addrlen, struct xdr_buf *xdr, unsigned int base, bool zerocopy)
{
	unsigned int remainder = xdr->len - base;
	int err, sent = 0;

	if (unlikely(!sock))
		return -ENOTSOCK;

	clear_bit(SOCK_ASYNC_NOSPACE, &sock->flags);
	if (base != 0) {
		addr = NULL;
		addrlen = 0;
	}

	if (base < xdr->head[0].iov_len || addr != NULL) {
		unsigned int len = xdr->head[0].iov_len - base;
		remainder -= len;
		err = xs_send_kvec(sock, addr, addrlen, &xdr->head[0], base, remainder != 0);
		if (remainder == 0 || err != len)
			goto out;
		sent += err;
		base = 0;
	} else
		base -= xdr->head[0].iov_len;

	if (base < xdr->page_len) {
		unsigned int len = xdr->page_len - base;
		remainder -= len;
		err = xs_send_pagedata(sock, xdr, base, remainder != 0, zerocopy);
		if (remainder == 0 || err != len)
			goto out;
		sent += err;
		base = 0;
	} else
		base -= xdr->page_len;

	if (base >= xdr->tail[0].iov_len)
		return sent;
	err = xs_send_kvec(sock, NULL, 0, &xdr->tail[0], base, 0);
out:
	if (sent == 0)
		return err;
	if (err > 0)
		sent += err;
	return sent;
}

static void xs_nospace_callback(struct rpc_task *task)
{
	struct sock_xprt *transport = container_of(task->tk_rqstp->rq_xprt, struct sock_xprt, xprt);

	transport->inet->sk_write_pending--;
	clear_bit(SOCK_ASYNC_NOSPACE, &transport->sock->flags);
}

/**
 * xs_nospace - place task on wait queue if transmit was incomplete
 * @task: task to put to sleep
 *
 */
static int xs_nospace(struct rpc_task *task)
{
	struct rpc_rqst *req = task->tk_rqstp;
	struct rpc_xprt *xprt = req->rq_xprt;
	struct sock_xprt *transport = container_of(xprt, struct sock_xprt, xprt);
	struct sock *sk = transport->inet;
	int ret = -EAGAIN;

	dprintk("RPC: %5u xmit incomplete (%u left of %u)\n",
			task->tk_pid, req->rq_slen - req->rq_bytes_sent,
			req->rq_slen);

	/* Protect against races with write_space */
	spin_lock_bh(&xprt->transport_lock);

	/* Don't race with disconnect */
	if (xprt_connected(xprt)) {
		if (test_bit(SOCK_ASYNC_NOSPACE, &transport->sock->flags)) {
			/*
			 * Notify TCP that we're limited by the application
			 * window size
			 */
			set_bit(SOCK_NOSPACE, &transport->sock->flags);
			sk->sk_write_pending++;
			/* ...and wait for more buffer space */
			xprt_wait_for_buffer_space(task, xs_nospace_callback);
		}
	} else {
		clear_bit(SOCK_ASYNC_NOSPACE, &transport->sock->flags);
		ret = -ENOTCONN;
	}

	spin_unlock_bh(&xprt->transport_lock);

	/* Race breaker in case memory is freed before above code is called */
	sk->sk_write_space(sk);
	return ret;
}

/*
 * Construct a stream transport record marker in @buf.
 */
static inline void xs_encode_stream_record_marker(struct xdr_buf *buf)
{
	u32 reclen = buf->len - sizeof(rpc_fraghdr);
	rpc_fraghdr *base = buf->head[0].iov_base;
	*base = cpu_to_be32(RPC_LAST_STREAM_FRAGMENT | reclen);
=======
static size_t
xs_alloc_sparse_pages(struct xdr_buf *buf, size_t want, gfp_t gfp)
{
	size_t i,n;

	if (!want || !(buf->flags & XDRBUF_SPARSE_PAGES))
		return want;
	n = (buf->page_base + want + PAGE_SIZE - 1) >> PAGE_SHIFT;
	for (i = 0; i < n; i++) {
		if (buf->pages[i])
			continue;
		buf->bvec[i].bv_page = buf->pages[i] = alloc_page(gfp);
		if (!buf->pages[i]) {
			i *= PAGE_SIZE;
			return i > buf->page_base ? i - buf->page_base : 0;
		}
	}
	return want;
}

static int
xs_sock_process_cmsg(struct socket *sock, struct msghdr *msg,
		     struct cmsghdr *cmsg, int ret)
{
	u8 content_type = tls_get_record_type(sock->sk, cmsg);
	u8 level, description;

	switch (content_type) {
	case 0:
		break;
	case TLS_RECORD_TYPE_DATA:
		/* TLS sets EOR at the end of each application data
		 * record, even though there might be more frames
		 * waiting to be decrypted.
		 */
		msg->msg_flags &= ~MSG_EOR;
		break;
	case TLS_RECORD_TYPE_ALERT:
		tls_alert_recv(sock->sk, msg, &level, &description);
		ret = (level == TLS_ALERT_LEVEL_FATAL) ?
			-EACCES : -EAGAIN;
		break;
	default:
		/* discard this record type */
		ret = -EAGAIN;
	}
	return ret;
}

static int
xs_sock_recv_cmsg(struct socket *sock, struct msghdr *msg, int flags)
{
	union {
		struct cmsghdr	cmsg;
		u8		buf[CMSG_SPACE(sizeof(u8))];
	} u;
	int ret;

	msg->msg_control = &u;
	msg->msg_controllen = sizeof(u);
	ret = sock_recvmsg(sock, msg, flags);
	if (msg->msg_controllen != sizeof(u))
		ret = xs_sock_process_cmsg(sock, msg, &u.cmsg, ret);
	return ret;
}

static ssize_t
xs_sock_recvmsg(struct socket *sock, struct msghdr *msg, int flags, size_t seek)
{
	ssize_t ret;
	if (seek != 0)
		iov_iter_advance(&msg->msg_iter, seek);
	ret = xs_sock_recv_cmsg(sock, msg, flags);
	return ret > 0 ? ret + seek : ret;
}

static ssize_t
xs_read_kvec(struct socket *sock, struct msghdr *msg, int flags,
		struct kvec *kvec, size_t count, size_t seek)
{
	iov_iter_kvec(&msg->msg_iter, ITER_DEST, kvec, 1, count);
	return xs_sock_recvmsg(sock, msg, flags, seek);
}

static ssize_t
xs_read_bvec(struct socket *sock, struct msghdr *msg, int flags,
		struct bio_vec *bvec, unsigned long nr, size_t count,
		size_t seek)
{
	iov_iter_bvec(&msg->msg_iter, ITER_DEST, bvec, nr, count);
	return xs_sock_recvmsg(sock, msg, flags, seek);
}

static ssize_t
xs_read_discard(struct socket *sock, struct msghdr *msg, int flags,
		size_t count)
{
	iov_iter_discard(&msg->msg_iter, ITER_DEST, count);
	return xs_sock_recv_cmsg(sock, msg, flags);
}

#if ARCH_IMPLEMENTS_FLUSH_DCACHE_PAGE
static void
xs_flush_bvec(const struct bio_vec *bvec, size_t count, size_t seek)
{
	struct bvec_iter bi = {
		.bi_size = count,
	};
	struct bio_vec bv;

	bvec_iter_advance(bvec, &bi, seek & PAGE_MASK);
	for_each_bvec(bv, bvec, bi, bi)
		flush_dcache_page(bv.bv_page);
}
#else
static inline void
xs_flush_bvec(const struct bio_vec *bvec, size_t count, size_t seek)
{
}
#endif

static ssize_t
xs_read_xdr_buf(struct socket *sock, struct msghdr *msg, int flags,
		struct xdr_buf *buf, size_t count, size_t seek, size_t *read)
{
	size_t want, seek_init = seek, offset = 0;
	ssize_t ret;

	want = min_t(size_t, count, buf->head[0].iov_len);
	if (seek < want) {
		ret = xs_read_kvec(sock, msg, flags, &buf->head[0], want, seek);
		if (ret <= 0)
			goto sock_err;
		offset += ret;
		if (offset == count || msg->msg_flags & (MSG_EOR|MSG_TRUNC))
			goto out;
		if (ret != want)
			goto out;
		seek = 0;
	} else {
		seek -= want;
		offset += want;
	}

	want = xs_alloc_sparse_pages(
		buf, min_t(size_t, count - offset, buf->page_len),
		GFP_KERNEL | __GFP_NORETRY | __GFP_NOWARN);
	if (seek < want) {
		ret = xs_read_bvec(sock, msg, flags, buf->bvec,
				xdr_buf_pagecount(buf),
				want + buf->page_base,
				seek + buf->page_base);
		if (ret <= 0)
			goto sock_err;
		xs_flush_bvec(buf->bvec, ret, seek + buf->page_base);
		ret -= buf->page_base;
		offset += ret;
		if (offset == count || msg->msg_flags & (MSG_EOR|MSG_TRUNC))
			goto out;
		if (ret != want)
			goto out;
		seek = 0;
	} else {
		seek -= want;
		offset += want;
	}

	want = min_t(size_t, count - offset, buf->tail[0].iov_len);
	if (seek < want) {
		ret = xs_read_kvec(sock, msg, flags, &buf->tail[0], want, seek);
		if (ret <= 0)
			goto sock_err;
		offset += ret;
		if (offset == count || msg->msg_flags & (MSG_EOR|MSG_TRUNC))
			goto out;
		if (ret != want)
			goto out;
	} else if (offset < seek_init)
		offset = seek_init;
	ret = -EMSGSIZE;
out:
	*read = offset - seek_init;
	return ret;
sock_err:
	offset += seek;
	goto out;
}

static void
xs_read_header(struct sock_xprt *transport, struct xdr_buf *buf)
{
	if (!transport->recv.copied) {
		if (buf->head[0].iov_len >= transport->recv.offset)
			memcpy(buf->head[0].iov_base,
					&transport->recv.xid,
					transport->recv.offset);
		transport->recv.copied = transport->recv.offset;
	}
}

static bool
xs_read_stream_request_done(struct sock_xprt *transport)
{
	return transport->recv.fraghdr & cpu_to_be32(RPC_LAST_STREAM_FRAGMENT);
}

static void
xs_read_stream_check_eor(struct sock_xprt *transport,
		struct msghdr *msg)
{
	if (xs_read_stream_request_done(transport))
		msg->msg_flags |= MSG_EOR;
}

static ssize_t
xs_read_stream_request(struct sock_xprt *transport, struct msghdr *msg,
		int flags, struct rpc_rqst *req)
{
	struct xdr_buf *buf = &req->rq_private_buf;
	size_t want, read;
	ssize_t ret;

	xs_read_header(transport, buf);

	want = transport->recv.len - transport->recv.offset;
	if (want != 0) {
		ret = xs_read_xdr_buf(transport->sock, msg, flags, buf,
				transport->recv.copied + want,
				transport->recv.copied,
				&read);
		transport->recv.offset += read;
		transport->recv.copied += read;
	}

	if (transport->recv.offset == transport->recv.len)
		xs_read_stream_check_eor(transport, msg);

	if (want == 0)
		return 0;

	switch (ret) {
	default:
		break;
	case -EFAULT:
	case -EMSGSIZE:
		msg->msg_flags |= MSG_TRUNC;
		return read;
	case 0:
		return -ESHUTDOWN;
	}
	return ret < 0 ? ret : read;
}

static size_t
xs_read_stream_headersize(bool isfrag)
{
	if (isfrag)
		return sizeof(__be32);
	return 3 * sizeof(__be32);
}

static ssize_t
xs_read_stream_header(struct sock_xprt *transport, struct msghdr *msg,
		int flags, size_t want, size_t seek)
{
	struct kvec kvec = {
		.iov_base = &transport->recv.fraghdr,
		.iov_len = want,
	};
	return xs_read_kvec(transport->sock, msg, flags, &kvec, want, seek);
}

#if defined(CONFIG_SUNRPC_BACKCHANNEL)
static ssize_t
xs_read_stream_call(struct sock_xprt *transport, struct msghdr *msg, int flags)
{
	struct rpc_xprt *xprt = &transport->xprt;
	struct rpc_rqst *req;
	ssize_t ret;

	/* Is this transport associated with the backchannel? */
	if (!xprt->bc_serv)
		return -ESHUTDOWN;

	/* Look up and lock the request corresponding to the given XID */
	req = xprt_lookup_bc_request(xprt, transport->recv.xid);
	if (!req) {
		printk(KERN_WARNING "Callback slot table overflowed\n");
		return -ESHUTDOWN;
	}
	if (transport->recv.copied && !req->rq_private_buf.len)
		return -ESHUTDOWN;

	ret = xs_read_stream_request(transport, msg, flags, req);
	if (msg->msg_flags & (MSG_EOR|MSG_TRUNC))
		xprt_complete_bc_request(req, transport->recv.copied);
	else
		req->rq_private_buf.len = transport->recv.copied;

	return ret;
}
#else /* CONFIG_SUNRPC_BACKCHANNEL */
static ssize_t
xs_read_stream_call(struct sock_xprt *transport, struct msghdr *msg, int flags)
{
	return -ESHUTDOWN;
}
#endif /* CONFIG_SUNRPC_BACKCHANNEL */

static ssize_t
xs_read_stream_reply(struct sock_xprt *transport, struct msghdr *msg, int flags)
{
	struct rpc_xprt *xprt = &transport->xprt;
	struct rpc_rqst *req;
	ssize_t ret = 0;

	/* Look up and lock the request corresponding to the given XID */
	spin_lock(&xprt->queue_lock);
	req = xprt_lookup_rqst(xprt, transport->recv.xid);
	if (!req || (transport->recv.copied && !req->rq_private_buf.len)) {
		msg->msg_flags |= MSG_TRUNC;
		goto out;
	}
	xprt_pin_rqst(req);
	spin_unlock(&xprt->queue_lock);

	ret = xs_read_stream_request(transport, msg, flags, req);

	spin_lock(&xprt->queue_lock);
	if (msg->msg_flags & (MSG_EOR|MSG_TRUNC))
		xprt_complete_rqst(req->rq_task, transport->recv.copied);
	else
		req->rq_private_buf.len = transport->recv.copied;
	xprt_unpin_rqst(req);
out:
	spin_unlock(&xprt->queue_lock);
	return ret;
}

static ssize_t
xs_read_stream(struct sock_xprt *transport, int flags)
{
	struct msghdr msg = { 0 };
	size_t want, read = 0;
	ssize_t ret = 0;

	if (transport->recv.len == 0) {
		want = xs_read_stream_headersize(transport->recv.copied != 0);
		ret = xs_read_stream_header(transport, &msg, flags, want,
				transport->recv.offset);
		if (ret <= 0)
			goto out_err;
		transport->recv.offset = ret;
		if (transport->recv.offset != want)
			return transport->recv.offset;
		transport->recv.len = be32_to_cpu(transport->recv.fraghdr) &
			RPC_FRAGMENT_SIZE_MASK;
		transport->recv.offset -= sizeof(transport->recv.fraghdr);
		read = ret;
	}

	switch (be32_to_cpu(transport->recv.calldir)) {
	default:
		msg.msg_flags |= MSG_TRUNC;
		break;
	case RPC_CALL:
		ret = xs_read_stream_call(transport, &msg, flags);
		break;
	case RPC_REPLY:
		ret = xs_read_stream_reply(transport, &msg, flags);
	}
	if (msg.msg_flags & MSG_TRUNC) {
		transport->recv.calldir = cpu_to_be32(-1);
		transport->recv.copied = -1;
	}
	if (ret < 0)
		goto out_err;
	read += ret;
	if (transport->recv.offset < transport->recv.len) {
		if (!(msg.msg_flags & MSG_TRUNC))
			return read;
		msg.msg_flags = 0;
		ret = xs_read_discard(transport->sock, &msg, flags,
				transport->recv.len - transport->recv.offset);
		if (ret <= 0)
			goto out_err;
		transport->recv.offset += ret;
		read += ret;
		if (transport->recv.offset != transport->recv.len)
			return read;
	}
	if (xs_read_stream_request_done(transport)) {
		trace_xs_stream_read_request(transport);
		transport->recv.copied = 0;
	}
	transport->recv.offset = 0;
	transport->recv.len = 0;
	return read;
out_err:
	return ret != 0 ? ret : -ESHUTDOWN;
}

static __poll_t xs_poll_socket(struct sock_xprt *transport)
{
	return transport->sock->ops->poll(transport->file, transport->sock,
			NULL);
}

static bool xs_poll_socket_readable(struct sock_xprt *transport)
{
	__poll_t events = xs_poll_socket(transport);

	return (events & (EPOLLIN | EPOLLRDNORM)) && !(events & EPOLLRDHUP);
}

static void xs_poll_check_readable(struct sock_xprt *transport)
{

	clear_bit(XPRT_SOCK_DATA_READY, &transport->sock_state);
	if (test_bit(XPRT_SOCK_IGNORE_RECV, &transport->sock_state))
		return;
	if (!xs_poll_socket_readable(transport))
		return;
	if (!test_and_set_bit(XPRT_SOCK_DATA_READY, &transport->sock_state))
		queue_work(xprtiod_workqueue, &transport->recv_worker);
}

static void xs_stream_data_receive(struct sock_xprt *transport)
{
	size_t read = 0;
	ssize_t ret = 0;

	mutex_lock(&transport->recv_mutex);
	if (transport->sock == NULL)
		goto out;
	for (;;) {
		ret = xs_read_stream(transport, MSG_DONTWAIT);
		if (ret < 0)
			break;
		read += ret;
		cond_resched();
	}
	if (ret == -ESHUTDOWN)
		kernel_sock_shutdown(transport->sock, SHUT_RDWR);
	else if (ret == -EACCES)
		xprt_wake_pending_tasks(&transport->xprt, -EACCES);
	else
		xs_poll_check_readable(transport);
out:
	mutex_unlock(&transport->recv_mutex);
	trace_xs_stream_read_data(&transport->xprt, ret, read);
}

static void xs_stream_data_receive_workfn(struct work_struct *work)
{
	struct sock_xprt *transport =
		container_of(work, struct sock_xprt, recv_worker);
	unsigned int pflags = memalloc_nofs_save();

	xs_stream_data_receive(transport);
	memalloc_nofs_restore(pflags);
}

static void
xs_stream_reset_connect(struct sock_xprt *transport)
{
	transport->recv.offset = 0;
	transport->recv.len = 0;
	transport->recv.copied = 0;
	transport->xmit.offset = 0;
}

static void
xs_stream_start_connect(struct sock_xprt *transport)
{
	transport->xprt.stat.connect_count++;
	transport->xprt.stat.connect_start = jiffies;
}

#define XS_SENDMSG_FLAGS	(MSG_DONTWAIT | MSG_NOSIGNAL)

/**
 * xs_nospace - handle transmit was incomplete
 * @req: pointer to RPC request
 * @transport: pointer to struct sock_xprt
 *
 */
static int xs_nospace(struct rpc_rqst *req, struct sock_xprt *transport)
{
	struct rpc_xprt *xprt = &transport->xprt;
	struct sock *sk = transport->inet;
	int ret = -EAGAIN;

	trace_rpc_socket_nospace(req, transport);

	/* Protect against races with write_space */
	spin_lock(&xprt->transport_lock);

	/* Don't race with disconnect */
	if (xprt_connected(xprt)) {
		/* wait for more buffer space */
		set_bit(XPRT_SOCK_NOSPACE, &transport->sock_state);
		set_bit(SOCK_NOSPACE, &sk->sk_socket->flags);
		sk->sk_write_pending++;
		xprt_wait_for_buffer_space(xprt);
	} else
		ret = -ENOTCONN;

	spin_unlock(&xprt->transport_lock);
	return ret;
}

static int xs_sock_nospace(struct rpc_rqst *req)
{
	struct sock_xprt *transport =
		container_of(req->rq_xprt, struct sock_xprt, xprt);
	struct sock *sk = transport->inet;
	int ret = -EAGAIN;

	lock_sock(sk);
	if (!sock_writeable(sk))
		ret = xs_nospace(req, transport);
	release_sock(sk);
	return ret;
}

static int xs_stream_nospace(struct rpc_rqst *req, bool vm_wait)
{
	struct sock_xprt *transport =
		container_of(req->rq_xprt, struct sock_xprt, xprt);
	struct sock *sk = transport->inet;
	int ret = -EAGAIN;

	if (vm_wait)
		return -ENOBUFS;
	lock_sock(sk);
	if (!sk_stream_memory_free(sk))
		ret = xs_nospace(req, transport);
	release_sock(sk);
	return ret;
}

static int xs_stream_prepare_request(struct rpc_rqst *req, struct xdr_buf *buf)
{
	return xdr_alloc_bvec(buf, rpc_task_gfp_mask());
}

static void xs_stream_abort_send_request(struct rpc_rqst *req)
{
	struct rpc_xprt *xprt = req->rq_xprt;
	struct sock_xprt *transport =
		container_of(xprt, struct sock_xprt, xprt);

	if (transport->xmit.offset != 0 &&
	    !test_bit(XPRT_CLOSE_WAIT, &xprt->state))
		xprt_force_disconnect(xprt);
}

/*
 * Determine if the previous message in the stream was aborted before it
 * could complete transmission.
 */
static bool
xs_send_request_was_aborted(struct sock_xprt *transport, struct rpc_rqst *req)
{
	return transport->xmit.offset != 0 && req->rq_bytes_sent == 0;
}

/*
 * Return the stream record marker field for a record of length < 2^31-1
 */
static rpc_fraghdr
xs_stream_record_marker(struct xdr_buf *xdr)
{
	if (!xdr->len)
		return 0;
	return cpu_to_be32(RPC_LAST_STREAM_FRAGMENT | (u32)xdr->len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * xs_local_send_request - write an RPC request to an AF_LOCAL socket
<<<<<<< HEAD
 * @task: RPC task that manages the state of an RPC request
 *
 * Return values:
 *        0:	The request has been sent
 *   EAGAIN:	The socket was blocked, please call again later to
 *		complete the request
 * ENOTCONN:	Caller needs to invoke connect logic then call again
 *    other:	Some other error occured, the request was not sent
 */
static int xs_local_send_request(struct rpc_task *task)
{
	struct rpc_rqst *req = task->tk_rqstp;
	struct rpc_xprt *xprt = req->rq_xprt;
	struct sock_xprt *transport =
				container_of(xprt, struct sock_xprt, xprt);
	struct xdr_buf *xdr = &req->rq_snd_buf;
	int status;

	xs_encode_stream_record_marker(&req->rq_snd_buf);

	xs_pktdump("packet data:",
			req->rq_svec->iov_base, req->rq_svec->iov_len);

	status = xs_sendpages(transport->sock, NULL, 0,
						xdr, req->rq_bytes_sent, true);
	dprintk("RPC:       %s(%u) = %d\n",
			__func__, xdr->len - req->rq_bytes_sent, status);
	if (likely(status >= 0)) {
		req->rq_bytes_sent += status;
		req->rq_xmit_bytes_sent += status;
		if (likely(req->rq_bytes_sent >= req->rq_slen)) {
			req->rq_bytes_sent = 0;
			return 0;
		}
		status = -EAGAIN;
	}

	switch (status) {
	case -EAGAIN:
		status = xs_nospace(task);
		break;
	default:
		dprintk("RPC:       sendmsg returned unrecognized error %d\n",
			-status);
	case -EPIPE:
		xs_close(xprt);
		status = -ENOTCONN;
	}

	return status;
}

/**
 * xs_udp_send_request - write an RPC request to a UDP socket
 * @task: address of RPC task that manages the state of an RPC request
=======
 * @req: pointer to RPC request
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Return values:
 *        0:	The request has been sent
 *   EAGAIN:	The socket was blocked, please call again later to
 *		complete the request
 * ENOTCONN:	Caller needs to invoke connect logic then call again
 *    other:	Some other error occurred, the request was not sent
 */
<<<<<<< HEAD
static int xs_udp_send_request(struct rpc_task *task)
{
	struct rpc_rqst *req = task->tk_rqstp;
	struct rpc_xprt *xprt = req->rq_xprt;
	struct sock_xprt *transport = container_of(xprt, struct sock_xprt, xprt);
	struct xdr_buf *xdr = &req->rq_snd_buf;
=======
static int xs_local_send_request(struct rpc_rqst *req)
{
	struct rpc_xprt *xprt = req->rq_xprt;
	struct sock_xprt *transport =
				container_of(xprt, struct sock_xprt, xprt);
	struct xdr_buf *xdr = &req->rq_snd_buf;
	rpc_fraghdr rm = xs_stream_record_marker(xdr);
	unsigned int msglen = rm ? req->rq_slen + sizeof(rm) : req->rq_slen;
	struct msghdr msg = {
		.msg_flags	= XS_SENDMSG_FLAGS,
	};
	bool vm_wait;
	unsigned int sent;
	int status;

	/* Close the stream if the previous transmission was incomplete */
	if (xs_send_request_was_aborted(transport, req)) {
		xprt_force_disconnect(xprt);
		return -ENOTCONN;
	}

	xs_pktdump("packet data:",
			req->rq_svec->iov_base, req->rq_svec->iov_len);

	vm_wait = sk_stream_is_writeable(transport->inet) ? true : false;

	req->rq_xtime = ktime_get();
	status = xprt_sock_sendmsg(transport->sock, &msg, xdr,
				   transport->xmit.offset, rm, &sent);
	dprintk("RPC:       %s(%u) = %d\n",
			__func__, xdr->len - transport->xmit.offset, status);

	if (likely(sent > 0) || status == 0) {
		transport->xmit.offset += sent;
		req->rq_bytes_sent = transport->xmit.offset;
		if (likely(req->rq_bytes_sent >= msglen)) {
			req->rq_xmit_bytes_sent += transport->xmit.offset;
			transport->xmit.offset = 0;
			return 0;
		}
		status = -EAGAIN;
		vm_wait = false;
	}

	switch (status) {
	case -EAGAIN:
		status = xs_stream_nospace(req, vm_wait);
		break;
	default:
		dprintk("RPC:       sendmsg returned unrecognized error %d\n",
			-status);
		fallthrough;
	case -EPIPE:
		xprt_force_disconnect(xprt);
		status = -ENOTCONN;
	}

	return status;
}

/**
 * xs_udp_send_request - write an RPC request to a UDP socket
 * @req: pointer to RPC request
 *
 * Return values:
 *        0:	The request has been sent
 *   EAGAIN:	The socket was blocked, please call again later to
 *		complete the request
 * ENOTCONN:	Caller needs to invoke connect logic then call again
 *    other:	Some other error occurred, the request was not sent
 */
static int xs_udp_send_request(struct rpc_rqst *req)
{
	struct rpc_xprt *xprt = req->rq_xprt;
	struct sock_xprt *transport = container_of(xprt, struct sock_xprt, xprt);
	struct xdr_buf *xdr = &req->rq_snd_buf;
	struct msghdr msg = {
		.msg_name	= xs_addr(xprt),
		.msg_namelen	= xprt->addrlen,
		.msg_flags	= XS_SENDMSG_FLAGS,
	};
	unsigned int sent;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int status;

	xs_pktdump("packet data:",
				req->rq_svec->iov_base,
				req->rq_svec->iov_len);

	if (!xprt_bound(xprt))
		return -ENOTCONN;
<<<<<<< HEAD
	status = xs_sendpages(transport->sock,
			      xs_addr(xprt),
			      xprt->addrlen, xdr,
			      req->rq_bytes_sent, true);

	dprintk("RPC:       xs_udp_send_request(%u) = %d\n",
			xdr->len - req->rq_bytes_sent, status);

	if (status >= 0) {
		req->rq_xmit_bytes_sent += status;
		if (status >= req->rq_slen)
=======

	if (!xprt_request_get_cong(xprt, req))
		return -EBADSLT;

	status = xdr_alloc_bvec(xdr, rpc_task_gfp_mask());
	if (status < 0)
		return status;
	req->rq_xtime = ktime_get();
	status = xprt_sock_sendmsg(transport->sock, &msg, xdr, 0, 0, &sent);

	dprintk("RPC:       xs_udp_send_request(%u) = %d\n",
			xdr->len, status);

	/* firewall is blocking us, don't return -EAGAIN or we end up looping */
	if (status == -EPERM)
		goto process_status;

	if (status == -EAGAIN && sock_writeable(transport->inet))
		status = -ENOBUFS;

	if (sent > 0 || status == 0) {
		req->rq_xmit_bytes_sent += sent;
		if (sent >= req->rq_slen)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return 0;
		/* Still some bytes left; set up for a retry later. */
		status = -EAGAIN;
	}

<<<<<<< HEAD
=======
process_status:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	switch (status) {
	case -ENOTSOCK:
		status = -ENOTCONN;
		/* Should we call xs_close() here? */
		break;
	case -EAGAIN:
<<<<<<< HEAD
		status = xs_nospace(task);
=======
		status = xs_sock_nospace(req);
		break;
	case -ENETUNREACH:
	case -ENOBUFS:
	case -EPIPE:
	case -ECONNREFUSED:
	case -EPERM:
		/* When the server has died, an ICMP port unreachable message
		 * prompts ECONNREFUSED. */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	default:
		dprintk("RPC:       sendmsg returned unrecognized error %d\n",
			-status);
<<<<<<< HEAD
	case -ENETUNREACH:
	case -EPIPE:
	case -ECONNREFUSED:
		/* When the server has died, an ICMP port unreachable message
		 * prompts ECONNREFUSED. */
		clear_bit(SOCK_ASYNC_NOSPACE, &transport->sock->flags);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return status;
}

/**
<<<<<<< HEAD
 * xs_tcp_shutdown - gracefully shut down a TCP socket
 * @xprt: transport
 *
 * Initiates a graceful shutdown of the TCP socket by calling the
 * equivalent of shutdown(SHUT_WR);
 */
static void xs_tcp_shutdown(struct rpc_xprt *xprt)
{
	struct sock_xprt *transport = container_of(xprt, struct sock_xprt, xprt);
	struct socket *sock = transport->sock;

	if (sock != NULL)
		kernel_sock_shutdown(sock, SHUT_WR);
}

/**
 * xs_tcp_send_request - write an RPC request to a TCP socket
 * @task: address of RPC task that manages the state of an RPC request
=======
 * xs_tcp_send_request - write an RPC request to a TCP socket
 * @req: pointer to RPC request
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Return values:
 *        0:	The request has been sent
 *   EAGAIN:	The socket was blocked, please call again later to
 *		complete the request
 * ENOTCONN:	Caller needs to invoke connect logic then call again
 *    other:	Some other error occurred, the request was not sent
 *
 * XXX: In the case of soft timeouts, should we eventually give up
 *	if sendmsg is not able to make progress?
 */
<<<<<<< HEAD
static int xs_tcp_send_request(struct rpc_task *task)
{
	struct rpc_rqst *req = task->tk_rqstp;
	struct rpc_xprt *xprt = req->rq_xprt;
	struct sock_xprt *transport = container_of(xprt, struct sock_xprt, xprt);
	struct xdr_buf *xdr = &req->rq_snd_buf;
	bool zerocopy = true;
	int status;

	xs_encode_stream_record_marker(&req->rq_snd_buf);
=======
static int xs_tcp_send_request(struct rpc_rqst *req)
{
	struct rpc_xprt *xprt = req->rq_xprt;
	struct sock_xprt *transport = container_of(xprt, struct sock_xprt, xprt);
	struct xdr_buf *xdr = &req->rq_snd_buf;
	rpc_fraghdr rm = xs_stream_record_marker(xdr);
	unsigned int msglen = rm ? req->rq_slen + sizeof(rm) : req->rq_slen;
	struct msghdr msg = {
		.msg_flags	= XS_SENDMSG_FLAGS,
	};
	bool vm_wait;
	unsigned int sent;
	int status;

	/* Close the stream if the previous transmission was incomplete */
	if (xs_send_request_was_aborted(transport, req)) {
		if (transport->sock != NULL)
			kernel_sock_shutdown(transport->sock, SHUT_RDWR);
		return -ENOTCONN;
	}
	if (!transport->inet)
		return -ENOTCONN;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	xs_pktdump("packet data:",
				req->rq_svec->iov_base,
				req->rq_svec->iov_len);
<<<<<<< HEAD
	/* Don't use zero copy if this is a resend. If the RPC call
	 * completes while the socket holds a reference to the pages,
	 * then we may end up resending corrupted data.
	 */
	if (task->tk_flags & RPC_TASK_SENT)
		zerocopy = false;
=======

	if (test_bit(XPRT_SOCK_UPD_TIMEOUT, &transport->sock_state))
		xs_tcp_set_socket_timeouts(xprt, transport->sock);

	xs_set_srcport(transport, transport->sock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Continue transmitting the packet/record. We must be careful
	 * to cope with writespace callbacks arriving _after_ we have
	 * called sendmsg(). */
<<<<<<< HEAD
	while (1) {
		status = xs_sendpages(transport->sock,
					NULL, 0, xdr, req->rq_bytes_sent,
					zerocopy);

		dprintk("RPC:       xs_tcp_send_request(%u) = %d\n",
				xdr->len - req->rq_bytes_sent, status);

		if (unlikely(status < 0))
			break;

		/* If we've sent the entire packet, immediately
		 * reset the count of bytes sent. */
		req->rq_bytes_sent += status;
		req->rq_xmit_bytes_sent += status;
		if (likely(req->rq_bytes_sent >= req->rq_slen)) {
			req->rq_bytes_sent = 0;
			return 0;
		}

		if (status != 0)
			continue;
		status = -EAGAIN;
		break;
	}
=======
	req->rq_xtime = ktime_get();
	tcp_sock_set_cork(transport->inet, true);

	vm_wait = sk_stream_is_writeable(transport->inet) ? true : false;

	do {
		status = xprt_sock_sendmsg(transport->sock, &msg, xdr,
					   transport->xmit.offset, rm, &sent);

		dprintk("RPC:       xs_tcp_send_request(%u) = %d\n",
				xdr->len - transport->xmit.offset, status);

		/* If we've sent the entire packet, immediately
		 * reset the count of bytes sent. */
		transport->xmit.offset += sent;
		req->rq_bytes_sent = transport->xmit.offset;
		if (likely(req->rq_bytes_sent >= msglen)) {
			req->rq_xmit_bytes_sent += transport->xmit.offset;
			transport->xmit.offset = 0;
			if (atomic_long_read(&xprt->xmit_queuelen) == 1)
				tcp_sock_set_cork(transport->inet, false);
			return 0;
		}

		WARN_ON_ONCE(sent == 0 && status == 0);

		if (sent > 0)
			vm_wait = false;

	} while (status == 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (status) {
	case -ENOTSOCK:
		status = -ENOTCONN;
		/* Should we call xs_close() here? */
		break;
	case -EAGAIN:
<<<<<<< HEAD
		status = xs_nospace(task);
=======
		status = xs_stream_nospace(req, vm_wait);
		break;
	case -ECONNRESET:
	case -ECONNREFUSED:
	case -ENOTCONN:
	case -EADDRINUSE:
	case -ENOBUFS:
	case -EPIPE:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	default:
		dprintk("RPC:       sendmsg returned unrecognized error %d\n",
			-status);
<<<<<<< HEAD
	case -ECONNRESET:
		xs_tcp_shutdown(xprt);
	case -ECONNREFUSED:
	case -ENOTCONN:
	case -EPIPE:
		clear_bit(SOCK_ASYNC_NOSPACE, &transport->sock->flags);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return status;
}

<<<<<<< HEAD
/**
 * xs_tcp_release_xprt - clean up after a tcp transmission
 * @xprt: transport
 * @task: rpc task
 *
 * This cleans up if an error causes us to abort the transmission of a request.
 * In this case, the socket may need to be reset in order to avoid confusing
 * the server.
 */
static void xs_tcp_release_xprt(struct rpc_xprt *xprt, struct rpc_task *task)
{
	struct rpc_rqst *req;

	if (task != xprt->snd_task)
		return;
	if (task == NULL)
		goto out_release;
	req = task->tk_rqstp;
	if (req == NULL)
		goto out_release;
	if (req->rq_bytes_sent == 0)
		goto out_release;
	if (req->rq_bytes_sent == req->rq_snd_buf.len)
		goto out_release;
	set_bit(XPRT_CLOSE_WAIT, &task->tk_xprt->state);
out_release:
	xprt_release_xprt(xprt, task);
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void xs_save_old_callbacks(struct sock_xprt *transport, struct sock *sk)
{
	transport->old_data_ready = sk->sk_data_ready;
	transport->old_state_change = sk->sk_state_change;
	transport->old_write_space = sk->sk_write_space;
<<<<<<< HEAD
=======
	transport->old_error_report = sk->sk_error_report;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void xs_restore_old_callbacks(struct sock_xprt *transport, struct sock *sk)
{
	sk->sk_data_ready = transport->old_data_ready;
	sk->sk_state_change = transport->old_state_change;
	sk->sk_write_space = transport->old_write_space;
<<<<<<< HEAD
=======
	sk->sk_error_report = transport->old_error_report;
}

static void xs_sock_reset_state_flags(struct rpc_xprt *xprt)
{
	struct sock_xprt *transport = container_of(xprt, struct sock_xprt, xprt);

	transport->xprt_err = 0;
	clear_bit(XPRT_SOCK_DATA_READY, &transport->sock_state);
	clear_bit(XPRT_SOCK_WAKE_ERROR, &transport->sock_state);
	clear_bit(XPRT_SOCK_WAKE_WRITE, &transport->sock_state);
	clear_bit(XPRT_SOCK_WAKE_DISCONNECT, &transport->sock_state);
	clear_bit(XPRT_SOCK_NOSPACE, &transport->sock_state);
}

static void xs_run_error_worker(struct sock_xprt *transport, unsigned int nr)
{
	set_bit(nr, &transport->sock_state);
	queue_work(xprtiod_workqueue, &transport->error_worker);
}

static void xs_sock_reset_connection_flags(struct rpc_xprt *xprt)
{
	xprt->connect_cookie++;
	smp_mb__before_atomic();
	clear_bit(XPRT_CLOSE_WAIT, &xprt->state);
	clear_bit(XPRT_CLOSING, &xprt->state);
	xs_sock_reset_state_flags(xprt);
	smp_mb__after_atomic();
}

/**
 * xs_error_report - callback to handle TCP socket state errors
 * @sk: socket
 *
 * Note: we don't call sock_error() since there may be a rpc_task
 * using the socket, and so we don't want to clear sk->sk_err.
 */
static void xs_error_report(struct sock *sk)
{
	struct sock_xprt *transport;
	struct rpc_xprt *xprt;

	if (!(xprt = xprt_from_sock(sk)))
		return;

	transport = container_of(xprt, struct sock_xprt, xprt);
	transport->xprt_err = -sk->sk_err;
	if (transport->xprt_err == 0)
		return;
	dprintk("RPC:       xs_error_report client %p, error=%d...\n",
			xprt, -transport->xprt_err);
	trace_rpc_socket_error(xprt, sk->sk_socket, transport->xprt_err);

	/* barrier ensures xprt_err is set before XPRT_SOCK_WAKE_ERROR */
	smp_mb__before_atomic();
	xs_run_error_worker(transport, XPRT_SOCK_WAKE_ERROR);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void xs_reset_transport(struct sock_xprt *transport)
{
	struct socket *sock = transport->sock;
	struct sock *sk = transport->inet;
	struct rpc_xprt *xprt = &transport->xprt;
<<<<<<< HEAD

	if (sk == NULL)
		return;

	transport->srcport = 0;

	write_lock_bh(&sk->sk_callback_lock);
	transport->inet = NULL;
	transport->sock = NULL;
=======
	struct file *filp = transport->file;

	if (sk == NULL)
		return;
	/*
	 * Make sure we're calling this in a context from which it is safe
	 * to call __fput_sync(). In practice that means rpciod and the
	 * system workqueue.
	 */
	if (!(current->flags & PF_WQ_WORKER)) {
		WARN_ON_ONCE(1);
		set_bit(XPRT_CLOSE_WAIT, &xprt->state);
		return;
	}

	if (atomic_read(&transport->xprt.swapper))
		sk_clear_memalloc(sk);

	tls_handshake_cancel(sk);

	kernel_sock_shutdown(sock, SHUT_RDWR);

	mutex_lock(&transport->recv_mutex);
	lock_sock(sk);
	transport->inet = NULL;
	transport->sock = NULL;
	transport->file = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	sk->sk_user_data = NULL;

	xs_restore_old_callbacks(transport, sk);
	xprt_clear_connected(xprt);
<<<<<<< HEAD
	write_unlock_bh(&sk->sk_callback_lock);

	sk->sk_no_check = 0;

	sock_release(sock);
=======
	xs_sock_reset_connection_flags(xprt);
	/* Reset stream record info */
	xs_stream_reset_connect(transport);
	release_sock(sk);
	mutex_unlock(&transport->recv_mutex);

	trace_rpc_socket_close(xprt, sock);
	__fput_sync(filp);

	xprt_disconnect_done(xprt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * xs_close - close a socket
 * @xprt: transport
 *
 * This is used when all requests are complete; ie, no DRC state remains
 * on the server we want to save.
 *
 * The caller _must_ be holding XPRT_LOCKED in order to avoid issues with
 * xs_reset_transport() zeroing the socket from underneath a writer.
 */
static void xs_close(struct rpc_xprt *xprt)
{
	struct sock_xprt *transport = container_of(xprt, struct sock_xprt, xprt);

	dprintk("RPC:       xs_close xprt %p\n", xprt);

<<<<<<< HEAD
	xs_reset_transport(transport);
	xprt->reestablish_timeout = 0;

	smp_mb__before_clear_bit();
	clear_bit(XPRT_CONNECTION_ABORT, &xprt->state);
	clear_bit(XPRT_CLOSE_WAIT, &xprt->state);
	clear_bit(XPRT_CLOSING, &xprt->state);
	smp_mb__after_clear_bit();
	xprt_disconnect_done(xprt);
}

static void xs_tcp_close(struct rpc_xprt *xprt)
{
	if (test_and_clear_bit(XPRT_CONNECTION_CLOSE, &xprt->state))
		xs_close(xprt);
	else
		xs_tcp_shutdown(xprt);
=======
	if (transport->sock)
		tls_handshake_close(transport->sock);
	xs_reset_transport(transport);
	xprt->reestablish_timeout = 0;
}

static void xs_inject_disconnect(struct rpc_xprt *xprt)
{
	dprintk("RPC:       injecting transport disconnect on xprt=%p\n",
		xprt);
	xprt_disconnect_done(xprt);
}

static void xs_xprt_free(struct rpc_xprt *xprt)
{
	xs_free_peer_addresses(xprt);
	xprt_free(xprt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * xs_destroy - prepare to shutdown a transport
 * @xprt: doomed transport
 *
 */
static void xs_destroy(struct rpc_xprt *xprt)
{
<<<<<<< HEAD
	struct sock_xprt *transport = container_of(xprt, struct sock_xprt, xprt);

	dprintk("RPC:       xs_destroy xprt %p\n", xprt);

	cancel_delayed_work_sync(&transport->connect_worker);

	xs_close(xprt);
	xs_free_peer_addresses(xprt);
	xprt_free(xprt);
	module_put(THIS_MODULE);
}

static inline struct rpc_xprt *xprt_from_sock(struct sock *sk)
{
	return (struct rpc_xprt *) sk->sk_user_data;
}

static int xs_local_copy_to_xdr(struct xdr_buf *xdr, struct sk_buff *skb)
{
	struct xdr_skb_reader desc = {
		.skb		= skb,
		.offset		= sizeof(rpc_fraghdr),
		.count		= skb->len - sizeof(rpc_fraghdr),
	};

	if (xdr_partial_copy_from_skb(xdr, 0, &desc, xdr_skb_read_bits) < 0)
		return -1;
	if (desc.count)
		return -1;
	return 0;
}

/**
 * xs_local_data_ready - "data ready" callback for AF_LOCAL sockets
 * @sk: socket with data to read
 * @len: how much data to read
 *
 * Currently this assumes we can read the whole reply in a single gulp.
 */
static void xs_local_data_ready(struct sock *sk, int len)
{
	struct rpc_task *task;
	struct rpc_xprt *xprt;
	struct rpc_rqst *rovr;
	struct sk_buff *skb;
	int err, repsize, copied;
	u32 _xid;
	__be32 *xp;

	read_lock_bh(&sk->sk_callback_lock);
	dprintk("RPC:       %s...\n", __func__);
	xprt = xprt_from_sock(sk);
	if (xprt == NULL)
		goto out;

	skb = skb_recv_datagram(sk, 0, 1, &err);
	if (skb == NULL)
		goto out;

	if (xprt->shutdown)
		goto dropit;

	repsize = skb->len - sizeof(rpc_fraghdr);
	if (repsize < 4) {
		dprintk("RPC:       impossible RPC reply size %d\n", repsize);
		goto dropit;
	}

	/* Copy the XID from the skb... */
	xp = skb_header_pointer(skb, sizeof(rpc_fraghdr), sizeof(_xid), &_xid);
	if (xp == NULL)
		goto dropit;

	/* Look up and lock the request corresponding to the given XID */
	spin_lock(&xprt->transport_lock);
	rovr = xprt_lookup_rqst(xprt, *xp);
	if (!rovr)
		goto out_unlock;
	task = rovr->rq_task;

	copied = rovr->rq_private_buf.buflen;
	if (copied > repsize)
		copied = repsize;

	if (xs_local_copy_to_xdr(&rovr->rq_private_buf, skb)) {
		dprintk("RPC:       sk_buff copy failed\n");
		goto out_unlock;
	}

	xprt_complete_rqst(task, copied);

 out_unlock:
	spin_unlock(&xprt->transport_lock);
 dropit:
	skb_free_datagram(sk, skb);
 out:
	read_unlock_bh(&sk->sk_callback_lock);
}

/**
 * xs_udp_data_ready - "data ready" callback for UDP sockets
 * @sk: socket with data to read
 * @len: how much data to read
 *
 */
static void xs_udp_data_ready(struct sock *sk, int len)
{
	struct rpc_task *task;
	struct rpc_xprt *xprt;
	struct rpc_rqst *rovr;
	struct sk_buff *skb;
	int err, repsize, copied;
	u32 _xid;
	__be32 *xp;

	read_lock_bh(&sk->sk_callback_lock);
	dprintk("RPC:       xs_udp_data_ready...\n");
	if (!(xprt = xprt_from_sock(sk)))
		goto out;

	if ((skb = skb_recv_datagram(sk, 0, 1, &err)) == NULL)
		goto out;

	if (xprt->shutdown)
		goto dropit;

	repsize = skb->len - sizeof(struct udphdr);
	if (repsize < 4) {
		dprintk("RPC:       impossible RPC reply size %d!\n", repsize);
		goto dropit;
	}

	/* Copy the XID from the skb... */
	xp = skb_header_pointer(skb, sizeof(struct udphdr),
				sizeof(_xid), &_xid);
	if (xp == NULL)
		goto dropit;

	/* Look up and lock the request corresponding to the given XID */
	spin_lock(&xprt->transport_lock);
	rovr = xprt_lookup_rqst(xprt, *xp);
	if (!rovr)
		goto out_unlock;
=======
	struct sock_xprt *transport = container_of(xprt,
			struct sock_xprt, xprt);
	dprintk("RPC:       xs_destroy xprt %p\n", xprt);

	cancel_delayed_work_sync(&transport->connect_worker);
	xs_close(xprt);
	cancel_work_sync(&transport->recv_worker);
	cancel_work_sync(&transport->error_worker);
	xs_xprt_free(xprt);
	module_put(THIS_MODULE);
}

/**
 * xs_udp_data_read_skb - receive callback for UDP sockets
 * @xprt: transport
 * @sk: socket
 * @skb: skbuff
 *
 */
static void xs_udp_data_read_skb(struct rpc_xprt *xprt,
		struct sock *sk,
		struct sk_buff *skb)
{
	struct rpc_task *task;
	struct rpc_rqst *rovr;
	int repsize, copied;
	u32 _xid;
	__be32 *xp;

	repsize = skb->len;
	if (repsize < 4) {
		dprintk("RPC:       impossible RPC reply size %d!\n", repsize);
		return;
	}

	/* Copy the XID from the skb... */
	xp = skb_header_pointer(skb, 0, sizeof(_xid), &_xid);
	if (xp == NULL)
		return;

	/* Look up and lock the request corresponding to the given XID */
	spin_lock(&xprt->queue_lock);
	rovr = xprt_lookup_rqst(xprt, *xp);
	if (!rovr)
		goto out_unlock;
	xprt_pin_rqst(rovr);
	xprt_update_rtt(rovr->rq_task);
	spin_unlock(&xprt->queue_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	task = rovr->rq_task;

	if ((copied = rovr->rq_private_buf.buflen) > repsize)
		copied = repsize;

	/* Suck it into the iovec, verify checksum if not done by hw. */
	if (csum_partial_copy_to_xdr(&rovr->rq_private_buf, skb)) {
<<<<<<< HEAD
		UDPX_INC_STATS_BH(sk, UDP_MIB_INERRORS);
		goto out_unlock;
	}

	UDPX_INC_STATS_BH(sk, UDP_MIB_INDATAGRAMS);

	/* Something worked... */
	dst_confirm(skb_dst(skb));

	xprt_adjust_cwnd(task, copied);
	xprt_complete_rqst(task, copied);

 out_unlock:
	spin_unlock(&xprt->transport_lock);
 dropit:
	skb_free_datagram(sk, skb);
 out:
	read_unlock_bh(&sk->sk_callback_lock);
=======
		spin_lock(&xprt->queue_lock);
		__UDPX_INC_STATS(sk, UDP_MIB_INERRORS);
		goto out_unpin;
	}


	spin_lock(&xprt->transport_lock);
	xprt_adjust_cwnd(xprt, task, copied);
	spin_unlock(&xprt->transport_lock);
	spin_lock(&xprt->queue_lock);
	xprt_complete_rqst(task, copied);
	__UDPX_INC_STATS(sk, UDP_MIB_INDATAGRAMS);
out_unpin:
	xprt_unpin_rqst(rovr);
 out_unlock:
	spin_unlock(&xprt->queue_lock);
}

static void xs_udp_data_receive(struct sock_xprt *transport)
{
	struct sk_buff *skb;
	struct sock *sk;
	int err;

	mutex_lock(&transport->recv_mutex);
	sk = transport->inet;
	if (sk == NULL)
		goto out;
	for (;;) {
		skb = skb_recv_udp(sk, MSG_DONTWAIT, &err);
		if (skb == NULL)
			break;
		xs_udp_data_read_skb(&transport->xprt, sk, skb);
		consume_skb(skb);
		cond_resched();
	}
	xs_poll_check_readable(transport);
out:
	mutex_unlock(&transport->recv_mutex);
}

static void xs_udp_data_receive_workfn(struct work_struct *work)
{
	struct sock_xprt *transport =
		container_of(work, struct sock_xprt, recv_worker);
	unsigned int pflags = memalloc_nofs_save();

	xs_udp_data_receive(transport);
	memalloc_nofs_restore(pflags);
}

/**
 * xs_data_ready - "data ready" callback for sockets
 * @sk: socket with data to read
 *
 */
static void xs_data_ready(struct sock *sk)
{
	struct rpc_xprt *xprt;

	trace_sk_data_ready(sk);

	xprt = xprt_from_sock(sk);
	if (xprt != NULL) {
		struct sock_xprt *transport = container_of(xprt,
				struct sock_xprt, xprt);

		trace_xs_data_ready(xprt);

		transport->old_data_ready(sk);

		if (test_bit(XPRT_SOCK_IGNORE_RECV, &transport->sock_state))
			return;

		/* Any data means we had a useful conversation, so
		 * then we don't need to delay the next reconnect
		 */
		if (xprt->reestablish_timeout)
			xprt->reestablish_timeout = 0;
		if (!test_and_set_bit(XPRT_SOCK_DATA_READY, &transport->sock_state))
			queue_work(xprtiod_workqueue, &transport->recv_worker);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Helper function to force a TCP close if the server is sending
 * junk and/or it has put us in CLOSE_WAIT
 */
static void xs_tcp_force_close(struct rpc_xprt *xprt)
{
<<<<<<< HEAD
	set_bit(XPRT_CONNECTION_CLOSE, &xprt->state);
	xprt_force_disconnect(xprt);
}

static inline void xs_tcp_read_fraghdr(struct rpc_xprt *xprt, struct xdr_skb_reader *desc)
{
	struct sock_xprt *transport = container_of(xprt, struct sock_xprt, xprt);
	size_t len, used;
	char *p;

	p = ((char *) &transport->tcp_fraghdr) + transport->tcp_offset;
	len = sizeof(transport->tcp_fraghdr) - transport->tcp_offset;
	used = xdr_skb_read_bits(desc, p, len);
	transport->tcp_offset += used;
	if (used != len)
		return;

	transport->tcp_reclen = ntohl(transport->tcp_fraghdr);
	if (transport->tcp_reclen & RPC_LAST_STREAM_FRAGMENT)
		transport->tcp_flags |= TCP_RCV_LAST_FRAG;
	else
		transport->tcp_flags &= ~TCP_RCV_LAST_FRAG;
	transport->tcp_reclen &= RPC_FRAGMENT_SIZE_MASK;

	transport->tcp_flags &= ~TCP_RCV_COPY_FRAGHDR;
	transport->tcp_offset = 0;

	/* Sanity check of the record length */
	if (unlikely(transport->tcp_reclen < 8)) {
		dprintk("RPC:       invalid TCP record fragment length\n");
		xs_tcp_force_close(xprt);
		return;
	}
	dprintk("RPC:       reading TCP record fragment of length %d\n",
			transport->tcp_reclen);
}

static void xs_tcp_check_fraghdr(struct sock_xprt *transport)
{
	if (transport->tcp_offset == transport->tcp_reclen) {
		transport->tcp_flags |= TCP_RCV_COPY_FRAGHDR;
		transport->tcp_offset = 0;
		if (transport->tcp_flags & TCP_RCV_LAST_FRAG) {
			transport->tcp_flags &= ~TCP_RCV_COPY_DATA;
			transport->tcp_flags |= TCP_RCV_COPY_XID;
			transport->tcp_copied = 0;
		}
	}
}

static inline void xs_tcp_read_xid(struct sock_xprt *transport, struct xdr_skb_reader *desc)
{
	size_t len, used;
	char *p;

	len = sizeof(transport->tcp_xid) - transport->tcp_offset;
	dprintk("RPC:       reading XID (%Zu bytes)\n", len);
	p = ((char *) &transport->tcp_xid) + transport->tcp_offset;
	used = xdr_skb_read_bits(desc, p, len);
	transport->tcp_offset += used;
	if (used != len)
		return;
	transport->tcp_flags &= ~TCP_RCV_COPY_XID;
	transport->tcp_flags |= TCP_RCV_READ_CALLDIR;
	transport->tcp_copied = 4;
	dprintk("RPC:       reading %s XID %08x\n",
			(transport->tcp_flags & TCP_RPC_REPLY) ? "reply for"
							      : "request with",
			ntohl(transport->tcp_xid));
	xs_tcp_check_fraghdr(transport);
}

static inline void xs_tcp_read_calldir(struct sock_xprt *transport,
				       struct xdr_skb_reader *desc)
{
	size_t len, used;
	u32 offset;
	char *p;

	/*
	 * We want transport->tcp_offset to be 8 at the end of this routine
	 * (4 bytes for the xid and 4 bytes for the call/reply flag).
	 * When this function is called for the first time,
	 * transport->tcp_offset is 4 (after having already read the xid).
	 */
	offset = transport->tcp_offset - sizeof(transport->tcp_xid);
	len = sizeof(transport->tcp_calldir) - offset;
	dprintk("RPC:       reading CALL/REPLY flag (%Zu bytes)\n", len);
	p = ((char *) &transport->tcp_calldir) + offset;
	used = xdr_skb_read_bits(desc, p, len);
	transport->tcp_offset += used;
	if (used != len)
		return;
	transport->tcp_flags &= ~TCP_RCV_READ_CALLDIR;
	/*
	 * We don't yet have the XDR buffer, so we will write the calldir
	 * out after we get the buffer from the 'struct rpc_rqst'
	 */
	switch (ntohl(transport->tcp_calldir)) {
	case RPC_REPLY:
		transport->tcp_flags |= TCP_RCV_COPY_CALLDIR;
		transport->tcp_flags |= TCP_RCV_COPY_DATA;
		transport->tcp_flags |= TCP_RPC_REPLY;
		break;
	case RPC_CALL:
		transport->tcp_flags |= TCP_RCV_COPY_CALLDIR;
		transport->tcp_flags |= TCP_RCV_COPY_DATA;
		transport->tcp_flags &= ~TCP_RPC_REPLY;
		break;
	default:
		dprintk("RPC:       invalid request message type\n");
		xs_tcp_force_close(&transport->xprt);
	}
	xs_tcp_check_fraghdr(transport);
}

static inline void xs_tcp_read_common(struct rpc_xprt *xprt,
				     struct xdr_skb_reader *desc,
				     struct rpc_rqst *req)
{
	struct sock_xprt *transport =
				container_of(xprt, struct sock_xprt, xprt);
	struct xdr_buf *rcvbuf;
	size_t len;
	ssize_t r;

	rcvbuf = &req->rq_private_buf;

	if (transport->tcp_flags & TCP_RCV_COPY_CALLDIR) {
		/*
		 * Save the RPC direction in the XDR buffer
		 */
		memcpy(rcvbuf->head[0].iov_base + transport->tcp_copied,
			&transport->tcp_calldir,
			sizeof(transport->tcp_calldir));
		transport->tcp_copied += sizeof(transport->tcp_calldir);
		transport->tcp_flags &= ~TCP_RCV_COPY_CALLDIR;
	}

	len = desc->count;
	if (len > transport->tcp_reclen - transport->tcp_offset) {
		struct xdr_skb_reader my_desc;

		len = transport->tcp_reclen - transport->tcp_offset;
		memcpy(&my_desc, desc, sizeof(my_desc));
		my_desc.count = len;
		r = xdr_partial_copy_from_skb(rcvbuf, transport->tcp_copied,
					  &my_desc, xdr_skb_read_bits);
		desc->count -= r;
		desc->offset += r;
	} else
		r = xdr_partial_copy_from_skb(rcvbuf, transport->tcp_copied,
					  desc, xdr_skb_read_bits);

	if (r > 0) {
		transport->tcp_copied += r;
		transport->tcp_offset += r;
	}
	if (r != len) {
		/* Error when copying to the receive buffer,
		 * usually because we weren't able to allocate
		 * additional buffer pages. All we can do now
		 * is turn off TCP_RCV_COPY_DATA, so the request
		 * will not receive any additional updates,
		 * and time out.
		 * Any remaining data from this record will
		 * be discarded.
		 */
		transport->tcp_flags &= ~TCP_RCV_COPY_DATA;
		dprintk("RPC:       XID %08x truncated request\n",
				ntohl(transport->tcp_xid));
		dprintk("RPC:       xprt = %p, tcp_copied = %lu, "
				"tcp_offset = %u, tcp_reclen = %u\n",
				xprt, transport->tcp_copied,
				transport->tcp_offset, transport->tcp_reclen);
		return;
	}

	dprintk("RPC:       XID %08x read %Zd bytes\n",
			ntohl(transport->tcp_xid), r);
	dprintk("RPC:       xprt = %p, tcp_copied = %lu, tcp_offset = %u, "
			"tcp_reclen = %u\n", xprt, transport->tcp_copied,
			transport->tcp_offset, transport->tcp_reclen);

	if (transport->tcp_copied == req->rq_private_buf.buflen)
		transport->tcp_flags &= ~TCP_RCV_COPY_DATA;
	else if (transport->tcp_offset == transport->tcp_reclen) {
		if (transport->tcp_flags & TCP_RCV_LAST_FRAG)
			transport->tcp_flags &= ~TCP_RCV_COPY_DATA;
	}
}

/*
 * Finds the request corresponding to the RPC xid and invokes the common
 * tcp read code to read the data.
 */
static inline int xs_tcp_read_reply(struct rpc_xprt *xprt,
				    struct xdr_skb_reader *desc)
{
	struct sock_xprt *transport =
				container_of(xprt, struct sock_xprt, xprt);
	struct rpc_rqst *req;

	dprintk("RPC:       read reply XID %08x\n", ntohl(transport->tcp_xid));

	/* Find and lock the request corresponding to this xid */
	spin_lock(&xprt->transport_lock);
	req = xprt_lookup_rqst(xprt, transport->tcp_xid);
	if (!req) {
		dprintk("RPC:       XID %08x request not found!\n",
				ntohl(transport->tcp_xid));
		spin_unlock(&xprt->transport_lock);
		return -1;
	}

	xs_tcp_read_common(xprt, desc, req);

	if (!(transport->tcp_flags & TCP_RCV_COPY_DATA))
		xprt_complete_rqst(req->rq_task, transport->tcp_copied);

	spin_unlock(&xprt->transport_lock);
	return 0;
}

#if defined(CONFIG_SUNRPC_BACKCHANNEL)
/*
 * Obtains an rpc_rqst previously allocated and invokes the common
 * tcp read code to read the data.  The result is placed in the callback
 * queue.
 * If we're unable to obtain the rpc_rqst we schedule the closing of the
 * connection and return -1.
 */
static inline int xs_tcp_read_callback(struct rpc_xprt *xprt,
				       struct xdr_skb_reader *desc)
{
	struct sock_xprt *transport =
				container_of(xprt, struct sock_xprt, xprt);
	struct rpc_rqst *req;

	req = xprt_alloc_bc_request(xprt);
	if (req == NULL) {
		printk(KERN_WARNING "Callback slot table overflowed\n");
		xprt_force_disconnect(xprt);
		return -1;
	}

	req->rq_xid = transport->tcp_xid;
	dprintk("RPC:       read callback  XID %08x\n", ntohl(req->rq_xid));
	xs_tcp_read_common(xprt, desc, req);

	if (!(transport->tcp_flags & TCP_RCV_COPY_DATA)) {
		struct svc_serv *bc_serv = xprt->bc_serv;

		/*
		 * Add callback request to callback list.  The callback
		 * service sleeps on the sv_cb_waitq waiting for new
		 * requests.  Wake it up after adding enqueing the
		 * request.
		 */
		dprintk("RPC:       add callback request to list\n");
		spin_lock(&bc_serv->sv_cb_lock);
		list_add(&req->rq_bc_list, &bc_serv->sv_cb_list);
		spin_unlock(&bc_serv->sv_cb_lock);
		wake_up(&bc_serv->sv_cb_waitq);
	}

	req->rq_private_buf.len = transport->tcp_copied;

	return 0;
}

static inline int _xs_tcp_read_data(struct rpc_xprt *xprt,
					struct xdr_skb_reader *desc)
{
	struct sock_xprt *transport =
				container_of(xprt, struct sock_xprt, xprt);

	return (transport->tcp_flags & TCP_RPC_REPLY) ?
		xs_tcp_read_reply(xprt, desc) :
		xs_tcp_read_callback(xprt, desc);
}
#else
static inline int _xs_tcp_read_data(struct rpc_xprt *xprt,
					struct xdr_skb_reader *desc)
{
	return xs_tcp_read_reply(xprt, desc);
}
#endif /* CONFIG_SUNRPC_BACKCHANNEL */

/*
 * Read data off the transport.  This can be either an RPC_CALL or an
 * RPC_REPLY.  Relay the processing to helper functions.
 */
static void xs_tcp_read_data(struct rpc_xprt *xprt,
				    struct xdr_skb_reader *desc)
{
	struct sock_xprt *transport =
				container_of(xprt, struct sock_xprt, xprt);

	if (_xs_tcp_read_data(xprt, desc) == 0)
		xs_tcp_check_fraghdr(transport);
	else {
		/*
		 * The transport_lock protects the request handling.
		 * There's no need to hold it to update the tcp_flags.
		 */
		transport->tcp_flags &= ~TCP_RCV_COPY_DATA;
	}
}

static inline void xs_tcp_read_discard(struct sock_xprt *transport, struct xdr_skb_reader *desc)
{
	size_t len;

	len = transport->tcp_reclen - transport->tcp_offset;
	if (len > desc->count)
		len = desc->count;
	desc->count -= len;
	desc->offset += len;
	transport->tcp_offset += len;
	dprintk("RPC:       discarded %Zu bytes\n", len);
	xs_tcp_check_fraghdr(transport);
}

static int xs_tcp_data_recv(read_descriptor_t *rd_desc, struct sk_buff *skb, unsigned int offset, size_t len)
{
	struct rpc_xprt *xprt = rd_desc->arg.data;
	struct sock_xprt *transport = container_of(xprt, struct sock_xprt, xprt);
	struct xdr_skb_reader desc = {
		.skb	= skb,
		.offset	= offset,
		.count	= len,
	};

	dprintk("RPC:       xs_tcp_data_recv started\n");
	do {
		/* Read in a new fragment marker if necessary */
		/* Can we ever really expect to get completely empty fragments? */
		if (transport->tcp_flags & TCP_RCV_COPY_FRAGHDR) {
			xs_tcp_read_fraghdr(xprt, &desc);
			continue;
		}
		/* Read in the xid if necessary */
		if (transport->tcp_flags & TCP_RCV_COPY_XID) {
			xs_tcp_read_xid(transport, &desc);
			continue;
		}
		/* Read in the call/reply flag */
		if (transport->tcp_flags & TCP_RCV_READ_CALLDIR) {
			xs_tcp_read_calldir(transport, &desc);
			continue;
		}
		/* Read in the request data */
		if (transport->tcp_flags & TCP_RCV_COPY_DATA) {
			xs_tcp_read_data(xprt, &desc);
			continue;
		}
		/* Skip over any trailing bytes on short reads */
		xs_tcp_read_discard(transport, &desc);
	} while (desc.count);
	dprintk("RPC:       xs_tcp_data_recv done\n");
	return len - desc.count;
}

/**
 * xs_tcp_data_ready - "data ready" callback for TCP sockets
 * @sk: socket with data to read
 * @bytes: how much data to read
 *
 */
static void xs_tcp_data_ready(struct sock *sk, int bytes)
{
	struct rpc_xprt *xprt;
	read_descriptor_t rd_desc;
	int read;

	dprintk("RPC:       xs_tcp_data_ready...\n");

	read_lock_bh(&sk->sk_callback_lock);
	if (!(xprt = xprt_from_sock(sk)))
		goto out;
	if (xprt->shutdown)
		goto out;

	/* Any data means we had a useful conversation, so
	 * the we don't need to delay the next reconnect
	 */
	if (xprt->reestablish_timeout)
		xprt->reestablish_timeout = 0;

	/* We use rd_desc to pass struct xprt to xs_tcp_data_recv */
	rd_desc.arg.data = xprt;
	do {
		rd_desc.count = 65536;
		read = tcp_read_sock(sk, &rd_desc, xs_tcp_data_recv);
	} while (read > 0);
out:
	read_unlock_bh(&sk->sk_callback_lock);
}

/*
 * Do the equivalent of linger/linger2 handling for dealing with
 * broken servers that don't close the socket in a timely
 * fashion
 */
static void xs_tcp_schedule_linger_timeout(struct rpc_xprt *xprt,
		unsigned long timeout)
{
	struct sock_xprt *transport;

	if (xprt_test_and_set_connecting(xprt))
		return;
	set_bit(XPRT_CONNECTION_ABORT, &xprt->state);
	transport = container_of(xprt, struct sock_xprt, xprt);
	queue_delayed_work(rpciod_workqueue, &transport->connect_worker,
			   timeout);
}

static void xs_tcp_cancel_linger_timeout(struct rpc_xprt *xprt)
{
	struct sock_xprt *transport;

	transport = container_of(xprt, struct sock_xprt, xprt);

	if (!test_bit(XPRT_CONNECTION_ABORT, &xprt->state) ||
	    !cancel_delayed_work(&transport->connect_worker))
		return;
	clear_bit(XPRT_CONNECTION_ABORT, &xprt->state);
	xprt_clear_connecting(xprt);
}

static void xs_sock_reset_connection_flags(struct rpc_xprt *xprt)
{
	smp_mb__before_clear_bit();
	clear_bit(XPRT_CONNECTION_ABORT, &xprt->state);
	clear_bit(XPRT_CONNECTION_CLOSE, &xprt->state);
	clear_bit(XPRT_CLOSE_WAIT, &xprt->state);
	clear_bit(XPRT_CLOSING, &xprt->state);
	smp_mb__after_clear_bit();
}

static void xs_sock_mark_closed(struct rpc_xprt *xprt)
{
	xs_sock_reset_connection_flags(xprt);
	/* Mark transport as closed and wake up all pending tasks */
	xprt_disconnect_done(xprt);
=======
	xprt_force_disconnect(xprt);
}

#if defined(CONFIG_SUNRPC_BACKCHANNEL)
static size_t xs_tcp_bc_maxpayload(struct rpc_xprt *xprt)
{
	return PAGE_SIZE;
}
#endif /* CONFIG_SUNRPC_BACKCHANNEL */

/**
 * xs_local_state_change - callback to handle AF_LOCAL socket state changes
 * @sk: socket whose state has changed
 *
 */
static void xs_local_state_change(struct sock *sk)
{
	struct rpc_xprt *xprt;
	struct sock_xprt *transport;

	if (!(xprt = xprt_from_sock(sk)))
		return;
	transport = container_of(xprt, struct sock_xprt, xprt);
	if (sk->sk_shutdown & SHUTDOWN_MASK) {
		clear_bit(XPRT_CONNECTED, &xprt->state);
		/* Trigger the socket release */
		xs_run_error_worker(transport, XPRT_SOCK_WAKE_DISCONNECT);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * xs_tcp_state_change - callback to handle TCP socket state changes
 * @sk: socket whose state has changed
 *
 */
static void xs_tcp_state_change(struct sock *sk)
{
	struct rpc_xprt *xprt;
<<<<<<< HEAD

	read_lock_bh(&sk->sk_callback_lock);
	if (!(xprt = xprt_from_sock(sk)))
		goto out;
=======
	struct sock_xprt *transport;

	if (!(xprt = xprt_from_sock(sk)))
		return;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dprintk("RPC:       xs_tcp_state_change client %p...\n", xprt);
	dprintk("RPC:       state %x conn %d dead %d zapped %d sk_shutdown %d\n",
			sk->sk_state, xprt_connected(xprt),
			sock_flag(sk, SOCK_DEAD),
			sock_flag(sk, SOCK_ZAPPED),
			sk->sk_shutdown);

<<<<<<< HEAD
	switch (sk->sk_state) {
	case TCP_ESTABLISHED:
		spin_lock(&xprt->transport_lock);
		if (!xprt_test_and_set_connected(xprt)) {
			struct sock_xprt *transport = container_of(xprt,
					struct sock_xprt, xprt);

			/* Reset TCP record info */
			transport->tcp_offset = 0;
			transport->tcp_reclen = 0;
			transport->tcp_copied = 0;
			transport->tcp_flags =
				TCP_RCV_COPY_FRAGHDR | TCP_RCV_COPY_XID;

			xprt_wake_pending_tasks(xprt, -EAGAIN);
		}
		spin_unlock(&xprt->transport_lock);
=======
	transport = container_of(xprt, struct sock_xprt, xprt);
	trace_rpc_socket_state_change(xprt, sk->sk_socket);
	switch (sk->sk_state) {
	case TCP_ESTABLISHED:
		if (!xprt_test_and_set_connected(xprt)) {
			xprt->connect_cookie++;
			clear_bit(XPRT_SOCK_CONNECTING, &transport->sock_state);
			xprt_clear_connecting(xprt);

			xprt->stat.connect_count++;
			xprt->stat.connect_time += (long)jiffies -
						   xprt->stat.connect_start;
			xs_run_error_worker(transport, XPRT_SOCK_WAKE_PENDING);
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case TCP_FIN_WAIT1:
		/* The client initiated a shutdown of the socket */
		xprt->connect_cookie++;
		xprt->reestablish_timeout = 0;
		set_bit(XPRT_CLOSING, &xprt->state);
<<<<<<< HEAD
		smp_mb__before_clear_bit();
		clear_bit(XPRT_CONNECTED, &xprt->state);
		clear_bit(XPRT_CLOSE_WAIT, &xprt->state);
		smp_mb__after_clear_bit();
		xs_tcp_schedule_linger_timeout(xprt, xs_tcp_fin_timeout);
=======
		smp_mb__before_atomic();
		clear_bit(XPRT_CONNECTED, &xprt->state);
		clear_bit(XPRT_CLOSE_WAIT, &xprt->state);
		smp_mb__after_atomic();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case TCP_CLOSE_WAIT:
		/* The server initiated a shutdown of the socket */
		xprt->connect_cookie++;
		clear_bit(XPRT_CONNECTED, &xprt->state);
<<<<<<< HEAD
		xs_tcp_force_close(xprt);
=======
		xs_run_error_worker(transport, XPRT_SOCK_WAKE_DISCONNECT);
		fallthrough;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case TCP_CLOSING:
		/*
		 * If the server closed down the connection, make sure that
		 * we back off before reconnecting
		 */
		if (xprt->reestablish_timeout < XS_TCP_INIT_REEST_TO)
			xprt->reestablish_timeout = XS_TCP_INIT_REEST_TO;
		break;
	case TCP_LAST_ACK:
		set_bit(XPRT_CLOSING, &xprt->state);
<<<<<<< HEAD
		xs_tcp_schedule_linger_timeout(xprt, xs_tcp_fin_timeout);
		smp_mb__before_clear_bit();
		clear_bit(XPRT_CONNECTED, &xprt->state);
		smp_mb__after_clear_bit();
		break;
	case TCP_CLOSE:
		xs_tcp_cancel_linger_timeout(xprt);
		xs_sock_mark_closed(xprt);
	}
 out:
	read_unlock_bh(&sk->sk_callback_lock);
=======
		smp_mb__before_atomic();
		clear_bit(XPRT_CONNECTED, &xprt->state);
		smp_mb__after_atomic();
		break;
	case TCP_CLOSE:
		if (test_and_clear_bit(XPRT_SOCK_CONNECTING,
				       &transport->sock_state)) {
			xs_reset_srcport(transport);
			xprt_clear_connecting(xprt);
		}
		clear_bit(XPRT_CLOSING, &xprt->state);
		/* Trigger the socket release */
		xs_run_error_worker(transport, XPRT_SOCK_WAKE_DISCONNECT);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void xs_write_space(struct sock *sk)
{
<<<<<<< HEAD
	struct socket *sock;
	struct rpc_xprt *xprt;

	if (unlikely(!(sock = sk->sk_socket)))
		return;
	clear_bit(SOCK_NOSPACE, &sock->flags);

	if (unlikely(!(xprt = xprt_from_sock(sk))))
		return;
	if (test_and_clear_bit(SOCK_ASYNC_NOSPACE, &sock->flags) == 0)
		return;

	xprt_write_space(xprt);
=======
	struct sock_xprt *transport;
	struct rpc_xprt *xprt;

	if (!sk->sk_socket)
		return;
	clear_bit(SOCK_NOSPACE, &sk->sk_socket->flags);

	if (unlikely(!(xprt = xprt_from_sock(sk))))
		return;
	transport = container_of(xprt, struct sock_xprt, xprt);
	if (!test_and_clear_bit(XPRT_SOCK_NOSPACE, &transport->sock_state))
		return;
	xs_run_error_worker(transport, XPRT_SOCK_WAKE_WRITE);
	sk->sk_write_pending--;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * xs_udp_write_space - callback invoked when socket buffer space
 *                             becomes available
 * @sk: socket whose state has changed
 *
 * Called when more output buffer space is available for this socket.
 * We try not to wake our writers until they can make "significant"
 * progress, otherwise we'll waste resources thrashing kernel_sendmsg
 * with a bunch of small requests.
 */
static void xs_udp_write_space(struct sock *sk)
{
<<<<<<< HEAD
	read_lock_bh(&sk->sk_callback_lock);

	/* from net/core/sock.c:sock_def_write_space */
	if (sock_writeable(sk))
		xs_write_space(sk);

	read_unlock_bh(&sk->sk_callback_lock);
=======
	/* from net/core/sock.c:sock_def_write_space */
	if (sock_writeable(sk))
		xs_write_space(sk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * xs_tcp_write_space - callback invoked when socket buffer space
 *                             becomes available
 * @sk: socket whose state has changed
 *
 * Called when more output buffer space is available for this socket.
 * We try not to wake our writers until they can make "significant"
 * progress, otherwise we'll waste resources thrashing kernel_sendmsg
 * with a bunch of small requests.
 */
static void xs_tcp_write_space(struct sock *sk)
{
<<<<<<< HEAD
	read_lock_bh(&sk->sk_callback_lock);

	/* from net/core/stream.c:sk_stream_write_space */
	if (sk_stream_wspace(sk) >= sk_stream_min_wspace(sk))
		xs_write_space(sk);

	read_unlock_bh(&sk->sk_callback_lock);
=======
	/* from net/core/stream.c:sk_stream_write_space */
	if (sk_stream_is_writeable(sk))
		xs_write_space(sk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void xs_udp_do_set_buffer_size(struct rpc_xprt *xprt)
{
	struct sock_xprt *transport = container_of(xprt, struct sock_xprt, xprt);
	struct sock *sk = transport->inet;

	if (transport->rcvsize) {
		sk->sk_userlocks |= SOCK_RCVBUF_LOCK;
		sk->sk_rcvbuf = transport->rcvsize * xprt->max_reqs * 2;
	}
	if (transport->sndsize) {
		sk->sk_userlocks |= SOCK_SNDBUF_LOCK;
		sk->sk_sndbuf = transport->sndsize * xprt->max_reqs * 2;
		sk->sk_write_space(sk);
	}
}

/**
 * xs_udp_set_buffer_size - set send and receive limits
 * @xprt: generic transport
 * @sndsize: requested size of send buffer, in bytes
 * @rcvsize: requested size of receive buffer, in bytes
 *
 * Set socket send and receive buffer size limits.
 */
static void xs_udp_set_buffer_size(struct rpc_xprt *xprt, size_t sndsize, size_t rcvsize)
{
	struct sock_xprt *transport = container_of(xprt, struct sock_xprt, xprt);

	transport->sndsize = 0;
	if (sndsize)
		transport->sndsize = sndsize + 1024;
	transport->rcvsize = 0;
	if (rcvsize)
		transport->rcvsize = rcvsize + 1024;

	xs_udp_do_set_buffer_size(xprt);
}

/**
 * xs_udp_timer - called when a retransmit timeout occurs on a UDP transport
<<<<<<< HEAD
=======
 * @xprt: controlling transport
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @task: task that timed out
 *
 * Adjust the congestion window after a retransmit timeout has occurred.
 */
<<<<<<< HEAD
static void xs_udp_timer(struct rpc_task *task)
{
	xprt_adjust_cwnd(task, -ETIMEDOUT);
}

static unsigned short xs_get_random_port(void)
{
	unsigned short range = xprt_max_resvport - xprt_min_resvport;
	unsigned short rand = (unsigned short) net_random() % range;
	return rand + xprt_min_resvport;
=======
static void xs_udp_timer(struct rpc_xprt *xprt, struct rpc_task *task)
{
	spin_lock(&xprt->transport_lock);
	xprt_adjust_cwnd(xprt, task, -ETIMEDOUT);
	spin_unlock(&xprt->transport_lock);
}

static int xs_get_random_port(void)
{
	unsigned short min = xprt_min_resvport, max = xprt_max_resvport;
	unsigned short range;
	unsigned short rand;

	if (max < min)
		return -EADDRINUSE;
	range = max - min + 1;
	rand = get_random_u32_below(range);
	return rand + min;
}

static unsigned short xs_sock_getport(struct socket *sock)
{
	struct sockaddr_storage buf;
	unsigned short port = 0;

	if (kernel_getsockname(sock, (struct sockaddr *)&buf) < 0)
		goto out;
	switch (buf.ss_family) {
	case AF_INET6:
		port = ntohs(((struct sockaddr_in6 *)&buf)->sin6_port);
		break;
	case AF_INET:
		port = ntohs(((struct sockaddr_in *)&buf)->sin_port);
	}
out:
	return port;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * xs_set_port - reset the port number in the remote endpoint address
 * @xprt: generic transport
 * @port: new port number
 *
 */
static void xs_set_port(struct rpc_xprt *xprt, unsigned short port)
{
	dprintk("RPC:       setting port for xprt %p to %u\n", xprt, port);

	rpc_set_port(xs_addr(xprt), port);
	xs_update_peer_port(xprt);
}

<<<<<<< HEAD
static unsigned short xs_get_srcport(struct sock_xprt *transport)
{
	unsigned short port = transport->srcport;
=======
static void xs_reset_srcport(struct sock_xprt *transport)
{
	transport->srcport = 0;
}

static void xs_set_srcport(struct sock_xprt *transport, struct socket *sock)
{
	if (transport->srcport == 0 && transport->xprt.reuseport)
		transport->srcport = xs_sock_getport(sock);
}

static int xs_get_srcport(struct sock_xprt *transport)
{
	int port = transport->srcport;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (port == 0 && transport->xprt.resvport)
		port = xs_get_random_port();
	return port;
}

<<<<<<< HEAD
=======
static unsigned short xs_sock_srcport(struct rpc_xprt *xprt)
{
	struct sock_xprt *sock = container_of(xprt, struct sock_xprt, xprt);
	unsigned short ret = 0;
	mutex_lock(&sock->recv_mutex);
	if (sock->sock)
		ret = xs_sock_getport(sock->sock);
	mutex_unlock(&sock->recv_mutex);
	return ret;
}

static int xs_sock_srcaddr(struct rpc_xprt *xprt, char *buf, size_t buflen)
{
	struct sock_xprt *sock = container_of(xprt, struct sock_xprt, xprt);
	union {
		struct sockaddr sa;
		struct sockaddr_storage st;
	} saddr;
	int ret = -ENOTCONN;

	mutex_lock(&sock->recv_mutex);
	if (sock->sock) {
		ret = kernel_getsockname(sock->sock, &saddr.sa);
		if (ret >= 0)
			ret = snprintf(buf, buflen, "%pISc", &saddr.sa);
	}
	mutex_unlock(&sock->recv_mutex);
	return ret;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static unsigned short xs_next_srcport(struct sock_xprt *transport, unsigned short port)
{
	if (transport->srcport != 0)
		transport->srcport = 0;
	if (!transport->xprt.resvport)
		return 0;
	if (port <= xprt_min_resvport || port > xprt_max_resvport)
		return xprt_max_resvport;
	return --port;
}
static int xs_bind(struct sock_xprt *transport, struct socket *sock)
{
	struct sockaddr_storage myaddr;
	int err, nloop = 0;
<<<<<<< HEAD
	unsigned short port = xs_get_srcport(transport);
	unsigned short last;

=======
	int port = xs_get_srcport(transport);
	unsigned short last;

	/*
	 * If we are asking for any ephemeral port (i.e. port == 0 &&
	 * transport->xprt.resvport == 0), don't bind.  Let the local
	 * port selection happen implicitly when the socket is used
	 * (for example at connect time).
	 *
	 * This ensures that we can continue to establish TCP
	 * connections even when all local ephemeral ports are already
	 * a part of some TCP connection.  This makes no difference
	 * for UDP sockets, but also doesn't harm them.
	 *
	 * If we're asking for any reserved port (i.e. port == 0 &&
	 * transport->xprt.resvport == 1) xs_get_srcport above will
	 * ensure that port is non-zero and we will bind as needed.
	 */
	if (port <= 0)
		return port;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	memcpy(&myaddr, &transport->srcaddr, transport->xprt.addrlen);
	do {
		rpc_set_port((struct sockaddr *)&myaddr, port);
		err = kernel_bind(sock, (struct sockaddr *)&myaddr,
				transport->xprt.addrlen);
<<<<<<< HEAD
		if (port == 0)
			break;
		if (err == 0) {
			transport->srcport = port;
=======
		if (err == 0) {
			if (transport->xprt.reuseport)
				transport->srcport = port;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		}
		last = port;
		port = xs_next_srcport(transport, port);
		if (port > last)
			nloop++;
	} while (err == -EADDRINUSE && nloop != 2);

	if (myaddr.ss_family == AF_INET)
		dprintk("RPC:       %s %pI4:%u: %s (%d)\n", __func__,
				&((struct sockaddr_in *)&myaddr)->sin_addr,
				port, err ? "failed" : "ok", err);
	else
		dprintk("RPC:       %s %pI6:%u: %s (%d)\n", __func__,
				&((struct sockaddr_in6 *)&myaddr)->sin6_addr,
				port, err ? "failed" : "ok", err);
	return err;
}

/*
 * We don't support autobind on AF_LOCAL sockets
 */
static void xs_local_rpcbind(struct rpc_task *task)
{
	xprt_set_bound(task->tk_xprt);
}

static void xs_local_set_port(struct rpc_xprt *xprt, unsigned short port)
{
}

#ifdef CONFIG_DEBUG_LOCK_ALLOC
<<<<<<< HEAD
static struct lock_class_key xs_key[2];
static struct lock_class_key xs_slock_key[2];
=======
static struct lock_class_key xs_key[3];
static struct lock_class_key xs_slock_key[3];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline void xs_reclassify_socketu(struct socket *sock)
{
	struct sock *sk = sock->sk;

<<<<<<< HEAD
	BUG_ON(sock_owned_by_user(sk));
	sock_lock_init_class_and_name(sk, "slock-AF_LOCAL-RPC",
		&xs_slock_key[1], "sk_lock-AF_LOCAL-RPC", &xs_key[1]);
=======
	sock_lock_init_class_and_name(sk, "slock-AF_LOCAL-RPC",
		&xs_slock_key[0], "sk_lock-AF_LOCAL-RPC", &xs_key[0]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void xs_reclassify_socket4(struct socket *sock)
{
	struct sock *sk = sock->sk;

<<<<<<< HEAD
	BUG_ON(sock_owned_by_user(sk));
	sock_lock_init_class_and_name(sk, "slock-AF_INET-RPC",
		&xs_slock_key[0], "sk_lock-AF_INET-RPC", &xs_key[0]);
=======
	sock_lock_init_class_and_name(sk, "slock-AF_INET-RPC",
		&xs_slock_key[1], "sk_lock-AF_INET-RPC", &xs_key[1]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void xs_reclassify_socket6(struct socket *sock)
{
	struct sock *sk = sock->sk;

<<<<<<< HEAD
	BUG_ON(sock_owned_by_user(sk));
	sock_lock_init_class_and_name(sk, "slock-AF_INET6-RPC",
		&xs_slock_key[1], "sk_lock-AF_INET6-RPC", &xs_key[1]);
=======
	sock_lock_init_class_and_name(sk, "slock-AF_INET6-RPC",
		&xs_slock_key[2], "sk_lock-AF_INET6-RPC", &xs_key[2]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void xs_reclassify_socket(int family, struct socket *sock)
{
<<<<<<< HEAD
=======
	if (WARN_ON_ONCE(!sock_allow_reclassification(sock->sk)))
		return;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	switch (family) {
	case AF_LOCAL:
		xs_reclassify_socketu(sock);
		break;
	case AF_INET:
		xs_reclassify_socket4(sock);
		break;
	case AF_INET6:
		xs_reclassify_socket6(sock);
		break;
	}
}
#else
<<<<<<< HEAD
static inline void xs_reclassify_socketu(struct socket *sock)
{
}

static inline void xs_reclassify_socket4(struct socket *sock)
{
}

static inline void xs_reclassify_socket6(struct socket *sock)
{
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline void xs_reclassify_socket(int family, struct socket *sock)
{
}
#endif

<<<<<<< HEAD
static struct socket *xs_create_sock(struct rpc_xprt *xprt,
		struct sock_xprt *transport, int family, int type, int protocol)
{
=======
static void xs_dummy_setup_socket(struct work_struct *work)
{
}

static struct socket *xs_create_sock(struct rpc_xprt *xprt,
		struct sock_xprt *transport, int family, int type,
		int protocol, bool reuseport)
{
	struct file *filp;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct socket *sock;
	int err;

	err = __sock_create(xprt->xprt_net, family, type, protocol, &sock, 1);
	if (err < 0) {
		dprintk("RPC:       can't create %d transport socket (%d).\n",
				protocol, -err);
		goto out;
	}
	xs_reclassify_socket(family, sock);

<<<<<<< HEAD
=======
	if (reuseport)
		sock_set_reuseport(sock->sk);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	err = xs_bind(transport, sock);
	if (err) {
		sock_release(sock);
		goto out;
	}

<<<<<<< HEAD
=======
	filp = sock_alloc_file(sock, O_NONBLOCK, NULL);
	if (IS_ERR(filp))
		return ERR_CAST(filp);
	transport->file = filp;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return sock;
out:
	return ERR_PTR(err);
}

static int xs_local_finish_connecting(struct rpc_xprt *xprt,
				      struct socket *sock)
{
	struct sock_xprt *transport = container_of(xprt, struct sock_xprt,
									xprt);

	if (!transport->inet) {
		struct sock *sk = sock->sk;

<<<<<<< HEAD
		write_lock_bh(&sk->sk_callback_lock);
=======
		lock_sock(sk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		xs_save_old_callbacks(transport, sk);

		sk->sk_user_data = xprt;
<<<<<<< HEAD
		sk->sk_data_ready = xs_local_data_ready;
		sk->sk_write_space = xs_udp_write_space;
		sk->sk_allocation = GFP_ATOMIC;
=======
		sk->sk_data_ready = xs_data_ready;
		sk->sk_write_space = xs_udp_write_space;
		sk->sk_state_change = xs_local_state_change;
		sk->sk_error_report = xs_error_report;
		sk->sk_use_task_frag = false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		xprt_clear_connected(xprt);

		/* Reset to new socket */
		transport->sock = sock;
		transport->inet = sk;

<<<<<<< HEAD
		write_unlock_bh(&sk->sk_callback_lock);
	}

	/* Tell the socket layer to start connecting... */
	xprt->stat.connect_count++;
	xprt->stat.connect_start = jiffies;
=======
		release_sock(sk);
	}

	xs_stream_start_connect(transport);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return kernel_connect(sock, xs_addr(xprt), xprt->addrlen, 0);
}

/**
 * xs_local_setup_socket - create AF_LOCAL socket, connect to a local endpoint
<<<<<<< HEAD
 * @xprt: RPC transport to connect
 * @transport: socket transport to connect
 * @create_sock: function to create a socket of the correct type
 *
 * Invoked by a work queue tasklet.
 */
static void xs_local_setup_socket(struct work_struct *work)
{
	struct sock_xprt *transport =
		container_of(work, struct sock_xprt, connect_worker.work);
	struct rpc_xprt *xprt = &transport->xprt;
	struct socket *sock;
	int status = -EIO;

	if (xprt->shutdown)
		goto out;

	current->flags |= PF_FSTRANS;

	clear_bit(XPRT_CONNECTION_ABORT, &xprt->state);
=======
 * @transport: socket transport to connect
 */
static int xs_local_setup_socket(struct sock_xprt *transport)
{
	struct rpc_xprt *xprt = &transport->xprt;
	struct file *filp;
	struct socket *sock;
	int status;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	status = __sock_create(xprt->xprt_net, AF_LOCAL,
					SOCK_STREAM, 0, &sock, 1);
	if (status < 0) {
		dprintk("RPC:       can't create AF_LOCAL "
			"transport socket (%d).\n", -status);
		goto out;
	}
<<<<<<< HEAD
	xs_reclassify_socketu(sock);
=======
	xs_reclassify_socket(AF_LOCAL, sock);

	filp = sock_alloc_file(sock, O_NONBLOCK, NULL);
	if (IS_ERR(filp)) {
		status = PTR_ERR(filp);
		goto out;
	}
	transport->file = filp;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	dprintk("RPC:       worker connecting xprt %p via AF_LOCAL to %s\n",
			xprt, xprt->address_strings[RPC_DISPLAY_ADDR]);

	status = xs_local_finish_connecting(xprt, sock);
<<<<<<< HEAD
=======
	trace_rpc_socket_connect(xprt, sock, status);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	switch (status) {
	case 0:
		dprintk("RPC:       xprt %p connected to %s\n",
				xprt, xprt->address_strings[RPC_DISPLAY_ADDR]);
<<<<<<< HEAD
		xprt_set_connected(xprt);
		break;
=======
		xprt->stat.connect_count++;
		xprt->stat.connect_time += (long)jiffies -
					   xprt->stat.connect_start;
		xprt_set_connected(xprt);
		break;
	case -ENOBUFS:
		break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case -ENOENT:
		dprintk("RPC:       xprt %p: socket %s does not exist\n",
				xprt, xprt->address_strings[RPC_DISPLAY_ADDR]);
		break;
<<<<<<< HEAD
=======
	case -ECONNREFUSED:
		dprintk("RPC:       xprt %p: connection refused for %s\n",
				xprt, xprt->address_strings[RPC_DISPLAY_ADDR]);
		break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	default:
		printk(KERN_ERR "%s: unhandled error (%d) connecting to %s\n",
				__func__, -status,
				xprt->address_strings[RPC_DISPLAY_ADDR]);
	}

out:
	xprt_clear_connecting(xprt);
	xprt_wake_pending_tasks(xprt, status);
<<<<<<< HEAD
	current->flags &= ~PF_FSTRANS;
}

=======
	return status;
}

static void xs_local_connect(struct rpc_xprt *xprt, struct rpc_task *task)
{
	struct sock_xprt *transport = container_of(xprt, struct sock_xprt, xprt);
	int ret;

	if (transport->file)
		goto force_disconnect;

	if (RPC_IS_ASYNC(task)) {
		/*
		 * We want the AF_LOCAL connect to be resolved in the
		 * filesystem namespace of the process making the rpc
		 * call.  Thus we connect synchronously.
		 *
		 * If we want to support asynchronous AF_LOCAL calls,
		 * we'll need to figure out how to pass a namespace to
		 * connect.
		 */
		rpc_task_set_rpc_status(task, -ENOTCONN);
		goto out_wake;
	}
	ret = xs_local_setup_socket(transport);
	if (ret && !RPC_IS_SOFTCONN(task))
		msleep_interruptible(15000);
	return;
force_disconnect:
	xprt_force_disconnect(xprt);
out_wake:
	xprt_clear_connecting(xprt);
	xprt_wake_pending_tasks(xprt, -ENOTCONN);
}

#if IS_ENABLED(CONFIG_SUNRPC_SWAP)
/*
 * Note that this should be called with XPRT_LOCKED held, or recv_mutex
 * held, or when we otherwise know that we have exclusive access to the
 * socket, to guard against races with xs_reset_transport.
 */
static void xs_set_memalloc(struct rpc_xprt *xprt)
{
	struct sock_xprt *transport = container_of(xprt, struct sock_xprt,
			xprt);

	/*
	 * If there's no sock, then we have nothing to set. The
	 * reconnecting process will get it for us.
	 */
	if (!transport->inet)
		return;
	if (atomic_read(&xprt->swapper))
		sk_set_memalloc(transport->inet);
}

/**
 * xs_enable_swap - Tag this transport as being used for swap.
 * @xprt: transport to tag
 *
 * Take a reference to this transport on behalf of the rpc_clnt, and
 * optionally mark it for swapping if it wasn't already.
 */
static int
xs_enable_swap(struct rpc_xprt *xprt)
{
	struct sock_xprt *xs = container_of(xprt, struct sock_xprt, xprt);

	mutex_lock(&xs->recv_mutex);
	if (atomic_inc_return(&xprt->swapper) == 1 &&
	    xs->inet)
		sk_set_memalloc(xs->inet);
	mutex_unlock(&xs->recv_mutex);
	return 0;
}

/**
 * xs_disable_swap - Untag this transport as being used for swap.
 * @xprt: transport to tag
 *
 * Drop a "swapper" reference to this xprt on behalf of the rpc_clnt. If the
 * swapper refcount goes to 0, untag the socket as a memalloc socket.
 */
static void
xs_disable_swap(struct rpc_xprt *xprt)
{
	struct sock_xprt *xs = container_of(xprt, struct sock_xprt, xprt);

	mutex_lock(&xs->recv_mutex);
	if (atomic_dec_and_test(&xprt->swapper) &&
	    xs->inet)
		sk_clear_memalloc(xs->inet);
	mutex_unlock(&xs->recv_mutex);
}
#else
static void xs_set_memalloc(struct rpc_xprt *xprt)
{
}

static int
xs_enable_swap(struct rpc_xprt *xprt)
{
	return -EINVAL;
}

static void
xs_disable_swap(struct rpc_xprt *xprt)
{
}
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void xs_udp_finish_connecting(struct rpc_xprt *xprt, struct socket *sock)
{
	struct sock_xprt *transport = container_of(xprt, struct sock_xprt, xprt);

	if (!transport->inet) {
		struct sock *sk = sock->sk;

<<<<<<< HEAD
		write_lock_bh(&sk->sk_callback_lock);
=======
		lock_sock(sk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		xs_save_old_callbacks(transport, sk);

		sk->sk_user_data = xprt;
<<<<<<< HEAD
		sk->sk_data_ready = xs_udp_data_ready;
		sk->sk_write_space = xs_udp_write_space;
		sk->sk_no_check = UDP_CSUM_NORCV;
		sk->sk_allocation = GFP_ATOMIC;
=======
		sk->sk_data_ready = xs_data_ready;
		sk->sk_write_space = xs_udp_write_space;
		sk->sk_use_task_frag = false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		xprt_set_connected(xprt);

		/* Reset to new socket */
		transport->sock = sock;
		transport->inet = sk;

<<<<<<< HEAD
		write_unlock_bh(&sk->sk_callback_lock);
	}
	xs_udp_do_set_buffer_size(xprt);
=======
		xs_set_memalloc(xprt);

		release_sock(sk);
	}
	xs_udp_do_set_buffer_size(xprt);

	xprt->stat.connect_start = jiffies;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void xs_udp_setup_socket(struct work_struct *work)
{
	struct sock_xprt *transport =
		container_of(work, struct sock_xprt, connect_worker.work);
	struct rpc_xprt *xprt = &transport->xprt;
<<<<<<< HEAD
	struct socket *sock = transport->sock;
	int status = -EIO;

	if (xprt->shutdown)
		goto out;

	current->flags |= PF_FSTRANS;

	/* Start by resetting any existing state */
	xs_reset_transport(transport);
	sock = xs_create_sock(xprt, transport,
			xs_addr(xprt)->sa_family, SOCK_DGRAM, IPPROTO_UDP);
=======
	struct socket *sock;
	int status = -EIO;
	unsigned int pflags = current->flags;

	if (atomic_read(&xprt->swapper))
		current->flags |= PF_MEMALLOC;
	sock = xs_create_sock(xprt, transport,
			xs_addr(xprt)->sa_family, SOCK_DGRAM,
			IPPROTO_UDP, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (IS_ERR(sock))
		goto out;

	dprintk("RPC:       worker connecting xprt %p via %s to "
				"%s (port %s)\n", xprt,
			xprt->address_strings[RPC_DISPLAY_PROTO],
			xprt->address_strings[RPC_DISPLAY_ADDR],
			xprt->address_strings[RPC_DISPLAY_PORT]);

	xs_udp_finish_connecting(xprt, sock);
<<<<<<< HEAD
	status = 0;
out:
	xprt_clear_connecting(xprt);
	xprt_wake_pending_tasks(xprt, status);
	current->flags &= ~PF_FSTRANS;
}

/*
 * We need to preserve the port number so the reply cache on the server can
 * find our cached RPC replies when we get around to reconnecting.
 */
static void xs_abort_connection(struct sock_xprt *transport)
{
	int result;
	struct sockaddr any;

	dprintk("RPC:       disconnecting xprt %p to reuse port\n", transport);

	/*
	 * Disconnect the transport socket by doing a connect operation
	 * with AF_UNSPEC.  This should return immediately...
	 */
	memset(&any, 0, sizeof(any));
	any.sa_family = AF_UNSPEC;
	result = kernel_connect(transport->sock, &any, sizeof(any), 0);
	if (!result)
		xs_sock_reset_connection_flags(&transport->xprt);
	dprintk("RPC:       AF_UNSPEC connect return code %d\n", result);
}

static void xs_tcp_reuse_connection(struct sock_xprt *transport)
{
	unsigned int state = transport->inet->sk_state;

	if (state == TCP_CLOSE && transport->sock->state == SS_UNCONNECTED) {
		/* we don't need to abort the connection if the socket
		 * hasn't undergone a shutdown
		 */
		if (transport->inet->sk_shutdown == 0)
			return;
		dprintk("RPC:       %s: TCP_CLOSEd and sk_shutdown set to %d\n",
				__func__, transport->inet->sk_shutdown);
	}
	if ((1 << state) & (TCPF_ESTABLISHED|TCPF_SYN_SENT)) {
		/* we don't need to abort the connection if the socket
		 * hasn't undergone a shutdown
		 */
		if (transport->inet->sk_shutdown == 0)
			return;
		dprintk("RPC:       %s: ESTABLISHED/SYN_SENT "
				"sk_shutdown set to %d\n",
				__func__, transport->inet->sk_shutdown);
	}
	xs_abort_connection(transport);
=======
	trace_rpc_socket_connect(xprt, sock, 0);
	status = 0;
out:
	xprt_clear_connecting(xprt);
	xprt_unlock_connect(xprt, transport);
	xprt_wake_pending_tasks(xprt, status);
	current_restore_flags(pflags, PF_MEMALLOC);
}

/**
 * xs_tcp_shutdown - gracefully shut down a TCP socket
 * @xprt: transport
 *
 * Initiates a graceful shutdown of the TCP socket by calling the
 * equivalent of shutdown(SHUT_RDWR);
 */
static void xs_tcp_shutdown(struct rpc_xprt *xprt)
{
	struct sock_xprt *transport = container_of(xprt, struct sock_xprt, xprt);
	struct socket *sock = transport->sock;
	int skst = transport->inet ? transport->inet->sk_state : TCP_CLOSE;

	if (sock == NULL)
		return;
	if (!xprt->reuseport) {
		xs_close(xprt);
		return;
	}
	switch (skst) {
	case TCP_FIN_WAIT1:
	case TCP_FIN_WAIT2:
	case TCP_LAST_ACK:
		break;
	case TCP_ESTABLISHED:
	case TCP_CLOSE_WAIT:
		kernel_sock_shutdown(sock, SHUT_RDWR);
		trace_rpc_socket_shutdown(xprt, sock);
		break;
	default:
		xs_reset_transport(transport);
	}
}

static void xs_tcp_set_socket_timeouts(struct rpc_xprt *xprt,
		struct socket *sock)
{
	struct sock_xprt *transport = container_of(xprt, struct sock_xprt, xprt);
	struct net *net = sock_net(sock->sk);
	unsigned long connect_timeout;
	unsigned long syn_retries;
	unsigned int keepidle;
	unsigned int keepcnt;
	unsigned int timeo;
	unsigned long t;

	spin_lock(&xprt->transport_lock);
	keepidle = DIV_ROUND_UP(xprt->timeout->to_initval, HZ);
	keepcnt = xprt->timeout->to_retries + 1;
	timeo = jiffies_to_msecs(xprt->timeout->to_initval) *
		(xprt->timeout->to_retries + 1);
	clear_bit(XPRT_SOCK_UPD_TIMEOUT, &transport->sock_state);
	spin_unlock(&xprt->transport_lock);

	/* TCP Keepalive options */
	sock_set_keepalive(sock->sk);
	tcp_sock_set_keepidle(sock->sk, keepidle);
	tcp_sock_set_keepintvl(sock->sk, keepidle);
	tcp_sock_set_keepcnt(sock->sk, keepcnt);

	/* TCP user timeout (see RFC5482) */
	tcp_sock_set_user_timeout(sock->sk, timeo);

	/* Connect timeout */
	connect_timeout = max_t(unsigned long,
				DIV_ROUND_UP(xprt->connect_timeout, HZ), 1);
	syn_retries = max_t(unsigned long,
			    READ_ONCE(net->ipv4.sysctl_tcp_syn_retries), 1);
	for (t = 0; t <= syn_retries && (1UL << t) < connect_timeout; t++)
		;
	if (t <= syn_retries)
		tcp_sock_set_syncnt(sock->sk, t - 1);
}

static void xs_tcp_do_set_connect_timeout(struct rpc_xprt *xprt,
					  unsigned long connect_timeout)
{
	struct sock_xprt *transport =
		container_of(xprt, struct sock_xprt, xprt);
	struct rpc_timeout to;
	unsigned long initval;

	memcpy(&to, xprt->timeout, sizeof(to));
	/* Arbitrary lower limit */
	initval = max_t(unsigned long, connect_timeout, XS_TCP_INIT_REEST_TO);
	to.to_initval = initval;
	to.to_maxval = initval;
	to.to_retries = 0;
	memcpy(&transport->tcp_timeout, &to, sizeof(transport->tcp_timeout));
	xprt->timeout = &transport->tcp_timeout;
	xprt->connect_timeout = connect_timeout;
}

static void xs_tcp_set_connect_timeout(struct rpc_xprt *xprt,
		unsigned long connect_timeout,
		unsigned long reconnect_timeout)
{
	struct sock_xprt *transport = container_of(xprt, struct sock_xprt, xprt);

	spin_lock(&xprt->transport_lock);
	if (reconnect_timeout < xprt->max_reconnect_timeout)
		xprt->max_reconnect_timeout = reconnect_timeout;
	if (connect_timeout < xprt->connect_timeout)
		xs_tcp_do_set_connect_timeout(xprt, connect_timeout);
	set_bit(XPRT_SOCK_UPD_TIMEOUT, &transport->sock_state);
	spin_unlock(&xprt->transport_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int xs_tcp_finish_connecting(struct rpc_xprt *xprt, struct socket *sock)
{
	struct sock_xprt *transport = container_of(xprt, struct sock_xprt, xprt);
<<<<<<< HEAD
	int ret = -ENOTCONN;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!transport->inet) {
		struct sock *sk = sock->sk;

<<<<<<< HEAD
		write_lock_bh(&sk->sk_callback_lock);
=======
		/* Avoid temporary address, they are bad for long-lived
		 * connections such as NFS mounts.
		 * RFC4941, section 3.6 suggests that:
		 *    Individual applications, which have specific
		 *    knowledge about the normal duration of connections,
		 *    MAY override this as appropriate.
		 */
		if (xs_addr(xprt)->sa_family == PF_INET6) {
			ip6_sock_set_addr_preferences(sk,
				IPV6_PREFER_SRC_PUBLIC);
		}

		xs_tcp_set_socket_timeouts(xprt, sock);
		tcp_sock_set_nodelay(sk);

		lock_sock(sk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		xs_save_old_callbacks(transport, sk);

		sk->sk_user_data = xprt;
<<<<<<< HEAD
		sk->sk_data_ready = xs_tcp_data_ready;
		sk->sk_state_change = xs_tcp_state_change;
		sk->sk_write_space = xs_tcp_write_space;
		sk->sk_allocation = GFP_ATOMIC;

		/* socket options */
		sk->sk_userlocks |= SOCK_BINDPORT_LOCK;
		sock_reset_flag(sk, SOCK_LINGER);
		tcp_sk(sk)->linger2 = 0;
		tcp_sk(sk)->nonagle |= TCP_NAGLE_OFF;
=======
		sk->sk_data_ready = xs_data_ready;
		sk->sk_state_change = xs_tcp_state_change;
		sk->sk_write_space = xs_tcp_write_space;
		sk->sk_error_report = xs_error_report;
		sk->sk_use_task_frag = false;

		/* socket options */
		sock_reset_flag(sk, SOCK_LINGER);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		xprt_clear_connected(xprt);

		/* Reset to new socket */
		transport->sock = sock;
		transport->inet = sk;

<<<<<<< HEAD
		write_unlock_bh(&sk->sk_callback_lock);
	}

	if (!xprt_bound(xprt))
		goto out;

	/* Tell the socket layer to start connecting... */
	xprt->stat.connect_count++;
	xprt->stat.connect_start = jiffies;
	ret = kernel_connect(sock, xs_addr(xprt), xprt->addrlen, O_NONBLOCK);
	switch (ret) {
	case 0:
	case -EINPROGRESS:
		/* SYN_SENT! */
		xprt->connect_cookie++;
		if (xprt->reestablish_timeout < XS_TCP_INIT_REEST_TO)
			xprt->reestablish_timeout = XS_TCP_INIT_REEST_TO;
	}
out:
	return ret;
=======
		release_sock(sk);
	}

	if (!xprt_bound(xprt))
		return -ENOTCONN;

	xs_set_memalloc(xprt);

	xs_stream_start_connect(transport);

	/* Tell the socket layer to start connecting... */
	set_bit(XPRT_SOCK_CONNECTING, &transport->sock_state);
	return kernel_connect(sock, xs_addr(xprt), xprt->addrlen, O_NONBLOCK);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * xs_tcp_setup_socket - create a TCP socket and connect to a remote endpoint
<<<<<<< HEAD
 * @xprt: RPC transport to connect
 * @transport: socket transport to connect
 * @create_sock: function to create a socket of the correct type
=======
 * @work: queued work item
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Invoked by a work queue tasklet.
 */
static void xs_tcp_setup_socket(struct work_struct *work)
{
	struct sock_xprt *transport =
		container_of(work, struct sock_xprt, connect_worker.work);
	struct socket *sock = transport->sock;
	struct rpc_xprt *xprt = &transport->xprt;
<<<<<<< HEAD
	int status = -EIO;

	if (xprt->shutdown)
		goto out;

	current->flags |= PF_FSTRANS;

	if (!sock) {
		clear_bit(XPRT_CONNECTION_ABORT, &xprt->state);
		sock = xs_create_sock(xprt, transport,
				xs_addr(xprt)->sa_family, SOCK_STREAM, IPPROTO_TCP);
		if (IS_ERR(sock)) {
			status = PTR_ERR(sock);
			goto out;
		}
	} else {
		int abort_and_exit;

		abort_and_exit = test_and_clear_bit(XPRT_CONNECTION_ABORT,
				&xprt->state);
		/* "close" the socket, preserving the local port */
		xs_tcp_reuse_connection(transport);

		if (abort_and_exit)
			goto out_eagain;
=======
	int status;
	unsigned int pflags = current->flags;

	if (atomic_read(&xprt->swapper))
		current->flags |= PF_MEMALLOC;

	if (xprt_connected(xprt))
		goto out;
	if (test_and_clear_bit(XPRT_SOCK_CONNECT_SENT,
			       &transport->sock_state) ||
	    !sock) {
		xs_reset_transport(transport);
		sock = xs_create_sock(xprt, transport, xs_addr(xprt)->sa_family,
				      SOCK_STREAM, IPPROTO_TCP, true);
		if (IS_ERR(sock)) {
			xprt_wake_pending_tasks(xprt, PTR_ERR(sock));
			goto out;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	dprintk("RPC:       worker connecting xprt %p via %s to "
				"%s (port %s)\n", xprt,
			xprt->address_strings[RPC_DISPLAY_PROTO],
			xprt->address_strings[RPC_DISPLAY_ADDR],
			xprt->address_strings[RPC_DISPLAY_PORT]);

	status = xs_tcp_finish_connecting(xprt, sock);
<<<<<<< HEAD
=======
	trace_rpc_socket_connect(xprt, sock, status);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dprintk("RPC:       %p connect status %d connected %d sock state %d\n",
			xprt, -status, xprt_connected(xprt),
			sock->sk->sk_state);
	switch (status) {
<<<<<<< HEAD
	default:
		printk("%s: connect returned unhandled error %d\n",
			__func__, status);
	case -EADDRNOTAVAIL:
		/* We're probably in TIME_WAIT. Get rid of existing socket,
		 * and retry
		 */
		xs_tcp_force_close(xprt);
		break;
	case -ECONNREFUSED:
	case -ECONNRESET:
	case -ENETUNREACH:
		/* retry with existing socket, after a delay */
	case 0:
	case -EINPROGRESS:
	case -EALREADY:
		xprt_clear_connecting(xprt);
		current->flags &= ~PF_FSTRANS;
		return;
=======
	case 0:
	case -EINPROGRESS:
		/* SYN_SENT! */
		set_bit(XPRT_SOCK_CONNECT_SENT, &transport->sock_state);
		if (xprt->reestablish_timeout < XS_TCP_INIT_REEST_TO)
			xprt->reestablish_timeout = XS_TCP_INIT_REEST_TO;
		fallthrough;
	case -EALREADY:
		goto out_unlock;
	case -EADDRNOTAVAIL:
		/* Source port number is unavailable. Try a new one! */
		transport->srcport = 0;
		status = -EAGAIN;
		break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case -EINVAL:
		/* Happens, for instance, if the user specified a link
		 * local IPv6 address without a scope-id.
		 */
<<<<<<< HEAD
		goto out;
	}
out_eagain:
	status = -EAGAIN;
out:
	xprt_clear_connecting(xprt);
	xprt_wake_pending_tasks(xprt, status);
	current->flags &= ~PF_FSTRANS;
=======
	case -ECONNREFUSED:
	case -ECONNRESET:
	case -ENETDOWN:
	case -ENETUNREACH:
	case -EHOSTUNREACH:
	case -EADDRINUSE:
	case -ENOBUFS:
		break;
	default:
		printk("%s: connect returned unhandled error %d\n",
			__func__, status);
		status = -EAGAIN;
	}

	/* xs_tcp_force_close() wakes tasks with a fixed error code.
	 * We need to wake them first to ensure the correct error code.
	 */
	xprt_wake_pending_tasks(xprt, status);
	xs_tcp_force_close(xprt);
out:
	xprt_clear_connecting(xprt);
out_unlock:
	xprt_unlock_connect(xprt, transport);
	current_restore_flags(pflags, PF_MEMALLOC);
}

/*
 * Transfer the connected socket to @upper_transport, then mark that
 * xprt CONNECTED.
 */
static int xs_tcp_tls_finish_connecting(struct rpc_xprt *lower_xprt,
					struct sock_xprt *upper_transport)
{
	struct sock_xprt *lower_transport =
			container_of(lower_xprt, struct sock_xprt, xprt);
	struct rpc_xprt *upper_xprt = &upper_transport->xprt;

	if (!upper_transport->inet) {
		struct socket *sock = lower_transport->sock;
		struct sock *sk = sock->sk;

		/* Avoid temporary address, they are bad for long-lived
		 * connections such as NFS mounts.
		 * RFC4941, section 3.6 suggests that:
		 *    Individual applications, which have specific
		 *    knowledge about the normal duration of connections,
		 *    MAY override this as appropriate.
		 */
		if (xs_addr(upper_xprt)->sa_family == PF_INET6)
			ip6_sock_set_addr_preferences(sk, IPV6_PREFER_SRC_PUBLIC);

		xs_tcp_set_socket_timeouts(upper_xprt, sock);
		tcp_sock_set_nodelay(sk);

		lock_sock(sk);

		/* @sk is already connected, so it now has the RPC callbacks.
		 * Reach into @lower_transport to save the original ones.
		 */
		upper_transport->old_data_ready = lower_transport->old_data_ready;
		upper_transport->old_state_change = lower_transport->old_state_change;
		upper_transport->old_write_space = lower_transport->old_write_space;
		upper_transport->old_error_report = lower_transport->old_error_report;
		sk->sk_user_data = upper_xprt;

		/* socket options */
		sock_reset_flag(sk, SOCK_LINGER);

		xprt_clear_connected(upper_xprt);

		upper_transport->sock = sock;
		upper_transport->inet = sk;
		upper_transport->file = lower_transport->file;

		release_sock(sk);

		/* Reset lower_transport before shutting down its clnt */
		mutex_lock(&lower_transport->recv_mutex);
		lower_transport->inet = NULL;
		lower_transport->sock = NULL;
		lower_transport->file = NULL;

		xprt_clear_connected(lower_xprt);
		xs_sock_reset_connection_flags(lower_xprt);
		xs_stream_reset_connect(lower_transport);
		mutex_unlock(&lower_transport->recv_mutex);
	}

	if (!xprt_bound(upper_xprt))
		return -ENOTCONN;

	xs_set_memalloc(upper_xprt);

	if (!xprt_test_and_set_connected(upper_xprt)) {
		upper_xprt->connect_cookie++;
		clear_bit(XPRT_SOCK_CONNECTING, &upper_transport->sock_state);
		xprt_clear_connecting(upper_xprt);

		upper_xprt->stat.connect_count++;
		upper_xprt->stat.connect_time += (long)jiffies -
					   upper_xprt->stat.connect_start;
		xs_run_error_worker(upper_transport, XPRT_SOCK_WAKE_PENDING);
	}
	return 0;
}

/**
 * xs_tls_handshake_done - TLS handshake completion handler
 * @data: address of xprt to wake
 * @status: status of handshake
 * @peerid: serial number of key containing the remote's identity
 *
 */
static void xs_tls_handshake_done(void *data, int status, key_serial_t peerid)
{
	struct rpc_xprt *lower_xprt = data;
	struct sock_xprt *lower_transport =
				container_of(lower_xprt, struct sock_xprt, xprt);

	lower_transport->xprt_err = status ? -EACCES : 0;
	complete(&lower_transport->handshake_done);
	xprt_put(lower_xprt);
}

static int xs_tls_handshake_sync(struct rpc_xprt *lower_xprt, struct xprtsec_parms *xprtsec)
{
	struct sock_xprt *lower_transport =
				container_of(lower_xprt, struct sock_xprt, xprt);
	struct tls_handshake_args args = {
		.ta_sock	= lower_transport->sock,
		.ta_done	= xs_tls_handshake_done,
		.ta_data	= xprt_get(lower_xprt),
		.ta_peername	= lower_xprt->servername,
	};
	struct sock *sk = lower_transport->inet;
	int rc;

	init_completion(&lower_transport->handshake_done);
	set_bit(XPRT_SOCK_IGNORE_RECV, &lower_transport->sock_state);
	lower_transport->xprt_err = -ETIMEDOUT;
	switch (xprtsec->policy) {
	case RPC_XPRTSEC_TLS_ANON:
		rc = tls_client_hello_anon(&args, GFP_KERNEL);
		if (rc)
			goto out_put_xprt;
		break;
	case RPC_XPRTSEC_TLS_X509:
		args.ta_my_cert = xprtsec->cert_serial;
		args.ta_my_privkey = xprtsec->privkey_serial;
		rc = tls_client_hello_x509(&args, GFP_KERNEL);
		if (rc)
			goto out_put_xprt;
		break;
	default:
		rc = -EACCES;
		goto out_put_xprt;
	}

	rc = wait_for_completion_interruptible_timeout(&lower_transport->handshake_done,
						       XS_TLS_HANDSHAKE_TO);
	if (rc <= 0) {
		if (!tls_handshake_cancel(sk)) {
			if (rc == 0)
				rc = -ETIMEDOUT;
			goto out_put_xprt;
		}
	}

	rc = lower_transport->xprt_err;

out:
	xs_stream_reset_connect(lower_transport);
	clear_bit(XPRT_SOCK_IGNORE_RECV, &lower_transport->sock_state);
	return rc;

out_put_xprt:
	xprt_put(lower_xprt);
	goto out;
}

/**
 * xs_tcp_tls_setup_socket - establish a TLS session on a TCP socket
 * @work: queued work item
 *
 * Invoked by a work queue tasklet.
 *
 * For RPC-with-TLS, there is a two-stage connection process.
 *
 * The "upper-layer xprt" is visible to the RPC consumer. Once it has
 * been marked connected, the consumer knows that a TCP connection and
 * a TLS session have been established.
 *
 * A "lower-layer xprt", created in this function, handles the mechanics
 * of connecting the TCP socket, performing the RPC_AUTH_TLS probe, and
 * then driving the TLS handshake. Once all that is complete, the upper
 * layer xprt is marked connected.
 */
static void xs_tcp_tls_setup_socket(struct work_struct *work)
{
	struct sock_xprt *upper_transport =
		container_of(work, struct sock_xprt, connect_worker.work);
	struct rpc_clnt *upper_clnt = upper_transport->clnt;
	struct rpc_xprt *upper_xprt = &upper_transport->xprt;
	struct rpc_create_args args = {
		.net		= upper_xprt->xprt_net,
		.protocol	= upper_xprt->prot,
		.address	= (struct sockaddr *)&upper_xprt->addr,
		.addrsize	= upper_xprt->addrlen,
		.timeout	= upper_clnt->cl_timeout,
		.servername	= upper_xprt->servername,
		.program	= upper_clnt->cl_program,
		.prognumber	= upper_clnt->cl_prog,
		.version	= upper_clnt->cl_vers,
		.authflavor	= RPC_AUTH_TLS,
		.cred		= upper_clnt->cl_cred,
		.xprtsec	= {
			.policy		= RPC_XPRTSEC_NONE,
		},
		.stats		= upper_clnt->cl_stats,
	};
	unsigned int pflags = current->flags;
	struct rpc_clnt *lower_clnt;
	struct rpc_xprt *lower_xprt;
	int status;

	if (atomic_read(&upper_xprt->swapper))
		current->flags |= PF_MEMALLOC;

	xs_stream_start_connect(upper_transport);

	/* This implicitly sends an RPC_AUTH_TLS probe */
	lower_clnt = rpc_create(&args);
	if (IS_ERR(lower_clnt)) {
		trace_rpc_tls_unavailable(upper_clnt, upper_xprt);
		clear_bit(XPRT_SOCK_CONNECTING, &upper_transport->sock_state);
		xprt_clear_connecting(upper_xprt);
		xprt_wake_pending_tasks(upper_xprt, PTR_ERR(lower_clnt));
		xs_run_error_worker(upper_transport, XPRT_SOCK_WAKE_PENDING);
		goto out_unlock;
	}

	/* RPC_AUTH_TLS probe was successful. Try a TLS handshake on
	 * the lower xprt.
	 */
	rcu_read_lock();
	lower_xprt = rcu_dereference(lower_clnt->cl_xprt);
	rcu_read_unlock();

	if (wait_on_bit_lock(&lower_xprt->state, XPRT_LOCKED, TASK_KILLABLE))
		goto out_unlock;

	status = xs_tls_handshake_sync(lower_xprt, &upper_xprt->xprtsec);
	if (status) {
		trace_rpc_tls_not_started(upper_clnt, upper_xprt);
		goto out_close;
	}

	status = xs_tcp_tls_finish_connecting(lower_xprt, upper_transport);
	if (status)
		goto out_close;
	xprt_release_write(lower_xprt, NULL);

	trace_rpc_socket_connect(upper_xprt, upper_transport->sock, 0);
	if (!xprt_test_and_set_connected(upper_xprt)) {
		upper_xprt->connect_cookie++;
		clear_bit(XPRT_SOCK_CONNECTING, &upper_transport->sock_state);
		xprt_clear_connecting(upper_xprt);

		upper_xprt->stat.connect_count++;
		upper_xprt->stat.connect_time += (long)jiffies -
					   upper_xprt->stat.connect_start;
		xs_run_error_worker(upper_transport, XPRT_SOCK_WAKE_PENDING);
	}
	rpc_shutdown_client(lower_clnt);

out_unlock:
	current_restore_flags(pflags, PF_MEMALLOC);
	upper_transport->clnt = NULL;
	xprt_unlock_connect(upper_xprt, upper_transport);
	return;

out_close:
	xprt_release_write(lower_xprt, NULL);
	rpc_shutdown_client(lower_clnt);

	/* xprt_force_disconnect() wakes tasks with a fixed tk_status code.
	 * Wake them first here to ensure they get our tk_status code.
	 */
	xprt_wake_pending_tasks(upper_xprt, status);
	xs_tcp_force_close(upper_xprt);
	xprt_clear_connecting(upper_xprt);
	goto out_unlock;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * xs_connect - connect a socket to a remote endpoint
<<<<<<< HEAD
=======
 * @xprt: pointer to transport structure
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @task: address of RPC task that manages state of connect request
 *
 * TCP: If the remote end dropped the connection, delay reconnecting.
 *
 * UDP socket connects are synchronous, but we use a work queue anyway
 * to guarantee that even unprivileged user processes can set up a
 * socket on a privileged port.
 *
 * If a UDP socket connect fails, the delay behavior here prevents
 * retry floods (hard mounts).
 */
<<<<<<< HEAD
static void xs_connect(struct rpc_task *task)
{
	struct rpc_xprt *xprt = task->tk_xprt;
	struct sock_xprt *transport = container_of(xprt, struct sock_xprt, xprt);

	if (transport->sock != NULL && !RPC_IS_SOFTCONN(task)) {
		dprintk("RPC:       xs_connect delayed xprt %p for %lu "
				"seconds\n",
				xprt, xprt->reestablish_timeout / HZ);
		queue_delayed_work(rpciod_workqueue,
				   &transport->connect_worker,
				   xprt->reestablish_timeout);
		xprt->reestablish_timeout <<= 1;
		if (xprt->reestablish_timeout < XS_TCP_INIT_REEST_TO)
			xprt->reestablish_timeout = XS_TCP_INIT_REEST_TO;
		if (xprt->reestablish_timeout > XS_TCP_MAX_REEST_TO)
			xprt->reestablish_timeout = XS_TCP_MAX_REEST_TO;
	} else {
		dprintk("RPC:       xs_connect scheduled xprt %p\n", xprt);
		queue_delayed_work(rpciod_workqueue,
				   &transport->connect_worker, 0);
	}
}

/**
 * xs_local_print_stats - display AF_LOCAL socket-specifc stats
=======
static void xs_connect(struct rpc_xprt *xprt, struct rpc_task *task)
{
	struct sock_xprt *transport = container_of(xprt, struct sock_xprt, xprt);
	unsigned long delay = 0;

	WARN_ON_ONCE(!xprt_lock_connect(xprt, task, transport));

	if (transport->sock != NULL) {
		dprintk("RPC:       xs_connect delayed xprt %p for %lu "
			"seconds\n", xprt, xprt->reestablish_timeout / HZ);

		delay = xprt_reconnect_delay(xprt);
		xprt_reconnect_backoff(xprt, XS_TCP_INIT_REEST_TO);

	} else
		dprintk("RPC:       xs_connect scheduled xprt %p\n", xprt);

	transport->clnt = task->tk_client;
	queue_delayed_work(xprtiod_workqueue,
			&transport->connect_worker,
			delay);
}

static void xs_wake_disconnect(struct sock_xprt *transport)
{
	if (test_and_clear_bit(XPRT_SOCK_WAKE_DISCONNECT, &transport->sock_state))
		xs_tcp_force_close(&transport->xprt);
}

static void xs_wake_write(struct sock_xprt *transport)
{
	if (test_and_clear_bit(XPRT_SOCK_WAKE_WRITE, &transport->sock_state))
		xprt_write_space(&transport->xprt);
}

static void xs_wake_error(struct sock_xprt *transport)
{
	int sockerr;

	if (!test_and_clear_bit(XPRT_SOCK_WAKE_ERROR, &transport->sock_state))
		return;
	sockerr = xchg(&transport->xprt_err, 0);
	if (sockerr < 0) {
		xprt_wake_pending_tasks(&transport->xprt, sockerr);
		xs_tcp_force_close(&transport->xprt);
	}
}

static void xs_wake_pending(struct sock_xprt *transport)
{
	if (test_and_clear_bit(XPRT_SOCK_WAKE_PENDING, &transport->sock_state))
		xprt_wake_pending_tasks(&transport->xprt, -EAGAIN);
}

static void xs_error_handle(struct work_struct *work)
{
	struct sock_xprt *transport = container_of(work,
			struct sock_xprt, error_worker);

	xs_wake_disconnect(transport);
	xs_wake_write(transport);
	xs_wake_error(transport);
	xs_wake_pending(transport);
}

/**
 * xs_local_print_stats - display AF_LOCAL socket-specific stats
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @xprt: rpc_xprt struct containing statistics
 * @seq: output file
 *
 */
static void xs_local_print_stats(struct rpc_xprt *xprt, struct seq_file *seq)
{
	long idle_time = 0;

	if (xprt_connected(xprt))
		idle_time = (long)(jiffies - xprt->last_used) / HZ;

	seq_printf(seq, "\txprt:\tlocal %lu %lu %lu %ld %lu %lu %lu "
			"%llu %llu %lu %llu %llu\n",
			xprt->stat.bind_count,
			xprt->stat.connect_count,
<<<<<<< HEAD
			xprt->stat.connect_time,
=======
			xprt->stat.connect_time / HZ,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			idle_time,
			xprt->stat.sends,
			xprt->stat.recvs,
			xprt->stat.bad_xids,
			xprt->stat.req_u,
			xprt->stat.bklog_u,
			xprt->stat.max_slots,
			xprt->stat.sending_u,
			xprt->stat.pending_u);
}

/**
<<<<<<< HEAD
 * xs_udp_print_stats - display UDP socket-specifc stats
=======
 * xs_udp_print_stats - display UDP socket-specific stats
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @xprt: rpc_xprt struct containing statistics
 * @seq: output file
 *
 */
static void xs_udp_print_stats(struct rpc_xprt *xprt, struct seq_file *seq)
{
	struct sock_xprt *transport = container_of(xprt, struct sock_xprt, xprt);

	seq_printf(seq, "\txprt:\tudp %u %lu %lu %lu %lu %llu %llu "
			"%lu %llu %llu\n",
			transport->srcport,
			xprt->stat.bind_count,
			xprt->stat.sends,
			xprt->stat.recvs,
			xprt->stat.bad_xids,
			xprt->stat.req_u,
			xprt->stat.bklog_u,
			xprt->stat.max_slots,
			xprt->stat.sending_u,
			xprt->stat.pending_u);
}

/**
<<<<<<< HEAD
 * xs_tcp_print_stats - display TCP socket-specifc stats
=======
 * xs_tcp_print_stats - display TCP socket-specific stats
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @xprt: rpc_xprt struct containing statistics
 * @seq: output file
 *
 */
static void xs_tcp_print_stats(struct rpc_xprt *xprt, struct seq_file *seq)
{
	struct sock_xprt *transport = container_of(xprt, struct sock_xprt, xprt);
	long idle_time = 0;

	if (xprt_connected(xprt))
		idle_time = (long)(jiffies - xprt->last_used) / HZ;

	seq_printf(seq, "\txprt:\ttcp %u %lu %lu %lu %ld %lu %lu %lu "
			"%llu %llu %lu %llu %llu\n",
			transport->srcport,
			xprt->stat.bind_count,
			xprt->stat.connect_count,
<<<<<<< HEAD
			xprt->stat.connect_time,
=======
			xprt->stat.connect_time / HZ,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			idle_time,
			xprt->stat.sends,
			xprt->stat.recvs,
			xprt->stat.bad_xids,
			xprt->stat.req_u,
			xprt->stat.bklog_u,
			xprt->stat.max_slots,
			xprt->stat.sending_u,
			xprt->stat.pending_u);
}

/*
 * Allocate a bunch of pages for a scratch buffer for the rpc code. The reason
 * we allocate pages instead doing a kmalloc like rpc_malloc is because we want
 * to use the server side send routines.
 */
<<<<<<< HEAD
static void *bc_malloc(struct rpc_task *task, size_t size)
{
	struct page *page;
	struct rpc_buffer *buf;

	BUG_ON(size > PAGE_SIZE - sizeof(struct rpc_buffer));
	page = alloc_page(GFP_KERNEL);

	if (!page)
		return NULL;
=======
static int bc_malloc(struct rpc_task *task)
{
	struct rpc_rqst *rqst = task->tk_rqstp;
	size_t size = rqst->rq_callsize;
	struct page *page;
	struct rpc_buffer *buf;

	if (size > PAGE_SIZE - sizeof(struct rpc_buffer)) {
		WARN_ONCE(1, "xprtsock: large bc buffer request (size %zu)\n",
			  size);
		return -EINVAL;
	}

	page = alloc_page(GFP_KERNEL | __GFP_NORETRY | __GFP_NOWARN);
	if (!page)
		return -ENOMEM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	buf = page_address(page);
	buf->len = PAGE_SIZE;

<<<<<<< HEAD
	return buf->data;
=======
	rqst->rq_buffer = buf->data;
	rqst->rq_rbuffer = (char *)rqst->rq_buffer + rqst->rq_callsize;
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Free the space allocated in the bc_alloc routine
 */
<<<<<<< HEAD
static void bc_free(void *buffer)
{
	struct rpc_buffer *buf;

	if (!buffer)
		return;

=======
static void bc_free(struct rpc_task *task)
{
	void *buffer = task->tk_rqstp->rq_buffer;
	struct rpc_buffer *buf;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	buf = container_of(buffer, struct rpc_buffer, data);
	free_page((unsigned long)buf);
}

<<<<<<< HEAD
/*
 * Use the svc_sock to send the callback. Must be called with svsk->sk_mutex
 * held. Borrows heavily from svc_tcp_sendto and xs_tcp_send_request.
 */
static int bc_sendto(struct rpc_rqst *req)
{
	int len;
	struct xdr_buf *xbufp = &req->rq_snd_buf;
	struct rpc_xprt *xprt = req->rq_xprt;
	struct sock_xprt *transport =
				container_of(xprt, struct sock_xprt, xprt);
	struct socket *sock = transport->sock;
	unsigned long headoff;
	unsigned long tailoff;

	xs_encode_stream_record_marker(xbufp);

	tailoff = (unsigned long)xbufp->tail[0].iov_base & ~PAGE_MASK;
	headoff = (unsigned long)xbufp->head[0].iov_base & ~PAGE_MASK;
	len = svc_send_common(sock, xbufp,
			      virt_to_page(xbufp->head[0].iov_base), headoff,
			      xbufp->tail[0].iov_base, tailoff);

	if (len != xbufp->len) {
		printk(KERN_NOTICE "Error sending entire callback!\n");
		len = -EAGAIN;
	}

	return len;
}

/*
 * The send routine. Borrows from svc_send
 */
static int bc_send_request(struct rpc_task *task)
{
	struct rpc_rqst *req = task->tk_rqstp;
	struct svc_xprt	*xprt;
	struct svc_sock         *svsk;
	u32                     len;

	dprintk("sending request with xid: %08x\n", ntohl(req->rq_xid));
=======
static int bc_sendto(struct rpc_rqst *req)
{
	struct xdr_buf *xdr = &req->rq_snd_buf;
	struct sock_xprt *transport =
			container_of(req->rq_xprt, struct sock_xprt, xprt);
	struct msghdr msg = {
		.msg_flags	= 0,
	};
	rpc_fraghdr marker = cpu_to_be32(RPC_LAST_STREAM_FRAGMENT |
					 (u32)xdr->len);
	unsigned int sent = 0;
	int err;

	req->rq_xtime = ktime_get();
	err = xdr_alloc_bvec(xdr, rpc_task_gfp_mask());
	if (err < 0)
		return err;
	err = xprt_sock_sendmsg(transport->sock, &msg, xdr, 0, marker, &sent);
	xdr_free_bvec(xdr);
	if (err < 0 || sent != (xdr->len + sizeof(marker)))
		return -EAGAIN;
	return sent;
}

/**
 * bc_send_request - Send a backchannel Call on a TCP socket
 * @req: rpc_rqst containing Call message to be sent
 *
 * xpt_mutex ensures @rqstp's whole message is written to the socket
 * without interruption.
 *
 * Return values:
 *   %0 if the message was sent successfully
 *   %ENOTCONN if the message was not sent
 */
static int bc_send_request(struct rpc_rqst *req)
{
	struct svc_xprt	*xprt;
	int len;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Get the server socket associated with this callback xprt
	 */
	xprt = req->rq_xprt->bc_xprt;
<<<<<<< HEAD
	svsk = container_of(xprt, struct svc_sock, sk_xprt);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Grab the mutex to serialize data as the connection is shared
	 * with the fore channel
	 */
<<<<<<< HEAD
	if (!mutex_trylock(&xprt->xpt_mutex)) {
		rpc_sleep_on(&xprt->xpt_bc_pending, task, NULL);
		if (!mutex_trylock(&xprt->xpt_mutex))
			return -EAGAIN;
		rpc_wake_up_queued_task(&xprt->xpt_bc_pending, task);
	}
=======
	mutex_lock(&xprt->xpt_mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (test_bit(XPT_DEAD, &xprt->xpt_flags))
		len = -ENOTCONN;
	else
		len = bc_sendto(req);
	mutex_unlock(&xprt->xpt_mutex);

	if (len > 0)
		len = 0;

	return len;
}

<<<<<<< HEAD
/*
 * The close routine. Since this is client initiated, we do nothing
 */

static void bc_close(struct rpc_xprt *xprt)
{
}

/*
 * The xprt destroy routine. Again, because this connection is client
 * initiated, we do nothing
 */

static void bc_destroy(struct rpc_xprt *xprt)
{
}

static struct rpc_xprt_ops xs_local_ops = {
	.reserve_xprt		= xprt_reserve_xprt,
	.release_xprt		= xs_tcp_release_xprt,
	.alloc_slot		= xprt_alloc_slot,
	.rpcbind		= xs_local_rpcbind,
	.set_port		= xs_local_set_port,
	.connect		= xs_connect,
	.buf_alloc		= rpc_malloc,
	.buf_free		= rpc_free,
	.send_request		= xs_local_send_request,
	.set_retrans_timeout	= xprt_set_retrans_timeout_def,
	.close			= xs_close,
	.destroy		= xs_destroy,
	.print_stats		= xs_local_print_stats,
};

static struct rpc_xprt_ops xs_udp_ops = {
=======
static void bc_close(struct rpc_xprt *xprt)
{
	xprt_disconnect_done(xprt);
}

static void bc_destroy(struct rpc_xprt *xprt)
{
	dprintk("RPC:       bc_destroy xprt %p\n", xprt);

	xs_xprt_free(xprt);
	module_put(THIS_MODULE);
}

static const struct rpc_xprt_ops xs_local_ops = {
	.reserve_xprt		= xprt_reserve_xprt,
	.release_xprt		= xprt_release_xprt,
	.alloc_slot		= xprt_alloc_slot,
	.free_slot		= xprt_free_slot,
	.rpcbind		= xs_local_rpcbind,
	.set_port		= xs_local_set_port,
	.connect		= xs_local_connect,
	.buf_alloc		= rpc_malloc,
	.buf_free		= rpc_free,
	.prepare_request	= xs_stream_prepare_request,
	.send_request		= xs_local_send_request,
	.abort_send_request	= xs_stream_abort_send_request,
	.wait_for_reply_request	= xprt_wait_for_reply_request_def,
	.close			= xs_close,
	.destroy		= xs_destroy,
	.print_stats		= xs_local_print_stats,
	.enable_swap		= xs_enable_swap,
	.disable_swap		= xs_disable_swap,
};

static const struct rpc_xprt_ops xs_udp_ops = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.set_buffer_size	= xs_udp_set_buffer_size,
	.reserve_xprt		= xprt_reserve_xprt_cong,
	.release_xprt		= xprt_release_xprt_cong,
	.alloc_slot		= xprt_alloc_slot,
<<<<<<< HEAD
	.rpcbind		= rpcb_getport_async,
	.set_port		= xs_set_port,
	.connect		= xs_connect,
	.buf_alloc		= rpc_malloc,
	.buf_free		= rpc_free,
	.send_request		= xs_udp_send_request,
	.set_retrans_timeout	= xprt_set_retrans_timeout_rtt,
=======
	.free_slot		= xprt_free_slot,
	.rpcbind		= rpcb_getport_async,
	.set_port		= xs_set_port,
	.connect		= xs_connect,
	.get_srcaddr		= xs_sock_srcaddr,
	.get_srcport		= xs_sock_srcport,
	.buf_alloc		= rpc_malloc,
	.buf_free		= rpc_free,
	.send_request		= xs_udp_send_request,
	.wait_for_reply_request	= xprt_wait_for_reply_request_rtt,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.timer			= xs_udp_timer,
	.release_request	= xprt_release_rqst_cong,
	.close			= xs_close,
	.destroy		= xs_destroy,
	.print_stats		= xs_udp_print_stats,
<<<<<<< HEAD
};

static struct rpc_xprt_ops xs_tcp_ops = {
	.reserve_xprt		= xprt_reserve_xprt,
	.release_xprt		= xs_tcp_release_xprt,
	.alloc_slot		= xprt_lock_and_alloc_slot,
	.rpcbind		= rpcb_getport_async,
	.set_port		= xs_set_port,
	.connect		= xs_connect,
	.buf_alloc		= rpc_malloc,
	.buf_free		= rpc_free,
	.send_request		= xs_tcp_send_request,
	.set_retrans_timeout	= xprt_set_retrans_timeout_def,
	.close			= xs_tcp_close,
	.destroy		= xs_destroy,
	.print_stats		= xs_tcp_print_stats,
=======
	.enable_swap		= xs_enable_swap,
	.disable_swap		= xs_disable_swap,
	.inject_disconnect	= xs_inject_disconnect,
};

static const struct rpc_xprt_ops xs_tcp_ops = {
	.reserve_xprt		= xprt_reserve_xprt,
	.release_xprt		= xprt_release_xprt,
	.alloc_slot		= xprt_alloc_slot,
	.free_slot		= xprt_free_slot,
	.rpcbind		= rpcb_getport_async,
	.set_port		= xs_set_port,
	.connect		= xs_connect,
	.get_srcaddr		= xs_sock_srcaddr,
	.get_srcport		= xs_sock_srcport,
	.buf_alloc		= rpc_malloc,
	.buf_free		= rpc_free,
	.prepare_request	= xs_stream_prepare_request,
	.send_request		= xs_tcp_send_request,
	.abort_send_request	= xs_stream_abort_send_request,
	.wait_for_reply_request	= xprt_wait_for_reply_request_def,
	.close			= xs_tcp_shutdown,
	.destroy		= xs_destroy,
	.set_connect_timeout	= xs_tcp_set_connect_timeout,
	.print_stats		= xs_tcp_print_stats,
	.enable_swap		= xs_enable_swap,
	.disable_swap		= xs_disable_swap,
	.inject_disconnect	= xs_inject_disconnect,
#ifdef CONFIG_SUNRPC_BACKCHANNEL
	.bc_setup		= xprt_setup_bc,
	.bc_maxpayload		= xs_tcp_bc_maxpayload,
	.bc_num_slots		= xprt_bc_max_slots,
	.bc_free_rqst		= xprt_free_bc_rqst,
	.bc_destroy		= xprt_destroy_bc,
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/*
 * The rpc_xprt_ops for the server backchannel
 */

<<<<<<< HEAD
static struct rpc_xprt_ops bc_tcp_ops = {
	.reserve_xprt		= xprt_reserve_xprt,
	.release_xprt		= xprt_release_xprt,
	.alloc_slot		= xprt_alloc_slot,
	.rpcbind		= xs_local_rpcbind,
	.buf_alloc		= bc_malloc,
	.buf_free		= bc_free,
	.send_request		= bc_send_request,
	.set_retrans_timeout	= xprt_set_retrans_timeout_def,
	.close			= bc_close,
	.destroy		= bc_destroy,
	.print_stats		= xs_tcp_print_stats,
=======
static const struct rpc_xprt_ops bc_tcp_ops = {
	.reserve_xprt		= xprt_reserve_xprt,
	.release_xprt		= xprt_release_xprt,
	.alloc_slot		= xprt_alloc_slot,
	.free_slot		= xprt_free_slot,
	.buf_alloc		= bc_malloc,
	.buf_free		= bc_free,
	.send_request		= bc_send_request,
	.wait_for_reply_request	= xprt_wait_for_reply_request_def,
	.close			= bc_close,
	.destroy		= bc_destroy,
	.print_stats		= xs_tcp_print_stats,
	.enable_swap		= xs_enable_swap,
	.disable_swap		= xs_disable_swap,
	.inject_disconnect	= xs_inject_disconnect,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static int xs_init_anyaddr(const int family, struct sockaddr *sap)
{
	static const struct sockaddr_in sin = {
		.sin_family		= AF_INET,
		.sin_addr.s_addr	= htonl(INADDR_ANY),
	};
	static const struct sockaddr_in6 sin6 = {
		.sin6_family		= AF_INET6,
		.sin6_addr		= IN6ADDR_ANY_INIT,
	};

	switch (family) {
	case AF_LOCAL:
		break;
	case AF_INET:
		memcpy(sap, &sin, sizeof(sin));
		break;
	case AF_INET6:
		memcpy(sap, &sin6, sizeof(sin6));
		break;
	default:
		dprintk("RPC:       %s: Bad address family\n", __func__);
		return -EAFNOSUPPORT;
	}
	return 0;
}

static struct rpc_xprt *xs_setup_xprt(struct xprt_create *args,
				      unsigned int slot_table_size,
				      unsigned int max_slot_table_size)
{
	struct rpc_xprt *xprt;
	struct sock_xprt *new;

	if (args->addrlen > sizeof(xprt->addr)) {
		dprintk("RPC:       xs_setup_xprt: address too large\n");
		return ERR_PTR(-EBADF);
	}

	xprt = xprt_alloc(args->net, sizeof(*new), slot_table_size,
			max_slot_table_size);
	if (xprt == NULL) {
		dprintk("RPC:       xs_setup_xprt: couldn't allocate "
				"rpc_xprt\n");
		return ERR_PTR(-ENOMEM);
	}

	new = container_of(xprt, struct sock_xprt, xprt);
<<<<<<< HEAD
=======
	mutex_init(&new->recv_mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	memcpy(&xprt->addr, args->dstaddr, args->addrlen);
	xprt->addrlen = args->addrlen;
	if (args->srcaddr)
		memcpy(&new->srcaddr, args->srcaddr, args->addrlen);
	else {
		int err;
		err = xs_init_anyaddr(args->dstaddr->sa_family,
					(struct sockaddr *)&new->srcaddr);
		if (err != 0) {
			xprt_free(xprt);
			return ERR_PTR(err);
		}
	}

	return xprt;
}

static const struct rpc_timeout xs_local_default_timeout = {
	.to_initval = 10 * HZ,
	.to_maxval = 10 * HZ,
	.to_retries = 2,
};

/**
 * xs_setup_local - Set up transport to use an AF_LOCAL socket
 * @args: rpc transport creation arguments
 *
 * AF_LOCAL is a "tpi_cots_ord" transport, just like TCP
 */
static struct rpc_xprt *xs_setup_local(struct xprt_create *args)
{
	struct sockaddr_un *sun = (struct sockaddr_un *)args->dstaddr;
	struct sock_xprt *transport;
	struct rpc_xprt *xprt;
	struct rpc_xprt *ret;

	xprt = xs_setup_xprt(args, xprt_tcp_slot_table_entries,
			xprt_max_tcp_slot_table_entries);
	if (IS_ERR(xprt))
		return xprt;
	transport = container_of(xprt, struct sock_xprt, xprt);

	xprt->prot = 0;
<<<<<<< HEAD
	xprt->tsh_size = sizeof(rpc_fraghdr) / sizeof(u32);
=======
	xprt->xprt_class = &xs_local_transport;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	xprt->max_payload = RPC_MAX_FRAGMENT_SIZE;

	xprt->bind_timeout = XS_BIND_TO;
	xprt->reestablish_timeout = XS_TCP_INIT_REEST_TO;
	xprt->idle_timeout = XS_IDLE_DISC_TO;

	xprt->ops = &xs_local_ops;
	xprt->timeout = &xs_local_default_timeout;

<<<<<<< HEAD
	switch (sun->sun_family) {
	case AF_LOCAL:
		if (sun->sun_path[0] != '/') {
=======
	INIT_WORK(&transport->recv_worker, xs_stream_data_receive_workfn);
	INIT_WORK(&transport->error_worker, xs_error_handle);
	INIT_DELAYED_WORK(&transport->connect_worker, xs_dummy_setup_socket);

	switch (sun->sun_family) {
	case AF_LOCAL:
		if (sun->sun_path[0] != '/' && sun->sun_path[0] != '\0') {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			dprintk("RPC:       bad AF_LOCAL address: %s\n",
					sun->sun_path);
			ret = ERR_PTR(-EINVAL);
			goto out_err;
		}
		xprt_set_bound(xprt);
<<<<<<< HEAD
		INIT_DELAYED_WORK(&transport->connect_worker,
					xs_local_setup_socket);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		xs_format_peer_addresses(xprt, "local", RPCBIND_NETID_LOCAL);
		break;
	default:
		ret = ERR_PTR(-EAFNOSUPPORT);
		goto out_err;
	}

	dprintk("RPC:       set up xprt to %s via AF_LOCAL\n",
			xprt->address_strings[RPC_DISPLAY_ADDR]);

	if (try_module_get(THIS_MODULE))
		return xprt;
	ret = ERR_PTR(-EINVAL);
out_err:
<<<<<<< HEAD
	xprt_free(xprt);
=======
	xs_xprt_free(xprt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

static const struct rpc_timeout xs_udp_default_timeout = {
	.to_initval = 5 * HZ,
	.to_maxval = 30 * HZ,
	.to_increment = 5 * HZ,
	.to_retries = 5,
};

/**
 * xs_setup_udp - Set up transport to use a UDP socket
 * @args: rpc transport creation arguments
 *
 */
static struct rpc_xprt *xs_setup_udp(struct xprt_create *args)
{
	struct sockaddr *addr = args->dstaddr;
	struct rpc_xprt *xprt;
	struct sock_xprt *transport;
	struct rpc_xprt *ret;

	xprt = xs_setup_xprt(args, xprt_udp_slot_table_entries,
			xprt_udp_slot_table_entries);
	if (IS_ERR(xprt))
		return xprt;
	transport = container_of(xprt, struct sock_xprt, xprt);

	xprt->prot = IPPROTO_UDP;
<<<<<<< HEAD
	xprt->tsh_size = 0;
=======
	xprt->xprt_class = &xs_udp_transport;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* XXX: header size can vary due to auth type, IPv6, etc. */
	xprt->max_payload = (1U << 16) - (MAX_HEADER << 3);

	xprt->bind_timeout = XS_BIND_TO;
	xprt->reestablish_timeout = XS_UDP_REEST_TO;
	xprt->idle_timeout = XS_IDLE_DISC_TO;

	xprt->ops = &xs_udp_ops;

	xprt->timeout = &xs_udp_default_timeout;

<<<<<<< HEAD
=======
	INIT_WORK(&transport->recv_worker, xs_udp_data_receive_workfn);
	INIT_WORK(&transport->error_worker, xs_error_handle);
	INIT_DELAYED_WORK(&transport->connect_worker, xs_udp_setup_socket);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	switch (addr->sa_family) {
	case AF_INET:
		if (((struct sockaddr_in *)addr)->sin_port != htons(0))
			xprt_set_bound(xprt);

<<<<<<< HEAD
		INIT_DELAYED_WORK(&transport->connect_worker,
					xs_udp_setup_socket);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		xs_format_peer_addresses(xprt, "udp", RPCBIND_NETID_UDP);
		break;
	case AF_INET6:
		if (((struct sockaddr_in6 *)addr)->sin6_port != htons(0))
			xprt_set_bound(xprt);

<<<<<<< HEAD
		INIT_DELAYED_WORK(&transport->connect_worker,
					xs_udp_setup_socket);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		xs_format_peer_addresses(xprt, "udp", RPCBIND_NETID_UDP6);
		break;
	default:
		ret = ERR_PTR(-EAFNOSUPPORT);
		goto out_err;
	}

	if (xprt_bound(xprt))
		dprintk("RPC:       set up xprt to %s (port %s) via %s\n",
				xprt->address_strings[RPC_DISPLAY_ADDR],
				xprt->address_strings[RPC_DISPLAY_PORT],
				xprt->address_strings[RPC_DISPLAY_PROTO]);
	else
		dprintk("RPC:       set up xprt to %s (autobind) via %s\n",
				xprt->address_strings[RPC_DISPLAY_ADDR],
				xprt->address_strings[RPC_DISPLAY_PROTO]);

	if (try_module_get(THIS_MODULE))
		return xprt;
	ret = ERR_PTR(-EINVAL);
out_err:
<<<<<<< HEAD
	xprt_free(xprt);
=======
	xs_xprt_free(xprt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

static const struct rpc_timeout xs_tcp_default_timeout = {
	.to_initval = 60 * HZ,
	.to_maxval = 60 * HZ,
	.to_retries = 2,
};

/**
 * xs_setup_tcp - Set up transport to use a TCP socket
 * @args: rpc transport creation arguments
 *
 */
static struct rpc_xprt *xs_setup_tcp(struct xprt_create *args)
{
	struct sockaddr *addr = args->dstaddr;
	struct rpc_xprt *xprt;
	struct sock_xprt *transport;
	struct rpc_xprt *ret;
<<<<<<< HEAD

	xprt = xs_setup_xprt(args, xprt_tcp_slot_table_entries,
			xprt_max_tcp_slot_table_entries);
=======
	unsigned int max_slot_table_size = xprt_max_tcp_slot_table_entries;

	if (args->flags & XPRT_CREATE_INFINITE_SLOTS)
		max_slot_table_size = RPC_MAX_SLOT_TABLE_LIMIT;

	xprt = xs_setup_xprt(args, xprt_tcp_slot_table_entries,
			max_slot_table_size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (IS_ERR(xprt))
		return xprt;
	transport = container_of(xprt, struct sock_xprt, xprt);

	xprt->prot = IPPROTO_TCP;
<<<<<<< HEAD
	xprt->tsh_size = sizeof(rpc_fraghdr) / sizeof(u32);
=======
	xprt->xprt_class = &xs_tcp_transport;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	xprt->max_payload = RPC_MAX_FRAGMENT_SIZE;

	xprt->bind_timeout = XS_BIND_TO;
	xprt->reestablish_timeout = XS_TCP_INIT_REEST_TO;
	xprt->idle_timeout = XS_IDLE_DISC_TO;

	xprt->ops = &xs_tcp_ops;
	xprt->timeout = &xs_tcp_default_timeout;

<<<<<<< HEAD
=======
	xprt->max_reconnect_timeout = xprt->timeout->to_maxval;
	if (args->reconnect_timeout)
		xprt->max_reconnect_timeout = args->reconnect_timeout;

	xprt->connect_timeout = xprt->timeout->to_initval *
		(xprt->timeout->to_retries + 1);
	if (args->connect_timeout)
		xs_tcp_do_set_connect_timeout(xprt, args->connect_timeout);

	INIT_WORK(&transport->recv_worker, xs_stream_data_receive_workfn);
	INIT_WORK(&transport->error_worker, xs_error_handle);
	INIT_DELAYED_WORK(&transport->connect_worker, xs_tcp_setup_socket);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	switch (addr->sa_family) {
	case AF_INET:
		if (((struct sockaddr_in *)addr)->sin_port != htons(0))
			xprt_set_bound(xprt);

<<<<<<< HEAD
		INIT_DELAYED_WORK(&transport->connect_worker,
					xs_tcp_setup_socket);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		xs_format_peer_addresses(xprt, "tcp", RPCBIND_NETID_TCP);
		break;
	case AF_INET6:
		if (((struct sockaddr_in6 *)addr)->sin6_port != htons(0))
			xprt_set_bound(xprt);

<<<<<<< HEAD
		INIT_DELAYED_WORK(&transport->connect_worker,
					xs_tcp_setup_socket);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		xs_format_peer_addresses(xprt, "tcp", RPCBIND_NETID_TCP6);
		break;
	default:
		ret = ERR_PTR(-EAFNOSUPPORT);
		goto out_err;
	}

	if (xprt_bound(xprt))
		dprintk("RPC:       set up xprt to %s (port %s) via %s\n",
				xprt->address_strings[RPC_DISPLAY_ADDR],
				xprt->address_strings[RPC_DISPLAY_PORT],
				xprt->address_strings[RPC_DISPLAY_PROTO]);
	else
		dprintk("RPC:       set up xprt to %s (autobind) via %s\n",
				xprt->address_strings[RPC_DISPLAY_ADDR],
				xprt->address_strings[RPC_DISPLAY_PROTO]);

<<<<<<< HEAD
=======
	if (try_module_get(THIS_MODULE))
		return xprt;
	ret = ERR_PTR(-EINVAL);
out_err:
	xs_xprt_free(xprt);
	return ret;
}

/**
 * xs_setup_tcp_tls - Set up transport to use a TCP with TLS
 * @args: rpc transport creation arguments
 *
 */
static struct rpc_xprt *xs_setup_tcp_tls(struct xprt_create *args)
{
	struct sockaddr *addr = args->dstaddr;
	struct rpc_xprt *xprt;
	struct sock_xprt *transport;
	struct rpc_xprt *ret;
	unsigned int max_slot_table_size = xprt_max_tcp_slot_table_entries;

	if (args->flags & XPRT_CREATE_INFINITE_SLOTS)
		max_slot_table_size = RPC_MAX_SLOT_TABLE_LIMIT;

	xprt = xs_setup_xprt(args, xprt_tcp_slot_table_entries,
			     max_slot_table_size);
	if (IS_ERR(xprt))
		return xprt;
	transport = container_of(xprt, struct sock_xprt, xprt);

	xprt->prot = IPPROTO_TCP;
	xprt->xprt_class = &xs_tcp_transport;
	xprt->max_payload = RPC_MAX_FRAGMENT_SIZE;

	xprt->bind_timeout = XS_BIND_TO;
	xprt->reestablish_timeout = XS_TCP_INIT_REEST_TO;
	xprt->idle_timeout = XS_IDLE_DISC_TO;

	xprt->ops = &xs_tcp_ops;
	xprt->timeout = &xs_tcp_default_timeout;

	xprt->max_reconnect_timeout = xprt->timeout->to_maxval;
	xprt->connect_timeout = xprt->timeout->to_initval *
		(xprt->timeout->to_retries + 1);

	INIT_WORK(&transport->recv_worker, xs_stream_data_receive_workfn);
	INIT_WORK(&transport->error_worker, xs_error_handle);

	switch (args->xprtsec.policy) {
	case RPC_XPRTSEC_TLS_ANON:
	case RPC_XPRTSEC_TLS_X509:
		xprt->xprtsec = args->xprtsec;
		INIT_DELAYED_WORK(&transport->connect_worker,
				  xs_tcp_tls_setup_socket);
		break;
	default:
		ret = ERR_PTR(-EACCES);
		goto out_err;
	}

	switch (addr->sa_family) {
	case AF_INET:
		if (((struct sockaddr_in *)addr)->sin_port != htons(0))
			xprt_set_bound(xprt);

		xs_format_peer_addresses(xprt, "tcp", RPCBIND_NETID_TCP);
		break;
	case AF_INET6:
		if (((struct sockaddr_in6 *)addr)->sin6_port != htons(0))
			xprt_set_bound(xprt);

		xs_format_peer_addresses(xprt, "tcp", RPCBIND_NETID_TCP6);
		break;
	default:
		ret = ERR_PTR(-EAFNOSUPPORT);
		goto out_err;
	}

	if (xprt_bound(xprt))
		dprintk("RPC:       set up xprt to %s (port %s) via %s\n",
			xprt->address_strings[RPC_DISPLAY_ADDR],
			xprt->address_strings[RPC_DISPLAY_PORT],
			xprt->address_strings[RPC_DISPLAY_PROTO]);
	else
		dprintk("RPC:       set up xprt to %s (autobind) via %s\n",
			xprt->address_strings[RPC_DISPLAY_ADDR],
			xprt->address_strings[RPC_DISPLAY_PROTO]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (try_module_get(THIS_MODULE))
		return xprt;
	ret = ERR_PTR(-EINVAL);
out_err:
<<<<<<< HEAD
	xprt_free(xprt);
=======
	xs_xprt_free(xprt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

/**
 * xs_setup_bc_tcp - Set up transport to use a TCP backchannel socket
 * @args: rpc transport creation arguments
 *
 */
static struct rpc_xprt *xs_setup_bc_tcp(struct xprt_create *args)
{
	struct sockaddr *addr = args->dstaddr;
	struct rpc_xprt *xprt;
	struct sock_xprt *transport;
	struct svc_sock *bc_sock;
	struct rpc_xprt *ret;

<<<<<<< HEAD
	if (args->bc_xprt->xpt_bc_xprt) {
		/*
		 * This server connection already has a backchannel
		 * export; we can't create a new one, as we wouldn't be
		 * able to match replies based on xid any more.  So,
		 * reuse the already-existing one:
		 */
		 return args->bc_xprt->xpt_bc_xprt;
	}
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	xprt = xs_setup_xprt(args, xprt_tcp_slot_table_entries,
			xprt_tcp_slot_table_entries);
	if (IS_ERR(xprt))
		return xprt;
	transport = container_of(xprt, struct sock_xprt, xprt);

	xprt->prot = IPPROTO_TCP;
<<<<<<< HEAD
	xprt->tsh_size = sizeof(rpc_fraghdr) / sizeof(u32);
=======
	xprt->xprt_class = &xs_bc_tcp_transport;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	xprt->max_payload = RPC_MAX_FRAGMENT_SIZE;
	xprt->timeout = &xs_tcp_default_timeout;

	/* backchannel */
	xprt_set_bound(xprt);
	xprt->bind_timeout = 0;
	xprt->reestablish_timeout = 0;
	xprt->idle_timeout = 0;

	xprt->ops = &bc_tcp_ops;

	switch (addr->sa_family) {
	case AF_INET:
		xs_format_peer_addresses(xprt, "tcp",
					 RPCBIND_NETID_TCP);
		break;
	case AF_INET6:
		xs_format_peer_addresses(xprt, "tcp",
				   RPCBIND_NETID_TCP6);
		break;
	default:
		ret = ERR_PTR(-EAFNOSUPPORT);
		goto out_err;
	}

	dprintk("RPC:       set up xprt to %s (port %s) via %s\n",
			xprt->address_strings[RPC_DISPLAY_ADDR],
			xprt->address_strings[RPC_DISPLAY_PORT],
			xprt->address_strings[RPC_DISPLAY_PROTO]);

	/*
	 * Once we've associated a backchannel xprt with a connection,
<<<<<<< HEAD
	 * we want to keep it around as long as long as the connection
	 * lasts, in case we need to start using it for a backchannel
	 * again; this reference won't be dropped until bc_xprt is
	 * destroyed.
=======
	 * we want to keep it around as long as the connection lasts,
	 * in case we need to start using it for a backchannel again;
	 * this reference won't be dropped until bc_xprt is destroyed.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 */
	xprt_get(xprt);
	args->bc_xprt->xpt_bc_xprt = xprt;
	xprt->bc_xprt = args->bc_xprt;
	bc_sock = container_of(args->bc_xprt, struct svc_sock, sk_xprt);
	transport->sock = bc_sock->sk_sock;
	transport->inet = bc_sock->sk_sk;

	/*
	 * Since we don't want connections for the backchannel, we set
	 * the xprt status to connected
	 */
	xprt_set_connected(xprt);

<<<<<<< HEAD

	if (try_module_get(THIS_MODULE))
		return xprt;
	xprt_put(xprt);
	ret = ERR_PTR(-EINVAL);
out_err:
	xprt_free(xprt);
=======
	if (try_module_get(THIS_MODULE))
		return xprt;

	args->bc_xprt->xpt_bc_xprt = NULL;
	args->bc_xprt->xpt_bc_xps = NULL;
	xprt_put(xprt);
	ret = ERR_PTR(-EINVAL);
out_err:
	xs_xprt_free(xprt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

static struct xprt_class	xs_local_transport = {
	.list		= LIST_HEAD_INIT(xs_local_transport.list),
	.name		= "named UNIX socket",
	.owner		= THIS_MODULE,
	.ident		= XPRT_TRANSPORT_LOCAL,
	.setup		= xs_setup_local,
<<<<<<< HEAD
=======
	.netid		= { "" },
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static struct xprt_class	xs_udp_transport = {
	.list		= LIST_HEAD_INIT(xs_udp_transport.list),
	.name		= "udp",
	.owner		= THIS_MODULE,
	.ident		= XPRT_TRANSPORT_UDP,
	.setup		= xs_setup_udp,
<<<<<<< HEAD
=======
	.netid		= { "udp", "udp6", "" },
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static struct xprt_class	xs_tcp_transport = {
	.list		= LIST_HEAD_INIT(xs_tcp_transport.list),
	.name		= "tcp",
	.owner		= THIS_MODULE,
	.ident		= XPRT_TRANSPORT_TCP,
	.setup		= xs_setup_tcp,
<<<<<<< HEAD
=======
	.netid		= { "tcp", "tcp6", "" },
};

static struct xprt_class	xs_tcp_tls_transport = {
	.list		= LIST_HEAD_INIT(xs_tcp_tls_transport.list),
	.name		= "tcp-with-tls",
	.owner		= THIS_MODULE,
	.ident		= XPRT_TRANSPORT_TCP_TLS,
	.setup		= xs_setup_tcp_tls,
	.netid		= { "tcp", "tcp6", "" },
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static struct xprt_class	xs_bc_tcp_transport = {
	.list		= LIST_HEAD_INIT(xs_bc_tcp_transport.list),
	.name		= "tcp NFSv4.1 backchannel",
	.owner		= THIS_MODULE,
	.ident		= XPRT_TRANSPORT_BC_TCP,
	.setup		= xs_setup_bc_tcp,
<<<<<<< HEAD
=======
	.netid		= { "" },
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/**
 * init_socket_xprt - set up xprtsock's sysctls, register with RPC client
 *
 */
int init_socket_xprt(void)
{
<<<<<<< HEAD
#ifdef RPC_DEBUG
	if (!sunrpc_table_header)
		sunrpc_table_header = register_sysctl_table(sunrpc_table);
#endif
=======
	if (!sunrpc_table_header)
		sunrpc_table_header = register_sysctl("sunrpc", xs_tunables_table);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	xprt_register_transport(&xs_local_transport);
	xprt_register_transport(&xs_udp_transport);
	xprt_register_transport(&xs_tcp_transport);
<<<<<<< HEAD
=======
	xprt_register_transport(&xs_tcp_tls_transport);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	xprt_register_transport(&xs_bc_tcp_transport);

	return 0;
}

/**
 * cleanup_socket_xprt - remove xprtsock's sysctls, unregister
 *
 */
void cleanup_socket_xprt(void)
{
<<<<<<< HEAD
#ifdef RPC_DEBUG
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (sunrpc_table_header) {
		unregister_sysctl_table(sunrpc_table_header);
		sunrpc_table_header = NULL;
	}
<<<<<<< HEAD
#endif
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	xprt_unregister_transport(&xs_local_transport);
	xprt_unregister_transport(&xs_udp_transport);
	xprt_unregister_transport(&xs_tcp_transport);
<<<<<<< HEAD
	xprt_unregister_transport(&xs_bc_tcp_transport);
}

static int param_set_uint_minmax(const char *val,
		const struct kernel_param *kp,
		unsigned int min, unsigned int max)
{
	unsigned long num;
	int ret;

	if (!val)
		return -EINVAL;
	ret = strict_strtoul(val, 0, &num);
	if (ret == -EINVAL || num < min || num > max)
		return -EINVAL;
	*((unsigned int *)kp->arg) = num;
	return 0;
}

=======
	xprt_unregister_transport(&xs_tcp_tls_transport);
	xprt_unregister_transport(&xs_bc_tcp_transport);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int param_set_portnr(const char *val, const struct kernel_param *kp)
{
	return param_set_uint_minmax(val, kp,
			RPC_MIN_RESVPORT,
			RPC_MAX_RESVPORT);
}

<<<<<<< HEAD
static struct kernel_param_ops param_ops_portnr = {
=======
static const struct kernel_param_ops param_ops_portnr = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.set = param_set_portnr,
	.get = param_get_uint,
};

#define param_check_portnr(name, p) \
	__param_check(name, p, unsigned int);

module_param_named(min_resvport, xprt_min_resvport, portnr, 0644);
module_param_named(max_resvport, xprt_max_resvport, portnr, 0644);

static int param_set_slot_table_size(const char *val,
				     const struct kernel_param *kp)
{
	return param_set_uint_minmax(val, kp,
			RPC_MIN_SLOT_TABLE,
			RPC_MAX_SLOT_TABLE);
}

<<<<<<< HEAD
static struct kernel_param_ops param_ops_slot_table_size = {
=======
static const struct kernel_param_ops param_ops_slot_table_size = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.set = param_set_slot_table_size,
	.get = param_get_uint,
};

#define param_check_slot_table_size(name, p) \
	__param_check(name, p, unsigned int);

static int param_set_max_slot_table_size(const char *val,
				     const struct kernel_param *kp)
{
	return param_set_uint_minmax(val, kp,
			RPC_MIN_SLOT_TABLE,
			RPC_MAX_SLOT_TABLE_LIMIT);
}

<<<<<<< HEAD
static struct kernel_param_ops param_ops_max_slot_table_size = {
=======
static const struct kernel_param_ops param_ops_max_slot_table_size = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.set = param_set_max_slot_table_size,
	.get = param_get_uint,
};

#define param_check_max_slot_table_size(name, p) \
	__param_check(name, p, unsigned int);

module_param_named(tcp_slot_table_entries, xprt_tcp_slot_table_entries,
		   slot_table_size, 0644);
module_param_named(tcp_max_slot_table_entries, xprt_max_tcp_slot_table_entries,
		   max_slot_table_size, 0644);
module_param_named(udp_slot_table_entries, xprt_udp_slot_table_entries,
		   slot_table_size, 0644);
<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
