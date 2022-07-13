/*
 * net/tipc/link.c: TIPC link code
 *
<<<<<<< HEAD
 * Copyright (c) 1996-2007, Ericsson AB
 * Copyright (c) 2004-2007, 2010-2011, Wind River Systems
=======
 * Copyright (c) 1996-2007, 2012-2016, Ericsson AB
 * Copyright (c) 2004-2007, 2010-2013, Wind River Systems
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the names of the copyright holders nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * Alternatively, this software may be distributed under the terms of the
 * GNU General Public License ("GPL") version 2 as published by the Free
 * Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "core.h"
<<<<<<< HEAD
#include "link.h"
#include "port.h"
#include "name_distr.h"
#include "discover.h"
#include "config.h"


/*
 * Out-of-range value for link session numbers
 */

#define INVALID_SESSION 0x10000

/*
 * Link state events:
 */

#define  STARTING_EVT    856384768	/* link processing trigger */
#define  TRAFFIC_MSG_EVT 560815u	/* rx'd ??? */
#define  TIMEOUT_EVT     560817u	/* link timer expired */

/*
 * The following two 'message types' is really just implementation
 * data conveniently stored in the message header.
 * They must not be considered part of the protocol
 */
#define OPEN_MSG   0
#define CLOSED_MSG 1

/*
 * State value stored in 'exp_msg_count'
 */

#define START_CHANGEOVER 100000u

/**
 * struct tipc_link_name - deconstructed link name
 * @addr_local: network address of node at this end
 * @if_local: name of interface at this end
 * @addr_peer: network address of node at far end
 * @if_peer: name of interface at far end
 */

struct tipc_link_name {
	u32 addr_local;
	char if_local[TIPC_MAX_IF_NAME];
	u32 addr_peer;
	char if_peer[TIPC_MAX_IF_NAME];
};

static void link_handle_out_of_seq_msg(struct tipc_link *l_ptr,
				       struct sk_buff *buf);
static void link_recv_proto_msg(struct tipc_link *l_ptr, struct sk_buff *buf);
static int  link_recv_changeover_msg(struct tipc_link **l_ptr,
				     struct sk_buff **buf);
static void link_set_supervision_props(struct tipc_link *l_ptr, u32 tolerance);
static int  link_send_sections_long(struct tipc_port *sender,
				    struct iovec const *msg_sect,
				    u32 num_sect, unsigned int total_len,
				    u32 destnode);
static void link_check_defragm_bufs(struct tipc_link *l_ptr);
static void link_state_event(struct tipc_link *l_ptr, u32 event);
static void link_reset_statistics(struct tipc_link *l_ptr);
static void link_print(struct tipc_link *l_ptr, const char *str);
static void link_start(struct tipc_link *l_ptr);
static int link_send_long_buf(struct tipc_link *l_ptr, struct sk_buff *buf);

/*
 *  Simple link routines
 */

static unsigned int align(unsigned int i)
{
	return (i + 3) & ~3u;
}

static void link_init_max_pkt(struct tipc_link *l_ptr)
{
	u32 max_pkt;

	max_pkt = (l_ptr->b_ptr->mtu & ~3);
	if (max_pkt > MAX_MSG_SIZE)
		max_pkt = MAX_MSG_SIZE;

	l_ptr->max_pkt_target = max_pkt;
	if (l_ptr->max_pkt_target < MAX_PKT_DEFAULT)
		l_ptr->max_pkt = l_ptr->max_pkt_target;
	else
		l_ptr->max_pkt = MAX_PKT_DEFAULT;

	l_ptr->max_pkt_probes = 0;
}

static u32 link_next_sent(struct tipc_link *l_ptr)
{
	if (l_ptr->next_out)
		return buf_seqno(l_ptr->next_out);
	return mod(l_ptr->next_out_no);
}

static u32 link_last_sent(struct tipc_link *l_ptr)
{
	return mod(link_next_sent(l_ptr) - 1);
}

/*
 *  Simple non-static link routines (i.e. referenced outside this file)
 */

int tipc_link_is_up(struct tipc_link *l_ptr)
{
	if (!l_ptr)
		return 0;
	return link_working_working(l_ptr) || link_working_unknown(l_ptr);
}

int tipc_link_is_active(struct tipc_link *l_ptr)
{
	return	(l_ptr->owner->active_links[0] == l_ptr) ||
		(l_ptr->owner->active_links[1] == l_ptr);
}

/**
 * link_name_validate - validate & (optionally) deconstruct tipc_link name
 * @name - ptr to link name string
 * @name_parts - ptr to area for link name components (or NULL if not needed)
 *
 * Returns 1 if link name is valid, otherwise 0.
 */

static int link_name_validate(const char *name,
				struct tipc_link_name *name_parts)
{
	char name_copy[TIPC_MAX_LINK_NAME];
	char *addr_local;
	char *if_local;
	char *addr_peer;
	char *if_peer;
	char dummy;
	u32 z_local, c_local, n_local;
	u32 z_peer, c_peer, n_peer;
	u32 if_local_len;
	u32 if_peer_len;

	/* copy link name & ensure length is OK */

	name_copy[TIPC_MAX_LINK_NAME - 1] = 0;
	/* need above in case non-Posix strncpy() doesn't pad with nulls */
	strncpy(name_copy, name, TIPC_MAX_LINK_NAME);
	if (name_copy[TIPC_MAX_LINK_NAME - 1] != 0)
		return 0;

	/* ensure all component parts of link name are present */

	addr_local = name_copy;
	if_local = strchr(addr_local, ':');
	if (if_local == NULL)
		return 0;
	*(if_local++) = 0;
	addr_peer = strchr(if_local, '-');
	if (addr_peer == NULL)
		return 0;
	*(addr_peer++) = 0;
	if_local_len = addr_peer - if_local;
	if_peer = strchr(addr_peer, ':');
	if (if_peer == NULL)
		return 0;
	*(if_peer++) = 0;
	if_peer_len = strlen(if_peer) + 1;

	/* validate component parts of link name */

	if ((sscanf(addr_local, "%u.%u.%u%c",
		    &z_local, &c_local, &n_local, &dummy) != 3) ||
	    (sscanf(addr_peer, "%u.%u.%u%c",
		    &z_peer, &c_peer, &n_peer, &dummy) != 3) ||
	    (z_local > 255) || (c_local > 4095) || (n_local > 4095) ||
	    (z_peer  > 255) || (c_peer  > 4095) || (n_peer  > 4095) ||
	    (if_local_len <= 1) || (if_local_len > TIPC_MAX_IF_NAME) ||
	    (if_peer_len  <= 1) || (if_peer_len  > TIPC_MAX_IF_NAME) ||
	    (strspn(if_local, tipc_alphabet) != (if_local_len - 1)) ||
	    (strspn(if_peer, tipc_alphabet) != (if_peer_len - 1)))
		return 0;

	/* return link name components, if necessary */

	if (name_parts) {
		name_parts->addr_local = tipc_addr(z_local, c_local, n_local);
		strcpy(name_parts->if_local, if_local);
		name_parts->addr_peer = tipc_addr(z_peer, c_peer, n_peer);
		strcpy(name_parts->if_peer, if_peer);
	}
	return 1;
}

/**
 * link_timeout - handle expiration of link timer
 * @l_ptr: pointer to link
 *
 * This routine must not grab "tipc_net_lock" to avoid a potential deadlock conflict
 * with tipc_link_delete().  (There is no risk that the node will be deleted by
 * another thread because tipc_link_delete() always cancels the link timer before
 * tipc_node_delete() is called.)
 */

static void link_timeout(struct tipc_link *l_ptr)
{
	tipc_node_lock(l_ptr->owner);

	/* update counters used in statistical profiling of send traffic */

	l_ptr->stats.accu_queue_sz += l_ptr->out_queue_size;
	l_ptr->stats.queue_sz_counts++;

	if (l_ptr->first_out) {
		struct tipc_msg *msg = buf_msg(l_ptr->first_out);
		u32 length = msg_size(msg);

		if ((msg_user(msg) == MSG_FRAGMENTER) &&
		    (msg_type(msg) == FIRST_FRAGMENT)) {
			length = msg_size(msg_get_wrapped(msg));
		}
		if (length) {
			l_ptr->stats.msg_lengths_total += length;
			l_ptr->stats.msg_length_counts++;
			if (length <= 64)
				l_ptr->stats.msg_length_profile[0]++;
			else if (length <= 256)
				l_ptr->stats.msg_length_profile[1]++;
			else if (length <= 1024)
				l_ptr->stats.msg_length_profile[2]++;
			else if (length <= 4096)
				l_ptr->stats.msg_length_profile[3]++;
			else if (length <= 16384)
				l_ptr->stats.msg_length_profile[4]++;
			else if (length <= 32768)
				l_ptr->stats.msg_length_profile[5]++;
			else
				l_ptr->stats.msg_length_profile[6]++;
		}
	}

	/* do all other link processing performed on a periodic basis */

	link_check_defragm_bufs(l_ptr);

	link_state_event(l_ptr, TIMEOUT_EVT);

	if (l_ptr->next_out)
		tipc_link_push_queue(l_ptr);

	tipc_node_unlock(l_ptr->owner);
}

