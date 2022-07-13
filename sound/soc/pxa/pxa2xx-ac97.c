<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * linux/sound/pxa2xx-ac97.c -- AC97 support for the Intel PXA2xx chip.
 *
 * Author:	Nicolas Pitre
 * Created:	Dec 02, 2004
 * Copyright:	MontaVista Software Inc.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/init.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/platform_device.h>
<<<<<<< HEAD

=======
#include <linux/dmaengine.h>
#include <linux/dma/pxa-dma.h>

#include <sound/ac97/controller.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <sound/core.h>
#include <sound/ac97_codec.h>
#include <sound/soc.h>
#include <sound/pxa2xx-lib.h>
<<<<<<< HEAD

#include <mach/hardware.h>
#include <mach/regs-ac97.h>
#include <mach/dma.h>
#include <mach/audio.h>

#include "pxa2xx-ac97.h"

static void pxa2xx_ac97_warm_reset(struct snd_ac97 *ac97)
{
	pxa2xx_ac97_try_warm_reset(ac97);

	pxa2xx_ac97_finish_reset(ac97);
}

static void pxa2xx_ac97_cold_reset(struct snd_ac97 *ac97)
{
	pxa2xx_ac97_try_cold_reset(ac97);

	pxa2xx_ac97_finish_reset(ac97);
}

