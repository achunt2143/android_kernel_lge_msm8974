/*
 * net/tipc/bcast.c: TIPC broadcast code
 *
<<<<<<< HEAD
 * Copyright (c) 2004-2006, Ericsson AB
=======
 * Copyright (c) 2004-2006, 2014-2017, Ericsson AB
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Copyright (c) 2004, Intel Corporation.
 * Copyright (c) 2005, 2010-2011, Wind River Systems
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

<<<<<<< HEAD
#include "core.h"
#include "link.h"
#include "port.h"
#include "bcast.h"
#include "name_distr.h"

#define MAX_PKT_DEFAULT_MCAST 1500	/* bcast link max packet size (fixed) */

#define BCLINK_WIN_DEFAULT 20		/* bcast link window size (default) */

/**
 * struct tipc_bcbearer_pair - a pair of bearers used by broadcast link
 * @primary: pointer to primary bearer
 * @secondary: pointer to secondary bearer
 *
 * Bearers must have same priority and same set of reachable destinations
 * to be paired.
 */

struct tipc_bcbearer_pair {
	struct tipc_bearer *primary;
	struct tipc_bearer *secondary;
};

/**
 * struct tipc_bcbearer - bearer used by broadcast link
 * @bearer: (non-standard) broadcast bearer structure
 * @media: (non-standard) broadcast media structure
 * @bpairs: array of bearer pairs
 * @bpairs_temp: temporary array of bearer pairs used by tipc_bcbearer_sort()
 * @remains: temporary node map used by tipc_bcbearer_send()
 * @remains_new: temporary node map used tipc_bcbearer_send()
 *
 * Note: The fields labelled "temporary" are incorporated into the bearer
 * to avoid consuming potentially limited stack space through the use of
 * large local variables within multicast routines.  Concurrent access is
 * prevented through use of the spinlock "bc_lock".
 */

struct tipc_bcbearer {
	struct tipc_bearer bearer;
	struct tipc_media media;
	struct tipc_bcbearer_pair bpairs[MAX_BEARERS];
	struct tipc_bcbearer_pair bpairs_temp[TIPC_MAX_LINK_PRI + 1];
	struct tipc_node_map remains;
	struct tipc_node_map remains_new;
};

/**
 * struct tipc_bclink - link used for broadcast messages
 * @link: (non-standard) broadcast link structure
 * @node: (non-standard) node structure representing b'cast link's peer node
 * @bcast_nodes: map of broadcast-capable nodes
 * @retransmit_to: node that most recently requested a retransmit
 *
 * Handles sequence numbering, fragmentation, bundling, etc.
 */

struct tipc_bclink {
	struct tipc_link link;
	struct tipc_node node;
	struct tipc_node_map bcast_nodes;
	struct tipc_node *retransmit_to;
};

static struct tipc_bcbearer bcast_bearer;
static struct tipc_bclink bcast_link;

static struct tipc_bcbearer *bcbearer = &bcast_bearer;
static struct tipc_bclink *bclink = &bcast_link;
static struct tipc_link *bcl = &bcast_link.link;

static DEFINE_SPINLOCK(bc_lock);

const char tipc_bclink_name[] = "broadcast-link";

static void tipc_nmap_diff(struct tipc_node_map *nm_a,
			   struct tipc_node_map *nm_b,
			   struct tipc_node_map *nm_diff);

static u32 bcbuf_acks(struct sk_buff *buf)
{
	return (u32)(unsigned long)TIPC_SKB_CB(buf)->handle;
}

static void bcbuf_set_acks(struct sk_buff *buf, u32 acks)
{
	TIPC_SKB_CB(buf)->handle = (void *)(unsigned long)acks;
}

static void bcbuf_decr_acks(struct sk_buff *buf)
{
	bcbuf_set_acks(buf, bcbuf_acks(buf) - 1);
}

void tipc_bclink_add_node(u32 addr)
{
	spin_lock_bh(&bc_lock);
	tipc_nmap_add(&bclink->bcast_nodes, addr);
	spin_unlock_bh(&bc_lock);
}

void tipc_bclink_remove_node(u32 addr)
{
	spin_lock_bh(&bc_lock);
	tipc_nmap_remove(&bclink->bcast_nodes, addr);
	spin_unlock_bh(&bc_lock);
}

static void bclink_set_last_sent(void)
{
	if (bcl->next_out)
		bcl->fsm_msg_cnt = mod(buf_seqno(bcl->next_out) - 1);
	else
		bcl->fsm_msg_cnt = mod(bcl->next_out_no - 1);
}

u32 tipc_bclink_get_last_sent(void)
{
	return bcl->fsm_msg_cnt;
}

static void bclink_update_last_sent(struct tipc_node *node, u32 seqno)
{
	node->bclink.last_sent = less_eq(node->bclink.last_sent, seqno) ?
						seqno : node->bclink.last_sent;
}


/*
 * tipc_bclink_retransmit_to - get most recent node to request retransmission
 *
 * Called with bc_lock locked
 */

struct tipc_node *tipc_bclink_retransmit_to(void)
{
	return bclink->retransmit_to;
}

/**
 * bclink_retransmit_pkt - retransmit broadcast packets
 * @after: sequence number of last packet to *not* retransmit
 * @to: sequence number of last packet to retransmit
 *
 * Called with bc_lock locked
 */

static void bclink_retransmit_pkt(u32 after, u32 to)
{
	struct sk_buff *buf;

	buf = bcl->first_out;
	while (buf && less_eq(buf_seqno(buf), after))
		buf = buf->next;
	tipc_link_retransmit(bcl, buf, mod(to - after));
}

/**
 * tipc_bclink_acknowledge - handle acknowledgement of broadcast packets
 * @n_ptr: node that sent acknowledgement info
 * @acked: broadcast sequence # that has been acknowledged
 *
 * Node is locked, bc_lock unlocked.
 */

void tipc_bclink_acknowledge(struct tipc_node *n_ptr, u32 acked)
{
	struct sk_buff *crs;
	struct sk_buff *next;
	unsigned int released = 0;

	spin_lock_bh(&bc_lock);

	/* Bail out if tx queue is empty (no clean up is required) */
	crs = bcl->first_out;
	if (!crs)
		goto exit;

	/* Determine which messages need to be acknowledged */
	if (acked == INVALID_LINK_SEQ) {
		/*
		 * Contact with specified node has been lost, so need to
		 * acknowledge sent messages only (if other nodes still exist)
		 * or both sent and unsent messages (otherwise)
		 */
		if (bclink->bcast_nodes.count)
			acked = bcl->fsm_msg_cnt;
		else
			acked = bcl->next_out_no;
	} else {
		/*
		 * Bail out if specified sequence number does not correspond
		 * to a message that has been sent and not yet acknowledged
		 */
		if (less(acked, buf_seqno(crs)) ||
		    less(bcl->fsm_msg_cnt, acked) ||
		    less_eq(acked, n_ptr->bclink.acked))
			goto exit;
	}

	/* Skip over packets that node has previously acknowledged */
	while (crs && less_eq(buf_seqno(crs), n_ptr->bclink.acked))
		crs = crs->next;

	/* Update packets that node is now acknowledging */

	while (crs && less_eq(buf_seqno(crs), acked)) {
		next = crs->next;

		if (crs != bcl->next_out)
			bcbuf_decr_acks(crs);
		else {
			bcbuf_set_acks(crs, 0);
			bcl->next_out = next;
			bclink_set_last_sent();
		}

		if (bcbuf_acks(crs) == 0) {
			bcl->first_out = next;
			bcl->out_queue_size--;
			kfree_skb(crs);
			released = 1;
		}
		crs = next;
	}
	n_ptr->bclink.acked = acked;

	/* Try resolving broadcast link congestion, if necessary */

	if (unlikely(bcl->next_out)) {
		tipc_link_push_queue(bcl);
		bclink_set_last_sent();
	}
	if (unlikely(released && !list_empty(&bcl->waiting_ports)))
		tipc_link_wakeup_ports(bcl, 0);
exit:
	spin_unlock_bh(&bc_lock);
}

