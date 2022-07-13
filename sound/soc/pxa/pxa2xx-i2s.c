<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * pxa2xx-i2s.c  --  ALSA Soc Audio Layer
 *
 * Copyright 2005 Wolfson Microelectronics PLC.
 * Author: Liam Girdwood
 *         lrg@slimlogic.co.uk
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/clk.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/initval.h>
#include <sound/soc.h>
#include <sound/pxa2xx-lib.h>
<<<<<<< HEAD

#include <mach/hardware.h>
#include <mach/dma.h>
#include <mach/audio.h>
=======
#include <sound/dmaengine_pcm.h>

#include <linux/platform_data/asoc-pxa.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include "pxa2xx-i2s.h"

/*
 * I2S Controller Register and Bit Definitions
 */
<<<<<<< HEAD
#define SACR0		__REG(0x40400000)  /* Global Control Register */
#define SACR1		__REG(0x40400004)  /* Serial Audio I 2 S/MSB-Justified Control Register */
#define SASR0		__REG(0x4040000C)  /* Serial Audio I 2 S/MSB-Justified Interface and FIFO Status Register */
#define SAIMR		__REG(0x40400014)  /* Serial Audio Interrupt Mask Register */
#define SAICR		__REG(0x40400018)  /* Serial Audio Interrupt Clear Register */
#define SADIV		__REG(0x40400060)  /* Audio Clock Divider Register. */
#define SADR		__REG(0x40400080)  /* Serial Audio Data Register (TX and RX FIFO access Register). */
=======
#define SACR0		(0x0000)	/* Global Control Register */
#define SACR1		(0x0004)	/* Serial Audio I 2 S/MSB-Justified Control Register */
#define SASR0		(0x000C)	/* Serial Audio I 2 S/MSB-Justified Interface and FIFO Status Register */
#define SAIMR		(0x0014)	/* Serial Audio Interrupt Mask Register */
#define SAICR		(0x0018)	/* Serial Audio Interrupt Clear Register */
#define SADIV		(0x0060)	/* Audio Clock Divider Register. */
#define SADR		(0x0080)	/* Serial Audio Data Register (TX and RX FIFO access Register). */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define SACR0_RFTH(x)	((x) << 12)	/* Rx FIFO Interrupt or DMA Trigger Threshold */
#define SACR0_TFTH(x)	((x) << 8)	/* Tx FIFO Interrupt or DMA Trigger Threshold */
#define SACR0_STRF	(1 << 5)	/* FIFO Select for EFWR Special Function */
#define SACR0_EFWR	(1 << 4)	/* Enable EFWR Function  */
#define SACR0_RST	(1 << 3)	/* FIFO, i2s Register Reset */
<<<<<<< HEAD
#define SACR0_BCKD	(1 << 2) 	/* Bit Clock Direction */
#define SACR0_ENB	(1 << 0)	/* Enable I2S Link */
#define SACR1_ENLBF	(1 << 5)	/* Enable Loopback */
#define SACR1_DRPL	(1 << 4) 	/* Disable Replaying Function */
=======
#define SACR0_BCKD	(1 << 2)	/* Bit Clock Direction */
#define SACR0_ENB	(1 << 0)	/* Enable I2S Link */
#define SACR1_ENLBF	(1 << 5)	/* Enable Loopback */
#define SACR1_DRPL	(1 << 4)	/* Disable Replaying Function */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define SACR1_DREC	(1 << 3)	/* Disable Recording Function */
#define SACR1_AMSL	(1 << 0)	/* Specify Alternate Mode */

