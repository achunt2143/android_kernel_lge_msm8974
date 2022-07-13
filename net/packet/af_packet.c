<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * INET		An implementation of the TCP/IP protocol suite for the LINUX
 *		operating system.  INET is implemented using the  BSD Socket
 *		interface as the means of communication with the user level.
 *
 *		PACKET - implements raw packet sockets.
 *
 * Authors:	Ross Biro
 *		Fred N. van Kempen, <waltje@uWalt.NL.Mugnet.ORG>
 *		Alan Cox, <gw4pts@gw4pts.ampr.org>
 *
 * Fixes:
 *		Alan Cox	:	verify_area() now used correctly
 *		Alan Cox	:	new skbuff lists, look ma no backlogs!
 *		Alan Cox	:	tidied skbuff lists.
 *		Alan Cox	:	Now uses generic datagram routines I
 *					added. Also fixed the peek/read crash
 *					from all old Linux datagram code.
 *		Alan Cox	:	Uses the improved datagram code.
 *		Alan Cox	:	Added NULL's for socket options.
 *		Alan Cox	:	Re-commented the code.
 *		Alan Cox	:	Use new kernel side addressing
 *		Rob Janssen	:	Correct MTU usage.
 *		Dave Platt	:	Counter leaks caused by incorrect
 *					interrupt locking and some slightly
 *					dubious gcc output. Can you read
 *					compiler: it said _VOLATILE_
 *	Richard Kooijman	:	Timestamp fixes.
 *		Alan Cox	:	New buffers. Use sk->mac.raw.
 *		Alan Cox	:	sendmsg/recvmsg support.
 *		Alan Cox	:	Protocol setting support
 *	Alexey Kuznetsov	:	Untied from IPv4 stack.
 *	Cyrus Durgin		:	Fixed kerneld for kmod.
 *	Michal Ostrowski        :       Module initialization cleanup.
 *         Ulises Alonso        :       Frame number limit removal and
 *                                      packet_set_ring memory leak.
 *		Eric Biederman	:	Allow for > 8 byte hardware addresses.
 *					The convention is that longer addresses
 *					will simply extend the hardware address
 *					byte arrays at the end of sockaddr_ll
 *					and packet_mreq.
 *		Johann Baudy	:	Added TX RING.
 *		Chetan Loke	:	Implemented TPACKET_V3 block abstraction
 *					layer.
 *					Copyright (C) 2011, <lokec@ccs.neu.edu>
<<<<<<< HEAD
 *
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
 */

=======
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/ethtool.h>
#include <linux/filter.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/types.h>
#include <linux/mm.h>
#include <linux/capability.h>
#include <linux/fcntl.h>
#include <linux/socket.h>
#include <linux/in.h>
#include <linux/inet.h>
#include <linux/netdevice.h>
#include <linux/if_packet.h>
#include <linux/wireless.h>
#include <linux/kernel.h>
#include <linux/kmod.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <net/net_namespace.h>
#include <net/ip.h>
#include <net/protocol.h>
#include <linux/skbuff.h>
#include <net/sock.h>
#include <linux/errno.h>
#include <linux/timer.h>
<<<<<<< HEAD
#include <asm/uaccess.h>
=======
#include <linux/uaccess.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/ioctls.h>
#include <asm/page.h>
#include <asm/cacheflush.h>
#include <asm/io.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/poll.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mutex.h>
#include <linux/if_vlan.h>
#include <linux/virtio_net.h>
#include <linux/errqueue.h>
#include <linux/net_tstamp.h>
<<<<<<< HEAD

#ifdef CONFIG_INET
#include <net/inet_common.h>
#endif

/*
   Assumptions:
   - if device has no dev->hard_header routine, it adds and removes ll header
     inside itself. In this case ll header is invisible outside of device,
     but higher levels still should reserve dev->hard_header_len.
     Some devices are enough clever to reallocate skb, when header
     will not fit to reserved space (tunnel), another ones are silly
     (PPP).
=======
#include <linux/percpu.h>
#ifdef CONFIG_INET
#include <net/inet_common.h>
#endif
#include <linux/bpf.h>
#include <net/compat.h>
#include <linux/netfilter_netdev.h>

#include "internal.h"

/*
   Assumptions:
   - If the device has no dev->header_ops->create, there is no LL header
     visible above the device. In this case, its hard_header_len should be 0.
     The device may prepend its own header internally. In this case, its
     needed_headroom should be set to the space needed for it to add its
     internal header.
     For example, a WiFi driver pretending to be an Ethernet driver should
     set its hard_header_len to be the Ethernet header length, and set its
     needed_headroom to be (the real WiFi header length - the fake Ethernet
     header length).
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
   - packet socket receives packets with pulled ll header,
     so that SOCK_RAW should push it back.

On receive:
-----------

<<<<<<< HEAD
Incoming, dev->hard_header!=NULL
   mac_header -> ll header
   data       -> data

Outgoing, dev->hard_header!=NULL
   mac_header -> ll header
   data       -> ll header

Incoming, dev->hard_header==NULL
   mac_header -> UNKNOWN position. It is very likely, that it points to ll
		 header.  PPP makes it, that is wrong, because introduce
		 assymetry between rx and tx paths.
   data       -> data

Outgoing, dev->hard_header==NULL
   mac_header -> data. ll header is still not built!
   data       -> data

Resume
  If dev->hard_header==NULL we are unlikely to restore sensible ll header.
=======
Incoming, dev_has_header(dev) == true
   mac_header -> ll header
   data       -> data

Outgoing, dev_has_header(dev) == true
   mac_header -> ll header
   data       -> ll header

Incoming, dev_has_header(dev) == false
   mac_header -> data
     However drivers often make it point to the ll header.
     This is incorrect because the ll header should be invisible to us.
   data       -> data

Outgoing, dev_has_header(dev) == false
   mac_header -> data. ll header is invisible to us.
   data       -> data

Resume
  If dev_has_header(dev) == false we are unable to restore the ll header,
    because it is invisible to us.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)


On transmit:
------------

<<<<<<< HEAD
dev->hard_header != NULL
   mac_header -> ll header
   data       -> ll header

dev->hard_header == NULL (ll header is added by device, we cannot control it)
   mac_header -> data
   data       -> data

   We should set nh.raw on output to correct posistion,
=======
dev_has_header(dev) == true
   mac_header -> ll header
   data       -> ll header

dev_has_header(dev) == false (ll header is invisible to us)
   mac_header -> data
   data       -> data

   We should set network_header on output to the correct position,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
   packet classifier depends on it.
 */

/* Private packet socket structures. */

<<<<<<< HEAD
struct packet_mclist {
	struct packet_mclist	*next;
	int			ifindex;
	int			count;
	unsigned short		type;
	unsigned short		alen;
	unsigned char		addr[MAX_ADDR_LEN];
};
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* identical to struct packet_mreq except it has
 * a longer address field.
 */
struct packet_mreq_max {
	int		mr_ifindex;
	unsigned short	mr_type;
	unsigned short	mr_alen;
	unsigned char	mr_address[MAX_ADDR_LEN];
};

<<<<<<< HEAD
static int packet_set_ring(struct sock *sk, union tpacket_req_u *req_u,
		int closing, int tx_ring);


=======
union tpacket_uhdr {
	struct tpacket_hdr  *h1;
	struct tpacket2_hdr *h2;
	struct tpacket3_hdr *h3;
	void *raw;
};

static int packet_set_ring(struct sock *sk, union tpacket_req_u *req_u,
		int closing, int tx_ring);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define V3_ALIGNMENT	(8)

#define BLK_HDR_LEN	(ALIGN(sizeof(struct tpacket_block_desc), V3_ALIGNMENT))

#define BLK_PLUS_PRIV(sz_of_priv) \
	(BLK_HDR_LEN + ALIGN((sz_of_priv), V3_ALIGNMENT))

<<<<<<< HEAD
/* kbdq - kernel block descriptor queue */
struct tpacket_kbdq_core {
	struct pgv	*pkbdq;
	unsigned int	feature_req_word;
	unsigned int	hdrlen;
	unsigned char	reset_pending_on_curr_blk;
	unsigned char   delete_blk_timer;
	unsigned short	kactive_blk_num;
	unsigned short	blk_sizeof_priv;

	/* last_kactive_blk_num:
	 * trick to see if user-space has caught up
	 * in order to avoid refreshing timer when every single pkt arrives.
	 */
	unsigned short	last_kactive_blk_num;

	char		*pkblk_start;
	char		*pkblk_end;
	int		kblk_size;
	unsigned int	knum_blocks;
	uint64_t	knxt_seq_num;
	char		*prev;
	char		*nxt_offset;
	struct sk_buff	*skb;

	atomic_t	blk_fill_in_prog;

	/* Default is set to 8ms */
#define DEFAULT_PRB_RETIRE_TOV	(8)

	unsigned short  retire_blk_tov;
	unsigned short  version;
	unsigned long	tov_in_jiffies;

	/* timer to retire an outstanding block */
	struct timer_list retire_blk_timer;
};

#define PGV_FROM_VMALLOC 1
struct pgv {
	char *buffer;
};

struct packet_ring_buffer {
	struct pgv		*pg_vec;
	unsigned int		head;
	unsigned int		frames_per_block;
	unsigned int		frame_size;
	unsigned int		frame_max;

	unsigned int		pg_vec_order;
	unsigned int		pg_vec_pages;
	unsigned int		pg_vec_len;

	struct tpacket_kbdq_core	prb_bdqc;
	atomic_t		pending;
};

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define BLOCK_STATUS(x)	((x)->hdr.bh1.block_status)
#define BLOCK_NUM_PKTS(x)	((x)->hdr.bh1.num_pkts)
#define BLOCK_O2FP(x)		((x)->hdr.bh1.offset_to_first_pkt)
#define BLOCK_LEN(x)		((x)->hdr.bh1.blk_len)
#define BLOCK_SNUM(x)		((x)->hdr.bh1.seq_num)
#define BLOCK_O2PRIV(x)	((x)->offset_to_priv)
<<<<<<< HEAD
#define BLOCK_PRIV(x)		((void *)((char *)(x) + BLOCK_O2PRIV(x)))

struct packet_sock;
static int tpacket_snd(struct packet_sock *po, struct msghdr *msg);
=======

struct packet_sock;
static int tpacket_rcv(struct sk_buff *skb, struct net_device *dev,
		       struct packet_type *pt, struct net_device *orig_dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static void *packet_previous_frame(struct packet_sock *po,
		struct packet_ring_buffer *rb,
		int status);
static void packet_increment_head(struct packet_ring_buffer *buff);
<<<<<<< HEAD
static int prb_curr_blk_in_use(struct tpacket_kbdq_core *,
			struct tpacket_block_desc *);
=======
static int prb_curr_blk_in_use(struct tpacket_block_desc *);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void *prb_dispatch_next_block(struct tpacket_kbdq_core *,
			struct packet_sock *);
static void prb_retire_current_block(struct tpacket_kbdq_core *,
		struct packet_sock *, unsigned int status);
static int prb_queue_frozen(struct tpacket_kbdq_core *);
static void prb_open_block(struct tpacket_kbdq_core *,
		struct tpacket_block_desc *);
<<<<<<< HEAD
static void prb_retire_rx_blk_timer_expired(unsigned long);
static void _prb_refresh_rx_retire_blk_timer(struct tpacket_kbdq_core *);
static void prb_init_blk_timer(struct packet_sock *,
		struct tpacket_kbdq_core *,
		void (*func) (unsigned long));
=======
static void prb_retire_rx_blk_timer_expired(struct timer_list *);
static void _prb_refresh_rx_retire_blk_timer(struct tpacket_kbdq_core *);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void prb_fill_rxhash(struct tpacket_kbdq_core *, struct tpacket3_hdr *);
static void prb_clear_rxhash(struct tpacket_kbdq_core *,
		struct tpacket3_hdr *);
static void prb_fill_vlan_info(struct tpacket_kbdq_core *,
		struct tpacket3_hdr *);
static void packet_flush_mclist(struct sock *sk);
<<<<<<< HEAD

struct packet_fanout;
struct packet_sock {
	/* struct sock has to be the first member of packet_sock */
	struct sock		sk;
	struct packet_fanout	*fanout;
	struct tpacket_stats	stats;
	union  tpacket_stats_u	stats_u;
	struct packet_ring_buffer	rx_ring;
	struct packet_ring_buffer	tx_ring;
	int			copy_thresh;
	spinlock_t		bind_lock;
	struct mutex		pg_vec_lock;
	unsigned int		running:1,	/* prot_hook is attached*/
				auxdata:1,
				origdev:1,
				has_vnet_hdr:1;
	int			ifindex;	/* bound device		*/
	__be16			num;
	struct packet_mclist	*mclist;
	atomic_t		mapped;
	enum tpacket_versions	tp_version;
	unsigned int		tp_hdrlen;
	unsigned int		tp_reserve;
	unsigned int		tp_loss:1;
	unsigned int		tp_tstamp;
	struct net_device __rcu	*cached_dev;
	struct packet_type	prot_hook ____cacheline_aligned_in_smp;
};

#define PACKET_FANOUT_MAX	256

struct packet_fanout {
#ifdef CONFIG_NET_NS
	struct net		*net;
#endif
	unsigned int		num_members;
	u16			id;
	u8			type;
	u8			defrag;
	atomic_t		rr_cur;
	struct list_head	list;
	struct sock		*arr[PACKET_FANOUT_MAX];
	spinlock_t		lock;
	atomic_t		sk_ref;
	struct packet_type	prot_hook ____cacheline_aligned_in_smp;
};

struct packet_skb_cb {
	unsigned int origlen;
	union {
		struct sockaddr_pkt pkt;
		struct sockaddr_ll ll;
	} sa;
};

=======
static u16 packet_pick_tx_queue(struct sk_buff *skb);

struct packet_skb_cb {
	union {
		struct sockaddr_pkt pkt;
		union {
			/* Trick: alias skb original length with
			 * ll.sll_family and ll.protocol in order
			 * to save room.
			 */
			unsigned int origlen;
			struct sockaddr_ll ll;
		};
	} sa;
};

#define vio_le() virtio_legacy_is_little_endian()

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define PACKET_SKB_CB(__skb)	((struct packet_skb_cb *)((__skb)->cb))

#define GET_PBDQC_FROM_RB(x)	((struct tpacket_kbdq_core *)(&(x)->prb_bdqc))
#define GET_PBLOCK_DESC(x, bid)	\
	((struct tpacket_block_desc *)((x)->pkbdq[(bid)].buffer))
#define GET_CURR_PBLOCK_DESC_FROM_CORE(x)	\
	((struct tpacket_block_desc *)((x)->pkbdq[(x)->kactive_blk_num].buffer))
#define GET_NEXT_PRB_BLK_NUM(x) \
	(((x)->kactive_blk_num < ((x)->knum_blocks-1)) ? \
	((x)->kactive_blk_num+1) : 0)

<<<<<<< HEAD
static struct packet_sock *pkt_sk(struct sock *sk)
{
	return (struct packet_sock *)sk;
}

static void __fanout_unlink(struct sock *sk, struct packet_sock *po);
static void __fanout_link(struct sock *sk, struct packet_sock *po);

/* register_prot_hook must be invoked with the po->bind_lock held,
 * or from a context in which asynchronous accesses to the packet
 * socket is not possible (packet_create()).
 */
static void register_prot_hook(struct sock *sk)
{
	struct packet_sock *po = pkt_sk(sk);

	if (!po->running) {
		if (po->fanout) {
			__fanout_link(sk, po);
		} else {
			dev_add_pack(&po->prot_hook);
			rcu_assign_pointer(po->cached_dev, po->prot_hook.dev);
		}

		sock_hold(sk);
		po->running = 1;
	}
}

/* {,__}unregister_prot_hook() must be invoked with the po->bind_lock
 * held.   If the sync parameter is true, we will temporarily drop
=======
static void __fanout_unlink(struct sock *sk, struct packet_sock *po);
static void __fanout_link(struct sock *sk, struct packet_sock *po);

#ifdef CONFIG_NETFILTER_EGRESS
static noinline struct sk_buff *nf_hook_direct_egress(struct sk_buff *skb)
{
	struct sk_buff *next, *head = NULL, *tail;
	int rc;

	rcu_read_lock();
	for (; skb != NULL; skb = next) {
		next = skb->next;
		skb_mark_not_on_list(skb);

		if (!nf_hook_egress(skb, &rc, skb->dev))
			continue;

		if (!head)
			head = skb;
		else
			tail->next = skb;

		tail = skb;
	}
	rcu_read_unlock();

	return head;
}
#endif

static int packet_xmit(const struct packet_sock *po, struct sk_buff *skb)
{
	if (!packet_sock_flag(po, PACKET_SOCK_QDISC_BYPASS))
		return dev_queue_xmit(skb);

#ifdef CONFIG_NETFILTER_EGRESS
	if (nf_hook_egress_active()) {
		skb = nf_hook_direct_egress(skb);
		if (!skb)
			return NET_XMIT_DROP;
	}
#endif
	return dev_direct_xmit(skb, packet_pick_tx_queue(skb));
}

static struct net_device *packet_cached_dev_get(struct packet_sock *po)
{
	struct net_device *dev;

	rcu_read_lock();
	dev = rcu_dereference(po->cached_dev);
	dev_hold(dev);
	rcu_read_unlock();

	return dev;
}

static void packet_cached_dev_assign(struct packet_sock *po,
				     struct net_device *dev)
{
	rcu_assign_pointer(po->cached_dev, dev);
}

static void packet_cached_dev_reset(struct packet_sock *po)
{
	RCU_INIT_POINTER(po->cached_dev, NULL);
}

static u16 packet_pick_tx_queue(struct sk_buff *skb)
{
	struct net_device *dev = skb->dev;
	const struct net_device_ops *ops = dev->netdev_ops;
	int cpu = raw_smp_processor_id();
	u16 queue_index;

#ifdef CONFIG_XPS
	skb->sender_cpu = cpu + 1;
#endif
	skb_record_rx_queue(skb, cpu % dev->real_num_tx_queues);
	if (ops->ndo_select_queue) {
		queue_index = ops->ndo_select_queue(dev, skb, NULL);
		queue_index = netdev_cap_txqueue(dev, queue_index);
	} else {
		queue_index = netdev_pick_tx(dev, skb, NULL);
	}

	return queue_index;
}

/* __register_prot_hook must be invoked through register_prot_hook
 * or from a context in which asynchronous accesses to the packet
 * socket is not possible (packet_create()).
 */
static void __register_prot_hook(struct sock *sk)
{
	struct packet_sock *po = pkt_sk(sk);

	if (!packet_sock_flag(po, PACKET_SOCK_RUNNING)) {
		if (po->fanout)
			__fanout_link(sk, po);
		else
			dev_add_pack(&po->prot_hook);

		sock_hold(sk);
		packet_sock_flag_set(po, PACKET_SOCK_RUNNING, 1);
	}
}

static void register_prot_hook(struct sock *sk)
{
	lockdep_assert_held_once(&pkt_sk(sk)->bind_lock);
	__register_prot_hook(sk);
}

/* If the sync parameter is true, we will temporarily drop
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * the po->bind_lock and do a synchronize_net to make sure no
 * asynchronous packet processing paths still refer to the elements
 * of po->prot_hook.  If the sync parameter is false, it is the
 * callers responsibility to take care of this.
 */
static void __unregister_prot_hook(struct sock *sk, bool sync)
{
	struct packet_sock *po = pkt_sk(sk);

<<<<<<< HEAD
	po->running = 0;
	if (po->fanout) {
		__fanout_unlink(sk, po);
	} else {
		__dev_remove_pack(&po->prot_hook);
		RCU_INIT_POINTER(po->cached_dev, NULL);
	}
=======
	lockdep_assert_held_once(&po->bind_lock);

	packet_sock_flag_set(po, PACKET_SOCK_RUNNING, 0);

	if (po->fanout)
		__fanout_unlink(sk, po);
	else
		__dev_remove_pack(&po->prot_hook);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	__sock_put(sk);

	if (sync) {
		spin_unlock(&po->bind_lock);
		synchronize_net();
		spin_lock(&po->bind_lock);
	}
}

static void unregister_prot_hook(struct sock *sk, bool sync)
{
	struct packet_sock *po = pkt_sk(sk);

<<<<<<< HEAD
	if (po->running)
		__unregister_prot_hook(sk, sync);
}

static inline __pure struct page *pgv_to_page(void *addr)
=======
	if (packet_sock_flag(po, PACKET_SOCK_RUNNING))
		__unregister_prot_hook(sk, sync);
}

static inline struct page * __pure pgv_to_page(void *addr)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	if (is_vmalloc_addr(addr))
		return vmalloc_to_page(addr);
	return virt_to_page(addr);
}

static void __packet_set_status(struct packet_sock *po, void *frame, int status)
{
<<<<<<< HEAD
	union {
		struct tpacket_hdr *h1;
		struct tpacket2_hdr *h2;
		void *raw;
	} h;
=======
	union tpacket_uhdr h;

	/* WRITE_ONCE() are paired with READ_ONCE() in __packet_get_status */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	h.raw = frame;
	switch (po->tp_version) {
	case TPACKET_V1:
<<<<<<< HEAD
		h.h1->tp_status = status;
		flush_dcache_page(pgv_to_page(&h.h1->tp_status));
		break;
	case TPACKET_V2:
		h.h2->tp_status = status;
		flush_dcache_page(pgv_to_page(&h.h2->tp_status));
		break;
	case TPACKET_V3:
=======
		WRITE_ONCE(h.h1->tp_status, status);
		flush_dcache_page(pgv_to_page(&h.h1->tp_status));
		break;
	case TPACKET_V2:
		WRITE_ONCE(h.h2->tp_status, status);
		flush_dcache_page(pgv_to_page(&h.h2->tp_status));
		break;
	case TPACKET_V3:
		WRITE_ONCE(h.h3->tp_status, status);
		flush_dcache_page(pgv_to_page(&h.h3->tp_status));
		break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	default:
		WARN(1, "TPACKET version not supported.\n");
		BUG();
	}

	smp_wmb();
}

