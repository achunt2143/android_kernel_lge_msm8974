<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *     SUCS NET3:
 *
 *     Generic stream handling routines. These are generic for most
 *     protocols. Even IP. Tonight 8-).
 *     This is used because TCP, LLC (others too) layer all have mostly
 *     identical sendmsg() and recvmsg() code.
 *     So we (will) share it here.
 *
 *     Authors:        Arnaldo Carvalho de Melo <acme@conectiva.com.br>
 *                     (from old tcp.c code)
 *                     Alan Cox <alan@lxorguk.ukuu.org.uk> (Borrowed comments 8-))
 */

#include <linux/module.h>
<<<<<<< HEAD
=======
#include <linux/sched/signal.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/net.h>
#include <linux/signal.h>
#include <linux/tcp.h>
#include <linux/wait.h>
#include <net/sock.h>

/**
 * sk_stream_write_space - stream socket write_space callback.
 * @sk: socket
 *
 * FIXME: write proper description
 */
void sk_stream_write_space(struct sock *sk)
{
	struct socket *sock = sk->sk_socket;
	struct socket_wq *wq;

<<<<<<< HEAD
	if (sk_stream_wspace(sk) >= sk_stream_min_wspace(sk) && sock) {
=======
	if (__sk_stream_is_writeable(sk, 1) && sock) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		clear_bit(SOCK_NOSPACE, &sock->flags);

		rcu_read_lock();
		wq = rcu_dereference(sk->sk_wq);
<<<<<<< HEAD
		if (wq_has_sleeper(wq))
			wake_up_interruptible_poll(&wq->wait, POLLOUT |
						POLLWRNORM | POLLWRBAND);
		if (wq && wq->fasync_list && !(sk->sk_shutdown & SEND_SHUTDOWN))
			sock_wake_async(sock, SOCK_WAKE_SPACE, POLL_OUT);
		rcu_read_unlock();
	}
}
EXPORT_SYMBOL(sk_stream_write_space);
=======
		if (skwq_has_sleeper(wq))
			wake_up_interruptible_poll(&wq->wait, EPOLLOUT |
						EPOLLWRNORM | EPOLLWRBAND);
		if (wq && wq->fasync_list && !(sk->sk_shutdown & SEND_SHUTDOWN))
			sock_wake_async(wq, SOCK_WAKE_SPACE, POLL_OUT);
		rcu_read_unlock();
	}
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * sk_stream_wait_connect - Wait for a socket to get into the connected state
 * @sk: sock to wait on
 * @timeo_p: for how long to wait
 *
 * Must be called with the socket locked.
 */
int sk_stream_wait_connect(struct sock *sk, long *timeo_p)
{
<<<<<<< HEAD
	struct task_struct *tsk = current;
	DEFINE_WAIT(wait);
=======
	DEFINE_WAIT_FUNC(wait, woken_wake_function);
	struct task_struct *tsk = current;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int done;

	do {
		int err = sock_error(sk);
		if (err)
			return err;
		if ((1 << sk->sk_state) & ~(TCPF_SYN_SENT | TCPF_SYN_RECV))
			return -EPIPE;
		if (!*timeo_p)
			return -EAGAIN;
		if (signal_pending(tsk))
			return sock_intr_errno(*timeo_p);

<<<<<<< HEAD
		prepare_to_wait(sk_sleep(sk), &wait, TASK_INTERRUPTIBLE);
		sk->sk_write_pending++;
		done = sk_wait_event(sk, timeo_p,
				     !sk->sk_err &&
				     !((1 << sk->sk_state) &
				       ~(TCPF_ESTABLISHED | TCPF_CLOSE_WAIT)));
		finish_wait(sk_sleep(sk), &wait);
		sk->sk_write_pending--;
	} while (!done);
	return 0;
=======
		add_wait_queue(sk_sleep(sk), &wait);
		sk->sk_write_pending++;
		done = sk_wait_event(sk, timeo_p,
				     !READ_ONCE(sk->sk_err) &&
				     !((1 << READ_ONCE(sk->sk_state)) &
				       ~(TCPF_ESTABLISHED | TCPF_CLOSE_WAIT)), &wait);
		remove_wait_queue(sk_sleep(sk), &wait);
		sk->sk_write_pending--;
	} while (!done);
	return done < 0 ? done : 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL(sk_stream_wait_connect);

/**
 * sk_stream_closing - Return 1 if we still have things to send in our buffers.
 * @sk: socket to verify
 */
