<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *	SUCS NET3:
 *
 *	Generic datagram handling routines. These are generic for all
 *	protocols. Possibly a generic IP version on top of these would
 *	make sense. Not tonight however 8-).
 *	This is used because UDP, RAW, PACKET, DDP, IPX, AX.25 and
 *	NetROM layer all have identical poll code and mostly
 *	identical recvmsg() code. So we share it here. The poll was
 *	shared before but buried in udp.c so I moved it.
 *
 *	Authors:	Alan Cox <alan@lxorguk.ukuu.org.uk>. (datagram_poll() from old
 *						     udp.c code)
 *
 *	Fixes:
 *		Alan Cox	:	NULL return from skb_peek_copy()
 *					understood
 *		Alan Cox	:	Rewrote skb_read_datagram to avoid the
 *					skb_peek_copy stuff.
 *		Alan Cox	:	Added support for SOCK_SEQPACKET.
 *					IPX can no longer use the SO_TYPE hack
 *					but AX.25 now works right, and SPX is
 *					feasible.
 *		Alan Cox	:	Fixed write poll of non IP protocol
 *					crash.
 *		Florian  La Roche:	Changed for my new skbuff handling.
 *		Darryl Miles	:	Fixed non-blocking SOCK_SEQPACKET.
 *		Linus Torvalds	:	BSD semantic fixes.
 *		Alan Cox	:	Datagram iovec handling
 *		Darryl Miles	:	Fixed non-blocking SOCK_STREAM.
 *		Alan Cox	:	POSIXisms
 *		Pete Wyckoff    :       Unconnected accept() fix.
 *
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
<<<<<<< HEAD
#include <asm/uaccess.h>
=======
#include <linux/uaccess.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/mm.h>
#include <linux/interrupt.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/inet.h>
#include <linux/netdevice.h>
#include <linux/rtnetlink.h>
#include <linux/poll.h>
#include <linux/highmem.h>
#include <linux/spinlock.h>
#include <linux/slab.h>
<<<<<<< HEAD
=======
#include <linux/pagemap.h>
#include <linux/iov_iter.h>
#include <linux/indirect_call_wrapper.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <net/protocol.h>
#include <linux/skbuff.h>

#include <net/checksum.h>
#include <net/sock.h>
#include <net/tcp_states.h>
#include <trace/events/skb.h>
<<<<<<< HEAD
=======
#include <net/busy_poll.h>
#include <crypto/hash.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 *	Is a socket 'connection oriented' ?
 */
static inline int connection_based(struct sock *sk)
{
	return sk->sk_type == SOCK_SEQPACKET || sk->sk_type == SOCK_STREAM;
}

<<<<<<< HEAD
static int receiver_wake_function(wait_queue_t *wait, unsigned mode, int sync,
				  void *key)
{
	unsigned long bits = (unsigned long)key;

	/*
	 * Avoid a wakeup if event not interesting for us
	 */
	if (bits && !(bits & (POLLIN | POLLERR)))
=======
static int receiver_wake_function(wait_queue_entry_t *wait, unsigned int mode, int sync,
				  void *key)
{
	/*
	 * Avoid a wakeup if event not interesting for us
	 */
	if (key && !(key_to_poll(key) & (EPOLLIN | EPOLLERR)))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;
	return autoremove_wake_function(wait, mode, sync, key);
}
/*
<<<<<<< HEAD
 * Wait for a packet..
 */
static int wait_for_packet(struct sock *sk, int *err, long *timeo_p)
=======
 * Wait for the last received packet to be different from skb
 */
int __skb_wait_for_more_packets(struct sock *sk, struct sk_buff_head *queue,
				int *err, long *timeo_p,
				const struct sk_buff *skb)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int error;
	DEFINE_WAIT_FUNC(wait, receiver_wake_function);

	prepare_to_wait_exclusive(sk_sleep(sk), &wait, TASK_INTERRUPTIBLE);

	/* Socket errors? */
	error = sock_error(sk);
	if (error)
		goto out_err;

<<<<<<< HEAD
	if (!skb_queue_empty(&sk->sk_receive_queue))
=======
	if (READ_ONCE(queue->prev) != skb)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;

	/* Socket shut down? */
	if (sk->sk_shutdown & RCV_SHUTDOWN)
		goto out_noerr;

	/* Sequenced packets can come disconnected.
	 * If so we report the problem
	 */
	error = -ENOTCONN;
	if (connection_based(sk) &&
	    !(sk->sk_state == TCP_ESTABLISHED || sk->sk_state == TCP_LISTEN))
		goto out_err;

	/* handle signals */
	if (signal_pending(current))
		goto interrupted;

	error = 0;
	*timeo_p = schedule_timeout(*timeo_p);
out:
	finish_wait(sk_sleep(sk), &wait);
	return error;
interrupted:
	error = sock_intr_errno(*timeo_p);
out_err:
	*err = error;
	goto out;
out_noerr:
	*err = 0;
	error = 1;
	goto out;
}
<<<<<<< HEAD

static int skb_set_peeked(struct sk_buff *skb)
=======
EXPORT_SYMBOL(__skb_wait_for_more_packets);

static struct sk_buff *skb_set_peeked(struct sk_buff *skb)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct sk_buff *nskb;

	if (skb->peeked)
<<<<<<< HEAD
		return 0;
=======
		return skb;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* We have to unshare an skb before modifying it. */
	if (!skb_shared(skb))
		goto done;

	nskb = skb_clone(skb, GFP_ATOMIC);
	if (!nskb)
<<<<<<< HEAD
		return -ENOMEM;
=======
		return ERR_PTR(-ENOMEM);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	skb->prev->next = nskb;
	skb->next->prev = nskb;
	nskb->prev = skb->prev;
	nskb->next = skb->next;

	consume_skb(skb);
	skb = nskb;

