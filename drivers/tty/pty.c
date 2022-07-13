<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  Copyright (C) 1991, 1992  Linus Torvalds
 *
 *  Added support for a Unix98-style ptmx device.
 *    -- C. Scott Ananian <cananian@alumni.princeton.edu>, 14-Jan-1998
 *
<<<<<<< HEAD
 *  When reading this code see also fs/devpts. In particular note that the
 *  driver_data field is used by the devpts side as a binding to the devpts
 *  inode.
 */

#include <linux/module.h>

=======
 */

#include <linux/module.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/errno.h>
#include <linux/interrupt.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/fcntl.h>
<<<<<<< HEAD
#include <linux/sched.h>
=======
#include <linux/sched/signal.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/string.h>
#include <linux/major.h>
#include <linux/mm.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/bitops.h>
#include <linux/devpts_fs.h>
#include <linux/slab.h>
<<<<<<< HEAD

=======
#include <linux/mutex.h>
#include <linux/poll.h>
#include <linux/mount.h>
#include <linux/file.h>
#include <linux/ioctl.h>
#include <linux/compat.h>
#include "tty.h"

#undef TTY_DEBUG_HANGUP
#ifdef TTY_DEBUG_HANGUP
# define tty_debug_hangup(tty, f, args...)	tty_debug(tty, f, ##args)
#else
# define tty_debug_hangup(tty, f, args...)	do {} while (0)
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_UNIX98_PTYS
static struct tty_driver *ptm_driver;
static struct tty_driver *pts_driver;
<<<<<<< HEAD
=======
static DEFINE_MUTEX(devpts_mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

static void pty_close(struct tty_struct *tty, struct file *filp)
{
<<<<<<< HEAD
	BUG_ON(!tty);
	if (tty->driver->subtype == PTY_TYPE_MASTER)
		WARN_ON(tty->count > 1);
	else {
		if (tty->count > 2)
			return;
	}
	wake_up_interruptible(&tty->read_wait);
	wake_up_interruptible(&tty->write_wait);
	tty->packet = 0;
=======
	if (tty->driver->subtype == PTY_TYPE_MASTER)
		WARN_ON(tty->count > 1);
	else {
		if (tty_io_error(tty))
			return;
		if (tty->count > 2)
			return;
	}
	set_bit(TTY_IO_ERROR, &tty->flags);
	wake_up_interruptible(&tty->read_wait);
	wake_up_interruptible(&tty->write_wait);
	spin_lock_irq(&tty->ctrl.lock);
	tty->ctrl.packet = false;
	spin_unlock_irq(&tty->ctrl.lock);
	/* Review - krefs on tty_link ?? */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!tty->link)
		return;
	set_bit(TTY_OTHER_CLOSED, &tty->link->flags);
	wake_up_interruptible(&tty->link->read_wait);
	wake_up_interruptible(&tty->link->write_wait);
	if (tty->driver->subtype == PTY_TYPE_MASTER) {
		set_bit(TTY_OTHER_CLOSED, &tty->flags);
#ifdef CONFIG_UNIX98_PTYS
<<<<<<< HEAD
		if (tty->driver == ptm_driver)
			devpts_pty_kill(tty->link);
#endif
		tty_unlock();
		tty_vhangup(tty->link);
		tty_lock();
=======
		if (tty->driver == ptm_driver) {
			mutex_lock(&devpts_mutex);
			if (tty->link->driver_data)
				devpts_pty_kill(tty->link->driver_data);
			mutex_unlock(&devpts_mutex);
		}
#endif
		tty_vhangup(tty->link);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

/*
 * The unthrottle routine is called by the line discipline to signal
 * that it can receive more characters.  For PTY's, the TTY_THROTTLED
 * flag is always set, to force the line discipline to always call the
 * unthrottle routine when there are fewer than TTY_THRESHOLD_UNTHROTTLE
 * characters in the queue.  This is necessary since each time this
 * happens, we need to wake up any sleeping processes that could be
 * (1) trying to send data to the pty, or (2) waiting in wait_until_sent()
 * for the pty buffer to be drained.
 */
static void pty_unthrottle(struct tty_struct *tty)
{
	tty_wakeup(tty->link);
	set_bit(TTY_THROTTLED, &tty->flags);
}

/**
<<<<<<< HEAD
 *	pty_space	-	report space left for writing
 *	@to: tty we are writing into
 *
 *	The tty buffers allow 64K but we sneak a peak and clip at 8K this
 *	allows a lot of overspill room for echo and other fun messes to
 *	be handled properly
 */

static int pty_space(struct tty_struct *to)
{
	int n = 8192 - to->buf.memory_used;
	if (n < 0)
		return 0;
	return n;
}

/**
 *	pty_write		-	write to a pty
 *	@tty: the tty we write from
 *	@buf: kernel buffer of data
 *	@count: bytes to write
=======
 *	pty_write		-	write to a pty
 *	@tty: the tty we write from
 *	@buf: kernel buffer of data
 *	@c: bytes to write
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 *	Our "hardware" write method. Data is coming from the ldisc which
 *	may be in a non sleeping state. We simply throw this at the other
 *	end of the link as if we were an IRQ handler receiving stuff for
 *	the other side of the pty/tty pair.
 */

<<<<<<< HEAD
static int pty_write(struct tty_struct *tty, const unsigned char *buf, int c)
{
	struct tty_struct *to = tty->link;

	if (tty->stopped)
		return 0;

	if (c > 0) {
		/* Stuff the data into the input queue of the other end */
		c = tty_insert_flip_string(to, buf, c);
		/* And shovel */
		if (c) {
			tty_flip_buffer_push(to);
			tty_wakeup(tty);
		}
	}
	return c;
=======
static ssize_t pty_write(struct tty_struct *tty, const u8 *buf, size_t c)
{
	struct tty_struct *to = tty->link;

	if (tty->flow.stopped || !c)
		return 0;

	return tty_insert_flip_string_and_push_buffer(to->port, buf, c);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 *	pty_write_room	-	write space
 *	@tty: tty we are writing from
 *
 *	Report how many bytes the ldisc can send into the queue for
 *	the other device.
 */

<<<<<<< HEAD
static int pty_write_room(struct tty_struct *tty)
{
	if (tty->stopped)
		return 0;
	return pty_space(tty->link);
}

/**
 *	pty_chars_in_buffer	-	characters currently in our tx queue
 *	@tty: our tty
 *
 *	Report how much we have in the transmit queue. As everything is
 *	instantly at the other end this is easy to implement.
 */

static int pty_chars_in_buffer(struct tty_struct *tty)
{
	return 0;
=======
static unsigned int pty_write_room(struct tty_struct *tty)
{
	if (tty->flow.stopped)
		return 0;
	return tty_buffer_space_avail(tty->link->port);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Set the lock flag on a pty */
static int pty_set_lock(struct tty_struct *tty, int __user *arg)
{
	int val;
<<<<<<< HEAD
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (get_user(val, arg))
		return -EFAULT;
	if (val)
		set_bit(TTY_PTY_LOCK, &tty->flags);
	else
		clear_bit(TTY_PTY_LOCK, &tty->flags);
	return 0;
}

<<<<<<< HEAD
/* Send a signal to the slave */
static int pty_signal(struct tty_struct *tty, int sig)
{
	unsigned long flags;
=======
static int pty_get_lock(struct tty_struct *tty, int __user *arg)
{
	int locked = test_bit(TTY_PTY_LOCK, &tty->flags);

	return put_user(locked, arg);
}

/* Set the packet mode on a pty */
static int pty_set_pktmode(struct tty_struct *tty, int __user *arg)
{
	int pktmode;

	if (get_user(pktmode, arg))
		return -EFAULT;

	spin_lock_irq(&tty->ctrl.lock);
	if (pktmode) {
		if (!tty->ctrl.packet) {
			tty->link->ctrl.pktstatus = 0;
			smp_mb();
			tty->ctrl.packet = true;
		}
	} else
		tty->ctrl.packet = false;
	spin_unlock_irq(&tty->ctrl.lock);

	return 0;
}

/* Get the packet mode of a pty */
static int pty_get_pktmode(struct tty_struct *tty, int __user *arg)
{
	int pktmode = tty->ctrl.packet;

	return put_user(pktmode, arg);
}

/* Send a signal to the slave */
static int pty_signal(struct tty_struct *tty, int sig)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct pid *pgrp;

	if (sig != SIGINT && sig != SIGQUIT && sig != SIGTSTP)
		return -EINVAL;

	if (tty->link) {
<<<<<<< HEAD
		spin_lock_irqsave(&tty->link->ctrl_lock, flags);
		pgrp = get_pid(tty->link->pgrp);
		spin_unlock_irqrestore(&tty->link->ctrl_lock, flags);

		kill_pgrp(pgrp, sig, 1);
=======
		pgrp = tty_get_pgrp(tty->link);
		if (pgrp)
			kill_pgrp(pgrp, sig, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		put_pid(pgrp);
	}
	return 0;
}

static void pty_flush_buffer(struct tty_struct *tty)
{
	struct tty_struct *to = tty->link;
<<<<<<< HEAD
	unsigned long flags;

	if (!to)
		return;
	/* tty_buffer_flush(to); FIXME */
	if (to->packet) {
		spin_lock_irqsave(&tty->ctrl_lock, flags);
		tty->ctrl_status |= TIOCPKT_FLUSHWRITE;
		wake_up_interruptible(&to->read_wait);
		spin_unlock_irqrestore(&tty->ctrl_lock, flags);
=======

	if (!to)
		return;

	tty_buffer_flush(to, NULL);
	if (to->ctrl.packet) {
		spin_lock_irq(&tty->ctrl.lock);
		tty->ctrl.pktstatus |= TIOCPKT_FLUSHWRITE;
		wake_up_interruptible(&to->read_wait);
		spin_unlock_irq(&tty->ctrl.lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

static int pty_open(struct tty_struct *tty, struct file *filp)
{
<<<<<<< HEAD
	int	retval = -ENODEV;

	if (!tty || !tty->link)
		goto out;

	retval = -EIO;
=======
	if (!tty || !tty->link)
		return -ENODEV;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (test_bit(TTY_OTHER_CLOSED, &tty->flags))
		goto out;
	if (test_bit(TTY_PTY_LOCK, &tty->link->flags))
		goto out;
<<<<<<< HEAD
	if (tty->link->count != 1)
		goto out;

	clear_bit(TTY_OTHER_CLOSED, &tty->link->flags);
	set_bit(TTY_THROTTLED, &tty->flags);
	retval = 0;
out:
	return retval;
}

static void pty_set_termios(struct tty_struct *tty,
					struct ktermios *old_termios)
{
	tty->termios->c_cflag &= ~(CSIZE | PARENB);
	tty->termios->c_cflag |= (CS8 | CREAD);
}

/**
 *	pty_do_resize		-	resize event
=======
	if (tty->driver->subtype == PTY_TYPE_SLAVE && tty->link->count != 1)
		goto out;

	clear_bit(TTY_IO_ERROR, &tty->flags);
	clear_bit(TTY_OTHER_CLOSED, &tty->link->flags);
	set_bit(TTY_THROTTLED, &tty->flags);
	return 0;

out:
	set_bit(TTY_IO_ERROR, &tty->flags);
	return -EIO;
}

static void pty_set_termios(struct tty_struct *tty,
			    const struct ktermios *old_termios)
{
	/* See if packet mode change of state. */
	if (tty->link && tty->link->ctrl.packet) {
		int extproc = (old_termios->c_lflag & EXTPROC) | L_EXTPROC(tty);
		int old_flow = ((old_termios->c_iflag & IXON) &&
				(old_termios->c_cc[VSTOP] == '\023') &&
				(old_termios->c_cc[VSTART] == '\021'));
		int new_flow = (I_IXON(tty) &&
				STOP_CHAR(tty) == '\023' &&
				START_CHAR(tty) == '\021');
		if ((old_flow != new_flow) || extproc) {
			spin_lock_irq(&tty->ctrl.lock);
			if (old_flow != new_flow) {
				tty->ctrl.pktstatus &= ~(TIOCPKT_DOSTOP | TIOCPKT_NOSTOP);
				if (new_flow)
					tty->ctrl.pktstatus |= TIOCPKT_DOSTOP;
				else
					tty->ctrl.pktstatus |= TIOCPKT_NOSTOP;
			}
			if (extproc)
				tty->ctrl.pktstatus |= TIOCPKT_IOCTL;
			spin_unlock_irq(&tty->ctrl.lock);
			wake_up_interruptible(&tty->link->read_wait);
		}
	}

	tty->termios.c_cflag &= ~(CSIZE | PARENB);
	tty->termios.c_cflag |= (CS8 | CREAD);
}

/**
 *	pty_resize		-	resize event
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *	@tty: tty being resized
 *	@ws: window size being set.
 *
 *	Update the termios variables and send the necessary signals to
 *	peform a terminal resize correctly
 */

<<<<<<< HEAD
int pty_resize(struct tty_struct *tty,  struct winsize *ws)
{
	struct pid *pgrp, *rpgrp;
	unsigned long flags;
	struct tty_struct *pty = tty->link;

	/* For a PTY we need to lock the tty side */
	mutex_lock(&tty->termios_mutex);
	if (!memcmp(ws, &tty->winsize, sizeof(*ws)))
		goto done;

	/* Get the PID values and reference them so we can
	   avoid holding the tty ctrl lock while sending signals.
	   We need to lock these individually however. */

	spin_lock_irqsave(&tty->ctrl_lock, flags);
	pgrp = get_pid(tty->pgrp);
	spin_unlock_irqrestore(&tty->ctrl_lock, flags);

	spin_lock_irqsave(&pty->ctrl_lock, flags);
	rpgrp = get_pid(pty->pgrp);
	spin_unlock_irqrestore(&pty->ctrl_lock, flags);
=======
static int pty_resize(struct tty_struct *tty,  struct winsize *ws)
{
	struct pid *pgrp, *rpgrp;
	struct tty_struct *pty = tty->link;

	/* For a PTY we need to lock the tty side */
	mutex_lock(&tty->winsize_mutex);
	if (!memcmp(ws, &tty->winsize, sizeof(*ws)))
		goto done;

	/* Signal the foreground process group of both ptys */
	pgrp = tty_get_pgrp(tty);
	rpgrp = tty_get_pgrp(pty);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (pgrp)
		kill_pgrp(pgrp, SIGWINCH, 1);
	if (rpgrp != pgrp && rpgrp)
		kill_pgrp(rpgrp, SIGWINCH, 1);

	put_pid(pgrp);
	put_pid(rpgrp);

	tty->winsize = *ws;
	pty->winsize = *ws;	/* Never used so will go away soon */
done:
<<<<<<< HEAD
	mutex_unlock(&tty->termios_mutex);
	return 0;
}

=======
	mutex_unlock(&tty->winsize_mutex);
	return 0;
}

/**
 *	pty_start - start() handler
 *	pty_stop  - stop() handler
 *	@tty: tty being flow-controlled
 *
 *	Propagates the TIOCPKT status to the master pty.
 *
 *	NB: only the master pty can be in packet mode so only the slave
 *	    needs start()/stop() handlers
 */
static void pty_start(struct tty_struct *tty)
{
	unsigned long flags;

	if (tty->link && tty->link->ctrl.packet) {
		spin_lock_irqsave(&tty->ctrl.lock, flags);
		tty->ctrl.pktstatus &= ~TIOCPKT_STOP;
		tty->ctrl.pktstatus |= TIOCPKT_START;
		spin_unlock_irqrestore(&tty->ctrl.lock, flags);
		wake_up_interruptible_poll(&tty->link->read_wait, EPOLLIN);
	}
}

static void pty_stop(struct tty_struct *tty)
{
	unsigned long flags;

	if (tty->link && tty->link->ctrl.packet) {
		spin_lock_irqsave(&tty->ctrl.lock, flags);
		tty->ctrl.pktstatus &= ~TIOCPKT_START;
		tty->ctrl.pktstatus |= TIOCPKT_STOP;
		spin_unlock_irqrestore(&tty->ctrl.lock, flags);
		wake_up_interruptible_poll(&tty->link->read_wait, EPOLLIN);
	}
}

/**
 *	pty_common_install		-	set up the pty pair
 *	@driver: the pty driver
 *	@tty: the tty being instantiated
 *	@legacy: true if this is BSD style
 *
 *	Perform the initial set up for the tty/pty pair. Called from the
 *	tty layer when the port is first opened.
 *
 *	Locking: the caller must hold the tty_mutex
 */
static int pty_common_install(struct tty_driver *driver, struct tty_struct *tty,
		bool legacy)
{
	struct tty_struct *o_tty;
	struct tty_port *ports[2];
	int idx = tty->index;
	int retval = -ENOMEM;

	/* Opening the slave first has always returned -EIO */
	if (driver->subtype != PTY_TYPE_MASTER)
		return -EIO;

	ports[0] = kmalloc(sizeof **ports, GFP_KERNEL);
	ports[1] = kmalloc(sizeof **ports, GFP_KERNEL);
	if (!ports[0] || !ports[1])
		goto err;
	if (!try_module_get(driver->other->owner)) {
		/* This cannot in fact currently happen */
		goto err;
	}
	o_tty = alloc_tty_struct(driver->other, idx);
	if (!o_tty)
		goto err_put_module;

	tty_set_lock_subclass(o_tty);
	lockdep_set_subclass(&o_tty->termios_rwsem, TTY_LOCK_SLAVE);

	if (legacy) {
		/* We always use new tty termios data so we can do this
		   the easy way .. */
		tty_init_termios(tty);
		tty_init_termios(o_tty);

		driver->other->ttys[idx] = o_tty;
		driver->ttys[idx] = tty;
	} else {
		memset(&tty->termios_locked, 0, sizeof(tty->termios_locked));
		tty->termios = driver->init_termios;
		memset(&o_tty->termios_locked, 0, sizeof(tty->termios_locked));
		o_tty->termios = driver->other->init_termios;
	}

	/*
	 * Everything allocated ... set up the o_tty structure.
	 */
	tty_driver_kref_get(driver->other);
	/* Establish the links in both directions */
	tty->link   = o_tty;
	o_tty->link = tty;
	tty_port_init(ports[0]);
	tty_port_init(ports[1]);
	tty_buffer_set_limit(ports[0], 8192);
	tty_buffer_set_limit(ports[1], 8192);
	o_tty->port = ports[0];
	tty->port = ports[1];
	o_tty->port->itty = o_tty;

	tty_buffer_set_lock_subclass(o_tty->port);

	tty_driver_kref_get(driver);
	tty->count++;
	o_tty->count++;
	return 0;

err_put_module:
	module_put(driver->other->owner);
err:
	kfree(ports[0]);
	kfree(ports[1]);
	return retval;
}

static void pty_cleanup(struct tty_struct *tty)
{
	tty_port_put(tty->port);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Traditional BSD devices */
#ifdef CONFIG_LEGACY_PTYS

static int pty_install(struct tty_driver *driver, struct tty_struct *tty)
{
<<<<<<< HEAD
	struct tty_struct *o_tty;
	int idx = tty->index;
	int retval;

	o_tty = alloc_tty_struct();
	if (!o_tty)
		return -ENOMEM;
	if (!try_module_get(driver->other->owner)) {
		/* This cannot in fact currently happen */
		retval = -ENOMEM;
		goto err_free_tty;
	}
	initialize_tty_struct(o_tty, driver->other, idx);

	/* We always use new tty termios data so we can do this
	   the easy way .. */
	retval = tty_init_termios(tty);
	if (retval)
		goto err_deinit_tty;

	retval = tty_init_termios(o_tty);
	if (retval)
		goto err_free_termios;

	/*
	 * Everything allocated ... set up the o_tty structure.
	 */
	driver->other->ttys[idx] = o_tty;
	tty_driver_kref_get(driver->other);
	if (driver->subtype == PTY_TYPE_MASTER)
		o_tty->count++;
	/* Establish the links in both directions */
	tty->link   = o_tty;
	o_tty->link = tty;

	tty_driver_kref_get(driver);
	tty->count++;
	driver->ttys[idx] = tty;
	return 0;
err_free_termios:
	tty_free_termios(tty);
err_deinit_tty:
	deinitialize_tty_struct(o_tty);
	module_put(o_tty->driver->owner);
err_free_tty:
	free_tty_struct(o_tty);
	return retval;
=======
	return pty_common_install(driver, tty, true);
}

static void pty_remove(struct tty_driver *driver, struct tty_struct *tty)
{
	struct tty_struct *pair = tty->link;

	driver->ttys[tty->index] = NULL;
	if (pair)
		pair->driver->ttys[pair->index] = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int pty_bsd_ioctl(struct tty_struct *tty,
			 unsigned int cmd, unsigned long arg)
{
	switch (cmd) {
	case TIOCSPTLCK: /* Set PT Lock (disallow slave open) */
		return pty_set_lock(tty, (int __user *) arg);
<<<<<<< HEAD
	case TIOCSIG:    /* Send signal to other side of pty */
		return pty_signal(tty, (int) arg);
=======
	case TIOCGPTLCK: /* Get PT Lock status */
		return pty_get_lock(tty, (int __user *)arg);
	case TIOCPKT: /* Set PT packet mode */
		return pty_set_pktmode(tty, (int __user *)arg);
	case TIOCGPKT: /* Get PT packet mode */
		return pty_get_pktmode(tty, (int __user *)arg);
	case TIOCSIG:    /* Send signal to other side of pty */
		return pty_signal(tty, (int) arg);
	case TIOCGPTN: /* TTY returns ENOTTY, but glibc expects EINVAL here */
		return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return -ENOIOCTLCMD;
}

<<<<<<< HEAD
static int legacy_count = CONFIG_LEGACY_PTY_COUNT;
=======
#ifdef CONFIG_COMPAT
static long pty_bsd_compat_ioctl(struct tty_struct *tty,
				 unsigned int cmd, unsigned long arg)
{
	/*
	 * PTY ioctls don't require any special translation between 32-bit and
	 * 64-bit userspace, they are already compatible.
	 */
	return pty_bsd_ioctl(tty, cmd, (unsigned long)compat_ptr(arg));
}
#else
#define pty_bsd_compat_ioctl NULL
#endif

static int legacy_count = CONFIG_LEGACY_PTY_COUNT;
/*
 * not really modular, but the easiest way to keep compat with existing
 * bootargs behaviour is to continue using module_param here.
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
module_param(legacy_count, int, 0);

/*
 * The master side of a pty can do TIOCSPTLCK and thus
 * has pty_bsd_ioctl.
 */
static const struct tty_operations master_pty_ops_bsd = {
	.install = pty_install,
	.open = pty_open,
	.close = pty_close,
	.write = pty_write,
	.write_room = pty_write_room,
	.flush_buffer = pty_flush_buffer,
<<<<<<< HEAD
	.chars_in_buffer = pty_chars_in_buffer,
	.unthrottle = pty_unthrottle,
	.set_termios = pty_set_termios,
	.ioctl = pty_bsd_ioctl,
	.resize = pty_resize
=======
	.unthrottle = pty_unthrottle,
	.ioctl = pty_bsd_ioctl,
	.compat_ioctl = pty_bsd_compat_ioctl,
	.cleanup = pty_cleanup,
	.resize = pty_resize,
	.remove = pty_remove
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static const struct tty_operations slave_pty_ops_bsd = {
	.install = pty_install,
	.open = pty_open,
	.close = pty_close,
	.write = pty_write,
	.write_room = pty_write_room,
	.flush_buffer = pty_flush_buffer,
<<<<<<< HEAD
	.chars_in_buffer = pty_chars_in_buffer,
	.unthrottle = pty_unthrottle,
	.set_termios = pty_set_termios,
	.resize = pty_resize
=======
	.unthrottle = pty_unthrottle,
	.set_termios = pty_set_termios,
	.cleanup = pty_cleanup,
	.resize = pty_resize,
	.start = pty_start,
	.stop = pty_stop,
	.remove = pty_remove
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static void __init legacy_pty_init(void)
{
	struct tty_driver *pty_driver, *pty_slave_driver;

	if (legacy_count <= 0)
		return;

<<<<<<< HEAD
	pty_driver = alloc_tty_driver(legacy_count);
	if (!pty_driver)
		panic("Couldn't allocate pty driver");

	pty_slave_driver = alloc_tty_driver(legacy_count);
	if (!pty_slave_driver)
=======
	pty_driver = tty_alloc_driver(legacy_count,
			TTY_DRIVER_RESET_TERMIOS |
			TTY_DRIVER_REAL_RAW |
			TTY_DRIVER_DYNAMIC_ALLOC);
	if (IS_ERR(pty_driver))
		panic("Couldn't allocate pty driver");

	pty_slave_driver = tty_alloc_driver(legacy_count,
			TTY_DRIVER_RESET_TERMIOS |
			TTY_DRIVER_REAL_RAW |
			TTY_DRIVER_DYNAMIC_ALLOC);
	if (IS_ERR(pty_slave_driver))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		panic("Couldn't allocate pty slave driver");

	pty_driver->driver_name = "pty_master";
	pty_driver->name = "pty";
	pty_driver->major = PTY_MASTER_MAJOR;
	pty_driver->minor_start = 0;
	pty_driver->type = TTY_DRIVER_TYPE_PTY;
	pty_driver->subtype = PTY_TYPE_MASTER;
	pty_driver->init_termios = tty_std_termios;
	pty_driver->init_termios.c_iflag = 0;
	pty_driver->init_termios.c_oflag = 0;
	pty_driver->init_termios.c_cflag = B38400 | CS8 | CREAD;
	pty_driver->init_termios.c_lflag = 0;
	pty_driver->init_termios.c_ispeed = 38400;
	pty_driver->init_termios.c_ospeed = 38400;
<<<<<<< HEAD
	pty_driver->flags = TTY_DRIVER_RESET_TERMIOS | TTY_DRIVER_REAL_RAW;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	pty_driver->other = pty_slave_driver;
	tty_set_operations(pty_driver, &master_pty_ops_bsd);

	pty_slave_driver->driver_name = "pty_slave";
	pty_slave_driver->name = "ttyp";
	pty_slave_driver->major = PTY_SLAVE_MAJOR;
	pty_slave_driver->minor_start = 0;
	pty_slave_driver->type = TTY_DRIVER_TYPE_PTY;
	pty_slave_driver->subtype = PTY_TYPE_SLAVE;
	pty_slave_driver->init_termios = tty_std_termios;
	pty_slave_driver->init_termios.c_cflag = B38400 | CS8 | CREAD;
	pty_slave_driver->init_termios.c_ispeed = 38400;
	pty_slave_driver->init_termios.c_ospeed = 38400;
<<<<<<< HEAD
	pty_slave_driver->flags = TTY_DRIVER_RESET_TERMIOS |
					TTY_DRIVER_REAL_RAW;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	pty_slave_driver->other = pty_driver;
	tty_set_operations(pty_slave_driver, &slave_pty_ops_bsd);

	if (tty_register_driver(pty_driver))
		panic("Couldn't register pty driver");
	if (tty_register_driver(pty_slave_driver))
		panic("Couldn't register pty slave driver");
}
#else
static inline void legacy_pty_init(void) { }
#endif

/* Unix98 devices */
#ifdef CONFIG_UNIX98_PTYS
<<<<<<< HEAD

static struct cdev ptmx_cdev;

=======
static struct cdev ptmx_cdev;

/**
 *	ptm_open_peer - open the peer of a pty
 *	@master: the open struct file of the ptmx device node
 *	@tty: the master of the pty being opened
 *	@flags: the flags for open
 *
 *	Provide a race free way for userspace to open the slave end of a pty
 *	(where they have the master fd and cannot access or trust the mount
 *	namespace /dev/pts was mounted inside).
 */
int ptm_open_peer(struct file *master, struct tty_struct *tty, int flags)
{
	int fd;
	struct file *filp;
	int retval = -EINVAL;
	struct path path;

	if (tty->driver != ptm_driver)
		return -EIO;

	fd = get_unused_fd_flags(flags);
	if (fd < 0) {
		retval = fd;
		goto err;
	}

	/* Compute the slave's path */
	path.mnt = devpts_mntget(master, tty->driver_data);
	if (IS_ERR(path.mnt)) {
		retval = PTR_ERR(path.mnt);
		goto err_put;
	}
	path.dentry = tty->link->driver_data;

	filp = dentry_open(&path, flags, current_cred());
	mntput(path.mnt);
	if (IS_ERR(filp)) {
		retval = PTR_ERR(filp);
		goto err_put;
	}

	fd_install(fd, filp);
	return fd;

err_put:
	put_unused_fd(fd);
err:
	return retval;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int pty_unix98_ioctl(struct tty_struct *tty,
			    unsigned int cmd, unsigned long arg)
{
	switch (cmd) {
	case TIOCSPTLCK: /* Set PT Lock (disallow slave open) */
		return pty_set_lock(tty, (int __user *)arg);
<<<<<<< HEAD
=======
	case TIOCGPTLCK: /* Get PT Lock status */
		return pty_get_lock(tty, (int __user *)arg);
	case TIOCPKT: /* Set PT packet mode */
		return pty_set_pktmode(tty, (int __user *)arg);
	case TIOCGPKT: /* Get PT packet mode */
		return pty_get_pktmode(tty, (int __user *)arg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case TIOCGPTN: /* Get PT Number */
		return put_user(tty->index, (unsigned int __user *)arg);
	case TIOCSIG:    /* Send signal to other side of pty */
		return pty_signal(tty, (int) arg);
	}

	return -ENOIOCTLCMD;
}

<<<<<<< HEAD
/**
 *	ptm_unix98_lookup	-	find a pty master
 *	@driver: ptm driver
=======
#ifdef CONFIG_COMPAT
static long pty_unix98_compat_ioctl(struct tty_struct *tty,
				 unsigned int cmd, unsigned long arg)
{
	/*
	 * PTY ioctls don't require any special translation between 32-bit and
	 * 64-bit userspace, they are already compatible.
	 */
	return pty_unix98_ioctl(tty, cmd,
		cmd == TIOCSIG ? arg : (unsigned long)compat_ptr(arg));
}
#else
#define pty_unix98_compat_ioctl NULL
#endif

/**
 *	ptm_unix98_lookup	-	find a pty master
 *	@driver: ptm driver
 *	@file: unused
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *	@idx: tty index
 *
 *	Look up a pty master device. Called under the tty_mutex for now.
 *	This provides our locking.
 */

static struct tty_struct *ptm_unix98_lookup(struct tty_driver *driver,
<<<<<<< HEAD
		struct inode *ptm_inode, int idx)
=======
		struct file *file, int idx)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	/* Master must be open via /dev/ptmx */
	return ERR_PTR(-EIO);
}

/**
 *	pts_unix98_lookup	-	find a pty slave
 *	@driver: pts driver
<<<<<<< HEAD
 *	@idx: tty index
 *
 *	Look up a pty master device. Called under the tty_mutex for now.
 *	This provides our locking.
 */

static struct tty_struct *pts_unix98_lookup(struct tty_driver *driver,
		struct inode *pts_inode, int idx)
{
	struct tty_struct *tty = devpts_get_tty(pts_inode, idx);
=======
 *	@file: file pointer to tty
 *	@idx: tty index
 *
 *	Look up a pty master device. Called under the tty_mutex for now.
 *	This provides our locking for the tty pointer.
 */

static struct tty_struct *pts_unix98_lookup(struct tty_driver *driver,
		struct file *file, int idx)
{
	struct tty_struct *tty;

	mutex_lock(&devpts_mutex);
	tty = devpts_get_priv(file->f_path.dentry);
	mutex_unlock(&devpts_mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Master must be open before slave */
	if (!tty)
		return ERR_PTR(-EIO);
	return tty;
}

<<<<<<< HEAD
static void pty_unix98_shutdown(struct tty_struct *tty)
{
	tty_driver_remove_tty(tty->driver, tty);
	/* We have our own method as we don't use the tty index */
	kfree(tty->termios);
}

/* We have no need to install and remove our tty objects as devpts does all
   the work for us */

static int pty_unix98_install(struct tty_driver *driver, struct tty_struct *tty)
{
	struct tty_struct *o_tty;
	int idx = tty->index;

	o_tty = alloc_tty_struct();
	if (!o_tty)
		return -ENOMEM;
	if (!try_module_get(driver->other->owner)) {
		/* This cannot in fact currently happen */
		goto err_free_tty;
	}
	initialize_tty_struct(o_tty, driver->other, idx);

	tty->termios = kzalloc(sizeof(struct ktermios[2]), GFP_KERNEL);
	if (tty->termios == NULL)
		goto err_free_mem;
	*tty->termios = driver->init_termios;
	tty->termios_locked = tty->termios + 1;

	o_tty->termios = kzalloc(sizeof(struct ktermios[2]), GFP_KERNEL);
	if (o_tty->termios == NULL)
		goto err_free_mem;
	*o_tty->termios = driver->other->init_termios;
	o_tty->termios_locked = o_tty->termios + 1;

	tty_driver_kref_get(driver->other);
	if (driver->subtype == PTY_TYPE_MASTER)
		o_tty->count++;
	/* Establish the links in both directions */
	tty->link   = o_tty;
	o_tty->link = tty;
	/*
	 * All structures have been allocated, so now we install them.
	 * Failures after this point use release_tty to clean up, so
	 * there's no need to null out the local pointers.
	 */
	tty_driver_kref_get(driver);
	tty->count++;
	return 0;
err_free_mem:
	deinitialize_tty_struct(o_tty);
	kfree(o_tty->termios);
	kfree(tty->termios);
	module_put(o_tty->driver->owner);
err_free_tty:
	free_tty_struct(o_tty);
	return -ENOMEM;
}

static void ptm_unix98_remove(struct tty_driver *driver, struct tty_struct *tty)
{
}

static void pts_unix98_remove(struct tty_driver *driver, struct tty_struct *tty)
{
=======
static int pty_unix98_install(struct tty_driver *driver, struct tty_struct *tty)
{
	return pty_common_install(driver, tty, false);
}

/* this is called once with whichever end is closed last */
static void pty_unix98_remove(struct tty_driver *driver, struct tty_struct *tty)
{
	struct pts_fs_info *fsi;

	if (tty->driver->subtype == PTY_TYPE_MASTER)
		fsi = tty->driver_data;
	else
		fsi = tty->link->driver_data;

	if (fsi) {
		devpts_kill_index(fsi, tty->index);
		devpts_release(fsi);
	}
}

static void pty_show_fdinfo(struct tty_struct *tty, struct seq_file *m)
{
	seq_printf(m, "tty-index:\t%d\n", tty->index);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const struct tty_operations ptm_unix98_ops = {
	.lookup = ptm_unix98_lookup,
	.install = pty_unix98_install,
<<<<<<< HEAD
	.remove = ptm_unix98_remove,
=======
	.remove = pty_unix98_remove,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.open = pty_open,
	.close = pty_close,
	.write = pty_write,
	.write_room = pty_write_room,
	.flush_buffer = pty_flush_buffer,
<<<<<<< HEAD
	.chars_in_buffer = pty_chars_in_buffer,
	.unthrottle = pty_unthrottle,
	.set_termios = pty_set_termios,
	.ioctl = pty_unix98_ioctl,
	.shutdown = pty_unix98_shutdown,
	.resize = pty_resize
=======
	.unthrottle = pty_unthrottle,
	.ioctl = pty_unix98_ioctl,
	.compat_ioctl = pty_unix98_compat_ioctl,
	.resize = pty_resize,
	.cleanup = pty_cleanup,
	.show_fdinfo = pty_show_fdinfo,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static const struct tty_operations pty_unix98_ops = {
	.lookup = pts_unix98_lookup,
	.install = pty_unix98_install,
<<<<<<< HEAD
	.remove = pts_unix98_remove,
=======
	.remove = pty_unix98_remove,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.open = pty_open,
	.close = pty_close,
	.write = pty_write,
	.write_room = pty_write_room,
	.flush_buffer = pty_flush_buffer,
<<<<<<< HEAD
	.chars_in_buffer = pty_chars_in_buffer,
	.unthrottle = pty_unthrottle,
	.set_termios = pty_set_termios,
	.shutdown = pty_unix98_shutdown
=======
	.unthrottle = pty_unthrottle,
	.set_termios = pty_set_termios,
	.start = pty_start,
	.stop = pty_stop,
	.cleanup = pty_cleanup,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/**
 *	ptmx_open		-	open a unix 98 pty master
 *	@inode: inode of device file
 *	@filp: file pointer to tty
 *
 *	Allocate a unix98 pty master device from the ptmx driver.
 *
 *	Locking: tty_mutex protects the init_dev work. tty->count should
<<<<<<< HEAD
 * 		protect the rest.
=======
 *		protect the rest.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *		allocated_ptys_lock handles the list of free pty numbers
 */

static int ptmx_open(struct inode *inode, struct file *filp)
{
<<<<<<< HEAD
	struct tty_struct *tty;
=======
	struct pts_fs_info *fsi;
	struct tty_struct *tty;
	struct dentry *dentry;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int retval;
	int index;

	nonseekable_open(inode, filp);

	/* We refuse fsnotify events on ptmx, since it's a shared resource */
	filp->f_mode |= FMODE_NONOTIFY;

	retval = tty_alloc_file(filp);
	if (retval)
		return retval;

<<<<<<< HEAD
	/* find a device that is not in use. */
	tty_lock();
	index = devpts_new_index(inode);
	tty_unlock();
	if (index < 0) {
		retval = index;
		goto err_file;
	}

	mutex_lock(&tty_mutex);
	tty_lock();
	tty = tty_init_dev(ptm_driver, index);
	mutex_unlock(&tty_mutex);

	if (IS_ERR(tty)) {
		retval = PTR_ERR(tty);
		goto out;
	}

	set_bit(TTY_PTY_LOCK, &tty->flags); /* LOCK THE SLAVE */

	tty_add_file(tty, filp);

	retval = devpts_pty_new(inode, tty->link);
	if (retval)
		goto err_release;
=======
	fsi = devpts_acquire(filp);
	if (IS_ERR(fsi)) {
		retval = PTR_ERR(fsi);
		goto out_free_file;
	}

	/* find a device that is not in use. */
	mutex_lock(&devpts_mutex);
	index = devpts_new_index(fsi);
	mutex_unlock(&devpts_mutex);

	retval = index;
	if (index < 0)
		goto out_put_fsi;


	mutex_lock(&tty_mutex);
	tty = tty_init_dev(ptm_driver, index);
	/* The tty returned here is locked so we can safely
	   drop the mutex */
	mutex_unlock(&tty_mutex);

	retval = PTR_ERR(tty);
	if (IS_ERR(tty))
		goto out;

	/*
	 * From here on out, the tty is "live", and the index and
	 * fsi will be killed/put by the tty_release()
	 */
	set_bit(TTY_PTY_LOCK, &tty->flags); /* LOCK THE SLAVE */
	tty->driver_data = fsi;

	tty_add_file(tty, filp);

	dentry = devpts_pty_new(fsi, index, tty->link);
	if (IS_ERR(dentry)) {
		retval = PTR_ERR(dentry);
		goto err_release;
	}
	tty->link->driver_data = dentry;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	retval = ptm_driver->ops->open(tty, filp);
	if (retval)
		goto err_release;

<<<<<<< HEAD
	tty_unlock();
	return 0;
err_release:
	tty_unlock();
	tty_release(inode, filp);
	return retval;
out:
	devpts_kill_index(inode, index);
	tty_unlock();
err_file:
=======
	tty_debug_hangup(tty, "opening (count=%d)\n", tty->count);

	tty_unlock(tty);
	return 0;
err_release:
	tty_unlock(tty);
	// This will also put-ref the fsi
	tty_release(inode, filp);
	return retval;
out:
	devpts_kill_index(fsi, index);
out_put_fsi:
	devpts_release(fsi);
out_free_file:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	tty_free_file(filp);
	return retval;
}

<<<<<<< HEAD
static struct file_operations ptmx_fops;

static void __init unix98_pty_init(void)
{
	ptm_driver = alloc_tty_driver(NR_UNIX98_PTY_MAX);
	if (!ptm_driver)
		panic("Couldn't allocate Unix98 ptm driver");
	pts_driver = alloc_tty_driver(NR_UNIX98_PTY_MAX);
	if (!pts_driver)
=======
static struct file_operations ptmx_fops __ro_after_init;

static void __init unix98_pty_init(void)
{
	ptm_driver = tty_alloc_driver(NR_UNIX98_PTY_MAX,
			TTY_DRIVER_RESET_TERMIOS |
			TTY_DRIVER_REAL_RAW |
			TTY_DRIVER_DYNAMIC_DEV |
			TTY_DRIVER_DEVPTS_MEM |
			TTY_DRIVER_DYNAMIC_ALLOC);
	if (IS_ERR(ptm_driver))
		panic("Couldn't allocate Unix98 ptm driver");
	pts_driver = tty_alloc_driver(NR_UNIX98_PTY_MAX,
			TTY_DRIVER_RESET_TERMIOS |
			TTY_DRIVER_REAL_RAW |
			TTY_DRIVER_DYNAMIC_DEV |
			TTY_DRIVER_DEVPTS_MEM |
			TTY_DRIVER_DYNAMIC_ALLOC);
	if (IS_ERR(pts_driver))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		panic("Couldn't allocate Unix98 pts driver");

	ptm_driver->driver_name = "pty_master";
	ptm_driver->name = "ptm";
	ptm_driver->major = UNIX98_PTY_MASTER_MAJOR;
	ptm_driver->minor_start = 0;
	ptm_driver->type = TTY_DRIVER_TYPE_PTY;
	ptm_driver->subtype = PTY_TYPE_MASTER;
	ptm_driver->init_termios = tty_std_termios;
	ptm_driver->init_termios.c_iflag = 0;
	ptm_driver->init_termios.c_oflag = 0;
	ptm_driver->init_termios.c_cflag = B38400 | CS8 | CREAD;
	ptm_driver->init_termios.c_lflag = 0;
	ptm_driver->init_termios.c_ispeed = 38400;
	ptm_driver->init_termios.c_ospeed = 38400;
<<<<<<< HEAD
	ptm_driver->flags = TTY_DRIVER_RESET_TERMIOS | TTY_DRIVER_REAL_RAW |
		TTY_DRIVER_DYNAMIC_DEV | TTY_DRIVER_DEVPTS_MEM;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ptm_driver->other = pts_driver;
	tty_set_operations(ptm_driver, &ptm_unix98_ops);

	pts_driver->driver_name = "pty_slave";
	pts_driver->name = "pts";
	pts_driver->major = UNIX98_PTY_SLAVE_MAJOR;
	pts_driver->minor_start = 0;
	pts_driver->type = TTY_DRIVER_TYPE_PTY;
	pts_driver->subtype = PTY_TYPE_SLAVE;
	pts_driver->init_termios = tty_std_termios;
	pts_driver->init_termios.c_cflag = B38400 | CS8 | CREAD;
	pts_driver->init_termios.c_ispeed = 38400;
	pts_driver->init_termios.c_ospeed = 38400;
<<<<<<< HEAD
	pts_driver->flags = TTY_DRIVER_RESET_TERMIOS | TTY_DRIVER_REAL_RAW |
		TTY_DRIVER_DYNAMIC_DEV | TTY_DRIVER_DEVPTS_MEM;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	pts_driver->other = ptm_driver;
	tty_set_operations(pts_driver, &pty_unix98_ops);

	if (tty_register_driver(ptm_driver))
		panic("Couldn't register Unix98 ptm driver");
	if (tty_register_driver(pts_driver))
		panic("Couldn't register Unix98 pts driver");

	/* Now create the /dev/ptmx special device */
	tty_default_fops(&ptmx_fops);
	ptmx_fops.open = ptmx_open;

	cdev_init(&ptmx_cdev, &ptmx_fops);
	if (cdev_add(&ptmx_cdev, MKDEV(TTYAUX_MAJOR, 2), 1) ||
	    register_chrdev_region(MKDEV(TTYAUX_MAJOR, 2), 1, "/dev/ptmx") < 0)
<<<<<<< HEAD
		panic("Couldn't register /dev/ptmx driver\n");
	device_create(tty_class, NULL, MKDEV(TTYAUX_MAJOR, 2), NULL, "ptmx");
=======
		panic("Couldn't register /dev/ptmx driver");
	device_create(&tty_class, NULL, MKDEV(TTYAUX_MAJOR, 2), NULL, "ptmx");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#else
static inline void unix98_pty_init(void) { }
#endif

static int __init pty_init(void)
{
	legacy_pty_init();
	unix98_pty_init();
	return 0;
}
<<<<<<< HEAD
module_init(pty_init);
=======
device_initcall(pty_init);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