<<<<<<< HEAD
static inline int sk_stream_closing(struct sock *sk)
{
	return (1 << sk->sk_state) &
=======
static int sk_stream_closing(const struct sock *sk)
{
	return (1 << READ_ONCE(sk->sk_state)) &
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	       (TCPF_FIN_WAIT1 | TCPF_CLOSING | TCPF_LAST_ACK);
}

void sk_stream_wait_close(struct sock *sk, long timeout)
{
	if (timeout) {
<<<<<<< HEAD
		DEFINE_WAIT(wait);

		do {
			prepare_to_wait(sk_sleep(sk), &wait,
					TASK_INTERRUPTIBLE);
			if (sk_wait_event(sk, &timeout, !sk_stream_closing(sk)))
				break;
		} while (!signal_pending(current) && timeout);

		finish_wait(sk_sleep(sk), &wait);
=======
		DEFINE_WAIT_FUNC(wait, woken_wake_function);

		add_wait_queue(sk_sleep(sk), &wait);

		do {
			if (sk_wait_event(sk, &timeout, !sk_stream_closing(sk), &wait))
				break;
		} while (!signal_pending(current) && timeout);

		remove_wait_queue(sk_sleep(sk), &wait);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}
EXPORT_SYMBOL(sk_stream_wait_close);

/**
 * sk_stream_wait_memory - Wait for more memory for a socket
 * @sk: socket to wait for memory
 * @timeo_p: for how long
 */
int sk_stream_wait_memory(struct sock *sk, long *timeo_p)
{
<<<<<<< HEAD
	int err = 0;
	long vm_wait = 0;
	long current_timeo = *timeo_p;
	DEFINE_WAIT(wait);

	if (sk_stream_memory_free(sk))
		current_timeo = vm_wait = (net_random() % (HZ / 5)) + 2;

	while (1) {
		set_bit(SOCK_ASYNC_NOSPACE, &sk->sk_socket->flags);

		prepare_to_wait(sk_sleep(sk), &wait, TASK_INTERRUPTIBLE);
=======
	int ret, err = 0;
	long vm_wait = 0;
	long current_timeo = *timeo_p;
	DEFINE_WAIT_FUNC(wait, woken_wake_function);

	if (sk_stream_memory_free(sk))
		current_timeo = vm_wait = get_random_u32_below(HZ / 5) + 2;

	add_wait_queue(sk_sleep(sk), &wait);

	while (1) {
		sk_set_bit(SOCKWQ_ASYNC_NOSPACE, sk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (sk->sk_err || (sk->sk_shutdown & SEND_SHUTDOWN))
			goto do_error;
		if (!*timeo_p)
<<<<<<< HEAD
			goto do_nonblock;
		if (signal_pending(current))
			goto do_interrupted;
		clear_bit(SOCK_ASYNC_NOSPACE, &sk->sk_socket->flags);
=======
			goto do_eagain;
		if (signal_pending(current))
			goto do_interrupted;
		sk_clear_bit(SOCKWQ_ASYNC_NOSPACE, sk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (sk_stream_memory_free(sk) && !vm_wait)
			break;

		set_bit(SOCK_NOSPACE, &sk->sk_socket->flags);
		sk->sk_write_pending++;
<<<<<<< HEAD
		sk_wait_event(sk, &current_timeo, sk->sk_err ||
						  (sk->sk_shutdown & SEND_SHUTDOWN) ||
						  (sk_stream_memory_free(sk) &&
						  !vm_wait));
		sk->sk_write_pending--;
=======
		ret = sk_wait_event(sk, &current_timeo, READ_ONCE(sk->sk_err) ||
				    (READ_ONCE(sk->sk_shutdown) & SEND_SHUTDOWN) ||
				    (sk_stream_memory_free(sk) && !vm_wait),
				    &wait);
		sk->sk_write_pending--;
		if (ret < 0)
			goto do_error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (vm_wait) {
			vm_wait -= current_timeo;
			current_timeo = *timeo_p;
			if (current_timeo != MAX_SCHEDULE_TIMEOUT &&
			    (current_timeo -= vm_wait) < 0)
				current_timeo = 0;
			vm_wait = 0;
		}
		*timeo_p = current_timeo;
	}
out:
<<<<<<< HEAD
	finish_wait(sk_sleep(sk), &wait);
=======
	if (!sock_flag(sk, SOCK_DEAD))
		remove_wait_queue(sk_sleep(sk), &wait);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;

do_error:
	err = -EPIPE;
	goto out;
<<<<<<< HEAD
do_nonblock:
=======
do_eagain:
	/* Make sure that whenever EAGAIN is returned, EPOLLOUT event can
	 * be generated later.
	 * When TCP receives ACK packets that make room, tcp_check_space()
	 * only calls tcp_new_space() if SOCK_NOSPACE is set.
	 */
	set_bit(SOCK_NOSPACE, &sk->sk_socket->flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	err = -EAGAIN;
	goto out;
do_interrupted:
	err = sock_intr_errno(*timeo_p);
	goto out;
}
EXPORT_SYMBOL(sk_stream_wait_memory);

int sk_stream_error(struct sock *sk, int flags, int err)
{
	if (err == -EPIPE)
		err = sock_error(sk) ? : -EPIPE;
	if (err == -EPIPE && !(flags & MSG_NOSIGNAL))
		send_sig(SIGPIPE, current, 0);
	return err;
}
EXPORT_SYMBOL(sk_stream_error);

void sk_stream_kill_queues(struct sock *sk)
{
	/* First the read buffer. */
	__skb_queue_purge(&sk->sk_receive_queue);

<<<<<<< HEAD
	/* Next, the error queue. */
	__skb_queue_purge(&sk->sk_error_queue);

	/* Next, the write queue. */
	WARN_ON(!skb_queue_empty(&sk->sk_write_queue));

	/* Account for returned memory. */
	sk_mem_reclaim(sk);

	WARN_ON(sk->sk_wmem_queued);
	WARN_ON(sk->sk_forward_alloc);
=======
	/* Next, the error queue.
	 * We need to use queue lock, because other threads might
	 * add packets to the queue without socket lock being held.
	 */
	skb_queue_purge(&sk->sk_error_queue);

	/* Next, the write queue. */
	WARN_ON_ONCE(!skb_queue_empty(&sk->sk_write_queue));

	/* Account for returned memory. */
	sk_mem_reclaim_final(sk);

	WARN_ON_ONCE(sk->sk_wmem_queued);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* It is _impossible_ for the backlog to contain anything
	 * when we get here.  All user references to this socket
	 * have gone away, only the net layer knows can touch it.
	 */
}
EXPORT_SYMBOL(sk_stream_kill_queues);
