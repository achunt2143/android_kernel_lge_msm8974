<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Released under the GPLv2 only.
 */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/usb.h>
#include <linux/usb/ch9.h>
#include <linux/usb/hcd.h>
#include <linux/usb/quirks.h>
#include <linux/module.h>
<<<<<<< HEAD
#include <linux/init.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/slab.h>
#include <linux/device.h>
#include <asm/byteorder.h>
#include "usb.h"


#define USB_MAXALTSETTING		128	/* Hard limit */
<<<<<<< HEAD
#define USB_MAXENDPOINTS		30	/* Hard limit */
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define USB_MAXCONFIG			8	/* Arbitrary limit */


static inline const char *plural(int n)
{
	return (n == 1 ? "" : "s");
}

static int find_next_descriptor(unsigned char *buffer, int size,
    int dt1, int dt2, int *num_skipped)
{
	struct usb_descriptor_header *h;
	int n = 0;
	unsigned char *buffer0 = buffer;

	/* Find the next descriptor of type dt1 or dt2 */
	while (size > 0) {
		h = (struct usb_descriptor_header *) buffer;
		if (h->bDescriptorType == dt1 || h->bDescriptorType == dt2)
			break;
		buffer += h->bLength;
		size -= h->bLength;
		++n;
	}

	/* Store the number of descriptors skipped and return the
	 * number of bytes skipped */
	if (num_skipped)
		*num_skipped = n;
	return buffer - buffer0;
}