#define SASR0_I2SOFF	(1 << 7)	/* Controller Status */
#define SASR0_ROR	(1 << 6)	/* Rx FIFO Overrun */
#define SASR0_TUR	(1 << 5)	/* Tx FIFO Underrun */
#define SASR0_RFS	(1 << 4)	/* Rx FIFO Service Request */
#define SASR0_TFS	(1 << 3)	/* Tx FIFO Service Request */
#define SASR0_BSY	(1 << 2)	/* I2S Busy */
#define SASR0_RNE	(1 << 1)	/* Rx FIFO Not Empty */
<<<<<<< HEAD
#define SASR0_TNF	(1 << 0) 	/* Tx FIFO Not Empty */
=======
#define SASR0_TNF	(1 << 0)	/* Tx FIFO Not Empty */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define SAICR_ROR	(1 << 6)	/* Clear Rx FIFO Overrun Interrupt */
#define SAICR_TUR	(1 << 5)	/* Clear Tx FIFO Underrun Interrupt */

#define SAIMR_ROR	(1 << 6)	/* Enable Rx FIFO Overrun Condition Interrupt */
#define SAIMR_TUR	(1 << 5)	/* Enable Tx FIFO Underrun Condition Interrupt */
#define SAIMR_RFS	(1 << 4)	/* Enable Rx FIFO Service Interrupt */
#define SAIMR_TFS	(1 << 3)	/* Enable Tx FIFO Service Interrupt */

struct pxa_i2s_port {
	u32 sadiv;
	u32 sacr0;
	u32 sacr1;
	u32 saimr;
	int master;
	u32 fmt;
};
static struct pxa_i2s_port pxa_i2s;
static struct clk *clk_i2s;
static int clk_ena = 0;
<<<<<<< HEAD

static struct pxa2xx_pcm_dma_params pxa2xx_i2s_pcm_stereo_out = {
	.name			= "I2S PCM Stereo out",
	.dev_addr		= __PREG(SADR),
	.drcmr			= &DRCMR(3),
	.dcmd			= DCMD_INCSRCADDR | DCMD_FLOWTRG |
				  DCMD_BURST32 | DCMD_WIDTH4,
};

