<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *	LAPB release 002
 *
 *	This code REQUIRES 2.1.15 or higher/ NET3.038
 *
<<<<<<< HEAD
 *	This module:
 *		This module is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *	History
 *	LAPB 001	Jonathan Naylor	Started Coding
 *	LAPB 002	Jonathan Naylor	New timer architecture.
 */

<<<<<<< HEAD
=======
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/socket.h>
#include <linux/in.h>
#include <linux/kernel.h>
#include <linux/jiffies.h>
#include <linux/timer.h>
#include <linux/string.h>
#include <linux/sockios.h>
#include <linux/net.h>
#include <linux/inet.h>
#include <linux/skbuff.h>
#include <net/sock.h>
<<<<<<< HEAD
#include <asm/uaccess.h>
=======
#include <linux/uaccess.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/fcntl.h>
#include <linux/mm.h>
#include <linux/interrupt.h>
#include <net/lapb.h>

<<<<<<< HEAD
static void lapb_t1timer_expiry(unsigned long);
static void lapb_t2timer_expiry(unsigned long);
=======
static void lapb_t1timer_expiry(struct timer_list *);
static void lapb_t2timer_expiry(struct timer_list *);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

void lapb_start_t1timer(struct lapb_cb *lapb)
{
	del_timer(&lapb->t1timer);

<<<<<<< HEAD
	lapb->t1timer.data     = (unsigned long)lapb;
	lapb->t1timer.function = &lapb_t1timer_expiry;
	lapb->t1timer.expires  = jiffies + lapb->t1;

=======
	lapb->t1timer.function = lapb_t1timer_expiry;
	lapb->t1timer.expires  = jiffies + lapb->t1;

	lapb->t1timer_running = true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	add_timer(&lapb->t1timer);
}

void lapb_start_t2timer(struct lapb_cb *lapb)
{
	del_timer(&lapb->t2timer);

<<<<<<< HEAD
	lapb->t2timer.data     = (unsigned long)lapb;
	lapb->t2timer.function = &lapb_t2timer_expiry;
	lapb->t2timer.expires  = jiffies + lapb->t2;

=======
	lapb->t2timer.function = lapb_t2timer_expiry;
	lapb->t2timer.expires  = jiffies + lapb->t2;

	lapb->t2timer_running = true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	add_timer(&lapb->t2timer);
}

void lapb_stop_t1timer(struct lapb_cb *lapb)
{
<<<<<<< HEAD
=======
	lapb->t1timer_running = false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	del_timer(&lapb->t1timer);
}

void lapb_stop_t2timer(struct lapb_cb *lapb)
{
<<<<<<< HEAD
=======
	lapb->t2timer_running = false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	del_timer(&lapb->t2timer);
}

int lapb_t1timer_running(struct lapb_cb *lapb)
{
<<<<<<< HEAD
	return timer_pending(&lapb->t1timer);
}

static void lapb_t2timer_expiry(unsigned long param)
{
	struct lapb_cb *lapb = (struct lapb_cb *)param;
=======
	return lapb->t1timer_running;
}

static void lapb_t2timer_expiry(struct timer_list *t)
{
	struct lapb_cb *lapb = from_timer(lapb, t, t2timer);

	spin_lock_bh(&lapb->lock);
	if (timer_pending(&lapb->t2timer)) /* A new timer has been set up */
		goto out;
	if (!lapb->t2timer_running) /* The timer has been stopped */
		goto out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (lapb->condition & LAPB_ACK_PENDING_CONDITION) {
		lapb->condition &= ~LAPB_ACK_PENDING_CONDITION;
		lapb_timeout_response(lapb);
	}
<<<<<<< HEAD
}

static void lapb_t1timer_expiry(unsigned long param)
{
	struct lapb_cb *lapb = (struct lapb_cb *)param;
=======
	lapb->t2timer_running = false;

out:
	spin_unlock_bh(&lapb->lock);
}

