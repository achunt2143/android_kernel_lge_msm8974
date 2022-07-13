<<<<<<< HEAD
/*
 *  Digital Audio (PCM) abstract layer
 *  Copyright (c) by Jaroslav Kysela <perex@perex.cz>
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
 *
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 *  Digital Audio (PCM) abstract layer
 *  Copyright (c) by Jaroslav Kysela <perex@perex.cz>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/init.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/time.h>
#include <linux/mutex.h>
<<<<<<< HEAD
#include <sound/core.h>
#include <sound/minors.h>
#include <sound/pcm.h>
#include <sound/control.h>
#include <sound/info.h>

=======
#include <linux/device.h>
#include <linux/nospec.h>
#include <sound/core.h>
#include <sound/minors.h>
#include <sound/pcm.h>
#include <sound/timer.h>
#include <sound/control.h>
#include <sound/info.h>

#include "pcm_local.h"

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_AUTHOR("Jaroslav Kysela <perex@perex.cz>, Abramo Bagnara <abramo@alsa-project.org>");
MODULE_DESCRIPTION("Midlevel PCM code for ALSA.");
MODULE_LICENSE("GPL");

static LIST_HEAD(snd_pcm_devices);
<<<<<<< HEAD
static LIST_HEAD(snd_pcm_notify_list);
static DEFINE_MUTEX(register_mutex);
=======
static DEFINE_MUTEX(register_mutex);
#if IS_ENABLED(CONFIG_SND_PCM_OSS)
static LIST_HEAD(snd_pcm_notify_list);
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int snd_pcm_free(struct snd_pcm *pcm);
static int snd_pcm_dev_free(struct snd_device *device);
static int snd_pcm_dev_register(struct snd_device *device);
<<<<<<< HEAD
static int snd_pcm_dev_register_soc_be(struct snd_device *device);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int snd_pcm_dev_disconnect(struct snd_device *device);

static struct snd_pcm *snd_pcm_get(struct snd_card *card, int device)
{
	struct snd_pcm *pcm;

	list_for_each_entry(pcm, &snd_pcm_devices, list) {
<<<<<<< HEAD
		if (pcm->internal)
			continue;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (pcm->card == card && pcm->device == device)
			return pcm;
	}
	return NULL;
}

static int snd_pcm_next(struct snd_card *card, int device)
{
	struct snd_pcm *pcm;

	list_for_each_entry(pcm, &snd_pcm_devices, list) {
<<<<<<< HEAD
		if (pcm->internal)
			continue;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (pcm->card == card && pcm->device > device)
			return pcm->device;
		else if (pcm->card->number > card->number)
			return -1;
	}
	return -1;
}

static int snd_pcm_add(struct snd_pcm *newpcm)
{
	struct snd_pcm *pcm;

<<<<<<< HEAD
=======
	if (newpcm->internal)
		return 0;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	list_for_each_entry(pcm, &snd_pcm_devices, list) {
		if (pcm->card == newpcm->card && pcm->device == newpcm->device)
			return -EBUSY;
		if (pcm->card->number > newpcm->card->number ||
				(pcm->card == newpcm->card &&
				pcm->device > newpcm->device)) {
			list_add(&newpcm->list, pcm->list.prev);
			return 0;
		}
	}
	list_add_tail(&newpcm->list, &snd_pcm_devices);
	return 0;
}

static int snd_pcm_control_ioctl(struct snd_card *card,
				 struct snd_ctl_file *control,
				 unsigned int cmd, unsigned long arg)
{
	switch (cmd) {
	case SNDRV_CTL_IOCTL_PCM_NEXT_DEVICE:
		{
			int device;

			if (get_user(device, (int __user *)arg))
				return -EFAULT;
<<<<<<< HEAD
			mutex_lock(&register_mutex);
			device = snd_pcm_next(card, device);
			mutex_unlock(&register_mutex);
=======
			scoped_guard(mutex, &register_mutex)
				device = snd_pcm_next(card, device);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (put_user(device, (int __user *)arg))
				return -EFAULT;
			return 0;
		}
	case SNDRV_CTL_IOCTL_PCM_INFO:
		{
			struct snd_pcm_info __user *info;
			unsigned int device, subdevice;
			int stream;
			struct snd_pcm *pcm;
			struct snd_pcm_str *pstr;
			struct snd_pcm_substream *substream;
<<<<<<< HEAD
			int err;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

			info = (struct snd_pcm_info __user *)arg;
			if (get_user(device, &info->device))
				return -EFAULT;
			if (get_user(stream, &info->stream))
				return -EFAULT;
			if (stream < 0 || stream > 1)
				return -EINVAL;
<<<<<<< HEAD
			if (get_user(subdevice, &info->subdevice))
				return -EFAULT;
			mutex_lock(&register_mutex);
			pcm = snd_pcm_get(card, device);
			if (pcm == NULL) {
				err = -ENXIO;
				goto _error;
			}
			pstr = &pcm->streams[stream];
			if (pstr->substream_count == 0) {
				err = -ENOENT;
				goto _error;
			}
			if (subdevice >= pstr->substream_count) {
				err = -ENXIO;
				goto _error;
			}
=======
			stream = array_index_nospec(stream, 2);
			if (get_user(subdevice, &info->subdevice))
				return -EFAULT;
			guard(mutex)(&register_mutex);
			pcm = snd_pcm_get(card, device);
			if (pcm == NULL)
				return -ENXIO;
			pstr = &pcm->streams[stream];
			if (pstr->substream_count == 0)
				return -ENOENT;
			if (subdevice >= pstr->substream_count)
				return -ENXIO;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			for (substream = pstr->substream; substream;
			     substream = substream->next)
				if (substream->number == (int)subdevice)
					break;
<<<<<<< HEAD
			if (substream == NULL) {
				err = -ENXIO;
				goto _error;
			}
			err = snd_pcm_info_user(substream, info);
		_error:
			mutex_unlock(&register_mutex);
			return err;
=======
			if (substream == NULL)
				return -ENXIO;
			guard(mutex)(&pcm->open_mutex);
			return snd_pcm_info_user(substream, info);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	case SNDRV_CTL_IOCTL_PCM_PREFER_SUBDEVICE:
		{
			int val;
			
			if (get_user(val, (int __user *)arg))
				return -EFAULT;
<<<<<<< HEAD
			control->prefer_pcm_subdevice = val;
=======
			control->preferred_subdevice[SND_CTL_SUBDEV_PCM] = val;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return 0;
		}
	}
	return -ENOIOCTLCMD;
}

#define FORMAT(v) [SNDRV_PCM_FORMAT_##v] = #v

<<<<<<< HEAD
static char *snd_pcm_format_names[] = {
=======
static const char * const snd_pcm_format_names[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	FORMAT(S8),
	FORMAT(U8),
	FORMAT(S16_LE),
	FORMAT(S16_BE),
	FORMAT(U16_LE),
	FORMAT(U16_BE),
	FORMAT(S24_LE),
	FORMAT(S24_BE),
	FORMAT(U24_LE),
	FORMAT(U24_BE),
	FORMAT(S32_LE),
	FORMAT(S32_BE),
	FORMAT(U32_LE),
	FORMAT(U32_BE),
	FORMAT(FLOAT_LE),
	FORMAT(FLOAT_BE),
	FORMAT(FLOAT64_LE),
	FORMAT(FLOAT64_BE),
	FORMAT(IEC958_SUBFRAME_LE),
	FORMAT(IEC958_SUBFRAME_BE),
	FORMAT(MU_LAW),
	FORMAT(A_LAW),
	FORMAT(IMA_ADPCM),
	FORMAT(MPEG),
	FORMAT(GSM),
	FORMAT(SPECIAL),
	FORMAT(S24_3LE),
	FORMAT(S24_3BE),
	FORMAT(U24_3LE),
	FORMAT(U24_3BE),
	FORMAT(S20_3LE),
	FORMAT(S20_3BE),
	FORMAT(U20_3LE),
	FORMAT(U20_3BE),
	FORMAT(S18_3LE),
	FORMAT(S18_3BE),
	FORMAT(U18_3LE),
	FORMAT(U18_3BE),
	FORMAT(G723_24),
	FORMAT(G723_24_1B),
	FORMAT(G723_40),
	FORMAT(G723_40_1B),
<<<<<<< HEAD
};

const char *snd_pcm_format_name(snd_pcm_format_t format)
{
	if ((__force unsigned int)format >= ARRAY_SIZE(snd_pcm_format_names))
		return "Unknown";
	return snd_pcm_format_names[(__force unsigned int)format];
=======
	FORMAT(DSD_U8),
	FORMAT(DSD_U16_LE),
	FORMAT(DSD_U32_LE),
	FORMAT(DSD_U16_BE),
	FORMAT(DSD_U32_BE),
	FORMAT(S20_LE),
	FORMAT(S20_BE),
	FORMAT(U20_LE),
	FORMAT(U20_BE),
};

/**
 * snd_pcm_format_name - Return a name string for the given PCM format
 * @format: PCM format
 *
 * Return: the format name string
 */
