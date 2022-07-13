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
#include <linux/usb.h>

#include <sound/core.h>
#include <sound/info.h>
#include <sound/pcm.h>

#include "usbaudio.h"
#include "helper.h"
#include "card.h"
<<<<<<< HEAD
=======
#include "endpoint.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "proc.h"

/* convert our full speed USB rate into sampling rate in Hz */
static inline unsigned get_full_speed_hz(unsigned int usb_rate)
{
	return (usb_rate * 125 + (1 << 12)) >> 13;
}

/* convert our high speed USB rate into sampling rate in Hz */
static inline unsigned get_high_speed_hz(unsigned int usb_rate)
{
	return (usb_rate * 125 + (1 << 9)) >> 10;
}

/*
 * common proc files to show the usb device info
 */
static void proc_audio_usbbus_read(struct snd_info_entry *entry, struct snd_info_buffer *buffer)
{
	struct snd_usb_audio *chip = entry->private_data;
<<<<<<< HEAD
	if (!chip->shutdown)
=======
	if (!atomic_read(&chip->shutdown))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		snd_iprintf(buffer, "%03d/%03d\n", chip->dev->bus->busnum, chip->dev->devnum);
}

static void proc_audio_usbid_read(struct snd_info_entry *entry, struct snd_info_buffer *buffer)
{
	struct snd_usb_audio *chip = entry->private_data;
<<<<<<< HEAD
	if (!chip->shutdown)
=======
	if (!atomic_read(&chip->shutdown))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		snd_iprintf(buffer, "%04x:%04x\n", 
			    USB_ID_VENDOR(chip->usb_id),
			    USB_ID_PRODUCT(chip->usb_id));
}

void snd_usb_audio_create_proc(struct snd_usb_audio *chip)
{
<<<<<<< HEAD
	struct snd_info_entry *entry;
	if (!snd_card_proc_new(chip->card, "usbbus", &entry))
		snd_info_set_text_ops(entry, chip, proc_audio_usbbus_read);
	if (!snd_card_proc_new(chip->card, "usbid", &entry))
		snd_info_set_text_ops(entry, chip, proc_audio_usbid_read);
}

=======
	snd_card_ro_proc_new(chip->card, "usbbus", chip,
			     proc_audio_usbbus_read);
	snd_card_ro_proc_new(chip->card, "usbid", chip,
			     proc_audio_usbid_read);
}

