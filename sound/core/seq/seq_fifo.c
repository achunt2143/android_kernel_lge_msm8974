<<<<<<< HEAD
/*
 *   ALSA sequencer FIFO
 *   Copyright (c) 1998 by Frank van de Pol <fvdpol@coil.demon.nl>
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
 *   ALSA sequencer FIFO
 *   Copyright (c) 1998 by Frank van de Pol <fvdpol@coil.demon.nl>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <sound/core.h>
#include <linux/slab.h>
<<<<<<< HEAD
=======
#include <linux/sched/signal.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "seq_fifo.h"
#include "seq_lock.h"


/* FIFO */

/* create new fifo */
struct snd_seq_fifo *snd_seq_fifo_new(int poolsize)
{
	struct snd_seq_fifo *f;

	f = kzalloc(sizeof(*f), GFP_KERNEL);
<<<<<<< HEAD
	if (f == NULL) {
		snd_printd("malloc failed for snd_seq_fifo_new() \n");
		return NULL;
	}
=======
	if (!f)
		return NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	f->pool = snd_seq_pool_new(poolsize);
	if (f->pool == NULL) {
		kfree(f);
		return NULL;
	}
	if (snd_seq_pool_init(f->pool) < 0) {
		snd_seq_pool_delete(&f->pool);
		kfree(f);
		return NULL;
	}

	spin_lock_init(&f->lock);
	snd_use_lock_init(&f->use_lock);
	init_waitqueue_head(&f->input_sleep);
	atomic_set(&f->overflow, 0);

	f->head = NULL;
	f->tail = NULL;
	f->cells = 0;
	
	return f;
}

void snd_seq_fifo_delete(struct snd_seq_fifo **fifo)
{
	struct snd_seq_fifo *f;

	if (snd_BUG_ON(!fifo))
		return;
	f = *fifo;
	if (snd_BUG_ON(!f))
		return;
	*fifo = NULL;

<<<<<<< HEAD
=======
	if (f->pool)
		snd_seq_pool_mark_closing(f->pool);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	snd_seq_fifo_clear(f);

	/* wake up clients if any */
	if (waitqueue_active(&f->input_sleep))
		wake_up(&f->input_sleep);

	/* release resources...*/
	/*....................*/

	if (f->pool) {
		snd_seq_pool_done(f->pool);
		snd_seq_pool_delete(&f->pool);
	}
	
	kfree(f);
}

static struct snd_seq_event_cell *fifo_cell_out(struct snd_seq_fifo *f);

