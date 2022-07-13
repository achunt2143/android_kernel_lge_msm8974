<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Tty buffer allocation management
 */

#include <linux/types.h>
#include <linux/errno.h>
<<<<<<< HEAD
#include <linux/tty.h>
=======
#include <linux/minmax.h>
#include <linux/tty.h>
#include <linux/tty_buffer.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/tty_driver.h>
#include <linux/tty_flip.h>
#include <linux/timer.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/sched.h>
<<<<<<< HEAD
#include <linux/init.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/wait.h>
#include <linux/bitops.h>
#include <linux/delay.h>
#include <linux/module.h>
<<<<<<< HEAD

/**
 *	tty_buffer_free_all		-	free buffers used by a tty
 *	@tty: tty to free from
 *
 *	Remove all the buffers pending on a tty whether queued with data
 *	or in the free ring. Must be called when the tty is no longer in use
 *
 *	Locking: none
 */

void tty_buffer_free_all(struct tty_struct *tty)
{
	struct tty_buffer *thead;
	while ((thead = tty->buf.head) != NULL) {
		tty->buf.head = thead->next;
		kfree(thead);
	}
	while ((thead = tty->buf.free) != NULL) {
		tty->buf.free = thead->next;
		kfree(thead);
	}
	tty->buf.tail = NULL;
	tty->buf.memory_used = 0;
}

/**
 *	tty_buffer_alloc	-	allocate a tty buffer
 *	@tty: tty device
 *	@size: desired size (characters)
 *
 *	Allocate a new tty buffer to hold the desired number of characters.
 *	Return NULL if out of memory or the allocation would exceed the
 *	per device queue
 *
 *	Locking: Caller must hold tty->buf.lock
 */

static struct tty_buffer *tty_buffer_alloc(struct tty_struct *tty, size_t size)
{
	struct tty_buffer *p;

	if (tty->buf.memory_used + size > 65536)
		return NULL;
	p = kmalloc(sizeof(struct tty_buffer) + 2 * size, GFP_ATOMIC);
	if (p == NULL)
		return NULL;
=======
#include <linux/ratelimit.h>
#include "tty.h"

#define MIN_TTYB_SIZE	256
#define TTYB_ALIGN_MASK	0xff

/*
 * Byte threshold to limit memory consumption for flip buffers.
 * The actual memory limit is > 2x this amount.
 */
#define TTYB_DEFAULT_MEM_LIMIT	(640 * 1024UL)

/*
 * We default to dicing tty buffer allocations to this many characters
 * in order to avoid multiple page allocations. We know the size of
 * tty_buffer itself but it must also be taken into account that the
 * buffer is 256 byte aligned. See tty_buffer_find for the allocation
 * logic this must match.
 */

#define TTY_BUFFER_PAGE	(((PAGE_SIZE - sizeof(struct tty_buffer)) / 2) & ~TTYB_ALIGN_MASK)

/**
 * tty_buffer_lock_exclusive	-	gain exclusive access to buffer
 * @port: tty port owning the flip buffer
 *
 * Guarantees safe use of the &tty_ldisc_ops.receive_buf() method by excluding
 * the buffer work and any pending flush from using the flip buffer. Data can
 * continue to be added concurrently to the flip buffer from the driver side.
 *
 * See also tty_buffer_unlock_exclusive().
 */
void tty_buffer_lock_exclusive(struct tty_port *port)
{
	struct tty_bufhead *buf = &port->buf;

	atomic_inc(&buf->priority);
	mutex_lock(&buf->lock);
}
EXPORT_SYMBOL_GPL(tty_buffer_lock_exclusive);

/**
 * tty_buffer_unlock_exclusive	-	release exclusive access
 * @port: tty port owning the flip buffer
 *
 * The buffer work is restarted if there is data in the flip buffer.
 *
 * See also tty_buffer_lock_exclusive().
 */
void tty_buffer_unlock_exclusive(struct tty_port *port)
{
	struct tty_bufhead *buf = &port->buf;
	bool restart = buf->head->commit != buf->head->read;

	atomic_dec(&buf->priority);
	mutex_unlock(&buf->lock);

	if (restart)
		queue_work(system_unbound_wq, &buf->work);
}
EXPORT_SYMBOL_GPL(tty_buffer_unlock_exclusive);

/**
 * tty_buffer_space_avail	-	return unused buffer space
 * @port: tty port owning the flip buffer
 *
 * Returns: the # of bytes which can be written by the driver without reaching
 * the buffer limit.
 *
 * Note: this does not guarantee that memory is available to write the returned
 * # of bytes (use tty_prepare_flip_string() to pre-allocate if memory
 * guarantee is required).
 */
unsigned int tty_buffer_space_avail(struct tty_port *port)
{
	int space = port->buf.mem_limit - atomic_read(&port->buf.mem_used);

	return max(space, 0);
}
EXPORT_SYMBOL_GPL(tty_buffer_space_avail);

static void tty_buffer_reset(struct tty_buffer *p, size_t size)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	p->used = 0;
	p->size = size;
	p->next = NULL;
	p->commit = 0;
<<<<<<< HEAD
	p->read = 0;
	p->char_buf_ptr = (char *)(p->data);
	p->flag_buf_ptr = (unsigned char *)p->char_buf_ptr + size;
	tty->buf.memory_used += size;
=======
	p->lookahead = 0;
	p->read = 0;
	p->flags = true;
}

