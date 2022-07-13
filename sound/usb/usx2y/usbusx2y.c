<<<<<<< HEAD
/*
 * usbusy2y.c - ALSA USB US-428 Driver
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * usbusx2y.c - ALSA USB US-428 Driver
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
2005-04-14 Karsten Wiese
	Version 0.8.7.2:
	Call snd_card_free() instead of snd_card_free_in_thread() to prevent oops with dead keyboard symptom.
	Tested ok with kernel 2.6.12-rc2.

2004-12-14 Karsten Wiese
	Version 0.8.7.1:
	snd_pcm_open for rawusb pcm-devices now returns -EBUSY if called without rawusb's hwdep device being open.

2004-12-02 Karsten Wiese
	Version 0.8.7:
	Use macro usb_maxpacket() for portability.

2004-10-26 Karsten Wiese
	Version 0.8.6:
<<<<<<< HEAD
	wake_up() process waiting in usX2Y_urbs_start() on error.
=======
	wake_up() process waiting in usx2y_urbs_start() on error.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

2004-10-21 Karsten Wiese
	Version 0.8.5:
	nrpacks is runtime or compiletime configurable now with tested values from 1 to 4.

2004-10-03 Karsten Wiese
	Version 0.8.2:
	Avoid any possible racing while in prepare callback.

2004-09-30 Karsten Wiese
	Version 0.8.0:
	Simplified things and made ohci work again.

2004-09-20 Karsten Wiese
	Version 0.7.3:
	Use usb_kill_urb() instead of deprecated (kernel 2.6.9) usb_unlink_urb().

2004-07-13 Karsten Wiese
	Version 0.7.1:
	Don't sleep in START/STOP callbacks anymore.
	us428 channels C/D not handled just for this version, sorry.

2004-06-21 Karsten Wiese
	Version 0.6.4:
	Temporarely suspend midi input
	to sanely call usb_set_interface() when setting format.

2004-06-12 Karsten Wiese
	Version 0.6.3:
	Made it thus the following rule is enforced:
<<<<<<< HEAD
	"All pcm substreams of one usX2Y have to operate at the same rate & format."
=======
	"All pcm substreams of one usx2y have to operate at the same rate & format."
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

2004-04-06 Karsten Wiese
	Version 0.6.0:
	Runs on 2.6.5 kernel without any "--with-debug=" things.
	us224 reported running.

2004-01-14 Karsten Wiese
	Version 0.5.1:
	Runs with 2.6.1 kernel.

2003-12-30 Karsten Wiese
	Version 0.4.1:
	Fix 24Bit 4Channel capturing for the us428.

2003-11-27 Karsten Wiese, Martin Langer
	Version 0.4:
	us122 support.
	us224 could be tested by uncommenting the sections containing USB_ID_US224

2003-11-03 Karsten Wiese
	Version 0.3:
<<<<<<< HEAD
	24Bit support. 
=======
	24Bit support.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	"arecord -D hw:1 -c 2 -r 48000 -M -f S24_3LE|aplay -D hw:1 -c 2 -r 48000 -M -f S24_3LE" works.

2003-08-22 Karsten Wiese
	Version 0.0.8:
	Removed EZUSB Firmware. First Stage Firmwaredownload is now done by tascam-firmware downloader.
	See:
	http://usb-midi-fw.sourceforge.net/tascam-firmware.tar.gz

2003-06-18 Karsten Wiese
	Version 0.0.5:
	changed to compile with kernel 2.4.21 and alsa 0.9.4

2002-10-16 Karsten Wiese
	Version 0.0.4:
	compiles again with alsa-current.
	USB_ISO_ASAP not used anymore (most of the time), instead
	urb->start_frame is calculated here now, some calls inside usb-driver don't need to happen anymore.

	To get the best out of this:
	Disable APM-support in the kernel as APM-BIOS calls (once each second) hard disable interrupt for many precious milliseconds.
	This helped me much on my slowish PII 400 & PIII 500.
	ACPI yet untested but might cause the same bad behaviour.
	Use a kernel with lowlatency and preemptiv patches applied.
<<<<<<< HEAD
	To autoload snd-usb-midi append a line 
=======
	To autoload snd-usb-midi append a line
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		post-install snd-usb-us428 modprobe snd-usb-midi
	to /etc/modules.conf.

	known problems:
	sliders, knobs, lights not yet handled except MASTER Volume slider.
<<<<<<< HEAD
       	"pcm -c 2" doesn't work. "pcm -c 2 -m direct_interleaved" does.
	KDE3: "Enable full duplex operation" deadlocks.

	
=======
	"pcm -c 2" doesn't work. "pcm -c 2 -m direct_interleaved" does.
	KDE3: "Enable full duplex operation" deadlocks.

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
2002-08-31 Karsten Wiese
	Version 0.0.3: audio also simplex;
	simplifying: iso urbs only 1 packet, melted structs.
	ASYNC_UNLINK not used anymore: no more crashes so far.....
	for alsa 0.9 rc3.

2002-08-09 Karsten Wiese
	Version 0.0.2: midi works with snd-usb-midi, audio (only fullduplex now) with i.e. bristol.
	The firmware has been sniffed from win2k us-428 driver 3.09.

 *   Copyright (c) 2002 - 2004 Karsten Wiese
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
*/
=======
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/usb.h>
#include <sound/core.h>
#include <sound/initval.h>
#include <sound/pcm.h>

