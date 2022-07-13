<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * USB Debug cable driver
 *
 * Copyright (C) 2006 Greg Kroah-Hartman <greg@kroah.com>
<<<<<<< HEAD
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License version
 *	2 as published by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/gfp.h>
#include <linux/kernel.h>
<<<<<<< HEAD
#include <linux/init.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/tty.h>
#include <linux/module.h>
#include <linux/usb.h>
#include <linux/usb/serial.h>

#define USB_DEBUG_MAX_PACKET_SIZE	8
#define USB_DEBUG_BRK_SIZE		8
<<<<<<< HEAD
static char USB_DEBUG_BRK[USB_DEBUG_BRK_SIZE] = {
=======
static const char USB_DEBUG_BRK[USB_DEBUG_BRK_SIZE] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	0x00,
	0xff,
	0x01,
	0xfe,
	0x00,
	0xfe,
	0x01,
	0xff,
};

static const struct usb_device_id id_table[] = {
	{ USB_DEVICE(0x0525, 0x127a) },
	{ },
};
<<<<<<< HEAD
MODULE_DEVICE_TABLE(usb, id_table);

static struct usb_driver debug_driver = {
	.name =		"debug",
	.probe =	usb_serial_probe,
	.disconnect =	usb_serial_disconnect,
	.id_table =	id_table,
};

/* This HW really does not support a serial break, so one will be
 * emulated when ever the break state is set to true.
 */
static void usb_debug_break_ctl(struct tty_struct *tty, int break_state)
{
	struct usb_serial_port *port = tty->driver_data;
	if (!break_state)
		return;
	usb_serial_generic_write(tty, port, USB_DEBUG_BRK, USB_DEBUG_BRK_SIZE);
=======

static const struct usb_device_id dbc_id_table[] = {
	{ USB_DEVICE(0x1d6b, 0x0010) },
	{ USB_DEVICE(0x1d6b, 0x0011) },
	{ },
};

static const struct usb_device_id id_table_combined[] = {
	{ USB_DEVICE(0x0525, 0x127a) },
	{ USB_DEVICE(0x1d6b, 0x0010) },
	{ USB_DEVICE(0x1d6b, 0x0011) },
	{ },
};
MODULE_DEVICE_TABLE(usb, id_table_combined);

/* This HW really does not support a serial break, so one will be
 * emulated when ever the break state is set to true.
 */
static int usb_debug_break_ctl(struct tty_struct *tty, int break_state)
{
	struct usb_serial_port *port = tty->driver_data;
	int ret;

	if (!break_state)
		return 0;

	ret = usb_serial_generic_write(tty, port, USB_DEBUG_BRK, USB_DEBUG_BRK_SIZE);
	if (ret < 0)
		return ret;

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void usb_debug_process_read_urb(struct urb *urb)
{
	struct usb_serial_port *port = urb->context;

	if (urb->actual_length == USB_DEBUG_BRK_SIZE &&
		memcmp(urb->transfer_buffer, USB_DEBUG_BRK,
						USB_DEBUG_BRK_SIZE) == 0) {
		usb_serial_handle_break(port);
		return;
	}

	usb_serial_generic_process_read_urb(urb);
}

static struct usb_serial_driver debug_device = {
	.driver = {
		.owner =	THIS_MODULE,
		.name =		"debug",
	},
	.id_table =		id_table,
	.num_ports =		1,
	.bulk_out_size =	USB_DEBUG_MAX_PACKET_SIZE,
	.break_ctl =		usb_debug_break_ctl,
	.process_read_urb =	usb_debug_process_read_urb,
};

<<<<<<< HEAD
static struct usb_serial_driver * const serial_drivers[] = {
	&debug_device, NULL
};

module_usb_serial_driver(debug_driver, serial_drivers);
MODULE_LICENSE("GPL");
=======
static struct usb_serial_driver dbc_device = {
	.driver = {
		.owner =	THIS_MODULE,
		.name =		"xhci_dbc",
	},
	.id_table =		dbc_id_table,
	.num_ports =		1,
	.break_ctl =		usb_debug_break_ctl,
	.process_read_urb =	usb_debug_process_read_urb,
};

static struct usb_serial_driver * const serial_drivers[] = {
	&debug_device, &dbc_device, NULL
};

module_usb_serial_driver(serial_drivers, id_table_combined);
MODULE_LICENSE("GPL v2");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