struct snd_ac97_bus_ops soc_ac97_ops = {
	.read	= pxa2xx_ac97_read,
	.write	= pxa2xx_ac97_write,
=======
#include <sound/dmaengine_pcm.h>

#include <linux/platform_data/asoc-pxa.h>

#define PCDR	0x0040  /* PCM FIFO Data Register */
#define MODR	0x0140  /* Modem FIFO Data Register */
#define MCDR	0x0060  /* Mic-in FIFO Data Register */

static void pxa2xx_ac97_warm_reset(struct ac97_controller *adrv)
{
	pxa2xx_ac97_try_warm_reset();

	pxa2xx_ac97_finish_reset();
}

static void pxa2xx_ac97_cold_reset(struct ac97_controller *adrv)
{
	pxa2xx_ac97_try_cold_reset();

	pxa2xx_ac97_finish_reset();
}

static int pxa2xx_ac97_read_actrl(struct ac97_controller *adrv, int slot,
				  unsigned short reg)
{
	return pxa2xx_ac97_read(slot, reg);
}

static int pxa2xx_ac97_write_actrl(struct ac97_controller *adrv, int slot,
				   unsigned short reg, unsigned short val)
{
	return pxa2xx_ac97_write(slot, reg, val);
}

static struct ac97_controller_ops pxa2xx_ac97_ops = {
	.read	= pxa2xx_ac97_read_actrl,
	.write	= pxa2xx_ac97_write_actrl,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.warm_reset	= pxa2xx_ac97_warm_reset,
	.reset	= pxa2xx_ac97_cold_reset,
};

<<<<<<< HEAD
static struct pxa2xx_pcm_dma_params pxa2xx_ac97_pcm_stereo_out = {
	.name			= "AC97 PCM Stereo out",
	.dev_addr		= __PREG(PCDR),
	.drcmr			= &DRCMR(12),
	.dcmd			= DCMD_INCSRCADDR | DCMD_FLOWTRG |
				  DCMD_BURST32 | DCMD_WIDTH4,
};

static struct pxa2xx_pcm_dma_params pxa2xx_ac97_pcm_stereo_in = {
	.name			= "AC97 PCM Stereo in",
	.dev_addr		= __PREG(PCDR),
	.drcmr			= &DRCMR(11),
	.dcmd			= DCMD_INCTRGADDR | DCMD_FLOWSRC |
				  DCMD_BURST32 | DCMD_WIDTH4,
};

static struct pxa2xx_pcm_dma_params pxa2xx_ac97_pcm_aux_mono_out = {
	.name			= "AC97 Aux PCM (Slot 5) Mono out",
	.dev_addr		= __PREG(MODR),
	.drcmr			= &DRCMR(10),
	.dcmd			= DCMD_INCSRCADDR | DCMD_FLOWTRG |
				  DCMD_BURST16 | DCMD_WIDTH2,
};

static struct pxa2xx_pcm_dma_params pxa2xx_ac97_pcm_aux_mono_in = {
	.name			= "AC97 Aux PCM (Slot 5) Mono in",
	.dev_addr		= __PREG(MODR),
	.drcmr			= &DRCMR(9),
	.dcmd			= DCMD_INCTRGADDR | DCMD_FLOWSRC |
				  DCMD_BURST16 | DCMD_WIDTH2,
};

static struct pxa2xx_pcm_dma_params pxa2xx_ac97_pcm_mic_mono_in = {
	.name			= "AC97 Mic PCM (Slot 6) Mono in",
	.dev_addr		= __PREG(MCDR),
	.drcmr			= &DRCMR(8),
	.dcmd			= DCMD_INCTRGADDR | DCMD_FLOWSRC |
				  DCMD_BURST16 | DCMD_WIDTH2,
};

#ifdef CONFIG_PM
static int pxa2xx_ac97_suspend(struct snd_soc_dai *dai)
{
	return pxa2xx_ac97_hw_suspend();
}

static int pxa2xx_ac97_resume(struct snd_soc_dai *dai)
{
	return pxa2xx_ac97_hw_resume();
}

#else
#define pxa2xx_ac97_suspend	NULL
#define pxa2xx_ac97_resume	NULL
#endif

static int __devinit pxa2xx_ac97_probe(struct snd_soc_dai *dai)
{
	return pxa2xx_ac97_hw_probe(to_platform_device(dai->dev));
}

static int pxa2xx_ac97_remove(struct snd_soc_dai *dai)
{
	pxa2xx_ac97_hw_remove(to_platform_device(dai->dev));
	return 0;
}

static int pxa2xx_ac97_hw_params(struct snd_pcm_substream *substream,
				 struct snd_pcm_hw_params *params,
				 struct snd_soc_dai *cpu_dai)
{
	struct pxa2xx_pcm_dma_params *dma_data;
=======
static struct snd_dmaengine_dai_dma_data pxa2xx_ac97_pcm_stereo_in = {
	.addr_width	= DMA_SLAVE_BUSWIDTH_4_BYTES,
	.chan_name	= "pcm_pcm_stereo_in",
	.maxburst	= 32,
};

static struct snd_dmaengine_dai_dma_data pxa2xx_ac97_pcm_stereo_out = {
	.addr_width	= DMA_SLAVE_BUSWIDTH_4_BYTES,
	.chan_name	= "pcm_pcm_stereo_out",
	.maxburst	= 32,
};

static struct snd_dmaengine_dai_dma_data pxa2xx_ac97_pcm_aux_mono_out = {
	.addr_width	= DMA_SLAVE_BUSWIDTH_2_BYTES,
	.chan_name	= "pcm_aux_mono_out",
	.maxburst	= 16,
};

static struct snd_dmaengine_dai_dma_data pxa2xx_ac97_pcm_aux_mono_in = {
	.addr_width	= DMA_SLAVE_BUSWIDTH_2_BYTES,
	.chan_name	= "pcm_aux_mono_in",
	.maxburst	= 16,
};

static struct snd_dmaengine_dai_dma_data pxa2xx_ac97_pcm_mic_mono_in = {
	.addr_width	= DMA_SLAVE_BUSWIDTH_2_BYTES,
	.chan_name	= "pcm_aux_mic_mono",
	.maxburst	= 16,
};

static int pxa2xx_ac97_hifi_startup(struct snd_pcm_substream *substream,
				    struct snd_soc_dai *cpu_dai)
{
	struct snd_dmaengine_dai_dma_data *dma_data;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
		dma_data = &pxa2xx_ac97_pcm_stereo_out;
	else
		dma_data = &pxa2xx_ac97_pcm_stereo_in;

	snd_soc_dai_set_dma_data(cpu_dai, substream, dma_data);

	return 0;
}

<<<<<<< HEAD
static int pxa2xx_ac97_hw_aux_params(struct snd_pcm_substream *substream,
				     struct snd_pcm_hw_params *params,
				     struct snd_soc_dai *cpu_dai)
{
	struct pxa2xx_pcm_dma_params *dma_data;
=======
static int pxa2xx_ac97_aux_startup(struct snd_pcm_substream *substream,
				   struct snd_soc_dai *cpu_dai)
{
	struct snd_dmaengine_dai_dma_data *dma_data;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
		dma_data = &pxa2xx_ac97_pcm_aux_mono_out;
	else
		dma_data = &pxa2xx_ac97_pcm_aux_mono_in;

	snd_soc_dai_set_dma_data(cpu_dai, substream, dma_data);

