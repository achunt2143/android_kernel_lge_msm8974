<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * ac97.c  --  ALSA Soc AC97 codec support
 *
 * Copyright 2005 Wolfson Microelectronics PLC.
 * Author: Liam Girdwood <lrg@slimlogic.co.uk>
 *
<<<<<<< HEAD
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Generic AC97 support.
 */

#include <linux/init.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/module.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/ac97_codec.h>
#include <sound/initval.h>
#include <sound/soc.h>

<<<<<<< HEAD
static int ac97_prepare(struct snd_pcm_substream *substream,
			struct snd_soc_dai *dai)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_codec *codec = rtd->codec;

	int reg = (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) ?
		  AC97_PCM_FRONT_DAC_RATE : AC97_PCM_LR_ADC_RATE;
	return snd_ac97_set_rate(codec->ac97, reg, runtime->rate);
}

#define STD_AC97_RATES (SNDRV_PCM_RATE_8000 | SNDRV_PCM_RATE_11025 |\
		SNDRV_PCM_RATE_22050 | SNDRV_PCM_RATE_44100 |\
		SNDRV_PCM_RATE_48000)

=======
static const struct snd_soc_dapm_widget ac97_widgets[] = {
	SND_SOC_DAPM_INPUT("RX"),
	SND_SOC_DAPM_OUTPUT("TX"),
};

static const struct snd_soc_dapm_route ac97_routes[] = {
	{ "AC97 Capture", NULL, "RX" },
	{ "TX", NULL, "AC97 Playback" },
};

static int ac97_prepare(struct snd_pcm_substream *substream,
			struct snd_soc_dai *dai)
{
	struct snd_soc_component *component = dai->component;
	struct snd_ac97 *ac97 = snd_soc_component_get_drvdata(component);

	int reg = (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) ?
		  AC97_PCM_FRONT_DAC_RATE : AC97_PCM_LR_ADC_RATE;
	return snd_ac97_set_rate(ac97, reg, substream->runtime->rate);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static const struct snd_soc_dai_ops ac97_dai_ops = {
	.prepare	= ac97_prepare,
};

static struct snd_soc_dai_driver ac97_dai = {
	.name = "ac97-hifi",
<<<<<<< HEAD
	.ac97_control = 1,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.playback = {
		.stream_name = "AC97 Playback",
		.channels_min = 1,
		.channels_max = 2,
<<<<<<< HEAD
		.rates = STD_AC97_RATES,
=======
		.rates = SNDRV_PCM_RATE_KNOT,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.formats = SND_SOC_STD_AC97_FMTS,},
	.capture = {
		.stream_name = "AC97 Capture",
		.channels_min = 1,
		.channels_max = 2,
<<<<<<< HEAD
		.rates = STD_AC97_RATES,
=======
		.rates = SNDRV_PCM_RATE_KNOT,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.formats = SND_SOC_STD_AC97_FMTS,},
	.ops = &ac97_dai_ops,
};

<<<<<<< HEAD
static unsigned int ac97_read(struct snd_soc_codec *codec,
	unsigned int reg)
{
	return soc_ac97_ops.read(codec->ac97, reg);
}

static int ac97_write(struct snd_soc_codec *codec, unsigned int reg,
	unsigned int val)
{
	soc_ac97_ops.write(codec->ac97, reg, val);
	return 0;
}

static int ac97_soc_probe(struct snd_soc_codec *codec)
{
=======
static int ac97_soc_probe(struct snd_soc_component *component)
{
	struct snd_ac97 *ac97;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct snd_ac97_bus *ac97_bus;
	struct snd_ac97_template ac97_template;
	int ret;

	/* add codec as bus device for standard ac97 */
<<<<<<< HEAD
	ret = snd_ac97_bus(codec->card->snd_card, 0, &soc_ac97_ops, NULL, &ac97_bus);
=======
	ret = snd_ac97_bus(component->card->snd_card, 0, soc_ac97_ops,
			   NULL, &ac97_bus);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ret < 0)
		return ret;

	memset(&ac97_template, 0, sizeof(struct snd_ac97_template));
<<<<<<< HEAD
	ret = snd_ac97_mixer(ac97_bus, &ac97_template, &codec->ac97);
	if (ret < 0)
		return ret;

	return 0;
}

static int ac97_soc_remove(struct snd_soc_codec *codec)
{
=======
	ret = snd_ac97_mixer(ac97_bus, &ac97_template, &ac97);
	if (ret < 0)
		return ret;

	snd_soc_component_set_drvdata(component, ac97);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

#ifdef CONFIG_PM
<<<<<<< HEAD
static int ac97_soc_suspend(struct snd_soc_codec *codec)
{
	snd_ac97_suspend(codec->ac97);
=======
static int ac97_soc_suspend(struct snd_soc_component *component)
{
	struct snd_ac97 *ac97 = snd_soc_component_get_drvdata(component);

	snd_ac97_suspend(ac97);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static int ac97_soc_resume(struct snd_soc_codec *codec)
{
	snd_ac97_resume(codec->ac97);
=======
static int ac97_soc_resume(struct snd_soc_component *component)
{

	struct snd_ac97 *ac97 = snd_soc_component_get_drvdata(component);

	snd_ac97_resume(ac97);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}
#else
#define ac97_soc_suspend NULL
#define ac97_soc_resume NULL
#endif

<<<<<<< HEAD
static struct snd_soc_codec_driver soc_codec_dev_ac97 = {
	.write =	ac97_write,
	.read =		ac97_read,
	.probe = 	ac97_soc_probe,
	.remove = 	ac97_soc_remove,
	.suspend =	ac97_soc_suspend,
	.resume =	ac97_soc_resume,
};

static __devinit int ac97_probe(struct platform_device *pdev)
{
	return snd_soc_register_codec(&pdev->dev,
			&soc_codec_dev_ac97, &ac97_dai, 1);
}

static int __devexit ac97_remove(struct platform_device *pdev)
{
	snd_soc_unregister_codec(&pdev->dev);
	return 0;
=======
static const struct snd_soc_component_driver soc_component_dev_ac97 = {
	.probe			= ac97_soc_probe,
	.suspend		= ac97_soc_suspend,
	.resume			= ac97_soc_resume,
	.dapm_widgets		= ac97_widgets,
	.num_dapm_widgets	= ARRAY_SIZE(ac97_widgets),
	.dapm_routes		= ac97_routes,
	.num_dapm_routes	= ARRAY_SIZE(ac97_routes),
	.idle_bias_on		= 1,
	.use_pmdown_time	= 1,
	.endianness		= 1,
};

static int ac97_probe(struct platform_device *pdev)
{
	return devm_snd_soc_register_component(&pdev->dev,
			&soc_component_dev_ac97, &ac97_dai, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct platform_driver ac97_codec_driver = {
	.driver = {
		.name = "ac97-codec",
<<<<<<< HEAD
		.owner = THIS_MODULE,
	},

	.probe = ac97_probe,
	.remove = __devexit_p(ac97_remove),
=======
	},

	.probe = ac97_probe,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

module_platform_driver(ac97_codec_driver);

MODULE_DESCRIPTION("Soc Generic AC97 driver");
MODULE_AUTHOR("Liam Girdwood");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:ac97-codec");
