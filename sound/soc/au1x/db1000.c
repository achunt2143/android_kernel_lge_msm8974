<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * DB1000/DB1500/DB1100 ASoC audio fabric support code.
 *
 * (c) 2011 Manuel Lauss <manuel.lauss@googlemail.com>
 *
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/timer.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/soc.h>
#include <asm/mach-au1x00/au1000.h>
#include <asm/mach-db1x00/bcsr.h>

#include "psc.h"

<<<<<<< HEAD
static struct snd_soc_dai_link db1000_ac97_dai = {
	.name		= "AC97",
	.stream_name	= "AC97 HiFi",
	.codec_dai_name	= "ac97-hifi",
	.cpu_dai_name	= "alchemy-ac97c",
	.platform_name	= "alchemy-pcm-dma.0",
	.codec_name	= "ac97-codec",
=======
SND_SOC_DAILINK_DEFS(hifi,
	DAILINK_COMP_ARRAY(COMP_CPU("alchemy-ac97c")),
	DAILINK_COMP_ARRAY(COMP_CODEC("ac97-codec", "ac97-hifi")),
	DAILINK_COMP_ARRAY(COMP_PLATFORM("alchemy-pcm-dma.0")));

static struct snd_soc_dai_link db1000_ac97_dai = {
	.name		= "AC97",
	.stream_name	= "AC97 HiFi",
	SND_SOC_DAILINK_REG(hifi),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static struct snd_soc_card db1000_ac97 = {
	.name		= "DB1000_AC97",
	.owner		= THIS_MODULE,
	.dai_link	= &db1000_ac97_dai,
	.num_links	= 1,
};

<<<<<<< HEAD
static int __devinit db1000_audio_probe(struct platform_device *pdev)
{
	struct snd_soc_card *card = &db1000_ac97;
	card->dev = &pdev->dev;
	return snd_soc_register_card(card);
}

static int __devexit db1000_audio_remove(struct platform_device *pdev)
{
	struct snd_soc_card *card = platform_get_drvdata(pdev);
	snd_soc_unregister_card(card);
	return 0;
=======
static int db1000_audio_probe(struct platform_device *pdev)
{
	struct snd_soc_card *card = &db1000_ac97;
	card->dev = &pdev->dev;
	return devm_snd_soc_register_card(&pdev->dev, card);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct platform_driver db1000_audio_driver = {
	.driver	= {
		.name	= "db1000-audio",
<<<<<<< HEAD
		.owner	= THIS_MODULE,
		.pm	= &snd_soc_pm_ops,
	},
	.probe		= db1000_audio_probe,
	.remove		= __devexit_p(db1000_audio_remove),
=======
		.pm	= &snd_soc_pm_ops,
	},
	.probe		= db1000_audio_probe,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

module_platform_driver(db1000_audio_driver);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("DB1000/DB1500/DB1100 ASoC audio");
MODULE_AUTHOR("Manuel Lauss");
