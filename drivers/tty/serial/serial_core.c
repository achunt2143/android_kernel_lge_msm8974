<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0+
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  Driver core for serial ports
 *
 *  Based on drivers/char/serial.c, by Linus Torvalds, Theodore Ts'o.
 *
 *  Copyright 1999 ARM Limited
 *  Copyright (C) 2000-2001 Deep Blue Solutions Ltd.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#include <linux/module.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/slab.h>
<<<<<<< HEAD
#include <linux/init.h>
#include <linux/console.h>
=======
#include <linux/sched/signal.h>
#include <linux/init.h>
#include <linux/console.h>
#include <linux/gpio/consumer.h>
#include <linux/kernel.h>
#include <linux/of.h>
#include <linux/pm_runtime.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/device.h>
#include <linux/serial.h> /* for serial_state and serial_icounter_struct */
#include <linux/serial_core.h>
<<<<<<< HEAD
#include <linux/delay.h>
#include <linux/mutex.h>

#include <asm/irq.h>
#include <asm/uaccess.h>
=======
#include <linux/sysrq.h>
#include <linux/delay.h>
#include <linux/mutex.h>
#include <linux/math64.h>
#include <linux/security.h>

#include <linux/irq.h>
#include <linux/uaccess.h>

#include "serial_base.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * This is used to lock changes in serial line configuration.
 */
static DEFINE_MUTEX(port_mutex);

/*
 * lockdep: port->lock is initialized in two places, but we
 *          want only one lock-class:
 */
static struct lock_class_key port_lock_key;

#define HIGH_BITS_OFFSET	((sizeof(long)-sizeof(int))*8)

<<<<<<< HEAD
#ifdef CONFIG_SERIAL_CORE_CONSOLE
#define uart_console(port)	((port)->cons && (port)->cons->index == (port)->line)
#else
#define uart_console(port)	(0)
#endif

static void uart_change_speed(struct tty_struct *tty, struct uart_state *state,
					struct ktermios *old_termios);
static void uart_wait_until_sent(struct tty_struct *tty, int timeout);
static void uart_change_pm(struct uart_state *state, int pm_state);

static void uart_port_shutdown(struct tty_port *port);

/*
 * This routine is used by the interrupt handler to schedule processing in
 * the software interrupt portion of the driver.
=======
/*
 * Max time with active RTS before/after data is sent.
 */
#define RS485_MAX_RTS_DELAY	100 /* msecs */

static void uart_change_pm(struct uart_state *state,
			   enum uart_pm_state pm_state);

static void uart_port_shutdown(struct tty_port *port);

static int uart_dcd_enabled(struct uart_port *uport)
{
	return !!(uport->status & UPSTAT_DCD_ENABLE);
}

static inline struct uart_port *uart_port_ref(struct uart_state *state)
{
	if (atomic_add_unless(&state->refcount, 1, 0))
		return state->uart_port;
	return NULL;
}

static inline void uart_port_deref(struct uart_port *uport)
{
	if (atomic_dec_and_test(&uport->state->refcount))
		wake_up(&uport->state->remove_wait);
}

#define uart_port_lock(state, flags)					\
	({								\
		struct uart_port *__uport = uart_port_ref(state);	\
		if (__uport)						\
			uart_port_lock_irqsave(__uport, &flags);	\
		__uport;						\
	})

#define uart_port_unlock(uport, flags)					\
	({								\
		struct uart_port *__uport = uport;			\
		if (__uport) {						\
			uart_port_unlock_irqrestore(__uport, flags);	\
			uart_port_deref(__uport);			\
		}							\
	})

static inline struct uart_port *uart_port_check(struct uart_state *state)
{
	lockdep_assert_held(&state->port.mutex);
	return state->uart_port;
}

/**
 * uart_write_wakeup - schedule write processing
 * @port: port to be processed
 *
 * This routine is used by the interrupt handler to schedule processing in the
 * software interrupt portion of the driver. A driver is expected to call this
 * function when the number of characters in the transmit buffer have dropped
 * below a threshold.
 *
 * Locking: @port->lock should be held
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
void uart_write_wakeup(struct uart_port *port)
{
	struct uart_state *state = port->state;
	/*
	 * This means you called this function _after_ the port was
	 * closed.  No cookie for you.
	 */
	BUG_ON(!state);
<<<<<<< HEAD
	tty_wakeup(state->port.tty);
}
=======
	tty_port_tty_wakeup(&state->port);
}
EXPORT_SYMBOL(uart_write_wakeup);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static void uart_stop(struct tty_struct *tty)
{
	struct uart_state *state = tty->driver_data;
<<<<<<< HEAD
	struct uart_port *port = state->uart_port;
	unsigned long flags;

	spin_lock_irqsave(&port->lock, flags);
	port->ops->stop_tx(port);
	spin_unlock_irqrestore(&port->lock, flags);
}

static void __uart_start(struct tty_struct *tty)
{
	struct uart_state *state = tty->driver_data;
	struct uart_port *port = state->uart_port;

	if (port->ops->wake_peer)
		port->ops->wake_peer(port);

	if (!uart_circ_empty(&state->xmit) && state->xmit.buf &&
	    !tty->stopped && !tty->hw_stopped)
		port->ops->start_tx(port);
=======
	struct uart_port *port;
	unsigned long flags;

	port = uart_port_lock(state, flags);
	if (port)
		port->ops->stop_tx(port);
	uart_port_unlock(port, flags);
}

