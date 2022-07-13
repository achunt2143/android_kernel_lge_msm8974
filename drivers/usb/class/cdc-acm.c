<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0+
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * cdc-acm.c
 *
 * Copyright (c) 1999 Armin Fuerst	<fuerst@in.tum.de>
 * Copyright (c) 1999 Pavel Machek	<pavel@ucw.cz>
 * Copyright (c) 1999 Johannes Erdfelt	<johannes@erdfelt.com>
 * Copyright (c) 2000 Vojtech Pavlik	<vojtech@suse.cz>
 * Copyright (c) 2004 Oliver Neukum	<oliver@neukum.name>
 * Copyright (c) 2005 David Kubicek	<dave@awk.cz>
 * Copyright (c) 2011 Johan Hovold	<jhovold@gmail.com>
 *
 * USB Abstract Control Model driver for USB modems and ISDN adapters
 *
 * Sponsored by SuSE
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#undef DEBUG
#undef VERBOSE_DEBUG

#include <linux/kernel.h>
<<<<<<< HEAD
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/slab.h>
=======
#include <linux/sched/signal.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/log2.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/tty.h>
#include <linux/serial.h>
#include <linux/tty_driver.h>
#include <linux/tty_flip.h>
<<<<<<< HEAD
#include <linux/serial.h>
=======
#include <linux/tty_ldisc.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/uaccess.h>
#include <linux/usb.h>
#include <linux/usb/cdc.h>
#include <asm/byteorder.h>
#include <asm/unaligned.h>
<<<<<<< HEAD
=======
#include <linux/idr.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/list.h>

#include "cdc-acm.h"


#define DRIVER_AUTHOR "Armin Fuerst, Pavel Machek, Johannes Erdfelt, Vojtech Pavlik, David Kubicek, Johan Hovold"
#define DRIVER_DESC "USB Abstract Control Model driver for USB modems and ISDN adapters"

static struct usb_driver acm_driver;
static struct tty_driver *acm_tty_driver;
<<<<<<< HEAD
static struct acm *acm_table[ACM_TTY_MINORS];

static DEFINE_MUTEX(acm_table_lock);

/*
 * acm_table accessors
 */

/*
 * Look up an ACM structure by index. If found and not disconnected, increment
 * its refcount and return it with its mutex held.
 */
static struct acm *acm_get_by_index(unsigned index)
{
	struct acm *acm;

	mutex_lock(&acm_table_lock);
	acm = acm_table[index];
=======

static DEFINE_IDR(acm_minors);
static DEFINE_MUTEX(acm_minors_lock);

static void acm_tty_set_termios(struct tty_struct *tty,
				const struct ktermios *termios_old);

/*
 * acm_minors accessors
 */

/*
 * Look up an ACM structure by minor. If found and not disconnected, increment
 * its refcount and return it with its mutex held.
 */
static struct acm *acm_get_by_minor(unsigned int minor)
{
	struct acm *acm;