static void link_set_timer(struct tipc_link *l_ptr, u32 time)
{
	k_start_timer(&l_ptr->timer, time);
=======
#include "subscr.h"
#include "link.h"
#include "bcast.h"
#include "socket.h"
#include "name_distr.h"
#include "discover.h"
#include "netlink.h"
#include "monitor.h"
#include "trace.h"
#include "crypto.h"

#include <linux/pkt_sched.h>

struct tipc_stats {
	u32 sent_pkts;
	u32 recv_pkts;
	u32 sent_states;
	u32 recv_states;
	u32 sent_probes;
	u32 recv_probes;
	u32 sent_nacks;
	u32 recv_nacks;
	u32 sent_acks;
	u32 sent_bundled;
	u32 sent_bundles;
	u32 recv_bundled;
	u32 recv_bundles;
	u32 retransmitted;
	u32 sent_fragmented;
	u32 sent_fragments;
	u32 recv_fragmented;
	u32 recv_fragments;
	u32 link_congs;		/* # port sends blocked by congestion */
	u32 deferred_recv;
	u32 duplicates;
	u32 max_queue_sz;	/* send queue size high water mark */
	u32 accu_queue_sz;	/* used for send queue size profiling */
	u32 queue_sz_counts;	/* used for send queue size profiling */
	u32 msg_length_counts;	/* used for message length profiling */
	u32 msg_lengths_total;	/* used for message length profiling */
	u32 msg_length_profile[7]; /* used for msg. length profiling */
};

/**
 * struct tipc_link - TIPC link data structure
 * @addr: network address of link's peer node
 * @name: link name character string
 * @net: pointer to namespace struct
 * @peer_session: link session # being used by peer end of link
 * @peer_bearer_id: bearer id used by link's peer endpoint
 * @bearer_id: local bearer id used by link
 * @tolerance: minimum link continuity loss needed to reset link [in ms]
 * @abort_limit: # of unacknowledged continuity probes needed to reset link
 * @state: current state of link FSM
 * @peer_caps: bitmap describing capabilities of peer node
 * @silent_intv_cnt: # of timer intervals without any reception from peer
 * @priority: current link priority
 * @net_plane: current link network plane ('A' through 'H')
 * @mon_state: cookie with information needed by link monitor
 * @mtu: current maximum packet size for this link
 * @advertised_mtu: advertised own mtu when link is being established
 * @backlogq: queue for messages waiting to be sent
 * @ackers: # of peers that needs to ack each packet before it can be released
 * @acked: # last packet acked by a certain peer. Used for broadcast.
 * @rcv_nxt: next sequence number to expect for inbound messages
 * @inputq: buffer queue for messages to be delivered upwards
 * @namedq: buffer queue for name table messages to be delivered upwards
 * @wakeupq: linked list of wakeup msgs waiting for link congestion to abate
 * @reasm_buf: head of partially reassembled inbound message fragments
 * @stats: collects statistics regarding link activity
 * @session: session to be used by link
 * @snd_nxt_state: next send seq number
 * @rcv_nxt_state: next rcv seq number
 * @in_session: have received ACTIVATE_MSG from peer
 * @active: link is active
 * @if_name: associated interface name
 * @rst_cnt: link reset counter
 * @drop_point: seq number for failover handling (FIXME)
 * @failover_reasm_skb: saved failover msg ptr (FIXME)
 * @failover_deferdq: deferred message queue for failover processing (FIXME)
 * @transmq: the link's transmit queue
 * @backlog: link's backlog by priority (importance)
 * @snd_nxt: next sequence number to be used
 * @rcv_unacked: # messages read by user, but not yet acked back to peer
 * @deferdq: deferred receive queue
 * @window: sliding window size for congestion handling
 * @min_win: minimal send window to be used by link
 * @ssthresh: slow start threshold for congestion handling
 * @max_win: maximal send window to be used by link
 * @cong_acks: congestion acks for congestion avoidance (FIXME)
 * @checkpoint: seq number for congestion window size handling
 * @reasm_tnlmsg: fragmentation/reassembly area for tunnel protocol message
 * @last_gap: last gap ack blocks for bcast (FIXME)
 * @last_ga: ptr to gap ack blocks
 * @bc_rcvlink: the peer specific link used for broadcast reception
 * @bc_sndlink: the namespace global link used for broadcast sending
 * @nack_state: bcast nack state
 * @bc_peer_is_up: peer has acked the bcast init msg
 */
struct tipc_link {
	u32 addr;
	char name[TIPC_MAX_LINK_NAME];
	struct net *net;

	/* Management and link supervision data */
	u16 peer_session;
	u16 session;
	u16 snd_nxt_state;
	u16 rcv_nxt_state;
	u32 peer_bearer_id;
	u32 bearer_id;
	u32 tolerance;
	u32 abort_limit;
	u32 state;
	u16 peer_caps;
	bool in_session;
	bool active;
	u32 silent_intv_cnt;
	char if_name[TIPC_MAX_IF_NAME];
	u32 priority;
	char net_plane;
	struct tipc_mon_state mon_state;
	u16 rst_cnt;

	/* Failover/synch */
	u16 drop_point;
	struct sk_buff *failover_reasm_skb;
	struct sk_buff_head failover_deferdq;

	/* Max packet negotiation */
	u16 mtu;
	u16 advertised_mtu;

	/* Sending */
	struct sk_buff_head transmq;
	struct sk_buff_head backlogq;
	struct {
		u16 len;
		u16 limit;
		struct sk_buff *target_bskb;
	} backlog[5];
	u16 snd_nxt;

	/* Reception */
	u16 rcv_nxt;
	u32 rcv_unacked;
	struct sk_buff_head deferdq;
	struct sk_buff_head *inputq;
	struct sk_buff_head *namedq;

	/* Congestion handling */
	struct sk_buff_head wakeupq;
	u16 window;
	u16 min_win;
	u16 ssthresh;
	u16 max_win;
	u16 cong_acks;
	u16 checkpoint;

	/* Fragmentation/reassembly */
	struct sk_buff *reasm_buf;
	struct sk_buff *reasm_tnlmsg;

	/* Broadcast */
	u16 ackers;
	u16 acked;
	u16 last_gap;
	struct tipc_gap_ack_blks *last_ga;
	struct tipc_link *bc_rcvlink;
	struct tipc_link *bc_sndlink;
	u8 nack_state;
	bool bc_peer_is_up;

	/* Statistics */
	struct tipc_stats stats;
};

/*
 * Error message prefixes
 */
static const char *link_co_err = "Link tunneling error, ";
static const char *link_rst_msg = "Resetting link ";

/* Send states for broadcast NACKs
 */
enum {
	BC_NACK_SND_CONDITIONAL,
	BC_NACK_SND_UNCONDITIONAL,
	BC_NACK_SND_SUPPRESS,
};

#define TIPC_BC_RETR_LIM  (jiffies + msecs_to_jiffies(10))
#define TIPC_UC_RETR_TIME (jiffies + msecs_to_jiffies(1))

/* Link FSM states:
 */
enum {
	LINK_ESTABLISHED     = 0xe,
	LINK_ESTABLISHING    = 0xe  << 4,
	LINK_RESET           = 0x1  << 8,
	LINK_RESETTING       = 0x2  << 12,
	LINK_PEER_RESET      = 0xd  << 16,
	LINK_FAILINGOVER     = 0xf  << 20,
	LINK_SYNCHING        = 0xc  << 24
};

/* Link FSM state checking routines
 */
static int link_is_up(struct tipc_link *l)
{
	return l->state & (LINK_ESTABLISHED | LINK_SYNCHING);
}

static int tipc_link_proto_rcv(struct tipc_link *l, struct sk_buff *skb,
			       struct sk_buff_head *xmitq);
static void tipc_link_build_proto_msg(struct tipc_link *l, int mtyp, bool probe,
				      bool probe_reply, u16 rcvgap,
				      int tolerance, int priority,
				      struct sk_buff_head *xmitq);
static void link_print(struct tipc_link *l, const char *str);
static int tipc_link_build_nack_msg(struct tipc_link *l,
				    struct sk_buff_head *xmitq);
static void tipc_link_build_bc_init_msg(struct tipc_link *l,
					struct sk_buff_head *xmitq);
static u8 __tipc_build_gap_ack_blks(struct tipc_gap_ack_blks *ga,
				    struct tipc_link *l, u8 start_index);
static u16 tipc_build_gap_ack_blks(struct tipc_link *l, struct tipc_msg *hdr);
static int tipc_link_advance_transmq(struct tipc_link *l, struct tipc_link *r,
				     u16 acked, u16 gap,
				     struct tipc_gap_ack_blks *ga,
				     struct sk_buff_head *xmitq,
				     bool *retransmitted, int *rc);
static void tipc_link_update_cwin(struct tipc_link *l, int released,
				  bool retransmitted);
/*
 *  Simple non-static link routines (i.e. referenced outside this file)
 */
bool tipc_link_is_up(struct tipc_link *l)
{
	return link_is_up(l);
}

bool tipc_link_peer_is_down(struct tipc_link *l)
{
	return l->state == LINK_PEER_RESET;
}

bool tipc_link_is_reset(struct tipc_link *l)
{
	return l->state & (LINK_RESET | LINK_FAILINGOVER | LINK_ESTABLISHING);
}

bool tipc_link_is_establishing(struct tipc_link *l)
{
	return l->state == LINK_ESTABLISHING;
}

bool tipc_link_is_synching(struct tipc_link *l)
{
	return l->state == LINK_SYNCHING;
}

bool tipc_link_is_failingover(struct tipc_link *l)
{
	return l->state == LINK_FAILINGOVER;
}

bool tipc_link_is_blocked(struct tipc_link *l)
{
	return l->state & (LINK_RESETTING | LINK_PEER_RESET | LINK_FAILINGOVER);
}

static bool link_is_bc_sndlink(struct tipc_link *l)
{
	return !l->bc_sndlink;
}

static bool link_is_bc_rcvlink(struct tipc_link *l)
{
	return ((l->bc_rcvlink == l) && !link_is_bc_sndlink(l));
}

void tipc_link_set_active(struct tipc_link *l, bool active)
{
	l->active = active;
}

u32 tipc_link_id(struct tipc_link *l)
{
	return l->peer_bearer_id << 16 | l->bearer_id;
}

int tipc_link_min_win(struct tipc_link *l)
{
	return l->min_win;
}

int tipc_link_max_win(struct tipc_link *l)
{
	return l->max_win;
}

int tipc_link_prio(struct tipc_link *l)
{
	return l->priority;
}

unsigned long tipc_link_tolerance(struct tipc_link *l)
{
	return l->tolerance;
}

struct sk_buff_head *tipc_link_inputq(struct tipc_link *l)
{
	return l->inputq;
}

char tipc_link_plane(struct tipc_link *l)
{
	return l->net_plane;
}

struct net *tipc_link_net(struct tipc_link *l)
{
	return l->net;
}

void tipc_link_update_caps(struct tipc_link *l, u16 capabilities)
{
	l->peer_caps = capabilities;
}

void tipc_link_add_bc_peer(struct tipc_link *snd_l,
			   struct tipc_link *uc_l,
			   struct sk_buff_head *xmitq)
{
	struct tipc_link *rcv_l = uc_l->bc_rcvlink;

	snd_l->ackers++;
	rcv_l->acked = snd_l->snd_nxt - 1;
	snd_l->state = LINK_ESTABLISHED;
	tipc_link_build_bc_init_msg(uc_l, xmitq);
}

void tipc_link_remove_bc_peer(struct tipc_link *snd_l,
			      struct tipc_link *rcv_l,
			      struct sk_buff_head *xmitq)
{
	u16 ack = snd_l->snd_nxt - 1;

	snd_l->ackers--;
	rcv_l->bc_peer_is_up = true;
	rcv_l->state = LINK_ESTABLISHED;
	tipc_link_bc_ack_rcv(rcv_l, ack, 0, NULL, xmitq, NULL);
	trace_tipc_link_reset(rcv_l, TIPC_DUMP_ALL, "bclink removed!");
	tipc_link_reset(rcv_l);
	rcv_l->state = LINK_RESET;
	if (!snd_l->ackers) {
		trace_tipc_link_reset(snd_l, TIPC_DUMP_ALL, "zero ackers!");
		tipc_link_reset(snd_l);
		snd_l->state = LINK_RESET;
		__skb_queue_purge(xmitq);
	}
}

int tipc_link_bc_peers(struct tipc_link *l)
{
	return l->ackers;
}

static u16 link_bc_rcv_gap(struct tipc_link *l)
{
	struct sk_buff *skb = skb_peek(&l->deferdq);
	u16 gap = 0;

	if (more(l->snd_nxt, l->rcv_nxt))
		gap = l->snd_nxt - l->rcv_nxt;
	if (skb)
		gap = buf_seqno(skb) - l->rcv_nxt;
	return gap;
}

void tipc_link_set_mtu(struct tipc_link *l, int mtu)
{
	l->mtu = mtu;
}

int tipc_link_mtu(struct tipc_link *l)
{
	return l->mtu;
}

int tipc_link_mss(struct tipc_link *l)
{
#ifdef CONFIG_TIPC_CRYPTO
	return l->mtu - INT_H_SIZE - EMSG_OVERHEAD;
#else
	return l->mtu - INT_H_SIZE;
#endif
}

u16 tipc_link_rcv_nxt(struct tipc_link *l)
{
	return l->rcv_nxt;
}

u16 tipc_link_acked(struct tipc_link *l)
{
	return l->acked;
}

char *tipc_link_name(struct tipc_link *l)
{
	return l->name;
}

u32 tipc_link_state(struct tipc_link *l)
{
	return l->state;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * tipc_link_create - create a new link
<<<<<<< HEAD
 * @n_ptr: pointer to associated node
 * @b_ptr: pointer to associated bearer
 * @media_addr: media address to use when sending messages over link
 *
 * Returns pointer to link.
 */

struct tipc_link *tipc_link_create(struct tipc_node *n_ptr,
			      struct tipc_bearer *b_ptr,
			      const struct tipc_media_addr *media_addr)
{
	struct tipc_link *l_ptr;
	struct tipc_msg *msg;
	char *if_name;
	char addr_string[16];
	u32 peer = n_ptr->addr;

	if (n_ptr->link_cnt >= 2) {
		tipc_addr_string_fill(addr_string, n_ptr->addr);
		err("Attempt to establish third link to %s\n", addr_string);
		return NULL;
	}

	if (n_ptr->links[b_ptr->identity]) {
		tipc_addr_string_fill(addr_string, n_ptr->addr);
		err("Attempt to establish second link on <%s> to %s\n",
		    b_ptr->name, addr_string);
		return NULL;
	}

	l_ptr = kzalloc(sizeof(*l_ptr), GFP_ATOMIC);
	if (!l_ptr) {
		warn("Link creation failed, no memory\n");
		return NULL;
	}

	l_ptr->addr = peer;
	if_name = strchr(b_ptr->name, ':') + 1;
	sprintf(l_ptr->name, "%u.%u.%u:%s-%u.%u.%u:unknown",
		tipc_zone(tipc_own_addr), tipc_cluster(tipc_own_addr),
		tipc_node(tipc_own_addr),
		if_name,
		tipc_zone(peer), tipc_cluster(peer), tipc_node(peer));
		/* note: peer i/f name is updated by reset/activate message */
	memcpy(&l_ptr->media_addr, media_addr, sizeof(*media_addr));
	l_ptr->owner = n_ptr;
	l_ptr->checkpoint = 1;
	l_ptr->peer_session = INVALID_SESSION;
	l_ptr->b_ptr = b_ptr;
	link_set_supervision_props(l_ptr, b_ptr->tolerance);
	l_ptr->state = RESET_UNKNOWN;

	l_ptr->pmsg = (struct tipc_msg *)&l_ptr->proto_msg;
	msg = l_ptr->pmsg;
	tipc_msg_init(msg, LINK_PROTOCOL, RESET_MSG, INT_H_SIZE, l_ptr->addr);
	msg_set_size(msg, sizeof(l_ptr->proto_msg));
	msg_set_session(msg, (tipc_random & 0xffff));
	msg_set_bearer_id(msg, b_ptr->identity);
	strcpy((char *)msg_data(msg), if_name);

	l_ptr->priority = b_ptr->priority;
	tipc_link_set_queue_limits(l_ptr, b_ptr->window);

	link_init_max_pkt(l_ptr);

	l_ptr->next_out_no = 1;
	INIT_LIST_HEAD(&l_ptr->waiting_ports);

	link_reset_statistics(l_ptr);

	tipc_node_attach_link(n_ptr, l_ptr);

	k_init_timer(&l_ptr->timer, (Handler)link_timeout, (unsigned long)l_ptr);
	list_add_tail(&l_ptr->link_list, &b_ptr->links);
	tipc_k_signal((Handler)link_start, (unsigned long)l_ptr);

	return l_ptr;
}

/**
 * tipc_link_delete - delete a link
 * @l_ptr: pointer to link
 *
 * Note: 'tipc_net_lock' is write_locked, bearer is locked.
 * This routine must not grab the node lock until after link timer cancellation
 * to avoid a potential deadlock situation.
 */

void tipc_link_delete(struct tipc_link *l_ptr)
{
	if (!l_ptr) {
		err("Attempt to delete non-existent link\n");
		return;
	}

	k_cancel_timer(&l_ptr->timer);

	tipc_node_lock(l_ptr->owner);
	tipc_link_reset(l_ptr);
	tipc_node_detach_link(l_ptr->owner, l_ptr);
	tipc_link_stop(l_ptr);
	list_del_init(&l_ptr->link_list);
	tipc_node_unlock(l_ptr->owner);
	k_term_timer(&l_ptr->timer);
	kfree(l_ptr);
}

static void link_start(struct tipc_link *l_ptr)
{
	tipc_node_lock(l_ptr->owner);
	link_state_event(l_ptr, STARTING_EVT);
	tipc_node_unlock(l_ptr->owner);
}

/**
 * link_schedule_port - schedule port for deferred sending
 * @l_ptr: pointer to link
 * @origport: reference to sending port
 * @sz: amount of data to be sent
 *
 * Schedules port for renewed sending of messages after link congestion
 * has abated.
 */

static int link_schedule_port(struct tipc_link *l_ptr, u32 origport, u32 sz)
{
	struct tipc_port *p_ptr;

	spin_lock_bh(&tipc_port_list_lock);
	p_ptr = tipc_port_lock(origport);
	if (p_ptr) {
		if (!p_ptr->wakeup)
			goto exit;
		if (!list_empty(&p_ptr->wait_list))
			goto exit;
		p_ptr->congested = 1;
		p_ptr->waiting_pkts = 1 + ((sz - 1) / l_ptr->max_pkt);
		list_add_tail(&p_ptr->wait_list, &l_ptr->waiting_ports);
		l_ptr->stats.link_congs++;
exit:
		tipc_port_unlock(p_ptr);
	}
	spin_unlock_bh(&tipc_port_list_lock);
	return -ELINKCONG;
}

void tipc_link_wakeup_ports(struct tipc_link *l_ptr, int all)
{
	struct tipc_port *p_ptr;
	struct tipc_port *temp_p_ptr;
	int win = l_ptr->queue_limit[0] - l_ptr->out_queue_size;

	if (all)
		win = 100000;
	if (win <= 0)
		return;
	if (!spin_trylock_bh(&tipc_port_list_lock))
		return;
	if (link_congested(l_ptr))
		goto exit;
	list_for_each_entry_safe(p_ptr, temp_p_ptr, &l_ptr->waiting_ports,
				 wait_list) {
		if (win <= 0)
			break;
		list_del_init(&p_ptr->wait_list);
		spin_lock_bh(p_ptr->lock);
		p_ptr->congested = 0;
		p_ptr->wakeup(p_ptr);
		win -= p_ptr->waiting_pkts;
		spin_unlock_bh(p_ptr->lock);
	}

exit:
	spin_unlock_bh(&tipc_port_list_lock);
}

/**
 * link_release_outqueue - purge link's outbound message queue
 * @l_ptr: pointer to link
 */

static void link_release_outqueue(struct tipc_link *l_ptr)
{
	struct sk_buff *buf = l_ptr->first_out;
	struct sk_buff *next;

	while (buf) {
		next = buf->next;
		kfree_skb(buf);
		buf = next;
	}
	l_ptr->first_out = NULL;
	l_ptr->out_queue_size = 0;
}

/**
 * tipc_link_reset_fragments - purge link's inbound message fragments queue
 * @l_ptr: pointer to link
 */

void tipc_link_reset_fragments(struct tipc_link *l_ptr)
{
	struct sk_buff *buf = l_ptr->defragm_buf;
	struct sk_buff *next;

	while (buf) {
		next = buf->next;
		kfree_skb(buf);
		buf = next;
	}
	l_ptr->defragm_buf = NULL;
}

/**
 * tipc_link_stop - purge all inbound and outbound messages associated with link
 * @l_ptr: pointer to link
 */

void tipc_link_stop(struct tipc_link *l_ptr)
{
	struct sk_buff *buf;
	struct sk_buff *next;

	buf = l_ptr->oldest_deferred_in;
	while (buf) {
		next = buf->next;
		kfree_skb(buf);
		buf = next;
	}

	buf = l_ptr->first_out;
	while (buf) {
		next = buf->next;
		kfree_skb(buf);
		buf = next;
	}

	tipc_link_reset_fragments(l_ptr);

	kfree_skb(l_ptr->proto_msg_queue);
	l_ptr->proto_msg_queue = NULL;
}

void tipc_link_reset(struct tipc_link *l_ptr)
{
	struct sk_buff *buf;
	u32 prev_state = l_ptr->state;
	u32 checkpoint = l_ptr->next_in_no;
	int was_active_link = tipc_link_is_active(l_ptr);

	msg_set_session(l_ptr->pmsg, ((msg_session(l_ptr->pmsg) + 1) & 0xffff));

	/* Link is down, accept any session */
	l_ptr->peer_session = INVALID_SESSION;

	/* Prepare for max packet size negotiation */
	link_init_max_pkt(l_ptr);

	l_ptr->state = RESET_UNKNOWN;

	if ((prev_state == RESET_UNKNOWN) || (prev_state == RESET_RESET))
		return;

	tipc_node_link_down(l_ptr->owner, l_ptr);
	tipc_bearer_remove_dest(l_ptr->b_ptr, l_ptr->addr);

	if (was_active_link && tipc_node_active_links(l_ptr->owner) &&
	    l_ptr->owner->permit_changeover) {
		l_ptr->reset_checkpoint = checkpoint;
		l_ptr->exp_msg_count = START_CHANGEOVER;
	}

	/* Clean up all queues: */

	link_release_outqueue(l_ptr);
	kfree_skb(l_ptr->proto_msg_queue);
	l_ptr->proto_msg_queue = NULL;
	buf = l_ptr->oldest_deferred_in;
	while (buf) {
		struct sk_buff *next = buf->next;
		kfree_skb(buf);
		buf = next;
	}
	if (!list_empty(&l_ptr->waiting_ports))
		tipc_link_wakeup_ports(l_ptr, 1);

	l_ptr->retransm_queue_head = 0;
	l_ptr->retransm_queue_size = 0;
	l_ptr->last_out = NULL;
	l_ptr->first_out = NULL;
	l_ptr->next_out = NULL;
	l_ptr->unacked_window = 0;
	l_ptr->checkpoint = 1;
	l_ptr->next_out_no = 1;
	l_ptr->deferred_inqueue_sz = 0;
	l_ptr->oldest_deferred_in = NULL;
	l_ptr->newest_deferred_in = NULL;
	l_ptr->fsm_msg_cnt = 0;
	l_ptr->stale_count = 0;
	link_reset_statistics(l_ptr);
}


static void link_activate(struct tipc_link *l_ptr)
{
	l_ptr->next_in_no = l_ptr->stats.recv_info = 1;
	tipc_node_link_up(l_ptr->owner, l_ptr);
	tipc_bearer_add_dest(l_ptr->b_ptr, l_ptr->addr);
}

/**
 * link_state_event - link finite state machine
 * @l_ptr: pointer to link
 * @event: state machine event to process
 */

static void link_state_event(struct tipc_link *l_ptr, unsigned event)
{
	struct tipc_link *other;
	u32 cont_intv = l_ptr->continuity_interval;

	if (!l_ptr->started && (event != STARTING_EVT))
		return;		/* Not yet. */

	if (link_blocked(l_ptr)) {
		if (event == TIMEOUT_EVT)
			link_set_timer(l_ptr, cont_intv);
		return;	  /* Changeover going on */
	}

	switch (l_ptr->state) {
	case WORKING_WORKING:
		switch (event) {
		case TRAFFIC_MSG_EVT:
		case ACTIVATE_MSG:
			break;
		case TIMEOUT_EVT:
			if (l_ptr->next_in_no != l_ptr->checkpoint) {
				l_ptr->checkpoint = l_ptr->next_in_no;
				if (tipc_bclink_acks_missing(l_ptr->owner)) {
					tipc_link_send_proto_msg(l_ptr, STATE_MSG,
								 0, 0, 0, 0, 0);
					l_ptr->fsm_msg_cnt++;
				} else if (l_ptr->max_pkt < l_ptr->max_pkt_target) {
					tipc_link_send_proto_msg(l_ptr, STATE_MSG,
								 1, 0, 0, 0, 0);
					l_ptr->fsm_msg_cnt++;
				}
				link_set_timer(l_ptr, cont_intv);
				break;
			}
			l_ptr->state = WORKING_UNKNOWN;
			l_ptr->fsm_msg_cnt = 0;
			tipc_link_send_proto_msg(l_ptr, STATE_MSG, 1, 0, 0, 0, 0);
			l_ptr->fsm_msg_cnt++;
			link_set_timer(l_ptr, cont_intv / 4);
			break;
		case RESET_MSG:
			info("Resetting link <%s>, requested by peer\n",
			     l_ptr->name);
			tipc_link_reset(l_ptr);
			l_ptr->state = RESET_RESET;
			l_ptr->fsm_msg_cnt = 0;
			tipc_link_send_proto_msg(l_ptr, ACTIVATE_MSG, 0, 0, 0, 0, 0);
			l_ptr->fsm_msg_cnt++;
			link_set_timer(l_ptr, cont_intv);
			break;
		default:
			err("Unknown link event %u in WW state\n", event);
		}
		break;
	case WORKING_UNKNOWN:
		switch (event) {
		case TRAFFIC_MSG_EVT:
		case ACTIVATE_MSG:
			l_ptr->state = WORKING_WORKING;
			l_ptr->fsm_msg_cnt = 0;
			link_set_timer(l_ptr, cont_intv);
			break;
		case RESET_MSG:
			info("Resetting link <%s>, requested by peer "
			     "while probing\n", l_ptr->name);
			tipc_link_reset(l_ptr);
			l_ptr->state = RESET_RESET;
			l_ptr->fsm_msg_cnt = 0;
			tipc_link_send_proto_msg(l_ptr, ACTIVATE_MSG, 0, 0, 0, 0, 0);
			l_ptr->fsm_msg_cnt++;
			link_set_timer(l_ptr, cont_intv);
			break;
		case TIMEOUT_EVT:
			if (l_ptr->next_in_no != l_ptr->checkpoint) {
				l_ptr->state = WORKING_WORKING;
				l_ptr->fsm_msg_cnt = 0;
				l_ptr->checkpoint = l_ptr->next_in_no;
				if (tipc_bclink_acks_missing(l_ptr->owner)) {
					tipc_link_send_proto_msg(l_ptr, STATE_MSG,
								 0, 0, 0, 0, 0);
					l_ptr->fsm_msg_cnt++;
				}
				link_set_timer(l_ptr, cont_intv);
			} else if (l_ptr->fsm_msg_cnt < l_ptr->abort_limit) {
				tipc_link_send_proto_msg(l_ptr, STATE_MSG,
							 1, 0, 0, 0, 0);
				l_ptr->fsm_msg_cnt++;
				link_set_timer(l_ptr, cont_intv / 4);
			} else {	/* Link has failed */
				warn("Resetting link <%s>, peer not responding\n",
				     l_ptr->name);
				tipc_link_reset(l_ptr);
				l_ptr->state = RESET_UNKNOWN;
				l_ptr->fsm_msg_cnt = 0;
				tipc_link_send_proto_msg(l_ptr, RESET_MSG,
							 0, 0, 0, 0, 0);
				l_ptr->fsm_msg_cnt++;
				link_set_timer(l_ptr, cont_intv);
			}
			break;
		default:
			err("Unknown link event %u in WU state\n", event);
		}
		break;
	case RESET_UNKNOWN:
		switch (event) {
		case TRAFFIC_MSG_EVT:
			break;
		case ACTIVATE_MSG:
			other = l_ptr->owner->active_links[0];
			if (other && link_working_unknown(other))
				break;
			l_ptr->state = WORKING_WORKING;
			l_ptr->fsm_msg_cnt = 0;
			link_activate(l_ptr);
			tipc_link_send_proto_msg(l_ptr, STATE_MSG, 1, 0, 0, 0, 0);
			l_ptr->fsm_msg_cnt++;
			link_set_timer(l_ptr, cont_intv);
			break;
		case RESET_MSG:
			l_ptr->state = RESET_RESET;
			l_ptr->fsm_msg_cnt = 0;
			tipc_link_send_proto_msg(l_ptr, ACTIVATE_MSG, 1, 0, 0, 0, 0);
			l_ptr->fsm_msg_cnt++;
			link_set_timer(l_ptr, cont_intv);
			break;
		case STARTING_EVT:
			l_ptr->started = 1;
			/* fall through */
		case TIMEOUT_EVT:
			tipc_link_send_proto_msg(l_ptr, RESET_MSG, 0, 0, 0, 0, 0);
			l_ptr->fsm_msg_cnt++;
			link_set_timer(l_ptr, cont_intv);
			break;
		default:
			err("Unknown link event %u in RU state\n", event);
		}
		break;
	case RESET_RESET:
		switch (event) {
		case TRAFFIC_MSG_EVT:
		case ACTIVATE_MSG:
			other = l_ptr->owner->active_links[0];
			if (other && link_working_unknown(other))
				break;
			l_ptr->state = WORKING_WORKING;
			l_ptr->fsm_msg_cnt = 0;
			link_activate(l_ptr);
			tipc_link_send_proto_msg(l_ptr, STATE_MSG, 1, 0, 0, 0, 0);
			l_ptr->fsm_msg_cnt++;
			link_set_timer(l_ptr, cont_intv);
			break;
		case RESET_MSG:
			break;
		case TIMEOUT_EVT:
			tipc_link_send_proto_msg(l_ptr, ACTIVATE_MSG, 0, 0, 0, 0, 0);
			l_ptr->fsm_msg_cnt++;
			link_set_timer(l_ptr, cont_intv);
			break;
		default:
			err("Unknown link event %u in RR state\n", event);
		}
		break;
	default:
		err("Unknown link state %u/%u\n", l_ptr->state, event);
	}
}

/*
 * link_bundle_buf(): Append contents of a buffer to
 * the tail of an existing one.
 */

static int link_bundle_buf(struct tipc_link *l_ptr,
			   struct sk_buff *bundler,
			   struct sk_buff *buf)
{
	struct tipc_msg *bundler_msg = buf_msg(bundler);
	struct tipc_msg *msg = buf_msg(buf);
	u32 size = msg_size(msg);
	u32 bundle_size = msg_size(bundler_msg);
	u32 to_pos = align(bundle_size);
	u32 pad = to_pos - bundle_size;

	if (msg_user(bundler_msg) != MSG_BUNDLER)
		return 0;
	if (msg_type(bundler_msg) != OPEN_MSG)
		return 0;
	if (skb_tailroom(bundler) < (pad + size))
		return 0;
	if (l_ptr->max_pkt < (to_pos + size))
		return 0;

	skb_put(bundler, pad + size);
	skb_copy_to_linear_data_offset(bundler, to_pos, buf->data, size);
	msg_set_size(bundler_msg, to_pos + size);
	msg_set_msgcnt(bundler_msg, msg_msgcnt(bundler_msg) + 1);
	kfree_skb(buf);
	l_ptr->stats.sent_bundled++;
	return 1;
}

static void link_add_to_outqueue(struct tipc_link *l_ptr,
				 struct sk_buff *buf,
				 struct tipc_msg *msg)
{
	u32 ack = mod(l_ptr->next_in_no - 1);
	u32 seqno = mod(l_ptr->next_out_no++);

	msg_set_word(msg, 2, ((ack << 16) | seqno));
	msg_set_bcast_ack(msg, l_ptr->owner->bclink.last_in);
	buf->next = NULL;
	if (l_ptr->first_out) {
		l_ptr->last_out->next = buf;
		l_ptr->last_out = buf;
	} else
		l_ptr->first_out = l_ptr->last_out = buf;

	l_ptr->out_queue_size++;
	if (l_ptr->out_queue_size > l_ptr->stats.max_queue_sz)
		l_ptr->stats.max_queue_sz = l_ptr->out_queue_size;
}

static void link_add_chain_to_outqueue(struct tipc_link *l_ptr,
				       struct sk_buff *buf_chain,
				       u32 long_msgno)
{
	struct sk_buff *buf;
	struct tipc_msg *msg;

	if (!l_ptr->next_out)
		l_ptr->next_out = buf_chain;
	while (buf_chain) {
		buf = buf_chain;
		buf_chain = buf_chain->next;

		msg = buf_msg(buf);
		msg_set_long_msgno(msg, long_msgno);
		link_add_to_outqueue(l_ptr, buf, msg);
	}
}

/*
 * tipc_link_send_buf() is the 'full path' for messages, called from
 * inside TIPC when the 'fast path' in tipc_send_buf
 * has failed, and from link_send()
 */

int tipc_link_send_buf(struct tipc_link *l_ptr, struct sk_buff *buf)
{
	struct tipc_msg *msg = buf_msg(buf);
	u32 size = msg_size(msg);
	u32 dsz = msg_data_sz(msg);
	u32 queue_size = l_ptr->out_queue_size;
	u32 imp = tipc_msg_tot_importance(msg);
	u32 queue_limit = l_ptr->queue_limit[imp];
	u32 max_packet = l_ptr->max_pkt;

	/* Match msg importance against queue limits: */

	if (unlikely(queue_size >= queue_limit)) {
		if (imp <= TIPC_CRITICAL_IMPORTANCE) {
			link_schedule_port(l_ptr, msg_origport(msg), size);
			kfree_skb(buf);
			return -ELINKCONG;
		}
		kfree_skb(buf);
		if (imp > CONN_MANAGER) {
			warn("Resetting link <%s>, send queue full", l_ptr->name);
			tipc_link_reset(l_ptr);
		}
		return dsz;
	}

	/* Fragmentation needed ? */

	if (size > max_packet)
		return link_send_long_buf(l_ptr, buf);

	/* Packet can be queued or sent: */

	if (likely(!tipc_bearer_congested(l_ptr->b_ptr, l_ptr) &&
		   !link_congested(l_ptr))) {
		link_add_to_outqueue(l_ptr, buf, msg);

		if (likely(tipc_bearer_send(l_ptr->b_ptr, buf, &l_ptr->media_addr))) {
			l_ptr->unacked_window = 0;
		} else {
			tipc_bearer_schedule(l_ptr->b_ptr, l_ptr);
			l_ptr->stats.bearer_congs++;
			l_ptr->next_out = buf;
		}
		return dsz;
	}
	/* Congestion: can message be bundled ?: */

	if ((msg_user(msg) != CHANGEOVER_PROTOCOL) &&
	    (msg_user(msg) != MSG_FRAGMENTER)) {

		/* Try adding message to an existing bundle */

		if (l_ptr->next_out &&
		    link_bundle_buf(l_ptr, l_ptr->last_out, buf)) {
			tipc_bearer_resolve_congestion(l_ptr->b_ptr, l_ptr);
			return dsz;
		}

		/* Try creating a new bundle */

		if (size <= max_packet * 2 / 3) {
			struct sk_buff *bundler = tipc_buf_acquire(max_packet);
			struct tipc_msg bundler_hdr;

			if (bundler) {
				tipc_msg_init(&bundler_hdr, MSG_BUNDLER, OPEN_MSG,
					 INT_H_SIZE, l_ptr->addr);
				skb_copy_to_linear_data(bundler, &bundler_hdr,
							INT_H_SIZE);
				skb_trim(bundler, INT_H_SIZE);
				link_bundle_buf(l_ptr, bundler, buf);
				buf = bundler;
				msg = buf_msg(buf);
				l_ptr->stats.sent_bundles++;
			}
		}
	}
	if (!l_ptr->next_out)
		l_ptr->next_out = buf;
	link_add_to_outqueue(l_ptr, buf, msg);
	tipc_bearer_resolve_congestion(l_ptr->b_ptr, l_ptr);
	return dsz;
}

/*
 * tipc_link_send(): same as tipc_link_send_buf(), but the link to use has
 * not been selected yet, and the the owner node is not locked
 * Called by TIPC internal users, e.g. the name distributor
 */

int tipc_link_send(struct sk_buff *buf, u32 dest, u32 selector)
{
	struct tipc_link *l_ptr;
	struct tipc_node *n_ptr;
	int res = -ELINKCONG;

	read_lock_bh(&tipc_net_lock);
	n_ptr = tipc_node_find(dest);
	if (n_ptr) {
		tipc_node_lock(n_ptr);
		l_ptr = n_ptr->active_links[selector & 1];
		if (l_ptr)
			res = tipc_link_send_buf(l_ptr, buf);
		else
			kfree_skb(buf);
		tipc_node_unlock(n_ptr);
	} else {
		kfree_skb(buf);
	}
	read_unlock_bh(&tipc_net_lock);
	return res;
}

/*
 * tipc_link_send_names - send name table entries to new neighbor
 *
 * Send routine for bulk delivery of name table messages when contact
 * with a new neighbor occurs. No link congestion checking is performed
 * because name table messages *must* be delivered. The messages must be
 * small enough not to require fragmentation.
 * Called without any locks held.
 */

void tipc_link_send_names(struct list_head *message_list, u32 dest)
{
	struct tipc_node *n_ptr;
	struct tipc_link *l_ptr;
	struct sk_buff *buf;
	struct sk_buff *temp_buf;

	if (list_empty(message_list))
		return;

	read_lock_bh(&tipc_net_lock);
	n_ptr = tipc_node_find(dest);
	if (n_ptr) {
		tipc_node_lock(n_ptr);
		l_ptr = n_ptr->active_links[0];
		if (l_ptr) {
			/* convert circular list to linear list */
			((struct sk_buff *)message_list->prev)->next = NULL;
			link_add_chain_to_outqueue(l_ptr,
				(struct sk_buff *)message_list->next, 0);
			tipc_link_push_queue(l_ptr);
			INIT_LIST_HEAD(message_list);
		}
		tipc_node_unlock(n_ptr);
	}
	read_unlock_bh(&tipc_net_lock);

	/* discard the messages if they couldn't be sent */

	list_for_each_safe(buf, temp_buf, ((struct sk_buff *)message_list)) {
		list_del((struct list_head *)buf);
		kfree_skb(buf);
	}
}

/*
 * link_send_buf_fast: Entry for data messages where the
 * destination link is known and the header is complete,
 * inclusive total message length. Very time critical.
 * Link is locked. Returns user data length.
 */

static int link_send_buf_fast(struct tipc_link *l_ptr, struct sk_buff *buf,
			      u32 *used_max_pkt)
{
	struct tipc_msg *msg = buf_msg(buf);
	int res = msg_data_sz(msg);

	if (likely(!link_congested(l_ptr))) {
		if (likely(msg_size(msg) <= l_ptr->max_pkt)) {
			if (likely(list_empty(&l_ptr->b_ptr->cong_links))) {
				link_add_to_outqueue(l_ptr, buf, msg);
				if (likely(tipc_bearer_send(l_ptr->b_ptr, buf,
							    &l_ptr->media_addr))) {
					l_ptr->unacked_window = 0;
					return res;
				}
				tipc_bearer_schedule(l_ptr->b_ptr, l_ptr);
				l_ptr->stats.bearer_congs++;
				l_ptr->next_out = buf;
				return res;
			}
		} else
			*used_max_pkt = l_ptr->max_pkt;
	}
	return tipc_link_send_buf(l_ptr, buf);  /* All other cases */
}

/*
 * tipc_send_buf_fast: Entry for data messages where the
 * destination node is known and the header is complete,
 * inclusive total message length.
 * Returns user data length.
 */
int tipc_send_buf_fast(struct sk_buff *buf, u32 destnode)
{
	struct tipc_link *l_ptr;
	struct tipc_node *n_ptr;
	int res;
	u32 selector = msg_origport(buf_msg(buf)) & 1;
	u32 dummy;

	read_lock_bh(&tipc_net_lock);
	n_ptr = tipc_node_find(destnode);
	if (likely(n_ptr)) {
		tipc_node_lock(n_ptr);
		l_ptr = n_ptr->active_links[selector];
		if (likely(l_ptr)) {
			res = link_send_buf_fast(l_ptr, buf, &dummy);
			tipc_node_unlock(n_ptr);
			read_unlock_bh(&tipc_net_lock);
			return res;
		}
		tipc_node_unlock(n_ptr);
	}
	read_unlock_bh(&tipc_net_lock);
	res = msg_data_sz(buf_msg(buf));
	tipc_reject_msg(buf, TIPC_ERR_NO_NODE);
	return res;
}


/*
 * tipc_link_send_sections_fast: Entry for messages where the
 * destination processor is known and the header is complete,
 * except for total message length.
 * Returns user data length or errno.
 */
int tipc_link_send_sections_fast(struct tipc_port *sender,
				 struct iovec const *msg_sect,
				 const u32 num_sect,
				 unsigned int total_len,
				 u32 destaddr)
{
	struct tipc_msg *hdr = &sender->phdr;
	struct tipc_link *l_ptr;
	struct sk_buff *buf;
	struct tipc_node *node;
	int res;
	u32 selector = msg_origport(hdr) & 1;

again:
	/*
	 * Try building message using port's max_pkt hint.
	 * (Must not hold any locks while building message.)
	 */

	res = tipc_msg_build(hdr, msg_sect, num_sect, total_len,
			     sender->max_pkt, !sender->user_port, &buf);

	read_lock_bh(&tipc_net_lock);
	node = tipc_node_find(destaddr);
	if (likely(node)) {
		tipc_node_lock(node);
		l_ptr = node->active_links[selector];
		if (likely(l_ptr)) {
			if (likely(buf)) {
				res = link_send_buf_fast(l_ptr, buf,
							 &sender->max_pkt);
exit:
				tipc_node_unlock(node);
				read_unlock_bh(&tipc_net_lock);
				return res;
			}

			/* Exit if build request was invalid */

			if (unlikely(res < 0))
				goto exit;

			/* Exit if link (or bearer) is congested */

			if (link_congested(l_ptr) ||
			    !list_empty(&l_ptr->b_ptr->cong_links)) {
				res = link_schedule_port(l_ptr,
							 sender->ref, res);
				goto exit;
			}

			/*
			 * Message size exceeds max_pkt hint; update hint,
			 * then re-try fast path or fragment the message
			 */

			sender->max_pkt = l_ptr->max_pkt;
			tipc_node_unlock(node);
			read_unlock_bh(&tipc_net_lock);


			if ((msg_hdr_sz(hdr) + res) <= sender->max_pkt)
				goto again;

			return link_send_sections_long(sender, msg_sect,
						       num_sect, total_len,
						       destaddr);
		}
		tipc_node_unlock(node);
	}
	read_unlock_bh(&tipc_net_lock);

	/* Couldn't find a link to the destination node */

	if (buf)
		return tipc_reject_msg(buf, TIPC_ERR_NO_NODE);
	if (res >= 0)
		return tipc_port_reject_sections(sender, hdr, msg_sect, num_sect,
						 total_len, TIPC_ERR_NO_NODE);
	return res;
}

/*
 * link_send_sections_long(): Entry for long messages where the
 * destination node is known and the header is complete,
 * inclusive total message length.
 * Link and bearer congestion status have been checked to be ok,
 * and are ignored if they change.
 *
 * Note that fragments do not use the full link MTU so that they won't have
 * to undergo refragmentation if link changeover causes them to be sent
 * over another link with an additional tunnel header added as prefix.
 * (Refragmentation will still occur if the other link has a smaller MTU.)
 *
 * Returns user data length or errno.
 */
static int link_send_sections_long(struct tipc_port *sender,
				   struct iovec const *msg_sect,
				   u32 num_sect,
				   unsigned int total_len,
				   u32 destaddr)
{
	struct tipc_link *l_ptr;
	struct tipc_node *node;
	struct tipc_msg *hdr = &sender->phdr;
	u32 dsz = total_len;
	u32 max_pkt, fragm_sz, rest;
	struct tipc_msg fragm_hdr;
	struct sk_buff *buf, *buf_chain, *prev;
	u32 fragm_crs, fragm_rest, hsz, sect_rest;
	const unchar *sect_crs;
	int curr_sect;
	u32 fragm_no;

again:
	fragm_no = 1;
	max_pkt = sender->max_pkt - INT_H_SIZE;
		/* leave room for tunnel header in case of link changeover */
	fragm_sz = max_pkt - INT_H_SIZE;
		/* leave room for fragmentation header in each fragment */
	rest = dsz;
	fragm_crs = 0;
	fragm_rest = 0;
	sect_rest = 0;
	sect_crs = NULL;
	curr_sect = -1;

	/* Prepare reusable fragment header: */

	tipc_msg_init(&fragm_hdr, MSG_FRAGMENTER, FIRST_FRAGMENT,
		 INT_H_SIZE, msg_destnode(hdr));
	msg_set_size(&fragm_hdr, max_pkt);
	msg_set_fragm_no(&fragm_hdr, 1);

	/* Prepare header of first fragment: */

	buf_chain = buf = tipc_buf_acquire(max_pkt);
	if (!buf)
		return -ENOMEM;
	buf->next = NULL;
	skb_copy_to_linear_data(buf, &fragm_hdr, INT_H_SIZE);
	hsz = msg_hdr_sz(hdr);
	skb_copy_to_linear_data_offset(buf, INT_H_SIZE, hdr, hsz);

	/* Chop up message: */

	fragm_crs = INT_H_SIZE + hsz;
	fragm_rest = fragm_sz - hsz;

	do {		/* For all sections */
		u32 sz;

		if (!sect_rest) {
			sect_rest = msg_sect[++curr_sect].iov_len;
			sect_crs = (const unchar *)msg_sect[curr_sect].iov_base;
		}

		if (sect_rest < fragm_rest)
			sz = sect_rest;
		else
			sz = fragm_rest;

		if (likely(!sender->user_port)) {
			if (copy_from_user(buf->data + fragm_crs, sect_crs, sz)) {
error:
				for (; buf_chain; buf_chain = buf) {
					buf = buf_chain->next;
					kfree_skb(buf_chain);
				}
				return -EFAULT;
			}
		} else
			skb_copy_to_linear_data_offset(buf, fragm_crs,
						       sect_crs, sz);
		sect_crs += sz;
		sect_rest -= sz;
		fragm_crs += sz;
		fragm_rest -= sz;
		rest -= sz;

		if (!fragm_rest && rest) {

			/* Initiate new fragment: */
			if (rest <= fragm_sz) {
				fragm_sz = rest;
				msg_set_type(&fragm_hdr, LAST_FRAGMENT);
			} else {
				msg_set_type(&fragm_hdr, FRAGMENT);
			}
			msg_set_size(&fragm_hdr, fragm_sz + INT_H_SIZE);
			msg_set_fragm_no(&fragm_hdr, ++fragm_no);
			prev = buf;
			buf = tipc_buf_acquire(fragm_sz + INT_H_SIZE);
			if (!buf)
				goto error;

			buf->next = NULL;
			prev->next = buf;
			skb_copy_to_linear_data(buf, &fragm_hdr, INT_H_SIZE);
			fragm_crs = INT_H_SIZE;
			fragm_rest = fragm_sz;
		}
	} while (rest > 0);

	/*
	 * Now we have a buffer chain. Select a link and check
	 * that packet size is still OK
	 */
	node = tipc_node_find(destaddr);
	if (likely(node)) {
		tipc_node_lock(node);
		l_ptr = node->active_links[sender->ref & 1];
		if (!l_ptr) {
			tipc_node_unlock(node);
			goto reject;
		}
		if (l_ptr->max_pkt < max_pkt) {
			sender->max_pkt = l_ptr->max_pkt;
			tipc_node_unlock(node);
			for (; buf_chain; buf_chain = buf) {
				buf = buf_chain->next;
				kfree_skb(buf_chain);
			}
			goto again;
		}
	} else {
reject:
		for (; buf_chain; buf_chain = buf) {
			buf = buf_chain->next;
			kfree_skb(buf_chain);
		}
		return tipc_port_reject_sections(sender, hdr, msg_sect, num_sect,
						 total_len, TIPC_ERR_NO_NODE);
	}

	/* Append chain of fragments to send queue & send them */

	l_ptr->long_msg_seq_no++;
	link_add_chain_to_outqueue(l_ptr, buf_chain, l_ptr->long_msg_seq_no);
	l_ptr->stats.sent_fragments += fragm_no;
	l_ptr->stats.sent_fragmented++;
	tipc_link_push_queue(l_ptr);
	tipc_node_unlock(node);
	return dsz;
}

/*
 * tipc_link_push_packet: Push one unsent packet to the media
 */
u32 tipc_link_push_packet(struct tipc_link *l_ptr)
{
	struct sk_buff *buf = l_ptr->first_out;
	u32 r_q_size = l_ptr->retransm_queue_size;
	u32 r_q_head = l_ptr->retransm_queue_head;

	/* Step to position where retransmission failed, if any,    */
	/* consider that buffers may have been released in meantime */

	if (r_q_size && buf) {
		u32 last = lesser(mod(r_q_head + r_q_size),
				  link_last_sent(l_ptr));
		u32 first = buf_seqno(buf);

		while (buf && less(first, r_q_head)) {
			first = mod(first + 1);
			buf = buf->next;
		}
		l_ptr->retransm_queue_head = r_q_head = first;
		l_ptr->retransm_queue_size = r_q_size = mod(last - first);
	}

	/* Continue retransmission now, if there is anything: */

	if (r_q_size && buf) {
		msg_set_ack(buf_msg(buf), mod(l_ptr->next_in_no - 1));
		msg_set_bcast_ack(buf_msg(buf), l_ptr->owner->bclink.last_in);
		if (tipc_bearer_send(l_ptr->b_ptr, buf, &l_ptr->media_addr)) {
			l_ptr->retransm_queue_head = mod(++r_q_head);
			l_ptr->retransm_queue_size = --r_q_size;
			l_ptr->stats.retransmitted++;
			return 0;
		} else {
			l_ptr->stats.bearer_congs++;
			return PUSH_FAILED;
		}
	}

	/* Send deferred protocol message, if any: */

	buf = l_ptr->proto_msg_queue;
	if (buf) {
		msg_set_ack(buf_msg(buf), mod(l_ptr->next_in_no - 1));
		msg_set_bcast_ack(buf_msg(buf), l_ptr->owner->bclink.last_in);
		if (tipc_bearer_send(l_ptr->b_ptr, buf, &l_ptr->media_addr)) {
			l_ptr->unacked_window = 0;
			kfree_skb(buf);
			l_ptr->proto_msg_queue = NULL;
			return 0;
		} else {
			l_ptr->stats.bearer_congs++;
			return PUSH_FAILED;
		}
	}

	/* Send one deferred data message, if send window not full: */

	buf = l_ptr->next_out;
	if (buf) {
		struct tipc_msg *msg = buf_msg(buf);
		u32 next = msg_seqno(msg);
		u32 first = buf_seqno(l_ptr->first_out);

		if (mod(next - first) < l_ptr->queue_limit[0]) {
			msg_set_ack(msg, mod(l_ptr->next_in_no - 1));
			msg_set_bcast_ack(msg, l_ptr->owner->bclink.last_in);
			if (tipc_bearer_send(l_ptr->b_ptr, buf, &l_ptr->media_addr)) {
				if (msg_user(msg) == MSG_BUNDLER)
					msg_set_type(msg, CLOSED_MSG);
				l_ptr->next_out = buf->next;
				return 0;
			} else {
				l_ptr->stats.bearer_congs++;
				return PUSH_FAILED;
			}
		}
	}
	return PUSH_FINISHED;
}

/*
 * push_queue(): push out the unsent messages of a link where
 *               congestion has abated. Node is locked
 */
void tipc_link_push_queue(struct tipc_link *l_ptr)
{
	u32 res;

	if (tipc_bearer_congested(l_ptr->b_ptr, l_ptr))
		return;

	do {
		res = tipc_link_push_packet(l_ptr);
	} while (!res);

	if (res == PUSH_FAILED)
		tipc_bearer_schedule(l_ptr->b_ptr, l_ptr);
}

static void link_reset_all(unsigned long addr)
{
	struct tipc_node *n_ptr;
	char addr_string[16];
	u32 i;

	read_lock_bh(&tipc_net_lock);
	n_ptr = tipc_node_find((u32)addr);
	if (!n_ptr) {
		read_unlock_bh(&tipc_net_lock);
		return;	/* node no longer exists */
	}

	tipc_node_lock(n_ptr);

	warn("Resetting all links to %s\n",
	     tipc_addr_string_fill(addr_string, n_ptr->addr));

	for (i = 0; i < MAX_BEARERS; i++) {
		if (n_ptr->links[i]) {
			link_print(n_ptr->links[i], "Resetting link\n");
			tipc_link_reset(n_ptr->links[i]);
		}
	}

	tipc_node_unlock(n_ptr);
	read_unlock_bh(&tipc_net_lock);
}

static void link_retransmit_failure(struct tipc_link *l_ptr,
					struct sk_buff *buf)
{
	struct tipc_msg *msg = buf_msg(buf);

	warn("Retransmission failure on link <%s>\n", l_ptr->name);

	if (l_ptr->addr) {

		/* Handle failure on standard link */

		link_print(l_ptr, "Resetting link\n");
		tipc_link_reset(l_ptr);

	} else {

		/* Handle failure on broadcast link */

		struct tipc_node *n_ptr;
		char addr_string[16];

		info("Msg seq number: %u,  ", msg_seqno(msg));
		info("Outstanding acks: %lu\n",
		     (unsigned long) TIPC_SKB_CB(buf)->handle);

		n_ptr = tipc_bclink_retransmit_to();
		tipc_node_lock(n_ptr);

		tipc_addr_string_fill(addr_string, n_ptr->addr);
		info("Broadcast link info for %s\n", addr_string);
		info("Supportable: %d,  ", n_ptr->bclink.supportable);
		info("Supported: %d,  ", n_ptr->bclink.supported);
		info("Acked: %u\n", n_ptr->bclink.acked);
		info("Last in: %u,  ", n_ptr->bclink.last_in);
		info("Oos state: %u,  ", n_ptr->bclink.oos_state);
		info("Last sent: %u\n", n_ptr->bclink.last_sent);

		tipc_k_signal((Handler)link_reset_all, (unsigned long)n_ptr->addr);

		tipc_node_unlock(n_ptr);

		l_ptr->stale_count = 0;
	}
}

void tipc_link_retransmit(struct tipc_link *l_ptr, struct sk_buff *buf,
			  u32 retransmits)
{
	struct tipc_msg *msg;

	if (!buf)
		return;

	msg = buf_msg(buf);

	if (tipc_bearer_congested(l_ptr->b_ptr, l_ptr)) {
		if (l_ptr->retransm_queue_size == 0) {
			l_ptr->retransm_queue_head = msg_seqno(msg);
			l_ptr->retransm_queue_size = retransmits;
		} else {
			err("Unexpected retransmit on link %s (qsize=%d)\n",
			    l_ptr->name, l_ptr->retransm_queue_size);
		}
		return;
	} else {
		/* Detect repeated retransmit failures on uncongested bearer */

		if (l_ptr->last_retransmitted == msg_seqno(msg)) {
			if (++l_ptr->stale_count > 100) {
				link_retransmit_failure(l_ptr, buf);
				return;
			}
		} else {
			l_ptr->last_retransmitted = msg_seqno(msg);
			l_ptr->stale_count = 1;
		}
	}

	while (retransmits && (buf != l_ptr->next_out) && buf) {
		msg = buf_msg(buf);
		msg_set_ack(msg, mod(l_ptr->next_in_no - 1));
		msg_set_bcast_ack(msg, l_ptr->owner->bclink.last_in);
		if (tipc_bearer_send(l_ptr->b_ptr, buf, &l_ptr->media_addr)) {
			buf = buf->next;
			retransmits--;
			l_ptr->stats.retransmitted++;
		} else {
			tipc_bearer_schedule(l_ptr->b_ptr, l_ptr);
			l_ptr->stats.bearer_congs++;
			l_ptr->retransm_queue_head = buf_seqno(buf);
			l_ptr->retransm_queue_size = retransmits;
			return;
		}
	}

	l_ptr->retransm_queue_head = l_ptr->retransm_queue_size = 0;
}

/**
 * link_insert_deferred_queue - insert deferred messages back into receive chain
 */

static struct sk_buff *link_insert_deferred_queue(struct tipc_link *l_ptr,
						  struct sk_buff *buf)
{
	u32 seq_no;

	if (l_ptr->oldest_deferred_in == NULL)
		return buf;

	seq_no = buf_seqno(l_ptr->oldest_deferred_in);
	if (seq_no == mod(l_ptr->next_in_no)) {
		l_ptr->newest_deferred_in->next = buf;
		buf = l_ptr->oldest_deferred_in;
		l_ptr->oldest_deferred_in = NULL;
		l_ptr->deferred_inqueue_sz = 0;
	}
	return buf;
}

/**
 * link_recv_buf_validate - validate basic format of received message
 *
 * This routine ensures a TIPC message has an acceptable header, and at least
 * as much data as the header indicates it should.  The routine also ensures
 * that the entire message header is stored in the main fragment of the message
 * buffer, to simplify future access to message header fields.
 *
 * Note: Having extra info present in the message header or data areas is OK.
 * TIPC will ignore the excess, under the assumption that it is optional info
 * introduced by a later release of the protocol.
 */

static int link_recv_buf_validate(struct sk_buff *buf)
{
	static u32 min_data_hdr_size[8] = {
		SHORT_H_SIZE, MCAST_H_SIZE, NAMED_H_SIZE, BASIC_H_SIZE,
		MAX_H_SIZE, MAX_H_SIZE, MAX_H_SIZE, MAX_H_SIZE
		};

	struct tipc_msg *msg;
	u32 tipc_hdr[2];
	u32 size;
	u32 hdr_size;
	u32 min_hdr_size;

	if (unlikely(buf->len < MIN_H_SIZE))
		return 0;

	msg = skb_header_pointer(buf, 0, sizeof(tipc_hdr), tipc_hdr);
	if (msg == NULL)
		return 0;

	if (unlikely(msg_version(msg) != TIPC_VERSION))
		return 0;

	size = msg_size(msg);
	hdr_size = msg_hdr_sz(msg);
	min_hdr_size = msg_isdata(msg) ?
		min_data_hdr_size[msg_type(msg)] : INT_H_SIZE;

	if (unlikely((hdr_size < min_hdr_size) ||
		     (size < hdr_size) ||
		     (buf->len < size) ||
		     (size - hdr_size > TIPC_MAX_USER_MSG_SIZE)))
		return 0;

	return pskb_may_pull(buf, hdr_size);
}

/**
 * tipc_recv_msg - process TIPC messages arriving from off-node
 * @head: pointer to message buffer chain
 * @tb_ptr: pointer to bearer message arrived on
 *
 * Invoked with no locks held.  Bearer pointer must point to a valid bearer
 * structure (i.e. cannot be NULL), but bearer can be inactive.
 */

void tipc_recv_msg(struct sk_buff *head, struct tipc_bearer *b_ptr)
{
	read_lock_bh(&tipc_net_lock);
	while (head) {
		struct tipc_node *n_ptr;
		struct tipc_link *l_ptr;
		struct sk_buff *crs;
		struct sk_buff *buf = head;
		struct tipc_msg *msg;
		u32 seq_no;
		u32 ackd;
		u32 released = 0;
		int type;

		head = head->next;

		/* Ensure bearer is still enabled */

		if (unlikely(!b_ptr->active))
			goto cont;

		/* Ensure message is well-formed */

		if (unlikely(!link_recv_buf_validate(buf)))
			goto cont;

		/* Ensure message data is a single contiguous unit */

		if (unlikely(skb_linearize(buf)))
			goto cont;

		/* Handle arrival of a non-unicast link message */

		msg = buf_msg(buf);

		if (unlikely(msg_non_seq(msg))) {
			if (msg_user(msg) ==  LINK_CONFIG)
				tipc_disc_recv_msg(buf, b_ptr);
			else
				tipc_bclink_recv_pkt(buf);
			continue;
		}

		/* Discard unicast link messages destined for another node */

		if (unlikely(!msg_short(msg) &&
			     (msg_destnode(msg) != tipc_own_addr)))
			goto cont;

		/* Locate neighboring node that sent message */

		n_ptr = tipc_node_find(msg_prevnode(msg));
		if (unlikely(!n_ptr))
			goto cont;
		tipc_node_lock(n_ptr);

		/* Locate unicast link endpoint that should handle message */

		l_ptr = n_ptr->links[b_ptr->identity];
		if (unlikely(!l_ptr)) {
			tipc_node_unlock(n_ptr);
			goto cont;
		}

		/* Verify that communication with node is currently allowed */

		if ((n_ptr->block_setup & WAIT_PEER_DOWN) &&
			msg_user(msg) == LINK_PROTOCOL &&
			(msg_type(msg) == RESET_MSG ||
					msg_type(msg) == ACTIVATE_MSG) &&
			!msg_redundant_link(msg))
			n_ptr->block_setup &= ~WAIT_PEER_DOWN;

		if (n_ptr->block_setup) {
			tipc_node_unlock(n_ptr);
			goto cont;
		}

		/* Validate message sequence number info */

		seq_no = msg_seqno(msg);
		ackd = msg_ack(msg);

		/* Release acked messages */

		if (n_ptr->bclink.supported)
			tipc_bclink_acknowledge(n_ptr, msg_bcast_ack(msg));

		crs = l_ptr->first_out;
		while ((crs != l_ptr->next_out) &&
		       less_eq(buf_seqno(crs), ackd)) {
			struct sk_buff *next = crs->next;

			kfree_skb(crs);
			crs = next;
			released++;
		}
		if (released) {
			l_ptr->first_out = crs;
			l_ptr->out_queue_size -= released;
		}

		/* Try sending any messages link endpoint has pending */

		if (unlikely(l_ptr->next_out))
			tipc_link_push_queue(l_ptr);
		if (unlikely(!list_empty(&l_ptr->waiting_ports)))
			tipc_link_wakeup_ports(l_ptr, 0);
		if (unlikely(++l_ptr->unacked_window >= TIPC_MIN_LINK_WIN)) {
			l_ptr->stats.sent_acks++;
			tipc_link_send_proto_msg(l_ptr, STATE_MSG, 0, 0, 0, 0, 0);
		}

		/* Now (finally!) process the incoming message */

protocol_check:
		if (likely(link_working_working(l_ptr))) {
			if (likely(seq_no == mod(l_ptr->next_in_no))) {
				l_ptr->next_in_no++;
				if (unlikely(l_ptr->oldest_deferred_in))
					head = link_insert_deferred_queue(l_ptr,
									  head);
deliver:
				if (likely(msg_isdata(msg))) {
					tipc_node_unlock(n_ptr);
					tipc_port_recv_msg(buf);
					continue;
				}
				switch (msg_user(msg)) {
					int ret;
				case MSG_BUNDLER:
					l_ptr->stats.recv_bundles++;
					l_ptr->stats.recv_bundled +=
						msg_msgcnt(msg);
					tipc_node_unlock(n_ptr);
					tipc_link_recv_bundle(buf);
					continue;
				case NAME_DISTRIBUTOR:
					tipc_node_unlock(n_ptr);
					tipc_named_recv(buf);
					continue;
				case CONN_MANAGER:
					tipc_node_unlock(n_ptr);
					tipc_port_recv_proto_msg(buf);
					continue;
				case MSG_FRAGMENTER:
					l_ptr->stats.recv_fragments++;
					ret = tipc_link_recv_fragment(
						&l_ptr->defragm_buf,
						&buf, &msg);
					if (ret == 1) {
						l_ptr->stats.recv_fragmented++;
						goto deliver;
					}
					if (ret == -1)
						l_ptr->next_in_no--;
					break;
				case CHANGEOVER_PROTOCOL:
					type = msg_type(msg);
					if (link_recv_changeover_msg(&l_ptr,
								     &buf)) {
						msg = buf_msg(buf);
						seq_no = msg_seqno(msg);
						if (type == ORIGINAL_MSG)
							goto deliver;
						goto protocol_check;
					}
					break;
				default:
					kfree_skb(buf);
					buf = NULL;
					break;
				}
				tipc_node_unlock(n_ptr);
				tipc_net_route_msg(buf);
				continue;
			}
			link_handle_out_of_seq_msg(l_ptr, buf);
			head = link_insert_deferred_queue(l_ptr, head);
			tipc_node_unlock(n_ptr);
			continue;
		}

		if (msg_user(msg) == LINK_PROTOCOL) {
			link_recv_proto_msg(l_ptr, buf);
			head = link_insert_deferred_queue(l_ptr, head);
			tipc_node_unlock(n_ptr);
			continue;
		}
		link_state_event(l_ptr, TRAFFIC_MSG_EVT);

		if (link_working_working(l_ptr)) {
			/* Re-insert in front of queue */
			buf->next = head;
			head = buf;
			tipc_node_unlock(n_ptr);
			continue;
		}
		tipc_node_unlock(n_ptr);
cont:
		kfree_skb(buf);
	}
	read_unlock_bh(&tipc_net_lock);
}

/*
 * tipc_link_defer_pkt - Add out-of-sequence message to deferred reception queue
 *
 * Returns increase in queue length (i.e. 0 or 1)
 */

u32 tipc_link_defer_pkt(struct sk_buff **head, struct sk_buff **tail,
			struct sk_buff *buf)
{
	struct sk_buff *queue_buf;
	struct sk_buff **prev;
	u32 seq_no = buf_seqno(buf);

	buf->next = NULL;

	/* Empty queue ? */
	if (*head == NULL) {
		*head = *tail = buf;
		return 1;
	}

	/* Last ? */
	if (less(buf_seqno(*tail), seq_no)) {
		(*tail)->next = buf;
		*tail = buf;
		return 1;
	}

	/* Locate insertion point in queue, then insert; discard if duplicate */
	prev = head;
	queue_buf = *head;
	for (;;) {
		u32 curr_seqno = buf_seqno(queue_buf);

		if (seq_no == curr_seqno) {
			kfree_skb(buf);
			return 0;
		}

		if (less(seq_no, curr_seqno))
			break;

		prev = &queue_buf->next;
		queue_buf = queue_buf->next;
	}

	buf->next = queue_buf;
	*prev = buf;
	return 1;
}

/*
 * link_handle_out_of_seq_msg - handle arrival of out-of-sequence packet
 */

static void link_handle_out_of_seq_msg(struct tipc_link *l_ptr,
				       struct sk_buff *buf)
{
	u32 seq_no = buf_seqno(buf);

	if (likely(msg_user(buf_msg(buf)) == LINK_PROTOCOL)) {
		link_recv_proto_msg(l_ptr, buf);
		return;
	}

	/* Record OOS packet arrival (force mismatch on next timeout) */

	l_ptr->checkpoint--;

	/*
	 * Discard packet if a duplicate; otherwise add it to deferred queue
	 * and notify peer of gap as per protocol specification
	 */

	if (less(seq_no, mod(l_ptr->next_in_no))) {
		l_ptr->stats.duplicates++;
		kfree_skb(buf);
		return;
	}

	if (tipc_link_defer_pkt(&l_ptr->oldest_deferred_in,
				&l_ptr->newest_deferred_in, buf)) {
		l_ptr->deferred_inqueue_sz++;
		l_ptr->stats.deferred_recv++;
		if ((l_ptr->deferred_inqueue_sz % 16) == 1)
			tipc_link_send_proto_msg(l_ptr, STATE_MSG, 0, 0, 0, 0, 0);
	} else
		l_ptr->stats.duplicates++;
}

/*
 * Send protocol message to the other endpoint.
 */
void tipc_link_send_proto_msg(struct tipc_link *l_ptr, u32 msg_typ,
				int probe_msg, u32 gap, u32 tolerance,
				u32 priority, u32 ack_mtu)
{
	struct sk_buff *buf = NULL;
	struct tipc_msg *msg = l_ptr->pmsg;
	u32 msg_size = sizeof(l_ptr->proto_msg);
	int r_flag;

	/* Discard any previous message that was deferred due to congestion */

	if (l_ptr->proto_msg_queue) {
		kfree_skb(l_ptr->proto_msg_queue);
		l_ptr->proto_msg_queue = NULL;
	}

	if (link_blocked(l_ptr))
		return;

	/* Abort non-RESET send if communication with node is prohibited */

	if ((l_ptr->owner->block_setup) && (msg_typ != RESET_MSG))
		return;

	/* Create protocol message with "out-of-sequence" sequence number */

	msg_set_type(msg, msg_typ);
	msg_set_net_plane(msg, l_ptr->b_ptr->net_plane);
	msg_set_bcast_ack(msg, l_ptr->owner->bclink.last_in);
	msg_set_last_bcast(msg, tipc_bclink_get_last_sent());

	if (msg_typ == STATE_MSG) {
		u32 next_sent = mod(l_ptr->next_out_no);

		if (!tipc_link_is_up(l_ptr))
			return;
		if (l_ptr->next_out)
			next_sent = buf_seqno(l_ptr->next_out);
		msg_set_next_sent(msg, next_sent);
		if (l_ptr->oldest_deferred_in) {
			u32 rec = buf_seqno(l_ptr->oldest_deferred_in);
			gap = mod(rec - mod(l_ptr->next_in_no));
		}
		msg_set_seq_gap(msg, gap);
		if (gap)
			l_ptr->stats.sent_nacks++;
		msg_set_link_tolerance(msg, tolerance);
		msg_set_linkprio(msg, priority);
		msg_set_max_pkt(msg, ack_mtu);
		msg_set_ack(msg, mod(l_ptr->next_in_no - 1));
		msg_set_probe(msg, probe_msg != 0);
		if (probe_msg) {
			u32 mtu = l_ptr->max_pkt;

			if ((mtu < l_ptr->max_pkt_target) &&
			    link_working_working(l_ptr) &&
			    l_ptr->fsm_msg_cnt) {
				msg_size = (mtu + (l_ptr->max_pkt_target - mtu)/2 + 2) & ~3;
				if (l_ptr->max_pkt_probes == 10) {
					l_ptr->max_pkt_target = (msg_size - 4);
					l_ptr->max_pkt_probes = 0;
					msg_size = (mtu + (l_ptr->max_pkt_target - mtu)/2 + 2) & ~3;
				}
				l_ptr->max_pkt_probes++;
			}

			l_ptr->stats.sent_probes++;
		}
		l_ptr->stats.sent_states++;
	} else {		/* RESET_MSG or ACTIVATE_MSG */
		msg_set_ack(msg, mod(l_ptr->reset_checkpoint - 1));
		msg_set_seq_gap(msg, 0);
		msg_set_next_sent(msg, 1);
		msg_set_probe(msg, 0);
		msg_set_link_tolerance(msg, l_ptr->tolerance);
		msg_set_linkprio(msg, l_ptr->priority);
		msg_set_max_pkt(msg, l_ptr->max_pkt_target);
	}

	r_flag = (l_ptr->owner->working_links > tipc_link_is_up(l_ptr));
	msg_set_redundant_link(msg, r_flag);
	msg_set_linkprio(msg, l_ptr->priority);
	msg_set_size(msg, msg_size);

	msg_set_seqno(msg, mod(l_ptr->next_out_no + (0xffff/2)));

	buf = tipc_buf_acquire(msg_size);
	if (!buf)
		return;

	skb_copy_to_linear_data(buf, msg, sizeof(l_ptr->proto_msg));

	/* Defer message if bearer is already congested */

	if (tipc_bearer_congested(l_ptr->b_ptr, l_ptr)) {
		l_ptr->proto_msg_queue = buf;
		return;
	}

	/* Defer message if attempting to send results in bearer congestion */

	if (!tipc_bearer_send(l_ptr->b_ptr, buf, &l_ptr->media_addr)) {
		tipc_bearer_schedule(l_ptr->b_ptr, l_ptr);
		l_ptr->proto_msg_queue = buf;
		l_ptr->stats.bearer_congs++;
		return;
	}

	/* Discard message if it was sent successfully */

	l_ptr->unacked_window = 0;
	kfree_skb(buf);
}

/*
 * Receive protocol message :
 * Note that network plane id propagates through the network, and may
 * change at any time. The node with lowest address rules
 */

static void link_recv_proto_msg(struct tipc_link *l_ptr, struct sk_buff *buf)
{
	u32 rec_gap = 0;
	u32 max_pkt_info;
	u32 max_pkt_ack;
	u32 msg_tol;
	struct tipc_msg *msg = buf_msg(buf);

	if (link_blocked(l_ptr))
		goto exit;

	/* record unnumbered packet arrival (force mismatch on next timeout) */

	l_ptr->checkpoint--;

	if (l_ptr->b_ptr->net_plane != msg_net_plane(msg))
		if (tipc_own_addr > msg_prevnode(msg))
			l_ptr->b_ptr->net_plane = msg_net_plane(msg);

	l_ptr->owner->permit_changeover = msg_redundant_link(msg);

	switch (msg_type(msg)) {

	case RESET_MSG:
		if (!link_working_unknown(l_ptr) &&
		    (l_ptr->peer_session != INVALID_SESSION)) {
			if (less_eq(msg_session(msg), l_ptr->peer_session))
				break; /* duplicate or old reset: ignore */
		}

		if (!msg_redundant_link(msg) && (link_working_working(l_ptr) ||
				link_working_unknown(l_ptr))) {
			/*
			 * peer has lost contact -- don't allow peer's links
			 * to reactivate before we recognize loss & clean up
			 */
			l_ptr->owner->block_setup = WAIT_NODE_DOWN;
		}

		link_state_event(l_ptr, RESET_MSG);

		/* fall thru' */
	case ACTIVATE_MSG:
		/* Update link settings according other endpoint's values */

		strcpy((strrchr(l_ptr->name, ':') + 1), (char *)msg_data(msg));

		msg_tol = msg_link_tolerance(msg);
		if (msg_tol > l_ptr->tolerance)
			link_set_supervision_props(l_ptr, msg_tol);

		if (msg_linkprio(msg) > l_ptr->priority)
			l_ptr->priority = msg_linkprio(msg);

		max_pkt_info = msg_max_pkt(msg);
		if (max_pkt_info) {
			if (max_pkt_info < l_ptr->max_pkt_target)
				l_ptr->max_pkt_target = max_pkt_info;
			if (l_ptr->max_pkt > l_ptr->max_pkt_target)
				l_ptr->max_pkt = l_ptr->max_pkt_target;
		} else {
			l_ptr->max_pkt = l_ptr->max_pkt_target;
		}
		l_ptr->owner->bclink.supportable = (max_pkt_info != 0);

		/* Synchronize broadcast link info, if not done previously */

		if (!tipc_node_is_up(l_ptr->owner)) {
			l_ptr->owner->bclink.last_sent =
				l_ptr->owner->bclink.last_in =
				msg_last_bcast(msg);
			l_ptr->owner->bclink.oos_state = 0;
		}

		l_ptr->peer_session = msg_session(msg);
		l_ptr->peer_bearer_id = msg_bearer_id(msg);

		if (msg_type(msg) == ACTIVATE_MSG)
			link_state_event(l_ptr, ACTIVATE_MSG);
		break;
	case STATE_MSG:

		msg_tol = msg_link_tolerance(msg);
		if (msg_tol)
			link_set_supervision_props(l_ptr, msg_tol);

		if (msg_linkprio(msg) &&
		    (msg_linkprio(msg) != l_ptr->priority)) {
			warn("Resetting link <%s>, priority change %u->%u\n",
			     l_ptr->name, l_ptr->priority, msg_linkprio(msg));
			l_ptr->priority = msg_linkprio(msg);
			tipc_link_reset(l_ptr); /* Enforce change to take effect */
			break;
		}
		link_state_event(l_ptr, TRAFFIC_MSG_EVT);
		l_ptr->stats.recv_states++;
		if (link_reset_unknown(l_ptr))
			break;

		if (less_eq(mod(l_ptr->next_in_no), msg_next_sent(msg))) {
			rec_gap = mod(msg_next_sent(msg) -
				      mod(l_ptr->next_in_no));
		}

		max_pkt_ack = msg_max_pkt(msg);
		if (max_pkt_ack > l_ptr->max_pkt) {
			l_ptr->max_pkt = max_pkt_ack;
			l_ptr->max_pkt_probes = 0;
		}

		max_pkt_ack = 0;
		if (msg_probe(msg)) {
			l_ptr->stats.recv_probes++;
			if (msg_size(msg) > sizeof(l_ptr->proto_msg))
				max_pkt_ack = msg_size(msg);
		}

		/* Protocol message before retransmits, reduce loss risk */

		if (l_ptr->owner->bclink.supported)
			tipc_bclink_update_link_state(l_ptr->owner,
						      msg_last_bcast(msg));

		if (rec_gap || (msg_probe(msg))) {
			tipc_link_send_proto_msg(l_ptr, STATE_MSG,
						 0, rec_gap, 0, 0, max_pkt_ack);
		}
		if (msg_seq_gap(msg)) {
			l_ptr->stats.recv_nacks++;
			tipc_link_retransmit(l_ptr, l_ptr->first_out,
					     msg_seq_gap(msg));
		}
		break;
	}
exit:
	kfree_skb(buf);
}


/*
 * tipc_link_tunnel(): Send one message via a link belonging to
 * another bearer. Owner node is locked.
 */
static void tipc_link_tunnel(struct tipc_link *l_ptr,
			     struct tipc_msg *tunnel_hdr,
			     struct tipc_msg  *msg,
			     u32 selector)
{
	struct tipc_link *tunnel;
	struct sk_buff *buf;
	u32 length = msg_size(msg);

	tunnel = l_ptr->owner->active_links[selector & 1];
	if (!tipc_link_is_up(tunnel)) {
		warn("Link changeover error, "
		     "tunnel link no longer available\n");
		return;
	}
	msg_set_size(tunnel_hdr, length + INT_H_SIZE);
	buf = tipc_buf_acquire(length + INT_H_SIZE);
	if (!buf) {
		warn("Link changeover error, "
		     "unable to send tunnel msg\n");
		return;
	}
	skb_copy_to_linear_data(buf, tunnel_hdr, INT_H_SIZE);
	skb_copy_to_linear_data_offset(buf, INT_H_SIZE, msg, length);
	tipc_link_send_buf(tunnel, buf);
}



/*
 * changeover(): Send whole message queue via the remaining link
 *               Owner node is locked.
 */

void tipc_link_changeover(struct tipc_link *l_ptr)
{
	u32 msgcount = l_ptr->out_queue_size;
	struct sk_buff *crs = l_ptr->first_out;
	struct tipc_link *tunnel = l_ptr->owner->active_links[0];
	struct tipc_msg tunnel_hdr;
	int split_bundles;

	if (!tunnel)
		return;

	if (!l_ptr->owner->permit_changeover) {
		warn("Link changeover error, "
		     "peer did not permit changeover\n");
		return;
	}

	tipc_msg_init(&tunnel_hdr, CHANGEOVER_PROTOCOL,
		 ORIGINAL_MSG, INT_H_SIZE, l_ptr->addr);
	msg_set_bearer_id(&tunnel_hdr, l_ptr->peer_bearer_id);
	msg_set_msgcnt(&tunnel_hdr, msgcount);

	if (!l_ptr->first_out) {
		struct sk_buff *buf;

		buf = tipc_buf_acquire(INT_H_SIZE);
		if (buf) {
			skb_copy_to_linear_data(buf, &tunnel_hdr, INT_H_SIZE);
			msg_set_size(&tunnel_hdr, INT_H_SIZE);
			tipc_link_send_buf(tunnel, buf);
		} else {
			warn("Link changeover error, "
			     "unable to send changeover msg\n");
		}
		return;
	}

	split_bundles = (l_ptr->owner->active_links[0] !=
			 l_ptr->owner->active_links[1]);

	while (crs) {
		struct tipc_msg *msg = buf_msg(crs);

		if ((msg_user(msg) == MSG_BUNDLER) && split_bundles) {
			struct tipc_msg *m = msg_get_wrapped(msg);
			unchar *pos = (unchar *)m;

			msgcount = msg_msgcnt(msg);
			while (msgcount--) {
				msg_set_seqno(m, msg_seqno(msg));
				tipc_link_tunnel(l_ptr, &tunnel_hdr, m,
						 msg_link_selector(m));
				pos += align(msg_size(m));
				m = (struct tipc_msg *)pos;
			}
		} else {
			tipc_link_tunnel(l_ptr, &tunnel_hdr, msg,
					 msg_link_selector(msg));
		}
		crs = crs->next;
	}
}

void tipc_link_send_duplicate(struct tipc_link *l_ptr, struct tipc_link *tunnel)
{
	struct sk_buff *iter;
	struct tipc_msg tunnel_hdr;

	tipc_msg_init(&tunnel_hdr, CHANGEOVER_PROTOCOL,
		 DUPLICATE_MSG, INT_H_SIZE, l_ptr->addr);
	msg_set_msgcnt(&tunnel_hdr, l_ptr->out_queue_size);
	msg_set_bearer_id(&tunnel_hdr, l_ptr->peer_bearer_id);
	iter = l_ptr->first_out;
	while (iter) {
		struct sk_buff *outbuf;
		struct tipc_msg *msg = buf_msg(iter);
		u32 length = msg_size(msg);

		if (msg_user(msg) == MSG_BUNDLER)
			msg_set_type(msg, CLOSED_MSG);
		msg_set_ack(msg, mod(l_ptr->next_in_no - 1));	/* Update */
		msg_set_bcast_ack(msg, l_ptr->owner->bclink.last_in);
		msg_set_size(&tunnel_hdr, length + INT_H_SIZE);
		outbuf = tipc_buf_acquire(length + INT_H_SIZE);
		if (outbuf == NULL) {
			warn("Link changeover error, "
			     "unable to send duplicate msg\n");
			return;
		}
		skb_copy_to_linear_data(outbuf, &tunnel_hdr, INT_H_SIZE);
		skb_copy_to_linear_data_offset(outbuf, INT_H_SIZE, iter->data,
					       length);
		tipc_link_send_buf(tunnel, outbuf);
		if (!tipc_link_is_up(l_ptr))
			return;
		iter = iter->next;
	}
}



/**
 * buf_extract - extracts embedded TIPC message from another message
 * @skb: encapsulating message buffer
 * @from_pos: offset to extract from
 *
 * Returns a new message buffer containing an embedded message.  The
 * encapsulating message itself is left unchanged.
 */

static struct sk_buff *buf_extract(struct sk_buff *skb, u32 from_pos)
{
	struct tipc_msg *msg = (struct tipc_msg *)(skb->data + from_pos);
	u32 size = msg_size(msg);
	struct sk_buff *eb;

	eb = tipc_buf_acquire(size);
	if (eb)
		skb_copy_to_linear_data(eb, msg, size);
	return eb;
}

/*
 *  link_recv_changeover_msg(): Receive tunneled packet sent
 *  via other link. Node is locked. Return extracted buffer.
 */

static int link_recv_changeover_msg(struct tipc_link **l_ptr,
				    struct sk_buff **buf)
{
	struct sk_buff *tunnel_buf = *buf;
	struct tipc_link *dest_link;
	struct tipc_msg *msg;
	struct tipc_msg *tunnel_msg = buf_msg(tunnel_buf);
	u32 msg_typ = msg_type(tunnel_msg);
	u32 msg_count = msg_msgcnt(tunnel_msg);

	dest_link = (*l_ptr)->owner->links[msg_bearer_id(tunnel_msg)];
	if (!dest_link)
		goto exit;
	if (dest_link == *l_ptr) {
		err("Unexpected changeover message on link <%s>\n",
		    (*l_ptr)->name);
		goto exit;
	}
	*l_ptr = dest_link;
	msg = msg_get_wrapped(tunnel_msg);

	if (msg_typ == DUPLICATE_MSG) {
		if (less(msg_seqno(msg), mod(dest_link->next_in_no)))
			goto exit;
		*buf = buf_extract(tunnel_buf, INT_H_SIZE);
		if (*buf == NULL) {
			warn("Link changeover error, duplicate msg dropped\n");
			goto exit;
		}
		kfree_skb(tunnel_buf);
		return 1;
	}

	/* First original message ?: */

	if (tipc_link_is_up(dest_link)) {
		info("Resetting link <%s>, changeover initiated by peer\n",
		     dest_link->name);
		tipc_link_reset(dest_link);
		dest_link->exp_msg_count = msg_count;
		if (!msg_count)
			goto exit;
	} else if (dest_link->exp_msg_count == START_CHANGEOVER) {
		dest_link->exp_msg_count = msg_count;
		if (!msg_count)
			goto exit;
	}

	/* Receive original message */

	if (dest_link->exp_msg_count == 0) {
		warn("Link switchover error, "
		     "got too many tunnelled messages\n");
		goto exit;
	}
	dest_link->exp_msg_count--;
	if (less(msg_seqno(msg), dest_link->reset_checkpoint)) {
		goto exit;
	} else {
		*buf = buf_extract(tunnel_buf, INT_H_SIZE);
		if (*buf != NULL) {
			kfree_skb(tunnel_buf);
			return 1;
		} else {
			warn("Link changeover error, original msg dropped\n");
		}
	}
exit:
	*buf = NULL;
	kfree_skb(tunnel_buf);
	return 0;
}

/*
 *  Bundler functionality:
 */
void tipc_link_recv_bundle(struct sk_buff *buf)
{
	u32 msgcount = msg_msgcnt(buf_msg(buf));
	u32 pos = INT_H_SIZE;
	struct sk_buff *obuf;

	while (msgcount--) {
		obuf = buf_extract(buf, pos);
		if (obuf == NULL) {
			warn("Link unable to unbundle message(s)\n");
			break;
		}
		pos += align(msg_size(buf_msg(obuf)));
		tipc_net_route_msg(obuf);
	}
	kfree_skb(buf);
}

/*
 *  Fragmentation/defragmentation:
 */


/*
 * link_send_long_buf: Entry for buffers needing fragmentation.
 * The buffer is complete, inclusive total message length.
 * Returns user data length.
 */
static int link_send_long_buf(struct tipc_link *l_ptr, struct sk_buff *buf)
{
	struct sk_buff *buf_chain = NULL;
	struct sk_buff *buf_chain_tail = (struct sk_buff *)&buf_chain;
	struct tipc_msg *inmsg = buf_msg(buf);
	struct tipc_msg fragm_hdr;
	u32 insize = msg_size(inmsg);
	u32 dsz = msg_data_sz(inmsg);
	unchar *crs = buf->data;
	u32 rest = insize;
	u32 pack_sz = l_ptr->max_pkt;
	u32 fragm_sz = pack_sz - INT_H_SIZE;
	u32 fragm_no = 0;
	u32 destaddr;

	if (msg_short(inmsg))
		destaddr = l_ptr->addr;
	else
		destaddr = msg_destnode(inmsg);

	/* Prepare reusable fragment header: */

	tipc_msg_init(&fragm_hdr, MSG_FRAGMENTER, FIRST_FRAGMENT,
		 INT_H_SIZE, destaddr);

	/* Chop up message: */

	while (rest > 0) {
		struct sk_buff *fragm;

		if (rest <= fragm_sz) {
			fragm_sz = rest;
			msg_set_type(&fragm_hdr, LAST_FRAGMENT);
		}
		fragm = tipc_buf_acquire(fragm_sz + INT_H_SIZE);
		if (fragm == NULL) {
			kfree_skb(buf);
			while (buf_chain) {
				buf = buf_chain;
				buf_chain = buf_chain->next;
				kfree_skb(buf);
			}
			return -ENOMEM;
		}
		msg_set_size(&fragm_hdr, fragm_sz + INT_H_SIZE);
		fragm_no++;
		msg_set_fragm_no(&fragm_hdr, fragm_no);
		skb_copy_to_linear_data(fragm, &fragm_hdr, INT_H_SIZE);
		skb_copy_to_linear_data_offset(fragm, INT_H_SIZE, crs,
					       fragm_sz);
		buf_chain_tail->next = fragm;
		buf_chain_tail = fragm;

		rest -= fragm_sz;
		crs += fragm_sz;
		msg_set_type(&fragm_hdr, FRAGMENT);
	}
	kfree_skb(buf);

	/* Append chain of fragments to send queue & send them */

	l_ptr->long_msg_seq_no++;
	link_add_chain_to_outqueue(l_ptr, buf_chain, l_ptr->long_msg_seq_no);
	l_ptr->stats.sent_fragments += fragm_no;
	l_ptr->stats.sent_fragmented++;
	tipc_link_push_queue(l_ptr);

	return dsz;
}

/*
 * A pending message being re-assembled must store certain values
 * to handle subsequent fragments correctly. The following functions
 * help storing these values in unused, available fields in the
 * pending message. This makes dynamic memory allocation unnecessary.
 */

static void set_long_msg_seqno(struct sk_buff *buf, u32 seqno)
{
	msg_set_seqno(buf_msg(buf), seqno);
}

static u32 get_fragm_size(struct sk_buff *buf)
{
	return msg_ack(buf_msg(buf));
}

static void set_fragm_size(struct sk_buff *buf, u32 sz)
{
	msg_set_ack(buf_msg(buf), sz);
}

static u32 get_expected_frags(struct sk_buff *buf)
{
	return msg_bcast_ack(buf_msg(buf));
}

static void set_expected_frags(struct sk_buff *buf, u32 exp)
{
	msg_set_bcast_ack(buf_msg(buf), exp);
}

static u32 get_timer_cnt(struct sk_buff *buf)
{
	return msg_reroute_cnt(buf_msg(buf));
}

static void incr_timer_cnt(struct sk_buff *buf)
{
	msg_incr_reroute_cnt(buf_msg(buf));
}

/*
 * tipc_link_recv_fragment(): Called with node lock on. Returns
 * the reassembled buffer if message is complete.
 */
int tipc_link_recv_fragment(struct sk_buff **pending, struct sk_buff **fb,
			    struct tipc_msg **m)
{
	struct sk_buff *prev = NULL;
	struct sk_buff *fbuf = *fb;
	struct tipc_msg *fragm = buf_msg(fbuf);
	struct sk_buff *pbuf = *pending;
	u32 long_msg_seq_no = msg_long_msgno(fragm);

	*fb = NULL;

	/* Is there an incomplete message waiting for this fragment? */

	while (pbuf && ((buf_seqno(pbuf) != long_msg_seq_no) ||
			(msg_orignode(fragm) != msg_orignode(buf_msg(pbuf))))) {
		prev = pbuf;
		pbuf = pbuf->next;
	}

	if (!pbuf && (msg_type(fragm) == FIRST_FRAGMENT)) {
		struct tipc_msg *imsg = (struct tipc_msg *)msg_data(fragm);
		u32 msg_sz = msg_size(imsg);
		u32 fragm_sz = msg_data_sz(fragm);
		u32 exp_fragm_cnt = msg_sz/fragm_sz + !!(msg_sz % fragm_sz);
		u32 max =  TIPC_MAX_USER_MSG_SIZE + NAMED_H_SIZE;
		if (msg_type(imsg) == TIPC_MCAST_MSG)
			max = TIPC_MAX_USER_MSG_SIZE + MCAST_H_SIZE;
		if (msg_size(imsg) > max) {
			kfree_skb(fbuf);
			return 0;
		}
		pbuf = tipc_buf_acquire(msg_size(imsg));
		if (pbuf != NULL) {
			pbuf->next = *pending;
			*pending = pbuf;
			skb_copy_to_linear_data(pbuf, imsg,
						msg_data_sz(fragm));
			/*  Prepare buffer for subsequent fragments. */

			set_long_msg_seqno(pbuf, long_msg_seq_no);
			set_fragm_size(pbuf, fragm_sz);
			set_expected_frags(pbuf, exp_fragm_cnt - 1);
		} else {
			dbg("Link unable to reassemble fragmented message\n");
			kfree_skb(fbuf);
			return -1;
		}
		kfree_skb(fbuf);
		return 0;
	} else if (pbuf && (msg_type(fragm) != FIRST_FRAGMENT)) {
		u32 dsz = msg_data_sz(fragm);
		u32 fsz = get_fragm_size(pbuf);
		u32 crs = ((msg_fragm_no(fragm) - 1) * fsz);
		u32 exp_frags = get_expected_frags(pbuf) - 1;
		skb_copy_to_linear_data_offset(pbuf, crs,
					       msg_data(fragm), dsz);
		kfree_skb(fbuf);

		/* Is message complete? */

		if (exp_frags == 0) {
			if (prev)
				prev->next = pbuf->next;
			else
				*pending = pbuf->next;
			msg_reset_reroute_cnt(buf_msg(pbuf));
			*fb = pbuf;
			*m = buf_msg(pbuf);
			return 1;
		}
		set_expected_frags(pbuf, exp_frags);
		return 0;
	}
	kfree_skb(fbuf);
	return 0;
}

/**
 * link_check_defragm_bufs - flush stale incoming message fragments
 * @l_ptr: pointer to link
 */

static void link_check_defragm_bufs(struct tipc_link *l_ptr)
{
	struct sk_buff *prev = NULL;
	struct sk_buff *next = NULL;
	struct sk_buff *buf = l_ptr->defragm_buf;

	if (!buf)
		return;
	if (!link_working_working(l_ptr))
		return;
	while (buf) {
		u32 cnt = get_timer_cnt(buf);

		next = buf->next;
		if (cnt < 4) {
			incr_timer_cnt(buf);
			prev = buf;
		} else {
			if (prev)
				prev->next = buf->next;
			else
				l_ptr->defragm_buf = buf->next;
			kfree_skb(buf);
		}
		buf = next;
	}
}



static void link_set_supervision_props(struct tipc_link *l_ptr, u32 tolerance)
{
	if ((tolerance < TIPC_MIN_LINK_TOL) || (tolerance > TIPC_MAX_LINK_TOL))
		return;

	l_ptr->tolerance = tolerance;
	l_ptr->continuity_interval =
		((tolerance / 4) > 500) ? 500 : tolerance / 4;
	l_ptr->abort_limit = tolerance / (l_ptr->continuity_interval / 4);
}


void tipc_link_set_queue_limits(struct tipc_link *l_ptr, u32 window)
{
	/* Data messages from this node, inclusive FIRST_FRAGM */
	l_ptr->queue_limit[TIPC_LOW_IMPORTANCE] = window;
	l_ptr->queue_limit[TIPC_MEDIUM_IMPORTANCE] = (window / 3) * 4;
	l_ptr->queue_limit[TIPC_HIGH_IMPORTANCE] = (window / 3) * 5;
	l_ptr->queue_limit[TIPC_CRITICAL_IMPORTANCE] = (window / 3) * 6;
	/* Transiting data messages,inclusive FIRST_FRAGM */
	l_ptr->queue_limit[TIPC_LOW_IMPORTANCE + 4] = 300;
	l_ptr->queue_limit[TIPC_MEDIUM_IMPORTANCE + 4] = 600;
	l_ptr->queue_limit[TIPC_HIGH_IMPORTANCE + 4] = 900;
	l_ptr->queue_limit[TIPC_CRITICAL_IMPORTANCE + 4] = 1200;
	l_ptr->queue_limit[CONN_MANAGER] = 1200;
	l_ptr->queue_limit[CHANGEOVER_PROTOCOL] = 2500;
	l_ptr->queue_limit[NAME_DISTRIBUTOR] = 3000;
	/* FRAGMENT and LAST_FRAGMENT packets */
	l_ptr->queue_limit[MSG_FRAGMENTER] = 4000;
}

/**
 * link_find_link - locate link by name
 * @name - ptr to link name string
 * @node - ptr to area to be filled with ptr to associated node
 *
 * Caller must hold 'tipc_net_lock' to ensure node and bearer are not deleted;
 * this also prevents link deletion.
 *
 * Returns pointer to link (or 0 if invalid link name).
 */

static struct tipc_link *link_find_link(const char *name,
					struct tipc_node **node)
{
	struct tipc_link_name link_name_parts;
	struct tipc_bearer *b_ptr;
	struct tipc_link *l_ptr;

	if (!link_name_validate(name, &link_name_parts))
		return NULL;

	b_ptr = tipc_bearer_find_interface(link_name_parts.if_local);
	if (!b_ptr)
		return NULL;

	*node = tipc_node_find(link_name_parts.addr_peer);
	if (!*node)
		return NULL;

	l_ptr = (*node)->links[b_ptr->identity];
	if (!l_ptr || strcmp(l_ptr->name, name))
		return NULL;

	return l_ptr;
}

/**
 * link_value_is_valid -- validate proposed link tolerance/priority/window
 *
 * @cmd - value type (TIPC_CMD_SET_LINK_*)
 * @new_value - the new value
 *
 * Returns 1 if value is within range, 0 if not.
 */

static int link_value_is_valid(u16 cmd, u32 new_value)
{
	switch (cmd) {
	case TIPC_CMD_SET_LINK_TOL:
		return (new_value >= TIPC_MIN_LINK_TOL) &&
			(new_value <= TIPC_MAX_LINK_TOL);
	case TIPC_CMD_SET_LINK_PRI:
		return (new_value <= TIPC_MAX_LINK_PRI);
	case TIPC_CMD_SET_LINK_WINDOW:
		return (new_value >= TIPC_MIN_LINK_WIN) &&
			(new_value <= TIPC_MAX_LINK_WIN);
	}
	return 0;
}


/**
 * link_cmd_set_value - change priority/tolerance/window for link/bearer/media
 * @name - ptr to link, bearer, or media name
 * @new_value - new value of link, bearer, or media setting
 * @cmd - which link, bearer, or media attribute to set (TIPC_CMD_SET_LINK_*)
 *
 * Caller must hold 'tipc_net_lock' to ensure link/bearer/media is not deleted.
 *
 * Returns 0 if value updated and negative value on error.
 */

static int link_cmd_set_value(const char *name, u32 new_value, u16 cmd)
{
	struct tipc_node *node;
	struct tipc_link *l_ptr;
	struct tipc_bearer *b_ptr;
	struct tipc_media *m_ptr;

	l_ptr = link_find_link(name, &node);
	if (l_ptr) {
		/*
		 * acquire node lock for tipc_link_send_proto_msg().
		 * see "TIPC locking policy" in net.c.
		 */
		tipc_node_lock(node);
		switch (cmd) {
		case TIPC_CMD_SET_LINK_TOL:
			link_set_supervision_props(l_ptr, new_value);
			tipc_link_send_proto_msg(l_ptr,
				STATE_MSG, 0, 0, new_value, 0, 0);
			break;
		case TIPC_CMD_SET_LINK_PRI:
			l_ptr->priority = new_value;
			tipc_link_send_proto_msg(l_ptr,
				STATE_MSG, 0, 0, 0, new_value, 0);
			break;
		case TIPC_CMD_SET_LINK_WINDOW:
			tipc_link_set_queue_limits(l_ptr, new_value);
			break;
		}
		tipc_node_unlock(node);
		return 0;
	}

	b_ptr = tipc_bearer_find(name);
	if (b_ptr) {
		switch (cmd) {
		case TIPC_CMD_SET_LINK_TOL:
			b_ptr->tolerance = new_value;
			return 0;
		case TIPC_CMD_SET_LINK_PRI:
			b_ptr->priority = new_value;
			return 0;
		case TIPC_CMD_SET_LINK_WINDOW:
			b_ptr->window = new_value;
			return 0;
		}
		return -EINVAL;
	}

	m_ptr = tipc_media_find(name);
	if (!m_ptr)
		return -ENODEV;
	switch (cmd) {
	case TIPC_CMD_SET_LINK_TOL:
		m_ptr->tolerance = new_value;
		return 0;
	case TIPC_CMD_SET_LINK_PRI:
		m_ptr->priority = new_value;
		return 0;
	case TIPC_CMD_SET_LINK_WINDOW:
		m_ptr->window = new_value;
		return 0;
	}
	return -EINVAL;
}

struct sk_buff *tipc_link_cmd_config(const void *req_tlv_area, int req_tlv_space,
				     u16 cmd)
{
	struct tipc_link_config *args;
	u32 new_value;
	int res;

	if (!TLV_CHECK(req_tlv_area, req_tlv_space, TIPC_TLV_LINK_CONFIG))
		return tipc_cfg_reply_error_string(TIPC_CFG_TLV_ERROR);

	args = (struct tipc_link_config *)TLV_DATA(req_tlv_area);
	new_value = ntohl(args->value);

	if (!link_value_is_valid(cmd, new_value))
		return tipc_cfg_reply_error_string(
			"cannot change, value invalid");

	if (!strcmp(args->name, tipc_bclink_name)) {
		if ((cmd == TIPC_CMD_SET_LINK_WINDOW) &&
		    (tipc_bclink_set_queue_limits(new_value) == 0))
			return tipc_cfg_reply_none();
		return tipc_cfg_reply_error_string(TIPC_CFG_NOT_SUPPORTED
						   " (cannot change setting on broadcast link)");
	}

	read_lock_bh(&tipc_net_lock);
	res = link_cmd_set_value(args->name, new_value, cmd);
	read_unlock_bh(&tipc_net_lock);
	if (res)
		return tipc_cfg_reply_error_string("cannot change link setting");

	return tipc_cfg_reply_none();
}

/**
 * link_reset_statistics - reset link statistics
 * @l_ptr: pointer to link
 */

static void link_reset_statistics(struct tipc_link *l_ptr)
{
	memset(&l_ptr->stats, 0, sizeof(l_ptr->stats));
	l_ptr->stats.sent_info = l_ptr->next_out_no;
	l_ptr->stats.recv_info = l_ptr->next_in_no;
}

struct sk_buff *tipc_link_cmd_reset_stats(const void *req_tlv_area, int req_tlv_space)
{
	char *link_name;
	struct tipc_link *l_ptr;
	struct tipc_node *node;

	if (!TLV_CHECK(req_tlv_area, req_tlv_space, TIPC_TLV_LINK_NAME))
		return tipc_cfg_reply_error_string(TIPC_CFG_TLV_ERROR);

	link_name = (char *)TLV_DATA(req_tlv_area);
	if (!strcmp(link_name, tipc_bclink_name)) {
		if (tipc_bclink_reset_stats())
			return tipc_cfg_reply_error_string("link not found");
		return tipc_cfg_reply_none();
	}

	read_lock_bh(&tipc_net_lock);
	l_ptr = link_find_link(link_name, &node);
	if (!l_ptr) {
		read_unlock_bh(&tipc_net_lock);
		return tipc_cfg_reply_error_string("link not found");
	}

	tipc_node_lock(node);
	link_reset_statistics(l_ptr);
	tipc_node_unlock(node);
	read_unlock_bh(&tipc_net_lock);
	return tipc_cfg_reply_none();
}

/**
 * percent - convert count to a percentage of total (rounding up or down)
 */

static u32 percent(u32 count, u32 total)
{
	return (count * 100 + (total / 2)) / total;
}

/**
 * tipc_link_stats - print link statistics
 * @name: link name
 * @buf: print buffer area
 * @buf_size: size of print buffer area
 *
 * Returns length of print buffer data string (or 0 if error)
 */

static int tipc_link_stats(const char *name, char *buf, const u32 buf_size)
{
	struct print_buf pb;
	struct tipc_link *l_ptr;
	struct tipc_node *node;
	char *status;
	u32 profile_total = 0;

	if (!strcmp(name, tipc_bclink_name))
		return tipc_bclink_stats(buf, buf_size);

	tipc_printbuf_init(&pb, buf, buf_size);

	read_lock_bh(&tipc_net_lock);
	l_ptr = link_find_link(name, &node);
	if (!l_ptr) {
		read_unlock_bh(&tipc_net_lock);
		return 0;
	}
	tipc_node_lock(node);

	if (tipc_link_is_active(l_ptr))
		status = "ACTIVE";
	else if (tipc_link_is_up(l_ptr))
		status = "STANDBY";
	else
		status = "DEFUNCT";
	tipc_printf(&pb, "Link <%s>\n"
			 "  %s  MTU:%u  Priority:%u  Tolerance:%u ms"
			 "  Window:%u packets\n",
		    l_ptr->name, status, l_ptr->max_pkt,
		    l_ptr->priority, l_ptr->tolerance, l_ptr->queue_limit[0]);
	tipc_printf(&pb, "  RX packets:%u fragments:%u/%u bundles:%u/%u\n",
		    l_ptr->next_in_no - l_ptr->stats.recv_info,
		    l_ptr->stats.recv_fragments,
		    l_ptr->stats.recv_fragmented,
		    l_ptr->stats.recv_bundles,
		    l_ptr->stats.recv_bundled);
	tipc_printf(&pb, "  TX packets:%u fragments:%u/%u bundles:%u/%u\n",
		    l_ptr->next_out_no - l_ptr->stats.sent_info,
		    l_ptr->stats.sent_fragments,
		    l_ptr->stats.sent_fragmented,
		    l_ptr->stats.sent_bundles,
		    l_ptr->stats.sent_bundled);
	profile_total = l_ptr->stats.msg_length_counts;
	if (!profile_total)
		profile_total = 1;
	tipc_printf(&pb, "  TX profile sample:%u packets  average:%u octets\n"
			 "  0-64:%u%% -256:%u%% -1024:%u%% -4096:%u%% "
			 "-16384:%u%% -32768:%u%% -66000:%u%%\n",
		    l_ptr->stats.msg_length_counts,
		    l_ptr->stats.msg_lengths_total / profile_total,
		    percent(l_ptr->stats.msg_length_profile[0], profile_total),
		    percent(l_ptr->stats.msg_length_profile[1], profile_total),
		    percent(l_ptr->stats.msg_length_profile[2], profile_total),
		    percent(l_ptr->stats.msg_length_profile[3], profile_total),
		    percent(l_ptr->stats.msg_length_profile[4], profile_total),
		    percent(l_ptr->stats.msg_length_profile[5], profile_total),
		    percent(l_ptr->stats.msg_length_profile[6], profile_total));
	tipc_printf(&pb, "  RX states:%u probes:%u naks:%u defs:%u dups:%u\n",
		    l_ptr->stats.recv_states,
		    l_ptr->stats.recv_probes,
		    l_ptr->stats.recv_nacks,
		    l_ptr->stats.deferred_recv,
		    l_ptr->stats.duplicates);
	tipc_printf(&pb, "  TX states:%u probes:%u naks:%u acks:%u dups:%u\n",
		    l_ptr->stats.sent_states,
		    l_ptr->stats.sent_probes,
		    l_ptr->stats.sent_nacks,
		    l_ptr->stats.sent_acks,
		    l_ptr->stats.retransmitted);
	tipc_printf(&pb, "  Congestion bearer:%u link:%u  Send queue max:%u avg:%u\n",
		    l_ptr->stats.bearer_congs,
		    l_ptr->stats.link_congs,
		    l_ptr->stats.max_queue_sz,
		    l_ptr->stats.queue_sz_counts
		    ? (l_ptr->stats.accu_queue_sz / l_ptr->stats.queue_sz_counts)
		    : 0);

	tipc_node_unlock(node);
	read_unlock_bh(&tipc_net_lock);
	return tipc_printbuf_validate(&pb);
}

#define MAX_LINK_STATS_INFO 2000

struct sk_buff *tipc_link_cmd_show_stats(const void *req_tlv_area, int req_tlv_space)
{
	struct sk_buff *buf;
	struct tlv_desc *rep_tlv;
	int str_len;

	if (!TLV_CHECK(req_tlv_area, req_tlv_space, TIPC_TLV_LINK_NAME))
		return tipc_cfg_reply_error_string(TIPC_CFG_TLV_ERROR);

	buf = tipc_cfg_reply_alloc(TLV_SPACE(MAX_LINK_STATS_INFO));
	if (!buf)
		return NULL;

	rep_tlv = (struct tlv_desc *)buf->data;

	str_len = tipc_link_stats((char *)TLV_DATA(req_tlv_area),
				  (char *)TLV_DATA(rep_tlv), MAX_LINK_STATS_INFO);
	if (!str_len) {
		kfree_skb(buf);
		return tipc_cfg_reply_error_string("link not found");
	}

	skb_put(buf, TLV_SPACE(str_len));
	TLV_SET(rep_tlv, TIPC_TLV_ULTRA_STRING, NULL, str_len);

	return buf;
}

/**
 * tipc_link_get_max_pkt - get maximum packet size to use when sending to destination
 * @dest: network address of destination node
 * @selector: used to select from set of active links
 *
 * If no active link can be found, uses default maximum packet size.
 */

u32 tipc_link_get_max_pkt(u32 dest, u32 selector)
{
	struct tipc_node *n_ptr;
	struct tipc_link *l_ptr;
	u32 res = MAX_PKT_DEFAULT;

	if (dest == tipc_own_addr)
		return MAX_MSG_SIZE;

	read_lock_bh(&tipc_net_lock);
	n_ptr = tipc_node_find(dest);
	if (n_ptr) {
		tipc_node_lock(n_ptr);
		l_ptr = n_ptr->active_links[selector & 1];
		if (l_ptr)
			res = l_ptr->max_pkt;
		tipc_node_unlock(n_ptr);
	}
	read_unlock_bh(&tipc_net_lock);
	return res;
}

static void link_print(struct tipc_link *l_ptr, const char *str)
{
	char print_area[256];
	struct print_buf pb;
	struct print_buf *buf = &pb;

	tipc_printbuf_init(buf, print_area, sizeof(print_area));

	tipc_printf(buf, str);
	tipc_printf(buf, "Link %x<%s>:",
		    l_ptr->addr, l_ptr->b_ptr->name);

#ifdef CONFIG_TIPC_DEBUG
	if (link_reset_reset(l_ptr) || link_reset_unknown(l_ptr))
		goto print_state;

	tipc_printf(buf, ": NXO(%u):", mod(l_ptr->next_out_no));
	tipc_printf(buf, "NXI(%u):", mod(l_ptr->next_in_no));
	tipc_printf(buf, "SQUE");
	if (l_ptr->first_out) {
		tipc_printf(buf, "[%u..", buf_seqno(l_ptr->first_out));
		if (l_ptr->next_out)
			tipc_printf(buf, "%u..", buf_seqno(l_ptr->next_out));
		tipc_printf(buf, "%u]", buf_seqno(l_ptr->last_out));
		if ((mod(buf_seqno(l_ptr->last_out) -
			 buf_seqno(l_ptr->first_out))
		     != (l_ptr->out_queue_size - 1)) ||
		    (l_ptr->last_out->next != NULL)) {
			tipc_printf(buf, "\nSend queue inconsistency\n");
			tipc_printf(buf, "first_out= %p ", l_ptr->first_out);
			tipc_printf(buf, "next_out= %p ", l_ptr->next_out);
			tipc_printf(buf, "last_out= %p ", l_ptr->last_out);
		}
	} else
		tipc_printf(buf, "[]");
	tipc_printf(buf, "SQSIZ(%u)", l_ptr->out_queue_size);
	if (l_ptr->oldest_deferred_in) {
		u32 o = buf_seqno(l_ptr->oldest_deferred_in);
		u32 n = buf_seqno(l_ptr->newest_deferred_in);
		tipc_printf(buf, ":RQUE[%u..%u]", o, n);
		if (l_ptr->deferred_inqueue_sz != mod((n + 1) - o)) {
			tipc_printf(buf, ":RQSIZ(%u)",
				    l_ptr->deferred_inqueue_sz);
		}
	}
print_state:
#endif

	if (link_working_unknown(l_ptr))
		tipc_printf(buf, ":WU");
	else if (link_reset_reset(l_ptr))
		tipc_printf(buf, ":RR");
	else if (link_reset_unknown(l_ptr))
		tipc_printf(buf, ":RU");
	else if (link_working_working(l_ptr))
		tipc_printf(buf, ":WW");
	tipc_printf(buf, "\n");

	tipc_printbuf_validate(buf);
	info("%s", print_area);
}

=======
 * @net: pointer to associated network namespace
 * @if_name: associated interface name
 * @bearer_id: id (index) of associated bearer
 * @tolerance: link tolerance to be used by link
 * @net_plane: network plane (A,B,c..) this link belongs to
 * @mtu: mtu to be advertised by link
 * @priority: priority to be used by link
 * @min_win: minimal send window to be used by link
 * @max_win: maximal send window to be used by link
 * @session: session to be used by link
 * @peer: node id of peer node
 * @peer_caps: bitmap describing peer node capabilities
 * @bc_sndlink: the namespace global link used for broadcast sending
 * @bc_rcvlink: the peer specific link used for broadcast reception
 * @inputq: queue to put messages ready for delivery
 * @namedq: queue to put binding table update messages ready for delivery
 * @link: return value, pointer to put the created link
 * @self: local unicast link id
 * @peer_id: 128-bit ID of peer
 *
 * Return: true if link was created, otherwise false
 */
bool tipc_link_create(struct net *net, char *if_name, int bearer_id,
		      int tolerance, char net_plane, u32 mtu, int priority,
		      u32 min_win, u32 max_win, u32 session, u32 self,
		      u32 peer, u8 *peer_id, u16 peer_caps,
		      struct tipc_link *bc_sndlink,
		      struct tipc_link *bc_rcvlink,
		      struct sk_buff_head *inputq,
		      struct sk_buff_head *namedq,
		      struct tipc_link **link)
{
	char peer_str[NODE_ID_STR_LEN] = {0,};
	char self_str[NODE_ID_STR_LEN] = {0,};
	struct tipc_link *l;

	l = kzalloc(sizeof(*l), GFP_ATOMIC);
	if (!l)
		return false;
	*link = l;
	l->session = session;

	/* Set link name for unicast links only */
	if (peer_id) {
		tipc_nodeid2string(self_str, tipc_own_id(net));
		if (strlen(self_str) > 16)
			sprintf(self_str, "%x", self);
		tipc_nodeid2string(peer_str, peer_id);
		if (strlen(peer_str) > 16)
			sprintf(peer_str, "%x", peer);
	}
	/* Peer i/f name will be completed by reset/activate message */
	snprintf(l->name, sizeof(l->name), "%s:%s-%s:unknown",
		 self_str, if_name, peer_str);

	strcpy(l->if_name, if_name);
	l->addr = peer;
	l->peer_caps = peer_caps;
	l->net = net;
	l->in_session = false;
	l->bearer_id = bearer_id;
	l->tolerance = tolerance;
	if (bc_rcvlink)
		bc_rcvlink->tolerance = tolerance;
	l->net_plane = net_plane;
	l->advertised_mtu = mtu;
	l->mtu = mtu;
	l->priority = priority;
	tipc_link_set_queue_limits(l, min_win, max_win);
	l->ackers = 1;
	l->bc_sndlink = bc_sndlink;
	l->bc_rcvlink = bc_rcvlink;
	l->inputq = inputq;
	l->namedq = namedq;
	l->state = LINK_RESETTING;
	__skb_queue_head_init(&l->transmq);
	__skb_queue_head_init(&l->backlogq);
	__skb_queue_head_init(&l->deferdq);
	__skb_queue_head_init(&l->failover_deferdq);
	skb_queue_head_init(&l->wakeupq);
	skb_queue_head_init(l->inputq);
	return true;
}

/**
 * tipc_link_bc_create - create new link to be used for broadcast
 * @net: pointer to associated network namespace
 * @mtu: mtu to be used initially if no peers
 * @min_win: minimal send window to be used by link
 * @max_win: maximal send window to be used by link
 * @inputq: queue to put messages ready for delivery
 * @namedq: queue to put binding table update messages ready for delivery
 * @link: return value, pointer to put the created link
 * @ownnode: identity of own node
 * @peer: node id of peer node
 * @peer_id: 128-bit ID of peer
 * @peer_caps: bitmap describing peer node capabilities
 * @bc_sndlink: the namespace global link used for broadcast sending
 *
 * Return: true if link was created, otherwise false
 */
bool tipc_link_bc_create(struct net *net, u32 ownnode, u32 peer, u8 *peer_id,
			 int mtu, u32 min_win, u32 max_win, u16 peer_caps,
			 struct sk_buff_head *inputq,
			 struct sk_buff_head *namedq,
			 struct tipc_link *bc_sndlink,
			 struct tipc_link **link)
{
	struct tipc_link *l;

	if (!tipc_link_create(net, "", MAX_BEARERS, 0, 'Z', mtu, 0, min_win,
			      max_win, 0, ownnode, peer, NULL, peer_caps,
			      bc_sndlink, NULL, inputq, namedq, link))
		return false;

	l = *link;
	if (peer_id) {
		char peer_str[NODE_ID_STR_LEN] = {0,};

		tipc_nodeid2string(peer_str, peer_id);
		if (strlen(peer_str) > 16)
			sprintf(peer_str, "%x", peer);
		/* Broadcast receiver link name: "broadcast-link:<peer>" */
		snprintf(l->name, sizeof(l->name), "%s:%s", tipc_bclink_name,
			 peer_str);
	} else {
		strcpy(l->name, tipc_bclink_name);
	}
	trace_tipc_link_reset(l, TIPC_DUMP_ALL, "bclink created!");
	tipc_link_reset(l);
	l->state = LINK_RESET;
	l->ackers = 0;
	l->bc_rcvlink = l;

	/* Broadcast send link is always up */
	if (link_is_bc_sndlink(l))
		l->state = LINK_ESTABLISHED;

	/* Disable replicast if even a single peer doesn't support it */
	if (link_is_bc_rcvlink(l) && !(peer_caps & TIPC_BCAST_RCAST))
		tipc_bcast_toggle_rcast(net, false);

	return true;
}

/**
 * tipc_link_fsm_evt - link finite state machine
 * @l: pointer to link
 * @evt: state machine event to be processed
 */
int tipc_link_fsm_evt(struct tipc_link *l, int evt)
{
	int rc = 0;
	int old_state = l->state;

	switch (l->state) {
	case LINK_RESETTING:
		switch (evt) {
		case LINK_PEER_RESET_EVT:
			l->state = LINK_PEER_RESET;
			break;
		case LINK_RESET_EVT:
			l->state = LINK_RESET;
			break;
		case LINK_FAILURE_EVT:
		case LINK_FAILOVER_BEGIN_EVT:
		case LINK_ESTABLISH_EVT:
		case LINK_FAILOVER_END_EVT:
		case LINK_SYNCH_BEGIN_EVT:
		case LINK_SYNCH_END_EVT:
		default:
			goto illegal_evt;
		}
		break;
	case LINK_RESET:
		switch (evt) {
		case LINK_PEER_RESET_EVT:
			l->state = LINK_ESTABLISHING;
			break;
		case LINK_FAILOVER_BEGIN_EVT:
			l->state = LINK_FAILINGOVER;
			break;
		case LINK_FAILURE_EVT:
		case LINK_RESET_EVT:
		case LINK_ESTABLISH_EVT:
		case LINK_FAILOVER_END_EVT:
			break;
		case LINK_SYNCH_BEGIN_EVT:
		case LINK_SYNCH_END_EVT:
		default:
			goto illegal_evt;
		}
		break;
	case LINK_PEER_RESET:
		switch (evt) {
		case LINK_RESET_EVT:
			l->state = LINK_ESTABLISHING;
			break;
		case LINK_PEER_RESET_EVT:
		case LINK_ESTABLISH_EVT:
		case LINK_FAILURE_EVT:
			break;
		case LINK_SYNCH_BEGIN_EVT:
		case LINK_SYNCH_END_EVT:
		case LINK_FAILOVER_BEGIN_EVT:
		case LINK_FAILOVER_END_EVT:
		default:
			goto illegal_evt;
		}
		break;
	case LINK_FAILINGOVER:
		switch (evt) {
		case LINK_FAILOVER_END_EVT:
			l->state = LINK_RESET;
			break;
		case LINK_PEER_RESET_EVT:
		case LINK_RESET_EVT:
		case LINK_ESTABLISH_EVT:
		case LINK_FAILURE_EVT:
			break;
		case LINK_FAILOVER_BEGIN_EVT:
		case LINK_SYNCH_BEGIN_EVT:
		case LINK_SYNCH_END_EVT:
		default:
			goto illegal_evt;
		}
		break;
	case LINK_ESTABLISHING:
		switch (evt) {
		case LINK_ESTABLISH_EVT:
			l->state = LINK_ESTABLISHED;
			break;
		case LINK_FAILOVER_BEGIN_EVT:
			l->state = LINK_FAILINGOVER;
			break;
		case LINK_RESET_EVT:
			l->state = LINK_RESET;
			break;
		case LINK_FAILURE_EVT:
		case LINK_PEER_RESET_EVT:
		case LINK_SYNCH_BEGIN_EVT:
		case LINK_FAILOVER_END_EVT:
			break;
		case LINK_SYNCH_END_EVT:
		default:
			goto illegal_evt;
		}
		break;
	case LINK_ESTABLISHED:
		switch (evt) {
		case LINK_PEER_RESET_EVT:
			l->state = LINK_PEER_RESET;
			rc |= TIPC_LINK_DOWN_EVT;
			break;
		case LINK_FAILURE_EVT:
			l->state = LINK_RESETTING;
			rc |= TIPC_LINK_DOWN_EVT;
			break;
		case LINK_RESET_EVT:
			l->state = LINK_RESET;
			break;
		case LINK_ESTABLISH_EVT:
		case LINK_SYNCH_END_EVT:
			break;
		case LINK_SYNCH_BEGIN_EVT:
			l->state = LINK_SYNCHING;
			break;
		case LINK_FAILOVER_BEGIN_EVT:
		case LINK_FAILOVER_END_EVT:
		default:
			goto illegal_evt;
		}
		break;
	case LINK_SYNCHING:
		switch (evt) {
		case LINK_PEER_RESET_EVT:
			l->state = LINK_PEER_RESET;
			rc |= TIPC_LINK_DOWN_EVT;
			break;
		case LINK_FAILURE_EVT:
			l->state = LINK_RESETTING;
			rc |= TIPC_LINK_DOWN_EVT;
			break;
		case LINK_RESET_EVT:
			l->state = LINK_RESET;
			break;
		case LINK_ESTABLISH_EVT:
		case LINK_SYNCH_BEGIN_EVT:
			break;
		case LINK_SYNCH_END_EVT:
			l->state = LINK_ESTABLISHED;
			break;
		case LINK_FAILOVER_BEGIN_EVT:
		case LINK_FAILOVER_END_EVT:
		default:
			goto illegal_evt;
		}
		break;
	default:
		pr_err("Unknown FSM state %x in %s\n", l->state, l->name);
	}
	trace_tipc_link_fsm(l->name, old_state, l->state, evt);
	return rc;
illegal_evt:
	pr_err("Illegal FSM event %x in state %x on link %s\n",
	       evt, l->state, l->name);
	trace_tipc_link_fsm(l->name, old_state, l->state, evt);
	return rc;
}

/* link_profile_stats - update statistical profiling of traffic
 */
static void link_profile_stats(struct tipc_link *l)
{
	struct sk_buff *skb;
	struct tipc_msg *msg;
	int length;

	/* Update counters used in statistical profiling of send traffic */
	l->stats.accu_queue_sz += skb_queue_len(&l->transmq);
	l->stats.queue_sz_counts++;

	skb = skb_peek(&l->transmq);
	if (!skb)
		return;
	msg = buf_msg(skb);
	length = msg_size(msg);

	if (msg_user(msg) == MSG_FRAGMENTER) {
		if (msg_type(msg) != FIRST_FRAGMENT)
			return;
		length = msg_size(msg_inner_hdr(msg));
	}
	l->stats.msg_lengths_total += length;
	l->stats.msg_length_counts++;
	if (length <= 64)
		l->stats.msg_length_profile[0]++;
	else if (length <= 256)
		l->stats.msg_length_profile[1]++;
	else if (length <= 1024)
		l->stats.msg_length_profile[2]++;
	else if (length <= 4096)
		l->stats.msg_length_profile[3]++;
	else if (length <= 16384)
		l->stats.msg_length_profile[4]++;
	else if (length <= 32768)
		l->stats.msg_length_profile[5]++;
	else
		l->stats.msg_length_profile[6]++;
}

/**
 * tipc_link_too_silent - check if link is "too silent"
 * @l: tipc link to be checked
 *
 * Return: true if the link 'silent_intv_cnt' is about to reach the
 * 'abort_limit' value, otherwise false
 */
bool tipc_link_too_silent(struct tipc_link *l)
{
	return (l->silent_intv_cnt + 2 > l->abort_limit);
}

/* tipc_link_timeout - perform periodic task as instructed from node timeout
 */
int tipc_link_timeout(struct tipc_link *l, struct sk_buff_head *xmitq)
{
	int mtyp = 0;
	int rc = 0;
	bool state = false;
	bool probe = false;
	bool setup = false;
	u16 bc_snt = l->bc_sndlink->snd_nxt - 1;
	u16 bc_acked = l->bc_rcvlink->acked;
	struct tipc_mon_state *mstate = &l->mon_state;

	trace_tipc_link_timeout(l, TIPC_DUMP_NONE, " ");
	trace_tipc_link_too_silent(l, TIPC_DUMP_ALL, " ");
	switch (l->state) {
	case LINK_ESTABLISHED:
	case LINK_SYNCHING:
		mtyp = STATE_MSG;
		link_profile_stats(l);
		tipc_mon_get_state(l->net, l->addr, mstate, l->bearer_id);
		if (mstate->reset || (l->silent_intv_cnt > l->abort_limit))
			return tipc_link_fsm_evt(l, LINK_FAILURE_EVT);
		state = bc_acked != bc_snt;
		state |= l->bc_rcvlink->rcv_unacked;
		state |= l->rcv_unacked;
		state |= !skb_queue_empty(&l->transmq);
		probe = mstate->probing;
		probe |= l->silent_intv_cnt;
		if (probe || mstate->monitoring)
			l->silent_intv_cnt++;
		probe |= !skb_queue_empty(&l->deferdq);
		if (l->snd_nxt == l->checkpoint) {
			tipc_link_update_cwin(l, 0, 0);
			probe = true;
		}
		l->checkpoint = l->snd_nxt;
		break;
	case LINK_RESET:
		setup = l->rst_cnt++ <= 4;
		setup |= !(l->rst_cnt % 16);
		mtyp = RESET_MSG;
		break;
	case LINK_ESTABLISHING:
		setup = true;
		mtyp = ACTIVATE_MSG;
		break;
	case LINK_PEER_RESET:
	case LINK_RESETTING:
	case LINK_FAILINGOVER:
		break;
	default:
		break;
	}

	if (state || probe || setup)
		tipc_link_build_proto_msg(l, mtyp, probe, 0, 0, 0, 0, xmitq);

	return rc;
}

/**
 * link_schedule_user - schedule a message sender for wakeup after congestion
 * @l: congested link
 * @hdr: header of message that is being sent
 * Create pseudo msg to send back to user when congestion abates
 */
static int link_schedule_user(struct tipc_link *l, struct tipc_msg *hdr)
{
	u32 dnode = tipc_own_addr(l->net);
	u32 dport = msg_origport(hdr);
	struct sk_buff *skb;

	/* Create and schedule wakeup pseudo message */
	skb = tipc_msg_create(SOCK_WAKEUP, 0, INT_H_SIZE, 0,
			      dnode, l->addr, dport, 0, 0);
	if (!skb)
		return -ENOBUFS;
	msg_set_dest_droppable(buf_msg(skb), true);
	TIPC_SKB_CB(skb)->chain_imp = msg_importance(hdr);
	skb_queue_tail(&l->wakeupq, skb);
	l->stats.link_congs++;
	trace_tipc_link_conges(l, TIPC_DUMP_ALL, "wakeup scheduled!");
	return -ELINKCONG;
}

/**
 * link_prepare_wakeup - prepare users for wakeup after congestion
 * @l: congested link
 * Wake up a number of waiting users, as permitted by available space
 * in the send queue
 */
static void link_prepare_wakeup(struct tipc_link *l)
{
	struct sk_buff_head *wakeupq = &l->wakeupq;
	struct sk_buff_head *inputq = l->inputq;
	struct sk_buff *skb, *tmp;
	struct sk_buff_head tmpq;
	int avail[5] = {0,};
	int imp = 0;

	__skb_queue_head_init(&tmpq);

	for (; imp <= TIPC_SYSTEM_IMPORTANCE; imp++)
		avail[imp] = l->backlog[imp].limit - l->backlog[imp].len;

	skb_queue_walk_safe(wakeupq, skb, tmp) {
		imp = TIPC_SKB_CB(skb)->chain_imp;
		if (avail[imp] <= 0)
			continue;
		avail[imp]--;
		__skb_unlink(skb, wakeupq);
		__skb_queue_tail(&tmpq, skb);
	}

	spin_lock_bh(&inputq->lock);
	skb_queue_splice_tail(&tmpq, inputq);
	spin_unlock_bh(&inputq->lock);

}

/**
 * tipc_link_set_skb_retransmit_time - set the time at which retransmission of
 *                                     the given skb should be next attempted
 * @skb: skb to set a future retransmission time for
 * @l: link the skb will be transmitted on
 */
static void tipc_link_set_skb_retransmit_time(struct sk_buff *skb,
					      struct tipc_link *l)
{
	if (link_is_bc_sndlink(l))
		TIPC_SKB_CB(skb)->nxt_retr = TIPC_BC_RETR_LIM;
	else
		TIPC_SKB_CB(skb)->nxt_retr = TIPC_UC_RETR_TIME;
}

void tipc_link_reset(struct tipc_link *l)
{
	struct sk_buff_head list;
	u32 imp;

	__skb_queue_head_init(&list);

	l->in_session = false;
	/* Force re-synch of peer session number before establishing */
	l->peer_session--;
	l->session++;
	l->mtu = l->advertised_mtu;

	spin_lock_bh(&l->wakeupq.lock);
	skb_queue_splice_init(&l->wakeupq, &list);
	spin_unlock_bh(&l->wakeupq.lock);

	spin_lock_bh(&l->inputq->lock);
	skb_queue_splice_init(&list, l->inputq);
	spin_unlock_bh(&l->inputq->lock);

	__skb_queue_purge(&l->transmq);
	__skb_queue_purge(&l->deferdq);
	__skb_queue_purge(&l->backlogq);
	__skb_queue_purge(&l->failover_deferdq);
	for (imp = 0; imp <= TIPC_SYSTEM_IMPORTANCE; imp++) {
		l->backlog[imp].len = 0;
		l->backlog[imp].target_bskb = NULL;
	}
	kfree_skb(l->reasm_buf);
	kfree_skb(l->reasm_tnlmsg);
	kfree_skb(l->failover_reasm_skb);
	l->reasm_buf = NULL;
	l->reasm_tnlmsg = NULL;
	l->failover_reasm_skb = NULL;
	l->rcv_unacked = 0;
	l->snd_nxt = 1;
	l->rcv_nxt = 1;
	l->snd_nxt_state = 1;
	l->rcv_nxt_state = 1;
	l->acked = 0;
	l->last_gap = 0;
	kfree(l->last_ga);
	l->last_ga = NULL;
	l->silent_intv_cnt = 0;
	l->rst_cnt = 0;
	l->bc_peer_is_up = false;
	memset(&l->mon_state, 0, sizeof(l->mon_state));
	tipc_link_reset_stats(l);
}

/**
 * tipc_link_xmit(): enqueue buffer list according to queue situation
 * @l: link to use
 * @list: chain of buffers containing message
 * @xmitq: returned list of packets to be sent by caller
 *
 * Consumes the buffer chain.
 * Messages at TIPC_SYSTEM_IMPORTANCE are always accepted
 * Return: 0 if success, or errno: -ELINKCONG, -EMSGSIZE or -ENOBUFS
 */
int tipc_link_xmit(struct tipc_link *l, struct sk_buff_head *list,
		   struct sk_buff_head *xmitq)
{
	struct sk_buff_head *backlogq = &l->backlogq;
	struct sk_buff_head *transmq = &l->transmq;
	struct sk_buff *skb, *_skb;
	u16 bc_ack = l->bc_rcvlink->rcv_nxt - 1;
	u16 ack = l->rcv_nxt - 1;
	u16 seqno = l->snd_nxt;
	int pkt_cnt = skb_queue_len(list);
	unsigned int mss = tipc_link_mss(l);
	unsigned int cwin = l->window;
	unsigned int mtu = l->mtu;
	struct tipc_msg *hdr;
	bool new_bundle;
	int rc = 0;
	int imp;

	if (pkt_cnt <= 0)
		return 0;

	hdr = buf_msg(skb_peek(list));
	if (unlikely(msg_size(hdr) > mtu)) {
		pr_warn("Too large msg, purging xmit list %d %d %d %d %d!\n",
			skb_queue_len(list), msg_user(hdr),
			msg_type(hdr), msg_size(hdr), mtu);
		__skb_queue_purge(list);
		return -EMSGSIZE;
	}

	imp = msg_importance(hdr);
	/* Allow oversubscription of one data msg per source at congestion */
	if (unlikely(l->backlog[imp].len >= l->backlog[imp].limit)) {
		if (imp == TIPC_SYSTEM_IMPORTANCE) {
			pr_warn("%s<%s>, link overflow", link_rst_msg, l->name);
			return -ENOBUFS;
		}
		rc = link_schedule_user(l, hdr);
	}

	if (pkt_cnt > 1) {
		l->stats.sent_fragmented++;
		l->stats.sent_fragments += pkt_cnt;
	}

	/* Prepare each packet for sending, and add to relevant queue: */
	while ((skb = __skb_dequeue(list))) {
		if (likely(skb_queue_len(transmq) < cwin)) {
			hdr = buf_msg(skb);
			msg_set_seqno(hdr, seqno);
			msg_set_ack(hdr, ack);
			msg_set_bcast_ack(hdr, bc_ack);
			_skb = skb_clone(skb, GFP_ATOMIC);
			if (!_skb) {
				kfree_skb(skb);
				__skb_queue_purge(list);
				return -ENOBUFS;
			}
			__skb_queue_tail(transmq, skb);
			tipc_link_set_skb_retransmit_time(skb, l);
			__skb_queue_tail(xmitq, _skb);
			TIPC_SKB_CB(skb)->ackers = l->ackers;
			l->rcv_unacked = 0;
			l->stats.sent_pkts++;
			seqno++;
			continue;
		}
		if (tipc_msg_try_bundle(l->backlog[imp].target_bskb, &skb,
					mss, l->addr, &new_bundle)) {
			if (skb) {
				/* Keep a ref. to the skb for next try */
				l->backlog[imp].target_bskb = skb;
				l->backlog[imp].len++;
				__skb_queue_tail(backlogq, skb);
			} else {
				if (new_bundle) {
					l->stats.sent_bundles++;
					l->stats.sent_bundled++;
				}
				l->stats.sent_bundled++;
			}
			continue;
		}
		l->backlog[imp].target_bskb = NULL;
		l->backlog[imp].len += (1 + skb_queue_len(list));
		__skb_queue_tail(backlogq, skb);
		skb_queue_splice_tail_init(list, backlogq);
	}
	l->snd_nxt = seqno;
	return rc;
}

static void tipc_link_update_cwin(struct tipc_link *l, int released,
				  bool retransmitted)
{
	int bklog_len = skb_queue_len(&l->backlogq);
	struct sk_buff_head *txq = &l->transmq;
	int txq_len = skb_queue_len(txq);
	u16 cwin = l->window;

	/* Enter fast recovery */
	if (unlikely(retransmitted)) {
		l->ssthresh = max_t(u16, l->window / 2, 300);
		l->window = min_t(u16, l->ssthresh, l->window);
		return;
	}
	/* Enter slow start */
	if (unlikely(!released)) {
		l->ssthresh = max_t(u16, l->window / 2, 300);
		l->window = l->min_win;
		return;
	}
	/* Don't increase window if no pressure on the transmit queue */
	if (txq_len + bklog_len < cwin)
		return;

	/* Don't increase window if there are holes the transmit queue */
	if (txq_len && l->snd_nxt - buf_seqno(skb_peek(txq)) != txq_len)
		return;

	l->cong_acks += released;

	/* Slow start  */
	if (cwin <= l->ssthresh) {
		l->window = min_t(u16, cwin + released, l->max_win);
		return;
	}
	/* Congestion avoidance */
	if (l->cong_acks < cwin)
		return;
	l->window = min_t(u16, ++cwin, l->max_win);
	l->cong_acks = 0;
}

static void tipc_link_advance_backlog(struct tipc_link *l,
				      struct sk_buff_head *xmitq)
{
	u16 bc_ack = l->bc_rcvlink->rcv_nxt - 1;
	struct sk_buff_head *txq = &l->transmq;
	struct sk_buff *skb, *_skb;
	u16 ack = l->rcv_nxt - 1;
	u16 seqno = l->snd_nxt;
	struct tipc_msg *hdr;
	u16 cwin = l->window;
	u32 imp;

	while (skb_queue_len(txq) < cwin) {
		skb = skb_peek(&l->backlogq);
		if (!skb)
			break;
		_skb = skb_clone(skb, GFP_ATOMIC);
		if (!_skb)
			break;
		__skb_dequeue(&l->backlogq);
		hdr = buf_msg(skb);
		imp = msg_importance(hdr);
		l->backlog[imp].len--;
		if (unlikely(skb == l->backlog[imp].target_bskb))
			l->backlog[imp].target_bskb = NULL;
		__skb_queue_tail(&l->transmq, skb);
		tipc_link_set_skb_retransmit_time(skb, l);

		__skb_queue_tail(xmitq, _skb);
		TIPC_SKB_CB(skb)->ackers = l->ackers;
		msg_set_seqno(hdr, seqno);
		msg_set_ack(hdr, ack);
		msg_set_bcast_ack(hdr, bc_ack);
		l->rcv_unacked = 0;
		l->stats.sent_pkts++;
		seqno++;
	}
	l->snd_nxt = seqno;
}

/**
 * link_retransmit_failure() - Detect repeated retransmit failures
 * @l: tipc link sender
 * @r: tipc link receiver (= l in case of unicast)
 * @rc: returned code
 *
 * Return: true if the repeated retransmit failures happens, otherwise
 * false
 */
static bool link_retransmit_failure(struct tipc_link *l, struct tipc_link *r,
				    int *rc)
{
	struct sk_buff *skb = skb_peek(&l->transmq);
	struct tipc_msg *hdr;

	if (!skb)
		return false;

	if (!TIPC_SKB_CB(skb)->retr_cnt)
		return false;

	if (!time_after(jiffies, TIPC_SKB_CB(skb)->retr_stamp +
			msecs_to_jiffies(r->tolerance * 10)))
		return false;

	hdr = buf_msg(skb);
	if (link_is_bc_sndlink(l) && !less(r->acked, msg_seqno(hdr)))
		return false;

	pr_warn("Retransmission failure on link <%s>\n", l->name);
	link_print(l, "State of link ");
	pr_info("Failed msg: usr %u, typ %u, len %u, err %u\n",
		msg_user(hdr), msg_type(hdr), msg_size(hdr), msg_errcode(hdr));
	pr_info("sqno %u, prev: %x, dest: %x\n",
		msg_seqno(hdr), msg_prevnode(hdr), msg_destnode(hdr));
	pr_info("retr_stamp %d, retr_cnt %d\n",
		jiffies_to_msecs(TIPC_SKB_CB(skb)->retr_stamp),
		TIPC_SKB_CB(skb)->retr_cnt);

	trace_tipc_list_dump(&l->transmq, true, "retrans failure!");
	trace_tipc_link_dump(l, TIPC_DUMP_NONE, "retrans failure!");
	trace_tipc_link_dump(r, TIPC_DUMP_NONE, "retrans failure!");

	if (link_is_bc_sndlink(l)) {
		r->state = LINK_RESET;
		*rc |= TIPC_LINK_DOWN_EVT;
	} else {
		*rc |= tipc_link_fsm_evt(l, LINK_FAILURE_EVT);
	}

	return true;
}

/* tipc_data_input - deliver data and name distr msgs to upper layer
 *
 * Consumes buffer if message is of right type
 * Node lock must be held
 */
static bool tipc_data_input(struct tipc_link *l, struct sk_buff *skb,
			    struct sk_buff_head *inputq)
{
	struct sk_buff_head *mc_inputq = l->bc_rcvlink->inputq;
	struct tipc_msg *hdr = buf_msg(skb);

	switch (msg_user(hdr)) {
	case TIPC_LOW_IMPORTANCE:
	case TIPC_MEDIUM_IMPORTANCE:
	case TIPC_HIGH_IMPORTANCE:
	case TIPC_CRITICAL_IMPORTANCE:
		if (unlikely(msg_in_group(hdr) || msg_mcast(hdr))) {
			skb_queue_tail(mc_inputq, skb);
			return true;
		}
		fallthrough;
	case CONN_MANAGER:
		skb_queue_tail(inputq, skb);
		return true;
	case GROUP_PROTOCOL:
		skb_queue_tail(mc_inputq, skb);
		return true;
	case NAME_DISTRIBUTOR:
		l->bc_rcvlink->state = LINK_ESTABLISHED;
		skb_queue_tail(l->namedq, skb);
		return true;
	case MSG_BUNDLER:
	case TUNNEL_PROTOCOL:
	case MSG_FRAGMENTER:
	case BCAST_PROTOCOL:
		return false;
#ifdef CONFIG_TIPC_CRYPTO
	case MSG_CRYPTO:
		if (sysctl_tipc_key_exchange_enabled &&
		    TIPC_SKB_CB(skb)->decrypted) {
			tipc_crypto_msg_rcv(l->net, skb);
			return true;
		}
		fallthrough;
#endif
	default:
		pr_warn("Dropping received illegal msg type\n");
		kfree_skb(skb);
		return true;
	}
}

/* tipc_link_input - process packet that has passed link protocol check
 *
 * Consumes buffer
 */
static int tipc_link_input(struct tipc_link *l, struct sk_buff *skb,
			   struct sk_buff_head *inputq,
			   struct sk_buff **reasm_skb)
{
	struct tipc_msg *hdr = buf_msg(skb);
	struct sk_buff *iskb;
	struct sk_buff_head tmpq;
	int usr = msg_user(hdr);
	int pos = 0;

	if (usr == MSG_BUNDLER) {
		skb_queue_head_init(&tmpq);
		l->stats.recv_bundles++;
		l->stats.recv_bundled += msg_msgcnt(hdr);
		while (tipc_msg_extract(skb, &iskb, &pos))
			tipc_data_input(l, iskb, &tmpq);
		tipc_skb_queue_splice_tail(&tmpq, inputq);
		return 0;
	} else if (usr == MSG_FRAGMENTER) {
		l->stats.recv_fragments++;
		if (tipc_buf_append(reasm_skb, &skb)) {
			l->stats.recv_fragmented++;
			tipc_data_input(l, skb, inputq);
		} else if (!*reasm_skb && !link_is_bc_rcvlink(l)) {
			pr_warn_ratelimited("Unable to build fragment list\n");
			return tipc_link_fsm_evt(l, LINK_FAILURE_EVT);
		}
		return 0;
	} else if (usr == BCAST_PROTOCOL) {
		tipc_bcast_lock(l->net);
		tipc_link_bc_init_rcv(l->bc_rcvlink, hdr);
		tipc_bcast_unlock(l->net);
	}

	kfree_skb(skb);
	return 0;
}

/* tipc_link_tnl_rcv() - receive TUNNEL_PROTOCOL message, drop or process the
 *			 inner message along with the ones in the old link's
 *			 deferdq
 * @l: tunnel link
 * @skb: TUNNEL_PROTOCOL message
 * @inputq: queue to put messages ready for delivery
 */
static int tipc_link_tnl_rcv(struct tipc_link *l, struct sk_buff *skb,
			     struct sk_buff_head *inputq)
{
	struct sk_buff **reasm_skb = &l->failover_reasm_skb;
	struct sk_buff **reasm_tnlmsg = &l->reasm_tnlmsg;
	struct sk_buff_head *fdefq = &l->failover_deferdq;
	struct tipc_msg *hdr = buf_msg(skb);
	struct sk_buff *iskb;
	int ipos = 0;
	int rc = 0;
	u16 seqno;

	if (msg_type(hdr) == SYNCH_MSG) {
		kfree_skb(skb);
		return 0;
	}

	/* Not a fragment? */
	if (likely(!msg_nof_fragms(hdr))) {
		if (unlikely(!tipc_msg_extract(skb, &iskb, &ipos))) {
			pr_warn_ratelimited("Unable to extract msg, defq: %d\n",
					    skb_queue_len(fdefq));
			return 0;
		}
		kfree_skb(skb);
	} else {
		/* Set fragment type for buf_append */
		if (msg_fragm_no(hdr) == 1)
			msg_set_type(hdr, FIRST_FRAGMENT);
		else if (msg_fragm_no(hdr) < msg_nof_fragms(hdr))
			msg_set_type(hdr, FRAGMENT);
		else
			msg_set_type(hdr, LAST_FRAGMENT);

		if (!tipc_buf_append(reasm_tnlmsg, &skb)) {
			/* Successful but non-complete reassembly? */
			if (*reasm_tnlmsg || link_is_bc_rcvlink(l))
				return 0;
			pr_warn_ratelimited("Unable to reassemble tunnel msg\n");
			return tipc_link_fsm_evt(l, LINK_FAILURE_EVT);
		}
		iskb = skb;
	}

	do {
		seqno = buf_seqno(iskb);
		if (unlikely(less(seqno, l->drop_point))) {
			kfree_skb(iskb);
			continue;
		}
		if (unlikely(seqno != l->drop_point)) {
			__tipc_skb_queue_sorted(fdefq, seqno, iskb);
			continue;
		}

		l->drop_point++;
		if (!tipc_data_input(l, iskb, inputq))
			rc |= tipc_link_input(l, iskb, inputq, reasm_skb);
		if (unlikely(rc))
			break;
	} while ((iskb = __tipc_skb_dequeue(fdefq, l->drop_point)));

	return rc;
}

/**
 * tipc_get_gap_ack_blks - get Gap ACK blocks from PROTOCOL/STATE_MSG
 * @ga: returned pointer to the Gap ACK blocks if any
 * @l: the tipc link
 * @hdr: the PROTOCOL/STATE_MSG header
 * @uc: desired Gap ACK blocks type, i.e. unicast (= 1) or broadcast (= 0)
 *
 * Return: the total Gap ACK blocks size
 */
u16 tipc_get_gap_ack_blks(struct tipc_gap_ack_blks **ga, struct tipc_link *l,
			  struct tipc_msg *hdr, bool uc)
{
	struct tipc_gap_ack_blks *p;
	u16 sz = 0;

	/* Does peer support the Gap ACK blocks feature? */
	if (l->peer_caps & TIPC_GAP_ACK_BLOCK) {
		p = (struct tipc_gap_ack_blks *)msg_data(hdr);
		sz = ntohs(p->len);
		/* Sanity check */
		if (sz == struct_size(p, gacks, size_add(p->ugack_cnt, p->bgack_cnt))) {
			/* Good, check if the desired type exists */
			if ((uc && p->ugack_cnt) || (!uc && p->bgack_cnt))
				goto ok;
		/* Backward compatible: peer might not support bc, but uc? */
		} else if (uc && sz == struct_size(p, gacks, p->ugack_cnt)) {
			if (p->ugack_cnt) {
				p->bgack_cnt = 0;
				goto ok;
			}
		}
	}
	/* Other cases: ignore! */
	p = NULL;

ok:
	*ga = p;
	return sz;
}

static u8 __tipc_build_gap_ack_blks(struct tipc_gap_ack_blks *ga,
				    struct tipc_link *l, u8 start_index)
{
	struct tipc_gap_ack *gacks = &ga->gacks[start_index];
	struct sk_buff *skb = skb_peek(&l->deferdq);
	u16 expect, seqno = 0;
	u8 n = 0;

	if (!skb)
		return 0;

	expect = buf_seqno(skb);
	skb_queue_walk(&l->deferdq, skb) {
		seqno = buf_seqno(skb);
		if (unlikely(more(seqno, expect))) {
			gacks[n].ack = htons(expect - 1);
			gacks[n].gap = htons(seqno - expect);
			if (++n >= MAX_GAP_ACK_BLKS / 2) {
				pr_info_ratelimited("Gacks on %s: %d, ql: %d!\n",
						    l->name, n,
						    skb_queue_len(&l->deferdq));
				return n;
			}
		} else if (unlikely(less(seqno, expect))) {
			pr_warn("Unexpected skb in deferdq!\n");
			continue;
		}
		expect = seqno + 1;
	}

	/* last block */
	gacks[n].ack = htons(seqno);
	gacks[n].gap = 0;
	n++;
	return n;
}

/* tipc_build_gap_ack_blks - build Gap ACK blocks
 * @l: tipc unicast link
 * @hdr: the tipc message buffer to store the Gap ACK blocks after built
 *
 * The function builds Gap ACK blocks for both the unicast & broadcast receiver
 * links of a certain peer, the buffer after built has the network data format
 * as found at the struct tipc_gap_ack_blks definition.
 *
 * returns the actual allocated memory size
 */
static u16 tipc_build_gap_ack_blks(struct tipc_link *l, struct tipc_msg *hdr)
{
	struct tipc_link *bcl = l->bc_rcvlink;
	struct tipc_gap_ack_blks *ga;
	u16 len;

	ga = (struct tipc_gap_ack_blks *)msg_data(hdr);

	/* Start with broadcast link first */
	tipc_bcast_lock(bcl->net);
	msg_set_bcast_ack(hdr, bcl->rcv_nxt - 1);
	msg_set_bc_gap(hdr, link_bc_rcv_gap(bcl));
	ga->bgack_cnt = __tipc_build_gap_ack_blks(ga, bcl, 0);
	tipc_bcast_unlock(bcl->net);

	/* Now for unicast link, but an explicit NACK only (???) */
	ga->ugack_cnt = (msg_seq_gap(hdr)) ?
			__tipc_build_gap_ack_blks(ga, l, ga->bgack_cnt) : 0;

	/* Total len */
	len = struct_size(ga, gacks, size_add(ga->bgack_cnt, ga->ugack_cnt));
	ga->len = htons(len);
	return len;
}

/* tipc_link_advance_transmq - advance TIPC link transmq queue by releasing
 *			       acked packets, also doing retransmissions if
 *			       gaps found
 * @l: tipc link with transmq queue to be advanced
 * @r: tipc link "receiver" i.e. in case of broadcast (= "l" if unicast)
 * @acked: seqno of last packet acked by peer without any gaps before
 * @gap: # of gap packets
 * @ga: buffer pointer to Gap ACK blocks from peer
 * @xmitq: queue for accumulating the retransmitted packets if any
 * @retransmitted: returned boolean value if a retransmission is really issued
 * @rc: returned code e.g. TIPC_LINK_DOWN_EVT if a repeated retransmit failures
 *      happens (- unlikely case)
 *
 * Return: the number of packets released from the link transmq
 */
static int tipc_link_advance_transmq(struct tipc_link *l, struct tipc_link *r,
				     u16 acked, u16 gap,
				     struct tipc_gap_ack_blks *ga,
				     struct sk_buff_head *xmitq,
				     bool *retransmitted, int *rc)
{
	struct tipc_gap_ack_blks *last_ga = r->last_ga, *this_ga = NULL;
	struct tipc_gap_ack *gacks = NULL;
	struct sk_buff *skb, *_skb, *tmp;
	struct tipc_msg *hdr;
	u32 qlen = skb_queue_len(&l->transmq);
	u16 nacked = acked, ngap = gap, gack_cnt = 0;
	u16 bc_ack = l->bc_rcvlink->rcv_nxt - 1;
	u16 ack = l->rcv_nxt - 1;
	u16 seqno, n = 0;
	u16 end = r->acked, start = end, offset = r->last_gap;
	u16 si = (last_ga) ? last_ga->start_index : 0;
	bool is_uc = !link_is_bc_sndlink(l);
	bool bc_has_acked = false;

	trace_tipc_link_retrans(r, acked + 1, acked + gap, &l->transmq);

	/* Determine Gap ACK blocks if any for the particular link */
	if (ga && is_uc) {
		/* Get the Gap ACKs, uc part */
		gack_cnt = ga->ugack_cnt;
		gacks = &ga->gacks[ga->bgack_cnt];
	} else if (ga) {
		/* Copy the Gap ACKs, bc part, for later renewal if needed */
		this_ga = kmemdup(ga, struct_size(ga, gacks, ga->bgack_cnt),
				  GFP_ATOMIC);
		if (likely(this_ga)) {
			this_ga->start_index = 0;
			/* Start with the bc Gap ACKs */
			gack_cnt = this_ga->bgack_cnt;
			gacks = &this_ga->gacks[0];
		} else {
			/* Hmm, we can get in trouble..., simply ignore it */
			pr_warn_ratelimited("Ignoring bc Gap ACKs, no memory\n");
		}
	}

	/* Advance the link transmq */
	skb_queue_walk_safe(&l->transmq, skb, tmp) {
		seqno = buf_seqno(skb);

next_gap_ack:
		if (less_eq(seqno, nacked)) {
			if (is_uc)
				goto release;
			/* Skip packets peer has already acked */
			if (!more(seqno, r->acked))
				continue;
			/* Get the next of last Gap ACK blocks */
			while (more(seqno, end)) {
				if (!last_ga || si >= last_ga->bgack_cnt)
					break;
				start = end + offset + 1;
				end = ntohs(last_ga->gacks[si].ack);
				offset = ntohs(last_ga->gacks[si].gap);
				si++;
				WARN_ONCE(more(start, end) ||
					  (!offset &&
					   si < last_ga->bgack_cnt) ||
					  si > MAX_GAP_ACK_BLKS,
					  "Corrupted Gap ACK: %d %d %d %d %d\n",
					  start, end, offset, si,
					  last_ga->bgack_cnt);
			}
			/* Check against the last Gap ACK block */
			if (tipc_in_range(seqno, start, end))
				continue;
			/* Update/release the packet peer is acking */
			bc_has_acked = true;
			if (--TIPC_SKB_CB(skb)->ackers)
				continue;
release:
			/* release skb */
			__skb_unlink(skb, &l->transmq);
			kfree_skb(skb);
		} else if (less_eq(seqno, nacked + ngap)) {
			/* First gap: check if repeated retrans failures? */
			if (unlikely(seqno == acked + 1 &&
				     link_retransmit_failure(l, r, rc))) {
				/* Ignore this bc Gap ACKs if any */
				kfree(this_ga);
				this_ga = NULL;
				break;
			}
			/* retransmit skb if unrestricted*/
			if (time_before(jiffies, TIPC_SKB_CB(skb)->nxt_retr))
				continue;
			tipc_link_set_skb_retransmit_time(skb, l);
			_skb = pskb_copy(skb, GFP_ATOMIC);
			if (!_skb)
				continue;
			hdr = buf_msg(_skb);
			msg_set_ack(hdr, ack);
			msg_set_bcast_ack(hdr, bc_ack);
			_skb->priority = TC_PRIO_CONTROL;
			__skb_queue_tail(xmitq, _skb);
			l->stats.retransmitted++;
			if (!is_uc)
				r->stats.retransmitted++;
			*retransmitted = true;
			/* Increase actual retrans counter & mark first time */
			if (!TIPC_SKB_CB(skb)->retr_cnt++)
				TIPC_SKB_CB(skb)->retr_stamp = jiffies;
		} else {
			/* retry with Gap ACK blocks if any */
			if (n >= gack_cnt)
				break;
			nacked = ntohs(gacks[n].ack);
			ngap = ntohs(gacks[n].gap);
			n++;
			goto next_gap_ack;
		}
	}

	/* Renew last Gap ACK blocks for bc if needed */
	if (bc_has_acked) {
		if (this_ga) {
			kfree(last_ga);
			r->last_ga = this_ga;
			r->last_gap = gap;
		} else if (last_ga) {
			if (less(acked, start)) {
				si--;
				offset = start - acked - 1;
			} else if (less(acked, end)) {
				acked = end;
			}
			if (si < last_ga->bgack_cnt) {
				last_ga->start_index = si;
				r->last_gap = offset;
			} else {
				kfree(last_ga);
				r->last_ga = NULL;
				r->last_gap = 0;
			}
		} else {
			r->last_gap = 0;
		}
		r->acked = acked;
	} else {
		kfree(this_ga);
	}

	return qlen - skb_queue_len(&l->transmq);
}

/* tipc_link_build_state_msg: prepare link state message for transmission
 *
 * Note that sending of broadcast ack is coordinated among nodes, to reduce
 * risk of ack storms towards the sender
 */
int tipc_link_build_state_msg(struct tipc_link *l, struct sk_buff_head *xmitq)
{
	if (!l)
		return 0;

	/* Broadcast ACK must be sent via a unicast link => defer to caller */
	if (link_is_bc_rcvlink(l)) {
		if (((l->rcv_nxt ^ tipc_own_addr(l->net)) & 0xf) != 0xf)
			return 0;
		l->rcv_unacked = 0;

		/* Use snd_nxt to store peer's snd_nxt in broadcast rcv link */
		l->snd_nxt = l->rcv_nxt;
		return TIPC_LINK_SND_STATE;
	}
	/* Unicast ACK */
	l->rcv_unacked = 0;
	l->stats.sent_acks++;
	tipc_link_build_proto_msg(l, STATE_MSG, 0, 0, 0, 0, 0, xmitq);
	return 0;
}

/* tipc_link_build_reset_msg: prepare link RESET or ACTIVATE message
 */
void tipc_link_build_reset_msg(struct tipc_link *l, struct sk_buff_head *xmitq)
{
	int mtyp = RESET_MSG;
	struct sk_buff *skb;

	if (l->state == LINK_ESTABLISHING)
		mtyp = ACTIVATE_MSG;

	tipc_link_build_proto_msg(l, mtyp, 0, 0, 0, 0, 0, xmitq);

	/* Inform peer that this endpoint is going down if applicable */
	skb = skb_peek_tail(xmitq);
	if (skb && (l->state == LINK_RESET))
		msg_set_peer_stopping(buf_msg(skb), 1);
}

/* tipc_link_build_nack_msg: prepare link nack message for transmission
 * Note that sending of broadcast NACK is coordinated among nodes, to
 * reduce the risk of NACK storms towards the sender
 */
static int tipc_link_build_nack_msg(struct tipc_link *l,
				    struct sk_buff_head *xmitq)
{
	u32 def_cnt = ++l->stats.deferred_recv;
	struct sk_buff_head *dfq = &l->deferdq;
	u32 defq_len = skb_queue_len(dfq);
	int match1, match2;

	if (link_is_bc_rcvlink(l)) {
		match1 = def_cnt & 0xf;
		match2 = tipc_own_addr(l->net) & 0xf;
		if (match1 == match2)
			return TIPC_LINK_SND_STATE;
		return 0;
	}

	if (defq_len >= 3 && !((defq_len - 3) % 16)) {
		u16 rcvgap = buf_seqno(skb_peek(dfq)) - l->rcv_nxt;

		tipc_link_build_proto_msg(l, STATE_MSG, 0, 0,
					  rcvgap, 0, 0, xmitq);
	}
	return 0;
}

/* tipc_link_rcv - process TIPC packets/messages arriving from off-node
 * @l: the link that should handle the message
 * @skb: TIPC packet
 * @xmitq: queue to place packets to be sent after this call
 */
int tipc_link_rcv(struct tipc_link *l, struct sk_buff *skb,
		  struct sk_buff_head *xmitq)
{
	struct sk_buff_head *defq = &l->deferdq;
	struct tipc_msg *hdr = buf_msg(skb);
	u16 seqno, rcv_nxt, win_lim;
	int released = 0;
	int rc = 0;

	/* Verify and update link state */
	if (unlikely(msg_user(hdr) == LINK_PROTOCOL))
		return tipc_link_proto_rcv(l, skb, xmitq);

	/* Don't send probe at next timeout expiration */
	l->silent_intv_cnt = 0;

	do {
		hdr = buf_msg(skb);
		seqno = msg_seqno(hdr);
		rcv_nxt = l->rcv_nxt;
		win_lim = rcv_nxt + TIPC_MAX_LINK_WIN;

		if (unlikely(!link_is_up(l))) {
			if (l->state == LINK_ESTABLISHING)
				rc = TIPC_LINK_UP_EVT;
			kfree_skb(skb);
			break;
		}

		/* Drop if outside receive window */
		if (unlikely(less(seqno, rcv_nxt) || more(seqno, win_lim))) {
			l->stats.duplicates++;
			kfree_skb(skb);
			break;
		}
		released += tipc_link_advance_transmq(l, l, msg_ack(hdr), 0,
						      NULL, NULL, NULL, NULL);

		/* Defer delivery if sequence gap */
		if (unlikely(seqno != rcv_nxt)) {
			if (!__tipc_skb_queue_sorted(defq, seqno, skb))
				l->stats.duplicates++;
			rc |= tipc_link_build_nack_msg(l, xmitq);
			break;
		}

		/* Deliver packet */
		l->rcv_nxt++;
		l->stats.recv_pkts++;

		if (unlikely(msg_user(hdr) == TUNNEL_PROTOCOL))
			rc |= tipc_link_tnl_rcv(l, skb, l->inputq);
		else if (!tipc_data_input(l, skb, l->inputq))
			rc |= tipc_link_input(l, skb, l->inputq, &l->reasm_buf);
		if (unlikely(++l->rcv_unacked >= TIPC_MIN_LINK_WIN))
			rc |= tipc_link_build_state_msg(l, xmitq);
		if (unlikely(rc & ~TIPC_LINK_SND_STATE))
			break;
	} while ((skb = __tipc_skb_dequeue(defq, l->rcv_nxt)));

	/* Forward queues and wake up waiting users */
	if (released) {
		tipc_link_update_cwin(l, released, 0);
		tipc_link_advance_backlog(l, xmitq);
		if (unlikely(!skb_queue_empty(&l->wakeupq)))
			link_prepare_wakeup(l);
	}
	return rc;
}

static void tipc_link_build_proto_msg(struct tipc_link *l, int mtyp, bool probe,
				      bool probe_reply, u16 rcvgap,
				      int tolerance, int priority,
				      struct sk_buff_head *xmitq)
{
	struct tipc_mon_state *mstate = &l->mon_state;
	struct sk_buff_head *dfq = &l->deferdq;
	struct tipc_link *bcl = l->bc_rcvlink;
	struct tipc_msg *hdr;
	struct sk_buff *skb;
	bool node_up = link_is_up(bcl);
	u16 glen = 0, bc_rcvgap = 0;
	int dlen = 0;
	void *data;

	/* Don't send protocol message during reset or link failover */
	if (tipc_link_is_blocked(l))
		return;

	if (!tipc_link_is_up(l) && (mtyp == STATE_MSG))
		return;

	if ((probe || probe_reply) && !skb_queue_empty(dfq))
		rcvgap = buf_seqno(skb_peek(dfq)) - l->rcv_nxt;

	skb = tipc_msg_create(LINK_PROTOCOL, mtyp, INT_H_SIZE,
			      tipc_max_domain_size + MAX_GAP_ACK_BLKS_SZ,
			      l->addr, tipc_own_addr(l->net), 0, 0, 0);
	if (!skb)
		return;

	hdr = buf_msg(skb);
	data = msg_data(hdr);
	msg_set_session(hdr, l->session);
	msg_set_bearer_id(hdr, l->bearer_id);
	msg_set_net_plane(hdr, l->net_plane);
	msg_set_next_sent(hdr, l->snd_nxt);
	msg_set_ack(hdr, l->rcv_nxt - 1);
	msg_set_bcast_ack(hdr, bcl->rcv_nxt - 1);
	msg_set_bc_ack_invalid(hdr, !node_up);
	msg_set_last_bcast(hdr, l->bc_sndlink->snd_nxt - 1);
	msg_set_link_tolerance(hdr, tolerance);
	msg_set_linkprio(hdr, priority);
	msg_set_redundant_link(hdr, node_up);
	msg_set_seq_gap(hdr, 0);
	msg_set_seqno(hdr, l->snd_nxt + U16_MAX / 2);

	if (mtyp == STATE_MSG) {
		if (l->peer_caps & TIPC_LINK_PROTO_SEQNO)
			msg_set_seqno(hdr, l->snd_nxt_state++);
		msg_set_seq_gap(hdr, rcvgap);
		bc_rcvgap = link_bc_rcv_gap(bcl);
		msg_set_bc_gap(hdr, bc_rcvgap);
		msg_set_probe(hdr, probe);
		msg_set_is_keepalive(hdr, probe || probe_reply);
		if (l->peer_caps & TIPC_GAP_ACK_BLOCK)
			glen = tipc_build_gap_ack_blks(l, hdr);
		tipc_mon_prep(l->net, data + glen, &dlen, mstate, l->bearer_id);
		msg_set_size(hdr, INT_H_SIZE + glen + dlen);
		skb_trim(skb, INT_H_SIZE + glen + dlen);
		l->stats.sent_states++;
		l->rcv_unacked = 0;
	} else {
		/* RESET_MSG or ACTIVATE_MSG */
		if (mtyp == ACTIVATE_MSG) {
			msg_set_dest_session_valid(hdr, 1);
			msg_set_dest_session(hdr, l->peer_session);
		}
		msg_set_max_pkt(hdr, l->advertised_mtu);
		strcpy(data, l->if_name);
		msg_set_size(hdr, INT_H_SIZE + TIPC_MAX_IF_NAME);
		skb_trim(skb, INT_H_SIZE + TIPC_MAX_IF_NAME);
	}
	if (probe)
		l->stats.sent_probes++;
	if (rcvgap)
		l->stats.sent_nacks++;
	if (bc_rcvgap)
		bcl->stats.sent_nacks++;
	skb->priority = TC_PRIO_CONTROL;
	__skb_queue_tail(xmitq, skb);
	trace_tipc_proto_build(skb, false, l->name);
}

void tipc_link_create_dummy_tnl_msg(struct tipc_link *l,
				    struct sk_buff_head *xmitq)
{
	u32 onode = tipc_own_addr(l->net);
	struct tipc_msg *hdr, *ihdr;
	struct sk_buff_head tnlq;
	struct sk_buff *skb;
	u32 dnode = l->addr;

	__skb_queue_head_init(&tnlq);
	skb = tipc_msg_create(TUNNEL_PROTOCOL, FAILOVER_MSG,
			      INT_H_SIZE, BASIC_H_SIZE,
			      dnode, onode, 0, 0, 0);
	if (!skb) {
		pr_warn("%sunable to create tunnel packet\n", link_co_err);
		return;
	}

	hdr = buf_msg(skb);
	msg_set_msgcnt(hdr, 1);
	msg_set_bearer_id(hdr, l->peer_bearer_id);

	ihdr = (struct tipc_msg *)msg_data(hdr);
	tipc_msg_init(onode, ihdr, TIPC_LOW_IMPORTANCE, TIPC_DIRECT_MSG,
		      BASIC_H_SIZE, dnode);
	msg_set_errcode(ihdr, TIPC_ERR_NO_PORT);
	__skb_queue_tail(&tnlq, skb);
	tipc_link_xmit(l, &tnlq, xmitq);
}

/* tipc_link_tnl_prepare(): prepare and return a list of tunnel packets
 * with contents of the link's transmit and backlog queues.
 */
void tipc_link_tnl_prepare(struct tipc_link *l, struct tipc_link *tnl,
			   int mtyp, struct sk_buff_head *xmitq)
{
	struct sk_buff_head *fdefq = &tnl->failover_deferdq;
	struct sk_buff *skb, *tnlskb;
	struct tipc_msg *hdr, tnlhdr;
	struct sk_buff_head *queue = &l->transmq;
	struct sk_buff_head tmpxq, tnlq, frags;
	u16 pktlen, pktcnt, seqno = l->snd_nxt;
	bool pktcnt_need_update = false;
	u16 syncpt;
	int rc;

	if (!tnl)
		return;

	__skb_queue_head_init(&tnlq);
	/* Link Synching:
	 * From now on, send only one single ("dummy") SYNCH message
	 * to peer. The SYNCH message does not contain any data, just
	 * a header conveying the synch point to the peer.
	 */
	if (mtyp == SYNCH_MSG && (tnl->peer_caps & TIPC_TUNNEL_ENHANCED)) {
		tnlskb = tipc_msg_create(TUNNEL_PROTOCOL, SYNCH_MSG,
					 INT_H_SIZE, 0, l->addr,
					 tipc_own_addr(l->net),
					 0, 0, 0);
		if (!tnlskb) {
			pr_warn("%sunable to create dummy SYNCH_MSG\n",
				link_co_err);
			return;
		}

		hdr = buf_msg(tnlskb);
		syncpt = l->snd_nxt + skb_queue_len(&l->backlogq) - 1;
		msg_set_syncpt(hdr, syncpt);
		msg_set_bearer_id(hdr, l->peer_bearer_id);
		__skb_queue_tail(&tnlq, tnlskb);
		tipc_link_xmit(tnl, &tnlq, xmitq);
		return;
	}

	__skb_queue_head_init(&tmpxq);
	__skb_queue_head_init(&frags);
	/* At least one packet required for safe algorithm => add dummy */
	skb = tipc_msg_create(TIPC_LOW_IMPORTANCE, TIPC_DIRECT_MSG,
			      BASIC_H_SIZE, 0, l->addr, tipc_own_addr(l->net),
			      0, 0, TIPC_ERR_NO_PORT);
	if (!skb) {
		pr_warn("%sunable to create tunnel packet\n", link_co_err);
		return;
	}
	__skb_queue_tail(&tnlq, skb);
	tipc_link_xmit(l, &tnlq, &tmpxq);
	__skb_queue_purge(&tmpxq);

	/* Initialize reusable tunnel packet header */
	tipc_msg_init(tipc_own_addr(l->net), &tnlhdr, TUNNEL_PROTOCOL,
		      mtyp, INT_H_SIZE, l->addr);
	if (mtyp == SYNCH_MSG)
		pktcnt = l->snd_nxt - buf_seqno(skb_peek(&l->transmq));
	else
		pktcnt = skb_queue_len(&l->transmq);
	pktcnt += skb_queue_len(&l->backlogq);
	msg_set_msgcnt(&tnlhdr, pktcnt);
	msg_set_bearer_id(&tnlhdr, l->peer_bearer_id);
tnl:
	/* Wrap each packet into a tunnel packet */
	skb_queue_walk(queue, skb) {
		hdr = buf_msg(skb);
		if (queue == &l->backlogq)
			msg_set_seqno(hdr, seqno++);
		pktlen = msg_size(hdr);

		/* Tunnel link MTU is not large enough? This could be
		 * due to:
		 * 1) Link MTU has just changed or set differently;
		 * 2) Or FAILOVER on the top of a SYNCH message
		 *
		 * The 2nd case should not happen if peer supports
		 * TIPC_TUNNEL_ENHANCED
		 */
		if (pktlen > tnl->mtu - INT_H_SIZE) {
			if (mtyp == FAILOVER_MSG &&
			    (tnl->peer_caps & TIPC_TUNNEL_ENHANCED)) {
				rc = tipc_msg_fragment(skb, &tnlhdr, tnl->mtu,
						       &frags);
				if (rc) {
					pr_warn("%sunable to frag msg: rc %d\n",
						link_co_err, rc);
					return;
				}
				pktcnt += skb_queue_len(&frags) - 1;
				pktcnt_need_update = true;
				skb_queue_splice_tail_init(&frags, &tnlq);
				continue;
			}
			/* Unluckily, peer doesn't have TIPC_TUNNEL_ENHANCED
			 * => Just warn it and return!
			 */
			pr_warn_ratelimited("%stoo large msg <%d, %d>: %d!\n",
					    link_co_err, msg_user(hdr),
					    msg_type(hdr), msg_size(hdr));
			return;
		}

		msg_set_size(&tnlhdr, pktlen + INT_H_SIZE);
		tnlskb = tipc_buf_acquire(pktlen + INT_H_SIZE, GFP_ATOMIC);
		if (!tnlskb) {
			pr_warn("%sunable to send packet\n", link_co_err);
			return;
		}
		skb_copy_to_linear_data(tnlskb, &tnlhdr, INT_H_SIZE);
		skb_copy_to_linear_data_offset(tnlskb, INT_H_SIZE, hdr, pktlen);
		__skb_queue_tail(&tnlq, tnlskb);
	}
	if (queue != &l->backlogq) {
		queue = &l->backlogq;
		goto tnl;
	}

	if (pktcnt_need_update)
		skb_queue_walk(&tnlq, skb) {
			hdr = buf_msg(skb);
			msg_set_msgcnt(hdr, pktcnt);
		}

	tipc_link_xmit(tnl, &tnlq, xmitq);

	if (mtyp == FAILOVER_MSG) {
		tnl->drop_point = l->rcv_nxt;
		tnl->failover_reasm_skb = l->reasm_buf;
		l->reasm_buf = NULL;

		/* Failover the link's deferdq */
		if (unlikely(!skb_queue_empty(fdefq))) {
			pr_warn("Link failover deferdq not empty: %d!\n",
				skb_queue_len(fdefq));
			__skb_queue_purge(fdefq);
		}
		skb_queue_splice_init(&l->deferdq, fdefq);
	}
}

/**
 * tipc_link_failover_prepare() - prepare tnl for link failover
 *
 * This is a special version of the precursor - tipc_link_tnl_prepare(),
 * see the tipc_node_link_failover() for details
 *
 * @l: failover link
 * @tnl: tunnel link
 * @xmitq: queue for messages to be xmited
 */
void tipc_link_failover_prepare(struct tipc_link *l, struct tipc_link *tnl,
				struct sk_buff_head *xmitq)
{
	struct sk_buff_head *fdefq = &tnl->failover_deferdq;

	tipc_link_create_dummy_tnl_msg(tnl, xmitq);

	/* This failover link endpoint was never established before,
	 * so it has not received anything from peer.
	 * Otherwise, it must be a normal failover situation or the
	 * node has entered SELF_DOWN_PEER_LEAVING and both peer nodes
	 * would have to start over from scratch instead.
	 */
	tnl->drop_point = 1;
	tnl->failover_reasm_skb = NULL;

	/* Initiate the link's failover deferdq */
	if (unlikely(!skb_queue_empty(fdefq))) {
		pr_warn("Link failover deferdq not empty: %d!\n",
			skb_queue_len(fdefq));
		__skb_queue_purge(fdefq);
	}
}

/* tipc_link_validate_msg(): validate message against current link state
 * Returns true if message should be accepted, otherwise false
 */
bool tipc_link_validate_msg(struct tipc_link *l, struct tipc_msg *hdr)
{
	u16 curr_session = l->peer_session;
	u16 session = msg_session(hdr);
	int mtyp = msg_type(hdr);

	if (msg_user(hdr) != LINK_PROTOCOL)
		return true;

	switch (mtyp) {
	case RESET_MSG:
		if (!l->in_session)
			return true;
		/* Accept only RESET with new session number */
		return more(session, curr_session);
	case ACTIVATE_MSG:
		if (!l->in_session)
			return true;
		/* Accept only ACTIVATE with new or current session number */
		return !less(session, curr_session);
	case STATE_MSG:
		/* Accept only STATE with current session number */
		if (!l->in_session)
			return false;
		if (session != curr_session)
			return false;
		/* Extra sanity check */
		if (!link_is_up(l) && msg_ack(hdr))
			return false;
		if (!(l->peer_caps & TIPC_LINK_PROTO_SEQNO))
			return true;
		/* Accept only STATE with new sequence number */
		return !less(msg_seqno(hdr), l->rcv_nxt_state);
	default:
		return false;
	}
}

/* tipc_link_proto_rcv(): receive link level protocol message :
 * Note that network plane id propagates through the network, and may
 * change at any time. The node with lowest numerical id determines
 * network plane
 */
static int tipc_link_proto_rcv(struct tipc_link *l, struct sk_buff *skb,
			       struct sk_buff_head *xmitq)
{
	struct tipc_msg *hdr = buf_msg(skb);
	struct tipc_gap_ack_blks *ga = NULL;
	bool reply = msg_probe(hdr), retransmitted = false;
	u32 dlen = msg_data_sz(hdr), glen = 0, msg_max;
	u16 peers_snd_nxt =  msg_next_sent(hdr);
	u16 peers_tol = msg_link_tolerance(hdr);
	u16 peers_prio = msg_linkprio(hdr);
	u16 gap = msg_seq_gap(hdr);
	u16 ack = msg_ack(hdr);
	u16 rcv_nxt = l->rcv_nxt;
	u16 rcvgap = 0;
	int mtyp = msg_type(hdr);
	int rc = 0, released;
	char *if_name;
	void *data;

	trace_tipc_proto_rcv(skb, false, l->name);

	if (dlen > U16_MAX)
		goto exit;

	if (tipc_link_is_blocked(l) || !xmitq)
		goto exit;

	if (tipc_own_addr(l->net) > msg_prevnode(hdr))
		l->net_plane = msg_net_plane(hdr);

	if (skb_linearize(skb))
		goto exit;

	hdr = buf_msg(skb);
	data = msg_data(hdr);

	if (!tipc_link_validate_msg(l, hdr)) {
		trace_tipc_skb_dump(skb, false, "PROTO invalid (1)!");
		trace_tipc_link_dump(l, TIPC_DUMP_NONE, "PROTO invalid (1)!");
		goto exit;
	}

	switch (mtyp) {
	case RESET_MSG:
	case ACTIVATE_MSG:
		msg_max = msg_max_pkt(hdr);
		if (msg_max < tipc_bearer_min_mtu(l->net, l->bearer_id))
			break;
		/* Complete own link name with peer's interface name */
		if_name =  strrchr(l->name, ':') + 1;
		if (sizeof(l->name) - (if_name - l->name) <= TIPC_MAX_IF_NAME)
			break;
		if (msg_data_sz(hdr) < TIPC_MAX_IF_NAME)
			break;
		strncpy(if_name, data, TIPC_MAX_IF_NAME);

		/* Update own tolerance if peer indicates a non-zero value */
		if (tipc_in_range(peers_tol, TIPC_MIN_LINK_TOL, TIPC_MAX_LINK_TOL)) {
			l->tolerance = peers_tol;
			l->bc_rcvlink->tolerance = peers_tol;
		}
		/* Update own priority if peer's priority is higher */
		if (tipc_in_range(peers_prio, l->priority + 1, TIPC_MAX_LINK_PRI))
			l->priority = peers_prio;

		/* If peer is going down we want full re-establish cycle */
		if (msg_peer_stopping(hdr)) {
			rc = tipc_link_fsm_evt(l, LINK_FAILURE_EVT);
			break;
		}

		/* If this endpoint was re-created while peer was ESTABLISHING
		 * it doesn't know current session number. Force re-synch.
		 */
		if (mtyp == ACTIVATE_MSG && msg_dest_session_valid(hdr) &&
		    l->session != msg_dest_session(hdr)) {
			if (less(l->session, msg_dest_session(hdr)))
				l->session = msg_dest_session(hdr) + 1;
			break;
		}

		/* ACTIVATE_MSG serves as PEER_RESET if link is already down */
		if (mtyp == RESET_MSG || !link_is_up(l))
			rc = tipc_link_fsm_evt(l, LINK_PEER_RESET_EVT);

		/* ACTIVATE_MSG takes up link if it was already locally reset */
		if (mtyp == ACTIVATE_MSG && l->state == LINK_ESTABLISHING)
			rc = TIPC_LINK_UP_EVT;

		l->peer_session = msg_session(hdr);
		l->in_session = true;
		l->peer_bearer_id = msg_bearer_id(hdr);
		if (l->mtu > msg_max)
			l->mtu = msg_max;
		break;

	case STATE_MSG:
		/* Validate Gap ACK blocks, drop if invalid */
		glen = tipc_get_gap_ack_blks(&ga, l, hdr, true);
		if (glen > dlen)
			break;

		l->rcv_nxt_state = msg_seqno(hdr) + 1;

		/* Update own tolerance if peer indicates a non-zero value */
		if (tipc_in_range(peers_tol, TIPC_MIN_LINK_TOL, TIPC_MAX_LINK_TOL)) {
			l->tolerance = peers_tol;
			l->bc_rcvlink->tolerance = peers_tol;
		}
		/* Update own prio if peer indicates a different value */
		if ((peers_prio != l->priority) &&
		    tipc_in_range(peers_prio, 1, TIPC_MAX_LINK_PRI)) {
			l->priority = peers_prio;
			rc = tipc_link_fsm_evt(l, LINK_FAILURE_EVT);
		}

		l->silent_intv_cnt = 0;
		l->stats.recv_states++;
		if (msg_probe(hdr))
			l->stats.recv_probes++;

		if (!link_is_up(l)) {
			if (l->state == LINK_ESTABLISHING)
				rc = TIPC_LINK_UP_EVT;
			break;
		}

		tipc_mon_rcv(l->net, data + glen, dlen - glen, l->addr,
			     &l->mon_state, l->bearer_id);

		/* Send NACK if peer has sent pkts we haven't received yet */
		if ((reply || msg_is_keepalive(hdr)) &&
		    more(peers_snd_nxt, rcv_nxt) &&
		    !tipc_link_is_synching(l) &&
		    skb_queue_empty(&l->deferdq))
			rcvgap = peers_snd_nxt - l->rcv_nxt;
		if (rcvgap || reply)
			tipc_link_build_proto_msg(l, STATE_MSG, 0, reply,
						  rcvgap, 0, 0, xmitq);

		released = tipc_link_advance_transmq(l, l, ack, gap, ga, xmitq,
						     &retransmitted, &rc);
		if (gap)
			l->stats.recv_nacks++;
		if (released || retransmitted)
			tipc_link_update_cwin(l, released, retransmitted);
		if (released)
			tipc_link_advance_backlog(l, xmitq);
		if (unlikely(!skb_queue_empty(&l->wakeupq)))
			link_prepare_wakeup(l);
	}
exit:
	kfree_skb(skb);
	return rc;
}

/* tipc_link_build_bc_proto_msg() - create broadcast protocol message
 */
static bool tipc_link_build_bc_proto_msg(struct tipc_link *l, bool bcast,
					 u16 peers_snd_nxt,
					 struct sk_buff_head *xmitq)
{
	struct sk_buff *skb;
	struct tipc_msg *hdr;
	struct sk_buff *dfrd_skb = skb_peek(&l->deferdq);
	u16 ack = l->rcv_nxt - 1;
	u16 gap_to = peers_snd_nxt - 1;

	skb = tipc_msg_create(BCAST_PROTOCOL, STATE_MSG, INT_H_SIZE,
			      0, l->addr, tipc_own_addr(l->net), 0, 0, 0);
	if (!skb)
		return false;
	hdr = buf_msg(skb);
	msg_set_last_bcast(hdr, l->bc_sndlink->snd_nxt - 1);
	msg_set_bcast_ack(hdr, ack);
	msg_set_bcgap_after(hdr, ack);
	if (dfrd_skb)
		gap_to = buf_seqno(dfrd_skb) - 1;
	msg_set_bcgap_to(hdr, gap_to);
	msg_set_non_seq(hdr, bcast);
	__skb_queue_tail(xmitq, skb);
	return true;
}

/* tipc_link_build_bc_init_msg() - synchronize broadcast link endpoints.
 *
 * Give a newly added peer node the sequence number where it should
 * start receiving and acking broadcast packets.
 */
static void tipc_link_build_bc_init_msg(struct tipc_link *l,
					struct sk_buff_head *xmitq)
{
	struct sk_buff_head list;

	__skb_queue_head_init(&list);
	if (!tipc_link_build_bc_proto_msg(l->bc_rcvlink, false, 0, &list))
		return;
	msg_set_bc_ack_invalid(buf_msg(skb_peek(&list)), true);
	tipc_link_xmit(l, &list, xmitq);
}

/* tipc_link_bc_init_rcv - receive initial broadcast synch data from peer
 */
void tipc_link_bc_init_rcv(struct tipc_link *l, struct tipc_msg *hdr)
{
	int mtyp = msg_type(hdr);
	u16 peers_snd_nxt = msg_bc_snd_nxt(hdr);

	if (link_is_up(l))
		return;

	if (msg_user(hdr) == BCAST_PROTOCOL) {
		l->rcv_nxt = peers_snd_nxt;
		l->state = LINK_ESTABLISHED;
		return;
	}

	if (l->peer_caps & TIPC_BCAST_SYNCH)
		return;

	if (msg_peer_node_is_up(hdr))
		return;

	/* Compatibility: accept older, less safe initial synch data */
	if ((mtyp == RESET_MSG) || (mtyp == ACTIVATE_MSG))
		l->rcv_nxt = peers_snd_nxt;
}

/* tipc_link_bc_sync_rcv - update rcv link according to peer's send state
 */
int tipc_link_bc_sync_rcv(struct tipc_link *l, struct tipc_msg *hdr,
			  struct sk_buff_head *xmitq)
{
	u16 peers_snd_nxt = msg_bc_snd_nxt(hdr);
	int rc = 0;

	if (!link_is_up(l))
		return rc;

	if (!msg_peer_node_is_up(hdr))
		return rc;

	/* Open when peer acknowledges our bcast init msg (pkt #1) */
	if (msg_ack(hdr))
		l->bc_peer_is_up = true;

	if (!l->bc_peer_is_up)
		return rc;

	/* Ignore if peers_snd_nxt goes beyond receive window */
	if (more(peers_snd_nxt, l->rcv_nxt + l->window))
		return rc;

	l->snd_nxt = peers_snd_nxt;
	if (link_bc_rcv_gap(l))
		rc |= TIPC_LINK_SND_STATE;

	/* Return now if sender supports nack via STATE messages */
	if (l->peer_caps & TIPC_BCAST_STATE_NACK)
		return rc;

	/* Otherwise, be backwards compatible */

	if (!more(peers_snd_nxt, l->rcv_nxt)) {
		l->nack_state = BC_NACK_SND_CONDITIONAL;
		return 0;
	}

	/* Don't NACK if one was recently sent or peeked */
	if (l->nack_state == BC_NACK_SND_SUPPRESS) {
		l->nack_state = BC_NACK_SND_UNCONDITIONAL;
		return 0;
	}

	/* Conditionally delay NACK sending until next synch rcv */
	if (l->nack_state == BC_NACK_SND_CONDITIONAL) {
		l->nack_state = BC_NACK_SND_UNCONDITIONAL;
		if ((peers_snd_nxt - l->rcv_nxt) < TIPC_MIN_LINK_WIN)
			return 0;
	}

	/* Send NACK now but suppress next one */
	tipc_link_build_bc_proto_msg(l, true, peers_snd_nxt, xmitq);
	l->nack_state = BC_NACK_SND_SUPPRESS;
	return 0;
}

int tipc_link_bc_ack_rcv(struct tipc_link *r, u16 acked, u16 gap,
			 struct tipc_gap_ack_blks *ga,
			 struct sk_buff_head *xmitq,
			 struct sk_buff_head *retrq)
{
	struct tipc_link *l = r->bc_sndlink;
	bool unused = false;
	int rc = 0;

	if (!link_is_up(r) || !r->bc_peer_is_up)
		return 0;

	if (gap) {
		l->stats.recv_nacks++;
		r->stats.recv_nacks++;
	}

	if (less(acked, r->acked) || (acked == r->acked && !gap && !ga))
		return 0;

	trace_tipc_link_bc_ack(r, acked, gap, &l->transmq);
	tipc_link_advance_transmq(l, r, acked, gap, ga, retrq, &unused, &rc);

	tipc_link_advance_backlog(l, xmitq);
	if (unlikely(!skb_queue_empty(&l->wakeupq)))
		link_prepare_wakeup(l);

	return rc;
}

/* tipc_link_bc_nack_rcv(): receive broadcast nack message
 * This function is here for backwards compatibility, since
 * no BCAST_PROTOCOL/STATE messages occur from TIPC v2.5.
 */
int tipc_link_bc_nack_rcv(struct tipc_link *l, struct sk_buff *skb,
			  struct sk_buff_head *xmitq)
{
	struct tipc_msg *hdr = buf_msg(skb);
	u32 dnode = msg_destnode(hdr);
	int mtyp = msg_type(hdr);
	u16 acked = msg_bcast_ack(hdr);
	u16 from = acked + 1;
	u16 to = msg_bcgap_to(hdr);
	u16 peers_snd_nxt = to + 1;
	int rc = 0;

	kfree_skb(skb);

	if (!tipc_link_is_up(l) || !l->bc_peer_is_up)
		return 0;

	if (mtyp != STATE_MSG)
		return 0;

	if (dnode == tipc_own_addr(l->net)) {
		rc = tipc_link_bc_ack_rcv(l, acked, to - acked, NULL, xmitq,
					  xmitq);
		l->stats.recv_nacks++;
		return rc;
	}

	/* Msg for other node => suppress own NACK at next sync if applicable */
	if (more(peers_snd_nxt, l->rcv_nxt) && !less(l->rcv_nxt, from))
		l->nack_state = BC_NACK_SND_SUPPRESS;

	return 0;
}

void tipc_link_set_queue_limits(struct tipc_link *l, u32 min_win, u32 max_win)
{
	int max_bulk = TIPC_MAX_PUBL / (l->mtu / ITEM_SIZE);

	l->min_win = min_win;
	l->ssthresh = max_win;
	l->max_win = max_win;
	l->window = min_win;
	l->backlog[TIPC_LOW_IMPORTANCE].limit      = min_win * 2;
	l->backlog[TIPC_MEDIUM_IMPORTANCE].limit   = min_win * 4;
	l->backlog[TIPC_HIGH_IMPORTANCE].limit     = min_win * 6;
	l->backlog[TIPC_CRITICAL_IMPORTANCE].limit = min_win * 8;
	l->backlog[TIPC_SYSTEM_IMPORTANCE].limit   = max_bulk;
}

/**
 * tipc_link_reset_stats - reset link statistics
 * @l: pointer to link
 */
void tipc_link_reset_stats(struct tipc_link *l)
{
	memset(&l->stats, 0, sizeof(l->stats));
}

static void link_print(struct tipc_link *l, const char *str)
{
	struct sk_buff *hskb = skb_peek(&l->transmq);
	u16 head = hskb ? msg_seqno(buf_msg(hskb)) : l->snd_nxt - 1;
	u16 tail = l->snd_nxt - 1;

	pr_info("%s Link <%s> state %x\n", str, l->name, l->state);
	pr_info("XMTQ: %u [%u-%u], BKLGQ: %u, SNDNX: %u, RCVNX: %u\n",
		skb_queue_len(&l->transmq), head, tail,
		skb_queue_len(&l->backlogq), l->snd_nxt, l->rcv_nxt);
}

/* Parse and validate nested (link) properties valid for media, bearer and link
 */
int tipc_nl_parse_link_prop(struct nlattr *prop, struct nlattr *props[])
{
	int err;

	err = nla_parse_nested_deprecated(props, TIPC_NLA_PROP_MAX, prop,
					  tipc_nl_prop_policy, NULL);
	if (err)
		return err;

	if (props[TIPC_NLA_PROP_PRIO]) {
		u32 prio;

		prio = nla_get_u32(props[TIPC_NLA_PROP_PRIO]);
		if (prio > TIPC_MAX_LINK_PRI)
			return -EINVAL;
	}

	if (props[TIPC_NLA_PROP_TOL]) {
		u32 tol;

		tol = nla_get_u32(props[TIPC_NLA_PROP_TOL]);
		if ((tol < TIPC_MIN_LINK_TOL) || (tol > TIPC_MAX_LINK_TOL))
			return -EINVAL;
	}

	if (props[TIPC_NLA_PROP_WIN]) {
		u32 max_win;

		max_win = nla_get_u32(props[TIPC_NLA_PROP_WIN]);
		if (max_win < TIPC_DEF_LINK_WIN || max_win > TIPC_MAX_LINK_WIN)
			return -EINVAL;
	}

	return 0;
}

static int __tipc_nl_add_stats(struct sk_buff *skb, struct tipc_stats *s)
{
	int i;
	struct nlattr *stats;

	struct nla_map {
		u32 key;
		u32 val;
	};

	struct nla_map map[] = {
		{TIPC_NLA_STATS_RX_INFO, 0},
		{TIPC_NLA_STATS_RX_FRAGMENTS, s->recv_fragments},
		{TIPC_NLA_STATS_RX_FRAGMENTED, s->recv_fragmented},
		{TIPC_NLA_STATS_RX_BUNDLES, s->recv_bundles},
		{TIPC_NLA_STATS_RX_BUNDLED, s->recv_bundled},
		{TIPC_NLA_STATS_TX_INFO, 0},
		{TIPC_NLA_STATS_TX_FRAGMENTS, s->sent_fragments},
		{TIPC_NLA_STATS_TX_FRAGMENTED, s->sent_fragmented},
		{TIPC_NLA_STATS_TX_BUNDLES, s->sent_bundles},
		{TIPC_NLA_STATS_TX_BUNDLED, s->sent_bundled},
		{TIPC_NLA_STATS_MSG_PROF_TOT, (s->msg_length_counts) ?
			s->msg_length_counts : 1},
		{TIPC_NLA_STATS_MSG_LEN_CNT, s->msg_length_counts},
		{TIPC_NLA_STATS_MSG_LEN_TOT, s->msg_lengths_total},
		{TIPC_NLA_STATS_MSG_LEN_P0, s->msg_length_profile[0]},
		{TIPC_NLA_STATS_MSG_LEN_P1, s->msg_length_profile[1]},
		{TIPC_NLA_STATS_MSG_LEN_P2, s->msg_length_profile[2]},
		{TIPC_NLA_STATS_MSG_LEN_P3, s->msg_length_profile[3]},
		{TIPC_NLA_STATS_MSG_LEN_P4, s->msg_length_profile[4]},
		{TIPC_NLA_STATS_MSG_LEN_P5, s->msg_length_profile[5]},
		{TIPC_NLA_STATS_MSG_LEN_P6, s->msg_length_profile[6]},
		{TIPC_NLA_STATS_RX_STATES, s->recv_states},
		{TIPC_NLA_STATS_RX_PROBES, s->recv_probes},
		{TIPC_NLA_STATS_RX_NACKS, s->recv_nacks},
		{TIPC_NLA_STATS_RX_DEFERRED, s->deferred_recv},
		{TIPC_NLA_STATS_TX_STATES, s->sent_states},
		{TIPC_NLA_STATS_TX_PROBES, s->sent_probes},
		{TIPC_NLA_STATS_TX_NACKS, s->sent_nacks},
		{TIPC_NLA_STATS_TX_ACKS, s->sent_acks},
		{TIPC_NLA_STATS_RETRANSMITTED, s->retransmitted},
		{TIPC_NLA_STATS_DUPLICATES, s->duplicates},
		{TIPC_NLA_STATS_LINK_CONGS, s->link_congs},
		{TIPC_NLA_STATS_MAX_QUEUE, s->max_queue_sz},
		{TIPC_NLA_STATS_AVG_QUEUE, s->queue_sz_counts ?
			(s->accu_queue_sz / s->queue_sz_counts) : 0}
	};

	stats = nla_nest_start_noflag(skb, TIPC_NLA_LINK_STATS);
	if (!stats)
		return -EMSGSIZE;

	for (i = 0; i <  ARRAY_SIZE(map); i++)
		if (nla_put_u32(skb, map[i].key, map[i].val))
			goto msg_full;

	nla_nest_end(skb, stats);

	return 0;
msg_full:
	nla_nest_cancel(skb, stats);

	return -EMSGSIZE;
}

/* Caller should hold appropriate locks to protect the link */
int __tipc_nl_add_link(struct net *net, struct tipc_nl_msg *msg,
		       struct tipc_link *link, int nlflags)
{
	u32 self = tipc_own_addr(net);
	struct nlattr *attrs;
	struct nlattr *prop;
	void *hdr;
	int err;

	hdr = genlmsg_put(msg->skb, msg->portid, msg->seq, &tipc_genl_family,
			  nlflags, TIPC_NL_LINK_GET);
	if (!hdr)
		return -EMSGSIZE;

	attrs = nla_nest_start_noflag(msg->skb, TIPC_NLA_LINK);
	if (!attrs)
		goto msg_full;

	if (nla_put_string(msg->skb, TIPC_NLA_LINK_NAME, link->name))
		goto attr_msg_full;
	if (nla_put_u32(msg->skb, TIPC_NLA_LINK_DEST, tipc_cluster_mask(self)))
		goto attr_msg_full;
	if (nla_put_u32(msg->skb, TIPC_NLA_LINK_MTU, link->mtu))
		goto attr_msg_full;
	if (nla_put_u32(msg->skb, TIPC_NLA_LINK_RX, link->stats.recv_pkts))
		goto attr_msg_full;
	if (nla_put_u32(msg->skb, TIPC_NLA_LINK_TX, link->stats.sent_pkts))
		goto attr_msg_full;

	if (tipc_link_is_up(link))
		if (nla_put_flag(msg->skb, TIPC_NLA_LINK_UP))
			goto attr_msg_full;
	if (link->active)
		if (nla_put_flag(msg->skb, TIPC_NLA_LINK_ACTIVE))
			goto attr_msg_full;

	prop = nla_nest_start_noflag(msg->skb, TIPC_NLA_LINK_PROP);
	if (!prop)
		goto attr_msg_full;
	if (nla_put_u32(msg->skb, TIPC_NLA_PROP_PRIO, link->priority))
		goto prop_msg_full;
	if (nla_put_u32(msg->skb, TIPC_NLA_PROP_TOL, link->tolerance))
		goto prop_msg_full;
	if (nla_put_u32(msg->skb, TIPC_NLA_PROP_WIN,
			link->window))
		goto prop_msg_full;
	if (nla_put_u32(msg->skb, TIPC_NLA_PROP_PRIO, link->priority))
		goto prop_msg_full;
	nla_nest_end(msg->skb, prop);

	err = __tipc_nl_add_stats(msg->skb, &link->stats);
	if (err)
		goto attr_msg_full;

	nla_nest_end(msg->skb, attrs);
	genlmsg_end(msg->skb, hdr);

	return 0;

prop_msg_full:
	nla_nest_cancel(msg->skb, prop);
attr_msg_full:
	nla_nest_cancel(msg->skb, attrs);
msg_full:
	genlmsg_cancel(msg->skb, hdr);

	return -EMSGSIZE;
}

static int __tipc_nl_add_bc_link_stat(struct sk_buff *skb,
				      struct tipc_stats *stats)
{
	int i;
	struct nlattr *nest;

	struct nla_map {
		__u32 key;
		__u32 val;
	};

	struct nla_map map[] = {
		{TIPC_NLA_STATS_RX_INFO, stats->recv_pkts},
		{TIPC_NLA_STATS_RX_FRAGMENTS, stats->recv_fragments},
		{TIPC_NLA_STATS_RX_FRAGMENTED, stats->recv_fragmented},
		{TIPC_NLA_STATS_RX_BUNDLES, stats->recv_bundles},
		{TIPC_NLA_STATS_RX_BUNDLED, stats->recv_bundled},
		{TIPC_NLA_STATS_TX_INFO, stats->sent_pkts},
		{TIPC_NLA_STATS_TX_FRAGMENTS, stats->sent_fragments},
		{TIPC_NLA_STATS_TX_FRAGMENTED, stats->sent_fragmented},
		{TIPC_NLA_STATS_TX_BUNDLES, stats->sent_bundles},
		{TIPC_NLA_STATS_TX_BUNDLED, stats->sent_bundled},
		{TIPC_NLA_STATS_RX_NACKS, stats->recv_nacks},
		{TIPC_NLA_STATS_RX_DEFERRED, stats->deferred_recv},
		{TIPC_NLA_STATS_TX_NACKS, stats->sent_nacks},
		{TIPC_NLA_STATS_TX_ACKS, stats->sent_acks},
		{TIPC_NLA_STATS_RETRANSMITTED, stats->retransmitted},
		{TIPC_NLA_STATS_DUPLICATES, stats->duplicates},
		{TIPC_NLA_STATS_LINK_CONGS, stats->link_congs},
		{TIPC_NLA_STATS_MAX_QUEUE, stats->max_queue_sz},
		{TIPC_NLA_STATS_AVG_QUEUE, stats->queue_sz_counts ?
			(stats->accu_queue_sz / stats->queue_sz_counts) : 0}
	};

	nest = nla_nest_start_noflag(skb, TIPC_NLA_LINK_STATS);
	if (!nest)
		return -EMSGSIZE;

	for (i = 0; i <  ARRAY_SIZE(map); i++)
		if (nla_put_u32(skb, map[i].key, map[i].val))
			goto msg_full;

	nla_nest_end(skb, nest);

	return 0;
msg_full:
	nla_nest_cancel(skb, nest);

	return -EMSGSIZE;
}

int tipc_nl_add_bc_link(struct net *net, struct tipc_nl_msg *msg,
			struct tipc_link *bcl)
{
	int err;
	void *hdr;
	struct nlattr *attrs;
	struct nlattr *prop;
	u32 bc_mode = tipc_bcast_get_mode(net);
	u32 bc_ratio = tipc_bcast_get_broadcast_ratio(net);

	if (!bcl)
		return 0;

	tipc_bcast_lock(net);

	hdr = genlmsg_put(msg->skb, msg->portid, msg->seq, &tipc_genl_family,
			  NLM_F_MULTI, TIPC_NL_LINK_GET);
	if (!hdr) {
		tipc_bcast_unlock(net);
		return -EMSGSIZE;
	}

	attrs = nla_nest_start_noflag(msg->skb, TIPC_NLA_LINK);
	if (!attrs)
		goto msg_full;

	/* The broadcast link is always up */
	if (nla_put_flag(msg->skb, TIPC_NLA_LINK_UP))
		goto attr_msg_full;

	if (nla_put_flag(msg->skb, TIPC_NLA_LINK_BROADCAST))
		goto attr_msg_full;
	if (nla_put_string(msg->skb, TIPC_NLA_LINK_NAME, bcl->name))
		goto attr_msg_full;
	if (nla_put_u32(msg->skb, TIPC_NLA_LINK_RX, 0))
		goto attr_msg_full;
	if (nla_put_u32(msg->skb, TIPC_NLA_LINK_TX, 0))
		goto attr_msg_full;

	prop = nla_nest_start_noflag(msg->skb, TIPC_NLA_LINK_PROP);
	if (!prop)
		goto attr_msg_full;
	if (nla_put_u32(msg->skb, TIPC_NLA_PROP_WIN, bcl->max_win))
		goto prop_msg_full;
	if (nla_put_u32(msg->skb, TIPC_NLA_PROP_BROADCAST, bc_mode))
		goto prop_msg_full;
	if (bc_mode & BCLINK_MODE_SEL)
		if (nla_put_u32(msg->skb, TIPC_NLA_PROP_BROADCAST_RATIO,
				bc_ratio))
			goto prop_msg_full;
	nla_nest_end(msg->skb, prop);

	err = __tipc_nl_add_bc_link_stat(msg->skb, &bcl->stats);
	if (err)
		goto attr_msg_full;

	tipc_bcast_unlock(net);
	nla_nest_end(msg->skb, attrs);
	genlmsg_end(msg->skb, hdr);

	return 0;

prop_msg_full:
	nla_nest_cancel(msg->skb, prop);
attr_msg_full:
	nla_nest_cancel(msg->skb, attrs);
msg_full:
	tipc_bcast_unlock(net);
	genlmsg_cancel(msg->skb, hdr);

	return -EMSGSIZE;
}

void tipc_link_set_tolerance(struct tipc_link *l, u32 tol,
			     struct sk_buff_head *xmitq)
{
	l->tolerance = tol;
	if (l->bc_rcvlink)
		l->bc_rcvlink->tolerance = tol;
	if (link_is_up(l))
		tipc_link_build_proto_msg(l, STATE_MSG, 0, 0, 0, tol, 0, xmitq);
}

void tipc_link_set_prio(struct tipc_link *l, u32 prio,
			struct sk_buff_head *xmitq)
{
	l->priority = prio;
	tipc_link_build_proto_msg(l, STATE_MSG, 0, 0, 0, 0, prio, xmitq);
}

void tipc_link_set_abort_limit(struct tipc_link *l, u32 limit)
{
	l->abort_limit = limit;
}

/**
 * tipc_link_dump - dump TIPC link data
 * @l: tipc link to be dumped
 * @dqueues: bitmask to decide if any link queue to be dumped?
 *           - TIPC_DUMP_NONE: don't dump link queues
 *           - TIPC_DUMP_TRANSMQ: dump link transmq queue
 *           - TIPC_DUMP_BACKLOGQ: dump link backlog queue
 *           - TIPC_DUMP_DEFERDQ: dump link deferd queue
 *           - TIPC_DUMP_INPUTQ: dump link input queue
 *           - TIPC_DUMP_WAKEUP: dump link wakeup queue
 *           - TIPC_DUMP_ALL: dump all the link queues above
 * @buf: returned buffer of dump data in format
 */
int tipc_link_dump(struct tipc_link *l, u16 dqueues, char *buf)
{
	int i = 0;
	size_t sz = (dqueues) ? LINK_LMAX : LINK_LMIN;
	struct sk_buff_head *list;
	struct sk_buff *hskb, *tskb;
	u32 len;

	if (!l) {
		i += scnprintf(buf, sz, "link data: (null)\n");
		return i;
	}

	i += scnprintf(buf, sz, "link data: %x", l->addr);
	i += scnprintf(buf + i, sz - i, " %x", l->state);
	i += scnprintf(buf + i, sz - i, " %u", l->in_session);
	i += scnprintf(buf + i, sz - i, " %u", l->session);
	i += scnprintf(buf + i, sz - i, " %u", l->peer_session);
	i += scnprintf(buf + i, sz - i, " %u", l->snd_nxt);
	i += scnprintf(buf + i, sz - i, " %u", l->rcv_nxt);
	i += scnprintf(buf + i, sz - i, " %u", l->snd_nxt_state);
	i += scnprintf(buf + i, sz - i, " %u", l->rcv_nxt_state);
	i += scnprintf(buf + i, sz - i, " %x", l->peer_caps);
	i += scnprintf(buf + i, sz - i, " %u", l->silent_intv_cnt);
	i += scnprintf(buf + i, sz - i, " %u", l->rst_cnt);
	i += scnprintf(buf + i, sz - i, " %u", 0);
	i += scnprintf(buf + i, sz - i, " %u", 0);
	i += scnprintf(buf + i, sz - i, " %u", l->acked);

	list = &l->transmq;
	len = skb_queue_len(list);
	hskb = skb_peek(list);
	tskb = skb_peek_tail(list);
	i += scnprintf(buf + i, sz - i, " | %u %u %u", len,
		       (hskb) ? msg_seqno(buf_msg(hskb)) : 0,
		       (tskb) ? msg_seqno(buf_msg(tskb)) : 0);

	list = &l->deferdq;
	len = skb_queue_len(list);
	hskb = skb_peek(list);
	tskb = skb_peek_tail(list);
	i += scnprintf(buf + i, sz - i, " | %u %u %u", len,
		       (hskb) ? msg_seqno(buf_msg(hskb)) : 0,
		       (tskb) ? msg_seqno(buf_msg(tskb)) : 0);

	list = &l->backlogq;
	len = skb_queue_len(list);
	hskb = skb_peek(list);
	tskb = skb_peek_tail(list);
	i += scnprintf(buf + i, sz - i, " | %u %u %u", len,
		       (hskb) ? msg_seqno(buf_msg(hskb)) : 0,
		       (tskb) ? msg_seqno(buf_msg(tskb)) : 0);

	list = l->inputq;
	len = skb_queue_len(list);
	hskb = skb_peek(list);
	tskb = skb_peek_tail(list);
	i += scnprintf(buf + i, sz - i, " | %u %u %u\n", len,
		       (hskb) ? msg_seqno(buf_msg(hskb)) : 0,
		       (tskb) ? msg_seqno(buf_msg(tskb)) : 0);

	if (dqueues & TIPC_DUMP_TRANSMQ) {
		i += scnprintf(buf + i, sz - i, "transmq: ");
		i += tipc_list_dump(&l->transmq, false, buf + i);
	}
	if (dqueues & TIPC_DUMP_BACKLOGQ) {
		i += scnprintf(buf + i, sz - i,
			       "backlogq: <%u %u %u %u %u>, ",
			       l->backlog[TIPC_LOW_IMPORTANCE].len,
			       l->backlog[TIPC_MEDIUM_IMPORTANCE].len,
			       l->backlog[TIPC_HIGH_IMPORTANCE].len,
			       l->backlog[TIPC_CRITICAL_IMPORTANCE].len,
			       l->backlog[TIPC_SYSTEM_IMPORTANCE].len);
		i += tipc_list_dump(&l->backlogq, false, buf + i);
	}
	if (dqueues & TIPC_DUMP_DEFERDQ) {
		i += scnprintf(buf + i, sz - i, "deferdq: ");
		i += tipc_list_dump(&l->deferdq, false, buf + i);
	}
	if (dqueues & TIPC_DUMP_INPUTQ) {
		i += scnprintf(buf + i, sz - i, "inputq: ");
		i += tipc_list_dump(l->inputq, false, buf + i);
	}
	if (dqueues & TIPC_DUMP_WAKEUP) {
		i += scnprintf(buf + i, sz - i, "wakeup: ");
		i += tipc_list_dump(&l->wakeupq, false, buf + i);
	}

	return i;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
