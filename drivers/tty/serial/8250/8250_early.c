<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Early serial console for 8250/16550 devices
 *
 * (c) Copyright 2004 Hewlett-Packard Development Company, L.P.
 *	Bjorn Helgaas <bjorn.helgaas@hp.com>
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Based on the 8250.c serial driver, Copyright (C) 2001 Russell King,
 * and on early_printk.c by Andi Kleen.
 *
 * This is for use before the serial driver has initialized, in
 * particular, before the UARTs have been discovered and named.
 * Instead of specifying the console device as, e.g., "ttyS0",
 * we locate the device directly by its MMIO or I/O port address.
 *
 * The user can specify the device directly, e.g.,
 *	earlycon=uart8250,io,0x3f8,9600n8
 *	earlycon=uart8250,mmio,0xff5e0000,115200n8
 *	earlycon=uart8250,mmio32,0xff5e0000,115200n8
 * or
 *	console=uart8250,io,0x3f8,9600n8
 *	console=uart8250,mmio,0xff5e0000,115200n8
 *	console=uart8250,mmio32,0xff5e0000,115200n8
 */

#include <linux/tty.h>
#include <linux/init.h>
#include <linux/console.h>
<<<<<<< HEAD
#include <linux/serial_core.h>
=======
#include <linux/of.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/serial_reg.h>
#include <linux/serial.h>
#include <linux/serial_8250.h>
#include <asm/io.h>
#include <asm/serial.h>
<<<<<<< HEAD
#ifdef CONFIG_FIX_EARLYCON_MEM
#include <asm/pgtable.h>
#include <asm/fixmap.h>
#endif

struct early_serial8250_device {
	struct uart_port port;
	char options[16];		/* e.g., 115200n8 */
	unsigned int baud;
};

static struct early_serial8250_device early_device;