/*
 * tipc_bclink_update_link_state - update broadcast link state
 *
 * tipc_net_lock and node lock set
 */

void tipc_bclink_update_link_state(struct tipc_node *n_ptr, u32 last_sent)
{
	struct sk_buff *buf;

	/* Ignore "stale" link state info */

	if (less_eq(last_sent, n_ptr->bclink.last_in))
		return;

	/* Update link synchronization state; quit if in sync */

	bclink_update_last_sent(n_ptr, last_sent);

	if (n_ptr->bclink.last_sent == n_ptr->bclink.last_in)
		return;

	/* Update out-of-sync state; quit if loss is still unconfirmed */

	if ((++n_ptr->bclink.oos_state) == 1) {
		if (n_ptr->bclink.deferred_size < (TIPC_MIN_LINK_WIN / 2))
			return;
		n_ptr->bclink.oos_state++;
	}

	/* Don't NACK if one has been recently sent (or seen) */

	if (n_ptr->bclink.oos_state & 0x1)
		return;

	/* Send NACK */

	buf = tipc_buf_acquire(INT_H_SIZE);
	if (buf) {
		struct tipc_msg *msg = buf_msg(buf);

		tipc_msg_init(msg, BCAST_PROTOCOL, STATE_MSG,
			      INT_H_SIZE, n_ptr->addr);
		msg_set_non_seq(msg, 1);
		msg_set_mc_netid(msg, tipc_net_id);
		msg_set_bcast_ack(msg, n_ptr->bclink.last_in);
		msg_set_bcgap_after(msg, n_ptr->bclink.last_in);
		msg_set_bcgap_to(msg, n_ptr->bclink.deferred_head
				 ? buf_seqno(n_ptr->bclink.deferred_head) - 1
				 : n_ptr->bclink.last_sent);

		spin_lock_bh(&bc_lock);
		tipc_bearer_send(&bcbearer->bearer, buf, NULL);
		bcl->stats.sent_nacks++;
		spin_unlock_bh(&bc_lock);
		kfree_skb(buf);

		n_ptr->bclink.oos_state++;
	}
}

/*
 * bclink_peek_nack - monitor retransmission requests sent by other nodes
 *
 * Delay any upcoming NACK by this node if another node has already
 * requested the first message this node is going to ask for.
 *
 * Only tipc_net_lock set.
 */

static void bclink_peek_nack(struct tipc_msg *msg)
{
	struct tipc_node *n_ptr = tipc_node_find(msg_destnode(msg));

	if (unlikely(!n_ptr))
		return;

	tipc_node_lock(n_ptr);

	if (n_ptr->bclink.supported &&
	    (n_ptr->bclink.last_in != n_ptr->bclink.last_sent) &&
	    (n_ptr->bclink.last_in == msg_bcgap_after(msg)))
		n_ptr->bclink.oos_state = 2;

	tipc_node_unlock(n_ptr);
}

/*
 * tipc_bclink_send_msg - broadcast a packet to all nodes in cluster
 */

int tipc_bclink_send_msg(struct sk_buff *buf)
{
	int res;

	spin_lock_bh(&bc_lock);

	if (!bclink->bcast_nodes.count) {
		res = msg_data_sz(buf_msg(buf));
		kfree_skb(buf);
		goto exit;
	}

	res = tipc_link_send_buf(bcl, buf);
	if (likely(res >= 0)) {
		bclink_set_last_sent();
		bcl->stats.queue_sz_counts++;
		bcl->stats.accu_queue_sz += bcl->out_queue_size;
	}
exit:
	spin_unlock_bh(&bc_lock);
	return res;
}

/*
 * bclink_accept_pkt - accept an incoming, in-sequence broadcast packet
 *
 * Called with both sending node's lock and bc_lock taken.
 */

static void bclink_accept_pkt(struct tipc_node *node, u32 seqno)
{
	bclink_update_last_sent(node, seqno);
	node->bclink.last_in = seqno;
	node->bclink.oos_state = 0;
	bcl->stats.recv_info++;

	/*
	 * Unicast an ACK periodically, ensuring that
	 * all nodes in the cluster don't ACK at the same time
	 */

	if (((seqno - tipc_own_addr) % TIPC_MIN_LINK_WIN) == 0) {
		tipc_link_send_proto_msg(
			node->active_links[node->addr & 1],
			STATE_MSG, 0, 0, 0, 0, 0);
		bcl->stats.sent_acks++;
	}
}

/*
 * tipc_bclink_recv_pkt - receive a broadcast packet, and deliver upwards
 *
 * tipc_net_lock is read_locked, no other locks set
 */

