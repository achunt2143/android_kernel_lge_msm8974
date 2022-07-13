<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0+
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * altera_jtaguart.c -- Altera JTAG UART driver
 *
 * Based on mcf.c -- Freescale ColdFire UART driver
 *
 * (C) Copyright 2003-2007, Greg Ungerer <gerg@snapgear.com>
 * (C) Copyright 2008, Thomas Chou <thomas@wytron.com.tw>
 * (C) Copyright 2010, Tobias Klauser <tklauser@distanz.ch>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

=======
 */

#include <linux/bitfield.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/console.h>
#include <linux/of.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/serial.h>
#include <linux/serial_core.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/altera_jtaguart.h>

#define DRV_NAME "altera_jtaguart"

/*
 * Altera JTAG UART register definitions according to the Altera JTAG UART
<<<<<<< HEAD
 * datasheet: http://www.altera.com/literature/hb/nios2/n2cpu_nii51009.pdf
=======
 * datasheet: https://www.altera.com/literature/hb/nios2/n2cpu_nii51009.pdf
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#define ALTERA_JTAGUART_SIZE			8

#define ALTERA_JTAGUART_DATA_REG		0

#define ALTERA_JTAGUART_DATA_DATA_MSK		0x000000FF
#define ALTERA_JTAGUART_DATA_RVALID_MSK		0x00008000
#define ALTERA_JTAGUART_DATA_RAVAIL_MSK		0xFFFF0000
#define ALTERA_JTAGUART_DATA_RAVAIL_OFF		16

#define ALTERA_JTAGUART_CONTROL_REG		4

#define ALTERA_JTAGUART_CONTROL_RE_MSK		0x00000001
#define ALTERA_JTAGUART_CONTROL_WE_MSK		0x00000002
#define ALTERA_JTAGUART_CONTROL_RI_MSK		0x00000100
#define ALTERA_JTAGUART_CONTROL_RI_OFF		8
#define ALTERA_JTAGUART_CONTROL_WI_MSK		0x00000200
#define ALTERA_JTAGUART_CONTROL_AC_MSK		0x00000400
#define ALTERA_JTAGUART_CONTROL_WSPACE_MSK	0xFFFF0000
<<<<<<< HEAD
#define ALTERA_JTAGUART_CONTROL_WSPACE_OFF	16

/*
 * Local per-uart structure.
 */
struct altera_jtaguart {
	struct uart_port port;
	unsigned int sigs;	/* Local copy of line sigs */
	unsigned long imr;	/* Local IMR mirror */
};

