<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * INET		An implementation of the TCP/IP protocol suite for the LINUX
 *		operating system.  INET is implemented using the  BSD Socket
 *		interface as the means of communication with the user level.
 *
 *		Definitions for the AF_INET socket handler.
 *
 * Version:	@(#)sock.h	1.0.4	05/13/93
 *
 * Authors:	Ross Biro
 *		Fred N. van Kempen, <waltje@uWalt.NL.Mugnet.ORG>
 *		Corey Minyard <wf-rch!minyard@relay.EU.net>
 *		Florian La Roche <flla@stud.uni-sb.de>
 *
 * Fixes:
 *		Alan Cox	:	Volatiles in skbuff pointers. See
 *					skbuff comments. May be overdone,
 *					better to prove they can be removed
 *					than the reverse.
 *		Alan Cox	:	Added a zapped field for tcp to note
 *					a socket is reset and must stay shut up
 *		Alan Cox	:	New fields for options
 *	Pauline Middelink	:	identd support
 *		Alan Cox	:	Eliminate low level recv/recvfrom
 *		David S. Miller	:	New socket lookup architecture.
 *              Steve Whitehouse:       Default routines for sock_ops
 *              Arnaldo C. Melo :	removed net_pinfo, tp_pinfo and made
 *              			protinfo be just a void pointer, as the
 *              			protocol specific parts were moved to
 *              			respective headers and ipv4/v6, etc now
 *              			use private slabcaches for its socks
 *              Pedro Hortas	:	New flags field for socket options
<<<<<<< HEAD
 *
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#ifndef _SOCK_H
#define _SOCK_H

#include <linux/hardirq.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/list_nulls.h>
#include <linux/timer.h>
#include <linux/cache.h>
<<<<<<< HEAD
=======
#include <linux/bitops.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/lockdep.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>	/* struct sk_buff */
#include <linux/mm.h>
#include <linux/security.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
<<<<<<< HEAD
#include <linux/memcontrol.h>
#include <linux/res_counter.h>
#include <linux/static_key.h>
#include <linux/aio.h>
#include <linux/sched.h>

#include <linux/filter.h>
#include <linux/rculist_nulls.h>
#include <linux/poll.h>

#include <linux/atomic.h>
#include <net/dst.h>
#include <net/checksum.h>
#include <net/tcp_states.h>

struct cgroup;
struct cgroup_subsys;
#ifdef CONFIG_NET
int mem_cgroup_sockets_init(struct cgroup *cgrp, struct cgroup_subsys *ss);
void mem_cgroup_sockets_destroy(struct cgroup *cgrp);
#else
static inline
int mem_cgroup_sockets_init(struct cgroup *cgrp, struct cgroup_subsys *ss)
{
	return 0;
}
static inline
void mem_cgroup_sockets_destroy(struct cgroup *cgrp)
{
}
#endif
=======
#include <linux/page_counter.h>
#include <linux/memcontrol.h>
#include <linux/static_key.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/cgroup-defs.h>
#include <linux/rbtree.h>
#include <linux/rculist_nulls.h>
#include <linux/poll.h>
#include <linux/sockptr.h>
#include <linux/indirect_call_wrapper.h>
#include <linux/atomic.h>
#include <linux/refcount.h>
#include <linux/llist.h>
#include <net/dst.h>
#include <net/checksum.h>
#include <net/tcp_states.h>
#include <linux/net_tstamp.h>
#include <net/l3mdev.h>
#include <uapi/linux/socket.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * This structure really needs to be cleaned up.
 * Most of it is for TCP, and not used by any of
 * the other protocols.
 */

<<<<<<< HEAD
/* Define this to get the SOCK_DBG debugging facility. */
#define SOCK_DEBUGGING
#ifdef SOCK_DEBUGGING
#define SOCK_DEBUG(sk, msg...) do { if ((sk) && sock_flag((sk), SOCK_DBG)) \
					printk(KERN_DEBUG msg); } while (0)
#else
/* Validate arguments and do nothing */
static inline __printf(2, 3)
void SOCK_DEBUG(struct sock *sk, const char *msg, ...)
{
}
#endif

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* This is the per-socket lock.  The spinlock provides a synchronization
 * between user contexts and software interrupt processing, whereas the
 * mini-semaphore synchronizes multiple users amongst themselves.
 */
typedef struct {
	spinlock_t		slock;
	int			owned;
	wait_queue_head_t	wq;
	/*
	 * We express the mutex-alike socket_lock semantics
	 * to the lock validator by explicitly managing
	 * the slock as a lock variant (in addition to
	 * the slock itself):
	 */
#ifdef CONFIG_DEBUG_LOCK_ALLOC
	struct lockdep_map dep_map;
#endif
} socket_lock_t;

struct sock;
struct proto;
struct net;

<<<<<<< HEAD
=======
typedef __u32 __bitwise __portpair;
typedef __u64 __bitwise __addrpair;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 *	struct sock_common - minimal network layer representation of sockets
 *	@skc_daddr: Foreign IPv4 addr
 *	@skc_rcv_saddr: Bound local IPv4 addr
<<<<<<< HEAD
 *	@skc_hash: hash value used with various protocol lookup tables
 *	@skc_u16hashes: two u16 hash values used by UDP lookup tables
 *	@skc_family: network address family
 *	@skc_state: Connection state
 *	@skc_reuse: %SO_REUSEADDR setting
=======
 *	@skc_addrpair: 8-byte-aligned __u64 union of @skc_daddr & @skc_rcv_saddr
 *	@skc_hash: hash value used with various protocol lookup tables
 *	@skc_u16hashes: two u16 hash values used by UDP lookup tables
 *	@skc_dport: placeholder for inet_dport/tw_dport
 *	@skc_num: placeholder for inet_num/tw_num
 *	@skc_portpair: __u32 union of @skc_dport & @skc_num
 *	@skc_family: network address family
 *	@skc_state: Connection state
 *	@skc_reuse: %SO_REUSEADDR setting
 *	@skc_reuseport: %SO_REUSEPORT setting
 *	@skc_ipv6only: socket is IPV6 only
 *	@skc_net_refcnt: socket is using net ref counting
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *	@skc_bound_dev_if: bound device index if != 0
 *	@skc_bind_node: bind hash linkage for various protocol lookup tables
 *	@skc_portaddr_node: second hash linkage for UDP/UDP-Lite protocol
 *	@skc_prot: protocol handlers inside a network family
 *	@skc_net: reference to the network namespace of this socket
<<<<<<< HEAD
 *	@skc_node: main hash linkage for various protocol lookup tables
 *	@skc_nulls_node: main hash linkage for TCP/UDP/UDP-Lite protocol
 *	@skc_tx_queue_mapping: tx queue number for this connection
=======
 *	@skc_v6_daddr: IPV6 destination address
 *	@skc_v6_rcv_saddr: IPV6 source address
 *	@skc_cookie: socket's cookie value
 *	@skc_node: main hash linkage for various protocol lookup tables
 *	@skc_nulls_node: main hash linkage for TCP/UDP/UDP-Lite protocol
 *	@skc_tx_queue_mapping: tx queue number for this connection
 *	@skc_rx_queue_mapping: rx queue number for this connection
 *	@skc_flags: place holder for sk_flags
 *		%SO_LINGER (l_onoff), %SO_BROADCAST, %SO_KEEPALIVE,
 *		%SO_OOBINLINE settings, %SO_TIMESTAMPING settings
 *	@skc_listener: connection request listener socket (aka rsk_listener)
 *		[union with @skc_flags]
 *	@skc_tw_dr: (aka tw_dr) ptr to &struct inet_timewait_death_row
 *		[union with @skc_flags]
 *	@skc_incoming_cpu: record/match cpu processing incoming packets
 *	@skc_rcv_wnd: (aka rsk_rcv_wnd) TCP receive window size (possibly scaled)
 *		[union with @skc_incoming_cpu]
 *	@skc_tw_rcv_nxt: (aka tw_rcv_nxt) TCP window next expected seq number
 *		[union with @skc_incoming_cpu]
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *	@skc_refcnt: reference count
 *
 *	This is the minimal network layer representation of sockets, the header
 *	for struct sock and struct inet_timewait_sock.
 */
struct sock_common {
<<<<<<< HEAD
	/* skc_daddr and skc_rcv_saddr must be grouped :
	 * cf INET_MATCH() and INET_TW_MATCH()
	 */
	__be32			skc_daddr;
	__be32			skc_rcv_saddr;

=======
	union {
		__addrpair	skc_addrpair;
		struct {
			__be32	skc_daddr;
			__be32	skc_rcv_saddr;
		};
	};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	union  {
		unsigned int	skc_hash;
		__u16		skc_u16hashes[2];
	};
<<<<<<< HEAD
	unsigned short		skc_family;
	volatile unsigned char	skc_state;
	unsigned char		skc_reuse;
	int			skc_bound_dev_if;
	int			padding[2];
	union {
		struct hlist_node	skc_bind_node;
		struct hlist_nulls_node skc_portaddr_node;
	};
	struct proto		*skc_prot;
#ifdef CONFIG_NET_NS
	struct net	 	*skc_net;
#endif
=======
	/* skc_dport && skc_num must be grouped as well */
	union {
		__portpair	skc_portpair;
		struct {
			__be16	skc_dport;
			__u16	skc_num;
		};
	};

	unsigned short		skc_family;
	volatile unsigned char	skc_state;
	unsigned char		skc_reuse:4;
	unsigned char		skc_reuseport:1;
	unsigned char		skc_ipv6only:1;
	unsigned char		skc_net_refcnt:1;
	int			skc_bound_dev_if;
	union {
		struct hlist_node	skc_bind_node;
		struct hlist_node	skc_portaddr_node;
	};
	struct proto		*skc_prot;
	possible_net_t		skc_net;

#if IS_ENABLED(CONFIG_IPV6)
	struct in6_addr		skc_v6_daddr;
	struct in6_addr		skc_v6_rcv_saddr;
#endif

	atomic64_t		skc_cookie;

	/* following fields are padding to force
	 * offset(struct sock, sk_refcnt) == 128 on 64bit arches
	 * assuming IPV6 is enabled. We use this padding differently
	 * for different kind of 'sockets'
	 */
	union {
		unsigned long	skc_flags;
		struct sock	*skc_listener; /* request_sock */
		struct inet_timewait_death_row *skc_tw_dr; /* inet_timewait_sock */
	};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * fields between dontcopy_begin/dontcopy_end
	 * are not copied in sock_copy()
	 */
	/* private: */
	int			skc_dontcopy_begin[0];
	/* public: */
	union {
		struct hlist_node	skc_node;
		struct hlist_nulls_node skc_nulls_node;
	};
<<<<<<< HEAD
	int			skc_tx_queue_mapping;
	atomic_t		skc_refcnt;
	/* private: */
	int                     skc_dontcopy_end[0];
	/* public: */
};

struct cg_proto;
=======
	unsigned short		skc_tx_queue_mapping;
#ifdef CONFIG_SOCK_RX_QUEUE_MAPPING
	unsigned short		skc_rx_queue_mapping;
#endif
	union {
		int		skc_incoming_cpu;
		u32		skc_rcv_wnd;
		u32		skc_tw_rcv_nxt; /* struct tcp_timewait_sock  */
	};

	refcount_t		skc_refcnt;
	/* private: */
	int                     skc_dontcopy_end[0];
	union {
		u32		skc_rxhash;
		u32		skc_window_clamp;
		u32		skc_tw_snd_nxt; /* struct tcp_timewait_sock */
	};
	/* public: */
};

struct bpf_local_storage;
struct sk_filter;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
  *	struct sock - network layer representation of sockets
  *	@__sk_common: shared layout with inet_timewait_sock
  *	@sk_shutdown: mask of %SEND_SHUTDOWN and/or %RCV_SHUTDOWN
  *	@sk_userlocks: %SO_SNDBUF and %SO_RCVBUF settings
  *	@sk_lock:	synchronizer
<<<<<<< HEAD
  *	@sk_rcvbuf: size of receive buffer in bytes
  *	@sk_wq: sock wait queue and async head
  *	@sk_dst_cache: destination cache
  *	@sk_dst_lock: destination cache lock
  *	@sk_policy: flow policy
  *	@sk_receive_queue: incoming packets
  *	@sk_wmem_alloc: transmit queue bytes committed
  *	@sk_write_queue: Packet sending queue
  *	@sk_async_wait_queue: DMA copied packets
  *	@sk_omem_alloc: "o" is "option" or "other"
  *	@sk_wmem_queued: persistent queue size
  *	@sk_forward_alloc: space allocated forward
  *	@sk_allocation: allocation mode
  *	@sk_pacing_rate: Pacing rate (if supported by transport/packet scheduler)
  *	@sk_max_pacing_rate: Maximum pacing rate (%SO_MAX_PACING_RATE)
  *	@sk_sndbuf: size of send buffer in bytes
  *	@sk_flags: %SO_LINGER (l_onoff), %SO_BROADCAST, %SO_KEEPALIVE,
  *		   %SO_OOBINLINE settings, %SO_TIMESTAMPING settings
  *	@sk_no_check: %SO_NO_CHECK setting, wether or not checkup packets
  *	@sk_route_caps: route capabilities (e.g. %NETIF_F_TSO)
  *	@sk_route_nocaps: forbidden route capabilities (e.g NETIF_F_GSO_MASK)
  *	@sk_gso_type: GSO type (e.g. %SKB_GSO_TCPV4)
  *	@sk_gso_max_size: Maximum GSO segment size to build
  *	@sk_gso_max_segs: Maximum number of GSO segments
=======
  *	@sk_kern_sock: True if sock is using kernel lock classes
  *	@sk_rcvbuf: size of receive buffer in bytes
  *	@sk_wq: sock wait queue and async head
  *	@sk_rx_dst: receive input route used by early demux
  *	@sk_rx_dst_ifindex: ifindex for @sk_rx_dst
  *	@sk_rx_dst_cookie: cookie for @sk_rx_dst
  *	@sk_dst_cache: destination cache
  *	@sk_dst_pending_confirm: need to confirm neighbour
  *	@sk_policy: flow policy
  *	@sk_receive_queue: incoming packets
  *	@sk_wmem_alloc: transmit queue bytes committed
  *	@sk_tsq_flags: TCP Small Queues flags
  *	@sk_write_queue: Packet sending queue
  *	@sk_omem_alloc: "o" is "option" or "other"
  *	@sk_wmem_queued: persistent queue size
  *	@sk_forward_alloc: space allocated forward
  *	@sk_reserved_mem: space reserved and non-reclaimable for the socket
  *	@sk_napi_id: id of the last napi context to receive data for sk
  *	@sk_ll_usec: usecs to busypoll when there is no data
  *	@sk_allocation: allocation mode
  *	@sk_pacing_rate: Pacing rate (if supported by transport/packet scheduler)
  *	@sk_pacing_status: Pacing status (requested, handled by sch_fq)
  *	@sk_max_pacing_rate: Maximum pacing rate (%SO_MAX_PACING_RATE)
  *	@sk_sndbuf: size of send buffer in bytes
  *	@sk_no_check_tx: %SO_NO_CHECK setting, set checksum in TX packets
  *	@sk_no_check_rx: allow zero checksum in RX packets
  *	@sk_route_caps: route capabilities (e.g. %NETIF_F_TSO)
  *	@sk_gso_disabled: if set, NETIF_F_GSO_MASK is forbidden.
  *	@sk_gso_type: GSO type (e.g. %SKB_GSO_TCPV4)
  *	@sk_gso_max_size: Maximum GSO segment size to build
  *	@sk_gso_max_segs: Maximum number of GSO segments
  *	@sk_pacing_shift: scaling factor for TCP Small Queues
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
  *	@sk_lingertime: %SO_LINGER l_linger setting
  *	@sk_backlog: always used with the per-socket spinlock held
  *	@sk_callback_lock: used with the callbacks in the end of this struct
  *	@sk_error_queue: rarely used
  *	@sk_prot_creator: sk_prot of original sock creator (see ipv6_setsockopt,
  *			  IPV6_ADDRFORM for instance)
  *	@sk_err: last error
  *	@sk_err_soft: errors that don't cause failure but are the cause of a
  *		      persistent failure not just 'timed out'
  *	@sk_drops: raw/udp drops counter
  *	@sk_ack_backlog: current listen backlog
  *	@sk_max_ack_backlog: listen backlog set in listen()
<<<<<<< HEAD
  *	@sk_priority: %SO_PRIORITY setting
  *	@sk_cgrp_prioidx: socket group's priority map index
  *	@sk_type: socket type (%SOCK_STREAM, etc)
  *	@sk_protocol: which protocol this socket belongs in this network family
=======
  *	@sk_uid: user id of owner
  *	@sk_prefer_busy_poll: prefer busypolling over softirq processing
  *	@sk_busy_poll_budget: napi processing budget when busypolling
  *	@sk_priority: %SO_PRIORITY setting
  *	@sk_type: socket type (%SOCK_STREAM, etc)
  *	@sk_protocol: which protocol this socket belongs in this network family
  *	@sk_peer_lock: lock protecting @sk_peer_pid and @sk_peer_cred
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
  *	@sk_peer_pid: &struct pid for this socket's peer
  *	@sk_peer_cred: %SO_PEERCRED setting
  *	@sk_rcvlowat: %SO_RCVLOWAT setting
  *	@sk_rcvtimeo: %SO_RCVTIMEO setting
  *	@sk_sndtimeo: %SO_SNDTIMEO setting
<<<<<<< HEAD
  *	@sk_rxhash: flow hash received from netif layer
  *	@sk_filter: socket filtering instructions
  *	@sk_protinfo: private area, net family specific, when not using slab
  *	@sk_timer: sock cleanup timer
  *	@sk_stamp: time stamp of last packet received
  *	@sk_socket: Identd and reporting IO signals
  *	@sk_user_data: RPC layer private data
  *	@sk_sndmsg_page: cached page for sendmsg
  *	@sk_sndmsg_off: cached offset for sendmsg
  *	@sk_peek_off: current peek_offset value
  *	@sk_send_head: front of stuff to transmit
  *	@sk_security: used by security modules
  *	@sk_mark: generic packet mark
  *	@sk_classid: this socket's cgroup classid
  *	@sk_cgrp: this socket's cgroup-specific proto data
  *	@sk_write_pending: a write to stream socket waits to start
=======
  *	@sk_txhash: computed flow hash for use on transmit
  *	@sk_txrehash: enable TX hash rethink
  *	@sk_filter: socket filtering instructions
  *	@sk_timer: sock cleanup timer
  *	@sk_stamp: time stamp of last packet received
  *	@sk_stamp_seq: lock for accessing sk_stamp on 32 bit architectures only
  *	@sk_tsflags: SO_TIMESTAMPING flags
  *	@sk_use_task_frag: allow sk_page_frag() to use current->task_frag.
  *			   Sockets that can be used under memory reclaim should
  *			   set this to false.
  *	@sk_bind_phc: SO_TIMESTAMPING bind PHC index of PTP virtual clock
  *	              for timestamping
  *	@sk_tskey: counter to disambiguate concurrent tstamp requests
  *	@sk_zckey: counter to order MSG_ZEROCOPY notifications
  *	@sk_socket: Identd and reporting IO signals
  *	@sk_user_data: RPC layer private data. Write-protected by @sk_callback_lock.
  *	@sk_frag: cached page frag
  *	@sk_peek_off: current peek_offset value
  *	@sk_send_head: front of stuff to transmit
  *	@tcp_rtx_queue: TCP re-transmit queue [union with @sk_send_head]
  *	@sk_security: used by security modules
  *	@sk_mark: generic packet mark
  *	@sk_cgrp_data: cgroup data for this cgroup
  *	@sk_memcg: this socket's memory cgroup association
  *	@sk_write_pending: a write to stream socket waits to start
  *	@sk_disconnects: number of disconnect operations performed on this sock
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
  *	@sk_state_change: callback to indicate change in the state of the sock
  *	@sk_data_ready: callback to indicate there is data to be processed
  *	@sk_write_space: callback to indicate there is bf sending space available
  *	@sk_error_report: callback to indicate errors (e.g. %MSG_ERRQUEUE)
  *	@sk_backlog_rcv: callback to process the backlog