#include <sound/rawmidi.h>
#include "usx2y.h"
#include "usbusx2y.h"
#include "usX2Yhwdep.h"

<<<<<<< HEAD


MODULE_AUTHOR("Karsten Wiese <annabellesgarden@yahoo.de>");
MODULE_DESCRIPTION("TASCAM "NAME_ALLCAPS" Version 0.8.7.2");
MODULE_LICENSE("GPL");
MODULE_SUPPORTED_DEVICE("{{TASCAM(0x1604), "NAME_ALLCAPS"(0x8001)(0x8005)(0x8007) }}");

static int index[SNDRV_CARDS] = SNDRV_DEFAULT_IDX; /* Index 0-max */
static char* id[SNDRV_CARDS] = SNDRV_DEFAULT_STR; /* Id for this card */
=======
MODULE_AUTHOR("Karsten Wiese <annabellesgarden@yahoo.de>");
MODULE_DESCRIPTION("TASCAM "NAME_ALLCAPS" Version 0.8.7.2");
MODULE_LICENSE("GPL");

static int index[SNDRV_CARDS] = SNDRV_DEFAULT_IDX; /* Index 0-max */
static char *id[SNDRV_CARDS] = SNDRV_DEFAULT_STR; /* Id for this card */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static bool enable[SNDRV_CARDS] = SNDRV_DEFAULT_ENABLE_PNP; /* Enable this card */

module_param_array(index, int, NULL, 0444);
MODULE_PARM_DESC(index, "Index value for "NAME_ALLCAPS".");
module_param_array(id, charp, NULL, 0444);
MODULE_PARM_DESC(id, "ID string for "NAME_ALLCAPS".");
module_param_array(enable, bool, NULL, 0444);
MODULE_PARM_DESC(enable, "Enable "NAME_ALLCAPS".");

<<<<<<< HEAD

static int snd_usX2Y_card_used[SNDRV_CARDS];

static void usX2Y_usb_disconnect(struct usb_device* usb_device, void* ptr);
static void snd_usX2Y_card_private_free(struct snd_card *card);

/* 
 * pipe 4 is used for switching the lamps, setting samplerate, volumes ....   
 */