/**
 * tty_buffer_free_all		-	free buffers used by a tty
 * @port: tty port to free from
 *
 * Remove all the buffers pending on a tty whether queued with data or in the
 * free ring. Must be called when the tty is no longer in use.
 */
void tty_buffer_free_all(struct tty_port *port)
{
	struct tty_bufhead *buf = &port->buf;
	struct tty_buffer *p, *next;
	struct llist_node *llist;
	unsigned int freed = 0;
	int still_used;

	while ((p = buf->head) != NULL) {
		buf->head = p->next;
		freed += p->size;
		if (p->size > 0)
			kfree(p);
	}
	llist = llist_del_all(&buf->free);
	llist_for_each_entry_safe(p, next, llist, free)
		kfree(p);

	tty_buffer_reset(&buf->sentinel, 0);
	buf->head = &buf->sentinel;
	buf->tail = &buf->sentinel;

	still_used = atomic_xchg(&buf->mem_used, 0);
	WARN(still_used != freed, "we still have not freed %d bytes!",
			still_used - freed);
}

/**
 * tty_buffer_alloc	-	allocate a tty buffer
 * @port: tty port
 * @size: desired size (characters)
 *
 * Allocate a new tty buffer to hold the desired number of characters. We
 * round our buffers off in 256 character chunks to get better allocation
 * behaviour.
 *
 * Returns: %NULL if out of memory or the allocation would exceed the per
 * device queue.
 */
static struct tty_buffer *tty_buffer_alloc(struct tty_port *port, size_t size)
{
	struct llist_node *free;
	struct tty_buffer *p;

	/* Round the buffer size out */
	size = __ALIGN_MASK(size, TTYB_ALIGN_MASK);

	if (size <= MIN_TTYB_SIZE) {
		free = llist_del_first(&port->buf.free);
		if (free) {
			p = llist_entry(free, struct tty_buffer, free);
			goto found;
		}
	}

	/* Should possibly check if this fails for the largest buffer we
	 * have queued and recycle that ?
	 */
	if (atomic_read(&port->buf.mem_used) > port->buf.mem_limit)
		return NULL;
	p = kmalloc(struct_size(p, data, 2 * size), GFP_ATOMIC | __GFP_NOWARN);
	if (p == NULL)
		return NULL;

found:
	tty_buffer_reset(p, size);
	atomic_add(size, &port->buf.mem_used);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return p;
}

/**
<<<<<<< HEAD
 *	tty_buffer_free		-	free a tty buffer
 *	@tty: tty owning the buffer
 *	@b: the buffer to free
 *
 *	Free a tty buffer, or add it to the free list according to our
 *	internal strategy
 *
 *	Locking: Caller must hold tty->buf.lock
 */

static void tty_buffer_free(struct tty_struct *tty, struct tty_buffer *b)
{
	/* Dumb strategy for now - should keep some stats */
	tty->buf.memory_used -= b->size;
	WARN_ON(tty->buf.memory_used < 0);

	if (b->size >= 512)
		kfree(b);
	else {
		b->next = tty->buf.free;
		tty->buf.free = b;
	}
}

/**
 *	__tty_buffer_flush		-	flush full tty buffers
 *	@tty: tty to flush
 *
 *	flush all the buffers containing receive data. Caller must
 *	hold the buffer lock and must have ensured no parallel flush to
 *	ldisc is running.
 *
 *	Locking: Caller must hold tty->buf.lock
 */

static void __tty_buffer_flush(struct tty_struct *tty)
{
	struct tty_buffer *thead;

	if (tty->buf.head == NULL)
		return;
	while ((thead = tty->buf.head->next) != NULL) {
		tty_buffer_free(tty, tty->buf.head);
		tty->buf.head = thead;
	}
	WARN_ON(tty->buf.head != tty->buf.tail);
	tty->buf.head->read = tty->buf.head->commit;
}