<<<<<<< HEAD
  *	@sk_destruct: called at sock freeing time, i.e. when all refcnt == 0
 */
=======
  *	@sk_validate_xmit_skb: ptr to an optional validate function
  *	@sk_destruct: called at sock freeing time, i.e. when all refcnt == 0
  *	@sk_reuseport_cb: reuseport group container
  *	@sk_bpf_storage: ptr to cache and control for bpf_sk_storage
  *	@sk_rcu: used during RCU grace period
  *	@sk_clockid: clockid used by time-based scheduling (SO_TXTIME)
  *	@sk_txtime_deadline_mode: set deadline mode for SO_TXTIME
  *	@sk_txtime_report_errors: set report errors mode for SO_TXTIME
  *	@sk_txtime_unused: unused txtime flags
  *	@ns_tracker: tracker for netns reference
  */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct sock {
	/*
	 * Now struct inet_timewait_sock also uses sock_common, so please just
	 * don't add nothing before this first member (__sk_common) --acme
	 */
	struct sock_common	__sk_common;
#define sk_node			__sk_common.skc_node
#define sk_nulls_node		__sk_common.skc_nulls_node
#define sk_refcnt		__sk_common.skc_refcnt
#define sk_tx_queue_mapping	__sk_common.skc_tx_queue_mapping
<<<<<<< HEAD
=======
#ifdef CONFIG_SOCK_RX_QUEUE_MAPPING
#define sk_rx_queue_mapping	__sk_common.skc_rx_queue_mapping
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define sk_dontcopy_begin	__sk_common.skc_dontcopy_begin
#define sk_dontcopy_end		__sk_common.skc_dontcopy_end
#define sk_hash			__sk_common.skc_hash
<<<<<<< HEAD
#define sk_family		__sk_common.skc_family
#define sk_state		__sk_common.skc_state
#define sk_reuse		__sk_common.skc_reuse
=======
#define sk_portpair		__sk_common.skc_portpair
#define sk_num			__sk_common.skc_num
#define sk_dport		__sk_common.skc_dport
#define sk_addrpair		__sk_common.skc_addrpair
#define sk_daddr		__sk_common.skc_daddr
#define sk_rcv_saddr		__sk_common.skc_rcv_saddr
#define sk_family		__sk_common.skc_family
#define sk_state		__sk_common.skc_state
#define sk_reuse		__sk_common.skc_reuse
#define sk_reuseport		__sk_common.skc_reuseport
#define sk_ipv6only		__sk_common.skc_ipv6only
#define sk_net_refcnt		__sk_common.skc_net_refcnt
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define sk_bound_dev_if		__sk_common.skc_bound_dev_if
#define sk_bind_node		__sk_common.skc_bind_node
#define sk_prot			__sk_common.skc_prot
#define sk_net			__sk_common.skc_net
<<<<<<< HEAD
	socket_lock_t		sk_lock;
=======
#define sk_v6_daddr		__sk_common.skc_v6_daddr
#define sk_v6_rcv_saddr	__sk_common.skc_v6_rcv_saddr
#define sk_cookie		__sk_common.skc_cookie
#define sk_incoming_cpu		__sk_common.skc_incoming_cpu
#define sk_flags		__sk_common.skc_flags
#define sk_rxhash		__sk_common.skc_rxhash

	__cacheline_group_begin(sock_write_rx);

	atomic_t		sk_drops;
	__s32			sk_peek_off;
	struct sk_buff_head	sk_error_queue;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct sk_buff_head	sk_receive_queue;
	/*
	 * The backlog queue is special, it is always used with
	 * the per-socket spinlock held and requires low latency
	 * access. Therefore we special case it's implementation.
	 * Note : rmem_alloc is in this structure to fill a hole
	 * on 64bit arches, not because its logically part of
	 * backlog.
	 */
	struct {
		atomic_t	rmem_alloc;
		int		len;
		struct sk_buff	*head;
		struct sk_buff	*tail;
	} sk_backlog;
#define sk_rmem_alloc sk_backlog.rmem_alloc
<<<<<<< HEAD
	int			sk_forward_alloc;
#ifdef CONFIG_RPS
	__u32			sk_rxhash;
#endif
	atomic_t		sk_drops;
	int			sk_rcvbuf;

	struct sk_filter __rcu	*sk_filter;
	struct socket_wq __rcu	*sk_wq;

#ifdef CONFIG_NET_DMA
	struct sk_buff_head	sk_async_wait_queue;
#endif

#ifdef CONFIG_XFRM
	struct xfrm_policy	*sk_policy[2];
#endif
	unsigned long 		sk_flags;
	struct dst_entry	*sk_dst_cache;
	spinlock_t		sk_dst_lock;
	atomic_t		sk_wmem_alloc;
	atomic_t		sk_omem_alloc;
	int			sk_sndbuf;
	struct sk_buff_head	sk_write_queue;
	kmemcheck_bitfield_begin(flags);
	unsigned int		sk_shutdown  : 2,
				sk_no_check  : 2,
				sk_userlocks : 4,
				sk_protocol  : 8,
				sk_type      : 16;
#define SK_PROTOCOL_MAX ((u8)~0U)
	kmemcheck_bitfield_end(flags);
	int			sk_wmem_queued;
	gfp_t			sk_allocation;
	u32			sk_pacing_rate; /* bytes per second */
	u32			sk_max_pacing_rate;
	netdev_features_t	sk_route_caps;
	netdev_features_t	sk_route_nocaps;
	int			sk_gso_type;
	unsigned int		sk_gso_max_size;
	u16			sk_gso_max_segs;
	int			sk_rcvlowat;
	unsigned long	        sk_lingertime;
	struct sk_buff_head	sk_error_queue;
	struct proto		*sk_prot_creator;
	rwlock_t		sk_callback_lock;
	int			sk_err,
				sk_err_soft;
	unsigned short		sk_ack_backlog;
	unsigned short		sk_max_ack_backlog;
	__u32			sk_priority;
#ifdef CONFIG_CGROUPS
	__u32			sk_cgrp_prioidx;
#endif
	struct pid		*sk_peer_pid;
	const struct cred	*sk_peer_cred;
	long			sk_rcvtimeo;
	long			sk_sndtimeo;
	void			*sk_protinfo;
	struct timer_list	sk_timer;
	ktime_t			sk_stamp;
	struct socket		*sk_socket;
	void			*sk_user_data;
	struct page		*sk_sndmsg_page;
	struct sk_buff		*sk_send_head;
	__u32			sk_sndmsg_off;
	__s32			sk_peek_off;
	int			sk_write_pending;
#ifdef CONFIG_SECURITY
	void			*sk_security;
#endif
	__u32			sk_mark;
	kuid_t			sk_uid;
	u32			sk_classid;
	struct cg_proto		*sk_cgrp;
	void			(*sk_state_change)(struct sock *sk);
	void			(*sk_data_ready)(struct sock *sk, int bytes);
	void			(*sk_write_space)(struct sock *sk);
	void			(*sk_error_report)(struct sock *sk);
  	int			(*sk_backlog_rcv)(struct sock *sk,
						  struct sk_buff *skb);  
	void                    (*sk_destruct)(struct sock *sk);
};

static inline int sk_peek_offset(struct sock *sk, int flags)
{
	if ((flags & MSG_PEEK) && (sk->sk_peek_off >= 0))
		return sk->sk_peek_off;
	else
		return 0;
=======

	__cacheline_group_end(sock_write_rx);

	__cacheline_group_begin(sock_read_rx);
	/* early demux fields */
	struct dst_entry __rcu	*sk_rx_dst;
	int			sk_rx_dst_ifindex;
	u32			sk_rx_dst_cookie;

#ifdef CONFIG_NET_RX_BUSY_POLL
	unsigned int		sk_ll_usec;
	unsigned int		sk_napi_id;
	u16			sk_busy_poll_budget;
	u8			sk_prefer_busy_poll;
#endif
	u8			sk_userlocks;
	int			sk_rcvbuf;

	struct sk_filter __rcu	*sk_filter;
	union {
		struct socket_wq __rcu	*sk_wq;
		/* private: */
		struct socket_wq	*sk_wq_raw;
		/* public: */
	};

	void			(*sk_data_ready)(struct sock *sk);
	long			sk_rcvtimeo;
	int			sk_rcvlowat;
	__cacheline_group_end(sock_read_rx);

	__cacheline_group_begin(sock_read_rxtx);
	int			sk_err;
	struct socket		*sk_socket;
	struct mem_cgroup	*sk_memcg;
#ifdef CONFIG_XFRM
	struct xfrm_policy __rcu *sk_policy[2];
#endif
	__cacheline_group_end(sock_read_rxtx);

	__cacheline_group_begin(sock_write_rxtx);
	socket_lock_t		sk_lock;
	u32			sk_reserved_mem;
	int			sk_forward_alloc;
	u32			sk_tsflags;
	__cacheline_group_end(sock_write_rxtx);

	__cacheline_group_begin(sock_write_tx);
	int			sk_write_pending;
	atomic_t		sk_omem_alloc;
	int			sk_sndbuf;

	int			sk_wmem_queued;
	refcount_t		sk_wmem_alloc;
	unsigned long		sk_tsq_flags;
	union {
		struct sk_buff	*sk_send_head;
		struct rb_root	tcp_rtx_queue;
	};
	struct sk_buff_head	sk_write_queue;
	u32			sk_dst_pending_confirm;
	u32			sk_pacing_status; /* see enum sk_pacing */
	struct page_frag	sk_frag;
	struct timer_list	sk_timer;

	unsigned long		sk_pacing_rate; /* bytes per second */
	atomic_t		sk_zckey;
	atomic_t		sk_tskey;
	__cacheline_group_end(sock_write_tx);

	__cacheline_group_begin(sock_read_tx);
	unsigned long		sk_max_pacing_rate;
	long			sk_sndtimeo;
	u32			sk_priority;
	u32			sk_mark;
	struct dst_entry __rcu	*sk_dst_cache;
	netdev_features_t	sk_route_caps;
#ifdef CONFIG_SOCK_VALIDATE_XMIT
	struct sk_buff*		(*sk_validate_xmit_skb)(struct sock *sk,
							struct net_device *dev,
							struct sk_buff *skb);
#endif
	u16			sk_gso_type;
	u16			sk_gso_max_segs;
	unsigned int		sk_gso_max_size;
	gfp_t			sk_allocation;
	u32			sk_txhash;
	u8			sk_pacing_shift;
	bool			sk_use_task_frag;
	__cacheline_group_end(sock_read_tx);

	/*
	 * Because of non atomicity rules, all
	 * changes are protected by socket lock.
	 */
	u8			sk_gso_disabled : 1,
				sk_kern_sock : 1,
				sk_no_check_tx : 1,
				sk_no_check_rx : 1;
	u8			sk_shutdown;
	u16			sk_type;
	u16			sk_protocol;
	unsigned long	        sk_lingertime;
	struct proto		*sk_prot_creator;
	rwlock_t		sk_callback_lock;
	int			sk_err_soft;
	u32			sk_ack_backlog;
	u32			sk_max_ack_backlog;
	kuid_t			sk_uid;
	spinlock_t		sk_peer_lock;
	int			sk_bind_phc;
	struct pid		*sk_peer_pid;
	const struct cred	*sk_peer_cred;

	ktime_t			sk_stamp;
#if BITS_PER_LONG==32
	seqlock_t		sk_stamp_seq;
#endif
	int			sk_disconnects;

	u8			sk_txrehash;
	u8			sk_clockid;
	u8			sk_txtime_deadline_mode : 1,
				sk_txtime_report_errors : 1,
				sk_txtime_unused : 6;

	void			*sk_user_data;
#ifdef CONFIG_SECURITY
	void			*sk_security;
#endif
	struct sock_cgroup_data	sk_cgrp_data;
	void			(*sk_state_change)(struct sock *sk);
	void			(*sk_write_space)(struct sock *sk);
	void			(*sk_error_report)(struct sock *sk);
	int			(*sk_backlog_rcv)(struct sock *sk,
						  struct sk_buff *skb);
	void                    (*sk_destruct)(struct sock *sk);
	struct sock_reuseport __rcu	*sk_reuseport_cb;
#ifdef CONFIG_BPF_SYSCALL
	struct bpf_local_storage __rcu	*sk_bpf_storage;
#endif
	struct rcu_head		sk_rcu;
	netns_tracker		ns_tracker;
};

enum sk_pacing {
	SK_PACING_NONE		= 0,
	SK_PACING_NEEDED	= 1,
	SK_PACING_FQ		= 2,
};

/* flag bits in sk_user_data
 *
 * - SK_USER_DATA_NOCOPY:      Pointer stored in sk_user_data might
 *   not be suitable for copying when cloning the socket. For instance,
 *   it can point to a reference counted object. sk_user_data bottom
 *   bit is set if pointer must not be copied.
 *
 * - SK_USER_DATA_BPF:         Mark whether sk_user_data field is
 *   managed/owned by a BPF reuseport array. This bit should be set
 *   when sk_user_data's sk is added to the bpf's reuseport_array.
 *
 * - SK_USER_DATA_PSOCK:       Mark whether pointer stored in
 *   sk_user_data points to psock type. This bit should be set
 *   when sk_user_data is assigned to a psock object.
 */
#define SK_USER_DATA_NOCOPY	1UL
#define SK_USER_DATA_BPF	2UL
#define SK_USER_DATA_PSOCK	4UL
#define SK_USER_DATA_PTRMASK	~(SK_USER_DATA_NOCOPY | SK_USER_DATA_BPF |\
				  SK_USER_DATA_PSOCK)

/**
 * sk_user_data_is_nocopy - Test if sk_user_data pointer must not be copied
 * @sk: socket
 */
static inline bool sk_user_data_is_nocopy(const struct sock *sk)
{
	return ((uintptr_t)sk->sk_user_data & SK_USER_DATA_NOCOPY);
}

#define __sk_user_data(sk) ((*((void __rcu **)&(sk)->sk_user_data)))

/**
 * __locked_read_sk_user_data_with_flags - return the pointer
 * only if argument flags all has been set in sk_user_data. Otherwise
 * return NULL
 *
 * @sk: socket
 * @flags: flag bits
 *
 * The caller must be holding sk->sk_callback_lock.
 */
static inline void *
__locked_read_sk_user_data_with_flags(const struct sock *sk,
				      uintptr_t flags)
{
	uintptr_t sk_user_data =
		(uintptr_t)rcu_dereference_check(__sk_user_data(sk),
						 lockdep_is_held(&sk->sk_callback_lock));

	WARN_ON_ONCE(flags & SK_USER_DATA_PTRMASK);

	if ((sk_user_data & flags) == flags)
		return (void *)(sk_user_data & SK_USER_DATA_PTRMASK);
	return NULL;
}

/**
 * __rcu_dereference_sk_user_data_with_flags - return the pointer
 * only if argument flags all has been set in sk_user_data. Otherwise
 * return NULL
 *
 * @sk: socket
 * @flags: flag bits
 */
static inline void *
__rcu_dereference_sk_user_data_with_flags(const struct sock *sk,
					  uintptr_t flags)
{
	uintptr_t sk_user_data = (uintptr_t)rcu_dereference(__sk_user_data(sk));

	WARN_ON_ONCE(flags & SK_USER_DATA_PTRMASK);

	if ((sk_user_data & flags) == flags)
		return (void *)(sk_user_data & SK_USER_DATA_PTRMASK);
	return NULL;
}

#define rcu_dereference_sk_user_data(sk)				\
	__rcu_dereference_sk_user_data_with_flags(sk, 0)
#define __rcu_assign_sk_user_data_with_flags(sk, ptr, flags)		\
({									\
	uintptr_t __tmp1 = (uintptr_t)(ptr),				\
		  __tmp2 = (uintptr_t)(flags);				\
	WARN_ON_ONCE(__tmp1 & ~SK_USER_DATA_PTRMASK);			\
	WARN_ON_ONCE(__tmp2 & SK_USER_DATA_PTRMASK);			\
	rcu_assign_pointer(__sk_user_data((sk)),			\
			   __tmp1 | __tmp2);				\
})
#define rcu_assign_sk_user_data(sk, ptr)				\
	__rcu_assign_sk_user_data_with_flags(sk, ptr, 0)

static inline
struct net *sock_net(const struct sock *sk)
{
	return read_pnet(&sk->sk_net);
}

static inline
void sock_net_set(struct sock *sk, struct net *net)
{
	write_pnet(&sk->sk_net, net);
}

/*
 * SK_CAN_REUSE and SK_NO_REUSE on a socket mean that the socket is OK
 * or not whether his port will be reused by someone else. SK_FORCE_REUSE
 * on a socket means that the socket will reuse everybody else's port
 * without looking at the other's sk_reuse value.
 */

#define SK_NO_REUSE	0
#define SK_CAN_REUSE	1
#define SK_FORCE_REUSE	2

int sk_set_peek_off(struct sock *sk, int val);