void tipc_bclink_recv_pkt(struct sk_buff *buf)
{
	struct tipc_msg *msg = buf_msg(buf);
	struct tipc_node *node;
	u32 next_in;
	u32 seqno;
	int deferred;

	/* Screen out unwanted broadcast messages */

	if (msg_mc_netid(msg) != tipc_net_id)
		goto exit;

	node = tipc_node_find(msg_prevnode(msg));
	if (unlikely(!node))
		goto exit;

	tipc_node_lock(node);
	if (unlikely(!node->bclink.supported))
		goto unlock;

	/* Handle broadcast protocol message */

	if (unlikely(msg_user(msg) == BCAST_PROTOCOL)) {
		if (msg_type(msg) != STATE_MSG)
			goto unlock;
		if (msg_destnode(msg) == tipc_own_addr) {
			tipc_bclink_acknowledge(node, msg_bcast_ack(msg));
			tipc_node_unlock(node);
			spin_lock_bh(&bc_lock);
			bcl->stats.recv_nacks++;
			bclink->retransmit_to = node;
			bclink_retransmit_pkt(msg_bcgap_after(msg),
					      msg_bcgap_to(msg));
			spin_unlock_bh(&bc_lock);
		} else {
			tipc_node_unlock(node);
			bclink_peek_nack(msg);
		}
		goto exit;
	}

	/* Handle in-sequence broadcast message */

	seqno = msg_seqno(msg);
	next_in = mod(node->bclink.last_in + 1);

	if (likely(seqno == next_in)) {
receive:
		/* Deliver message to destination */

		if (likely(msg_isdata(msg))) {
			spin_lock_bh(&bc_lock);
			bclink_accept_pkt(node, seqno);
			spin_unlock_bh(&bc_lock);
			tipc_node_unlock(node);
			if (likely(msg_mcast(msg)))
				tipc_port_recv_mcast(buf, NULL);
			else
				kfree_skb(buf);
		} else if (msg_user(msg) == MSG_BUNDLER) {
			spin_lock_bh(&bc_lock);
			bclink_accept_pkt(node, seqno);
			bcl->stats.recv_bundles++;
			bcl->stats.recv_bundled += msg_msgcnt(msg);
			spin_unlock_bh(&bc_lock);
			tipc_node_unlock(node);
			tipc_link_recv_bundle(buf);
		} else if (msg_user(msg) == MSG_FRAGMENTER) {
			int ret = tipc_link_recv_fragment(&node->bclink.defragm,
						      &buf, &msg);
			if (ret < 0)
				goto unlock;
			spin_lock_bh(&bc_lock);
			bclink_accept_pkt(node, seqno);
			bcl->stats.recv_fragments++;
			if (ret > 0)
				bcl->stats.recv_fragmented++;
			spin_unlock_bh(&bc_lock);
			tipc_node_unlock(node);
			tipc_net_route_msg(buf);
		} else if (msg_user(msg) == NAME_DISTRIBUTOR) {
			spin_lock_bh(&bc_lock);
			bclink_accept_pkt(node, seqno);
			spin_unlock_bh(&bc_lock);
			tipc_node_unlock(node);
			tipc_named_recv(buf);
		} else {
			spin_lock_bh(&bc_lock);
			bclink_accept_pkt(node, seqno);
			spin_unlock_bh(&bc_lock);
			tipc_node_unlock(node);
			kfree_skb(buf);
		}
		buf = NULL;

		/* Determine new synchronization state */

		tipc_node_lock(node);
		if (unlikely(!tipc_node_is_up(node)))
			goto unlock;

		if (node->bclink.last_in == node->bclink.last_sent)
			goto unlock;

		if (!node->bclink.deferred_head) {
			node->bclink.oos_state = 1;
			goto unlock;
		}

		msg = buf_msg(node->bclink.deferred_head);
		seqno = msg_seqno(msg);
		next_in = mod(next_in + 1);
		if (seqno != next_in)
			goto unlock;

		/* Take in-sequence message from deferred queue & deliver it */

		buf = node->bclink.deferred_head;
		node->bclink.deferred_head = buf->next;
		buf->next = NULL;
		node->bclink.deferred_size--;
		goto receive;
	}

	/* Handle out-of-sequence broadcast message */

	if (less(next_in, seqno)) {
		deferred = tipc_link_defer_pkt(&node->bclink.deferred_head,
					       &node->bclink.deferred_tail,
					       buf);
		node->bclink.deferred_size += deferred;
		bclink_update_last_sent(node, seqno);
		buf = NULL;
	} else
		deferred = 0;

	spin_lock_bh(&bc_lock);

	if (deferred)
		bcl->stats.deferred_recv++;
	else
		bcl->stats.duplicates++;

	spin_unlock_bh(&bc_lock);

unlock:
	tipc_node_unlock(node);
exit:
	kfree_skb(buf);
}

u32 tipc_bclink_acks_missing(struct tipc_node *n_ptr)
{
	return (n_ptr->bclink.supported &&
		(tipc_bclink_get_last_sent() != n_ptr->bclink.acked));
}


/**
 * tipc_bcbearer_send - send a packet through the broadcast pseudo-bearer
 *
 * Send packet over as many bearers as necessary to reach all nodes
 * that have joined the broadcast link.
 *
 * Returns 0 (packet sent successfully) under all circumstances,
 * since the broadcast link's pseudo-bearer never blocks
 */

static int tipc_bcbearer_send(struct sk_buff *buf,
			      struct tipc_bearer *unused1,
			      struct tipc_media_addr *unused2)
{
	int bp_index;

	/*
	 * Prepare broadcast link message for reliable transmission,
	 * if first time trying to send it;
	 * preparation is skipped for broadcast link protocol messages
	 * since they are sent in an unreliable manner and don't need it
	 */

	if (likely(!msg_non_seq(buf_msg(buf)))) {
		struct tipc_msg *msg;

		bcbuf_set_acks(buf, bclink->bcast_nodes.count);
		msg = buf_msg(buf);
		msg_set_non_seq(msg, 1);
		msg_set_mc_netid(msg, tipc_net_id);
		bcl->stats.sent_info++;

		if (WARN_ON(!bclink->bcast_nodes.count)) {
			dump_stack();
			return 0;
		}
	}

	/* Send buffer over bearers until all targets reached */

	bcbearer->remains = bclink->bcast_nodes;

	for (bp_index = 0; bp_index < MAX_BEARERS; bp_index++) {
		struct tipc_bearer *p = bcbearer->bpairs[bp_index].primary;
		struct tipc_bearer *s = bcbearer->bpairs[bp_index].secondary;

		if (!p)
			break;	/* no more bearers to try */

		tipc_nmap_diff(&bcbearer->remains, &p->nodes, &bcbearer->remains_new);
		if (bcbearer->remains_new.count == bcbearer->remains.count)
			continue;	/* bearer pair doesn't add anything */

		if (p->blocked ||
		    p->media->send_msg(buf, p, &p->media->bcast_addr)) {
			/* unable to send on primary bearer */
			if (!s || s->blocked ||
			    s->media->send_msg(buf, s,
					       &s->media->bcast_addr)) {
				/* unable to send on either bearer */
				continue;
			}
		}

		if (s) {
			bcbearer->bpairs[bp_index].primary = s;
			bcbearer->bpairs[bp_index].secondary = p;
		}

		if (bcbearer->remains_new.count == 0)
			break;	/* all targets reached */

		bcbearer->remains = bcbearer->remains_new;
	}

	return 0;
}

/**
 * tipc_bcbearer_sort - create sets of bearer pairs used by broadcast bearer
 */

void tipc_bcbearer_sort(void)
{
	struct tipc_bcbearer_pair *bp_temp = bcbearer->bpairs_temp;
	struct tipc_bcbearer_pair *bp_curr;
	int b_index;
	int pri;

	spin_lock_bh(&bc_lock);

	/* Group bearers by priority (can assume max of two per priority) */

	memset(bp_temp, 0, sizeof(bcbearer->bpairs_temp));

	for (b_index = 0; b_index < MAX_BEARERS; b_index++) {
		struct tipc_bearer *b = &tipc_bearers[b_index];

		if (!b->active || !b->nodes.count)
			continue;

		if (!bp_temp[b->priority].primary)
			bp_temp[b->priority].primary = b;
		else
			bp_temp[b->priority].secondary = b;
	}

	/* Create array of bearer pairs for broadcasting */

	bp_curr = bcbearer->bpairs;
	memset(bcbearer->bpairs, 0, sizeof(bcbearer->bpairs));

	for (pri = TIPC_MAX_LINK_PRI; pri >= 0; pri--) {

		if (!bp_temp[pri].primary)
			continue;

		bp_curr->primary = bp_temp[pri].primary;

		if (bp_temp[pri].secondary) {
			if (tipc_nmap_equal(&bp_temp[pri].primary->nodes,
					    &bp_temp[pri].secondary->nodes)) {
				bp_curr->secondary = bp_temp[pri].secondary;
			} else {
				bp_curr++;
				bp_curr->primary = bp_temp[pri].secondary;
			}
		}

		bp_curr++;
	}

	spin_unlock_bh(&bc_lock);
}


