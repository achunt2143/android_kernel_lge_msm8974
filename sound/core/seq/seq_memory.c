<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  ALSA sequencer Memory Manager
 *  Copyright (c) 1998 by Frank van de Pol <fvdpol@coil.demon.nl>
 *                        Jaroslav Kysela <perex@perex.cz>
 *                2000 by Takashi Iwai <tiwai@suse.de>
<<<<<<< HEAD
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
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/init.h>
#include <linux/export.h>
#include <linux/slab.h>
<<<<<<< HEAD
#include <linux/vmalloc.h>
=======
#include <linux/sched/signal.h>
#include <linux/mm.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <sound/core.h>

#include <sound/seq_kernel.h>
#include "seq_memory.h"
#include "seq_queue.h"
#include "seq_info.h"
#include "seq_lock.h"

static inline int snd_seq_pool_available(struct snd_seq_pool *pool)
{
	return pool->total_elements - atomic_read(&pool->counter);
}

static inline int snd_seq_output_ok(struct snd_seq_pool *pool)
{
	return snd_seq_pool_available(pool) >= pool->room;
}

/*
 * Variable length event:
 * The event like sysex uses variable length type.
 * The external data may be stored in three different formats.
 * 1) kernel space
 *    This is the normal case.
 *      ext.data.len = length
 *      ext.data.ptr = buffer pointer
 * 2) user space
 *    When an event is generated via read(), the external data is
 *    kept in user space until expanded.
 *      ext.data.len = length | SNDRV_SEQ_EXT_USRPTR
 *      ext.data.ptr = userspace pointer
 * 3) chained cells
 *    When the variable length event is enqueued (in prioq or fifo),
 *    the external data is decomposed to several cells.
 *      ext.data.len = length | SNDRV_SEQ_EXT_CHAINED
 *      ext.data.ptr = the additiona cell head
 *         -> cell.next -> cell.next -> ..
 */

/*
 * exported:
 * call dump function to expand external data.
 */

static int get_var_len(const struct snd_seq_event *event)
{
	if ((event->flags & SNDRV_SEQ_EVENT_LENGTH_MASK) != SNDRV_SEQ_EVENT_LENGTH_VARIABLE)
		return -EINVAL;

	return event->data.ext.len & ~SNDRV_SEQ_EXT_MASK;
}

<<<<<<< HEAD
int snd_seq_dump_var_event(const struct snd_seq_event *event,
			   snd_seq_dump_func_t func, void *private_data)
=======
static int dump_var_event(const struct snd_seq_event *event,
			  snd_seq_dump_func_t func, void *private_data,
			  int offset, int maxlen)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int len, err;
	struct snd_seq_event_cell *cell;

<<<<<<< HEAD
	if ((len = get_var_len(event)) <= 0)
		return len;
=======
	len = get_var_len(event);
	if (len <= 0)
		return len;
	if (len <= offset)
		return 0;
	if (maxlen && len > offset + maxlen)
		len = offset + maxlen;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (event->data.ext.len & SNDRV_SEQ_EXT_USRPTR) {
		char buf[32];
		char __user *curptr = (char __force __user *)event->data.ext.ptr;
<<<<<<< HEAD
=======
		curptr += offset;
		len -= offset;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		while (len > 0) {
			int size = sizeof(buf);
			if (len < size)
				size = len;
			if (copy_from_user(buf, curptr, size))
				return -EFAULT;
			err = func(private_data, buf, size);
			if (err < 0)
				return err;
			curptr += size;
			len -= size;
		}
		return 0;
<<<<<<< HEAD
	} if (! (event->data.ext.len & SNDRV_SEQ_EXT_CHAINED)) {
		return func(private_data, event->data.ext.ptr, len);
	}
=======
	}
	if (!(event->data.ext.len & SNDRV_SEQ_EXT_CHAINED))
		return func(private_data, event->data.ext.ptr + offset,
			    len - offset);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	cell = (struct snd_seq_event_cell *)event->data.ext.ptr;
	for (; len > 0 && cell; cell = cell->next) {
		int size = sizeof(struct snd_seq_event);
<<<<<<< HEAD
		if (len < size)
			size = len;
		err = func(private_data, &cell->event, size);
		if (err < 0)
			return err;
=======
		char *curptr = (char *)&cell->event;

		if (offset >= size) {
			offset -= size;
			len -= size;
			continue;
		}
		if (len < size)
			size = len;
		err = func(private_data, curptr + offset, size - offset);
		if (err < 0)
			return err;
		offset = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		len -= size;
	}
	return 0;
}

