<<<<<<< HEAD
/*
 * raw.c - Raw sockets for protocol family CAN
=======
// SPDX-License-Identifier: (GPL-2.0 OR BSD-3-Clause)
/* raw.c - Raw sockets for protocol family CAN
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Copyright (c) 2002-2007 Volkswagen Group Electronic Research
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of Volkswagen nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * Alternatively, provided that this notice is retained in full, this
 * software may be distributed under the terms of the GNU General
 * Public License ("GPL") version 2, in which case the provisions of the
 * GPL apply INSTEAD OF those given above.
 *
 * The provided data structures and external interfaces from this code
 * are not restricted to be used by modules with a GPL compatible license.
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
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/uio.h>
#include <linux/net.h>
#include <linux/slab.h>
#include <linux/netdevice.h>
#include <linux/socket.h>
#include <linux/if_arp.h>
#include <linux/skbuff.h>
#include <linux/can.h>
#include <linux/can/core.h>
<<<<<<< HEAD
=======
#include <linux/can/dev.h> /* for can_is_canxl_dev_mtu() */
#include <linux/can/skb.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/can/raw.h>
#include <net/sock.h>
#include <net/net_namespace.h>

<<<<<<< HEAD
#define CAN_RAW_VERSION CAN_VERSION
static __initdata const char banner[] =
	KERN_INFO "can: raw protocol (rev " CAN_RAW_VERSION ")\n";

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_DESCRIPTION("PF_CAN raw protocol");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Urs Thuermann <urs.thuermann@volkswagen.de>");
MODULE_ALIAS("can-proto-1");

<<<<<<< HEAD
#define MASK_ALL 0

/*
 * A raw socket has a list of can_filters attached to it, each receiving
=======
#define RAW_MIN_NAMELEN CAN_REQUIRED_SIZE(struct sockaddr_can, can_ifindex)

#define MASK_ALL 0

/* A raw socket has a list of can_filters attached to it, each receiving
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * the CAN frames matching that filter.  If the filter list is empty,
 * no CAN frames will be received by the socket.  The default after
 * opening the socket, is to have one filter which receives all frames.
 * The filter list is allocated dynamically with the exception of the
 * list containing only one item.  This common case is optimized by
 * storing the single filter in dfilter, to avoid using dynamic memory.
 */