int tipc_bclink_stats(char *buf, const u32 buf_size)
{
	struct print_buf pb;

	if (!bcl)
		return 0;

	tipc_printbuf_init(&pb, buf, buf_size);

	spin_lock_bh(&bc_lock);

	tipc_printf(&pb, "Link <%s>\n"
			 "  Window:%u packets\n",
		    bcl->name, bcl->queue_limit[0]);
	tipc_printf(&pb, "  RX packets:%u fragments:%u/%u bundles:%u/%u\n",
		    bcl->stats.recv_info,
		    bcl->stats.recv_fragments,
		    bcl->stats.recv_fragmented,
		    bcl->stats.recv_bundles,
		    bcl->stats.recv_bundled);
	tipc_printf(&pb, "  TX packets:%u fragments:%u/%u bundles:%u/%u\n",
		    bcl->stats.sent_info,
		    bcl->stats.sent_fragments,
		    bcl->stats.sent_fragmented,
		    bcl->stats.sent_bundles,
		    bcl->stats.sent_bundled);
	tipc_printf(&pb, "  RX naks:%u defs:%u dups:%u\n",
		    bcl->stats.recv_nacks,
		    bcl->stats.deferred_recv,
		    bcl->stats.duplicates);
	tipc_printf(&pb, "  TX naks:%u acks:%u dups:%u\n",
		    bcl->stats.sent_nacks,
		    bcl->stats.sent_acks,
		    bcl->stats.retransmitted);
	tipc_printf(&pb, "  Congestion bearer:%u link:%u  Send queue max:%u avg:%u\n",
		    bcl->stats.bearer_congs,
		    bcl->stats.link_congs,
		    bcl->stats.max_queue_sz,
		    bcl->stats.queue_sz_counts
		    ? (bcl->stats.accu_queue_sz / bcl->stats.queue_sz_counts)
		    : 0);

	spin_unlock_bh(&bc_lock);
	return tipc_printbuf_validate(&pb);
}

int tipc_bclink_reset_stats(void)
{
	if (!bcl)
		return -ENOPROTOOPT;

	spin_lock_bh(&bc_lock);
	memset(&bcl->stats, 0, sizeof(bcl->stats));
	spin_unlock_bh(&bc_lock);
	return 0;
}

int tipc_bclink_set_queue_limits(u32 limit)
{
	if (!bcl)
		return -ENOPROTOOPT;
	if ((limit < TIPC_MIN_LINK_WIN) || (limit > TIPC_MAX_LINK_WIN))
		return -EINVAL;

	spin_lock_bh(&bc_lock);
	tipc_link_set_queue_limits(bcl, limit);
	spin_unlock_bh(&bc_lock);
	return 0;
}

void tipc_bclink_init(void)
{
	INIT_LIST_HEAD(&bcbearer->bearer.cong_links);
	bcbearer->bearer.media = &bcbearer->media;
	bcbearer->media.send_msg = tipc_bcbearer_send;
	sprintf(bcbearer->media.name, "tipc-broadcast");

	INIT_LIST_HEAD(&bcl->waiting_ports);
	bcl->next_out_no = 1;
	spin_lock_init(&bclink->node.lock);
	bcl->owner = &bclink->node;
	bcl->max_pkt = MAX_PKT_DEFAULT_MCAST;
	tipc_link_set_queue_limits(bcl, BCLINK_WIN_DEFAULT);
	bcl->b_ptr = &bcbearer->bearer;
	bcl->state = WORKING_WORKING;
	strlcpy(bcl->name, tipc_bclink_name, TIPC_MAX_LINK_NAME);
}

void tipc_bclink_stop(void)
{
	spin_lock_bh(&bc_lock);
	tipc_link_stop(bcl);
	spin_unlock_bh(&bc_lock);

	memset(bclink, 0, sizeof(*bclink));
	memset(bcbearer, 0, sizeof(*bcbearer));
}


/**
 * tipc_nmap_add - add a node to a node map
 */

void tipc_nmap_add(struct tipc_node_map *nm_ptr, u32 node)
{
	int n = tipc_node(node);
	int w = n / WSIZE;
	u32 mask = (1 << (n % WSIZE));

	if ((nm_ptr->map[w] & mask) == 0) {
		nm_ptr->count++;
		nm_ptr->map[w] |= mask;
	}
}

/**
 * tipc_nmap_remove - remove a node from a node map
 */

void tipc_nmap_remove(struct tipc_node_map *nm_ptr, u32 node)
{
	int n = tipc_node(node);
	int w = n / WSIZE;
	u32 mask = (1 << (n % WSIZE));

	if ((nm_ptr->map[w] & mask) != 0) {
		nm_ptr->map[w] &= ~mask;
		nm_ptr->count--;
	}
}

/**
 * tipc_nmap_diff - find differences between node maps
 * @nm_a: input node map A
 * @nm_b: input node map B
 * @nm_diff: output node map A-B (i.e. nodes of A that are not in B)
 */

static void tipc_nmap_diff(struct tipc_node_map *nm_a,
			   struct tipc_node_map *nm_b,
			   struct tipc_node_map *nm_diff)
{
	int stop = ARRAY_SIZE(nm_a->map);
	int w;
	int b;
	u32 map;

	memset(nm_diff, 0, sizeof(*nm_diff));
	for (w = 0; w < stop; w++) {
		map = nm_a->map[w] ^ (nm_a->map[w] & nm_b->map[w]);
		nm_diff->map[w] = map;
		if (map != 0) {
			for (b = 0 ; b < WSIZE; b++) {
				if (map & (1 << b))
					nm_diff->count++;
			}
		}
	}
}

/**
 * tipc_port_list_add - add a port to a port list, ensuring no duplicates
 */

void tipc_port_list_add(struct tipc_port_list *pl_ptr, u32 port)
{
	struct tipc_port_list *item = pl_ptr;
	int i;
	int item_sz = PLSIZE;
	int cnt = pl_ptr->count;

	for (; ; cnt -= item_sz, item = item->next) {
		if (cnt < PLSIZE)
			item_sz = cnt;
		for (i = 0; i < item_sz; i++)
			if (item->ports[i] == port)
				return;
		if (i < PLSIZE) {
			item->ports[i] = port;
			pl_ptr->count++;
			return;
		}
		if (!item->next) {
			item->next = kmalloc(sizeof(*item), GFP_ATOMIC);
			if (!item->next) {
				warn("Incomplete multicast delivery, no memory\n");
				return;
			}
			item->next->next = NULL;
		}
	}
}

