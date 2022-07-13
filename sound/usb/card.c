<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *   (Tentative) USB Audio Driver for ALSA
 *
 *   Copyright (c) 2002 by Takashi Iwai <tiwai@suse.de>
 *
 *   Many codes borrowed from audio.c by
 *	    Alan Cox (alan@lxorguk.ukuu.org.uk)
 *	    Thomas Sailer (sailer@ife.ee.ethz.ch)
 *
<<<<<<< HEAD
 *
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
 *
 *  NOTES:
 *
 *   - async unlink should be used for avoiding the sleep inside lock.
 *     2.4.22 usb-uhci seems buggy for async unlinking and results in
 *     oops.  in such a cse, pass async_unlink=0 option.
=======
 *   Audio Class 3.0 support by Ruslan Bilovol <ruslan.bilovol@gmail.com>
 *
 *  NOTES:
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *   - the linked URBs would be preferred but not used so far because of
 *     the instability of unlinking.
 *   - type II is not supported properly.  there is no device which supports
 *     this type *correctly*.  SB extigy looks as if it supports, but it's
 *     indeed an AC3 stream packed in SPDIF frames (i.e. no real AC3 stream).
 */


#include <linux/bitops.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/ctype.h>
#include <linux/usb.h>
#include <linux/moduleparam.h>
#include <linux/mutex.h>
#include <linux/usb/audio.h>
#include <linux/usb/audio-v2.h>
<<<<<<< HEAD
=======
#include <linux/usb/audio-v3.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/module.h>

#include <sound/control.h>
#include <sound/core.h>
#include <sound/info.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/initval.h>

#include "usbaudio.h"
#include "card.h"
#include "midi.h"
<<<<<<< HEAD
=======
#include "midi2.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "mixer.h"
#include "proc.h"
#include "quirks.h"
#include "endpoint.h"
#include "helper.h"
<<<<<<< HEAD
#include "debug.h"
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "pcm.h"
#include "format.h"
#include "power.h"
#include "stream.h"
<<<<<<< HEAD
=======
#include "media.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

MODULE_AUTHOR("Takashi Iwai <tiwai@suse.de>");
MODULE_DESCRIPTION("USB Audio");
MODULE_LICENSE("GPL");
<<<<<<< HEAD
MODULE_SUPPORTED_DEVICE("{{Generic,USB Audio}}");

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int index[SNDRV_CARDS] = SNDRV_DEFAULT_IDX;	/* Index 0-MAX */
static char *id[SNDRV_CARDS] = SNDRV_DEFAULT_STR;	/* ID for this card */
static bool enable[SNDRV_CARDS] = SNDRV_DEFAULT_ENABLE_PNP;/* Enable this card */
/* Vendor/product IDs for this card */
static int vid[SNDRV_CARDS] = { [0 ... (SNDRV_CARDS-1)] = -1 };
static int pid[SNDRV_CARDS] = { [0 ... (SNDRV_CARDS-1)] = -1 };
<<<<<<< HEAD
static int nrpacks = 8;		/* max. number of packets per urb */
static bool async_unlink = 1;
static int device_setup[SNDRV_CARDS]; /* device parameter for this card */
static bool ignore_ctl_error;
=======
static int device_setup[SNDRV_CARDS]; /* device parameter for this card */
static bool ignore_ctl_error;
static bool autoclock = true;
static bool lowlatency = true;
static char *quirk_alias[SNDRV_CARDS];
static char *delayed_register[SNDRV_CARDS];
static bool implicit_fb[SNDRV_CARDS];
static unsigned int quirk_flags[SNDRV_CARDS];

bool snd_usb_use_vmalloc = true;
bool snd_usb_skip_validation;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

module_param_array(index, int, NULL, 0444);
MODULE_PARM_DESC(index, "Index value for the USB audio adapter.");
module_param_array(id, charp, NULL, 0444);
MODULE_PARM_DESC(id, "ID string for the USB audio adapter.");
module_param_array(enable, bool, NULL, 0444);
MODULE_PARM_DESC(enable, "Enable USB audio adapter.");
module_param_array(vid, int, NULL, 0444);
MODULE_PARM_DESC(vid, "Vendor ID for the USB audio device.");
module_param_array(pid, int, NULL, 0444);
MODULE_PARM_DESC(pid, "Product ID for the USB audio device.");
<<<<<<< HEAD
module_param(nrpacks, int, 0644);
MODULE_PARM_DESC(nrpacks, "Max. number of packets per URB.");
module_param(async_unlink, bool, 0444);
MODULE_PARM_DESC(async_unlink, "Use async unlink mode.");
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
module_param_array(device_setup, int, NULL, 0444);
MODULE_PARM_DESC(device_setup, "Specific device setup (if needed).");
module_param(ignore_ctl_error, bool, 0444);
MODULE_PARM_DESC(ignore_ctl_error,
		 "Ignore errors from USB controller for mixer interfaces.");
<<<<<<< HEAD
=======
module_param(autoclock, bool, 0444);
MODULE_PARM_DESC(autoclock, "Enable auto-clock selection for UAC2 devices (default: yes).");
module_param(lowlatency, bool, 0444);
MODULE_PARM_DESC(lowlatency, "Enable low latency playback (default: yes).");
module_param_array(quirk_alias, charp, NULL, 0444);
MODULE_PARM_DESC(quirk_alias, "Quirk aliases, e.g. 0123abcd:5678beef.");
module_param_array(delayed_register, charp, NULL, 0444);
MODULE_PARM_DESC(delayed_register, "Quirk for delayed registration, given by id:iface, e.g. 0123abcd:4.");
module_param_array(implicit_fb, bool, NULL, 0444);
MODULE_PARM_DESC(implicit_fb, "Apply generic implicit feedback sync mode.");
module_param_array(quirk_flags, uint, NULL, 0444);
MODULE_PARM_DESC(quirk_flags, "Driver quirk bit flags.");
module_param_named(use_vmalloc, snd_usb_use_vmalloc, bool, 0444);
MODULE_PARM_DESC(use_vmalloc, "Use vmalloc for PCM intermediate buffers (default: yes).");
module_param_named(skip_validation, snd_usb_skip_validation, bool, 0444);
MODULE_PARM_DESC(skip_validation, "Skip unit descriptor validation (default: no).");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * we keep the snd_usb_audio_t instances by ourselves for merging
 * the all interfaces on the same card as one sound device.
 */

static DEFINE_MUTEX(register_mutex);
static struct snd_usb_audio *usb_chip[SNDRV_CARDS];
static struct usb_driver usb_audio_driver;

/*
 * disconnect streams
<<<<<<< HEAD
 * called from snd_usb_audio_disconnect()
 */
