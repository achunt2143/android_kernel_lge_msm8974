<<<<<<< HEAD
/*
 *   ALSA sequencer Priority Queue
 *   Copyright (c) 1998-1999 by Frank van de Pol <fvdpol@coil.demon.nl>
 *
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 *   ALSA sequencer Priority Queue
 *   Copyright (c) 1998-1999 by Frank van de Pol <fvdpol@coil.demon.nl>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/time.h>
#include <linux/slab.h>
#include <sound/core.h>
#include "seq_timer.h"
#include "seq_prioq.h"


/* Implementation is a simple linked list for now...

   This priority queue orders the events on timestamp. For events with an
   equeal timestamp the queue behaves as a FIFO. 

   *
   *           +-------+
   *  Head --> | first |
   *           +-------+
   *                 |next
   *           +-----v-+
   *           |       |
   *           +-------+
   *                 |
   *           +-----v-+
   *           |       |
   *           +-------+
   *                 |
   *           +-----v-+
   *  Tail --> | last  |
   *           +-------+
   *

 */



/* create new prioq (constructor) */
struct snd_seq_prioq *snd_seq_prioq_new(void)
{
	struct snd_seq_prioq *f;

	f = kzalloc(sizeof(*f), GFP_KERNEL);
<<<<<<< HEAD
	if (f == NULL) {
		snd_printd("oops: malloc failed for snd_seq_prioq_new()\n");
		return NULL;
	}
=======
	if (!f)
		return NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	
	spin_lock_init(&f->lock);
	f->head = NULL;
	f->tail = NULL;
	f->cells = 0;
	
	return f;
}