	return 0;
}

<<<<<<< HEAD
static int pxa2xx_ac97_hw_mic_params(struct snd_pcm_substream *substream,
				     struct snd_pcm_hw_params *params,
				     struct snd_soc_dai *cpu_dai)
{
	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
		return -ENODEV;
	else
		snd_soc_dai_set_dma_data(cpu_dai, substream,
					 &pxa2xx_ac97_pcm_mic_mono_in);
=======
static int pxa2xx_ac97_mic_startup(struct snd_pcm_substream *substream,
				   struct snd_soc_dai *cpu_dai)
{
	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
		return -ENODEV;
	snd_soc_dai_set_dma_data(cpu_dai, substream,
				 &pxa2xx_ac97_pcm_mic_mono_in);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

#define PXA2XX_AC97_RATES (SNDRV_PCM_RATE_8000 | SNDRV_PCM_RATE_11025 |\
		SNDRV_PCM_RATE_16000 | SNDRV_PCM_RATE_22050 | SNDRV_PCM_RATE_44100 | \
		SNDRV_PCM_RATE_48000)

static const struct snd_soc_dai_ops pxa_ac97_hifi_dai_ops = {
<<<<<<< HEAD
	.hw_params	= pxa2xx_ac97_hw_params,
};

static const struct snd_soc_dai_ops pxa_ac97_aux_dai_ops = {
	.hw_params	= pxa2xx_ac97_hw_aux_params,
};

static const struct snd_soc_dai_ops pxa_ac97_mic_dai_ops = {
	.hw_params	= pxa2xx_ac97_hw_mic_params,
=======
	.startup	= pxa2xx_ac97_hifi_startup,
};

static const struct snd_soc_dai_ops pxa_ac97_aux_dai_ops = {
	.startup	= pxa2xx_ac97_aux_startup,
};

static const struct snd_soc_dai_ops pxa_ac97_mic_dai_ops = {
	.startup	= pxa2xx_ac97_mic_startup,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/*
 * There is only 1 physical AC97 interface for pxa2xx, but it
 * has extra fifo's that can be used for aux DACs and ADCs.
 */
static struct snd_soc_dai_driver pxa_ac97_dai_driver[] = {
{
	.name = "pxa2xx-ac97",
<<<<<<< HEAD
	.ac97_control = 1,
	.probe = pxa2xx_ac97_probe,
	.remove = pxa2xx_ac97_remove,
	.suspend = pxa2xx_ac97_suspend,
	.resume = pxa2xx_ac97_resume,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.playback = {
		.stream_name = "AC97 Playback",
		.channels_min = 2,
		.channels_max = 2,
		.rates = PXA2XX_AC97_RATES,
		.formats = SNDRV_PCM_FMTBIT_S16_LE,},
	.capture = {
		.stream_name = "AC97 Capture",
		.channels_min = 2,
		.channels_max = 2,
		.rates = PXA2XX_AC97_RATES,
		.formats = SNDRV_PCM_FMTBIT_S16_LE,},
	.ops = &pxa_ac97_hifi_dai_ops,
},
{
	.name = "pxa2xx-ac97-aux",
<<<<<<< HEAD
	.ac97_control = 1,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.playback = {
		.stream_name = "AC97 Aux Playback",
		.channels_min = 1,
		.channels_max = 1,
		.rates = PXA2XX_AC97_RATES,
		.formats = SNDRV_PCM_FMTBIT_S16_LE,},
	.capture = {
		.stream_name = "AC97 Aux Capture",
		.channels_min = 1,
		.channels_max = 1,
		.rates = PXA2XX_AC97_RATES,
		.formats = SNDRV_PCM_FMTBIT_S16_LE,},
	.ops = &pxa_ac97_aux_dai_ops,
},
{
	.name = "pxa2xx-ac97-mic",
<<<<<<< HEAD
	.ac97_control = 1,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.capture = {
		.stream_name = "AC97 Mic Capture",
		.channels_min = 1,
		.channels_max = 1,
		.rates = PXA2XX_AC97_RATES,
		.formats = SNDRV_PCM_FMTBIT_S16_LE,},
	.ops = &pxa_ac97_mic_dai_ops,
},
};

<<<<<<< HEAD
EXPORT_SYMBOL_GPL(soc_ac97_ops);

static __devinit int pxa2xx_ac97_dev_probe(struct platform_device *pdev)
{
=======
static const struct snd_soc_component_driver pxa_ac97_component = {
	.name		= "pxa-ac97",
	.pcm_construct	= pxa2xx_soc_pcm_new,
	.open		= pxa2xx_soc_pcm_open,
	.close		= pxa2xx_soc_pcm_close,
	.hw_params	= pxa2xx_soc_pcm_hw_params,
	.prepare	= pxa2xx_soc_pcm_prepare,
	.trigger	= pxa2xx_soc_pcm_trigger,
	.pointer	= pxa2xx_soc_pcm_pointer,
};

#ifdef CONFIG_OF
static const struct of_device_id pxa2xx_ac97_dt_ids[] = {
	{ .compatible = "marvell,pxa250-ac97", },
	{ .compatible = "marvell,pxa270-ac97", },
	{ .compatible = "marvell,pxa300-ac97", },
	{ }
};
MODULE_DEVICE_TABLE(of, pxa2xx_ac97_dt_ids);

#endif

static int pxa2xx_ac97_dev_probe(struct platform_device *pdev)
{
	int ret;
	struct ac97_controller *ctrl;
	pxa2xx_audio_ops_t *pdata = pdev->dev.platform_data;
	struct resource *regs;
	void **codecs_pdata;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (pdev->id != -1) {
		dev_err(&pdev->dev, "PXA2xx has only one AC97 port.\n");
		return -ENXIO;
	}

<<<<<<< HEAD
=======
	regs = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!regs)
		return -ENXIO;

	pxa2xx_ac97_pcm_stereo_in.addr = regs->start + PCDR;
	pxa2xx_ac97_pcm_stereo_out.addr = regs->start + PCDR;
	pxa2xx_ac97_pcm_aux_mono_out.addr = regs->start + MODR;
	pxa2xx_ac97_pcm_aux_mono_in.addr = regs->start + MODR;
	pxa2xx_ac97_pcm_mic_mono_in.addr = regs->start + MCDR;

	ret = pxa2xx_ac97_hw_probe(pdev);
	if (ret) {
		dev_err(&pdev->dev, "PXA2xx AC97 hw probe error (%d)\n", ret);
		return ret;
	}

	codecs_pdata = pdata ? pdata->codec_pdata : NULL;
	ctrl = snd_ac97_controller_register(&pxa2xx_ac97_ops, &pdev->dev,
					    AC97_SLOTS_AVAILABLE_ALL,
					    codecs_pdata);
	if (IS_ERR(ctrl))
		return PTR_ERR(ctrl);

	platform_set_drvdata(pdev, ctrl);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Punt most of the init to the SoC probe; we may need the machine
	 * driver to do interesting things with the clocking to get us up
	 * and running.
	 */
<<<<<<< HEAD
	return snd_soc_register_dais(&pdev->dev, pxa_ac97_dai_driver,
			ARRAY_SIZE(pxa_ac97_dai_driver));
}

static int __devexit pxa2xx_ac97_dev_remove(struct platform_device *pdev)
{
	snd_soc_unregister_dais(&pdev->dev, ARRAY_SIZE(pxa_ac97_dai_driver));
	return 0;
}

static struct platform_driver pxa2xx_ac97_driver = {
	.probe		= pxa2xx_ac97_dev_probe,
	.remove		= __devexit_p(pxa2xx_ac97_dev_remove),
	.driver		= {
		.name	= "pxa2xx-ac97",
		.owner	= THIS_MODULE,
=======
	return devm_snd_soc_register_component(&pdev->dev, &pxa_ac97_component,
					  pxa_ac97_dai_driver, ARRAY_SIZE(pxa_ac97_dai_driver));
}

static void pxa2xx_ac97_dev_remove(struct platform_device *pdev)
{
	struct ac97_controller *ctrl = platform_get_drvdata(pdev);

	snd_ac97_controller_unregister(ctrl);
	pxa2xx_ac97_hw_remove(pdev);
}

static int pxa2xx_ac97_dev_suspend(struct device *dev)
{
	return pxa2xx_ac97_hw_suspend();
}

static int pxa2xx_ac97_dev_resume(struct device *dev)
{
	return pxa2xx_ac97_hw_resume();
}

static DEFINE_SIMPLE_DEV_PM_OPS(pxa2xx_ac97_pm_ops,
		pxa2xx_ac97_dev_suspend, pxa2xx_ac97_dev_resume);

static struct platform_driver pxa2xx_ac97_driver = {
	.probe		= pxa2xx_ac97_dev_probe,
	.remove_new	= pxa2xx_ac97_dev_remove,
	.driver		= {
		.name	= "pxa2xx-ac97",
		.pm	= &pxa2xx_ac97_pm_ops,
		.of_match_table = of_match_ptr(pxa2xx_ac97_dt_ids),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
};

module_platform_driver(pxa2xx_ac97_driver);

MODULE_AUTHOR("Nicolas Pitre");
MODULE_DESCRIPTION("AC97 driver for the Intel PXA2xx chip");
MODULE_LICENSE("GPL");
<<<<<<< HEAD
=======
MODULE_ALIAS("platform:pxa2xx-ac97");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