static unsigned int altera_jtaguart_tx_empty(struct uart_port *port)
{
	return (readl(port->membase + ALTERA_JTAGUART_CONTROL_REG) &
		ALTERA_JTAGUART_CONTROL_WSPACE_MSK) ? TIOCSER_TEMT : 0;
=======

static unsigned int altera_jtaguart_tx_space(struct uart_port *port, u32 *ctlp)
{
	u32 ctl = readl(port->membase + ALTERA_JTAGUART_CONTROL_REG);

	if (ctlp)
		*ctlp = ctl;

	return FIELD_GET(ALTERA_JTAGUART_CONTROL_WSPACE_MSK, ctl);
}

static unsigned int altera_jtaguart_tx_empty(struct uart_port *port)
{
	return altera_jtaguart_tx_space(port, NULL) ? TIOCSER_TEMT : 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static unsigned int altera_jtaguart_get_mctrl(struct uart_port *port)
{
	return TIOCM_CAR | TIOCM_DSR | TIOCM_CTS;
}

static void altera_jtaguart_set_mctrl(struct uart_port *port, unsigned int sigs)
{
}

static void altera_jtaguart_start_tx(struct uart_port *port)
{
<<<<<<< HEAD
	struct altera_jtaguart *pp =
	    container_of(port, struct altera_jtaguart, port);

	pp->imr |= ALTERA_JTAGUART_CONTROL_WE_MSK;
	writel(pp->imr, port->membase + ALTERA_JTAGUART_CONTROL_REG);
=======
	port->read_status_mask |= ALTERA_JTAGUART_CONTROL_WE_MSK;
	writel(port->read_status_mask,
			port->membase + ALTERA_JTAGUART_CONTROL_REG);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void altera_jtaguart_stop_tx(struct uart_port *port)
{
<<<<<<< HEAD
	struct altera_jtaguart *pp =
	    container_of(port, struct altera_jtaguart, port);

	pp->imr &= ~ALTERA_JTAGUART_CONTROL_WE_MSK;
	writel(pp->imr, port->membase + ALTERA_JTAGUART_CONTROL_REG);
=======
	port->read_status_mask &= ~ALTERA_JTAGUART_CONTROL_WE_MSK;
	writel(port->read_status_mask,
			port->membase + ALTERA_JTAGUART_CONTROL_REG);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void altera_jtaguart_stop_rx(struct uart_port *port)
{
<<<<<<< HEAD
	struct altera_jtaguart *pp =
	    container_of(port, struct altera_jtaguart, port);

	pp->imr &= ~ALTERA_JTAGUART_CONTROL_RE_MSK;
	writel(pp->imr, port->membase + ALTERA_JTAGUART_CONTROL_REG);
=======
	port->read_status_mask &= ~ALTERA_JTAGUART_CONTROL_RE_MSK;
	writel(port->read_status_mask,
			port->membase + ALTERA_JTAGUART_CONTROL_REG);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void altera_jtaguart_break_ctl(struct uart_port *port, int break_state)
{
}

<<<<<<< HEAD
static void altera_jtaguart_enable_ms(struct uart_port *port)
{
}

static void altera_jtaguart_set_termios(struct uart_port *port,
					struct ktermios *termios,
					struct ktermios *old)
=======
static void altera_jtaguart_set_termios(struct uart_port *port,
				        struct ktermios *termios,
				        const struct ktermios *old)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	/* Just copy the old termios settings back */
	if (old)
		tty_termios_copy_hw(termios, old);
}

<<<<<<< HEAD
static void altera_jtaguart_rx_chars(struct altera_jtaguart *pp)
{
	struct uart_port *port = &pp->port;
	unsigned char ch, flag;
	unsigned long status;
=======
static void altera_jtaguart_rx_chars(struct uart_port *port)
{
	u32 status;
	u8 ch;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	while ((status = readl(port->membase + ALTERA_JTAGUART_DATA_REG)) &
	       ALTERA_JTAGUART_DATA_RVALID_MSK) {
		ch = status & ALTERA_JTAGUART_DATA_DATA_MSK;
<<<<<<< HEAD
		flag = TTY_NORMAL;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		port->icount.rx++;

		if (uart_handle_sysrq_char(port, ch))
			continue;
<<<<<<< HEAD
		uart_insert_char(port, 0, 0, ch, flag);
	}

	tty_flip_buffer_push(port->state->port.tty);
}

static void altera_jtaguart_tx_chars(struct altera_jtaguart *pp)
{
	struct uart_port *port = &pp->port;
	struct circ_buf *xmit = &port->state->xmit;
	unsigned int pending, count;

	if (port->x_char) {
		/* Send special char - probably flow control */
		writel(port->x_char, port->membase + ALTERA_JTAGUART_DATA_REG);
		port->x_char = 0;
		port->icount.tx++;
		return;
	}

	pending = uart_circ_chars_pending(xmit);
	if (pending > 0) {
		count = (readl(port->membase + ALTERA_JTAGUART_CONTROL_REG) &
				ALTERA_JTAGUART_CONTROL_WSPACE_MSK) >>
			ALTERA_JTAGUART_CONTROL_WSPACE_OFF;
		if (count > pending)
			count = pending;
		if (count > 0) {
			pending -= count;
			while (count--) {
				writel(xmit->buf[xmit->tail],
				       port->membase + ALTERA_JTAGUART_DATA_REG);
				xmit->tail = (xmit->tail + 1) & (UART_XMIT_SIZE - 1);
				port->icount.tx++;
			}
			if (pending < WAKEUP_CHARS)
				uart_write_wakeup(port);
		}
	}

	if (pending == 0) {
		pp->imr &= ~ALTERA_JTAGUART_CONTROL_WE_MSK;
		writel(pp->imr, port->membase + ALTERA_JTAGUART_CONTROL_REG);
	}
=======
		uart_insert_char(port, 0, 0, ch, TTY_NORMAL);
	}

	tty_flip_buffer_push(&port->state->port);
}

static void altera_jtaguart_tx_chars(struct uart_port *port)
{
	unsigned int count;
	u8 ch;

	count = altera_jtaguart_tx_space(port, NULL);

	uart_port_tx_limited(port, ch, count,
		true,
		writel(ch, port->membase + ALTERA_JTAGUART_DATA_REG),
		({}));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static irqreturn_t altera_jtaguart_interrupt(int irq, void *data)
{
	struct uart_port *port = data;
<<<<<<< HEAD
	struct altera_jtaguart *pp =
	    container_of(port, struct altera_jtaguart, port);
	unsigned int isr;

	isr = (readl(port->membase + ALTERA_JTAGUART_CONTROL_REG) >>
	       ALTERA_JTAGUART_CONTROL_RI_OFF) & pp->imr;

	spin_lock(&port->lock);

	if (isr & ALTERA_JTAGUART_CONTROL_RE_MSK)
		altera_jtaguart_rx_chars(pp);
	if (isr & ALTERA_JTAGUART_CONTROL_WE_MSK)
		altera_jtaguart_tx_chars(pp);

	spin_unlock(&port->lock);
=======
	unsigned int isr;

	isr = (readl(port->membase + ALTERA_JTAGUART_CONTROL_REG) >>
	       ALTERA_JTAGUART_CONTROL_RI_OFF) & port->read_status_mask;

	uart_port_lock(port);

	if (isr & ALTERA_JTAGUART_CONTROL_RE_MSK)
		altera_jtaguart_rx_chars(port);
	if (isr & ALTERA_JTAGUART_CONTROL_WE_MSK)
		altera_jtaguart_tx_chars(port);

	uart_port_unlock(port);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return IRQ_RETVAL(isr);
}

static void altera_jtaguart_config_port(struct uart_port *port, int flags)
{
	port->type = PORT_ALTERA_JTAGUART;

	/* Clear mask, so no surprise interrupts. */
	writel(0, port->membase + ALTERA_JTAGUART_CONTROL_REG);
}

static int altera_jtaguart_startup(struct uart_port *port)
{
<<<<<<< HEAD
	struct altera_jtaguart *pp =
	    container_of(port, struct altera_jtaguart, port);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned long flags;
	int ret;

	ret = request_irq(port->irq, altera_jtaguart_interrupt, 0,
			DRV_NAME, port);
	if (ret) {
		pr_err(DRV_NAME ": unable to attach Altera JTAG UART %d "
		       "interrupt vector=%d\n", port->line, port->irq);
		return ret;
	}

<<<<<<< HEAD
	spin_lock_irqsave(&port->lock, flags);

	/* Enable RX interrupts now */
	pp->imr = ALTERA_JTAGUART_CONTROL_RE_MSK;
	writel(pp->imr, port->membase + ALTERA_JTAGUART_CONTROL_REG);

	spin_unlock_irqrestore(&port->lock, flags);
=======
	uart_port_lock_irqsave(port, &flags);

	/* Enable RX interrupts now */
	port->read_status_mask = ALTERA_JTAGUART_CONTROL_RE_MSK;
	writel(port->read_status_mask,
			port->membase + ALTERA_JTAGUART_CONTROL_REG);

	uart_port_unlock_irqrestore(port, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static void altera_jtaguart_shutdown(struct uart_port *port)
{
<<<<<<< HEAD
	struct altera_jtaguart *pp =
	    container_of(port, struct altera_jtaguart, port);
	unsigned long flags;

	spin_lock_irqsave(&port->lock, flags);

	/* Disable all interrupts now */
	pp->imr = 0;
	writel(pp->imr, port->membase + ALTERA_JTAGUART_CONTROL_REG);

	spin_unlock_irqrestore(&port->lock, flags);
=======
	unsigned long flags;

	uart_port_lock_irqsave(port, &flags);

	/* Disable all interrupts now */
	port->read_status_mask = 0;
	writel(port->read_status_mask,
			port->membase + ALTERA_JTAGUART_CONTROL_REG);

	uart_port_unlock_irqrestore(port, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	free_irq(port->irq, port);
}

static const char *altera_jtaguart_type(struct uart_port *port)
{
	return (port->type == PORT_ALTERA_JTAGUART) ? "Altera JTAG UART" : NULL;
}

static int altera_jtaguart_request_port(struct uart_port *port)
{
	/* UARTs always present */
	return 0;
}

static void altera_jtaguart_release_port(struct uart_port *port)
{
	/* Nothing to release... */
}

static int altera_jtaguart_verify_port(struct uart_port *port,
				       struct serial_struct *ser)
{
	if (ser->type != PORT_UNKNOWN && ser->type != PORT_ALTERA_JTAGUART)
		return -EINVAL;
	return 0;
}

/*
 *	Define the basic serial functions we support.
 */
<<<<<<< HEAD
static struct uart_ops altera_jtaguart_ops = {
=======
static const struct uart_ops altera_jtaguart_ops = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.tx_empty	= altera_jtaguart_tx_empty,
	.get_mctrl	= altera_jtaguart_get_mctrl,
	.set_mctrl	= altera_jtaguart_set_mctrl,
	.start_tx	= altera_jtaguart_start_tx,
	.stop_tx	= altera_jtaguart_stop_tx,
	.stop_rx	= altera_jtaguart_stop_rx,
<<<<<<< HEAD
	.enable_ms	= altera_jtaguart_enable_ms,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.break_ctl	= altera_jtaguart_break_ctl,
	.startup	= altera_jtaguart_startup,
	.shutdown	= altera_jtaguart_shutdown,
	.set_termios	= altera_jtaguart_set_termios,
	.type		= altera_jtaguart_type,
	.request_port	= altera_jtaguart_request_port,
	.release_port	= altera_jtaguart_release_port,
	.config_port	= altera_jtaguart_config_port,
	.verify_port	= altera_jtaguart_verify_port,
};

#define ALTERA_JTAGUART_MAXPORTS 1
<<<<<<< HEAD
static struct altera_jtaguart altera_jtaguart_ports[ALTERA_JTAGUART_MAXPORTS];
=======
static struct uart_port altera_jtaguart_ports[ALTERA_JTAGUART_MAXPORTS];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#if defined(CONFIG_SERIAL_ALTERA_JTAGUART_CONSOLE)

#if defined(CONFIG_SERIAL_ALTERA_JTAGUART_CONSOLE_BYPASS)
<<<<<<< HEAD
static void altera_jtaguart_console_putc(struct console *co, const char c)
{
	struct uart_port *port = &(altera_jtaguart_ports + co->index)->port;
	unsigned long status;
	unsigned long flags;

	spin_lock_irqsave(&port->lock, flags);
	while (((status = readl(port->membase + ALTERA_JTAGUART_CONTROL_REG)) &
		ALTERA_JTAGUART_CONTROL_WSPACE_MSK) == 0) {
		if ((status & ALTERA_JTAGUART_CONTROL_AC_MSK) == 0) {
			spin_unlock_irqrestore(&port->lock, flags);
			return;	/* no connection activity */
		}
		spin_unlock_irqrestore(&port->lock, flags);
		cpu_relax();
		spin_lock_irqsave(&port->lock, flags);
	}
	writel(c, port->membase + ALTERA_JTAGUART_DATA_REG);
	spin_unlock_irqrestore(&port->lock, flags);
}
#else
static void altera_jtaguart_console_putc(struct console *co, const char c)
{
	struct uart_port *port = &(altera_jtaguart_ports + co->index)->port;
	unsigned long flags;

	spin_lock_irqsave(&port->lock, flags);
	while ((readl(port->membase + ALTERA_JTAGUART_CONTROL_REG) &
		ALTERA_JTAGUART_CONTROL_WSPACE_MSK) == 0) {
		spin_unlock_irqrestore(&port->lock, flags);
		cpu_relax();
		spin_lock_irqsave(&port->lock, flags);
	}
	writel(c, port->membase + ALTERA_JTAGUART_DATA_REG);
	spin_unlock_irqrestore(&port->lock, flags);
=======
static void altera_jtaguart_console_putc(struct uart_port *port, unsigned char c)
{
	unsigned long flags;
	u32 status;

	uart_port_lock_irqsave(port, &flags);
	while (!altera_jtaguart_tx_space(port, &status)) {
		uart_port_unlock_irqrestore(port, flags);

		if ((status & ALTERA_JTAGUART_CONTROL_AC_MSK) == 0) {
			return;	/* no connection activity */
		}

		cpu_relax();
		uart_port_lock_irqsave(port, &flags);
	}
	writel(c, port->membase + ALTERA_JTAGUART_DATA_REG);
	uart_port_unlock_irqrestore(port, flags);
}
#else
static void altera_jtaguart_console_putc(struct uart_port *port, unsigned char c)
{
	unsigned long flags;

	uart_port_lock_irqsave(port, &flags);
	while (!altera_jtaguart_tx_space(port, NULL)) {
		uart_port_unlock_irqrestore(port, flags);
		cpu_relax();
		uart_port_lock_irqsave(port, &flags);
	}
	writel(c, port->membase + ALTERA_JTAGUART_DATA_REG);
	uart_port_unlock_irqrestore(port, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
#endif

static void altera_jtaguart_console_write(struct console *co, const char *s,
					  unsigned int count)
{
<<<<<<< HEAD
	for (; count; count--, s++) {
		altera_jtaguart_console_putc(co, *s);
		if (*s == '\n')
			altera_jtaguart_console_putc(co, '\r');
	}
=======
	struct uart_port *port = &altera_jtaguart_ports[co->index];

	uart_console_write(port, s, count, altera_jtaguart_console_putc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int __init altera_jtaguart_console_setup(struct console *co,
						char *options)
{
	struct uart_port *port;

	if (co->index < 0 || co->index >= ALTERA_JTAGUART_MAXPORTS)
		return -EINVAL;
<<<<<<< HEAD
	port = &altera_jtaguart_ports[co->index].port;
=======
	port = &altera_jtaguart_ports[co->index];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (port->membase == NULL)
		return -ENODEV;
	return 0;
}

static struct uart_driver altera_jtaguart_driver;

static struct console altera_jtaguart_console = {
	.name	= "ttyJ",
	.write	= altera_jtaguart_console_write,
	.device	= uart_console_device,
	.setup	= altera_jtaguart_console_setup,
	.flags	= CON_PRINTBUFFER,
	.index	= -1,
	.data	= &altera_jtaguart_driver,
};

static int __init altera_jtaguart_console_init(void)
{
	register_console(&altera_jtaguart_console);
	return 0;
}

console_initcall(altera_jtaguart_console_init);

#define	ALTERA_JTAGUART_CONSOLE	(&altera_jtaguart_console)

<<<<<<< HEAD
=======
static void altera_jtaguart_earlycon_write(struct console *co, const char *s,
					   unsigned int count)
{
	struct earlycon_device *dev = co->data;

	uart_console_write(&dev->port, s, count, altera_jtaguart_console_putc);
}

static int __init altera_jtaguart_earlycon_setup(struct earlycon_device *dev,
						 const char *options)
{
	if (!dev->port.membase)
		return -ENODEV;

	dev->con->write = altera_jtaguart_earlycon_write;
	return 0;
}

OF_EARLYCON_DECLARE(juart, "altr,juart-1.0", altera_jtaguart_earlycon_setup);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#else

#define	ALTERA_JTAGUART_CONSOLE	NULL

<<<<<<< HEAD
#endif /* CONFIG_ALTERA_JTAGUART_CONSOLE */
=======
#endif /* CONFIG_SERIAL_ALTERA_JTAGUART_CONSOLE */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static struct uart_driver altera_jtaguart_driver = {
	.owner		= THIS_MODULE,
	.driver_name	= "altera_jtaguart",
	.dev_name	= "ttyJ",
	.major		= ALTERA_JTAGUART_MAJOR,
	.minor		= ALTERA_JTAGUART_MINOR,
	.nr		= ALTERA_JTAGUART_MAXPORTS,
	.cons		= ALTERA_JTAGUART_CONSOLE,
};

<<<<<<< HEAD
static int __devinit altera_jtaguart_probe(struct platform_device *pdev)
{
	struct altera_jtaguart_platform_uart *platp = pdev->dev.platform_data;
	struct uart_port *port;
	struct resource *res_irq, *res_mem;
	int i = pdev->id;
=======
static int altera_jtaguart_probe(struct platform_device *pdev)
{
	struct altera_jtaguart_platform_uart *platp =
			dev_get_platdata(&pdev->dev);
	struct uart_port *port;
	struct resource *res_mem;
	int i = pdev->id;
	int irq;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* -1 emphasizes that the platform must have one port, no .N suffix */
	if (i == -1)
		i = 0;

	if (i >= ALTERA_JTAGUART_MAXPORTS)
		return -EINVAL;

<<<<<<< HEAD
	port = &altera_jtaguart_ports[i].port;
=======
	port = &altera_jtaguart_ports[i];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	res_mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (res_mem)
		port->mapbase = res_mem->start;
	else if (platp)
		port->mapbase = platp->mapbase;
	else
		return -ENODEV;

<<<<<<< HEAD
	res_irq = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	if (res_irq)
		port->irq = res_irq->start;
=======
	irq = platform_get_irq_optional(pdev, 0);
	if (irq < 0 && irq != -ENXIO)
		return irq;
	if (irq > 0)
		port->irq = irq;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	else if (platp)
		port->irq = platp->irq;
	else
		return -ENODEV;

	port->membase = ioremap(port->mapbase, ALTERA_JTAGUART_SIZE);
	if (!port->membase)
		return -ENOMEM;

	port->line = i;
	port->type = PORT_ALTERA_JTAGUART;
	port->iotype = SERIAL_IO_MEM;
	port->ops = &altera_jtaguart_ops;
	port->flags = UPF_BOOT_AUTOCONF;
<<<<<<< HEAD
=======
	port->dev = &pdev->dev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	uart_add_one_port(&altera_jtaguart_driver, port);

	return 0;
}

<<<<<<< HEAD
static int __devexit altera_jtaguart_remove(struct platform_device *pdev)
=======
static void altera_jtaguart_remove(struct platform_device *pdev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct uart_port *port;
	int i = pdev->id;

	if (i == -1)
		i = 0;

<<<<<<< HEAD
	port = &altera_jtaguart_ports[i].port;
	uart_remove_one_port(&altera_jtaguart_driver, port);

	return 0;
}

#ifdef CONFIG_OF
static struct of_device_id altera_jtaguart_match[] = {
	{ .compatible = "ALTR,juart-1.0", },
=======
	port = &altera_jtaguart_ports[i];
	uart_remove_one_port(&altera_jtaguart_driver, port);
	iounmap(port->membase);
}

#ifdef CONFIG_OF
static const struct of_device_id altera_jtaguart_match[] = {
	{ .compatible = "ALTR,juart-1.0", },
	{ .compatible = "altr,juart-1.0", },
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{},
};
MODULE_DEVICE_TABLE(of, altera_jtaguart_match);
#endif /* CONFIG_OF */

static struct platform_driver altera_jtaguart_platform_driver = {
	.probe	= altera_jtaguart_probe,
<<<<<<< HEAD
	.remove	= __devexit_p(altera_jtaguart_remove),
	.driver	= {
		.name		= DRV_NAME,
		.owner		= THIS_MODULE,
=======
	.remove_new = altera_jtaguart_remove,
	.driver	= {
		.name		= DRV_NAME,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.of_match_table	= of_match_ptr(altera_jtaguart_match),
	},
};

static int __init altera_jtaguart_init(void)
{
	int rc;

	rc = uart_register_driver(&altera_jtaguart_driver);
	if (rc)
		return rc;
	rc = platform_driver_register(&altera_jtaguart_platform_driver);
<<<<<<< HEAD
	if (rc) {
		uart_unregister_driver(&altera_jtaguart_driver);
		return rc;
	}
	return 0;
=======
	if (rc)
		uart_unregister_driver(&altera_jtaguart_driver);
	return rc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void __exit altera_jtaguart_exit(void)
{
	platform_driver_unregister(&altera_jtaguart_platform_driver);
	uart_unregister_driver(&altera_jtaguart_driver);
}

module_init(altera_jtaguart_init);
module_exit(altera_jtaguart_exit);

MODULE_DESCRIPTION("Altera JTAG UART driver");
MODULE_AUTHOR("Thomas Chou <thomas@wytron.com.tw>");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:" DRV_NAME);