/* delete prioq (destructor) */
void snd_seq_prioq_delete(struct snd_seq_prioq **fifo)
{
	struct snd_seq_prioq *f = *fifo;
	*fifo = NULL;

	if (f == NULL) {
<<<<<<< HEAD
		snd_printd("oops: snd_seq_prioq_delete() called with NULL prioq\n");
=======
		pr_debug("ALSA: seq: snd_seq_prioq_delete() called with NULL prioq\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;
	}

	/* release resources...*/
	/*....................*/
	
	if (f->cells > 0) {
		/* drain prioQ */
		while (f->cells > 0)
<<<<<<< HEAD
			snd_seq_cell_free(snd_seq_prioq_cell_out(f));
=======
			snd_seq_cell_free(snd_seq_prioq_cell_out(f, NULL));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	
	kfree(f);
}




/* compare timestamp between events */
/* return 1 if a >= b; 0 */
static inline int compare_timestamp(struct snd_seq_event *a,
				    struct snd_seq_event *b)
{
	if ((a->flags & SNDRV_SEQ_TIME_STAMP_MASK) == SNDRV_SEQ_TIME_STAMP_TICK) {
		/* compare ticks */
		return (snd_seq_compare_tick_time(&a->time.tick, &b->time.tick));
	} else {
		/* compare real time */
		return (snd_seq_compare_real_time(&a->time.time, &b->time.time));
	}
}

/* compare timestamp between events */
/* return negative if a < b;
 *        zero     if a = b;
 *        positive if a > b;
 */
static inline int compare_timestamp_rel(struct snd_seq_event *a,
					struct snd_seq_event *b)
{
	if ((a->flags & SNDRV_SEQ_TIME_STAMP_MASK) == SNDRV_SEQ_TIME_STAMP_TICK) {
		/* compare ticks */
		if (a->time.tick > b->time.tick)
			return 1;
		else if (a->time.tick == b->time.tick)
			return 0;
		else
			return -1;
	} else {
		/* compare real time */
		if (a->time.time.tv_sec > b->time.time.tv_sec)
			return 1;
		else if (a->time.time.tv_sec == b->time.time.tv_sec) {
			if (a->time.time.tv_nsec > b->time.time.tv_nsec)
				return 1;
			else if (a->time.time.tv_nsec == b->time.time.tv_nsec)
				return 0;
			else
				return -1;
		} else
			return -1;
	}
}

/* enqueue cell to prioq */
int snd_seq_prioq_cell_in(struct snd_seq_prioq * f,
			  struct snd_seq_event_cell * cell)
{
	struct snd_seq_event_cell *cur, *prev;
<<<<<<< HEAD
	unsigned long flags;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int count;
	int prior;

	if (snd_BUG_ON(!f || !cell))
		return -EINVAL;
	
	/* check flags */
	prior = (cell->event.flags & SNDRV_SEQ_PRIORITY_MASK);

<<<<<<< HEAD
	spin_lock_irqsave(&f->lock, flags);
=======
	guard(spinlock_irqsave)(&f->lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* check if this element needs to inserted at the end (ie. ordered 
	   data is inserted) This will be very likeley if a sequencer 
	   application or midi file player is feeding us (sequential) data */
	if (f->tail && !prior) {
		if (compare_timestamp(&cell->event, &f->tail->event)) {
			/* add new cell to tail of the fifo */
			f->tail->next = cell;
			f->tail = cell;
			cell->next = NULL;
			f->cells++;
<<<<<<< HEAD
			spin_unlock_irqrestore(&f->lock, flags);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return 0;
		}
	}
	/* traverse list of elements to find the place where the new cell is
	   to be inserted... Note that this is a order n process ! */

	prev = NULL;		/* previous cell */
	cur = f->head;		/* cursor */

	count = 10000; /* FIXME: enough big, isn't it? */
	while (cur != NULL) {
		/* compare timestamps */
		int rel = compare_timestamp_rel(&cell->event, &cur->event);
		if (rel < 0)
			/* new cell has earlier schedule time, */
			break;
		else if (rel == 0 && prior)
			/* equal schedule time and prior to others */
			break;
		/* new cell has equal or larger schedule time, */
		/* move cursor to next cell */
		prev = cur;
		cur = cur->next;
		if (! --count) {
<<<<<<< HEAD
			spin_unlock_irqrestore(&f->lock, flags);
			snd_printk(KERN_ERR "cannot find a pointer.. infinite loop?\n");
=======
			pr_err("ALSA: seq: cannot find a pointer.. infinite loop?\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -EINVAL;
		}
	}

	/* insert it before cursor */
	if (prev != NULL)
		prev->next = cell;
	cell->next = cur;

	if (f->head == cur) /* this is the first cell, set head to it */
		f->head = cell;
	if (cur == NULL) /* reached end of the list */
		f->tail = cell;
	f->cells++;
<<<<<<< HEAD
	spin_unlock_irqrestore(&f->lock, flags);
	return 0;
}

/* dequeue cell from prioq */
struct snd_seq_event_cell *snd_seq_prioq_cell_out(struct snd_seq_prioq *f)
{
	struct snd_seq_event_cell *cell;
	unsigned long flags;

	if (f == NULL) {
		snd_printd("oops: snd_seq_prioq_cell_in() called with NULL prioq\n");
		return NULL;
	}
	spin_lock_irqsave(&f->lock, flags);

	cell = f->head;
=======
	return 0;
}

/* return 1 if the current time >= event timestamp */
static int event_is_ready(struct snd_seq_event *ev, void *current_time)
{
	if ((ev->flags & SNDRV_SEQ_TIME_STAMP_MASK) == SNDRV_SEQ_TIME_STAMP_TICK)
		return snd_seq_compare_tick_time(current_time, &ev->time.tick);
	else
		return snd_seq_compare_real_time(current_time, &ev->time.time);
}

/* dequeue cell from prioq */
struct snd_seq_event_cell *snd_seq_prioq_cell_out(struct snd_seq_prioq *f,
						  void *current_time)
{
	struct snd_seq_event_cell *cell;

	if (f == NULL) {
		pr_debug("ALSA: seq: snd_seq_prioq_cell_in() called with NULL prioq\n");
		return NULL;
	}

	guard(spinlock_irqsave)(&f->lock);
	cell = f->head;
	if (cell && current_time && !event_is_ready(&cell->event, current_time))
		cell = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (cell) {
		f->head = cell->next;

		/* reset tail if this was the last element */
		if (f->tail == cell)
			f->tail = NULL;

		cell->next = NULL;
		f->cells--;
	}

<<<<<<< HEAD
	spin_unlock_irqrestore(&f->lock, flags);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return cell;
}

/* return number of events available in prioq */
int snd_seq_prioq_avail(struct snd_seq_prioq * f)
{
	if (f == NULL) {
<<<<<<< HEAD
		snd_printd("oops: snd_seq_prioq_cell_in() called with NULL prioq\n");
=======
		pr_debug("ALSA: seq: snd_seq_prioq_cell_in() called with NULL prioq\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;
	}
	return f->cells;
}

<<<<<<< HEAD

/* peek at cell at the head of the prioq */
struct snd_seq_event_cell *snd_seq_prioq_cell_peek(struct snd_seq_prioq * f)
{
	if (f == NULL) {
		snd_printd("oops: snd_seq_prioq_cell_in() called with NULL prioq\n");
		return NULL;
	}
	return f->head;
}


static inline int prioq_match(struct snd_seq_event_cell *cell,
			      int client, int timestamp)
{
	if (cell->event.source.client == client ||
	    cell->event.dest.client == client)
		return 1;
	if (!timestamp)
		return 0;
	switch (cell->event.flags & SNDRV_SEQ_TIME_STAMP_MASK) {
	case SNDRV_SEQ_TIME_STAMP_TICK:
		if (cell->event.time.tick)
			return 1;
		break;
	case SNDRV_SEQ_TIME_STAMP_REAL:
		if (cell->event.time.time.tv_sec ||
		    cell->event.time.time.tv_nsec)
			return 1;
		break;
	}
	return 0;
}

/* remove cells for left client */
void snd_seq_prioq_leave(struct snd_seq_prioq * f, int client, int timestamp)
{
	register struct snd_seq_event_cell *cell, *next;
	unsigned long flags;
=======
/* remove cells matching with the condition */
static void prioq_remove_cells(struct snd_seq_prioq *f,
			       bool (*match)(struct snd_seq_event_cell *cell,
					     void *arg),
			       void *arg)
{
	register struct snd_seq_event_cell *cell, *next;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct snd_seq_event_cell *prev = NULL;
	struct snd_seq_event_cell *freefirst = NULL, *freeprev = NULL, *freenext;

	/* collect all removed cells */
<<<<<<< HEAD
	spin_lock_irqsave(&f->lock, flags);
	cell = f->head;
	while (cell) {
		next = cell->next;
		if (prioq_match(cell, client, timestamp)) {
			/* remove cell from prioq */
			if (cell == f->head) {
				f->head = cell->next;
			} else {
				prev->next = cell->next;
			}
			if (cell == f->tail)
				f->tail = cell->next;
			f->cells--;
			/* add cell to free list */
			cell->next = NULL;
			if (freefirst == NULL) {
				freefirst = cell;
			} else {
				freeprev->next = cell;
			}
			freeprev = cell;
		} else {
#if 0
			printk(KERN_DEBUG "type = %i, source = %i, dest = %i, "
			       "client = %i\n",
				cell->event.type,
				cell->event.source.client,
				cell->event.dest.client,
				client);
#endif
			prev = cell;
		}
		cell = next;		
	}
	spin_unlock_irqrestore(&f->lock, flags);	
=======
	scoped_guard(spinlock_irqsave, &f->lock) {
		for (cell = f->head; cell; cell = next) {
			next = cell->next;
			if (!match(cell, arg)) {
				prev = cell;
				continue;
			}

			/* remove cell from prioq */
			if (cell == f->head)
				f->head = cell->next;
			else
				prev->next = cell->next;
			if (cell == f->tail)
				f->tail = cell->next;
			f->cells--;

			/* add cell to free list */
			cell->next = NULL;
			if (freefirst == NULL)
				freefirst = cell;
			else
				freeprev->next = cell;
			freeprev = cell;
		}
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* remove selected cells */
	while (freefirst) {
		freenext = freefirst->next;
		snd_seq_cell_free(freefirst);
		freefirst = freenext;
	}
}

<<<<<<< HEAD
static int prioq_remove_match(struct snd_seq_remove_events *info,
			      struct snd_seq_event *ev)
{
	int res;

	if (info->remove_mode & SNDRV_SEQ_REMOVE_DEST) {
		if (ev->dest.client != info->dest.client ||
				ev->dest.port != info->dest.port)
			return 0;
	}
	if (info->remove_mode & SNDRV_SEQ_REMOVE_DEST_CHANNEL) {
		if (! snd_seq_ev_is_channel_type(ev))
			return 0;
		/* data.note.channel and data.control.channel are identical */
		if (ev->data.note.channel != info->channel)
			return 0;
=======
struct prioq_match_arg {
	int client;
	int timestamp;
};

static inline bool prioq_match(struct snd_seq_event_cell *cell, void *arg)
{
	struct prioq_match_arg *v = arg;

	if (cell->event.source.client == v->client ||
	    cell->event.dest.client == v->client)
		return true;
	if (!v->timestamp)
		return false;
	switch (cell->event.flags & SNDRV_SEQ_TIME_STAMP_MASK) {
	case SNDRV_SEQ_TIME_STAMP_TICK:
		if (cell->event.time.tick)
			return true;
		break;
	case SNDRV_SEQ_TIME_STAMP_REAL:
		if (cell->event.time.time.tv_sec ||
		    cell->event.time.time.tv_nsec)
			return true;
		break;
	}
	return false;
}

/* remove cells for left client */
void snd_seq_prioq_leave(struct snd_seq_prioq *f, int client, int timestamp)
{
	struct prioq_match_arg arg = { client, timestamp };

	return prioq_remove_cells(f, prioq_match, &arg);
}

struct prioq_remove_match_arg {
	int client;
	struct snd_seq_remove_events *info;
};

static bool prioq_remove_match(struct snd_seq_event_cell *cell, void *arg)
{
	struct prioq_remove_match_arg *v = arg;
	struct snd_seq_event *ev = &cell->event;
	struct snd_seq_remove_events *info = v->info;
	int res;

	if (ev->source.client != v->client)
		return false;

	if (info->remove_mode & SNDRV_SEQ_REMOVE_DEST) {
		if (ev->dest.client != info->dest.client ||
				ev->dest.port != info->dest.port)
			return false;
	}
	if (info->remove_mode & SNDRV_SEQ_REMOVE_DEST_CHANNEL) {
		if (! snd_seq_ev_is_channel_type(ev))
			return false;
		/* data.note.channel and data.control.channel are identical */
		if (ev->data.note.channel != info->channel)
			return false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	if (info->remove_mode & SNDRV_SEQ_REMOVE_TIME_AFTER) {
		if (info->remove_mode & SNDRV_SEQ_REMOVE_TIME_TICK)
			res = snd_seq_compare_tick_time(&ev->time.tick, &info->time.tick);
		else
			res = snd_seq_compare_real_time(&ev->time.time, &info->time.time);
		if (!res)
<<<<<<< HEAD
			return 0;
=======
			return false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	if (info->remove_mode & SNDRV_SEQ_REMOVE_TIME_BEFORE) {
		if (info->remove_mode & SNDRV_SEQ_REMOVE_TIME_TICK)
			res = snd_seq_compare_tick_time(&ev->time.tick, &info->time.tick);
		else
			res = snd_seq_compare_real_time(&ev->time.time, &info->time.time);
		if (res)
<<<<<<< HEAD
			return 0;
	}
	if (info->remove_mode & SNDRV_SEQ_REMOVE_EVENT_TYPE) {
		if (ev->type != info->type)
			return 0;
=======
			return false;
	}
	if (info->remove_mode & SNDRV_SEQ_REMOVE_EVENT_TYPE) {
		if (ev->type != info->type)
			return false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	if (info->remove_mode & SNDRV_SEQ_REMOVE_IGNORE_OFF) {
		/* Do not remove off events */
		switch (ev->type) {
		case SNDRV_SEQ_EVENT_NOTEOFF:
		/* case SNDRV_SEQ_EVENT_SAMPLE_STOP: */
<<<<<<< HEAD
			return 0;
=======
			return false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		default:
			break;
		}
	}
	if (info->remove_mode & SNDRV_SEQ_REMOVE_TAG_MATCH) {
		if (info->tag != ev->tag)
<<<<<<< HEAD
			return 0;
	}

	return 1;
=======
			return false;
	}

	return true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* remove cells matching remove criteria */
void snd_seq_prioq_remove_events(struct snd_seq_prioq * f, int client,
				 struct snd_seq_remove_events *info)
{
<<<<<<< HEAD
	struct snd_seq_event_cell *cell, *next;
	unsigned long flags;
	struct snd_seq_event_cell *prev = NULL;
	struct snd_seq_event_cell *freefirst = NULL, *freeprev = NULL, *freenext;

	/* collect all removed cells */
	spin_lock_irqsave(&f->lock, flags);
	cell = f->head;

	while (cell) {
		next = cell->next;
		if (cell->event.source.client == client &&
			prioq_remove_match(info, &cell->event)) {

			/* remove cell from prioq */
			if (cell == f->head) {
				f->head = cell->next;
			} else {
				prev->next = cell->next;
			}

			if (cell == f->tail)
				f->tail = cell->next;
			f->cells--;

			/* add cell to free list */
			cell->next = NULL;
			if (freefirst == NULL) {
				freefirst = cell;
			} else {
				freeprev->next = cell;
			}

			freeprev = cell;
		} else {
			prev = cell;
		}
		cell = next;		
	}
	spin_unlock_irqrestore(&f->lock, flags);	

	/* remove selected cells */
	while (freefirst) {
		freenext = freefirst->next;
		snd_seq_cell_free(freefirst);
		freefirst = freenext;
	}
}


=======
	struct prioq_remove_match_arg arg = { client, info };

	return prioq_remove_cells(f, prioq_remove_match, &arg);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