static void snd_usb_stream_disconnect(struct list_head *head)
{
	int idx;
	struct snd_usb_stream *as;
	struct snd_usb_substream *subs;

	as = list_entry(head, struct snd_usb_stream, list);
=======
 * called from usb_audio_disconnect()
 */
static void snd_usb_stream_disconnect(struct snd_usb_stream *as)
{
	int idx;
	struct snd_usb_substream *subs;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	for (idx = 0; idx < 2; idx++) {
		subs = &as->substream[idx];
		if (!subs->num_formats)
			continue;
<<<<<<< HEAD
		snd_usb_release_substream_urbs(subs, 1);
		subs->interface = -1;
=======
		subs->data_endpoint = NULL;
		subs->sync_endpoint = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

static int snd_usb_create_stream(struct snd_usb_audio *chip, int ctrlif, int interface)
{
	struct usb_device *dev = chip->dev;
	struct usb_host_interface *alts;
	struct usb_interface_descriptor *altsd;
	struct usb_interface *iface = usb_ifnum_to_if(dev, interface);

	if (!iface) {
<<<<<<< HEAD
		snd_printk(KERN_ERR "%d:%u:%d : does not exist\n",
			   dev->devnum, ctrlif, interface);
=======
		dev_err(&dev->dev, "%u:%d : does not exist\n",
			ctrlif, interface);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;
	}

	alts = &iface->altsetting[0];
	altsd = get_iface_desc(alts);

	/*
	 * Android with both accessory and audio interfaces enabled gets the
	 * interface numbers wrong.
	 */
	if ((chip->usb_id == USB_ID(0x18d1, 0x2d04) ||
	     chip->usb_id == USB_ID(0x18d1, 0x2d05)) &&
	    interface == 0 &&
	    altsd->bInterfaceClass == USB_CLASS_VENDOR_SPEC &&
	    altsd->bInterfaceSubClass == USB_SUBCLASS_VENDOR_SPEC) {
		interface = 2;
		iface = usb_ifnum_to_if(dev, interface);
		if (!iface)
			return -EINVAL;
		alts = &iface->altsetting[0];
		altsd = get_iface_desc(alts);
	}

	if (usb_interface_claimed(iface)) {
<<<<<<< HEAD
		snd_printdd(KERN_INFO "%d:%d:%d: skipping, already claimed\n",
						dev->devnum, ctrlif, interface);
=======
		dev_dbg(&dev->dev, "%d:%d: skipping, already claimed\n",
			ctrlif, interface);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;
	}

	if ((altsd->bInterfaceClass == USB_CLASS_AUDIO ||
	     altsd->bInterfaceClass == USB_CLASS_VENDOR_SPEC) &&
	    altsd->bInterfaceSubClass == USB_SUBCLASS_MIDISTREAMING) {
<<<<<<< HEAD
		int err = snd_usbmidi_create(chip->card, iface,
					     &chip->midi_list, NULL);
		if (err < 0) {
			snd_printk(KERN_ERR "%d:%u:%d: cannot create sequencer device\n",
						dev->devnum, ctrlif, interface);
			return -EINVAL;
		}
		usb_driver_claim_interface(&usb_audio_driver, iface, (void *)-1L);

		return 0;
=======
		int err = snd_usb_midi_v2_create(chip, iface, NULL,
						 chip->usb_id);
		if (err < 0) {
			dev_err(&dev->dev,
				"%u:%d: cannot create sequencer device\n",
				ctrlif, interface);
			return -EINVAL;
		}
		return usb_driver_claim_interface(&usb_audio_driver, iface,
						  USB_AUDIO_IFACE_UNUSED);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if ((altsd->bInterfaceClass != USB_CLASS_AUDIO &&
	     altsd->bInterfaceClass != USB_CLASS_VENDOR_SPEC) ||
	    altsd->bInterfaceSubClass != USB_SUBCLASS_AUDIOSTREAMING) {
<<<<<<< HEAD
		snd_printdd(KERN_ERR "%d:%u:%d: skipping non-supported interface %d\n",
					dev->devnum, ctrlif, interface, altsd->bInterfaceClass);
=======
		dev_dbg(&dev->dev,
			"%u:%d: skipping non-supported interface %d\n",
			ctrlif, interface, altsd->bInterfaceClass);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* skip non-supported classes */
		return -EINVAL;
	}

	if (snd_usb_get_speed(dev) == USB_SPEED_LOW) {
<<<<<<< HEAD
		snd_printk(KERN_ERR "low speed audio streaming not supported\n");
=======
		dev_err(&dev->dev, "low speed audio streaming not supported\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;
	}

	if (! snd_usb_parse_audio_interface(chip, interface)) {
		usb_set_interface(dev, interface, 0); /* reset the current interface */
<<<<<<< HEAD
		usb_driver_claim_interface(&usb_audio_driver, iface, (void *)-1L);
		return -EINVAL;
=======
		return usb_driver_claim_interface(&usb_audio_driver, iface,
						  USB_AUDIO_IFACE_UNUSED);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return 0;
}

/*
 * parse audio control descriptor and create pcm/midi streams
 */
static int snd_usb_create_streams(struct snd_usb_audio *chip, int ctrlif)
{
	struct usb_device *dev = chip->dev;
	struct usb_host_interface *host_iface;
	struct usb_interface_descriptor *altsd;
<<<<<<< HEAD
	struct usb_interface *usb_iface;
	void *control_header;
	int i, protocol;
	int rest_bytes;

	usb_iface = usb_ifnum_to_if(dev, ctrlif);
	if (!usb_iface) {
		snd_printk(KERN_ERR "%d:%u : does not exist\n",
					dev->devnum, ctrlif);
		return -EINVAL;
	}

	/* find audiocontrol interface */
	host_iface = &usb_iface->altsetting[0];
	if (!host_iface) {
		snd_printk(KERN_ERR "Audio Control interface is not available.");
		return -EINVAL;
	}

	control_header = snd_usb_find_csint_desc(host_iface->extra,
						 host_iface->extralen,
						 NULL, UAC_HEADER);
	altsd = get_iface_desc(host_iface);
	protocol = altsd->bInterfaceProtocol;

	if (!control_header) {
		snd_printk(KERN_ERR "cannot find UAC_HEADER\n");
		return -EINVAL;
	}

	rest_bytes = (void *)(host_iface->extra + host_iface->extralen) -
		control_header;

	/* just to be sure -- this shouldn't hit at all */
	if (rest_bytes <= 0) {
		dev_err(&dev->dev, "invalid control header\n");
		return -EINVAL;
	}

	switch (protocol) {
	default:
		snd_printdd(KERN_WARNING "unknown interface protocol %#02x, assuming v1\n",
			    protocol);
		/* fall through */

	case UAC_VERSION_1: {
		struct uac1_ac_header_descriptor *h1 = control_header;
=======
	int i, protocol;

	/* find audiocontrol interface */
	host_iface = &usb_ifnum_to_if(dev, ctrlif)->altsetting[0];
	altsd = get_iface_desc(host_iface);
	protocol = altsd->bInterfaceProtocol;

	switch (protocol) {
	default:
		dev_warn(&dev->dev,
			 "unknown interface protocol %#02x, assuming v1\n",
			 protocol);
		fallthrough;

	case UAC_VERSION_1: {
		struct uac1_ac_header_descriptor *h1;
		int rest_bytes;

		h1 = snd_usb_find_csint_desc(host_iface->extra,
							 host_iface->extralen,
							 NULL, UAC_HEADER);
		if (!h1 || h1->bLength < sizeof(*h1)) {
			dev_err(&dev->dev, "cannot find UAC_HEADER\n");
			return -EINVAL;
		}

		rest_bytes = (void *)(host_iface->extra +
				host_iface->extralen) - (void *)h1;

		/* just to be sure -- this shouldn't hit at all */
		if (rest_bytes <= 0) {
			dev_err(&dev->dev, "invalid control header\n");
			return -EINVAL;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (rest_bytes < sizeof(*h1)) {
			dev_err(&dev->dev, "too short v1 buffer descriptor\n");
			return -EINVAL;
		}

		if (!h1->bInCollection) {
<<<<<<< HEAD
			snd_printk(KERN_INFO "skipping empty audio interface (v1)\n");
=======
			dev_info(&dev->dev, "skipping empty audio interface (v1)\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -EINVAL;
		}

		if (rest_bytes < h1->bLength) {
			dev_err(&dev->dev, "invalid buffer length (v1)\n");
			return -EINVAL;
		}

		if (h1->bLength < sizeof(*h1) + h1->bInCollection) {
<<<<<<< HEAD
			snd_printk(KERN_ERR "invalid UAC_HEADER (v1)\n");
=======
			dev_err(&dev->dev, "invalid UAC_HEADER (v1)\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -EINVAL;
		}

		for (i = 0; i < h1->bInCollection; i++)
			snd_usb_create_stream(chip, ctrlif, h1->baInterfaceNr[i]);

		break;
	}

<<<<<<< HEAD
	case UAC_VERSION_2: {
		struct usb_interface_assoc_descriptor *assoc =
						usb_iface->intf_assoc;
		if (!assoc) {
			/*
			* Firmware writers cannot count to three. So to find
			* the IAD on the NuForce UDH-100, also check the next
			* interface.
			*/
			struct usb_interface *iface =
					usb_ifnum_to_if(dev, ctrlif + 1);
			if (iface &&
				iface->intf_assoc &&
				iface->intf_assoc->bFunctionClass == USB_CLASS_AUDIO &&
				iface->intf_assoc->bFunctionProtocol == UAC_VERSION_2)
=======
	case UAC_VERSION_2:
	case UAC_VERSION_3: {
		struct usb_interface_assoc_descriptor *assoc =
			usb_ifnum_to_if(dev, ctrlif)->intf_assoc;

		if (!assoc) {
			/*
			 * Firmware writers cannot count to three.  So to find
			 * the IAD on the NuForce UDH-100, also check the next
			 * interface.
			 */
			struct usb_interface *iface =
				usb_ifnum_to_if(dev, ctrlif + 1);
			if (iface &&
			    iface->intf_assoc &&
			    iface->intf_assoc->bFunctionClass == USB_CLASS_AUDIO &&
			    iface->intf_assoc->bFunctionProtocol == UAC_VERSION_2)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				assoc = iface->intf_assoc;
		}

		if (!assoc) {
<<<<<<< HEAD
			snd_printk(KERN_ERR "Audio class v2 interfaces need an interface association\n");
			return -EINVAL;
		}

=======
			dev_err(&dev->dev, "Audio class v2/v3 interfaces need an interface association\n");
			return -EINVAL;
		}

		if (protocol == UAC_VERSION_3) {
			int badd = assoc->bFunctionSubClass;

			if (badd != UAC3_FUNCTION_SUBCLASS_FULL_ADC_3_0 &&
			    (badd < UAC3_FUNCTION_SUBCLASS_GENERIC_IO ||
			     badd > UAC3_FUNCTION_SUBCLASS_SPEAKERPHONE)) {
				dev_err(&dev->dev,
					"Unsupported UAC3 BADD profile\n");
				return -EINVAL;
			}

			chip->badd_profile = badd;
		}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		for (i = 0; i < assoc->bInterfaceCount; i++) {
			int intf = assoc->bFirstInterface + i;

			if (intf != ctrlif)
				snd_usb_create_stream(chip, ctrlif, intf);
		}

		break;
	}
	}
<<<<<<< HEAD
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

/*
<<<<<<< HEAD
=======
 * Profile name preset table
 */
struct usb_audio_device_name {
	u32 id;
	const char *vendor_name;
	const char *product_name;
	const char *profile_name;	/* override card->longname */
};

#define PROFILE_NAME(vid, pid, vendor, product, profile)	 \
	{ .id = USB_ID(vid, pid), .vendor_name = (vendor),	 \
	  .product_name = (product), .profile_name = (profile) }
#define DEVICE_NAME(vid, pid, vendor, product) \
	PROFILE_NAME(vid, pid, vendor, product, NULL)

/* vendor/product and profile name presets, sorted in device id order */
static const struct usb_audio_device_name usb_audio_names[] = {
	/* HP Thunderbolt Dock Audio Headset */
	PROFILE_NAME(0x03f0, 0x0269, "HP", "Thunderbolt Dock Audio Headset",
		     "HP-Thunderbolt-Dock-Audio-Headset"),
	/* HP Thunderbolt Dock Audio Module */
	PROFILE_NAME(0x03f0, 0x0567, "HP", "Thunderbolt Dock Audio Module",
		     "HP-Thunderbolt-Dock-Audio-Module"),

	/* Two entries for Gigabyte TRX40 Aorus Master:
	 * TRX40 Aorus Master has two USB-audio devices, one for the front
	 * headphone with ESS SABRE9218 DAC chip, while another for the rest
	 * I/O (the rear panel and the front mic) with Realtek ALC1220-VB.
	 * Here we provide two distinct names for making UCM profiles easier.
	 */
	PROFILE_NAME(0x0414, 0xa000, "Gigabyte", "Aorus Master Front Headphone",
		     "Gigabyte-Aorus-Master-Front-Headphone"),
	PROFILE_NAME(0x0414, 0xa001, "Gigabyte", "Aorus Master Main Audio",
		     "Gigabyte-Aorus-Master-Main-Audio"),

	/* Gigabyte TRX40 Aorus Pro WiFi */
	PROFILE_NAME(0x0414, 0xa002,
		     "Realtek", "ALC1220-VB-DT", "Realtek-ALC1220-VB-Desktop"),

	/* Creative/E-Mu devices */
	DEVICE_NAME(0x041e, 0x3010, "Creative Labs", "Sound Blaster MP3+"),
	/* Creative/Toshiba Multimedia Center SB-0500 */
	DEVICE_NAME(0x041e, 0x3048, "Toshiba", "SB-0500"),

	DEVICE_NAME(0x046d, 0x0990, "Logitech, Inc.", "QuickCam Pro 9000"),

	DEVICE_NAME(0x05e1, 0x0408, "Syntek", "STK1160"),
	DEVICE_NAME(0x05e1, 0x0480, "Hauppauge", "Woodbury"),

	/* ASUS ROG Zenith II: this machine has also two devices, one for
	 * the front headphone and another for the rest
	 */
	PROFILE_NAME(0x0b05, 0x1915, "ASUS", "Zenith II Front Headphone",
		     "Zenith-II-Front-Headphone"),
	PROFILE_NAME(0x0b05, 0x1916, "ASUS", "Zenith II Main Audio",
		     "Zenith-II-Main-Audio"),

	/* ASUS ROG Strix */
	PROFILE_NAME(0x0b05, 0x1917,
		     "Realtek", "ALC1220-VB-DT", "Realtek-ALC1220-VB-Desktop"),
	/* ASUS PRIME TRX40 PRO-S */
	PROFILE_NAME(0x0b05, 0x1918,
		     "Realtek", "ALC1220-VB-DT", "Realtek-ALC1220-VB-Desktop"),

	/* Dell WD15 Dock */
	PROFILE_NAME(0x0bda, 0x4014, "Dell", "WD15 Dock", "Dell-WD15-Dock"),
	/* Dell WD19 Dock */
	PROFILE_NAME(0x0bda, 0x402e, "Dell", "WD19 Dock", "Dell-WD15-Dock"),

	DEVICE_NAME(0x0ccd, 0x0028, "TerraTec", "Aureon5.1MkII"),

	/*
	 * The original product_name is "USB Sound Device", however this name
	 * is also used by the CM106 based cards, so make it unique.
	 */
	DEVICE_NAME(0x0d8c, 0x0102, NULL, "ICUSBAUDIO7D"),
	DEVICE_NAME(0x0d8c, 0x0103, NULL, "Audio Advantage MicroII"),

	/* MSI TRX40 Creator */
	PROFILE_NAME(0x0db0, 0x0d64,
		     "Realtek", "ALC1220-VB-DT", "Realtek-ALC1220-VB-Desktop"),
	/* MSI TRX40 */
	PROFILE_NAME(0x0db0, 0x543d,
		     "Realtek", "ALC1220-VB-DT", "Realtek-ALC1220-VB-Desktop"),

	DEVICE_NAME(0x0fd9, 0x0008, "Hauppauge", "HVR-950Q"),

	/* Stanton/N2IT Final Scratch v1 device ('Scratchamp') */
	DEVICE_NAME(0x103d, 0x0100, "Stanton", "ScratchAmp"),
	DEVICE_NAME(0x103d, 0x0101, "Stanton", "ScratchAmp"),

	/* aka. Serato Scratch Live DJ Box */
	DEVICE_NAME(0x13e5, 0x0001, "Rane", "SL-1"),

	/* Lenovo ThinkStation P620 Rear Line-in, Line-out and Microphone */
	PROFILE_NAME(0x17aa, 0x1046, "Lenovo", "ThinkStation P620 Rear",
		     "Lenovo-ThinkStation-P620-Rear"),
	/* Lenovo ThinkStation P620 Internal Speaker + Front Headset */
	PROFILE_NAME(0x17aa, 0x104d, "Lenovo", "ThinkStation P620 Main",
		     "Lenovo-ThinkStation-P620-Main"),

	/* Asrock TRX40 Creator */
	PROFILE_NAME(0x26ce, 0x0a01,
		     "Realtek", "ALC1220-VB-DT", "Realtek-ALC1220-VB-Desktop"),

	DEVICE_NAME(0x2040, 0x7200, "Hauppauge", "HVR-950Q"),
	DEVICE_NAME(0x2040, 0x7201, "Hauppauge", "HVR-950Q-MXL"),
	DEVICE_NAME(0x2040, 0x7210, "Hauppauge", "HVR-950Q"),
	DEVICE_NAME(0x2040, 0x7211, "Hauppauge", "HVR-950Q-MXL"),
	DEVICE_NAME(0x2040, 0x7213, "Hauppauge", "HVR-950Q"),
	DEVICE_NAME(0x2040, 0x7217, "Hauppauge", "HVR-950Q"),
	DEVICE_NAME(0x2040, 0x721b, "Hauppauge", "HVR-950Q"),
	DEVICE_NAME(0x2040, 0x721e, "Hauppauge", "HVR-950Q"),
	DEVICE_NAME(0x2040, 0x721f, "Hauppauge", "HVR-950Q"),
	DEVICE_NAME(0x2040, 0x7240, "Hauppauge", "HVR-850"),
	DEVICE_NAME(0x2040, 0x7260, "Hauppauge", "HVR-950Q"),
	DEVICE_NAME(0x2040, 0x7270, "Hauppauge", "HVR-950Q"),
	DEVICE_NAME(0x2040, 0x7280, "Hauppauge", "HVR-950Q"),
	DEVICE_NAME(0x2040, 0x7281, "Hauppauge", "HVR-950Q-MXL"),
	DEVICE_NAME(0x2040, 0x8200, "Hauppauge", "Woodbury"),

	{ } /* terminator */
};

static const struct usb_audio_device_name *
lookup_device_name(u32 id)
{
	static const struct usb_audio_device_name *p;

	for (p = usb_audio_names; p->id; p++)
		if (p->id == id)
			return p;
	return NULL;
}

/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * free the chip instance
 *
 * here we have to do not much, since pcm and controls are already freed
 *
 */

<<<<<<< HEAD
static int snd_usb_audio_free(struct snd_usb_audio *chip)
{
	kfree(chip);
	return 0;
}

static int snd_usb_audio_dev_free(struct snd_device *device)
{
	struct snd_usb_audio *chip = device->device_data;
	return snd_usb_audio_free(chip);
}

static void remove_trailing_spaces(char *str)
{
	char *p;

	if (!*str)
		return;
	for (p = str + strlen(str) - 1; p >= str && isspace(*p); p--)
		*p = 0;
}

/*
 * create a chip instance and set its names.
 */
static int snd_usb_audio_create(struct usb_device *dev, int idx,
				const struct snd_usb_audio_quirk *quirk,
				struct snd_usb_audio **rchip)
{
	struct snd_card *card;
	struct snd_usb_audio *chip;
	int err, len;
	char component[14];
	static struct snd_device_ops ops = {
		.dev_free =	snd_usb_audio_dev_free,
	};

	*rchip = NULL;

	switch (snd_usb_get_speed(dev)) {
	case USB_SPEED_LOW:
	case USB_SPEED_FULL:
	case USB_SPEED_HIGH:
	case USB_SPEED_SUPER:
		break;
	default:
		snd_printk(KERN_ERR "unknown device speed %d\n", snd_usb_get_speed(dev));
		return -ENXIO;
	}

	err = snd_card_create(index[idx], id[idx], THIS_MODULE, 0, &card);
	if (err < 0) {
		snd_printk(KERN_ERR "cannot create card instance %d\n", idx);
		return err;
	}

	chip = kzalloc(sizeof(*chip), GFP_KERNEL);
	if (! chip) {
		snd_card_free(card);
		return -ENOMEM;
	}

	init_rwsem(&chip->shutdown_rwsem);
	chip->index = idx;
	chip->dev = dev;
	chip->card = card;
	chip->setup = device_setup[idx];
	chip->nrpacks = nrpacks;
	chip->async_unlink = async_unlink;
	chip->probing = 1;

	chip->usb_id = USB_ID(le16_to_cpu(dev->descriptor.idVendor),
			      le16_to_cpu(dev->descriptor.idProduct));
	INIT_LIST_HEAD(&chip->pcm_list);
	INIT_LIST_HEAD(&chip->midi_list);
	INIT_LIST_HEAD(&chip->mixer_list);

	if ((err = snd_device_new(card, SNDRV_DEV_LOWLEVEL, chip, &ops)) < 0) {
		snd_usb_audio_free(chip);
		snd_card_free(card);
		return err;
	}

	strcpy(card->driver, "USB-Audio");
	sprintf(component, "USB%04x:%04x",
		USB_ID_VENDOR(chip->usb_id), USB_ID_PRODUCT(chip->usb_id));
	snd_component_add(card, component);

	/* retrieve the device string as shortname */
	if (quirk && quirk->product_name && *quirk->product_name) {
		strlcpy(card->shortname, quirk->product_name, sizeof(card->shortname));
	} else {
		if (!dev->descriptor.iProduct ||
		    usb_string(dev, dev->descriptor.iProduct,
		    card->shortname, sizeof(card->shortname)) <= 0) {
			/* no name available from anywhere, so use ID */
			sprintf(card->shortname, "USB Device %#04x:%#04x",
				USB_ID_VENDOR(chip->usb_id),
				USB_ID_PRODUCT(chip->usb_id));
		}
	}
	remove_trailing_spaces(card->shortname);

	/* retrieve the vendor and device strings as longname */
	if (quirk && quirk->vendor_name && *quirk->vendor_name) {
		len = strlcpy(card->longname, quirk->vendor_name, sizeof(card->longname));
	} else {
		if (dev->descriptor.iManufacturer)
			len = usb_string(dev, dev->descriptor.iManufacturer,
					 card->longname, sizeof(card->longname));
		else
			len = 0;
		/* we don't really care if there isn't any vendor string */
	}
	if (len > 0) {
		remove_trailing_spaces(card->longname);
=======
static void snd_usb_audio_free(struct snd_card *card)
{
	struct snd_usb_audio *chip = card->private_data;

	snd_usb_endpoint_free_all(chip);
	snd_usb_midi_v2_free_all(chip);

	mutex_destroy(&chip->mutex);
	if (!atomic_read(&chip->shutdown))
		dev_set_drvdata(&chip->dev->dev, NULL);
}

static void usb_audio_make_shortname(struct usb_device *dev,
				     struct snd_usb_audio *chip,
				     const struct snd_usb_audio_quirk *quirk)
{
	struct snd_card *card = chip->card;
	const struct usb_audio_device_name *preset;
	const char *s = NULL;

	preset = lookup_device_name(chip->usb_id);
	if (preset && preset->product_name)
		s = preset->product_name;
	else if (quirk && quirk->product_name)
		s = quirk->product_name;
	if (s && *s) {
		strscpy(card->shortname, s, sizeof(card->shortname));
		return;
	}

	/* retrieve the device string as shortname */
	if (!dev->descriptor.iProduct ||
	    usb_string(dev, dev->descriptor.iProduct,
		       card->shortname, sizeof(card->shortname)) <= 0) {
		/* no name available from anywhere, so use ID */
		sprintf(card->shortname, "USB Device %#04x:%#04x",
			USB_ID_VENDOR(chip->usb_id),
			USB_ID_PRODUCT(chip->usb_id));
	}

	strim(card->shortname);
}

static void usb_audio_make_longname(struct usb_device *dev,
				    struct snd_usb_audio *chip,
				    const struct snd_usb_audio_quirk *quirk)
{
	struct snd_card *card = chip->card;
	const struct usb_audio_device_name *preset;
	const char *s = NULL;
	int len;

	preset = lookup_device_name(chip->usb_id);

	/* shortcut - if any pre-defined string is given, use it */
	if (preset && preset->profile_name)
		s = preset->profile_name;
	if (s && *s) {
		strscpy(card->longname, s, sizeof(card->longname));
		return;
	}

	if (preset && preset->vendor_name)
		s = preset->vendor_name;
	else if (quirk && quirk->vendor_name)
		s = quirk->vendor_name;
	*card->longname = 0;
	if (s && *s) {
		strscpy(card->longname, s, sizeof(card->longname));
	} else {
		/* retrieve the vendor and device strings as longname */
		if (dev->descriptor.iManufacturer)
			usb_string(dev, dev->descriptor.iManufacturer,
				   card->longname, sizeof(card->longname));
		/* we don't really care if there isn't any vendor string */
	}
	if (*card->longname) {
		strim(card->longname);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (*card->longname)
			strlcat(card->longname, " ", sizeof(card->longname));
	}

	strlcat(card->longname, card->shortname, sizeof(card->longname));

	len = strlcat(card->longname, " at ", sizeof(card->longname));

	if (len < sizeof(card->longname))
		usb_make_path(dev, card->longname + len, sizeof(card->longname) - len);

	switch (snd_usb_get_speed(dev)) {
	case USB_SPEED_LOW:
		strlcat(card->longname, ", low speed", sizeof(card->longname));
		break;
	case USB_SPEED_FULL:
		strlcat(card->longname, ", full speed", sizeof(card->longname));
		break;
	case USB_SPEED_HIGH:
		strlcat(card->longname, ", high speed", sizeof(card->longname));
		break;
	case USB_SPEED_SUPER:
		strlcat(card->longname, ", super speed", sizeof(card->longname));
		break;
<<<<<<< HEAD
	default:
		break;
	}
=======
	case USB_SPEED_SUPER_PLUS:
		strlcat(card->longname, ", super speed plus", sizeof(card->longname));
		break;
	default:
		break;
	}
}

/*
 * create a chip instance and set its names.
 */
static int snd_usb_audio_create(struct usb_interface *intf,
				struct usb_device *dev, int idx,
				const struct snd_usb_audio_quirk *quirk,
				unsigned int usb_id,
				struct snd_usb_audio **rchip)
{
	struct snd_card *card;
	struct snd_usb_audio *chip;
	int err;
	char component[14];

	*rchip = NULL;

	switch (snd_usb_get_speed(dev)) {
	case USB_SPEED_LOW:
	case USB_SPEED_FULL:
	case USB_SPEED_HIGH:
	case USB_SPEED_SUPER:
	case USB_SPEED_SUPER_PLUS:
		break;
	default:
		dev_err(&dev->dev, "unknown device speed %d\n", snd_usb_get_speed(dev));
		return -ENXIO;
	}

	err = snd_card_new(&intf->dev, index[idx], id[idx], THIS_MODULE,
			   sizeof(*chip), &card);
	if (err < 0) {
		dev_err(&dev->dev, "cannot create card instance %d\n", idx);
		return err;
	}

	chip = card->private_data;
	mutex_init(&chip->mutex);
	init_waitqueue_head(&chip->shutdown_wait);
	chip->index = idx;
	chip->dev = dev;
	chip->card = card;
	chip->setup = device_setup[idx];
	chip->generic_implicit_fb = implicit_fb[idx];
	chip->autoclock = autoclock;
	chip->lowlatency = lowlatency;
	atomic_set(&chip->active, 1); /* avoid autopm during probing */
	atomic_set(&chip->usage_count, 0);
	atomic_set(&chip->shutdown, 0);

	chip->usb_id = usb_id;
	INIT_LIST_HEAD(&chip->pcm_list);
	INIT_LIST_HEAD(&chip->ep_list);
	INIT_LIST_HEAD(&chip->iface_ref_list);
	INIT_LIST_HEAD(&chip->clock_ref_list);
	INIT_LIST_HEAD(&chip->midi_list);
	INIT_LIST_HEAD(&chip->midi_v2_list);
	INIT_LIST_HEAD(&chip->mixer_list);

	if (quirk_flags[idx])
		chip->quirk_flags = quirk_flags[idx];
	else
		snd_usb_init_quirk_flags(chip);

	card->private_free = snd_usb_audio_free;

	strcpy(card->driver, "USB-Audio");
	sprintf(component, "USB%04x:%04x",
		USB_ID_VENDOR(chip->usb_id), USB_ID_PRODUCT(chip->usb_id));
	snd_component_add(card, component);

	usb_audio_make_shortname(dev, chip, quirk);
	usb_audio_make_longname(dev, chip, quirk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	snd_usb_audio_create_proc(chip);

	*rchip = chip;
	return 0;
}

<<<<<<< HEAD
=======
/* look for a matching quirk alias id */
static bool get_alias_id(struct usb_device *dev, unsigned int *id)
{
	int i;
	unsigned int src, dst;

	for (i = 0; i < ARRAY_SIZE(quirk_alias); i++) {
		if (!quirk_alias[i] ||
		    sscanf(quirk_alias[i], "%x:%x", &src, &dst) != 2 ||
		    src != *id)
			continue;
		dev_info(&dev->dev,
			 "device (%04x:%04x): applying quirk alias %04x:%04x\n",
			 USB_ID_VENDOR(*id), USB_ID_PRODUCT(*id),
			 USB_ID_VENDOR(dst), USB_ID_PRODUCT(dst));
		*id = dst;
		return true;
	}

	return false;
}

static int check_delayed_register_option(struct snd_usb_audio *chip)
{
	int i;
	unsigned int id, inum;

	for (i = 0; i < ARRAY_SIZE(delayed_register); i++) {
		if (delayed_register[i] &&
		    sscanf(delayed_register[i], "%x:%x", &id, &inum) == 2 &&
		    id == chip->usb_id)
			return inum;
	}

	return -1;
}

static const struct usb_device_id usb_audio_ids[]; /* defined below */

/* look for the last interface that matches with our ids and remember it */
static void find_last_interface(struct snd_usb_audio *chip)
{
	struct usb_host_config *config = chip->dev->actconfig;
	struct usb_interface *intf;
	int i;

	if (!config)
		return;
	for (i = 0; i < config->desc.bNumInterfaces; i++) {
		intf = config->interface[i];
		if (usb_match_id(intf, usb_audio_ids))
			chip->last_iface = intf->altsetting[0].desc.bInterfaceNumber;
	}
	usb_audio_dbg(chip, "Found last interface = %d\n", chip->last_iface);
}

/* look for the corresponding quirk */
static const struct snd_usb_audio_quirk *
get_alias_quirk(struct usb_device *dev, unsigned int id)
{
	const struct usb_device_id *p;

	for (p = usb_audio_ids; p->match_flags; p++) {
		/* FIXME: this checks only vendor:product pair in the list */
		if ((p->match_flags & USB_DEVICE_ID_MATCH_DEVICE) ==
		    USB_DEVICE_ID_MATCH_DEVICE &&
		    p->idVendor == USB_ID_VENDOR(id) &&
		    p->idProduct == USB_ID_PRODUCT(id))
			return (const struct snd_usb_audio_quirk *)p->driver_info;
	}

	return NULL;
}

/* register card if we reach to the last interface or to the specified
 * one given via option
 */
static int try_to_register_card(struct snd_usb_audio *chip, int ifnum)
{
	if (check_delayed_register_option(chip) == ifnum ||
	    chip->last_iface == ifnum ||
	    usb_interface_claimed(usb_ifnum_to_if(chip->dev, chip->last_iface)))
		return snd_card_register(chip->card);
	return 0;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * probe the active usb device
 *
 * note that this can be called multiple times per a device, when it
 * includes multiple audio control interfaces.
 *
 * thus we check the usb device pointer and creates the card instance
 * only at the first time.  the successive calls of this function will
 * append the pcm interface to the corresponding card.
 */
<<<<<<< HEAD
static struct snd_usb_audio *
snd_usb_audio_probe(struct usb_device *dev,
		    struct usb_interface *intf,
		    const struct usb_device_id *usb_id)
{
	const struct snd_usb_audio_quirk *quirk = (const struct snd_usb_audio_quirk *)usb_id->driver_info;
	int i, err;
	struct snd_usb_audio *chip;
=======
static int usb_audio_probe(struct usb_interface *intf,
			   const struct usb_device_id *usb_id)
{
	struct usb_device *dev = interface_to_usbdev(intf);
	const struct snd_usb_audio_quirk *quirk =
		(const struct snd_usb_audio_quirk *)usb_id->driver_info;
	struct snd_usb_audio *chip;
	int i, err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct usb_host_interface *alts;
	int ifnum;
	u32 id;

	alts = &intf->altsetting[0];
	ifnum = get_iface_desc(alts)->bInterfaceNumber;
	id = USB_ID(le16_to_cpu(dev->descriptor.idVendor),
		    le16_to_cpu(dev->descriptor.idProduct));
<<<<<<< HEAD
	if (quirk && quirk->ifnum >= 0 && ifnum != quirk->ifnum)
		goto __err_val;

	if (snd_usb_apply_boot_quirk(dev, intf, quirk) < 0)
		goto __err_val;
=======
	if (get_alias_id(dev, &id))
		quirk = get_alias_quirk(dev, id);
	if (quirk && quirk->ifnum >= 0 && ifnum != quirk->ifnum)
		return -ENXIO;
	if (quirk && quirk->ifnum == QUIRK_NODEV_INTERFACE)
		return -ENODEV;

	err = snd_usb_apply_boot_quirk(dev, intf, quirk, id);
	if (err < 0)
		return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * found a config.  now register to ALSA
	 */

	/* check whether it's already registered */
	chip = NULL;
	mutex_lock(&register_mutex);
	for (i = 0; i < SNDRV_CARDS; i++) {
		if (usb_chip[i] && usb_chip[i]->dev == dev) {
<<<<<<< HEAD
			if (usb_chip[i]->shutdown) {
				snd_printk(KERN_ERR "USB device is in the shutdown state, cannot create a card instance\n");
				goto __error;
			}
			chip = usb_chip[i];
			chip->probing = 1;
=======
			if (atomic_read(&usb_chip[i]->shutdown)) {
				dev_err(&dev->dev, "USB device is in the shutdown state, cannot create a card instance\n");
				err = -EIO;
				goto __error;
			}
			chip = usb_chip[i];
			atomic_inc(&chip->active); /* avoid autopm */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		}
	}
	if (! chip) {
<<<<<<< HEAD
=======
		err = snd_usb_apply_boot_quirk_once(dev, intf, quirk, id);
		if (err < 0)
			goto __error;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* it's a fresh one.
		 * now look for an empty slot and create a new card instance
		 */
		for (i = 0; i < SNDRV_CARDS; i++)
<<<<<<< HEAD
			if (enable[i] && ! usb_chip[i] &&
			    (vid[i] == -1 || vid[i] == USB_ID_VENDOR(id)) &&
			    (pid[i] == -1 || pid[i] == USB_ID_PRODUCT(id))) {
				if (snd_usb_audio_create(dev, i, quirk, &chip) < 0) {
					goto __error;
				}
				snd_card_set_dev(chip->card, &intf->dev);
				chip->pm_intf = intf;
				break;
			}
		if (!chip) {
			printk(KERN_ERR "no available usb audio device\n");
			goto __error;
		}
	}

=======
			if (!usb_chip[i] &&
			    (vid[i] == -1 || vid[i] == USB_ID_VENDOR(id)) &&
			    (pid[i] == -1 || pid[i] == USB_ID_PRODUCT(id))) {
				if (enable[i]) {
					err = snd_usb_audio_create(intf, dev, i, quirk,
								   id, &chip);
					if (err < 0)
						goto __error;
					break;
				} else if (vid[i] != -1 || pid[i] != -1) {
					dev_info(&dev->dev,
						 "device (%04x:%04x) is disabled\n",
						 USB_ID_VENDOR(id),
						 USB_ID_PRODUCT(id));
					err = -ENOENT;
					goto __error;
				}
			}
		if (!chip) {
			dev_err(&dev->dev, "no available usb audio device\n");
			err = -ENODEV;
			goto __error;
		}
		find_last_interface(chip);
	}

	if (chip->num_interfaces >= MAX_CARD_INTERFACES) {
		dev_info(&dev->dev, "Too many interfaces assigned to the single USB-audio card\n");
		err = -EINVAL;
		goto __error;
	}

	dev_set_drvdata(&dev->dev, chip);

	if (ignore_ctl_error)
		chip->quirk_flags |= QUIRK_FLAG_IGNORE_CTL_ERROR;

	if (chip->quirk_flags & QUIRK_FLAG_DISABLE_AUTOSUSPEND)
		usb_disable_autosuspend(interface_to_usbdev(intf));

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * For devices with more than one control interface, we assume the
	 * first contains the audio controls. We might need a more specific
	 * check here in the future.
	 */
	if (!chip->ctrl_intf)
		chip->ctrl_intf = alts;

<<<<<<< HEAD
	chip->txfr_quirk = 0;
	err = 1; /* continue */
	if (quirk && quirk->ifnum != QUIRK_NO_INTERFACE) {
		/* need some special handlings */
		if ((err = snd_usb_create_quirk(chip, intf, &usb_audio_driver, quirk)) < 0)
=======
	err = 1; /* continue */
	if (quirk && quirk->ifnum != QUIRK_NO_INTERFACE) {
		/* need some special handlings */
		err = snd_usb_create_quirk(chip, intf, &usb_audio_driver, quirk);
		if (err < 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto __error;
	}

	if (err > 0) {
		/* create normal USB audio interfaces */
<<<<<<< HEAD
		if (snd_usb_create_streams(chip, ifnum) < 0 ||
		    snd_usb_create_mixer(chip, ifnum, ignore_ctl_error) < 0) {
			goto __error;
		}
	}

	/* we are allowed to call snd_card_register() many times */
	if (snd_card_register(chip->card) < 0) {
		goto __error;
	}

	usb_chip[chip->index] = chip;
	chip->num_interfaces++;
	chip->probing = 0;
	mutex_unlock(&register_mutex);
	return chip;

 __error:
	if (chip) {
		if (!chip->num_interfaces)
			snd_card_free(chip->card);
		chip->probing = 0;
	}
	mutex_unlock(&register_mutex);
 __err_val:
	return NULL;
=======
		err = snd_usb_create_streams(chip, ifnum);
		if (err < 0)
			goto __error;
		err = snd_usb_create_mixer(chip, ifnum);
		if (err < 0)
			goto __error;
	}

	if (chip->need_delayed_register) {
		dev_info(&dev->dev,
			 "Found post-registration device assignment: %08x:%02x\n",
			 chip->usb_id, ifnum);
		chip->need_delayed_register = false; /* clear again */
	}

	err = try_to_register_card(chip, ifnum);
	if (err < 0)
		goto __error_no_register;

	if (chip->quirk_flags & QUIRK_FLAG_SHARE_MEDIA_DEVICE) {
		/* don't want to fail when snd_media_device_create() fails */
		snd_media_device_create(chip, intf);
	}

	if (quirk)
		chip->quirk_type = quirk->type;

	usb_chip[chip->index] = chip;
	chip->intf[chip->num_interfaces] = intf;
	chip->num_interfaces++;
	usb_set_intfdata(intf, chip);
	atomic_dec(&chip->active);
	mutex_unlock(&register_mutex);
	return 0;

 __error:
	/* in the case of error in secondary interface, still try to register */
	if (chip)
		try_to_register_card(chip, ifnum);

 __error_no_register:
	if (chip) {
		/* chip->active is inside the chip->card object,
		 * decrement before memory is possibly returned.
		 */
		atomic_dec(&chip->active);
		if (!chip->num_interfaces)
			snd_card_free(chip->card);
	}
	mutex_unlock(&register_mutex);
	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * we need to take care of counter, since disconnection can be called also
 * many times as well as usb_audio_probe().
 */
<<<<<<< HEAD
static void snd_usb_audio_disconnect(struct usb_device *dev,
				     struct snd_usb_audio *chip)
{
	struct snd_card *card;
	struct list_head *p;
	bool was_shutdown;

	if (chip == (void *)-1L)
		return;

	card = chip->card;
	down_write(&chip->shutdown_rwsem);
	was_shutdown = chip->shutdown;
	chip->shutdown = 1;
	up_write(&chip->shutdown_rwsem);

	mutex_lock(&register_mutex);
	if (!was_shutdown) {
		snd_card_disconnect(card);
		/* release the pcm resources */
		list_for_each(p, &chip->pcm_list) {
			snd_usb_stream_disconnect(p);
=======
static void usb_audio_disconnect(struct usb_interface *intf)
{
	struct snd_usb_audio *chip = usb_get_intfdata(intf);
	struct snd_card *card;
	struct list_head *p;

	if (chip == USB_AUDIO_IFACE_UNUSED)
		return;

	card = chip->card;

	mutex_lock(&register_mutex);
	if (atomic_inc_return(&chip->shutdown) == 1) {
		struct snd_usb_stream *as;
		struct snd_usb_endpoint *ep;
		struct usb_mixer_interface *mixer;

		/* wait until all pending tasks done;
		 * they are protected by snd_usb_lock_shutdown()
		 */
		wait_event(chip->shutdown_wait,
			   !atomic_read(&chip->usage_count));
		snd_card_disconnect(card);
		/* release the pcm resources */
		list_for_each_entry(as, &chip->pcm_list, list) {
			snd_usb_stream_disconnect(as);
		}
		/* release the endpoint resources */
		list_for_each_entry(ep, &chip->ep_list, list) {
			snd_usb_endpoint_release(ep);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		/* release the midi resources */
		list_for_each(p, &chip->midi_list) {
			snd_usbmidi_disconnect(p);
		}
<<<<<<< HEAD
		/* release mixer resources */
		list_for_each(p, &chip->mixer_list) {
			snd_usb_mixer_disconnect(p);
		}
	}

=======
		snd_usb_midi_v2_disconnect_all(chip);
		/*
		 * Nice to check quirk && quirk->shares_media_device and
		 * then call the snd_media_device_delete(). Don't have
		 * access to the quirk here. snd_media_device_delete()
		 * accesses mixer_list
		 */
		snd_media_device_delete(chip);

		/* release mixer resources */
		list_for_each_entry(mixer, &chip->mixer_list, list) {
			snd_usb_mixer_disconnect(mixer);
		}
	}

	if (chip->quirk_flags & QUIRK_FLAG_DISABLE_AUTOSUSPEND)
		usb_enable_autosuspend(interface_to_usbdev(intf));

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	chip->num_interfaces--;
	if (chip->num_interfaces <= 0) {
		usb_chip[chip->index] = NULL;
		mutex_unlock(&register_mutex);
		snd_card_free_when_closed(card);
	} else {
		mutex_unlock(&register_mutex);
	}
}

<<<<<<< HEAD
/*
 * new 2.5 USB kernel API
 */
static int usb_audio_probe(struct usb_interface *intf,
			   const struct usb_device_id *id)
{
	struct snd_usb_audio *chip;
	chip = snd_usb_audio_probe(interface_to_usbdev(intf), intf, id);
	if (chip) {
		usb_set_intfdata(intf, chip);
		return 0;
	} else
		return -EIO;
}

static void usb_audio_disconnect(struct usb_interface *intf)
{
	snd_usb_audio_disconnect(interface_to_usbdev(intf),
				 usb_get_intfdata(intf));
}

#ifdef CONFIG_PM

int snd_usb_autoresume(struct snd_usb_audio *chip)
{
	int err = -ENODEV;

	down_read(&chip->shutdown_rwsem);
	if (chip->probing)
		err = 0;
	else if (!chip->shutdown)
		err = usb_autopm_get_interface(chip->pm_intf);
	up_read(&chip->shutdown_rwsem);

	return err;
=======
/* lock the shutdown (disconnect) task and autoresume */
int snd_usb_lock_shutdown(struct snd_usb_audio *chip)
{
	int err;

	atomic_inc(&chip->usage_count);
	if (atomic_read(&chip->shutdown)) {
		err = -EIO;
		goto error;
	}
	err = snd_usb_autoresume(chip);
	if (err < 0)
		goto error;
	return 0;

 error:
	if (atomic_dec_and_test(&chip->usage_count))
		wake_up(&chip->shutdown_wait);
	return err;
}

/* autosuspend and unlock the shutdown */
void snd_usb_unlock_shutdown(struct snd_usb_audio *chip)
{
	snd_usb_autosuspend(chip);
	if (atomic_dec_and_test(&chip->usage_count))
		wake_up(&chip->shutdown_wait);
}

int snd_usb_autoresume(struct snd_usb_audio *chip)
{
	int i, err;

	if (atomic_read(&chip->shutdown))
		return -EIO;
	if (atomic_inc_return(&chip->active) != 1)
		return 0;

	for (i = 0; i < chip->num_interfaces; i++) {
		err = usb_autopm_get_interface(chip->intf[i]);
		if (err < 0) {
			/* rollback */
			while (--i >= 0)
				usb_autopm_put_interface(chip->intf[i]);
			atomic_dec(&chip->active);
			return err;
		}
	}
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void snd_usb_autosuspend(struct snd_usb_audio *chip)
{
<<<<<<< HEAD
	down_read(&chip->shutdown_rwsem);
	if (!chip->shutdown && !chip->probing)
		usb_autopm_put_interface(chip->pm_intf);
	up_read(&chip->shutdown_rwsem);
=======
	int i;

	if (atomic_read(&chip->shutdown))
		return;
	if (!atomic_dec_and_test(&chip->active))
		return;

	for (i = 0; i < chip->num_interfaces; i++)
		usb_autopm_put_interface(chip->intf[i]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int usb_audio_suspend(struct usb_interface *intf, pm_message_t message)
{
	struct snd_usb_audio *chip = usb_get_intfdata(intf);
<<<<<<< HEAD
	struct list_head *p;
	struct snd_usb_stream *as;
	struct usb_mixer_interface *mixer;

	if (chip == (void *)-1L)
		return 0;

	if (!PMSG_IS_AUTO(message)) {
		snd_power_change_state(chip->card, SNDRV_CTL_POWER_D3hot);
		if (!chip->num_suspended_intf++) {
			list_for_each(p, &chip->pcm_list) {
				as = list_entry(p, struct snd_usb_stream, list);
				snd_pcm_suspend_all(as->pcm);
			}
 		}
	} else {
		/*
		 * otherwise we keep the rest of the system in the dark
		 * to keep this transparent
		 */
		if (!chip->num_suspended_intf++)
			chip->autosuspended = 1;
	}

	list_for_each_entry(mixer, &chip->mixer_list, list)
		snd_usb_mixer_inactivate(mixer);
=======
	struct snd_usb_stream *as;
	struct snd_usb_endpoint *ep;
	struct usb_mixer_interface *mixer;
	struct list_head *p;

	if (chip == USB_AUDIO_IFACE_UNUSED)
		return 0;

	if (!chip->num_suspended_intf++) {
		list_for_each_entry(as, &chip->pcm_list, list)
			snd_usb_pcm_suspend(as);
		list_for_each_entry(ep, &chip->ep_list, list)
			snd_usb_endpoint_suspend(ep);
		list_for_each(p, &chip->midi_list)
			snd_usbmidi_suspend(p);
		list_for_each_entry(mixer, &chip->mixer_list, list)
			snd_usb_mixer_suspend(mixer);
		snd_usb_midi_v2_suspend_all(chip);
	}

	if (!PMSG_IS_AUTO(message) && !chip->system_suspend) {
		snd_power_change_state(chip->card, SNDRV_CTL_POWER_D3hot);
		chip->system_suspend = chip->num_suspended_intf;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static int usb_audio_resume(struct usb_interface *intf)
{
	struct snd_usb_audio *chip = usb_get_intfdata(intf);
<<<<<<< HEAD
	struct usb_mixer_interface *mixer;
	int err = 0;

	if (chip == (void *)-1L)
		return 0;
	if (--chip->num_suspended_intf)
		return 0;
=======
	struct snd_usb_stream *as;
	struct usb_mixer_interface *mixer;
	struct list_head *p;
	int err = 0;

	if (chip == USB_AUDIO_IFACE_UNUSED)
		return 0;

	atomic_inc(&chip->active); /* avoid autopm */
	if (chip->num_suspended_intf > 1)
		goto out;

	list_for_each_entry(as, &chip->pcm_list, list) {
		err = snd_usb_pcm_resume(as);
		if (err < 0)
			goto err_out;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * ALSA leaves material resumption to user space
	 * we just notify and restart the mixers
	 */
	list_for_each_entry(mixer, &chip->mixer_list, list) {
<<<<<<< HEAD
		err = snd_usb_mixer_activate(mixer);
=======
		err = snd_usb_mixer_resume(mixer);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (err < 0)
			goto err_out;
	}

<<<<<<< HEAD
	if (!chip->autosuspended)
		snd_power_change_state(chip->card, SNDRV_CTL_POWER_D0);
	chip->autosuspended = 0;

err_out:
	return err;
}
#else
#define usb_audio_suspend	NULL
#define usb_audio_resume	NULL
#endif		/* CONFIG_PM */

static struct usb_device_id usb_audio_ids [] = {
=======
	list_for_each(p, &chip->midi_list) {
		snd_usbmidi_resume(p);
	}

	snd_usb_midi_v2_resume_all(chip);

 out:
	if (chip->num_suspended_intf == chip->system_suspend) {
		snd_power_change_state(chip->card, SNDRV_CTL_POWER_D0);
		chip->system_suspend = 0;
	}
	chip->num_suspended_intf--;

err_out:
	atomic_dec(&chip->active); /* allow autopm after this point */
	return err;
}

static const struct usb_device_id usb_audio_ids [] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "quirks-table.h"
    { .match_flags = (USB_DEVICE_ID_MATCH_INT_CLASS | USB_DEVICE_ID_MATCH_INT_SUBCLASS),
      .bInterfaceClass = USB_CLASS_AUDIO,
      .bInterfaceSubClass = USB_SUBCLASS_AUDIOCONTROL },
    { }						/* Terminating entry */
};
<<<<<<< HEAD

MODULE_DEVICE_TABLE (usb, usb_audio_ids);
=======
MODULE_DEVICE_TABLE(usb, usb_audio_ids);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * entry point for linux usb interface
 */

static struct usb_driver usb_audio_driver = {
	.name =		"snd-usb-audio",
	.probe =	usb_audio_probe,
	.disconnect =	usb_audio_disconnect,
	.suspend =	usb_audio_suspend,
	.resume =	usb_audio_resume,
<<<<<<< HEAD
=======
	.reset_resume =	usb_audio_resume,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.id_table =	usb_audio_ids,
	.supports_autosuspend = 1,
};

<<<<<<< HEAD
static int __init snd_usb_audio_init(void)
{
	if (nrpacks < 1 || nrpacks > MAX_PACKS) {
		printk(KERN_WARNING "invalid nrpacks value.\n");
		return -EINVAL;
	}

	return usb_register(&usb_audio_driver);
}

static void __exit snd_usb_audio_cleanup(void)
{
	usb_deregister(&usb_audio_driver);
}

module_init(snd_usb_audio_init);
module_exit(snd_usb_audio_cleanup);
=======
module_usb_driver(usb_audio_driver);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