/**
 * tipc_port_list_free - free dynamically created entries in port_list chain
 *
 */

void tipc_port_list_free(struct tipc_port_list *pl_ptr)
{
	struct tipc_port_list *item;
	struct tipc_port_list *next;

	for (item = pl_ptr->next; item; item = next) {
		next = item->next;
		kfree(item);
	}
}

=======
#include <linux/tipc_config.h>
#include "socket.h"
#include "msg.h"
#include "bcast.h"
#include "link.h"
#include "name_table.h"

#define BCLINK_WIN_DEFAULT  50	/* bcast link window size (default) */
#define BCLINK_WIN_MIN      32	/* bcast minimum link window size */

const char tipc_bclink_name[] = "broadcast-link";
unsigned long sysctl_tipc_bc_retruni __read_mostly;

/**
 * struct tipc_bc_base - base structure for keeping broadcast send state
 * @link: broadcast send link structure
 * @inputq: data input queue; will only carry SOCK_WAKEUP messages
 * @dests: array keeping number of reachable destinations per bearer
 * @primary_bearer: a bearer having links to all broadcast destinations, if any
 * @bcast_support: indicates if primary bearer, if any, supports broadcast
 * @force_bcast: forces broadcast for multicast traffic
 * @rcast_support: indicates if all peer nodes support replicast
 * @force_rcast: forces replicast for multicast traffic
 * @rc_ratio: dest count as percentage of cluster size where send method changes
 * @bc_threshold: calculated from rc_ratio; if dests > threshold use broadcast
 */
struct tipc_bc_base {
	struct tipc_link *link;
	struct sk_buff_head inputq;
	int dests[MAX_BEARERS];
	int primary_bearer;
	bool bcast_support;
	bool force_bcast;
	bool rcast_support;
	bool force_rcast;
	int rc_ratio;
	int bc_threshold;
};

static struct tipc_bc_base *tipc_bc_base(struct net *net)
{
	return tipc_net(net)->bcbase;
}

/* tipc_bcast_get_mtu(): -get the MTU currently used by broadcast link
 * Note: the MTU is decremented to give room for a tunnel header, in
 * case the message needs to be sent as replicast
 */
int tipc_bcast_get_mtu(struct net *net)
{
	return tipc_link_mss(tipc_bc_sndlink(net));
}

void tipc_bcast_toggle_rcast(struct net *net, bool supp)
{
	tipc_bc_base(net)->rcast_support = supp;
}

static void tipc_bcbase_calc_bc_threshold(struct net *net)
{
	struct tipc_bc_base *bb = tipc_bc_base(net);
	int cluster_size = tipc_link_bc_peers(tipc_bc_sndlink(net));

	bb->bc_threshold = 1 + (cluster_size * bb->rc_ratio / 100);
}

/* tipc_bcbase_select_primary(): find a bearer with links to all destinations,
 *                               if any, and make it primary bearer
 */
static void tipc_bcbase_select_primary(struct net *net)
{
	struct tipc_bc_base *bb = tipc_bc_base(net);
	int all_dests =  tipc_link_bc_peers(bb->link);
	int max_win = tipc_link_max_win(bb->link);
	int min_win = tipc_link_min_win(bb->link);
	int i, mtu, prim;

	bb->primary_bearer = INVALID_BEARER_ID;
	bb->bcast_support = true;

	if (!all_dests)
		return;

	for (i = 0; i < MAX_BEARERS; i++) {
		if (!bb->dests[i])
			continue;

		mtu = tipc_bearer_mtu(net, i);
		if (mtu < tipc_link_mtu(bb->link)) {
			tipc_link_set_mtu(bb->link, mtu);
			tipc_link_set_queue_limits(bb->link,
						   min_win,
						   max_win);
		}
		bb->bcast_support &= tipc_bearer_bcast_support(net, i);
		if (bb->dests[i] < all_dests)
			continue;

		bb->primary_bearer = i;

		/* Reduce risk that all nodes select same primary */
		if ((i ^ tipc_own_addr(net)) & 1)
			break;
	}
	prim = bb->primary_bearer;
	if (prim != INVALID_BEARER_ID)
		bb->bcast_support = tipc_bearer_bcast_support(net, prim);
}

void tipc_bcast_inc_bearer_dst_cnt(struct net *net, int bearer_id)
{
	struct tipc_bc_base *bb = tipc_bc_base(net);

	tipc_bcast_lock(net);
	bb->dests[bearer_id]++;
	tipc_bcbase_select_primary(net);
	tipc_bcast_unlock(net);
}

void tipc_bcast_dec_bearer_dst_cnt(struct net *net, int bearer_id)
{
	struct tipc_bc_base *bb = tipc_bc_base(net);

	tipc_bcast_lock(net);
	bb->dests[bearer_id]--;
	tipc_bcbase_select_primary(net);
	tipc_bcast_unlock(net);
}

/* tipc_bcbase_xmit - broadcast a packet queue across one or more bearers
 *
 * Note that number of reachable destinations, as indicated in the dests[]
 * array, may transitionally differ from the number of destinations indicated
 * in each sent buffer. We can sustain this. Excess destination nodes will
 * drop and never acknowledge the unexpected packets, and missing destinations
 * will either require retransmission (if they are just about to be added to
 * the bearer), or be removed from the buffer's 'ackers' counter (if they
 * just went down)
 */
static void tipc_bcbase_xmit(struct net *net, struct sk_buff_head *xmitq)
{
	int bearer_id;
	struct tipc_bc_base *bb = tipc_bc_base(net);
	struct sk_buff *skb, *_skb;
	struct sk_buff_head _xmitq;

	if (skb_queue_empty(xmitq))
		return;

	/* The typical case: at least one bearer has links to all nodes */
	bearer_id = bb->primary_bearer;
	if (bearer_id >= 0) {
		tipc_bearer_bc_xmit(net, bearer_id, xmitq);
		return;
	}

	/* We have to transmit across all bearers */
	__skb_queue_head_init(&_xmitq);
	for (bearer_id = 0; bearer_id < MAX_BEARERS; bearer_id++) {
		if (!bb->dests[bearer_id])
			continue;

		skb_queue_walk(xmitq, skb) {
			_skb = pskb_copy_for_clone(skb, GFP_ATOMIC);
			if (!_skb)
				break;
			__skb_queue_tail(&_xmitq, _skb);
		}
		tipc_bearer_bc_xmit(net, bearer_id, &_xmitq);
	}
	__skb_queue_purge(xmitq);
	__skb_queue_purge(&_xmitq);
}

static void tipc_bcast_select_xmit_method(struct net *net, int dests,
					  struct tipc_mc_method *method)
{
	struct tipc_bc_base *bb = tipc_bc_base(net);
	unsigned long exp = method->expires;