/**
 *	tty_buffer_flush		-	flush full tty buffers
 *	@tty: tty to flush
 *
 *	flush all the buffers containing receive data. If the buffer is
 *	being processed by flush_to_ldisc then we defer the processing
 *	to that function
 *
 *	Locking: none
 */

void tty_buffer_flush(struct tty_struct *tty)
{
	unsigned long flags;
	spin_lock_irqsave(&tty->buf.lock, flags);

	/* If the data is being pushed to the tty layer then we can't
	   process it here. Instead set a flag and the flush_to_ldisc
	   path will process the flush request before it exits */
	if (test_bit(TTY_FLUSHING, &tty->flags)) {
		set_bit(TTY_FLUSHPENDING, &tty->flags);
		spin_unlock_irqrestore(&tty->buf.lock, flags);
		wait_event(tty->read_wait,
				test_bit(TTY_FLUSHPENDING, &tty->flags) == 0);
		return;
	} else
		__tty_buffer_flush(tty);
	spin_unlock_irqrestore(&tty->buf.lock, flags);
}

/**
 *	tty_buffer_find		-	find a free tty buffer
 *	@tty: tty owning the buffer
 *	@size: characters wanted
 *
 *	Locate an existing suitable tty buffer or if we are lacking one then
 *	allocate a new one. We round our buffers off in 256 character chunks
 *	to get better allocation behaviour.
 *
 *	Locking: Caller must hold tty->buf.lock
 */

static struct tty_buffer *tty_buffer_find(struct tty_struct *tty, size_t size)
{
	struct tty_buffer **tbh = &tty->buf.free;
	while ((*tbh) != NULL) {
		struct tty_buffer *t = *tbh;
		if (t->size >= size) {
			*tbh = t->next;
			t->next = NULL;
			t->used = 0;
			t->commit = 0;
			t->read = 0;
			tty->buf.memory_used += t->size;
			return t;
		}
		tbh = &((*tbh)->next);
	}
	/* Round the buffer size out */
	size = (size + 0xFF) & ~0xFF;
	return tty_buffer_alloc(tty, size);
	/* Should possibly check if this fails for the largest buffer we
	   have queued and recycle that ? */
}
/**
 *	__tty_buffer_request_room		-	grow tty buffer if needed
 *	@tty: tty structure
 *	@size: size desired
 *
 *	Make at least size bytes of linear space available for the tty
 *	buffer. If we fail return the size we managed to find.
 *      Locking: Caller must hold tty->buf.lock
 */