static inline int sk_peek_offset(const struct sock *sk, int flags)
{
	if (unlikely(flags & MSG_PEEK)) {
		return READ_ONCE(sk->sk_peek_off);
	}

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void sk_peek_offset_bwd(struct sock *sk, int val)
{
<<<<<<< HEAD
	if (sk->sk_peek_off >= 0) {
		if (sk->sk_peek_off >= val)
			sk->sk_peek_off -= val;
		else
			sk->sk_peek_off = 0;
=======
	s32 off = READ_ONCE(sk->sk_peek_off);

	if (unlikely(off >= 0)) {
		off = max_t(s32, off - val, 0);
		WRITE_ONCE(sk->sk_peek_off, off);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

static inline void sk_peek_offset_fwd(struct sock *sk, int val)
{
<<<<<<< HEAD
	if (sk->sk_peek_off >= 0)
		sk->sk_peek_off += val;
=======
	sk_peek_offset_bwd(sk, -val);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Hashed lists helper routines
 */
static inline struct sock *sk_entry(const struct hlist_node *node)
{
	return hlist_entry(node, struct sock, sk_node);
}

static inline struct sock *__sk_head(const struct hlist_head *head)
{
	return hlist_entry(head->first, struct sock, sk_node);
}

static inline struct sock *sk_head(const struct hlist_head *head)
{
	return hlist_empty(head) ? NULL : __sk_head(head);
}

static inline struct sock *__sk_nulls_head(const struct hlist_nulls_head *head)
{
	return hlist_nulls_entry(head->first, struct sock, sk_nulls_node);
}

static inline struct sock *sk_nulls_head(const struct hlist_nulls_head *head)
{
	return hlist_nulls_empty(head) ? NULL : __sk_nulls_head(head);
}

static inline struct sock *sk_next(const struct sock *sk)
{
<<<<<<< HEAD
	return sk->sk_node.next ?
		hlist_entry(sk->sk_node.next, struct sock, sk_node) : NULL;
=======
	return hlist_entry_safe(sk->sk_node.next, struct sock, sk_node);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline struct sock *sk_nulls_next(const struct sock *sk)
{
	return (!is_a_nulls(sk->sk_nulls_node.next)) ?
		hlist_nulls_entry(sk->sk_nulls_node.next,
				  struct sock, sk_nulls_node) :
		NULL;
}

<<<<<<< HEAD
static inline int sk_unhashed(const struct sock *sk)
=======
static inline bool sk_unhashed(const struct sock *sk)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return hlist_unhashed(&sk->sk_node);
}

<<<<<<< HEAD
static inline int sk_hashed(const struct sock *sk)
=======
static inline bool sk_hashed(const struct sock *sk)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return !sk_unhashed(sk);
}

<<<<<<< HEAD
static __inline__ void sk_node_init(struct hlist_node *node)
=======
static inline void sk_node_init(struct hlist_node *node)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	node->pprev = NULL;
}

<<<<<<< HEAD
static __inline__ void sk_nulls_node_init(struct hlist_nulls_node *node)
{
	node->pprev = NULL;
}

static __inline__ void __sk_del_node(struct sock *sk)
=======
static inline void __sk_del_node(struct sock *sk)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	__hlist_del(&sk->sk_node);
}

/* NB: equivalent to hlist_del_init_rcu */
<<<<<<< HEAD
static __inline__ int __sk_del_node_init(struct sock *sk)
=======
static inline bool __sk_del_node_init(struct sock *sk)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	if (sk_hashed(sk)) {
		__sk_del_node(sk);
		sk_node_init(&sk->sk_node);
<<<<<<< HEAD
		return 1;
	}
	return 0;
=======
		return true;
	}
	return false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Grab socket reference count. This operation is valid only
   when sk is ALREADY grabbed f.e. it is found in hash table
   or a list and the lookup is made under lock preventing hash table
   modifications.
 */

<<<<<<< HEAD
static inline void sock_hold(struct sock *sk)
{
	atomic_inc(&sk->sk_refcnt);
=======
static __always_inline void sock_hold(struct sock *sk)
{
	refcount_inc(&sk->sk_refcnt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Ungrab socket in the context, which assumes that socket refcnt
   cannot hit zero, f.e. it is true in context of any socketcall.
 */
<<<<<<< HEAD
static inline void __sock_put(struct sock *sk)
{
	atomic_dec(&sk->sk_refcnt);
}

static __inline__ int sk_del_node_init(struct sock *sk)
{
	int rc = __sk_del_node_init(sk);

	if (rc) {
		/* paranoid for a while -acme */
		WARN_ON(atomic_read(&sk->sk_refcnt) == 1);
=======
static __always_inline void __sock_put(struct sock *sk)
{
	refcount_dec(&sk->sk_refcnt);
}

static inline bool sk_del_node_init(struct sock *sk)
{
	bool rc = __sk_del_node_init(sk);

	if (rc) {
		/* paranoid for a while -acme */
		WARN_ON(refcount_read(&sk->sk_refcnt) == 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		__sock_put(sk);
	}
	return rc;
}
#define sk_del_node_init_rcu(sk)	sk_del_node_init(sk)

<<<<<<< HEAD
static __inline__ int __sk_nulls_del_node_init_rcu(struct sock *sk)
{
	if (sk_hashed(sk)) {
		hlist_nulls_del_init_rcu(&sk->sk_nulls_node);
		return 1;
	}
	return 0;
}

static __inline__ int sk_nulls_del_node_init_rcu(struct sock *sk)
{
	int rc = __sk_nulls_del_node_init_rcu(sk);

	if (rc) {
		/* paranoid for a while -acme */
		WARN_ON(atomic_read(&sk->sk_refcnt) == 1);
=======
static inline bool __sk_nulls_del_node_init_rcu(struct sock *sk)
{
	if (sk_hashed(sk)) {
		hlist_nulls_del_init_rcu(&sk->sk_nulls_node);
		return true;
	}
	return false;
}

static inline bool sk_nulls_del_node_init_rcu(struct sock *sk)
{
	bool rc = __sk_nulls_del_node_init_rcu(sk);

	if (rc) {
		/* paranoid for a while -acme */
		WARN_ON(refcount_read(&sk->sk_refcnt) == 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		__sock_put(sk);
	}
	return rc;
}

<<<<<<< HEAD
static __inline__ void __sk_add_node(struct sock *sk, struct hlist_head *list)
=======
static inline void __sk_add_node(struct sock *sk, struct hlist_head *list)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	hlist_add_head(&sk->sk_node, list);
}

<<<<<<< HEAD
static __inline__ void sk_add_node(struct sock *sk, struct hlist_head *list)
=======
static inline void sk_add_node(struct sock *sk, struct hlist_head *list)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	sock_hold(sk);
	__sk_add_node(sk, list);
}

<<<<<<< HEAD
static __inline__ void sk_add_node_rcu(struct sock *sk, struct hlist_head *list)
{
	sock_hold(sk);
	hlist_add_head_rcu(&sk->sk_node, list);
}

static __inline__ void __sk_nulls_add_node_rcu(struct sock *sk, struct hlist_nulls_head *list)
=======
static inline void sk_add_node_rcu(struct sock *sk, struct hlist_head *list)
{
	sock_hold(sk);
	if (IS_ENABLED(CONFIG_IPV6) && sk->sk_reuseport &&
	    sk->sk_family == AF_INET6)
		hlist_add_tail_rcu(&sk->sk_node, list);
	else
		hlist_add_head_rcu(&sk->sk_node, list);
}

static inline void sk_add_node_tail_rcu(struct sock *sk, struct hlist_head *list)
{
	sock_hold(sk);
	hlist_add_tail_rcu(&sk->sk_node, list);
}

static inline void __sk_nulls_add_node_rcu(struct sock *sk, struct hlist_nulls_head *list)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	hlist_nulls_add_head_rcu(&sk->sk_nulls_node, list);
}

<<<<<<< HEAD
static __inline__ void sk_nulls_add_node_rcu(struct sock *sk, struct hlist_nulls_head *list)
=======
static inline void __sk_nulls_add_node_tail_rcu(struct sock *sk, struct hlist_nulls_head *list)
{
	hlist_nulls_add_tail_rcu(&sk->sk_nulls_node, list);
}

static inline void sk_nulls_add_node_rcu(struct sock *sk, struct hlist_nulls_head *list)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	sock_hold(sk);
	__sk_nulls_add_node_rcu(sk, list);
}

<<<<<<< HEAD
static __inline__ void __sk_del_bind_node(struct sock *sk)
=======
static inline void __sk_del_bind_node(struct sock *sk)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	__hlist_del(&sk->sk_bind_node);
}

<<<<<<< HEAD
static __inline__ void sk_add_bind_node(struct sock *sk,
=======
static inline void sk_add_bind_node(struct sock *sk,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					struct hlist_head *list)
{
	hlist_add_head(&sk->sk_bind_node, list);
}

<<<<<<< HEAD
#define sk_for_each(__sk, node, list) \
	hlist_for_each_entry(__sk, node, list, sk_node)
#define sk_for_each_rcu(__sk, node, list) \
	hlist_for_each_entry_rcu(__sk, node, list, sk_node)
=======
#define sk_for_each(__sk, list) \
	hlist_for_each_entry(__sk, list, sk_node)
#define sk_for_each_rcu(__sk, list) \
	hlist_for_each_entry_rcu(__sk, list, sk_node)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define sk_nulls_for_each(__sk, node, list) \
	hlist_nulls_for_each_entry(__sk, node, list, sk_nulls_node)
#define sk_nulls_for_each_rcu(__sk, node, list) \
	hlist_nulls_for_each_entry_rcu(__sk, node, list, sk_nulls_node)
<<<<<<< HEAD
#define sk_for_each_from(__sk, node) \
	if (__sk && ({ node = &(__sk)->sk_node; 1; })) \
		hlist_for_each_entry_from(__sk, node, sk_node)
#define sk_nulls_for_each_from(__sk, node) \
	if (__sk && ({ node = &(__sk)->sk_nulls_node; 1; })) \
		hlist_nulls_for_each_entry_from(__sk, node, sk_nulls_node)
#define sk_for_each_safe(__sk, node, tmp, list) \
	hlist_for_each_entry_safe(__sk, node, tmp, list, sk_node)
#define sk_for_each_bound(__sk, node, list) \
	hlist_for_each_entry(__sk, node, list, sk_bind_node)
=======
#define sk_for_each_from(__sk) \
	hlist_for_each_entry_from(__sk, sk_node)
#define sk_nulls_for_each_from(__sk, node) \
	if (__sk && ({ node = &(__sk)->sk_nulls_node; 1; })) \
		hlist_nulls_for_each_entry_from(__sk, node, sk_nulls_node)
#define sk_for_each_safe(__sk, tmp, list) \
	hlist_for_each_entry_safe(__sk, tmp, list, sk_node)
#define sk_for_each_bound(__sk, list) \
	hlist_for_each_entry(__sk, list, sk_bind_node)

/**
 * sk_for_each_entry_offset_rcu - iterate over a list at a given struct offset
 * @tpos:	the type * to use as a loop cursor.
 * @pos:	the &struct hlist_node to use as a loop cursor.
 * @head:	the head for your list.
 * @offset:	offset of hlist_node within the struct.
 *
 */
#define sk_for_each_entry_offset_rcu(tpos, pos, head, offset)		       \
	for (pos = rcu_dereference(hlist_first_rcu(head));		       \
	     pos != NULL &&						       \
		({ tpos = (typeof(*tpos) *)((void *)pos - offset); 1;});       \
	     pos = rcu_dereference(hlist_next_rcu(pos)))

static inline struct user_namespace *sk_user_ns(const struct sock *sk)
{
	/* Careful only use this in a context where these parameters
	 * can not change and must all be valid, such as recvmsg from
	 * userspace.
	 */
	return sk->sk_socket->file->f_cred->user_ns;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Sock flags */
enum sock_flags {
	SOCK_DEAD,
	SOCK_DONE,
	SOCK_URGINLINE,
	SOCK_KEEPOPEN,
	SOCK_LINGER,
	SOCK_DESTROY,
	SOCK_BROADCAST,
	SOCK_TIMESTAMP,
	SOCK_ZAPPED,
	SOCK_USE_WRITE_QUEUE, /* whether to call sk->sk_write_space in sock_wfree */
	SOCK_DBG, /* %SO_DEBUG setting */
	SOCK_RCVTSTAMP, /* %SO_TIMESTAMP setting */
	SOCK_RCVTSTAMPNS, /* %SO_TIMESTAMPNS setting */
	SOCK_LOCALROUTE, /* route locally only, %SO_DONTROUTE setting */
<<<<<<< HEAD
	SOCK_QUEUE_SHRUNK, /* write queue has been shrunk recently */
	SOCK_TIMESTAMPING_TX_HARDWARE,  /* %SOF_TIMESTAMPING_TX_HARDWARE */
	SOCK_TIMESTAMPING_TX_SOFTWARE,  /* %SOF_TIMESTAMPING_TX_SOFTWARE */
	SOCK_TIMESTAMPING_RX_HARDWARE,  /* %SOF_TIMESTAMPING_RX_HARDWARE */
	SOCK_TIMESTAMPING_RX_SOFTWARE,  /* %SOF_TIMESTAMPING_RX_SOFTWARE */
	SOCK_TIMESTAMPING_SOFTWARE,     /* %SOF_TIMESTAMPING_SOFTWARE */
	SOCK_TIMESTAMPING_RAW_HARDWARE, /* %SOF_TIMESTAMPING_RAW_HARDWARE */
	SOCK_TIMESTAMPING_SYS_HARDWARE, /* %SOF_TIMESTAMPING_SYS_HARDWARE */
=======
	SOCK_MEMALLOC, /* VM depends on this socket for swapping */
	SOCK_TIMESTAMPING_RX_SOFTWARE,  /* %SOF_TIMESTAMPING_RX_SOFTWARE */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	SOCK_FASYNC, /* fasync() active */
	SOCK_RXQ_OVFL,
	SOCK_ZEROCOPY, /* buffers from userspace */
	SOCK_WIFI_STATUS, /* push wifi status to userspace */
	SOCK_NOFCS, /* Tell NIC not to do the Ethernet FCS.
		     * Will use last 4 bytes of packet sent from
		     * user-space instead.
		     */
<<<<<<< HEAD
};

static inline void sock_copy_flags(struct sock *nsk, struct sock *osk)
=======
	SOCK_FILTER_LOCKED, /* Filter cannot be changed anymore */
	SOCK_SELECT_ERR_QUEUE, /* Wake select on error queue */
	SOCK_RCU_FREE, /* wait rcu grace period in sk_destruct() */
	SOCK_TXTIME,
	SOCK_XDP, /* XDP is attached */
	SOCK_TSTAMP_NEW, /* Indicates 64 bit timestamps always */
	SOCK_RCVMARK, /* Receive SO_MARK  ancillary data with packet */
};

#define SK_FLAGS_TIMESTAMP ((1UL << SOCK_TIMESTAMP) | (1UL << SOCK_TIMESTAMPING_RX_SOFTWARE))

static inline void sock_copy_flags(struct sock *nsk, const struct sock *osk)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	nsk->sk_flags = osk->sk_flags;
}

static inline void sock_set_flag(struct sock *sk, enum sock_flags flag)
{
	__set_bit(flag, &sk->sk_flags);
}

static inline void sock_reset_flag(struct sock *sk, enum sock_flags flag)
{
	__clear_bit(flag, &sk->sk_flags);
}

<<<<<<< HEAD
static inline int sock_flag(struct sock *sk, enum sock_flags flag)
=======
static inline void sock_valbool_flag(struct sock *sk, enum sock_flags bit,
				     int valbool)
{
	if (valbool)
		sock_set_flag(sk, bit);
	else
		sock_reset_flag(sk, bit);
}

static inline bool sock_flag(const struct sock *sk, enum sock_flags flag)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return test_bit(flag, &sk->sk_flags);
}

<<<<<<< HEAD
static inline void sk_acceptq_removed(struct sock *sk)
{
	sk->sk_ack_backlog--;
=======
#ifdef CONFIG_NET
DECLARE_STATIC_KEY_FALSE(memalloc_socks_key);
static inline int sk_memalloc_socks(void)
{
	return static_branch_unlikely(&memalloc_socks_key);
}

void __receive_sock(struct file *file);
#else

static inline int sk_memalloc_socks(void)
{
	return 0;
}

static inline void __receive_sock(struct file *file)
{ }
#endif

static inline gfp_t sk_gfp_mask(const struct sock *sk, gfp_t gfp_mask)
{
	return gfp_mask | (sk->sk_allocation & __GFP_MEMALLOC);
}

static inline void sk_acceptq_removed(struct sock *sk)
{
	WRITE_ONCE(sk->sk_ack_backlog, sk->sk_ack_backlog - 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void sk_acceptq_added(struct sock *sk)
{
<<<<<<< HEAD
	sk->sk_ack_backlog++;
}

static inline int sk_acceptq_is_full(struct sock *sk)
{
	return sk->sk_ack_backlog > sk->sk_max_ack_backlog;
=======
	WRITE_ONCE(sk->sk_ack_backlog, sk->sk_ack_backlog + 1);
}

/* Note: If you think the test should be:
 *	return READ_ONCE(sk->sk_ack_backlog) >= READ_ONCE(sk->sk_max_ack_backlog);
 * Then please take a look at commit 64a146513f8f ("[NET]: Revert incorrect accept queue backlog changes.")
 */
static inline bool sk_acceptq_is_full(const struct sock *sk)
{
	return READ_ONCE(sk->sk_ack_backlog) > READ_ONCE(sk->sk_max_ack_backlog);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Compute minimal free write space needed to queue new packets.
 */
<<<<<<< HEAD
static inline int sk_stream_min_wspace(struct sock *sk)
{
	return sk->sk_wmem_queued >> 1;
}

static inline int sk_stream_wspace(struct sock *sk)
{
	return sk->sk_sndbuf - sk->sk_wmem_queued;
}

extern void sk_stream_write_space(struct sock *sk);

static inline int sk_stream_memory_free(struct sock *sk)
{
	return sk->sk_wmem_queued < sk->sk_sndbuf;
}

=======
static inline int sk_stream_min_wspace(const struct sock *sk)
{
	return READ_ONCE(sk->sk_wmem_queued) >> 1;
}

static inline int sk_stream_wspace(const struct sock *sk)
{
	return READ_ONCE(sk->sk_sndbuf) - READ_ONCE(sk->sk_wmem_queued);
}

static inline void sk_wmem_queued_add(struct sock *sk, int val)
{
	WRITE_ONCE(sk->sk_wmem_queued, sk->sk_wmem_queued + val);
}

static inline void sk_forward_alloc_add(struct sock *sk, int val)
{
	/* Paired with lockless reads of sk->sk_forward_alloc */
	WRITE_ONCE(sk->sk_forward_alloc, sk->sk_forward_alloc + val);
}

void sk_stream_write_space(struct sock *sk);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* OOB backlog add */
static inline void __sk_add_backlog(struct sock *sk, struct sk_buff *skb)
{
	/* dont let skb dst not refcounted, we are going to leave rcu lock */
	skb_dst_force(skb);

	if (!sk->sk_backlog.tail)
<<<<<<< HEAD
		sk->sk_backlog.head = skb;
	else
		sk->sk_backlog.tail->next = skb;

	sk->sk_backlog.tail = skb;
=======
		WRITE_ONCE(sk->sk_backlog.head, skb);
	else
		sk->sk_backlog.tail->next = skb;

	WRITE_ONCE(sk->sk_backlog.tail, skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	skb->next = NULL;
}

/*
 * Take into account size of receive queue and backlog queue
 * Do not take into account this skb truesize,
 * to allow even a single big packet to come.
 */
<<<<<<< HEAD
static inline bool sk_rcvqueues_full(const struct sock *sk, const struct sk_buff *skb)
{
	unsigned int qsize = sk->sk_backlog.len + atomic_read(&sk->sk_rmem_alloc);

	return qsize > sk->sk_rcvbuf;
}

/* The per-socket spinlock must be held here. */
static inline __must_check int sk_add_backlog(struct sock *sk, struct sk_buff *skb)
{
	if (sk_rcvqueues_full(sk, skb))
		return -ENOBUFS;

=======
static inline bool sk_rcvqueues_full(const struct sock *sk, unsigned int limit)
{
	unsigned int qsize = sk->sk_backlog.len + atomic_read(&sk->sk_rmem_alloc);

	return qsize > limit;
}

/* The per-socket spinlock must be held here. */
static inline __must_check int sk_add_backlog(struct sock *sk, struct sk_buff *skb,
					      unsigned int limit)
{
	if (sk_rcvqueues_full(sk, limit))
		return -ENOBUFS;

	/*
	 * If the skb was allocated from pfmemalloc reserves, only
	 * allow SOCK_MEMALLOC sockets to use it as this socket is
	 * helping free memory
	 */
	if (skb_pfmemalloc(skb) && !sock_flag(sk, SOCK_MEMALLOC))
		return -ENOMEM;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	__sk_add_backlog(sk, skb);
	sk->sk_backlog.len += skb->truesize;
	return 0;
}

<<<<<<< HEAD
static inline int sk_backlog_rcv(struct sock *sk, struct sk_buff *skb)
{
	return sk->sk_backlog_rcv(sk, skb);
}

static inline void sock_rps_record_flow(const struct sock *sk)
{
#ifdef CONFIG_RPS
	struct rps_sock_flow_table *sock_flow_table;

	rcu_read_lock();
	sock_flow_table = rcu_dereference(rps_sock_flow_table);
	rps_record_sock_flow(sock_flow_table, sk->sk_rxhash);
	rcu_read_unlock();
#endif
}

static inline void sock_rps_reset_flow(const struct sock *sk)
{
#ifdef CONFIG_RPS
	struct rps_sock_flow_table *sock_flow_table;

	rcu_read_lock();
	sock_flow_table = rcu_dereference(rps_sock_flow_table);
	rps_reset_sock_flow(sock_flow_table, sk->sk_rxhash);
	rcu_read_unlock();
#endif
}
=======
int __sk_backlog_rcv(struct sock *sk, struct sk_buff *skb);

INDIRECT_CALLABLE_DECLARE(int tcp_v4_do_rcv(struct sock *sk, struct sk_buff *skb));
INDIRECT_CALLABLE_DECLARE(int tcp_v6_do_rcv(struct sock *sk, struct sk_buff *skb));

static inline int sk_backlog_rcv(struct sock *sk, struct sk_buff *skb)
{
	if (sk_memalloc_socks() && skb_pfmemalloc(skb))
		return __sk_backlog_rcv(sk, skb);

	return INDIRECT_CALL_INET(sk->sk_backlog_rcv,
				  tcp_v6_do_rcv,
				  tcp_v4_do_rcv,
				  sk, skb);
}

static inline void sk_incoming_cpu_update(struct sock *sk)
{
	int cpu = raw_smp_processor_id();

	if (unlikely(READ_ONCE(sk->sk_incoming_cpu) != cpu))
		WRITE_ONCE(sk->sk_incoming_cpu, cpu);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline void sock_rps_save_rxhash(struct sock *sk,
					const struct sk_buff *skb)
{
#ifdef CONFIG_RPS
<<<<<<< HEAD
	if (unlikely(sk->sk_rxhash != skb->rxhash)) {
		sock_rps_reset_flow(sk);
		sk->sk_rxhash = skb->rxhash;
	}
=======
	/* The following WRITE_ONCE() is paired with the READ_ONCE()
	 * here, and another one in sock_rps_record_flow().
	 */
	if (unlikely(READ_ONCE(sk->sk_rxhash) != skb->hash))
		WRITE_ONCE(sk->sk_rxhash, skb->hash);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
}

static inline void sock_rps_reset_rxhash(struct sock *sk)
{
#ifdef CONFIG_RPS
<<<<<<< HEAD
	sock_rps_reset_flow(sk);
	sk->sk_rxhash = 0;
#endif
}

#define sk_wait_event(__sk, __timeo, __condition)			\
	({	int __rc;						\
		release_sock(__sk);					\
		__rc = __condition;					\
		if (!__rc) {						\
			*(__timeo) = schedule_timeout(*(__timeo));	\
		}							\
		lock_sock(__sk);					\
		__rc = __condition;					\
		__rc;							\
	})

extern int sk_stream_wait_connect(struct sock *sk, long *timeo_p);
extern int sk_stream_wait_memory(struct sock *sk, long *timeo_p);
extern void sk_stream_wait_close(struct sock *sk, long timeo_p);
extern int sk_stream_error(struct sock *sk, int flags, int err);
extern void sk_stream_kill_queues(struct sock *sk);

extern int sk_wait_data(struct sock *sk, long *timeo);
=======
	/* Paired with READ_ONCE() in sock_rps_record_flow() */
	WRITE_ONCE(sk->sk_rxhash, 0);
#endif
}

#define sk_wait_event(__sk, __timeo, __condition, __wait)		\
	({	int __rc, __dis = __sk->sk_disconnects;			\
		release_sock(__sk);					\
		__rc = __condition;					\
		if (!__rc) {						\
			*(__timeo) = wait_woken(__wait,			\
						TASK_INTERRUPTIBLE,	\
						*(__timeo));		\
		}							\
		sched_annotate_sleep();					\
		lock_sock(__sk);					\
		__rc = __dis == __sk->sk_disconnects ? __condition : -EPIPE; \
		__rc;							\
	})

int sk_stream_wait_connect(struct sock *sk, long *timeo_p);
int sk_stream_wait_memory(struct sock *sk, long *timeo_p);
void sk_stream_wait_close(struct sock *sk, long timeo_p);
int sk_stream_error(struct sock *sk, int flags, int err);
void sk_stream_kill_queues(struct sock *sk);
void sk_set_memalloc(struct sock *sk);
void sk_clear_memalloc(struct sock *sk);

void __sk_flush_backlog(struct sock *sk);

static inline bool sk_flush_backlog(struct sock *sk)
{
	if (unlikely(READ_ONCE(sk->sk_backlog.tail))) {
		__sk_flush_backlog(sk);
		return true;
	}
	return false;
}

int sk_wait_data(struct sock *sk, long *timeo, const struct sk_buff *skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct request_sock_ops;
struct timewait_sock_ops;
struct inet_hashinfo;
struct raw_hashinfo;
<<<<<<< HEAD
struct module;

/*
 * caches using SLAB_DESTROY_BY_RCU should let .next pointer from nulls nodes
=======
struct smc_hashinfo;
struct module;
struct sk_psock;

/*
 * caches using SLAB_TYPESAFE_BY_RCU should let .next pointer from nulls nodes
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * un-modified. Special care is taken when initializing object to zero.
 */
static inline void sk_prot_clear_nulls(struct sock *sk, int size)
{
	if (offsetof(struct sock, sk_node.next) != 0)
		memset(sk, 0, offsetof(struct sock, sk_node.next));
	memset(&sk->sk_node.pprev, 0,
	       size - offsetof(struct sock, sk_node.pprev));
}

/* Networking protocol blocks we attach to sockets.
 * socket layer -> transport layer interface
<<<<<<< HEAD
 * transport -> network interface is defined by struct inet_proto
 */
struct proto {
	void			(*close)(struct sock *sk, 
					long timeout);
	int			(*connect)(struct sock *sk,
				        struct sockaddr *uaddr, 
					int addr_len);
	int			(*disconnect)(struct sock *sk, int flags);

	struct sock *		(*accept) (struct sock *sk, int flags, int *err);

	int			(*ioctl)(struct sock *sk, int cmd,
					 unsigned long arg);
	int			(*init)(struct sock *sk);
	void			(*destroy)(struct sock *sk);
	void			(*shutdown)(struct sock *sk, int how);
	int			(*setsockopt)(struct sock *sk, int level, 
					int optname, char __user *optval,
					unsigned int optlen);
	int			(*getsockopt)(struct sock *sk, int level, 
					int optname, char __user *optval, 
					int __user *option);  	 
#ifdef CONFIG_COMPAT
	int			(*compat_setsockopt)(struct sock *sk,
					int level,
					int optname, char __user *optval,
					unsigned int optlen);
	int			(*compat_getsockopt)(struct sock *sk,
					int level,
					int optname, char __user *optval,
					int __user *option);
	int			(*compat_ioctl)(struct sock *sk,
					unsigned int cmd, unsigned long arg);
#endif
	int			(*sendmsg)(struct kiocb *iocb, struct sock *sk,
					   struct msghdr *msg, size_t len);
	int			(*recvmsg)(struct kiocb *iocb, struct sock *sk,
					   struct msghdr *msg,
					size_t len, int noblock, int flags, 
					int *addr_len);
	int			(*sendpage)(struct sock *sk, struct page *page,
					int offset, size_t size, int flags);
	int			(*bind)(struct sock *sk, 
					struct sockaddr *uaddr, int addr_len);

	int			(*backlog_rcv) (struct sock *sk, 
						struct sk_buff *skb);

	/* Keeping track of sk's, looking them up, and port selection methods. */
	void			(*hash)(struct sock *sk);
	void			(*unhash)(struct sock *sk);
	void			(*rehash)(struct sock *sk);
	int			(*get_port)(struct sock *sk, unsigned short snum);
	void			(*clear_sk)(struct sock *sk, int size);
=======
 */
struct proto {
	void			(*close)(struct sock *sk,
					long timeout);
	int			(*pre_connect)(struct sock *sk,
					struct sockaddr *uaddr,
					int addr_len);
	int			(*connect)(struct sock *sk,
					struct sockaddr *uaddr,
					int addr_len);
	int			(*disconnect)(struct sock *sk, int flags);

	struct sock *		(*accept)(struct sock *sk, int flags, int *err,
					  bool kern);

	int			(*ioctl)(struct sock *sk, int cmd,
					 int *karg);
	int			(*init)(struct sock *sk);
	void			(*destroy)(struct sock *sk);
	void			(*shutdown)(struct sock *sk, int how);
	int			(*setsockopt)(struct sock *sk, int level,
					int optname, sockptr_t optval,
					unsigned int optlen);
	int			(*getsockopt)(struct sock *sk, int level,
					int optname, char __user *optval,
					int __user *option);
	void			(*keepalive)(struct sock *sk, int valbool);
#ifdef CONFIG_COMPAT
	int			(*compat_ioctl)(struct sock *sk,
					unsigned int cmd, unsigned long arg);
#endif
	int			(*sendmsg)(struct sock *sk, struct msghdr *msg,
					   size_t len);
	int			(*recvmsg)(struct sock *sk, struct msghdr *msg,
					   size_t len, int flags, int *addr_len);
	void			(*splice_eof)(struct socket *sock);
	int			(*bind)(struct sock *sk,
					struct sockaddr *addr, int addr_len);
	int			(*bind_add)(struct sock *sk,
					struct sockaddr *addr, int addr_len);

	int			(*backlog_rcv) (struct sock *sk,
						struct sk_buff *skb);
	bool			(*bpf_bypass_getsockopt)(int level,
							 int optname);

	void		(*release_cb)(struct sock *sk);

	/* Keeping track of sk's, looking them up, and port selection methods. */
	int			(*hash)(struct sock *sk);
	void			(*unhash)(struct sock *sk);
	void			(*rehash)(struct sock *sk);
	int			(*get_port)(struct sock *sk, unsigned short snum);
	void			(*put_port)(struct sock *sk);
#ifdef CONFIG_BPF_SYSCALL
	int			(*psock_update_sk_prot)(struct sock *sk,
							struct sk_psock *psock,
							bool restore);
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Keeping track of sockets in use */
#ifdef CONFIG_PROC_FS
	unsigned int		inuse_idx;
#endif

<<<<<<< HEAD
	/* Memory pressure */
	void			(*enter_memory_pressure)(struct sock *sk);
	atomic_long_t		*memory_allocated;	/* Current allocated memory. */
	struct percpu_counter	*sockets_allocated;	/* Current number of sockets. */
	/*
	 * Pressure flag: try to collapse.
	 * Technical note: it is used by multiple contexts non atomically.
	 * All the __sk_mem_schedule() is of this nature: accounting
	 * is strict, actions are advisory and have some latency.
	 */
	int			*memory_pressure;
	long			*sysctl_mem;
	int			*sysctl_wmem;
	int			*sysctl_rmem;
=======
#if IS_ENABLED(CONFIG_MPTCP)
	int			(*forward_alloc_get)(const struct sock *sk);
#endif

	bool			(*stream_memory_free)(const struct sock *sk, int wake);
	bool			(*sock_is_readable)(struct sock *sk);
	/* Memory pressure */
	void			(*enter_memory_pressure)(struct sock *sk);
	void			(*leave_memory_pressure)(struct sock *sk);
	atomic_long_t		*memory_allocated;	/* Current allocated memory. */
	int  __percpu		*per_cpu_fw_alloc;
	struct percpu_counter	*sockets_allocated;	/* Current number of sockets. */

	/*
	 * Pressure flag: try to collapse.
	 * Technical note: it is used by multiple contexts non atomically.
	 * Make sure to use READ_ONCE()/WRITE_ONCE() for all reads/writes.
	 * All the __sk_mem_schedule() is of this nature: accounting
	 * is strict, actions are advisory and have some latency.
	 */
	unsigned long		*memory_pressure;
	long			*sysctl_mem;

	int			*sysctl_wmem;
	int			*sysctl_rmem;
	u32			sysctl_wmem_offset;
	u32			sysctl_rmem_offset;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int			max_header;
	bool			no_autobind;

	struct kmem_cache	*slab;
	unsigned int		obj_size;
<<<<<<< HEAD
	int			slab_flags;

	struct percpu_counter	*orphan_count;
=======
	unsigned int		ipv6_pinfo_offset;
	slab_flags_t		slab_flags;
	unsigned int		useroffset;	/* Usercopy region offset */
	unsigned int		usersize;	/* Usercopy region size */

	unsigned int __percpu	*orphan_count;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	struct request_sock_ops	*rsk_prot;
	struct timewait_sock_ops *twsk_prot;

	union {
		struct inet_hashinfo	*hashinfo;
		struct udp_table	*udp_table;
		struct raw_hashinfo	*raw_hash;
<<<<<<< HEAD
=======
		struct smc_hashinfo	*smc_hash;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} h;

	struct module		*owner;

	char			name[32];

	struct list_head	node;
<<<<<<< HEAD
#ifdef SOCK_REFCNT_DEBUG
	atomic_t		socks;
#endif
#ifdef CONFIG_CGROUP_MEM_RES_CTLR_KMEM
	/*
	 * cgroup specific init/deinit functions. Called once for all
	 * protocols that implement it, from cgroups populate function.
	 * This function has to setup any files the protocol want to
	 * appear in the kmem cgroup filesystem.
	 */
	int			(*init_cgroup)(struct cgroup *cgrp,
					       struct cgroup_subsys *ss);
	void			(*destroy_cgroup)(struct cgroup *cgrp);
	struct cg_proto		*(*proto_cgroup)(struct mem_cgroup *memcg);
#endif
	int			(*diag_destroy)(struct sock *sk, int err);
};

struct cg_proto {
	void			(*enter_memory_pressure)(struct sock *sk);
	struct res_counter	*memory_allocated;	/* Current allocated memory. */
	struct percpu_counter	*sockets_allocated;	/* Current number of sockets. */
	int			*memory_pressure;
	long			*sysctl_mem;
	/*
	 * memcg field is used to find which memcg we belong directly
	 * Each memcg struct can hold more than one cg_proto, so container_of
	 * won't really cut.
	 *
	 * The elegant solution would be having an inverse function to
	 * proto_cgroup in struct proto, but that means polluting the structure
	 * for everybody, instead of just for memcg users.
	 */
	struct mem_cgroup	*memcg;
};

extern int proto_register(struct proto *prot, int alloc_slab);
extern void proto_unregister(struct proto *prot);

#ifdef SOCK_REFCNT_DEBUG
static inline void sk_refcnt_debug_inc(struct sock *sk)
{
	atomic_inc(&sk->sk_prot->socks);
}

static inline void sk_refcnt_debug_dec(struct sock *sk)
{
	atomic_dec(&sk->sk_prot->socks);
	printk(KERN_DEBUG "%s socket %p released, %d are still alive\n",
	       sk->sk_prot->name, sk, atomic_read(&sk->sk_prot->socks));
}

static inline void sk_refcnt_debug_release(const struct sock *sk)
{
	if (atomic_read(&sk->sk_refcnt) != 1)
		printk(KERN_DEBUG "Destruction of the %s socket %p delayed, refcnt=%d\n",
		       sk->sk_prot->name, sk, atomic_read(&sk->sk_refcnt));
}
#else /* SOCK_REFCNT_DEBUG */
#define sk_refcnt_debug_inc(sk) do { } while (0)
#define sk_refcnt_debug_dec(sk) do { } while (0)
#define sk_refcnt_debug_release(sk) do { } while (0)
#endif /* SOCK_REFCNT_DEBUG */

#if defined(CONFIG_CGROUP_MEM_RES_CTLR_KMEM) && defined(CONFIG_NET)
extern struct static_key memcg_socket_limit_enabled;
static inline struct cg_proto *parent_cg_proto(struct proto *proto,
					       struct cg_proto *cg_proto)
{
	return proto->proto_cgroup(parent_mem_cgroup(cg_proto->memcg));
}
#define mem_cgroup_sockets_enabled static_key_false(&memcg_socket_limit_enabled)
#else
#define mem_cgroup_sockets_enabled 0
static inline struct cg_proto *parent_cg_proto(struct proto *proto,
					       struct cg_proto *cg_proto)
{
	return NULL;
}
#endif

=======
	int			(*diag_destroy)(struct sock *sk, int err);
} __randomize_layout;

int proto_register(struct proto *prot, int alloc_slab);
void proto_unregister(struct proto *prot);
int sock_load_diag_module(int family, int protocol);

INDIRECT_CALLABLE_DECLARE(bool tcp_stream_memory_free(const struct sock *sk, int wake));

static inline int sk_forward_alloc_get(const struct sock *sk)
{
#if IS_ENABLED(CONFIG_MPTCP)
	if (sk->sk_prot->forward_alloc_get)
		return sk->sk_prot->forward_alloc_get(sk);
#endif
	return READ_ONCE(sk->sk_forward_alloc);
}

static inline bool __sk_stream_memory_free(const struct sock *sk, int wake)
{
	if (READ_ONCE(sk->sk_wmem_queued) >= READ_ONCE(sk->sk_sndbuf))
		return false;

	return sk->sk_prot->stream_memory_free ?
		INDIRECT_CALL_INET_1(sk->sk_prot->stream_memory_free,
				     tcp_stream_memory_free, sk, wake) : true;
}

static inline bool sk_stream_memory_free(const struct sock *sk)
{
	return __sk_stream_memory_free(sk, 0);
}

static inline bool __sk_stream_is_writeable(const struct sock *sk, int wake)
{
	return sk_stream_wspace(sk) >= sk_stream_min_wspace(sk) &&
	       __sk_stream_memory_free(sk, wake);
}

static inline bool sk_stream_is_writeable(const struct sock *sk)
{
	return __sk_stream_is_writeable(sk, 0);
}

static inline int sk_under_cgroup_hierarchy(struct sock *sk,
					    struct cgroup *ancestor)
{
#ifdef CONFIG_SOCK_CGROUP_DATA
	return cgroup_is_descendant(sock_cgroup_ptr(&sk->sk_cgrp_data),
				    ancestor);
#else
	return -ENOTSUPP;
#endif
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline bool sk_has_memory_pressure(const struct sock *sk)
{
	return sk->sk_prot->memory_pressure != NULL;
}

<<<<<<< HEAD
=======
static inline bool sk_under_global_memory_pressure(const struct sock *sk)
{
	return sk->sk_prot->memory_pressure &&
		!!READ_ONCE(*sk->sk_prot->memory_pressure);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline bool sk_under_memory_pressure(const struct sock *sk)
{
	if (!sk->sk_prot->memory_pressure)
		return false;

<<<<<<< HEAD
	if (mem_cgroup_sockets_enabled && sk->sk_cgrp)
		return !!*sk->sk_cgrp->memory_pressure;

	return !!*sk->sk_prot->memory_pressure;
}

static inline void sk_leave_memory_pressure(struct sock *sk)
{
	int *memory_pressure = sk->sk_prot->memory_pressure;

	if (!memory_pressure)
		return;

	if (*memory_pressure)
		*memory_pressure = 0;

	if (mem_cgroup_sockets_enabled && sk->sk_cgrp) {
		struct cg_proto *cg_proto = sk->sk_cgrp;
		struct proto *prot = sk->sk_prot;

		for (; cg_proto; cg_proto = parent_cg_proto(prot, cg_proto))
			if (*cg_proto->memory_pressure)
				*cg_proto->memory_pressure = 0;
	}

}

static inline void sk_enter_memory_pressure(struct sock *sk)
{
	if (!sk->sk_prot->enter_memory_pressure)
		return;

	if (mem_cgroup_sockets_enabled && sk->sk_cgrp) {
		struct cg_proto *cg_proto = sk->sk_cgrp;
		struct proto *prot = sk->sk_prot;

		for (; cg_proto; cg_proto = parent_cg_proto(prot, cg_proto))
			cg_proto->enter_memory_pressure(sk);
	}

	sk->sk_prot->enter_memory_pressure(sk);
}

static inline long sk_prot_mem_limits(const struct sock *sk, int index)
{
	long *prot = sk->sk_prot->sysctl_mem;
	if (mem_cgroup_sockets_enabled && sk->sk_cgrp)
		prot = sk->sk_cgrp->sysctl_mem;
	return prot[index];
}

static inline void memcg_memory_allocated_add(struct cg_proto *prot,
					      unsigned long amt,
					      int *parent_status)
{
	struct res_counter *fail;
	int ret;

	ret = res_counter_charge_nofail(prot->memory_allocated,
					amt << PAGE_SHIFT, &fail);
	if (ret < 0)
		*parent_status = OVER_LIMIT;
}

static inline void memcg_memory_allocated_sub(struct cg_proto *prot,
					      unsigned long amt)
{
	res_counter_uncharge(prot->memory_allocated, amt << PAGE_SHIFT);
}

static inline u64 memcg_memory_allocated_read(struct cg_proto *prot)
{
	u64 ret;
	ret = res_counter_read_u64(prot->memory_allocated, RES_USAGE);
	return ret >> PAGE_SHIFT;
=======
	if (mem_cgroup_sockets_enabled && sk->sk_memcg &&
	    mem_cgroup_under_socket_pressure(sk->sk_memcg))
		return true;

	return !!READ_ONCE(*sk->sk_prot->memory_pressure);
}

static inline long
proto_memory_allocated(const struct proto *prot)
{
	return max(0L, atomic_long_read(prot->memory_allocated));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline long
sk_memory_allocated(const struct sock *sk)
{
<<<<<<< HEAD
	struct proto *prot = sk->sk_prot;
	if (mem_cgroup_sockets_enabled && sk->sk_cgrp)
		return memcg_memory_allocated_read(sk->sk_cgrp);

	return atomic_long_read(prot->memory_allocated);
}

static inline long
sk_memory_allocated_add(struct sock *sk, int amt, int *parent_status)
{
	struct proto *prot = sk->sk_prot;

	if (mem_cgroup_sockets_enabled && sk->sk_cgrp) {
		memcg_memory_allocated_add(sk->sk_cgrp, amt, parent_status);
		/* update the root cgroup regardless */
		atomic_long_add_return(amt, prot->memory_allocated);
		return memcg_memory_allocated_read(sk->sk_cgrp);
	}

	return atomic_long_add_return(amt, prot->memory_allocated);
}

static inline void
sk_memory_allocated_sub(struct sock *sk, int amt)
{
	struct proto *prot = sk->sk_prot;

	if (mem_cgroup_sockets_enabled && sk->sk_cgrp)
		memcg_memory_allocated_sub(sk->sk_cgrp, amt);

	atomic_long_sub(amt, prot->memory_allocated);
}

static inline void sk_sockets_allocated_dec(struct sock *sk)
{
	struct proto *prot = sk->sk_prot;

	if (mem_cgroup_sockets_enabled && sk->sk_cgrp) {
		struct cg_proto *cg_proto = sk->sk_cgrp;

		for (; cg_proto; cg_proto = parent_cg_proto(prot, cg_proto))
			percpu_counter_dec(cg_proto->sockets_allocated);
	}

	percpu_counter_dec(prot->sockets_allocated);
=======
	return proto_memory_allocated(sk->sk_prot);
}

/* 1 MB per cpu, in page units */
#define SK_MEMORY_PCPU_RESERVE (1 << (20 - PAGE_SHIFT))
extern int sysctl_mem_pcpu_rsv;

static inline void proto_memory_pcpu_drain(struct proto *proto)
{
	int val = this_cpu_xchg(*proto->per_cpu_fw_alloc, 0);

	if (val)
		atomic_long_add(val, proto->memory_allocated);
}

static inline void
sk_memory_allocated_add(const struct sock *sk, int val)
{
	struct proto *proto = sk->sk_prot;

	val = this_cpu_add_return(*proto->per_cpu_fw_alloc, val);

	if (unlikely(val >= READ_ONCE(sysctl_mem_pcpu_rsv)))
		proto_memory_pcpu_drain(proto);
}

static inline void
sk_memory_allocated_sub(const struct sock *sk, int val)
{
	struct proto *proto = sk->sk_prot;

	val = this_cpu_sub_return(*proto->per_cpu_fw_alloc, val);

	if (unlikely(val <= -READ_ONCE(sysctl_mem_pcpu_rsv)))
		proto_memory_pcpu_drain(proto);
}

#define SK_ALLOC_PERCPU_COUNTER_BATCH 16

static inline void sk_sockets_allocated_dec(struct sock *sk)
{
	percpu_counter_add_batch(sk->sk_prot->sockets_allocated, -1,
				 SK_ALLOC_PERCPU_COUNTER_BATCH);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void sk_sockets_allocated_inc(struct sock *sk)
{
<<<<<<< HEAD
	struct proto *prot = sk->sk_prot;

	if (mem_cgroup_sockets_enabled && sk->sk_cgrp) {
		struct cg_proto *cg_proto = sk->sk_cgrp;

		for (; cg_proto; cg_proto = parent_cg_proto(prot, cg_proto))
			percpu_counter_inc(cg_proto->sockets_allocated);
	}

	percpu_counter_inc(prot->sockets_allocated);
}

static inline int
sk_sockets_allocated_read_positive(struct sock *sk)
{
	struct proto *prot = sk->sk_prot;

	if (mem_cgroup_sockets_enabled && sk->sk_cgrp)
		return percpu_counter_read_positive(sk->sk_cgrp->sockets_allocated);

	return percpu_counter_read_positive(prot->sockets_allocated);
=======
	percpu_counter_add_batch(sk->sk_prot->sockets_allocated, 1,
				 SK_ALLOC_PERCPU_COUNTER_BATCH);
}

static inline u64
sk_sockets_allocated_read_positive(struct sock *sk)
{
	return percpu_counter_read_positive(sk->sk_prot->sockets_allocated);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline int
proto_sockets_allocated_sum_positive(struct proto *prot)
{
	return percpu_counter_sum_positive(prot->sockets_allocated);
}

<<<<<<< HEAD
static inline long
proto_memory_allocated(struct proto *prot)
{
	return atomic_long_read(prot->memory_allocated);
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline bool
proto_memory_pressure(struct proto *prot)
{
	if (!prot->memory_pressure)
		return false;
<<<<<<< HEAD
	return !!*prot->memory_pressure;
=======
	return !!READ_ONCE(*prot->memory_pressure);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}


#ifdef CONFIG_PROC_FS
<<<<<<< HEAD
/* Called with local bh disabled */
extern void sock_prot_inuse_add(struct net *net, struct proto *prot, int inc);
extern int sock_prot_inuse_get(struct net *net, struct proto *proto);
#else
static void inline sock_prot_inuse_add(struct net *net, struct proto *prot,
		int inc)
=======
#define PROTO_INUSE_NR	64	/* should be enough for the first time */
struct prot_inuse {
	int all;
	int val[PROTO_INUSE_NR];
};

static inline void sock_prot_inuse_add(const struct net *net,
				       const struct proto *prot, int val)
{
	this_cpu_add(net->core.prot_inuse->val[prot->inuse_idx], val);
}

static inline void sock_inuse_add(const struct net *net, int val)
{
	this_cpu_add(net->core.prot_inuse->all, val);
}

int sock_prot_inuse_get(struct net *net, struct proto *proto);
int sock_inuse_get(struct net *net);
#else
static inline void sock_prot_inuse_add(const struct net *net,
				       const struct proto *prot, int val)
{
}

static inline void sock_inuse_add(const struct net *net, int val)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
}
#endif


/* With per-bucket locks this operation is not-atomic, so that
 * this version is not worse.
 */
<<<<<<< HEAD
static inline void __sk_prot_rehash(struct sock *sk)
{
	sk->sk_prot->unhash(sk);
	sk->sk_prot->hash(sk);
}

void sk_prot_clear_portaddr_nulls(struct sock *sk, int size);

=======
static inline int __sk_prot_rehash(struct sock *sk)
{
	sk->sk_prot->unhash(sk);
	return sk->sk_prot->hash(sk);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* About 10 seconds */
#define SOCK_DESTROY_TIME (10*HZ)

/* Sockets 0-1023 can't be bound to unless you are superuser */
#define PROT_SOCK	1024

#define SHUTDOWN_MASK	3
#define RCV_SHUTDOWN	1
#define SEND_SHUTDOWN	2

<<<<<<< HEAD
#define SOCK_SNDBUF_LOCK	1
#define SOCK_RCVBUF_LOCK	2
#define SOCK_BINDADDR_LOCK	4
#define SOCK_BINDPORT_LOCK	8

/* sock_iocb: used to kick off async processing of socket ios */
struct sock_iocb {
	struct list_head	list;

	int			flags;
	int			size;
	struct socket		*sock;
	struct sock		*sk;
	struct scm_cookie	*scm;
	struct msghdr		*msg, async_msg;
	struct kiocb		*kiocb;
};

static inline struct sock_iocb *kiocb_to_siocb(struct kiocb *iocb)
{
	return (struct sock_iocb *)iocb->private;
}

static inline struct kiocb *siocb_to_kiocb(struct sock_iocb *si)
{
	return si->kiocb;
}

=======
#define SOCK_BINDADDR_LOCK	4
#define SOCK_BINDPORT_LOCK	8

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct socket_alloc {
	struct socket socket;
	struct inode vfs_inode;
};

static inline struct socket *SOCKET_I(struct inode *inode)
{
	return &container_of(inode, struct socket_alloc, vfs_inode)->socket;
}

static inline struct inode *SOCK_INODE(struct socket *socket)
{
	return &container_of(socket, struct socket_alloc, socket)->vfs_inode;
}

/*
 * Functions for memory accounting
 */
<<<<<<< HEAD
extern int __sk_mem_schedule(struct sock *sk, int size, int kind);
extern void __sk_mem_reclaim(struct sock *sk);

#define SK_MEM_QUANTUM ((int)PAGE_SIZE)
#define SK_MEM_QUANTUM_SHIFT ilog2(SK_MEM_QUANTUM)
#define SK_MEM_SEND	0
#define SK_MEM_RECV	1

static inline int sk_mem_pages(int amt)
{
	return (amt + SK_MEM_QUANTUM - 1) >> SK_MEM_QUANTUM_SHIFT;
}

static inline int sk_has_account(struct sock *sk)
=======
int __sk_mem_raise_allocated(struct sock *sk, int size, int amt, int kind);
int __sk_mem_schedule(struct sock *sk, int size, int kind);
void __sk_mem_reduce_allocated(struct sock *sk, int amount);
void __sk_mem_reclaim(struct sock *sk, int amount);

#define SK_MEM_SEND	0
#define SK_MEM_RECV	1

/* sysctl_mem values are in pages */
static inline long sk_prot_mem_limits(const struct sock *sk, int index)
{
	return READ_ONCE(sk->sk_prot->sysctl_mem[index]);
}

static inline int sk_mem_pages(int amt)
{
	return (amt + PAGE_SIZE - 1) >> PAGE_SHIFT;
}

static inline bool sk_has_account(struct sock *sk)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	/* return true if protocol supports memory accounting */
	return !!sk->sk_prot->memory_allocated;
}

<<<<<<< HEAD
static inline int sk_wmem_schedule(struct sock *sk, int size)
{
	if (!sk_has_account(sk))
		return 1;
	return size <= sk->sk_forward_alloc ||
		__sk_mem_schedule(sk, size, SK_MEM_SEND);
}

static inline int sk_rmem_schedule(struct sock *sk, int size)
{
	if (!sk_has_account(sk))
		return 1;
	return size <= sk->sk_forward_alloc ||
		__sk_mem_schedule(sk, size, SK_MEM_RECV);
=======
static inline bool sk_wmem_schedule(struct sock *sk, int size)
{
	int delta;

	if (!sk_has_account(sk))
		return true;
	delta = size - sk->sk_forward_alloc;
	return delta <= 0 || __sk_mem_schedule(sk, delta, SK_MEM_SEND);
}

static inline bool
sk_rmem_schedule(struct sock *sk, struct sk_buff *skb, int size)
{
	int delta;

	if (!sk_has_account(sk))
		return true;
	delta = size - sk->sk_forward_alloc;
	return delta <= 0 || __sk_mem_schedule(sk, delta, SK_MEM_RECV) ||
		skb_pfmemalloc(skb);
}

static inline int sk_unused_reserved_mem(const struct sock *sk)
{
	int unused_mem;

	if (likely(!sk->sk_reserved_mem))
		return 0;

	unused_mem = sk->sk_reserved_mem - sk->sk_wmem_queued -
			atomic_read(&sk->sk_rmem_alloc);

	return unused_mem > 0 ? unused_mem : 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void sk_mem_reclaim(struct sock *sk)
{
<<<<<<< HEAD
	if (!sk_has_account(sk))
		return;
	if (sk->sk_forward_alloc >= SK_MEM_QUANTUM)
		__sk_mem_reclaim(sk);
}

static inline void sk_mem_reclaim_partial(struct sock *sk)
{
	if (!sk_has_account(sk))
		return;
	if (sk->sk_forward_alloc > SK_MEM_QUANTUM)
		__sk_mem_reclaim(sk);
=======
	int reclaimable;

	if (!sk_has_account(sk))
		return;

	reclaimable = sk->sk_forward_alloc - sk_unused_reserved_mem(sk);

	if (reclaimable >= (int)PAGE_SIZE)
		__sk_mem_reclaim(sk, reclaimable);
}

static inline void sk_mem_reclaim_final(struct sock *sk)
{
	sk->sk_reserved_mem = 0;
	sk_mem_reclaim(sk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void sk_mem_charge(struct sock *sk, int size)
{
	if (!sk_has_account(sk))
		return;
<<<<<<< HEAD
	sk->sk_forward_alloc -= size;
=======
	sk_forward_alloc_add(sk, -size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void sk_mem_uncharge(struct sock *sk, int size)
{
	if (!sk_has_account(sk))
		return;
<<<<<<< HEAD
	sk->sk_forward_alloc += size;
}

static inline void sk_wmem_free_skb(struct sock *sk, struct sk_buff *skb)
{
	sock_set_flag(sk, SOCK_QUEUE_SHRUNK);
	sk->sk_wmem_queued -= skb->truesize;
	sk_mem_uncharge(sk, skb->truesize);
	__kfree_skb(skb);
}

=======
	sk_forward_alloc_add(sk, size);
	sk_mem_reclaim(sk);
}

/*
 * Macro so as to not evaluate some arguments when
 * lockdep is not enabled.
 *
 * Mark both the sk_lock and the sk_lock.slock as a
 * per-address-family lock class.
 */
#define sock_lock_init_class_and_name(sk, sname, skey, name, key)	\
do {									\
	sk->sk_lock.owned = 0;						\
	init_waitqueue_head(&sk->sk_lock.wq);				\
	spin_lock_init(&(sk)->sk_lock.slock);				\
	debug_check_no_locks_freed((void *)&(sk)->sk_lock,		\
			sizeof((sk)->sk_lock));				\
	lockdep_set_class_and_name(&(sk)->sk_lock.slock,		\
				(skey), (sname));				\
	lockdep_init_map(&(sk)->sk_lock.dep_map, (name), (key), 0);	\
} while (0)

static inline bool lockdep_sock_is_held(const struct sock *sk)
{
	return lockdep_is_held(&sk->sk_lock) ||
	       lockdep_is_held(&sk->sk_lock.slock);
}

void lock_sock_nested(struct sock *sk, int subclass);

static inline void lock_sock(struct sock *sk)
{
	lock_sock_nested(sk, 0);
}

void __lock_sock(struct sock *sk);
void __release_sock(struct sock *sk);
void release_sock(struct sock *sk);

/* BH context may only use the following locking interface. */
#define bh_lock_sock(__sk)	spin_lock(&((__sk)->sk_lock.slock))
#define bh_lock_sock_nested(__sk) \
				spin_lock_nested(&((__sk)->sk_lock.slock), \
				SINGLE_DEPTH_NESTING)
#define bh_unlock_sock(__sk)	spin_unlock(&((__sk)->sk_lock.slock))

bool __lock_sock_fast(struct sock *sk) __acquires(&sk->sk_lock.slock);

/**
 * lock_sock_fast - fast version of lock_sock
 * @sk: socket
 *
 * This version should be used for very small section, where process wont block
 * return false if fast path is taken:
 *
 *   sk_lock.slock locked, owned = 0, BH disabled
 *
 * return true if slow path is taken:
 *
 *   sk_lock.slock unlocked, owned = 1, BH enabled
 */
static inline bool lock_sock_fast(struct sock *sk)
{
	/* The sk_lock has mutex_lock() semantics here. */
	mutex_acquire(&sk->sk_lock.dep_map, 0, 0, _RET_IP_);

	return __lock_sock_fast(sk);
}

/* fast socket lock variant for caller already holding a [different] socket lock */
static inline bool lock_sock_fast_nested(struct sock *sk)
{
	mutex_acquire(&sk->sk_lock.dep_map, SINGLE_DEPTH_NESTING, 0, _RET_IP_);

	return __lock_sock_fast(sk);
}

/**
 * unlock_sock_fast - complement of lock_sock_fast
 * @sk: socket
 * @slow: slow mode
 *
 * fast unlock socket for user context.
 * If slow mode is on, we call regular release_sock()
 */
static inline void unlock_sock_fast(struct sock *sk, bool slow)
	__releases(&sk->sk_lock.slock)
{
	if (slow) {
		release_sock(sk);
		__release(&sk->sk_lock.slock);
	} else {
		mutex_release(&sk->sk_lock.dep_map, _RET_IP_);
		spin_unlock_bh(&sk->sk_lock.slock);
	}
}

void sockopt_lock_sock(struct sock *sk);
void sockopt_release_sock(struct sock *sk);
bool sockopt_ns_capable(struct user_namespace *ns, int cap);
bool sockopt_capable(int cap);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Used by processes to "lock" a socket state, so that
 * interrupts and bottom half handlers won't change it
 * from under us. It essentially blocks any incoming
 * packets, so that we won't get any new data or any
 * packets that change the state of the socket.
 *
 * While locked, BH processing will add new packets to
 * the backlog queue.  This queue is processed by the
 * owner of the socket lock right before it is released.
 *
 * Since ~2.3.5 it is also exclusive sleep lock serializing
 * accesses from user process context.
 */
<<<<<<< HEAD
#define sock_owned_by_user(sk)	((sk)->sk_lock.owned)

/*
 * Macro so as to not evaluate some arguments when
 * lockdep is not enabled.
 *
 * Mark both the sk_lock and the sk_lock.slock as a
 * per-address-family lock class.
 */
#define sock_lock_init_class_and_name(sk, sname, skey, name, key) 	\
do {									\
	sk->sk_lock.owned = 0;						\
	init_waitqueue_head(&sk->sk_lock.wq);				\
	spin_lock_init(&(sk)->sk_lock.slock);				\
	debug_check_no_locks_freed((void *)&(sk)->sk_lock,		\
			sizeof((sk)->sk_lock));				\
	lockdep_set_class_and_name(&(sk)->sk_lock.slock,		\
		       	(skey), (sname));				\
	lockdep_init_map(&(sk)->sk_lock.dep_map, (name), (key), 0);	\
} while (0)

extern void lock_sock_nested(struct sock *sk, int subclass);

static inline void lock_sock(struct sock *sk)
{
	lock_sock_nested(sk, 0);
}

extern void release_sock(struct sock *sk);

/* BH context may only use the following locking interface. */
#define bh_lock_sock(__sk)	spin_lock(&((__sk)->sk_lock.slock))
#define bh_lock_sock_nested(__sk) \
				spin_lock_nested(&((__sk)->sk_lock.slock), \
				SINGLE_DEPTH_NESTING)
#define bh_unlock_sock(__sk)	spin_unlock(&((__sk)->sk_lock.slock))

extern bool lock_sock_fast(struct sock *sk);
/**
 * unlock_sock_fast - complement of lock_sock_fast
 * @sk: socket
 * @slow: slow mode
 *
 * fast unlock socket for user context.
 * If slow mode is on, we call regular release_sock()
 */
static inline void unlock_sock_fast(struct sock *sk, bool slow)
{
	if (slow)
		release_sock(sk);
	else
		spin_unlock_bh(&sk->sk_lock.slock);
}


extern struct sock		*sk_alloc(struct net *net, int family,
					  gfp_t priority,
					  struct proto *prot);
extern void			sk_free(struct sock *sk);
extern void			sk_release_kernel(struct sock *sk);
extern struct sock		*sk_clone_lock(const struct sock *sk,
					       const gfp_t priority);

extern struct sk_buff		*sock_wmalloc(struct sock *sk,
					      unsigned long size, int force,
					      gfp_t priority);
extern struct sk_buff		*sock_rmalloc(struct sock *sk,
					      unsigned long size, int force,
					      gfp_t priority);
extern void			sock_wfree(struct sk_buff *skb);
extern void			sock_rfree(struct sk_buff *skb);

extern int			sock_setsockopt(struct socket *sock, int level,
						int op, char __user *optval,
						unsigned int optlen);

extern int			sock_getsockopt(struct socket *sock, int level,
						int op, char __user *optval, 
						int __user *optlen);
extern struct sk_buff 		*sock_alloc_send_skb(struct sock *sk,
						     unsigned long size,
						     int noblock,
						     int *errcode);
extern struct sk_buff 		*sock_alloc_send_pskb(struct sock *sk,
						      unsigned long header_len,
						      unsigned long data_len,
						      int noblock,
						      int *errcode);
extern void *sock_kmalloc(struct sock *sk, int size,
			  gfp_t priority);
extern void sock_kfree_s(struct sock *sk, void *mem, int size);
extern void sk_send_sigurg(struct sock *sk);

#ifdef CONFIG_CGROUPS
extern void sock_update_classid(struct sock *sk);
#else
static inline void sock_update_classid(struct sock *sk)
{
}
#endif
=======

static inline void sock_owned_by_me(const struct sock *sk)
{
#ifdef CONFIG_LOCKDEP
	WARN_ON_ONCE(!lockdep_sock_is_held(sk) && debug_locks);
#endif
}

static inline void sock_not_owned_by_me(const struct sock *sk)
{
#ifdef CONFIG_LOCKDEP
	WARN_ON_ONCE(lockdep_sock_is_held(sk) && debug_locks);
#endif
}

static inline bool sock_owned_by_user(const struct sock *sk)
{
	sock_owned_by_me(sk);
	return sk->sk_lock.owned;
}

static inline bool sock_owned_by_user_nocheck(const struct sock *sk)
{
	return sk->sk_lock.owned;
}

static inline void sock_release_ownership(struct sock *sk)
{
	DEBUG_NET_WARN_ON_ONCE(!sock_owned_by_user_nocheck(sk));
	sk->sk_lock.owned = 0;

	/* The sk_lock has mutex_unlock() semantics: */
	mutex_release(&sk->sk_lock.dep_map, _RET_IP_);
}

/* no reclassification while locks are held */
static inline bool sock_allow_reclassification(const struct sock *csk)
{
	struct sock *sk = (struct sock *)csk;

	return !sock_owned_by_user_nocheck(sk) &&
		!spin_is_locked(&sk->sk_lock.slock);
}

struct sock *sk_alloc(struct net *net, int family, gfp_t priority,
		      struct proto *prot, int kern);
void sk_free(struct sock *sk);
void sk_destruct(struct sock *sk);
struct sock *sk_clone_lock(const struct sock *sk, const gfp_t priority);
void sk_free_unlock_clone(struct sock *sk);

struct sk_buff *sock_wmalloc(struct sock *sk, unsigned long size, int force,
			     gfp_t priority);
void __sock_wfree(struct sk_buff *skb);
void sock_wfree(struct sk_buff *skb);
struct sk_buff *sock_omalloc(struct sock *sk, unsigned long size,
			     gfp_t priority);
void skb_orphan_partial(struct sk_buff *skb);
void sock_rfree(struct sk_buff *skb);
void sock_efree(struct sk_buff *skb);
#ifdef CONFIG_INET
void sock_edemux(struct sk_buff *skb);
void sock_pfree(struct sk_buff *skb);
#else
#define sock_edemux sock_efree
#endif

int sk_setsockopt(struct sock *sk, int level, int optname,
		  sockptr_t optval, unsigned int optlen);
int sock_setsockopt(struct socket *sock, int level, int op,
		    sockptr_t optval, unsigned int optlen);
int do_sock_setsockopt(struct socket *sock, bool compat, int level,
		       int optname, sockptr_t optval, int optlen);
int do_sock_getsockopt(struct socket *sock, bool compat, int level,
		       int optname, sockptr_t optval, sockptr_t optlen);

int sk_getsockopt(struct sock *sk, int level, int optname,
		  sockptr_t optval, sockptr_t optlen);
int sock_gettstamp(struct socket *sock, void __user *userstamp,
		   bool timeval, bool time32);
struct sk_buff *sock_alloc_send_pskb(struct sock *sk, unsigned long header_len,
				     unsigned long data_len, int noblock,
				     int *errcode, int max_page_order);

static inline struct sk_buff *sock_alloc_send_skb(struct sock *sk,
						  unsigned long size,
						  int noblock, int *errcode)
{
	return sock_alloc_send_pskb(sk, size, 0, noblock, errcode, 0);
}

void *sock_kmalloc(struct sock *sk, int size, gfp_t priority);
void sock_kfree_s(struct sock *sk, void *mem, int size);
void sock_kzfree_s(struct sock *sk, void *mem, int size);
void sk_send_sigurg(struct sock *sk);

static inline void sock_replace_proto(struct sock *sk, struct proto *proto)
{
	if (sk->sk_socket)
		clear_bit(SOCK_SUPPORT_ZC, &sk->sk_socket->flags);
	WRITE_ONCE(sk->sk_prot, proto);
}

struct sockcm_cookie {
	u64 transmit_time;
	u32 mark;
	u32 tsflags;
};

static inline void sockcm_init(struct sockcm_cookie *sockc,
			       const struct sock *sk)
{
	*sockc = (struct sockcm_cookie) {
		.tsflags = READ_ONCE(sk->sk_tsflags)
	};
}

int __sock_cmsg_send(struct sock *sk, struct cmsghdr *cmsg,
		     struct sockcm_cookie *sockc);
int sock_cmsg_send(struct sock *sk, struct msghdr *msg,
		   struct sockcm_cookie *sockc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Functions to fill in entries in struct proto_ops when a protocol
 * does not implement a particular function.
 */
<<<<<<< HEAD
extern int                      sock_no_bind(struct socket *, 
					     struct sockaddr *, int);
extern int                      sock_no_connect(struct socket *,
						struct sockaddr *, int, int);
extern int                      sock_no_socketpair(struct socket *,
						   struct socket *);
extern int                      sock_no_accept(struct socket *,
					       struct socket *, int);
extern int                      sock_no_getname(struct socket *,
						struct sockaddr *, int *, int);
extern unsigned int             sock_no_poll(struct file *, struct socket *,
					     struct poll_table_struct *);
extern int                      sock_no_ioctl(struct socket *, unsigned int,
					      unsigned long);
extern int			sock_no_listen(struct socket *, int);
extern int                      sock_no_shutdown(struct socket *, int);
extern int			sock_no_getsockopt(struct socket *, int , int,
						   char __user *, int __user *);
extern int			sock_no_setsockopt(struct socket *, int, int,
						   char __user *, unsigned int);
extern int                      sock_no_sendmsg(struct kiocb *, struct socket *,
						struct msghdr *, size_t);
extern int                      sock_no_recvmsg(struct kiocb *, struct socket *,
						struct msghdr *, size_t, int);
extern int			sock_no_mmap(struct file *file,
					     struct socket *sock,
					     struct vm_area_struct *vma);
extern ssize_t			sock_no_sendpage(struct socket *sock,
						struct page *page,
						int offset, size_t size, 
						int flags);
=======
int sock_no_bind(struct socket *, struct sockaddr *, int);
int sock_no_connect(struct socket *, struct sockaddr *, int, int);
int sock_no_socketpair(struct socket *, struct socket *);
int sock_no_accept(struct socket *, struct socket *, int, bool);
int sock_no_getname(struct socket *, struct sockaddr *, int);
int sock_no_ioctl(struct socket *, unsigned int, unsigned long);
int sock_no_listen(struct socket *, int);
int sock_no_shutdown(struct socket *, int);
int sock_no_sendmsg(struct socket *, struct msghdr *, size_t);
int sock_no_sendmsg_locked(struct sock *sk, struct msghdr *msg, size_t len);
int sock_no_recvmsg(struct socket *, struct msghdr *, size_t, int);
int sock_no_mmap(struct file *file, struct socket *sock,
		 struct vm_area_struct *vma);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Functions to fill in entries in struct proto_ops when a protocol
 * uses the inet style.
 */
<<<<<<< HEAD
extern int sock_common_getsockopt(struct socket *sock, int level, int optname,
				  char __user *optval, int __user *optlen);
extern int sock_common_recvmsg(struct kiocb *iocb, struct socket *sock,
			       struct msghdr *msg, size_t size, int flags);
extern int sock_common_setsockopt(struct socket *sock, int level, int optname,
				  char __user *optval, unsigned int optlen);
extern int compat_sock_common_getsockopt(struct socket *sock, int level,
		int optname, char __user *optval, int __user *optlen);
extern int compat_sock_common_setsockopt(struct socket *sock, int level,
		int optname, char __user *optval, unsigned int optlen);

extern void sk_common_release(struct sock *sk);
=======
int sock_common_getsockopt(struct socket *sock, int level, int optname,
				  char __user *optval, int __user *optlen);
int sock_common_recvmsg(struct socket *sock, struct msghdr *msg, size_t size,
			int flags);
int sock_common_setsockopt(struct socket *sock, int level, int optname,
			   sockptr_t optval, unsigned int optlen);

void sk_common_release(struct sock *sk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 *	Default socket callbacks and setup code
 */
<<<<<<< HEAD
 
/* Initialise core socket variables */
extern void sock_init_data(struct socket *sock, struct sock *sk);

extern void sk_filter_release_rcu(struct rcu_head *rcu);

/**
 *	sk_filter_release - release a socket filter
 *	@fp: filter to remove
 *
 *	Remove a filter from a socket and release its resources.
 */

static inline void sk_filter_release(struct sk_filter *fp)
{
	if (atomic_dec_and_test(&fp->refcnt))
		call_rcu(&fp->rcu, sk_filter_release_rcu);
}

static inline void sk_filter_uncharge(struct sock *sk, struct sk_filter *fp)
{
	unsigned int size = sk_filter_len(fp);

	atomic_sub(size, &sk->sk_omem_alloc);
	sk_filter_release(fp);
}

static inline void sk_filter_charge(struct sock *sk, struct sk_filter *fp)
{
	atomic_inc(&fp->refcnt);
	atomic_add(sk_filter_len(fp), &sk->sk_omem_alloc);
}
=======

/* Initialise core socket variables using an explicit uid. */
void sock_init_data_uid(struct socket *sock, struct sock *sk, kuid_t uid);

/* Initialise core socket variables.
 * Assumes struct socket *sock is embedded in a struct socket_alloc.
 */
void sock_init_data(struct socket *sock, struct sock *sk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Socket reference counting postulates.
 *
 * * Each user of socket SHOULD hold a reference count.
 * * Each access point to socket (an hash table bucket, reference from a list,
 *   running timer, skb in flight MUST hold a reference count.
 * * When reference count hits 0, it means it will never increase back.
 * * When reference count hits 0, it means that no references from
 *   outside exist to this socket and current process on current CPU
 *   is last user and may/should destroy this socket.
 * * sk_free is called from any context: process, BH, IRQ. When
 *   it is called, socket has no references from outside -> sk_free
 *   may release descendant resources allocated by the socket, but
 *   to the time when it is called, socket is NOT referenced by any
 *   hash tables, lists etc.
 * * Packets, delivered from outside (from network or from another process)
 *   and enqueued on receive/error queues SHOULD NOT grab reference count,
 *   when they sit in queue. Otherwise, packets will leak to hole, when
 *   socket is looked up by one cpu and unhasing is made by another CPU.
 *   It is true for udp/raw, netlink (leak to receive and error queues), tcp
 *   (leak to backlog). Packet socket does all the processing inside
 *   BR_NETPROTO_LOCK, so that it has not this race condition. UNIX sockets
 *   use separate SMP lock, so that they are prone too.
 */

/* Ungrab socket and destroy it, if it was the last reference. */
static inline void sock_put(struct sock *sk)
{
<<<<<<< HEAD
	if (atomic_dec_and_test(&sk->sk_refcnt))
		sk_free(sk);
}

extern int sk_receive_skb(struct sock *sk, struct sk_buff *skb,
			  const int nested);

static inline void sk_tx_queue_set(struct sock *sk, int tx_queue)
{
	sk->sk_tx_queue_mapping = tx_queue;
}

static inline void sk_tx_queue_clear(struct sock *sk)
{
	sk->sk_tx_queue_mapping = -1;
=======
	if (refcount_dec_and_test(&sk->sk_refcnt))
		sk_free(sk);
}
/* Generic version of sock_put(), dealing with all sockets
 * (TCP_TIMEWAIT, TCP_NEW_SYN_RECV, ESTABLISHED...)
 */
void sock_gen_put(struct sock *sk);

int __sk_receive_skb(struct sock *sk, struct sk_buff *skb, const int nested,
		     unsigned int trim_cap, bool refcounted);
static inline int sk_receive_skb(struct sock *sk, struct sk_buff *skb,
				 const int nested)
{
	return __sk_receive_skb(sk, skb, nested, 1, true);
}

static inline void sk_tx_queue_set(struct sock *sk, int tx_queue)
{
	/* sk_tx_queue_mapping accept only upto a 16-bit value */
	if (WARN_ON_ONCE((unsigned short)tx_queue >= USHRT_MAX))
		return;
	/* Paired with READ_ONCE() in sk_tx_queue_get() and
	 * other WRITE_ONCE() because socket lock might be not held.
	 */
	WRITE_ONCE(sk->sk_tx_queue_mapping, tx_queue);
}

#define NO_QUEUE_MAPPING	USHRT_MAX

static inline void sk_tx_queue_clear(struct sock *sk)
{
	/* Paired with READ_ONCE() in sk_tx_queue_get() and
	 * other WRITE_ONCE() because socket lock might be not held.
	 */
	WRITE_ONCE(sk->sk_tx_queue_mapping, NO_QUEUE_MAPPING);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline int sk_tx_queue_get(const struct sock *sk)
{
<<<<<<< HEAD
	return sk ? sk->sk_tx_queue_mapping : -1;
=======
	if (sk) {
		/* Paired with WRITE_ONCE() in sk_tx_queue_clear()
		 * and sk_tx_queue_set().
		 */
		int val = READ_ONCE(sk->sk_tx_queue_mapping);

		if (val != NO_QUEUE_MAPPING)
			return val;
	}
	return -1;
}

static inline void __sk_rx_queue_set(struct sock *sk,
				     const struct sk_buff *skb,
				     bool force_set)
{
#ifdef CONFIG_SOCK_RX_QUEUE_MAPPING
	if (skb_rx_queue_recorded(skb)) {
		u16 rx_queue = skb_get_rx_queue(skb);

		if (force_set ||
		    unlikely(READ_ONCE(sk->sk_rx_queue_mapping) != rx_queue))
			WRITE_ONCE(sk->sk_rx_queue_mapping, rx_queue);
	}
#endif
}

static inline void sk_rx_queue_set(struct sock *sk, const struct sk_buff *skb)
{
	__sk_rx_queue_set(sk, skb, true);
}

static inline void sk_rx_queue_update(struct sock *sk, const struct sk_buff *skb)
{
	__sk_rx_queue_set(sk, skb, false);
}

static inline void sk_rx_queue_clear(struct sock *sk)
{
#ifdef CONFIG_SOCK_RX_QUEUE_MAPPING
	WRITE_ONCE(sk->sk_rx_queue_mapping, NO_QUEUE_MAPPING);
#endif
}

static inline int sk_rx_queue_get(const struct sock *sk)
{
#ifdef CONFIG_SOCK_RX_QUEUE_MAPPING
	if (sk) {
		int res = READ_ONCE(sk->sk_rx_queue_mapping);

		if (res != NO_QUEUE_MAPPING)
			return res;
	}
#endif

	return -1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void sk_set_socket(struct sock *sk, struct socket *sock)
{
<<<<<<< HEAD
	sk_tx_queue_clear(sk);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	sk->sk_socket = sock;
}

static inline wait_queue_head_t *sk_sleep(struct sock *sk)
{
	BUILD_BUG_ON(offsetof(struct socket_wq, wait) != 0);
	return &rcu_dereference_raw(sk->sk_wq)->wait;
}
/* Detach socket from process context.
 * Announce socket dead, detach it from wait queue and inode.
 * Note that parent inode held reference count on this struct sock,
 * we do not release it in this function, because protocol
 * probably wants some additional cleanups or even continuing
 * to work with this socket (TCP).
 */
static inline void sock_orphan(struct sock *sk)
{
	write_lock_bh(&sk->sk_callback_lock);
	sock_set_flag(sk, SOCK_DEAD);
	sk_set_socket(sk, NULL);
	sk->sk_wq  = NULL;
	write_unlock_bh(&sk->sk_callback_lock);
}

static inline void sock_graft(struct sock *sk, struct socket *parent)
{
<<<<<<< HEAD
	write_lock_bh(&sk->sk_callback_lock);
	sk->sk_wq = parent->wq;
=======
	WARN_ON(parent->sk);
	write_lock_bh(&sk->sk_callback_lock);
	rcu_assign_pointer(sk->sk_wq, &parent->wq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	parent->sk = sk;
	sk_set_socket(sk, parent);
	sk->sk_uid = SOCK_INODE(parent)->i_uid;
	security_sock_graft(sk, parent);
	write_unlock_bh(&sk->sk_callback_lock);
}

<<<<<<< HEAD
extern int sock_i_uid(struct sock *sk);
extern unsigned long sock_i_ino(struct sock *sk);
=======
kuid_t sock_i_uid(struct sock *sk);
unsigned long __sock_i_ino(struct sock *sk);
unsigned long sock_i_ino(struct sock *sk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline kuid_t sock_net_uid(const struct net *net, const struct sock *sk)
{
	return sk ? sk->sk_uid : make_kuid(net->user_ns, 0);
}

<<<<<<< HEAD
static inline struct dst_entry *
__sk_dst_get(struct sock *sk)
{
	return rcu_dereference_check(sk->sk_dst_cache, sock_owned_by_user(sk) ||
						       lockdep_is_held(&sk->sk_lock.slock));
}

static inline struct dst_entry *
sk_dst_get(struct sock *sk)
=======
static inline u32 net_tx_rndhash(void)
{
	u32 v = get_random_u32();

	return v ?: 1;
}

static inline void sk_set_txhash(struct sock *sk)
{
	/* This pairs with READ_ONCE() in skb_set_hash_from_sk() */
	WRITE_ONCE(sk->sk_txhash, net_tx_rndhash());
}

static inline bool sk_rethink_txhash(struct sock *sk)
{
	if (sk->sk_txhash && sk->sk_txrehash == SOCK_TXREHASH_ENABLED) {
		sk_set_txhash(sk);
		return true;
	}
	return false;
}

static inline struct dst_entry *
__sk_dst_get(const struct sock *sk)
{
	return rcu_dereference_check(sk->sk_dst_cache,
				     lockdep_sock_is_held(sk));
}

static inline struct dst_entry *
sk_dst_get(const struct sock *sk)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct dst_entry *dst;

	rcu_read_lock();
	dst = rcu_dereference(sk->sk_dst_cache);
<<<<<<< HEAD
	if (dst)
		dst_hold(dst);
=======
	if (dst && !rcuref_get(&dst->__rcuref))
		dst = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rcu_read_unlock();
	return dst;
}

<<<<<<< HEAD
extern void sk_reset_txq(struct sock *sk);

static inline void dst_negative_advice(struct sock *sk)
=======
static inline void __dst_negative_advice(struct sock *sk)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct dst_entry *ndst, *dst = __sk_dst_get(sk);

	if (dst && dst->ops->negative_advice) {
		ndst = dst->ops->negative_advice(dst);

		if (ndst != dst) {
			rcu_assign_pointer(sk->sk_dst_cache, ndst);
<<<<<<< HEAD
			sk_reset_txq(sk);
=======
			sk_tx_queue_clear(sk);
			WRITE_ONCE(sk->sk_dst_pending_confirm, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}
}

<<<<<<< HEAD
=======
static inline void dst_negative_advice(struct sock *sk)
{
	sk_rethink_txhash(sk);
	__dst_negative_advice(sk);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline void
__sk_dst_set(struct sock *sk, struct dst_entry *dst)
{
	struct dst_entry *old_dst;

	sk_tx_queue_clear(sk);
<<<<<<< HEAD
	/*
	 * This can be called while sk is owned by the caller only,
	 * with no state that can be checked in a rcu_dereference_check() cond
	 */
	old_dst = rcu_dereference_raw(sk->sk_dst_cache);
=======
	WRITE_ONCE(sk->sk_dst_pending_confirm, 0);
	old_dst = rcu_dereference_protected(sk->sk_dst_cache,
					    lockdep_sock_is_held(sk));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rcu_assign_pointer(sk->sk_dst_cache, dst);
	dst_release(old_dst);
}

static inline void
sk_dst_set(struct sock *sk, struct dst_entry *dst)
{
<<<<<<< HEAD
	spin_lock(&sk->sk_dst_lock);
	__sk_dst_set(sk, dst);
	spin_unlock(&sk->sk_dst_lock);
=======
	struct dst_entry *old_dst;

	sk_tx_queue_clear(sk);
	WRITE_ONCE(sk->sk_dst_pending_confirm, 0);
	old_dst = xchg((__force struct dst_entry **)&sk->sk_dst_cache, dst);
	dst_release(old_dst);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void
__sk_dst_reset(struct sock *sk)
{
	__sk_dst_set(sk, NULL);
}

static inline void
sk_dst_reset(struct sock *sk)
{
<<<<<<< HEAD
	spin_lock(&sk->sk_dst_lock);
	__sk_dst_reset(sk);
	spin_unlock(&sk->sk_dst_lock);
}

extern struct dst_entry *__sk_dst_check(struct sock *sk, u32 cookie);

extern struct dst_entry *sk_dst_check(struct sock *sk, u32 cookie);

static inline int sk_can_gso(const struct sock *sk)
=======
	sk_dst_set(sk, NULL);
}

struct dst_entry *__sk_dst_check(struct sock *sk, u32 cookie);

struct dst_entry *sk_dst_check(struct sock *sk, u32 cookie);

static inline void sk_dst_confirm(struct sock *sk)
{
	if (!READ_ONCE(sk->sk_dst_pending_confirm))
		WRITE_ONCE(sk->sk_dst_pending_confirm, 1);
}

static inline void sock_confirm_neigh(struct sk_buff *skb, struct neighbour *n)
{
	if (skb_get_dst_pending_confirm(skb)) {
		struct sock *sk = skb->sk;

		if (sk && READ_ONCE(sk->sk_dst_pending_confirm))
			WRITE_ONCE(sk->sk_dst_pending_confirm, 0);
		neigh_confirm(n);
	}
}

bool sk_mc_loop(const struct sock *sk);

static inline bool sk_can_gso(const struct sock *sk)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return net_gso_ok(sk->sk_route_caps, sk->sk_gso_type);
}

<<<<<<< HEAD
extern void sk_setup_caps(struct sock *sk, struct dst_entry *dst);

static inline void sk_nocaps_add(struct sock *sk, netdev_features_t flags)
{
	sk->sk_route_nocaps |= flags;
	sk->sk_route_caps &= ~flags;
}

static inline int skb_do_copy_data_nocache(struct sock *sk, struct sk_buff *skb,
					   char __user *from, char *to,
					   int copy, int offset)
{
	if (skb->ip_summed == CHECKSUM_NONE) {
		int err = 0;
		__wsum csum = csum_and_copy_from_user(from, to, copy, 0, &err);
		if (err)
			return err;
		skb->csum = csum_block_add(skb->csum, csum, offset);
	} else if (sk->sk_route_caps & NETIF_F_NOCACHE_COPY) {
		if (!access_ok(VERIFY_READ, from, copy) ||
		    __copy_from_user_nocache(to, from, copy))
			return -EFAULT;
	} else if (copy_from_user(to, from, copy))
=======
void sk_setup_caps(struct sock *sk, struct dst_entry *dst);

static inline void sk_gso_disable(struct sock *sk)
{
	sk->sk_gso_disabled = 1;
	sk->sk_route_caps &= ~NETIF_F_GSO_MASK;
}

static inline int skb_do_copy_data_nocache(struct sock *sk, struct sk_buff *skb,
					   struct iov_iter *from, char *to,
					   int copy, int offset)
{
	if (skb->ip_summed == CHECKSUM_NONE) {
		__wsum csum = 0;
		if (!csum_and_copy_from_iter_full(to, copy, &csum, from))
			return -EFAULT;
		skb->csum = csum_block_add(skb->csum, csum, offset);
	} else if (sk->sk_route_caps & NETIF_F_NOCACHE_COPY) {
		if (!copy_from_iter_full_nocache(to, copy, from))
			return -EFAULT;
	} else if (!copy_from_iter_full(to, copy, from))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EFAULT;

	return 0;
}

static inline int skb_add_data_nocache(struct sock *sk, struct sk_buff *skb,
<<<<<<< HEAD
				       char __user *from, int copy)
=======
				       struct iov_iter *from, int copy)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int err, offset = skb->len;

	err = skb_do_copy_data_nocache(sk, skb, from, skb_put(skb, copy),
				       copy, offset);
	if (err)
		__skb_trim(skb, offset);

	return err;
}

<<<<<<< HEAD
static inline int skb_copy_to_page_nocache(struct sock *sk, char __user *from,
=======
static inline int skb_copy_to_page_nocache(struct sock *sk, struct iov_iter *from,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					   struct sk_buff *skb,
					   struct page *page,
					   int off, int copy)
{
	int err;

	err = skb_do_copy_data_nocache(sk, skb, from, page_address(page) + off,
				       copy, skb->len);
	if (err)
		return err;

<<<<<<< HEAD
	skb->len	     += copy;
	skb->data_len	     += copy;
	skb->truesize	     += copy;
	sk->sk_wmem_queued   += copy;
	sk_mem_charge(sk, copy);
	return 0;
}

static inline int skb_copy_to_page(struct sock *sk, char __user *from,
				   struct sk_buff *skb, struct page *page,
				   int off, int copy)
{
	if (skb->ip_summed == CHECKSUM_NONE) {
		int err = 0;
		__wsum csum = csum_and_copy_from_user(from,
						     page_address(page) + off,
							    copy, 0, &err);
		if (err)
			return err;
		skb->csum = csum_block_add(skb->csum, csum, skb->len);
	} else if (copy_from_user(page_address(page) + off, from, copy))
		return -EFAULT;

	skb->len	     += copy;
	skb->data_len	     += copy;
	skb->truesize	     += copy;
	sk->sk_wmem_queued   += copy;
=======
	skb_len_add(skb, copy);
	sk_wmem_queued_add(sk, copy);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	sk_mem_charge(sk, copy);
	return 0;
}

/**
 * sk_wmem_alloc_get - returns write allocations
 * @sk: socket
 *
<<<<<<< HEAD
 * Returns sk_wmem_alloc minus initial offset of one
 */
static inline int sk_wmem_alloc_get(const struct sock *sk)
{
	return atomic_read(&sk->sk_wmem_alloc) - 1;
=======
 * Return: sk_wmem_alloc minus initial offset of one
 */
static inline int sk_wmem_alloc_get(const struct sock *sk)
{
	return refcount_read(&sk->sk_wmem_alloc) - 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * sk_rmem_alloc_get - returns read allocations
 * @sk: socket
 *
<<<<<<< HEAD
 * Returns sk_rmem_alloc
=======
 * Return: sk_rmem_alloc
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
static inline int sk_rmem_alloc_get(const struct sock *sk)
{
	return atomic_read(&sk->sk_rmem_alloc);
}

/**
 * sk_has_allocations - check if allocations are outstanding
 * @sk: socket
 *
<<<<<<< HEAD
 * Returns true if socket has write or read allocations
 */
static inline int sk_has_allocations(const struct sock *sk)
=======
 * Return: true if socket has write or read allocations
 */
static inline bool sk_has_allocations(const struct sock *sk)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return sk_wmem_alloc_get(sk) || sk_rmem_alloc_get(sk);
}

/**
<<<<<<< HEAD
 * wq_has_sleeper - check if there are any waiting processes
 * @wq: struct socket_wq
 *
 * Returns true if socket_wq has waiting processes
 *
 * The purpose of the wq_has_sleeper and sock_poll_wait is to wrap the memory
 * barrier call. They were added due to the race found within the tcp code.
 *
 * Consider following tcp code paths:
 *
 * CPU1                  CPU2
 *
 * sys_select            receive packet
=======
 * skwq_has_sleeper - check if there are any waiting processes
 * @wq: struct socket_wq
 *
 * Return: true if socket_wq has waiting processes
 *
 * The purpose of the skwq_has_sleeper and sock_poll_wait is to wrap the memory
 * barrier call. They were added due to the race found within the tcp code.
 *
 * Consider following tcp code paths::
 *
 *   CPU1                CPU2
 *   sys_select          receive packet
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *   ...                 ...
 *   __add_wait_queue    update tp->rcv_nxt
 *   ...                 ...
 *   tp->rcv_nxt check   sock_def_readable
 *   ...                 {
 *   schedule               rcu_read_lock();
 *                          wq = rcu_dereference(sk->sk_wq);
 *                          if (wq && waitqueue_active(&wq->wait))
 *                              wake_up_interruptible(&wq->wait)
 *                          ...
 *                       }
 *
 * The race for tcp fires when the __add_wait_queue changes done by CPU1 stay
 * in its cache, and so does the tp->rcv_nxt update on CPU2 side.  The CPU1
 * could then endup calling schedule and sleep forever if there are no more
 * data on the socket.
 *
 */
<<<<<<< HEAD
static inline bool wq_has_sleeper(struct socket_wq *wq)
{

	/*
	 * We need to be sure we are in sync with the
	 * add_wait_queue modifications to the wait queue.
	 *
	 * This memory barrier is paired in the sock_poll_wait.
	 */
	smp_mb();
	return wq && waitqueue_active(&wq->wait);
=======
static inline bool skwq_has_sleeper(struct socket_wq *wq)
{
	return wq && wq_has_sleeper(&wq->wait);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * sock_poll_wait - place memory barrier behind the poll_wait call.
 * @filp:           file
<<<<<<< HEAD
 * @wait_address:   socket wait queue
=======
 * @sock:           socket to wait on
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @p:              poll_table
 *
 * See the comments in the wq_has_sleeper function.
 */
<<<<<<< HEAD
static inline void sock_poll_wait(struct file *filp,
		wait_queue_head_t *wait_address, poll_table *p)
{
	if (!poll_does_not_wait(p) && wait_address) {
		poll_wait(filp, wait_address, p);
		/*
		 * We need to be sure we are in sync with the
		 * socket flags modification.
		 *
		 * This memory barrier is paired in the wq_has_sleeper.
		*/
=======
static inline void sock_poll_wait(struct file *filp, struct socket *sock,
				  poll_table *p)
{
	if (!poll_does_not_wait(p)) {
		poll_wait(filp, &sock->wq.wait, p);
		/* We need to be sure we are in sync with the
		 * socket flags modification.
		 *
		 * This memory barrier is paired in the wq_has_sleeper.
		 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		smp_mb();
	}
}

<<<<<<< HEAD
/*
 * 	Queue a received datagram if it will fit. Stream and sequenced
 *	protocols can't normally use this as they need to fit buffers in
 *	and play with them.
 *
 * 	Inlined as it's very short and called for pretty much every
 *	packet ever received.
 */

static inline void skb_set_owner_w(struct sk_buff *skb, struct sock *sk)
{
	skb_orphan(skb);
	skb->sk = sk;
	skb->destructor = sock_wfree;
	/*
	 * We used to take a refcount on sk, but following operation
	 * is enough to guarantee sk_free() wont free this sock until
	 * all in-flight packets are completed
	 */
	atomic_add(skb->truesize, &sk->sk_wmem_alloc);
}

=======
static inline void skb_set_hash_from_sk(struct sk_buff *skb, struct sock *sk)
{
	/* This pairs with WRITE_ONCE() in sk_set_txhash() */
	u32 txhash = READ_ONCE(sk->sk_txhash);

	if (txhash) {
		skb->l4_hash = 1;
		skb->hash = txhash;
	}
}

void skb_set_owner_w(struct sk_buff *skb, struct sock *sk);

/*
 *	Queue a received datagram if it will fit. Stream and sequenced
 *	protocols can't normally use this as they need to fit buffers in
 *	and play with them.
 *
 *	Inlined as it's very short and called for pretty much every
 *	packet ever received.
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline void skb_set_owner_r(struct sk_buff *skb, struct sock *sk)
{
	skb_orphan(skb);
	skb->sk = sk;
	skb->destructor = sock_rfree;
	atomic_add(skb->truesize, &sk->sk_rmem_alloc);
	sk_mem_charge(sk, skb->truesize);
}

<<<<<<< HEAD
extern void sk_reset_timer(struct sock *sk, struct timer_list* timer,
			   unsigned long expires);

extern void sk_stop_timer(struct sock *sk, struct timer_list* timer);

extern int sock_queue_rcv_skb(struct sock *sk, struct sk_buff *skb);

extern int sock_queue_err_skb(struct sock *sk, struct sk_buff *skb);
=======
static inline __must_check bool skb_set_owner_sk_safe(struct sk_buff *skb, struct sock *sk)
{
	if (sk && refcount_inc_not_zero(&sk->sk_refcnt)) {
		skb_orphan(skb);
		skb->destructor = sock_efree;
		skb->sk = sk;
		return true;
	}
	return false;
}

static inline struct sk_buff *skb_clone_and_charge_r(struct sk_buff *skb, struct sock *sk)
{
	skb = skb_clone(skb, sk_gfp_mask(sk, GFP_ATOMIC));
	if (skb) {
		if (sk_rmem_schedule(sk, skb, skb->truesize)) {
			skb_set_owner_r(skb, sk);
			return skb;
		}
		__kfree_skb(skb);
	}
	return NULL;
}

static inline void skb_prepare_for_gro(struct sk_buff *skb)
{
	if (skb->destructor != sock_wfree) {
		skb_orphan(skb);
		return;
	}
	skb->slow_gro = 1;
}

void sk_reset_timer(struct sock *sk, struct timer_list *timer,
		    unsigned long expires);

void sk_stop_timer(struct sock *sk, struct timer_list *timer);

void sk_stop_timer_sync(struct sock *sk, struct timer_list *timer);

int __sk_queue_drop_skb(struct sock *sk, struct sk_buff_head *sk_queue,
			struct sk_buff *skb, unsigned int flags,
			void (*destructor)(struct sock *sk,
					   struct sk_buff *skb));
int __sock_queue_rcv_skb(struct sock *sk, struct sk_buff *skb);

int sock_queue_rcv_skb_reason(struct sock *sk, struct sk_buff *skb,
			      enum skb_drop_reason *reason);

static inline int sock_queue_rcv_skb(struct sock *sk, struct sk_buff *skb)
{
	return sock_queue_rcv_skb_reason(sk, skb, NULL);
}

int sock_queue_err_skb(struct sock *sk, struct sk_buff *skb);
struct sk_buff *sock_dequeue_err_skb(struct sock *sk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 *	Recover an error report and clear atomically
 */
<<<<<<< HEAD
 
static inline int sock_error(struct sock *sk)
{
	int err;
	if (likely(!sk->sk_err))
		return 0;
=======

static inline int sock_error(struct sock *sk)
{
	int err;

	/* Avoid an atomic operation for the common case.
	 * This is racy since another cpu/thread can change sk_err under us.
	 */
	if (likely(data_race(!sk->sk_err)))
		return 0;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	err = xchg(&sk->sk_err, 0);
	return -err;
}

<<<<<<< HEAD
=======
void sk_error_report(struct sock *sk);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline unsigned long sock_wspace(struct sock *sk)
{
	int amt = 0;

	if (!(sk->sk_shutdown & SEND_SHUTDOWN)) {
<<<<<<< HEAD
		amt = sk->sk_sndbuf - atomic_read(&sk->sk_wmem_alloc);
		if (amt < 0) 
=======
		amt = sk->sk_sndbuf - refcount_read(&sk->sk_wmem_alloc);
		if (amt < 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			amt = 0;
	}
	return amt;
}

<<<<<<< HEAD
static inline void sk_wake_async(struct sock *sk, int how, int band)
{
	if (sock_flag(sk, SOCK_FASYNC))
		sock_wake_async(sk->sk_socket, how, band);
}

#define SOCK_MIN_SNDBUF 2048
/*
 * Since sk_rmem_alloc sums skb->truesize, even a small frame might need
 * sizeof(sk_buff) + MTU + padding, unless net driver perform copybreak
 */
#define SOCK_MIN_RCVBUF (2048 + sizeof(struct sk_buff))

static inline void sk_stream_moderate_sndbuf(struct sock *sk)
{
	if (!(sk->sk_userlocks & SOCK_SNDBUF_LOCK)) {
		sk->sk_sndbuf = min(sk->sk_sndbuf, sk->sk_wmem_queued >> 1);
		sk->sk_sndbuf = max(sk->sk_sndbuf, SOCK_MIN_SNDBUF);
	}
}

struct sk_buff *sk_stream_alloc_skb(struct sock *sk, int size, gfp_t gfp);

static inline struct page *sk_stream_alloc_page(struct sock *sk)
{
	struct page *page = NULL;

	page = alloc_pages(sk->sk_allocation, 0);
	if (!page) {
		sk_enter_memory_pressure(sk);
		sk_stream_moderate_sndbuf(sk);
	}
	return page;
}

/*
 *	Default write policy as shown to user space via poll/select/SIGIO
 */
static inline int sock_writeable(const struct sock *sk) 
{
	return atomic_read(&sk->sk_wmem_alloc) < (sk->sk_sndbuf >> 1);
=======
/* Note:
 *  We use sk->sk_wq_raw, from contexts knowing this
 *  pointer is not NULL and cannot disappear/change.
 */
static inline void sk_set_bit(int nr, struct sock *sk)
{
	if ((nr == SOCKWQ_ASYNC_NOSPACE || nr == SOCKWQ_ASYNC_WAITDATA) &&
	    !sock_flag(sk, SOCK_FASYNC))
		return;

	set_bit(nr, &sk->sk_wq_raw->flags);
}

static inline void sk_clear_bit(int nr, struct sock *sk)
{
	if ((nr == SOCKWQ_ASYNC_NOSPACE || nr == SOCKWQ_ASYNC_WAITDATA) &&
	    !sock_flag(sk, SOCK_FASYNC))
		return;

	clear_bit(nr, &sk->sk_wq_raw->flags);
}

static inline void sk_wake_async(const struct sock *sk, int how, int band)
{
	if (sock_flag(sk, SOCK_FASYNC)) {
		rcu_read_lock();
		sock_wake_async(rcu_dereference(sk->sk_wq), how, band);
		rcu_read_unlock();
	}
}

/* Since sk_{r,w}mem_alloc sums skb->truesize, even a small frame might
 * need sizeof(sk_buff) + MTU + padding, unless net driver perform copybreak.
 * Note: for send buffers, TCP works better if we can build two skbs at
 * minimum.
 */
#define TCP_SKB_MIN_TRUESIZE	(2048 + SKB_DATA_ALIGN(sizeof(struct sk_buff)))

#define SOCK_MIN_SNDBUF		(TCP_SKB_MIN_TRUESIZE * 2)
#define SOCK_MIN_RCVBUF		 TCP_SKB_MIN_TRUESIZE

static inline void sk_stream_moderate_sndbuf(struct sock *sk)
{
	u32 val;

	if (sk->sk_userlocks & SOCK_SNDBUF_LOCK)
		return;

	val = min(sk->sk_sndbuf, sk->sk_wmem_queued >> 1);
	val = max_t(u32, val, sk_unused_reserved_mem(sk));

	WRITE_ONCE(sk->sk_sndbuf, max_t(u32, val, SOCK_MIN_SNDBUF));
}

/**
 * sk_page_frag - return an appropriate page_frag
 * @sk: socket
 *
 * Use the per task page_frag instead of the per socket one for
 * optimization when we know that we're in process context and own
 * everything that's associated with %current.
 *
 * Both direct reclaim and page faults can nest inside other
 * socket operations and end up recursing into sk_page_frag()
 * while it's already in use: explicitly avoid task page_frag
 * when users disable sk_use_task_frag.
 *
 * Return: a per task page_frag if context allows that,
 * otherwise a per socket one.
 */
static inline struct page_frag *sk_page_frag(struct sock *sk)
{
	if (sk->sk_use_task_frag)
		return &current->task_frag;

	return &sk->sk_frag;
}

bool sk_page_frag_refill(struct sock *sk, struct page_frag *pfrag);

/*
 *	Default write policy as shown to user space via poll/select/SIGIO
 */
static inline bool sock_writeable(const struct sock *sk)
{
	return refcount_read(&sk->sk_wmem_alloc) < (READ_ONCE(sk->sk_sndbuf) >> 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline gfp_t gfp_any(void)
{
	return in_softirq() ? GFP_ATOMIC : GFP_KERNEL;
}

<<<<<<< HEAD
static inline long sock_rcvtimeo(const struct sock *sk, int noblock)
=======
static inline gfp_t gfp_memcg_charge(void)
{
	return in_softirq() ? GFP_ATOMIC : GFP_KERNEL;
}

static inline long sock_rcvtimeo(const struct sock *sk, bool noblock)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return noblock ? 0 : sk->sk_rcvtimeo;
}

<<<<<<< HEAD
static inline long sock_sndtimeo(const struct sock *sk, int noblock)
=======
static inline long sock_sndtimeo(const struct sock *sk, bool noblock)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return noblock ? 0 : sk->sk_sndtimeo;
}

static inline int sock_rcvlowat(const struct sock *sk, int waitall, int len)
{
<<<<<<< HEAD
	return (waitall ? len : min_t(int, sk->sk_rcvlowat, len)) ? : 1;
=======
	int v = waitall ? len : min_t(int, READ_ONCE(sk->sk_rcvlowat), len);

	return v ?: 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Alas, with timeout socket operations are not restartable.
 * Compare this to poll().
 */
static inline int sock_intr_errno(long timeo)
{
	return timeo == MAX_SCHEDULE_TIMEOUT ? -ERESTARTSYS : -EINTR;
}

<<<<<<< HEAD
extern void __sock_recv_timestamp(struct msghdr *msg, struct sock *sk,
	struct sk_buff *skb);
extern void __sock_recv_wifi_status(struct msghdr *msg, struct sock *sk,
	struct sk_buff *skb);

static __inline__ void
sock_recv_timestamp(struct msghdr *msg, struct sock *sk, struct sk_buff *skb)
{
	ktime_t kt = skb->tstamp;
	struct skb_shared_hwtstamps *hwtstamps = skb_hwtstamps(skb);

	/*
	 * generate control messages if
	 * - receive time stamping in software requested (SOCK_RCVTSTAMP
	 *   or SOCK_TIMESTAMPING_RX_SOFTWARE)
	 * - software time stamp available and wanted
	 *   (SOCK_TIMESTAMPING_SOFTWARE)
	 * - hardware time stamps available and wanted
	 *   (SOCK_TIMESTAMPING_SYS_HARDWARE or
	 *   SOCK_TIMESTAMPING_RAW_HARDWARE)
	 */
	if (sock_flag(sk, SOCK_RCVTSTAMP) ||
	    sock_flag(sk, SOCK_TIMESTAMPING_RX_SOFTWARE) ||
	    (kt.tv64 && sock_flag(sk, SOCK_TIMESTAMPING_SOFTWARE)) ||
	    (hwtstamps->hwtstamp.tv64 &&
	     sock_flag(sk, SOCK_TIMESTAMPING_RAW_HARDWARE)) ||
	    (hwtstamps->syststamp.tv64 &&
	     sock_flag(sk, SOCK_TIMESTAMPING_SYS_HARDWARE)))
		__sock_recv_timestamp(msg, sk, skb);
	else
		sk->sk_stamp = kt;

	if (sock_flag(sk, SOCK_WIFI_STATUS) && skb->wifi_acked_valid)
		__sock_recv_wifi_status(msg, sk, skb);
}

extern void __sock_recv_ts_and_drops(struct msghdr *msg, struct sock *sk,
				     struct sk_buff *skb);

static inline void sock_recv_ts_and_drops(struct msghdr *msg, struct sock *sk,
					  struct sk_buff *skb)
{
#define FLAGS_TS_OR_DROPS ((1UL << SOCK_RXQ_OVFL)			| \
			   (1UL << SOCK_RCVTSTAMP)			| \
			   (1UL << SOCK_TIMESTAMPING_RX_SOFTWARE)	| \
			   (1UL << SOCK_TIMESTAMPING_SOFTWARE)		| \
			   (1UL << SOCK_TIMESTAMPING_RAW_HARDWARE) 	| \
			   (1UL << SOCK_TIMESTAMPING_SYS_HARDWARE))

	if (sk->sk_flags & FLAGS_TS_OR_DROPS)
		__sock_recv_ts_and_drops(msg, sk, skb);
	else
		sk->sk_stamp = skb->tstamp;
}

/**
 * sock_tx_timestamp - checks whether the outgoing packet is to be time stamped
 * @sk:		socket sending this packet
 * @tx_flags:	filled with instructions for time stamping
 *
 * Currently only depends on SOCK_TIMESTAMPING* flags. Returns error code if
 * parameters are invalid.
 */
extern int sock_tx_timestamp(struct sock *sk, __u8 *tx_flags);
=======
struct sock_skb_cb {
	u32 dropcount;
};

/* Store sock_skb_cb at the end of skb->cb[] so protocol families
 * using skb->cb[] would keep using it directly and utilize its
 * alignement guarantee.
 */
#define SOCK_SKB_CB_OFFSET ((sizeof_field(struct sk_buff, cb) - \
			    sizeof(struct sock_skb_cb)))

#define SOCK_SKB_CB(__skb) ((struct sock_skb_cb *)((__skb)->cb + \
			    SOCK_SKB_CB_OFFSET))

#define sock_skb_cb_check_size(size) \
	BUILD_BUG_ON((size) > SOCK_SKB_CB_OFFSET)

static inline void
sock_skb_set_dropcount(const struct sock *sk, struct sk_buff *skb)
{
	SOCK_SKB_CB(skb)->dropcount = sock_flag(sk, SOCK_RXQ_OVFL) ?
						atomic_read(&sk->sk_drops) : 0;
}

static inline void sk_drops_add(struct sock *sk, const struct sk_buff *skb)
{
	int segs = max_t(u16, 1, skb_shinfo(skb)->gso_segs);

	atomic_add(segs, &sk->sk_drops);
}

static inline ktime_t sock_read_timestamp(struct sock *sk)
{
#if BITS_PER_LONG==32
	unsigned int seq;
	ktime_t kt;

	do {
		seq = read_seqbegin(&sk->sk_stamp_seq);
		kt = sk->sk_stamp;
	} while (read_seqretry(&sk->sk_stamp_seq, seq));

	return kt;
#else
	return READ_ONCE(sk->sk_stamp);
#endif
}

static inline void sock_write_timestamp(struct sock *sk, ktime_t kt)
{
#if BITS_PER_LONG==32
	write_seqlock(&sk->sk_stamp_seq);
	sk->sk_stamp = kt;
	write_sequnlock(&sk->sk_stamp_seq);
#else
	WRITE_ONCE(sk->sk_stamp, kt);
#endif
}

void __sock_recv_timestamp(struct msghdr *msg, struct sock *sk,
			   struct sk_buff *skb);
void __sock_recv_wifi_status(struct msghdr *msg, struct sock *sk,
			     struct sk_buff *skb);

static inline void
sock_recv_timestamp(struct msghdr *msg, struct sock *sk, struct sk_buff *skb)
{
	struct skb_shared_hwtstamps *hwtstamps = skb_hwtstamps(skb);
	u32 tsflags = READ_ONCE(sk->sk_tsflags);
	ktime_t kt = skb->tstamp;
	/*
	 * generate control messages if
	 * - receive time stamping in software requested
	 * - software time stamp available and wanted
	 * - hardware time stamps available and wanted
	 */
	if (sock_flag(sk, SOCK_RCVTSTAMP) ||
	    (tsflags & SOF_TIMESTAMPING_RX_SOFTWARE) ||
	    (kt && tsflags & SOF_TIMESTAMPING_SOFTWARE) ||
	    (hwtstamps->hwtstamp &&
	     (tsflags & SOF_TIMESTAMPING_RAW_HARDWARE)))
		__sock_recv_timestamp(msg, sk, skb);
	else
		sock_write_timestamp(sk, kt);

	if (sock_flag(sk, SOCK_WIFI_STATUS) && skb_wifi_acked_valid(skb))
		__sock_recv_wifi_status(msg, sk, skb);
}

void __sock_recv_cmsgs(struct msghdr *msg, struct sock *sk,
		       struct sk_buff *skb);

#define SK_DEFAULT_STAMP (-1L * NSEC_PER_SEC)
static inline void sock_recv_cmsgs(struct msghdr *msg, struct sock *sk,
				   struct sk_buff *skb)
{
#define FLAGS_RECV_CMSGS ((1UL << SOCK_RXQ_OVFL)			| \
			   (1UL << SOCK_RCVTSTAMP)			| \
			   (1UL << SOCK_RCVMARK))
#define TSFLAGS_ANY	  (SOF_TIMESTAMPING_SOFTWARE			| \
			   SOF_TIMESTAMPING_RAW_HARDWARE)

	if (sk->sk_flags & FLAGS_RECV_CMSGS ||
	    READ_ONCE(sk->sk_tsflags) & TSFLAGS_ANY)
		__sock_recv_cmsgs(msg, sk, skb);
	else if (unlikely(sock_flag(sk, SOCK_TIMESTAMP)))
		sock_write_timestamp(sk, skb->tstamp);
	else if (unlikely(sock_read_timestamp(sk) == SK_DEFAULT_STAMP))
		sock_write_timestamp(sk, 0);
}

void __sock_tx_timestamp(__u16 tsflags, __u8 *tx_flags);

/**
 * _sock_tx_timestamp - checks whether the outgoing packet is to be time stamped
 * @sk:		socket sending this packet
 * @tsflags:	timestamping flags to use
 * @tx_flags:	completed with instructions for time stamping
 * @tskey:      filled in with next sk_tskey (not for TCP, which uses seqno)
 *
 * Note: callers should take care of initial ``*tx_flags`` value (usually 0)
 */
static inline void _sock_tx_timestamp(struct sock *sk, __u16 tsflags,
				      __u8 *tx_flags, __u32 *tskey)
{
	if (unlikely(tsflags)) {
		__sock_tx_timestamp(tsflags, tx_flags);
		if (tsflags & SOF_TIMESTAMPING_OPT_ID && tskey &&
		    tsflags & SOF_TIMESTAMPING_TX_RECORD_MASK)
			*tskey = atomic_inc_return(&sk->sk_tskey) - 1;
	}
	if (unlikely(sock_flag(sk, SOCK_WIFI_STATUS)))
		*tx_flags |= SKBTX_WIFI_STATUS;
}

static inline void sock_tx_timestamp(struct sock *sk, __u16 tsflags,
				     __u8 *tx_flags)
{
	_sock_tx_timestamp(sk, tsflags, tx_flags, NULL);
}

static inline void skb_setup_tx_timestamp(struct sk_buff *skb, __u16 tsflags)
{
	_sock_tx_timestamp(skb->sk, tsflags, &skb_shinfo(skb)->tx_flags,
			   &skb_shinfo(skb)->tskey);
}

static inline bool sk_is_inet(const struct sock *sk)
{
	int family = READ_ONCE(sk->sk_family);

	return family == AF_INET || family == AF_INET6;
}

static inline bool sk_is_tcp(const struct sock *sk)
{
	return sk_is_inet(sk) &&
	       sk->sk_type == SOCK_STREAM &&
	       sk->sk_protocol == IPPROTO_TCP;
}

static inline bool sk_is_udp(const struct sock *sk)
{
	return sk_is_inet(sk) &&
	       sk->sk_type == SOCK_DGRAM &&
	       sk->sk_protocol == IPPROTO_UDP;
}

static inline bool sk_is_stream_unix(const struct sock *sk)
{
	return sk->sk_family == AF_UNIX && sk->sk_type == SOCK_STREAM;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * sk_eat_skb - Release a skb if it is no longer needed
 * @sk: socket to eat this skb from
 * @skb: socket buffer to eat
<<<<<<< HEAD
 * @copied_early: flag indicating whether DMA operations copied this data early
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * This routine must be called with interrupts disabled or with the socket
 * locked so that the sk_buff queue operation is ok.
*/
<<<<<<< HEAD
#ifdef CONFIG_NET_DMA
static inline void sk_eat_skb(struct sock *sk, struct sk_buff *skb, int copied_early)
{
	__skb_unlink(skb, &sk->sk_receive_queue);
	if (!copied_early)
		__kfree_skb(skb);
	else
		__skb_queue_tail(&sk->sk_async_wait_queue, skb);
}
#else
static inline void sk_eat_skb(struct sock *sk, struct sk_buff *skb, int copied_early)
=======
static inline void sk_eat_skb(struct sock *sk, struct sk_buff *skb)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	__skb_unlink(skb, &sk->sk_receive_queue);
	__kfree_skb(skb);
}
<<<<<<< HEAD
#endif

static inline
struct net *sock_net(const struct sock *sk)
{
	return read_pnet(&sk->sk_net);
}

static inline
void sock_net_set(struct sock *sk, struct net *net)
{
	write_pnet(&sk->sk_net, net);
}

/*
 * Kernel sockets, f.e. rtnl or icmp_socket, are a part of a namespace.
 * They should not hold a reference to a namespace in order to allow
 * to stop it.
 * Sockets after sk_change_net should be released using sk_release_kernel
 */
static inline void sk_change_net(struct sock *sk, struct net *net)
{
	put_net(sock_net(sk));
	sock_net_set(sk, hold_net(net));
}

static inline struct sock *skb_steal_sock(struct sk_buff *skb)
{
	if (unlikely(skb->sk)) {
		struct sock *sk = skb->sk;

		skb->destructor = NULL;
		skb->sk = NULL;
		return sk;
	}
	return NULL;
=======

static inline bool
skb_sk_is_prefetched(struct sk_buff *skb)
{
#ifdef CONFIG_INET
	return skb->destructor == sock_pfree;
#else
	return false;
#endif /* CONFIG_INET */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* This helper checks if a socket is a full socket,
 * ie _not_ a timewait or request socket.
<<<<<<< HEAD
 * TODO: Check for TCPF_NEW_SYN_RECV when that starts to exist.
 */
static inline bool sk_fullsock(const struct sock *sk)
{
    return (1 << sk->sk_state) & ~(TCPF_TIME_WAIT);
}

extern void sock_enable_timestamp(struct sock *sk, int flag);
extern int sock_get_timestamp(struct sock *, struct timeval __user *);
extern int sock_get_timestampns(struct sock *, struct timespec __user *);

/* 
 *	Enable debug/info messages 
 */
extern int net_msg_warn;
#define NETDEBUG(fmt, args...) \
	do { if (net_msg_warn) printk(fmt,##args); } while (0)

#define LIMIT_NETDEBUG(fmt, args...) \
	do { if (net_msg_warn && net_ratelimit()) printk(fmt,##args); } while(0)
=======
 */
static inline bool sk_fullsock(const struct sock *sk)
{
	return (1 << sk->sk_state) & ~(TCPF_TIME_WAIT | TCPF_NEW_SYN_RECV);
}

static inline bool
sk_is_refcounted(struct sock *sk)
{
	/* Only full sockets have sk->sk_flags. */
	return !sk_fullsock(sk) || !sock_flag(sk, SOCK_RCU_FREE);
}

/* Checks if this SKB belongs to an HW offloaded socket
 * and whether any SW fallbacks are required based on dev.
 * Check decrypted mark in case skb_orphan() cleared socket.
 */
static inline struct sk_buff *sk_validate_xmit_skb(struct sk_buff *skb,
						   struct net_device *dev)
{
#ifdef CONFIG_SOCK_VALIDATE_XMIT
	struct sock *sk = skb->sk;

	if (sk && sk_fullsock(sk) && sk->sk_validate_xmit_skb) {
		skb = sk->sk_validate_xmit_skb(sk, dev, skb);
#ifdef CONFIG_TLS_DEVICE
	} else if (unlikely(skb->decrypted)) {
		pr_warn_ratelimited("unencrypted skb with no associated socket - dropping\n");
		kfree_skb(skb);
		skb = NULL;
#endif
	}
#endif

	return skb;
}

/* This helper checks if a socket is a LISTEN or NEW_SYN_RECV
 * SYNACK messages can be attached to either ones (depending on SYNCOOKIE)
 */
static inline bool sk_listener(const struct sock *sk)
{
	return (1 << sk->sk_state) & (TCPF_LISTEN | TCPF_NEW_SYN_RECV);
}

void sock_enable_timestamp(struct sock *sk, enum sock_flags flag);
int sock_recv_errqueue(struct sock *sk, struct msghdr *msg, int len, int level,
		       int type);

bool sk_ns_capable(const struct sock *sk,
		   struct user_namespace *user_ns, int cap);
bool sk_capable(const struct sock *sk, int cap);
bool sk_net_capable(const struct sock *sk, int cap);

void sk_get_meminfo(const struct sock *sk, u32 *meminfo);

/* Take into consideration the size of the struct sk_buff overhead in the
 * determination of these values, since that is non-constant across
 * platforms.  This makes socket queueing behavior and performance
 * not depend upon such differences.
 */
#define _SK_MEM_PACKETS		256
#define _SK_MEM_OVERHEAD	SKB_TRUESIZE(256)
#define SK_WMEM_MAX		(_SK_MEM_OVERHEAD * _SK_MEM_PACKETS)
#define SK_RMEM_MAX		(_SK_MEM_OVERHEAD * _SK_MEM_PACKETS)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

extern __u32 sysctl_wmem_max;
extern __u32 sysctl_rmem_max;

<<<<<<< HEAD
extern void sk_init(void);

extern int sysctl_optmem_max;
=======
extern int sysctl_tstamp_allow_data;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

extern __u32 sysctl_wmem_default;
extern __u32 sysctl_rmem_default;

<<<<<<< HEAD
/* SOCKEV Notifier Events */
#define SOCKEV_SOCKET   0x00
#define SOCKEV_BIND     0x01
#define SOCKEV_LISTEN   0x02
#define SOCKEV_ACCEPT   0x03
#define SOCKEV_CONNECT  0x04
#define SOCKEV_SHUTDOWN 0x05

int sockev_register_notify(struct notifier_block *nb);
int sockev_unregister_notify(struct notifier_block *nb);

=======
#define SKB_FRAG_PAGE_ORDER	get_order(32768)
DECLARE_STATIC_KEY_FALSE(net_high_order_alloc_disable_key);

static inline int sk_get_wmem0(const struct sock *sk, const struct proto *proto)
{
	/* Does this proto have per netns sysctl_wmem ? */
	if (proto->sysctl_wmem_offset)
		return READ_ONCE(*(int *)((void *)sock_net(sk) + proto->sysctl_wmem_offset));

	return READ_ONCE(*proto->sysctl_wmem);
}

static inline int sk_get_rmem0(const struct sock *sk, const struct proto *proto)
{
	/* Does this proto have per netns sysctl_rmem ? */
	if (proto->sysctl_rmem_offset)
		return READ_ONCE(*(int *)((void *)sock_net(sk) + proto->sysctl_rmem_offset));

	return READ_ONCE(*proto->sysctl_rmem);
}

/* Default TCP Small queue budget is ~1 ms of data (1sec >> 10)
 * Some wifi drivers need to tweak it to get more chunks.
 * They can use this helper from their ndo_start_xmit()
 */
static inline void sk_pacing_shift_update(struct sock *sk, int val)
{
	if (!sk || !sk_fullsock(sk) || READ_ONCE(sk->sk_pacing_shift) == val)
		return;
	WRITE_ONCE(sk->sk_pacing_shift, val);
}

/* if a socket is bound to a device, check that the given device
 * index is either the same or that the socket is bound to an L3
 * master device and the given device index is also enslaved to
 * that L3 master
 */
static inline bool sk_dev_equal_l3scope(struct sock *sk, int dif)
{
	int bound_dev_if = READ_ONCE(sk->sk_bound_dev_if);
	int mdif;

	if (!bound_dev_if || bound_dev_if == dif)
		return true;

	mdif = l3mdev_master_ifindex_by_index(sock_net(sk), dif);
	if (mdif && mdif == bound_dev_if)
		return true;

	return false;
}

void sock_def_readable(struct sock *sk);

int sock_bindtoindex(struct sock *sk, int ifindex, bool lock_sk);
void sock_set_timestamp(struct sock *sk, int optname, bool valbool);
int sock_set_timestamping(struct sock *sk, int optname,
			  struct so_timestamping timestamping);

void sock_enable_timestamps(struct sock *sk);
void sock_no_linger(struct sock *sk);
void sock_set_keepalive(struct sock *sk);
void sock_set_priority(struct sock *sk, u32 priority);
void sock_set_rcvbuf(struct sock *sk, int val);
void sock_set_mark(struct sock *sk, u32 val);
void sock_set_reuseaddr(struct sock *sk);
void sock_set_reuseport(struct sock *sk);
void sock_set_sndtimeo(struct sock *sk, s64 secs);

int sock_bind_add(struct sock *sk, struct sockaddr *addr, int addr_len);

int sock_get_timeout(long timeo, void *optval, bool old_timeval);
int sock_copy_user_timeval(struct __kernel_sock_timeval *tv,
			   sockptr_t optval, int optlen, bool old_timeval);

int sock_ioctl_inout(struct sock *sk, unsigned int cmd,
		     void __user *arg, void *karg, size_t size);
int sk_ioctl(struct sock *sk, unsigned int cmd, void __user *arg);
static inline bool sk_is_readable(struct sock *sk)
{
	if (sk->sk_prot->sock_is_readable)
		return sk->sk_prot->sock_is_readable(sk);
	return false;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif	/* _SOCK_H */
