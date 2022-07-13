/*
<<<<<<< HEAD
 * Copyright (c) 2006 Oracle.  All rights reserved.
=======
 * Copyright (c) 2006, 2017 Oracle and/or its affiliates. All rights reserved.
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
#include <linux/in.h>
#include <net/tcp.h>

<<<<<<< HEAD
#include "rds.h"
#include "tcp.h"

static void rds_tcp_cork(struct socket *sock, int val)
{
	mm_segment_t oldfs;

	oldfs = get_fs();
	set_fs(KERNEL_DS);
	sock->ops->setsockopt(sock, SOL_TCP, TCP_CORK, (char __user *)&val,
			      sizeof(val));
	set_fs(oldfs);
}

void rds_tcp_xmit_prepare(struct rds_connection *conn)
{
	struct rds_tcp_connection *tc = conn->c_transport_data;

	rds_tcp_cork(tc->t_sock, 1);
}

void rds_tcp_xmit_complete(struct rds_connection *conn)
{
	struct rds_tcp_connection *tc = conn->c_transport_data;

	rds_tcp_cork(tc->t_sock, 0);
=======
#include "rds_single_path.h"
#include "rds.h"
#include "tcp.h"

void rds_tcp_xmit_path_prepare(struct rds_conn_path *cp)
{
	struct rds_tcp_connection *tc = cp->cp_transport_data;

	tcp_sock_set_cork(tc->t_sock->sk, true);
}

void rds_tcp_xmit_path_complete(struct rds_conn_path *cp)
{
	struct rds_tcp_connection *tc = cp->cp_transport_data;

	tcp_sock_set_cork(tc->t_sock->sk, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* the core send_sem serializes this with other xmit and shutdown */
static int rds_tcp_sendmsg(struct socket *sock, void *data, unsigned int len)
{
	struct kvec vec = {
<<<<<<< HEAD
                .iov_base = data,
                .iov_len = len,
	};
        struct msghdr msg = {
                .msg_flags = MSG_DONTWAIT | MSG_NOSIGNAL,
        };
=======
		.iov_base = data,
		.iov_len = len,
	};
	struct msghdr msg = {
		.msg_flags = MSG_DONTWAIT | MSG_NOSIGNAL,
	};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return kernel_sendmsg(sock, &msg, &vec, 1, vec.iov_len);
}