	/* Broadcast supported by used bearer/bearers? */
	if (!bb->bcast_support) {
		method->rcast = true;
		return;
	}
	/* Any destinations which don't support replicast ? */
	if (!bb->rcast_support) {
		method->rcast = false;
		return;
	}
	/* Can current method be changed ? */
	method->expires = jiffies + TIPC_METHOD_EXPIRE;
	if (method->mandatory)
		return;

	if (!(tipc_net(net)->capabilities & TIPC_MCAST_RBCTL) &&
	    time_before(jiffies, exp))
		return;

	/* Configuration as force 'broadcast' method */
	if (bb->force_bcast) {
		method->rcast = false;
		return;
	}
	/* Configuration as force 'replicast' method */
	if (bb->force_rcast) {
		method->rcast = true;
		return;
	}
	/* Configuration as 'autoselect' or default method */
	/* Determine method to use now */
	method->rcast = dests <= bb->bc_threshold;
}

/* tipc_bcast_xmit - broadcast the buffer chain to all external nodes
 * @net: the applicable net namespace
 * @pkts: chain of buffers containing message
 * @cong_link_cnt: set to 1 if broadcast link is congested, otherwise 0
 * Consumes the buffer chain.
 * Returns 0 if success, otherwise errno: -EHOSTUNREACH,-EMSGSIZE
 */
int tipc_bcast_xmit(struct net *net, struct sk_buff_head *pkts,
		    u16 *cong_link_cnt)
{
	struct tipc_link *l = tipc_bc_sndlink(net);
	struct sk_buff_head xmitq;
	int rc = 0;

	__skb_queue_head_init(&xmitq);
	tipc_bcast_lock(net);
	if (tipc_link_bc_peers(l))
		rc = tipc_link_xmit(l, pkts, &xmitq);
	tipc_bcast_unlock(net);
	tipc_bcbase_xmit(net, &xmitq);
	__skb_queue_purge(pkts);
	if (rc == -ELINKCONG) {
		*cong_link_cnt = 1;
		rc = 0;
	}
	return rc;
}

/* tipc_rcast_xmit - replicate and send a message to given destination nodes
 * @net: the applicable net namespace
 * @pkts: chain of buffers containing message
 * @dests: list of destination nodes
 * @cong_link_cnt: returns number of congested links
 * @cong_links: returns identities of congested links
 * Returns 0 if success, otherwise errno
 */
static int tipc_rcast_xmit(struct net *net, struct sk_buff_head *pkts,
			   struct tipc_nlist *dests, u16 *cong_link_cnt)
{
	struct tipc_dest *dst, *tmp;
	struct sk_buff_head _pkts;
	u32 dnode, selector;

	selector = msg_link_selector(buf_msg(skb_peek(pkts)));
	__skb_queue_head_init(&_pkts);

	list_for_each_entry_safe(dst, tmp, &dests->list, list) {
		dnode = dst->node;
		if (!tipc_msg_pskb_copy(dnode, pkts, &_pkts))
			return -ENOMEM;

		/* Any other return value than -ELINKCONG is ignored */
		if (tipc_node_xmit(net, &_pkts, dnode, selector) == -ELINKCONG)
			(*cong_link_cnt)++;
	}
	return 0;
}

/* tipc_mcast_send_sync - deliver a dummy message with SYN bit
 * @net: the applicable net namespace
 * @skb: socket buffer to copy
 * @method: send method to be used
 * @dests: destination nodes for message.
 * Returns 0 if success, otherwise errno
 */
static int tipc_mcast_send_sync(struct net *net, struct sk_buff *skb,
				struct tipc_mc_method *method,
				struct tipc_nlist *dests)
{
	struct tipc_msg *hdr, *_hdr;
	struct sk_buff_head tmpq;
	struct sk_buff *_skb;
	u16 cong_link_cnt;
	int rc = 0;

	/* Is a cluster supporting with new capabilities ? */
	if (!(tipc_net(net)->capabilities & TIPC_MCAST_RBCTL))
		return 0;

	hdr = buf_msg(skb);
	if (msg_user(hdr) == MSG_FRAGMENTER)
		hdr = msg_inner_hdr(hdr);
	if (msg_type(hdr) != TIPC_MCAST_MSG)
		return 0;

	/* Allocate dummy message */
	_skb = tipc_buf_acquire(MCAST_H_SIZE, GFP_KERNEL);
	if (!_skb)
		return -ENOMEM;

	/* Preparing for 'synching' header */
	msg_set_syn(hdr, 1);

	/* Copy skb's header into a dummy header */
	skb_copy_to_linear_data(_skb, hdr, MCAST_H_SIZE);
	skb_orphan(_skb);

	/* Reverse method for dummy message */
	_hdr = buf_msg(_skb);
	msg_set_size(_hdr, MCAST_H_SIZE);
	msg_set_is_rcast(_hdr, !msg_is_rcast(hdr));
	msg_set_errcode(_hdr, TIPC_ERR_NO_PORT);

	__skb_queue_head_init(&tmpq);
	__skb_queue_tail(&tmpq, _skb);
	if (method->rcast)
		rc = tipc_bcast_xmit(net, &tmpq, &cong_link_cnt);
	else
		rc = tipc_rcast_xmit(net, &tmpq, dests, &cong_link_cnt);

	/* This queue should normally be empty by now */
	__skb_queue_purge(&tmpq);

	return rc;
}

/* tipc_mcast_xmit - deliver message to indicated destination nodes
 *                   and to identified node local sockets
 * @net: the applicable net namespace
 * @pkts: chain of buffers containing message
 * @method: send method to be used
 * @dests: destination nodes for message.
 * @cong_link_cnt: returns number of encountered congested destination links
 * Consumes buffer chain.
 * Returns 0 if success, otherwise errno
 */
int tipc_mcast_xmit(struct net *net, struct sk_buff_head *pkts,
		    struct tipc_mc_method *method, struct tipc_nlist *dests,
		    u16 *cong_link_cnt)
{
	struct sk_buff_head inputq, localq;
	bool rcast = method->rcast;
	struct tipc_msg *hdr;
	struct sk_buff *skb;
	int rc = 0;

	skb_queue_head_init(&inputq);
	__skb_queue_head_init(&localq);

	/* Clone packets before they are consumed by next call */
	if (dests->local && !tipc_msg_reassemble(pkts, &localq)) {
		rc = -ENOMEM;
		goto exit;
	}
	/* Send according to determined transmit method */
	if (dests->remote) {
		tipc_bcast_select_xmit_method(net, dests->remote, method);

		skb = skb_peek(pkts);
		hdr = buf_msg(skb);
		if (msg_user(hdr) == MSG_FRAGMENTER)
			hdr = msg_inner_hdr(hdr);
		msg_set_is_rcast(hdr, method->rcast);

		/* Switch method ? */
		if (rcast != method->rcast) {
			rc = tipc_mcast_send_sync(net, skb, method, dests);
			if (unlikely(rc)) {
				pr_err("Unable to send SYN: method %d, rc %d\n",
				       rcast, rc);
				goto exit;
			}
		}

		if (method->rcast)
			rc = tipc_rcast_xmit(net, pkts, dests, cong_link_cnt);
		else
			rc = tipc_bcast_xmit(net, pkts, cong_link_cnt);
	}

	if (dests->local) {
		tipc_loopback_trace(net, &localq);
		tipc_sk_mcast_rcv(net, &localq, &inputq);
	}
exit:
	/* This queue should normally be empty by now */
	__skb_queue_purge(pkts);
	return rc;
}