static unsigned int __init serial_in(struct uart_port *port, int offset)
{
	switch (port->iotype) {
	case UPIO_MEM:
		return readb(port->membase + offset);
	case UPIO_MEM32:
		return readl(port->membase + (offset << 2));
=======

static unsigned int serial8250_early_in(struct uart_port *port, int offset)
{
	offset <<= port->regshift;

	switch (port->iotype) {
	case UPIO_MEM:
		return readb(port->membase + offset);
	case UPIO_MEM16:
		return readw(port->membase + offset);
	case UPIO_MEM32:
		return readl(port->membase + offset);
	case UPIO_MEM32BE:
		return ioread32be(port->membase + offset);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case UPIO_PORT:
		return inb(port->iobase + offset);
	default:
		return 0;
	}
}

<<<<<<< HEAD
static void __init serial_out(struct uart_port *port, int offset, int value)
{
=======
static void serial8250_early_out(struct uart_port *port, int offset, int value)
{
	offset <<= port->regshift;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	switch (port->iotype) {
	case UPIO_MEM:
		writeb(value, port->membase + offset);
		break;
<<<<<<< HEAD
	case UPIO_MEM32:
		writel(value, port->membase + (offset << 2));
=======
	case UPIO_MEM16:
		writew(value, port->membase + offset);
		break;
	case UPIO_MEM32:
		writel(value, port->membase + offset);
		break;
	case UPIO_MEM32BE:
		iowrite32be(value, port->membase + offset);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case UPIO_PORT:
		outb(value, port->iobase + offset);
		break;
	}
}

<<<<<<< HEAD
#define BOTH_EMPTY (UART_LSR_TEMT | UART_LSR_THRE)

static void __init wait_for_xmitr(struct uart_port *port)
{
	unsigned int status;

	for (;;) {
		status = serial_in(port, UART_LSR);
		if ((status & BOTH_EMPTY) == BOTH_EMPTY)
			return;
=======
static void serial_putc(struct uart_port *port, unsigned char c)
{
	unsigned int status;

	serial8250_early_out(port, UART_TX, c);

	for (;;) {
		status = serial8250_early_in(port, UART_LSR);
		if (uart_lsr_tx_empty(status))
			break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		cpu_relax();
	}
}

<<<<<<< HEAD
static void __init serial_putc(struct uart_port *port, int c)
{
	wait_for_xmitr(port);
	serial_out(port, UART_TX, c);
}

static void __init early_serial8250_write(struct console *console,
					const char *s, unsigned int count)
{
	struct uart_port *port = &early_device.port;
	unsigned int ier;

	/* Save the IER and disable interrupts */
	ier = serial_in(port, UART_IER);
	serial_out(port, UART_IER, 0);

	uart_console_write(port, s, count, serial_putc);

	/* Wait for transmitter to become empty and restore the IER */
	wait_for_xmitr(port);
	serial_out(port, UART_IER, ier);
}

static unsigned int __init probe_baud(struct uart_port *port)
{
	unsigned char lcr, dll, dlm;
	unsigned int quot;

	lcr = serial_in(port, UART_LCR);
	serial_out(port, UART_LCR, lcr | UART_LCR_DLAB);
	dll = serial_in(port, UART_DLL);
	dlm = serial_in(port, UART_DLM);
	serial_out(port, UART_LCR, lcr);

	quot = (dlm << 8) | dll;
	return (port->uartclk / 16) / quot;
}

static void __init init_port(struct early_serial8250_device *device)
=======
static void early_serial8250_write(struct console *console,
					const char *s, unsigned int count)
{
	struct earlycon_device *device = console->data;
	struct uart_port *port = &device->port;

	uart_console_write(port, s, count, serial_putc);
}

#ifdef CONFIG_CONSOLE_POLL
static int early_serial8250_read(struct console *console,
				 char *s, unsigned int count)
{
	struct earlycon_device *device = console->data;
	struct uart_port *port = &device->port;
	unsigned int status;
	int num_read = 0;

	while (num_read < count) {
		status = serial8250_early_in(port, UART_LSR);
		if (!(status & UART_LSR_DR))
			break;
		s[num_read++] = serial8250_early_in(port, UART_RX);
	}

	return num_read;
}
#else
#define early_serial8250_read NULL
#endif

static void __init init_port(struct earlycon_device *device)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct uart_port *port = &device->port;
	unsigned int divisor;
	unsigned char c;
<<<<<<< HEAD

	serial_out(port, UART_LCR, 0x3);	/* 8n1 */
	serial_out(port, UART_IER, 0);		/* no interrupt */
	serial_out(port, UART_FCR, 0);		/* no fifo */
	serial_out(port, UART_MCR, 0x3);	/* DTR + RTS */

	divisor = port->uartclk / (16 * device->baud);
	c = serial_in(port, UART_LCR);
	serial_out(port, UART_LCR, c | UART_LCR_DLAB);
	serial_out(port, UART_DLL, divisor & 0xff);
	serial_out(port, UART_DLM, (divisor >> 8) & 0xff);
	serial_out(port, UART_LCR, c & ~UART_LCR_DLAB);
}

static int __init parse_options(struct early_serial8250_device *device,
								char *options)
{
	struct uart_port *port = &device->port;
	int mmio, mmio32, length;

	if (!options)
		return -ENODEV;

	port->uartclk = BASE_BAUD * 16;

	mmio = !strncmp(options, "mmio,", 5);
	mmio32 = !strncmp(options, "mmio32,", 7);
	if (mmio || mmio32) {
		port->iotype = (mmio ? UPIO_MEM : UPIO_MEM32);
		port->mapbase = simple_strtoul(options + (mmio ? 5 : 7),
					       &options, 0);
		if (mmio32)
			port->regshift = 2;
#ifdef CONFIG_FIX_EARLYCON_MEM
		set_fixmap_nocache(FIX_EARLYCON_MEM_BASE,
					port->mapbase & PAGE_MASK);
		port->membase =
			(void __iomem *)__fix_to_virt(FIX_EARLYCON_MEM_BASE);
		port->membase += port->mapbase & ~PAGE_MASK;
#else
		port->membase = ioremap_nocache(port->mapbase, 64);
		if (!port->membase) {
			printk(KERN_ERR "%s: Couldn't ioremap 0x%llx\n",
				__func__,
			       (unsigned long long) port->mapbase);
			return -ENOMEM;
		}
#endif
	} else if (!strncmp(options, "io,", 3)) {
		port->iotype = UPIO_PORT;
		port->iobase = simple_strtoul(options + 3, &options, 0);
		mmio = 0;
	} else
		return -EINVAL;

	options = strchr(options, ',');
	if (options) {
		options++;
		device->baud = simple_strtoul(options, NULL, 0);
		length = min(strcspn(options, " "), sizeof(device->options));
		strncpy(device->options, options, length);
	} else {
		device->baud = probe_baud(port);
		snprintf(device->options, sizeof(device->options), "%u",
			device->baud);
	}

	if (mmio || mmio32)
		printk(KERN_INFO
		       "Early serial console at MMIO%s 0x%llx (options '%s')\n",
			mmio32 ? "32" : "",
			(unsigned long long)port->mapbase,
			device->options);
	else
		printk(KERN_INFO
		      "Early serial console at I/O port 0x%lx (options '%s')\n",
			port->iobase,
			device->options);

	return 0;
}

static struct console early_serial8250_console __initdata = {
	.name	= "uart",
	.write	= early_serial8250_write,
	.flags	= CON_PRINTBUFFER | CON_BOOT,
	.index	= -1,
};

static int __init early_serial8250_setup(char *options)
{
	struct early_serial8250_device *device = &early_device;
	int err;

	if (device->port.membase || device->port.iobase)
		return 0;

	err = parse_options(device, options);
	if (err < 0)
		return err;

	init_port(device);
	return 0;
}

int __init setup_early_serial8250_console(char *cmdline)
{
	char *options;
	int err;

	options = strstr(cmdline, "uart8250,");
	if (!options) {
		options = strstr(cmdline, "uart,");
		if (!options)
			return 0;
	}

	options = strchr(cmdline, ',') + 1;
	err = early_serial8250_setup(options);
	if (err < 0)
		return err;

	register_console(&early_serial8250_console);

	return 0;
}

int serial8250_find_port_for_earlycon(void)
{
	struct early_serial8250_device *device = &early_device;
	struct uart_port *port = &device->port;
	int line;
	int ret;

	if (!device->port.membase && !device->port.iobase)
		return -ENODEV;

	line = serial8250_find_port(port);
	if (line < 0)
		return -ENODEV;

	ret = update_console_cmdline("uart", 8250,
			     "ttyS", line, device->options);
	if (ret < 0)
		ret = update_console_cmdline("uart", 0,
				     "ttyS", line, device->options);

	return ret;
}

early_param("earlycon", setup_early_serial8250_console);
=======
	unsigned int ier;

	serial8250_early_out(port, UART_LCR, UART_LCR_WLEN8);		/* 8n1 */
	ier = serial8250_early_in(port, UART_IER);
	serial8250_early_out(port, UART_IER, ier & UART_IER_UUE); /* no interrupt */
	serial8250_early_out(port, UART_FCR, 0);	/* no fifo */
	serial8250_early_out(port, UART_MCR, UART_MCR_DTR | UART_MCR_RTS);

	if (port->uartclk) {
		divisor = DIV_ROUND_CLOSEST(port->uartclk, 16 * device->baud);
		c = serial8250_early_in(port, UART_LCR);
		serial8250_early_out(port, UART_LCR, c | UART_LCR_DLAB);
		serial8250_early_out(port, UART_DLL, divisor & 0xff);
		serial8250_early_out(port, UART_DLM, (divisor >> 8) & 0xff);
		serial8250_early_out(port, UART_LCR, c & ~UART_LCR_DLAB);
	}
}

int __init early_serial8250_setup(struct earlycon_device *device,
					 const char *options)
{
	if (!(device->port.membase || device->port.iobase))
		return -ENODEV;

	if (!device->baud) {
		struct uart_port *port = &device->port;
		unsigned int ier;

		/* assume the device was initialized, only mask interrupts */
		ier = serial8250_early_in(port, UART_IER);
		serial8250_early_out(port, UART_IER, ier & UART_IER_UUE);
	} else
		init_port(device);

	device->con->write = early_serial8250_write;
	device->con->read = early_serial8250_read;
	return 0;
}
EARLYCON_DECLARE(uart8250, early_serial8250_setup);
EARLYCON_DECLARE(uart, early_serial8250_setup);
OF_EARLYCON_DECLARE(ns16550, "ns16550", early_serial8250_setup);
OF_EARLYCON_DECLARE(ns16550a, "ns16550a", early_serial8250_setup);
OF_EARLYCON_DECLARE(uart, "nvidia,tegra20-uart", early_serial8250_setup);
OF_EARLYCON_DECLARE(uart, "snps,dw-apb-uart", early_serial8250_setup);

#ifdef CONFIG_SERIAL_8250_OMAP

static int __init early_omap8250_setup(struct earlycon_device *device,
				       const char *options)
{
	struct uart_port *port = &device->port;

	if (!(device->port.membase || device->port.iobase))
		return -ENODEV;

	port->regshift = 2;
	device->con->write = early_serial8250_write;
	return 0;
}

OF_EARLYCON_DECLARE(omap8250, "ti,omap2-uart", early_omap8250_setup);
OF_EARLYCON_DECLARE(omap8250, "ti,omap3-uart", early_omap8250_setup);
OF_EARLYCON_DECLARE(omap8250, "ti,omap4-uart", early_omap8250_setup);
OF_EARLYCON_DECLARE(omap8250, "ti,am654-uart", early_omap8250_setup);

#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