static void lapb_t1timer_expiry(struct timer_list *t)
{
	struct lapb_cb *lapb = from_timer(lapb, t, t1timer);

	spin_lock_bh(&lapb->lock);
	if (timer_pending(&lapb->t1timer)) /* A new timer has been set up */
		goto out;
	if (!lapb->t1timer_running) /* The timer has been stopped */
		goto out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (lapb->state) {

		/*
<<<<<<< HEAD
		 *	If we are a DCE, keep going DM .. DM .. DM
		 */
		case LAPB_STATE_0:
			if (lapb->mode & LAPB_DCE)
				lapb_send_control(lapb, LAPB_DM, LAPB_POLLOFF, LAPB_RESPONSE);
=======
		 *	If we are a DCE, send DM up to N2 times, then switch to
		 *	STATE_1 and send SABM(E).
		 */
		case LAPB_STATE_0:
			if (lapb->mode & LAPB_DCE &&
			    lapb->n2count != lapb->n2) {
				lapb->n2count++;
				lapb_send_control(lapb, LAPB_DM, LAPB_POLLOFF, LAPB_RESPONSE);
			} else {
				lapb->state = LAPB_STATE_1;
				lapb_establish_data_link(lapb);
			}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;

		/*
		 *	Awaiting connection state, send SABM(E), up to N2 times.
		 */
		case LAPB_STATE_1:
			if (lapb->n2count == lapb->n2) {
				lapb_clear_queues(lapb);
				lapb->state = LAPB_STATE_0;
				lapb_disconnect_indication(lapb, LAPB_TIMEDOUT);
<<<<<<< HEAD
#if LAPB_DEBUG > 0
				printk(KERN_DEBUG "lapb: (%p) S1 -> S0\n", lapb->dev);
#endif
				return;
			} else {
				lapb->n2count++;
				if (lapb->mode & LAPB_EXTENDED) {
#if LAPB_DEBUG > 1
					printk(KERN_DEBUG "lapb: (%p) S1 TX SABME(1)\n", lapb->dev);
#endif
					lapb_send_control(lapb, LAPB_SABME, LAPB_POLLON, LAPB_COMMAND);
				} else {
#if LAPB_DEBUG > 1
					printk(KERN_DEBUG "lapb: (%p) S1 TX SABM(1)\n", lapb->dev);
#endif
=======
				lapb_dbg(0, "(%p) S1 -> S0\n", lapb->dev);
				lapb->t1timer_running = false;
				goto out;
			} else {
				lapb->n2count++;
				if (lapb->mode & LAPB_EXTENDED) {
					lapb_dbg(1, "(%p) S1 TX SABME(1)\n",
						 lapb->dev);
					lapb_send_control(lapb, LAPB_SABME, LAPB_POLLON, LAPB_COMMAND);
				} else {
					lapb_dbg(1, "(%p) S1 TX SABM(1)\n",
						 lapb->dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					lapb_send_control(lapb, LAPB_SABM, LAPB_POLLON, LAPB_COMMAND);
				}
			}
			break;

		/*
		 *	Awaiting disconnection state, send DISC, up to N2 times.
		 */
		case LAPB_STATE_2:
			if (lapb->n2count == lapb->n2) {
				lapb_clear_queues(lapb);
				lapb->state = LAPB_STATE_0;
				lapb_disconnect_confirmation(lapb, LAPB_TIMEDOUT);
<<<<<<< HEAD
#if LAPB_DEBUG > 0
				printk(KERN_DEBUG "lapb: (%p) S2 -> S0\n", lapb->dev);
#endif
				return;
			} else {
				lapb->n2count++;
#if LAPB_DEBUG > 1
				printk(KERN_DEBUG "lapb: (%p) S2 TX DISC(1)\n", lapb->dev);
#endif
=======
				lapb_dbg(0, "(%p) S2 -> S0\n", lapb->dev);
				lapb->t1timer_running = false;
				goto out;
			} else {
				lapb->n2count++;
				lapb_dbg(1, "(%p) S2 TX DISC(1)\n", lapb->dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				lapb_send_control(lapb, LAPB_DISC, LAPB_POLLON, LAPB_COMMAND);
			}
			break;

		/*
		 *	Data transfer state, restransmit I frames, up to N2 times.
		 */
		case LAPB_STATE_3:
			if (lapb->n2count == lapb->n2) {
				lapb_clear_queues(lapb);
				lapb->state = LAPB_STATE_0;
				lapb_stop_t2timer(lapb);
				lapb_disconnect_indication(lapb, LAPB_TIMEDOUT);
<<<<<<< HEAD
#if LAPB_DEBUG > 0
				printk(KERN_DEBUG "lapb: (%p) S3 -> S0\n", lapb->dev);
#endif
				return;
			} else {
				lapb->n2count++;
				lapb_requeue_frames(lapb);
=======
				lapb_dbg(0, "(%p) S3 -> S0\n", lapb->dev);
				lapb->t1timer_running = false;
				goto out;
			} else {
				lapb->n2count++;
				lapb_requeue_frames(lapb);
				lapb_kick(lapb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			}
			break;

		/*
		 *	Frame reject state, restransmit FRMR frames, up to N2 times.
		 */
		case LAPB_STATE_4:
			if (lapb->n2count == lapb->n2) {
				lapb_clear_queues(lapb);
				lapb->state = LAPB_STATE_0;
				lapb_disconnect_indication(lapb, LAPB_TIMEDOUT);
<<<<<<< HEAD
#if LAPB_DEBUG > 0
				printk(KERN_DEBUG "lapb: (%p) S4 -> S0\n", lapb->dev);
#endif
				return;
=======
				lapb_dbg(0, "(%p) S4 -> S0\n", lapb->dev);
				lapb->t1timer_running = false;
				goto out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			} else {
				lapb->n2count++;
				lapb_transmit_frmr(lapb);
			}
			break;
	}

	lapb_start_t1timer(lapb);
<<<<<<< HEAD
=======

out:
	spin_unlock_bh(&lapb->lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
