/*
   BlueZ - Bluetooth protocol stack for Linux
<<<<<<< HEAD
   Copyright (c) 2000-2001, 2010-2013 The Linux Foundation. All rights reserved.
   Copyright (C) 2009-2010 Gustavo F. Padovan <gustavo@padovan.org>
   Copyright (C) 2010 Google Inc.
=======
   Copyright (C) 2000-2001 Qualcomm Incorporated
   Copyright (C) 2009-2010 Gustavo F. Padovan <gustavo@padovan.org>
   Copyright (C) 2010 Google Inc.
   Copyright (C) 2011 ProFUSION Embedded Systems
   Copyright (c) 2012 Code Aurora Forum.  All rights reserved.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

   Written 2000,2001 by Maxim Krasnyansky <maxk@qualcomm.com>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License version 2 as
   published by the Free Software Foundation;

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF THIRD PARTY RIGHTS.
   IN NO EVENT SHALL THE COPYRIGHT HOLDER(S) AND AUTHOR(S) BE LIABLE FOR ANY
   CLAIM, OR ANY SPECIAL INDIRECT OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES
   WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
   ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
   OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

   ALL LIABILITY, INCLUDING LIABILITY FOR INFRINGEMENT OF ANY PATENTS,
   COPYRIGHTS, TRADEMARKS OR OTHER RIGHTS, RELATING TO USE OF THIS
   SOFTWARE IS DISCLAIMED.
*/

/* Bluetooth L2CAP core. */

#include <linux/module.h>

<<<<<<< HEAD
#include <linux/types.h>
#include <linux/capability.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/poll.h>
#include <linux/fcntl.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/socket.h>
#include <linux/skbuff.h>
#include <linux/list.h>
#include <linux/device.h>
#include <linux/debugfs.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>
#include <linux/crc16.h>
#include <linux/math64.h>
#include <net/sock.h>

#include <asm/system.h>
#include <asm/unaligned.h>
=======
#include <linux/debugfs.h>
#include <linux/crc16.h>
#include <linux/filter.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <net/bluetooth/bluetooth.h>
#include <net/bluetooth/hci_core.h>
#include <net/bluetooth/l2cap.h>
<<<<<<< HEAD
#include <net/bluetooth/smp.h>
#include <net/bluetooth/amp.h>

bool disable_ertm;
bool enable_hs;
bool enable_reconfig;

static u32 l2cap_feat_mask = L2CAP_FEAT_FIXED_CHAN;
static u8 l2cap_fc_mask = L2CAP_FC_L2CAP;

struct workqueue_struct *_l2cap_wq;

struct bt_sock_list l2cap_sk_list = {
	.lock = __RW_LOCK_UNLOCKED(l2cap_sk_list.lock)
};

static void l2cap_send_move_chan_req(struct l2cap_conn *conn,
			struct l2cap_pinfo *pi, u16 icid, u8 dest_amp_id);
static void l2cap_send_move_chan_cfm(struct l2cap_conn *conn,
			struct l2cap_pinfo *pi, u16 icid, u16 result);
static void l2cap_send_move_chan_rsp(struct l2cap_conn *conn, u8 ident,
			u16 icid, u16 result);

static void l2cap_amp_move_setup(struct sock *sk);
static void l2cap_amp_move_success(struct sock *sk);
static void l2cap_amp_move_revert(struct sock *sk);

static int l2cap_ertm_rx_queued_iframes(struct sock *sk);

static struct sk_buff *l2cap_build_cmd(struct l2cap_conn *conn,
				u8 code, u8 ident, u16 dlen, void *data);
static int l2cap_answer_move_poll(struct sock *sk);
static int l2cap_create_cfm(struct hci_chan *chan, u8 status);
static int l2cap_deaggregate(struct hci_chan *chan, struct l2cap_pinfo *pi);
static void l2cap_chan_ready(struct sock *sk);
static void l2cap_conn_del(struct hci_conn *hcon, int err, u8 is_process);
static u16 l2cap_get_smallest_flushto(struct l2cap_chan_list *l);
static void l2cap_set_acl_flushto(struct hci_conn *hcon, u16 flush_to);
static void l2cap_queue_acl_data(struct work_struct *worker);
static struct att_channel_parameters{
	struct sk_buff *skb;
	struct l2cap_conn *conn;
	__le16 cid;
	int dir;
} att_chn_params;

/* ---- L2CAP channels ---- */
static struct sock *__l2cap_get_chan_by_dcid(struct l2cap_chan_list *l, u16 cid)
{
	struct sock *s;
	for (s = l->head; s; s = l2cap_pi(s)->next_c) {
		if (l2cap_pi(s)->dcid == cid)
			break;
	}
	return s;
}

/* Find channel with given DCID.
 * Returns locked socket */
static inline struct sock *l2cap_get_chan_by_dcid(struct l2cap_chan_list *l,
						u16 cid)
{
	struct sock *s;
	read_lock(&l->lock);
	s = __l2cap_get_chan_by_dcid(l, cid);
	if (s)
		bh_lock_sock(s);
	read_unlock(&l->lock);
	return s;
}

static struct sock *__l2cap_get_chan_by_scid(struct l2cap_chan_list *l, u16 cid)
{
	struct sock *s;
	for (s = l->head; s; s = l2cap_pi(s)->next_c) {
		if (l2cap_pi(s)->scid == cid)
			break;
	}
	return s;
}

/* Find channel with given SCID.
 * Returns locked socket */
static inline struct sock *l2cap_get_chan_by_scid(struct l2cap_chan_list *l, u16 cid)
{
	struct sock *s;
	read_lock(&l->lock);
	s = __l2cap_get_chan_by_scid(l, cid);
	if (s)
		bh_lock_sock(s);
	read_unlock(&l->lock);
	return s;
}

static struct sock *__l2cap_get_chan_by_ident(struct l2cap_chan_list *l, u8 ident)
{
	struct sock *s;
	for (s = l->head; s; s = l2cap_pi(s)->next_c) {
		if (l2cap_pi(s)->ident == ident)
			break;
	}
	return s;
}

static inline struct sock *l2cap_get_chan_by_ident(struct l2cap_chan_list *l, u8 ident)
{
	struct sock *s;
	read_lock(&l->lock);
	s = __l2cap_get_chan_by_ident(l, ident);
	if (s)
		bh_lock_sock(s);
	read_unlock(&l->lock);
	return s;
}

static inline struct sk_buff *l2cap_ertm_seq_in_queue(struct sk_buff_head *head,
						u16 seq)
=======

#include "smp.h"

#define LE_FLOWCTL_MAX_CREDITS 65535

bool disable_ertm;
bool enable_ecred = IS_ENABLED(CONFIG_BT_LE_L2CAP_ECRED);

static u32 l2cap_feat_mask = L2CAP_FEAT_FIXED_CHAN | L2CAP_FEAT_UCD;

static LIST_HEAD(chan_list);
static DEFINE_RWLOCK(chan_list_lock);

static struct sk_buff *l2cap_build_cmd(struct l2cap_conn *conn,
				       u8 code, u8 ident, u16 dlen, void *data);
static void l2cap_send_cmd(struct l2cap_conn *conn, u8 ident, u8 code, u16 len,
			   void *data);
static int l2cap_build_conf_req(struct l2cap_chan *chan, void *data, size_t data_size);
static void l2cap_send_disconn_req(struct l2cap_chan *chan, int err);

static void l2cap_tx(struct l2cap_chan *chan, struct l2cap_ctrl *control,
		     struct sk_buff_head *skbs, u8 event);
static void l2cap_retrans_timeout(struct work_struct *work);
static void l2cap_monitor_timeout(struct work_struct *work);
static void l2cap_ack_timeout(struct work_struct *work);

static inline u8 bdaddr_type(u8 link_type, u8 bdaddr_type)
{
	if (link_type == LE_LINK) {
		if (bdaddr_type == ADDR_LE_DEV_PUBLIC)
			return BDADDR_LE_PUBLIC;
		else
			return BDADDR_LE_RANDOM;
	}

	return BDADDR_BREDR;
}

static inline u8 bdaddr_src_type(struct hci_conn *hcon)
{
	return bdaddr_type(hcon->type, hcon->src_type);
}

static inline u8 bdaddr_dst_type(struct hci_conn *hcon)
{
	return bdaddr_type(hcon->type, hcon->dst_type);
}

/* ---- L2CAP channels ---- */

static struct l2cap_chan *__l2cap_get_chan_by_dcid(struct l2cap_conn *conn,
						   u16 cid)
{
	struct l2cap_chan *c;

	list_for_each_entry(c, &conn->chan_l, list) {
		if (c->dcid == cid)
			return c;
	}
	return NULL;
}

static struct l2cap_chan *__l2cap_get_chan_by_scid(struct l2cap_conn *conn,
						   u16 cid)
{
	struct l2cap_chan *c;

	list_for_each_entry(c, &conn->chan_l, list) {
		if (c->scid == cid)
			return c;
	}
	return NULL;
}

/* Find channel with given SCID.
 * Returns a reference locked channel.
 */
static struct l2cap_chan *l2cap_get_chan_by_scid(struct l2cap_conn *conn,
						 u16 cid)
{
	struct l2cap_chan *c;

	mutex_lock(&conn->chan_lock);
	c = __l2cap_get_chan_by_scid(conn, cid);
	if (c) {
		/* Only lock if chan reference is not 0 */
		c = l2cap_chan_hold_unless_zero(c);
		if (c)
			l2cap_chan_lock(c);
	}
	mutex_unlock(&conn->chan_lock);

	return c;
}

/* Find channel with given DCID.
 * Returns a reference locked channel.
 */
static struct l2cap_chan *l2cap_get_chan_by_dcid(struct l2cap_conn *conn,
						 u16 cid)
{
	struct l2cap_chan *c;

	mutex_lock(&conn->chan_lock);
	c = __l2cap_get_chan_by_dcid(conn, cid);
	if (c) {
		/* Only lock if chan reference is not 0 */
		c = l2cap_chan_hold_unless_zero(c);
		if (c)
			l2cap_chan_lock(c);
	}
	mutex_unlock(&conn->chan_lock);

	return c;
}

static struct l2cap_chan *__l2cap_get_chan_by_ident(struct l2cap_conn *conn,
						    u8 ident)
{
	struct l2cap_chan *c;

	list_for_each_entry(c, &conn->chan_l, list) {
		if (c->ident == ident)
			return c;
	}
	return NULL;
}

static struct l2cap_chan *__l2cap_global_chan_by_addr(__le16 psm, bdaddr_t *src,
						      u8 src_type)
{
	struct l2cap_chan *c;

	list_for_each_entry(c, &chan_list, global_l) {
		if (src_type == BDADDR_BREDR && c->src_type != BDADDR_BREDR)
			continue;

		if (src_type != BDADDR_BREDR && c->src_type == BDADDR_BREDR)
			continue;

		if (c->sport == psm && !bacmp(&c->src, src))
			return c;
	}
	return NULL;
}

int l2cap_add_psm(struct l2cap_chan *chan, bdaddr_t *src, __le16 psm)
{
	int err;

	write_lock(&chan_list_lock);

	if (psm && __l2cap_global_chan_by_addr(psm, src, chan->src_type)) {
		err = -EADDRINUSE;
		goto done;
	}

	if (psm) {
		chan->psm = psm;
		chan->sport = psm;
		err = 0;
	} else {
		u16 p, start, end, incr;

		if (chan->src_type == BDADDR_BREDR) {
			start = L2CAP_PSM_DYN_START;
			end = L2CAP_PSM_AUTO_END;
			incr = 2;
		} else {
			start = L2CAP_PSM_LE_DYN_START;
			end = L2CAP_PSM_LE_DYN_END;
			incr = 1;
		}

		err = -EINVAL;
		for (p = start; p <= end; p += incr)
			if (!__l2cap_global_chan_by_addr(cpu_to_le16(p), src,
							 chan->src_type)) {
				chan->psm   = cpu_to_le16(p);
				chan->sport = cpu_to_le16(p);
				err = 0;
				break;
			}
	}

done:
	write_unlock(&chan_list_lock);
	return err;
}
EXPORT_SYMBOL_GPL(l2cap_add_psm);

int l2cap_add_scid(struct l2cap_chan *chan,  __u16 scid)
{
	write_lock(&chan_list_lock);

	/* Override the defaults (which are for conn-oriented) */
	chan->omtu = L2CAP_DEFAULT_MTU;
	chan->chan_type = L2CAP_CHAN_FIXED;

	chan->scid = scid;

	write_unlock(&chan_list_lock);

	return 0;
}

static u16 l2cap_alloc_cid(struct l2cap_conn *conn)
{
	u16 cid, dyn_end;

	if (conn->hcon->type == LE_LINK)
		dyn_end = L2CAP_CID_LE_DYN_END;
	else
		dyn_end = L2CAP_CID_DYN_END;

	for (cid = L2CAP_CID_DYN_START; cid <= dyn_end; cid++) {
		if (!__l2cap_get_chan_by_scid(conn, cid))
			return cid;
	}

	return 0;
}

static void l2cap_state_change(struct l2cap_chan *chan, int state)
{
	BT_DBG("chan %p %s -> %s", chan, state_to_string(chan->state),
	       state_to_string(state));

	chan->state = state;
	chan->ops->state_change(chan, state, 0);
}

static inline void l2cap_state_change_and_error(struct l2cap_chan *chan,
						int state, int err)
{
	chan->state = state;
	chan->ops->state_change(chan, chan->state, err);
}

static inline void l2cap_chan_set_err(struct l2cap_chan *chan, int err)
{
	chan->ops->state_change(chan, chan->state, err);
}

static void __set_retrans_timer(struct l2cap_chan *chan)
{
	if (!delayed_work_pending(&chan->monitor_timer) &&
	    chan->retrans_timeout) {
		l2cap_set_timer(chan, &chan->retrans_timer,
				msecs_to_jiffies(chan->retrans_timeout));
	}
}

static void __set_monitor_timer(struct l2cap_chan *chan)
{
	__clear_retrans_timer(chan);
	if (chan->monitor_timeout) {
		l2cap_set_timer(chan, &chan->monitor_timer,
				msecs_to_jiffies(chan->monitor_timeout));
	}
}

static struct sk_buff *l2cap_ertm_seq_in_queue(struct sk_buff_head *head,
					       u16 seq)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct sk_buff *skb;

	skb_queue_walk(head, skb) {
<<<<<<< HEAD
		if (bt_cb(skb)->control.txseq == seq)
=======
		if (bt_cb(skb)->l2cap.txseq == seq)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return skb;
	}

	return NULL;
}

<<<<<<< HEAD
static int l2cap_seq_list_init(struct l2cap_seq_list *seq_list, u16 size)
{
	u16 allocSize = 1;
	int err = 0;
	int i;

	/* Actual allocated size must be a power of 2 */
	while (allocSize && allocSize <= size)
		allocSize <<= 1;
	if (!allocSize)
		return -ENOMEM;

	seq_list->list = kzalloc(sizeof(u16) * allocSize, GFP_ATOMIC);
	if (!seq_list->list)
		return -ENOMEM;

	seq_list->size = allocSize;
	seq_list->mask = allocSize - 1;
	seq_list->head = L2CAP_SEQ_LIST_CLEAR;
	seq_list->tail = L2CAP_SEQ_LIST_CLEAR;
	for (i = 0; i < allocSize; i++)
		seq_list->list[i] = L2CAP_SEQ_LIST_CLEAR;

	return err;
=======
/* ---- L2CAP sequence number lists ---- */

/* For ERTM, ordered lists of sequence numbers must be tracked for
 * SREJ requests that are received and for frames that are to be
 * retransmitted. These seq_list functions implement a singly-linked
 * list in an array, where membership in the list can also be checked
 * in constant time. Items can also be added to the tail of the list
 * and removed from the head in constant time, without further memory
 * allocs or frees.
 */

static int l2cap_seq_list_init(struct l2cap_seq_list *seq_list, u16 size)
{
	size_t alloc_size, i;

	/* Allocated size is a power of 2 to map sequence numbers
	 * (which may be up to 14 bits) in to a smaller array that is
	 * sized for the negotiated ERTM transmit windows.
	 */
	alloc_size = roundup_pow_of_two(size);

	seq_list->list = kmalloc_array(alloc_size, sizeof(u16), GFP_KERNEL);
	if (!seq_list->list)
		return -ENOMEM;

	seq_list->mask = alloc_size - 1;
	seq_list->head = L2CAP_SEQ_LIST_CLEAR;
	seq_list->tail = L2CAP_SEQ_LIST_CLEAR;
	for (i = 0; i < alloc_size; i++)
		seq_list->list[i] = L2CAP_SEQ_LIST_CLEAR;

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void l2cap_seq_list_free(struct l2cap_seq_list *seq_list)
{
	kfree(seq_list->list);
}

static inline bool l2cap_seq_list_contains(struct l2cap_seq_list *seq_list,
<<<<<<< HEAD
					u16 seq)
{
	return seq_list->list[seq & seq_list->mask] != L2CAP_SEQ_LIST_CLEAR;
}

static u16 l2cap_seq_list_remove(struct l2cap_seq_list *seq_list, u16 seq)
{
	u16 mask = seq_list->mask;

	BT_DBG("seq_list %p, seq %d", seq_list, (int) seq);

	if (seq_list->head == L2CAP_SEQ_LIST_CLEAR) {
		/* In case someone tries to pop the head of an empty list */
		BT_DBG("List empty");
		return L2CAP_SEQ_LIST_CLEAR;
	} else if (seq_list->head == seq) {
		/* Head can be removed quickly */
		BT_DBG("Remove head");
		seq_list->head = seq_list->list[seq & mask];
		seq_list->list[seq & mask] = L2CAP_SEQ_LIST_CLEAR;

		if (seq_list->head == L2CAP_SEQ_LIST_TAIL) {
			seq_list->head = L2CAP_SEQ_LIST_CLEAR;
			seq_list->tail = L2CAP_SEQ_LIST_CLEAR;
		}
	} else {
		/* Non-head item must be found first */
		u16 prev = seq_list->head;
		BT_DBG("Find and remove");
		while (seq_list->list[prev & mask] != seq) {
			prev = seq_list->list[prev & mask];
			if (prev == L2CAP_SEQ_LIST_TAIL) {
				BT_DBG("seq %d not in list", (int) seq);
				return L2CAP_SEQ_LIST_CLEAR;
			}
		}

		seq_list->list[prev & mask] = seq_list->list[seq & mask];
		seq_list->list[seq & mask] = L2CAP_SEQ_LIST_CLEAR;
		if (seq_list->tail == seq)
			seq_list->tail = prev;
	}
	return seq;
}

static inline u16 l2cap_seq_list_pop(struct l2cap_seq_list *seq_list)
{
	return l2cap_seq_list_remove(seq_list, seq_list->head);
=======
					   u16 seq)
{
	/* Constant-time check for list membership */
	return seq_list->list[seq & seq_list->mask] != L2CAP_SEQ_LIST_CLEAR;
}

static inline u16 l2cap_seq_list_pop(struct l2cap_seq_list *seq_list)
{
	u16 seq = seq_list->head;
	u16 mask = seq_list->mask;

	seq_list->head = seq_list->list[seq & mask];
	seq_list->list[seq & mask] = L2CAP_SEQ_LIST_CLEAR;

	if (seq_list->head == L2CAP_SEQ_LIST_TAIL) {
		seq_list->head = L2CAP_SEQ_LIST_CLEAR;
		seq_list->tail = L2CAP_SEQ_LIST_CLEAR;
	}

	return seq;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void l2cap_seq_list_clear(struct l2cap_seq_list *seq_list)
{
<<<<<<< HEAD
	if (seq_list->head != L2CAP_SEQ_LIST_CLEAR) {
		u16 i;
		for (i = 0; i < seq_list->size; i++)
			seq_list->list[i] = L2CAP_SEQ_LIST_CLEAR;

		seq_list->head = L2CAP_SEQ_LIST_CLEAR;
		seq_list->tail = L2CAP_SEQ_LIST_CLEAR;
	}
=======
	u16 i;

	if (seq_list->head == L2CAP_SEQ_LIST_CLEAR)
		return;

	for (i = 0; i <= seq_list->mask; i++)
		seq_list->list[i] = L2CAP_SEQ_LIST_CLEAR;

	seq_list->head = L2CAP_SEQ_LIST_CLEAR;
	seq_list->tail = L2CAP_SEQ_LIST_CLEAR;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void l2cap_seq_list_append(struct l2cap_seq_list *seq_list, u16 seq)
{
	u16 mask = seq_list->mask;

<<<<<<< HEAD
	BT_DBG("seq_list %p, seq %d", seq_list, (int) seq);

	if (seq_list->list[seq & mask] == L2CAP_SEQ_LIST_CLEAR) {
		if (seq_list->tail == L2CAP_SEQ_LIST_CLEAR)
			seq_list->head = seq;
		else
			seq_list->list[seq_list->tail & mask] = seq;

		seq_list->tail = seq;
		seq_list->list[seq & mask] = L2CAP_SEQ_LIST_TAIL;
	}
}

static u16 __pack_enhanced_control(struct bt_l2cap_control *control)
{
	u16 packed;

	packed = (control->reqseq << L2CAP_CTRL_REQSEQ_SHIFT) &
		L2CAP_CTRL_REQSEQ;
	packed |= (control->final << L2CAP_CTRL_FINAL_SHIFT) &
		L2CAP_CTRL_FINAL;

	if (control->frame_type == 's') {
		packed |= (control->poll << L2CAP_CTRL_POLL_SHIFT) &
			L2CAP_CTRL_POLL;
		packed |= (control->super << L2CAP_CTRL_SUPERVISE_SHIFT) &
			L2CAP_CTRL_SUPERVISE;
		packed |= L2CAP_CTRL_FRAME_TYPE;
	} else {
		packed |= (control->sar << L2CAP_CTRL_SAR_SHIFT) &
			L2CAP_CTRL_SAR;
		packed |= (control->txseq << L2CAP_CTRL_TXSEQ_SHIFT) &
			L2CAP_CTRL_TXSEQ;
	}

	return packed;
}

static void __get_enhanced_control(u16 enhanced,
					struct bt_l2cap_control *control)
{
	control->reqseq = (enhanced & L2CAP_CTRL_REQSEQ) >>
		L2CAP_CTRL_REQSEQ_SHIFT;
	control->final = (enhanced & L2CAP_CTRL_FINAL) >>
		L2CAP_CTRL_FINAL_SHIFT;

	if (enhanced & L2CAP_CTRL_FRAME_TYPE) {
		control->frame_type = 's';
		control->poll = (enhanced & L2CAP_CTRL_POLL) >>
			L2CAP_CTRL_POLL_SHIFT;
		control->super = (enhanced & L2CAP_CTRL_SUPERVISE) >>
			L2CAP_CTRL_SUPERVISE_SHIFT;

		control->sar = 0;
		control->txseq = 0;
	} else {
		control->frame_type = 'i';
		control->sar = (enhanced & L2CAP_CTRL_SAR) >>
			L2CAP_CTRL_SAR_SHIFT;
		control->txseq = (enhanced & L2CAP_CTRL_TXSEQ) >>
			L2CAP_CTRL_TXSEQ_SHIFT;

		control->poll = 0;
		control->super = 0;
	}
}

static u32 __pack_extended_control(struct bt_l2cap_control *control)
{
	u32 packed;

	packed = (control->reqseq << L2CAP_EXT_CTRL_REQSEQ_SHIFT) &
		L2CAP_EXT_CTRL_REQSEQ;
	packed |= (control->final << L2CAP_EXT_CTRL_FINAL_SHIFT) &
		L2CAP_EXT_CTRL_FINAL;

	if (control->frame_type == 's') {
		packed |= (control->poll << L2CAP_EXT_CTRL_POLL_SHIFT) &
			L2CAP_EXT_CTRL_POLL;
		packed |= (control->super << L2CAP_EXT_CTRL_SUPERVISE_SHIFT) &
			L2CAP_EXT_CTRL_SUPERVISE;
		packed |= L2CAP_EXT_CTRL_FRAME_TYPE;
	} else {
		packed |= (control->sar << L2CAP_EXT_CTRL_SAR_SHIFT) &
			L2CAP_EXT_CTRL_SAR;
		packed |= (control->txseq << L2CAP_EXT_CTRL_TXSEQ_SHIFT) &
			L2CAP_EXT_CTRL_TXSEQ;
	}

	return packed;
}

static void __get_extended_control(u32 extended,
				struct bt_l2cap_control *control)
{
	control->reqseq = (extended & L2CAP_EXT_CTRL_REQSEQ) >>
		L2CAP_EXT_CTRL_REQSEQ_SHIFT;
	control->final = (extended & L2CAP_EXT_CTRL_FINAL) >>
		L2CAP_EXT_CTRL_FINAL_SHIFT;

	if (extended & L2CAP_EXT_CTRL_FRAME_TYPE) {
		control->frame_type = 's';
		control->poll = (extended & L2CAP_EXT_CTRL_POLL) >>
			L2CAP_EXT_CTRL_POLL_SHIFT;
		control->super = (extended & L2CAP_EXT_CTRL_SUPERVISE) >>
			L2CAP_EXT_CTRL_SUPERVISE_SHIFT;

		control->sar = 0;
		control->txseq = 0;
	} else {
		control->frame_type = 'i';
		control->sar = (extended & L2CAP_EXT_CTRL_SAR) >>
			L2CAP_EXT_CTRL_SAR_SHIFT;
		control->txseq = (extended & L2CAP_EXT_CTRL_TXSEQ) >>
			L2CAP_EXT_CTRL_TXSEQ_SHIFT;

		control->poll = 0;
		control->super = 0;
	}
}

static inline void l2cap_ertm_stop_ack_timer(struct l2cap_pinfo *pi)
{
	BT_DBG("pi %p", pi);
	__cancel_delayed_work(&pi->ack_work);
}

static inline void l2cap_ertm_start_ack_timer(struct l2cap_pinfo *pi)
{
	BT_DBG("pi %p, pending %d", pi, delayed_work_pending(&pi->ack_work));
	if (!delayed_work_pending(&pi->ack_work)) {
		queue_delayed_work(_l2cap_wq, &pi->ack_work,
				msecs_to_jiffies(L2CAP_DEFAULT_ACK_TO));
	}
}

static inline void l2cap_ertm_stop_retrans_timer(struct l2cap_pinfo *pi)
{
	BT_DBG("pi %p", pi);
	__cancel_delayed_work(&pi->retrans_work);
}

static inline void l2cap_ertm_start_retrans_timer(struct l2cap_pinfo *pi)
{
	BT_DBG("pi %p", pi);
	if (!delayed_work_pending(&pi->monitor_work) && pi->retrans_timeout) {
		__cancel_delayed_work(&pi->retrans_work);
		queue_delayed_work(_l2cap_wq, &pi->retrans_work,
			msecs_to_jiffies(pi->retrans_timeout));
	}
}

static inline void l2cap_ertm_stop_monitor_timer(struct l2cap_pinfo *pi)
{
	BT_DBG("pi %p", pi);
	__cancel_delayed_work(&pi->monitor_work);
}

static inline void l2cap_ertm_start_monitor_timer(struct l2cap_pinfo *pi)
{
	BT_DBG("pi %p", pi);
	l2cap_ertm_stop_retrans_timer(pi);
	__cancel_delayed_work(&pi->monitor_work);
	if (pi->monitor_timeout) {
		queue_delayed_work(_l2cap_wq, &pi->monitor_work,
				msecs_to_jiffies(pi->monitor_timeout));
	}
}

static u16 l2cap_alloc_cid(struct l2cap_chan_list *l)
{
	u16 cid = L2CAP_CID_DYN_START;

	for (; cid < L2CAP_CID_DYN_END; cid++) {
		if (!__l2cap_get_chan_by_scid(l, cid))
			return cid;
	}

	return 0;
}

static inline void __l2cap_chan_link(struct l2cap_chan_list *l, struct sock *sk)
{
	sock_hold(sk);

	if (l->head)
		l2cap_pi(l->head)->prev_c = sk;

	l2cap_pi(sk)->next_c = l->head;
	l2cap_pi(sk)->prev_c = NULL;
	l->head = sk;
}

static inline void l2cap_chan_unlink(struct l2cap_chan_list *l, struct sock *sk)
{
	struct sock *next = l2cap_pi(sk)->next_c, *prev = l2cap_pi(sk)->prev_c;

	write_lock_bh(&l->lock);
	if (sk == l->head)
		l->head = next;

	if (next)
		l2cap_pi(next)->prev_c = prev;
	if (prev)
		l2cap_pi(prev)->next_c = next;
	write_unlock_bh(&l->lock);

	__sock_put(sk);
}

static void __l2cap_chan_add(struct l2cap_conn *conn, struct sock *sk)
{
	struct l2cap_chan_list *l = &conn->chan_list;

	BT_DBG("conn %p, psm 0x%2.2x, dcid 0x%4.4x", conn,
			l2cap_pi(sk)->psm, l2cap_pi(sk)->dcid);

	conn->disc_reason = 0x13;

	l2cap_pi(sk)->conn = conn;

	if (!l2cap_pi(sk)->fixed_channel &&
		(sk->sk_type == SOCK_SEQPACKET || sk->sk_type == SOCK_STREAM)) {
		if (conn->hcon->type == LE_LINK) {
			/* LE connection */
			if (l2cap_pi(sk)->imtu < L2CAP_LE_DEFAULT_MTU)
				l2cap_pi(sk)->imtu = L2CAP_LE_DEFAULT_MTU;
			if (l2cap_pi(sk)->omtu < L2CAP_LE_DEFAULT_MTU)
				l2cap_pi(sk)->omtu = L2CAP_LE_DEFAULT_MTU;

			l2cap_pi(sk)->scid = L2CAP_CID_LE_DATA;
			l2cap_pi(sk)->dcid = L2CAP_CID_LE_DATA;
		} else {
			/* Alloc CID for connection-oriented socket */
			l2cap_pi(sk)->scid = l2cap_alloc_cid(l);
			l2cap_pi(sk)->omtu = L2CAP_DEFAULT_MTU;
		}
	} else if (sk->sk_type == SOCK_DGRAM) {
		/* Connectionless socket */
		l2cap_pi(sk)->scid = L2CAP_CID_CONN_LESS;
		l2cap_pi(sk)->dcid = L2CAP_CID_CONN_LESS;
		l2cap_pi(sk)->omtu = L2CAP_DEFAULT_MTU;
	} else if (sk->sk_type == SOCK_RAW) {
		/* Raw socket can send/recv signalling messages only */
		l2cap_pi(sk)->scid = L2CAP_CID_SIGNALING;
		l2cap_pi(sk)->dcid = L2CAP_CID_SIGNALING;
		l2cap_pi(sk)->omtu = L2CAP_DEFAULT_MTU;
	}

	if (l2cap_get_smallest_flushto(l) > l2cap_pi(sk)->flush_to) {
		/*if flush timeout of the channel is lesser than existing */
		l2cap_set_acl_flushto(conn->hcon, l2cap_pi(sk)->flush_to);
	}
	/* Otherwise, do not set scid/dcid/omtu.  These will be set up
	 * by l2cap_fixed_channel_config()
	 */

	__l2cap_chan_link(l, sk);
}

/* Delete channel.
 * Must be called on the locked socket. */
void l2cap_chan_del(struct sock *sk, int err)
{
	struct l2cap_conn *conn = l2cap_pi(sk)->conn;
	struct sock *parent = bt_sk(sk)->parent;

	l2cap_sock_clear_timer(sk);

	BT_DBG("sk %p, conn %p, err %d", sk, conn, err);

	if (conn) {
		struct l2cap_chan_list *l = &conn->chan_list;
		/* Unlink from channel list */
		l2cap_chan_unlink(l, sk);
		l2cap_pi(sk)->conn = NULL;
		if (!l2cap_pi(sk)->fixed_channel)
			hci_conn_put(conn->hcon);

		read_lock(&l->lock);
		if (l2cap_pi(sk)->flush_to < l2cap_get_smallest_flushto(l))
			l2cap_set_acl_flushto(conn->hcon,
				l2cap_get_smallest_flushto(l));
		read_unlock(&l->lock);
	}

	if (l2cap_pi(sk)->ampchan) {
		struct hci_chan *ampchan = l2cap_pi(sk)->ampchan;
		struct hci_conn *ampcon = l2cap_pi(sk)->ampcon;
		l2cap_pi(sk)->ampchan = NULL;
		l2cap_pi(sk)->ampcon = NULL;
		l2cap_pi(sk)->amp_id = 0;
		if (hci_chan_put(ampchan))
			ampcon->l2cap_data = NULL;
		else
			l2cap_deaggregate(ampchan, l2cap_pi(sk));
	}

	sk->sk_state = BT_CLOSED;
	sock_set_flag(sk, SOCK_ZAPPED);

	if (err)
		sk->sk_err = err;

	if (parent) {
		bt_accept_unlink(sk);
		parent->sk_data_ready(parent, 0);
	} else
		sk->sk_state_change(sk);

	sk->sk_send_head = NULL;
	skb_queue_purge(TX_QUEUE(sk));

	if (l2cap_pi(sk)->mode == L2CAP_MODE_ERTM) {
		if (l2cap_pi(sk)->sdu)
			kfree_skb(l2cap_pi(sk)->sdu);

		skb_queue_purge(SREJ_QUEUE(sk));

		__cancel_delayed_work(&l2cap_pi(sk)->ack_work);
		__cancel_delayed_work(&l2cap_pi(sk)->retrans_work);
		__cancel_delayed_work(&l2cap_pi(sk)->monitor_work);
	}
}

static inline u8 l2cap_get_auth_type(struct sock *sk)
{
	if (sk->sk_type == SOCK_RAW) {
		switch (l2cap_pi(sk)->sec_level) {
		case BT_SECURITY_VERY_HIGH:
		case BT_SECURITY_HIGH:
=======
	/* All appends happen in constant time */

	if (seq_list->list[seq & mask] != L2CAP_SEQ_LIST_CLEAR)
		return;

	if (seq_list->tail == L2CAP_SEQ_LIST_CLEAR)
		seq_list->head = seq;
	else
		seq_list->list[seq_list->tail & mask] = seq;

	seq_list->tail = seq;
	seq_list->list[seq & mask] = L2CAP_SEQ_LIST_TAIL;
}

static void l2cap_chan_timeout(struct work_struct *work)
{
	struct l2cap_chan *chan = container_of(work, struct l2cap_chan,
					       chan_timer.work);
	struct l2cap_conn *conn = chan->conn;
	int reason;

	BT_DBG("chan %p state %s", chan, state_to_string(chan->state));

	if (!conn)
		return;

	mutex_lock(&conn->chan_lock);
	/* __set_chan_timer() calls l2cap_chan_hold(chan) while scheduling
	 * this work. No need to call l2cap_chan_hold(chan) here again.
	 */
	l2cap_chan_lock(chan);

	if (chan->state == BT_CONNECTED || chan->state == BT_CONFIG)
		reason = ECONNREFUSED;
	else if (chan->state == BT_CONNECT &&
		 chan->sec_level != BT_SECURITY_SDP)
		reason = ECONNREFUSED;
	else
		reason = ETIMEDOUT;

	l2cap_chan_close(chan, reason);

	chan->ops->close(chan);

	l2cap_chan_unlock(chan);
	l2cap_chan_put(chan);

	mutex_unlock(&conn->chan_lock);
}

struct l2cap_chan *l2cap_chan_create(void)
{
	struct l2cap_chan *chan;

	chan = kzalloc(sizeof(*chan), GFP_ATOMIC);
	if (!chan)
		return NULL;

	skb_queue_head_init(&chan->tx_q);
	skb_queue_head_init(&chan->srej_q);
	mutex_init(&chan->lock);

	/* Set default lock nesting level */
	atomic_set(&chan->nesting, L2CAP_NESTING_NORMAL);

	write_lock(&chan_list_lock);
	list_add(&chan->global_l, &chan_list);
	write_unlock(&chan_list_lock);

	INIT_DELAYED_WORK(&chan->chan_timer, l2cap_chan_timeout);
	INIT_DELAYED_WORK(&chan->retrans_timer, l2cap_retrans_timeout);
	INIT_DELAYED_WORK(&chan->monitor_timer, l2cap_monitor_timeout);
	INIT_DELAYED_WORK(&chan->ack_timer, l2cap_ack_timeout);

	chan->state = BT_OPEN;

	kref_init(&chan->kref);

	/* This flag is cleared in l2cap_chan_ready() */
	set_bit(CONF_NOT_COMPLETE, &chan->conf_state);

	BT_DBG("chan %p", chan);

	return chan;
}
EXPORT_SYMBOL_GPL(l2cap_chan_create);

static void l2cap_chan_destroy(struct kref *kref)
{
	struct l2cap_chan *chan = container_of(kref, struct l2cap_chan, kref);

	BT_DBG("chan %p", chan);

	write_lock(&chan_list_lock);
	list_del(&chan->global_l);
	write_unlock(&chan_list_lock);

	kfree(chan);
}

void l2cap_chan_hold(struct l2cap_chan *c)
{
	BT_DBG("chan %p orig refcnt %u", c, kref_read(&c->kref));

	kref_get(&c->kref);
}

struct l2cap_chan *l2cap_chan_hold_unless_zero(struct l2cap_chan *c)
{
	BT_DBG("chan %p orig refcnt %u", c, kref_read(&c->kref));

	if (!kref_get_unless_zero(&c->kref))
		return NULL;

	return c;
}

void l2cap_chan_put(struct l2cap_chan *c)
{
	BT_DBG("chan %p orig refcnt %u", c, kref_read(&c->kref));

	kref_put(&c->kref, l2cap_chan_destroy);
}
EXPORT_SYMBOL_GPL(l2cap_chan_put);

void l2cap_chan_set_defaults(struct l2cap_chan *chan)
{
	chan->fcs  = L2CAP_FCS_CRC16;
	chan->max_tx = L2CAP_DEFAULT_MAX_TX;
	chan->tx_win = L2CAP_DEFAULT_TX_WINDOW;
	chan->tx_win_max = L2CAP_DEFAULT_TX_WINDOW;
	chan->remote_max_tx = chan->max_tx;
	chan->remote_tx_win = chan->tx_win;
	chan->ack_win = L2CAP_DEFAULT_TX_WINDOW;
	chan->sec_level = BT_SECURITY_LOW;
	chan->flush_to = L2CAP_DEFAULT_FLUSH_TO;
	chan->retrans_timeout = L2CAP_DEFAULT_RETRANS_TO;
	chan->monitor_timeout = L2CAP_DEFAULT_MONITOR_TO;

	chan->conf_state = 0;
	set_bit(CONF_NOT_COMPLETE, &chan->conf_state);

	set_bit(FLAG_FORCE_ACTIVE, &chan->flags);
}
EXPORT_SYMBOL_GPL(l2cap_chan_set_defaults);

static void l2cap_le_flowctl_init(struct l2cap_chan *chan, u16 tx_credits)
{
	chan->sdu = NULL;
	chan->sdu_last_frag = NULL;
	chan->sdu_len = 0;
	chan->tx_credits = tx_credits;
	/* Derive MPS from connection MTU to stop HCI fragmentation */
	chan->mps = min_t(u16, chan->imtu, chan->conn->mtu - L2CAP_HDR_SIZE);
	/* Give enough credits for a full packet */
	chan->rx_credits = (chan->imtu / chan->mps) + 1;

	skb_queue_head_init(&chan->tx_q);
}

static void l2cap_ecred_init(struct l2cap_chan *chan, u16 tx_credits)
{
	l2cap_le_flowctl_init(chan, tx_credits);

	/* L2CAP implementations shall support a minimum MPS of 64 octets */
	if (chan->mps < L2CAP_ECRED_MIN_MPS) {
		chan->mps = L2CAP_ECRED_MIN_MPS;
		chan->rx_credits = (chan->imtu / chan->mps) + 1;
	}
}

void __l2cap_chan_add(struct l2cap_conn *conn, struct l2cap_chan *chan)
{
	BT_DBG("conn %p, psm 0x%2.2x, dcid 0x%4.4x", conn,
	       __le16_to_cpu(chan->psm), chan->dcid);

	conn->disc_reason = HCI_ERROR_REMOTE_USER_TERM;

	chan->conn = conn;

	switch (chan->chan_type) {
	case L2CAP_CHAN_CONN_ORIENTED:
		/* Alloc CID for connection-oriented socket */
		chan->scid = l2cap_alloc_cid(conn);
		if (conn->hcon->type == ACL_LINK)
			chan->omtu = L2CAP_DEFAULT_MTU;
		break;

	case L2CAP_CHAN_CONN_LESS:
		/* Connectionless socket */
		chan->scid = L2CAP_CID_CONN_LESS;
		chan->dcid = L2CAP_CID_CONN_LESS;
		chan->omtu = L2CAP_DEFAULT_MTU;
		break;

	case L2CAP_CHAN_FIXED:
		/* Caller will set CID and CID specific MTU values */
		break;

	default:
		/* Raw socket can send/recv signalling messages only */
		chan->scid = L2CAP_CID_SIGNALING;
		chan->dcid = L2CAP_CID_SIGNALING;
		chan->omtu = L2CAP_DEFAULT_MTU;
	}

	chan->local_id		= L2CAP_BESTEFFORT_ID;
	chan->local_stype	= L2CAP_SERV_BESTEFFORT;
	chan->local_msdu	= L2CAP_DEFAULT_MAX_SDU_SIZE;
	chan->local_sdu_itime	= L2CAP_DEFAULT_SDU_ITIME;
	chan->local_acc_lat	= L2CAP_DEFAULT_ACC_LAT;
	chan->local_flush_to	= L2CAP_EFS_DEFAULT_FLUSH_TO;

	l2cap_chan_hold(chan);

	/* Only keep a reference for fixed channels if they requested it */
	if (chan->chan_type != L2CAP_CHAN_FIXED ||
	    test_bit(FLAG_HOLD_HCI_CONN, &chan->flags))
		hci_conn_hold(conn->hcon);

	list_add(&chan->list, &conn->chan_l);
}

void l2cap_chan_add(struct l2cap_conn *conn, struct l2cap_chan *chan)
{
	mutex_lock(&conn->chan_lock);
	__l2cap_chan_add(conn, chan);
	mutex_unlock(&conn->chan_lock);
}

void l2cap_chan_del(struct l2cap_chan *chan, int err)
{
	struct l2cap_conn *conn = chan->conn;

	__clear_chan_timer(chan);

	BT_DBG("chan %p, conn %p, err %d, state %s", chan, conn, err,
	       state_to_string(chan->state));

	chan->ops->teardown(chan, err);

	if (conn) {
		/* Delete from channel list */
		list_del(&chan->list);

		l2cap_chan_put(chan);

		chan->conn = NULL;

		/* Reference was only held for non-fixed channels or
		 * fixed channels that explicitly requested it using the
		 * FLAG_HOLD_HCI_CONN flag.
		 */
		if (chan->chan_type != L2CAP_CHAN_FIXED ||
		    test_bit(FLAG_HOLD_HCI_CONN, &chan->flags))
			hci_conn_drop(conn->hcon);
	}

	if (test_bit(CONF_NOT_COMPLETE, &chan->conf_state))
		return;

	switch (chan->mode) {
	case L2CAP_MODE_BASIC:
		break;

	case L2CAP_MODE_LE_FLOWCTL:
	case L2CAP_MODE_EXT_FLOWCTL:
		skb_queue_purge(&chan->tx_q);
		break;

	case L2CAP_MODE_ERTM:
		__clear_retrans_timer(chan);
		__clear_monitor_timer(chan);
		__clear_ack_timer(chan);

		skb_queue_purge(&chan->srej_q);

		l2cap_seq_list_free(&chan->srej_list);
		l2cap_seq_list_free(&chan->retrans_list);
		fallthrough;

	case L2CAP_MODE_STREAMING:
		skb_queue_purge(&chan->tx_q);
		break;
	}
}
EXPORT_SYMBOL_GPL(l2cap_chan_del);

static void __l2cap_chan_list_id(struct l2cap_conn *conn, u16 id,
				 l2cap_chan_func_t func, void *data)
{
	struct l2cap_chan *chan, *l;

	list_for_each_entry_safe(chan, l, &conn->chan_l, list) {
		if (chan->ident == id)
			func(chan, data);
	}
}

static void __l2cap_chan_list(struct l2cap_conn *conn, l2cap_chan_func_t func,
			      void *data)
{
	struct l2cap_chan *chan;

	list_for_each_entry(chan, &conn->chan_l, list) {
		func(chan, data);
	}
}

void l2cap_chan_list(struct l2cap_conn *conn, l2cap_chan_func_t func,
		     void *data)
{
	if (!conn)
		return;

	mutex_lock(&conn->chan_lock);
	__l2cap_chan_list(conn, func, data);
	mutex_unlock(&conn->chan_lock);
}

EXPORT_SYMBOL_GPL(l2cap_chan_list);

static void l2cap_conn_update_id_addr(struct work_struct *work)
{
	struct l2cap_conn *conn = container_of(work, struct l2cap_conn,
					       id_addr_timer.work);
	struct hci_conn *hcon = conn->hcon;
	struct l2cap_chan *chan;

	mutex_lock(&conn->chan_lock);

	list_for_each_entry(chan, &conn->chan_l, list) {
		l2cap_chan_lock(chan);
		bacpy(&chan->dst, &hcon->dst);
		chan->dst_type = bdaddr_dst_type(hcon);
		l2cap_chan_unlock(chan);
	}

	mutex_unlock(&conn->chan_lock);
}

static void l2cap_chan_le_connect_reject(struct l2cap_chan *chan)
{
	struct l2cap_conn *conn = chan->conn;
	struct l2cap_le_conn_rsp rsp;
	u16 result;

	if (test_bit(FLAG_DEFER_SETUP, &chan->flags))
		result = L2CAP_CR_LE_AUTHORIZATION;
	else
		result = L2CAP_CR_LE_BAD_PSM;

	l2cap_state_change(chan, BT_DISCONN);

	rsp.dcid    = cpu_to_le16(chan->scid);
	rsp.mtu     = cpu_to_le16(chan->imtu);
	rsp.mps     = cpu_to_le16(chan->mps);
	rsp.credits = cpu_to_le16(chan->rx_credits);
	rsp.result  = cpu_to_le16(result);

	l2cap_send_cmd(conn, chan->ident, L2CAP_LE_CONN_RSP, sizeof(rsp),
		       &rsp);
}

static void l2cap_chan_ecred_connect_reject(struct l2cap_chan *chan)
{
	l2cap_state_change(chan, BT_DISCONN);

	__l2cap_ecred_conn_rsp_defer(chan);
}

static void l2cap_chan_connect_reject(struct l2cap_chan *chan)
{
	struct l2cap_conn *conn = chan->conn;
	struct l2cap_conn_rsp rsp;
	u16 result;

	if (test_bit(FLAG_DEFER_SETUP, &chan->flags))
		result = L2CAP_CR_SEC_BLOCK;
	else
		result = L2CAP_CR_BAD_PSM;

	l2cap_state_change(chan, BT_DISCONN);

	rsp.scid   = cpu_to_le16(chan->dcid);
	rsp.dcid   = cpu_to_le16(chan->scid);
	rsp.result = cpu_to_le16(result);
	rsp.status = cpu_to_le16(L2CAP_CS_NO_INFO);

	l2cap_send_cmd(conn, chan->ident, L2CAP_CONN_RSP, sizeof(rsp), &rsp);
}

void l2cap_chan_close(struct l2cap_chan *chan, int reason)
{
	struct l2cap_conn *conn = chan->conn;

	BT_DBG("chan %p state %s", chan, state_to_string(chan->state));

	switch (chan->state) {
	case BT_LISTEN:
		chan->ops->teardown(chan, 0);
		break;

	case BT_CONNECTED:
	case BT_CONFIG:
		if (chan->chan_type == L2CAP_CHAN_CONN_ORIENTED) {
			__set_chan_timer(chan, chan->ops->get_sndtimeo(chan));
			l2cap_send_disconn_req(chan, reason);
		} else
			l2cap_chan_del(chan, reason);
		break;

	case BT_CONNECT2:
		if (chan->chan_type == L2CAP_CHAN_CONN_ORIENTED) {
			if (conn->hcon->type == ACL_LINK)
				l2cap_chan_connect_reject(chan);
			else if (conn->hcon->type == LE_LINK) {
				switch (chan->mode) {
				case L2CAP_MODE_LE_FLOWCTL:
					l2cap_chan_le_connect_reject(chan);
					break;
				case L2CAP_MODE_EXT_FLOWCTL:
					l2cap_chan_ecred_connect_reject(chan);
					return;
				}
			}
		}

		l2cap_chan_del(chan, reason);
		break;

	case BT_CONNECT:
	case BT_DISCONN:
		l2cap_chan_del(chan, reason);
		break;

	default:
		chan->ops->teardown(chan, 0);
		break;
	}
}
EXPORT_SYMBOL(l2cap_chan_close);

static inline u8 l2cap_get_auth_type(struct l2cap_chan *chan)
{
	switch (chan->chan_type) {
	case L2CAP_CHAN_RAW:
		switch (chan->sec_level) {
		case BT_SECURITY_HIGH:
		case BT_SECURITY_FIPS:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return HCI_AT_DEDICATED_BONDING_MITM;
		case BT_SECURITY_MEDIUM:
			return HCI_AT_DEDICATED_BONDING;
		default:
			return HCI_AT_NO_BONDING;
		}
<<<<<<< HEAD
	} else if (l2cap_pi(sk)->psm == cpu_to_le16(0x0001)) {
		if (l2cap_pi(sk)->sec_level == BT_SECURITY_LOW)
			l2cap_pi(sk)->sec_level = BT_SECURITY_SDP;

		if (l2cap_pi(sk)->sec_level == BT_SECURITY_HIGH ||
			l2cap_pi(sk)->sec_level == BT_SECURITY_VERY_HIGH)
			return HCI_AT_NO_BONDING_MITM;
		else
			return HCI_AT_NO_BONDING;
	} else {
		switch (l2cap_pi(sk)->sec_level) {
		case BT_SECURITY_VERY_HIGH:
		case BT_SECURITY_HIGH:
=======
		break;
	case L2CAP_CHAN_CONN_LESS:
		if (chan->psm == cpu_to_le16(L2CAP_PSM_3DSP)) {
			if (chan->sec_level == BT_SECURITY_LOW)
				chan->sec_level = BT_SECURITY_SDP;
		}
		if (chan->sec_level == BT_SECURITY_HIGH ||
		    chan->sec_level == BT_SECURITY_FIPS)
			return HCI_AT_NO_BONDING_MITM;
		else
			return HCI_AT_NO_BONDING;
		break;
	case L2CAP_CHAN_CONN_ORIENTED:
		if (chan->psm == cpu_to_le16(L2CAP_PSM_SDP)) {
			if (chan->sec_level == BT_SECURITY_LOW)
				chan->sec_level = BT_SECURITY_SDP;

			if (chan->sec_level == BT_SECURITY_HIGH ||
			    chan->sec_level == BT_SECURITY_FIPS)
				return HCI_AT_NO_BONDING_MITM;
			else
				return HCI_AT_NO_BONDING;
		}
		fallthrough;

	default:
		switch (chan->sec_level) {
		case BT_SECURITY_HIGH:
		case BT_SECURITY_FIPS:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return HCI_AT_GENERAL_BONDING_MITM;
		case BT_SECURITY_MEDIUM:
			return HCI_AT_GENERAL_BONDING;
		default:
			return HCI_AT_NO_BONDING;
		}
<<<<<<< HEAD
=======
		break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

/* Service level security */
<<<<<<< HEAD
static inline int l2cap_check_security(struct sock *sk)
{
	struct l2cap_conn *conn = l2cap_pi(sk)->conn;
	__u8 auth_type;

	auth_type = l2cap_get_auth_type(sk);

	return hci_conn_security(conn->hcon, l2cap_pi(sk)->sec_level,
								auth_type);
}

u8 l2cap_get_ident(struct l2cap_conn *conn)
=======
int l2cap_chan_check_security(struct l2cap_chan *chan, bool initiator)
{
	struct l2cap_conn *conn = chan->conn;
	__u8 auth_type;

	if (conn->hcon->type == LE_LINK)
		return smp_conn_security(conn->hcon, chan->sec_level);

	auth_type = l2cap_get_auth_type(chan);

	return hci_conn_security(conn->hcon, chan->sec_level, auth_type,
				 initiator);
}

static u8 l2cap_get_ident(struct l2cap_conn *conn)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	u8 id;

	/* Get next available identificator.
	 *    1 - 128 are used by kernel.
	 *  129 - 199 are reserved.
	 *  200 - 254 are used by utilities like l2ping, etc.
	 */

<<<<<<< HEAD
	spin_lock_bh(&conn->lock);
=======
	mutex_lock(&conn->ident_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (++conn->tx_ident > 128)
		conn->tx_ident = 1;

	id = conn->tx_ident;

<<<<<<< HEAD
	spin_unlock_bh(&conn->lock);
=======
	mutex_unlock(&conn->ident_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return id;
}

<<<<<<< HEAD
static void apply_fcs(struct sk_buff *skb)
{
	size_t len;
	u16 partial_crc;
	struct sk_buff *iter;
	struct sk_buff *final_frag = skb;

	if (skb_has_frag_list(skb))
		len = skb_headlen(skb);
	else
		len = skb->len - L2CAP_FCS_SIZE;

	partial_crc = crc16(0, (u8 *) skb->data, len);

	skb_walk_frags(skb, iter) {
		len = iter->len;
		if (!iter->next)
			len -= L2CAP_FCS_SIZE;

		partial_crc = crc16(partial_crc, iter->data, len);
		final_frag = iter;
	}

	put_unaligned_le16(partial_crc,
		final_frag->data + final_frag->len - L2CAP_FCS_SIZE);
}

void l2cap_send_cmd(struct l2cap_conn *conn, u8 ident, u8 code, u16 len, void *data)
=======
static void l2cap_send_cmd(struct l2cap_conn *conn, u8 ident, u8 code, u16 len,
			   void *data)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct sk_buff *skb = l2cap_build_cmd(conn, code, ident, len, data);
	u8 flags;

	BT_DBG("code 0x%2.2x", code);

	if (!skb)
		return;

<<<<<<< HEAD
	if (conn->hcon == NULL || conn->hcon->hdev == NULL)
		return;

	if (lmp_no_flush_capable(conn->hcon->hdev))
=======
	/* Use NO_FLUSH if supported or we have an LE link (which does
	 * not support auto-flushing packets) */
	if (lmp_no_flush_capable(conn->hcon->hdev) ||
	    conn->hcon->type == LE_LINK)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		flags = ACL_START_NO_FLUSH;
	else
		flags = ACL_START;

<<<<<<< HEAD
	bt_cb(skb)->force_active = 1;

	hci_send_acl(conn->hcon, NULL, skb, flags);
}

static inline int __l2cap_no_conn_pending(struct sock *sk)
{
	return !(l2cap_pi(sk)->conf_state & L2CAP_CONF_CONNECT_PEND);
}

static void l2cap_send_conn_req(struct sock *sk)
{
	struct l2cap_conn_req req;
	req.scid = cpu_to_le16(l2cap_pi(sk)->scid);
	req.psm  = l2cap_pi(sk)->psm;

	l2cap_pi(sk)->ident = l2cap_get_ident(l2cap_pi(sk)->conn);

	l2cap_send_cmd(l2cap_pi(sk)->conn, l2cap_pi(sk)->ident,
			L2CAP_CONN_REQ, sizeof(req), &req);
}

static void l2cap_send_create_chan_req(struct sock *sk, u8 amp_id)
{
	struct l2cap_create_chan_req req;
	req.scid = cpu_to_le16(l2cap_pi(sk)->scid);
	req.psm  = l2cap_pi(sk)->psm;
	req.amp_id = amp_id;

	l2cap_pi(sk)->conf_state |= L2CAP_CONF_LOCKSTEP;
	l2cap_pi(sk)->ident = l2cap_get_ident(l2cap_pi(sk)->conn);

	l2cap_send_cmd(l2cap_pi(sk)->conn, l2cap_pi(sk)->ident,
			L2CAP_CREATE_CHAN_REQ, sizeof(req), &req);
}

static void l2cap_do_start(struct sock *sk)
{
	struct l2cap_conn *conn = l2cap_pi(sk)->conn;

	if (conn->info_state & L2CAP_INFO_FEAT_MASK_REQ_SENT) {
		if (!(conn->info_state & L2CAP_INFO_FEAT_MASK_REQ_DONE))
			return;

		if (l2cap_check_security(sk) && __l2cap_no_conn_pending(sk)) {
			l2cap_pi(sk)->conf_state |= L2CAP_CONF_CONNECT_PEND;

			if (l2cap_pi(sk)->amp_pref ==
					BT_AMP_POLICY_PREFER_AMP &&
					enable_hs &&
					conn->fc_mask & L2CAP_FC_A2MP)
				amp_create_physical(conn, sk);
			else
				l2cap_send_conn_req(sk);
		}
	} else {
		struct l2cap_info_req req;
		req.type = cpu_to_le16(L2CAP_IT_FEAT_MASK);

		conn->info_state |= L2CAP_INFO_FEAT_MASK_REQ_SENT;
		conn->info_ident = l2cap_get_ident(conn);

		mod_timer(&conn->info_timer, jiffies +
					msecs_to_jiffies(L2CAP_INFO_TIMEOUT));

		l2cap_send_cmd(conn, conn->info_ident,
					L2CAP_INFO_REQ, sizeof(req), &req);
	}
}

=======
	bt_cb(skb)->force_active = BT_POWER_FORCE_ACTIVE_ON;
	skb->priority = HCI_PRIO_MAX;

	hci_send_acl(conn->hchan, skb, flags);
}

static void l2cap_do_send(struct l2cap_chan *chan, struct sk_buff *skb)
{
	struct hci_conn *hcon = chan->conn->hcon;
	u16 flags;

	BT_DBG("chan %p, skb %p len %d priority %u", chan, skb, skb->len,
	       skb->priority);

	/* Use NO_FLUSH for LE links (where this is the only option) or
	 * if the BR/EDR link supports it and flushing has not been
	 * explicitly requested (through FLAG_FLUSHABLE).
	 */
	if (hcon->type == LE_LINK ||
	    (!test_bit(FLAG_FLUSHABLE, &chan->flags) &&
	     lmp_no_flush_capable(hcon->hdev)))
		flags = ACL_START_NO_FLUSH;
	else
		flags = ACL_START;

	bt_cb(skb)->force_active = test_bit(FLAG_FORCE_ACTIVE, &chan->flags);
	hci_send_acl(chan->conn->hchan, skb, flags);
}

static void __unpack_enhanced_control(u16 enh, struct l2cap_ctrl *control)
{
	control->reqseq = (enh & L2CAP_CTRL_REQSEQ) >> L2CAP_CTRL_REQSEQ_SHIFT;
	control->final = (enh & L2CAP_CTRL_FINAL) >> L2CAP_CTRL_FINAL_SHIFT;

	if (enh & L2CAP_CTRL_FRAME_TYPE) {
		/* S-Frame */
		control->sframe = 1;
		control->poll = (enh & L2CAP_CTRL_POLL) >> L2CAP_CTRL_POLL_SHIFT;
		control->super = (enh & L2CAP_CTRL_SUPERVISE) >> L2CAP_CTRL_SUPER_SHIFT;

		control->sar = 0;
		control->txseq = 0;
	} else {
		/* I-Frame */
		control->sframe = 0;
		control->sar = (enh & L2CAP_CTRL_SAR) >> L2CAP_CTRL_SAR_SHIFT;
		control->txseq = (enh & L2CAP_CTRL_TXSEQ) >> L2CAP_CTRL_TXSEQ_SHIFT;

		control->poll = 0;
		control->super = 0;
	}
}

static void __unpack_extended_control(u32 ext, struct l2cap_ctrl *control)
{
	control->reqseq = (ext & L2CAP_EXT_CTRL_REQSEQ) >> L2CAP_EXT_CTRL_REQSEQ_SHIFT;
	control->final = (ext & L2CAP_EXT_CTRL_FINAL) >> L2CAP_EXT_CTRL_FINAL_SHIFT;

	if (ext & L2CAP_EXT_CTRL_FRAME_TYPE) {
		/* S-Frame */
		control->sframe = 1;
		control->poll = (ext & L2CAP_EXT_CTRL_POLL) >> L2CAP_EXT_CTRL_POLL_SHIFT;
		control->super = (ext & L2CAP_EXT_CTRL_SUPERVISE) >> L2CAP_EXT_CTRL_SUPER_SHIFT;

		control->sar = 0;
		control->txseq = 0;
	} else {
		/* I-Frame */
		control->sframe = 0;
		control->sar = (ext & L2CAP_EXT_CTRL_SAR) >> L2CAP_EXT_CTRL_SAR_SHIFT;
		control->txseq = (ext & L2CAP_EXT_CTRL_TXSEQ) >> L2CAP_EXT_CTRL_TXSEQ_SHIFT;

		control->poll = 0;
		control->super = 0;
	}
}

static inline void __unpack_control(struct l2cap_chan *chan,
				    struct sk_buff *skb)
{
	if (test_bit(FLAG_EXT_CTRL, &chan->flags)) {
		__unpack_extended_control(get_unaligned_le32(skb->data),
					  &bt_cb(skb)->l2cap);
		skb_pull(skb, L2CAP_EXT_CTRL_SIZE);
	} else {
		__unpack_enhanced_control(get_unaligned_le16(skb->data),
					  &bt_cb(skb)->l2cap);
		skb_pull(skb, L2CAP_ENH_CTRL_SIZE);
	}
}

static u32 __pack_extended_control(struct l2cap_ctrl *control)
{
	u32 packed;

	packed = control->reqseq << L2CAP_EXT_CTRL_REQSEQ_SHIFT;
	packed |= control->final << L2CAP_EXT_CTRL_FINAL_SHIFT;

	if (control->sframe) {
		packed |= control->poll << L2CAP_EXT_CTRL_POLL_SHIFT;
		packed |= control->super << L2CAP_EXT_CTRL_SUPER_SHIFT;
		packed |= L2CAP_EXT_CTRL_FRAME_TYPE;
	} else {
		packed |= control->sar << L2CAP_EXT_CTRL_SAR_SHIFT;
		packed |= control->txseq << L2CAP_EXT_CTRL_TXSEQ_SHIFT;
	}

	return packed;
}

static u16 __pack_enhanced_control(struct l2cap_ctrl *control)
{
	u16 packed;

	packed = control->reqseq << L2CAP_CTRL_REQSEQ_SHIFT;
	packed |= control->final << L2CAP_CTRL_FINAL_SHIFT;

	if (control->sframe) {
		packed |= control->poll << L2CAP_CTRL_POLL_SHIFT;
		packed |= control->super << L2CAP_CTRL_SUPER_SHIFT;
		packed |= L2CAP_CTRL_FRAME_TYPE;
	} else {
		packed |= control->sar << L2CAP_CTRL_SAR_SHIFT;
		packed |= control->txseq << L2CAP_CTRL_TXSEQ_SHIFT;
	}

	return packed;
}

static inline void __pack_control(struct l2cap_chan *chan,
				  struct l2cap_ctrl *control,
				  struct sk_buff *skb)
{
	if (test_bit(FLAG_EXT_CTRL, &chan->flags)) {
		put_unaligned_le32(__pack_extended_control(control),
				   skb->data + L2CAP_HDR_SIZE);
	} else {
		put_unaligned_le16(__pack_enhanced_control(control),
				   skb->data + L2CAP_HDR_SIZE);
	}
}

static inline unsigned int __ertm_hdr_size(struct l2cap_chan *chan)
{
	if (test_bit(FLAG_EXT_CTRL, &chan->flags))
		return L2CAP_EXT_HDR_SIZE;
	else
		return L2CAP_ENH_HDR_SIZE;
}

static struct sk_buff *l2cap_create_sframe_pdu(struct l2cap_chan *chan,
					       u32 control)
{
	struct sk_buff *skb;
	struct l2cap_hdr *lh;
	int hlen = __ertm_hdr_size(chan);

	if (chan->fcs == L2CAP_FCS_CRC16)
		hlen += L2CAP_FCS_SIZE;

	skb = bt_skb_alloc(hlen, GFP_KERNEL);

	if (!skb)
		return ERR_PTR(-ENOMEM);

	lh = skb_put(skb, L2CAP_HDR_SIZE);
	lh->len = cpu_to_le16(hlen - L2CAP_HDR_SIZE);
	lh->cid = cpu_to_le16(chan->dcid);

	if (test_bit(FLAG_EXT_CTRL, &chan->flags))
		put_unaligned_le32(control, skb_put(skb, L2CAP_EXT_CTRL_SIZE));
	else
		put_unaligned_le16(control, skb_put(skb, L2CAP_ENH_CTRL_SIZE));

	if (chan->fcs == L2CAP_FCS_CRC16) {
		u16 fcs = crc16(0, (u8 *)skb->data, skb->len);
		put_unaligned_le16(fcs, skb_put(skb, L2CAP_FCS_SIZE));
	}

	skb->priority = HCI_PRIO_MAX;
	return skb;
}

static void l2cap_send_sframe(struct l2cap_chan *chan,
			      struct l2cap_ctrl *control)
{
	struct sk_buff *skb;
	u32 control_field;

	BT_DBG("chan %p, control %p", chan, control);

	if (!control->sframe)
		return;

	if (test_and_clear_bit(CONN_SEND_FBIT, &chan->conn_state) &&
	    !control->poll)
		control->final = 1;

	if (control->super == L2CAP_SUPER_RR)
		clear_bit(CONN_RNR_SENT, &chan->conn_state);
	else if (control->super == L2CAP_SUPER_RNR)
		set_bit(CONN_RNR_SENT, &chan->conn_state);

	if (control->super != L2CAP_SUPER_SREJ) {
		chan->last_acked_seq = control->reqseq;
		__clear_ack_timer(chan);
	}

	BT_DBG("reqseq %d, final %d, poll %d, super %d", control->reqseq,
	       control->final, control->poll, control->super);

	if (test_bit(FLAG_EXT_CTRL, &chan->flags))
		control_field = __pack_extended_control(control);
	else
		control_field = __pack_enhanced_control(control);

	skb = l2cap_create_sframe_pdu(chan, control_field);
	if (!IS_ERR(skb))
		l2cap_do_send(chan, skb);
}

static void l2cap_send_rr_or_rnr(struct l2cap_chan *chan, bool poll)
{
	struct l2cap_ctrl control;

	BT_DBG("chan %p, poll %d", chan, poll);

	memset(&control, 0, sizeof(control));
	control.sframe = 1;
	control.poll = poll;

	if (test_bit(CONN_LOCAL_BUSY, &chan->conn_state))
		control.super = L2CAP_SUPER_RNR;
	else
		control.super = L2CAP_SUPER_RR;

	control.reqseq = chan->buffer_seq;
	l2cap_send_sframe(chan, &control);
}

static inline int __l2cap_no_conn_pending(struct l2cap_chan *chan)
{
	if (chan->chan_type != L2CAP_CHAN_CONN_ORIENTED)
		return true;

	return !test_bit(CONF_CONNECT_PEND, &chan->conf_state);
}

void l2cap_send_conn_req(struct l2cap_chan *chan)
{
	struct l2cap_conn *conn = chan->conn;
	struct l2cap_conn_req req;

	req.scid = cpu_to_le16(chan->scid);
	req.psm  = chan->psm;

	chan->ident = l2cap_get_ident(conn);

	set_bit(CONF_CONNECT_PEND, &chan->conf_state);

	l2cap_send_cmd(conn, chan->ident, L2CAP_CONN_REQ, sizeof(req), &req);
}

static void l2cap_chan_ready(struct l2cap_chan *chan)
{
	/* The channel may have already been flagged as connected in
	 * case of receiving data before the L2CAP info req/rsp
	 * procedure is complete.
	 */
	if (chan->state == BT_CONNECTED)
		return;

	/* This clears all conf flags, including CONF_NOT_COMPLETE */
	chan->conf_state = 0;
	__clear_chan_timer(chan);

	switch (chan->mode) {
	case L2CAP_MODE_LE_FLOWCTL:
	case L2CAP_MODE_EXT_FLOWCTL:
		if (!chan->tx_credits)
			chan->ops->suspend(chan);
		break;
	}

	chan->state = BT_CONNECTED;

	chan->ops->ready(chan);
}

static void l2cap_le_connect(struct l2cap_chan *chan)
{
	struct l2cap_conn *conn = chan->conn;
	struct l2cap_le_conn_req req;

	if (test_and_set_bit(FLAG_LE_CONN_REQ_SENT, &chan->flags))
		return;

	if (!chan->imtu)
		chan->imtu = chan->conn->mtu;

	l2cap_le_flowctl_init(chan, 0);

	memset(&req, 0, sizeof(req));
	req.psm     = chan->psm;
	req.scid    = cpu_to_le16(chan->scid);
	req.mtu     = cpu_to_le16(chan->imtu);
	req.mps     = cpu_to_le16(chan->mps);
	req.credits = cpu_to_le16(chan->rx_credits);

	chan->ident = l2cap_get_ident(conn);

	l2cap_send_cmd(conn, chan->ident, L2CAP_LE_CONN_REQ,
		       sizeof(req), &req);
}

struct l2cap_ecred_conn_data {
	struct {
		struct l2cap_ecred_conn_req req;
		__le16 scid[5];
	} __packed pdu;
	struct l2cap_chan *chan;
	struct pid *pid;
	int count;
};

static void l2cap_ecred_defer_connect(struct l2cap_chan *chan, void *data)
{
	struct l2cap_ecred_conn_data *conn = data;
	struct pid *pid;

	if (chan == conn->chan)
		return;

	if (!test_and_clear_bit(FLAG_DEFER_SETUP, &chan->flags))
		return;

	pid = chan->ops->get_peer_pid(chan);

	/* Only add deferred channels with the same PID/PSM */
	if (conn->pid != pid || chan->psm != conn->chan->psm || chan->ident ||
	    chan->mode != L2CAP_MODE_EXT_FLOWCTL || chan->state != BT_CONNECT)
		return;

	if (test_and_set_bit(FLAG_ECRED_CONN_REQ_SENT, &chan->flags))
		return;

	l2cap_ecred_init(chan, 0);

	/* Set the same ident so we can match on the rsp */
	chan->ident = conn->chan->ident;

	/* Include all channels deferred */
	conn->pdu.scid[conn->count] = cpu_to_le16(chan->scid);

	conn->count++;
}

static void l2cap_ecred_connect(struct l2cap_chan *chan)
{
	struct l2cap_conn *conn = chan->conn;
	struct l2cap_ecred_conn_data data;

	if (test_bit(FLAG_DEFER_SETUP, &chan->flags))
		return;

	if (test_and_set_bit(FLAG_ECRED_CONN_REQ_SENT, &chan->flags))
		return;

	l2cap_ecred_init(chan, 0);

	memset(&data, 0, sizeof(data));
	data.pdu.req.psm     = chan->psm;
	data.pdu.req.mtu     = cpu_to_le16(chan->imtu);
	data.pdu.req.mps     = cpu_to_le16(chan->mps);
	data.pdu.req.credits = cpu_to_le16(chan->rx_credits);
	data.pdu.scid[0]     = cpu_to_le16(chan->scid);

	chan->ident = l2cap_get_ident(conn);

	data.count = 1;
	data.chan = chan;
	data.pid = chan->ops->get_peer_pid(chan);

	__l2cap_chan_list(conn, l2cap_ecred_defer_connect, &data);

	l2cap_send_cmd(conn, chan->ident, L2CAP_ECRED_CONN_REQ,
		       sizeof(data.pdu.req) + data.count * sizeof(__le16),
		       &data.pdu);
}

static void l2cap_le_start(struct l2cap_chan *chan)
{
	struct l2cap_conn *conn = chan->conn;

	if (!smp_conn_security(conn->hcon, chan->sec_level))
		return;

	if (!chan->psm) {
		l2cap_chan_ready(chan);
		return;
	}

	if (chan->state == BT_CONNECT) {
		if (chan->mode == L2CAP_MODE_EXT_FLOWCTL)
			l2cap_ecred_connect(chan);
		else
			l2cap_le_connect(chan);
	}
}

static void l2cap_start_connection(struct l2cap_chan *chan)
{
	if (chan->conn->hcon->type == LE_LINK) {
		l2cap_le_start(chan);
	} else {
		l2cap_send_conn_req(chan);
	}
}

static void l2cap_request_info(struct l2cap_conn *conn)
{
	struct l2cap_info_req req;

	if (conn->info_state & L2CAP_INFO_FEAT_MASK_REQ_SENT)
		return;

	req.type = cpu_to_le16(L2CAP_IT_FEAT_MASK);

	conn->info_state |= L2CAP_INFO_FEAT_MASK_REQ_SENT;
	conn->info_ident = l2cap_get_ident(conn);

	schedule_delayed_work(&conn->info_timer, L2CAP_INFO_TIMEOUT);

	l2cap_send_cmd(conn, conn->info_ident, L2CAP_INFO_REQ,
		       sizeof(req), &req);
}

static bool l2cap_check_enc_key_size(struct hci_conn *hcon)
{
	/* The minimum encryption key size needs to be enforced by the
	 * host stack before establishing any L2CAP connections. The
	 * specification in theory allows a minimum of 1, but to align
	 * BR/EDR and LE transports, a minimum of 7 is chosen.
	 *
	 * This check might also be called for unencrypted connections
	 * that have no key size requirements. Ensure that the link is
	 * actually encrypted before enforcing a key size.
	 */
	int min_key_size = hcon->hdev->min_enc_key_size;

	/* On FIPS security level, key size must be 16 bytes */
	if (hcon->sec_level == BT_SECURITY_FIPS)
		min_key_size = 16;

	return (!test_bit(HCI_CONN_ENCRYPT, &hcon->flags) ||
		hcon->enc_key_size >= min_key_size);
}

static void l2cap_do_start(struct l2cap_chan *chan)
{
	struct l2cap_conn *conn = chan->conn;

	if (conn->hcon->type == LE_LINK) {
		l2cap_le_start(chan);
		return;
	}

	if (!(conn->info_state & L2CAP_INFO_FEAT_MASK_REQ_SENT)) {
		l2cap_request_info(conn);
		return;
	}

	if (!(conn->info_state & L2CAP_INFO_FEAT_MASK_REQ_DONE))
		return;

	if (!l2cap_chan_check_security(chan, true) ||
	    !__l2cap_no_conn_pending(chan))
		return;

	if (l2cap_check_enc_key_size(conn->hcon))
		l2cap_start_connection(chan);
	else
		__set_chan_timer(chan, L2CAP_DISC_TIMEOUT);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline int l2cap_mode_supported(__u8 mode, __u32 feat_mask)
{
	u32 local_feat_mask = l2cap_feat_mask;
	if (!disable_ertm)
		local_feat_mask |= L2CAP_FEAT_ERTM | L2CAP_FEAT_STREAMING;

	switch (mode) {
	case L2CAP_MODE_ERTM:
		return L2CAP_FEAT_ERTM & feat_mask & local_feat_mask;
	case L2CAP_MODE_STREAMING:
		return L2CAP_FEAT_STREAMING & feat_mask & local_feat_mask;
	default:
		return 0x00;
	}
}

<<<<<<< HEAD
void l2cap_send_disconn_req(struct l2cap_conn *conn, struct sock *sk, int err)
{
=======
static void l2cap_send_disconn_req(struct l2cap_chan *chan, int err)
{
	struct l2cap_conn *conn = chan->conn;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct l2cap_disconn_req req;

	if (!conn)
		return;

<<<<<<< HEAD
	sk->sk_send_head = NULL;
	skb_queue_purge(TX_QUEUE(sk));

	if (l2cap_pi(sk)->mode == L2CAP_MODE_ERTM) {
		skb_queue_purge(SREJ_QUEUE(sk));

		__cancel_delayed_work(&l2cap_pi(sk)->ack_work);
		__cancel_delayed_work(&l2cap_pi(sk)->retrans_work);
		__cancel_delayed_work(&l2cap_pi(sk)->monitor_work);
	}

	req.dcid = cpu_to_le16(l2cap_pi(sk)->dcid);
	req.scid = cpu_to_le16(l2cap_pi(sk)->scid);
	l2cap_send_cmd(conn, l2cap_get_ident(conn),
			L2CAP_DISCONN_REQ, sizeof(req), &req);

	sk->sk_state = BT_DISCONN;
	sk->sk_err = err;
=======
	if (chan->mode == L2CAP_MODE_ERTM && chan->state == BT_CONNECTED) {
		__clear_retrans_timer(chan);
		__clear_monitor_timer(chan);
		__clear_ack_timer(chan);
	}

	req.dcid = cpu_to_le16(chan->dcid);
	req.scid = cpu_to_le16(chan->scid);
	l2cap_send_cmd(conn, l2cap_get_ident(conn), L2CAP_DISCONN_REQ,
		       sizeof(req), &req);

	l2cap_state_change_and_error(chan, BT_DISCONN, err);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* ---- L2CAP connections ---- */
static void l2cap_conn_start(struct l2cap_conn *conn)
{
<<<<<<< HEAD
	struct l2cap_chan_list *l = &conn->chan_list;
	struct sock_del_list del, *tmp1, *tmp2;
	struct sock *sk;

	BT_DBG("conn %p", conn);

	INIT_LIST_HEAD(&del.list);

	read_lock(&l->lock);

	for (sk = l->head; sk; sk = l2cap_pi(sk)->next_c) {
		bh_lock_sock(sk);

		if (sk->sk_type != SOCK_SEQPACKET &&
				sk->sk_type != SOCK_STREAM) {
			bh_unlock_sock(sk);
			continue;
		}

		if (sk->sk_state == BT_CONNECT) {
			if (!l2cap_check_security(sk) ||
					!__l2cap_no_conn_pending(sk)) {
				bh_unlock_sock(sk);
				continue;
			}

			if (!l2cap_mode_supported(l2cap_pi(sk)->mode,
					conn->feat_mask)
					&& l2cap_pi(sk)->conf_state &
					L2CAP_CONF_STATE2_DEVICE) {
				tmp1 = kzalloc(sizeof(struct sock_del_list),
						GFP_ATOMIC);
				tmp1->sk = sk;
				list_add_tail(&tmp1->list, &del.list);
				bh_unlock_sock(sk);
				continue;
			}

			l2cap_pi(sk)->conf_state |= L2CAP_CONF_CONNECT_PEND;

			if (l2cap_pi(sk)->amp_pref ==
					BT_AMP_POLICY_PREFER_AMP &&
					enable_hs &&
					conn->fc_mask & L2CAP_FC_A2MP)
				amp_create_physical(conn, sk);
			else
				l2cap_send_conn_req(sk);

		} else if (sk->sk_state == BT_CONNECT2) {
			struct l2cap_conn_rsp rsp;
			char buf[128];
			rsp.scid = cpu_to_le16(l2cap_pi(sk)->dcid);
			rsp.dcid = cpu_to_le16(l2cap_pi(sk)->scid);

			if (l2cap_check_security(sk)) {
				if (bt_sk(sk)->defer_setup) {
					struct sock *parent = bt_sk(sk)->parent;
					rsp.result = cpu_to_le16(L2CAP_CR_PEND);
					rsp.status = cpu_to_le16(L2CAP_CS_AUTHOR_PEND);
					if (parent)
						parent->sk_data_ready(parent, 0);

				} else {
					sk->sk_state = BT_CONFIG;
=======
	struct l2cap_chan *chan, *tmp;

	BT_DBG("conn %p", conn);

	mutex_lock(&conn->chan_lock);

	list_for_each_entry_safe(chan, tmp, &conn->chan_l, list) {
		l2cap_chan_lock(chan);

		if (chan->chan_type != L2CAP_CHAN_CONN_ORIENTED) {
			l2cap_chan_ready(chan);
			l2cap_chan_unlock(chan);
			continue;
		}

		if (chan->state == BT_CONNECT) {
			if (!l2cap_chan_check_security(chan, true) ||
			    !__l2cap_no_conn_pending(chan)) {
				l2cap_chan_unlock(chan);
				continue;
			}

			if (!l2cap_mode_supported(chan->mode, conn->feat_mask)
			    && test_bit(CONF_STATE2_DEVICE,
					&chan->conf_state)) {
				l2cap_chan_close(chan, ECONNRESET);
				l2cap_chan_unlock(chan);
				continue;
			}

			if (l2cap_check_enc_key_size(conn->hcon))
				l2cap_start_connection(chan);
			else
				l2cap_chan_close(chan, ECONNREFUSED);

		} else if (chan->state == BT_CONNECT2) {
			struct l2cap_conn_rsp rsp;
			char buf[128];
			rsp.scid = cpu_to_le16(chan->dcid);
			rsp.dcid = cpu_to_le16(chan->scid);

			if (l2cap_chan_check_security(chan, false)) {
				if (test_bit(FLAG_DEFER_SETUP, &chan->flags)) {
					rsp.result = cpu_to_le16(L2CAP_CR_PEND);
					rsp.status = cpu_to_le16(L2CAP_CS_AUTHOR_PEND);
					chan->ops->defer(chan);

				} else {
					l2cap_state_change(chan, BT_CONFIG);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					rsp.result = cpu_to_le16(L2CAP_CR_SUCCESS);
					rsp.status = cpu_to_le16(L2CAP_CS_NO_INFO);
				}
			} else {
				rsp.result = cpu_to_le16(L2CAP_CR_PEND);
				rsp.status = cpu_to_le16(L2CAP_CS_AUTHEN_PEND);
			}

<<<<<<< HEAD
			if (rsp.result == cpu_to_le16(L2CAP_CR_SUCCESS) &&
					l2cap_pi(sk)->amp_id) {
				amp_accept_physical(conn,
						l2cap_pi(sk)->amp_id, sk);
				bh_unlock_sock(sk);
				continue;
			}

			l2cap_send_cmd(conn, l2cap_pi(sk)->ident,
					L2CAP_CONN_RSP, sizeof(rsp), &rsp);

			if (l2cap_pi(sk)->conf_state & L2CAP_CONF_REQ_SENT ||
					rsp.result != L2CAP_CR_SUCCESS) {
				bh_unlock_sock(sk);
				continue;
			}

			l2cap_pi(sk)->conf_state |= L2CAP_CONF_REQ_SENT;
			l2cap_send_cmd(conn, l2cap_get_ident(conn), L2CAP_CONF_REQ,
						l2cap_build_conf_req(sk, buf, sizeof(buf)), buf);
			l2cap_pi(sk)->num_conf_req++;
		}

		bh_unlock_sock(sk);
	}

	read_unlock(&l->lock);

	list_for_each_entry_safe(tmp1, tmp2, &del.list, list) {
		bh_lock_sock(tmp1->sk);
		__l2cap_sock_close(tmp1->sk, ECONNRESET);
		bh_unlock_sock(tmp1->sk);
		list_del(&tmp1->list);
		kfree(tmp1);
	}
}

/* Find socket with fixed cid with given source and destination bdaddrs.
 * Direction of the req/rsp must match.
 */
struct sock *l2cap_find_sock_by_fixed_cid_and_dir(__le16 cid, bdaddr_t *src,
						bdaddr_t *dst, int incoming)
{
	struct sock *sk = NULL, *sk1 = NULL;
	struct hlist_node *node;

	BT_DBG(" %d", incoming);

	read_lock(&l2cap_sk_list.lock);

	sk_for_each(sk, node, &l2cap_sk_list.head) {

		if (incoming && !l2cap_pi(sk)->incoming)
			continue;

		if (!incoming && l2cap_pi(sk)->incoming)
			continue;

		if (l2cap_pi(sk)->scid == cid && !bacmp(&bt_sk(sk)->dst, dst)) {
			/* Exact match. */
			if (!bacmp(&bt_sk(sk)->src, src))
				break;

			/* Closest match */
			if (!bacmp(&bt_sk(sk)->src, BDADDR_ANY))
				sk1 = sk;
		}
	}

	read_unlock(&l2cap_sk_list.lock);

	return node ? sk : sk1;
}

/* Find socket with cid and source bdaddr.
 * Returns closest match, locked.
 */
static struct sock *l2cap_get_sock_by_scid(int state, __le16 cid, bdaddr_t *src)
{
	struct sock *sk = NULL, *sk1 = NULL;
	struct hlist_node *node;

	read_lock(&l2cap_sk_list.lock);

	sk_for_each(sk, node, &l2cap_sk_list.head) {
		if (state && sk->sk_state != state)
			continue;

		if (l2cap_pi(sk)->scid == cid) {
			/* Exact match. */
			if (!bacmp(&bt_sk(sk)->src, src))
				break;

			/* Closest match */
			if (!bacmp(&bt_sk(sk)->src, BDADDR_ANY))
				sk1 = sk;
		}
	}

	read_unlock(&l2cap_sk_list.lock);

	return node ? sk : sk1;
=======
			l2cap_send_cmd(conn, chan->ident, L2CAP_CONN_RSP,
				       sizeof(rsp), &rsp);

			if (test_bit(CONF_REQ_SENT, &chan->conf_state) ||
			    rsp.result != L2CAP_CR_SUCCESS) {
				l2cap_chan_unlock(chan);
				continue;
			}

			set_bit(CONF_REQ_SENT, &chan->conf_state);
			l2cap_send_cmd(conn, l2cap_get_ident(conn), L2CAP_CONF_REQ,
				       l2cap_build_conf_req(chan, buf, sizeof(buf)), buf);
			chan->num_conf_req++;
		}

		l2cap_chan_unlock(chan);
	}

	mutex_unlock(&conn->chan_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void l2cap_le_conn_ready(struct l2cap_conn *conn)
{
<<<<<<< HEAD
	struct l2cap_chan_list *list = &conn->chan_list;
	struct sock *parent, *uninitialized_var(sk);

	BT_DBG("");

	/* Check if we have socket listening on cid */
	parent = l2cap_get_sock_by_scid(BT_LISTEN, L2CAP_CID_LE_DATA,
							conn->src);
	if (!parent)
		return;

	bh_lock_sock(parent);

	/* Check for backlog size */
	if (sk_acceptq_is_full(parent)) {
		BT_DBG("backlog full %d", parent->sk_ack_backlog);
		goto clean;
	}

	sk = l2cap_sock_alloc(sock_net(parent), NULL, BTPROTO_L2CAP, GFP_ATOMIC);
	if (!sk)
		goto clean;

	write_lock_bh(&list->lock);

	hci_conn_hold(conn->hcon);

	l2cap_sock_init(sk, parent);
	bacpy(&bt_sk(sk)->src, conn->src);
	bacpy(&bt_sk(sk)->dst, conn->dst);
	l2cap_pi(sk)->incoming = 1;

	bt_accept_enqueue(parent, sk);

	__l2cap_chan_add(conn, sk);

	sk->sk_state = BT_CONNECTED;
	parent->sk_data_ready(parent, 0);

	write_unlock_bh(&list->lock);

clean:
	bh_unlock_sock(parent);
=======
	struct hci_conn *hcon = conn->hcon;
	struct hci_dev *hdev = hcon->hdev;

	BT_DBG("%s conn %p", hdev->name, conn);

	/* For outgoing pairing which doesn't necessarily have an
	 * associated socket (e.g. mgmt_pair_device).
	 */
	if (hcon->out)
		smp_conn_security(hcon, hcon->pending_sec_level);

	/* For LE peripheral connections, make sure the connection interval
	 * is in the range of the minimum and maximum interval that has
	 * been configured for this connection. If not, then trigger
	 * the connection update procedure.
	 */
	if (hcon->role == HCI_ROLE_SLAVE &&
	    (hcon->le_conn_interval < hcon->le_conn_min_interval ||
	     hcon->le_conn_interval > hcon->le_conn_max_interval)) {
		struct l2cap_conn_param_update_req req;

		req.min = cpu_to_le16(hcon->le_conn_min_interval);
		req.max = cpu_to_le16(hcon->le_conn_max_interval);
		req.latency = cpu_to_le16(hcon->le_conn_latency);
		req.to_multiplier = cpu_to_le16(hcon->le_supv_timeout);

		l2cap_send_cmd(conn, l2cap_get_ident(conn),
			       L2CAP_CONN_PARAM_UPDATE_REQ, sizeof(req), &req);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void l2cap_conn_ready(struct l2cap_conn *conn)
{
<<<<<<< HEAD
	struct l2cap_chan_list *l = &conn->chan_list;
	struct sock *sk;

	BT_DBG("conn %p", conn);

	if (!conn->hcon->out && conn->hcon->type == LE_LINK)
		l2cap_le_conn_ready(conn);

	read_lock(&l->lock);

	if (l->head) {
		for (sk = l->head; sk; sk = l2cap_pi(sk)->next_c) {
			bh_lock_sock(sk);

			if (conn->hcon->type == LE_LINK) {
				u8 sec_level = l2cap_pi(sk)->sec_level;
				u8 pending_sec = conn->hcon->pending_sec_level;

				if (pending_sec > sec_level)
					sec_level = pending_sec;

				if (smp_conn_security(conn, sec_level))
					l2cap_chan_ready(sk);

				hci_conn_put(conn->hcon);

			} else if (sk->sk_type != SOCK_SEQPACKET &&
					sk->sk_type != SOCK_STREAM) {
				l2cap_sock_clear_timer(sk);
				sk->sk_state = BT_CONNECTED;
				sk->sk_state_change(sk);
			} else if (sk->sk_state == BT_CONNECT)
				l2cap_do_start(sk);

			bh_unlock_sock(sk);
		}
	} else if (conn->hcon->type == LE_LINK) {
		smp_conn_security(conn, BT_SECURITY_HIGH);
	}

	read_unlock(&l->lock);

	if (conn->hcon->out && conn->hcon->type == LE_LINK)
		l2cap_le_conn_ready(conn);
=======
	struct l2cap_chan *chan;
	struct hci_conn *hcon = conn->hcon;

	BT_DBG("conn %p", conn);

	if (hcon->type == ACL_LINK)
		l2cap_request_info(conn);

	mutex_lock(&conn->chan_lock);

	list_for_each_entry(chan, &conn->chan_l, list) {

		l2cap_chan_lock(chan);

		if (hcon->type == LE_LINK) {
			l2cap_le_start(chan);
		} else if (chan->chan_type != L2CAP_CHAN_CONN_ORIENTED) {
			if (conn->info_state & L2CAP_INFO_FEAT_MASK_REQ_DONE)
				l2cap_chan_ready(chan);
		} else if (chan->state == BT_CONNECT) {
			l2cap_do_start(chan);
		}

		l2cap_chan_unlock(chan);
	}

	mutex_unlock(&conn->chan_lock);

	if (hcon->type == LE_LINK)
		l2cap_le_conn_ready(conn);

	queue_work(hcon->hdev->workqueue, &conn->pending_rx_work);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Notify sockets that we cannot guaranty reliability anymore */
static void l2cap_conn_unreliable(struct l2cap_conn *conn, int err)
{
<<<<<<< HEAD
	struct l2cap_chan_list *l = &conn->chan_list;
	struct sock *sk;

	BT_DBG("conn %p", conn);

	read_lock(&l->lock);

	for (sk = l->head; sk; sk = l2cap_pi(sk)->next_c) {
		if (l2cap_pi(sk)->force_reliable)
			sk->sk_err = err;
	}

	read_unlock(&l->lock);
}

static void l2cap_info_timeout(unsigned long arg)
{
	struct l2cap_conn *conn = (void *) arg;
=======
	struct l2cap_chan *chan;

	BT_DBG("conn %p", conn);

	mutex_lock(&conn->chan_lock);

	list_for_each_entry(chan, &conn->chan_l, list) {
		if (test_bit(FLAG_FORCE_RELIABLE, &chan->flags))
			l2cap_chan_set_err(chan, err);
	}

	mutex_unlock(&conn->chan_lock);
}

static void l2cap_info_timeout(struct work_struct *work)
{
	struct l2cap_conn *conn = container_of(work, struct l2cap_conn,
					       info_timer.work);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	conn->info_state |= L2CAP_INFO_FEAT_MASK_REQ_DONE;
	conn->info_ident = 0;

	l2cap_conn_start(conn);
}

<<<<<<< HEAD
static struct l2cap_conn *l2cap_conn_add(struct hci_conn *hcon, u8 status)
{
	struct l2cap_conn *conn = hcon->l2cap_data;

	if (conn || status)
		return conn;

	conn = kzalloc(sizeof(struct l2cap_conn), GFP_ATOMIC);
	if (!conn)
		return NULL;

	hcon->l2cap_data = conn;
	conn->hcon = hcon;

	BT_DBG("hcon %p conn %p", hcon, conn);

	if (hcon->hdev->le_mtu && hcon->type == LE_LINK)
		conn->mtu = hcon->hdev->le_mtu;
	else
		conn->mtu = hcon->hdev->acl_mtu;

	conn->src = &hcon->hdev->bdaddr;
	conn->dst = &hcon->dst;

	conn->feat_mask = 0;

	spin_lock_init(&conn->lock);
	rwlock_init(&conn->chan_list.lock);

	if (hcon->type == LE_LINK)
		setup_timer(&hcon->smp_timer, smp_timeout,
						(unsigned long) conn);
	else
		setup_timer(&conn->info_timer, l2cap_info_timeout,
						(unsigned long) conn);

	conn->disc_reason = 0x13;

	return conn;
}

static void l2cap_conn_del(struct hci_conn *hcon, int err, u8 is_process)
{
	struct l2cap_conn *conn = hcon->l2cap_data;
	struct sock *sk;
	struct sock *next;
=======
/*
 * l2cap_user
 * External modules can register l2cap_user objects on l2cap_conn. The ->probe
 * callback is called during registration. The ->remove callback is called
 * during unregistration.
 * An l2cap_user object can either be explicitly unregistered or when the
 * underlying l2cap_conn object is deleted. This guarantees that l2cap->hcon,
 * l2cap->hchan, .. are valid as long as the remove callback hasn't been called.
 * External modules must own a reference to the l2cap_conn object if they intend
 * to call l2cap_unregister_user(). The l2cap_conn object might get destroyed at
 * any time if they don't.
 */

int l2cap_register_user(struct l2cap_conn *conn, struct l2cap_user *user)
{
	struct hci_dev *hdev = conn->hcon->hdev;
	int ret;

	/* We need to check whether l2cap_conn is registered. If it is not, we
	 * must not register the l2cap_user. l2cap_conn_del() is unregisters
	 * l2cap_conn objects, but doesn't provide its own locking. Instead, it
	 * relies on the parent hci_conn object to be locked. This itself relies
	 * on the hci_dev object to be locked. So we must lock the hci device
	 * here, too. */

	hci_dev_lock(hdev);

	if (!list_empty(&user->list)) {
		ret = -EINVAL;
		goto out_unlock;
	}

	/* conn->hchan is NULL after l2cap_conn_del() was called */
	if (!conn->hchan) {
		ret = -ENODEV;
		goto out_unlock;
	}

	ret = user->probe(conn, user);
	if (ret)
		goto out_unlock;

	list_add(&user->list, &conn->users);
	ret = 0;

out_unlock:
	hci_dev_unlock(hdev);
	return ret;
}
EXPORT_SYMBOL(l2cap_register_user);

void l2cap_unregister_user(struct l2cap_conn *conn, struct l2cap_user *user)
{
	struct hci_dev *hdev = conn->hcon->hdev;

	hci_dev_lock(hdev);

	if (list_empty(&user->list))
		goto out_unlock;

	list_del_init(&user->list);
	user->remove(conn, user);

out_unlock:
	hci_dev_unlock(hdev);
}
EXPORT_SYMBOL(l2cap_unregister_user);

static void l2cap_unregister_all_users(struct l2cap_conn *conn)
{
	struct l2cap_user *user;

	while (!list_empty(&conn->users)) {
		user = list_first_entry(&conn->users, struct l2cap_user, list);
		list_del_init(&user->list);
		user->remove(conn, user);
	}
}

static void l2cap_conn_del(struct hci_conn *hcon, int err)
{
	struct l2cap_conn *conn = hcon->l2cap_data;
	struct l2cap_chan *chan, *l;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!conn)
		return;

	BT_DBG("hcon %p conn %p, err %d", hcon, conn, err);

<<<<<<< HEAD
	if ((conn->hcon == hcon) && (conn->rx_skb))
		kfree_skb(conn->rx_skb);

	BT_DBG("conn->hcon %p", conn->hcon);

	/* Kill channels */
	for (sk = conn->chan_list.head; sk; ) {
		BT_DBG("ampcon %p", l2cap_pi(sk)->ampcon);
		if ((conn->hcon == hcon) || (l2cap_pi(sk)->ampcon == hcon)) {
			next = l2cap_pi(sk)->next_c;
			if (is_process)
				lock_sock(sk);
			else
				bh_lock_sock(sk);
			l2cap_chan_del(sk, err);
			if (is_process)
				release_sock(sk);
			else
				bh_unlock_sock(sk);
			l2cap_sock_kill(sk);
			sk = next;
		} else
			sk = l2cap_pi(sk)->next_c;
	}

	if (conn->hcon == hcon) {
		if (conn->info_state & L2CAP_INFO_FEAT_MASK_REQ_SENT)
			del_timer_sync(&conn->info_timer);

		hcon->l2cap_data = NULL;

		kfree(conn);
	}
	att_chn_params.conn = NULL;
	BT_DBG("att_chn_params.conn set to NULL");
}

static inline void l2cap_chan_add(struct l2cap_conn *conn, struct sock *sk)
{
	struct l2cap_chan_list *l = &conn->chan_list;
	write_lock_bh(&l->lock);
	__l2cap_chan_add(conn, sk);
	write_unlock_bh(&l->lock);
}

/* ---- Socket interface ---- */

/* Find socket with psm and source bdaddr.
 * Returns closest match.
 */
static struct sock *l2cap_get_sock_by_psm(int state, __le16 psm, bdaddr_t *src)
{
	struct sock *sk = NULL, *sk1 = NULL;
	struct hlist_node *node;

	read_lock(&l2cap_sk_list.lock);

	sk_for_each(sk, node, &l2cap_sk_list.head) {
		if (state && sk->sk_state != state)
			continue;

		if (l2cap_pi(sk)->psm == psm) {
			/* Exact match. */
			if (!bacmp(&bt_sk(sk)->src, src))
				break;

			/* Closest match */
			if (!bacmp(&bt_sk(sk)->src, BDADDR_ANY))
				sk1 = sk;
		}
	}

	read_unlock(&l2cap_sk_list.lock);

	return node ? sk : sk1;
}

int l2cap_do_connect(struct sock *sk)
{
	bdaddr_t *src = &bt_sk(sk)->src;
	bdaddr_t *dst = &bt_sk(sk)->dst;
	struct l2cap_conn *conn;
	struct hci_conn *hcon;
	struct hci_dev *hdev;
	__u8 auth_type;
	int err;

	BT_DBG("%s -> %s psm 0x%2.2x", batostr(src), batostr(dst),
							l2cap_pi(sk)->psm);

	hdev = hci_get_route(dst, src);
	if (!hdev)
		return -EHOSTUNREACH;

	hci_dev_lock_bh(hdev);

	auth_type = l2cap_get_auth_type(sk);

	if (l2cap_pi(sk)->fixed_channel) {
		/* Fixed channels piggyback on existing ACL connections */
		hcon = hci_conn_hash_lookup_ba(hdev, ACL_LINK, dst);
		if (!hcon || !hcon->l2cap_data) {
			err = -ENOTCONN;
			goto done;
		}

		conn = hcon->l2cap_data;
	} else {
		if (l2cap_pi(sk)->dcid == L2CAP_CID_LE_DATA)
			hcon = hci_le_connect(hdev, 0, dst,
					l2cap_pi(sk)->sec_level, auth_type,
					&bt_sk(sk)->le_params);
		else
			hcon = hci_connect(hdev, ACL_LINK, 0, dst,
					l2cap_pi(sk)->sec_level, auth_type);

		if (IS_ERR(hcon)) {
			err = PTR_ERR(hcon);
			goto done;
		}

		conn = l2cap_conn_add(hcon, 0);
		if (!conn) {
			hci_conn_put(hcon);
			err = -ENOMEM;
			goto done;
		}
	}

	/* Update source addr of the socket */
	bacpy(src, conn->src);

	l2cap_chan_add(conn, sk);

	if ((l2cap_pi(sk)->fixed_channel) ||
			(l2cap_pi(sk)->dcid == L2CAP_CID_LE_DATA &&
				hcon->state == BT_CONNECTED)) {
		sk->sk_state = BT_CONNECTED;
		sk->sk_state_change(sk);
	} else {
		sk->sk_state = BT_CONNECT;
		/* If we have valid LE Params, let timeout override default */
		if (l2cap_pi(sk)->dcid == L2CAP_CID_LE_DATA &&
			l2cap_sock_le_params_valid(&bt_sk(sk)->le_params)) {
			u16 timeout = bt_sk(sk)->le_params.conn_timeout;

			if (timeout)
				l2cap_sock_set_timer(sk,
						msecs_to_jiffies(timeout*1000));
		} else
			l2cap_sock_set_timer(sk, sk->sk_sndtimeo);

		sk->sk_state_change(sk);

		if (hcon->state == BT_CONNECTED) {
			if (sk->sk_type != SOCK_SEQPACKET &&
				sk->sk_type != SOCK_STREAM) {
				l2cap_sock_clear_timer(sk);
				if (l2cap_check_security(sk)) {
					sk->sk_state = BT_CONNECTED;
					sk->sk_state_change(sk);
				}
			} else
				l2cap_do_start(sk);
		}
	}

	err = 0;

done:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);
	return err;
}

int __l2cap_wait_ack(struct sock *sk)
{
	DECLARE_WAITQUEUE(wait, current);
	int err = 0;
	int timeo = HZ/5;

	add_wait_queue(sk_sleep(sk), &wait);
	while (l2cap_pi(sk)->unacked_frames > 0 && l2cap_pi(sk)->conn &&
		atomic_read(&l2cap_pi(sk)->ertm_queued)) {
		set_current_state(TASK_INTERRUPTIBLE);

		if (!timeo)
			timeo = HZ/5;

		if (signal_pending(current)) {
			err = sock_intr_errno(timeo);
			break;
		}

		release_sock(sk);
		timeo = schedule_timeout(timeo);
		lock_sock(sk);

		err = sock_error(sk);
		if (err)
			break;
	}
	set_current_state(TASK_RUNNING);
	remove_wait_queue(sk_sleep(sk), &wait);
	return err;
}

static void l2cap_ertm_tx_worker(struct work_struct *work)
{
	struct l2cap_pinfo *pi =
		container_of(work, struct l2cap_pinfo, tx_work);
	struct sock *sk = (struct sock *)pi;
	BT_DBG("%p", pi);

	lock_sock(sk);
	l2cap_ertm_send(sk);
	release_sock(sk);
	sock_put(sk);
}

static void l2cap_skb_destructor(struct sk_buff *skb)
{
	struct sock *sk = skb->sk;
	int queued;
	int keep_sk = 0;

	queued = atomic_sub_return(1, &l2cap_pi(sk)->ertm_queued);
	if (queued < L2CAP_MIN_ERTM_QUEUED)
		keep_sk = queue_work(_l2cap_wq, &l2cap_pi(sk)->tx_work);

	if (!keep_sk)
		sock_put(sk);
}

void l2cap_do_send(struct sock *sk, struct sk_buff *skb)
{
	struct l2cap_pinfo *pi = l2cap_pi(sk);

	BT_DBG("sk %p, skb %p len %d", sk, skb, skb->len);

	if (pi->ampcon && (pi->amp_move_state == L2CAP_AMP_STATE_STABLE ||
			pi->amp_move_state == L2CAP_AMP_STATE_WAIT_PREPARE)) {
		BT_DBG("Sending on AMP connection %p %p",
			pi->ampcon, pi->ampchan);
		if (pi->ampchan)
			hci_send_acl(pi->ampcon, pi->ampchan, skb,
					ACL_COMPLETE);
		else
			kfree_skb(skb);
	} else {
		u16 flags;

		if (!(pi->conn)) {
			kfree_skb(skb);
			return;
		}

		bt_cb(skb)->force_active = pi->force_active;
		BT_DBG("Sending on BR/EDR connection %p", pi->conn->hcon);

		if (lmp_no_flush_capable(pi->conn->hcon->hdev) &&
			!l2cap_pi(sk)->flushable)
			flags = ACL_START_NO_FLUSH;
		else
			flags = ACL_START;

		hci_send_acl(pi->conn->hcon, NULL, skb, flags);
	}
}

int l2cap_ertm_send(struct sock *sk)
{
	struct sk_buff *skb, *tx_skb;
	struct l2cap_pinfo *pi = l2cap_pi(sk);
	struct bt_l2cap_control *control;
	int sent = 0;

	BT_DBG("sk %p", sk);

	if (sk->sk_state != BT_CONNECTED)
		return -ENOTCONN;

	if (pi->conn_state & L2CAP_CONN_REMOTE_BUSY)
		return 0;

	if (pi->amp_move_state != L2CAP_AMP_STATE_STABLE &&
			pi->amp_move_state != L2CAP_AMP_STATE_WAIT_PREPARE)
		return 0;

	while (sk->sk_send_head && (pi->unacked_frames < pi->remote_tx_win) &&
		atomic_read(&pi->ertm_queued) < L2CAP_MAX_ERTM_QUEUED &&
		(pi->tx_state == L2CAP_ERTM_TX_STATE_XMIT)) {

		skb = sk->sk_send_head;

		bt_cb(skb)->retries = 1;
		control = &bt_cb(skb)->control;

		if (pi->conn_state & L2CAP_CONN_SEND_FBIT) {
			control->final = 1;
			pi->conn_state &= ~L2CAP_CONN_SEND_FBIT;
		}
		control->reqseq = pi->buffer_seq;
		pi->last_acked_seq = pi->buffer_seq;
		control->txseq = pi->next_tx_seq;

		if (pi->extended_control) {
			put_unaligned_le32(__pack_extended_control(control),
					skb->data + L2CAP_HDR_SIZE);
		} else {
			put_unaligned_le16(__pack_enhanced_control(control),
					skb->data + L2CAP_HDR_SIZE);
		}

		if (pi->fcs == L2CAP_FCS_CRC16)
			apply_fcs(skb);
=======
	kfree_skb(conn->rx_skb);

	skb_queue_purge(&conn->pending_rx);

	/* We can not call flush_work(&conn->pending_rx_work) here since we
	 * might block if we are running on a worker from the same workqueue
	 * pending_rx_work is waiting on.
	 */
	if (work_pending(&conn->pending_rx_work))
		cancel_work_sync(&conn->pending_rx_work);

	cancel_delayed_work_sync(&conn->id_addr_timer);

	l2cap_unregister_all_users(conn);

	/* Force the connection to be immediately dropped */
	hcon->disc_timeout = 0;

	mutex_lock(&conn->chan_lock);

	/* Kill channels */
	list_for_each_entry_safe(chan, l, &conn->chan_l, list) {
		l2cap_chan_hold(chan);
		l2cap_chan_lock(chan);

		l2cap_chan_del(chan, err);

		chan->ops->close(chan);

		l2cap_chan_unlock(chan);
		l2cap_chan_put(chan);
	}

	mutex_unlock(&conn->chan_lock);

	hci_chan_del(conn->hchan);

	if (conn->info_state & L2CAP_INFO_FEAT_MASK_REQ_SENT)
		cancel_delayed_work_sync(&conn->info_timer);

	hcon->l2cap_data = NULL;
	conn->hchan = NULL;
	l2cap_conn_put(conn);
}

static void l2cap_conn_free(struct kref *ref)
{
	struct l2cap_conn *conn = container_of(ref, struct l2cap_conn, ref);

	hci_conn_put(conn->hcon);
	kfree(conn);
}

struct l2cap_conn *l2cap_conn_get(struct l2cap_conn *conn)
{
	kref_get(&conn->ref);
	return conn;
}
EXPORT_SYMBOL(l2cap_conn_get);

void l2cap_conn_put(struct l2cap_conn *conn)
{
	kref_put(&conn->ref, l2cap_conn_free);
}
EXPORT_SYMBOL(l2cap_conn_put);

/* ---- Socket interface ---- */

/* Find socket with psm and source / destination bdaddr.
 * Returns closest match.
 */
static struct l2cap_chan *l2cap_global_chan_by_psm(int state, __le16 psm,
						   bdaddr_t *src,
						   bdaddr_t *dst,
						   u8 link_type)
{
	struct l2cap_chan *c, *tmp, *c1 = NULL;

	read_lock(&chan_list_lock);

	list_for_each_entry_safe(c, tmp, &chan_list, global_l) {
		if (state && c->state != state)
			continue;

		if (link_type == ACL_LINK && c->src_type != BDADDR_BREDR)
			continue;

		if (link_type == LE_LINK && c->src_type == BDADDR_BREDR)
			continue;

		if (c->chan_type != L2CAP_CHAN_FIXED && c->psm == psm) {
			int src_match, dst_match;
			int src_any, dst_any;

			/* Exact match. */
			src_match = !bacmp(&c->src, src);
			dst_match = !bacmp(&c->dst, dst);
			if (src_match && dst_match) {
				if (!l2cap_chan_hold_unless_zero(c))
					continue;

				read_unlock(&chan_list_lock);
				return c;
			}

			/* Closest match */
			src_any = !bacmp(&c->src, BDADDR_ANY);
			dst_any = !bacmp(&c->dst, BDADDR_ANY);
			if ((src_match && dst_any) || (src_any && dst_match) ||
			    (src_any && dst_any))
				c1 = c;
		}
	}

	if (c1)
		c1 = l2cap_chan_hold_unless_zero(c1);

	read_unlock(&chan_list_lock);

	return c1;
}

static void l2cap_monitor_timeout(struct work_struct *work)
{
	struct l2cap_chan *chan = container_of(work, struct l2cap_chan,
					       monitor_timer.work);

	BT_DBG("chan %p", chan);

	l2cap_chan_lock(chan);

	if (!chan->conn) {
		l2cap_chan_unlock(chan);
		l2cap_chan_put(chan);
		return;
	}

	l2cap_tx(chan, NULL, NULL, L2CAP_EV_MONITOR_TO);

	l2cap_chan_unlock(chan);
	l2cap_chan_put(chan);
}

static void l2cap_retrans_timeout(struct work_struct *work)
{
	struct l2cap_chan *chan = container_of(work, struct l2cap_chan,
					       retrans_timer.work);

	BT_DBG("chan %p", chan);

	l2cap_chan_lock(chan);

	if (!chan->conn) {
		l2cap_chan_unlock(chan);
		l2cap_chan_put(chan);
		return;
	}

	l2cap_tx(chan, NULL, NULL, L2CAP_EV_RETRANS_TO);
	l2cap_chan_unlock(chan);
	l2cap_chan_put(chan);
}

static void l2cap_streaming_send(struct l2cap_chan *chan,
				 struct sk_buff_head *skbs)
{
	struct sk_buff *skb;
	struct l2cap_ctrl *control;

	BT_DBG("chan %p, skbs %p", chan, skbs);

	skb_queue_splice_tail_init(skbs, &chan->tx_q);

	while (!skb_queue_empty(&chan->tx_q)) {

		skb = skb_dequeue(&chan->tx_q);

		bt_cb(skb)->l2cap.retries = 1;
		control = &bt_cb(skb)->l2cap;

		control->reqseq = 0;
		control->txseq = chan->next_tx_seq;

		__pack_control(chan, control, skb);

		if (chan->fcs == L2CAP_FCS_CRC16) {
			u16 fcs = crc16(0, (u8 *) skb->data, skb->len);
			put_unaligned_le16(fcs, skb_put(skb, L2CAP_FCS_SIZE));
		}

		l2cap_do_send(chan, skb);

		BT_DBG("Sent txseq %u", control->txseq);

		chan->next_tx_seq = __next_seq(chan, chan->next_tx_seq);
		chan->frames_sent++;
	}
}

static int l2cap_ertm_send(struct l2cap_chan *chan)
{
	struct sk_buff *skb, *tx_skb;
	struct l2cap_ctrl *control;
	int sent = 0;

	BT_DBG("chan %p", chan);

	if (chan->state != BT_CONNECTED)
		return -ENOTCONN;

	if (test_bit(CONN_REMOTE_BUSY, &chan->conn_state))
		return 0;

	while (chan->tx_send_head &&
	       chan->unacked_frames < chan->remote_tx_win &&
	       chan->tx_state == L2CAP_TX_STATE_XMIT) {

		skb = chan->tx_send_head;

		bt_cb(skb)->l2cap.retries = 1;
		control = &bt_cb(skb)->l2cap;

		if (test_and_clear_bit(CONN_SEND_FBIT, &chan->conn_state))
			control->final = 1;

		control->reqseq = chan->buffer_seq;
		chan->last_acked_seq = chan->buffer_seq;
		control->txseq = chan->next_tx_seq;

		__pack_control(chan, control, skb);

		if (chan->fcs == L2CAP_FCS_CRC16) {
			u16 fcs = crc16(0, (u8 *) skb->data, skb->len);
			put_unaligned_le16(fcs, skb_put(skb, L2CAP_FCS_SIZE));
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/* Clone after data has been modified. Data is assumed to be
		   read-only (for locking purposes) on cloned sk_buffs.
		 */
<<<<<<< HEAD
		tx_skb = skb_clone(skb, GFP_ATOMIC);
=======
		tx_skb = skb_clone(skb, GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (!tx_skb)
			break;

<<<<<<< HEAD
		sock_hold(sk);
		tx_skb->sk = sk;
		tx_skb->destructor = l2cap_skb_destructor;
		atomic_inc(&pi->ertm_queued);

		l2cap_ertm_start_retrans_timer(pi);

		pi->next_tx_seq = __next_seq(pi->next_tx_seq, pi);
		pi->unacked_frames += 1;
		pi->frames_sent += 1;
		sent += 1;

		if (skb_queue_is_last(TX_QUEUE(sk), skb))
			sk->sk_send_head = NULL;
		else
			sk->sk_send_head = skb_queue_next(TX_QUEUE(sk), skb);

		l2cap_do_send(sk, tx_skb);
		BT_DBG("Sent txseq %d", (int)control->txseq);
	}

	BT_DBG("Sent %d, %d unacked, %d in ERTM queue, %d in HCI queue", sent,
		(int) pi->unacked_frames, skb_queue_len(TX_QUEUE(sk)),
		atomic_read(&pi->ertm_queued));
=======
		__set_retrans_timer(chan);

		chan->next_tx_seq = __next_seq(chan, chan->next_tx_seq);
		chan->unacked_frames++;
		chan->frames_sent++;
		sent++;

		if (skb_queue_is_last(&chan->tx_q, skb))
			chan->tx_send_head = NULL;
		else
			chan->tx_send_head = skb_queue_next(&chan->tx_q, skb);

		l2cap_do_send(chan, tx_skb);
		BT_DBG("Sent txseq %u", control->txseq);
	}

	BT_DBG("Sent %d, %u unacked, %u in ERTM queue", sent,
	       chan->unacked_frames, skb_queue_len(&chan->tx_q));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return sent;
}

<<<<<<< HEAD
int l2cap_strm_tx(struct sock *sk, struct sk_buff_head *skbs)
{
	struct sk_buff *skb;
	struct l2cap_pinfo *pi = l2cap_pi(sk);
	struct bt_l2cap_control *control;
	int sent = 0;

	BT_DBG("sk %p, skbs %p", sk, skbs);

	if (sk->sk_state != BT_CONNECTED)
		return -ENOTCONN;

	if (pi->amp_move_state != L2CAP_AMP_STATE_STABLE &&
			pi->amp_move_state != L2CAP_AMP_STATE_WAIT_PREPARE)
		return 0;

	skb_queue_splice_tail_init(skbs, TX_QUEUE(sk));

	BT_DBG("skb queue empty 0x%2.2x", skb_queue_empty(TX_QUEUE(sk)));
	while (!skb_queue_empty(TX_QUEUE(sk))) {

		skb = skb_dequeue(TX_QUEUE(sk));

		BT_DBG("skb %p", skb);

		bt_cb(skb)->retries = 1;
		control = &bt_cb(skb)->control;

		BT_DBG("control %p", control);

		control->reqseq = 0;
		control->txseq = pi->next_tx_seq;

		if (pi->extended_control) {
			put_unaligned_le32(__pack_extended_control(control),
					skb->data + L2CAP_HDR_SIZE);
		} else {
			put_unaligned_le16(__pack_enhanced_control(control),
					skb->data + L2CAP_HDR_SIZE);
		}

		if (pi->fcs == L2CAP_FCS_CRC16)
			apply_fcs(skb);

		l2cap_do_send(sk, skb);

		BT_DBG("Sent txseq %d", (int)control->txseq);

		pi->next_tx_seq = __next_seq(pi->next_tx_seq, pi);
		pi->frames_sent += 1;
		sent += 1;
	}

	BT_DBG("Sent %d", sent);

	return 0;
}

static int memcpy_fromkvec(unsigned char *kdata, struct kvec *iv, int len)
{
	while (len > 0) {
		if (iv->iov_len) {
			int copy = min_t(unsigned int, len, iv->iov_len);
			memcpy(kdata, iv->iov_base, copy);
			len -= copy;
			kdata += copy;
			iv->iov_base += copy;
			iv->iov_len -= copy;
		}
		iv++;
	}

	return 0;
}

static inline int l2cap_skbuff_fromiovec(struct sock *sk, struct msghdr *msg,
					int len, int count, struct sk_buff *skb,
					int reseg)
{
	struct l2cap_conn *conn = l2cap_pi(sk)->conn;
	struct sk_buff **frag;
	struct sk_buff *final;
	int err, sent = 0;

	BT_DBG("sk %p, msg %p, len %d, count %d, skb %p", sk,
		msg, (int)len, (int)count, skb);

	if (!conn)
		return -ENOTCONN;

	/* When resegmenting, data is copied from kernel space */
	if (reseg) {
		err = memcpy_fromkvec(skb_put(skb, count),
				(struct kvec *) msg->msg_iov, count);
	} else {
		err = memcpy_fromiovec(skb_put(skb, count), msg->msg_iov,
					count);
	}

	if (err)
		return -EFAULT;

	sent += count;
	len  -= count;
	final = skb;

	/* Continuation fragments (no L2CAP header) */
	frag = &skb_shinfo(skb)->frag_list;
	while (len) {
		int skblen;
		count = min_t(unsigned int, conn->mtu, len);

		/* Add room for the FCS if it fits */
		if (bt_cb(skb)->control.fcs == L2CAP_FCS_CRC16 &&
			len + L2CAP_FCS_SIZE <= conn->mtu)
			skblen = count + L2CAP_FCS_SIZE;
		else
			skblen = count;

		/* Don't use bt_skb_send_alloc() while resegmenting, since
		 * it is not ok to block.
		 */
		if (reseg) {
			*frag = bt_skb_alloc(skblen, GFP_ATOMIC);
			if (*frag)
				skb_set_owner_w(*frag, sk);
		} else {
			*frag = bt_skb_send_alloc(sk, skblen,
					msg->msg_flags & MSG_DONTWAIT, &err);
		}

		if (!*frag)
			return -EFAULT;

		/* When resegmenting, data is copied from kernel space */
		if (reseg) {
			err = memcpy_fromkvec(skb_put(*frag, count),
						(struct kvec *) msg->msg_iov,
						count);
		} else {
			err = memcpy_fromiovec(skb_put(*frag, count),
						msg->msg_iov, count);
		}

		if (err)
			return -EFAULT;

		sent += count;
		len  -= count;

		final = *frag;

		frag = &(*frag)->next;
	}

	if (bt_cb(skb)->control.fcs == L2CAP_FCS_CRC16) {
		if (skb_tailroom(final) < L2CAP_FCS_SIZE) {
			if (reseg) {
				*frag = bt_skb_alloc(L2CAP_FCS_SIZE,
						GFP_ATOMIC);
				if (*frag)
					skb_set_owner_w(*frag, sk);
			} else {
				*frag = bt_skb_send_alloc(sk, L2CAP_FCS_SIZE,
						msg->msg_flags & MSG_DONTWAIT,
						&err);
			}

			if (!*frag)
				return -EFAULT;

			final = *frag;
		}

		skb_put(final, L2CAP_FCS_SIZE);
	}

	return sent;
}

struct sk_buff *l2cap_create_connless_pdu(struct sock *sk, struct msghdr *msg, size_t len)
{
	struct l2cap_conn *conn = l2cap_pi(sk)->conn;
	struct sk_buff *skb;
	int err, count, hlen = L2CAP_HDR_SIZE + 2;
	struct l2cap_hdr *lh;

	BT_DBG("sk %p len %d", sk, (int)len);

	count = min_t(unsigned int, (conn->mtu - hlen), len);
	skb = bt_skb_send_alloc(sk, count + hlen,
			msg->msg_flags & MSG_DONTWAIT, &err);
	if (!skb)
		return ERR_PTR(err);

	/* Create L2CAP header */
	lh = (struct l2cap_hdr *) skb_put(skb, L2CAP_HDR_SIZE);
	lh->cid = cpu_to_le16(l2cap_pi(sk)->dcid);
	lh->len = cpu_to_le16(len + (hlen - L2CAP_HDR_SIZE));
	put_unaligned_le16(l2cap_pi(sk)->psm, skb_put(skb, 2));

	err = l2cap_skbuff_fromiovec(sk, msg, len, count, skb, 0);
	if (unlikely(err < 0)) {
		kfree_skb(skb);
		return ERR_PTR(err);
	}
	return skb;
}

struct sk_buff *l2cap_create_basic_pdu(struct sock *sk, struct msghdr *msg, size_t len)
{
	struct l2cap_conn *conn = l2cap_pi(sk)->conn;
	struct sk_buff *skb;
	int err, count, hlen = L2CAP_HDR_SIZE;
	struct l2cap_hdr *lh;

	BT_DBG("sk %p len %d", sk, (int)len);

	count = min_t(unsigned int, (conn->mtu - hlen), len);
	skb = bt_skb_send_alloc(sk, count + hlen,
			msg->msg_flags & MSG_DONTWAIT, &err);
	if (!skb)
		return ERR_PTR(err);

	/* Create L2CAP header */
	lh = (struct l2cap_hdr *) skb_put(skb, L2CAP_HDR_SIZE);
	lh->cid = cpu_to_le16(l2cap_pi(sk)->dcid);
	lh->len = cpu_to_le16(len + (hlen - L2CAP_HDR_SIZE));

	err = l2cap_skbuff_fromiovec(sk, msg, len, count, skb, 0);
	if (unlikely(err < 0)) {
		kfree_skb(skb);
		return ERR_PTR(err);
	}
	return skb;
}

struct sk_buff *l2cap_create_iframe_pdu(struct sock *sk,
					struct msghdr *msg, size_t len,
					u16 sdulen, int reseg)
{
	struct sk_buff *skb;
	int err, count, hlen;
	int reserve = 0;
	struct l2cap_hdr *lh;
	u8 fcs = l2cap_pi(sk)->fcs;

	if (l2cap_pi(sk)->extended_control)
		hlen = L2CAP_EXTENDED_HDR_SIZE;
	else
		hlen = L2CAP_ENHANCED_HDR_SIZE;

	if (sdulen)
		hlen += L2CAP_SDULEN_SIZE;

	if (fcs == L2CAP_FCS_CRC16)
		hlen += L2CAP_FCS_SIZE;

	BT_DBG("sk %p, msg %p, len %d, sdulen %d, hlen %d",
		sk, msg, (int)len, (int)sdulen, hlen);

	count = min_t(unsigned int, (l2cap_pi(sk)->conn->mtu - hlen), len);

	/* Allocate extra headroom for Qualcomm PAL.  This is only
	 * necessary in two places (here and when creating sframes)
	 * because only unfragmented iframes and sframes are sent
	 * using AMP controllers.
	 */
	if (l2cap_pi(sk)->ampcon &&
			l2cap_pi(sk)->ampcon->hdev->manufacturer == 0x001d)
		reserve = BT_SKB_RESERVE_80211;

	/* Don't use bt_skb_send_alloc() while resegmenting, since
	 * it is not ok to block.
	 */
	if (reseg) {
		skb = bt_skb_alloc(count + hlen + reserve, GFP_ATOMIC);
		if (skb)
			skb_set_owner_w(skb, sk);
	} else {
		skb = bt_skb_send_alloc(sk, count + hlen + reserve,
					msg->msg_flags & MSG_DONTWAIT, &err);
	}
	if (!skb)
		return ERR_PTR(err);

	if (reserve)
		skb_reserve(skb, reserve);

	bt_cb(skb)->control.fcs = fcs;

	/* Create L2CAP header */
	lh = (struct l2cap_hdr *) skb_put(skb, L2CAP_HDR_SIZE);
	lh->cid = cpu_to_le16(l2cap_pi(sk)->dcid);
	lh->len = cpu_to_le16(len + hlen - L2CAP_HDR_SIZE);

	/* Control header is populated later */
	if (l2cap_pi(sk)->extended_control)
		put_unaligned_le32(0, skb_put(skb, 4));
	else
		put_unaligned_le16(0, skb_put(skb, 2));

	if (sdulen)
		put_unaligned_le16(sdulen, skb_put(skb, L2CAP_SDULEN_SIZE));

	err = l2cap_skbuff_fromiovec(sk, msg, len, count, skb, reseg);
	if (unlikely(err < 0)) {
		BT_DBG("err %d", err);
		kfree_skb(skb);
		return ERR_PTR(err);
	}

	bt_cb(skb)->retries = 0;
	return skb;
}

static void l2cap_ertm_process_reqseq(struct sock *sk, u16 reqseq)
{
	struct l2cap_pinfo *pi;
	struct sk_buff *acked_skb;
	u16 ackseq;

	BT_DBG("sk %p, reqseq %d", sk, (int) reqseq);

	pi = l2cap_pi(sk);

	if (pi->unacked_frames == 0 || reqseq == pi->expected_ack_seq)
		return;

	BT_DBG("expected_ack_seq %d, unacked_frames %d",
		(int) pi->expected_ack_seq, (int) pi->unacked_frames);

	for (ackseq = pi->expected_ack_seq; ackseq != reqseq;
		ackseq = __next_seq(ackseq, pi)) {

		acked_skb = l2cap_ertm_seq_in_queue(TX_QUEUE(sk), ackseq);
		if (acked_skb) {
			skb_unlink(acked_skb, TX_QUEUE(sk));
			kfree_skb(acked_skb);
			pi->unacked_frames--;
		}
	}

	pi->expected_ack_seq = reqseq;

	if (pi->unacked_frames == 0)
		l2cap_ertm_stop_retrans_timer(pi);

	BT_DBG("unacked_frames %d", (int) pi->unacked_frames);
}

static struct sk_buff *l2cap_create_sframe_pdu(struct sock *sk, u32 control)
{
	struct sk_buff *skb;
	int len;
	int reserve = 0;
	struct l2cap_hdr *lh;

	if (l2cap_pi(sk)->extended_control)
		len = L2CAP_EXTENDED_HDR_SIZE;
	else
		len = L2CAP_ENHANCED_HDR_SIZE;

	if (l2cap_pi(sk)->fcs == L2CAP_FCS_CRC16)
		len += L2CAP_FCS_SIZE;

	/* Allocate extra headroom for Qualcomm PAL */
	if (l2cap_pi(sk)->ampcon &&
			l2cap_pi(sk)->ampcon->hdev->manufacturer == 0x001d)
		reserve = BT_SKB_RESERVE_80211;

	skb = bt_skb_alloc(len + reserve, GFP_ATOMIC);

	if (!skb)
		return ERR_PTR(-ENOMEM);

	if (reserve)
		skb_reserve(skb, reserve);

	lh = (struct l2cap_hdr *) skb_put(skb, L2CAP_HDR_SIZE);
	lh->cid = cpu_to_le16(l2cap_pi(sk)->dcid);
	lh->len = cpu_to_le16(len - L2CAP_HDR_SIZE);

	if (l2cap_pi(sk)->extended_control)
		put_unaligned_le32(control, skb_put(skb, 4));
	else
		put_unaligned_le16(control, skb_put(skb, 2));

	if (l2cap_pi(sk)->fcs == L2CAP_FCS_CRC16) {
		u16 fcs = crc16(0, (u8 *) skb->data, skb->len);
		put_unaligned_le16(fcs, skb_put(skb, L2CAP_FCS_SIZE));
	}

	return skb;
}

static void l2cap_ertm_send_sframe(struct sock *sk,
				struct bt_l2cap_control *control)
{
	struct l2cap_pinfo *pi;
	struct sk_buff *skb;
	u32 control_field;

	BT_DBG("sk %p, control %p", sk, control);

	if (control->frame_type != 's')
		return;

	pi = l2cap_pi(sk);

	if (pi->amp_move_state != L2CAP_AMP_STATE_STABLE &&
		pi->amp_move_state != L2CAP_AMP_STATE_WAIT_PREPARE &&
		pi->amp_move_state != L2CAP_AMP_STATE_RESEGMENT) {
		BT_DBG("AMP error - attempted S-Frame send during AMP move");
		return;
	}

	if ((pi->conn_state & L2CAP_CONN_SEND_FBIT) && !control->poll) {
		control->final = 1;
		pi->conn_state &= ~L2CAP_CONN_SEND_FBIT;
	}

	if (control->super == L2CAP_SFRAME_RR)
		pi->conn_state &= ~L2CAP_CONN_SENT_RNR;
	else if (control->super == L2CAP_SFRAME_RNR)
		pi->conn_state |= L2CAP_CONN_SENT_RNR;

	if (control->super != L2CAP_SFRAME_SREJ) {
		pi->last_acked_seq = control->reqseq;
		l2cap_ertm_stop_ack_timer(pi);
	}

	BT_DBG("reqseq %d, final %d, poll %d, super %d", (int) control->reqseq,
		(int) control->final, (int) control->poll,
		(int) control->super);

	if (pi->extended_control)
		control_field = __pack_extended_control(control);
	else
		control_field = __pack_enhanced_control(control);

	skb = l2cap_create_sframe_pdu(sk, control_field);
	if (!IS_ERR(skb))
		l2cap_do_send(sk, skb);
}

static void l2cap_ertm_send_ack(struct sock *sk)
{
	struct l2cap_pinfo *pi = l2cap_pi(sk);
	struct bt_l2cap_control control;
	u16 frames_to_ack = __delta_seq(pi->buffer_seq, pi->last_acked_seq, pi);
	int threshold;

	BT_DBG("sk %p", sk);
	BT_DBG("last_acked_seq %d, buffer_seq %d", (int)pi->last_acked_seq,
		(int)pi->buffer_seq);

	memset(&control, 0, sizeof(control));
	control.frame_type = 's';

	if ((pi->conn_state & L2CAP_CONN_LOCAL_BUSY) &&
		pi->rx_state == L2CAP_ERTM_RX_STATE_RECV) {
		l2cap_ertm_stop_ack_timer(pi);
		control.super = L2CAP_SFRAME_RNR;
		control.reqseq = pi->buffer_seq;
		l2cap_ertm_send_sframe(sk, &control);
	} else {
		if (!(pi->conn_state & L2CAP_CONN_REMOTE_BUSY)) {
			l2cap_ertm_send(sk);
			/* If any i-frames were sent, they included an ack */
			if (pi->buffer_seq == pi->last_acked_seq)
=======
static void l2cap_ertm_resend(struct l2cap_chan *chan)
{
	struct l2cap_ctrl control;
	struct sk_buff *skb;
	struct sk_buff *tx_skb;
	u16 seq;

	BT_DBG("chan %p", chan);

	if (test_bit(CONN_REMOTE_BUSY, &chan->conn_state))
		return;

	while (chan->retrans_list.head != L2CAP_SEQ_LIST_CLEAR) {
		seq = l2cap_seq_list_pop(&chan->retrans_list);

		skb = l2cap_ertm_seq_in_queue(&chan->tx_q, seq);
		if (!skb) {
			BT_DBG("Error: Can't retransmit seq %d, frame missing",
			       seq);
			continue;
		}

		bt_cb(skb)->l2cap.retries++;
		control = bt_cb(skb)->l2cap;

		if (chan->max_tx != 0 &&
		    bt_cb(skb)->l2cap.retries > chan->max_tx) {
			BT_DBG("Retry limit exceeded (%d)", chan->max_tx);
			l2cap_send_disconn_req(chan, ECONNRESET);
			l2cap_seq_list_clear(&chan->retrans_list);
			break;
		}

		control.reqseq = chan->buffer_seq;
		if (test_and_clear_bit(CONN_SEND_FBIT, &chan->conn_state))
			control.final = 1;
		else
			control.final = 0;

		if (skb_cloned(skb)) {
			/* Cloned sk_buffs are read-only, so we need a
			 * writeable copy
			 */
			tx_skb = skb_copy(skb, GFP_KERNEL);
		} else {
			tx_skb = skb_clone(skb, GFP_KERNEL);
		}

		if (!tx_skb) {
			l2cap_seq_list_clear(&chan->retrans_list);
			break;
		}

		/* Update skb contents */
		if (test_bit(FLAG_EXT_CTRL, &chan->flags)) {
			put_unaligned_le32(__pack_extended_control(&control),
					   tx_skb->data + L2CAP_HDR_SIZE);
		} else {
			put_unaligned_le16(__pack_enhanced_control(&control),
					   tx_skb->data + L2CAP_HDR_SIZE);
		}

		/* Update FCS */
		if (chan->fcs == L2CAP_FCS_CRC16) {
			u16 fcs = crc16(0, (u8 *) tx_skb->data,
					tx_skb->len - L2CAP_FCS_SIZE);
			put_unaligned_le16(fcs, skb_tail_pointer(tx_skb) -
						L2CAP_FCS_SIZE);
		}

		l2cap_do_send(chan, tx_skb);

		BT_DBG("Resent txseq %d", control.txseq);

		chan->last_acked_seq = chan->buffer_seq;
	}
}

static void l2cap_retransmit(struct l2cap_chan *chan,
			     struct l2cap_ctrl *control)
{
	BT_DBG("chan %p, control %p", chan, control);

	l2cap_seq_list_append(&chan->retrans_list, control->reqseq);
	l2cap_ertm_resend(chan);
}

static void l2cap_retransmit_all(struct l2cap_chan *chan,
				 struct l2cap_ctrl *control)
{
	struct sk_buff *skb;

	BT_DBG("chan %p, control %p", chan, control);

	if (control->poll)
		set_bit(CONN_SEND_FBIT, &chan->conn_state);

	l2cap_seq_list_clear(&chan->retrans_list);

	if (test_bit(CONN_REMOTE_BUSY, &chan->conn_state))
		return;

	if (chan->unacked_frames) {
		skb_queue_walk(&chan->tx_q, skb) {
			if (bt_cb(skb)->l2cap.txseq == control->reqseq ||
			    skb == chan->tx_send_head)
				break;
		}

		skb_queue_walk_from(&chan->tx_q, skb) {
			if (skb == chan->tx_send_head)
				break;

			l2cap_seq_list_append(&chan->retrans_list,
					      bt_cb(skb)->l2cap.txseq);
		}

		l2cap_ertm_resend(chan);
	}
}

static void l2cap_send_ack(struct l2cap_chan *chan)
{
	struct l2cap_ctrl control;
	u16 frames_to_ack = __seq_offset(chan, chan->buffer_seq,
					 chan->last_acked_seq);
	int threshold;

	BT_DBG("chan %p last_acked_seq %d buffer_seq %d",
	       chan, chan->last_acked_seq, chan->buffer_seq);

	memset(&control, 0, sizeof(control));
	control.sframe = 1;

	if (test_bit(CONN_LOCAL_BUSY, &chan->conn_state) &&
	    chan->rx_state == L2CAP_RX_STATE_RECV) {
		__clear_ack_timer(chan);
		control.super = L2CAP_SUPER_RNR;
		control.reqseq = chan->buffer_seq;
		l2cap_send_sframe(chan, &control);
	} else {
		if (!test_bit(CONN_REMOTE_BUSY, &chan->conn_state)) {
			l2cap_ertm_send(chan);
			/* If any i-frames were sent, they included an ack */
			if (chan->buffer_seq == chan->last_acked_seq)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				frames_to_ack = 0;
		}

		/* Ack now if the window is 3/4ths full.
		 * Calculate without mul or div
		 */
<<<<<<< HEAD
		threshold = pi->ack_win;
		threshold += threshold << 1;
		threshold >>= 2;

		BT_DBG("frames_to_ack %d, threshold %d", (int)frames_to_ack,
			threshold);

		if (frames_to_ack >= threshold) {
			l2cap_ertm_stop_ack_timer(pi);
			control.super = L2CAP_SFRAME_RR;
			control.reqseq = pi->buffer_seq;
			l2cap_ertm_send_sframe(sk, &control);
=======
		threshold = chan->ack_win;
		threshold += threshold << 1;
		threshold >>= 2;

		BT_DBG("frames_to_ack %u, threshold %d", frames_to_ack,
		       threshold);

		if (frames_to_ack >= threshold) {
			__clear_ack_timer(chan);
			control.super = L2CAP_SUPER_RR;
			control.reqseq = chan->buffer_seq;
			l2cap_send_sframe(chan, &control);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			frames_to_ack = 0;
		}

		if (frames_to_ack)
<<<<<<< HEAD
			l2cap_ertm_start_ack_timer(pi);
	}
}

static void l2cap_ertm_send_rr_or_rnr(struct sock *sk, bool poll)
{
	struct l2cap_pinfo *pi;
	struct bt_l2cap_control control;

	BT_DBG("sk %p, poll %d", sk, (int) poll);

	pi = l2cap_pi(sk);

	memset(&control, 0, sizeof(control));
	control.frame_type = 's';
	control.poll = poll;

	if (pi->conn_state & L2CAP_CONN_LOCAL_BUSY)
		control.super = L2CAP_SFRAME_RNR;
	else
		control.super = L2CAP_SFRAME_RR;

	control.reqseq = pi->buffer_seq;
	l2cap_ertm_send_sframe(sk, &control);
}

static void l2cap_ertm_send_i_or_rr_or_rnr(struct sock *sk)
{
	struct l2cap_pinfo *pi;
	struct bt_l2cap_control control;

	BT_DBG("sk %p", sk);

	pi = l2cap_pi(sk);

	memset(&control, 0, sizeof(control));
	control.frame_type = 's';
	control.final = 1;
	control.reqseq = pi->buffer_seq;
	pi->conn_state |= L2CAP_CONN_SEND_FBIT;

	if (pi->conn_state & L2CAP_CONN_LOCAL_BUSY) {
		control.super = L2CAP_SFRAME_RNR;
		l2cap_ertm_send_sframe(sk, &control);
	}

	if ((pi->conn_state & L2CAP_CONN_REMOTE_BUSY) &&
		(pi->unacked_frames > 0))
		l2cap_ertm_start_retrans_timer(pi);

	pi->conn_state &= ~L2CAP_CONN_REMOTE_BUSY;

	/* Send pending iframes */
	l2cap_ertm_send(sk);

	if (pi->conn_state & L2CAP_CONN_SEND_FBIT) {
		/* F-bit wasn't sent in an s-frame or i-frame yet, so
		 * send it now.
		 */
		control.super = L2CAP_SFRAME_RR;
		l2cap_ertm_send_sframe(sk, &control);
	}
}

static void l2cap_ertm_send_srej(struct sock *sk, u16 txseq)
{
	struct bt_l2cap_control control;
	struct l2cap_pinfo *pi;
	u16 seq;

	BT_DBG("sk %p, txseq %d", sk, (int)txseq);

	pi = l2cap_pi(sk);
	memset(&control, 0, sizeof(control));
	control.frame_type = 's';
	control.super = L2CAP_SFRAME_SREJ;

	for (seq = pi->expected_tx_seq; seq != txseq;
		seq = __next_seq(seq, pi)) {
		if (!l2cap_ertm_seq_in_queue(SREJ_QUEUE(pi), seq)) {
			control.reqseq = seq;
			l2cap_ertm_send_sframe(sk, &control);
			l2cap_seq_list_append(&pi->srej_list, seq);
		}
	}

	pi->expected_tx_seq = __next_seq(txseq, pi);
}

static void l2cap_ertm_send_srej_tail(struct sock *sk)
{
	struct bt_l2cap_control control;
	struct l2cap_pinfo *pi;

	BT_DBG("sk %p", sk);

	pi = l2cap_pi(sk);

	if (pi->srej_list.tail == L2CAP_SEQ_LIST_CLEAR)
		return;

	memset(&control, 0, sizeof(control));
	control.frame_type = 's';
	control.super = L2CAP_SFRAME_SREJ;
	control.reqseq = pi->srej_list.tail;
	l2cap_ertm_send_sframe(sk, &control);
}

static void l2cap_ertm_send_srej_list(struct sock *sk, u16 txseq)
{
	struct bt_l2cap_control control;
	struct l2cap_pinfo *pi;
	u16 initial_head;
	u16 seq;

	BT_DBG("sk %p, txseq %d", sk, (int) txseq);

	pi = l2cap_pi(sk);
	memset(&control, 0, sizeof(control));
	control.frame_type = 's';
	control.super = L2CAP_SFRAME_SREJ;

	/* Capture initial list head to allow only one pass through the list. */
	initial_head = pi->srej_list.head;

	do {
		seq = l2cap_seq_list_pop(&pi->srej_list);
		if ((seq == txseq) || (seq == L2CAP_SEQ_LIST_CLEAR))
			break;

		control.reqseq = seq;
		l2cap_ertm_send_sframe(sk, &control);
		l2cap_seq_list_append(&pi->srej_list, seq);
	} while (pi->srej_list.head != initial_head);
}

static void l2cap_ertm_abort_rx_srej_sent(struct sock *sk)
{
	struct l2cap_pinfo *pi = l2cap_pi(sk);
	BT_DBG("sk %p", sk);

	pi->expected_tx_seq = pi->buffer_seq;
	l2cap_seq_list_clear(&l2cap_pi(sk)->srej_list);
	skb_queue_purge(SREJ_QUEUE(sk));
	pi->rx_state = L2CAP_ERTM_RX_STATE_RECV;
}

static int l2cap_ertm_tx_state_xmit(struct sock *sk,
				struct bt_l2cap_control *control,
				struct sk_buff_head *skbs, u8 event)
{
	struct l2cap_pinfo *pi;
	int err = 0;

	BT_DBG("sk %p, control %p, skbs %p, event %d", sk, control, skbs,
		(int)event);
	pi = l2cap_pi(sk);

	switch (event) {
	case L2CAP_ERTM_EVENT_DATA_REQUEST:
		if (sk->sk_send_head == NULL)
			sk->sk_send_head = skb_peek(skbs);

		skb_queue_splice_tail_init(skbs, TX_QUEUE(sk));
		l2cap_ertm_send(sk);
		break;
	case L2CAP_ERTM_EVENT_LOCAL_BUSY_DETECTED:
		BT_DBG("Enter LOCAL_BUSY");
		pi->conn_state |= L2CAP_CONN_LOCAL_BUSY;

		if (pi->rx_state == L2CAP_ERTM_RX_STATE_SREJ_SENT) {
			/* The SREJ_SENT state must be aborted if we are to
			 * enter the LOCAL_BUSY state.
			 */
			l2cap_ertm_abort_rx_srej_sent(sk);
		}

		l2cap_ertm_send_ack(sk);

		break;
	case L2CAP_ERTM_EVENT_LOCAL_BUSY_CLEAR:
		BT_DBG("Exit LOCAL_BUSY");
		pi->conn_state &= ~L2CAP_CONN_LOCAL_BUSY;

		if (pi->amp_move_state == L2CAP_AMP_STATE_WAIT_LOCAL_BUSY) {
			if (pi->amp_move_role == L2CAP_AMP_MOVE_INITIATOR) {
				pi->amp_move_state =
					L2CAP_AMP_STATE_WAIT_MOVE_CONFIRM_RSP;
				l2cap_send_move_chan_cfm(pi->conn, pi,
						pi->scid,
						L2CAP_MOVE_CHAN_CONFIRMED);
				l2cap_sock_set_timer(sk, L2CAP_MOVE_TIMEOUT);
			} else if (pi->amp_move_role ==
					L2CAP_AMP_MOVE_RESPONDER) {
				pi->amp_move_state =
					L2CAP_AMP_STATE_WAIT_MOVE_CONFIRM;
				l2cap_send_move_chan_rsp(pi->conn,
						pi->amp_move_cmd_ident,
						pi->dcid,
						L2CAP_MOVE_CHAN_SUCCESS);
			}
			break;
		}

		if (pi->amp_move_role == L2CAP_AMP_MOVE_NONE &&
			(pi->conn_state & L2CAP_CONN_SENT_RNR)) {
			struct bt_l2cap_control local_control;

			memset(&local_control, 0, sizeof(local_control));
			local_control.frame_type = 's';
			local_control.super = L2CAP_SFRAME_RR;
			local_control.poll = 1;
			local_control.reqseq = pi->buffer_seq;
			l2cap_ertm_send_sframe(sk, &local_control);

			pi->retry_count = 1;
			l2cap_ertm_start_monitor_timer(pi);
			pi->tx_state = L2CAP_ERTM_TX_STATE_WAIT_F;
		}
		break;
	case L2CAP_ERTM_EVENT_RECV_REQSEQ_AND_FBIT:
		l2cap_ertm_process_reqseq(sk, control->reqseq);
		break;
	case L2CAP_ERTM_EVENT_EXPLICIT_POLL:
		l2cap_ertm_send_rr_or_rnr(sk, 1);
		pi->retry_count = 1;
		l2cap_ertm_start_monitor_timer(pi);
		l2cap_ertm_stop_ack_timer(pi);
		pi->tx_state = L2CAP_ERTM_TX_STATE_WAIT_F;
		break;
	case L2CAP_ERTM_EVENT_RETRANS_TIMER_EXPIRES:
		l2cap_ertm_send_rr_or_rnr(sk, 1);
		pi->retry_count = 1;
		l2cap_ertm_start_monitor_timer(pi);
		pi->tx_state = L2CAP_ERTM_TX_STATE_WAIT_F;
		break;
	case L2CAP_ERTM_EVENT_RECV_FBIT:
		/* Nothing to process */
		break;
	default:
		break;
	}

	return err;
}

static int l2cap_ertm_tx_state_wait_f(struct sock *sk,
				struct bt_l2cap_control *control,
				struct sk_buff_head *skbs, u8 event)
{
	struct l2cap_pinfo *pi;
	int err = 0;

	BT_DBG("sk %p, control %p, skbs %p, event %d", sk, control, skbs,
		(int)event);
	pi = l2cap_pi(sk);

	switch (event) {
	case L2CAP_ERTM_EVENT_DATA_REQUEST:
		if (sk->sk_send_head == NULL)
			sk->sk_send_head = skb_peek(skbs);
		/* Queue data, but don't send. */
		skb_queue_splice_tail_init(skbs, TX_QUEUE(sk));
		break;
	case L2CAP_ERTM_EVENT_LOCAL_BUSY_DETECTED:
		BT_DBG("Enter LOCAL_BUSY");
		pi->conn_state |= L2CAP_CONN_LOCAL_BUSY;

		if (pi->rx_state == L2CAP_ERTM_RX_STATE_SREJ_SENT) {
			/* The SREJ_SENT state must be aborted if we are to
			 * enter the LOCAL_BUSY state.
			 */
			l2cap_ertm_abort_rx_srej_sent(sk);
		}

		l2cap_ertm_send_ack(sk);

		break;
	case L2CAP_ERTM_EVENT_LOCAL_BUSY_CLEAR:
		BT_DBG("Exit LOCAL_BUSY");
		pi->conn_state &= ~L2CAP_CONN_LOCAL_BUSY;

		if (pi->conn_state & L2CAP_CONN_SENT_RNR) {
			struct bt_l2cap_control local_control;
			memset(&local_control, 0, sizeof(local_control));
			local_control.frame_type = 's';
			local_control.super = L2CAP_SFRAME_RR;
			local_control.poll = 1;
			local_control.reqseq = pi->buffer_seq;
			l2cap_ertm_send_sframe(sk, &local_control);

			pi->retry_count = 1;
			l2cap_ertm_start_monitor_timer(pi);
			pi->tx_state = L2CAP_ERTM_TX_STATE_WAIT_F;
		}
		break;
	case L2CAP_ERTM_EVENT_RECV_REQSEQ_AND_FBIT:
		l2cap_ertm_process_reqseq(sk, control->reqseq);

		/* Fall through */

	case L2CAP_ERTM_EVENT_RECV_FBIT:
		if (control && control->final) {
			l2cap_ertm_stop_monitor_timer(pi);
			if (pi->unacked_frames > 0)
				l2cap_ertm_start_retrans_timer(pi);
			pi->retry_count = 0;
			pi->tx_state = L2CAP_ERTM_TX_STATE_XMIT;
			BT_DBG("recv fbit tx_state 0x2.2%x", pi->tx_state);
		}
		break;
	case L2CAP_ERTM_EVENT_EXPLICIT_POLL:
		/* Ignore */
		break;
	case L2CAP_ERTM_EVENT_MONITOR_TIMER_EXPIRES:
		if ((pi->max_tx == 0) || (pi->retry_count < pi->max_tx)) {
			l2cap_ertm_send_rr_or_rnr(sk, 1);
			l2cap_ertm_start_monitor_timer(pi);
			pi->retry_count += 1;
		} else
			l2cap_send_disconn_req(pi->conn, sk, ECONNABORTED);
		break;
	default:
		break;
	}

	return err;
}

int l2cap_ertm_tx(struct sock *sk, struct bt_l2cap_control *control,
			struct sk_buff_head *skbs, u8 event)
{
	struct l2cap_pinfo *pi;
	int err = 0;

	BT_DBG("sk %p, control %p, skbs %p, event %d, state %d",
		sk, control, skbs, (int)event, l2cap_pi(sk)->tx_state);

	pi = l2cap_pi(sk);

	switch (pi->tx_state) {
	case L2CAP_ERTM_TX_STATE_XMIT:
		err = l2cap_ertm_tx_state_xmit(sk, control, skbs, event);
		break;
	case L2CAP_ERTM_TX_STATE_WAIT_F:
		err = l2cap_ertm_tx_state_wait_f(sk, control, skbs, event);
		break;
	default:
		/* Ignore event */
		break;
	}

	return err;
}

int l2cap_segment_sdu(struct sock *sk, struct sk_buff_head* seg_queue,
			struct msghdr *msg, size_t len, int reseg)
=======
			__set_ack_timer(chan);
	}
}

static inline int l2cap_skbuff_fromiovec(struct l2cap_chan *chan,
					 struct msghdr *msg, int len,
					 int count, struct sk_buff *skb)
{
	struct l2cap_conn *conn = chan->conn;
	struct sk_buff **frag;
	int sent = 0;

	if (!copy_from_iter_full(skb_put(skb, count), count, &msg->msg_iter))
		return -EFAULT;

	sent += count;
	len  -= count;

	/* Continuation fragments (no L2CAP header) */
	frag = &skb_shinfo(skb)->frag_list;
	while (len) {
		struct sk_buff *tmp;

		count = min_t(unsigned int, conn->mtu, len);

		tmp = chan->ops->alloc_skb(chan, 0, count,
					   msg->msg_flags & MSG_DONTWAIT);
		if (IS_ERR(tmp))
			return PTR_ERR(tmp);

		*frag = tmp;

		if (!copy_from_iter_full(skb_put(*frag, count), count,
				   &msg->msg_iter))
			return -EFAULT;

		sent += count;
		len  -= count;

		skb->len += (*frag)->len;
		skb->data_len += (*frag)->len;

		frag = &(*frag)->next;
	}

	return sent;
}

static struct sk_buff *l2cap_create_connless_pdu(struct l2cap_chan *chan,
						 struct msghdr *msg, size_t len)
{
	struct l2cap_conn *conn = chan->conn;
	struct sk_buff *skb;
	int err, count, hlen = L2CAP_HDR_SIZE + L2CAP_PSMLEN_SIZE;
	struct l2cap_hdr *lh;

	BT_DBG("chan %p psm 0x%2.2x len %zu", chan,
	       __le16_to_cpu(chan->psm), len);

	count = min_t(unsigned int, (conn->mtu - hlen), len);

	skb = chan->ops->alloc_skb(chan, hlen, count,
				   msg->msg_flags & MSG_DONTWAIT);
	if (IS_ERR(skb))
		return skb;

	/* Create L2CAP header */
	lh = skb_put(skb, L2CAP_HDR_SIZE);
	lh->cid = cpu_to_le16(chan->dcid);
	lh->len = cpu_to_le16(len + L2CAP_PSMLEN_SIZE);
	put_unaligned(chan->psm, (__le16 *) skb_put(skb, L2CAP_PSMLEN_SIZE));

	err = l2cap_skbuff_fromiovec(chan, msg, len, count, skb);
	if (unlikely(err < 0)) {
		kfree_skb(skb);
		return ERR_PTR(err);
	}
	return skb;
}

static struct sk_buff *l2cap_create_basic_pdu(struct l2cap_chan *chan,
					      struct msghdr *msg, size_t len)
{
	struct l2cap_conn *conn = chan->conn;
	struct sk_buff *skb;
	int err, count;
	struct l2cap_hdr *lh;

	BT_DBG("chan %p len %zu", chan, len);

	count = min_t(unsigned int, (conn->mtu - L2CAP_HDR_SIZE), len);

	skb = chan->ops->alloc_skb(chan, L2CAP_HDR_SIZE, count,
				   msg->msg_flags & MSG_DONTWAIT);
	if (IS_ERR(skb))
		return skb;

	/* Create L2CAP header */
	lh = skb_put(skb, L2CAP_HDR_SIZE);
	lh->cid = cpu_to_le16(chan->dcid);
	lh->len = cpu_to_le16(len);

	err = l2cap_skbuff_fromiovec(chan, msg, len, count, skb);
	if (unlikely(err < 0)) {
		kfree_skb(skb);
		return ERR_PTR(err);
	}
	return skb;
}

static struct sk_buff *l2cap_create_iframe_pdu(struct l2cap_chan *chan,
					       struct msghdr *msg, size_t len,
					       u16 sdulen)
{
	struct l2cap_conn *conn = chan->conn;
	struct sk_buff *skb;
	int err, count, hlen;
	struct l2cap_hdr *lh;

	BT_DBG("chan %p len %zu", chan, len);

	if (!conn)
		return ERR_PTR(-ENOTCONN);

	hlen = __ertm_hdr_size(chan);

	if (sdulen)
		hlen += L2CAP_SDULEN_SIZE;

	if (chan->fcs == L2CAP_FCS_CRC16)
		hlen += L2CAP_FCS_SIZE;

	count = min_t(unsigned int, (conn->mtu - hlen), len);

	skb = chan->ops->alloc_skb(chan, hlen, count,
				   msg->msg_flags & MSG_DONTWAIT);
	if (IS_ERR(skb))
		return skb;

	/* Create L2CAP header */
	lh = skb_put(skb, L2CAP_HDR_SIZE);
	lh->cid = cpu_to_le16(chan->dcid);
	lh->len = cpu_to_le16(len + (hlen - L2CAP_HDR_SIZE));

	/* Control header is populated later */
	if (test_bit(FLAG_EXT_CTRL, &chan->flags))
		put_unaligned_le32(0, skb_put(skb, L2CAP_EXT_CTRL_SIZE));
	else
		put_unaligned_le16(0, skb_put(skb, L2CAP_ENH_CTRL_SIZE));

	if (sdulen)
		put_unaligned_le16(sdulen, skb_put(skb, L2CAP_SDULEN_SIZE));

	err = l2cap_skbuff_fromiovec(chan, msg, len, count, skb);
	if (unlikely(err < 0)) {
		kfree_skb(skb);
		return ERR_PTR(err);
	}

	bt_cb(skb)->l2cap.fcs = chan->fcs;
	bt_cb(skb)->l2cap.retries = 0;
	return skb;
}

static int l2cap_segment_sdu(struct l2cap_chan *chan,
			     struct sk_buff_head *seg_queue,
			     struct msghdr *msg, size_t len)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct sk_buff *skb;
	u16 sdu_len;
	size_t pdu_len;
<<<<<<< HEAD
	int err = 0;
	u8 sar;

	BT_DBG("sk %p, msg %p, len %d", sk, msg, (int)len);
=======
	u8 sar;

	BT_DBG("chan %p, msg %p, len %zu", chan, msg, len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* It is critical that ERTM PDUs fit in a single HCI fragment,
	 * so fragmented skbs are not used.  The HCI layer's handling
	 * of fragmented skbs is not compatible with ERTM's queueing.
	 */

	/* PDU size is derived from the HCI MTU */
<<<<<<< HEAD
	pdu_len = l2cap_pi(sk)->conn->mtu;

	/* Constrain BR/EDR PDU size to fit within the largest radio packet */
	if (!l2cap_pi(sk)->ampcon)
		pdu_len = min_t(size_t, pdu_len, L2CAP_BREDR_MAX_PAYLOAD);

	/* Adjust for largest possible L2CAP overhead. */
	pdu_len -= L2CAP_EXTENDED_HDR_SIZE + L2CAP_FCS_SIZE;

	/* Remote device may have requested smaller PDUs */
	pdu_len = min_t(size_t, pdu_len, l2cap_pi(sk)->remote_mps);
=======
	pdu_len = chan->conn->mtu;

	/* Constrain PDU size for BR/EDR connections */
	pdu_len = min_t(size_t, pdu_len, L2CAP_BREDR_MAX_PAYLOAD);

	/* Adjust for largest possible L2CAP overhead. */
	if (chan->fcs)
		pdu_len -= L2CAP_FCS_SIZE;

	pdu_len -= __ertm_hdr_size(chan);

	/* Remote device may have requested smaller PDUs */
	pdu_len = min_t(size_t, pdu_len, chan->remote_mps);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (len <= pdu_len) {
		sar = L2CAP_SAR_UNSEGMENTED;
		sdu_len = 0;
		pdu_len = len;
	} else {
		sar = L2CAP_SAR_START;
		sdu_len = len;
<<<<<<< HEAD
		pdu_len -= L2CAP_SDULEN_SIZE;
	}

	while (len) {
		skb = l2cap_create_iframe_pdu(sk, msg, pdu_len, sdu_len, reseg);

		BT_DBG("iframe skb %p", skb);
=======
	}

	while (len > 0) {
		skb = l2cap_create_iframe_pdu(chan, msg, pdu_len, sdu_len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (IS_ERR(skb)) {
			__skb_queue_purge(seg_queue);
			return PTR_ERR(skb);
		}

<<<<<<< HEAD
		bt_cb(skb)->control.sar = sar;
		__skb_queue_tail(seg_queue, skb);

		len -= pdu_len;
		if (sdu_len) {
			sdu_len = 0;
			pdu_len += L2CAP_SDULEN_SIZE;
		}
=======
		bt_cb(skb)->l2cap.sar = sar;
		__skb_queue_tail(seg_queue, skb);

		len -= pdu_len;
		if (sdu_len)
			sdu_len = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (len <= pdu_len) {
			sar = L2CAP_SAR_END;
			pdu_len = len;
		} else {
			sar = L2CAP_SAR_CONTINUE;
		}
	}

<<<<<<< HEAD
	return err;
}

static inline int is_initial_frame(u8 sar)
{
	return (sar == L2CAP_SAR_UNSEGMENTED ||
		sar == L2CAP_SAR_START);
}

static inline int l2cap_skbuff_to_kvec(struct sk_buff *skb, struct kvec *iv,
					size_t veclen)
{
	struct sk_buff *frag_iter;

	BT_DBG("skb %p (len %d), iv %p", skb, (int)skb->len, iv);

	if (iv->iov_len + skb->len > veclen)
		return -ENOMEM;

	memcpy(iv->iov_base + iv->iov_len, skb->data, skb->len);
	iv->iov_len += skb->len;

	skb_walk_frags(skb, frag_iter) {
		if (iv->iov_len + skb->len > veclen)
			return -ENOMEM;

		BT_DBG("Copying %d bytes", (int)frag_iter->len);
		memcpy(iv->iov_base + iv->iov_len, frag_iter->data,
			frag_iter->len);
		iv->iov_len += frag_iter->len;
	}

	return 0;
}

int l2cap_resegment_queue(struct sock *sk, struct sk_buff_head *queue)
{
	void *buf;
	int buflen;
	int err = 0;
	struct sk_buff *skb;
	struct msghdr msg;
	struct kvec iv;
	struct sk_buff_head old_frames;
	struct l2cap_pinfo *pi = l2cap_pi(sk);

	BT_DBG("sk %p", sk);

	if (skb_queue_empty(queue))
		return 0;

	memset(&msg, 0, sizeof(msg));
	msg.msg_iov = (struct iovec *) &iv;

	buflen = pi->omtu + L2CAP_FCS_SIZE;
	buf = kzalloc(buflen, GFP_TEMPORARY);

	if (!buf) {
		BT_DBG("Could not allocate resegmentation buffer");
		return -ENOMEM;
	}

	/* Move current frames off the original queue */
	__skb_queue_head_init(&old_frames);
	skb_queue_splice_tail_init(queue, &old_frames);

	while (!skb_queue_empty(&old_frames)) {
		struct sk_buff_head current_sdu;
		u8 original_sar;

		/* Reassemble each SDU from one or more PDUs */

		iv.iov_base = buf;
		iv.iov_len = 0;

		skb = skb_peek(&old_frames);
		original_sar = bt_cb(skb)->control.sar;

		__skb_unlink(skb, &old_frames);

		/* Append data to SDU */
		if (pi->extended_control)
			skb_pull(skb, L2CAP_EXTENDED_HDR_SIZE);
		else
			skb_pull(skb, L2CAP_ENHANCED_HDR_SIZE);

		if (original_sar == L2CAP_SAR_START)
			skb_pull(skb, L2CAP_SDULEN_SIZE);

		err = l2cap_skbuff_to_kvec(skb, &iv, buflen);

		if (bt_cb(skb)->control.fcs == L2CAP_FCS_CRC16)
			iv.iov_len -= L2CAP_FCS_SIZE;

		/* Free skb */
		kfree_skb(skb);

		if (err)
			break;

		while (!skb_queue_empty(&old_frames) && !err) {
			/* Check next frame */
			skb = skb_peek(&old_frames);

			if (is_initial_frame(bt_cb(skb)->control.sar))
				break;

			__skb_unlink(skb, &old_frames);

			/* Append data to SDU */
			if (pi->extended_control)
				skb_pull(skb, L2CAP_EXTENDED_HDR_SIZE);
			else
				skb_pull(skb, L2CAP_ENHANCED_HDR_SIZE);

			if (bt_cb(skb)->control.sar == L2CAP_SAR_START)
				skb_pull(skb, L2CAP_SDULEN_SIZE);

			err = l2cap_skbuff_to_kvec(skb, &iv, buflen);

			if (bt_cb(skb)->control.fcs == L2CAP_FCS_CRC16)
				iv.iov_len -= L2CAP_FCS_SIZE;

			/* Free skb */
			kfree_skb(skb);
		}

		if (err)
			break;

		/* Segment data */

		__skb_queue_head_init(&current_sdu);

		/* skbs for the SDU were just freed, but the
		 * resegmenting process could produce more, smaller
		 * skbs due to smaller PDUs and reduced HCI MTU.  The
		 * overhead from the sk_buff structs could put us over
		 * the sk_sndbuf limit.
		 *
		 * Since this code is running in response to a
		 * received poll/final packet, it cannot block.
		 * Therefore, memory allocation needs to be allowed by
		 * falling back to bt_skb_alloc() (with
		 * skb_set_owner_w() to maintain sk_wmem_alloc
		 * correctly).
		 */
		msg.msg_iovlen = iv.iov_len;
		err = l2cap_segment_sdu(sk, &current_sdu, &msg,
					msg.msg_iovlen, 1);

		if (err || skb_queue_empty(&current_sdu)) {
			BT_DBG("Error %d resegmenting data for socket %p",
				err, sk);
			__skb_queue_purge(&current_sdu);
			break;
		}

		/* Fix up first PDU SAR bits */
		if (!is_initial_frame(original_sar)) {
			BT_DBG("Changing SAR bits, %d PDUs",
				skb_queue_len(&current_sdu));
			skb = skb_peek(&current_sdu);

			if (skb_queue_len(&current_sdu) == 1) {
				/* Change SAR from 'unsegmented' to 'end' */
				bt_cb(skb)->control.sar = L2CAP_SAR_END;
			} else {
				struct l2cap_hdr *lh;
				size_t hdrlen;

				/* Change SAR from 'start' to 'continue' */
				bt_cb(skb)->control.sar = L2CAP_SAR_CONTINUE;

				/* Start frames contain 2 bytes for
				 * sdulen and continue frames don't.
				 * Must rewrite header to eliminate
				 * sdulen and then adjust l2cap frame
				 * length.
				 */
				if (pi->extended_control)
					hdrlen = L2CAP_EXTENDED_HDR_SIZE;
				else
					hdrlen = L2CAP_ENHANCED_HDR_SIZE;

				memmove(skb->data + L2CAP_SDULEN_SIZE,
					skb->data, hdrlen);
				skb_pull(skb, L2CAP_SDULEN_SIZE);
				lh = (struct l2cap_hdr *)skb->data;
				lh->len = cpu_to_le16(le16_to_cpu(lh->len) -
							L2CAP_SDULEN_SIZE);
			}
		}

		/* Add to queue */
		skb_queue_splice_tail(&current_sdu, queue);
	}

	__skb_queue_purge(&old_frames);
	if (err)
		__skb_queue_purge(queue);

	kfree(buf);

	BT_DBG("Queue resegmented, err=%d", err);
	return err;
}

static void l2cap_resegment_worker(struct work_struct *work)
{
	int err = 0;
	struct l2cap_resegment_work *seg_work =
		container_of(work, struct l2cap_resegment_work, work);
	struct sock *sk = seg_work->sk;

	kfree(seg_work);

	BT_DBG("sk %p", sk);
	lock_sock(sk);

	if (l2cap_pi(sk)->amp_move_state != L2CAP_AMP_STATE_RESEGMENT) {
		release_sock(sk);
		sock_put(sk);
		return;
	}

	err = l2cap_resegment_queue(sk, TX_QUEUE(sk));

	l2cap_pi(sk)->amp_move_state = L2CAP_AMP_STATE_STABLE;

	if (skb_queue_empty(TX_QUEUE(sk)))
		sk->sk_send_head = NULL;
	else
		sk->sk_send_head = skb_peek(TX_QUEUE(sk));

	if (err)
		l2cap_send_disconn_req(l2cap_pi(sk)->conn, sk, ECONNRESET);
	else
		l2cap_ertm_send(sk);

	release_sock(sk);
	sock_put(sk);
}

static int l2cap_setup_resegment(struct sock *sk)
{
	struct l2cap_resegment_work *seg_work;

	BT_DBG("sk %p", sk);

	if (skb_queue_empty(TX_QUEUE(sk)))
		return 0;

	seg_work = kzalloc(sizeof(*seg_work), GFP_ATOMIC);
	if (!seg_work)
		return -ENOMEM;

	INIT_WORK(&seg_work->work, l2cap_resegment_worker);
	sock_hold(sk);
	seg_work->sk = sk;

	if (!queue_work(_l2cap_wq, &seg_work->work)) {
		kfree(seg_work);
		sock_put(sk);
		return -ENOMEM;
	}

	l2cap_pi(sk)->amp_move_state = L2CAP_AMP_STATE_RESEGMENT;

	return 0;
}

static inline int l2cap_rmem_available(struct sock *sk)
{
	BT_DBG("sk_rmem_alloc %d, sk_rcvbuf %d",
		atomic_read(&sk->sk_rmem_alloc), sk->sk_rcvbuf);
	return atomic_read(&sk->sk_rmem_alloc) < sk->sk_rcvbuf / 3;
}

static inline int l2cap_rmem_full(struct sock *sk)
{
	BT_DBG("sk_rmem_alloc %d, sk_rcvbuf %d",
		atomic_read(&sk->sk_rmem_alloc), sk->sk_rcvbuf);
	return atomic_read(&sk->sk_rmem_alloc) > (2 * sk->sk_rcvbuf) / 3;
}

void l2cap_amp_move_init(struct sock *sk)
{
	BT_DBG("sk %p", sk);

	if (!l2cap_pi(sk)->conn)
		return;

	if (!(l2cap_pi(sk)->conn->fc_mask & L2CAP_FC_A2MP) || !enable_hs)
		return;

	if (l2cap_pi(sk)->amp_id == 0) {
		if (l2cap_pi(sk)->amp_pref != BT_AMP_POLICY_PREFER_AMP)
			return;
		l2cap_pi(sk)->amp_move_role = L2CAP_AMP_MOVE_INITIATOR;
		l2cap_pi(sk)->amp_move_state = L2CAP_AMP_STATE_WAIT_PREPARE;
		amp_create_physical(l2cap_pi(sk)->conn, sk);
	} else {
		l2cap_pi(sk)->amp_move_role = L2CAP_AMP_MOVE_INITIATOR;
		l2cap_pi(sk)->amp_move_state =
					L2CAP_AMP_STATE_WAIT_MOVE_RSP_SUCCESS;
		l2cap_pi(sk)->amp_move_id = 0;
		l2cap_amp_move_setup(sk);
		l2cap_send_move_chan_req(l2cap_pi(sk)->conn,
					l2cap_pi(sk), l2cap_pi(sk)->scid, 0);
		l2cap_sock_set_timer(sk, L2CAP_MOVE_TIMEOUT);
	}
}

static void l2cap_chan_ready(struct sock *sk)
{
	struct sock *parent = bt_sk(sk)->parent;

	BT_DBG("sk %p, parent %p", sk, parent);

	l2cap_pi(sk)->conf_state = 0;
	l2cap_sock_clear_timer(sk);

	if (!parent) {
		/* Outgoing channel.
		 * Wake up socket sleeping on connect.
		 */
		sk->sk_state = BT_CONNECTED;
		sk->sk_state_change(sk);
	} else {
		/* Incoming channel.
		 * Wake up socket sleeping on accept.
		 */
		parent->sk_data_ready(parent, 0);
	}
}

/* Copy frame to all raw sockets on that connection */
static void l2cap_raw_recv(struct l2cap_conn *conn, struct sk_buff *skb)
{
	struct l2cap_chan_list *l = &conn->chan_list;
	struct sk_buff *nskb;
	struct sock *sk;

	BT_DBG("conn %p", conn);

	read_lock(&l->lock);
	for (sk = l->head; sk; sk = l2cap_pi(sk)->next_c) {
		if (sk->sk_type != SOCK_RAW)
			continue;

		/* Don't send frame to the socket it came from */
		if (skb->sk == sk)
			continue;
		nskb = skb_clone(skb, GFP_ATOMIC);
		if (!nskb)
			continue;

		if (sock_queue_rcv_skb(sk, nskb))
			kfree_skb(nskb);
	}
	read_unlock(&l->lock);
}

/* ---- L2CAP signalling commands ---- */
static struct sk_buff *l2cap_build_cmd(struct l2cap_conn *conn,
				u8 code, u8 ident, u16 dlen, void *data)
=======
	return 0;
}

static struct sk_buff *l2cap_create_le_flowctl_pdu(struct l2cap_chan *chan,
						   struct msghdr *msg,
						   size_t len, u16 sdulen)
{
	struct l2cap_conn *conn = chan->conn;
	struct sk_buff *skb;
	int err, count, hlen;
	struct l2cap_hdr *lh;

	BT_DBG("chan %p len %zu", chan, len);

	if (!conn)
		return ERR_PTR(-ENOTCONN);

	hlen = L2CAP_HDR_SIZE;

	if (sdulen)
		hlen += L2CAP_SDULEN_SIZE;

	count = min_t(unsigned int, (conn->mtu - hlen), len);

	skb = chan->ops->alloc_skb(chan, hlen, count,
				   msg->msg_flags & MSG_DONTWAIT);
	if (IS_ERR(skb))
		return skb;

	/* Create L2CAP header */
	lh = skb_put(skb, L2CAP_HDR_SIZE);
	lh->cid = cpu_to_le16(chan->dcid);
	lh->len = cpu_to_le16(len + (hlen - L2CAP_HDR_SIZE));

	if (sdulen)
		put_unaligned_le16(sdulen, skb_put(skb, L2CAP_SDULEN_SIZE));

	err = l2cap_skbuff_fromiovec(chan, msg, len, count, skb);
	if (unlikely(err < 0)) {
		kfree_skb(skb);
		return ERR_PTR(err);
	}

	return skb;
}

static int l2cap_segment_le_sdu(struct l2cap_chan *chan,
				struct sk_buff_head *seg_queue,
				struct msghdr *msg, size_t len)
{
	struct sk_buff *skb;
	size_t pdu_len;
	u16 sdu_len;

	BT_DBG("chan %p, msg %p, len %zu", chan, msg, len);

	sdu_len = len;
	pdu_len = chan->remote_mps - L2CAP_SDULEN_SIZE;

	while (len > 0) {
		if (len <= pdu_len)
			pdu_len = len;

		skb = l2cap_create_le_flowctl_pdu(chan, msg, pdu_len, sdu_len);
		if (IS_ERR(skb)) {
			__skb_queue_purge(seg_queue);
			return PTR_ERR(skb);
		}

		__skb_queue_tail(seg_queue, skb);

		len -= pdu_len;

		if (sdu_len) {
			sdu_len = 0;
			pdu_len += L2CAP_SDULEN_SIZE;
		}
	}

	return 0;
}

static void l2cap_le_flowctl_send(struct l2cap_chan *chan)
{
	int sent = 0;

	BT_DBG("chan %p", chan);

	while (chan->tx_credits && !skb_queue_empty(&chan->tx_q)) {
		l2cap_do_send(chan, skb_dequeue(&chan->tx_q));
		chan->tx_credits--;
		sent++;
	}

	BT_DBG("Sent %d credits %u queued %u", sent, chan->tx_credits,
	       skb_queue_len(&chan->tx_q));
}

int l2cap_chan_send(struct l2cap_chan *chan, struct msghdr *msg, size_t len)
{
	struct sk_buff *skb;
	int err;
	struct sk_buff_head seg_queue;

	if (!chan->conn)
		return -ENOTCONN;

	/* Connectionless channel */
	if (chan->chan_type == L2CAP_CHAN_CONN_LESS) {
		skb = l2cap_create_connless_pdu(chan, msg, len);
		if (IS_ERR(skb))
			return PTR_ERR(skb);

		l2cap_do_send(chan, skb);
		return len;
	}

	switch (chan->mode) {
	case L2CAP_MODE_LE_FLOWCTL:
	case L2CAP_MODE_EXT_FLOWCTL:
		/* Check outgoing MTU */
		if (len > chan->omtu)
			return -EMSGSIZE;

		__skb_queue_head_init(&seg_queue);

		err = l2cap_segment_le_sdu(chan, &seg_queue, msg, len);

		if (chan->state != BT_CONNECTED) {
			__skb_queue_purge(&seg_queue);
			err = -ENOTCONN;
		}

		if (err)
			return err;

		skb_queue_splice_tail_init(&seg_queue, &chan->tx_q);

		l2cap_le_flowctl_send(chan);

		if (!chan->tx_credits)
			chan->ops->suspend(chan);

		err = len;

		break;

	case L2CAP_MODE_BASIC:
		/* Check outgoing MTU */
		if (len > chan->omtu)
			return -EMSGSIZE;

		/* Create a basic PDU */
		skb = l2cap_create_basic_pdu(chan, msg, len);
		if (IS_ERR(skb))
			return PTR_ERR(skb);

		l2cap_do_send(chan, skb);
		err = len;
		break;

	case L2CAP_MODE_ERTM:
	case L2CAP_MODE_STREAMING:
		/* Check outgoing MTU */
		if (len > chan->omtu) {
			err = -EMSGSIZE;
			break;
		}

		__skb_queue_head_init(&seg_queue);

		/* Do segmentation before calling in to the state machine,
		 * since it's possible to block while waiting for memory
		 * allocation.
		 */
		err = l2cap_segment_sdu(chan, &seg_queue, msg, len);

		if (err)
			break;

		if (chan->mode == L2CAP_MODE_ERTM)
			l2cap_tx(chan, NULL, &seg_queue, L2CAP_EV_DATA_REQUEST);
		else
			l2cap_streaming_send(chan, &seg_queue);

		err = len;

		/* If the skbs were not queued for sending, they'll still be in
		 * seg_queue and need to be purged.
		 */
		__skb_queue_purge(&seg_queue);
		break;

	default:
		BT_DBG("bad state %1.1x", chan->mode);
		err = -EBADFD;
	}

	return err;
}
EXPORT_SYMBOL_GPL(l2cap_chan_send);

static void l2cap_send_srej(struct l2cap_chan *chan, u16 txseq)
{
	struct l2cap_ctrl control;
	u16 seq;

	BT_DBG("chan %p, txseq %u", chan, txseq);

	memset(&control, 0, sizeof(control));
	control.sframe = 1;
	control.super = L2CAP_SUPER_SREJ;

	for (seq = chan->expected_tx_seq; seq != txseq;
	     seq = __next_seq(chan, seq)) {
		if (!l2cap_ertm_seq_in_queue(&chan->srej_q, seq)) {
			control.reqseq = seq;
			l2cap_send_sframe(chan, &control);
			l2cap_seq_list_append(&chan->srej_list, seq);
		}
	}

	chan->expected_tx_seq = __next_seq(chan, txseq);
}

static void l2cap_send_srej_tail(struct l2cap_chan *chan)
{
	struct l2cap_ctrl control;

	BT_DBG("chan %p", chan);

	if (chan->srej_list.tail == L2CAP_SEQ_LIST_CLEAR)
		return;

	memset(&control, 0, sizeof(control));
	control.sframe = 1;
	control.super = L2CAP_SUPER_SREJ;
	control.reqseq = chan->srej_list.tail;
	l2cap_send_sframe(chan, &control);
}

static void l2cap_send_srej_list(struct l2cap_chan *chan, u16 txseq)
{
	struct l2cap_ctrl control;
	u16 initial_head;
	u16 seq;

	BT_DBG("chan %p, txseq %u", chan, txseq);

	memset(&control, 0, sizeof(control));
	control.sframe = 1;
	control.super = L2CAP_SUPER_SREJ;

	/* Capture initial list head to allow only one pass through the list. */
	initial_head = chan->srej_list.head;

	do {
		seq = l2cap_seq_list_pop(&chan->srej_list);
		if (seq == txseq || seq == L2CAP_SEQ_LIST_CLEAR)
			break;

		control.reqseq = seq;
		l2cap_send_sframe(chan, &control);
		l2cap_seq_list_append(&chan->srej_list, seq);
	} while (chan->srej_list.head != initial_head);
}

static void l2cap_process_reqseq(struct l2cap_chan *chan, u16 reqseq)
{
	struct sk_buff *acked_skb;
	u16 ackseq;

	BT_DBG("chan %p, reqseq %u", chan, reqseq);

	if (chan->unacked_frames == 0 || reqseq == chan->expected_ack_seq)
		return;

	BT_DBG("expected_ack_seq %u, unacked_frames %u",
	       chan->expected_ack_seq, chan->unacked_frames);

	for (ackseq = chan->expected_ack_seq; ackseq != reqseq;
	     ackseq = __next_seq(chan, ackseq)) {

		acked_skb = l2cap_ertm_seq_in_queue(&chan->tx_q, ackseq);
		if (acked_skb) {
			skb_unlink(acked_skb, &chan->tx_q);
			kfree_skb(acked_skb);
			chan->unacked_frames--;
		}
	}

	chan->expected_ack_seq = reqseq;

	if (chan->unacked_frames == 0)
		__clear_retrans_timer(chan);

	BT_DBG("unacked_frames %u", chan->unacked_frames);
}

static void l2cap_abort_rx_srej_sent(struct l2cap_chan *chan)
{
	BT_DBG("chan %p", chan);

	chan->expected_tx_seq = chan->buffer_seq;
	l2cap_seq_list_clear(&chan->srej_list);
	skb_queue_purge(&chan->srej_q);
	chan->rx_state = L2CAP_RX_STATE_RECV;
}

static void l2cap_tx_state_xmit(struct l2cap_chan *chan,
				struct l2cap_ctrl *control,
				struct sk_buff_head *skbs, u8 event)
{
	BT_DBG("chan %p, control %p, skbs %p, event %d", chan, control, skbs,
	       event);

	switch (event) {
	case L2CAP_EV_DATA_REQUEST:
		if (chan->tx_send_head == NULL)
			chan->tx_send_head = skb_peek(skbs);

		skb_queue_splice_tail_init(skbs, &chan->tx_q);
		l2cap_ertm_send(chan);
		break;
	case L2CAP_EV_LOCAL_BUSY_DETECTED:
		BT_DBG("Enter LOCAL_BUSY");
		set_bit(CONN_LOCAL_BUSY, &chan->conn_state);

		if (chan->rx_state == L2CAP_RX_STATE_SREJ_SENT) {
			/* The SREJ_SENT state must be aborted if we are to
			 * enter the LOCAL_BUSY state.
			 */
			l2cap_abort_rx_srej_sent(chan);
		}

		l2cap_send_ack(chan);

		break;
	case L2CAP_EV_LOCAL_BUSY_CLEAR:
		BT_DBG("Exit LOCAL_BUSY");
		clear_bit(CONN_LOCAL_BUSY, &chan->conn_state);

		if (test_bit(CONN_RNR_SENT, &chan->conn_state)) {
			struct l2cap_ctrl local_control;

			memset(&local_control, 0, sizeof(local_control));
			local_control.sframe = 1;
			local_control.super = L2CAP_SUPER_RR;
			local_control.poll = 1;
			local_control.reqseq = chan->buffer_seq;
			l2cap_send_sframe(chan, &local_control);

			chan->retry_count = 1;
			__set_monitor_timer(chan);
			chan->tx_state = L2CAP_TX_STATE_WAIT_F;
		}
		break;
	case L2CAP_EV_RECV_REQSEQ_AND_FBIT:
		l2cap_process_reqseq(chan, control->reqseq);
		break;
	case L2CAP_EV_EXPLICIT_POLL:
		l2cap_send_rr_or_rnr(chan, 1);
		chan->retry_count = 1;
		__set_monitor_timer(chan);
		__clear_ack_timer(chan);
		chan->tx_state = L2CAP_TX_STATE_WAIT_F;
		break;
	case L2CAP_EV_RETRANS_TO:
		l2cap_send_rr_or_rnr(chan, 1);
		chan->retry_count = 1;
		__set_monitor_timer(chan);
		chan->tx_state = L2CAP_TX_STATE_WAIT_F;
		break;
	case L2CAP_EV_RECV_FBIT:
		/* Nothing to process */
		break;
	default:
		break;
	}
}

static void l2cap_tx_state_wait_f(struct l2cap_chan *chan,
				  struct l2cap_ctrl *control,
				  struct sk_buff_head *skbs, u8 event)
{
	BT_DBG("chan %p, control %p, skbs %p, event %d", chan, control, skbs,
	       event);

	switch (event) {
	case L2CAP_EV_DATA_REQUEST:
		if (chan->tx_send_head == NULL)
			chan->tx_send_head = skb_peek(skbs);
		/* Queue data, but don't send. */
		skb_queue_splice_tail_init(skbs, &chan->tx_q);
		break;
	case L2CAP_EV_LOCAL_BUSY_DETECTED:
		BT_DBG("Enter LOCAL_BUSY");
		set_bit(CONN_LOCAL_BUSY, &chan->conn_state);

		if (chan->rx_state == L2CAP_RX_STATE_SREJ_SENT) {
			/* The SREJ_SENT state must be aborted if we are to
			 * enter the LOCAL_BUSY state.
			 */
			l2cap_abort_rx_srej_sent(chan);
		}

		l2cap_send_ack(chan);

		break;
	case L2CAP_EV_LOCAL_BUSY_CLEAR:
		BT_DBG("Exit LOCAL_BUSY");
		clear_bit(CONN_LOCAL_BUSY, &chan->conn_state);

		if (test_bit(CONN_RNR_SENT, &chan->conn_state)) {
			struct l2cap_ctrl local_control;
			memset(&local_control, 0, sizeof(local_control));
			local_control.sframe = 1;
			local_control.super = L2CAP_SUPER_RR;
			local_control.poll = 1;
			local_control.reqseq = chan->buffer_seq;
			l2cap_send_sframe(chan, &local_control);

			chan->retry_count = 1;
			__set_monitor_timer(chan);
			chan->tx_state = L2CAP_TX_STATE_WAIT_F;
		}
		break;
	case L2CAP_EV_RECV_REQSEQ_AND_FBIT:
		l2cap_process_reqseq(chan, control->reqseq);
		fallthrough;

	case L2CAP_EV_RECV_FBIT:
		if (control && control->final) {
			__clear_monitor_timer(chan);
			if (chan->unacked_frames > 0)
				__set_retrans_timer(chan);
			chan->retry_count = 0;
			chan->tx_state = L2CAP_TX_STATE_XMIT;
			BT_DBG("recv fbit tx_state 0x2.2%x", chan->tx_state);
		}
		break;
	case L2CAP_EV_EXPLICIT_POLL:
		/* Ignore */
		break;
	case L2CAP_EV_MONITOR_TO:
		if (chan->max_tx == 0 || chan->retry_count < chan->max_tx) {
			l2cap_send_rr_or_rnr(chan, 1);
			__set_monitor_timer(chan);
			chan->retry_count++;
		} else {
			l2cap_send_disconn_req(chan, ECONNABORTED);
		}
		break;
	default:
		break;
	}
}

static void l2cap_tx(struct l2cap_chan *chan, struct l2cap_ctrl *control,
		     struct sk_buff_head *skbs, u8 event)
{
	BT_DBG("chan %p, control %p, skbs %p, event %d, state %d",
	       chan, control, skbs, event, chan->tx_state);

	switch (chan->tx_state) {
	case L2CAP_TX_STATE_XMIT:
		l2cap_tx_state_xmit(chan, control, skbs, event);
		break;
	case L2CAP_TX_STATE_WAIT_F:
		l2cap_tx_state_wait_f(chan, control, skbs, event);
		break;
	default:
		/* Ignore event */
		break;
	}
}

static void l2cap_pass_to_tx(struct l2cap_chan *chan,
			     struct l2cap_ctrl *control)
{
	BT_DBG("chan %p, control %p", chan, control);
	l2cap_tx(chan, control, NULL, L2CAP_EV_RECV_REQSEQ_AND_FBIT);
}

static void l2cap_pass_to_tx_fbit(struct l2cap_chan *chan,
				  struct l2cap_ctrl *control)
{
	BT_DBG("chan %p, control %p", chan, control);
	l2cap_tx(chan, control, NULL, L2CAP_EV_RECV_FBIT);
}

/* Copy frame to all raw sockets on that connection */
static void l2cap_raw_recv(struct l2cap_conn *conn, struct sk_buff *skb)
{
	struct sk_buff *nskb;
	struct l2cap_chan *chan;

	BT_DBG("conn %p", conn);

	mutex_lock(&conn->chan_lock);

	list_for_each_entry(chan, &conn->chan_l, list) {
		if (chan->chan_type != L2CAP_CHAN_RAW)
			continue;

		/* Don't send frame to the channel it came from */
		if (bt_cb(skb)->l2cap.chan == chan)
			continue;

		nskb = skb_clone(skb, GFP_KERNEL);
		if (!nskb)
			continue;
		if (chan->ops->recv(chan, nskb))
			kfree_skb(nskb);
	}

	mutex_unlock(&conn->chan_lock);
}

/* ---- L2CAP signalling commands ---- */
static struct sk_buff *l2cap_build_cmd(struct l2cap_conn *conn, u8 code,
				       u8 ident, u16 dlen, void *data)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct sk_buff *skb, **frag;
	struct l2cap_cmd_hdr *cmd;
	struct l2cap_hdr *lh;
	int len, count;
<<<<<<< HEAD
	unsigned int mtu = conn->hcon->hdev->acl_mtu;

	BT_DBG("conn %p, code 0x%2.2x, ident 0x%2.2x, len %d",
			conn, code, ident, dlen);
=======

	BT_DBG("conn %p, code 0x%2.2x, ident 0x%2.2x, len %u",
	       conn, code, ident, dlen);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (conn->mtu < L2CAP_HDR_SIZE + L2CAP_CMD_HDR_SIZE)
		return NULL;

	len = L2CAP_HDR_SIZE + L2CAP_CMD_HDR_SIZE + dlen;
<<<<<<< HEAD
	count = min_t(unsigned int, mtu, len);

	skb = bt_skb_alloc(count, GFP_ATOMIC);
	if (!skb)
		return NULL;

	lh = (struct l2cap_hdr *) skb_put(skb, L2CAP_HDR_SIZE);
=======
	count = min_t(unsigned int, conn->mtu, len);

	skb = bt_skb_alloc(count, GFP_KERNEL);
	if (!skb)
		return NULL;

	lh = skb_put(skb, L2CAP_HDR_SIZE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	lh->len = cpu_to_le16(L2CAP_CMD_HDR_SIZE + dlen);

	if (conn->hcon->type == LE_LINK)
		lh->cid = cpu_to_le16(L2CAP_CID_LE_SIGNALING);
	else
		lh->cid = cpu_to_le16(L2CAP_CID_SIGNALING);

<<<<<<< HEAD
	cmd = (struct l2cap_cmd_hdr *) skb_put(skb, L2CAP_CMD_HDR_SIZE);
=======
	cmd = skb_put(skb, L2CAP_CMD_HDR_SIZE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	cmd->code  = code;
	cmd->ident = ident;
	cmd->len   = cpu_to_le16(dlen);

	if (dlen) {
		count -= L2CAP_HDR_SIZE + L2CAP_CMD_HDR_SIZE;
<<<<<<< HEAD
		memcpy(skb_put(skb, count), data, count);
=======
		skb_put_data(skb, data, count);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		data += count;
	}

	len -= skb->len;

	/* Continuation fragments (no L2CAP header) */
	frag = &skb_shinfo(skb)->frag_list;
	while (len) {
<<<<<<< HEAD
		count = min_t(unsigned int, mtu, len);

		*frag = bt_skb_alloc(count, GFP_ATOMIC);
		if (!*frag)
			goto fail;

		memcpy(skb_put(*frag, count), data, count);
=======
		count = min_t(unsigned int, conn->mtu, len);

		*frag = bt_skb_alloc(count, GFP_KERNEL);
		if (!*frag)
			goto fail;

		skb_put_data(*frag, data, count);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		len  -= count;
		data += count;

		frag = &(*frag)->next;
	}

	return skb;

fail:
	kfree_skb(skb);
	return NULL;
}

<<<<<<< HEAD
static inline int l2cap_get_conf_opt(void **ptr, int *type, int *olen, unsigned long *val)
=======
static inline int l2cap_get_conf_opt(void **ptr, int *type, int *olen,
				     unsigned long *val)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct l2cap_conf_opt *opt = *ptr;
	int len;

	len = L2CAP_CONF_OPT_SIZE + opt->len;
	*ptr += len;

	*type = opt->type;
	*olen = opt->len;

	switch (opt->len) {
	case 1:
		*val = *((u8 *) opt->val);
		break;

	case 2:
		*val = get_unaligned_le16(opt->val);
		break;

	case 4:
		*val = get_unaligned_le32(opt->val);
		break;

	default:
		*val = (unsigned long) opt->val;
		break;
	}

<<<<<<< HEAD
	BT_DBG("type 0x%2.2x len %d val 0x%lx", *type, opt->len, *val);
=======
	BT_DBG("type 0x%2.2x len %u val 0x%lx", *type, opt->len, *val);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return len;
}

static void l2cap_add_conf_opt(void **ptr, u8 type, u8 len, unsigned long val, size_t size)
{
	struct l2cap_conf_opt *opt = *ptr;

<<<<<<< HEAD
	BT_DBG("type 0x%2.2x len %d val 0x%lx", type, len, val);
=======
	BT_DBG("type 0x%2.2x len %u val 0x%lx", type, len, val);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (size < L2CAP_CONF_OPT_SIZE + len)
		return;

	opt->type = type;
	opt->len  = len;

	switch (len) {
	case 1:
		*((u8 *) opt->val)  = val;
		break;

	case 2:
		put_unaligned_le16(val, opt->val);
		break;

	case 4:
		put_unaligned_le32(val, opt->val);
		break;

	default:
		memcpy(opt->val, (void *) val, len);
		break;
	}

	*ptr += L2CAP_CONF_OPT_SIZE + len;
}

<<<<<<< HEAD
static void l2cap_ertm_ack_timeout(struct work_struct *work)
{
	struct delayed_work *delayed =
		container_of(work, struct delayed_work, work);
	struct l2cap_pinfo *pi =
		container_of(delayed, struct l2cap_pinfo, ack_work);
	struct sock *sk = (struct sock *)pi;
	u16 frames_to_ack;

	BT_DBG("sk %p", sk);

	if (!sk)
		return;

	lock_sock(sk);

	if (!l2cap_pi(sk)->conn) {
		release_sock(sk);
		return;
	}

	frames_to_ack = __delta_seq(l2cap_pi(sk)->buffer_seq,
				    l2cap_pi(sk)->last_acked_seq,
				    l2cap_pi(sk));

	if (frames_to_ack)
		l2cap_ertm_send_rr_or_rnr(sk, 0);

	release_sock(sk);
}

static void l2cap_ertm_retrans_timeout(struct work_struct *work)
{
	struct delayed_work *delayed =
		container_of(work, struct delayed_work, work);
	struct l2cap_pinfo *pi =
		container_of(delayed, struct l2cap_pinfo, retrans_work);
	struct sock *sk = (struct sock *)pi;

	BT_DBG("sk %p", sk);

	if (!sk)
		return;

	lock_sock(sk);

	if (!l2cap_pi(sk)->conn) {
		release_sock(sk);
		return;
	}

	l2cap_ertm_tx(sk, 0, 0, L2CAP_ERTM_EVENT_RETRANS_TIMER_EXPIRES);
	release_sock(sk);
}

static void l2cap_ertm_monitor_timeout(struct work_struct *work)
{
	struct delayed_work *delayed =
		container_of(work, struct delayed_work, work);
	struct l2cap_pinfo *pi =
		container_of(delayed, struct l2cap_pinfo, monitor_work);
	struct sock *sk = (struct sock *)pi;

	BT_DBG("sk %p", sk);

	if (!sk)
		return;

	lock_sock(sk);

	if (!l2cap_pi(sk)->conn) {
		release_sock(sk);
		return;
	}

	l2cap_ertm_tx(sk, 0, 0, L2CAP_ERTM_EVENT_MONITOR_TIMER_EXPIRES);

	release_sock(sk);
}

static inline void l2cap_ertm_init(struct sock *sk)
{
	l2cap_pi(sk)->next_tx_seq = 0;
	l2cap_pi(sk)->expected_tx_seq = 0;
	l2cap_pi(sk)->expected_ack_seq = 0;
	l2cap_pi(sk)->unacked_frames = 0;
	l2cap_pi(sk)->buffer_seq = 0;
	l2cap_pi(sk)->frames_sent = 0;
	l2cap_pi(sk)->last_acked_seq = 0;
	l2cap_pi(sk)->sdu = NULL;
	l2cap_pi(sk)->sdu_last_frag = NULL;
	l2cap_pi(sk)->sdu_len = 0;
	atomic_set(&l2cap_pi(sk)->ertm_queued, 0);

	l2cap_pi(sk)->rx_state = L2CAP_ERTM_RX_STATE_RECV;
	l2cap_pi(sk)->tx_state = L2CAP_ERTM_TX_STATE_XMIT;

	BT_DBG("tx_state 0x2.2%x rx_state 0x2.2%x", l2cap_pi(sk)->tx_state,
		l2cap_pi(sk)->rx_state);

	l2cap_pi(sk)->amp_id = 0;
	l2cap_pi(sk)->amp_move_state = L2CAP_AMP_STATE_STABLE;
	l2cap_pi(sk)->amp_move_role = L2CAP_AMP_MOVE_NONE;
	l2cap_pi(sk)->amp_move_reqseq = 0;
	l2cap_pi(sk)->amp_move_event = 0;

	INIT_DELAYED_WORK(&l2cap_pi(sk)->ack_work, l2cap_ertm_ack_timeout);
	INIT_DELAYED_WORK(&l2cap_pi(sk)->retrans_work,
			l2cap_ertm_retrans_timeout);
	INIT_DELAYED_WORK(&l2cap_pi(sk)->monitor_work,
			l2cap_ertm_monitor_timeout);
	INIT_WORK(&l2cap_pi(sk)->tx_work, l2cap_ertm_tx_worker);
	skb_queue_head_init(SREJ_QUEUE(sk));
	skb_queue_head_init(TX_QUEUE(sk));

	l2cap_seq_list_init(&l2cap_pi(sk)->srej_list, l2cap_pi(sk)->tx_win);
	l2cap_seq_list_init(&l2cap_pi(sk)->retrans_list,
			l2cap_pi(sk)->remote_tx_win);
}

void l2cap_ertm_destruct(struct sock *sk)
{
	l2cap_seq_list_free(&l2cap_pi(sk)->srej_list);
	l2cap_seq_list_free(&l2cap_pi(sk)->retrans_list);
}

void l2cap_ertm_shutdown(struct sock *sk)
{
	l2cap_ertm_stop_ack_timer(l2cap_pi(sk));
	l2cap_ertm_stop_retrans_timer(l2cap_pi(sk));
	l2cap_ertm_stop_monitor_timer(l2cap_pi(sk));
}

void l2cap_ertm_recv_done(struct sock *sk)
{
	lock_sock(sk);

	if (l2cap_pi(sk)->mode != L2CAP_MODE_ERTM ||
			sk->sk_state != BT_CONNECTED) {
		release_sock(sk);
		return;
	}

	/* Consume any queued incoming frames and update local busy status */
	if (l2cap_pi(sk)->rx_state == L2CAP_ERTM_RX_STATE_SREJ_SENT &&
			l2cap_ertm_rx_queued_iframes(sk))
		l2cap_send_disconn_req(l2cap_pi(sk)->conn, sk, ECONNRESET);
	else if ((l2cap_pi(sk)->conn_state & L2CAP_CONN_LOCAL_BUSY) &&
			l2cap_rmem_available(sk))
		l2cap_ertm_tx(sk, 0, 0, L2CAP_ERTM_EVENT_LOCAL_BUSY_CLEAR);

	release_sock(sk);
=======
static void l2cap_add_opt_efs(void **ptr, struct l2cap_chan *chan, size_t size)
{
	struct l2cap_conf_efs efs;

	switch (chan->mode) {
	case L2CAP_MODE_ERTM:
		efs.id		= chan->local_id;
		efs.stype	= chan->local_stype;
		efs.msdu	= cpu_to_le16(chan->local_msdu);
		efs.sdu_itime	= cpu_to_le32(chan->local_sdu_itime);
		efs.acc_lat	= cpu_to_le32(L2CAP_DEFAULT_ACC_LAT);
		efs.flush_to	= cpu_to_le32(L2CAP_EFS_DEFAULT_FLUSH_TO);
		break;

	case L2CAP_MODE_STREAMING:
		efs.id		= 1;
		efs.stype	= L2CAP_SERV_BESTEFFORT;
		efs.msdu	= cpu_to_le16(chan->local_msdu);
		efs.sdu_itime	= cpu_to_le32(chan->local_sdu_itime);
		efs.acc_lat	= 0;
		efs.flush_to	= 0;
		break;

	default:
		return;
	}

	l2cap_add_conf_opt(ptr, L2CAP_CONF_EFS, sizeof(efs),
			   (unsigned long) &efs, size);
}

static void l2cap_ack_timeout(struct work_struct *work)
{
	struct l2cap_chan *chan = container_of(work, struct l2cap_chan,
					       ack_timer.work);
	u16 frames_to_ack;

	BT_DBG("chan %p", chan);

	l2cap_chan_lock(chan);

	frames_to_ack = __seq_offset(chan, chan->buffer_seq,
				     chan->last_acked_seq);

	if (frames_to_ack)
		l2cap_send_rr_or_rnr(chan, 0);

	l2cap_chan_unlock(chan);
	l2cap_chan_put(chan);
}

int l2cap_ertm_init(struct l2cap_chan *chan)
{
	int err;

	chan->next_tx_seq = 0;
	chan->expected_tx_seq = 0;
	chan->expected_ack_seq = 0;
	chan->unacked_frames = 0;
	chan->buffer_seq = 0;
	chan->frames_sent = 0;
	chan->last_acked_seq = 0;
	chan->sdu = NULL;
	chan->sdu_last_frag = NULL;
	chan->sdu_len = 0;

	skb_queue_head_init(&chan->tx_q);

	if (chan->mode != L2CAP_MODE_ERTM)
		return 0;

	chan->rx_state = L2CAP_RX_STATE_RECV;
	chan->tx_state = L2CAP_TX_STATE_XMIT;

	skb_queue_head_init(&chan->srej_q);

	err = l2cap_seq_list_init(&chan->srej_list, chan->tx_win);
	if (err < 0)
		return err;

	err = l2cap_seq_list_init(&chan->retrans_list, chan->remote_tx_win);
	if (err < 0)
		l2cap_seq_list_free(&chan->srej_list);

	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline __u8 l2cap_select_mode(__u8 mode, __u16 remote_feat_mask)
{
	switch (mode) {
	case L2CAP_MODE_STREAMING:
	case L2CAP_MODE_ERTM:
		if (l2cap_mode_supported(mode, remote_feat_mask))
			return mode;
<<<<<<< HEAD
		/* fall through */
=======
		fallthrough;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	default:
		return L2CAP_MODE_BASIC;
	}
}

<<<<<<< HEAD
static void l2cap_setup_txwin(struct l2cap_pinfo *pi)
{
	if (pi->tx_win > L2CAP_TX_WIN_MAX_ENHANCED &&
		(pi->conn->feat_mask & L2CAP_FEAT_EXT_WINDOW)) {
		pi->tx_win_max = L2CAP_TX_WIN_MAX_EXTENDED;
		pi->extended_control = 1;
	} else {
		if (pi->tx_win > L2CAP_TX_WIN_MAX_ENHANCED)
			pi->tx_win = L2CAP_TX_WIN_MAX_ENHANCED;

		pi->tx_win_max = L2CAP_TX_WIN_MAX_ENHANCED;
		pi->extended_control = 0;
	}
	pi->ack_win = pi->tx_win;
}

static void l2cap_aggregate_fs(struct hci_ext_fs *cur,
		struct hci_ext_fs *new,
		struct hci_ext_fs *agg)
{
	*agg = *cur;
	if ((cur->max_sdu != 0xFFFF) && (cur->sdu_arr_time != 0xFFFFFFFF)) {
		/* current flow spec has known rate */
		if ((new->max_sdu == 0xFFFF) ||
				(new->sdu_arr_time == 0xFFFFFFFF)) {
			/* new fs has unknown rate, so aggregate is unknown */
			agg->max_sdu = 0xFFFF;
			agg->sdu_arr_time = 0xFFFFFFFF;
		} else {
			/* new fs has known rate, so aggregate is known */
			u64 cur_rate;
			u64 new_rate;
			cur_rate = cur->max_sdu * 1000000ULL;
			if (cur->sdu_arr_time)
				cur_rate = div_u64(cur_rate, cur->sdu_arr_time);
			new_rate = new->max_sdu * 1000000ULL;
			if (new->sdu_arr_time)
				new_rate = div_u64(new_rate, new->sdu_arr_time);
			cur_rate = cur_rate + new_rate;
			if (cur_rate)
				agg->sdu_arr_time = div64_u64(
					agg->max_sdu * 1000000ULL, cur_rate);
		}
	}
}

static int l2cap_aggregate(struct hci_chan *chan, struct l2cap_pinfo *pi)
{
	struct hci_ext_fs tx_fs;
	struct hci_ext_fs rx_fs;

	BT_DBG("chan %p", chan);

	if (((chan->tx_fs.max_sdu == 0xFFFF) ||
			(chan->tx_fs.sdu_arr_time == 0xFFFFFFFF)) &&
			((chan->rx_fs.max_sdu == 0xFFFF) ||
			(chan->rx_fs.sdu_arr_time == 0xFFFFFFFF)))
		return 0;

	l2cap_aggregate_fs(&chan->tx_fs,
				(struct hci_ext_fs *) &pi->local_fs, &tx_fs);
	l2cap_aggregate_fs(&chan->rx_fs,
				(struct hci_ext_fs *) &pi->remote_fs, &rx_fs);
	hci_chan_modify(chan, &tx_fs, &rx_fs);
	return 1;
}

static void l2cap_deaggregate_fs(struct hci_ext_fs *cur,
		struct hci_ext_fs *old,
		struct hci_ext_fs *agg)
{
	*agg = *cur;
	if ((cur->max_sdu != 0xFFFF) && (cur->sdu_arr_time != 0xFFFFFFFF)) {
		u64 cur_rate;
		u64 old_rate;
		cur_rate = cur->max_sdu * 1000000ULL;
		if (cur->sdu_arr_time)
			cur_rate = div_u64(cur_rate, cur->sdu_arr_time);
		old_rate = old->max_sdu * 1000000ULL;
		if (old->sdu_arr_time)
			old_rate = div_u64(old_rate, old->sdu_arr_time);
		cur_rate = cur_rate - old_rate;
		if (cur_rate)
			agg->sdu_arr_time = div64_u64(
				agg->max_sdu * 1000000ULL, cur_rate);
	}
}

static int l2cap_deaggregate(struct hci_chan *chan, struct l2cap_pinfo *pi)
{
	struct hci_ext_fs tx_fs;
	struct hci_ext_fs rx_fs;

	BT_DBG("chan %p", chan);

	if (((chan->tx_fs.max_sdu == 0xFFFF) ||
			(chan->tx_fs.sdu_arr_time == 0xFFFFFFFF)) &&
			((chan->rx_fs.max_sdu == 0xFFFF) ||
			(chan->rx_fs.sdu_arr_time == 0xFFFFFFFF)))
		return 0;

	l2cap_deaggregate_fs(&chan->tx_fs,
				(struct hci_ext_fs *) &pi->local_fs, &tx_fs);
	l2cap_deaggregate_fs(&chan->rx_fs,
				(struct hci_ext_fs *) &pi->remote_fs, &rx_fs);
	hci_chan_modify(chan, &tx_fs, &rx_fs);
	return 1;
}

static struct hci_chan *l2cap_chan_admit(u8 amp_id, struct sock *sk)
{
	struct l2cap_pinfo *pi = l2cap_pi(sk);
	struct hci_dev *hdev;
	struct hci_conn *hcon;
	struct hci_chan *chan;

	hdev = hci_dev_get(amp_id);
	if (!hdev)
		return NULL;

	BT_DBG("hdev %s", hdev->name);

	hcon = hci_conn_hash_lookup_ba(hdev, ACL_LINK, pi->conn->dst);
	if (!hcon) {
		chan = NULL;
		goto done;
	}

	chan = hci_chan_list_lookup_id(hdev, hcon->handle);
	if (chan) {
		l2cap_aggregate(chan, pi);
		sock_hold(sk);
		chan->l2cap_sk = sk;
		hci_chan_hold(chan);
		pi->ampchan = chan;
		goto done;
	}

	chan = hci_chan_add(hdev);
	if (chan) {
		chan->conn = hcon;
		sock_hold(sk);
		chan->l2cap_sk = sk;
		hci_chan_hold(chan);
		pi->ampchan = chan;
		hci_chan_create(chan,
			(struct hci_ext_fs *) &pi->local_fs,
			(struct hci_ext_fs *) &pi->remote_fs);
	}
done:
	hci_dev_put(hdev);
	return chan;
}

static void l2cap_get_ertm_timeouts(struct l2cap_conf_rfc *rfc,
						struct l2cap_pinfo *pi)
{
	if (pi->amp_id && pi->ampcon) {
		u64 ertm_to = pi->ampcon->hdev->amp_be_flush_to;

		/* Class 1 devices have must have ERTM timeouts
		 * exceeding the Link Supervision Timeout.  The
		 * default Link Supervision Timeout for AMP
		 * controllers is 10 seconds.
		 *
		 * Class 1 devices use 0xffffffff for their
		 * best-effort flush timeout, so the clamping logic
		 * will result in a timeout that meets the above
		 * requirement.  ERTM timeouts are 16-bit values, so
		 * the maximum timeout is 65.535 seconds.
		 */

		/* Convert timeout to milliseconds and round */
		ertm_to = div_u64(ertm_to + 999, 1000);

		/* This is the recommended formula for class 2 devices
		 * that start ERTM timers when packets are sent to the
		 * controller.
		 */
		ertm_to = 3 * ertm_to + 500;

		if (ertm_to > 0xffff)
			ertm_to = 0xffff;

		rfc->retrans_timeout = cpu_to_le16((u16) ertm_to);
		rfc->monitor_timeout = rfc->retrans_timeout;
	} else {
		rfc->retrans_timeout = cpu_to_le16(L2CAP_DEFAULT_RETRANS_TO);
		rfc->monitor_timeout = cpu_to_le16(L2CAP_DEFAULT_MONITOR_TO);
	}
}

int l2cap_build_conf_req(struct sock *sk, void *data, size_t data_size)
{
	struct l2cap_pinfo *pi = l2cap_pi(sk);
	struct l2cap_conf_req *req = data;
	struct l2cap_conf_rfc rfc = { .mode = pi->mode };
	void *ptr = req->data;
	void *endptr = data + data_size;

	BT_DBG("sk %p mode %d", sk, pi->mode);

	if (pi->num_conf_req || pi->num_conf_rsp)
		goto done;

	switch (pi->mode) {
	case L2CAP_MODE_STREAMING:
	case L2CAP_MODE_ERTM:
		if (pi->conf_state & L2CAP_CONF_STATE2_DEVICE)
			break;

		/* fall through */
	default:
		pi->mode = l2cap_select_mode(rfc.mode, pi->conn->feat_mask);
=======
static inline bool __l2cap_ews_supported(struct l2cap_conn *conn)
{
	return (conn->feat_mask & L2CAP_FEAT_EXT_WINDOW);
}

static inline bool __l2cap_efs_supported(struct l2cap_conn *conn)
{
	return (conn->feat_mask & L2CAP_FEAT_EXT_FLOW);
}

static void __l2cap_set_ertm_timeouts(struct l2cap_chan *chan,
				      struct l2cap_conf_rfc *rfc)
{
	rfc->retrans_timeout = cpu_to_le16(L2CAP_DEFAULT_RETRANS_TO);
	rfc->monitor_timeout = cpu_to_le16(L2CAP_DEFAULT_MONITOR_TO);
}

static inline void l2cap_txwin_setup(struct l2cap_chan *chan)
{
	if (chan->tx_win > L2CAP_DEFAULT_TX_WINDOW &&
	    __l2cap_ews_supported(chan->conn)) {
		/* use extended control field */
		set_bit(FLAG_EXT_CTRL, &chan->flags);
		chan->tx_win_max = L2CAP_DEFAULT_EXT_WINDOW;
	} else {
		chan->tx_win = min_t(u16, chan->tx_win,
				     L2CAP_DEFAULT_TX_WINDOW);
		chan->tx_win_max = L2CAP_DEFAULT_TX_WINDOW;
	}
	chan->ack_win = chan->tx_win;
}

static void l2cap_mtu_auto(struct l2cap_chan *chan)
{
	struct hci_conn *conn = chan->conn->hcon;

	chan->imtu = L2CAP_DEFAULT_MIN_MTU;

	/* The 2-DH1 packet has between 2 and 56 information bytes
	 * (including the 2-byte payload header)
	 */
	if (!(conn->pkt_type & HCI_2DH1))
		chan->imtu = 54;

	/* The 3-DH1 packet has between 2 and 85 information bytes
	 * (including the 2-byte payload header)
	 */
	if (!(conn->pkt_type & HCI_3DH1))
		chan->imtu = 83;

	/* The 2-DH3 packet has between 2 and 369 information bytes
	 * (including the 2-byte payload header)
	 */
	if (!(conn->pkt_type & HCI_2DH3))
		chan->imtu = 367;

	/* The 3-DH3 packet has between 2 and 554 information bytes
	 * (including the 2-byte payload header)
	 */
	if (!(conn->pkt_type & HCI_3DH3))
		chan->imtu = 552;

	/* The 2-DH5 packet has between 2 and 681 information bytes
	 * (including the 2-byte payload header)
	 */
	if (!(conn->pkt_type & HCI_2DH5))
		chan->imtu = 679;

	/* The 3-DH5 packet has between 2 and 1023 information bytes
	 * (including the 2-byte payload header)
	 */
	if (!(conn->pkt_type & HCI_3DH5))
		chan->imtu = 1021;
}

static int l2cap_build_conf_req(struct l2cap_chan *chan, void *data, size_t data_size)
{
	struct l2cap_conf_req *req = data;
	struct l2cap_conf_rfc rfc = { .mode = chan->mode };
	void *ptr = req->data;
	void *endptr = data + data_size;
	u16 size;

	BT_DBG("chan %p", chan);

	if (chan->num_conf_req || chan->num_conf_rsp)
		goto done;

	switch (chan->mode) {
	case L2CAP_MODE_STREAMING:
	case L2CAP_MODE_ERTM:
		if (test_bit(CONF_STATE2_DEVICE, &chan->conf_state))
			break;

		if (__l2cap_efs_supported(chan->conn))
			set_bit(FLAG_EFS_ENABLE, &chan->flags);

		fallthrough;
	default:
		chan->mode = l2cap_select_mode(rfc.mode, chan->conn->feat_mask);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	}

done:
<<<<<<< HEAD
	if (pi->imtu != L2CAP_DEFAULT_MTU)
		l2cap_add_conf_opt(&ptr, L2CAP_CONF_MTU, 2, pi->imtu, endptr - ptr);

	switch (pi->mode) {
	case L2CAP_MODE_BASIC:
		if (!(pi->conn->feat_mask & L2CAP_FEAT_ERTM) &&
				!(pi->conn->feat_mask & L2CAP_FEAT_STREAMING))
			break;
=======
	if (chan->imtu != L2CAP_DEFAULT_MTU) {
		if (!chan->imtu)
			l2cap_mtu_auto(chan);
		l2cap_add_conf_opt(&ptr, L2CAP_CONF_MTU, 2, chan->imtu,
				   endptr - ptr);
	}

	switch (chan->mode) {
	case L2CAP_MODE_BASIC:
		if (disable_ertm)
			break;

		if (!(chan->conn->feat_mask & L2CAP_FEAT_ERTM) &&
		    !(chan->conn->feat_mask & L2CAP_FEAT_STREAMING))
			break;

		rfc.mode            = L2CAP_MODE_BASIC;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		rfc.txwin_size      = 0;
		rfc.max_transmit    = 0;
		rfc.retrans_timeout = 0;
		rfc.monitor_timeout = 0;
		rfc.max_pdu_size    = 0;

		l2cap_add_conf_opt(&ptr, L2CAP_CONF_RFC, sizeof(rfc),
				   (unsigned long) &rfc, endptr - ptr);
		break;

	case L2CAP_MODE_ERTM:
<<<<<<< HEAD
		l2cap_setup_txwin(pi);
		if (pi->tx_win > L2CAP_TX_WIN_MAX_ENHANCED)
			rfc.txwin_size = L2CAP_TX_WIN_MAX_ENHANCED;
		else
			rfc.txwin_size = pi->tx_win;
		rfc.max_transmit = pi->max_tx;
		rfc.max_pdu_size = cpu_to_le16(L2CAP_DEFAULT_MAX_PDU_SIZE);
		l2cap_get_ertm_timeouts(&rfc, pi);

		if (L2CAP_DEFAULT_MAX_PDU_SIZE > pi->imtu)
			rfc.max_pdu_size = cpu_to_le16(pi->imtu);
=======
		rfc.mode            = L2CAP_MODE_ERTM;
		rfc.max_transmit    = chan->max_tx;

		__l2cap_set_ertm_timeouts(chan, &rfc);

		size = min_t(u16, L2CAP_DEFAULT_MAX_PDU_SIZE, chan->conn->mtu -
			     L2CAP_EXT_HDR_SIZE - L2CAP_SDULEN_SIZE -
			     L2CAP_FCS_SIZE);
		rfc.max_pdu_size = cpu_to_le16(size);

		l2cap_txwin_setup(chan);

		rfc.txwin_size = min_t(u16, chan->tx_win,
				       L2CAP_DEFAULT_TX_WINDOW);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		l2cap_add_conf_opt(&ptr, L2CAP_CONF_RFC, sizeof(rfc),
				   (unsigned long) &rfc, endptr - ptr);

<<<<<<< HEAD
		if ((pi->conn->feat_mask & L2CAP_FEAT_EXT_WINDOW) &&
			pi->extended_control) {
			l2cap_add_conf_opt(&ptr, L2CAP_CONF_EXT_WINDOW, 2,
					   pi->tx_win, endptr - ptr);
		}

		if (pi->amp_id) {
			/* default best effort extended flow spec */
			struct l2cap_conf_ext_fs fs = {1, 1, 0xFFFF,
					0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
			l2cap_add_conf_opt(&ptr, L2CAP_CONF_EXT_FS,
				sizeof(fs), (unsigned long) &fs, endptr - ptr);
		}

		if (!(pi->conn->feat_mask & L2CAP_FEAT_FCS))
			break;

		if (pi->fcs == L2CAP_FCS_NONE ||
				pi->conf_state & L2CAP_CONF_NO_FCS_RECV) {
			pi->fcs = L2CAP_FCS_NONE;
			l2cap_add_conf_opt(&ptr, L2CAP_CONF_FCS, 1, pi->fcs, endptr - ptr);
		}
		break;

	case L2CAP_MODE_STREAMING:
		l2cap_setup_txwin(pi);
=======
		if (test_bit(FLAG_EFS_ENABLE, &chan->flags))
			l2cap_add_opt_efs(&ptr, chan, endptr - ptr);

		if (test_bit(FLAG_EXT_CTRL, &chan->flags))
			l2cap_add_conf_opt(&ptr, L2CAP_CONF_EWS, 2,
					   chan->tx_win, endptr - ptr);

		if (chan->conn->feat_mask & L2CAP_FEAT_FCS)
			if (chan->fcs == L2CAP_FCS_NONE ||
			    test_bit(CONF_RECV_NO_FCS, &chan->conf_state)) {
				chan->fcs = L2CAP_FCS_NONE;
				l2cap_add_conf_opt(&ptr, L2CAP_CONF_FCS, 1,
						   chan->fcs, endptr - ptr);
			}
		break;

	case L2CAP_MODE_STREAMING:
		l2cap_txwin_setup(chan);
		rfc.mode            = L2CAP_MODE_STREAMING;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		rfc.txwin_size      = 0;
		rfc.max_transmit    = 0;
		rfc.retrans_timeout = 0;
		rfc.monitor_timeout = 0;
<<<<<<< HEAD
		rfc.max_pdu_size    = cpu_to_le16(L2CAP_DEFAULT_MAX_PDU_SIZE);
		if (L2CAP_DEFAULT_MAX_PDU_SIZE > pi->imtu)
			rfc.max_pdu_size = cpu_to_le16(pi->imtu);
=======

		size = min_t(u16, L2CAP_DEFAULT_MAX_PDU_SIZE, chan->conn->mtu -
			     L2CAP_EXT_HDR_SIZE - L2CAP_SDULEN_SIZE -
			     L2CAP_FCS_SIZE);
		rfc.max_pdu_size = cpu_to_le16(size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		l2cap_add_conf_opt(&ptr, L2CAP_CONF_RFC, sizeof(rfc),
				   (unsigned long) &rfc, endptr - ptr);

<<<<<<< HEAD
		if ((pi->conn->feat_mask & L2CAP_FEAT_EXT_WINDOW) &&
			pi->extended_control) {
			l2cap_add_conf_opt(&ptr, L2CAP_CONF_EXT_WINDOW, 2, 0, endptr - ptr);
		}

		if (!(pi->conn->feat_mask & L2CAP_FEAT_FCS))
			break;

		if (pi->fcs == L2CAP_FCS_NONE ||
				pi->conf_state & L2CAP_CONF_NO_FCS_RECV) {
			pi->fcs = L2CAP_FCS_NONE;
			l2cap_add_conf_opt(&ptr, L2CAP_CONF_FCS, 1, pi->fcs, endptr - ptr);
		}
		break;
	}

	req->dcid  = cpu_to_le16(pi->dcid);
=======
		if (test_bit(FLAG_EFS_ENABLE, &chan->flags))
			l2cap_add_opt_efs(&ptr, chan, endptr - ptr);

		if (chan->conn->feat_mask & L2CAP_FEAT_FCS)
			if (chan->fcs == L2CAP_FCS_NONE ||
			    test_bit(CONF_RECV_NO_FCS, &chan->conf_state)) {
				chan->fcs = L2CAP_FCS_NONE;
				l2cap_add_conf_opt(&ptr, L2CAP_CONF_FCS, 1,
						   chan->fcs, endptr - ptr);
			}
		break;
	}

	req->dcid  = cpu_to_le16(chan->dcid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	req->flags = cpu_to_le16(0);

	return ptr - data;
}

<<<<<<< HEAD

static int l2cap_build_amp_reconf_req(struct sock *sk, void *data, size_t data_size)
{
	struct l2cap_pinfo *pi = l2cap_pi(sk);
	struct l2cap_conf_req *req = data;
	struct l2cap_conf_rfc rfc = { .mode = pi->mode };
	void *ptr = req->data;
	void *endptr = data + data_size;

	BT_DBG("sk %p", sk);

	switch (pi->mode) {
	case L2CAP_MODE_ERTM:
		rfc.mode            = L2CAP_MODE_ERTM;
		rfc.txwin_size      = pi->tx_win;
		rfc.max_transmit    = pi->max_tx;
		rfc.max_pdu_size    = cpu_to_le16(L2CAP_DEFAULT_MAX_PDU_SIZE);
		l2cap_get_ertm_timeouts(&rfc, pi);
		if (L2CAP_DEFAULT_MAX_PDU_SIZE > pi->imtu)
			rfc.max_pdu_size = cpu_to_le16(pi->imtu);

		break;

	default:
		return -ECONNREFUSED;
	}

	l2cap_add_conf_opt(&ptr, L2CAP_CONF_RFC, sizeof(rfc),
			   (unsigned long) &rfc, endptr - ptr);

	if (pi->conn->feat_mask & L2CAP_FEAT_FCS) {
		/* TODO assign fcs for br/edr based on socket config option */
		/* FCS is not used with AMP because it is redundant - lower
		 * layers already include a checksum. */
		if (pi->amp_id)
			pi->local_conf.fcs = L2CAP_FCS_NONE;
		else
			pi->local_conf.fcs = L2CAP_FCS_CRC16;

		l2cap_add_conf_opt(&ptr, L2CAP_CONF_FCS, 1, pi->local_conf.fcs, endptr - ptr);
		pi->fcs = pi->local_conf.fcs | pi->remote_conf.fcs;
	}

	req->dcid  = cpu_to_le16(pi->dcid);
	req->flags = cpu_to_le16(0);

	return ptr - data;
}

static int l2cap_parse_conf_req(struct sock *sk, void *data, size_t data_size)
{
	struct l2cap_pinfo *pi = l2cap_pi(sk);
	struct l2cap_conf_rsp *rsp = data;
	void *ptr = rsp->data;
	void *endptr = data + data_size;
	void *req = pi->conf_req;
	int len = pi->conf_len;
	int type, hint, olen;
	unsigned long val;
	struct l2cap_conf_rfc rfc = { .mode = L2CAP_MODE_BASIC };
	struct l2cap_conf_ext_fs fs;
	u16 mtu = L2CAP_DEFAULT_MTU;
	u16 result = L2CAP_CONF_SUCCESS;

	BT_DBG("sk %p", sk);

	if (pi->omtu > mtu)
		mtu = pi->omtu;

	while (len >= L2CAP_CONF_OPT_SIZE) {
		len -= l2cap_get_conf_opt(&req, &type, &olen, &val);
=======
static int l2cap_parse_conf_req(struct l2cap_chan *chan, void *data, size_t data_size)
{
	struct l2cap_conf_rsp *rsp = data;
	void *ptr = rsp->data;
	void *endptr = data + data_size;
	void *req = chan->conf_req;
	int len = chan->conf_len;
	int type, hint, olen;
	unsigned long val;
	struct l2cap_conf_rfc rfc = { .mode = L2CAP_MODE_BASIC };
	struct l2cap_conf_efs efs;
	u8 remote_efs = 0;
	u16 mtu = L2CAP_DEFAULT_MTU;
	u16 result = L2CAP_CONF_SUCCESS;
	u16 size;

	BT_DBG("chan %p", chan);

	while (len >= L2CAP_CONF_OPT_SIZE) {
		len -= l2cap_get_conf_opt(&req, &type, &olen, &val);
		if (len < 0)
			break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		hint  = type & L2CAP_CONF_HINT;
		type &= L2CAP_CONF_MASK;

		switch (type) {
		case L2CAP_CONF_MTU:
<<<<<<< HEAD
=======
			if (olen != 2)
				break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			mtu = val;
			break;

		case L2CAP_CONF_FLUSH_TO:
<<<<<<< HEAD
			pi->flush_to = val;
			if (pi->conf_state & L2CAP_CONF_LOCKSTEP)
				result = L2CAP_CONF_UNACCEPT;
			else
				pi->remote_conf.flush_to = val;
			break;

		case L2CAP_CONF_QOS:
			if (pi->conf_state & L2CAP_CONF_LOCKSTEP)
				result = L2CAP_CONF_UNACCEPT;
			break;

		case L2CAP_CONF_RFC:
			if (olen == sizeof(rfc))
				memcpy(&rfc, (void *) val, olen);
			break;

		case L2CAP_CONF_FCS:
			if (val == L2CAP_FCS_NONE)
				pi->conf_state |= L2CAP_CONF_NO_FCS_RECV;
			pi->remote_conf.fcs = val;
			break;

		case L2CAP_CONF_EXT_FS:
			if (olen == sizeof(fs)) {
				pi->conf_state |= L2CAP_CONF_EFS_RECV;
				if (!(pi->conf_state & L2CAP_CONF_LOCKSTEP)) {
					result = L2CAP_CONF_UNACCEPT;
					break;
				}
				memcpy(&fs, (void *) val, olen);
				if (fs.type != L2CAP_SERVICE_BEST_EFFORT) {
					result = L2CAP_CONF_FLOW_SPEC_REJECT;
					break;
				}
				pi->remote_conf.flush_to =
						le32_to_cpu(fs.flush_to);
				pi->remote_fs.id = fs.id;
				pi->remote_fs.type = fs.type;
				pi->remote_fs.max_sdu =
						le16_to_cpu(fs.max_sdu);
				pi->remote_fs.sdu_arr_time =
						le32_to_cpu(fs.sdu_arr_time);
				pi->remote_fs.acc_latency =
						le32_to_cpu(fs.acc_latency);
				pi->remote_fs.flush_to =
						le32_to_cpu(fs.flush_to);
			}
			break;

		case L2CAP_CONF_EXT_WINDOW:
			pi->extended_control = 1;
			pi->remote_tx_win = val;
			pi->tx_win_max = L2CAP_TX_WIN_MAX_EXTENDED;
			pi->conf_state |= L2CAP_CONF_EXT_WIN_RECV;
			break;
=======
			if (olen != 2)
				break;
			chan->flush_to = val;
			break;

		case L2CAP_CONF_QOS:
			break;

		case L2CAP_CONF_RFC:
			if (olen != sizeof(rfc))
				break;
			memcpy(&rfc, (void *) val, olen);
			break;

		case L2CAP_CONF_FCS:
			if (olen != 1)
				break;
			if (val == L2CAP_FCS_NONE)
				set_bit(CONF_RECV_NO_FCS, &chan->conf_state);
			break;

		case L2CAP_CONF_EFS:
			if (olen != sizeof(efs))
				break;
			remote_efs = 1;
			memcpy(&efs, (void *) val, olen);
			break;

		case L2CAP_CONF_EWS:
			if (olen != 2)
				break;
			return -ECONNREFUSED;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		default:
			if (hint)
				break;
<<<<<<< HEAD

			result = L2CAP_CONF_UNKNOWN;
			*((u8 *) ptr++) = type;
=======
			result = L2CAP_CONF_UNKNOWN;
			l2cap_add_conf_opt(&ptr, (u8)type, sizeof(u8), type, endptr - ptr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		}
	}

<<<<<<< HEAD
	if (pi->num_conf_rsp || pi->num_conf_req > 1)
		goto done;

	switch (pi->mode) {
	case L2CAP_MODE_STREAMING:
	case L2CAP_MODE_ERTM:
		if (!(pi->conf_state & L2CAP_CONF_STATE2_DEVICE)) {
			pi->mode = l2cap_select_mode(rfc.mode,
					pi->conn->feat_mask);
			break;
		}

		if (pi->mode != rfc.mode)
=======
	if (chan->num_conf_rsp || chan->num_conf_req > 1)
		goto done;

	switch (chan->mode) {
	case L2CAP_MODE_STREAMING:
	case L2CAP_MODE_ERTM:
		if (!test_bit(CONF_STATE2_DEVICE, &chan->conf_state)) {
			chan->mode = l2cap_select_mode(rfc.mode,
						       chan->conn->feat_mask);
			break;
		}

		if (remote_efs) {
			if (__l2cap_efs_supported(chan->conn))
				set_bit(FLAG_EFS_ENABLE, &chan->flags);
			else
				return -ECONNREFUSED;
		}

		if (chan->mode != rfc.mode)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -ECONNREFUSED;

		break;
	}

done:
<<<<<<< HEAD
	if (pi->mode != rfc.mode) {
		result = L2CAP_CONF_UNACCEPT;
		rfc.mode = pi->mode;
		if (mtu > L2CAP_DEFAULT_MTU)
			pi->omtu = mtu;

		if (pi->num_conf_rsp == 1)
			return -ECONNREFUSED;

		l2cap_add_conf_opt(&ptr, L2CAP_CONF_RFC,
				   sizeof(rfc), (unsigned long) &rfc,
				   endptr - ptr);
	}


	if ((pi->conf_state & L2CAP_CONF_LOCKSTEP) &&
			!(pi->conf_state & L2CAP_CONF_EFS_RECV))
		return -ECONNREFUSED;

=======
	if (chan->mode != rfc.mode) {
		result = L2CAP_CONF_UNACCEPT;
		rfc.mode = chan->mode;

		if (chan->num_conf_rsp == 1)
			return -ECONNREFUSED;

		l2cap_add_conf_opt(&ptr, L2CAP_CONF_RFC, sizeof(rfc),
				   (unsigned long) &rfc, endptr - ptr);
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (result == L2CAP_CONF_SUCCESS) {
		/* Configure output options and let the other side know
		 * which ones we don't like. */

<<<<<<< HEAD
		if (mtu < L2CAP_DEFAULT_MIN_MTU) {
			result = L2CAP_CONF_UNACCEPT;
			pi->omtu = L2CAP_DEFAULT_MIN_MTU;
		} else {
			pi->omtu = mtu;
			pi->conf_state |= L2CAP_CONF_MTU_DONE;
		}
		l2cap_add_conf_opt(&ptr, L2CAP_CONF_MTU, 2, pi->omtu, endptr - ptr);

		switch (rfc.mode) {
		case L2CAP_MODE_BASIC:
			pi->fcs = L2CAP_FCS_NONE;
			pi->conf_state |= L2CAP_CONF_MODE_DONE;
			break;

		case L2CAP_MODE_ERTM:
			if (!(pi->conf_state & L2CAP_CONF_EXT_WIN_RECV))
				pi->remote_tx_win = rfc.txwin_size;
			pi->remote_max_tx = rfc.max_transmit;
			pi->remote_mps = le16_to_cpu(rfc.max_pdu_size);
			l2cap_get_ertm_timeouts(&rfc, pi);

			pi->conf_state |= L2CAP_CONF_MODE_DONE;

			l2cap_add_conf_opt(&ptr, L2CAP_CONF_RFC,
					sizeof(rfc), (unsigned long) &rfc, endptr - ptr);

			if (pi->conf_state & L2CAP_CONF_LOCKSTEP)
				l2cap_add_conf_opt(&ptr, L2CAP_CONF_EXT_FS,
					sizeof(fs), (unsigned long) &fs, endptr - ptr);

			break;

		case L2CAP_MODE_STREAMING:
			pi->remote_mps = le16_to_cpu(rfc.max_pdu_size);

			pi->conf_state |= L2CAP_CONF_MODE_DONE;

			l2cap_add_conf_opt(&ptr, L2CAP_CONF_RFC,
					sizeof(rfc), (unsigned long) &rfc, endptr - ptr);
=======
		if (mtu < L2CAP_DEFAULT_MIN_MTU)
			result = L2CAP_CONF_UNACCEPT;
		else {
			chan->omtu = mtu;
			set_bit(CONF_MTU_DONE, &chan->conf_state);
		}
		l2cap_add_conf_opt(&ptr, L2CAP_CONF_MTU, 2, chan->omtu, endptr - ptr);

		if (remote_efs) {
			if (chan->local_stype != L2CAP_SERV_NOTRAFIC &&
			    efs.stype != L2CAP_SERV_NOTRAFIC &&
			    efs.stype != chan->local_stype) {

				result = L2CAP_CONF_UNACCEPT;

				if (chan->num_conf_req >= 1)
					return -ECONNREFUSED;

				l2cap_add_conf_opt(&ptr, L2CAP_CONF_EFS,
						   sizeof(efs),
						   (unsigned long) &efs, endptr - ptr);
			} else {
				/* Send PENDING Conf Rsp */
				result = L2CAP_CONF_PENDING;
				set_bit(CONF_LOC_CONF_PEND, &chan->conf_state);
			}
		}

		switch (rfc.mode) {
		case L2CAP_MODE_BASIC:
			chan->fcs = L2CAP_FCS_NONE;
			set_bit(CONF_MODE_DONE, &chan->conf_state);
			break;

		case L2CAP_MODE_ERTM:
			if (!test_bit(CONF_EWS_RECV, &chan->conf_state))
				chan->remote_tx_win = rfc.txwin_size;
			else
				rfc.txwin_size = L2CAP_DEFAULT_TX_WINDOW;

			chan->remote_max_tx = rfc.max_transmit;

			size = min_t(u16, le16_to_cpu(rfc.max_pdu_size),
				     chan->conn->mtu - L2CAP_EXT_HDR_SIZE -
				     L2CAP_SDULEN_SIZE - L2CAP_FCS_SIZE);
			rfc.max_pdu_size = cpu_to_le16(size);
			chan->remote_mps = size;

			__l2cap_set_ertm_timeouts(chan, &rfc);

			set_bit(CONF_MODE_DONE, &chan->conf_state);

			l2cap_add_conf_opt(&ptr, L2CAP_CONF_RFC,
					   sizeof(rfc), (unsigned long) &rfc, endptr - ptr);

			if (remote_efs &&
			    test_bit(FLAG_EFS_ENABLE, &chan->flags)) {
				chan->remote_id = efs.id;
				chan->remote_stype = efs.stype;
				chan->remote_msdu = le16_to_cpu(efs.msdu);
				chan->remote_flush_to =
					le32_to_cpu(efs.flush_to);
				chan->remote_acc_lat =
					le32_to_cpu(efs.acc_lat);
				chan->remote_sdu_itime =
					le32_to_cpu(efs.sdu_itime);
				l2cap_add_conf_opt(&ptr, L2CAP_CONF_EFS,
						   sizeof(efs),
						   (unsigned long) &efs, endptr - ptr);
			}
			break;

		case L2CAP_MODE_STREAMING:
			size = min_t(u16, le16_to_cpu(rfc.max_pdu_size),
				     chan->conn->mtu - L2CAP_EXT_HDR_SIZE -
				     L2CAP_SDULEN_SIZE - L2CAP_FCS_SIZE);
			rfc.max_pdu_size = cpu_to_le16(size);
			chan->remote_mps = size;

			set_bit(CONF_MODE_DONE, &chan->conf_state);

			l2cap_add_conf_opt(&ptr, L2CAP_CONF_RFC, sizeof(rfc),
					   (unsigned long) &rfc, endptr - ptr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

			break;

		default:
			result = L2CAP_CONF_UNACCEPT;

			memset(&rfc, 0, sizeof(rfc));
<<<<<<< HEAD
			rfc.mode = pi->mode;
		}

		if (pi->conf_state & L2CAP_CONF_LOCKSTEP &&
				!(pi->conf_state & L2CAP_CONF_PEND_SENT)) {
			pi->conf_state |= L2CAP_CONF_PEND_SENT;
			result = L2CAP_CONF_PENDING;

			if (pi->conf_state & L2CAP_CONF_LOCKSTEP_PEND &&
					pi->amp_id) {
				struct hci_chan *chan;
				/* Trigger logical link creation only on AMP */

				chan = l2cap_chan_admit(pi->amp_id, sk);
				if (!chan)
					return -ECONNREFUSED;

				if (chan->state == BT_CONNECTED)
					l2cap_create_cfm(chan, 0);
			}
		}

		if (result == L2CAP_CONF_SUCCESS)
			pi->conf_state |= L2CAP_CONF_OUTPUT_DONE;
	}
	rsp->scid   = cpu_to_le16(pi->dcid);
	rsp->result = cpu_to_le16(result);
	rsp->flags  = cpu_to_le16(0x0000);
=======
			rfc.mode = chan->mode;
		}

		if (result == L2CAP_CONF_SUCCESS)
			set_bit(CONF_OUTPUT_DONE, &chan->conf_state);
	}
	rsp->scid   = cpu_to_le16(chan->dcid);
	rsp->result = cpu_to_le16(result);
	rsp->flags  = cpu_to_le16(0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ptr - data;
}

<<<<<<< HEAD
static int l2cap_parse_amp_move_reconf_req(struct sock *sk, void *data, size_t data_size)
{
	struct l2cap_pinfo *pi = l2cap_pi(sk);
	struct l2cap_conf_rsp *rsp = data;
	void *ptr = rsp->data;
	void *endptr = data + data_size;
	void *req = pi->conf_req;
	int len = pi->conf_len;
	int type, hint, olen;
	unsigned long val;
	struct l2cap_conf_rfc rfc = { .mode = L2CAP_MODE_BASIC };
	struct l2cap_conf_ext_fs fs;
	u16 mtu = pi->omtu;
	u16 tx_win = pi->remote_tx_win;
	u16 result = L2CAP_CONF_SUCCESS;

	BT_DBG("sk %p", sk);

	while (len >= L2CAP_CONF_OPT_SIZE) {
		len -= l2cap_get_conf_opt(&req, &type, &olen, &val);

		hint  = type & L2CAP_CONF_HINT;
		type &= L2CAP_CONF_MASK;

		switch (type) {
		case L2CAP_CONF_MTU:
			mtu = val;
			break;

		case L2CAP_CONF_FLUSH_TO:
			if (pi->amp_move_id)
				result = L2CAP_CONF_UNACCEPT;
			else
				pi->remote_conf.flush_to = val;
			break;

		case L2CAP_CONF_QOS:
			if (pi->amp_move_id)
				result = L2CAP_CONF_UNACCEPT;
			break;

		case L2CAP_CONF_RFC:
			if (olen == sizeof(rfc))
				memcpy(&rfc, (void *) val, olen);
			break;

		case L2CAP_CONF_FCS:
			pi->remote_conf.fcs = val;
			break;

		case L2CAP_CONF_EXT_FS:
			if (olen == sizeof(fs)) {
				memcpy(&fs, (void *) val, olen);
				if (fs.type != L2CAP_SERVICE_BEST_EFFORT)
					result = L2CAP_CONF_FLOW_SPEC_REJECT;
				else {
					pi->remote_conf.flush_to =
						le32_to_cpu(fs.flush_to);
				}
			}
			break;

		case L2CAP_CONF_EXT_WINDOW:
			tx_win = val;
			break;

		default:
			if (hint)
				break;

			result = L2CAP_CONF_UNKNOWN;
			*((u8 *) ptr++) = type;
			break;
			}
	}

	BT_DBG("result 0x%2.2x cur mode 0x%2.2x req  mode 0x%2.2x",
		result, pi->mode, rfc.mode);

	if (pi->mode != rfc.mode || rfc.mode == L2CAP_MODE_BASIC)
		result = L2CAP_CONF_UNACCEPT;

	if (result == L2CAP_CONF_SUCCESS) {
		/* Configure output options and let the other side know
		 * which ones we don't like. */

		/* Don't allow mtu to decrease. */
		if (mtu < pi->omtu)
			result = L2CAP_CONF_UNACCEPT;

		BT_DBG("mtu %d omtu %d", mtu, pi->omtu);

		l2cap_add_conf_opt(&ptr, L2CAP_CONF_MTU, 2, pi->omtu, endptr - ptr);

		/* Don't allow extended transmit window to change. */
		if (tx_win != pi->remote_tx_win) {
			result = L2CAP_CONF_UNACCEPT;
			l2cap_add_conf_opt(&ptr, L2CAP_CONF_EXT_WINDOW, 2,
					pi->remote_tx_win, endptr - ptr);
		}

		pi->remote_mps = rfc.max_pdu_size;

		if (rfc.mode == L2CAP_MODE_ERTM) {
			l2cap_get_ertm_timeouts(&rfc, pi);
		} else {
			rfc.retrans_timeout = 0;
			rfc.monitor_timeout = 0;
		}

		l2cap_add_conf_opt(&ptr, L2CAP_CONF_RFC,
					sizeof(rfc), (unsigned long) &rfc, endptr - ptr);
	}

	if (result != L2CAP_CONF_SUCCESS)
		goto done;

	pi->fcs = pi->remote_conf.fcs | pi->local_conf.fcs;

	if (pi->rx_state == L2CAP_ERTM_RX_STATE_WAIT_F_FLAG)
		pi->flush_to = pi->remote_conf.flush_to;

done:
	rsp->scid   = cpu_to_le16(pi->dcid);
	rsp->result = cpu_to_le16(result);
	rsp->flags  = cpu_to_le16(0x0000);

	return ptr - data;
}

static int l2cap_parse_conf_rsp(struct sock *sk, void *rsp, int len, void *data, size_t size, u16 *result)
{
	struct l2cap_pinfo *pi = l2cap_pi(sk);
=======
static int l2cap_parse_conf_rsp(struct l2cap_chan *chan, void *rsp, int len,
				void *data, size_t size, u16 *result)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct l2cap_conf_req *req = data;
	void *ptr = req->data;
	void *endptr = data + size;
	int type, olen;
	unsigned long val;
<<<<<<< HEAD
	struct l2cap_conf_rfc rfc;

	BT_DBG("sk %p, rsp %p, len %d, req %p", sk, rsp, len, data);

	/* Initialize rfc in case no rfc option is received */
	rfc.mode = pi->mode;
	rfc.retrans_timeout = cpu_to_le16(L2CAP_DEFAULT_RETRANS_TO);
	rfc.monitor_timeout = cpu_to_le16(L2CAP_DEFAULT_MONITOR_TO);
	rfc.max_pdu_size = cpu_to_le16(L2CAP_DEFAULT_MAX_PDU_SIZE);

	while (len >= L2CAP_CONF_OPT_SIZE) {
		len -= l2cap_get_conf_opt(&rsp, &type, &olen, &val);

		switch (type) {
		case L2CAP_CONF_MTU:
			if (val < L2CAP_DEFAULT_MIN_MTU) {
				*result = L2CAP_CONF_UNACCEPT;
				pi->imtu = L2CAP_DEFAULT_MIN_MTU;
			} else
				pi->imtu = val;
			l2cap_add_conf_opt(&ptr, L2CAP_CONF_MTU, 2, pi->imtu, endptr - ptr);
			break;

		case L2CAP_CONF_FLUSH_TO:
			pi->flush_to = val;
			l2cap_add_conf_opt(&ptr, L2CAP_CONF_FLUSH_TO,
							2, pi->flush_to, endptr - ptr);
			break;

		case L2CAP_CONF_RFC:
			if (olen == sizeof(rfc))
				memcpy(&rfc, (void *)val, olen);

			if ((pi->conf_state & L2CAP_CONF_STATE2_DEVICE) &&
							rfc.mode != pi->mode)
				return -ECONNREFUSED;

			pi->fcs = 0;

			l2cap_add_conf_opt(&ptr, L2CAP_CONF_RFC,
					sizeof(rfc), (unsigned long) &rfc, endptr - ptr);
			break;

		case L2CAP_CONF_EXT_WINDOW:
			pi->ack_win = min_t(u16, val, pi->ack_win);

			l2cap_add_conf_opt(&ptr, L2CAP_CONF_EXT_WINDOW,
					2, pi->tx_win, endptr - ptr);
			break;

		default:
=======
	struct l2cap_conf_rfc rfc = { .mode = L2CAP_MODE_BASIC };
	struct l2cap_conf_efs efs;

	BT_DBG("chan %p, rsp %p, len %d, req %p", chan, rsp, len, data);

	while (len >= L2CAP_CONF_OPT_SIZE) {
		len -= l2cap_get_conf_opt(&rsp, &type, &olen, &val);
		if (len < 0)
			break;

		switch (type) {
		case L2CAP_CONF_MTU:
			if (olen != 2)
				break;
			if (val < L2CAP_DEFAULT_MIN_MTU) {
				*result = L2CAP_CONF_UNACCEPT;
				chan->imtu = L2CAP_DEFAULT_MIN_MTU;
			} else
				chan->imtu = val;
			l2cap_add_conf_opt(&ptr, L2CAP_CONF_MTU, 2, chan->imtu,
					   endptr - ptr);
			break;

		case L2CAP_CONF_FLUSH_TO:
			if (olen != 2)
				break;
			chan->flush_to = val;
			l2cap_add_conf_opt(&ptr, L2CAP_CONF_FLUSH_TO, 2,
					   chan->flush_to, endptr - ptr);
			break;

		case L2CAP_CONF_RFC:
			if (olen != sizeof(rfc))
				break;
			memcpy(&rfc, (void *)val, olen);
			if (test_bit(CONF_STATE2_DEVICE, &chan->conf_state) &&
			    rfc.mode != chan->mode)
				return -ECONNREFUSED;
			chan->fcs = 0;
			l2cap_add_conf_opt(&ptr, L2CAP_CONF_RFC, sizeof(rfc),
					   (unsigned long) &rfc, endptr - ptr);
			break;

		case L2CAP_CONF_EWS:
			if (olen != 2)
				break;
			chan->ack_win = min_t(u16, val, chan->ack_win);
			l2cap_add_conf_opt(&ptr, L2CAP_CONF_EWS, 2,
					   chan->tx_win, endptr - ptr);
			break;

		case L2CAP_CONF_EFS:
			if (olen != sizeof(efs))
				break;
			memcpy(&efs, (void *)val, olen);
			if (chan->local_stype != L2CAP_SERV_NOTRAFIC &&
			    efs.stype != L2CAP_SERV_NOTRAFIC &&
			    efs.stype != chan->local_stype)
				return -ECONNREFUSED;
			l2cap_add_conf_opt(&ptr, L2CAP_CONF_EFS, sizeof(efs),
					   (unsigned long) &efs, endptr - ptr);
			break;

		case L2CAP_CONF_FCS:
			if (olen != 1)
				break;
			if (*result == L2CAP_CONF_PENDING)
				if (val == L2CAP_FCS_NONE)
					set_bit(CONF_RECV_NO_FCS,
						&chan->conf_state);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		}
	}

<<<<<<< HEAD
	if (pi->mode == L2CAP_MODE_BASIC && pi->mode != rfc.mode)
		return -ECONNREFUSED;

	pi->mode = rfc.mode;

	if (*result == L2CAP_CONF_SUCCESS) {
		switch (rfc.mode) {
		case L2CAP_MODE_ERTM:
			pi->retrans_timeout = le16_to_cpu(rfc.retrans_timeout);
			pi->monitor_timeout = le16_to_cpu(rfc.monitor_timeout);
			pi->mps    = le16_to_cpu(rfc.max_pdu_size);
			if (!pi->extended_control) {
				pi->ack_win = min_t(u16, pi->ack_win,
						    rfc.txwin_size);
			}
			break;
		case L2CAP_MODE_STREAMING:
			pi->mps    = le16_to_cpu(rfc.max_pdu_size);
		}
	}

	req->dcid   = cpu_to_le16(pi->dcid);
	req->flags  = cpu_to_le16(0x0000);
=======
	if (chan->mode == L2CAP_MODE_BASIC && chan->mode != rfc.mode)
		return -ECONNREFUSED;

	chan->mode = rfc.mode;

	if (*result == L2CAP_CONF_SUCCESS || *result == L2CAP_CONF_PENDING) {
		switch (rfc.mode) {
		case L2CAP_MODE_ERTM:
			chan->retrans_timeout = le16_to_cpu(rfc.retrans_timeout);
			chan->monitor_timeout = le16_to_cpu(rfc.monitor_timeout);
			chan->mps    = le16_to_cpu(rfc.max_pdu_size);
			if (!test_bit(FLAG_EXT_CTRL, &chan->flags))
				chan->ack_win = min_t(u16, chan->ack_win,
						      rfc.txwin_size);

			if (test_bit(FLAG_EFS_ENABLE, &chan->flags)) {
				chan->local_msdu = le16_to_cpu(efs.msdu);
				chan->local_sdu_itime =
					le32_to_cpu(efs.sdu_itime);
				chan->local_acc_lat = le32_to_cpu(efs.acc_lat);
				chan->local_flush_to =
					le32_to_cpu(efs.flush_to);
			}
			break;

		case L2CAP_MODE_STREAMING:
			chan->mps    = le16_to_cpu(rfc.max_pdu_size);
		}
	}

	req->dcid   = cpu_to_le16(chan->dcid);
	req->flags  = cpu_to_le16(0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ptr - data;
}

<<<<<<< HEAD
static int l2cap_build_conf_rsp(struct sock *sk, void *data, u16 result, u16 flags)
=======
static int l2cap_build_conf_rsp(struct l2cap_chan *chan, void *data,
				u16 result, u16 flags)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct l2cap_conf_rsp *rsp = data;
	void *ptr = rsp->data;

<<<<<<< HEAD
	BT_DBG("sk %p", sk);

	rsp->scid   = cpu_to_le16(l2cap_pi(sk)->dcid);
=======
	BT_DBG("chan %p", chan);

	rsp->scid   = cpu_to_le16(chan->dcid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rsp->result = cpu_to_le16(result);
	rsp->flags  = cpu_to_le16(flags);

	return ptr - data;
}

<<<<<<< HEAD
static void l2cap_conf_rfc_get(struct sock *sk, void *rsp, int len)
{
	struct l2cap_pinfo *pi = l2cap_pi(sk);
	int type, olen;
	unsigned long val;
	struct l2cap_conf_rfc rfc;
	u16 txwin_ext = pi->ack_win;

	BT_DBG("sk %p, rsp %p, len %d", sk, rsp, len);

	/* Initialize rfc in case no rfc option is received */
	rfc.mode = pi->mode;
	rfc.retrans_timeout = cpu_to_le16(L2CAP_DEFAULT_RETRANS_TO);
	rfc.monitor_timeout = cpu_to_le16(L2CAP_DEFAULT_MONITOR_TO);
	rfc.max_pdu_size = cpu_to_le16(L2CAP_DEFAULT_MAX_PDU_SIZE);
	rfc.txwin_size = min_t(u16, pi->ack_win, L2CAP_DEFAULT_TX_WINDOW);

	if ((pi->mode != L2CAP_MODE_ERTM) && (pi->mode != L2CAP_MODE_STREAMING))
=======
void __l2cap_le_connect_rsp_defer(struct l2cap_chan *chan)
{
	struct l2cap_le_conn_rsp rsp;
	struct l2cap_conn *conn = chan->conn;

	BT_DBG("chan %p", chan);

	rsp.dcid    = cpu_to_le16(chan->scid);
	rsp.mtu     = cpu_to_le16(chan->imtu);
	rsp.mps     = cpu_to_le16(chan->mps);
	rsp.credits = cpu_to_le16(chan->rx_credits);
	rsp.result  = cpu_to_le16(L2CAP_CR_LE_SUCCESS);

	l2cap_send_cmd(conn, chan->ident, L2CAP_LE_CONN_RSP, sizeof(rsp),
		       &rsp);
}

static void l2cap_ecred_list_defer(struct l2cap_chan *chan, void *data)
{
	int *result = data;

	if (*result || test_bit(FLAG_ECRED_CONN_REQ_SENT, &chan->flags))
		return;

	switch (chan->state) {
	case BT_CONNECT2:
		/* If channel still pending accept add to result */
		(*result)++;
		return;
	case BT_CONNECTED:
		return;
	default:
		/* If not connected or pending accept it has been refused */
		*result = -ECONNREFUSED;
		return;
	}
}

struct l2cap_ecred_rsp_data {
	struct {
		struct l2cap_ecred_conn_rsp rsp;
		__le16 scid[L2CAP_ECRED_MAX_CID];
	} __packed pdu;
	int count;
};

static void l2cap_ecred_rsp_defer(struct l2cap_chan *chan, void *data)
{
	struct l2cap_ecred_rsp_data *rsp = data;

	if (test_bit(FLAG_ECRED_CONN_REQ_SENT, &chan->flags))
		return;

	/* Reset ident so only one response is sent */
	chan->ident = 0;

	/* Include all channels pending with the same ident */
	if (!rsp->pdu.rsp.result)
		rsp->pdu.rsp.dcid[rsp->count++] = cpu_to_le16(chan->scid);
	else
		l2cap_chan_del(chan, ECONNRESET);
}

void __l2cap_ecred_conn_rsp_defer(struct l2cap_chan *chan)
{
	struct l2cap_conn *conn = chan->conn;
	struct l2cap_ecred_rsp_data data;
	u16 id = chan->ident;
	int result = 0;

	if (!id)
		return;

	BT_DBG("chan %p id %d", chan, id);

	memset(&data, 0, sizeof(data));

	data.pdu.rsp.mtu     = cpu_to_le16(chan->imtu);
	data.pdu.rsp.mps     = cpu_to_le16(chan->mps);
	data.pdu.rsp.credits = cpu_to_le16(chan->rx_credits);
	data.pdu.rsp.result  = cpu_to_le16(L2CAP_CR_LE_SUCCESS);

	/* Verify that all channels are ready */
	__l2cap_chan_list_id(conn, id, l2cap_ecred_list_defer, &result);

	if (result > 0)
		return;

	if (result < 0)
		data.pdu.rsp.result = cpu_to_le16(L2CAP_CR_LE_AUTHORIZATION);

	/* Build response */
	__l2cap_chan_list_id(conn, id, l2cap_ecred_rsp_defer, &data);

	l2cap_send_cmd(conn, id, L2CAP_ECRED_CONN_RSP,
		       sizeof(data.pdu.rsp) + (data.count * sizeof(__le16)),
		       &data.pdu);
}

void __l2cap_connect_rsp_defer(struct l2cap_chan *chan)
{
	struct l2cap_conn_rsp rsp;
	struct l2cap_conn *conn = chan->conn;
	u8 buf[128];
	u8 rsp_code;

	rsp.scid   = cpu_to_le16(chan->dcid);
	rsp.dcid   = cpu_to_le16(chan->scid);
	rsp.result = cpu_to_le16(L2CAP_CR_SUCCESS);
	rsp.status = cpu_to_le16(L2CAP_CS_NO_INFO);
	rsp_code = L2CAP_CONN_RSP;

	BT_DBG("chan %p rsp_code %u", chan, rsp_code);

	l2cap_send_cmd(conn, chan->ident, rsp_code, sizeof(rsp), &rsp);

	if (test_and_set_bit(CONF_REQ_SENT, &chan->conf_state))
		return;

	l2cap_send_cmd(conn, l2cap_get_ident(conn), L2CAP_CONF_REQ,
		       l2cap_build_conf_req(chan, buf, sizeof(buf)), buf);
	chan->num_conf_req++;
}

static void l2cap_conf_rfc_get(struct l2cap_chan *chan, void *rsp, int len)
{
	int type, olen;
	unsigned long val;
	/* Use sane default values in case a misbehaving remote device
	 * did not send an RFC or extended window size option.
	 */
	u16 txwin_ext = chan->ack_win;
	struct l2cap_conf_rfc rfc = {
		.mode = chan->mode,
		.retrans_timeout = cpu_to_le16(L2CAP_DEFAULT_RETRANS_TO),
		.monitor_timeout = cpu_to_le16(L2CAP_DEFAULT_MONITOR_TO),
		.max_pdu_size = cpu_to_le16(chan->imtu),
		.txwin_size = min_t(u16, chan->ack_win, L2CAP_DEFAULT_TX_WINDOW),
	};

	BT_DBG("chan %p, rsp %p, len %d", chan, rsp, len);

	if ((chan->mode != L2CAP_MODE_ERTM) && (chan->mode != L2CAP_MODE_STREAMING))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;

	while (len >= L2CAP_CONF_OPT_SIZE) {
		len -= l2cap_get_conf_opt(&rsp, &type, &olen, &val);
<<<<<<< HEAD

		switch (type) {
		case L2CAP_CONF_RFC:
			if (olen == sizeof(rfc))
				memcpy(&rfc, (void *)val, olen);
			break;
		case L2CAP_CONF_EXT_WINDOW:
=======
		if (len < 0)
			break;

		switch (type) {
		case L2CAP_CONF_RFC:
			if (olen != sizeof(rfc))
				break;
			memcpy(&rfc, (void *)val, olen);
			break;
		case L2CAP_CONF_EWS:
			if (olen != 2)
				break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			txwin_ext = val;
			break;
		}
	}

	switch (rfc.mode) {
	case L2CAP_MODE_ERTM:
<<<<<<< HEAD
		pi->retrans_timeout = le16_to_cpu(rfc.retrans_timeout);
		pi->monitor_timeout = le16_to_cpu(rfc.monitor_timeout);
		pi->mps    = le16_to_cpu(rfc.max_pdu_size);
		if (pi->extended_control)
			pi->ack_win = min_t(u16, pi->ack_win, txwin_ext);
		else
			pi->ack_win = min_t(u16, pi->ack_win, rfc.txwin_size);
		break;
	case L2CAP_MODE_STREAMING:
		pi->mps    = le16_to_cpu(rfc.max_pdu_size);
	}
}

static void l2cap_conf_ext_fs_get(struct sock *sk, void *rsp, int len)
{
	struct l2cap_pinfo *pi = l2cap_pi(sk);
	int type, olen;
	unsigned long val;
	struct l2cap_conf_ext_fs fs;

	BT_DBG("sk %p, rsp %p, len %d", sk, rsp, len);

	while (len >= L2CAP_CONF_OPT_SIZE) {
		len -= l2cap_get_conf_opt(&rsp, &type, &olen, &val);
		if ((type == L2CAP_CONF_EXT_FS) &&
				(olen == sizeof(struct l2cap_conf_ext_fs))) {
			memcpy(&fs, (void *)val, olen);
			pi->local_fs.id = fs.id;
			pi->local_fs.type = fs.type;
			pi->local_fs.max_sdu = le16_to_cpu(fs.max_sdu);
			pi->local_fs.sdu_arr_time =
						le32_to_cpu(fs.sdu_arr_time);
			pi->local_fs.acc_latency = le32_to_cpu(fs.acc_latency);
			pi->local_fs.flush_to = le32_to_cpu(fs.flush_to);
			break;
		}
	}

}

static int l2cap_finish_amp_move(struct sock *sk)
{
	struct l2cap_pinfo *pi;
	int err;

	BT_DBG("sk %p", sk);

	pi = l2cap_pi(sk);

	pi->amp_move_role = L2CAP_AMP_MOVE_NONE;
	pi->rx_state = L2CAP_ERTM_RX_STATE_RECV;

	if (pi->ampcon)
		pi->conn->mtu = pi->ampcon->hdev->acl_mtu;
	else
		pi->conn->mtu = pi->conn->hcon->hdev->acl_mtu;

	err = l2cap_setup_resegment(sk);

	return err;
}

static int l2cap_amp_move_reconf_rsp(struct sock *sk, void *rsp, int len,
					u16 result)
{
	int err = 0;
	struct l2cap_conf_rfc rfc = {.mode = L2CAP_MODE_BASIC};
	struct l2cap_pinfo *pi = l2cap_pi(sk);

	BT_DBG("sk %p, rsp %p, len %d, res 0x%2.2x", sk, rsp, len, result);

	if (pi->reconf_state == L2CAP_RECONF_NONE)
		return -ECONNREFUSED;

	if (result == L2CAP_CONF_SUCCESS) {
		while (len >= L2CAP_CONF_OPT_SIZE) {
			int type, olen;
			unsigned long val;

			len -= l2cap_get_conf_opt(&rsp, &type, &olen, &val);

			if (type == L2CAP_CONF_RFC) {
				if (olen == sizeof(rfc))
					memcpy(&rfc, (void *)val, olen);

				if (rfc.mode != pi->mode) {
					l2cap_send_disconn_req(pi->conn, sk,
								ECONNRESET);
					return -ECONNRESET;
				}

				goto done;
			}
		}
	}

	BT_ERR("Expected RFC option was missing, using existing values");

	rfc.mode = pi->mode;
	rfc.retrans_timeout = cpu_to_le16(pi->retrans_timeout);
	rfc.monitor_timeout = cpu_to_le16(pi->monitor_timeout);

done:
	l2cap_ertm_stop_ack_timer(pi);
	l2cap_ertm_stop_retrans_timer(pi);
	l2cap_ertm_stop_monitor_timer(pi);

	pi->mps = le16_to_cpu(rfc.max_pdu_size);
	if (pi->mode == L2CAP_MODE_ERTM) {
		pi->retrans_timeout = le16_to_cpu(rfc.retrans_timeout);
		pi->monitor_timeout = le16_to_cpu(rfc.monitor_timeout);
	}

	if (l2cap_pi(sk)->reconf_state == L2CAP_RECONF_ACC) {
		l2cap_pi(sk)->reconf_state = L2CAP_RECONF_NONE;

		/* Respond to poll */
		err = l2cap_answer_move_poll(sk);
	} else if (l2cap_pi(sk)->reconf_state == L2CAP_RECONF_INT) {
		if (pi->mode == L2CAP_MODE_ERTM) {
			l2cap_ertm_tx(sk, NULL, NULL,
					L2CAP_ERTM_EVENT_EXPLICIT_POLL);
			pi->rx_state = L2CAP_ERTM_RX_STATE_WAIT_F_FLAG;
		}
	}

	return err;
}

=======
		chan->retrans_timeout = le16_to_cpu(rfc.retrans_timeout);
		chan->monitor_timeout = le16_to_cpu(rfc.monitor_timeout);
		chan->mps = le16_to_cpu(rfc.max_pdu_size);
		if (test_bit(FLAG_EXT_CTRL, &chan->flags))
			chan->ack_win = min_t(u16, chan->ack_win, txwin_ext);
		else
			chan->ack_win = min_t(u16, chan->ack_win,
					      rfc.txwin_size);
		break;
	case L2CAP_MODE_STREAMING:
		chan->mps    = le16_to_cpu(rfc.max_pdu_size);
	}
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline int l2cap_command_rej(struct l2cap_conn *conn,
				    struct l2cap_cmd_hdr *cmd, u16 cmd_len,
				    u8 *data)
{
<<<<<<< HEAD
	struct l2cap_cmd_rej *rej = (struct l2cap_cmd_rej *) data;
=======
	struct l2cap_cmd_rej_unk *rej = (struct l2cap_cmd_rej_unk *) data;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (cmd_len < sizeof(*rej))
		return -EPROTO;

<<<<<<< HEAD
	if (rej->reason != 0x0000)
		return 0;

	if ((conn->info_state & L2CAP_INFO_FEAT_MASK_REQ_SENT) &&
					cmd->ident == conn->info_ident) {
		del_timer(&conn->info_timer);
=======
	if (rej->reason != L2CAP_REJ_NOT_UNDERSTOOD)
		return 0;

	if ((conn->info_state & L2CAP_INFO_FEAT_MASK_REQ_SENT) &&
	    cmd->ident == conn->info_ident) {
		cancel_delayed_work(&conn->info_timer);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		conn->info_state |= L2CAP_INFO_FEAT_MASK_REQ_DONE;
		conn->info_ident = 0;

		l2cap_conn_start(conn);
	}

	return 0;
}

<<<<<<< HEAD
static struct sock *l2cap_create_connect(struct l2cap_conn *conn,
						struct l2cap_cmd_hdr *cmd,
						u8 *data, u8 rsp_code,
						u8 amp_id)
{
	struct l2cap_chan_list *list = &conn->chan_list;
	struct l2cap_conn_req *req = (struct l2cap_conn_req *) data;
	struct l2cap_conn_rsp rsp;
	struct sock *parent, *sk = NULL;
=======
static void l2cap_connect(struct l2cap_conn *conn, struct l2cap_cmd_hdr *cmd,
			  u8 *data, u8 rsp_code, u8 amp_id)
{
	struct l2cap_conn_req *req = (struct l2cap_conn_req *) data;
	struct l2cap_conn_rsp rsp;
	struct l2cap_chan *chan = NULL, *pchan = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int result, status = L2CAP_CS_NO_INFO;

	u16 dcid = 0, scid = __le16_to_cpu(req->scid);
	__le16 psm = req->psm;

<<<<<<< HEAD
	BT_DBG("psm 0x%2.2x scid 0x%4.4x", psm, scid);

	/* Check if we have socket listening on psm */
	parent = l2cap_get_sock_by_psm(BT_LISTEN, psm, conn->src);
	if (!parent) {
		result = L2CAP_CR_BAD_PSM;
		goto sendresp;
	}

	bh_lock_sock(parent);

	/* Check if the ACL is secure enough (if not SDP) */
	if (psm != cpu_to_le16(0x0001) &&
				!hci_conn_check_link_mode(conn->hcon)) {
		conn->disc_reason = 0x05;
=======
	BT_DBG("psm 0x%2.2x scid 0x%4.4x", __le16_to_cpu(psm), scid);

	/* Check if we have socket listening on psm */
	pchan = l2cap_global_chan_by_psm(BT_LISTEN, psm, &conn->hcon->src,
					 &conn->hcon->dst, ACL_LINK);
	if (!pchan) {
		result = L2CAP_CR_BAD_PSM;
		goto response;
	}

	mutex_lock(&conn->chan_lock);
	l2cap_chan_lock(pchan);

	/* Check if the ACL is secure enough (if not SDP) */
	if (psm != cpu_to_le16(L2CAP_PSM_SDP) &&
	    !hci_conn_check_link_mode(conn->hcon)) {
		conn->disc_reason = HCI_ERROR_AUTH_FAILURE;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		result = L2CAP_CR_SEC_BLOCK;
		goto response;
	}

	result = L2CAP_CR_NO_MEM;

<<<<<<< HEAD
	/* Check for backlog size */
	if (sk_acceptq_is_full(parent)) {
		BT_DBG("backlog full %d", parent->sk_ack_backlog);
		goto response;
	}

	sk = l2cap_sock_alloc(sock_net(parent), NULL, BTPROTO_L2CAP, GFP_ATOMIC);
	if (!sk)
		goto response;

	write_lock_bh(&list->lock);

	/* Check if we already have channel with that dcid */
	if (__l2cap_get_chan_by_dcid(list, scid)) {
		write_unlock_bh(&list->lock);
		sock_set_flag(sk, SOCK_ZAPPED);
		l2cap_sock_kill(sk);
		sk = NULL;
		goto response;
	}

	hci_conn_hold(conn->hcon);

	l2cap_sock_init(sk, parent);
	bacpy(&bt_sk(sk)->src, conn->src);
	bacpy(&bt_sk(sk)->dst, conn->dst);
	l2cap_pi(sk)->psm  = psm;
	l2cap_pi(sk)->dcid = scid;

	bt_accept_enqueue(parent, sk);

	__l2cap_chan_add(conn, sk);
	dcid = l2cap_pi(sk)->scid;
	l2cap_pi(sk)->amp_id = amp_id;

	l2cap_sock_set_timer(sk, sk->sk_sndtimeo);

	l2cap_pi(sk)->ident = cmd->ident;

	if (conn->info_state & L2CAP_INFO_FEAT_MASK_REQ_DONE) {
		if (l2cap_check_security(sk)) {
			if (bt_sk(sk)->defer_setup) {
				sk->sk_state = BT_CONNECT2;
				result = L2CAP_CR_PEND;
				status = L2CAP_CS_AUTHOR_PEND;
				parent->sk_data_ready(parent, 0);
			} else {
				/* Force pending result for AMP controllers.
				 * The connection will succeed after the
				 * physical link is up. */
				if (amp_id) {
					sk->sk_state = BT_CONNECT2;
					result = L2CAP_CR_PEND;
				} else {
					sk->sk_state = BT_CONFIG;
					result = L2CAP_CR_SUCCESS;
=======
	/* Check for valid dynamic CID range (as per Erratum 3253) */
	if (scid < L2CAP_CID_DYN_START || scid > L2CAP_CID_DYN_END) {
		result = L2CAP_CR_INVALID_SCID;
		goto response;
	}

	/* Check if we already have channel with that dcid */
	if (__l2cap_get_chan_by_dcid(conn, scid)) {
		result = L2CAP_CR_SCID_IN_USE;
		goto response;
	}

	chan = pchan->ops->new_connection(pchan);
	if (!chan)
		goto response;

	/* For certain devices (ex: HID mouse), support for authentication,
	 * pairing and bonding is optional. For such devices, inorder to avoid
	 * the ACL alive for too long after L2CAP disconnection, reset the ACL
	 * disc_timeout back to HCI_DISCONN_TIMEOUT during L2CAP connect.
	 */
	conn->hcon->disc_timeout = HCI_DISCONN_TIMEOUT;

	bacpy(&chan->src, &conn->hcon->src);
	bacpy(&chan->dst, &conn->hcon->dst);
	chan->src_type = bdaddr_src_type(conn->hcon);
	chan->dst_type = bdaddr_dst_type(conn->hcon);
	chan->psm  = psm;
	chan->dcid = scid;

	__l2cap_chan_add(conn, chan);

	dcid = chan->scid;

	__set_chan_timer(chan, chan->ops->get_sndtimeo(chan));

	chan->ident = cmd->ident;

	if (conn->info_state & L2CAP_INFO_FEAT_MASK_REQ_DONE) {
		if (l2cap_chan_check_security(chan, false)) {
			if (test_bit(FLAG_DEFER_SETUP, &chan->flags)) {
				l2cap_state_change(chan, BT_CONNECT2);
				result = L2CAP_CR_PEND;
				status = L2CAP_CS_AUTHOR_PEND;
				chan->ops->defer(chan);
			} else {
				/* Force pending result for AMP controllers.
				 * The connection will succeed after the
				 * physical link is up.
				 */
				if (amp_id == AMP_ID_BREDR) {
					l2cap_state_change(chan, BT_CONFIG);
					result = L2CAP_CR_SUCCESS;
				} else {
					l2cap_state_change(chan, BT_CONNECT2);
					result = L2CAP_CR_PEND;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				}
				status = L2CAP_CS_NO_INFO;
			}
		} else {
<<<<<<< HEAD
			sk->sk_state = BT_CONNECT2;
=======
			l2cap_state_change(chan, BT_CONNECT2);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			result = L2CAP_CR_PEND;
			status = L2CAP_CS_AUTHEN_PEND;
		}
	} else {
<<<<<<< HEAD
		sk->sk_state = BT_CONNECT2;
=======
		l2cap_state_change(chan, BT_CONNECT2);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		result = L2CAP_CR_PEND;
		status = L2CAP_CS_NO_INFO;
	}

<<<<<<< HEAD
	write_unlock_bh(&list->lock);

response:
	bh_unlock_sock(parent);

sendresp:
=======
response:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rsp.scid   = cpu_to_le16(scid);
	rsp.dcid   = cpu_to_le16(dcid);
	rsp.result = cpu_to_le16(result);
	rsp.status = cpu_to_le16(status);
	l2cap_send_cmd(conn, cmd->ident, rsp_code, sizeof(rsp), &rsp);

<<<<<<< HEAD
	if (!(conn->info_state & L2CAP_INFO_FEAT_MASK_REQ_DONE)) {
=======
	if (!pchan)
		return;

	if (result == L2CAP_CR_PEND && status == L2CAP_CS_NO_INFO) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		struct l2cap_info_req info;
		info.type = cpu_to_le16(L2CAP_IT_FEAT_MASK);

		conn->info_state |= L2CAP_INFO_FEAT_MASK_REQ_SENT;
		conn->info_ident = l2cap_get_ident(conn);

<<<<<<< HEAD
		mod_timer(&conn->info_timer, jiffies +
					msecs_to_jiffies(L2CAP_INFO_TIMEOUT));

		l2cap_send_cmd(conn, conn->info_ident,
					L2CAP_INFO_REQ, sizeof(info), &info);
	}

	if (sk && !(l2cap_pi(sk)->conf_state & L2CAP_CONF_REQ_SENT) &&
				result == L2CAP_CR_SUCCESS) {
		u8 buf[128];
		l2cap_pi(sk)->conf_state |= L2CAP_CONF_REQ_SENT;
		l2cap_send_cmd(conn, l2cap_get_ident(conn), L2CAP_CONF_REQ,
					l2cap_build_conf_req(sk, buf, sizeof(buf)), buf);
		l2cap_pi(sk)->num_conf_req++;
	}

	return sk;
=======
		schedule_delayed_work(&conn->info_timer, L2CAP_INFO_TIMEOUT);

		l2cap_send_cmd(conn, conn->info_ident, L2CAP_INFO_REQ,
			       sizeof(info), &info);
	}

	if (chan && !test_bit(CONF_REQ_SENT, &chan->conf_state) &&
	    result == L2CAP_CR_SUCCESS) {
		u8 buf[128];
		set_bit(CONF_REQ_SENT, &chan->conf_state);
		l2cap_send_cmd(conn, l2cap_get_ident(conn), L2CAP_CONF_REQ,
			       l2cap_build_conf_req(chan, buf, sizeof(buf)), buf);
		chan->num_conf_req++;
	}

	l2cap_chan_unlock(pchan);
	mutex_unlock(&conn->chan_lock);
	l2cap_chan_put(pchan);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int l2cap_connect_req(struct l2cap_conn *conn,
			     struct l2cap_cmd_hdr *cmd, u16 cmd_len, u8 *data)
{
<<<<<<< HEAD
	if (cmd_len < sizeof(struct l2cap_conn_req))
		return -EPROTO;

	l2cap_create_connect(conn, cmd, data, L2CAP_CONN_RSP, 0);
	return 0;
}

static int l2cap_connect_rsp(struct l2cap_conn *conn,
			      struct l2cap_cmd_hdr *cmd, u16 cmd_len,
			      u8 *data)
{
	struct l2cap_conn_rsp *rsp = (struct l2cap_conn_rsp *) data;
	u16 scid, dcid, result, status;
	struct sock *sk;
	u8 req[128];
=======
	struct hci_dev *hdev = conn->hcon->hdev;
	struct hci_conn *hcon = conn->hcon;

	if (cmd_len < sizeof(struct l2cap_conn_req))
		return -EPROTO;

	hci_dev_lock(hdev);
	if (hci_dev_test_flag(hdev, HCI_MGMT))
		mgmt_device_connected(hdev, hcon, NULL, 0);
	hci_dev_unlock(hdev);

	l2cap_connect(conn, cmd, data, L2CAP_CONN_RSP, 0);
	return 0;
}

static int l2cap_connect_create_rsp(struct l2cap_conn *conn,
				    struct l2cap_cmd_hdr *cmd, u16 cmd_len,
				    u8 *data)
{
	struct l2cap_conn_rsp *rsp = (struct l2cap_conn_rsp *) data;
	u16 scid, dcid, result, status;
	struct l2cap_chan *chan;
	u8 req[128];
	int err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (cmd_len < sizeof(*rsp))
		return -EPROTO;

	scid   = __le16_to_cpu(rsp->scid);
	dcid   = __le16_to_cpu(rsp->dcid);
	result = __le16_to_cpu(rsp->result);
	status = __le16_to_cpu(rsp->status);

<<<<<<< HEAD
	BT_DBG("dcid 0x%4.4x scid 0x%4.4x result 0x%2.2x status 0x%2.2x", dcid, scid, result, status);

	if (scid) {
		sk = l2cap_get_chan_by_scid(&conn->chan_list, scid);
		if (!sk)
			return -EFAULT;
	} else {
		sk = l2cap_get_chan_by_ident(&conn->chan_list, cmd->ident);
		if (!sk)
			return -EFAULT;
	}

	switch (result) {
	case L2CAP_CR_SUCCESS:
		sk->sk_state = BT_CONFIG;
		l2cap_pi(sk)->ident = 0;
		l2cap_pi(sk)->dcid = dcid;
		l2cap_pi(sk)->conf_state &= ~L2CAP_CONF_CONNECT_PEND;

		if (l2cap_pi(sk)->conf_state & L2CAP_CONF_REQ_SENT)
			break;

		l2cap_pi(sk)->conf_state |= L2CAP_CONF_REQ_SENT;

		l2cap_send_cmd(conn, l2cap_get_ident(conn), L2CAP_CONF_REQ,
					l2cap_build_conf_req(sk, req, sizeof(req)), req);
		l2cap_pi(sk)->num_conf_req++;
		break;

	case L2CAP_CR_PEND:
		l2cap_pi(sk)->conf_state |= L2CAP_CONF_CONNECT_PEND;
		break;

	default:
		/* don't delete l2cap channel if sk is owned by user */
		if (sock_owned_by_user(sk)) {
			sk->sk_state = BT_DISCONN;
			l2cap_sock_clear_timer(sk);
			l2cap_sock_set_timer(sk, HZ / 5);
			break;
		}

		l2cap_chan_del(sk, ECONNREFUSED);
		break;
	}

	bh_unlock_sock(sk);
	return 0;
}

static inline void set_default_fcs(struct l2cap_pinfo *pi)
=======
	if (result == L2CAP_CR_SUCCESS && (dcid < L2CAP_CID_DYN_START ||
					   dcid > L2CAP_CID_DYN_END))
		return -EPROTO;

	BT_DBG("dcid 0x%4.4x scid 0x%4.4x result 0x%2.2x status 0x%2.2x",
	       dcid, scid, result, status);

	mutex_lock(&conn->chan_lock);

	if (scid) {
		chan = __l2cap_get_chan_by_scid(conn, scid);
		if (!chan) {
			err = -EBADSLT;
			goto unlock;
		}
	} else {
		chan = __l2cap_get_chan_by_ident(conn, cmd->ident);
		if (!chan) {
			err = -EBADSLT;
			goto unlock;
		}
	}

	chan = l2cap_chan_hold_unless_zero(chan);
	if (!chan) {
		err = -EBADSLT;
		goto unlock;
	}

	err = 0;

	l2cap_chan_lock(chan);

	switch (result) {
	case L2CAP_CR_SUCCESS:
		if (__l2cap_get_chan_by_dcid(conn, dcid)) {
			err = -EBADSLT;
			break;
		}

		l2cap_state_change(chan, BT_CONFIG);
		chan->ident = 0;
		chan->dcid = dcid;
		clear_bit(CONF_CONNECT_PEND, &chan->conf_state);

		if (test_and_set_bit(CONF_REQ_SENT, &chan->conf_state))
			break;

		l2cap_send_cmd(conn, l2cap_get_ident(conn), L2CAP_CONF_REQ,
			       l2cap_build_conf_req(chan, req, sizeof(req)), req);
		chan->num_conf_req++;
		break;

	case L2CAP_CR_PEND:
		set_bit(CONF_CONNECT_PEND, &chan->conf_state);
		break;

	default:
		l2cap_chan_del(chan, ECONNREFUSED);
		break;
	}

	l2cap_chan_unlock(chan);
	l2cap_chan_put(chan);

unlock:
	mutex_unlock(&conn->chan_lock);

	return err;
}

static inline void set_default_fcs(struct l2cap_chan *chan)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	/* FCS is enabled only in ERTM or streaming mode, if one or both
	 * sides request it.
	 */
<<<<<<< HEAD
	if (pi->mode != L2CAP_MODE_ERTM && pi->mode != L2CAP_MODE_STREAMING)
		pi->fcs = L2CAP_FCS_NONE;
	else if (!(pi->conf_state & L2CAP_CONF_NO_FCS_RECV))
		pi->fcs = L2CAP_FCS_CRC16;
}

static inline int l2cap_config_req(struct l2cap_conn *conn, struct l2cap_cmd_hdr *cmd, u16 cmd_len, u8 *data)
{
	struct l2cap_conf_req *req = (struct l2cap_conf_req *) data;
	u16 dcid, flags;
	u8 rspbuf[64];
	struct l2cap_conf_rsp *rsp = (struct l2cap_conf_rsp *) rspbuf;
	struct sock *sk;
	int len;
	u8 amp_move_reconf = 0;
=======
	if (chan->mode != L2CAP_MODE_ERTM && chan->mode != L2CAP_MODE_STREAMING)
		chan->fcs = L2CAP_FCS_NONE;
	else if (!test_bit(CONF_RECV_NO_FCS, &chan->conf_state))
		chan->fcs = L2CAP_FCS_CRC16;
}

static void l2cap_send_efs_conf_rsp(struct l2cap_chan *chan, void *data,
				    u8 ident, u16 flags)
{
	struct l2cap_conn *conn = chan->conn;

	BT_DBG("conn %p chan %p ident %d flags 0x%4.4x", conn, chan, ident,
	       flags);

	clear_bit(CONF_LOC_CONF_PEND, &chan->conf_state);
	set_bit(CONF_OUTPUT_DONE, &chan->conf_state);

	l2cap_send_cmd(conn, ident, L2CAP_CONF_RSP,
		       l2cap_build_conf_rsp(chan, data,
					    L2CAP_CONF_SUCCESS, flags), data);
}

static void cmd_reject_invalid_cid(struct l2cap_conn *conn, u8 ident,
				   u16 scid, u16 dcid)
{
	struct l2cap_cmd_rej_cid rej;

	rej.reason = cpu_to_le16(L2CAP_REJ_INVALID_CID);
	rej.scid = __cpu_to_le16(scid);
	rej.dcid = __cpu_to_le16(dcid);

	l2cap_send_cmd(conn, ident, L2CAP_COMMAND_REJ, sizeof(rej), &rej);
}

static inline int l2cap_config_req(struct l2cap_conn *conn,
				   struct l2cap_cmd_hdr *cmd, u16 cmd_len,
				   u8 *data)
{
	struct l2cap_conf_req *req = (struct l2cap_conf_req *) data;
	u16 dcid, flags;
	u8 rsp[64];
	struct l2cap_chan *chan;
	int len, err = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (cmd_len < sizeof(*req))
		return -EPROTO;

	dcid  = __le16_to_cpu(req->dcid);
	flags = __le16_to_cpu(req->flags);

	BT_DBG("dcid 0x%4.4x flags 0x%2.2x", dcid, flags);

<<<<<<< HEAD
	sk = l2cap_get_chan_by_scid(&conn->chan_list, dcid);
	if (!sk)
		return -ENOENT;

	BT_DBG("sk_state 0x%2.2x rx_state 0x%2.2x "
		"reconf_state 0x%2.2x amp_id 0x%2.2x amp_move_id 0x%2.2x",
		sk->sk_state, l2cap_pi(sk)->rx_state,
		l2cap_pi(sk)->reconf_state, l2cap_pi(sk)->amp_id,
		l2cap_pi(sk)->amp_move_id);

	/* Detect a reconfig request due to channel move between
	 * BR/EDR and AMP
	 */
	if (sk->sk_state == BT_CONNECTED &&
		l2cap_pi(sk)->rx_state ==
			L2CAP_ERTM_RX_STATE_WAIT_P_FLAG_RECONFIGURE)
		l2cap_pi(sk)->reconf_state = L2CAP_RECONF_ACC;

	if (l2cap_pi(sk)->reconf_state != L2CAP_RECONF_NONE)
		amp_move_reconf = 1;

	if (sk->sk_state != BT_CONFIG && !amp_move_reconf) {
		struct l2cap_cmd_rej rej;

		rej.reason = cpu_to_le16(0x0002);
		l2cap_send_cmd(conn, cmd->ident, L2CAP_COMMAND_REJ,
				sizeof(rej), &rej);
=======
	chan = l2cap_get_chan_by_scid(conn, dcid);
	if (!chan) {
		cmd_reject_invalid_cid(conn, cmd->ident, dcid, 0);
		return 0;
	}

	if (chan->state != BT_CONFIG && chan->state != BT_CONNECT2 &&
	    chan->state != BT_CONNECTED) {
		cmd_reject_invalid_cid(conn, cmd->ident, chan->scid,
				       chan->dcid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto unlock;
	}

	/* Reject if config buffer is too small. */
<<<<<<< HEAD

	len = cmd_len - sizeof(*req);
	if (l2cap_pi(sk)->conf_len + len > sizeof(l2cap_pi(sk)->conf_req)) {
			l2cap_send_cmd(conn, cmd->ident, L2CAP_CONF_RSP,
				l2cap_build_conf_rsp(sk, rspbuf,
					L2CAP_CONF_REJECT, flags), rspbuf);
=======
	len = cmd_len - sizeof(*req);
	if (chan->conf_len + len > sizeof(chan->conf_req)) {
		l2cap_send_cmd(conn, cmd->ident, L2CAP_CONF_RSP,
			       l2cap_build_conf_rsp(chan, rsp,
			       L2CAP_CONF_REJECT, flags), rsp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto unlock;
	}

	/* Store config. */
<<<<<<< HEAD
	memcpy(l2cap_pi(sk)->conf_req + l2cap_pi(sk)->conf_len, req->data, len);
	l2cap_pi(sk)->conf_len += len;

	if (flags & 0x0001) {
		/* Incomplete config. Send empty response. */
		l2cap_send_cmd(conn, cmd->ident, L2CAP_CONF_RSP,
				l2cap_build_conf_rsp(sk, rspbuf,
					L2CAP_CONF_SUCCESS, 0x0001), rspbuf);
=======
	memcpy(chan->conf_req + chan->conf_len, req->data, len);
	chan->conf_len += len;

	if (flags & L2CAP_CONF_FLAG_CONTINUATION) {
		/* Incomplete config. Send empty response. */
		l2cap_send_cmd(conn, cmd->ident, L2CAP_CONF_RSP,
			       l2cap_build_conf_rsp(chan, rsp,
			       L2CAP_CONF_SUCCESS, flags), rsp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto unlock;
	}

	/* Complete config. */
<<<<<<< HEAD
	if (!amp_move_reconf)
		len = l2cap_parse_conf_req(sk, rspbuf, sizeof(rspbuf));
	else
		len = l2cap_parse_amp_move_reconf_req(sk, rspbuf, sizeof(rspbuf));

	if (len < 0) {
		l2cap_send_disconn_req(conn, sk, ECONNRESET);
		goto unlock;
	}

	l2cap_pi(sk)->conf_ident = cmd->ident;
	l2cap_send_cmd(conn, cmd->ident, L2CAP_CONF_RSP, len, rspbuf);

	if (l2cap_pi(sk)->conf_state & L2CAP_CONF_LOCKSTEP &&
			rsp->result == cpu_to_le16(L2CAP_CONF_PENDING) &&
			!l2cap_pi(sk)->amp_id) {
		/* Send success response right after pending if using
		 * lockstep config on BR/EDR
		 */
		rsp->result = cpu_to_le16(L2CAP_CONF_SUCCESS);
		l2cap_pi(sk)->conf_state |= L2CAP_CONF_OUTPUT_DONE;
		l2cap_send_cmd(conn, cmd->ident, L2CAP_CONF_RSP, len, rspbuf);
	}

	/* Reset config buffer. */
	l2cap_pi(sk)->conf_len = 0;

	if (amp_move_reconf)
		goto unlock;

	l2cap_pi(sk)->num_conf_rsp++;

	if (!(l2cap_pi(sk)->conf_state & L2CAP_CONF_OUTPUT_DONE))
		goto unlock;

	if (l2cap_pi(sk)->conf_state & L2CAP_CONF_INPUT_DONE) {
		set_default_fcs(l2cap_pi(sk));

		sk->sk_state = BT_CONNECTED;

		if (l2cap_pi(sk)->mode == L2CAP_MODE_ERTM ||
			l2cap_pi(sk)->mode == L2CAP_MODE_STREAMING)
			l2cap_ertm_init(sk);

		l2cap_chan_ready(sk);
		goto unlock;
	}

	if (!(l2cap_pi(sk)->conf_state & L2CAP_CONF_REQ_SENT)) {
		u8 buf[64];
		l2cap_pi(sk)->conf_state |= L2CAP_CONF_REQ_SENT;
		l2cap_send_cmd(conn, l2cap_get_ident(conn), L2CAP_CONF_REQ,
					l2cap_build_conf_req(sk, buf, sizeof(buf)), buf);
		l2cap_pi(sk)->num_conf_req++;
	}

unlock:
	bh_unlock_sock(sk);
	return 0;
=======
	len = l2cap_parse_conf_req(chan, rsp, sizeof(rsp));
	if (len < 0) {
		l2cap_send_disconn_req(chan, ECONNRESET);
		goto unlock;
	}

	chan->ident = cmd->ident;
	l2cap_send_cmd(conn, cmd->ident, L2CAP_CONF_RSP, len, rsp);
	if (chan->num_conf_rsp < L2CAP_CONF_MAX_CONF_RSP)
		chan->num_conf_rsp++;

	/* Reset config buffer. */
	chan->conf_len = 0;

	if (!test_bit(CONF_OUTPUT_DONE, &chan->conf_state))
		goto unlock;

	if (test_bit(CONF_INPUT_DONE, &chan->conf_state)) {
		set_default_fcs(chan);

		if (chan->mode == L2CAP_MODE_ERTM ||
		    chan->mode == L2CAP_MODE_STREAMING)
			err = l2cap_ertm_init(chan);

		if (err < 0)
			l2cap_send_disconn_req(chan, -err);
		else
			l2cap_chan_ready(chan);

		goto unlock;
	}

	if (!test_and_set_bit(CONF_REQ_SENT, &chan->conf_state)) {
		u8 buf[64];
		l2cap_send_cmd(conn, l2cap_get_ident(conn), L2CAP_CONF_REQ,
			       l2cap_build_conf_req(chan, buf, sizeof(buf)), buf);
		chan->num_conf_req++;
	}

	/* Got Conf Rsp PENDING from remote side and assume we sent
	   Conf Rsp PENDING in the code above */
	if (test_bit(CONF_REM_CONF_PEND, &chan->conf_state) &&
	    test_bit(CONF_LOC_CONF_PEND, &chan->conf_state)) {

		/* check compatibility */

		/* Send rsp for BR/EDR channel */
		l2cap_send_efs_conf_rsp(chan, rsp, cmd->ident, flags);
	}

unlock:
	l2cap_chan_unlock(chan);
	l2cap_chan_put(chan);
	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline int l2cap_config_rsp(struct l2cap_conn *conn,
				   struct l2cap_cmd_hdr *cmd, u16 cmd_len,
				   u8 *data)
{
	struct l2cap_conf_rsp *rsp = (struct l2cap_conf_rsp *)data;
	u16 scid, flags, result;
<<<<<<< HEAD
	struct sock *sk;
	struct l2cap_pinfo *pi;
	int len = cmd_len - sizeof(*rsp);
=======
	struct l2cap_chan *chan;
	int len = cmd_len - sizeof(*rsp);
	int err = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (cmd_len < sizeof(*rsp))
		return -EPROTO;

	scid   = __le16_to_cpu(rsp->scid);
	flags  = __le16_to_cpu(rsp->flags);
	result = __le16_to_cpu(rsp->result);

<<<<<<< HEAD
	BT_DBG("scid 0x%4.4x flags 0x%2.2x result 0x%2.2x",
			scid, flags, result);

	sk = l2cap_get_chan_by_scid(&conn->chan_list, scid);
	if (!sk)
		return 0;

	pi = l2cap_pi(sk);

	if (pi->reconf_state != L2CAP_RECONF_NONE)  {
		l2cap_amp_move_reconf_rsp(sk, rsp->data, len, result);
		goto done;
	}

	switch (result) {
	case L2CAP_CONF_SUCCESS:
		if (pi->conf_state & L2CAP_CONF_LOCKSTEP &&
				!(pi->conf_state & L2CAP_CONF_LOCKSTEP_PEND)) {
			/* Lockstep procedure requires a pending response
			 * before success.
			 */
			l2cap_send_disconn_req(conn, sk, ECONNRESET);
			goto done;
		}

		l2cap_conf_rfc_get(sk, rsp->data, len);
		break;

	case L2CAP_CONF_PENDING:
		if (!(pi->conf_state & L2CAP_CONF_LOCKSTEP)) {
			l2cap_send_disconn_req(conn, sk, ECONNRESET);
			goto done;
		}

		l2cap_conf_rfc_get(sk, rsp->data, len);

		pi->conf_state |= L2CAP_CONF_LOCKSTEP_PEND;

		l2cap_conf_ext_fs_get(sk, rsp->data, len);

		if (pi->amp_id && pi->conf_state & L2CAP_CONF_PEND_SENT) {
			struct hci_chan *chan;

			/* Already sent a 'pending' response, so set up
			 * the logical link now
			 */
			chan = l2cap_chan_admit(pi->amp_id, sk);
			if (!chan) {
				l2cap_send_disconn_req(pi->conn, sk,
							ECONNRESET);
				goto done;
			}

			if (chan->state == BT_CONNECTED)
				l2cap_create_cfm(chan, 0);
		}

		goto done;

	case L2CAP_CONF_UNACCEPT:
		if (pi->num_conf_rsp <= L2CAP_CONF_MAX_CONF_RSP) {
			char req[64];

			if (len > sizeof(req) - sizeof(struct l2cap_conf_req)) {
				l2cap_send_disconn_req(conn, sk, ECONNRESET);
=======
	BT_DBG("scid 0x%4.4x flags 0x%2.2x result 0x%2.2x len %d", scid, flags,
	       result, len);

	chan = l2cap_get_chan_by_scid(conn, scid);
	if (!chan)
		return 0;

	switch (result) {
	case L2CAP_CONF_SUCCESS:
		l2cap_conf_rfc_get(chan, rsp->data, len);
		clear_bit(CONF_REM_CONF_PEND, &chan->conf_state);
		break;

	case L2CAP_CONF_PENDING:
		set_bit(CONF_REM_CONF_PEND, &chan->conf_state);

		if (test_bit(CONF_LOC_CONF_PEND, &chan->conf_state)) {
			char buf[64];

			len = l2cap_parse_conf_rsp(chan, rsp->data, len,
						   buf, sizeof(buf), &result);
			if (len < 0) {
				l2cap_send_disconn_req(chan, ECONNRESET);
				goto done;
			}

			l2cap_send_efs_conf_rsp(chan, buf, cmd->ident, 0);
		}
		goto done;

	case L2CAP_CONF_UNKNOWN:
	case L2CAP_CONF_UNACCEPT:
		if (chan->num_conf_rsp <= L2CAP_CONF_MAX_CONF_RSP) {
			char req[64];

			if (len > sizeof(req) - sizeof(struct l2cap_conf_req)) {
				l2cap_send_disconn_req(chan, ECONNRESET);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				goto done;
			}

			/* throw out any old stored conf requests */
			result = L2CAP_CONF_SUCCESS;
<<<<<<< HEAD
			len = l2cap_parse_conf_rsp(sk, rsp->data,
							len, req, sizeof(req), &result);
			if (len < 0) {
				l2cap_send_disconn_req(conn, sk, ECONNRESET);
=======
			len = l2cap_parse_conf_rsp(chan, rsp->data, len,
						   req, sizeof(req), &result);
			if (len < 0) {
				l2cap_send_disconn_req(chan, ECONNRESET);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				goto done;
			}

			l2cap_send_cmd(conn, l2cap_get_ident(conn),
<<<<<<< HEAD
						L2CAP_CONF_REQ, len, req);
			pi->num_conf_req++;
=======
				       L2CAP_CONF_REQ, len, req);
			chan->num_conf_req++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (result != L2CAP_CONF_SUCCESS)
				goto done;
			break;
		}
<<<<<<< HEAD

	default:
		sk->sk_err = ECONNRESET;
		l2cap_sock_set_timer(sk, HZ * 5);
		l2cap_send_disconn_req(conn, sk, ECONNRESET);
		goto done;
	}

	if (flags & 0x01)
		goto done;

	pi->conf_state |= L2CAP_CONF_INPUT_DONE;

	if (pi->conf_state & L2CAP_CONF_OUTPUT_DONE) {
		set_default_fcs(pi);

		sk->sk_state = BT_CONNECTED;

		if (pi->mode == L2CAP_MODE_ERTM ||
			pi->mode == L2CAP_MODE_STREAMING)
			l2cap_ertm_init(sk);

		l2cap_chan_ready(sk);
	}

done:
	bh_unlock_sock(sk);
	return 0;
=======
		fallthrough;

	default:
		l2cap_chan_set_err(chan, ECONNRESET);

		__set_chan_timer(chan, L2CAP_DISC_REJ_TIMEOUT);
		l2cap_send_disconn_req(chan, ECONNRESET);
		goto done;
	}

	if (flags & L2CAP_CONF_FLAG_CONTINUATION)
		goto done;

	set_bit(CONF_INPUT_DONE, &chan->conf_state);

	if (test_bit(CONF_OUTPUT_DONE, &chan->conf_state)) {
		set_default_fcs(chan);

		if (chan->mode == L2CAP_MODE_ERTM ||
		    chan->mode == L2CAP_MODE_STREAMING)
			err = l2cap_ertm_init(chan);

		if (err < 0)
			l2cap_send_disconn_req(chan, -err);
		else
			l2cap_chan_ready(chan);
	}

done:
	l2cap_chan_unlock(chan);
	l2cap_chan_put(chan);
	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline int l2cap_disconnect_req(struct l2cap_conn *conn,
				       struct l2cap_cmd_hdr *cmd, u16 cmd_len,
				       u8 *data)
{
	struct l2cap_disconn_req *req = (struct l2cap_disconn_req *) data;
	struct l2cap_disconn_rsp rsp;
	u16 dcid, scid;
<<<<<<< HEAD
	struct sock *sk;
=======
	struct l2cap_chan *chan;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (cmd_len != sizeof(*req))
		return -EPROTO;

	scid = __le16_to_cpu(req->scid);
	dcid = __le16_to_cpu(req->dcid);

	BT_DBG("scid 0x%4.4x dcid 0x%4.4x", scid, dcid);

<<<<<<< HEAD
	sk = l2cap_get_chan_by_scid(&conn->chan_list, dcid);
	if (!sk)
		return 0;

	rsp.dcid = cpu_to_le16(l2cap_pi(sk)->scid);
	rsp.scid = cpu_to_le16(l2cap_pi(sk)->dcid);
	l2cap_send_cmd(conn, cmd->ident, L2CAP_DISCONN_RSP, sizeof(rsp), &rsp);

	/* Only do cleanup if a disconnect request was not sent already */
	if (sk->sk_state != BT_DISCONN) {
		sk->sk_shutdown = SHUTDOWN_MASK;

		sk->sk_send_head = NULL;
		skb_queue_purge(TX_QUEUE(sk));

		if (l2cap_pi(sk)->mode == L2CAP_MODE_ERTM) {
			skb_queue_purge(SREJ_QUEUE(sk));

			__cancel_delayed_work(&l2cap_pi(sk)->ack_work);
			__cancel_delayed_work(&l2cap_pi(sk)->retrans_work);
			__cancel_delayed_work(&l2cap_pi(sk)->monitor_work);
		}
	}

	/* don't delete l2cap channel if sk is owned by user */
	if (sock_owned_by_user(sk)) {
		sk->sk_state = BT_DISCONN;
		l2cap_sock_clear_timer(sk);
		l2cap_sock_set_timer(sk, HZ / 5);
		bh_unlock_sock(sk);
		return 0;
	}

	l2cap_chan_del(sk, ECONNRESET);

	bh_unlock_sock(sk);

	l2cap_sock_kill(sk);
=======
	chan = l2cap_get_chan_by_scid(conn, dcid);
	if (!chan) {
		cmd_reject_invalid_cid(conn, cmd->ident, dcid, scid);
		return 0;
	}

	rsp.dcid = cpu_to_le16(chan->scid);
	rsp.scid = cpu_to_le16(chan->dcid);
	l2cap_send_cmd(conn, cmd->ident, L2CAP_DISCONN_RSP, sizeof(rsp), &rsp);

	chan->ops->set_shutdown(chan);

	l2cap_chan_unlock(chan);
	mutex_lock(&conn->chan_lock);
	l2cap_chan_lock(chan);
	l2cap_chan_del(chan, ECONNRESET);
	mutex_unlock(&conn->chan_lock);

	chan->ops->close(chan);

	l2cap_chan_unlock(chan);
	l2cap_chan_put(chan);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static inline int l2cap_disconnect_rsp(struct l2cap_conn *conn,
				       struct l2cap_cmd_hdr *cmd, u16 cmd_len,
				       u8 *data)
{
	struct l2cap_disconn_rsp *rsp = (struct l2cap_disconn_rsp *) data;
	u16 dcid, scid;
<<<<<<< HEAD
	struct sock *sk;
=======
	struct l2cap_chan *chan;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (cmd_len != sizeof(*rsp))
		return -EPROTO;

	scid = __le16_to_cpu(rsp->scid);
	dcid = __le16_to_cpu(rsp->dcid);

	BT_DBG("dcid 0x%4.4x scid 0x%4.4x", dcid, scid);

<<<<<<< HEAD
	sk = l2cap_get_chan_by_scid(&conn->chan_list, scid);
	if (!sk)
		return 0;

	/* don't delete l2cap channel if sk is owned by user */
	if (sock_owned_by_user(sk)) {
		sk->sk_state = BT_DISCONN;
		l2cap_sock_clear_timer(sk);
		l2cap_sock_set_timer(sk, HZ / 5);
		bh_unlock_sock(sk);
		return 0;
	}

	l2cap_chan_del(sk, 0);
	bh_unlock_sock(sk);

	l2cap_sock_kill(sk);
=======
	chan = l2cap_get_chan_by_scid(conn, scid);
	if (!chan) {
		return 0;
	}

	if (chan->state != BT_DISCONN) {
		l2cap_chan_unlock(chan);
		l2cap_chan_put(chan);
		return 0;
	}

	l2cap_chan_unlock(chan);
	mutex_lock(&conn->chan_lock);
	l2cap_chan_lock(chan);
	l2cap_chan_del(chan, 0);
	mutex_unlock(&conn->chan_lock);

	chan->ops->close(chan);

	l2cap_chan_unlock(chan);
	l2cap_chan_put(chan);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static inline int l2cap_information_req(struct l2cap_conn *conn,
					struct l2cap_cmd_hdr *cmd, u16 cmd_len,
					u8 *data)
{
	struct l2cap_info_req *req = (struct l2cap_info_req *) data;
	u16 type;

	if (cmd_len != sizeof(*req))
		return -EPROTO;

	type = __le16_to_cpu(req->type);

	BT_DBG("type 0x%4.4x", type);

	if (type == L2CAP_IT_FEAT_MASK) {
		u8 buf[8];
		u32 feat_mask = l2cap_feat_mask;
		struct l2cap_info_rsp *rsp = (struct l2cap_info_rsp *) buf;
		rsp->type   = cpu_to_le16(L2CAP_IT_FEAT_MASK);
		rsp->result = cpu_to_le16(L2CAP_IR_SUCCESS);
		if (!disable_ertm)
			feat_mask |= L2CAP_FEAT_ERTM | L2CAP_FEAT_STREAMING
<<<<<<< HEAD
				| L2CAP_FEAT_FCS | L2CAP_FEAT_EXT_WINDOW;
		put_unaligned_le32(feat_mask, rsp->data);
		l2cap_send_cmd(conn, cmd->ident,
					L2CAP_INFO_RSP, sizeof(buf), buf);
	} else if (type == L2CAP_IT_FIXED_CHAN) {
		u8 buf[12];
		u8 fc_mask = l2cap_fc_mask;
		struct l2cap_info_rsp *rsp = (struct l2cap_info_rsp *) buf;
		rsp->type   = cpu_to_le16(L2CAP_IT_FIXED_CHAN);
		rsp->result = cpu_to_le16(L2CAP_IR_SUCCESS);
		if (enable_hs)
			fc_mask |= L2CAP_FC_A2MP;
		memset(rsp->data, 0, 8);
		rsp->data[0] = fc_mask;
		l2cap_send_cmd(conn, cmd->ident,
					L2CAP_INFO_RSP, sizeof(buf), buf);
=======
				| L2CAP_FEAT_FCS;

		put_unaligned_le32(feat_mask, rsp->data);
		l2cap_send_cmd(conn, cmd->ident, L2CAP_INFO_RSP, sizeof(buf),
			       buf);
	} else if (type == L2CAP_IT_FIXED_CHAN) {
		u8 buf[12];
		struct l2cap_info_rsp *rsp = (struct l2cap_info_rsp *) buf;

		rsp->type   = cpu_to_le16(L2CAP_IT_FIXED_CHAN);
		rsp->result = cpu_to_le16(L2CAP_IR_SUCCESS);
		rsp->data[0] = conn->local_fixed_chan;
		memset(rsp->data + 1, 0, 7);
		l2cap_send_cmd(conn, cmd->ident, L2CAP_INFO_RSP, sizeof(buf),
			       buf);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		struct l2cap_info_rsp rsp;
		rsp.type   = cpu_to_le16(type);
		rsp.result = cpu_to_le16(L2CAP_IR_NOTSUPP);
<<<<<<< HEAD
		l2cap_send_cmd(conn, cmd->ident,
					L2CAP_INFO_RSP, sizeof(rsp), &rsp);
=======
		l2cap_send_cmd(conn, cmd->ident, L2CAP_INFO_RSP, sizeof(rsp),
			       &rsp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return 0;
}

static inline int l2cap_information_rsp(struct l2cap_conn *conn,
					struct l2cap_cmd_hdr *cmd, u16 cmd_len,
					u8 *data)
{
	struct l2cap_info_rsp *rsp = (struct l2cap_info_rsp *) data;
	u16 type, result;

	if (cmd_len < sizeof(*rsp))
		return -EPROTO;

	type   = __le16_to_cpu(rsp->type);
	result = __le16_to_cpu(rsp->result);

	BT_DBG("type 0x%4.4x result 0x%2.2x", type, result);

	/* L2CAP Info req/rsp are unbound to channels, add extra checks */
	if (cmd->ident != conn->info_ident ||
<<<<<<< HEAD
			conn->info_state & L2CAP_INFO_FEAT_MASK_REQ_DONE)
		return 0;

	del_timer(&conn->info_timer);
=======
	    conn->info_state & L2CAP_INFO_FEAT_MASK_REQ_DONE)
		return 0;

	cancel_delayed_work(&conn->info_timer);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (result != L2CAP_IR_SUCCESS) {
		conn->info_state |= L2CAP_INFO_FEAT_MASK_REQ_DONE;
		conn->info_ident = 0;

		l2cap_conn_start(conn);

		return 0;
	}

<<<<<<< HEAD
	if (type == L2CAP_IT_FEAT_MASK) {
=======
	switch (type) {
	case L2CAP_IT_FEAT_MASK:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		conn->feat_mask = get_unaligned_le32(rsp->data);

		if (conn->feat_mask & L2CAP_FEAT_FIXED_CHAN) {
			struct l2cap_info_req req;
			req.type = cpu_to_le16(L2CAP_IT_FIXED_CHAN);

			conn->info_ident = l2cap_get_ident(conn);

			l2cap_send_cmd(conn, conn->info_ident,
<<<<<<< HEAD
					L2CAP_INFO_REQ, sizeof(req), &req);
=======
				       L2CAP_INFO_REQ, sizeof(req), &req);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		} else {
			conn->info_state |= L2CAP_INFO_FEAT_MASK_REQ_DONE;
			conn->info_ident = 0;

			l2cap_conn_start(conn);
		}
<<<<<<< HEAD
	} else if (type == L2CAP_IT_FIXED_CHAN) {
		conn->fc_mask = rsp->data[0];
=======
		break;

	case L2CAP_IT_FIXED_CHAN:
		conn->remote_fixed_chan = rsp->data[0];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		conn->info_state |= L2CAP_INFO_FEAT_MASK_REQ_DONE;
		conn->info_ident = 0;

		l2cap_conn_start(conn);
<<<<<<< HEAD
	}

	return 0;
}

static void l2cap_send_move_chan_req(struct l2cap_conn *conn,
			struct l2cap_pinfo *pi, u16 icid, u8 dest_amp_id)
{
	struct l2cap_move_chan_req req;
	u8 ident;

	BT_DBG("pi %p, icid %d, dest_amp_id %d", pi, (int) icid,
		(int) dest_amp_id);

	ident = l2cap_get_ident(conn);
	if (pi)
		pi->ident = ident;

	req.icid = cpu_to_le16(icid);
	req.dest_amp_id = dest_amp_id;

	l2cap_send_cmd(conn, ident, L2CAP_MOVE_CHAN_REQ, sizeof(req), &req);
}

static void l2cap_send_move_chan_rsp(struct l2cap_conn *conn, u8 ident,
				u16 icid, u16 result)
{
	struct l2cap_move_chan_rsp rsp;

	BT_DBG("icid %d, result %d", (int) icid, (int) result);

	rsp.icid = cpu_to_le16(icid);
	rsp.result = cpu_to_le16(result);

	l2cap_send_cmd(conn, ident, L2CAP_MOVE_CHAN_RSP, sizeof(rsp), &rsp);
}

static void l2cap_send_move_chan_cfm(struct l2cap_conn *conn,
				struct l2cap_pinfo *pi, u16 icid, u16 result)
{
	struct l2cap_move_chan_cfm cfm;
	u8 ident;

	BT_DBG("icid %d, result %d", (int) icid, (int) result);

	ident = l2cap_get_ident(conn);
	if (pi)
		pi->ident = ident;

	cfm.icid = cpu_to_le16(icid);
	cfm.result = cpu_to_le16(result);

	l2cap_send_cmd(conn, ident, L2CAP_MOVE_CHAN_CFM, sizeof(cfm), &cfm);
}

static void l2cap_send_move_chan_cfm_rsp(struct l2cap_conn *conn, u8 ident,
					u16 icid)
{
	struct l2cap_move_chan_cfm_rsp rsp;

	BT_DBG("icid %d", (int) icid);

	rsp.icid = cpu_to_le16(icid);
	l2cap_send_cmd(conn, ident, L2CAP_MOVE_CHAN_CFM_RSP, sizeof(rsp), &rsp);
}

static inline int l2cap_create_channel_req(struct l2cap_conn *conn,
					struct l2cap_cmd_hdr *cmd, u16 cmd_len, u8 *data)
{
	struct l2cap_create_chan_req *req =
		(struct l2cap_create_chan_req *) data;
	struct sock *sk;
	u16 psm, scid;

	if (cmd_len < sizeof(struct l2cap_conn_req))
		return -EPROTO;

	psm = le16_to_cpu(req->psm);
	scid = le16_to_cpu(req->scid);

	BT_DBG("psm %d, scid %d, amp_id %d", (int) psm, (int) scid,
		(int) req->amp_id);

	if (req->amp_id) {
		struct hci_dev *hdev;

		/* Validate AMP controller id */
		hdev = hci_dev_get(req->amp_id);
		if (!hdev || !test_bit(HCI_UP, &hdev->flags)) {
			struct l2cap_create_chan_rsp rsp;

			rsp.dcid = 0;
			rsp.scid = cpu_to_le16(scid);
			rsp.result = L2CAP_CREATE_CHAN_REFUSED_CONTROLLER;
			rsp.status = L2CAP_CREATE_CHAN_STATUS_NONE;

			l2cap_send_cmd(conn, cmd->ident, L2CAP_CREATE_CHAN_RSP,
				       sizeof(rsp), &rsp);

			if (hdev)
				hci_dev_put(hdev);

			return 0;
		}

		hci_dev_put(hdev);
	}

	sk = l2cap_create_connect(conn, cmd, data, L2CAP_CREATE_CHAN_RSP,
					req->amp_id);

	if (sk)
		l2cap_pi(sk)->conf_state |= L2CAP_CONF_LOCKSTEP;

	if (sk && req->amp_id &&
			(conn->info_state & L2CAP_INFO_FEAT_MASK_REQ_DONE))
		amp_accept_physical(conn, req->amp_id, sk);

	return 0;
}

static inline int l2cap_create_channel_rsp(struct l2cap_conn *conn,
					struct l2cap_cmd_hdr *cmd, u16 cmd_len,
					void *data)
{
	BT_DBG("conn %p", conn);

	return l2cap_connect_rsp(conn, cmd, cmd_len, data);
}

static inline int l2cap_move_channel_req(struct l2cap_conn *conn,
					struct l2cap_cmd_hdr *cmd, u8 *data)
{
	struct l2cap_move_chan_req *req = (struct l2cap_move_chan_req *) data;
	struct sock *sk;
	struct l2cap_pinfo *pi;
	u16 icid = 0;
	u16 result = L2CAP_MOVE_CHAN_REFUSED_NOT_ALLOWED;

	icid = le16_to_cpu(req->icid);

	BT_DBG("icid %d, dest_amp_id %d", (int) icid, (int) req->dest_amp_id);

	read_lock(&conn->chan_list.lock);
	sk = __l2cap_get_chan_by_dcid(&conn->chan_list, icid);
	read_unlock(&conn->chan_list.lock);

	if (!sk)
		goto send_move_response;

	lock_sock(sk);
	pi = l2cap_pi(sk);

	if (pi->scid < L2CAP_CID_DYN_START ||
		(pi->mode != L2CAP_MODE_ERTM &&
		 pi->mode != L2CAP_MODE_STREAMING)) {
		goto send_move_response;
	}

	if (pi->amp_id == req->dest_amp_id) {
		result = L2CAP_MOVE_CHAN_REFUSED_SAME_ID;
		goto send_move_response;
	}

	if (req->dest_amp_id) {
		struct hci_dev *hdev;
		hdev = hci_dev_get(req->dest_amp_id);
		if (!hdev || !test_bit(HCI_UP, &hdev->flags)) {
			if (hdev)
				hci_dev_put(hdev);

			result = L2CAP_MOVE_CHAN_REFUSED_CONTROLLER;
			goto send_move_response;
		}
		hci_dev_put(hdev);
	}

	if (((pi->amp_move_state != L2CAP_AMP_STATE_STABLE &&
		pi->amp_move_state != L2CAP_AMP_STATE_WAIT_PREPARE) ||
		pi->amp_move_role != L2CAP_AMP_MOVE_NONE) &&
		bacmp(conn->src, conn->dst) > 0) {
		result = L2CAP_MOVE_CHAN_REFUSED_COLLISION;
		goto send_move_response;
	}

	if (pi->amp_pref == BT_AMP_POLICY_REQUIRE_BR_EDR) {
		result = L2CAP_MOVE_CHAN_REFUSED_NOT_ALLOWED;
		goto send_move_response;
	}

	pi->amp_move_cmd_ident = cmd->ident;
	pi->amp_move_role = L2CAP_AMP_MOVE_RESPONDER;
	l2cap_amp_move_setup(sk);
	pi->amp_move_id = req->dest_amp_id;
	icid = pi->dcid;

	if (req->dest_amp_id == 0) {
		/* Moving to BR/EDR */
		if (pi->conn_state & L2CAP_CONN_LOCAL_BUSY) {
			pi->amp_move_state = L2CAP_AMP_STATE_WAIT_LOCAL_BUSY;
			result = L2CAP_MOVE_CHAN_PENDING;
		} else {
			pi->amp_move_state = L2CAP_AMP_STATE_WAIT_MOVE_CONFIRM;
			result = L2CAP_MOVE_CHAN_SUCCESS;
		}
	} else {
		pi->amp_move_state = L2CAP_AMP_STATE_WAIT_PREPARE;
		amp_accept_physical(pi->conn, req->dest_amp_id, sk);
		result = L2CAP_MOVE_CHAN_PENDING;
	}

send_move_response:
	l2cap_send_move_chan_rsp(conn, cmd->ident, icid, result);

	if (sk)
		release_sock(sk);

	return 0;
}

static inline int l2cap_move_channel_rsp(struct l2cap_conn *conn,
					struct l2cap_cmd_hdr *cmd, u8 *data)
{
	struct l2cap_move_chan_rsp *rsp = (struct l2cap_move_chan_rsp *) data;
	struct sock *sk;
	struct l2cap_pinfo *pi;
	u16 icid, result;

	icid = le16_to_cpu(rsp->icid);
	result = le16_to_cpu(rsp->result);

	BT_DBG("icid %d, result %d", (int) icid, (int) result);

	switch (result) {
	case L2CAP_MOVE_CHAN_SUCCESS:
	case L2CAP_MOVE_CHAN_PENDING:
		read_lock(&conn->chan_list.lock);
		sk = __l2cap_get_chan_by_scid(&conn->chan_list, icid);
		read_unlock(&conn->chan_list.lock);

		if (!sk) {
			l2cap_send_move_chan_cfm(conn, NULL, icid,
						L2CAP_MOVE_CHAN_UNCONFIRMED);
			break;
		}

		lock_sock(sk);
		pi = l2cap_pi(sk);

		l2cap_sock_clear_timer(sk);
		if (result == L2CAP_MOVE_CHAN_PENDING)
			l2cap_sock_set_timer(sk, L2CAP_MOVE_ERTX_TIMEOUT);

		if (pi->amp_move_state ==
				L2CAP_AMP_STATE_WAIT_LOGICAL_COMPLETE) {
			/* Move confirm will be sent when logical link
			 * is complete.
			 */
			pi->amp_move_state =
				L2CAP_AMP_STATE_WAIT_LOGICAL_CONFIRM;
		} else if (pi->amp_move_state ==
				L2CAP_AMP_STATE_WAIT_MOVE_RSP_SUCCESS) {
			if (result == L2CAP_MOVE_CHAN_PENDING) {
				break;
			} else if (pi->conn_state & L2CAP_CONN_LOCAL_BUSY) {
				pi->amp_move_state =
					L2CAP_AMP_STATE_WAIT_LOCAL_BUSY;
			} else {
				/* Logical link is up or moving to BR/EDR,
				 * proceed with move */
				pi->amp_move_state =
					L2CAP_AMP_STATE_WAIT_MOVE_CONFIRM_RSP;
				l2cap_send_move_chan_cfm(conn, pi, pi->scid,
						L2CAP_MOVE_CHAN_CONFIRMED);
				l2cap_sock_set_timer(sk, L2CAP_MOVE_TIMEOUT);
			}
		} else if (pi->amp_move_state ==
				L2CAP_AMP_STATE_WAIT_MOVE_RSP) {
			struct l2cap_conf_ext_fs default_fs = {1, 1, 0xFFFF,
					0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
			struct hci_chan *chan;
			/* Moving to AMP */
			if (result == L2CAP_MOVE_CHAN_SUCCESS) {
				/* Remote is ready, send confirm immediately
				 * after logical link is ready
				 */
				pi->amp_move_state =
					L2CAP_AMP_STATE_WAIT_LOGICAL_CONFIRM;
			} else {
				/* Both logical link and move success
				 * are required to confirm
				 */
				pi->amp_move_state =
					L2CAP_AMP_STATE_WAIT_LOGICAL_COMPLETE;
			}
			pi->remote_fs = default_fs;
			pi->local_fs = default_fs;
			chan = l2cap_chan_admit(pi->amp_move_id, sk);
			if (!chan) {
				/* Logical link not available */
				l2cap_send_move_chan_cfm(conn, pi, pi->scid,
						L2CAP_MOVE_CHAN_UNCONFIRMED);
				break;
			}

			if (chan->state == BT_CONNECTED) {
				/* Logical link is already ready to go */
				pi->ampcon = chan->conn;
				pi->ampcon->l2cap_data = pi->conn;
				if (result == L2CAP_MOVE_CHAN_SUCCESS) {
					/* Can confirm now */
					l2cap_send_move_chan_cfm(conn, pi,
						pi->scid,
						L2CAP_MOVE_CHAN_CONFIRMED);
				} else {
					/* Now only need move success
					 * required to confirm
					 */
					pi->amp_move_state =
					L2CAP_AMP_STATE_WAIT_MOVE_RSP_SUCCESS;
				}

				l2cap_create_cfm(chan, 0);
			}
		} else {
			/* Any other amp move state means the move failed. */
			pi->amp_move_id = pi->amp_id;
			pi->amp_move_state = L2CAP_AMP_STATE_STABLE;
			l2cap_amp_move_revert(sk);
			pi->amp_move_role = L2CAP_AMP_MOVE_NONE;
			l2cap_send_move_chan_cfm(conn, pi, pi->scid,
						L2CAP_MOVE_CHAN_UNCONFIRMED);
			l2cap_sock_set_timer(sk, L2CAP_MOVE_TIMEOUT);
		}
		break;
	default:
		/* Failed (including collision case) */
		read_lock(&conn->chan_list.lock);
		sk = __l2cap_get_chan_by_ident(&conn->chan_list, cmd->ident);
		read_unlock(&conn->chan_list.lock);

		if (!sk) {
			/* Could not locate channel, icid is best guess */
			l2cap_send_move_chan_cfm(conn, NULL, icid,
						L2CAP_MOVE_CHAN_UNCONFIRMED);
			break;
		}

		lock_sock(sk);
		pi = l2cap_pi(sk);

		l2cap_sock_clear_timer(sk);

		if (pi->amp_move_role == L2CAP_AMP_MOVE_INITIATOR) {
			if (result == L2CAP_MOVE_CHAN_REFUSED_COLLISION)
				pi->amp_move_role = L2CAP_AMP_MOVE_RESPONDER;
			else {
				/* Cleanup - cancel move */
				pi->amp_move_id = pi->amp_id;
				pi->amp_move_state = L2CAP_AMP_STATE_STABLE;
				l2cap_amp_move_revert(sk);
				pi->amp_move_role = L2CAP_AMP_MOVE_NONE;
			}
		}

		l2cap_send_move_chan_cfm(conn, pi, pi->scid,
					L2CAP_MOVE_CHAN_UNCONFIRMED);
		l2cap_sock_set_timer(sk, L2CAP_MOVE_TIMEOUT);
		break;
	}

	if (sk)
		release_sock(sk);

	return 0;
}

static inline int l2cap_move_channel_confirm(struct l2cap_conn *conn,
					struct l2cap_cmd_hdr *cmd, u8 *data)
{
	struct l2cap_move_chan_cfm *cfm = (struct l2cap_move_chan_cfm *) data;
	struct sock *sk;
	struct l2cap_pinfo *pi;
	u16 icid, result;

	icid = le16_to_cpu(cfm->icid);
	result = le16_to_cpu(cfm->result);

	BT_DBG("icid %d, result %d", (int) icid, (int) result);

	read_lock(&conn->chan_list.lock);
	sk = __l2cap_get_chan_by_dcid(&conn->chan_list, icid);
	read_unlock(&conn->chan_list.lock);

	if (!sk) {
		BT_DBG("Bad channel (%d)", (int) icid);
		goto send_move_confirm_response;
	}

	lock_sock(sk);
	pi = l2cap_pi(sk);

	if (pi->amp_move_state == L2CAP_AMP_STATE_WAIT_MOVE_CONFIRM) {
		pi->amp_move_state = L2CAP_AMP_STATE_STABLE;
		if (result == L2CAP_MOVE_CHAN_CONFIRMED) {
			pi->amp_id = pi->amp_move_id;
			if (!pi->amp_id && pi->ampchan) {
				struct hci_chan *ampchan = pi->ampchan;
				struct hci_conn *ampcon = pi->ampcon;
				/* Have moved off of AMP, free the channel */
				pi->ampchan = NULL;
				pi->ampcon = NULL;
				if (hci_chan_put(ampchan))
					ampcon->l2cap_data = NULL;
				else
					l2cap_deaggregate(ampchan, pi);
			}
			l2cap_amp_move_success(sk);
		} else {
			pi->amp_move_id = pi->amp_id;
			l2cap_amp_move_revert(sk);
		}
		pi->amp_move_role = L2CAP_AMP_MOVE_NONE;
	} else if (pi->amp_move_state ==
			L2CAP_AMP_STATE_WAIT_LOGICAL_CONFIRM) {
		BT_DBG("Bad AMP_MOVE_STATE (%d)", pi->amp_move_state);
	}

send_move_confirm_response:
	l2cap_send_move_chan_cfm_rsp(conn, cmd->ident, icid);

	if (sk)
		release_sock(sk);

	return 0;
}

static inline int l2cap_move_channel_confirm_rsp(struct l2cap_conn *conn,
					struct l2cap_cmd_hdr *cmd, u8 *data)
{
	struct l2cap_move_chan_cfm_rsp *rsp =
		(struct l2cap_move_chan_cfm_rsp *) data;
	struct sock *sk;
	struct l2cap_pinfo *pi;

	u16 icid;

	icid = le16_to_cpu(rsp->icid);

	BT_DBG("icid %d", (int) icid);

	read_lock(&conn->chan_list.lock);
	sk = __l2cap_get_chan_by_scid(&conn->chan_list, icid);
	read_unlock(&conn->chan_list.lock);

	if (!sk)
		return 0;

	lock_sock(sk);
	pi = l2cap_pi(sk);

	l2cap_sock_clear_timer(sk);

	if (pi->amp_move_state ==
			L2CAP_AMP_STATE_WAIT_MOVE_CONFIRM_RSP) {
		pi->amp_move_state = L2CAP_AMP_STATE_STABLE;
		pi->amp_id = pi->amp_move_id;

		if (!pi->amp_id && pi->ampchan) {
			struct hci_chan *ampchan = pi->ampchan;
			struct hci_conn *ampcon = pi->ampcon;
			/* Have moved off of AMP, free the channel */
			pi->ampchan = NULL;
			pi->ampcon = NULL;
			if (hci_chan_put(ampchan))
				ampcon->l2cap_data = NULL;
			else
				l2cap_deaggregate(ampchan, pi);
		}

		l2cap_amp_move_success(sk);

		pi->amp_move_role = L2CAP_AMP_MOVE_NONE;
	}

	release_sock(sk);

	return 0;
}

static void l2cap_amp_signal_worker(struct work_struct *work)
{
	int err = 0;
	struct l2cap_amp_signal_work *ampwork =
		container_of(work, struct l2cap_amp_signal_work, work);

	switch (ampwork->cmd.code) {
	case L2CAP_MOVE_CHAN_REQ:
		err = l2cap_move_channel_req(ampwork->conn, &ampwork->cmd,
						ampwork->data);
		break;

	case L2CAP_MOVE_CHAN_RSP:
		err = l2cap_move_channel_rsp(ampwork->conn, &ampwork->cmd,
						ampwork->data);
		break;

	case L2CAP_MOVE_CHAN_CFM:
		err = l2cap_move_channel_confirm(ampwork->conn, &ampwork->cmd,
						ampwork->data);
		break;

	case L2CAP_MOVE_CHAN_CFM_RSP:
		err = l2cap_move_channel_confirm_rsp(ampwork->conn,
						&ampwork->cmd, ampwork->data);
		break;

	default:
		BT_ERR("Unknown signaling command 0x%2.2x", ampwork->cmd.code);
		err = -EINVAL;
		break;
	}

	if (err) {
		struct l2cap_cmd_rej rej;
		BT_DBG("error %d", err);

		/* In this context, commands are only rejected with
		 * "command not understood", code 0.
		 */
		rej.reason = cpu_to_le16(0);
		l2cap_send_cmd(ampwork->conn, ampwork->cmd.ident,
				L2CAP_COMMAND_REJ, sizeof(rej), &rej);
	}

	kfree_skb(ampwork->skb);
	kfree(ampwork);
}

void l2cap_amp_physical_complete(int result, u8 local_id, u8 remote_id,
				struct sock *sk)
{
	struct l2cap_pinfo *pi;

	BT_DBG("result %d, local_id %d, remote_id %d, sk %p", result,
		(int) local_id, (int) remote_id, sk);

	lock_sock(sk);

	if (sk->sk_state == BT_DISCONN || sk->sk_state == BT_CLOSED) {
		release_sock(sk);
		return;
	}

	pi = l2cap_pi(sk);

	if (sk->sk_state != BT_CONNECTED) {
		if (bt_sk(sk)->parent) {
			struct l2cap_conn_rsp rsp;
			char buf[128];
			rsp.scid = cpu_to_le16(l2cap_pi(sk)->dcid);
			rsp.dcid = cpu_to_le16(l2cap_pi(sk)->scid);

			/* Incoming channel on AMP */
			if (result == L2CAP_CREATE_CHAN_SUCCESS) {
				/* Send successful response */
				rsp.result = cpu_to_le16(L2CAP_CR_SUCCESS);
				rsp.status = cpu_to_le16(L2CAP_CS_NO_INFO);
			} else {
				/* Send negative response */
				rsp.result = cpu_to_le16(L2CAP_CR_NO_MEM);
				rsp.status = cpu_to_le16(L2CAP_CS_NO_INFO);
			}

			l2cap_send_cmd(pi->conn, pi->ident,
					L2CAP_CREATE_CHAN_RSP,
					sizeof(rsp), &rsp);

			if (result == L2CAP_CREATE_CHAN_SUCCESS) {
				sk->sk_state = BT_CONFIG;
				pi->conf_state |= L2CAP_CONF_REQ_SENT;
				l2cap_send_cmd(pi->conn,
					l2cap_get_ident(pi->conn),
					L2CAP_CONF_REQ,
					l2cap_build_conf_req(sk, buf, sizeof(buf)), buf);
				l2cap_pi(sk)->num_conf_req++;
			}
		} else {
			/* Outgoing channel on AMP */
			if (result != L2CAP_CREATE_CHAN_SUCCESS) {
				/* Revert to BR/EDR connect */
				l2cap_send_conn_req(sk);
			} else {
				pi->amp_id = local_id;
				l2cap_send_create_chan_req(sk, remote_id);
			}
		}
	} else if (result == L2CAP_MOVE_CHAN_SUCCESS &&
		pi->amp_move_role == L2CAP_AMP_MOVE_INITIATOR) {
		l2cap_amp_move_setup(sk);
		pi->amp_move_id = local_id;
		pi->amp_move_state = L2CAP_AMP_STATE_WAIT_MOVE_RSP;

		l2cap_send_move_chan_req(pi->conn, pi, pi->scid, remote_id);
		l2cap_sock_set_timer(sk, L2CAP_MOVE_TIMEOUT);
	} else if (result == L2CAP_MOVE_CHAN_SUCCESS &&
		pi->amp_move_role == L2CAP_AMP_MOVE_RESPONDER) {
		struct hci_chan *chan;
		struct l2cap_conf_ext_fs default_fs = {1, 1, 0xFFFF,
				0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
		pi->remote_fs = default_fs;
		pi->local_fs = default_fs;
		chan = l2cap_chan_admit(local_id, sk);
		if (chan) {
			if (chan->state == BT_CONNECTED) {
				/* Logical link is ready to go */
				pi->ampcon = chan->conn;
				pi->ampcon->l2cap_data = pi->conn;
				pi->amp_move_state =
					L2CAP_AMP_STATE_WAIT_MOVE_CONFIRM;
				l2cap_send_move_chan_rsp(pi->conn,
					pi->amp_move_cmd_ident, pi->dcid,
					L2CAP_MOVE_CHAN_SUCCESS);

				l2cap_create_cfm(chan, 0);
			} else {
				/* Wait for logical link to be ready */
				pi->amp_move_state =
					L2CAP_AMP_STATE_WAIT_LOGICAL_CONFIRM;
			}
		} else {
			/* Logical link not available */
			l2cap_send_move_chan_rsp(pi->conn,
				pi->amp_move_cmd_ident, pi->dcid,
				L2CAP_MOVE_CHAN_REFUSED_NOT_ALLOWED);
		}
	} else {
		BT_DBG("result %d, role %d, local_busy %d", result,
			(int) pi->amp_move_role,
			(int) ((pi->conn_state & L2CAP_CONN_LOCAL_BUSY) != 0));

		if (pi->amp_move_role == L2CAP_AMP_MOVE_RESPONDER) {
			if (result == -EINVAL)
				l2cap_send_move_chan_rsp(pi->conn,
					pi->amp_move_cmd_ident, pi->dcid,
					L2CAP_MOVE_CHAN_REFUSED_CONTROLLER);
			else
				l2cap_send_move_chan_rsp(pi->conn,
					pi->amp_move_cmd_ident, pi->dcid,
					L2CAP_MOVE_CHAN_REFUSED_NOT_ALLOWED);
		}

		pi->amp_move_role = L2CAP_AMP_MOVE_NONE;
		pi->amp_move_state = L2CAP_AMP_STATE_STABLE;

		if ((l2cap_pi(sk)->conn_state & L2CAP_CONN_LOCAL_BUSY) &&
			l2cap_rmem_available(sk))
			l2cap_ertm_tx(sk, 0, 0,
					L2CAP_ERTM_EVENT_LOCAL_BUSY_CLEAR);

		/* Restart data transmission */
		l2cap_ertm_send(sk);
	}

	release_sock(sk);
}

static void l2cap_logical_link_complete(struct hci_chan *chan, u8 status)
{
	struct l2cap_pinfo *pi;
	struct sock *sk;
	struct hci_chan *ampchan;
	struct hci_conn *ampcon;

	BT_DBG("status %d, chan %p, conn %p", (int) status, chan, chan->conn);

	sk = chan->l2cap_sk;
	chan->l2cap_sk = NULL;

	BT_DBG("sk %p", sk);

	lock_sock(sk);

	if (sk->sk_state != BT_CONNECTED && !l2cap_pi(sk)->amp_id) {
		release_sock(sk);
		return;
	}

	pi = l2cap_pi(sk);

	if ((!status) && (chan != NULL)) {
		pi->ampcon = chan->conn;
		pi->ampcon->l2cap_data = pi->conn;

		BT_DBG("amp_move_state %d", pi->amp_move_state);

		if (sk->sk_state != BT_CONNECTED) {
			struct l2cap_conf_rsp rsp;

			/* Must use spinlock to prevent concurrent
			 * execution of l2cap_config_rsp()
			 */
			bh_lock_sock(sk);
			l2cap_send_cmd(pi->conn, pi->conf_ident, L2CAP_CONF_RSP,
					l2cap_build_conf_rsp(sk, &rsp,
						L2CAP_CONF_SUCCESS, 0), &rsp);
			pi->conf_state |= L2CAP_CONF_OUTPUT_DONE;

			if (l2cap_pi(sk)->conf_state & L2CAP_CONF_INPUT_DONE) {
				set_default_fcs(l2cap_pi(sk));

				sk->sk_state = BT_CONNECTED;

				if (l2cap_pi(sk)->mode == L2CAP_MODE_ERTM ||
				    l2cap_pi(sk)->mode == L2CAP_MODE_STREAMING)
					l2cap_ertm_init(sk);

				l2cap_chan_ready(sk);
			}
			bh_unlock_sock(sk);
		} else if (pi->amp_move_state ==
				L2CAP_AMP_STATE_WAIT_LOGICAL_COMPLETE) {
			/* Move confirm will be sent after a success
			 * response is received
			 */
			pi->amp_move_state =
				L2CAP_AMP_STATE_WAIT_MOVE_RSP_SUCCESS;
		} else if (pi->amp_move_state ==
				L2CAP_AMP_STATE_WAIT_LOGICAL_CONFIRM) {
			if (pi->conn_state & L2CAP_CONN_LOCAL_BUSY)
				pi->amp_move_state =
					L2CAP_AMP_STATE_WAIT_LOCAL_BUSY;
			else if (pi->amp_move_role ==
					L2CAP_AMP_MOVE_INITIATOR) {
				pi->amp_move_state =
					L2CAP_AMP_STATE_WAIT_MOVE_CONFIRM_RSP;
				l2cap_send_move_chan_cfm(pi->conn, pi, pi->scid,
					L2CAP_MOVE_CHAN_SUCCESS);
				l2cap_sock_set_timer(sk, L2CAP_MOVE_TIMEOUT);
			} else if (pi->amp_move_role ==
					L2CAP_AMP_MOVE_RESPONDER) {
				pi->amp_move_state =
					L2CAP_AMP_STATE_WAIT_MOVE_CONFIRM;
				l2cap_send_move_chan_rsp(pi->conn,
					pi->amp_move_cmd_ident, pi->dcid,
					L2CAP_MOVE_CHAN_SUCCESS);
			}
		} else if ((pi->amp_move_state !=
				L2CAP_AMP_STATE_WAIT_MOVE_RSP_SUCCESS) &&
			(pi->amp_move_state !=
				L2CAP_AMP_STATE_WAIT_MOVE_CONFIRM) &&
			(pi->amp_move_state !=
				L2CAP_AMP_STATE_WAIT_MOVE_CONFIRM_RSP)) {
			/* Move was not in expected state, free the channel */
			ampchan = pi->ampchan;
			ampcon = pi->ampcon;
			pi->ampchan = NULL;
			pi->ampcon = NULL;
			if (ampchan) {
				if (hci_chan_put(ampchan))
					ampcon->l2cap_data = NULL;
				else
					l2cap_deaggregate(ampchan, pi);
			}
			pi->amp_move_state = L2CAP_AMP_STATE_STABLE;
		}
	} else {
		/* Logical link setup failed. */

		if (sk->sk_state != BT_CONNECTED)
			l2cap_send_disconn_req(pi->conn, sk, ECONNRESET);
		else if (pi->amp_move_role == L2CAP_AMP_MOVE_RESPONDER) {
			l2cap_amp_move_revert(sk);
			l2cap_pi(sk)->amp_move_role = L2CAP_AMP_MOVE_NONE;
			pi->amp_move_state = L2CAP_AMP_STATE_STABLE;
			l2cap_send_move_chan_rsp(pi->conn,
					pi->amp_move_cmd_ident, pi->dcid,
					L2CAP_MOVE_CHAN_REFUSED_CONFIG);
		} else if (pi->amp_move_role == L2CAP_AMP_MOVE_INITIATOR) {
			if ((pi->amp_move_state ==
				L2CAP_AMP_STATE_WAIT_LOGICAL_COMPLETE) ||
				(pi->amp_move_state ==
				    L2CAP_AMP_STATE_WAIT_LOGICAL_CONFIRM)) {
				/* Remote has only sent pending or
				 * success responses, clean up
				 */
				l2cap_amp_move_revert(sk);
				l2cap_pi(sk)->amp_move_role =
					L2CAP_AMP_MOVE_NONE;
				pi->amp_move_state = L2CAP_AMP_STATE_STABLE;
			}

			/* Other amp move states imply that the move
			 * has already aborted
			 */
			l2cap_send_move_chan_cfm(pi->conn, pi, pi->scid,
						L2CAP_MOVE_CHAN_UNCONFIRMED);
			l2cap_sock_set_timer(sk, L2CAP_MOVE_TIMEOUT);
		}
		ampchan = pi->ampchan;
		ampcon = pi->ampcon;
		pi->ampchan = NULL;
		pi->ampcon = NULL;
		if (ampchan) {
			if (hci_chan_put(ampchan))
				ampcon->l2cap_data = NULL;
			else
				l2cap_deaggregate(ampchan, pi);
		}
	}

	release_sock(sk);
}

static void l2cap_logical_link_worker(struct work_struct *work)
{
	struct l2cap_logical_link_work *log_link_work =
		container_of(work, struct l2cap_logical_link_work, work);
	struct sock *sk = log_link_work->chan->l2cap_sk;

	if (sk) {
		l2cap_logical_link_complete(log_link_work->chan,
							log_link_work->status);
		sock_put(sk);
	}
	hci_chan_put(log_link_work->chan);
	kfree(log_link_work);
}

static int l2cap_create_cfm(struct hci_chan *chan, u8 status)
{
	struct l2cap_logical_link_work *amp_work;

	if (!chan->l2cap_sk) {
		BT_ERR("Expected l2cap_sk to point to connecting socket");
		return -EFAULT;
	}

	amp_work = kzalloc(sizeof(*amp_work), GFP_ATOMIC);
	if (!amp_work) {
		sock_put(chan->l2cap_sk);
		return -ENOMEM;
	}

	INIT_WORK(&amp_work->work, l2cap_logical_link_worker);
	amp_work->chan = chan;
	amp_work->status = status;

	hci_chan_hold(chan);

	if (!queue_work(_l2cap_wq, &amp_work->work)) {
		kfree(amp_work);
		sock_put(chan->l2cap_sk);
		hci_chan_put(chan);
		return -ENOMEM;
	}

	return 0;
}

int l2cap_modify_cfm(struct hci_chan *chan, u8 status)
{
	struct l2cap_conn *conn = chan->conn->l2cap_data;

	BT_DBG("chan %p conn %p status %d", chan, conn, status);

	/* TODO: if failed status restore previous fs */
	return 0;
}

int l2cap_destroy_cfm(struct hci_chan *chan, u8 reason)
{
	struct l2cap_chan_list *l;
	struct l2cap_conn *conn = chan->conn->l2cap_data;
	struct sock *sk;

	BT_DBG("chan %p conn %p", chan, conn);

	if (!conn)
		return 0;

	l = &conn->chan_list;

	read_lock(&l->lock);

	for (sk = l->head; sk; sk = l2cap_pi(sk)->next_c) {
		bh_lock_sock(sk);
		/* TODO MM/PK - What to do if connection is LOCAL_BUSY?  */
		if (l2cap_pi(sk)->ampchan == chan) {
			struct hci_conn *ampcon = l2cap_pi(sk)->ampcon;
			l2cap_pi(sk)->ampchan = NULL;
			l2cap_pi(sk)->ampcon = NULL;
			if (hci_chan_put(chan))
				ampcon->l2cap_data = NULL;
			else
				l2cap_deaggregate(chan, l2cap_pi(sk));

			l2cap_amp_move_init(sk);
		}
		bh_unlock_sock(sk);
	}

	read_unlock(&l->lock);

	return 0;


}

static int l2cap_sig_amp(struct l2cap_conn *conn, struct l2cap_cmd_hdr *cmd,
			u16 cmd_len, u8 *data, struct sk_buff *skb)
{
	struct l2cap_amp_signal_work *amp_work;

	if (cmd_len < sizeof(struct l2cap_conn_req))
		return -EPROTO;

	amp_work = kzalloc(sizeof(*amp_work), GFP_ATOMIC);
	if (!amp_work)
		return -ENOMEM;

	INIT_WORK(&amp_work->work, l2cap_amp_signal_worker);
	amp_work->conn = conn;
	amp_work->cmd = *cmd;
	amp_work->data = data;
	amp_work->skb = skb_clone(skb, GFP_ATOMIC);
	if (!amp_work->skb) {
		kfree(amp_work);
		return -ENOMEM;
	}

	if (!queue_work(_l2cap_wq, &amp_work->work)) {
		kfree_skb(amp_work->skb);
		kfree(amp_work);
		return -ENOMEM;
	}

	return 0;
}

static inline int l2cap_check_conn_param(u16 min, u16 max, u16 latency,
							u16 to_multiplier)
{
	u16 max_latency;

	if (min > max || min < 6 || max > 3200)
		return -EINVAL;

	if (to_multiplier < 10 || to_multiplier > 3200)
		return -EINVAL;

	if (max >= to_multiplier * 8)
		return -EINVAL;

	max_latency = (to_multiplier * 8 / max) - 1;
	if (latency > 499 || latency > max_latency)
		return -EINVAL;

=======
		break;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static inline int l2cap_conn_param_update_req(struct l2cap_conn *conn,
<<<<<<< HEAD
					struct l2cap_cmd_hdr *cmd, u8 *data)
=======
					      struct l2cap_cmd_hdr *cmd,
					      u16 cmd_len, u8 *data)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct hci_conn *hcon = conn->hcon;
	struct l2cap_conn_param_update_req *req;
	struct l2cap_conn_param_update_rsp rsp;
<<<<<<< HEAD
	struct sock *sk;
	u16 min, max, latency, timeout, cmd_len;
	int err;

	if (!(hcon->link_mode & HCI_LM_MASTER))
		return -EINVAL;

	cmd_len = __le16_to_cpu(cmd->len);
	if (cmd_len != sizeof(struct l2cap_conn_param_update_req))
		return -EPROTO;

	memset(&rsp, 0, sizeof(rsp));
	rsp.result = cpu_to_le16(L2CAP_CONN_PARAM_REJECTED);

	sk = l2cap_find_sock_by_fixed_cid_and_dir(4, conn->src, conn->dst, 0);

	if (sk && !bt_sk(sk)->le_params.prohibit_remote_chg) {
		req = (struct l2cap_conn_param_update_req *) data;
		min = __le16_to_cpu(req->min);
		max = __le16_to_cpu(req->max);
		latency = __le16_to_cpu(req->latency);
		timeout = __le16_to_cpu(req->to_multiplier);

		err = l2cap_check_conn_param(min, max, latency, timeout);
		if (!err) {
			rsp.result = cpu_to_le16(L2CAP_CONN_PARAM_ACCEPTED);
			hci_le_conn_update(hcon, min, max, latency, timeout);
			bt_sk(sk)->le_params.interval_min = min;
			bt_sk(sk)->le_params.interval_max = max;
			bt_sk(sk)->le_params.latency = latency;
			bt_sk(sk)->le_params.supervision_timeout = timeout;
		}
	}

	l2cap_send_cmd(conn, cmd->ident, L2CAP_CONN_PARAM_UPDATE_RSP,
							sizeof(rsp), &rsp);

=======
	u16 min, max, latency, to_multiplier;
	int err;

	if (hcon->role != HCI_ROLE_MASTER)
		return -EINVAL;

	if (cmd_len != sizeof(struct l2cap_conn_param_update_req))
		return -EPROTO;

	req = (struct l2cap_conn_param_update_req *) data;
	min		= __le16_to_cpu(req->min);
	max		= __le16_to_cpu(req->max);
	latency		= __le16_to_cpu(req->latency);
	to_multiplier	= __le16_to_cpu(req->to_multiplier);

	BT_DBG("min 0x%4.4x max 0x%4.4x latency: 0x%4.4x Timeout: 0x%4.4x",
	       min, max, latency, to_multiplier);

	memset(&rsp, 0, sizeof(rsp));

	if (max > hcon->le_conn_max_interval) {
		BT_DBG("requested connection interval exceeds current bounds.");
		err = -EINVAL;
	} else {
		err = hci_check_conn_params(min, max, latency, to_multiplier);
	}

	if (err)
		rsp.result = cpu_to_le16(L2CAP_CONN_PARAM_REJECTED);
	else
		rsp.result = cpu_to_le16(L2CAP_CONN_PARAM_ACCEPTED);

	l2cap_send_cmd(conn, cmd->ident, L2CAP_CONN_PARAM_UPDATE_RSP,
		       sizeof(rsp), &rsp);

	if (!err) {
		u8 store_hint;

		store_hint = hci_le_conn_update(hcon, min, max, latency,
						to_multiplier);
		mgmt_new_conn_param(hcon->hdev, &hcon->dst, hcon->dst_type,
				    store_hint, min, max, latency,
				    to_multiplier);

	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static inline int l2cap_bredr_sig_cmd(struct l2cap_conn *conn,
			struct l2cap_cmd_hdr *cmd, u16 cmd_len, u8 *data,
			struct sk_buff *skb)
=======
static int l2cap_le_connect_rsp(struct l2cap_conn *conn,
				struct l2cap_cmd_hdr *cmd, u16 cmd_len,
				u8 *data)
{
	struct l2cap_le_conn_rsp *rsp = (struct l2cap_le_conn_rsp *) data;
	struct hci_conn *hcon = conn->hcon;
	u16 dcid, mtu, mps, credits, result;
	struct l2cap_chan *chan;
	int err, sec_level;

	if (cmd_len < sizeof(*rsp))
		return -EPROTO;

	dcid    = __le16_to_cpu(rsp->dcid);
	mtu     = __le16_to_cpu(rsp->mtu);
	mps     = __le16_to_cpu(rsp->mps);
	credits = __le16_to_cpu(rsp->credits);
	result  = __le16_to_cpu(rsp->result);

	if (result == L2CAP_CR_LE_SUCCESS && (mtu < 23 || mps < 23 ||
					   dcid < L2CAP_CID_DYN_START ||
					   dcid > L2CAP_CID_LE_DYN_END))
		return -EPROTO;

	BT_DBG("dcid 0x%4.4x mtu %u mps %u credits %u result 0x%2.2x",
	       dcid, mtu, mps, credits, result);

	mutex_lock(&conn->chan_lock);

	chan = __l2cap_get_chan_by_ident(conn, cmd->ident);
	if (!chan) {
		err = -EBADSLT;
		goto unlock;
	}

	err = 0;

	l2cap_chan_lock(chan);

	switch (result) {
	case L2CAP_CR_LE_SUCCESS:
		if (__l2cap_get_chan_by_dcid(conn, dcid)) {
			err = -EBADSLT;
			break;
		}

		chan->ident = 0;
		chan->dcid = dcid;
		chan->omtu = mtu;
		chan->remote_mps = mps;
		chan->tx_credits = credits;
		l2cap_chan_ready(chan);
		break;

	case L2CAP_CR_LE_AUTHENTICATION:
	case L2CAP_CR_LE_ENCRYPTION:
		/* If we already have MITM protection we can't do
		 * anything.
		 */
		if (hcon->sec_level > BT_SECURITY_MEDIUM) {
			l2cap_chan_del(chan, ECONNREFUSED);
			break;
		}

		sec_level = hcon->sec_level + 1;
		if (chan->sec_level < sec_level)
			chan->sec_level = sec_level;

		/* We'll need to send a new Connect Request */
		clear_bit(FLAG_LE_CONN_REQ_SENT, &chan->flags);

		smp_conn_security(hcon, chan->sec_level);
		break;

	default:
		l2cap_chan_del(chan, ECONNREFUSED);
		break;
	}

	l2cap_chan_unlock(chan);

unlock:
	mutex_unlock(&conn->chan_lock);

	return err;
}

static inline int l2cap_bredr_sig_cmd(struct l2cap_conn *conn,
				      struct l2cap_cmd_hdr *cmd, u16 cmd_len,
				      u8 *data)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int err = 0;

	switch (cmd->code) {
	case L2CAP_COMMAND_REJ:
		l2cap_command_rej(conn, cmd, cmd_len, data);
		break;

	case L2CAP_CONN_REQ:
		err = l2cap_connect_req(conn, cmd, cmd_len, data);
		break;

	case L2CAP_CONN_RSP:
<<<<<<< HEAD
		err = l2cap_connect_rsp(conn, cmd, cmd_len, data);
=======
		l2cap_connect_create_rsp(conn, cmd, cmd_len, data);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;

	case L2CAP_CONF_REQ:
		err = l2cap_config_req(conn, cmd, cmd_len, data);
		break;

	case L2CAP_CONF_RSP:
<<<<<<< HEAD
		err = l2cap_config_rsp(conn, cmd, cmd_len, data);
=======
		l2cap_config_rsp(conn, cmd, cmd_len, data);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;

	case L2CAP_DISCONN_REQ:
		err = l2cap_disconnect_req(conn, cmd, cmd_len, data);
		break;

	case L2CAP_DISCONN_RSP:
<<<<<<< HEAD
		err = l2cap_disconnect_rsp(conn, cmd, cmd_len, data);
=======
		l2cap_disconnect_rsp(conn, cmd, cmd_len, data);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;

	case L2CAP_ECHO_REQ:
		l2cap_send_cmd(conn, cmd->ident, L2CAP_ECHO_RSP, cmd_len, data);
		break;

	case L2CAP_ECHO_RSP:
		break;

	case L2CAP_INFO_REQ:
		err = l2cap_information_req(conn, cmd, cmd_len, data);
		break;

	case L2CAP_INFO_RSP:
<<<<<<< HEAD
		err = l2cap_information_rsp(conn, cmd, cmd_len, data);
		break;

	case L2CAP_CREATE_CHAN_REQ:
		err = l2cap_create_channel_req(conn, cmd, cmd_len, data);
		break;

	case L2CAP_CREATE_CHAN_RSP:
		err = l2cap_create_channel_rsp(conn, cmd, cmd_len, data);
		break;

	case L2CAP_MOVE_CHAN_REQ:
	case L2CAP_MOVE_CHAN_RSP:
	case L2CAP_MOVE_CHAN_CFM:
	case L2CAP_MOVE_CHAN_CFM_RSP:
		err = l2cap_sig_amp(conn, cmd, cmd_len, data, skb);
		break;
=======
		l2cap_information_rsp(conn, cmd, cmd_len, data);
		break;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	default:
		BT_ERR("Unknown BR/EDR signaling command 0x%2.2x", cmd->code);
		err = -EINVAL;
		break;
	}

	return err;
}

<<<<<<< HEAD
static inline int l2cap_le_sig_cmd(struct l2cap_conn *conn,
					struct l2cap_cmd_hdr *cmd, u8 *data)
{
	switch (cmd->code) {
	case L2CAP_COMMAND_REJ:
		return 0;

	case L2CAP_CONN_PARAM_UPDATE_REQ:
		return l2cap_conn_param_update_req(conn, cmd, data);

	case L2CAP_CONN_PARAM_UPDATE_RSP:
		return 0;

	default:
		BT_ERR("Unknown LE signaling command 0x%2.2x", cmd->code);
		return -EINVAL;
	}
}

static inline void l2cap_sig_channel(struct l2cap_conn *conn,
							struct sk_buff *skb)
{
	u8 *data = skb->data;
	int len = skb->len;
	struct l2cap_cmd_hdr cmd;
=======
static int l2cap_le_connect_req(struct l2cap_conn *conn,
				struct l2cap_cmd_hdr *cmd, u16 cmd_len,
				u8 *data)
{
	struct l2cap_le_conn_req *req = (struct l2cap_le_conn_req *) data;
	struct l2cap_le_conn_rsp rsp;
	struct l2cap_chan *chan, *pchan;
	u16 dcid, scid, credits, mtu, mps;
	__le16 psm;
	u8 result;

	if (cmd_len != sizeof(*req))
		return -EPROTO;

	scid = __le16_to_cpu(req->scid);
	mtu  = __le16_to_cpu(req->mtu);
	mps  = __le16_to_cpu(req->mps);
	psm  = req->psm;
	dcid = 0;
	credits = 0;

	if (mtu < 23 || mps < 23)
		return -EPROTO;

	BT_DBG("psm 0x%2.2x scid 0x%4.4x mtu %u mps %u", __le16_to_cpu(psm),
	       scid, mtu, mps);

	/* BLUETOOTH CORE SPECIFICATION Version 5.3 | Vol 3, Part A
	 * page 1059:
	 *
	 * Valid range: 0x0001-0x00ff
	 *
	 * Table 4.15: L2CAP_LE_CREDIT_BASED_CONNECTION_REQ SPSM ranges
	 */
	if (!psm || __le16_to_cpu(psm) > L2CAP_PSM_LE_DYN_END) {
		result = L2CAP_CR_LE_BAD_PSM;
		chan = NULL;
		goto response;
	}

	/* Check if we have socket listening on psm */
	pchan = l2cap_global_chan_by_psm(BT_LISTEN, psm, &conn->hcon->src,
					 &conn->hcon->dst, LE_LINK);
	if (!pchan) {
		result = L2CAP_CR_LE_BAD_PSM;
		chan = NULL;
		goto response;
	}

	mutex_lock(&conn->chan_lock);
	l2cap_chan_lock(pchan);

	if (!smp_sufficient_security(conn->hcon, pchan->sec_level,
				     SMP_ALLOW_STK)) {
		result = L2CAP_CR_LE_AUTHENTICATION;
		chan = NULL;
		goto response_unlock;
	}

	/* Check for valid dynamic CID range */
	if (scid < L2CAP_CID_DYN_START || scid > L2CAP_CID_LE_DYN_END) {
		result = L2CAP_CR_LE_INVALID_SCID;
		chan = NULL;
		goto response_unlock;
	}

	/* Check if we already have channel with that dcid */
	if (__l2cap_get_chan_by_dcid(conn, scid)) {
		result = L2CAP_CR_LE_SCID_IN_USE;
		chan = NULL;
		goto response_unlock;
	}

	chan = pchan->ops->new_connection(pchan);
	if (!chan) {
		result = L2CAP_CR_LE_NO_MEM;
		goto response_unlock;
	}

	bacpy(&chan->src, &conn->hcon->src);
	bacpy(&chan->dst, &conn->hcon->dst);
	chan->src_type = bdaddr_src_type(conn->hcon);
	chan->dst_type = bdaddr_dst_type(conn->hcon);
	chan->psm  = psm;
	chan->dcid = scid;
	chan->omtu = mtu;
	chan->remote_mps = mps;

	__l2cap_chan_add(conn, chan);

	l2cap_le_flowctl_init(chan, __le16_to_cpu(req->credits));

	dcid = chan->scid;
	credits = chan->rx_credits;

	__set_chan_timer(chan, chan->ops->get_sndtimeo(chan));

	chan->ident = cmd->ident;

	if (test_bit(FLAG_DEFER_SETUP, &chan->flags)) {
		l2cap_state_change(chan, BT_CONNECT2);
		/* The following result value is actually not defined
		 * for LE CoC but we use it to let the function know
		 * that it should bail out after doing its cleanup
		 * instead of sending a response.
		 */
		result = L2CAP_CR_PEND;
		chan->ops->defer(chan);
	} else {
		l2cap_chan_ready(chan);
		result = L2CAP_CR_LE_SUCCESS;
	}

response_unlock:
	l2cap_chan_unlock(pchan);
	mutex_unlock(&conn->chan_lock);
	l2cap_chan_put(pchan);

	if (result == L2CAP_CR_PEND)
		return 0;

response:
	if (chan) {
		rsp.mtu = cpu_to_le16(chan->imtu);
		rsp.mps = cpu_to_le16(chan->mps);
	} else {
		rsp.mtu = 0;
		rsp.mps = 0;
	}

	rsp.dcid    = cpu_to_le16(dcid);
	rsp.credits = cpu_to_le16(credits);
	rsp.result  = cpu_to_le16(result);

	l2cap_send_cmd(conn, cmd->ident, L2CAP_LE_CONN_RSP, sizeof(rsp), &rsp);

	return 0;
}

static inline int l2cap_le_credits(struct l2cap_conn *conn,
				   struct l2cap_cmd_hdr *cmd, u16 cmd_len,
				   u8 *data)
{
	struct l2cap_le_credits *pkt;
	struct l2cap_chan *chan;
	u16 cid, credits, max_credits;

	if (cmd_len != sizeof(*pkt))
		return -EPROTO;

	pkt = (struct l2cap_le_credits *) data;
	cid	= __le16_to_cpu(pkt->cid);
	credits	= __le16_to_cpu(pkt->credits);

	BT_DBG("cid 0x%4.4x credits 0x%4.4x", cid, credits);

	chan = l2cap_get_chan_by_dcid(conn, cid);
	if (!chan)
		return -EBADSLT;

	max_credits = LE_FLOWCTL_MAX_CREDITS - chan->tx_credits;
	if (credits > max_credits) {
		BT_ERR("LE credits overflow");
		l2cap_send_disconn_req(chan, ECONNRESET);

		/* Return 0 so that we don't trigger an unnecessary
		 * command reject packet.
		 */
		goto unlock;
	}

	chan->tx_credits += credits;

	/* Resume sending */
	l2cap_le_flowctl_send(chan);

	if (chan->tx_credits)
		chan->ops->resume(chan);

unlock:
	l2cap_chan_unlock(chan);
	l2cap_chan_put(chan);

	return 0;
}

static inline int l2cap_ecred_conn_req(struct l2cap_conn *conn,
				       struct l2cap_cmd_hdr *cmd, u16 cmd_len,
				       u8 *data)
{
	struct l2cap_ecred_conn_req *req = (void *) data;
	struct {
		struct l2cap_ecred_conn_rsp rsp;
		__le16 dcid[L2CAP_ECRED_MAX_CID];
	} __packed pdu;
	struct l2cap_chan *chan, *pchan;
	u16 mtu, mps;
	__le16 psm;
	u8 result, len = 0;
	int i, num_scid;
	bool defer = false;

	if (!enable_ecred)
		return -EINVAL;

	if (cmd_len < sizeof(*req) || (cmd_len - sizeof(*req)) % sizeof(u16)) {
		result = L2CAP_CR_LE_INVALID_PARAMS;
		goto response;
	}

	cmd_len -= sizeof(*req);
	num_scid = cmd_len / sizeof(u16);

	if (num_scid > ARRAY_SIZE(pdu.dcid)) {
		result = L2CAP_CR_LE_INVALID_PARAMS;
		goto response;
	}

	mtu  = __le16_to_cpu(req->mtu);
	mps  = __le16_to_cpu(req->mps);

	if (mtu < L2CAP_ECRED_MIN_MTU || mps < L2CAP_ECRED_MIN_MPS) {
		result = L2CAP_CR_LE_UNACCEPT_PARAMS;
		goto response;
	}

	psm  = req->psm;

	/* BLUETOOTH CORE SPECIFICATION Version 5.3 | Vol 3, Part A
	 * page 1059:
	 *
	 * Valid range: 0x0001-0x00ff
	 *
	 * Table 4.15: L2CAP_LE_CREDIT_BASED_CONNECTION_REQ SPSM ranges
	 */
	if (!psm || __le16_to_cpu(psm) > L2CAP_PSM_LE_DYN_END) {
		result = L2CAP_CR_LE_BAD_PSM;
		goto response;
	}

	BT_DBG("psm 0x%2.2x mtu %u mps %u", __le16_to_cpu(psm), mtu, mps);

	memset(&pdu, 0, sizeof(pdu));

	/* Check if we have socket listening on psm */
	pchan = l2cap_global_chan_by_psm(BT_LISTEN, psm, &conn->hcon->src,
					 &conn->hcon->dst, LE_LINK);
	if (!pchan) {
		result = L2CAP_CR_LE_BAD_PSM;
		goto response;
	}

	mutex_lock(&conn->chan_lock);
	l2cap_chan_lock(pchan);

	if (!smp_sufficient_security(conn->hcon, pchan->sec_level,
				     SMP_ALLOW_STK)) {
		result = L2CAP_CR_LE_AUTHENTICATION;
		goto unlock;
	}

	result = L2CAP_CR_LE_SUCCESS;

	for (i = 0; i < num_scid; i++) {
		u16 scid = __le16_to_cpu(req->scid[i]);

		BT_DBG("scid[%d] 0x%4.4x", i, scid);

		pdu.dcid[i] = 0x0000;
		len += sizeof(*pdu.dcid);

		/* Check for valid dynamic CID range */
		if (scid < L2CAP_CID_DYN_START || scid > L2CAP_CID_LE_DYN_END) {
			result = L2CAP_CR_LE_INVALID_SCID;
			continue;
		}

		/* Check if we already have channel with that dcid */
		if (__l2cap_get_chan_by_dcid(conn, scid)) {
			result = L2CAP_CR_LE_SCID_IN_USE;
			continue;
		}

		chan = pchan->ops->new_connection(pchan);
		if (!chan) {
			result = L2CAP_CR_LE_NO_MEM;
			continue;
		}

		bacpy(&chan->src, &conn->hcon->src);
		bacpy(&chan->dst, &conn->hcon->dst);
		chan->src_type = bdaddr_src_type(conn->hcon);
		chan->dst_type = bdaddr_dst_type(conn->hcon);
		chan->psm  = psm;
		chan->dcid = scid;
		chan->omtu = mtu;
		chan->remote_mps = mps;

		__l2cap_chan_add(conn, chan);

		l2cap_ecred_init(chan, __le16_to_cpu(req->credits));

		/* Init response */
		if (!pdu.rsp.credits) {
			pdu.rsp.mtu = cpu_to_le16(chan->imtu);
			pdu.rsp.mps = cpu_to_le16(chan->mps);
			pdu.rsp.credits = cpu_to_le16(chan->rx_credits);
		}

		pdu.dcid[i] = cpu_to_le16(chan->scid);

		__set_chan_timer(chan, chan->ops->get_sndtimeo(chan));

		chan->ident = cmd->ident;
		chan->mode = L2CAP_MODE_EXT_FLOWCTL;

		if (test_bit(FLAG_DEFER_SETUP, &chan->flags)) {
			l2cap_state_change(chan, BT_CONNECT2);
			defer = true;
			chan->ops->defer(chan);
		} else {
			l2cap_chan_ready(chan);
		}
	}

unlock:
	l2cap_chan_unlock(pchan);
	mutex_unlock(&conn->chan_lock);
	l2cap_chan_put(pchan);

response:
	pdu.rsp.result = cpu_to_le16(result);

	if (defer)
		return 0;

	l2cap_send_cmd(conn, cmd->ident, L2CAP_ECRED_CONN_RSP,
		       sizeof(pdu.rsp) + len, &pdu);

	return 0;
}

static inline int l2cap_ecred_conn_rsp(struct l2cap_conn *conn,
				       struct l2cap_cmd_hdr *cmd, u16 cmd_len,
				       u8 *data)
{
	struct l2cap_ecred_conn_rsp *rsp = (void *) data;
	struct hci_conn *hcon = conn->hcon;
	u16 mtu, mps, credits, result;
	struct l2cap_chan *chan, *tmp;
	int err = 0, sec_level;
	int i = 0;

	if (cmd_len < sizeof(*rsp))
		return -EPROTO;

	mtu     = __le16_to_cpu(rsp->mtu);
	mps     = __le16_to_cpu(rsp->mps);
	credits = __le16_to_cpu(rsp->credits);
	result  = __le16_to_cpu(rsp->result);

	BT_DBG("mtu %u mps %u credits %u result 0x%4.4x", mtu, mps, credits,
	       result);

	mutex_lock(&conn->chan_lock);

	cmd_len -= sizeof(*rsp);

	list_for_each_entry_safe(chan, tmp, &conn->chan_l, list) {
		u16 dcid;

		if (chan->ident != cmd->ident ||
		    chan->mode != L2CAP_MODE_EXT_FLOWCTL ||
		    chan->state == BT_CONNECTED)
			continue;

		l2cap_chan_lock(chan);

		/* Check that there is a dcid for each pending channel */
		if (cmd_len < sizeof(dcid)) {
			l2cap_chan_del(chan, ECONNREFUSED);
			l2cap_chan_unlock(chan);
			continue;
		}

		dcid = __le16_to_cpu(rsp->dcid[i++]);
		cmd_len -= sizeof(u16);

		BT_DBG("dcid[%d] 0x%4.4x", i, dcid);

		/* Check if dcid is already in use */
		if (dcid && __l2cap_get_chan_by_dcid(conn, dcid)) {
			/* If a device receives a
			 * L2CAP_CREDIT_BASED_CONNECTION_RSP packet with an
			 * already-assigned Destination CID, then both the
			 * original channel and the new channel shall be
			 * immediately discarded and not used.
			 */
			l2cap_chan_del(chan, ECONNREFUSED);
			l2cap_chan_unlock(chan);
			chan = __l2cap_get_chan_by_dcid(conn, dcid);
			l2cap_chan_lock(chan);
			l2cap_chan_del(chan, ECONNRESET);
			l2cap_chan_unlock(chan);
			continue;
		}

		switch (result) {
		case L2CAP_CR_LE_AUTHENTICATION:
		case L2CAP_CR_LE_ENCRYPTION:
			/* If we already have MITM protection we can't do
			 * anything.
			 */
			if (hcon->sec_level > BT_SECURITY_MEDIUM) {
				l2cap_chan_del(chan, ECONNREFUSED);
				break;
			}

			sec_level = hcon->sec_level + 1;
			if (chan->sec_level < sec_level)
				chan->sec_level = sec_level;

			/* We'll need to send a new Connect Request */
			clear_bit(FLAG_ECRED_CONN_REQ_SENT, &chan->flags);

			smp_conn_security(hcon, chan->sec_level);
			break;

		case L2CAP_CR_LE_BAD_PSM:
			l2cap_chan_del(chan, ECONNREFUSED);
			break;

		default:
			/* If dcid was not set it means channels was refused */
			if (!dcid) {
				l2cap_chan_del(chan, ECONNREFUSED);
				break;
			}

			chan->ident = 0;
			chan->dcid = dcid;
			chan->omtu = mtu;
			chan->remote_mps = mps;
			chan->tx_credits = credits;
			l2cap_chan_ready(chan);
			break;
		}

		l2cap_chan_unlock(chan);
	}

	mutex_unlock(&conn->chan_lock);

	return err;
}

static inline int l2cap_ecred_reconf_req(struct l2cap_conn *conn,
					 struct l2cap_cmd_hdr *cmd, u16 cmd_len,
					 u8 *data)
{
	struct l2cap_ecred_reconf_req *req = (void *) data;
	struct l2cap_ecred_reconf_rsp rsp;
	u16 mtu, mps, result;
	struct l2cap_chan *chan;
	int i, num_scid;

	if (!enable_ecred)
		return -EINVAL;

	if (cmd_len < sizeof(*req) || cmd_len - sizeof(*req) % sizeof(u16)) {
		result = L2CAP_CR_LE_INVALID_PARAMS;
		goto respond;
	}

	mtu = __le16_to_cpu(req->mtu);
	mps = __le16_to_cpu(req->mps);

	BT_DBG("mtu %u mps %u", mtu, mps);

	if (mtu < L2CAP_ECRED_MIN_MTU) {
		result = L2CAP_RECONF_INVALID_MTU;
		goto respond;
	}

	if (mps < L2CAP_ECRED_MIN_MPS) {
		result = L2CAP_RECONF_INVALID_MPS;
		goto respond;
	}

	cmd_len -= sizeof(*req);
	num_scid = cmd_len / sizeof(u16);
	result = L2CAP_RECONF_SUCCESS;

	for (i = 0; i < num_scid; i++) {
		u16 scid;

		scid = __le16_to_cpu(req->scid[i]);
		if (!scid)
			return -EPROTO;

		chan = __l2cap_get_chan_by_dcid(conn, scid);
		if (!chan)
			continue;

		/* If the MTU value is decreased for any of the included
		 * channels, then the receiver shall disconnect all
		 * included channels.
		 */
		if (chan->omtu > mtu) {
			BT_ERR("chan %p decreased MTU %u -> %u", chan,
			       chan->omtu, mtu);
			result = L2CAP_RECONF_INVALID_MTU;
		}

		chan->omtu = mtu;
		chan->remote_mps = mps;
	}

respond:
	rsp.result = cpu_to_le16(result);

	l2cap_send_cmd(conn, cmd->ident, L2CAP_ECRED_RECONF_RSP, sizeof(rsp),
		       &rsp);

	return 0;
}

static inline int l2cap_ecred_reconf_rsp(struct l2cap_conn *conn,
					 struct l2cap_cmd_hdr *cmd, u16 cmd_len,
					 u8 *data)
{
	struct l2cap_chan *chan, *tmp;
	struct l2cap_ecred_conn_rsp *rsp = (void *) data;
	u16 result;

	if (cmd_len < sizeof(*rsp))
		return -EPROTO;

	result = __le16_to_cpu(rsp->result);

	BT_DBG("result 0x%4.4x", rsp->result);

	if (!result)
		return 0;

	list_for_each_entry_safe(chan, tmp, &conn->chan_l, list) {
		if (chan->ident != cmd->ident)
			continue;

		l2cap_chan_del(chan, ECONNRESET);
	}

	return 0;
}

static inline int l2cap_le_command_rej(struct l2cap_conn *conn,
				       struct l2cap_cmd_hdr *cmd, u16 cmd_len,
				       u8 *data)
{
	struct l2cap_cmd_rej_unk *rej = (struct l2cap_cmd_rej_unk *) data;
	struct l2cap_chan *chan;

	if (cmd_len < sizeof(*rej))
		return -EPROTO;

	mutex_lock(&conn->chan_lock);

	chan = __l2cap_get_chan_by_ident(conn, cmd->ident);
	if (!chan)
		goto done;

	chan = l2cap_chan_hold_unless_zero(chan);
	if (!chan)
		goto done;

	l2cap_chan_lock(chan);
	l2cap_chan_del(chan, ECONNREFUSED);
	l2cap_chan_unlock(chan);
	l2cap_chan_put(chan);

done:
	mutex_unlock(&conn->chan_lock);
	return 0;
}

static inline int l2cap_le_sig_cmd(struct l2cap_conn *conn,
				   struct l2cap_cmd_hdr *cmd, u16 cmd_len,
				   u8 *data)
{
	int err = 0;

	switch (cmd->code) {
	case L2CAP_COMMAND_REJ:
		l2cap_le_command_rej(conn, cmd, cmd_len, data);
		break;

	case L2CAP_CONN_PARAM_UPDATE_REQ:
		err = l2cap_conn_param_update_req(conn, cmd, cmd_len, data);
		break;

	case L2CAP_CONN_PARAM_UPDATE_RSP:
		break;

	case L2CAP_LE_CONN_RSP:
		l2cap_le_connect_rsp(conn, cmd, cmd_len, data);
		break;

	case L2CAP_LE_CONN_REQ:
		err = l2cap_le_connect_req(conn, cmd, cmd_len, data);
		break;

	case L2CAP_LE_CREDITS:
		err = l2cap_le_credits(conn, cmd, cmd_len, data);
		break;

	case L2CAP_ECRED_CONN_REQ:
		err = l2cap_ecred_conn_req(conn, cmd, cmd_len, data);
		break;

	case L2CAP_ECRED_CONN_RSP:
		err = l2cap_ecred_conn_rsp(conn, cmd, cmd_len, data);
		break;

	case L2CAP_ECRED_RECONF_REQ:
		err = l2cap_ecred_reconf_req(conn, cmd, cmd_len, data);
		break;

	case L2CAP_ECRED_RECONF_RSP:
		err = l2cap_ecred_reconf_rsp(conn, cmd, cmd_len, data);
		break;

	case L2CAP_DISCONN_REQ:
		err = l2cap_disconnect_req(conn, cmd, cmd_len, data);
		break;

	case L2CAP_DISCONN_RSP:
		l2cap_disconnect_rsp(conn, cmd, cmd_len, data);
		break;

	default:
		BT_ERR("Unknown LE signaling command 0x%2.2x", cmd->code);
		err = -EINVAL;
		break;
	}

	return err;
}

static inline void l2cap_le_sig_channel(struct l2cap_conn *conn,
					struct sk_buff *skb)
{
	struct hci_conn *hcon = conn->hcon;
	struct l2cap_cmd_hdr *cmd;
	u16 len;
	int err;

	if (hcon->type != LE_LINK)
		goto drop;

	if (skb->len < L2CAP_CMD_HDR_SIZE)
		goto drop;

	cmd = (void *) skb->data;
	skb_pull(skb, L2CAP_CMD_HDR_SIZE);

	len = le16_to_cpu(cmd->len);

	BT_DBG("code 0x%2.2x len %d id 0x%2.2x", cmd->code, len, cmd->ident);

	if (len != skb->len || !cmd->ident) {
		BT_DBG("corrupted command");
		goto drop;
	}

	err = l2cap_le_sig_cmd(conn, cmd, len, skb->data);
	if (err) {
		struct l2cap_cmd_rej_unk rej;

		BT_ERR("Wrong link type (%d)", err);

		rej.reason = cpu_to_le16(L2CAP_REJ_NOT_UNDERSTOOD);
		l2cap_send_cmd(conn, cmd->ident, L2CAP_COMMAND_REJ,
			       sizeof(rej), &rej);
	}

drop:
	kfree_skb(skb);
}

static inline void l2cap_sig_send_rej(struct l2cap_conn *conn, u16 ident)
{
	struct l2cap_cmd_rej_unk rej;

	rej.reason = cpu_to_le16(L2CAP_REJ_NOT_UNDERSTOOD);
	l2cap_send_cmd(conn, ident, L2CAP_COMMAND_REJ, sizeof(rej), &rej);
}

static inline void l2cap_sig_channel(struct l2cap_conn *conn,
				     struct sk_buff *skb)
{
	struct hci_conn *hcon = conn->hcon;
	struct l2cap_cmd_hdr *cmd;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int err;

	l2cap_raw_recv(conn, skb);

<<<<<<< HEAD
	while (len >= L2CAP_CMD_HDR_SIZE) {
		u16 cmd_len;
		memcpy(&cmd, data, L2CAP_CMD_HDR_SIZE);
		data += L2CAP_CMD_HDR_SIZE;
		len  -= L2CAP_CMD_HDR_SIZE;

		cmd_len = le16_to_cpu(cmd.len);

		BT_DBG("code 0x%2.2x len %d id 0x%2.2x", cmd.code, cmd_len, cmd.ident);

		if (cmd_len > len || !cmd.ident) {
			BT_DBG("corrupted command");
			break;
		}

		if (conn->hcon->type == LE_LINK)
			err = l2cap_le_sig_cmd(conn, &cmd, data);
		else
			err = l2cap_bredr_sig_cmd(conn, &cmd, cmd_len,
							data, skb);

		if (err) {
			struct l2cap_cmd_rej rej;

			BT_ERR("Wrong link type (%d)", err);

			/* FIXME: Map err to a valid reason */
			rej.reason = cpu_to_le16(0);
			l2cap_send_cmd(conn, cmd.ident, L2CAP_COMMAND_REJ, sizeof(rej), &rej);
		}

		data += cmd_len;
		len  -= cmd_len;
	}

	kfree_skb(skb);
}

static int l2cap_check_fcs(struct l2cap_pinfo *pi,  struct sk_buff *skb)
=======
	if (hcon->type != ACL_LINK)
		goto drop;

	while (skb->len >= L2CAP_CMD_HDR_SIZE) {
		u16 len;

		cmd = (void *) skb->data;
		skb_pull(skb, L2CAP_CMD_HDR_SIZE);

		len = le16_to_cpu(cmd->len);

		BT_DBG("code 0x%2.2x len %d id 0x%2.2x", cmd->code, len,
		       cmd->ident);

		if (len > skb->len || !cmd->ident) {
			BT_DBG("corrupted command");
			l2cap_sig_send_rej(conn, cmd->ident);
			skb_pull(skb, len > skb->len ? skb->len : len);
			continue;
		}

		err = l2cap_bredr_sig_cmd(conn, cmd, len, skb->data);
		if (err) {
			BT_ERR("Wrong link type (%d)", err);
			l2cap_sig_send_rej(conn, cmd->ident);
		}

		skb_pull(skb, len);
	}

	if (skb->len > 0) {
		BT_DBG("corrupted command");
		l2cap_sig_send_rej(conn, 0);
	}

drop:
	kfree_skb(skb);
}

static int l2cap_check_fcs(struct l2cap_chan *chan,  struct sk_buff *skb)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	u16 our_fcs, rcv_fcs;
	int hdr_size;

<<<<<<< HEAD
	if (pi->extended_control)
		hdr_size = L2CAP_EXTENDED_HDR_SIZE;
	else
		hdr_size = L2CAP_ENHANCED_HDR_SIZE;

	if (pi->fcs == L2CAP_FCS_CRC16) {
=======
	if (test_bit(FLAG_EXT_CTRL, &chan->flags))
		hdr_size = L2CAP_EXT_HDR_SIZE;
	else
		hdr_size = L2CAP_ENH_HDR_SIZE;

	if (chan->fcs == L2CAP_FCS_CRC16) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		skb_trim(skb, skb->len - L2CAP_FCS_SIZE);
		rcv_fcs = get_unaligned_le16(skb->data + skb->len);
		our_fcs = crc16(0, skb->data - hdr_size, skb->len + hdr_size);

<<<<<<< HEAD
		if (our_fcs != rcv_fcs) {
			BT_DBG("Bad FCS");
			return -EBADMSG;
		}
=======
		if (our_fcs != rcv_fcs)
			return -EBADMSG;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return 0;
}

<<<<<<< HEAD
static void l2cap_ertm_pass_to_tx(struct sock *sk,
				struct bt_l2cap_control *control)
{
	BT_DBG("sk %p, control %p", sk, control);
	l2cap_ertm_tx(sk, control, 0, L2CAP_ERTM_EVENT_RECV_REQSEQ_AND_FBIT);
}

static void l2cap_ertm_pass_to_tx_fbit(struct sock *sk,
				struct bt_l2cap_control *control)
{
	BT_DBG("sk %p, control %p", sk, control);
	l2cap_ertm_tx(sk, control, 0, L2CAP_ERTM_EVENT_RECV_FBIT);
}

static void l2cap_ertm_resend(struct sock *sk)
{
	struct bt_l2cap_control control;
	struct l2cap_pinfo *pi;
	struct sk_buff *skb;
	struct sk_buff *tx_skb;
	u16 seq;

	BT_DBG("sk %p", sk);

	pi = l2cap_pi(sk);

	if (pi->conn_state & L2CAP_CONN_REMOTE_BUSY)
		return;

	if (pi->amp_move_state != L2CAP_AMP_STATE_STABLE &&
			pi->amp_move_state != L2CAP_AMP_STATE_WAIT_PREPARE)
		return;

	while (pi->retrans_list.head != L2CAP_SEQ_LIST_CLEAR) {
		seq = l2cap_seq_list_pop(&pi->retrans_list);

		skb = l2cap_ertm_seq_in_queue(TX_QUEUE(sk), seq);
		if (!skb) {
			BT_DBG("Error: Can't retransmit seq %d, frame missing",
				(int) seq);
			continue;
		}

		bt_cb(skb)->retries += 1;
		control = bt_cb(skb)->control;

		if ((pi->max_tx != 0) && (bt_cb(skb)->retries > pi->max_tx)) {
			BT_DBG("Retry limit exceeded (%d)", (int) pi->max_tx);
			l2cap_send_disconn_req(pi->conn, sk, ECONNRESET);
			l2cap_seq_list_clear(&pi->retrans_list);
			break;
		}

		control.reqseq = pi->buffer_seq;
		if (pi->conn_state & L2CAP_CONN_SEND_FBIT) {
			control.final = 1;
			pi->conn_state &= ~L2CAP_CONN_SEND_FBIT;
		} else {
			control.final = 0;
		}

		if (skb_cloned(skb)) {
			/* Cloned sk_buffs are read-only, so we need a
			 * writeable copy
			 */
			tx_skb = skb_copy(skb, GFP_ATOMIC);
		} else {
			tx_skb = skb_clone(skb, GFP_ATOMIC);
		}

		if (!tx_skb) {
			l2cap_seq_list_clear(&pi->retrans_list);
			break;
		}

		/* Update skb contents */
		if (pi->extended_control) {
			put_unaligned_le32(__pack_extended_control(&control),
					tx_skb->data + L2CAP_HDR_SIZE);
		} else {
			put_unaligned_le16(__pack_enhanced_control(&control),
					tx_skb->data + L2CAP_HDR_SIZE);
		}

		if (pi->fcs == L2CAP_FCS_CRC16)
			apply_fcs(tx_skb);

		sock_hold(sk);
		tx_skb->sk = sk;
		tx_skb->destructor = l2cap_skb_destructor;
		atomic_inc(&pi->ertm_queued);

		l2cap_do_send(sk, tx_skb);

		BT_DBG("Resent txseq %d", (int)control.txseq);

		pi->last_acked_seq = pi->buffer_seq;
	}
}

static inline void l2cap_ertm_retransmit(struct sock *sk,
					struct bt_l2cap_control *control)
{
	BT_DBG("sk %p, control %p", sk, control);

	l2cap_seq_list_append(&l2cap_pi(sk)->retrans_list, control->reqseq);
	l2cap_ertm_resend(sk);
}

static void l2cap_ertm_retransmit_all(struct sock *sk,
				struct bt_l2cap_control *control)
{
	struct l2cap_pinfo *pi;
	struct sk_buff *skb;

	BT_DBG("sk %p, control %p", sk, control);

	pi = l2cap_pi(sk);

	if (control->poll)
		pi->conn_state |= L2CAP_CONN_SEND_FBIT;

	l2cap_seq_list_clear(&pi->retrans_list);

	if (pi->conn_state & L2CAP_CONN_REMOTE_BUSY)
		return;

	if (pi->unacked_frames) {
		skb_queue_walk(TX_QUEUE(sk), skb) {
			if ((bt_cb(skb)->control.txseq == control->reqseq) ||
				skb == sk->sk_send_head)
				break;
		}

		skb_queue_walk_from(TX_QUEUE(sk), skb) {
			if (skb == sk->sk_send_head)
				break;

			l2cap_seq_list_append(&pi->retrans_list,
					bt_cb(skb)->control.txseq);
		}

		l2cap_ertm_resend(sk);
	}
}

static inline void append_skb_frag(struct sk_buff *skb,
			struct sk_buff *new_frag, struct sk_buff **last_frag)
{
	/* skb->len reflects data in skb as well as all fragments
	   skb->data_len reflects only data in fragments
	 */
	BT_DBG("skb %p, new_frag %p, *last_frag %p", skb, new_frag, *last_frag);

=======
static void l2cap_send_i_or_rr_or_rnr(struct l2cap_chan *chan)
{
	struct l2cap_ctrl control;

	BT_DBG("chan %p", chan);

	memset(&control, 0, sizeof(control));
	control.sframe = 1;
	control.final = 1;
	control.reqseq = chan->buffer_seq;
	set_bit(CONN_SEND_FBIT, &chan->conn_state);

	if (test_bit(CONN_LOCAL_BUSY, &chan->conn_state)) {
		control.super = L2CAP_SUPER_RNR;
		l2cap_send_sframe(chan, &control);
	}

	if (test_and_clear_bit(CONN_REMOTE_BUSY, &chan->conn_state) &&
	    chan->unacked_frames > 0)
		__set_retrans_timer(chan);

	/* Send pending iframes */
	l2cap_ertm_send(chan);

	if (!test_bit(CONN_LOCAL_BUSY, &chan->conn_state) &&
	    test_bit(CONN_SEND_FBIT, &chan->conn_state)) {
		/* F-bit wasn't sent in an s-frame or i-frame yet, so
		 * send it now.
		 */
		control.super = L2CAP_SUPER_RR;
		l2cap_send_sframe(chan, &control);
	}
}

static void append_skb_frag(struct sk_buff *skb, struct sk_buff *new_frag,
			    struct sk_buff **last_frag)
{
	/* skb->len reflects data in skb as well as all fragments
	 * skb->data_len reflects only data in fragments
	 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!skb_has_frag_list(skb))
		skb_shinfo(skb)->frag_list = new_frag;

	new_frag->next = NULL;

	(*last_frag)->next = new_frag;
	*last_frag = new_frag;

	skb->len += new_frag->len;
	skb->data_len += new_frag->len;
	skb->truesize += new_frag->truesize;
}

<<<<<<< HEAD
static int l2cap_ertm_rx_expected_iframe(struct sock *sk,
			struct bt_l2cap_control *control, struct sk_buff *skb)
{
	struct l2cap_pinfo *pi;
	int err = -EINVAL;

	BT_DBG("sk %p, control %p, skb %p len %d truesize %d", sk, control,
		skb, skb->len, skb->truesize);

	if (!control)
		return err;

	pi = l2cap_pi(sk);

	BT_DBG("type %c, sar %d, txseq %d, reqseq %d, final %d",
		control->frame_type, control->sar, control->txseq,
		control->reqseq, control->final);

	switch (control->sar) {
	case L2CAP_SAR_UNSEGMENTED:
		if (pi->sdu) {
			BT_DBG("Unexpected unsegmented PDU during reassembly");
			kfree_skb(pi->sdu);
			pi->sdu = NULL;
			pi->sdu_last_frag = NULL;
			pi->sdu_len = 0;
		}

		BT_DBG("Unsegmented");
		err = sock_queue_rcv_skb(sk, skb);
		break;

	case L2CAP_SAR_START:
		if (pi->sdu) {
			BT_DBG("Unexpected start PDU during reassembly");
			kfree_skb(pi->sdu);
		}

		pi->sdu_len = get_unaligned_le16(skb->data);
		skb_pull(skb, 2);

		if (pi->sdu_len > pi->imtu) {
=======
static int l2cap_reassemble_sdu(struct l2cap_chan *chan, struct sk_buff *skb,
				struct l2cap_ctrl *control)
{
	int err = -EINVAL;

	switch (control->sar) {
	case L2CAP_SAR_UNSEGMENTED:
		if (chan->sdu)
			break;

		err = chan->ops->recv(chan, skb);
		break;

	case L2CAP_SAR_START:
		if (chan->sdu)
			break;

		if (!pskb_may_pull(skb, L2CAP_SDULEN_SIZE))
			break;

		chan->sdu_len = get_unaligned_le16(skb->data);
		skb_pull(skb, L2CAP_SDULEN_SIZE);

		if (chan->sdu_len > chan->imtu) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			err = -EMSGSIZE;
			break;
		}

<<<<<<< HEAD
		if (skb->len >= pi->sdu_len)
			break;

		pi->sdu = skb;
		pi->sdu_last_frag = skb;

		BT_DBG("Start");
=======
		if (skb->len >= chan->sdu_len)
			break;

		chan->sdu = skb;
		chan->sdu_last_frag = skb;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		skb = NULL;
		err = 0;
		break;

	case L2CAP_SAR_CONTINUE:
<<<<<<< HEAD
		if (!pi->sdu)
			break;

		append_skb_frag(pi->sdu, skb,
				&pi->sdu_last_frag);
		skb = NULL;

		if (pi->sdu->len >= pi->sdu_len)
			break;

		BT_DBG("Continue, reassembled %d", pi->sdu->len);

=======
		if (!chan->sdu)
			break;

		append_skb_frag(chan->sdu, skb,
				&chan->sdu_last_frag);
		skb = NULL;

		if (chan->sdu->len >= chan->sdu_len)
			break;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = 0;
		break;

	case L2CAP_SAR_END:
<<<<<<< HEAD
		if (!pi->sdu)
			break;

		append_skb_frag(pi->sdu, skb,
				&pi->sdu_last_frag);
		skb = NULL;

		if (pi->sdu->len != pi->sdu_len)
			break;

		BT_DBG("End, reassembled %d", pi->sdu->len);
		/* If the sender used tiny PDUs, the rcv queuing could fail.
		 * Applications that have issues here should use a larger
		 * sk_rcvbuf.
		 */
		err = sock_queue_rcv_skb(sk, pi->sdu);

		if (!err) {
			/* Reassembly complete */
			pi->sdu = NULL;
			pi->sdu_last_frag = NULL;
			pi->sdu_len = 0;
		}
		break;

	default:
		BT_DBG("Bad SAR value");
		break;
	}

	if (err) {
		BT_DBG("Reassembly error %d, sk_rcvbuf %d, sk_rmem_alloc %d",
			err, sk->sk_rcvbuf, atomic_read(&sk->sk_rmem_alloc));
		if (pi->sdu) {
			kfree_skb(pi->sdu);
			pi->sdu = NULL;
		}
		pi->sdu_last_frag = NULL;
		pi->sdu_len = 0;
		if (skb)
			kfree_skb(skb);
	}

	/* Update local busy state */
	if (!(pi->conn_state & L2CAP_CONN_LOCAL_BUSY) && l2cap_rmem_full(sk))
		l2cap_ertm_tx(sk, 0, 0, L2CAP_ERTM_EVENT_LOCAL_BUSY_DETECTED);

	return err;
}

static int l2cap_ertm_rx_queued_iframes(struct sock *sk)
{
	int err = 0;
	/* Pass sequential frames to l2cap_ertm_rx_expected_iframe()
	 * until a gap is encountered.
	 */

	struct l2cap_pinfo *pi;

	BT_DBG("sk %p", sk);
	pi = l2cap_pi(sk);

	while (l2cap_rmem_available(sk)) {
		struct sk_buff *skb;
		BT_DBG("Searching for skb with txseq %d (queue len %d)",
			(int) pi->buffer_seq, skb_queue_len(SREJ_QUEUE(sk)));

		skb = l2cap_ertm_seq_in_queue(SREJ_QUEUE(sk), pi->buffer_seq);
=======
		if (!chan->sdu)
			break;

		append_skb_frag(chan->sdu, skb,
				&chan->sdu_last_frag);
		skb = NULL;

		if (chan->sdu->len != chan->sdu_len)
			break;

		err = chan->ops->recv(chan, chan->sdu);

		if (!err) {
			/* Reassembly complete */
			chan->sdu = NULL;
			chan->sdu_last_frag = NULL;
			chan->sdu_len = 0;
		}
		break;
	}

	if (err) {
		kfree_skb(skb);
		kfree_skb(chan->sdu);
		chan->sdu = NULL;
		chan->sdu_last_frag = NULL;
		chan->sdu_len = 0;
	}

	return err;
}

static int l2cap_resegment(struct l2cap_chan *chan)
{
	/* Placeholder */
	return 0;
}

void l2cap_chan_busy(struct l2cap_chan *chan, int busy)
{
	u8 event;

	if (chan->mode != L2CAP_MODE_ERTM)
		return;

	event = busy ? L2CAP_EV_LOCAL_BUSY_DETECTED : L2CAP_EV_LOCAL_BUSY_CLEAR;
	l2cap_tx(chan, NULL, NULL, event);
}

static int l2cap_rx_queued_iframes(struct l2cap_chan *chan)
{
	int err = 0;
	/* Pass sequential frames to l2cap_reassemble_sdu()
	 * until a gap is encountered.
	 */

	BT_DBG("chan %p", chan);

	while (!test_bit(CONN_LOCAL_BUSY, &chan->conn_state)) {
		struct sk_buff *skb;
		BT_DBG("Searching for skb with txseq %d (queue len %d)",
		       chan->buffer_seq, skb_queue_len(&chan->srej_q));

		skb = l2cap_ertm_seq_in_queue(&chan->srej_q, chan->buffer_seq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (!skb)
			break;

<<<<<<< HEAD
		skb_unlink(skb, SREJ_QUEUE(sk));
		pi->buffer_seq = __next_seq(pi->buffer_seq, pi);
		err = l2cap_ertm_rx_expected_iframe(sk,
						&bt_cb(skb)->control, skb);
=======
		skb_unlink(skb, &chan->srej_q);
		chan->buffer_seq = __next_seq(chan, chan->buffer_seq);
		err = l2cap_reassemble_sdu(chan, skb, &bt_cb(skb)->l2cap);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (err)
			break;
	}

<<<<<<< HEAD
	if (skb_queue_empty(SREJ_QUEUE(sk))) {
		pi->rx_state = L2CAP_ERTM_RX_STATE_RECV;
		l2cap_ertm_send_ack(sk);
=======
	if (skb_queue_empty(&chan->srej_q)) {
		chan->rx_state = L2CAP_RX_STATE_RECV;
		l2cap_send_ack(chan);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return err;
}

<<<<<<< HEAD
static void l2cap_ertm_handle_srej(struct sock *sk,
				struct bt_l2cap_control *control)
{
	struct l2cap_pinfo *pi;
	struct sk_buff *skb;

	BT_DBG("sk %p, control %p", sk, control);

	pi = l2cap_pi(sk);

	if (control->reqseq == pi->next_tx_seq) {
		BT_DBG("Invalid reqseq %d, disconnecting",
			(int) control->reqseq);
		l2cap_send_disconn_req(pi->conn, sk, ECONNRESET);
		return;
	}

	skb = l2cap_ertm_seq_in_queue(TX_QUEUE(sk), control->reqseq);

	if (skb == NULL) {
		BT_DBG("Seq %d not available for retransmission",
			(int) control->reqseq);
		return;
	}

	if ((pi->max_tx != 0) && (bt_cb(skb)->retries >= pi->max_tx)) {
		BT_DBG("Retry limit exceeded (%d)", (int) pi->max_tx);
		l2cap_send_disconn_req(pi->conn, sk, ECONNRESET);
		return;
	}

	pi->conn_state &= ~L2CAP_CONN_REMOTE_BUSY;

	if (control->poll) {
		l2cap_ertm_pass_to_tx(sk, control);

		pi->conn_state |= L2CAP_CONN_SEND_FBIT;
		l2cap_ertm_retransmit(sk, control);
		l2cap_ertm_send(sk);

		if (pi->tx_state == L2CAP_ERTM_TX_STATE_WAIT_F) {
			pi->conn_state |= L2CAP_CONN_SREJ_ACT;
			pi->srej_save_reqseq = control->reqseq;
		}
	} else {
		l2cap_ertm_pass_to_tx_fbit(sk, control);

		if (control->final) {
			if ((pi->conn_state & L2CAP_CONN_SREJ_ACT) &&
				(pi->srej_save_reqseq == control->reqseq)) {
				pi->conn_state &= ~L2CAP_CONN_SREJ_ACT;
			} else {
				l2cap_ertm_retransmit(sk, control);
			}
		} else {
			l2cap_ertm_retransmit(sk, control);
			if (pi->tx_state == L2CAP_ERTM_TX_STATE_WAIT_F) {
				pi->conn_state |= L2CAP_CONN_SREJ_ACT;
				pi->srej_save_reqseq = control->reqseq;
=======
static void l2cap_handle_srej(struct l2cap_chan *chan,
			      struct l2cap_ctrl *control)
{
	struct sk_buff *skb;

	BT_DBG("chan %p, control %p", chan, control);

	if (control->reqseq == chan->next_tx_seq) {
		BT_DBG("Invalid reqseq %d, disconnecting", control->reqseq);
		l2cap_send_disconn_req(chan, ECONNRESET);
		return;
	}

	skb = l2cap_ertm_seq_in_queue(&chan->tx_q, control->reqseq);

	if (skb == NULL) {
		BT_DBG("Seq %d not available for retransmission",
		       control->reqseq);
		return;
	}

	if (chan->max_tx != 0 && bt_cb(skb)->l2cap.retries >= chan->max_tx) {
		BT_DBG("Retry limit exceeded (%d)", chan->max_tx);
		l2cap_send_disconn_req(chan, ECONNRESET);
		return;
	}

	clear_bit(CONN_REMOTE_BUSY, &chan->conn_state);

	if (control->poll) {
		l2cap_pass_to_tx(chan, control);

		set_bit(CONN_SEND_FBIT, &chan->conn_state);
		l2cap_retransmit(chan, control);
		l2cap_ertm_send(chan);

		if (chan->tx_state == L2CAP_TX_STATE_WAIT_F) {
			set_bit(CONN_SREJ_ACT, &chan->conn_state);
			chan->srej_save_reqseq = control->reqseq;
		}
	} else {
		l2cap_pass_to_tx_fbit(chan, control);

		if (control->final) {
			if (chan->srej_save_reqseq != control->reqseq ||
			    !test_and_clear_bit(CONN_SREJ_ACT,
						&chan->conn_state))
				l2cap_retransmit(chan, control);
		} else {
			l2cap_retransmit(chan, control);
			if (chan->tx_state == L2CAP_TX_STATE_WAIT_F) {
				set_bit(CONN_SREJ_ACT, &chan->conn_state);
				chan->srej_save_reqseq = control->reqseq;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			}
		}
	}
}

<<<<<<< HEAD
static void l2cap_ertm_handle_rej(struct sock *sk,
				struct bt_l2cap_control *control)
{
	struct l2cap_pinfo *pi;
	struct sk_buff *skb;

	BT_DBG("sk %p, control %p", sk, control);

	pi = l2cap_pi(sk);

	if (control->reqseq == pi->next_tx_seq) {
		BT_DBG("Invalid reqseq %d, disconnecting",
			(int) control->reqseq);
		l2cap_send_disconn_req(pi->conn, sk, ECONNRESET);
		return;
	}

	skb = l2cap_ertm_seq_in_queue(TX_QUEUE(sk), control->reqseq);

	if (pi->max_tx && skb && bt_cb(skb)->retries >= pi->max_tx) {
		BT_DBG("Retry limit exceeded (%d)", (int) pi->max_tx);
		l2cap_send_disconn_req(pi->conn, sk, ECONNRESET);
		return;
	}

	pi->conn_state &= ~L2CAP_CONN_REMOTE_BUSY;

	l2cap_ertm_pass_to_tx(sk, control);

	if (control->final) {
		if (pi->conn_state & L2CAP_CONN_REJ_ACT)
			pi->conn_state &= ~L2CAP_CONN_REJ_ACT;
		else
			l2cap_ertm_retransmit_all(sk, control);
	} else {
		l2cap_ertm_retransmit_all(sk, control);
		l2cap_ertm_send(sk);
		if (pi->tx_state == L2CAP_ERTM_TX_STATE_WAIT_F)
			pi->conn_state |= L2CAP_CONN_REJ_ACT;
	}
}

static u8 l2cap_ertm_classify_txseq(struct sock *sk, u16 txseq)
{
	struct l2cap_pinfo *pi;

	BT_DBG("sk %p, txseq %d", sk, (int)txseq);
	pi = l2cap_pi(sk);

	BT_DBG("last_acked_seq %d, expected_tx_seq %d", (int)pi->last_acked_seq,
		(int)pi->expected_tx_seq);

	if (pi->rx_state == L2CAP_ERTM_RX_STATE_SREJ_SENT) {
		if (__delta_seq(txseq, pi->last_acked_seq, pi) >= pi->tx_win) {
			/* See notes below regarding "double poll" and
			 * invalid packets.
			 */
			if (pi->tx_win <= ((pi->tx_win_max + 1) >> 1)) {
				BT_DBG("Invalid/Ignore - txseq outside "
					"tx window after SREJ sent");
				return L2CAP_ERTM_TXSEQ_INVALID_IGNORE;
			} else {
				BT_DBG("Invalid - bad txseq within tx "
					"window after SREJ sent");
				return L2CAP_ERTM_TXSEQ_INVALID;
			}
		}

		if (pi->srej_list.head == txseq) {
			BT_DBG("Expected SREJ");
			return L2CAP_ERTM_TXSEQ_EXPECTED_SREJ;
		}

		if (l2cap_ertm_seq_in_queue(SREJ_QUEUE(sk), txseq)) {
			BT_DBG("Duplicate SREJ - txseq already stored");
			return L2CAP_ERTM_TXSEQ_DUPLICATE_SREJ;
		}

		if (l2cap_seq_list_contains(&pi->srej_list, txseq)) {
			BT_DBG("Unexpected SREJ - txseq not requested "
				"with SREJ");
			return L2CAP_ERTM_TXSEQ_UNEXPECTED_SREJ;
		}
	}

	if (pi->expected_tx_seq == txseq) {
		if (__delta_seq(txseq, pi->last_acked_seq, pi) >= pi->tx_win) {
			BT_DBG("Invalid - txseq outside tx window");
			return L2CAP_ERTM_TXSEQ_INVALID;
		} else {
			BT_DBG("Expected");
			return L2CAP_ERTM_TXSEQ_EXPECTED;
		}
	}

	if (__delta_seq(txseq, pi->last_acked_seq, pi) <
		__delta_seq(pi->expected_tx_seq, pi->last_acked_seq, pi)) {
		BT_DBG("Duplicate - expected_tx_seq later than txseq");
		return L2CAP_ERTM_TXSEQ_DUPLICATE;
	}

	if (__delta_seq(txseq, pi->last_acked_seq, pi) >= pi->tx_win) {
=======
static void l2cap_handle_rej(struct l2cap_chan *chan,
			     struct l2cap_ctrl *control)
{
	struct sk_buff *skb;

	BT_DBG("chan %p, control %p", chan, control);

	if (control->reqseq == chan->next_tx_seq) {
		BT_DBG("Invalid reqseq %d, disconnecting", control->reqseq);
		l2cap_send_disconn_req(chan, ECONNRESET);
		return;
	}

	skb = l2cap_ertm_seq_in_queue(&chan->tx_q, control->reqseq);

	if (chan->max_tx && skb &&
	    bt_cb(skb)->l2cap.retries >= chan->max_tx) {
		BT_DBG("Retry limit exceeded (%d)", chan->max_tx);
		l2cap_send_disconn_req(chan, ECONNRESET);
		return;
	}

	clear_bit(CONN_REMOTE_BUSY, &chan->conn_state);

	l2cap_pass_to_tx(chan, control);

	if (control->final) {
		if (!test_and_clear_bit(CONN_REJ_ACT, &chan->conn_state))
			l2cap_retransmit_all(chan, control);
	} else {
		l2cap_retransmit_all(chan, control);
		l2cap_ertm_send(chan);
		if (chan->tx_state == L2CAP_TX_STATE_WAIT_F)
			set_bit(CONN_REJ_ACT, &chan->conn_state);
	}
}

static u8 l2cap_classify_txseq(struct l2cap_chan *chan, u16 txseq)
{
	BT_DBG("chan %p, txseq %d", chan, txseq);

	BT_DBG("last_acked_seq %d, expected_tx_seq %d", chan->last_acked_seq,
	       chan->expected_tx_seq);

	if (chan->rx_state == L2CAP_RX_STATE_SREJ_SENT) {
		if (__seq_offset(chan, txseq, chan->last_acked_seq) >=
		    chan->tx_win) {
			/* See notes below regarding "double poll" and
			 * invalid packets.
			 */
			if (chan->tx_win <= ((chan->tx_win_max + 1) >> 1)) {
				BT_DBG("Invalid/Ignore - after SREJ");
				return L2CAP_TXSEQ_INVALID_IGNORE;
			} else {
				BT_DBG("Invalid - in window after SREJ sent");
				return L2CAP_TXSEQ_INVALID;
			}
		}

		if (chan->srej_list.head == txseq) {
			BT_DBG("Expected SREJ");
			return L2CAP_TXSEQ_EXPECTED_SREJ;
		}

		if (l2cap_ertm_seq_in_queue(&chan->srej_q, txseq)) {
			BT_DBG("Duplicate SREJ - txseq already stored");
			return L2CAP_TXSEQ_DUPLICATE_SREJ;
		}

		if (l2cap_seq_list_contains(&chan->srej_list, txseq)) {
			BT_DBG("Unexpected SREJ - not requested");
			return L2CAP_TXSEQ_UNEXPECTED_SREJ;
		}
	}

	if (chan->expected_tx_seq == txseq) {
		if (__seq_offset(chan, txseq, chan->last_acked_seq) >=
		    chan->tx_win) {
			BT_DBG("Invalid - txseq outside tx window");
			return L2CAP_TXSEQ_INVALID;
		} else {
			BT_DBG("Expected");
			return L2CAP_TXSEQ_EXPECTED;
		}
	}

	if (__seq_offset(chan, txseq, chan->last_acked_seq) <
	    __seq_offset(chan, chan->expected_tx_seq, chan->last_acked_seq)) {
		BT_DBG("Duplicate - expected_tx_seq later than txseq");
		return L2CAP_TXSEQ_DUPLICATE;
	}

	if (__seq_offset(chan, txseq, chan->last_acked_seq) >= chan->tx_win) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* A source of invalid packets is a "double poll" condition,
		 * where delays cause us to send multiple poll packets.  If
		 * the remote stack receives and processes both polls,
		 * sequence numbers can wrap around in such a way that a
		 * resent frame has a sequence number that looks like new data
		 * with a sequence gap.  This would trigger an erroneous SREJ
		 * request.
		 *
		 * Fortunately, this is impossible with a tx window that's
		 * less than half of the maximum sequence number, which allows
		 * invalid frames to be safely ignored.
		 *
		 * With tx window sizes greater than half of the tx window
		 * maximum, the frame is invalid and cannot be ignored.  This
		 * causes a disconnect.
		 */

<<<<<<< HEAD
		if (pi->tx_win <= ((pi->tx_win_max + 1) >> 1)) {
			BT_DBG("Invalid/Ignore - txseq outside tx window");
			return L2CAP_ERTM_TXSEQ_INVALID_IGNORE;
		} else {
			BT_DBG("Invalid - txseq outside tx window");
			return L2CAP_ERTM_TXSEQ_INVALID;
		}
	} else {
		BT_DBG("Unexpected - txseq indicates missing frames");
		return L2CAP_ERTM_TXSEQ_UNEXPECTED;
	}
}

static int l2cap_ertm_rx_state_recv(struct sock *sk,
				struct bt_l2cap_control *control,
				struct sk_buff *skb, u8 event)
{
	struct l2cap_pinfo *pi;
	int err = 0;
	bool skb_in_use = 0;

	BT_DBG("sk %p, control %p, skb %p, event %d", sk, control, skb,
		(int)event);
	pi = l2cap_pi(sk);

	switch (event) {
	case L2CAP_ERTM_EVENT_RECV_IFRAME:
		switch (l2cap_ertm_classify_txseq(sk, control->txseq)) {
		case L2CAP_ERTM_TXSEQ_EXPECTED:
			l2cap_ertm_pass_to_tx(sk, control);

			if (pi->conn_state & L2CAP_CONN_LOCAL_BUSY) {
				BT_DBG("Busy, discarding expected seq %d",
					control->txseq);
				break;
			}

			pi->expected_tx_seq = __next_seq(control->txseq, pi);
			pi->buffer_seq = pi->expected_tx_seq;
			skb_in_use = 1;

			err = l2cap_ertm_rx_expected_iframe(sk, control, skb);
			if (err)
				break;

			if (control->final) {
				if (pi->conn_state & L2CAP_CONN_REJ_ACT)
					pi->conn_state &= ~L2CAP_CONN_REJ_ACT;
				else {
					control->final = 0;
					l2cap_ertm_retransmit_all(sk, control);
					l2cap_ertm_send(sk);
				}
			}

			if (!(pi->conn_state & L2CAP_CONN_LOCAL_BUSY))
				l2cap_ertm_send_ack(sk);
			break;
		case L2CAP_ERTM_TXSEQ_UNEXPECTED:
			l2cap_ertm_pass_to_tx(sk, control);
=======
		if (chan->tx_win <= ((chan->tx_win_max + 1) >> 1)) {
			BT_DBG("Invalid/Ignore - txseq outside tx window");
			return L2CAP_TXSEQ_INVALID_IGNORE;
		} else {
			BT_DBG("Invalid - txseq outside tx window");
			return L2CAP_TXSEQ_INVALID;
		}
	} else {
		BT_DBG("Unexpected - txseq indicates missing frames");
		return L2CAP_TXSEQ_UNEXPECTED;
	}
}

static int l2cap_rx_state_recv(struct l2cap_chan *chan,
			       struct l2cap_ctrl *control,
			       struct sk_buff *skb, u8 event)
{
	struct l2cap_ctrl local_control;
	int err = 0;
	bool skb_in_use = false;

	BT_DBG("chan %p, control %p, skb %p, event %d", chan, control, skb,
	       event);

	switch (event) {
	case L2CAP_EV_RECV_IFRAME:
		switch (l2cap_classify_txseq(chan, control->txseq)) {
		case L2CAP_TXSEQ_EXPECTED:
			l2cap_pass_to_tx(chan, control);

			if (test_bit(CONN_LOCAL_BUSY, &chan->conn_state)) {
				BT_DBG("Busy, discarding expected seq %d",
				       control->txseq);
				break;
			}

			chan->expected_tx_seq = __next_seq(chan,
							   control->txseq);

			chan->buffer_seq = chan->expected_tx_seq;
			skb_in_use = true;

			/* l2cap_reassemble_sdu may free skb, hence invalidate
			 * control, so make a copy in advance to use it after
			 * l2cap_reassemble_sdu returns and to avoid the race
			 * condition, for example:
			 *
			 * The current thread calls:
			 *   l2cap_reassemble_sdu
			 *     chan->ops->recv == l2cap_sock_recv_cb
			 *       __sock_queue_rcv_skb
			 * Another thread calls:
			 *   bt_sock_recvmsg
			 *     skb_recv_datagram
			 *     skb_free_datagram
			 * Then the current thread tries to access control, but
			 * it was freed by skb_free_datagram.
			 */
			local_control = *control;
			err = l2cap_reassemble_sdu(chan, skb, control);
			if (err)
				break;

			if (local_control.final) {
				if (!test_and_clear_bit(CONN_REJ_ACT,
							&chan->conn_state)) {
					local_control.final = 0;
					l2cap_retransmit_all(chan, &local_control);
					l2cap_ertm_send(chan);
				}
			}

			if (!test_bit(CONN_LOCAL_BUSY, &chan->conn_state))
				l2cap_send_ack(chan);
			break;
		case L2CAP_TXSEQ_UNEXPECTED:
			l2cap_pass_to_tx(chan, control);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

			/* Can't issue SREJ frames in the local busy state.
			 * Drop this frame, it will be seen as missing
			 * when local busy is exited.
			 */
<<<<<<< HEAD
			if (pi->conn_state & L2CAP_CONN_LOCAL_BUSY) {
				BT_DBG("Busy, discarding unexpected seq %d",
					control->txseq);
=======
			if (test_bit(CONN_LOCAL_BUSY, &chan->conn_state)) {
				BT_DBG("Busy, discarding unexpected seq %d",
				       control->txseq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				break;
			}

			/* There was a gap in the sequence, so an SREJ
			 * must be sent for each missing frame.  The
			 * current frame is stored for later use.
			 */
<<<<<<< HEAD
			skb_queue_tail(SREJ_QUEUE(sk), skb);
			skb_in_use = 1;
			BT_DBG("Queued %p (queue len %d)", skb,
			       skb_queue_len(SREJ_QUEUE(sk)));

			pi->conn_state &= ~L2CAP_CONN_SREJ_ACT;
			l2cap_seq_list_clear(&pi->srej_list);
			l2cap_ertm_send_srej(sk, control->txseq);

			pi->rx_state = L2CAP_ERTM_RX_STATE_SREJ_SENT;
			break;
		case L2CAP_ERTM_TXSEQ_DUPLICATE:
			l2cap_ertm_pass_to_tx(sk, control);
			break;
		case L2CAP_ERTM_TXSEQ_INVALID_IGNORE:
			break;
		case L2CAP_ERTM_TXSEQ_INVALID:
		default:
			l2cap_send_disconn_req(l2cap_pi(sk)->conn, sk,
					ECONNRESET);
			break;
		}
		break;
	case L2CAP_ERTM_EVENT_RECV_RR:
		l2cap_ertm_pass_to_tx(sk, control);
		if (control->final) {
			pi->conn_state &= ~L2CAP_CONN_REMOTE_BUSY;

			if (pi->conn_state & L2CAP_CONN_REJ_ACT)
				pi->conn_state &= ~L2CAP_CONN_REJ_ACT;
			else if (pi->amp_move_state == L2CAP_AMP_STATE_STABLE ||
				pi->amp_move_state ==
						L2CAP_AMP_STATE_WAIT_PREPARE) {
				control->final = 0;
				l2cap_ertm_retransmit_all(sk, control);
			}

			l2cap_ertm_send(sk);
		} else if (control->poll) {
			l2cap_ertm_send_i_or_rr_or_rnr(sk);
		} else {
			if ((pi->conn_state & L2CAP_CONN_REMOTE_BUSY) &&
				pi->unacked_frames)
				l2cap_ertm_start_retrans_timer(pi);
			pi->conn_state &= ~L2CAP_CONN_REMOTE_BUSY;
			l2cap_ertm_send(sk);
		}
		break;
	case L2CAP_ERTM_EVENT_RECV_RNR:
		pi->conn_state |= L2CAP_CONN_REMOTE_BUSY;
		l2cap_ertm_pass_to_tx(sk, control);
		if (control && control->poll) {
			pi->conn_state |= L2CAP_CONN_SEND_FBIT;
			l2cap_ertm_send_rr_or_rnr(sk, 0);
		}
		l2cap_ertm_stop_retrans_timer(pi);
		l2cap_seq_list_clear(&pi->retrans_list);
		break;
	case L2CAP_ERTM_EVENT_RECV_REJ:
		l2cap_ertm_handle_rej(sk, control);
		break;
	case L2CAP_ERTM_EVENT_RECV_SREJ:
		l2cap_ertm_handle_srej(sk, control);
=======
			skb_queue_tail(&chan->srej_q, skb);
			skb_in_use = true;
			BT_DBG("Queued %p (queue len %d)", skb,
			       skb_queue_len(&chan->srej_q));

			clear_bit(CONN_SREJ_ACT, &chan->conn_state);
			l2cap_seq_list_clear(&chan->srej_list);
			l2cap_send_srej(chan, control->txseq);

			chan->rx_state = L2CAP_RX_STATE_SREJ_SENT;
			break;
		case L2CAP_TXSEQ_DUPLICATE:
			l2cap_pass_to_tx(chan, control);
			break;
		case L2CAP_TXSEQ_INVALID_IGNORE:
			break;
		case L2CAP_TXSEQ_INVALID:
		default:
			l2cap_send_disconn_req(chan, ECONNRESET);
			break;
		}
		break;
	case L2CAP_EV_RECV_RR:
		l2cap_pass_to_tx(chan, control);
		if (control->final) {
			clear_bit(CONN_REMOTE_BUSY, &chan->conn_state);

			if (!test_and_clear_bit(CONN_REJ_ACT,
						&chan->conn_state)) {
				control->final = 0;
				l2cap_retransmit_all(chan, control);
			}

			l2cap_ertm_send(chan);
		} else if (control->poll) {
			l2cap_send_i_or_rr_or_rnr(chan);
		} else {
			if (test_and_clear_bit(CONN_REMOTE_BUSY,
					       &chan->conn_state) &&
			    chan->unacked_frames)
				__set_retrans_timer(chan);

			l2cap_ertm_send(chan);
		}
		break;
	case L2CAP_EV_RECV_RNR:
		set_bit(CONN_REMOTE_BUSY, &chan->conn_state);
		l2cap_pass_to_tx(chan, control);
		if (control && control->poll) {
			set_bit(CONN_SEND_FBIT, &chan->conn_state);
			l2cap_send_rr_or_rnr(chan, 0);
		}
		__clear_retrans_timer(chan);
		l2cap_seq_list_clear(&chan->retrans_list);
		break;
	case L2CAP_EV_RECV_REJ:
		l2cap_handle_rej(chan, control);
		break;
	case L2CAP_EV_RECV_SREJ:
		l2cap_handle_srej(chan, control);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	default:
		break;
	}

	if (skb && !skb_in_use) {
		BT_DBG("Freeing %p", skb);
		kfree_skb(skb);
	}

	return err;
}

<<<<<<< HEAD
static int l2cap_ertm_rx_state_srej_sent(struct sock *sk,
					struct bt_l2cap_control *control,
					struct sk_buff *skb, u8 event)
{
	struct l2cap_pinfo *pi;
	int err = 0;
	u16 txseq = control->txseq;
	bool skb_in_use = 0;

	BT_DBG("sk %p, control %p, skb %p, event %d", sk, control, skb,
		(int)event);
	pi = l2cap_pi(sk);

	switch (event) {
	case L2CAP_ERTM_EVENT_RECV_IFRAME:
		switch (l2cap_ertm_classify_txseq(sk, txseq)) {
		case L2CAP_ERTM_TXSEQ_EXPECTED:
			/* Keep frame for reassembly later */
			l2cap_ertm_pass_to_tx(sk, control);
			skb_queue_tail(SREJ_QUEUE(sk), skb);
			skb_in_use = 1;
			BT_DBG("Queued %p (queue len %d)", skb,
			       skb_queue_len(SREJ_QUEUE(sk)));

			pi->expected_tx_seq = __next_seq(txseq, pi);
			break;
		case L2CAP_ERTM_TXSEQ_EXPECTED_SREJ:
			l2cap_seq_list_pop(&pi->srej_list);

			l2cap_ertm_pass_to_tx(sk, control);
			skb_queue_tail(SREJ_QUEUE(sk), skb);
			skb_in_use = 1;
			BT_DBG("Queued %p (queue len %d)", skb,
			       skb_queue_len(SREJ_QUEUE(sk)));

			err = l2cap_ertm_rx_queued_iframes(sk);
=======
static int l2cap_rx_state_srej_sent(struct l2cap_chan *chan,
				    struct l2cap_ctrl *control,
				    struct sk_buff *skb, u8 event)
{
	int err = 0;
	u16 txseq = control->txseq;
	bool skb_in_use = false;

	BT_DBG("chan %p, control %p, skb %p, event %d", chan, control, skb,
	       event);

	switch (event) {
	case L2CAP_EV_RECV_IFRAME:
		switch (l2cap_classify_txseq(chan, txseq)) {
		case L2CAP_TXSEQ_EXPECTED:
			/* Keep frame for reassembly later */
			l2cap_pass_to_tx(chan, control);
			skb_queue_tail(&chan->srej_q, skb);
			skb_in_use = true;
			BT_DBG("Queued %p (queue len %d)", skb,
			       skb_queue_len(&chan->srej_q));

			chan->expected_tx_seq = __next_seq(chan, txseq);
			break;
		case L2CAP_TXSEQ_EXPECTED_SREJ:
			l2cap_seq_list_pop(&chan->srej_list);

			l2cap_pass_to_tx(chan, control);
			skb_queue_tail(&chan->srej_q, skb);
			skb_in_use = true;
			BT_DBG("Queued %p (queue len %d)", skb,
			       skb_queue_len(&chan->srej_q));

			err = l2cap_rx_queued_iframes(chan);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (err)
				break;

			break;
<<<<<<< HEAD
		case L2CAP_ERTM_TXSEQ_UNEXPECTED:
=======
		case L2CAP_TXSEQ_UNEXPECTED:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/* Got a frame that can't be reassembled yet.
			 * Save it for later, and send SREJs to cover
			 * the missing frames.
			 */
<<<<<<< HEAD
			skb_queue_tail(SREJ_QUEUE(sk), skb);
			skb_in_use = 1;
			BT_DBG("Queued %p (queue len %d)", skb,
			       skb_queue_len(SREJ_QUEUE(sk)));

			l2cap_ertm_pass_to_tx(sk, control);
			l2cap_ertm_send_srej(sk, control->txseq);
			break;
		case L2CAP_ERTM_TXSEQ_UNEXPECTED_SREJ:
=======
			skb_queue_tail(&chan->srej_q, skb);
			skb_in_use = true;
			BT_DBG("Queued %p (queue len %d)", skb,
			       skb_queue_len(&chan->srej_q));

			l2cap_pass_to_tx(chan, control);
			l2cap_send_srej(chan, control->txseq);
			break;
		case L2CAP_TXSEQ_UNEXPECTED_SREJ:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/* This frame was requested with an SREJ, but
			 * some expected retransmitted frames are
			 * missing.  Request retransmission of missing
			 * SREJ'd frames.
			 */
<<<<<<< HEAD
			skb_queue_tail(SREJ_QUEUE(sk), skb);
			skb_in_use = 1;
			BT_DBG("Queued %p (queue len %d)", skb,
			       skb_queue_len(SREJ_QUEUE(sk)));

			l2cap_ertm_pass_to_tx(sk, control);
			l2cap_ertm_send_srej_list(sk, control->txseq);
			break;
		case L2CAP_ERTM_TXSEQ_DUPLICATE_SREJ:
			/* We've already queued this frame.  Drop this copy. */
			l2cap_ertm_pass_to_tx(sk, control);
			break;
		case L2CAP_ERTM_TXSEQ_DUPLICATE:
=======
			skb_queue_tail(&chan->srej_q, skb);
			skb_in_use = true;
			BT_DBG("Queued %p (queue len %d)", skb,
			       skb_queue_len(&chan->srej_q));

			l2cap_pass_to_tx(chan, control);
			l2cap_send_srej_list(chan, control->txseq);
			break;
		case L2CAP_TXSEQ_DUPLICATE_SREJ:
			/* We've already queued this frame.  Drop this copy. */
			l2cap_pass_to_tx(chan, control);
			break;
		case L2CAP_TXSEQ_DUPLICATE:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/* Expecting a later sequence number, so this frame
			 * was already received.  Ignore it completely.
			 */
			break;
<<<<<<< HEAD
		case L2CAP_ERTM_TXSEQ_INVALID_IGNORE:
			break;
		case L2CAP_ERTM_TXSEQ_INVALID:
		default:
			l2cap_send_disconn_req(l2cap_pi(sk)->conn, sk,
					ECONNRESET);
			break;
		}
		break;
	case L2CAP_ERTM_EVENT_RECV_RR:
		l2cap_ertm_pass_to_tx(sk, control);
		if (control->final) {
			pi->conn_state &= ~L2CAP_CONN_REMOTE_BUSY;

			if (pi->conn_state & L2CAP_CONN_REJ_ACT)
				pi->conn_state &= ~L2CAP_CONN_REJ_ACT;
			else {
				control->final = 0;
				l2cap_ertm_retransmit_all(sk, control);
			}

			l2cap_ertm_send(sk);
		} else if (control->poll) {
			if ((pi->conn_state & L2CAP_CONN_REMOTE_BUSY) &&
				pi->unacked_frames) {
				l2cap_ertm_start_retrans_timer(pi);
			}
			pi->conn_state &= ~L2CAP_CONN_REMOTE_BUSY;
			pi->conn_state |= L2CAP_CONN_SEND_FBIT;
			l2cap_ertm_send_srej_tail(sk);
		} else {
			if ((pi->conn_state & L2CAP_CONN_REMOTE_BUSY) &&
				pi->unacked_frames) {
				l2cap_ertm_start_retrans_timer(pi);
			}
			pi->conn_state &= ~L2CAP_CONN_REMOTE_BUSY;
			l2cap_ertm_send_ack(sk);
		}
		break;
	case L2CAP_ERTM_EVENT_RECV_RNR:
		pi->conn_state |= L2CAP_CONN_REMOTE_BUSY;
		l2cap_ertm_pass_to_tx(sk, control);
		if (control->poll)
			l2cap_ertm_send_srej_tail(sk);
		else {
			struct bt_l2cap_control rr_control;
			memset(&rr_control, 0, sizeof(rr_control));
			rr_control.frame_type = 's';
			rr_control.super = L2CAP_SFRAME_RR;
			rr_control.reqseq = pi->buffer_seq;
			l2cap_ertm_send_sframe(sk, &rr_control);
		}

		break;
	case L2CAP_ERTM_EVENT_RECV_REJ:
		l2cap_ertm_handle_rej(sk, control);
		break;
	case L2CAP_ERTM_EVENT_RECV_SREJ:
		l2cap_ertm_handle_srej(sk, control);
=======
		case L2CAP_TXSEQ_INVALID_IGNORE:
			break;
		case L2CAP_TXSEQ_INVALID:
		default:
			l2cap_send_disconn_req(chan, ECONNRESET);
			break;
		}
		break;
	case L2CAP_EV_RECV_RR:
		l2cap_pass_to_tx(chan, control);
		if (control->final) {
			clear_bit(CONN_REMOTE_BUSY, &chan->conn_state);

			if (!test_and_clear_bit(CONN_REJ_ACT,
						&chan->conn_state)) {
				control->final = 0;
				l2cap_retransmit_all(chan, control);
			}

			l2cap_ertm_send(chan);
		} else if (control->poll) {
			if (test_and_clear_bit(CONN_REMOTE_BUSY,
					       &chan->conn_state) &&
			    chan->unacked_frames) {
				__set_retrans_timer(chan);
			}

			set_bit(CONN_SEND_FBIT, &chan->conn_state);
			l2cap_send_srej_tail(chan);
		} else {
			if (test_and_clear_bit(CONN_REMOTE_BUSY,
					       &chan->conn_state) &&
			    chan->unacked_frames)
				__set_retrans_timer(chan);

			l2cap_send_ack(chan);
		}
		break;
	case L2CAP_EV_RECV_RNR:
		set_bit(CONN_REMOTE_BUSY, &chan->conn_state);
		l2cap_pass_to_tx(chan, control);
		if (control->poll) {
			l2cap_send_srej_tail(chan);
		} else {
			struct l2cap_ctrl rr_control;
			memset(&rr_control, 0, sizeof(rr_control));
			rr_control.sframe = 1;
			rr_control.super = L2CAP_SUPER_RR;
			rr_control.reqseq = chan->buffer_seq;
			l2cap_send_sframe(chan, &rr_control);
		}

		break;
	case L2CAP_EV_RECV_REJ:
		l2cap_handle_rej(chan, control);
		break;
	case L2CAP_EV_RECV_SREJ:
		l2cap_handle_srej(chan, control);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	}

	if (skb && !skb_in_use) {
		BT_DBG("Freeing %p", skb);
		kfree_skb(skb);
	}

	return err;
}

<<<<<<< HEAD
static int l2cap_ertm_rx_state_amp_move(struct sock *sk,
					struct bt_l2cap_control *control,
					struct sk_buff *skb, u8 event)
{
	struct l2cap_pinfo *pi;
	int err = 0;
	bool skb_in_use = 0;

	BT_DBG("sk %p, control %p, skb %p, event %d", sk, control, skb,
		(int)event);
	pi = l2cap_pi(sk);

	/* Only handle expected frames, to avoid state changes. */

	switch (event) {
	case L2CAP_ERTM_EVENT_RECV_IFRAME:
		if (l2cap_ertm_classify_txseq(sk, control->txseq) ==
				L2CAP_ERTM_TXSEQ_EXPECTED) {
			l2cap_ertm_pass_to_tx(sk, control);

			if (pi->conn_state & L2CAP_CONN_LOCAL_BUSY) {
				BT_DBG("Busy, discarding expected seq %d",
					control->txseq);
				break;
			}

			pi->expected_tx_seq = __next_seq(control->txseq, pi);
			pi->buffer_seq = pi->expected_tx_seq;
			skb_in_use = 1;

			err = l2cap_ertm_rx_expected_iframe(sk, control, skb);
			if (err)
				break;

			if (control->final) {
				if (pi->conn_state & L2CAP_CONN_REJ_ACT)
					pi->conn_state &= ~L2CAP_CONN_REJ_ACT;
				else
					control->final = 0;
			}
		}
		break;
	case L2CAP_ERTM_EVENT_RECV_RR:
	case L2CAP_ERTM_EVENT_RECV_RNR:
	case L2CAP_ERTM_EVENT_RECV_REJ:
		l2cap_ertm_process_reqseq(sk, control->reqseq);
		break;
	case L2CAP_ERTM_EVENT_RECV_SREJ:
		/* Ignore */
		break;
	default:
		break;
	}

	if (skb && !skb_in_use) {
		BT_DBG("Freeing %p", skb);
		kfree_skb(skb);
	}

	return err;
}

static int l2cap_answer_move_poll(struct sock *sk)
{
	struct l2cap_pinfo *pi;
	struct bt_l2cap_control control;
	int err = 0;

	BT_DBG("sk %p", sk);

	pi = l2cap_pi(sk);

	l2cap_ertm_process_reqseq(sk, pi->amp_move_reqseq);

	if (!skb_queue_empty(TX_QUEUE(sk)))
		sk->sk_send_head = skb_peek(TX_QUEUE(sk));
	else
		sk->sk_send_head = NULL;
=======
static int l2cap_finish_move(struct l2cap_chan *chan)
{
	BT_DBG("chan %p", chan);

	chan->rx_state = L2CAP_RX_STATE_RECV;
	chan->conn->mtu = chan->conn->hcon->hdev->acl_mtu;

	return l2cap_resegment(chan);
}

static int l2cap_rx_state_wait_p(struct l2cap_chan *chan,
				 struct l2cap_ctrl *control,
				 struct sk_buff *skb, u8 event)
{
	int err;

	BT_DBG("chan %p, control %p, skb %p, event %d", chan, control, skb,
	       event);

	if (!control->poll)
		return -EPROTO;

	l2cap_process_reqseq(chan, control->reqseq);

	if (!skb_queue_empty(&chan->tx_q))
		chan->tx_send_head = skb_peek(&chan->tx_q);
	else
		chan->tx_send_head = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Rewind next_tx_seq to the point expected
	 * by the receiver.
	 */
<<<<<<< HEAD
	pi->next_tx_seq = pi->amp_move_reqseq;
	pi->unacked_frames = 0;

	err = l2cap_finish_amp_move(sk);

	if (err)
		return err;

	pi->conn_state |= L2CAP_CONN_SEND_FBIT;
	l2cap_ertm_send_i_or_rr_or_rnr(sk);

	memset(&control, 0, sizeof(control));
	control.reqseq = pi->amp_move_reqseq;

	if (pi->amp_move_event == L2CAP_ERTM_EVENT_RECV_IFRAME)
		err = -EPROTO;
	else
		err = l2cap_ertm_rx_state_recv(sk, &control, NULL,
					pi->amp_move_event);
=======
	chan->next_tx_seq = control->reqseq;
	chan->unacked_frames = 0;

	err = l2cap_finish_move(chan);
	if (err)
		return err;

	set_bit(CONN_SEND_FBIT, &chan->conn_state);
	l2cap_send_i_or_rr_or_rnr(chan);

	if (event == L2CAP_EV_RECV_IFRAME)
		return -EPROTO;

	return l2cap_rx_state_recv(chan, control, NULL, event);
}

static int l2cap_rx_state_wait_f(struct l2cap_chan *chan,
				 struct l2cap_ctrl *control,
				 struct sk_buff *skb, u8 event)
{
	int err;

	if (!control->final)
		return -EPROTO;

	clear_bit(CONN_REMOTE_BUSY, &chan->conn_state);

	chan->rx_state = L2CAP_RX_STATE_RECV;
	l2cap_process_reqseq(chan, control->reqseq);

	if (!skb_queue_empty(&chan->tx_q))
		chan->tx_send_head = skb_peek(&chan->tx_q);
	else
		chan->tx_send_head = NULL;

	/* Rewind next_tx_seq to the point expected
	 * by the receiver.
	 */
	chan->next_tx_seq = control->reqseq;
	chan->unacked_frames = 0;
	chan->conn->mtu = chan->conn->hcon->hdev->acl_mtu;

	err = l2cap_resegment(chan);

	if (!err)
		err = l2cap_rx_state_recv(chan, control, skb, event);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return err;
}

<<<<<<< HEAD
static void l2cap_amp_move_setup(struct sock *sk)
{
	struct l2cap_pinfo *pi;
	struct sk_buff *skb;

	BT_DBG("sk %p", sk);

	pi = l2cap_pi(sk);

	l2cap_ertm_stop_ack_timer(pi);
	l2cap_ertm_stop_retrans_timer(pi);
	l2cap_ertm_stop_monitor_timer(pi);

	pi->retry_count = 0;
	skb_queue_walk(TX_QUEUE(sk), skb) {
		if (bt_cb(skb)->retries)
			bt_cb(skb)->retries = 1;
		else
			break;
	}

	pi->expected_tx_seq = pi->buffer_seq;

	pi->conn_state &= ~(L2CAP_CONN_REJ_ACT | L2CAP_CONN_SREJ_ACT);
	l2cap_seq_list_clear(&pi->retrans_list);
	l2cap_seq_list_clear(&l2cap_pi(sk)->srej_list);
	skb_queue_purge(SREJ_QUEUE(sk));

	pi->tx_state = L2CAP_ERTM_TX_STATE_XMIT;
	pi->rx_state = L2CAP_ERTM_RX_STATE_AMP_MOVE;

	BT_DBG("tx_state 0x2.2%x rx_state  0x2.2%x", pi->tx_state,
		pi->rx_state);

	pi->conn_state |= L2CAP_CONN_REMOTE_BUSY;
}

static void l2cap_amp_move_revert(struct sock *sk)
{
	struct l2cap_pinfo *pi;

	BT_DBG("sk %p", sk);

	pi = l2cap_pi(sk);

	if (pi->amp_move_role == L2CAP_AMP_MOVE_INITIATOR) {
		l2cap_ertm_tx(sk, NULL, NULL, L2CAP_ERTM_EVENT_EXPLICIT_POLL);
		pi->rx_state = L2CAP_ERTM_RX_STATE_WAIT_F_FLAG;
	} else if (pi->amp_move_role == L2CAP_AMP_MOVE_RESPONDER)
		pi->rx_state = L2CAP_ERTM_RX_STATE_WAIT_P_FLAG;
}

static int l2cap_amp_move_reconf(struct sock *sk)
{
	struct l2cap_pinfo *pi;
	u8 buf[64];
	int err = 0;

	BT_DBG("sk %p", sk);

	pi = l2cap_pi(sk);

	l2cap_send_cmd(pi->conn, l2cap_get_ident(pi->conn), L2CAP_CONF_REQ,
				l2cap_build_amp_reconf_req(sk, buf, sizeof(buf)), buf);
	return err;
}

static void l2cap_amp_move_success(struct sock *sk)
{
	struct l2cap_pinfo *pi;

	BT_DBG("sk %p", sk);

	pi = l2cap_pi(sk);

	if (pi->amp_move_role == L2CAP_AMP_MOVE_INITIATOR) {
		int err = 0;
		/* Send reconfigure request */
		if (pi->mode == L2CAP_MODE_ERTM) {
			pi->reconf_state = L2CAP_RECONF_INT;
			if (enable_reconfig)
				err = l2cap_amp_move_reconf(sk);

			if (err || !enable_reconfig) {
				pi->reconf_state = L2CAP_RECONF_NONE;
				l2cap_ertm_tx(sk, NULL, NULL,
						L2CAP_ERTM_EVENT_EXPLICIT_POLL);
				pi->rx_state = L2CAP_ERTM_RX_STATE_WAIT_F_FLAG;
			}
		} else
			pi->rx_state = L2CAP_ERTM_RX_STATE_RECV;
	} else if (pi->amp_move_role == L2CAP_AMP_MOVE_RESPONDER) {
		if (pi->mode == L2CAP_MODE_ERTM)
			pi->rx_state =
				L2CAP_ERTM_RX_STATE_WAIT_P_FLAG_RECONFIGURE;
		else
			pi->rx_state = L2CAP_ERTM_RX_STATE_RECV;
	}
}

static inline bool __valid_reqseq(struct l2cap_pinfo *pi, u16 reqseq)
{
	/* Make sure reqseq is for a packet that has been sent but not acked */
	u16 unacked = __delta_seq(pi->next_tx_seq, pi->expected_ack_seq, pi);
	return __delta_seq(pi->next_tx_seq, reqseq, pi) <= unacked;
}

static int l2cap_strm_rx(struct sock *sk, struct bt_l2cap_control *control,
			struct sk_buff *skb)
{
	struct l2cap_pinfo *pi;
	int err = 0;

	BT_DBG("sk %p, control %p, skb %p, state %d",
		sk, control, skb, l2cap_pi(sk)->rx_state);

	pi = l2cap_pi(sk);

	if (l2cap_ertm_classify_txseq(sk, control->txseq) ==
		L2CAP_ERTM_TXSEQ_EXPECTED) {
		l2cap_ertm_pass_to_tx(sk, control);

		BT_DBG("buffer_seq %d->%d", pi->buffer_seq,
			   __next_seq(pi->buffer_seq, pi));

		pi->buffer_seq = __next_seq(pi->buffer_seq, pi);

		l2cap_ertm_rx_expected_iframe(sk, control, skb);
	} else {
		if (pi->sdu) {
			kfree_skb(pi->sdu);
			pi->sdu = NULL;
		}
		pi->sdu_last_frag = NULL;
		pi->sdu_len = 0;

		if (skb) {
			BT_DBG("Freeing %p", skb);
			kfree_skb(skb);
		}
	}

	pi->last_acked_seq = control->txseq;
	pi->expected_tx_seq = __next_seq(control->txseq, pi);

	return err;
}

static int l2cap_ertm_rx(struct sock *sk, struct bt_l2cap_control *control,
			struct sk_buff *skb, u8 event)
{
	struct l2cap_pinfo *pi;
	int err = 0;

	BT_DBG("sk %p, control %p, skb %p, event %d, state %d",
		sk, control, skb, (int)event, l2cap_pi(sk)->rx_state);

	pi = l2cap_pi(sk);

	if (__valid_reqseq(pi, control->reqseq)) {
		switch (pi->rx_state) {
		case L2CAP_ERTM_RX_STATE_RECV:
			err = l2cap_ertm_rx_state_recv(sk, control, skb, event);
			break;
		case L2CAP_ERTM_RX_STATE_SREJ_SENT:
			err = l2cap_ertm_rx_state_srej_sent(sk, control, skb,
							event);
			break;
		case L2CAP_ERTM_RX_STATE_AMP_MOVE:
			err = l2cap_ertm_rx_state_amp_move(sk, control, skb,
							event);
			break;
		case L2CAP_ERTM_RX_STATE_WAIT_F_FLAG:
			if (control->final) {
				pi->conn_state &= ~L2CAP_CONN_REMOTE_BUSY;
				pi->amp_move_role = L2CAP_AMP_MOVE_NONE;

				pi->rx_state = L2CAP_ERTM_RX_STATE_RECV;
				l2cap_ertm_process_reqseq(sk, control->reqseq);

				if (!skb_queue_empty(TX_QUEUE(sk)))
					sk->sk_send_head =
						skb_peek(TX_QUEUE(sk));
				else
					sk->sk_send_head = NULL;

				/* Rewind next_tx_seq to the point expected
				 * by the receiver.
				 */
				pi->next_tx_seq = control->reqseq;
				pi->unacked_frames = 0;

				if (pi->ampcon)
					pi->conn->mtu =
						pi->ampcon->hdev->acl_mtu;
				else
					pi->conn->mtu =
						pi->conn->hcon->hdev->acl_mtu;

				err = l2cap_setup_resegment(sk);

				if (err)
					break;

				err = l2cap_ertm_rx_state_recv(sk, control, skb,
							event);
			}
			break;
		case L2CAP_ERTM_RX_STATE_WAIT_P_FLAG:
			if (control->poll) {
				pi->amp_move_reqseq = control->reqseq;
				pi->amp_move_event = event;
				err = l2cap_answer_move_poll(sk);
			}
			break;
		case L2CAP_ERTM_RX_STATE_WAIT_P_FLAG_RECONFIGURE:
			if (control->poll) {
				pi->amp_move_reqseq = control->reqseq;
				pi->amp_move_event = event;

				BT_DBG("amp_move_role 0x%2.2x, "
					"reconf_state 0x%2.2x",
					pi->amp_move_role, pi->reconf_state);

				if (pi->reconf_state == L2CAP_RECONF_ACC)
					err = l2cap_amp_move_reconf(sk);
				else
					err = l2cap_answer_move_poll(sk);
			}
=======
static bool __valid_reqseq(struct l2cap_chan *chan, u16 reqseq)
{
	/* Make sure reqseq is for a packet that has been sent but not acked */
	u16 unacked;

	unacked = __seq_offset(chan, chan->next_tx_seq, chan->expected_ack_seq);
	return __seq_offset(chan, chan->next_tx_seq, reqseq) <= unacked;
}

static int l2cap_rx(struct l2cap_chan *chan, struct l2cap_ctrl *control,
		    struct sk_buff *skb, u8 event)
{
	int err = 0;

	BT_DBG("chan %p, control %p, skb %p, event %d, state %d", chan,
	       control, skb, event, chan->rx_state);

	if (__valid_reqseq(chan, control->reqseq)) {
		switch (chan->rx_state) {
		case L2CAP_RX_STATE_RECV:
			err = l2cap_rx_state_recv(chan, control, skb, event);
			break;
		case L2CAP_RX_STATE_SREJ_SENT:
			err = l2cap_rx_state_srej_sent(chan, control, skb,
						       event);
			break;
		case L2CAP_RX_STATE_WAIT_P:
			err = l2cap_rx_state_wait_p(chan, control, skb, event);
			break;
		case L2CAP_RX_STATE_WAIT_F:
			err = l2cap_rx_state_wait_f(chan, control, skb, event);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		default:
			/* shut it down */
			break;
		}
	} else {
		BT_DBG("Invalid reqseq %d (next_tx_seq %d, expected_ack_seq %d",
<<<<<<< HEAD
			control->reqseq, pi->next_tx_seq, pi->expected_ack_seq);
		l2cap_send_disconn_req(pi->conn, sk, ECONNRESET);
=======
		       control->reqseq, chan->next_tx_seq,
		       chan->expected_ack_seq);
		l2cap_send_disconn_req(chan, ECONNRESET);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return err;
}

<<<<<<< HEAD
void l2cap_fixed_channel_config(struct sock *sk, struct l2cap_options *opt)
{
	lock_sock(sk);

	l2cap_pi(sk)->fixed_channel = 1;

	l2cap_pi(sk)->imtu = opt->imtu;
	l2cap_pi(sk)->omtu = opt->omtu;
	l2cap_pi(sk)->remote_mps = opt->omtu;
	l2cap_pi(sk)->mps = opt->omtu;
	l2cap_pi(sk)->flush_to = opt->flush_to;
	l2cap_pi(sk)->mode = opt->mode;
	l2cap_pi(sk)->fcs = opt->fcs;
	l2cap_pi(sk)->max_tx = opt->max_tx;
	l2cap_pi(sk)->remote_max_tx = opt->max_tx;
	l2cap_pi(sk)->tx_win = opt->txwin_size;
	l2cap_pi(sk)->remote_tx_win = opt->txwin_size;
	l2cap_pi(sk)->retrans_timeout = L2CAP_DEFAULT_RETRANS_TO;
	l2cap_pi(sk)->monitor_timeout = L2CAP_DEFAULT_MONITOR_TO;

	if (opt->mode == L2CAP_MODE_ERTM ||
		l2cap_pi(sk)->mode == L2CAP_MODE_STREAMING)
		l2cap_ertm_init(sk);

	release_sock(sk);

	return;
}

static const u8 l2cap_ertm_rx_func_to_event[4] = {
	L2CAP_ERTM_EVENT_RECV_RR, L2CAP_ERTM_EVENT_RECV_REJ,
	L2CAP_ERTM_EVENT_RECV_RNR, L2CAP_ERTM_EVENT_RECV_SREJ
};

int l2cap_data_channel(struct sock *sk, struct sk_buff *skb)
{
	struct l2cap_pinfo *pi;
	struct bt_l2cap_control *control;
	u16 len;
	u8 event;
	pi = l2cap_pi(sk);

	BT_DBG("sk %p, len %d, mode %d", sk, skb->len, pi->mode);

	if (sk->sk_state != BT_CONNECTED)
		goto drop;

	switch (pi->mode) {
=======
static int l2cap_stream_rx(struct l2cap_chan *chan, struct l2cap_ctrl *control,
			   struct sk_buff *skb)
{
	/* l2cap_reassemble_sdu may free skb, hence invalidate control, so store
	 * the txseq field in advance to use it after l2cap_reassemble_sdu
	 * returns and to avoid the race condition, for example:
	 *
	 * The current thread calls:
	 *   l2cap_reassemble_sdu
	 *     chan->ops->recv == l2cap_sock_recv_cb
	 *       __sock_queue_rcv_skb
	 * Another thread calls:
	 *   bt_sock_recvmsg
	 *     skb_recv_datagram
	 *     skb_free_datagram
	 * Then the current thread tries to access control, but it was freed by
	 * skb_free_datagram.
	 */
	u16 txseq = control->txseq;

	BT_DBG("chan %p, control %p, skb %p, state %d", chan, control, skb,
	       chan->rx_state);

	if (l2cap_classify_txseq(chan, txseq) == L2CAP_TXSEQ_EXPECTED) {
		l2cap_pass_to_tx(chan, control);

		BT_DBG("buffer_seq %u->%u", chan->buffer_seq,
		       __next_seq(chan, chan->buffer_seq));

		chan->buffer_seq = __next_seq(chan, chan->buffer_seq);

		l2cap_reassemble_sdu(chan, skb, control);
	} else {
		if (chan->sdu) {
			kfree_skb(chan->sdu);
			chan->sdu = NULL;
		}
		chan->sdu_last_frag = NULL;
		chan->sdu_len = 0;

		if (skb) {
			BT_DBG("Freeing %p", skb);
			kfree_skb(skb);
		}
	}

	chan->last_acked_seq = txseq;
	chan->expected_tx_seq = __next_seq(chan, txseq);

	return 0;
}

static int l2cap_data_rcv(struct l2cap_chan *chan, struct sk_buff *skb)
{
	struct l2cap_ctrl *control = &bt_cb(skb)->l2cap;
	u16 len;
	u8 event;

	__unpack_control(chan, skb);

	len = skb->len;

	/*
	 * We can just drop the corrupted I-frame here.
	 * Receiver will miss it and start proper recovery
	 * procedures and ask for retransmission.
	 */
	if (l2cap_check_fcs(chan, skb))
		goto drop;

	if (!control->sframe && control->sar == L2CAP_SAR_START)
		len -= L2CAP_SDULEN_SIZE;

	if (chan->fcs == L2CAP_FCS_CRC16)
		len -= L2CAP_FCS_SIZE;

	if (len > chan->mps) {
		l2cap_send_disconn_req(chan, ECONNRESET);
		goto drop;
	}

	if (chan->ops->filter) {
		if (chan->ops->filter(chan, skb))
			goto drop;
	}

	if (!control->sframe) {
		int err;

		BT_DBG("iframe sar %d, reqseq %d, final %d, txseq %d",
		       control->sar, control->reqseq, control->final,
		       control->txseq);

		/* Validate F-bit - F=0 always valid, F=1 only
		 * valid in TX WAIT_F
		 */
		if (control->final && chan->tx_state != L2CAP_TX_STATE_WAIT_F)
			goto drop;

		if (chan->mode != L2CAP_MODE_STREAMING) {
			event = L2CAP_EV_RECV_IFRAME;
			err = l2cap_rx(chan, control, skb, event);
		} else {
			err = l2cap_stream_rx(chan, control, skb);
		}

		if (err)
			l2cap_send_disconn_req(chan, ECONNRESET);
	} else {
		const u8 rx_func_to_event[4] = {
			L2CAP_EV_RECV_RR, L2CAP_EV_RECV_REJ,
			L2CAP_EV_RECV_RNR, L2CAP_EV_RECV_SREJ
		};

		/* Only I-frames are expected in streaming mode */
		if (chan->mode == L2CAP_MODE_STREAMING)
			goto drop;

		BT_DBG("sframe reqseq %d, final %d, poll %d, super %d",
		       control->reqseq, control->final, control->poll,
		       control->super);

		if (len != 0) {
			BT_ERR("Trailing bytes: %d in sframe", len);
			l2cap_send_disconn_req(chan, ECONNRESET);
			goto drop;
		}

		/* Validate F and P bits */
		if (control->final && (control->poll ||
				       chan->tx_state != L2CAP_TX_STATE_WAIT_F))
			goto drop;

		event = rx_func_to_event[control->super];
		if (l2cap_rx(chan, control, skb, event))
			l2cap_send_disconn_req(chan, ECONNRESET);
	}

	return 0;

drop:
	kfree_skb(skb);
	return 0;
}

static void l2cap_chan_le_send_credits(struct l2cap_chan *chan)
{
	struct l2cap_conn *conn = chan->conn;
	struct l2cap_le_credits pkt;
	u16 return_credits;

	return_credits = (chan->imtu / chan->mps) + 1;

	if (chan->rx_credits >= return_credits)
		return;

	return_credits -= chan->rx_credits;

	BT_DBG("chan %p returning %u credits to sender", chan, return_credits);

	chan->rx_credits += return_credits;

	pkt.cid     = cpu_to_le16(chan->scid);
	pkt.credits = cpu_to_le16(return_credits);

	chan->ident = l2cap_get_ident(conn);

	l2cap_send_cmd(conn, chan->ident, L2CAP_LE_CREDITS, sizeof(pkt), &pkt);
}

static int l2cap_ecred_recv(struct l2cap_chan *chan, struct sk_buff *skb)
{
	int err;

	BT_DBG("SDU reassemble complete: chan %p skb->len %u", chan, skb->len);

	/* Wait recv to confirm reception before updating the credits */
	err = chan->ops->recv(chan, skb);

	/* Update credits whenever an SDU is received */
	l2cap_chan_le_send_credits(chan);

	return err;
}

static int l2cap_ecred_data_rcv(struct l2cap_chan *chan, struct sk_buff *skb)
{
	int err;

	if (!chan->rx_credits) {
		BT_ERR("No credits to receive LE L2CAP data");
		l2cap_send_disconn_req(chan, ECONNRESET);
		return -ENOBUFS;
	}

	if (chan->imtu < skb->len) {
		BT_ERR("Too big LE L2CAP PDU");
		return -ENOBUFS;
	}

	chan->rx_credits--;
	BT_DBG("rx_credits %u -> %u", chan->rx_credits + 1, chan->rx_credits);

	/* Update if remote had run out of credits, this should only happens
	 * if the remote is not using the entire MPS.
	 */
	if (!chan->rx_credits)
		l2cap_chan_le_send_credits(chan);

	err = 0;

	if (!chan->sdu) {
		u16 sdu_len;

		sdu_len = get_unaligned_le16(skb->data);
		skb_pull(skb, L2CAP_SDULEN_SIZE);

		BT_DBG("Start of new SDU. sdu_len %u skb->len %u imtu %u",
		       sdu_len, skb->len, chan->imtu);

		if (sdu_len > chan->imtu) {
			BT_ERR("Too big LE L2CAP SDU length received");
			err = -EMSGSIZE;
			goto failed;
		}

		if (skb->len > sdu_len) {
			BT_ERR("Too much LE L2CAP data received");
			err = -EINVAL;
			goto failed;
		}

		if (skb->len == sdu_len)
			return l2cap_ecred_recv(chan, skb);

		chan->sdu = skb;
		chan->sdu_len = sdu_len;
		chan->sdu_last_frag = skb;

		/* Detect if remote is not able to use the selected MPS */
		if (skb->len + L2CAP_SDULEN_SIZE < chan->mps) {
			u16 mps_len = skb->len + L2CAP_SDULEN_SIZE;

			/* Adjust the number of credits */
			BT_DBG("chan->mps %u -> %u", chan->mps, mps_len);
			chan->mps = mps_len;
			l2cap_chan_le_send_credits(chan);
		}

		return 0;
	}

	BT_DBG("SDU fragment. chan->sdu->len %u skb->len %u chan->sdu_len %u",
	       chan->sdu->len, skb->len, chan->sdu_len);

	if (chan->sdu->len + skb->len > chan->sdu_len) {
		BT_ERR("Too much LE L2CAP data received");
		err = -EINVAL;
		goto failed;
	}

	append_skb_frag(chan->sdu, skb, &chan->sdu_last_frag);
	skb = NULL;

	if (chan->sdu->len == chan->sdu_len) {
		err = l2cap_ecred_recv(chan, chan->sdu);
		if (!err) {
			chan->sdu = NULL;
			chan->sdu_last_frag = NULL;
			chan->sdu_len = 0;
		}
	}

failed:
	if (err) {
		kfree_skb(skb);
		kfree_skb(chan->sdu);
		chan->sdu = NULL;
		chan->sdu_last_frag = NULL;
		chan->sdu_len = 0;
	}

	/* We can't return an error here since we took care of the skb
	 * freeing internally. An error return would cause the caller to
	 * do a double-free of the skb.
	 */
	return 0;
}

static void l2cap_data_channel(struct l2cap_conn *conn, u16 cid,
			       struct sk_buff *skb)
{
	struct l2cap_chan *chan;

	chan = l2cap_get_chan_by_scid(conn, cid);
	if (!chan) {
		BT_DBG("unknown cid 0x%4.4x", cid);
		/* Drop packet and return */
		kfree_skb(skb);
		return;
	}

	BT_DBG("chan %p, len %d", chan, skb->len);

	/* If we receive data on a fixed channel before the info req/rsp
	 * procedure is done simply assume that the channel is supported
	 * and mark it as ready.
	 */
	if (chan->chan_type == L2CAP_CHAN_FIXED)
		l2cap_chan_ready(chan);

	if (chan->state != BT_CONNECTED)
		goto drop;

	switch (chan->mode) {
	case L2CAP_MODE_LE_FLOWCTL:
	case L2CAP_MODE_EXT_FLOWCTL:
		if (l2cap_ecred_data_rcv(chan, skb) < 0)
			goto drop;

		goto done;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case L2CAP_MODE_BASIC:
		/* If socket recv buffers overflows we drop data here
		 * which is *bad* because L2CAP has to be reliable.
		 * But we don't have any other choice. L2CAP doesn't
		 * provide flow control mechanism. */

<<<<<<< HEAD
		if (pi->imtu < skb->len)
			goto drop;

		if (!sock_queue_rcv_skb(sk, skb))
=======
		if (chan->imtu < skb->len) {
			BT_ERR("Dropping L2CAP data: receive buffer overflow");
			goto drop;
		}

		if (!chan->ops->recv(chan, skb))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto done;
		break;

	case L2CAP_MODE_ERTM:
	case L2CAP_MODE_STREAMING:
<<<<<<< HEAD
		control = &bt_cb(skb)->control;
		if (pi->extended_control) {
			__get_extended_control(get_unaligned_le32(skb->data),
						control);
			skb_pull(skb, 4);
		} else {
			__get_enhanced_control(get_unaligned_le16(skb->data),
						control);
			skb_pull(skb, 2);
		}

		len = skb->len;

		if (l2cap_check_fcs(pi, skb))
			goto drop;

		if ((control->frame_type == 'i') &&
			(control->sar == L2CAP_SAR_START))
			len -= 2;

		if (pi->fcs == L2CAP_FCS_CRC16)
			len -= 2;

		/*
		 * We can just drop the corrupted I-frame here.
		 * Receiver will miss it and start proper recovery
		 * procedures and ask for retransmission.
		 */
		if (len > pi->mps) {
			l2cap_send_disconn_req(pi->conn, sk, ECONNRESET);
			goto drop;
		}

		if (control->frame_type == 'i') {

			int err;

			BT_DBG("iframe sar %d, reqseq %d, final %d, txseq %d",
				control->sar, control->reqseq, control->final,
				control->txseq);

			/* Validate F-bit - F=0 always valid, F=1 only
			 * valid in TX WAIT_F
			 */
			if (control->final && (pi->tx_state !=
					L2CAP_ERTM_TX_STATE_WAIT_F))
				goto drop;

			if (pi->mode != L2CAP_MODE_STREAMING) {
				event = L2CAP_ERTM_EVENT_RECV_IFRAME;
				err = l2cap_ertm_rx(sk, control, skb, event);
			} else
				err = l2cap_strm_rx(sk, control, skb);
			if (err)
				l2cap_send_disconn_req(pi->conn, sk,
						ECONNRESET);
		} else {
			/* Only I-frames are expected in streaming mode */
			if (pi->mode == L2CAP_MODE_STREAMING)
				goto drop;

			BT_DBG("sframe reqseq %d, final %d, poll %d, super %d",
				control->reqseq, control->final, control->poll,
				control->super);

			if (len != 0) {
				l2cap_send_disconn_req(pi->conn, sk,
						ECONNRESET);
				goto drop;
			}

			/* Validate F and P bits */
			if (control->final &&
				((pi->tx_state != L2CAP_ERTM_TX_STATE_WAIT_F)
					|| control->poll))
				goto drop;

			event = l2cap_ertm_rx_func_to_event[control->super];
			if (l2cap_ertm_rx(sk, control, skb, event))
				l2cap_send_disconn_req(pi->conn, sk,
						ECONNRESET);
		}

		goto done;

	default:
		BT_DBG("sk %p: bad mode 0x%2.2x", sk, pi->mode);
=======
		l2cap_data_rcv(chan, skb);
		goto done;

	default:
		BT_DBG("chan %p: bad mode 0x%2.2x", chan, chan->mode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	}

drop:
	kfree_skb(skb);

done:
<<<<<<< HEAD
	return 0;
}

void l2cap_recv_deferred_frame(struct sock *sk, struct sk_buff *skb)
{
	lock_sock(sk);
	l2cap_data_channel(sk, skb);
	release_sock(sk);
}

static inline int l2cap_conless_channel(struct l2cap_conn *conn, __le16 psm, struct sk_buff *skb)
{
	struct sock *sk;

	sk = l2cap_get_sock_by_psm(0, psm, conn->src);
	if (!sk)
		goto drop;

	bh_lock_sock(sk);

	BT_DBG("sk %p, len %d", sk, skb->len);

	if (sk->sk_state != BT_BOUND && sk->sk_state != BT_CONNECTED)
		goto drop;

	if (l2cap_pi(sk)->imtu < skb->len)
		goto drop;

	if (!sock_queue_rcv_skb(sk, skb))
		goto done;

drop:
	kfree_skb(skb);

done:
	if (sk)
		bh_unlock_sock(sk);
	return 0;
}

static inline int l2cap_att_channel(struct l2cap_conn *conn, __le16 cid,
							struct sk_buff *skb)
{
	struct sock *sk = NULL;
	struct sk_buff *skb_rsp;
	struct l2cap_hdr *lh;
	int dir;
	struct work_struct *open_worker;
	u8 err_rsp[] = {L2CAP_ATT_ERROR, 0x00, 0x00, 0x00,
						L2CAP_ATT_NOT_SUPPORTED};

	if (skb->data[0] == L2CAP_ATT_MTU_REQ) {
		u8 mtu_rsp[] = {L2CAP_ATT_MTU_RSP, 23, 0};

		skb_rsp = bt_skb_alloc(sizeof(mtu_rsp) + L2CAP_HDR_SIZE,
								GFP_ATOMIC);
		if (!skb_rsp)
			goto drop;

		lh = (struct l2cap_hdr *) skb_put(skb_rsp, L2CAP_HDR_SIZE);
		lh->len = cpu_to_le16(sizeof(mtu_rsp));
		lh->cid = cpu_to_le16(L2CAP_CID_LE_DATA);
		memcpy(skb_put(skb_rsp, sizeof(mtu_rsp)), mtu_rsp,
							sizeof(mtu_rsp));
		hci_send_acl(conn->hcon, NULL, skb_rsp, 0);

		goto free_skb;
	}

	dir = (skb->data[0] & L2CAP_ATT_RESPONSE_BIT) ? 0 : 1;

	sk = l2cap_find_sock_by_fixed_cid_and_dir(cid, conn->src,
							conn->dst, dir);

	BT_DBG("sk %p, dir:%d", sk, dir);

	if (!sk)
		goto drop;

	bh_lock_sock(sk);

	BT_DBG("sk %p, len %d", sk, skb->len);

	if (sk->sk_state != BT_BOUND && sk->sk_state != BT_CONNECTED) {
		att_chn_params.cid = cid;
		att_chn_params.conn = conn;
		att_chn_params.dir = dir;
		att_chn_params.skb = skb;
		open_worker = kzalloc(sizeof(*open_worker), GFP_ATOMIC);
		if (!open_worker)
			BT_ERR("Out of memory");
		INIT_WORK(open_worker, l2cap_queue_acl_data);
		schedule_work(open_worker);
		goto done;
	}

	if (l2cap_pi(sk)->imtu < skb->len)
		goto drop;

	if (!sock_queue_rcv_skb(sk, skb))
		goto done;

drop:
	if (skb->data[0] != L2CAP_ATT_INDICATE)
		goto not_indicate;

	/* If this is an incoming Indication, we are required to confirm */

	skb_rsp = bt_skb_alloc(sizeof(u8) + L2CAP_HDR_SIZE, GFP_ATOMIC);
	if (!skb_rsp)
		goto free_skb;

	lh = (struct l2cap_hdr *) skb_put(skb_rsp, L2CAP_HDR_SIZE);
	lh->len = cpu_to_le16(sizeof(u8));
	lh->cid = cpu_to_le16(L2CAP_CID_LE_DATA);
	err_rsp[0] = L2CAP_ATT_CONFIRM;
	memcpy(skb_put(skb_rsp, sizeof(u8)), err_rsp, sizeof(u8));
	hci_send_acl(conn->hcon, NULL, skb_rsp, 0);
	goto free_skb;

not_indicate:
	if (skb->data[0] & L2CAP_ATT_RESPONSE_BIT ||
			skb->data[0] == L2CAP_ATT_CONFIRM)
		goto free_skb;

	/* If this is an incoming PDU that requires a response, respond with
	 * a generic error so remote device doesn't hang */

	skb_rsp = bt_skb_alloc(sizeof(err_rsp) + L2CAP_HDR_SIZE, GFP_ATOMIC);
	if (!skb_rsp)
		goto free_skb;

	lh = (struct l2cap_hdr *) skb_put(skb_rsp, L2CAP_HDR_SIZE);
	lh->len = cpu_to_le16(sizeof(err_rsp));
	lh->cid = cpu_to_le16(L2CAP_CID_LE_DATA);
	err_rsp[1] = skb->data[0];
	memcpy(skb_put(skb_rsp, sizeof(err_rsp)), err_rsp, sizeof(err_rsp));
	hci_send_acl(conn->hcon, NULL, skb_rsp, 0);

free_skb:
	kfree_skb(skb);

done:
	if (sk)
		bh_unlock_sock(sk);
	return 0;
=======
	l2cap_chan_unlock(chan);
	l2cap_chan_put(chan);
}

static void l2cap_conless_channel(struct l2cap_conn *conn, __le16 psm,
				  struct sk_buff *skb)
{
	struct hci_conn *hcon = conn->hcon;
	struct l2cap_chan *chan;

	if (hcon->type != ACL_LINK)
		goto free_skb;

	chan = l2cap_global_chan_by_psm(0, psm, &hcon->src, &hcon->dst,
					ACL_LINK);
	if (!chan)
		goto free_skb;

	BT_DBG("chan %p, len %d", chan, skb->len);

	if (chan->state != BT_BOUND && chan->state != BT_CONNECTED)
		goto drop;

	if (chan->imtu < skb->len)
		goto drop;

	/* Store remote BD_ADDR and PSM for msg_name */
	bacpy(&bt_cb(skb)->l2cap.bdaddr, &hcon->dst);
	bt_cb(skb)->l2cap.psm = psm;

	if (!chan->ops->recv(chan, skb)) {
		l2cap_chan_put(chan);
		return;
	}

drop:
	l2cap_chan_put(chan);
free_skb:
	kfree_skb(skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void l2cap_recv_frame(struct l2cap_conn *conn, struct sk_buff *skb)
{
	struct l2cap_hdr *lh = (void *) skb->data;
<<<<<<< HEAD
	struct sock *sk;
	u16 cid, len;
	__le16 psm;

=======
	struct hci_conn *hcon = conn->hcon;
	u16 cid, len;
	__le16 psm;

	if (hcon->state != BT_CONNECTED) {
		BT_DBG("queueing pending rx skb");
		skb_queue_tail(&conn->pending_rx, skb);
		return;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	skb_pull(skb, L2CAP_HDR_SIZE);
	cid = __le16_to_cpu(lh->cid);
	len = __le16_to_cpu(lh->len);

	if (len != skb->len) {
		kfree_skb(skb);
		return;
	}

<<<<<<< HEAD
	BT_DBG("len %d, cid 0x%4.4x", len, cid);

	switch (cid) {
	case L2CAP_CID_LE_SIGNALING:
=======
	/* Since we can't actively block incoming LE connections we must
	 * at least ensure that we ignore incoming data from them.
	 */
	if (hcon->type == LE_LINK &&
	    hci_bdaddr_list_lookup(&hcon->hdev->reject_list, &hcon->dst,
				   bdaddr_dst_type(hcon))) {
		kfree_skb(skb);
		return;
	}

	BT_DBG("len %d, cid 0x%4.4x", len, cid);

	switch (cid) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case L2CAP_CID_SIGNALING:
		l2cap_sig_channel(conn, skb);
		break;

	case L2CAP_CID_CONN_LESS:
<<<<<<< HEAD
		psm = get_unaligned_le16(skb->data);
		skb_pull(skb, 2);
		l2cap_conless_channel(conn, psm, skb);
		break;

	case L2CAP_CID_LE_DATA:
		l2cap_att_channel(conn, cid, skb);
		break;

	case L2CAP_CID_SMP:
		if (smp_sig_channel(conn, skb))
			l2cap_conn_del(conn->hcon, EACCES, 0);
		break;

	default:
		sk = l2cap_get_chan_by_scid(&conn->chan_list, cid);
		if (sk) {
			if (sock_owned_by_user(sk)) {
				BT_DBG("backlog sk %p", sk);
				if (sk_add_backlog(sk, skb))
					kfree_skb(skb);
			} else
				l2cap_data_channel(sk, skb);

			bh_unlock_sock(sk);
		} else if ((cid == L2CAP_CID_A2MP) && enable_hs) {
			BT_DBG("A2MP");
			amp_conn_ind(conn->hcon, skb);
		} else {
			BT_DBG("unknown cid 0x%4.4x", cid);
			kfree_skb(skb);
		}

=======
		psm = get_unaligned((__le16 *) skb->data);
		skb_pull(skb, L2CAP_PSMLEN_SIZE);
		l2cap_conless_channel(conn, psm, skb);
		break;

	case L2CAP_CID_LE_SIGNALING:
		l2cap_le_sig_channel(conn, skb);
		break;

	default:
		l2cap_data_channel(conn, cid, skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	}
}

<<<<<<< HEAD
/* ---- L2CAP interface with lower layer (HCI) ---- */

static int l2cap_connect_ind(struct hci_dev *hdev, bdaddr_t *bdaddr, u8 type)
{
	int exact = 0, lm1 = 0, lm2 = 0;
	register struct sock *sk;
	struct hlist_node *node;

	if (type != ACL_LINK)
		return 0;

	BT_DBG("hdev %s, bdaddr %s", hdev->name, batostr(bdaddr));

	/* Find listening sockets and check their link_mode */
	read_lock(&l2cap_sk_list.lock);
	sk_for_each(sk, node, &l2cap_sk_list.head) {
		if (sk->sk_state != BT_LISTEN)
			continue;

		if (!bacmp(&bt_sk(sk)->src, &hdev->bdaddr)) {
			lm1 |= HCI_LM_ACCEPT;
			if (l2cap_pi(sk)->role_switch)
				lm1 |= HCI_LM_MASTER;
			exact++;
		} else if (!bacmp(&bt_sk(sk)->src, BDADDR_ANY)) {
			lm2 |= HCI_LM_ACCEPT;
			if (l2cap_pi(sk)->role_switch)
				lm2 |= HCI_LM_MASTER;
		}
	}
	read_unlock(&l2cap_sk_list.lock);

	return exact ? lm1 : lm2;
}

static int l2cap_connect_cfm(struct hci_conn *hcon, u8 status)
{
	struct l2cap_conn *conn;

	BT_DBG("hcon %p bdaddr %s status %d", hcon, batostr(&hcon->dst), status);

	if (!(hcon->type == ACL_LINK || hcon->type == LE_LINK))
		return -EINVAL;

	if (!status) {
		conn = l2cap_conn_add(hcon, status);
		if (conn)
			l2cap_conn_ready(conn);
	} else
		l2cap_conn_del(hcon, bt_err(status), 0);
=======
static void process_pending_rx(struct work_struct *work)
{
	struct l2cap_conn *conn = container_of(work, struct l2cap_conn,
					       pending_rx_work);
	struct sk_buff *skb;

	BT_DBG("");

	while ((skb = skb_dequeue(&conn->pending_rx)))
		l2cap_recv_frame(conn, skb);
}

static struct l2cap_conn *l2cap_conn_add(struct hci_conn *hcon)
{
	struct l2cap_conn *conn = hcon->l2cap_data;
	struct hci_chan *hchan;

	if (conn)
		return conn;

	hchan = hci_chan_create(hcon);
	if (!hchan)
		return NULL;

	conn = kzalloc(sizeof(*conn), GFP_KERNEL);
	if (!conn) {
		hci_chan_del(hchan);
		return NULL;
	}

	kref_init(&conn->ref);
	hcon->l2cap_data = conn;
	conn->hcon = hci_conn_get(hcon);
	conn->hchan = hchan;

	BT_DBG("hcon %p conn %p hchan %p", hcon, conn, hchan);

	switch (hcon->type) {
	case LE_LINK:
		if (hcon->hdev->le_mtu) {
			conn->mtu = hcon->hdev->le_mtu;
			break;
		}
		fallthrough;
	default:
		conn->mtu = hcon->hdev->acl_mtu;
		break;
	}

	conn->feat_mask = 0;

	conn->local_fixed_chan = L2CAP_FC_SIG_BREDR | L2CAP_FC_CONNLESS;

	if (hci_dev_test_flag(hcon->hdev, HCI_LE_ENABLED) &&
	    (bredr_sc_enabled(hcon->hdev) ||
	     hci_dev_test_flag(hcon->hdev, HCI_FORCE_BREDR_SMP)))
		conn->local_fixed_chan |= L2CAP_FC_SMP_BREDR;

	mutex_init(&conn->ident_lock);
	mutex_init(&conn->chan_lock);

	INIT_LIST_HEAD(&conn->chan_l);
	INIT_LIST_HEAD(&conn->users);

	INIT_DELAYED_WORK(&conn->info_timer, l2cap_info_timeout);

	skb_queue_head_init(&conn->pending_rx);
	INIT_WORK(&conn->pending_rx_work, process_pending_rx);
	INIT_DELAYED_WORK(&conn->id_addr_timer, l2cap_conn_update_id_addr);

	conn->disc_reason = HCI_ERROR_REMOTE_USER_TERM;

	return conn;
}

static bool is_valid_psm(u16 psm, u8 dst_type)
{
	if (!psm)
		return false;

	if (bdaddr_type_is_le(dst_type))
		return (psm <= 0x00ff);

	/* PSM must be odd and lsb of upper byte must be 0 */
	return ((psm & 0x0101) == 0x0001);
}

struct l2cap_chan_data {
	struct l2cap_chan *chan;
	struct pid *pid;
	int count;
};

static void l2cap_chan_by_pid(struct l2cap_chan *chan, void *data)
{
	struct l2cap_chan_data *d = data;
	struct pid *pid;

	if (chan == d->chan)
		return;

	if (!test_bit(FLAG_DEFER_SETUP, &chan->flags))
		return;

	pid = chan->ops->get_peer_pid(chan);

	/* Only count deferred channels with the same PID/PSM */
	if (d->pid != pid || chan->psm != d->chan->psm || chan->ident ||
	    chan->mode != L2CAP_MODE_EXT_FLOWCTL || chan->state != BT_CONNECT)
		return;

	d->count++;
}

int l2cap_chan_connect(struct l2cap_chan *chan, __le16 psm, u16 cid,
		       bdaddr_t *dst, u8 dst_type, u16 timeout)
{
	struct l2cap_conn *conn;
	struct hci_conn *hcon;
	struct hci_dev *hdev;
	int err;

	BT_DBG("%pMR -> %pMR (type %u) psm 0x%4.4x mode 0x%2.2x", &chan->src,
	       dst, dst_type, __le16_to_cpu(psm), chan->mode);

	hdev = hci_get_route(dst, &chan->src, chan->src_type);
	if (!hdev)
		return -EHOSTUNREACH;

	hci_dev_lock(hdev);

	if (!is_valid_psm(__le16_to_cpu(psm), dst_type) && !cid &&
	    chan->chan_type != L2CAP_CHAN_RAW) {
		err = -EINVAL;
		goto done;
	}

	if (chan->chan_type == L2CAP_CHAN_CONN_ORIENTED && !psm) {
		err = -EINVAL;
		goto done;
	}

	if (chan->chan_type == L2CAP_CHAN_FIXED && !cid) {
		err = -EINVAL;
		goto done;
	}

	switch (chan->mode) {
	case L2CAP_MODE_BASIC:
		break;
	case L2CAP_MODE_LE_FLOWCTL:
		break;
	case L2CAP_MODE_EXT_FLOWCTL:
		if (!enable_ecred) {
			err = -EOPNOTSUPP;
			goto done;
		}
		break;
	case L2CAP_MODE_ERTM:
	case L2CAP_MODE_STREAMING:
		if (!disable_ertm)
			break;
		fallthrough;
	default:
		err = -EOPNOTSUPP;
		goto done;
	}

	switch (chan->state) {
	case BT_CONNECT:
	case BT_CONNECT2:
	case BT_CONFIG:
		/* Already connecting */
		err = 0;
		goto done;

	case BT_CONNECTED:
		/* Already connected */
		err = -EISCONN;
		goto done;

	case BT_OPEN:
	case BT_BOUND:
		/* Can connect */
		break;

	default:
		err = -EBADFD;
		goto done;
	}

	/* Set destination address and psm */
	bacpy(&chan->dst, dst);
	chan->dst_type = dst_type;

	chan->psm = psm;
	chan->dcid = cid;

	if (bdaddr_type_is_le(dst_type)) {
		/* Convert from L2CAP channel address type to HCI address type
		 */
		if (dst_type == BDADDR_LE_PUBLIC)
			dst_type = ADDR_LE_DEV_PUBLIC;
		else
			dst_type = ADDR_LE_DEV_RANDOM;

		if (hci_dev_test_flag(hdev, HCI_ADVERTISING))
			hcon = hci_connect_le(hdev, dst, dst_type, false,
					      chan->sec_level, timeout,
					      HCI_ROLE_SLAVE, 0, 0);
		else
			hcon = hci_connect_le_scan(hdev, dst, dst_type,
						   chan->sec_level, timeout,
						   CONN_REASON_L2CAP_CHAN);

	} else {
		u8 auth_type = l2cap_get_auth_type(chan);
		hcon = hci_connect_acl(hdev, dst, chan->sec_level, auth_type,
				       CONN_REASON_L2CAP_CHAN, timeout);
	}

	if (IS_ERR(hcon)) {
		err = PTR_ERR(hcon);
		goto done;
	}

	conn = l2cap_conn_add(hcon);
	if (!conn) {
		hci_conn_drop(hcon);
		err = -ENOMEM;
		goto done;
	}

	if (chan->mode == L2CAP_MODE_EXT_FLOWCTL) {
		struct l2cap_chan_data data;

		data.chan = chan;
		data.pid = chan->ops->get_peer_pid(chan);
		data.count = 1;

		l2cap_chan_list(conn, l2cap_chan_by_pid, &data);

		/* Check if there isn't too many channels being connected */
		if (data.count > L2CAP_ECRED_CONN_SCID_MAX) {
			hci_conn_drop(hcon);
			err = -EPROTO;
			goto done;
		}
	}

	mutex_lock(&conn->chan_lock);
	l2cap_chan_lock(chan);

	if (cid && __l2cap_get_chan_by_dcid(conn, cid)) {
		hci_conn_drop(hcon);
		err = -EBUSY;
		goto chan_unlock;
	}

	/* Update source addr of the socket */
	bacpy(&chan->src, &hcon->src);
	chan->src_type = bdaddr_src_type(hcon);

	__l2cap_chan_add(conn, chan);

	/* l2cap_chan_add takes its own ref so we can drop this one */
	hci_conn_drop(hcon);

	l2cap_state_change(chan, BT_CONNECT);
	__set_chan_timer(chan, chan->ops->get_sndtimeo(chan));

	/* Release chan->sport so that it can be reused by other
	 * sockets (as it's only used for listening sockets).
	 */
	write_lock(&chan_list_lock);
	chan->sport = 0;
	write_unlock(&chan_list_lock);

	if (hcon->state == BT_CONNECTED) {
		if (chan->chan_type != L2CAP_CHAN_CONN_ORIENTED) {
			__clear_chan_timer(chan);
			if (l2cap_chan_check_security(chan, true))
				l2cap_state_change(chan, BT_CONNECTED);
		} else
			l2cap_do_start(chan);
	}

	err = 0;

chan_unlock:
	l2cap_chan_unlock(chan);
	mutex_unlock(&conn->chan_lock);
done:
	hci_dev_unlock(hdev);
	hci_dev_put(hdev);
	return err;
}
EXPORT_SYMBOL_GPL(l2cap_chan_connect);

static void l2cap_ecred_reconfigure(struct l2cap_chan *chan)
{
	struct l2cap_conn *conn = chan->conn;
	struct {
		struct l2cap_ecred_reconf_req req;
		__le16 scid;
	} pdu;

	pdu.req.mtu = cpu_to_le16(chan->imtu);
	pdu.req.mps = cpu_to_le16(chan->mps);
	pdu.scid    = cpu_to_le16(chan->scid);

	chan->ident = l2cap_get_ident(conn);

	l2cap_send_cmd(conn, chan->ident, L2CAP_ECRED_RECONF_REQ,
		       sizeof(pdu), &pdu);
}

int l2cap_chan_reconfigure(struct l2cap_chan *chan, __u16 mtu)
{
	if (chan->imtu > mtu)
		return -EINVAL;

	BT_DBG("chan %p mtu 0x%4.4x", chan, mtu);

	chan->imtu = mtu;

	l2cap_ecred_reconfigure(chan);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static int l2cap_disconn_ind(struct hci_conn *hcon)
=======
/* ---- L2CAP interface with lower layer (HCI) ---- */

int l2cap_connect_ind(struct hci_dev *hdev, bdaddr_t *bdaddr)
{
	int exact = 0, lm1 = 0, lm2 = 0;
	struct l2cap_chan *c;

	BT_DBG("hdev %s, bdaddr %pMR", hdev->name, bdaddr);

	/* Find listening sockets and check their link_mode */
	read_lock(&chan_list_lock);
	list_for_each_entry(c, &chan_list, global_l) {
		if (c->state != BT_LISTEN)
			continue;

		if (!bacmp(&c->src, &hdev->bdaddr)) {
			lm1 |= HCI_LM_ACCEPT;
			if (test_bit(FLAG_ROLE_SWITCH, &c->flags))
				lm1 |= HCI_LM_MASTER;
			exact++;
		} else if (!bacmp(&c->src, BDADDR_ANY)) {
			lm2 |= HCI_LM_ACCEPT;
			if (test_bit(FLAG_ROLE_SWITCH, &c->flags))
				lm2 |= HCI_LM_MASTER;
		}
	}
	read_unlock(&chan_list_lock);

	return exact ? lm1 : lm2;
}

/* Find the next fixed channel in BT_LISTEN state, continue iteration
 * from an existing channel in the list or from the beginning of the
 * global list (by passing NULL as first parameter).
 */
static struct l2cap_chan *l2cap_global_fixed_chan(struct l2cap_chan *c,
						  struct hci_conn *hcon)
{
	u8 src_type = bdaddr_src_type(hcon);

	read_lock(&chan_list_lock);

	if (c)
		c = list_next_entry(c, global_l);
	else
		c = list_entry(chan_list.next, typeof(*c), global_l);

	list_for_each_entry_from(c, &chan_list, global_l) {
		if (c->chan_type != L2CAP_CHAN_FIXED)
			continue;
		if (c->state != BT_LISTEN)
			continue;
		if (bacmp(&c->src, &hcon->src) && bacmp(&c->src, BDADDR_ANY))
			continue;
		if (src_type != c->src_type)
			continue;

		c = l2cap_chan_hold_unless_zero(c);
		read_unlock(&chan_list_lock);
		return c;
	}

	read_unlock(&chan_list_lock);

	return NULL;
}

static void l2cap_connect_cfm(struct hci_conn *hcon, u8 status)
{
	struct hci_dev *hdev = hcon->hdev;
	struct l2cap_conn *conn;
	struct l2cap_chan *pchan;
	u8 dst_type;

	if (hcon->type != ACL_LINK && hcon->type != LE_LINK)
		return;

	BT_DBG("hcon %p bdaddr %pMR status %d", hcon, &hcon->dst, status);

	if (status) {
		l2cap_conn_del(hcon, bt_to_errno(status));
		return;
	}

	conn = l2cap_conn_add(hcon);
	if (!conn)
		return;

	dst_type = bdaddr_dst_type(hcon);

	/* If device is blocked, do not create channels for it */
	if (hci_bdaddr_list_lookup(&hdev->reject_list, &hcon->dst, dst_type))
		return;

	/* Find fixed channels and notify them of the new connection. We
	 * use multiple individual lookups, continuing each time where
	 * we left off, because the list lock would prevent calling the
	 * potentially sleeping l2cap_chan_lock() function.
	 */
	pchan = l2cap_global_fixed_chan(NULL, hcon);
	while (pchan) {
		struct l2cap_chan *chan, *next;

		/* Client fixed channels should override server ones */
		if (__l2cap_get_chan_by_dcid(conn, pchan->scid))
			goto next;

		l2cap_chan_lock(pchan);
		chan = pchan->ops->new_connection(pchan);
		if (chan) {
			bacpy(&chan->src, &hcon->src);
			bacpy(&chan->dst, &hcon->dst);
			chan->src_type = bdaddr_src_type(hcon);
			chan->dst_type = dst_type;

			__l2cap_chan_add(conn, chan);
		}

		l2cap_chan_unlock(pchan);
next:
		next = l2cap_global_fixed_chan(pchan, hcon);
		l2cap_chan_put(pchan);
		pchan = next;
	}

	l2cap_conn_ready(conn);
}

int l2cap_disconn_ind(struct hci_conn *hcon)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct l2cap_conn *conn = hcon->l2cap_data;

	BT_DBG("hcon %p", hcon);

<<<<<<< HEAD
	if (hcon->type != ACL_LINK || !conn)
		return 0x13;

	return conn->disc_reason;
}

static int l2cap_disconn_cfm(struct hci_conn *hcon, u8 reason, u8 is_process)
{
	BT_DBG("hcon %p reason %d", hcon, reason);

	if (!(hcon->type == ACL_LINK || hcon->type == LE_LINK))
		return -EINVAL;

	l2cap_conn_del(hcon, bt_err(reason), is_process);

	return 0;
}

static inline void l2cap_check_encryption(struct sock *sk, u8 encrypt)
{
	if (sk->sk_type != SOCK_SEQPACKET && sk->sk_type != SOCK_STREAM)
		return;

	if (encrypt == 0x00) {
		if (l2cap_pi(sk)->sec_level == BT_SECURITY_MEDIUM) {
			l2cap_sock_clear_timer(sk);
			l2cap_sock_set_timer(sk, HZ * 5);
		} else if (l2cap_pi(sk)->sec_level == BT_SECURITY_HIGH ||
			l2cap_pi(sk)->sec_level == BT_SECURITY_VERY_HIGH)
			__l2cap_sock_close(sk, ECONNREFUSED);
	} else {
		if (l2cap_pi(sk)->sec_level == BT_SECURITY_MEDIUM)
			l2cap_sock_clear_timer(sk);
	}
}

static int l2cap_security_cfm(struct hci_conn *hcon, u8 status, u8 encrypt)
{
	struct l2cap_chan_list *l;
	struct l2cap_conn *conn = hcon->l2cap_data;
	struct sock *sk;
	int smp = 0;

	if (!conn)
		return 0;

	l = &conn->chan_list;

	BT_DBG("conn %p", conn);

	read_lock(&l->lock);

	for (sk = l->head; sk; sk = l2cap_pi(sk)->next_c) {
		bh_lock_sock(sk);

		BT_DBG("sk->scid %d", l2cap_pi(sk)->scid);

		if (l2cap_pi(sk)->scid == L2CAP_CID_LE_DATA) {
			if (!status && encrypt) {
				l2cap_pi(sk)->sec_level = hcon->sec_level;
				l2cap_chan_ready(sk);
			}

			smp = 1;
			bh_unlock_sock(sk);
			continue;
		}

		if (l2cap_pi(sk)->conf_state & L2CAP_CONF_CONNECT_PEND) {
			bh_unlock_sock(sk);
			continue;
		}

		if (!status && (sk->sk_state == BT_CONNECTED ||
						sk->sk_state == BT_CONFIG)) {
			l2cap_check_encryption(sk, encrypt);
			bh_unlock_sock(sk);
			continue;
		}

		if (sk->sk_state == BT_CONNECT) {
			if (!status) {
				l2cap_pi(sk)->conf_state |=
						L2CAP_CONF_CONNECT_PEND;
				if ((l2cap_pi(sk)->amp_pref ==
						BT_AMP_POLICY_PREFER_AMP) &&
						enable_hs) {
					amp_create_physical(l2cap_pi(sk)->conn,
								sk);
				} else
					l2cap_send_conn_req(sk);
			} else {
				l2cap_sock_clear_timer(sk);
				l2cap_sock_set_timer(sk, HZ / 10);
			}
		} else if (sk->sk_state == BT_CONNECT2) {
			struct l2cap_conn_rsp rsp;
			__u16 result;

			if (!status) {
				if (l2cap_pi(sk)->amp_id) {
					amp_accept_physical(conn,
						l2cap_pi(sk)->amp_id, sk);
					bh_unlock_sock(sk);
					continue;
				}

				sk->sk_state = BT_CONFIG;
				result = L2CAP_CR_SUCCESS;
			} else {
				sk->sk_state = BT_DISCONN;
				l2cap_sock_set_timer(sk, HZ / 10);
				result = L2CAP_CR_SEC_BLOCK;
			}

			rsp.scid   = cpu_to_le16(l2cap_pi(sk)->dcid);
			rsp.dcid   = cpu_to_le16(l2cap_pi(sk)->scid);
			rsp.result = cpu_to_le16(result);
			rsp.status = cpu_to_le16(L2CAP_CS_NO_INFO);
			l2cap_send_cmd(conn, l2cap_pi(sk)->ident,
					L2CAP_CONN_RSP, sizeof(rsp), &rsp);

			if (!(l2cap_pi(sk)->conf_state & L2CAP_CONF_REQ_SENT) &&
				result == L2CAP_CR_SUCCESS) {
				char buf[128];
				l2cap_pi(sk)->conf_state |= L2CAP_CONF_REQ_SENT;
				l2cap_send_cmd(conn, l2cap_get_ident(conn),
					       L2CAP_CONF_REQ,
					       l2cap_build_conf_req(sk, buf, sizeof(buf)),
					       buf);
				l2cap_pi(sk)->num_conf_req++;
			}
		}

		bh_unlock_sock(sk);
	}

	read_unlock(&l->lock);

	if (smp) {
		del_timer(&hcon->smp_timer);
		smp_link_encrypt_cmplt(conn, status, encrypt);
	}

	return 0;
}

static int l2cap_recv_acldata(struct hci_conn *hcon, struct sk_buff *skb, u16 flags)
{
	struct l2cap_conn *conn = hcon->l2cap_data;

	if (!conn && hcon->hdev->dev_type != HCI_BREDR)
		goto drop;

	if (!conn)
		conn = l2cap_conn_add(hcon, 0);
=======
	if (!conn)
		return HCI_ERROR_REMOTE_USER_TERM;
	return conn->disc_reason;
}

static void l2cap_disconn_cfm(struct hci_conn *hcon, u8 reason)
{
	if (hcon->type != ACL_LINK && hcon->type != LE_LINK)
		return;

	BT_DBG("hcon %p reason %d", hcon, reason);

	l2cap_conn_del(hcon, bt_to_errno(reason));
}

static inline void l2cap_check_encryption(struct l2cap_chan *chan, u8 encrypt)
{
	if (chan->chan_type != L2CAP_CHAN_CONN_ORIENTED)
		return;

	if (encrypt == 0x00) {
		if (chan->sec_level == BT_SECURITY_MEDIUM) {
			__set_chan_timer(chan, L2CAP_ENC_TIMEOUT);
		} else if (chan->sec_level == BT_SECURITY_HIGH ||
			   chan->sec_level == BT_SECURITY_FIPS)
			l2cap_chan_close(chan, ECONNREFUSED);
	} else {
		if (chan->sec_level == BT_SECURITY_MEDIUM)
			__clear_chan_timer(chan);
	}
}

static void l2cap_security_cfm(struct hci_conn *hcon, u8 status, u8 encrypt)
{
	struct l2cap_conn *conn = hcon->l2cap_data;
	struct l2cap_chan *chan;

	if (!conn)
		return;

	BT_DBG("conn %p status 0x%2.2x encrypt %u", conn, status, encrypt);

	mutex_lock(&conn->chan_lock);

	list_for_each_entry(chan, &conn->chan_l, list) {
		l2cap_chan_lock(chan);

		BT_DBG("chan %p scid 0x%4.4x state %s", chan, chan->scid,
		       state_to_string(chan->state));

		if (!status && encrypt)
			chan->sec_level = hcon->sec_level;

		if (!__l2cap_no_conn_pending(chan)) {
			l2cap_chan_unlock(chan);
			continue;
		}

		if (!status && (chan->state == BT_CONNECTED ||
				chan->state == BT_CONFIG)) {
			chan->ops->resume(chan);
			l2cap_check_encryption(chan, encrypt);
			l2cap_chan_unlock(chan);
			continue;
		}

		if (chan->state == BT_CONNECT) {
			if (!status && l2cap_check_enc_key_size(hcon))
				l2cap_start_connection(chan);
			else
				__set_chan_timer(chan, L2CAP_DISC_TIMEOUT);
		} else if (chan->state == BT_CONNECT2 &&
			   !(chan->mode == L2CAP_MODE_EXT_FLOWCTL ||
			     chan->mode == L2CAP_MODE_LE_FLOWCTL)) {
			struct l2cap_conn_rsp rsp;
			__u16 res, stat;

			if (!status && l2cap_check_enc_key_size(hcon)) {
				if (test_bit(FLAG_DEFER_SETUP, &chan->flags)) {
					res = L2CAP_CR_PEND;
					stat = L2CAP_CS_AUTHOR_PEND;
					chan->ops->defer(chan);
				} else {
					l2cap_state_change(chan, BT_CONFIG);
					res = L2CAP_CR_SUCCESS;
					stat = L2CAP_CS_NO_INFO;
				}
			} else {
				l2cap_state_change(chan, BT_DISCONN);
				__set_chan_timer(chan, L2CAP_DISC_TIMEOUT);
				res = L2CAP_CR_SEC_BLOCK;
				stat = L2CAP_CS_NO_INFO;
			}

			rsp.scid   = cpu_to_le16(chan->dcid);
			rsp.dcid   = cpu_to_le16(chan->scid);
			rsp.result = cpu_to_le16(res);
			rsp.status = cpu_to_le16(stat);
			l2cap_send_cmd(conn, chan->ident, L2CAP_CONN_RSP,
				       sizeof(rsp), &rsp);

			if (!test_bit(CONF_REQ_SENT, &chan->conf_state) &&
			    res == L2CAP_CR_SUCCESS) {
				char buf[128];
				set_bit(CONF_REQ_SENT, &chan->conf_state);
				l2cap_send_cmd(conn, l2cap_get_ident(conn),
					       L2CAP_CONF_REQ,
					       l2cap_build_conf_req(chan, buf, sizeof(buf)),
					       buf);
				chan->num_conf_req++;
			}
		}

		l2cap_chan_unlock(chan);
	}

	mutex_unlock(&conn->chan_lock);
}

/* Append fragment into frame respecting the maximum len of rx_skb */
static int l2cap_recv_frag(struct l2cap_conn *conn, struct sk_buff *skb,
			   u16 len)
{
	if (!conn->rx_skb) {
		/* Allocate skb for the complete frame (with header) */
		conn->rx_skb = bt_skb_alloc(len, GFP_KERNEL);
		if (!conn->rx_skb)
			return -ENOMEM;
		/* Init rx_len */
		conn->rx_len = len;
	}

	/* Copy as much as the rx_skb can hold */
	len = min_t(u16, len, skb->len);
	skb_copy_from_linear_data(skb, skb_put(conn->rx_skb, len), len);
	skb_pull(skb, len);
	conn->rx_len -= len;

	return len;
}

static int l2cap_recv_len(struct l2cap_conn *conn, struct sk_buff *skb)
{
	struct sk_buff *rx_skb;
	int len;

	/* Append just enough to complete the header */
	len = l2cap_recv_frag(conn, skb, L2CAP_LEN_SIZE - conn->rx_skb->len);

	/* If header could not be read just continue */
	if (len < 0 || conn->rx_skb->len < L2CAP_LEN_SIZE)
		return len;

	rx_skb = conn->rx_skb;
	len = get_unaligned_le16(rx_skb->data);

	/* Check if rx_skb has enough space to received all fragments */
	if (len + (L2CAP_HDR_SIZE - L2CAP_LEN_SIZE) <= skb_tailroom(rx_skb)) {
		/* Update expected len */
		conn->rx_len = len + (L2CAP_HDR_SIZE - L2CAP_LEN_SIZE);
		return L2CAP_LEN_SIZE;
	}

	/* Reset conn->rx_skb since it will need to be reallocated in order to
	 * fit all fragments.
	 */
	conn->rx_skb = NULL;

	/* Reallocates rx_skb using the exact expected length */
	len = l2cap_recv_frag(conn, rx_skb,
			      len + (L2CAP_HDR_SIZE - L2CAP_LEN_SIZE));
	kfree_skb(rx_skb);

	return len;
}

static void l2cap_recv_reset(struct l2cap_conn *conn)
{
	kfree_skb(conn->rx_skb);
	conn->rx_skb = NULL;
	conn->rx_len = 0;
}

void l2cap_recv_acldata(struct hci_conn *hcon, struct sk_buff *skb, u16 flags)
{
	struct l2cap_conn *conn = hcon->l2cap_data;
	int len;

	/* For AMP controller do not create l2cap conn */
	if (!conn && hcon->hdev->dev_type != HCI_PRIMARY)
		goto drop;

	if (!conn)
		conn = l2cap_conn_add(hcon);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!conn)
		goto drop;

<<<<<<< HEAD
	BT_DBG("conn %p len %d flags 0x%x", conn, skb->len, flags);

	if (flags & ACL_START) {
		struct l2cap_hdr *hdr;
		int len;

		if (conn->rx_len) {
			BT_ERR("Unexpected start frame (len %d)", skb->len);
			kfree_skb(conn->rx_skb);
			conn->rx_skb = NULL;
			conn->rx_len = 0;
			l2cap_conn_unreliable(conn, ECOMM);
		}

		/* Start fragment always begin with Basic L2CAP header */
		if (skb->len < L2CAP_HDR_SIZE) {
			BT_ERR("Frame is too short (len %d)", skb->len);
			l2cap_conn_unreliable(conn, ECOMM);
			goto drop;
		}

		hdr = (struct l2cap_hdr *) skb->data;
		len = __le16_to_cpu(hdr->len) + L2CAP_HDR_SIZE;
=======
	BT_DBG("conn %p len %u flags 0x%x", conn, skb->len, flags);

	switch (flags) {
	case ACL_START:
	case ACL_START_NO_FLUSH:
	case ACL_COMPLETE:
		if (conn->rx_skb) {
			BT_ERR("Unexpected start frame (len %d)", skb->len);
			l2cap_recv_reset(conn);
			l2cap_conn_unreliable(conn, ECOMM);
		}

		/* Start fragment may not contain the L2CAP length so just
		 * copy the initial byte when that happens and use conn->mtu as
		 * expected length.
		 */
		if (skb->len < L2CAP_LEN_SIZE) {
			l2cap_recv_frag(conn, skb, conn->mtu);
			break;
		}

		len = get_unaligned_le16(skb->data) + L2CAP_HDR_SIZE;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (len == skb->len) {
			/* Complete frame received */
			l2cap_recv_frame(conn, skb);
<<<<<<< HEAD
			return 0;
		}

		if (flags & ACL_CONT) {
			BT_ERR("Complete frame is incomplete "
				"(len %d, expected len %d)",
				skb->len, len);
			l2cap_conn_unreliable(conn, ECOMM);
			goto drop;
		}

		BT_DBG("Start: total len %d, frag len %d", len, skb->len);

		if (skb->len > len) {
			BT_ERR("Frame is too long (len %d, expected len %d)",
				skb->len, len);
=======
			return;
		}

		BT_DBG("Start: total len %d, frag len %u", len, skb->len);

		if (skb->len > len) {
			BT_ERR("Frame is too long (len %u, expected len %d)",
			       skb->len, len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			l2cap_conn_unreliable(conn, ECOMM);
			goto drop;
		}

<<<<<<< HEAD
		/* Allocate skb for the complete frame (with header) */
		conn->rx_skb = bt_skb_alloc(len, GFP_ATOMIC);
		if (!conn->rx_skb)
			goto drop;

		skb_copy_from_linear_data(skb, skb_put(conn->rx_skb, skb->len),
								skb->len);
		conn->rx_len = len - skb->len;
	} else {
		BT_DBG("Cont: frag len %d (expecting %d)", skb->len, conn->rx_len);

		if (!conn->rx_len) {
=======
		/* Append fragment into frame (with header) */
		if (l2cap_recv_frag(conn, skb, len) < 0)
			goto drop;

		break;

	case ACL_CONT:
		BT_DBG("Cont: frag len %u (expecting %u)", skb->len, conn->rx_len);

		if (!conn->rx_skb) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			BT_ERR("Unexpected continuation frame (len %d)", skb->len);
			l2cap_conn_unreliable(conn, ECOMM);
			goto drop;
		}

<<<<<<< HEAD
		if (skb->len > conn->rx_len) {
			BT_ERR("Fragment is too long (len %d, expected %d)",
					skb->len, conn->rx_len);
			kfree_skb(conn->rx_skb);
			conn->rx_skb = NULL;
			conn->rx_len = 0;
=======
		/* Complete the L2CAP length if it has not been read */
		if (conn->rx_skb->len < L2CAP_LEN_SIZE) {
			if (l2cap_recv_len(conn, skb) < 0) {
				l2cap_conn_unreliable(conn, ECOMM);
				goto drop;
			}

			/* Header still could not be read just continue */
			if (conn->rx_skb->len < L2CAP_LEN_SIZE)
				break;
		}

		if (skb->len > conn->rx_len) {
			BT_ERR("Fragment is too long (len %u, expected %u)",
			       skb->len, conn->rx_len);
			l2cap_recv_reset(conn);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			l2cap_conn_unreliable(conn, ECOMM);
			goto drop;
		}

<<<<<<< HEAD
		skb_copy_from_linear_data(skb, skb_put(conn->rx_skb, skb->len),
								skb->len);
		conn->rx_len -= skb->len;

		if (!conn->rx_len) {
			/* Complete frame received */
			l2cap_recv_frame(conn, conn->rx_skb);
			conn->rx_skb = NULL;
		}
=======
		/* Append fragment into frame (with header) */
		l2cap_recv_frag(conn, skb, skb->len);

		if (!conn->rx_len) {
			/* Complete frame received. l2cap_recv_frame
			 * takes ownership of the skb so set the global
			 * rx_skb pointer to NULL first.
			 */
			struct sk_buff *rx_skb = conn->rx_skb;
			conn->rx_skb = NULL;
			l2cap_recv_frame(conn, rx_skb);
		}
		break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

drop:
	kfree_skb(skb);
<<<<<<< HEAD
	return 0;
}

static void l2cap_set_acl_flushto(struct hci_conn *hcon, u16 flush_to)
{
	struct hci_cp_write_automatic_flush_timeout flush_tm;
	if (hcon && hcon->hdev) {
		flush_tm.handle = hcon->handle;
		if (flush_to == L2CAP_DEFAULT_FLUSH_TO)
			flush_to = 0;
		flush_tm.timeout = (flush_to < L2CAP_MAX_FLUSH_TO) ?
				flush_to : L2CAP_MAX_FLUSH_TO;
		hci_send_cmd(hcon->hdev,
			HCI_OP_WRITE_AUTOMATIC_FLUSH_TIMEOUT,
			4, &(flush_tm));
	}
}

static u16 l2cap_get_smallest_flushto(struct l2cap_chan_list *l)
{
	int ret_flush_to = L2CAP_DEFAULT_FLUSH_TO;
	struct sock *s;
	for (s = l->head; s; s = l2cap_pi(s)->next_c) {
		if (l2cap_pi(s)->flush_to > 0 &&
				l2cap_pi(s)->flush_to < ret_flush_to)
			ret_flush_to = l2cap_pi(s)->flush_to;
	}
	return ret_flush_to;
}

static int l2cap_debugfs_show(struct seq_file *f, void *p)
{
	struct sock *sk;
	struct hlist_node *node;

	read_lock_bh(&l2cap_sk_list.lock);

	sk_for_each(sk, node, &l2cap_sk_list.head) {
		struct l2cap_pinfo *pi = l2cap_pi(sk);

		seq_printf(f, "%s %s %d %d 0x%4.4x 0x%4.4x %d %d %d %d\n",
					batostr(&bt_sk(sk)->src),
					batostr(&bt_sk(sk)->dst),
					sk->sk_state, __le16_to_cpu(pi->psm),
					pi->scid, pi->dcid,
					pi->imtu, pi->omtu, pi->sec_level,
					pi->mode);
	}

	read_unlock_bh(&l2cap_sk_list.lock);
=======
}

static struct hci_cb l2cap_cb = {
	.name		= "L2CAP",
	.connect_cfm	= l2cap_connect_cfm,
	.disconn_cfm	= l2cap_disconn_cfm,
	.security_cfm	= l2cap_security_cfm,
};

static int l2cap_debugfs_show(struct seq_file *f, void *p)
{
	struct l2cap_chan *c;

	read_lock(&chan_list_lock);

	list_for_each_entry(c, &chan_list, global_l) {
		seq_printf(f, "%pMR (%u) %pMR (%u) %d %d 0x%4.4x 0x%4.4x %d %d %d %d\n",
			   &c->src, c->src_type, &c->dst, c->dst_type,
			   c->state, __le16_to_cpu(c->psm),
			   c->scid, c->dcid, c->imtu, c->omtu,
			   c->sec_level, c->mode);
	}

	read_unlock(&chan_list_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static void l2cap_queue_acl_data(struct work_struct *worker)
{
	struct sock *sk = NULL;
	int attempts = 0;
	struct sk_buff *skb_rsp;
	struct l2cap_hdr *lh;
	u8 err_rsp[] = {L2CAP_ATT_ERROR, 0x00, 0x00, 0x00,
						L2CAP_ATT_NOT_SUPPORTED};

	for (attempts = 0; attempts < 40; attempts++) {
		msleep(50);
		if (!att_chn_params.conn) {
			BT_DBG("att_chn_params.conn is NULL");
			return;
		}
		sk = l2cap_find_sock_by_fixed_cid_and_dir
				(att_chn_params.cid,
				att_chn_params.conn->src,
				att_chn_params.conn->dst,
				att_chn_params.dir);
		bh_lock_sock(sk);
		if (sk->sk_state == BT_CONNECTED) {
			sock_queue_rcv_skb(sk, att_chn_params.skb);
			if (sk)
				bh_unlock_sock(sk);
			return;
		}
		bh_unlock_sock(sk);
	}
	bh_lock_sock(sk);

	if (att_chn_params.skb->data[0] != L2CAP_ATT_INDICATE)
		goto not_indicate;

	/* If this is an incoming Indication, we are required to confirm */
	skb_rsp = bt_skb_alloc(sizeof(u8) + L2CAP_HDR_SIZE, GFP_ATOMIC);
	if (!skb_rsp)
		goto free_skb;

	lh = (struct l2cap_hdr *) skb_put(skb_rsp, L2CAP_HDR_SIZE);
	lh->len = cpu_to_le16(sizeof(u8));
	lh->cid = cpu_to_le16(L2CAP_CID_LE_DATA);
	err_rsp[0] = L2CAP_ATT_CONFIRM;
	memcpy(skb_put(skb_rsp, sizeof(u8)), err_rsp, sizeof(u8));
	hci_send_acl(att_chn_params.conn->hcon, NULL, skb_rsp, 0);
	goto free_skb;

not_indicate:
	if (att_chn_params.skb->data[0] & L2CAP_ATT_RESPONSE_BIT ||
			att_chn_params.skb->data[0] == L2CAP_ATT_CONFIRM)
		goto free_skb;

	/* If this is an incoming PDU that requires a response, respond with
	 * a generic error so remote device doesn't hang */

	skb_rsp = bt_skb_alloc(sizeof(err_rsp) + L2CAP_HDR_SIZE, GFP_ATOMIC);
	if (!skb_rsp)
		goto free_skb;

	lh = (struct l2cap_hdr *) skb_put(skb_rsp, L2CAP_HDR_SIZE);
	lh->len = cpu_to_le16(sizeof(err_rsp));
	lh->cid = cpu_to_le16(L2CAP_CID_LE_DATA);
	err_rsp[1] = att_chn_params.skb->data[0];
	memcpy(skb_put(skb_rsp, sizeof(err_rsp)), err_rsp, sizeof(err_rsp));
	hci_send_acl(att_chn_params.conn->hcon, NULL, skb_rsp, 0);

free_skb:
	kfree_skb(att_chn_params.skb);

	if (sk)
		bh_unlock_sock(sk);

}

static int l2cap_debugfs_open(struct inode *inode, struct file *file)
{
	return single_open(file, l2cap_debugfs_show, inode->i_private);
}

static const struct file_operations l2cap_debugfs_fops = {
	.open		= l2cap_debugfs_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static struct dentry *l2cap_debugfs;

static struct hci_proto l2cap_hci_proto = {
	.name		= "L2CAP",
	.id		= HCI_PROTO_L2CAP,
	.connect_ind	= l2cap_connect_ind,
	.connect_cfm	= l2cap_connect_cfm,
	.disconn_ind	= l2cap_disconn_ind,
	.disconn_cfm	= l2cap_disconn_cfm,
	.security_cfm	= l2cap_security_cfm,
	.recv_acldata	= l2cap_recv_acldata,
	.create_cfm	= l2cap_create_cfm,
	.modify_cfm	= l2cap_modify_cfm,
	.destroy_cfm	= l2cap_destroy_cfm,
};

=======
DEFINE_SHOW_ATTRIBUTE(l2cap_debugfs);

static struct dentry *l2cap_debugfs;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int __init l2cap_init(void)
{
	int err;

	err = l2cap_init_sockets();
	if (err < 0)
		return err;

<<<<<<< HEAD
	_l2cap_wq = create_singlethread_workqueue("l2cap");
	if (!_l2cap_wq) {
		err = -ENOMEM;
		goto error;
	}

	err = hci_register_proto(&l2cap_hci_proto);
	if (err < 0) {
		BT_ERR("L2CAP protocol registration failed");
		bt_sock_unregister(BTPROTO_L2CAP);
		goto error;
	}

	if (bt_debugfs) {
		l2cap_debugfs = debugfs_create_file("l2cap", 0444,
					bt_debugfs, NULL, &l2cap_debugfs_fops);
		if (!l2cap_debugfs)
			BT_ERR("Failed to create L2CAP debug file");
	}

	if (amp_init() < 0) {
		BT_ERR("AMP Manager initialization failed");
		goto error;
	}

	return 0;

error:
	destroy_workqueue(_l2cap_wq);
	l2cap_cleanup_sockets();
	return err;
=======
	hci_register_cb(&l2cap_cb);

	if (IS_ERR_OR_NULL(bt_debugfs))
		return 0;

	l2cap_debugfs = debugfs_create_file("l2cap", 0444, bt_debugfs,
					    NULL, &l2cap_debugfs_fops);

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void l2cap_exit(void)
{
<<<<<<< HEAD
	amp_exit();

	debugfs_remove(l2cap_debugfs);

	flush_workqueue(_l2cap_wq);
	destroy_workqueue(_l2cap_wq);

	if (hci_unregister_proto(&l2cap_hci_proto) < 0)
		BT_ERR("L2CAP protocol unregistration failed");

=======
	debugfs_remove(l2cap_debugfs);
	hci_unregister_cb(&l2cap_cb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	l2cap_cleanup_sockets();
}

module_param(disable_ertm, bool, 0644);
MODULE_PARM_DESC(disable_ertm, "Disable enhanced retransmission mode");

<<<<<<< HEAD
module_param(enable_hs, bool, 0644);
MODULE_PARM_DESC(enable_hs, "Enable A2MP protocol");

module_param(enable_reconfig, bool, 0644);
MODULE_PARM_DESC(enable_reconfig, "Enable reconfig after initiating AMP move");
=======
module_param(enable_ecred, bool, 0644);
MODULE_PARM_DESC(enable_ecred, "Enable enhanced credit flow control mode");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
