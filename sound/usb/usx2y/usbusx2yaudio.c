<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *   US-X2Y AUDIO
 *   Copyright (c) 2002-2004 by Karsten Wiese
 *
 *   based on
 *
 *   (Tentative) USB Audio Driver for ALSA
 *
 *   Main and PCM part
 *
 *   Copyright (c) 2002 by Takashi Iwai <tiwai@suse.de>
 *
<<<<<<< HEAD
 *   Many codes borrowed from audio.c by 
 *	    Alan Cox (alan@lxorguk.ukuu.org.uk)
 *	    Thomas Sailer (sailer@ife.ee.ethz.ch)
 *
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
=======
 *   Many codes borrowed from audio.c by
 *	    Alan Cox (alan@lxorguk.ukuu.org.uk)
 *	    Thomas Sailer (sailer@ife.ee.ethz.ch)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */


#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/usb.h>
#include <linux/moduleparam.h>
#include <sound/core.h>
#include <sound/info.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include "usx2y.h"
#include "usbusx2y.h"

<<<<<<< HEAD
#define USX2Y_NRPACKS 4			/* Default value used for nr of packs per urb.
					  1 to 4 have been tested ok on uhci.
					  To use 3 on ohci, you'd need a patch:
					  look for "0000425-linux-2.6.9-rc4-mm1_ohci-hcd.patch.gz" on
					  "https://bugtrack.alsa-project.org/alsa-bug/bug_view_page.php?bug_id=0000425"
					  .
					  1, 2 and 4 work out of the box on ohci, if I recall correctly.
					  Bigger is safer operation,
					  smaller gives lower latencies.
					*/
#define USX2Y_NRPACKS_VARIABLE y	/* If your system works ok with this module's parameter
					   nrpacks set to 1, you might as well comment 
					   this #define out, and thereby produce smaller, faster code.
					   You'd also set USX2Y_NRPACKS to 1 then.
					*/

#ifdef USX2Y_NRPACKS_VARIABLE
 static int nrpacks = USX2Y_NRPACKS; /* number of packets per urb */
 #define  nr_of_packs() nrpacks
 module_param(nrpacks, int, 0444);
 MODULE_PARM_DESC(nrpacks, "Number of packets per URB.");
#else
 #define nr_of_packs() USX2Y_NRPACKS
#endif


static int usX2Y_urb_capt_retire(struct snd_usX2Y_substream *subs)
=======
/* Default value used for nr of packs per urb.
 * 1 to 4 have been tested ok on uhci.
 * To use 3 on ohci, you'd need a patch:
 * look for "0000425-linux-2.6.9-rc4-mm1_ohci-hcd.patch.gz" on
 * "https://bugtrack.alsa-project.org/alsa-bug/bug_view_page.php?bug_id=0000425"
 *
 * 1, 2 and 4 work out of the box on ohci, if I recall correctly.
 * Bigger is safer operation, smaller gives lower latencies.
 */
#define USX2Y_NRPACKS 4

/* If your system works ok with this module's parameter
 * nrpacks set to 1, you might as well comment
 * this define out, and thereby produce smaller, faster code.
 * You'd also set USX2Y_NRPACKS to 1 then.
 */
#define USX2Y_NRPACKS_VARIABLE 1

#ifdef USX2Y_NRPACKS_VARIABLE
static int nrpacks = USX2Y_NRPACKS; /* number of packets per urb */
#define  nr_of_packs() nrpacks
module_param(nrpacks, int, 0444);
MODULE_PARM_DESC(nrpacks, "Number of packets per URB.");
#else
#define nr_of_packs() USX2Y_NRPACKS
#endif

static int usx2y_urb_capt_retire(struct snd_usx2y_substream *subs)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct urb	*urb = subs->completed_urb;
	struct snd_pcm_runtime *runtime = subs->pcm_substream->runtime;
	unsigned char	*cp;
<<<<<<< HEAD
	int 		i, len, lens = 0, hwptr_done = subs->hwptr_done;
	struct usX2Ydev	*usX2Y = subs->usX2Y;

	for (i = 0; i < nr_of_packs(); i++) {
		cp = (unsigned char*)urb->transfer_buffer + urb->iso_frame_desc[i].offset;
		if (urb->iso_frame_desc[i].status) { /* active? hmm, skip this */
			snd_printk(KERN_ERR "active frame status %i. "
				   "Most probably some hardware problem.\n",
				   urb->iso_frame_desc[i].status);
			return urb->iso_frame_desc[i].status;
		}
		len = urb->iso_frame_desc[i].actual_length / usX2Y->stride;
		if (! len) {
=======
	int		i, len, lens = 0, hwptr_done = subs->hwptr_done;
	int		cnt, blen;
	struct usx2ydev	*usx2y = subs->usx2y;

	for (i = 0; i < nr_of_packs(); i++) {
		cp = (unsigned char *)urb->transfer_buffer + urb->iso_frame_desc[i].offset;
		if (urb->iso_frame_desc[i].status) { /* active? hmm, skip this */
			snd_printk(KERN_ERR
				   "active frame status %i. Most probably some hardware problem.\n",
				   urb->iso_frame_desc[i].status);
			return urb->iso_frame_desc[i].status;
		}
		len = urb->iso_frame_desc[i].actual_length / usx2y->stride;
		if (!len) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			snd_printd("0 == len ERROR!\n");
			continue;
		}

		/* copy a data chunk */
		if ((hwptr_done + len) > runtime->buffer_size) {
<<<<<<< HEAD
			int cnt = runtime->buffer_size - hwptr_done;
			int blen = cnt * usX2Y->stride;
			memcpy(runtime->dma_area + hwptr_done * usX2Y->stride, cp, blen);
			memcpy(runtime->dma_area, cp + blen, len * usX2Y->stride - blen);
		} else {
			memcpy(runtime->dma_area + hwptr_done * usX2Y->stride, cp,
			       len * usX2Y->stride);
		}
		lens += len;
		if ((hwptr_done += len) >= runtime->buffer_size)
=======
			cnt = runtime->buffer_size - hwptr_done;
			blen = cnt * usx2y->stride;
			memcpy(runtime->dma_area + hwptr_done * usx2y->stride, cp, blen);
			memcpy(runtime->dma_area, cp + blen, len * usx2y->stride - blen);
		} else {
			memcpy(runtime->dma_area + hwptr_done * usx2y->stride, cp,
			       len * usx2y->stride);
		}
		lens += len;
		hwptr_done += len;
		if (hwptr_done >= runtime->buffer_size)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			hwptr_done -= runtime->buffer_size;
	}

	subs->hwptr_done = hwptr_done;
	subs->transfer_done += lens;
	/* update the pointer, call callback if necessary */
	if (subs->transfer_done >= runtime->period_size) {
		subs->transfer_done -= runtime->period_size;
		snd_pcm_period_elapsed(subs->pcm_substream);
	}
	return 0;
}
<<<<<<< HEAD
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * prepare urb for playback data pipe
 *
 * we copy the data directly from the pcm buffer.
 * the current position to be copied is held in hwptr field.
 * since a urb can handle only a single linear buffer, if the total
 * transferred area overflows the buffer boundary, we cannot send
 * it directly from the buffer.  thus the data is once copied to
 * a temporary buffer and urb points to that.
 */