const char *snd_pcm_format_name(snd_pcm_format_t format)
{
	unsigned int format_num = (__force unsigned int)format;

	if (format_num >= ARRAY_SIZE(snd_pcm_format_names) || !snd_pcm_format_names[format_num])
		return "Unknown";
	return snd_pcm_format_names[format_num];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(snd_pcm_format_name);

#ifdef CONFIG_SND_VERBOSE_PROCFS

#define STATE(v) [SNDRV_PCM_STATE_##v] = #v
#define STREAM(v) [SNDRV_PCM_STREAM_##v] = #v
#define READY(v) [SNDRV_PCM_READY_##v] = #v
#define XRUN(v) [SNDRV_PCM_XRUN_##v] = #v
#define SILENCE(v) [SNDRV_PCM_SILENCE_##v] = #v
#define TSTAMP(v) [SNDRV_PCM_TSTAMP_##v] = #v
#define ACCESS(v) [SNDRV_PCM_ACCESS_##v] = #v
#define START(v) [SNDRV_PCM_START_##v] = #v
#define SUBFORMAT(v) [SNDRV_PCM_SUBFORMAT_##v] = #v 

<<<<<<< HEAD
static char *snd_pcm_stream_names[] = {
=======
static const char * const snd_pcm_stream_names[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	STREAM(PLAYBACK),
	STREAM(CAPTURE),
};

<<<<<<< HEAD
static char *snd_pcm_state_names[] = {
=======
static const char * const snd_pcm_state_names[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	STATE(OPEN),
	STATE(SETUP),
	STATE(PREPARED),
	STATE(RUNNING),
	STATE(XRUN),
	STATE(DRAINING),
	STATE(PAUSED),
	STATE(SUSPENDED),
<<<<<<< HEAD
};

static char *snd_pcm_access_names[] = {
=======
	STATE(DISCONNECTED),
};

static const char * const snd_pcm_access_names[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ACCESS(MMAP_INTERLEAVED), 
	ACCESS(MMAP_NONINTERLEAVED),
	ACCESS(MMAP_COMPLEX),
	ACCESS(RW_INTERLEAVED),
	ACCESS(RW_NONINTERLEAVED),
};

<<<<<<< HEAD
static char *snd_pcm_subformat_names[] = {
	SUBFORMAT(STD), 
};

static char *snd_pcm_tstamp_mode_names[] = {
=======
static const char * const snd_pcm_subformat_names[] = {
	SUBFORMAT(STD), 
	SUBFORMAT(MSBITS_MAX),
	SUBFORMAT(MSBITS_20),
	SUBFORMAT(MSBITS_24),
};

static const char * const snd_pcm_tstamp_mode_names[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	TSTAMP(NONE),
	TSTAMP(ENABLE),
};

static const char *snd_pcm_stream_name(int stream)
{
	return snd_pcm_stream_names[stream];
}

static const char *snd_pcm_access_name(snd_pcm_access_t access)
{
	return snd_pcm_access_names[(__force int)access];
}

static const char *snd_pcm_subformat_name(snd_pcm_subformat_t subformat)
{
	return snd_pcm_subformat_names[(__force int)subformat];
}

static const char *snd_pcm_tstamp_mode_name(int mode)
{
	return snd_pcm_tstamp_mode_names[mode];
}

static const char *snd_pcm_state_name(snd_pcm_state_t state)
{
	return snd_pcm_state_names[(__force int)state];
}

<<<<<<< HEAD
#if defined(CONFIG_SND_PCM_OSS) || defined(CONFIG_SND_PCM_OSS_MODULE)
=======
#if IS_ENABLED(CONFIG_SND_PCM_OSS)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/soundcard.h>

static const char *snd_pcm_oss_format_name(int format)
{
	switch (format) {
	case AFMT_MU_LAW:
		return "MU_LAW";
	case AFMT_A_LAW:
		return "A_LAW";
	case AFMT_IMA_ADPCM:
		return "IMA_ADPCM";
	case AFMT_U8:
		return "U8";
	case AFMT_S16_LE:
		return "S16_LE";
	case AFMT_S16_BE:
		return "S16_BE";
	case AFMT_S8:
		return "S8";
	case AFMT_U16_LE:
		return "U16_LE";
	case AFMT_U16_BE:
		return "U16_BE";
	case AFMT_MPEG:
		return "MPEG";
	default:
		return "unknown";
	}
}
#endif

static void snd_pcm_proc_info_read(struct snd_pcm_substream *substream,
				   struct snd_info_buffer *buffer)
{
<<<<<<< HEAD
	struct snd_pcm_info *info;
=======
	struct snd_pcm_info *info __free(kfree) = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int err;

	if (! substream)
		return;

	info = kmalloc(sizeof(*info), GFP_KERNEL);
<<<<<<< HEAD
	if (! info) {
		printk(KERN_DEBUG "snd_pcm_proc_info_read: cannot malloc\n");
		return;
	}
=======
	if (!info)
		return;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	err = snd_pcm_info(substream, info);
	if (err < 0) {
		snd_iprintf(buffer, "error %d\n", err);
<<<<<<< HEAD
		kfree(info);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;
	}
	snd_iprintf(buffer, "card: %d\n", info->card);
	snd_iprintf(buffer, "device: %d\n", info->device);
	snd_iprintf(buffer, "subdevice: %d\n", info->subdevice);
	snd_iprintf(buffer, "stream: %s\n", snd_pcm_stream_name(info->stream));
	snd_iprintf(buffer, "id: %s\n", info->id);
	snd_iprintf(buffer, "name: %s\n", info->name);
	snd_iprintf(buffer, "subname: %s\n", info->subname);
	snd_iprintf(buffer, "class: %d\n", info->dev_class);
	snd_iprintf(buffer, "subclass: %d\n", info->dev_subclass);
	snd_iprintf(buffer, "subdevices_count: %d\n", info->subdevices_count);
	snd_iprintf(buffer, "subdevices_avail: %d\n", info->subdevices_avail);
<<<<<<< HEAD
	kfree(info);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void snd_pcm_stream_proc_info_read(struct snd_info_entry *entry,
					  struct snd_info_buffer *buffer)
{
	snd_pcm_proc_info_read(((struct snd_pcm_str *)entry->private_data)->substream,
			       buffer);
}

static void snd_pcm_substream_proc_info_read(struct snd_info_entry *entry,
					     struct snd_info_buffer *buffer)
{
	snd_pcm_proc_info_read(entry->private_data, buffer);
}

static void snd_pcm_substream_proc_hw_params_read(struct snd_info_entry *entry,
						  struct snd_info_buffer *buffer)
{
	struct snd_pcm_substream *substream = entry->private_data;
	struct snd_pcm_runtime *runtime;

<<<<<<< HEAD
	mutex_lock(&substream->pcm->open_mutex);
	runtime = substream->runtime;
	if (!runtime) {
		snd_iprintf(buffer, "closed\n");
		goto unlock;
	}
	if (runtime->status->state == SNDRV_PCM_STATE_OPEN) {
		snd_iprintf(buffer, "no setup\n");
		goto unlock;
=======
	guard(mutex)(&substream->pcm->open_mutex);
	runtime = substream->runtime;
	if (!runtime) {
		snd_iprintf(buffer, "closed\n");
		return;
	}
	if (runtime->state == SNDRV_PCM_STATE_OPEN) {
		snd_iprintf(buffer, "no setup\n");
		return;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	snd_iprintf(buffer, "access: %s\n", snd_pcm_access_name(runtime->access));
	snd_iprintf(buffer, "format: %s\n", snd_pcm_format_name(runtime->format));
	snd_iprintf(buffer, "subformat: %s\n", snd_pcm_subformat_name(runtime->subformat));
	snd_iprintf(buffer, "channels: %u\n", runtime->channels);	
	snd_iprintf(buffer, "rate: %u (%u/%u)\n", runtime->rate, runtime->rate_num, runtime->rate_den);	
	snd_iprintf(buffer, "period_size: %lu\n", runtime->period_size);	
	snd_iprintf(buffer, "buffer_size: %lu\n", runtime->buffer_size);	
<<<<<<< HEAD
#if defined(CONFIG_SND_PCM_OSS) || defined(CONFIG_SND_PCM_OSS_MODULE)
=======
#if IS_ENABLED(CONFIG_SND_PCM_OSS)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (substream->oss.oss) {
		snd_iprintf(buffer, "OSS format: %s\n", snd_pcm_oss_format_name(runtime->oss.format));
		snd_iprintf(buffer, "OSS channels: %u\n", runtime->oss.channels);	
		snd_iprintf(buffer, "OSS rate: %u\n", runtime->oss.rate);
		snd_iprintf(buffer, "OSS period bytes: %lu\n", (unsigned long)runtime->oss.period_bytes);
		snd_iprintf(buffer, "OSS periods: %u\n", runtime->oss.periods);
		snd_iprintf(buffer, "OSS period frames: %lu\n", (unsigned long)runtime->oss.period_frames);
	}
#endif
<<<<<<< HEAD
 unlock:
	mutex_unlock(&substream->pcm->open_mutex);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void snd_pcm_substream_proc_sw_params_read(struct snd_info_entry *entry,
						  struct snd_info_buffer *buffer)
{
	struct snd_pcm_substream *substream = entry->private_data;
	struct snd_pcm_runtime *runtime;

<<<<<<< HEAD
	mutex_lock(&substream->pcm->open_mutex);
	runtime = substream->runtime;
	if (!runtime) {
		snd_iprintf(buffer, "closed\n");
		goto unlock;
	}
	if (runtime->status->state == SNDRV_PCM_STATE_OPEN) {
		snd_iprintf(buffer, "no setup\n");
		goto unlock;
=======
	guard(mutex)(&substream->pcm->open_mutex);
	runtime = substream->runtime;
	if (!runtime) {
		snd_iprintf(buffer, "closed\n");
		return;
	}
	if (runtime->state == SNDRV_PCM_STATE_OPEN) {
		snd_iprintf(buffer, "no setup\n");
		return;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	snd_iprintf(buffer, "tstamp_mode: %s\n", snd_pcm_tstamp_mode_name(runtime->tstamp_mode));
	snd_iprintf(buffer, "period_step: %u\n", runtime->period_step);
	snd_iprintf(buffer, "avail_min: %lu\n", runtime->control->avail_min);
	snd_iprintf(buffer, "start_threshold: %lu\n", runtime->start_threshold);
	snd_iprintf(buffer, "stop_threshold: %lu\n", runtime->stop_threshold);
	snd_iprintf(buffer, "silence_threshold: %lu\n", runtime->silence_threshold);
	snd_iprintf(buffer, "silence_size: %lu\n", runtime->silence_size);
	snd_iprintf(buffer, "boundary: %lu\n", runtime->boundary);
<<<<<<< HEAD
 unlock:
	mutex_unlock(&substream->pcm->open_mutex);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void snd_pcm_substream_proc_status_read(struct snd_info_entry *entry,
					       struct snd_info_buffer *buffer)
{
	struct snd_pcm_substream *substream = entry->private_data;
	struct snd_pcm_runtime *runtime;
<<<<<<< HEAD
	struct snd_pcm_status status;
	int err;

	mutex_lock(&substream->pcm->open_mutex);
	runtime = substream->runtime;
	if (!runtime) {
		snd_iprintf(buffer, "closed\n");
		goto unlock;
	}
	memset(&status, 0, sizeof(status));
	err = snd_pcm_status(substream, &status);
	if (err < 0) {
		snd_iprintf(buffer, "error %d\n", err);
		goto unlock;
	}
	snd_iprintf(buffer, "state: %s\n", snd_pcm_state_name(status.state));
	snd_iprintf(buffer, "owner_pid   : %d\n", pid_vnr(substream->pid));
	snd_iprintf(buffer, "trigger_time: %ld.%09ld\n",
		status.trigger_tstamp.tv_sec, status.trigger_tstamp.tv_nsec);
	snd_iprintf(buffer, "tstamp      : %ld.%09ld\n",
		status.tstamp.tv_sec, status.tstamp.tv_nsec);
=======
	struct snd_pcm_status64 status;
	int err;

	guard(mutex)(&substream->pcm->open_mutex);
	runtime = substream->runtime;
	if (!runtime) {
		snd_iprintf(buffer, "closed\n");
		return;
	}
	memset(&status, 0, sizeof(status));
	err = snd_pcm_status64(substream, &status);
	if (err < 0) {
		snd_iprintf(buffer, "error %d\n", err);
		return;
	}
	snd_iprintf(buffer, "state: %s\n", snd_pcm_state_name(status.state));
	snd_iprintf(buffer, "owner_pid   : %d\n", pid_vnr(substream->pid));
	snd_iprintf(buffer, "trigger_time: %lld.%09lld\n",
		status.trigger_tstamp_sec, status.trigger_tstamp_nsec);
	snd_iprintf(buffer, "tstamp      : %lld.%09lld\n",
		status.tstamp_sec, status.tstamp_nsec);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	snd_iprintf(buffer, "delay       : %ld\n", status.delay);
	snd_iprintf(buffer, "avail       : %ld\n", status.avail);
	snd_iprintf(buffer, "avail_max   : %ld\n", status.avail_max);
	snd_iprintf(buffer, "-----\n");
	snd_iprintf(buffer, "hw_ptr      : %ld\n", runtime->status->hw_ptr);
	snd_iprintf(buffer, "appl_ptr    : %ld\n", runtime->control->appl_ptr);
<<<<<<< HEAD
 unlock:
	mutex_unlock(&substream->pcm->open_mutex);
}

#ifdef CONFIG_SND_PCM_XRUN_DEBUG
=======
}

#ifdef CONFIG_SND_PCM_XRUN_DEBUG
static void snd_pcm_xrun_injection_write(struct snd_info_entry *entry,
					 struct snd_info_buffer *buffer)
{
	struct snd_pcm_substream *substream = entry->private_data;

	snd_pcm_stop_xrun(substream);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void snd_pcm_xrun_debug_read(struct snd_info_entry *entry,
				    struct snd_info_buffer *buffer)
{
	struct snd_pcm_str *pstr = entry->private_data;
	snd_iprintf(buffer, "%d\n", pstr->xrun_debug);
}

static void snd_pcm_xrun_debug_write(struct snd_info_entry *entry,
				     struct snd_info_buffer *buffer)
{
	struct snd_pcm_str *pstr = entry->private_data;
	char line[64];
	if (!snd_info_get_line(buffer, line, sizeof(line)))
		pstr->xrun_debug = simple_strtoul(line, NULL, 10);
}
#endif

static int snd_pcm_stream_proc_init(struct snd_pcm_str *pstr)
{
	struct snd_pcm *pcm = pstr->pcm;
	struct snd_info_entry *entry;
	char name[16];

	sprintf(name, "pcm%i%c", pcm->device, 
		pstr->stream == SNDRV_PCM_STREAM_PLAYBACK ? 'p' : 'c');
<<<<<<< HEAD
	if ((entry = snd_info_create_card_entry(pcm->card, name, pcm->card->proc_root)) == NULL)
		return -ENOMEM;
	entry->mode = S_IFDIR | S_IRUGO | S_IXUGO;
	if (snd_info_register(entry) < 0) {
		snd_info_free_entry(entry);
		return -ENOMEM;
	}
	pstr->proc_root = entry;

	if ((entry = snd_info_create_card_entry(pcm->card, "info", pstr->proc_root)) != NULL) {
		snd_info_set_text_ops(entry, pstr, snd_pcm_stream_proc_info_read);
		if (snd_info_register(entry) < 0) {
			snd_info_free_entry(entry);
			entry = NULL;
		}
	}
	pstr->proc_info_entry = entry;

#ifdef CONFIG_SND_PCM_XRUN_DEBUG
	if ((entry = snd_info_create_card_entry(pcm->card, "xrun_debug",
						pstr->proc_root)) != NULL) {
		entry->c.text.read = snd_pcm_xrun_debug_read;
		entry->c.text.write = snd_pcm_xrun_debug_write;
		entry->mode |= S_IWUSR;
		entry->private_data = pstr;
		if (snd_info_register(entry) < 0) {
			snd_info_free_entry(entry);
			entry = NULL;
		}
	}
	pstr->proc_xrun_debug_entry = entry;
=======
	entry = snd_info_create_card_entry(pcm->card, name,
					   pcm->card->proc_root);
	if (!entry)
		return -ENOMEM;
	entry->mode = S_IFDIR | 0555;
	pstr->proc_root = entry;
	entry = snd_info_create_card_entry(pcm->card, "info", pstr->proc_root);
	if (entry)
		snd_info_set_text_ops(entry, pstr, snd_pcm_stream_proc_info_read);
#ifdef CONFIG_SND_PCM_XRUN_DEBUG
	entry = snd_info_create_card_entry(pcm->card, "xrun_debug",
					   pstr->proc_root);
	if (entry) {
		snd_info_set_text_ops(entry, pstr, snd_pcm_xrun_debug_read);
		entry->c.text.write = snd_pcm_xrun_debug_write;
		entry->mode |= 0200;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
	return 0;
}

static int snd_pcm_stream_proc_done(struct snd_pcm_str *pstr)
{
<<<<<<< HEAD
#ifdef CONFIG_SND_PCM_XRUN_DEBUG
	snd_info_free_entry(pstr->proc_xrun_debug_entry);
	pstr->proc_xrun_debug_entry = NULL;
#endif
	snd_info_free_entry(pstr->proc_info_entry);
	pstr->proc_info_entry = NULL;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	snd_info_free_entry(pstr->proc_root);
	pstr->proc_root = NULL;
	return 0;
}

<<<<<<< HEAD
=======
static struct snd_info_entry *
create_substream_info_entry(struct snd_pcm_substream *substream,
			    const char *name,
			    void (*read)(struct snd_info_entry *,
					 struct snd_info_buffer *))
{
	struct snd_info_entry *entry;

	entry = snd_info_create_card_entry(substream->pcm->card, name,
					   substream->proc_root);
	if (entry)
		snd_info_set_text_ops(entry, substream, read);
	return entry;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int snd_pcm_substream_proc_init(struct snd_pcm_substream *substream)
{
	struct snd_info_entry *entry;
	struct snd_card *card;
	char name[16];

	card = substream->pcm->card;

	sprintf(name, "sub%i", substream->number);
<<<<<<< HEAD
	if ((entry = snd_info_create_card_entry(card, name, substream->pstr->proc_root)) == NULL)
		return -ENOMEM;
	entry->mode = S_IFDIR | S_IRUGO | S_IXUGO;
	if (snd_info_register(entry) < 0) {
		snd_info_free_entry(entry);
		return -ENOMEM;
	}
	substream->proc_root = entry;

	if ((entry = snd_info_create_card_entry(card, "info", substream->proc_root)) != NULL) {
		snd_info_set_text_ops(entry, substream,
				      snd_pcm_substream_proc_info_read);
		if (snd_info_register(entry) < 0) {
			snd_info_free_entry(entry);
			entry = NULL;
		}
	}
	substream->proc_info_entry = entry;

	if ((entry = snd_info_create_card_entry(card, "hw_params", substream->proc_root)) != NULL) {
		snd_info_set_text_ops(entry, substream,
				      snd_pcm_substream_proc_hw_params_read);
		if (snd_info_register(entry) < 0) {
			snd_info_free_entry(entry);
			entry = NULL;
		}
	}
	substream->proc_hw_params_entry = entry;

	if ((entry = snd_info_create_card_entry(card, "sw_params", substream->proc_root)) != NULL) {
		snd_info_set_text_ops(entry, substream,
				      snd_pcm_substream_proc_sw_params_read);
		if (snd_info_register(entry) < 0) {
			snd_info_free_entry(entry);
			entry = NULL;
		}
	}
	substream->proc_sw_params_entry = entry;

	if ((entry = snd_info_create_card_entry(card, "status", substream->proc_root)) != NULL) {
		snd_info_set_text_ops(entry, substream,
				      snd_pcm_substream_proc_status_read);
		if (snd_info_register(entry) < 0) {
			snd_info_free_entry(entry);
			entry = NULL;
		}
	}
	substream->proc_status_entry = entry;
=======
	entry = snd_info_create_card_entry(card, name,
					   substream->pstr->proc_root);
	if (!entry)
		return -ENOMEM;
	entry->mode = S_IFDIR | 0555;
	substream->proc_root = entry;

	create_substream_info_entry(substream, "info",
				    snd_pcm_substream_proc_info_read);
	create_substream_info_entry(substream, "hw_params",
				    snd_pcm_substream_proc_hw_params_read);
	create_substream_info_entry(substream, "sw_params",
				    snd_pcm_substream_proc_sw_params_read);
	create_substream_info_entry(substream, "status",
				    snd_pcm_substream_proc_status_read);

#ifdef CONFIG_SND_PCM_XRUN_DEBUG
	entry = create_substream_info_entry(substream, "xrun_injection", NULL);
	if (entry) {
		entry->c.text.write = snd_pcm_xrun_injection_write;
		entry->mode = S_IFREG | 0200;
	}
#endif /* CONFIG_SND_PCM_XRUN_DEBUG */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static int snd_pcm_substream_proc_done(struct snd_pcm_substream *substream)
{
	snd_info_free_entry(substream->proc_info_entry);
	substream->proc_info_entry = NULL;
	snd_info_free_entry(substream->proc_hw_params_entry);
	substream->proc_hw_params_entry = NULL;
	snd_info_free_entry(substream->proc_sw_params_entry);
	substream->proc_sw_params_entry = NULL;
	snd_info_free_entry(substream->proc_status_entry);
	substream->proc_status_entry = NULL;
	snd_info_free_entry(substream->proc_root);
	substream->proc_root = NULL;
	return 0;
}
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#else /* !CONFIG_SND_VERBOSE_PROCFS */
static inline int snd_pcm_stream_proc_init(struct snd_pcm_str *pstr) { return 0; }
static inline int snd_pcm_stream_proc_done(struct snd_pcm_str *pstr) { return 0; }
static inline int snd_pcm_substream_proc_init(struct snd_pcm_substream *substream) { return 0; }
<<<<<<< HEAD
static inline int snd_pcm_substream_proc_done(struct snd_pcm_substream *substream) { return 0; }
#endif /* CONFIG_SND_VERBOSE_PROCFS */

=======
#endif /* CONFIG_SND_VERBOSE_PROCFS */

static const struct attribute_group *pcm_dev_attr_groups[];

/*
 * PM callbacks: we need to deal only with suspend here, as the resume is
 * triggered either from user-space or the driver's resume callback
 */
#ifdef CONFIG_PM_SLEEP
static int do_pcm_suspend(struct device *dev)
{
	struct snd_pcm_str *pstr = dev_get_drvdata(dev);

	if (!pstr->pcm->no_device_suspend)
		snd_pcm_suspend_all(pstr->pcm);
	return 0;
}
#endif

static const struct dev_pm_ops pcm_dev_pm_ops = {
	SET_SYSTEM_SLEEP_PM_OPS(do_pcm_suspend, NULL)
};

/* device type for PCM -- basically only for passing PM callbacks */
static const struct device_type pcm_dev_type = {
	.name = "pcm",
	.pm = &pcm_dev_pm_ops,
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * snd_pcm_new_stream - create a new PCM stream
 * @pcm: the pcm instance
 * @stream: the stream direction, SNDRV_PCM_STREAM_XXX
 * @substream_count: the number of substreams
 *
 * Creates a new stream for the pcm.
 * The corresponding stream on the pcm must have been empty before
 * calling this, i.e. zero must be given to the argument of
 * snd_pcm_new().
 *
<<<<<<< HEAD
 * Returns zero if successful, or a negative error code on failure.
=======
 * Return: Zero if successful, or a negative error code on failure.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
int snd_pcm_new_stream(struct snd_pcm *pcm, int stream, int substream_count)
{
	int idx, err;
	struct snd_pcm_str *pstr = &pcm->streams[stream];
	struct snd_pcm_substream *substream, *prev;

<<<<<<< HEAD
#if defined(CONFIG_SND_PCM_OSS) || defined(CONFIG_SND_PCM_OSS_MODULE)
=======
#if IS_ENABLED(CONFIG_SND_PCM_OSS)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mutex_init(&pstr->oss.setup_mutex);
#endif
	pstr->stream = stream;
	pstr->pcm = pcm;
	pstr->substream_count = substream_count;
<<<<<<< HEAD
	if (substream_count > 0 && !pcm->internal) {
		err = snd_pcm_stream_proc_init(pstr);
		if (err < 0) {
			snd_printk(KERN_ERR "Error in snd_pcm_stream_proc_init\n");
=======
	if (!substream_count)
		return 0;

	err = snd_device_alloc(&pstr->dev, pcm->card);
	if (err < 0)
		return err;
	dev_set_name(pstr->dev, "pcmC%iD%i%c", pcm->card->number, pcm->device,
		     stream == SNDRV_PCM_STREAM_PLAYBACK ? 'p' : 'c');
	pstr->dev->groups = pcm_dev_attr_groups;
	pstr->dev->type = &pcm_dev_type;
	dev_set_drvdata(pstr->dev, pstr);

	if (!pcm->internal) {
		err = snd_pcm_stream_proc_init(pstr);
		if (err < 0) {
			pcm_err(pcm, "Error in snd_pcm_stream_proc_init\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return err;
		}
	}
	prev = NULL;
	for (idx = 0, prev = NULL; idx < substream_count; idx++) {
		substream = kzalloc(sizeof(*substream), GFP_KERNEL);
<<<<<<< HEAD
		if (substream == NULL) {
			snd_printk(KERN_ERR "Cannot allocate PCM substream\n");
			return -ENOMEM;
		}
=======
		if (!substream)
			return -ENOMEM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		substream->pcm = pcm;
		substream->pstr = pstr;
		substream->number = idx;
		substream->stream = stream;
		sprintf(substream->name, "subdevice #%i", idx);
		substream->buffer_bytes_max = UINT_MAX;
		if (prev == NULL)
			pstr->substream = substream;
		else
			prev->next = substream;

		if (!pcm->internal) {
			err = snd_pcm_substream_proc_init(substream);
			if (err < 0) {
<<<<<<< HEAD
				snd_printk(KERN_ERR "Error in snd_pcm_stream_proc_init\n");
=======
				pcm_err(pcm,
					"Error in snd_pcm_stream_proc_init\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				if (prev == NULL)
					pstr->substream = NULL;
				else
					prev->next = NULL;
				kfree(substream);
				return err;
			}
		}
		substream->group = &substream->self_group;
<<<<<<< HEAD
		spin_lock_init(&substream->self_group.lock);
		INIT_LIST_HEAD(&substream->self_group.substreams);
=======
		snd_pcm_group_init(&substream->self_group);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		list_add_tail(&substream->link_list, &substream->self_group.substreams);
		atomic_set(&substream->mmap_count, 0);
		prev = substream;
	}
	return 0;
}				
<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
EXPORT_SYMBOL(snd_pcm_new_stream);

static int _snd_pcm_new(struct snd_card *card, const char *id, int device,
		int playback_count, int capture_count, bool internal,
		struct snd_pcm **rpcm)
{
	struct snd_pcm *pcm;
	int err;
<<<<<<< HEAD
	static struct snd_device_ops ops = {
=======
	static const struct snd_device_ops ops = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.dev_free = snd_pcm_dev_free,
		.dev_register =	snd_pcm_dev_register,
		.dev_disconnect = snd_pcm_dev_disconnect,
	};
<<<<<<< HEAD
=======
	static const struct snd_device_ops internal_ops = {
		.dev_free = snd_pcm_dev_free,
	};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (snd_BUG_ON(!card))
		return -ENXIO;
	if (rpcm)
		*rpcm = NULL;
	pcm = kzalloc(sizeof(*pcm), GFP_KERNEL);
<<<<<<< HEAD
	if (pcm == NULL) {
		snd_printk(KERN_ERR "Cannot allocate PCM\n");
		return -ENOMEM;
	}
	pcm->card = card;
	pcm->device = device;
	pcm->internal = internal;
	if (id)
		strlcpy(pcm->id, id, sizeof(pcm->id));
	if ((err = snd_pcm_new_stream(pcm, SNDRV_PCM_STREAM_PLAYBACK, playback_count)) < 0) {
		snd_pcm_free(pcm);
		return err;
	}
	if ((err = snd_pcm_new_stream(pcm, SNDRV_PCM_STREAM_CAPTURE, capture_count)) < 0) {
		snd_pcm_free(pcm);
		return err;
	}
	mutex_init(&pcm->open_mutex);
	init_waitqueue_head(&pcm->open_wait);
	if ((err = snd_device_new(card, SNDRV_DEV_PCM, pcm, &ops)) < 0) {
		snd_pcm_free(pcm);
		return err;
	}
	if (rpcm)
		*rpcm = pcm;
	return 0;
=======
	if (!pcm)
		return -ENOMEM;
	pcm->card = card;
	pcm->device = device;
	pcm->internal = internal;
	mutex_init(&pcm->open_mutex);
	init_waitqueue_head(&pcm->open_wait);
	INIT_LIST_HEAD(&pcm->list);
	if (id)
		strscpy(pcm->id, id, sizeof(pcm->id));

	err = snd_pcm_new_stream(pcm, SNDRV_PCM_STREAM_PLAYBACK,
				 playback_count);
	if (err < 0)
		goto free_pcm;

	err = snd_pcm_new_stream(pcm, SNDRV_PCM_STREAM_CAPTURE, capture_count);
	if (err < 0)
		goto free_pcm;

	err = snd_device_new(card, SNDRV_DEV_PCM, pcm,
			     internal ? &internal_ops : &ops);
	if (err < 0)
		goto free_pcm;

	if (rpcm)
		*rpcm = pcm;
	return 0;

free_pcm:
	snd_pcm_free(pcm);
	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * snd_pcm_new - create a new PCM instance
 * @card: the card instance
 * @id: the id string
 * @device: the device index (zero based)
 * @playback_count: the number of substreams for playback
 * @capture_count: the number of substreams for capture
 * @rpcm: the pointer to store the new pcm instance
 *
 * Creates a new PCM instance.
 *
 * The pcm operators have to be set afterwards to the new instance
 * via snd_pcm_set_ops().
 *
<<<<<<< HEAD
 * Returns zero if successful, or a negative error code on failure.
=======
 * Return: Zero if successful, or a negative error code on failure.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
int snd_pcm_new(struct snd_card *card, const char *id, int device,
		int playback_count, int capture_count, struct snd_pcm **rpcm)
{
	return _snd_pcm_new(card, id, device, playback_count, capture_count,
			false, rpcm);
}
EXPORT_SYMBOL(snd_pcm_new);

<<<<<<< HEAD
static int snd_pcm_new_stream_soc_be(struct snd_pcm *pcm, int stream,
	int substream_count)
{
	int idx;
	struct snd_pcm_str *pstr = &pcm->streams[stream];
	struct snd_pcm_substream *substream, *prev;

	pstr->stream = stream;
	pstr->pcm = pcm;
	pstr->substream_count = substream_count;

	prev = NULL;
	for (idx = 0, prev = NULL; idx < substream_count; idx++) {
		substream = kzalloc(sizeof(*substream), GFP_KERNEL);
		if (substream == NULL) {
			snd_printk(KERN_ERR "Cannot allocate BE PCM substream\n");
			return -ENOMEM;
		}
		substream->pcm = pcm;
		substream->pstr = pstr;
		substream->number = idx;
		substream->stream = stream;
		sprintf(substream->name, "subdevice #%i", idx);
		substream->buffer_bytes_max = UINT_MAX;
		if (prev == NULL)
			pstr->substream = substream;
		else
			prev->next = substream;

		substream->group = &substream->self_group;
		spin_lock_init(&substream->self_group.lock);
		INIT_LIST_HEAD(&substream->self_group.substreams);
		list_add_tail(&substream->link_list, &substream->self_group.substreams);
		atomic_set(&substream->mmap_count, 0);
		prev = substream;
	}
	return 0;
}

/**
 * snd_pcm_new_soc_be - create a new PCM instance for ASoC BE DAI link
 * @card: the card instance
 * @id: the id string
 * @device: the device index (zero based - shared with normal PCMs)
 * @playback_count: the number of substreams for playback
 * @capture_count: the number of substreams for capture
 * @rpcm: the pointer to store the new pcm instance
 *
 * Creates a new PCM instance with no userspace device or procfs entries.
 * This is used by ASoC Back End PCMs in order to create a PCM that will only
 * be used internally by kernel drivers. i.e. it cannot be opened by userspace.
 * It also provides existing ASoC components drivers with a substream and
 * access to any private data.
 *
 * The pcm operators have to be set afterwards to the new instance
 * via snd_pcm_set_ops().
 *
 * Returns zero if successful, or a negative error code on failure.
 */
int snd_pcm_new_soc_be(struct snd_card *card, const char *id, int device,
	int playback_count, int capture_count,
	struct snd_pcm ** rpcm)
{
	struct snd_pcm *pcm;
	int err;
	static struct snd_device_ops ops = {
		.dev_free = snd_pcm_dev_free,
		.dev_register =	snd_pcm_dev_register_soc_be,
		.dev_disconnect = snd_pcm_dev_disconnect,
	};

	if (snd_BUG_ON(!card))
		return -ENXIO;
	if (rpcm)
		*rpcm = NULL;
	pcm = kzalloc(sizeof(*pcm), GFP_KERNEL);
	if (pcm == NULL) {
		snd_printk(KERN_ERR "Cannot allocate virtual PCM\n");
		return -ENOMEM;
	}
	pcm->card = card;
	pcm->device = device;

	if (id)
		strlcpy(pcm->id, id, sizeof(pcm->id));
	if ((err = snd_pcm_new_stream_soc_be(pcm, SNDRV_PCM_STREAM_PLAYBACK, playback_count)) < 0) {
		snd_pcm_free(pcm);
		return err;
	}
	if ((err = snd_pcm_new_stream_soc_be(pcm, SNDRV_PCM_STREAM_CAPTURE, capture_count)) < 0) {
		snd_pcm_free(pcm);
		return err;
	}

	if ((err = snd_device_new(card, SNDRV_DEV_PCM, pcm, &ops)) < 0) {
		snd_pcm_free(pcm);
		return err;
	}
	if (rpcm)
		*rpcm = pcm;
	return 0;
}

EXPORT_SYMBOL(snd_pcm_new_soc_be);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * snd_pcm_new_internal - create a new internal PCM instance
 * @card: the card instance
 * @id: the id string
 * @device: the device index (zero based - shared with normal PCMs)
 * @playback_count: the number of substreams for playback
 * @capture_count: the number of substreams for capture
 * @rpcm: the pointer to store the new pcm instance
 *
 * Creates a new internal PCM instance with no userspace device or procfs
 * entries. This is used by ASoC Back End PCMs in order to create a PCM that
 * will only be used internally by kernel drivers. i.e. it cannot be opened
 * by userspace. It provides existing ASoC components drivers with a substream
 * and access to any private data.
 *
 * The pcm operators have to be set afterwards to the new instance
 * via snd_pcm_set_ops().
 *
<<<<<<< HEAD
 * Returns zero if successful, or a negative error code on failure.
=======
 * Return: Zero if successful, or a negative error code on failure.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
int snd_pcm_new_internal(struct snd_card *card, const char *id, int device,
	int playback_count, int capture_count,
	struct snd_pcm **rpcm)
{
	return _snd_pcm_new(card, id, device, playback_count, capture_count,
			true, rpcm);
}
EXPORT_SYMBOL(snd_pcm_new_internal);

<<<<<<< HEAD
static void snd_pcm_free_stream(struct snd_pcm_str * pstr)
{
	struct snd_pcm_substream *substream, *substream_next;
#if defined(CONFIG_SND_PCM_OSS) || defined(CONFIG_SND_PCM_OSS_MODULE)
	struct snd_pcm_oss_setup *setup, *setupn;
#endif
=======
static void free_chmap(struct snd_pcm_str *pstr)
{
	if (pstr->chmap_kctl) {
		struct snd_card *card = pstr->pcm->card;

		snd_ctl_remove(card, pstr->chmap_kctl);
		pstr->chmap_kctl = NULL;
	}
}

static void snd_pcm_free_stream(struct snd_pcm_str * pstr)
{
	struct snd_pcm_substream *substream, *substream_next;
#if IS_ENABLED(CONFIG_SND_PCM_OSS)
	struct snd_pcm_oss_setup *setup, *setupn;
#endif

	/* free all proc files under the stream */
	snd_pcm_stream_proc_done(pstr);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	substream = pstr->substream;
	while (substream) {
		substream_next = substream->next;
		snd_pcm_timer_done(substream);
<<<<<<< HEAD
		snd_pcm_substream_proc_done(substream);
		kfree(substream);
		substream = substream_next;
	}
	snd_pcm_stream_proc_done(pstr);
#if defined(CONFIG_SND_PCM_OSS) || defined(CONFIG_SND_PCM_OSS_MODULE)
=======
		kfree(substream);
		substream = substream_next;
	}
#if IS_ENABLED(CONFIG_SND_PCM_OSS)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	for (setup = pstr->oss.setup_list; setup; setup = setupn) {
		setupn = setup->next;
		kfree(setup->task_name);
		kfree(setup);
	}
#endif
<<<<<<< HEAD
}

static int snd_pcm_free(struct snd_pcm *pcm)
{
	struct snd_pcm_notify *notify;

	if (!pcm)
		return 0;
	list_for_each_entry(notify, &snd_pcm_notify_list, list) {
		notify->n_unregister(pcm);
	}
=======
	free_chmap(pstr);
	if (pstr->substream_count)
		put_device(pstr->dev);
}

#if IS_ENABLED(CONFIG_SND_PCM_OSS)
#define pcm_call_notify(pcm, call)					\
	do {								\
		struct snd_pcm_notify *_notify;				\
		list_for_each_entry(_notify, &snd_pcm_notify_list, list) \
			_notify->call(pcm);				\
	} while (0)
#else
#define pcm_call_notify(pcm, call) do {} while (0)
#endif

static int snd_pcm_free(struct snd_pcm *pcm)
{
	if (!pcm)
		return 0;
	if (!pcm->internal)
		pcm_call_notify(pcm, n_unregister);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (pcm->private_free)
		pcm->private_free(pcm);
	snd_pcm_lib_preallocate_free_for_all(pcm);
	snd_pcm_free_stream(&pcm->streams[SNDRV_PCM_STREAM_PLAYBACK]);
	snd_pcm_free_stream(&pcm->streams[SNDRV_PCM_STREAM_CAPTURE]);
	kfree(pcm);
	return 0;
}

static int snd_pcm_dev_free(struct snd_device *device)
{
	struct snd_pcm *pcm = device->device_data;
	return snd_pcm_free(pcm);
}

int snd_pcm_attach_substream(struct snd_pcm *pcm, int stream,
			     struct file *file,
			     struct snd_pcm_substream **rsubstream)
{
	struct snd_pcm_str * pstr;
	struct snd_pcm_substream *substream;
	struct snd_pcm_runtime *runtime;
<<<<<<< HEAD
	struct snd_ctl_file *kctl;
	struct snd_card *card;
	int prefer_subdevice = -1;
=======
	struct snd_card *card;
	int prefer_subdevice;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	size_t size;

	if (snd_BUG_ON(!pcm || !rsubstream))
		return -ENXIO;
<<<<<<< HEAD
=======
	if (snd_BUG_ON(stream != SNDRV_PCM_STREAM_PLAYBACK &&
		       stream != SNDRV_PCM_STREAM_CAPTURE))
		return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	*rsubstream = NULL;
	pstr = &pcm->streams[stream];
	if (pstr->substream == NULL || pstr->substream_count == 0)
		return -ENODEV;

	card = pcm->card;
<<<<<<< HEAD
	read_lock(&card->ctl_files_rwlock);
	list_for_each_entry(kctl, &card->ctl_files, list) {
		if (kctl->pid == task_pid(current)) {
			prefer_subdevice = kctl->prefer_pcm_subdevice;
			if (prefer_subdevice != -1)
				break;
		}
	}
	read_unlock(&card->ctl_files_rwlock);

	switch (stream) {
	case SNDRV_PCM_STREAM_PLAYBACK:
		if (pcm->info_flags & SNDRV_PCM_INFO_HALF_DUPLEX) {
			for (substream = pcm->streams[SNDRV_PCM_STREAM_CAPTURE].substream; substream; substream = substream->next) {
				if (SUBSTREAM_BUSY(substream))
					return -EAGAIN;
			}
		}
		break;
	case SNDRV_PCM_STREAM_CAPTURE:
		if (pcm->info_flags & SNDRV_PCM_INFO_HALF_DUPLEX) {
			for (substream = pcm->streams[SNDRV_PCM_STREAM_PLAYBACK].substream; substream; substream = substream->next) {
				if (SUBSTREAM_BUSY(substream))
					return -EAGAIN;
			}
		}
		break;
	default:
		return -EINVAL;
=======
	prefer_subdevice = snd_ctl_get_preferred_subdevice(card, SND_CTL_SUBDEV_PCM);

	if (pcm->info_flags & SNDRV_PCM_INFO_HALF_DUPLEX) {
		int opposite = !stream;

		for (substream = pcm->streams[opposite].substream; substream;
		     substream = substream->next) {
			if (SUBSTREAM_BUSY(substream))
				return -EAGAIN;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (file->f_flags & O_APPEND) {
		if (prefer_subdevice < 0) {
			if (pstr->substream_count > 1)
				return -EINVAL; /* must be unique */
			substream = pstr->substream;
		} else {
			for (substream = pstr->substream; substream;
			     substream = substream->next)
				if (substream->number == prefer_subdevice)
					break;
		}
		if (! substream)
			return -ENODEV;
		if (! SUBSTREAM_BUSY(substream))
			return -EBADFD;
		substream->ref_count++;
		*rsubstream = substream;
		return 0;
	}

<<<<<<< HEAD
	if (prefer_subdevice >= 0) {
		for (substream = pstr->substream; substream; substream = substream->next)
			if (!SUBSTREAM_BUSY(substream) && substream->number == prefer_subdevice)
				goto __ok;
	}
	for (substream = pstr->substream; substream; substream = substream->next)
		if (!SUBSTREAM_BUSY(substream))
			break;
      __ok:
=======
	for (substream = pstr->substream; substream; substream = substream->next) {
		if (!SUBSTREAM_BUSY(substream) &&
		    (prefer_subdevice == -1 ||
		     substream->number == prefer_subdevice))
			break;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (substream == NULL)
		return -EAGAIN;

	runtime = kzalloc(sizeof(*runtime), GFP_KERNEL);
	if (runtime == NULL)
		return -ENOMEM;

	size = PAGE_ALIGN(sizeof(struct snd_pcm_mmap_status));
<<<<<<< HEAD
	runtime->status = snd_malloc_pages(size, GFP_KERNEL);
=======
	runtime->status = alloc_pages_exact(size, GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (runtime->status == NULL) {
		kfree(runtime);
		return -ENOMEM;
	}
<<<<<<< HEAD
	memset((void*)runtime->status, 0, size);

	size = PAGE_ALIGN(sizeof(struct snd_pcm_mmap_control));
	runtime->control = snd_malloc_pages(size, GFP_KERNEL);
	if (runtime->control == NULL) {
		snd_free_pages((void*)runtime->status,
=======
	memset(runtime->status, 0, size);

	size = PAGE_ALIGN(sizeof(struct snd_pcm_mmap_control));
	runtime->control = alloc_pages_exact(size, GFP_KERNEL);
	if (runtime->control == NULL) {
		free_pages_exact(runtime->status,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			       PAGE_ALIGN(sizeof(struct snd_pcm_mmap_status)));
		kfree(runtime);
		return -ENOMEM;
	}
<<<<<<< HEAD
	memset((void*)runtime->control, 0, size);
=======
	memset(runtime->control, 0, size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	init_waitqueue_head(&runtime->sleep);
	init_waitqueue_head(&runtime->tsleep);

<<<<<<< HEAD
	runtime->status->state = SNDRV_PCM_STATE_OPEN;
=======
	__snd_pcm_set_state(runtime, SNDRV_PCM_STATE_OPEN);
	mutex_init(&runtime->buffer_mutex);
	atomic_set(&runtime->buffer_accessing, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	substream->runtime = runtime;
	substream->private_data = pcm->private_data;
	substream->ref_count = 1;
	substream->f_flags = file->f_flags;
	substream->pid = get_pid(task_pid(current));
	pstr->substream_opened++;
	*rsubstream = substream;
	return 0;
}

void snd_pcm_detach_substream(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime;

	if (PCM_RUNTIME_CHECK(substream))
		return;
	runtime = substream->runtime;
	if (runtime->private_free != NULL)
		runtime->private_free(runtime);
<<<<<<< HEAD
	snd_free_pages((void*)runtime->status,
		       PAGE_ALIGN(sizeof(struct snd_pcm_mmap_status)));
	snd_free_pages((void*)runtime->control,
		       PAGE_ALIGN(sizeof(struct snd_pcm_mmap_control)));
	kfree(runtime->hw_constraints.rules);
#ifdef CONFIG_SND_PCM_XRUN_DEBUG
	if (runtime->hwptr_log)
		kfree(runtime->hwptr_log);
#endif
	kfree(runtime);
	substream->runtime = NULL;
=======
	free_pages_exact(runtime->status,
		       PAGE_ALIGN(sizeof(struct snd_pcm_mmap_status)));
	free_pages_exact(runtime->control,
		       PAGE_ALIGN(sizeof(struct snd_pcm_mmap_control)));
	kfree(runtime->hw_constraints.rules);
	/* Avoid concurrent access to runtime via PCM timer interface */
	if (substream->timer) {
		scoped_guard(spinlock_irq, &substream->timer->lock)
			substream->runtime = NULL;
	} else {
		substream->runtime = NULL;
	}
	mutex_destroy(&runtime->buffer_mutex);
	snd_fasync_free(runtime->fasync);
	kfree(runtime);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	put_pid(substream->pid);
	substream->pid = NULL;
	substream->pstr->substream_opened--;
}

<<<<<<< HEAD
static ssize_t show_pcm_class(struct device *dev,
			      struct device_attribute *attr, char *buf)
{
	struct snd_pcm *pcm;
=======
static ssize_t pcm_class_show(struct device *dev,
			      struct device_attribute *attr, char *buf)
{
	struct snd_pcm_str *pstr = dev_get_drvdata(dev);
	struct snd_pcm *pcm = pstr->pcm;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	const char *str;
	static const char *strs[SNDRV_PCM_CLASS_LAST + 1] = {
		[SNDRV_PCM_CLASS_GENERIC] = "generic",
		[SNDRV_PCM_CLASS_MULTI] = "multi",
		[SNDRV_PCM_CLASS_MODEM] = "modem",
		[SNDRV_PCM_CLASS_DIGITIZER] = "digitizer",
	};

<<<<<<< HEAD
	if (! (pcm = dev_get_drvdata(dev)) ||
	    pcm->dev_class > SNDRV_PCM_CLASS_LAST)
		str = "none";
	else
		str = strs[pcm->dev_class];
        return snprintf(buf, PAGE_SIZE, "%s\n", str);
}

static struct device_attribute pcm_attrs =
	__ATTR(pcm_class, S_IRUGO, show_pcm_class, NULL);
=======
	if (pcm->dev_class > SNDRV_PCM_CLASS_LAST)
		str = "none";
	else
		str = strs[pcm->dev_class];
	return sysfs_emit(buf, "%s\n", str);
}

static DEVICE_ATTR_RO(pcm_class);
static struct attribute *pcm_dev_attrs[] = {
	&dev_attr_pcm_class.attr,
	NULL
};

static const struct attribute_group pcm_dev_attr_group = {
	.attrs	= pcm_dev_attrs,
};

static const struct attribute_group *pcm_dev_attr_groups[] = {
	&pcm_dev_attr_group,
	NULL
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int snd_pcm_dev_register(struct snd_device *device)
{
	int cidx, err;
	struct snd_pcm_substream *substream;
<<<<<<< HEAD
	struct snd_pcm_notify *notify;
	char str[16];
	struct snd_pcm *pcm;
	struct device *dev;
=======
	struct snd_pcm *pcm;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (snd_BUG_ON(!device || !device->device_data))
		return -ENXIO;
	pcm = device->device_data;
<<<<<<< HEAD
	mutex_lock(&register_mutex);
	err = snd_pcm_add(pcm);
	if (err) {
		mutex_unlock(&register_mutex);
		return err;
	}
	for (cidx = 0; cidx < 2; cidx++) {
		int devtype = -1;
		if (pcm->streams[cidx].substream == NULL || pcm->internal)
			continue;
		switch (cidx) {
		case SNDRV_PCM_STREAM_PLAYBACK:
			sprintf(str, "pcmC%iD%ip", pcm->card->number, pcm->device);
			devtype = SNDRV_DEVICE_TYPE_PCM_PLAYBACK;
			break;
		case SNDRV_PCM_STREAM_CAPTURE:
			sprintf(str, "pcmC%iD%ic", pcm->card->number, pcm->device);
			devtype = SNDRV_DEVICE_TYPE_PCM_CAPTURE;
			break;
		}
		/* device pointer to use, pcm->dev takes precedence if
		 * it is assigned, otherwise fall back to card's device
		 * if possible */
		dev = pcm->dev;
		if (!dev)
			dev = snd_card_get_device_link(pcm->card);
		/* register pcm */
		err = snd_register_device_for_dev(devtype, pcm->card,
						  pcm->device,
						  &snd_pcm_f_ops[cidx],
						  pcm, str, dev);
		if (err < 0) {
			list_del(&pcm->list);
			mutex_unlock(&register_mutex);
			return err;
		}
		snd_add_device_sysfs_file(devtype, pcm->card, pcm->device,
					  &pcm_attrs);
=======

	guard(mutex)(&register_mutex);
	err = snd_pcm_add(pcm);
	if (err)
		return err;
	for (cidx = 0; cidx < 2; cidx++) {
		int devtype = -1;
		if (pcm->streams[cidx].substream == NULL)
			continue;
		switch (cidx) {
		case SNDRV_PCM_STREAM_PLAYBACK:
			devtype = SNDRV_DEVICE_TYPE_PCM_PLAYBACK;
			break;
		case SNDRV_PCM_STREAM_CAPTURE:
			devtype = SNDRV_DEVICE_TYPE_PCM_CAPTURE;
			break;
		}
		/* register pcm */
		err = snd_register_device(devtype, pcm->card, pcm->device,
					  &snd_pcm_f_ops[cidx], pcm,
					  pcm->streams[cidx].dev);
		if (err < 0) {
			list_del_init(&pcm->list);
			return err;
		}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		for (substream = pcm->streams[cidx].substream; substream; substream = substream->next)
			snd_pcm_timer_init(substream);
	}

<<<<<<< HEAD
	list_for_each_entry(notify, &snd_pcm_notify_list, list)
		notify->n_register(pcm);

	mutex_unlock(&register_mutex);
	return 0;
}

static int snd_pcm_dev_register_soc_be(struct snd_device *device)
{
	int err;
	struct snd_pcm_notify *notify;
	struct snd_pcm *pcm;

	if (snd_BUG_ON(!device || !device->device_data))
		return -ENXIO;
	pcm = device->device_data;
	mutex_lock(&register_mutex);
	err = snd_pcm_add(pcm);
	if (err) {
		mutex_unlock(&register_mutex);
		return err;
	}

	list_for_each_entry(notify, &snd_pcm_notify_list, list)
		notify->n_register(pcm);

	mutex_unlock(&register_mutex);
	return 0;
=======
	pcm_call_notify(pcm, n_register);
	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int snd_pcm_dev_disconnect(struct snd_device *device)
{
	struct snd_pcm *pcm = device->device_data;
<<<<<<< HEAD
	struct snd_pcm_notify *notify;
	struct snd_pcm_substream *substream;
	int cidx, devtype;

	mutex_lock(&register_mutex);
	if (list_empty(&pcm->list))
		goto unlock;

	mutex_lock(&pcm->open_mutex);
	wake_up(&pcm->open_wait);
	list_del_init(&pcm->list);
	for (cidx = 0; cidx < 2; cidx++)
		for (substream = pcm->streams[cidx].substream; substream; substream = substream->next) {
			snd_pcm_stream_lock_irq(substream);
			if (substream->runtime) {
				substream->runtime->status->state = SNDRV_PCM_STATE_DISCONNECTED;
				wake_up(&substream->runtime->sleep);
				wake_up(&substream->runtime->tsleep);
			}
			snd_pcm_stream_unlock_irq(substream);
		}
	list_for_each_entry(notify, &snd_pcm_notify_list, list) {
		notify->n_disconnect(pcm);
	}
	for (cidx = 0; cidx < 2; cidx++) {
		devtype = -1;
		switch (cidx) {
		case SNDRV_PCM_STREAM_PLAYBACK:
			devtype = SNDRV_DEVICE_TYPE_PCM_PLAYBACK;
			break;
		case SNDRV_PCM_STREAM_CAPTURE:
			devtype = SNDRV_DEVICE_TYPE_PCM_CAPTURE;
			break;
		}
		snd_unregister_device(devtype, pcm->card, pcm->device);
		if (pcm->streams[cidx].chmap_kctl) {
			snd_ctl_remove(pcm->card, pcm->streams[cidx].chmap_kctl);
			pcm->streams[cidx].chmap_kctl = NULL;
		}
		if (pcm->streams[cidx].vol_kctl) {
			snd_ctl_remove(pcm->card, pcm->streams[cidx].vol_kctl);
			pcm->streams[cidx].vol_kctl = NULL;
		}
	}
	mutex_unlock(&pcm->open_mutex);
 unlock:
	mutex_unlock(&register_mutex);
	return 0;
}

=======
	struct snd_pcm_substream *substream;
	int cidx;

	guard(mutex)(&register_mutex);
	guard(mutex)(&pcm->open_mutex);
	wake_up(&pcm->open_wait);
	list_del_init(&pcm->list);

	for_each_pcm_substream(pcm, cidx, substream) {
		snd_pcm_stream_lock_irq(substream);
		if (substream->runtime) {
			if (snd_pcm_running(substream))
				snd_pcm_stop(substream, SNDRV_PCM_STATE_DISCONNECTED);
			/* to be sure, set the state unconditionally */
			__snd_pcm_set_state(substream->runtime,
					    SNDRV_PCM_STATE_DISCONNECTED);
			wake_up(&substream->runtime->sleep);
			wake_up(&substream->runtime->tsleep);
		}
		snd_pcm_stream_unlock_irq(substream);
	}

	for_each_pcm_substream(pcm, cidx, substream)
		snd_pcm_sync_stop(substream, false);

	pcm_call_notify(pcm, n_disconnect);
	for (cidx = 0; cidx < 2; cidx++) {
		if (pcm->streams[cidx].dev)
			snd_unregister_device(pcm->streams[cidx].dev);
		free_chmap(&pcm->streams[cidx]);
	}
	return 0;
}

#if IS_ENABLED(CONFIG_SND_PCM_OSS)
/**
 * snd_pcm_notify - Add/remove the notify list
 * @notify: PCM notify list
 * @nfree: 0 = register, 1 = unregister
 *
 * This adds the given notifier to the global list so that the callback is
 * called for each registered PCM devices.  This exists only for PCM OSS
 * emulation, so far.
 *
 * Return: zero if successful, or a negative error code
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int snd_pcm_notify(struct snd_pcm_notify *notify, int nfree)
{
	struct snd_pcm *pcm;

	if (snd_BUG_ON(!notify ||
		       !notify->n_register ||
		       !notify->n_unregister ||
		       !notify->n_disconnect))
		return -EINVAL;
<<<<<<< HEAD
	mutex_lock(&register_mutex);
=======
	guard(mutex)(&register_mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (nfree) {
		list_del(&notify->list);
		list_for_each_entry(pcm, &snd_pcm_devices, list)
			notify->n_unregister(pcm);
	} else {
		list_add_tail(&notify->list, &snd_pcm_notify_list);
		list_for_each_entry(pcm, &snd_pcm_devices, list)
			notify->n_register(pcm);
	}
<<<<<<< HEAD
	mutex_unlock(&register_mutex);
	return 0;
}

EXPORT_SYMBOL(snd_pcm_notify);

#ifdef CONFIG_PROC_FS
=======
	return 0;
}
EXPORT_SYMBOL(snd_pcm_notify);
#endif /* CONFIG_SND_PCM_OSS */

#ifdef CONFIG_SND_PROC_FS
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  Info interface
 */

static void snd_pcm_proc_read(struct snd_info_entry *entry,
			      struct snd_info_buffer *buffer)
{
	struct snd_pcm *pcm;

<<<<<<< HEAD
	mutex_lock(&register_mutex);
=======
	guard(mutex)(&register_mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	list_for_each_entry(pcm, &snd_pcm_devices, list) {
		snd_iprintf(buffer, "%02i-%02i: %s : %s",
			    pcm->card->number, pcm->device, pcm->id, pcm->name);
		if (pcm->streams[SNDRV_PCM_STREAM_PLAYBACK].substream)
			snd_iprintf(buffer, " : playback %i",
				    pcm->streams[SNDRV_PCM_STREAM_PLAYBACK].substream_count);
		if (pcm->streams[SNDRV_PCM_STREAM_CAPTURE].substream)
			snd_iprintf(buffer, " : capture %i",
				    pcm->streams[SNDRV_PCM_STREAM_CAPTURE].substream_count);
		snd_iprintf(buffer, "\n");
	}
<<<<<<< HEAD
	mutex_unlock(&register_mutex);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct snd_info_entry *snd_pcm_proc_entry;

static void snd_pcm_proc_init(void)
{
	struct snd_info_entry *entry;

<<<<<<< HEAD
	if ((entry = snd_info_create_module_entry(THIS_MODULE, "pcm", NULL)) != NULL) {
=======
	entry = snd_info_create_module_entry(THIS_MODULE, "pcm", NULL);
	if (entry) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		snd_info_set_text_ops(entry, NULL, snd_pcm_proc_read);
		if (snd_info_register(entry) < 0) {
			snd_info_free_entry(entry);
			entry = NULL;
		}
	}
	snd_pcm_proc_entry = entry;
}

static void snd_pcm_proc_done(void)
{
	snd_info_free_entry(snd_pcm_proc_entry);
}

<<<<<<< HEAD
#else /* !CONFIG_PROC_FS */
#define snd_pcm_proc_init()
#define snd_pcm_proc_done()
#endif /* CONFIG_PROC_FS */
=======
#else /* !CONFIG_SND_PROC_FS */
#define snd_pcm_proc_init()
#define snd_pcm_proc_done()
#endif /* CONFIG_SND_PROC_FS */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)


/*
 *  ENTRY functions
 */

static int __init alsa_pcm_init(void)
{
	snd_ctl_register_ioctl(snd_pcm_control_ioctl);
	snd_ctl_register_ioctl_compat(snd_pcm_control_ioctl);
	snd_pcm_proc_init();
	return 0;
}

static void __exit alsa_pcm_exit(void)
{
	snd_ctl_unregister_ioctl(snd_pcm_control_ioctl);
	snd_ctl_unregister_ioctl_compat(snd_pcm_control_ioctl);
	snd_pcm_proc_done();
}

module_init(alsa_pcm_init)
module_exit(alsa_pcm_exit)