/* tipc_bcast_rcv - receive a broadcast packet, and deliver to rcv link
 *
 * RCU is locked, no other locks set
 */
int tipc_bcast_rcv(struct net *net, struct tipc_link *l, struct sk_buff *skb)
{
	struct tipc_msg *hdr = buf_msg(skb);
	struct sk_buff_head *inputq = &tipc_bc_base(net)->inputq;
	struct sk_buff_head xmitq;
	int rc;

	__skb_queue_head_init(&xmitq);

	if (msg_mc_netid(hdr) != tipc_netid(net) || !tipc_link_is_up(l)) {
		kfree_skb(skb);
		return 0;
	}

	tipc_bcast_lock(net);
	if (msg_user(hdr) == BCAST_PROTOCOL)
		rc = tipc_link_bc_nack_rcv(l, skb, &xmitq);
	else
		rc = tipc_link_rcv(l, skb, NULL);
	tipc_bcast_unlock(net);

	tipc_bcbase_xmit(net, &xmitq);

	/* Any socket wakeup messages ? */
	if (!skb_queue_empty(inputq))
		tipc_sk_rcv(net, inputq);

	return rc;
}

/* tipc_bcast_ack_rcv - receive and handle a broadcast acknowledge
 *
 * RCU is locked, no other locks set
 */
void tipc_bcast_ack_rcv(struct net *net, struct tipc_link *l,
			struct tipc_msg *hdr)
{
	struct sk_buff_head *inputq = &tipc_bc_base(net)->inputq;
	u16 acked = msg_bcast_ack(hdr);
	struct sk_buff_head xmitq;

	/* Ignore bc acks sent by peer before bcast synch point was received */
	if (msg_bc_ack_invalid(hdr))
		return;

	__skb_queue_head_init(&xmitq);

	tipc_bcast_lock(net);
	tipc_link_bc_ack_rcv(l, acked, 0, NULL, &xmitq, NULL);
	tipc_bcast_unlock(net);

	tipc_bcbase_xmit(net, &xmitq);

	/* Any socket wakeup messages ? */
	if (!skb_queue_empty(inputq))
		tipc_sk_rcv(net, inputq);
}

/* tipc_bcast_synch_rcv -  check and update rcv link with peer's send state
 *
 * RCU is locked, no other locks set
 */
int tipc_bcast_sync_rcv(struct net *net, struct tipc_link *l,
			struct tipc_msg *hdr,
			struct sk_buff_head *retrq)
{
	struct sk_buff_head *inputq = &tipc_bc_base(net)->inputq;
	struct tipc_gap_ack_blks *ga;
	struct sk_buff_head xmitq;
	int rc = 0;

	__skb_queue_head_init(&xmitq);

	tipc_bcast_lock(net);
	if (msg_type(hdr) != STATE_MSG) {
		tipc_link_bc_init_rcv(l, hdr);
	} else if (!msg_bc_ack_invalid(hdr)) {
		tipc_get_gap_ack_blks(&ga, l, hdr, false);
		if (!sysctl_tipc_bc_retruni)
			retrq = &xmitq;
		rc = tipc_link_bc_ack_rcv(l, msg_bcast_ack(hdr),
					  msg_bc_gap(hdr), ga, &xmitq,
					  retrq);
		rc |= tipc_link_bc_sync_rcv(l, hdr, &xmitq);
	}
	tipc_bcast_unlock(net);

	tipc_bcbase_xmit(net, &xmitq);

	/* Any socket wakeup messages ? */
	if (!skb_queue_empty(inputq))
		tipc_sk_rcv(net, inputq);
	return rc;
}

/* tipc_bcast_add_peer - add a peer node to broadcast link and bearer
 *
 * RCU is locked, node lock is set
 */
void tipc_bcast_add_peer(struct net *net, struct tipc_link *uc_l,
			 struct sk_buff_head *xmitq)
{
	struct tipc_link *snd_l = tipc_bc_sndlink(net);

	tipc_bcast_lock(net);
	tipc_link_add_bc_peer(snd_l, uc_l, xmitq);
	tipc_bcbase_select_primary(net);
	tipc_bcbase_calc_bc_threshold(net);
	tipc_bcast_unlock(net);
}

/* tipc_bcast_remove_peer - remove a peer node from broadcast link and bearer
 *
 * RCU is locked, node lock is set
 */
void tipc_bcast_remove_peer(struct net *net, struct tipc_link *rcv_l)
{
	struct tipc_link *snd_l = tipc_bc_sndlink(net);
	struct sk_buff_head *inputq = &tipc_bc_base(net)->inputq;
	struct sk_buff_head xmitq;

	__skb_queue_head_init(&xmitq);

	tipc_bcast_lock(net);
	tipc_link_remove_bc_peer(snd_l, rcv_l, &xmitq);
	tipc_bcbase_select_primary(net);
	tipc_bcbase_calc_bc_threshold(net);
	tipc_bcast_unlock(net);

	tipc_bcbase_xmit(net, &xmitq);

	/* Any socket wakeup messages ? */
	if (!skb_queue_empty(inputq))
		tipc_sk_rcv(net, inputq);
}

int tipc_bclink_reset_stats(struct net *net, struct tipc_link *l)
{
	if (!l)
		return -ENOPROTOOPT;

	tipc_bcast_lock(net);
	tipc_link_reset_stats(l);
	tipc_bcast_unlock(net);
	return 0;
}

static int tipc_bc_link_set_queue_limits(struct net *net, u32 max_win)
{
	struct tipc_link *l = tipc_bc_sndlink(net);

	if (!l)
		return -ENOPROTOOPT;
	if (max_win < BCLINK_WIN_MIN)
		max_win = BCLINK_WIN_MIN;
	if (max_win > TIPC_MAX_LINK_WIN)
		return -EINVAL;
	tipc_bcast_lock(net);
	tipc_link_set_queue_limits(l, tipc_link_min_win(l), max_win);
	tipc_bcast_unlock(net);
	return 0;
}