static const char * const channel_labels[] = {
	[SNDRV_CHMAP_NA]	= "N/A",
	[SNDRV_CHMAP_MONO]	= "MONO",
	[SNDRV_CHMAP_FL]	= "FL",
	[SNDRV_CHMAP_FR]	= "FR",
	[SNDRV_CHMAP_FC]	= "FC",
	[SNDRV_CHMAP_LFE]	= "LFE",
	[SNDRV_CHMAP_RL]	= "RL",
	[SNDRV_CHMAP_RR]	= "RR",
	[SNDRV_CHMAP_FLC]	= "FLC",
	[SNDRV_CHMAP_FRC]	= "FRC",
	[SNDRV_CHMAP_RC]	= "RC",
	[SNDRV_CHMAP_SL]	= "SL",
	[SNDRV_CHMAP_SR]	= "SR",
	[SNDRV_CHMAP_TC]	= "TC",
	[SNDRV_CHMAP_TFL]	= "TFL",
	[SNDRV_CHMAP_TFC]	= "TFC",
	[SNDRV_CHMAP_TFR]	= "TFR",
	[SNDRV_CHMAP_TRL]	= "TRL",
	[SNDRV_CHMAP_TRC]	= "TRC",
	[SNDRV_CHMAP_TRR]	= "TRR",
	[SNDRV_CHMAP_TFLC]	= "TFLC",
	[SNDRV_CHMAP_TFRC]	= "TFRC",
	[SNDRV_CHMAP_LLFE]	= "LLFE",
	[SNDRV_CHMAP_RLFE]	= "RLFE",
	[SNDRV_CHMAP_TSL]	= "TSL",
	[SNDRV_CHMAP_TSR]	= "TSR",
	[SNDRV_CHMAP_BC]	= "BC",
	[SNDRV_CHMAP_RLC]	= "RLC",
	[SNDRV_CHMAP_RRC]	= "RRC",
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * proc interface for list the supported pcm formats
 */
static void proc_dump_substream_formats(struct snd_usb_substream *subs, struct snd_info_buffer *buffer)
{
<<<<<<< HEAD
	struct list_head *p;
	static char *sync_types[4] = {
		"NONE", "ASYNC", "ADAPTIVE", "SYNC"
	};

	list_for_each(p, &subs->fmt_list) {
		struct audioformat *fp;
		snd_pcm_format_t fmt;
		fp = list_entry(p, struct audioformat, list);
		snd_iprintf(buffer, "  Interface %d\n", fp->iface);
		snd_iprintf(buffer, "    Altset %d\n", fp->altsetting);
		snd_iprintf(buffer, "    Format:");
		for (fmt = 0; fmt <= SNDRV_PCM_FORMAT_LAST; ++fmt)
			if (fp->formats & (1uLL << fmt))
=======
	struct audioformat *fp;
	static const char * const sync_types[4] = {
		"NONE", "ASYNC", "ADAPTIVE", "SYNC"
	};

	list_for_each_entry(fp, &subs->fmt_list, list) {
		snd_pcm_format_t fmt;

		snd_iprintf(buffer, "  Interface %d\n", fp->iface);
		snd_iprintf(buffer, "    Altset %d\n", fp->altsetting);
		snd_iprintf(buffer, "    Format:");
		pcm_for_each_format(fmt)
			if (fp->formats & pcm_format_to_bits(fmt))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				snd_iprintf(buffer, " %s",
					    snd_pcm_format_name(fmt));
		snd_iprintf(buffer, "\n");
		snd_iprintf(buffer, "    Channels: %d\n", fp->channels);
<<<<<<< HEAD
		snd_iprintf(buffer, "    Endpoint: %d %s (%s)\n",
=======
		snd_iprintf(buffer, "    Endpoint: 0x%02x (%d %s) (%s)\n",
			    fp->endpoint,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			    fp->endpoint & USB_ENDPOINT_NUMBER_MASK,
			    fp->endpoint & USB_DIR_IN ? "IN" : "OUT",
			    sync_types[(fp->ep_attr & USB_ENDPOINT_SYNCTYPE) >> 2]);
		if (fp->rates & SNDRV_PCM_RATE_CONTINUOUS) {
			snd_iprintf(buffer, "    Rates: %d - %d (continuous)\n",
				    fp->rate_min, fp->rate_max);
		} else {
			unsigned int i;
			snd_iprintf(buffer, "    Rates: ");
			for (i = 0; i < fp->nr_rates; i++) {
				if (i > 0)
					snd_iprintf(buffer, ", ");
				snd_iprintf(buffer, "%d", fp->rate_table[i]);
			}
			snd_iprintf(buffer, "\n");
		}
		if (subs->speed != USB_SPEED_FULL)
			snd_iprintf(buffer, "    Data packet interval: %d us\n",
				    125 * (1 << fp->datainterval));
<<<<<<< HEAD
=======
		snd_iprintf(buffer, "    Bits: %d\n", fp->fmt_bits);

		if (fp->dsd_raw)
			snd_iprintf(buffer, "    DSD raw: DOP=%d, bitrev=%d\n",
				    fp->dsd_dop, fp->dsd_bitrev);

		if (fp->chmap) {
			const struct snd_pcm_chmap_elem *map = fp->chmap;
			int c;

			snd_iprintf(buffer, "    Channel map:");
			for (c = 0; c < map->channels; c++) {
				if (map->map[c] >= ARRAY_SIZE(channel_labels) ||
				    !channel_labels[map->map[c]])
					snd_iprintf(buffer, " --");
				else
					snd_iprintf(buffer, " %s",
						    channel_labels[map->map[c]]);
			}
			snd_iprintf(buffer, "\n");
		}

		if (fp->sync_ep) {
			snd_iprintf(buffer, "    Sync Endpoint: 0x%02x (%d %s)\n",
				    fp->sync_ep,
				    fp->sync_ep & USB_ENDPOINT_NUMBER_MASK,
				    fp->sync_ep & USB_DIR_IN ? "IN" : "OUT");
			snd_iprintf(buffer, "    Sync EP Interface: %d\n",
				    fp->sync_iface);
			snd_iprintf(buffer, "    Sync EP Altset: %d\n",
				    fp->sync_altsetting);
			snd_iprintf(buffer, "    Implicit Feedback Mode: %s\n",
				    fp->implicit_fb ? "Yes" : "No");
		}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		// snd_iprintf(buffer, "    Max Packet Size = %d\n", fp->maxpacksize);
		// snd_iprintf(buffer, "    EP Attribute = %#x\n", fp->attributes);
	}
}

<<<<<<< HEAD
static void proc_dump_substream_status(struct snd_usb_substream *subs, struct snd_info_buffer *buffer)
{
	if (subs->running) {
		unsigned int i;
		snd_iprintf(buffer, "  Status: Running\n");
		snd_iprintf(buffer, "    Interface = %d\n", subs->interface);
		snd_iprintf(buffer, "    Altset = %d\n", subs->altset_idx);
		snd_iprintf(buffer, "    URBs = %d [ ", subs->nurbs);
		for (i = 0; i < subs->nurbs; i++)
			snd_iprintf(buffer, "%d ", subs->dataurb[i].packets);
		snd_iprintf(buffer, "]\n");
		snd_iprintf(buffer, "    Packet Size = %d\n", subs->curpacksize);
		snd_iprintf(buffer, "    Momentary freq = %u Hz (%#x.%04x)\n",
			    subs->speed == USB_SPEED_FULL
			    ? get_full_speed_hz(subs->freqm)
			    : get_high_speed_hz(subs->freqm),
			    subs->freqm >> 16, subs->freqm & 0xffff);
		if (subs->freqshift != INT_MIN)
			snd_iprintf(buffer, "    Feedback Format = %d.%d\n",
				    (subs->syncmaxsize > 3 ? 32 : 24)
						- (16 - subs->freqshift),
				    16 - subs->freqshift);
	} else {
		snd_iprintf(buffer, "  Status: Stop\n");
	}
=======
static void proc_dump_ep_status(struct snd_usb_substream *subs,
				struct snd_usb_endpoint *data_ep,
				struct snd_usb_endpoint *sync_ep,
				struct snd_info_buffer *buffer)
{
	if (!data_ep)
		return;
	snd_iprintf(buffer, "    Packet Size = %d\n", data_ep->curpacksize);
	snd_iprintf(buffer, "    Momentary freq = %u Hz (%#x.%04x)\n",
		    subs->speed == USB_SPEED_FULL
		    ? get_full_speed_hz(data_ep->freqm)
		    : get_high_speed_hz(data_ep->freqm),
		    data_ep->freqm >> 16, data_ep->freqm & 0xffff);
	if (sync_ep && data_ep->freqshift != INT_MIN) {
		int res = 16 - data_ep->freqshift;
		snd_iprintf(buffer, "    Feedback Format = %d.%d\n",
			    (sync_ep->syncmaxsize > 3 ? 32 : 24) - res, res);
	}
}

static void proc_dump_substream_status(struct snd_usb_audio *chip,
				       struct snd_usb_substream *subs,
				       struct snd_info_buffer *buffer)
{
	mutex_lock(&chip->mutex);
	if (subs->running) {
		snd_iprintf(buffer, "  Status: Running\n");
		if (subs->cur_audiofmt) {
			snd_iprintf(buffer, "    Interface = %d\n", subs->cur_audiofmt->iface);
			snd_iprintf(buffer, "    Altset = %d\n", subs->cur_audiofmt->altsetting);
		}
		proc_dump_ep_status(subs, subs->data_endpoint, subs->sync_endpoint, buffer);
	} else {
		snd_iprintf(buffer, "  Status: Stop\n");
	}
	mutex_unlock(&chip->mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void proc_pcm_format_read(struct snd_info_entry *entry, struct snd_info_buffer *buffer)
{
	struct snd_usb_stream *stream = entry->private_data;
<<<<<<< HEAD

	snd_iprintf(buffer, "%s : %s\n", stream->chip->card->longname, stream->pcm->name);

	if (stream->substream[SNDRV_PCM_STREAM_PLAYBACK].num_formats) {
		snd_iprintf(buffer, "\nPlayback:\n");
		proc_dump_substream_status(&stream->substream[SNDRV_PCM_STREAM_PLAYBACK], buffer);
=======
	struct snd_usb_audio *chip = stream->chip;

	snd_iprintf(buffer, "%s : %s\n", chip->card->longname, stream->pcm->name);

	if (stream->substream[SNDRV_PCM_STREAM_PLAYBACK].num_formats) {
		snd_iprintf(buffer, "\nPlayback:\n");
		proc_dump_substream_status(chip, &stream->substream[SNDRV_PCM_STREAM_PLAYBACK], buffer);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		proc_dump_substream_formats(&stream->substream[SNDRV_PCM_STREAM_PLAYBACK], buffer);
	}
	if (stream->substream[SNDRV_PCM_STREAM_CAPTURE].num_formats) {
		snd_iprintf(buffer, "\nCapture:\n");
<<<<<<< HEAD
		proc_dump_substream_status(&stream->substream[SNDRV_PCM_STREAM_CAPTURE], buffer);
=======
		proc_dump_substream_status(chip, &stream->substream[SNDRV_PCM_STREAM_CAPTURE], buffer);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		proc_dump_substream_formats(&stream->substream[SNDRV_PCM_STREAM_CAPTURE], buffer);
	}
}

void snd_usb_proc_pcm_format_add(struct snd_usb_stream *stream)
{
<<<<<<< HEAD
	struct snd_info_entry *entry;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	char name[32];
	struct snd_card *card = stream->chip->card;

	sprintf(name, "stream%d", stream->pcm_index);
<<<<<<< HEAD
	if (!snd_card_proc_new(card, name, &entry))
		snd_info_set_text_ops(entry, stream, proc_pcm_format_read);
=======
	snd_card_ro_proc_new(card, name, stream, proc_pcm_format_read);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

