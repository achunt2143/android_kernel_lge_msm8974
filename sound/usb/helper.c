<<<<<<< HEAD
/*
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
// SPDX-License-Identifier: GPL-2.0-or-later
/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/init.h>
#include <linux/slab.h>
#include <linux/usb.h>

#include "usbaudio.h"
#include "helper.h"
<<<<<<< HEAD
=======
#include "quirks.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * combine bytes and get an integer value
 */
unsigned int snd_usb_combine_bytes(unsigned char *bytes, int size)
{
	switch (size) {
	case 1:  return *bytes;
	case 2:  return combine_word(bytes);
	case 3:  return combine_triple(bytes);
	case 4:  return combine_quad(bytes);
	default: return 0;
	}
}

/*
 * parse descriptor buffer and return the pointer starting the given
 * descriptor type.
 */
void *snd_usb_find_desc(void *descstart, int desclen, void *after, u8 dtype)
{
	u8 *p, *end, *next;

	p = descstart;
	end = p + desclen;
	for (; p < end;) {
		if (p[0] < 2)
			return NULL;
		next = p + p[0];
		if (next > end)
			return NULL;
		if (p[1] == dtype && (!after || (void *)p > after)) {
			return p;
		}
		p = next;
	}
	return NULL;
}

/*
 * find a class-specified interface descriptor with the given subtype.
 */
void *snd_usb_find_csint_desc(void *buffer, int buflen, void *after, u8 dsubtype)
{
	unsigned char *p = after;

	while ((p = snd_usb_find_desc(buffer, buflen, p,
				      USB_DT_CS_INTERFACE)) != NULL) {
		if (p[0] >= 3 && p[2] == dsubtype)
			return p;
	}
	return NULL;
}

/*
 * Wrapper for usb_control_msg().
 * Allocates a temp buffer to prevent dmaing from/to the stack.
 */
int snd_usb_ctl_msg(struct usb_device *dev, unsigned int pipe, __u8 request,
		    __u8 requesttype, __u16 value, __u16 index, void *data,
		    __u16 size)
{
	int err;
	void *buf = NULL;
<<<<<<< HEAD
=======
	int timeout;

	if (usb_pipe_type_check(dev, pipe))
		return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (size > 0) {
		buf = kmemdup(data, size, GFP_KERNEL);
		if (!buf)
			return -ENOMEM;
	}
<<<<<<< HEAD
	err = usb_control_msg(dev, pipe, request, requesttype,
			      value, index, buf, size, 1000);
=======

	if (requesttype & USB_DIR_IN)
		timeout = USB_CTRL_GET_TIMEOUT;
	else
		timeout = USB_CTRL_SET_TIMEOUT;

	err = usb_control_msg(dev, pipe, request, requesttype,
			      value, index, buf, size, timeout);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (size > 0) {
		memcpy(data, buf, size);
		kfree(buf);
	}
<<<<<<< HEAD
=======

	snd_usb_ctl_msg_quirk(dev, pipe, request, requesttype,
			      value, index, data, size);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

unsigned char snd_usb_parse_datainterval(struct snd_usb_audio *chip,
					 struct usb_host_interface *alts)
{
	switch (snd_usb_get_speed(chip->dev)) {
	case USB_SPEED_HIGH:
	case USB_SPEED_SUPER:
<<<<<<< HEAD
=======
	case USB_SPEED_SUPER_PLUS:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (get_endpoint(alts, 0)->bInterval >= 1 &&
		    get_endpoint(alts, 0)->bInterval <= 4)
			return get_endpoint(alts, 0)->bInterval - 1;
		break;
	default:
		break;
	}
	return 0;
}

<<<<<<< HEAD
=======
struct usb_host_interface *
snd_usb_get_host_interface(struct snd_usb_audio *chip, int ifnum, int altsetting)
{
	struct usb_interface *iface;

	iface = usb_ifnum_to_if(chip->dev, ifnum);
	if (!iface)
		return NULL;
	return usb_altnum_to_altsetting(iface, altsetting);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