done:
	skb->peeked = 1;

<<<<<<< HEAD
	return 0;
}

/**
 *	__skb_recv_datagram - Receive a datagram skbuff
 *	@sk: socket
 *	@flags: MSG_ flags
 *	@off: an offset in bytes to peek skb from. Returns an offset
 *	      within an skb where data actually starts
 *	@peeked: returns non-zero if this packet has been seen before
 *	@err: error code returned
=======
	return skb;
}

struct sk_buff *__skb_try_recv_from_queue(struct sock *sk,
					  struct sk_buff_head *queue,
					  unsigned int flags,
					  int *off, int *err,
					  struct sk_buff **last)
{
	bool peek_at_off = false;
	struct sk_buff *skb;
	int _off = 0;

	if (unlikely(flags & MSG_PEEK && *off >= 0)) {
		peek_at_off = true;
		_off = *off;
	}

	*last = queue->prev;
	skb_queue_walk(queue, skb) {
		if (flags & MSG_PEEK) {
			if (peek_at_off && _off >= skb->len &&
			    (_off || skb->peeked)) {
				_off -= skb->len;
				continue;
			}
			if (!skb->len) {
				skb = skb_set_peeked(skb);
				if (IS_ERR(skb)) {
					*err = PTR_ERR(skb);
					return NULL;
				}
			}
			refcount_inc(&skb->users);
		} else {
			__skb_unlink(skb, queue);
		}
		*off = _off;
		return skb;
	}
	return NULL;
}

/**
 *	__skb_try_recv_datagram - Receive a datagram skbuff
 *	@sk: socket
 *	@queue: socket queue from which to receive
 *	@flags: MSG\_ flags
 *	@off: an offset in bytes to peek skb from. Returns an offset
 *	      within an skb where data actually starts
 *	@err: error code returned
 *	@last: set to last peeked message to inform the wait function
 *	       what to look for when peeking
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 *	Get a datagram skbuff, understands the peeking, nonblocking wakeups
 *	and possible races. This replaces identical code in packet, raw and
 *	udp, as well as the IPX AX.25 and Appletalk. It also finally fixes
 *	the long standing peek and read race for datagram sockets. If you
 *	alter this routine remember it must be re-entrant.
 *
<<<<<<< HEAD
 *	This function will lock the socket if a skb is returned, so the caller
 *	needs to unlock the socket in that case (usually by calling
 *	skb_free_datagram)
=======
 *	This function will lock the socket if a skb is returned, so
 *	the caller needs to unlock the socket in that case (usually by
 *	calling skb_free_datagram). Returns NULL with @err set to
 *	-EAGAIN if no data was available or to some other value if an
 *	error was detected.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 *	* It does not lock socket since today. This function is
 *	* free of race conditions. This measure should/can improve
 *	* significantly datagram socket latencies at high loads,
 *	* when data copying to user space takes lots of time.
 *	* (BTW I've just killed the last cli() in IP/IPv6/core/netlink/packet
 *	*  8) Great win.)
 *	*			                    --ANK (980729)
 *
 *	The order of the tests when we find no data waiting are specified
 *	quite explicitly by POSIX 1003.1g, don't change them without having
 *	the standard around please.
 */
<<<<<<< HEAD
struct sk_buff *__skb_recv_datagram(struct sock *sk, unsigned flags,
				    int *peeked, int *off, int *err)
{
	struct sk_buff_head *queue = &sk->sk_receive_queue;
	struct sk_buff *skb;
	unsigned long cpu_flags;
	long timeo;
=======
struct sk_buff *__skb_try_recv_datagram(struct sock *sk,
					struct sk_buff_head *queue,
					unsigned int flags, int *off, int *err,
					struct sk_buff **last)
{
	struct sk_buff *skb;
	unsigned long cpu_flags;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Caller is allowed not to check sk->sk_err before skb_recv_datagram()
	 */
	int error = sock_error(sk);

	if (error)
		goto no_packet;

<<<<<<< HEAD
	timeo = sock_rcvtimeo(sk, flags & MSG_DONTWAIT);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	do {
		/* Again only user level code calls this function, so nothing
		 * interrupt level will suddenly eat the receive_queue.
		 *
		 * Look at current nfs client by the way...
		 * However, this function was correct in any case. 8)
		 */
<<<<<<< HEAD

		spin_lock_irqsave(&queue->lock, cpu_flags);
		skb_queue_walk(queue, skb) {
			*peeked = skb->peeked;
			if (flags & MSG_PEEK) {
				if (*off >= skb->len && skb->len) {
					*off -= skb->len;
					continue;
				}

				error = skb_set_peeked(skb);
				if (error)
					goto unlock_err;

				atomic_inc(&skb->users);
			} else
				__skb_unlink(skb, queue);

			spin_unlock_irqrestore(&queue->lock, cpu_flags);
			return skb;
		}
		spin_unlock_irqrestore(&queue->lock, cpu_flags);

		/* User doesn't want to wait */
		error = -EAGAIN;
		if (!timeo)
			goto no_packet;

	} while (!wait_for_packet(sk, err, &timeo));

	return NULL;

unlock_err:
	spin_unlock_irqrestore(&queue->lock, cpu_flags);
=======
		spin_lock_irqsave(&queue->lock, cpu_flags);
		skb = __skb_try_recv_from_queue(sk, queue, flags, off, &error,
						last);
		spin_unlock_irqrestore(&queue->lock, cpu_flags);
		if (error)
			goto no_packet;
		if (skb)
			return skb;

		if (!sk_can_busy_loop(sk))
			break;

		sk_busy_loop(sk, flags & MSG_DONTWAIT);
	} while (READ_ONCE(queue->prev) != *last);

	error = -EAGAIN;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
no_packet:
	*err = error;
	return NULL;
}
<<<<<<< HEAD
EXPORT_SYMBOL(__skb_recv_datagram);