<<<<<<< HEAD
=======
static void usb_parse_ssp_isoc_endpoint_companion(struct device *ddev,
		int cfgno, int inum, int asnum, struct usb_host_endpoint *ep,
		unsigned char *buffer, int size)
{
	struct usb_ssp_isoc_ep_comp_descriptor *desc;

	/*
	 * The SuperSpeedPlus Isoc endpoint companion descriptor immediately
	 * follows the SuperSpeed Endpoint Companion descriptor
	 */
	desc = (struct usb_ssp_isoc_ep_comp_descriptor *) buffer;
	if (desc->bDescriptorType != USB_DT_SSP_ISOC_ENDPOINT_COMP ||
	    size < USB_DT_SSP_ISOC_EP_COMP_SIZE) {
		dev_notice(ddev, "Invalid SuperSpeedPlus isoc endpoint companion"
			 "for config %d interface %d altsetting %d ep %d.\n",
			 cfgno, inum, asnum, ep->desc.bEndpointAddress);
		return;
	}
	memcpy(&ep->ssp_isoc_ep_comp, desc, USB_DT_SSP_ISOC_EP_COMP_SIZE);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void usb_parse_ss_endpoint_companion(struct device *ddev, int cfgno,
		int inum, int asnum, struct usb_host_endpoint *ep,
		unsigned char *buffer, int size)
{
	struct usb_ss_ep_comp_descriptor *desc;
	int max_tx;

	/* The SuperSpeed endpoint companion descriptor is supposed to
	 * be the first thing immediately following the endpoint descriptor.
	 */
	desc = (struct usb_ss_ep_comp_descriptor *) buffer;
<<<<<<< HEAD
	if (desc->bDescriptorType != USB_DT_SS_ENDPOINT_COMP ||
			size < USB_DT_SS_EP_COMP_SIZE) {
		dev_warn(ddev, "No SuperSpeed endpoint companion for config %d "
=======

	if (desc->bDescriptorType != USB_DT_SS_ENDPOINT_COMP ||
			size < USB_DT_SS_EP_COMP_SIZE) {
		dev_notice(ddev, "No SuperSpeed endpoint companion for config %d "
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				" interface %d altsetting %d ep %d: "
				"using minimum values\n",
				cfgno, inum, asnum, ep->desc.bEndpointAddress);

		/* Fill in some default values.
		 * Leave bmAttributes as zero, which will mean no streams for
		 * bulk, and isoc won't support multiple bursts of packets.
		 * With bursts of only one packet, and a Mult of 1, the max
		 * amount of data moved per endpoint service interval is one
		 * packet.
		 */
		ep->ss_ep_comp.bLength = USB_DT_SS_EP_COMP_SIZE;
		ep->ss_ep_comp.bDescriptorType = USB_DT_SS_ENDPOINT_COMP;
		if (usb_endpoint_xfer_isoc(&ep->desc) ||
				usb_endpoint_xfer_int(&ep->desc))
			ep->ss_ep_comp.wBytesPerInterval =
					ep->desc.wMaxPacketSize;
		return;
	}
<<<<<<< HEAD

=======
	buffer += desc->bLength;
	size -= desc->bLength;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	memcpy(&ep->ss_ep_comp, desc, USB_DT_SS_EP_COMP_SIZE);

	/* Check the various values */
	if (usb_endpoint_xfer_control(&ep->desc) && desc->bMaxBurst != 0) {
<<<<<<< HEAD
		dev_warn(ddev, "Control endpoint with bMaxBurst = %d in "
=======
		dev_notice(ddev, "Control endpoint with bMaxBurst = %d in "
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				"config %d interface %d altsetting %d ep %d: "
				"setting to zero\n", desc->bMaxBurst,
				cfgno, inum, asnum, ep->desc.bEndpointAddress);
		ep->ss_ep_comp.bMaxBurst = 0;
	} else if (desc->bMaxBurst > 15) {
<<<<<<< HEAD
		dev_warn(ddev, "Endpoint with bMaxBurst = %d in "
=======
		dev_notice(ddev, "Endpoint with bMaxBurst = %d in "
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				"config %d interface %d altsetting %d ep %d: "
				"setting to 15\n", desc->bMaxBurst,
				cfgno, inum, asnum, ep->desc.bEndpointAddress);
		ep->ss_ep_comp.bMaxBurst = 15;
	}

	if ((usb_endpoint_xfer_control(&ep->desc) ||
			usb_endpoint_xfer_int(&ep->desc)) &&
				desc->bmAttributes != 0) {
<<<<<<< HEAD
		dev_warn(ddev, "%s endpoint with bmAttributes = %d in "
=======
		dev_notice(ddev, "%s endpoint with bmAttributes = %d in "
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				"config %d interface %d altsetting %d ep %d: "
				"setting to zero\n",
				usb_endpoint_xfer_control(&ep->desc) ? "Control" : "Bulk",
				desc->bmAttributes,
				cfgno, inum, asnum, ep->desc.bEndpointAddress);
		ep->ss_ep_comp.bmAttributes = 0;
	} else if (usb_endpoint_xfer_bulk(&ep->desc) &&
			desc->bmAttributes > 16) {
<<<<<<< HEAD
		dev_warn(ddev, "Bulk endpoint with more than 65536 streams in "
=======
		dev_notice(ddev, "Bulk endpoint with more than 65536 streams in "
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				"config %d interface %d altsetting %d ep %d: "
				"setting to max\n",
				cfgno, inum, asnum, ep->desc.bEndpointAddress);
		ep->ss_ep_comp.bmAttributes = 16;
	} else if (usb_endpoint_xfer_isoc(&ep->desc) &&
<<<<<<< HEAD
		   USB_SS_MULT(desc->bmAttributes) > 3) {
		dev_warn(ddev, "Isoc endpoint has Mult of %d in "
=======
		   !USB_SS_SSP_ISOC_COMP(desc->bmAttributes) &&
		   USB_SS_MULT(desc->bmAttributes) > 3) {
		dev_notice(ddev, "Isoc endpoint has Mult of %d in "
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				"config %d interface %d altsetting %d ep %d: "
				"setting to 3\n",
				USB_SS_MULT(desc->bmAttributes),
				cfgno, inum, asnum, ep->desc.bEndpointAddress);
		ep->ss_ep_comp.bmAttributes = 2;
	}

	if (usb_endpoint_xfer_isoc(&ep->desc))
		max_tx = (desc->bMaxBurst + 1) *
			(USB_SS_MULT(desc->bmAttributes)) *
			usb_endpoint_maxp(&ep->desc);
	else if (usb_endpoint_xfer_int(&ep->desc))
		max_tx = usb_endpoint_maxp(&ep->desc) *
			(desc->bMaxBurst + 1);
	else
		max_tx = 999999;
	if (le16_to_cpu(desc->wBytesPerInterval) > max_tx) {
<<<<<<< HEAD
		dev_warn(ddev, "%s endpoint with wBytesPerInterval of %d in "
=======
		dev_notice(ddev, "%s endpoint with wBytesPerInterval of %d in "
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				"config %d interface %d altsetting %d ep %d: "
				"setting to %d\n",
				usb_endpoint_xfer_isoc(&ep->desc) ? "Isoc" : "Int",
				le16_to_cpu(desc->wBytesPerInterval),
				cfgno, inum, asnum, ep->desc.bEndpointAddress,
				max_tx);
		ep->ss_ep_comp.wBytesPerInterval = cpu_to_le16(max_tx);
	}
<<<<<<< HEAD
}

static int usb_parse_endpoint(struct device *ddev, int cfgno, int inum,
    int asnum, struct usb_host_interface *ifp, int num_ep,
    unsigned char *buffer, int size)
{
=======
	/* Parse a possible SuperSpeedPlus isoc ep companion descriptor */
	if (usb_endpoint_xfer_isoc(&ep->desc) &&
	    USB_SS_SSP_ISOC_COMP(desc->bmAttributes))
		usb_parse_ssp_isoc_endpoint_companion(ddev, cfgno, inum, asnum,
							ep, buffer, size);
}

static const unsigned short low_speed_maxpacket_maxes[4] = {
	[USB_ENDPOINT_XFER_CONTROL] = 8,
	[USB_ENDPOINT_XFER_ISOC] = 0,
	[USB_ENDPOINT_XFER_BULK] = 0,
	[USB_ENDPOINT_XFER_INT] = 8,
};
static const unsigned short full_speed_maxpacket_maxes[4] = {
	[USB_ENDPOINT_XFER_CONTROL] = 64,
	[USB_ENDPOINT_XFER_ISOC] = 1023,
	[USB_ENDPOINT_XFER_BULK] = 64,
	[USB_ENDPOINT_XFER_INT] = 64,
};
static const unsigned short high_speed_maxpacket_maxes[4] = {
	[USB_ENDPOINT_XFER_CONTROL] = 64,
	[USB_ENDPOINT_XFER_ISOC] = 1024,

	/* Bulk should be 512, but some devices use 1024: we will warn below */
	[USB_ENDPOINT_XFER_BULK] = 1024,
	[USB_ENDPOINT_XFER_INT] = 1024,
};
static const unsigned short super_speed_maxpacket_maxes[4] = {
	[USB_ENDPOINT_XFER_CONTROL] = 512,
	[USB_ENDPOINT_XFER_ISOC] = 1024,
	[USB_ENDPOINT_XFER_BULK] = 1024,
	[USB_ENDPOINT_XFER_INT] = 1024,
};

static bool endpoint_is_duplicate(struct usb_endpoint_descriptor *e1,
		struct usb_endpoint_descriptor *e2)
{
	if (e1->bEndpointAddress == e2->bEndpointAddress)
		return true;

	if (usb_endpoint_xfer_control(e1) || usb_endpoint_xfer_control(e2)) {
		if (usb_endpoint_num(e1) == usb_endpoint_num(e2))
			return true;
	}

	return false;
}

/*
 * Check for duplicate endpoint addresses in other interfaces and in the
 * altsetting currently being parsed.
 */
static bool config_endpoint_is_duplicate(struct usb_host_config *config,
		int inum, int asnum, struct usb_endpoint_descriptor *d)
{
	struct usb_endpoint_descriptor *epd;
	struct usb_interface_cache *intfc;
	struct usb_host_interface *alt;
	int i, j, k;

	for (i = 0; i < config->desc.bNumInterfaces; ++i) {
		intfc = config->intf_cache[i];

		for (j = 0; j < intfc->num_altsetting; ++j) {
			alt = &intfc->altsetting[j];

			if (alt->desc.bInterfaceNumber == inum &&
					alt->desc.bAlternateSetting != asnum)
				continue;

			for (k = 0; k < alt->desc.bNumEndpoints; ++k) {
				epd = &alt->endpoint[k].desc;

				if (endpoint_is_duplicate(epd, d))
					return true;
			}
		}
	}

	return false;
}

static int usb_parse_endpoint(struct device *ddev, int cfgno,
		struct usb_host_config *config, int inum, int asnum,
		struct usb_host_interface *ifp, int num_ep,
		unsigned char *buffer, int size)
{
	struct usb_device *udev = to_usb_device(ddev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned char *buffer0 = buffer;
	struct usb_endpoint_descriptor *d;
	struct usb_host_endpoint *endpoint;
	int n, i, j, retval;
<<<<<<< HEAD
=======
	unsigned int maxp;
	const unsigned short *maxpacket_maxes;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	d = (struct usb_endpoint_descriptor *) buffer;
	buffer += d->bLength;
	size -= d->bLength;

	if (d->bLength >= USB_DT_ENDPOINT_AUDIO_SIZE)
		n = USB_DT_ENDPOINT_AUDIO_SIZE;
	else if (d->bLength >= USB_DT_ENDPOINT_SIZE)
		n = USB_DT_ENDPOINT_SIZE;
	else {
<<<<<<< HEAD
		dev_warn(ddev, "config %d interface %d altsetting %d has an "
=======
		dev_notice(ddev, "config %d interface %d altsetting %d has an "
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		    "invalid endpoint descriptor of length %d, skipping\n",
		    cfgno, inum, asnum, d->bLength);
		goto skip_to_next_endpoint_or_interface_descriptor;
	}

	i = d->bEndpointAddress & ~USB_ENDPOINT_DIR_MASK;
	if (i >= 16 || i == 0) {
<<<<<<< HEAD
		dev_warn(ddev, "config %d interface %d altsetting %d has an "
=======
		dev_notice(ddev, "config %d interface %d altsetting %d has an "
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		    "invalid endpoint with address 0x%X, skipping\n",
		    cfgno, inum, asnum, d->bEndpointAddress);
		goto skip_to_next_endpoint_or_interface_descriptor;
	}

	/* Only store as many endpoints as we have room for */
	if (ifp->desc.bNumEndpoints >= num_ep)
		goto skip_to_next_endpoint_or_interface_descriptor;

<<<<<<< HEAD
=======
	/* Check for duplicate endpoint addresses */
	if (config_endpoint_is_duplicate(config, inum, asnum, d)) {
		dev_notice(ddev, "config %d interface %d altsetting %d has a duplicate endpoint with address 0x%X, skipping\n",
				cfgno, inum, asnum, d->bEndpointAddress);
		goto skip_to_next_endpoint_or_interface_descriptor;
	}

	/* Ignore some endpoints */
	if (udev->quirks & USB_QUIRK_ENDPOINT_IGNORE) {
		if (usb_endpoint_is_ignored(udev, ifp, d)) {
			dev_notice(ddev, "config %d interface %d altsetting %d has an ignored endpoint with address 0x%X, skipping\n",
					cfgno, inum, asnum,
					d->bEndpointAddress);
			goto skip_to_next_endpoint_or_interface_descriptor;
		}
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	endpoint = &ifp->endpoint[ifp->desc.bNumEndpoints];
	++ifp->desc.bNumEndpoints;

	memcpy(&endpoint->desc, d, n);
	INIT_LIST_HEAD(&endpoint->urb_list);

<<<<<<< HEAD
	/* Fix up bInterval values outside the legal range. Use 32 ms if no
	 * proper value can be guessed. */
=======
	/*
	 * Fix up bInterval values outside the legal range.
	 * Use 10 or 8 ms if no proper value can be guessed.
	 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	i = 0;		/* i = min, j = max, n = default */
	j = 255;
	if (usb_endpoint_xfer_int(d)) {
		i = 1;
<<<<<<< HEAD
		switch (to_usb_device(ddev)->speed) {
		case USB_SPEED_SUPER:
		case USB_SPEED_HIGH:
			/* Many device manufacturers are using full-speed
			 * bInterval values in high-speed interrupt endpoint
			 * descriptors. Try to fix those and fall back to a
			 * 32 ms default value otherwise. */
			n = fls(d->bInterval*8);
			if (n == 0)
				n = 9;	/* 32 ms = 2^(9-1) uframes */
=======
		switch (udev->speed) {
		case USB_SPEED_SUPER_PLUS:
		case USB_SPEED_SUPER:
		case USB_SPEED_HIGH:
			/*
			 * Many device manufacturers are using full-speed
			 * bInterval values in high-speed interrupt endpoint
			 * descriptors. Try to fix those and fall back to an
			 * 8-ms default value otherwise.
			 */
			n = fls(d->bInterval*8);
			if (n == 0)
				n = 7;	/* 8 ms = 2^(7-1) uframes */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			j = 16;

			/*
			 * Adjust bInterval for quirked devices.
<<<<<<< HEAD
			 * This quirk fixes bIntervals reported in
			 * linear microframes.
			 */
			if (to_usb_device(ddev)->quirks &
				USB_QUIRK_LINEAR_UFRAME_INTR_BINTERVAL) {
=======
			 */
			/*
			 * This quirk fixes bIntervals reported in ms.
			 */
			if (udev->quirks & USB_QUIRK_LINEAR_FRAME_INTR_BINTERVAL) {
				n = clamp(fls(d->bInterval) + 3, i, j);
				i = j = n;
			}
			/*
			 * This quirk fixes bIntervals reported in
			 * linear microframes.
			 */
			if (udev->quirks & USB_QUIRK_LINEAR_UFRAME_INTR_BINTERVAL) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				n = clamp(fls(d->bInterval), i, j);
				i = j = n;
			}
			break;
		default:		/* USB_SPEED_FULL or _LOW */
<<<<<<< HEAD
			/* For low-speed, 10 ms is the official minimum.
			 * But some "overclocked" devices might want faster
			 * polling so we'll allow it. */
			n = 32;
=======
			/*
			 * For low-speed, 10 ms is the official minimum.
			 * But some "overclocked" devices might want faster
			 * polling so we'll allow it.
			 */
			n = 10;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		}
	} else if (usb_endpoint_xfer_isoc(d)) {
		i = 1;
		j = 16;
<<<<<<< HEAD
		switch (to_usb_device(ddev)->speed) {
		case USB_SPEED_HIGH:
			n = 9;		/* 32 ms = 2^(9-1) uframes */
			break;
		default:		/* USB_SPEED_FULL */
			n = 6;		/* 32 ms = 2^(6-1) frames */
=======
		switch (udev->speed) {
		case USB_SPEED_HIGH:
			n = 7;		/* 8 ms = 2^(7-1) uframes */
			break;
		default:		/* USB_SPEED_FULL */
			n = 4;		/* 8 ms = 2^(4-1) frames */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		}
	}
	if (d->bInterval < i || d->bInterval > j) {
<<<<<<< HEAD
		dev_warn(ddev, "config %d interface %d altsetting %d "
=======
		dev_notice(ddev, "config %d interface %d altsetting %d "
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		    "endpoint 0x%X has an invalid bInterval %d, "
		    "changing to %d\n",
		    cfgno, inum, asnum,
		    d->bEndpointAddress, d->bInterval, n);
		endpoint->desc.bInterval = n;
	}

	/* Some buggy low-speed devices have Bulk endpoints, which is
	 * explicitly forbidden by the USB spec.  In an attempt to make
	 * them usable, we will try treating them as Interrupt endpoints.
	 */
<<<<<<< HEAD
	if (to_usb_device(ddev)->speed == USB_SPEED_LOW &&
			usb_endpoint_xfer_bulk(d)) {
		dev_warn(ddev, "config %d interface %d altsetting %d "
=======
	if (udev->speed == USB_SPEED_LOW && usb_endpoint_xfer_bulk(d)) {
		dev_notice(ddev, "config %d interface %d altsetting %d "
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		    "endpoint 0x%X is Bulk; changing to Interrupt\n",
		    cfgno, inum, asnum, d->bEndpointAddress);
		endpoint->desc.bmAttributes = USB_ENDPOINT_XFER_INT;
		endpoint->desc.bInterval = 1;
		if (usb_endpoint_maxp(&endpoint->desc) > 8)
			endpoint->desc.wMaxPacketSize = cpu_to_le16(8);
	}

	/*
<<<<<<< HEAD
=======
	 * Validate the wMaxPacketSize field.
	 * Some devices have isochronous endpoints in altsetting 0;
	 * the USB-2 spec requires such endpoints to have wMaxPacketSize = 0
	 * (see the end of section 5.6.3), so don't warn about them.
	 */
	maxp = le16_to_cpu(endpoint->desc.wMaxPacketSize);
	if (maxp == 0 && !(usb_endpoint_xfer_isoc(d) && asnum == 0)) {
		dev_notice(ddev, "config %d interface %d altsetting %d endpoint 0x%X has invalid wMaxPacketSize 0\n",
		    cfgno, inum, asnum, d->bEndpointAddress);
	}

	/* Find the highest legal maxpacket size for this endpoint */
	i = 0;		/* additional transactions per microframe */
	switch (udev->speed) {
	case USB_SPEED_LOW:
		maxpacket_maxes = low_speed_maxpacket_maxes;
		break;
	case USB_SPEED_FULL:
		maxpacket_maxes = full_speed_maxpacket_maxes;
		break;
	case USB_SPEED_HIGH:
		/* Multiple-transactions bits are allowed only for HS periodic endpoints */
		if (usb_endpoint_xfer_int(d) || usb_endpoint_xfer_isoc(d)) {
			i = maxp & USB_EP_MAXP_MULT_MASK;
			maxp &= ~i;
		}
		fallthrough;
	default:
		maxpacket_maxes = high_speed_maxpacket_maxes;
		break;
	case USB_SPEED_SUPER:
	case USB_SPEED_SUPER_PLUS:
		maxpacket_maxes = super_speed_maxpacket_maxes;
		break;
	}
	j = maxpacket_maxes[usb_endpoint_type(&endpoint->desc)];

	if (maxp > j) {
		dev_notice(ddev, "config %d interface %d altsetting %d endpoint 0x%X has invalid maxpacket %d, setting to %d\n",
		    cfgno, inum, asnum, d->bEndpointAddress, maxp, j);
		maxp = j;
		endpoint->desc.wMaxPacketSize = cpu_to_le16(i | maxp);
	}

	/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * Some buggy high speed devices have bulk endpoints using
	 * maxpacket sizes other than 512.  High speed HCDs may not
	 * be able to handle that particular bug, so let's warn...
	 */
<<<<<<< HEAD
	if (to_usb_device(ddev)->speed == USB_SPEED_HIGH
			&& usb_endpoint_xfer_bulk(d)) {
		unsigned maxp;

		maxp = usb_endpoint_maxp(&endpoint->desc) & 0x07ff;
		if (maxp != 512)
			dev_warn(ddev, "config %d interface %d altsetting %d "
=======
	if (udev->speed == USB_SPEED_HIGH && usb_endpoint_xfer_bulk(d)) {
		if (maxp != 512)
			dev_notice(ddev, "config %d interface %d altsetting %d "
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				"bulk endpoint 0x%X has invalid maxpacket %d\n",
				cfgno, inum, asnum, d->bEndpointAddress,
				maxp);
	}

	/* Parse a possible SuperSpeed endpoint companion descriptor */
<<<<<<< HEAD
	if (to_usb_device(ddev)->speed == USB_SPEED_SUPER)
=======
	if (udev->speed >= USB_SPEED_SUPER)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		usb_parse_ss_endpoint_companion(ddev, cfgno,
				inum, asnum, endpoint, buffer, size);

	/* Skip over any Class Specific or Vendor Specific descriptors;
	 * find the next endpoint or interface descriptor */
	endpoint->extra = buffer;
	i = find_next_descriptor(buffer, size, USB_DT_ENDPOINT,
			USB_DT_INTERFACE, &n);
	endpoint->extralen = i;
	retval = buffer - buffer0 + i;
	if (n > 0)
		dev_dbg(ddev, "skipped %d descriptor%s after %s\n",
		    n, plural(n), "endpoint");
	return retval;

skip_to_next_endpoint_or_interface_descriptor:
	i = find_next_descriptor(buffer, size, USB_DT_ENDPOINT,
	    USB_DT_INTERFACE, NULL);
	return buffer - buffer0 + i;
}

void usb_release_interface_cache(struct kref *ref)
{
	struct usb_interface_cache *intfc = ref_to_usb_interface_cache(ref);
	int j;

	for (j = 0; j < intfc->num_altsetting; j++) {
		struct usb_host_interface *alt = &intfc->altsetting[j];

		kfree(alt->endpoint);
		kfree(alt->string);
	}
	kfree(intfc);
}

static int usb_parse_interface(struct device *ddev, int cfgno,
    struct usb_host_config *config, unsigned char *buffer, int size,
    u8 inums[], u8 nalts[])
{
	unsigned char *buffer0 = buffer;
	struct usb_interface_descriptor	*d;
	int inum, asnum;
	struct usb_interface_cache *intfc;
	struct usb_host_interface *alt;
	int i, n;
	int len, retval;
	int num_ep, num_ep_orig;

	d = (struct usb_interface_descriptor *) buffer;
	buffer += d->bLength;
	size -= d->bLength;

	if (d->bLength < USB_DT_INTERFACE_SIZE)
		goto skip_to_next_interface_descriptor;

	/* Which interface entry is this? */
	intfc = NULL;
	inum = d->bInterfaceNumber;
	for (i = 0; i < config->desc.bNumInterfaces; ++i) {
		if (inums[i] == inum) {
			intfc = config->intf_cache[i];
			break;
		}
	}
	if (!intfc || intfc->num_altsetting >= nalts[i])
		goto skip_to_next_interface_descriptor;

	/* Check for duplicate altsetting entries */
	asnum = d->bAlternateSetting;
	for ((i = 0, alt = &intfc->altsetting[0]);
	      i < intfc->num_altsetting;
	     (++i, ++alt)) {
		if (alt->desc.bAlternateSetting == asnum) {
<<<<<<< HEAD
			dev_warn(ddev, "Duplicate descriptor for config %d "
=======
			dev_notice(ddev, "Duplicate descriptor for config %d "
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			    "interface %d altsetting %d, skipping\n",
			    cfgno, inum, asnum);
			goto skip_to_next_interface_descriptor;
		}
	}

	++intfc->num_altsetting;
	memcpy(&alt->desc, d, USB_DT_INTERFACE_SIZE);

	/* Skip over any Class Specific or Vendor Specific descriptors;
	 * find the first endpoint or interface descriptor */
	alt->extra = buffer;
	i = find_next_descriptor(buffer, size, USB_DT_ENDPOINT,
	    USB_DT_INTERFACE, &n);
	alt->extralen = i;
	if (n > 0)
		dev_dbg(ddev, "skipped %d descriptor%s after %s\n",
		    n, plural(n), "interface");
	buffer += i;
	size -= i;

	/* Allocate space for the right(?) number of endpoints */
	num_ep = num_ep_orig = alt->desc.bNumEndpoints;
	alt->desc.bNumEndpoints = 0;		/* Use as a counter */
	if (num_ep > USB_MAXENDPOINTS) {
<<<<<<< HEAD
		dev_warn(ddev, "too many endpoints for config %d interface %d "
=======
		dev_notice(ddev, "too many endpoints for config %d interface %d "
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		    "altsetting %d: %d, using maximum allowed: %d\n",
		    cfgno, inum, asnum, num_ep, USB_MAXENDPOINTS);
		num_ep = USB_MAXENDPOINTS;
	}

	if (num_ep > 0) {
		/* Can't allocate 0 bytes */
		len = sizeof(struct usb_host_endpoint) * num_ep;
		alt->endpoint = kzalloc(len, GFP_KERNEL);
		if (!alt->endpoint)
			return -ENOMEM;
	}

	/* Parse all the endpoint descriptors */
	n = 0;
	while (size > 0) {
		if (((struct usb_descriptor_header *) buffer)->bDescriptorType
		     == USB_DT_INTERFACE)
			break;
<<<<<<< HEAD
		retval = usb_parse_endpoint(ddev, cfgno, inum, asnum, alt,
		    num_ep, buffer, size);
=======
		retval = usb_parse_endpoint(ddev, cfgno, config, inum, asnum,
				alt, num_ep, buffer, size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (retval < 0)
			return retval;
		++n;

		buffer += retval;
		size -= retval;
	}

	if (n != num_ep_orig)
<<<<<<< HEAD
		dev_warn(ddev, "config %d interface %d altsetting %d has %d "
=======
		dev_notice(ddev, "config %d interface %d altsetting %d has %d "
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		    "endpoint descriptor%s, different from the interface "
		    "descriptor's value: %d\n",
		    cfgno, inum, asnum, n, plural(n), num_ep_orig);
	return buffer - buffer0;

skip_to_next_interface_descriptor:
	i = find_next_descriptor(buffer, size, USB_DT_INTERFACE,
	    USB_DT_INTERFACE, NULL);
	return buffer - buffer0 + i;
}

static int usb_parse_configuration(struct usb_device *dev, int cfgidx,
    struct usb_host_config *config, unsigned char *buffer, int size)
{
	struct device *ddev = &dev->dev;
	unsigned char *buffer0 = buffer;
	int cfgno;
	int nintf, nintf_orig;
	int i, j, n;
	struct usb_interface_cache *intfc;
	unsigned char *buffer2;
	int size2;
	struct usb_descriptor_header *header;
<<<<<<< HEAD
	int len, retval;
=======
	int retval;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u8 inums[USB_MAXINTERFACES], nalts[USB_MAXINTERFACES];
	unsigned iad_num = 0;

	memcpy(&config->desc, buffer, USB_DT_CONFIG_SIZE);
	nintf = nintf_orig = config->desc.bNumInterfaces;
	config->desc.bNumInterfaces = 0;	// Adjusted later

	if (config->desc.bDescriptorType != USB_DT_CONFIG ||
	    config->desc.bLength < USB_DT_CONFIG_SIZE ||
	    config->desc.bLength > size) {
<<<<<<< HEAD
		dev_err(ddev, "invalid descriptor for config index %d: "
=======
		dev_notice(ddev, "invalid descriptor for config index %d: "
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		    "type = 0x%X, length = %d\n", cfgidx,
		    config->desc.bDescriptorType, config->desc.bLength);
		return -EINVAL;
	}
	cfgno = config->desc.bConfigurationValue;

	buffer += config->desc.bLength;
	size -= config->desc.bLength;

	if (nintf > USB_MAXINTERFACES) {
<<<<<<< HEAD
		dev_warn(ddev, "config %d has too many interfaces: %d, "
=======
		dev_notice(ddev, "config %d has too many interfaces: %d, "
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		    "using maximum allowed: %d\n",
		    cfgno, nintf, USB_MAXINTERFACES);
		nintf = USB_MAXINTERFACES;
	}

	/* Go through the descriptors, checking their length and counting the
	 * number of altsettings for each interface */
	n = 0;
	for ((buffer2 = buffer, size2 = size);
	      size2 > 0;
	     (buffer2 += header->bLength, size2 -= header->bLength)) {

		if (size2 < sizeof(struct usb_descriptor_header)) {
<<<<<<< HEAD
			dev_warn(ddev, "config %d descriptor has %d excess "
=======
			dev_notice(ddev, "config %d descriptor has %d excess "
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			    "byte%s, ignoring\n",
			    cfgno, size2, plural(size2));
			break;
		}

		header = (struct usb_descriptor_header *) buffer2;
		if ((header->bLength > size2) || (header->bLength < 2)) {
<<<<<<< HEAD
			dev_warn(ddev, "config %d has an invalid descriptor "
=======
			dev_notice(ddev, "config %d has an invalid descriptor "
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			    "of length %d, skipping remainder of the config\n",
			    cfgno, header->bLength);
			break;
		}

		if (header->bDescriptorType == USB_DT_INTERFACE) {
			struct usb_interface_descriptor *d;
			int inum;

			d = (struct usb_interface_descriptor *) header;
			if (d->bLength < USB_DT_INTERFACE_SIZE) {
<<<<<<< HEAD
				dev_warn(ddev, "config %d has an invalid "
=======
				dev_notice(ddev, "config %d has an invalid "
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				    "interface descriptor of length %d, "
				    "skipping\n", cfgno, d->bLength);
				continue;
			}

			inum = d->bInterfaceNumber;

			if ((dev->quirks & USB_QUIRK_HONOR_BNUMINTERFACES) &&
			    n >= nintf_orig) {
<<<<<<< HEAD
				dev_warn(ddev, "config %d has more interface "
=======
				dev_notice(ddev, "config %d has more interface "
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				    "descriptors, than it declares in "
				    "bNumInterfaces, ignoring interface "
				    "number: %d\n", cfgno, inum);
				continue;
			}

			if (inum >= nintf_orig)
<<<<<<< HEAD
				dev_warn(ddev, "config %d has an invalid "
=======
				dev_notice(ddev, "config %d has an invalid "
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				    "interface number: %d but max is %d\n",
				    cfgno, inum, nintf_orig - 1);

			/* Have we already encountered this interface?
			 * Count its altsettings */
			for (i = 0; i < n; ++i) {
				if (inums[i] == inum)
					break;
			}
			if (i < n) {
				if (nalts[i] < 255)
					++nalts[i];
			} else if (n < USB_MAXINTERFACES) {
				inums[n] = inum;
				nalts[n] = 1;
				++n;
			}

		} else if (header->bDescriptorType ==
				USB_DT_INTERFACE_ASSOCIATION) {
			struct usb_interface_assoc_descriptor *d;

			d = (struct usb_interface_assoc_descriptor *)header;
			if (d->bLength < USB_DT_INTERFACE_ASSOCIATION_SIZE) {
<<<<<<< HEAD
				dev_warn(ddev,
=======
				dev_notice(ddev,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					 "config %d has an invalid interface association descriptor of length %d, skipping\n",
					 cfgno, d->bLength);
				continue;
			}

			if (iad_num == USB_MAXIADS) {
<<<<<<< HEAD
				dev_warn(ddev, "found more Interface "
=======
				dev_notice(ddev, "found more Interface "
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					       "Association Descriptors "
					       "than allocated for in "
					       "configuration %d\n", cfgno);
			} else {
				config->intf_assoc[iad_num] = d;
				iad_num++;
			}

		} else if (header->bDescriptorType == USB_DT_DEVICE ||
			    header->bDescriptorType == USB_DT_CONFIG)
<<<<<<< HEAD
			dev_warn(ddev, "config %d contains an unexpected "
=======
			dev_notice(ddev, "config %d contains an unexpected "
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			    "descriptor of type 0x%X, skipping\n",
			    cfgno, header->bDescriptorType);

	}	/* for ((buffer2 = buffer, size2 = size); ...) */
	size = buffer2 - buffer;
	config->desc.wTotalLength = cpu_to_le16(buffer2 - buffer0);

	if (n != nintf)
<<<<<<< HEAD
		dev_warn(ddev, "config %d has %d interface%s, different from "
		    "the descriptor's value: %d\n",
		    cfgno, n, plural(n), nintf_orig);
	else if (n == 0)
		dev_warn(ddev, "config %d has no interfaces?\n", cfgno);
=======
		dev_notice(ddev, "config %d has %d interface%s, different from "
		    "the descriptor's value: %d\n",
		    cfgno, n, plural(n), nintf_orig);
	else if (n == 0)
		dev_notice(ddev, "config %d has no interfaces?\n", cfgno);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	config->desc.bNumInterfaces = nintf = n;

	/* Check for missing interface numbers */
	for (i = 0; i < nintf; ++i) {
		for (j = 0; j < nintf; ++j) {
			if (inums[j] == i)
				break;
		}
		if (j >= nintf)
<<<<<<< HEAD
			dev_warn(ddev, "config %d has no interface number "
=======
			dev_notice(ddev, "config %d has no interface number "
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			    "%d\n", cfgno, i);
	}

	/* Allocate the usb_interface_caches and altsetting arrays */
	for (i = 0; i < nintf; ++i) {
		j = nalts[i];
		if (j > USB_MAXALTSETTING) {
<<<<<<< HEAD
			dev_warn(ddev, "too many alternate settings for "
=======
			dev_notice(ddev, "too many alternate settings for "
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			    "config %d interface %d: %d, "
			    "using maximum allowed: %d\n",
			    cfgno, inums[i], j, USB_MAXALTSETTING);
			nalts[i] = j = USB_MAXALTSETTING;
		}

<<<<<<< HEAD
		len = sizeof(*intfc) + sizeof(struct usb_host_interface) * j;
		config->intf_cache[i] = intfc = kzalloc(len, GFP_KERNEL);
=======
		intfc = kzalloc(struct_size(intfc, altsetting, j), GFP_KERNEL);
		config->intf_cache[i] = intfc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!intfc)
			return -ENOMEM;
		kref_init(&intfc->ref);
	}

	/* FIXME: parse the BOS descriptor */

	/* Skip over any Class Specific or Vendor Specific descriptors;
	 * find the first interface descriptor */
	config->extra = buffer;
	i = find_next_descriptor(buffer, size, USB_DT_INTERFACE,
	    USB_DT_INTERFACE, &n);
	config->extralen = i;
	if (n > 0)
		dev_dbg(ddev, "skipped %d descriptor%s after %s\n",
		    n, plural(n), "configuration");
	buffer += i;
	size -= i;

	/* Parse all the interface/altsetting descriptors */
	while (size > 0) {
		retval = usb_parse_interface(ddev, cfgno, config,
		    buffer, size, inums, nalts);
		if (retval < 0)
			return retval;

		buffer += retval;
		size -= retval;
	}

	/* Check for missing altsettings */
	for (i = 0; i < nintf; ++i) {
		intfc = config->intf_cache[i];
		for (j = 0; j < intfc->num_altsetting; ++j) {
			for (n = 0; n < intfc->num_altsetting; ++n) {
				if (intfc->altsetting[n].desc.
				    bAlternateSetting == j)
					break;
			}
			if (n >= intfc->num_altsetting)
<<<<<<< HEAD
				dev_warn(ddev, "config %d interface %d has no "
=======
				dev_notice(ddev, "config %d interface %d has no "
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				    "altsetting %d\n", cfgno, inums[i], j);
		}
	}

	return 0;
}

/* hub-only!! ... and only exported for reset/reinit path.
 * otherwise used internally on disconnect/destroy path
 */
void usb_destroy_configuration(struct usb_device *dev)
{
	int c, i;

	if (!dev->config)
		return;

	if (dev->rawdescriptors) {
		for (i = 0; i < dev->descriptor.bNumConfigurations; i++)
			kfree(dev->rawdescriptors[i]);

		kfree(dev->rawdescriptors);
		dev->rawdescriptors = NULL;
	}

	for (c = 0; c < dev->descriptor.bNumConfigurations; c++) {
		struct usb_host_config *cf = &dev->config[c];

		kfree(cf->string);
		for (i = 0; i < cf->desc.bNumInterfaces; i++) {
			if (cf->intf_cache[i])
				kref_put(&cf->intf_cache[i]->ref,
					  usb_release_interface_cache);
		}
	}
	kfree(dev->config);
	dev->config = NULL;
}


/*
 * Get the USB config descriptors, cache and parse'em
 *
 * hub-only!! ... and only in reset path, or usb_new_device()
 * (used by real hubs and virtual root hubs)
 */
int usb_get_configuration(struct usb_device *dev)
{
	struct device *ddev = &dev->dev;
	int ncfg = dev->descriptor.bNumConfigurations;
<<<<<<< HEAD
	int result = 0;
	unsigned int cfgno, length;
	unsigned char *bigbuffer;
	struct usb_config_descriptor *desc;

	cfgno = 0;
	result = -ENOMEM;
	if (ncfg > USB_MAXCONFIG) {
		dev_warn(ddev, "too many configurations: %d, "
=======
	unsigned int cfgno, length;
	unsigned char *bigbuffer;
	struct usb_config_descriptor *desc;
	int result;

	if (ncfg > USB_MAXCONFIG) {
		dev_notice(ddev, "too many configurations: %d, "
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		    "using maximum allowed: %d\n", ncfg, USB_MAXCONFIG);
		dev->descriptor.bNumConfigurations = ncfg = USB_MAXCONFIG;
	}

	if (ncfg < 1) {
		dev_err(ddev, "no configurations\n");
		return -EINVAL;
	}

	length = ncfg * sizeof(struct usb_host_config);
	dev->config = kzalloc(length, GFP_KERNEL);
	if (!dev->config)
<<<<<<< HEAD
		goto err2;
=======
		return -ENOMEM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	length = ncfg * sizeof(char *);
	dev->rawdescriptors = kzalloc(length, GFP_KERNEL);
	if (!dev->rawdescriptors)
<<<<<<< HEAD
		goto err2;

	desc = kmalloc(USB_DT_CONFIG_SIZE, GFP_KERNEL);
	if (!desc)
		goto err2;

	result = 0;
	for (; cfgno < ncfg; cfgno++) {
=======
		return -ENOMEM;

	desc = kmalloc(USB_DT_CONFIG_SIZE, GFP_KERNEL);
	if (!desc)
		return -ENOMEM;

	for (cfgno = 0; cfgno < ncfg; cfgno++) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* We grab just the first descriptor so we know how long
		 * the whole configuration is */
		result = usb_get_descriptor(dev, USB_DT_CONFIG, cfgno,
		    desc, USB_DT_CONFIG_SIZE);
		if (result < 0) {
			dev_err(ddev, "unable to read config index %d "
			    "descriptor/%s: %d\n", cfgno, "start", result);
<<<<<<< HEAD
			dev_err(ddev, "chopping to %d config(s)\n", cfgno);
=======
			if (result != -EPIPE)
				goto err;
			dev_notice(ddev, "chopping to %d config(s)\n", cfgno);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			dev->descriptor.bNumConfigurations = cfgno;
			break;
		} else if (result < 4) {
			dev_err(ddev, "config index %d descriptor too short "
			    "(expected %i, got %i)\n", cfgno,
			    USB_DT_CONFIG_SIZE, result);
			result = -EINVAL;
			goto err;
		}
		length = max((int) le16_to_cpu(desc->wTotalLength),
		    USB_DT_CONFIG_SIZE);

		/* Now that we know the length, get the whole thing */
		bigbuffer = kmalloc(length, GFP_KERNEL);
		if (!bigbuffer) {
			result = -ENOMEM;
			goto err;
		}
<<<<<<< HEAD
=======

		if (dev->quirks & USB_QUIRK_DELAY_INIT)
			msleep(200);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		result = usb_get_descriptor(dev, USB_DT_CONFIG, cfgno,
		    bigbuffer, length);
		if (result < 0) {
			dev_err(ddev, "unable to read config index %d "
			    "descriptor/%s\n", cfgno, "all");
			kfree(bigbuffer);
			goto err;
		}
		if (result < length) {
<<<<<<< HEAD
			dev_warn(ddev, "config index %d descriptor too short "
=======
			dev_notice(ddev, "config index %d descriptor too short "
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			    "(expected %i, got %i)\n", cfgno, length, result);
			length = result;
		}

		dev->rawdescriptors[cfgno] = bigbuffer;

		result = usb_parse_configuration(dev, cfgno,
		    &dev->config[cfgno], bigbuffer, length);
		if (result < 0) {
			++cfgno;
			goto err;
		}
	}
<<<<<<< HEAD
	result = 0;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

err:
	kfree(desc);
	dev->descriptor.bNumConfigurations = cfgno;
<<<<<<< HEAD
err2:
	if (result == -ENOMEM)
		dev_err(ddev, "out of memory\n");
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return result;
}

void usb_release_bos_descriptor(struct usb_device *dev)
{
	if (dev->bos) {
		kfree(dev->bos->desc);
		kfree(dev->bos);
		dev->bos = NULL;
	}
}

<<<<<<< HEAD
=======
static const __u8 bos_desc_len[256] = {
	[USB_CAP_TYPE_WIRELESS_USB] = USB_DT_USB_WIRELESS_CAP_SIZE,
	[USB_CAP_TYPE_EXT]          = USB_DT_USB_EXT_CAP_SIZE,
	[USB_SS_CAP_TYPE]           = USB_DT_USB_SS_CAP_SIZE,
	[USB_SSP_CAP_TYPE]          = USB_DT_USB_SSP_CAP_SIZE(1),
	[CONTAINER_ID_TYPE]         = USB_DT_USB_SS_CONTN_ID_SIZE,
	[USB_PTM_CAP_TYPE]          = USB_DT_USB_PTM_ID_SIZE,
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Get BOS descriptor set */
int usb_get_bos_descriptor(struct usb_device *dev)
{
	struct device *ddev = &dev->dev;
	struct usb_bos_descriptor *bos;
	struct usb_dev_cap_header *cap;
<<<<<<< HEAD
	unsigned char *buffer;
	int length, total_len, num, i;
	int ret;

	bos = kzalloc(sizeof(struct usb_bos_descriptor), GFP_KERNEL);
=======
	struct usb_ssp_cap_descriptor *ssp_cap;
	unsigned char *buffer, *buffer0;
	int length, total_len, num, i, ssac;
	__u8 cap_type;
	int ret;

	bos = kzalloc(sizeof(*bos), GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!bos)
		return -ENOMEM;

	/* Get BOS descriptor */
	ret = usb_get_descriptor(dev, USB_DT_BOS, 0, bos, USB_DT_BOS_SIZE);
<<<<<<< HEAD
	if (ret < USB_DT_BOS_SIZE) {
		dev_err(ddev, "unable to get BOS descriptor\n");
=======
	if (ret < USB_DT_BOS_SIZE || bos->bLength < USB_DT_BOS_SIZE) {
		dev_notice(ddev, "unable to get BOS descriptor or descriptor too short\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (ret >= 0)
			ret = -ENOMSG;
		kfree(bos);
		return ret;
	}

	length = bos->bLength;
	total_len = le16_to_cpu(bos->wTotalLength);
	num = bos->bNumDeviceCaps;
	kfree(bos);
	if (total_len < length)
		return -EINVAL;

<<<<<<< HEAD
	dev->bos = kzalloc(sizeof(struct usb_host_bos), GFP_KERNEL);
=======
	dev->bos = kzalloc(sizeof(*dev->bos), GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!dev->bos)
		return -ENOMEM;

	/* Now let's get the whole BOS descriptor set */
	buffer = kzalloc(total_len, GFP_KERNEL);
	if (!buffer) {
		ret = -ENOMEM;
		goto err;
	}
	dev->bos->desc = (struct usb_bos_descriptor *)buffer;

	ret = usb_get_descriptor(dev, USB_DT_BOS, 0, buffer, total_len);
	if (ret < total_len) {
<<<<<<< HEAD
		dev_err(ddev, "unable to get BOS descriptor set\n");
=======
		dev_notice(ddev, "unable to get BOS descriptor set\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (ret >= 0)
			ret = -ENOMSG;
		goto err;
	}
<<<<<<< HEAD
	total_len -= length;

	for (i = 0; i < num; i++) {
		buffer += length;
=======

	buffer0 = buffer;
	total_len -= length;
	buffer += length;

	for (i = 0; i < num; i++) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		cap = (struct usb_dev_cap_header *)buffer;

		if (total_len < sizeof(*cap) || total_len < cap->bLength) {
			dev->bos->desc->bNumDeviceCaps = i;
			break;
		}
<<<<<<< HEAD
		length = cap->bLength;
		total_len -= length;

		if (cap->bDescriptorType != USB_DT_DEVICE_CAPABILITY) {
			dev_warn(ddev, "descriptor type invalid, skip\n");
			continue;
		}

		switch (cap->bDevCapabilityType) {
		case USB_CAP_TYPE_WIRELESS_USB:
			/* Wireless USB cap descriptor is handled by wusb */
			break;
=======
		cap_type = cap->bDevCapabilityType;
		length = cap->bLength;
		if (bos_desc_len[cap_type] && length < bos_desc_len[cap_type]) {
			dev->bos->desc->bNumDeviceCaps = i;
			break;
		}

		if (cap->bDescriptorType != USB_DT_DEVICE_CAPABILITY) {
			dev_notice(ddev, "descriptor type invalid, skip\n");
			goto skip_to_next_descriptor;
		}

		switch (cap_type) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		case USB_CAP_TYPE_EXT:
			dev->bos->ext_cap =
				(struct usb_ext_cap_descriptor *)buffer;
			break;
		case USB_SS_CAP_TYPE:
			dev->bos->ss_cap =
				(struct usb_ss_cap_descriptor *)buffer;
			break;
<<<<<<< HEAD
=======
		case USB_SSP_CAP_TYPE:
			ssp_cap = (struct usb_ssp_cap_descriptor *)buffer;
			ssac = (le32_to_cpu(ssp_cap->bmAttributes) &
				USB_SSP_SUBLINK_SPEED_ATTRIBS);
			if (length >= USB_DT_USB_SSP_CAP_SIZE(ssac))
				dev->bos->ssp_cap = ssp_cap;
			break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		case CONTAINER_ID_TYPE:
			dev->bos->ss_id =
				(struct usb_ss_container_id_descriptor *)buffer;
			break;
<<<<<<< HEAD
		default:
			break;
		}
	}
=======
		case USB_PTM_CAP_TYPE:
			dev->bos->ptm_cap =
				(struct usb_ptm_cap_descriptor *)buffer;
			break;
		default:
			break;
		}

skip_to_next_descriptor:
		total_len -= length;
		buffer += length;
	}
	dev->bos->desc->wTotalLength = cpu_to_le16(buffer - buffer0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;

err:
	usb_release_bos_descriptor(dev);
	return ret;
}
