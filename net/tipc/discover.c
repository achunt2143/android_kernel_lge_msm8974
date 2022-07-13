/*
 * net/tipc/discover.c
 *
<<<<<<< HEAD
 * Copyright (c) 2003-2006, Ericsson AB
=======
 * Copyright (c) 2003-2006, 2014-2018, Ericsson AB
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Copyright (c) 2005-2006, 2010-2011, Wind River Systems
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
#include "discover.h"

#define TIPC_LINK_REQ_INIT	125	/* min delay during bearer start up */
#define TIPC_LINK_REQ_FAST	1000	/* max delay if bearer has no links */
#define TIPC_LINK_REQ_SLOW	60000	/* max delay if bearer has links */
#define TIPC_LINK_REQ_INACTIVE	0xffffffff /* indicates no timer in use */


/**
 * struct tipc_link_req - information about an ongoing link setup request
 * @bearer: bearer issuing requests
 * @dest: destination address for request messages
 * @domain: network domain to which links can be established
 * @num_nodes: number of nodes currently discovered (i.e. with an active link)
 * @buf: request message to be (repeatedly) sent
 * @timer: timer governing period between requests
 * @timer_intv: current interval between requests (in ms)
 */
struct tipc_link_req {
	struct tipc_bearer *bearer;
	struct tipc_media_addr dest;
	u32 domain;
	int num_nodes;
	struct sk_buff *buf;
	struct timer_list timer;
	unsigned int timer_intv;
=======
#include "node.h"
#include "discover.h"

/* min delay during bearer start up */
#define TIPC_DISC_INIT	msecs_to_jiffies(125)
/* max delay if bearer has no links */
#define TIPC_DISC_FAST	msecs_to_jiffies(1000)
/* max delay if bearer has links */
#define TIPC_DISC_SLOW	msecs_to_jiffies(60000)
/* indicates no timer in use */
#define TIPC_DISC_INACTIVE	0xffffffff

/**
 * struct tipc_discoverer - information about an ongoing link setup request
 * @bearer_id: identity of bearer issuing requests
 * @net: network namespace instance
 * @dest: destination address for request messages
 * @domain: network domain to which links can be established
 * @num_nodes: number of nodes currently discovered (i.e. with an active link)
 * @lock: spinlock for controlling access to requests
 * @skb: request message to be (repeatedly) sent
 * @timer: timer governing period between requests
 * @timer_intv: current interval between requests (in ms)
 */
struct tipc_discoverer {
	u32 bearer_id;
	struct tipc_media_addr dest;
	struct net *net;
	u32 domain;
	int num_nodes;
	spinlock_t lock;
	struct sk_buff *skb;
	struct timer_list timer;
	unsigned long timer_intv;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/**
 * tipc_disc_init_msg - initialize a link setup message
<<<<<<< HEAD
 * @type: message type (request or response)
 * @dest_domain: network domain of node(s) which should respond to message
 * @b_ptr: ptr to bearer issuing message
 */

static struct sk_buff *tipc_disc_init_msg(u32 type,
					  u32 dest_domain,
					  struct tipc_bearer *b_ptr)
{
	struct sk_buff *buf = tipc_buf_acquire(INT_H_SIZE);
	struct tipc_msg *msg;

	if (buf) {
		msg = buf_msg(buf);
		tipc_msg_init(msg, LINK_CONFIG, type, INT_H_SIZE, dest_domain);
		msg_set_non_seq(msg, 1);
		msg_set_node_sig(msg, tipc_random);
		msg_set_dest_domain(msg, dest_domain);
		msg_set_bc_netid(msg, tipc_net_id);
		b_ptr->media->addr2msg(&b_ptr->addr, msg_media_addr(msg));
	}
	return buf;
=======
 * @net: the applicable net namespace
 * @skb: buffer containing message
 * @mtyp: message type (request or response)
 * @b: ptr to bearer issuing message
 */
static void tipc_disc_init_msg(struct net *net, struct sk_buff *skb,
			       u32 mtyp,  struct tipc_bearer *b)
{
	struct tipc_net *tn = tipc_net(net);
	u32 dest_domain = b->domain;
	struct tipc_msg *hdr;

	hdr = buf_msg(skb);
	tipc_msg_init(tn->trial_addr, hdr, LINK_CONFIG, mtyp,
		      MAX_H_SIZE, dest_domain);
	msg_set_size(hdr, MAX_H_SIZE + NODE_ID_LEN);
	msg_set_non_seq(hdr, 1);
	msg_set_node_sig(hdr, tn->random);
	msg_set_node_capabilities(hdr, TIPC_NODE_CAPABILITIES);
	msg_set_dest_domain(hdr, dest_domain);
	msg_set_bc_netid(hdr, tn->net_id);
	b->media->addr2msg(msg_media_addr(hdr), &b->addr);
	msg_set_peer_net_hash(hdr, tipc_net_hash_mixes(net, tn->random));
	msg_set_node_id(hdr, tipc_own_id(net));
}

static void tipc_disc_msg_xmit(struct net *net, u32 mtyp, u32 dst,
			       u32 src, u32 sugg_addr,
			       struct tipc_media_addr *maddr,
			       struct tipc_bearer *b)
{
	struct tipc_msg *hdr;
	struct sk_buff *skb;