/* clear queue */
void snd_seq_fifo_clear(struct snd_seq_fifo *f)
{
	struct snd_seq_event_cell *cell;
<<<<<<< HEAD
	unsigned long flags;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* clear overflow flag */
	atomic_set(&f->overflow, 0);

	snd_use_lock_sync(&f->use_lock);
<<<<<<< HEAD
	spin_lock_irqsave(&f->lock, flags);
=======
	guard(spinlock_irq)(&f->lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* drain the fifo */
	while ((cell = fifo_cell_out(f)) != NULL) {
		snd_seq_cell_free(cell);
	}
<<<<<<< HEAD
	spin_unlock_irqrestore(&f->lock, flags);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}


/* enqueue event to fifo */
int snd_seq_fifo_event_in(struct snd_seq_fifo *f,
			  struct snd_seq_event *event)
{
	struct snd_seq_event_cell *cell;
<<<<<<< HEAD
	unsigned long flags;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int err;

	if (snd_BUG_ON(!f))
		return -EINVAL;

	snd_use_lock_use(&f->use_lock);
<<<<<<< HEAD
	err = snd_seq_event_dup(f->pool, event, &cell, 1, NULL); /* always non-blocking */
	if (err < 0) {
		if (err == -ENOMEM)
=======
	err = snd_seq_event_dup(f->pool, event, &cell, 1, NULL, NULL); /* always non-blocking */
	if (err < 0) {
		if ((err == -ENOMEM) || (err == -EAGAIN))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			atomic_inc(&f->overflow);
		snd_use_lock_free(&f->use_lock);
		return err;
	}
		
	/* append new cells to fifo */
<<<<<<< HEAD
	spin_lock_irqsave(&f->lock, flags);
	if (f->tail != NULL)
		f->tail->next = cell;
	f->tail = cell;
	if (f->head == NULL)
		f->head = cell;
	f->cells++;
	spin_unlock_irqrestore(&f->lock, flags);
=======
	scoped_guard(spinlock_irqsave, &f->lock) {
		if (f->tail != NULL)
			f->tail->next = cell;
		f->tail = cell;
		if (f->head == NULL)
			f->head = cell;
		cell->next = NULL;
		f->cells++;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* wakeup client */
	if (waitqueue_active(&f->input_sleep))
		wake_up(&f->input_sleep);

	snd_use_lock_free(&f->use_lock);

	return 0; /* success */

}

/* dequeue cell from fifo */
static struct snd_seq_event_cell *fifo_cell_out(struct snd_seq_fifo *f)
{
	struct snd_seq_event_cell *cell;

<<<<<<< HEAD
	if ((cell = f->head) != NULL) {
=======
	cell = f->head;
	if (cell) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		f->head = cell->next;

		/* reset tail if this was the last element */
		if (f->tail == cell)
			f->tail = NULL;

		cell->next = NULL;
		f->cells--;
	}

	return cell;
}

/* dequeue cell from fifo and copy on user space */
int snd_seq_fifo_cell_out(struct snd_seq_fifo *f,
			  struct snd_seq_event_cell **cellp, int nonblock)
{
	struct snd_seq_event_cell *cell;
	unsigned long flags;
<<<<<<< HEAD
	wait_queue_t wait;
=======
	wait_queue_entry_t wait;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (snd_BUG_ON(!f))
		return -EINVAL;

	*cellp = NULL;
	init_waitqueue_entry(&wait, current);
	spin_lock_irqsave(&f->lock, flags);
	while ((cell = fifo_cell_out(f)) == NULL) {
		if (nonblock) {
			/* non-blocking - return immediately */
			spin_unlock_irqrestore(&f->lock, flags);
			return -EAGAIN;
		}
		set_current_state(TASK_INTERRUPTIBLE);
		add_wait_queue(&f->input_sleep, &wait);
<<<<<<< HEAD
		spin_unlock_irq(&f->lock);
		schedule();
		spin_lock_irq(&f->lock);
=======
		spin_unlock_irqrestore(&f->lock, flags);
		schedule();
		spin_lock_irqsave(&f->lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		remove_wait_queue(&f->input_sleep, &wait);
		if (signal_pending(current)) {
			spin_unlock_irqrestore(&f->lock, flags);
			return -ERESTARTSYS;
		}
	}
	spin_unlock_irqrestore(&f->lock, flags);
	*cellp = cell;

	return 0;
}


void snd_seq_fifo_cell_putback(struct snd_seq_fifo *f,
			       struct snd_seq_event_cell *cell)
{
<<<<<<< HEAD
	unsigned long flags;

	if (cell) {
		spin_lock_irqsave(&f->lock, flags);
		cell->next = f->head;
		f->head = cell;
		f->cells++;
		spin_unlock_irqrestore(&f->lock, flags);
=======
	if (cell) {
		guard(spinlock_irqsave)(&f->lock);
		cell->next = f->head;
		f->head = cell;
		if (!f->tail)
			f->tail = cell;
		f->cells++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}


/* polling; return non-zero if queue is available */
int snd_seq_fifo_poll_wait(struct snd_seq_fifo *f, struct file *file,
			   poll_table *wait)
{
	poll_wait(file, &f->input_sleep, wait);
	return (f->cells > 0);
}

/* change the size of pool; all old events are removed */
int snd_seq_fifo_resize(struct snd_seq_fifo *f, int poolsize)
{
<<<<<<< HEAD
	unsigned long flags;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct snd_seq_pool *newpool, *oldpool;
	struct snd_seq_event_cell *cell, *next, *oldhead;

	if (snd_BUG_ON(!f || !f->pool))
		return -EINVAL;

	/* allocate new pool */
	newpool = snd_seq_pool_new(poolsize);
	if (newpool == NULL)
		return -ENOMEM;
	if (snd_seq_pool_init(newpool) < 0) {
		snd_seq_pool_delete(&newpool);
		return -ENOMEM;
	}

<<<<<<< HEAD
	spin_lock_irqsave(&f->lock, flags);
	/* remember old pool */
	oldpool = f->pool;
	oldhead = f->head;
	/* exchange pools */
	f->pool = newpool;
	f->head = NULL;
	f->tail = NULL;
	f->cells = 0;
	/* NOTE: overflow flag is not cleared */
	spin_unlock_irqrestore(&f->lock, flags);
=======
	scoped_guard(spinlock_irq, &f->lock) {
		/* remember old pool */
		oldpool = f->pool;
		oldhead = f->head;
		/* exchange pools */
		f->pool = newpool;
		f->head = NULL;
		f->tail = NULL;
		f->cells = 0;
		/* NOTE: overflow flag is not cleared */
	}

	/* close the old pool and wait until all users are gone */
	snd_seq_pool_mark_closing(oldpool);
	snd_use_lock_sync(&f->use_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* release cells in old pool */
	for (cell = oldhead; cell; cell = next) {
		next = cell->next;
		snd_seq_cell_free(cell);
	}
	snd_seq_pool_delete(&oldpool);

	return 0;
}
<<<<<<< HEAD
=======

/* get the number of unused cells safely */
int snd_seq_fifo_unused_cells(struct snd_seq_fifo *f)
{
	int cells;

	if (!f)
		return 0;

	snd_use_lock_use(&f->use_lock);
	scoped_guard(spinlock_irqsave, &f->lock)
		cells = snd_seq_unused_cells(f->pool);
	snd_use_lock_free(&f->use_lock);
	return cells;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
