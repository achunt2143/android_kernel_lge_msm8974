<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  Atheros AR933X SoC built-in UART driver
 *
 *  Copyright (C) 2011 Gabor Juhos <juhosg@openwrt.org>
 *
 *  Based on drivers/char/serial.c, by Linus Torvalds, Theodore Ts'o.
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/module.h>
#include <linux/ioport.h>
#include <linux/init.h>
#include <linux/console.h>
#include <linux/sysrq.h>
#include <linux/delay.h>
<<<<<<< HEAD
#include <linux/platform_device.h>
=======
#include <linux/gpio/consumer.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_platform.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/serial_core.h>
#include <linux/serial.h>
#include <linux/slab.h>
#include <linux/io.h>
#include <linux/irq.h>
<<<<<<< HEAD

#include <asm/mach-ath79/ar933x_uart.h>
#include <asm/mach-ath79/ar933x_uart_platform.h>

#define DRIVER_NAME "ar933x-uart"

=======
#include <linux/clk.h>

#include <asm/div64.h>

#include <asm/mach-ath79/ar933x_uart.h>

#include "serial_mctrl_gpio.h"

#define DRIVER_NAME "ar933x-uart"

#define AR933X_UART_MAX_SCALE	0xff
#define AR933X_UART_MAX_STEP	0xffff

#define AR933X_UART_MIN_BAUD	300
#define AR933X_UART_MAX_BAUD	3000000

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define AR933X_DUMMY_STATUS_RD	0x01

static struct uart_driver ar933x_uart_driver;