static int __tty_buffer_request_room(struct tty_struct *tty, size_t size)
{
	struct tty_buffer *b, *n;
	int left;
	/* OPTIMISATION: We could keep a per tty "zero" sized buffer to
	   remove this conditional if its worth it. This would be invisible
	   to the callers */
	if ((b = tty->buf.tail) != NULL)
		left = b->size - b->used;
	else
		left = 0;

	if (left < size) {
		/* This is the slow path - looking for new buffers to use */
		if ((n = tty_buffer_find(tty, size)) != NULL) {
			if (b != NULL) {
				b->next = n;
				b->commit = b->used;
			} else
				tty->buf.head = n;
			tty->buf.tail = n;
		} else
			size = left;
	}
=======
 * tty_buffer_free		-	free a tty buffer
 * @port: tty port owning the buffer
 * @b: the buffer to free
 *
 * Free a tty buffer, or add it to the free list according to our internal
 * strategy.
 */
static void tty_buffer_free(struct tty_port *port, struct tty_buffer *b)
{
	struct tty_bufhead *buf = &port->buf;

	/* Dumb strategy for now - should keep some stats */
	WARN_ON(atomic_sub_return(b->size, &buf->mem_used) < 0);

	if (b->size > MIN_TTYB_SIZE)
		kfree(b);
	else if (b->size > 0)
		llist_add(&b->free, &buf->free);
}

/**
 * tty_buffer_flush		-	flush full tty buffers
 * @tty: tty to flush
 * @ld: optional ldisc ptr (must be referenced)
 *
 * Flush all the buffers containing receive data. If @ld != %NULL, flush the
 * ldisc input buffer.
 *
 * Locking: takes buffer lock to ensure single-threaded flip buffer 'consumer'.
 */
void tty_buffer_flush(struct tty_struct *tty, struct tty_ldisc *ld)
{
	struct tty_port *port = tty->port;
	struct tty_bufhead *buf = &port->buf;
	struct tty_buffer *next;

	atomic_inc(&buf->priority);

	mutex_lock(&buf->lock);
	/* paired w/ release in __tty_buffer_request_room; ensures there are
	 * no pending memory accesses to the freed buffer
	 */
	while ((next = smp_load_acquire(&buf->head->next)) != NULL) {
		tty_buffer_free(port, buf->head);
		buf->head = next;
	}
	buf->head->read = buf->head->commit;
	buf->head->lookahead = buf->head->read;

	if (ld && ld->ops->flush_buffer)
		ld->ops->flush_buffer(tty);

	atomic_dec(&buf->priority);
	mutex_unlock(&buf->lock);
}

/**
 * __tty_buffer_request_room	-	grow tty buffer if needed
 * @port: tty port
 * @size: size desired
 * @flags: buffer has to store flags along character data
 *
 * Make at least @size bytes of linear space available for the tty buffer.
 *
 * Will change over to a new buffer if the current buffer is encoded as
 * %TTY_NORMAL (so has no flags buffer) and the new buffer requires a flags
 * buffer.
 *
 * Returns: the size we managed to find.
 */
static int __tty_buffer_request_room(struct tty_port *port, size_t size,
				     bool flags)
{
	struct tty_bufhead *buf = &port->buf;
	struct tty_buffer *n, *b = buf->tail;
	size_t left = (b->flags ? 1 : 2) * b->size - b->used;
	bool change = !b->flags && flags;

	if (!change && left >= size)
		return size;

	/* This is the slow path - looking for new buffers to use */
	n = tty_buffer_alloc(port, size);
	if (n == NULL)
		return change ? 0 : left;

	n->flags = flags;
	buf->tail = n;
	/*
	 * Paired w/ acquire in flush_to_ldisc() and lookahead_bufs()
	 * ensures they see all buffer data.
	 */
	smp_store_release(&b->commit, b->used);
	/*
	 * Paired w/ acquire in flush_to_ldisc() and lookahead_bufs()
	 * ensures the latest commit value can be read before the head
	 * is advanced to the next buffer.
	 */
	smp_store_release(&b->next, n);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return size;
}

<<<<<<< HEAD

/**
 *	tty_buffer_request_room		-	grow tty buffer if needed
 *	@tty: tty structure
 *	@size: size desired
 *
 *	Make at least size bytes of linear space available for the tty
 *	buffer. If we fail return the size we managed to find.
 *
 *	Locking: Takes tty->buf.lock
 */
int tty_buffer_request_room(struct tty_struct *tty, size_t size)
{
	unsigned long flags;
	int length;

	spin_lock_irqsave(&tty->buf.lock, flags);
	length = __tty_buffer_request_room(tty, size);
	spin_unlock_irqrestore(&tty->buf.lock, flags);
	return length;
}
EXPORT_SYMBOL_GPL(tty_buffer_request_room);

/**
 *	tty_insert_flip_string_fixed_flag - Add characters to the tty buffer
 *	@tty: tty structure
 *	@chars: characters
 *	@flag: flag value for each character
 *	@size: size
 *
 *	Queue a series of bytes to the tty buffering. All the characters
 *	passed are marked with the supplied flag. Returns the number added.
 *
 *	Locking: Called functions may take tty->buf.lock
 */

int tty_insert_flip_string_fixed_flag(struct tty_struct *tty,
		const unsigned char *chars, char flag, size_t size)
{
	int copied = 0;
	do {
		int goal = min_t(size_t, size - copied, TTY_BUFFER_PAGE);
		int space;
		unsigned long flags;
		struct tty_buffer *tb;

		spin_lock_irqsave(&tty->buf.lock, flags);
		space = __tty_buffer_request_room(tty, goal);
		tb = tty->buf.tail;
		/* If there is no space then tb may be NULL */
		if (unlikely(space == 0)) {
			spin_unlock_irqrestore(&tty->buf.lock, flags);
			break;
		}
		memcpy(tb->char_buf_ptr + tb->used, chars, space);
		memset(tb->flag_buf_ptr + tb->used, flag, space);
		tb->used += space;
		spin_unlock_irqrestore(&tty->buf.lock, flags);
		copied += space;
		chars += space;
		/* There is a small chance that we need to split the data over
		   several buffers. If this is the case we must loop */
	} while (unlikely(size > copied));
	return copied;
}
EXPORT_SYMBOL(tty_insert_flip_string_fixed_flag);

/**
 *	tty_insert_flip_string_flags	-	Add characters to the tty buffer
 *	@tty: tty structure
 *	@chars: characters
 *	@flags: flag bytes
 *	@size: size
 *
 *	Queue a series of bytes to the tty buffering. For each character
 *	the flags array indicates the status of the character. Returns the
 *	number added.
 *
 *	Locking: Called functions may take tty->buf.lock
 */

int tty_insert_flip_string_flags(struct tty_struct *tty,
		const unsigned char *chars, const char *flags, size_t size)
{
	int copied = 0;
	do {
		int goal = min_t(size_t, size - copied, TTY_BUFFER_PAGE);
		int space;
		unsigned long __flags;
		struct tty_buffer *tb;

		spin_lock_irqsave(&tty->buf.lock, __flags);
		space = __tty_buffer_request_room(tty, goal);
		tb = tty->buf.tail;
		/* If there is no space then tb may be NULL */
		if (unlikely(space == 0)) {
			spin_unlock_irqrestore(&tty->buf.lock, __flags);
			break;
		}
		memcpy(tb->char_buf_ptr + tb->used, chars, space);
		memcpy(tb->flag_buf_ptr + tb->used, flags, space);
		tb->used += space;
		spin_unlock_irqrestore(&tty->buf.lock, __flags);
		copied += space;
		chars += space;
		flags += space;
		/* There is a small chance that we need to split the data over
		   several buffers. If this is the case we must loop */
	} while (unlikely(size > copied));
	return copied;
}
EXPORT_SYMBOL(tty_insert_flip_string_flags);

/**
 *	tty_schedule_flip	-	push characters to ldisc
 *	@tty: tty to push from
 *
 *	Takes any pending buffers and transfers their ownership to the
 *	ldisc side of the queue. It then schedules those characters for
 *	processing by the line discipline.
 *
 *	Locking: Takes tty->buf.lock
 */

void tty_schedule_flip(struct tty_struct *tty)
{
	unsigned long flags;
	spin_lock_irqsave(&tty->buf.lock, flags);
	if (tty->buf.tail != NULL)
		tty->buf.tail->commit = tty->buf.tail->used;
	spin_unlock_irqrestore(&tty->buf.lock, flags);
	schedule_work(&tty->buf.work);
}
EXPORT_SYMBOL(tty_schedule_flip);

/**
 *	tty_prepare_flip_string		-	make room for characters
 *	@tty: tty
 *	@chars: return pointer for character write area
 *	@size: desired size
 *
 *	Prepare a block of space in the buffer for data. Returns the length
 *	available and buffer pointer to the space which is now allocated and
 *	accounted for as ready for normal characters. This is used for drivers
 *	that need their own block copy routines into the buffer. There is no
 *	guarantee the buffer is a DMA target!
 *
 *	Locking: May call functions taking tty->buf.lock
 */

int tty_prepare_flip_string(struct tty_struct *tty, unsigned char **chars,
								size_t size)
{
	int space;
	unsigned long flags;
	struct tty_buffer *tb;

	spin_lock_irqsave(&tty->buf.lock, flags);
	space = __tty_buffer_request_room(tty, size);

	tb = tty->buf.tail;
	if (likely(space)) {
		*chars = tb->char_buf_ptr + tb->used;
		memset(tb->flag_buf_ptr + tb->used, TTY_NORMAL, space);
		tb->used += space;
	}
	spin_unlock_irqrestore(&tty->buf.lock, flags);
=======
int tty_buffer_request_room(struct tty_port *port, size_t size)
{
	return __tty_buffer_request_room(port, size, true);
}
EXPORT_SYMBOL_GPL(tty_buffer_request_room);

size_t __tty_insert_flip_string_flags(struct tty_port *port, const u8 *chars,
				      const u8 *flags, bool mutable_flags,
				      size_t size)
{
	bool need_flags = mutable_flags || flags[0] != TTY_NORMAL;
	size_t copied = 0;

	do {
		size_t goal = min_t(size_t, size - copied, TTY_BUFFER_PAGE);
		size_t space = __tty_buffer_request_room(port, goal, need_flags);
		struct tty_buffer *tb = port->buf.tail;

		if (unlikely(space == 0))
			break;

		memcpy(char_buf_ptr(tb, tb->used), chars, space);

		if (mutable_flags) {
			memcpy(flag_buf_ptr(tb, tb->used), flags, space);
			flags += space;
		} else if (tb->flags) {
			memset(flag_buf_ptr(tb, tb->used), flags[0], space);
		} else {
			/* tb->flags should be available once requested */
			WARN_ON_ONCE(need_flags);
		}

		tb->used += space;
		copied += space;
		chars += space;

		/* There is a small chance that we need to split the data over
		 * several buffers. If this is the case we must loop.
		 */
	} while (unlikely(size > copied));

	return copied;
}
EXPORT_SYMBOL(__tty_insert_flip_string_flags);

/**
 * tty_prepare_flip_string	-	make room for characters
 * @port: tty port
 * @chars: return pointer for character write area
 * @size: desired size
 *
 * Prepare a block of space in the buffer for data.
 *
 * This is used for drivers that need their own block copy routines into the
 * buffer. There is no guarantee the buffer is a DMA target!
 *
 * Returns: the length available and buffer pointer (@chars) to the space which
 * is now allocated and accounted for as ready for normal characters.
 */
size_t tty_prepare_flip_string(struct tty_port *port, u8 **chars, size_t size)
{
	size_t space = __tty_buffer_request_room(port, size, false);

	if (likely(space)) {
		struct tty_buffer *tb = port->buf.tail;

		*chars = char_buf_ptr(tb, tb->used);
		if (tb->flags)
			memset(flag_buf_ptr(tb, tb->used), TTY_NORMAL, space);
		tb->used += space;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return space;
}
EXPORT_SYMBOL_GPL(tty_prepare_flip_string);

/**
<<<<<<< HEAD
 *	tty_prepare_flip_string_flags	-	make room for characters
 *	@tty: tty
 *	@chars: return pointer for character write area
 *	@flags: return pointer for status flag write area
 *	@size: desired size
 *
 *	Prepare a block of space in the buffer for data. Returns the length
 *	available and buffer pointer to the space which is now allocated and
 *	accounted for as ready for characters. This is used for drivers
 *	that need their own block copy routines into the buffer. There is no
 *	guarantee the buffer is a DMA target!
 *
 *	Locking: May call functions taking tty->buf.lock
 */

int tty_prepare_flip_string_flags(struct tty_struct *tty,
			unsigned char **chars, char **flags, size_t size)
{
	int space;
	unsigned long __flags;
	struct tty_buffer *tb;

	spin_lock_irqsave(&tty->buf.lock, __flags);
	space = __tty_buffer_request_room(tty, size);

	tb = tty->buf.tail;
	if (likely(space)) {
		*chars = tb->char_buf_ptr + tb->used;
		*flags = tb->flag_buf_ptr + tb->used;
		tb->used += space;
	}
	spin_unlock_irqrestore(&tty->buf.lock, __flags);
	return space;
}
EXPORT_SYMBOL_GPL(tty_prepare_flip_string_flags);



/**
 *	flush_to_ldisc
 *	@work: tty structure passed from work queue.
 *
 *	This routine is called out of the software interrupt to flush data
 *	from the buffer chain to the line discipline.
 *
 *	Locking: holds tty->buf.lock to guard buffer list. Drops the lock
 *	while invoking the line discipline receive_buf method. The
 *	receive_buf method is single threaded for each tty instance.
 */

static void flush_to_ldisc(struct work_struct *work)
{
	struct tty_struct *tty =
		container_of(work, struct tty_struct, buf.work);
	unsigned long 	flags;
	struct tty_ldisc *disc;

	disc = tty_ldisc_ref(tty);
	if (disc == NULL)	/*  !TTY_LDISC */
		return;

	spin_lock_irqsave(&tty->buf.lock, flags);

	if (!test_and_set_bit(TTY_FLUSHING, &tty->flags)) {
		struct tty_buffer *head;
		while ((head = tty->buf.head) != NULL) {
			int count;
			char *char_buf;
			unsigned char *flag_buf;

			count = head->commit - head->read;
			if (!count) {
				if (head->next == NULL)
					break;
				tty->buf.head = head->next;
				tty_buffer_free(tty, head);
				continue;
			}

			if (!tty->receive_room)
				break;
			if (count > tty->receive_room)
				count = tty->receive_room;
			char_buf = head->char_buf_ptr + head->read;
			flag_buf = head->flag_buf_ptr + head->read;
			head->read += count;
			spin_unlock_irqrestore(&tty->buf.lock, flags);
			if (disc->ops->receive_buf)
				disc->ops->receive_buf(tty, char_buf,
							flag_buf, count);
			spin_lock_irqsave(&tty->buf.lock, flags);
			/* Ldisc or user is trying to flush the buffers.
			   We may have a deferred request to flush the
			   input buffer, if so pull the chain under the lock
			   and empty the queue */
			if (test_bit(TTY_FLUSHPENDING, &tty->flags)) {
				__tty_buffer_flush(tty);
				clear_bit(TTY_FLUSHPENDING, &tty->flags);
				wake_up(&tty->read_wait);
				break;
			}
		}
		clear_bit(TTY_FLUSHING, &tty->flags);
	}
	spin_unlock_irqrestore(&tty->buf.lock, flags);

	tty_ldisc_deref(disc);
}

/**
 *	tty_flush_to_ldisc
 *	@tty: tty to push
 *
 *	Push the terminal flip buffers to the line discipline.
 *
 *	Must not be called from IRQ context.
 */
void tty_flush_to_ldisc(struct tty_struct *tty)
{
	flush_work(&tty->buf.work);
}

/**
 *	tty_flip_buffer_push	-	terminal
 *	@tty: tty to push
 *
 *	Queue a push of the terminal flip buffers to the line discipline. This
 *	function must not be called from IRQ context if tty->low_latency is set.
 *
 *	In the event of the queue being busy for flipping the work will be
 *	held off and retried later.
 *
 *	Locking: tty buffer lock. Driver locks in low latency mode.
 */

void tty_flip_buffer_push(struct tty_struct *tty)
{
	unsigned long flags;
	spin_lock_irqsave(&tty->buf.lock, flags);
	if (tty->buf.tail != NULL)
		tty->buf.tail->commit = tty->buf.tail->used;
	spin_unlock_irqrestore(&tty->buf.lock, flags);

	if (tty->low_latency)
		flush_to_ldisc(&tty->buf.work);
	else
		schedule_work(&tty->buf.work);
=======
 * tty_ldisc_receive_buf	-	forward data to line discipline
 * @ld: line discipline to process input
 * @p: char buffer
 * @f: %TTY_NORMAL, %TTY_BREAK, etc. flags buffer
 * @count: number of bytes to process
 *
 * Callers other than flush_to_ldisc() need to exclude the kworker from
 * concurrent use of the line discipline, see paste_selection().
 *
 * Returns: the number of bytes processed.
 */
size_t tty_ldisc_receive_buf(struct tty_ldisc *ld, const u8 *p, const u8 *f,
			     size_t count)
{
	if (ld->ops->receive_buf2)
		count = ld->ops->receive_buf2(ld->tty, p, f, count);
	else {
		count = min_t(size_t, count, ld->tty->receive_room);
		if (count && ld->ops->receive_buf)
			ld->ops->receive_buf(ld->tty, p, f, count);
	}
	return count;
}
EXPORT_SYMBOL_GPL(tty_ldisc_receive_buf);

static void lookahead_bufs(struct tty_port *port, struct tty_buffer *head)
{
	head->lookahead = max(head->lookahead, head->read);

	while (head) {
		struct tty_buffer *next;
		unsigned int count;

		/*
		 * Paired w/ release in __tty_buffer_request_room();
		 * ensures commit value read is not stale if the head
		 * is advancing to the next buffer.
		 */
		next = smp_load_acquire(&head->next);
		/*
		 * Paired w/ release in __tty_buffer_request_room() or in
		 * tty_buffer_flush(); ensures we see the committed buffer data.
		 */
		count = smp_load_acquire(&head->commit) - head->lookahead;
		if (!count) {
			head = next;
			continue;
		}

		if (port->client_ops->lookahead_buf) {
			u8 *p, *f = NULL;

			p = char_buf_ptr(head, head->lookahead);
			if (head->flags)
				f = flag_buf_ptr(head, head->lookahead);

			port->client_ops->lookahead_buf(port, p, f, count);
		}

		head->lookahead += count;
	}
}

static size_t
receive_buf(struct tty_port *port, struct tty_buffer *head, size_t count)
{
	u8 *p = char_buf_ptr(head, head->read);
	const u8 *f = NULL;
	size_t n;

	if (head->flags)
		f = flag_buf_ptr(head, head->read);

	n = port->client_ops->receive_buf(port, p, f, count);
	if (n > 0)
		memset(p, 0, n);
	return n;
}

/**
 * flush_to_ldisc		-	flush data from buffer to ldisc
 * @work: tty structure passed from work queue.
 *
 * This routine is called out of the software interrupt to flush data from the
 * buffer chain to the line discipline.
 *
 * The receive_buf() method is single threaded for each tty instance.
 *
 * Locking: takes buffer lock to ensure single-threaded flip buffer 'consumer'.
 */
static void flush_to_ldisc(struct work_struct *work)
{
	struct tty_port *port = container_of(work, struct tty_port, buf.work);
	struct tty_bufhead *buf = &port->buf;

	mutex_lock(&buf->lock);

	while (1) {
		struct tty_buffer *head = buf->head;
		struct tty_buffer *next;
		size_t count, rcvd;

		/* Ldisc or user is trying to gain exclusive access */
		if (atomic_read(&buf->priority))
			break;

		/* paired w/ release in __tty_buffer_request_room();
		 * ensures commit value read is not stale if the head
		 * is advancing to the next buffer
		 */
		next = smp_load_acquire(&head->next);
		/* paired w/ release in __tty_buffer_request_room() or in
		 * tty_buffer_flush(); ensures we see the committed buffer data
		 */
		count = smp_load_acquire(&head->commit) - head->read;
		if (!count) {
			if (next == NULL)
				break;
			buf->head = next;
			tty_buffer_free(port, head);
			continue;
		}

		rcvd = receive_buf(port, head, count);
		head->read += rcvd;
		if (rcvd < count)
			lookahead_bufs(port, head);
		if (!rcvd)
			break;

		if (need_resched())
			cond_resched();
	}

	mutex_unlock(&buf->lock);

}

static inline void tty_flip_buffer_commit(struct tty_buffer *tail)
{
	/*
	 * Paired w/ acquire in flush_to_ldisc(); ensures flush_to_ldisc() sees
	 * buffer data.
	 */
	smp_store_release(&tail->commit, tail->used);
}

/**
 * tty_flip_buffer_push		-	push terminal buffers
 * @port: tty port to push
 *
 * Queue a push of the terminal flip buffers to the line discipline. Can be
 * called from IRQ/atomic context.
 *
 * In the event of the queue being busy for flipping the work will be held off
 * and retried later.
 */
void tty_flip_buffer_push(struct tty_port *port)
{
	struct tty_bufhead *buf = &port->buf;

	tty_flip_buffer_commit(buf->tail);
	queue_work(system_unbound_wq, &buf->work);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL(tty_flip_buffer_push);

/**
<<<<<<< HEAD
 *	tty_buffer_init		-	prepare a tty buffer structure
 *	@tty: tty to initialise
 *
 *	Set up the initial state of the buffer management for a tty device.
 *	Must be called before the other tty buffer functions are used.
 *
 *	Locking: none
 */

void tty_buffer_init(struct tty_struct *tty)
{
	spin_lock_init(&tty->buf.lock);
	tty->buf.head = NULL;
	tty->buf.tail = NULL;
	tty->buf.free = NULL;
	tty->buf.memory_used = 0;
	INIT_WORK(&tty->buf.work, flush_to_ldisc);
}

=======
 * tty_insert_flip_string_and_push_buffer - add characters to the tty buffer and
 *	push
 * @port: tty port
 * @chars: characters
 * @size: size
 *
 * The function combines tty_insert_flip_string() and tty_flip_buffer_push()
 * with the exception of properly holding the @port->lock.
 *
 * To be used only internally (by pty currently).
 *
 * Returns: the number added.
 */
int tty_insert_flip_string_and_push_buffer(struct tty_port *port,
					   const u8 *chars, size_t size)
{
	struct tty_bufhead *buf = &port->buf;
	unsigned long flags;

	spin_lock_irqsave(&port->lock, flags);
	size = tty_insert_flip_string(port, chars, size);
	if (size)
		tty_flip_buffer_commit(buf->tail);
	spin_unlock_irqrestore(&port->lock, flags);

	queue_work(system_unbound_wq, &buf->work);

	return size;
}

/**
 * tty_buffer_init		-	prepare a tty buffer structure
 * @port: tty port to initialise
 *
 * Set up the initial state of the buffer management for a tty device. Must be
 * called before the other tty buffer functions are used.
 */
void tty_buffer_init(struct tty_port *port)
{
	struct tty_bufhead *buf = &port->buf;

	mutex_init(&buf->lock);
	tty_buffer_reset(&buf->sentinel, 0);
	buf->head = &buf->sentinel;
	buf->tail = &buf->sentinel;
	init_llist_head(&buf->free);
	atomic_set(&buf->mem_used, 0);
	atomic_set(&buf->priority, 0);
	INIT_WORK(&buf->work, flush_to_ldisc);
	buf->mem_limit = TTYB_DEFAULT_MEM_LIMIT;
}

/**
 * tty_buffer_set_limit		-	change the tty buffer memory limit
 * @port: tty port to change
 * @limit: memory limit to set
 *
 * Change the tty buffer memory limit.
 *
 * Must be called before the other tty buffer functions are used.
 */
int tty_buffer_set_limit(struct tty_port *port, int limit)
{
	if (limit < MIN_TTYB_SIZE)
		return -EINVAL;
	port->buf.mem_limit = limit;
	return 0;
}
EXPORT_SYMBOL_GPL(tty_buffer_set_limit);

/* slave ptys can claim nested buffer lock when handling BRK and INTR */
void tty_buffer_set_lock_subclass(struct tty_port *port)
{
	lockdep_set_subclass(&port->buf.lock, TTY_LOCK_SLAVE);
}

bool tty_buffer_restart_work(struct tty_port *port)
{
	return queue_work(system_unbound_wq, &port->buf.work);
}

bool tty_buffer_cancel_work(struct tty_port *port)
{
	return cancel_work_sync(&port->buf.work);
}

void tty_buffer_flush_work(struct tty_port *port)
{
	flush_work(&port->buf.work);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