<<<<<<< HEAD
static int usX2Y_urb_play_prepare(struct snd_usX2Y_substream *subs,
				  struct urb *cap_urb,
				  struct urb *urb)
{
	int count, counts, pack;
	struct usX2Ydev *usX2Y = subs->usX2Y;
	struct snd_pcm_runtime *runtime = subs->pcm_substream->runtime;
=======
static int usx2y_urb_play_prepare(struct snd_usx2y_substream *subs,
				  struct urb *cap_urb,
				  struct urb *urb)
{
	struct usx2ydev *usx2y = subs->usx2y;
	struct snd_pcm_runtime *runtime = subs->pcm_substream->runtime;
	int count, counts, pack, len;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	count = 0;
	for (pack = 0; pack <  nr_of_packs(); pack++) {
		/* calculate the size of a packet */
<<<<<<< HEAD
		counts = cap_urb->iso_frame_desc[pack].actual_length / usX2Y->stride;
=======
		counts = cap_urb->iso_frame_desc[pack].actual_length / usx2y->stride;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		count += counts;
		if (counts < 43 || counts > 50) {
			snd_printk(KERN_ERR "should not be here with counts=%i\n", counts);
			return -EPIPE;
		}
		/* set up descriptor */
		urb->iso_frame_desc[pack].offset = pack ?
			urb->iso_frame_desc[pack - 1].offset +
			urb->iso_frame_desc[pack - 1].length :
			0;
		urb->iso_frame_desc[pack].length = cap_urb->iso_frame_desc[pack].actual_length;
	}
<<<<<<< HEAD
	if (atomic_read(&subs->state) >= state_PRERUNNING)
=======
	if (atomic_read(&subs->state) >= STATE_PRERUNNING) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (subs->hwptr + count > runtime->buffer_size) {
			/* err, the transferred area goes over buffer boundary.
			 * copy the data to the temp buffer.
			 */
<<<<<<< HEAD
			int len;
			len = runtime->buffer_size - subs->hwptr;
			urb->transfer_buffer = subs->tmpbuf;
			memcpy(subs->tmpbuf, runtime->dma_area +
			       subs->hwptr * usX2Y->stride, len * usX2Y->stride);
			memcpy(subs->tmpbuf + len * usX2Y->stride,
			       runtime->dma_area, (count - len) * usX2Y->stride);
=======
			len = runtime->buffer_size - subs->hwptr;
			urb->transfer_buffer = subs->tmpbuf;
			memcpy(subs->tmpbuf, runtime->dma_area +
			       subs->hwptr * usx2y->stride, len * usx2y->stride);
			memcpy(subs->tmpbuf + len * usx2y->stride,
			       runtime->dma_area, (count - len) * usx2y->stride);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			subs->hwptr += count;
			subs->hwptr -= runtime->buffer_size;
		} else {
			/* set the buffer pointer */
<<<<<<< HEAD
			urb->transfer_buffer = runtime->dma_area + subs->hwptr * usX2Y->stride;
			if ((subs->hwptr += count) >= runtime->buffer_size)
			subs->hwptr -= runtime->buffer_size;			
		}
	else
		urb->transfer_buffer = subs->tmpbuf;
	urb->transfer_buffer_length = count * usX2Y->stride;
=======
			urb->transfer_buffer = runtime->dma_area + subs->hwptr * usx2y->stride;
			subs->hwptr += count;
			if (subs->hwptr >= runtime->buffer_size)
				subs->hwptr -= runtime->buffer_size;
		}
	} else {
		urb->transfer_buffer = subs->tmpbuf;
	}
	urb->transfer_buffer_length = count * usx2y->stride;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

/*
 * process after playback data complete
 *
 * update the current position and call callback if a period is processed.
 */
