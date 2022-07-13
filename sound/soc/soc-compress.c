<<<<<<< HEAD
/*
 * soc-compress.c  --  ALSA SoC Compress
 *
 * Copyright (C) 2012 Intel Corp.
 *
 * Authors: Namarta Kohli <namartax.kohli@intel.com>
 *          Ramesh Babu K V <ramesh.babu@linux.intel.com>
 *          Vinod Koul <vinod.koul@linux.intel.com>
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
 */
=======
// SPDX-License-Identifier: GPL-2.0+
//
// soc-compress.c  --  ALSA SoC Compress
//
// Copyright (C) 2012 Intel Corp.
//
// Authors: Namarta Kohli <namartax.kohli@intel.com>
//          Ramesh Babu K V <ramesh.babu@linux.intel.com>
//          Vinod Koul <vinod.koul@linux.intel.com>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/workqueue.h>
#include <sound/core.h>
#include <sound/compress_params.h>
#include <sound/compress_driver.h>
#include <sound/soc.h>
#include <sound/initval.h>
#include <sound/soc-dpcm.h>
<<<<<<< HEAD
=======
#include <sound/soc-link.h>

static int snd_soc_compr_components_open(struct snd_compr_stream *cstream)
{
	struct snd_soc_pcm_runtime *rtd = cstream->private_data;
	struct snd_soc_component *component;
	int ret = 0;
	int i;

	for_each_rtd_components(rtd, i, component) {
		ret = snd_soc_component_module_get_when_open(component, cstream);
		if (ret < 0)
			break;

		ret = snd_soc_component_compr_open(component, cstream);
		if (ret < 0)
			break;
	}

	return ret;
}

static void snd_soc_compr_components_free(struct snd_compr_stream *cstream,
					  int rollback)
{
	struct snd_soc_pcm_runtime *rtd = cstream->private_data;
	struct snd_soc_component *component;
	int i;

	for_each_rtd_components(rtd, i, component) {
		snd_soc_component_compr_free(component, cstream, rollback);
		snd_soc_component_module_put_when_close(component, cstream, rollback);
	}
}

static int soc_compr_clean(struct snd_compr_stream *cstream, int rollback)
{
	struct snd_soc_pcm_runtime *rtd = cstream->private_data;
	struct snd_soc_dai *cpu_dai = snd_soc_rtd_to_cpu(rtd, 0);
	struct snd_soc_dai *codec_dai = snd_soc_rtd_to_codec(rtd, 0);
	int stream = cstream->direction; /* SND_COMPRESS_xxx is same as SNDRV_PCM_STREAM_xxx */

	snd_soc_dpcm_mutex_lock(rtd);

	if (!rollback)
		snd_soc_runtime_deactivate(rtd, stream);

	snd_soc_dai_digital_mute(codec_dai, 1, stream);

	if (!snd_soc_dai_active(cpu_dai))
		cpu_dai->rate = 0;

	if (!snd_soc_dai_active(codec_dai))
		codec_dai->rate = 0;

	snd_soc_link_compr_shutdown(cstream, rollback);

	snd_soc_compr_components_free(cstream, rollback);

	snd_soc_dai_compr_shutdown(cpu_dai, cstream, rollback);

	if (!rollback)
		snd_soc_dapm_stream_stop(rtd, stream);

	snd_soc_dpcm_mutex_unlock(rtd);

	snd_soc_pcm_component_pm_runtime_put(rtd, cstream, rollback);

	return 0;
}

