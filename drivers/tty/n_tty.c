<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-1.0+
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * n_tty.c --- implements the N_TTY line discipline.
 *
 * This code used to be in tty_io.c, but things are getting hairy
 * enough that it made sense to split things off.  (The N_TTY
 * processing has changed so much that it's hardly recognizable,
 * anyway...)
 *
 * Note that the open routine for N_TTY is guaranteed never to return
 * an error.  This is because Linux will fall back to setting a line
 * to N_TTY if it can not switch to any other line discipline.
 *
 * Written by Theodore Ts'o, Copyright 1994.
 *
 * This file also contains code originally written by Linus Torvalds,
 * Copyright 1991, 1992, 1993, and by Julian Cowley, Copyright 1994.
 *
<<<<<<< HEAD
 * This file may be redistributed under the terms of the GNU General Public
 * License.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Reduced memory usage for older ARM systems  - Russell King.
 *
 * 2000/01/20   Fixed SMP locking on put_tty_queue using bits of
 *		the patch by Andrew J. Kroll <ag784@freenet.buffalo.edu>
 *		who actually finally proved there really was a race.
 *
 * 2002/03/18   Implemented n_tty_wakeup to send SIGIO POLL_OUTs to
 *		waiting writing processes-Sapan Bhatia <sapan@corewars.org>.
 *		Also fixed a bug in BLOCKING mode where n_tty_write returns
 *		EAGAIN
 */

<<<<<<< HEAD
#include <linux/types.h>
#include <linux/major.h>
#include <linux/errno.h>
#include <linux/signal.h>
#include <linux/fcntl.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/tty.h>
#include <linux/timer.h>
#include <linux/ctype.h>
#include <linux/mm.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/poll.h>
#include <linux/bitops.h>
#include <linux/audit.h>
#include <linux/file.h>
#include <linux/uaccess.h>
#include <linux/module.h>


/* number of characters left in xmit buffer before select has we have room */
=======
#include <linux/bitmap.h>
#include <linux/bitops.h>
#include <linux/ctype.h>
#include <linux/errno.h>
#include <linux/export.h>
#include <linux/fcntl.h>
#include <linux/file.h>
#include <linux/jiffies.h>
#include <linux/math.h>
#include <linux/poll.h>
#include <linux/ratelimit.h>
#include <linux/sched.h>
#include <linux/signal.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/tty.h>
#include <linux/types.h>
#include <linux/uaccess.h>
#include <linux/vmalloc.h>

#include "tty.h"

/*
 * Until this number of characters is queued in the xmit buffer, select will
 * return "we have room for writes".
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define WAKEUP_CHARS 256

/*
 * This defines the low- and high-watermarks for throttling and
 * unthrottling the TTY driver.  These watermarks are used for
 * controlling the space in the read buffer.
 */
#define TTY_THRESHOLD_THROTTLE		128 /* now based on remaining room */
#define TTY_THRESHOLD_UNTHROTTLE	128

/*
 * Special byte codes used in the echo buffer to represent operations
 * or special handling of characters.  Bytes in the echo buffer that
 * are not part of such special blocks are treated as normal character
 * codes.
 */
#define ECHO_OP_START 0xff
#define ECHO_OP_MOVE_BACK_COL 0x80
#define ECHO_OP_SET_CANON_COL 0x81
#define ECHO_OP_ERASE_TAB 0x82

<<<<<<< HEAD
static inline int tty_put_user(struct tty_struct *tty, unsigned char x,
			       unsigned char __user *ptr)
{
	tty_audit_add_data(tty, &x, 1);
	return put_user(x, ptr);
}

/**
 *	n_tty_set_room	-	receive space
 *	@tty: terminal
 *
 *	Sets tty->receive_room to reflect the currently available space
 *	in the input buffer, and re-schedules the flip buffer work if space
 *	just became available.
 *
 *	Locks: Concurrent update is protected with read_lock
 */

static void n_tty_set_room(struct tty_struct *tty)
{
	int left;
	int old_left;
	unsigned long flags;

	spin_lock_irqsave(&tty->read_lock, flags);

	if (I_PARMRK(tty)) {
		/* Multiply read_cnt by 3, since each byte might take up to
		 * three times as many spaces when PARMRK is set (depending on
		 * its flags, e.g. parity error). */
		left = N_TTY_BUF_SIZE - tty->read_cnt * 3 - 1;
	} else
		left = N_TTY_BUF_SIZE - tty->read_cnt - 1;

	/*
	 * If we are doing input canonicalization, and there are no
	 * pending newlines, let characters through without limit, so
	 * that erase characters will be handled.  Other excess
	 * characters will be beeped.
	 */
	if (left <= 0)
		left = tty->icanon && !tty->canon_data;
	old_left = tty->receive_room;
	tty->receive_room = left;

	spin_unlock_irqrestore(&tty->read_lock, flags);

	/* Did this open up the receive buffer? We may need to flip */
	if (left && !old_left)
		schedule_work(&tty->buf.work);
}

static void put_tty_queue_nolock(unsigned char c, struct tty_struct *tty)
{
	if (tty->read_cnt < N_TTY_BUF_SIZE) {
		tty->read_buf[tty->read_head] = c;
		tty->read_head = (tty->read_head + 1) & (N_TTY_BUF_SIZE-1);
		tty->read_cnt++;
	}
}

/**
 *	put_tty_queue		-	add character to tty
 *	@c: character
 *	@tty: tty device
 *
 *	Add a character to the tty read_buf queue. This is done under the
 *	read_lock to serialize character addition and also to protect us
 *	against parallel reads or flushes
 */

static void put_tty_queue(unsigned char c, struct tty_struct *tty)
{
	unsigned long flags;
	/*
	 *	The problem of stomping on the buffers ends here.
	 *	Why didn't anyone see this one coming? --AJK
	*/
	spin_lock_irqsave(&tty->read_lock, flags);
	put_tty_queue_nolock(c, tty);
	spin_unlock_irqrestore(&tty->read_lock, flags);
}

/**
 *	check_unthrottle	-	allow new receive data
 *	@tty; tty device
 *
 *	Check whether to call the driver unthrottle functions
 *
 *	Can sleep, may be called under the atomic_read_lock mutex but
 *	this is not guaranteed.
 */
static void check_unthrottle(struct tty_struct *tty)
{
	if (tty->count)
		tty_unthrottle(tty);
}

/**
 *	reset_buffer_flags	-	reset buffer state
 *	@tty: terminal to reset
 *
 *	Reset the read buffer counters, clear the flags,
 *	and make sure the driver is unthrottled. Called
 *	from n_tty_open() and n_tty_flush_buffer().
 *
 *	Locking: tty_read_lock for read fields.
 */

static void reset_buffer_flags(struct tty_struct *tty)
{
	unsigned long flags;

	spin_lock_irqsave(&tty->read_lock, flags);
	tty->read_head = tty->read_tail = tty->read_cnt = 0;
	spin_unlock_irqrestore(&tty->read_lock, flags);

	mutex_lock(&tty->echo_lock);
	tty->echo_pos = tty->echo_cnt = tty->echo_overrun = 0;
	mutex_unlock(&tty->echo_lock);

	tty->canon_head = tty->canon_data = tty->erasing = 0;
	memset(&tty->read_flags, 0, sizeof tty->read_flags);
	n_tty_set_room(tty);
}

/**
 *	n_tty_flush_buffer	-	clean input queue
 *	@tty:	terminal device
 *
 *	Flush the input buffer. Called when the line discipline is
 *	being closed, when the tty layer wants the buffer flushed (eg
 *	at hangup) or when the N_TTY line discipline internally has to
 *	clean the pending queue (for example some signals).
 *
 *	Locking: ctrl_lock, read_lock.
 */

static void n_tty_flush_buffer(struct tty_struct *tty)
{
	unsigned long flags;
	/* clear everything and unthrottle the driver */
	reset_buffer_flags(tty);

	if (!tty->link)
		return;

	spin_lock_irqsave(&tty->ctrl_lock, flags);
	if (tty->link->packet) {
		tty->ctrl_status |= TIOCPKT_FLUSHREAD;
		wake_up_interruptible(&tty->link->read_wait);
	}
	spin_unlock_irqrestore(&tty->ctrl_lock, flags);
}

/**
 *	n_tty_chars_in_buffer	-	report available bytes
 *	@tty: tty device
 *
 *	Report the number of characters buffered to be delivered to user
 *	at this instant in time.
 *
 *	Locking: read_lock
 */

static ssize_t n_tty_chars_in_buffer(struct tty_struct *tty)
{
	unsigned long flags;
	ssize_t n = 0;

	spin_lock_irqsave(&tty->read_lock, flags);
	if (!tty->icanon) {
		n = tty->read_cnt;
	} else if (tty->canon_data) {
		n = (tty->canon_head > tty->read_tail) ?
			tty->canon_head - tty->read_tail :
			tty->canon_head + (N_TTY_BUF_SIZE - tty->read_tail);
	}
	spin_unlock_irqrestore(&tty->read_lock, flags);
	return n;
}

/**
 *	is_utf8_continuation	-	utf8 multibyte check
 *	@c: byte to check
 *
 *	Returns true if the utf8 character 'c' is a multibyte continuation
 *	character. We use this to correctly compute the on screen size
 *	of the character when printing
 */

static inline int is_utf8_continuation(unsigned char c)
=======
#define ECHO_COMMIT_WATERMARK	256
#define ECHO_BLOCK		256
#define ECHO_DISCARD_WATERMARK	N_TTY_BUF_SIZE - (ECHO_BLOCK + 32)


#undef N_TTY_TRACE
#ifdef N_TTY_TRACE
# define n_tty_trace(f, args...)	trace_printk(f, ##args)
#else
# define n_tty_trace(f, args...)	no_printk(f, ##args)
#endif

struct n_tty_data {
	/* producer-published */
	size_t read_head;
	size_t commit_head;
	size_t canon_head;
	size_t echo_head;
	size_t echo_commit;
	size_t echo_mark;
	DECLARE_BITMAP(char_map, 256);

	/* private to n_tty_receive_overrun (single-threaded) */
	unsigned long overrun_time;
	unsigned int num_overrun;

	/* non-atomic */
	bool no_room;

	/* must hold exclusive termios_rwsem to reset these */
	unsigned char lnext:1, erasing:1, raw:1, real_raw:1, icanon:1;
	unsigned char push:1;

	/* shared by producer and consumer */
	u8 read_buf[N_TTY_BUF_SIZE];
	DECLARE_BITMAP(read_flags, N_TTY_BUF_SIZE);
	u8 echo_buf[N_TTY_BUF_SIZE];

	/* consumer-published */
	size_t read_tail;
	size_t line_start;

	/* # of chars looked ahead (to find software flow control chars) */
	size_t lookahead_count;

	/* protected by output lock */
	unsigned int column;
	unsigned int canon_column;
	size_t echo_tail;

	struct mutex atomic_read_lock;
	struct mutex output_lock;
};

#define MASK(x) ((x) & (N_TTY_BUF_SIZE - 1))

static inline size_t read_cnt(struct n_tty_data *ldata)
{
	return ldata->read_head - ldata->read_tail;
}

static inline u8 read_buf(struct n_tty_data *ldata, size_t i)
{
	return ldata->read_buf[MASK(i)];
}

static inline u8 *read_buf_addr(struct n_tty_data *ldata, size_t i)
{
	return &ldata->read_buf[MASK(i)];
}

static inline u8 echo_buf(struct n_tty_data *ldata, size_t i)
{
	smp_rmb(); /* Matches smp_wmb() in add_echo_byte(). */
	return ldata->echo_buf[MASK(i)];
}

static inline u8 *echo_buf_addr(struct n_tty_data *ldata, size_t i)
{
	return &ldata->echo_buf[MASK(i)];
}

/* If we are not echoing the data, perhaps this is a secret so erase it */
static void zero_buffer(const struct tty_struct *tty, u8 *buffer, size_t size)
{
	if (L_ICANON(tty) && !L_ECHO(tty))
		memset(buffer, 0, size);
}

static void tty_copy(const struct tty_struct *tty, void *to, size_t tail,
		     size_t n)
{
	struct n_tty_data *ldata = tty->disc_data;
	size_t size = N_TTY_BUF_SIZE - tail;
	void *from = read_buf_addr(ldata, tail);

	if (n > size) {
		tty_audit_add_data(tty, from, size);
		memcpy(to, from, size);
		zero_buffer(tty, from, size);
		to += size;
		n -= size;
		from = ldata->read_buf;
	}

	tty_audit_add_data(tty, from, n);
	memcpy(to, from, n);
	zero_buffer(tty, from, n);
}

/**
 * n_tty_kick_worker - start input worker (if required)
 * @tty: terminal
 *
 * Re-schedules the flip buffer work if it may have stopped.
 *
 * Locking:
 *  * Caller holds exclusive %termios_rwsem, or
 *  * n_tty_read()/consumer path:
 *	holds non-exclusive %termios_rwsem
 */
static void n_tty_kick_worker(const struct tty_struct *tty)
{
	struct n_tty_data *ldata = tty->disc_data;

	/* Did the input worker stop? Restart it */
	if (unlikely(READ_ONCE(ldata->no_room))) {
		WRITE_ONCE(ldata->no_room, 0);

		WARN_RATELIMIT(tty->port->itty == NULL,
				"scheduling with invalid itty\n");
		/* see if ldisc has been killed - if so, this means that
		 * even though the ldisc has been halted and ->buf.work
		 * cancelled, ->buf.work is about to be rescheduled
		 */
		WARN_RATELIMIT(test_bit(TTY_LDISC_HALTED, &tty->flags),
			       "scheduling buffer work for halted ldisc\n");
		tty_buffer_restart_work(tty->port);
	}
}

static ssize_t chars_in_buffer(const struct tty_struct *tty)
{
	const struct n_tty_data *ldata = tty->disc_data;
	size_t head = ldata->icanon ? ldata->canon_head : ldata->commit_head;

	return head - ldata->read_tail;
}

/**
 * n_tty_write_wakeup	-	asynchronous I/O notifier
 * @tty: tty device
 *
 * Required for the ptys, serial driver etc. since processes that attach
 * themselves to the master and rely on ASYNC IO must be woken up.
 */
static void n_tty_write_wakeup(struct tty_struct *tty)
{
	clear_bit(TTY_DO_WRITE_WAKEUP, &tty->flags);
	kill_fasync(&tty->fasync, SIGIO, POLL_OUT);
}

static void n_tty_check_throttle(struct tty_struct *tty)
{
	struct n_tty_data *ldata = tty->disc_data;

	/*
	 * Check the remaining room for the input canonicalization
	 * mode.  We don't want to throttle the driver if we're in
	 * canonical mode and don't have a newline yet!
	 */
	if (ldata->icanon && ldata->canon_head == ldata->read_tail)
		return;

	do {
		tty_set_flow_change(tty, TTY_THROTTLE_SAFE);
		if (N_TTY_BUF_SIZE - read_cnt(ldata) >= TTY_THRESHOLD_THROTTLE)
			break;
	} while (!tty_throttle_safe(tty));

	__tty_set_flow_change(tty, 0);
}

static void n_tty_check_unthrottle(struct tty_struct *tty)
{
	if (tty->driver->type == TTY_DRIVER_TYPE_PTY) {
		if (chars_in_buffer(tty) > TTY_THRESHOLD_UNTHROTTLE)
			return;
		n_tty_kick_worker(tty);
		tty_wakeup(tty->link);
		return;
	}

	/* If there is enough space in the read buffer now, let the
	 * low-level driver know. We use chars_in_buffer() to
	 * check the buffer, as it now knows about canonical mode.
	 * Otherwise, if the driver is throttled and the line is
	 * longer than TTY_THRESHOLD_UNTHROTTLE in canonical mode,
	 * we won't get any more characters.
	 */

	do {
		tty_set_flow_change(tty, TTY_UNTHROTTLE_SAFE);
		if (chars_in_buffer(tty) > TTY_THRESHOLD_UNTHROTTLE)
			break;

		n_tty_kick_worker(tty);
	} while (!tty_unthrottle_safe(tty));

	__tty_set_flow_change(tty, 0);
}

/**
 * put_tty_queue		-	add character to tty
 * @c: character
 * @ldata: n_tty data
 *
 * Add a character to the tty read_buf queue.
 *
 * Locking:
 *  * n_tty_receive_buf()/producer path:
 *	caller holds non-exclusive %termios_rwsem
 */
static inline void put_tty_queue(u8 c, struct n_tty_data *ldata)
{
	*read_buf_addr(ldata, ldata->read_head) = c;
	ldata->read_head++;
}

/**
 * reset_buffer_flags	-	reset buffer state
 * @ldata: line disc data to reset
 *
 * Reset the read buffer counters and clear the flags. Called from
 * n_tty_open() and n_tty_flush_buffer().
 *
 * Locking:
 *  * caller holds exclusive %termios_rwsem, or
 *  * (locking is not required)
 */
static void reset_buffer_flags(struct n_tty_data *ldata)
{
	ldata->read_head = ldata->canon_head = ldata->read_tail = 0;
	ldata->commit_head = 0;
	ldata->line_start = 0;

	ldata->erasing = 0;
	bitmap_zero(ldata->read_flags, N_TTY_BUF_SIZE);
	ldata->push = 0;

	ldata->lookahead_count = 0;
}

