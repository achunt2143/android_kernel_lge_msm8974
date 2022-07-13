<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * ALSA SoC CQ0093 Voice Codec Driver for DaVinci platforms
 *
 * Copyright (C) 2010 Texas Instruments, Inc
 *
 * Author: Miguel Aguilar <miguel.aguilar@ridgerun.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/pm.h>
#include <linux/platform_device.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/clk.h>
#include <linux/mfd/davinci_voicecodec.h>
#include <linux/spi/spi.h>

#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/initval.h>

<<<<<<< HEAD
static inline unsigned int cq93vc_read(struct snd_soc_codec *codec,
						unsigned int reg)
{
	struct davinci_vc *davinci_vc = codec->control_data;

	return readl(davinci_vc->base + reg);
}

static inline int cq93vc_write(struct snd_soc_codec *codec, unsigned int reg,
		       unsigned int value)
{
	struct davinci_vc *davinci_vc = codec->control_data;

	writel(value, davinci_vc->base + reg);

	return 0;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static const struct snd_kcontrol_new cq93vc_snd_controls[] = {
	SOC_SINGLE("PGA Capture Volume", DAVINCI_VC_REG05, 0, 0x03, 0),
	SOC_SINGLE("Mono DAC Playback Volume", DAVINCI_VC_REG09, 0, 0x3f, 0),
};

<<<<<<< HEAD
static int cq93vc_mute(struct snd_soc_dai *dai, int mute)
{
	struct snd_soc_codec *codec = dai->codec;
	u8 reg = cq93vc_read(codec, DAVINCI_VC_REG09) & ~DAVINCI_VC_REG09_MUTE;

	if (mute)
		cq93vc_write(codec, DAVINCI_VC_REG09,
			     reg | DAVINCI_VC_REG09_MUTE);
	else
		cq93vc_write(codec, DAVINCI_VC_REG09, reg);
=======
static int cq93vc_mute(struct snd_soc_dai *dai, int mute, int direction)
{
	struct snd_soc_component *component = dai->component;
	u8 reg;

	if (mute)
		reg = DAVINCI_VC_REG09_MUTE;
	else
		reg = 0;

	snd_soc_component_update_bits(component, DAVINCI_VC_REG09, DAVINCI_VC_REG09_MUTE,
			    reg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static int cq93vc_set_dai_sysclk(struct snd_soc_dai *codec_dai,
				 int clk_id, unsigned int freq, int dir)
{
<<<<<<< HEAD
	struct snd_soc_codec *codec = codec_dai->codec;
	struct davinci_vc *davinci_vc = codec->control_data;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	switch (freq) {
	case 22579200:
	case 27000000:
	case 33868800:
<<<<<<< HEAD
		davinci_vc->cq93vc.sysclk = freq;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;
	}

	return -EINVAL;
}

<<<<<<< HEAD
static int cq93vc_set_bias_level(struct snd_soc_codec *codec,
=======
static int cq93vc_set_bias_level(struct snd_soc_component *component,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				enum snd_soc_bias_level level)
{
	switch (level) {
	case SND_SOC_BIAS_ON:
<<<<<<< HEAD
		cq93vc_write(codec, DAVINCI_VC_REG12,
=======
		snd_soc_component_write(component, DAVINCI_VC_REG12,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			     DAVINCI_VC_REG12_POWER_ALL_ON);
		break;
	case SND_SOC_BIAS_PREPARE:
		break;
	case SND_SOC_BIAS_STANDBY:
<<<<<<< HEAD
		cq93vc_write(codec, DAVINCI_VC_REG12,
=======
		snd_soc_component_write(component, DAVINCI_VC_REG12,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			     DAVINCI_VC_REG12_POWER_ALL_OFF);
		break;
	case SND_SOC_BIAS_OFF:
		/* force all power off */
<<<<<<< HEAD
		cq93vc_write(codec, DAVINCI_VC_REG12,
			     DAVINCI_VC_REG12_POWER_ALL_OFF);
		break;
	}
	codec->dapm.bias_level = level;
=======
		snd_soc_component_write(component, DAVINCI_VC_REG12,
			     DAVINCI_VC_REG12_POWER_ALL_OFF);
		break;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

#define CQ93VC_RATES	(SNDRV_PCM_RATE_8000 | SNDRV_PCM_RATE_16000)
#define CQ93VC_FORMATS	(SNDRV_PCM_FMTBIT_U8 | SNDRV_PCM_FMTBIT_S16_LE)

static const struct snd_soc_dai_ops cq93vc_dai_ops = {
<<<<<<< HEAD
	.digital_mute	= cq93vc_mute,
	.set_sysclk	= cq93vc_set_dai_sysclk,
=======
	.mute_stream	= cq93vc_mute,
	.set_sysclk	= cq93vc_set_dai_sysclk,
	.no_capture_mute = 1,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static struct snd_soc_dai_driver cq93vc_dai = {
	.name = "cq93vc-hifi",
	.playback = {
		.stream_name = "Playback",
		.channels_min = 1,
		.channels_max = 2,
		.rates = CQ93VC_RATES,
		.formats = CQ93VC_FORMATS,},
	.capture = {
		.stream_name = "Capture",
		.channels_min = 1,
		.channels_max = 2,
		.rates = CQ93VC_RATES,
		.formats = CQ93VC_FORMATS,},
	.ops = &cq93vc_dai_ops,
};

<<<<<<< HEAD
static int cq93vc_resume(struct snd_soc_codec *codec)
{
	cq93vc_set_bias_level(codec, SND_SOC_BIAS_STANDBY);
=======
static int cq93vc_probe(struct snd_soc_component *component)
{
	struct davinci_vc *davinci_vc = component->dev->platform_data;

	snd_soc_component_init_regmap(component, davinci_vc->regmap);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static int cq93vc_probe(struct snd_soc_codec *codec)
{
	struct davinci_vc *davinci_vc = codec->dev->platform_data;

	davinci_vc->cq93vc.codec = codec;
	codec->control_data = davinci_vc;

	/* Set controls */
	snd_soc_add_codec_controls(codec, cq93vc_snd_controls,
			     ARRAY_SIZE(cq93vc_snd_controls));

	/* Off, with power on */
	cq93vc_set_bias_level(codec, SND_SOC_BIAS_STANDBY);

	return 0;
}

static int cq93vc_remove(struct snd_soc_codec *codec)
{
	cq93vc_set_bias_level(codec, SND_SOC_BIAS_OFF);

	return 0;
}

static struct snd_soc_codec_driver soc_codec_dev_cq93vc = {
	.read = cq93vc_read,
	.write = cq93vc_write,
	.set_bias_level = cq93vc_set_bias_level,
	.probe = cq93vc_probe,
	.remove = cq93vc_remove,
	.resume = cq93vc_resume,
=======
static const struct snd_soc_component_driver soc_component_dev_cq93vc = {
	.set_bias_level		= cq93vc_set_bias_level,
	.probe			= cq93vc_probe,
	.controls		= cq93vc_snd_controls,
	.num_controls		= ARRAY_SIZE(cq93vc_snd_controls),
	.idle_bias_on		= 1,
	.use_pmdown_time	= 1,
	.endianness		= 1,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static int cq93vc_platform_probe(struct platform_device *pdev)
{
<<<<<<< HEAD
	return snd_soc_register_codec(&pdev->dev,
			&soc_codec_dev_cq93vc, &cq93vc_dai, 1);
}

static int cq93vc_platform_remove(struct platform_device *pdev)
{
	snd_soc_unregister_codec(&pdev->dev);
	return 0;
=======
	return devm_snd_soc_register_component(&pdev->dev,
			&soc_component_dev_cq93vc, &cq93vc_dai, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct platform_driver cq93vc_codec_driver = {
	.driver = {
			.name = "cq93vc-codec",
<<<<<<< HEAD
			.owner = THIS_MODULE,
	},

	.probe = cq93vc_platform_probe,
	.remove = __devexit_p(cq93vc_platform_remove),
=======
	},

	.probe = cq93vc_platform_probe,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

module_platform_driver(cq93vc_codec_driver);

MODULE_DESCRIPTION("Texas Instruments DaVinci ASoC CQ0093 Voice Codec Driver");
MODULE_AUTHOR("Miguel Aguilar");
MODULE_LICENSE("GPL");
