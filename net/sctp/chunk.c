<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* SCTP kernel implementation
 * (C) Copyright IBM Corp. 2003, 2004
 *
 * This file is part of the SCTP kernel implementation
 *
 * This file contains the code relating the chunk abstraction.
 *
<<<<<<< HEAD
 * This SCTP implementation is free software;
 * you can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This SCTP implementation is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 *                 ************************
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU CC; see the file COPYING.  If not, write to
 * the Free Software Foundation, 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * Please send any bug reports or fixes you make to the
 * email address(es):
 *    lksctp developers <lksctp-developers@lists.sourceforge.net>
 *
 * Or submit a bug report through the following website:
 *    http://www.sf.net/projects/lksctp
=======
 * Please send any bug reports or fixes you make to the
 * email address(es):
 *    lksctp developers <linux-sctp@vger.kernel.org>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Written or modified by:
 *    Jon Grimm             <jgrimm@us.ibm.com>
 *    Sridhar Samudrala     <sri@us.ibm.com>
<<<<<<< HEAD
 *
 * Any bugs reported given to us we will try to fix... any fixes shared will
 * be incorporated into the next SCTP release.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/net.h>
#include <linux/inet.h>
#include <linux/skbuff.h>
#include <linux/slab.h>
#include <net/sock.h>
#include <net/sctp/sctp.h>
#include <net/sctp/sm.h>

/* This file is mostly in anticipation of future work, but initially
 * populate with fragment tracking for an outbound message.
 */

/* Initialize datamsg from memory. */
static void sctp_datamsg_init(struct sctp_datamsg *msg)
{
<<<<<<< HEAD
	atomic_set(&msg->refcnt, 1);
	msg->send_failed = 0;
	msg->send_error = 0;
	msg->can_abandon = 0;
	msg->can_delay = 1;
=======
	refcount_set(&msg->refcnt, 1);
	msg->send_failed = 0;
	msg->send_error = 0;
	msg->can_delay = 1;
	msg->abandoned = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	msg->expires_at = 0;
	INIT_LIST_HEAD(&msg->chunks);
}

/* Allocate and initialize datamsg. */
<<<<<<< HEAD
SCTP_STATIC struct sctp_datamsg *sctp_datamsg_new(gfp_t gfp)
=======
static struct sctp_datamsg *sctp_datamsg_new(gfp_t gfp)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct sctp_datamsg *msg;
	msg = kmalloc(sizeof(struct sctp_datamsg), gfp);
	if (msg) {
		sctp_datamsg_init(msg);
		SCTP_DBG_OBJCNT_INC(datamsg);
	}
	return msg;
}

void sctp_datamsg_free(struct sctp_datamsg *msg)
{
	struct sctp_chunk *chunk;

	/* This doesn't have to be a _safe vairant because
	 * sctp_chunk_free() only drops the refs.
	 */
	list_for_each_entry(chunk, &msg->chunks, frag_list)
		sctp_chunk_free(chunk);

	sctp_datamsg_put(msg);
}