<<<<<<< HEAD
static int __packet_get_status(struct packet_sock *po, void *frame)
{
	union {
		struct tpacket_hdr *h1;
		struct tpacket2_hdr *h2;
		void *raw;
	} h;

	smp_rmb();

=======
static int __packet_get_status(const struct packet_sock *po, void *frame)
{
	union tpacket_uhdr h;

	smp_rmb();

	/* READ_ONCE() are paired with WRITE_ONCE() in __packet_set_status */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	h.raw = frame;
	switch (po->tp_version) {
	case TPACKET_V1:
		flush_dcache_page(pgv_to_page(&h.h1->tp_status));
<<<<<<< HEAD
		return h.h1->tp_status;
	case TPACKET_V2:
		flush_dcache_page(pgv_to_page(&h.h2->tp_status));
		return h.h2->tp_status;
	case TPACKET_V3:
=======
		return READ_ONCE(h.h1->tp_status);
	case TPACKET_V2:
		flush_dcache_page(pgv_to_page(&h.h2->tp_status));
		return READ_ONCE(h.h2->tp_status);
	case TPACKET_V3:
		flush_dcache_page(pgv_to_page(&h.h3->tp_status));
		return READ_ONCE(h.h3->tp_status);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	default:
		WARN(1, "TPACKET version not supported.\n");
		BUG();
		return 0;
	}
}

<<<<<<< HEAD
static void *packet_lookup_frame(struct packet_sock *po,
		struct packet_ring_buffer *rb,
		unsigned int position,
		int status)
{
	unsigned int pg_vec_pos, frame_offset;
	union {
		struct tpacket_hdr *h1;
		struct tpacket2_hdr *h2;
		void *raw;
	} h;
=======
static __u32 tpacket_get_timestamp(struct sk_buff *skb, struct timespec64 *ts,
				   unsigned int flags)
{
	struct skb_shared_hwtstamps *shhwtstamps = skb_hwtstamps(skb);

	if (shhwtstamps &&
	    (flags & SOF_TIMESTAMPING_RAW_HARDWARE) &&
	    ktime_to_timespec64_cond(shhwtstamps->hwtstamp, ts))
		return TP_STATUS_TS_RAW_HARDWARE;

	if ((flags & SOF_TIMESTAMPING_SOFTWARE) &&
	    ktime_to_timespec64_cond(skb_tstamp(skb), ts))
		return TP_STATUS_TS_SOFTWARE;

	return 0;
}

static __u32 __packet_set_timestamp(struct packet_sock *po, void *frame,
				    struct sk_buff *skb)
{
	union tpacket_uhdr h;
	struct timespec64 ts;
	__u32 ts_status;

	if (!(ts_status = tpacket_get_timestamp(skb, &ts, READ_ONCE(po->tp_tstamp))))
		return 0;

	h.raw = frame;
	/*
	 * versions 1 through 3 overflow the timestamps in y2106, since they
	 * all store the seconds in a 32-bit unsigned integer.
	 * If we create a version 4, that should have a 64-bit timestamp,
	 * either 64-bit seconds + 32-bit nanoseconds, or just 64-bit
	 * nanoseconds.
	 */
	switch (po->tp_version) {
	case TPACKET_V1:
		h.h1->tp_sec = ts.tv_sec;
		h.h1->tp_usec = ts.tv_nsec / NSEC_PER_USEC;
		break;
	case TPACKET_V2:
		h.h2->tp_sec = ts.tv_sec;
		h.h2->tp_nsec = ts.tv_nsec;
		break;
	case TPACKET_V3:
		h.h3->tp_sec = ts.tv_sec;
		h.h3->tp_nsec = ts.tv_nsec;
		break;
	default:
		WARN(1, "TPACKET version not supported.\n");
		BUG();
	}

	/* one flush is safe, as both fields always lie on the same cacheline */
	flush_dcache_page(pgv_to_page(&h.h1->tp_sec));
	smp_wmb();

	return ts_status;
}

static void *packet_lookup_frame(const struct packet_sock *po,
				 const struct packet_ring_buffer *rb,
				 unsigned int position,
				 int status)
{
	unsigned int pg_vec_pos, frame_offset;
	union tpacket_uhdr h;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	pg_vec_pos = position / rb->frames_per_block;
	frame_offset = position % rb->frames_per_block;

	h.raw = rb->pg_vec[pg_vec_pos].buffer +
		(frame_offset * rb->frame_size);

	if (status != __packet_get_status(po, h.raw))
		return NULL;

	return h.raw;
}

static void *packet_current_frame(struct packet_sock *po,
		struct packet_ring_buffer *rb,
		int status)
{
	return packet_lookup_frame(po, rb, rb->head, status);
}

static void prb_del_retire_blk_timer(struct tpacket_kbdq_core *pkc)
{
	del_timer_sync(&pkc->retire_blk_timer);
}

static void prb_shutdown_retire_blk_timer(struct packet_sock *po,
<<<<<<< HEAD
		int tx_ring,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		struct sk_buff_head *rb_queue)
{
	struct tpacket_kbdq_core *pkc;

<<<<<<< HEAD
	pkc = tx_ring ? &po->tx_ring.prb_bdqc : &po->rx_ring.prb_bdqc;
=======
	pkc = GET_PBDQC_FROM_RB(&po->rx_ring);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spin_lock_bh(&rb_queue->lock);
	pkc->delete_blk_timer = 1;
	spin_unlock_bh(&rb_queue->lock);

	prb_del_retire_blk_timer(pkc);
}

<<<<<<< HEAD
static void prb_init_blk_timer(struct packet_sock *po,
		struct tpacket_kbdq_core *pkc,
		void (*func) (unsigned long))
{
	init_timer(&pkc->retire_blk_timer);
	pkc->retire_blk_timer.data = (long)po;
	pkc->retire_blk_timer.function = func;
	pkc->retire_blk_timer.expires = jiffies;
}

static void prb_setup_retire_blk_timer(struct packet_sock *po, int tx_ring)
{
	struct tpacket_kbdq_core *pkc;

	if (tx_ring)
		BUG();

	pkc = tx_ring ? &po->tx_ring.prb_bdqc : &po->rx_ring.prb_bdqc;
	prb_init_blk_timer(po, pkc, prb_retire_rx_blk_timer_expired);
=======
static void prb_setup_retire_blk_timer(struct packet_sock *po)
{
	struct tpacket_kbdq_core *pkc;

	pkc = GET_PBDQC_FROM_RB(&po->rx_ring);
	timer_setup(&pkc->retire_blk_timer, prb_retire_rx_blk_timer_expired,
		    0);
	pkc->retire_blk_timer.expires = jiffies;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int prb_calc_retire_blk_tmo(struct packet_sock *po,
				int blk_size_in_bytes)
{
	struct net_device *dev;
<<<<<<< HEAD
	unsigned int mbits = 0, msec = 0, div = 0, tmo = 0;
	struct ethtool_cmd ecmd;
=======
	unsigned int mbits, div;
	struct ethtool_link_ksettings ecmd;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int err;

	rtnl_lock();
	dev = __dev_get_by_index(sock_net(&po->sk), po->ifindex);
	if (unlikely(!dev)) {
		rtnl_unlock();
		return DEFAULT_PRB_RETIRE_TOV;
	}
<<<<<<< HEAD
	err = __ethtool_get_settings(dev, &ecmd);
	rtnl_unlock();
	if (!err) {
		switch (ecmd.speed) {
		case SPEED_10000:
			msec = 1;
			div = 10000/1000;
			break;
		case SPEED_1000:
			msec = 1;
			div = 1000/1000;
			break;
		/*
		 * If the link speed is so slow you don't really
		 * need to worry about perf anyways
		 */
		case SPEED_100:
		case SPEED_10:
		default:
			return DEFAULT_PRB_RETIRE_TOV;
		}
	}

=======
	err = __ethtool_get_link_ksettings(dev, &ecmd);
	rtnl_unlock();
	if (err)
		return DEFAULT_PRB_RETIRE_TOV;

	/* If the link speed is so slow you don't really
	 * need to worry about perf anyways
	 */
	if (ecmd.base.speed < SPEED_1000 ||
	    ecmd.base.speed == SPEED_UNKNOWN)
		return DEFAULT_PRB_RETIRE_TOV;

	div = ecmd.base.speed / 1000;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mbits = (blk_size_in_bytes * 8) / (1024 * 1024);

	if (div)
		mbits /= div;

<<<<<<< HEAD
	tmo = mbits * msec;

	if (div)
		return tmo+1;
	return tmo;
=======
	if (div)
		return mbits + 1;
	return mbits;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void prb_init_ft_ops(struct tpacket_kbdq_core *p1,
			union tpacket_req_u *req_u)
{
	p1->feature_req_word = req_u->req3.tp_feature_req_word;
}

static void init_prb_bdqc(struct packet_sock *po,
			struct packet_ring_buffer *rb,
			struct pgv *pg_vec,
<<<<<<< HEAD
			union tpacket_req_u *req_u, int tx_ring)
{
	struct tpacket_kbdq_core *p1 = &rb->prb_bdqc;
=======
			union tpacket_req_u *req_u)
{
	struct tpacket_kbdq_core *p1 = GET_PBDQC_FROM_RB(rb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct tpacket_block_desc *pbd;

	memset(p1, 0x0, sizeof(*p1));

	p1->knxt_seq_num = 1;
	p1->pkbdq = pg_vec;
	pbd = (struct tpacket_block_desc *)pg_vec[0].buffer;
<<<<<<< HEAD
	p1->pkblk_start	= (char *)pg_vec[0].buffer;
=======
	p1->pkblk_start	= pg_vec[0].buffer;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	p1->kblk_size = req_u->req3.tp_block_size;
	p1->knum_blocks	= req_u->req3.tp_block_nr;
	p1->hdrlen = po->tp_hdrlen;
	p1->version = po->tp_version;
	p1->last_kactive_blk_num = 0;
<<<<<<< HEAD
	po->stats_u.stats3.tp_freeze_q_cnt = 0;
=======
	po->stats.stats3.tp_freeze_q_cnt = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (req_u->req3.tp_retire_blk_tov)
		p1->retire_blk_tov = req_u->req3.tp_retire_blk_tov;
	else
		p1->retire_blk_tov = prb_calc_retire_blk_tmo(po,
						req_u->req3.tp_block_size);
	p1->tov_in_jiffies = msecs_to_jiffies(p1->retire_blk_tov);
	p1->blk_sizeof_priv = req_u->req3.tp_sizeof_priv;
<<<<<<< HEAD

	prb_init_ft_ops(p1, req_u);
	prb_setup_retire_blk_timer(po, tx_ring);
=======
	rwlock_init(&p1->blk_fill_in_prog_lock);

	p1->max_frame_len = p1->kblk_size - BLK_PLUS_PRIV(p1->blk_sizeof_priv);
	prb_init_ft_ops(p1, req_u);
	prb_setup_retire_blk_timer(po);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	prb_open_block(p1, pbd);
}

/*  Do NOT update the last_blk_num first.
 *  Assumes sk_buff_head lock is held.
 */
static void _prb_refresh_rx_retire_blk_timer(struct tpacket_kbdq_core *pkc)
{
	mod_timer(&pkc->retire_blk_timer,
			jiffies + pkc->tov_in_jiffies);
	pkc->last_kactive_blk_num = pkc->kactive_blk_num;
}

/*
 * Timer logic:
 * 1) We refresh the timer only when we open a block.
 *    By doing this we don't waste cycles refreshing the timer
 *	  on packet-by-packet basis.
 *
 * With a 1MB block-size, on a 1Gbps line, it will take
 * i) ~8 ms to fill a block + ii) memcpy etc.
 * In this cut we are not accounting for the memcpy time.
 *
 * So, if the user sets the 'tmo' to 10ms then the timer
 * will never fire while the block is still getting filled
 * (which is what we want). However, the user could choose
 * to close a block early and that's fine.
 *
 * But when the timer does fire, we check whether or not to refresh it.
 * Since the tmo granularity is in msecs, it is not too expensive
 * to refresh the timer, lets say every '8' msecs.
 * Either the user can set the 'tmo' or we can derive it based on
 * a) line-speed and b) block-size.
 * prb_calc_retire_blk_tmo() calculates the tmo.
 *
 */
<<<<<<< HEAD
static void prb_retire_rx_blk_timer_expired(unsigned long data)
{
	struct packet_sock *po = (struct packet_sock *)data;
	struct tpacket_kbdq_core *pkc = &po->rx_ring.prb_bdqc;
=======
static void prb_retire_rx_blk_timer_expired(struct timer_list *t)
{
	struct packet_sock *po =
		from_timer(po, t, rx_ring.prb_bdqc.retire_blk_timer);
	struct tpacket_kbdq_core *pkc = GET_PBDQC_FROM_RB(&po->rx_ring);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned int frozen;
	struct tpacket_block_desc *pbd;

	spin_lock(&po->sk.sk_receive_queue.lock);

	frozen = prb_queue_frozen(pkc);
	pbd = GET_CURR_PBLOCK_DESC_FROM_CORE(pkc);

	if (unlikely(pkc->delete_blk_timer))
		goto out;

	/* We only need to plug the race when the block is partially filled.
	 * tpacket_rcv:
	 *		lock(); increment BLOCK_NUM_PKTS; unlock()
	 *		copy_bits() is in progress ...
	 *		timer fires on other cpu:
	 *		we can't retire the current block because copy_bits
	 *		is in progress.
	 *
	 */
	if (BLOCK_NUM_PKTS(pbd)) {
<<<<<<< HEAD
		while (atomic_read(&pkc->blk_fill_in_prog)) {
			/* Waiting for skb_copy_bits to finish... */
			cpu_relax();
		}
=======
		/* Waiting for skb_copy_bits to finish... */
		write_lock(&pkc->blk_fill_in_prog_lock);
		write_unlock(&pkc->blk_fill_in_prog_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (pkc->last_kactive_blk_num == pkc->kactive_blk_num) {
		if (!frozen) {
<<<<<<< HEAD
=======
			if (!BLOCK_NUM_PKTS(pbd)) {
				/* An empty block. Just refresh the timer. */
				goto refresh_timer;
			}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			prb_retire_current_block(pkc, po, TP_STATUS_BLK_TMO);
			if (!prb_dispatch_next_block(pkc, po))
				goto refresh_timer;
			else
				goto out;
		} else {
			/* Case 1. Queue was frozen because user-space was
			 *	   lagging behind.
			 */
<<<<<<< HEAD
			if (prb_curr_blk_in_use(pkc, pbd)) {
=======
			if (prb_curr_blk_in_use(pbd)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				/*
				 * Ok, user-space is still behind.
				 * So just refresh the timer.
				 */
				goto refresh_timer;
			} else {
			       /* Case 2. queue was frozen,user-space caught up,
				* now the link went idle && the timer fired.
				* We don't have a block to close.So we open this
				* block and restart the timer.
				* opening a block thaws the queue,restarts timer
				* Thawing/timer-refresh is a side effect.
				*/
				prb_open_block(pkc, pbd);
				goto out;
			}
		}
	}

refresh_timer:
	_prb_refresh_rx_retire_blk_timer(pkc);

out:
	spin_unlock(&po->sk.sk_receive_queue.lock);
}

static void prb_flush_block(struct tpacket_kbdq_core *pkc1,
		struct tpacket_block_desc *pbd1, __u32 status)
{
	/* Flush everything minus the block header */

#if ARCH_IMPLEMENTS_FLUSH_DCACHE_PAGE == 1
	u8 *start, *end;

	start = (u8 *)pbd1;

	/* Skip the block header(we know header WILL fit in 4K) */
	start += PAGE_SIZE;

	end = (u8 *)PAGE_ALIGN((unsigned long)pkc1->pkblk_end);
	for (; start < end; start += PAGE_SIZE)
		flush_dcache_page(pgv_to_page(start));

	smp_wmb();
#endif

	/* Now update the block status. */

	BLOCK_STATUS(pbd1) = status;

	/* Flush the block header */

#if ARCH_IMPLEMENTS_FLUSH_DCACHE_PAGE == 1
	start = (u8 *)pbd1;
	flush_dcache_page(pgv_to_page(start));

	smp_wmb();
#endif
}

/*
 * Side effect:
 *
 * 1) flush the block
 * 2) Increment active_blk_num
 *
 * Note:We DONT refresh the timer on purpose.
 *	Because almost always the next block will be opened.
 */
static void prb_close_block(struct tpacket_kbdq_core *pkc1,
		struct tpacket_block_desc *pbd1,
		struct packet_sock *po, unsigned int stat)
{
	__u32 status = TP_STATUS_USER | stat;

	struct tpacket3_hdr *last_pkt;
	struct tpacket_hdr_v1 *h1 = &pbd1->hdr.bh1;
<<<<<<< HEAD

	if (po->stats.tp_drops)
=======
	struct sock *sk = &po->sk;

	if (atomic_read(&po->tp_drops))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		status |= TP_STATUS_LOSING;

	last_pkt = (struct tpacket3_hdr *)pkc1->prev;
	last_pkt->tp_next_offset = 0;

	/* Get the ts of the last pkt */
	if (BLOCK_NUM_PKTS(pbd1)) {
		h1->ts_last_pkt.ts_sec = last_pkt->tp_sec;
		h1->ts_last_pkt.ts_nsec	= last_pkt->tp_nsec;
	} else {
<<<<<<< HEAD
		/* Ok, we tmo'd - so get the current time */
		struct timespec ts;
		getnstimeofday(&ts);
=======
		/* Ok, we tmo'd - so get the current time.
		 *
		 * It shouldn't really happen as we don't close empty
		 * blocks. See prb_retire_rx_blk_timer_expired().
		 */
		struct timespec64 ts;
		ktime_get_real_ts64(&ts);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		h1->ts_last_pkt.ts_sec = ts.tv_sec;
		h1->ts_last_pkt.ts_nsec	= ts.tv_nsec;
	}

	smp_wmb();

	/* Flush the block */
	prb_flush_block(pkc1, pbd1, status);

<<<<<<< HEAD
=======
	sk->sk_data_ready(sk);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	pkc1->kactive_blk_num = GET_NEXT_PRB_BLK_NUM(pkc1);
}

static void prb_thaw_queue(struct tpacket_kbdq_core *pkc)
{
	pkc->reset_pending_on_curr_blk = 0;
}

/*
 * Side effect of opening a block:
 *
 * 1) prb_queue is thawed.
 * 2) retire_blk_timer is refreshed.
 *
 */
static void prb_open_block(struct tpacket_kbdq_core *pkc1,
	struct tpacket_block_desc *pbd1)
{
<<<<<<< HEAD
	struct timespec ts;
=======
	struct timespec64 ts;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct tpacket_hdr_v1 *h1 = &pbd1->hdr.bh1;

	smp_rmb();

	/* We could have just memset this but we will lose the
	 * flexibility of making the priv area sticky
	 */
<<<<<<< HEAD
	BLOCK_SNUM(pbd1) = pkc1->knxt_seq_num++;
	BLOCK_NUM_PKTS(pbd1) = 0;
	BLOCK_LEN(pbd1) = BLK_PLUS_PRIV(pkc1->blk_sizeof_priv);
	getnstimeofday(&ts);
	h1->ts_first_pkt.ts_sec = ts.tv_sec;
	h1->ts_first_pkt.ts_nsec = ts.tv_nsec;
	pkc1->pkblk_start = (char *)pbd1;
	pkc1->nxt_offset = (char *)(pkc1->pkblk_start +
				    BLK_PLUS_PRIV(pkc1->blk_sizeof_priv));
	BLOCK_O2FP(pbd1) = (__u32)BLK_PLUS_PRIV(pkc1->blk_sizeof_priv);
	BLOCK_O2PRIV(pbd1) = BLK_HDR_LEN;
	pbd1->version = pkc1->version;
	pkc1->prev = pkc1->nxt_offset;
	pkc1->pkblk_end = pkc1->pkblk_start + pkc1->kblk_size;
=======

	BLOCK_SNUM(pbd1) = pkc1->knxt_seq_num++;
	BLOCK_NUM_PKTS(pbd1) = 0;
	BLOCK_LEN(pbd1) = BLK_PLUS_PRIV(pkc1->blk_sizeof_priv);

	ktime_get_real_ts64(&ts);

	h1->ts_first_pkt.ts_sec = ts.tv_sec;
	h1->ts_first_pkt.ts_nsec = ts.tv_nsec;

	pkc1->pkblk_start = (char *)pbd1;
	pkc1->nxt_offset = pkc1->pkblk_start + BLK_PLUS_PRIV(pkc1->blk_sizeof_priv);

	BLOCK_O2FP(pbd1) = (__u32)BLK_PLUS_PRIV(pkc1->blk_sizeof_priv);
	BLOCK_O2PRIV(pbd1) = BLK_HDR_LEN;

	pbd1->version = pkc1->version;
	pkc1->prev = pkc1->nxt_offset;
	pkc1->pkblk_end = pkc1->pkblk_start + pkc1->kblk_size;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	prb_thaw_queue(pkc1);
	_prb_refresh_rx_retire_blk_timer(pkc1);

	smp_wmb();
}

/*
 * Queue freeze logic:
 * 1) Assume tp_block_nr = 8 blocks.
 * 2) At time 't0', user opens Rx ring.
 * 3) Some time past 't0', kernel starts filling blocks starting from 0 .. 7
 * 4) user-space is either sleeping or processing block '0'.
 * 5) tpacket_rcv is currently filling block '7', since there is no space left,
 *    it will close block-7,loop around and try to fill block '0'.
 *    call-flow:
 *    __packet_lookup_frame_in_block
 *      prb_retire_current_block()
 *      prb_dispatch_next_block()
 *        |->(BLOCK_STATUS == USER) evaluates to true
 *    5.1) Since block-0 is currently in-use, we just freeze the queue.
 * 6) Now there are two cases:
 *    6.1) Link goes idle right after the queue is frozen.
 *         But remember, the last open_block() refreshed the timer.
 *         When this timer expires,it will refresh itself so that we can
 *         re-open block-0 in near future.
 *    6.2) Link is busy and keeps on receiving packets. This is a simple
 *         case and __packet_lookup_frame_in_block will check if block-0
 *         is free and can now be re-used.
 */
static void prb_freeze_queue(struct tpacket_kbdq_core *pkc,
				  struct packet_sock *po)
{
	pkc->reset_pending_on_curr_blk = 1;
<<<<<<< HEAD
	po->stats_u.stats3.tp_freeze_q_cnt++;
=======
	po->stats.stats3.tp_freeze_q_cnt++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#define TOTAL_PKT_LEN_INCL_ALIGN(length) (ALIGN((length), V3_ALIGNMENT))

/*
 * If the next block is free then we will dispatch it
 * and return a good offset.
 * Else, we will freeze the queue.
 * So, caller must check the return value.
 */
static void *prb_dispatch_next_block(struct tpacket_kbdq_core *pkc,
		struct packet_sock *po)
{
	struct tpacket_block_desc *pbd;

	smp_rmb();

	/* 1. Get current block num */
	pbd = GET_CURR_PBLOCK_DESC_FROM_CORE(pkc);

	/* 2. If this block is currently in_use then freeze the queue */
	if (TP_STATUS_USER & BLOCK_STATUS(pbd)) {
		prb_freeze_queue(pkc, po);
		return NULL;
	}

	/*
	 * 3.
	 * open this block and return the offset where the first packet
	 * needs to get stored.
	 */
	prb_open_block(pkc, pbd);
	return (void *)pkc->nxt_offset;
}

static void prb_retire_current_block(struct tpacket_kbdq_core *pkc,
		struct packet_sock *po, unsigned int status)
{
	struct tpacket_block_desc *pbd = GET_CURR_PBLOCK_DESC_FROM_CORE(pkc);

	/* retire/close the current block */
	if (likely(TP_STATUS_KERNEL == BLOCK_STATUS(pbd))) {
		/*
		 * Plug the case where copy_bits() is in progress on
		 * cpu-0 and tpacket_rcv() got invoked on cpu-1, didn't
		 * have space to copy the pkt in the current block and
		 * called prb_retire_current_block()
		 *
		 * We don't need to worry about the TMO case because
		 * the timer-handler already handled this case.
		 */
		if (!(status & TP_STATUS_BLK_TMO)) {
<<<<<<< HEAD
			while (atomic_read(&pkc->blk_fill_in_prog)) {
				/* Waiting for skb_copy_bits to finish... */
				cpu_relax();
			}
=======
			/* Waiting for skb_copy_bits to finish... */
			write_lock(&pkc->blk_fill_in_prog_lock);
			write_unlock(&pkc->blk_fill_in_prog_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		prb_close_block(pkc, pbd, po, status);
		return;
	}
}

<<<<<<< HEAD
static int prb_curr_blk_in_use(struct tpacket_kbdq_core *pkc,
				      struct tpacket_block_desc *pbd)
=======
static int prb_curr_blk_in_use(struct tpacket_block_desc *pbd)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return TP_STATUS_USER & BLOCK_STATUS(pbd);
}

static int prb_queue_frozen(struct tpacket_kbdq_core *pkc)
{
	return pkc->reset_pending_on_curr_blk;
}

static void prb_clear_blk_fill_status(struct packet_ring_buffer *rb)
<<<<<<< HEAD
{
	struct tpacket_kbdq_core *pkc  = GET_PBDQC_FROM_RB(rb);
	atomic_dec(&pkc->blk_fill_in_prog);
=======
	__releases(&pkc->blk_fill_in_prog_lock)
{
	struct tpacket_kbdq_core *pkc  = GET_PBDQC_FROM_RB(rb);

	read_unlock(&pkc->blk_fill_in_prog_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void prb_fill_rxhash(struct tpacket_kbdq_core *pkc,
			struct tpacket3_hdr *ppd)
{
<<<<<<< HEAD
	ppd->hv1.tp_rxhash = skb_get_rxhash(pkc->skb);
=======
	ppd->hv1.tp_rxhash = skb_get_hash(pkc->skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void prb_clear_rxhash(struct tpacket_kbdq_core *pkc,
			struct tpacket3_hdr *ppd)
{
	ppd->hv1.tp_rxhash = 0;
}

static void prb_fill_vlan_info(struct tpacket_kbdq_core *pkc,
			struct tpacket3_hdr *ppd)
{
<<<<<<< HEAD
	if (vlan_tx_tag_present(pkc->skb)) {
		ppd->hv1.tp_vlan_tci = vlan_tx_tag_get(pkc->skb);
		ppd->tp_status = TP_STATUS_VLAN_VALID;
	} else {
		ppd->hv1.tp_vlan_tci = ppd->tp_status = 0;
=======
	if (skb_vlan_tag_present(pkc->skb)) {
		ppd->hv1.tp_vlan_tci = skb_vlan_tag_get(pkc->skb);
		ppd->hv1.tp_vlan_tpid = ntohs(pkc->skb->vlan_proto);
		ppd->tp_status = TP_STATUS_VLAN_VALID | TP_STATUS_VLAN_TPID_VALID;
	} else {
		ppd->hv1.tp_vlan_tci = 0;
		ppd->hv1.tp_vlan_tpid = 0;
		ppd->tp_status = TP_STATUS_AVAILABLE;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

static void prb_run_all_ft_ops(struct tpacket_kbdq_core *pkc,
			struct tpacket3_hdr *ppd)
{
<<<<<<< HEAD
=======
	ppd->hv1.tp_padding = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	prb_fill_vlan_info(pkc, ppd);

	if (pkc->feature_req_word & TP_FT_REQ_FILL_RXHASH)
		prb_fill_rxhash(pkc, ppd);
	else
		prb_clear_rxhash(pkc, ppd);
}

static void prb_fill_curr_block(char *curr,
				struct tpacket_kbdq_core *pkc,
				struct tpacket_block_desc *pbd,
				unsigned int len)
<<<<<<< HEAD
=======
	__acquires(&pkc->blk_fill_in_prog_lock)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct tpacket3_hdr *ppd;

	ppd  = (struct tpacket3_hdr *)curr;
	ppd->tp_next_offset = TOTAL_PKT_LEN_INCL_ALIGN(len);
	pkc->prev = curr;
	pkc->nxt_offset += TOTAL_PKT_LEN_INCL_ALIGN(len);
	BLOCK_LEN(pbd) += TOTAL_PKT_LEN_INCL_ALIGN(len);
	BLOCK_NUM_PKTS(pbd) += 1;
<<<<<<< HEAD
	atomic_inc(&pkc->blk_fill_in_prog);
=======
	read_lock(&pkc->blk_fill_in_prog_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	prb_run_all_ft_ops(pkc, ppd);
}

/* Assumes caller has the sk->rx_queue.lock */
static void *__packet_lookup_frame_in_block(struct packet_sock *po,
					    struct sk_buff *skb,
<<<<<<< HEAD
						int status,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					    unsigned int len
					    )
{
	struct tpacket_kbdq_core *pkc;
	struct tpacket_block_desc *pbd;
	char *curr, *end;

<<<<<<< HEAD
	pkc = GET_PBDQC_FROM_RB(((struct packet_ring_buffer *)&po->rx_ring));
=======
	pkc = GET_PBDQC_FROM_RB(&po->rx_ring);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	pbd = GET_CURR_PBLOCK_DESC_FROM_CORE(pkc);

	/* Queue is frozen when user space is lagging behind */
	if (prb_queue_frozen(pkc)) {
		/*
		 * Check if that last block which caused the queue to freeze,
		 * is still in_use by user-space.
		 */
<<<<<<< HEAD
		if (prb_curr_blk_in_use(pkc, pbd)) {
=======
		if (prb_curr_blk_in_use(pbd)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/* Can't record this packet */
			return NULL;
		} else {
			/*
			 * Ok, the block was released by user-space.
			 * Now let's open that block.
			 * opening a block also thaws the queue.
			 * Thawing is a side effect.
			 */
			prb_open_block(pkc, pbd);
		}
	}

	smp_mb();
	curr = pkc->nxt_offset;
	pkc->skb = skb;
<<<<<<< HEAD
	end = (char *) ((char *)pbd + pkc->kblk_size);
=======
	end = (char *)pbd + pkc->kblk_size;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* first try the current block */
	if (curr+TOTAL_PKT_LEN_INCL_ALIGN(len) < end) {
		prb_fill_curr_block(curr, pkc, pbd, len);
		return (void *)curr;
	}

	/* Ok, close the current block */
	prb_retire_current_block(pkc, po, 0);

	/* Now, try to dispatch the next block */
	curr = (char *)prb_dispatch_next_block(pkc, po);
	if (curr) {
		pbd = GET_CURR_PBLOCK_DESC_FROM_CORE(pkc);
		prb_fill_curr_block(curr, pkc, pbd, len);
		return (void *)curr;
	}

	/*
	 * No free blocks are available.user_space hasn't caught up yet.
	 * Queue was just frozen and now this packet will get dropped.
	 */
	return NULL;
}

static void *packet_current_rx_frame(struct packet_sock *po,
					    struct sk_buff *skb,
					    int status, unsigned int len)
{
	char *curr = NULL;
	switch (po->tp_version) {
	case TPACKET_V1:
	case TPACKET_V2:
		curr = packet_lookup_frame(po, &po->rx_ring,
					po->rx_ring.head, status);
		return curr;
	case TPACKET_V3:
<<<<<<< HEAD
		return __packet_lookup_frame_in_block(po, skb, status, len);
	default:
		WARN(1, "TPACKET version not supported\n");
		BUG();
		return 0;
	}
}

static void *prb_lookup_block(struct packet_sock *po,
				     struct packet_ring_buffer *rb,
				     unsigned int previous,
				     int status)
{
	struct tpacket_kbdq_core *pkc  = GET_PBDQC_FROM_RB(rb);
	struct tpacket_block_desc *pbd = GET_PBLOCK_DESC(pkc, previous);
=======
		return __packet_lookup_frame_in_block(po, skb, len);
	default:
		WARN(1, "TPACKET version not supported\n");
		BUG();
		return NULL;
	}
}

static void *prb_lookup_block(const struct packet_sock *po,
			      const struct packet_ring_buffer *rb,
			      unsigned int idx,
			      int status)
{
	struct tpacket_kbdq_core *pkc  = GET_PBDQC_FROM_RB(rb);
	struct tpacket_block_desc *pbd = GET_PBLOCK_DESC(pkc, idx);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (status != BLOCK_STATUS(pbd))
		return NULL;
	return pbd;
}

static int prb_previous_blk_num(struct packet_ring_buffer *rb)
{
	unsigned int prev;
	if (rb->prb_bdqc.kactive_blk_num)
		prev = rb->prb_bdqc.kactive_blk_num-1;
	else
		prev = rb->prb_bdqc.knum_blocks-1;
	return prev;
}

/* Assumes caller has held the rx_queue.lock */
static void *__prb_previous_block(struct packet_sock *po,
					 struct packet_ring_buffer *rb,
					 int status)
{
	unsigned int previous = prb_previous_blk_num(rb);
	return prb_lookup_block(po, rb, previous, status);
}

static void *packet_previous_rx_frame(struct packet_sock *po,
					     struct packet_ring_buffer *rb,
					     int status)
{
	if (po->tp_version <= TPACKET_V2)
		return packet_previous_frame(po, rb, status);

	return __prb_previous_block(po, rb, status);
}

static void packet_increment_rx_head(struct packet_sock *po,
					    struct packet_ring_buffer *rb)
{
	switch (po->tp_version) {
	case TPACKET_V1:
	case TPACKET_V2:
		return packet_increment_head(rb);
	case TPACKET_V3:
	default:
		WARN(1, "TPACKET version not supported.\n");
		BUG();
		return;
	}
}

static void *packet_previous_frame(struct packet_sock *po,
		struct packet_ring_buffer *rb,
		int status)
{
	unsigned int previous = rb->head ? rb->head - 1 : rb->frame_max;
	return packet_lookup_frame(po, rb, previous, status);
}

static void packet_increment_head(struct packet_ring_buffer *buff)
{
	buff->head = buff->head != buff->frame_max ? buff->head+1 : 0;
}

<<<<<<< HEAD
=======
static void packet_inc_pending(struct packet_ring_buffer *rb)
{
	this_cpu_inc(*rb->pending_refcnt);
}

static void packet_dec_pending(struct packet_ring_buffer *rb)
{
	this_cpu_dec(*rb->pending_refcnt);
}

static unsigned int packet_read_pending(const struct packet_ring_buffer *rb)
{
	unsigned int refcnt = 0;
	int cpu;

	/* We don't use pending refcount in rx_ring. */
	if (rb->pending_refcnt == NULL)
		return 0;

	for_each_possible_cpu(cpu)
		refcnt += *per_cpu_ptr(rb->pending_refcnt, cpu);

	return refcnt;
}

static int packet_alloc_pending(struct packet_sock *po)
{
	po->rx_ring.pending_refcnt = NULL;

	po->tx_ring.pending_refcnt = alloc_percpu(unsigned int);
	if (unlikely(po->tx_ring.pending_refcnt == NULL))
		return -ENOBUFS;

	return 0;
}

static void packet_free_pending(struct packet_sock *po)
{
	free_percpu(po->tx_ring.pending_refcnt);
}

#define ROOM_POW_OFF	2
#define ROOM_NONE	0x0
#define ROOM_LOW	0x1
#define ROOM_NORMAL	0x2

static bool __tpacket_has_room(const struct packet_sock *po, int pow_off)
{
	int idx, len;

	len = READ_ONCE(po->rx_ring.frame_max) + 1;
	idx = READ_ONCE(po->rx_ring.head);
	if (pow_off)
		idx += len >> pow_off;
	if (idx >= len)
		idx -= len;
	return packet_lookup_frame(po, &po->rx_ring, idx, TP_STATUS_KERNEL);
}

static bool __tpacket_v3_has_room(const struct packet_sock *po, int pow_off)
{
	int idx, len;

	len = READ_ONCE(po->rx_ring.prb_bdqc.knum_blocks);
	idx = READ_ONCE(po->rx_ring.prb_bdqc.kactive_blk_num);
	if (pow_off)
		idx += len >> pow_off;
	if (idx >= len)
		idx -= len;
	return prb_lookup_block(po, &po->rx_ring, idx, TP_STATUS_KERNEL);
}

static int __packet_rcv_has_room(const struct packet_sock *po,
				 const struct sk_buff *skb)
{
	const struct sock *sk = &po->sk;
	int ret = ROOM_NONE;

	if (po->prot_hook.func != tpacket_rcv) {
		int rcvbuf = READ_ONCE(sk->sk_rcvbuf);
		int avail = rcvbuf - atomic_read(&sk->sk_rmem_alloc)
				   - (skb ? skb->truesize : 0);

		if (avail > (rcvbuf >> ROOM_POW_OFF))
			return ROOM_NORMAL;
		else if (avail > 0)
			return ROOM_LOW;
		else
			return ROOM_NONE;
	}

	if (po->tp_version == TPACKET_V3) {
		if (__tpacket_v3_has_room(po, ROOM_POW_OFF))
			ret = ROOM_NORMAL;
		else if (__tpacket_v3_has_room(po, 0))
			ret = ROOM_LOW;
	} else {
		if (__tpacket_has_room(po, ROOM_POW_OFF))
			ret = ROOM_NORMAL;
		else if (__tpacket_has_room(po, 0))
			ret = ROOM_LOW;
	}

	return ret;
}

static int packet_rcv_has_room(struct packet_sock *po, struct sk_buff *skb)
{
	bool pressure;
	int ret;

	ret = __packet_rcv_has_room(po, skb);
	pressure = ret != ROOM_NORMAL;

	if (packet_sock_flag(po, PACKET_SOCK_PRESSURE) != pressure)
		packet_sock_flag_set(po, PACKET_SOCK_PRESSURE, pressure);

	return ret;
}

static void packet_rcv_try_clear_pressure(struct packet_sock *po)
{
	if (packet_sock_flag(po, PACKET_SOCK_PRESSURE) &&
	    __packet_rcv_has_room(po, NULL) == ROOM_NORMAL)
		packet_sock_flag_set(po, PACKET_SOCK_PRESSURE, false);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void packet_sock_destruct(struct sock *sk)
{
	skb_queue_purge(&sk->sk_error_queue);

	WARN_ON(atomic_read(&sk->sk_rmem_alloc));
<<<<<<< HEAD
	WARN_ON(atomic_read(&sk->sk_wmem_alloc));

	if (!sock_flag(sk, SOCK_DEAD)) {
		WARN(1, "Attempt to release alive packet socket: %p\n", sk);
		return;
	}

	sk_refcnt_debug_dec(sk);
}

static struct sock *fanout_demux_hash(struct packet_fanout *f, struct sk_buff *skb, unsigned int num)
{
	u32 idx, hash = skb->rxhash;

	idx = ((u64)hash * num) >> 32;

	return f->arr[idx];
}

static struct sock *fanout_demux_lb(struct packet_fanout *f, struct sk_buff *skb, unsigned int num)
{
	unsigned int val = atomic_inc_return(&f->rr_cur);

	return f->arr[val % num];
}

static struct sock *fanout_demux_cpu(struct packet_fanout *f, struct sk_buff *skb, unsigned int num)
{
	unsigned int cpu = smp_processor_id();

	return f->arr[cpu % num];
=======
	WARN_ON(refcount_read(&sk->sk_wmem_alloc));

	if (!sock_flag(sk, SOCK_DEAD)) {
		pr_err("Attempt to release alive packet socket: %p\n", sk);
		return;
	}
}

static bool fanout_flow_is_huge(struct packet_sock *po, struct sk_buff *skb)
{
	u32 *history = po->rollover->history;
	u32 victim, rxhash;
	int i, count = 0;

	rxhash = skb_get_hash(skb);
	for (i = 0; i < ROLLOVER_HLEN; i++)
		if (READ_ONCE(history[i]) == rxhash)
			count++;

	victim = get_random_u32_below(ROLLOVER_HLEN);

	/* Avoid dirtying the cache line if possible */
	if (READ_ONCE(history[victim]) != rxhash)
		WRITE_ONCE(history[victim], rxhash);

	return count > (ROLLOVER_HLEN >> 1);
}

static unsigned int fanout_demux_hash(struct packet_fanout *f,
				      struct sk_buff *skb,
				      unsigned int num)
{
	return reciprocal_scale(__skb_get_hash_symmetric(skb), num);
}

static unsigned int fanout_demux_lb(struct packet_fanout *f,
				    struct sk_buff *skb,
				    unsigned int num)
{
	unsigned int val = atomic_inc_return(&f->rr_cur);

	return val % num;
}

static unsigned int fanout_demux_cpu(struct packet_fanout *f,
				     struct sk_buff *skb,
				     unsigned int num)
{
	return smp_processor_id() % num;
}

static unsigned int fanout_demux_rnd(struct packet_fanout *f,
				     struct sk_buff *skb,
				     unsigned int num)
{
	return get_random_u32_below(num);
}

static unsigned int fanout_demux_rollover(struct packet_fanout *f,
					  struct sk_buff *skb,
					  unsigned int idx, bool try_self,
					  unsigned int num)
{
	struct packet_sock *po, *po_next, *po_skip = NULL;
	unsigned int i, j, room = ROOM_NONE;

	po = pkt_sk(rcu_dereference(f->arr[idx]));

	if (try_self) {
		room = packet_rcv_has_room(po, skb);
		if (room == ROOM_NORMAL ||
		    (room == ROOM_LOW && !fanout_flow_is_huge(po, skb)))
			return idx;
		po_skip = po;
	}

	i = j = min_t(int, po->rollover->sock, num - 1);
	do {
		po_next = pkt_sk(rcu_dereference(f->arr[i]));
		if (po_next != po_skip &&
		    !packet_sock_flag(po_next, PACKET_SOCK_PRESSURE) &&
		    packet_rcv_has_room(po_next, skb) == ROOM_NORMAL) {
			if (i != j)
				po->rollover->sock = i;
			atomic_long_inc(&po->rollover->num);
			if (room == ROOM_LOW)
				atomic_long_inc(&po->rollover->num_huge);
			return i;
		}

		if (++i == num)
			i = 0;
	} while (i != j);

	atomic_long_inc(&po->rollover->num_failed);
	return idx;
}

static unsigned int fanout_demux_qm(struct packet_fanout *f,
				    struct sk_buff *skb,
				    unsigned int num)
{
	return skb_get_queue_mapping(skb) % num;
}

static unsigned int fanout_demux_bpf(struct packet_fanout *f,
				     struct sk_buff *skb,
				     unsigned int num)
{
	struct bpf_prog *prog;
	unsigned int ret = 0;

	rcu_read_lock();
	prog = rcu_dereference(f->bpf_prog);
	if (prog)
		ret = bpf_prog_run_clear_cb(prog, skb) % num;
	rcu_read_unlock();

	return ret;
}

static bool fanout_has_flag(struct packet_fanout *f, u16 flag)
{
	return f->flags & (flag >> 8);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int packet_rcv_fanout(struct sk_buff *skb, struct net_device *dev,
			     struct packet_type *pt, struct net_device *orig_dev)
{
	struct packet_fanout *f = pt->af_packet_priv;
<<<<<<< HEAD
	unsigned int num = ACCESS_ONCE(f->num_members);
	struct packet_sock *po;
	struct sock *sk;

	if (!net_eq(dev_net(dev), read_pnet(&f->net)) ||
	    !num) {
=======
	unsigned int num = READ_ONCE(f->num_members);
	struct net *net = read_pnet(&f->net);
	struct packet_sock *po;
	unsigned int idx;

	if (!net_eq(dev_net(dev), net) || !num) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		kfree_skb(skb);
		return 0;
	}

<<<<<<< HEAD
	switch (f->type) {
	case PACKET_FANOUT_HASH:
	default:
		if (f->defrag) {
			skb = ip_check_defrag(skb, IP_DEFRAG_AF_PACKET);
			if (!skb)
				return 0;
		}
		skb_get_rxhash(skb);
		sk = fanout_demux_hash(f, skb, num);
		break;
	case PACKET_FANOUT_LB:
		sk = fanout_demux_lb(f, skb, num);
		break;
	case PACKET_FANOUT_CPU:
		sk = fanout_demux_cpu(f, skb, num);
		break;
	}

	po = pkt_sk(sk);

	return po->prot_hook.func(skb, dev, &po->prot_hook, orig_dev);
}

static DEFINE_MUTEX(fanout_mutex);
static LIST_HEAD(fanout_list);
=======
	if (fanout_has_flag(f, PACKET_FANOUT_FLAG_DEFRAG)) {
		skb = ip_check_defrag(net, skb, IP_DEFRAG_AF_PACKET);
		if (!skb)
			return 0;
	}
	switch (f->type) {
	case PACKET_FANOUT_HASH:
	default:
		idx = fanout_demux_hash(f, skb, num);
		break;
	case PACKET_FANOUT_LB:
		idx = fanout_demux_lb(f, skb, num);
		break;
	case PACKET_FANOUT_CPU:
		idx = fanout_demux_cpu(f, skb, num);
		break;
	case PACKET_FANOUT_RND:
		idx = fanout_demux_rnd(f, skb, num);
		break;
	case PACKET_FANOUT_QM:
		idx = fanout_demux_qm(f, skb, num);
		break;
	case PACKET_FANOUT_ROLLOVER:
		idx = fanout_demux_rollover(f, skb, 0, false, num);
		break;
	case PACKET_FANOUT_CBPF:
	case PACKET_FANOUT_EBPF:
		idx = fanout_demux_bpf(f, skb, num);
		break;
	}

	if (fanout_has_flag(f, PACKET_FANOUT_FLAG_ROLLOVER))
		idx = fanout_demux_rollover(f, skb, idx, true, num);

	po = pkt_sk(rcu_dereference(f->arr[idx]));
	return po->prot_hook.func(skb, dev, &po->prot_hook, orig_dev);
}

DEFINE_MUTEX(fanout_mutex);
EXPORT_SYMBOL_GPL(fanout_mutex);
static LIST_HEAD(fanout_list);
static u16 fanout_next_id;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static void __fanout_link(struct sock *sk, struct packet_sock *po)
{
	struct packet_fanout *f = po->fanout;

	spin_lock(&f->lock);
<<<<<<< HEAD
	f->arr[f->num_members] = sk;
	smp_wmb();
	f->num_members++;
=======
	rcu_assign_pointer(f->arr[f->num_members], sk);
	smp_wmb();
	f->num_members++;
	if (f->num_members == 1)
		dev_add_pack(&f->prot_hook);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_unlock(&f->lock);
}

static void __fanout_unlink(struct sock *sk, struct packet_sock *po)
{
	struct packet_fanout *f = po->fanout;
	int i;

	spin_lock(&f->lock);
	for (i = 0; i < f->num_members; i++) {
<<<<<<< HEAD
		if (f->arr[i] == sk)
			break;
	}
	BUG_ON(i >= f->num_members);
	f->arr[i] = f->arr[f->num_members - 1];
	f->num_members--;
	spin_unlock(&f->lock);
}

bool match_fanout_group(struct packet_type *ptype, struct sock * sk)
{
	if (ptype->af_packet_priv == (void*)((struct packet_sock *)sk)->fanout)
		return true;
=======
		if (rcu_dereference_protected(f->arr[i],
					      lockdep_is_held(&f->lock)) == sk)
			break;
	}
	BUG_ON(i >= f->num_members);
	rcu_assign_pointer(f->arr[i],
			   rcu_dereference_protected(f->arr[f->num_members - 1],
						     lockdep_is_held(&f->lock)));
	f->num_members--;
	if (f->num_members == 0)
		__dev_remove_pack(&f->prot_hook);
	spin_unlock(&f->lock);
}

static bool match_fanout_group(struct packet_type *ptype, struct sock *sk)
{
	if (sk->sk_family != PF_PACKET)
		return false;

	return ptype->af_packet_priv == pkt_sk(sk)->fanout;
}

static void fanout_init_data(struct packet_fanout *f)
{
	switch (f->type) {
	case PACKET_FANOUT_LB:
		atomic_set(&f->rr_cur, 0);
		break;
	case PACKET_FANOUT_CBPF:
	case PACKET_FANOUT_EBPF:
		RCU_INIT_POINTER(f->bpf_prog, NULL);
		break;
	}
}

static void __fanout_set_data_bpf(struct packet_fanout *f, struct bpf_prog *new)
{
	struct bpf_prog *old;

	spin_lock(&f->lock);
	old = rcu_dereference_protected(f->bpf_prog, lockdep_is_held(&f->lock));
	rcu_assign_pointer(f->bpf_prog, new);
	spin_unlock(&f->lock);

	if (old) {
		synchronize_net();
		bpf_prog_destroy(old);
	}
}

static int fanout_set_data_cbpf(struct packet_sock *po, sockptr_t data,
				unsigned int len)
{
	struct bpf_prog *new;
	struct sock_fprog fprog;
	int ret;

	if (sock_flag(&po->sk, SOCK_FILTER_LOCKED))
		return -EPERM;

	ret = copy_bpf_fprog_from_user(&fprog, data, len);
	if (ret)
		return ret;

	ret = bpf_prog_create_from_user(&new, &fprog, NULL, false);
	if (ret)
		return ret;

	__fanout_set_data_bpf(po->fanout, new);
	return 0;
}

static int fanout_set_data_ebpf(struct packet_sock *po, sockptr_t data,
				unsigned int len)
{
	struct bpf_prog *new;
	u32 fd;

	if (sock_flag(&po->sk, SOCK_FILTER_LOCKED))
		return -EPERM;
	if (len != sizeof(fd))
		return -EINVAL;
	if (copy_from_sockptr(&fd, data, len))
		return -EFAULT;

	new = bpf_prog_get_type(fd, BPF_PROG_TYPE_SOCKET_FILTER);
	if (IS_ERR(new))
		return PTR_ERR(new);

	__fanout_set_data_bpf(po->fanout, new);
	return 0;
}

static int fanout_set_data(struct packet_sock *po, sockptr_t data,
			   unsigned int len)
{
	switch (po->fanout->type) {
	case PACKET_FANOUT_CBPF:
		return fanout_set_data_cbpf(po, data, len);
	case PACKET_FANOUT_EBPF:
		return fanout_set_data_ebpf(po, data, len);
	default:
		return -EINVAL;
	}
}

static void fanout_release_data(struct packet_fanout *f)
{
	switch (f->type) {
	case PACKET_FANOUT_CBPF:
	case PACKET_FANOUT_EBPF:
		__fanout_set_data_bpf(f, NULL);
	}
}

static bool __fanout_id_is_free(struct sock *sk, u16 candidate_id)
{
	struct packet_fanout *f;

	list_for_each_entry(f, &fanout_list, list) {
		if (f->id == candidate_id &&
		    read_pnet(&f->net) == sock_net(sk)) {
			return false;
		}
	}
	return true;
}

static bool fanout_find_new_id(struct sock *sk, u16 *new_id)
{
	u16 id = fanout_next_id;

	do {
		if (__fanout_id_is_free(sk, id)) {
			*new_id = id;
			fanout_next_id = id + 1;
			return true;
		}

		id++;
	} while (id != fanout_next_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return false;
}

<<<<<<< HEAD
static int fanout_add(struct sock *sk, u16 id, u16 type_flags)
{
	struct packet_sock *po = pkt_sk(sk);
	struct packet_fanout *f, *match;
	u8 type = type_flags & 0xff;
	u8 defrag = (type_flags & PACKET_FANOUT_FLAG_DEFRAG) ? 1 : 0;
	int err;

	switch (type) {
	case PACKET_FANOUT_HASH:
	case PACKET_FANOUT_LB:
	case PACKET_FANOUT_CPU:
=======
static int fanout_add(struct sock *sk, struct fanout_args *args)
{
	struct packet_rollover *rollover = NULL;
	struct packet_sock *po = pkt_sk(sk);
	u16 type_flags = args->type_flags;
	struct packet_fanout *f, *match;
	u8 type = type_flags & 0xff;
	u8 flags = type_flags >> 8;
	u16 id = args->id;
	int err;

	switch (type) {
	case PACKET_FANOUT_ROLLOVER:
		if (type_flags & PACKET_FANOUT_FLAG_ROLLOVER)
			return -EINVAL;
		break;
	case PACKET_FANOUT_HASH:
	case PACKET_FANOUT_LB:
	case PACKET_FANOUT_CPU:
	case PACKET_FANOUT_RND:
	case PACKET_FANOUT_QM:
	case PACKET_FANOUT_CBPF:
	case PACKET_FANOUT_EBPF:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	default:
		return -EINVAL;
	}

	mutex_lock(&fanout_mutex);

<<<<<<< HEAD
	err = -EINVAL;
	if (!po->running)
		goto out;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	err = -EALREADY;
	if (po->fanout)
		goto out;

<<<<<<< HEAD
=======
	if (type == PACKET_FANOUT_ROLLOVER ||
	    (type_flags & PACKET_FANOUT_FLAG_ROLLOVER)) {
		err = -ENOMEM;
		rollover = kzalloc(sizeof(*rollover), GFP_KERNEL);
		if (!rollover)
			goto out;
		atomic_long_set(&rollover->num, 0);
		atomic_long_set(&rollover->num_huge, 0);
		atomic_long_set(&rollover->num_failed, 0);
	}

	if (type_flags & PACKET_FANOUT_FLAG_UNIQUEID) {
		if (id != 0) {
			err = -EINVAL;
			goto out;
		}
		if (!fanout_find_new_id(sk, &id)) {
			err = -ENOMEM;
			goto out;
		}
		/* ephemeral flag for the first socket in the group: drop it */
		flags &= ~(PACKET_FANOUT_FLAG_UNIQUEID >> 8);
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	match = NULL;
	list_for_each_entry(f, &fanout_list, list) {
		if (f->id == id &&
		    read_pnet(&f->net) == sock_net(sk)) {
			match = f;
			break;
		}
	}
	err = -EINVAL;
<<<<<<< HEAD
	if (match && match->defrag != defrag)
		goto out;
	if (!match) {
		err = -ENOMEM;
		match = kzalloc(sizeof(*match), GFP_KERNEL);
=======
	if (match) {
		if (match->flags != flags)
			goto out;
		if (args->max_num_members &&
		    args->max_num_members != match->max_num_members)
			goto out;
	} else {
		if (args->max_num_members > PACKET_FANOUT_MAX)
			goto out;
		if (!args->max_num_members)
			/* legacy PACKET_FANOUT_MAX */
			args->max_num_members = 256;
		err = -ENOMEM;
		match = kvzalloc(struct_size(match, arr, args->max_num_members),
				 GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!match)
			goto out;
		write_pnet(&match->net, sock_net(sk));
		match->id = id;
		match->type = type;
<<<<<<< HEAD
		match->defrag = defrag;
		atomic_set(&match->rr_cur, 0);
		INIT_LIST_HEAD(&match->list);
		spin_lock_init(&match->lock);
		atomic_set(&match->sk_ref, 0);
=======
		match->flags = flags;
		INIT_LIST_HEAD(&match->list);
		spin_lock_init(&match->lock);
		refcount_set(&match->sk_ref, 0);
		fanout_init_data(match);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		match->prot_hook.type = po->prot_hook.type;
		match->prot_hook.dev = po->prot_hook.dev;
		match->prot_hook.func = packet_rcv_fanout;
		match->prot_hook.af_packet_priv = match;
<<<<<<< HEAD
		match->prot_hook.id_match = match_fanout_group;
		dev_add_pack(&match->prot_hook);
		list_add(&match->list, &fanout_list);
	}
	err = -EINVAL;
	if (match->type == type &&
	    match->prot_hook.type == po->prot_hook.type &&
	    match->prot_hook.dev == po->prot_hook.dev) {
		err = -ENOSPC;
		if (atomic_read(&match->sk_ref) < PACKET_FANOUT_MAX) {
			__dev_remove_pack(&po->prot_hook);
			po->fanout = match;
			atomic_inc(&match->sk_ref);
=======
		match->prot_hook.af_packet_net = read_pnet(&match->net);
		match->prot_hook.id_match = match_fanout_group;
		match->max_num_members = args->max_num_members;
		match->prot_hook.ignore_outgoing = type_flags & PACKET_FANOUT_FLAG_IGNORE_OUTGOING;
		list_add(&match->list, &fanout_list);
	}
	err = -EINVAL;

	spin_lock(&po->bind_lock);
	if (packet_sock_flag(po, PACKET_SOCK_RUNNING) &&
	    match->type == type &&
	    match->prot_hook.type == po->prot_hook.type &&
	    match->prot_hook.dev == po->prot_hook.dev) {
		err = -ENOSPC;
		if (refcount_read(&match->sk_ref) < match->max_num_members) {
			__dev_remove_pack(&po->prot_hook);

			/* Paired with packet_setsockopt(PACKET_FANOUT_DATA) */
			WRITE_ONCE(po->fanout, match);

			po->rollover = rollover;
			rollover = NULL;
			refcount_set(&match->sk_ref, refcount_read(&match->sk_ref) + 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			__fanout_link(sk, po);
			err = 0;
		}
	}
<<<<<<< HEAD
out:
=======
	spin_unlock(&po->bind_lock);

	if (err && !refcount_read(&match->sk_ref)) {
		list_del(&match->list);
		kvfree(match);
	}

out:
	kfree(rollover);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mutex_unlock(&fanout_mutex);
	return err;
}

<<<<<<< HEAD
static void fanout_release(struct sock *sk)
=======
/* If pkt_sk(sk)->fanout->sk_ref is zero, this function removes
 * pkt_sk(sk)->fanout from fanout_list and returns pkt_sk(sk)->fanout.
 * It is the responsibility of the caller to call fanout_release_data() and
 * free the returned packet_fanout (after synchronize_net())
 */
static struct packet_fanout *fanout_release(struct sock *sk)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct packet_sock *po = pkt_sk(sk);
	struct packet_fanout *f;

	mutex_lock(&fanout_mutex);
	f = po->fanout;
	if (f) {
		po->fanout = NULL;

<<<<<<< HEAD
		if (atomic_dec_and_test(&f->sk_ref)) {
			list_del(&f->list);
			dev_remove_pack(&f->prot_hook);
			kfree(f);
		}
	}
	mutex_unlock(&fanout_mutex);
=======
		if (refcount_dec_and_test(&f->sk_ref))
			list_del(&f->list);
		else
			f = NULL;
	}
	mutex_unlock(&fanout_mutex);

	return f;
}

static bool packet_extra_vlan_len_allowed(const struct net_device *dev,
					  struct sk_buff *skb)
{
	/* Earlier code assumed this would be a VLAN pkt, double-check
	 * this now that we have the actual packet in hand. We can only
	 * do this check on Ethernet devices.
	 */
	if (unlikely(dev->type != ARPHRD_ETHER))
		return false;

	skb_reset_mac_header(skb);
	return likely(eth_hdr(skb)->h_proto == htons(ETH_P_8021Q));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const struct proto_ops packet_ops;

static const struct proto_ops packet_ops_spkt;

static int packet_rcv_spkt(struct sk_buff *skb, struct net_device *dev,
			   struct packet_type *pt, struct net_device *orig_dev)
{
	struct sock *sk;
	struct sockaddr_pkt *spkt;

	/*
	 *	When we registered the protocol we saved the socket in the data
	 *	field for just this event.
	 */

	sk = pt->af_packet_priv;

	/*
	 *	Yank back the headers [hope the device set this
	 *	right or kerboom...]
	 *
	 *	Incoming packets have ll header pulled,
	 *	push it back.
	 *
	 *	For outgoing ones skb->data == skb_mac_header(skb)
	 *	so that this procedure is noop.
	 */

	if (skb->pkt_type == PACKET_LOOPBACK)
		goto out;

	if (!net_eq(dev_net(dev), sock_net(sk)))
		goto out;

	skb = skb_share_check(skb, GFP_ATOMIC);
	if (skb == NULL)
		goto oom;

	/* drop any routing info */
	skb_dst_drop(skb);

	/* drop conntrack reference */
<<<<<<< HEAD
	nf_reset(skb);
=======
	nf_reset_ct(skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spkt = &PACKET_SKB_CB(skb)->sa.pkt;

	skb_push(skb, skb->data - skb_mac_header(skb));

	/*
	 *	The SOCK_PACKET socket receives _all_ frames.
	 */

	spkt->spkt_family = dev->type;
<<<<<<< HEAD
	strlcpy(spkt->spkt_device, dev->name, sizeof(spkt->spkt_device));
=======
	strscpy(spkt->spkt_device, dev->name, sizeof(spkt->spkt_device));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spkt->spkt_protocol = skb->protocol;

	/*
	 *	Charge the memory to the socket. This is done specifically
	 *	to prevent sockets using all the memory up.
	 */

	if (sock_queue_rcv_skb(sk, skb) == 0)
		return 0;

out:
	kfree_skb(skb);
oom:
	return 0;
}

<<<<<<< HEAD
=======
static void packet_parse_headers(struct sk_buff *skb, struct socket *sock)
{
	int depth;

	if ((!skb->protocol || skb->protocol == htons(ETH_P_ALL)) &&
	    sock->type == SOCK_RAW) {
		skb_reset_mac_header(skb);
		skb->protocol = dev_parse_header_protocol(skb);
	}

	/* Move network header to the right position for VLAN tagged packets */
	if (likely(skb->dev->type == ARPHRD_ETHER) &&
	    eth_type_vlan(skb->protocol) &&
	    vlan_get_protocol_and_depth(skb, skb->protocol, &depth) != 0)
		skb_set_network_header(skb, depth);

	skb_probe_transport_header(skb);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 *	Output a raw packet to a device layer. This bypasses all the other
 *	protocol layers and you must therefore supply it with a complete frame
 */

<<<<<<< HEAD
static int packet_sendmsg_spkt(struct kiocb *iocb, struct socket *sock,
			       struct msghdr *msg, size_t len)
{
	struct sock *sk = sock->sk;
	struct sockaddr_pkt *saddr = (struct sockaddr_pkt *)msg->msg_name;
	struct sk_buff *skb = NULL;
	struct net_device *dev;
=======
static int packet_sendmsg_spkt(struct socket *sock, struct msghdr *msg,
			       size_t len)
{
	struct sock *sk = sock->sk;
	DECLARE_SOCKADDR(struct sockaddr_pkt *, saddr, msg->msg_name);
	struct sk_buff *skb = NULL;
	struct net_device *dev;
	struct sockcm_cookie sockc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	__be16 proto = 0;
	int err;
	int extra_len = 0;

	/*
	 *	Get and verify the address.
	 */

	if (saddr) {
		if (msg->msg_namelen < sizeof(struct sockaddr))
			return -EINVAL;
		if (msg->msg_namelen == sizeof(struct sockaddr_pkt))
			proto = saddr->spkt_protocol;
	} else
		return -ENOTCONN;	/* SOCK_PACKET must be sent giving an address */

	/*
	 *	Find the device first to size check it
	 */

<<<<<<< HEAD
	saddr->spkt_device[13] = 0;
=======
	saddr->spkt_device[sizeof(saddr->spkt_device) - 1] = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
retry:
	rcu_read_lock();
	dev = dev_get_by_name_rcu(sock_net(sk), saddr->spkt_device);
	err = -ENODEV;
	if (dev == NULL)
		goto out_unlock;

	err = -ENETDOWN;
	if (!(dev->flags & IFF_UP))
		goto out_unlock;

	/*
	 * You may not queue a frame bigger than the mtu. This is the lowest level
	 * raw protocol and you must do your own fragmentation at this level.
	 */

	if (unlikely(sock_flag(sk, SOCK_NOFCS))) {
		if (!netif_supports_nofcs(dev)) {
			err = -EPROTONOSUPPORT;
			goto out_unlock;
		}
		extra_len = 4; /* We're doing our own CRC */
	}

	err = -EMSGSIZE;
	if (len > dev->mtu + dev->hard_header_len + VLAN_HLEN + extra_len)
		goto out_unlock;

	if (!skb) {
		size_t reserved = LL_RESERVED_SPACE(dev);
		int tlen = dev->needed_tailroom;
		unsigned int hhlen = dev->header_ops ? dev->hard_header_len : 0;

		rcu_read_unlock();
		skb = sock_wmalloc(sk, len + reserved + tlen, 0, GFP_KERNEL);
		if (skb == NULL)
			return -ENOBUFS;
		/* FIXME: Save some space for broken drivers that write a hard
		 * header at transmission time by themselves. PPP is the notable
		 * one here. This should really be fixed at the driver level.
		 */
		skb_reserve(skb, reserved);
		skb_reset_network_header(skb);

		/* Try to align data part correctly */
		if (hhlen) {
			skb->data -= hhlen;
			skb->tail -= hhlen;
			if (len < hhlen)
				skb_reset_network_header(skb);
		}
<<<<<<< HEAD
		err = memcpy_fromiovec(skb_put(skb, len), msg->msg_iov, len);
=======
		err = memcpy_from_msg(skb_put(skb, len), msg, len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (err)
			goto out_free;
		goto retry;
	}

<<<<<<< HEAD
	if (len > (dev->mtu + dev->hard_header_len + extra_len)) {
		/* Earlier code assumed this would be a VLAN pkt,
		 * double-check this now that we have the actual
		 * packet in hand.
		 */
		struct ethhdr *ehdr;
		skb_reset_mac_header(skb);
		ehdr = eth_hdr(skb);
		if (ehdr->h_proto != htons(ETH_P_8021Q)) {
			err = -EMSGSIZE;
			goto out_unlock;
		}
=======
	if (!dev_validate_header(dev, skb->data, len) || !skb->len) {
		err = -EINVAL;
		goto out_unlock;
	}
	if (len > (dev->mtu + dev->hard_header_len + extra_len) &&
	    !packet_extra_vlan_len_allowed(dev, skb)) {
		err = -EMSGSIZE;
		goto out_unlock;
	}

	sockcm_init(&sockc, sk);
	if (msg->msg_controllen) {
		err = sock_cmsg_send(sk, msg, &sockc);
		if (unlikely(err))
			goto out_unlock;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	skb->protocol = proto;
	skb->dev = dev;
<<<<<<< HEAD
	skb->priority = sk->sk_priority;
	skb->mark = sk->sk_mark;
	err = sock_tx_timestamp(sk, &skb_shinfo(skb)->tx_flags);
	if (err < 0)
		goto out_unlock;
=======
	skb->priority = READ_ONCE(sk->sk_priority);
	skb->mark = READ_ONCE(sk->sk_mark);
	skb->tstamp = sockc.transmit_time;

	skb_setup_tx_timestamp(skb, sockc.tsflags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (unlikely(extra_len == 4))
		skb->no_fcs = 1;

<<<<<<< HEAD
=======
	packet_parse_headers(skb, sock);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dev_queue_xmit(skb);
	rcu_read_unlock();
	return len;

out_unlock:
	rcu_read_unlock();
out_free:
	kfree_skb(skb);
	return err;
}

<<<<<<< HEAD
static unsigned int run_filter(const struct sk_buff *skb,
				      const struct sock *sk,
				      unsigned int res)
=======
static unsigned int run_filter(struct sk_buff *skb,
			       const struct sock *sk,
			       unsigned int res)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct sk_filter *filter;

	rcu_read_lock();
	filter = rcu_dereference(sk->sk_filter);
	if (filter != NULL)
<<<<<<< HEAD
		res = SK_RUN_FILTER(filter, skb);
=======
		res = bpf_prog_run_clear_cb(filter->prog, skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rcu_read_unlock();

	return res;
}

<<<<<<< HEAD
=======
static int packet_rcv_vnet(struct msghdr *msg, const struct sk_buff *skb,
			   size_t *len, int vnet_hdr_sz)
{
	struct virtio_net_hdr_mrg_rxbuf vnet_hdr = { .num_buffers = 0 };

	if (*len < vnet_hdr_sz)
		return -EINVAL;
	*len -= vnet_hdr_sz;

	if (virtio_net_hdr_from_skb(skb, (struct virtio_net_hdr *)&vnet_hdr, vio_le(), true, 0))
		return -EINVAL;

	return memcpy_to_msg(msg, (void *)&vnet_hdr, vnet_hdr_sz);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * This function makes lazy skb cloning in hope that most of packets
 * are discarded by BPF.
 *
 * Note tricky part: we DO mangle shared skb! skb->data, skb->len
 * and skb->cb are mangled. It works because (and until) packets
 * falling here are owned by current CPU. Output packets are cloned
 * by dev_queue_xmit_nit(), input packets are processed by net_bh
<<<<<<< HEAD
 * sequencially, so that if we return skb to original state on exit,
=======
 * sequentially, so that if we return skb to original state on exit,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * we will not harm anyone.
 */

static int packet_rcv(struct sk_buff *skb, struct net_device *dev,
		      struct packet_type *pt, struct net_device *orig_dev)
{
<<<<<<< HEAD
=======
	enum skb_drop_reason drop_reason = SKB_CONSUMED;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct sock *sk;
	struct sockaddr_ll *sll;
	struct packet_sock *po;
	u8 *skb_head = skb->data;
	int skb_len = skb->len;
	unsigned int snaplen, res;

	if (skb->pkt_type == PACKET_LOOPBACK)
		goto drop;

	sk = pt->af_packet_priv;
	po = pkt_sk(sk);

	if (!net_eq(dev_net(dev), sock_net(sk)))
		goto drop;

	skb->dev = dev;

<<<<<<< HEAD
	if (dev->header_ops) {
=======
	if (dev_has_header(dev)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* The device has an explicit notion of ll header,
		 * exported to higher levels.
		 *
		 * Otherwise, the device hides details of its frame
		 * structure, so that corresponding packet head is
		 * never delivered to user.
		 */
		if (sk->sk_type != SOCK_DGRAM)
			skb_push(skb, skb->data - skb_mac_header(skb));
		else if (skb->pkt_type == PACKET_OUTGOING) {
			/* Special case: outgoing packets have ll header at head */
			skb_pull(skb, skb_network_offset(skb));
		}
	}

	snaplen = skb->len;

	res = run_filter(skb, sk, snaplen);
	if (!res)
		goto drop_n_restore;
	if (snaplen > res)
		snaplen = res;

	if (atomic_read(&sk->sk_rmem_alloc) >= sk->sk_rcvbuf)
		goto drop_n_acct;

	if (skb_shared(skb)) {
		struct sk_buff *nskb = skb_clone(skb, GFP_ATOMIC);
		if (nskb == NULL)
			goto drop_n_acct;

		if (skb_head != skb->data) {
			skb->data = skb_head;
			skb->len = skb_len;
		}
<<<<<<< HEAD
		kfree_skb(skb);
		skb = nskb;
	}

	BUILD_BUG_ON(sizeof(*PACKET_SKB_CB(skb)) + MAX_ADDR_LEN - 8 >
		     sizeof(skb->cb));

	sll = &PACKET_SKB_CB(skb)->sa.ll;
	sll->sll_family = AF_PACKET;
	sll->sll_hatype = dev->type;
	sll->sll_protocol = skb->protocol;
	sll->sll_pkttype = skb->pkt_type;
	if (unlikely(po->origdev))
=======
		consume_skb(skb);
		skb = nskb;
	}

	sock_skb_cb_check_size(sizeof(*PACKET_SKB_CB(skb)) + MAX_ADDR_LEN - 8);

	sll = &PACKET_SKB_CB(skb)->sa.ll;
	sll->sll_hatype = dev->type;
	sll->sll_pkttype = skb->pkt_type;
	if (unlikely(packet_sock_flag(po, PACKET_SOCK_ORIGDEV)))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		sll->sll_ifindex = orig_dev->ifindex;
	else
		sll->sll_ifindex = dev->ifindex;

	sll->sll_halen = dev_parse_header(skb, sll->sll_addr);

<<<<<<< HEAD
	PACKET_SKB_CB(skb)->origlen = skb->len;
=======
	/* sll->sll_family and sll->sll_protocol are set in packet_recvmsg().
	 * Use their space for storing the original skb length.
	 */
	PACKET_SKB_CB(skb)->sa.origlen = skb->len;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (pskb_trim(skb, snaplen))
		goto drop_n_acct;

	skb_set_owner_r(skb, sk);
	skb->dev = NULL;
	skb_dst_drop(skb);

	/* drop conntrack reference */
<<<<<<< HEAD
	nf_reset(skb);

	spin_lock(&sk->sk_receive_queue.lock);
	po->stats.tp_packets++;
	skb->dropcount = atomic_read(&sk->sk_drops);
	__skb_queue_tail(&sk->sk_receive_queue, skb);
	spin_unlock(&sk->sk_receive_queue.lock);
	sk->sk_data_ready(sk, skb->len);
	return 0;

drop_n_acct:
	spin_lock(&sk->sk_receive_queue.lock);
	po->stats.tp_drops++;
	atomic_inc(&sk->sk_drops);
	spin_unlock(&sk->sk_receive_queue.lock);
=======
	nf_reset_ct(skb);

	spin_lock(&sk->sk_receive_queue.lock);
	po->stats.stats1.tp_packets++;
	sock_skb_set_dropcount(sk, skb);
	skb_clear_delivery_time(skb);
	__skb_queue_tail(&sk->sk_receive_queue, skb);
	spin_unlock(&sk->sk_receive_queue.lock);
	sk->sk_data_ready(sk);
	return 0;

drop_n_acct:
	atomic_inc(&po->tp_drops);
	atomic_inc(&sk->sk_drops);
	drop_reason = SKB_DROP_REASON_PACKET_SOCK_ERROR;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

drop_n_restore:
	if (skb_head != skb->data && skb_shared(skb)) {
		skb->data = skb_head;
		skb->len = skb_len;
	}
drop:
<<<<<<< HEAD
	consume_skb(skb);
=======
	kfree_skb_reason(skb, drop_reason);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static int tpacket_rcv(struct sk_buff *skb, struct net_device *dev,
		       struct packet_type *pt, struct net_device *orig_dev)
{
<<<<<<< HEAD
	struct sock *sk;
	struct packet_sock *po;
	struct sockaddr_ll *sll;
	union {
		struct tpacket_hdr *h1;
		struct tpacket2_hdr *h2;
		struct tpacket3_hdr *h3;
		void *raw;
	} h;
=======
	enum skb_drop_reason drop_reason = SKB_CONSUMED;
	struct sock *sk;
	struct packet_sock *po;
	struct sockaddr_ll *sll;
	union tpacket_uhdr h;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u8 *skb_head = skb->data;
	int skb_len = skb->len;
	unsigned int snaplen, res;
	unsigned long status = TP_STATUS_USER;
<<<<<<< HEAD
	unsigned short macoff, netoff, hdrlen;
	struct sk_buff *copy_skb = NULL;
	struct timeval tv;
	struct timespec ts;
	struct skb_shared_hwtstamps *shhwtstamps = skb_hwtstamps(skb);
=======
	unsigned short macoff, hdrlen;
	unsigned int netoff;
	struct sk_buff *copy_skb = NULL;
	struct timespec64 ts;
	__u32 ts_status;
	unsigned int slot_id = 0;
	int vnet_hdr_sz = 0;

	/* struct tpacket{2,3}_hdr is aligned to a multiple of TPACKET_ALIGNMENT.
	 * We may add members to them until current aligned size without forcing
	 * userspace to call getsockopt(..., PACKET_HDRLEN, ...).
	 */
	BUILD_BUG_ON(TPACKET_ALIGN(sizeof(*h.h2)) != 32);
	BUILD_BUG_ON(TPACKET_ALIGN(sizeof(*h.h3)) != 48);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (skb->pkt_type == PACKET_LOOPBACK)
		goto drop;

	sk = pt->af_packet_priv;
	po = pkt_sk(sk);

	if (!net_eq(dev_net(dev), sock_net(sk)))
		goto drop;

<<<<<<< HEAD
	if (dev->header_ops) {
=======
	if (dev_has_header(dev)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (sk->sk_type != SOCK_DGRAM)
			skb_push(skb, skb->data - skb_mac_header(skb));
		else if (skb->pkt_type == PACKET_OUTGOING) {
			/* Special case: outgoing packets have ll header at head */
			skb_pull(skb, skb_network_offset(skb));
		}
	}

<<<<<<< HEAD
	if (skb->ip_summed == CHECKSUM_PARTIAL)
		status |= TP_STATUS_CSUMNOTREADY;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	snaplen = skb->len;

	res = run_filter(skb, sk, snaplen);
	if (!res)
		goto drop_n_restore;
<<<<<<< HEAD
=======

	/* If we are flooded, just give up */
	if (__packet_rcv_has_room(po, skb) == ROOM_NONE) {
		atomic_inc(&po->tp_drops);
		goto drop_n_restore;
	}

	if (skb->ip_summed == CHECKSUM_PARTIAL)
		status |= TP_STATUS_CSUMNOTREADY;
	else if (skb->pkt_type != PACKET_OUTGOING &&
		 skb_csum_unnecessary(skb))
		status |= TP_STATUS_CSUM_VALID;
	if (skb_is_gso(skb) && skb_is_gso_tcp(skb))
		status |= TP_STATUS_GSO_TCP;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (snaplen > res)
		snaplen = res;

	if (sk->sk_type == SOCK_DGRAM) {
		macoff = netoff = TPACKET_ALIGN(po->tp_hdrlen) + 16 +
				  po->tp_reserve;
	} else {
<<<<<<< HEAD
		unsigned maclen = skb_network_offset(skb);
		netoff = TPACKET_ALIGN(po->tp_hdrlen +
				       (maclen < 16 ? 16 : maclen)) +
			po->tp_reserve;
		macoff = netoff - maclen;
	}
	if (po->tp_version <= TPACKET_V2) {
		if (macoff + snaplen > po->rx_ring.frame_size) {
			if (po->copy_thresh &&
=======
		unsigned int maclen = skb_network_offset(skb);
		netoff = TPACKET_ALIGN(po->tp_hdrlen +
				       (maclen < 16 ? 16 : maclen)) +
				       po->tp_reserve;
		vnet_hdr_sz = READ_ONCE(po->vnet_hdr_sz);
		if (vnet_hdr_sz)
			netoff += vnet_hdr_sz;
		macoff = netoff - maclen;
	}
	if (netoff > USHRT_MAX) {
		atomic_inc(&po->tp_drops);
		goto drop_n_restore;
	}
	if (po->tp_version <= TPACKET_V2) {
		if (macoff + snaplen > po->rx_ring.frame_size) {
			if (READ_ONCE(po->copy_thresh) &&
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			    atomic_read(&sk->sk_rmem_alloc) < sk->sk_rcvbuf) {
				if (skb_shared(skb)) {
					copy_skb = skb_clone(skb, GFP_ATOMIC);
				} else {
					copy_skb = skb_get(skb);
					skb_head = skb->data;
				}
<<<<<<< HEAD
				if (copy_skb)
					skb_set_owner_r(copy_skb, sk);
			}
			snaplen = po->rx_ring.frame_size - macoff;
			if ((int)snaplen < 0)
				snaplen = 0;
=======
				if (copy_skb) {
					memset(&PACKET_SKB_CB(copy_skb)->sa.ll, 0,
					       sizeof(PACKET_SKB_CB(copy_skb)->sa.ll));
					skb_set_owner_r(copy_skb, sk);
				}
			}
			snaplen = po->rx_ring.frame_size - macoff;
			if ((int)snaplen < 0) {
				snaplen = 0;
				vnet_hdr_sz = 0;
			}
		}
	} else if (unlikely(macoff + snaplen >
			    GET_PBDQC_FROM_RB(&po->rx_ring)->max_frame_len)) {
		u32 nval;

		nval = GET_PBDQC_FROM_RB(&po->rx_ring)->max_frame_len - macoff;
		pr_err_once("tpacket_rcv: packet too big, clamped from %u to %u. macoff=%u\n",
			    snaplen, nval, macoff);
		snaplen = nval;
		if (unlikely((int)snaplen < 0)) {
			snaplen = 0;
			macoff = GET_PBDQC_FROM_RB(&po->rx_ring)->max_frame_len;
			vnet_hdr_sz = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}
	spin_lock(&sk->sk_receive_queue.lock);
	h.raw = packet_current_rx_frame(po, skb,
					TP_STATUS_KERNEL, (macoff+snaplen));
	if (!h.raw)
<<<<<<< HEAD
		goto ring_is_full;
=======
		goto drop_n_account;

	if (po->tp_version <= TPACKET_V2) {
		slot_id = po->rx_ring.head;
		if (test_bit(slot_id, po->rx_ring.rx_owner_map))
			goto drop_n_account;
		__set_bit(slot_id, po->rx_ring.rx_owner_map);
	}

	if (vnet_hdr_sz &&
	    virtio_net_hdr_from_skb(skb, h.raw + macoff -
				    sizeof(struct virtio_net_hdr),
				    vio_le(), true, 0)) {
		if (po->tp_version == TPACKET_V3)
			prb_clear_blk_fill_status(&po->rx_ring);
		goto drop_n_account;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (po->tp_version <= TPACKET_V2) {
		packet_increment_rx_head(po, &po->rx_ring);
	/*
	 * LOSING will be reported till you read the stats,
	 * because it's COR - Clear On Read.
	 * Anyways, moving it for V1/V2 only as V3 doesn't need this
	 * at packet level.
	 */
<<<<<<< HEAD
		if (po->stats.tp_drops)
			status |= TP_STATUS_LOSING;
	}
	po->stats.tp_packets++;
	if (copy_skb) {
		status |= TP_STATUS_COPY;
=======
		if (atomic_read(&po->tp_drops))
			status |= TP_STATUS_LOSING;
	}

	po->stats.stats1.tp_packets++;
	if (copy_skb) {
		status |= TP_STATUS_COPY;
		skb_clear_delivery_time(copy_skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		__skb_queue_tail(&sk->sk_receive_queue, copy_skb);
	}
	spin_unlock(&sk->sk_receive_queue.lock);

	skb_copy_bits(skb, 0, h.raw + macoff, snaplen);

<<<<<<< HEAD
=======
	/* Always timestamp; prefer an existing software timestamp taken
	 * closer to the time of capture.
	 */
	ts_status = tpacket_get_timestamp(skb, &ts,
					  READ_ONCE(po->tp_tstamp) |
					  SOF_TIMESTAMPING_SOFTWARE);
	if (!ts_status)
		ktime_get_real_ts64(&ts);

	status |= ts_status;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	switch (po->tp_version) {
	case TPACKET_V1:
		h.h1->tp_len = skb->len;
		h.h1->tp_snaplen = snaplen;
		h.h1->tp_mac = macoff;
		h.h1->tp_net = netoff;
<<<<<<< HEAD
		if ((po->tp_tstamp & SOF_TIMESTAMPING_SYS_HARDWARE)
				&& shhwtstamps->syststamp.tv64)
			tv = ktime_to_timeval(shhwtstamps->syststamp);
		else if ((po->tp_tstamp & SOF_TIMESTAMPING_RAW_HARDWARE)
				&& shhwtstamps->hwtstamp.tv64)
			tv = ktime_to_timeval(shhwtstamps->hwtstamp);
		else if (skb->tstamp.tv64)
			tv = ktime_to_timeval(skb->tstamp);
		else
			do_gettimeofday(&tv);
		h.h1->tp_sec = tv.tv_sec;
		h.h1->tp_usec = tv.tv_usec;
=======
		h.h1->tp_sec = ts.tv_sec;
		h.h1->tp_usec = ts.tv_nsec / NSEC_PER_USEC;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		hdrlen = sizeof(*h.h1);
		break;
	case TPACKET_V2:
		h.h2->tp_len = skb->len;
		h.h2->tp_snaplen = snaplen;
		h.h2->tp_mac = macoff;
		h.h2->tp_net = netoff;
<<<<<<< HEAD
		if ((po->tp_tstamp & SOF_TIMESTAMPING_SYS_HARDWARE)
				&& shhwtstamps->syststamp.tv64)
			ts = ktime_to_timespec(shhwtstamps->syststamp);
		else if ((po->tp_tstamp & SOF_TIMESTAMPING_RAW_HARDWARE)
				&& shhwtstamps->hwtstamp.tv64)
			ts = ktime_to_timespec(shhwtstamps->hwtstamp);
		else if (skb->tstamp.tv64)
			ts = ktime_to_timespec(skb->tstamp);
		else
			getnstimeofday(&ts);
		h.h2->tp_sec = ts.tv_sec;
		h.h2->tp_nsec = ts.tv_nsec;
		if (vlan_tx_tag_present(skb)) {
			h.h2->tp_vlan_tci = vlan_tx_tag_get(skb);
			status |= TP_STATUS_VLAN_VALID;
		} else {
			h.h2->tp_vlan_tci = 0;
		}
		h.h2->tp_padding = 0;
=======
		h.h2->tp_sec = ts.tv_sec;
		h.h2->tp_nsec = ts.tv_nsec;
		if (skb_vlan_tag_present(skb)) {
			h.h2->tp_vlan_tci = skb_vlan_tag_get(skb);
			h.h2->tp_vlan_tpid = ntohs(skb->vlan_proto);
			status |= TP_STATUS_VLAN_VALID | TP_STATUS_VLAN_TPID_VALID;
		} else {
			h.h2->tp_vlan_tci = 0;
			h.h2->tp_vlan_tpid = 0;
		}
		memset(h.h2->tp_padding, 0, sizeof(h.h2->tp_padding));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		hdrlen = sizeof(*h.h2);
		break;
	case TPACKET_V3:
		/* tp_nxt_offset,vlan are already populated above.
		 * So DONT clear those fields here
		 */
		h.h3->tp_status |= status;
		h.h3->tp_len = skb->len;
		h.h3->tp_snaplen = snaplen;
		h.h3->tp_mac = macoff;
		h.h3->tp_net = netoff;
<<<<<<< HEAD
		if ((po->tp_tstamp & SOF_TIMESTAMPING_SYS_HARDWARE)
				&& shhwtstamps->syststamp.tv64)
			ts = ktime_to_timespec(shhwtstamps->syststamp);
		else if ((po->tp_tstamp & SOF_TIMESTAMPING_RAW_HARDWARE)
				&& shhwtstamps->hwtstamp.tv64)
			ts = ktime_to_timespec(shhwtstamps->hwtstamp);
		else if (skb->tstamp.tv64)
			ts = ktime_to_timespec(skb->tstamp);
		else
			getnstimeofday(&ts);
		h.h3->tp_sec  = ts.tv_sec;
		h.h3->tp_nsec = ts.tv_nsec;
=======
		h.h3->tp_sec  = ts.tv_sec;
		h.h3->tp_nsec = ts.tv_nsec;
		memset(h.h3->tp_padding, 0, sizeof(h.h3->tp_padding));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		hdrlen = sizeof(*h.h3);
		break;
	default:
		BUG();
	}

	sll = h.raw + TPACKET_ALIGN(hdrlen);
	sll->sll_halen = dev_parse_header(skb, sll->sll_addr);
	sll->sll_family = AF_PACKET;
	sll->sll_hatype = dev->type;
	sll->sll_protocol = skb->protocol;
	sll->sll_pkttype = skb->pkt_type;
<<<<<<< HEAD
	if (unlikely(po->origdev))
=======
	if (unlikely(packet_sock_flag(po, PACKET_SOCK_ORIGDEV)))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		sll->sll_ifindex = orig_dev->ifindex;
	else
		sll->sll_ifindex = dev->ifindex;

	smp_mb();
<<<<<<< HEAD
#if ARCH_IMPLEMENTS_FLUSH_DCACHE_PAGE == 1
	{
		u8 *start, *end;

		if (po->tp_version <= TPACKET_V2) {
			end = (u8 *)PAGE_ALIGN((unsigned long)h.raw
				+ macoff + snaplen);
			for (start = h.raw; start < end; start += PAGE_SIZE)
				flush_dcache_page(pgv_to_page(start));
		}
		smp_wmb();
	}
#endif
	if (po->tp_version <= TPACKET_V2)
		__packet_set_status(po, h.raw, status);
	else
		prb_clear_blk_fill_status(&po->rx_ring);

	sk->sk_data_ready(sk, 0);
=======

#if ARCH_IMPLEMENTS_FLUSH_DCACHE_PAGE == 1
	if (po->tp_version <= TPACKET_V2) {
		u8 *start, *end;

		end = (u8 *) PAGE_ALIGN((unsigned long) h.raw +
					macoff + snaplen);

		for (start = h.raw; start < end; start += PAGE_SIZE)
			flush_dcache_page(pgv_to_page(start));
	}
	smp_wmb();
#endif

	if (po->tp_version <= TPACKET_V2) {
		spin_lock(&sk->sk_receive_queue.lock);
		__packet_set_status(po, h.raw, status);
		__clear_bit(slot_id, po->rx_ring.rx_owner_map);
		spin_unlock(&sk->sk_receive_queue.lock);
		sk->sk_data_ready(sk);
	} else if (po->tp_version == TPACKET_V3) {
		prb_clear_blk_fill_status(&po->rx_ring);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

drop_n_restore:
	if (skb_head != skb->data && skb_shared(skb)) {
		skb->data = skb_head;
		skb->len = skb_len;
	}
drop:
<<<<<<< HEAD
	kfree_skb(skb);
	return 0;

ring_is_full:
	po->stats.tp_drops++;
	spin_unlock(&sk->sk_receive_queue.lock);

	sk->sk_data_ready(sk, 0);
	kfree_skb(copy_skb);
=======
	kfree_skb_reason(skb, drop_reason);
	return 0;

drop_n_account:
	spin_unlock(&sk->sk_receive_queue.lock);
	atomic_inc(&po->tp_drops);
	drop_reason = SKB_DROP_REASON_PACKET_SOCK_ERROR;

	sk->sk_data_ready(sk);
	kfree_skb_reason(copy_skb, drop_reason);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	goto drop_n_restore;
}

static void tpacket_destruct_skb(struct sk_buff *skb)
{
	struct packet_sock *po = pkt_sk(skb->sk);
<<<<<<< HEAD
	void *ph;

	if (likely(po->tx_ring.pg_vec)) {
		ph = skb_shinfo(skb)->destructor_arg;
		BUG_ON(atomic_read(&po->tx_ring.pending) == 0);
		atomic_dec(&po->tx_ring.pending);
		__packet_set_status(po, ph, TP_STATUS_AVAILABLE);
=======

	if (likely(po->tx_ring.pg_vec)) {
		void *ph;
		__u32 ts;

		ph = skb_zcopy_get_nouarg(skb);
		packet_dec_pending(&po->tx_ring);

		ts = __packet_set_timestamp(po, ph, skb);
		__packet_set_status(po, ph, TP_STATUS_AVAILABLE | ts);

		if (!packet_read_pending(&po->tx_ring))
			complete(&po->skb_completion);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	sock_wfree(skb);
}

<<<<<<< HEAD
static int tpacket_fill_skb(struct packet_sock *po, struct sk_buff *skb,
		void *frame, struct net_device *dev, int size_max,
		__be16 proto, unsigned char *addr, int hlen)
{
	union {
		struct tpacket_hdr *h1;
		struct tpacket2_hdr *h2;
		void *raw;
	} ph;
	int to_write, offset, len, tp_len, nr_frags, len_max;
	struct socket *sock = po->sk.sk_socket;
	struct page *page;
	void *data;
=======
static int __packet_snd_vnet_parse(struct virtio_net_hdr *vnet_hdr, size_t len)
{
	if ((vnet_hdr->flags & VIRTIO_NET_HDR_F_NEEDS_CSUM) &&
	    (__virtio16_to_cpu(vio_le(), vnet_hdr->csum_start) +
	     __virtio16_to_cpu(vio_le(), vnet_hdr->csum_offset) + 2 >
	      __virtio16_to_cpu(vio_le(), vnet_hdr->hdr_len)))
		vnet_hdr->hdr_len = __cpu_to_virtio16(vio_le(),
			 __virtio16_to_cpu(vio_le(), vnet_hdr->csum_start) +
			__virtio16_to_cpu(vio_le(), vnet_hdr->csum_offset) + 2);

	if (__virtio16_to_cpu(vio_le(), vnet_hdr->hdr_len) > len)
		return -EINVAL;

	return 0;
}

static int packet_snd_vnet_parse(struct msghdr *msg, size_t *len,
				 struct virtio_net_hdr *vnet_hdr, int vnet_hdr_sz)
{
	int ret;

	if (*len < vnet_hdr_sz)
		return -EINVAL;
	*len -= vnet_hdr_sz;

	if (!copy_from_iter_full(vnet_hdr, sizeof(*vnet_hdr), &msg->msg_iter))
		return -EFAULT;

	ret = __packet_snd_vnet_parse(vnet_hdr, *len);
	if (ret)
		return ret;

	/* move iter to point to the start of mac header */
	if (vnet_hdr_sz != sizeof(struct virtio_net_hdr))
		iov_iter_advance(&msg->msg_iter, vnet_hdr_sz - sizeof(struct virtio_net_hdr));

	return 0;
}

static int tpacket_fill_skb(struct packet_sock *po, struct sk_buff *skb,
		void *frame, struct net_device *dev, void *data, int tp_len,
		__be16 proto, unsigned char *addr, int hlen, int copylen,
		const struct sockcm_cookie *sockc)
{
	union tpacket_uhdr ph;
	int to_write, offset, len, nr_frags, len_max;
	struct socket *sock = po->sk.sk_socket;
	struct page *page;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int err;

	ph.raw = frame;

	skb->protocol = proto;
	skb->dev = dev;
<<<<<<< HEAD
	skb->priority = po->sk.sk_priority;
	skb->mark = po->sk.sk_mark;
	skb_shinfo(skb)->destructor_arg = ph.raw;

	switch (po->tp_version) {
	case TPACKET_V2:
		tp_len = ph.h2->tp_len;
		break;
	default:
		tp_len = ph.h1->tp_len;
		break;
	}
	if (unlikely(tp_len > size_max)) {
		pr_err("packet size is too long (%d > %d)\n", tp_len, size_max);
		return -EMSGSIZE;
	}
=======
	skb->priority = READ_ONCE(po->sk.sk_priority);
	skb->mark = READ_ONCE(po->sk.sk_mark);
	skb->tstamp = sockc->transmit_time;
	skb_setup_tx_timestamp(skb, sockc->tsflags);
	skb_zcopy_set_nouarg(skb, ph.raw);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	skb_reserve(skb, hlen);
	skb_reset_network_header(skb);

<<<<<<< HEAD
	data = ph.raw + po->tp_hdrlen - sizeof(struct sockaddr_ll);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	to_write = tp_len;

	if (sock->type == SOCK_DGRAM) {
		err = dev_hard_header(skb, dev, ntohs(proto), addr,
				NULL, tp_len);
		if (unlikely(err < 0))
			return -EINVAL;
<<<<<<< HEAD
	} else if (dev->hard_header_len) {
		/* net device doesn't like empty head */
		if (unlikely(tp_len <= dev->hard_header_len)) {
			pr_err("packet size is too short (%d < %d)\n",
			       tp_len, dev->hard_header_len);
			return -EINVAL;
		}

		skb_push(skb, dev->hard_header_len);
		err = skb_store_bits(skb, 0, data,
				dev->hard_header_len);
		if (unlikely(err))
			return err;

		data += dev->hard_header_len;
		to_write -= dev->hard_header_len;
	}

	err = -EFAULT;
=======
	} else if (copylen) {
		int hdrlen = min_t(int, copylen, tp_len);

		skb_push(skb, dev->hard_header_len);
		skb_put(skb, copylen - dev->hard_header_len);
		err = skb_store_bits(skb, 0, data, hdrlen);
		if (unlikely(err))
			return err;
		if (!dev_validate_header(dev, skb->data, hdrlen))
			return -EINVAL;

		data += hdrlen;
		to_write -= hdrlen;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	offset = offset_in_page(data);
	len_max = PAGE_SIZE - offset;
	len = ((to_write > len_max) ? len_max : to_write);

	skb->data_len = to_write;
	skb->len += to_write;
	skb->truesize += to_write;
<<<<<<< HEAD
	atomic_add(to_write, &po->sk.sk_wmem_alloc);
=======
	refcount_add(to_write, &po->sk.sk_wmem_alloc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	while (likely(to_write)) {
		nr_frags = skb_shinfo(skb)->nr_frags;

		if (unlikely(nr_frags >= MAX_SKB_FRAGS)) {
<<<<<<< HEAD
			pr_err("Packet exceed the number of skb frags(%lu)\n",
			       MAX_SKB_FRAGS);
=======
			pr_err("Packet exceed the number of skb frags(%u)\n",
			       (unsigned int)MAX_SKB_FRAGS);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -EFAULT;
		}

		page = pgv_to_page(data);
		data += len;
		flush_dcache_page(page);
		get_page(page);
		skb_fill_page_desc(skb, nr_frags, page, offset, len);
		to_write -= len;
		offset = 0;
		len_max = PAGE_SIZE;
		len = ((to_write > len_max) ? len_max : to_write);
	}

<<<<<<< HEAD
	return tp_len;
}

static struct net_device *packet_cached_dev_get(struct packet_sock *po)
{
	struct net_device *dev;

	rcu_read_lock();
	dev = rcu_dereference(po->cached_dev);
	if (dev)
		dev_hold(dev);
	rcu_read_unlock();

	return dev;
=======
	packet_parse_headers(skb, sock);

	return tp_len;
}

static int tpacket_parse_header(struct packet_sock *po, void *frame,
				int size_max, void **data)
{
	union tpacket_uhdr ph;
	int tp_len, off;

	ph.raw = frame;

	switch (po->tp_version) {
	case TPACKET_V3:
		if (ph.h3->tp_next_offset != 0) {
			pr_warn_once("variable sized slot not supported");
			return -EINVAL;
		}
		tp_len = ph.h3->tp_len;
		break;
	case TPACKET_V2:
		tp_len = ph.h2->tp_len;
		break;
	default:
		tp_len = ph.h1->tp_len;
		break;
	}
	if (unlikely(tp_len > size_max)) {
		pr_err("packet size is too long (%d > %d)\n", tp_len, size_max);
		return -EMSGSIZE;
	}

	if (unlikely(packet_sock_flag(po, PACKET_SOCK_TX_HAS_OFF))) {
		int off_min, off_max;

		off_min = po->tp_hdrlen - sizeof(struct sockaddr_ll);
		off_max = po->tx_ring.frame_size - tp_len;
		if (po->sk.sk_type == SOCK_DGRAM) {
			switch (po->tp_version) {
			case TPACKET_V3:
				off = ph.h3->tp_net;
				break;
			case TPACKET_V2:
				off = ph.h2->tp_net;
				break;
			default:
				off = ph.h1->tp_net;
				break;
			}
		} else {
			switch (po->tp_version) {
			case TPACKET_V3:
				off = ph.h3->tp_mac;
				break;
			case TPACKET_V2:
				off = ph.h2->tp_mac;
				break;
			default:
				off = ph.h1->tp_mac;
				break;
			}
		}
		if (unlikely((off < off_min) || (off_max < off)))
			return -EINVAL;
	} else {
		off = po->tp_hdrlen - sizeof(struct sockaddr_ll);
	}

	*data = frame + off;
	return tp_len;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int tpacket_snd(struct packet_sock *po, struct msghdr *msg)
{
<<<<<<< HEAD
	struct sk_buff *skb;
	struct net_device *dev;
	__be16 proto;
	int err, reserve = 0;
	void *ph;
	struct sockaddr_ll *saddr = (struct sockaddr_ll *)msg->msg_name;
	int tp_len, size_max;
	unsigned char *addr;
	int len_sum = 0;
	int status = 0;
	int hlen, tlen;

	mutex_lock(&po->pg_vec_lock);

	err = -EBUSY;
	if (saddr == NULL) {
		dev	= packet_cached_dev_get(po);
		proto	= po->num;
		addr	= NULL;
=======
	struct sk_buff *skb = NULL;
	struct net_device *dev;
	struct virtio_net_hdr *vnet_hdr = NULL;
	struct sockcm_cookie sockc;
	__be16 proto;
	int err, reserve = 0;
	void *ph;
	DECLARE_SOCKADDR(struct sockaddr_ll *, saddr, msg->msg_name);
	bool need_wait = !(msg->msg_flags & MSG_DONTWAIT);
	int vnet_hdr_sz = READ_ONCE(po->vnet_hdr_sz);
	unsigned char *addr = NULL;
	int tp_len, size_max;
	void *data;
	int len_sum = 0;
	int status = TP_STATUS_AVAILABLE;
	int hlen, tlen, copylen = 0;
	long timeo = 0;

	mutex_lock(&po->pg_vec_lock);

	/* packet_sendmsg() check on tx_ring.pg_vec was lockless,
	 * we need to confirm it under protection of pg_vec_lock.
	 */
	if (unlikely(!po->tx_ring.pg_vec)) {
		err = -EBUSY;
		goto out;
	}
	if (likely(saddr == NULL)) {
		dev	= packet_cached_dev_get(po);
		proto	= READ_ONCE(po->num);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		err = -EINVAL;
		if (msg->msg_namelen < sizeof(struct sockaddr_ll))
			goto out;
		if (msg->msg_namelen < (saddr->sll_halen
					+ offsetof(struct sockaddr_ll,
						sll_addr)))
			goto out;
		proto	= saddr->sll_protocol;
<<<<<<< HEAD
		addr	= saddr->sll_addr;
		dev = dev_get_by_index(sock_net(&po->sk), saddr->sll_ifindex);
=======
		dev = dev_get_by_index(sock_net(&po->sk), saddr->sll_ifindex);
		if (po->sk.sk_socket->type == SOCK_DGRAM) {
			if (dev && msg->msg_namelen < dev->addr_len +
				   offsetof(struct sockaddr_ll, sll_addr))
				goto out_put;
			addr = saddr->sll_addr;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	err = -ENXIO;
	if (unlikely(dev == NULL))
		goto out;
	err = -ENETDOWN;
	if (unlikely(!(dev->flags & IFF_UP)))
		goto out_put;

<<<<<<< HEAD
	reserve = dev->hard_header_len;

	size_max = po->tx_ring.frame_size
		- (po->tp_hdrlen - sizeof(struct sockaddr_ll));

	if (size_max > dev->mtu + reserve)
		size_max = dev->mtu + reserve;

	do {
		ph = packet_current_frame(po, &po->tx_ring,
				TP_STATUS_SEND_REQUEST);

		if (unlikely(ph == NULL)) {
			schedule();
			continue;
		}

		status = TP_STATUS_SEND_REQUEST;
		hlen = LL_RESERVED_SPACE(dev);
		tlen = dev->needed_tailroom;
		skb = sock_alloc_send_skb(&po->sk,
				hlen + tlen + sizeof(struct sockaddr_ll),
				0, &err);

		if (unlikely(skb == NULL))
			goto out_status;

		tp_len = tpacket_fill_skb(po, skb, ph, dev, size_max, proto,
				addr, hlen);

		if (unlikely(tp_len < 0)) {
			if (po->tp_loss) {
=======
	sockcm_init(&sockc, &po->sk);
	if (msg->msg_controllen) {
		err = sock_cmsg_send(&po->sk, msg, &sockc);
		if (unlikely(err))
			goto out_put;
	}

	if (po->sk.sk_socket->type == SOCK_RAW)
		reserve = dev->hard_header_len;
	size_max = po->tx_ring.frame_size
		- (po->tp_hdrlen - sizeof(struct sockaddr_ll));

	if ((size_max > dev->mtu + reserve + VLAN_HLEN) && !vnet_hdr_sz)
		size_max = dev->mtu + reserve + VLAN_HLEN;

	reinit_completion(&po->skb_completion);

	do {
		ph = packet_current_frame(po, &po->tx_ring,
					  TP_STATUS_SEND_REQUEST);
		if (unlikely(ph == NULL)) {
			if (need_wait && skb) {
				timeo = sock_sndtimeo(&po->sk, msg->msg_flags & MSG_DONTWAIT);
				timeo = wait_for_completion_interruptible_timeout(&po->skb_completion, timeo);
				if (timeo <= 0) {
					err = !timeo ? -ETIMEDOUT : -ERESTARTSYS;
					goto out_put;
				}
			}
			/* check for additional frames */
			continue;
		}

		skb = NULL;
		tp_len = tpacket_parse_header(po, ph, size_max, &data);
		if (tp_len < 0)
			goto tpacket_error;

		status = TP_STATUS_SEND_REQUEST;
		hlen = LL_RESERVED_SPACE(dev);
		tlen = dev->needed_tailroom;
		if (vnet_hdr_sz) {
			vnet_hdr = data;
			data += vnet_hdr_sz;
			tp_len -= vnet_hdr_sz;
			if (tp_len < 0 ||
			    __packet_snd_vnet_parse(vnet_hdr, tp_len)) {
				tp_len = -EINVAL;
				goto tpacket_error;
			}
			copylen = __virtio16_to_cpu(vio_le(),
						    vnet_hdr->hdr_len);
		}
		copylen = max_t(int, copylen, dev->hard_header_len);
		skb = sock_alloc_send_skb(&po->sk,
				hlen + tlen + sizeof(struct sockaddr_ll) +
				(copylen - dev->hard_header_len),
				!need_wait, &err);

		if (unlikely(skb == NULL)) {
			/* we assume the socket was initially writeable ... */
			if (likely(len_sum > 0))
				err = len_sum;
			goto out_status;
		}
		tp_len = tpacket_fill_skb(po, skb, ph, dev, data, tp_len, proto,
					  addr, hlen, copylen, &sockc);
		if (likely(tp_len >= 0) &&
		    tp_len > dev->mtu + reserve &&
		    !vnet_hdr_sz &&
		    !packet_extra_vlan_len_allowed(dev, skb))
			tp_len = -EMSGSIZE;

		if (unlikely(tp_len < 0)) {
tpacket_error:
			if (packet_sock_flag(po, PACKET_SOCK_TP_LOSS)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				__packet_set_status(po, ph,
						TP_STATUS_AVAILABLE);
				packet_increment_head(&po->tx_ring);
				kfree_skb(skb);
				continue;
			} else {
				status = TP_STATUS_WRONG_FORMAT;
				err = tp_len;
				goto out_status;
			}
		}

<<<<<<< HEAD
		skb->destructor = tpacket_destruct_skb;
		__packet_set_status(po, ph, TP_STATUS_SENDING);
		atomic_inc(&po->tx_ring.pending);

		status = TP_STATUS_SEND_REQUEST;
		err = dev_queue_xmit(skb);
		if (unlikely(err > 0)) {
			err = net_xmit_errno(err);
=======
		if (vnet_hdr_sz) {
			if (virtio_net_hdr_to_skb(skb, vnet_hdr, vio_le())) {
				tp_len = -EINVAL;
				goto tpacket_error;
			}
			virtio_net_hdr_set_proto(skb, vnet_hdr);
		}

		skb->destructor = tpacket_destruct_skb;
		__packet_set_status(po, ph, TP_STATUS_SENDING);
		packet_inc_pending(&po->tx_ring);

		status = TP_STATUS_SEND_REQUEST;
		err = packet_xmit(po, skb);
		if (unlikely(err != 0)) {
			if (err > 0)
				err = net_xmit_errno(err);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (err && __packet_get_status(po, ph) ==
				   TP_STATUS_AVAILABLE) {
				/* skb was destructed already */
				skb = NULL;
				goto out_status;
			}
			/*
			 * skb was dropped but not destructed yet;
			 * let's treat it like congestion or err < 0
			 */
			err = 0;
		}
		packet_increment_head(&po->tx_ring);
		len_sum += tp_len;
	} while (likely((ph != NULL) ||
<<<<<<< HEAD
			((!(msg->msg_flags & MSG_DONTWAIT)) &&
			 (atomic_read(&po->tx_ring.pending))))
		);
=======
		/* Note: packet_read_pending() might be slow if we have
		 * to call it as it's per_cpu variable, but in fast-path
		 * we already short-circuit the loop with the first
		 * condition, and luckily don't have to go that path
		 * anyway.
		 */
		 (need_wait && packet_read_pending(&po->tx_ring))));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	err = len_sum;
	goto out_put;

out_status:
	__packet_set_status(po, ph, status);
	kfree_skb(skb);
out_put:
	dev_put(dev);
out:
	mutex_unlock(&po->pg_vec_lock);
	return err;
}

static struct sk_buff *packet_alloc_skb(struct sock *sk, size_t prepad,
				        size_t reserve, size_t len,
				        size_t linear, int noblock,
				        int *err)
{
	struct sk_buff *skb;

	/* Under a page?  Don't bother with paged skb. */
	if (prepad + len < PAGE_SIZE || !linear)
		linear = len;

<<<<<<< HEAD
	skb = sock_alloc_send_pskb(sk, prepad + linear, len - linear, noblock,
				   err);
=======
	if (len - linear > MAX_SKB_FRAGS * (PAGE_SIZE << PAGE_ALLOC_COSTLY_ORDER))
		linear = len - MAX_SKB_FRAGS * (PAGE_SIZE << PAGE_ALLOC_COSTLY_ORDER);
	skb = sock_alloc_send_pskb(sk, prepad + linear, len - linear, noblock,
				   err, PAGE_ALLOC_COSTLY_ORDER);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!skb)
		return NULL;

	skb_reserve(skb, reserve);
	skb_put(skb, linear);
	skb->data_len = len - linear;
	skb->len += len - linear;

	return skb;
}

<<<<<<< HEAD
static int packet_snd(struct socket *sock,
			  struct msghdr *msg, size_t len)
{
	struct sock *sk = sock->sk;
	struct sockaddr_ll *saddr = (struct sockaddr_ll *)msg->msg_name;
	struct sk_buff *skb;
	struct net_device *dev;
	__be16 proto;
	unsigned char *addr;
	int err, reserve = 0;
	struct virtio_net_hdr vnet_hdr = { 0 };
	int offset = 0;
	int vnet_hdr_len;
	struct packet_sock *po = pkt_sk(sk);
	unsigned short gso_type = 0;
	int hlen, tlen;
=======
static int packet_snd(struct socket *sock, struct msghdr *msg, size_t len)
{
	struct sock *sk = sock->sk;
	DECLARE_SOCKADDR(struct sockaddr_ll *, saddr, msg->msg_name);
	struct sk_buff *skb;
	struct net_device *dev;
	__be16 proto;
	unsigned char *addr = NULL;
	int err, reserve = 0;
	struct sockcm_cookie sockc;
	struct virtio_net_hdr vnet_hdr = { 0 };
	int offset = 0;
	struct packet_sock *po = pkt_sk(sk);
	int vnet_hdr_sz = READ_ONCE(po->vnet_hdr_sz);
	int hlen, tlen, linear;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int extra_len = 0;

	/*
	 *	Get and verify the address.
	 */

<<<<<<< HEAD
	if (saddr == NULL) {
		dev	= packet_cached_dev_get(po);
		proto	= po->num;
		addr	= NULL;
=======
	if (likely(saddr == NULL)) {
		dev	= packet_cached_dev_get(po);
		proto	= READ_ONCE(po->num);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		err = -EINVAL;
		if (msg->msg_namelen < sizeof(struct sockaddr_ll))
			goto out;
		if (msg->msg_namelen < (saddr->sll_halen + offsetof(struct sockaddr_ll, sll_addr)))
			goto out;
		proto	= saddr->sll_protocol;
<<<<<<< HEAD
		addr	= saddr->sll_addr;
		dev = dev_get_by_index(sock_net(sk), saddr->sll_ifindex);
=======
		dev = dev_get_by_index(sock_net(sk), saddr->sll_ifindex);
		if (sock->type == SOCK_DGRAM) {
			if (dev && msg->msg_namelen < dev->addr_len +
				   offsetof(struct sockaddr_ll, sll_addr))
				goto out_unlock;
			addr = saddr->sll_addr;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	err = -ENXIO;
	if (unlikely(dev == NULL))
		goto out_unlock;
	err = -ENETDOWN;
	if (unlikely(!(dev->flags & IFF_UP)))
		goto out_unlock;

<<<<<<< HEAD
	if (sock->type == SOCK_RAW)
		reserve = dev->hard_header_len;
	if (po->has_vnet_hdr) {
		vnet_hdr_len = sizeof(vnet_hdr);

		err = -EINVAL;
		if (len < vnet_hdr_len)
			goto out_unlock;

		len -= vnet_hdr_len;

		err = memcpy_fromiovec((void *)&vnet_hdr, msg->msg_iov,
				       vnet_hdr_len);
		if (err < 0)
			goto out_unlock;

		if ((vnet_hdr.flags & VIRTIO_NET_HDR_F_NEEDS_CSUM) &&
		    (vnet_hdr.csum_start + vnet_hdr.csum_offset + 2 >
		      vnet_hdr.hdr_len))
			vnet_hdr.hdr_len = vnet_hdr.csum_start +
						 vnet_hdr.csum_offset + 2;

		err = -EINVAL;
		if (vnet_hdr.hdr_len > len)
			goto out_unlock;

		if (vnet_hdr.gso_type != VIRTIO_NET_HDR_GSO_NONE) {
			switch (vnet_hdr.gso_type & ~VIRTIO_NET_HDR_GSO_ECN) {
			case VIRTIO_NET_HDR_GSO_TCPV4:
				gso_type = SKB_GSO_TCPV4;
				break;
			case VIRTIO_NET_HDR_GSO_TCPV6:
				gso_type = SKB_GSO_TCPV6;
				break;
			case VIRTIO_NET_HDR_GSO_UDP:
				gso_type = SKB_GSO_UDP;
				break;
			default:
				goto out_unlock;
			}

			if (vnet_hdr.gso_type & VIRTIO_NET_HDR_GSO_ECN)
				gso_type |= SKB_GSO_TCP_ECN;

			if (vnet_hdr.gso_size == 0)
				goto out_unlock;

		}
=======
	sockcm_init(&sockc, sk);
	sockc.mark = READ_ONCE(sk->sk_mark);
	if (msg->msg_controllen) {
		err = sock_cmsg_send(sk, msg, &sockc);
		if (unlikely(err))
			goto out_unlock;
	}

	if (sock->type == SOCK_RAW)
		reserve = dev->hard_header_len;
	if (vnet_hdr_sz) {
		err = packet_snd_vnet_parse(msg, &len, &vnet_hdr, vnet_hdr_sz);
		if (err)
			goto out_unlock;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (unlikely(sock_flag(sk, SOCK_NOFCS))) {
		if (!netif_supports_nofcs(dev)) {
			err = -EPROTONOSUPPORT;
			goto out_unlock;
		}
		extra_len = 4; /* We're doing our own CRC */
	}

	err = -EMSGSIZE;
<<<<<<< HEAD
	if (!gso_type && (len > dev->mtu + reserve + VLAN_HLEN + extra_len))
=======
	if (!vnet_hdr.gso_type &&
	    (len > dev->mtu + reserve + VLAN_HLEN + extra_len))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out_unlock;

	err = -ENOBUFS;
	hlen = LL_RESERVED_SPACE(dev);
	tlen = dev->needed_tailroom;
<<<<<<< HEAD
	skb = packet_alloc_skb(sk, hlen + tlen, hlen, len, vnet_hdr.hdr_len,
=======
	linear = __virtio16_to_cpu(vio_le(), vnet_hdr.hdr_len);
	linear = max(linear, min_t(int, len, dev->hard_header_len));
	skb = packet_alloc_skb(sk, hlen + tlen, hlen, len, linear,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			       msg->msg_flags & MSG_DONTWAIT, &err);
	if (skb == NULL)
		goto out_unlock;

<<<<<<< HEAD
	skb_set_network_header(skb, reserve);

	err = -EINVAL;
	if (sock->type == SOCK_DGRAM &&
	    (offset = dev_hard_header(skb, dev, ntohs(proto), addr, NULL, len)) < 0)
		goto out_free;

	/* Returns -EFAULT on error */
	err = skb_copy_datagram_from_iovec(skb, offset, msg->msg_iov, 0, len);
	if (err)
		goto out_free;
	err = sock_tx_timestamp(sk, &skb_shinfo(skb)->tx_flags);
	if (err < 0)
		goto out_free;

	if (!gso_type && (len > dev->mtu + reserve + extra_len)) {
		/* Earlier code assumed this would be a VLAN pkt,
		 * double-check this now that we have the actual
		 * packet in hand.
		 */
		struct ethhdr *ehdr;
		skb_reset_mac_header(skb);
		ehdr = eth_hdr(skb);
		if (ehdr->h_proto != htons(ETH_P_8021Q)) {
			err = -EMSGSIZE;
			goto out_free;
		}
=======
	skb_reset_network_header(skb);

	err = -EINVAL;
	if (sock->type == SOCK_DGRAM) {
		offset = dev_hard_header(skb, dev, ntohs(proto), addr, NULL, len);
		if (unlikely(offset < 0))
			goto out_free;
	} else if (reserve) {
		skb_reserve(skb, -reserve);
		if (len < reserve + sizeof(struct ipv6hdr) &&
		    dev->min_header_len != dev->hard_header_len)
			skb_reset_network_header(skb);
	}

	/* Returns -EFAULT on error */
	err = skb_copy_datagram_from_iter(skb, offset, &msg->msg_iter, len);
	if (err)
		goto out_free;

	if ((sock->type == SOCK_RAW &&
	     !dev_validate_header(dev, skb->data, len)) || !skb->len) {
		err = -EINVAL;
		goto out_free;
	}

	skb_setup_tx_timestamp(skb, sockc.tsflags);

	if (!vnet_hdr.gso_type && (len > dev->mtu + reserve + extra_len) &&
	    !packet_extra_vlan_len_allowed(dev, skb)) {
		err = -EMSGSIZE;
		goto out_free;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	skb->protocol = proto;
	skb->dev = dev;
<<<<<<< HEAD
	skb->priority = sk->sk_priority;
	skb->mark = sk->sk_mark;

	if (po->has_vnet_hdr) {
		if (vnet_hdr.flags & VIRTIO_NET_HDR_F_NEEDS_CSUM) {
			if (!skb_partial_csum_set(skb, vnet_hdr.csum_start,
						  vnet_hdr.csum_offset)) {
				err = -EINVAL;
				goto out_free;
			}
		}

		skb_shinfo(skb)->gso_size = vnet_hdr.gso_size;
		skb_shinfo(skb)->gso_type = gso_type;

		/* Header must be checked, and gso_segs computed. */
		skb_shinfo(skb)->gso_type |= SKB_GSO_DODGY;
		skb_shinfo(skb)->gso_segs = 0;

		len += vnet_hdr_len;
	}
=======
	skb->priority = READ_ONCE(sk->sk_priority);
	skb->mark = sockc.mark;
	skb->tstamp = sockc.transmit_time;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (unlikely(extra_len == 4))
		skb->no_fcs = 1;

<<<<<<< HEAD
	/*
	 *	Now send it
	 */

	err = dev_queue_xmit(skb);
	if (err > 0 && (err = net_xmit_errno(err)) != 0)
		goto out_unlock;
=======
	packet_parse_headers(skb, sock);

	if (vnet_hdr_sz) {
		err = virtio_net_hdr_to_skb(skb, &vnet_hdr, vio_le());
		if (err)
			goto out_free;
		len += vnet_hdr_sz;
		virtio_net_hdr_set_proto(skb, &vnet_hdr);
	}

	err = packet_xmit(po, skb);

	if (unlikely(err != 0)) {
		if (err > 0)
			err = net_xmit_errno(err);
		if (err)
			goto out_unlock;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	dev_put(dev);

	return len;

out_free:
	kfree_skb(skb);
out_unlock:
<<<<<<< HEAD
	if (dev)
		dev_put(dev);
=======
	dev_put(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	return err;
}

<<<<<<< HEAD
static int packet_sendmsg(struct kiocb *iocb, struct socket *sock,
		struct msghdr *msg, size_t len)
{
	struct sock *sk = sock->sk;
	struct packet_sock *po = pkt_sk(sk);
	if (po->tx_ring.pg_vec)
		return tpacket_snd(po, msg);
	else
		return packet_snd(sock, msg, len);
=======
static int packet_sendmsg(struct socket *sock, struct msghdr *msg, size_t len)
{
	struct sock *sk = sock->sk;
	struct packet_sock *po = pkt_sk(sk);

	/* Reading tx_ring.pg_vec without holding pg_vec_lock is racy.
	 * tpacket_snd() will redo the check safely.
	 */
	if (data_race(po->tx_ring.pg_vec))
		return tpacket_snd(po, msg);

	return packet_snd(sock, msg, len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 *	Close a PACKET socket. This is fairly simple. We immediately go
 *	to 'closed' state and remove our protocol entry in the device list.
 */

static int packet_release(struct socket *sock)
{
	struct sock *sk = sock->sk;
	struct packet_sock *po;
<<<<<<< HEAD
=======
	struct packet_fanout *f;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct net *net;
	union tpacket_req_u req_u;

	if (!sk)
		return 0;

	net = sock_net(sk);
	po = pkt_sk(sk);

<<<<<<< HEAD
	spin_lock_bh(&net->packet.sklist_lock);
	sk_del_node_init_rcu(sk);
	sock_prot_inuse_add(net, sk->sk_prot, -1);
	spin_unlock_bh(&net->packet.sklist_lock);

	spin_lock(&po->bind_lock);
	unregister_prot_hook(sk, false);
	if (po->prot_hook.dev) {
		dev_put(po->prot_hook.dev);
=======
	mutex_lock(&net->packet.sklist_lock);
	sk_del_node_init_rcu(sk);
	mutex_unlock(&net->packet.sklist_lock);

	sock_prot_inuse_add(net, sk->sk_prot, -1);

	spin_lock(&po->bind_lock);
	unregister_prot_hook(sk, false);
	packet_cached_dev_reset(po);

	if (po->prot_hook.dev) {
		netdev_put(po->prot_hook.dev, &po->prot_hook.dev_tracker);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		po->prot_hook.dev = NULL;
	}
	spin_unlock(&po->bind_lock);

	packet_flush_mclist(sk);

<<<<<<< HEAD
=======
	lock_sock(sk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (po->rx_ring.pg_vec) {
		memset(&req_u, 0, sizeof(req_u));
		packet_set_ring(sk, &req_u, 1, 0);
	}

	if (po->tx_ring.pg_vec) {
		memset(&req_u, 0, sizeof(req_u));
		packet_set_ring(sk, &req_u, 1, 1);
	}
<<<<<<< HEAD

	fanout_release(sk);

	synchronize_net();
=======
	release_sock(sk);

	f = fanout_release(sk);

	synchronize_net();

	kfree(po->rollover);
	if (f) {
		fanout_release_data(f);
		kvfree(f);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 *	Now the socket is dead. No more input will appear.
	 */
	sock_orphan(sk);
	sock->sk = NULL;

	/* Purge queues */

	skb_queue_purge(&sk->sk_receive_queue);
<<<<<<< HEAD
	sk_refcnt_debug_release(sk);
=======
	packet_free_pending(po);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	sock_put(sk);
	return 0;
}

/*
 *	Attach a packet hook.
 */

<<<<<<< HEAD
static int packet_do_bind(struct sock *sk, struct net_device *dev, __be16 protocol)
{
	struct packet_sock *po = pkt_sk(sk);
	int ret = 0;

	lock_sock(sk);

	spin_lock(&po->bind_lock);

	if (po->fanout) {
		if (dev)
			dev_put(dev);

=======
static int packet_do_bind(struct sock *sk, const char *name, int ifindex,
			  __be16 proto)
{
	struct packet_sock *po = pkt_sk(sk);
	struct net_device *dev = NULL;
	bool unlisted = false;
	bool need_rehook;
	int ret = 0;

	lock_sock(sk);
	spin_lock(&po->bind_lock);
	if (!proto)
		proto = po->num;

	rcu_read_lock();

	if (po->fanout) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = -EINVAL;
		goto out_unlock;
	}

<<<<<<< HEAD
	unregister_prot_hook(sk, true);
	po->num = protocol;
	po->prot_hook.type = protocol;
	if (po->prot_hook.dev)
		dev_put(po->prot_hook.dev);
	po->prot_hook.dev = dev;

	po->ifindex = dev ? dev->ifindex : 0;

	if (protocol == 0)
		goto out_unlock;

	if (!dev || (dev->flags & IFF_UP)) {
=======
	if (name) {
		dev = dev_get_by_name_rcu(sock_net(sk), name);
		if (!dev) {
			ret = -ENODEV;
			goto out_unlock;
		}
	} else if (ifindex) {
		dev = dev_get_by_index_rcu(sock_net(sk), ifindex);
		if (!dev) {
			ret = -ENODEV;
			goto out_unlock;
		}
	}

	need_rehook = po->prot_hook.type != proto || po->prot_hook.dev != dev;

	if (need_rehook) {
		dev_hold(dev);
		if (packet_sock_flag(po, PACKET_SOCK_RUNNING)) {
			rcu_read_unlock();
			/* prevents packet_notifier() from calling
			 * register_prot_hook()
			 */
			WRITE_ONCE(po->num, 0);
			__unregister_prot_hook(sk, true);
			rcu_read_lock();
			if (dev)
				unlisted = !dev_get_by_index_rcu(sock_net(sk),
								 dev->ifindex);
		}

		BUG_ON(packet_sock_flag(po, PACKET_SOCK_RUNNING));
		WRITE_ONCE(po->num, proto);
		po->prot_hook.type = proto;

		netdev_put(po->prot_hook.dev, &po->prot_hook.dev_tracker);

		if (unlikely(unlisted)) {
			po->prot_hook.dev = NULL;
			WRITE_ONCE(po->ifindex, -1);
			packet_cached_dev_reset(po);
		} else {
			netdev_hold(dev, &po->prot_hook.dev_tracker,
				    GFP_ATOMIC);
			po->prot_hook.dev = dev;
			WRITE_ONCE(po->ifindex, dev ? dev->ifindex : 0);
			packet_cached_dev_assign(po, dev);
		}
		dev_put(dev);
	}

	if (proto == 0 || !need_rehook)
		goto out_unlock;

	if (!unlisted && (!dev || (dev->flags & IFF_UP))) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		register_prot_hook(sk);
	} else {
		sk->sk_err = ENETDOWN;
		if (!sock_flag(sk, SOCK_DEAD))
<<<<<<< HEAD
			sk->sk_error_report(sk);
	}

out_unlock:
=======
			sk_error_report(sk);
	}

out_unlock:
	rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_unlock(&po->bind_lock);
	release_sock(sk);
	return ret;
}

/*
 *	Bind a packet socket to a device
 */

static int packet_bind_spkt(struct socket *sock, struct sockaddr *uaddr,
			    int addr_len)
{
	struct sock *sk = sock->sk;
<<<<<<< HEAD
	char name[15];
	struct net_device *dev;
	int err = -ENODEV;
=======
	char name[sizeof(uaddr->sa_data_min) + 1];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 *	Check legality
	 */

	if (addr_len != sizeof(struct sockaddr))
		return -EINVAL;
<<<<<<< HEAD
	strlcpy(name, uaddr->sa_data, sizeof(name));

	dev = dev_get_by_name(sock_net(sk), name);
	if (dev)
		err = packet_do_bind(sk, dev, pkt_sk(sk)->num);
	return err;
=======
	/* uaddr->sa_data comes from the userspace, it's not guaranteed to be
	 * zero-terminated.
	 */
	memcpy(name, uaddr->sa_data, sizeof(uaddr->sa_data_min));
	name[sizeof(uaddr->sa_data_min)] = 0;

	return packet_do_bind(sk, name, 0, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int packet_bind(struct socket *sock, struct sockaddr *uaddr, int addr_len)
{
	struct sockaddr_ll *sll = (struct sockaddr_ll *)uaddr;
	struct sock *sk = sock->sk;
<<<<<<< HEAD
	struct net_device *dev = NULL;
	int err;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 *	Check legality
	 */

	if (addr_len < sizeof(struct sockaddr_ll))
		return -EINVAL;
	if (sll->sll_family != AF_PACKET)
		return -EINVAL;

<<<<<<< HEAD
	if (sll->sll_ifindex) {
		err = -ENODEV;
		dev = dev_get_by_index(sock_net(sk), sll->sll_ifindex);
		if (dev == NULL)
			goto out;
	}
	err = packet_do_bind(sk, dev, sll->sll_protocol ? : pkt_sk(sk)->num);

out:
	return err;
=======
	return packet_do_bind(sk, NULL, sll->sll_ifindex, sll->sll_protocol);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct proto packet_proto = {
	.name	  = "PACKET",
	.owner	  = THIS_MODULE,
	.obj_size = sizeof(struct packet_sock),
};

/*
 *	Create a packet of type SOCK_PACKET.
 */

static int packet_create(struct net *net, struct socket *sock, int protocol,
			 int kern)
{
	struct sock *sk;
	struct packet_sock *po;
	__be16 proto = (__force __be16)protocol; /* weird, but documented */
	int err;

<<<<<<< HEAD
	if (!capable(CAP_NET_RAW))
=======
	if (!ns_capable(net->user_ns, CAP_NET_RAW))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EPERM;
	if (sock->type != SOCK_DGRAM && sock->type != SOCK_RAW &&
	    sock->type != SOCK_PACKET)
		return -ESOCKTNOSUPPORT;

	sock->state = SS_UNCONNECTED;

	err = -ENOBUFS;
<<<<<<< HEAD
	sk = sk_alloc(net, PF_PACKET, GFP_KERNEL, &packet_proto);
=======
	sk = sk_alloc(net, PF_PACKET, GFP_KERNEL, &packet_proto, kern);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (sk == NULL)
		goto out;

	sock->ops = &packet_ops;
	if (sock->type == SOCK_PACKET)
		sock->ops = &packet_ops_spkt;

	sock_init_data(sock, sk);

	po = pkt_sk(sk);
<<<<<<< HEAD
	sk->sk_family = PF_PACKET;
	po->num = proto;
	RCU_INIT_POINTER(po->cached_dev, NULL);

	sk->sk_destruct = packet_sock_destruct;
	sk_refcnt_debug_inc(sk);
=======
	init_completion(&po->skb_completion);
	sk->sk_family = PF_PACKET;
	po->num = proto;

	err = packet_alloc_pending(po);
	if (err)
		goto out2;

	packet_cached_dev_reset(po);

	sk->sk_destruct = packet_sock_destruct;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 *	Attach a protocol block
	 */

	spin_lock_init(&po->bind_lock);
	mutex_init(&po->pg_vec_lock);
<<<<<<< HEAD
=======
	po->rollover = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	po->prot_hook.func = packet_rcv;

	if (sock->type == SOCK_PACKET)
		po->prot_hook.func = packet_rcv_spkt;

	po->prot_hook.af_packet_priv = sk;
<<<<<<< HEAD

	if (proto) {
		po->prot_hook.type = proto;
		register_prot_hook(sk);
	}

	spin_lock_bh(&net->packet.sklist_lock);
	sk_add_node_rcu(sk, &net->packet.sklist);
	sock_prot_inuse_add(net, &packet_proto, 1);
	spin_unlock_bh(&net->packet.sklist_lock);

	return 0;
out:
	return err;
}

static int packet_recv_error(struct sock *sk, struct msghdr *msg, int len)
{
	struct sock_exterr_skb *serr;
	struct sk_buff *skb, *skb2;
	int copied, err;

	err = -EAGAIN;
	skb = skb_dequeue(&sk->sk_error_queue);
	if (skb == NULL)
		goto out;

	copied = skb->len;
	if (copied > len) {
		msg->msg_flags |= MSG_TRUNC;
		copied = len;
	}
	err = skb_copy_datagram_iovec(skb, 0, msg->msg_iov, copied);
	if (err)
		goto out_free_skb;

	sock_recv_timestamp(msg, sk, skb);

	serr = SKB_EXT_ERR(skb);
	put_cmsg(msg, SOL_PACKET, PACKET_TX_TIMESTAMP,
		 sizeof(serr->ee), &serr->ee);

	msg->msg_flags |= MSG_ERRQUEUE;
	err = copied;

	/* Reset and regenerate socket error */
	spin_lock_bh(&sk->sk_error_queue.lock);
	sk->sk_err = 0;
	if ((skb2 = skb_peek(&sk->sk_error_queue)) != NULL) {
		sk->sk_err = SKB_EXT_ERR(skb2)->ee.ee_errno;
		spin_unlock_bh(&sk->sk_error_queue.lock);
		sk->sk_error_report(sk);
	} else
		spin_unlock_bh(&sk->sk_error_queue.lock);

out_free_skb:
	kfree_skb(skb);
=======
	po->prot_hook.af_packet_net = sock_net(sk);

	if (proto) {
		po->prot_hook.type = proto;
		__register_prot_hook(sk);
	}

	mutex_lock(&net->packet.sklist_lock);
	sk_add_node_tail_rcu(sk, &net->packet.sklist);
	mutex_unlock(&net->packet.sklist_lock);

	sock_prot_inuse_add(net, &packet_proto, 1);

	return 0;
out2:
	sk_free(sk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	return err;
}

/*
 *	Pull a packet from our receive queue and hand it to the user.
 *	If necessary we block.
 */

<<<<<<< HEAD
static int packet_recvmsg(struct kiocb *iocb, struct socket *sock,
			  struct msghdr *msg, size_t len, int flags)
=======
static int packet_recvmsg(struct socket *sock, struct msghdr *msg, size_t len,
			  int flags)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct sock *sk = sock->sk;
	struct sk_buff *skb;
	int copied, err;
<<<<<<< HEAD
	int vnet_hdr_len = 0;
=======
	int vnet_hdr_len = READ_ONCE(pkt_sk(sk)->vnet_hdr_sz);
	unsigned int origlen = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	err = -EINVAL;
	if (flags & ~(MSG_PEEK|MSG_DONTWAIT|MSG_TRUNC|MSG_CMSG_COMPAT|MSG_ERRQUEUE))
		goto out;

#if 0
	/* What error should we return now? EUNATTACH? */
	if (pkt_sk(sk)->ifindex < 0)
		return -ENODEV;
#endif

	if (flags & MSG_ERRQUEUE) {
<<<<<<< HEAD
		err = packet_recv_error(sk, msg, len);
=======
		err = sock_recv_errqueue(sk, msg, len,
					 SOL_PACKET, PACKET_TX_TIMESTAMP);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;
	}

	/*
	 *	Call the generic datagram receiver. This handles all sorts
	 *	of horrible races and re-entrancy so we can forget about it
	 *	in the protocol layers.
	 *
	 *	Now it will return ENETDOWN, if device have just gone down,
	 *	but then it will block.
	 */

<<<<<<< HEAD
	skb = skb_recv_datagram(sk, flags, flags & MSG_DONTWAIT, &err);
=======
	skb = skb_recv_datagram(sk, flags, &err);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 *	An error occurred so return it. Because skb_recv_datagram()
	 *	handles the blocking we don't see and worry about blocking
	 *	retries.
	 */

	if (skb == NULL)
		goto out;

<<<<<<< HEAD
	if (pkt_sk(sk)->has_vnet_hdr) {
		struct virtio_net_hdr vnet_hdr = { 0 };

		err = -EINVAL;
		vnet_hdr_len = sizeof(vnet_hdr);
		if (len < vnet_hdr_len)
			goto out_free;

		len -= vnet_hdr_len;

		if (skb_is_gso(skb)) {
			struct skb_shared_info *sinfo = skb_shinfo(skb);

			/* This is a hint as to how much should be linear. */
			vnet_hdr.hdr_len = skb_headlen(skb);
			vnet_hdr.gso_size = sinfo->gso_size;
			if (sinfo->gso_type & SKB_GSO_TCPV4)
				vnet_hdr.gso_type = VIRTIO_NET_HDR_GSO_TCPV4;
			else if (sinfo->gso_type & SKB_GSO_TCPV6)
				vnet_hdr.gso_type = VIRTIO_NET_HDR_GSO_TCPV6;
			else if (sinfo->gso_type & SKB_GSO_UDP)
				vnet_hdr.gso_type = VIRTIO_NET_HDR_GSO_UDP;
			else if (sinfo->gso_type & SKB_GSO_FCOE)
				goto out_free;
			else
				BUG();
			if (sinfo->gso_type & SKB_GSO_TCP_ECN)
				vnet_hdr.gso_type |= VIRTIO_NET_HDR_GSO_ECN;
		} else
			vnet_hdr.gso_type = VIRTIO_NET_HDR_GSO_NONE;

		if (skb->ip_summed == CHECKSUM_PARTIAL) {
			vnet_hdr.flags = VIRTIO_NET_HDR_F_NEEDS_CSUM;
			vnet_hdr.csum_start = skb_checksum_start_offset(skb);
			vnet_hdr.csum_offset = skb->csum_offset;
		} else if (skb->ip_summed == CHECKSUM_UNNECESSARY) {
			vnet_hdr.flags = VIRTIO_NET_HDR_F_DATA_VALID;
		} /* else everything is zero */

		err = memcpy_toiovec(msg->msg_iov, (void *)&vnet_hdr,
				     vnet_hdr_len);
		if (err < 0)
=======
	packet_rcv_try_clear_pressure(pkt_sk(sk));

	if (vnet_hdr_len) {
		err = packet_rcv_vnet(msg, skb, &len, vnet_hdr_len);
		if (err)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto out_free;
	}

	/* You lose any data beyond the buffer you gave. If it worries
	 * a user program they can ask the device for its MTU
	 * anyway.
	 */
	copied = skb->len;
	if (copied > len) {
		copied = len;
		msg->msg_flags |= MSG_TRUNC;
	}

<<<<<<< HEAD
	err = skb_copy_datagram_iovec(skb, 0, msg->msg_iov, copied);
	if (err)
		goto out_free;

	sock_recv_ts_and_drops(msg, sk, skb);

	if (msg->msg_name) {
=======
	err = skb_copy_datagram_msg(skb, 0, msg, copied);
	if (err)
		goto out_free;

	if (sock->type != SOCK_PACKET) {
		struct sockaddr_ll *sll = &PACKET_SKB_CB(skb)->sa.ll;

		/* Original length was stored in sockaddr_ll fields */
		origlen = PACKET_SKB_CB(skb)->sa.origlen;
		sll->sll_family = AF_PACKET;
		sll->sll_protocol = skb->protocol;
	}

	sock_recv_cmsgs(msg, sk, skb);

	if (msg->msg_name) {
		const size_t max_len = min(sizeof(skb->cb),
					   sizeof(struct sockaddr_storage));
		int copy_len;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* If the address length field is there to be filled
		 * in, we fill it in now.
		 */
		if (sock->type == SOCK_PACKET) {
<<<<<<< HEAD
			msg->msg_namelen = sizeof(struct sockaddr_pkt);
		} else {
			struct sockaddr_ll *sll = &PACKET_SKB_CB(skb)->sa.ll;
			msg->msg_namelen = sll->sll_halen +
				offsetof(struct sockaddr_ll, sll_addr);
		}
		memcpy(msg->msg_name, &PACKET_SKB_CB(skb)->sa,
		       msg->msg_namelen);
	}

	if (pkt_sk(sk)->auxdata) {
=======
			__sockaddr_check_size(sizeof(struct sockaddr_pkt));
			msg->msg_namelen = sizeof(struct sockaddr_pkt);
			copy_len = msg->msg_namelen;
		} else {
			struct sockaddr_ll *sll = &PACKET_SKB_CB(skb)->sa.ll;

			msg->msg_namelen = sll->sll_halen +
				offsetof(struct sockaddr_ll, sll_addr);
			copy_len = msg->msg_namelen;
			if (msg->msg_namelen < sizeof(struct sockaddr_ll)) {
				memset(msg->msg_name +
				       offsetof(struct sockaddr_ll, sll_addr),
				       0, sizeof(sll->sll_addr));
				msg->msg_namelen = sizeof(struct sockaddr_ll);
			}
		}
		if (WARN_ON_ONCE(copy_len > max_len)) {
			copy_len = max_len;
			msg->msg_namelen = copy_len;
		}
		memcpy(msg->msg_name, &PACKET_SKB_CB(skb)->sa, copy_len);
	}

	if (packet_sock_flag(pkt_sk(sk), PACKET_SOCK_AUXDATA)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		struct tpacket_auxdata aux;

		aux.tp_status = TP_STATUS_USER;
		if (skb->ip_summed == CHECKSUM_PARTIAL)
			aux.tp_status |= TP_STATUS_CSUMNOTREADY;
<<<<<<< HEAD
		aux.tp_len = PACKET_SKB_CB(skb)->origlen;
		aux.tp_snaplen = skb->len;
		aux.tp_mac = 0;
		aux.tp_net = skb_network_offset(skb);
		if (vlan_tx_tag_present(skb)) {
			aux.tp_vlan_tci = vlan_tx_tag_get(skb);
			aux.tp_status |= TP_STATUS_VLAN_VALID;
		} else {
			aux.tp_vlan_tci = 0;
		}
		aux.tp_padding = 0;
=======
		else if (skb->pkt_type != PACKET_OUTGOING &&
			 skb_csum_unnecessary(skb))
			aux.tp_status |= TP_STATUS_CSUM_VALID;
		if (skb_is_gso(skb) && skb_is_gso_tcp(skb))
			aux.tp_status |= TP_STATUS_GSO_TCP;

		aux.tp_len = origlen;
		aux.tp_snaplen = skb->len;
		aux.tp_mac = 0;
		aux.tp_net = skb_network_offset(skb);
		if (skb_vlan_tag_present(skb)) {
			aux.tp_vlan_tci = skb_vlan_tag_get(skb);
			aux.tp_vlan_tpid = ntohs(skb->vlan_proto);
			aux.tp_status |= TP_STATUS_VLAN_VALID | TP_STATUS_VLAN_TPID_VALID;
		} else {
			aux.tp_vlan_tci = 0;
			aux.tp_vlan_tpid = 0;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		put_cmsg(msg, SOL_PACKET, PACKET_AUXDATA, sizeof(aux), &aux);
	}

	/*
	 *	Free or return the buffer as appropriate. Again this
	 *	hides all the races and re-entrancy issues from us.
	 */
	err = vnet_hdr_len + ((flags&MSG_TRUNC) ? skb->len : copied);

out_free:
	skb_free_datagram(sk, skb);
out:
	return err;
}

static int packet_getname_spkt(struct socket *sock, struct sockaddr *uaddr,
<<<<<<< HEAD
			       int *uaddr_len, int peer)
=======
			       int peer)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct net_device *dev;
	struct sock *sk	= sock->sk;

	if (peer)
		return -EOPNOTSUPP;

	uaddr->sa_family = AF_PACKET;
<<<<<<< HEAD
	memset(uaddr->sa_data, 0, sizeof(uaddr->sa_data));
	rcu_read_lock();
	dev = dev_get_by_index_rcu(sock_net(sk), pkt_sk(sk)->ifindex);
	if (dev)
		strlcpy(uaddr->sa_data, dev->name, sizeof(uaddr->sa_data));
	rcu_read_unlock();
	*uaddr_len = sizeof(*uaddr);

	return 0;
}

static int packet_getname(struct socket *sock, struct sockaddr *uaddr,
			  int *uaddr_len, int peer)
=======
	memset(uaddr->sa_data, 0, sizeof(uaddr->sa_data_min));
	rcu_read_lock();
	dev = dev_get_by_index_rcu(sock_net(sk), READ_ONCE(pkt_sk(sk)->ifindex));
	if (dev)
		strscpy(uaddr->sa_data, dev->name, sizeof(uaddr->sa_data_min));
	rcu_read_unlock();

	return sizeof(*uaddr);
}

static int packet_getname(struct socket *sock, struct sockaddr *uaddr,
			  int peer)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct net_device *dev;
	struct sock *sk = sock->sk;
	struct packet_sock *po = pkt_sk(sk);
	DECLARE_SOCKADDR(struct sockaddr_ll *, sll, uaddr);
<<<<<<< HEAD
=======
	int ifindex;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (peer)
		return -EOPNOTSUPP;

<<<<<<< HEAD
	sll->sll_family = AF_PACKET;
	sll->sll_ifindex = po->ifindex;
	sll->sll_protocol = po->num;
	sll->sll_pkttype = 0;
	rcu_read_lock();
	dev = dev_get_by_index_rcu(sock_net(sk), po->ifindex);
	if (dev) {
		sll->sll_hatype = dev->type;
		sll->sll_halen = dev->addr_len;
		memcpy(sll->sll_addr, dev->dev_addr, dev->addr_len);
=======
	ifindex = READ_ONCE(po->ifindex);
	sll->sll_family = AF_PACKET;
	sll->sll_ifindex = ifindex;
	sll->sll_protocol = READ_ONCE(po->num);
	sll->sll_pkttype = 0;
	rcu_read_lock();
	dev = dev_get_by_index_rcu(sock_net(sk), ifindex);
	if (dev) {
		sll->sll_hatype = dev->type;
		sll->sll_halen = dev->addr_len;

		/* Let __fortify_memcpy_chk() know the actual buffer size. */
		memcpy(((struct sockaddr_storage *)sll)->__data +
		       offsetof(struct sockaddr_ll, sll_addr) -
		       offsetofend(struct sockaddr_ll, sll_family),
		       dev->dev_addr, dev->addr_len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		sll->sll_hatype = 0;	/* Bad: we have no ARPHRD_UNSPEC */
		sll->sll_halen = 0;
	}
	rcu_read_unlock();
<<<<<<< HEAD
	*uaddr_len = offsetof(struct sockaddr_ll, sll_addr) + sll->sll_halen;

	return 0;
=======

	return offsetof(struct sockaddr_ll, sll_addr) + sll->sll_halen;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int packet_dev_mc(struct net_device *dev, struct packet_mclist *i,
			 int what)
{
	switch (i->type) {
	case PACKET_MR_MULTICAST:
		if (i->alen != dev->addr_len)
			return -EINVAL;
		if (what > 0)
			return dev_mc_add(dev, i->addr);
		else
			return dev_mc_del(dev, i->addr);
		break;
	case PACKET_MR_PROMISC:
		return dev_set_promiscuity(dev, what);
<<<<<<< HEAD
		break;
	case PACKET_MR_ALLMULTI:
		return dev_set_allmulti(dev, what);
		break;
=======
	case PACKET_MR_ALLMULTI:
		return dev_set_allmulti(dev, what);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case PACKET_MR_UNICAST:
		if (i->alen != dev->addr_len)
			return -EINVAL;
		if (what > 0)
			return dev_uc_add(dev, i->addr);
		else
			return dev_uc_del(dev, i->addr);
		break;
	default:
		break;
	}
	return 0;
}

<<<<<<< HEAD
static void packet_dev_mclist(struct net_device *dev, struct packet_mclist *i, int what)
{
	for ( ; i; i = i->next) {
		if (i->ifindex == dev->ifindex)
			packet_dev_mc(dev, i, what);
=======
static void packet_dev_mclist_delete(struct net_device *dev,
				     struct packet_mclist **mlp)
{
	struct packet_mclist *ml;

	while ((ml = *mlp) != NULL) {
		if (ml->ifindex == dev->ifindex) {
			packet_dev_mc(dev, ml, -1);
			*mlp = ml->next;
			kfree(ml);
		} else
			mlp = &ml->next;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

static int packet_mc_add(struct sock *sk, struct packet_mreq_max *mreq)
{
	struct packet_sock *po = pkt_sk(sk);
	struct packet_mclist *ml, *i;
	struct net_device *dev;
	int err;

	rtnl_lock();

	err = -ENODEV;
	dev = __dev_get_by_index(sock_net(sk), mreq->mr_ifindex);
	if (!dev)
		goto done;

	err = -EINVAL;
	if (mreq->mr_alen > dev->addr_len)
		goto done;

	err = -ENOBUFS;
	i = kmalloc(sizeof(*i), GFP_KERNEL);
	if (i == NULL)
		goto done;

	err = 0;
	for (ml = po->mclist; ml; ml = ml->next) {
		if (ml->ifindex == mreq->mr_ifindex &&
		    ml->type == mreq->mr_type &&
		    ml->alen == mreq->mr_alen &&
		    memcmp(ml->addr, mreq->mr_address, ml->alen) == 0) {
			ml->count++;
			/* Free the new element ... */
			kfree(i);
			goto done;
		}
	}

	i->type = mreq->mr_type;
	i->ifindex = mreq->mr_ifindex;
	i->alen = mreq->mr_alen;
	memcpy(i->addr, mreq->mr_address, i->alen);
<<<<<<< HEAD
=======
	memset(i->addr + i->alen, 0, sizeof(i->addr) - i->alen);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	i->count = 1;
	i->next = po->mclist;
	po->mclist = i;
	err = packet_dev_mc(dev, i, 1);
	if (err) {
		po->mclist = i->next;
		kfree(i);
	}

done:
	rtnl_unlock();
	return err;
}

static int packet_mc_drop(struct sock *sk, struct packet_mreq_max *mreq)
{
	struct packet_mclist *ml, **mlp;

	rtnl_lock();

	for (mlp = &pkt_sk(sk)->mclist; (ml = *mlp) != NULL; mlp = &ml->next) {
		if (ml->ifindex == mreq->mr_ifindex &&
		    ml->type == mreq->mr_type &&
		    ml->alen == mreq->mr_alen &&
		    memcmp(ml->addr, mreq->mr_address, ml->alen) == 0) {
			if (--ml->count == 0) {
				struct net_device *dev;
				*mlp = ml->next;
				dev = __dev_get_by_index(sock_net(sk), ml->ifindex);
				if (dev)
					packet_dev_mc(dev, ml, -1);
				kfree(ml);
			}
<<<<<<< HEAD
			rtnl_unlock();
			return 0;
		}
	}
	rtnl_unlock();
	return -EADDRNOTAVAIL;
=======
			break;
		}
	}
	rtnl_unlock();
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void packet_flush_mclist(struct sock *sk)
{
	struct packet_sock *po = pkt_sk(sk);
	struct packet_mclist *ml;

	if (!po->mclist)
		return;

	rtnl_lock();
	while ((ml = po->mclist) != NULL) {
		struct net_device *dev;

		po->mclist = ml->next;
		dev = __dev_get_by_index(sock_net(sk), ml->ifindex);
		if (dev != NULL)
			packet_dev_mc(dev, ml, -1);
		kfree(ml);
	}
	rtnl_unlock();
}

static int
<<<<<<< HEAD
packet_setsockopt(struct socket *sock, int level, int optname, char __user *optval, unsigned int optlen)
=======
packet_setsockopt(struct socket *sock, int level, int optname, sockptr_t optval,
		  unsigned int optlen)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct sock *sk = sock->sk;
	struct packet_sock *po = pkt_sk(sk);
	int ret;

	if (level != SOL_PACKET)
		return -ENOPROTOOPT;

	switch (optname) {
	case PACKET_ADD_MEMBERSHIP:
	case PACKET_DROP_MEMBERSHIP:
	{
		struct packet_mreq_max mreq;
		int len = optlen;
		memset(&mreq, 0, sizeof(mreq));
		if (len < sizeof(struct packet_mreq))
			return -EINVAL;
		if (len > sizeof(mreq))
			len = sizeof(mreq);
<<<<<<< HEAD
		if (copy_from_user(&mreq, optval, len))
=======
		if (copy_from_sockptr(&mreq, optval, len))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -EFAULT;
		if (len < (mreq.mr_alen + offsetof(struct packet_mreq, mr_address)))
			return -EINVAL;
		if (optname == PACKET_ADD_MEMBERSHIP)
			ret = packet_mc_add(sk, &mreq);
		else
			ret = packet_mc_drop(sk, &mreq);
		return ret;
	}

	case PACKET_RX_RING:
	case PACKET_TX_RING:
	{
		union tpacket_req_u req_u;
		int len;

<<<<<<< HEAD
=======
		lock_sock(sk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		switch (po->tp_version) {
		case TPACKET_V1:
		case TPACKET_V2:
			len = sizeof(req_u.req);
			break;
		case TPACKET_V3:
		default:
			len = sizeof(req_u.req3);
			break;
		}
<<<<<<< HEAD
		if (optlen < len)
			return -EINVAL;
		if (pkt_sk(sk)->has_vnet_hdr)
			return -EINVAL;
		if (copy_from_user(&req_u.req, optval, len))
			return -EFAULT;
		return packet_set_ring(sk, &req_u, 0,
			optname == PACKET_TX_RING);
=======
		if (optlen < len) {
			ret = -EINVAL;
		} else {
			if (copy_from_sockptr(&req_u.req, optval, len))
				ret = -EFAULT;
			else
				ret = packet_set_ring(sk, &req_u, 0,
						    optname == PACKET_TX_RING);
		}
		release_sock(sk);
		return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	case PACKET_COPY_THRESH:
	{
		int val;

		if (optlen != sizeof(val))
			return -EINVAL;
<<<<<<< HEAD
		if (copy_from_user(&val, optval, sizeof(val)))
			return -EFAULT;

		pkt_sk(sk)->copy_thresh = val;
=======
		if (copy_from_sockptr(&val, optval, sizeof(val)))
			return -EFAULT;

		WRITE_ONCE(pkt_sk(sk)->copy_thresh, val);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;
	}
	case PACKET_VERSION:
	{
		int val;

		if (optlen != sizeof(val))
			return -EINVAL;
<<<<<<< HEAD
		if (copy_from_user(&val, optval, sizeof(val)))
=======
		if (copy_from_sockptr(&val, optval, sizeof(val)))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -EFAULT;
		switch (val) {
		case TPACKET_V1:
		case TPACKET_V2:
		case TPACKET_V3:
			break;
		default:
			return -EINVAL;
		}
		lock_sock(sk);
		if (po->rx_ring.pg_vec || po->tx_ring.pg_vec) {
			ret = -EBUSY;
		} else {
			po->tp_version = val;
			ret = 0;
		}
		release_sock(sk);
		return ret;
	}
	case PACKET_RESERVE:
	{
		unsigned int val;

		if (optlen != sizeof(val))
			return -EINVAL;
<<<<<<< HEAD
		if (po->rx_ring.pg_vec || po->tx_ring.pg_vec)
			return -EBUSY;
		if (copy_from_user(&val, optval, sizeof(val)))
			return -EFAULT;
		if (val > INT_MAX)
			return -EINVAL;
		po->tp_reserve = val;
		return 0;
=======
		if (copy_from_sockptr(&val, optval, sizeof(val)))
			return -EFAULT;
		if (val > INT_MAX)
			return -EINVAL;
		lock_sock(sk);
		if (po->rx_ring.pg_vec || po->tx_ring.pg_vec) {
			ret = -EBUSY;
		} else {
			po->tp_reserve = val;
			ret = 0;
		}
		release_sock(sk);
		return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	case PACKET_LOSS:
	{
		unsigned int val;

		if (optlen != sizeof(val))
			return -EINVAL;
<<<<<<< HEAD
		if (po->rx_ring.pg_vec || po->tx_ring.pg_vec)
			return -EBUSY;
		if (copy_from_user(&val, optval, sizeof(val)))
			return -EFAULT;
		po->tp_loss = !!val;
		return 0;
=======
		if (copy_from_sockptr(&val, optval, sizeof(val)))
			return -EFAULT;

		lock_sock(sk);
		if (po->rx_ring.pg_vec || po->tx_ring.pg_vec) {
			ret = -EBUSY;
		} else {
			packet_sock_flag_set(po, PACKET_SOCK_TP_LOSS, val);
			ret = 0;
		}
		release_sock(sk);
		return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	case PACKET_AUXDATA:
	{
		int val;

		if (optlen < sizeof(val))
			return -EINVAL;
<<<<<<< HEAD
		if (copy_from_user(&val, optval, sizeof(val)))
			return -EFAULT;

		po->auxdata = !!val;
=======
		if (copy_from_sockptr(&val, optval, sizeof(val)))
			return -EFAULT;

		packet_sock_flag_set(po, PACKET_SOCK_AUXDATA, val);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;
	}
	case PACKET_ORIGDEV:
	{
		int val;

		if (optlen < sizeof(val))
			return -EINVAL;
<<<<<<< HEAD
		if (copy_from_user(&val, optval, sizeof(val)))
			return -EFAULT;

		po->origdev = !!val;
		return 0;
	}
	case PACKET_VNET_HDR:
	{
		int val;

		if (sock->type != SOCK_RAW)
			return -EINVAL;
		if (po->rx_ring.pg_vec || po->tx_ring.pg_vec)
			return -EBUSY;
		if (optlen < sizeof(val))
			return -EINVAL;
		if (copy_from_user(&val, optval, sizeof(val)))
			return -EFAULT;

		po->has_vnet_hdr = !!val;
		return 0;
=======
		if (copy_from_sockptr(&val, optval, sizeof(val)))
			return -EFAULT;

		packet_sock_flag_set(po, PACKET_SOCK_ORIGDEV, val);
		return 0;
	}
	case PACKET_VNET_HDR:
	case PACKET_VNET_HDR_SZ:
	{
		int val, hdr_len;

		if (sock->type != SOCK_RAW)
			return -EINVAL;
		if (optlen < sizeof(val))
			return -EINVAL;
		if (copy_from_sockptr(&val, optval, sizeof(val)))
			return -EFAULT;

		if (optname == PACKET_VNET_HDR_SZ) {
			if (val && val != sizeof(struct virtio_net_hdr) &&
			    val != sizeof(struct virtio_net_hdr_mrg_rxbuf))
				return -EINVAL;
			hdr_len = val;
		} else {
			hdr_len = val ? sizeof(struct virtio_net_hdr) : 0;
		}
		lock_sock(sk);
		if (po->rx_ring.pg_vec || po->tx_ring.pg_vec) {
			ret = -EBUSY;
		} else {
			WRITE_ONCE(po->vnet_hdr_sz, hdr_len);
			ret = 0;
		}
		release_sock(sk);
		return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	case PACKET_TIMESTAMP:
	{
		int val;

		if (optlen != sizeof(val))
			return -EINVAL;
<<<<<<< HEAD
		if (copy_from_user(&val, optval, sizeof(val)))
			return -EFAULT;

		po->tp_tstamp = val;
=======
		if (copy_from_sockptr(&val, optval, sizeof(val)))
			return -EFAULT;

		WRITE_ONCE(po->tp_tstamp, val);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;
	}
	case PACKET_FANOUT:
	{
<<<<<<< HEAD
=======
		struct fanout_args args = { 0 };

		if (optlen != sizeof(int) && optlen != sizeof(args))
			return -EINVAL;
		if (copy_from_sockptr(&args, optval, optlen))
			return -EFAULT;

		return fanout_add(sk, &args);
	}
	case PACKET_FANOUT_DATA:
	{
		/* Paired with the WRITE_ONCE() in fanout_add() */
		if (!READ_ONCE(po->fanout))
			return -EINVAL;

		return fanout_set_data(po, optval, optlen);
	}
	case PACKET_IGNORE_OUTGOING:
	{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		int val;

		if (optlen != sizeof(val))
			return -EINVAL;
<<<<<<< HEAD
		if (copy_from_user(&val, optval, sizeof(val)))
			return -EFAULT;

		return fanout_add(sk, val & 0xffff, val >> 16);
=======
		if (copy_from_sockptr(&val, optval, sizeof(val)))
			return -EFAULT;
		if (val < 0 || val > 1)
			return -EINVAL;

		WRITE_ONCE(po->prot_hook.ignore_outgoing, !!val);
		return 0;
	}
	case PACKET_TX_HAS_OFF:
	{
		unsigned int val;

		if (optlen != sizeof(val))
			return -EINVAL;
		if (copy_from_sockptr(&val, optval, sizeof(val)))
			return -EFAULT;

		lock_sock(sk);
		if (!po->rx_ring.pg_vec && !po->tx_ring.pg_vec)
			packet_sock_flag_set(po, PACKET_SOCK_TX_HAS_OFF, val);

		release_sock(sk);
		return 0;
	}
	case PACKET_QDISC_BYPASS:
	{
		int val;

		if (optlen != sizeof(val))
			return -EINVAL;
		if (copy_from_sockptr(&val, optval, sizeof(val)))
			return -EFAULT;

		packet_sock_flag_set(po, PACKET_SOCK_QDISC_BYPASS, val);
		return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	default:
		return -ENOPROTOOPT;
	}
}

static int packet_getsockopt(struct socket *sock, int level, int optname,
			     char __user *optval, int __user *optlen)
{
	int len;
<<<<<<< HEAD
	int val;
	struct sock *sk = sock->sk;
	struct packet_sock *po = pkt_sk(sk);
	void *data;
	struct tpacket_stats st;
	union tpacket_stats_u st_u;
=======
	int val, lv = sizeof(val);
	struct sock *sk = sock->sk;
	struct packet_sock *po = pkt_sk(sk);
	void *data = &val;
	union tpacket_stats_u st;
	struct tpacket_rollover_stats rstats;
	int drops;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (level != SOL_PACKET)
		return -ENOPROTOOPT;

	if (get_user(len, optlen))
		return -EFAULT;

	if (len < 0)
		return -EINVAL;

	switch (optname) {
	case PACKET_STATISTICS:
<<<<<<< HEAD
		if (po->tp_version == TPACKET_V3) {
			len = sizeof(struct tpacket_stats_v3);
		} else {
			if (len > sizeof(struct tpacket_stats))
				len = sizeof(struct tpacket_stats);
		}
		spin_lock_bh(&sk->sk_receive_queue.lock);
		if (po->tp_version == TPACKET_V3) {
			memcpy(&st_u.stats3, &po->stats,
			sizeof(struct tpacket_stats));
			st_u.stats3.tp_freeze_q_cnt =
			po->stats_u.stats3.tp_freeze_q_cnt;
			st_u.stats3.tp_packets += po->stats.tp_drops;
			data = &st_u.stats3;
		} else {
			st = po->stats;
			st.tp_packets += st.tp_drops;
			data = &st;
		}
		memset(&po->stats, 0, sizeof(st));
		spin_unlock_bh(&sk->sk_receive_queue.lock);
		break;
	case PACKET_AUXDATA:
		if (len > sizeof(int))
			len = sizeof(int);
		val = po->auxdata;

		data = &val;
		break;
	case PACKET_ORIGDEV:
		if (len > sizeof(int))
			len = sizeof(int);
		val = po->origdev;

		data = &val;
		break;
	case PACKET_VNET_HDR:
		if (len > sizeof(int))
			len = sizeof(int);
		val = po->has_vnet_hdr;

		data = &val;
		break;
	case PACKET_VERSION:
		if (len > sizeof(int))
			len = sizeof(int);
		val = po->tp_version;
		data = &val;
=======
		spin_lock_bh(&sk->sk_receive_queue.lock);
		memcpy(&st, &po->stats, sizeof(st));
		memset(&po->stats, 0, sizeof(po->stats));
		spin_unlock_bh(&sk->sk_receive_queue.lock);
		drops = atomic_xchg(&po->tp_drops, 0);

		if (po->tp_version == TPACKET_V3) {
			lv = sizeof(struct tpacket_stats_v3);
			st.stats3.tp_drops = drops;
			st.stats3.tp_packets += drops;
			data = &st.stats3;
		} else {
			lv = sizeof(struct tpacket_stats);
			st.stats1.tp_drops = drops;
			st.stats1.tp_packets += drops;
			data = &st.stats1;
		}

		break;
	case PACKET_AUXDATA:
		val = packet_sock_flag(po, PACKET_SOCK_AUXDATA);
		break;
	case PACKET_ORIGDEV:
		val = packet_sock_flag(po, PACKET_SOCK_ORIGDEV);
		break;
	case PACKET_VNET_HDR:
		val = !!READ_ONCE(po->vnet_hdr_sz);
		break;
	case PACKET_VNET_HDR_SZ:
		val = READ_ONCE(po->vnet_hdr_sz);
		break;
	case PACKET_COPY_THRESH:
		val = READ_ONCE(pkt_sk(sk)->copy_thresh);
		break;
	case PACKET_VERSION:
		val = po->tp_version;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case PACKET_HDRLEN:
		if (len > sizeof(int))
			len = sizeof(int);
<<<<<<< HEAD
=======
		if (len < sizeof(int))
			return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (copy_from_user(&val, optval, len))
			return -EFAULT;
		switch (val) {
		case TPACKET_V1:
			val = sizeof(struct tpacket_hdr);
			break;
		case TPACKET_V2:
			val = sizeof(struct tpacket2_hdr);
			break;
		case TPACKET_V3:
			val = sizeof(struct tpacket3_hdr);
			break;
		default:
			return -EINVAL;
		}
<<<<<<< HEAD
		data = &val;
		break;
	case PACKET_RESERVE:
		if (len > sizeof(unsigned int))
			len = sizeof(unsigned int);
		val = po->tp_reserve;
		data = &val;
		break;
	case PACKET_LOSS:
		if (len > sizeof(unsigned int))
			len = sizeof(unsigned int);
		val = po->tp_loss;
		data = &val;
		break;
	case PACKET_TIMESTAMP:
		if (len > sizeof(int))
			len = sizeof(int);
		val = po->tp_tstamp;
		data = &val;
		break;
	case PACKET_FANOUT:
		if (len > sizeof(int))
			len = sizeof(int);
		val = (po->fanout ?
		       ((u32)po->fanout->id |
			((u32)po->fanout->type << 16)) :
		       0);
		data = &val;
=======
		break;
	case PACKET_RESERVE:
		val = po->tp_reserve;
		break;
	case PACKET_LOSS:
		val = packet_sock_flag(po, PACKET_SOCK_TP_LOSS);
		break;
	case PACKET_TIMESTAMP:
		val = READ_ONCE(po->tp_tstamp);
		break;
	case PACKET_FANOUT:
		val = (po->fanout ?
		       ((u32)po->fanout->id |
			((u32)po->fanout->type << 16) |
			((u32)po->fanout->flags << 24)) :
		       0);
		break;
	case PACKET_IGNORE_OUTGOING:
		val = READ_ONCE(po->prot_hook.ignore_outgoing);
		break;
	case PACKET_ROLLOVER_STATS:
		if (!po->rollover)
			return -EINVAL;
		rstats.tp_all = atomic_long_read(&po->rollover->num);
		rstats.tp_huge = atomic_long_read(&po->rollover->num_huge);
		rstats.tp_failed = atomic_long_read(&po->rollover->num_failed);
		data = &rstats;
		lv = sizeof(rstats);
		break;
	case PACKET_TX_HAS_OFF:
		val = packet_sock_flag(po, PACKET_SOCK_TX_HAS_OFF);
		break;
	case PACKET_QDISC_BYPASS:
		val = packet_sock_flag(po, PACKET_SOCK_QDISC_BYPASS);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	default:
		return -ENOPROTOOPT;
	}

<<<<<<< HEAD
=======
	if (len > lv)
		len = lv;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (put_user(len, optlen))
		return -EFAULT;
	if (copy_to_user(optval, data, len))
		return -EFAULT;
	return 0;
}

<<<<<<< HEAD

static int packet_notifier(struct notifier_block *this, unsigned long msg, void *data)
{
	struct sock *sk;
	struct hlist_node *node;
	struct net_device *dev = data;
	struct net *net = dev_net(dev);

	rcu_read_lock();
	sk_for_each_rcu(sk, node, &net->packet.sklist) {
=======
static int packet_notifier(struct notifier_block *this,
			   unsigned long msg, void *ptr)
{
	struct sock *sk;
	struct net_device *dev = netdev_notifier_info_to_dev(ptr);
	struct net *net = dev_net(dev);

	rcu_read_lock();
	sk_for_each_rcu(sk, &net->packet.sklist) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		struct packet_sock *po = pkt_sk(sk);

		switch (msg) {
		case NETDEV_UNREGISTER:
			if (po->mclist)
<<<<<<< HEAD
				packet_dev_mclist(dev, po->mclist, -1);
			/* fallthrough */
=======
				packet_dev_mclist_delete(dev, &po->mclist);
			fallthrough;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		case NETDEV_DOWN:
			if (dev->ifindex == po->ifindex) {
				spin_lock(&po->bind_lock);
<<<<<<< HEAD
				if (po->running) {
					__unregister_prot_hook(sk, false);
					sk->sk_err = ENETDOWN;
					if (!sock_flag(sk, SOCK_DEAD))
						sk->sk_error_report(sk);
				}
				if (msg == NETDEV_UNREGISTER) {
					po->ifindex = -1;
					if (po->prot_hook.dev)
						dev_put(po->prot_hook.dev);
=======
				if (packet_sock_flag(po, PACKET_SOCK_RUNNING)) {
					__unregister_prot_hook(sk, false);
					sk->sk_err = ENETDOWN;
					if (!sock_flag(sk, SOCK_DEAD))
						sk_error_report(sk);
				}
				if (msg == NETDEV_UNREGISTER) {
					packet_cached_dev_reset(po);
					WRITE_ONCE(po->ifindex, -1);
					netdev_put(po->prot_hook.dev,
						   &po->prot_hook.dev_tracker);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					po->prot_hook.dev = NULL;
				}
				spin_unlock(&po->bind_lock);
			}
			break;
		case NETDEV_UP:
			if (dev->ifindex == po->ifindex) {
				spin_lock(&po->bind_lock);
				if (po->num)
					register_prot_hook(sk);
				spin_unlock(&po->bind_lock);
			}
			break;
		}
	}
	rcu_read_unlock();
	return NOTIFY_DONE;
}


static int packet_ioctl(struct socket *sock, unsigned int cmd,
			unsigned long arg)
{
	struct sock *sk = sock->sk;

	switch (cmd) {
	case SIOCOUTQ:
	{
		int amount = sk_wmem_alloc_get(sk);

		return put_user(amount, (int __user *)arg);
	}
	case SIOCINQ:
	{
		struct sk_buff *skb;
		int amount = 0;

		spin_lock_bh(&sk->sk_receive_queue.lock);
		skb = skb_peek(&sk->sk_receive_queue);
		if (skb)
			amount = skb->len;
		spin_unlock_bh(&sk->sk_receive_queue.lock);
		return put_user(amount, (int __user *)arg);
	}
<<<<<<< HEAD
	case SIOCGSTAMP:
		return sock_get_timestamp(sk, (struct timeval __user *)arg);
	case SIOCGSTAMPNS:
		return sock_get_timestampns(sk, (struct timespec __user *)arg);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_INET
	case SIOCADDRT:
	case SIOCDELRT:
	case SIOCDARP:
	case SIOCGARP:
	case SIOCSARP:
	case SIOCGIFADDR:
	case SIOCSIFADDR:
	case SIOCGIFBRDADDR:
	case SIOCSIFBRDADDR:
	case SIOCGIFNETMASK:
	case SIOCSIFNETMASK:
	case SIOCGIFDSTADDR:
	case SIOCSIFDSTADDR:
	case SIOCSIFFLAGS:
		return inet_dgram_ops.ioctl(sock, cmd, arg);
#endif

	default:
		return -ENOIOCTLCMD;
	}
	return 0;
}

<<<<<<< HEAD
static unsigned int packet_poll(struct file *file, struct socket *sock,
=======
static __poll_t packet_poll(struct file *file, struct socket *sock,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				poll_table *wait)
{
	struct sock *sk = sock->sk;
	struct packet_sock *po = pkt_sk(sk);
<<<<<<< HEAD
	unsigned int mask = datagram_poll(file, sock, wait);
=======
	__poll_t mask = datagram_poll(file, sock, wait);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spin_lock_bh(&sk->sk_receive_queue.lock);
	if (po->rx_ring.pg_vec) {
		if (!packet_previous_rx_frame(po, &po->rx_ring,
			TP_STATUS_KERNEL))
<<<<<<< HEAD
			mask |= POLLIN | POLLRDNORM;
	}
=======
			mask |= EPOLLIN | EPOLLRDNORM;
	}
	packet_rcv_try_clear_pressure(po);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_unlock_bh(&sk->sk_receive_queue.lock);
	spin_lock_bh(&sk->sk_write_queue.lock);
	if (po->tx_ring.pg_vec) {
		if (packet_current_frame(po, &po->tx_ring, TP_STATUS_AVAILABLE))
<<<<<<< HEAD
			mask |= POLLOUT | POLLWRNORM;
=======
			mask |= EPOLLOUT | EPOLLWRNORM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	spin_unlock_bh(&sk->sk_write_queue.lock);
	return mask;
}


/* Dirty? Well, I still did not learn better way to account
 * for user mmaps.
 */

static void packet_mm_open(struct vm_area_struct *vma)
{
	struct file *file = vma->vm_file;
	struct socket *sock = file->private_data;
	struct sock *sk = sock->sk;

	if (sk)
<<<<<<< HEAD
		atomic_inc(&pkt_sk(sk)->mapped);
=======
		atomic_long_inc(&pkt_sk(sk)->mapped);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void packet_mm_close(struct vm_area_struct *vma)
{
	struct file *file = vma->vm_file;
	struct socket *sock = file->private_data;
	struct sock *sk = sock->sk;

	if (sk)
<<<<<<< HEAD
		atomic_dec(&pkt_sk(sk)->mapped);
=======
		atomic_long_dec(&pkt_sk(sk)->mapped);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const struct vm_operations_struct packet_mmap_ops = {
	.open	=	packet_mm_open,
	.close	=	packet_mm_close,
};

static void free_pg_vec(struct pgv *pg_vec, unsigned int order,
			unsigned int len)
{
	int i;

	for (i = 0; i < len; i++) {
		if (likely(pg_vec[i].buffer)) {
			if (is_vmalloc_addr(pg_vec[i].buffer))
				vfree(pg_vec[i].buffer);
			else
				free_pages((unsigned long)pg_vec[i].buffer,
					   order);
			pg_vec[i].buffer = NULL;
		}
	}
	kfree(pg_vec);
}

static char *alloc_one_pg_vec_page(unsigned long order)
{
<<<<<<< HEAD
	char *buffer = NULL;
=======
	char *buffer;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	gfp_t gfp_flags = GFP_KERNEL | __GFP_COMP |
			  __GFP_ZERO | __GFP_NOWARN | __GFP_NORETRY;

	buffer = (char *) __get_free_pages(gfp_flags, order);
<<<<<<< HEAD

	if (buffer)
		return buffer;

	/*
	 * __get_free_pages failed, fall back to vmalloc
	 */
	buffer = vzalloc((1 << order) * PAGE_SIZE);

	if (buffer)
		return buffer;

	/*
	 * vmalloc failed, lets dig into swap here
	 */
	gfp_flags &= ~__GFP_NORETRY;
	buffer = (char *)__get_free_pages(gfp_flags, order);
	if (buffer)
		return buffer;

	/*
	 * complete and utter failure
	 */
=======
	if (buffer)
		return buffer;

	/* __get_free_pages failed, fall back to vmalloc */
	buffer = vzalloc(array_size((1 << order), PAGE_SIZE));
	if (buffer)
		return buffer;

	/* vmalloc failed, lets dig into swap here */
	gfp_flags &= ~__GFP_NORETRY;
	buffer = (char *) __get_free_pages(gfp_flags, order);
	if (buffer)
		return buffer;

	/* complete and utter failure */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return NULL;
}

static struct pgv *alloc_pg_vec(struct tpacket_req *req, int order)
{
	unsigned int block_nr = req->tp_block_nr;
	struct pgv *pg_vec;
	int i;

<<<<<<< HEAD
	pg_vec = kcalloc(block_nr, sizeof(struct pgv), GFP_KERNEL);
=======
	pg_vec = kcalloc(block_nr, sizeof(struct pgv), GFP_KERNEL | __GFP_NOWARN);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (unlikely(!pg_vec))
		goto out;

	for (i = 0; i < block_nr; i++) {
		pg_vec[i].buffer = alloc_one_pg_vec_page(order);
		if (unlikely(!pg_vec[i].buffer))
			goto out_free_pgvec;
	}

out:
	return pg_vec;

out_free_pgvec:
	free_pg_vec(pg_vec, order, block_nr);
	pg_vec = NULL;
	goto out;
}

static int packet_set_ring(struct sock *sk, union tpacket_req_u *req_u,
		int closing, int tx_ring)
{
	struct pgv *pg_vec = NULL;
	struct packet_sock *po = pkt_sk(sk);
<<<<<<< HEAD
=======
	unsigned long *rx_owner_map = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int was_running, order = 0;
	struct packet_ring_buffer *rb;
	struct sk_buff_head *rb_queue;
	__be16 num;
<<<<<<< HEAD
	int err = -EINVAL;
	/* Added to avoid minimal code churn */
	struct tpacket_req *req = &req_u->req;

	lock_sock(sk);
	/* Opening a Tx-ring is NOT supported in TPACKET_V3 */
	if (!closing && tx_ring && (po->tp_version > TPACKET_V2)) {
		WARN(1, "Tx-ring is not supported.\n");
		goto out;
	}

=======
	int err;
	/* Added to avoid minimal code churn */
	struct tpacket_req *req = &req_u->req;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rb = tx_ring ? &po->tx_ring : &po->rx_ring;
	rb_queue = tx_ring ? &sk->sk_write_queue : &sk->sk_receive_queue;

	err = -EBUSY;
	if (!closing) {
<<<<<<< HEAD
		if (atomic_read(&po->mapped))
			goto out;
		if (atomic_read(&rb->pending))
=======
		if (atomic_long_read(&po->mapped))
			goto out;
		if (packet_read_pending(rb))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto out;
	}

	if (req->tp_block_nr) {
<<<<<<< HEAD
=======
		unsigned int min_frame_size;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* Sanity tests and some calculations */
		err = -EBUSY;
		if (unlikely(rb->pg_vec))
			goto out;

		switch (po->tp_version) {
		case TPACKET_V1:
			po->tp_hdrlen = TPACKET_HDRLEN;
			break;
		case TPACKET_V2:
			po->tp_hdrlen = TPACKET2_HDRLEN;
			break;
		case TPACKET_V3:
			po->tp_hdrlen = TPACKET3_HDRLEN;
			break;
		}

		err = -EINVAL;
		if (unlikely((int)req->tp_block_size <= 0))
			goto out;
<<<<<<< HEAD
		if (unlikely(req->tp_block_size & (PAGE_SIZE - 1)))
			goto out;
		if (unlikely(req->tp_frame_size < po->tp_hdrlen +
					po->tp_reserve))
=======
		if (unlikely(!PAGE_ALIGNED(req->tp_block_size)))
			goto out;
		min_frame_size = po->tp_hdrlen + po->tp_reserve;
		if (po->tp_version >= TPACKET_V3 &&
		    req->tp_block_size <
		    BLK_PLUS_PRIV((u64)req_u->req3.tp_sizeof_priv) + min_frame_size)
			goto out;
		if (unlikely(req->tp_frame_size < min_frame_size))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto out;
		if (unlikely(req->tp_frame_size & (TPACKET_ALIGNMENT - 1)))
			goto out;

<<<<<<< HEAD
		rb->frames_per_block = req->tp_block_size/req->tp_frame_size;
		if (unlikely(rb->frames_per_block <= 0))
=======
		rb->frames_per_block = req->tp_block_size / req->tp_frame_size;
		if (unlikely(rb->frames_per_block == 0))
			goto out;
		if (unlikely(rb->frames_per_block > UINT_MAX / req->tp_block_nr))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto out;
		if (unlikely((rb->frames_per_block * req->tp_block_nr) !=
					req->tp_frame_nr))
			goto out;

		err = -ENOMEM;
		order = get_order(req->tp_block_size);
		pg_vec = alloc_pg_vec(req, order);
		if (unlikely(!pg_vec))
			goto out;
		switch (po->tp_version) {
		case TPACKET_V3:
<<<<<<< HEAD
		/* Transmit path is not supported. We checked
		 * it above but just being paranoid
		 */
			if (!tx_ring)
				init_prb_bdqc(po, rb, pg_vec, req_u, tx_ring);
				break;
		default:
=======
			/* Block transmit is not supported yet */
			if (!tx_ring) {
				init_prb_bdqc(po, rb, pg_vec, req_u);
			} else {
				struct tpacket_req3 *req3 = &req_u->req3;

				if (req3->tp_retire_blk_tov ||
				    req3->tp_sizeof_priv ||
				    req3->tp_feature_req_word) {
					err = -EINVAL;
					goto out_free_pg_vec;
				}
			}
			break;
		default:
			if (!tx_ring) {
				rx_owner_map = bitmap_alloc(req->tp_frame_nr,
					GFP_KERNEL | __GFP_NOWARN | __GFP_ZERO);
				if (!rx_owner_map)
					goto out_free_pg_vec;
			}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		}
	}
	/* Done */
	else {
		err = -EINVAL;
		if (unlikely(req->tp_frame_nr))
			goto out;
	}


	/* Detach socket from network */
	spin_lock(&po->bind_lock);
<<<<<<< HEAD
	was_running = po->running;
	num = po->num;
	if (was_running) {
		po->num = 0;
=======
	was_running = packet_sock_flag(po, PACKET_SOCK_RUNNING);
	num = po->num;
	if (was_running) {
		WRITE_ONCE(po->num, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		__unregister_prot_hook(sk, false);
	}
	spin_unlock(&po->bind_lock);

	synchronize_net();

	err = -EBUSY;
	mutex_lock(&po->pg_vec_lock);
<<<<<<< HEAD
	if (closing || atomic_read(&po->mapped) == 0) {
		err = 0;
		spin_lock_bh(&rb_queue->lock);
		swap(rb->pg_vec, pg_vec);
=======
	if (closing || atomic_long_read(&po->mapped) == 0) {
		err = 0;
		spin_lock_bh(&rb_queue->lock);
		swap(rb->pg_vec, pg_vec);
		if (po->tp_version <= TPACKET_V2)
			swap(rb->rx_owner_map, rx_owner_map);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		rb->frame_max = (req->tp_frame_nr - 1);
		rb->head = 0;
		rb->frame_size = req->tp_frame_size;
		spin_unlock_bh(&rb_queue->lock);

		swap(rb->pg_vec_order, order);
		swap(rb->pg_vec_len, req->tp_block_nr);

		rb->pg_vec_pages = req->tp_block_size/PAGE_SIZE;
		po->prot_hook.func = (po->rx_ring.pg_vec) ?
						tpacket_rcv : packet_rcv;
		skb_queue_purge(rb_queue);
<<<<<<< HEAD
		if (atomic_read(&po->mapped))
			pr_err("packet_mmap: vma is busy: %d\n",
			       atomic_read(&po->mapped));
=======
		if (atomic_long_read(&po->mapped))
			pr_err("packet_mmap: vma is busy: %ld\n",
			       atomic_long_read(&po->mapped));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	mutex_unlock(&po->pg_vec_lock);

	spin_lock(&po->bind_lock);
	if (was_running) {
<<<<<<< HEAD
		po->num = num;
		register_prot_hook(sk);
	}
	spin_unlock(&po->bind_lock);
	if (closing && (po->tp_version > TPACKET_V2)) {
		/* Because we don't support block-based V3 on tx-ring */
		if (!tx_ring)
			prb_shutdown_retire_blk_timer(po, tx_ring, rb_queue);
	}

	if (pg_vec)
		free_pg_vec(pg_vec, order, req->tp_block_nr);
out:
	release_sock(sk);
=======
		WRITE_ONCE(po->num, num);
		register_prot_hook(sk);
	}
	spin_unlock(&po->bind_lock);
	if (pg_vec && (po->tp_version > TPACKET_V2)) {
		/* Because we don't support block-based V3 on tx-ring */
		if (!tx_ring)
			prb_shutdown_retire_blk_timer(po, rb_queue);
	}

out_free_pg_vec:
	if (pg_vec) {
		bitmap_free(rx_owner_map);
		free_pg_vec(pg_vec, order, req->tp_block_nr);
	}
out:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

static int packet_mmap(struct file *file, struct socket *sock,
		struct vm_area_struct *vma)
{
	struct sock *sk = sock->sk;
	struct packet_sock *po = pkt_sk(sk);
	unsigned long size, expected_size;
	struct packet_ring_buffer *rb;
	unsigned long start;
	int err = -EINVAL;
	int i;

	if (vma->vm_pgoff)
		return -EINVAL;

	mutex_lock(&po->pg_vec_lock);

	expected_size = 0;
	for (rb = &po->rx_ring; rb <= &po->tx_ring; rb++) {
		if (rb->pg_vec) {
			expected_size += rb->pg_vec_len
						* rb->pg_vec_pages
						* PAGE_SIZE;
		}
	}

	if (expected_size == 0)
		goto out;

	size = vma->vm_end - vma->vm_start;
	if (size != expected_size)
		goto out;

	start = vma->vm_start;
	for (rb = &po->rx_ring; rb <= &po->tx_ring; rb++) {
		if (rb->pg_vec == NULL)
			continue;

		for (i = 0; i < rb->pg_vec_len; i++) {
			struct page *page;
			void *kaddr = rb->pg_vec[i].buffer;
			int pg_num;

			for (pg_num = 0; pg_num < rb->pg_vec_pages; pg_num++) {
				page = pgv_to_page(kaddr);
				err = vm_insert_page(vma, start, page);
				if (unlikely(err))
					goto out;
				start += PAGE_SIZE;
				kaddr += PAGE_SIZE;
			}
		}
	}

<<<<<<< HEAD
	atomic_inc(&po->mapped);
=======
	atomic_long_inc(&po->mapped);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	vma->vm_ops = &packet_mmap_ops;
	err = 0;

out:
	mutex_unlock(&po->pg_vec_lock);
	return err;
}

static const struct proto_ops packet_ops_spkt = {
	.family =	PF_PACKET,
	.owner =	THIS_MODULE,
	.release =	packet_release,
	.bind =		packet_bind_spkt,
	.connect =	sock_no_connect,
	.socketpair =	sock_no_socketpair,
	.accept =	sock_no_accept,
	.getname =	packet_getname_spkt,
	.poll =		datagram_poll,
	.ioctl =	packet_ioctl,
<<<<<<< HEAD
	.listen =	sock_no_listen,
	.shutdown =	sock_no_shutdown,
	.setsockopt =	sock_no_setsockopt,
	.getsockopt =	sock_no_getsockopt,
	.sendmsg =	packet_sendmsg_spkt,
	.recvmsg =	packet_recvmsg,
	.mmap =		sock_no_mmap,
	.sendpage =	sock_no_sendpage,
=======
	.gettstamp =	sock_gettstamp,
	.listen =	sock_no_listen,
	.shutdown =	sock_no_shutdown,
	.sendmsg =	packet_sendmsg_spkt,
	.recvmsg =	packet_recvmsg,
	.mmap =		sock_no_mmap,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static const struct proto_ops packet_ops = {
	.family =	PF_PACKET,
	.owner =	THIS_MODULE,
	.release =	packet_release,
	.bind =		packet_bind,
	.connect =	sock_no_connect,
	.socketpair =	sock_no_socketpair,
	.accept =	sock_no_accept,
	.getname =	packet_getname,
	.poll =		packet_poll,
	.ioctl =	packet_ioctl,
<<<<<<< HEAD
=======
	.gettstamp =	sock_gettstamp,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.listen =	sock_no_listen,
	.shutdown =	sock_no_shutdown,
	.setsockopt =	packet_setsockopt,
	.getsockopt =	packet_getsockopt,
	.sendmsg =	packet_sendmsg,
	.recvmsg =	packet_recvmsg,
	.mmap =		packet_mmap,
<<<<<<< HEAD
	.sendpage =	sock_no_sendpage,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static const struct net_proto_family packet_family_ops = {
	.family =	PF_PACKET,
	.create =	packet_create,
	.owner	=	THIS_MODULE,
};

static struct notifier_block packet_netdev_notifier = {
	.notifier_call =	packet_notifier,
};

#ifdef CONFIG_PROC_FS

static void *packet_seq_start(struct seq_file *seq, loff_t *pos)
	__acquires(RCU)
{
	struct net *net = seq_file_net(seq);

	rcu_read_lock();
	return seq_hlist_start_head_rcu(&net->packet.sklist, *pos);
}

static void *packet_seq_next(struct seq_file *seq, void *v, loff_t *pos)
{
	struct net *net = seq_file_net(seq);
	return seq_hlist_next_rcu(v, &net->packet.sklist, pos);
}

static void packet_seq_stop(struct seq_file *seq, void *v)
	__releases(RCU)
{
	rcu_read_unlock();
}

static int packet_seq_show(struct seq_file *seq, void *v)
{
	if (v == SEQ_START_TOKEN)
<<<<<<< HEAD
		seq_puts(seq, "sk       RefCnt Type Proto  Iface R Rmem   User   Inode\n");
=======
		seq_printf(seq,
			   "%*sRefCnt Type Proto  Iface R Rmem   User   Inode\n",
			   IS_ENABLED(CONFIG_64BIT) ? -17 : -9, "sk");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	else {
		struct sock *s = sk_entry(v);
		const struct packet_sock *po = pkt_sk(s);

		seq_printf(seq,
			   "%pK %-6d %-4d %04x   %-5d %1d %-6u %-6u %-6lu\n",
			   s,
<<<<<<< HEAD
			   atomic_read(&s->sk_refcnt),
			   s->sk_type,
			   ntohs(po->num),
			   po->ifindex,
			   po->running,
			   atomic_read(&s->sk_rmem_alloc),
			   sock_i_uid(s),
=======
			   refcount_read(&s->sk_refcnt),
			   s->sk_type,
			   ntohs(READ_ONCE(po->num)),
			   READ_ONCE(po->ifindex),
			   packet_sock_flag(po, PACKET_SOCK_RUNNING),
			   atomic_read(&s->sk_rmem_alloc),
			   from_kuid_munged(seq_user_ns(seq), sock_i_uid(s)),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			   sock_i_ino(s));
	}

	return 0;
}

static const struct seq_operations packet_seq_ops = {
	.start	= packet_seq_start,
	.next	= packet_seq_next,
	.stop	= packet_seq_stop,
	.show	= packet_seq_show,
};
<<<<<<< HEAD

static int packet_seq_open(struct inode *inode, struct file *file)
{
	return seq_open_net(inode, file, &packet_seq_ops,
			    sizeof(struct seq_net_private));
}

static const struct file_operations packet_seq_fops = {
	.owner		= THIS_MODULE,
	.open		= packet_seq_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release_net,
};

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

static int __net_init packet_net_init(struct net *net)
{
<<<<<<< HEAD
	spin_lock_init(&net->packet.sklist_lock);
	INIT_HLIST_HEAD(&net->packet.sklist);

	if (!proc_net_fops_create(net, "packet", 0, &packet_seq_fops))
		return -ENOMEM;
=======
	mutex_init(&net->packet.sklist_lock);
	INIT_HLIST_HEAD(&net->packet.sklist);

#ifdef CONFIG_PROC_FS
	if (!proc_create_net("packet", 0, net->proc_net, &packet_seq_ops,
			sizeof(struct seq_net_private)))
		return -ENOMEM;
#endif /* CONFIG_PROC_FS */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static void __net_exit packet_net_exit(struct net *net)
{
<<<<<<< HEAD
	proc_net_remove(net, "packet");
=======
	remove_proc_entry("packet", net->proc_net);
	WARN_ON_ONCE(!hlist_empty(&net->packet.sklist));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct pernet_operations packet_net_ops = {
	.init = packet_net_init,
	.exit = packet_net_exit,
};


static void __exit packet_exit(void)
{
<<<<<<< HEAD
	unregister_netdevice_notifier(&packet_netdev_notifier);
	unregister_pernet_subsys(&packet_net_ops);
	sock_unregister(PF_PACKET);
	proto_unregister(&packet_proto);
=======
	sock_unregister(PF_PACKET);
	proto_unregister(&packet_proto);
	unregister_netdevice_notifier(&packet_netdev_notifier);
	unregister_pernet_subsys(&packet_net_ops);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int __init packet_init(void)
{
<<<<<<< HEAD
	int rc = proto_register(&packet_proto, 0);

	if (rc != 0)
		goto out;

	sock_register(&packet_family_ops);
	register_pernet_subsys(&packet_net_ops);
	register_netdevice_notifier(&packet_netdev_notifier);
=======
	int rc;

	rc = register_pernet_subsys(&packet_net_ops);
	if (rc)
		goto out;
	rc = register_netdevice_notifier(&packet_netdev_notifier);
	if (rc)
		goto out_pernet;
	rc = proto_register(&packet_proto, 0);
	if (rc)
		goto out_notifier;
	rc = sock_register(&packet_family_ops);
	if (rc)
		goto out_proto;

	return 0;

out_proto:
	proto_unregister(&packet_proto);
out_notifier:
	unregister_netdevice_notifier(&packet_netdev_notifier);
out_pernet:
	unregister_pernet_subsys(&packet_net_ops);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	return rc;
}

module_init(packet_init);
module_exit(packet_exit);
<<<<<<< HEAD
=======
MODULE_DESCRIPTION("Packet socket support (AF_PACKET)");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_LICENSE("GPL");
MODULE_ALIAS_NETPROTO(PF_PACKET);