static void n_tty_packet_mode_flush(struct tty_struct *tty)
{
	unsigned long flags;

	if (tty->link->ctrl.packet) {
		spin_lock_irqsave(&tty->ctrl.lock, flags);
		tty->ctrl.pktstatus |= TIOCPKT_FLUSHREAD;
		spin_unlock_irqrestore(&tty->ctrl.lock, flags);
		wake_up_interruptible(&tty->link->read_wait);
	}
}

/**
 * n_tty_flush_buffer	-	clean input queue
 * @tty: terminal device
 *
 * Flush the input buffer. Called when the tty layer wants the buffer flushed
 * (eg at hangup) or when the %N_TTY line discipline internally has to clean
 * the pending queue (for example some signals).
 *
 * Holds %termios_rwsem to exclude producer/consumer while buffer indices are
 * reset.
 *
 * Locking: %ctrl.lock, exclusive %termios_rwsem
 */
static void n_tty_flush_buffer(struct tty_struct *tty)
{
	down_write(&tty->termios_rwsem);
	reset_buffer_flags(tty->disc_data);
	n_tty_kick_worker(tty);

	if (tty->link)
		n_tty_packet_mode_flush(tty);
	up_write(&tty->termios_rwsem);
}

/**
 * is_utf8_continuation	-	utf8 multibyte check
 * @c: byte to check
 *
 * Returns: true if the utf8 character @c is a multibyte continuation
 * character. We use this to correctly compute the on-screen size of the
 * character when printing.
 */
static inline int is_utf8_continuation(u8 c)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return (c & 0xc0) == 0x80;
}

/**
<<<<<<< HEAD
 *	is_continuation		-	multibyte check
 *	@c: byte to check
 *
 *	Returns true if the utf8 character 'c' is a multibyte continuation
 *	character and the terminal is in unicode mode.
 */

static inline int is_continuation(unsigned char c, struct tty_struct *tty)
=======
 * is_continuation	-	multibyte check
 * @c: byte to check
 * @tty: terminal device
 *
 * Returns: true if the utf8 character @c is a multibyte continuation character
 * and the terminal is in unicode mode.
 */
static inline int is_continuation(u8 c, const struct tty_struct *tty)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return I_IUTF8(tty) && is_utf8_continuation(c);
}

/**
<<<<<<< HEAD
 *	do_output_char			-	output one character
 *	@c: character (or partial unicode symbol)
 *	@tty: terminal device
 *	@space: space available in tty driver write buffer
 *
 *	This is a helper function that handles one output character
 *	(including special characters like TAB, CR, LF, etc.),
 *	doing OPOST processing and putting the results in the
 *	tty driver's write buffer.
 *
 *	Note that Linux currently ignores TABDLY, CRDLY, VTDLY, FFDLY
 *	and NLDLY.  They simply aren't relevant in the world today.
 *	If you ever need them, add them here.
 *
 *	Returns the number of bytes of buffer space used or -1 if
 *	no space left.
 *
 *	Locking: should be called under the output_lock to protect
 *		 the column state and space left in the buffer
 */