static int tipc_bc_link_set_broadcast_mode(struct net *net, u32 bc_mode)
{
	struct tipc_bc_base *bb = tipc_bc_base(net);

	switch (bc_mode) {
	case BCLINK_MODE_BCAST:
		if (!bb->bcast_support)
			return -ENOPROTOOPT;

		bb->force_bcast = true;
		bb->force_rcast = false;
		break;
	case BCLINK_MODE_RCAST:
		if (!bb->rcast_support)
			return -ENOPROTOOPT;

		bb->force_bcast = false;
		bb->force_rcast = true;
		break;
	case BCLINK_MODE_SEL:
		if (!bb->bcast_support || !bb->rcast_support)
			return -ENOPROTOOPT;

		bb->force_bcast = false;
		bb->force_rcast = false;
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

static int tipc_bc_link_set_broadcast_ratio(struct net *net, u32 bc_ratio)
{
	struct tipc_bc_base *bb = tipc_bc_base(net);

	if (!bb->bcast_support || !bb->rcast_support)
		return -ENOPROTOOPT;

	if (bc_ratio > 100 || bc_ratio <= 0)
		return -EINVAL;

	bb->rc_ratio = bc_ratio;
	tipc_bcast_lock(net);
	tipc_bcbase_calc_bc_threshold(net);
	tipc_bcast_unlock(net);

	return 0;
}

int tipc_nl_bc_link_set(struct net *net, struct nlattr *attrs[])
{
	int err;
	u32 win;
	u32 bc_mode;
	u32 bc_ratio;
	struct nlattr *props[TIPC_NLA_PROP_MAX + 1];

	if (!attrs[TIPC_NLA_LINK_PROP])
		return -EINVAL;

	err = tipc_nl_parse_link_prop(attrs[TIPC_NLA_LINK_PROP], props);
	if (err)
		return err;

	if (!props[TIPC_NLA_PROP_WIN] &&
	    !props[TIPC_NLA_PROP_BROADCAST] &&
	    !props[TIPC_NLA_PROP_BROADCAST_RATIO]) {
		return -EOPNOTSUPP;
	}

	if (props[TIPC_NLA_PROP_BROADCAST]) {
		bc_mode = nla_get_u32(props[TIPC_NLA_PROP_BROADCAST]);
		err = tipc_bc_link_set_broadcast_mode(net, bc_mode);
	}

	if (!err && props[TIPC_NLA_PROP_BROADCAST_RATIO]) {
		bc_ratio = nla_get_u32(props[TIPC_NLA_PROP_BROADCAST_RATIO]);
		err = tipc_bc_link_set_broadcast_ratio(net, bc_ratio);
	}

	if (!err && props[TIPC_NLA_PROP_WIN]) {
		win = nla_get_u32(props[TIPC_NLA_PROP_WIN]);
		err = tipc_bc_link_set_queue_limits(net, win);
	}

	return err;
}

int tipc_bcast_init(struct net *net)
{
	struct tipc_net *tn = tipc_net(net);
	struct tipc_bc_base *bb = NULL;
	struct tipc_link *l = NULL;

	bb = kzalloc(sizeof(*bb), GFP_KERNEL);
	if (!bb)
		goto enomem;
	tn->bcbase = bb;
	spin_lock_init(&tipc_net(net)->bclock);

	if (!tipc_link_bc_create(net, 0, 0, NULL,
				 one_page_mtu,
				 BCLINK_WIN_DEFAULT,
				 BCLINK_WIN_DEFAULT,
				 0,
				 &bb->inputq,
				 NULL,
				 NULL,
				 &l))
		goto enomem;
	bb->link = l;
	tn->bcl = l;
	bb->rc_ratio = 10;
	bb->rcast_support = true;
	return 0;
enomem:
	kfree(bb);
	kfree(l);
	return -ENOMEM;
}

void tipc_bcast_stop(struct net *net)
{
	struct tipc_net *tn = net_generic(net, tipc_net_id);

	synchronize_net();
	kfree(tn->bcbase);
	kfree(tn->bcl);
}

void tipc_nlist_init(struct tipc_nlist *nl, u32 self)
{
	memset(nl, 0, sizeof(*nl));
	INIT_LIST_HEAD(&nl->list);
	nl->self = self;
}

void tipc_nlist_add(struct tipc_nlist *nl, u32 node)
{
	if (node == nl->self)
		nl->local = true;
	else if (tipc_dest_push(&nl->list, node, 0))
		nl->remote++;
}

void tipc_nlist_del(struct tipc_nlist *nl, u32 node)
{
	if (node == nl->self)
		nl->local = false;
	else if (tipc_dest_del(&nl->list, node, 0))
		nl->remote--;
}

void tipc_nlist_purge(struct tipc_nlist *nl)
{
	tipc_dest_list_purge(&nl->list);
	nl->remote = 0;
	nl->local = false;
}

u32 tipc_bcast_get_mode(struct net *net)
{
	struct tipc_bc_base *bb = tipc_bc_base(net);

	if (bb->force_bcast)
		return BCLINK_MODE_BCAST;

	if (bb->force_rcast)
		return BCLINK_MODE_RCAST;

	if (bb->bcast_support && bb->rcast_support)
		return BCLINK_MODE_SEL;

	return 0;
}

u32 tipc_bcast_get_broadcast_ratio(struct net *net)
{
	struct tipc_bc_base *bb = tipc_bc_base(net);

	return bb->rc_ratio;
}

void tipc_mcast_filter_msg(struct net *net, struct sk_buff_head *defq,
			   struct sk_buff_head *inputq)
{
	struct sk_buff *skb, *_skb, *tmp;
	struct tipc_msg *hdr, *_hdr;
	bool match = false;
	u32 node, port;

	skb = skb_peek(inputq);
	if (!skb)
		return;

	hdr = buf_msg(skb);

	if (likely(!msg_is_syn(hdr) && skb_queue_empty(defq)))
		return;

	node = msg_orignode(hdr);
	if (node == tipc_own_addr(net))
		return;

	port = msg_origport(hdr);

	/* Has the twin SYN message already arrived ? */
	skb_queue_walk(defq, _skb) {
		_hdr = buf_msg(_skb);
		if (msg_orignode(_hdr) != node)
			continue;
		if (msg_origport(_hdr) != port)
			continue;
		match = true;
		break;
	}

	if (!match) {
		if (!msg_is_syn(hdr))
			return;
		__skb_dequeue(inputq);
		__skb_queue_tail(defq, skb);
		return;
	}

	/* Deliver non-SYN message from other link, otherwise queue it */
	if (!msg_is_syn(hdr)) {
		if (msg_is_rcast(hdr) != msg_is_rcast(_hdr))
			return;
		__skb_dequeue(inputq);
		__skb_queue_tail(defq, skb);
		return;
	}

	/* Queue non-SYN/SYN message from same link */
	if (msg_is_rcast(hdr) == msg_is_rcast(_hdr)) {
		__skb_dequeue(inputq);
		__skb_queue_tail(defq, skb);
		return;
	}

	/* Matching SYN messages => return the one with data, if any */
	__skb_unlink(_skb, defq);
	if (msg_data_sz(hdr)) {
		kfree_skb(_skb);
	} else {
		__skb_dequeue(inputq);
		kfree_skb(skb);
		__skb_queue_tail(inputq, _skb);
	}

	/* Deliver subsequent non-SYN messages from same peer */
	skb_queue_walk_safe(defq, _skb, tmp) {
		_hdr = buf_msg(_skb);
		if (msg_orignode(_hdr) != node)
			continue;
		if (msg_origport(_hdr) != port)
			continue;
		if (msg_is_syn(_hdr))
			break;
		__skb_unlink(_skb, defq);
		__skb_queue_tail(inputq, _skb);
	}
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
