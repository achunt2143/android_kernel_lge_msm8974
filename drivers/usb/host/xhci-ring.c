<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * xHCI host controller driver
 *
 * Copyright (C) 2008 Intel Corp.
 *
 * Author: Sarah Sharp
 * Some code borrowed from the Linux EHCI driver.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

/*
 * Ring initialization rules:
 * 1. Each segment is initialized to zero, except for link TRBs.
 * 2. Ring cycle state = 0.  This represents Producer Cycle State (PCS) or
 *    Consumer Cycle State (CCS), depending on ring function.
 * 3. Enqueue pointer = dequeue pointer = address of first TRB in the segment.
 *
 * Ring behavior rules:
 * 1. A ring is empty if enqueue == dequeue.  This means there will always be at
 *    least one free TRB in the ring.  This is useful if you want to turn that
 *    into a link TRB and expand the ring.
 * 2. When incrementing an enqueue or dequeue pointer, if the next TRB is a
 *    link TRB, then load the pointer with the address in the link TRB.  If the
 *    link TRB had its toggle bit set, you may need to update the ring cycle
 *    state (see cycle bit rules).  You may have to do this multiple times
 *    until you reach a non-link TRB.
 * 3. A ring is full if enqueue++ (for the definition of increment above)
 *    equals the dequeue pointer.
 *
 * Cycle bit rules:
 * 1. When a consumer increments a dequeue pointer and encounters a toggle bit
 *    in a link TRB, it must toggle the ring cycle state.
 * 2. When a producer increments an enqueue pointer and encounters a toggle bit
 *    in a link TRB, it must toggle the ring cycle state.
 *
 * Producer rules:
 * 1. Check if ring is full before you enqueue.
 * 2. Write the ring cycle state to the cycle bit in the TRB you're enqueuing.
 *    Update enqueue pointer between each write (which may update the ring
 *    cycle state).
 * 3. Notify consumer.  If SW is producer, it rings the doorbell for command
 *    and endpoint rings.  If HC is the producer for the event ring,
 *    and it generates an interrupt according to interrupt modulation rules.
 *
 * Consumer rules:
 * 1. Check if TRB belongs to you.  If the cycle bit == your ring cycle state,
 *    the TRB is owned by the consumer.
 * 2. Update dequeue pointer (which may update the ring cycle state) and
 *    continue processing TRBs until you reach a TRB which is not owned by you.
 * 3. Notify the producer.  SW is the consumer for the event ring, and it
 *   updates event ring dequeue pointer.  HC is the consumer for the command and
 *   endpoint rings; it generates events on the event ring for these.
 */

#include <linux/scatterlist.h>
#include <linux/slab.h>
<<<<<<< HEAD
#include "xhci.h"

static int handle_cmd_in_cmd_wait_list(struct xhci_hcd *xhci,
		struct xhci_virt_device *virt_dev,
		struct xhci_event_cmd *event);
=======
#include <linux/dma-mapping.h>
#include "xhci.h"
#include "xhci-trace.h"

static int queue_command(struct xhci_hcd *xhci, struct xhci_command *cmd,
			 u32 field1, u32 field2,
			 u32 field3, u32 field4, bool command_must_succeed);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Returns zero if the TRB isn't in this segment, otherwise it returns the DMA
 * address of the TRB.
 */
dma_addr_t xhci_trb_virt_to_dma(struct xhci_segment *seg,
		union xhci_trb *trb)
{
	unsigned long segment_offset;

	if (!seg || !trb || trb < seg->trbs)
		return 0;
	/* offset in TRBs */
	segment_offset = trb - seg->trbs;
	if (segment_offset >= TRBS_PER_SEGMENT)
		return 0;
	return seg->dma + (segment_offset * sizeof(*trb));
}

<<<<<<< HEAD
/* Does this link TRB point to the first segment in a ring,
 * or was the previous TRB the last TRB on the last segment in the ERST?
 */
static bool last_trb_on_last_seg(struct xhci_hcd *xhci, struct xhci_ring *ring,
		struct xhci_segment *seg, union xhci_trb *trb)
{
	if (ring == xhci->event_ring)
		return (trb == &seg->trbs[TRBS_PER_SEGMENT]) &&
			(seg->next == xhci->event_ring->first_seg);
	else
		return le32_to_cpu(trb->link.control) & LINK_TOGGLE;
}

/* Is this TRB a link TRB or was the last TRB the last TRB in this event ring
 * segment?  I.e. would the updated event TRB pointer step off the end of the
 * event seg?
 */
static int last_trb(struct xhci_hcd *xhci, struct xhci_ring *ring,
		struct xhci_segment *seg, union xhci_trb *trb)
{
	if (ring == xhci->event_ring)
		return trb == &seg->trbs[TRBS_PER_SEGMENT];
	else
		return TRB_TYPE_LINK_LE32(trb->link.control);
}

static int enqueue_is_link_trb(struct xhci_ring *ring)
{
	struct xhci_link_trb *link = &ring->enqueue->link;
	return TRB_TYPE_LINK_LE32(link->control);
}