static void __uart_start(struct uart_state *state)
{
	struct uart_port *port = state->uart_port;
	struct serial_port_device *port_dev;
	int err;

	if (!port || port->flags & UPF_DEAD || uart_tx_stopped(port))
		return;

	port_dev = port->port_dev;

	/* Increment the runtime PM usage count for the active check below */
	err = pm_runtime_get(&port_dev->dev);
	if (err < 0 && err != -EINPROGRESS) {
		pm_runtime_put_noidle(&port_dev->dev);
		return;
	}

	/*
	 * Start TX if enabled, and kick runtime PM. If the device is not
	 * enabled, serial_port_runtime_resume() calls start_tx() again
	 * after enabling the device.
	 */
	if (!pm_runtime_enabled(port->dev) || pm_runtime_active(&port_dev->dev))
		port->ops->start_tx(port);
	pm_runtime_mark_last_busy(&port_dev->dev);
	pm_runtime_put_autosuspend(&port_dev->dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void uart_start(struct tty_struct *tty)
{
	struct uart_state *state = tty->driver_data;
<<<<<<< HEAD
	struct uart_port *port = state->uart_port;
	unsigned long flags;

	spin_lock_irqsave(&port->lock, flags);
	__uart_start(tty);
	spin_unlock_irqrestore(&port->lock, flags);
}

static inline void
=======
	struct uart_port *port;
	unsigned long flags;

	port = uart_port_lock(state, flags);
	__uart_start(state);
	uart_port_unlock(port, flags);
}

static void
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
uart_update_mctrl(struct uart_port *port, unsigned int set, unsigned int clear)
{
	unsigned long flags;
	unsigned int old;

<<<<<<< HEAD
	spin_lock_irqsave(&port->lock, flags);
	old = port->mctrl;
	port->mctrl = (old & ~clear) | set;
	if (old != port->mctrl)
		port->ops->set_mctrl(port, port->mctrl);
	spin_unlock_irqrestore(&port->lock, flags);
=======
	uart_port_lock_irqsave(port, &flags);
	old = port->mctrl;
	port->mctrl = (old & ~clear) | set;
	if (old != port->mctrl && !(port->rs485.flags & SER_RS485_ENABLED))
		port->ops->set_mctrl(port, port->mctrl);
	uart_port_unlock_irqrestore(port, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#define uart_set_mctrl(port, set)	uart_update_mctrl(port, set, 0)
#define uart_clear_mctrl(port, clear)	uart_update_mctrl(port, 0, clear)

<<<<<<< HEAD
=======
static void uart_port_dtr_rts(struct uart_port *uport, bool active)
{
	if (active)
		uart_set_mctrl(uport, TIOCM_DTR | TIOCM_RTS);
	else
		uart_clear_mctrl(uport, TIOCM_DTR | TIOCM_RTS);
}

/* Caller holds port mutex */
static void uart_change_line_settings(struct tty_struct *tty, struct uart_state *state,
				      const struct ktermios *old_termios)
{
	struct uart_port *uport = uart_port_check(state);
	struct ktermios *termios;
	bool old_hw_stopped;

	/*
	 * If we have no tty, termios, or the port does not exist,
	 * then we can't set the parameters for this port.
	 */
	if (!tty || uport->type == PORT_UNKNOWN)
		return;

	termios = &tty->termios;
	uport->ops->set_termios(uport, termios, old_termios);

	/*
	 * Set modem status enables based on termios cflag
	 */
	uart_port_lock_irq(uport);
	if (termios->c_cflag & CRTSCTS)
		uport->status |= UPSTAT_CTS_ENABLE;
	else
		uport->status &= ~UPSTAT_CTS_ENABLE;

	if (termios->c_cflag & CLOCAL)
		uport->status &= ~UPSTAT_DCD_ENABLE;
	else
		uport->status |= UPSTAT_DCD_ENABLE;

	/* reset sw-assisted CTS flow control based on (possibly) new mode */
	old_hw_stopped = uport->hw_stopped;
	uport->hw_stopped = uart_softcts_mode(uport) &&
			    !(uport->ops->get_mctrl(uport) & TIOCM_CTS);
	if (uport->hw_stopped != old_hw_stopped) {
		if (!old_hw_stopped)
			uport->ops->stop_tx(uport);
		else
			__uart_start(state);
	}
	uart_port_unlock_irq(uport);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Startup the port.  This will be called once per open.  All calls
 * will be serialised by the per-port mutex.
 */
static int uart_port_startup(struct tty_struct *tty, struct uart_state *state,
<<<<<<< HEAD
		int init_hw)
{
	struct uart_port *uport = state->uart_port;
	struct tty_port *port = &state->port;
=======
			     bool init_hw)
{
	struct uart_port *uport = uart_port_check(state);
	unsigned long flags;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned long page;
	int retval = 0;

	if (uport->type == PORT_UNKNOWN)
		return 1;

	/*
<<<<<<< HEAD
	 * Initialise and allocate the transmit and temporary
	 * buffer.
	 */
	if (!state->xmit.buf) {
		/* This is protected by the per port mutex */
		page = get_zeroed_page(GFP_KERNEL);
		if (!page)
			return -ENOMEM;

		state->xmit.buf = (unsigned char *) page;
		uart_circ_clear(&state->xmit);
=======
	 * Make sure the device is in D0 state.
	 */
	uart_change_pm(state, UART_PM_STATE_ON);

	/*
	 * Initialise and allocate the transmit and temporary
	 * buffer.
	 */
	page = get_zeroed_page(GFP_KERNEL);
	if (!page)
		return -ENOMEM;

	uart_port_lock(state, flags);
	if (!state->xmit.buf) {
		state->xmit.buf = (unsigned char *) page;
		uart_circ_clear(&state->xmit);
		uart_port_unlock(uport, flags);
	} else {
		uart_port_unlock(uport, flags);
		/*
		 * Do not free() the page under the port lock, see
		 * uart_shutdown().
		 */
		free_page(page);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	retval = uport->ops->startup(uport);
	if (retval == 0) {
		if (uart_console(uport) && uport->cons->cflag) {
<<<<<<< HEAD
			tty->termios->c_cflag = uport->cons->cflag;
			uport->cons->cflag = 0;
=======
			tty->termios.c_cflag = uport->cons->cflag;
			tty->termios.c_ispeed = uport->cons->ispeed;
			tty->termios.c_ospeed = uport->cons->ospeed;
			uport->cons->cflag = 0;
			uport->cons->ispeed = 0;
			uport->cons->ospeed = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		/*
		 * Initialise the hardware port settings.
		 */
<<<<<<< HEAD
		uart_change_speed(tty, state, NULL);

		if (init_hw) {
			/*
			 * Setup the RTS and DTR signals once the
			 * port is open and ready to respond.
			 */
			if (tty->termios->c_cflag & CBAUD)
				uart_set_mctrl(uport, TIOCM_RTS | TIOCM_DTR);
		}

		if (port->flags & ASYNC_CTS_FLOW) {
			spin_lock_irq(&uport->lock);
			if (!(uport->ops->get_mctrl(uport) & TIOCM_CTS))
				tty->hw_stopped = 1;
			spin_unlock_irq(&uport->lock);
		}
=======
		uart_change_line_settings(tty, state, NULL);

		/*
		 * Setup the RTS and DTR signals once the
		 * port is open and ready to respond.
		 */
		if (init_hw && C_BAUD(tty))
			uart_port_dtr_rts(uport, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/*
	 * This is to allow setserial on this port. People may want to set
	 * port/irq/type and then reconfigure the port properly if it failed
	 * now.
	 */
	if (retval && capable(CAP_SYS_ADMIN))
		return 1;

	return retval;
}

static int uart_startup(struct tty_struct *tty, struct uart_state *state,
<<<<<<< HEAD
		int init_hw)
{
	struct tty_port *port = &state->port;
	int retval;

	if (port->flags & ASYNC_INITIALIZED)
		return 0;

	/*
	 * Set the TTY IO error marker - we will only clear this
	 * once we have successfully opened the port.
	 */
	set_bit(TTY_IO_ERROR, &tty->flags);

	retval = uart_port_startup(tty, state, init_hw);
	if (!retval) {
		set_bit(ASYNCB_INITIALIZED, &port->flags);
		clear_bit(TTY_IO_ERROR, &tty->flags);
	} else if (retval > 0)
		retval = 0;

	return retval;
=======
			bool init_hw)
{
	struct tty_port *port = &state->port;
	struct uart_port *uport;
	int retval;

	if (tty_port_initialized(port))
		goto out_base_port_startup;

	retval = uart_port_startup(tty, state, init_hw);
	if (retval) {
		set_bit(TTY_IO_ERROR, &tty->flags);
		return retval;
	}

out_base_port_startup:
	uport = uart_port_check(state);
	if (!uport)
		return -EIO;

	serial_base_port_startup(uport);

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * This routine will shutdown a serial port; interrupts are disabled, and
 * DTR is dropped if the hangup on close termio flag is on.  Calls to
 * uart_shutdown are serialised by the per-port semaphore.
<<<<<<< HEAD
 */
static void uart_shutdown(struct tty_struct *tty, struct uart_state *state)
{
	struct uart_port *uport = state->uart_port;
	struct tty_port *port = &state->port;
=======
 *
 * uport == NULL if uart_port has already been removed
 */
static void uart_shutdown(struct tty_struct *tty, struct uart_state *state)
{
	struct uart_port *uport = uart_port_check(state);
	struct tty_port *port = &state->port;
	unsigned long flags;
	char *xmit_buf = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Set the TTY IO error marker
	 */
	if (tty)
		set_bit(TTY_IO_ERROR, &tty->flags);

<<<<<<< HEAD
	if (test_and_clear_bit(ASYNCB_INITIALIZED, &port->flags)) {
		/*
		 * Turn off DTR and RTS early.
		 */
		if (!tty || (tty->termios->c_cflag & HUPCL))
			uart_clear_mctrl(uport, TIOCM_DTR | TIOCM_RTS);
=======
	if (uport)
		serial_base_port_shutdown(uport);

	if (tty_port_initialized(port)) {
		tty_port_set_initialized(port, false);

		/*
		 * Turn off DTR and RTS early.
		 */
		if (uport && uart_console(uport) && tty) {
			uport->cons->cflag = tty->termios.c_cflag;
			uport->cons->ispeed = tty->termios.c_ispeed;
			uport->cons->ospeed = tty->termios.c_ospeed;
		}

		if (!tty || C_HUPCL(tty))
			uart_port_dtr_rts(uport, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		uart_port_shutdown(port);
	}

	/*
	 * It's possible for shutdown to be called after suspend if we get
	 * a DCD drop (hangup) at just the right time.  Clear suspended bit so
	 * we don't try to resume a port that has been shutdown.
	 */
<<<<<<< HEAD
	clear_bit(ASYNCB_SUSPENDED, &port->flags);

	/*
	 * Free the transmit buffer page.
	 */
	if (state->xmit.buf) {
		free_page((unsigned long)state->xmit.buf);
		state->xmit.buf = NULL;
	}
}

/**
 *	uart_update_timeout - update per-port FIFO timeout.
 *	@port:  uart_port structure describing the port
 *	@cflag: termios cflag value
 *	@baud:  speed of the port
 *
 *	Set the port FIFO timeout value.  The @cflag value should
 *	reflect the actual hardware settings.
=======
	tty_port_set_suspended(port, false);

	/*
	 * Do not free() the transmit buffer page under the port lock since
	 * this can create various circular locking scenarios. For instance,
	 * console driver may need to allocate/free a debug object, which
	 * can endup in printk() recursion.
	 */
	uart_port_lock(state, flags);
	xmit_buf = state->xmit.buf;
	state->xmit.buf = NULL;
	uart_port_unlock(uport, flags);

	free_page((unsigned long)xmit_buf);
}

/**
 * uart_update_timeout - update per-port frame timing information
 * @port: uart_port structure describing the port
 * @cflag: termios cflag value
 * @baud: speed of the port
 *
 * Set the @port frame timing information from which the FIFO timeout value is
 * derived. The @cflag value should reflect the actual hardware settings as
 * number of bits, parity, stop bits and baud rate is taken into account here.
 *
 * Locking: caller is expected to take @port->lock
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
void
uart_update_timeout(struct uart_port *port, unsigned int cflag,
		    unsigned int baud)
{
<<<<<<< HEAD
	unsigned int bits;

	/* byte size and parity */
	switch (cflag & CSIZE) {
	case CS5:
		bits = 7;
		break;
	case CS6:
		bits = 8;
		break;
	case CS7:
		bits = 9;
		break;
	default:
		bits = 10;
		break; /* CS8 */
	}

	if (cflag & CSTOPB)
		bits++;
	if (cflag & PARENB)
		bits++;

	/*
	 * The total number of bits to be transmitted in the fifo.
	 */
	bits = bits * port->fifosize;

	/*
	 * Figure the timeout to send the above number of bits.
	 * Add .02 seconds of slop
	 */
	port->timeout = (HZ * bits) / baud + HZ/50;
}

EXPORT_SYMBOL(uart_update_timeout);

/**
 *	uart_get_baud_rate - return baud rate for a particular port
 *	@port: uart_port structure describing the port in question.
 *	@termios: desired termios settings.
 *	@old: old termios (or NULL)
 *	@min: minimum acceptable baud rate
 *	@max: maximum acceptable baud rate
 *
 *	Decode the termios structure into a numeric baud rate,
 *	taking account of the magic 38400 baud rate (with spd_*
 *	flags), and mapping the %B0 rate to 9600 baud.
 *
 *	If the new baud rate is invalid, try the old termios setting.
 *	If it's still invalid, we try 9600 baud.
 *
 *	Update the @termios structure to reflect the baud rate
 *	we're actually going to be using. Don't do this for the case
 *	where B0 is requested ("hang up").
 */
unsigned int
uart_get_baud_rate(struct uart_port *port, struct ktermios *termios,
		   struct ktermios *old, unsigned int min, unsigned int max)
{
	unsigned int try, baud, altbaud = 38400;
	int hung_up = 0;
	upf_t flags = port->flags & UPF_SPD_MASK;

	if (flags == UPF_SPD_HI)
		altbaud = 57600;
	else if (flags == UPF_SPD_VHI)
		altbaud = 115200;
	else if (flags == UPF_SPD_SHI)
		altbaud = 230400;
	else if (flags == UPF_SPD_WARP)
		altbaud = 460800;
=======
	u64 temp = tty_get_frame_size(cflag);

	temp *= NSEC_PER_SEC;
	port->frame_time = (unsigned int)DIV64_U64_ROUND_UP(temp, baud);
}
EXPORT_SYMBOL(uart_update_timeout);

/**
 * uart_get_baud_rate - return baud rate for a particular port
 * @port: uart_port structure describing the port in question.
 * @termios: desired termios settings
 * @old: old termios (or %NULL)
 * @min: minimum acceptable baud rate
 * @max: maximum acceptable baud rate
 *
 * Decode the termios structure into a numeric baud rate, taking account of the
 * magic 38400 baud rate (with spd_* flags), and mapping the %B0 rate to 9600
 * baud.
 *
 * If the new baud rate is invalid, try the @old termios setting. If it's still
 * invalid, we try 9600 baud. If that is also invalid 0 is returned.
 *
 * The @termios structure is updated to reflect the baud rate we're actually
 * going to be using. Don't do this for the case where B0 is requested ("hang
 * up").
 *
 * Locking: caller dependent
 */
unsigned int
uart_get_baud_rate(struct uart_port *port, struct ktermios *termios,
		   const struct ktermios *old, unsigned int min, unsigned int max)
{
	unsigned int try;
	unsigned int baud;
	unsigned int altbaud;
	int hung_up = 0;
	upf_t flags = port->flags & UPF_SPD_MASK;

	switch (flags) {
	case UPF_SPD_HI:
		altbaud = 57600;
		break;
	case UPF_SPD_VHI:
		altbaud = 115200;
		break;
	case UPF_SPD_SHI:
		altbaud = 230400;
		break;
	case UPF_SPD_WARP:
		altbaud = 460800;
		break;
	default:
		altbaud = 38400;
		break;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	for (try = 0; try < 2; try++) {
		baud = tty_termios_baud_rate(termios);

		/*
		 * The spd_hi, spd_vhi, spd_shi, spd_warp kludge...
		 * Die! Die! Die!
		 */
		if (try == 0 && baud == 38400)
			baud = altbaud;

		/*
		 * Special case: B0 rate.
		 */
		if (baud == 0) {
			hung_up = 1;
			baud = 9600;
		}

		if (baud >= min && baud <= max)
			return baud;

		/*
		 * Oops, the quotient was zero.  Try again with
		 * the old baud rate if possible.
		 */
		termios->c_cflag &= ~CBAUD;
		if (old) {
			baud = tty_termios_baud_rate(old);
			if (!hung_up)
				tty_termios_encode_baud_rate(termios,
								baud, baud);
			old = NULL;
			continue;
		}

		/*
		 * As a last resort, if the range cannot be met then clip to
		 * the nearest chip supported rate.
		 */
		if (!hung_up) {
			if (baud <= min)
				tty_termios_encode_baud_rate(termios,
							min + 1, min + 1);
			else
				tty_termios_encode_baud_rate(termios,
							max - 1, max - 1);
		}
	}
<<<<<<< HEAD
	/* Should never happen */
	WARN_ON(1);
	return 0;
}

EXPORT_SYMBOL(uart_get_baud_rate);

/**
 *	uart_get_divisor - return uart clock divisor
 *	@port: uart_port structure describing the port.
 *	@baud: desired baud rate
 *
 *	Calculate the uart clock divisor for the port.
=======
	return 0;
}
EXPORT_SYMBOL(uart_get_baud_rate);

/**
 * uart_get_divisor - return uart clock divisor
 * @port: uart_port structure describing the port
 * @baud: desired baud rate
 *
 * Calculate the divisor (baud_base / baud) for the specified @baud,
 * appropriately rounded.
 *
 * If 38400 baud and custom divisor is selected, return the custom divisor
 * instead.
 *
 * Locking: caller dependent
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
unsigned int
uart_get_divisor(struct uart_port *port, unsigned int baud)
{
	unsigned int quot;

	/*
	 * Old custom speed handling.
	 */
	if (baud == 38400 && (port->flags & UPF_SPD_MASK) == UPF_SPD_CUST)
		quot = port->custom_divisor;
	else
		quot = DIV_ROUND_CLOSEST(port->uartclk, 16 * baud);

	return quot;
}
<<<<<<< HEAD

EXPORT_SYMBOL(uart_get_divisor);

/* FIXME: Consistent locking policy */
static void uart_change_speed(struct tty_struct *tty, struct uart_state *state,
					struct ktermios *old_termios)
{
	struct tty_port *port = &state->port;
	struct uart_port *uport = state->uart_port;
	struct ktermios *termios;

	/*
	 * If we have no tty, termios, or the port does not exist,
	 * then we can't set the parameters for this port.
	 */
	if (!tty || !tty->termios || uport->type == PORT_UNKNOWN)
		return;

	termios = tty->termios;

	/*
	 * Set flags based on termios cflag
	 */
	if (termios->c_cflag & CRTSCTS)
		set_bit(ASYNCB_CTS_FLOW, &port->flags);
	else
		clear_bit(ASYNCB_CTS_FLOW, &port->flags);

	if (termios->c_cflag & CLOCAL)
		clear_bit(ASYNCB_CHECK_CD, &port->flags);
	else
		set_bit(ASYNCB_CHECK_CD, &port->flags);

	uport->ops->set_termios(uport, termios, old_termios);
}

static inline int __uart_put_char(struct uart_port *port,
				struct circ_buf *circ, unsigned char c)
{
	unsigned long flags;
	int ret = 0;

	if (!circ->buf)
		return 0;

	spin_lock_irqsave(&port->lock, flags);
	if (uart_circ_chars_free(circ) != 0) {
=======
EXPORT_SYMBOL(uart_get_divisor);

static int uart_put_char(struct tty_struct *tty, u8 c)
{
	struct uart_state *state = tty->driver_data;
	struct uart_port *port;
	struct circ_buf *circ;
	unsigned long flags;
	int ret = 0;

	circ = &state->xmit;
	port = uart_port_lock(state, flags);
	if (!circ->buf) {
		uart_port_unlock(port, flags);
		return 0;
	}

	if (port && uart_circ_chars_free(circ) != 0) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		circ->buf[circ->head] = c;
		circ->head = (circ->head + 1) & (UART_XMIT_SIZE - 1);
		ret = 1;
	}
<<<<<<< HEAD
	spin_unlock_irqrestore(&port->lock, flags);
	return ret;
}

static int uart_put_char(struct tty_struct *tty, unsigned char ch)
{
	struct uart_state *state = tty->driver_data;

	return __uart_put_char(state->uart_port, &state->xmit, ch);
}

=======
	uart_port_unlock(port, flags);
	return ret;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void uart_flush_chars(struct tty_struct *tty)
{
	uart_start(tty);
}

<<<<<<< HEAD
static int uart_write(struct tty_struct *tty,
					const unsigned char *buf, int count)
=======
static ssize_t uart_write(struct tty_struct *tty, const u8 *buf, size_t count)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct uart_state *state = tty->driver_data;
	struct uart_port *port;
	struct circ_buf *circ;
	unsigned long flags;
	int c, ret = 0;

	/*
	 * This means you called this function _after_ the port was
	 * closed.  No cookie for you.
	 */
<<<<<<< HEAD
	if (!state) {
		WARN_ON(1);
		return -EL3HLT;
	}

	port = state->uart_port;
	circ = &state->xmit;

	if (!circ->buf)
		return 0;

	spin_lock_irqsave(&port->lock, flags);
	while (1) {
=======
	if (WARN_ON(!state))
		return -EL3HLT;

	port = uart_port_lock(state, flags);
	circ = &state->xmit;
	if (!circ->buf) {
		uart_port_unlock(port, flags);
		return 0;
	}

	while (port) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		c = CIRC_SPACE_TO_END(circ->head, circ->tail, UART_XMIT_SIZE);
		if (count < c)
			c = count;
		if (c <= 0)
			break;
		memcpy(circ->buf + circ->head, buf, c);
		circ->head = (circ->head + c) & (UART_XMIT_SIZE - 1);
		buf += c;
		count -= c;
		ret += c;
	}
<<<<<<< HEAD
	spin_unlock_irqrestore(&port->lock, flags);

	uart_start(tty);
	return ret;
}

static int uart_write_room(struct tty_struct *tty)
{
	struct uart_state *state = tty->driver_data;
	unsigned long flags;
	int ret;

	spin_lock_irqsave(&state->uart_port->lock, flags);
	ret = uart_circ_chars_free(&state->xmit);
	spin_unlock_irqrestore(&state->uart_port->lock, flags);
	return ret;
}

static int uart_chars_in_buffer(struct tty_struct *tty)
{
	struct uart_state *state = tty->driver_data;
	unsigned long flags;
	int ret;

	spin_lock_irqsave(&state->uart_port->lock, flags);
	ret = uart_circ_chars_pending(&state->xmit);
	spin_unlock_irqrestore(&state->uart_port->lock, flags);
=======

	__uart_start(state);
	uart_port_unlock(port, flags);
	return ret;
}

static unsigned int uart_write_room(struct tty_struct *tty)
{
	struct uart_state *state = tty->driver_data;
	struct uart_port *port;
	unsigned long flags;
	unsigned int ret;

	port = uart_port_lock(state, flags);
	ret = uart_circ_chars_free(&state->xmit);
	uart_port_unlock(port, flags);
	return ret;
}

static unsigned int uart_chars_in_buffer(struct tty_struct *tty)
{
	struct uart_state *state = tty->driver_data;
	struct uart_port *port;
	unsigned long flags;
	unsigned int ret;

	port = uart_port_lock(state, flags);
	ret = uart_circ_chars_pending(&state->xmit);
	uart_port_unlock(port, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

static void uart_flush_buffer(struct tty_struct *tty)
{
	struct uart_state *state = tty->driver_data;
	struct uart_port *port;
	unsigned long flags;

	/*
	 * This means you called this function _after_ the port was
	 * closed.  No cookie for you.
	 */
<<<<<<< HEAD
	if (!state) {
		WARN_ON(1);
		return;
	}

	port = state->uart_port;
	pr_debug("uart_flush_buffer(%d) called\n", tty->index);

	spin_lock_irqsave(&port->lock, flags);
	uart_circ_clear(&state->xmit);
	if (port->ops->flush_buffer)
		port->ops->flush_buffer(port);
	spin_unlock_irqrestore(&port->lock, flags);
	tty_wakeup(tty);
}

/*
 * This function is used to send a high-priority XON/XOFF character to
 * the device
 */
static void uart_send_xchar(struct tty_struct *tty, char ch)
{
	struct uart_state *state = tty->driver_data;
	struct uart_port *port = state->uart_port;
	unsigned long flags;

	if (port->ops->send_xchar)
		port->ops->send_xchar(port, ch);
	else {
		port->x_char = ch;
		if (ch) {
			spin_lock_irqsave(&port->lock, flags);
			port->ops->start_tx(port);
			spin_unlock_irqrestore(&port->lock, flags);
		}
	}
=======
	if (WARN_ON(!state))
		return;

	pr_debug("uart_flush_buffer(%d) called\n", tty->index);

	port = uart_port_lock(state, flags);
	if (!port)
		return;
	uart_circ_clear(&state->xmit);
	if (port->ops->flush_buffer)
		port->ops->flush_buffer(port);
	uart_port_unlock(port, flags);
	tty_port_tty_wakeup(&state->port);
}

/*
 * This function performs low-level write of high-priority XON/XOFF
 * character and accounting for it.
 *
 * Requires uart_port to implement .serial_out().
 */
void uart_xchar_out(struct uart_port *uport, int offset)
{
	serial_port_out(uport, offset, uport->x_char);
	uport->icount.tx++;
	uport->x_char = 0;
}
EXPORT_SYMBOL_GPL(uart_xchar_out);

/*
 * This function is used to send a high-priority XON/XOFF character to
 * the device
 */
static void uart_send_xchar(struct tty_struct *tty, u8 ch)
{
	struct uart_state *state = tty->driver_data;
	struct uart_port *port;
	unsigned long flags;

	port = uart_port_ref(state);
	if (!port)
		return;

	if (port->ops->send_xchar)
		port->ops->send_xchar(port, ch);
	else {
		uart_port_lock_irqsave(port, &flags);
		port->x_char = ch;
		if (ch)
			port->ops->start_tx(port);
		uart_port_unlock_irqrestore(port, flags);
	}
	uart_port_deref(port);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void uart_throttle(struct tty_struct *tty)
{
	struct uart_state *state = tty->driver_data;
<<<<<<< HEAD

	if (I_IXOFF(tty))
		uart_send_xchar(tty, STOP_CHAR(tty));

	if (tty->termios->c_cflag & CRTSCTS)
		uart_clear_mctrl(state->uart_port, TIOCM_RTS);
=======
	upstat_t mask = UPSTAT_SYNC_FIFO;
	struct uart_port *port;

	port = uart_port_ref(state);
	if (!port)
		return;

	if (I_IXOFF(tty))
		mask |= UPSTAT_AUTOXOFF;
	if (C_CRTSCTS(tty))
		mask |= UPSTAT_AUTORTS;

	if (port->status & mask) {
		port->ops->throttle(port);
		mask &= ~port->status;
	}

	if (mask & UPSTAT_AUTORTS)
		uart_clear_mctrl(port, TIOCM_RTS);

	if (mask & UPSTAT_AUTOXOFF)
		uart_send_xchar(tty, STOP_CHAR(tty));

	uart_port_deref(port);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void uart_unthrottle(struct tty_struct *tty)
{
	struct uart_state *state = tty->driver_data;
<<<<<<< HEAD
	struct uart_port *port = state->uart_port;

	if (I_IXOFF(tty)) {
		if (port->x_char)
			port->x_char = 0;
		else
			uart_send_xchar(tty, START_CHAR(tty));
	}

	if (tty->termios->c_cflag & CRTSCTS)
		uart_set_mctrl(port, TIOCM_RTS);
}

static int uart_get_info(struct uart_state *state,
			 struct serial_struct __user *retinfo)
{
	struct uart_port *uport = state->uart_port;
	struct tty_port *port = &state->port;
	struct serial_struct tmp;

	memset(&tmp, 0, sizeof(tmp));

	/* Ensure the state we copy is consistent and no hardware changes
	   occur as we go */
	mutex_lock(&port->mutex);

	tmp.type	    = uport->type;
	tmp.line	    = uport->line;
	tmp.port	    = uport->iobase;
	if (HIGH_BITS_OFFSET)
		tmp.port_high = (long) uport->iobase >> HIGH_BITS_OFFSET;
	tmp.irq		    = uport->irq;
	tmp.flags	    = uport->flags;
	tmp.xmit_fifo_size  = uport->fifosize;
	tmp.baud_base	    = uport->uartclk / 16;
	tmp.close_delay	    = jiffies_to_msecs(port->close_delay) / 10;
	tmp.closing_wait    = port->closing_wait == ASYNC_CLOSING_WAIT_NONE ?
				ASYNC_CLOSING_WAIT_NONE :
				jiffies_to_msecs(port->closing_wait) / 10;
	tmp.custom_divisor  = uport->custom_divisor;
	tmp.hub6	    = uport->hub6;
	tmp.io_type         = uport->iotype;
	tmp.iomem_reg_shift = uport->regshift;
	tmp.iomem_base      = (void *)(unsigned long)uport->mapbase;

	mutex_unlock(&port->mutex);

	if (copy_to_user(retinfo, &tmp, sizeof(*retinfo)))
		return -EFAULT;
	return 0;
}

static int uart_set_info(struct tty_struct *tty, struct uart_state *state,
			 struct serial_struct __user *newinfo)
{
	struct serial_struct new_serial;
	struct uart_port *uport = state->uart_port;
	struct tty_port *port = &state->port;
=======
	upstat_t mask = UPSTAT_SYNC_FIFO;
	struct uart_port *port;

	port = uart_port_ref(state);
	if (!port)
		return;

	if (I_IXOFF(tty))
		mask |= UPSTAT_AUTOXOFF;
	if (C_CRTSCTS(tty))
		mask |= UPSTAT_AUTORTS;

	if (port->status & mask) {
		port->ops->unthrottle(port);
		mask &= ~port->status;
	}

	if (mask & UPSTAT_AUTORTS)
		uart_set_mctrl(port, TIOCM_RTS);

	if (mask & UPSTAT_AUTOXOFF)
		uart_send_xchar(tty, START_CHAR(tty));

	uart_port_deref(port);
}

static int uart_get_info(struct tty_port *port, struct serial_struct *retinfo)
{
	struct uart_state *state = container_of(port, struct uart_state, port);
	struct uart_port *uport;
	int ret = -ENODEV;

	/* Initialize structure in case we error out later to prevent any stack info leakage. */
	*retinfo = (struct serial_struct){};

	/*
	 * Ensure the state we copy is consistent and no hardware changes
	 * occur as we go
	 */
	mutex_lock(&port->mutex);
	uport = uart_port_check(state);
	if (!uport)
		goto out;

	retinfo->type	    = uport->type;
	retinfo->line	    = uport->line;
	retinfo->port	    = uport->iobase;
	if (HIGH_BITS_OFFSET)
		retinfo->port_high = (long) uport->iobase >> HIGH_BITS_OFFSET;
	retinfo->irq		    = uport->irq;
	retinfo->flags	    = (__force int)uport->flags;
	retinfo->xmit_fifo_size  = uport->fifosize;
	retinfo->baud_base	    = uport->uartclk / 16;
	retinfo->close_delay	    = jiffies_to_msecs(port->close_delay) / 10;
	retinfo->closing_wait    = port->closing_wait == ASYNC_CLOSING_WAIT_NONE ?
				ASYNC_CLOSING_WAIT_NONE :
				jiffies_to_msecs(port->closing_wait) / 10;
	retinfo->custom_divisor  = uport->custom_divisor;
	retinfo->hub6	    = uport->hub6;
	retinfo->io_type         = uport->iotype;
	retinfo->iomem_reg_shift = uport->regshift;
	retinfo->iomem_base      = (void *)(unsigned long)uport->mapbase;

	ret = 0;
out:
	mutex_unlock(&port->mutex);
	return ret;
}

static int uart_get_info_user(struct tty_struct *tty,
			 struct serial_struct *ss)
{
	struct uart_state *state = tty->driver_data;
	struct tty_port *port = &state->port;

	return uart_get_info(port, ss) < 0 ? -EIO : 0;
}

static int uart_set_info(struct tty_struct *tty, struct tty_port *port,
			 struct uart_state *state,
			 struct serial_struct *new_info)
{
	struct uart_port *uport = uart_port_check(state);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned long new_port;
	unsigned int change_irq, change_port, closing_wait;
	unsigned int old_custom_divisor, close_delay;
	upf_t old_flags, new_flags;
	int retval = 0;

<<<<<<< HEAD
	if (copy_from_user(&new_serial, newinfo, sizeof(new_serial)))
		return -EFAULT;

	new_port = new_serial.port;
	if (HIGH_BITS_OFFSET)
		new_port += (unsigned long) new_serial.port_high << HIGH_BITS_OFFSET;

	new_serial.irq = irq_canonicalize(new_serial.irq);
	close_delay = msecs_to_jiffies(new_serial.close_delay * 10);
	closing_wait = new_serial.closing_wait == ASYNC_CLOSING_WAIT_NONE ?
			ASYNC_CLOSING_WAIT_NONE :
			msecs_to_jiffies(new_serial.closing_wait * 10);

	/*
	 * This semaphore protects port->count.  It is also
	 * very useful to prevent opens.  Also, take the
	 * port configuration semaphore to make sure that a
	 * module insertion/removal doesn't change anything
	 * under us.
	 */
	mutex_lock(&port->mutex);

	change_irq  = !(uport->flags & UPF_FIXED_PORT)
		&& new_serial.irq != uport->irq;
=======
	if (!uport)
		return -EIO;

	new_port = new_info->port;
	if (HIGH_BITS_OFFSET)
		new_port += (unsigned long) new_info->port_high << HIGH_BITS_OFFSET;

	new_info->irq = irq_canonicalize(new_info->irq);
	close_delay = msecs_to_jiffies(new_info->close_delay * 10);
	closing_wait = new_info->closing_wait == ASYNC_CLOSING_WAIT_NONE ?
			ASYNC_CLOSING_WAIT_NONE :
			msecs_to_jiffies(new_info->closing_wait * 10);


	change_irq  = !(uport->flags & UPF_FIXED_PORT)
		&& new_info->irq != uport->irq;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Since changing the 'type' of the port changes its resource
	 * allocations, we should treat type changes the same as
	 * IO port changes.
	 */
	change_port = !(uport->flags & UPF_FIXED_PORT)
		&& (new_port != uport->iobase ||
<<<<<<< HEAD
		    (unsigned long)new_serial.iomem_base != uport->mapbase ||
		    new_serial.hub6 != uport->hub6 ||
		    new_serial.io_type != uport->iotype ||
		    new_serial.iomem_reg_shift != uport->regshift ||
		    new_serial.type != uport->type);

	old_flags = uport->flags;
	new_flags = new_serial.flags;
=======
		    (unsigned long)new_info->iomem_base != uport->mapbase ||
		    new_info->hub6 != uport->hub6 ||
		    new_info->io_type != uport->iotype ||
		    new_info->iomem_reg_shift != uport->regshift ||
		    new_info->type != uport->type);

	old_flags = uport->flags;
	new_flags = (__force upf_t)new_info->flags;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	old_custom_divisor = uport->custom_divisor;

	if (!capable(CAP_SYS_ADMIN)) {
		retval = -EPERM;
		if (change_irq || change_port ||
<<<<<<< HEAD
		    (new_serial.baud_base != uport->uartclk / 16) ||
		    (close_delay != port->close_delay) ||
		    (closing_wait != port->closing_wait) ||
		    (new_serial.xmit_fifo_size &&
		     new_serial.xmit_fifo_size != uport->fifosize) ||
=======
		    (new_info->baud_base != uport->uartclk / 16) ||
		    (close_delay != port->close_delay) ||
		    (closing_wait != port->closing_wait) ||
		    (new_info->xmit_fifo_size &&
		     new_info->xmit_fifo_size != uport->fifosize) ||
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		    (((new_flags ^ old_flags) & ~UPF_USR_MASK) != 0))
			goto exit;
		uport->flags = ((uport->flags & ~UPF_USR_MASK) |
			       (new_flags & UPF_USR_MASK));
<<<<<<< HEAD
		uport->custom_divisor = new_serial.custom_divisor;
		goto check_and_exit;
	}

=======
		uport->custom_divisor = new_info->custom_divisor;
		goto check_and_exit;
	}

	if (change_irq || change_port) {
		retval = security_locked_down(LOCKDOWN_TIOCSSERIAL);
		if (retval)
			goto exit;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Ask the low level driver to verify the settings.
	 */
	if (uport->ops->verify_port)
<<<<<<< HEAD
		retval = uport->ops->verify_port(uport, &new_serial);

	if ((new_serial.irq >= nr_irqs) || (new_serial.irq < 0) ||
	    (new_serial.baud_base < 9600))
=======
		retval = uport->ops->verify_port(uport, new_info);

	if ((new_info->irq >= nr_irqs) || (new_info->irq < 0) ||
	    (new_info->baud_base < 9600))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		retval = -EINVAL;

	if (retval)
		goto exit;

	if (change_port || change_irq) {
		retval = -EBUSY;

		/*
		 * Make sure that we are the sole user of this port.
		 */
		if (tty_port_users(port) > 1)
			goto exit;

		/*
		 * We need to shutdown the serial port at the old
		 * port/type/irq combination.
		 */
		uart_shutdown(tty, state);
	}

	if (change_port) {
		unsigned long old_iobase, old_mapbase;
		unsigned int old_type, old_iotype, old_hub6, old_shift;

		old_iobase = uport->iobase;
		old_mapbase = uport->mapbase;
		old_type = uport->type;
		old_hub6 = uport->hub6;
		old_iotype = uport->iotype;
		old_shift = uport->regshift;

		/*
		 * Free and release old regions
		 */
<<<<<<< HEAD
		if (old_type != PORT_UNKNOWN)
			uport->ops->release_port(uport);

		uport->iobase = new_port;
		uport->type = new_serial.type;
		uport->hub6 = new_serial.hub6;
		uport->iotype = new_serial.io_type;
		uport->regshift = new_serial.iomem_reg_shift;
		uport->mapbase = (unsigned long)new_serial.iomem_base;
=======
		if (old_type != PORT_UNKNOWN && uport->ops->release_port)
			uport->ops->release_port(uport);

		uport->iobase = new_port;
		uport->type = new_info->type;
		uport->hub6 = new_info->hub6;
		uport->iotype = new_info->io_type;
		uport->regshift = new_info->iomem_reg_shift;
		uport->mapbase = (unsigned long)new_info->iomem_base;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/*
		 * Claim and map the new regions
		 */
<<<<<<< HEAD
		if (uport->type != PORT_UNKNOWN) {
=======
		if (uport->type != PORT_UNKNOWN && uport->ops->request_port) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			retval = uport->ops->request_port(uport);
		} else {
			/* Always success - Jean II */
			retval = 0;
		}

		/*
		 * If we fail to request resources for the
		 * new port, try to restore the old settings.
		 */
<<<<<<< HEAD
		if (retval && old_type != PORT_UNKNOWN) {
=======
		if (retval) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			uport->iobase = old_iobase;
			uport->type = old_type;
			uport->hub6 = old_hub6;
			uport->iotype = old_iotype;
			uport->regshift = old_shift;
			uport->mapbase = old_mapbase;
<<<<<<< HEAD
			retval = uport->ops->request_port(uport);
			/*
			 * If we failed to restore the old settings,
			 * we fail like this.
			 */
			if (retval)
				uport->type = PORT_UNKNOWN;

			/*
			 * We failed anyway.
			 */
			retval = -EBUSY;
=======

			if (old_type != PORT_UNKNOWN) {
				retval = uport->ops->request_port(uport);
				/*
				 * If we failed to restore the old settings,
				 * we fail like this.
				 */
				if (retval)
					uport->type = PORT_UNKNOWN;

				/*
				 * We failed anyway.
				 */
				retval = -EBUSY;
			}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/* Added to return the correct error -Ram Gupta */
			goto exit;
		}
	}

	if (change_irq)
<<<<<<< HEAD
		uport->irq      = new_serial.irq;
	if (!(uport->flags & UPF_FIXED_PORT))
		uport->uartclk  = new_serial.baud_base * 16;
	uport->flags            = (uport->flags & ~UPF_CHANGE_MASK) |
				 (new_flags & UPF_CHANGE_MASK);
	uport->custom_divisor   = new_serial.custom_divisor;
	port->close_delay     = close_delay;
	port->closing_wait    = closing_wait;
	if (new_serial.xmit_fifo_size)
		uport->fifosize = new_serial.xmit_fifo_size;
	if (port->tty)
		port->tty->low_latency =
			(uport->flags & UPF_LOW_LATENCY) ? 1 : 0;
=======
		uport->irq      = new_info->irq;
	if (!(uport->flags & UPF_FIXED_PORT))
		uport->uartclk  = new_info->baud_base * 16;
	uport->flags            = (uport->flags & ~UPF_CHANGE_MASK) |
				 (new_flags & UPF_CHANGE_MASK);
	uport->custom_divisor   = new_info->custom_divisor;
	port->close_delay     = close_delay;
	port->closing_wait    = closing_wait;
	if (new_info->xmit_fifo_size)
		uport->fifosize = new_info->xmit_fifo_size;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

 check_and_exit:
	retval = 0;
	if (uport->type == PORT_UNKNOWN)
		goto exit;
<<<<<<< HEAD
	if (port->flags & ASYNC_INITIALIZED) {
=======
	if (tty_port_initialized(port)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (((old_flags ^ uport->flags) & UPF_SPD_MASK) ||
		    old_custom_divisor != uport->custom_divisor) {
			/*
			 * If they're setting up a custom divisor or speed,
<<<<<<< HEAD
			 * instead of clearing it, then bitch about it. No
			 * need to rate-limit; it's CAP_SYS_ADMIN only.
			 */
			if (uport->flags & UPF_SPD_MASK) {
				char buf[64];
				printk(KERN_NOTICE
				       "%s sets custom speed on %s. This "
				       "is deprecated.\n", current->comm,
				       tty_name(port->tty, buf));
			}
			uart_change_speed(tty, state, NULL);
		}
	} else
		retval = uart_startup(tty, state, 1);
 exit:
	mutex_unlock(&port->mutex);
=======
			 * instead of clearing it, then bitch about it.
			 */
			if (uport->flags & UPF_SPD_MASK) {
				dev_notice_ratelimited(uport->dev,
				       "%s sets custom speed on %s. This is deprecated.\n",
				      current->comm,
				      tty_name(port->tty));
			}
			uart_change_line_settings(tty, state, NULL);
		}
	} else {
		retval = uart_startup(tty, state, true);
		if (retval == 0)
			tty_port_set_initialized(port, true);
		if (retval > 0)
			retval = 0;
	}
 exit:
	return retval;
}

static int uart_set_info_user(struct tty_struct *tty, struct serial_struct *ss)
{
	struct uart_state *state = tty->driver_data;
	struct tty_port *port = &state->port;
	int retval;

	down_write(&tty->termios_rwsem);
	/*
	 * This semaphore protects port->count.  It is also
	 * very useful to prevent opens.  Also, take the
	 * port configuration semaphore to make sure that a
	 * module insertion/removal doesn't change anything
	 * under us.
	 */
	mutex_lock(&port->mutex);
	retval = uart_set_info(tty, port, state, ss);
	mutex_unlock(&port->mutex);
	up_write(&tty->termios_rwsem);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return retval;
}

/**
<<<<<<< HEAD
 *	uart_get_lsr_info	-	get line status register info
 *	@tty: tty associated with the UART
 *	@state: UART being queried
 *	@value: returned modem value
 *
 *	Note: uart_ioctl protects us against hangups.
=======
 * uart_get_lsr_info - get line status register info
 * @tty: tty associated with the UART
 * @state: UART being queried
 * @value: returned modem value
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
static int uart_get_lsr_info(struct tty_struct *tty,
			struct uart_state *state, unsigned int __user *value)
{
<<<<<<< HEAD
	struct uart_port *uport = state->uart_port;
=======
	struct uart_port *uport = uart_port_check(state);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned int result;

	result = uport->ops->tx_empty(uport);

	/*
	 * If we're about to load something into the transmit
	 * register, we'll pretend the transmitter isn't empty to
	 * avoid a race condition (depending on when the transmit
	 * interrupt happens).
	 */
	if (uport->x_char ||
	    ((uart_circ_chars_pending(&state->xmit) > 0) &&
<<<<<<< HEAD
	     !tty->stopped && !tty->hw_stopped))
=======
	     !uart_tx_stopped(uport)))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		result &= ~TIOCSER_TEMT;

	return put_user(result, value);
}

static int uart_tiocmget(struct tty_struct *tty)
{
	struct uart_state *state = tty->driver_data;
	struct tty_port *port = &state->port;
<<<<<<< HEAD
	struct uart_port *uport = state->uart_port;
	int result = -EIO;

	mutex_lock(&port->mutex);
	if (!(tty->flags & (1 << TTY_IO_ERROR))) {
		result = uport->mctrl;
		spin_lock_irq(&uport->lock);
		result |= uport->ops->get_mctrl(uport);
		spin_unlock_irq(&uport->lock);
	}
	mutex_unlock(&port->mutex);

=======
	struct uart_port *uport;
	int result = -EIO;

	mutex_lock(&port->mutex);
	uport = uart_port_check(state);
	if (!uport)
		goto out;

	if (!tty_io_error(tty)) {
		uart_port_lock_irq(uport);
		result = uport->mctrl;
		result |= uport->ops->get_mctrl(uport);
		uart_port_unlock_irq(uport);
	}
out:
	mutex_unlock(&port->mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return result;
}

static int
uart_tiocmset(struct tty_struct *tty, unsigned int set, unsigned int clear)
{
	struct uart_state *state = tty->driver_data;
<<<<<<< HEAD
	struct uart_port *uport = state->uart_port;
	struct tty_port *port = &state->port;
	int ret = -EIO;

	mutex_lock(&port->mutex);
	if (!(tty->flags & (1 << TTY_IO_ERROR))) {
		uart_update_mctrl(uport, set, clear);
		ret = 0;
	}
=======
	struct tty_port *port = &state->port;
	struct uart_port *uport;
	int ret = -EIO;

	mutex_lock(&port->mutex);
	uport = uart_port_check(state);
	if (!uport)
		goto out;

	if (!tty_io_error(tty)) {
		uart_update_mctrl(uport, set, clear);
		ret = 0;
	}
out:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mutex_unlock(&port->mutex);
	return ret;
}

static int uart_break_ctl(struct tty_struct *tty, int break_state)
{
	struct uart_state *state = tty->driver_data;
	struct tty_port *port = &state->port;
<<<<<<< HEAD
	struct uart_port *uport = state->uart_port;

	mutex_lock(&port->mutex);

	if (uport->type != PORT_UNKNOWN)
		uport->ops->break_ctl(uport, break_state);

	mutex_unlock(&port->mutex);
	return 0;
}

static int uart_do_autoconfig(struct tty_struct *tty,struct uart_state *state)
{
	struct uart_port *uport = state->uart_port;
	struct tty_port *port = &state->port;
=======
	struct uart_port *uport;
	int ret = -EIO;

	mutex_lock(&port->mutex);
	uport = uart_port_check(state);
	if (!uport)
		goto out;

	if (uport->type != PORT_UNKNOWN && uport->ops->break_ctl)
		uport->ops->break_ctl(uport, break_state);
	ret = 0;
out:
	mutex_unlock(&port->mutex);
	return ret;
}

static int uart_do_autoconfig(struct tty_struct *tty, struct uart_state *state)
{
	struct tty_port *port = &state->port;
	struct uart_port *uport;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int flags, ret;

	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;

	/*
	 * Take the per-port semaphore.  This prevents count from
	 * changing, and hence any extra opens of the port while
	 * we're auto-configuring.
	 */
	if (mutex_lock_interruptible(&port->mutex))
		return -ERESTARTSYS;

<<<<<<< HEAD
=======
	uport = uart_port_check(state);
	if (!uport) {
		ret = -EIO;
		goto out;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ret = -EBUSY;
	if (tty_port_users(port) == 1) {
		uart_shutdown(tty, state);

		/*
		 * If we already have a port type configured,
		 * we must release its resources.
		 */
<<<<<<< HEAD
		if (uport->type != PORT_UNKNOWN)
=======
		if (uport->type != PORT_UNKNOWN && uport->ops->release_port)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			uport->ops->release_port(uport);

		flags = UART_CONFIG_TYPE;
		if (uport->flags & UPF_AUTO_IRQ)
			flags |= UART_CONFIG_IRQ;

		/*
		 * This will claim the ports resources if
		 * a port is found.
		 */
		uport->ops->config_port(uport, flags);

<<<<<<< HEAD
		ret = uart_startup(tty, state, 1);
	}
=======
		ret = uart_startup(tty, state, true);
		if (ret == 0)
			tty_port_set_initialized(port, true);
		if (ret > 0)
			ret = 0;
	}
out:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mutex_unlock(&port->mutex);
	return ret;
}

<<<<<<< HEAD
=======
static void uart_enable_ms(struct uart_port *uport)
{
	/*
	 * Force modem status interrupts on
	 */
	if (uport->ops->enable_ms)
		uport->ops->enable_ms(uport);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Wait for any of the 4 modem inputs (DCD,RI,DSR,CTS) to change
 * - mask passed in arg for lines of interest
 *   (use |'ed TIOCM_RNG/DSR/CD/CTS for masking)
 * Caller should use TIOCGICOUNT to see which one it was
 *
 * FIXME: This wants extracting into a common all driver implementation
 * of TIOCMWAIT using tty_port.
 */
<<<<<<< HEAD
static int
uart_wait_modem_status(struct uart_state *state, unsigned long arg)
{
	struct uart_port *uport = state->uart_port;
=======
static int uart_wait_modem_status(struct uart_state *state, unsigned long arg)
{
	struct uart_port *uport;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct tty_port *port = &state->port;
	DECLARE_WAITQUEUE(wait, current);
	struct uart_icount cprev, cnow;
	int ret;

	/*
	 * note the counters on entry
	 */
<<<<<<< HEAD
	spin_lock_irq(&uport->lock);
	memcpy(&cprev, &uport->icount, sizeof(struct uart_icount));

	/*
	 * Force modem status interrupts on
	 */
	uport->ops->enable_ms(uport);
	spin_unlock_irq(&uport->lock);

	add_wait_queue(&port->delta_msr_wait, &wait);
	for (;;) {
		spin_lock_irq(&uport->lock);
		memcpy(&cnow, &uport->icount, sizeof(struct uart_icount));
		spin_unlock_irq(&uport->lock);
=======
	uport = uart_port_ref(state);
	if (!uport)
		return -EIO;
	uart_port_lock_irq(uport);
	memcpy(&cprev, &uport->icount, sizeof(struct uart_icount));
	uart_enable_ms(uport);
	uart_port_unlock_irq(uport);

	add_wait_queue(&port->delta_msr_wait, &wait);
	for (;;) {
		uart_port_lock_irq(uport);
		memcpy(&cnow, &uport->icount, sizeof(struct uart_icount));
		uart_port_unlock_irq(uport);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		set_current_state(TASK_INTERRUPTIBLE);

		if (((arg & TIOCM_RNG) && (cnow.rng != cprev.rng)) ||
		    ((arg & TIOCM_DSR) && (cnow.dsr != cprev.dsr)) ||
		    ((arg & TIOCM_CD)  && (cnow.dcd != cprev.dcd)) ||
		    ((arg & TIOCM_CTS) && (cnow.cts != cprev.cts))) {
			ret = 0;
			break;
		}

		schedule();

		/* see if a signal did it */
		if (signal_pending(current)) {
			ret = -ERESTARTSYS;
			break;
		}

		cprev = cnow;
	}
<<<<<<< HEAD

	current->state = TASK_RUNNING;
	remove_wait_queue(&port->delta_msr_wait, &wait);
=======
	__set_current_state(TASK_RUNNING);
	remove_wait_queue(&port->delta_msr_wait, &wait);
	uart_port_deref(uport);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ret;
}

/*
 * Get counter of input serial line interrupts (DCD,RI,DSR,CTS)
 * Return: write counters to the user passed counter struct
 * NB: both 1->0 and 0->1 transitions are counted except for
 *     RI where only 0->1 is counted.
 */
static int uart_get_icount(struct tty_struct *tty,
			  struct serial_icounter_struct *icount)
{
	struct uart_state *state = tty->driver_data;
	struct uart_icount cnow;
<<<<<<< HEAD
	struct uart_port *uport = state->uart_port;

	spin_lock_irq(&uport->lock);
	memcpy(&cnow, &uport->icount, sizeof(struct uart_icount));
	spin_unlock_irq(&uport->lock);
=======
	struct uart_port *uport;

	uport = uart_port_ref(state);
	if (!uport)
		return -EIO;
	uart_port_lock_irq(uport);
	memcpy(&cnow, &uport->icount, sizeof(struct uart_icount));
	uart_port_unlock_irq(uport);
	uart_port_deref(uport);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	icount->cts         = cnow.cts;
	icount->dsr         = cnow.dsr;
	icount->rng         = cnow.rng;
	icount->dcd         = cnow.dcd;
	icount->rx          = cnow.rx;
	icount->tx          = cnow.tx;
	icount->frame       = cnow.frame;
	icount->overrun     = cnow.overrun;
	icount->parity      = cnow.parity;
	icount->brk         = cnow.brk;
	icount->buf_overrun = cnow.buf_overrun;

	return 0;
}

<<<<<<< HEAD
=======
#define SER_RS485_LEGACY_FLAGS	(SER_RS485_ENABLED | SER_RS485_RTS_ON_SEND | \
				 SER_RS485_RTS_AFTER_SEND | SER_RS485_RX_DURING_TX | \
				 SER_RS485_TERMINATE_BUS)

static int uart_check_rs485_flags(struct uart_port *port, struct serial_rs485 *rs485)
{
	u32 flags = rs485->flags;

	/* Don't return -EINVAL for unsupported legacy flags */
	flags &= ~SER_RS485_LEGACY_FLAGS;

	/*
	 * For any bit outside of the legacy ones that is not supported by
	 * the driver, return -EINVAL.
	 */
	if (flags & ~port->rs485_supported.flags)
		return -EINVAL;

	/* Asking for address w/o addressing mode? */
	if (!(rs485->flags & SER_RS485_ADDRB) &&
	    (rs485->flags & (SER_RS485_ADDR_RECV|SER_RS485_ADDR_DEST)))
		return -EINVAL;

	/* Address given but not enabled? */
	if (!(rs485->flags & SER_RS485_ADDR_RECV) && rs485->addr_recv)
		return -EINVAL;
	if (!(rs485->flags & SER_RS485_ADDR_DEST) && rs485->addr_dest)
		return -EINVAL;

	return 0;
}

static void uart_sanitize_serial_rs485_delays(struct uart_port *port,
					      struct serial_rs485 *rs485)
{
	if (!port->rs485_supported.delay_rts_before_send) {
		if (rs485->delay_rts_before_send) {
			dev_warn_ratelimited(port->dev,
				"%s (%d): RTS delay before sending not supported\n",
				port->name, port->line);
		}
		rs485->delay_rts_before_send = 0;
	} else if (rs485->delay_rts_before_send > RS485_MAX_RTS_DELAY) {
		rs485->delay_rts_before_send = RS485_MAX_RTS_DELAY;
		dev_warn_ratelimited(port->dev,
			"%s (%d): RTS delay before sending clamped to %u ms\n",
			port->name, port->line, rs485->delay_rts_before_send);
	}

	if (!port->rs485_supported.delay_rts_after_send) {
		if (rs485->delay_rts_after_send) {
			dev_warn_ratelimited(port->dev,
				"%s (%d): RTS delay after sending not supported\n",
				port->name, port->line);
		}
		rs485->delay_rts_after_send = 0;
	} else if (rs485->delay_rts_after_send > RS485_MAX_RTS_DELAY) {
		rs485->delay_rts_after_send = RS485_MAX_RTS_DELAY;
		dev_warn_ratelimited(port->dev,
			"%s (%d): RTS delay after sending clamped to %u ms\n",
			port->name, port->line, rs485->delay_rts_after_send);
	}
}

static void uart_sanitize_serial_rs485(struct uart_port *port, struct serial_rs485 *rs485)
{
	u32 supported_flags = port->rs485_supported.flags;

	if (!(rs485->flags & SER_RS485_ENABLED)) {
		memset(rs485, 0, sizeof(*rs485));
		return;
	}

	/* Clear other RS485 flags but SER_RS485_TERMINATE_BUS and return if enabling RS422 */
	if (rs485->flags & SER_RS485_MODE_RS422) {
		rs485->flags &= (SER_RS485_ENABLED | SER_RS485_MODE_RS422 | SER_RS485_TERMINATE_BUS);
		return;
	}

	rs485->flags &= supported_flags;

	/* Pick sane settings if the user hasn't */
	if (!(rs485->flags & SER_RS485_RTS_ON_SEND) ==
	    !(rs485->flags & SER_RS485_RTS_AFTER_SEND)) {
		if (supported_flags & SER_RS485_RTS_ON_SEND) {
			rs485->flags |= SER_RS485_RTS_ON_SEND;
			rs485->flags &= ~SER_RS485_RTS_AFTER_SEND;

			dev_warn_ratelimited(port->dev,
				"%s (%d): invalid RTS setting, using RTS_ON_SEND instead\n",
				port->name, port->line);
		} else {
			rs485->flags |= SER_RS485_RTS_AFTER_SEND;
			rs485->flags &= ~SER_RS485_RTS_ON_SEND;

			dev_warn_ratelimited(port->dev,
				"%s (%d): invalid RTS setting, using RTS_AFTER_SEND instead\n",
				port->name, port->line);
		}
	}

	uart_sanitize_serial_rs485_delays(port, rs485);

	/* Return clean padding area to userspace */
	memset(rs485->padding0, 0, sizeof(rs485->padding0));
	memset(rs485->padding1, 0, sizeof(rs485->padding1));
}

static void uart_set_rs485_termination(struct uart_port *port,
				       const struct serial_rs485 *rs485)
{
	if (!(rs485->flags & SER_RS485_ENABLED))
		return;

	gpiod_set_value_cansleep(port->rs485_term_gpio,
				 !!(rs485->flags & SER_RS485_TERMINATE_BUS));
}

static void uart_set_rs485_rx_during_tx(struct uart_port *port,
					const struct serial_rs485 *rs485)
{
	if (!(rs485->flags & SER_RS485_ENABLED))
		return;

	gpiod_set_value_cansleep(port->rs485_rx_during_tx_gpio,
				 !!(rs485->flags & SER_RS485_RX_DURING_TX));
}

static int uart_rs485_config(struct uart_port *port)
{
	struct serial_rs485 *rs485 = &port->rs485;
	unsigned long flags;
	int ret;

	if (!(rs485->flags & SER_RS485_ENABLED))
		return 0;

	uart_sanitize_serial_rs485(port, rs485);
	uart_set_rs485_termination(port, rs485);
	uart_set_rs485_rx_during_tx(port, rs485);

	uart_port_lock_irqsave(port, &flags);
	ret = port->rs485_config(port, NULL, rs485);
	uart_port_unlock_irqrestore(port, flags);
	if (ret) {
		memset(rs485, 0, sizeof(*rs485));
		/* unset GPIOs */
		gpiod_set_value_cansleep(port->rs485_term_gpio, 0);
		gpiod_set_value_cansleep(port->rs485_rx_during_tx_gpio, 0);
	}

	return ret;
}

static int uart_get_rs485_config(struct uart_port *port,
			 struct serial_rs485 __user *rs485)
{
	unsigned long flags;
	struct serial_rs485 aux;

	uart_port_lock_irqsave(port, &flags);
	aux = port->rs485;
	uart_port_unlock_irqrestore(port, flags);

	if (copy_to_user(rs485, &aux, sizeof(aux)))
		return -EFAULT;

	return 0;
}

static int uart_set_rs485_config(struct tty_struct *tty, struct uart_port *port,
			 struct serial_rs485 __user *rs485_user)
{
	struct serial_rs485 rs485;
	int ret;
	unsigned long flags;

	if (!(port->rs485_supported.flags & SER_RS485_ENABLED))
		return -ENOTTY;

	if (copy_from_user(&rs485, rs485_user, sizeof(*rs485_user)))
		return -EFAULT;

	ret = uart_check_rs485_flags(port, &rs485);
	if (ret)
		return ret;
	uart_sanitize_serial_rs485(port, &rs485);
	uart_set_rs485_termination(port, &rs485);
	uart_set_rs485_rx_during_tx(port, &rs485);

	uart_port_lock_irqsave(port, &flags);
	ret = port->rs485_config(port, &tty->termios, &rs485);
	if (!ret) {
		port->rs485 = rs485;

		/* Reset RTS and other mctrl lines when disabling RS485 */
		if (!(rs485.flags & SER_RS485_ENABLED))
			port->ops->set_mctrl(port, port->mctrl);
	}
	uart_port_unlock_irqrestore(port, flags);
	if (ret) {
		/* restore old GPIO settings */
		gpiod_set_value_cansleep(port->rs485_term_gpio,
			!!(port->rs485.flags & SER_RS485_TERMINATE_BUS));
		gpiod_set_value_cansleep(port->rs485_rx_during_tx_gpio,
			!!(port->rs485.flags & SER_RS485_RX_DURING_TX));
		return ret;
	}

	if (copy_to_user(rs485_user, &port->rs485, sizeof(port->rs485)))
		return -EFAULT;

	return 0;
}

static int uart_get_iso7816_config(struct uart_port *port,
				   struct serial_iso7816 __user *iso7816)
{
	unsigned long flags;
	struct serial_iso7816 aux;

	if (!port->iso7816_config)
		return -ENOTTY;

	uart_port_lock_irqsave(port, &flags);
	aux = port->iso7816;
	uart_port_unlock_irqrestore(port, flags);

	if (copy_to_user(iso7816, &aux, sizeof(aux)))
		return -EFAULT;

	return 0;
}

static int uart_set_iso7816_config(struct uart_port *port,
				   struct serial_iso7816 __user *iso7816_user)
{
	struct serial_iso7816 iso7816;
	int i, ret;
	unsigned long flags;

	if (!port->iso7816_config)
		return -ENOTTY;

	if (copy_from_user(&iso7816, iso7816_user, sizeof(*iso7816_user)))
		return -EFAULT;

	/*
	 * There are 5 words reserved for future use. Check that userspace
	 * doesn't put stuff in there to prevent breakages in the future.
	 */
	for (i = 0; i < ARRAY_SIZE(iso7816.reserved); i++)
		if (iso7816.reserved[i])
			return -EINVAL;

	uart_port_lock_irqsave(port, &flags);
	ret = port->iso7816_config(port, &iso7816);
	uart_port_unlock_irqrestore(port, flags);
	if (ret)
		return ret;

	if (copy_to_user(iso7816_user, &port->iso7816, sizeof(port->iso7816)))
		return -EFAULT;

	return 0;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Called via sys_ioctl.  We can use spin_lock_irq() here.
 */
static int
<<<<<<< HEAD
uart_ioctl(struct tty_struct *tty, unsigned int cmd,
	   unsigned long arg)
{
	struct uart_state *state = tty->driver_data;
	struct tty_port *port = &state->port;
=======
uart_ioctl(struct tty_struct *tty, unsigned int cmd, unsigned long arg)
{
	struct uart_state *state = tty->driver_data;
	struct tty_port *port = &state->port;
	struct uart_port *uport;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	void __user *uarg = (void __user *)arg;
	int ret = -ENOIOCTLCMD;


	/*
	 * These ioctls don't rely on the hardware to be present.
	 */
	switch (cmd) {
<<<<<<< HEAD
	case TIOCGSERIAL:
		ret = uart_get_info(state, uarg);
		break;

	case TIOCSSERIAL:
		ret = uart_set_info(tty, state, uarg);
		break;

	case TIOCSERCONFIG:
		ret = uart_do_autoconfig(tty, state);
		break;

	case TIOCSERGWILD: /* obsolete */
	case TIOCSERSWILD: /* obsolete */
		ret = 0;
=======
	case TIOCSERCONFIG:
		down_write(&tty->termios_rwsem);
		ret = uart_do_autoconfig(tty, state);
		up_write(&tty->termios_rwsem);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	}

	if (ret != -ENOIOCTLCMD)
		goto out;

<<<<<<< HEAD
	if (tty->flags & (1 << TTY_IO_ERROR)) {
=======
	if (tty_io_error(tty)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = -EIO;
		goto out;
	}

	/*
	 * The following should only be used when hardware is present.
	 */
	switch (cmd) {
	case TIOCMIWAIT:
		ret = uart_wait_modem_status(state, arg);
		break;
	}

	if (ret != -ENOIOCTLCMD)
		goto out;

<<<<<<< HEAD
	mutex_lock(&port->mutex);

	if (tty->flags & (1 << TTY_IO_ERROR)) {
=======
	/* rs485_config requires more locking than others */
	if (cmd == TIOCSRS485)
		down_write(&tty->termios_rwsem);

	mutex_lock(&port->mutex);
	uport = uart_port_check(state);

	if (!uport || tty_io_error(tty)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = -EIO;
		goto out_up;
	}

	/*
	 * All these rely on hardware being present and need to be
	 * protected against the tty being hung up.
	 */
<<<<<<< HEAD
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	switch (cmd) {
	case TIOCSERGETLSR: /* Get line status register */
		ret = uart_get_lsr_info(tty, state, uarg);
		break;

<<<<<<< HEAD
	default: {
		struct uart_port *uport = state->uart_port;
=======
	case TIOCGRS485:
		ret = uart_get_rs485_config(uport, uarg);
		break;

	case TIOCSRS485:
		ret = uart_set_rs485_config(tty, uport, uarg);
		break;

	case TIOCSISO7816:
		ret = uart_set_iso7816_config(state->uart_port, uarg);
		break;

	case TIOCGISO7816:
		ret = uart_get_iso7816_config(state->uart_port, uarg);
		break;
	default:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (uport->ops->ioctl)
			ret = uport->ops->ioctl(uport, cmd, arg);
		break;
	}
<<<<<<< HEAD
	}
out_up:
	mutex_unlock(&port->mutex);
=======
out_up:
	mutex_unlock(&port->mutex);
	if (cmd == TIOCSRS485)
		up_write(&tty->termios_rwsem);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	return ret;
}

static void uart_set_ldisc(struct tty_struct *tty)
{
	struct uart_state *state = tty->driver_data;
<<<<<<< HEAD
	struct uart_port *uport = state->uart_port;

	if (uport->ops->set_ldisc)
		uport->ops->set_ldisc(uport, tty->termios->c_line);
}

static void uart_set_termios(struct tty_struct *tty,
						struct ktermios *old_termios)
{
	struct uart_state *state = tty->driver_data;
	unsigned long flags;
	unsigned int cflag = tty->termios->c_cflag;

=======
	struct uart_port *uport;
	struct tty_port *port = &state->port;

	if (!tty_port_initialized(port))
		return;

	mutex_lock(&state->port.mutex);
	uport = uart_port_check(state);
	if (uport && uport->ops->set_ldisc)
		uport->ops->set_ldisc(uport, &tty->termios);
	mutex_unlock(&state->port.mutex);
}

static void uart_set_termios(struct tty_struct *tty,
			     const struct ktermios *old_termios)
{
	struct uart_state *state = tty->driver_data;
	struct uart_port *uport;
	unsigned int cflag = tty->termios.c_cflag;
	unsigned int iflag_mask = IGNBRK|BRKINT|IGNPAR|PARMRK|INPCK;
	bool sw_changed = false;

	mutex_lock(&state->port.mutex);
	uport = uart_port_check(state);
	if (!uport)
		goto out;

	/*
	 * Drivers doing software flow control also need to know
	 * about changes to these input settings.
	 */
	if (uport->flags & UPF_SOFT_FLOW) {
		iflag_mask |= IXANY|IXON|IXOFF;
		sw_changed =
		   tty->termios.c_cc[VSTART] != old_termios->c_cc[VSTART] ||
		   tty->termios.c_cc[VSTOP] != old_termios->c_cc[VSTOP];
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * These are the bits that are used to setup various
	 * flags in the low level driver. We can ignore the Bfoo
	 * bits in c_cflag; c_[io]speed will always be set
	 * appropriately by set_termios() in tty_ioctl.c
	 */
<<<<<<< HEAD
#define RELEVANT_IFLAG(iflag)	((iflag) & (IGNBRK|BRKINT|IGNPAR|PARMRK|INPCK))
	if ((cflag ^ old_termios->c_cflag) == 0 &&
	    tty->termios->c_ospeed == old_termios->c_ospeed &&
	    tty->termios->c_ispeed == old_termios->c_ispeed &&
	    RELEVANT_IFLAG(tty->termios->c_iflag ^ old_termios->c_iflag) == 0) {
		return;
	}

	uart_change_speed(tty, state, old_termios);

	/* Handle transition to B0 status */
	if ((old_termios->c_cflag & CBAUD) && !(cflag & CBAUD))
		uart_clear_mctrl(state->uart_port, TIOCM_RTS | TIOCM_DTR);
	/* Handle transition away from B0 status */
	else if (!(old_termios->c_cflag & CBAUD) && (cflag & CBAUD)) {
		unsigned int mask = TIOCM_DTR;
		if (!(cflag & CRTSCTS) ||
		    !test_bit(TTY_THROTTLED, &tty->flags))
			mask |= TIOCM_RTS;
		uart_set_mctrl(state->uart_port, mask);
	}

	/* Handle turning off CRTSCTS */
	if ((old_termios->c_cflag & CRTSCTS) && !(cflag & CRTSCTS)) {
		spin_lock_irqsave(&state->uart_port->lock, flags);
		tty->hw_stopped = 0;
		__uart_start(tty);
		spin_unlock_irqrestore(&state->uart_port->lock, flags);
	}
	/* Handle turning on CRTSCTS */
	else if (!(old_termios->c_cflag & CRTSCTS) && (cflag & CRTSCTS)) {
		spin_lock_irqsave(&state->uart_port->lock, flags);
		if (!(state->uart_port->ops->get_mctrl(state->uart_port) & TIOCM_CTS)) {
			tty->hw_stopped = 1;
			state->uart_port->ops->stop_tx(state->uart_port);
		}
		spin_unlock_irqrestore(&state->uart_port->lock, flags);
	}
}

/*
 * In 2.4.5, calls to this will be serialized via the BKL in
 *  linux/drivers/char/tty_io.c:tty_release()
 *  linux/drivers/char/tty_io.c:do_tty_handup()
=======
	if ((cflag ^ old_termios->c_cflag) == 0 &&
	    tty->termios.c_ospeed == old_termios->c_ospeed &&
	    tty->termios.c_ispeed == old_termios->c_ispeed &&
	    ((tty->termios.c_iflag ^ old_termios->c_iflag) & iflag_mask) == 0 &&
	    !sw_changed) {
		goto out;
	}

	uart_change_line_settings(tty, state, old_termios);
	/* reload cflag from termios; port driver may have overridden flags */
	cflag = tty->termios.c_cflag;

	/* Handle transition to B0 status */
	if (((old_termios->c_cflag & CBAUD) != B0) && ((cflag & CBAUD) == B0))
		uart_clear_mctrl(uport, TIOCM_RTS | TIOCM_DTR);
	/* Handle transition away from B0 status */
	else if (((old_termios->c_cflag & CBAUD) == B0) && ((cflag & CBAUD) != B0)) {
		unsigned int mask = TIOCM_DTR;

		if (!(cflag & CRTSCTS) || !tty_throttled(tty))
			mask |= TIOCM_RTS;
		uart_set_mctrl(uport, mask);
	}
out:
	mutex_unlock(&state->port.mutex);
}

/*
 * Calls to uart_close() are serialised via the tty_lock in
 *   drivers/tty/tty_io.c:tty_release()
 *   drivers/tty/tty_io.c:do_tty_hangup()
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
static void uart_close(struct tty_struct *tty, struct file *filp)
{
	struct uart_state *state = tty->driver_data;
<<<<<<< HEAD
	struct tty_port *port;
	struct uart_port *uport;
	unsigned long flags;

	if (!state)
		return;

	uport = state->uart_port;
	port = &state->port;

	pr_debug("uart_close(%d) called\n", uport->line);

	if (tty_port_close_start(port, tty, filp) == 0)
		return;
=======

	if (!state) {
		struct uart_driver *drv = tty->driver->driver_state;
		struct tty_port *port;

		state = drv->state + tty->index;
		port = &state->port;
		spin_lock_irq(&port->lock);
		--port->count;
		spin_unlock_irq(&port->lock);
		return;
	}

	pr_debug("uart_close(%d) called\n", tty->index);

	tty_port_close(tty->port, tty, filp);
}

static void uart_tty_port_shutdown(struct tty_port *port)
{
	struct uart_state *state = container_of(port, struct uart_state, port);
	struct uart_port *uport = uart_port_check(state);
	char *buf;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * At this point, we stop accepting input.  To do this, we
	 * disable the receive line status interrupts.
	 */
<<<<<<< HEAD
	if (port->flags & ASYNC_INITIALIZED) {
		unsigned long flags;
		spin_lock_irqsave(&uport->lock, flags);
		uport->ops->stop_rx(uport);
		spin_unlock_irqrestore(&uport->lock, flags);
		/*
		 * Before we drop DTR, make sure the UART transmitter
		 * has completely drained; this is especially
		 * important if there is a transmit FIFO!
		 */
		uart_wait_until_sent(tty, uport->timeout);
	}

	mutex_lock(&port->mutex);
	uart_shutdown(tty, state);
	uart_flush_buffer(tty);

	tty_ldisc_flush(tty);

	tty_port_tty_set(port, NULL);
	spin_lock_irqsave(&port->lock, flags);
	tty->closing = 0;

	if (port->blocked_open) {
		spin_unlock_irqrestore(&port->lock, flags);
		if (port->close_delay)
			msleep_interruptible(
					jiffies_to_msecs(port->close_delay));
		spin_lock_irqsave(&port->lock, flags);
	} else if (!uart_console(uport)) {
		spin_unlock_irqrestore(&port->lock, flags);
		uart_change_pm(state, 3);
		spin_lock_irqsave(&port->lock, flags);
	}

	/*
	 * Wake up anyone trying to open this port.
	 */
	clear_bit(ASYNCB_NORMAL_ACTIVE, &port->flags);
	clear_bit(ASYNCB_CLOSING, &port->flags);
	spin_unlock_irqrestore(&port->lock, flags);
	wake_up_interruptible(&port->open_wait);
	wake_up_interruptible(&port->close_wait);

	mutex_unlock(&port->mutex);
=======
	if (WARN(!uport, "detached port still initialized!\n"))
		return;

	uart_port_lock_irq(uport);
	uport->ops->stop_rx(uport);
	uart_port_unlock_irq(uport);

	serial_base_port_shutdown(uport);
	uart_port_shutdown(port);

	/*
	 * It's possible for shutdown to be called after suspend if we get
	 * a DCD drop (hangup) at just the right time.  Clear suspended bit so
	 * we don't try to resume a port that has been shutdown.
	 */
	tty_port_set_suspended(port, false);

	/*
	 * Free the transmit buffer.
	 */
	uart_port_lock_irq(uport);
	uart_circ_clear(&state->xmit);
	buf = state->xmit.buf;
	state->xmit.buf = NULL;
	uart_port_unlock_irq(uport);

	free_page((unsigned long)buf);

	uart_change_pm(state, UART_PM_STATE_OFF);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void uart_wait_until_sent(struct tty_struct *tty, int timeout)
{
	struct uart_state *state = tty->driver_data;
<<<<<<< HEAD
	struct uart_port *port = state->uart_port;
	unsigned long char_time, expire;

	if (port->type == PORT_UNKNOWN || port->fifosize == 0)
		return;

=======
	struct uart_port *port;
	unsigned long char_time, expire, fifo_timeout;

	port = uart_port_ref(state);
	if (!port)
		return;

	if (port->type == PORT_UNKNOWN || port->fifosize == 0) {
		uart_port_deref(port);
		return;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Set the check interval to be 1/5 of the estimated time to
	 * send a single character, and make it at least 1.  The check
	 * interval should also be less than the timeout.
	 *
	 * Note: we have to use pretty tight timings here to satisfy
	 * the NIST-PCTS.
	 */
<<<<<<< HEAD
	char_time = (port->timeout - HZ/50) / port->fifosize;
	char_time = char_time / 5;
	if (char_time == 0)
		char_time = 1;
	if (timeout && timeout < char_time)
		char_time = timeout;

	/*
	 * If the transmitter hasn't cleared in twice the approximate
	 * amount of time to send the entire FIFO, it probably won't
	 * ever clear.  This assumes the UART isn't doing flow
	 * control, which is currently the case.  Hence, if it ever
	 * takes longer than port->timeout, this is probably due to a
	 * UART bug of some kind.  So, we clamp the timeout parameter at
	 * 2*port->timeout.
	 */
	if (timeout == 0 || timeout > 2 * port->timeout)
		timeout = 2 * port->timeout;
=======
	char_time = max(nsecs_to_jiffies(port->frame_time / 5), 1UL);

	if (timeout && timeout < char_time)
		char_time = timeout;

	if (!uart_cts_enabled(port)) {
		/*
		 * If the transmitter hasn't cleared in twice the approximate
		 * amount of time to send the entire FIFO, it probably won't
		 * ever clear.  This assumes the UART isn't doing flow
		 * control, which is currently the case.  Hence, if it ever
		 * takes longer than FIFO timeout, this is probably due to a
		 * UART bug of some kind.  So, we clamp the timeout parameter at
		 * 2 * FIFO timeout.
		 */
		fifo_timeout = uart_fifo_timeout(port);
		if (timeout == 0 || timeout > 2 * fifo_timeout)
			timeout = 2 * fifo_timeout;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	expire = jiffies + timeout;

	pr_debug("uart_wait_until_sent(%d), jiffies=%lu, expire=%lu...\n",
		port->line, jiffies, expire);

	/*
	 * Check whether the transmitter is empty every 'char_time'.
	 * 'timeout' / 'expire' give us the maximum amount of time
	 * we wait.
	 */
	while (!port->ops->tx_empty(port)) {
		msleep_interruptible(jiffies_to_msecs(char_time));
		if (signal_pending(current))
			break;
<<<<<<< HEAD
		if (time_after(jiffies, expire))
			break;
	}
}

/*
 * This is called with the BKL held in
 *  linux/drivers/char/tty_io.c:do_tty_hangup()
 * We're called from the eventd thread, so we can sleep for
 * a _short_ time only.
=======
		if (timeout && time_after(jiffies, expire))
			break;
	}
	uart_port_deref(port);
}

/*
 * Calls to uart_hangup() are serialised by the tty_lock in
 *   drivers/tty/tty_io.c:do_tty_hangup()
 * This runs from a workqueue and can sleep for a _short_ time only.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
static void uart_hangup(struct tty_struct *tty)
{
	struct uart_state *state = tty->driver_data;
	struct tty_port *port = &state->port;
<<<<<<< HEAD
	unsigned long flags;

	pr_debug("uart_hangup(%d)\n", state->uart_port->line);

	mutex_lock(&port->mutex);
	if (port->flags & ASYNC_NORMAL_ACTIVE) {
=======
	struct uart_port *uport;
	unsigned long flags;

	pr_debug("uart_hangup(%d)\n", tty->index);

	mutex_lock(&port->mutex);
	uport = uart_port_check(state);
	WARN(!uport, "hangup of detached port!\n");

	if (tty_port_active(port)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		uart_flush_buffer(tty);
		uart_shutdown(tty, state);
		spin_lock_irqsave(&port->lock, flags);
		port->count = 0;
<<<<<<< HEAD
		clear_bit(ASYNCB_NORMAL_ACTIVE, &port->flags);
		spin_unlock_irqrestore(&port->lock, flags);
		tty_port_tty_set(port, NULL);
=======
		spin_unlock_irqrestore(&port->lock, flags);
		tty_port_set_active(port, false);
		tty_port_tty_set(port, NULL);
		if (uport && !uart_console(uport))
			uart_change_pm(state, UART_PM_STATE_OFF);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		wake_up_interruptible(&port->open_wait);
		wake_up_interruptible(&port->delta_msr_wait);
	}
	mutex_unlock(&port->mutex);
}

<<<<<<< HEAD
static int uart_port_activate(struct tty_port *port, struct tty_struct *tty)
{
	return 0;
}

static void uart_port_shutdown(struct tty_port *port)
{
	struct uart_state *state = container_of(port, struct uart_state, port);
	struct uart_port *uport = state->uart_port;
=======
/* uport == NULL if uart_port has already been removed */
static void uart_port_shutdown(struct tty_port *port)
{
	struct uart_state *state = container_of(port, struct uart_state, port);
	struct uart_port *uport = uart_port_check(state);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * clear delta_msr_wait queue to avoid mem leaks: we may free
	 * the irq here so the queue might never be woken up.  Note
	 * that we won't end up waiting on delta_msr_wait again since
	 * any outstanding file descriptors should be pointing at
	 * hung_up_tty_fops now.
	 */
	wake_up_interruptible(&port->delta_msr_wait);

<<<<<<< HEAD
	/*
	 * Free the IRQ and disable the port.
	 */
	uport->ops->shutdown(uport);

	/*
	 * Ensure that the IRQ handler isn't running on another CPU.
	 */
	synchronize_irq(uport->irq);
}

static int uart_carrier_raised(struct tty_port *port)
{
	struct uart_state *state = container_of(port, struct uart_state, port);
	struct uart_port *uport = state->uart_port;
	int mctrl;
	spin_lock_irq(&uport->lock);
	uport->ops->enable_ms(uport);
	mctrl = uport->ops->get_mctrl(uport);
	spin_unlock_irq(&uport->lock);
	if (mctrl & TIOCM_CAR)
		return 1;
	return 0;
}

static void uart_dtr_rts(struct tty_port *port, int onoff)
{
	struct uart_state *state = container_of(port, struct uart_state, port);
	struct uart_port *uport = state->uart_port;

	if (onoff)
		uart_set_mctrl(uport, TIOCM_DTR | TIOCM_RTS);
	else
		uart_clear_mctrl(uport, TIOCM_DTR | TIOCM_RTS);
}

/*
 * calls to uart_open are serialised by the BKL in
 *   fs/char_dev.c:chrdev_open()
=======
	if (uport) {
		/* Free the IRQ and disable the port. */
		uport->ops->shutdown(uport);

		/* Ensure that the IRQ handler isn't running on another CPU. */
		synchronize_irq(uport->irq);
	}
}

static bool uart_carrier_raised(struct tty_port *port)
{
	struct uart_state *state = container_of(port, struct uart_state, port);
	struct uart_port *uport;
	int mctrl;

	uport = uart_port_ref(state);
	/*
	 * Should never observe uport == NULL since checks for hangup should
	 * abort the tty_port_block_til_ready() loop before checking for carrier
	 * raised -- but report carrier raised if it does anyway so open will
	 * continue and not sleep
	 */
	if (WARN_ON(!uport))
		return true;
	uart_port_lock_irq(uport);
	uart_enable_ms(uport);
	mctrl = uport->ops->get_mctrl(uport);
	uart_port_unlock_irq(uport);
	uart_port_deref(uport);

	return mctrl & TIOCM_CAR;
}

static void uart_dtr_rts(struct tty_port *port, bool active)
{
	struct uart_state *state = container_of(port, struct uart_state, port);
	struct uart_port *uport;

	uport = uart_port_ref(state);
	if (!uport)
		return;
	uart_port_dtr_rts(uport, active);
	uart_port_deref(uport);
}

static int uart_install(struct tty_driver *driver, struct tty_struct *tty)
{
	struct uart_driver *drv = driver->driver_state;
	struct uart_state *state = drv->state + tty->index;

	tty->driver_data = state;

	return tty_standard_install(driver, tty);
}

/*
 * Calls to uart_open are serialised by the tty_lock in
 *   drivers/tty/tty_io.c:tty_open()
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Note that if this fails, then uart_close() _will_ be called.
 *
 * In time, we want to scrap the "opening nonpresent ports"
 * behaviour and implement an alternative way for setserial
 * to set base addresses/ports/types.  This will allow us to
 * get rid of a certain amount of extra tests.
 */
static int uart_open(struct tty_struct *tty, struct file *filp)
{
<<<<<<< HEAD
	struct uart_driver *drv = (struct uart_driver *)tty->driver->driver_state;
	int retval, line = tty->index;
	struct uart_state *state = drv->state + line;
	struct tty_port *port = &state->port;

	pr_debug("uart_open(%d) called\n", line);

	/*
	 * We take the semaphore here to guarantee that we won't be re-entered
	 * while allocating the state structure, or while we request any IRQs
	 * that the driver may need.  This also has the nice side-effect that
	 * it delays the action of uart_hangup, so we can guarantee that
	 * state->port.tty will always contain something reasonable.
	 */
	if (mutex_lock_interruptible(&port->mutex)) {
		retval = -ERESTARTSYS;
		goto end;
	}

	port->count++;
	if (!state->uart_port || state->uart_port->flags & UPF_DEAD) {
		retval = -ENXIO;
		goto err_dec_count;
	}

	/*
	 * Once we set tty->driver_data here, we are guaranteed that
	 * uart_close() will decrement the driver module use count.
	 * Any failures from here onwards should not touch the count.
	 */
	tty->driver_data = state;
	state->uart_port->state = state;
	tty->low_latency = (state->uart_port->flags & UPF_LOW_LATENCY) ? 1 : 0;
	tty_port_tty_set(port, tty);

	/*
	 * If the port is in the middle of closing, bail out now.
	 */
	if (tty_hung_up_p(filp)) {
		retval = -EAGAIN;
		goto err_dec_count;
	}

	/*
	 * Make sure the device is in D0 state.
	 */
	if (port->count == 1)
		uart_change_pm(state, 0);
=======
	struct uart_state *state = tty->driver_data;
	int retval;

	retval = tty_port_open(&state->port, tty, filp);
	if (retval > 0)
		retval = 0;

	return retval;
}

static int uart_port_activate(struct tty_port *port, struct tty_struct *tty)
{
	struct uart_state *state = container_of(port, struct uart_state, port);
	struct uart_port *uport;
	int ret;

	uport = uart_port_check(state);
	if (!uport || uport->flags & UPF_DEAD)
		return -ENXIO;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Start up the serial port.
	 */
<<<<<<< HEAD
	retval = uart_startup(tty, state, 0);

	/*
	 * If we succeeded, wait until the port is ready.
	 */
	mutex_unlock(&port->mutex);
	if (retval == 0)
		retval = tty_port_block_til_ready(port, tty, filp);

end:
	return retval;
err_dec_count:
	port->count--;
	mutex_unlock(&port->mutex);
	goto end;
=======
	ret = uart_startup(tty, state, false);
	if (ret > 0)
		tty_port_set_active(port, true);

	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const char *uart_type(struct uart_port *port)
{
	const char *str = NULL;

	if (port->ops->type)
		str = port->ops->type(port);

	if (!str)
		str = "unknown";

	return str;
}

#ifdef CONFIG_PROC_FS

static void uart_line_info(struct seq_file *m, struct uart_driver *drv, int i)
{
	struct uart_state *state = drv->state + i;
	struct tty_port *port = &state->port;
<<<<<<< HEAD
	int pm_state;
	struct uart_port *uport = state->uart_port;
=======
	enum uart_pm_state pm_state;
	struct uart_port *uport;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	char stat_buf[32];
	unsigned int status;
	int mmio;

<<<<<<< HEAD
	if (!uport)
		return;
=======
	mutex_lock(&port->mutex);
	uport = uart_port_check(state);
	if (!uport)
		goto out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	mmio = uport->iotype >= UPIO_MEM;
	seq_printf(m, "%d: uart:%s %s%08llX irq:%d",
			uport->line, uart_type(uport),
			mmio ? "mmio:0x" : "port:",
			mmio ? (unsigned long long)uport->mapbase
			     : (unsigned long long)uport->iobase,
			uport->irq);

	if (uport->type == PORT_UNKNOWN) {
		seq_putc(m, '\n');
<<<<<<< HEAD
		return;
	}

	if (capable(CAP_SYS_ADMIN)) {
		mutex_lock(&port->mutex);
		pm_state = state->pm_state;
		if (pm_state)
			uart_change_pm(state, 0);
		spin_lock_irq(&uport->lock);
		status = uport->ops->get_mctrl(uport);
		spin_unlock_irq(&uport->lock);
		if (pm_state)
			uart_change_pm(state, pm_state);
		mutex_unlock(&port->mutex);
=======
		goto out;
	}

	if (capable(CAP_SYS_ADMIN)) {
		pm_state = state->pm_state;
		if (pm_state != UART_PM_STATE_ON)
			uart_change_pm(state, UART_PM_STATE_ON);
		uart_port_lock_irq(uport);
		status = uport->ops->get_mctrl(uport);
		uart_port_unlock_irq(uport);
		if (pm_state != UART_PM_STATE_ON)
			uart_change_pm(state, pm_state);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		seq_printf(m, " tx:%d rx:%d",
				uport->icount.tx, uport->icount.rx);
		if (uport->icount.frame)
<<<<<<< HEAD
			seq_printf(m, " fe:%d",
				uport->icount.frame);
		if (uport->icount.parity)
			seq_printf(m, " pe:%d",
				uport->icount.parity);
		if (uport->icount.brk)
			seq_printf(m, " brk:%d",
				uport->icount.brk);
		if (uport->icount.overrun)
			seq_printf(m, " oe:%d",
				uport->icount.overrun);
=======
			seq_printf(m, " fe:%d",	uport->icount.frame);
		if (uport->icount.parity)
			seq_printf(m, " pe:%d",	uport->icount.parity);
		if (uport->icount.brk)
			seq_printf(m, " brk:%d", uport->icount.brk);
		if (uport->icount.overrun)
			seq_printf(m, " oe:%d", uport->icount.overrun);
		if (uport->icount.buf_overrun)
			seq_printf(m, " bo:%d", uport->icount.buf_overrun);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define INFOBIT(bit, str) \
	if (uport->mctrl & (bit)) \
		strncat(stat_buf, (str), sizeof(stat_buf) - \
			strlen(stat_buf) - 2)
#define STATBIT(bit, str) \
	if (status & (bit)) \
		strncat(stat_buf, (str), sizeof(stat_buf) - \
		       strlen(stat_buf) - 2)

		stat_buf[0] = '\0';
		stat_buf[1] = '\0';
		INFOBIT(TIOCM_RTS, "|RTS");
		STATBIT(TIOCM_CTS, "|CTS");
		INFOBIT(TIOCM_DTR, "|DTR");
		STATBIT(TIOCM_DSR, "|DSR");
		STATBIT(TIOCM_CAR, "|CD");
		STATBIT(TIOCM_RNG, "|RI");
		if (stat_buf[0])
			stat_buf[0] = ' ';

		seq_puts(m, stat_buf);
	}
	seq_putc(m, '\n');
#undef STATBIT
#undef INFOBIT
<<<<<<< HEAD
=======
out:
	mutex_unlock(&port->mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int uart_proc_show(struct seq_file *m, void *v)
{
	struct tty_driver *ttydrv = m->private;
	struct uart_driver *drv = ttydrv->driver_state;
	int i;

<<<<<<< HEAD
	seq_printf(m, "serinfo:1.0 driver%s%s revision:%s\n",
			"", "", "");
=======
	seq_printf(m, "serinfo:1.0 driver%s%s revision:%s\n", "", "", "");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	for (i = 0; i < drv->nr; i++)
		uart_line_info(m, drv, i);
	return 0;
}
<<<<<<< HEAD

static int uart_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, uart_proc_show, PDE(inode)->data);
}

static const struct file_operations uart_proc_fops = {
	.owner		= THIS_MODULE,
	.open		= uart_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};
#endif

#if defined(CONFIG_SERIAL_CORE_CONSOLE) || defined(CONFIG_CONSOLE_POLL)
/*
 *	uart_console_write - write a console message to a serial port
 *	@port: the port to write the message
 *	@s: array of characters
 *	@count: number of characters in string to write
 *	@write: function to write character to port
 */
void uart_console_write(struct uart_port *port, const char *s,
			unsigned int count,
			void (*putchar)(struct uart_port *, int))
=======
#endif

static void uart_port_spin_lock_init(struct uart_port *port)
{
	spin_lock_init(&port->lock);
	lockdep_set_class(&port->lock, &port_lock_key);
}

#if defined(CONFIG_SERIAL_CORE_CONSOLE) || defined(CONFIG_CONSOLE_POLL)
/**
 * uart_console_write - write a console message to a serial port
 * @port: the port to write the message
 * @s: array of characters
 * @count: number of characters in string to write
 * @putchar: function to write character to port
 */
void uart_console_write(struct uart_port *port, const char *s,
			unsigned int count,
			void (*putchar)(struct uart_port *, unsigned char))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned int i;

	for (i = 0; i < count; i++, s++) {
		if (*s == '\n')
			putchar(port, '\r');
		putchar(port, *s);
	}
}
EXPORT_SYMBOL_GPL(uart_console_write);

<<<<<<< HEAD
/*
 *	Check whether an invalid uart number has been specified, and
 *	if so, search for the first available port that does have
 *	console support.
=======
/**
 * uart_get_console - get uart port for console
 * @ports: ports to search in
 * @nr: number of @ports
 * @co: console to search for
 * Returns: uart_port for the console @co
 *
 * Check whether an invalid uart number has been specified (as @co->index), and
 * if so, search for the first available port that does have console support.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
struct uart_port * __init
uart_get_console(struct uart_port *ports, int nr, struct console *co)
{
	int idx = co->index;

	if (idx < 0 || idx >= nr || (ports[idx].iobase == 0 &&
				     ports[idx].membase == NULL))
		for (idx = 0; idx < nr; idx++)
			if (ports[idx].iobase != 0 ||
			    ports[idx].membase != NULL)
				break;

	co->index = idx;

	return ports + idx;
}

/**
<<<<<<< HEAD
 *	uart_parse_options - Parse serial port baud/parity/bits/flow contro.
 *	@options: pointer to option string
 *	@baud: pointer to an 'int' variable for the baud rate.
 *	@parity: pointer to an 'int' variable for the parity.
 *	@bits: pointer to an 'int' variable for the number of data bits.
 *	@flow: pointer to an 'int' variable for the flow control character.
 *
 *	uart_parse_options decodes a string containing the serial console
 *	options.  The format of the string is <baud><parity><bits><flow>,
 *	eg: 115200n8r
 */
void
uart_parse_options(char *options, int *baud, int *parity, int *bits, int *flow)
{
	char *s = options;
=======
 * uart_parse_earlycon - Parse earlycon options
 * @p:	     ptr to 2nd field (ie., just beyond '<name>,')
 * @iotype:  ptr for decoded iotype (out)
 * @addr:    ptr for decoded mapbase/iobase (out)
 * @options: ptr for <options> field; %NULL if not present (out)
 *
 * Decodes earlycon kernel command line parameters of the form:
 *  * earlycon=<name>,io|mmio|mmio16|mmio32|mmio32be|mmio32native,<addr>,<options>
 *  * console=<name>,io|mmio|mmio16|mmio32|mmio32be|mmio32native,<addr>,<options>
 *
 * The optional form:
 *  * earlycon=<name>,0x<addr>,<options>
 *  * console=<name>,0x<addr>,<options>
 *
 * is also accepted; the returned @iotype will be %UPIO_MEM.
 *
 * Returns: 0 on success or -%EINVAL on failure
 */
int uart_parse_earlycon(char *p, unsigned char *iotype, resource_size_t *addr,
			char **options)
{
	if (strncmp(p, "mmio,", 5) == 0) {
		*iotype = UPIO_MEM;
		p += 5;
	} else if (strncmp(p, "mmio16,", 7) == 0) {
		*iotype = UPIO_MEM16;
		p += 7;
	} else if (strncmp(p, "mmio32,", 7) == 0) {
		*iotype = UPIO_MEM32;
		p += 7;
	} else if (strncmp(p, "mmio32be,", 9) == 0) {
		*iotype = UPIO_MEM32BE;
		p += 9;
	} else if (strncmp(p, "mmio32native,", 13) == 0) {
		*iotype = IS_ENABLED(CONFIG_CPU_BIG_ENDIAN) ?
			UPIO_MEM32BE : UPIO_MEM32;
		p += 13;
	} else if (strncmp(p, "io,", 3) == 0) {
		*iotype = UPIO_PORT;
		p += 3;
	} else if (strncmp(p, "0x", 2) == 0) {
		*iotype = UPIO_MEM;
	} else {
		return -EINVAL;
	}

	/*
	 * Before you replace it with kstrtoull(), think about options separator
	 * (',') it will not tolerate
	 */
	*addr = simple_strtoull(p, NULL, 0);
	p = strchr(p, ',');
	if (p)
		p++;

	*options = p;
	return 0;
}
EXPORT_SYMBOL_GPL(uart_parse_earlycon);

/**
 * uart_parse_options - Parse serial port baud/parity/bits/flow control.
 * @options: pointer to option string
 * @baud: pointer to an 'int' variable for the baud rate.
 * @parity: pointer to an 'int' variable for the parity.
 * @bits: pointer to an 'int' variable for the number of data bits.
 * @flow: pointer to an 'int' variable for the flow control character.
 *
 * uart_parse_options() decodes a string containing the serial console
 * options. The format of the string is <baud><parity><bits><flow>,
 * eg: 115200n8r
 */
void
uart_parse_options(const char *options, int *baud, int *parity,
		   int *bits, int *flow)
{
	const char *s = options;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	*baud = simple_strtoul(s, NULL, 10);
	while (*s >= '0' && *s <= '9')
		s++;
	if (*s)
		*parity = *s++;
	if (*s)
		*bits = *s++ - '0';
	if (*s)
		*flow = *s;
}
EXPORT_SYMBOL_GPL(uart_parse_options);

<<<<<<< HEAD
struct baud_rates {
	unsigned int rate;
	unsigned int cflag;
};

static const struct baud_rates baud_rates[] = {
	{ 921600, B921600 },
	{ 460800, B460800 },
	{ 230400, B230400 },
	{ 115200, B115200 },
	{  57600, B57600  },
	{  38400, B38400  },
	{  19200, B19200  },
	{   9600, B9600   },
	{   4800, B4800   },
	{   2400, B2400   },
	{   1200, B1200   },
	{      0, B38400  }
};

/**
 *	uart_set_options - setup the serial console parameters
 *	@port: pointer to the serial ports uart_port structure
 *	@co: console pointer
 *	@baud: baud rate
 *	@parity: parity character - 'n' (none), 'o' (odd), 'e' (even)
 *	@bits: number of data bits
 *	@flow: flow control character - 'r' (rts)
=======
/**
 * uart_set_options - setup the serial console parameters
 * @port: pointer to the serial ports uart_port structure
 * @co: console pointer
 * @baud: baud rate
 * @parity: parity character - 'n' (none), 'o' (odd), 'e' (even)
 * @bits: number of data bits
 * @flow: flow control character - 'r' (rts)
 *
 * Locking: Caller must hold console_list_lock in order to serialize
 * early initialization of the serial-console lock.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
int
uart_set_options(struct uart_port *port, struct console *co,
		 int baud, int parity, int bits, int flow)
{
	struct ktermios termios;
	static struct ktermios dummy;
<<<<<<< HEAD
	int i;

	/*
	 * Ensure that the serial console lock is initialised
	 * early.
	 */
	spin_lock_init(&port->lock);
	lockdep_set_class(&port->lock, &port_lock_key);

	memset(&termios, 0, sizeof(struct ktermios));

	termios.c_cflag = CREAD | HUPCL | CLOCAL;

	/*
	 * Construct a cflag setting.
	 */
	for (i = 0; baud_rates[i].rate; i++)
		if (baud_rates[i].rate <= baud)
			break;

	termios.c_cflag |= baud_rates[i].cflag;
=======

	/*
	 * Ensure that the serial-console lock is initialised early.
	 *
	 * Note that the console-registered check is needed because
	 * kgdboc can call uart_set_options() for an already registered
	 * console via tty_find_polling_driver() and uart_poll_init().
	 */
	if (!uart_console_registered_locked(port) && !port->console_reinit)
		uart_port_spin_lock_init(port);

	memset(&termios, 0, sizeof(struct ktermios));

	termios.c_cflag |= CREAD | HUPCL | CLOCAL;
	tty_termios_encode_baud_rate(&termios, baud, baud);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (bits == 7)
		termios.c_cflag |= CS7;
	else
		termios.c_cflag |= CS8;

	switch (parity) {
	case 'o': case 'O':
		termios.c_cflag |= PARODD;
<<<<<<< HEAD
		/*fall through*/
=======
		fallthrough;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case 'e': case 'E':
		termios.c_cflag |= PARENB;
		break;
	}

	if (flow == 'r')
		termios.c_cflag |= CRTSCTS;

	/*
	 * some uarts on other side don't support no flow control.
	 * So we set * DTR in host uart to make them happy
	 */
	port->mctrl |= TIOCM_DTR;

	port->ops->set_termios(port, &termios, &dummy);
	/*
	 * Allow the setting of the UART parameters with a NULL console
	 * too:
	 */
<<<<<<< HEAD
	if (co)
		co->cflag = termios.c_cflag;
=======
	if (co) {
		co->cflag = termios.c_cflag;
		co->ispeed = termios.c_ispeed;
		co->ospeed = termios.c_ospeed;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}
EXPORT_SYMBOL_GPL(uart_set_options);
#endif /* CONFIG_SERIAL_CORE_CONSOLE */

/**
 * uart_change_pm - set power state of the port
 *
 * @state: port descriptor
 * @pm_state: new state
 *
 * Locking: port->mutex has to be held
 */
<<<<<<< HEAD
static void uart_change_pm(struct uart_state *state, int pm_state)
{
	struct uart_port *port = state->uart_port;

	if (state->pm_state != pm_state) {
		if (port->ops->pm)
=======
static void uart_change_pm(struct uart_state *state,
			   enum uart_pm_state pm_state)
{
	struct uart_port *port = uart_port_check(state);

	if (state->pm_state != pm_state) {
		if (port && port->ops->pm)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			port->ops->pm(port, pm_state, state->pm_state);
		state->pm_state = pm_state;
	}
}

struct uart_match {
	struct uart_port *port;
	struct uart_driver *driver;
};

static int serial_match_port(struct device *dev, void *data)
{
	struct uart_match *match = data;
	struct tty_driver *tty_drv = match->driver->tty_driver;
	dev_t devt = MKDEV(tty_drv->major, tty_drv->minor_start) +
		match->port->line;

	return dev->devt == devt; /* Actually, only one tty per port */
}

int uart_suspend_port(struct uart_driver *drv, struct uart_port *uport)
{
	struct uart_state *state = drv->state + uport->line;
	struct tty_port *port = &state->port;
	struct device *tty_dev;
	struct uart_match match = {uport, drv};

	mutex_lock(&port->mutex);

<<<<<<< HEAD
	tty_dev = device_find_child(uport->dev, &match, serial_match_port);
	if (device_may_wakeup(tty_dev)) {
		if (!enable_irq_wake(uport->irq))
			uport->irq_wake = 1;
=======
	tty_dev = device_find_child(&uport->port_dev->dev, &match, serial_match_port);
	if (tty_dev && device_may_wakeup(tty_dev)) {
		enable_irq_wake(uport->irq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		put_device(tty_dev);
		mutex_unlock(&port->mutex);
		return 0;
	}
	put_device(tty_dev);

<<<<<<< HEAD
	if (console_suspend_enabled || !uart_console(uport))
		uport->suspended = 1;

	if (port->flags & ASYNC_INITIALIZED) {
		const struct uart_ops *ops = uport->ops;
		int tries;

		if (console_suspend_enabled || !uart_console(uport)) {
			set_bit(ASYNCB_SUSPENDED, &port->flags);
			clear_bit(ASYNCB_INITIALIZED, &port->flags);

			spin_lock_irq(&uport->lock);
			ops->stop_tx(uport);
			ops->set_mctrl(uport, 0);
			ops->stop_rx(uport);
			spin_unlock_irq(&uport->lock);
		}
=======
	/*
	 * Nothing to do if the console is not suspending
	 * except stop_rx to prevent any asynchronous data
	 * over RX line. However ensure that we will be
	 * able to Re-start_rx later.
	 */
	if (!console_suspend_enabled && uart_console(uport)) {
		if (uport->ops->start_rx) {
			uart_port_lock_irq(uport);
			uport->ops->stop_rx(uport);
			uart_port_unlock_irq(uport);
		}
		goto unlock;
	}

	uport->suspended = 1;

	if (tty_port_initialized(port)) {
		const struct uart_ops *ops = uport->ops;
		int tries;
		unsigned int mctrl;

		tty_port_set_suspended(port, true);
		tty_port_set_initialized(port, false);

		uart_port_lock_irq(uport);
		ops->stop_tx(uport);
		if (!(uport->rs485.flags & SER_RS485_ENABLED))
			ops->set_mctrl(uport, 0);
		/* save mctrl so it can be restored on resume */
		mctrl = uport->mctrl;
		uport->mctrl = 0;
		ops->stop_rx(uport);
		uart_port_unlock_irq(uport);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/*
		 * Wait for the transmitter to empty.
		 */
		for (tries = 3; !ops->tx_empty(uport) && tries; tries--)
			msleep(10);
		if (!tries)
<<<<<<< HEAD
			printk(KERN_ERR "%s%s%s%d: Unable to drain "
					"transmitter\n",
			       uport->dev ? dev_name(uport->dev) : "",
			       uport->dev ? ": " : "",
			       drv->dev_name,
			       drv->tty_driver->name_base + uport->line);

		if (console_suspend_enabled || !uart_console(uport))
			ops->shutdown(uport);
=======
			dev_err(uport->dev, "%s: Unable to drain transmitter\n",
				uport->name);

		ops->shutdown(uport);
		uport->mctrl = mctrl;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/*
	 * Disable the console device before suspending.
	 */
<<<<<<< HEAD
	if (console_suspend_enabled && uart_console(uport))
		console_stop(uport->cons);

	if (console_suspend_enabled || !uart_console(uport))
		uart_change_pm(state, 3);

=======
	if (uart_console(uport))
		console_stop(uport->cons);

	uart_change_pm(state, UART_PM_STATE_OFF);
unlock:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mutex_unlock(&port->mutex);

	return 0;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL(uart_suspend_port);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

int uart_resume_port(struct uart_driver *drv, struct uart_port *uport)
{
	struct uart_state *state = drv->state + uport->line;
	struct tty_port *port = &state->port;
	struct device *tty_dev;
	struct uart_match match = {uport, drv};
	struct ktermios termios;

	mutex_lock(&port->mutex);

<<<<<<< HEAD
	tty_dev = device_find_child(uport->dev, &match, serial_match_port);
	if (!uport->suspended && device_may_wakeup(tty_dev)) {
		if (uport->irq_wake) {
			disable_irq_wake(uport->irq);
			uport->irq_wake = 0;
		}
=======
	tty_dev = device_find_child(&uport->port_dev->dev, &match, serial_match_port);
	if (!uport->suspended && device_may_wakeup(tty_dev)) {
		if (irqd_is_wakeup_set(irq_get_irq_data((uport->irq))))
			disable_irq_wake(uport->irq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		put_device(tty_dev);
		mutex_unlock(&port->mutex);
		return 0;
	}
	put_device(tty_dev);
	uport->suspended = 0;

	/*
	 * Re-enable the console device after suspending.
	 */
	if (uart_console(uport)) {
		/*
		 * First try to use the console cflag setting.
		 */
		memset(&termios, 0, sizeof(struct ktermios));
		termios.c_cflag = uport->cons->cflag;
<<<<<<< HEAD
=======
		termios.c_ispeed = uport->cons->ispeed;
		termios.c_ospeed = uport->cons->ospeed;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/*
		 * If that's unset, use the tty termios setting.
		 */
<<<<<<< HEAD
		if (port->tty && port->tty->termios && termios.c_cflag == 0)
			termios = *(port->tty->termios);
		/*
		 * As we need to set the uart clock rate back to 7.3 MHz.
		 * We need this change.
		 *
		 */
		if (console_suspend_enabled)
			uart_change_pm(state, 0);
		uport->ops->set_termios(uport, &termios, NULL);
=======
		if (port->tty && termios.c_cflag == 0)
			termios = port->tty->termios;

		if (console_suspend_enabled)
			uart_change_pm(state, UART_PM_STATE_ON);
		uport->ops->set_termios(uport, &termios, NULL);
		if (!console_suspend_enabled && uport->ops->start_rx) {
			uart_port_lock_irq(uport);
			uport->ops->start_rx(uport);
			uart_port_unlock_irq(uport);
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (console_suspend_enabled)
			console_start(uport->cons);
	}

<<<<<<< HEAD
	if (port->flags & ASYNC_SUSPENDED) {
		const struct uart_ops *ops = uport->ops;
		int ret;

		uart_change_pm(state, 0);
		spin_lock_irq(&uport->lock);
		ops->set_mctrl(uport, 0);
		spin_unlock_irq(&uport->lock);
		if (console_suspend_enabled || !uart_console(uport)) {
			/* Protected by port mutex for now */
			struct tty_struct *tty = port->tty;
			ret = ops->startup(uport);
			if (ret == 0) {
				if (tty)
					uart_change_speed(tty, state, NULL);
				spin_lock_irq(&uport->lock);
				ops->set_mctrl(uport, uport->mctrl);
				ops->start_tx(uport);
				spin_unlock_irq(&uport->lock);
				set_bit(ASYNCB_INITIALIZED, &port->flags);
=======
	if (tty_port_suspended(port)) {
		const struct uart_ops *ops = uport->ops;
		int ret;

		uart_change_pm(state, UART_PM_STATE_ON);
		uart_port_lock_irq(uport);
		if (!(uport->rs485.flags & SER_RS485_ENABLED))
			ops->set_mctrl(uport, 0);
		uart_port_unlock_irq(uport);
		if (console_suspend_enabled || !uart_console(uport)) {
			/* Protected by port mutex for now */
			struct tty_struct *tty = port->tty;

			ret = ops->startup(uport);
			if (ret == 0) {
				if (tty)
					uart_change_line_settings(tty, state, NULL);
				uart_rs485_config(uport);
				uart_port_lock_irq(uport);
				if (!(uport->rs485.flags & SER_RS485_ENABLED))
					ops->set_mctrl(uport, uport->mctrl);
				ops->start_tx(uport);
				uart_port_unlock_irq(uport);
				tty_port_set_initialized(port, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			} else {
				/*
				 * Failed to resume - maybe hardware went away?
				 * Clear the "initialized" flag so we won't try
				 * to call the low level drivers shutdown method.
				 */
				uart_shutdown(tty, state);
			}
		}

<<<<<<< HEAD
		clear_bit(ASYNCB_SUSPENDED, &port->flags);
=======
		tty_port_set_suspended(port, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	mutex_unlock(&port->mutex);

	return 0;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL(uart_resume_port);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline void
uart_report_port(struct uart_driver *drv, struct uart_port *port)
{
	char address[64];

	switch (port->iotype) {
	case UPIO_PORT:
		snprintf(address, sizeof(address), "I/O 0x%lx", port->iobase);
		break;
	case UPIO_HUB6:
		snprintf(address, sizeof(address),
			 "I/O 0x%lx offset 0x%x", port->iobase, port->hub6);
		break;
	case UPIO_MEM:
<<<<<<< HEAD
	case UPIO_MEM32:
=======
	case UPIO_MEM16:
	case UPIO_MEM32:
	case UPIO_MEM32BE:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case UPIO_AU:
	case UPIO_TSI:
		snprintf(address, sizeof(address),
			 "MMIO 0x%llx", (unsigned long long)port->mapbase);
		break;
	default:
<<<<<<< HEAD
		strlcpy(address, "*unknown*", sizeof(address));
		break;
	}

	printk(KERN_INFO "%s%s%s%d at %s (irq = %d) is a %s\n",
	       port->dev ? dev_name(port->dev) : "",
	       port->dev ? ": " : "",
	       drv->dev_name,
	       drv->tty_driver->name_base + port->line,
	       address, port->irq, uart_type(port));
=======
		strscpy(address, "*unknown*", sizeof(address));
		break;
	}

	pr_info("%s%s%s at %s (irq = %d, base_baud = %d) is a %s\n",
	       port->dev ? dev_name(port->dev) : "",
	       port->dev ? ": " : "",
	       port->name,
	       address, port->irq, port->uartclk / 16, uart_type(port));

	/* The magic multiplier feature is a bit obscure, so report it too.  */
	if (port->flags & UPF_MAGIC_MULTIPLIER)
		pr_info("%s%s%s extra baud rates supported: %d, %d",
			port->dev ? dev_name(port->dev) : "",
			port->dev ? ": " : "",
			port->name,
			port->uartclk / 8, port->uartclk / 4);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void
uart_configure_port(struct uart_driver *drv, struct uart_state *state,
		    struct uart_port *port)
{
	unsigned int flags;

	/*
	 * If there isn't a port here, don't do anything further.
	 */
	if (!port->iobase && !port->mapbase && !port->membase)
		return;

	/*
	 * Now do the auto configuration stuff.  Note that config_port
	 * is expected to claim the resources and map the port for us.
	 */
	flags = 0;
	if (port->flags & UPF_AUTO_IRQ)
		flags |= UART_CONFIG_IRQ;
	if (port->flags & UPF_BOOT_AUTOCONF) {
		if (!(port->flags & UPF_FIXED_TYPE)) {
			port->type = PORT_UNKNOWN;
			flags |= UART_CONFIG_TYPE;
		}
<<<<<<< HEAD
		port->ops->config_port(port, flags);
=======
		/* Synchronize with possible boot console. */
		if (uart_console(port))
			console_lock();
		port->ops->config_port(port, flags);
		if (uart_console(port))
			console_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (port->type != PORT_UNKNOWN) {
		unsigned long flags;

		uart_report_port(drv, port);

<<<<<<< HEAD
		/* Power up port for set_mctrl() */
		uart_change_pm(state, 0);
=======
		/* Synchronize with possible boot console. */
		if (uart_console(port))
			console_lock();

		/* Power up port for set_mctrl() */
		uart_change_pm(state, UART_PM_STATE_ON);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/*
		 * Ensure that the modem control lines are de-activated.
		 * keep the DTR setting that is set in uart_set_options()
		 * We probably don't need a spinlock around this, but
		 */
<<<<<<< HEAD
		spin_lock_irqsave(&port->lock, flags);
		port->ops->set_mctrl(port, port->mctrl & TIOCM_DTR);
		spin_unlock_irqrestore(&port->lock, flags);
=======
		uart_port_lock_irqsave(port, &flags);
		port->mctrl &= TIOCM_DTR;
		if (!(port->rs485.flags & SER_RS485_ENABLED))
			port->ops->set_mctrl(port, port->mctrl);
		uart_port_unlock_irqrestore(port, flags);

		uart_rs485_config(port);

		if (uart_console(port))
			console_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/*
		 * If this driver supports console, and it hasn't been
		 * successfully registered yet, try to re-register it.
		 * It may be that the port was not available.
		 */
<<<<<<< HEAD
		if (port->cons && !(port->cons->flags & CON_ENABLED))
=======
		if (port->cons && !console_is_registered(port->cons))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			register_console(port->cons);

		/*
		 * Power down all ports by default, except the
		 * console if we have one.
		 */
		if (!uart_console(port))
<<<<<<< HEAD
			uart_change_pm(state, 3);
=======
			uart_change_pm(state, UART_PM_STATE_OFF);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

#ifdef CONFIG_CONSOLE_POLL

static int uart_poll_init(struct tty_driver *driver, int line, char *options)
{
	struct uart_driver *drv = driver->driver_state;
	struct uart_state *state = drv->state + line;
<<<<<<< HEAD
=======
	enum uart_pm_state pm_state;
	struct tty_port *tport;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct uart_port *port;
	int baud = 9600;
	int bits = 8;
	int parity = 'n';
	int flow = 'n';
<<<<<<< HEAD

	if (!state || !state->uart_port)
		return -1;

	port = state->uart_port;
	if (!(port->ops->poll_get_char && port->ops->poll_put_char))
		return -1;

	if (options) {
		uart_parse_options(options, &baud, &parity, &bits, &flow);
		return uart_set_options(port, NULL, baud, parity, bits, flow);
	}

	return 0;
=======
	int ret = 0;

	tport = &state->port;
	mutex_lock(&tport->mutex);

	port = uart_port_check(state);
	if (!port || port->type == PORT_UNKNOWN ||
	    !(port->ops->poll_get_char && port->ops->poll_put_char)) {
		ret = -1;
		goto out;
	}

	pm_state = state->pm_state;
	uart_change_pm(state, UART_PM_STATE_ON);

	if (port->ops->poll_init) {
		/*
		 * We don't set initialized as we only initialized the hw,
		 * e.g. state->xmit is still uninitialized.
		 */
		if (!tty_port_initialized(tport))
			ret = port->ops->poll_init(port);
	}

	if (!ret && options) {
		uart_parse_options(options, &baud, &parity, &bits, &flow);
		console_list_lock();
		ret = uart_set_options(port, NULL, baud, parity, bits, flow);
		console_list_unlock();
	}
out:
	if (ret)
		uart_change_pm(state, pm_state);
	mutex_unlock(&tport->mutex);
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int uart_poll_get_char(struct tty_driver *driver, int line)
{
	struct uart_driver *drv = driver->driver_state;
	struct uart_state *state = drv->state + line;
	struct uart_port *port;
<<<<<<< HEAD

	if (!state || !state->uart_port)
		return -1;

	port = state->uart_port;
	return port->ops->poll_get_char(port);
=======
	int ret = -1;

	port = uart_port_ref(state);
	if (port) {
		ret = port->ops->poll_get_char(port);
		uart_port_deref(port);
	}

	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void uart_poll_put_char(struct tty_driver *driver, int line, char ch)
{
	struct uart_driver *drv = driver->driver_state;
	struct uart_state *state = drv->state + line;
	struct uart_port *port;

<<<<<<< HEAD
	if (!state || !state->uart_port)
		return;

	port = state->uart_port;
	port->ops->poll_put_char(port, ch);
=======
	port = uart_port_ref(state);
	if (!port)
		return;

	if (ch == '\n')
		port->ops->poll_put_char(port, '\r');
	port->ops->poll_put_char(port, ch);
	uart_port_deref(port);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
#endif

static const struct tty_operations uart_ops = {
<<<<<<< HEAD
=======
	.install	= uart_install,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.open		= uart_open,
	.close		= uart_close,
	.write		= uart_write,
	.put_char	= uart_put_char,
	.flush_chars	= uart_flush_chars,
	.write_room	= uart_write_room,
	.chars_in_buffer= uart_chars_in_buffer,
	.flush_buffer	= uart_flush_buffer,
	.ioctl		= uart_ioctl,
	.throttle	= uart_throttle,
	.unthrottle	= uart_unthrottle,
	.send_xchar	= uart_send_xchar,
	.set_termios	= uart_set_termios,
	.set_ldisc	= uart_set_ldisc,
	.stop		= uart_stop,
	.start		= uart_start,
	.hangup		= uart_hangup,
	.break_ctl	= uart_break_ctl,
	.wait_until_sent= uart_wait_until_sent,
#ifdef CONFIG_PROC_FS
<<<<<<< HEAD
	.proc_fops	= &uart_proc_fops,
#endif
	.tiocmget	= uart_tiocmget,
	.tiocmset	= uart_tiocmset,
=======
	.proc_show	= uart_proc_show,
#endif
	.tiocmget	= uart_tiocmget,
	.tiocmset	= uart_tiocmset,
	.set_serial	= uart_set_info_user,
	.get_serial	= uart_get_info_user,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.get_icount	= uart_get_icount,
#ifdef CONFIG_CONSOLE_POLL
	.poll_init	= uart_poll_init,
	.poll_get_char	= uart_poll_get_char,
	.poll_put_char	= uart_poll_put_char,
#endif
};

static const struct tty_port_operations uart_port_ops = {
<<<<<<< HEAD
	.activate	= uart_port_activate,
	.shutdown	= uart_port_shutdown,
	.carrier_raised = uart_carrier_raised,
	.dtr_rts	= uart_dtr_rts,
};

/**
 *	uart_register_driver - register a driver with the uart core layer
 *	@drv: low level driver structure
 *
 *	Register a uart driver with the core driver.  We in turn register
 *	with the tty layer, and initialise the core driver per-port state.
 *
 *	We have a proc file in /proc/tty/driver which is named after the
 *	normal driver.
 *
 *	drv->port should be NULL, and the per-port structures should be
 *	registered using uart_add_one_port after this call has succeeded.
=======
	.carrier_raised = uart_carrier_raised,
	.dtr_rts	= uart_dtr_rts,
	.activate	= uart_port_activate,
	.shutdown	= uart_tty_port_shutdown,
};

/**
 * uart_register_driver - register a driver with the uart core layer
 * @drv: low level driver structure
 *
 * Register a uart driver with the core driver. We in turn register with the
 * tty layer, and initialise the core driver per-port state.
 *
 * We have a proc file in /proc/tty/driver which is named after the normal
 * driver.
 *
 * @drv->port should be %NULL, and the per-port structures should be registered
 * using uart_add_one_port() after this call has succeeded.
 *
 * Locking: none, Interrupts: enabled
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
int uart_register_driver(struct uart_driver *drv)
{
	struct tty_driver *normal;
<<<<<<< HEAD
	int i, retval;
=======
	int i, retval = -ENOMEM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	BUG_ON(drv->state);

	/*
	 * Maybe we should be using a slab cache for this, especially if
	 * we have a large number of ports to handle.
	 */
<<<<<<< HEAD
	drv->state = kzalloc(sizeof(struct uart_state) * drv->nr, GFP_KERNEL);
	if (!drv->state)
		goto out;

	normal = alloc_tty_driver(drv->nr);
	if (!normal)
		goto out_kfree;
=======
	drv->state = kcalloc(drv->nr, sizeof(struct uart_state), GFP_KERNEL);
	if (!drv->state)
		goto out;

	normal = tty_alloc_driver(drv->nr, TTY_DRIVER_REAL_RAW |
			TTY_DRIVER_DYNAMIC_DEV);
	if (IS_ERR(normal)) {
		retval = PTR_ERR(normal);
		goto out_kfree;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	drv->tty_driver = normal;

	normal->driver_name	= drv->driver_name;
	normal->name		= drv->dev_name;
	normal->major		= drv->major;
	normal->minor_start	= drv->minor;
	normal->type		= TTY_DRIVER_TYPE_SERIAL;
	normal->subtype		= SERIAL_TYPE_NORMAL;
	normal->init_termios	= tty_std_termios;
	normal->init_termios.c_cflag = B9600 | CS8 | CREAD | HUPCL | CLOCAL;
	normal->init_termios.c_ispeed = normal->init_termios.c_ospeed = 9600;
<<<<<<< HEAD
	normal->flags		= TTY_DRIVER_REAL_RAW | TTY_DRIVER_DYNAMIC_DEV;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	normal->driver_state    = drv;
	tty_set_operations(normal, &uart_ops);

	/*
	 * Initialise the UART state(s).
	 */
	for (i = 0; i < drv->nr; i++) {
		struct uart_state *state = drv->state + i;
		struct tty_port *port = &state->port;

		tty_port_init(port);
		port->ops = &uart_port_ops;
<<<<<<< HEAD
		port->close_delay     = HZ / 2;	/* .5 seconds */
		port->closing_wait    = 30 * HZ;/* 30 seconds */
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	retval = tty_register_driver(normal);
	if (retval >= 0)
		return retval;

<<<<<<< HEAD
	put_tty_driver(normal);
out_kfree:
	kfree(drv->state);
out:
	return -ENOMEM;
}

/**
 *	uart_unregister_driver - remove a driver from the uart core layer
 *	@drv: low level driver structure
 *
 *	Remove all references to a driver from the core driver.  The low
 *	level driver must have removed all its ports via the
 *	uart_remove_one_port() if it registered them with uart_add_one_port().
 *	(ie, drv->port == NULL)
=======
	for (i = 0; i < drv->nr; i++)
		tty_port_destroy(&drv->state[i].port);
	tty_driver_kref_put(normal);
out_kfree:
	kfree(drv->state);
out:
	return retval;
}
EXPORT_SYMBOL(uart_register_driver);

/**
 * uart_unregister_driver - remove a driver from the uart core layer
 * @drv: low level driver structure
 *
 * Remove all references to a driver from the core driver. The low level
 * driver must have removed all its ports via the uart_remove_one_port() if it
 * registered them with uart_add_one_port(). (I.e. @drv->port is %NULL.)
 *
 * Locking: none, Interrupts: enabled
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
void uart_unregister_driver(struct uart_driver *drv)
{
	struct tty_driver *p = drv->tty_driver;
<<<<<<< HEAD
	tty_unregister_driver(p);
	put_tty_driver(p);
=======
	unsigned int i;

	tty_unregister_driver(p);
	tty_driver_kref_put(p);
	for (i = 0; i < drv->nr; i++)
		tty_port_destroy(&drv->state[i].port);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kfree(drv->state);
	drv->state = NULL;
	drv->tty_driver = NULL;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL(uart_unregister_driver);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct tty_driver *uart_console_device(struct console *co, int *index)
{
	struct uart_driver *p = co->data;
	*index = co->index;
	return p->tty_driver;
}
<<<<<<< HEAD

/**
 *	uart_add_one_port - attach a driver-defined port structure
 *	@drv: pointer to the uart low level driver structure for this port
 *	@uport: uart port structure to use for this port.
 *
 *	This allows the driver to register its own uart_port structure
 *	with the core driver.  The main purpose is to allow the low
 *	level uart drivers to expand uart_port, rather than having yet
 *	more levels of structures.
 */
int uart_add_one_port(struct uart_driver *drv, struct uart_port *uport)
=======
EXPORT_SYMBOL_GPL(uart_console_device);

static ssize_t uartclk_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct serial_struct tmp;
	struct tty_port *port = dev_get_drvdata(dev);

	uart_get_info(port, &tmp);
	return sprintf(buf, "%d\n", tmp.baud_base * 16);
}

static ssize_t type_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct serial_struct tmp;
	struct tty_port *port = dev_get_drvdata(dev);

	uart_get_info(port, &tmp);
	return sprintf(buf, "%d\n", tmp.type);
}

static ssize_t line_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct serial_struct tmp;
	struct tty_port *port = dev_get_drvdata(dev);

	uart_get_info(port, &tmp);
	return sprintf(buf, "%d\n", tmp.line);
}

static ssize_t port_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct serial_struct tmp;
	struct tty_port *port = dev_get_drvdata(dev);
	unsigned long ioaddr;

	uart_get_info(port, &tmp);
	ioaddr = tmp.port;
	if (HIGH_BITS_OFFSET)
		ioaddr |= (unsigned long)tmp.port_high << HIGH_BITS_OFFSET;
	return sprintf(buf, "0x%lX\n", ioaddr);
}

static ssize_t irq_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct serial_struct tmp;
	struct tty_port *port = dev_get_drvdata(dev);

	uart_get_info(port, &tmp);
	return sprintf(buf, "%d\n", tmp.irq);
}

static ssize_t flags_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct serial_struct tmp;
	struct tty_port *port = dev_get_drvdata(dev);

	uart_get_info(port, &tmp);
	return sprintf(buf, "0x%X\n", tmp.flags);
}

static ssize_t xmit_fifo_size_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct serial_struct tmp;
	struct tty_port *port = dev_get_drvdata(dev);

	uart_get_info(port, &tmp);
	return sprintf(buf, "%d\n", tmp.xmit_fifo_size);
}

static ssize_t close_delay_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct serial_struct tmp;
	struct tty_port *port = dev_get_drvdata(dev);

	uart_get_info(port, &tmp);
	return sprintf(buf, "%d\n", tmp.close_delay);
}

static ssize_t closing_wait_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct serial_struct tmp;
	struct tty_port *port = dev_get_drvdata(dev);

	uart_get_info(port, &tmp);
	return sprintf(buf, "%d\n", tmp.closing_wait);
}

static ssize_t custom_divisor_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct serial_struct tmp;
	struct tty_port *port = dev_get_drvdata(dev);

	uart_get_info(port, &tmp);
	return sprintf(buf, "%d\n", tmp.custom_divisor);
}

static ssize_t io_type_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct serial_struct tmp;
	struct tty_port *port = dev_get_drvdata(dev);

	uart_get_info(port, &tmp);
	return sprintf(buf, "%d\n", tmp.io_type);
}

static ssize_t iomem_base_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct serial_struct tmp;
	struct tty_port *port = dev_get_drvdata(dev);

	uart_get_info(port, &tmp);
	return sprintf(buf, "0x%lX\n", (unsigned long)tmp.iomem_base);
}

static ssize_t iomem_reg_shift_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct serial_struct tmp;
	struct tty_port *port = dev_get_drvdata(dev);

	uart_get_info(port, &tmp);
	return sprintf(buf, "%d\n", tmp.iomem_reg_shift);
}

static ssize_t console_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct tty_port *port = dev_get_drvdata(dev);
	struct uart_state *state = container_of(port, struct uart_state, port);
	struct uart_port *uport;
	bool console = false;

	mutex_lock(&port->mutex);
	uport = uart_port_check(state);
	if (uport)
		console = uart_console_registered(uport);
	mutex_unlock(&port->mutex);

	return sprintf(buf, "%c\n", console ? 'Y' : 'N');
}

static ssize_t console_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct tty_port *port = dev_get_drvdata(dev);
	struct uart_state *state = container_of(port, struct uart_state, port);
	struct uart_port *uport;
	bool oldconsole, newconsole;
	int ret;

	ret = kstrtobool(buf, &newconsole);
	if (ret)
		return ret;

	mutex_lock(&port->mutex);
	uport = uart_port_check(state);
	if (uport) {
		oldconsole = uart_console_registered(uport);
		if (oldconsole && !newconsole) {
			ret = unregister_console(uport->cons);
		} else if (!oldconsole && newconsole) {
			if (uart_console(uport)) {
				uport->console_reinit = 1;
				register_console(uport->cons);
			} else {
				ret = -ENOENT;
			}
		}
	} else {
		ret = -ENXIO;
	}
	mutex_unlock(&port->mutex);

	return ret < 0 ? ret : count;
}

static DEVICE_ATTR_RO(uartclk);
static DEVICE_ATTR_RO(type);
static DEVICE_ATTR_RO(line);
static DEVICE_ATTR_RO(port);
static DEVICE_ATTR_RO(irq);
static DEVICE_ATTR_RO(flags);
static DEVICE_ATTR_RO(xmit_fifo_size);
static DEVICE_ATTR_RO(close_delay);
static DEVICE_ATTR_RO(closing_wait);
static DEVICE_ATTR_RO(custom_divisor);
static DEVICE_ATTR_RO(io_type);
static DEVICE_ATTR_RO(iomem_base);
static DEVICE_ATTR_RO(iomem_reg_shift);
static DEVICE_ATTR_RW(console);

static struct attribute *tty_dev_attrs[] = {
	&dev_attr_uartclk.attr,
	&dev_attr_type.attr,
	&dev_attr_line.attr,
	&dev_attr_port.attr,
	&dev_attr_irq.attr,
	&dev_attr_flags.attr,
	&dev_attr_xmit_fifo_size.attr,
	&dev_attr_close_delay.attr,
	&dev_attr_closing_wait.attr,
	&dev_attr_custom_divisor.attr,
	&dev_attr_io_type.attr,
	&dev_attr_iomem_base.attr,
	&dev_attr_iomem_reg_shift.attr,
	&dev_attr_console.attr,
	NULL
};

static const struct attribute_group tty_dev_attr_group = {
	.attrs = tty_dev_attrs,
};

/**
 * serial_core_add_one_port - attach a driver-defined port structure
 * @drv: pointer to the uart low level driver structure for this port
 * @uport: uart port structure to use for this port.
 *
 * Context: task context, might sleep
 *
 * This allows the driver @drv to register its own uart_port structure with the
 * core driver. The main purpose is to allow the low level uart drivers to
 * expand uart_port, rather than having yet more levels of structures.
 * Caller must hold port_mutex.
 */
static int serial_core_add_one_port(struct uart_driver *drv, struct uart_port *uport)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct uart_state *state;
	struct tty_port *port;
	int ret = 0;
	struct device *tty_dev;
<<<<<<< HEAD

	BUG_ON(in_interrupt());
=======
	int num_groups;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (uport->line >= drv->nr)
		return -EINVAL;

	state = drv->state + uport->line;
	port = &state->port;

<<<<<<< HEAD
	mutex_lock(&port_mutex);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mutex_lock(&port->mutex);
	if (state->uart_port) {
		ret = -EINVAL;
		goto out;
	}

<<<<<<< HEAD
	state->uart_port = uport;
	state->pm_state = -1;

	uport->cons = drv->cons;
	uport->state = state;

	/*
	 * If this port is a console, then the spinlock is already
	 * initialised.
	 */
	if (!(uart_console(uport) && (uport->cons->flags & CON_ENABLED))) {
		spin_lock_init(&uport->lock);
		lockdep_set_class(&uport->lock, &port_lock_key);
	}

	uart_configure_port(drv, state, uport);

	/*
	 * Register the port whether it's detected or not.  This allows
	 * setserial to be used to alter this ports parameters.
	 */
	tty_dev = tty_register_device(drv->tty_driver, uport->line, uport->dev);
	if (likely(!IS_ERR(tty_dev))) {
		device_set_wakeup_capable(tty_dev, 1);
	} else {
		printk(KERN_ERR "Cannot register tty device on line %d\n",
		       uport->line);
	}

	/*
	 * Ensure UPF_DEAD is not set.
	 */
	uport->flags &= ~UPF_DEAD;

 out:
	mutex_unlock(&port->mutex);
	mutex_unlock(&port_mutex);
=======
	/* Link the port to the driver state table and vice versa */
	atomic_set(&state->refcount, 1);
	init_waitqueue_head(&state->remove_wait);
	state->uart_port = uport;
	uport->state = state;

	state->pm_state = UART_PM_STATE_UNDEFINED;
	uport->cons = drv->cons;
	uport->minor = drv->tty_driver->minor_start + uport->line;
	uport->name = kasprintf(GFP_KERNEL, "%s%d", drv->dev_name,
				drv->tty_driver->name_base + uport->line);
	if (!uport->name) {
		ret = -ENOMEM;
		goto out;
	}

	/*
	 * If this port is in use as a console then the spinlock is already
	 * initialised.
	 */
	if (!uart_console_registered(uport))
		uart_port_spin_lock_init(uport);

	if (uport->cons && uport->dev)
		of_console_check(uport->dev->of_node, uport->cons->name, uport->line);

	tty_port_link_device(port, drv->tty_driver, uport->line);
	uart_configure_port(drv, state, uport);

	port->console = uart_console(uport);

	num_groups = 2;
	if (uport->attr_group)
		num_groups++;

	uport->tty_groups = kcalloc(num_groups, sizeof(*uport->tty_groups),
				    GFP_KERNEL);
	if (!uport->tty_groups) {
		ret = -ENOMEM;
		goto out;
	}
	uport->tty_groups[0] = &tty_dev_attr_group;
	if (uport->attr_group)
		uport->tty_groups[1] = uport->attr_group;

	/*
	 * Register the port whether it's detected or not.  This allows
	 * setserial to be used to alter this port's parameters.
	 */
	tty_dev = tty_port_register_device_attr_serdev(port, drv->tty_driver,
			uport->line, uport->dev, &uport->port_dev->dev, port,
			uport->tty_groups);
	if (!IS_ERR(tty_dev)) {
		device_set_wakeup_capable(tty_dev, 1);
	} else {
		dev_err(uport->dev, "Cannot register tty device on line %d\n",
		       uport->line);
	}

 out:
	mutex_unlock(&port->mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ret;
}

/**
<<<<<<< HEAD
 *	uart_remove_one_port - detach a driver defined port structure
 *	@drv: pointer to the uart low level driver structure for this port
 *	@uport: uart port structure for this port
 *
 *	This unhooks (and hangs up) the specified port structure from the
 *	core driver.  No further calls will be made to the low-level code
 *	for this port.
 */
int uart_remove_one_port(struct uart_driver *drv, struct uart_port *uport)
{
	struct uart_state *state = drv->state + uport->line;
	struct tty_port *port = &state->port;

	BUG_ON(in_interrupt());

	if (state->uart_port != uport)
		printk(KERN_ALERT "Removing wrong port: %p != %p\n",
			state->uart_port, uport);

	mutex_lock(&port_mutex);

	/*
	 * Mark the port "dead" - this prevents any opens from
	 * succeeding while we shut down the port.
	 */
	mutex_lock(&port->mutex);
	uport->flags |= UPF_DEAD;
=======
 * serial_core_remove_one_port - detach a driver defined port structure
 * @drv: pointer to the uart low level driver structure for this port
 * @uport: uart port structure for this port
 *
 * Context: task context, might sleep
 *
 * This unhooks (and hangs up) the specified port structure from the core
 * driver. No further calls will be made to the low-level code for this port.
 * Caller must hold port_mutex.
 */
static void serial_core_remove_one_port(struct uart_driver *drv,
					struct uart_port *uport)
{
	struct uart_state *state = drv->state + uport->line;
	struct tty_port *port = &state->port;
	struct uart_port *uart_port;
	struct tty_struct *tty;

	mutex_lock(&port->mutex);
	uart_port = uart_port_check(state);
	if (uart_port != uport)
		dev_alert(uport->dev, "Removing wrong port: %p != %p\n",
			  uart_port, uport);

	if (!uart_port) {
		mutex_unlock(&port->mutex);
		return;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mutex_unlock(&port->mutex);

	/*
	 * Remove the devices from the tty layer
	 */
<<<<<<< HEAD
	tty_unregister_device(drv->tty_driver, uport->line);

	if (port->tty)
		tty_vhangup(port->tty);
=======
	tty_port_unregister_device(port, drv->tty_driver, uport->line);

	tty = tty_port_tty_get(port);
	if (tty) {
		tty_vhangup(port->tty);
		tty_kref_put(tty);
	}

	/*
	 * If the port is used as a console, unregister it
	 */
	if (uart_console(uport))
		unregister_console(uport->cons);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Free the port IO and memory resources, if any.
	 */
<<<<<<< HEAD
	if (uport->type != PORT_UNKNOWN)
		uport->ops->release_port(uport);
=======
	if (uport->type != PORT_UNKNOWN && uport->ops->release_port)
		uport->ops->release_port(uport);
	kfree(uport->tty_groups);
	kfree(uport->name);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Indicate that there isn't a port here anymore.
	 */
	uport->type = PORT_UNKNOWN;
<<<<<<< HEAD

	state->uart_port = NULL;
	mutex_unlock(&port_mutex);
=======
	uport->port_dev = NULL;

	mutex_lock(&port->mutex);
	WARN_ON(atomic_dec_return(&state->refcount) < 0);
	wait_event(state->remove_wait, !atomic_read(&state->refcount));
	state->uart_port = NULL;
	mutex_unlock(&port->mutex);
}

/**
 * uart_match_port - are the two ports equivalent?
 * @port1: first port
 * @port2: second port
 *
 * This utility function can be used to determine whether two uart_port
 * structures describe the same port.
 */
bool uart_match_port(const struct uart_port *port1,
		const struct uart_port *port2)
{
	if (port1->iotype != port2->iotype)
		return false;

	switch (port1->iotype) {
	case UPIO_PORT:
		return port1->iobase == port2->iobase;
	case UPIO_HUB6:
		return port1->iobase == port2->iobase &&
		       port1->hub6   == port2->hub6;
	case UPIO_MEM:
	case UPIO_MEM16:
	case UPIO_MEM32:
	case UPIO_MEM32BE:
	case UPIO_AU:
	case UPIO_TSI:
		return port1->mapbase == port2->mapbase;
	}

	return false;
}
EXPORT_SYMBOL(uart_match_port);

static struct serial_ctrl_device *
serial_core_get_ctrl_dev(struct serial_port_device *port_dev)
{
	struct device *dev = &port_dev->dev;

	return to_serial_base_ctrl_device(dev->parent);
}

/*
 * Find a registered serial core controller device if one exists. Returns
 * the first device matching the ctrl_id. Caller must hold port_mutex.
 */
static struct serial_ctrl_device *serial_core_ctrl_find(struct uart_driver *drv,
							struct device *phys_dev,
							int ctrl_id)
{
	struct uart_state *state;
	int i;

	lockdep_assert_held(&port_mutex);

	for (i = 0; i < drv->nr; i++) {
		state = drv->state + i;
		if (!state->uart_port || !state->uart_port->port_dev)
			continue;

		if (state->uart_port->dev == phys_dev &&
		    state->uart_port->ctrl_id == ctrl_id)
			return serial_core_get_ctrl_dev(state->uart_port->port_dev);
	}

	return NULL;
}

static struct serial_ctrl_device *serial_core_ctrl_device_add(struct uart_port *port)
{
	return serial_base_ctrl_add(port, port->dev);
}

static int serial_core_port_device_add(struct serial_ctrl_device *ctrl_dev,
				       struct uart_port *port)
{
	struct serial_port_device *port_dev;

	port_dev = serial_base_port_add(port, ctrl_dev);
	if (IS_ERR(port_dev))
		return PTR_ERR(port_dev);

	port->port_dev = port_dev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

/*
<<<<<<< HEAD
 *	Are the two ports equivalent?
 */
int uart_match_port(struct uart_port *port1, struct uart_port *port2)
{
	if (port1->iotype != port2->iotype)
		return 0;

	switch (port1->iotype) {
	case UPIO_PORT:
		return (port1->iobase == port2->iobase);
	case UPIO_HUB6:
		return (port1->iobase == port2->iobase) &&
		       (port1->hub6   == port2->hub6);
	case UPIO_MEM:
	case UPIO_MEM32:
	case UPIO_AU:
	case UPIO_TSI:
		return (port1->mapbase == port2->mapbase);
	}
	return 0;
}
EXPORT_SYMBOL(uart_match_port);

/**
 *	uart_handle_dcd_change - handle a change of carrier detect state
 *	@uport: uart_port structure for the open port
 *	@status: new carrier detect status, nonzero if active
 */
void uart_handle_dcd_change(struct uart_port *uport, unsigned int status)
{
	struct uart_state *state = uport->state;
	struct tty_port *port = &state->port;
	struct tty_ldisc *ld = tty_ldisc_ref(port->tty);
	struct pps_event_time ts;

	if (ld && ld->ops->dcd_change)
		pps_get_ts(&ts);

	uport->icount.dcd++;
#ifdef CONFIG_HARD_PPS
	if ((uport->flags & UPF_HARDPPS_CD) && status)
		hardpps();
#endif

	if (port->flags & ASYNC_CHECK_CD) {
		if (status)
			wake_up_interruptible(&port->open_wait);
		else if (port->tty)
			tty_hangup(port->tty);
	}

	if (ld && ld->ops->dcd_change)
		ld->ops->dcd_change(port->tty, status, &ts);
	if (ld)
		tty_ldisc_deref(ld);
=======
 * Initialize a serial core port device, and a controller device if needed.
 */
int serial_core_register_port(struct uart_driver *drv, struct uart_port *port)
{
	struct serial_ctrl_device *ctrl_dev, *new_ctrl_dev = NULL;
	int ret;

	mutex_lock(&port_mutex);

	/*
	 * Prevent serial_port_runtime_resume() from trying to use the port
	 * until serial_core_add_one_port() has completed
	 */
	port->flags |= UPF_DEAD;

	/* Inititalize a serial core controller device if needed */
	ctrl_dev = serial_core_ctrl_find(drv, port->dev, port->ctrl_id);
	if (!ctrl_dev) {
		new_ctrl_dev = serial_core_ctrl_device_add(port);
		if (IS_ERR(new_ctrl_dev)) {
			ret = PTR_ERR(new_ctrl_dev);
			goto err_unlock;
		}
		ctrl_dev = new_ctrl_dev;
	}

	/*
	 * Initialize a serial core port device. Tag the port dead to prevent
	 * serial_port_runtime_resume() trying to do anything until port has
	 * been registered. It gets cleared by serial_core_add_one_port().
	 */
	ret = serial_core_port_device_add(ctrl_dev, port);
	if (ret)
		goto err_unregister_ctrl_dev;

	ret = serial_core_add_one_port(drv, port);
	if (ret)
		goto err_unregister_port_dev;

	port->flags &= ~UPF_DEAD;

	mutex_unlock(&port_mutex);

	return 0;

err_unregister_port_dev:
	serial_base_port_device_remove(port->port_dev);

err_unregister_ctrl_dev:
	serial_base_ctrl_device_remove(new_ctrl_dev);

err_unlock:
	mutex_unlock(&port_mutex);

	return ret;
}

/*
 * Removes a serial core port device, and the related serial core controller
 * device if the last instance.
 */
void serial_core_unregister_port(struct uart_driver *drv, struct uart_port *port)
{
	struct device *phys_dev = port->dev;
	struct serial_port_device *port_dev = port->port_dev;
	struct serial_ctrl_device *ctrl_dev = serial_core_get_ctrl_dev(port_dev);
	int ctrl_id = port->ctrl_id;

	mutex_lock(&port_mutex);

	port->flags |= UPF_DEAD;

	serial_core_remove_one_port(drv, port);

	/* Note that struct uart_port *port is no longer valid at this point */
	serial_base_port_device_remove(port_dev);

	/* Drop the serial core controller device if no ports are using it */
	if (!serial_core_ctrl_find(drv, phys_dev, ctrl_id))
		serial_base_ctrl_device_remove(ctrl_dev);

	mutex_unlock(&port_mutex);
}

/**
 * uart_handle_dcd_change - handle a change of carrier detect state
 * @uport: uart_port structure for the open port
 * @active: new carrier detect status
 *
 * Caller must hold uport->lock.
 */
void uart_handle_dcd_change(struct uart_port *uport, bool active)
{
	struct tty_port *port = &uport->state->port;
	struct tty_struct *tty = port->tty;
	struct tty_ldisc *ld;

	lockdep_assert_held_once(&uport->lock);

	if (tty) {
		ld = tty_ldisc_ref(tty);
		if (ld) {
			if (ld->ops->dcd_change)
				ld->ops->dcd_change(tty, active);
			tty_ldisc_deref(ld);
		}
	}

	uport->icount.dcd++;

	if (uart_dcd_enabled(uport)) {
		if (active)
			wake_up_interruptible(&port->open_wait);
		else if (tty)
			tty_hangup(tty);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(uart_handle_dcd_change);

/**
<<<<<<< HEAD
 *	uart_handle_cts_change - handle a change of clear-to-send state
 *	@uport: uart_port structure for the open port
 *	@status: new clear to send status, nonzero if active
 */
void uart_handle_cts_change(struct uart_port *uport, unsigned int status)
{
	struct tty_port *port = &uport->state->port;
	struct tty_struct *tty = port->tty;

	uport->icount.cts++;

	if (port->flags & ASYNC_CTS_FLOW) {
		if (tty->hw_stopped) {
			if (status) {
				tty->hw_stopped = 0;
=======
 * uart_handle_cts_change - handle a change of clear-to-send state
 * @uport: uart_port structure for the open port
 * @active: new clear-to-send status
 *
 * Caller must hold uport->lock.
 */
void uart_handle_cts_change(struct uart_port *uport, bool active)
{
	lockdep_assert_held_once(&uport->lock);

	uport->icount.cts++;

	if (uart_softcts_mode(uport)) {
		if (uport->hw_stopped) {
			if (active) {
				uport->hw_stopped = false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				uport->ops->start_tx(uport);
				uart_write_wakeup(uport);
			}
		} else {
<<<<<<< HEAD
			if (!status) {
				tty->hw_stopped = 1;
				uport->ops->stop_tx(uport);
			}
		}
=======
			if (!active) {
				uport->hw_stopped = true;
				uport->ops->stop_tx(uport);
			}
		}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}
EXPORT_SYMBOL_GPL(uart_handle_cts_change);

/**
 * uart_insert_char - push a char to the uart layer
 *
 * User is responsible to call tty_flip_buffer_push when they are done with
 * insertion.
 *
 * @port: corresponding port
 * @status: state of the serial port RX buffer (LSR for 8250)
 * @overrun: mask of overrun bits in @status
 * @ch: character to push
 * @flag: flag for the character (see TTY_NORMAL and friends)
 */
void uart_insert_char(struct uart_port *port, unsigned int status,
<<<<<<< HEAD
		 unsigned int overrun, unsigned int ch, unsigned int flag)
{
	struct tty_struct *tty = port->state->port.tty;

	if ((status & port->ignore_status_mask & ~overrun) == 0)
		tty_insert_flip_char(tty, ch, flag);
=======
		      unsigned int overrun, u8 ch, u8 flag)
{
	struct tty_port *tport = &port->state->port;

	if ((status & port->ignore_status_mask & ~overrun) == 0)
		if (tty_insert_flip_char(tport, ch, flag) == 0)
			++port->icount.buf_overrun;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Overrun is special.  Since it's reported immediately,
	 * it doesn't affect the current character.
	 */
	if (status & ~port->ignore_status_mask & overrun)
<<<<<<< HEAD
		tty_insert_flip_char(tty, 0, TTY_OVERRUN);
}
EXPORT_SYMBOL_GPL(uart_insert_char);

EXPORT_SYMBOL(uart_write_wakeup);
EXPORT_SYMBOL(uart_register_driver);
EXPORT_SYMBOL(uart_unregister_driver);
EXPORT_SYMBOL(uart_suspend_port);
EXPORT_SYMBOL(uart_resume_port);
EXPORT_SYMBOL(uart_add_one_port);
EXPORT_SYMBOL(uart_remove_one_port);
=======
		if (tty_insert_flip_char(tport, 0, TTY_OVERRUN) == 0)
			++port->icount.buf_overrun;
}
EXPORT_SYMBOL_GPL(uart_insert_char);

#ifdef CONFIG_MAGIC_SYSRQ_SERIAL
static const u8 sysrq_toggle_seq[] = CONFIG_MAGIC_SYSRQ_SERIAL_SEQUENCE;

static void uart_sysrq_on(struct work_struct *w)
{
	int sysrq_toggle_seq_len = strlen(sysrq_toggle_seq);

	sysrq_toggle_support(1);
	pr_info("SysRq is enabled by magic sequence '%*pE' on serial\n",
		sysrq_toggle_seq_len, sysrq_toggle_seq);
}
static DECLARE_WORK(sysrq_enable_work, uart_sysrq_on);

/**
 * uart_try_toggle_sysrq - Enables SysRq from serial line
 * @port: uart_port structure where char(s) after BREAK met
 * @ch: new character in the sequence after received BREAK
 *
 * Enables magic SysRq when the required sequence is met on port
 * (see CONFIG_MAGIC_SYSRQ_SERIAL_SEQUENCE).
 *
 * Returns: %false if @ch is out of enabling sequence and should be
 * handled some other way, %true if @ch was consumed.
 */
bool uart_try_toggle_sysrq(struct uart_port *port, u8 ch)
{
	int sysrq_toggle_seq_len = strlen(sysrq_toggle_seq);

	if (!sysrq_toggle_seq_len)
		return false;

	BUILD_BUG_ON(ARRAY_SIZE(sysrq_toggle_seq) >= U8_MAX);
	if (sysrq_toggle_seq[port->sysrq_seq] != ch) {
		port->sysrq_seq = 0;
		return false;
	}

	if (++port->sysrq_seq < sysrq_toggle_seq_len) {
		port->sysrq = jiffies + SYSRQ_TIMEOUT;
		return true;
	}

	schedule_work(&sysrq_enable_work);

	port->sysrq = 0;
	return true;
}
EXPORT_SYMBOL_GPL(uart_try_toggle_sysrq);
#endif

/**
 * uart_get_rs485_mode() - retrieve rs485 properties for given uart
 * @port: uart device's target port
 *
 * This function implements the device tree binding described in
 * Documentation/devicetree/bindings/serial/rs485.txt.
 */
int uart_get_rs485_mode(struct uart_port *port)
{
	struct serial_rs485 *rs485conf = &port->rs485;
	struct device *dev = port->dev;
	enum gpiod_flags dflags;
	struct gpio_desc *desc;
	u32 rs485_delay[2];
	int ret;

	if (!(port->rs485_supported.flags & SER_RS485_ENABLED))
		return 0;

	ret = device_property_read_u32_array(dev, "rs485-rts-delay",
					     rs485_delay, 2);
	if (!ret) {
		rs485conf->delay_rts_before_send = rs485_delay[0];
		rs485conf->delay_rts_after_send = rs485_delay[1];
	} else {
		rs485conf->delay_rts_before_send = 0;
		rs485conf->delay_rts_after_send = 0;
	}

	uart_sanitize_serial_rs485_delays(port, rs485conf);

	/*
	 * Clear full-duplex and enabled flags, set RTS polarity to active high
	 * to get to a defined state with the following properties:
	 */
	rs485conf->flags &= ~(SER_RS485_RX_DURING_TX | SER_RS485_ENABLED |
			      SER_RS485_TERMINATE_BUS |
			      SER_RS485_RTS_AFTER_SEND);
	rs485conf->flags |= SER_RS485_RTS_ON_SEND;

	if (device_property_read_bool(dev, "rs485-rx-during-tx"))
		rs485conf->flags |= SER_RS485_RX_DURING_TX;

	if (device_property_read_bool(dev, "linux,rs485-enabled-at-boot-time"))
		rs485conf->flags |= SER_RS485_ENABLED;

	if (device_property_read_bool(dev, "rs485-rts-active-low")) {
		rs485conf->flags &= ~SER_RS485_RTS_ON_SEND;
		rs485conf->flags |= SER_RS485_RTS_AFTER_SEND;
	}

	/*
	 * Disabling termination by default is the safe choice:  Else if many
	 * bus participants enable it, no communication is possible at all.
	 * Works fine for short cables and users may enable for longer cables.
	 */
	desc = devm_gpiod_get_optional(dev, "rs485-term", GPIOD_OUT_LOW);
	if (IS_ERR(desc))
		return dev_err_probe(dev, PTR_ERR(desc), "Cannot get rs485-term-gpios\n");
	port->rs485_term_gpio = desc;
	if (port->rs485_term_gpio)
		port->rs485_supported.flags |= SER_RS485_TERMINATE_BUS;

	dflags = (rs485conf->flags & SER_RS485_RX_DURING_TX) ?
		 GPIOD_OUT_HIGH : GPIOD_OUT_LOW;
	desc = devm_gpiod_get_optional(dev, "rs485-rx-during-tx", dflags);
	if (IS_ERR(desc))
		return dev_err_probe(dev, PTR_ERR(desc), "Cannot get rs485-rx-during-tx-gpios\n");
	port->rs485_rx_during_tx_gpio = desc;
	if (port->rs485_rx_during_tx_gpio)
		port->rs485_supported.flags |= SER_RS485_RX_DURING_TX;

	return 0;
}
EXPORT_SYMBOL_GPL(uart_get_rs485_mode);

/* Compile-time assertions for serial_rs485 layout */
static_assert(offsetof(struct serial_rs485, padding) ==
              (offsetof(struct serial_rs485, delay_rts_after_send) + sizeof(__u32)));
static_assert(offsetof(struct serial_rs485, padding1) ==
	      offsetof(struct serial_rs485, padding[1]));
static_assert((offsetof(struct serial_rs485, padding[4]) + sizeof(__u32)) ==
	      sizeof(struct serial_rs485));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

MODULE_DESCRIPTION("Serial driver core");
MODULE_LICENSE("GPL");