	mutex_lock(&acm_minors_lock);
	acm = idr_find(&acm_minors, minor);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (acm) {
		mutex_lock(&acm->mutex);
		if (acm->disconnected) {
			mutex_unlock(&acm->mutex);
			acm = NULL;
		} else {
			tty_port_get(&acm->port);
			mutex_unlock(&acm->mutex);
		}
	}
<<<<<<< HEAD
	mutex_unlock(&acm_table_lock);
=======
	mutex_unlock(&acm_minors_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return acm;
}

/*
 * Try to find an available minor number and if found, associate it with 'acm'.
 */
static int acm_alloc_minor(struct acm *acm)
{
	int minor;

<<<<<<< HEAD
	mutex_lock(&acm_table_lock);
	for (minor = 0; minor < ACM_TTY_MINORS; minor++) {
		if (!acm_table[minor]) {
			acm_table[minor] = acm;
			break;
		}
	}
	mutex_unlock(&acm_table_lock);
=======
	mutex_lock(&acm_minors_lock);
	minor = idr_alloc(&acm_minors, acm, 0, ACM_TTY_MINORS, GFP_KERNEL);
	mutex_unlock(&acm_minors_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return minor;
}

/* Release the minor number associated with 'acm'.  */
static void acm_release_minor(struct acm *acm)
{
<<<<<<< HEAD
	mutex_lock(&acm_table_lock);
	acm_table[acm->minor] = NULL;
	mutex_unlock(&acm_table_lock);
=======
	mutex_lock(&acm_minors_lock);
	idr_remove(&acm_minors, acm->minor);
	mutex_unlock(&acm_minors_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Functions for ACM control messages.
 */

static int acm_ctrl_msg(struct acm *acm, int request, int value,
							void *buf, int len)
{
	int retval;

	retval = usb_autopm_get_interface(acm->control);
	if (retval)
		return retval;

	retval = usb_control_msg(acm->dev, usb_sndctrlpipe(acm->dev, 0),
		request, USB_RT_ACM, value,
		acm->control->altsetting[0].desc.bInterfaceNumber,
<<<<<<< HEAD
		buf, len, 5000);

	dev_dbg(&acm->control->dev,
			"%s - rq 0x%02x, val %#x, len %#x, result %d\n",
			__func__, request, value, len, retval);
=======
		buf, len, USB_CTRL_SET_TIMEOUT);

	dev_dbg(&acm->control->dev,
		"%s - rq 0x%02x, val %#x, len %#x, result %d\n",
		__func__, request, value, len, retval);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	usb_autopm_put_interface(acm->control);

	return retval < 0 ? retval : 0;
}

/* devices aren't required to support these requests.
 * the cdc acm descriptor tells whether they do...
 */
<<<<<<< HEAD
#define acm_set_control(acm, control) \
	acm_ctrl_msg(acm, USB_CDC_REQ_SET_CONTROL_LINE_STATE, control, NULL, 0)
=======
static inline int acm_set_control(struct acm *acm, int control)
{
	if (acm->quirks & QUIRK_CONTROL_LINE_STATE)
		return -EOPNOTSUPP;

	return acm_ctrl_msg(acm, USB_CDC_REQ_SET_CONTROL_LINE_STATE,
			control, NULL, 0);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define acm_set_line(acm, line) \
	acm_ctrl_msg(acm, USB_CDC_REQ_SET_LINE_CODING, 0, line, sizeof *(line))
#define acm_send_break(acm, ms) \
	acm_ctrl_msg(acm, USB_CDC_REQ_SEND_BREAK, ms, NULL, 0)

<<<<<<< HEAD
=======
static void acm_poison_urbs(struct acm *acm)
{
	int i;

	usb_poison_urb(acm->ctrlurb);
	for (i = 0; i < ACM_NW; i++)
		usb_poison_urb(acm->wb[i].urb);
	for (i = 0; i < acm->rx_buflimit; i++)
		usb_poison_urb(acm->read_urbs[i]);
}

static void acm_unpoison_urbs(struct acm *acm)
{
	int i;

	for (i = 0; i < acm->rx_buflimit; i++)
		usb_unpoison_urb(acm->read_urbs[i]);
	for (i = 0; i < ACM_NW; i++)
		usb_unpoison_urb(acm->wb[i].urb);
	usb_unpoison_urb(acm->ctrlurb);
}


>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Write buffer management.
 * All of these assume proper locks taken by the caller.
 */

static int acm_wb_alloc(struct acm *acm)
{
	int i, wbn;
	struct acm_wb *wb;

	wbn = 0;
	i = 0;
	for (;;) {
		wb = &acm->wb[wbn];
		if (!wb->use) {
<<<<<<< HEAD
			wb->use = 1;
=======
			wb->use = true;
			wb->len = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return wbn;
		}
		wbn = (wbn + 1) % ACM_NW;
		if (++i >= ACM_NW)
			return -1;
	}
}

static int acm_wb_is_avail(struct acm *acm)
{
	int i, n;
	unsigned long flags;

	n = ACM_NW;
	spin_lock_irqsave(&acm->write_lock, flags);
	for (i = 0; i < ACM_NW; i++)
<<<<<<< HEAD
		n -= acm->wb[i].use;
=======
		if(acm->wb[i].use)
			n--;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_unlock_irqrestore(&acm->write_lock, flags);
	return n;
}

/*
 * Finish write. Caller must hold acm->write_lock
 */
static void acm_write_done(struct acm *acm, struct acm_wb *wb)
{
<<<<<<< HEAD
	wb->use = 0;
=======
	wb->use = false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	acm->transmitting--;
	usb_autopm_put_interface_async(acm->control);
}

/*
 * Poke write.
 *
 * the caller is responsible for locking
 */

static int acm_start_wb(struct acm *acm, struct acm_wb *wb)
{
	int rc;

	acm->transmitting++;

	wb->urb->transfer_buffer = wb->buf;
	wb->urb->transfer_dma = wb->dmah;
	wb->urb->transfer_buffer_length = wb->len;
	wb->urb->dev = acm->dev;

	rc = usb_submit_urb(wb->urb, GFP_ATOMIC);
	if (rc < 0) {
<<<<<<< HEAD
		dev_err(&acm->data->dev,
			"%s - usb_submit_urb(write bulk) failed: %d\n",
			__func__, rc);
=======
		if (rc != -EPERM)
			dev_err(&acm->data->dev,
				"%s - usb_submit_urb(write bulk) failed: %d\n",
				__func__, rc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		acm_write_done(acm, wb);
	}
	return rc;
}

<<<<<<< HEAD
static int acm_write_start(struct acm *acm, int wbn)
{
	unsigned long flags;
	struct acm_wb *wb = &acm->wb[wbn];
	int rc;

	spin_lock_irqsave(&acm->write_lock, flags);
	if (!acm->dev) {
		wb->use = 0;
		spin_unlock_irqrestore(&acm->write_lock, flags);
		return -ENODEV;
	}

	dev_vdbg(&acm->data->dev, "%s - susp_count %d\n", __func__,
							acm->susp_count);
	usb_autopm_get_interface_async(acm->control);
	if (acm->susp_count) {
		usb_anchor_urb(wb->urb, &acm->delayed);
		spin_unlock_irqrestore(&acm->write_lock, flags);
		return 0;
	}
	usb_mark_last_busy(acm->dev);

	rc = acm_start_wb(acm, wb);
	spin_unlock_irqrestore(&acm->write_lock, flags);

	return rc;

}
/*
 * attributes exported through sysfs
 */
static ssize_t show_caps
=======
/*
 * attributes exported through sysfs
 */
static ssize_t bmCapabilities_show
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct usb_interface *intf = to_usb_interface(dev);
	struct acm *acm = usb_get_intfdata(intf);

	return sprintf(buf, "%d", acm->ctrl_caps);
}
<<<<<<< HEAD
static DEVICE_ATTR(bmCapabilities, S_IRUGO, show_caps, NULL);

static ssize_t show_country_codes
=======
static DEVICE_ATTR_RO(bmCapabilities);

static ssize_t wCountryCodes_show
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct usb_interface *intf = to_usb_interface(dev);
	struct acm *acm = usb_get_intfdata(intf);

	memcpy(buf, acm->country_codes, acm->country_code_size);
	return acm->country_code_size;
}

<<<<<<< HEAD
static DEVICE_ATTR(wCountryCodes, S_IRUGO, show_country_codes, NULL);

static ssize_t show_country_rel_date
=======
static DEVICE_ATTR_RO(wCountryCodes);

static ssize_t iCountryCodeRelDate_show
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct usb_interface *intf = to_usb_interface(dev);
	struct acm *acm = usb_get_intfdata(intf);

	return sprintf(buf, "%d", acm->country_rel_date);
}

<<<<<<< HEAD
static DEVICE_ATTR(iCountryCodeRelDate, S_IRUGO, show_country_rel_date, NULL);
=======
static DEVICE_ATTR_RO(iCountryCodeRelDate);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Interrupt handlers for various ACM device responses
 */

<<<<<<< HEAD
=======
static void acm_process_notification(struct acm *acm, unsigned char *buf)
{
	int newctrl;
	int difference;
	unsigned long flags;
	struct usb_cdc_notification *dr = (struct usb_cdc_notification *)buf;
	unsigned char *data = buf + sizeof(struct usb_cdc_notification);

	switch (dr->bNotificationType) {
	case USB_CDC_NOTIFY_NETWORK_CONNECTION:
		dev_dbg(&acm->control->dev,
			"%s - network connection: %d\n", __func__, dr->wValue);
		break;

	case USB_CDC_NOTIFY_SERIAL_STATE:
		if (le16_to_cpu(dr->wLength) != 2) {
			dev_dbg(&acm->control->dev,
				"%s - malformed serial state\n", __func__);
			break;
		}

		newctrl = get_unaligned_le16(data);
		dev_dbg(&acm->control->dev,
			"%s - serial state: 0x%x\n", __func__, newctrl);

		if (!acm->clocal && (acm->ctrlin & ~newctrl & USB_CDC_SERIAL_STATE_DCD)) {
			dev_dbg(&acm->control->dev,
				"%s - calling hangup\n", __func__);
			tty_port_tty_hangup(&acm->port, false);
		}

		difference = acm->ctrlin ^ newctrl;

		if ((difference & USB_CDC_SERIAL_STATE_DCD) && acm->port.tty) {
			struct tty_ldisc *ld = tty_ldisc_ref(acm->port.tty);
			if (ld) {
				if (ld->ops->dcd_change)
					ld->ops->dcd_change(acm->port.tty, newctrl & USB_CDC_SERIAL_STATE_DCD);
				tty_ldisc_deref(ld);
			}
		}

		spin_lock_irqsave(&acm->read_lock, flags);
		acm->ctrlin = newctrl;
		acm->oldcount = acm->iocount;

		if (difference & USB_CDC_SERIAL_STATE_DSR)
			acm->iocount.dsr++;
		if (difference & USB_CDC_SERIAL_STATE_DCD)
			acm->iocount.dcd++;
		if (newctrl & USB_CDC_SERIAL_STATE_BREAK) {
			acm->iocount.brk++;
			tty_insert_flip_char(&acm->port, 0, TTY_BREAK);
		}
		if (newctrl & USB_CDC_SERIAL_STATE_RING_SIGNAL)
			acm->iocount.rng++;
		if (newctrl & USB_CDC_SERIAL_STATE_FRAMING)
			acm->iocount.frame++;
		if (newctrl & USB_CDC_SERIAL_STATE_PARITY)
			acm->iocount.parity++;
		if (newctrl & USB_CDC_SERIAL_STATE_OVERRUN)
			acm->iocount.overrun++;
		spin_unlock_irqrestore(&acm->read_lock, flags);

		if (newctrl & USB_CDC_SERIAL_STATE_BREAK)
			tty_flip_buffer_push(&acm->port);

		if (difference)
			wake_up_all(&acm->wioctl);

		break;

	default:
		dev_dbg(&acm->control->dev,
			"%s - unknown notification %d received: index %d len %d\n",
			__func__,
			dr->bNotificationType, dr->wIndex, dr->wLength);
	}
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* control interface reports status changes with "interrupt" transfers */
static void acm_ctrl_irq(struct urb *urb)
{
	struct acm *acm = urb->context;
	struct usb_cdc_notification *dr = urb->transfer_buffer;
<<<<<<< HEAD
	struct tty_struct *tty;
	unsigned char *data;
	int newctrl;
=======
	unsigned int current_size = urb->actual_length;
	unsigned int expected_size, copy_size, alloc_size;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int retval;
	int status = urb->status;

	switch (status) {
	case 0:
		/* success */
		break;
	case -ECONNRESET:
	case -ENOENT:
	case -ESHUTDOWN:
		/* this urb is terminated, clean up */
		dev_dbg(&acm->control->dev,
<<<<<<< HEAD
				"%s - urb shutting down with status: %d\n",
				__func__, status);
		return;
	default:
		dev_dbg(&acm->control->dev,
				"%s - nonzero urb status received: %d\n",
				__func__, status);
=======
			"%s - urb shutting down with status: %d\n",
			__func__, status);
		return;
	default:
		dev_dbg(&acm->control->dev,
			"%s - nonzero urb status received: %d\n",
			__func__, status);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto exit;
	}

	usb_mark_last_busy(acm->dev);

<<<<<<< HEAD
	data = (unsigned char *)(dr + 1);
	switch (dr->bNotificationType) {
	case USB_CDC_NOTIFY_NETWORK_CONNECTION:
		dev_dbg(&acm->control->dev, "%s - network connection: %d\n",
							__func__, dr->wValue);
		break;

	case USB_CDC_NOTIFY_SERIAL_STATE:
		tty = tty_port_tty_get(&acm->port);
		newctrl = get_unaligned_le16(data);

		if (tty) {
			if (!acm->clocal &&
				(acm->ctrlin & ~newctrl & ACM_CTRL_DCD)) {
				dev_dbg(&acm->control->dev,
					"%s - calling hangup\n", __func__);
				tty_hangup(tty);
			}
			tty_kref_put(tty);
		}

		acm->ctrlin = newctrl;

		dev_dbg(&acm->control->dev,
			"%s - input control lines: dcd%c dsr%c break%c "
			"ring%c framing%c parity%c overrun%c\n",
			__func__,
			acm->ctrlin & ACM_CTRL_DCD ? '+' : '-',
			acm->ctrlin & ACM_CTRL_DSR ? '+' : '-',
			acm->ctrlin & ACM_CTRL_BRK ? '+' : '-',
			acm->ctrlin & ACM_CTRL_RI  ? '+' : '-',
			acm->ctrlin & ACM_CTRL_FRAMING ? '+' : '-',
			acm->ctrlin & ACM_CTRL_PARITY ? '+' : '-',
			acm->ctrlin & ACM_CTRL_OVERRUN ? '+' : '-');
			break;

	default:
		dev_dbg(&acm->control->dev,
			"%s - unknown notification %d received: index %d "
			"len %d data0 %d data1 %d\n",
			__func__,
			dr->bNotificationType, dr->wIndex,
			dr->wLength, data[0], data[1]);
		break;
	}
exit:
	retval = usb_submit_urb(urb, GFP_ATOMIC);
	if (retval)
		dev_err(&acm->control->dev, "%s - usb_submit_urb failed: %d\n",
							__func__, retval);
=======
	if (acm->nb_index)
		dr = (struct usb_cdc_notification *)acm->notification_buffer;

	/* size = notification-header + (optional) data */
	expected_size = sizeof(struct usb_cdc_notification) +
					le16_to_cpu(dr->wLength);

	if (current_size < expected_size) {
		/* notification is transmitted fragmented, reassemble */
		if (acm->nb_size < expected_size) {
			u8 *new_buffer;
			alloc_size = roundup_pow_of_two(expected_size);
			/* Final freeing is done on disconnect. */
			new_buffer = krealloc(acm->notification_buffer,
					      alloc_size, GFP_ATOMIC);
			if (!new_buffer) {
				acm->nb_index = 0;
				goto exit;
			}

			acm->notification_buffer = new_buffer;
			acm->nb_size = alloc_size;
			dr = (struct usb_cdc_notification *)acm->notification_buffer;
		}

		copy_size = min(current_size,
				expected_size - acm->nb_index);

		memcpy(&acm->notification_buffer[acm->nb_index],
		       urb->transfer_buffer, copy_size);
		acm->nb_index += copy_size;
		current_size = acm->nb_index;
	}

	if (current_size >= expected_size) {
		/* notification complete */
		acm_process_notification(acm, (unsigned char *)dr);
		acm->nb_index = 0;
	}

exit:
	retval = usb_submit_urb(urb, GFP_ATOMIC);
	if (retval && retval != -EPERM && retval != -ENODEV)
		dev_err(&acm->control->dev,
			"%s - usb_submit_urb failed: %d\n", __func__, retval);
	else
		dev_vdbg(&acm->control->dev,
			"control resubmission terminated %d\n", retval);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int acm_submit_read_urb(struct acm *acm, int index, gfp_t mem_flags)
{
	int res;

	if (!test_and_clear_bit(index, &acm->read_urbs_free))
		return 0;

<<<<<<< HEAD
	dev_vdbg(&acm->data->dev, "%s - urb %d\n", __func__, index);

	res = usb_submit_urb(acm->read_urbs[index], mem_flags);
	if (res) {
		if (res != -EPERM) {
			dev_err(&acm->data->dev,
					"%s - usb_submit_urb failed: %d\n",
					__func__, res);
		}
		set_bit(index, &acm->read_urbs_free);
		return res;
=======
	res = usb_submit_urb(acm->read_urbs[index], mem_flags);
	if (res) {
		if (res != -EPERM && res != -ENODEV) {
			dev_err(&acm->data->dev,
				"urb %d failed submission with %d\n",
				index, res);
		} else {
			dev_vdbg(&acm->data->dev, "intended failure %d\n", res);
		}
		set_bit(index, &acm->read_urbs_free);
		return res;
	} else {
		dev_vdbg(&acm->data->dev, "submitted urb %d\n", index);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return 0;
}

static int acm_submit_read_urbs(struct acm *acm, gfp_t mem_flags)
{
	int res;
	int i;

	for (i = 0; i < acm->rx_buflimit; ++i) {
		res = acm_submit_read_urb(acm, i, mem_flags);
		if (res)
			return res;
	}

	return 0;
}

static void acm_process_read_urb(struct acm *acm, struct urb *urb)
{
<<<<<<< HEAD
	struct tty_struct *tty;
=======
	unsigned long flags;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!urb->actual_length)
		return;

<<<<<<< HEAD
	tty = tty_port_tty_get(&acm->port);
	if (!tty)
		return;

	tty_insert_flip_string(tty, urb->transfer_buffer, urb->actual_length);
	tty_flip_buffer_push(tty);

	tty_kref_put(tty);
=======
	spin_lock_irqsave(&acm->read_lock, flags);
	tty_insert_flip_string(&acm->port, urb->transfer_buffer,
			urb->actual_length);
	spin_unlock_irqrestore(&acm->read_lock, flags);

	tty_flip_buffer_push(&acm->port);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void acm_read_bulk_callback(struct urb *urb)
{
	struct acm_rb *rb = urb->context;
	struct acm *acm = rb->instance;
<<<<<<< HEAD
	unsigned long flags;

	dev_vdbg(&acm->data->dev, "%s - urb %d, len %d\n", __func__,
					rb->index, urb->actual_length);
	set_bit(rb->index, &acm->read_urbs_free);

	if (!acm->dev) {
		dev_dbg(&acm->data->dev, "%s - disconnected\n", __func__);
		return;
	}
	usb_mark_last_busy(acm->dev);

	if (urb->status) {
		dev_dbg(&acm->data->dev, "%s - non-zero urb status: %d\n",
							__func__, urb->status);
		return;
	}
	acm_process_read_urb(acm, urb);

	/* throttle device if requested by tty */
	spin_lock_irqsave(&acm->read_lock, flags);
	acm->throttled = acm->throttle_req;
	if (!acm->throttled && !acm->susp_count) {
		spin_unlock_irqrestore(&acm->read_lock, flags);
		acm_submit_read_urb(acm, rb->index, GFP_ATOMIC);
	} else {
		spin_unlock_irqrestore(&acm->read_lock, flags);
	}
=======
	int status = urb->status;
	bool stopped = false;
	bool stalled = false;
	bool cooldown = false;

	dev_vdbg(&acm->data->dev, "got urb %d, len %d, status %d\n",
		rb->index, urb->actual_length, status);

	switch (status) {
	case 0:
		usb_mark_last_busy(acm->dev);
		acm_process_read_urb(acm, urb);
		break;
	case -EPIPE:
		set_bit(EVENT_RX_STALL, &acm->flags);
		stalled = true;
		break;
	case -ENOENT:
	case -ECONNRESET:
	case -ESHUTDOWN:
		dev_dbg(&acm->data->dev,
			"%s - urb shutting down with status: %d\n",
			__func__, status);
		stopped = true;
		break;
	case -EOVERFLOW:
	case -EPROTO:
		dev_dbg(&acm->data->dev,
			"%s - cooling babbling device\n", __func__);
		usb_mark_last_busy(acm->dev);
		set_bit(rb->index, &acm->urbs_in_error_delay);
		set_bit(ACM_ERROR_DELAY, &acm->flags);
		cooldown = true;
		break;
	default:
		dev_dbg(&acm->data->dev,
			"%s - nonzero urb status received: %d\n",
			__func__, status);
		break;
	}

	/*
	 * Make sure URB processing is done before marking as free to avoid
	 * racing with unthrottle() on another CPU. Matches the barriers
	 * implied by the test_and_clear_bit() in acm_submit_read_urb().
	 */
	smp_mb__before_atomic();
	set_bit(rb->index, &acm->read_urbs_free);
	/*
	 * Make sure URB is marked as free before checking the throttled flag
	 * to avoid racing with unthrottle() on another CPU. Matches the
	 * smp_mb() in unthrottle().
	 */
	smp_mb__after_atomic();

	if (stopped || stalled || cooldown) {
		if (stalled)
			schedule_delayed_work(&acm->dwork, 0);
		else if (cooldown)
			schedule_delayed_work(&acm->dwork, HZ / 2);
		return;
	}

	if (test_bit(ACM_THROTTLED, &acm->flags))
		return;

	acm_submit_read_urb(acm, rb->index, GFP_ATOMIC);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* data interface wrote those outgoing bytes */
static void acm_write_bulk(struct urb *urb)
{
	struct acm_wb *wb = urb->context;
	struct acm *acm = wb->instance;
	unsigned long flags;
<<<<<<< HEAD

	if (urb->status	|| (urb->actual_length != urb->transfer_buffer_length))
		dev_vdbg(&acm->data->dev, "%s - len %d/%d, status %d\n",
			__func__,
			urb->actual_length,
			urb->transfer_buffer_length,
			urb->status);
=======
	int status = urb->status;

	if (status || (urb->actual_length != urb->transfer_buffer_length))
		dev_vdbg(&acm->data->dev, "wrote len %d/%d, status %d\n",
			urb->actual_length,
			urb->transfer_buffer_length,
			status);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spin_lock_irqsave(&acm->write_lock, flags);
	acm_write_done(acm, wb);
	spin_unlock_irqrestore(&acm->write_lock, flags);
<<<<<<< HEAD
	schedule_work(&acm->work);
=======
	set_bit(EVENT_TTY_WAKEUP, &acm->flags);
	schedule_delayed_work(&acm->dwork, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void acm_softint(struct work_struct *work)
{
<<<<<<< HEAD
	struct acm *acm = container_of(work, struct acm, work);
	struct tty_struct *tty;

	dev_vdbg(&acm->data->dev, "%s\n", __func__);

	tty = tty_port_tty_get(&acm->port);
	if (!tty)
		return;
	tty_wakeup(tty);
	tty_kref_put(tty);
=======
	int i;
	struct acm *acm = container_of(work, struct acm, dwork.work);

	if (test_bit(EVENT_RX_STALL, &acm->flags)) {
		smp_mb(); /* against acm_suspend() */
		if (!acm->susp_count) {
			for (i = 0; i < acm->rx_buflimit; i++)
				usb_kill_urb(acm->read_urbs[i]);
			usb_clear_halt(acm->dev, acm->in);
			acm_submit_read_urbs(acm, GFP_KERNEL);
			clear_bit(EVENT_RX_STALL, &acm->flags);
		}
	}

	if (test_and_clear_bit(ACM_ERROR_DELAY, &acm->flags)) {
		for (i = 0; i < acm->rx_buflimit; i++)
			if (test_and_clear_bit(i, &acm->urbs_in_error_delay))
				acm_submit_read_urb(acm, i, GFP_KERNEL);
	}

	if (test_and_clear_bit(EVENT_TTY_WAKEUP, &acm->flags))
		tty_port_tty_wakeup(&acm->port);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * TTY handlers
 */

static int acm_tty_install(struct tty_driver *driver, struct tty_struct *tty)
{
	struct acm *acm;
	int retval;

<<<<<<< HEAD
	dev_dbg(tty->dev, "%s\n", __func__);

	acm = acm_get_by_index(tty->index);
=======
	acm = acm_get_by_minor(tty->index);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!acm)
		return -ENODEV;

	retval = tty_standard_install(driver, tty);
	if (retval)
		goto error_init_termios;

<<<<<<< HEAD
=======
	/*
	 * Suppress initial echoing for some devices which might send data
	 * immediately after acm driver has been installed.
	 */
	if (acm->quirks & DISABLE_ECHO)
		tty->termios.c_lflag &= ~ECHO;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	tty->driver_data = acm;

	return 0;

error_init_termios:
	tty_port_put(&acm->port);
	return retval;
}

static int acm_tty_open(struct tty_struct *tty, struct file *filp)
{
	struct acm *acm = tty->driver_data;

<<<<<<< HEAD
	dev_dbg(tty->dev, "%s\n", __func__);

	return tty_port_open(&acm->port, tty, filp);
}

=======
	return tty_port_open(&acm->port, tty, filp);
}

static void acm_port_dtr_rts(struct tty_port *port, bool active)
{
	struct acm *acm = container_of(port, struct acm, port);
	int val;
	int res;

	if (active)
		val = USB_CDC_CTRL_DTR | USB_CDC_CTRL_RTS;
	else
		val = 0;

	/* FIXME: add missing ctrlout locking throughout driver */
	acm->ctrlout = val;

	res = acm_set_control(acm, val);
	if (res && (acm->ctrl_caps & USB_CDC_CAP_LINE))
		/* This is broken in too many devices to spam the logs */
		dev_dbg(&acm->control->dev, "failed to set dtr/rts\n");
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int acm_port_activate(struct tty_port *port, struct tty_struct *tty)
{
	struct acm *acm = container_of(port, struct acm, port);
	int retval = -ENODEV;
	int i;

<<<<<<< HEAD
	dev_dbg(&acm->control->dev, "%s\n", __func__);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mutex_lock(&acm->mutex);
	if (acm->disconnected)
		goto disconnected;

	retval = usb_autopm_get_interface(acm->control);
	if (retval)
		goto error_get_interface;

<<<<<<< HEAD
	/*
	 * FIXME: Why do we need this? Allocating 64K of physically contiguous
	 * memory is really nasty...
	 */
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	set_bit(TTY_NO_WRITE_SPLIT, &tty->flags);
	acm->control->needs_remote_wakeup = 1;

	acm->ctrlurb->dev = acm->dev;
<<<<<<< HEAD
	if (usb_submit_urb(acm->ctrlurb, GFP_KERNEL)) {
=======
	retval = usb_submit_urb(acm->ctrlurb, GFP_KERNEL);
	if (retval) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		dev_err(&acm->control->dev,
			"%s - usb_submit_urb(ctrl irq) failed\n", __func__);
		goto error_submit_urb;
	}

<<<<<<< HEAD
	acm->ctrlout = ACM_CTRL_DTR | ACM_CTRL_RTS;
	if (acm_set_control(acm, acm->ctrlout) < 0 &&
	    (acm->ctrl_caps & USB_CDC_CAP_LINE))
		goto error_set_control;

	usb_autopm_put_interface(acm->control);
=======
	acm_tty_set_termios(tty, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Unthrottle device in case the TTY was closed while throttled.
	 */
<<<<<<< HEAD
	spin_lock_irq(&acm->read_lock);
	acm->throttled = 0;
	acm->throttle_req = 0;
	spin_unlock_irq(&acm->read_lock);

	if (acm_submit_read_urbs(acm, GFP_KERNEL))
		goto error_submit_read_urbs;

=======
	clear_bit(ACM_THROTTLED, &acm->flags);

	retval = acm_submit_read_urbs(acm, GFP_KERNEL);
	if (retval)
		goto error_submit_read_urbs;

	usb_autopm_put_interface(acm->control);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mutex_unlock(&acm->mutex);

	return 0;

error_submit_read_urbs:
	for (i = 0; i < acm->rx_buflimit; i++)
		usb_kill_urb(acm->read_urbs[i]);
<<<<<<< HEAD
	acm->ctrlout = 0;
	acm_set_control(acm, acm->ctrlout);
error_set_control:
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	usb_kill_urb(acm->ctrlurb);
error_submit_urb:
	usb_autopm_put_interface(acm->control);
error_get_interface:
disconnected:
	mutex_unlock(&acm->mutex);
<<<<<<< HEAD
	return retval;
=======

	return usb_translate_errors(retval);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void acm_port_destruct(struct tty_port *port)
{
	struct acm *acm = container_of(port, struct acm, port);

<<<<<<< HEAD
	dev_dbg(&acm->control->dev, "%s\n", __func__);

	acm_release_minor(acm);
=======
	if (acm->minor != ACM_MINOR_INVALID)
		acm_release_minor(acm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	usb_put_intf(acm->control);
	kfree(acm->country_codes);
	kfree(acm);
}

static void acm_port_shutdown(struct tty_port *port)
{
	struct acm *acm = container_of(port, struct acm, port);
	struct urb *urb;
	struct acm_wb *wb;
<<<<<<< HEAD
	int i;
	int pm_err;

	dev_dbg(&acm->control->dev, "%s\n", __func__);

	mutex_lock(&acm->mutex);
	if (!acm->disconnected) {
		pm_err = usb_autopm_get_interface(acm->control);
		acm_set_control(acm, acm->ctrlout = 0);

		for (;;) {
			urb = usb_get_from_anchor(&acm->delayed);
			if (!urb)
				break;
			wb = urb->context;
			wb->use = 0;
			usb_autopm_put_interface_async(acm->control);
		}

		usb_kill_urb(acm->ctrlurb);
		for (i = 0; i < ACM_NW; i++)
			usb_kill_urb(acm->wb[i].urb);
		for (i = 0; i < acm->rx_buflimit; i++)
			usb_kill_urb(acm->read_urbs[i]);
		acm->control->needs_remote_wakeup = 0;
		if (!pm_err)
			usb_autopm_put_interface(acm->control);
	}
	mutex_unlock(&acm->mutex);
=======

	/*
	 * Need to grab write_lock to prevent race with resume, but no need to
	 * hold it due to the tty-port initialised flag.
	 */
	acm_poison_urbs(acm);
	spin_lock_irq(&acm->write_lock);
	spin_unlock_irq(&acm->write_lock);

	usb_autopm_get_interface_no_resume(acm->control);
	acm->control->needs_remote_wakeup = 0;
	usb_autopm_put_interface(acm->control);

	for (;;) {
		urb = usb_get_from_anchor(&acm->delayed);
		if (!urb)
			break;
		wb = urb->context;
		wb->use = false;
		usb_autopm_put_interface_async(acm->control);
	}

	acm_unpoison_urbs(acm);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void acm_tty_cleanup(struct tty_struct *tty)
{
	struct acm *acm = tty->driver_data;
<<<<<<< HEAD
	dev_dbg(&acm->control->dev, "%s\n", __func__);
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	tty_port_put(&acm->port);
}

static void acm_tty_hangup(struct tty_struct *tty)
{
	struct acm *acm = tty->driver_data;
<<<<<<< HEAD
	dev_dbg(&acm->control->dev, "%s\n", __func__);
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	tty_port_hangup(&acm->port);
}

static void acm_tty_close(struct tty_struct *tty, struct file *filp)
{
	struct acm *acm = tty->driver_data;
<<<<<<< HEAD
	dev_dbg(&acm->control->dev, "%s\n", __func__);
	tty_port_close(&acm->port, tty, filp);
}

static int acm_tty_write(struct tty_struct *tty,
					const unsigned char *buf, int count)
=======

	tty_port_close(&acm->port, tty, filp);
}

static ssize_t acm_tty_write(struct tty_struct *tty, const u8 *buf,
			     size_t count)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct acm *acm = tty->driver_data;
	int stat;
	unsigned long flags;
	int wbn;
	struct acm_wb *wb;

	if (!count)
		return 0;

<<<<<<< HEAD
	dev_vdbg(&acm->data->dev, "%s - count %d\n", __func__, count);
=======
	dev_vdbg(&acm->data->dev, "%zu bytes from tty layer\n", count);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spin_lock_irqsave(&acm->write_lock, flags);
	wbn = acm_wb_alloc(acm);
	if (wbn < 0) {
		spin_unlock_irqrestore(&acm->write_lock, flags);
		return 0;
	}
	wb = &acm->wb[wbn];

<<<<<<< HEAD
	count = (count > acm->writesize) ? acm->writesize : count;
	dev_vdbg(&acm->data->dev, "%s - write %d\n", __func__, count);
	memcpy(wb->buf, buf, count);
	wb->len = count;
	spin_unlock_irqrestore(&acm->write_lock, flags);

	stat = acm_write_start(acm, wbn);
=======
	if (!acm->dev) {
		wb->use = false;
		spin_unlock_irqrestore(&acm->write_lock, flags);
		return -ENODEV;
	}

	count = (count > acm->writesize) ? acm->writesize : count;
	dev_vdbg(&acm->data->dev, "writing %zu bytes\n", count);
	memcpy(wb->buf, buf, count);
	wb->len = count;

	stat = usb_autopm_get_interface_async(acm->control);
	if (stat) {
		wb->use = false;
		spin_unlock_irqrestore(&acm->write_lock, flags);
		return stat;
	}

	if (acm->susp_count) {
		usb_anchor_urb(wb->urb, &acm->delayed);
		spin_unlock_irqrestore(&acm->write_lock, flags);
		return count;
	}

	stat = acm_start_wb(acm, wb);
	spin_unlock_irqrestore(&acm->write_lock, flags);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (stat < 0)
		return stat;
	return count;
}

<<<<<<< HEAD
static int acm_tty_write_room(struct tty_struct *tty)
=======
static unsigned int acm_tty_write_room(struct tty_struct *tty)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct acm *acm = tty->driver_data;
	/*
	 * Do not let the line discipline to know that we have a reserve,
	 * or it might get too enthusiastic.
	 */
	return acm_wb_is_avail(acm) ? acm->writesize : 0;
}

<<<<<<< HEAD
static int acm_tty_chars_in_buffer(struct tty_struct *tty)
=======
static void acm_tty_flush_buffer(struct tty_struct *tty)
{
	struct acm *acm = tty->driver_data;
	unsigned long flags;
	int i;

	spin_lock_irqsave(&acm->write_lock, flags);
	for (i = 0; i < ACM_NW; i++)
		if (acm->wb[i].use)
			usb_unlink_urb(acm->wb[i].urb);
	spin_unlock_irqrestore(&acm->write_lock, flags);
}

static unsigned int acm_tty_chars_in_buffer(struct tty_struct *tty)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct acm *acm = tty->driver_data;
	/*
	 * if the device was unplugged then any remaining characters fell out
	 * of the connector ;)
	 */
	if (acm->disconnected)
		return 0;
	/*
	 * This is inaccurate (overcounts), but it works.
	 */
	return (ACM_NW - acm_wb_is_avail(acm)) * acm->writesize;
}

static void acm_tty_throttle(struct tty_struct *tty)
{
	struct acm *acm = tty->driver_data;

<<<<<<< HEAD
	spin_lock_irq(&acm->read_lock);
	acm->throttle_req = 1;
	spin_unlock_irq(&acm->read_lock);
=======
	set_bit(ACM_THROTTLED, &acm->flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void acm_tty_unthrottle(struct tty_struct *tty)
{
	struct acm *acm = tty->driver_data;
<<<<<<< HEAD
	unsigned int was_throttled;

	spin_lock_irq(&acm->read_lock);
	was_throttled = acm->throttled;
	acm->throttled = 0;
	acm->throttle_req = 0;
	spin_unlock_irq(&acm->read_lock);

	if (was_throttled)
		acm_submit_read_urbs(acm, GFP_KERNEL);
=======

	clear_bit(ACM_THROTTLED, &acm->flags);

	/* Matches the smp_mb__after_atomic() in acm_read_bulk_callback(). */
	smp_mb();

	acm_submit_read_urbs(acm, GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int acm_tty_break_ctl(struct tty_struct *tty, int state)
{
	struct acm *acm = tty->driver_data;
	int retval;

<<<<<<< HEAD
	retval = acm_send_break(acm, state ? 0xffff : 0);
	if (retval < 0)
		dev_dbg(&acm->control->dev, "%s - send break failed\n",
								__func__);
=======
	if (!(acm->ctrl_caps & USB_CDC_CAP_BRK))
		return -EOPNOTSUPP;

	retval = acm_send_break(acm, state ? 0xffff : 0);
	if (retval < 0)
		dev_dbg(&acm->control->dev,
			"%s - send break failed\n", __func__);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return retval;
}

static int acm_tty_tiocmget(struct tty_struct *tty)
{
	struct acm *acm = tty->driver_data;

<<<<<<< HEAD
	return (acm->ctrlout & ACM_CTRL_DTR ? TIOCM_DTR : 0) |
	       (acm->ctrlout & ACM_CTRL_RTS ? TIOCM_RTS : 0) |
	       (acm->ctrlin  & ACM_CTRL_DSR ? TIOCM_DSR : 0) |
	       (acm->ctrlin  & ACM_CTRL_RI  ? TIOCM_RI  : 0) |
	       (acm->ctrlin  & ACM_CTRL_DCD ? TIOCM_CD  : 0) |
=======
	return (acm->ctrlout & USB_CDC_CTRL_DTR ? TIOCM_DTR : 0) |
	       (acm->ctrlout & USB_CDC_CTRL_RTS ? TIOCM_RTS : 0) |
	       (acm->ctrlin  & USB_CDC_SERIAL_STATE_DSR ? TIOCM_DSR : 0) |
	       (acm->ctrlin  & USB_CDC_SERIAL_STATE_RING_SIGNAL ? TIOCM_RI : 0) |
	       (acm->ctrlin  & USB_CDC_SERIAL_STATE_DCD ? TIOCM_CD : 0) |
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	       TIOCM_CTS;
}

static int acm_tty_tiocmset(struct tty_struct *tty,
			    unsigned int set, unsigned int clear)
{
	struct acm *acm = tty->driver_data;
	unsigned int newctrl;

	newctrl = acm->ctrlout;
<<<<<<< HEAD
	set = (set & TIOCM_DTR ? ACM_CTRL_DTR : 0) |
					(set & TIOCM_RTS ? ACM_CTRL_RTS : 0);
	clear = (clear & TIOCM_DTR ? ACM_CTRL_DTR : 0) |
					(clear & TIOCM_RTS ? ACM_CTRL_RTS : 0);
=======
	set = (set & TIOCM_DTR ? USB_CDC_CTRL_DTR : 0) |
	      (set & TIOCM_RTS ? USB_CDC_CTRL_RTS : 0);
	clear = (clear & TIOCM_DTR ? USB_CDC_CTRL_DTR : 0) |
		(clear & TIOCM_RTS ? USB_CDC_CTRL_RTS : 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	newctrl = (newctrl & ~clear) | set;

	if (acm->ctrlout == newctrl)
		return 0;
	return acm_set_control(acm, acm->ctrlout = newctrl);
}

<<<<<<< HEAD
static int get_serial_info(struct acm *acm, struct serial_struct __user *info)
{
	struct serial_struct tmp;

	if (!info)
		return -EINVAL;

	memset(&tmp, 0, sizeof(tmp));
	tmp.flags = ASYNC_LOW_LATENCY;
	tmp.xmit_fifo_size = acm->writesize;
	tmp.baud_base = le32_to_cpu(acm->line.dwDTERate);
	tmp.close_delay	= acm->port.close_delay / 10;
	tmp.closing_wait = acm->port.closing_wait == ASYNC_CLOSING_WAIT_NONE ?
				ASYNC_CLOSING_WAIT_NONE :
				acm->port.closing_wait / 10;

	if (copy_to_user(info, &tmp, sizeof(tmp)))
		return -EFAULT;
	else
		return 0;
}

static int set_serial_info(struct acm *acm,
				struct serial_struct __user *newinfo)
{
	struct serial_struct new_serial;
	unsigned int closing_wait, close_delay;
	int retval = 0;

	if (copy_from_user(&new_serial, newinfo, sizeof(new_serial)))
		return -EFAULT;

	close_delay = new_serial.close_delay * 10;
	closing_wait = new_serial.closing_wait == ASYNC_CLOSING_WAIT_NONE ?
			ASYNC_CLOSING_WAIT_NONE : new_serial.closing_wait * 10;
=======
static int get_serial_info(struct tty_struct *tty, struct serial_struct *ss)
{
	struct acm *acm = tty->driver_data;

	ss->line = acm->minor;
	ss->close_delay	= jiffies_to_msecs(acm->port.close_delay) / 10;
	ss->closing_wait = acm->port.closing_wait == ASYNC_CLOSING_WAIT_NONE ?
				ASYNC_CLOSING_WAIT_NONE :
				jiffies_to_msecs(acm->port.closing_wait) / 10;
	return 0;
}

static int set_serial_info(struct tty_struct *tty, struct serial_struct *ss)
{
	struct acm *acm = tty->driver_data;
	unsigned int closing_wait, close_delay;
	int retval = 0;

	close_delay = msecs_to_jiffies(ss->close_delay * 10);
	closing_wait = ss->closing_wait == ASYNC_CLOSING_WAIT_NONE ?
			ASYNC_CLOSING_WAIT_NONE :
			msecs_to_jiffies(ss->closing_wait * 10);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	mutex_lock(&acm->port.mutex);

	if (!capable(CAP_SYS_ADMIN)) {
		if ((close_delay != acm->port.close_delay) ||
		    (closing_wait != acm->port.closing_wait))
			retval = -EPERM;
<<<<<<< HEAD
		else
			retval = -EOPNOTSUPP;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		acm->port.close_delay  = close_delay;
		acm->port.closing_wait = closing_wait;
	}

	mutex_unlock(&acm->port.mutex);
	return retval;
}

<<<<<<< HEAD
=======
static int wait_serial_change(struct acm *acm, unsigned long arg)
{
	int rv = 0;
	DECLARE_WAITQUEUE(wait, current);
	struct async_icount old, new;

	do {
		spin_lock_irq(&acm->read_lock);
		old = acm->oldcount;
		new = acm->iocount;
		acm->oldcount = new;
		spin_unlock_irq(&acm->read_lock);

		if ((arg & TIOCM_DSR) &&
			old.dsr != new.dsr)
			break;
		if ((arg & TIOCM_CD)  &&
			old.dcd != new.dcd)
			break;
		if ((arg & TIOCM_RI) &&
			old.rng != new.rng)
			break;

		add_wait_queue(&acm->wioctl, &wait);
		set_current_state(TASK_INTERRUPTIBLE);
		schedule();
		remove_wait_queue(&acm->wioctl, &wait);
		if (acm->disconnected) {
			if (arg & TIOCM_CD)
				break;
			else
				rv = -ENODEV;
		} else {
			if (signal_pending(current))
				rv = -ERESTARTSYS;
		}
	} while (!rv);

	

	return rv;
}

static int acm_tty_get_icount(struct tty_struct *tty,
					struct serial_icounter_struct *icount)
{
	struct acm *acm = tty->driver_data;

	icount->dsr = acm->iocount.dsr;
	icount->rng = acm->iocount.rng;
	icount->dcd = acm->iocount.dcd;
	icount->frame = acm->iocount.frame;
	icount->overrun = acm->iocount.overrun;
	icount->parity = acm->iocount.parity;
	icount->brk = acm->iocount.brk;

	return 0;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int acm_tty_ioctl(struct tty_struct *tty,
					unsigned int cmd, unsigned long arg)
{
	struct acm *acm = tty->driver_data;
	int rv = -ENOIOCTLCMD;

	switch (cmd) {
<<<<<<< HEAD
	case TIOCGSERIAL: /* gets serial port data */
		rv = get_serial_info(acm, (struct serial_struct __user *) arg);
		break;
	case TIOCSSERIAL:
		rv = set_serial_info(acm, (struct serial_struct __user *) arg);
=======
	case TIOCMIWAIT:
		rv = usb_autopm_get_interface(acm->control);
		if (rv < 0) {
			rv = -EIO;
			break;
		}
		rv = wait_serial_change(acm, arg);
		usb_autopm_put_interface(acm->control);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	}

	return rv;
}

<<<<<<< HEAD
static const __u32 acm_tty_speed[] = {
	0, 50, 75, 110, 134, 150, 200, 300, 600,
	1200, 1800, 2400, 4800, 9600, 19200, 38400,
	57600, 115200, 230400, 460800, 500000, 576000,
	921600, 1000000, 1152000, 1500000, 2000000,
	2500000, 3000000, 3500000, 4000000
};

static void acm_tty_set_termios(struct tty_struct *tty,
						struct ktermios *termios_old)
{
	struct acm *acm = tty->driver_data;
	struct ktermios *termios = tty->termios;
=======
static void acm_tty_set_termios(struct tty_struct *tty,
				const struct ktermios *termios_old)
{
	struct acm *acm = tty->driver_data;
	struct ktermios *termios = &tty->termios;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct usb_cdc_line_coding newline;
	int newctrl = acm->ctrlout;

	newline.dwDTERate = cpu_to_le32(tty_get_baud_rate(tty));
	newline.bCharFormat = termios->c_cflag & CSTOPB ? 2 : 0;
	newline.bParityType = termios->c_cflag & PARENB ?
				(termios->c_cflag & PARODD ? 1 : 2) +
				(termios->c_cflag & CMSPAR ? 2 : 0) : 0;
<<<<<<< HEAD
	switch (termios->c_cflag & CSIZE) {
	case CS5:
		newline.bDataBits = 5;
		break;
	case CS6:
		newline.bDataBits = 6;
		break;
	case CS7:
		newline.bDataBits = 7;
		break;
	case CS8:
	default:
		newline.bDataBits = 8;
		break;
	}
=======
	newline.bDataBits = tty_get_char_size(termios->c_cflag);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* FIXME: Needs to clear unsupported bits in the termios */
	acm->clocal = ((termios->c_cflag & CLOCAL) != 0);

	if (C_BAUD(tty) == B0) {
		newline.dwDTERate = acm->line.dwDTERate;
<<<<<<< HEAD
		newctrl &= ~ACM_CTRL_DTR;
	} else if (termios_old && (termios_old->c_cflag & CBAUD) == B0) {
		newctrl |=  ACM_CTRL_DTR;
=======
		newctrl &= ~USB_CDC_CTRL_DTR;
	} else if (termios_old && (termios_old->c_cflag & CBAUD) == B0) {
		newctrl |=  USB_CDC_CTRL_DTR;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (newctrl != acm->ctrlout)
		acm_set_control(acm, acm->ctrlout = newctrl);

	if (memcmp(&acm->line, &newline, sizeof newline)) {
		memcpy(&acm->line, &newline, sizeof newline);
		dev_dbg(&acm->control->dev, "%s - set line: %d %d %d %d\n",
			__func__,
			le32_to_cpu(newline.dwDTERate),
			newline.bCharFormat, newline.bParityType,
			newline.bDataBits);
		acm_set_line(acm, &acm->line);
	}
}

static const struct tty_port_operations acm_port_ops = {
<<<<<<< HEAD
=======
	.dtr_rts = acm_port_dtr_rts,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.shutdown = acm_port_shutdown,
	.activate = acm_port_activate,
	.destruct = acm_port_destruct,
};

/*
 * USB probe and disconnect routines.
 */

/* Little helpers: write/read buffers free */
static void acm_write_buffers_free(struct acm *acm)
{
	int i;
	struct acm_wb *wb;
<<<<<<< HEAD
	struct usb_device *usb_dev = interface_to_usbdev(acm->control);

	for (wb = &acm->wb[0], i = 0; i < ACM_NW; i++, wb++)
		usb_free_coherent(usb_dev, acm->writesize, wb->buf, wb->dmah);
=======

	for (wb = &acm->wb[0], i = 0; i < ACM_NW; i++, wb++)
		usb_free_coherent(acm->dev, acm->writesize, wb->buf, wb->dmah);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void acm_read_buffers_free(struct acm *acm)
{
<<<<<<< HEAD
	struct usb_device *usb_dev = interface_to_usbdev(acm->control);
	int i;

	for (i = 0; i < acm->rx_buflimit; i++)
		usb_free_coherent(usb_dev, acm->readsize,
=======
	int i;

	for (i = 0; i < acm->rx_buflimit; i++)
		usb_free_coherent(acm->dev, acm->readsize,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			  acm->read_buffers[i].base, acm->read_buffers[i].dma);
}

/* Little helper: write buffers allocate */
static int acm_write_buffers_alloc(struct acm *acm)
{
	int i;
	struct acm_wb *wb;

	for (wb = &acm->wb[0], i = 0; i < ACM_NW; i++, wb++) {
		wb->buf = usb_alloc_coherent(acm->dev, acm->writesize, GFP_KERNEL,
		    &wb->dmah);
		if (!wb->buf) {
			while (i != 0) {
				--i;
				--wb;
				usb_free_coherent(acm->dev, acm->writesize,
				    wb->buf, wb->dmah);
			}
			return -ENOMEM;
		}
	}
	return 0;
}

static int acm_probe(struct usb_interface *intf,
		     const struct usb_device_id *id)
{
	struct usb_cdc_union_desc *union_header = NULL;
<<<<<<< HEAD
	struct usb_cdc_country_functional_desc *cfd = NULL;
=======
	struct usb_cdc_call_mgmt_descriptor *cmgmd = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned char *buffer = intf->altsetting->extra;
	int buflen = intf->altsetting->extralen;
	struct usb_interface *control_interface;
	struct usb_interface *data_interface;
	struct usb_endpoint_descriptor *epctrl = NULL;
	struct usb_endpoint_descriptor *epread = NULL;
	struct usb_endpoint_descriptor *epwrite = NULL;
	struct usb_device *usb_dev = interface_to_usbdev(intf);
<<<<<<< HEAD
=======
	struct usb_cdc_parsed_header h;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct acm *acm;
	int minor;
	int ctrlsize, readsize;
	u8 *buf;
<<<<<<< HEAD
	u8 ac_management_function = 0;
	u8 call_management_function = 0;
	int call_interface_num = -1;
	int data_interface_num = -1;
	unsigned long quirks;
	int num_rx_buf;
	int i;
	unsigned int elength = 0;
	int combined_interfaces = 0;

	/* normal quirks */
	quirks = (unsigned long)id->driver_info;
=======
	int call_intf_num = -1;
	int data_intf_num = -1;
	unsigned long quirks;
	int num_rx_buf;
	int i;
	int combined_interfaces = 0;
	struct device *tty_dev;
	int rv = -ENOMEM;
	int res;

	/* normal quirks */
	quirks = (unsigned long)id->driver_info;

	if (quirks == IGNORE_DEVICE)
		return -ENODEV;

	memset(&h, 0x00, sizeof(struct usb_cdc_parsed_header));

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	num_rx_buf = (quirks == SINGLE_RX_URB) ? 1 : ACM_NR;

	/* handle quirks deadly to normal probing*/
	if (quirks == NO_UNION_NORMAL) {
		data_interface = usb_ifnum_to_if(usb_dev, 1);
		control_interface = usb_ifnum_to_if(usb_dev, 0);
		/* we would crash */
		if (!data_interface || !control_interface)
			return -ENODEV;
		goto skip_normal_probe;
	}

	/* normal probing*/
	if (!buffer) {
		dev_err(&intf->dev, "Weird descriptor references\n");
		return -EINVAL;
	}

	if (!buflen) {
		if (intf->cur_altsetting->endpoint &&
				intf->cur_altsetting->endpoint->extralen &&
				intf->cur_altsetting->endpoint->extra) {
			dev_dbg(&intf->dev,
				"Seeking extra descriptors on endpoint\n");
			buflen = intf->cur_altsetting->endpoint->extralen;
			buffer = intf->cur_altsetting->endpoint->extra;
		} else {
			dev_err(&intf->dev,
				"Zero length descriptor references\n");
			return -EINVAL;
		}
	}

<<<<<<< HEAD
	while (buflen > 0) {
		if ((buflen < buffer[0]) || (buffer[0] < 3)) {
			dev_err(&intf->dev, "invalid descriptor buffer length\n");
			break;
		}

		elength = buffer[0];
		if (!elength) {
			dev_err(&intf->dev, "skipping garbage byte\n");
			elength = 1;
			goto next_desc;
		}
		if (buffer[1] != USB_DT_CS_INTERFACE) {
			dev_err(&intf->dev, "skipping garbage\n");
			goto next_desc;
		}

		switch (buffer[2]) {
		case USB_CDC_UNION_TYPE: /* we've found it */
			if (elength < sizeof(struct usb_cdc_union_desc))
				goto next_desc;
			if (union_header) {
				dev_err(&intf->dev, "More than one "
					"union descriptor, skipping ...\n");
				goto next_desc;
			}
			union_header = (struct usb_cdc_union_desc *)buffer;
			break;
		case USB_CDC_COUNTRY_TYPE: /* export through sysfs*/
			if (elength < sizeof(struct usb_cdc_country_functional_desc))
				goto next_desc;
			cfd = (struct usb_cdc_country_functional_desc *)buffer;
			break;
		case USB_CDC_HEADER_TYPE: /* maybe check version */
			break; /* for now we ignore it */
		case USB_CDC_ACM_TYPE:
			if (elength < 4)
				goto next_desc;
			ac_management_function = buffer[3];
			break;
		case USB_CDC_CALL_MANAGEMENT_TYPE:
			if (elength < 5)
				goto next_desc;
			call_management_function = buffer[3];
			call_interface_num = buffer[4];
			if ( (quirks & NOT_A_MODEM) == 0 && (call_management_function & 3) != 3)
				dev_err(&intf->dev, "This device cannot do calls on its own. It is not a modem.\n");
			break;
		default:
			/*
			 * there are LOTS more CDC descriptors that
			 * could legitimately be found here.
			 */
			dev_dbg(&intf->dev, "Ignoring descriptor: "
					"type %02x, length %ud\n",
					buffer[2], elength);
			break;
		}
next_desc:
		buflen -= elength;
		buffer += elength;
	}

	if (!union_header) {
		if (call_interface_num > 0) {
			dev_dbg(&intf->dev, "No union descriptor, using call management descriptor\n");
			/* quirks for Droids MuIn LCD */
			if (quirks & NO_DATA_INTERFACE)
				data_interface = usb_ifnum_to_if(usb_dev, 0);
			else
				data_interface = usb_ifnum_to_if(usb_dev, (data_interface_num = call_interface_num));
			control_interface = intf;
		} else {
			if (intf->cur_altsetting->desc.bNumEndpoints != 3) {
				dev_dbg(&intf->dev,"No union descriptor, giving up\n");
				return -ENODEV;
			} else {
				dev_warn(&intf->dev,"No union descriptor, testing for castrated device\n");
				combined_interfaces = 1;
				control_interface = data_interface = intf;
				goto look_for_collapsed_interface;
			}
		}
	} else {
		control_interface = usb_ifnum_to_if(usb_dev, union_header->bMasterInterface0);
		data_interface = usb_ifnum_to_if(usb_dev, (data_interface_num = union_header->bSlaveInterface0));
=======
	cdc_parse_cdc_header(&h, intf, buffer, buflen);
	union_header = h.usb_cdc_union_desc;
	cmgmd = h.usb_cdc_call_mgmt_descriptor;
	if (cmgmd)
		call_intf_num = cmgmd->bDataInterface;

	if (!union_header) {
		if (intf->cur_altsetting->desc.bNumEndpoints == 3) {
			dev_dbg(&intf->dev, "No union descriptor, assuming single interface\n");
			combined_interfaces = 1;
			control_interface = data_interface = intf;
			goto look_for_collapsed_interface;
		} else if (call_intf_num > 0) {
			dev_dbg(&intf->dev, "No union descriptor, using call management descriptor\n");
			data_intf_num = call_intf_num;
			data_interface = usb_ifnum_to_if(usb_dev, data_intf_num);
			control_interface = intf;
		} else {
			dev_dbg(&intf->dev, "No union descriptor, giving up\n");
			return -ENODEV;
		}
	} else {
		int class = -1;

		data_intf_num = union_header->bSlaveInterface0;
		control_interface = usb_ifnum_to_if(usb_dev, union_header->bMasterInterface0);
		data_interface = usb_ifnum_to_if(usb_dev, data_intf_num);

		if (control_interface)
			class = control_interface->cur_altsetting->desc.bInterfaceClass;

		if (class != USB_CLASS_COMM && class != USB_CLASS_CDC_DATA) {
			dev_dbg(&intf->dev, "Broken union descriptor, assuming single interface\n");
			combined_interfaces = 1;
			control_interface = data_interface = intf;
			goto look_for_collapsed_interface;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (!control_interface || !data_interface) {
		dev_dbg(&intf->dev, "no interfaces\n");
		return -ENODEV;
	}

<<<<<<< HEAD
	if (data_interface_num != call_interface_num)
=======
	if (data_intf_num != call_intf_num)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		dev_dbg(&intf->dev, "Separate call control interface. That is not fully supported.\n");

	if (control_interface == data_interface) {
		/* some broken devices designed for windows work this way */
		dev_warn(&intf->dev,"Control and data interfaces are not separated!\n");
		combined_interfaces = 1;
		/* a popular other OS doesn't use it */
		quirks |= NO_CAP_LINE;
		if (data_interface->cur_altsetting->desc.bNumEndpoints != 3) {
			dev_err(&intf->dev, "This needs exactly 3 endpoints\n");
			return -EINVAL;
		}
look_for_collapsed_interface:
<<<<<<< HEAD
		for (i = 0; i < 3; i++) {
			struct usb_endpoint_descriptor *ep;
			ep = &data_interface->cur_altsetting->endpoint[i].desc;

			if (usb_endpoint_is_int_in(ep))
				epctrl = ep;
			else if (usb_endpoint_is_bulk_out(ep))
				epwrite = ep;
			else if (usb_endpoint_is_bulk_in(ep))
				epread = ep;
			else
				return -EINVAL;
		}
		if (!epctrl || !epread || !epwrite)
			return -ENODEV;
		else
			goto made_compressed_probe;
=======
		res = usb_find_common_endpoints(data_interface->cur_altsetting,
				&epread, &epwrite, &epctrl, NULL);
		if (res)
			return res;

		goto made_compressed_probe;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

skip_normal_probe:

	/*workaround for switched interfaces */
<<<<<<< HEAD
	if (data_interface->cur_altsetting->desc.bInterfaceClass
						!= CDC_DATA_INTERFACE_TYPE) {
		if (control_interface->cur_altsetting->desc.bInterfaceClass
						== CDC_DATA_INTERFACE_TYPE) {
			struct usb_interface *t;
			dev_dbg(&intf->dev,
				"Your device has switched interfaces.\n");
			t = control_interface;
			control_interface = data_interface;
			data_interface = t;
=======
	if (data_interface->cur_altsetting->desc.bInterfaceClass != USB_CLASS_CDC_DATA) {
		if (control_interface->cur_altsetting->desc.bInterfaceClass == USB_CLASS_CDC_DATA) {
			dev_dbg(&intf->dev,
				"Your device has switched interfaces.\n");
			swap(control_interface, data_interface);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		} else {
			return -EINVAL;
		}
	}

	/* Accept probe requests only for the control interface */
	if (!combined_interfaces && intf != control_interface)
		return -ENODEV;

<<<<<<< HEAD
	if (!combined_interfaces && usb_interface_claimed(data_interface)) {
		/* valid in this context */
		dev_dbg(&intf->dev, "The data interface isn't available\n");
		return -EBUSY;
	}


=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (data_interface->cur_altsetting->desc.bNumEndpoints < 2 ||
	    control_interface->cur_altsetting->desc.bNumEndpoints == 0)
		return -EINVAL;

	epctrl = &control_interface->cur_altsetting->endpoint[0].desc;
	epread = &data_interface->cur_altsetting->endpoint[0].desc;
	epwrite = &data_interface->cur_altsetting->endpoint[1].desc;


	/* workaround for switched endpoints */
	if (!usb_endpoint_dir_in(epread)) {
		/* descriptors are swapped */
<<<<<<< HEAD
		struct usb_endpoint_descriptor *t;
		dev_dbg(&intf->dev,
			"The data interface has switched endpoints\n");
		t = epread;
		epread = epwrite;
		epwrite = t;
=======
		dev_dbg(&intf->dev,
			"The data interface has switched endpoints\n");
		swap(epread, epwrite);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
made_compressed_probe:
	dev_dbg(&intf->dev, "interfaces are valid\n");

	acm = kzalloc(sizeof(struct acm), GFP_KERNEL);
<<<<<<< HEAD
	if (acm == NULL) {
		dev_err(&intf->dev, "out of memory (acm kzalloc)\n");
		goto alloc_fail;
	}

	minor = acm_alloc_minor(acm);
	if (minor == ACM_TTY_MINORS) {
		dev_err(&intf->dev, "no more free acm devices\n");
		kfree(acm);
		return -ENODEV;
	}
=======
	if (!acm)
		return -ENOMEM;

	tty_port_init(&acm->port);
	acm->port.ops = &acm_port_ops;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ctrlsize = usb_endpoint_maxp(epctrl);
	readsize = usb_endpoint_maxp(epread) *
				(quirks == SINGLE_RX_URB ? 1 : 2);
	acm->combined_interfaces = combined_interfaces;
	acm->writesize = usb_endpoint_maxp(epwrite) * 20;
	acm->control = control_interface;
	acm->data = data_interface;
<<<<<<< HEAD
	acm->minor = minor;
	acm->dev = usb_dev;
	acm->ctrl_caps = ac_management_function;
=======

	usb_get_intf(acm->control); /* undone in destruct() */

	minor = acm_alloc_minor(acm);
	if (minor < 0) {
		acm->minor = ACM_MINOR_INVALID;
		goto err_put_port;
	}

	acm->minor = minor;
	acm->dev = usb_dev;
	if (h.usb_cdc_acm_descriptor)
		acm->ctrl_caps = h.usb_cdc_acm_descriptor->bmCapabilities;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (quirks & NO_CAP_LINE)
		acm->ctrl_caps &= ~USB_CDC_CAP_LINE;
	acm->ctrlsize = ctrlsize;
	acm->readsize = readsize;
	acm->rx_buflimit = num_rx_buf;
<<<<<<< HEAD
	INIT_WORK(&acm->work, acm_softint);
	spin_lock_init(&acm->write_lock);
	spin_lock_init(&acm->read_lock);
	mutex_init(&acm->mutex);
	acm->rx_endpoint = usb_rcvbulkpipe(usb_dev, epread->bEndpointAddress);
	acm->is_int_ep = usb_endpoint_xfer_int(epread);
	if (acm->is_int_ep)
		acm->bInterval = epread->bInterval;
	tty_port_init(&acm->port);
	acm->port.ops = &acm_port_ops;
	init_usb_anchor(&acm->delayed);

	buf = usb_alloc_coherent(usb_dev, ctrlsize, GFP_KERNEL, &acm->ctrl_dma);
	if (!buf) {
		dev_err(&intf->dev, "out of memory (ctrl buffer alloc)\n");
		goto alloc_fail2;
	}
	acm->ctrl_buffer = buf;

	if (acm_write_buffers_alloc(acm) < 0) {
		dev_err(&intf->dev, "out of memory (write buffer alloc)\n");
		goto alloc_fail4;
	}

	acm->ctrlurb = usb_alloc_urb(0, GFP_KERNEL);
	if (!acm->ctrlurb) {
		dev_err(&intf->dev, "out of memory (ctrlurb kmalloc)\n");
		goto alloc_fail5;
	}
=======
	INIT_DELAYED_WORK(&acm->dwork, acm_softint);
	init_waitqueue_head(&acm->wioctl);
	spin_lock_init(&acm->write_lock);
	spin_lock_init(&acm->read_lock);
	mutex_init(&acm->mutex);
	if (usb_endpoint_xfer_int(epread)) {
		acm->bInterval = epread->bInterval;
		acm->in = usb_rcvintpipe(usb_dev, epread->bEndpointAddress);
	} else {
		acm->in = usb_rcvbulkpipe(usb_dev, epread->bEndpointAddress);
	}
	if (usb_endpoint_xfer_int(epwrite))
		acm->out = usb_sndintpipe(usb_dev, epwrite->bEndpointAddress);
	else
		acm->out = usb_sndbulkpipe(usb_dev, epwrite->bEndpointAddress);
	init_usb_anchor(&acm->delayed);
	acm->quirks = quirks;

	buf = usb_alloc_coherent(usb_dev, ctrlsize, GFP_KERNEL, &acm->ctrl_dma);
	if (!buf)
		goto err_put_port;
	acm->ctrl_buffer = buf;

	if (acm_write_buffers_alloc(acm) < 0)
		goto err_free_ctrl_buffer;

	acm->ctrlurb = usb_alloc_urb(0, GFP_KERNEL);
	if (!acm->ctrlurb)
		goto err_free_write_buffers;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	for (i = 0; i < num_rx_buf; i++) {
		struct acm_rb *rb = &(acm->read_buffers[i]);
		struct urb *urb;

		rb->base = usb_alloc_coherent(acm->dev, readsize, GFP_KERNEL,
								&rb->dma);
<<<<<<< HEAD
		if (!rb->base) {
			dev_err(&intf->dev, "out of memory "
					"(read bufs usb_alloc_coherent)\n");
			goto alloc_fail6;
		}
=======
		if (!rb->base)
			goto err_free_read_urbs;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		rb->index = i;
		rb->instance = acm;

		urb = usb_alloc_urb(0, GFP_KERNEL);
<<<<<<< HEAD
		if (!urb) {
			dev_err(&intf->dev,
				"out of memory (read urbs usb_alloc_urb)\n");
			goto alloc_fail6;
		}
		urb->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;
		urb->transfer_dma = rb->dma;
		if (acm->is_int_ep) {
			usb_fill_int_urb(urb, acm->dev,
					 acm->rx_endpoint,
					 rb->base,
					 acm->readsize,
					 acm_read_bulk_callback, rb,
					 acm->bInterval);
		} else {
			usb_fill_bulk_urb(urb, acm->dev,
					  acm->rx_endpoint,
					  rb->base,
					  acm->readsize,
					  acm_read_bulk_callback, rb);
		}
=======
		if (!urb)
			goto err_free_read_urbs;

		urb->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;
		urb->transfer_dma = rb->dma;
		if (usb_endpoint_xfer_int(epread))
			usb_fill_int_urb(urb, acm->dev, acm->in, rb->base,
					 acm->readsize,
					 acm_read_bulk_callback, rb,
					 acm->bInterval);
		else
			usb_fill_bulk_urb(urb, acm->dev, acm->in, rb->base,
					  acm->readsize,
					  acm_read_bulk_callback, rb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		acm->read_urbs[i] = urb;
		__set_bit(i, &acm->read_urbs_free);
	}
	for (i = 0; i < ACM_NW; i++) {
		struct acm_wb *snd = &(acm->wb[i]);

		snd->urb = usb_alloc_urb(0, GFP_KERNEL);
<<<<<<< HEAD
		if (snd->urb == NULL) {
			dev_err(&intf->dev,
				"out of memory (write urbs usb_alloc_urb)\n");
			goto alloc_fail7;
		}

		if (usb_endpoint_xfer_int(epwrite))
			usb_fill_int_urb(snd->urb, usb_dev,
				usb_sndintpipe(usb_dev, epwrite->bEndpointAddress),
				NULL, acm->writesize, acm_write_bulk, snd, epwrite->bInterval);
		else
			usb_fill_bulk_urb(snd->urb, usb_dev,
				usb_sndbulkpipe(usb_dev, epwrite->bEndpointAddress),
				NULL, acm->writesize, acm_write_bulk, snd);
		snd->urb->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;
=======
		if (!snd->urb)
			goto err_free_write_urbs;

		if (usb_endpoint_xfer_int(epwrite))
			usb_fill_int_urb(snd->urb, usb_dev, acm->out,
				NULL, acm->writesize, acm_write_bulk, snd, epwrite->bInterval);
		else
			usb_fill_bulk_urb(snd->urb, usb_dev, acm->out,
				NULL, acm->writesize, acm_write_bulk, snd);
		snd->urb->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;
		if (quirks & SEND_ZERO_PACKET)
			snd->urb->transfer_flags |= URB_ZERO_PACKET;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		snd->instance = acm;
	}

	usb_set_intfdata(intf, acm);

	i = device_create_file(&intf->dev, &dev_attr_bmCapabilities);
	if (i < 0)
<<<<<<< HEAD
		goto alloc_fail7;

	if (cfd) { /* export the country data */
=======
		goto err_free_write_urbs;

	if (h.usb_cdc_country_functional_desc) { /* export the country data */
		struct usb_cdc_country_functional_desc * cfd =
					h.usb_cdc_country_functional_desc;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		acm->country_codes = kmalloc(cfd->bLength - 4, GFP_KERNEL);
		if (!acm->country_codes)
			goto skip_countries;
		acm->country_code_size = cfd->bLength - 4;
		memcpy(acm->country_codes, (u8 *)&cfd->wCountyCode0,
							cfd->bLength - 4);
		acm->country_rel_date = cfd->iCountryCodeRelDate;

		i = device_create_file(&intf->dev, &dev_attr_wCountryCodes);
		if (i < 0) {
			kfree(acm->country_codes);
			acm->country_codes = NULL;
			acm->country_code_size = 0;
			goto skip_countries;
		}

		i = device_create_file(&intf->dev,
						&dev_attr_iCountryCodeRelDate);
		if (i < 0) {
			device_remove_file(&intf->dev, &dev_attr_wCountryCodes);
			kfree(acm->country_codes);
			acm->country_codes = NULL;
			acm->country_code_size = 0;
			goto skip_countries;
		}
	}

skip_countries:
	usb_fill_int_urb(acm->ctrlurb, usb_dev,
			 usb_rcvintpipe(usb_dev, epctrl->bEndpointAddress),
			 acm->ctrl_buffer, ctrlsize, acm_ctrl_irq, acm,
			 /* works around buggy devices */
<<<<<<< HEAD
			 epctrl->bInterval ? epctrl->bInterval : 0xff);
	acm->ctrlurb->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;
	acm->ctrlurb->transfer_dma = acm->ctrl_dma;

	dev_info(&intf->dev, "ttyACM%d: USB ACM device\n", minor);

	acm_set_control(acm, acm->ctrlout);
=======
			 epctrl->bInterval ? epctrl->bInterval : 16);
	acm->ctrlurb->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;
	acm->ctrlurb->transfer_dma = acm->ctrl_dma;
	acm->notification_buffer = NULL;
	acm->nb_index = 0;
	acm->nb_size = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	acm->line.dwDTERate = cpu_to_le32(9600);
	acm->line.bDataBits = 8;
	acm_set_line(acm, &acm->line);

<<<<<<< HEAD
	usb_driver_claim_interface(&acm_driver, data_interface, acm);
	usb_set_intfdata(data_interface, acm);

	usb_get_intf(control_interface);
	tty_register_device(acm_tty_driver, minor, &control_interface->dev);

	return 0;
alloc_fail7:
	for (i = 0; i < ACM_NW; i++)
		usb_free_urb(acm->wb[i].urb);
alloc_fail6:
	for (i = 0; i < num_rx_buf; i++)
		usb_free_urb(acm->read_urbs[i]);
	acm_read_buffers_free(acm);
	usb_free_urb(acm->ctrlurb);
alloc_fail5:
	acm_write_buffers_free(acm);
alloc_fail4:
	usb_free_coherent(usb_dev, ctrlsize, acm->ctrl_buffer, acm->ctrl_dma);
alloc_fail2:
	acm_release_minor(acm);
	kfree(acm);
alloc_fail:
	return -ENOMEM;
}

static void stop_data_traffic(struct acm *acm)
{
	int i;

	dev_dbg(&acm->control->dev, "%s\n", __func__);

	usb_kill_urb(acm->ctrlurb);
	for (i = 0; i < ACM_NW; i++)
		usb_kill_urb(acm->wb[i].urb);
	for (i = 0; i < acm->rx_buflimit; i++)
		usb_kill_urb(acm->read_urbs[i]);

	cancel_work_sync(&acm->work);
}

static void acm_disconnect(struct usb_interface *intf)
{
	struct acm *acm = usb_get_intfdata(intf);
	struct usb_device *usb_dev = interface_to_usbdev(intf);
	struct tty_struct *tty;
	int i;

	dev_dbg(&intf->dev, "%s\n", __func__);

	/* sibling interface is already cleaning up */
	if (!acm)
		return;

	mutex_lock(&acm->mutex);
	acm->disconnected = true;
=======
	if (!acm->combined_interfaces) {
		rv = usb_driver_claim_interface(&acm_driver, data_interface, acm);
		if (rv)
			goto err_remove_files;
	}

	tty_dev = tty_port_register_device(&acm->port, acm_tty_driver, minor,
			&control_interface->dev);
	if (IS_ERR(tty_dev)) {
		rv = PTR_ERR(tty_dev);
		goto err_release_data_interface;
	}

	if (quirks & CLEAR_HALT_CONDITIONS) {
		usb_clear_halt(usb_dev, acm->in);
		usb_clear_halt(usb_dev, acm->out);
	}

	dev_info(&intf->dev, "ttyACM%d: USB ACM device\n", minor);

	return 0;

err_release_data_interface:
	if (!acm->combined_interfaces) {
		/* Clear driver data so that disconnect() returns early. */
		usb_set_intfdata(data_interface, NULL);
		usb_driver_release_interface(&acm_driver, data_interface);
	}
err_remove_files:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (acm->country_codes) {
		device_remove_file(&acm->control->dev,
				&dev_attr_wCountryCodes);
		device_remove_file(&acm->control->dev,
				&dev_attr_iCountryCodeRelDate);
<<<<<<< HEAD
		kfree(acm->country_codes);
	}
	device_remove_file(&acm->control->dev, &dev_attr_bmCapabilities);
=======
	}
	device_remove_file(&acm->control->dev, &dev_attr_bmCapabilities);
err_free_write_urbs:
	for (i = 0; i < ACM_NW; i++)
		usb_free_urb(acm->wb[i].urb);
err_free_read_urbs:
	for (i = 0; i < num_rx_buf; i++)
		usb_free_urb(acm->read_urbs[i]);
	acm_read_buffers_free(acm);
	usb_free_urb(acm->ctrlurb);
err_free_write_buffers:
	acm_write_buffers_free(acm);
err_free_ctrl_buffer:
	usb_free_coherent(usb_dev, ctrlsize, acm->ctrl_buffer, acm->ctrl_dma);
err_put_port:
	tty_port_put(&acm->port);

	return rv;
}

static void acm_disconnect(struct usb_interface *intf)
{
	struct acm *acm = usb_get_intfdata(intf);
	struct tty_struct *tty;
	int i;

	/* sibling interface is already cleaning up */
	if (!acm)
		return;

	acm->disconnected = true;
	/*
	 * there is a circular dependency. acm_softint() can resubmit
	 * the URBs in error handling so we need to block any
	 * submission right away
	 */
	acm_poison_urbs(acm);
	mutex_lock(&acm->mutex);
	if (acm->country_codes) {
		device_remove_file(&acm->control->dev,
				&dev_attr_wCountryCodes);
		device_remove_file(&acm->control->dev,
				&dev_attr_iCountryCodeRelDate);
	}
	wake_up_all(&acm->wioctl);
	device_remove_file(&acm->control->dev, &dev_attr_bmCapabilities);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	usb_set_intfdata(acm->control, NULL);
	usb_set_intfdata(acm->data, NULL);
	mutex_unlock(&acm->mutex);

	tty = tty_port_tty_get(&acm->port);
	if (tty) {
		tty_vhangup(tty);
		tty_kref_put(tty);
	}

<<<<<<< HEAD
	stop_data_traffic(acm);
=======
	cancel_delayed_work_sync(&acm->dwork);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	tty_unregister_device(acm_tty_driver, acm->minor);

	usb_free_urb(acm->ctrlurb);
	for (i = 0; i < ACM_NW; i++)
		usb_free_urb(acm->wb[i].urb);
	for (i = 0; i < acm->rx_buflimit; i++)
		usb_free_urb(acm->read_urbs[i]);
	acm_write_buffers_free(acm);
<<<<<<< HEAD
	usb_free_coherent(usb_dev, acm->ctrlsize, acm->ctrl_buffer, acm->ctrl_dma);
	acm_read_buffers_free(acm);

=======
	usb_free_coherent(acm->dev, acm->ctrlsize, acm->ctrl_buffer, acm->ctrl_dma);
	acm_read_buffers_free(acm);

	kfree(acm->notification_buffer);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!acm->combined_interfaces)
		usb_driver_release_interface(&acm_driver, intf == acm->control ?
					acm->data : acm->control);

	tty_port_put(&acm->port);
}

#ifdef CONFIG_PM
static int acm_suspend(struct usb_interface *intf, pm_message_t message)
{
	struct acm *acm = usb_get_intfdata(intf);
	int cnt;

<<<<<<< HEAD
	spin_lock_irq(&acm->read_lock);
	spin_lock(&acm->write_lock);
	if (PMSG_IS_AUTO(message)) {
		if (acm->transmitting) {
			spin_unlock(&acm->write_lock);
			spin_unlock_irq(&acm->read_lock);
=======
	spin_lock_irq(&acm->write_lock);
	if (PMSG_IS_AUTO(message)) {
		if (acm->transmitting) {
			spin_unlock_irq(&acm->write_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -EBUSY;
		}
	}
	cnt = acm->susp_count++;
<<<<<<< HEAD
	spin_unlock(&acm->write_lock);
	spin_unlock_irq(&acm->read_lock);
=======
	spin_unlock_irq(&acm->write_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (cnt)
		return 0;

<<<<<<< HEAD
	stop_data_traffic(acm);
=======
	acm_poison_urbs(acm);
	cancel_delayed_work_sync(&acm->dwork);
	acm->urbs_in_error_delay = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static int acm_resume(struct usb_interface *intf)
{
	struct acm *acm = usb_get_intfdata(intf);
	struct urb *urb;
	int rv = 0;

<<<<<<< HEAD
	spin_lock_irq(&acm->read_lock);
	spin_lock(&acm->write_lock);
=======
	spin_lock_irq(&acm->write_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (--acm->susp_count)
		goto out;

<<<<<<< HEAD
	if (test_bit(ASYNCB_INITIALIZED, &acm->port.flags)) {
=======
	acm_unpoison_urbs(acm);

	if (tty_port_initialized(&acm->port)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		rv = usb_submit_urb(acm->ctrlurb, GFP_ATOMIC);

		for (;;) {
			urb = usb_get_from_anchor(&acm->delayed);
			if (!urb)
				break;

			acm_start_wb(acm, urb->context);
		}

		/*
		 * delayed error checking because we must
		 * do the write path at all cost
		 */
		if (rv < 0)
			goto out;

		rv = acm_submit_read_urbs(acm, GFP_ATOMIC);
	}
out:
<<<<<<< HEAD
	spin_unlock(&acm->write_lock);
	spin_unlock_irq(&acm->read_lock);
=======
	spin_unlock_irq(&acm->write_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return rv;
}

static int acm_reset_resume(struct usb_interface *intf)
{
	struct acm *acm = usb_get_intfdata(intf);
<<<<<<< HEAD
	struct tty_struct *tty;

	if (test_bit(ASYNCB_INITIALIZED, &acm->port.flags)) {
		tty = tty_port_tty_get(&acm->port);
		if (tty) {
			tty_hangup(tty);
			tty_kref_put(tty);
		}
	}
=======

	if (tty_port_initialized(&acm->port))
		tty_port_tty_hangup(&acm->port, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return acm_resume(intf);
}

#endif /* CONFIG_PM */

<<<<<<< HEAD
=======
static int acm_pre_reset(struct usb_interface *intf)
{
	struct acm *acm = usb_get_intfdata(intf);

	clear_bit(EVENT_RX_STALL, &acm->flags);
	acm->nb_index = 0; /* pending control transfers are lost */

	return 0;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define NOKIA_PCSUITE_ACM_INFO(x) \
		USB_DEVICE_AND_INTERFACE_INFO(0x0421, x, \
		USB_CLASS_COMM, USB_CDC_SUBCLASS_ACM, \
		USB_CDC_ACM_PROTO_VENDOR)

#define SAMSUNG_PCSUITE_ACM_INFO(x) \
		USB_DEVICE_AND_INTERFACE_INFO(0x04e7, x, \
		USB_CLASS_COMM, USB_CDC_SUBCLASS_ACM, \
		USB_CDC_ACM_PROTO_VENDOR)

/*
 * USB driver structure.
 */

static const struct usb_device_id acm_ids[] = {
	/* quirky and broken devices */
<<<<<<< HEAD
=======
	{ USB_DEVICE(0x0424, 0x274e), /* Microchip Technology, Inc. (formerly SMSC) */
	  .driver_info = DISABLE_ECHO, }, /* DISABLE ECHO in termios flag */
	{ USB_DEVICE(0x076d, 0x0006), /* Denso Cradle CU-321 */
	.driver_info = NO_UNION_NORMAL, },/* has no union descriptor */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{ USB_DEVICE(0x17ef, 0x7000), /* Lenovo USB modem */
	.driver_info = NO_UNION_NORMAL, },/* has no union descriptor */
	{ USB_DEVICE(0x0870, 0x0001), /* Metricom GS Modem */
	.driver_info = NO_UNION_NORMAL, /* has no union descriptor */
	},
<<<<<<< HEAD
	{ USB_DEVICE(0x0e8d, 0x0003), /* FIREFLY, MediaTek Inc; andrey.arapov@gmail.com */
	.driver_info = NO_UNION_NORMAL, /* has no union descriptor */
	},
=======
	{ USB_DEVICE(0x045b, 0x023c),	/* Renesas USB Download mode */
	.driver_info = DISABLE_ECHO,	/* Don't echo banner */
	},
	{ USB_DEVICE(0x045b, 0x0248),	/* Renesas USB Download mode */
	.driver_info = DISABLE_ECHO,	/* Don't echo banner */
	},
	{ USB_DEVICE(0x045b, 0x024D),	/* Renesas USB Download mode */
	.driver_info = DISABLE_ECHO,	/* Don't echo banner */
	},
	{ USB_DEVICE(0x0e8d, 0x0003), /* FIREFLY, MediaTek Inc; andrey.arapov@gmail.com */
	.driver_info = NO_UNION_NORMAL, /* has no union descriptor */
	},
	{ USB_DEVICE(0x0e8d, 0x2000), /* MediaTek Inc Preloader */
	.driver_info = DISABLE_ECHO, /* DISABLE ECHO in termios flag */
	},
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{ USB_DEVICE(0x0e8d, 0x3329), /* MediaTek Inc GPS */
	.driver_info = NO_UNION_NORMAL, /* has no union descriptor */
	},
	{ USB_DEVICE(0x0482, 0x0203), /* KYOCERA AH-K3001V */
	.driver_info = NO_UNION_NORMAL, /* has no union descriptor */
	},
	{ USB_DEVICE(0x079b, 0x000f), /* BT On-Air USB MODEM */
	.driver_info = NO_UNION_NORMAL, /* has no union descriptor */
	},
	{ USB_DEVICE(0x0ace, 0x1602), /* ZyDAS 56K USB MODEM */
	.driver_info = SINGLE_RX_URB,
	},
	{ USB_DEVICE(0x0ace, 0x1608), /* ZyDAS 56K USB MODEM */
	.driver_info = SINGLE_RX_URB, /* firmware bug */
	},
	{ USB_DEVICE(0x0ace, 0x1611), /* ZyDAS 56K USB MODEM - new version */
	.driver_info = SINGLE_RX_URB, /* firmware bug */
	},
<<<<<<< HEAD
=======
	{ USB_DEVICE(0x11ca, 0x0201), /* VeriFone Mx870 Gadget Serial */
	.driver_info = SINGLE_RX_URB,
	},
	{ USB_DEVICE(0x1965, 0x0018), /* Uniden UBC125XLT */
	.driver_info = NO_UNION_NORMAL, /* has no union descriptor */
	},
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{ USB_DEVICE(0x22b8, 0x7000), /* Motorola Q Phone */
	.driver_info = NO_UNION_NORMAL, /* has no union descriptor */
	},
	{ USB_DEVICE(0x0803, 0x3095), /* Zoom Telephonics Model 3095F USB MODEM */
	.driver_info = NO_UNION_NORMAL, /* has no union descriptor */
	},
	{ USB_DEVICE(0x0572, 0x1321), /* Conexant USB MODEM CX93010 */
	.driver_info = NO_UNION_NORMAL, /* has no union descriptor */
	},
	{ USB_DEVICE(0x0572, 0x1324), /* Conexant USB MODEM RD02-D400 */
	.driver_info = NO_UNION_NORMAL, /* has no union descriptor */
	},
	{ USB_DEVICE(0x0572, 0x1328), /* Shiro / Aztech USB MODEM UM-3100 */
	.driver_info = NO_UNION_NORMAL, /* has no union descriptor */
	},
<<<<<<< HEAD
	{ USB_DEVICE(0x2184, 0x001c) },	/* GW Instek AFG-2225 */
=======
	{ USB_DEVICE(0x0572, 0x1349), /* Hiro (Conexant) USB MODEM H50228 */
	.driver_info = NO_UNION_NORMAL, /* has no union descriptor */
	},
	{ USB_DEVICE(0x20df, 0x0001), /* Simtec Electronics Entropy Key */
	.driver_info = QUIRK_CONTROL_LINE_STATE, },
	{ USB_DEVICE(0x2184, 0x001c) },	/* GW Instek AFG-2225 */
	{ USB_DEVICE(0x2184, 0x0036) },	/* GW Instek AFG-125 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{ USB_DEVICE(0x22b8, 0x6425), /* Motorola MOTOMAGX phones */
	},
	/* Motorola H24 HSPA module: */
	{ USB_DEVICE(0x22b8, 0x2d91) }, /* modem                                */
	{ USB_DEVICE(0x22b8, 0x2d92),   /* modem           + diagnostics        */
	.driver_info = NO_UNION_NORMAL, /* handle only modem interface          */
	},
	{ USB_DEVICE(0x22b8, 0x2d93),   /* modem + AT port                      */
	.driver_info = NO_UNION_NORMAL, /* handle only modem interface          */
	},
	{ USB_DEVICE(0x22b8, 0x2d95),   /* modem + AT port + diagnostics        */
	.driver_info = NO_UNION_NORMAL, /* handle only modem interface          */
	},
	{ USB_DEVICE(0x22b8, 0x2d96),   /* modem                         + NMEA */
	.driver_info = NO_UNION_NORMAL, /* handle only modem interface          */
	},
	{ USB_DEVICE(0x22b8, 0x2d97),   /* modem           + diagnostics + NMEA */
	.driver_info = NO_UNION_NORMAL, /* handle only modem interface          */
	},
	{ USB_DEVICE(0x22b8, 0x2d99),   /* modem + AT port               + NMEA */
	.driver_info = NO_UNION_NORMAL, /* handle only modem interface          */
	},
	{ USB_DEVICE(0x22b8, 0x2d9a),   /* modem + AT port + diagnostics + NMEA */
	.driver_info = NO_UNION_NORMAL, /* handle only modem interface          */
	},

	{ USB_DEVICE(0x0572, 0x1329), /* Hummingbird huc56s (Conexant) */
	.driver_info = NO_UNION_NORMAL, /* union descriptor misplaced on
					   data interface instead of
					   communications interface.
					   Maybe we should define a new
					   quirk for this. */
	},
	{ USB_DEVICE(0x0572, 0x1340), /* Conexant CX93010-2x UCMxx */
	.driver_info = NO_UNION_NORMAL,
	},
	{ USB_DEVICE(0x05f9, 0x4002), /* PSC Scanning, Magellan 800i */
	.driver_info = NO_UNION_NORMAL,
	},
	{ USB_DEVICE(0x1bbb, 0x0003), /* Alcatel OT-I650 */
	.driver_info = NO_UNION_NORMAL, /* reports zero length descriptor */
	},
	{ USB_DEVICE(0x1576, 0x03b1), /* Maretron USB100 */
	.driver_info = NO_UNION_NORMAL, /* reports zero length descriptor */
	},
<<<<<<< HEAD
=======
	{ USB_DEVICE(0xfff0, 0x0100), /* DATECS FP-2000 */
	.driver_info = NO_UNION_NORMAL, /* reports zero length descriptor */
	},
	{ USB_DEVICE(0x09d8, 0x0320), /* Elatec GmbH TWN3 */
	.driver_info = NO_UNION_NORMAL, /* has misplaced union descriptor */
	},
	{ USB_DEVICE(0x0c26, 0x0020), /* Icom ICF3400 Serie */
	.driver_info = NO_UNION_NORMAL, /* reports zero length descriptor */
	},
	{ USB_DEVICE(0x0ca6, 0xa050), /* Castles VEGA3000 */
	.driver_info = NO_UNION_NORMAL, /* reports zero length descriptor */
	},

	{ USB_DEVICE(0x2912, 0x0001), /* ATOL FPrint */
	.driver_info = CLEAR_HALT_CONDITIONS,
	},
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Nokia S60 phones expose two ACM channels. The first is
	 * a modem and is picked up by the standard AT-command
	 * information below. The second is 'vendor-specific' but
	 * is treated as a serial device at the S60 end, so we want
	 * to expose it on Linux too. */
	{ NOKIA_PCSUITE_ACM_INFO(0x042D), }, /* Nokia 3250 */
	{ NOKIA_PCSUITE_ACM_INFO(0x04D8), }, /* Nokia 5500 Sport */
	{ NOKIA_PCSUITE_ACM_INFO(0x04C9), }, /* Nokia E50 */
	{ NOKIA_PCSUITE_ACM_INFO(0x0419), }, /* Nokia E60 */
	{ NOKIA_PCSUITE_ACM_INFO(0x044D), }, /* Nokia E61 */
	{ NOKIA_PCSUITE_ACM_INFO(0x0001), }, /* Nokia E61i */
	{ NOKIA_PCSUITE_ACM_INFO(0x0475), }, /* Nokia E62 */
	{ NOKIA_PCSUITE_ACM_INFO(0x0508), }, /* Nokia E65 */
	{ NOKIA_PCSUITE_ACM_INFO(0x0418), }, /* Nokia E70 */
	{ NOKIA_PCSUITE_ACM_INFO(0x0425), }, /* Nokia N71 */
	{ NOKIA_PCSUITE_ACM_INFO(0x0486), }, /* Nokia N73 */
	{ NOKIA_PCSUITE_ACM_INFO(0x04DF), }, /* Nokia N75 */
	{ NOKIA_PCSUITE_ACM_INFO(0x000e), }, /* Nokia N77 */
	{ NOKIA_PCSUITE_ACM_INFO(0x0445), }, /* Nokia N80 */
	{ NOKIA_PCSUITE_ACM_INFO(0x042F), }, /* Nokia N91 & N91 8GB */
	{ NOKIA_PCSUITE_ACM_INFO(0x048E), }, /* Nokia N92 */
	{ NOKIA_PCSUITE_ACM_INFO(0x0420), }, /* Nokia N93 */
	{ NOKIA_PCSUITE_ACM_INFO(0x04E6), }, /* Nokia N93i  */
	{ NOKIA_PCSUITE_ACM_INFO(0x04B2), }, /* Nokia 5700 XpressMusic */
	{ NOKIA_PCSUITE_ACM_INFO(0x0134), }, /* Nokia 6110 Navigator (China) */
	{ NOKIA_PCSUITE_ACM_INFO(0x046E), }, /* Nokia 6110 Navigator */
	{ NOKIA_PCSUITE_ACM_INFO(0x002f), }, /* Nokia 6120 classic &  */
	{ NOKIA_PCSUITE_ACM_INFO(0x0088), }, /* Nokia 6121 classic */
	{ NOKIA_PCSUITE_ACM_INFO(0x00fc), }, /* Nokia 6124 classic */
	{ NOKIA_PCSUITE_ACM_INFO(0x0042), }, /* Nokia E51 */
	{ NOKIA_PCSUITE_ACM_INFO(0x00b0), }, /* Nokia E66 */
	{ NOKIA_PCSUITE_ACM_INFO(0x00ab), }, /* Nokia E71 */
	{ NOKIA_PCSUITE_ACM_INFO(0x0481), }, /* Nokia N76 */
	{ NOKIA_PCSUITE_ACM_INFO(0x0007), }, /* Nokia N81 & N81 8GB */
	{ NOKIA_PCSUITE_ACM_INFO(0x0071), }, /* Nokia N82 */
	{ NOKIA_PCSUITE_ACM_INFO(0x04F0), }, /* Nokia N95 & N95-3 NAM */
	{ NOKIA_PCSUITE_ACM_INFO(0x0070), }, /* Nokia N95 8GB  */
<<<<<<< HEAD
	{ NOKIA_PCSUITE_ACM_INFO(0x00e9), }, /* Nokia 5320 XpressMusic */
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{ NOKIA_PCSUITE_ACM_INFO(0x0099), }, /* Nokia 6210 Navigator, RM-367 */
	{ NOKIA_PCSUITE_ACM_INFO(0x0128), }, /* Nokia 6210 Navigator, RM-419 */
	{ NOKIA_PCSUITE_ACM_INFO(0x008f), }, /* Nokia 6220 Classic */
	{ NOKIA_PCSUITE_ACM_INFO(0x00a0), }, /* Nokia 6650 */
	{ NOKIA_PCSUITE_ACM_INFO(0x007b), }, /* Nokia N78 */
	{ NOKIA_PCSUITE_ACM_INFO(0x0094), }, /* Nokia N85 */
	{ NOKIA_PCSUITE_ACM_INFO(0x003a), }, /* Nokia N96 & N96-3  */
	{ NOKIA_PCSUITE_ACM_INFO(0x00e9), }, /* Nokia 5320 XpressMusic */
	{ NOKIA_PCSUITE_ACM_INFO(0x0108), }, /* Nokia 5320 XpressMusic 2G */
	{ NOKIA_PCSUITE_ACM_INFO(0x01f5), }, /* Nokia N97, RM-505 */
	{ NOKIA_PCSUITE_ACM_INFO(0x02e3), }, /* Nokia 5230, RM-588 */
	{ NOKIA_PCSUITE_ACM_INFO(0x0178), }, /* Nokia E63 */
	{ NOKIA_PCSUITE_ACM_INFO(0x010e), }, /* Nokia E75 */
	{ NOKIA_PCSUITE_ACM_INFO(0x02d9), }, /* Nokia 6760 Slide */
	{ NOKIA_PCSUITE_ACM_INFO(0x01d0), }, /* Nokia E52 */
	{ NOKIA_PCSUITE_ACM_INFO(0x0223), }, /* Nokia E72 */
	{ NOKIA_PCSUITE_ACM_INFO(0x0275), }, /* Nokia X6 */
	{ NOKIA_PCSUITE_ACM_INFO(0x026c), }, /* Nokia N97 Mini */
	{ NOKIA_PCSUITE_ACM_INFO(0x0154), }, /* Nokia 5800 XpressMusic */
	{ NOKIA_PCSUITE_ACM_INFO(0x04ce), }, /* Nokia E90 */
	{ NOKIA_PCSUITE_ACM_INFO(0x01d4), }, /* Nokia E55 */
	{ NOKIA_PCSUITE_ACM_INFO(0x0302), }, /* Nokia N8 */
	{ NOKIA_PCSUITE_ACM_INFO(0x0335), }, /* Nokia E7 */
	{ NOKIA_PCSUITE_ACM_INFO(0x03cd), }, /* Nokia C7 */
	{ SAMSUNG_PCSUITE_ACM_INFO(0x6651), }, /* Samsung GTi8510 (INNOV8) */

	/* Support for Owen devices */
	{ USB_DEVICE(0x03eb, 0x0030), }, /* Owen SI30 */

	/* NOTE: non-Nokia COMM/ACM/0xff is likely MSFT RNDIS... NOT a modem! */

<<<<<<< HEAD
	/* Support Lego NXT using pbLua firmware */
	{ USB_DEVICE(0x0694, 0xff00),
	.driver_info = NOT_A_MODEM,
	},

	/* Support for Droids MuIn LCD */
	{ USB_DEVICE(0x04d8, 0x000b),
	.driver_info = NO_DATA_INTERFACE,
=======
#if IS_ENABLED(CONFIG_INPUT_IMS_PCU)
	{ USB_DEVICE(0x04d8, 0x0082),	/* Application mode */
	.driver_info = IGNORE_DEVICE,
	},
	{ USB_DEVICE(0x04d8, 0x0083),	/* Bootloader mode */
	.driver_info = IGNORE_DEVICE,
	},
#endif

#if IS_ENABLED(CONFIG_IR_TOY)
	{ USB_DEVICE(0x04d8, 0xfd08),
	.driver_info = IGNORE_DEVICE,
	},

	{ USB_DEVICE(0x04d8, 0xf58b),
	.driver_info = IGNORE_DEVICE,
	},
#endif

#if IS_ENABLED(CONFIG_USB_SERIAL_XR)
	{ USB_DEVICE(0x04e2, 0x1400), .driver_info = IGNORE_DEVICE },
	{ USB_DEVICE(0x04e2, 0x1401), .driver_info = IGNORE_DEVICE },
	{ USB_DEVICE(0x04e2, 0x1402), .driver_info = IGNORE_DEVICE },
	{ USB_DEVICE(0x04e2, 0x1403), .driver_info = IGNORE_DEVICE },
	{ USB_DEVICE(0x04e2, 0x1410), .driver_info = IGNORE_DEVICE },
	{ USB_DEVICE(0x04e2, 0x1411), .driver_info = IGNORE_DEVICE },
	{ USB_DEVICE(0x04e2, 0x1412), .driver_info = IGNORE_DEVICE },
	{ USB_DEVICE(0x04e2, 0x1414), .driver_info = IGNORE_DEVICE },
	{ USB_DEVICE(0x04e2, 0x1420), .driver_info = IGNORE_DEVICE },
	{ USB_DEVICE(0x04e2, 0x1422), .driver_info = IGNORE_DEVICE },
	{ USB_DEVICE(0x04e2, 0x1424), .driver_info = IGNORE_DEVICE },
#endif

	/*Samsung phone in firmware update mode */
	{ USB_DEVICE(0x04e8, 0x685d),
	.driver_info = IGNORE_DEVICE,
	},

	/* Exclude Infineon Flash Loader utility */
	{ USB_DEVICE(0x058b, 0x0041),
	.driver_info = IGNORE_DEVICE,
	},

	/* Exclude ETAS ES58x */
	{ USB_DEVICE(0x108c, 0x0159), /* ES581.4 */
	.driver_info = IGNORE_DEVICE,
	},
	{ USB_DEVICE(0x108c, 0x0168), /* ES582.1 */
	.driver_info = IGNORE_DEVICE,
	},
	{ USB_DEVICE(0x108c, 0x0169), /* ES584.1 */
	.driver_info = IGNORE_DEVICE,
	},

	{ USB_DEVICE(0x1bc7, 0x0021), /* Telit 3G ACM only composition */
	.driver_info = SEND_ZERO_PACKET,
	},
	{ USB_DEVICE(0x1bc7, 0x0023), /* Telit 3G ACM + ECM composition */
	.driver_info = SEND_ZERO_PACKET,
	},

	/* Exclude Goodix Fingerprint Reader */
	{ USB_DEVICE(0x27c6, 0x5395),
	.driver_info = IGNORE_DEVICE,
	},

	/* Exclude Heimann Sensor GmbH USB appset demo */
	{ USB_DEVICE(0x32a7, 0x0000),
	.driver_info = IGNORE_DEVICE,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},

	/* control interfaces without any protocol set */
	{ USB_INTERFACE_INFO(USB_CLASS_COMM, USB_CDC_SUBCLASS_ACM,
		USB_CDC_PROTO_NONE) },

	/* control interfaces with various AT-command sets */
	{ USB_INTERFACE_INFO(USB_CLASS_COMM, USB_CDC_SUBCLASS_ACM,
		USB_CDC_ACM_PROTO_AT_V25TER) },
	{ USB_INTERFACE_INFO(USB_CLASS_COMM, USB_CDC_SUBCLASS_ACM,
		USB_CDC_ACM_PROTO_AT_PCCA101) },
	{ USB_INTERFACE_INFO(USB_CLASS_COMM, USB_CDC_SUBCLASS_ACM,
		USB_CDC_ACM_PROTO_AT_PCCA101_WAKE) },
	{ USB_INTERFACE_INFO(USB_CLASS_COMM, USB_CDC_SUBCLASS_ACM,
		USB_CDC_ACM_PROTO_AT_GSM) },
	{ USB_INTERFACE_INFO(USB_CLASS_COMM, USB_CDC_SUBCLASS_ACM,
		USB_CDC_ACM_PROTO_AT_3G) },
	{ USB_INTERFACE_INFO(USB_CLASS_COMM, USB_CDC_SUBCLASS_ACM,
		USB_CDC_ACM_PROTO_AT_CDMA) },

<<<<<<< HEAD
=======
	{ USB_DEVICE(0x1519, 0x0452), /* Intel 7260 modem */
	.driver_info = SEND_ZERO_PACKET,
	},

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{ }
};

MODULE_DEVICE_TABLE(usb, acm_ids);

static struct usb_driver acm_driver = {
	.name =		"cdc_acm",
	.probe =	acm_probe,
	.disconnect =	acm_disconnect,
#ifdef CONFIG_PM
	.suspend =	acm_suspend,
	.resume =	acm_resume,
	.reset_resume =	acm_reset_resume,
#endif
<<<<<<< HEAD
=======
	.pre_reset =	acm_pre_reset,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.id_table =	acm_ids,
#ifdef CONFIG_PM
	.supports_autosuspend = 1,
#endif
<<<<<<< HEAD
=======
	.disable_hub_initiated_lpm = 1,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/*
 * TTY driver structures.
 */

static const struct tty_operations acm_ops = {
	.install =		acm_tty_install,
	.open =			acm_tty_open,
	.close =		acm_tty_close,
	.cleanup =		acm_tty_cleanup,
	.hangup =		acm_tty_hangup,
	.write =		acm_tty_write,
	.write_room =		acm_tty_write_room,
<<<<<<< HEAD
=======
	.flush_buffer =		acm_tty_flush_buffer,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.ioctl =		acm_tty_ioctl,
	.throttle =		acm_tty_throttle,
	.unthrottle =		acm_tty_unthrottle,
	.chars_in_buffer =	acm_tty_chars_in_buffer,
	.break_ctl =		acm_tty_break_ctl,
	.set_termios =		acm_tty_set_termios,
	.tiocmget =		acm_tty_tiocmget,
	.tiocmset =		acm_tty_tiocmset,
<<<<<<< HEAD
=======
	.get_serial =		get_serial_info,
	.set_serial =		set_serial_info,
	.get_icount =		acm_tty_get_icount,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/*
 * Init / exit.
 */

static int __init acm_init(void)
{
	int retval;
<<<<<<< HEAD
	acm_tty_driver = alloc_tty_driver(ACM_TTY_MINORS);
	if (!acm_tty_driver)
		return -ENOMEM;
=======
	acm_tty_driver = tty_alloc_driver(ACM_TTY_MINORS, TTY_DRIVER_REAL_RAW |
			TTY_DRIVER_DYNAMIC_DEV);
	if (IS_ERR(acm_tty_driver))
		return PTR_ERR(acm_tty_driver);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	acm_tty_driver->driver_name = "acm",
	acm_tty_driver->name = "ttyACM",
	acm_tty_driver->major = ACM_TTY_MAJOR,
	acm_tty_driver->minor_start = 0,
	acm_tty_driver->type = TTY_DRIVER_TYPE_SERIAL,
	acm_tty_driver->subtype = SERIAL_TYPE_NORMAL,
<<<<<<< HEAD
	acm_tty_driver->flags = TTY_DRIVER_REAL_RAW | TTY_DRIVER_DYNAMIC_DEV;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	acm_tty_driver->init_termios = tty_std_termios;
	acm_tty_driver->init_termios.c_cflag = B9600 | CS8 | CREAD |
								HUPCL | CLOCAL;
	tty_set_operations(acm_tty_driver, &acm_ops);

	retval = tty_register_driver(acm_tty_driver);
	if (retval) {
<<<<<<< HEAD
		put_tty_driver(acm_tty_driver);
=======
		tty_driver_kref_put(acm_tty_driver);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return retval;
	}

	retval = usb_register(&acm_driver);
	if (retval) {
		tty_unregister_driver(acm_tty_driver);
<<<<<<< HEAD
		put_tty_driver(acm_tty_driver);
=======
		tty_driver_kref_put(acm_tty_driver);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return retval;
	}

	printk(KERN_INFO KBUILD_MODNAME ": " DRIVER_DESC "\n");

	return 0;
}

static void __exit acm_exit(void)
{
	usb_deregister(&acm_driver);
	tty_unregister_driver(acm_tty_driver);
<<<<<<< HEAD
	put_tty_driver(acm_tty_driver);
=======
	tty_driver_kref_put(acm_tty_driver);
	idr_destroy(&acm_minors);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

module_init(acm_init);
module_exit(acm_exit);

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");
MODULE_ALIAS_CHARDEV_MAJOR(ACM_TTY_MAJOR);