<<<<<<< HEAD
=======
int snd_seq_dump_var_event(const struct snd_seq_event *event,
			   snd_seq_dump_func_t func, void *private_data)
{
	return dump_var_event(event, func, private_data, 0, 0);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
EXPORT_SYMBOL(snd_seq_dump_var_event);


/*
 * exported:
 * expand the variable length event to linear buffer space.
 */

<<<<<<< HEAD
static int seq_copy_in_kernel(char **bufptr, const void *src, int size)
{
=======
static int seq_copy_in_kernel(void *ptr, void *src, int size)
{
	char **bufptr = ptr;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	memcpy(*bufptr, src, size);
	*bufptr += size;
	return 0;
}

<<<<<<< HEAD
static int seq_copy_in_user(char __user **bufptr, const void *src, int size)
{
=======
static int seq_copy_in_user(void *ptr, void *src, int size)
{
	char __user **bufptr = ptr;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (copy_to_user(*bufptr, src, size))
		return -EFAULT;
	*bufptr += size;
	return 0;
}

<<<<<<< HEAD
int snd_seq_expand_var_event(const struct snd_seq_event *event, int count, char *buf,
			     int in_kernel, int size_aligned)
{
	int len, newlen;
	int err;

	if ((len = get_var_len(event)) < 0)
=======
static int expand_var_event(const struct snd_seq_event *event,
			    int offset, int size, char *buf, bool in_kernel)
{
	if (event->data.ext.len & SNDRV_SEQ_EXT_USRPTR) {
		if (! in_kernel)
			return -EINVAL;
		if (copy_from_user(buf,
				   (char __force __user *)event->data.ext.ptr + offset,
				   size))
			return -EFAULT;
		return 0;
	}
	return dump_var_event(event,
			     in_kernel ? seq_copy_in_kernel : seq_copy_in_user,
			     &buf, offset, size);
}

int snd_seq_expand_var_event(const struct snd_seq_event *event, int count, char *buf,
			     int in_kernel, int size_aligned)
{
	int len, newlen, err;

	len = get_var_len(event);
	if (len < 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return len;
	newlen = len;
	if (size_aligned > 0)
		newlen = roundup(len, size_aligned);
	if (count < newlen)
		return -EAGAIN;
<<<<<<< HEAD

	if (event->data.ext.len & SNDRV_SEQ_EXT_USRPTR) {
		if (! in_kernel)
			return -EINVAL;
		if (copy_from_user(buf, (void __force __user *)event->data.ext.ptr, len))
			return -EFAULT;
		return newlen;
	}
	err = snd_seq_dump_var_event(event,
				     in_kernel ? (snd_seq_dump_func_t)seq_copy_in_kernel :
				     (snd_seq_dump_func_t)seq_copy_in_user,
				     &buf);
	return err < 0 ? err : newlen;
}

EXPORT_SYMBOL(snd_seq_expand_var_event);

=======
	err = expand_var_event(event, 0, len, buf, in_kernel);
	if (err < 0)
		return err;
	if (len != newlen) {
		if (in_kernel)
			memset(buf + len, 0, newlen - len);
		else if (clear_user((__force void __user *)buf + len,
				    newlen - len))
			return -EFAULT;
	}
	return newlen;
}
EXPORT_SYMBOL(snd_seq_expand_var_event);

int snd_seq_expand_var_event_at(const struct snd_seq_event *event, int count,
				char *buf, int offset)
{
	int len, err;

	len = get_var_len(event);
	if (len < 0)
		return len;
	if (len <= offset)
		return 0;
	len -= offset;
	if (len > count)
		len = count;
	err = expand_var_event(event, offset, count, buf, true);
	if (err < 0)
		return err;
	return len;
}
EXPORT_SYMBOL_GPL(snd_seq_expand_var_event_at);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * release this cell, free extended data if available
 */

static inline void free_cell(struct snd_seq_pool *pool,
			     struct snd_seq_event_cell *cell)
{
	cell->next = pool->free;
	pool->free = cell;
	atomic_dec(&pool->counter);
}

void snd_seq_cell_free(struct snd_seq_event_cell * cell)
{
<<<<<<< HEAD
	unsigned long flags;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct snd_seq_pool *pool;

	if (snd_BUG_ON(!cell))
		return;
	pool = cell->pool;
	if (snd_BUG_ON(!pool))
		return;

<<<<<<< HEAD
	spin_lock_irqsave(&pool->lock, flags);
=======
	guard(spinlock_irqsave)(&pool->lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	free_cell(pool, cell);
	if (snd_seq_ev_is_variable(&cell->event)) {
		if (cell->event.data.ext.len & SNDRV_SEQ_EXT_CHAINED) {
			struct snd_seq_event_cell *curp, *nextptr;
			curp = cell->event.data.ext.ptr;
			for (; curp; curp = nextptr) {
				nextptr = curp->next;
				curp->next = pool->free;
				free_cell(pool, curp);
			}
		}
	}
	if (waitqueue_active(&pool->output_sleep)) {
		/* has enough space now? */
		if (snd_seq_output_ok(pool))
			wake_up(&pool->output_sleep);
	}
<<<<<<< HEAD
	spin_unlock_irqrestore(&pool->lock, flags);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}


/*
 * allocate an event cell.
 */
static int snd_seq_cell_alloc(struct snd_seq_pool *pool,
			      struct snd_seq_event_cell **cellp,
<<<<<<< HEAD
			      int nonblock, struct file *file)
=======
			      int nonblock, struct file *file,
			      struct mutex *mutexp)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct snd_seq_event_cell *cell;
	unsigned long flags;
	int err = -EAGAIN;
<<<<<<< HEAD
	wait_queue_t wait;
=======
	wait_queue_entry_t wait;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (pool == NULL)
		return -EINVAL;

	*cellp = NULL;

	init_waitqueue_entry(&wait, current);
	spin_lock_irqsave(&pool->lock, flags);
	if (pool->ptr == NULL) {	/* not initialized */
<<<<<<< HEAD
		snd_printd("seq: pool is not initialized\n");
=======
		pr_debug("ALSA: seq: pool is not initialized\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = -EINVAL;
		goto __error;
	}
	while (pool->free == NULL && ! nonblock && ! pool->closing) {

		set_current_state(TASK_INTERRUPTIBLE);
		add_wait_queue(&pool->output_sleep, &wait);
<<<<<<< HEAD
		spin_unlock_irq(&pool->lock);
		schedule();
		spin_lock_irq(&pool->lock);
=======
		spin_unlock_irqrestore(&pool->lock, flags);
		if (mutexp)
			mutex_unlock(mutexp);
		schedule();
		if (mutexp)
			mutex_lock(mutexp);
		spin_lock_irqsave(&pool->lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		remove_wait_queue(&pool->output_sleep, &wait);
		/* interrupted? */
		if (signal_pending(current)) {
			err = -ERESTARTSYS;
			goto __error;
		}
	}
	if (pool->closing) { /* closing.. */
		err = -ENOMEM;
		goto __error;
	}

	cell = pool->free;
	if (cell) {
		int used;
		pool->free = cell->next;
		atomic_inc(&pool->counter);
		used = atomic_read(&pool->counter);
		if (pool->max_used < used)
			pool->max_used = used;
		pool->event_alloc_success++;
		/* clear cell pointers */
		cell->next = NULL;
		err = 0;
	} else
		pool->event_alloc_failures++;
	*cellp = cell;

__error:
	spin_unlock_irqrestore(&pool->lock, flags);
	return err;
}


/*
 * duplicate the event to a cell.
 * if the event has external data, the data is decomposed to additional
 * cells.
 */
int snd_seq_event_dup(struct snd_seq_pool *pool, struct snd_seq_event *event,
		      struct snd_seq_event_cell **cellp, int nonblock,
<<<<<<< HEAD
		      struct file *file)
=======
		      struct file *file, struct mutex *mutexp)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int ncells, err;
	unsigned int extlen;
	struct snd_seq_event_cell *cell;
<<<<<<< HEAD
=======
	int size;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	*cellp = NULL;

	ncells = 0;
	extlen = 0;
	if (snd_seq_ev_is_variable(event)) {
		extlen = event->data.ext.len & ~SNDRV_SEQ_EXT_MASK;
<<<<<<< HEAD
		ncells = (extlen + sizeof(struct snd_seq_event) - 1) / sizeof(struct snd_seq_event);
=======
		ncells = DIV_ROUND_UP(extlen, sizeof(struct snd_seq_event));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	if (ncells >= pool->total_elements)
		return -ENOMEM;

<<<<<<< HEAD
	err = snd_seq_cell_alloc(pool, &cell, nonblock, file);
=======
	err = snd_seq_cell_alloc(pool, &cell, nonblock, file, mutexp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err < 0)
		return err;

	/* copy the event */
<<<<<<< HEAD
	cell->event = *event;
=======
	size = snd_seq_event_packet_size(event);
	memcpy(&cell->ump, event, size);
#if IS_ENABLED(CONFIG_SND_SEQ_UMP)
	if (size < sizeof(cell->event))
		cell->ump.raw.extra = 0;
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* decompose */
	if (snd_seq_ev_is_variable(event)) {
		int len = extlen;
		int is_chained = event->data.ext.len & SNDRV_SEQ_EXT_CHAINED;
		int is_usrptr = event->data.ext.len & SNDRV_SEQ_EXT_USRPTR;
		struct snd_seq_event_cell *src, *tmp, *tail;
		char *buf;

		cell->event.data.ext.len = extlen | SNDRV_SEQ_EXT_CHAINED;
		cell->event.data.ext.ptr = NULL;

		src = (struct snd_seq_event_cell *)event->data.ext.ptr;
		buf = (char *)event->data.ext.ptr;
		tail = NULL;

		while (ncells-- > 0) {
<<<<<<< HEAD
			int size = sizeof(struct snd_seq_event);
			if (len < size)
				size = len;
			err = snd_seq_cell_alloc(pool, &tmp, nonblock, file);
=======
			size = sizeof(struct snd_seq_event);
			if (len < size)
				size = len;
			err = snd_seq_cell_alloc(pool, &tmp, nonblock, file,
						 mutexp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (err < 0)
				goto __error;
			if (cell->event.data.ext.ptr == NULL)
				cell->event.data.ext.ptr = tmp;
			if (tail)
				tail->next = tmp;
			tail = tmp;
			/* copy chunk */
			if (is_chained && src) {
				tmp->event = src->event;
				src = src->next;
			} else if (is_usrptr) {
				if (copy_from_user(&tmp->event, (char __force __user *)buf, size)) {
					err = -EFAULT;
					goto __error;
				}
			} else {
				memcpy(&tmp->event, buf, size);
			}
			buf += size;
			len -= size;
		}
	}

	*cellp = cell;
	return 0;

__error:
	snd_seq_cell_free(cell);
	return err;
}
  

/* poll wait */
int snd_seq_pool_poll_wait(struct snd_seq_pool *pool, struct file *file,
			   poll_table *wait)
{
	poll_wait(file, &pool->output_sleep, wait);
	return snd_seq_output_ok(pool);
}


/* allocate room specified number of events */
int snd_seq_pool_init(struct snd_seq_pool *pool)
{
	int cell;
	struct snd_seq_event_cell *cellptr;
<<<<<<< HEAD
	unsigned long flags;

	if (snd_BUG_ON(!pool))
		return -EINVAL;
	if (pool->ptr)			/* should be atomic? */
		return 0;

	pool->ptr = vmalloc(sizeof(struct snd_seq_event_cell) * pool->size);
	if (pool->ptr == NULL) {
		snd_printd("seq: malloc for sequencer events failed\n");
		return -ENOMEM;
	}

	/* add new cells to the free cell list */
	spin_lock_irqsave(&pool->lock, flags);
=======

	if (snd_BUG_ON(!pool))
		return -EINVAL;

	cellptr = kvmalloc_array(pool->size,
				 sizeof(struct snd_seq_event_cell),
				 GFP_KERNEL);
	if (!cellptr)
		return -ENOMEM;

	/* add new cells to the free cell list */
	guard(spinlock_irq)(&pool->lock);
	if (pool->ptr) {
		kvfree(cellptr);
		return 0;
	}

	pool->ptr = cellptr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	pool->free = NULL;

	for (cell = 0; cell < pool->size; cell++) {
		cellptr = pool->ptr + cell;
		cellptr->pool = pool;
		cellptr->next = pool->free;
		pool->free = cellptr;
	}
	pool->room = (pool->size + 1) / 2;

	/* init statistics */
	pool->max_used = 0;
	pool->total_elements = pool->size;
<<<<<<< HEAD
	spin_unlock_irqrestore(&pool->lock, flags);
	return 0;
}

/* remove events */
int snd_seq_pool_done(struct snd_seq_pool *pool)
{
	unsigned long flags;
	struct snd_seq_event_cell *ptr;
	int max_count = 5 * HZ;
=======
	return 0;
}

/* refuse the further insertion to the pool */
void snd_seq_pool_mark_closing(struct snd_seq_pool *pool)
{
	if (snd_BUG_ON(!pool))
		return;
	guard(spinlock_irqsave)(&pool->lock);
	pool->closing = 1;
}

/* remove events */
int snd_seq_pool_done(struct snd_seq_pool *pool)
{
	struct snd_seq_event_cell *ptr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (snd_BUG_ON(!pool))
		return -EINVAL;

	/* wait for closing all threads */
<<<<<<< HEAD
	spin_lock_irqsave(&pool->lock, flags);
	pool->closing = 1;
	spin_unlock_irqrestore(&pool->lock, flags);

	if (waitqueue_active(&pool->output_sleep))
		wake_up(&pool->output_sleep);

	while (atomic_read(&pool->counter) > 0) {
		if (max_count == 0) {
			snd_printk(KERN_WARNING "snd_seq_pool_done timeout: %d cells remain\n", atomic_read(&pool->counter));
			break;
		}
		schedule_timeout_uninterruptible(1);
		max_count--;
	}
	
	/* release all resources */
	spin_lock_irqsave(&pool->lock, flags);
	ptr = pool->ptr;
	pool->ptr = NULL;
	pool->free = NULL;
	pool->total_elements = 0;
	spin_unlock_irqrestore(&pool->lock, flags);

	vfree(ptr);

	spin_lock_irqsave(&pool->lock, flags);
	pool->closing = 0;
	spin_unlock_irqrestore(&pool->lock, flags);
=======
	if (waitqueue_active(&pool->output_sleep))
		wake_up(&pool->output_sleep);

	while (atomic_read(&pool->counter) > 0)
		schedule_timeout_uninterruptible(1);
	
	/* release all resources */
	scoped_guard(spinlock_irq, &pool->lock) {
		ptr = pool->ptr;
		pool->ptr = NULL;
		pool->free = NULL;
		pool->total_elements = 0;
	}

	kvfree(ptr);

	guard(spinlock_irq)(&pool->lock);
	pool->closing = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}


/* init new memory pool */
struct snd_seq_pool *snd_seq_pool_new(int poolsize)
{
	struct snd_seq_pool *pool;

	/* create pool block */
	pool = kzalloc(sizeof(*pool), GFP_KERNEL);
<<<<<<< HEAD
	if (pool == NULL) {
		snd_printd("seq: malloc failed for pool\n");
		return NULL;
	}
=======
	if (!pool)
		return NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_lock_init(&pool->lock);
	pool->ptr = NULL;
	pool->free = NULL;
	pool->total_elements = 0;
	atomic_set(&pool->counter, 0);
	pool->closing = 0;
	init_waitqueue_head(&pool->output_sleep);
	
	pool->size = poolsize;

	/* init statistics */
	pool->max_used = 0;
	return pool;
}

/* remove memory pool */
int snd_seq_pool_delete(struct snd_seq_pool **ppool)
{
	struct snd_seq_pool *pool = *ppool;

	*ppool = NULL;
	if (pool == NULL)
		return 0;
<<<<<<< HEAD
=======
	snd_seq_pool_mark_closing(pool);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	snd_seq_pool_done(pool);
	kfree(pool);
	return 0;
}

<<<<<<< HEAD
/* initialize sequencer memory */
int __init snd_sequencer_memory_init(void)
{
	return 0;
}

/* release sequencer memory */
void __exit snd_sequencer_memory_done(void)
{
}


=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* exported to seq_clientmgr.c */
void snd_seq_info_pool(struct snd_info_buffer *buffer,
		       struct snd_seq_pool *pool, char *space)
{
	if (pool == NULL)
		return;
	snd_iprintf(buffer, "%sPool size          : %d\n", space, pool->total_elements);
	snd_iprintf(buffer, "%sCells in use       : %d\n", space, atomic_read(&pool->counter));
	snd_iprintf(buffer, "%sPeak cells in use  : %d\n", space, pool->max_used);
	snd_iprintf(buffer, "%sAlloc success      : %d\n", space, pool->event_alloc_success);
	snd_iprintf(buffer, "%sAlloc failures     : %d\n", space, pool->event_alloc_failures);
}