static int soc_compr_free(struct snd_compr_stream *cstream)
{
	return soc_compr_clean(cstream, 0);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int soc_compr_open(struct snd_compr_stream *cstream)
{
	struct snd_soc_pcm_runtime *rtd = cstream->private_data;
<<<<<<< HEAD
	struct snd_soc_platform *platform = rtd->platform;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	struct snd_soc_dai *codec_dai = rtd->codec_dai;
	int ret = 0;

	mutex_lock_nested(&rtd->pcm_mutex, rtd->pcm_subclass);

	if (platform->driver->compr_ops && platform->driver->compr_ops->open) {
		ret = platform->driver->compr_ops->open(cstream);
		if (ret < 0) {
			pr_err("compress asoc: can't open platform %s\n", platform->name);
			goto out;
		}
	}

	if (rtd->dai_link->compr_ops && rtd->dai_link->compr_ops->startup) {
		ret = rtd->dai_link->compr_ops->startup(cstream);
		if (ret < 0) {
			pr_err("compress asoc: %s startup failed\n", rtd->dai_link->name);
			goto machine_err;
		}
	}

	if (cstream->direction == SND_COMPRESS_PLAYBACK) {
		cpu_dai->playback_active++;
		codec_dai->playback_active++;
	} else {
		cpu_dai->capture_active++;
		codec_dai->capture_active++;
	}

	cpu_dai->active++;
	codec_dai->active++;
	rtd->codec->active++;

	mutex_unlock(&rtd->pcm_mutex);

	return 0;

machine_err:
	if (platform->driver->compr_ops && platform->driver->compr_ops->free)
		platform->driver->compr_ops->free(cstream);
out:
	mutex_unlock(&rtd->pcm_mutex);
=======
	struct snd_soc_dai *cpu_dai = snd_soc_rtd_to_cpu(rtd, 0);
	int stream = cstream->direction; /* SND_COMPRESS_xxx is same as SNDRV_PCM_STREAM_xxx */
	int ret;

	ret = snd_soc_pcm_component_pm_runtime_get(rtd, cstream);
	if (ret < 0)
		goto err_no_lock;

	snd_soc_dpcm_mutex_lock(rtd);

	ret = snd_soc_dai_compr_startup(cpu_dai, cstream);
	if (ret < 0)
		goto err;

	ret = snd_soc_compr_components_open(cstream);
	if (ret < 0)
		goto err;

	ret = snd_soc_link_compr_startup(cstream);
	if (ret < 0)
		goto err;

	snd_soc_runtime_activate(rtd, stream);
err:
	snd_soc_dpcm_mutex_unlock(rtd);
err_no_lock:
	if (ret < 0)
		soc_compr_clean(cstream, 1);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

static int soc_compr_open_fe(struct snd_compr_stream *cstream)
{
	struct snd_soc_pcm_runtime *fe = cstream->private_data;
<<<<<<< HEAD
	struct snd_pcm_substream *fe_substream = fe->pcm->streams[0].substream;
	struct snd_soc_platform *platform = fe->platform;
	struct snd_soc_dpcm_params *dpcm;
	struct snd_soc_dapm_widget_list *list;

	struct snd_soc_dai *cpu_dai = fe->cpu_dai;
	struct snd_soc_dai *codec_dai = fe->codec_dai;

	int stream;
	int ret = 0;

	if (cstream->direction == SND_COMPRESS_PLAYBACK)
		stream = SNDRV_PCM_STREAM_PLAYBACK;
	else
		stream = SNDRV_PCM_STREAM_CAPTURE;

	mutex_lock(&fe->card->dpcm_mutex);

	if (platform->driver->compr_ops && platform->driver->compr_ops->open) {
		ret = platform->driver->compr_ops->open(cstream);
		if (ret < 0) {
			pr_err("compress asoc: can't open platform %s\n", platform->name);
			goto out;
		}
	}

	if (fe->dai_link->compr_ops && fe->dai_link->compr_ops->startup) {
		ret = fe->dai_link->compr_ops->startup(cstream);
		if (ret < 0) {
			pr_err("compress asoc: %s startup failed\n", fe->dai_link->name);
			goto machine_err;
		}
	}

	fe->dpcm[stream].runtime = fe_substream->runtime;

	if (dpcm_path_get(fe, stream, &list) <= 0) {
		dev_dbg(fe->dev, "ASoC: %s no valid %s route\n",
			fe->dai_link->name, stream ? "capture" : "playback");
	}
=======
	struct snd_soc_dai *cpu_dai = snd_soc_rtd_to_cpu(fe, 0);
	struct snd_soc_dpcm *dpcm;
	struct snd_soc_dapm_widget_list *list;
	int stream = cstream->direction; /* SND_COMPRESS_xxx is same as SNDRV_PCM_STREAM_xxx */
	int ret;

	snd_soc_card_mutex_lock(fe->card);

	ret = dpcm_path_get(fe, stream, &list);
	if (ret < 0)
		goto be_err;

	snd_soc_dpcm_mutex_lock(fe);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* calculate valid and active FE <-> BE dpcms */
	dpcm_process_paths(fe, stream, &list, 1);

	fe->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_FE;

	ret = dpcm_be_dai_startup(fe, stream);
	if (ret < 0) {
		/* clean up all links */
<<<<<<< HEAD
		list_for_each_entry(dpcm, &fe->dpcm[stream].be_clients, list_be)
			dpcm->state = SND_SOC_DPCM_LINK_STATE_FREE;

		dpcm_be_disconnect(fe, stream);
		fe->dpcm[stream].runtime = NULL;
		goto fe_err;
	}

=======
		for_each_dpcm_be(fe, stream, dpcm)
			dpcm->state = SND_SOC_DPCM_LINK_STATE_FREE;

		dpcm_be_disconnect(fe, stream);
		goto out;
	}

	ret = snd_soc_dai_compr_startup(cpu_dai, cstream);
	if (ret < 0)
		goto out;

	ret = snd_soc_compr_components_open(cstream);
	if (ret < 0)
		goto open_err;

	ret = snd_soc_link_compr_startup(cstream);
	if (ret < 0)
		goto machine_err;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dpcm_clear_pending_state(fe, stream);
	dpcm_path_put(&list);

	fe->dpcm[stream].state = SND_SOC_DPCM_STATE_OPEN;
	fe->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_NO;

<<<<<<< HEAD
	if (cstream->direction == SND_COMPRESS_PLAYBACK) {
		cpu_dai->playback_active++;
		codec_dai->playback_active++;
	} else {
		cpu_dai->capture_active++;
		codec_dai->capture_active++;
	}

	cpu_dai->active++;
	codec_dai->active++;
	fe->codec->active++;

	mutex_unlock(&fe->card->dpcm_mutex);

	return 0;

fe_err:
	if (fe->dai_link->compr_ops && fe->dai_link->compr_ops->shutdown)
		fe->dai_link->compr_ops->shutdown(cstream);
machine_err:
	if (platform->driver->compr_ops && platform->driver->compr_ops->free)
		platform->driver->compr_ops->free(cstream);
out:
	fe->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_NO;
	mutex_unlock(&fe->card->dpcm_mutex);
	return ret;
}

/*
 * Power down the audio subsystem pmdown_time msecs after close is called.
 * This is to ensure there are no pops or clicks in between any music tracks
 * due to DAPM power cycling.
 */
static void close_delayed_work(struct work_struct *work)
{
	struct snd_soc_pcm_runtime *rtd =
			container_of(work, struct snd_soc_pcm_runtime, delayed_work.work);
	struct snd_soc_dai *codec_dai = rtd->codec_dai;

	mutex_lock_nested(&rtd->pcm_mutex, rtd->pcm_subclass);

	dev_dbg(rtd->dev, "ASoC: pop wq checking: %s status: %s waiting: %s\n",
		 codec_dai->driver->playback.stream_name,
		 codec_dai->playback_active ? "active" : "inactive",
		     codec_dai->pop_wait ? "yes" : "no");

	/* are we waiting on this codec DAI stream */
	if (codec_dai->pop_wait == 1) {
	  codec_dai->pop_wait = 0;
		snd_soc_dapm_stream_event(rtd, SNDRV_PCM_STREAM_PLAYBACK,
					  SND_SOC_DAPM_STREAM_STOP);
	}

	mutex_unlock(&rtd->pcm_mutex);
}

static int soc_compr_free(struct snd_compr_stream *cstream)
{
#ifdef CONFIG_SND_SOC_WM5110
	struct snd_soc_pcm_runtime *rtd = cstream->private_data;
	struct snd_soc_platform *platform = rtd->platform;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	struct snd_soc_dai *codec_dai = rtd->codec_dai;
	struct snd_soc_codec *codec = rtd->codec;

	mutex_lock_nested(&rtd->pcm_mutex, rtd->pcm_subclass);

	if (cstream->direction == SND_COMPRESS_PLAYBACK) {
		cpu_dai->playback_active--;
		codec_dai->playback_active--;
		snd_soc_dai_digital_mute(codec_dai, 1);
	} else {
		cpu_dai->capture_active--;
		codec_dai->capture_active--;
	}

	cpu_dai->active--;
	codec_dai->active--;
	codec->active--;

	if (!cpu_dai->active)
		cpu_dai->rate = 0;

	if (!codec_dai->active)
		codec_dai->rate = 0;


	if (rtd->dai_link->compr_ops && rtd->dai_link->compr_ops->shutdown)
		rtd->dai_link->compr_ops->shutdown(cstream);

	if (platform->driver->compr_ops && platform->driver->compr_ops->free)
		platform->driver->compr_ops->free(cstream);
	cpu_dai->runtime = NULL;

	if (cstream->direction == SND_COMPRESS_PLAYBACK) {
		if (!rtd->pmdown_time || codec->ignore_pmdown_time ||
		    rtd->dai_link->ignore_pmdown_time) {
			snd_soc_dapm_stream_event(rtd,
					codec_dai->driver->playback.stream_name,
					SND_SOC_DAPM_STREAM_STOP);
		} else {
			codec_dai->pop_wait = 1;
			schedule_delayed_work(&rtd->delayed_work,
				msecs_to_jiffies(rtd->pmdown_time));
		}
	} else {
		/* capture streams can be powered down now */
		snd_soc_dapm_stream_event(rtd,
			codec_dai->driver->capture.stream_name,
			SND_SOC_DAPM_STREAM_STOP);
	}

	mutex_unlock(&rtd->pcm_mutex);
#endif
	return 0;
}

static int soc_compr_free_fe(struct snd_compr_stream *cstream)
{
	struct snd_soc_pcm_runtime *fe = cstream->private_data;
	struct snd_soc_platform *platform = fe->platform;
	struct snd_soc_dpcm_params *dpcm;
	int stream, ret;

	struct snd_soc_dai *cpu_dai = fe->cpu_dai;
	struct snd_soc_dai *codec_dai = fe->codec_dai;
	struct snd_soc_codec *codec = fe->codec;

	if (cstream->direction == SND_COMPRESS_PLAYBACK)
		stream = SNDRV_PCM_STREAM_PLAYBACK;
	else
		stream = SNDRV_PCM_STREAM_CAPTURE;

	mutex_lock(&fe->card->dpcm_mutex);
	if (cstream->direction == SND_COMPRESS_PLAYBACK) {
		cpu_dai->playback_active--;
		codec_dai->playback_active--;
		snd_soc_dai_digital_mute(codec_dai, 1);
	} else {
		cpu_dai->capture_active--;
		codec_dai->capture_active--;
	}

	cpu_dai->active--;
	codec_dai->active--;
	codec->active--;

	fe->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_FE;

	ret = dpcm_be_dai_hw_free(fe, stream);
	if (ret < 0)
		dev_err(fe->dev, "compressed hw_free failed %d\n", ret);

	ret = dpcm_be_dai_shutdown(fe, stream);

	/* mark FE's links ready to prune */
	list_for_each_entry(dpcm, &fe->dpcm[stream].be_clients, list_be)
		dpcm->state = SND_SOC_DPCM_LINK_STATE_FREE;

	if (stream == SNDRV_PCM_STREAM_PLAYBACK)
		dpcm_dapm_stream_event(fe, stream,
				fe->cpu_dai->driver->playback.stream_name,
				SND_SOC_DAPM_STREAM_STOP);
	else
		dpcm_dapm_stream_event(fe, stream,
				fe->cpu_dai->driver->capture.stream_name,
				SND_SOC_DAPM_STREAM_STOP);
=======
	snd_soc_runtime_activate(fe, stream);
	snd_soc_dpcm_mutex_unlock(fe);

	snd_soc_card_mutex_unlock(fe->card);

	return 0;

machine_err:
	snd_soc_compr_components_free(cstream, 1);
open_err:
	snd_soc_dai_compr_shutdown(cpu_dai, cstream, 1);
out:
	dpcm_path_put(&list);
	snd_soc_dpcm_mutex_unlock(fe);
be_err:
	fe->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_NO;
	snd_soc_card_mutex_unlock(fe->card);
	return ret;
}

static int soc_compr_free_fe(struct snd_compr_stream *cstream)
{
	struct snd_soc_pcm_runtime *fe = cstream->private_data;
	struct snd_soc_dai *cpu_dai = snd_soc_rtd_to_cpu(fe, 0);
	struct snd_soc_dpcm *dpcm;
	int stream = cstream->direction; /* SND_COMPRESS_xxx is same as SNDRV_PCM_STREAM_xxx */

	snd_soc_card_mutex_lock(fe->card);

	snd_soc_dpcm_mutex_lock(fe);
	snd_soc_runtime_deactivate(fe, stream);

	fe->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_FE;

	dpcm_be_dai_hw_free(fe, stream);

	dpcm_be_dai_shutdown(fe, stream);

	/* mark FE's links ready to prune */
	for_each_dpcm_be(fe, stream, dpcm)
		dpcm->state = SND_SOC_DPCM_LINK_STATE_FREE;

	dpcm_dapm_stream_event(fe, stream, SND_SOC_DAPM_STREAM_STOP);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	fe->dpcm[stream].state = SND_SOC_DPCM_STATE_CLOSE;
	fe->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_NO;

	dpcm_be_disconnect(fe, stream);

<<<<<<< HEAD
	fe->dpcm[stream].runtime = NULL;

	if (fe->dai_link->compr_ops && fe->dai_link->compr_ops->shutdown)
		fe->dai_link->compr_ops->shutdown(cstream);

	if (platform->driver->compr_ops && platform->driver->compr_ops->free)
		platform->driver->compr_ops->free(cstream);
	//cpu_dai->runtime = NULL;

	mutex_unlock(&fe->card->dpcm_mutex);
=======
	snd_soc_dpcm_mutex_unlock(fe);

	snd_soc_link_compr_shutdown(cstream, 0);

	snd_soc_compr_components_free(cstream, 0);

	snd_soc_dai_compr_shutdown(cpu_dai, cstream, 0);

	snd_soc_card_mutex_unlock(fe->card);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static int soc_compr_trigger(struct snd_compr_stream *cstream, int cmd)
{
<<<<<<< HEAD

	struct snd_soc_pcm_runtime *rtd = cstream->private_data;
	struct snd_soc_platform *platform = rtd->platform;
	struct snd_soc_dai *codec_dai = rtd->codec_dai;
	int ret = 0;

	/* for partial drain and drain cmd, don't acquire lock while invoking DSP.
	 * These calls will be blocked till these operation can complete which
	 * will be a while. And during that time, app can invoke STOP, PAUSE etc
	 */
	if (cmd == SND_COMPR_TRIGGER_PARTIAL_DRAIN ||
				cmd == SND_COMPR_TRIGGER_DRAIN) {
		if (platform->driver->compr_ops &&
					platform->driver->compr_ops->trigger)
			return platform->driver->compr_ops->trigger(cstream, cmd);
	}

	mutex_lock_nested(&rtd->pcm_mutex, rtd->pcm_subclass);

	if (platform->driver->compr_ops && platform->driver->compr_ops->trigger) {
		ret = platform->driver->compr_ops->trigger(cstream, cmd);
		if (ret < 0)
			goto out;
	}

	if (cstream->direction == SND_COMPRESS_PLAYBACK) {
		switch (cmd) {
		case SNDRV_PCM_TRIGGER_START:
			snd_soc_dai_digital_mute(codec_dai, 0);
			break;
		case SNDRV_PCM_TRIGGER_STOP:
			snd_soc_dai_digital_mute(codec_dai, 1);
			break;
		}
	}

out:
	mutex_unlock(&rtd->pcm_mutex);
=======
	struct snd_soc_pcm_runtime *rtd = cstream->private_data;
	struct snd_soc_dai *codec_dai = snd_soc_rtd_to_codec(rtd, 0);
	struct snd_soc_dai *cpu_dai = snd_soc_rtd_to_cpu(rtd, 0);
	int stream = cstream->direction; /* SND_COMPRESS_xxx is same as SNDRV_PCM_STREAM_xxx */
	int ret;

	snd_soc_dpcm_mutex_lock(rtd);

	ret = snd_soc_component_compr_trigger(cstream, cmd);
	if (ret < 0)
		goto out;

	ret = snd_soc_dai_compr_trigger(cpu_dai, cstream, cmd);
	if (ret < 0)
		goto out;

	switch (cmd) {
	case SNDRV_PCM_TRIGGER_START:
		snd_soc_dai_digital_mute(codec_dai, 0, stream);
		break;
	case SNDRV_PCM_TRIGGER_STOP:
		snd_soc_dai_digital_mute(codec_dai, 1, stream);
		break;
	}

out:
	snd_soc_dpcm_mutex_unlock(rtd);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

static int soc_compr_trigger_fe(struct snd_compr_stream *cstream, int cmd)
{
	struct snd_soc_pcm_runtime *fe = cstream->private_data;
<<<<<<< HEAD
	struct snd_soc_platform *platform = fe->platform;
	int ret = 0, stream;

	/* for partial drain and drain cmd, don't acquire lock while invoking DSP.
	 * These calls will be blocked till these operation can complete which
	 * will be a while. And during that time, app can invoke STOP, PAUSE etc
	 */
	if (cmd == SND_COMPR_TRIGGER_PARTIAL_DRAIN ||
				cmd == SND_COMPR_TRIGGER_DRAIN) {
		if (platform->driver->compr_ops &&
					platform->driver->compr_ops->trigger)
			return platform->driver->compr_ops->trigger(cstream, cmd);
	}

	if (cstream->direction == SND_COMPRESS_PLAYBACK)
		stream = SNDRV_PCM_STREAM_PLAYBACK;
	else
		stream = SNDRV_PCM_STREAM_CAPTURE;


	mutex_lock(&fe->card->dpcm_mutex);
	if (platform->driver->compr_ops && platform->driver->compr_ops->trigger) {
		ret = platform->driver->compr_ops->trigger(cstream, cmd);
		if (ret < 0)
			goto out;
	}
=======
	struct snd_soc_dai *cpu_dai = snd_soc_rtd_to_cpu(fe, 0);
	int stream = cstream->direction; /* SND_COMPRESS_xxx is same as SNDRV_PCM_STREAM_xxx */
	int ret;

	if (cmd == SND_COMPR_TRIGGER_PARTIAL_DRAIN ||
	    cmd == SND_COMPR_TRIGGER_DRAIN)
		return snd_soc_component_compr_trigger(cstream, cmd);

	snd_soc_card_mutex_lock(fe->card);

	ret = snd_soc_dai_compr_trigger(cpu_dai, cstream, cmd);
	if (ret < 0)
		goto out;

	ret = snd_soc_component_compr_trigger(cstream, cmd);
	if (ret < 0)
		goto out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	fe->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_FE;

	ret = dpcm_be_dai_trigger(fe, stream, cmd);

	switch (cmd) {
	case SNDRV_PCM_TRIGGER_START:
	case SNDRV_PCM_TRIGGER_RESUME:
	case SNDRV_PCM_TRIGGER_PAUSE_RELEASE:
		fe->dpcm[stream].state = SND_SOC_DPCM_STATE_START;
		break;
	case SNDRV_PCM_TRIGGER_STOP:
	case SNDRV_PCM_TRIGGER_SUSPEND:
		fe->dpcm[stream].state = SND_SOC_DPCM_STATE_STOP;
		break;
	case SNDRV_PCM_TRIGGER_PAUSE_PUSH:
		fe->dpcm[stream].state = SND_SOC_DPCM_STATE_PAUSED;
		break;
	}

out:
	fe->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_NO;
<<<<<<< HEAD
	mutex_unlock(&fe->card->dpcm_mutex);
=======
	snd_soc_card_mutex_unlock(fe->card);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

static int soc_compr_set_params(struct snd_compr_stream *cstream,
<<<<<<< HEAD
					struct snd_compr_params *params)
{
	struct snd_soc_pcm_runtime *rtd = cstream->private_data;
	struct snd_soc_platform *platform = rtd->platform;
	struct snd_soc_dai *codec_dai = rtd->codec_dai;
	int ret = 0;

	mutex_lock_nested(&rtd->pcm_mutex, rtd->pcm_subclass);

	/* first we call set_params for the platform driver
	 * this should configure the soc side
	 * if the machine has compressed ops then we call that as well
	 * expectation is that platform and machine will configure everything
	 * for this compress path, like configuring pcm port for codec
	 */
	if (platform->driver->compr_ops && platform->driver->compr_ops->set_params) {
		ret = platform->driver->compr_ops->set_params(cstream, params);
		if (ret < 0)
			goto err;
	}

	if (rtd->dai_link->compr_ops && rtd->dai_link->compr_ops->set_params) {
		ret = rtd->dai_link->compr_ops->set_params(cstream);
		if (ret < 0)
			goto err;
	}

	snd_soc_dapm_stream_event(rtd,
				codec_dai->driver->playback.stream_name,
				SND_SOC_DAPM_STREAM_START);

	/* cancel any delayed stream shutdown that is pending */
	codec_dai->pop_wait = 0;
	mutex_unlock(&rtd->pcm_mutex);

	cancel_delayed_work_sync(&rtd->delayed_work);

	return ret;

err:
	mutex_unlock(&rtd->pcm_mutex);
=======
				struct snd_compr_params *params)
{
	struct snd_soc_pcm_runtime *rtd = cstream->private_data;
	struct snd_soc_dai *cpu_dai = snd_soc_rtd_to_cpu(rtd, 0);
	int stream = cstream->direction; /* SND_COMPRESS_xxx is same as SNDRV_PCM_STREAM_xxx */
	int ret;

	snd_soc_dpcm_mutex_lock(rtd);

	/*
	 * First we call set_params for the CPU DAI, then the component
	 * driver this should configure the SoC side. If the machine has
	 * compressed ops then we call that as well. The expectation is
	 * that these callbacks will configure everything for this compress
	 * path, like configuring a PCM port for a CODEC.
	 */
	ret = snd_soc_dai_compr_set_params(cpu_dai, cstream, params);
	if (ret < 0)
		goto err;

	ret = snd_soc_component_compr_set_params(cstream, params);
	if (ret < 0)
		goto err;

	ret = snd_soc_link_compr_set_params(cstream);
	if (ret < 0)
		goto err;

	snd_soc_dapm_stream_event(rtd, stream, SND_SOC_DAPM_STREAM_START);

	/* cancel any delayed stream shutdown that is pending */
	rtd->pop_wait = 0;
	snd_soc_dpcm_mutex_unlock(rtd);

	cancel_delayed_work_sync(&rtd->delayed_work);

	return 0;

err:
	snd_soc_dpcm_mutex_unlock(rtd);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

static int soc_compr_set_params_fe(struct snd_compr_stream *cstream,
<<<<<<< HEAD
					struct snd_compr_params *params)
{
	struct snd_soc_pcm_runtime *fe = cstream->private_data;
	struct snd_pcm_substream *fe_substream = fe->pcm->streams[0].substream;
	struct snd_soc_platform *platform = fe->platform;
	int ret = 0, stream;

	if (cstream->direction == SND_COMPRESS_PLAYBACK)
		stream = SNDRV_PCM_STREAM_PLAYBACK;
	else
		stream = SNDRV_PCM_STREAM_CAPTURE;

	mutex_lock(&fe->card->dpcm_mutex);
	/* first we call set_params for the platform driver
	 * this should configure the soc side
	 * if the machine has compressed ops then we call that as well
	 * expectation is that platform and machine will configure everything
	 * for this compress path, like configuring pcm port for codec
	 */
	if (platform->driver->compr_ops && platform->driver->compr_ops->set_params) {
		ret = platform->driver->compr_ops->set_params(cstream, params);
		if (ret < 0)
			goto out;
	}

	if (fe->dai_link->compr_ops && fe->dai_link->compr_ops->set_params) {
		ret = fe->dai_link->compr_ops->set_params(cstream);
		if (ret < 0)
			goto out;
	}

	memcpy(&fe->dpcm[fe_substream->stream].hw_params, params,
			sizeof(struct snd_pcm_hw_params));

	fe->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_FE;

	ret = dpcm_be_dai_hw_params(fe, stream);
	if (ret < 0)
		goto out;

	ret = dpcm_be_dai_prepare(fe, stream);
	if (ret < 0)
		goto out;

	if (stream == SNDRV_PCM_STREAM_PLAYBACK)
		dpcm_dapm_stream_event(fe, stream,
				fe->cpu_dai->driver->playback.stream_name,
				SND_SOC_DAPM_STREAM_START);
	else
		dpcm_dapm_stream_event(fe, stream,
				fe->cpu_dai->driver->capture.stream_name,
				SND_SOC_DAPM_STREAM_START);

=======
				   struct snd_compr_params *params)
{
	struct snd_soc_pcm_runtime *fe = cstream->private_data;
	struct snd_pcm_substream *fe_substream =
		 fe->pcm->streams[cstream->direction].substream;
	struct snd_soc_dai *cpu_dai = snd_soc_rtd_to_cpu(fe, 0);
	int stream = cstream->direction; /* SND_COMPRESS_xxx is same as SNDRV_PCM_STREAM_xxx */
	int ret;

	snd_soc_card_mutex_lock(fe->card);

	/*
	 * Create an empty hw_params for the BE as the machine driver must
	 * fix this up to match DSP decoder and ASRC configuration.
	 * I.e. machine driver fixup for compressed BE is mandatory.
	 */
	memset(&fe->dpcm[fe_substream->stream].hw_params, 0,
		sizeof(struct snd_pcm_hw_params));

	fe->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_FE;

	snd_soc_dpcm_mutex_lock(fe);
	ret = dpcm_be_dai_hw_params(fe, stream);
	snd_soc_dpcm_mutex_unlock(fe);
	if (ret < 0)
		goto out;

	snd_soc_dpcm_mutex_lock(fe);
	ret = dpcm_be_dai_prepare(fe, stream);
	snd_soc_dpcm_mutex_unlock(fe);
	if (ret < 0)
		goto out;

	ret = snd_soc_dai_compr_set_params(cpu_dai, cstream, params);
	if (ret < 0)
		goto out;

	ret = snd_soc_component_compr_set_params(cstream, params);
	if (ret < 0)
		goto out;

	ret = snd_soc_link_compr_set_params(cstream);
	if (ret < 0)
		goto out;
	snd_soc_dpcm_mutex_lock(fe);
	dpcm_dapm_stream_event(fe, stream, SND_SOC_DAPM_STREAM_START);
	snd_soc_dpcm_mutex_unlock(fe);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	fe->dpcm[stream].state = SND_SOC_DPCM_STATE_PREPARE;

out:
	fe->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_NO;
<<<<<<< HEAD
	mutex_unlock(&fe->card->dpcm_mutex);
=======
	snd_soc_card_mutex_unlock(fe->card);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

static int soc_compr_get_params(struct snd_compr_stream *cstream,
<<<<<<< HEAD
					struct snd_codec *params)
{
	struct snd_soc_pcm_runtime *rtd = cstream->private_data;
	struct snd_soc_platform *platform = rtd->platform;
	int ret = 0;

	mutex_lock_nested(&rtd->pcm_mutex, rtd->pcm_subclass);

	if (platform->driver->compr_ops && platform->driver->compr_ops->get_params)
		ret = platform->driver->compr_ops->get_params(cstream, params);

	mutex_unlock(&rtd->pcm_mutex);
	return ret;
}

static int soc_compr_get_caps(struct snd_compr_stream *cstream,
				struct snd_compr_caps *caps)
{
	struct snd_soc_pcm_runtime *rtd = cstream->private_data;
	struct snd_soc_platform *platform = rtd->platform;
	int ret = 0;

	mutex_lock_nested(&rtd->pcm_mutex, rtd->pcm_subclass);

	if (platform->driver->compr_ops && platform->driver->compr_ops->get_caps)
		ret = platform->driver->compr_ops->get_caps(cstream, caps);

	mutex_unlock(&rtd->pcm_mutex);
	return ret;
}

static int soc_compr_get_codec_caps(struct snd_compr_stream *cstream,
				struct snd_compr_codec_caps *codec)
{
	struct snd_soc_pcm_runtime *rtd = cstream->private_data;
	struct snd_soc_platform *platform = rtd->platform;
	int ret = 0;

	mutex_lock_nested(&rtd->pcm_mutex, rtd->pcm_subclass);

	if (platform->driver->compr_ops && platform->driver->compr_ops->get_codec_caps)
		ret = platform->driver->compr_ops->get_codec_caps(cstream, codec);

	mutex_unlock(&rtd->pcm_mutex);
=======
				struct snd_codec *params)
{
	struct snd_soc_pcm_runtime *rtd = cstream->private_data;
	struct snd_soc_dai *cpu_dai = snd_soc_rtd_to_cpu(rtd, 0);
	int ret = 0;

	snd_soc_dpcm_mutex_lock(rtd);

	ret = snd_soc_dai_compr_get_params(cpu_dai, cstream, params);
	if (ret < 0)
		goto err;

	ret = snd_soc_component_compr_get_params(cstream, params);
err:
	snd_soc_dpcm_mutex_unlock(rtd);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

static int soc_compr_ack(struct snd_compr_stream *cstream, size_t bytes)
{
	struct snd_soc_pcm_runtime *rtd = cstream->private_data;
<<<<<<< HEAD
	struct snd_soc_platform *platform = rtd->platform;
	int ret = 0;

	mutex_lock_nested(&rtd->pcm_mutex, rtd->pcm_subclass);

	if (platform->driver->compr_ops && platform->driver->compr_ops->ack)
		ret = platform->driver->compr_ops->ack(cstream, bytes);

	mutex_unlock(&rtd->pcm_mutex);
=======
	struct snd_soc_dai *cpu_dai = snd_soc_rtd_to_cpu(rtd, 0);
	int ret;

	snd_soc_dpcm_mutex_lock(rtd);

	ret = snd_soc_dai_compr_ack(cpu_dai, cstream, bytes);
	if (ret < 0)
		goto err;

	ret = snd_soc_component_compr_ack(cstream, bytes);
err:
	snd_soc_dpcm_mutex_unlock(rtd);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

static int soc_compr_pointer(struct snd_compr_stream *cstream,
<<<<<<< HEAD
			struct snd_compr_tstamp *tstamp)
{
	struct snd_soc_pcm_runtime *rtd = cstream->private_data;
	struct snd_soc_platform *platform = rtd->platform;
	int ret = 0;

	mutex_lock_nested(&rtd->pcm_mutex, rtd->pcm_subclass);

	if (platform->driver->compr_ops && platform->driver->compr_ops->pointer)
		ret = platform->driver->compr_ops->pointer(cstream, tstamp);

	mutex_unlock(&rtd->pcm_mutex);
	return ret;
}

static int soc_compr_copy(struct snd_compr_stream *cstream,
			  char __user *buf, size_t count)
{
	struct snd_soc_pcm_runtime *rtd = cstream->private_data;
	struct snd_soc_platform *platform = rtd->platform;
	int ret = 0;

	mutex_lock_nested(&rtd->pcm_mutex, rtd->pcm_subclass);

	if (platform->driver->compr_ops && platform->driver->compr_ops->copy)
		ret = platform->driver->compr_ops->copy(cstream, buf, count);

	mutex_unlock(&rtd->pcm_mutex);
	return ret;
}

static int sst_compr_set_metadata(struct snd_compr_stream *cstream,
				struct snd_compr_metadata *metadata)
{
	struct snd_soc_pcm_runtime *rtd = cstream->private_data;
	struct snd_soc_platform *platform = rtd->platform;
	int ret = 0;

	if (platform->driver->compr_ops && platform->driver->compr_ops->set_metadata)
		ret = platform->driver->compr_ops->set_metadata(cstream, metadata);

	return ret;
}

static int sst_compr_get_metadata(struct snd_compr_stream *cstream,
				struct snd_compr_metadata *metadata)
{
	struct snd_soc_pcm_runtime *rtd = cstream->private_data;
	struct snd_soc_platform *platform = rtd->platform;
	int ret = 0;

	if (platform->driver->compr_ops && platform->driver->compr_ops->get_metadata)
		ret = platform->driver->compr_ops->get_metadata(cstream, metadata);

	return ret;
}
=======
			     struct snd_compr_tstamp *tstamp)
{
	struct snd_soc_pcm_runtime *rtd = cstream->private_data;
	int ret;
	struct snd_soc_dai *cpu_dai = snd_soc_rtd_to_cpu(rtd, 0);

	snd_soc_dpcm_mutex_lock(rtd);

	ret = snd_soc_dai_compr_pointer(cpu_dai, cstream, tstamp);
	if (ret < 0)
		goto out;

	ret = snd_soc_component_compr_pointer(cstream, tstamp);
out:
	snd_soc_dpcm_mutex_unlock(rtd);
	return ret;
}

static int soc_compr_set_metadata(struct snd_compr_stream *cstream,
				  struct snd_compr_metadata *metadata)
{
	struct snd_soc_pcm_runtime *rtd = cstream->private_data;
	struct snd_soc_dai *cpu_dai = snd_soc_rtd_to_cpu(rtd, 0);
	int ret;

	ret = snd_soc_dai_compr_set_metadata(cpu_dai, cstream, metadata);
	if (ret < 0)
		return ret;

	return snd_soc_component_compr_set_metadata(cstream, metadata);
}

static int soc_compr_get_metadata(struct snd_compr_stream *cstream,
				  struct snd_compr_metadata *metadata)
{
	struct snd_soc_pcm_runtime *rtd = cstream->private_data;
	struct snd_soc_dai *cpu_dai = snd_soc_rtd_to_cpu(rtd, 0);
	int ret;

	ret = snd_soc_dai_compr_get_metadata(cpu_dai, cstream, metadata);
	if (ret < 0)
		return ret;

	return snd_soc_component_compr_get_metadata(cstream, metadata);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* ASoC Compress operations */
static struct snd_compr_ops soc_compr_ops = {
	.open		= soc_compr_open,
	.free		= soc_compr_free,
	.set_params	= soc_compr_set_params,
<<<<<<< HEAD
	.set_metadata   = sst_compr_set_metadata,
	.get_metadata	= sst_compr_get_metadata,
=======
	.set_metadata   = soc_compr_set_metadata,
	.get_metadata	= soc_compr_get_metadata,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.get_params	= soc_compr_get_params,
	.trigger	= soc_compr_trigger,
	.pointer	= soc_compr_pointer,
	.ack		= soc_compr_ack,
<<<<<<< HEAD
	.get_caps	= soc_compr_get_caps,
	.get_codec_caps = soc_compr_get_codec_caps
=======
	.get_caps	= snd_soc_component_compr_get_caps,
	.get_codec_caps = snd_soc_component_compr_get_codec_caps,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/* ASoC Dynamic Compress operations */
static struct snd_compr_ops soc_compr_dyn_ops = {
	.open		= soc_compr_open_fe,
	.free		= soc_compr_free_fe,
	.set_params	= soc_compr_set_params_fe,
	.get_params	= soc_compr_get_params,
<<<<<<< HEAD
	.set_metadata   = sst_compr_set_metadata,
	.get_metadata	= sst_compr_get_metadata,
	.trigger	= soc_compr_trigger_fe,
	.pointer	= soc_compr_pointer,
	.ack		= soc_compr_ack,
	.get_caps	= soc_compr_get_caps,
	.get_codec_caps = soc_compr_get_codec_caps
};

/* create a new compress */
int soc_new_compress(struct snd_soc_pcm_runtime *rtd, int num)
{
	struct snd_soc_codec *codec = rtd->codec;
	struct snd_soc_platform *platform = rtd->platform;
	struct snd_soc_dai *codec_dai = rtd->codec_dai;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
=======
	.set_metadata   = soc_compr_set_metadata,
	.get_metadata	= soc_compr_get_metadata,
	.trigger	= soc_compr_trigger_fe,
	.pointer	= soc_compr_pointer,
	.ack		= soc_compr_ack,
	.get_caps	= snd_soc_component_compr_get_caps,
	.get_codec_caps = snd_soc_component_compr_get_codec_caps,
};

/**
 * snd_soc_new_compress - create a new compress.
 *
 * @rtd: The runtime for which we will create compress
 * @num: the device index number (zero based - shared with normal PCMs)
 *
 * Return: 0 for success, else error.
 */
int snd_soc_new_compress(struct snd_soc_pcm_runtime *rtd, int num)
{
	struct snd_soc_component *component;
	struct snd_soc_dai *codec_dai = snd_soc_rtd_to_codec(rtd, 0);
	struct snd_soc_dai *cpu_dai = snd_soc_rtd_to_cpu(rtd, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct snd_compr *compr;
	struct snd_pcm *be_pcm;
	char new_name[64];
	int ret = 0, direction = 0;
<<<<<<< HEAD

	/* check client and interface hw capabilities */
	snprintf(new_name, sizeof(new_name), "%s %s-%d",
			rtd->dai_link->stream_name, codec_dai->name, num);
#ifdef CONFIG_SND_SOC_WM5110
	if (codec_dai->driver->playback.channels_min)
		direction = SND_COMPRESS_PLAYBACK;
	else if (codec_dai->driver->capture.channels_min)
		direction = SND_COMPRESS_CAPTURE;
	else
		return -EINVAL;
#else
	direction = SND_COMPRESS_PLAYBACK;
#endif
	compr = kzalloc(sizeof(*compr), GFP_KERNEL);
	if (compr == NULL) {
		snd_printk(KERN_ERR "Cannot allocate compr\n");
		return -ENOMEM;
	}

	compr->ops = devm_kzalloc(rtd->card->dev, sizeof(soc_compr_ops),
				  GFP_KERNEL);
	if (compr->ops == NULL) {
		dev_err(rtd->card->dev, "Cannot allocate compressed ops\n");
		ret = -ENOMEM;
		goto compr_err;
	}
=======
	int playback = 0, capture = 0;
	int i;

	/*
	 * make sure these are same value,
	 * and then use these as equally
	 */
	BUILD_BUG_ON((int)SNDRV_PCM_STREAM_PLAYBACK != (int)SND_COMPRESS_PLAYBACK);
	BUILD_BUG_ON((int)SNDRV_PCM_STREAM_CAPTURE  != (int)SND_COMPRESS_CAPTURE);

	if (rtd->dai_link->num_cpus > 1 ||
	    rtd->dai_link->num_codecs > 1) {
		dev_err(rtd->card->dev,
			"Compress ASoC: Multi CPU/Codec not supported\n");
		return -EINVAL;
	}

	if (!codec_dai) {
		dev_err(rtd->card->dev, "Missing codec\n");
		return -EINVAL;
	}

	/* check client and interface hw capabilities */
	if (snd_soc_dai_stream_valid(codec_dai, SNDRV_PCM_STREAM_PLAYBACK) &&
	    snd_soc_dai_stream_valid(cpu_dai,   SNDRV_PCM_STREAM_PLAYBACK))
		playback = 1;
	if (snd_soc_dai_stream_valid(codec_dai, SNDRV_PCM_STREAM_CAPTURE) &&
	    snd_soc_dai_stream_valid(cpu_dai,   SNDRV_PCM_STREAM_CAPTURE))
		capture = 1;

	/*
	 * Compress devices are unidirectional so only one of the directions
	 * should be set, check for that (xor)
	 */
	if (playback + capture != 1) {
		dev_err(rtd->card->dev,
			"Compress ASoC: Invalid direction for P %d, C %d\n",
			playback, capture);
		return -EINVAL;
	}

	if (playback)
		direction = SND_COMPRESS_PLAYBACK;
	else
		direction = SND_COMPRESS_CAPTURE;

	compr = devm_kzalloc(rtd->card->dev, sizeof(*compr), GFP_KERNEL);
	if (!compr)
		return -ENOMEM;

	compr->ops = devm_kzalloc(rtd->card->dev, sizeof(soc_compr_ops),
				  GFP_KERNEL);
	if (!compr->ops)
		return -ENOMEM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (rtd->dai_link->dynamic) {
		snprintf(new_name, sizeof(new_name), "(%s)",
			rtd->dai_link->stream_name);

		ret = snd_pcm_new_internal(rtd->card->snd_card, new_name, num,
<<<<<<< HEAD
				1, 0, &be_pcm);
		if (ret < 0) {
			dev_err(rtd->card->dev, "ASoC: can't create compressed for %s\n",
				rtd->dai_link->name);
			goto compr_err;
		}

		rtd->pcm = be_pcm;
		rtd->fe_compr = 1;
		be_pcm->streams[SNDRV_PCM_STREAM_PLAYBACK].substream->private_data = rtd;
		//be_pcm->streams[SNDRV_PCM_STREAM_CAPTURE].substream->private_data = rtd;
		memcpy(compr->ops, &soc_compr_dyn_ops, sizeof(soc_compr_dyn_ops));
	} else
		memcpy(compr->ops, &soc_compr_ops, sizeof(soc_compr_ops));

	/* Add copy callback for not memory mapped DSPs */
	if (platform->driver->compr_ops && platform->driver->compr_ops->copy)
		compr->ops->copy = soc_compr_copy;

	mutex_init(&compr->lock);
	ret = snd_compress_new(rtd->card->snd_card, num, direction, compr);
	if (ret < 0) {
		pr_err("compress asoc: can't create compress for codec %s\n",
			codec->name);
		goto compr_err;
	}

	/* DAPM dai link stream work */
	INIT_DELAYED_WORK(&rtd->delayed_work, close_delayed_work);
=======
				rtd->dai_link->dpcm_playback,
				rtd->dai_link->dpcm_capture, &be_pcm);
		if (ret < 0) {
			dev_err(rtd->card->dev,
				"Compress ASoC: can't create compressed for %s: %d\n",
				rtd->dai_link->name, ret);
			return ret;
		}

		/* inherit atomicity from DAI link */
		be_pcm->nonatomic = rtd->dai_link->nonatomic;

		rtd->pcm = be_pcm;
		rtd->fe_compr = 1;
		if (rtd->dai_link->dpcm_playback)
			be_pcm->streams[SNDRV_PCM_STREAM_PLAYBACK].substream->private_data = rtd;
		if (rtd->dai_link->dpcm_capture)
			be_pcm->streams[SNDRV_PCM_STREAM_CAPTURE].substream->private_data = rtd;
		memcpy(compr->ops, &soc_compr_dyn_ops, sizeof(soc_compr_dyn_ops));
	} else {
		snprintf(new_name, sizeof(new_name), "%s %s-%d",
			rtd->dai_link->stream_name, codec_dai->name, num);

		memcpy(compr->ops, &soc_compr_ops, sizeof(soc_compr_ops));
	}

	for_each_rtd_components(rtd, i, component) {
		if (!component->driver->compress_ops ||
		    !component->driver->compress_ops->copy)
			continue;

		compr->ops->copy = snd_soc_component_compr_copy;
		break;
	}

	ret = snd_compress_new(rtd->card->snd_card, num, direction,
				new_name, compr);
	if (ret < 0) {
		component = snd_soc_rtd_to_codec(rtd, 0)->component;
		dev_err(component->dev,
			"Compress ASoC: can't create compress for codec %s: %d\n",
			component->name, ret);
		return ret;
	}

	/* DAPM dai link stream work */
	rtd->close_delayed_work_func = snd_soc_close_delayed_work;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	rtd->compr = compr;
	compr->private_data = rtd;

<<<<<<< HEAD
	if (platform->driver->pcm_new) {
		ret = platform->driver->pcm_new(rtd);
		if (ret < 0) {
			pr_err("asoc: compress pcm constructor failed\n");
			goto compr_err;
		}
	}

	printk(KERN_INFO "compress asoc: %s <-> %s mapping ok\n", codec_dai->name,
		cpu_dai->name);
	return ret;

compr_err:
	kfree(compr);
	return ret;
}
=======
	dev_dbg(rtd->card->dev, "Compress ASoC: %s <-> %s mapping ok\n",
		codec_dai->name, cpu_dai->name);

	return 0;
}
EXPORT_SYMBOL_GPL(snd_soc_new_compress);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