	skb = tipc_buf_acquire(MAX_H_SIZE + NODE_ID_LEN, GFP_ATOMIC);
	if (!skb)
		return;
	hdr = buf_msg(skb);
	tipc_disc_init_msg(net, skb, mtyp, b);
	msg_set_sugg_node_addr(hdr, sugg_addr);
	msg_set_dest_domain(hdr, dst);
	tipc_bearer_xmit_skb(net, b->identity, skb, maddr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * disc_dupl_alert - issue node address duplication alert
<<<<<<< HEAD
 * @b_ptr: pointer to bearer detecting duplication
 * @node_addr: duplicated node address
 * @media_addr: media address advertised by duplicated node
 */

static void disc_dupl_alert(struct tipc_bearer *b_ptr, u32 node_addr,
			    struct tipc_media_addr *media_addr)
{
	char node_addr_str[16];
	char media_addr_str[64];
	struct print_buf pb;

	tipc_addr_string_fill(node_addr_str, node_addr);
	tipc_printbuf_init(&pb, media_addr_str, sizeof(media_addr_str));
	tipc_media_addr_printf(&pb, media_addr);
	tipc_printbuf_validate(&pb);
	warn("Duplicate %s using %s seen on <%s>\n",
	     node_addr_str, media_addr_str, b_ptr->name);
}

/**
 * tipc_disc_recv_msg - handle incoming link setup message (request or response)
 * @buf: buffer containing message
 * @b_ptr: bearer that message arrived on
 */

void tipc_disc_recv_msg(struct sk_buff *buf, struct tipc_bearer *b_ptr)
{
	struct tipc_node *n_ptr;
	struct tipc_link *link;
	struct tipc_media_addr media_addr;
	struct sk_buff *rbuf;
	struct tipc_msg *msg = buf_msg(buf);
	u32 dest = msg_dest_domain(msg);
	u32 orig = msg_prevnode(msg);
	u32 net_id = msg_bc_netid(msg);
	u32 type = msg_type(msg);
	u32 signature = msg_node_sig(msg);
	int addr_mismatch;
	int link_fully_up;

	media_addr.broadcast = 1;
	b_ptr->media->msg2addr(&media_addr, msg_media_addr(msg));
	kfree_skb(buf);

	/* Ensure message from node is valid and communication is permitted */
	if (net_id != tipc_net_id)
		return;
	if (media_addr.broadcast)
		return;
	if (!tipc_addr_domain_valid(dest))
		return;
	if (!tipc_addr_node_valid(orig))
		return;
	if (orig == tipc_own_addr) {
		if (memcmp(&media_addr, &b_ptr->addr, sizeof(media_addr)))
			disc_dupl_alert(b_ptr, tipc_own_addr, &media_addr);
		return;
	}
	if (!tipc_in_scope(dest, tipc_own_addr))
		return;
	if (!tipc_in_scope(b_ptr->link_req->domain, orig))
		return;

	/* Locate structure corresponding to requesting node */
	n_ptr = tipc_node_find(orig);
	if (!n_ptr) {
		n_ptr = tipc_node_create(orig);
		if (!n_ptr)
			return;
	}
	tipc_node_lock(n_ptr);

	/* Prepare to validate requesting node's signature and media address */
	link = n_ptr->links[b_ptr->identity];
	addr_mismatch = (link != NULL) &&
		memcmp(&link->media_addr, &media_addr, sizeof(media_addr));

	/*
	 * Ensure discovery message's signature is correct
	 *
	 * If signature is incorrect and there is no working link to the node,
	 * accept the new signature but invalidate all existing links to the
	 * node so they won't re-activate without a new discovery message.
	 *
	 * If signature is incorrect and the requested link to the node is
	 * working, accept the new signature. (This is an instance of delayed
	 * rediscovery, where a link endpoint was able to re-establish contact
	 * with its peer endpoint on a node that rebooted before receiving a
	 * discovery message from that node.)
	 *
	 * If signature is incorrect and there is a working link to the node
	 * that is not the requested link, reject the request (must be from
	 * a duplicate node).
	 */
	if (signature != n_ptr->signature) {
		if (n_ptr->working_links == 0) {
			struct tipc_link *curr_link;
			int i;

			for (i = 0; i < MAX_BEARERS; i++) {
				curr_link = n_ptr->links[i];
				if (curr_link) {
					memset(&curr_link->media_addr, 0,
					       sizeof(media_addr));
					tipc_link_reset(curr_link);
				}
			}
			addr_mismatch = (link != NULL);
		} else if (tipc_link_is_up(link) && !addr_mismatch) {
			/* delayed rediscovery */
		} else {
			disc_dupl_alert(b_ptr, orig, &media_addr);
			tipc_node_unlock(n_ptr);
			return;
		}
		n_ptr->signature = signature;
	}

	/*
	 * Ensure requesting node's media address is correct
	 *
	 * If media address doesn't match and the link is working, reject the
	 * request (must be from a duplicate node).
	 *
	 * If media address doesn't match and the link is not working, accept
	 * the new media address and reset the link to ensure it starts up
	 * cleanly.
	 */

	if (addr_mismatch) {
		if (tipc_link_is_up(link)) {
			disc_dupl_alert(b_ptr, orig, &media_addr);
			tipc_node_unlock(n_ptr);
			return;
		} else {
			memcpy(&link->media_addr, &media_addr,
			       sizeof(media_addr));
			tipc_link_reset(link);
		}
	}

	/* Create a link endpoint for this bearer, if necessary */
	if (!link) {
		link = tipc_link_create(n_ptr, b_ptr, &media_addr);
		if (!link) {
			tipc_node_unlock(n_ptr);
			return;
		}
	}

	/* Accept discovery message & send response, if necessary */
	link_fully_up = link_working_working(link);

	if ((type == DSC_REQ_MSG) && !link_fully_up && !b_ptr->blocked) {
		rbuf = tipc_disc_init_msg(DSC_RESP_MSG, orig, b_ptr);
		if (rbuf) {
			b_ptr->media->send_msg(rbuf, b_ptr, &media_addr);
			kfree_skb(rbuf);
		}
	}

	tipc_node_unlock(n_ptr);
}

/**
 * disc_update - update frequency of periodic link setup requests
 * @req: ptr to link request structure
 *
 * Reinitiates discovery process if discovery object has no associated nodes
 * and is either not currently searching or is searching at a slow rate
 */

static void disc_update(struct tipc_link_req *req)
{
	if (!req->num_nodes) {
		if ((req->timer_intv == TIPC_LINK_REQ_INACTIVE) ||
		    (req->timer_intv > TIPC_LINK_REQ_FAST)) {
			req->timer_intv = TIPC_LINK_REQ_INIT;
			k_start_timer(&req->timer, req->timer_intv);
		}
	}
}

/**
 * tipc_disc_add_dest - increment set of discovered nodes
 * @req: ptr to link request structure
 */

void tipc_disc_add_dest(struct tipc_link_req *req)
{
	req->num_nodes++;
}

/**
 * tipc_disc_remove_dest - decrement set of discovered nodes
 * @req: ptr to link request structure
 */

void tipc_disc_remove_dest(struct tipc_link_req *req)
{
	req->num_nodes--;
	disc_update(req);
}

/**
 * disc_send_msg - send link setup request message
 * @req: ptr to link request structure
 */

static void disc_send_msg(struct tipc_link_req *req)
{
	if (!req->bearer->blocked)
		tipc_bearer_send(req->bearer, req->buf, &req->dest);
}

/**
 * disc_timeout - send a periodic link setup request
 * @req: ptr to link request structure
 *
 * Called whenever a link setup request timer associated with a bearer expires.
 */

static void disc_timeout(struct tipc_link_req *req)
{
	int max_delay;

	spin_lock_bh(&req->bearer->lock);

	/* Stop searching if only desired node has been found */

	if (tipc_node(req->domain) && req->num_nodes) {
		req->timer_intv = TIPC_LINK_REQ_INACTIVE;
		goto exit;
	}

	/*
	 * Send discovery message, then update discovery timer
	 *
	 * Keep doubling time between requests until limit is reached;
	 * hold at fast polling rate if don't have any associated nodes,
	 * otherwise hold at slow polling rate
	 */

	disc_send_msg(req);

	req->timer_intv *= 2;
	if (req->num_nodes)
		max_delay = TIPC_LINK_REQ_SLOW;
	else
		max_delay = TIPC_LINK_REQ_FAST;
	if (req->timer_intv > max_delay)
		req->timer_intv = max_delay;

	k_start_timer(&req->timer, req->timer_intv);
exit:
	spin_unlock_bh(&req->bearer->lock);
=======
 * @b: pointer to bearer detecting duplication
 * @node_addr: duplicated node address
 * @media_addr: media address advertised by duplicated node
 */
static void disc_dupl_alert(struct tipc_bearer *b, u32 node_addr,
			    struct tipc_media_addr *media_addr)
{
	char media_addr_str[64];

	tipc_media_addr_printf(media_addr_str, sizeof(media_addr_str),
			       media_addr);
	pr_warn("Duplicate %x using %s seen on <%s>\n", node_addr,
		media_addr_str, b->name);
}

/* tipc_disc_addr_trial(): - handle an address uniqueness trial from peer
 * Returns true if message should be dropped by caller, i.e., if it is a
 * trial message or we are inside trial period. Otherwise false.
 */
static bool tipc_disc_addr_trial_msg(struct tipc_discoverer *d,
				     struct tipc_media_addr *maddr,
				     struct tipc_bearer *b,
				     u32 dst, u32 src,
				     u32 sugg_addr,
				     u8 *peer_id,
				     int mtyp)
{
	struct net *net = d->net;
	struct tipc_net *tn = tipc_net(net);
	u32 self = tipc_own_addr(net);
	bool trial = time_before(jiffies, tn->addr_trial_end) && !self;

	if (mtyp == DSC_TRIAL_FAIL_MSG) {
		if (!trial)
			return true;

		/* Ignore if somebody else already gave new suggestion */
		if (dst != tn->trial_addr)
			return true;

		/* Otherwise update trial address and restart trial period */
		tn->trial_addr = sugg_addr;
		msg_set_prevnode(buf_msg(d->skb), sugg_addr);
		tn->addr_trial_end = jiffies + msecs_to_jiffies(1000);
		return true;
	}

	/* Apply trial address if we just left trial period */
	if (!trial && !self) {
		schedule_work(&tn->work);
		msg_set_prevnode(buf_msg(d->skb), tn->trial_addr);
		msg_set_type(buf_msg(d->skb), DSC_REQ_MSG);
	}

	/* Accept regular link requests/responses only after trial period */
	if (mtyp != DSC_TRIAL_MSG)
		return trial;

	sugg_addr = tipc_node_try_addr(net, peer_id, src);
	if (sugg_addr)
		tipc_disc_msg_xmit(net, DSC_TRIAL_FAIL_MSG, src,
				   self, sugg_addr, maddr, b);
	return true;
}

/**
 * tipc_disc_rcv - handle incoming discovery message (request or response)
 * @net: applicable net namespace
 * @skb: buffer containing message
 * @b: bearer that message arrived on
 */
void tipc_disc_rcv(struct net *net, struct sk_buff *skb,
		   struct tipc_bearer *b)
{
	struct tipc_net *tn = tipc_net(net);
	struct tipc_msg *hdr = buf_msg(skb);
	u32 pnet_hash = msg_peer_net_hash(hdr);
	u16 caps = msg_node_capabilities(hdr);
	bool legacy = tn->legacy_addr_format;
	u32 sugg = msg_sugg_node_addr(hdr);
	u32 signature = msg_node_sig(hdr);
	u8 peer_id[NODE_ID_LEN] = {0,};
	u32 dst = msg_dest_domain(hdr);
	u32 net_id = msg_bc_netid(hdr);
	struct tipc_media_addr maddr;
	u32 src = msg_prevnode(hdr);
	u32 mtyp = msg_type(hdr);
	bool dupl_addr = false;
	bool respond = false;
	u32 self;
	int err;

	if (skb_linearize(skb)) {
		kfree_skb(skb);
		return;
	}
	hdr = buf_msg(skb);

	if (caps & TIPC_NODE_ID128)
		memcpy(peer_id, msg_node_id(hdr), NODE_ID_LEN);
	else
		sprintf(peer_id, "%x", src);

	err = b->media->msg2addr(b, &maddr, msg_media_addr(hdr));
	kfree_skb(skb);
	if (err || maddr.broadcast) {
		pr_warn_ratelimited("Rcv corrupt discovery message\n");
		return;
	}
	/* Ignore discovery messages from own node */
	if (!memcmp(&maddr, &b->addr, sizeof(maddr)))
		return;
	if (net_id != tn->net_id)
		return;
	if (tipc_disc_addr_trial_msg(b->disc, &maddr, b, dst,
				     src, sugg, peer_id, mtyp))
		return;
	self = tipc_own_addr(net);

	/* Message from somebody using this node's address */
	if (in_own_node(net, src)) {
		disc_dupl_alert(b, self, &maddr);
		return;
	}
	if (!tipc_in_scope(legacy, dst, self))
		return;
	if (!tipc_in_scope(legacy, b->domain, src))
		return;
	tipc_node_check_dest(net, src, peer_id, b, caps, signature, pnet_hash,
			     &maddr, &respond, &dupl_addr);
	if (dupl_addr)
		disc_dupl_alert(b, src, &maddr);
	if (!respond)
		return;
	if (mtyp != DSC_REQ_MSG)
		return;
	tipc_disc_msg_xmit(net, DSC_RESP_MSG, src, self, 0, &maddr, b);
}

/* tipc_disc_add_dest - increment set of discovered nodes
 */
void tipc_disc_add_dest(struct tipc_discoverer *d)
{
	spin_lock_bh(&d->lock);
	d->num_nodes++;
	spin_unlock_bh(&d->lock);
}

/* tipc_disc_remove_dest - decrement set of discovered nodes
 */
void tipc_disc_remove_dest(struct tipc_discoverer *d)
{
	int intv, num;

	spin_lock_bh(&d->lock);
	d->num_nodes--;
	num = d->num_nodes;
	intv = d->timer_intv;
	if (!num && (intv == TIPC_DISC_INACTIVE || intv > TIPC_DISC_FAST))  {
		d->timer_intv = TIPC_DISC_INIT;
		mod_timer(&d->timer, jiffies + d->timer_intv);
	}
	spin_unlock_bh(&d->lock);
}

/* tipc_disc_timeout - send a periodic link setup request
 * Called whenever a link setup request timer associated with a bearer expires.
 * - Keep doubling time between sent request until limit is reached;
 * - Hold at fast polling rate if we don't have any associated nodes
 * - Otherwise hold at slow polling rate
 */
static void tipc_disc_timeout(struct timer_list *t)
{
	struct tipc_discoverer *d = from_timer(d, t, timer);
	struct tipc_net *tn = tipc_net(d->net);
	struct tipc_media_addr maddr;
	struct sk_buff *skb = NULL;
	struct net *net = d->net;
	u32 bearer_id;

	spin_lock_bh(&d->lock);

	/* Stop searching if only desired node has been found */
	if (tipc_node(d->domain) && d->num_nodes) {
		d->timer_intv = TIPC_DISC_INACTIVE;
		goto exit;
	}

	/* Did we just leave trial period ? */
	if (!time_before(jiffies, tn->addr_trial_end) && !tipc_own_addr(net)) {
		mod_timer(&d->timer, jiffies + TIPC_DISC_INIT);
		spin_unlock_bh(&d->lock);
		schedule_work(&tn->work);
		return;
	}

	/* Adjust timeout interval according to discovery phase */
	if (time_before(jiffies, tn->addr_trial_end)) {
		d->timer_intv = TIPC_DISC_INIT;
	} else {
		d->timer_intv *= 2;
		if (d->num_nodes && d->timer_intv > TIPC_DISC_SLOW)
			d->timer_intv = TIPC_DISC_SLOW;
		else if (!d->num_nodes && d->timer_intv > TIPC_DISC_FAST)
			d->timer_intv = TIPC_DISC_FAST;
		msg_set_type(buf_msg(d->skb), DSC_REQ_MSG);
		msg_set_prevnode(buf_msg(d->skb), tn->trial_addr);
	}

	mod_timer(&d->timer, jiffies + d->timer_intv);
	memcpy(&maddr, &d->dest, sizeof(maddr));
	skb = skb_clone(d->skb, GFP_ATOMIC);
	bearer_id = d->bearer_id;
exit:
	spin_unlock_bh(&d->lock);
	if (skb)
		tipc_bearer_xmit_skb(net, bearer_id, skb, &maddr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * tipc_disc_create - create object to send periodic link setup requests
<<<<<<< HEAD
 * @b_ptr: ptr to bearer issuing requests
 * @dest: destination address for request messages
 * @dest_domain: network domain to which links can be established
 *
 * Returns 0 if successful, otherwise -errno.
 */

int tipc_disc_create(struct tipc_bearer *b_ptr,
		     struct tipc_media_addr *dest, u32 dest_domain)
{
	struct tipc_link_req *req;

	req = kmalloc(sizeof(*req), GFP_ATOMIC);
	if (!req)
		return -ENOMEM;

	req->buf = tipc_disc_init_msg(DSC_REQ_MSG, dest_domain, b_ptr);
	if (!req->buf) {
		kfree(req);
		return -ENOMSG;
	}

	memcpy(&req->dest, dest, sizeof(*dest));
	req->bearer = b_ptr;
	req->domain = dest_domain;
	req->num_nodes = 0;
	req->timer_intv = TIPC_LINK_REQ_INIT;
	k_init_timer(&req->timer, (Handler)disc_timeout, (unsigned long)req);
	k_start_timer(&req->timer, req->timer_intv);
	b_ptr->link_req = req;
	disc_send_msg(req);
=======
 * @net: the applicable net namespace
 * @b: ptr to bearer issuing requests
 * @dest: destination address for request messages
 * @skb: pointer to created frame
 *
 * Return: 0 if successful, otherwise -errno.
 */
int tipc_disc_create(struct net *net, struct tipc_bearer *b,
		     struct tipc_media_addr *dest, struct sk_buff **skb)
{
	struct tipc_net *tn = tipc_net(net);
	struct tipc_discoverer *d;

	d = kmalloc(sizeof(*d), GFP_ATOMIC);
	if (!d)
		return -ENOMEM;
	d->skb = tipc_buf_acquire(MAX_H_SIZE + NODE_ID_LEN, GFP_ATOMIC);
	if (!d->skb) {
		kfree(d);
		return -ENOMEM;
	}
	tipc_disc_init_msg(net, d->skb, DSC_REQ_MSG, b);

	/* Do we need an address trial period first ? */
	if (!tipc_own_addr(net)) {
		tn->addr_trial_end = jiffies + msecs_to_jiffies(1000);
		msg_set_type(buf_msg(d->skb), DSC_TRIAL_MSG);
	}
	memcpy(&d->dest, dest, sizeof(*dest));
	d->net = net;
	d->bearer_id = b->identity;
	d->domain = b->domain;
	d->num_nodes = 0;
	d->timer_intv = TIPC_DISC_INIT;
	spin_lock_init(&d->lock);
	timer_setup(&d->timer, tipc_disc_timeout, 0);
	mod_timer(&d->timer, jiffies + d->timer_intv);
	b->disc = d;
	*skb = skb_clone(d->skb, GFP_ATOMIC);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

/**
 * tipc_disc_delete - destroy object sending periodic link setup requests
<<<<<<< HEAD
 * @req: ptr to link request structure
 */

void tipc_disc_delete(struct tipc_link_req *req)
{
	k_cancel_timer(&req->timer);
	k_term_timer(&req->timer);
	kfree_skb(req->buf);
	kfree(req);
}

=======
 * @d: ptr to link dest structure
 */
void tipc_disc_delete(struct tipc_discoverer *d)
{
	timer_shutdown_sync(&d->timer);
	kfree_skb(d->skb);
	kfree(d);
}

/**
 * tipc_disc_reset - reset object to send periodic link setup requests
 * @net: the applicable net namespace
 * @b: ptr to bearer issuing requests
 */
void tipc_disc_reset(struct net *net, struct tipc_bearer *b)
{
	struct tipc_discoverer *d = b->disc;
	struct tipc_media_addr maddr;
	struct sk_buff *skb;

	spin_lock_bh(&d->lock);
	tipc_disc_init_msg(net, d->skb, DSC_REQ_MSG, b);
	d->net = net;
	d->bearer_id = b->identity;
	d->domain = b->domain;
	d->num_nodes = 0;
	d->timer_intv = TIPC_DISC_INIT;
	memcpy(&maddr, &d->dest, sizeof(maddr));
	mod_timer(&d->timer, jiffies + d->timer_intv);
	skb = skb_clone(d->skb, GFP_ATOMIC);
	spin_unlock_bh(&d->lock);
	if (skb)
		tipc_bearer_xmit_skb(net, b->identity, skb, &maddr);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