<<<<<<< HEAD
=======
struct uniqframe {
	int skbcnt;
	const struct sk_buff *skb;
	unsigned int join_rx_count;
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct raw_sock {
	struct sock sk;
	int bound;
	int ifindex;
<<<<<<< HEAD
	struct notifier_block notifier;
	int loopback;
	int recv_own_msgs;
=======
	struct net_device *dev;
	netdevice_tracker dev_tracker;
	struct list_head notifier;
	int loopback;
	int recv_own_msgs;
	int fd_frames;
	int xl_frames;
	struct can_raw_vcid_options raw_vcid_opts;
	canid_t tx_vcid_shifted;
	canid_t rx_vcid_shifted;
	canid_t rx_vcid_mask_shifted;
	int join_filters;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int count;                 /* number of active filters */
	struct can_filter dfilter; /* default/single filter */
	struct can_filter *filter; /* pointer to filter(s) */
	can_err_mask_t err_mask;
<<<<<<< HEAD
};

/*
 * Return pointer to store the extra msg flags for raw_recvmsg().
=======
	struct uniqframe __percpu *uniq;
};

static LIST_HEAD(raw_notifier_list);
static DEFINE_SPINLOCK(raw_notifier_lock);
static struct raw_sock *raw_busy_notifier;

/* Return pointer to store the extra msg flags for raw_recvmsg().
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * We use the space of one unsigned int beyond the 'struct sockaddr_can'
 * in skb->cb.
 */
static inline unsigned int *raw_flags(struct sk_buff *skb)
{
<<<<<<< HEAD
	BUILD_BUG_ON(sizeof(skb->cb) <= (sizeof(struct sockaddr_can) +
					 sizeof(unsigned int)));
=======
	sock_skb_cb_check_size(sizeof(struct sockaddr_can) +
			       sizeof(unsigned int));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* return pointer after struct sockaddr_can */
	return (unsigned int *)(&((struct sockaddr_can *)skb->cb)[1]);
}

static inline struct raw_sock *raw_sk(const struct sock *sk)
{
	return (struct raw_sock *)sk;
}

static void raw_rcv(struct sk_buff *oskb, void *data)
{
	struct sock *sk = (struct sock *)data;
	struct raw_sock *ro = raw_sk(sk);
	struct sockaddr_can *addr;
	struct sk_buff *skb;
	unsigned int *pflags;

	/* check the received tx sock reference */
	if (!ro->recv_own_msgs && oskb->sk == sk)
		return;

<<<<<<< HEAD
=======
	/* make sure to not pass oversized frames to the socket */
	if (!ro->fd_frames && can_is_canfd_skb(oskb))
		return;

	if (can_is_canxl_skb(oskb)) {
		struct canxl_frame *cxl = (struct canxl_frame *)oskb->data;

		/* make sure to not pass oversized frames to the socket */
		if (!ro->xl_frames)
			return;

		/* filter CAN XL VCID content */
		if (ro->raw_vcid_opts.flags & CAN_RAW_XL_VCID_RX_FILTER) {
			/* apply VCID filter if user enabled the filter */
			if ((cxl->prio & ro->rx_vcid_mask_shifted) !=
			    (ro->rx_vcid_shifted & ro->rx_vcid_mask_shifted))
				return;
		} else {
			/* no filter => do not forward VCID tagged frames */
			if (cxl->prio & CANXL_VCID_MASK)
				return;
		}
	}

	/* eliminate multiple filter matches for the same skb */
	if (this_cpu_ptr(ro->uniq)->skb == oskb &&
	    this_cpu_ptr(ro->uniq)->skbcnt == can_skb_prv(oskb)->skbcnt) {
		if (!ro->join_filters)
			return;

		this_cpu_inc(ro->uniq->join_rx_count);
		/* drop frame until all enabled filters matched */
		if (this_cpu_ptr(ro->uniq)->join_rx_count < ro->count)
			return;
	} else {
		this_cpu_ptr(ro->uniq)->skb = oskb;
		this_cpu_ptr(ro->uniq)->skbcnt = can_skb_prv(oskb)->skbcnt;
		this_cpu_ptr(ro->uniq)->join_rx_count = 1;
		/* drop first frame to check all enabled filters? */
		if (ro->join_filters && ro->count > 1)
			return;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* clone the given skb to be able to enqueue it into the rcv queue */
	skb = skb_clone(oskb, GFP_ATOMIC);
	if (!skb)
		return;

<<<<<<< HEAD
	/*
	 *  Put the datagram to the queue so that raw_recvmsg() can
	 *  get it from there.  We need to pass the interface index to
	 *  raw_recvmsg().  We pass a whole struct sockaddr_can in skb->cb
	 *  containing the interface index.
	 */

	BUILD_BUG_ON(sizeof(skb->cb) < sizeof(struct sockaddr_can));
	addr = (struct sockaddr_can *)skb->cb;
	memset(addr, 0, sizeof(*addr));
	addr->can_family  = AF_CAN;
=======
	/* Put the datagram to the queue so that raw_recvmsg() can get
	 * it from there. We need to pass the interface index to
	 * raw_recvmsg(). We pass a whole struct sockaddr_can in
	 * skb->cb containing the interface index.
	 */

	sock_skb_cb_check_size(sizeof(struct sockaddr_can));
	addr = (struct sockaddr_can *)skb->cb;
	memset(addr, 0, sizeof(*addr));
	addr->can_family = AF_CAN;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	addr->can_ifindex = skb->dev->ifindex;

	/* add CAN specific message flags for raw_recvmsg() */
	pflags = raw_flags(skb);
	*pflags = 0;
	if (oskb->sk)
		*pflags |= MSG_DONTROUTE;
	if (oskb->sk == sk)
		*pflags |= MSG_CONFIRM;

	if (sock_queue_rcv_skb(sk, skb) < 0)
		kfree_skb(skb);
}

<<<<<<< HEAD
static int raw_enable_filters(struct net_device *dev, struct sock *sk,
			      struct can_filter *filter, int count)
=======
static int raw_enable_filters(struct net *net, struct net_device *dev,
			      struct sock *sk, struct can_filter *filter,
			      int count)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int err = 0;
	int i;

	for (i = 0; i < count; i++) {
<<<<<<< HEAD
		err = can_rx_register(dev, filter[i].can_id,
				      filter[i].can_mask,
				      raw_rcv, sk, "raw");
		if (err) {
			/* clean up successfully registered filters */
			while (--i >= 0)
				can_rx_unregister(dev, filter[i].can_id,
=======
		err = can_rx_register(net, dev, filter[i].can_id,
				      filter[i].can_mask,
				      raw_rcv, sk, "raw", sk);
		if (err) {
			/* clean up successfully registered filters */
			while (--i >= 0)
				can_rx_unregister(net, dev, filter[i].can_id,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
						  filter[i].can_mask,
						  raw_rcv, sk);
			break;
		}
	}

	return err;
}

<<<<<<< HEAD
static int raw_enable_errfilter(struct net_device *dev, struct sock *sk,
				can_err_mask_t err_mask)
=======
static int raw_enable_errfilter(struct net *net, struct net_device *dev,
				struct sock *sk, can_err_mask_t err_mask)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int err = 0;

	if (err_mask)
<<<<<<< HEAD
		err = can_rx_register(dev, 0, err_mask | CAN_ERR_FLAG,
				      raw_rcv, sk, "raw");
=======
		err = can_rx_register(net, dev, 0, err_mask | CAN_ERR_FLAG,
				      raw_rcv, sk, "raw", sk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return err;
}

<<<<<<< HEAD
static void raw_disable_filters(struct net_device *dev, struct sock *sk,
			      struct can_filter *filter, int count)
=======
static void raw_disable_filters(struct net *net, struct net_device *dev,
				struct sock *sk, struct can_filter *filter,
				int count)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int i;

	for (i = 0; i < count; i++)
<<<<<<< HEAD
		can_rx_unregister(dev, filter[i].can_id, filter[i].can_mask,
				  raw_rcv, sk);
}

static inline void raw_disable_errfilter(struct net_device *dev,
=======
		can_rx_unregister(net, dev, filter[i].can_id,
				  filter[i].can_mask, raw_rcv, sk);
}

static inline void raw_disable_errfilter(struct net *net,
					 struct net_device *dev,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					 struct sock *sk,
					 can_err_mask_t err_mask)

{
	if (err_mask)
<<<<<<< HEAD
		can_rx_unregister(dev, 0, err_mask | CAN_ERR_FLAG,
				  raw_rcv, sk);
}

static inline void raw_disable_allfilters(struct net_device *dev,
=======
		can_rx_unregister(net, dev, 0, err_mask | CAN_ERR_FLAG,
				  raw_rcv, sk);
}

static inline void raw_disable_allfilters(struct net *net,
					  struct net_device *dev,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					  struct sock *sk)
{
	struct raw_sock *ro = raw_sk(sk);

<<<<<<< HEAD
	raw_disable_filters(dev, sk, ro->filter, ro->count);
	raw_disable_errfilter(dev, sk, ro->err_mask);
}

static int raw_enable_allfilters(struct net_device *dev, struct sock *sk)
=======
	raw_disable_filters(net, dev, sk, ro->filter, ro->count);
	raw_disable_errfilter(net, dev, sk, ro->err_mask);
}

static int raw_enable_allfilters(struct net *net, struct net_device *dev,
				 struct sock *sk)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct raw_sock *ro = raw_sk(sk);
	int err;

<<<<<<< HEAD
	err = raw_enable_filters(dev, sk, ro->filter, ro->count);
	if (!err) {
		err = raw_enable_errfilter(dev, sk, ro->err_mask);
		if (err)
			raw_disable_filters(dev, sk, ro->filter, ro->count);
=======
	err = raw_enable_filters(net, dev, sk, ro->filter, ro->count);
	if (!err) {
		err = raw_enable_errfilter(net, dev, sk, ro->err_mask);
		if (err)
			raw_disable_filters(net, dev, sk, ro->filter,
					    ro->count);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return err;
}

<<<<<<< HEAD
static int raw_notifier(struct notifier_block *nb,
			unsigned long msg, void *data)
{
	struct net_device *dev = (struct net_device *)data;
	struct raw_sock *ro = container_of(nb, struct raw_sock, notifier);
	struct sock *sk = &ro->sk;

	if (!net_eq(dev_net(dev), &init_net))
		return NOTIFY_DONE;

	if (dev->type != ARPHRD_CAN)
		return NOTIFY_DONE;

	if (ro->ifindex != dev->ifindex)
		return NOTIFY_DONE;

	switch (msg) {

	case NETDEV_UNREGISTER:
		lock_sock(sk);
		/* remove current filters & unregister */
		if (ro->bound)
			raw_disable_allfilters(dev, sk);
=======
static void raw_notify(struct raw_sock *ro, unsigned long msg,
		       struct net_device *dev)
{
	struct sock *sk = &ro->sk;

	if (!net_eq(dev_net(dev), sock_net(sk)))
		return;

	if (ro->dev != dev)
		return;

	switch (msg) {
	case NETDEV_UNREGISTER:
		lock_sock(sk);
		/* remove current filters & unregister */
		if (ro->bound) {
			raw_disable_allfilters(dev_net(dev), dev, sk);
			netdev_put(dev, &ro->dev_tracker);
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (ro->count > 1)
			kfree(ro->filter);

		ro->ifindex = 0;
<<<<<<< HEAD
		ro->bound   = 0;
		ro->count   = 0;
=======
		ro->bound = 0;
		ro->dev = NULL;
		ro->count = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		release_sock(sk);

		sk->sk_err = ENODEV;
		if (!sock_flag(sk, SOCK_DEAD))
<<<<<<< HEAD
			sk->sk_error_report(sk);
=======
			sk_error_report(sk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;

	case NETDEV_DOWN:
		sk->sk_err = ENETDOWN;
		if (!sock_flag(sk, SOCK_DEAD))
<<<<<<< HEAD
			sk->sk_error_report(sk);
		break;
	}

=======
			sk_error_report(sk);
		break;
	}
}

static int raw_notifier(struct notifier_block *nb, unsigned long msg,
			void *ptr)
{
	struct net_device *dev = netdev_notifier_info_to_dev(ptr);

	if (dev->type != ARPHRD_CAN)
		return NOTIFY_DONE;
	if (msg != NETDEV_UNREGISTER && msg != NETDEV_DOWN)
		return NOTIFY_DONE;
	if (unlikely(raw_busy_notifier)) /* Check for reentrant bug. */
		return NOTIFY_DONE;

	spin_lock(&raw_notifier_lock);
	list_for_each_entry(raw_busy_notifier, &raw_notifier_list, notifier) {
		spin_unlock(&raw_notifier_lock);
		raw_notify(raw_busy_notifier, msg, dev);
		spin_lock(&raw_notifier_lock);
	}
	raw_busy_notifier = NULL;
	spin_unlock(&raw_notifier_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return NOTIFY_DONE;
}

static int raw_init(struct sock *sk)
{
	struct raw_sock *ro = raw_sk(sk);

	ro->bound            = 0;
	ro->ifindex          = 0;
<<<<<<< HEAD
=======
	ro->dev              = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* set default filter to single entry dfilter */
	ro->dfilter.can_id   = 0;
	ro->dfilter.can_mask = MASK_ALL;
	ro->filter           = &ro->dfilter;
	ro->count            = 1;

	/* set default loopback behaviour */
	ro->loopback         = 1;
	ro->recv_own_msgs    = 0;
<<<<<<< HEAD

	/* set notifier */
	ro->notifier.notifier_call = raw_notifier;

	register_netdevice_notifier(&ro->notifier);
=======
	ro->fd_frames        = 0;
	ro->xl_frames        = 0;
	ro->join_filters     = 0;

	/* alloc_percpu provides zero'ed memory */
	ro->uniq = alloc_percpu(struct uniqframe);
	if (unlikely(!ro->uniq))
		return -ENOMEM;

	/* set notifier */
	spin_lock(&raw_notifier_lock);
	list_add_tail(&ro->notifier, &raw_notifier_list);
	spin_unlock(&raw_notifier_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static int raw_release(struct socket *sock)
{
	struct sock *sk = sock->sk;
	struct raw_sock *ro;

	if (!sk)
		return 0;

	ro = raw_sk(sk);

<<<<<<< HEAD
	unregister_netdevice_notifier(&ro->notifier);

=======
	spin_lock(&raw_notifier_lock);
	while (raw_busy_notifier == ro) {
		spin_unlock(&raw_notifier_lock);
		schedule_timeout_uninterruptible(1);
		spin_lock(&raw_notifier_lock);
	}
	list_del(&ro->notifier);
	spin_unlock(&raw_notifier_lock);

	rtnl_lock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	lock_sock(sk);

	/* remove current filters & unregister */
	if (ro->bound) {
<<<<<<< HEAD
		if (ro->ifindex) {
			struct net_device *dev;

			dev = dev_get_by_index(&init_net, ro->ifindex);
			if (dev) {
				raw_disable_allfilters(dev, sk);
				dev_put(dev);
			}
		} else
			raw_disable_allfilters(NULL, sk);
=======
		if (ro->dev) {
			raw_disable_allfilters(dev_net(ro->dev), ro->dev, sk);
			netdev_put(ro->dev, &ro->dev_tracker);
		} else {
			raw_disable_allfilters(sock_net(sk), NULL, sk);
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (ro->count > 1)
		kfree(ro->filter);

	ro->ifindex = 0;
<<<<<<< HEAD
	ro->bound   = 0;
	ro->count   = 0;
=======
	ro->bound = 0;
	ro->dev = NULL;
	ro->count = 0;
	free_percpu(ro->uniq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	sock_orphan(sk);
	sock->sk = NULL;

	release_sock(sk);
<<<<<<< HEAD
=======
	rtnl_unlock();

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	sock_put(sk);

	return 0;
}

static int raw_bind(struct socket *sock, struct sockaddr *uaddr, int len)
{
	struct sockaddr_can *addr = (struct sockaddr_can *)uaddr;
	struct sock *sk = sock->sk;
	struct raw_sock *ro = raw_sk(sk);
<<<<<<< HEAD
=======
	struct net_device *dev = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int ifindex;
	int err = 0;
	int notify_enetdown = 0;

<<<<<<< HEAD
	if (len < sizeof(*addr))
		return -EINVAL;

=======
	if (len < RAW_MIN_NAMELEN)
		return -EINVAL;
	if (addr->can_family != AF_CAN)
		return -EINVAL;

	rtnl_lock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	lock_sock(sk);

	if (ro->bound && addr->can_ifindex == ro->ifindex)
		goto out;

	if (addr->can_ifindex) {
<<<<<<< HEAD
		struct net_device *dev;

		dev = dev_get_by_index(&init_net, addr->can_ifindex);
=======
		dev = dev_get_by_index(sock_net(sk), addr->can_ifindex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!dev) {
			err = -ENODEV;
			goto out;
		}
		if (dev->type != ARPHRD_CAN) {
<<<<<<< HEAD
			dev_put(dev);
			err = -ENODEV;
			goto out;
		}
=======
			err = -ENODEV;
			goto out_put_dev;
		}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!(dev->flags & IFF_UP))
			notify_enetdown = 1;

		ifindex = dev->ifindex;

		/* filters set by default/setsockopt */
<<<<<<< HEAD
		err = raw_enable_allfilters(dev, sk);
		dev_put(dev);
=======
		err = raw_enable_allfilters(sock_net(sk), dev, sk);
		if (err)
			goto out_put_dev;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		ifindex = 0;

		/* filters set by default/setsockopt */
<<<<<<< HEAD
		err = raw_enable_allfilters(NULL, sk);
=======
		err = raw_enable_allfilters(sock_net(sk), NULL, sk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (!err) {
		if (ro->bound) {
			/* unregister old filters */
<<<<<<< HEAD
			if (ro->ifindex) {
				struct net_device *dev;

				dev = dev_get_by_index(&init_net, ro->ifindex);
				if (dev) {
					raw_disable_allfilters(dev, sk);
					dev_put(dev);
				}
			} else
				raw_disable_allfilters(NULL, sk);
		}
		ro->ifindex = ifindex;
		ro->bound = 1;
	}

 out:
	release_sock(sk);
=======
			if (ro->dev) {
				raw_disable_allfilters(dev_net(ro->dev),
						       ro->dev, sk);
				/* drop reference to old ro->dev */
				netdev_put(ro->dev, &ro->dev_tracker);
			} else {
				raw_disable_allfilters(sock_net(sk), NULL, sk);
			}
		}
		ro->ifindex = ifindex;
		ro->bound = 1;
		/* bind() ok -> hold a reference for new ro->dev */
		ro->dev = dev;
		if (ro->dev)
			netdev_hold(ro->dev, &ro->dev_tracker, GFP_KERNEL);
	}

out_put_dev:
	/* remove potential reference from dev_get_by_index() */
	dev_put(dev);
out:
	release_sock(sk);
	rtnl_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (notify_enetdown) {
		sk->sk_err = ENETDOWN;
		if (!sock_flag(sk, SOCK_DEAD))
<<<<<<< HEAD
			sk->sk_error_report(sk);
=======
			sk_error_report(sk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return err;
}

static int raw_getname(struct socket *sock, struct sockaddr *uaddr,
<<<<<<< HEAD
		       int *len, int peer)
=======
		       int peer)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct sockaddr_can *addr = (struct sockaddr_can *)uaddr;
	struct sock *sk = sock->sk;
	struct raw_sock *ro = raw_sk(sk);

	if (peer)
		return -EOPNOTSUPP;

<<<<<<< HEAD
	memset(addr, 0, sizeof(*addr));
	addr->can_family  = AF_CAN;
	addr->can_ifindex = ro->ifindex;

	*len = sizeof(*addr);

	return 0;
}

static int raw_setsockopt(struct socket *sock, int level, int optname,
			  char __user *optval, unsigned int optlen)
=======
	memset(addr, 0, RAW_MIN_NAMELEN);
	addr->can_family  = AF_CAN;
	addr->can_ifindex = ro->ifindex;

	return RAW_MIN_NAMELEN;
}

static int raw_setsockopt(struct socket *sock, int level, int optname,
			  sockptr_t optval, unsigned int optlen)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct sock *sk = sock->sk;
	struct raw_sock *ro = raw_sk(sk);
	struct can_filter *filter = NULL;  /* dyn. alloc'ed filters */
	struct can_filter sfilter;         /* single filter */
	struct net_device *dev = NULL;
	can_err_mask_t err_mask = 0;
<<<<<<< HEAD
=======
	int fd_frames;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int count = 0;
	int err = 0;

	if (level != SOL_CAN_RAW)
		return -EINVAL;

	switch (optname) {
<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case CAN_RAW_FILTER:
		if (optlen % sizeof(struct can_filter) != 0)
			return -EINVAL;

<<<<<<< HEAD
=======
		if (optlen > CAN_RAW_FILTER_MAX * sizeof(struct can_filter))
			return -EINVAL;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		count = optlen / sizeof(struct can_filter);

		if (count > 1) {
			/* filter does not fit into dfilter => alloc space */
<<<<<<< HEAD
			filter = memdup_user(optval, optlen);
			if (IS_ERR(filter))
				return PTR_ERR(filter);
		} else if (count == 1) {
			if (copy_from_user(&sfilter, optval, sizeof(sfilter)))
				return -EFAULT;
		}

		lock_sock(sk);

		if (ro->bound && ro->ifindex)
			dev = dev_get_by_index(&init_net, ro->ifindex);
=======
			filter = memdup_sockptr(optval, optlen);
			if (IS_ERR(filter))
				return PTR_ERR(filter);
		} else if (count == 1) {
			if (copy_from_sockptr(&sfilter, optval, sizeof(sfilter)))
				return -EFAULT;
		}

		rtnl_lock();
		lock_sock(sk);

		dev = ro->dev;
		if (ro->bound && dev) {
			if (dev->reg_state != NETREG_REGISTERED) {
				if (count > 1)
					kfree(filter);
				err = -ENODEV;
				goto out_fil;
			}
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (ro->bound) {
			/* (try to) register the new filters */
			if (count == 1)
<<<<<<< HEAD
				err = raw_enable_filters(dev, sk, &sfilter, 1);
			else
				err = raw_enable_filters(dev, sk, filter,
							 count);
=======
				err = raw_enable_filters(sock_net(sk), dev, sk,
							 &sfilter, 1);
			else
				err = raw_enable_filters(sock_net(sk), dev, sk,
							 filter, count);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (err) {
				if (count > 1)
					kfree(filter);
				goto out_fil;
			}

			/* remove old filter registrations */
<<<<<<< HEAD
			raw_disable_filters(dev, sk, ro->filter, ro->count);
=======
			raw_disable_filters(sock_net(sk), dev, sk, ro->filter,
					    ro->count);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}

		/* remove old filter space */
		if (ro->count > 1)
			kfree(ro->filter);

		/* link new filters to the socket */
		if (count == 1) {
			/* copy filter data for single filter */
			ro->dfilter = sfilter;
			filter = &ro->dfilter;
		}
		ro->filter = filter;
		ro->count  = count;

 out_fil:
<<<<<<< HEAD
		if (dev)
			dev_put(dev);

		release_sock(sk);
=======
		release_sock(sk);
		rtnl_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		break;

	case CAN_RAW_ERR_FILTER:
		if (optlen != sizeof(err_mask))
			return -EINVAL;

<<<<<<< HEAD
		if (copy_from_user(&err_mask, optval, optlen))
=======
		if (copy_from_sockptr(&err_mask, optval, optlen))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -EFAULT;

		err_mask &= CAN_ERR_MASK;

<<<<<<< HEAD
		lock_sock(sk);

		if (ro->bound && ro->ifindex)
			dev = dev_get_by_index(&init_net, ro->ifindex);
=======
		rtnl_lock();
		lock_sock(sk);

		dev = ro->dev;
		if (ro->bound && dev) {
			if (dev->reg_state != NETREG_REGISTERED) {
				err = -ENODEV;
				goto out_err;
			}
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/* remove current error mask */
		if (ro->bound) {
			/* (try to) register the new err_mask */
<<<<<<< HEAD
			err = raw_enable_errfilter(dev, sk, err_mask);
=======
			err = raw_enable_errfilter(sock_net(sk), dev, sk,
						   err_mask);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

			if (err)
				goto out_err;

			/* remove old err_mask registration */
<<<<<<< HEAD
			raw_disable_errfilter(dev, sk, ro->err_mask);
=======
			raw_disable_errfilter(sock_net(sk), dev, sk,
					      ro->err_mask);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}

		/* link new err_mask to the socket */
		ro->err_mask = err_mask;

 out_err:
<<<<<<< HEAD
		if (dev)
			dev_put(dev);

		release_sock(sk);
=======
		release_sock(sk);
		rtnl_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		break;

	case CAN_RAW_LOOPBACK:
		if (optlen != sizeof(ro->loopback))
			return -EINVAL;

<<<<<<< HEAD
		if (copy_from_user(&ro->loopback, optval, optlen))
=======
		if (copy_from_sockptr(&ro->loopback, optval, optlen))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -EFAULT;

		break;

	case CAN_RAW_RECV_OWN_MSGS:
		if (optlen != sizeof(ro->recv_own_msgs))
			return -EINVAL;

<<<<<<< HEAD
		if (copy_from_user(&ro->recv_own_msgs, optval, optlen))
=======
		if (copy_from_sockptr(&ro->recv_own_msgs, optval, optlen))
			return -EFAULT;

		break;

	case CAN_RAW_FD_FRAMES:
		if (optlen != sizeof(fd_frames))
			return -EINVAL;

		if (copy_from_sockptr(&fd_frames, optval, optlen))
			return -EFAULT;

		/* Enabling CAN XL includes CAN FD */
		if (ro->xl_frames && !fd_frames)
			return -EINVAL;

		ro->fd_frames = fd_frames;
		break;

	case CAN_RAW_XL_FRAMES:
		if (optlen != sizeof(ro->xl_frames))
			return -EINVAL;

		if (copy_from_sockptr(&ro->xl_frames, optval, optlen))
			return -EFAULT;

		/* Enabling CAN XL includes CAN FD */
		if (ro->xl_frames)
			ro->fd_frames = ro->xl_frames;
		break;

	case CAN_RAW_XL_VCID_OPTS:
		if (optlen != sizeof(ro->raw_vcid_opts))
			return -EINVAL;

		if (copy_from_sockptr(&ro->raw_vcid_opts, optval, optlen))
			return -EFAULT;

		/* prepare 32 bit values for handling in hot path */
		ro->tx_vcid_shifted = ro->raw_vcid_opts.tx_vcid << CANXL_VCID_OFFSET;
		ro->rx_vcid_shifted = ro->raw_vcid_opts.rx_vcid << CANXL_VCID_OFFSET;
		ro->rx_vcid_mask_shifted = ro->raw_vcid_opts.rx_vcid_mask << CANXL_VCID_OFFSET;
		break;

	case CAN_RAW_JOIN_FILTERS:
		if (optlen != sizeof(ro->join_filters))
			return -EINVAL;

		if (copy_from_sockptr(&ro->join_filters, optval, optlen))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -EFAULT;

		break;

	default:
		return -ENOPROTOOPT;
	}
	return err;
}

static int raw_getsockopt(struct socket *sock, int level, int optname,
			  char __user *optval, int __user *optlen)
{
	struct sock *sk = sock->sk;
	struct raw_sock *ro = raw_sk(sk);
	int len;
	void *val;
<<<<<<< HEAD
	int err = 0;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (level != SOL_CAN_RAW)
		return -EINVAL;
	if (get_user(len, optlen))
		return -EFAULT;
	if (len < 0)
		return -EINVAL;

	switch (optname) {
<<<<<<< HEAD

	case CAN_RAW_FILTER:
		lock_sock(sk);
		if (ro->count > 0) {
			int fsize = ro->count * sizeof(struct can_filter);
			if (len > fsize)
				len = fsize;
			if (copy_to_user(optval, ro->filter, len))
				err = -EFAULT;
		} else
			len = 0;
=======
	case CAN_RAW_FILTER: {
		int err = 0;

		lock_sock(sk);
		if (ro->count > 0) {
			int fsize = ro->count * sizeof(struct can_filter);

			/* user space buffer to small for filter list? */
			if (len < fsize) {
				/* return -ERANGE and needed space in optlen */
				err = -ERANGE;
				if (put_user(fsize, optlen))
					err = -EFAULT;
			} else {
				if (len > fsize)
					len = fsize;
				if (copy_to_user(optval, ro->filter, len))
					err = -EFAULT;
			}
		} else {
			len = 0;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		release_sock(sk);

		if (!err)
			err = put_user(len, optlen);
		return err;
<<<<<<< HEAD

=======
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case CAN_RAW_ERR_FILTER:
		if (len > sizeof(can_err_mask_t))
			len = sizeof(can_err_mask_t);
		val = &ro->err_mask;
		break;

	case CAN_RAW_LOOPBACK:
		if (len > sizeof(int))
			len = sizeof(int);
		val = &ro->loopback;
		break;

	case CAN_RAW_RECV_OWN_MSGS:
		if (len > sizeof(int))
			len = sizeof(int);
		val = &ro->recv_own_msgs;
		break;

<<<<<<< HEAD
=======
	case CAN_RAW_FD_FRAMES:
		if (len > sizeof(int))
			len = sizeof(int);
		val = &ro->fd_frames;
		break;

	case CAN_RAW_XL_FRAMES:
		if (len > sizeof(int))
			len = sizeof(int);
		val = &ro->xl_frames;
		break;

	case CAN_RAW_XL_VCID_OPTS: {
		int err = 0;

		/* user space buffer to small for VCID opts? */
		if (len < sizeof(ro->raw_vcid_opts)) {
			/* return -ERANGE and needed space in optlen */
			err = -ERANGE;
			if (put_user(sizeof(ro->raw_vcid_opts), optlen))
				err = -EFAULT;
		} else {
			if (len > sizeof(ro->raw_vcid_opts))
				len = sizeof(ro->raw_vcid_opts);
			if (copy_to_user(optval, &ro->raw_vcid_opts, len))
				err = -EFAULT;
		}
		if (!err)
			err = put_user(len, optlen);
		return err;
	}
	case CAN_RAW_JOIN_FILTERS:
		if (len > sizeof(int))
			len = sizeof(int);
		val = &ro->join_filters;
		break;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	default:
		return -ENOPROTOOPT;
	}

	if (put_user(len, optlen))
		return -EFAULT;
	if (copy_to_user(optval, val, len))
		return -EFAULT;
	return 0;
}

<<<<<<< HEAD
static int raw_sendmsg(struct kiocb *iocb, struct socket *sock,
		       struct msghdr *msg, size_t size)
{
	struct sock *sk = sock->sk;
	struct raw_sock *ro = raw_sk(sk);
	struct sk_buff *skb;
	struct net_device *dev;
	int ifindex;
	int err;

	if (msg->msg_name) {
		struct sockaddr_can *addr =
			(struct sockaddr_can *)msg->msg_name;

		if (msg->msg_namelen < sizeof(*addr))
=======
static void raw_put_canxl_vcid(struct raw_sock *ro, struct sk_buff *skb)
{
	struct canxl_frame *cxl = (struct canxl_frame *)skb->data;

	/* sanitize non CAN XL bits */
	cxl->prio &= (CANXL_PRIO_MASK | CANXL_VCID_MASK);

	/* clear VCID in CAN XL frame if pass through is disabled */
	if (!(ro->raw_vcid_opts.flags & CAN_RAW_XL_VCID_TX_PASS))
		cxl->prio &= CANXL_PRIO_MASK;

	/* set VCID in CAN XL frame if enabled */
	if (ro->raw_vcid_opts.flags & CAN_RAW_XL_VCID_TX_SET) {
		cxl->prio &= CANXL_PRIO_MASK;
		cxl->prio |= ro->tx_vcid_shifted;
	}
}

static unsigned int raw_check_txframe(struct raw_sock *ro, struct sk_buff *skb, int mtu)
{
	/* Classical CAN -> no checks for flags and device capabilities */
	if (can_is_can_skb(skb))
		return CAN_MTU;

	/* CAN FD -> needs to be enabled and a CAN FD or CAN XL device */
	if (ro->fd_frames && can_is_canfd_skb(skb) &&
	    (mtu == CANFD_MTU || can_is_canxl_dev_mtu(mtu)))
		return CANFD_MTU;

	/* CAN XL -> needs to be enabled and a CAN XL device */
	if (ro->xl_frames && can_is_canxl_skb(skb) &&
	    can_is_canxl_dev_mtu(mtu))
		return CANXL_MTU;

	return 0;
}

static int raw_sendmsg(struct socket *sock, struct msghdr *msg, size_t size)
{
	struct sock *sk = sock->sk;
	struct raw_sock *ro = raw_sk(sk);
	struct sockcm_cookie sockc;
	struct sk_buff *skb;
	struct net_device *dev;
	unsigned int txmtu;
	int ifindex;
	int err = -EINVAL;

	/* check for valid CAN frame sizes */
	if (size < CANXL_HDR_SIZE + CANXL_MIN_DLEN || size > CANXL_MTU)
		return -EINVAL;

	if (msg->msg_name) {
		DECLARE_SOCKADDR(struct sockaddr_can *, addr, msg->msg_name);

		if (msg->msg_namelen < RAW_MIN_NAMELEN)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -EINVAL;

		if (addr->can_family != AF_CAN)
			return -EINVAL;

		ifindex = addr->can_ifindex;
<<<<<<< HEAD
	} else
		ifindex = ro->ifindex;

	if (size != sizeof(struct can_frame))
		return -EINVAL;

	dev = dev_get_by_index(&init_net, ifindex);
	if (!dev)
		return -ENXIO;

	skb = sock_alloc_send_skb(sk, size, msg->msg_flags & MSG_DONTWAIT,
				  &err);
	if (!skb)
		goto put_dev;

	err = memcpy_fromiovec(skb_put(skb, size), msg->msg_iov, size);
	if (err < 0)
		goto free_skb;
	err = sock_tx_timestamp(sk, &skb_shinfo(skb)->tx_flags);
	if (err < 0)
		goto free_skb;

	skb->dev = dev;
	skb->sk  = sk;
=======
	} else {
		ifindex = ro->ifindex;
	}

	dev = dev_get_by_index(sock_net(sk), ifindex);
	if (!dev)
		return -ENXIO;

	skb = sock_alloc_send_skb(sk, size + sizeof(struct can_skb_priv),
				  msg->msg_flags & MSG_DONTWAIT, &err);
	if (!skb)
		goto put_dev;

	can_skb_reserve(skb);
	can_skb_prv(skb)->ifindex = dev->ifindex;
	can_skb_prv(skb)->skbcnt = 0;

	/* fill the skb before testing for valid CAN frames */
	err = memcpy_from_msg(skb_put(skb, size), msg, size);
	if (err < 0)
		goto free_skb;

	err = -EINVAL;

	/* check for valid CAN (CC/FD/XL) frame content */
	txmtu = raw_check_txframe(ro, skb, dev->mtu);
	if (!txmtu)
		goto free_skb;

	/* only CANXL: clear/forward/set VCID value */
	if (txmtu == CANXL_MTU)
		raw_put_canxl_vcid(ro, skb);

	sockcm_init(&sockc, sk);
	if (msg->msg_controllen) {
		err = sock_cmsg_send(sk, msg, &sockc);
		if (unlikely(err))
			goto free_skb;
	}

	skb->dev = dev;
	skb->priority = READ_ONCE(sk->sk_priority);
	skb->mark = READ_ONCE(sk->sk_mark);
	skb->tstamp = sockc.transmit_time;

	skb_setup_tx_timestamp(skb, sockc.tsflags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	err = can_send(skb, ro->loopback);

	dev_put(dev);

	if (err)
		goto send_failed;

	return size;

free_skb:
	kfree_skb(skb);
put_dev:
	dev_put(dev);
send_failed:
	return err;
}

<<<<<<< HEAD
static int raw_recvmsg(struct kiocb *iocb, struct socket *sock,
		       struct msghdr *msg, size_t size, int flags)
=======
static int raw_recvmsg(struct socket *sock, struct msghdr *msg, size_t size,
		       int flags)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct sock *sk = sock->sk;
	struct sk_buff *skb;
	int err = 0;
<<<<<<< HEAD
	int noblock;

	noblock =  flags & MSG_DONTWAIT;
	flags   &= ~MSG_DONTWAIT;

	skb = skb_recv_datagram(sk, flags, noblock, &err);
=======

	if (flags & MSG_ERRQUEUE)
		return sock_recv_errqueue(sk, msg, size,
					  SOL_CAN_RAW, SCM_CAN_RAW_ERRQUEUE);

	skb = skb_recv_datagram(sk, flags, &err);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!skb)
		return err;

	if (size < skb->len)
		msg->msg_flags |= MSG_TRUNC;
	else
		size = skb->len;

<<<<<<< HEAD
	err = memcpy_toiovec(msg->msg_iov, skb->data, size);
=======
	err = memcpy_to_msg(msg, skb->data, size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err < 0) {
		skb_free_datagram(sk, skb);
		return err;
	}

<<<<<<< HEAD
	sock_recv_ts_and_drops(msg, sk, skb);

	if (msg->msg_name) {
		msg->msg_namelen = sizeof(struct sockaddr_can);
=======
	sock_recv_cmsgs(msg, sk, skb);

	if (msg->msg_name) {
		__sockaddr_check_size(RAW_MIN_NAMELEN);
		msg->msg_namelen = RAW_MIN_NAMELEN;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		memcpy(msg->msg_name, skb->cb, msg->msg_namelen);
	}

	/* assign the flags that have been recorded in raw_rcv() */
	msg->msg_flags |= *(raw_flags(skb));

	skb_free_datagram(sk, skb);

	return size;
}

<<<<<<< HEAD
=======
static int raw_sock_no_ioctlcmd(struct socket *sock, unsigned int cmd,
				unsigned long arg)
{
	/* no ioctls for socket layer -> hand it down to NIC layer */
	return -ENOIOCTLCMD;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static const struct proto_ops raw_ops = {
	.family        = PF_CAN,
	.release       = raw_release,
	.bind          = raw_bind,
	.connect       = sock_no_connect,
	.socketpair    = sock_no_socketpair,
	.accept        = sock_no_accept,
	.getname       = raw_getname,
	.poll          = datagram_poll,
<<<<<<< HEAD
	.ioctl         = can_ioctl,	/* use can_ioctl() from af_can.c */
=======
	.ioctl         = raw_sock_no_ioctlcmd,
	.gettstamp     = sock_gettstamp,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.listen        = sock_no_listen,
	.shutdown      = sock_no_shutdown,
	.setsockopt    = raw_setsockopt,
	.getsockopt    = raw_getsockopt,
	.sendmsg       = raw_sendmsg,
	.recvmsg       = raw_recvmsg,
	.mmap          = sock_no_mmap,
<<<<<<< HEAD
	.sendpage      = sock_no_sendpage,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static struct proto raw_proto __read_mostly = {
	.name       = "CAN_RAW",
	.owner      = THIS_MODULE,
	.obj_size   = sizeof(struct raw_sock),
	.init       = raw_init,
};

static const struct can_proto raw_can_proto = {
	.type       = SOCK_RAW,
	.protocol   = CAN_RAW,
	.ops        = &raw_ops,
	.prot       = &raw_proto,
};

<<<<<<< HEAD
=======
static struct notifier_block canraw_notifier = {
	.notifier_call = raw_notifier
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static __init int raw_module_init(void)
{
	int err;

<<<<<<< HEAD
	printk(banner);

	err = can_proto_register(&raw_can_proto);
	if (err < 0)
		printk(KERN_ERR "can: registration of raw protocol failed\n");

=======
	pr_info("can: raw protocol\n");

	err = register_netdevice_notifier(&canraw_notifier);
	if (err)
		return err;

	err = can_proto_register(&raw_can_proto);
	if (err < 0) {
		pr_err("can: registration of raw protocol failed\n");
		goto register_proto_failed;
	}

	return 0;

register_proto_failed:
	unregister_netdevice_notifier(&canraw_notifier);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

static __exit void raw_module_exit(void)
{
	can_proto_unregister(&raw_can_proto);
<<<<<<< HEAD
=======
	unregister_netdevice_notifier(&canraw_notifier);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

module_init(raw_module_init);
module_exit(raw_module_exit);