static int do_output_char(unsigned char c, struct tty_struct *tty, int space)
{
=======
 * do_output_char	-	output one character
 * @c: character (or partial unicode symbol)
 * @tty: terminal device
 * @space: space available in tty driver write buffer
 *
 * This is a helper function that handles one output character (including
 * special characters like TAB, CR, LF, etc.), doing OPOST processing and
 * putting the results in the tty driver's write buffer.
 *
 * Note that Linux currently ignores TABDLY, CRDLY, VTDLY, FFDLY and NLDLY.
 * They simply aren't relevant in the world today. If you ever need them, add
 * them here.
 *
 * Returns: the number of bytes of buffer space used or -1 if no space left.
 *
 * Locking: should be called under the %output_lock to protect the column state
 * and space left in the buffer.
 */
static int do_output_char(u8 c, struct tty_struct *tty, int space)
{
	struct n_tty_data *ldata = tty->disc_data;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int	spaces;

	if (!space)
		return -1;

	switch (c) {
	case '\n':
		if (O_ONLRET(tty))
<<<<<<< HEAD
			tty->column = 0;
		if (O_ONLCR(tty)) {
			if (space < 2)
				return -1;
			tty->canon_column = tty->column = 0;
			tty->ops->write(tty, "\r\n", 2);
			return 2;
		}
		tty->canon_column = tty->column;
		break;
	case '\r':
		if (O_ONOCR(tty) && tty->column == 0)
=======
			ldata->column = 0;
		if (O_ONLCR(tty)) {
			if (space < 2)
				return -1;
			ldata->canon_column = ldata->column = 0;
			tty->ops->write(tty, "\r\n", 2);
			return 2;
		}
		ldata->canon_column = ldata->column;
		break;
	case '\r':
		if (O_ONOCR(tty) && ldata->column == 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return 0;
		if (O_OCRNL(tty)) {
			c = '\n';
			if (O_ONLRET(tty))
<<<<<<< HEAD
				tty->canon_column = tty->column = 0;
			break;
		}
		tty->canon_column = tty->column = 0;
		break;
	case '\t':
		spaces = 8 - (tty->column & 7);
		if (O_TABDLY(tty) == XTABS) {
			if (space < spaces)
				return -1;
			tty->column += spaces;
			tty->ops->write(tty, "        ", spaces);
			return spaces;
		}
		tty->column += spaces;
		break;
	case '\b':
		if (tty->column > 0)
			tty->column--;
=======
				ldata->canon_column = ldata->column = 0;
			break;
		}
		ldata->canon_column = ldata->column = 0;
		break;
	case '\t':
		spaces = 8 - (ldata->column & 7);
		if (O_TABDLY(tty) == XTABS) {
			if (space < spaces)
				return -1;
			ldata->column += spaces;
			tty->ops->write(tty, "        ", spaces);
			return spaces;
		}
		ldata->column += spaces;
		break;
	case '\b':
		if (ldata->column > 0)
			ldata->column--;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	default:
		if (!iscntrl(c)) {
			if (O_OLCUC(tty))
				c = toupper(c);
			if (!is_continuation(c, tty))
<<<<<<< HEAD
				tty->column++;
=======
				ldata->column++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		break;
	}

	tty_put_char(tty, c);
	return 1;
}

/**
<<<<<<< HEAD
 *	process_output			-	output post processor
 *	@c: character (or partial unicode symbol)
 *	@tty: terminal device
 *
 *	Output one character with OPOST processing.
 *	Returns -1 when the output device is full and the character
 *	must be retried.
 *
 *	Locking: output_lock to protect column state and space left
 *		 (also, this is called from n_tty_write under the
 *		  tty layer write lock)
 */

static int process_output(unsigned char c, struct tty_struct *tty)
{
	int	space, retval;

	mutex_lock(&tty->output_lock);
=======
 * process_output	-	output post processor
 * @c: character (or partial unicode symbol)
 * @tty: terminal device
 *
 * Output one character with OPOST processing.
 *
 * Returns: -1 when the output device is full and the character must be
 * retried.
 *
 * Locking: %output_lock to protect column state and space left (also, this is
 *called from n_tty_write() under the tty layer write lock).
 */
static int process_output(u8 c, struct tty_struct *tty)
{
	struct n_tty_data *ldata = tty->disc_data;
	int	space, retval;

	mutex_lock(&ldata->output_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	space = tty_write_room(tty);
	retval = do_output_char(c, tty, space);

<<<<<<< HEAD
	mutex_unlock(&tty->output_lock);
=======
	mutex_unlock(&ldata->output_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (retval < 0)
		return -1;
	else
		return 0;
}

/**
<<<<<<< HEAD
 *	process_output_block		-	block post processor
 *	@tty: terminal device
 *	@buf: character buffer
 *	@nr: number of bytes to output
 *
 *	Output a block of characters with OPOST processing.
 *	Returns the number of characters output.
 *
 *	This path is used to speed up block console writes, among other
 *	things when processing blocks of output data. It handles only
 *	the simple cases normally found and helps to generate blocks of
 *	symbols for the console driver and thus improve performance.
 *
 *	Locking: output_lock to protect column state and space left
 *		 (also, this is called from n_tty_write under the
 *		  tty layer write lock)
 */

static ssize_t process_output_block(struct tty_struct *tty,
				    const unsigned char *buf, unsigned int nr)
{
	int	space;
	int	i;
	const unsigned char *cp;

	mutex_lock(&tty->output_lock);

	space = tty_write_room(tty);
	if (!space) {
		mutex_unlock(&tty->output_lock);
		return 0;
=======
 * process_output_block	-	block post processor
 * @tty: terminal device
 * @buf: character buffer
 * @nr: number of bytes to output
 *
 * Output a block of characters with OPOST processing.
 *
 * This path is used to speed up block console writes, among other things when
 * processing blocks of output data. It handles only the simple cases normally
 * found and helps to generate blocks of symbols for the console driver and
 * thus improve performance.
 *
 * Returns: the number of characters output.
 *
 * Locking: %output_lock to protect column state and space left (also, this is
 * called from n_tty_write() under the tty layer write lock).
 */
static ssize_t process_output_block(struct tty_struct *tty,
				    const u8 *buf, unsigned int nr)
{
	struct n_tty_data *ldata = tty->disc_data;
	int	space;
	int	i;
	const u8 *cp;

	mutex_lock(&ldata->output_lock);

	space = tty_write_room(tty);
	if (space <= 0) {
		mutex_unlock(&ldata->output_lock);
		return space;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	if (nr > space)
		nr = space;

	for (i = 0, cp = buf; i < nr; i++, cp++) {
<<<<<<< HEAD
		unsigned char c = *cp;
=======
		u8 c = *cp;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		switch (c) {
		case '\n':
			if (O_ONLRET(tty))
<<<<<<< HEAD
				tty->column = 0;
			if (O_ONLCR(tty))
				goto break_out;
			tty->canon_column = tty->column;
			break;
		case '\r':
			if (O_ONOCR(tty) && tty->column == 0)
				goto break_out;
			if (O_OCRNL(tty))
				goto break_out;
			tty->canon_column = tty->column = 0;
=======
				ldata->column = 0;
			if (O_ONLCR(tty))
				goto break_out;
			ldata->canon_column = ldata->column;
			break;
		case '\r':
			if (O_ONOCR(tty) && ldata->column == 0)
				goto break_out;
			if (O_OCRNL(tty))
				goto break_out;
			ldata->canon_column = ldata->column = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		case '\t':
			goto break_out;
		case '\b':
<<<<<<< HEAD
			if (tty->column > 0)
				tty->column--;
=======
			if (ldata->column > 0)
				ldata->column--;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		default:
			if (!iscntrl(c)) {
				if (O_OLCUC(tty))
					goto break_out;
				if (!is_continuation(c, tty))
<<<<<<< HEAD
					tty->column++;
=======
					ldata->column++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			}
			break;
		}
	}
break_out:
	i = tty->ops->write(tty, buf, i);

<<<<<<< HEAD
	mutex_unlock(&tty->output_lock);
	return i;
}

/**
 *	process_echoes	-	write pending echo characters
 *	@tty: terminal device
 *
 *	Write previously buffered echo (and other ldisc-generated)
 *	characters to the tty.
 *
 *	Characters generated by the ldisc (including echoes) need to
 *	be buffered because the driver's write buffer can fill during
 *	heavy program output.  Echoing straight to the driver will
 *	often fail under these conditions, causing lost characters and
 *	resulting mismatches of ldisc state information.
 *
 *	Since the ldisc state must represent the characters actually sent
 *	to the driver at the time of the write, operations like certain
 *	changes in column state are also saved in the buffer and executed
 *	here.
 *
 *	A circular fifo buffer is used so that the most recent characters
 *	are prioritized.  Also, when control characters are echoed with a
 *	prefixed "^", the pair is treated atomically and thus not separated.
 *
 *	Locking: output_lock to protect column state and space left,
 *		 echo_lock to protect the echo buffer
 */

static void process_echoes(struct tty_struct *tty)
{
	int	space, nr;
	unsigned char c;
	unsigned char *cp, *buf_end;

	if (!tty->echo_cnt)
		return;

	mutex_lock(&tty->output_lock);
	mutex_lock(&tty->echo_lock);

	space = tty_write_room(tty);

	buf_end = tty->echo_buf + N_TTY_BUF_SIZE;
	cp = tty->echo_buf + tty->echo_pos;
	nr = tty->echo_cnt;
	while (nr > 0) {
		c = *cp;
		if (c == ECHO_OP_START) {
			unsigned char op;
			unsigned char *opp;
			int no_space_left = 0;

			/*
			 * If the buffer byte is the start of a multi-byte
			 * operation, get the next byte, which is either the
			 * op code or a control character value.
			 */
			opp = cp + 1;
			if (opp == buf_end)
				opp -= N_TTY_BUF_SIZE;
			op = *opp;

			switch (op) {
				unsigned int num_chars, num_bs;

			case ECHO_OP_ERASE_TAB:
				if (++opp == buf_end)
					opp -= N_TTY_BUF_SIZE;
				num_chars = *opp;

				/*
				 * Determine how many columns to go back
				 * in order to erase the tab.
				 * This depends on the number of columns
				 * used by other characters within the tab
				 * area.  If this (modulo 8) count is from
				 * the start of input rather than from a
				 * previous tab, we offset by canon column.
				 * Otherwise, tab spacing is normal.
				 */
				if (!(num_chars & 0x80))
					num_chars += tty->canon_column;
				num_bs = 8 - (num_chars & 7);

				if (num_bs > space) {
					no_space_left = 1;
					break;
				}
				space -= num_bs;
				while (num_bs--) {
					tty_put_char(tty, '\b');
					if (tty->column > 0)
						tty->column--;
				}
				cp += 3;
				nr -= 3;
				break;

			case ECHO_OP_SET_CANON_COL:
				tty->canon_column = tty->column;
				cp += 2;
				nr -= 2;
				break;

			case ECHO_OP_MOVE_BACK_COL:
				if (tty->column > 0)
					tty->column--;
				cp += 2;
				nr -= 2;
				break;

			case ECHO_OP_START:
				/* This is an escaped echo op start code */
				if (!space) {
					no_space_left = 1;
					break;
				}
				tty_put_char(tty, ECHO_OP_START);
				tty->column++;
				space--;
				cp += 2;
				nr -= 2;
				break;

			default:
				/*
				 * If the op is not a special byte code,
				 * it is a ctrl char tagged to be echoed
				 * as "^X" (where X is the letter
				 * representing the control char).
				 * Note that we must ensure there is
				 * enough space for the whole ctrl pair.
				 *
				 */
				if (space < 2) {
					no_space_left = 1;
					break;
				}
				tty_put_char(tty, '^');
				tty_put_char(tty, op ^ 0100);
				tty->column += 2;
				space -= 2;
				cp += 2;
				nr -= 2;
			}

			if (no_space_left)
				break;
		} else {
			if (O_OPOST(tty) &&
			    !(test_bit(TTY_HW_COOK_OUT, &tty->flags))) {
=======
	mutex_unlock(&ldata->output_lock);
	return i;
}

static int n_tty_process_echo_ops(struct tty_struct *tty, size_t *tail,
				  int space)
{
	struct n_tty_data *ldata = tty->disc_data;
	u8 op;

	/*
	 * Since add_echo_byte() is called without holding output_lock, we
	 * might see only portion of multi-byte operation.
	 */
	if (MASK(ldata->echo_commit) == MASK(*tail + 1))
		return -ENODATA;

	/*
	 * If the buffer byte is the start of a multi-byte operation, get the
	 * next byte, which is either the op code or a control character value.
	 */
	op = echo_buf(ldata, *tail + 1);

	switch (op) {
	case ECHO_OP_ERASE_TAB: {
		unsigned int num_chars, num_bs;

		if (MASK(ldata->echo_commit) == MASK(*tail + 2))
			return -ENODATA;

		num_chars = echo_buf(ldata, *tail + 2);

		/*
		 * Determine how many columns to go back in order to erase the
		 * tab. This depends on the number of columns used by other
		 * characters within the tab area. If this (modulo 8) count is
		 * from the start of input rather than from a previous tab, we
		 * offset by canon column. Otherwise, tab spacing is normal.
		 */
		if (!(num_chars & 0x80))
			num_chars += ldata->canon_column;
		num_bs = 8 - (num_chars & 7);

		if (num_bs > space)
			return -ENOSPC;

		space -= num_bs;
		while (num_bs--) {
			tty_put_char(tty, '\b');
			if (ldata->column > 0)
				ldata->column--;
		}
		*tail += 3;
		break;
	}
	case ECHO_OP_SET_CANON_COL:
		ldata->canon_column = ldata->column;
		*tail += 2;
		break;

	case ECHO_OP_MOVE_BACK_COL:
		if (ldata->column > 0)
			ldata->column--;
		*tail += 2;
		break;

	case ECHO_OP_START:
		/* This is an escaped echo op start code */
		if (!space)
			return -ENOSPC;

		tty_put_char(tty, ECHO_OP_START);
		ldata->column++;
		space--;
		*tail += 2;
		break;

	default:
		/*
		 * If the op is not a special byte code, it is a ctrl char
		 * tagged to be echoed as "^X" (where X is the letter
		 * representing the control char). Note that we must ensure
		 * there is enough space for the whole ctrl pair.
		 */
		if (space < 2)
			return -ENOSPC;

		tty_put_char(tty, '^');
		tty_put_char(tty, op ^ 0100);
		ldata->column += 2;
		space -= 2;
		*tail += 2;
		break;
	}

	return space;
}

/**
 * __process_echoes	-	write pending echo characters
 * @tty: terminal device
 *
 * Write previously buffered echo (and other ldisc-generated) characters to the
 * tty.
 *
 * Characters generated by the ldisc (including echoes) need to be buffered
 * because the driver's write buffer can fill during heavy program output.
 * Echoing straight to the driver will often fail under these conditions,
 * causing lost characters and resulting mismatches of ldisc state information.
 *
 * Since the ldisc state must represent the characters actually sent to the
 * driver at the time of the write, operations like certain changes in column
 * state are also saved in the buffer and executed here.
 *
 * A circular fifo buffer is used so that the most recent characters are
 * prioritized. Also, when control characters are echoed with a prefixed "^",
 * the pair is treated atomically and thus not separated.
 *
 * Locking: callers must hold %output_lock.
 */
static size_t __process_echoes(struct tty_struct *tty)
{
	struct n_tty_data *ldata = tty->disc_data;
	int	space, old_space;
	size_t tail;
	u8 c;

	old_space = space = tty_write_room(tty);

	tail = ldata->echo_tail;
	while (MASK(ldata->echo_commit) != MASK(tail)) {
		c = echo_buf(ldata, tail);
		if (c == ECHO_OP_START) {
			int ret = n_tty_process_echo_ops(tty, &tail, space);
			if (ret == -ENODATA)
				goto not_yet_stored;
			if (ret < 0)
				break;
			space = ret;
		} else {
			if (O_OPOST(tty)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				int retval = do_output_char(c, tty, space);
				if (retval < 0)
					break;
				space -= retval;
			} else {
				if (!space)
					break;
				tty_put_char(tty, c);
				space -= 1;
			}
<<<<<<< HEAD
			cp += 1;
			nr -= 1;
		}

		/* When end of circular buffer reached, wrap around */
		if (cp >= buf_end)
			cp -= N_TTY_BUF_SIZE;
	}

	if (nr == 0) {
		tty->echo_pos = 0;
		tty->echo_cnt = 0;
		tty->echo_overrun = 0;
	} else {
		int num_processed = tty->echo_cnt - nr;
		tty->echo_pos += num_processed;
		tty->echo_pos &= N_TTY_BUF_SIZE - 1;
		tty->echo_cnt = nr;
		if (num_processed > 0)
			tty->echo_overrun = 0;
	}

	mutex_unlock(&tty->echo_lock);
	mutex_unlock(&tty->output_lock);

	if (tty->ops->flush_chars)
		tty->ops->flush_chars(tty);
}

/**
 *	add_echo_byte	-	add a byte to the echo buffer
 *	@c: unicode byte to echo
 *	@tty: terminal device
 *
 *	Add a character or operation byte to the echo buffer.
 *
 *	Should be called under the echo lock to protect the echo buffer.
 */

static void add_echo_byte(unsigned char c, struct tty_struct *tty)
{
	int	new_byte_pos;

	if (tty->echo_cnt == N_TTY_BUF_SIZE) {
		/* Circular buffer is already at capacity */
		new_byte_pos = tty->echo_pos;

		/*
		 * Since the buffer start position needs to be advanced,
		 * be sure to step by a whole operation byte group.
		 */
		if (tty->echo_buf[tty->echo_pos] == ECHO_OP_START) {
			if (tty->echo_buf[(tty->echo_pos + 1) &
					  (N_TTY_BUF_SIZE - 1)] ==
						ECHO_OP_ERASE_TAB) {
				tty->echo_pos += 3;
				tty->echo_cnt -= 2;
			} else {
				tty->echo_pos += 2;
				tty->echo_cnt -= 1;
			}
		} else {
			tty->echo_pos++;
		}
		tty->echo_pos &= N_TTY_BUF_SIZE - 1;

		tty->echo_overrun = 1;
	} else {
		new_byte_pos = tty->echo_pos + tty->echo_cnt;
		new_byte_pos &= N_TTY_BUF_SIZE - 1;
		tty->echo_cnt++;
	}

	tty->echo_buf[new_byte_pos] = c;
}

/**
 *	echo_move_back_col	-	add operation to move back a column
 *	@tty: terminal device
 *
 *	Add an operation to the echo buffer to move back one column.
 *
 *	Locking: echo_lock to protect the echo buffer
 */

static void echo_move_back_col(struct tty_struct *tty)
{
	mutex_lock(&tty->echo_lock);

	add_echo_byte(ECHO_OP_START, tty);
	add_echo_byte(ECHO_OP_MOVE_BACK_COL, tty);

	mutex_unlock(&tty->echo_lock);
}

/**
 *	echo_set_canon_col	-	add operation to set the canon column
 *	@tty: terminal device
 *
 *	Add an operation to the echo buffer to set the canon column
 *	to the current column.
 *
 *	Locking: echo_lock to protect the echo buffer
 */

static void echo_set_canon_col(struct tty_struct *tty)
{
	mutex_lock(&tty->echo_lock);

	add_echo_byte(ECHO_OP_START, tty);
	add_echo_byte(ECHO_OP_SET_CANON_COL, tty);

	mutex_unlock(&tty->echo_lock);
}

/**
 *	echo_erase_tab	-	add operation to erase a tab
 *	@num_chars: number of character columns already used
 *	@after_tab: true if num_chars starts after a previous tab
 *	@tty: terminal device
 *
 *	Add an operation to the echo buffer to erase a tab.
 *
 *	Called by the eraser function, which knows how many character
 *	columns have been used since either a previous tab or the start
 *	of input.  This information will be used later, along with
 *	canon column (if applicable), to go back the correct number
 *	of columns.
 *
 *	Locking: echo_lock to protect the echo buffer
 */

static void echo_erase_tab(unsigned int num_chars, int after_tab,
			   struct tty_struct *tty)
{
	mutex_lock(&tty->echo_lock);

	add_echo_byte(ECHO_OP_START, tty);
	add_echo_byte(ECHO_OP_ERASE_TAB, tty);
=======
			tail += 1;
		}
	}

	/* If the echo buffer is nearly full (so that the possibility exists
	 * of echo overrun before the next commit), then discard enough
	 * data at the tail to prevent a subsequent overrun */
	while (ldata->echo_commit > tail &&
	       ldata->echo_commit - tail >= ECHO_DISCARD_WATERMARK) {
		if (echo_buf(ldata, tail) == ECHO_OP_START) {
			if (echo_buf(ldata, tail + 1) == ECHO_OP_ERASE_TAB)
				tail += 3;
			else
				tail += 2;
		} else
			tail++;
	}

 not_yet_stored:
	ldata->echo_tail = tail;
	return old_space - space;
}

static void commit_echoes(struct tty_struct *tty)
{
	struct n_tty_data *ldata = tty->disc_data;
	size_t nr, old, echoed;
	size_t head;

	mutex_lock(&ldata->output_lock);
	head = ldata->echo_head;
	ldata->echo_mark = head;
	old = ldata->echo_commit - ldata->echo_tail;

	/* Process committed echoes if the accumulated # of bytes
	 * is over the threshold (and try again each time another
	 * block is accumulated) */
	nr = head - ldata->echo_tail;
	if (nr < ECHO_COMMIT_WATERMARK ||
	    (nr % ECHO_BLOCK > old % ECHO_BLOCK)) {
		mutex_unlock(&ldata->output_lock);
		return;
	}

	ldata->echo_commit = head;
	echoed = __process_echoes(tty);
	mutex_unlock(&ldata->output_lock);

	if (echoed && tty->ops->flush_chars)
		tty->ops->flush_chars(tty);
}

static void process_echoes(struct tty_struct *tty)
{
	struct n_tty_data *ldata = tty->disc_data;
	size_t echoed;

	if (ldata->echo_mark == ldata->echo_tail)
		return;

	mutex_lock(&ldata->output_lock);
	ldata->echo_commit = ldata->echo_mark;
	echoed = __process_echoes(tty);
	mutex_unlock(&ldata->output_lock);

	if (echoed && tty->ops->flush_chars)
		tty->ops->flush_chars(tty);
}

/* NB: echo_mark and echo_head should be equivalent here */
static void flush_echoes(struct tty_struct *tty)
{
	struct n_tty_data *ldata = tty->disc_data;

	if ((!L_ECHO(tty) && !L_ECHONL(tty)) ||
	    ldata->echo_commit == ldata->echo_head)
		return;

	mutex_lock(&ldata->output_lock);
	ldata->echo_commit = ldata->echo_head;
	__process_echoes(tty);
	mutex_unlock(&ldata->output_lock);
}

/**
 * add_echo_byte	-	add a byte to the echo buffer
 * @c: unicode byte to echo
 * @ldata: n_tty data
 *
 * Add a character or operation byte to the echo buffer.
 */
static inline void add_echo_byte(u8 c, struct n_tty_data *ldata)
{
	*echo_buf_addr(ldata, ldata->echo_head) = c;
	smp_wmb(); /* Matches smp_rmb() in echo_buf(). */
	ldata->echo_head++;
}

/**
 * echo_move_back_col	-	add operation to move back a column
 * @ldata: n_tty data
 *
 * Add an operation to the echo buffer to move back one column.
 */
static void echo_move_back_col(struct n_tty_data *ldata)
{
	add_echo_byte(ECHO_OP_START, ldata);
	add_echo_byte(ECHO_OP_MOVE_BACK_COL, ldata);
}

/**
 * echo_set_canon_col	-	add operation to set the canon column
 * @ldata: n_tty data
 *
 * Add an operation to the echo buffer to set the canon column to the current
 * column.
 */
static void echo_set_canon_col(struct n_tty_data *ldata)
{
	add_echo_byte(ECHO_OP_START, ldata);
	add_echo_byte(ECHO_OP_SET_CANON_COL, ldata);
}

/**
 * echo_erase_tab	-	add operation to erase a tab
 * @num_chars: number of character columns already used
 * @after_tab: true if num_chars starts after a previous tab
 * @ldata: n_tty data
 *
 * Add an operation to the echo buffer to erase a tab.
 *
 * Called by the eraser function, which knows how many character columns have
 * been used since either a previous tab or the start of input. This
 * information will be used later, along with canon column (if applicable), to
 * go back the correct number of columns.
 */
static void echo_erase_tab(unsigned int num_chars, int after_tab,
			   struct n_tty_data *ldata)
{
	add_echo_byte(ECHO_OP_START, ldata);
	add_echo_byte(ECHO_OP_ERASE_TAB, ldata);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* We only need to know this modulo 8 (tab spacing) */
	num_chars &= 7;

	/* Set the high bit as a flag if num_chars is after a previous tab */
	if (after_tab)
		num_chars |= 0x80;

<<<<<<< HEAD
	add_echo_byte(num_chars, tty);

	mutex_unlock(&tty->echo_lock);
}

/**
 *	echo_char_raw	-	echo a character raw
 *	@c: unicode byte to echo
 *	@tty: terminal device
 *
 *	Echo user input back onto the screen. This must be called only when
 *	L_ECHO(tty) is true. Called from the driver receive_buf path.
 *
 *	This variant does not treat control characters specially.
 *
 *	Locking: echo_lock to protect the echo buffer
 */

static void echo_char_raw(unsigned char c, struct tty_struct *tty)
{
	mutex_lock(&tty->echo_lock);

	if (c == ECHO_OP_START) {
		add_echo_byte(ECHO_OP_START, tty);
		add_echo_byte(ECHO_OP_START, tty);
	} else {
		add_echo_byte(c, tty);
	}

	mutex_unlock(&tty->echo_lock);
}

/**
 *	echo_char	-	echo a character
 *	@c: unicode byte to echo
 *	@tty: terminal device
 *
 *	Echo user input back onto the screen. This must be called only when
 *	L_ECHO(tty) is true. Called from the driver receive_buf path.
 *
 *	This variant tags control characters to be echoed as "^X"
 *	(where X is the letter representing the control char).
 *
 *	Locking: echo_lock to protect the echo buffer
 */

static void echo_char(unsigned char c, struct tty_struct *tty)
{
	mutex_lock(&tty->echo_lock);

	if (c == ECHO_OP_START) {
		add_echo_byte(ECHO_OP_START, tty);
		add_echo_byte(ECHO_OP_START, tty);
	} else {
		if (L_ECHOCTL(tty) && iscntrl(c) && c != '\t')
			add_echo_byte(ECHO_OP_START, tty);
		add_echo_byte(c, tty);
	}

	mutex_unlock(&tty->echo_lock);
}

/**
 *	finish_erasing		-	complete erase
 *	@tty: tty doing the erase
 */

static inline void finish_erasing(struct tty_struct *tty)
{
	if (tty->erasing) {
		echo_char_raw('/', tty);
		tty->erasing = 0;
=======
	add_echo_byte(num_chars, ldata);
}

/**
 * echo_char_raw	-	echo a character raw
 * @c: unicode byte to echo
 * @ldata: line disc data
 *
 * Echo user input back onto the screen. This must be called only when
 * L_ECHO(tty) is true. Called from the &tty_driver.receive_buf() path.
 *
 * This variant does not treat control characters specially.
 */
static void echo_char_raw(u8 c, struct n_tty_data *ldata)
{
	if (c == ECHO_OP_START) {
		add_echo_byte(ECHO_OP_START, ldata);
		add_echo_byte(ECHO_OP_START, ldata);
	} else {
		add_echo_byte(c, ldata);
	}
}

/**
 * echo_char		-	echo a character
 * @c: unicode byte to echo
 * @tty: terminal device
 *
 * Echo user input back onto the screen. This must be called only when
 * L_ECHO(tty) is true. Called from the &tty_driver.receive_buf() path.
 *
 * This variant tags control characters to be echoed as "^X" (where X is the
 * letter representing the control char).
 */
static void echo_char(u8 c, const struct tty_struct *tty)
{
	struct n_tty_data *ldata = tty->disc_data;

	if (c == ECHO_OP_START) {
		add_echo_byte(ECHO_OP_START, ldata);
		add_echo_byte(ECHO_OP_START, ldata);
	} else {
		if (L_ECHOCTL(tty) && iscntrl(c) && c != '\t')
			add_echo_byte(ECHO_OP_START, ldata);
		add_echo_byte(c, ldata);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

/**
<<<<<<< HEAD
 *	eraser		-	handle erase function
 *	@c: character input
 *	@tty: terminal device
 *
 *	Perform erase and necessary output when an erase character is
 *	present in the stream from the driver layer. Handles the complexities
 *	of UTF-8 multibyte symbols.
 *
 *	Locking: read_lock for tty buffers
 */

static void eraser(unsigned char c, struct tty_struct *tty)
{
	enum { ERASE, WERASE, KILL } kill_type;
	int head, seen_alnums, cnt;
	unsigned long flags;

	/* FIXME: locking needed ? */
	if (tty->read_head == tty->canon_head) {
=======
 * finish_erasing	-	complete erase
 * @ldata: n_tty data
 */
static inline void finish_erasing(struct n_tty_data *ldata)
{
	if (ldata->erasing) {
		echo_char_raw('/', ldata);
		ldata->erasing = 0;
	}
}

/**
 * eraser		-	handle erase function
 * @c: character input
 * @tty: terminal device
 *
 * Perform erase and necessary output when an erase character is present in the
 * stream from the driver layer. Handles the complexities of UTF-8 multibyte
 * symbols.
 *
 * Locking: n_tty_receive_buf()/producer path:
 *	caller holds non-exclusive %termios_rwsem
 */
static void eraser(u8 c, const struct tty_struct *tty)
{
	struct n_tty_data *ldata = tty->disc_data;
	enum { ERASE, WERASE, KILL } kill_type;
	size_t head;
	size_t cnt;
	int seen_alnums;

	if (ldata->read_head == ldata->canon_head) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* process_output('\a', tty); */ /* what do you think? */
		return;
	}
	if (c == ERASE_CHAR(tty))
		kill_type = ERASE;
	else if (c == WERASE_CHAR(tty))
		kill_type = WERASE;
	else {
		if (!L_ECHO(tty)) {
<<<<<<< HEAD
			spin_lock_irqsave(&tty->read_lock, flags);
			tty->read_cnt -= ((tty->read_head - tty->canon_head) &
					  (N_TTY_BUF_SIZE - 1));
			tty->read_head = tty->canon_head;
			spin_unlock_irqrestore(&tty->read_lock, flags);
			return;
		}
		if (!L_ECHOK(tty) || !L_ECHOKE(tty) || !L_ECHOE(tty)) {
			spin_lock_irqsave(&tty->read_lock, flags);
			tty->read_cnt -= ((tty->read_head - tty->canon_head) &
					  (N_TTY_BUF_SIZE - 1));
			tty->read_head = tty->canon_head;
			spin_unlock_irqrestore(&tty->read_lock, flags);
			finish_erasing(tty);
			echo_char(KILL_CHAR(tty), tty);
			/* Add a newline if ECHOK is on and ECHOKE is off. */
			if (L_ECHOK(tty))
				echo_char_raw('\n', tty);
=======
			ldata->read_head = ldata->canon_head;
			return;
		}
		if (!L_ECHOK(tty) || !L_ECHOKE(tty) || !L_ECHOE(tty)) {
			ldata->read_head = ldata->canon_head;
			finish_erasing(ldata);
			echo_char(KILL_CHAR(tty), tty);
			/* Add a newline if ECHOK is on and ECHOKE is off. */
			if (L_ECHOK(tty))
				echo_char_raw('\n', ldata);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return;
		}
		kill_type = KILL;
	}

	seen_alnums = 0;
<<<<<<< HEAD
	/* FIXME: Locking ?? */
	while (tty->read_head != tty->canon_head) {
		head = tty->read_head;

		/* erase a single possibly multibyte character */
		do {
			head = (head - 1) & (N_TTY_BUF_SIZE-1);
			c = tty->read_buf[head];
		} while (is_continuation(c, tty) && head != tty->canon_head);
=======
	while (MASK(ldata->read_head) != MASK(ldata->canon_head)) {
		head = ldata->read_head;

		/* erase a single possibly multibyte character */
		do {
			head--;
			c = read_buf(ldata, head);
		} while (is_continuation(c, tty) &&
			 MASK(head) != MASK(ldata->canon_head));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/* do not partially erase */
		if (is_continuation(c, tty))
			break;

		if (kill_type == WERASE) {
			/* Equivalent to BSD's ALTWERASE. */
			if (isalnum(c) || c == '_')
				seen_alnums++;
			else if (seen_alnums)
				break;
		}
<<<<<<< HEAD
		cnt = (tty->read_head - head) & (N_TTY_BUF_SIZE-1);
		spin_lock_irqsave(&tty->read_lock, flags);
		tty->read_head = head;
		tty->read_cnt -= cnt;
		spin_unlock_irqrestore(&tty->read_lock, flags);
		if (L_ECHO(tty)) {
			if (L_ECHOPRT(tty)) {
				if (!tty->erasing) {
					echo_char_raw('\\', tty);
					tty->erasing = 1;
=======
		cnt = ldata->read_head - head;
		ldata->read_head = head;
		if (L_ECHO(tty)) {
			if (L_ECHOPRT(tty)) {
				if (!ldata->erasing) {
					echo_char_raw('\\', ldata);
					ldata->erasing = 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				}
				/* if cnt > 1, output a multi-byte character */
				echo_char(c, tty);
				while (--cnt > 0) {
<<<<<<< HEAD
					head = (head+1) & (N_TTY_BUF_SIZE-1);
					echo_char_raw(tty->read_buf[head], tty);
					echo_move_back_col(tty);
=======
					head++;
					echo_char_raw(read_buf(ldata, head), ldata);
					echo_move_back_col(ldata);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				}
			} else if (kill_type == ERASE && !L_ECHOE(tty)) {
				echo_char(ERASE_CHAR(tty), tty);
			} else if (c == '\t') {
				unsigned int num_chars = 0;
				int after_tab = 0;
<<<<<<< HEAD
				unsigned long tail = tty->read_head;
=======
				size_t tail = ldata->read_head;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

				/*
				 * Count the columns used for characters
				 * since the start of input or after a
				 * previous tab.
				 * This info is used to go back the correct
				 * number of columns.
				 */
<<<<<<< HEAD
				while (tail != tty->canon_head) {
					tail = (tail-1) & (N_TTY_BUF_SIZE-1);
					c = tty->read_buf[tail];
=======
				while (MASK(tail) != MASK(ldata->canon_head)) {
					tail--;
					c = read_buf(ldata, tail);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					if (c == '\t') {
						after_tab = 1;
						break;
					} else if (iscntrl(c)) {
						if (L_ECHOCTL(tty))
							num_chars += 2;
					} else if (!is_continuation(c, tty)) {
						num_chars++;
					}
				}
<<<<<<< HEAD
				echo_erase_tab(num_chars, after_tab, tty);
			} else {
				if (iscntrl(c) && L_ECHOCTL(tty)) {
					echo_char_raw('\b', tty);
					echo_char_raw(' ', tty);
					echo_char_raw('\b', tty);
				}
				if (!iscntrl(c) || L_ECHOCTL(tty)) {
					echo_char_raw('\b', tty);
					echo_char_raw(' ', tty);
					echo_char_raw('\b', tty);
=======
				echo_erase_tab(num_chars, after_tab, ldata);
			} else {
				if (iscntrl(c) && L_ECHOCTL(tty)) {
					echo_char_raw('\b', ldata);
					echo_char_raw(' ', ldata);
					echo_char_raw('\b', ldata);
				}
				if (!iscntrl(c) || L_ECHOCTL(tty)) {
					echo_char_raw('\b', ldata);
					echo_char_raw(' ', ldata);
					echo_char_raw('\b', ldata);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				}
			}
		}
		if (kill_type == ERASE)
			break;
	}
<<<<<<< HEAD
	if (tty->read_head == tty->canon_head && L_ECHO(tty))
		finish_erasing(tty);
}

/**
 *	isig		-	handle the ISIG optio
 *	@sig: signal
 *	@tty: terminal
 *	@flush: force flush
 *
 *	Called when a signal is being sent due to terminal input. This
 *	may caus terminal flushing to take place according to the termios
 *	settings and character used. Called from the driver receive_buf
 *	path so serialized.
 *
 *	Locking: ctrl_lock, read_lock (both via flush buffer)
 */

static inline void isig(int sig, struct tty_struct *tty, int flush)
{
	if (tty->pgrp)
		kill_pgrp(tty->pgrp, sig, 1);
	if (flush || !L_NOFLSH(tty)) {
		n_tty_flush_buffer(tty);
		tty_driver_flush_buffer(tty);
=======
	if (ldata->read_head == ldata->canon_head && L_ECHO(tty))
		finish_erasing(ldata);
}


static void __isig(int sig, struct tty_struct *tty)
{
	struct pid *tty_pgrp = tty_get_pgrp(tty);
	if (tty_pgrp) {
		kill_pgrp(tty_pgrp, sig, 1);
		put_pid(tty_pgrp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

/**
<<<<<<< HEAD
 *	n_tty_receive_break	-	handle break
 *	@tty: terminal
 *
 *	An RS232 break event has been hit in the incoming bitstream. This
 *	can cause a variety of events depending upon the termios settings.
 *
 *	Called from the receive_buf path so single threaded.
 */

static inline void n_tty_receive_break(struct tty_struct *tty)
{
	if (I_IGNBRK(tty))
		return;
	if (I_BRKINT(tty)) {
		isig(SIGINT, tty, 1);
		return;
	}
	if (I_PARMRK(tty)) {
		put_tty_queue('\377', tty);
		put_tty_queue('\0', tty);
	}
	put_tty_queue('\0', tty);
	wake_up_interruptible(&tty->read_wait);
}

/**
 *	n_tty_receive_overrun	-	handle overrun reporting
 *	@tty: terminal
 *
 *	Data arrived faster than we could process it. While the tty
 *	driver has flagged this the bits that were missed are gone
 *	forever.
 *
 *	Called from the receive_buf path so single threaded. Does not
 *	need locking as num_overrun and overrun_time are function
 *	private.
 */

static inline void n_tty_receive_overrun(struct tty_struct *tty)
{
	char buf[64];

	tty->num_overrun++;
	if (time_before(tty->overrun_time, jiffies - HZ) ||
			time_after(tty->overrun_time, jiffies)) {
		printk(KERN_WARNING "%s: %d input overrun(s)\n",
			tty_name(tty, buf),
			tty->num_overrun);
		tty->overrun_time = jiffies;
		tty->num_overrun = 0;
=======
 * isig			-	handle the ISIG optio
 * @sig: signal
 * @tty: terminal
 *
 * Called when a signal is being sent due to terminal input. Called from the
 * &tty_driver.receive_buf() path, so serialized.
 *
 * Performs input and output flush if !NOFLSH. In this context, the echo
 * buffer is 'output'. The signal is processed first to alert any current
 * readers or writers to discontinue and exit their i/o loops.
 *
 * Locking: %ctrl.lock
 */
static void isig(int sig, struct tty_struct *tty)
{
	struct n_tty_data *ldata = tty->disc_data;

	if (L_NOFLSH(tty)) {
		/* signal only */
		__isig(sig, tty);

	} else { /* signal and flush */
		up_read(&tty->termios_rwsem);
		down_write(&tty->termios_rwsem);

		__isig(sig, tty);

		/* clear echo buffer */
		mutex_lock(&ldata->output_lock);
		ldata->echo_head = ldata->echo_tail = 0;
		ldata->echo_mark = ldata->echo_commit = 0;
		mutex_unlock(&ldata->output_lock);

		/* clear output buffer */
		tty_driver_flush_buffer(tty);

		/* clear input buffer */
		reset_buffer_flags(tty->disc_data);

		/* notify pty master of flush */
		if (tty->link)
			n_tty_packet_mode_flush(tty);

		up_write(&tty->termios_rwsem);
		down_read(&tty->termios_rwsem);
	}
}

/**
 * n_tty_receive_break	-	handle break
 * @tty: terminal
 *
 * An RS232 break event has been hit in the incoming bitstream. This can cause
 * a variety of events depending upon the termios settings.
 *
 * Locking: n_tty_receive_buf()/producer path:
 *	caller holds non-exclusive termios_rwsem
 *
 * Note: may get exclusive %termios_rwsem if flushing input buffer
 */
static void n_tty_receive_break(struct tty_struct *tty)
{
	struct n_tty_data *ldata = tty->disc_data;

	if (I_IGNBRK(tty))
		return;
	if (I_BRKINT(tty)) {
		isig(SIGINT, tty);
		return;
	}
	if (I_PARMRK(tty)) {
		put_tty_queue('\377', ldata);
		put_tty_queue('\0', ldata);
	}
	put_tty_queue('\0', ldata);
}

/**
 * n_tty_receive_overrun	-	handle overrun reporting
 * @tty: terminal
 *
 * Data arrived faster than we could process it. While the tty driver has
 * flagged this the bits that were missed are gone forever.
 *
 * Called from the receive_buf path so single threaded. Does not need locking
 * as num_overrun and overrun_time are function private.
 */
static void n_tty_receive_overrun(const struct tty_struct *tty)
{
	struct n_tty_data *ldata = tty->disc_data;

	ldata->num_overrun++;
	if (time_is_before_jiffies(ldata->overrun_time + HZ)) {
		tty_warn(tty, "%u input overrun(s)\n", ldata->num_overrun);
		ldata->overrun_time = jiffies;
		ldata->num_overrun = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

/**
<<<<<<< HEAD
 *	n_tty_receive_parity_error	-	error notifier
 *	@tty: terminal device
 *	@c: character
 *
 *	Process a parity error and queue the right data to indicate
 *	the error case if necessary. Locking as per n_tty_receive_buf.
 */
static inline void n_tty_receive_parity_error(struct tty_struct *tty,
					      unsigned char c)
{
	if (I_IGNPAR(tty))
		return;
	if (I_PARMRK(tty)) {
		put_tty_queue('\377', tty);
		put_tty_queue('\0', tty);
		put_tty_queue(c, tty);
	} else	if (I_INPCK(tty))
		put_tty_queue('\0', tty);
	else
		put_tty_queue(c, tty);
	wake_up_interruptible(&tty->read_wait);
}

/**
 *	n_tty_receive_char	-	perform processing
 *	@tty: terminal device
 *	@c: character
 *
 *	Process an individual character of input received from the driver.
 *	This is serialized with respect to itself by the rules for the
 *	driver above.
 */

static inline void n_tty_receive_char(struct tty_struct *tty, unsigned char c)
{
	unsigned long flags;
	int parmrk;

	if (tty->raw) {
		put_tty_queue(c, tty);
		return;
	}

	if (I_ISTRIP(tty))
		c &= 0x7f;
	if (I_IUCLC(tty) && L_IEXTEN(tty))
		c = tolower(c);

	if (L_EXTPROC(tty)) {
		put_tty_queue(c, tty);
		return;
	}

	if (tty->stopped && !tty->flow_stopped && I_IXON(tty) &&
	    I_IXANY(tty) && c != START_CHAR(tty) && c != STOP_CHAR(tty) &&
	    c != INTR_CHAR(tty) && c != QUIT_CHAR(tty) && c != SUSP_CHAR(tty)) {
		start_tty(tty);
		process_echoes(tty);
	}

	if (tty->closing) {
		if (I_IXON(tty)) {
			if (c == START_CHAR(tty)) {
				start_tty(tty);
				process_echoes(tty);
			} else if (c == STOP_CHAR(tty))
				stop_tty(tty);
		}
		return;
	}

	/*
	 * If the previous character was LNEXT, or we know that this
	 * character is not one of the characters that we'll have to
	 * handle specially, do shortcut processing to speed things
	 * up.
	 */
	if (!test_bit(c, tty->process_char_map) || tty->lnext) {
		tty->lnext = 0;
		parmrk = (c == (unsigned char) '\377' && I_PARMRK(tty)) ? 1 : 0;
		if (tty->read_cnt >= (N_TTY_BUF_SIZE - parmrk - 1)) {
			/* beep if no space */
			if (L_ECHO(tty))
				process_output('\a', tty);
			return;
		}
		if (L_ECHO(tty)) {
			finish_erasing(tty);
			/* Record the column of first canon char. */
			if (tty->canon_head == tty->read_head)
				echo_set_canon_col(tty);
			echo_char(c, tty);
			process_echoes(tty);
		}
		if (parmrk)
			put_tty_queue(c, tty);
		put_tty_queue(c, tty);
		return;
	}

	if (I_IXON(tty)) {
		if (c == START_CHAR(tty)) {
			start_tty(tty);
			process_echoes(tty);
			return;
		}
		if (c == STOP_CHAR(tty)) {
			stop_tty(tty);
			return;
		}
	}

	if (L_ISIG(tty)) {
		int signal;
		signal = SIGINT;
		if (c == INTR_CHAR(tty))
			goto send_signal;
		signal = SIGQUIT;
		if (c == QUIT_CHAR(tty))
			goto send_signal;
		signal = SIGTSTP;
		if (c == SUSP_CHAR(tty)) {
send_signal:
			/*
			 * Note that we do not use isig() here because we want
			 * the order to be:
			 * 1) flush, 2) echo, 3) signal
			 */
			if (!L_NOFLSH(tty)) {
				n_tty_flush_buffer(tty);
				tty_driver_flush_buffer(tty);
			}
			if (I_IXON(tty))
				start_tty(tty);
			if (L_ECHO(tty)) {
				echo_char(c, tty);
				process_echoes(tty);
			}
			if (tty->pgrp)
				kill_pgrp(tty->pgrp, signal, 1);
=======
 * n_tty_receive_parity_error	-	error notifier
 * @tty: terminal device
 * @c: character
 *
 * Process a parity error and queue the right data to indicate the error case
 * if necessary.
 *
 * Locking: n_tty_receive_buf()/producer path:
 * 	caller holds non-exclusive %termios_rwsem
 */
static void n_tty_receive_parity_error(const struct tty_struct *tty,
				       u8 c)
{
	struct n_tty_data *ldata = tty->disc_data;

	if (I_INPCK(tty)) {
		if (I_IGNPAR(tty))
			return;
		if (I_PARMRK(tty)) {
			put_tty_queue('\377', ldata);
			put_tty_queue('\0', ldata);
			put_tty_queue(c, ldata);
		} else
			put_tty_queue('\0', ldata);
	} else
		put_tty_queue(c, ldata);
}

static void
n_tty_receive_signal_char(struct tty_struct *tty, int signal, u8 c)
{
	isig(signal, tty);
	if (I_IXON(tty))
		start_tty(tty);
	if (L_ECHO(tty)) {
		echo_char(c, tty);
		commit_echoes(tty);
	} else
		process_echoes(tty);
}

static bool n_tty_is_char_flow_ctrl(struct tty_struct *tty, u8 c)
{
	return c == START_CHAR(tty) || c == STOP_CHAR(tty);
}

/**
 * n_tty_receive_char_flow_ctrl - receive flow control chars
 * @tty: terminal device
 * @c: character
 * @lookahead_done: lookahead has processed this character already
 *
 * Receive and process flow control character actions.
 *
 * In case lookahead for flow control chars already handled the character in
 * advance to the normal receive, the actions are skipped during normal
 * receive.
 *
 * Returns true if @c is consumed as flow-control character, the character
 * must not be treated as normal character.
 */
static bool n_tty_receive_char_flow_ctrl(struct tty_struct *tty, u8 c,
					 bool lookahead_done)
{
	if (!n_tty_is_char_flow_ctrl(tty, c))
		return false;

	if (lookahead_done)
		return true;

	if (c == START_CHAR(tty)) {
		start_tty(tty);
		process_echoes(tty);
		return true;
	}

	/* STOP_CHAR */
	stop_tty(tty);
	return true;
}

static void n_tty_receive_handle_newline(struct tty_struct *tty, u8 c)
{
	struct n_tty_data *ldata = tty->disc_data;

	set_bit(MASK(ldata->read_head), ldata->read_flags);
	put_tty_queue(c, ldata);
	smp_store_release(&ldata->canon_head, ldata->read_head);
	kill_fasync(&tty->fasync, SIGIO, POLL_IN);
	wake_up_interruptible_poll(&tty->read_wait, EPOLLIN | EPOLLRDNORM);
}

static bool n_tty_receive_char_canon(struct tty_struct *tty, u8 c)
{
	struct n_tty_data *ldata = tty->disc_data;

	if (c == ERASE_CHAR(tty) || c == KILL_CHAR(tty) ||
	    (c == WERASE_CHAR(tty) && L_IEXTEN(tty))) {
		eraser(c, tty);
		commit_echoes(tty);

		return true;
	}

	if (c == LNEXT_CHAR(tty) && L_IEXTEN(tty)) {
		ldata->lnext = 1;
		if (L_ECHO(tty)) {
			finish_erasing(ldata);
			if (L_ECHOCTL(tty)) {
				echo_char_raw('^', ldata);
				echo_char_raw('\b', ldata);
				commit_echoes(tty);
			}
		}

		return true;
	}

	if (c == REPRINT_CHAR(tty) && L_ECHO(tty) && L_IEXTEN(tty)) {
		size_t tail = ldata->canon_head;

		finish_erasing(ldata);
		echo_char(c, tty);
		echo_char_raw('\n', ldata);
		while (MASK(tail) != MASK(ldata->read_head)) {
			echo_char(read_buf(ldata, tail), tty);
			tail++;
		}
		commit_echoes(tty);

		return true;
	}

	if (c == '\n') {
		if (L_ECHO(tty) || L_ECHONL(tty)) {
			echo_char_raw('\n', ldata);
			commit_echoes(tty);
		}
		n_tty_receive_handle_newline(tty, c);

		return true;
	}

	if (c == EOF_CHAR(tty)) {
		c = __DISABLED_CHAR;
		n_tty_receive_handle_newline(tty, c);

		return true;
	}

	if ((c == EOL_CHAR(tty)) ||
	    (c == EOL2_CHAR(tty) && L_IEXTEN(tty))) {
		/*
		 * XXX are EOL_CHAR and EOL2_CHAR echoed?!?
		 */
		if (L_ECHO(tty)) {
			/* Record the column of first canon char. */
			if (ldata->canon_head == ldata->read_head)
				echo_set_canon_col(ldata);
			echo_char(c, tty);
			commit_echoes(tty);
		}
		/*
		 * XXX does PARMRK doubling happen for
		 * EOL_CHAR and EOL2_CHAR?
		 */
		if (c == '\377' && I_PARMRK(tty))
			put_tty_queue(c, ldata);

		n_tty_receive_handle_newline(tty, c);

		return true;
	}

	return false;
}

static void n_tty_receive_char_special(struct tty_struct *tty, u8 c,
				       bool lookahead_done)
{
	struct n_tty_data *ldata = tty->disc_data;

	if (I_IXON(tty) && n_tty_receive_char_flow_ctrl(tty, c, lookahead_done))
		return;

	if (L_ISIG(tty)) {
		if (c == INTR_CHAR(tty)) {
			n_tty_receive_signal_char(tty, SIGINT, c);
			return;
		} else if (c == QUIT_CHAR(tty)) {
			n_tty_receive_signal_char(tty, SIGQUIT, c);
			return;
		} else if (c == SUSP_CHAR(tty)) {
			n_tty_receive_signal_char(tty, SIGTSTP, c);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return;
		}
	}

<<<<<<< HEAD
=======
	if (tty->flow.stopped && !tty->flow.tco_stopped && I_IXON(tty) && I_IXANY(tty)) {
		start_tty(tty);
		process_echoes(tty);
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (c == '\r') {
		if (I_IGNCR(tty))
			return;
		if (I_ICRNL(tty))
			c = '\n';
	} else if (c == '\n' && I_INLCR(tty))
		c = '\r';

<<<<<<< HEAD
	if (tty->icanon) {
		if (c == ERASE_CHAR(tty) || c == KILL_CHAR(tty) ||
		    (c == WERASE_CHAR(tty) && L_IEXTEN(tty))) {
			eraser(c, tty);
			process_echoes(tty);
			return;
		}
		if (c == LNEXT_CHAR(tty) && L_IEXTEN(tty)) {
			tty->lnext = 1;
			if (L_ECHO(tty)) {
				finish_erasing(tty);
				if (L_ECHOCTL(tty)) {
					echo_char_raw('^', tty);
					echo_char_raw('\b', tty);
					process_echoes(tty);
				}
			}
			return;
		}
		if (c == REPRINT_CHAR(tty) && L_ECHO(tty) &&
		    L_IEXTEN(tty)) {
			unsigned long tail = tty->canon_head;

			finish_erasing(tty);
			echo_char(c, tty);
			echo_char_raw('\n', tty);
			while (tail != tty->read_head) {
				echo_char(tty->read_buf[tail], tty);
				tail = (tail+1) & (N_TTY_BUF_SIZE-1);
			}
			process_echoes(tty);
			return;
		}
		if (c == '\n') {
			if (tty->read_cnt >= N_TTY_BUF_SIZE) {
				if (L_ECHO(tty))
					process_output('\a', tty);
				return;
			}
			if (L_ECHO(tty) || L_ECHONL(tty)) {
				echo_char_raw('\n', tty);
				process_echoes(tty);
			}
			goto handle_newline;
		}
		if (c == EOF_CHAR(tty)) {
			if (tty->read_cnt >= N_TTY_BUF_SIZE)
				return;
			if (tty->canon_head != tty->read_head)
				set_bit(TTY_PUSH, &tty->flags);
			c = __DISABLED_CHAR;
			goto handle_newline;
		}
		if ((c == EOL_CHAR(tty)) ||
		    (c == EOL2_CHAR(tty) && L_IEXTEN(tty))) {
			parmrk = (c == (unsigned char) '\377' && I_PARMRK(tty))
				 ? 1 : 0;
			if (tty->read_cnt >= (N_TTY_BUF_SIZE - parmrk)) {
				if (L_ECHO(tty))
					process_output('\a', tty);
				return;
			}
			/*
			 * XXX are EOL_CHAR and EOL2_CHAR echoed?!?
			 */
			if (L_ECHO(tty)) {
				/* Record the column of first canon char. */
				if (tty->canon_head == tty->read_head)
					echo_set_canon_col(tty);
				echo_char(c, tty);
				process_echoes(tty);
			}
			/*
			 * XXX does PARMRK doubling happen for
			 * EOL_CHAR and EOL2_CHAR?
			 */
			if (parmrk)
				put_tty_queue(c, tty);

handle_newline:
			spin_lock_irqsave(&tty->read_lock, flags);
			set_bit(tty->read_head, tty->read_flags);
			put_tty_queue_nolock(c, tty);
			tty->canon_head = tty->read_head;
			tty->canon_data++;
			spin_unlock_irqrestore(&tty->read_lock, flags);
			kill_fasync(&tty->fasync, SIGIO, POLL_IN);
			wake_up_interruptible(&tty->read_wait);
			return;
		}
	}

	parmrk = (c == (unsigned char) '\377' && I_PARMRK(tty)) ? 1 : 0;
	if (tty->read_cnt >= (N_TTY_BUF_SIZE - parmrk - 1)) {
		/* beep if no space */
		if (L_ECHO(tty))
			process_output('\a', tty);
		return;
	}
	if (L_ECHO(tty)) {
		finish_erasing(tty);
		if (c == '\n')
			echo_char_raw('\n', tty);
		else {
			/* Record the column of first canon char. */
			if (tty->canon_head == tty->read_head)
				echo_set_canon_col(tty);
			echo_char(c, tty);
		}
		process_echoes(tty);
	}

	if (parmrk)
		put_tty_queue(c, tty);

	put_tty_queue(c, tty);
}


/**
 *	n_tty_write_wakeup	-	asynchronous I/O notifier
 *	@tty: tty device
 *
 *	Required for the ptys, serial driver etc. since processes
 *	that attach themselves to the master and rely on ASYNC
 *	IO must be woken up
 */

static void n_tty_write_wakeup(struct tty_struct *tty)
{
	if (tty->fasync && test_and_clear_bit(TTY_DO_WRITE_WAKEUP, &tty->flags))
		kill_fasync(&tty->fasync, SIGIO, POLL_OUT);
}

/**
 *	n_tty_receive_buf	-	data receive
 *	@tty: terminal device
 *	@cp: buffer
 *	@fp: flag buffer
 *	@count: characters
 *
 *	Called by the terminal driver when a block of characters has
 *	been received. This function must be called from soft contexts
 *	not from interrupt context. The driver is responsible for making
 *	calls one at a time and in order (or using flush_to_ldisc)
 */

static void n_tty_receive_buf(struct tty_struct *tty, const unsigned char *cp,
			      char *fp, int count)
{
	const unsigned char *p;
	char *f, flags = TTY_NORMAL;
	int	i;
	char	buf[64];
	unsigned long cpuflags;

	if (!tty->read_buf)
		return;

	if (tty->real_raw) {
		spin_lock_irqsave(&tty->read_lock, cpuflags);
		i = min(N_TTY_BUF_SIZE - tty->read_cnt,
			N_TTY_BUF_SIZE - tty->read_head);
		i = min(count, i);
		memcpy(tty->read_buf + tty->read_head, cp, i);
		tty->read_head = (tty->read_head + i) & (N_TTY_BUF_SIZE-1);
		tty->read_cnt += i;
		cp += i;
		count -= i;

		i = min(N_TTY_BUF_SIZE - tty->read_cnt,
			N_TTY_BUF_SIZE - tty->read_head);
		i = min(count, i);
		memcpy(tty->read_buf + tty->read_head, cp, i);
		tty->read_head = (tty->read_head + i) & (N_TTY_BUF_SIZE-1);
		tty->read_cnt += i;
		spin_unlock_irqrestore(&tty->read_lock, cpuflags);
	} else {
		for (i = count, p = cp, f = fp; i; i--, p++) {
			if (f)
				flags = *f++;
			switch (flags) {
			case TTY_NORMAL:
				n_tty_receive_char(tty, *p);
				break;
			case TTY_BREAK:
				n_tty_receive_break(tty);
				break;
			case TTY_PARITY:
			case TTY_FRAME:
				n_tty_receive_parity_error(tty, *p);
				break;
			case TTY_OVERRUN:
				n_tty_receive_overrun(tty);
				break;
			default:
				printk(KERN_ERR "%s: unknown flag %d\n",
				       tty_name(tty, buf), flags);
				break;
			}
		}
=======
	if (ldata->icanon && n_tty_receive_char_canon(tty, c))
		return;

	if (L_ECHO(tty)) {
		finish_erasing(ldata);
		if (c == '\n')
			echo_char_raw('\n', ldata);
		else {
			/* Record the column of first canon char. */
			if (ldata->canon_head == ldata->read_head)
				echo_set_canon_col(ldata);
			echo_char(c, tty);
		}
		commit_echoes(tty);
	}

	/* PARMRK doubling check */
	if (c == '\377' && I_PARMRK(tty))
		put_tty_queue(c, ldata);

	put_tty_queue(c, ldata);
}

/**
 * n_tty_receive_char	-	perform processing
 * @tty: terminal device
 * @c: character
 *
 * Process an individual character of input received from the driver.  This is
 * serialized with respect to itself by the rules for the driver above.
 *
 * Locking: n_tty_receive_buf()/producer path:
 *	caller holds non-exclusive %termios_rwsem
 *	publishes canon_head if canonical mode is active
 */
static void n_tty_receive_char(struct tty_struct *tty, u8 c)
{
	struct n_tty_data *ldata = tty->disc_data;

	if (tty->flow.stopped && !tty->flow.tco_stopped && I_IXON(tty) && I_IXANY(tty)) {
		start_tty(tty);
		process_echoes(tty);
	}
	if (L_ECHO(tty)) {
		finish_erasing(ldata);
		/* Record the column of first canon char. */
		if (ldata->canon_head == ldata->read_head)
			echo_set_canon_col(ldata);
		echo_char(c, tty);
		commit_echoes(tty);
	}
	/* PARMRK doubling check */
	if (c == '\377' && I_PARMRK(tty))
		put_tty_queue(c, ldata);
	put_tty_queue(c, ldata);
}

static void n_tty_receive_char_closing(struct tty_struct *tty, u8 c,
				       bool lookahead_done)
{
	if (I_ISTRIP(tty))
		c &= 0x7f;
	if (I_IUCLC(tty) && L_IEXTEN(tty))
		c = tolower(c);

	if (I_IXON(tty)) {
		if (!n_tty_receive_char_flow_ctrl(tty, c, lookahead_done) &&
		    tty->flow.stopped && !tty->flow.tco_stopped && I_IXANY(tty) &&
		    c != INTR_CHAR(tty) && c != QUIT_CHAR(tty) &&
		    c != SUSP_CHAR(tty)) {
			start_tty(tty);
			process_echoes(tty);
		}
	}
}

static void
n_tty_receive_char_flagged(struct tty_struct *tty, u8 c, u8 flag)
{
	switch (flag) {
	case TTY_BREAK:
		n_tty_receive_break(tty);
		break;
	case TTY_PARITY:
	case TTY_FRAME:
		n_tty_receive_parity_error(tty, c);
		break;
	case TTY_OVERRUN:
		n_tty_receive_overrun(tty);
		break;
	default:
		tty_err(tty, "unknown flag %u\n", flag);
		break;
	}
}

static void
n_tty_receive_char_lnext(struct tty_struct *tty, u8 c, u8 flag)
{
	struct n_tty_data *ldata = tty->disc_data;

	ldata->lnext = 0;
	if (likely(flag == TTY_NORMAL)) {
		if (I_ISTRIP(tty))
			c &= 0x7f;
		if (I_IUCLC(tty) && L_IEXTEN(tty))
			c = tolower(c);
		n_tty_receive_char(tty, c);
	} else
		n_tty_receive_char_flagged(tty, c, flag);
}

/* Caller must ensure count > 0 */
static void n_tty_lookahead_flow_ctrl(struct tty_struct *tty, const u8 *cp,
				      const u8 *fp, size_t count)
{
	struct n_tty_data *ldata = tty->disc_data;
	u8 flag = TTY_NORMAL;

	ldata->lookahead_count += count;

	if (!I_IXON(tty))
		return;

	while (count--) {
		if (fp)
			flag = *fp++;
		if (likely(flag == TTY_NORMAL))
			n_tty_receive_char_flow_ctrl(tty, *cp, false);
		cp++;
	}
}

static void
n_tty_receive_buf_real_raw(const struct tty_struct *tty, const u8 *cp,
			   size_t count)
{
	struct n_tty_data *ldata = tty->disc_data;

	/* handle buffer wrap-around by a loop */
	for (unsigned int i = 0; i < 2; i++) {
		size_t head = MASK(ldata->read_head);
		size_t n = min(count, N_TTY_BUF_SIZE - head);

		memcpy(read_buf_addr(ldata, head), cp, n);

		ldata->read_head += n;
		cp += n;
		count -= n;
	}
}

static void
n_tty_receive_buf_raw(struct tty_struct *tty, const u8 *cp, const u8 *fp,
		      size_t count)
{
	struct n_tty_data *ldata = tty->disc_data;
	u8 flag = TTY_NORMAL;

	while (count--) {
		if (fp)
			flag = *fp++;
		if (likely(flag == TTY_NORMAL))
			put_tty_queue(*cp++, ldata);
		else
			n_tty_receive_char_flagged(tty, *cp++, flag);
	}
}

static void
n_tty_receive_buf_closing(struct tty_struct *tty, const u8 *cp, const u8 *fp,
			  size_t count, bool lookahead_done)
{
	u8 flag = TTY_NORMAL;

	while (count--) {
		if (fp)
			flag = *fp++;
		if (likely(flag == TTY_NORMAL))
			n_tty_receive_char_closing(tty, *cp++, lookahead_done);
	}
}

static void n_tty_receive_buf_standard(struct tty_struct *tty, const u8 *cp,
				       const u8 *fp, size_t count,
				       bool lookahead_done)
{
	struct n_tty_data *ldata = tty->disc_data;
	u8 flag = TTY_NORMAL;

	while (count--) {
		u8 c = *cp++;

		if (fp)
			flag = *fp++;

		if (ldata->lnext) {
			n_tty_receive_char_lnext(tty, c, flag);
			continue;
		}

		if (unlikely(flag != TTY_NORMAL)) {
			n_tty_receive_char_flagged(tty, c, flag);
			continue;
		}

		if (I_ISTRIP(tty))
			c &= 0x7f;
		if (I_IUCLC(tty) && L_IEXTEN(tty))
			c = tolower(c);
		if (L_EXTPROC(tty)) {
			put_tty_queue(c, ldata);
			continue;
		}

		if (test_bit(c, ldata->char_map))
			n_tty_receive_char_special(tty, c, lookahead_done);
		else
			n_tty_receive_char(tty, c);
	}
}

static void __receive_buf(struct tty_struct *tty, const u8 *cp, const u8 *fp,
			  size_t count)
{
	struct n_tty_data *ldata = tty->disc_data;
	bool preops = I_ISTRIP(tty) || (I_IUCLC(tty) && L_IEXTEN(tty));
	size_t la_count = min(ldata->lookahead_count, count);

	if (ldata->real_raw)
		n_tty_receive_buf_real_raw(tty, cp, count);
	else if (ldata->raw || (L_EXTPROC(tty) && !preops))
		n_tty_receive_buf_raw(tty, cp, fp, count);
	else if (tty->closing && !L_EXTPROC(tty)) {
		if (la_count > 0)
			n_tty_receive_buf_closing(tty, cp, fp, la_count, true);
		if (count > la_count)
			n_tty_receive_buf_closing(tty, cp, fp, count - la_count, false);
	} else {
		if (la_count > 0)
			n_tty_receive_buf_standard(tty, cp, fp, la_count, true);
		if (count > la_count)
			n_tty_receive_buf_standard(tty, cp, fp, count - la_count, false);

		flush_echoes(tty);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (tty->ops->flush_chars)
			tty->ops->flush_chars(tty);
	}

<<<<<<< HEAD
	n_tty_set_room(tty);

	if ((!tty->icanon && (tty->read_cnt >= tty->minimum_to_wake)) ||
		L_EXTPROC(tty)) {
		kill_fasync(&tty->fasync, SIGIO, POLL_IN);
		wake_up_interruptible(&tty->read_wait);
	}

	/*
	 * Check the remaining room for the input canonicalization
	 * mode.  We don't want to throttle the driver if we're in
	 * canonical mode and don't have a newline yet!
	 */
	if (tty->receive_room < TTY_THRESHOLD_THROTTLE)
		tty_throttle(tty);
}

int is_ignored(int sig)
{
	return (sigismember(&current->blocked, sig) ||
		current->sighand->action[sig-1].sa.sa_handler == SIG_IGN);
}

/**
 *	n_tty_set_termios	-	termios data changed
 *	@tty: terminal
 *	@old: previous data
 *
 *	Called by the tty layer when the user changes termios flags so
 *	that the line discipline can plan ahead. This function cannot sleep
 *	and is protected from re-entry by the tty layer. The user is
 *	guaranteed that this function will not be re-entered or in progress
 *	when the ldisc is closed.
 *
 *	Locking: Caller holds tty->termios_mutex
 */

static void n_tty_set_termios(struct tty_struct *tty, struct ktermios *old)
{
	int canon_change = 1;
	BUG_ON(!tty);

	if (old)
		canon_change = (old->c_lflag ^ tty->termios->c_lflag) & ICANON;
	if (canon_change) {
		memset(&tty->read_flags, 0, sizeof tty->read_flags);
		tty->canon_head = tty->read_tail;
		tty->canon_data = 0;
		tty->erasing = 0;
	}

	if (canon_change && !L_ICANON(tty) && tty->read_cnt)
		wake_up_interruptible(&tty->read_wait);

	tty->icanon = (L_ICANON(tty) != 0);
	if (test_bit(TTY_HW_COOK_IN, &tty->flags)) {
		tty->raw = 1;
		tty->real_raw = 1;
		n_tty_set_room(tty);
		return;
	}
=======
	ldata->lookahead_count -= la_count;

	if (ldata->icanon && !L_EXTPROC(tty))
		return;

	/* publish read_head to consumer */
	smp_store_release(&ldata->commit_head, ldata->read_head);

	if (read_cnt(ldata)) {
		kill_fasync(&tty->fasync, SIGIO, POLL_IN);
		wake_up_interruptible_poll(&tty->read_wait, EPOLLIN | EPOLLRDNORM);
	}
}

/**
 * n_tty_receive_buf_common	-	process input
 * @tty: device to receive input
 * @cp: input chars
 * @fp: flags for each char (if %NULL, all chars are %TTY_NORMAL)
 * @count: number of input chars in @cp
 * @flow: enable flow control
 *
 * Called by the terminal driver when a block of characters has been received.
 * This function must be called from soft contexts not from interrupt context.
 * The driver is responsible for making calls one at a time and in order (or
 * using flush_to_ldisc()).
 *
 * Returns: the # of input chars from @cp which were processed.
 *
 * In canonical mode, the maximum line length is 4096 chars (including the line
 * termination char); lines longer than 4096 chars are truncated. After 4095
 * chars, input data is still processed but not stored. Overflow processing
 * ensures the tty can always receive more input until at least one line can be
 * read.
 *
 * In non-canonical mode, the read buffer will only accept 4095 chars; this
 * provides the necessary space for a newline char if the input mode is
 * switched to canonical.
 *
 * Note it is possible for the read buffer to _contain_ 4096 chars in
 * non-canonical mode: the read buffer could already contain the maximum canon
 * line of 4096 chars when the mode is switched to non-canonical.
 *
 * Locking: n_tty_receive_buf()/producer path:
 *	claims non-exclusive %termios_rwsem
 *	publishes commit_head or canon_head
 */
static size_t
n_tty_receive_buf_common(struct tty_struct *tty, const u8 *cp, const u8 *fp,
			 size_t count, bool flow)
{
	struct n_tty_data *ldata = tty->disc_data;
	size_t n, rcvd = 0;
	int room, overflow;

	down_read(&tty->termios_rwsem);

	do {
		/*
		 * When PARMRK is set, each input char may take up to 3 chars
		 * in the read buf; reduce the buffer space avail by 3x
		 *
		 * If we are doing input canonicalization, and there are no
		 * pending newlines, let characters through without limit, so
		 * that erase characters will be handled.  Other excess
		 * characters will be beeped.
		 *
		 * paired with store in *_copy_from_read_buf() -- guarantees
		 * the consumer has loaded the data in read_buf up to the new
		 * read_tail (so this producer will not overwrite unread data)
		 */
		size_t tail = smp_load_acquire(&ldata->read_tail);

		room = N_TTY_BUF_SIZE - (ldata->read_head - tail);
		if (I_PARMRK(tty))
			room = DIV_ROUND_UP(room, 3);
		room--;
		if (room <= 0) {
			overflow = ldata->icanon && ldata->canon_head == tail;
			if (overflow && room < 0)
				ldata->read_head--;
			room = overflow;
			WRITE_ONCE(ldata->no_room, flow && !room);
		} else
			overflow = 0;

		n = min_t(size_t, count, room);
		if (!n)
			break;

		/* ignore parity errors if handling overflow */
		if (!overflow || !fp || *fp != TTY_PARITY)
			__receive_buf(tty, cp, fp, n);

		cp += n;
		if (fp)
			fp += n;
		count -= n;
		rcvd += n;
	} while (!test_bit(TTY_LDISC_CHANGING, &tty->flags));

	tty->receive_room = room;

	/* Unthrottle if handling overflow on pty */
	if (tty->driver->type == TTY_DRIVER_TYPE_PTY) {
		if (overflow) {
			tty_set_flow_change(tty, TTY_UNTHROTTLE_SAFE);
			tty_unthrottle_safe(tty);
			__tty_set_flow_change(tty, 0);
		}
	} else
		n_tty_check_throttle(tty);

	if (unlikely(ldata->no_room)) {
		/*
		 * Barrier here is to ensure to read the latest read_tail in
		 * chars_in_buffer() and to make sure that read_tail is not loaded
		 * before ldata->no_room is set.
		 */
		smp_mb();
		if (!chars_in_buffer(tty))
			n_tty_kick_worker(tty);
	}

	up_read(&tty->termios_rwsem);

	return rcvd;
}

static void n_tty_receive_buf(struct tty_struct *tty, const u8 *cp,
			      const u8 *fp, size_t count)
{
	n_tty_receive_buf_common(tty, cp, fp, count, false);
}

static size_t n_tty_receive_buf2(struct tty_struct *tty, const u8 *cp,
				 const u8 *fp, size_t count)
{
	return n_tty_receive_buf_common(tty, cp, fp, count, true);
}

/**
 * n_tty_set_termios	-	termios data changed
 * @tty: terminal
 * @old: previous data
 *
 * Called by the tty layer when the user changes termios flags so that the line
 * discipline can plan ahead. This function cannot sleep and is protected from
 * re-entry by the tty layer. The user is guaranteed that this function will
 * not be re-entered or in progress when the ldisc is closed.
 *
 * Locking: Caller holds @tty->termios_rwsem
 */
static void n_tty_set_termios(struct tty_struct *tty, const struct ktermios *old)
{
	struct n_tty_data *ldata = tty->disc_data;

	if (!old || (old->c_lflag ^ tty->termios.c_lflag) & (ICANON | EXTPROC)) {
		bitmap_zero(ldata->read_flags, N_TTY_BUF_SIZE);
		ldata->line_start = ldata->read_tail;
		if (!L_ICANON(tty) || !read_cnt(ldata)) {
			ldata->canon_head = ldata->read_tail;
			ldata->push = 0;
		} else {
			set_bit(MASK(ldata->read_head - 1), ldata->read_flags);
			ldata->canon_head = ldata->read_head;
			ldata->push = 1;
		}
		ldata->commit_head = ldata->read_head;
		ldata->erasing = 0;
		ldata->lnext = 0;
	}

	ldata->icanon = (L_ICANON(tty) != 0);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (I_ISTRIP(tty) || I_IUCLC(tty) || I_IGNCR(tty) ||
	    I_ICRNL(tty) || I_INLCR(tty) || L_ICANON(tty) ||
	    I_IXON(tty) || L_ISIG(tty) || L_ECHO(tty) ||
	    I_PARMRK(tty)) {
<<<<<<< HEAD
		memset(tty->process_char_map, 0, 256/8);

		if (I_IGNCR(tty) || I_ICRNL(tty))
			set_bit('\r', tty->process_char_map);
		if (I_INLCR(tty))
			set_bit('\n', tty->process_char_map);

		if (L_ICANON(tty)) {
			set_bit(ERASE_CHAR(tty), tty->process_char_map);
			set_bit(KILL_CHAR(tty), tty->process_char_map);
			set_bit(EOF_CHAR(tty), tty->process_char_map);
			set_bit('\n', tty->process_char_map);
			set_bit(EOL_CHAR(tty), tty->process_char_map);
			if (L_IEXTEN(tty)) {
				set_bit(WERASE_CHAR(tty),
					tty->process_char_map);
				set_bit(LNEXT_CHAR(tty),
					tty->process_char_map);
				set_bit(EOL2_CHAR(tty),
					tty->process_char_map);
				if (L_ECHO(tty))
					set_bit(REPRINT_CHAR(tty),
						tty->process_char_map);
			}
		}
		if (I_IXON(tty)) {
			set_bit(START_CHAR(tty), tty->process_char_map);
			set_bit(STOP_CHAR(tty), tty->process_char_map);
		}
		if (L_ISIG(tty)) {
			set_bit(INTR_CHAR(tty), tty->process_char_map);
			set_bit(QUIT_CHAR(tty), tty->process_char_map);
			set_bit(SUSP_CHAR(tty), tty->process_char_map);
		}
		clear_bit(__DISABLED_CHAR, tty->process_char_map);
		tty->raw = 0;
		tty->real_raw = 0;
	} else {
		tty->raw = 1;
		if ((I_IGNBRK(tty) || (!I_BRKINT(tty) && !I_PARMRK(tty))) &&
		    (I_IGNPAR(tty) || !I_INPCK(tty)) &&
		    (tty->driver->flags & TTY_DRIVER_REAL_RAW))
			tty->real_raw = 1;
		else
			tty->real_raw = 0;
	}
	n_tty_set_room(tty);
=======
		bitmap_zero(ldata->char_map, 256);

		if (I_IGNCR(tty) || I_ICRNL(tty))
			set_bit('\r', ldata->char_map);
		if (I_INLCR(tty))
			set_bit('\n', ldata->char_map);

		if (L_ICANON(tty)) {
			set_bit(ERASE_CHAR(tty), ldata->char_map);
			set_bit(KILL_CHAR(tty), ldata->char_map);
			set_bit(EOF_CHAR(tty), ldata->char_map);
			set_bit('\n', ldata->char_map);
			set_bit(EOL_CHAR(tty), ldata->char_map);
			if (L_IEXTEN(tty)) {
				set_bit(WERASE_CHAR(tty), ldata->char_map);
				set_bit(LNEXT_CHAR(tty), ldata->char_map);
				set_bit(EOL2_CHAR(tty), ldata->char_map);
				if (L_ECHO(tty))
					set_bit(REPRINT_CHAR(tty),
						ldata->char_map);
			}
		}
		if (I_IXON(tty)) {
			set_bit(START_CHAR(tty), ldata->char_map);
			set_bit(STOP_CHAR(tty), ldata->char_map);
		}
		if (L_ISIG(tty)) {
			set_bit(INTR_CHAR(tty), ldata->char_map);
			set_bit(QUIT_CHAR(tty), ldata->char_map);
			set_bit(SUSP_CHAR(tty), ldata->char_map);
		}
		clear_bit(__DISABLED_CHAR, ldata->char_map);
		ldata->raw = 0;
		ldata->real_raw = 0;
	} else {
		ldata->raw = 1;
		if ((I_IGNBRK(tty) || (!I_BRKINT(tty) && !I_PARMRK(tty))) &&
		    (I_IGNPAR(tty) || !I_INPCK(tty)) &&
		    (tty->driver->flags & TTY_DRIVER_REAL_RAW))
			ldata->real_raw = 1;
		else
			ldata->real_raw = 0;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Fix tty hang when I_IXON(tty) is cleared, but the tty
	 * been stopped by STOP_CHAR(tty) before it.
	 */
<<<<<<< HEAD
	if (!I_IXON(tty) && old && (old->c_iflag & IXON) && !tty->flow_stopped) {
		start_tty(tty);
=======
	if (!I_IXON(tty) && old && (old->c_iflag & IXON) && !tty->flow.tco_stopped) {
		start_tty(tty);
		process_echoes(tty);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* The termios change make the tty ready for I/O */
	wake_up_interruptible(&tty->write_wait);
	wake_up_interruptible(&tty->read_wait);
}

/**
<<<<<<< HEAD
 *	n_tty_close		-	close the ldisc for this tty
 *	@tty: device
 *
 *	Called from the terminal layer when this line discipline is
 *	being shut down, either because of a close or becsuse of a
 *	discipline change. The function will not be called while other
 *	ldisc methods are in progress.
 */

static void n_tty_close(struct tty_struct *tty)
{
	n_tty_flush_buffer(tty);
	if (tty->read_buf) {
		kfree(tty->read_buf);
		tty->read_buf = NULL;
	}
	if (tty->echo_buf) {
		kfree(tty->echo_buf);
		tty->echo_buf = NULL;
	}
}

/**
 *	n_tty_open		-	open an ldisc
 *	@tty: terminal to open
 *
 *	Called when this line discipline is being attached to the
 *	terminal device. Can sleep. Called serialized so that no
 *	other events will occur in parallel. No further open will occur
 *	until a close.
 */

static int n_tty_open(struct tty_struct *tty)
{
	if (!tty)
		return -EINVAL;

	/* These are ugly. Currently a malloc failure here can panic */
	if (!tty->read_buf) {
		tty->read_buf = kzalloc(N_TTY_BUF_SIZE, GFP_KERNEL);
		if (!tty->read_buf)
			return -ENOMEM;
	}
	if (!tty->echo_buf) {
		tty->echo_buf = kzalloc(N_TTY_BUF_SIZE, GFP_KERNEL);

		if (!tty->echo_buf)
			return -ENOMEM;
	}
	reset_buffer_flags(tty);
	tty_unthrottle(tty);
	tty->column = 0;
	n_tty_set_termios(tty, NULL);
	tty->minimum_to_wake = 1;
	tty->closing = 0;
	return 0;
}

static inline int input_available_p(struct tty_struct *tty, int amt)
{
	tty_flush_to_ldisc(tty);
	if (tty->icanon && !L_EXTPROC(tty)) {
		if (tty->canon_data)
			return 1;
	} else if (tty->read_cnt >= (amt ? amt : 1))
		return 1;

	return 0;
}

/**
 *	copy_from_read_buf	-	copy read data directly
 *	@tty: terminal device
 *	@b: user data
 *	@nr: size of data
 *
 *	Helper function to speed up n_tty_read.  It is only called when
 *	ICANON is off; it copies characters straight from the tty queue to
 *	user space directly.  It can be profitably called twice; once to
 *	drain the space from the tail pointer to the (physical) end of the
 *	buffer, and once to drain the space from the (physical) beginning of
 *	the buffer to head pointer.
 *
 *	Called under the tty->atomic_read_lock sem
 *
 */

static int copy_from_read_buf(struct tty_struct *tty,
				      unsigned char __user **b,
				      size_t *nr)

{
	int retval;
	size_t n;
	unsigned long flags;

	retval = 0;
	spin_lock_irqsave(&tty->read_lock, flags);
	n = min(tty->read_cnt, N_TTY_BUF_SIZE - tty->read_tail);
	n = min(*nr, n);
	spin_unlock_irqrestore(&tty->read_lock, flags);
	if (n) {
		retval = copy_to_user(*b, &tty->read_buf[tty->read_tail], n);
		n -= retval;
		tty_audit_add_data(tty, &tty->read_buf[tty->read_tail], n);
		spin_lock_irqsave(&tty->read_lock, flags);
		tty->read_tail = (tty->read_tail + n) & (N_TTY_BUF_SIZE-1);
		tty->read_cnt -= n;
		/* Turn single EOF into zero-length read */
		if (L_EXTPROC(tty) && tty->icanon && n == 1) {
			if (!tty->read_cnt && (*b)[n-1] == EOF_CHAR(tty))
				n--;
		}
		spin_unlock_irqrestore(&tty->read_lock, flags);
		*b += n;
		*nr -= n;
	}
	return retval;
}

extern ssize_t redirected_tty_write(struct file *, const char __user *,
							size_t, loff_t *);

/**
 *	job_control		-	check job control
 *	@tty: tty
 *	@file: file handle
 *
 *	Perform job control management checks on this file/tty descriptor
 *	and if appropriate send any needed signals and return a negative
 *	error code if action should be taken.
 *
 *	FIXME:
 *	Locking: None - redirected write test is safe, testing
 *	current->signal should possibly lock current->sighand
 *	pgrp locking ?
 */

=======
 * n_tty_close		-	close the ldisc for this tty
 * @tty: device
 *
 * Called from the terminal layer when this line discipline is being shut down,
 * either because of a close or becsuse of a discipline change. The function
 * will not be called while other ldisc methods are in progress.
 */
static void n_tty_close(struct tty_struct *tty)
{
	struct n_tty_data *ldata = tty->disc_data;

	if (tty->link)
		n_tty_packet_mode_flush(tty);

	down_write(&tty->termios_rwsem);
	vfree(ldata);
	tty->disc_data = NULL;
	up_write(&tty->termios_rwsem);
}

/**
 * n_tty_open		-	open an ldisc
 * @tty: terminal to open
 *
 * Called when this line discipline is being attached to the terminal device.
 * Can sleep. Called serialized so that no other events will occur in parallel.
 * No further open will occur until a close.
 */
static int n_tty_open(struct tty_struct *tty)
{
	struct n_tty_data *ldata;

	/* Currently a malloc failure here can panic */
	ldata = vzalloc(sizeof(*ldata));
	if (!ldata)
		return -ENOMEM;

	ldata->overrun_time = jiffies;
	mutex_init(&ldata->atomic_read_lock);
	mutex_init(&ldata->output_lock);

	tty->disc_data = ldata;
	tty->closing = 0;
	/* indicate buffer work may resume */
	clear_bit(TTY_LDISC_HALTED, &tty->flags);
	n_tty_set_termios(tty, NULL);
	tty_unthrottle(tty);
	return 0;
}

static inline int input_available_p(const struct tty_struct *tty, int poll)
{
	const struct n_tty_data *ldata = tty->disc_data;
	int amt = poll && !TIME_CHAR(tty) && MIN_CHAR(tty) ? MIN_CHAR(tty) : 1;

	if (ldata->icanon && !L_EXTPROC(tty))
		return ldata->canon_head != ldata->read_tail;
	else
		return ldata->commit_head - ldata->read_tail >= amt;
}

/**
 * copy_from_read_buf	-	copy read data directly
 * @tty: terminal device
 * @kbp: data
 * @nr: size of data
 *
 * Helper function to speed up n_tty_read(). It is only called when %ICANON is
 * off; it copies characters straight from the tty queue.
 *
 * Returns: true if it successfully copied data, but there is still more data
 * to be had.
 *
 * Locking:
 *  * called under the @ldata->atomic_read_lock sem
 *  * n_tty_read()/consumer path:
 *		caller holds non-exclusive %termios_rwsem;
 *		read_tail published
 */
static bool copy_from_read_buf(const struct tty_struct *tty, u8 **kbp,
			       size_t *nr)

{
	struct n_tty_data *ldata = tty->disc_data;
	size_t n;
	bool is_eof;
	size_t head = smp_load_acquire(&ldata->commit_head);
	size_t tail = MASK(ldata->read_tail);

	n = min3(head - ldata->read_tail, N_TTY_BUF_SIZE - tail, *nr);
	if (!n)
		return false;

	u8 *from = read_buf_addr(ldata, tail);
	memcpy(*kbp, from, n);
	is_eof = n == 1 && *from == EOF_CHAR(tty);
	tty_audit_add_data(tty, from, n);
	zero_buffer(tty, from, n);
	smp_store_release(&ldata->read_tail, ldata->read_tail + n);

	/* Turn single EOF into zero-length read */
	if (L_EXTPROC(tty) && ldata->icanon && is_eof &&
	    head == ldata->read_tail)
		return false;

	*kbp += n;
	*nr -= n;

	/* If we have more to copy, let the caller know */
	return head != ldata->read_tail;
}

/**
 * canon_copy_from_read_buf	-	copy read data in canonical mode
 * @tty: terminal device
 * @kbp: data
 * @nr: size of data
 *
 * Helper function for n_tty_read(). It is only called when %ICANON is on; it
 * copies one line of input up to and including the line-delimiting character
 * into the result buffer.
 *
 * Note: When termios is changed from non-canonical to canonical mode and the
 * read buffer contains data, n_tty_set_termios() simulates an EOF push (as if
 * C-d were input) _without_ the %DISABLED_CHAR in the buffer. This causes data
 * already processed as input to be immediately available as input although a
 * newline has not been received.
 *
 * Locking:
 *  * called under the %atomic_read_lock mutex
 *  * n_tty_read()/consumer path:
 *	caller holds non-exclusive %termios_rwsem;
 *	read_tail published
 */
static bool canon_copy_from_read_buf(const struct tty_struct *tty, u8 **kbp,
				     size_t *nr)
{
	struct n_tty_data *ldata = tty->disc_data;
	size_t n, size, more, c;
	size_t eol;
	size_t tail, canon_head;
	int found = 0;

	/* N.B. avoid overrun if nr == 0 */
	if (!*nr)
		return false;

	canon_head = smp_load_acquire(&ldata->canon_head);
	n = min(*nr, canon_head - ldata->read_tail);

	tail = MASK(ldata->read_tail);
	size = min_t(size_t, tail + n, N_TTY_BUF_SIZE);

	n_tty_trace("%s: nr:%zu tail:%zu n:%zu size:%zu\n",
		    __func__, *nr, tail, n, size);

	eol = find_next_bit(ldata->read_flags, size, tail);
	more = n - (size - tail);
	if (eol == N_TTY_BUF_SIZE && more) {
		/* scan wrapped without finding set bit */
		eol = find_first_bit(ldata->read_flags, more);
		found = eol != more;
	} else
		found = eol != size;

	n = eol - tail;
	if (n > N_TTY_BUF_SIZE)
		n += N_TTY_BUF_SIZE;
	c = n + found;

	if (!found || read_buf(ldata, eol) != __DISABLED_CHAR)
		n = c;

	n_tty_trace("%s: eol:%zu found:%d n:%zu c:%zu tail:%zu more:%zu\n",
		    __func__, eol, found, n, c, tail, more);

	tty_copy(tty, *kbp, tail, n);
	*kbp += n;
	*nr -= n;

	if (found)
		clear_bit(eol, ldata->read_flags);
	smp_store_release(&ldata->read_tail, ldata->read_tail + c);

	if (found) {
		if (!ldata->push)
			ldata->line_start = ldata->read_tail;
		else
			ldata->push = 0;
		tty_audit_push();
		return false;
	}

	/* No EOL found - do a continuation retry if there is more data */
	return ldata->read_tail != canon_head;
}

/*
 * If we finished a read at the exact location of an
 * EOF (special EOL character that's a __DISABLED_CHAR)
 * in the stream, silently eat the EOF.
 */
static void canon_skip_eof(struct n_tty_data *ldata)
{
	size_t tail, canon_head;

	canon_head = smp_load_acquire(&ldata->canon_head);
	tail = ldata->read_tail;

	// No data?
	if (tail == canon_head)
		return;

	// See if the tail position is EOF in the circular buffer
	tail &= (N_TTY_BUF_SIZE - 1);
	if (!test_bit(tail, ldata->read_flags))
		return;
	if (read_buf(ldata, tail) != __DISABLED_CHAR)
		return;

	// Clear the EOL bit, skip the EOF char.
	clear_bit(tail, ldata->read_flags);
	smp_store_release(&ldata->read_tail, ldata->read_tail + 1);
}

/**
 * job_control		-	check job control
 * @tty: tty
 * @file: file handle
 *
 * Perform job control management checks on this @file/@tty descriptor and if
 * appropriate send any needed signals and return a negative error code if
 * action should be taken.
 *
 * Locking:
 *  * redirected write test is safe
 *  * current->signal->tty check is safe
 *  * ctrl.lock to safely reference @tty->ctrl.pgrp
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int job_control(struct tty_struct *tty, struct file *file)
{
	/* Job control check -- must be done at start and after
	   every sleep (POSIX.1 7.1.1.4). */
	/* NOTE: not yet done after every sleep pending a thorough
	   check of the logic of this change. -- jlc */
	/* don't stop on /dev/console */
<<<<<<< HEAD
	if (file->f_op->write != redirected_tty_write &&
	    current->signal->tty == tty) {
		if (!tty->pgrp)
			printk(KERN_ERR "n_tty_read: no tty->pgrp!\n");
		else if (task_pgrp(current) != tty->pgrp) {
			if (is_ignored(SIGTTIN) ||
			    is_current_pgrp_orphaned())
				return -EIO;
			kill_pgrp(task_pgrp(current), SIGTTIN, 1);
			set_thread_flag(TIF_SIGPENDING);
			return -ERESTARTSYS;
		}
	}
	return 0;
=======
	if (file->f_op->write_iter == redirected_tty_write)
		return 0;

	return __tty_check_change(tty, SIGTTIN);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}


/**
<<<<<<< HEAD
 *	n_tty_read		-	read function for tty
 *	@tty: tty device
 *	@file: file object
 *	@buf: userspace buffer pointer
 *	@nr: size of I/O
 *
 *	Perform reads for the line discipline. We are guaranteed that the
 *	line discipline will not be closed under us but we may get multiple
 *	parallel readers and must handle this ourselves. We may also get
 *	a hangup. Always called in user context, may sleep.
 *
 *	This code must be sure never to sleep through a hangup.
 */

static ssize_t n_tty_read(struct tty_struct *tty, struct file *file,
			 unsigned char __user *buf, size_t nr)
{
	unsigned char __user *b = buf;
	DECLARE_WAITQUEUE(wait, current);
	int c;
	int minimum, time;
	ssize_t retval = 0;
	ssize_t size;
	long timeout;
	unsigned long flags;
	int packet;

do_it_again:

	if (WARN_ON(!tty->read_buf))
		return -EAGAIN;

	c = job_control(tty, file);
	if (c < 0)
		return c;

	minimum = time = 0;
	timeout = MAX_SCHEDULE_TIMEOUT;
	if (!tty->icanon) {
		time = (HZ / 10) * TIME_CHAR(tty);
		minimum = MIN_CHAR(tty);
		if (minimum) {
			if (time)
				tty->minimum_to_wake = 1;
			else if (!waitqueue_active(&tty->read_wait) ||
				 (tty->minimum_to_wake > minimum))
				tty->minimum_to_wake = minimum;
		} else {
			timeout = 0;
			if (time) {
				timeout = time;
				time = 0;
			}
			tty->minimum_to_wake = minimum = 1;
		}
	}

=======
 * n_tty_read		-	read function for tty
 * @tty: tty device
 * @file: file object
 * @kbuf: kernelspace buffer pointer
 * @nr: size of I/O
 * @cookie: if non-%NULL, this is a continuation read
 * @offset: where to continue reading from (unused in n_tty)
 *
 * Perform reads for the line discipline. We are guaranteed that the line
 * discipline will not be closed under us but we may get multiple parallel
 * readers and must handle this ourselves. We may also get a hangup. Always
 * called in user context, may sleep.
 *
 * This code must be sure never to sleep through a hangup.
 *
 * Locking: n_tty_read()/consumer path:
 *	claims non-exclusive termios_rwsem;
 *	publishes read_tail
 */
static ssize_t n_tty_read(struct tty_struct *tty, struct file *file, u8 *kbuf,
			  size_t nr, void **cookie, unsigned long offset)
{
	struct n_tty_data *ldata = tty->disc_data;
	u8 *kb = kbuf;
	DEFINE_WAIT_FUNC(wait, woken_wake_function);
	int minimum, time;
	ssize_t retval;
	long timeout;
	bool packet;
	size_t old_tail;

	/*
	 * Is this a continuation of a read started earler?
	 *
	 * If so, we still hold the atomic_read_lock and the
	 * termios_rwsem, and can just continue to copy data.
	 */
	if (*cookie) {
		if (ldata->icanon && !L_EXTPROC(tty)) {
			/*
			 * If we have filled the user buffer, see
			 * if we should skip an EOF character before
			 * releasing the lock and returning done.
			 */
			if (!nr)
				canon_skip_eof(ldata);
			else if (canon_copy_from_read_buf(tty, &kb, &nr))
				return kb - kbuf;
		} else {
			if (copy_from_read_buf(tty, &kb, &nr))
				return kb - kbuf;
		}

		/* No more data - release locks and stop retries */
		n_tty_kick_worker(tty);
		n_tty_check_unthrottle(tty);
		up_read(&tty->termios_rwsem);
		mutex_unlock(&ldata->atomic_read_lock);
		*cookie = NULL;
		return kb - kbuf;
	}

	retval = job_control(tty, file);
	if (retval < 0)
		return retval;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 *	Internal serialization of reads.
	 */
	if (file->f_flags & O_NONBLOCK) {
<<<<<<< HEAD
		if (!mutex_trylock(&tty->atomic_read_lock))
			return -EAGAIN;
	} else {
		if (mutex_lock_interruptible(&tty->atomic_read_lock))
			return -ERESTARTSYS;
	}
	packet = tty->packet;
=======
		if (!mutex_trylock(&ldata->atomic_read_lock))
			return -EAGAIN;
	} else {
		if (mutex_lock_interruptible(&ldata->atomic_read_lock))
			return -ERESTARTSYS;
	}

	down_read(&tty->termios_rwsem);

	minimum = time = 0;
	timeout = MAX_SCHEDULE_TIMEOUT;
	if (!ldata->icanon) {
		minimum = MIN_CHAR(tty);
		if (minimum) {
			time = (HZ / 10) * TIME_CHAR(tty);
		} else {
			timeout = (HZ / 10) * TIME_CHAR(tty);
			minimum = 1;
		}
	}

	packet = tty->ctrl.packet;
	old_tail = ldata->read_tail;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	add_wait_queue(&tty->read_wait, &wait);
	while (nr) {
		/* First test for status change. */
<<<<<<< HEAD
		if (packet && tty->link->ctrl_status) {
			unsigned char cs;
			if (b != buf)
				break;
			spin_lock_irqsave(&tty->link->ctrl_lock, flags);
			cs = tty->link->ctrl_status;
			tty->link->ctrl_status = 0;
			spin_unlock_irqrestore(&tty->link->ctrl_lock, flags);
			if (tty_put_user(tty, cs, b++)) {
				retval = -EFAULT;
				b--;
				break;
			}
			nr--;
			break;
		}
		/* This statement must be first before checking for input
		   so that any interrupt will set the state back to
		   TASK_RUNNING. */
		set_current_state(TASK_INTERRUPTIBLE);

		if (((minimum - (b - buf)) < tty->minimum_to_wake) &&
		    ((minimum - (b - buf)) >= 1))
			tty->minimum_to_wake = (minimum - (b - buf));

		if (!input_available_p(tty, 0)) {
			if (test_bit(TTY_OTHER_CLOSED, &tty->flags)) {
				retval = -EIO;
				break;
			}
			if (tty_hung_up_p(file))
				break;
			if (!timeout)
				break;
			if (file->f_flags & O_NONBLOCK) {
				retval = -EAGAIN;
				break;
			}
			if (signal_pending(current)) {
				retval = -ERESTARTSYS;
				break;
			}
			n_tty_set_room(tty);
			timeout = schedule_timeout(timeout);
			BUG_ON(!tty->read_buf);
			continue;
		}
		__set_current_state(TASK_RUNNING);

		/* Deal with packet mode. */
		if (packet && b == buf) {
			if (tty_put_user(tty, TIOCPKT_DATA, b++)) {
				retval = -EFAULT;
				b--;
				break;
			}
			nr--;
		}

		if (tty->icanon && !L_EXTPROC(tty)) {
			/* N.B. avoid overrun if nr == 0 */
			while (nr && tty->read_cnt) {
				int eol;

				eol = test_and_clear_bit(tty->read_tail,
						tty->read_flags);
				c = tty->read_buf[tty->read_tail];
				spin_lock_irqsave(&tty->read_lock, flags);
				tty->read_tail = ((tty->read_tail+1) &
						  (N_TTY_BUF_SIZE-1));
				tty->read_cnt--;
				if (eol) {
					/* this test should be redundant:
					 * we shouldn't be reading data if
					 * canon_data is 0
					 */
					if (--tty->canon_data < 0)
						tty->canon_data = 0;
				}
				spin_unlock_irqrestore(&tty->read_lock, flags);

				if (!eol || (c != __DISABLED_CHAR)) {
					if (tty_put_user(tty, c, b++)) {
						retval = -EFAULT;
						b--;
						break;
					}
					nr--;
				}
				if (eol) {
					tty_audit_push(tty);
					break;
				}
			}
			if (retval)
				break;
		} else {
			int uncopied;
			/* The copy function takes the read lock and handles
			   locking internally for this case */
			uncopied = copy_from_read_buf(tty, &b, &nr);
			uncopied += copy_from_read_buf(tty, &b, &nr);
			if (uncopied) {
				retval = -EFAULT;
				break;
			}
		}

		/* If there is enough space in the read buffer now, let the
		 * low-level driver know. We use n_tty_chars_in_buffer() to
		 * check the buffer, as it now knows about canonical mode.
		 * Otherwise, if the driver is throttled and the line is
		 * longer than TTY_THRESHOLD_UNTHROTTLE in canonical mode,
		 * we won't get any more characters.
		 */
		if (n_tty_chars_in_buffer(tty) <= TTY_THRESHOLD_UNTHROTTLE) {
			n_tty_set_room(tty);
			check_unthrottle(tty);
		}

		if (b - buf >= minimum)
=======
		if (packet && tty->link->ctrl.pktstatus) {
			u8 cs;
			if (kb != kbuf)
				break;
			spin_lock_irq(&tty->link->ctrl.lock);
			cs = tty->link->ctrl.pktstatus;
			tty->link->ctrl.pktstatus = 0;
			spin_unlock_irq(&tty->link->ctrl.lock);
			*kb++ = cs;
			nr--;
			break;
		}

		if (!input_available_p(tty, 0)) {
			up_read(&tty->termios_rwsem);
			tty_buffer_flush_work(tty->port);
			down_read(&tty->termios_rwsem);
			if (!input_available_p(tty, 0)) {
				if (test_bit(TTY_OTHER_CLOSED, &tty->flags)) {
					retval = -EIO;
					break;
				}
				if (tty_hung_up_p(file))
					break;
				/*
				 * Abort readers for ttys which never actually
				 * get hung up.  See __tty_hangup().
				 */
				if (test_bit(TTY_HUPPING, &tty->flags))
					break;
				if (!timeout)
					break;
				if (tty_io_nonblock(tty, file)) {
					retval = -EAGAIN;
					break;
				}
				if (signal_pending(current)) {
					retval = -ERESTARTSYS;
					break;
				}
				up_read(&tty->termios_rwsem);

				timeout = wait_woken(&wait, TASK_INTERRUPTIBLE,
						timeout);

				down_read(&tty->termios_rwsem);
				continue;
			}
		}

		if (ldata->icanon && !L_EXTPROC(tty)) {
			if (canon_copy_from_read_buf(tty, &kb, &nr))
				goto more_to_be_read;
		} else {
			/* Deal with packet mode. */
			if (packet && kb == kbuf) {
				*kb++ = TIOCPKT_DATA;
				nr--;
			}

			/*
			 * Copy data, and if there is more to be had
			 * and we have nothing more to wait for, then
			 * let's mark us for retries.
			 *
			 * NOTE! We return here with both the termios_sem
			 * and atomic_read_lock still held, the retries
			 * will release them when done.
			 */
			if (copy_from_read_buf(tty, &kb, &nr) && kb - kbuf >= minimum) {
more_to_be_read:
				remove_wait_queue(&tty->read_wait, &wait);
				*cookie = cookie;
				return kb - kbuf;
			}
		}

		n_tty_check_unthrottle(tty);

		if (kb - kbuf >= minimum)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		if (time)
			timeout = time;
	}
<<<<<<< HEAD
	mutex_unlock(&tty->atomic_read_lock);
	remove_wait_queue(&tty->read_wait, &wait);

	if (!waitqueue_active(&tty->read_wait))
		tty->minimum_to_wake = minimum;

	__set_current_state(TASK_RUNNING);
	size = b - buf;
	if (size) {
		retval = size;
		if (nr)
			clear_bit(TTY_PUSH, &tty->flags);
	} else if (test_and_clear_bit(TTY_PUSH, &tty->flags))
		goto do_it_again;

	n_tty_set_room(tty);
=======
	if (old_tail != ldata->read_tail) {
		/*
		 * Make sure no_room is not read in n_tty_kick_worker()
		 * before setting ldata->read_tail in copy_from_read_buf().
		 */
		smp_mb();
		n_tty_kick_worker(tty);
	}
	up_read(&tty->termios_rwsem);

	remove_wait_queue(&tty->read_wait, &wait);
	mutex_unlock(&ldata->atomic_read_lock);

	if (kb - kbuf)
		retval = kb - kbuf;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return retval;
}

/**
<<<<<<< HEAD
 *	n_tty_write		-	write function for tty
 *	@tty: tty device
 *	@file: file object
 *	@buf: userspace buffer pointer
 *	@nr: size of I/O
 *
 *	Write function of the terminal device.  This is serialized with
 *	respect to other write callers but not to termios changes, reads
 *	and other such events.  Since the receive code will echo characters,
 *	thus calling driver write methods, the output_lock is used in
 *	the output processing functions called here as well as in the
 *	echo processing function to protect the column state and space
 *	left in the buffer.
 *
 *	This code must be sure never to sleep through a hangup.
 *
 *	Locking: output_lock to protect column state and space left
 *		 (note that the process_output*() functions take this
 *		  lock themselves)
 */

static ssize_t n_tty_write(struct tty_struct *tty, struct file *file,
			   const unsigned char *buf, size_t nr)
{
	const unsigned char *b = buf;
	DECLARE_WAITQUEUE(wait, current);
	int c;
	ssize_t retval = 0;

	/* Job control check -- must be done at start (POSIX.1 7.1.1.4). */
	if (L_TOSTOP(tty) && file->f_op->write != redirected_tty_write) {
=======
 * n_tty_write		-	write function for tty
 * @tty: tty device
 * @file: file object
 * @buf: userspace buffer pointer
 * @nr: size of I/O
 *
 * Write function of the terminal device. This is serialized with respect to
 * other write callers but not to termios changes, reads and other such events.
 * Since the receive code will echo characters, thus calling driver write
 * methods, the %output_lock is used in the output processing functions called
 * here as well as in the echo processing function to protect the column state
 * and space left in the buffer.
 *
 * This code must be sure never to sleep through a hangup.
 *
 * Locking: output_lock to protect column state and space left
 *	 (note that the process_output*() functions take this lock themselves)
 */

static ssize_t n_tty_write(struct tty_struct *tty, struct file *file,
			   const u8 *buf, size_t nr)
{
	const u8 *b = buf;
	DEFINE_WAIT_FUNC(wait, woken_wake_function);
	ssize_t num, retval = 0;

	/* Job control check -- must be done at start (POSIX.1 7.1.1.4). */
	if (L_TOSTOP(tty) && file->f_op->write_iter != redirected_tty_write) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		retval = tty_check_change(tty);
		if (retval)
			return retval;
	}

<<<<<<< HEAD
=======
	down_read(&tty->termios_rwsem);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Write out any echoed characters that are still pending */
	process_echoes(tty);

	add_wait_queue(&tty->write_wait, &wait);
	while (1) {
<<<<<<< HEAD
		set_current_state(TASK_INTERRUPTIBLE);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (signal_pending(current)) {
			retval = -ERESTARTSYS;
			break;
		}
		if (tty_hung_up_p(file) || (tty->link && !tty->link->count)) {
			retval = -EIO;
			break;
		}
<<<<<<< HEAD
		if (O_OPOST(tty) && !(test_bit(TTY_HW_COOK_OUT, &tty->flags))) {
			while (nr > 0) {
				ssize_t num = process_output_block(tty, b, nr);
=======
		if (O_OPOST(tty)) {
			while (nr > 0) {
				num = process_output_block(tty, b, nr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				if (num < 0) {
					if (num == -EAGAIN)
						break;
					retval = num;
					goto break_out;
				}
				b += num;
				nr -= num;
				if (nr == 0)
					break;
<<<<<<< HEAD
				c = *b;
				if (process_output(c, tty) < 0)
=======
				if (process_output(*b, tty) < 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					break;
				b++; nr--;
			}
			if (tty->ops->flush_chars)
				tty->ops->flush_chars(tty);
		} else {
<<<<<<< HEAD

			while (nr > 0) {
				mutex_lock(&tty->output_lock);
				c = tty->ops->write(tty, b, nr);
				mutex_unlock(&tty->output_lock);
				if (c < 0) {
					retval = c;
					goto break_out;
				}
				if (!c)
					break;
				b += c;
				nr -= c;
=======
			struct n_tty_data *ldata = tty->disc_data;

			while (nr > 0) {
				mutex_lock(&ldata->output_lock);
				num = tty->ops->write(tty, b, nr);
				mutex_unlock(&ldata->output_lock);
				if (num < 0) {
					retval = num;
					goto break_out;
				}
				if (!num)
					break;
				b += num;
				nr -= num;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			}
		}
		if (!nr)
			break;
<<<<<<< HEAD
		if (file->f_flags & O_NONBLOCK) {
			retval = -EAGAIN;
			break;
		}
		schedule();
	}
break_out:
	__set_current_state(TASK_RUNNING);
	remove_wait_queue(&tty->write_wait, &wait);
	if (b - buf != nr && tty->fasync)
		set_bit(TTY_DO_WRITE_WAKEUP, &tty->flags);
=======
		if (tty_io_nonblock(tty, file)) {
			retval = -EAGAIN;
			break;
		}
		up_read(&tty->termios_rwsem);

		wait_woken(&wait, TASK_INTERRUPTIBLE, MAX_SCHEDULE_TIMEOUT);

		down_read(&tty->termios_rwsem);
	}
break_out:
	remove_wait_queue(&tty->write_wait, &wait);
	if (nr && tty->fasync)
		set_bit(TTY_DO_WRITE_WAKEUP, &tty->flags);
	up_read(&tty->termios_rwsem);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return (b - buf) ? b - buf : retval;
}

/**
<<<<<<< HEAD
 *	n_tty_poll		-	poll method for N_TTY
 *	@tty: terminal device
 *	@file: file accessing it
 *	@wait: poll table
 *
 *	Called when the line discipline is asked to poll() for data or
 *	for special events. This code is not serialized with respect to
 *	other events save open/close.
 *
 *	This code must be sure never to sleep through a hangup.
 *	Called without the kernel lock held - fine
 */

static unsigned int n_tty_poll(struct tty_struct *tty, struct file *file,
							poll_table *wait)
{
	unsigned int mask = 0;

	poll_wait(file, &tty->read_wait, wait);
	poll_wait(file, &tty->write_wait, wait);
	if (input_available_p(tty, TIME_CHAR(tty) ? 0 : MIN_CHAR(tty)))
		mask |= POLLIN | POLLRDNORM;
	if (tty->packet && tty->link->ctrl_status)
		mask |= POLLPRI | POLLIN | POLLRDNORM;
	if (test_bit(TTY_OTHER_CLOSED, &tty->flags))
		mask |= POLLHUP;
	if (tty_hung_up_p(file))
		mask |= POLLHUP;
	if (!(mask & (POLLHUP | POLLIN | POLLRDNORM))) {
		if (MIN_CHAR(tty) && !TIME_CHAR(tty))
			tty->minimum_to_wake = MIN_CHAR(tty);
		else
			tty->minimum_to_wake = 1;
	}
	if (tty->ops->write && !tty_is_writelocked(tty) &&
			tty_chars_in_buffer(tty) < WAKEUP_CHARS &&
			tty_write_room(tty) > 0)
		mask |= POLLOUT | POLLWRNORM;
	return mask;
}

static unsigned long inq_canon(struct tty_struct *tty)
{
	int nr, head, tail;

	if (!tty->canon_data)
		return 0;
	head = tty->canon_head;
	tail = tty->read_tail;
	nr = (head - tail) & (N_TTY_BUF_SIZE-1);
	/* Skip EOF-chars.. */
	while (head != tail) {
		if (test_bit(tail, tty->read_flags) &&
		    tty->read_buf[tail] == __DISABLED_CHAR)
			nr--;
		tail = (tail+1) & (N_TTY_BUF_SIZE-1);
=======
 * n_tty_poll		-	poll method for N_TTY
 * @tty: terminal device
 * @file: file accessing it
 * @wait: poll table
 *
 * Called when the line discipline is asked to poll() for data or for special
 * events. This code is not serialized with respect to other events save
 * open/close.
 *
 * This code must be sure never to sleep through a hangup.
 *
 * Locking: called without the kernel lock held -- fine.
 */
static __poll_t n_tty_poll(struct tty_struct *tty, struct file *file,
							poll_table *wait)
{
	__poll_t mask = 0;

	poll_wait(file, &tty->read_wait, wait);
	poll_wait(file, &tty->write_wait, wait);
	if (input_available_p(tty, 1))
		mask |= EPOLLIN | EPOLLRDNORM;
	else {
		tty_buffer_flush_work(tty->port);
		if (input_available_p(tty, 1))
			mask |= EPOLLIN | EPOLLRDNORM;
	}
	if (tty->ctrl.packet && tty->link->ctrl.pktstatus)
		mask |= EPOLLPRI | EPOLLIN | EPOLLRDNORM;
	if (test_bit(TTY_OTHER_CLOSED, &tty->flags))
		mask |= EPOLLHUP;
	if (tty_hung_up_p(file))
		mask |= EPOLLHUP;
	if (tty->ops->write && !tty_is_writelocked(tty) &&
			tty_chars_in_buffer(tty) < WAKEUP_CHARS &&
			tty_write_room(tty) > 0)
		mask |= EPOLLOUT | EPOLLWRNORM;
	return mask;
}

static unsigned long inq_canon(struct n_tty_data *ldata)
{
	size_t nr, head, tail;

	if (ldata->canon_head == ldata->read_tail)
		return 0;
	head = ldata->canon_head;
	tail = ldata->read_tail;
	nr = head - tail;
	/* Skip EOF-chars.. */
	while (MASK(head) != MASK(tail)) {
		if (test_bit(MASK(tail), ldata->read_flags) &&
		    read_buf(ldata, tail) == __DISABLED_CHAR)
			nr--;
		tail++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return nr;
}

<<<<<<< HEAD
static int n_tty_ioctl(struct tty_struct *tty, struct file *file,
		       unsigned int cmd, unsigned long arg)
{
	int retval;
=======
static int n_tty_ioctl(struct tty_struct *tty, unsigned int cmd,
		       unsigned long arg)
{
	struct n_tty_data *ldata = tty->disc_data;
	unsigned int num;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (cmd) {
	case TIOCOUTQ:
		return put_user(tty_chars_in_buffer(tty), (int __user *) arg);
	case TIOCINQ:
<<<<<<< HEAD
		/* FIXME: Locking */
		retval = tty->read_cnt;
		if (L_ICANON(tty))
			retval = inq_canon(tty);
		return put_user(retval, (unsigned int __user *) arg);
	default:
		return n_tty_ioctl_helper(tty, file, cmd, arg);
	}
}

struct tty_ldisc_ops tty_ldisc_N_TTY = {
	.magic           = TTY_LDISC_MAGIC,
=======
		down_write(&tty->termios_rwsem);
		if (L_ICANON(tty) && !L_EXTPROC(tty))
			num = inq_canon(ldata);
		else
			num = read_cnt(ldata);
		up_write(&tty->termios_rwsem);
		return put_user(num, (unsigned int __user *) arg);
	default:
		return n_tty_ioctl_helper(tty, cmd, arg);
	}
}

static struct tty_ldisc_ops n_tty_ops = {
	.owner		 = THIS_MODULE,
	.num		 = N_TTY,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.name            = "n_tty",
	.open            = n_tty_open,
	.close           = n_tty_close,
	.flush_buffer    = n_tty_flush_buffer,
<<<<<<< HEAD
	.chars_in_buffer = n_tty_chars_in_buffer,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.read            = n_tty_read,
	.write           = n_tty_write,
	.ioctl           = n_tty_ioctl,
	.set_termios     = n_tty_set_termios,
	.poll            = n_tty_poll,
	.receive_buf     = n_tty_receive_buf,
<<<<<<< HEAD
	.write_wakeup    = n_tty_write_wakeup
=======
	.write_wakeup    = n_tty_write_wakeup,
	.receive_buf2	 = n_tty_receive_buf2,
	.lookahead_buf	 = n_tty_lookahead_flow_ctrl,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/**
 *	n_tty_inherit_ops	-	inherit N_TTY methods
 *	@ops: struct tty_ldisc_ops where to save N_TTY methods
 *
<<<<<<< HEAD
 *	Used by a generic struct tty_ldisc_ops to easily inherit N_TTY
 *	methods.
=======
 *	Enables a 'subclass' line discipline to 'inherit' N_TTY methods.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

void n_tty_inherit_ops(struct tty_ldisc_ops *ops)
{
<<<<<<< HEAD
	*ops = tty_ldisc_N_TTY;
	ops->owner = NULL;
	ops->refcount = ops->flags = 0;
}
EXPORT_SYMBOL_GPL(n_tty_inherit_ops);
=======
	*ops = n_tty_ops;
	ops->owner = NULL;
}
EXPORT_SYMBOL_GPL(n_tty_inherit_ops);

void __init n_tty_init(void)
{
	tty_register_ldisc(&n_tty_ops);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