static struct pxa2xx_pcm_dma_params pxa2xx_i2s_pcm_stereo_in = {
	.name			= "I2S PCM Stereo in",
	.dev_addr		= __PREG(SADR),
	.drcmr			= &DRCMR(2),
	.dcmd			= DCMD_INCTRGADDR | DCMD_FLOWSRC |
				  DCMD_BURST32 | DCMD_WIDTH4,
=======
static void __iomem *i2s_reg_base;

static struct snd_dmaengine_dai_dma_data pxa2xx_i2s_pcm_stereo_out = {
	.addr_width	= DMA_SLAVE_BUSWIDTH_4_BYTES,
	.chan_name	= "tx",
	.maxburst	= 32,
};

static struct snd_dmaengine_dai_dma_data pxa2xx_i2s_pcm_stereo_in = {
	.addr_width	= DMA_SLAVE_BUSWIDTH_4_BYTES,
	.chan_name	= "rx",
	.maxburst	= 32,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static int pxa2xx_i2s_startup(struct snd_pcm_substream *substream,
			      struct snd_soc_dai *dai)
{
<<<<<<< HEAD
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
=======
	struct snd_soc_pcm_runtime *rtd = snd_soc_substream_to_rtd(substream);
	struct snd_soc_dai *cpu_dai = snd_soc_rtd_to_cpu(rtd, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (IS_ERR(clk_i2s))
		return PTR_ERR(clk_i2s);

<<<<<<< HEAD
	if (!cpu_dai->active)
		SACR0 = 0;
=======
	if (!snd_soc_dai_active(cpu_dai))
		writel(0, i2s_reg_base + SACR0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

/* wait for I2S controller to be ready */
static int pxa_i2s_wait(void)
{
	int i;

	/* flush the Rx FIFO */
<<<<<<< HEAD
	for(i = 0; i < 16; i++)
		SADR;
=======
	for (i = 0; i < 16; i++)
		readl(i2s_reg_base + SADR);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static int pxa2xx_i2s_set_dai_fmt(struct snd_soc_dai *cpu_dai,
		unsigned int fmt)
{
	/* interface format */
	switch (fmt & SND_SOC_DAIFMT_FORMAT_MASK) {
	case SND_SOC_DAIFMT_I2S:
		pxa_i2s.fmt = 0;
		break;
	case SND_SOC_DAIFMT_LEFT_J:
		pxa_i2s.fmt = SACR1_AMSL;
		break;
	}

<<<<<<< HEAD
	switch (fmt & SND_SOC_DAIFMT_MASTER_MASK) {
	case SND_SOC_DAIFMT_CBS_CFS:
		pxa_i2s.master = 1;
		break;
	case SND_SOC_DAIFMT_CBM_CFS:
=======
	switch (fmt & SND_SOC_DAIFMT_CLOCK_PROVIDER_MASK) {
	case SND_SOC_DAIFMT_BP_FP:
		pxa_i2s.master = 1;
		break;
	case SND_SOC_DAIFMT_BC_FP:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		pxa_i2s.master = 0;
		break;
	default:
		break;
	}
	return 0;
}

static int pxa2xx_i2s_set_dai_sysclk(struct snd_soc_dai *cpu_dai,
		int clk_id, unsigned int freq, int dir)
{
	if (clk_id != PXA2XX_I2S_SYSCLK)
		return -ENODEV;

	return 0;
}

static int pxa2xx_i2s_hw_params(struct snd_pcm_substream *substream,
				struct snd_pcm_hw_params *params,
				struct snd_soc_dai *dai)
{
<<<<<<< HEAD
	struct pxa2xx_pcm_dma_params *dma_data;

	BUG_ON(IS_ERR(clk_i2s));
	clk_enable(clk_i2s);
=======
	struct snd_dmaengine_dai_dma_data *dma_data;

	if (WARN_ON(IS_ERR(clk_i2s)))
		return -EINVAL;
	clk_prepare_enable(clk_i2s);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	clk_ena = 1;
	pxa_i2s_wait();

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
		dma_data = &pxa2xx_i2s_pcm_stereo_out;
	else
		dma_data = &pxa2xx_i2s_pcm_stereo_in;

	snd_soc_dai_set_dma_data(dai, substream, dma_data);

	/* is port used by another stream */
	if (!(SACR0 & SACR0_ENB)) {
<<<<<<< HEAD
		SACR0 = 0;
		if (pxa_i2s.master)
			SACR0 |= SACR0_BCKD;

		SACR0 |= SACR0_RFTH(14) | SACR0_TFTH(1);
		SACR1 |= pxa_i2s.fmt;
	}
	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
		SAIMR |= SAIMR_TFS;
	else
		SAIMR |= SAIMR_RFS;

	switch (params_rate(params)) {
	case 8000:
		SADIV = 0x48;
		break;
	case 11025:
		SADIV = 0x34;
		break;
	case 16000:
		SADIV = 0x24;
		break;
	case 22050:
		SADIV = 0x1a;
		break;
	case 44100:
		SADIV = 0xd;
		break;
	case 48000:
		SADIV = 0xc;
		break;
	case 96000: /* not in manual and possibly slightly inaccurate */
		SADIV = 0x6;
=======
		writel(0, i2s_reg_base + SACR0);
		if (pxa_i2s.master)
			writel(readl(i2s_reg_base + SACR0) | (SACR0_BCKD), i2s_reg_base + SACR0);

		writel(readl(i2s_reg_base + SACR0) | (SACR0_RFTH(14) | SACR0_TFTH(1)), i2s_reg_base + SACR0);
		writel(readl(i2s_reg_base + SACR1) | (pxa_i2s.fmt), i2s_reg_base + SACR1);
	}
	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
		writel(readl(i2s_reg_base + SAIMR) | (SAIMR_TFS), i2s_reg_base + SAIMR);
	else
		writel(readl(i2s_reg_base + SAIMR) | (SAIMR_RFS), i2s_reg_base + SAIMR);

	switch (params_rate(params)) {
	case 8000:
		writel(0x48, i2s_reg_base + SADIV);
		break;
	case 11025:
		writel(0x34, i2s_reg_base + SADIV);
		break;
	case 16000:
		writel(0x24, i2s_reg_base + SADIV);
		break;
	case 22050:
		writel(0x1a, i2s_reg_base + SADIV);
		break;
	case 44100:
		writel(0xd, i2s_reg_base + SADIV);
		break;
	case 48000:
		writel(0xc, i2s_reg_base + SADIV);
		break;
	case 96000: /* not in manual and possibly slightly inaccurate */
		writel(0x6, i2s_reg_base + SADIV);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	}

	return 0;
}

static int pxa2xx_i2s_trigger(struct snd_pcm_substream *substream, int cmd,
			      struct snd_soc_dai *dai)
{
	int ret = 0;

	switch (cmd) {
	case SNDRV_PCM_TRIGGER_START:
		if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
<<<<<<< HEAD
			SACR1 &= ~SACR1_DRPL;
		else
			SACR1 &= ~SACR1_DREC;
		SACR0 |= SACR0_ENB;
=======
			writel(readl(i2s_reg_base + SACR1) & (~SACR1_DRPL), i2s_reg_base + SACR1);
		else
			writel(readl(i2s_reg_base + SACR1) & (~SACR1_DREC), i2s_reg_base + SACR1);
		writel(readl(i2s_reg_base + SACR0) | (SACR0_ENB), i2s_reg_base + SACR0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case SNDRV_PCM_TRIGGER_RESUME:
	case SNDRV_PCM_TRIGGER_PAUSE_RELEASE:
	case SNDRV_PCM_TRIGGER_STOP:
	case SNDRV_PCM_TRIGGER_SUSPEND:
	case SNDRV_PCM_TRIGGER_PAUSE_PUSH:
		break;
	default:
		ret = -EINVAL;
	}

	return ret;
}

static void pxa2xx_i2s_shutdown(struct snd_pcm_substream *substream,
				struct snd_soc_dai *dai)
{
	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
<<<<<<< HEAD
		SACR1 |= SACR1_DRPL;
		SAIMR &= ~SAIMR_TFS;
	} else {
		SACR1 |= SACR1_DREC;
		SAIMR &= ~SAIMR_RFS;
	}

	if ((SACR1 & (SACR1_DREC | SACR1_DRPL)) == (SACR1_DREC | SACR1_DRPL)) {
		SACR0 &= ~SACR0_ENB;
		pxa_i2s_wait();
		if (clk_ena) {
			clk_disable(clk_i2s);
=======
		writel(readl(i2s_reg_base + SACR1) | (SACR1_DRPL), i2s_reg_base + SACR1);
		writel(readl(i2s_reg_base + SAIMR) & (~SAIMR_TFS), i2s_reg_base + SAIMR);
	} else {
		writel(readl(i2s_reg_base + SACR1) | (SACR1_DREC), i2s_reg_base + SACR1);
		writel(readl(i2s_reg_base + SAIMR) & (~SAIMR_RFS), i2s_reg_base + SAIMR);
	}

	if ((readl(i2s_reg_base + SACR1) & (SACR1_DREC | SACR1_DRPL)) == (SACR1_DREC | SACR1_DRPL)) {
		writel(readl(i2s_reg_base + SACR0) & (~SACR0_ENB), i2s_reg_base + SACR0);
		pxa_i2s_wait();
		if (clk_ena) {
			clk_disable_unprepare(clk_i2s);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			clk_ena = 0;
		}
	}
}

#ifdef CONFIG_PM
<<<<<<< HEAD
static int pxa2xx_i2s_suspend(struct snd_soc_dai *dai)
{
	/* store registers */
	pxa_i2s.sacr0 = SACR0;
	pxa_i2s.sacr1 = SACR1;
	pxa_i2s.saimr = SAIMR;
	pxa_i2s.sadiv = SADIV;

	/* deactivate link */
	SACR0 &= ~SACR0_ENB;
=======
static int pxa2xx_soc_pcm_suspend(struct snd_soc_component *component)
{
	/* store registers */
	pxa_i2s.sacr0 = readl(i2s_reg_base + SACR0);
	pxa_i2s.sacr1 = readl(i2s_reg_base + SACR1);
	pxa_i2s.saimr = readl(i2s_reg_base + SAIMR);
	pxa_i2s.sadiv = readl(i2s_reg_base + SADIV);

	/* deactivate link */
	writel(readl(i2s_reg_base + SACR0) & (~SACR0_ENB), i2s_reg_base + SACR0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	pxa_i2s_wait();
	return 0;
}

<<<<<<< HEAD
static int pxa2xx_i2s_resume(struct snd_soc_dai *dai)
{
	pxa_i2s_wait();

	SACR0 = pxa_i2s.sacr0 & ~SACR0_ENB;
	SACR1 = pxa_i2s.sacr1;
	SAIMR = pxa_i2s.saimr;
	SADIV = pxa_i2s.sadiv;

	SACR0 = pxa_i2s.sacr0;
=======
static int pxa2xx_soc_pcm_resume(struct snd_soc_component *component)
{
	pxa_i2s_wait();

	writel(pxa_i2s.sacr0 & ~SACR0_ENB, i2s_reg_base + SACR0);
	writel(pxa_i2s.sacr1, i2s_reg_base + SACR1);
	writel(pxa_i2s.saimr, i2s_reg_base + SAIMR);
	writel(pxa_i2s.sadiv, i2s_reg_base + SADIV);

	writel(pxa_i2s.sacr0, i2s_reg_base + SACR0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

#else
<<<<<<< HEAD
#define pxa2xx_i2s_suspend	NULL
#define pxa2xx_i2s_resume	NULL
=======
#define pxa2xx_soc_pcm_suspend	NULL
#define pxa2xx_soc_pcm_resume	NULL
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

static int pxa2xx_i2s_probe(struct snd_soc_dai *dai)
{
	clk_i2s = clk_get(dai->dev, "I2SCLK");
	if (IS_ERR(clk_i2s))
		return PTR_ERR(clk_i2s);

	/*
	 * PXA Developer's Manual:
	 * If SACR0[ENB] is toggled in the middle of a normal operation,
	 * the SACR0[RST] bit must also be set and cleared to reset all
	 * I2S controller registers.
	 */
<<<<<<< HEAD
	SACR0 = SACR0_RST;
	SACR0 = 0;
	/* Make sure RPL and REC are disabled */
	SACR1 = SACR1_DRPL | SACR1_DREC;
	/* Along with FIFO servicing */
	SAIMR &= ~(SAIMR_RFS | SAIMR_TFS);
=======
	writel(SACR0_RST, i2s_reg_base + SACR0);
	writel(0, i2s_reg_base + SACR0);
	/* Make sure RPL and REC are disabled */
	writel(SACR1_DRPL | SACR1_DREC, i2s_reg_base + SACR1);
	/* Along with FIFO servicing */
	writel(readl(i2s_reg_base + SAIMR) & (~(SAIMR_RFS | SAIMR_TFS)), i2s_reg_base + SAIMR);

	snd_soc_dai_init_dma_data(dai, &pxa2xx_i2s_pcm_stereo_out,
		&pxa2xx_i2s_pcm_stereo_in);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static int  pxa2xx_i2s_remove(struct snd_soc_dai *dai)
{
	clk_put(clk_i2s);
	clk_i2s = ERR_PTR(-ENOENT);
	return 0;
}

#define PXA2XX_I2S_RATES (SNDRV_PCM_RATE_8000 | SNDRV_PCM_RATE_11025 |\
		SNDRV_PCM_RATE_16000 | SNDRV_PCM_RATE_22050 | SNDRV_PCM_RATE_44100 | \
		SNDRV_PCM_RATE_48000 | SNDRV_PCM_RATE_96000)

static const struct snd_soc_dai_ops pxa_i2s_dai_ops = {
<<<<<<< HEAD
=======
	.probe		= pxa2xx_i2s_probe,
	.remove		= pxa2xx_i2s_remove,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.startup	= pxa2xx_i2s_startup,
	.shutdown	= pxa2xx_i2s_shutdown,
	.trigger	= pxa2xx_i2s_trigger,
	.hw_params	= pxa2xx_i2s_hw_params,
	.set_fmt	= pxa2xx_i2s_set_dai_fmt,
	.set_sysclk	= pxa2xx_i2s_set_dai_sysclk,
};

static struct snd_soc_dai_driver pxa_i2s_dai = {
<<<<<<< HEAD
	.probe = pxa2xx_i2s_probe,
	.remove = pxa2xx_i2s_remove,
	.suspend = pxa2xx_i2s_suspend,
	.resume = pxa2xx_i2s_resume,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.playback = {
		.channels_min = 2,
		.channels_max = 2,
		.rates = PXA2XX_I2S_RATES,
		.formats = SNDRV_PCM_FMTBIT_S16_LE,},
	.capture = {
		.channels_min = 2,
		.channels_max = 2,
		.rates = PXA2XX_I2S_RATES,
		.formats = SNDRV_PCM_FMTBIT_S16_LE,},
	.ops = &pxa_i2s_dai_ops,
<<<<<<< HEAD
	.symmetric_rates = 1,
=======
	.symmetric_rate = 1,
};

static const struct snd_soc_component_driver pxa_i2s_component = {
	.name			= "pxa-i2s",
	.pcm_construct		= pxa2xx_soc_pcm_new,
	.open			= pxa2xx_soc_pcm_open,
	.close			= pxa2xx_soc_pcm_close,
	.hw_params		= pxa2xx_soc_pcm_hw_params,
	.prepare		= pxa2xx_soc_pcm_prepare,
	.trigger		= pxa2xx_soc_pcm_trigger,
	.pointer		= pxa2xx_soc_pcm_pointer,
	.suspend		= pxa2xx_soc_pcm_suspend,
	.resume			= pxa2xx_soc_pcm_resume,
	.legacy_dai_naming	= 1,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static int pxa2xx_i2s_drv_probe(struct platform_device *pdev)
{
<<<<<<< HEAD
	return snd_soc_register_dai(&pdev->dev, &pxa_i2s_dai);
}

static int __devexit pxa2xx_i2s_drv_remove(struct platform_device *pdev)
{
	snd_soc_unregister_dai(&pdev->dev);
	return 0;
=======
	struct resource *res;

	i2s_reg_base = devm_platform_get_and_ioremap_resource(pdev, 0, &res);
	if (IS_ERR(i2s_reg_base))
		return PTR_ERR(i2s_reg_base);

	pxa2xx_i2s_pcm_stereo_out.addr = res->start + SADR;
	pxa2xx_i2s_pcm_stereo_in.addr = res->start + SADR;

	return devm_snd_soc_register_component(&pdev->dev, &pxa_i2s_component,
					       &pxa_i2s_dai, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct platform_driver pxa2xx_i2s_driver = {
	.probe = pxa2xx_i2s_drv_probe,
<<<<<<< HEAD
	.remove = __devexit_p(pxa2xx_i2s_drv_remove),

	.driver = {
		.name = "pxa2xx-i2s",
		.owner = THIS_MODULE,
=======

	.driver = {
		.name = "pxa2xx-i2s",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
};

static int __init pxa2xx_i2s_init(void)
{
	clk_i2s = ERR_PTR(-ENOENT);
	return platform_driver_register(&pxa2xx_i2s_driver);
}

static void __exit pxa2xx_i2s_exit(void)
{
	platform_driver_unregister(&pxa2xx_i2s_driver);
}

module_init(pxa2xx_i2s_init);
module_exit(pxa2xx_i2s_exit);

/* Module information */
MODULE_AUTHOR("Liam Girdwood, lrg@slimlogic.co.uk");
MODULE_DESCRIPTION("pxa2xx I2S SoC Interface");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:pxa2xx-i2s");