/* Final destructruction of datamsg memory. */
static void sctp_datamsg_destroy(struct sctp_datamsg *msg)
{
<<<<<<< HEAD
	struct list_head *pos, *temp;
	struct sctp_chunk *chunk;
	struct sctp_sock *sp;
	struct sctp_ulpevent *ev;
	struct sctp_association *asoc = NULL;
	int error = 0, notify;

	/* If we failed, we may need to notify. */
	notify = msg->send_failed ? -1 : 0;
=======
	struct sctp_association *asoc = NULL;
	struct list_head *pos, *temp;
	struct sctp_chunk *chunk;
	struct sctp_ulpevent *ev;
	int error, sent;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Release all references. */
	list_for_each_safe(pos, temp, &msg->chunks) {
		list_del_init(pos);
		chunk = list_entry(pos, struct sctp_chunk, frag_list);
<<<<<<< HEAD
		/* Check whether we _really_ need to notify. */
		if (notify < 0) {
			asoc = chunk->asoc;
			if (msg->send_error)
				error = msg->send_error;
			else
				error = asoc->outqueue.error;

			sp = sctp_sk(asoc->base.sk);
			notify = sctp_ulpevent_type_enabled(SCTP_SEND_FAILED,
							    &sp->subscribe);
		}

		/* Generate a SEND FAILED event only if enabled. */
		if (notify > 0) {
			int sent;
			if (chunk->has_tsn)
				sent = SCTP_DATA_SENT;
			else
				sent = SCTP_DATA_UNSENT;

			ev = sctp_ulpevent_make_send_failed(asoc, chunk, sent,
							    error, GFP_ATOMIC);
			if (ev)
				sctp_ulpq_tail_event(&asoc->ulpq, ev);
=======

		if (!msg->send_failed) {
			sctp_chunk_put(chunk);
			continue;
		}

		asoc = chunk->asoc;
		error = msg->send_error ?: asoc->outqueue.error;
		sent = chunk->has_tsn ? SCTP_DATA_SENT : SCTP_DATA_UNSENT;

		if (sctp_ulpevent_type_enabled(asoc->subscribe,
					       SCTP_SEND_FAILED)) {
			ev = sctp_ulpevent_make_send_failed(asoc, chunk, sent,
							    error, GFP_ATOMIC);
			if (ev)
				asoc->stream.si->enqueue_event(&asoc->ulpq, ev);
		}

		if (sctp_ulpevent_type_enabled(asoc->subscribe,
					       SCTP_SEND_FAILED_EVENT)) {
			ev = sctp_ulpevent_make_send_failed_event(asoc, chunk,
								  sent, error,
								  GFP_ATOMIC);
			if (ev)
				asoc->stream.si->enqueue_event(&asoc->ulpq, ev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}

		sctp_chunk_put(chunk);
	}

	SCTP_DBG_OBJCNT_DEC(datamsg);
	kfree(msg);
}

/* Hold a reference. */
static void sctp_datamsg_hold(struct sctp_datamsg *msg)
{
<<<<<<< HEAD
	atomic_inc(&msg->refcnt);
=======
	refcount_inc(&msg->refcnt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Release a reference. */
void sctp_datamsg_put(struct sctp_datamsg *msg)
{
<<<<<<< HEAD
	if (atomic_dec_and_test(&msg->refcnt))
=======
	if (refcount_dec_and_test(&msg->refcnt))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		sctp_datamsg_destroy(msg);
}

/* Assign a chunk to this datamsg. */
static void sctp_datamsg_assign(struct sctp_datamsg *msg, struct sctp_chunk *chunk)
{
	sctp_datamsg_hold(msg);
	chunk->msg = msg;
}


/* A data chunk can have a maximum payload of (2^16 - 20).  Break
 * down any such message into smaller chunks.  Opportunistically, fragment
 * the chunks down to the current MTU constraints.  We may get refragmented
 * later if the PMTU changes, but it is _much better_ to fragment immediately
 * with a reasonable guess than always doing our fragmentation on the
 * soft-interrupt.
 */
struct sctp_datamsg *sctp_datamsg_from_user(struct sctp_association *asoc,
					    struct sctp_sndrcvinfo *sinfo,
<<<<<<< HEAD
					    struct msghdr *msgh, int msg_len)
{
	int max, whole, i, offset, over, err;
	int len, first_len;
	int max_data;
	struct sctp_chunk *chunk;
	struct sctp_datamsg *msg;
	struct list_head *pos, *temp;
	__u8 frag;
=======
					    struct iov_iter *from)
{
	size_t len, first_len, max_data, remaining;
	size_t msg_len = iov_iter_count(from);
	struct sctp_shared_key *shkey = NULL;
	struct list_head *pos, *temp;
	struct sctp_chunk *chunk;
	struct sctp_datamsg *msg;
	int err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	msg = sctp_datamsg_new(GFP_KERNEL);
	if (!msg)
		return ERR_PTR(-ENOMEM);

	/* Note: Calculate this outside of the loop, so that all fragments
	 * have the same expiration.
	 */
<<<<<<< HEAD
	if (sinfo->sinfo_timetolive) {
		/* sinfo_timetolive is in milliseconds */
		msg->expires_at = jiffies +
				    msecs_to_jiffies(sinfo->sinfo_timetolive);
		msg->can_abandon = 1;
		SCTP_DEBUG_PRINTK("%s: msg:%p expires_at: %ld jiffies:%ld\n",
				  __func__, msg, msg->expires_at, jiffies);
	}
=======
	if (asoc->peer.prsctp_capable && sinfo->sinfo_timetolive &&
	    (SCTP_PR_TTL_ENABLED(sinfo->sinfo_flags) ||
	     !SCTP_PR_POLICY(sinfo->sinfo_flags)))
		msg->expires_at = jiffies +
				  msecs_to_jiffies(sinfo->sinfo_timetolive);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* This is the biggest possible DATA chunk that can fit into
	 * the packet
	 */
<<<<<<< HEAD
	max_data = asoc->pathmtu -
		sctp_sk(asoc->base.sk)->pf->af->net_header_len -
		sizeof(struct sctphdr) - sizeof(struct sctp_data_chunk);

	max = asoc->frag_point;
	/* If the the peer requested that we authenticate DATA chunks
	 * we need to accound for bundling of the AUTH chunks along with
=======
	max_data = asoc->frag_point;
	if (unlikely(!max_data)) {
		max_data = sctp_min_frag_point(sctp_sk(asoc->base.sk),
					       sctp_datachk_len(&asoc->stream));
		pr_warn_ratelimited("%s: asoc:%p frag_point is zero, forcing max_data to default minimum (%zu)",
				    __func__, asoc, max_data);
	}

	/* If the peer requested that we authenticate DATA chunks
	 * we need to account for bundling of the AUTH chunks along with
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * DATA.
	 */
	if (sctp_auth_send_cid(SCTP_CID_DATA, asoc)) {
		struct sctp_hmac *hmac_desc = sctp_auth_asoc_get_hmac(asoc);

		if (hmac_desc)
<<<<<<< HEAD
			max_data -= WORD_ROUND(sizeof(sctp_auth_chunk_t) +
					    hmac_desc->hmac_len);
	}

	/* Now, check if we need to reduce our max */
	if (max > max_data)
		max = max_data;

	whole = 0;
	first_len = max;
=======
			max_data -= SCTP_PAD4(sizeof(struct sctp_auth_chunk) +
					      hmac_desc->hmac_len);

		if (sinfo->sinfo_tsn &&
		    sinfo->sinfo_ssn != asoc->active_key_id) {
			shkey = sctp_auth_get_shkey(asoc, sinfo->sinfo_ssn);
			if (!shkey) {
				err = -EINVAL;
				goto errout;
			}
		} else {
			shkey = asoc->shkey;
		}
	}

	/* Set first_len and then account for possible bundles on first frag */
	first_len = max_data;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Check to see if we have a pending SACK and try to let it be bundled
	 * with this message.  Do this if we don't have any data queued already.
	 * To check that, look at out_qlen and retransmit list.
	 * NOTE: we will not reduce to account for SACK, if the message would
	 * not have been fragmented.
	 */
	if (timer_pending(&asoc->timers[SCTP_EVENT_TIMEOUT_SACK]) &&
	    asoc->outqueue.out_qlen == 0 &&
	    list_empty(&asoc->outqueue.retransmit) &&
<<<<<<< HEAD
	    msg_len > max)
		max_data -= WORD_ROUND(sizeof(sctp_sack_chunk_t));

	/* Encourage Cookie-ECHO bundling. */
	if (asoc->state < SCTP_STATE_COOKIE_ECHOED)
		max_data -= SCTP_ARBITRARY_COOKIE_ECHO_LEN;

	/* Now that we adjusted completely, reset first_len */
	if (first_len > max_data)
		first_len = max_data;

	/* Account for a different sized first fragment */
	if (msg_len >= first_len) {
		msg_len -= first_len;
		whole = 1;
		msg->can_delay = 0;
	}

	/* How many full sized?  How many bytes leftover? */
	whole += msg_len / max;
	over = msg_len % max;
	offset = 0;

	if ((whole > 1) || (whole && over))
		SCTP_INC_STATS_USER(SCTP_MIB_FRAGUSRMSGS);

	/* Create chunks for all the full sized DATA chunks. */
	for (i=0, len=first_len; i < whole; i++) {
		frag = SCTP_DATA_MIDDLE_FRAG;

		if (0 == i)
			frag |= SCTP_DATA_FIRST_FRAG;

		if ((i == (whole - 1)) && !over) {
=======
	    msg_len > max_data)
		first_len -= SCTP_PAD4(sizeof(struct sctp_sack_chunk));

	/* Encourage Cookie-ECHO bundling. */
	if (asoc->state < SCTP_STATE_COOKIE_ECHOED)
		first_len -= SCTP_ARBITRARY_COOKIE_ECHO_LEN;

	/* Account for a different sized first fragment */
	if (msg_len >= first_len) {
		msg->can_delay = 0;
		if (msg_len > first_len)
			SCTP_INC_STATS(asoc->base.net,
				       SCTP_MIB_FRAGUSRMSGS);
	} else {
		/* Which may be the only one... */
		first_len = msg_len;
	}

	/* Create chunks for all DATA chunks. */
	for (remaining = msg_len; remaining; remaining -= len) {
		u8 frag = SCTP_DATA_MIDDLE_FRAG;

		if (remaining == msg_len) {
			/* First frag, which may also be the last */
			frag |= SCTP_DATA_FIRST_FRAG;
			len = first_len;
		} else {
			/* Middle frags */
			len = max_data;
		}

		if (len >= remaining) {
			/* Last frag, which may also be the first */
			len = remaining;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			frag |= SCTP_DATA_LAST_FRAG;

			/* The application requests to set the I-bit of the
			 * last DATA chunk of a user message when providing
			 * the user message to the SCTP implementation.
			 */
			if ((sinfo->sinfo_flags & SCTP_EOF) ||
			    (sinfo->sinfo_flags & SCTP_SACK_IMMEDIATELY))
				frag |= SCTP_DATA_SACK_IMM;
		}

<<<<<<< HEAD
		chunk = sctp_make_datafrag_empty(asoc, sinfo, len, frag, 0);

=======
		chunk = asoc->stream.si->make_datafrag(asoc, sinfo, len, frag,
						       GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!chunk) {
			err = -ENOMEM;
			goto errout;
		}

<<<<<<< HEAD
		err = sctp_user_addto_chunk(chunk, offset, len, msgh->msg_iov);
		if (err < 0)
			goto errout_chunk_free;

		offset += len;

		/* Put the chunk->skb back into the form expected by send.  */
		__skb_pull(chunk->skb, (__u8 *)chunk->chunk_hdr
			   - (__u8 *)chunk->skb->data);

		sctp_datamsg_assign(msg, chunk);
		list_add_tail(&chunk->frag_list, &msg->chunks);

		/* The first chunk, the first chunk was likely short
		 * to allow bundling, so reset to full size.
		 */
		if (0 == i)
			len = max;
	}

	/* .. now the leftover bytes. */
	if (over) {
		if (!whole)
			frag = SCTP_DATA_NOT_FRAG;
		else
			frag = SCTP_DATA_LAST_FRAG;

		if ((sinfo->sinfo_flags & SCTP_EOF) ||
		    (sinfo->sinfo_flags & SCTP_SACK_IMMEDIATELY))
			frag |= SCTP_DATA_SACK_IMM;

		chunk = sctp_make_datafrag_empty(asoc, sinfo, over, frag, 0);

		if (!chunk) {
			err = -ENOMEM;
			goto errout;
		}

		err = sctp_user_addto_chunk(chunk, offset, over,msgh->msg_iov);

		/* Put the chunk->skb back into the form expected by send.  */
		__skb_pull(chunk->skb, (__u8 *)chunk->chunk_hdr
			   - (__u8 *)chunk->skb->data);
		if (err < 0)
			goto errout_chunk_free;
=======
		err = sctp_user_addto_chunk(chunk, len, from);
		if (err < 0)
			goto errout_chunk_free;

		chunk->shkey = shkey;

		/* Put the chunk->skb back into the form expected by send.  */
		__skb_pull(chunk->skb, (__u8 *)chunk->chunk_hdr -
				       chunk->skb->data);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		sctp_datamsg_assign(msg, chunk);
		list_add_tail(&chunk->frag_list, &msg->chunks);
	}

	return msg;

errout_chunk_free:
	sctp_chunk_free(chunk);

errout:
	list_for_each_safe(pos, temp, &msg->chunks) {
		list_del_init(pos);
		chunk = list_entry(pos, struct sctp_chunk, frag_list);
		sctp_chunk_free(chunk);
	}
	sctp_datamsg_put(msg);
<<<<<<< HEAD
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ERR_PTR(err);
}

/* Check whether this message has expired. */
int sctp_chunk_abandoned(struct sctp_chunk *chunk)
{
<<<<<<< HEAD
	struct sctp_datamsg *msg = chunk->msg;

	if (!msg->can_abandon)
		return 0;

	if (time_after(jiffies, msg->expires_at))
		return 1;

=======
	if (!chunk->asoc->peer.prsctp_capable)
		return 0;

	if (chunk->msg->abandoned)
		return 1;

	if (!chunk->has_tsn &&
	    !(chunk->chunk_hdr->flags & SCTP_DATA_FIRST_FRAG))
		return 0;

	if (SCTP_PR_TTL_ENABLED(chunk->sinfo.sinfo_flags) &&
	    time_after(jiffies, chunk->msg->expires_at)) {
		struct sctp_stream_out *streamout =
			SCTP_SO(&chunk->asoc->stream,
				chunk->sinfo.sinfo_stream);

		if (chunk->sent_count) {
			chunk->asoc->abandoned_sent[SCTP_PR_INDEX(TTL)]++;
			streamout->ext->abandoned_sent[SCTP_PR_INDEX(TTL)]++;
		} else {
			chunk->asoc->abandoned_unsent[SCTP_PR_INDEX(TTL)]++;
			streamout->ext->abandoned_unsent[SCTP_PR_INDEX(TTL)]++;
		}
		chunk->msg->abandoned = 1;
		return 1;
	} else if (SCTP_PR_RTX_ENABLED(chunk->sinfo.sinfo_flags) &&
		   chunk->sent_count > chunk->sinfo.sinfo_timetolive) {
		struct sctp_stream_out *streamout =
			SCTP_SO(&chunk->asoc->stream,
				chunk->sinfo.sinfo_stream);

		chunk->asoc->abandoned_sent[SCTP_PR_INDEX(RTX)]++;
		streamout->ext->abandoned_sent[SCTP_PR_INDEX(RTX)]++;
		chunk->msg->abandoned = 1;
		return 1;
	} else if (!SCTP_PR_POLICY(chunk->sinfo.sinfo_flags) &&
		   chunk->msg->expires_at &&
		   time_after(jiffies, chunk->msg->expires_at)) {
		chunk->msg->abandoned = 1;
		return 1;
	}
	/* PRIO policy is processed by sendmsg, not here */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

/* This chunk (and consequently entire message) has failed in its sending. */
void sctp_chunk_fail(struct sctp_chunk *chunk, int error)
{
	chunk->msg->send_failed = 1;
	chunk->msg->send_error = error;
}
