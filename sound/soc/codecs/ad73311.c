<<<<<<< HEAD
/*
 * ad73311.c  --  ALSA Soc AD73311 codec support
 *
 * Copyright:	Analog Device Inc.
 * Author:	Cliff Cai <cliff.cai@analog.com>
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * ad73311.c  --  ALSA Soc AD73311 codec support
 *
 * Copyright:	Analog Devices Inc.
 * Author:	Cliff Cai <cliff.cai@analog.com>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/init.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/ac97_codec.h>
#include <sound/initval.h>
#include <sound/soc.h>

#include "ad73311.h"

<<<<<<< HEAD
=======
static const struct snd_soc_dapm_widget ad73311_dapm_widgets[] = {
SND_SOC_DAPM_INPUT("VINP"),
SND_SOC_DAPM_INPUT("VINN"),
SND_SOC_DAPM_OUTPUT("VOUTN"),
SND_SOC_DAPM_OUTPUT("VOUTP"),
};

static const struct snd_soc_dapm_route ad73311_dapm_routes[] = {
	{ "Capture", NULL, "VINP" },
	{ "Capture", NULL, "VINN" },

	{ "VOUTN", NULL, "Playback" },
	{ "VOUTP", NULL, "Playback" },
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct snd_soc_dai_driver ad73311_dai = {
	.name = "ad73311-hifi",
	.playback = {
		.stream_name = "Playback",
		.channels_min = 1,
		.channels_max = 1,
		.rates = SNDRV_PCM_RATE_8000,
		.formats = SNDRV_PCM_FMTBIT_S16_LE, },
	.capture = {
		.stream_name = "Capture",
		.channels_min = 1,
		.channels_max = 1,
		.rates = SNDRV_PCM_RATE_8000,
		.formats = SNDRV_PCM_FMTBIT_S16_LE, },
};

<<<<<<< HEAD
static struct snd_soc_codec_driver soc_codec_dev_ad73311;

static int ad73311_probe(struct platform_device *pdev)
{
	return snd_soc_register_codec(&pdev->dev,
			&soc_codec_dev_ad73311, &ad73311_dai, 1);
}

static int __devexit ad73311_remove(struct platform_device *pdev)
{
	snd_soc_unregister_codec(&pdev->dev);
	return 0;
=======
static const struct snd_soc_component_driver soc_component_dev_ad73311 = {
	.dapm_widgets		= ad73311_dapm_widgets,
	.num_dapm_widgets	= ARRAY_SIZE(ad73311_dapm_widgets),
	.dapm_routes		= ad73311_dapm_routes,
	.num_dapm_routes	= ARRAY_SIZE(ad73311_dapm_routes),
	.idle_bias_on		= 1,
	.use_pmdown_time	= 1,
	.endianness		= 1,
};

static int ad73311_probe(struct platform_device *pdev)
{
	return devm_snd_soc_register_component(&pdev->dev,
			&soc_component_dev_ad73311, &ad73311_dai, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct platform_driver ad73311_codec_driver = {
	.driver = {
			.name = "ad73311",
<<<<<<< HEAD
			.owner = THIS_MODULE,
	},

	.probe = ad73311_probe,
	.remove = __devexit_p(ad73311_remove),
=======
	},

	.probe = ad73311_probe,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

module_platform_driver(ad73311_codec_driver);

MODULE_DESCRIPTION("ASoC ad73311 driver");
MODULE_AUTHOR("Cliff Cai ");
MODULE_LICENSE("GPL");