<<<<<<< HEAD
static void usX2Y_urb_play_retire(struct snd_usX2Y_substream *subs, struct urb *urb)
{
	struct snd_pcm_runtime *runtime = subs->pcm_substream->runtime;
	int		len = urb->actual_length / subs->usX2Y->stride;
=======
static void usx2y_urb_play_retire(struct snd_usx2y_substream *subs, struct urb *urb)
{
	struct snd_pcm_runtime *runtime = subs->pcm_substream->runtime;
	int		len = urb->actual_length / subs->usx2y->stride;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	subs->transfer_done += len;
	subs->hwptr_done +=  len;
	if (subs->hwptr_done >= runtime->buffer_size)
		subs->hwptr_done -= runtime->buffer_size;
	if (subs->transfer_done >= runtime->period_size) {
		subs->transfer_done -= runtime->period_size;
		snd_pcm_period_elapsed(subs->pcm_substream);
	}
}

<<<<<<< HEAD
static int usX2Y_urb_submit(struct snd_usX2Y_substream *subs, struct urb *urb, int frame)
{
	int err;
	if (!urb)
		return -ENODEV;
	urb->start_frame = (frame + NRURBS * nr_of_packs());  // let hcd do rollover sanity checks
	urb->hcpriv = NULL;
	urb->dev = subs->usX2Y->dev; /* we need to set this at each time */
	if ((err = usb_submit_urb(urb, GFP_ATOMIC)) < 0) {
=======
static int usx2y_urb_submit(struct snd_usx2y_substream *subs, struct urb *urb, int frame)
{
	int err;

	if (!urb)
		return -ENODEV;
	urb->start_frame = frame + NRURBS * nr_of_packs();  // let hcd do rollover sanity checks
	urb->hcpriv = NULL;
	urb->dev = subs->usx2y->dev; /* we need to set this at each time */
	err = usb_submit_urb(urb, GFP_ATOMIC);
	if (err < 0) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		snd_printk(KERN_ERR "usb_submit_urb() returned %i\n", err);
		return err;
	}
	return 0;
}

<<<<<<< HEAD
static inline int usX2Y_usbframe_complete(struct snd_usX2Y_substream *capsubs,
					  struct snd_usX2Y_substream *playbacksubs,
					  int frame)
=======
static int usx2y_usbframe_complete(struct snd_usx2y_substream *capsubs,
				   struct snd_usx2y_substream *playbacksubs,
				   int frame)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int err, state;
	struct urb *urb = playbacksubs->completed_urb;

	state = atomic_read(&playbacksubs->state);
<<<<<<< HEAD
	if (NULL != urb) {
		if (state == state_RUNNING)
			usX2Y_urb_play_retire(playbacksubs, urb);
		else if (state >= state_PRERUNNING)
			atomic_inc(&playbacksubs->state);
	} else {
		switch (state) {
		case state_STARTING1:
			urb = playbacksubs->urb[0];
			atomic_inc(&playbacksubs->state);
			break;
		case state_STARTING2:
=======
	if (urb) {
		if (state == STATE_RUNNING)
			usx2y_urb_play_retire(playbacksubs, urb);
		else if (state >= STATE_PRERUNNING)
			atomic_inc(&playbacksubs->state);
	} else {
		switch (state) {
		case STATE_STARTING1:
			urb = playbacksubs->urb[0];
			atomic_inc(&playbacksubs->state);
			break;
		case STATE_STARTING2:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			urb = playbacksubs->urb[1];
			atomic_inc(&playbacksubs->state);
			break;
		}
	}
	if (urb) {
<<<<<<< HEAD
		if ((err = usX2Y_urb_play_prepare(playbacksubs, capsubs->completed_urb, urb)) ||
		    (err = usX2Y_urb_submit(playbacksubs, urb, frame))) {
			return err;
		}
=======
		err = usx2y_urb_play_prepare(playbacksubs, capsubs->completed_urb, urb);
		if (err)
			return err;
		err = usx2y_urb_submit(playbacksubs, urb, frame);
		if (err)
			return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	playbacksubs->completed_urb = NULL;

	state = atomic_read(&capsubs->state);
<<<<<<< HEAD
	if (state >= state_PREPARED) {
		if (state == state_RUNNING) {
			if ((err = usX2Y_urb_capt_retire(capsubs)))
				return err;
		} else if (state >= state_PRERUNNING)
			atomic_inc(&capsubs->state);
		if ((err = usX2Y_urb_submit(capsubs, capsubs->completed_urb, frame)))
=======
	if (state >= STATE_PREPARED) {
		if (state == STATE_RUNNING) {
			err = usx2y_urb_capt_retire(capsubs);
			if (err)
				return err;
		} else if (state >= STATE_PRERUNNING) {
			atomic_inc(&capsubs->state);
		}
		err = usx2y_urb_submit(capsubs, capsubs->completed_urb, frame);
		if (err)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return err;
	}
	capsubs->completed_urb = NULL;
	return 0;
}

<<<<<<< HEAD

static void usX2Y_clients_stop(struct usX2Ydev *usX2Y)
{
	int s, u;

	for (s = 0; s < 4; s++) {
		struct snd_usX2Y_substream *subs = usX2Y->subs[s];
		if (subs) {
			snd_printdd("%i %p state=%i\n", s, subs, atomic_read(&subs->state));
			atomic_set(&subs->state, state_STOPPED);
		}
	}
	for (s = 0; s < 4; s++) {
		struct snd_usX2Y_substream *subs = usX2Y->subs[s];
		if (subs) {
			if (atomic_read(&subs->state) >= state_PRERUNNING) {
				unsigned long flags;

				snd_pcm_stream_lock_irqsave(subs->pcm_substream, flags);
				snd_pcm_stop(subs->pcm_substream, SNDRV_PCM_STATE_XRUN);
				snd_pcm_stream_unlock_irqrestore(subs->pcm_substream, flags);
			}
			for (u = 0; u < NRURBS; u++) {
				struct urb *urb = subs->urb[u];
				if (NULL != urb)
=======
static void usx2y_clients_stop(struct usx2ydev *usx2y)
{
	struct snd_usx2y_substream *subs;
	struct urb *urb;
	int s, u;

	for (s = 0; s < 4; s++) {
		subs = usx2y->subs[s];
		if (subs) {
			snd_printdd("%i %p state=%i\n", s, subs, atomic_read(&subs->state));
			atomic_set(&subs->state, STATE_STOPPED);
		}
	}
	for (s = 0; s < 4; s++) {
		subs = usx2y->subs[s];
		if (subs) {
			if (atomic_read(&subs->state) >= STATE_PRERUNNING)
				snd_pcm_stop_xrun(subs->pcm_substream);
			for (u = 0; u < NRURBS; u++) {
				urb = subs->urb[u];
				if (urb)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					snd_printdd("%i status=%i start_frame=%i\n",
						    u, urb->status, urb->start_frame);
			}
		}
	}
<<<<<<< HEAD
	usX2Y->prepare_subs = NULL;
	wake_up(&usX2Y->prepare_wait_queue);
}

static void usX2Y_error_urb_status(struct usX2Ydev *usX2Y,
				   struct snd_usX2Y_substream *subs, struct urb *urb)
{
	snd_printk(KERN_ERR "ep=%i stalled with status=%i\n", subs->endpoint, urb->status);
	urb->status = 0;
	usX2Y_clients_stop(usX2Y);
}

static void i_usX2Y_urb_complete(struct urb *urb)
{
	struct snd_usX2Y_substream *subs = urb->context;
	struct usX2Ydev *usX2Y = subs->usX2Y;

	if (unlikely(atomic_read(&subs->state) < state_PREPARED)) {
		snd_printdd("hcd_frame=%i ep=%i%s status=%i start_frame=%i\n",
			    usb_get_current_frame_number(usX2Y->dev),
=======
	usx2y->prepare_subs = NULL;
	wake_up(&usx2y->prepare_wait_queue);
}

static void usx2y_error_urb_status(struct usx2ydev *usx2y,
				   struct snd_usx2y_substream *subs, struct urb *urb)
{
	snd_printk(KERN_ERR "ep=%i stalled with status=%i\n", subs->endpoint, urb->status);
	urb->status = 0;
	usx2y_clients_stop(usx2y);
}

static void i_usx2y_urb_complete(struct urb *urb)
{
	struct snd_usx2y_substream *subs = urb->context;
	struct usx2ydev *usx2y = subs->usx2y;
	struct snd_usx2y_substream *capsubs, *playbacksubs;

	if (unlikely(atomic_read(&subs->state) < STATE_PREPARED)) {
		snd_printdd("hcd_frame=%i ep=%i%s status=%i start_frame=%i\n",
			    usb_get_current_frame_number(usx2y->dev),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			    subs->endpoint, usb_pipein(urb->pipe) ? "in" : "out",
			    urb->status, urb->start_frame);
		return;
	}
	if (unlikely(urb->status)) {
<<<<<<< HEAD
		usX2Y_error_urb_status(usX2Y, subs, urb);
=======
		usx2y_error_urb_status(usx2y, subs, urb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;
	}

	subs->completed_urb = urb;

<<<<<<< HEAD
	{
		struct snd_usX2Y_substream *capsubs = usX2Y->subs[SNDRV_PCM_STREAM_CAPTURE],
			*playbacksubs = usX2Y->subs[SNDRV_PCM_STREAM_PLAYBACK];
		if (capsubs->completed_urb &&
		    atomic_read(&capsubs->state) >= state_PREPARED &&
		    (playbacksubs->completed_urb ||
		     atomic_read(&playbacksubs->state) < state_PREPARED)) {
			if (!usX2Y_usbframe_complete(capsubs, playbacksubs, urb->start_frame))
				usX2Y->wait_iso_frame += nr_of_packs();
			else {
				snd_printdd("\n");
				usX2Y_clients_stop(usX2Y);
			}
=======
	capsubs = usx2y->subs[SNDRV_PCM_STREAM_CAPTURE];
	playbacksubs = usx2y->subs[SNDRV_PCM_STREAM_PLAYBACK];

	if (capsubs->completed_urb &&
	    atomic_read(&capsubs->state) >= STATE_PREPARED &&
	    (playbacksubs->completed_urb ||
	     atomic_read(&playbacksubs->state) < STATE_PREPARED)) {
		if (!usx2y_usbframe_complete(capsubs, playbacksubs, urb->start_frame)) {
			usx2y->wait_iso_frame += nr_of_packs();
		} else {
			snd_printdd("\n");
			usx2y_clients_stop(usx2y);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}
}

<<<<<<< HEAD
static void usX2Y_urbs_set_complete(struct usX2Ydev * usX2Y,
				    void (*complete)(struct urb *))
{
	int s, u;
	for (s = 0; s < 4; s++) {
		struct snd_usX2Y_substream *subs = usX2Y->subs[s];
		if (NULL != subs)
			for (u = 0; u < NRURBS; u++) {
				struct urb * urb = subs->urb[u];
				if (NULL != urb)
					urb->complete = complete;
			}
	}
}

static void usX2Y_subs_startup_finish(struct usX2Ydev * usX2Y)
{
	usX2Y_urbs_set_complete(usX2Y, i_usX2Y_urb_complete);
	usX2Y->prepare_subs = NULL;
}

static void i_usX2Y_subs_startup(struct urb *urb)
{
	struct snd_usX2Y_substream *subs = urb->context;
	struct usX2Ydev *usX2Y = subs->usX2Y;
	struct snd_usX2Y_substream *prepare_subs = usX2Y->prepare_subs;
	if (NULL != prepare_subs)
		if (urb->start_frame == prepare_subs->urb[0]->start_frame) {
			usX2Y_subs_startup_finish(usX2Y);
			atomic_inc(&prepare_subs->state);
			wake_up(&usX2Y->prepare_wait_queue);
		}

	i_usX2Y_urb_complete(urb);
}

static void usX2Y_subs_prepare(struct snd_usX2Y_substream *subs)
{
	snd_printdd("usX2Y_substream_prepare(%p) ep=%i urb0=%p urb1=%p\n",
=======
static void usx2y_urbs_set_complete(struct usx2ydev *usx2y,
				    void (*complete)(struct urb *))
{
	struct snd_usx2y_substream *subs;
	struct urb *urb;
	int s, u;

	for (s = 0; s < 4; s++) {
		subs = usx2y->subs[s];
		if (subs) {
			for (u = 0; u < NRURBS; u++) {
				urb = subs->urb[u];
				if (urb)
					urb->complete = complete;
			}
		}
	}
}

static void usx2y_subs_startup_finish(struct usx2ydev *usx2y)
{
	usx2y_urbs_set_complete(usx2y, i_usx2y_urb_complete);
	usx2y->prepare_subs = NULL;
}

static void i_usx2y_subs_startup(struct urb *urb)
{
	struct snd_usx2y_substream *subs = urb->context;
	struct usx2ydev *usx2y = subs->usx2y;
	struct snd_usx2y_substream *prepare_subs = usx2y->prepare_subs;

	if (prepare_subs) {
		if (urb->start_frame == prepare_subs->urb[0]->start_frame) {
			usx2y_subs_startup_finish(usx2y);
			atomic_inc(&prepare_subs->state);
			wake_up(&usx2y->prepare_wait_queue);
		}
	}

	i_usx2y_urb_complete(urb);
}

static void usx2y_subs_prepare(struct snd_usx2y_substream *subs)
{
	snd_printdd("usx2y_substream_prepare(%p) ep=%i urb0=%p urb1=%p\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		    subs, subs->endpoint, subs->urb[0], subs->urb[1]);
	/* reset the pointer */
	subs->hwptr = 0;
	subs->hwptr_done = 0;
	subs->transfer_done = 0;
}

<<<<<<< HEAD

static void usX2Y_urb_release(struct urb **urb, int free_tb)
=======
static void usx2y_urb_release(struct urb **urb, int free_tb)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	if (*urb) {
		usb_kill_urb(*urb);
		if (free_tb)
			kfree((*urb)->transfer_buffer);
		usb_free_urb(*urb);
		*urb = NULL;
	}
}
<<<<<<< HEAD
/*
 * release a substreams urbs
 */
static void usX2Y_urbs_release(struct snd_usX2Y_substream *subs)
{
	int i;
	snd_printdd("usX2Y_urbs_release() %i\n", subs->endpoint);
	for (i = 0; i < NRURBS; i++)
		usX2Y_urb_release(subs->urb + i,
				  subs != subs->usX2Y->subs[SNDRV_PCM_STREAM_PLAYBACK]);
=======

/*
 * release a substreams urbs
 */
static void usx2y_urbs_release(struct snd_usx2y_substream *subs)
{
	int i;

	snd_printdd("%s %i\n", __func__, subs->endpoint);
	for (i = 0; i < NRURBS; i++)
		usx2y_urb_release(subs->urb + i,
				  subs != subs->usx2y->subs[SNDRV_PCM_STREAM_PLAYBACK]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	kfree(subs->tmpbuf);
	subs->tmpbuf = NULL;
}
<<<<<<< HEAD
/*
 * initialize a substream's urbs
 */
static int usX2Y_urbs_allocate(struct snd_usX2Y_substream *subs)
{
	int i;
	unsigned int pipe;
	int is_playback = subs == subs->usX2Y->subs[SNDRV_PCM_STREAM_PLAYBACK];
	struct usb_device *dev = subs->usX2Y->dev;

	pipe = is_playback ? usb_sndisocpipe(dev, subs->endpoint) :
			usb_rcvisocpipe(dev, subs->endpoint);
	subs->maxpacksize = usb_maxpacket(dev, pipe, is_playback);
	if (!subs->maxpacksize)
		return -EINVAL;

	if (is_playback && NULL == subs->tmpbuf) {	/* allocate a temporary buffer for playback */
		subs->tmpbuf = kcalloc(nr_of_packs(), subs->maxpacksize, GFP_KERNEL);
		if (NULL == subs->tmpbuf) {
			snd_printk(KERN_ERR "cannot malloc tmpbuf\n");
			return -ENOMEM;
		}
	}
	/* allocate and initialize data urbs */
	for (i = 0; i < NRURBS; i++) {
		struct urb **purb = subs->urb + i;
=======

/*
 * initialize a substream's urbs
 */
static int usx2y_urbs_allocate(struct snd_usx2y_substream *subs)
{
	int i;
	unsigned int pipe;
	int is_playback = subs == subs->usx2y->subs[SNDRV_PCM_STREAM_PLAYBACK];
	struct usb_device *dev = subs->usx2y->dev;
	struct urb **purb;

	pipe = is_playback ? usb_sndisocpipe(dev, subs->endpoint) :
			usb_rcvisocpipe(dev, subs->endpoint);
	subs->maxpacksize = usb_maxpacket(dev, pipe);
	if (!subs->maxpacksize)
		return -EINVAL;

	if (is_playback && !subs->tmpbuf) {	/* allocate a temporary buffer for playback */
		subs->tmpbuf = kcalloc(nr_of_packs(), subs->maxpacksize, GFP_KERNEL);
		if (!subs->tmpbuf)
			return -ENOMEM;
	}
	/* allocate and initialize data urbs */
	for (i = 0; i < NRURBS; i++) {
		purb = subs->urb + i;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (*purb) {
			usb_kill_urb(*purb);
			continue;
		}
		*purb = usb_alloc_urb(nr_of_packs(), GFP_KERNEL);
<<<<<<< HEAD
		if (NULL == *purb) {
			usX2Y_urbs_release(subs);
=======
		if (!*purb) {
			usx2y_urbs_release(subs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -ENOMEM;
		}
		if (!is_playback && !(*purb)->transfer_buffer) {
			/* allocate a capture buffer per urb */
<<<<<<< HEAD
			(*purb)->transfer_buffer = kmalloc(subs->maxpacksize * nr_of_packs(), GFP_KERNEL);
			if (NULL == (*purb)->transfer_buffer) {
				usX2Y_urbs_release(subs);
=======
			(*purb)->transfer_buffer =
				kmalloc_array(subs->maxpacksize,
					      nr_of_packs(), GFP_KERNEL);
			if (!(*purb)->transfer_buffer) {
				usx2y_urbs_release(subs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				return -ENOMEM;
			}
		}
		(*purb)->dev = dev;
		(*purb)->pipe = pipe;
		(*purb)->number_of_packets = nr_of_packs();
		(*purb)->context = subs;
		(*purb)->interval = 1;
<<<<<<< HEAD
		(*purb)->complete = i_usX2Y_subs_startup;
=======
		(*purb)->complete = i_usx2y_subs_startup;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return 0;
}

<<<<<<< HEAD
static void usX2Y_subs_startup(struct snd_usX2Y_substream *subs)
{
	struct usX2Ydev *usX2Y = subs->usX2Y;
	usX2Y->prepare_subs = subs;
	subs->urb[0]->start_frame = -1;
	wmb();
	usX2Y_urbs_set_complete(usX2Y, i_usX2Y_subs_startup);
}

static int usX2Y_urbs_start(struct snd_usX2Y_substream *subs)
{
	int i, err;
	struct usX2Ydev *usX2Y = subs->usX2Y;

	if ((err = usX2Y_urbs_allocate(subs)) < 0)
		return err;
	subs->completed_urb = NULL;
	for (i = 0; i < 4; i++) {
		struct snd_usX2Y_substream *subs = usX2Y->subs[i];
		if (subs != NULL && atomic_read(&subs->state) >= state_PREPARED)
=======
static void usx2y_subs_startup(struct snd_usx2y_substream *subs)
{
	struct usx2ydev *usx2y = subs->usx2y;

	usx2y->prepare_subs = subs;
	subs->urb[0]->start_frame = -1;
	wmb();
	usx2y_urbs_set_complete(usx2y, i_usx2y_subs_startup);
}

static int usx2y_urbs_start(struct snd_usx2y_substream *subs)
{
	int i, err;
	struct usx2ydev *usx2y = subs->usx2y;
	struct urb *urb;
	unsigned long pack;

	err = usx2y_urbs_allocate(subs);
	if (err < 0)
		return err;
	subs->completed_urb = NULL;
	for (i = 0; i < 4; i++) {
		struct snd_usx2y_substream *subs = usx2y->subs[i];

		if (subs && atomic_read(&subs->state) >= STATE_PREPARED)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto start;
	}

 start:
<<<<<<< HEAD
	usX2Y_subs_startup(subs);
	for (i = 0; i < NRURBS; i++) {
		struct urb *urb = subs->urb[i];
		if (usb_pipein(urb->pipe)) {
			unsigned long pack;
			if (0 == i)
				atomic_set(&subs->state, state_STARTING3);
			urb->dev = usX2Y->dev;
			urb->transfer_flags = URB_ISO_ASAP;
=======
	usx2y_subs_startup(subs);
	for (i = 0; i < NRURBS; i++) {
		urb = subs->urb[i];
		if (usb_pipein(urb->pipe)) {
			if (!i)
				atomic_set(&subs->state, STATE_STARTING3);
			urb->dev = usx2y->dev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			for (pack = 0; pack < nr_of_packs(); pack++) {
				urb->iso_frame_desc[pack].offset = subs->maxpacksize * pack;
				urb->iso_frame_desc[pack].length = subs->maxpacksize;
			}
<<<<<<< HEAD
			urb->transfer_buffer_length = subs->maxpacksize * nr_of_packs(); 
			if ((err = usb_submit_urb(urb, GFP_ATOMIC)) < 0) {
				snd_printk (KERN_ERR "cannot submit datapipe for urb %d, err = %d\n", i, err);
				err = -EPIPE;
				goto cleanup;
			} else
				if (i == 0)
					usX2Y->wait_iso_frame = urb->start_frame;
			urb->transfer_flags = 0;
		} else {
			atomic_set(&subs->state, state_STARTING1);
=======
			urb->transfer_buffer_length = subs->maxpacksize * nr_of_packs();
			err = usb_submit_urb(urb, GFP_ATOMIC);
			if (err < 0) {
				snd_printk(KERN_ERR "cannot submit datapipe for urb %d, err = %d\n", i, err);
				err = -EPIPE;
				goto cleanup;
			} else {
				if (!i)
					usx2y->wait_iso_frame = urb->start_frame;
			}
			urb->transfer_flags = 0;
		} else {
			atomic_set(&subs->state, STATE_STARTING1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		}
	}
	err = 0;
<<<<<<< HEAD
	wait_event(usX2Y->prepare_wait_queue, NULL == usX2Y->prepare_subs);
	if (atomic_read(&subs->state) != state_PREPARED)
=======
	wait_event(usx2y->prepare_wait_queue, !usx2y->prepare_subs);
	if (atomic_read(&subs->state) != STATE_PREPARED)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = -EPIPE;

 cleanup:
	if (err) {
<<<<<<< HEAD
		usX2Y_subs_startup_finish(usX2Y);
		usX2Y_clients_stop(usX2Y);		// something is completely wroong > stop evrything
=======
		usx2y_subs_startup_finish(usx2y);
		usx2y_clients_stop(usx2y);	// something is completely wrong > stop everything
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return err;
}

/*
 * return the current pcm pointer.  just return the hwptr_done value.
 */
<<<<<<< HEAD
static snd_pcm_uframes_t snd_usX2Y_pcm_pointer(struct snd_pcm_substream *substream)
{
	struct snd_usX2Y_substream *subs = substream->runtime->private_data;
	return subs->hwptr_done;
}
/*
 * start/stop substream
 */
static int snd_usX2Y_pcm_trigger(struct snd_pcm_substream *substream, int cmd)
{
	struct snd_usX2Y_substream *subs = substream->runtime->private_data;

	switch (cmd) {
	case SNDRV_PCM_TRIGGER_START:
		snd_printdd("snd_usX2Y_pcm_trigger(START)\n");
		if (atomic_read(&subs->state) == state_PREPARED &&
		    atomic_read(&subs->usX2Y->subs[SNDRV_PCM_STREAM_CAPTURE]->state) >= state_PREPARED) {
			atomic_set(&subs->state, state_PRERUNNING);
=======
static snd_pcm_uframes_t snd_usx2y_pcm_pointer(struct snd_pcm_substream *substream)
{
	struct snd_usx2y_substream *subs = substream->runtime->private_data;

	return subs->hwptr_done;
}

/*
 * start/stop substream
 */
static int snd_usx2y_pcm_trigger(struct snd_pcm_substream *substream, int cmd)
{
	struct snd_usx2y_substream *subs = substream->runtime->private_data;

	switch (cmd) {
	case SNDRV_PCM_TRIGGER_START:
		snd_printdd("%s(START)\n", __func__);
		if (atomic_read(&subs->state) == STATE_PREPARED &&
		    atomic_read(&subs->usx2y->subs[SNDRV_PCM_STREAM_CAPTURE]->state) >= STATE_PREPARED) {
			atomic_set(&subs->state, STATE_PRERUNNING);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		} else {
			snd_printdd("\n");
			return -EPIPE;
		}
		break;
	case SNDRV_PCM_TRIGGER_STOP:
<<<<<<< HEAD
		snd_printdd("snd_usX2Y_pcm_trigger(STOP)\n");
		if (atomic_read(&subs->state) >= state_PRERUNNING)
			atomic_set(&subs->state, state_PREPARED);
=======
		snd_printdd("%s(STOP)\n", __func__);
		if (atomic_read(&subs->state) >= STATE_PRERUNNING)
			atomic_set(&subs->state, STATE_PREPARED);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	default:
		return -EINVAL;
	}
	return 0;
}

<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * allocate a buffer, setup samplerate
 *
 * so far we use a physically linear buffer although packetize transfer
 * doesn't need a continuous area.
 * if sg buffer is supported on the later version of alsa, we'll follow
 * that.
 */
<<<<<<< HEAD
static struct s_c2
{
	char c1, c2;
}
	SetRate44100[] =
{
=======
struct s_c2 {
	char c1, c2;
};

static const struct s_c2 setrate_44100[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{ 0x14, 0x08},	// this line sets 44100, well actually a little less
	{ 0x18, 0x40},	// only tascam / frontier design knows the further lines .......
	{ 0x18, 0x42},
	{ 0x18, 0x45},
	{ 0x18, 0x46},
	{ 0x18, 0x48},
	{ 0x18, 0x4A},
	{ 0x18, 0x4C},
	{ 0x18, 0x4E},
	{ 0x18, 0x50},
	{ 0x18, 0x52},
	{ 0x18, 0x54},
	{ 0x18, 0x56},
	{ 0x18, 0x58},
	{ 0x18, 0x5A},
	{ 0x18, 0x5C},
	{ 0x18, 0x5E},
	{ 0x18, 0x60},
	{ 0x18, 0x62},
	{ 0x18, 0x64},
	{ 0x18, 0x66},
	{ 0x18, 0x68},
	{ 0x18, 0x6A},
	{ 0x18, 0x6C},
	{ 0x18, 0x6E},
	{ 0x18, 0x70},
	{ 0x18, 0x72},
	{ 0x18, 0x74},
	{ 0x18, 0x76},
	{ 0x18, 0x78},
	{ 0x18, 0x7A},
	{ 0x18, 0x7C},
	{ 0x18, 0x7E}
};
<<<<<<< HEAD
static struct s_c2 SetRate48000[] =
{
=======

static const struct s_c2 setrate_48000[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{ 0x14, 0x09},	// this line sets 48000, well actually a little less
	{ 0x18, 0x40},	// only tascam / frontier design knows the further lines .......
	{ 0x18, 0x42},
	{ 0x18, 0x45},
	{ 0x18, 0x46},
	{ 0x18, 0x48},
	{ 0x18, 0x4A},
	{ 0x18, 0x4C},
	{ 0x18, 0x4E},
	{ 0x18, 0x50},
	{ 0x18, 0x52},
	{ 0x18, 0x54},
	{ 0x18, 0x56},
	{ 0x18, 0x58},
	{ 0x18, 0x5A},
	{ 0x18, 0x5C},
	{ 0x18, 0x5E},
	{ 0x18, 0x60},
	{ 0x18, 0x62},
	{ 0x18, 0x64},
	{ 0x18, 0x66},
	{ 0x18, 0x68},
	{ 0x18, 0x6A},
	{ 0x18, 0x6C},
	{ 0x18, 0x6E},
	{ 0x18, 0x70},
	{ 0x18, 0x73},
	{ 0x18, 0x74},
	{ 0x18, 0x76},
	{ 0x18, 0x78},
	{ 0x18, 0x7A},
	{ 0x18, 0x7C},
	{ 0x18, 0x7E}
};
<<<<<<< HEAD
#define NOOF_SETRATE_URBS ARRAY_SIZE(SetRate48000)

static void i_usX2Y_04Int(struct urb *urb)
{
	struct usX2Ydev *usX2Y = urb->context;
	
	if (urb->status)
		snd_printk(KERN_ERR "snd_usX2Y_04Int() urb->status=%i\n", urb->status);
	if (0 == --usX2Y->US04->len)
		wake_up(&usX2Y->In04WaitQueue);
}

static int usX2Y_rate_set(struct usX2Ydev *usX2Y, int rate)
{
	int			err = 0, i;
	struct snd_usX2Y_urbSeq	*us = NULL;
	int			*usbdata = NULL;
	struct s_c2		*ra = rate == 48000 ? SetRate48000 : SetRate44100;

	if (usX2Y->rate != rate) {
		us = kzalloc(sizeof(*us) + sizeof(struct urb*) * NOOF_SETRATE_URBS, GFP_KERNEL);
		if (NULL == us) {
			err = -ENOMEM;
			goto cleanup;
		}
		usbdata = kmalloc(sizeof(int) * NOOF_SETRATE_URBS, GFP_KERNEL);
		if (NULL == usbdata) {
=======

#define NOOF_SETRATE_URBS ARRAY_SIZE(setrate_48000)

static void i_usx2y_04int(struct urb *urb)
{
	struct usx2ydev *usx2y = urb->context;

	if (urb->status)
		snd_printk(KERN_ERR "snd_usx2y_04int() urb->status=%i\n", urb->status);
	if (!--usx2y->us04->len)
		wake_up(&usx2y->in04_wait_queue);
}

static int usx2y_rate_set(struct usx2ydev *usx2y, int rate)
{
	int err = 0, i;
	struct snd_usx2y_urb_seq *us = NULL;
	int *usbdata = NULL;
	const struct s_c2 *ra = rate == 48000 ? setrate_48000 : setrate_44100;
	struct urb *urb;

	if (usx2y->rate != rate) {
		us = kzalloc(struct_size(us, urb, NOOF_SETRATE_URBS),
			     GFP_KERNEL);
		if (!us) {
			err = -ENOMEM;
			goto cleanup;
		}
		us->len = NOOF_SETRATE_URBS;
		usbdata = kmalloc_array(NOOF_SETRATE_URBS, sizeof(int),
					GFP_KERNEL);
		if (!usbdata) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			err = -ENOMEM;
			goto cleanup;
		}
		for (i = 0; i < NOOF_SETRATE_URBS; ++i) {
<<<<<<< HEAD
			if (NULL == (us->urb[i] = usb_alloc_urb(0, GFP_KERNEL))) {
				err = -ENOMEM;
				goto cleanup;
			}
			((char*)(usbdata + i))[0] = ra[i].c1;
			((char*)(usbdata + i))[1] = ra[i].c2;
			usb_fill_bulk_urb(us->urb[i], usX2Y->dev, usb_sndbulkpipe(usX2Y->dev, 4),
					  usbdata + i, 2, i_usX2Y_04Int, usX2Y);
#ifdef OLD_USB
			us->urb[i]->transfer_flags = USB_QUEUE_BULK;
#endif
		}
		us->submitted =	0;
		us->len =	NOOF_SETRATE_URBS;
		usX2Y->US04 =	us;
		wait_event_timeout(usX2Y->In04WaitQueue, 0 == us->len, HZ);
		usX2Y->US04 =	NULL;
=======
			us->urb[i] = usb_alloc_urb(0, GFP_KERNEL);
			if (!us->urb[i]) {
				err = -ENOMEM;
				goto cleanup;
			}
			((char *)(usbdata + i))[0] = ra[i].c1;
			((char *)(usbdata + i))[1] = ra[i].c2;
			usb_fill_bulk_urb(us->urb[i], usx2y->dev, usb_sndbulkpipe(usx2y->dev, 4),
					  usbdata + i, 2, i_usx2y_04int, usx2y);
		}
		err = usb_urb_ep_type_check(us->urb[0]);
		if (err < 0)
			goto cleanup;
		us->submitted =	0;
		usx2y->us04 =	us;
		wait_event_timeout(usx2y->in04_wait_queue, !us->len, HZ);
		usx2y->us04 =	NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (us->len)
			err = -ENODEV;
	cleanup:
		if (us) {
			us->submitted =	2*NOOF_SETRATE_URBS;
			for (i = 0; i < NOOF_SETRATE_URBS; ++i) {
<<<<<<< HEAD
				struct urb *urb = us->urb[i];
=======
				urb = us->urb[i];
				if (!urb)
					continue;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				if (urb->status) {
					if (!err)
						err = -ENODEV;
					usb_kill_urb(urb);
				}
				usb_free_urb(urb);
			}
<<<<<<< HEAD
			usX2Y->US04 = NULL;
			kfree(usbdata);
			kfree(us);
			if (!err)
				usX2Y->rate = rate;
=======
			usx2y->us04 = NULL;
			kfree(usbdata);
			kfree(us);
			if (!err)
				usx2y->rate = rate;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}

	return err;
}

<<<<<<< HEAD

static int usX2Y_format_set(struct usX2Ydev *usX2Y, snd_pcm_format_t format)
{
	int alternate, err;
	struct list_head* p;
	if (format == SNDRV_PCM_FORMAT_S24_3LE) {
		alternate = 2;
		usX2Y->stride = 6;
	} else {
		alternate = 1;
		usX2Y->stride = 4;
	}
	list_for_each(p, &usX2Y->midi_list) {
		snd_usbmidi_input_stop(p);
	}
	usb_kill_urb(usX2Y->In04urb);
	if ((err = usb_set_interface(usX2Y->dev, 0, alternate))) {
		snd_printk(KERN_ERR "usb_set_interface error \n");
		return err;
	}
	usX2Y->In04urb->dev = usX2Y->dev;
	err = usb_submit_urb(usX2Y->In04urb, GFP_KERNEL);
	list_for_each(p, &usX2Y->midi_list) {
		snd_usbmidi_input_start(p);
	}
	usX2Y->format = format;
	usX2Y->rate = 0;
=======
static int usx2y_format_set(struct usx2ydev *usx2y, snd_pcm_format_t format)
{
	int alternate, err;
	struct list_head *p;

	if (format == SNDRV_PCM_FORMAT_S24_3LE) {
		alternate = 2;
		usx2y->stride = 6;
	} else {
		alternate = 1;
		usx2y->stride = 4;
	}
	list_for_each(p, &usx2y->midi_list) {
		snd_usbmidi_input_stop(p);
	}
	usb_kill_urb(usx2y->in04_urb);
	err = usb_set_interface(usx2y->dev, 0, alternate);
	if (err) {
		snd_printk(KERN_ERR "usb_set_interface error\n");
		return err;
	}
	usx2y->in04_urb->dev = usx2y->dev;
	err = usb_submit_urb(usx2y->in04_urb, GFP_KERNEL);
	list_for_each(p, &usx2y->midi_list) {
		snd_usbmidi_input_start(p);
	}
	usx2y->format = format;
	usx2y->rate = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}


<<<<<<< HEAD
static int snd_usX2Y_pcm_hw_params(struct snd_pcm_substream *substream,
=======
static int snd_usx2y_pcm_hw_params(struct snd_pcm_substream *substream,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				   struct snd_pcm_hw_params *hw_params)
{
	int			err = 0;
	unsigned int		rate = params_rate(hw_params);
	snd_pcm_format_t	format = params_format(hw_params);
	struct snd_card *card = substream->pstr->pcm->card;
<<<<<<< HEAD
	struct list_head *list;

	snd_printdd("snd_usX2Y_hw_params(%p, %p)\n", substream, hw_params);
	// all pcm substreams off one usX2Y have to operate at the same rate & format
	list_for_each(list, &card->devices) {
		struct snd_device *dev;
		struct snd_pcm *pcm;
		int s;
		dev = snd_device(list);
		if (dev->type != SNDRV_DEV_PCM)
			continue;
		pcm = dev->device_data;
		for (s = 0; s < 2; ++s) {
			struct snd_pcm_substream *test_substream;
			test_substream = pcm->streams[s].substream;
			if (test_substream && test_substream != substream  &&
			    test_substream->runtime &&
			    ((test_substream->runtime->format &&
			      test_substream->runtime->format != format) ||
			     (test_substream->runtime->rate &&
			      test_substream->runtime->rate != rate)))
				return -EINVAL;
		}
	}
	if (0 > (err = snd_pcm_lib_malloc_pages(substream, params_buffer_bytes(hw_params)))) {
		snd_printk(KERN_ERR "snd_pcm_lib_malloc_pages(%p, %i) returned %i\n",
			   substream, params_buffer_bytes(hw_params), err);
		return err;
	}
	return 0;
=======
	struct usx2ydev	*dev = usx2y(card);
	struct snd_usx2y_substream *subs;
	struct snd_pcm_substream *test_substream;
	int i;

	mutex_lock(&usx2y(card)->pcm_mutex);
	snd_printdd("snd_usx2y_hw_params(%p, %p)\n", substream, hw_params);
	/* all pcm substreams off one usx2y have to operate at the same
	 * rate & format
	 */
	for (i = 0; i < dev->pcm_devs * 2; i++) {
		subs = dev->subs[i];
		if (!subs)
			continue;
		test_substream = subs->pcm_substream;
		if (!test_substream || test_substream == substream ||
		    !test_substream->runtime)
			continue;
		if ((test_substream->runtime->format &&
		     test_substream->runtime->format != format) ||
		    (test_substream->runtime->rate &&
		     test_substream->runtime->rate != rate)) {
			err = -EINVAL;
			goto error;
		}
	}

 error:
	mutex_unlock(&usx2y(card)->pcm_mutex);
	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * free the buffer
 */
<<<<<<< HEAD
static int snd_usX2Y_pcm_hw_free(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct snd_usX2Y_substream *subs = runtime->private_data;
	mutex_lock(&subs->usX2Y->prepare_mutex);
	snd_printdd("snd_usX2Y_hw_free(%p)\n", substream);

	if (SNDRV_PCM_STREAM_PLAYBACK == substream->stream) {
		struct snd_usX2Y_substream *cap_subs = subs->usX2Y->subs[SNDRV_PCM_STREAM_CAPTURE];
		atomic_set(&subs->state, state_STOPPED);
		usX2Y_urbs_release(subs);
		if (!cap_subs->pcm_substream ||
		    !cap_subs->pcm_substream->runtime ||
		    !cap_subs->pcm_substream->runtime->status ||
		    cap_subs->pcm_substream->runtime->status->state < SNDRV_PCM_STATE_PREPARED) {
			atomic_set(&cap_subs->state, state_STOPPED);
			usX2Y_urbs_release(cap_subs);
		}
	} else {
		struct snd_usX2Y_substream *playback_subs = subs->usX2Y->subs[SNDRV_PCM_STREAM_PLAYBACK];
		if (atomic_read(&playback_subs->state) < state_PREPARED) {
			atomic_set(&subs->state, state_STOPPED);
			usX2Y_urbs_release(subs);
		}
	}
	mutex_unlock(&subs->usX2Y->prepare_mutex);
	return snd_pcm_lib_free_pages(substream);
}
=======
static int snd_usx2y_pcm_hw_free(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct snd_usx2y_substream *subs = runtime->private_data;
	struct snd_usx2y_substream *cap_subs, *playback_subs;

	mutex_lock(&subs->usx2y->pcm_mutex);
	snd_printdd("snd_usx2y_hw_free(%p)\n", substream);

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
		cap_subs = subs->usx2y->subs[SNDRV_PCM_STREAM_CAPTURE];
		atomic_set(&subs->state, STATE_STOPPED);
		usx2y_urbs_release(subs);
		if (!cap_subs->pcm_substream ||
		    !cap_subs->pcm_substream->runtime ||
		    cap_subs->pcm_substream->runtime->state < SNDRV_PCM_STATE_PREPARED) {
			atomic_set(&cap_subs->state, STATE_STOPPED);
			usx2y_urbs_release(cap_subs);
		}
	} else {
		playback_subs = subs->usx2y->subs[SNDRV_PCM_STREAM_PLAYBACK];
		if (atomic_read(&playback_subs->state) < STATE_PREPARED) {
			atomic_set(&subs->state, STATE_STOPPED);
			usx2y_urbs_release(subs);
		}
	}
	mutex_unlock(&subs->usx2y->pcm_mutex);
	return 0;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * prepare callback
 *
 * set format and initialize urbs
 */
<<<<<<< HEAD
static int snd_usX2Y_pcm_prepare(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct snd_usX2Y_substream *subs = runtime->private_data;
	struct usX2Ydev *usX2Y = subs->usX2Y;
	struct snd_usX2Y_substream *capsubs = subs->usX2Y->subs[SNDRV_PCM_STREAM_CAPTURE];
	int err = 0;
	snd_printdd("snd_usX2Y_pcm_prepare(%p)\n", substream);

	mutex_lock(&usX2Y->prepare_mutex);
	usX2Y_subs_prepare(subs);
// Start hardware streams
// SyncStream first....
	if (atomic_read(&capsubs->state) < state_PREPARED) {
		if (usX2Y->format != runtime->format)
			if ((err = usX2Y_format_set(usX2Y, runtime->format)) < 0)
				goto up_prepare_mutex;
		if (usX2Y->rate != runtime->rate)
			if ((err = usX2Y_rate_set(usX2Y, runtime->rate)) < 0)
				goto up_prepare_mutex;
		snd_printdd("starting capture pipe for %s\n", subs == capsubs ? "self" : "playpipe");
		if (0 > (err = usX2Y_urbs_start(capsubs)))
			goto up_prepare_mutex;
	}

	if (subs != capsubs && atomic_read(&subs->state) < state_PREPARED)
		err = usX2Y_urbs_start(subs);

 up_prepare_mutex:
	mutex_unlock(&usX2Y->prepare_mutex);
	return err;
}

static struct snd_pcm_hardware snd_usX2Y_2c =
{
=======
static int snd_usx2y_pcm_prepare(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct snd_usx2y_substream *subs = runtime->private_data;
	struct usx2ydev *usx2y = subs->usx2y;
	struct snd_usx2y_substream *capsubs = subs->usx2y->subs[SNDRV_PCM_STREAM_CAPTURE];
	int err = 0;

	snd_printdd("%s(%p)\n", __func__, substream);

	mutex_lock(&usx2y->pcm_mutex);
	usx2y_subs_prepare(subs);
	// Start hardware streams
	// SyncStream first....
	if (atomic_read(&capsubs->state) < STATE_PREPARED) {
		if (usx2y->format != runtime->format) {
			err = usx2y_format_set(usx2y, runtime->format);
			if (err < 0)
				goto up_prepare_mutex;
		}
		if (usx2y->rate != runtime->rate) {
			err = usx2y_rate_set(usx2y, runtime->rate);
			if (err < 0)
				goto up_prepare_mutex;
		}
		snd_printdd("starting capture pipe for %s\n", subs == capsubs ? "self" : "playpipe");
		err = usx2y_urbs_start(capsubs);
		if (err < 0)
			goto up_prepare_mutex;
	}

	if (subs != capsubs && atomic_read(&subs->state) < STATE_PREPARED)
		err = usx2y_urbs_start(subs);

 up_prepare_mutex:
	mutex_unlock(&usx2y->pcm_mutex);
	return err;
}

static const struct snd_pcm_hardware snd_usx2y_2c = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.info =			(SNDRV_PCM_INFO_MMAP | SNDRV_PCM_INFO_INTERLEAVED |
				 SNDRV_PCM_INFO_BLOCK_TRANSFER |
				 SNDRV_PCM_INFO_MMAP_VALID |
				 SNDRV_PCM_INFO_BATCH),
	.formats =                 SNDRV_PCM_FMTBIT_S16_LE | SNDRV_PCM_FMTBIT_S24_3LE,
	.rates =                   SNDRV_PCM_RATE_44100 | SNDRV_PCM_RATE_48000,
	.rate_min =                44100,
	.rate_max =                48000,
	.channels_min =            2,
	.channels_max =            2,
	.buffer_bytes_max =	(2*128*1024),
	.period_bytes_min =	64,
	.period_bytes_max =	(128*1024),
	.periods_min =		2,
	.periods_max =		1024,
	.fifo_size =              0
};

<<<<<<< HEAD


static int snd_usX2Y_pcm_open(struct snd_pcm_substream *substream)
{
	struct snd_usX2Y_substream	*subs = ((struct snd_usX2Y_substream **)
					 snd_pcm_substream_chip(substream))[substream->stream];
	struct snd_pcm_runtime	*runtime = substream->runtime;

	if (subs->usX2Y->chip_status & USX2Y_STAT_CHIP_MMAP_PCM_URBS)
		return -EBUSY;

	runtime->hw = snd_usX2Y_2c;
=======
static int snd_usx2y_pcm_open(struct snd_pcm_substream *substream)
{
	struct snd_usx2y_substream	*subs =
		((struct snd_usx2y_substream **)
		 snd_pcm_substream_chip(substream))[substream->stream];
	struct snd_pcm_runtime	*runtime = substream->runtime;

	if (subs->usx2y->chip_status & USX2Y_STAT_CHIP_MMAP_PCM_URBS)
		return -EBUSY;

	runtime->hw = snd_usx2y_2c;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	runtime->private_data = subs;
	subs->pcm_substream = substream;
	snd_pcm_hw_constraint_minmax(runtime, SNDRV_PCM_HW_PARAM_PERIOD_TIME, 1000, 200000);
	return 0;
}

<<<<<<< HEAD


static int snd_usX2Y_pcm_close(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct snd_usX2Y_substream *subs = runtime->private_data;
=======
static int snd_usx2y_pcm_close(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct snd_usx2y_substream *subs = runtime->private_data;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	subs->pcm_substream = NULL;

	return 0;
}

<<<<<<< HEAD

static struct snd_pcm_ops snd_usX2Y_pcm_ops = 
{
	.open =		snd_usX2Y_pcm_open,
	.close =	snd_usX2Y_pcm_close,
	.ioctl =	snd_pcm_lib_ioctl,
	.hw_params =	snd_usX2Y_pcm_hw_params,
	.hw_free =	snd_usX2Y_pcm_hw_free,
	.prepare =	snd_usX2Y_pcm_prepare,
	.trigger =	snd_usX2Y_pcm_trigger,
	.pointer =	snd_usX2Y_pcm_pointer,
};


/*
 * free a usb stream instance
 */
static void usX2Y_audio_stream_free(struct snd_usX2Y_substream **usX2Y_substream)
{
	kfree(usX2Y_substream[SNDRV_PCM_STREAM_PLAYBACK]);
	usX2Y_substream[SNDRV_PCM_STREAM_PLAYBACK] = NULL;

	kfree(usX2Y_substream[SNDRV_PCM_STREAM_CAPTURE]);
	usX2Y_substream[SNDRV_PCM_STREAM_CAPTURE] = NULL;
}

static void snd_usX2Y_pcm_private_free(struct snd_pcm *pcm)
{
	struct snd_usX2Y_substream **usX2Y_stream = pcm->private_data;
	if (usX2Y_stream)
		usX2Y_audio_stream_free(usX2Y_stream);
}

static int usX2Y_audio_stream_new(struct snd_card *card, int playback_endpoint, int capture_endpoint)
{
	struct snd_pcm *pcm;
	int err, i;
	struct snd_usX2Y_substream **usX2Y_substream =
		usX2Y(card)->subs + 2 * usX2Y(card)->pcm_devs;

	for (i = playback_endpoint ? SNDRV_PCM_STREAM_PLAYBACK : SNDRV_PCM_STREAM_CAPTURE;
	     i <= SNDRV_PCM_STREAM_CAPTURE; ++i) {
		usX2Y_substream[i] = kzalloc(sizeof(struct snd_usX2Y_substream), GFP_KERNEL);
		if (NULL == usX2Y_substream[i]) {
			snd_printk(KERN_ERR "cannot malloc\n");
			return -ENOMEM;
		}
		usX2Y_substream[i]->usX2Y = usX2Y(card);
	}

	if (playback_endpoint)
		usX2Y_substream[SNDRV_PCM_STREAM_PLAYBACK]->endpoint = playback_endpoint;
	usX2Y_substream[SNDRV_PCM_STREAM_CAPTURE]->endpoint = capture_endpoint;

	err = snd_pcm_new(card, NAME_ALLCAPS" Audio", usX2Y(card)->pcm_devs,
			  playback_endpoint ? 1 : 0, 1,
			  &pcm);
	if (err < 0) {
		usX2Y_audio_stream_free(usX2Y_substream);
=======
static const struct snd_pcm_ops snd_usx2y_pcm_ops = {
	.open =		snd_usx2y_pcm_open,
	.close =	snd_usx2y_pcm_close,
	.hw_params =	snd_usx2y_pcm_hw_params,
	.hw_free =	snd_usx2y_pcm_hw_free,
	.prepare =	snd_usx2y_pcm_prepare,
	.trigger =	snd_usx2y_pcm_trigger,
	.pointer =	snd_usx2y_pcm_pointer,
};

/*
 * free a usb stream instance
 */
static void usx2y_audio_stream_free(struct snd_usx2y_substream **usx2y_substream)
{
	int stream;

	for_each_pcm_streams(stream) {
		kfree(usx2y_substream[stream]);
		usx2y_substream[stream] = NULL;
	}
}

static void snd_usx2y_pcm_private_free(struct snd_pcm *pcm)
{
	struct snd_usx2y_substream **usx2y_stream = pcm->private_data;

	if (usx2y_stream)
		usx2y_audio_stream_free(usx2y_stream);
}

static int usx2y_audio_stream_new(struct snd_card *card, int playback_endpoint, int capture_endpoint)
{
	struct snd_pcm *pcm;
	int err, i;
	struct snd_usx2y_substream **usx2y_substream =
		usx2y(card)->subs + 2 * usx2y(card)->pcm_devs;

	for (i = playback_endpoint ? SNDRV_PCM_STREAM_PLAYBACK : SNDRV_PCM_STREAM_CAPTURE;
	     i <= SNDRV_PCM_STREAM_CAPTURE; ++i) {
		usx2y_substream[i] = kzalloc(sizeof(struct snd_usx2y_substream), GFP_KERNEL);
		if (!usx2y_substream[i])
			return -ENOMEM;

		usx2y_substream[i]->usx2y = usx2y(card);
	}

	if (playback_endpoint)
		usx2y_substream[SNDRV_PCM_STREAM_PLAYBACK]->endpoint = playback_endpoint;
	usx2y_substream[SNDRV_PCM_STREAM_CAPTURE]->endpoint = capture_endpoint;

	err = snd_pcm_new(card, NAME_ALLCAPS" Audio", usx2y(card)->pcm_devs,
			  playback_endpoint ? 1 : 0, 1,
			  &pcm);
	if (err < 0) {
		usx2y_audio_stream_free(usx2y_substream);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return err;
	}

	if (playback_endpoint)
<<<<<<< HEAD
		snd_pcm_set_ops(pcm, SNDRV_PCM_STREAM_PLAYBACK, &snd_usX2Y_pcm_ops);
	snd_pcm_set_ops(pcm, SNDRV_PCM_STREAM_CAPTURE, &snd_usX2Y_pcm_ops);

	pcm->private_data = usX2Y_substream;
	pcm->private_free = snd_usX2Y_pcm_private_free;
	pcm->info_flags = 0;

	sprintf(pcm->name, NAME_ALLCAPS" Audio #%d", usX2Y(card)->pcm_devs);

	if ((playback_endpoint &&
	     0 > (err = snd_pcm_lib_preallocate_pages(pcm->streams[SNDRV_PCM_STREAM_PLAYBACK].substream,
						     SNDRV_DMA_TYPE_CONTINUOUS,
						     snd_dma_continuous_data(GFP_KERNEL),
						     64*1024, 128*1024))) ||
	    0 > (err = snd_pcm_lib_preallocate_pages(pcm->streams[SNDRV_PCM_STREAM_CAPTURE].substream,
	    					     SNDRV_DMA_TYPE_CONTINUOUS,
	    					     snd_dma_continuous_data(GFP_KERNEL),
						     64*1024, 128*1024))) {
		snd_usX2Y_pcm_private_free(pcm);
		return err;
	}
	usX2Y(card)->pcm_devs++;
=======
		snd_pcm_set_ops(pcm, SNDRV_PCM_STREAM_PLAYBACK, &snd_usx2y_pcm_ops);
	snd_pcm_set_ops(pcm, SNDRV_PCM_STREAM_CAPTURE, &snd_usx2y_pcm_ops);

	pcm->private_data = usx2y_substream;
	pcm->private_free = snd_usx2y_pcm_private_free;
	pcm->info_flags = 0;

	sprintf(pcm->name, NAME_ALLCAPS" Audio #%d", usx2y(card)->pcm_devs);

	if (playback_endpoint) {
		snd_pcm_set_managed_buffer(pcm->streams[SNDRV_PCM_STREAM_PLAYBACK].substream,
					   SNDRV_DMA_TYPE_CONTINUOUS,
					   NULL,
					   64*1024, 128*1024);
	}

	snd_pcm_set_managed_buffer(pcm->streams[SNDRV_PCM_STREAM_CAPTURE].substream,
				   SNDRV_DMA_TYPE_CONTINUOUS,
				   NULL,
				   64*1024, 128*1024);
	usx2y(card)->pcm_devs++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

/*
 * create a chip instance and set its names.
 */
<<<<<<< HEAD
int usX2Y_audio_create(struct snd_card *card)
{
	int err = 0;
	
	INIT_LIST_HEAD(&usX2Y(card)->pcm_list);

	if (0 > (err = usX2Y_audio_stream_new(card, 0xA, 0x8)))
		return err;
	if (le16_to_cpu(usX2Y(card)->dev->descriptor.idProduct) == USB_ID_US428)
	     if (0 > (err = usX2Y_audio_stream_new(card, 0, 0xA)))
		     return err;
	if (le16_to_cpu(usX2Y(card)->dev->descriptor.idProduct) != USB_ID_US122)
		err = usX2Y_rate_set(usX2Y(card), 44100);	// Lets us428 recognize output-volume settings, disturbs us122.
=======
int usx2y_audio_create(struct snd_card *card)
{
	int err;

	err = usx2y_audio_stream_new(card, 0xA, 0x8);
	if (err < 0)
		return err;
	if (le16_to_cpu(usx2y(card)->dev->descriptor.idProduct) == USB_ID_US428) {
		err = usx2y_audio_stream_new(card, 0, 0xA);
		if (err < 0)
			return err;
	}
	if (le16_to_cpu(usx2y(card)->dev->descriptor.idProduct) != USB_ID_US122)
		err = usx2y_rate_set(usx2y(card), 44100);	// Lets us428 recognize output-volume settings, disturbs us122.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}