/* the core send_sem serializes this with other xmit and shutdown */
int rds_tcp_xmit(struct rds_connection *conn, struct rds_message *rm,
<<<<<<< HEAD
	         unsigned int hdr_off, unsigned int sg, unsigned int off)
{
	struct rds_tcp_connection *tc = conn->c_transport_data;
=======
		 unsigned int hdr_off, unsigned int sg, unsigned int off)
{
	struct rds_conn_path *cp = rm->m_inc.i_conn_path;
	struct rds_tcp_connection *tc = cp->cp_transport_data;
	struct msghdr msg = {};
	struct bio_vec bvec;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int done = 0;
	int ret = 0;

	if (hdr_off == 0) {
		/*
		 * m_ack_seq is set to the sequence number of the last byte of
		 * header and data.  see rds_tcp_is_acked().
		 */
<<<<<<< HEAD
		tc->t_last_sent_nxt = rds_tcp_snd_nxt(tc);
		rm->m_ack_seq = tc->t_last_sent_nxt +
				sizeof(struct rds_header) +
				be32_to_cpu(rm->m_inc.i_hdr.h_len) - 1;
		smp_mb__before_clear_bit();
		set_bit(RDS_MSG_HAS_ACK_SEQ, &rm->m_flags);
		tc->t_last_expected_una = rm->m_ack_seq + 1;

		rdsdebug("rm %p tcp nxt %u ack_seq %llu\n",
			 rm, rds_tcp_snd_nxt(tc),
=======
		tc->t_last_sent_nxt = rds_tcp_write_seq(tc);
		rm->m_ack_seq = tc->t_last_sent_nxt +
				sizeof(struct rds_header) +
				be32_to_cpu(rm->m_inc.i_hdr.h_len) - 1;
		smp_mb__before_atomic();
		set_bit(RDS_MSG_HAS_ACK_SEQ, &rm->m_flags);
		tc->t_last_expected_una = rm->m_ack_seq + 1;

		if (test_bit(RDS_MSG_RETRANSMITTED, &rm->m_flags))
			rm->m_inc.i_hdr.h_flags |= RDS_FLAG_RETRANSMITTED;

		rdsdebug("rm %p tcp nxt %u ack_seq %llu\n",
			 rm, rds_tcp_write_seq(tc),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			 (unsigned long long)rm->m_ack_seq);
	}

	if (hdr_off < sizeof(struct rds_header)) {
		/* see rds_tcp_write_space() */
		set_bit(SOCK_NOSPACE, &tc->t_sock->sk->sk_socket->flags);

		ret = rds_tcp_sendmsg(tc->t_sock,
				      (void *)&rm->m_inc.i_hdr + hdr_off,
				      sizeof(rm->m_inc.i_hdr) - hdr_off);
		if (ret < 0)
			goto out;
		done += ret;
		if (hdr_off + done != sizeof(struct rds_header))
			goto out;
	}

	while (sg < rm->data.op_nents) {
<<<<<<< HEAD
		ret = tc->t_sock->ops->sendpage(tc->t_sock,
						sg_page(&rm->data.op_sg[sg]),
						rm->data.op_sg[sg].offset + off,
						rm->data.op_sg[sg].length - off,
						MSG_DONTWAIT|MSG_NOSIGNAL);
=======
		msg.msg_flags = MSG_SPLICE_PAGES | MSG_DONTWAIT | MSG_NOSIGNAL;
		if (sg + 1 < rm->data.op_nents)
			msg.msg_flags |= MSG_MORE;

		bvec_set_page(&bvec, sg_page(&rm->data.op_sg[sg]),
			      rm->data.op_sg[sg].length - off,
			      rm->data.op_sg[sg].offset + off);
		iov_iter_bvec(&msg.msg_iter, ITER_SOURCE, &bvec, 1,
			      rm->data.op_sg[sg].length - off);
		ret = sock_sendmsg(tc->t_sock, &msg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		rdsdebug("tcp sendpage %p:%u:%u ret %d\n", (void *)sg_page(&rm->data.op_sg[sg]),
			 rm->data.op_sg[sg].offset + off, rm->data.op_sg[sg].length - off,
			 ret);
		if (ret <= 0)
			break;

		off += ret;
		done += ret;
		if (off == rm->data.op_sg[sg].length) {
			off = 0;
			sg++;
		}
	}

out:
	if (ret <= 0) {
		/* write_space will hit after EAGAIN, all else fatal */
		if (ret == -EAGAIN) {
			rds_tcp_stats_inc(s_tcp_sndbuf_full);
			ret = 0;
		} else {
<<<<<<< HEAD
			printk(KERN_WARNING "RDS/tcp: send to %pI4 "
			       "returned %d, disconnecting and reconnecting\n",
			       &conn->c_faddr, ret);
			rds_conn_drop(conn);
=======
			/* No need to disconnect/reconnect if path_drop
			 * has already been triggered, because, e.g., of
			 * an incoming RST.
			 */
			if (rds_conn_path_up(cp)) {
				pr_warn("RDS/tcp: send to %pI6c on cp [%d]"
					"returned %d, "
					"disconnecting and reconnecting\n",
					&conn->c_faddr, cp->cp_index, ret);
				rds_conn_path_drop(cp, false);
			}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}
	if (done == 0)
		done = ret;
	return done;
}

/*
 * rm->m_ack_seq is set to the tcp sequence number that corresponds to the
 * last byte of the message, including the header.  This means that the
 * entire message has been received if rm->m_ack_seq is "before" the next
 * unacked byte of the TCP sequence space.  We have to do very careful
 * wrapping 32bit comparisons here.
 */
static int rds_tcp_is_acked(struct rds_message *rm, uint64_t ack)
{
	if (!test_bit(RDS_MSG_HAS_ACK_SEQ, &rm->m_flags))
		return 0;
	return (__s32)((u32)rm->m_ack_seq - (u32)ack) < 0;
}

void rds_tcp_write_space(struct sock *sk)
{
	void (*write_space)(struct sock *sk);
<<<<<<< HEAD
	struct rds_connection *conn;
	struct rds_tcp_connection *tc;

	read_lock_bh(&sk->sk_callback_lock);
	conn = sk->sk_user_data;
	if (!conn) {
=======
	struct rds_conn_path *cp;
	struct rds_tcp_connection *tc;

	read_lock_bh(&sk->sk_callback_lock);
	cp = sk->sk_user_data;
	if (!cp) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		write_space = sk->sk_write_space;
		goto out;
	}

<<<<<<< HEAD
	tc = conn->c_transport_data;
=======
	tc = cp->cp_transport_data;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rdsdebug("write_space for tc %p\n", tc);
	write_space = tc->t_orig_write_space;
	rds_tcp_stats_inc(s_tcp_write_space_calls);

	rdsdebug("tcp una %u\n", rds_tcp_snd_una(tc));
	tc->t_last_seen_una = rds_tcp_snd_una(tc);
<<<<<<< HEAD
	rds_send_drop_acked(conn, rds_tcp_snd_una(tc), rds_tcp_is_acked);

        if ((atomic_read(&sk->sk_wmem_alloc) << 1) <= sk->sk_sndbuf)
		queue_delayed_work(rds_wq, &conn->c_send_w, 0);
=======
	rds_send_path_drop_acked(cp, rds_tcp_snd_una(tc), rds_tcp_is_acked);

	rcu_read_lock();
	if ((refcount_read(&sk->sk_wmem_alloc) << 1) <= sk->sk_sndbuf &&
	    !rds_destroy_pending(cp->cp_conn))
		queue_delayed_work(rds_wq, &cp->cp_send_w, 0);
	rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

out:
	read_unlock_bh(&sk->sk_callback_lock);

	/*
	 * write_space is only called when data leaves tcp's send queue if
	 * SOCK_NOSPACE is set.  We set SOCK_NOSPACE every time we put
	 * data in tcp's send queue because we use write_space to parse the
	 * sequence numbers and notice that rds messages have been fully
	 * received.
	 *
	 * tcp's write_space clears SOCK_NOSPACE if the send queue has more
	 * than a certain amount of space. So we need to set it again *after*
	 * we call tcp's write_space or else we might only get called on the
	 * first of a series of incoming tcp acks.
	 */
	write_space(sk);

	if (sk->sk_socket)
		set_bit(SOCK_NOSPACE, &sk->sk_socket->flags);
}