union xhci_trb *xhci_find_next_enqueue(struct xhci_ring *ring)
{
	/* Enqueue pointer can be left pointing to the link TRB,
	 * we must handle that
	 */
	if (TRB_TYPE_LINK_LE32(ring->enqueue->link.control))
		return ring->enq_seg->next->trbs;
	return ring->enqueue;
=======
static bool trb_is_noop(union xhci_trb *trb)
{
	return TRB_TYPE_NOOP_LE32(trb->generic.field[3]);
}

static bool trb_is_link(union xhci_trb *trb)
{
	return TRB_TYPE_LINK_LE32(trb->link.control);
}

static bool last_trb_on_seg(struct xhci_segment *seg, union xhci_trb *trb)
{
	return trb == &seg->trbs[TRBS_PER_SEGMENT - 1];
}

static bool last_trb_on_ring(struct xhci_ring *ring,
			struct xhci_segment *seg, union xhci_trb *trb)
{
	return last_trb_on_seg(seg, trb) && (seg->next == ring->first_seg);
}

static bool link_trb_toggles_cycle(union xhci_trb *trb)
{
	return le32_to_cpu(trb->link.control) & LINK_TOGGLE;
}

static bool last_td_in_urb(struct xhci_td *td)
{
	struct urb_priv *urb_priv = td->urb->hcpriv;

	return urb_priv->num_tds_done == urb_priv->num_tds;
}

static bool unhandled_event_trb(struct xhci_ring *ring)
{
	return ((le32_to_cpu(ring->dequeue->event_cmd.flags) & TRB_CYCLE) ==
		ring->cycle_state);
}

static void inc_td_cnt(struct urb *urb)
{
	struct urb_priv *urb_priv = urb->hcpriv;

	urb_priv->num_tds_done++;
}

static void trb_to_noop(union xhci_trb *trb, u32 noop_type)
{
	if (trb_is_link(trb)) {
		/* unchain chained link TRBs */
		trb->link.control &= cpu_to_le32(~TRB_CHAIN);
	} else {
		trb->generic.field[0] = 0;
		trb->generic.field[1] = 0;
		trb->generic.field[2] = 0;
		/* Preserve only the cycle bit of this TRB */
		trb->generic.field[3] &= cpu_to_le32(TRB_CYCLE);
		trb->generic.field[3] |= cpu_to_le32(TRB_TYPE(noop_type));
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Updates trb to point to the next TRB in the ring, and updates seg if the next
 * TRB is in a new segment.  This does not skip over link TRBs, and it does not
 * effect the ring dequeue or enqueue pointers.
 */
static void next_trb(struct xhci_hcd *xhci,
		struct xhci_ring *ring,
		struct xhci_segment **seg,
		union xhci_trb **trb)
{
<<<<<<< HEAD
	if (last_trb(xhci, ring, *seg, *trb)) {
=======
	if (trb_is_link(*trb) || last_trb_on_seg(*seg, *trb)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		*seg = (*seg)->next;
		*trb = ((*seg)->trbs);
	} else {
		(*trb)++;
	}
}

/*
 * See Cycle bit rules. SW is the consumer for the event ring only.
<<<<<<< HEAD
 * Don't make a ring full of link TRBs.  That would be dumb and this would loop.
 */
static void inc_deq(struct xhci_hcd *xhci, struct xhci_ring *ring)
{
	unsigned long long addr;

	ring->deq_updates++;

	/*
	 * If this is not event ring, and the dequeue pointer
	 * is not on a link TRB, there is one more usable TRB
	 */
	if (ring->type != TYPE_EVENT &&
			!last_trb(xhci, ring, ring->deq_seg, ring->dequeue))
		ring->num_trbs_free++;

	do {
		/*
		 * Update the dequeue pointer further if that was a link TRB or
		 * we're at the end of an event ring segment (which doesn't have
		 * link TRBS)
		 */
		if (last_trb(xhci, ring, ring->deq_seg, ring->dequeue)) {
			if (ring->type == TYPE_EVENT &&
					last_trb_on_last_seg(xhci, ring,
						ring->deq_seg, ring->dequeue)) {
				ring->cycle_state = (ring->cycle_state ? 0 : 1);
			}
			ring->deq_seg = ring->deq_seg->next;
			ring->dequeue = ring->deq_seg->trbs;
		} else {
			ring->dequeue++;
		}
	} while (last_trb(xhci, ring, ring->deq_seg, ring->dequeue));

	addr = (unsigned long long) xhci_trb_virt_to_dma(ring->deq_seg, ring->dequeue);
=======
 */
void inc_deq(struct xhci_hcd *xhci, struct xhci_ring *ring)
{
	unsigned int link_trb_count = 0;

	/* event ring doesn't have link trbs, check for last trb */
	if (ring->type == TYPE_EVENT) {
		if (!last_trb_on_seg(ring->deq_seg, ring->dequeue)) {
			ring->dequeue++;
			goto out;
		}
		if (last_trb_on_ring(ring, ring->deq_seg, ring->dequeue))
			ring->cycle_state ^= 1;
		ring->deq_seg = ring->deq_seg->next;
		ring->dequeue = ring->deq_seg->trbs;
		goto out;
	}

	/* All other rings have link trbs */
	if (!trb_is_link(ring->dequeue)) {
		if (last_trb_on_seg(ring->deq_seg, ring->dequeue))
			xhci_warn(xhci, "Missing link TRB at end of segment\n");
		else
			ring->dequeue++;
	}

	while (trb_is_link(ring->dequeue)) {
		ring->deq_seg = ring->deq_seg->next;
		ring->dequeue = ring->deq_seg->trbs;

		if (link_trb_count++ > ring->num_segs) {
			xhci_warn(xhci, "Ring is an endless link TRB loop\n");
			break;
		}
	}
out:
	trace_xhci_inc_deq(ring);

	return;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * See Cycle bit rules. SW is the consumer for the event ring only.
<<<<<<< HEAD
 * Don't make a ring full of link TRBs.  That would be dumb and this would loop.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * If we've just enqueued a TRB that is in the middle of a TD (meaning the
 * chain bit is set), then set the chain bit in all the following link TRBs.
 * If we've enqueued the last TRB in a TD, make sure the following link TRBs
 * have their chain bit cleared (so that each Link TRB is a separate TD).
 *
 * Section 6.4.4.1 of the 0.95 spec says link TRBs cannot have the chain bit
 * set, but other sections talk about dealing with the chain bit set.  This was
 * fixed in the 0.96 specification errata, but we have to assume that all 0.95
 * xHCI hardware can't handle the chain bit being cleared on a link TRB.
 *
 * @more_trbs_coming:	Will you enqueue more TRBs before calling
 *			prepare_transfer()?
 */
static void inc_enq(struct xhci_hcd *xhci, struct xhci_ring *ring,
			bool more_trbs_coming)
{
	u32 chain;
	union xhci_trb *next;
<<<<<<< HEAD
	unsigned long long addr;

	chain = le32_to_cpu(ring->enqueue->generic.field[3]) & TRB_CHAIN;
	/* If this is not event ring, there is one less usable TRB */
	if (ring->type != TYPE_EVENT &&
			!last_trb(xhci, ring, ring->enq_seg, ring->enqueue))
		ring->num_trbs_free--;
	next = ++(ring->enqueue);

	ring->enq_updates++;
	/* Update the dequeue pointer further if that was a link TRB or we're at
	 * the end of an event ring segment (which doesn't have link TRBS)
	 */
	while (last_trb(xhci, ring, ring->enq_seg, next)) {
		if (ring->type != TYPE_EVENT) {
			/*
			 * If the caller doesn't plan on enqueueing more
			 * TDs before ringing the doorbell, then we
			 * don't want to give the link TRB to the
			 * hardware just yet.  We'll give the link TRB
			 * back in prepare_ring() just before we enqueue
			 * the TD at the top of the ring.
			 */
			if (!chain && !more_trbs_coming)
				break;

			/* If we're not dealing with 0.95 hardware or
			 * isoc rings on AMD 0.96 host,
			 * carry over the chain bit of the previous TRB
			 * (which may mean the chain bit is cleared).
			 */
			if (!(ring->type == TYPE_ISOC &&
					(xhci->quirks & XHCI_AMD_0x96_HOST))
						&& !xhci_link_trb_quirk(xhci)) {
				next->link.control &=
					cpu_to_le32(~TRB_CHAIN);
				next->link.control |=
					cpu_to_le32(chain);
			}
			/* Give this link TRB to the hardware */
			wmb();
			next->link.control ^= cpu_to_le32(TRB_CYCLE);

			/* Toggle the cycle bit after the last ring segment. */
			if (last_trb_on_last_seg(xhci, ring, ring->enq_seg, next)) {
				ring->cycle_state = (ring->cycle_state ? 0 : 1);
			}
		}
		ring->enq_seg = ring->enq_seg->next;
		ring->enqueue = ring->enq_seg->trbs;
		next = ring->enqueue;
	}
	addr = (unsigned long long) xhci_trb_virt_to_dma(ring->enq_seg, ring->enqueue);
=======
	unsigned int link_trb_count = 0;

	chain = le32_to_cpu(ring->enqueue->generic.field[3]) & TRB_CHAIN;

	if (last_trb_on_seg(ring->enq_seg, ring->enqueue)) {
		xhci_err(xhci, "Tried to move enqueue past ring segment\n");
		return;
	}

	next = ++(ring->enqueue);

	/* Update the dequeue pointer further if that was a link TRB */
	while (trb_is_link(next)) {

		/*
		 * If the caller doesn't plan on enqueueing more TDs before
		 * ringing the doorbell, then we don't want to give the link TRB
		 * to the hardware just yet. We'll give the link TRB back in
		 * prepare_ring() just before we enqueue the TD at the top of
		 * the ring.
		 */
		if (!chain && !more_trbs_coming)
			break;

		/* If we're not dealing with 0.95 hardware or isoc rings on
		 * AMD 0.96 host, carry over the chain bit of the previous TRB
		 * (which may mean the chain bit is cleared).
		 */
		if (!(ring->type == TYPE_ISOC &&
		      (xhci->quirks & XHCI_AMD_0x96_HOST)) &&
		    !xhci_link_trb_quirk(xhci)) {
			next->link.control &= cpu_to_le32(~TRB_CHAIN);
			next->link.control |= cpu_to_le32(chain);
		}
		/* Give this link TRB to the hardware */
		wmb();
		next->link.control ^= cpu_to_le32(TRB_CYCLE);

		/* Toggle the cycle bit after the last ring segment. */
		if (link_trb_toggles_cycle(next))
			ring->cycle_state ^= 1;

		ring->enq_seg = ring->enq_seg->next;
		ring->enqueue = ring->enq_seg->trbs;
		next = ring->enqueue;

		if (link_trb_count++ > ring->num_segs) {
			xhci_warn(xhci, "%s: Ring link TRB loop\n", __func__);
			break;
		}
	}

	trace_xhci_inc_enq(ring);
}

/*
 * Return number of free normal TRBs from enqueue to dequeue pointer on ring.
 * Not counting an assumed link TRB at end of each TRBS_PER_SEGMENT sized segment.
 * Only for transfer and command rings where driver is the producer, not for
 * event rings.
 */
static unsigned int xhci_num_trbs_free(struct xhci_hcd *xhci, struct xhci_ring *ring)
{
	struct xhci_segment *enq_seg = ring->enq_seg;
	union xhci_trb *enq = ring->enqueue;
	union xhci_trb *last_on_seg;
	unsigned int free = 0;
	int i = 0;

	/* Ring might be empty even if enq != deq if enq is left on a link trb */
	if (trb_is_link(enq)) {
		enq_seg = enq_seg->next;
		enq = enq_seg->trbs;
	}

	/* Empty ring, common case, don't walk the segments */
	if (enq == ring->dequeue)
		return ring->num_segs * (TRBS_PER_SEGMENT - 1);

	do {
		if (ring->deq_seg == enq_seg && ring->dequeue >= enq)
			return free + (ring->dequeue - enq);
		last_on_seg = &enq_seg->trbs[TRBS_PER_SEGMENT - 1];
		free += last_on_seg - enq;
		enq_seg = enq_seg->next;
		enq = enq_seg->trbs;
	} while (i++ <= ring->num_segs);

	return free;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Check to see if there's room to enqueue num_trbs on the ring and make sure
 * enqueue pointer will not advance into dequeue segment. See rules above.
<<<<<<< HEAD
 */
static inline int room_on_ring(struct xhci_hcd *xhci, struct xhci_ring *ring,
		unsigned int num_trbs)
{
	int num_trbs_in_deq_seg;

	if (ring->num_trbs_free < num_trbs)
		return 0;

	if (ring->type != TYPE_COMMAND && ring->type != TYPE_EVENT) {
		num_trbs_in_deq_seg = ring->dequeue - ring->deq_seg->trbs;
		if (ring->num_trbs_free < num_trbs + num_trbs_in_deq_seg)
			return 0;
	}

	return 1;
=======
 * return number of new segments needed to ensure this.
 */

static unsigned int xhci_ring_expansion_needed(struct xhci_hcd *xhci, struct xhci_ring *ring,
					       unsigned int num_trbs)
{
	struct xhci_segment *seg;
	int trbs_past_seg;
	int enq_used;
	int new_segs;

	enq_used = ring->enqueue - ring->enq_seg->trbs;

	/* how many trbs will be queued past the enqueue segment? */
	trbs_past_seg = enq_used + num_trbs - (TRBS_PER_SEGMENT - 1);

	/*
	 * Consider expanding the ring already if num_trbs fills the current
	 * segment (i.e. trbs_past_seg == 0), not only when num_trbs goes into
	 * the next segment. Avoids confusing full ring with special empty ring
	 * case below
	 */
	if (trbs_past_seg < 0)
		return 0;

	/* Empty ring special case, enqueue stuck on link trb while dequeue advanced */
	if (trb_is_link(ring->enqueue) && ring->enq_seg->next->trbs == ring->dequeue)
		return 0;

	new_segs = 1 + (trbs_past_seg / (TRBS_PER_SEGMENT - 1));
	seg = ring->enq_seg;

	while (new_segs > 0) {
		seg = seg->next;
		if (seg == ring->deq_seg) {
			xhci_dbg(xhci, "Ring expansion by %d segments needed\n",
				 new_segs);
			xhci_dbg(xhci, "Adding %d trbs moves enq %d trbs into deq seg\n",
				 num_trbs, trbs_past_seg % TRBS_PER_SEGMENT);
			return new_segs;
		}
		new_segs--;
	}

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Ring the host controller doorbell after placing a command on the ring */
void xhci_ring_cmd_db(struct xhci_hcd *xhci)
{
	if (!(xhci->cmd_ring_state & CMD_RING_STATE_RUNNING))
		return;

	xhci_dbg(xhci, "// Ding dong!\n");
<<<<<<< HEAD
	xhci_writel(xhci, DB_VALUE_HOST, &xhci->dba->doorbell[0]);
	/* Flush PCI posted writes */
	xhci_readl(xhci, &xhci->dba->doorbell[0]);
}

static int xhci_abort_cmd_ring(struct xhci_hcd *xhci)
{
	u64 temp_64;
=======

	trace_xhci_ring_host_doorbell(0, DB_VALUE_HOST);

	writel(DB_VALUE_HOST, &xhci->dba->doorbell[0]);
	/* Flush PCI posted writes */
	readl(&xhci->dba->doorbell[0]);
}

static bool xhci_mod_cmd_timer(struct xhci_hcd *xhci)
{
	return mod_delayed_work(system_wq, &xhci->cmd_timer,
			msecs_to_jiffies(xhci->current_cmd->timeout_ms));
}

static struct xhci_command *xhci_next_queued_cmd(struct xhci_hcd *xhci)
{
	return list_first_entry_or_null(&xhci->cmd_list, struct xhci_command,
					cmd_list);
}

/*
 * Turn all commands on command ring with status set to "aborted" to no-op trbs.
 * If there are other commands waiting then restart the ring and kick the timer.
 * This must be called with command ring stopped and xhci->lock held.
 */
static void xhci_handle_stopped_cmd_ring(struct xhci_hcd *xhci,
					 struct xhci_command *cur_cmd)
{
	struct xhci_command *i_cmd;

	/* Turn all aborted commands in list to no-ops, then restart */
	list_for_each_entry(i_cmd, &xhci->cmd_list, cmd_list) {

		if (i_cmd->status != COMP_COMMAND_ABORTED)
			continue;

		i_cmd->status = COMP_COMMAND_RING_STOPPED;

		xhci_dbg(xhci, "Turn aborted command %p to no-op\n",
			 i_cmd->command_trb);

		trb_to_noop(i_cmd->command_trb, TRB_CMD_NOOP);

		/*
		 * caller waiting for completion is called when command
		 *  completion event is received for these no-op commands
		 */
	}

	xhci->cmd_ring_state = CMD_RING_STATE_RUNNING;

	/* ring command ring doorbell to restart the command ring */
	if ((xhci->cmd_ring->dequeue != xhci->cmd_ring->enqueue) &&
	    !(xhci->xhc_state & XHCI_STATE_DYING)) {
		xhci->current_cmd = cur_cmd;
		xhci_mod_cmd_timer(xhci);
		xhci_ring_cmd_db(xhci);
	}
}

/* Must be called with xhci->lock held, releases and aquires lock back */
static int xhci_abort_cmd_ring(struct xhci_hcd *xhci, unsigned long flags)
{
	struct xhci_segment *new_seg	= xhci->cmd_ring->deq_seg;
	union xhci_trb *new_deq		= xhci->cmd_ring->dequeue;
	u64 crcr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int ret;

	xhci_dbg(xhci, "Abort command ring\n");

<<<<<<< HEAD
	if (!(xhci->cmd_ring_state & CMD_RING_STATE_RUNNING)) {
		xhci_dbg(xhci, "The command ring isn't running, "
				"Have the command ring been stopped?\n");
		return 0;
	}

	temp_64 = xhci_read_64(xhci, &xhci->op_regs->cmd_ring);
	if (!(temp_64 & CMD_RING_RUNNING)) {
		xhci_dbg(xhci, "Command ring had been stopped\n");
		return 0;
	}
	xhci->cmd_ring_state = CMD_RING_STATE_ABORTED;
	xhci_write_64(xhci, temp_64 | CMD_RING_ABORT,
			&xhci->op_regs->cmd_ring);

	/* Section 4.6.1.2 of xHCI 1.0 spec says software should
	 * time the completion od all xHCI commands, including
	 * the Command Abort operation. If software doesn't see
	 * CRR negated in a timely manner (e.g. longer than 5
	 * seconds), then it should assume that the there are
	 * larger problems with the xHC and assert HCRST.
	 */
	ret = handshake(xhci, &xhci->op_regs->cmd_ring,
			CMD_RING_RUNNING, 0, 5 * 1000 * 1000);
	if (ret < 0) {
		/* we are about to kill xhci, give it one more chance */
		xhci_write_64(xhci, temp_64 | CMD_RING_ABORT,
			      &xhci->op_regs->cmd_ring);
		udelay(1000);
		ret = handshake(xhci, &xhci->op_regs->cmd_ring,
				CMD_RING_RUNNING, 0, 3 * 1000 * 1000);
		if (ret == 0)
			return 0;

		xhci_err(xhci, "Stopped the command ring failed, "
				"maybe the host is dead\n");
		xhci->xhc_state |= XHCI_STATE_DYING;
		xhci_quiesce(xhci);
		xhci_halt(xhci);
		return -ESHUTDOWN;
	}

	return 0;
}

static int xhci_queue_cd(struct xhci_hcd *xhci,
		struct xhci_command *command,
		union xhci_trb *cmd_trb)
{
	struct xhci_cd *cd;
	cd = kzalloc(sizeof(struct xhci_cd), GFP_ATOMIC);
	if (!cd)
		return -ENOMEM;
	INIT_LIST_HEAD(&cd->cancel_cmd_list);

	cd->command = command;
	cd->cmd_trb = cmd_trb;
	list_add_tail(&cd->cancel_cmd_list, &xhci->cancel_cmd_list);

	return 0;
}

/*
 * Cancel the command which has issue.
 *
 * Some commands may hang due to waiting for acknowledgement from
 * usb device. It is outside of the xHC's ability to control and
 * will cause the command ring is blocked. When it occurs software
 * should intervene to recover the command ring.
 * See Section 4.6.1.1 and 4.6.1.2
 */
int xhci_cancel_cmd(struct xhci_hcd *xhci, struct xhci_command *command,
		union xhci_trb *cmd_trb)
{
	int retval = 0;
	unsigned long flags;

	spin_lock_irqsave(&xhci->lock, flags);

	if (xhci->xhc_state & XHCI_STATE_DYING) {
		xhci_warn(xhci, "Abort the command ring,"
				" but the xHCI is dead.\n");
		retval = -ESHUTDOWN;
		goto fail;
	}

	/* queue the cmd desriptor to cancel_cmd_list */
	retval = xhci_queue_cd(xhci, command, cmd_trb);
	if (retval) {
		xhci_warn(xhci, "Queuing command descriptor failed.\n");
		goto fail;
	}

	/* abort command ring */
	retval = xhci_abort_cmd_ring(xhci);
	if (retval) {
		xhci_err(xhci, "Abort command ring failed\n");
		if (unlikely(retval == -ESHUTDOWN)) {
			spin_unlock_irqrestore(&xhci->lock, flags);
			usb_hc_died(xhci_to_hcd(xhci)->primary_hcd);
			xhci_dbg(xhci, "xHCI host controller is dead.\n");
			return retval;
		}
	}

fail:
	spin_unlock_irqrestore(&xhci->lock, flags);
	return retval;
=======
	reinit_completion(&xhci->cmd_ring_stop_completion);

	/*
	 * The control bits like command stop, abort are located in lower
	 * dword of the command ring control register.
	 * Some controllers require all 64 bits to be written to abort the ring.
	 * Make sure the upper dword is valid, pointing to the next command,
	 * avoiding corrupting the command ring pointer in case the command ring
	 * is stopped by the time the upper dword is written.
	 */
	next_trb(xhci, NULL, &new_seg, &new_deq);
	if (trb_is_link(new_deq))
		next_trb(xhci, NULL, &new_seg, &new_deq);

	crcr = xhci_trb_virt_to_dma(new_seg, new_deq);
	xhci_write_64(xhci, crcr | CMD_RING_ABORT, &xhci->op_regs->cmd_ring);

	/* Section 4.6.1.2 of xHCI 1.0 spec says software should also time the
	 * completion of the Command Abort operation. If CRR is not negated in 5
	 * seconds then driver handles it as if host died (-ENODEV).
	 * In the future we should distinguish between -ENODEV and -ETIMEDOUT
	 * and try to recover a -ETIMEDOUT with a host controller reset.
	 */
	ret = xhci_handshake_check_state(xhci, &xhci->op_regs->cmd_ring,
			CMD_RING_RUNNING, 0, 5 * 1000 * 1000,
			XHCI_STATE_REMOVING);
	if (ret < 0) {
		xhci_err(xhci, "Abort failed to stop command ring: %d\n", ret);
		xhci_halt(xhci);
		xhci_hc_died(xhci);
		return ret;
	}
	/*
	 * Writing the CMD_RING_ABORT bit should cause a cmd completion event,
	 * however on some host hw the CMD_RING_RUNNING bit is correctly cleared
	 * but the completion event in never sent. Wait 2 secs (arbitrary
	 * number) to handle those cases after negation of CMD_RING_RUNNING.
	 */
	spin_unlock_irqrestore(&xhci->lock, flags);
	ret = wait_for_completion_timeout(&xhci->cmd_ring_stop_completion,
					  msecs_to_jiffies(2000));
	spin_lock_irqsave(&xhci->lock, flags);
	if (!ret) {
		xhci_dbg(xhci, "No stop event for abort, ring start fail?\n");
		xhci_cleanup_command_queue(xhci);
	} else {
		xhci_handle_stopped_cmd_ring(xhci, xhci_next_queued_cmd(xhci));
	}
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void xhci_ring_ep_doorbell(struct xhci_hcd *xhci,
		unsigned int slot_id,
		unsigned int ep_index,
		unsigned int stream_id)
{
	__le32 __iomem *db_addr = &xhci->dba->doorbell[slot_id];
	struct xhci_virt_ep *ep = &xhci->devs[slot_id]->eps[ep_index];
	unsigned int ep_state = ep->ep_state;

	/* Don't ring the doorbell for this endpoint if there are pending
	 * cancellations because we don't want to interrupt processing.
	 * We don't want to restart any stream rings if there's a set dequeue
	 * pointer command pending because the device can choose to start any
	 * stream once the endpoint is on the HW schedule.
<<<<<<< HEAD
	 * FIXME - check all the stream rings for pending cancellations.
	 */
	if ((ep_state & EP_HALT_PENDING) || (ep_state & SET_DEQ_PENDING) ||
	    (ep_state & EP_HALTED))
		return;
	xhci_writel(xhci, DB_VALUE(ep_index, stream_id), db_addr);
	/* The CPU has better things to do at this point than wait for a
	 * write-posting flush.  It'll get there soon enough.
	 */
=======
	 */
	if ((ep_state & EP_STOP_CMD_PENDING) || (ep_state & SET_DEQ_PENDING) ||
	    (ep_state & EP_HALTED) || (ep_state & EP_CLEARING_TT))
		return;

	trace_xhci_ring_ep_doorbell(slot_id, DB_VALUE(ep_index, stream_id));

	writel(DB_VALUE(ep_index, stream_id), db_addr);
	/* flush the write */
	readl(db_addr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Ring the doorbell for any rings with pending URBs */
static void ring_doorbell_for_active_rings(struct xhci_hcd *xhci,
		unsigned int slot_id,
		unsigned int ep_index)
{
	unsigned int stream_id;
	struct xhci_virt_ep *ep;

	ep = &xhci->devs[slot_id]->eps[ep_index];

	/* A ring has pending URBs if its TD list is not empty */
	if (!(ep->ep_state & EP_HAS_STREAMS)) {
		if (ep->ring && !(list_empty(&ep->ring->td_list)))
			xhci_ring_ep_doorbell(xhci, slot_id, ep_index, 0);
		return;
	}

	for (stream_id = 1; stream_id < ep->stream_info->num_streams;
			stream_id++) {
		struct xhci_stream_info *stream_info = ep->stream_info;
		if (!list_empty(&stream_info->stream_rings[stream_id]->td_list))
			xhci_ring_ep_doorbell(xhci, slot_id, ep_index,
						stream_id);
	}
}

<<<<<<< HEAD
/*
 * Find the segment that trb is in.  Start searching in start_seg.
 * If we must move past a segment that has a link TRB with a toggle cycle state
 * bit set, then we will toggle the value pointed at by cycle_state.
 */
static struct xhci_segment *find_trb_seg(
		struct xhci_segment *start_seg,
		union xhci_trb	*trb, int *cycle_state)
{
	struct xhci_segment *cur_seg = start_seg;
	struct xhci_generic_trb *generic_trb;

	while (cur_seg->trbs > trb ||
			&cur_seg->trbs[TRBS_PER_SEGMENT - 1] < trb) {
		generic_trb = &cur_seg->trbs[TRBS_PER_SEGMENT - 1].generic;
		if (generic_trb->field[3] & cpu_to_le32(LINK_TOGGLE))
			*cycle_state ^= 0x1;
		cur_seg = cur_seg->next;
		if (cur_seg == start_seg)
			/* Looped over the entire list.  Oops! */
			return NULL;
	}
	return cur_seg;
}


static struct xhci_ring *xhci_triad_to_transfer_ring(struct xhci_hcd *xhci,
=======
void xhci_ring_doorbell_for_active_rings(struct xhci_hcd *xhci,
		unsigned int slot_id,
		unsigned int ep_index)
{
	ring_doorbell_for_active_rings(xhci, slot_id, ep_index);
}

static struct xhci_virt_ep *xhci_get_virt_ep(struct xhci_hcd *xhci,
					     unsigned int slot_id,
					     unsigned int ep_index)
{
	if (slot_id == 0 || slot_id >= MAX_HC_SLOTS) {
		xhci_warn(xhci, "Invalid slot_id %u\n", slot_id);
		return NULL;
	}
	if (ep_index >= EP_CTX_PER_DEV) {
		xhci_warn(xhci, "Invalid endpoint index %u\n", ep_index);
		return NULL;
	}
	if (!xhci->devs[slot_id]) {
		xhci_warn(xhci, "No xhci virt device for slot_id %u\n", slot_id);
		return NULL;
	}

	return &xhci->devs[slot_id]->eps[ep_index];
}

static struct xhci_ring *xhci_virt_ep_to_ring(struct xhci_hcd *xhci,
					      struct xhci_virt_ep *ep,
					      unsigned int stream_id)
{
	/* common case, no streams */
	if (!(ep->ep_state & EP_HAS_STREAMS))
		return ep->ring;

	if (!ep->stream_info)
		return NULL;

	if (stream_id == 0 || stream_id >= ep->stream_info->num_streams) {
		xhci_warn(xhci, "Invalid stream_id %u request for slot_id %u ep_index %u\n",
			  stream_id, ep->vdev->slot_id, ep->ep_index);
		return NULL;
	}

	return ep->stream_info->stream_rings[stream_id];
}

/* Get the right ring for the given slot_id, ep_index and stream_id.
 * If the endpoint supports streams, boundary check the URB's stream ID.
 * If the endpoint doesn't support streams, return the singular endpoint ring.
 */
struct xhci_ring *xhci_triad_to_transfer_ring(struct xhci_hcd *xhci,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		unsigned int slot_id, unsigned int ep_index,
		unsigned int stream_id)
{
	struct xhci_virt_ep *ep;

<<<<<<< HEAD
	ep = &xhci->devs[slot_id]->eps[ep_index];
	/* Common case: no streams */
	if (!(ep->ep_state & EP_HAS_STREAMS))
		return ep->ring;

	if (stream_id == 0) {
		xhci_warn(xhci,
				"WARN: Slot ID %u, ep index %u has streams, "
				"but URB has no stream ID.\n",
				slot_id, ep_index);
		return NULL;
	}

	if (stream_id < ep->stream_info->num_streams)
		return ep->stream_info->stream_rings[stream_id];

	xhci_warn(xhci,
			"WARN: Slot ID %u, ep index %u has "
			"stream IDs 1 to %u allocated, "
			"but stream ID %u is requested.\n",
			slot_id, ep_index,
			ep->stream_info->num_streams - 1,
			stream_id);
	return NULL;
}

/* Get the right ring for the given URB.
 * If the endpoint supports streams, boundary check the URB's stream ID.
 * If the endpoint doesn't support streams, return the singular endpoint ring.
 */
static struct xhci_ring *xhci_urb_to_transfer_ring(struct xhci_hcd *xhci,
		struct urb *urb)
{
	return xhci_triad_to_transfer_ring(xhci, urb->dev->slot_id,
		xhci_get_endpoint_index(&urb->ep->desc), urb->stream_id);
}

/*
 * Move the xHC's endpoint ring dequeue pointer past cur_td.
 * Record the new state of the xHC's endpoint ring dequeue segment,
 * dequeue pointer, and new consumer cycle state in state.
 * Update our internal representation of the ring's dequeue pointer.
 *
 * We do this in three jumps:
 *  - First we update our new ring state to be the same as when the xHC stopped.
 *  - Then we traverse the ring to find the segment that contains
 *    the last TRB in the TD.  We toggle the xHC's new cycle state when we pass
 *    any link TRBs with the toggle cycle bit set.
 *  - Finally we move the dequeue state one TRB further, toggling the cycle bit
 *    if we've moved it past a link TRB with the toggle cycle bit set.
 *
 * Some of the uses of xhci_generic_trb are grotty, but if they're done
 * with correct __le32 accesses they should work fine.  Only users of this are
 * in here.
 */
void xhci_find_new_dequeue_state(struct xhci_hcd *xhci,
		unsigned int slot_id, unsigned int ep_index,
		unsigned int stream_id, struct xhci_td *cur_td,
		struct xhci_dequeue_state *state)
{
	struct xhci_virt_device *dev = xhci->devs[slot_id];
	struct xhci_ring *ep_ring;
	struct xhci_generic_trb *trb;
	struct xhci_ep_ctx *ep_ctx;
	dma_addr_t addr;
=======
	ep = xhci_get_virt_ep(xhci, slot_id, ep_index);
	if (!ep)
		return NULL;

	return xhci_virt_ep_to_ring(xhci, ep, stream_id);
}


/*
 * Get the hw dequeue pointer xHC stopped on, either directly from the
 * endpoint context, or if streams are in use from the stream context.
 * The returned hw_dequeue contains the lowest four bits with cycle state
 * and possbile stream context type.
 */
static u64 xhci_get_hw_deq(struct xhci_hcd *xhci, struct xhci_virt_device *vdev,
			   unsigned int ep_index, unsigned int stream_id)
{
	struct xhci_ep_ctx *ep_ctx;
	struct xhci_stream_ctx *st_ctx;
	struct xhci_virt_ep *ep;

	ep = &vdev->eps[ep_index];

	if (ep->ep_state & EP_HAS_STREAMS) {
		st_ctx = &ep->stream_info->stream_ctx_array[stream_id];
		return le64_to_cpu(st_ctx->stream_ring);
	}
	ep_ctx = xhci_get_ep_ctx(xhci, vdev->out_ctx, ep_index);
	return le64_to_cpu(ep_ctx->deq);
}

static int xhci_move_dequeue_past_td(struct xhci_hcd *xhci,
				unsigned int slot_id, unsigned int ep_index,
				unsigned int stream_id, struct xhci_td *td)
{
	struct xhci_virt_device *dev = xhci->devs[slot_id];
	struct xhci_virt_ep *ep = &dev->eps[ep_index];
	struct xhci_ring *ep_ring;
	struct xhci_command *cmd;
	struct xhci_segment *new_seg;
	union xhci_trb *new_deq;
	int new_cycle;
	dma_addr_t addr;
	u64 hw_dequeue;
	bool cycle_found = false;
	bool td_last_trb_found = false;
	u32 trb_sct = 0;
	int ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ep_ring = xhci_triad_to_transfer_ring(xhci, slot_id,
			ep_index, stream_id);
	if (!ep_ring) {
<<<<<<< HEAD
		xhci_warn(xhci, "WARN can't find new dequeue state "
				"for invalid stream ID %u.\n",
				stream_id);
		return;
	}
	state->new_cycle_state = 0;
	xhci_dbg(xhci, "Finding segment containing stopped TRB.\n");
	state->new_deq_seg = find_trb_seg(cur_td->start_seg,
			dev->eps[ep_index].stopped_trb,
			&state->new_cycle_state);
	if (!state->new_deq_seg) {
		WARN_ON(1);
		return;
	}

	/* Dig out the cycle state saved by the xHC during the stop ep cmd */
	xhci_dbg(xhci, "Finding endpoint context\n");
	ep_ctx = xhci_get_ep_ctx(xhci, dev->out_ctx, ep_index);
	state->new_cycle_state = 0x1 & le64_to_cpu(ep_ctx->deq);

	state->new_deq_ptr = cur_td->last_trb;
	xhci_dbg(xhci, "Finding segment containing last TRB in TD.\n");
	state->new_deq_seg = find_trb_seg(state->new_deq_seg,
			state->new_deq_ptr,
			&state->new_cycle_state);
	if (!state->new_deq_seg) {
		WARN_ON(1);
		return;
	}

	trb = &state->new_deq_ptr->generic;
	if (TRB_TYPE_LINK_LE32(trb->field[3]) &&
	    (trb->field[3] & cpu_to_le32(LINK_TOGGLE)))
		state->new_cycle_state ^= 0x1;
	next_trb(xhci, ep_ring, &state->new_deq_seg, &state->new_deq_ptr);

	/*
	 * If there is only one segment in a ring, find_trb_seg()'s while loop
	 * will not run, and it will return before it has a chance to see if it
	 * needs to toggle the cycle bit.  It can't tell if the stalled transfer
	 * ended just before the link TRB on a one-segment ring, or if the TD
	 * wrapped around the top of the ring, because it doesn't have the TD in
	 * question.  Look for the one-segment case where stalled TRB's address
	 * is greater than the new dequeue pointer address.
	 */
	if (ep_ring->first_seg == ep_ring->first_seg->next &&
			state->new_deq_ptr < dev->eps[ep_index].stopped_trb)
		state->new_cycle_state ^= 0x1;
	xhci_dbg(xhci, "Cycle state = 0x%x\n", state->new_cycle_state);

	/* Don't update the ring cycle state for the producer (us). */
	xhci_dbg(xhci, "New dequeue segment = %p (virtual)\n",
			state->new_deq_seg);
	addr = xhci_trb_virt_to_dma(state->new_deq_seg, state->new_deq_ptr);
	xhci_dbg(xhci, "New dequeue pointer = 0x%llx (DMA)\n",
			(unsigned long long) addr);
=======
		xhci_warn(xhci, "WARN can't find new dequeue, invalid stream ID %u\n",
			  stream_id);
		return -ENODEV;
	}
	/*
	 * A cancelled TD can complete with a stall if HW cached the trb.
	 * In this case driver can't find td, but if the ring is empty we
	 * can move the dequeue pointer to the current enqueue position.
	 * We shouldn't hit this anymore as cached cancelled TRBs are given back
	 * after clearing the cache, but be on the safe side and keep it anyway
	 */
	if (!td) {
		if (list_empty(&ep_ring->td_list)) {
			new_seg = ep_ring->enq_seg;
			new_deq = ep_ring->enqueue;
			new_cycle = ep_ring->cycle_state;
			xhci_dbg(xhci, "ep ring empty, Set new dequeue = enqueue");
			goto deq_found;
		} else {
			xhci_warn(xhci, "Can't find new dequeue state, missing td\n");
			return -EINVAL;
		}
	}

	hw_dequeue = xhci_get_hw_deq(xhci, dev, ep_index, stream_id);
	new_seg = ep_ring->deq_seg;
	new_deq = ep_ring->dequeue;
	new_cycle = hw_dequeue & 0x1;

	/*
	 * We want to find the pointer, segment and cycle state of the new trb
	 * (the one after current TD's last_trb). We know the cycle state at
	 * hw_dequeue, so walk the ring until both hw_dequeue and last_trb are
	 * found.
	 */
	do {
		if (!cycle_found && xhci_trb_virt_to_dma(new_seg, new_deq)
		    == (dma_addr_t)(hw_dequeue & ~0xf)) {
			cycle_found = true;
			if (td_last_trb_found)
				break;
		}
		if (new_deq == td->last_trb)
			td_last_trb_found = true;

		if (cycle_found && trb_is_link(new_deq) &&
		    link_trb_toggles_cycle(new_deq))
			new_cycle ^= 0x1;

		next_trb(xhci, ep_ring, &new_seg, &new_deq);

		/* Search wrapped around, bail out */
		if (new_deq == ep->ring->dequeue) {
			xhci_err(xhci, "Error: Failed finding new dequeue state\n");
			return -EINVAL;
		}

	} while (!cycle_found || !td_last_trb_found);

deq_found:

	/* Don't update the ring cycle state for the producer (us). */
	addr = xhci_trb_virt_to_dma(new_seg, new_deq);
	if (addr == 0) {
		xhci_warn(xhci, "Can't find dma of new dequeue ptr\n");
		xhci_warn(xhci, "deq seg = %p, deq ptr = %p\n", new_seg, new_deq);
		return -EINVAL;
	}

	if ((ep->ep_state & SET_DEQ_PENDING)) {
		xhci_warn(xhci, "Set TR Deq already pending, don't submit for 0x%pad\n",
			  &addr);
		return -EBUSY;
	}

	/* This function gets called from contexts where it cannot sleep */
	cmd = xhci_alloc_command(xhci, false, GFP_ATOMIC);
	if (!cmd) {
		xhci_warn(xhci, "Can't alloc Set TR Deq cmd 0x%pad\n", &addr);
		return -ENOMEM;
	}

	if (stream_id)
		trb_sct = SCT_FOR_TRB(SCT_PRI_TR);
	ret = queue_command(xhci, cmd,
		lower_32_bits(addr) | trb_sct | new_cycle,
		upper_32_bits(addr),
		STREAM_ID_FOR_TRB(stream_id), SLOT_ID_FOR_TRB(slot_id) |
		EP_ID_FOR_TRB(ep_index) | TRB_TYPE(TRB_SET_DEQ), false);
	if (ret < 0) {
		xhci_free_command(xhci, cmd);
		return ret;
	}
	ep->queued_deq_seg = new_seg;
	ep->queued_deq_ptr = new_deq;

	xhci_dbg_trace(xhci, trace_xhci_dbg_cancel_urb,
		       "Set TR Deq ptr 0x%llx, cycle %u\n", addr, new_cycle);

	/* Stop the TD queueing code from ringing the doorbell until
	 * this command completes.  The HC won't set the dequeue pointer
	 * if the ring is running, and ringing the doorbell starts the
	 * ring running.
	 */
	ep->ep_state |= SET_DEQ_PENDING;
	xhci_ring_cmd_db(xhci);
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* flip_cycle means flip the cycle bit of all but the first and last TRB.
 * (The last TRB actually points to the ring enqueue pointer, which is not part
 * of this TD.)  This is used to remove partially enqueued isoc TDs from a ring.
 */
static void td_to_noop(struct xhci_hcd *xhci, struct xhci_ring *ep_ring,
<<<<<<< HEAD
		struct xhci_td *cur_td, bool flip_cycle)
{
	struct xhci_segment *cur_seg;
	union xhci_trb *cur_trb;

	for (cur_seg = cur_td->start_seg, cur_trb = cur_td->first_trb;
			true;
			next_trb(xhci, ep_ring, &cur_seg, &cur_trb)) {
		if (TRB_TYPE_LINK_LE32(cur_trb->generic.field[3])) {
			/* Unchain any chained Link TRBs, but
			 * leave the pointers intact.
			 */
			cur_trb->generic.field[3] &= cpu_to_le32(~TRB_CHAIN);
			/* Flip the cycle bit (link TRBs can't be the first
			 * or last TRB).
			 */
			if (flip_cycle)
				cur_trb->generic.field[3] ^=
					cpu_to_le32(TRB_CYCLE);
			xhci_dbg(xhci, "Cancel (unchain) link TRB\n");
			xhci_dbg(xhci, "Address = %p (0x%llx dma); "
					"in seg %p (0x%llx dma)\n",
					cur_trb,
					(unsigned long long)xhci_trb_virt_to_dma(cur_seg, cur_trb),
					cur_seg,
					(unsigned long long)cur_seg->dma);
		} else {
			cur_trb->generic.field[0] = 0;
			cur_trb->generic.field[1] = 0;
			cur_trb->generic.field[2] = 0;
			/* Preserve only the cycle bit of this TRB */
			cur_trb->generic.field[3] &= cpu_to_le32(TRB_CYCLE);
			/* Flip the cycle bit except on the first or last TRB */
			if (flip_cycle && cur_trb != cur_td->first_trb &&
					cur_trb != cur_td->last_trb)
				cur_trb->generic.field[3] ^=
					cpu_to_le32(TRB_CYCLE);
			cur_trb->generic.field[3] |= cpu_to_le32(
				TRB_TYPE(TRB_TR_NOOP));
			xhci_dbg(xhci, "TRB to noop at offset 0x%llx\n",
					(unsigned long long)
					xhci_trb_virt_to_dma(cur_seg, cur_trb));
		}
		if (cur_trb == cur_td->last_trb)
			break;
	}
}

static int queue_set_tr_deq(struct xhci_hcd *xhci, int slot_id,
		unsigned int ep_index, unsigned int stream_id,
		struct xhci_segment *deq_seg,
		union xhci_trb *deq_ptr, u32 cycle_state);

void xhci_queue_new_dequeue_state(struct xhci_hcd *xhci,
		unsigned int slot_id, unsigned int ep_index,
		unsigned int stream_id,
		struct xhci_dequeue_state *deq_state)
{
	struct xhci_virt_ep *ep = &xhci->devs[slot_id]->eps[ep_index];

	xhci_dbg(xhci, "Set TR Deq Ptr cmd, new deq seg = %p (0x%llx dma), "
			"new deq ptr = %p (0x%llx dma), new cycle = %u\n",
			deq_state->new_deq_seg,
			(unsigned long long)deq_state->new_deq_seg->dma,
			deq_state->new_deq_ptr,
			(unsigned long long)xhci_trb_virt_to_dma(deq_state->new_deq_seg, deq_state->new_deq_ptr),
			deq_state->new_cycle_state);
	queue_set_tr_deq(xhci, slot_id, ep_index, stream_id,
			deq_state->new_deq_seg,
			deq_state->new_deq_ptr,
			(u32) deq_state->new_cycle_state);
	/* Stop the TD queueing code from ringing the doorbell until
	 * this command completes.  The HC won't set the dequeue pointer
	 * if the ring is running, and ringing the doorbell starts the
	 * ring running.
	 */
	ep->ep_state |= SET_DEQ_PENDING;
}

static void xhci_stop_watchdog_timer_in_irq(struct xhci_hcd *xhci,
		struct xhci_virt_ep *ep)
{
	ep->ep_state &= ~EP_HALT_PENDING;
	/* Can't del_timer_sync in interrupt, so we attempt to cancel.  If the
	 * timer is running on another CPU, we don't decrement stop_cmds_pending
	 * (since we didn't successfully stop the watchdog timer).
	 */
	if (del_timer(&ep->stop_cmd_timer))
		ep->stop_cmds_pending--;
}

/* Must be called with xhci->lock held in interrupt context */
static void xhci_giveback_urb_in_irq(struct xhci_hcd *xhci,
		struct xhci_td *cur_td, int status, char *adjective)
{
	struct usb_hcd *hcd;
	struct urb	*urb;
	struct urb_priv	*urb_priv;

	urb = cur_td->urb;
	urb_priv = urb->hcpriv;
	urb_priv->td_cnt++;
	hcd = bus_to_hcd(urb->dev->bus);

	/* Only giveback urb when this is the last td in urb */
	if (urb_priv->td_cnt == urb_priv->length) {
		if (usb_pipetype(urb->pipe) == PIPE_ISOCHRONOUS) {
			xhci_to_hcd(xhci)->self.bandwidth_isoc_reqs--;
			if (xhci_to_hcd(xhci)->self.bandwidth_isoc_reqs	== 0) {
				if (xhci->quirks & XHCI_AMD_PLL_FIX)
					usb_amd_quirk_pll_enable();
			}
		}
		usb_hcd_unlink_urb_from_ep(hcd, urb);

		spin_unlock(&xhci->lock);
		usb_hcd_giveback_urb(hcd, urb, status);
		xhci_urb_free_priv(xhci, urb_priv);
		spin_lock(&xhci->lock);
	}
=======
		       struct xhci_td *td, bool flip_cycle)
{
	struct xhci_segment *seg	= td->start_seg;
	union xhci_trb *trb		= td->first_trb;

	while (1) {
		trb_to_noop(trb, TRB_TR_NOOP);

		/* flip cycle if asked to */
		if (flip_cycle && trb != td->first_trb && trb != td->last_trb)
			trb->generic.field[3] ^= cpu_to_le32(TRB_CYCLE);

		if (trb == td->last_trb)
			break;

		next_trb(xhci, ep_ring, &seg, &trb);
	}
}

/*
 * Must be called with xhci->lock held in interrupt context,
 * releases and re-acquires xhci->lock
 */
static void xhci_giveback_urb_in_irq(struct xhci_hcd *xhci,
				     struct xhci_td *cur_td, int status)
{
	struct urb	*urb		= cur_td->urb;
	struct urb_priv	*urb_priv	= urb->hcpriv;
	struct usb_hcd	*hcd		= bus_to_hcd(urb->dev->bus);

	if (usb_pipetype(urb->pipe) == PIPE_ISOCHRONOUS) {
		xhci_to_hcd(xhci)->self.bandwidth_isoc_reqs--;
		if (xhci_to_hcd(xhci)->self.bandwidth_isoc_reqs	== 0) {
			if (xhci->quirks & XHCI_AMD_PLL_FIX)
				usb_amd_quirk_pll_enable();
		}
	}
	xhci_urb_free_priv(urb_priv);
	usb_hcd_unlink_urb_from_ep(hcd, urb);
	trace_xhci_urb_giveback(urb);
	usb_hcd_giveback_urb(hcd, urb, status);
}

static void xhci_unmap_td_bounce_buffer(struct xhci_hcd *xhci,
		struct xhci_ring *ring, struct xhci_td *td)
{
	struct device *dev = xhci_to_hcd(xhci)->self.sysdev;
	struct xhci_segment *seg = td->bounce_seg;
	struct urb *urb = td->urb;
	size_t len;

	if (!ring || !seg || !urb)
		return;

	if (usb_urb_dir_out(urb)) {
		dma_unmap_single(dev, seg->bounce_dma, ring->bounce_buf_len,
				 DMA_TO_DEVICE);
		return;
	}

	dma_unmap_single(dev, seg->bounce_dma, ring->bounce_buf_len,
			 DMA_FROM_DEVICE);
	/* for in tranfers we need to copy the data from bounce to sg */
	if (urb->num_sgs) {
		len = sg_pcopy_from_buffer(urb->sg, urb->num_sgs, seg->bounce_buf,
					   seg->bounce_len, seg->bounce_offs);
		if (len != seg->bounce_len)
			xhci_warn(xhci, "WARN Wrong bounce buffer read length: %zu != %d\n",
				  len, seg->bounce_len);
	} else {
		memcpy(urb->transfer_buffer + seg->bounce_offs, seg->bounce_buf,
		       seg->bounce_len);
	}
	seg->bounce_len = 0;
	seg->bounce_offs = 0;
}

static int xhci_td_cleanup(struct xhci_hcd *xhci, struct xhci_td *td,
			   struct xhci_ring *ep_ring, int status)
{
	struct urb *urb = NULL;

	/* Clean up the endpoint's TD list */
	urb = td->urb;

	/* if a bounce buffer was used to align this td then unmap it */
	xhci_unmap_td_bounce_buffer(xhci, ep_ring, td);

	/* Do one last check of the actual transfer length.
	 * If the host controller said we transferred more data than the buffer
	 * length, urb->actual_length will be a very big number (since it's
	 * unsigned).  Play it safe and say we didn't transfer anything.
	 */
	if (urb->actual_length > urb->transfer_buffer_length) {
		xhci_warn(xhci, "URB req %u and actual %u transfer length mismatch\n",
			  urb->transfer_buffer_length, urb->actual_length);
		urb->actual_length = 0;
		status = 0;
	}
	/* TD might be removed from td_list if we are giving back a cancelled URB */
	if (!list_empty(&td->td_list))
		list_del_init(&td->td_list);
	/* Giving back a cancelled URB, or if a slated TD completed anyway */
	if (!list_empty(&td->cancelled_td_list))
		list_del_init(&td->cancelled_td_list);

	inc_td_cnt(urb);
	/* Giveback the urb when all the tds are completed */
	if (last_td_in_urb(td)) {
		if ((urb->actual_length != urb->transfer_buffer_length &&
		     (urb->transfer_flags & URB_SHORT_NOT_OK)) ||
		    (status != 0 && !usb_endpoint_xfer_isoc(&urb->ep->desc)))
			xhci_dbg(xhci, "Giveback URB %p, len = %d, expected = %d, status = %d\n",
				 urb, urb->actual_length,
				 urb->transfer_buffer_length, status);

		/* set isoc urb status to 0 just as EHCI, UHCI, and OHCI */
		if (usb_pipetype(urb->pipe) == PIPE_ISOCHRONOUS)
			status = 0;
		xhci_giveback_urb_in_irq(xhci, td, status);
	}

	return 0;
}


/* Complete the cancelled URBs we unlinked from td_list. */
static void xhci_giveback_invalidated_tds(struct xhci_virt_ep *ep)
{
	struct xhci_ring *ring;
	struct xhci_td *td, *tmp_td;

	list_for_each_entry_safe(td, tmp_td, &ep->cancelled_td_list,
				 cancelled_td_list) {

		ring = xhci_urb_to_transfer_ring(ep->xhci, td->urb);

		if (td->cancel_status == TD_CLEARED) {
			xhci_dbg(ep->xhci, "%s: Giveback cancelled URB %p TD\n",
				 __func__, td->urb);
			xhci_td_cleanup(ep->xhci, td, ring, td->status);
		} else {
			xhci_dbg(ep->xhci, "%s: Keep cancelled URB %p TD as cancel_status is %d\n",
				 __func__, td->urb, td->cancel_status);
		}
		if (ep->xhci->xhc_state & XHCI_STATE_DYING)
			return;
	}
}

static int xhci_reset_halted_ep(struct xhci_hcd *xhci, unsigned int slot_id,
				unsigned int ep_index, enum xhci_ep_reset_type reset_type)
{
	struct xhci_command *command;
	int ret = 0;

	command = xhci_alloc_command(xhci, false, GFP_ATOMIC);
	if (!command) {
		ret = -ENOMEM;
		goto done;
	}

	xhci_dbg(xhci, "%s-reset ep %u, slot %u\n",
		 (reset_type == EP_HARD_RESET) ? "Hard" : "Soft",
		 ep_index, slot_id);

	ret = xhci_queue_reset_ep(xhci, command, slot_id, ep_index, reset_type);
done:
	if (ret)
		xhci_err(xhci, "ERROR queuing reset endpoint for slot %d ep_index %d, %d\n",
			 slot_id, ep_index, ret);
	return ret;
}

static int xhci_handle_halted_endpoint(struct xhci_hcd *xhci,
				struct xhci_virt_ep *ep,
				struct xhci_td *td,
				enum xhci_ep_reset_type reset_type)
{
	unsigned int slot_id = ep->vdev->slot_id;
	int err;

	/*
	 * Avoid resetting endpoint if link is inactive. Can cause host hang.
	 * Device will be reset soon to recover the link so don't do anything
	 */
	if (ep->vdev->flags & VDEV_PORT_ERROR)
		return -ENODEV;

	/* add td to cancelled list and let reset ep handler take care of it */
	if (reset_type == EP_HARD_RESET) {
		ep->ep_state |= EP_HARD_CLEAR_TOGGLE;
		if (td && list_empty(&td->cancelled_td_list)) {
			list_add_tail(&td->cancelled_td_list, &ep->cancelled_td_list);
			td->cancel_status = TD_HALTED;
		}
	}

	if (ep->ep_state & EP_HALTED) {
		xhci_dbg(xhci, "Reset ep command for ep_index %d already pending\n",
			 ep->ep_index);
		return 0;
	}

	err = xhci_reset_halted_ep(xhci, slot_id, ep->ep_index, reset_type);
	if (err)
		return err;

	ep->ep_state |= EP_HALTED;

	xhci_ring_cmd_db(xhci);

	return 0;
}

/*
 * Fix up the ep ring first, so HW stops executing cancelled TDs.
 * We have the xHCI lock, so nothing can modify this list until we drop it.
 * We're also in the event handler, so we can't get re-interrupted if another
 * Stop Endpoint command completes.
 *
 * only call this when ring is not in a running state
 */

static int xhci_invalidate_cancelled_tds(struct xhci_virt_ep *ep)
{
	struct xhci_hcd		*xhci;
	struct xhci_td		*td = NULL;
	struct xhci_td		*tmp_td = NULL;
	struct xhci_td		*cached_td = NULL;
	struct xhci_ring	*ring;
	u64			hw_deq;
	unsigned int		slot_id = ep->vdev->slot_id;
	int			err;

	xhci = ep->xhci;

	list_for_each_entry_safe(td, tmp_td, &ep->cancelled_td_list, cancelled_td_list) {
		xhci_dbg_trace(xhci, trace_xhci_dbg_cancel_urb,
			       "Removing canceled TD starting at 0x%llx (dma) in stream %u URB %p",
			       (unsigned long long)xhci_trb_virt_to_dma(
				       td->start_seg, td->first_trb),
			       td->urb->stream_id, td->urb);
		list_del_init(&td->td_list);
		ring = xhci_urb_to_transfer_ring(xhci, td->urb);
		if (!ring) {
			xhci_warn(xhci, "WARN Cancelled URB %p has invalid stream ID %u.\n",
				  td->urb, td->urb->stream_id);
			continue;
		}
		/*
		 * If a ring stopped on the TD we need to cancel then we have to
		 * move the xHC endpoint ring dequeue pointer past this TD.
		 * Rings halted due to STALL may show hw_deq is past the stalled
		 * TD, but still require a set TR Deq command to flush xHC cache.
		 */
		hw_deq = xhci_get_hw_deq(xhci, ep->vdev, ep->ep_index,
					 td->urb->stream_id);
		hw_deq &= ~0xf;

		if (td->cancel_status == TD_HALTED ||
		    trb_in_td(xhci, td->start_seg, td->first_trb, td->last_trb, hw_deq, false)) {
			switch (td->cancel_status) {
			case TD_CLEARED: /* TD is already no-op */
			case TD_CLEARING_CACHE: /* set TR deq command already queued */
				break;
			case TD_DIRTY: /* TD is cached, clear it */
			case TD_HALTED:
				td->cancel_status = TD_CLEARING_CACHE;
				if (cached_td)
					/* FIXME  stream case, several stopped rings */
					xhci_dbg(xhci,
						 "Move dq past stream %u URB %p instead of stream %u URB %p\n",
						 td->urb->stream_id, td->urb,
						 cached_td->urb->stream_id, cached_td->urb);
				cached_td = td;
				break;
			}
		} else {
			td_to_noop(xhci, ring, td, false);
			td->cancel_status = TD_CLEARED;
		}
	}

	/* If there's no need to move the dequeue pointer then we're done */
	if (!cached_td)
		return 0;

	err = xhci_move_dequeue_past_td(xhci, slot_id, ep->ep_index,
					cached_td->urb->stream_id,
					cached_td);
	if (err) {
		/* Failed to move past cached td, just set cached TDs to no-op */
		list_for_each_entry_safe(td, tmp_td, &ep->cancelled_td_list, cancelled_td_list) {
			if (td->cancel_status != TD_CLEARING_CACHE)
				continue;
			xhci_dbg(xhci, "Failed to clear cancelled cached URB %p, mark clear anyway\n",
				 td->urb);
			td_to_noop(xhci, ring, td, false);
			td->cancel_status = TD_CLEARED;
		}
	}
	return 0;
}

/*
 * Returns the TD the endpoint ring halted on.
 * Only call for non-running rings without streams.
 */
static struct xhci_td *find_halted_td(struct xhci_virt_ep *ep)
{
	struct xhci_td	*td;
	u64		hw_deq;

	if (!list_empty(&ep->ring->td_list)) { /* Not streams compatible */
		hw_deq = xhci_get_hw_deq(ep->xhci, ep->vdev, ep->ep_index, 0);
		hw_deq &= ~0xf;
		td = list_first_entry(&ep->ring->td_list, struct xhci_td, td_list);
		if (trb_in_td(ep->xhci, td->start_seg, td->first_trb,
				td->last_trb, hw_deq, false))
			return td;
	}
	return NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * When we get a command completion for a Stop Endpoint Command, we need to
 * unlink any cancelled TDs from the ring.  There are two ways to do that:
 *
 *  1. If the HW was in the middle of processing the TD that needs to be
 *     cancelled, then we must move the ring's dequeue pointer past the last TRB
 *     in the TD with a Set Dequeue Pointer Command.
 *  2. Otherwise, we turn all the TRBs in the TD into No-op TRBs (with the chain
 *     bit cleared) so that the HW will skip over them.
 */
<<<<<<< HEAD
static void handle_stopped_endpoint(struct xhci_hcd *xhci,
		union xhci_trb *trb, struct xhci_event_cmd *event)
{
	unsigned int slot_id;
	unsigned int ep_index;
	struct xhci_virt_device *virt_dev;
	struct xhci_ring *ep_ring;
	struct xhci_virt_ep *ep;
	struct list_head *entry;
	struct xhci_td *cur_td = NULL;
	struct xhci_td *last_unlinked_td;

	struct xhci_dequeue_state deq_state;

	if (xhci->main_hcd->driver->set_autosuspend)
		xhci->main_hcd->driver->set_autosuspend(xhci->main_hcd, 1);
	if (unlikely(TRB_TO_SUSPEND_PORT(
			     le32_to_cpu(xhci->cmd_ring->dequeue->generic.field[3])))) {
		slot_id = TRB_TO_SLOT_ID(
			le32_to_cpu(xhci->cmd_ring->dequeue->generic.field[3]));
		virt_dev = xhci->devs[slot_id];
		if (virt_dev)
			handle_cmd_in_cmd_wait_list(xhci, virt_dev,
				event);
		else
			xhci_warn(xhci, "Stop endpoint command "
				"completion for disabled slot %u\n",
				slot_id);
		return;
	}

	memset(&deq_state, 0, sizeof(deq_state));
	slot_id = TRB_TO_SLOT_ID(le32_to_cpu(trb->generic.field[3]));
	ep_index = TRB_TO_EP_INDEX(le32_to_cpu(trb->generic.field[3]));
	ep = &xhci->devs[slot_id]->eps[ep_index];

	if (list_empty(&ep->cancelled_td_list)) {
		xhci_stop_watchdog_timer_in_irq(xhci, ep);
		ep->stopped_td = NULL;
		ep->stopped_trb = NULL;
		ring_doorbell_for_active_rings(xhci, slot_id, ep_index);
		return;
	}

	/* Fix up the ep ring first, so HW stops executing cancelled TDs.
	 * We have the xHCI lock, so nothing can modify this list until we drop
	 * it.  We're also in the event handler, so we can't get re-interrupted
	 * if another Stop Endpoint command completes
	 */
	list_for_each(entry, &ep->cancelled_td_list) {
		cur_td = list_entry(entry, struct xhci_td, cancelled_td_list);
		xhci_dbg(xhci, "Removing canceled TD starting at 0x%llx (dma).\n",
				(unsigned long long)xhci_trb_virt_to_dma(
					cur_td->start_seg, cur_td->first_trb));
		ep_ring = xhci_urb_to_transfer_ring(xhci, cur_td->urb);
		if (!ep_ring) {
			/* This shouldn't happen unless a driver is mucking
			 * with the stream ID after submission.  This will
			 * leave the TD on the hardware ring, and the hardware
			 * will try to execute it, and may access a buffer
			 * that has already been freed.  In the best case, the
			 * hardware will execute it, and the event handler will
			 * ignore the completion event for that TD, since it was
			 * removed from the td_list for that endpoint.  In
			 * short, don't muck with the stream ID after
			 * submission.
			 */
			xhci_warn(xhci, "WARN Cancelled URB %p "
					"has invalid stream ID %u.\n",
					cur_td->urb,
					cur_td->urb->stream_id);
			goto remove_finished_td;
		}
		/*
		 * If we stopped on the TD we need to cancel, then we have to
		 * move the xHC endpoint ring dequeue pointer past this TD.
		 */
		if (cur_td == ep->stopped_td)
			xhci_find_new_dequeue_state(xhci, slot_id, ep_index,
					cur_td->urb->stream_id,
					cur_td, &deq_state);
		else
			td_to_noop(xhci, ep_ring, cur_td, false);
remove_finished_td:
		/*
		 * The event handler won't see a completion for this TD anymore,
		 * so remove it from the endpoint ring's TD list.  Keep it in
		 * the cancelled TD list for URB completion later.
		 */
		list_del_init(&cur_td->td_list);
	}
	last_unlinked_td = cur_td;
	xhci_stop_watchdog_timer_in_irq(xhci, ep);

	/* If necessary, queue a Set Transfer Ring Dequeue Pointer command */
	if (deq_state.new_deq_ptr && deq_state.new_deq_seg) {
		xhci_queue_new_dequeue_state(xhci,
				slot_id, ep_index,
				ep->stopped_td->urb->stream_id,
				&deq_state);
		xhci_ring_cmd_db(xhci);
	} else {
		/* Otherwise ring the doorbell(s) to restart queued transfers */
		ring_doorbell_for_active_rings(xhci, slot_id, ep_index);
	}

	/* Clear stopped_td and stopped_trb if endpoint is not halted */
	if (!(ep->ep_state & EP_HALTED)) {
		ep->stopped_td = NULL;
		ep->stopped_trb = NULL;
	}

	/*
	 * Drop the lock and complete the URBs in the cancelled TD list.
	 * New TDs to be cancelled might be added to the end of the list before
	 * we can complete all the URBs for the TDs we already unlinked.
	 * So stop when we've completed the URB for the last TD we unlinked.
	 */
	do {
		cur_td = list_entry(ep->cancelled_td_list.next,
				struct xhci_td, cancelled_td_list);
		list_del_init(&cur_td->cancelled_td_list);

		/* Clean up the cancelled URB */
		/* Doesn't matter what we pass for status, since the core will
		 * just overwrite it (because the URB has been unlinked).
		 */
		xhci_giveback_urb_in_irq(xhci, cur_td, 0, "cancelled");

		/* Stop processing the cancelled list if the watchdog timer is
		 * running.
		 */
		if (xhci->xhc_state & XHCI_STATE_DYING)
			return;
	} while (cur_td != last_unlinked_td);

	/* Return to the event handler with xhci->lock re-acquired */
}

/* Watchdog timer function for when a stop endpoint command fails to complete.
 * In this case, we assume the host controller is broken or dying or dead.  The
 * host may still be completing some other events, so we have to be careful to
 * let the event ring handler and the URB dequeueing/enqueueing functions know
 * through xhci->state.
 *
 * The timer may also fire if the host takes a very long time to respond to the
 * command, and the stop endpoint command completion handler cannot delete the
 * timer before the timer function is called.  Another endpoint cancellation may
 * sneak in before the timer function can grab the lock, and that may queue
 * another stop endpoint command and add the timer back.  So we cannot use a
 * simple flag to say whether there is a pending stop endpoint command for a
 * particular endpoint.
 *
 * Instead we use a combination of that flag and a counter for the number of
 * pending stop endpoint commands.  If the timer is the tail end of the last
 * stop endpoint command, and the endpoint's command is still pending, we assume
 * the host is dying.
 */
void xhci_stop_endpoint_command_watchdog(unsigned long arg)
{
	struct xhci_hcd *xhci;
	struct xhci_virt_ep *ep;
	struct xhci_virt_ep *temp_ep;
	struct xhci_ring *ring;
	struct xhci_td *cur_td;
	int ret, i, j;
	unsigned long flags;

	ep = (struct xhci_virt_ep *) arg;
	xhci = ep->xhci;

	spin_lock_irqsave(&xhci->lock, flags);

	if (xhci->main_hcd->driver->set_autosuspend)
		xhci->main_hcd->driver->set_autosuspend(xhci->main_hcd, 1);
	ep->stop_cmds_pending--;
	if (xhci->xhc_state & XHCI_STATE_DYING) {
		xhci_dbg(xhci, "Stop EP timer ran, but another timer marked "
				"xHCI as DYING, exiting.\n");
		spin_unlock_irqrestore(&xhci->lock, flags);
		return;
	}
	if (!(ep->stop_cmds_pending == 0 && (ep->ep_state & EP_HALT_PENDING))) {
		xhci_dbg(xhci, "Stop EP timer ran, but no command pending, "
				"exiting.\n");
		spin_unlock_irqrestore(&xhci->lock, flags);
		return;
	}

	xhci_warn(xhci, "xHCI host not responding to stop endpoint command.\n");
	xhci_warn(xhci, "Assuming host is dying, halting host.\n");
	/* Oops, HC is dead or dying or at least not responding to the stop
	 * endpoint command.
	 */
	xhci->xhc_state |= XHCI_STATE_DYING;
	/* Disable interrupts from the host controller and start halting it */
	xhci_quiesce(xhci);
	spin_unlock_irqrestore(&xhci->lock, flags);

	ret = xhci_halt(xhci);

	spin_lock_irqsave(&xhci->lock, flags);
	if (ret < 0) {
		/* This is bad; the host is not responding to commands and it's
		 * not allowing itself to be halted.  At least interrupts are
		 * disabled. If we call usb_hc_died(), it will attempt to
		 * disconnect all device drivers under this host.  Those
		 * disconnect() methods will wait for all URBs to be unlinked,
		 * so we must complete them.
		 */
		xhci_warn(xhci, "Non-responsive xHCI host is not halting.\n");
		xhci_warn(xhci, "Completing active URBs anyway.\n");
		/* We could turn all TDs on the rings to no-ops.  This won't
		 * help if the host has cached part of the ring, and is slow if
		 * we want to preserve the cycle bit.  Skip it and hope the host
		 * doesn't touch the memory.
		 */
	}
	for (i = 0; i < MAX_HC_SLOTS; i++) {
		if (!xhci->devs[i])
			continue;
		for (j = 0; j < 31; j++) {
			temp_ep = &xhci->devs[i]->eps[j];
			ring = temp_ep->ring;
			if (!ring)
				continue;
			xhci_dbg(xhci, "Killing URBs for slot ID %u, "
					"ep index %u\n", i, j);
			while (!list_empty(&ring->td_list)) {
				cur_td = list_first_entry(&ring->td_list,
						struct xhci_td,
						td_list);
				list_del_init(&cur_td->td_list);
				if (!list_empty(&cur_td->cancelled_td_list))
					list_del_init(&cur_td->cancelled_td_list);
				xhci_giveback_urb_in_irq(xhci, cur_td,
						-ESHUTDOWN, "killed");
			}
			while (!list_empty(&temp_ep->cancelled_td_list)) {
				cur_td = list_first_entry(
						&temp_ep->cancelled_td_list,
						struct xhci_td,
						cancelled_td_list);
				list_del_init(&cur_td->cancelled_td_list);
				xhci_giveback_urb_in_irq(xhci, cur_td,
						-ESHUTDOWN, "killed");
			}
		}
	}
	spin_unlock_irqrestore(&xhci->lock, flags);
	xhci_dbg(xhci, "Calling usb_hc_died()\n");
	usb_hc_died(xhci_to_hcd(xhci)->primary_hcd);
	xhci_dbg(xhci, "xHCI host controller is dead.\n");
}

=======
static void xhci_handle_cmd_stop_ep(struct xhci_hcd *xhci, int slot_id,
				    union xhci_trb *trb, u32 comp_code)
{
	unsigned int ep_index;
	struct xhci_virt_ep *ep;
	struct xhci_ep_ctx *ep_ctx;
	struct xhci_td *td = NULL;
	enum xhci_ep_reset_type reset_type;
	struct xhci_command *command;
	int err;

	if (unlikely(TRB_TO_SUSPEND_PORT(le32_to_cpu(trb->generic.field[3])))) {
		if (!xhci->devs[slot_id])
			xhci_warn(xhci, "Stop endpoint command completion for disabled slot %u\n",
				  slot_id);
		return;
	}

	ep_index = TRB_TO_EP_INDEX(le32_to_cpu(trb->generic.field[3]));
	ep = xhci_get_virt_ep(xhci, slot_id, ep_index);
	if (!ep)
		return;

	ep_ctx = xhci_get_ep_ctx(xhci, ep->vdev->out_ctx, ep_index);

	trace_xhci_handle_cmd_stop_ep(ep_ctx);

	if (comp_code == COMP_CONTEXT_STATE_ERROR) {
	/*
	 * If stop endpoint command raced with a halting endpoint we need to
	 * reset the host side endpoint first.
	 * If the TD we halted on isn't cancelled the TD should be given back
	 * with a proper error code, and the ring dequeue moved past the TD.
	 * If streams case we can't find hw_deq, or the TD we halted on so do a
	 * soft reset.
	 *
	 * Proper error code is unknown here, it would be -EPIPE if device side
	 * of enadpoit halted (aka STALL), and -EPROTO if not (transaction error)
	 * We use -EPROTO, if device is stalled it should return a stall error on
	 * next transfer, which then will return -EPIPE, and device side stall is
	 * noted and cleared by class driver.
	 */
		switch (GET_EP_CTX_STATE(ep_ctx)) {
		case EP_STATE_HALTED:
			xhci_dbg(xhci, "Stop ep completion raced with stall, reset ep\n");
			if (ep->ep_state & EP_HAS_STREAMS) {
				reset_type = EP_SOFT_RESET;
			} else {
				reset_type = EP_HARD_RESET;
				td = find_halted_td(ep);
				if (td)
					td->status = -EPROTO;
			}
			/* reset ep, reset handler cleans up cancelled tds */
			err = xhci_handle_halted_endpoint(xhci, ep, td, reset_type);
			if (err)
				break;
			ep->ep_state &= ~EP_STOP_CMD_PENDING;
			return;
		case EP_STATE_STOPPED:
			/*
			 * NEC uPD720200 sometimes sets this state and fails with
			 * Context Error while continuing to process TRBs.
			 * Be conservative and trust EP_CTX_STATE on other chips.
			 */
			if (!(xhci->quirks & XHCI_NEC_HOST))
				break;
			fallthrough;
		case EP_STATE_RUNNING:
			/* Race, HW handled stop ep cmd before ep was running */
			xhci_dbg(xhci, "Stop ep completion ctx error, ep is running\n");

			command = xhci_alloc_command(xhci, false, GFP_ATOMIC);
			if (!command) {
				ep->ep_state &= ~EP_STOP_CMD_PENDING;
				return;
			}
			xhci_queue_stop_endpoint(xhci, command, slot_id, ep_index, 0);
			xhci_ring_cmd_db(xhci);

			return;
		default:
			break;
		}
	}

	/* will queue a set TR deq if stopped on a cancelled, uncleared TD */
	xhci_invalidate_cancelled_tds(ep);
	ep->ep_state &= ~EP_STOP_CMD_PENDING;

	/* Otherwise ring the doorbell(s) to restart queued transfers */
	xhci_giveback_invalidated_tds(ep);
	ring_doorbell_for_active_rings(xhci, slot_id, ep_index);
}

static void xhci_kill_ring_urbs(struct xhci_hcd *xhci, struct xhci_ring *ring)
{
	struct xhci_td *cur_td;
	struct xhci_td *tmp;

	list_for_each_entry_safe(cur_td, tmp, &ring->td_list, td_list) {
		list_del_init(&cur_td->td_list);

		if (!list_empty(&cur_td->cancelled_td_list))
			list_del_init(&cur_td->cancelled_td_list);

		xhci_unmap_td_bounce_buffer(xhci, ring, cur_td);

		inc_td_cnt(cur_td->urb);
		if (last_td_in_urb(cur_td))
			xhci_giveback_urb_in_irq(xhci, cur_td, -ESHUTDOWN);
	}
}

static void xhci_kill_endpoint_urbs(struct xhci_hcd *xhci,
		int slot_id, int ep_index)
{
	struct xhci_td *cur_td;
	struct xhci_td *tmp;
	struct xhci_virt_ep *ep;
	struct xhci_ring *ring;

	ep = xhci_get_virt_ep(xhci, slot_id, ep_index);
	if (!ep)
		return;

	if ((ep->ep_state & EP_HAS_STREAMS) ||
			(ep->ep_state & EP_GETTING_NO_STREAMS)) {
		int stream_id;

		for (stream_id = 1; stream_id < ep->stream_info->num_streams;
				stream_id++) {
			ring = ep->stream_info->stream_rings[stream_id];
			if (!ring)
				continue;

			xhci_dbg_trace(xhci, trace_xhci_dbg_cancel_urb,
					"Killing URBs for slot ID %u, ep index %u, stream %u",
					slot_id, ep_index, stream_id);
			xhci_kill_ring_urbs(xhci, ring);
		}
	} else {
		ring = ep->ring;
		if (!ring)
			return;
		xhci_dbg_trace(xhci, trace_xhci_dbg_cancel_urb,
				"Killing URBs for slot ID %u, ep index %u",
				slot_id, ep_index);
		xhci_kill_ring_urbs(xhci, ring);
	}

	list_for_each_entry_safe(cur_td, tmp, &ep->cancelled_td_list,
			cancelled_td_list) {
		list_del_init(&cur_td->cancelled_td_list);
		inc_td_cnt(cur_td->urb);

		if (last_td_in_urb(cur_td))
			xhci_giveback_urb_in_irq(xhci, cur_td, -ESHUTDOWN);
	}
}

/*
 * host controller died, register read returns 0xffffffff
 * Complete pending commands, mark them ABORTED.
 * URBs need to be given back as usb core might be waiting with device locks
 * held for the URBs to finish during device disconnect, blocking host remove.
 *
 * Call with xhci->lock held.
 * lock is relased and re-acquired while giving back urb.
 */
void xhci_hc_died(struct xhci_hcd *xhci)
{
	int i, j;

	if (xhci->xhc_state & XHCI_STATE_DYING)
		return;

	xhci_err(xhci, "xHCI host controller not responding, assume dead\n");
	xhci->xhc_state |= XHCI_STATE_DYING;

	xhci_cleanup_command_queue(xhci);

	/* return any pending urbs, remove may be waiting for them */
	for (i = 0; i <= HCS_MAX_SLOTS(xhci->hcs_params1); i++) {
		if (!xhci->devs[i])
			continue;
		for (j = 0; j < 31; j++)
			xhci_kill_endpoint_urbs(xhci, i, j);
	}

	/* inform usb core hc died if PCI remove isn't already handling it */
	if (!(xhci->xhc_state & XHCI_STATE_REMOVING))
		usb_hc_died(xhci_to_hcd(xhci));
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static void update_ring_for_set_deq_completion(struct xhci_hcd *xhci,
		struct xhci_virt_device *dev,
		struct xhci_ring *ep_ring,
		unsigned int ep_index)
{
	union xhci_trb *dequeue_temp;
<<<<<<< HEAD
	int num_trbs_free_temp;
	bool revert = false;

	num_trbs_free_temp = ep_ring->num_trbs_free;
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dequeue_temp = ep_ring->dequeue;

	/* If we get two back-to-back stalls, and the first stalled transfer
	 * ends just before a link TRB, the dequeue pointer will be left on
	 * the link TRB by the code in the while loop.  So we have to update
	 * the dequeue pointer one segment further, or we'll jump off
	 * the segment into la-la-land.
	 */
<<<<<<< HEAD
	if (last_trb(xhci, ep_ring, ep_ring->deq_seg, ep_ring->dequeue)) {
=======
	if (trb_is_link(ep_ring->dequeue)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ep_ring->deq_seg = ep_ring->deq_seg->next;
		ep_ring->dequeue = ep_ring->deq_seg->trbs;
	}

	while (ep_ring->dequeue != dev->eps[ep_index].queued_deq_ptr) {
		/* We have more usable TRBs */
<<<<<<< HEAD
		ep_ring->num_trbs_free++;
		ep_ring->dequeue++;
		if (last_trb(xhci, ep_ring, ep_ring->deq_seg,
				ep_ring->dequeue)) {
=======
		ep_ring->dequeue++;
		if (trb_is_link(ep_ring->dequeue)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (ep_ring->dequeue ==
					dev->eps[ep_index].queued_deq_ptr)
				break;
			ep_ring->deq_seg = ep_ring->deq_seg->next;
			ep_ring->dequeue = ep_ring->deq_seg->trbs;
		}
		if (ep_ring->dequeue == dequeue_temp) {
<<<<<<< HEAD
			revert = true;
			break;
		}
	}

	if (revert) {
		xhci_dbg(xhci, "Unable to find new dequeue pointer\n");
		ep_ring->num_trbs_free = num_trbs_free_temp;
	}
=======
			xhci_dbg(xhci, "Unable to find new dequeue pointer\n");
			break;
		}
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * When we get a completion for a Set Transfer Ring Dequeue Pointer command,
 * we need to clear the set deq pending flag in the endpoint ring state, so that
 * the TD queueing code can ring the doorbell again.  We also need to ring the
 * endpoint doorbell to restart the ring, but only if there aren't more
 * cancellations pending.
 */
<<<<<<< HEAD
static void handle_set_deq_completion(struct xhci_hcd *xhci,
		struct xhci_event_cmd *event,
		union xhci_trb *trb)
{
	unsigned int slot_id;
	unsigned int ep_index;
	unsigned int stream_id;
	struct xhci_ring *ep_ring;
	struct xhci_virt_device *dev;
	struct xhci_ep_ctx *ep_ctx;
	struct xhci_slot_ctx *slot_ctx;

	slot_id = TRB_TO_SLOT_ID(le32_to_cpu(trb->generic.field[3]));
	ep_index = TRB_TO_EP_INDEX(le32_to_cpu(trb->generic.field[3]));
	stream_id = TRB_TO_STREAM_ID(le32_to_cpu(trb->generic.field[2]));
	dev = xhci->devs[slot_id];

	ep_ring = xhci_stream_id_to_ring(dev, ep_index, stream_id);
	if (!ep_ring) {
		xhci_warn(xhci, "WARN Set TR deq ptr command for "
				"freed stream ID %u\n",
				stream_id);
		/* XXX: Harmless??? */
		dev->eps[ep_index].ep_state &= ~SET_DEQ_PENDING;
		return;
	}

	ep_ctx = xhci_get_ep_ctx(xhci, dev->out_ctx, ep_index);
	slot_ctx = xhci_get_slot_ctx(xhci, dev->out_ctx);

	if (GET_COMP_CODE(le32_to_cpu(event->status)) != COMP_SUCCESS) {
		unsigned int ep_state;
		unsigned int slot_state;

		switch (GET_COMP_CODE(le32_to_cpu(event->status))) {
		case COMP_TRB_ERR:
			xhci_warn(xhci, "WARN Set TR Deq Ptr cmd invalid because "
					"of stream ID configuration\n");
			break;
		case COMP_CTX_STATE:
			xhci_warn(xhci, "WARN Set TR Deq Ptr cmd failed due "
					"to incorrect slot or ep state.\n");
			ep_state = le32_to_cpu(ep_ctx->ep_info);
			ep_state &= EP_STATE_MASK;
			slot_state = le32_to_cpu(slot_ctx->dev_state);
			slot_state = GET_SLOT_STATE(slot_state);
			xhci_dbg(xhci, "Slot state = %u, EP state = %u\n",
					slot_state, ep_state);
			break;
		case COMP_EBADSLT:
			xhci_warn(xhci, "WARN Set TR Deq Ptr cmd failed because "
					"slot %u was not enabled.\n", slot_id);
			break;
		default:
			xhci_warn(xhci, "WARN Set TR Deq Ptr cmd with unknown "
					"completion code of %u.\n",
				  GET_COMP_CODE(le32_to_cpu(event->status)));
=======
static void xhci_handle_cmd_set_deq(struct xhci_hcd *xhci, int slot_id,
		union xhci_trb *trb, u32 cmd_comp_code)
{
	unsigned int ep_index;
	unsigned int stream_id;
	struct xhci_ring *ep_ring;
	struct xhci_virt_ep *ep;
	struct xhci_ep_ctx *ep_ctx;
	struct xhci_slot_ctx *slot_ctx;
	struct xhci_td *td, *tmp_td;

	ep_index = TRB_TO_EP_INDEX(le32_to_cpu(trb->generic.field[3]));
	stream_id = TRB_TO_STREAM_ID(le32_to_cpu(trb->generic.field[2]));
	ep = xhci_get_virt_ep(xhci, slot_id, ep_index);
	if (!ep)
		return;

	ep_ring = xhci_virt_ep_to_ring(xhci, ep, stream_id);
	if (!ep_ring) {
		xhci_warn(xhci, "WARN Set TR deq ptr command for freed stream ID %u\n",
				stream_id);
		/* XXX: Harmless??? */
		goto cleanup;
	}

	ep_ctx = xhci_get_ep_ctx(xhci, ep->vdev->out_ctx, ep_index);
	slot_ctx = xhci_get_slot_ctx(xhci, ep->vdev->out_ctx);
	trace_xhci_handle_cmd_set_deq(slot_ctx);
	trace_xhci_handle_cmd_set_deq_ep(ep_ctx);

	if (cmd_comp_code != COMP_SUCCESS) {
		unsigned int ep_state;
		unsigned int slot_state;

		switch (cmd_comp_code) {
		case COMP_TRB_ERROR:
			xhci_warn(xhci, "WARN Set TR Deq Ptr cmd invalid because of stream ID configuration\n");
			break;
		case COMP_CONTEXT_STATE_ERROR:
			xhci_warn(xhci, "WARN Set TR Deq Ptr cmd failed due to incorrect slot or ep state.\n");
			ep_state = GET_EP_CTX_STATE(ep_ctx);
			slot_state = le32_to_cpu(slot_ctx->dev_state);
			slot_state = GET_SLOT_STATE(slot_state);
			xhci_dbg_trace(xhci, trace_xhci_dbg_cancel_urb,
					"Slot state = %u, EP state = %u",
					slot_state, ep_state);
			break;
		case COMP_SLOT_NOT_ENABLED_ERROR:
			xhci_warn(xhci, "WARN Set TR Deq Ptr cmd failed because slot %u was not enabled.\n",
					slot_id);
			break;
		default:
			xhci_warn(xhci, "WARN Set TR Deq Ptr cmd with unknown completion code of %u.\n",
					cmd_comp_code);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		}
		/* OK what do we do now?  The endpoint state is hosed, and we
		 * should never get to this point if the synchronization between
		 * queueing, and endpoint state are correct.  This might happen
		 * if the device gets disconnected after we've finished
		 * cancelling URBs, which might not be an error...
		 */
	} else {
<<<<<<< HEAD
		xhci_dbg(xhci, "Successful Set TR Deq Ptr cmd, deq = @%08llx\n",
			 le64_to_cpu(ep_ctx->deq));
		if (xhci_trb_virt_to_dma(dev->eps[ep_index].queued_deq_seg,
					 dev->eps[ep_index].queued_deq_ptr) ==
		    (le64_to_cpu(ep_ctx->deq) & ~(EP_CTX_CYCLE_MASK))) {
			/* Update the ring's dequeue segment and dequeue pointer
			 * to reflect the new position.
			 */
			update_ring_for_set_deq_completion(xhci, dev,
				ep_ring, ep_index);
		} else {
			xhci_warn(xhci, "Mismatch between completed Set TR Deq "
					"Ptr command & xHCI internal state.\n");
			xhci_warn(xhci, "ep deq seg = %p, deq ptr = %p\n",
					dev->eps[ep_index].queued_deq_seg,
					dev->eps[ep_index].queued_deq_ptr);
		}
	}

	dev->eps[ep_index].ep_state &= ~SET_DEQ_PENDING;
	dev->eps[ep_index].queued_deq_seg = NULL;
	dev->eps[ep_index].queued_deq_ptr = NULL;
=======
		u64 deq;
		/* 4.6.10 deq ptr is written to the stream ctx for streams */
		if (ep->ep_state & EP_HAS_STREAMS) {
			struct xhci_stream_ctx *ctx =
				&ep->stream_info->stream_ctx_array[stream_id];
			deq = le64_to_cpu(ctx->stream_ring) & SCTX_DEQ_MASK;
		} else {
			deq = le64_to_cpu(ep_ctx->deq) & ~EP_CTX_CYCLE_MASK;
		}
		xhci_dbg_trace(xhci, trace_xhci_dbg_cancel_urb,
			"Successful Set TR Deq Ptr cmd, deq = @%08llx", deq);
		if (xhci_trb_virt_to_dma(ep->queued_deq_seg,
					 ep->queued_deq_ptr) == deq) {
			/* Update the ring's dequeue segment and dequeue pointer
			 * to reflect the new position.
			 */
			update_ring_for_set_deq_completion(xhci, ep->vdev,
				ep_ring, ep_index);
		} else {
			xhci_warn(xhci, "Mismatch between completed Set TR Deq Ptr command & xHCI internal state.\n");
			xhci_warn(xhci, "ep deq seg = %p, deq ptr = %p\n",
				  ep->queued_deq_seg, ep->queued_deq_ptr);
		}
	}
	/* HW cached TDs cleared from cache, give them back */
	list_for_each_entry_safe(td, tmp_td, &ep->cancelled_td_list,
				 cancelled_td_list) {
		ep_ring = xhci_urb_to_transfer_ring(ep->xhci, td->urb);
		if (td->cancel_status == TD_CLEARING_CACHE) {
			td->cancel_status = TD_CLEARED;
			xhci_dbg(ep->xhci, "%s: Giveback cancelled URB %p TD\n",
				 __func__, td->urb);
			xhci_td_cleanup(ep->xhci, td, ep_ring, td->status);
		} else {
			xhci_dbg(ep->xhci, "%s: Keep cancelled URB %p TD as cancel_status is %d\n",
				 __func__, td->urb, td->cancel_status);
		}
	}
cleanup:
	ep->ep_state &= ~SET_DEQ_PENDING;
	ep->queued_deq_seg = NULL;
	ep->queued_deq_ptr = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Restart any rings with pending URBs */
	ring_doorbell_for_active_rings(xhci, slot_id, ep_index);
}

<<<<<<< HEAD
static void handle_reset_ep_completion(struct xhci_hcd *xhci,
		struct xhci_event_cmd *event,
		union xhci_trb *trb)
{
	int slot_id;
	unsigned int ep_index;

	slot_id = TRB_TO_SLOT_ID(le32_to_cpu(trb->generic.field[3]));
	ep_index = TRB_TO_EP_INDEX(le32_to_cpu(trb->generic.field[3]));
	/* This command will only fail if the endpoint wasn't halted,
	 * but we don't care.
	 */
	xhci_dbg(xhci, "Ignoring reset ep completion code of %u\n",
		 GET_COMP_CODE(le32_to_cpu(event->status)));

	/* HW with the reset endpoint quirk needs to have a configure endpoint
	 * command complete before the endpoint can be used.  Queue that here
	 * because the HW can't handle two commands being queued in a row.
	 */
	if (xhci->quirks & XHCI_RESET_EP_QUIRK) {
		xhci_dbg(xhci, "Queueing configure endpoint command\n");
		xhci_queue_configure_endpoint(xhci,
				xhci->devs[slot_id]->in_ctx->dma, slot_id,
				false);
		xhci_ring_cmd_db(xhci);
	} else {
		/* Clear our internal halted state */
		xhci->devs[slot_id]->eps[ep_index].ep_state &= ~EP_HALTED;
	}
}

/* Complete the command and detele it from the devcie's command queue.
 */
static void xhci_complete_cmd_in_cmd_wait_list(struct xhci_hcd *xhci,
		struct xhci_command *command, u32 status)
{
	command->status = status;
	list_del(&command->cmd_list);
	if (command->completion)
		complete(command->completion);
	else
		xhci_free_command(xhci, command);
}


/* Check to see if a command in the device's command queue matches this one.
 * Signal the completion or free the command, and return 1.  Return 0 if the
 * completed command isn't at the head of the command list.
 */
static int handle_cmd_in_cmd_wait_list(struct xhci_hcd *xhci,
		struct xhci_virt_device *virt_dev,
		struct xhci_event_cmd *event)
{
	struct xhci_command *command;

	if (list_empty(&virt_dev->cmd_list))
		return 0;

	command = list_entry(virt_dev->cmd_list.next,
			struct xhci_command, cmd_list);
	if (xhci->cmd_ring->dequeue != command->command_trb)
		return 0;

	xhci_complete_cmd_in_cmd_wait_list(xhci, command,
			GET_COMP_CODE(le32_to_cpu(event->status)));
	return 1;
}

/*
 * Finding the command trb need to be cancelled and modifying it to
 * NO OP command. And if the command is in device's command wait
 * list, finishing and freeing it.
 *
 * If we can't find the command trb, we think it had already been
 * executed.
 */
static void xhci_cmd_to_noop(struct xhci_hcd *xhci, struct xhci_cd *cur_cd)
{
	struct xhci_segment *cur_seg;
	union xhci_trb *cmd_trb;
	u32 cycle_state;

	if (xhci->cmd_ring->dequeue == xhci->cmd_ring->enqueue)
		return;

	/* find the current segment of command ring */
	cur_seg = find_trb_seg(xhci->cmd_ring->first_seg,
			xhci->cmd_ring->dequeue, &cycle_state);

	if (!cur_seg) {
		xhci_warn(xhci, "Command ring mismatch, dequeue = %p %llx (dma)\n",
				xhci->cmd_ring->dequeue,
				(unsigned long long)
				xhci_trb_virt_to_dma(xhci->cmd_ring->deq_seg,
					xhci->cmd_ring->dequeue));
		xhci_debug_ring(xhci, xhci->cmd_ring);
		xhci_dbg_ring_ptrs(xhci, xhci->cmd_ring);
		return;
	}

	/* find the command trb matched by cd from command ring */
	for (cmd_trb = xhci->cmd_ring->dequeue;
			cmd_trb != xhci->cmd_ring->enqueue;
			next_trb(xhci, xhci->cmd_ring, &cur_seg, &cmd_trb)) {
		/* If the trb is link trb, continue */
		if (TRB_TYPE_LINK_LE32(cmd_trb->generic.field[3]))
			continue;

		if (cur_cd->cmd_trb == cmd_trb) {

			/* If the command in device's command list, we should
			 * finish it and free the command structure.
			 */
			if (cur_cd->command)
				xhci_complete_cmd_in_cmd_wait_list(xhci,
					cur_cd->command, COMP_CMD_STOP);

			/* get cycle state from the origin command trb */
			cycle_state = le32_to_cpu(cmd_trb->generic.field[3])
				& TRB_CYCLE;

			/* modify the command trb to NO OP command */
			cmd_trb->generic.field[0] = 0;
			cmd_trb->generic.field[1] = 0;
			cmd_trb->generic.field[2] = 0;
			cmd_trb->generic.field[3] = cpu_to_le32(
					TRB_TYPE(TRB_CMD_NOOP) | cycle_state);
			break;
		}
	}
}

static void xhci_cancel_cmd_in_cd_list(struct xhci_hcd *xhci)
{
	struct xhci_cd *cur_cd, *next_cd;

	if (list_empty(&xhci->cancel_cmd_list))
		return;

	list_for_each_entry_safe(cur_cd, next_cd,
			&xhci->cancel_cmd_list, cancel_cmd_list) {
		xhci_cmd_to_noop(xhci, cur_cd);
		list_del(&cur_cd->cancel_cmd_list);
		kfree(cur_cd);
	}
}

/*
 * traversing the cancel_cmd_list. If the command descriptor according
 * to cmd_trb is found, the function free it and return 1, otherwise
 * return 0.
 */
static int xhci_search_cmd_trb_in_cd_list(struct xhci_hcd *xhci,
		union xhci_trb *cmd_trb)
{
	struct xhci_cd *cur_cd, *next_cd;

	if (list_empty(&xhci->cancel_cmd_list))
		return 0;

	list_for_each_entry_safe(cur_cd, next_cd,
			&xhci->cancel_cmd_list, cancel_cmd_list) {
		if (cur_cd->cmd_trb == cmd_trb) {
			if (cur_cd->command)
				xhci_complete_cmd_in_cmd_wait_list(xhci,
					cur_cd->command, COMP_CMD_STOP);
			list_del(&cur_cd->cancel_cmd_list);
			kfree(cur_cd);
			return 1;
		}
	}

	return 0;
}

/*
 * If the cmd_trb_comp_code is COMP_CMD_ABORT, we just check whether the
 * trb pointed by the command ring dequeue pointer is the trb we want to
 * cancel or not. And if the cmd_trb_comp_code is COMP_CMD_STOP, we will
 * traverse the cancel_cmd_list to trun the all of the commands according
 * to command descriptor to NO-OP trb.
 */
static int handle_stopped_cmd_ring(struct xhci_hcd *xhci,
		int cmd_trb_comp_code)
{
	int cur_trb_is_good = 0;

	/* Searching the cmd trb pointed by the command ring dequeue
	 * pointer in command descriptor list. If it is found, free it.
	 */
	cur_trb_is_good = xhci_search_cmd_trb_in_cd_list(xhci,
			xhci->cmd_ring->dequeue);

	if (cmd_trb_comp_code == COMP_CMD_ABORT)
		xhci->cmd_ring_state = CMD_RING_STATE_STOPPED;
	else if (cmd_trb_comp_code == COMP_CMD_STOP) {
		/* traversing the cancel_cmd_list and canceling
		 * the command according to command descriptor
		 */
		xhci_cancel_cmd_in_cd_list(xhci);

		xhci->cmd_ring_state = CMD_RING_STATE_RUNNING;
		/*
		 * ring command ring doorbell again to restart the
		 * command ring
		 */
		if (xhci->cmd_ring->dequeue != xhci->cmd_ring->enqueue)
			xhci_ring_cmd_db(xhci);
	}
	return cur_trb_is_good;
=======
static void xhci_handle_cmd_reset_ep(struct xhci_hcd *xhci, int slot_id,
		union xhci_trb *trb, u32 cmd_comp_code)
{
	struct xhci_virt_ep *ep;
	struct xhci_ep_ctx *ep_ctx;
	unsigned int ep_index;

	ep_index = TRB_TO_EP_INDEX(le32_to_cpu(trb->generic.field[3]));
	ep = xhci_get_virt_ep(xhci, slot_id, ep_index);
	if (!ep)
		return;

	ep_ctx = xhci_get_ep_ctx(xhci, ep->vdev->out_ctx, ep_index);
	trace_xhci_handle_cmd_reset_ep(ep_ctx);

	/* This command will only fail if the endpoint wasn't halted,
	 * but we don't care.
	 */
	xhci_dbg_trace(xhci, trace_xhci_dbg_reset_ep,
		"Ignoring reset ep completion code of %u", cmd_comp_code);

	/* Cleanup cancelled TDs as ep is stopped. May queue a Set TR Deq cmd */
	xhci_invalidate_cancelled_tds(ep);

	/* Clear our internal halted state */
	ep->ep_state &= ~EP_HALTED;

	xhci_giveback_invalidated_tds(ep);

	/* if this was a soft reset, then restart */
	if ((le32_to_cpu(trb->generic.field[3])) & TRB_TSP)
		ring_doorbell_for_active_rings(xhci, slot_id, ep_index);
}

static void xhci_handle_cmd_enable_slot(struct xhci_hcd *xhci, int slot_id,
		struct xhci_command *command, u32 cmd_comp_code)
{
	if (cmd_comp_code == COMP_SUCCESS)
		command->slot_id = slot_id;
	else
		command->slot_id = 0;
}

static void xhci_handle_cmd_disable_slot(struct xhci_hcd *xhci, int slot_id)
{
	struct xhci_virt_device *virt_dev;
	struct xhci_slot_ctx *slot_ctx;

	virt_dev = xhci->devs[slot_id];
	if (!virt_dev)
		return;

	slot_ctx = xhci_get_slot_ctx(xhci, virt_dev->out_ctx);
	trace_xhci_handle_cmd_disable_slot(slot_ctx);

	if (xhci->quirks & XHCI_EP_LIMIT_QUIRK)
		/* Delete default control endpoint resources */
		xhci_free_device_endpoint_resources(xhci, virt_dev, true);
}

static void xhci_handle_cmd_config_ep(struct xhci_hcd *xhci, int slot_id,
		u32 cmd_comp_code)
{
	struct xhci_virt_device *virt_dev;
	struct xhci_input_control_ctx *ctrl_ctx;
	struct xhci_ep_ctx *ep_ctx;
	unsigned int ep_index;
	u32 add_flags;

	/*
	 * Configure endpoint commands can come from the USB core configuration
	 * or alt setting changes, or when streams were being configured.
	 */

	virt_dev = xhci->devs[slot_id];
	if (!virt_dev)
		return;
	ctrl_ctx = xhci_get_input_control_ctx(virt_dev->in_ctx);
	if (!ctrl_ctx) {
		xhci_warn(xhci, "Could not get input context, bad type.\n");
		return;
	}

	add_flags = le32_to_cpu(ctrl_ctx->add_flags);

	/* Input ctx add_flags are the endpoint index plus one */
	ep_index = xhci_last_valid_endpoint(add_flags) - 1;

	ep_ctx = xhci_get_ep_ctx(xhci, virt_dev->out_ctx, ep_index);
	trace_xhci_handle_cmd_config_ep(ep_ctx);

	return;
}

static void xhci_handle_cmd_addr_dev(struct xhci_hcd *xhci, int slot_id)
{
	struct xhci_virt_device *vdev;
	struct xhci_slot_ctx *slot_ctx;

	vdev = xhci->devs[slot_id];
	if (!vdev)
		return;
	slot_ctx = xhci_get_slot_ctx(xhci, vdev->out_ctx);
	trace_xhci_handle_cmd_addr_dev(slot_ctx);
}

static void xhci_handle_cmd_reset_dev(struct xhci_hcd *xhci, int slot_id)
{
	struct xhci_virt_device *vdev;
	struct xhci_slot_ctx *slot_ctx;

	vdev = xhci->devs[slot_id];
	if (!vdev) {
		xhci_warn(xhci, "Reset device command completion for disabled slot %u\n",
			  slot_id);
		return;
	}
	slot_ctx = xhci_get_slot_ctx(xhci, vdev->out_ctx);
	trace_xhci_handle_cmd_reset_dev(slot_ctx);

	xhci_dbg(xhci, "Completed reset device command.\n");
}

static void xhci_handle_cmd_nec_get_fw(struct xhci_hcd *xhci,
		struct xhci_event_cmd *event)
{
	if (!(xhci->quirks & XHCI_NEC_HOST)) {
		xhci_warn(xhci, "WARN NEC_GET_FW command on non-NEC host\n");
		return;
	}
	xhci_dbg_trace(xhci, trace_xhci_dbg_quirks,
			"NEC firmware version %2x.%02x",
			NEC_FW_MAJOR(le32_to_cpu(event->status)),
			NEC_FW_MINOR(le32_to_cpu(event->status)));
}

static void xhci_complete_del_and_free_cmd(struct xhci_command *cmd, u32 status)
{
	list_del(&cmd->cmd_list);

	if (cmd->completion) {
		cmd->status = status;
		complete(cmd->completion);
	} else {
		kfree(cmd);
	}
}

void xhci_cleanup_command_queue(struct xhci_hcd *xhci)
{
	struct xhci_command *cur_cmd, *tmp_cmd;
	xhci->current_cmd = NULL;
	list_for_each_entry_safe(cur_cmd, tmp_cmd, &xhci->cmd_list, cmd_list)
		xhci_complete_del_and_free_cmd(cur_cmd, COMP_COMMAND_ABORTED);
}

void xhci_handle_command_timeout(struct work_struct *work)
{
	struct xhci_hcd	*xhci;
	unsigned long	flags;
	char		str[XHCI_MSG_MAX];
	u64		hw_ring_state;
	u32		cmd_field3;
	u32		usbsts;

	xhci = container_of(to_delayed_work(work), struct xhci_hcd, cmd_timer);

	spin_lock_irqsave(&xhci->lock, flags);

	/*
	 * If timeout work is pending, or current_cmd is NULL, it means we
	 * raced with command completion. Command is handled so just return.
	 */
	if (!xhci->current_cmd || delayed_work_pending(&xhci->cmd_timer)) {
		spin_unlock_irqrestore(&xhci->lock, flags);
		return;
	}

	cmd_field3 = le32_to_cpu(xhci->current_cmd->command_trb->generic.field[3]);
	usbsts = readl(&xhci->op_regs->status);
	xhci_dbg(xhci, "Command timeout, USBSTS:%s\n", xhci_decode_usbsts(str, usbsts));

	/* Bail out and tear down xhci if a stop endpoint command failed */
	if (TRB_FIELD_TO_TYPE(cmd_field3) == TRB_STOP_RING) {
		struct xhci_virt_ep	*ep;

		xhci_warn(xhci, "xHCI host not responding to stop endpoint command\n");

		ep = xhci_get_virt_ep(xhci, TRB_TO_SLOT_ID(cmd_field3),
				      TRB_TO_EP_INDEX(cmd_field3));
		if (ep)
			ep->ep_state &= ~EP_STOP_CMD_PENDING;

		xhci_halt(xhci);
		xhci_hc_died(xhci);
		goto time_out_completed;
	}

	/* mark this command to be cancelled */
	xhci->current_cmd->status = COMP_COMMAND_ABORTED;

	/* Make sure command ring is running before aborting it */
	hw_ring_state = xhci_read_64(xhci, &xhci->op_regs->cmd_ring);
	if (hw_ring_state == ~(u64)0) {
		xhci_hc_died(xhci);
		goto time_out_completed;
	}

	if ((xhci->cmd_ring_state & CMD_RING_STATE_RUNNING) &&
	    (hw_ring_state & CMD_RING_RUNNING))  {
		/* Prevent new doorbell, and start command abort */
		xhci->cmd_ring_state = CMD_RING_STATE_ABORTED;
		xhci_dbg(xhci, "Command timeout\n");
		xhci_abort_cmd_ring(xhci, flags);
		goto time_out_completed;
	}

	/* host removed. Bail out */
	if (xhci->xhc_state & XHCI_STATE_REMOVING) {
		xhci_dbg(xhci, "host removed, ring start fail?\n");
		xhci_cleanup_command_queue(xhci);

		goto time_out_completed;
	}

	/* command timeout on stopped ring, ring can't be aborted */
	xhci_dbg(xhci, "Command timeout on stopped ring\n");
	xhci_handle_stopped_cmd_ring(xhci, xhci->current_cmd);

time_out_completed:
	spin_unlock_irqrestore(&xhci->lock, flags);
	return;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void handle_cmd_completion(struct xhci_hcd *xhci,
		struct xhci_event_cmd *event)
{
<<<<<<< HEAD
	int slot_id = TRB_TO_SLOT_ID(le32_to_cpu(event->flags));
	u64 cmd_dma;
	dma_addr_t cmd_dequeue_dma;
	struct xhci_input_control_ctx *ctrl_ctx;
	struct xhci_virt_device *virt_dev;
	unsigned int ep_index;
	struct xhci_ring *ep_ring;
	unsigned int ep_state;

	cmd_dma = le64_to_cpu(event->cmd_trb);
	cmd_dequeue_dma = xhci_trb_virt_to_dma(xhci->cmd_ring->deq_seg,
			xhci->cmd_ring->dequeue);
	/* Is the command ring deq ptr out of sync with the deq seg ptr? */
	if (cmd_dequeue_dma == 0) {
		xhci->error_bitmask |= 1 << 4;
		return;
	}
	/* Does the DMA address match our internal dequeue pointer address? */
	if (cmd_dma != (u64) cmd_dequeue_dma) {
		xhci->error_bitmask |= 1 << 5;
		return;
	}

	if ((GET_COMP_CODE(le32_to_cpu(event->status)) == COMP_CMD_ABORT) ||
		(GET_COMP_CODE(le32_to_cpu(event->status)) == COMP_CMD_STOP)) {
		/* If the return value is 0, we think the trb pointed by
		 * command ring dequeue pointer is a good trb. The good
		 * trb means we don't want to cancel the trb, but it have
		 * been stopped by host. So we should handle it normally.
		 * Otherwise, driver should invoke inc_deq() and return.
		 */
		if (handle_stopped_cmd_ring(xhci,
				GET_COMP_CODE(le32_to_cpu(event->status)))) {
			inc_deq(xhci, xhci->cmd_ring);
			return;
		}
		/* There is no command to handle if we get a stop event when the
		 * command ring is empty, event->cmd_trb points to the next
		 * unset command
		 */
		if (xhci->cmd_ring->dequeue == xhci->cmd_ring->enqueue)
			return;
	}

	switch (le32_to_cpu(xhci->cmd_ring->dequeue->generic.field[3])
		& TRB_TYPE_BITMASK) {
	case TRB_TYPE(TRB_ENABLE_SLOT):
		if (GET_COMP_CODE(le32_to_cpu(event->status)) == COMP_SUCCESS)
			xhci->slot_id = slot_id;
		else
			xhci->slot_id = 0;
		complete(&xhci->addr_dev);
		break;
	case TRB_TYPE(TRB_DISABLE_SLOT):
		if (xhci->devs[slot_id]) {
			if (xhci->quirks & XHCI_EP_LIMIT_QUIRK)
				/* Delete default control endpoint resources */
				xhci_free_device_endpoint_resources(xhci,
						xhci->devs[slot_id], true);
			xhci_free_virt_device(xhci, slot_id);
		}
		break;
	case TRB_TYPE(TRB_CONFIG_EP):
		virt_dev = xhci->devs[slot_id];
		if (handle_cmd_in_cmd_wait_list(xhci, virt_dev, event))
			break;
		/*
		 * Configure endpoint commands can come from the USB core
		 * configuration or alt setting changes, or because the HW
		 * needed an extra configure endpoint command after a reset
		 * endpoint command or streams were being configured.
		 * If the command was for a halted endpoint, the xHCI driver
		 * is not waiting on the configure endpoint command.
		 */
		ctrl_ctx = xhci_get_input_control_ctx(xhci,
				virt_dev->in_ctx);
		/* Input ctx add_flags are the endpoint index plus one */
		ep_index = xhci_last_valid_endpoint(le32_to_cpu(ctrl_ctx->add_flags)) - 1;
		/* A usb_set_interface() call directly after clearing a halted
		 * condition may race on this quirky hardware.  Not worth
		 * worrying about, since this is prototype hardware.  Not sure
		 * if this will work for streams, but streams support was
		 * untested on this prototype.
		 */
		if (xhci->quirks & XHCI_RESET_EP_QUIRK &&
				ep_index != (unsigned int) -1 &&
		    le32_to_cpu(ctrl_ctx->add_flags) - SLOT_FLAG ==
		    le32_to_cpu(ctrl_ctx->drop_flags)) {
			ep_ring = xhci->devs[slot_id]->eps[ep_index].ring;
			ep_state = xhci->devs[slot_id]->eps[ep_index].ep_state;
			if (!(ep_state & EP_HALTED))
				goto bandwidth_change;
			xhci_dbg(xhci, "Completed config ep cmd - "
					"last ep index = %d, state = %d\n",
					ep_index, ep_state);
			/* Clear internal halted state and restart ring(s) */
			xhci->devs[slot_id]->eps[ep_index].ep_state &=
				~EP_HALTED;
			ring_doorbell_for_active_rings(xhci, slot_id, ep_index);
			break;
		}
bandwidth_change:
		xhci_dbg(xhci, "Completed config ep cmd\n");
		xhci->devs[slot_id]->cmd_status =
			GET_COMP_CODE(le32_to_cpu(event->status));
		complete(&xhci->devs[slot_id]->cmd_completion);
		break;
	case TRB_TYPE(TRB_EVAL_CONTEXT):
		virt_dev = xhci->devs[slot_id];
		if (handle_cmd_in_cmd_wait_list(xhci, virt_dev, event))
			break;
		xhci->devs[slot_id]->cmd_status = GET_COMP_CODE(le32_to_cpu(event->status));
		complete(&xhci->devs[slot_id]->cmd_completion);
		break;
	case TRB_TYPE(TRB_ADDR_DEV):
		xhci->devs[slot_id]->cmd_status = GET_COMP_CODE(le32_to_cpu(event->status));
		complete(&xhci->addr_dev);
		break;
	case TRB_TYPE(TRB_STOP_RING):
		handle_stopped_endpoint(xhci, xhci->cmd_ring->dequeue, event);
		break;
	case TRB_TYPE(TRB_SET_DEQ):
		handle_set_deq_completion(xhci, event, xhci->cmd_ring->dequeue);
		break;
	case TRB_TYPE(TRB_CMD_NOOP):
		break;
	case TRB_TYPE(TRB_RESET_EP):
		handle_reset_ep_completion(xhci, event, xhci->cmd_ring->dequeue);
		break;
	case TRB_TYPE(TRB_RESET_DEV):
		xhci_dbg(xhci, "Completed reset device command.\n");
		slot_id = TRB_TO_SLOT_ID(
			le32_to_cpu(xhci->cmd_ring->dequeue->generic.field[3]));
		virt_dev = xhci->devs[slot_id];
		if (virt_dev)
			handle_cmd_in_cmd_wait_list(xhci, virt_dev, event);
		else
			xhci_warn(xhci, "Reset device command completion "
					"for disabled slot %u\n", slot_id);
		break;
	case TRB_TYPE(TRB_NEC_GET_FW):
		if (!(xhci->quirks & XHCI_NEC_HOST)) {
			xhci->error_bitmask |= 1 << 6;
			break;
		}
		xhci_dbg(xhci, "NEC firmware version %2x.%02x\n",
			 NEC_FW_MAJOR(le32_to_cpu(event->status)),
			 NEC_FW_MINOR(le32_to_cpu(event->status)));
		break;
	default:
		/* Skip over unknown commands on the event ring */
		xhci->error_bitmask |= 1 << 6;
		break;
	}
=======
	unsigned int slot_id = TRB_TO_SLOT_ID(le32_to_cpu(event->flags));
	u64 cmd_dma;
	dma_addr_t cmd_dequeue_dma;
	u32 cmd_comp_code;
	union xhci_trb *cmd_trb;
	struct xhci_command *cmd;
	u32 cmd_type;

	if (slot_id >= MAX_HC_SLOTS) {
		xhci_warn(xhci, "Invalid slot_id %u\n", slot_id);
		return;
	}

	cmd_dma = le64_to_cpu(event->cmd_trb);
	cmd_trb = xhci->cmd_ring->dequeue;

	trace_xhci_handle_command(xhci->cmd_ring, &cmd_trb->generic);

	cmd_dequeue_dma = xhci_trb_virt_to_dma(xhci->cmd_ring->deq_seg,
			cmd_trb);
	/*
	 * Check whether the completion event is for our internal kept
	 * command.
	 */
	if (!cmd_dequeue_dma || cmd_dma != (u64)cmd_dequeue_dma) {
		xhci_warn(xhci,
			  "ERROR mismatched command completion event\n");
		return;
	}

	cmd = list_first_entry(&xhci->cmd_list, struct xhci_command, cmd_list);

	cancel_delayed_work(&xhci->cmd_timer);

	cmd_comp_code = GET_COMP_CODE(le32_to_cpu(event->status));

	/* If CMD ring stopped we own the trbs between enqueue and dequeue */
	if (cmd_comp_code == COMP_COMMAND_RING_STOPPED) {
		complete_all(&xhci->cmd_ring_stop_completion);
		return;
	}

	if (cmd->command_trb != xhci->cmd_ring->dequeue) {
		xhci_err(xhci,
			 "Command completion event does not match command\n");
		return;
	}

	/*
	 * Host aborted the command ring, check if the current command was
	 * supposed to be aborted, otherwise continue normally.
	 * The command ring is stopped now, but the xHC will issue a Command
	 * Ring Stopped event which will cause us to restart it.
	 */
	if (cmd_comp_code == COMP_COMMAND_ABORTED) {
		xhci->cmd_ring_state = CMD_RING_STATE_STOPPED;
		if (cmd->status == COMP_COMMAND_ABORTED) {
			if (xhci->current_cmd == cmd)
				xhci->current_cmd = NULL;
			goto event_handled;
		}
	}

	cmd_type = TRB_FIELD_TO_TYPE(le32_to_cpu(cmd_trb->generic.field[3]));
	switch (cmd_type) {
	case TRB_ENABLE_SLOT:
		xhci_handle_cmd_enable_slot(xhci, slot_id, cmd, cmd_comp_code);
		break;
	case TRB_DISABLE_SLOT:
		xhci_handle_cmd_disable_slot(xhci, slot_id);
		break;
	case TRB_CONFIG_EP:
		if (!cmd->completion)
			xhci_handle_cmd_config_ep(xhci, slot_id, cmd_comp_code);
		break;
	case TRB_EVAL_CONTEXT:
		break;
	case TRB_ADDR_DEV:
		xhci_handle_cmd_addr_dev(xhci, slot_id);
		break;
	case TRB_STOP_RING:
		WARN_ON(slot_id != TRB_TO_SLOT_ID(
				le32_to_cpu(cmd_trb->generic.field[3])));
		if (!cmd->completion)
			xhci_handle_cmd_stop_ep(xhci, slot_id, cmd_trb,
						cmd_comp_code);
		break;
	case TRB_SET_DEQ:
		WARN_ON(slot_id != TRB_TO_SLOT_ID(
				le32_to_cpu(cmd_trb->generic.field[3])));
		xhci_handle_cmd_set_deq(xhci, slot_id, cmd_trb, cmd_comp_code);
		break;
	case TRB_CMD_NOOP:
		/* Is this an aborted command turned to NO-OP? */
		if (cmd->status == COMP_COMMAND_RING_STOPPED)
			cmd_comp_code = COMP_COMMAND_RING_STOPPED;
		break;
	case TRB_RESET_EP:
		WARN_ON(slot_id != TRB_TO_SLOT_ID(
				le32_to_cpu(cmd_trb->generic.field[3])));
		xhci_handle_cmd_reset_ep(xhci, slot_id, cmd_trb, cmd_comp_code);
		break;
	case TRB_RESET_DEV:
		/* SLOT_ID field in reset device cmd completion event TRB is 0.
		 * Use the SLOT_ID from the command TRB instead (xhci 4.6.11)
		 */
		slot_id = TRB_TO_SLOT_ID(
				le32_to_cpu(cmd_trb->generic.field[3]));
		xhci_handle_cmd_reset_dev(xhci, slot_id);
		break;
	case TRB_NEC_GET_FW:
		xhci_handle_cmd_nec_get_fw(xhci, event);
		break;
	default:
		/* Skip over unknown commands on the event ring */
		xhci_info(xhci, "INFO unknown command type %d\n", cmd_type);
		break;
	}

	/* restart timer if this wasn't the last command */
	if (!list_is_singular(&xhci->cmd_list)) {
		xhci->current_cmd = list_first_entry(&cmd->cmd_list,
						struct xhci_command, cmd_list);
		xhci_mod_cmd_timer(xhci);
	} else if (xhci->current_cmd == cmd) {
		xhci->current_cmd = NULL;
	}

event_handled:
	xhci_complete_del_and_free_cmd(cmd, cmd_comp_code);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	inc_deq(xhci, xhci->cmd_ring);
}

static void handle_vendor_event(struct xhci_hcd *xhci,
<<<<<<< HEAD
		union xhci_trb *event)
{
	u32 trb_type;

	trb_type = TRB_FIELD_TO_TYPE(le32_to_cpu(event->generic.field[3]));
=======
				union xhci_trb *event, u32 trb_type)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	xhci_dbg(xhci, "Vendor specific event TRB type = %u\n", trb_type);
	if (trb_type == TRB_NEC_CMD_COMP && (xhci->quirks & XHCI_NEC_HOST))
		handle_cmd_completion(xhci, &event->event_cmd);
}

<<<<<<< HEAD
/* @port_id: the one-based port ID from the hardware (indexed from array of all
 * port registers -- USB 3.0 and USB 2.0).
 *
 * Returns a zero-based port number, which is suitable for indexing into each of
 * the split roothubs' port arrays and bus state arrays.
 * Add one to it in order to call xhci_find_slot_id_by_port.
 */
static unsigned int find_faked_portnum_from_hw_portnum(struct usb_hcd *hcd,
		struct xhci_hcd *xhci, u32 port_id)
{
	unsigned int i;
	unsigned int num_similar_speed_ports = 0;

	/* port_id from the hardware is 1-based, but port_array[], usb3_ports[],
	 * and usb2_ports are 0-based indexes.  Count the number of similar
	 * speed ports, up to 1 port before this port.
	 */
	for (i = 0; i < (port_id - 1); i++) {
		u8 port_speed = xhci->port_array[i];

		/*
		 * Skip ports that don't have known speeds, or have duplicate
		 * Extended Capabilities port speed entries.
		 */
		if (port_speed == 0 || port_speed == DUPLICATE_ENTRY)
			continue;

		/*
		 * USB 3.0 ports are always under a USB 3.0 hub.  USB 2.0 and
		 * 1.1 ports are under the USB 2.0 hub.  If the port speed
		 * matches the device speed, it's a similar speed port.
		 */
		if ((port_speed == 0x03) == (hcd->speed == HCD_USB3))
			num_similar_speed_ports++;
	}
	return num_similar_speed_ports;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void handle_device_notification(struct xhci_hcd *xhci,
		union xhci_trb *event)
{
	u32 slot_id;
	struct usb_device *udev;

<<<<<<< HEAD
	slot_id = TRB_TO_SLOT_ID(event->generic.field[3]);
=======
	slot_id = TRB_TO_SLOT_ID(le32_to_cpu(event->generic.field[3]));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!xhci->devs[slot_id]) {
		xhci_warn(xhci, "Device Notification event for "
				"unused slot %u\n", slot_id);
		return;
	}

	xhci_dbg(xhci, "Device Wake Notification event for slot ID %u\n",
			slot_id);
	udev = xhci->devs[slot_id]->udev;
	if (udev && udev->parent)
		usb_wakeup_notification(udev->parent, udev->portnum);
}

<<<<<<< HEAD
static void handle_port_status(struct xhci_hcd *xhci,
		union xhci_trb *event)
{
	struct usb_hcd *hcd = NULL;
	u32 port_id;
	u32 temp, temp1;
	int max_ports;
	int slot_id;
	unsigned int faked_port_index;
	u8 major_revision;
	struct xhci_bus_state *bus_state;
	__le32 __iomem **port_array;
	bool bogus_port_status = false;

	/* Port status change events always have a successful completion code */
	if (GET_COMP_CODE(le32_to_cpu(event->generic.field[2])) != COMP_SUCCESS) {
		xhci_warn(xhci, "WARN: xHC returned failed port status event\n");
		xhci->error_bitmask |= 1 << 8;
	}
	port_id = GET_PORT_ID(le32_to_cpu(event->generic.field[0]));
	xhci_dbg(xhci, "Port Status Change Event for port %d\n", port_id);

	max_ports = HCS_MAX_PORTS(xhci->hcs_params1);
	if ((port_id <= 0) || (port_id > max_ports)) {
		xhci_warn(xhci, "Invalid port id %d\n", port_id);
=======
/*
 * Quirk hanlder for errata seen on Cavium ThunderX2 processor XHCI
 * Controller.
 * As per ThunderX2errata-129 USB 2 device may come up as USB 1
 * If a connection to a USB 1 device is followed by another connection
 * to a USB 2 device.
 *
 * Reset the PHY after the USB device is disconnected if device speed
 * is less than HCD_USB3.
 * Retry the reset sequence max of 4 times checking the PLL lock status.
 *
 */
static void xhci_cavium_reset_phy_quirk(struct xhci_hcd *xhci)
{
	struct usb_hcd *hcd = xhci_to_hcd(xhci);
	u32 pll_lock_check;
	u32 retry_count = 4;

	do {
		/* Assert PHY reset */
		writel(0x6F, hcd->regs + 0x1048);
		udelay(10);
		/* De-assert the PHY reset */
		writel(0x7F, hcd->regs + 0x1048);
		udelay(200);
		pll_lock_check = readl(hcd->regs + 0x1070);
	} while (!(pll_lock_check & 0x1) && --retry_count);
}

static void handle_port_status(struct xhci_hcd *xhci,
			       struct xhci_interrupter *ir,
			       union xhci_trb *event)
{
	struct usb_hcd *hcd;
	u32 port_id;
	u32 portsc, cmd_reg;
	int max_ports;
	unsigned int hcd_portnum;
	struct xhci_bus_state *bus_state;
	bool bogus_port_status = false;
	struct xhci_port *port;

	/* Port status change events always have a successful completion code */
	if (GET_COMP_CODE(le32_to_cpu(event->generic.field[2])) != COMP_SUCCESS)
		xhci_warn(xhci,
			  "WARN: xHC returned failed port status event\n");

	port_id = GET_PORT_ID(le32_to_cpu(event->generic.field[0]));
	max_ports = HCS_MAX_PORTS(xhci->hcs_params1);

	if ((port_id <= 0) || (port_id > max_ports)) {
		xhci_warn(xhci, "Port change event with invalid port ID %d\n",
			  port_id);
		return;
	}

	port = &xhci->hw_ports[port_id - 1];
	if (!port || !port->rhub || port->hcd_portnum == DUPLICATE_ENTRY) {
		xhci_warn(xhci, "Port change event, no port for port ID %u\n",
			  port_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		bogus_port_status = true;
		goto cleanup;
	}

<<<<<<< HEAD
	/* Figure out which usb_hcd this port is attached to:
	 * is it a USB 3.0 port or a USB 2.0/1.1 port?
	 */
	major_revision = xhci->port_array[port_id - 1];
	if (major_revision == 0) {
		xhci_warn(xhci, "Event for port %u not in "
				"Extended Capabilities, ignoring.\n",
				port_id);
		bogus_port_status = true;
		goto cleanup;
	}
	if (major_revision == DUPLICATE_ENTRY) {
		xhci_warn(xhci, "Event for port %u duplicated in"
				"Extended Capabilities, ignoring.\n",
				port_id);
=======
	/* We might get interrupts after shared_hcd is removed */
	if (port->rhub == &xhci->usb3_rhub && xhci->shared_hcd == NULL) {
		xhci_dbg(xhci, "ignore port event for removed USB3 hcd\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		bogus_port_status = true;
		goto cleanup;
	}

<<<<<<< HEAD
	/*
	 * Hardware port IDs reported by a Port Status Change Event include USB
	 * 3.0 and USB 2.0 ports.  We want to check if the port has reported a
	 * resume event, but we first need to translate the hardware port ID
	 * into the index into the ports on the correct split roothub, and the
	 * correct bus_state structure.
	 */
	/* Find the right roothub. */
	hcd = xhci_to_hcd(xhci);
	if (!hcd)
		goto cleanup;
	if ((major_revision == 0x03) != (hcd->speed == HCD_USB3))
		hcd = xhci->shared_hcd;
	bus_state = &xhci->bus_state[hcd_index(hcd)];
	if (hcd->speed == HCD_USB3)
		port_array = xhci->usb3_ports;
	else
		port_array = xhci->usb2_ports;
	/* Find the faked port hub number */
	faked_port_index = find_faked_portnum_from_hw_portnum(hcd, xhci,
			port_id);

	temp = xhci_readl(xhci, port_array[faked_port_index]);
=======
	hcd = port->rhub->hcd;
	bus_state = &port->rhub->bus_state;
	hcd_portnum = port->hcd_portnum;
	portsc = readl(port->addr);

	xhci_dbg(xhci, "Port change event, %d-%d, id %d, portsc: 0x%x\n",
		 hcd->self.busnum, hcd_portnum + 1, port_id, portsc);

	trace_xhci_handle_port_status(port, portsc);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (hcd->state == HC_STATE_SUSPENDED) {
		xhci_dbg(xhci, "resume root hub\n");
		usb_hcd_resume_root_hub(hcd);
	}

<<<<<<< HEAD
	if (hcd->speed == HCD_USB3 && (temp & PORT_PLS_MASK) == XDEV_INACTIVE)
		bus_state->port_remote_wakeup &= ~(1 << faked_port_index);

	if ((temp & PORT_PLC) && (temp & PORT_PLS_MASK) == XDEV_RESUME) {
		xhci_dbg(xhci, "port resume event for port %d\n", port_id);

		temp1 = xhci_readl(xhci, &xhci->op_regs->command);
		if (!(temp1 & CMD_RUN)) {
=======
	if (hcd->speed >= HCD_USB3 &&
	    (portsc & PORT_PLS_MASK) == XDEV_INACTIVE) {
		if (port->slot_id && xhci->devs[port->slot_id])
			xhci->devs[port->slot_id]->flags |= VDEV_PORT_ERROR;
	}

	if ((portsc & PORT_PLC) && (portsc & PORT_PLS_MASK) == XDEV_RESUME) {
		xhci_dbg(xhci, "port resume event for port %d\n", port_id);

		cmd_reg = readl(&xhci->op_regs->command);
		if (!(cmd_reg & CMD_RUN)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			xhci_warn(xhci, "xHC is not running.\n");
			goto cleanup;
		}

<<<<<<< HEAD
		if (DEV_SUPERSPEED(temp)) {
=======
		if (DEV_SUPERSPEED_ANY(portsc)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			xhci_dbg(xhci, "remote wake SS port %d\n", port_id);
			/* Set a flag to say the port signaled remote wakeup,
			 * so we can tell the difference between the end of
			 * device and host initiated resume.
			 */
<<<<<<< HEAD
			bus_state->port_remote_wakeup |= 1 << faked_port_index;
			xhci_test_and_clear_bit(xhci, port_array,
					faked_port_index, PORT_PLC);
			xhci_set_link_state(xhci, port_array, faked_port_index,
						XDEV_U0);
=======
			bus_state->port_remote_wakeup |= 1 << hcd_portnum;
			xhci_test_and_clear_bit(xhci, port, PORT_PLC);
			usb_hcd_start_port_resume(&hcd->self, hcd_portnum);
			xhci_set_link_state(xhci, port, XDEV_U0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/* Need to wait until the next link state change
			 * indicates the device is actually in U0.
			 */
			bogus_port_status = true;
			goto cleanup;
<<<<<<< HEAD
		} else {
			xhci_dbg(xhci, "resume HS port %d\n", port_id);
			bus_state->resume_done[faked_port_index] = jiffies +
				msecs_to_jiffies(20);
			set_bit(faked_port_index, &bus_state->resuming_ports);
			mod_timer(&hcd->rh_timer,
				  bus_state->resume_done[faked_port_index]);
			/* Do the rest in GetPortStatus */
		}
	}

	if ((temp & PORT_PLC) && (temp & PORT_PLS_MASK) == XDEV_U0 &&
			DEV_SUPERSPEED(temp)) {
		xhci_dbg(xhci, "resume SS port %d finished\n", port_id);
		/* We've just brought the device into U0 through either the
=======
		} else if (!test_bit(hcd_portnum, &bus_state->resuming_ports)) {
			xhci_dbg(xhci, "resume HS port %d\n", port_id);
			port->resume_timestamp = jiffies +
				msecs_to_jiffies(USB_RESUME_TIMEOUT);
			set_bit(hcd_portnum, &bus_state->resuming_ports);
			/* Do the rest in GetPortStatus after resume time delay.
			 * Avoid polling roothub status before that so that a
			 * usb device auto-resume latency around ~40ms.
			 */
			set_bit(HCD_FLAG_POLL_RH, &hcd->flags);
			mod_timer(&hcd->rh_timer,
				  port->resume_timestamp);
			usb_hcd_start_port_resume(&hcd->self, hcd_portnum);
			bogus_port_status = true;
		}
	}

	if ((portsc & PORT_PLC) &&
	    DEV_SUPERSPEED_ANY(portsc) &&
	    ((portsc & PORT_PLS_MASK) == XDEV_U0 ||
	     (portsc & PORT_PLS_MASK) == XDEV_U1 ||
	     (portsc & PORT_PLS_MASK) == XDEV_U2)) {
		xhci_dbg(xhci, "resume SS port %d finished\n", port_id);
		complete(&port->u3exit_done);
		/* We've just brought the device into U0/1/2 through either the
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		 * Resume state after a device remote wakeup, or through the
		 * U3Exit state after a host-initiated resume.  If it's a device
		 * initiated remote wake, don't pass up the link state change,
		 * so the roothub behavior is consistent with external
		 * USB 3.0 hub behavior.
		 */
<<<<<<< HEAD
		slot_id = xhci_find_slot_id_by_port(hcd, xhci,
				faked_port_index + 1);
		if (slot_id && xhci->devs[slot_id])
			xhci_ring_device(xhci, slot_id);
		if (bus_state->port_remote_wakeup & (1 << faked_port_index)) {
			bus_state->port_remote_wakeup &=
				~(1 << faked_port_index);
			xhci_test_and_clear_bit(xhci, port_array,
					faked_port_index, PORT_PLC);
			usb_wakeup_notification(hcd->self.root_hub,
					faked_port_index + 1);
=======
		if (port->slot_id && xhci->devs[port->slot_id])
			xhci_ring_device(xhci, port->slot_id);
		if (bus_state->port_remote_wakeup & (1 << hcd_portnum)) {
			xhci_test_and_clear_bit(xhci, port, PORT_PLC);
			usb_wakeup_notification(hcd->self.root_hub,
					hcd_portnum + 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			bogus_port_status = true;
			goto cleanup;
		}
	}

<<<<<<< HEAD
	if (hcd->speed != HCD_USB3)
		xhci_test_and_clear_bit(xhci, port_array, faked_port_index,
					PORT_PLC);

cleanup:
	/* Update event ring dequeue pointer before dropping the lock */
	inc_deq(xhci, xhci->event_ring);
=======
	/*
	 * Check to see if xhci-hub.c is waiting on RExit to U0 transition (or
	 * RExit to a disconnect state).  If so, let the driver know it's
	 * out of the RExit state.
	 */
	if (hcd->speed < HCD_USB3 && port->rexit_active) {
		complete(&port->rexit_done);
		port->rexit_active = false;
		bogus_port_status = true;
		goto cleanup;
	}

	if (hcd->speed < HCD_USB3) {
		xhci_test_and_clear_bit(xhci, port, PORT_PLC);
		if ((xhci->quirks & XHCI_RESET_PLL_ON_DISCONNECT) &&
		    (portsc & PORT_CSC) && !(portsc & PORT_CONNECT))
			xhci_cavium_reset_phy_quirk(xhci);
	}

cleanup:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Don't make the USB core poll the roothub if we got a bad port status
	 * change event.  Besides, at that point we can't tell which roothub
	 * (USB 2.0 or USB 3.0) to kick.
	 */
	if (bogus_port_status)
		return;

	/*
	 * xHCI port-status-change events occur when the "or" of all the
	 * status-change bits in the portsc register changes from 0 to 1.
	 * New status changes won't cause an event if any other change
	 * bits are still set.  When an event occurs, switch over to
	 * polling to avoid losing status changes.
	 */
<<<<<<< HEAD
	xhci_dbg(xhci, "%s: starting port polling.\n", __func__);
=======
	xhci_dbg(xhci, "%s: starting usb%d port polling.\n",
		 __func__, hcd->self.busnum);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	set_bit(HCD_FLAG_POLL_RH, &hcd->flags);
	spin_unlock(&xhci->lock);
	/* Pass this up to the core */
	usb_hcd_poll_rh_status(hcd);
	spin_lock(&xhci->lock);
}

/*
 * This TD is defined by the TRBs starting at start_trb in start_seg and ending
 * at end_trb, which may be in another segment.  If the suspect DMA address is a
 * TRB in this TD, this function returns that TRB's segment.  Otherwise it
 * returns 0.
 */
<<<<<<< HEAD
struct xhci_segment *trb_in_td(struct xhci_segment *start_seg,
		union xhci_trb	*start_trb,
		union xhci_trb	*end_trb,
		dma_addr_t	suspect_dma)
=======
struct xhci_segment *trb_in_td(struct xhci_hcd *xhci,
		struct xhci_segment *start_seg,
		union xhci_trb	*start_trb,
		union xhci_trb	*end_trb,
		dma_addr_t	suspect_dma,
		bool		debug)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	dma_addr_t start_dma;
	dma_addr_t end_seg_dma;
	dma_addr_t end_trb_dma;
	struct xhci_segment *cur_seg;

	start_dma = xhci_trb_virt_to_dma(start_seg, start_trb);
	cur_seg = start_seg;

	do {
		if (start_dma == 0)
			return NULL;
		/* We may get an event for a Link TRB in the middle of a TD */
		end_seg_dma = xhci_trb_virt_to_dma(cur_seg,
				&cur_seg->trbs[TRBS_PER_SEGMENT - 1]);
		/* If the end TRB isn't in this segment, this is set to 0 */
		end_trb_dma = xhci_trb_virt_to_dma(cur_seg, end_trb);

<<<<<<< HEAD
=======
		if (debug)
			xhci_warn(xhci,
				"Looking for event-dma %016llx trb-start %016llx trb-end %016llx seg-start %016llx seg-end %016llx\n",
				(unsigned long long)suspect_dma,
				(unsigned long long)start_dma,
				(unsigned long long)end_trb_dma,
				(unsigned long long)cur_seg->dma,
				(unsigned long long)end_seg_dma);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (end_trb_dma > 0) {
			/* The end TRB is in this segment, so suspect should be here */
			if (start_dma <= end_trb_dma) {
				if (suspect_dma >= start_dma && suspect_dma <= end_trb_dma)
					return cur_seg;
			} else {
				/* Case for one segment with
				 * a TD wrapped around to the top
				 */
				if ((suspect_dma >= start_dma &&
							suspect_dma <= end_seg_dma) ||
						(suspect_dma >= cur_seg->dma &&
						 suspect_dma <= end_trb_dma))
					return cur_seg;
			}
			return NULL;
		} else {
			/* Might still be somewhere in this segment */
			if (suspect_dma >= start_dma && suspect_dma <= end_seg_dma)
				return cur_seg;
		}
		cur_seg = cur_seg->next;
		start_dma = xhci_trb_virt_to_dma(cur_seg, &cur_seg->trbs[0]);
	} while (cur_seg != start_seg);

	return NULL;
}

<<<<<<< HEAD
static void xhci_cleanup_halted_endpoint(struct xhci_hcd *xhci,
		unsigned int slot_id, unsigned int ep_index,
		unsigned int stream_id,
		struct xhci_td *td, union xhci_trb *event_trb)
{
	struct xhci_virt_ep *ep = &xhci->devs[slot_id]->eps[ep_index];
	ep->ep_state |= EP_HALTED;
	ep->stopped_td = td;
	ep->stopped_trb = event_trb;
	ep->stopped_stream = stream_id;

	xhci_queue_reset_ep(xhci, slot_id, ep_index);
	xhci_cleanup_stalled_ring(xhci, td->urb->dev, ep_index);

	ep->stopped_td = NULL;
	ep->stopped_trb = NULL;
	ep->stopped_stream = 0;

	xhci_ring_cmd_db(xhci);
=======
static void xhci_clear_hub_tt_buffer(struct xhci_hcd *xhci, struct xhci_td *td,
		struct xhci_virt_ep *ep)
{
	/*
	 * As part of low/full-speed endpoint-halt processing
	 * we must clear the TT buffer (USB 2.0 specification 11.17.5).
	 */
	if (td->urb->dev->tt && !usb_pipeint(td->urb->pipe) &&
	    (td->urb->dev->tt->hub != xhci_to_hcd(xhci)->self.root_hub) &&
	    !(ep->ep_state & EP_CLEARING_TT)) {
		ep->ep_state |= EP_CLEARING_TT;
		td->urb->ep->hcpriv = td->urb->dev;
		if (usb_hub_clear_tt_buffer(td->urb))
			ep->ep_state &= ~EP_CLEARING_TT;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Check if an error has halted the endpoint ring.  The class driver will
 * cleanup the halt for a non-default control endpoint if we indicate a stall.
 * However, a babble and other errors also halt the endpoint ring, and the class
 * driver won't clear the halt in that case, so we need to issue a Set Transfer
 * Ring Dequeue Pointer command manually.
 */
static int xhci_requires_manual_halt_cleanup(struct xhci_hcd *xhci,
		struct xhci_ep_ctx *ep_ctx,
		unsigned int trb_comp_code)
{
	/* TRB completion codes that may require a manual halt cleanup */
<<<<<<< HEAD
	if (trb_comp_code == COMP_TX_ERR ||
			trb_comp_code == COMP_BABBLE ||
			trb_comp_code == COMP_SPLIT_ERR)
		/* The 0.96 spec says a babbling control endpoint
=======
	if (trb_comp_code == COMP_USB_TRANSACTION_ERROR ||
			trb_comp_code == COMP_BABBLE_DETECTED_ERROR ||
			trb_comp_code == COMP_SPLIT_TRANSACTION_ERROR)
		/* The 0.95 spec says a babbling control endpoint
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		 * is not halted. The 0.96 spec says it is.  Some HW
		 * claims to be 0.95 compliant, but it halts the control
		 * endpoint anyway.  Check if a babble halted the
		 * endpoint.
		 */
<<<<<<< HEAD
		if ((ep_ctx->ep_info & cpu_to_le32(EP_STATE_MASK)) ==
		    cpu_to_le32(EP_STATE_HALTED))
=======
		if (GET_EP_CTX_STATE(ep_ctx) == EP_STATE_HALTED)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return 1;

	return 0;
}

int xhci_is_vendor_info_code(struct xhci_hcd *xhci, unsigned int trb_comp_code)
{
	if (trb_comp_code >= 224 && trb_comp_code <= 255) {
		/* Vendor defined "informational" completion code,
		 * treat as not-an-error.
		 */
		xhci_dbg(xhci, "Vendor defined info completion code %u\n",
				trb_comp_code);
		xhci_dbg(xhci, "Treating code as success.\n");
		return 1;
	}
	return 0;
}

<<<<<<< HEAD
/*
 * Finish the td processing, remove the td from td list;
 * Return 1 if the urb can be given back.
 */
static int finish_td(struct xhci_hcd *xhci, struct xhci_td *td,
	union xhci_trb *event_trb, struct xhci_transfer_event *event,
	struct xhci_virt_ep *ep, int *status, bool skip)
{
	struct xhci_virt_device *xdev;
	struct xhci_ring *ep_ring;
	unsigned int slot_id;
	int ep_index;
	struct urb *urb = NULL;
	struct xhci_ep_ctx *ep_ctx;
	int ret = 0;
	struct urb_priv	*urb_priv;
	u32 trb_comp_code;

	slot_id = TRB_TO_SLOT_ID(le32_to_cpu(event->flags));
	xdev = xhci->devs[slot_id];
	ep_index = TRB_TO_EP_ID(le32_to_cpu(event->flags)) - 1;
	ep_ring = xhci_dma_to_transfer_ring(ep, le64_to_cpu(event->buffer));
	ep_ctx = xhci_get_ep_ctx(xhci, xdev->out_ctx, ep_index);
	trb_comp_code = GET_COMP_CODE(le32_to_cpu(event->transfer_len));

	if (skip)
		goto td_cleanup;

	if (trb_comp_code == COMP_STOP_INVAL ||
			trb_comp_code == COMP_STOP) {
		/* The Endpoint Stop Command completion will take care of any
		 * stopped TDs.  A stopped TD may be restarted, so don't update
		 * the ring dequeue pointer or take this TD off any lists yet.
		 */
		ep->stopped_td = td;
		ep->stopped_trb = event_trb;
		return 0;
	} else {
		if (trb_comp_code == COMP_STALL ||
		    xhci_requires_manual_halt_cleanup(xhci, ep_ctx,
						      trb_comp_code)) {
			/* Issue a reset endpoint command to clear the host side			 * halt, followed by a set dequeue command to move the
			 * dequeue pointer past the TD.
			 * The class driver clears the device side halt later.
			 */
			xhci_cleanup_halted_endpoint(xhci,
					slot_id, ep_index, ep_ring->stream_id,
					td, event_trb);
		} else {
			/* Update ring dequeue pointer */
			while (ep_ring->dequeue != td->last_trb)
				inc_deq(xhci, ep_ring);
			inc_deq(xhci, ep_ring);
		}

td_cleanup:
		/* Clean up the endpoint's TD list */
		urb = td->urb;
		urb_priv = urb->hcpriv;

		/* Do one last check of the actual transfer length.
		 * If the host controller said we transferred more data than
		 * the buffer length, urb->actual_length will be a very big
		 * number (since it's unsigned).  Play it safe and say we didn't
		 * transfer anything.
		 */
		if (urb->actual_length > urb->transfer_buffer_length) {
			xhci_warn(xhci, "URB transfer length is wrong, "
					"xHC issue? req. len = %u, "
					"act. len = %u\n",
					urb->transfer_buffer_length,
					urb->actual_length);
			urb->actual_length = 0;
			if (td->urb->transfer_flags & URB_SHORT_NOT_OK)
				*status = -EREMOTEIO;
			else
				*status = 0;
		}
		list_del_init(&td->td_list);
		/* Was this TD slated to be cancelled but completed anyway? */
		if (!list_empty(&td->cancelled_td_list))
			list_del_init(&td->cancelled_td_list);

		urb_priv->td_cnt++;
		/* Giveback the urb when all the tds are completed */
		if (urb_priv->td_cnt == urb_priv->length) {
			ret = 1;
			if (usb_pipetype(urb->pipe) == PIPE_ISOCHRONOUS) {
				xhci_to_hcd(xhci)->self.bandwidth_isoc_reqs--;
				if (xhci_to_hcd(xhci)->self.bandwidth_isoc_reqs
					== 0) {
					if (xhci->quirks & XHCI_AMD_PLL_FIX)
						usb_amd_quirk_pll_enable();
				}
			}
		}
	}

	return ret;
=======
static int finish_td(struct xhci_hcd *xhci, struct xhci_virt_ep *ep,
		     struct xhci_ring *ep_ring, struct xhci_td *td,
		     u32 trb_comp_code)
{
	struct xhci_ep_ctx *ep_ctx;

	ep_ctx = xhci_get_ep_ctx(xhci, ep->vdev->out_ctx, ep->ep_index);

	switch (trb_comp_code) {
	case COMP_STOPPED_LENGTH_INVALID:
	case COMP_STOPPED_SHORT_PACKET:
	case COMP_STOPPED:
		/*
		 * The "Stop Endpoint" completion will take care of any
		 * stopped TDs. A stopped TD may be restarted, so don't update
		 * the ring dequeue pointer or take this TD off any lists yet.
		 */
		return 0;
	case COMP_USB_TRANSACTION_ERROR:
	case COMP_BABBLE_DETECTED_ERROR:
	case COMP_SPLIT_TRANSACTION_ERROR:
		/*
		 * If endpoint context state is not halted we might be
		 * racing with a reset endpoint command issued by a unsuccessful
		 * stop endpoint completion (context error). In that case the
		 * td should be on the cancelled list, and EP_HALTED flag set.
		 *
		 * Or then it's not halted due to the 0.95 spec stating that a
		 * babbling control endpoint should not halt. The 0.96 spec
		 * again says it should.  Some HW claims to be 0.95 compliant,
		 * but it halts the control endpoint anyway.
		 */
		if (GET_EP_CTX_STATE(ep_ctx) != EP_STATE_HALTED) {
			/*
			 * If EP_HALTED is set and TD is on the cancelled list
			 * the TD and dequeue pointer will be handled by reset
			 * ep command completion
			 */
			if ((ep->ep_state & EP_HALTED) &&
			    !list_empty(&td->cancelled_td_list)) {
				xhci_dbg(xhci, "Already resolving halted ep for 0x%llx\n",
					 (unsigned long long)xhci_trb_virt_to_dma(
						 td->start_seg, td->first_trb));
				return 0;
			}
			/* endpoint not halted, don't reset it */
			break;
		}
		/* Almost same procedure as for STALL_ERROR below */
		xhci_clear_hub_tt_buffer(xhci, td, ep);
		xhci_handle_halted_endpoint(xhci, ep, td, EP_HARD_RESET);
		return 0;
	case COMP_STALL_ERROR:
		/*
		 * xhci internal endpoint state will go to a "halt" state for
		 * any stall, including default control pipe protocol stall.
		 * To clear the host side halt we need to issue a reset endpoint
		 * command, followed by a set dequeue command to move past the
		 * TD.
		 * Class drivers clear the device side halt from a functional
		 * stall later. Hub TT buffer should only be cleared for FS/LS
		 * devices behind HS hubs for functional stalls.
		 */
		if (ep->ep_index != 0)
			xhci_clear_hub_tt_buffer(xhci, td, ep);

		xhci_handle_halted_endpoint(xhci, ep, td, EP_HARD_RESET);

		return 0; /* xhci_handle_halted_endpoint marked td cancelled */
	default:
		break;
	}

	/* Update ring dequeue pointer */
	ep_ring->dequeue = td->last_trb;
	ep_ring->deq_seg = td->last_trb_seg;
	inc_deq(xhci, ep_ring);

	return xhci_td_cleanup(xhci, td, ep_ring, td->status);
}

/* sum trb lengths from ring dequeue up to stop_trb, _excluding_ stop_trb */
static int sum_trb_lengths(struct xhci_hcd *xhci, struct xhci_ring *ring,
			   union xhci_trb *stop_trb)
{
	u32 sum;
	union xhci_trb *trb = ring->dequeue;
	struct xhci_segment *seg = ring->deq_seg;

	for (sum = 0; trb != stop_trb; next_trb(xhci, ring, &seg, &trb)) {
		if (!trb_is_noop(trb) && !trb_is_link(trb))
			sum += TRB_LEN(le32_to_cpu(trb->generic.field[2]));
	}
	return sum;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Process control tds, update urb status and actual_length.
 */
<<<<<<< HEAD
static int process_ctrl_td(struct xhci_hcd *xhci, struct xhci_td *td,
	union xhci_trb *event_trb, struct xhci_transfer_event *event,
	struct xhci_virt_ep *ep, int *status)
{
	struct xhci_virt_device *xdev;
	struct xhci_ring *ep_ring;
	unsigned int slot_id;
	int ep_index;
	struct xhci_ep_ctx *ep_ctx;
	u32 trb_comp_code;

	slot_id = TRB_TO_SLOT_ID(le32_to_cpu(event->flags));
	xdev = xhci->devs[slot_id];
	ep_index = TRB_TO_EP_ID(le32_to_cpu(event->flags)) - 1;
	ep_ring = xhci_dma_to_transfer_ring(ep, le64_to_cpu(event->buffer));
	ep_ctx = xhci_get_ep_ctx(xhci, xdev->out_ctx, ep_index);
	trb_comp_code = GET_COMP_CODE(le32_to_cpu(event->transfer_len));

	switch (trb_comp_code) {
	case COMP_SUCCESS:
		if (event_trb == ep_ring->dequeue) {
			xhci_warn(xhci, "WARN: Success on ctrl setup TRB "
					"without IOC set??\n");
			*status = -ESHUTDOWN;
		} else if (event_trb != td->last_trb) {
			xhci_warn(xhci, "WARN: Success on ctrl data TRB "
					"without IOC set??\n");
			*status = -ESHUTDOWN;
		} else {
			*status = 0;
		}
		break;
	case COMP_SHORT_TX:
		if (td->urb->transfer_flags & URB_SHORT_NOT_OK)
			*status = -EREMOTEIO;
		else
			*status = 0;
		break;
	case COMP_STOP_INVAL:
	case COMP_STOP:
		return finish_td(xhci, td, event_trb, event, ep, status, false);
	default:
		if (!xhci_requires_manual_halt_cleanup(xhci,
					ep_ctx, trb_comp_code))
			break;
		xhci_dbg(xhci, "TRB error code %u, "
				"halted endpoint index = %u\n",
				trb_comp_code, ep_index);
		/* else fall through */
	case COMP_STALL:
		/* Did we transfer part of the data (middle) phase? */
		if (event_trb != ep_ring->dequeue &&
				event_trb != td->last_trb)
			td->urb->actual_length =
				td->urb->transfer_buffer_length -
				EVENT_TRB_LEN(le32_to_cpu(event->transfer_len));
		else
			td->urb->actual_length = 0;

		return finish_td(xhci, td, event_trb, event, ep, status, false);
	}
	/*
	 * Did we transfer any data, despite the errors that might have
	 * happened?  I.e. did we get past the setup stage?
	 */
	if (event_trb != ep_ring->dequeue) {
		/* The event was for the status stage */
		if (event_trb == td->last_trb) {
			if (td->urb_length_set) {
				/* Don't overwrite a previously set error code
				 */
				if ((*status == -EINPROGRESS || *status == 0) &&
						(td->urb->transfer_flags
						 & URB_SHORT_NOT_OK))
					/* Did we already see a short data
					 * stage? */
					*status = -EREMOTEIO;
			} else {
				td->urb->actual_length =
					td->urb->transfer_buffer_length;
			}
		} else {
			/*
			 * Maybe the event was for the data stage? If so, update
			 * already the actual_length of the URB and flag it as
			 * set, so that it is not overwritten in the event for
			 * the last TRB.
			 */
			td->urb_length_set = true;
			td->urb->actual_length =
				td->urb->transfer_buffer_length -
				EVENT_TRB_LEN(le32_to_cpu(event->transfer_len));
			xhci_dbg(xhci, "Waiting for status "
					"stage event\n");
			return 0;
		}
	}

	return finish_td(xhci, td, event_trb, event, ep, status, false);
=======
static int process_ctrl_td(struct xhci_hcd *xhci, struct xhci_virt_ep *ep,
		struct xhci_ring *ep_ring,  struct xhci_td *td,
			   union xhci_trb *ep_trb, struct xhci_transfer_event *event)
{
	struct xhci_ep_ctx *ep_ctx;
	u32 trb_comp_code;
	u32 remaining, requested;
	u32 trb_type;

	trb_type = TRB_FIELD_TO_TYPE(le32_to_cpu(ep_trb->generic.field[3]));
	ep_ctx = xhci_get_ep_ctx(xhci, ep->vdev->out_ctx, ep->ep_index);
	trb_comp_code = GET_COMP_CODE(le32_to_cpu(event->transfer_len));
	requested = td->urb->transfer_buffer_length;
	remaining = EVENT_TRB_LEN(le32_to_cpu(event->transfer_len));

	switch (trb_comp_code) {
	case COMP_SUCCESS:
		if (trb_type != TRB_STATUS) {
			xhci_warn(xhci, "WARN: Success on ctrl %s TRB without IOC set?\n",
				  (trb_type == TRB_DATA) ? "data" : "setup");
			td->status = -ESHUTDOWN;
			break;
		}
		td->status = 0;
		break;
	case COMP_SHORT_PACKET:
		td->status = 0;
		break;
	case COMP_STOPPED_SHORT_PACKET:
		if (trb_type == TRB_DATA || trb_type == TRB_NORMAL)
			td->urb->actual_length = remaining;
		else
			xhci_warn(xhci, "WARN: Stopped Short Packet on ctrl setup or status TRB\n");
		goto finish_td;
	case COMP_STOPPED:
		switch (trb_type) {
		case TRB_SETUP:
			td->urb->actual_length = 0;
			goto finish_td;
		case TRB_DATA:
		case TRB_NORMAL:
			td->urb->actual_length = requested - remaining;
			goto finish_td;
		case TRB_STATUS:
			td->urb->actual_length = requested;
			goto finish_td;
		default:
			xhci_warn(xhci, "WARN: unexpected TRB Type %d\n",
				  trb_type);
			goto finish_td;
		}
	case COMP_STOPPED_LENGTH_INVALID:
		goto finish_td;
	default:
		if (!xhci_requires_manual_halt_cleanup(xhci,
						       ep_ctx, trb_comp_code))
			break;
		xhci_dbg(xhci, "TRB error %u, halted endpoint index = %u\n",
			 trb_comp_code, ep->ep_index);
		fallthrough;
	case COMP_STALL_ERROR:
		/* Did we transfer part of the data (middle) phase? */
		if (trb_type == TRB_DATA || trb_type == TRB_NORMAL)
			td->urb->actual_length = requested - remaining;
		else if (!td->urb_length_set)
			td->urb->actual_length = 0;
		goto finish_td;
	}

	/* stopped at setup stage, no data transferred */
	if (trb_type == TRB_SETUP)
		goto finish_td;

	/*
	 * if on data stage then update the actual_length of the URB and flag it
	 * as set, so it won't be overwritten in the event for the last TRB.
	 */
	if (trb_type == TRB_DATA ||
		trb_type == TRB_NORMAL) {
		td->urb_length_set = true;
		td->urb->actual_length = requested - remaining;
		xhci_dbg(xhci, "Waiting for status stage event\n");
		return 0;
	}

	/* at status stage */
	if (!td->urb_length_set)
		td->urb->actual_length = requested;

finish_td:
	return finish_td(xhci, ep, ep_ring, td, trb_comp_code);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Process isochronous tds, update urb packet status and actual_length.
 */
<<<<<<< HEAD
static int process_isoc_td(struct xhci_hcd *xhci, struct xhci_td *td,
	union xhci_trb *event_trb, struct xhci_transfer_event *event,
	struct xhci_virt_ep *ep, int *status)
{
	struct xhci_ring *ep_ring;
	struct urb_priv *urb_priv;
	int idx;
	int len = 0;
	union xhci_trb *cur_trb;
	struct xhci_segment *cur_seg;
	struct usb_iso_packet_descriptor *frame;
	u32 trb_comp_code;
	bool skip_td = false;

	ep_ring = xhci_dma_to_transfer_ring(ep, le64_to_cpu(event->buffer));
	trb_comp_code = GET_COMP_CODE(le32_to_cpu(event->transfer_len));
	urb_priv = td->urb->hcpriv;
	idx = urb_priv->td_cnt;
	frame = &td->urb->iso_frame_desc[idx];
=======
static int process_isoc_td(struct xhci_hcd *xhci, struct xhci_virt_ep *ep,
		struct xhci_ring *ep_ring, struct xhci_td *td,
		union xhci_trb *ep_trb, struct xhci_transfer_event *event)
{
	struct urb_priv *urb_priv;
	int idx;
	struct usb_iso_packet_descriptor *frame;
	u32 trb_comp_code;
	bool sum_trbs_for_length = false;
	u32 remaining, requested, ep_trb_len;
	int short_framestatus;

	trb_comp_code = GET_COMP_CODE(le32_to_cpu(event->transfer_len));
	urb_priv = td->urb->hcpriv;
	idx = urb_priv->num_tds_done;
	frame = &td->urb->iso_frame_desc[idx];
	requested = frame->length;
	remaining = EVENT_TRB_LEN(le32_to_cpu(event->transfer_len));
	ep_trb_len = TRB_LEN(le32_to_cpu(ep_trb->generic.field[2]));
	short_framestatus = td->urb->transfer_flags & URB_SHORT_NOT_OK ?
		-EREMOTEIO : 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* handle completion code */
	switch (trb_comp_code) {
	case COMP_SUCCESS:
<<<<<<< HEAD
		if (EVENT_TRB_LEN(le32_to_cpu(event->transfer_len)) == 0) {
			frame->status = 0;
			break;
		}
		if ((xhci->quirks & XHCI_TRUST_TX_LENGTH))
			trb_comp_code = COMP_SHORT_TX;
	case COMP_SHORT_TX:
		frame->status = td->urb->transfer_flags & URB_SHORT_NOT_OK ?
				-EREMOTEIO : 0;
		break;
	case COMP_BW_OVER:
		frame->status = -ECOMM;
		skip_td = true;
		break;
	case COMP_BUFF_OVER:
	case COMP_BABBLE:
		frame->status = -EOVERFLOW;
		skip_td = true;
		break;
	case COMP_DEV_ERR:
	case COMP_STALL:
		frame->status = -EPROTO;
		skip_td = true;
		break;
	case COMP_TX_ERR:
		frame->status = -EPROTO;
		if (event_trb != td->last_trb)
			return 0;
		skip_td = true;
		break;
	case COMP_STOP:
	case COMP_STOP_INVAL:
		break;
	default:
=======
		/* Don't overwrite status if TD had an error, see xHCI 4.9.1 */
		if (td->error_mid_td)
			break;
		if (remaining) {
			frame->status = short_framestatus;
			if (xhci->quirks & XHCI_TRUST_TX_LENGTH)
				sum_trbs_for_length = true;
			break;
		}
		frame->status = 0;
		break;
	case COMP_SHORT_PACKET:
		frame->status = short_framestatus;
		sum_trbs_for_length = true;
		break;
	case COMP_BANDWIDTH_OVERRUN_ERROR:
		frame->status = -ECOMM;
		break;
	case COMP_BABBLE_DETECTED_ERROR:
		sum_trbs_for_length = true;
		fallthrough;
	case COMP_ISOCH_BUFFER_OVERRUN:
		frame->status = -EOVERFLOW;
		if (ep_trb != td->last_trb)
			td->error_mid_td = true;
		break;
	case COMP_INCOMPATIBLE_DEVICE_ERROR:
	case COMP_STALL_ERROR:
		frame->status = -EPROTO;
		break;
	case COMP_USB_TRANSACTION_ERROR:
		frame->status = -EPROTO;
		sum_trbs_for_length = true;
		if (ep_trb != td->last_trb)
			td->error_mid_td = true;
		break;
	case COMP_STOPPED:
		sum_trbs_for_length = true;
		break;
	case COMP_STOPPED_SHORT_PACKET:
		/* field normally containing residue now contains tranferred */
		frame->status = short_framestatus;
		requested = remaining;
		break;
	case COMP_STOPPED_LENGTH_INVALID:
		requested = 0;
		remaining = 0;
		break;
	default:
		sum_trbs_for_length = true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		frame->status = -1;
		break;
	}

<<<<<<< HEAD
	if (trb_comp_code == COMP_SUCCESS || skip_td) {
		frame->actual_length = frame->length;
		td->urb->actual_length += frame->length;
	} else {
		for (cur_trb = ep_ring->dequeue,
		     cur_seg = ep_ring->deq_seg; cur_trb != event_trb;
		     next_trb(xhci, ep_ring, &cur_seg, &cur_trb)) {
			if (!TRB_TYPE_NOOP_LE32(cur_trb->generic.field[3]) &&
			    !TRB_TYPE_LINK_LE32(cur_trb->generic.field[3]))
				len += TRB_LEN(le32_to_cpu(cur_trb->generic.field[2]));
		}
		len += TRB_LEN(le32_to_cpu(cur_trb->generic.field[2])) -
			EVENT_TRB_LEN(le32_to_cpu(event->transfer_len));

		if (trb_comp_code != COMP_STOP_INVAL) {
			frame->actual_length = len;
			td->urb->actual_length += len;
		}
	}

	return finish_td(xhci, td, event_trb, event, ep, status, false);
}

static int skip_isoc_td(struct xhci_hcd *xhci, struct xhci_td *td,
			struct xhci_transfer_event *event,
			struct xhci_virt_ep *ep, int *status)
{
	struct xhci_ring *ep_ring;
=======
	if (td->urb_length_set)
		goto finish_td;

	if (sum_trbs_for_length)
		frame->actual_length = sum_trb_lengths(xhci, ep->ring, ep_trb) +
			ep_trb_len - remaining;
	else
		frame->actual_length = requested;

	td->urb->actual_length += frame->actual_length;

finish_td:
	/* Don't give back TD yet if we encountered an error mid TD */
	if (td->error_mid_td && ep_trb != td->last_trb) {
		xhci_dbg(xhci, "Error mid isoc TD, wait for final completion event\n");
		td->urb_length_set = true;
		return 0;
	}

	return finish_td(xhci, ep, ep_ring, td, trb_comp_code);
}

static int skip_isoc_td(struct xhci_hcd *xhci, struct xhci_td *td,
			struct xhci_virt_ep *ep, int status)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct urb_priv *urb_priv;
	struct usb_iso_packet_descriptor *frame;
	int idx;

<<<<<<< HEAD
	ep_ring = xhci_dma_to_transfer_ring(ep, le64_to_cpu(event->buffer));
	urb_priv = td->urb->hcpriv;
	idx = urb_priv->td_cnt;
=======
	urb_priv = td->urb->hcpriv;
	idx = urb_priv->num_tds_done;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	frame = &td->urb->iso_frame_desc[idx];

	/* The transfer is partly done. */
	frame->status = -EXDEV;

	/* calc actual length */
	frame->actual_length = 0;

	/* Update ring dequeue pointer */
<<<<<<< HEAD
	while (ep_ring->dequeue != td->last_trb)
		inc_deq(xhci, ep_ring);
	inc_deq(xhci, ep_ring);

	return finish_td(xhci, td, NULL, event, ep, status, true);
=======
	ep->ring->dequeue = td->last_trb;
	ep->ring->deq_seg = td->last_trb_seg;
	inc_deq(xhci, ep->ring);

	return xhci_td_cleanup(xhci, td, ep->ring, status);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Process bulk and interrupt tds, update urb status and actual_length.
 */
<<<<<<< HEAD
static int process_bulk_intr_td(struct xhci_hcd *xhci, struct xhci_td *td,
	union xhci_trb *event_trb, struct xhci_transfer_event *event,
	struct xhci_virt_ep *ep, int *status)
{
	struct xhci_ring *ep_ring;
	union xhci_trb *cur_trb;
	struct xhci_segment *cur_seg;
	u32 trb_comp_code;

	ep_ring = xhci_dma_to_transfer_ring(ep, le64_to_cpu(event->buffer));
	trb_comp_code = GET_COMP_CODE(le32_to_cpu(event->transfer_len));

	switch (trb_comp_code) {
	case COMP_SUCCESS:
		/* Double check that the HW transferred everything. */
		if (event_trb != td->last_trb ||
		    EVENT_TRB_LEN(le32_to_cpu(event->transfer_len)) != 0) {
			xhci_warn(xhci, "WARN Successful completion "
					"on short TX\n");
			if (td->urb->transfer_flags & URB_SHORT_NOT_OK)
				*status = -EREMOTEIO;
			else
				*status = 0;
			if ((xhci->quirks & XHCI_TRUST_TX_LENGTH))
				trb_comp_code = COMP_SHORT_TX;
		} else {
			*status = 0;
		}
		break;
	case COMP_SHORT_TX:
		if (td->urb->transfer_flags & URB_SHORT_NOT_OK)
			*status = -EREMOTEIO;
		else
			*status = 0;
		break;
	default:
		/* Others already handled above */
		break;
	}
	if (trb_comp_code == COMP_SHORT_TX)
		xhci_dbg(xhci, "ep %#x - asked for %d bytes, "
				"%d bytes untransferred\n",
				td->urb->ep->desc.bEndpointAddress,
				td->urb->transfer_buffer_length,
				EVENT_TRB_LEN(le32_to_cpu(event->transfer_len)));
	/* Fast path - was this the last TRB in the TD for this URB? */
	if (event_trb == td->last_trb) {
		if (EVENT_TRB_LEN(le32_to_cpu(event->transfer_len)) != 0) {
			td->urb->actual_length =
				td->urb->transfer_buffer_length -
				EVENT_TRB_LEN(le32_to_cpu(event->transfer_len));
			if (td->urb->transfer_buffer_length <
					td->urb->actual_length) {
				xhci_warn(xhci, "HC gave bad length "
						"of %d bytes left\n",
					  EVENT_TRB_LEN(le32_to_cpu(event->transfer_len)));
				td->urb->actual_length = 0;
				if (td->urb->transfer_flags & URB_SHORT_NOT_OK)
					*status = -EREMOTEIO;
				else
					*status = 0;
			}
			/* Don't overwrite a previously set error code */
			if (*status == -EINPROGRESS) {
				if (td->urb->transfer_flags & URB_SHORT_NOT_OK)
					*status = -EREMOTEIO;
				else
					*status = 0;
			}
		} else {
			td->urb->actual_length =
				td->urb->transfer_buffer_length;
			/* Ignore a short packet completion if the
			 * untransferred length was zero.
			 */
			if (*status == -EREMOTEIO)
				*status = 0;
		}
	} else {
		/* Slow path - walk the list, starting from the dequeue
		 * pointer, to get the actual length transferred.
		 */
		td->urb->actual_length = 0;
		for (cur_trb = ep_ring->dequeue, cur_seg = ep_ring->deq_seg;
				cur_trb != event_trb;
				next_trb(xhci, ep_ring, &cur_seg, &cur_trb)) {
			if (!TRB_TYPE_NOOP_LE32(cur_trb->generic.field[3]) &&
			    !TRB_TYPE_LINK_LE32(cur_trb->generic.field[3]))
				td->urb->actual_length +=
					TRB_LEN(le32_to_cpu(cur_trb->generic.field[2]));
		}
		/* If the ring didn't stop on a Link or No-op TRB, add
		 * in the actual bytes transferred from the Normal TRB
		 */
		if (trb_comp_code != COMP_STOP_INVAL)
			td->urb->actual_length +=
				TRB_LEN(le32_to_cpu(cur_trb->generic.field[2])) -
				EVENT_TRB_LEN(le32_to_cpu(event->transfer_len));
	}

	return finish_td(xhci, td, event_trb, event, ep, status, false);
=======
static int process_bulk_intr_td(struct xhci_hcd *xhci, struct xhci_virt_ep *ep,
		struct xhci_ring *ep_ring, struct xhci_td *td,
		union xhci_trb *ep_trb, struct xhci_transfer_event *event)
{
	struct xhci_slot_ctx *slot_ctx;
	u32 trb_comp_code;
	u32 remaining, requested, ep_trb_len;

	slot_ctx = xhci_get_slot_ctx(xhci, ep->vdev->out_ctx);
	trb_comp_code = GET_COMP_CODE(le32_to_cpu(event->transfer_len));
	remaining = EVENT_TRB_LEN(le32_to_cpu(event->transfer_len));
	ep_trb_len = TRB_LEN(le32_to_cpu(ep_trb->generic.field[2]));
	requested = td->urb->transfer_buffer_length;

	switch (trb_comp_code) {
	case COMP_SUCCESS:
		ep->err_count = 0;
		/* handle success with untransferred data as short packet */
		if (ep_trb != td->last_trb || remaining) {
			xhci_warn(xhci, "WARN Successful completion on short TX\n");
			xhci_dbg(xhci, "ep %#x - asked for %d bytes, %d bytes untransferred\n",
				 td->urb->ep->desc.bEndpointAddress,
				 requested, remaining);
		}
		td->status = 0;
		break;
	case COMP_SHORT_PACKET:
		xhci_dbg(xhci, "ep %#x - asked for %d bytes, %d bytes untransferred\n",
			 td->urb->ep->desc.bEndpointAddress,
			 requested, remaining);
		td->status = 0;
		break;
	case COMP_STOPPED_SHORT_PACKET:
		td->urb->actual_length = remaining;
		goto finish_td;
	case COMP_STOPPED_LENGTH_INVALID:
		/* stopped on ep trb with invalid length, exclude it */
		ep_trb_len	= 0;
		remaining	= 0;
		break;
	case COMP_USB_TRANSACTION_ERROR:
		if (xhci->quirks & XHCI_NO_SOFT_RETRY ||
		    (ep->err_count++ > MAX_SOFT_RETRY) ||
		    le32_to_cpu(slot_ctx->tt_info) & TT_SLOT)
			break;

		td->status = 0;

		xhci_handle_halted_endpoint(xhci, ep, td, EP_SOFT_RESET);
		return 0;
	default:
		/* do nothing */
		break;
	}

	if (ep_trb == td->last_trb)
		td->urb->actual_length = requested - remaining;
	else
		td->urb->actual_length =
			sum_trb_lengths(xhci, ep_ring, ep_trb) +
			ep_trb_len - remaining;
finish_td:
	if (remaining > requested) {
		xhci_warn(xhci, "bad transfer trb length %d in event trb\n",
			  remaining);
		td->urb->actual_length = 0;
	}

	return finish_td(xhci, ep, ep_ring, td, trb_comp_code);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * If this function returns an error condition, it means it got a Transfer
 * event with a corrupted Slot ID, Endpoint ID, or TRB DMA address.
 * At this point, the host controller is probably hosed and should be reset.
 */
static int handle_tx_event(struct xhci_hcd *xhci,
<<<<<<< HEAD
		struct xhci_transfer_event *event)
{
	struct xhci_virt_device *xdev;
=======
			   struct xhci_interrupter *ir,
			   struct xhci_transfer_event *event)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct xhci_virt_ep *ep;
	struct xhci_ring *ep_ring;
	unsigned int slot_id;
	int ep_index;
	struct xhci_td *td = NULL;
<<<<<<< HEAD
	dma_addr_t event_dma;
	struct xhci_segment *event_seg;
	union xhci_trb *event_trb;
	struct urb *urb = NULL;
	int status = -EINPROGRESS;
	struct urb_priv *urb_priv;
	struct xhci_ep_ctx *ep_ctx;
	struct list_head *tmp;
	u32 trb_comp_code;
	int ret = 0;
=======
	dma_addr_t ep_trb_dma;
	struct xhci_segment *ep_seg;
	union xhci_trb *ep_trb;
	int status = -EINPROGRESS;
	struct xhci_ep_ctx *ep_ctx;
	u32 trb_comp_code;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int td_num = 0;
	bool handling_skipped_tds = false;

	slot_id = TRB_TO_SLOT_ID(le32_to_cpu(event->flags));
<<<<<<< HEAD
	xdev = xhci->devs[slot_id];
	if (!xdev) {
		xhci_err(xhci, "ERROR Transfer event pointed to bad slot\n");
		xhci_err(xhci, "@%016llx %08x %08x %08x %08x\n",
			 (unsigned long long) xhci_trb_virt_to_dma(
				 xhci->event_ring->deq_seg,
				 xhci->event_ring->dequeue),
			 lower_32_bits(le64_to_cpu(event->buffer)),
			 upper_32_bits(le64_to_cpu(event->buffer)),
			 le32_to_cpu(event->transfer_len),
			 le32_to_cpu(event->flags));
		xhci_dbg(xhci, "Event ring:\n");
		xhci_debug_segment(xhci, xhci->event_ring->deq_seg);
		return -ENODEV;
	}

	/* Endpoint ID is 1 based, our index is zero based */
	ep_index = TRB_TO_EP_ID(le32_to_cpu(event->flags)) - 1;
	ep = &xdev->eps[ep_index];
	ep_ring = xhci_dma_to_transfer_ring(ep, le64_to_cpu(event->buffer));
	ep_ctx = xhci_get_ep_ctx(xhci, xdev->out_ctx, ep_index);
	if (!ep_ring ||
	    (le32_to_cpu(ep_ctx->ep_info) & EP_STATE_MASK) ==
	    EP_STATE_DISABLED) {
		xhci_err(xhci, "ERROR Transfer event for disabled endpoint "
				"or incorrect stream ring\n");
		xhci_err(xhci, "@%016llx %08x %08x %08x %08x\n",
			 (unsigned long long) xhci_trb_virt_to_dma(
				 xhci->event_ring->deq_seg,
				 xhci->event_ring->dequeue),
			 lower_32_bits(le64_to_cpu(event->buffer)),
			 upper_32_bits(le64_to_cpu(event->buffer)),
			 le32_to_cpu(event->transfer_len),
			 le32_to_cpu(event->flags));
		xhci_dbg(xhci, "Event ring:\n");
		xhci_debug_segment(xhci, xhci->event_ring->deq_seg);
		return -ENODEV;
	}

	/* Count current td numbers if ep->skip is set */
	if (ep->skip) {
		list_for_each(tmp, &ep_ring->td_list)
			td_num++;
	}

	event_dma = le64_to_cpu(event->buffer);
	trb_comp_code = GET_COMP_CODE(le32_to_cpu(event->transfer_len));
=======
	ep_index = TRB_TO_EP_ID(le32_to_cpu(event->flags)) - 1;
	trb_comp_code = GET_COMP_CODE(le32_to_cpu(event->transfer_len));
	ep_trb_dma = le64_to_cpu(event->buffer);

	ep = xhci_get_virt_ep(xhci, slot_id, ep_index);
	if (!ep) {
		xhci_err(xhci, "ERROR Invalid Transfer event\n");
		goto err_out;
	}

	ep_ring = xhci_dma_to_transfer_ring(ep, ep_trb_dma);
	ep_ctx = xhci_get_ep_ctx(xhci, ep->vdev->out_ctx, ep_index);

	if (GET_EP_CTX_STATE(ep_ctx) == EP_STATE_DISABLED) {
		xhci_err(xhci,
			 "ERROR Transfer event for disabled endpoint slot %u ep %u\n",
			  slot_id, ep_index);
		goto err_out;
	}

	/* Some transfer events don't always point to a trb, see xhci 4.17.4 */
	if (!ep_ring) {
		switch (trb_comp_code) {
		case COMP_STALL_ERROR:
		case COMP_USB_TRANSACTION_ERROR:
		case COMP_INVALID_STREAM_TYPE_ERROR:
		case COMP_INVALID_STREAM_ID_ERROR:
			xhci_dbg(xhci, "Stream transaction error ep %u no id\n",
				 ep_index);
			if (ep->err_count++ > MAX_SOFT_RETRY)
				xhci_handle_halted_endpoint(xhci, ep, NULL,
							    EP_HARD_RESET);
			else
				xhci_handle_halted_endpoint(xhci, ep, NULL,
							    EP_SOFT_RESET);
			goto cleanup;
		case COMP_RING_UNDERRUN:
		case COMP_RING_OVERRUN:
		case COMP_STOPPED_LENGTH_INVALID:
			goto cleanup;
		default:
			xhci_err(xhci, "ERROR Transfer event for unknown stream ring slot %u ep %u\n",
				 slot_id, ep_index);
			goto err_out;
		}
	}

	/* Count current td numbers if ep->skip is set */
	if (ep->skip)
		td_num += list_count_nodes(&ep_ring->td_list);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Look for common error cases */
	switch (trb_comp_code) {
	/* Skip codes that require special handling depending on
	 * transfer type
	 */
	case COMP_SUCCESS:
		if (EVENT_TRB_LEN(le32_to_cpu(event->transfer_len)) == 0)
			break;
<<<<<<< HEAD
		if (xhci->quirks & XHCI_TRUST_TX_LENGTH)
			trb_comp_code = COMP_SHORT_TX;
		else
			xhci_warn(xhci, "WARN Successful completion on short TX: "
					"needs XHCI_TRUST_TX_LENGTH quirk?\n");
	case COMP_SHORT_TX:
		break;
	case COMP_STOP:
		xhci_dbg(xhci, "Stopped on Transfer TRB\n");
		break;
	case COMP_STOP_INVAL:
		xhci_dbg(xhci, "Stopped on No-op or Link TRB\n");
		break;
	case COMP_STALL:
		xhci_dbg(xhci, "Stalled endpoint\n");
		ep->ep_state |= EP_HALTED;
		status = -EPIPE;
		break;
	case COMP_TRB_ERR:
		xhci_warn(xhci, "WARN: TRB error on endpoint\n");
		status = -EILSEQ;
		break;
	case COMP_SPLIT_ERR:
	case COMP_TX_ERR:
		xhci_dbg(xhci, "Transfer error on endpoint\n");
		status = -EPROTO;
		break;
	case COMP_BABBLE:
		xhci_dbg(xhci, "Babble error on endpoint\n");
		status = -EOVERFLOW;
		break;
	case COMP_DB_ERR:
		xhci_warn(xhci, "WARN: HC couldn't access mem fast enough\n");
		status = -ENOSR;
		break;
	case COMP_BW_OVER:
		xhci_warn(xhci, "WARN: bandwidth overrun event on endpoint\n");
		break;
	case COMP_BUFF_OVER:
		xhci_warn(xhci, "WARN: buffer overrun event on endpoint\n");
		break;
	case COMP_UNDERRUN:
=======
		if (xhci->quirks & XHCI_TRUST_TX_LENGTH ||
		    ep_ring->last_td_was_short)
			trb_comp_code = COMP_SHORT_PACKET;
		else
			xhci_warn_ratelimited(xhci,
					      "WARN Successful completion on short TX for slot %u ep %u: needs XHCI_TRUST_TX_LENGTH quirk?\n",
					      slot_id, ep_index);
		break;
	case COMP_SHORT_PACKET:
		break;
	/* Completion codes for endpoint stopped state */
	case COMP_STOPPED:
		xhci_dbg(xhci, "Stopped on Transfer TRB for slot %u ep %u\n",
			 slot_id, ep_index);
		break;
	case COMP_STOPPED_LENGTH_INVALID:
		xhci_dbg(xhci,
			 "Stopped on No-op or Link TRB for slot %u ep %u\n",
			 slot_id, ep_index);
		break;
	case COMP_STOPPED_SHORT_PACKET:
		xhci_dbg(xhci,
			 "Stopped with short packet transfer detected for slot %u ep %u\n",
			 slot_id, ep_index);
		break;
	/* Completion codes for endpoint halted state */
	case COMP_STALL_ERROR:
		xhci_dbg(xhci, "Stalled endpoint for slot %u ep %u\n", slot_id,
			 ep_index);
		status = -EPIPE;
		break;
	case COMP_SPLIT_TRANSACTION_ERROR:
		xhci_dbg(xhci, "Split transaction error for slot %u ep %u\n",
			 slot_id, ep_index);
		status = -EPROTO;
		break;
	case COMP_USB_TRANSACTION_ERROR:
		xhci_dbg(xhci, "Transfer error for slot %u ep %u on endpoint\n",
			 slot_id, ep_index);
		status = -EPROTO;
		break;
	case COMP_BABBLE_DETECTED_ERROR:
		xhci_dbg(xhci, "Babble error for slot %u ep %u on endpoint\n",
			 slot_id, ep_index);
		status = -EOVERFLOW;
		break;
	/* Completion codes for endpoint error state */
	case COMP_TRB_ERROR:
		xhci_warn(xhci,
			  "WARN: TRB error for slot %u ep %u on endpoint\n",
			  slot_id, ep_index);
		status = -EILSEQ;
		break;
	/* completion codes not indicating endpoint state change */
	case COMP_DATA_BUFFER_ERROR:
		xhci_warn(xhci,
			  "WARN: HC couldn't access mem fast enough for slot %u ep %u\n",
			  slot_id, ep_index);
		status = -ENOSR;
		break;
	case COMP_BANDWIDTH_OVERRUN_ERROR:
		xhci_warn(xhci,
			  "WARN: bandwidth overrun event for slot %u ep %u on endpoint\n",
			  slot_id, ep_index);
		break;
	case COMP_ISOCH_BUFFER_OVERRUN:
		xhci_warn(xhci,
			  "WARN: buffer overrun event for slot %u ep %u on endpoint",
			  slot_id, ep_index);
		break;
	case COMP_RING_UNDERRUN:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * When the Isoch ring is empty, the xHC will generate
		 * a Ring Overrun Event for IN Isoch endpoint or Ring
		 * Underrun Event for OUT Isoch endpoint.
		 */
		xhci_dbg(xhci, "underrun event on endpoint\n");
		if (!list_empty(&ep_ring->td_list))
			xhci_dbg(xhci, "Underrun Event for slot %d ep %d "
					"still with TDs queued?\n",
				 TRB_TO_SLOT_ID(le32_to_cpu(event->flags)),
				 ep_index);
		goto cleanup;
<<<<<<< HEAD
	case COMP_OVERRUN:
=======
	case COMP_RING_OVERRUN:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		xhci_dbg(xhci, "overrun event on endpoint\n");
		if (!list_empty(&ep_ring->td_list))
			xhci_dbg(xhci, "Overrun Event for slot %d ep %d "
					"still with TDs queued?\n",
				 TRB_TO_SLOT_ID(le32_to_cpu(event->flags)),
				 ep_index);
		goto cleanup;
<<<<<<< HEAD
	case COMP_DEV_ERR:
		xhci_warn(xhci, "WARN: detect an incompatible device");
		status = -EPROTO;
		break;
	case COMP_MISSED_INT:
=======
	case COMP_MISSED_SERVICE_ERROR:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * When encounter missed service error, one or more isoc tds
		 * may be missed by xHC.
		 * Set skip flag of the ep_ring; Complete the missed tds as
		 * short transfer when process the ep_ring next time.
		 */
		ep->skip = true;
<<<<<<< HEAD
		xhci_dbg(xhci, "Miss service interval error, set skip flag\n");
		goto cleanup;
	case COMP_PING_ERR:
		ep->skip = true;
		xhci_dbg(xhci, "No Ping response error, Skip one Isoc TD\n");
		goto cleanup;
=======
		xhci_dbg(xhci,
			 "Miss service interval error for slot %u ep %u, set skip flag\n",
			 slot_id, ep_index);
		goto cleanup;
	case COMP_NO_PING_RESPONSE_ERROR:
		ep->skip = true;
		xhci_dbg(xhci,
			 "No Ping response error for slot %u ep %u, Skip one Isoc TD\n",
			 slot_id, ep_index);
		goto cleanup;

	case COMP_INCOMPATIBLE_DEVICE_ERROR:
		/* needs disable slot command to recover */
		xhci_warn(xhci,
			  "WARN: detect an incompatible device for slot %u ep %u",
			  slot_id, ep_index);
		status = -EPROTO;
		break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	default:
		if (xhci_is_vendor_info_code(xhci, trb_comp_code)) {
			status = 0;
			break;
		}
<<<<<<< HEAD
		xhci_warn(xhci, "ERROR Unknown event condition, HC probably "
				"busted\n");
=======
		xhci_warn(xhci,
			  "ERROR Unknown event condition %u for slot %u ep %u , HC probably busted\n",
			  trb_comp_code, slot_id, ep_index);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto cleanup;
	}

	do {
		/* This TRB should be in the TD at the head of this ring's
		 * TD list.
		 */
		if (list_empty(&ep_ring->td_list)) {
			/*
<<<<<<< HEAD
			 * A stopped endpoint may generate an extra completion
			 * event if the device was suspended.  Don't print
			 * warnings.
			 */
			if (!(trb_comp_code == COMP_STOP ||
						trb_comp_code == COMP_STOP_INVAL)) {
				xhci_warn(xhci, "WARN Event TRB for slot %d ep %d with no TDs queued?\n",
						TRB_TO_SLOT_ID(le32_to_cpu(event->flags)),
						ep_index);
				xhci_dbg(xhci, "Event TRB with TRB type ID %u\n",
						(le32_to_cpu(event->flags) &
						 TRB_TYPE_BITMASK)>>10);
				xhci_print_trb_offsets(xhci, (union xhci_trb *) event);
			}
			if (ep->skip) {
				ep->skip = false;
				xhci_dbg(xhci, "td_list is empty while skip "
						"flag set. Clear skip flag.\n");
			}
			ret = 0;
=======
			 * Don't print wanings if it's due to a stopped endpoint
			 * generating an extra completion event if the device
			 * was suspended. Or, a event for the last TRB of a
			 * short TD we already got a short event for.
			 * The short TD is already removed from the TD list.
			 */

			if (!(trb_comp_code == COMP_STOPPED ||
			      trb_comp_code == COMP_STOPPED_LENGTH_INVALID ||
			      ep_ring->last_td_was_short)) {
				xhci_warn(xhci, "WARN Event TRB for slot %d ep %d with no TDs queued?\n",
						TRB_TO_SLOT_ID(le32_to_cpu(event->flags)),
						ep_index);
			}
			if (ep->skip) {
				ep->skip = false;
				xhci_dbg(xhci, "td_list is empty while skip flag set. Clear skip flag for slot %u ep %u.\n",
					 slot_id, ep_index);
			}
			if (trb_comp_code == COMP_STALL_ERROR ||
			    xhci_requires_manual_halt_cleanup(xhci, ep_ctx,
							      trb_comp_code)) {
				xhci_handle_halted_endpoint(xhci, ep, NULL,
							    EP_HARD_RESET);
			}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto cleanup;
		}

		/* We've skipped all the TDs on the ep ring when ep->skip set */
		if (ep->skip && td_num == 0) {
			ep->skip = false;
<<<<<<< HEAD
			xhci_dbg(xhci, "All tds on the ep_ring skipped. "
						"Clear skip flag.\n");
			ret = 0;
			goto cleanup;
		}

		td = list_entry(ep_ring->td_list.next, struct xhci_td, td_list);
=======
			xhci_dbg(xhci, "All tds on the ep_ring skipped. Clear skip flag for slot %u ep %u.\n",
				 slot_id, ep_index);
			goto cleanup;
		}

		td = list_first_entry(&ep_ring->td_list, struct xhci_td,
				      td_list);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (ep->skip)
			td_num--;

		/* Is this a TRB in the currently executing TD? */
<<<<<<< HEAD
		event_seg = trb_in_td(ep_ring->deq_seg, ep_ring->dequeue,
				td->last_trb, event_dma);
=======
		ep_seg = trb_in_td(xhci, td->start_seg, td->first_trb,
				td->last_trb, ep_trb_dma, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/*
		 * Skip the Force Stopped Event. The event_trb(event_dma) of FSE
		 * is not in the current TD pointed by ep_ring->dequeue because
		 * that the hardware dequeue pointer still at the previous TRB
		 * of the current TD. The previous TRB maybe a Link TD or the
		 * last TRB of the previous TD. The command completion handle
		 * will take care the rest.
		 */
<<<<<<< HEAD
		if (!event_seg && (trb_comp_code == COMP_STOP ||
				   trb_comp_code == COMP_STOP_INVAL)) {
			ret = 0;
			goto cleanup;
		}

		if (!event_seg) {
			if (!ep->skip ||
			    !usb_endpoint_xfer_isoc(&td->urb->ep->desc)) {
				/* Some host controllers give a spurious
				 * successful event after a short transfer.
				 * Ignore it.
				 */
				if ((xhci->quirks & XHCI_SPURIOUS_SUCCESS) && 
						ep_ring->last_td_was_short) {
					ep_ring->last_td_was_short = false;
					ret = 0;
					goto cleanup;
				}
				/* HC is busted, give up! */
				xhci_err(xhci,
					"ERROR Transfer event TRB DMA ptr not "
					"part of current TD\n");
				return -ESHUTDOWN;
			}

			ret = skip_isoc_td(xhci, td, event, ep, &status);
			goto cleanup;
		}
		if (trb_comp_code == COMP_SHORT_TX)
=======
		if (!ep_seg && (trb_comp_code == COMP_STOPPED ||
			   trb_comp_code == COMP_STOPPED_LENGTH_INVALID)) {
			goto cleanup;
		}

		if (!ep_seg) {

			if (ep->skip && usb_endpoint_xfer_isoc(&td->urb->ep->desc)) {
				skip_isoc_td(xhci, td, ep, status);
				goto cleanup;
			}

			/*
			 * Some hosts give a spurious success event after a short
			 * transfer. Ignore it.
			 */
			if ((xhci->quirks & XHCI_SPURIOUS_SUCCESS) &&
			    ep_ring->last_td_was_short) {
				ep_ring->last_td_was_short = false;
				goto cleanup;
			}

			/*
			 * xhci 4.10.2 states isoc endpoints should continue
			 * processing the next TD if there was an error mid TD.
			 * So host like NEC don't generate an event for the last
			 * isoc TRB even if the IOC flag is set.
			 * xhci 4.9.1 states that if there are errors in mult-TRB
			 * TDs xHC should generate an error for that TRB, and if xHC
			 * proceeds to the next TD it should genete an event for
			 * any TRB with IOC flag on the way. Other host follow this.
			 * So this event might be for the next TD.
			 */
			if (td->error_mid_td &&
			    !list_is_last(&td->td_list, &ep_ring->td_list)) {
				struct xhci_td *td_next = list_next_entry(td, td_list);

				ep_seg = trb_in_td(xhci, td_next->start_seg, td_next->first_trb,
						   td_next->last_trb, ep_trb_dma, false);
				if (ep_seg) {
					/* give back previous TD, start handling new */
					xhci_dbg(xhci, "Missing TD completion event after mid TD error\n");
					ep_ring->dequeue = td->last_trb;
					ep_ring->deq_seg = td->last_trb_seg;
					inc_deq(xhci, ep_ring);
					xhci_td_cleanup(xhci, td, ep_ring, td->status);
					td = td_next;
				}
			}

			if (!ep_seg) {
				/* HC is busted, give up! */
				xhci_err(xhci,
					"ERROR Transfer event TRB DMA ptr not "
					"part of current TD ep_index %d "
					"comp_code %u\n", ep_index,
					trb_comp_code);
				trb_in_td(xhci, td->start_seg, td->first_trb,
					  td->last_trb, ep_trb_dma, true);
				return -ESHUTDOWN;
			}
		}
		if (trb_comp_code == COMP_SHORT_PACKET)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			ep_ring->last_td_was_short = true;
		else
			ep_ring->last_td_was_short = false;

		if (ep->skip) {
<<<<<<< HEAD
			xhci_dbg(xhci, "Found td. Clear skip flag.\n");
			ep->skip = false;
		}

		event_trb = &event_seg->trbs[(event_dma - event_seg->dma) /
						sizeof(*event_trb)];
		/*
		 * No-op TRB should not trigger interrupts.
		 * If event_trb is a no-op TRB, it means the
		 * corresponding TD has been cancelled. Just ignore
		 * the TD.
		 */
		if (TRB_TYPE_NOOP_LE32(event_trb->generic.field[3])) {
			xhci_dbg(xhci,
				 "event_trb is a no-op TRB. Skip it\n");
			goto cleanup;
		}

		/* Now update the urb's actual_length and give back to
		 * the core
		 */
		if (usb_endpoint_xfer_control(&td->urb->ep->desc))
			ret = process_ctrl_td(xhci, td, event_trb, event, ep,
						 &status);
		else if (usb_endpoint_xfer_isoc(&td->urb->ep->desc))
			ret = process_isoc_td(xhci, td, event_trb, event, ep,
						 &status);
		else
			ret = process_bulk_intr_td(xhci, td, event_trb, event,
						 ep, &status);

cleanup:


		handling_skipped_tds = ep->skip &&
			trb_comp_code != COMP_MISSED_INT &&
			trb_comp_code != COMP_PING_ERR;

		/*
		 * Do not update event ring dequeue pointer if we're in a loop
		 * processing missed tds.
		 */
		if (!handling_skipped_tds)
			inc_deq(xhci, xhci->event_ring);

		if (ret) {
			urb = td->urb;
			urb_priv = urb->hcpriv;

			xhci_urb_free_priv(xhci, urb_priv);

			usb_hcd_unlink_urb_from_ep(bus_to_hcd(urb->dev->bus), urb);
			if ((urb->actual_length != urb->transfer_buffer_length &&
						(urb->transfer_flags &
						 URB_SHORT_NOT_OK)) ||
					(status != 0 &&
					 !usb_endpoint_xfer_isoc(&urb->ep->desc)))
				xhci_dbg(xhci, "Giveback URB %p, len = %d, "
						"expected = %x, status = %d\n",
						urb, urb->actual_length,
						urb->transfer_buffer_length,
						status);
			spin_unlock(&xhci->lock);
			/* EHCI, UHCI, and OHCI always unconditionally set the
			 * urb->status of an isochronous endpoint to 0.
			 */
			if (usb_pipetype(urb->pipe) == PIPE_ISOCHRONOUS)
				status = 0;
			usb_hcd_giveback_urb(bus_to_hcd(urb->dev->bus), urb, status);
			spin_lock(&xhci->lock);
		}
=======
			xhci_dbg(xhci,
				 "Found td. Clear skip flag for slot %u ep %u.\n",
				 slot_id, ep_index);
			ep->skip = false;
		}

		ep_trb = &ep_seg->trbs[(ep_trb_dma - ep_seg->dma) /
						sizeof(*ep_trb)];

		trace_xhci_handle_transfer(ep_ring,
				(struct xhci_generic_trb *) ep_trb);

		/*
		 * No-op TRB could trigger interrupts in a case where
		 * a URB was killed and a STALL_ERROR happens right
		 * after the endpoint ring stopped. Reset the halted
		 * endpoint. Otherwise, the endpoint remains stalled
		 * indefinitely.
		 */

		if (trb_is_noop(ep_trb)) {
			if (trb_comp_code == COMP_STALL_ERROR ||
			    xhci_requires_manual_halt_cleanup(xhci, ep_ctx,
							      trb_comp_code))
				xhci_handle_halted_endpoint(xhci, ep, td,
							    EP_HARD_RESET);
			goto cleanup;
		}

		td->status = status;

		/* update the urb's actual_length and give back to the core */
		if (usb_endpoint_xfer_control(&td->urb->ep->desc))
			process_ctrl_td(xhci, ep, ep_ring, td, ep_trb, event);
		else if (usb_endpoint_xfer_isoc(&td->urb->ep->desc))
			process_isoc_td(xhci, ep, ep_ring, td, ep_trb, event);
		else
			process_bulk_intr_td(xhci, ep, ep_ring, td, ep_trb, event);
cleanup:
		handling_skipped_tds = ep->skip &&
			trb_comp_code != COMP_MISSED_SERVICE_ERROR &&
			trb_comp_code != COMP_NO_PING_RESPONSE_ERROR;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * If ep->skip is set, it means there are missed tds on the
	 * endpoint ring need to take care of.
	 * Process them as short transfer until reach the td pointed by
	 * the event.
	 */
	} while (handling_skipped_tds);

	return 0;
<<<<<<< HEAD
}

/*
 * This function handles all OS-owned events on the event ring.  It may drop
 * xhci->lock between event processing (e.g. to pass up port status changes).
 * Returns >0 for "possibly more events to process" (caller should call again),
 * otherwise 0 if done.  In future, <0 returns should indicate error code.
 */
static int xhci_handle_event(struct xhci_hcd *xhci)
{
	union xhci_trb *event;
	int update_ptrs = 1;
	int ret;

	if (!xhci->event_ring || !xhci->event_ring->dequeue) {
		xhci->error_bitmask |= 1 << 1;
		return 0;
	}

	event = xhci->event_ring->dequeue;
	/* Does the HC or OS own the TRB? */
	if ((le32_to_cpu(event->event_cmd.flags) & TRB_CYCLE) !=
	    xhci->event_ring->cycle_state) {
		xhci->error_bitmask |= 1 << 2;
		return 0;
	}

	/*
	 * Barrier between reading the TRB_CYCLE (valid) flag above and any
	 * speculative reads of the event's flags/data below.
	 */
	rmb();
	/* FIXME: Handle more event types. */
	switch ((le32_to_cpu(event->event_cmd.flags) & TRB_TYPE_BITMASK)) {
	case TRB_TYPE(TRB_COMPLETION):
		handle_cmd_completion(xhci, &event->event_cmd);
		break;
	case TRB_TYPE(TRB_PORT_STATUS):
		handle_port_status(xhci, event);
		update_ptrs = 0;
		break;
	case TRB_TYPE(TRB_TRANSFER):
		ret = handle_tx_event(xhci, &event->trans_event);
		if (ret < 0)
			xhci->error_bitmask |= 1 << 9;
		else
			update_ptrs = 0;
		break;
	case TRB_TYPE(TRB_DEV_NOTE):
		handle_device_notification(xhci, event);
		break;
	default:
		if ((le32_to_cpu(event->event_cmd.flags) & TRB_TYPE_BITMASK) >=
		    TRB_TYPE(48))
			handle_vendor_event(xhci, event);
		else
			xhci->error_bitmask |= 1 << 3;
=======

err_out:
	xhci_err(xhci, "@%016llx %08x %08x %08x %08x\n",
		 (unsigned long long) xhci_trb_virt_to_dma(
			 ir->event_ring->deq_seg,
			 ir->event_ring->dequeue),
		 lower_32_bits(le64_to_cpu(event->buffer)),
		 upper_32_bits(le64_to_cpu(event->buffer)),
		 le32_to_cpu(event->transfer_len),
		 le32_to_cpu(event->flags));
	return -ENODEV;
}

/*
 * This function handles one OS-owned event on the event ring. It may drop
 * xhci->lock between event processing (e.g. to pass up port status changes).
 */
static int xhci_handle_event_trb(struct xhci_hcd *xhci, struct xhci_interrupter *ir,
				 union xhci_trb *event)
{
	u32 trb_type;

	trace_xhci_handle_event(ir->event_ring, &event->generic);

	/*
	 * Barrier between reading the TRB_CYCLE (valid) flag before, and any
	 * speculative reads of the event's flags/data below.
	 */
	rmb();
	trb_type = TRB_FIELD_TO_TYPE(le32_to_cpu(event->event_cmd.flags));
	/* FIXME: Handle more event types. */

	switch (trb_type) {
	case TRB_COMPLETION:
		handle_cmd_completion(xhci, &event->event_cmd);
		break;
	case TRB_PORT_STATUS:
		handle_port_status(xhci, ir, event);
		break;
	case TRB_TRANSFER:
		handle_tx_event(xhci, ir, &event->trans_event);
		break;
	case TRB_DEV_NOTE:
		handle_device_notification(xhci, event);
		break;
	default:
		if (trb_type >= TRB_VENDOR_DEFINED_LOW)
			handle_vendor_event(xhci, event, trb_type);
		else
			xhci_warn(xhci, "ERROR unknown event type %d\n", trb_type);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	/* Any of the above functions may drop and re-acquire the lock, so check
	 * to make sure a watchdog timer didn't mark the host as non-responsive.
	 */
	if (xhci->xhc_state & XHCI_STATE_DYING) {
<<<<<<< HEAD
		xhci_dbg(xhci, "xHCI host dying, returning from "
				"event handler.\n");
		return 0;
	}

	if (update_ptrs)
		/* Update SW event ring dequeue pointer */
		inc_deq(xhci, xhci->event_ring);

	/* Are there more items on the event ring?  Caller will call us again to
	 * check.
	 */
	return 1;
=======
		xhci_dbg(xhci, "xHCI host dying, returning from event handler.\n");
		return -ENODEV;
	}

	return 0;
}

/*
 * Update Event Ring Dequeue Pointer:
 * - When all events have finished
 * - To avoid "Event Ring Full Error" condition
 */
static void xhci_update_erst_dequeue(struct xhci_hcd *xhci,
				     struct xhci_interrupter *ir,
				     bool clear_ehb)
{
	u64 temp_64;
	dma_addr_t deq;

	temp_64 = xhci_read_64(xhci, &ir->ir_set->erst_dequeue);
	deq = xhci_trb_virt_to_dma(ir->event_ring->deq_seg,
				   ir->event_ring->dequeue);
	if (deq == 0)
		xhci_warn(xhci, "WARN something wrong with SW event ring dequeue ptr\n");
	/*
	 * Per 4.9.4, Software writes to the ERDP register shall always advance
	 * the Event Ring Dequeue Pointer value.
	 */
	if ((temp_64 & ERST_PTR_MASK) == (deq & ERST_PTR_MASK) && !clear_ehb)
		return;

	/* Update HC event ring dequeue pointer */
	temp_64 = ir->event_ring->deq_seg->num & ERST_DESI_MASK;
	temp_64 |= deq & ERST_PTR_MASK;

	/* Clear the event handler busy flag (RW1C) */
	if (clear_ehb)
		temp_64 |= ERST_EHB;
	xhci_write_64(xhci, temp_64, &ir->ir_set->erst_dequeue);
}

/* Clear the interrupt pending bit for a specific interrupter. */
static void xhci_clear_interrupt_pending(struct xhci_hcd *xhci,
					 struct xhci_interrupter *ir)
{
	if (!ir->ip_autoclear) {
		u32 irq_pending;

		irq_pending = readl(&ir->ir_set->irq_pending);
		irq_pending |= IMAN_IP;
		writel(irq_pending, &ir->ir_set->irq_pending);
	}
}

/*
 * Handle all OS-owned events on an interrupter event ring. It may drop
 * and reaquire xhci->lock between event processing.
 */
static int xhci_handle_events(struct xhci_hcd *xhci, struct xhci_interrupter *ir)
{
	int event_loop = 0;
	int err;
	u64 temp;

	xhci_clear_interrupt_pending(xhci, ir);

	/* Event ring hasn't been allocated yet. */
	if (!ir->event_ring || !ir->event_ring->dequeue) {
		xhci_err(xhci, "ERROR interrupter event ring not ready\n");
		return -ENOMEM;
	}

	if (xhci->xhc_state & XHCI_STATE_DYING ||
	    xhci->xhc_state & XHCI_STATE_HALTED) {
		xhci_dbg(xhci, "xHCI dying, ignoring interrupt. Shouldn't IRQs be disabled?\n");

		/* Clear the event handler busy flag (RW1C) */
		temp = xhci_read_64(xhci, &ir->ir_set->erst_dequeue);
		xhci_write_64(xhci, temp | ERST_EHB, &ir->ir_set->erst_dequeue);
		return -ENODEV;
	}

	/* Process all OS owned event TRBs on this event ring */
	while (unhandled_event_trb(ir->event_ring)) {
		err = xhci_handle_event_trb(xhci, ir, ir->event_ring->dequeue);

		/*
		 * If half a segment of events have been handled in one go then
		 * update ERDP, and force isoc trbs to interrupt more often
		 */
		if (event_loop++ > TRBS_PER_SEGMENT / 2) {
			xhci_update_erst_dequeue(xhci, ir, false);

			if (ir->isoc_bei_interval > AVOID_BEI_INTERVAL_MIN)
				ir->isoc_bei_interval = ir->isoc_bei_interval / 2;

			event_loop = 0;
		}

		/* Update SW event ring dequeue pointer */
		inc_deq(xhci, ir->event_ring);

		if (err)
			break;
	}

	xhci_update_erst_dequeue(xhci, ir, true);

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * xHCI spec says we can get an interrupt, and if the HC has an error condition,
 * we might get bad data out of the event ring.  Section 4.10.2.7 has a list of
 * indicators of an event TRB error, but we check the status *first* to be safe.
 */
irqreturn_t xhci_irq(struct usb_hcd *hcd)
{
	struct xhci_hcd *xhci = hcd_to_xhci(hcd);
<<<<<<< HEAD
	u32 status;
	union xhci_trb *trb;
	u64 temp_64;
	union xhci_trb *event_ring_deq;
	dma_addr_t deq;

	spin_lock(&xhci->lock);
	trb = xhci->event_ring->dequeue;
	/* Check if the xHC generated the interrupt, or the irq is shared */
	status = xhci_readl(xhci, &xhci->op_regs->status);
	if (status == 0xffffffff)
		goto hw_died;

	if (!(status & STS_EINT)) {
		spin_unlock(&xhci->lock);
		return IRQ_NONE;
	}
	if (status & STS_FATAL) {
		xhci_warn(xhci, "WARNING: Host System Error\n");
		xhci_halt(xhci);
hw_died:
		spin_unlock(&xhci->lock);
		return IRQ_HANDLED;
=======
	irqreturn_t ret = IRQ_HANDLED;
	u32 status;

	spin_lock(&xhci->lock);
	/* Check if the xHC generated the interrupt, or the irq is shared */
	status = readl(&xhci->op_regs->status);
	if (status == ~(u32)0) {
		xhci_hc_died(xhci);
		goto out;
	}

	if (!(status & STS_EINT)) {
		ret = IRQ_NONE;
		goto out;
	}

	if (status & STS_HCE) {
		xhci_warn(xhci, "WARNING: Host Controller Error\n");
		goto out;
	}

	if (status & STS_FATAL) {
		xhci_warn(xhci, "WARNING: Host System Error\n");
		xhci_halt(xhci);
		goto out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/*
	 * Clear the op reg interrupt status first,
	 * so we can receive interrupts from other MSI-X interrupters.
	 * Write 1 to clear the interrupt status.
	 */
	status |= STS_EINT;
<<<<<<< HEAD
	xhci_writel(xhci, status, &xhci->op_regs->status);
	/* FIXME when MSI-X is supported and there are multiple vectors */
	/* Clear the MSI-X event interrupt status */

	if (hcd->irq) {
		u32 irq_pending;
		/* Acknowledge the PCI interrupt */
		irq_pending = xhci_readl(xhci, &xhci->ir_set->irq_pending);
		irq_pending |= IMAN_IP;
		xhci_writel(xhci, irq_pending, &xhci->ir_set->irq_pending);
	}

	if (xhci->xhc_state & XHCI_STATE_DYING) {
		xhci_dbg(xhci, "xHCI dying, ignoring interrupt. "
				"Shouldn't IRQs be disabled?\n");
		/* Clear the event handler busy flag (RW1C);
		 * the event ring should be empty.
		 */
		temp_64 = xhci_read_64(xhci, &xhci->ir_set->erst_dequeue);
		xhci_write_64(xhci, temp_64 | ERST_EHB,
				&xhci->ir_set->erst_dequeue);
		spin_unlock(&xhci->lock);

		return IRQ_HANDLED;
	}

	event_ring_deq = xhci->event_ring->dequeue;
	/* FIXME this should be a delayed service routine
	 * that clears the EHB.
	 */
	while (xhci_handle_event(xhci) > 0) {}

	temp_64 = xhci_read_64(xhci, &xhci->ir_set->erst_dequeue);
	/* If necessary, update the HW's version of the event ring deq ptr. */
	if (event_ring_deq != xhci->event_ring->dequeue) {
		deq = xhci_trb_virt_to_dma(xhci->event_ring->deq_seg,
				xhci->event_ring->dequeue);
		if (deq == 0)
			xhci_warn(xhci, "WARN something wrong with SW event "
					"ring dequeue ptr.\n");
		/* Update HC event ring dequeue pointer */
		temp_64 &= ERST_PTR_MASK;
		temp_64 |= ((u64) deq & (u64) ~ERST_PTR_MASK);
	}

	/* Clear the event handler busy flag (RW1C); event ring is empty. */
	temp_64 |= ERST_EHB;
	xhci_write_64(xhci, temp_64, &xhci->ir_set->erst_dequeue);

	spin_unlock(&xhci->lock);

	return IRQ_HANDLED;
}

irqreturn_t xhci_msi_irq(int irq, struct usb_hcd *hcd)
{
	return xhci_irq(hcd);
}
=======
	writel(status, &xhci->op_regs->status);

	/* This is the handler of the primary interrupter */
	xhci_handle_events(xhci, xhci->interrupters[0]);
out:
	spin_unlock(&xhci->lock);

	return ret;
}

irqreturn_t xhci_msi_irq(int irq, void *hcd)
{
	return xhci_irq(hcd);
}
EXPORT_SYMBOL_GPL(xhci_msi_irq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/****		Endpoint Ring Operations	****/

/*
 * Generic function for queueing a TRB on a ring.
 * The caller must have checked to make sure there's room on the ring.
 *
 * @more_trbs_coming:	Will you enqueue more TRBs before calling
 *			prepare_transfer()?
 */
static void queue_trb(struct xhci_hcd *xhci, struct xhci_ring *ring,
		bool more_trbs_coming,
		u32 field1, u32 field2, u32 field3, u32 field4)
{
	struct xhci_generic_trb *trb;

	trb = &ring->enqueue->generic;
	trb->field[0] = cpu_to_le32(field1);
	trb->field[1] = cpu_to_le32(field2);
	trb->field[2] = cpu_to_le32(field3);
<<<<<<< HEAD
	trb->field[3] = cpu_to_le32(field4);
=======
	/* make sure TRB is fully written before giving it to the controller */
	wmb();
	trb->field[3] = cpu_to_le32(field4);

	trace_xhci_queue_trb(ring, trb);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	inc_enq(xhci, ring, more_trbs_coming);
}

/*
 * Does various checks on the endpoint ring, and makes it ready to queue num_trbs.
<<<<<<< HEAD
 * FIXME allocate segments if the ring is full.
=======
 * expand ring if it start to be full.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
static int prepare_ring(struct xhci_hcd *xhci, struct xhci_ring *ep_ring,
		u32 ep_state, unsigned int num_trbs, gfp_t mem_flags)
{
<<<<<<< HEAD
	unsigned int num_trbs_needed;
=======
	unsigned int link_trb_count = 0;
	unsigned int new_segs = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Make sure the endpoint has been added to xHC schedule */
	switch (ep_state) {
	case EP_STATE_DISABLED:
		/*
		 * USB core changed config/interfaces without notifying us,
		 * or hardware is reporting the wrong state.
		 */
		xhci_warn(xhci, "WARN urb submitted to disabled ep\n");
		return -ENOENT;
	case EP_STATE_ERROR:
		xhci_warn(xhci, "WARN waiting for error on ep to be cleared\n");
		/* FIXME event handling code for error needs to clear it */
		/* XXX not sure if this should be -ENOENT or not */
		return -EINVAL;
	case EP_STATE_HALTED:
		xhci_dbg(xhci, "WARN halted endpoint, queueing URB anyway.\n");
<<<<<<< HEAD
=======
		break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case EP_STATE_STOPPED:
	case EP_STATE_RUNNING:
		break;
	default:
		xhci_err(xhci, "ERROR unknown endpoint state for ep\n");
		/*
		 * FIXME issue Configure Endpoint command to try to get the HC
		 * back into a known state.
		 */
		return -EINVAL;
	}

<<<<<<< HEAD
	while (1) {
		if (room_on_ring(xhci, ep_ring, num_trbs))
			break;

		if (ep_ring == xhci->cmd_ring) {
			xhci_err(xhci, "Do not support expand command ring\n");
			return -ENOMEM;
		}

		xhci_dbg(xhci, "ERROR no room on ep ring, "
					"try ring expansion\n");
		num_trbs_needed = num_trbs - ep_ring->num_trbs_free;
		if (xhci_ring_expansion(xhci, ep_ring, num_trbs_needed,
					mem_flags)) {
			xhci_err(xhci, "Ring expansion failed\n");
			return -ENOMEM;
		}
	};

	if (enqueue_is_link_trb(ep_ring)) {
		struct xhci_ring *ring = ep_ring;
		union xhci_trb *next;

		next = ring->enqueue;

		while (last_trb(xhci, ring, ring->enq_seg, next)) {
			/* If we're not dealing with 0.95 hardware or isoc rings
			 * on AMD 0.96 host, clear the chain bit.
			 */
			if (!xhci_link_trb_quirk(xhci) &&
					!(ring->type == TYPE_ISOC &&
					 (xhci->quirks & XHCI_AMD_0x96_HOST)))
				next->link.control &= cpu_to_le32(~TRB_CHAIN);
			else
				next->link.control |= cpu_to_le32(TRB_CHAIN);

			wmb();
			next->link.control ^= cpu_to_le32(TRB_CYCLE);

			/* Toggle the cycle bit after the last ring segment. */
			if (last_trb_on_last_seg(xhci, ring, ring->enq_seg, next)) {
				ring->cycle_state = (ring->cycle_state ? 0 : 1);
			}
			ring->enq_seg = ring->enq_seg->next;
			ring->enqueue = ring->enq_seg->trbs;
			next = ring->enqueue;
		}
	}

=======
	if (ep_ring != xhci->cmd_ring) {
		new_segs = xhci_ring_expansion_needed(xhci, ep_ring, num_trbs);
	} else if (xhci_num_trbs_free(xhci, ep_ring) <= num_trbs) {
		xhci_err(xhci, "Do not support expand command ring\n");
		return -ENOMEM;
	}

	if (new_segs) {
		xhci_dbg_trace(xhci, trace_xhci_dbg_ring_expansion,
				"ERROR no room on ep ring, try ring expansion");
		if (xhci_ring_expansion(xhci, ep_ring, new_segs, mem_flags)) {
			xhci_err(xhci, "Ring expansion failed\n");
			return -ENOMEM;
		}
	}

	while (trb_is_link(ep_ring->enqueue)) {
		/* If we're not dealing with 0.95 hardware or isoc rings
		 * on AMD 0.96 host, clear the chain bit.
		 */
		if (!xhci_link_trb_quirk(xhci) &&
		    !(ep_ring->type == TYPE_ISOC &&
		      (xhci->quirks & XHCI_AMD_0x96_HOST)))
			ep_ring->enqueue->link.control &=
				cpu_to_le32(~TRB_CHAIN);
		else
			ep_ring->enqueue->link.control |=
				cpu_to_le32(TRB_CHAIN);

		wmb();
		ep_ring->enqueue->link.control ^= cpu_to_le32(TRB_CYCLE);

		/* Toggle the cycle bit after the last ring segment. */
		if (link_trb_toggles_cycle(ep_ring->enqueue))
			ep_ring->cycle_state ^= 1;

		ep_ring->enq_seg = ep_ring->enq_seg->next;
		ep_ring->enqueue = ep_ring->enq_seg->trbs;

		/* prevent infinite loop if all first trbs are link trbs */
		if (link_trb_count++ > ep_ring->num_segs) {
			xhci_warn(xhci, "Ring is an endless link TRB loop\n");
			return -EINVAL;
		}
	}

	if (last_trb_on_seg(ep_ring->enq_seg, ep_ring->enqueue)) {
		xhci_warn(xhci, "Missing link TRB at end of ring segment\n");
		return -EINVAL;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static int prepare_transfer(struct xhci_hcd *xhci,
		struct xhci_virt_device *xdev,
		unsigned int ep_index,
		unsigned int stream_id,
		unsigned int num_trbs,
		struct urb *urb,
		unsigned int td_index,
		gfp_t mem_flags)
{
	int ret;
	struct urb_priv *urb_priv;
	struct xhci_td	*td;
	struct xhci_ring *ep_ring;
	struct xhci_ep_ctx *ep_ctx = xhci_get_ep_ctx(xhci, xdev->out_ctx, ep_index);

<<<<<<< HEAD
	ep_ring = xhci_stream_id_to_ring(xdev, ep_index, stream_id);
=======
	ep_ring = xhci_triad_to_transfer_ring(xhci, xdev->slot_id, ep_index,
					      stream_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!ep_ring) {
		xhci_dbg(xhci, "Can't prepare ring for bad stream ID %u\n",
				stream_id);
		return -EINVAL;
	}

<<<<<<< HEAD
	ret = prepare_ring(xhci, ep_ring,
			   le32_to_cpu(ep_ctx->ep_info) & EP_STATE_MASK,
=======
	ret = prepare_ring(xhci, ep_ring, GET_EP_CTX_STATE(ep_ctx),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			   num_trbs, mem_flags);
	if (ret)
		return ret;

	urb_priv = urb->hcpriv;
<<<<<<< HEAD
	td = urb_priv->td[td_index];
=======
	td = &urb_priv->td[td_index];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	INIT_LIST_HEAD(&td->td_list);
	INIT_LIST_HEAD(&td->cancelled_td_list);

	if (td_index == 0) {
		ret = usb_hcd_link_urb_to_ep(bus_to_hcd(urb->dev->bus), urb);
		if (unlikely(ret))
			return ret;
	}

	td->urb = urb;
	/* Add this TD to the tail of the endpoint ring's TD list */
	list_add_tail(&td->td_list, &ep_ring->td_list);
	td->start_seg = ep_ring->enq_seg;
	td->first_trb = ep_ring->enqueue;

<<<<<<< HEAD
	urb_priv->td[td_index] = td;

	return 0;
}

static unsigned int count_sg_trbs_needed(struct xhci_hcd *xhci, struct urb *urb)
{
	int num_sgs, num_trbs, running_total, temp, i;
	struct scatterlist *sg;

	sg = NULL;
	num_sgs = urb->num_mapped_sgs;
	temp = urb->transfer_buffer_length;

	num_trbs = 0;
	for_each_sg(urb->sg, sg, num_sgs, i) {
		unsigned int len = sg_dma_len(sg);

		/* Scatter gather list entries may cross 64KB boundaries */
		running_total = TRB_MAX_BUFF_SIZE -
			(sg_dma_address(sg) & (TRB_MAX_BUFF_SIZE - 1));
		running_total &= TRB_MAX_BUFF_SIZE - 1;
		if (running_total != 0)
			num_trbs++;

		/* How many more 64KB chunks to transfer, how many more TRBs? */
		while (running_total < sg_dma_len(sg) && running_total < temp) {
			num_trbs++;
			running_total += TRB_MAX_BUFF_SIZE;
		}
		len = min_t(int, len, temp);
		temp -= len;
		if (temp == 0)
			break;
	}
	return num_trbs;
}

static void check_trb_math(struct urb *urb, int num_trbs, int running_total)
{
	if (num_trbs != 0)
		dev_err(&urb->dev->dev, "%s - ep %#x - Miscalculated number of "
				"TRBs, %d left\n", __func__,
				urb->ep->desc.bEndpointAddress, num_trbs);
	if (running_total != urb->transfer_buffer_length)
=======
	return 0;
}

unsigned int count_trbs(u64 addr, u64 len)
{
	unsigned int num_trbs;

	num_trbs = DIV_ROUND_UP(len + (addr & (TRB_MAX_BUFF_SIZE - 1)),
			TRB_MAX_BUFF_SIZE);
	if (num_trbs == 0)
		num_trbs++;

	return num_trbs;
}

static inline unsigned int count_trbs_needed(struct urb *urb)
{
	return count_trbs(urb->transfer_dma, urb->transfer_buffer_length);
}

static unsigned int count_sg_trbs_needed(struct urb *urb)
{
	struct scatterlist *sg;
	unsigned int i, len, full_len, num_trbs = 0;

	full_len = urb->transfer_buffer_length;

	for_each_sg(urb->sg, sg, urb->num_mapped_sgs, i) {
		len = sg_dma_len(sg);
		num_trbs += count_trbs(sg_dma_address(sg), len);
		len = min_t(unsigned int, len, full_len);
		full_len -= len;
		if (full_len == 0)
			break;
	}

	return num_trbs;
}

static unsigned int count_isoc_trbs_needed(struct urb *urb, int i)
{
	u64 addr, len;

	addr = (u64) (urb->transfer_dma + urb->iso_frame_desc[i].offset);
	len = urb->iso_frame_desc[i].length;

	return count_trbs(addr, len);
}

static void check_trb_math(struct urb *urb, int running_total)
{
	if (unlikely(running_total != urb->transfer_buffer_length))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		dev_err(&urb->dev->dev, "%s - ep %#x - Miscalculated tx length, "
				"queued %#x (%d), asked for %#x (%d)\n",
				__func__,
				urb->ep->desc.bEndpointAddress,
				running_total, running_total,
				urb->transfer_buffer_length,
				urb->transfer_buffer_length);
}

static void giveback_first_trb(struct xhci_hcd *xhci, int slot_id,
		unsigned int ep_index, unsigned int stream_id, int start_cycle,
		struct xhci_generic_trb *start_trb)
{
	/*
	 * Pass all the TRBs to the hardware at once and make sure this write
	 * isn't reordered.
	 */
	wmb();
	if (start_cycle)
		start_trb->field[3] |= cpu_to_le32(start_cycle);
	else
		start_trb->field[3] &= cpu_to_le32(~TRB_CYCLE);
	xhci_ring_ep_doorbell(xhci, slot_id, ep_index, stream_id);
}

<<<<<<< HEAD
=======
static void check_interval(struct xhci_hcd *xhci, struct urb *urb,
						struct xhci_ep_ctx *ep_ctx)
{
	int xhci_interval;
	int ep_interval;

	xhci_interval = EP_INTERVAL_TO_UFRAMES(le32_to_cpu(ep_ctx->ep_info));
	ep_interval = urb->interval;

	/* Convert to microframes */
	if (urb->dev->speed == USB_SPEED_LOW ||
			urb->dev->speed == USB_SPEED_FULL)
		ep_interval *= 8;

	/* FIXME change this to a warning and a suggestion to use the new API
	 * to set the polling interval (once the API is added).
	 */
	if (xhci_interval != ep_interval) {
		dev_dbg_ratelimited(&urb->dev->dev,
				"Driver uses different interval (%d microframe%s) than xHCI (%d microframe%s)\n",
				ep_interval, ep_interval == 1 ? "" : "s",
				xhci_interval, xhci_interval == 1 ? "" : "s");
		urb->interval = xhci_interval;
		/* Convert back to frames for LS/FS devices */
		if (urb->dev->speed == USB_SPEED_LOW ||
				urb->dev->speed == USB_SPEED_FULL)
			urb->interval /= 8;
	}
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * xHCI uses normal TRBs for both bulk and interrupt.  When the interrupt
 * endpoint is to be serviced, the xHC will consume (at most) one TD.  A TD
 * (comprised of sg list entries) can take several service intervals to
 * transmit.
 */
int xhci_queue_intr_tx(struct xhci_hcd *xhci, gfp_t mem_flags,
		struct urb *urb, int slot_id, unsigned int ep_index)
{
<<<<<<< HEAD
	struct xhci_ep_ctx *ep_ctx = xhci_get_ep_ctx(xhci,
			xhci->devs[slot_id]->out_ctx, ep_index);
	int xhci_interval;
	int ep_interval;

	xhci_interval = EP_INTERVAL_TO_UFRAMES(le32_to_cpu(ep_ctx->ep_info));
	ep_interval = urb->interval;
	/* Convert to microframes */
	if (urb->dev->speed == USB_SPEED_LOW ||
			urb->dev->speed == USB_SPEED_FULL)
		ep_interval *= 8;
	/* FIXME change this to a warning and a suggestion to use the new API
	 * to set the polling interval (once the API is added).
	 */
	if (xhci_interval != ep_interval) {
		if (printk_ratelimit())
			dev_dbg(&urb->dev->dev, "Driver uses different interval"
					" (%d microframe%s) than xHCI "
					"(%d microframe%s)\n",
					ep_interval,
					ep_interval == 1 ? "" : "s",
					xhci_interval,
					xhci_interval == 1 ? "" : "s");
		urb->interval = xhci_interval;
		/* Convert back to frames for LS/FS devices */
		if (urb->dev->speed == USB_SPEED_LOW ||
				urb->dev->speed == USB_SPEED_FULL)
			urb->interval /= 8;
	}
=======
	struct xhci_ep_ctx *ep_ctx;

	ep_ctx = xhci_get_ep_ctx(xhci, xhci->devs[slot_id]->out_ctx, ep_index);
	check_interval(xhci, urb, ep_ctx);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return xhci_queue_bulk_tx(xhci, mem_flags, urb, slot_id, ep_index);
}

/*
<<<<<<< HEAD
 * The TD size is the number of bytes remaining in the TD (including this TRB),
 * right shifted by 10.
 * It must fit in bits 21:17, so it can't be bigger than 31.
 */
static u32 xhci_td_remainder(unsigned int remainder)
{
	u32 max = (1 << (21 - 17 + 1)) - 1;

	if ((remainder >> 10) >= max)
		return max << 17;
	else
		return (remainder >> 10) << 17;
}

/*
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * For xHCI 1.0 host controllers, TD size is the number of max packet sized
 * packets remaining in the TD (*not* including this TRB).
 *
 * Total TD packet count = total_packet_count =
 *     DIV_ROUND_UP(TD size in bytes / wMaxPacketSize)
 *
 * Packets transferred up to and including this TRB = packets_transferred =
 *     rounddown(total bytes transferred including this TRB / wMaxPacketSize)
 *
 * TD size = total_packet_count - packets_transferred
 *
<<<<<<< HEAD
 * It must fit in bits 21:17, so it can't be bigger than 31.
 * The last TRB in a TD must have the TD size set to zero.
 */
static u32 xhci_v1_0_td_remainder(int running_total, int trb_buff_len,
		unsigned int total_packet_count, struct urb *urb,
		unsigned int num_trbs_left)
{
	int packets_transferred;

	/* One TRB with a zero-length data packet. */
	if (num_trbs_left == 0 || (running_total == 0 && trb_buff_len == 0))
		return 0;

	/* All the TRB queueing functions don't count the current TRB in
	 * running_total.
	 */
	packets_transferred = (running_total + trb_buff_len) /
		GET_MAX_PACKET(usb_endpoint_maxp(&urb->ep->desc));

	if ((total_packet_count - packets_transferred) > 31)
		return 31 << 17;
	return (total_packet_count - packets_transferred) << 17;
}

static int queue_bulk_sg_tx(struct xhci_hcd *xhci, gfp_t mem_flags,
		struct urb *urb, int slot_id, unsigned int ep_index)
{
	struct xhci_ring *ep_ring;
	unsigned int num_trbs;
	struct urb_priv *urb_priv;
	struct xhci_td *td;
	struct scatterlist *sg;
	int num_sgs;
	int trb_buff_len, this_sg_len, running_total;
	unsigned int total_packet_count;
	bool first_trb;
	u64 addr;
	bool more_trbs_coming;

	struct xhci_generic_trb *start_trb;
	int start_cycle;

	ep_ring = xhci_urb_to_transfer_ring(xhci, urb);
	if (!ep_ring)
		return -EINVAL;

	num_trbs = count_sg_trbs_needed(xhci, urb);
	num_sgs = urb->num_mapped_sgs;
	total_packet_count = DIV_ROUND_UP(urb->transfer_buffer_length,
			usb_endpoint_maxp(&urb->ep->desc));

	trb_buff_len = prepare_transfer(xhci, xhci->devs[slot_id],
			ep_index, urb->stream_id,
			num_trbs, urb, 0, mem_flags);
	if (trb_buff_len < 0)
		return trb_buff_len;

	urb_priv = urb->hcpriv;
	td = urb_priv->td[0];

	/*
	 * Don't give the first TRB to the hardware (by toggling the cycle bit)
	 * until we've finished creating all the other TRBs.  The ring's cycle
	 * state may change as we enqueue the other TRBs, so save it too.
	 */
	start_trb = &ep_ring->enqueue->generic;
	start_cycle = ep_ring->cycle_state;

	running_total = 0;
	/*
	 * How much data is in the first TRB?
	 *
	 * There are three forces at work for TRB buffer pointers and lengths:
	 * 1. We don't want to walk off the end of this sg-list entry buffer.
	 * 2. The transfer length that the driver requested may be smaller than
	 *    the amount of memory allocated for this scatter-gather list.
	 * 3. TRBs buffers can't cross 64KB boundaries.
	 */
	sg = urb->sg;
	addr = (u64) sg_dma_address(sg);
	this_sg_len = sg_dma_len(sg);
	trb_buff_len = TRB_MAX_BUFF_SIZE - (addr & (TRB_MAX_BUFF_SIZE - 1));
	trb_buff_len = min_t(int, trb_buff_len, this_sg_len);
	if (trb_buff_len > urb->transfer_buffer_length)
		trb_buff_len = urb->transfer_buffer_length;

	first_trb = true;
	/* Queue the first TRB, even if it's zero-length */
	do {
		u32 field = 0;
		u32 length_field = 0;
		u32 remainder = 0;

		/* Don't change the cycle bit of the first TRB until later */
		if (first_trb) {
			first_trb = false;
			if (start_cycle == 0)
				field |= 0x1;
		} else
			field |= ep_ring->cycle_state;

		/* Chain all the TRBs together; clear the chain bit in the last
		 * TRB to indicate it's the last TRB in the chain.
		 */
		if (num_trbs > 1) {
			field |= TRB_CHAIN;
		} else {
			/* FIXME - add check for ZERO_PACKET flag before this */
			td->last_trb = ep_ring->enqueue;
			field |= TRB_IOC;
		}

		/* Only set interrupt on short packet for IN endpoints */
		if (usb_urb_dir_in(urb))
			field |= TRB_ISP;

		if (TRB_MAX_BUFF_SIZE -
				(addr & (TRB_MAX_BUFF_SIZE - 1)) < trb_buff_len) {
			xhci_warn(xhci, "WARN: sg dma xfer crosses 64KB boundaries!\n");
			xhci_dbg(xhci, "Next boundary at %#x, end dma = %#x\n",
					(unsigned int) (addr + TRB_MAX_BUFF_SIZE) & ~(TRB_MAX_BUFF_SIZE - 1),
					(unsigned int) addr + trb_buff_len);
		}

		/* Set the TRB length, TD size, and interrupter fields. */
		if (xhci->hci_version < 0x100) {
			remainder = xhci_td_remainder(
					urb->transfer_buffer_length -
					running_total);
		} else {
			remainder = xhci_v1_0_td_remainder(running_total,
					trb_buff_len, total_packet_count, urb,
					num_trbs - 1);
		}
		length_field = TRB_LEN(trb_buff_len) |
			remainder |
			TRB_INTR_TARGET(0);

		if (num_trbs > 1)
			more_trbs_coming = true;
		else
			more_trbs_coming = false;
		queue_trb(xhci, ep_ring, more_trbs_coming,
				lower_32_bits(addr),
				upper_32_bits(addr),
				length_field,
				field | TRB_TYPE(TRB_NORMAL));
		--num_trbs;
		running_total += trb_buff_len;

		/* Calculate length for next transfer --
		 * Are we done queueing all the TRBs for this sg entry?
		 */
		this_sg_len -= trb_buff_len;
		if (this_sg_len == 0) {
			--num_sgs;
			if (num_sgs == 0)
				break;
			sg = sg_next(sg);
			addr = (u64) sg_dma_address(sg);
			this_sg_len = sg_dma_len(sg);
		} else {
			addr += trb_buff_len;
		}

		trb_buff_len = TRB_MAX_BUFF_SIZE -
			(addr & (TRB_MAX_BUFF_SIZE - 1));
		trb_buff_len = min_t(int, trb_buff_len, this_sg_len);
		if (running_total + trb_buff_len > urb->transfer_buffer_length)
			trb_buff_len =
				urb->transfer_buffer_length - running_total;
	} while (running_total < urb->transfer_buffer_length);

	check_trb_math(urb, num_trbs, running_total);
	giveback_first_trb(xhci, slot_id, ep_index, urb->stream_id,
			start_cycle, start_trb);
	return 0;
=======
 * For xHCI 0.96 and older, TD size field should be the remaining bytes
 * including this TRB, right shifted by 10
 *
 * For all hosts it must fit in bits 21:17, so it can't be bigger than 31.
 * This is taken care of in the TRB_TD_SIZE() macro
 *
 * The last TRB in a TD must have the TD size set to zero.
 */
static u32 xhci_td_remainder(struct xhci_hcd *xhci, int transferred,
			      int trb_buff_len, unsigned int td_total_len,
			      struct urb *urb, bool more_trbs_coming)
{
	u32 maxp, total_packet_count;

	/* MTK xHCI 0.96 contains some features from 1.0 */
	if (xhci->hci_version < 0x100 && !(xhci->quirks & XHCI_MTK_HOST))
		return ((td_total_len - transferred) >> 10);

	/* One TRB with a zero-length data packet. */
	if (!more_trbs_coming || (transferred == 0 && trb_buff_len == 0) ||
	    trb_buff_len == td_total_len)
		return 0;

	/* for MTK xHCI 0.96, TD size include this TRB, but not in 1.x */
	if ((xhci->quirks & XHCI_MTK_HOST) && (xhci->hci_version < 0x100))
		trb_buff_len = 0;

	maxp = usb_endpoint_maxp(&urb->ep->desc);
	total_packet_count = DIV_ROUND_UP(td_total_len, maxp);

	/* Queueing functions don't count the current TRB into transferred */
	return (total_packet_count - ((transferred + trb_buff_len) / maxp));
}


static int xhci_align_td(struct xhci_hcd *xhci, struct urb *urb, u32 enqd_len,
			 u32 *trb_buff_len, struct xhci_segment *seg)
{
	struct device *dev = xhci_to_hcd(xhci)->self.sysdev;
	unsigned int unalign;
	unsigned int max_pkt;
	u32 new_buff_len;
	size_t len;

	max_pkt = usb_endpoint_maxp(&urb->ep->desc);
	unalign = (enqd_len + *trb_buff_len) % max_pkt;

	/* we got lucky, last normal TRB data on segment is packet aligned */
	if (unalign == 0)
		return 0;

	xhci_dbg(xhci, "Unaligned %d bytes, buff len %d\n",
		 unalign, *trb_buff_len);

	/* is the last nornal TRB alignable by splitting it */
	if (*trb_buff_len > unalign) {
		*trb_buff_len -= unalign;
		xhci_dbg(xhci, "split align, new buff len %d\n", *trb_buff_len);
		return 0;
	}

	/*
	 * We want enqd_len + trb_buff_len to sum up to a number aligned to
	 * number which is divisible by the endpoint's wMaxPacketSize. IOW:
	 * (size of currently enqueued TRBs + remainder) % wMaxPacketSize == 0.
	 */
	new_buff_len = max_pkt - (enqd_len % max_pkt);

	if (new_buff_len > (urb->transfer_buffer_length - enqd_len))
		new_buff_len = (urb->transfer_buffer_length - enqd_len);

	/* create a max max_pkt sized bounce buffer pointed to by last trb */
	if (usb_urb_dir_out(urb)) {
		if (urb->num_sgs) {
			len = sg_pcopy_to_buffer(urb->sg, urb->num_sgs,
						 seg->bounce_buf, new_buff_len, enqd_len);
			if (len != new_buff_len)
				xhci_warn(xhci, "WARN Wrong bounce buffer write length: %zu != %d\n",
					  len, new_buff_len);
		} else {
			memcpy(seg->bounce_buf, urb->transfer_buffer + enqd_len, new_buff_len);
		}

		seg->bounce_dma = dma_map_single(dev, seg->bounce_buf,
						 max_pkt, DMA_TO_DEVICE);
	} else {
		seg->bounce_dma = dma_map_single(dev, seg->bounce_buf,
						 max_pkt, DMA_FROM_DEVICE);
	}

	if (dma_mapping_error(dev, seg->bounce_dma)) {
		/* try without aligning. Some host controllers survive */
		xhci_warn(xhci, "Failed mapping bounce buffer, not aligning\n");
		return 0;
	}
	*trb_buff_len = new_buff_len;
	seg->bounce_len = new_buff_len;
	seg->bounce_offs = enqd_len;

	xhci_dbg(xhci, "Bounce align, new buff len %d\n", *trb_buff_len);

	return 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* This is very similar to what ehci-q.c qtd_fill() does */
int xhci_queue_bulk_tx(struct xhci_hcd *xhci, gfp_t mem_flags,
		struct urb *urb, int slot_id, unsigned int ep_index)
{
<<<<<<< HEAD
	struct xhci_ring *ep_ring;
	struct urb_priv *urb_priv;
	struct xhci_td *td;
	int num_trbs;
	struct xhci_generic_trb *start_trb;
	bool first_trb;
	bool more_trbs_coming;
	int start_cycle;
	u32 field, length_field;

	int running_total, trb_buff_len, ret;
	unsigned int total_packet_count;
	u64 addr;

	if (urb->num_sgs)
		return queue_bulk_sg_tx(xhci, mem_flags, urb, slot_id, ep_index);

	ep_ring = xhci_urb_to_transfer_ring(xhci, urb);
	if (!ep_ring)
		return -EINVAL;

	num_trbs = 0;
	/* How much data is (potentially) left before the 64KB boundary? */
	running_total = TRB_MAX_BUFF_SIZE -
		(urb->transfer_dma & (TRB_MAX_BUFF_SIZE - 1));
	running_total &= TRB_MAX_BUFF_SIZE - 1;

	/* If there's some data on this 64KB chunk, or we have to send a
	 * zero-length transfer, we need at least one TRB
	 */
	if (running_total != 0 || urb->transfer_buffer_length == 0)
		num_trbs++;
	/* How many more 64KB chunks to transfer, how many more TRBs? */
	while (running_total < urb->transfer_buffer_length) {
		num_trbs++;
		running_total += TRB_MAX_BUFF_SIZE;
	}
	/* FIXME: this doesn't deal with URB_ZERO_PACKET - need one more */

	ret = prepare_transfer(xhci, xhci->devs[slot_id],
			ep_index, urb->stream_id,
			num_trbs, urb, 0, mem_flags);
	if (ret < 0)
		return ret;

	urb_priv = urb->hcpriv;
	td = urb_priv->td[0];
=======
	struct xhci_ring *ring;
	struct urb_priv *urb_priv;
	struct xhci_td *td;
	struct xhci_generic_trb *start_trb;
	struct scatterlist *sg = NULL;
	bool more_trbs_coming = true;
	bool need_zero_pkt = false;
	bool first_trb = true;
	unsigned int num_trbs;
	unsigned int start_cycle, num_sgs = 0;
	unsigned int enqd_len, block_len, trb_buff_len, full_len;
	int sent_len, ret;
	u32 field, length_field, remainder;
	u64 addr, send_addr;

	ring = xhci_urb_to_transfer_ring(xhci, urb);
	if (!ring)
		return -EINVAL;

	full_len = urb->transfer_buffer_length;
	/* If we have scatter/gather list, we use it. */
	if (urb->num_sgs && !(urb->transfer_flags & URB_DMA_MAP_SINGLE)) {
		num_sgs = urb->num_mapped_sgs;
		sg = urb->sg;
		addr = (u64) sg_dma_address(sg);
		block_len = sg_dma_len(sg);
		num_trbs = count_sg_trbs_needed(urb);
	} else {
		num_trbs = count_trbs_needed(urb);
		addr = (u64) urb->transfer_dma;
		block_len = full_len;
	}
	ret = prepare_transfer(xhci, xhci->devs[slot_id],
			ep_index, urb->stream_id,
			num_trbs, urb, 0, mem_flags);
	if (unlikely(ret < 0))
		return ret;

	urb_priv = urb->hcpriv;

	/* Deal with URB_ZERO_PACKET - need one more td/trb */
	if (urb->transfer_flags & URB_ZERO_PACKET && urb_priv->num_tds > 1)
		need_zero_pkt = true;

	td = &urb_priv->td[0];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Don't give the first TRB to the hardware (by toggling the cycle bit)
	 * until we've finished creating all the other TRBs.  The ring's cycle
	 * state may change as we enqueue the other TRBs, so save it too.
	 */
<<<<<<< HEAD
	start_trb = &ep_ring->enqueue->generic;
	start_cycle = ep_ring->cycle_state;

	running_total = 0;
	total_packet_count = DIV_ROUND_UP(urb->transfer_buffer_length,
			usb_endpoint_maxp(&urb->ep->desc));
	/* How much data is in the first TRB? */
	addr = (u64) urb->transfer_dma;
	trb_buff_len = TRB_MAX_BUFF_SIZE -
		(urb->transfer_dma & (TRB_MAX_BUFF_SIZE - 1));
	if (trb_buff_len > urb->transfer_buffer_length)
		trb_buff_len = urb->transfer_buffer_length;

	first_trb = true;

	/* Queue the first TRB, even if it's zero-length */
	do {
		u32 remainder = 0;
		field = 0;
=======
	start_trb = &ring->enqueue->generic;
	start_cycle = ring->cycle_state;
	send_addr = addr;

	/* Queue the TRBs, even if they are zero-length */
	for (enqd_len = 0; first_trb || enqd_len < full_len;
			enqd_len += trb_buff_len) {
		field = TRB_TYPE(TRB_NORMAL);

		/* TRB buffer should not cross 64KB boundaries */
		trb_buff_len = TRB_BUFF_LEN_UP_TO_BOUNDARY(addr);
		trb_buff_len = min_t(unsigned int, trb_buff_len, block_len);

		if (enqd_len + trb_buff_len > full_len)
			trb_buff_len = full_len - enqd_len;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/* Don't change the cycle bit of the first TRB until later */
		if (first_trb) {
			first_trb = false;
			if (start_cycle == 0)
<<<<<<< HEAD
				field |= 0x1;
		} else
			field |= ep_ring->cycle_state;
=======
				field |= TRB_CYCLE;
		} else
			field |= ring->cycle_state;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/* Chain all the TRBs together; clear the chain bit in the last
		 * TRB to indicate it's the last TRB in the chain.
		 */
<<<<<<< HEAD
		if (num_trbs > 1) {
			field |= TRB_CHAIN;
		} else {
			/* FIXME - add check for ZERO_PACKET flag before this */
			td->last_trb = ep_ring->enqueue;
			field |= TRB_IOC;
=======
		if (enqd_len + trb_buff_len < full_len) {
			field |= TRB_CHAIN;
			if (trb_is_link(ring->enqueue + 1)) {
				if (xhci_align_td(xhci, urb, enqd_len,
						  &trb_buff_len,
						  ring->enq_seg)) {
					send_addr = ring->enq_seg->bounce_dma;
					/* assuming TD won't span 2 segs */
					td->bounce_seg = ring->enq_seg;
				}
			}
		}
		if (enqd_len + trb_buff_len >= full_len) {
			field &= ~TRB_CHAIN;
			field |= TRB_IOC;
			more_trbs_coming = false;
			td->last_trb = ring->enqueue;
			td->last_trb_seg = ring->enq_seg;
			if (xhci_urb_suitable_for_idt(urb)) {
				memcpy(&send_addr, urb->transfer_buffer,
				       trb_buff_len);
				le64_to_cpus(&send_addr);
				field |= TRB_IDT;
			}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}

		/* Only set interrupt on short packet for IN endpoints */
		if (usb_urb_dir_in(urb))
			field |= TRB_ISP;

		/* Set the TRB length, TD size, and interrupter fields. */
<<<<<<< HEAD
		if (xhci->hci_version < 0x100) {
			remainder = xhci_td_remainder(
					urb->transfer_buffer_length -
					running_total);
		} else {
			remainder = xhci_v1_0_td_remainder(running_total,
					trb_buff_len, total_packet_count, urb,
					num_trbs - 1);
		}
		length_field = TRB_LEN(trb_buff_len) |
			remainder |
			TRB_INTR_TARGET(0);

		if (num_trbs > 1)
			more_trbs_coming = true;
		else
			more_trbs_coming = false;
		queue_trb(xhci, ep_ring, more_trbs_coming,
				lower_32_bits(addr),
				upper_32_bits(addr),
				length_field,
				field | TRB_TYPE(TRB_NORMAL));
		--num_trbs;
		running_total += trb_buff_len;

		/* Calculate length for next transfer */
		addr += trb_buff_len;
		trb_buff_len = urb->transfer_buffer_length - running_total;
		if (trb_buff_len > TRB_MAX_BUFF_SIZE)
			trb_buff_len = TRB_MAX_BUFF_SIZE;
	} while (running_total < urb->transfer_buffer_length);

	check_trb_math(urb, num_trbs, running_total);
=======
		remainder = xhci_td_remainder(xhci, enqd_len, trb_buff_len,
					      full_len, urb, more_trbs_coming);

		length_field = TRB_LEN(trb_buff_len) |
			TRB_TD_SIZE(remainder) |
			TRB_INTR_TARGET(0);

		queue_trb(xhci, ring, more_trbs_coming | need_zero_pkt,
				lower_32_bits(send_addr),
				upper_32_bits(send_addr),
				length_field,
				field);
		td->num_trbs++;
		addr += trb_buff_len;
		sent_len = trb_buff_len;

		while (sg && sent_len >= block_len) {
			/* New sg entry */
			--num_sgs;
			sent_len -= block_len;
			sg = sg_next(sg);
			if (num_sgs != 0 && sg) {
				block_len = sg_dma_len(sg);
				addr = (u64) sg_dma_address(sg);
				addr += sent_len;
			}
		}
		block_len -= sent_len;
		send_addr = addr;
	}

	if (need_zero_pkt) {
		ret = prepare_transfer(xhci, xhci->devs[slot_id],
				       ep_index, urb->stream_id,
				       1, urb, 1, mem_flags);
		urb_priv->td[1].last_trb = ring->enqueue;
		urb_priv->td[1].last_trb_seg = ring->enq_seg;
		field = TRB_TYPE(TRB_NORMAL) | ring->cycle_state | TRB_IOC;
		queue_trb(xhci, ring, 0, 0, 0, TRB_INTR_TARGET(0), field);
		urb_priv->td[1].num_trbs++;
	}

	check_trb_math(urb, enqd_len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	giveback_first_trb(xhci, slot_id, ep_index, urb->stream_id,
			start_cycle, start_trb);
	return 0;
}

/* Caller must have locked xhci->lock */
int xhci_queue_ctrl_tx(struct xhci_hcd *xhci, gfp_t mem_flags,
		struct urb *urb, int slot_id, unsigned int ep_index)
{
	struct xhci_ring *ep_ring;
	int num_trbs;
	int ret;
	struct usb_ctrlrequest *setup;
	struct xhci_generic_trb *start_trb;
	int start_cycle;
<<<<<<< HEAD
	u32 field, length_field;
=======
	u32 field;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct urb_priv *urb_priv;
	struct xhci_td *td;

	ep_ring = xhci_urb_to_transfer_ring(xhci, urb);
	if (!ep_ring)
		return -EINVAL;

	/*
	 * Need to copy setup packet into setup TRB, so we can't use the setup
	 * DMA address.
	 */
	if (!urb->setup_packet)
		return -EINVAL;

	/* 1 TRB for setup, 1 for status */
	num_trbs = 2;
	/*
	 * Don't need to check if we need additional event data and normal TRBs,
	 * since data in control transfers will never get bigger than 16MB
	 * XXX: can we get a buffer that crosses 64KB boundaries?
	 */
	if (urb->transfer_buffer_length > 0)
		num_trbs++;
	ret = prepare_transfer(xhci, xhci->devs[slot_id],
			ep_index, urb->stream_id,
			num_trbs, urb, 0, mem_flags);
	if (ret < 0)
		return ret;

	urb_priv = urb->hcpriv;
<<<<<<< HEAD
	td = urb_priv->td[0];
=======
	td = &urb_priv->td[0];
	td->num_trbs = num_trbs;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Don't give the first TRB to the hardware (by toggling the cycle bit)
	 * until we've finished creating all the other TRBs.  The ring's cycle
	 * state may change as we enqueue the other TRBs, so save it too.
	 */
	start_trb = &ep_ring->enqueue->generic;
	start_cycle = ep_ring->cycle_state;

	/* Queue setup TRB - see section 6.4.1.2.1 */
	/* FIXME better way to translate setup_packet into two u32 fields? */
	setup = (struct usb_ctrlrequest *) urb->setup_packet;
	field = 0;
	field |= TRB_IDT | TRB_TYPE(TRB_SETUP);
	if (start_cycle == 0)
		field |= 0x1;

	/* xHCI 1.0/1.1 6.4.1.2.1: Transfer Type field */
<<<<<<< HEAD
	if (xhci->hci_version >= 0x100) {
=======
	if ((xhci->hci_version >= 0x100) || (xhci->quirks & XHCI_MTK_HOST)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (urb->transfer_buffer_length > 0) {
			if (setup->bRequestType & USB_DIR_IN)
				field |= TRB_TX_TYPE(TRB_DATA_IN);
			else
				field |= TRB_TX_TYPE(TRB_DATA_OUT);
		}
	}

	queue_trb(xhci, ep_ring, true,
		  setup->bRequestType | setup->bRequest << 8 | le16_to_cpu(setup->wValue) << 16,
		  le16_to_cpu(setup->wIndex) | le16_to_cpu(setup->wLength) << 16,
		  TRB_LEN(8) | TRB_INTR_TARGET(0),
		  /* Immediate data in pointer */
		  field);

	/* If there's data, queue data TRBs */
	/* Only set interrupt on short packet for IN endpoints */
	if (usb_urb_dir_in(urb))
		field = TRB_ISP | TRB_TYPE(TRB_DATA);
	else
		field = TRB_TYPE(TRB_DATA);

<<<<<<< HEAD
	length_field = TRB_LEN(urb->transfer_buffer_length) |
		xhci_td_remainder(urb->transfer_buffer_length) |
		TRB_INTR_TARGET(0);
	if (urb->transfer_buffer_length > 0) {
		if (setup->bRequestType & USB_DIR_IN)
			field |= TRB_DIR_IN;
		queue_trb(xhci, ep_ring, true,
				lower_32_bits(urb->transfer_dma),
				upper_32_bits(urb->transfer_dma),
=======
	if (urb->transfer_buffer_length > 0) {
		u32 length_field, remainder;
		u64 addr;

		if (xhci_urb_suitable_for_idt(urb)) {
			memcpy(&addr, urb->transfer_buffer,
			       urb->transfer_buffer_length);
			le64_to_cpus(&addr);
			field |= TRB_IDT;
		} else {
			addr = (u64) urb->transfer_dma;
		}

		remainder = xhci_td_remainder(xhci, 0,
				urb->transfer_buffer_length,
				urb->transfer_buffer_length,
				urb, 1);
		length_field = TRB_LEN(urb->transfer_buffer_length) |
				TRB_TD_SIZE(remainder) |
				TRB_INTR_TARGET(0);
		if (setup->bRequestType & USB_DIR_IN)
			field |= TRB_DIR_IN;
		queue_trb(xhci, ep_ring, true,
				lower_32_bits(addr),
				upper_32_bits(addr),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				length_field,
				field | ep_ring->cycle_state);
	}

	/* Save the DMA address of the last TRB in the TD */
	td->last_trb = ep_ring->enqueue;
<<<<<<< HEAD
=======
	td->last_trb_seg = ep_ring->enq_seg;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Queue status TRB - see Table 7 and sections 4.11.2.2 and 6.4.1.2.3 */
	/* If the device sent data, the status stage is an OUT transfer */
	if (urb->transfer_buffer_length > 0 && setup->bRequestType & USB_DIR_IN)
		field = 0;
	else
		field = TRB_DIR_IN;
	queue_trb(xhci, ep_ring, false,
			0,
			0,
			TRB_INTR_TARGET(0),
			/* Event on completion */
			field | TRB_IOC | TRB_TYPE(TRB_STATUS) | ep_ring->cycle_state);

	giveback_first_trb(xhci, slot_id, ep_index, 0,
			start_cycle, start_trb);
	return 0;
}

/*
<<<<<<< HEAD
 * Variant of xhci_queue_ctrl_tx() used to implement EHSET
 * SINGLE_STEP_SET_FEATURE test mode. It differs in that the control
 * transfer is broken up so that the SETUP stage can happen and call
 * the URB's completion handler before the DATA/STATUS stages are
 * executed by the xHC hardware. This assumes the control transfer is a
 * GetDescriptor, with a DATA stage in the IN direction, and an OUT
 * STATUS stage.
 *
 * This function is called twice, usually with a 15-second delay in between.
 * - with is_setup==true, the SETUP stage for the control request
 *   (GetDescriptor) is queued in the TRB ring and sent to HW immediately
 * - with is_setup==false, the DATA and STATUS TRBs are queued and exceuted
 *
 * Caller must have locked xhci->lock
 */
int xhci_submit_single_step_set_feature(struct usb_hcd *hcd, struct urb *urb,
					int is_setup)
{
	struct xhci_hcd *xhci = hcd_to_xhci(hcd);
	struct xhci_ring *ep_ring;
	int num_trbs;
	int ret;
	unsigned int slot_id, ep_index;
	struct usb_ctrlrequest *setup;
	struct xhci_generic_trb *start_trb;
	int start_cycle;
	u32 field, length_field;
	struct urb_priv *urb_priv;
	struct xhci_td *td;

	ep_ring = xhci_urb_to_transfer_ring(xhci, urb);
	if (!ep_ring)
		return -EINVAL;

	/* Need buffer for data stage */
	if (urb->transfer_buffer_length <= 0)
		return -EINVAL;

	/*
	 * Need to copy setup packet into setup TRB, so we can't use the setup
	 * DMA address.
	 */
	if (!urb->setup_packet)
		return -EINVAL;
	setup = (struct usb_ctrlrequest *) urb->setup_packet;

	slot_id = urb->dev->slot_id;
	ep_index = xhci_get_endpoint_index(&urb->ep->desc);

	urb_priv = kzalloc(sizeof(struct urb_priv) +
				  sizeof(struct xhci_td *), GFP_ATOMIC);
	if (!urb_priv)
		return -ENOMEM;

	td = urb_priv->td[0] = kzalloc(sizeof(struct xhci_td), GFP_ATOMIC);
	if (!td) {
		kfree(urb_priv);
		return -ENOMEM;
	}

	urb_priv->length = 1;
	urb_priv->td_cnt = 0;
	urb->hcpriv = urb_priv;

	num_trbs = is_setup ? 1 : 2;

	ret = prepare_transfer(xhci, xhci->devs[slot_id],
			ep_index, urb->stream_id,
			num_trbs, urb, 0, GFP_ATOMIC);
	if (ret < 0) {
		kfree(td);
		kfree(urb_priv);
		return ret;
	}

	/*
	 * Don't give the first TRB to the hardware (by toggling the cycle bit)
	 * until we've finished creating all the other TRBs.  The ring's cycle
	 * state may change as we enqueue the other TRBs, so save it too.
	 */
	start_trb = &ep_ring->enqueue->generic;
	start_cycle = ep_ring->cycle_state;

	if (is_setup) {
		/* Queue only the setup TRB */
		field = TRB_IDT | TRB_IOC | TRB_TYPE(TRB_SETUP);
		if (start_cycle == 0)
			field |= 0x1;

		/* xHCI 1.0 6.4.1.2.1: Transfer Type field */
		if (xhci->hci_version == 0x100) {
			if (setup->bRequestType & USB_DIR_IN)
				field |= TRB_TX_TYPE(TRB_DATA_IN);
			else
				field |= TRB_TX_TYPE(TRB_DATA_OUT);
		}

		/* Save the DMA address of the last TRB in the TD */
		td->last_trb = ep_ring->enqueue;

		queue_trb(xhci, ep_ring, false,
			  setup->bRequestType | setup->bRequest << 8 |
				le16_to_cpu(setup->wValue) << 16,
			  le16_to_cpu(setup->wIndex) |
				le16_to_cpu(setup->wLength) << 16,
			  TRB_LEN(8) | TRB_INTR_TARGET(0),
			  field);
	} else {
		/* Queue data TRB */
		field = TRB_ISP | TRB_TYPE(TRB_DATA);
		if (start_cycle == 0)
			field |= 0x1;
		if (setup->bRequestType & USB_DIR_IN)
			field |= TRB_DIR_IN;

		length_field = TRB_LEN(urb->transfer_buffer_length) |
			xhci_td_remainder(urb->transfer_buffer_length) |
			TRB_INTR_TARGET(0);
		queue_trb(xhci, ep_ring, true,
			  lower_32_bits(urb->transfer_dma),
			  upper_32_bits(urb->transfer_dma),
			  length_field,
			  field);

		/* Save the DMA address of the last TRB in the TD */
		td->last_trb = ep_ring->enqueue;

		/* Queue status TRB */
		field = TRB_IOC | TRB_TYPE(TRB_STATUS);
		if (!(setup->bRequestType & USB_DIR_IN))
			field |= TRB_DIR_IN;

		queue_trb(xhci, ep_ring, false,
			  0,
			  0,
			  TRB_INTR_TARGET(0),
			  field | ep_ring->cycle_state);
	}

	giveback_first_trb(xhci, slot_id, ep_index, 0, start_cycle, start_trb);
	return 0;
}

static int count_isoc_trbs_needed(struct xhci_hcd *xhci,
		struct urb *urb, int i)
{
	int num_trbs = 0;
	u64 addr, td_len;

	addr = (u64) (urb->transfer_dma + urb->iso_frame_desc[i].offset);
	td_len = urb->iso_frame_desc[i].length;

	num_trbs = DIV_ROUND_UP(td_len + (addr & (TRB_MAX_BUFF_SIZE - 1)),
			TRB_MAX_BUFF_SIZE);
	if (num_trbs == 0)
		num_trbs++;

	return num_trbs;
}

/*
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * The transfer burst count field of the isochronous TRB defines the number of
 * bursts that are required to move all packets in this TD.  Only SuperSpeed
 * devices can burst up to bMaxBurst number of packets per service interval.
 * This field is zero based, meaning a value of zero in the field means one
 * burst.  Basically, for everything but SuperSpeed devices, this field will be
 * zero.  Only xHCI 1.0 host controllers support this field.
 */
static unsigned int xhci_get_burst_count(struct xhci_hcd *xhci,
<<<<<<< HEAD
		struct usb_device *udev,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		struct urb *urb, unsigned int total_packet_count)
{
	unsigned int max_burst;

<<<<<<< HEAD
	if (xhci->hci_version < 0x100 || udev->speed != USB_SPEED_SUPER)
=======
	if (xhci->hci_version < 0x100 || urb->dev->speed < USB_SPEED_SUPER)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;

	max_burst = urb->ep->ss_ep_comp.bMaxBurst;
	return DIV_ROUND_UP(total_packet_count, max_burst + 1) - 1;
}

/*
 * Returns the number of packets in the last "burst" of packets.  This field is
 * valid for all speeds of devices.  USB 2.0 devices can only do one "burst", so
 * the last burst packet count is equal to the total number of packets in the
 * TD.  SuperSpeed endpoints can have up to 3 bursts.  All but the last burst
 * must contain (bMaxBurst + 1) number of packets, but the last burst can
 * contain 1 to (bMaxBurst + 1) packets.
 */
static unsigned int xhci_get_last_burst_packet_count(struct xhci_hcd *xhci,
<<<<<<< HEAD
		struct usb_device *udev,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		struct urb *urb, unsigned int total_packet_count)
{
	unsigned int max_burst;
	unsigned int residue;

	if (xhci->hci_version < 0x100)
		return 0;

<<<<<<< HEAD
	switch (udev->speed) {
	case USB_SPEED_SUPER:
=======
	if (urb->dev->speed >= USB_SPEED_SUPER) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* bMaxBurst is zero based: 0 means 1 packet per burst */
		max_burst = urb->ep->ss_ep_comp.bMaxBurst;
		residue = total_packet_count % (max_burst + 1);
		/* If residue is zero, the last burst contains (max_burst + 1)
		 * number of packets, but the TLBPC field is zero-based.
		 */
		if (residue == 0)
			return max_burst;
		return residue - 1;
<<<<<<< HEAD
	default:
		if (total_packet_count == 0)
			return 0;
		return total_packet_count - 1;
	}
=======
	}
	if (total_packet_count == 0)
		return 0;
	return total_packet_count - 1;
}

/*
 * Calculates Frame ID field of the isochronous TRB identifies the
 * target frame that the Interval associated with this Isochronous
 * Transfer Descriptor will start on. Refer to 4.11.2.5 in 1.1 spec.
 *
 * Returns actual frame id on success, negative value on error.
 */
static int xhci_get_isoc_frame_id(struct xhci_hcd *xhci,
		struct urb *urb, int index)
{
	int start_frame, ist, ret = 0;
	int start_frame_id, end_frame_id, current_frame_id;

	if (urb->dev->speed == USB_SPEED_LOW ||
			urb->dev->speed == USB_SPEED_FULL)
		start_frame = urb->start_frame + index * urb->interval;
	else
		start_frame = (urb->start_frame + index * urb->interval) >> 3;

	/* Isochronous Scheduling Threshold (IST, bits 0~3 in HCSPARAMS2):
	 *
	 * If bit [3] of IST is cleared to '0', software can add a TRB no
	 * later than IST[2:0] Microframes before that TRB is scheduled to
	 * be executed.
	 * If bit [3] of IST is set to '1', software can add a TRB no later
	 * than IST[2:0] Frames before that TRB is scheduled to be executed.
	 */
	ist = HCS_IST(xhci->hcs_params2) & 0x7;
	if (HCS_IST(xhci->hcs_params2) & (1 << 3))
		ist <<= 3;

	/* Software shall not schedule an Isoch TD with a Frame ID value that
	 * is less than the Start Frame ID or greater than the End Frame ID,
	 * where:
	 *
	 * End Frame ID = (Current MFINDEX register value + 895 ms.) MOD 2048
	 * Start Frame ID = (Current MFINDEX register value + IST + 1) MOD 2048
	 *
	 * Both the End Frame ID and Start Frame ID values are calculated
	 * in microframes. When software determines the valid Frame ID value;
	 * The End Frame ID value should be rounded down to the nearest Frame
	 * boundary, and the Start Frame ID value should be rounded up to the
	 * nearest Frame boundary.
	 */
	current_frame_id = readl(&xhci->run_regs->microframe_index);
	start_frame_id = roundup(current_frame_id + ist + 1, 8);
	end_frame_id = rounddown(current_frame_id + 895 * 8, 8);

	start_frame &= 0x7ff;
	start_frame_id = (start_frame_id >> 3) & 0x7ff;
	end_frame_id = (end_frame_id >> 3) & 0x7ff;

	xhci_dbg(xhci, "%s: index %d, reg 0x%x start_frame_id 0x%x, end_frame_id 0x%x, start_frame 0x%x\n",
		 __func__, index, readl(&xhci->run_regs->microframe_index),
		 start_frame_id, end_frame_id, start_frame);

	if (start_frame_id < end_frame_id) {
		if (start_frame > end_frame_id ||
				start_frame < start_frame_id)
			ret = -EINVAL;
	} else if (start_frame_id > end_frame_id) {
		if ((start_frame > end_frame_id &&
				start_frame < start_frame_id))
			ret = -EINVAL;
	} else {
			ret = -EINVAL;
	}

	if (index == 0) {
		if (ret == -EINVAL || start_frame == start_frame_id) {
			start_frame = start_frame_id + 1;
			if (urb->dev->speed == USB_SPEED_LOW ||
					urb->dev->speed == USB_SPEED_FULL)
				urb->start_frame = start_frame;
			else
				urb->start_frame = start_frame << 3;
			ret = 0;
		}
	}

	if (ret) {
		xhci_warn(xhci, "Frame ID %d (reg %d, index %d) beyond range (%d, %d)\n",
				start_frame, current_frame_id, index,
				start_frame_id, end_frame_id);
		xhci_warn(xhci, "Ignore frame ID field, use SIA bit instead\n");
		return ret;
	}

	return start_frame;
}

/* Check if we should generate event interrupt for a TD in an isoc URB */
static bool trb_block_event_intr(struct xhci_hcd *xhci, int num_tds, int i,
				 struct xhci_interrupter *ir)
{
	if (xhci->hci_version < 0x100)
		return false;
	/* always generate an event interrupt for the last TD */
	if (i == num_tds - 1)
		return false;
	/*
	 * If AVOID_BEI is set the host handles full event rings poorly,
	 * generate an event at least every 8th TD to clear the event ring
	 */
	if (i && ir->isoc_bei_interval && xhci->quirks & XHCI_AVOID_BEI)
		return !!(i % ir->isoc_bei_interval);

	return true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* This is for isoc transfer */
static int xhci_queue_isoc_tx(struct xhci_hcd *xhci, gfp_t mem_flags,
		struct urb *urb, int slot_id, unsigned int ep_index)
{
<<<<<<< HEAD
=======
	struct xhci_interrupter *ir;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct xhci_ring *ep_ring;
	struct urb_priv *urb_priv;
	struct xhci_td *td;
	int num_tds, trbs_per_td;
	struct xhci_generic_trb *start_trb;
	bool first_trb;
	int start_cycle;
	u32 field, length_field;
	int running_total, trb_buff_len, td_len, td_remain_len, ret;
	u64 start_addr, addr;
	int i, j;
	bool more_trbs_coming;
<<<<<<< HEAD

	ep_ring = xhci->devs[slot_id]->eps[ep_index].ring;
=======
	struct xhci_virt_ep *xep;
	int frame_id;

	xep = &xhci->devs[slot_id]->eps[ep_index];
	ep_ring = xhci->devs[slot_id]->eps[ep_index].ring;
	ir = xhci->interrupters[0];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	num_tds = urb->number_of_packets;
	if (num_tds < 1) {
		xhci_dbg(xhci, "Isoc URB with zero packets?\n");
		return -EINVAL;
	}
<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	start_addr = (u64) urb->transfer_dma;
	start_trb = &ep_ring->enqueue->generic;
	start_cycle = ep_ring->cycle_state;

	urb_priv = urb->hcpriv;
<<<<<<< HEAD
	/* Queue the first TRB, even if it's zero-length */
	for (i = 0; i < num_tds; i++) {
		unsigned int total_packet_count;
		unsigned int burst_count;
		unsigned int residue;
=======
	/* Queue the TRBs for each TD, even if they are zero-length */
	for (i = 0; i < num_tds; i++) {
		unsigned int total_pkt_count, max_pkt;
		unsigned int burst_count, last_burst_pkt_count;
		u32 sia_frame_id;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		first_trb = true;
		running_total = 0;
		addr = start_addr + urb->iso_frame_desc[i].offset;
		td_len = urb->iso_frame_desc[i].length;
		td_remain_len = td_len;
<<<<<<< HEAD
		total_packet_count = DIV_ROUND_UP(td_len,
				GET_MAX_PACKET(
					usb_endpoint_maxp(&urb->ep->desc)));
		/* A zero-length transfer still involves at least one packet. */
		if (total_packet_count == 0)
			total_packet_count++;
		burst_count = xhci_get_burst_count(xhci, urb->dev, urb,
				total_packet_count);
		residue = xhci_get_last_burst_packet_count(xhci,
				urb->dev, urb, total_packet_count);

		trbs_per_td = count_isoc_trbs_needed(xhci, urb, i);
=======
		max_pkt = usb_endpoint_maxp(&urb->ep->desc);
		total_pkt_count = DIV_ROUND_UP(td_len, max_pkt);

		/* A zero-length transfer still involves at least one packet. */
		if (total_pkt_count == 0)
			total_pkt_count++;
		burst_count = xhci_get_burst_count(xhci, urb, total_pkt_count);
		last_burst_pkt_count = xhci_get_last_burst_packet_count(xhci,
							urb, total_pkt_count);

		trbs_per_td = count_isoc_trbs_needed(urb, i);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		ret = prepare_transfer(xhci, xhci->devs[slot_id], ep_index,
				urb->stream_id, trbs_per_td, urb, i, mem_flags);
		if (ret < 0) {
			if (i == 0)
				return ret;
			goto cleanup;
		}
<<<<<<< HEAD

		td = urb_priv->td[i];
		for (j = 0; j < trbs_per_td; j++) {
			u32 remainder = 0;
			field = 0;

			if (first_trb) {
				field = TRB_TBC(burst_count) |
					TRB_TLBPC(residue);
				/* Queue the isoc TRB */
				field |= TRB_TYPE(TRB_ISOC);
				/* Assume URB_ISO_ASAP is set */
				field |= TRB_SIA;
				if (i == 0) {
					if (start_cycle == 0)
						field |= 0x1;
				} else
					field |= ep_ring->cycle_state;
				first_trb = false;
			} else {
				/* Queue other normal TRBs */
				field |= TRB_TYPE(TRB_NORMAL);
				field |= ep_ring->cycle_state;
			}
=======
		td = &urb_priv->td[i];
		td->num_trbs = trbs_per_td;
		/* use SIA as default, if frame id is used overwrite it */
		sia_frame_id = TRB_SIA;
		if (!(urb->transfer_flags & URB_ISO_ASAP) &&
		    HCC_CFC(xhci->hcc_params)) {
			frame_id = xhci_get_isoc_frame_id(xhci, urb, i);
			if (frame_id >= 0)
				sia_frame_id = TRB_FRAME_ID(frame_id);
		}
		/*
		 * Set isoc specific data for the first TRB in a TD.
		 * Prevent HW from getting the TRBs by keeping the cycle state
		 * inverted in the first TDs isoc TRB.
		 */
		field = TRB_TYPE(TRB_ISOC) |
			TRB_TLBPC(last_burst_pkt_count) |
			sia_frame_id |
			(i ? ep_ring->cycle_state : !start_cycle);

		/* xhci 1.1 with ETE uses TD_Size field for TBC, old is Rsvdz */
		if (!xep->use_extended_tbc)
			field |= TRB_TBC(burst_count);

		/* fill the rest of the TRB fields, and remaining normal TRBs */
		for (j = 0; j < trbs_per_td; j++) {
			u32 remainder = 0;

			/* only first TRB is isoc, overwrite otherwise */
			if (!first_trb)
				field = TRB_TYPE(TRB_NORMAL) |
					ep_ring->cycle_state;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

			/* Only set interrupt on short packet for IN EPs */
			if (usb_urb_dir_in(urb))
				field |= TRB_ISP;

<<<<<<< HEAD
			/* Chain all the TRBs together; clear the chain bit in
			 * the last TRB to indicate it's the last TRB in the
			 * chain.
			 */
			if (j < trbs_per_td - 1) {
				field |= TRB_CHAIN;
				more_trbs_coming = true;
			} else {
				td->last_trb = ep_ring->enqueue;
				field |= TRB_IOC;
				if (xhci->hci_version == 0x100 &&
						!(xhci->quirks &
							XHCI_AVOID_BEI)) {
					/* Set BEI bit except for the last td */
					if (i < num_tds - 1)
						field |= TRB_BEI;
				}
				more_trbs_coming = false;
			}

			/* Calculate TRB length */
			trb_buff_len = TRB_MAX_BUFF_SIZE -
				(addr & ((1 << TRB_MAX_BUFF_SHIFT) - 1));
=======
			/* Set the chain bit for all except the last TRB  */
			if (j < trbs_per_td - 1) {
				more_trbs_coming = true;
				field |= TRB_CHAIN;
			} else {
				more_trbs_coming = false;
				td->last_trb = ep_ring->enqueue;
				td->last_trb_seg = ep_ring->enq_seg;
				field |= TRB_IOC;
				if (trb_block_event_intr(xhci, num_tds, i, ir))
					field |= TRB_BEI;
			}
			/* Calculate TRB length */
			trb_buff_len = TRB_BUFF_LEN_UP_TO_BOUNDARY(addr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (trb_buff_len > td_remain_len)
				trb_buff_len = td_remain_len;

			/* Set the TRB length, TD size, & interrupter fields. */
<<<<<<< HEAD
			if (xhci->hci_version < 0x100) {
				remainder = xhci_td_remainder(
						td_len - running_total);
			} else {
				remainder = xhci_v1_0_td_remainder(
						running_total, trb_buff_len,
						total_packet_count, urb,
						(trbs_per_td - j - 1));
			}
			length_field = TRB_LEN(trb_buff_len) |
				remainder |
				TRB_INTR_TARGET(0);

=======
			remainder = xhci_td_remainder(xhci, running_total,
						   trb_buff_len, td_len,
						   urb, more_trbs_coming);

			length_field = TRB_LEN(trb_buff_len) |
				TRB_INTR_TARGET(0);

			/* xhci 1.1 with ETE uses TD Size field for TBC */
			if (first_trb && xep->use_extended_tbc)
				length_field |= TRB_TD_SIZE_TBC(burst_count);
			else
				length_field |= TRB_TD_SIZE(remainder);
			first_trb = false;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			queue_trb(xhci, ep_ring, more_trbs_coming,
				lower_32_bits(addr),
				upper_32_bits(addr),
				length_field,
				field);
			running_total += trb_buff_len;

			addr += trb_buff_len;
			td_remain_len -= trb_buff_len;
		}

		/* Check TD length */
		if (running_total != td_len) {
			xhci_err(xhci, "ISOC TD length unmatch\n");
			ret = -EINVAL;
			goto cleanup;
		}
	}

<<<<<<< HEAD
=======
	/* store the next frame id */
	if (HCC_CFC(xhci->hcc_params))
		xep->next_frame_id = urb->start_frame + num_tds * urb->interval;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (xhci_to_hcd(xhci)->self.bandwidth_isoc_reqs == 0) {
		if (xhci->quirks & XHCI_AMD_PLL_FIX)
			usb_amd_quirk_pll_disable();
	}
	xhci_to_hcd(xhci)->self.bandwidth_isoc_reqs++;

	giveback_first_trb(xhci, slot_id, ep_index, urb->stream_id,
			start_cycle, start_trb);
	return 0;
cleanup:
	/* Clean up a partially enqueued isoc transfer. */

	for (i--; i >= 0; i--)
<<<<<<< HEAD
		list_del_init(&urb_priv->td[i]->td_list);
=======
		list_del_init(&urb_priv->td[i].td_list);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Use the first TD as a temporary variable to turn the TDs we've queued
	 * into No-ops with a software-owned cycle bit. That way the hardware
	 * won't accidentally start executing bogus TDs when we partially
	 * overwrite them.  td->first_trb and td->start_seg are already set.
	 */
<<<<<<< HEAD
	urb_priv->td[0]->last_trb = ep_ring->enqueue;
	/* Every TRB except the first & last will have its cycle bit flipped. */
	td_to_noop(xhci, ep_ring, urb_priv->td[0], true);

	/* Reset the ring enqueue back to the first TRB and its cycle bit. */
	ep_ring->enqueue = urb_priv->td[0]->first_trb;
	ep_ring->enq_seg = urb_priv->td[0]->start_seg;
	ep_ring->cycle_state = start_cycle;
	ep_ring->num_trbs_free = ep_ring->num_trbs_free_temp;
=======
	urb_priv->td[0].last_trb = ep_ring->enqueue;
	/* Every TRB except the first & last will have its cycle bit flipped. */
	td_to_noop(xhci, ep_ring, &urb_priv->td[0], true);

	/* Reset the ring enqueue back to the first TRB and its cycle bit. */
	ep_ring->enqueue = urb_priv->td[0].first_trb;
	ep_ring->enq_seg = urb_priv->td[0].start_seg;
	ep_ring->cycle_state = start_cycle;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	usb_hcd_unlink_urb_from_ep(bus_to_hcd(urb->dev->bus), urb);
	return ret;
}

/*
 * Check transfer ring to guarantee there is enough room for the urb.
 * Update ISO URB start_frame and interval.
<<<<<<< HEAD
 * Update interval as xhci_queue_intr_tx does. Just use xhci frame_index to
 * update the urb->start_frame by now.
 * Always assume URB_ISO_ASAP set, and NEVER use urb->start_frame as input.
=======
 * Update interval as xhci_queue_intr_tx does. Use xhci frame_index to
 * update urb->start_frame if URB_ISO_ASAP is set in transfer_flags or
 * Contiguous Frame ID is not supported by HC.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
int xhci_queue_isoc_tx_prepare(struct xhci_hcd *xhci, gfp_t mem_flags,
		struct urb *urb, int slot_id, unsigned int ep_index)
{
	struct xhci_virt_device *xdev;
	struct xhci_ring *ep_ring;
	struct xhci_ep_ctx *ep_ctx;
	int start_frame;
<<<<<<< HEAD
	int xhci_interval;
	int ep_interval;
	int num_tds, num_trbs, i;
	int ret;

	xdev = xhci->devs[slot_id];
=======
	int num_tds, num_trbs, i;
	int ret;
	struct xhci_virt_ep *xep;
	int ist;

	xdev = xhci->devs[slot_id];
	xep = &xhci->devs[slot_id]->eps[ep_index];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ep_ring = xdev->eps[ep_index].ring;
	ep_ctx = xhci_get_ep_ctx(xhci, xdev->out_ctx, ep_index);

	num_trbs = 0;
	num_tds = urb->number_of_packets;
	for (i = 0; i < num_tds; i++)
<<<<<<< HEAD
		num_trbs += count_isoc_trbs_needed(xhci, urb, i);
=======
		num_trbs += count_isoc_trbs_needed(urb, i);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Check the ring to guarantee there is enough room for the whole urb.
	 * Do not insert any td of the urb to the ring if the check failed.
	 */
<<<<<<< HEAD
	ret = prepare_ring(xhci, ep_ring, le32_to_cpu(ep_ctx->ep_info) & EP_STATE_MASK,
=======
	ret = prepare_ring(xhci, ep_ring, GET_EP_CTX_STATE(ep_ctx),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			   num_trbs, mem_flags);
	if (ret)
		return ret;

<<<<<<< HEAD
	start_frame = xhci_readl(xhci, &xhci->run_regs->microframe_index);
	start_frame &= 0x3fff;

	urb->start_frame = start_frame;
	if (urb->dev->speed == USB_SPEED_LOW ||
			urb->dev->speed == USB_SPEED_FULL)
		urb->start_frame >>= 3;

	xhci_interval = EP_INTERVAL_TO_UFRAMES(le32_to_cpu(ep_ctx->ep_info));
	ep_interval = urb->interval;
	/* Convert to microframes */
	if (urb->dev->speed == USB_SPEED_LOW ||
			urb->dev->speed == USB_SPEED_FULL)
		ep_interval *= 8;
	/* FIXME change this to a warning and a suggestion to use the new API
	 * to set the polling interval (once the API is added).
	 */
	if (xhci_interval != ep_interval) {
		if (printk_ratelimit())
			dev_dbg(&urb->dev->dev, "Driver uses different interval"
					" (%d microframe%s) than xHCI "
					"(%d microframe%s)\n",
					ep_interval,
					ep_interval == 1 ? "" : "s",
					xhci_interval,
					xhci_interval == 1 ? "" : "s");
		urb->interval = xhci_interval;
		/* Convert back to frames for LS/FS devices */
		if (urb->dev->speed == USB_SPEED_LOW ||
				urb->dev->speed == USB_SPEED_FULL)
			urb->interval /= 8;
	}
	ep_ring->num_trbs_free_temp = ep_ring->num_trbs_free;
=======
	/*
	 * Check interval value. This should be done before we start to
	 * calculate the start frame value.
	 */
	check_interval(xhci, urb, ep_ctx);

	/* Calculate the start frame and put it in urb->start_frame. */
	if (HCC_CFC(xhci->hcc_params) && !list_empty(&ep_ring->td_list)) {
		if (GET_EP_CTX_STATE(ep_ctx) ==	EP_STATE_RUNNING) {
			urb->start_frame = xep->next_frame_id;
			goto skip_start_over;
		}
	}

	start_frame = readl(&xhci->run_regs->microframe_index);
	start_frame &= 0x3fff;
	/*
	 * Round up to the next frame and consider the time before trb really
	 * gets scheduled by hardare.
	 */
	ist = HCS_IST(xhci->hcs_params2) & 0x7;
	if (HCS_IST(xhci->hcs_params2) & (1 << 3))
		ist <<= 3;
	start_frame += ist + XHCI_CFC_DELAY;
	start_frame = roundup(start_frame, 8);

	/*
	 * Round up to the next ESIT (Endpoint Service Interval Time) if ESIT
	 * is greate than 8 microframes.
	 */
	if (urb->dev->speed == USB_SPEED_LOW ||
			urb->dev->speed == USB_SPEED_FULL) {
		start_frame = roundup(start_frame, urb->interval << 3);
		urb->start_frame = start_frame >> 3;
	} else {
		start_frame = roundup(start_frame, urb->interval);
		urb->start_frame = start_frame;
	}

skip_start_over:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return xhci_queue_isoc_tx(xhci, mem_flags, urb, slot_id, ep_index);
}

/****		Command Ring Operations		****/

/* Generic function for queueing a command TRB on the command ring.
 * Check to make sure there's room on the command ring for one command TRB.
 * Also check that there's room reserved for commands that must not fail.
 * If this is a command that must not fail, meaning command_must_succeed = TRUE,
 * then only check for the number of reserved spots.
 * Don't decrement xhci->cmd_ring_reserved_trbs after we've queued the TRB
 * because the command event handler may want to resubmit a failed command.
 */
<<<<<<< HEAD
static int queue_command(struct xhci_hcd *xhci, u32 field1, u32 field2,
		u32 field3, u32 field4, bool command_must_succeed)
=======
static int queue_command(struct xhci_hcd *xhci, struct xhci_command *cmd,
			 u32 field1, u32 field2,
			 u32 field3, u32 field4, bool command_must_succeed)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int reserved_trbs = xhci->cmd_ring_reserved_trbs;
	int ret;

<<<<<<< HEAD
=======
	if ((xhci->xhc_state & XHCI_STATE_DYING) ||
		(xhci->xhc_state & XHCI_STATE_HALTED)) {
		xhci_dbg(xhci, "xHCI dying or halted, can't queue_command\n");
		return -ESHUTDOWN;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!command_must_succeed)
		reserved_trbs++;

	ret = prepare_ring(xhci, xhci->cmd_ring, EP_STATE_RUNNING,
			reserved_trbs, GFP_ATOMIC);
	if (ret < 0) {
		xhci_err(xhci, "ERR: No room for command on command ring\n");
		if (command_must_succeed)
			xhci_err(xhci, "ERR: Reserved TRB counting for "
					"unfailable commands failed.\n");
		return ret;
	}
<<<<<<< HEAD
=======

	cmd->command_trb = xhci->cmd_ring->enqueue;

	/* if there are no other commands queued we start the timeout timer */
	if (list_empty(&xhci->cmd_list)) {
		xhci->current_cmd = cmd;
		xhci_mod_cmd_timer(xhci);
	}

	list_add_tail(&cmd->cmd_list, &xhci->cmd_list);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	queue_trb(xhci, xhci->cmd_ring, false, field1, field2, field3,
			field4 | xhci->cmd_ring->cycle_state);
	return 0;
}

/* Queue a slot enable or disable request on the command ring */
<<<<<<< HEAD
int xhci_queue_slot_control(struct xhci_hcd *xhci, u32 trb_type, u32 slot_id)
{
	return queue_command(xhci, 0, 0, 0,
=======
int xhci_queue_slot_control(struct xhci_hcd *xhci, struct xhci_command *cmd,
		u32 trb_type, u32 slot_id)
{
	return queue_command(xhci, cmd, 0, 0, 0,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			TRB_TYPE(trb_type) | SLOT_ID_FOR_TRB(slot_id), false);
}

/* Queue an address device command TRB */
<<<<<<< HEAD
int xhci_queue_address_device(struct xhci_hcd *xhci, dma_addr_t in_ctx_ptr,
		u32 slot_id)
{
	return queue_command(xhci, lower_32_bits(in_ctx_ptr),
			upper_32_bits(in_ctx_ptr), 0,
			TRB_TYPE(TRB_ADDR_DEV) | SLOT_ID_FOR_TRB(slot_id),
			false);
}

int xhci_queue_vendor_command(struct xhci_hcd *xhci,
		u32 field1, u32 field2, u32 field3, u32 field4)
{
	return queue_command(xhci, field1, field2, field3, field4, false);
}

/* Queue a reset device command TRB */
int xhci_queue_reset_device(struct xhci_hcd *xhci, u32 slot_id)
{
	return queue_command(xhci, 0, 0, 0,
=======
int xhci_queue_address_device(struct xhci_hcd *xhci, struct xhci_command *cmd,
		dma_addr_t in_ctx_ptr, u32 slot_id, enum xhci_setup_dev setup)
{
	return queue_command(xhci, cmd, lower_32_bits(in_ctx_ptr),
			upper_32_bits(in_ctx_ptr), 0,
			TRB_TYPE(TRB_ADDR_DEV) | SLOT_ID_FOR_TRB(slot_id)
			| (setup == SETUP_CONTEXT_ONLY ? TRB_BSR : 0), false);
}

int xhci_queue_vendor_command(struct xhci_hcd *xhci, struct xhci_command *cmd,
		u32 field1, u32 field2, u32 field3, u32 field4)
{
	return queue_command(xhci, cmd, field1, field2, field3, field4, false);
}

/* Queue a reset device command TRB */
int xhci_queue_reset_device(struct xhci_hcd *xhci, struct xhci_command *cmd,
		u32 slot_id)
{
	return queue_command(xhci, cmd, 0, 0, 0,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			TRB_TYPE(TRB_RESET_DEV) | SLOT_ID_FOR_TRB(slot_id),
			false);
}

/* Queue a configure endpoint command TRB */
<<<<<<< HEAD
int xhci_queue_configure_endpoint(struct xhci_hcd *xhci, dma_addr_t in_ctx_ptr,
		u32 slot_id, bool command_must_succeed)
{
	return queue_command(xhci, lower_32_bits(in_ctx_ptr),
=======
int xhci_queue_configure_endpoint(struct xhci_hcd *xhci,
		struct xhci_command *cmd, dma_addr_t in_ctx_ptr,
		u32 slot_id, bool command_must_succeed)
{
	return queue_command(xhci, cmd, lower_32_bits(in_ctx_ptr),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			upper_32_bits(in_ctx_ptr), 0,
			TRB_TYPE(TRB_CONFIG_EP) | SLOT_ID_FOR_TRB(slot_id),
			command_must_succeed);
}

/* Queue an evaluate context command TRB */
<<<<<<< HEAD
int xhci_queue_evaluate_context(struct xhci_hcd *xhci, dma_addr_t in_ctx_ptr,
		u32 slot_id)
{
	return queue_command(xhci, lower_32_bits(in_ctx_ptr),
			upper_32_bits(in_ctx_ptr), 0,
			TRB_TYPE(TRB_EVAL_CONTEXT) | SLOT_ID_FOR_TRB(slot_id),
			false);
=======
int xhci_queue_evaluate_context(struct xhci_hcd *xhci, struct xhci_command *cmd,
		dma_addr_t in_ctx_ptr, u32 slot_id, bool command_must_succeed)
{
	return queue_command(xhci, cmd, lower_32_bits(in_ctx_ptr),
			upper_32_bits(in_ctx_ptr), 0,
			TRB_TYPE(TRB_EVAL_CONTEXT) | SLOT_ID_FOR_TRB(slot_id),
			command_must_succeed);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Suspend is set to indicate "Stop Endpoint Command" is being issued to stop
 * activity on an endpoint that is about to be suspended.
 */
<<<<<<< HEAD
int xhci_queue_stop_endpoint(struct xhci_hcd *xhci, int slot_id,
		unsigned int ep_index, int suspend)
=======
int xhci_queue_stop_endpoint(struct xhci_hcd *xhci, struct xhci_command *cmd,
			     int slot_id, unsigned int ep_index, int suspend)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	u32 trb_slot_id = SLOT_ID_FOR_TRB(slot_id);
	u32 trb_ep_index = EP_ID_FOR_TRB(ep_index);
	u32 type = TRB_TYPE(TRB_STOP_RING);
	u32 trb_suspend = SUSPEND_PORT_FOR_TRB(suspend);

<<<<<<< HEAD
	return queue_command(xhci, 0, 0, 0,
			trb_slot_id | trb_ep_index | type | trb_suspend, false);
}

/* Set Transfer Ring Dequeue Pointer command.
 * This should not be used for endpoints that have streams enabled.
 */
static int queue_set_tr_deq(struct xhci_hcd *xhci, int slot_id,
		unsigned int ep_index, unsigned int stream_id,
		struct xhci_segment *deq_seg,
		union xhci_trb *deq_ptr, u32 cycle_state)
{
	dma_addr_t addr;
	u32 trb_slot_id = SLOT_ID_FOR_TRB(slot_id);
	u32 trb_ep_index = EP_ID_FOR_TRB(ep_index);
	u32 trb_stream_id = STREAM_ID_FOR_TRB(stream_id);
	u32 type = TRB_TYPE(TRB_SET_DEQ);
	struct xhci_virt_ep *ep;

	addr = xhci_trb_virt_to_dma(deq_seg, deq_ptr);
	if (addr == 0) {
		xhci_warn(xhci, "WARN Cannot submit Set TR Deq Ptr\n");
		xhci_warn(xhci, "WARN deq seg = %p, deq pt = %p\n",
				deq_seg, deq_ptr);
		return 0;
	}
	ep = &xhci->devs[slot_id]->eps[ep_index];
	if ((ep->ep_state & SET_DEQ_PENDING)) {
		xhci_warn(xhci, "WARN Cannot submit Set TR Deq Ptr\n");
		xhci_warn(xhci, "A Set TR Deq Ptr command is pending.\n");
		return 0;
	}
	ep->queued_deq_seg = deq_seg;
	ep->queued_deq_ptr = deq_ptr;
	return queue_command(xhci, lower_32_bits(addr) | cycle_state,
			upper_32_bits(addr), trb_stream_id,
			trb_slot_id | trb_ep_index | type, false);
}

int xhci_queue_reset_ep(struct xhci_hcd *xhci, int slot_id,
		unsigned int ep_index)
=======
	return queue_command(xhci, cmd, 0, 0, 0,
			trb_slot_id | trb_ep_index | type | trb_suspend, false);
}

int xhci_queue_reset_ep(struct xhci_hcd *xhci, struct xhci_command *cmd,
			int slot_id, unsigned int ep_index,
			enum xhci_ep_reset_type reset_type)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	u32 trb_slot_id = SLOT_ID_FOR_TRB(slot_id);
	u32 trb_ep_index = EP_ID_FOR_TRB(ep_index);
	u32 type = TRB_TYPE(TRB_RESET_EP);

<<<<<<< HEAD
	return queue_command(xhci, 0, 0, 0, trb_slot_id | trb_ep_index | type,
			false);
=======
	if (reset_type == EP_SOFT_RESET)
		type |= TRB_TSP;

	return queue_command(xhci, cmd, 0, 0, 0,
			trb_slot_id | trb_ep_index | type, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