struct ar933x_uart_port {
	struct uart_port	port;
	unsigned int		ier;	/* shadow Interrupt Enable Register */
<<<<<<< HEAD
=======
	unsigned int		min_baud;
	unsigned int		max_baud;
	struct clk		*clk;
	struct mctrl_gpios	*gpios;
	struct gpio_desc	*rts_gpiod;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static inline unsigned int ar933x_uart_read(struct ar933x_uart_port *up,
					    int offset)
{
	return readl(up->port.membase + offset);
}

static inline void ar933x_uart_write(struct ar933x_uart_port *up,
				     int offset, unsigned int value)
{
	writel(value, up->port.membase + offset);
}

static inline void ar933x_uart_rmw(struct ar933x_uart_port *up,
				  unsigned int offset,
				  unsigned int mask,
				  unsigned int val)
{
	unsigned int t;

	t = ar933x_uart_read(up, offset);
	t &= ~mask;
	t |= val;
	ar933x_uart_write(up, offset, t);
}

static inline void ar933x_uart_rmw_set(struct ar933x_uart_port *up,
				       unsigned int offset,
				       unsigned int val)
{
	ar933x_uart_rmw(up, offset, 0, val);
}

static inline void ar933x_uart_rmw_clear(struct ar933x_uart_port *up,
					 unsigned int offset,
					 unsigned int val)
{
	ar933x_uart_rmw(up, offset, val, 0);
}

static inline void ar933x_uart_start_tx_interrupt(struct ar933x_uart_port *up)
{
	up->ier |= AR933X_UART_INT_TX_EMPTY;
	ar933x_uart_write(up, AR933X_UART_INT_EN_REG, up->ier);
}

static inline void ar933x_uart_stop_tx_interrupt(struct ar933x_uart_port *up)
{
	up->ier &= ~AR933X_UART_INT_TX_EMPTY;
	ar933x_uart_write(up, AR933X_UART_INT_EN_REG, up->ier);
}

<<<<<<< HEAD
=======
static inline void ar933x_uart_start_rx_interrupt(struct ar933x_uart_port *up)
{
	up->ier |= AR933X_UART_INT_RX_VALID;
	ar933x_uart_write(up, AR933X_UART_INT_EN_REG, up->ier);
}

static inline void ar933x_uart_stop_rx_interrupt(struct ar933x_uart_port *up)
{
	up->ier &= ~AR933X_UART_INT_RX_VALID;
	ar933x_uart_write(up, AR933X_UART_INT_EN_REG, up->ier);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline void ar933x_uart_putc(struct ar933x_uart_port *up, int ch)
{
	unsigned int rdata;

	rdata = ch & AR933X_UART_DATA_TX_RX_MASK;
	rdata |= AR933X_UART_DATA_TX_CSR;
	ar933x_uart_write(up, AR933X_UART_DATA_REG, rdata);
}

static unsigned int ar933x_uart_tx_empty(struct uart_port *port)
{
<<<<<<< HEAD
	struct ar933x_uart_port *up = (struct ar933x_uart_port *) port;
	unsigned long flags;
	unsigned int rdata;

	spin_lock_irqsave(&up->port.lock, flags);
	rdata = ar933x_uart_read(up, AR933X_UART_DATA_REG);
	spin_unlock_irqrestore(&up->port.lock, flags);
=======
	struct ar933x_uart_port *up =
		container_of(port, struct ar933x_uart_port, port);
	unsigned long flags;
	unsigned int rdata;

	uart_port_lock_irqsave(&up->port, &flags);
	rdata = ar933x_uart_read(up, AR933X_UART_DATA_REG);
	uart_port_unlock_irqrestore(&up->port, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return (rdata & AR933X_UART_DATA_TX_CSR) ? 0 : TIOCSER_TEMT;
}

static unsigned int ar933x_uart_get_mctrl(struct uart_port *port)
{
<<<<<<< HEAD
	return TIOCM_CAR;
=======
	struct ar933x_uart_port *up =
		container_of(port, struct ar933x_uart_port, port);
	int ret = TIOCM_CTS | TIOCM_DSR | TIOCM_CAR;

	mctrl_gpio_get(up->gpios, &ret);

	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void ar933x_uart_set_mctrl(struct uart_port *port, unsigned int mctrl)
{
<<<<<<< HEAD
=======
	struct ar933x_uart_port *up =
		container_of(port, struct ar933x_uart_port, port);

	mctrl_gpio_set(up->gpios, mctrl);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void ar933x_uart_start_tx(struct uart_port *port)
{
<<<<<<< HEAD
	struct ar933x_uart_port *up = (struct ar933x_uart_port *) port;
=======
	struct ar933x_uart_port *up =
		container_of(port, struct ar933x_uart_port, port);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ar933x_uart_start_tx_interrupt(up);
}

<<<<<<< HEAD
static void ar933x_uart_stop_tx(struct uart_port *port)
{
	struct ar933x_uart_port *up = (struct ar933x_uart_port *) port;
=======
static void ar933x_uart_wait_tx_complete(struct ar933x_uart_port *up)
{
	unsigned int status;
	unsigned int timeout = 60000;

	/* Wait up to 60ms for the character(s) to be sent. */
	do {
		status = ar933x_uart_read(up, AR933X_UART_CS_REG);
		if (--timeout == 0)
			break;
		udelay(1);
	} while (status & AR933X_UART_CS_TX_BUSY);

	if (timeout == 0)
		dev_err(up->port.dev, "waiting for TX timed out\n");
}

static void ar933x_uart_rx_flush(struct ar933x_uart_port *up)
{
	unsigned int status;

	/* clear RX_VALID interrupt */
	ar933x_uart_write(up, AR933X_UART_INT_REG, AR933X_UART_INT_RX_VALID);

	/* remove characters from the RX FIFO */
	do {
		ar933x_uart_write(up, AR933X_UART_DATA_REG, AR933X_UART_DATA_RX_CSR);
		status = ar933x_uart_read(up, AR933X_UART_DATA_REG);
	} while (status & AR933X_UART_DATA_RX_CSR);
}

static void ar933x_uart_stop_tx(struct uart_port *port)
{
	struct ar933x_uart_port *up =
		container_of(port, struct ar933x_uart_port, port);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ar933x_uart_stop_tx_interrupt(up);
}

static void ar933x_uart_stop_rx(struct uart_port *port)
{
<<<<<<< HEAD
	struct ar933x_uart_port *up = (struct ar933x_uart_port *) port;

	up->ier &= ~AR933X_UART_INT_RX_VALID;
	ar933x_uart_write(up, AR933X_UART_INT_EN_REG, up->ier);
=======
	struct ar933x_uart_port *up =
		container_of(port, struct ar933x_uart_port, port);

	ar933x_uart_stop_rx_interrupt(up);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void ar933x_uart_break_ctl(struct uart_port *port, int break_state)
{
<<<<<<< HEAD
	struct ar933x_uart_port *up = (struct ar933x_uart_port *) port;
	unsigned long flags;

	spin_lock_irqsave(&up->port.lock, flags);
=======
	struct ar933x_uart_port *up =
		container_of(port, struct ar933x_uart_port, port);
	unsigned long flags;

	uart_port_lock_irqsave(&up->port, &flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (break_state == -1)
		ar933x_uart_rmw_set(up, AR933X_UART_CS_REG,
				    AR933X_UART_CS_TX_BREAK);
	else
		ar933x_uart_rmw_clear(up, AR933X_UART_CS_REG,
				      AR933X_UART_CS_TX_BREAK);
<<<<<<< HEAD
	spin_unlock_irqrestore(&up->port.lock, flags);
}

static void ar933x_uart_enable_ms(struct uart_port *port)
{
=======
	uart_port_unlock_irqrestore(&up->port, flags);
}

/*
 * baudrate = (clk / (scale + 1)) * (step * (1 / 2^17))
 */
static unsigned long ar933x_uart_get_baud(unsigned int clk,
					  unsigned int scale,
					  unsigned int step)
{
	u64 t;
	u32 div;

	div = (2 << 16) * (scale + 1);
	t = clk;
	t *= step;
	t += (div / 2);
	do_div(t, div);

	return t;
}

static void ar933x_uart_get_scale_step(unsigned int clk,
				       unsigned int baud,
				       unsigned int *scale,
				       unsigned int *step)
{
	unsigned int tscale;
	long min_diff;

	*scale = 0;
	*step = 0;

	min_diff = baud;
	for (tscale = 0; tscale < AR933X_UART_MAX_SCALE; tscale++) {
		u64 tstep;
		int diff;

		tstep = baud * (tscale + 1);
		tstep *= (2 << 16);
		do_div(tstep, clk);

		if (tstep > AR933X_UART_MAX_STEP)
			break;

		diff = abs(ar933x_uart_get_baud(clk, tscale, tstep) - baud);
		if (diff < min_diff) {
			min_diff = diff;
			*scale = tscale;
			*step = tstep;
		}
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void ar933x_uart_set_termios(struct uart_port *port,
				    struct ktermios *new,
<<<<<<< HEAD
				    struct ktermios *old)
{
	struct ar933x_uart_port *up = (struct ar933x_uart_port *) port;
	unsigned int cs;
	unsigned long flags;
	unsigned int baud, scale;
=======
				    const struct ktermios *old)
{
	struct ar933x_uart_port *up =
		container_of(port, struct ar933x_uart_port, port);
	unsigned int cs;
	unsigned long flags;
	unsigned int baud, scale, step;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Only CS8 is supported */
	new->c_cflag &= ~CSIZE;
	new->c_cflag |= CS8;

	/* Only one stop bit is supported */
	new->c_cflag &= ~CSTOPB;

	cs = 0;
	if (new->c_cflag & PARENB) {
		if (!(new->c_cflag & PARODD))
			cs |= AR933X_UART_CS_PARITY_EVEN;
		else
			cs |= AR933X_UART_CS_PARITY_ODD;
	} else {
		cs |= AR933X_UART_CS_PARITY_NONE;
	}

	/* Mark/space parity is not supported */
	new->c_cflag &= ~CMSPAR;

<<<<<<< HEAD
	baud = uart_get_baud_rate(port, new, old, 0, port->uartclk / 16);
	scale = (port->uartclk / (16 * baud)) - 1;
=======
	baud = uart_get_baud_rate(port, new, old, up->min_baud, up->max_baud);
	ar933x_uart_get_scale_step(port->uartclk, baud, &scale, &step);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Ok, we're now changing the port state. Do it with
	 * interrupts disabled.
	 */
<<<<<<< HEAD
	spin_lock_irqsave(&up->port.lock, flags);
=======
	uart_port_lock_irqsave(&up->port, &flags);

	/* disable the UART */
	ar933x_uart_rmw_clear(up, AR933X_UART_CS_REG,
		      AR933X_UART_CS_IF_MODE_M << AR933X_UART_CS_IF_MODE_S);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Update the per-port timeout. */
	uart_update_timeout(port, new->c_cflag, baud);

	up->port.ignore_status_mask = 0;

	/* ignore all characters if CREAD is not set */
	if ((new->c_cflag & CREAD) == 0)
		up->port.ignore_status_mask |= AR933X_DUMMY_STATUS_RD;

	ar933x_uart_write(up, AR933X_UART_CLOCK_REG,
<<<<<<< HEAD
			  scale << AR933X_UART_CLOCK_SCALE_S | 8192);
=======
			  scale << AR933X_UART_CLOCK_SCALE_S | step);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* setup configuration register */
	ar933x_uart_rmw(up, AR933X_UART_CS_REG, AR933X_UART_CS_PARITY_M, cs);

	/* enable host interrupt */
	ar933x_uart_rmw_set(up, AR933X_UART_CS_REG,
			    AR933X_UART_CS_HOST_INT_EN);

<<<<<<< HEAD
	spin_unlock_irqrestore(&up->port.lock, flags);
=======
	/* enable RX and TX ready overide */
	ar933x_uart_rmw_set(up, AR933X_UART_CS_REG,
		AR933X_UART_CS_TX_READY_ORIDE | AR933X_UART_CS_RX_READY_ORIDE);

	/* reenable the UART */
	ar933x_uart_rmw(up, AR933X_UART_CS_REG,
			AR933X_UART_CS_IF_MODE_M << AR933X_UART_CS_IF_MODE_S,
			AR933X_UART_CS_IF_MODE_DCE << AR933X_UART_CS_IF_MODE_S);

	uart_port_unlock_irqrestore(&up->port, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (tty_termios_baud_rate(new))
		tty_termios_encode_baud_rate(new, baud, baud);
}

static void ar933x_uart_rx_chars(struct ar933x_uart_port *up)
{
<<<<<<< HEAD
	struct tty_struct *tty;
	int max_count = 256;

	tty = tty_port_tty_get(&up->port.state->port);
=======
	struct tty_port *port = &up->port.state->port;
	int max_count = 256;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	do {
		unsigned int rdata;
		unsigned char ch;

		rdata = ar933x_uart_read(up, AR933X_UART_DATA_REG);
		if ((rdata & AR933X_UART_DATA_RX_CSR) == 0)
			break;

		/* remove the character from the FIFO */
		ar933x_uart_write(up, AR933X_UART_DATA_REG,
				  AR933X_UART_DATA_RX_CSR);

<<<<<<< HEAD
		if (!tty) {
			/* discard the data if no tty available */
			continue;
		}

		up->port.icount.rx++;
		ch = rdata & AR933X_UART_DATA_TX_RX_MASK;

		if (uart_handle_sysrq_char(&up->port, ch))
			continue;

		if ((up->port.ignore_status_mask & AR933X_DUMMY_STATUS_RD) == 0)
			tty_insert_flip_char(tty, ch, TTY_NORMAL);
	} while (max_count-- > 0);

	if (tty) {
		tty_flip_buffer_push(tty);
		tty_kref_put(tty);
	}
=======
		up->port.icount.rx++;
		ch = rdata & AR933X_UART_DATA_TX_RX_MASK;

		if (uart_prepare_sysrq_char(&up->port, ch))
			continue;

		if ((up->port.ignore_status_mask & AR933X_DUMMY_STATUS_RD) == 0)
			tty_insert_flip_char(port, ch, TTY_NORMAL);
	} while (max_count-- > 0);

	tty_flip_buffer_push(port);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void ar933x_uart_tx_chars(struct ar933x_uart_port *up)
{
	struct circ_buf *xmit = &up->port.state->xmit;
<<<<<<< HEAD
	int count;
=======
	struct serial_rs485 *rs485conf = &up->port.rs485;
	int count;
	bool half_duplex_send = false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (uart_tx_stopped(&up->port))
		return;

<<<<<<< HEAD
=======
	if ((rs485conf->flags & SER_RS485_ENABLED) &&
	    (up->port.x_char || !uart_circ_empty(xmit))) {
		ar933x_uart_stop_rx_interrupt(up);
		gpiod_set_value(up->rts_gpiod, !!(rs485conf->flags & SER_RS485_RTS_ON_SEND));
		half_duplex_send = true;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	count = up->port.fifosize;
	do {
		unsigned int rdata;

		rdata = ar933x_uart_read(up, AR933X_UART_DATA_REG);
		if ((rdata & AR933X_UART_DATA_TX_CSR) == 0)
			break;

		if (up->port.x_char) {
			ar933x_uart_putc(up, up->port.x_char);
			up->port.icount.tx++;
			up->port.x_char = 0;
			continue;
		}

		if (uart_circ_empty(xmit))
			break;

		ar933x_uart_putc(up, xmit->buf[xmit->tail]);

<<<<<<< HEAD
		xmit->tail = (xmit->tail + 1) & (UART_XMIT_SIZE - 1);
		up->port.icount.tx++;
=======
		uart_xmit_advance(&up->port, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} while (--count > 0);

	if (uart_circ_chars_pending(xmit) < WAKEUP_CHARS)
		uart_write_wakeup(&up->port);

<<<<<<< HEAD
	if (!uart_circ_empty(xmit))
		ar933x_uart_start_tx_interrupt(up);
=======
	if (!uart_circ_empty(xmit)) {
		ar933x_uart_start_tx_interrupt(up);
	} else if (half_duplex_send) {
		ar933x_uart_wait_tx_complete(up);
		ar933x_uart_rx_flush(up);
		ar933x_uart_start_rx_interrupt(up);
		gpiod_set_value(up->rts_gpiod, !!(rs485conf->flags & SER_RS485_RTS_AFTER_SEND));
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static irqreturn_t ar933x_uart_interrupt(int irq, void *dev_id)
{
	struct ar933x_uart_port *up = dev_id;
	unsigned int status;

	status = ar933x_uart_read(up, AR933X_UART_CS_REG);
	if ((status & AR933X_UART_CS_HOST_INT) == 0)
		return IRQ_NONE;

<<<<<<< HEAD
	spin_lock(&up->port.lock);
=======
	uart_port_lock(&up->port);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	status = ar933x_uart_read(up, AR933X_UART_INT_REG);
	status &= ar933x_uart_read(up, AR933X_UART_INT_EN_REG);

	if (status & AR933X_UART_INT_RX_VALID) {
		ar933x_uart_write(up, AR933X_UART_INT_REG,
				  AR933X_UART_INT_RX_VALID);
		ar933x_uart_rx_chars(up);
	}

	if (status & AR933X_UART_INT_TX_EMPTY) {
		ar933x_uart_write(up, AR933X_UART_INT_REG,
				  AR933X_UART_INT_TX_EMPTY);
		ar933x_uart_stop_tx_interrupt(up);
		ar933x_uart_tx_chars(up);
	}

<<<<<<< HEAD
	spin_unlock(&up->port.lock);
=======
	uart_unlock_and_check_sysrq(&up->port);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return IRQ_HANDLED;
}

static int ar933x_uart_startup(struct uart_port *port)
{
<<<<<<< HEAD
	struct ar933x_uart_port *up = (struct ar933x_uart_port *) port;
=======
	struct ar933x_uart_port *up =
		container_of(port, struct ar933x_uart_port, port);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned long flags;
	int ret;

	ret = request_irq(up->port.irq, ar933x_uart_interrupt,
			  up->port.irqflags, dev_name(up->port.dev), up);
	if (ret)
		return ret;

<<<<<<< HEAD
	spin_lock_irqsave(&up->port.lock, flags);
=======
	uart_port_lock_irqsave(&up->port, &flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Enable HOST interrupts */
	ar933x_uart_rmw_set(up, AR933X_UART_CS_REG,
			    AR933X_UART_CS_HOST_INT_EN);

<<<<<<< HEAD
	/* Enable RX interrupts */
	up->ier = AR933X_UART_INT_RX_VALID;
	ar933x_uart_write(up, AR933X_UART_INT_EN_REG, up->ier);

	spin_unlock_irqrestore(&up->port.lock, flags);
=======
	/* enable RX and TX ready overide */
	ar933x_uart_rmw_set(up, AR933X_UART_CS_REG,
		AR933X_UART_CS_TX_READY_ORIDE | AR933X_UART_CS_RX_READY_ORIDE);

	/* Enable RX interrupts */
	ar933x_uart_start_rx_interrupt(up);

	uart_port_unlock_irqrestore(&up->port, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static void ar933x_uart_shutdown(struct uart_port *port)
{
<<<<<<< HEAD
	struct ar933x_uart_port *up = (struct ar933x_uart_port *) port;
=======
	struct ar933x_uart_port *up =
		container_of(port, struct ar933x_uart_port, port);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Disable all interrupts */
	up->ier = 0;
	ar933x_uart_write(up, AR933X_UART_INT_EN_REG, up->ier);

	/* Disable break condition */
	ar933x_uart_rmw_clear(up, AR933X_UART_CS_REG,
			      AR933X_UART_CS_TX_BREAK);

	free_irq(up->port.irq, up);
}

static const char *ar933x_uart_type(struct uart_port *port)
{
	return (port->type == PORT_AR933X) ? "AR933X UART" : NULL;
}

static void ar933x_uart_release_port(struct uart_port *port)
{
	/* Nothing to release ... */
}

static int ar933x_uart_request_port(struct uart_port *port)
{
	/* UARTs always present */
	return 0;
}

static void ar933x_uart_config_port(struct uart_port *port, int flags)
{
	if (flags & UART_CONFIG_TYPE)
		port->type = PORT_AR933X;
}

static int ar933x_uart_verify_port(struct uart_port *port,
				   struct serial_struct *ser)
{
<<<<<<< HEAD
=======
	struct ar933x_uart_port *up =
		container_of(port, struct ar933x_uart_port, port);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ser->type != PORT_UNKNOWN &&
	    ser->type != PORT_AR933X)
		return -EINVAL;

	if (ser->irq < 0 || ser->irq >= NR_IRQS)
		return -EINVAL;

<<<<<<< HEAD
	if (ser->baud_base < 28800)
=======
	if (ser->baud_base < up->min_baud ||
	    ser->baud_base > up->max_baud)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;

	return 0;
}

<<<<<<< HEAD
static struct uart_ops ar933x_uart_ops = {
=======
static const struct uart_ops ar933x_uart_ops = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.tx_empty	= ar933x_uart_tx_empty,
	.set_mctrl	= ar933x_uart_set_mctrl,
	.get_mctrl	= ar933x_uart_get_mctrl,
	.stop_tx	= ar933x_uart_stop_tx,
	.start_tx	= ar933x_uart_start_tx,
	.stop_rx	= ar933x_uart_stop_rx,
<<<<<<< HEAD
	.enable_ms	= ar933x_uart_enable_ms,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.break_ctl	= ar933x_uart_break_ctl,
	.startup	= ar933x_uart_startup,
	.shutdown	= ar933x_uart_shutdown,
	.set_termios	= ar933x_uart_set_termios,
	.type		= ar933x_uart_type,
	.release_port	= ar933x_uart_release_port,
	.request_port	= ar933x_uart_request_port,
	.config_port	= ar933x_uart_config_port,
	.verify_port	= ar933x_uart_verify_port,
};

<<<<<<< HEAD
#ifdef CONFIG_SERIAL_AR933X_CONSOLE

=======
static int ar933x_config_rs485(struct uart_port *port, struct ktermios *termios,
				struct serial_rs485 *rs485conf)
{
	struct ar933x_uart_port *up =
			container_of(port, struct ar933x_uart_port, port);

	if (port->rs485.flags & SER_RS485_ENABLED)
		gpiod_set_value(up->rts_gpiod,
			!!(rs485conf->flags & SER_RS485_RTS_AFTER_SEND));

	return 0;
}

#ifdef CONFIG_SERIAL_AR933X_CONSOLE
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct ar933x_uart_port *
ar933x_console_ports[CONFIG_SERIAL_AR933X_NR_UARTS];

static void ar933x_uart_wait_xmitr(struct ar933x_uart_port *up)
{
	unsigned int status;
	unsigned int timeout = 60000;

	/* Wait up to 60ms for the character(s) to be sent. */
	do {
		status = ar933x_uart_read(up, AR933X_UART_DATA_REG);
		if (--timeout == 0)
			break;
		udelay(1);
	} while ((status & AR933X_UART_DATA_TX_CSR) == 0);
}

<<<<<<< HEAD
static void ar933x_uart_console_putchar(struct uart_port *port, int ch)
{
	struct ar933x_uart_port *up = (struct ar933x_uart_port *) port;
=======
static void ar933x_uart_console_putchar(struct uart_port *port, unsigned char ch)
{
	struct ar933x_uart_port *up =
		container_of(port, struct ar933x_uart_port, port);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ar933x_uart_wait_xmitr(up);
	ar933x_uart_putc(up, ch);
}

static void ar933x_uart_console_write(struct console *co, const char *s,
				      unsigned int count)
{
	struct ar933x_uart_port *up = ar933x_console_ports[co->index];
	unsigned long flags;
	unsigned int int_en;
	int locked = 1;

<<<<<<< HEAD
	local_irq_save(flags);

	if (up->port.sysrq)
		locked = 0;
	else if (oops_in_progress)
		locked = spin_trylock(&up->port.lock);
	else
		spin_lock(&up->port.lock);
=======
	if (oops_in_progress)
		locked = uart_port_trylock_irqsave(&up->port, &flags);
	else
		uart_port_lock_irqsave(&up->port, &flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * First save the IER then disable the interrupts
	 */
	int_en = ar933x_uart_read(up, AR933X_UART_INT_EN_REG);
	ar933x_uart_write(up, AR933X_UART_INT_EN_REG, 0);

	uart_console_write(&up->port, s, count, ar933x_uart_console_putchar);

	/*
	 * Finally, wait for transmitter to become empty
	 * and restore the IER
	 */
	ar933x_uart_wait_xmitr(up);
	ar933x_uart_write(up, AR933X_UART_INT_EN_REG, int_en);

	ar933x_uart_write(up, AR933X_UART_INT_REG, AR933X_UART_INT_ALLINTS);

	if (locked)
<<<<<<< HEAD
		spin_unlock(&up->port.lock);

	local_irq_restore(flags);
=======
		uart_port_unlock_irqrestore(&up->port, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int ar933x_uart_console_setup(struct console *co, char *options)
{
	struct ar933x_uart_port *up;
	int baud = 115200;
	int bits = 8;
	int parity = 'n';
	int flow = 'n';

	if (co->index < 0 || co->index >= CONFIG_SERIAL_AR933X_NR_UARTS)
		return -EINVAL;

	up = ar933x_console_ports[co->index];
	if (!up)
		return -ENODEV;

	if (options)
		uart_parse_options(options, &baud, &parity, &bits, &flow);

	return uart_set_options(&up->port, co, baud, parity, bits, flow);
}

static struct console ar933x_uart_console = {
	.name		= "ttyATH",
	.write		= ar933x_uart_console_write,
	.device		= uart_console_device,
	.setup		= ar933x_uart_console_setup,
	.flags		= CON_PRINTBUFFER,
	.index		= -1,
	.data		= &ar933x_uart_driver,
};
<<<<<<< HEAD

static void ar933x_uart_add_console_port(struct ar933x_uart_port *up)
{
	ar933x_console_ports[up->port.line] = up;
}

#define AR933X_SERIAL_CONSOLE	(&ar933x_uart_console)

#else

static inline void ar933x_uart_add_console_port(struct ar933x_uart_port *up) {}

#define AR933X_SERIAL_CONSOLE	NULL

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* CONFIG_SERIAL_AR933X_CONSOLE */

static struct uart_driver ar933x_uart_driver = {
	.owner		= THIS_MODULE,
	.driver_name	= DRIVER_NAME,
	.dev_name	= "ttyATH",
	.nr		= CONFIG_SERIAL_AR933X_NR_UARTS,
<<<<<<< HEAD
	.cons		= AR933X_SERIAL_CONSOLE,
};

static int __devinit ar933x_uart_probe(struct platform_device *pdev)
{
	struct ar933x_uart_platform_data *pdata;
	struct ar933x_uart_port *up;
	struct uart_port *port;
	struct resource *mem_res;
	struct resource *irq_res;
	int id;
	int ret;

	pdata = pdev->dev.platform_data;
	if (!pdata)
		return -EINVAL;

	id = pdev->id;
	if (id == -1)
		id = 0;

	if (id > CONFIG_SERIAL_AR933X_NR_UARTS)
		return -EINVAL;

	mem_res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!mem_res) {
		dev_err(&pdev->dev, "no MEM resource\n");
		return -EINVAL;
	}

	irq_res = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	if (!irq_res) {
		dev_err(&pdev->dev, "no IRQ resource\n");
		return -EINVAL;
	}

	up = kzalloc(sizeof(struct ar933x_uart_port), GFP_KERNEL);
	if (!up)
		return -ENOMEM;

	port = &up->port;
	port->mapbase = mem_res->start;

	port->membase = ioremap(mem_res->start, AR933X_UART_REGS_SIZE);
	if (!port->membase) {
		ret = -ENOMEM;
		goto err_free_up;
	}

	port->line = id;
	port->irq = irq_res->start;
	port->dev = &pdev->dev;
	port->type = PORT_AR933X;
	port->iotype = UPIO_MEM32;
	port->uartclk = pdata->uartclk;
=======
	.cons		= NULL, /* filled in runtime */
};

static const struct serial_rs485 ar933x_no_rs485 = {};
static const struct serial_rs485 ar933x_rs485_supported = {
	.flags = SER_RS485_ENABLED | SER_RS485_RTS_ON_SEND | SER_RS485_RTS_AFTER_SEND,
};

static int ar933x_uart_probe(struct platform_device *pdev)
{
	struct ar933x_uart_port *up;
	struct uart_port *port;
	struct resource *mem_res;
	struct device_node *np;
	unsigned int baud;
	int id;
	int ret;
	int irq;

	np = pdev->dev.of_node;
	if (IS_ENABLED(CONFIG_OF) && np) {
		id = of_alias_get_id(np, "serial");
		if (id < 0) {
			dev_err(&pdev->dev, "unable to get alias id, err=%d\n",
				id);
			return id;
		}
	} else {
		id = pdev->id;
		if (id == -1)
			id = 0;
	}

	if (id >= CONFIG_SERIAL_AR933X_NR_UARTS)
		return -EINVAL;

	irq = platform_get_irq(pdev, 0);
	if (irq < 0)
		return irq;

	up = devm_kzalloc(&pdev->dev, sizeof(struct ar933x_uart_port),
			  GFP_KERNEL);
	if (!up)
		return -ENOMEM;

	up->clk = devm_clk_get(&pdev->dev, "uart");
	if (IS_ERR(up->clk)) {
		dev_err(&pdev->dev, "unable to get UART clock\n");
		return PTR_ERR(up->clk);
	}

	port = &up->port;

	port->membase = devm_platform_get_and_ioremap_resource(pdev, 0, &mem_res);
	if (IS_ERR(port->membase))
		return PTR_ERR(port->membase);

	ret = clk_prepare_enable(up->clk);
	if (ret)
		return ret;

	port->uartclk = clk_get_rate(up->clk);
	if (!port->uartclk) {
		ret = -EINVAL;
		goto err_disable_clk;
	}

	port->mapbase = mem_res->start;
	port->line = id;
	port->irq = irq;
	port->dev = &pdev->dev;
	port->type = PORT_AR933X;
	port->iotype = UPIO_MEM32;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	port->regshift = 2;
	port->fifosize = AR933X_UART_FIFO_SIZE;
	port->ops = &ar933x_uart_ops;
<<<<<<< HEAD

	ar933x_uart_add_console_port(up);

	ret = uart_add_one_port(&ar933x_uart_driver, &up->port);
	if (ret)
		goto err_unmap;
=======
	port->rs485_config = ar933x_config_rs485;
	port->rs485_supported = ar933x_rs485_supported;

	baud = ar933x_uart_get_baud(port->uartclk, AR933X_UART_MAX_SCALE, 1);
	up->min_baud = max_t(unsigned int, baud, AR933X_UART_MIN_BAUD);

	baud = ar933x_uart_get_baud(port->uartclk, 0, AR933X_UART_MAX_STEP);
	up->max_baud = min_t(unsigned int, baud, AR933X_UART_MAX_BAUD);

	ret = uart_get_rs485_mode(port);
	if (ret)
		goto err_disable_clk;

	up->gpios = mctrl_gpio_init(port, 0);
	if (IS_ERR(up->gpios) && PTR_ERR(up->gpios) != -ENOSYS) {
		ret = PTR_ERR(up->gpios);
		goto err_disable_clk;
	}

	up->rts_gpiod = mctrl_gpio_to_gpiod(up->gpios, UART_GPIO_RTS);

	if (!up->rts_gpiod) {
		port->rs485_supported = ar933x_no_rs485;
		if (port->rs485.flags & SER_RS485_ENABLED) {
			dev_err(&pdev->dev, "lacking rts-gpio, disabling RS485\n");
			port->rs485.flags &= ~SER_RS485_ENABLED;
		}
	}

#ifdef CONFIG_SERIAL_AR933X_CONSOLE
	ar933x_console_ports[up->port.line] = up;
#endif

	ret = uart_add_one_port(&ar933x_uart_driver, &up->port);
	if (ret)
		goto err_disable_clk;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	platform_set_drvdata(pdev, up);
	return 0;

<<<<<<< HEAD
err_unmap:
	iounmap(up->port.membase);
err_free_up:
	kfree(up);
	return ret;
}

static int __devexit ar933x_uart_remove(struct platform_device *pdev)
=======
err_disable_clk:
	clk_disable_unprepare(up->clk);
	return ret;
}

static void ar933x_uart_remove(struct platform_device *pdev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ar933x_uart_port *up;

	up = platform_get_drvdata(pdev);
<<<<<<< HEAD
	platform_set_drvdata(pdev, NULL);

	if (up) {
		uart_remove_one_port(&ar933x_uart_driver, &up->port);
		iounmap(up->port.membase);
		kfree(up);
	}

	return 0;
}

static struct platform_driver ar933x_uart_platform_driver = {
	.probe		= ar933x_uart_probe,
	.remove		= __devexit_p(ar933x_uart_remove),
	.driver		= {
		.name		= DRIVER_NAME,
		.owner		= THIS_MODULE,
=======

	if (up) {
		uart_remove_one_port(&ar933x_uart_driver, &up->port);
		clk_disable_unprepare(up->clk);
	}
}

#ifdef CONFIG_OF
static const struct of_device_id ar933x_uart_of_ids[] = {
	{ .compatible = "qca,ar9330-uart" },
	{},
};
MODULE_DEVICE_TABLE(of, ar933x_uart_of_ids);
#endif

static struct platform_driver ar933x_uart_platform_driver = {
	.probe		= ar933x_uart_probe,
	.remove_new	= ar933x_uart_remove,
	.driver		= {
		.name		= DRIVER_NAME,
		.of_match_table = of_match_ptr(ar933x_uart_of_ids),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
};

static int __init ar933x_uart_init(void)
{
	int ret;

<<<<<<< HEAD
	ar933x_uart_driver.nr = CONFIG_SERIAL_AR933X_NR_UARTS;
=======
#ifdef CONFIG_SERIAL_AR933X_CONSOLE
	ar933x_uart_driver.cons = &ar933x_uart_console;
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ret = uart_register_driver(&ar933x_uart_driver);
	if (ret)
		goto err_out;

	ret = platform_driver_register(&ar933x_uart_platform_driver);
	if (ret)
		goto err_unregister_uart_driver;

	return 0;

err_unregister_uart_driver:
	uart_unregister_driver(&ar933x_uart_driver);
err_out:
	return ret;
}

static void __exit ar933x_uart_exit(void)
{
	platform_driver_unregister(&ar933x_uart_platform_driver);
	uart_unregister_driver(&ar933x_uart_driver);
}

module_init(ar933x_uart_init);
module_exit(ar933x_uart_exit);

MODULE_DESCRIPTION("Atheros AR933X UART driver");
MODULE_AUTHOR("Gabor Juhos <juhosg@openwrt.org>");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("platform:" DRIVER_NAME);