static void i_usX2Y_Out04Int(struct urb *urb)
{
#ifdef CONFIG_SND_DEBUG
	if (urb->status) {
		int 		i;
		struct usX2Ydev *usX2Y = urb->context;
		for (i = 0; i < 10 && usX2Y->AS04.urb[i] != urb; i++);
		snd_printdd("i_usX2Y_Out04Int() urb %i status=%i\n", i, urb->status);
=======
static int snd_usx2y_card_used[SNDRV_CARDS];

static void snd_usx2y_card_private_free(struct snd_card *card);
static void usx2y_unlinkseq(struct snd_usx2y_async_seq *s);

/*
 * pipe 4 is used for switching the lamps, setting samplerate, volumes ....
 */
static void i_usx2y_out04_int(struct urb *urb)
{
#ifdef CONFIG_SND_DEBUG
	if (urb->status) {
		int i;
		struct usx2ydev *usx2y = urb->context;

		for (i = 0; i < 10 && usx2y->as04.urb[i] != urb; i++)
			;
		snd_printdd("%s urb %i status=%i\n", __func__, i, urb->status);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
#endif
}

<<<<<<< HEAD
static void i_usX2Y_In04Int(struct urb *urb)
{
	int			err = 0;
	struct usX2Ydev		*usX2Y = urb->context;
	struct us428ctls_sharedmem	*us428ctls = usX2Y->us428ctls_sharedmem;

	usX2Y->In04IntCalls++;
=======
static void i_usx2y_in04_int(struct urb *urb)
{
	int			err = 0;
	struct usx2ydev		*usx2y = urb->context;
	struct us428ctls_sharedmem	*us428ctls = usx2y->us428ctls_sharedmem;
	struct us428_p4out *p4out;
	int i, j, n, diff, send;

	usx2y->in04_int_calls++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (urb->status) {
		snd_printdd("Interrupt Pipe 4 came back with status=%i\n", urb->status);
		return;
	}

<<<<<<< HEAD
	//	printk("%i:0x%02X ", 8, (int)((unsigned char*)usX2Y->In04Buf)[8]); Master volume shows 0 here if fader is at max during boot ?!?
	if (us428ctls) {
		int diff = -1;
		if (-2 == us428ctls->CtlSnapShotLast) {
			diff = 0;
			memcpy(usX2Y->In04Last, usX2Y->In04Buf, sizeof(usX2Y->In04Last));
			us428ctls->CtlSnapShotLast = -1;
		} else {
			int i;
			for (i = 0; i < 21; i++) {
				if (usX2Y->In04Last[i] != ((char*)usX2Y->In04Buf)[i]) {
					if (diff < 0)
						diff = i;
					usX2Y->In04Last[i] = ((char*)usX2Y->In04Buf)[i];
				}
			}
		}
		if (0 <= diff) {
			int n = us428ctls->CtlSnapShotLast + 1;
			if (n >= N_us428_ctl_BUFS  ||  n < 0)
				n = 0;
			memcpy(us428ctls->CtlSnapShot + n, usX2Y->In04Buf, sizeof(us428ctls->CtlSnapShot[0]));
			us428ctls->CtlSnapShotDiffersAt[n] = diff;
			us428ctls->CtlSnapShotLast = n;
			wake_up(&usX2Y->us428ctls_wait_queue_head);
		}
	}
	
	
	if (usX2Y->US04) {
		if (0 == usX2Y->US04->submitted)
			do {
				err = usb_submit_urb(usX2Y->US04->urb[usX2Y->US04->submitted++], GFP_ATOMIC);
			} while (!err && usX2Y->US04->submitted < usX2Y->US04->len);
	} else
		if (us428ctls && us428ctls->p4outLast >= 0 && us428ctls->p4outLast < N_us428_p4out_BUFS) {
			if (us428ctls->p4outLast != us428ctls->p4outSent) {
				int j, send = us428ctls->p4outSent + 1;
				if (send >= N_us428_p4out_BUFS)
					send = 0;
				for (j = 0; j < URBS_AsyncSeq  &&  !err; ++j)
					if (0 == usX2Y->AS04.urb[j]->status) {
						struct us428_p4out *p4out = us428ctls->p4out + send;	// FIXME if more than 1 p4out is new, 1 gets lost.
						usb_fill_bulk_urb(usX2Y->AS04.urb[j], usX2Y->dev,
								  usb_sndbulkpipe(usX2Y->dev, 0x04), &p4out->val.vol,
								  p4out->type == eLT_Light ? sizeof(struct us428_lights) : 5,
								  i_usX2Y_Out04Int, usX2Y);
						err = usb_submit_urb(usX2Y->AS04.urb[j], GFP_ATOMIC);
						us428ctls->p4outSent = send;
						break;
					}
			}
		}

	if (err)
		snd_printk(KERN_ERR "In04Int() usb_submit_urb err=%i\n", err);

	urb->dev = usX2Y->dev;
=======
	//	printk("%i:0x%02X ", 8, (int)((unsigned char*)usx2y->in04_buf)[8]); Master volume shows 0 here if fader is at max during boot ?!?
	if (us428ctls) {
		diff = -1;
		if (us428ctls->ctl_snapshot_last == -2) {
			diff = 0;
			memcpy(usx2y->in04_last, usx2y->in04_buf, sizeof(usx2y->in04_last));
			us428ctls->ctl_snapshot_last = -1;
		} else {
			for (i = 0; i < 21; i++) {
				if (usx2y->in04_last[i] != ((char *)usx2y->in04_buf)[i]) {
					if (diff < 0)
						diff = i;
					usx2y->in04_last[i] = ((char *)usx2y->in04_buf)[i];
				}
			}
		}
		if (diff >= 0) {
			n = us428ctls->ctl_snapshot_last + 1;
			if (n >= N_US428_CTL_BUFS || n < 0)
				n = 0;
			memcpy(us428ctls->ctl_snapshot + n, usx2y->in04_buf, sizeof(us428ctls->ctl_snapshot[0]));
			us428ctls->ctl_snapshot_differs_at[n] = diff;
			us428ctls->ctl_snapshot_last = n;
			wake_up(&usx2y->us428ctls_wait_queue_head);
		}
	}

	if (usx2y->us04) {
		if (!usx2y->us04->submitted) {
			do {
				err = usb_submit_urb(usx2y->us04->urb[usx2y->us04->submitted++], GFP_ATOMIC);
			} while (!err && usx2y->us04->submitted < usx2y->us04->len);
		}
	} else {
		if (us428ctls && us428ctls->p4out_last >= 0 && us428ctls->p4out_last < N_US428_P4OUT_BUFS) {
			if (us428ctls->p4out_last != us428ctls->p4out_sent) {
				send = us428ctls->p4out_sent + 1;
				if (send >= N_US428_P4OUT_BUFS)
					send = 0;
				for (j = 0; j < URBS_ASYNC_SEQ && !err; ++j) {
					if (!usx2y->as04.urb[j]->status) {
						p4out = us428ctls->p4out + send;	// FIXME if more than 1 p4out is new, 1 gets lost.
						usb_fill_bulk_urb(usx2y->as04.urb[j], usx2y->dev,
								  usb_sndbulkpipe(usx2y->dev, 0x04), &p4out->val.vol,
								  p4out->type == ELT_LIGHT ? sizeof(struct us428_lights) : 5,
								  i_usx2y_out04_int, usx2y);
						err = usb_submit_urb(usx2y->as04.urb[j], GFP_ATOMIC);
						us428ctls->p4out_sent = send;
						break;
					}
				}
			}
		}
	}

	if (err)
		snd_printk(KERN_ERR "in04_int() usb_submit_urb err=%i\n", err);

	urb->dev = usx2y->dev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	usb_submit_urb(urb, GFP_ATOMIC);
}

/*
 * Prepare some urbs
 */
<<<<<<< HEAD
int usX2Y_AsyncSeq04_init(struct usX2Ydev *usX2Y)
{
	int	err = 0,
		i;

	if (NULL == (usX2Y->AS04.buffer = kmalloc(URB_DataLen_AsyncSeq*URBS_AsyncSeq, GFP_KERNEL))) {
		err = -ENOMEM;
	} else
		for (i = 0; i < URBS_AsyncSeq; ++i) {
			if (NULL == (usX2Y->AS04.urb[i] = usb_alloc_urb(0, GFP_KERNEL))) {
				err = -ENOMEM;
				break;
			}
			usb_fill_bulk_urb(	usX2Y->AS04.urb[i], usX2Y->dev,
						usb_sndbulkpipe(usX2Y->dev, 0x04),
						usX2Y->AS04.buffer + URB_DataLen_AsyncSeq*i, 0,
						i_usX2Y_Out04Int, usX2Y
				);
		}
	return err;
}

int usX2Y_In04_init(struct usX2Ydev *usX2Y)
{
	if (! (usX2Y->In04urb = usb_alloc_urb(0, GFP_KERNEL)))
		return -ENOMEM;

	if (! (usX2Y->In04Buf = kmalloc(21, GFP_KERNEL))) {
		usb_free_urb(usX2Y->In04urb);
		return -ENOMEM;
	}
	 
	init_waitqueue_head(&usX2Y->In04WaitQueue);
	usb_fill_int_urb(usX2Y->In04urb, usX2Y->dev, usb_rcvintpipe(usX2Y->dev, 0x4),
			 usX2Y->In04Buf, 21,
			 i_usX2Y_In04Int, usX2Y,
			 10);
	return usb_submit_urb(usX2Y->In04urb, GFP_KERNEL);
}

static void usX2Y_unlinkSeq(struct snd_usX2Y_AsyncSeq *S)
{
	int	i;
	for (i = 0; i < URBS_AsyncSeq; ++i) {
		if (S[i].urb) {
			usb_kill_urb(S->urb[i]);
			usb_free_urb(S->urb[i]);
			S->urb[i] = NULL;
		}
	}
	kfree(S->buffer);
}


static struct usb_device_id snd_usX2Y_usb_id_table[] = {
	{
		.match_flags =	USB_DEVICE_ID_MATCH_DEVICE,
		.idVendor =	0x1604,
		.idProduct =	USB_ID_US428 
=======
int usx2y_async_seq04_init(struct usx2ydev *usx2y)
{
	int	err = 0, i;

	if (WARN_ON(usx2y->as04.buffer))
		return -EBUSY;

	usx2y->as04.buffer = kmalloc_array(URBS_ASYNC_SEQ,
					   URB_DATA_LEN_ASYNC_SEQ, GFP_KERNEL);
	if (!usx2y->as04.buffer) {
		err = -ENOMEM;
	} else {
		for (i = 0; i < URBS_ASYNC_SEQ; ++i) {
			usx2y->as04.urb[i] = usb_alloc_urb(0, GFP_KERNEL);
			if (!usx2y->as04.urb[i]) {
				err = -ENOMEM;
				break;
			}
			usb_fill_bulk_urb(usx2y->as04.urb[i], usx2y->dev,
					  usb_sndbulkpipe(usx2y->dev, 0x04),
					  usx2y->as04.buffer + URB_DATA_LEN_ASYNC_SEQ * i, 0,
					  i_usx2y_out04_int, usx2y);
			err = usb_urb_ep_type_check(usx2y->as04.urb[i]);
			if (err < 0)
				break;
		}
	}
	if (err)
		usx2y_unlinkseq(&usx2y->as04);
	return err;
}

int usx2y_in04_init(struct usx2ydev *usx2y)
{
	int err;

	if (WARN_ON(usx2y->in04_urb))
		return -EBUSY;

	usx2y->in04_urb = usb_alloc_urb(0, GFP_KERNEL);
	if (!usx2y->in04_urb) {
		err = -ENOMEM;
		goto error;
	}

	usx2y->in04_buf = kmalloc(21, GFP_KERNEL);
	if (!usx2y->in04_buf) {
		err = -ENOMEM;
		goto error;
	}

	init_waitqueue_head(&usx2y->in04_wait_queue);
	usb_fill_int_urb(usx2y->in04_urb, usx2y->dev, usb_rcvintpipe(usx2y->dev, 0x4),
			 usx2y->in04_buf, 21,
			 i_usx2y_in04_int, usx2y,
			 10);
	if (usb_urb_ep_type_check(usx2y->in04_urb)) {
		err = -EINVAL;
		goto error;
	}
	return usb_submit_urb(usx2y->in04_urb, GFP_KERNEL);

 error:
	kfree(usx2y->in04_buf);
	usb_free_urb(usx2y->in04_urb);
	usx2y->in04_buf = NULL;
	usx2y->in04_urb = NULL;
	return err;
}

static void usx2y_unlinkseq(struct snd_usx2y_async_seq *s)
{
	int	i;

	for (i = 0; i < URBS_ASYNC_SEQ; ++i) {
		if (!s->urb[i])
			continue;
		usb_kill_urb(s->urb[i]);
		usb_free_urb(s->urb[i]);
		s->urb[i] = NULL;
	}
	kfree(s->buffer);
	s->buffer = NULL;
}

static const struct usb_device_id snd_usx2y_usb_id_table[] = {
	{
		.match_flags =	USB_DEVICE_ID_MATCH_DEVICE,
		.idVendor =	0x1604,
		.idProduct =	USB_ID_US428
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
	{
		.match_flags =	USB_DEVICE_ID_MATCH_DEVICE,
		.idVendor =	0x1604,
<<<<<<< HEAD
		.idProduct =	USB_ID_US122 
	},
 	{
=======
		.idProduct =	USB_ID_US122
	},
	{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.match_flags =	USB_DEVICE_ID_MATCH_DEVICE,
		.idVendor =	0x1604,
		.idProduct =	USB_ID_US224
	},
	{ /* terminator */ }
};
<<<<<<< HEAD

static int usX2Y_create_card(struct usb_device *device, struct snd_card **cardp)
{
	int		dev;
	struct snd_card *	card;
	int err;

	for (dev = 0; dev < SNDRV_CARDS; ++dev)
		if (enable[dev] && !snd_usX2Y_card_used[dev])
			break;
	if (dev >= SNDRV_CARDS)
		return -ENODEV;
	err = snd_card_create(index[dev], id[dev], THIS_MODULE,
			      sizeof(struct usX2Ydev), &card);
	if (err < 0)
		return err;
	snd_usX2Y_card_used[usX2Y(card)->card_index = dev] = 1;
	card->private_free = snd_usX2Y_card_private_free;
	usX2Y(card)->dev = device;
	init_waitqueue_head(&usX2Y(card)->prepare_wait_queue);
	mutex_init(&usX2Y(card)->prepare_mutex);
	INIT_LIST_HEAD(&usX2Y(card)->midi_list);
	strcpy(card->driver, "USB "NAME_ALLCAPS"");
	sprintf(card->shortname, "TASCAM "NAME_ALLCAPS"");
	sprintf(card->longname, "%s (%x:%x if %d at %03d/%03d)",
		card->shortname, 
		le16_to_cpu(device->descriptor.idVendor),
		le16_to_cpu(device->descriptor.idProduct),
		0,//us428(card)->usbmidi.ifnum,
		usX2Y(card)->dev->bus->busnum, usX2Y(card)->dev->devnum
		);
=======
MODULE_DEVICE_TABLE(usb, snd_usx2y_usb_id_table);

static int usx2y_create_card(struct usb_device *device,
			     struct usb_interface *intf,
			     struct snd_card **cardp)
{
	int		dev;
	struct snd_card *card;
	int err;

	for (dev = 0; dev < SNDRV_CARDS; ++dev)
		if (enable[dev] && !snd_usx2y_card_used[dev])
			break;
	if (dev >= SNDRV_CARDS)
		return -ENODEV;
	err = snd_card_new(&intf->dev, index[dev], id[dev], THIS_MODULE,
			   sizeof(struct usx2ydev), &card);
	if (err < 0)
		return err;
	snd_usx2y_card_used[usx2y(card)->card_index = dev] = 1;
	card->private_free = snd_usx2y_card_private_free;
	usx2y(card)->dev = device;
	init_waitqueue_head(&usx2y(card)->prepare_wait_queue);
	init_waitqueue_head(&usx2y(card)->us428ctls_wait_queue_head);
	mutex_init(&usx2y(card)->pcm_mutex);
	INIT_LIST_HEAD(&usx2y(card)->midi_list);
	strcpy(card->driver, "USB "NAME_ALLCAPS"");
	sprintf(card->shortname, "TASCAM "NAME_ALLCAPS"");
	sprintf(card->longname, "%s (%x:%x if %d at %03d/%03d)",
		card->shortname,
		le16_to_cpu(device->descriptor.idVendor),
		le16_to_cpu(device->descriptor.idProduct),
		0,//us428(card)->usbmidi.ifnum,
		usx2y(card)->dev->bus->busnum, usx2y(card)->dev->devnum);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	*cardp = card;
	return 0;
}

<<<<<<< HEAD

static int usX2Y_usb_probe(struct usb_device *device,
			   struct usb_interface *intf,
			   const struct usb_device_id *device_id,
			   struct snd_card **cardp)
{
	int		err;
	struct snd_card *	card;

	*cardp = NULL;
=======
static void snd_usx2y_card_private_free(struct snd_card *card)
{
	struct usx2ydev *usx2y = usx2y(card);

	kfree(usx2y->in04_buf);
	usb_free_urb(usx2y->in04_urb);
	if (usx2y->us428ctls_sharedmem)
		free_pages_exact(usx2y->us428ctls_sharedmem,
				 US428_SHAREDMEM_PAGES);
	if (usx2y->card_index >= 0 && usx2y->card_index < SNDRV_CARDS)
		snd_usx2y_card_used[usx2y->card_index] = 0;
}

static void snd_usx2y_disconnect(struct usb_interface *intf)
{
	struct snd_card *card;
	struct usx2ydev *usx2y;
	struct list_head *p;

	card = usb_get_intfdata(intf);
	if (!card)
		return;
	usx2y = usx2y(card);
	usx2y->chip_status = USX2Y_STAT_CHIP_HUP;
	usx2y_unlinkseq(&usx2y->as04);
	usb_kill_urb(usx2y->in04_urb);
	snd_card_disconnect(card);

	/* release the midi resources */
	list_for_each(p, &usx2y->midi_list) {
		snd_usbmidi_disconnect(p);
	}
	if (usx2y->us428ctls_sharedmem)
		wake_up(&usx2y->us428ctls_wait_queue_head);
	snd_card_free(card);
}

static int snd_usx2y_probe(struct usb_interface *intf,
			   const struct usb_device_id *id)
{
	struct usb_device *device = interface_to_usbdev(intf);
	struct snd_card *card;
	int err;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (le16_to_cpu(device->descriptor.idVendor) != 0x1604 ||
	    (le16_to_cpu(device->descriptor.idProduct) != USB_ID_US122 &&
	     le16_to_cpu(device->descriptor.idProduct) != USB_ID_US224 &&
	     le16_to_cpu(device->descriptor.idProduct) != USB_ID_US428))
		return -EINVAL;

<<<<<<< HEAD
	err = usX2Y_create_card(device, &card);
	if (err < 0)
		return err;
	snd_card_set_dev(card, &intf->dev);
	if ((err = usX2Y_hwdep_new(card, device)) < 0  ||
	    (err = snd_card_register(card)) < 0) {
		snd_card_free(card);
		return err;
	}
	*cardp = card;
	return 0;
}

/*
 * new 2.5 USB kernel API
 */
static int snd_usX2Y_probe(struct usb_interface *intf, const struct usb_device_id *id)
{
	struct snd_card *card;
	int err;

	err = usX2Y_usb_probe(interface_to_usbdev(intf), intf, id, &card);
	if (err < 0)
		return err;
	dev_set_drvdata(&intf->dev, card);
	return 0;
}

static void snd_usX2Y_disconnect(struct usb_interface *intf)
{
	usX2Y_usb_disconnect(interface_to_usbdev(intf),
				 usb_get_intfdata(intf));
}

MODULE_DEVICE_TABLE(usb, snd_usX2Y_usb_id_table);
static struct usb_driver snd_usX2Y_usb_driver = {
	.name =		"snd-usb-usx2y",
	.probe =	snd_usX2Y_probe,
	.disconnect =	snd_usX2Y_disconnect,
	.id_table =	snd_usX2Y_usb_id_table,
};

static void snd_usX2Y_card_private_free(struct snd_card *card)
{
	kfree(usX2Y(card)->In04Buf);
	usb_free_urb(usX2Y(card)->In04urb);
	if (usX2Y(card)->us428ctls_sharedmem)
		snd_free_pages(usX2Y(card)->us428ctls_sharedmem, sizeof(*usX2Y(card)->us428ctls_sharedmem));
	if (usX2Y(card)->card_index >= 0  &&  usX2Y(card)->card_index < SNDRV_CARDS)
		snd_usX2Y_card_used[usX2Y(card)->card_index] = 0;
}

/*
 * Frees the device.
 */
static void usX2Y_usb_disconnect(struct usb_device *device, void* ptr)
{
	if (ptr) {
		struct snd_card *card = ptr;
		struct usX2Ydev *usX2Y = usX2Y(card);
		struct list_head *p;
		usX2Y->chip_status = USX2Y_STAT_CHIP_HUP;
		usX2Y_unlinkSeq(&usX2Y->AS04);
		usb_kill_urb(usX2Y->In04urb);
		snd_card_disconnect(card);
		/* release the midi resources */
		list_for_each(p, &usX2Y->midi_list) {
			snd_usbmidi_disconnect(p);
		}
		if (usX2Y->us428ctls_sharedmem) 
			wake_up(&usX2Y->us428ctls_wait_queue_head);
		snd_card_free(card);
	}
}

module_usb_driver(snd_usX2Y_usb_driver);
=======
	err = usx2y_create_card(device, intf, &card);
	if (err < 0)
		return err;
	err = usx2y_hwdep_new(card, device);
	if (err < 0)
		goto error;
	err = snd_card_register(card);
	if (err < 0)
		goto error;

	dev_set_drvdata(&intf->dev, card);
	return 0;

 error:
	snd_card_free(card);
	return err;
}

static struct usb_driver snd_usx2y_usb_driver = {
	.name =		"snd-usb-usx2y",
	.probe =	snd_usx2y_probe,
	.disconnect =	snd_usx2y_disconnect,
	.id_table =	snd_usx2y_usb_id_table,
};
module_usb_driver(snd_usx2y_usb_driver);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