struct sk_buff *skb_recv_datagram(struct sock *sk, unsigned flags,
				  int noblock, int *err)
{
	int peeked, off = 0;

	return __skb_recv_datagram(sk, flags | (noblock ? MSG_DONTWAIT : 0),
				   &peeked, &off, err);
=======
EXPORT_SYMBOL(__skb_try_recv_datagram);

struct sk_buff *__skb_recv_datagram(struct sock *sk,
				    struct sk_buff_head *sk_queue,
				    unsigned int flags, int *off, int *err)
{
	struct sk_buff *skb, *last;
	long timeo;

	timeo = sock_rcvtimeo(sk, flags & MSG_DONTWAIT);

	do {
		skb = __skb_try_recv_datagram(sk, sk_queue, flags, off, err,
					      &last);
		if (skb)
			return skb;

		if (*err != -EAGAIN)
			break;
	} while (timeo &&
		 !__skb_wait_for_more_packets(sk, sk_queue, err,
					      &timeo, last));

	return NULL;
}
EXPORT_SYMBOL(__skb_recv_datagram);

struct sk_buff *skb_recv_datagram(struct sock *sk, unsigned int flags,
				  int *err)
{
	int off = 0;

	return __skb_recv_datagram(sk, &sk->sk_receive_queue, flags,
				   &off, err);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL(skb_recv_datagram);

void skb_free_datagram(struct sock *sk, struct sk_buff *skb)
{
	consume_skb(skb);
<<<<<<< HEAD
	sk_mem_reclaim_partial(sk);
}
EXPORT_SYMBOL(skb_free_datagram);

void skb_free_datagram_locked(struct sock *sk, struct sk_buff *skb)
{
	bool slow;

	if (likely(atomic_read(&skb->users) == 1))
		smp_rmb();
	else if (likely(!atomic_dec_and_test(&skb->users)))
		return;

	slow = lock_sock_fast(sk);
	skb_orphan(skb);
	sk_mem_reclaim_partial(sk);
	unlock_sock_fast(sk, slow);

	/* skb is now orphaned, can be freed outside of locked section */
	trace_kfree_skb(skb, skb_free_datagram_locked);
	__kfree_skb(skb);
}
EXPORT_SYMBOL(skb_free_datagram_locked);
=======
}
EXPORT_SYMBOL(skb_free_datagram);

void __skb_free_datagram_locked(struct sock *sk, struct sk_buff *skb, int len)
{
	bool slow;

	if (!skb_unref(skb)) {
		sk_peek_offset_bwd(sk, len);
		return;
	}

	slow = lock_sock_fast(sk);
	sk_peek_offset_bwd(sk, len);
	skb_orphan(skb);
	unlock_sock_fast(sk, slow);

	/* skb is now orphaned, can be freed outside of locked section */
	__kfree_skb(skb);
}
EXPORT_SYMBOL(__skb_free_datagram_locked);

int __sk_queue_drop_skb(struct sock *sk, struct sk_buff_head *sk_queue,
			struct sk_buff *skb, unsigned int flags,
			void (*destructor)(struct sock *sk,
					   struct sk_buff *skb))
{
	int err = 0;

	if (flags & MSG_PEEK) {
		err = -ENOENT;
		spin_lock_bh(&sk_queue->lock);
		if (skb->next) {
			__skb_unlink(skb, sk_queue);
			refcount_dec(&skb->users);
			if (destructor)
				destructor(sk, skb);
			err = 0;
		}
		spin_unlock_bh(&sk_queue->lock);
	}

	atomic_inc(&sk->sk_drops);
	return err;
}
EXPORT_SYMBOL(__sk_queue_drop_skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 *	skb_kill_datagram - Free a datagram skbuff forcibly
 *	@sk: socket
 *	@skb: datagram skbuff
<<<<<<< HEAD
 *	@flags: MSG_ flags
=======
 *	@flags: MSG\_ flags
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 *	This function frees a datagram skbuff that was received by
 *	skb_recv_datagram.  The flags argument must match the one
 *	used for skb_recv_datagram.
 *
 *	If the MSG_PEEK flag is set, and the packet is still on the
 *	receive queue of the socket, it will be taken off the queue
 *	before it is freed.
 *
 *	This function currently only disables BH when acquiring the
 *	sk_receive_queue lock.  Therefore it must not be used in a
 *	context where that lock is acquired in an IRQ context.
 *
 *	It returns 0 if the packet was removed by us.
 */

int skb_kill_datagram(struct sock *sk, struct sk_buff *skb, unsigned int flags)
{
<<<<<<< HEAD
	int err = 0;

	if (flags & MSG_PEEK) {
		err = -ENOENT;
		spin_lock_bh(&sk->sk_receive_queue.lock);
		if (skb == skb_peek(&sk->sk_receive_queue)) {
			__skb_unlink(skb, &sk->sk_receive_queue);
			atomic_dec(&skb->users);
			err = 0;
		}
		spin_unlock_bh(&sk->sk_receive_queue.lock);
	}

	kfree_skb(skb);
	atomic_inc(&sk->sk_drops);
	sk_mem_reclaim_partial(sk);

=======
	int err = __sk_queue_drop_skb(sk, &sk->sk_receive_queue, skb, flags,
				      NULL);

	kfree_skb(skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}
EXPORT_SYMBOL(skb_kill_datagram);

<<<<<<< HEAD
/**
 *	skb_copy_datagram_iovec - Copy a datagram to an iovec.
 *	@skb: buffer to copy
 *	@offset: offset in the buffer to start copying from
 *	@to: io vector to copy to
 *	@len: amount of data to copy from buffer to iovec
 *
 *	Note: the iovec is modified during the copy.
 */
int skb_copy_datagram_iovec(const struct sk_buff *skb, int offset,
			    struct iovec *to, int len)
{
	int start = skb_headlen(skb);
	int i, copy = start - offset;
	struct sk_buff *frag_iter;

	trace_skb_copy_datagram_iovec(skb, len);

	/* Copy header. */
	if (copy > 0) {
		if (copy > len)
			copy = len;
		if (memcpy_toiovec(to, skb->data + offset, copy))
			goto fault;
		if ((len -= copy) == 0)
			return 0;
		offset += copy;
	}

	/* Copy paged appendix. Hmm... why does this look so complicated? */
	for (i = 0; i < skb_shinfo(skb)->nr_frags; i++) {
		int end;
		const skb_frag_t *frag = &skb_shinfo(skb)->frags[i];

		WARN_ON(start > offset + len);

		end = start + skb_frag_size(frag);
		if ((copy = end - offset) > 0) {
			int err;
			u8  *vaddr;
			struct page *page = skb_frag_page(frag);

			if (copy > len)
				copy = len;
			vaddr = kmap(page);
			err = memcpy_toiovec(to, vaddr + frag->page_offset +
					     offset - start, copy);
			kunmap(page);
			if (err)
				goto fault;
			if (!(len -= copy))
				return 0;
			offset += copy;
		}
		start = end;
	}

	skb_walk_frags(skb, frag_iter) {
		int end;

		WARN_ON(start > offset + len);

		end = start + frag_iter->len;
		if ((copy = end - offset) > 0) {
			if (copy > len)
				copy = len;
			if (skb_copy_datagram_iovec(frag_iter,
						    offset - start,
						    to, copy))
				goto fault;
			if ((len -= copy) == 0)
				return 0;
			offset += copy;
		}
		start = end;
	}
	if (!len)
		return 0;

fault:
	return -EFAULT;
}
EXPORT_SYMBOL(skb_copy_datagram_iovec);

/**
 *	skb_copy_datagram_const_iovec - Copy a datagram to an iovec.
 *	@skb: buffer to copy
 *	@offset: offset in the buffer to start copying from
 *	@to: io vector to copy to
 *	@to_offset: offset in the io vector to start copying to
 *	@len: amount of data to copy from buffer to iovec
 *
 *	Returns 0 or -EFAULT.
 *	Note: the iovec is not modified during the copy.
 */
int skb_copy_datagram_const_iovec(const struct sk_buff *skb, int offset,
				  const struct iovec *to, int to_offset,
				  int len)
{
	int start = skb_headlen(skb);
	int i, copy = start - offset;
	struct sk_buff *frag_iter;

	/* Copy header. */
	if (copy > 0) {
		if (copy > len)
			copy = len;
		if (memcpy_toiovecend(to, skb->data + offset, to_offset, copy))
			goto fault;
		if ((len -= copy) == 0)
			return 0;
		offset += copy;
		to_offset += copy;
	}

	/* Copy paged appendix. Hmm... why does this look so complicated? */
	for (i = 0; i < skb_shinfo(skb)->nr_frags; i++) {
		int end;
		const skb_frag_t *frag = &skb_shinfo(skb)->frags[i];

		WARN_ON(start > offset + len);

		end = start + skb_frag_size(frag);
		if ((copy = end - offset) > 0) {
			int err;
			u8  *vaddr;
			struct page *page = skb_frag_page(frag);

			if (copy > len)
				copy = len;
			vaddr = kmap(page);
			err = memcpy_toiovecend(to, vaddr + frag->page_offset +
						offset - start, to_offset, copy);
			kunmap(page);
			if (err)
				goto fault;
			if (!(len -= copy))
				return 0;
			offset += copy;
			to_offset += copy;
		}
		start = end;
	}

	skb_walk_frags(skb, frag_iter) {
		int end;

		WARN_ON(start > offset + len);

		end = start + frag_iter->len;
		if ((copy = end - offset) > 0) {
			if (copy > len)
				copy = len;
			if (skb_copy_datagram_const_iovec(frag_iter,
							  offset - start,
							  to, to_offset,
							  copy))
				goto fault;
			if ((len -= copy) == 0)
				return 0;
			offset += copy;
			to_offset += copy;
		}
		start = end;
	}
	if (!len)
		return 0;

fault:
	return -EFAULT;
}
EXPORT_SYMBOL(skb_copy_datagram_const_iovec);

/**
 *	skb_copy_datagram_from_iovec - Copy a datagram from an iovec.
 *	@skb: buffer to copy
 *	@offset: offset in the buffer to start copying to
 *	@from: io vector to copy to
 *	@from_offset: offset in the io vector to start copying from
 *	@len: amount of data to copy to buffer from iovec
 *
 *	Returns 0 or -EFAULT.
 *	Note: the iovec is not modified during the copy.
 */
int skb_copy_datagram_from_iovec(struct sk_buff *skb, int offset,
				 const struct iovec *from, int from_offset,
=======
INDIRECT_CALLABLE_DECLARE(static size_t simple_copy_to_iter(const void *addr,
						size_t bytes,
						void *data __always_unused,
						struct iov_iter *i));

static int __skb_datagram_iter(const struct sk_buff *skb, int offset,
			       struct iov_iter *to, int len, bool fault_short,
			       size_t (*cb)(const void *, size_t, void *,
					    struct iov_iter *), void *data)
{
	int start = skb_headlen(skb);
	int i, copy = start - offset, start_off = offset, n;
	struct sk_buff *frag_iter;

	/* Copy header. */
	if (copy > 0) {
		if (copy > len)
			copy = len;
		n = INDIRECT_CALL_1(cb, simple_copy_to_iter,
				    skb->data + offset, copy, data, to);
		offset += n;
		if (n != copy)
			goto short_copy;
		if ((len -= copy) == 0)
			return 0;
	}

	/* Copy paged appendix. Hmm... why does this look so complicated? */
	for (i = 0; i < skb_shinfo(skb)->nr_frags; i++) {
		int end;
		const skb_frag_t *frag = &skb_shinfo(skb)->frags[i];

		WARN_ON(start > offset + len);

		end = start + skb_frag_size(frag);
		if ((copy = end - offset) > 0) {
			struct page *page = skb_frag_page(frag);
			u8 *vaddr = kmap(page);

			if (copy > len)
				copy = len;
			n = INDIRECT_CALL_1(cb, simple_copy_to_iter,
					vaddr + skb_frag_off(frag) + offset - start,
					copy, data, to);
			kunmap(page);
			offset += n;
			if (n != copy)
				goto short_copy;
			if (!(len -= copy))
				return 0;
		}
		start = end;
	}

	skb_walk_frags(skb, frag_iter) {
		int end;

		WARN_ON(start > offset + len);

		end = start + frag_iter->len;
		if ((copy = end - offset) > 0) {
			if (copy > len)
				copy = len;
			if (__skb_datagram_iter(frag_iter, offset - start,
						to, copy, fault_short, cb, data))
				goto fault;
			if ((len -= copy) == 0)
				return 0;
			offset += copy;
		}
		start = end;
	}
	if (!len)
		return 0;

	/* This is not really a user copy fault, but rather someone
	 * gave us a bogus length on the skb.  We should probably
	 * print a warning here as it may indicate a kernel bug.
	 */

fault:
	iov_iter_revert(to, offset - start_off);
	return -EFAULT;

short_copy:
	if (fault_short || iov_iter_count(to))
		goto fault;

	return 0;
}

static size_t hash_and_copy_to_iter(const void *addr, size_t bytes, void *hashp,
				    struct iov_iter *i)
{
#ifdef CONFIG_CRYPTO_HASH
	struct ahash_request *hash = hashp;
	struct scatterlist sg;
	size_t copied;

	copied = copy_to_iter(addr, bytes, i);
	sg_init_one(&sg, addr, copied);
	ahash_request_set_crypt(hash, &sg, NULL, copied);
	crypto_ahash_update(hash);
	return copied;
#else
	return 0;
#endif
}

/**
 *	skb_copy_and_hash_datagram_iter - Copy datagram to an iovec iterator
 *          and update a hash.
 *	@skb: buffer to copy
 *	@offset: offset in the buffer to start copying from
 *	@to: iovec iterator to copy to
 *	@len: amount of data to copy from buffer to iovec
 *      @hash: hash request to update
 */
int skb_copy_and_hash_datagram_iter(const struct sk_buff *skb, int offset,
			   struct iov_iter *to, int len,
			   struct ahash_request *hash)
{
	return __skb_datagram_iter(skb, offset, to, len, true,
			hash_and_copy_to_iter, hash);
}
EXPORT_SYMBOL(skb_copy_and_hash_datagram_iter);

static size_t simple_copy_to_iter(const void *addr, size_t bytes,
		void *data __always_unused, struct iov_iter *i)
{
	return copy_to_iter(addr, bytes, i);
}

/**
 *	skb_copy_datagram_iter - Copy a datagram to an iovec iterator.
 *	@skb: buffer to copy
 *	@offset: offset in the buffer to start copying from
 *	@to: iovec iterator to copy to
 *	@len: amount of data to copy from buffer to iovec
 */
int skb_copy_datagram_iter(const struct sk_buff *skb, int offset,
			   struct iov_iter *to, int len)
{
	trace_skb_copy_datagram_iovec(skb, len);
	return __skb_datagram_iter(skb, offset, to, len, false,
			simple_copy_to_iter, NULL);
}
EXPORT_SYMBOL(skb_copy_datagram_iter);

/**
 *	skb_copy_datagram_from_iter - Copy a datagram from an iov_iter.
 *	@skb: buffer to copy
 *	@offset: offset in the buffer to start copying to
 *	@from: the copy source
 *	@len: amount of data to copy to buffer from iovec
 *
 *	Returns 0 or -EFAULT.
 */
int skb_copy_datagram_from_iter(struct sk_buff *skb, int offset,
				 struct iov_iter *from,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				 int len)
{
	int start = skb_headlen(skb);
	int i, copy = start - offset;
	struct sk_buff *frag_iter;

	/* Copy header. */
	if (copy > 0) {
		if (copy > len)
			copy = len;
<<<<<<< HEAD
		if (memcpy_fromiovecend(skb->data + offset, from, from_offset,
					copy))
=======
		if (copy_from_iter(skb->data + offset, copy, from) != copy)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto fault;
		if ((len -= copy) == 0)
			return 0;
		offset += copy;
<<<<<<< HEAD
		from_offset += copy;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* Copy paged appendix. Hmm... why does this look so complicated? */
	for (i = 0; i < skb_shinfo(skb)->nr_frags; i++) {
		int end;
		const skb_frag_t *frag = &skb_shinfo(skb)->frags[i];

		WARN_ON(start > offset + len);

		end = start + skb_frag_size(frag);
		if ((copy = end - offset) > 0) {
<<<<<<< HEAD
			int err;
			u8  *vaddr;
			struct page *page = skb_frag_page(frag);

			if (copy > len)
				copy = len;
			vaddr = kmap(page);
			err = memcpy_fromiovecend(vaddr + frag->page_offset +
						  offset - start,
						  from, from_offset, copy);
			kunmap(page);
			if (err)
=======
			size_t copied;

			if (copy > len)
				copy = len;
			copied = copy_page_from_iter(skb_frag_page(frag),
					  skb_frag_off(frag) + offset - start,
					  copy, from);
			if (copied != copy)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				goto fault;

			if (!(len -= copy))
				return 0;
			offset += copy;
<<<<<<< HEAD
			from_offset += copy;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		start = end;
	}

	skb_walk_frags(skb, frag_iter) {
		int end;

		WARN_ON(start > offset + len);

		end = start + frag_iter->len;
		if ((copy = end - offset) > 0) {
			if (copy > len)
				copy = len;
<<<<<<< HEAD
			if (skb_copy_datagram_from_iovec(frag_iter,
							 offset - start,
							 from,
							 from_offset,
							 copy))
				goto fault;
			if ((len -= copy) == 0)
				return 0;
			offset += copy;
			from_offset += copy;
		}
		start = end;
	}
	if (!len)
		return 0;

fault:
	return -EFAULT;
}
EXPORT_SYMBOL(skb_copy_datagram_from_iovec);

static int skb_copy_and_csum_datagram(const struct sk_buff *skb, int offset,
				      u8 __user *to, int len,
				      __wsum *csump)
{
	int start = skb_headlen(skb);
	int i, copy = start - offset;
	struct sk_buff *frag_iter;
	int pos = 0;

	/* Copy header. */
	if (copy > 0) {
		int err = 0;
		if (copy > len)
			copy = len;
		*csump = csum_and_copy_to_user(skb->data + offset, to, copy,
					       *csump, &err);
		if (err)
			goto fault;
		if ((len -= copy) == 0)
			return 0;
		offset += copy;
		to += copy;
		pos = copy;
	}

	for (i = 0; i < skb_shinfo(skb)->nr_frags; i++) {
		int end;
		const skb_frag_t *frag = &skb_shinfo(skb)->frags[i];

		WARN_ON(start > offset + len);

		end = start + skb_frag_size(frag);
		if ((copy = end - offset) > 0) {
			__wsum csum2;
			int err = 0;
			u8  *vaddr;
			struct page *page = skb_frag_page(frag);

			if (copy > len)
				copy = len;
			vaddr = kmap(page);
			csum2 = csum_and_copy_to_user(vaddr +
							frag->page_offset +
							offset - start,
						      to, copy, 0, &err);
			kunmap(page);
			if (err)
				goto fault;
			*csump = csum_block_add(*csump, csum2, pos);
			if (!(len -= copy))
				return 0;
			offset += copy;
			to += copy;
			pos += copy;
		}
		start = end;
	}

	skb_walk_frags(skb, frag_iter) {
		int end;

		WARN_ON(start > offset + len);

		end = start + frag_iter->len;
		if ((copy = end - offset) > 0) {
			__wsum csum2 = 0;
			if (copy > len)
				copy = len;
			if (skb_copy_and_csum_datagram(frag_iter,
						       offset - start,
						       to, copy,
						       &csum2))
				goto fault;
			*csump = csum_block_add(*csump, csum2, pos);
			if ((len -= copy) == 0)
				return 0;
			offset += copy;
			to += copy;
			pos += copy;
=======
			if (skb_copy_datagram_from_iter(frag_iter,
							offset - start,
							from, copy))
				goto fault;
			if ((len -= copy) == 0)
				return 0;
			offset += copy;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		start = end;
	}
	if (!len)
		return 0;

fault:
	return -EFAULT;
}
<<<<<<< HEAD

__sum16 __skb_checksum_complete_head(struct sk_buff *skb, int len)
{
	__sum16 sum;

	sum = csum_fold(skb_checksum(skb, 0, len, skb->csum));
	if (likely(!sum)) {
		if (unlikely(skb->ip_summed == CHECKSUM_COMPLETE))
			netdev_rx_csum_fault(skb->dev);
		if (!skb_shared(skb))
			skb->ip_summed = CHECKSUM_UNNECESSARY;
	}
	return sum;
}
EXPORT_SYMBOL(__skb_checksum_complete_head);

__sum16 __skb_checksum_complete(struct sk_buff *skb)
{
	return __skb_checksum_complete_head(skb, skb->len);
}
EXPORT_SYMBOL(__skb_checksum_complete);

/**
 *	skb_copy_and_csum_datagram_iovec - Copy and checkum skb to user iovec.
 *	@skb: skbuff
 *	@hlen: hardware length
 *	@iov: io vector
 *	@len: amount of data to copy from skb to iov
=======
EXPORT_SYMBOL(skb_copy_datagram_from_iter);

int __zerocopy_sg_from_iter(struct msghdr *msg, struct sock *sk,
			    struct sk_buff *skb, struct iov_iter *from,
			    size_t length)
{
	int frag;

	if (msg && msg->msg_ubuf && msg->sg_from_iter)
		return msg->sg_from_iter(sk, skb, from, length);

	frag = skb_shinfo(skb)->nr_frags;

	while (length && iov_iter_count(from)) {
		struct page *head, *last_head = NULL;
		struct page *pages[MAX_SKB_FRAGS];
		int refs, order, n = 0;
		size_t start;
		ssize_t copied;
		unsigned long truesize;

		if (frag == MAX_SKB_FRAGS)
			return -EMSGSIZE;

		copied = iov_iter_get_pages2(from, pages, length,
					    MAX_SKB_FRAGS - frag, &start);
		if (copied < 0)
			return -EFAULT;

		length -= copied;

		truesize = PAGE_ALIGN(copied + start);
		skb->data_len += copied;
		skb->len += copied;
		skb->truesize += truesize;
		if (sk && sk->sk_type == SOCK_STREAM) {
			sk_wmem_queued_add(sk, truesize);
			if (!skb_zcopy_pure(skb))
				sk_mem_charge(sk, truesize);
		} else {
			refcount_add(truesize, &skb->sk->sk_wmem_alloc);
		}

		head = compound_head(pages[n]);
		order = compound_order(head);

		for (refs = 0; copied != 0; start = 0) {
			int size = min_t(int, copied, PAGE_SIZE - start);

			if (pages[n] - head > (1UL << order) - 1) {
				head = compound_head(pages[n]);
				order = compound_order(head);
			}

			start += (pages[n] - head) << PAGE_SHIFT;
			copied -= size;
			n++;
			if (frag) {
				skb_frag_t *last = &skb_shinfo(skb)->frags[frag - 1];

				if (head == skb_frag_page(last) &&
				    start == skb_frag_off(last) + skb_frag_size(last)) {
					skb_frag_size_add(last, size);
					/* We combined this page, we need to release
					 * a reference. Since compound pages refcount
					 * is shared among many pages, batch the refcount
					 * adjustments to limit false sharing.
					 */
					last_head = head;
					refs++;
					continue;
				}
			}
			if (refs) {
				page_ref_sub(last_head, refs);
				refs = 0;
			}
			skb_fill_page_desc_noacc(skb, frag++, head, start, size);
		}
		if (refs)
			page_ref_sub(last_head, refs);
	}
	return 0;
}
EXPORT_SYMBOL(__zerocopy_sg_from_iter);

/**
 *	zerocopy_sg_from_iter - Build a zerocopy datagram from an iov_iter
 *	@skb: buffer to copy
 *	@from: the source to copy from
 *
 *	The function will first copy up to headlen, and then pin the userspace
 *	pages and build frags through them.
 *
 *	Returns 0, -EFAULT or -EMSGSIZE.
 */
int zerocopy_sg_from_iter(struct sk_buff *skb, struct iov_iter *from)
{
	int copy = min_t(int, skb_headlen(skb), iov_iter_count(from));

	/* copy up to skb headlen */
	if (skb_copy_datagram_from_iter(skb, 0, from, copy))
		return -EFAULT;

	return __zerocopy_sg_from_iter(NULL, NULL, skb, from, ~0U);
}
EXPORT_SYMBOL(zerocopy_sg_from_iter);

static __always_inline
size_t copy_to_user_iter_csum(void __user *iter_to, size_t progress,
			      size_t len, void *from, void *priv2)
{
	__wsum next, *csum = priv2;

	next = csum_and_copy_to_user(from + progress, iter_to, len);
	*csum = csum_block_add(*csum, next, progress);
	return next ? 0 : len;
}

static __always_inline
size_t memcpy_to_iter_csum(void *iter_to, size_t progress,
			   size_t len, void *from, void *priv2)
{
	__wsum *csum = priv2;
	__wsum next = csum_partial_copy_nocheck(from + progress, iter_to, len);

	*csum = csum_block_add(*csum, next, progress);
	return 0;
}

struct csum_state {
	__wsum csum;
	size_t off;
};

static size_t csum_and_copy_to_iter(const void *addr, size_t bytes, void *_csstate,
				    struct iov_iter *i)
{
	struct csum_state *csstate = _csstate;
	__wsum sum;

	if (WARN_ON_ONCE(i->data_source))
		return 0;
	if (unlikely(iov_iter_is_discard(i))) {
		// can't use csum_memcpy() for that one - data is not copied
		csstate->csum = csum_block_add(csstate->csum,
					       csum_partial(addr, bytes, 0),
					       csstate->off);
		csstate->off += bytes;
		return bytes;
	}

	sum = csum_shift(csstate->csum, csstate->off);

	bytes = iterate_and_advance2(i, bytes, (void *)addr, &sum,
				     copy_to_user_iter_csum,
				     memcpy_to_iter_csum);
	csstate->csum = csum_shift(sum, csstate->off);
	csstate->off += bytes;
	return bytes;
}

/**
 *	skb_copy_and_csum_datagram - Copy datagram to an iovec iterator
 *          and update a checksum.
 *	@skb: buffer to copy
 *	@offset: offset in the buffer to start copying from
 *	@to: iovec iterator to copy to
 *	@len: amount of data to copy from buffer to iovec
 *      @csump: checksum pointer
 */
static int skb_copy_and_csum_datagram(const struct sk_buff *skb, int offset,
				      struct iov_iter *to, int len,
				      __wsum *csump)
{
	struct csum_state csdata = { .csum = *csump };
	int ret;

	ret = __skb_datagram_iter(skb, offset, to, len, true,
				  csum_and_copy_to_iter, &csdata);
	if (ret)
		return ret;

	*csump = csdata.csum;
	return 0;
}

/**
 *	skb_copy_and_csum_datagram_msg - Copy and checksum skb to user iovec.
 *	@skb: skbuff
 *	@hlen: hardware length
 *	@msg: destination
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 *	Caller _must_ check that skb will fit to this iovec.
 *
 *	Returns: 0       - success.
 *		 -EINVAL - checksum failure.
<<<<<<< HEAD
 *		 -EFAULT - fault during copy. Beware, in this case iovec
 *			   can be modified!
 */
int skb_copy_and_csum_datagram_iovec(struct sk_buff *skb,
				     int hlen, struct iovec *iov, int len)
=======
 *		 -EFAULT - fault during copy.
 */
int skb_copy_and_csum_datagram_msg(struct sk_buff *skb,
				   int hlen, struct msghdr *msg)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	__wsum csum;
	int chunk = skb->len - hlen;

<<<<<<< HEAD
	if (chunk > len)
		chunk = len;

	if (!chunk)
		return 0;

	/* Skip filled elements.
	 * Pretty silly, look at memcpy_toiovec, though 8)
	 */
	while (!iov->iov_len)
		iov++;

	if (iov->iov_len < chunk) {
		if (__skb_checksum_complete(skb))
			goto csum_error;
		if (skb_copy_datagram_iovec(skb, hlen, iov, chunk))
			goto fault;
	} else {
		csum = csum_partial(skb->data, hlen, skb->csum);
		if (skb_copy_and_csum_datagram(skb, hlen, iov->iov_base,
					       chunk, &csum))
			goto fault;
		if (csum_fold(csum))
			goto csum_error;
		if (unlikely(skb->ip_summed == CHECKSUM_COMPLETE))
			netdev_rx_csum_fault(skb->dev);
		iov->iov_len -= chunk;
		iov->iov_base += chunk;
	}
	return 0;
csum_error:
	return -EINVAL;
fault:
	return -EFAULT;
}
EXPORT_SYMBOL(skb_copy_and_csum_datagram_iovec);
=======
	if (!chunk)
		return 0;

	if (msg_data_left(msg) < chunk) {
		if (__skb_checksum_complete(skb))
			return -EINVAL;
		if (skb_copy_datagram_msg(skb, hlen, msg, chunk))
			goto fault;
	} else {
		csum = csum_partial(skb->data, hlen, skb->csum);
		if (skb_copy_and_csum_datagram(skb, hlen, &msg->msg_iter,
					       chunk, &csum))
			goto fault;

		if (csum_fold(csum)) {
			iov_iter_revert(&msg->msg_iter, chunk);
			return -EINVAL;
		}

		if (unlikely(skb->ip_summed == CHECKSUM_COMPLETE) &&
		    !skb->csum_complete_sw)
			netdev_rx_csum_fault(NULL, skb);
	}
	return 0;
fault:
	return -EFAULT;
}
EXPORT_SYMBOL(skb_copy_and_csum_datagram_msg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * 	datagram_poll - generic datagram poll
 *	@file: file struct
 *	@sock: socket
 *	@wait: poll table
 *
 *	Datagram poll: Again totally generic. This also handles
 *	sequenced packet sockets providing the socket receive queue
 *	is only ever holding data ready to receive.
 *
<<<<<<< HEAD
 *	Note: when you _don't_ use this routine for this protocol,
 *	and you use a different write policy from sock_writeable()
 *	then please supply your own write_space callback.
 */
unsigned int datagram_poll(struct file *file, struct socket *sock,
			   poll_table *wait)
{
	struct sock *sk = sock->sk;
	unsigned int mask;

	sock_poll_wait(file, sk_sleep(sk), wait);
	mask = 0;

	/* exceptional events? */
	if (sk->sk_err || !skb_queue_empty(&sk->sk_error_queue))
		mask |= POLLERR;
	if (sk->sk_shutdown & RCV_SHUTDOWN)
		mask |= POLLRDHUP | POLLIN | POLLRDNORM;
	if (sk->sk_shutdown == SHUTDOWN_MASK)
		mask |= POLLHUP;

	/* readable? */
	if (!skb_queue_empty(&sk->sk_receive_queue))
		mask |= POLLIN | POLLRDNORM;

	/* Connection-based need to check for termination and startup */
	if (connection_based(sk)) {
		if (sk->sk_state == TCP_CLOSE)
			mask |= POLLHUP;
		/* connection hasn't started yet? */
		if (sk->sk_state == TCP_SYN_SENT)
=======
 *	Note: when you *don't* use this routine for this protocol,
 *	and you use a different write policy from sock_writeable()
 *	then please supply your own write_space callback.
 */
__poll_t datagram_poll(struct file *file, struct socket *sock,
			   poll_table *wait)
{
	struct sock *sk = sock->sk;
	__poll_t mask;
	u8 shutdown;

	sock_poll_wait(file, sock, wait);
	mask = 0;

	/* exceptional events? */
	if (READ_ONCE(sk->sk_err) ||
	    !skb_queue_empty_lockless(&sk->sk_error_queue))
		mask |= EPOLLERR |
			(sock_flag(sk, SOCK_SELECT_ERR_QUEUE) ? EPOLLPRI : 0);

	shutdown = READ_ONCE(sk->sk_shutdown);
	if (shutdown & RCV_SHUTDOWN)
		mask |= EPOLLRDHUP | EPOLLIN | EPOLLRDNORM;
	if (shutdown == SHUTDOWN_MASK)
		mask |= EPOLLHUP;

	/* readable? */
	if (!skb_queue_empty_lockless(&sk->sk_receive_queue))
		mask |= EPOLLIN | EPOLLRDNORM;

	/* Connection-based need to check for termination and startup */
	if (connection_based(sk)) {
		int state = READ_ONCE(sk->sk_state);

		if (state == TCP_CLOSE)
			mask |= EPOLLHUP;
		/* connection hasn't started yet? */
		if (state == TCP_SYN_SENT)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return mask;
	}

	/* writable? */
	if (sock_writeable(sk))
<<<<<<< HEAD
		mask |= POLLOUT | POLLWRNORM | POLLWRBAND;
	else
		set_bit(SOCK_ASYNC_NOSPACE, &sk->sk_socket->flags);
=======
		mask |= EPOLLOUT | EPOLLWRNORM | EPOLLWRBAND;
	else
		sk_set_bit(SOCKWQ_ASYNC_NOSPACE, sk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return mask;
}
EXPORT_SYMBOL(datagram_poll);
