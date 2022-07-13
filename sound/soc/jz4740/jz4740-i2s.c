<<<<<<< HEAD
/*
 *  Copyright (C) 2010, Lars-Peter Clausen <lars@metafoo.de>
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under  the terms of the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the License, or (at your
 *  option) any later version.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 *  Copyright (C) 2010, Lars-Peter Clausen <lars@metafoo.de>
 */

#include <linux/bitfield.h>
#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/dma-mapping.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/init.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/module.h>
<<<<<<< HEAD
#include <linux/platform_device.h>
#include <linux/slab.h>

#include <linux/clk.h>
#include <linux/delay.h>

#include <linux/dma-mapping.h>

=======
#include <linux/mod_devicetable.h>
#include <linux/platform_device.h>
#include <linux/regmap.h>
#include <linux/slab.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/initval.h>
<<<<<<< HEAD

#include "jz4740-i2s.h"
#include "jz4740-pcm.h"
=======
#include <sound/dmaengine_pcm.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define JZ_REG_AIC_CONF		0x00
#define JZ_REG_AIC_CTRL		0x04
#define JZ_REG_AIC_I2S_FMT	0x10
#define JZ_REG_AIC_FIFO_STATUS	0x14
#define JZ_REG_AIC_I2S_STATUS	0x1c
#define JZ_REG_AIC_CLK_DIV	0x30
#define JZ_REG_AIC_FIFO		0x34

<<<<<<< HEAD
#define JZ_AIC_CONF_FIFO_RX_THRESHOLD_MASK (0xf << 12)
#define JZ_AIC_CONF_FIFO_TX_THRESHOLD_MASK (0xf <<  8)
#define JZ_AIC_CONF_OVERFLOW_PLAY_LAST BIT(6)
#define JZ_AIC_CONF_INTERNAL_CODEC BIT(5)
#define JZ_AIC_CONF_I2S BIT(4)
#define JZ_AIC_CONF_RESET BIT(3)
#define JZ_AIC_CONF_BIT_CLK_MASTER BIT(2)
#define JZ_AIC_CONF_SYNC_CLK_MASTER BIT(1)
#define JZ_AIC_CONF_ENABLE BIT(0)

#define JZ_AIC_CONF_FIFO_RX_THRESHOLD_OFFSET 12
#define JZ_AIC_CONF_FIFO_TX_THRESHOLD_OFFSET 8

#define JZ_AIC_CTRL_OUTPUT_SAMPLE_SIZE_MASK (0x7 << 19)
#define JZ_AIC_CTRL_INPUT_SAMPLE_SIZE_MASK (0x7 << 16)
#define JZ_AIC_CTRL_ENABLE_RX_DMA BIT(15)
#define JZ_AIC_CTRL_ENABLE_TX_DMA BIT(14)
#define JZ_AIC_CTRL_MONO_TO_STEREO BIT(11)
#define JZ_AIC_CTRL_SWITCH_ENDIANNESS BIT(10)
#define JZ_AIC_CTRL_SIGNED_TO_UNSIGNED BIT(9)
#define JZ_AIC_CTRL_FLUSH		BIT(8)
#define JZ_AIC_CTRL_ENABLE_ROR_INT BIT(6)
#define JZ_AIC_CTRL_ENABLE_TUR_INT BIT(5)
#define JZ_AIC_CTRL_ENABLE_RFS_INT BIT(4)
#define JZ_AIC_CTRL_ENABLE_TFS_INT BIT(3)
#define JZ_AIC_CTRL_ENABLE_LOOPBACK BIT(2)
#define JZ_AIC_CTRL_ENABLE_PLAYBACK BIT(1)
#define JZ_AIC_CTRL_ENABLE_CAPTURE BIT(0)

#define JZ_AIC_CTRL_OUTPUT_SAMPLE_SIZE_OFFSET 19
#define JZ_AIC_CTRL_INPUT_SAMPLE_SIZE_OFFSET  16

#define JZ_AIC_I2S_FMT_DISABLE_BIT_CLK BIT(12)
#define JZ_AIC_I2S_FMT_ENABLE_SYS_CLK BIT(4)
#define JZ_AIC_I2S_FMT_MSB BIT(0)

#define JZ_AIC_I2S_STATUS_BUSY BIT(2)

#define JZ_AIC_CLK_DIV_MASK 0xf

struct jz4740_i2s {
	struct resource *mem;
	void __iomem *base;
	dma_addr_t phys_base;
=======
#define JZ_AIC_CONF_OVERFLOW_PLAY_LAST	BIT(6)
#define JZ_AIC_CONF_INTERNAL_CODEC	BIT(5)
#define JZ_AIC_CONF_I2S			BIT(4)
#define JZ_AIC_CONF_RESET		BIT(3)
#define JZ_AIC_CONF_BIT_CLK_MASTER	BIT(2)
#define JZ_AIC_CONF_SYNC_CLK_MASTER	BIT(1)
#define JZ_AIC_CONF_ENABLE		BIT(0)

#define JZ_AIC_CTRL_OUTPUT_SAMPLE_SIZE	GENMASK(21, 19)
#define JZ_AIC_CTRL_INPUT_SAMPLE_SIZE	GENMASK(18, 16)
#define JZ_AIC_CTRL_ENABLE_RX_DMA	BIT(15)
#define JZ_AIC_CTRL_ENABLE_TX_DMA	BIT(14)
#define JZ_AIC_CTRL_MONO_TO_STEREO	BIT(11)
#define JZ_AIC_CTRL_SWITCH_ENDIANNESS	BIT(10)
#define JZ_AIC_CTRL_SIGNED_TO_UNSIGNED	BIT(9)
#define JZ_AIC_CTRL_TFLUSH		BIT(8)
#define JZ_AIC_CTRL_RFLUSH		BIT(7)
#define JZ_AIC_CTRL_ENABLE_ROR_INT	BIT(6)
#define JZ_AIC_CTRL_ENABLE_TUR_INT	BIT(5)
#define JZ_AIC_CTRL_ENABLE_RFS_INT	BIT(4)
#define JZ_AIC_CTRL_ENABLE_TFS_INT	BIT(3)
#define JZ_AIC_CTRL_ENABLE_LOOPBACK	BIT(2)
#define JZ_AIC_CTRL_ENABLE_PLAYBACK	BIT(1)
#define JZ_AIC_CTRL_ENABLE_CAPTURE	BIT(0)

#define JZ_AIC_I2S_FMT_DISABLE_BIT_CLK	BIT(12)
#define JZ_AIC_I2S_FMT_DISABLE_BIT_ICLK	BIT(13)
#define JZ_AIC_I2S_FMT_ENABLE_SYS_CLK	BIT(4)
#define JZ_AIC_I2S_FMT_MSB		BIT(0)

#define JZ_AIC_I2S_STATUS_BUSY		BIT(2)

struct i2s_soc_info {
	struct snd_soc_dai_driver *dai;

	struct reg_field field_rx_fifo_thresh;
	struct reg_field field_tx_fifo_thresh;
	struct reg_field field_i2sdiv_capture;
	struct reg_field field_i2sdiv_playback;

	bool shared_fifo_flush;
};

struct jz4740_i2s {
	struct regmap *regmap;

	struct regmap_field *field_rx_fifo_thresh;
	struct regmap_field *field_tx_fifo_thresh;
	struct regmap_field *field_i2sdiv_capture;
	struct regmap_field *field_i2sdiv_playback;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	struct clk *clk_aic;
	struct clk *clk_i2s;

<<<<<<< HEAD
	struct jz4740_pcm_config pcm_config_playback;
	struct jz4740_pcm_config pcm_config_capture;
};

static inline uint32_t jz4740_i2s_read(const struct jz4740_i2s *i2s,
	unsigned int reg)
{
	return readl(i2s->base + reg);
}

static inline void jz4740_i2s_write(const struct jz4740_i2s *i2s,
	unsigned int reg, uint32_t value)
{
	writel(value, i2s->base + reg);
}

=======
	struct snd_dmaengine_dai_dma_data playback_dma_data;
	struct snd_dmaengine_dai_dma_data capture_dma_data;

	const struct i2s_soc_info *soc_info;
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int jz4740_i2s_startup(struct snd_pcm_substream *substream,
	struct snd_soc_dai *dai)
{
	struct jz4740_i2s *i2s = snd_soc_dai_get_drvdata(dai);
<<<<<<< HEAD
	uint32_t conf, ctrl;

	if (dai->active)
		return 0;

	ctrl = jz4740_i2s_read(i2s, JZ_REG_AIC_CTRL);
	ctrl |= JZ_AIC_CTRL_FLUSH;
	jz4740_i2s_write(i2s, JZ_REG_AIC_CTRL, ctrl);

	clk_enable(i2s->clk_i2s);

	conf = jz4740_i2s_read(i2s, JZ_REG_AIC_CONF);
	conf |= JZ_AIC_CONF_ENABLE;
	jz4740_i2s_write(i2s, JZ_REG_AIC_CONF, conf);

=======
	int ret;

	/*
	 * When we can flush FIFOs independently, only flush the FIFO
	 * that is starting up. We can do this when the DAI is active
	 * because it does not disturb other active substreams.
	 */
	if (!i2s->soc_info->shared_fifo_flush) {
		if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
			regmap_set_bits(i2s->regmap, JZ_REG_AIC_CTRL, JZ_AIC_CTRL_TFLUSH);
		else
			regmap_set_bits(i2s->regmap, JZ_REG_AIC_CTRL, JZ_AIC_CTRL_RFLUSH);
	}

	if (snd_soc_dai_active(dai))
		return 0;

	/*
	 * When there is a shared flush bit for both FIFOs, the TFLUSH
	 * bit flushes both FIFOs. Flushing while the DAI is active would
	 * cause FIFO underruns in other active substreams so we have to
	 * guard this behind the snd_soc_dai_active() check.
	 */
	if (i2s->soc_info->shared_fifo_flush)
		regmap_set_bits(i2s->regmap, JZ_REG_AIC_CTRL, JZ_AIC_CTRL_TFLUSH);

	ret = clk_prepare_enable(i2s->clk_i2s);
	if (ret)
		return ret;

	regmap_set_bits(i2s->regmap, JZ_REG_AIC_CONF, JZ_AIC_CONF_ENABLE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static void jz4740_i2s_shutdown(struct snd_pcm_substream *substream,
	struct snd_soc_dai *dai)
{
	struct jz4740_i2s *i2s = snd_soc_dai_get_drvdata(dai);
<<<<<<< HEAD
	uint32_t conf;

	if (dai->active)
		return;

	conf = jz4740_i2s_read(i2s, JZ_REG_AIC_CONF);
	conf &= ~JZ_AIC_CONF_ENABLE;
	jz4740_i2s_write(i2s, JZ_REG_AIC_CONF, conf);

	clk_disable(i2s->clk_i2s);
=======

	if (snd_soc_dai_active(dai))
		return;

	regmap_clear_bits(i2s->regmap, JZ_REG_AIC_CONF, JZ_AIC_CONF_ENABLE);

	clk_disable_unprepare(i2s->clk_i2s);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int jz4740_i2s_trigger(struct snd_pcm_substream *substream, int cmd,
	struct snd_soc_dai *dai)
{
	struct jz4740_i2s *i2s = snd_soc_dai_get_drvdata(dai);
<<<<<<< HEAD

	uint32_t ctrl;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	uint32_t mask;

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
		mask = JZ_AIC_CTRL_ENABLE_PLAYBACK | JZ_AIC_CTRL_ENABLE_TX_DMA;
	else
		mask = JZ_AIC_CTRL_ENABLE_CAPTURE | JZ_AIC_CTRL_ENABLE_RX_DMA;

<<<<<<< HEAD
	ctrl = jz4740_i2s_read(i2s, JZ_REG_AIC_CTRL);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	switch (cmd) {
	case SNDRV_PCM_TRIGGER_START:
	case SNDRV_PCM_TRIGGER_RESUME:
	case SNDRV_PCM_TRIGGER_PAUSE_RELEASE:
<<<<<<< HEAD
		ctrl |= mask;
=======
		regmap_set_bits(i2s->regmap, JZ_REG_AIC_CTRL, mask);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case SNDRV_PCM_TRIGGER_STOP:
	case SNDRV_PCM_TRIGGER_SUSPEND:
	case SNDRV_PCM_TRIGGER_PAUSE_PUSH:
<<<<<<< HEAD
		ctrl &= ~mask;
=======
		regmap_clear_bits(i2s->regmap, JZ_REG_AIC_CTRL, mask);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	default:
		return -EINVAL;
	}

<<<<<<< HEAD
	jz4740_i2s_write(i2s, JZ_REG_AIC_CTRL, ctrl);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static int jz4740_i2s_set_fmt(struct snd_soc_dai *dai, unsigned int fmt)
{
	struct jz4740_i2s *i2s = snd_soc_dai_get_drvdata(dai);
<<<<<<< HEAD

	uint32_t format = 0;
	uint32_t conf;

	conf = jz4740_i2s_read(i2s, JZ_REG_AIC_CONF);

	conf &= ~(JZ_AIC_CONF_BIT_CLK_MASTER | JZ_AIC_CONF_SYNC_CLK_MASTER);

	switch (fmt & SND_SOC_DAIFMT_MASTER_MASK) {
	case SND_SOC_DAIFMT_CBS_CFS:
		conf |= JZ_AIC_CONF_BIT_CLK_MASTER | JZ_AIC_CONF_SYNC_CLK_MASTER;
		format |= JZ_AIC_I2S_FMT_ENABLE_SYS_CLK;
		break;
	case SND_SOC_DAIFMT_CBM_CFS:
		conf |= JZ_AIC_CONF_SYNC_CLK_MASTER;
		break;
	case SND_SOC_DAIFMT_CBS_CFM:
		conf |= JZ_AIC_CONF_BIT_CLK_MASTER;
		break;
	case SND_SOC_DAIFMT_CBM_CFM:
=======
	const unsigned int conf_mask = JZ_AIC_CONF_BIT_CLK_MASTER |
				       JZ_AIC_CONF_SYNC_CLK_MASTER;
	unsigned int conf = 0, format = 0;

	switch (fmt & SND_SOC_DAIFMT_CLOCK_PROVIDER_MASK) {
	case SND_SOC_DAIFMT_BP_FP:
		conf |= JZ_AIC_CONF_BIT_CLK_MASTER | JZ_AIC_CONF_SYNC_CLK_MASTER;
		format |= JZ_AIC_I2S_FMT_ENABLE_SYS_CLK;
		break;
	case SND_SOC_DAIFMT_BC_FP:
		conf |= JZ_AIC_CONF_SYNC_CLK_MASTER;
		break;
	case SND_SOC_DAIFMT_BP_FC:
		conf |= JZ_AIC_CONF_BIT_CLK_MASTER;
		break;
	case SND_SOC_DAIFMT_BC_FC:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	default:
		return -EINVAL;
	}

	switch (fmt & SND_SOC_DAIFMT_FORMAT_MASK) {
	case SND_SOC_DAIFMT_MSB:
		format |= JZ_AIC_I2S_FMT_MSB;
		break;
	case SND_SOC_DAIFMT_I2S:
		break;
	default:
		return -EINVAL;
	}

	switch (fmt & SND_SOC_DAIFMT_INV_MASK) {
	case SND_SOC_DAIFMT_NB_NF:
		break;
	default:
		return -EINVAL;
	}

<<<<<<< HEAD
	jz4740_i2s_write(i2s, JZ_REG_AIC_CONF, conf);
	jz4740_i2s_write(i2s, JZ_REG_AIC_I2S_FMT, format);
=======
	regmap_update_bits(i2s->regmap, JZ_REG_AIC_CONF, conf_mask, conf);
	regmap_write(i2s->regmap, JZ_REG_AIC_I2S_FMT, format);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
=======
static int jz4740_i2s_get_i2sdiv(unsigned long mclk, unsigned long rate,
				 unsigned long i2sdiv_max)
{
	unsigned long div, rate1, rate2, err1, err2;

	div = mclk / (64 * rate);
	if (div == 0)
		div = 1;

	rate1 = mclk / (64 * div);
	rate2 = mclk / (64 * (div + 1));

	err1 = abs(rate1 - rate);
	err2 = abs(rate2 - rate);

	/*
	 * Choose the divider that produces the smallest error in the
	 * output rate and reject dividers with a 5% or higher error.
	 * In the event that both dividers are outside the acceptable
	 * error margin, reject the rate to prevent distorted audio.
	 * (The number 5% is arbitrary.)
	 */
	if (div <= i2sdiv_max && err1 <= err2 && err1 < rate/20)
		return div;
	if (div < i2sdiv_max && err2 < rate/20)
		return div + 1;

	return -EINVAL;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int jz4740_i2s_hw_params(struct snd_pcm_substream *substream,
	struct snd_pcm_hw_params *params, struct snd_soc_dai *dai)
{
	struct jz4740_i2s *i2s = snd_soc_dai_get_drvdata(dai);
<<<<<<< HEAD
	enum jz4740_dma_width dma_width;
	struct jz4740_pcm_config *pcm_config;
	unsigned int sample_size;
	uint32_t ctrl;

	ctrl = jz4740_i2s_read(i2s, JZ_REG_AIC_CTRL);
=======
	struct regmap_field *div_field;
	unsigned long i2sdiv_max;
	unsigned int sample_size;
	uint32_t ctrl, conf;
	int div = 1;

	regmap_read(i2s->regmap, JZ_REG_AIC_CTRL, &ctrl);
	regmap_read(i2s->regmap, JZ_REG_AIC_CONF, &conf);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (params_format(params)) {
	case SNDRV_PCM_FORMAT_S8:
		sample_size = 0;
<<<<<<< HEAD
		dma_width = JZ4740_DMA_WIDTH_8BIT;
		break;
	case SNDRV_PCM_FORMAT_S16:
		sample_size = 1;
		dma_width = JZ4740_DMA_WIDTH_16BIT;
=======
		break;
	case SNDRV_PCM_FORMAT_S16_LE:
		sample_size = 1;
		break;
	case SNDRV_PCM_FORMAT_S20_LE:
		sample_size = 3;
		break;
	case SNDRV_PCM_FORMAT_S24_LE:
		sample_size = 4;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	default:
		return -EINVAL;
	}

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
<<<<<<< HEAD
		ctrl &= ~JZ_AIC_CTRL_OUTPUT_SAMPLE_SIZE_MASK;
		ctrl |= sample_size << JZ_AIC_CTRL_OUTPUT_SAMPLE_SIZE_OFFSET;
=======
		ctrl &= ~JZ_AIC_CTRL_OUTPUT_SAMPLE_SIZE;
		ctrl |= FIELD_PREP(JZ_AIC_CTRL_OUTPUT_SAMPLE_SIZE, sample_size);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (params_channels(params) == 1)
			ctrl |= JZ_AIC_CTRL_MONO_TO_STEREO;
		else
			ctrl &= ~JZ_AIC_CTRL_MONO_TO_STEREO;

<<<<<<< HEAD
		pcm_config = &i2s->pcm_config_playback;
		pcm_config->dma_config.dst_width = dma_width;

	} else {
		ctrl &= ~JZ_AIC_CTRL_INPUT_SAMPLE_SIZE_MASK;
		ctrl |= sample_size << JZ_AIC_CTRL_INPUT_SAMPLE_SIZE_OFFSET;

		pcm_config = &i2s->pcm_config_capture;
		pcm_config->dma_config.src_width = dma_width;
	}

	jz4740_i2s_write(i2s, JZ_REG_AIC_CTRL, ctrl);

	snd_soc_dai_set_dma_data(dai, substream, pcm_config);
=======
		div_field = i2s->field_i2sdiv_playback;
		i2sdiv_max = GENMASK(i2s->soc_info->field_i2sdiv_playback.msb,
				     i2s->soc_info->field_i2sdiv_playback.lsb);
	} else {
		ctrl &= ~JZ_AIC_CTRL_INPUT_SAMPLE_SIZE;
		ctrl |= FIELD_PREP(JZ_AIC_CTRL_INPUT_SAMPLE_SIZE, sample_size);

		div_field = i2s->field_i2sdiv_capture;
		i2sdiv_max = GENMASK(i2s->soc_info->field_i2sdiv_capture.msb,
				     i2s->soc_info->field_i2sdiv_capture.lsb);
	}

	/*
	 * Only calculate I2SDIV if we're supplying the bit or frame clock.
	 * If the codec is supplying both clocks then the divider output is
	 * unused, and we don't want it to limit the allowed sample rates.
	 */
	if (conf & (JZ_AIC_CONF_BIT_CLK_MASTER | JZ_AIC_CONF_SYNC_CLK_MASTER)) {
		div = jz4740_i2s_get_i2sdiv(clk_get_rate(i2s->clk_i2s),
					    params_rate(params), i2sdiv_max);
		if (div < 0)
			return div;
	}

	regmap_write(i2s->regmap, JZ_REG_AIC_CTRL, ctrl);
	regmap_field_write(div_field, div - 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static int jz4740_i2s_set_sysclk(struct snd_soc_dai *dai, int clk_id,
	unsigned int freq, int dir)
{
	struct jz4740_i2s *i2s = snd_soc_dai_get_drvdata(dai);
	struct clk *parent;
	int ret = 0;

	switch (clk_id) {
	case JZ4740_I2S_CLKSRC_EXT:
		parent = clk_get(NULL, "ext");
		clk_set_parent(i2s->clk_i2s, parent);
		break;
	case JZ4740_I2S_CLKSRC_PLL:
		parent = clk_get(NULL, "pll half");
		clk_set_parent(i2s->clk_i2s, parent);
		ret = clk_set_rate(i2s->clk_i2s, freq);
		break;
	default:
		return -EINVAL;
	}
	clk_put(parent);

	return ret;
}

static int jz4740_i2s_suspend(struct snd_soc_dai *dai)
{
	struct jz4740_i2s *i2s = snd_soc_dai_get_drvdata(dai);
	uint32_t conf;

	if (dai->active) {
		conf = jz4740_i2s_read(i2s, JZ_REG_AIC_CONF);
		conf &= ~JZ_AIC_CONF_ENABLE;
		jz4740_i2s_write(i2s, JZ_REG_AIC_CONF, conf);

		clk_disable(i2s->clk_i2s);
	}

	clk_disable(i2s->clk_aic);

	return 0;
}

static int jz4740_i2s_resume(struct snd_soc_dai *dai)
{
	struct jz4740_i2s *i2s = snd_soc_dai_get_drvdata(dai);
	uint32_t conf;

	clk_enable(i2s->clk_aic);

	if (dai->active) {
		clk_enable(i2s->clk_i2s);

		conf = jz4740_i2s_read(i2s, JZ_REG_AIC_CONF);
		conf |= JZ_AIC_CONF_ENABLE;
		jz4740_i2s_write(i2s, JZ_REG_AIC_CONF, conf);
	}

	return 0;
}

static void jz4740_i2c_init_pcm_config(struct jz4740_i2s *i2s)
{
	struct jz4740_dma_config *dma_config;

	/* Playback */
	dma_config = &i2s->pcm_config_playback.dma_config;
	dma_config->src_width = JZ4740_DMA_WIDTH_32BIT,
	dma_config->transfer_size = JZ4740_DMA_TRANSFER_SIZE_16BYTE;
	dma_config->request_type = JZ4740_DMA_TYPE_AIC_TRANSMIT;
	dma_config->flags = JZ4740_DMA_SRC_AUTOINC;
	dma_config->mode = JZ4740_DMA_MODE_SINGLE;
	i2s->pcm_config_playback.fifo_addr = i2s->phys_base + JZ_REG_AIC_FIFO;

	/* Capture */
	dma_config = &i2s->pcm_config_capture.dma_config;
	dma_config->dst_width = JZ4740_DMA_WIDTH_32BIT,
	dma_config->transfer_size = JZ4740_DMA_TRANSFER_SIZE_16BYTE;
	dma_config->request_type = JZ4740_DMA_TYPE_AIC_RECEIVE;
	dma_config->flags = JZ4740_DMA_DST_AUTOINC;
	dma_config->mode = JZ4740_DMA_MODE_SINGLE;
	i2s->pcm_config_capture.fifo_addr = i2s->phys_base + JZ_REG_AIC_FIFO;
}

static int jz4740_i2s_dai_probe(struct snd_soc_dai *dai)
{
	struct jz4740_i2s *i2s = snd_soc_dai_get_drvdata(dai);
	uint32_t conf;

	clk_enable(i2s->clk_aic);

	jz4740_i2c_init_pcm_config(i2s);

	conf = (7 << JZ_AIC_CONF_FIFO_RX_THRESHOLD_OFFSET) |
		(8 << JZ_AIC_CONF_FIFO_TX_THRESHOLD_OFFSET) |
		JZ_AIC_CONF_OVERFLOW_PLAY_LAST |
		JZ_AIC_CONF_I2S |
		JZ_AIC_CONF_INTERNAL_CODEC;

	jz4740_i2s_write(i2s, JZ_REG_AIC_CONF, JZ_AIC_CONF_RESET);
	jz4740_i2s_write(i2s, JZ_REG_AIC_CONF, conf);

	return 0;
}

static int jz4740_i2s_dai_remove(struct snd_soc_dai *dai)
{
	struct jz4740_i2s *i2s = snd_soc_dai_get_drvdata(dai);

	clk_disable(i2s->clk_aic);
=======
static int jz4740_i2s_dai_probe(struct snd_soc_dai *dai)
{
	struct jz4740_i2s *i2s = snd_soc_dai_get_drvdata(dai);

	snd_soc_dai_init_dma_data(dai, &i2s->playback_dma_data,
		&i2s->capture_dma_data);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static const struct snd_soc_dai_ops jz4740_i2s_dai_ops = {
<<<<<<< HEAD
=======
	.probe = jz4740_i2s_dai_probe,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.startup = jz4740_i2s_startup,
	.shutdown = jz4740_i2s_shutdown,
	.trigger = jz4740_i2s_trigger,
	.hw_params = jz4740_i2s_hw_params,
	.set_fmt = jz4740_i2s_set_fmt,
<<<<<<< HEAD
	.set_sysclk = jz4740_i2s_set_sysclk,
};

#define JZ4740_I2S_FMTS (SNDRV_PCM_FMTBIT_S8 | \
		SNDRV_PCM_FMTBIT_S16_LE)

static struct snd_soc_dai_driver jz4740_i2s_dai = {
	.probe = jz4740_i2s_dai_probe,
	.remove = jz4740_i2s_dai_remove,
	.playback = {
		.channels_min = 1,
		.channels_max = 2,
		.rates = SNDRV_PCM_RATE_8000_48000,
=======
};

#define JZ4740_I2S_FMTS (SNDRV_PCM_FMTBIT_S8 | \
			 SNDRV_PCM_FMTBIT_S16_LE | \
			 SNDRV_PCM_FMTBIT_S20_LE | \
			 SNDRV_PCM_FMTBIT_S24_LE)

static struct snd_soc_dai_driver jz4740_i2s_dai = {
	.playback = {
		.channels_min = 1,
		.channels_max = 2,
		.rates = SNDRV_PCM_RATE_CONTINUOUS,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.formats = JZ4740_I2S_FMTS,
	},
	.capture = {
		.channels_min = 2,
		.channels_max = 2,
<<<<<<< HEAD
		.rates = SNDRV_PCM_RATE_8000_48000,
		.formats = JZ4740_I2S_FMTS,
	},
	.symmetric_rates = 1,
	.ops = &jz4740_i2s_dai_ops,
	.suspend = jz4740_i2s_suspend,
	.resume = jz4740_i2s_resume,
};

static int __devinit jz4740_i2s_dev_probe(struct platform_device *pdev)
{
	struct jz4740_i2s *i2s;
	int ret;

	i2s = kzalloc(sizeof(*i2s), GFP_KERNEL);

	if (!i2s)
		return -ENOMEM;

	i2s->mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!i2s->mem) {
		ret = -ENOENT;
		goto err_free;
	}

	i2s->mem = request_mem_region(i2s->mem->start, resource_size(i2s->mem),
				pdev->name);
	if (!i2s->mem) {
		ret = -EBUSY;
		goto err_free;
	}

	i2s->base = ioremap_nocache(i2s->mem->start, resource_size(i2s->mem));
	if (!i2s->base) {
		ret = -EBUSY;
		goto err_release_mem_region;
	}

	i2s->phys_base = i2s->mem->start;

	i2s->clk_aic = clk_get(&pdev->dev, "aic");
	if (IS_ERR(i2s->clk_aic)) {
		ret = PTR_ERR(i2s->clk_aic);
		goto err_iounmap;
	}

	i2s->clk_i2s = clk_get(&pdev->dev, "i2s");
	if (IS_ERR(i2s->clk_i2s)) {
		ret = PTR_ERR(i2s->clk_i2s);
		goto err_clk_put_aic;
	}

	platform_set_drvdata(pdev, i2s);
	ret = snd_soc_register_dai(&pdev->dev, &jz4740_i2s_dai);

	if (ret) {
		dev_err(&pdev->dev, "Failed to register DAI\n");
		goto err_clk_put_i2s;
	}

	return 0;

err_clk_put_i2s:
	clk_put(i2s->clk_i2s);
err_clk_put_aic:
	clk_put(i2s->clk_aic);
err_iounmap:
	iounmap(i2s->base);
err_release_mem_region:
	release_mem_region(i2s->mem->start, resource_size(i2s->mem));
err_free:
	kfree(i2s);

	return ret;
}

static int __devexit jz4740_i2s_dev_remove(struct platform_device *pdev)
{
	struct jz4740_i2s *i2s = platform_get_drvdata(pdev);

	snd_soc_unregister_dai(&pdev->dev);

	clk_put(i2s->clk_i2s);
	clk_put(i2s->clk_aic);

	iounmap(i2s->base);
	release_mem_region(i2s->mem->start, resource_size(i2s->mem));

	platform_set_drvdata(pdev, NULL);
	kfree(i2s);

	return 0;
=======
		.rates = SNDRV_PCM_RATE_CONTINUOUS,
		.formats = JZ4740_I2S_FMTS,
	},
	.symmetric_rate = 1,
	.ops = &jz4740_i2s_dai_ops,
};

static const struct i2s_soc_info jz4740_i2s_soc_info = {
	.dai			= &jz4740_i2s_dai,
	.field_rx_fifo_thresh	= REG_FIELD(JZ_REG_AIC_CONF, 12, 15),
	.field_tx_fifo_thresh	= REG_FIELD(JZ_REG_AIC_CONF, 8, 11),
	.field_i2sdiv_capture	= REG_FIELD(JZ_REG_AIC_CLK_DIV, 0, 3),
	.field_i2sdiv_playback	= REG_FIELD(JZ_REG_AIC_CLK_DIV, 0, 3),
	.shared_fifo_flush	= true,
};

static const struct i2s_soc_info jz4760_i2s_soc_info = {
	.dai			= &jz4740_i2s_dai,
	.field_rx_fifo_thresh	= REG_FIELD(JZ_REG_AIC_CONF, 24, 27),
	.field_tx_fifo_thresh	= REG_FIELD(JZ_REG_AIC_CONF, 16, 20),
	.field_i2sdiv_capture	= REG_FIELD(JZ_REG_AIC_CLK_DIV, 0, 3),
	.field_i2sdiv_playback	= REG_FIELD(JZ_REG_AIC_CLK_DIV, 0, 3),
};

static const struct i2s_soc_info x1000_i2s_soc_info = {
	.dai = &jz4740_i2s_dai,
	.field_rx_fifo_thresh	= REG_FIELD(JZ_REG_AIC_CONF, 24, 27),
	.field_tx_fifo_thresh	= REG_FIELD(JZ_REG_AIC_CONF, 16, 20),
	.field_i2sdiv_capture	= REG_FIELD(JZ_REG_AIC_CLK_DIV, 0, 8),
	.field_i2sdiv_playback	= REG_FIELD(JZ_REG_AIC_CLK_DIV, 0, 8),
};

static struct snd_soc_dai_driver jz4770_i2s_dai = {
	.playback = {
		.channels_min = 1,
		.channels_max = 2,
		.rates = SNDRV_PCM_RATE_CONTINUOUS,
		.formats = JZ4740_I2S_FMTS,
	},
	.capture = {
		.channels_min = 2,
		.channels_max = 2,
		.rates = SNDRV_PCM_RATE_CONTINUOUS,
		.formats = JZ4740_I2S_FMTS,
	},
	.ops = &jz4740_i2s_dai_ops,
};

static const struct i2s_soc_info jz4770_i2s_soc_info = {
	.dai			= &jz4770_i2s_dai,
	.field_rx_fifo_thresh	= REG_FIELD(JZ_REG_AIC_CONF, 24, 27),
	.field_tx_fifo_thresh	= REG_FIELD(JZ_REG_AIC_CONF, 16, 20),
	.field_i2sdiv_capture	= REG_FIELD(JZ_REG_AIC_CLK_DIV, 8, 11),
	.field_i2sdiv_playback	= REG_FIELD(JZ_REG_AIC_CLK_DIV, 0, 3),
};

static const struct i2s_soc_info jz4780_i2s_soc_info = {
	.dai			= &jz4770_i2s_dai,
	.field_rx_fifo_thresh	= REG_FIELD(JZ_REG_AIC_CONF, 24, 27),
	.field_tx_fifo_thresh	= REG_FIELD(JZ_REG_AIC_CONF, 16, 20),
	.field_i2sdiv_capture	= REG_FIELD(JZ_REG_AIC_CLK_DIV, 8, 11),
	.field_i2sdiv_playback	= REG_FIELD(JZ_REG_AIC_CLK_DIV, 0, 3),
};

static int jz4740_i2s_suspend(struct snd_soc_component *component)
{
	struct jz4740_i2s *i2s = snd_soc_component_get_drvdata(component);

	if (snd_soc_component_active(component)) {
		regmap_clear_bits(i2s->regmap, JZ_REG_AIC_CONF, JZ_AIC_CONF_ENABLE);
		clk_disable_unprepare(i2s->clk_i2s);
	}

	clk_disable_unprepare(i2s->clk_aic);

	return 0;
}

static int jz4740_i2s_resume(struct snd_soc_component *component)
{
	struct jz4740_i2s *i2s = snd_soc_component_get_drvdata(component);
	int ret;

	ret = clk_prepare_enable(i2s->clk_aic);
	if (ret)
		return ret;

	if (snd_soc_component_active(component)) {
		ret = clk_prepare_enable(i2s->clk_i2s);
		if (ret) {
			clk_disable_unprepare(i2s->clk_aic);
			return ret;
		}

		regmap_set_bits(i2s->regmap, JZ_REG_AIC_CONF, JZ_AIC_CONF_ENABLE);
	}

	return 0;
}

static int jz4740_i2s_probe(struct snd_soc_component *component)
{
	struct jz4740_i2s *i2s = snd_soc_component_get_drvdata(component);
	int ret;

	ret = clk_prepare_enable(i2s->clk_aic);
	if (ret)
		return ret;

	regmap_write(i2s->regmap, JZ_REG_AIC_CONF, JZ_AIC_CONF_RESET);

	regmap_write(i2s->regmap, JZ_REG_AIC_CONF,
		     JZ_AIC_CONF_OVERFLOW_PLAY_LAST |
		     JZ_AIC_CONF_I2S | JZ_AIC_CONF_INTERNAL_CODEC);

	regmap_field_write(i2s->field_rx_fifo_thresh, 7);
	regmap_field_write(i2s->field_tx_fifo_thresh, 8);

	return 0;
}

static void jz4740_i2s_remove(struct snd_soc_component *component)
{
	struct jz4740_i2s *i2s = snd_soc_component_get_drvdata(component);

	clk_disable_unprepare(i2s->clk_aic);
}

static const struct snd_soc_component_driver jz4740_i2s_component = {
	.name			= "jz4740-i2s",
	.probe			= jz4740_i2s_probe,
	.remove			= jz4740_i2s_remove,
	.suspend		= jz4740_i2s_suspend,
	.resume			= jz4740_i2s_resume,
	.legacy_dai_naming	= 1,
};

static const struct of_device_id jz4740_of_matches[] = {
	{ .compatible = "ingenic,jz4740-i2s", .data = &jz4740_i2s_soc_info },
	{ .compatible = "ingenic,jz4760-i2s", .data = &jz4760_i2s_soc_info },
	{ .compatible = "ingenic,jz4770-i2s", .data = &jz4770_i2s_soc_info },
	{ .compatible = "ingenic,jz4780-i2s", .data = &jz4780_i2s_soc_info },
	{ .compatible = "ingenic,x1000-i2s", .data = &x1000_i2s_soc_info },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, jz4740_of_matches);

static int jz4740_i2s_init_regmap_fields(struct device *dev,
					 struct jz4740_i2s *i2s)
{
	i2s->field_rx_fifo_thresh =
		devm_regmap_field_alloc(dev, i2s->regmap,
					i2s->soc_info->field_rx_fifo_thresh);
	if (IS_ERR(i2s->field_rx_fifo_thresh))
		return PTR_ERR(i2s->field_rx_fifo_thresh);

	i2s->field_tx_fifo_thresh =
		devm_regmap_field_alloc(dev, i2s->regmap,
					i2s->soc_info->field_tx_fifo_thresh);
	if (IS_ERR(i2s->field_tx_fifo_thresh))
		return PTR_ERR(i2s->field_tx_fifo_thresh);

	i2s->field_i2sdiv_capture =
		devm_regmap_field_alloc(dev, i2s->regmap,
					i2s->soc_info->field_i2sdiv_capture);
	if (IS_ERR(i2s->field_i2sdiv_capture))
		return PTR_ERR(i2s->field_i2sdiv_capture);

	i2s->field_i2sdiv_playback =
		devm_regmap_field_alloc(dev, i2s->regmap,
					i2s->soc_info->field_i2sdiv_playback);
	if (IS_ERR(i2s->field_i2sdiv_playback))
		return PTR_ERR(i2s->field_i2sdiv_playback);

	return 0;
}

static const struct regmap_config jz4740_i2s_regmap_config = {
	.reg_bits	= 32,
	.reg_stride	= 4,
	.val_bits	= 32,
	.max_register	= JZ_REG_AIC_FIFO,
};

static int jz4740_i2s_dev_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct jz4740_i2s *i2s;
	struct resource *mem;
	void __iomem *regs;
	int ret;

	i2s = devm_kzalloc(dev, sizeof(*i2s), GFP_KERNEL);
	if (!i2s)
		return -ENOMEM;

	i2s->soc_info = device_get_match_data(dev);

	regs = devm_platform_get_and_ioremap_resource(pdev, 0, &mem);
	if (IS_ERR(regs))
		return PTR_ERR(regs);

	i2s->playback_dma_data.maxburst = 16;
	i2s->playback_dma_data.addr = mem->start + JZ_REG_AIC_FIFO;

	i2s->capture_dma_data.maxburst = 16;
	i2s->capture_dma_data.addr = mem->start + JZ_REG_AIC_FIFO;

	i2s->clk_aic = devm_clk_get(dev, "aic");
	if (IS_ERR(i2s->clk_aic))
		return PTR_ERR(i2s->clk_aic);

	i2s->clk_i2s = devm_clk_get(dev, "i2s");
	if (IS_ERR(i2s->clk_i2s))
		return PTR_ERR(i2s->clk_i2s);

	i2s->regmap = devm_regmap_init_mmio(&pdev->dev, regs,
					    &jz4740_i2s_regmap_config);
	if (IS_ERR(i2s->regmap))
		return PTR_ERR(i2s->regmap);

	ret = jz4740_i2s_init_regmap_fields(dev, i2s);
	if (ret)
		return ret;

	platform_set_drvdata(pdev, i2s);

	ret = devm_snd_soc_register_component(dev, &jz4740_i2s_component,
					      i2s->soc_info->dai, 1);
	if (ret)
		return ret;

	return devm_snd_dmaengine_pcm_register(dev, NULL,
		SND_DMAENGINE_PCM_FLAG_COMPAT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct platform_driver jz4740_i2s_driver = {
	.probe = jz4740_i2s_dev_probe,
<<<<<<< HEAD
	.remove = __devexit_p(jz4740_i2s_dev_remove),
	.driver = {
		.name = "jz4740-i2s",
		.owner = THIS_MODULE,
=======
	.driver = {
		.name = "jz4740-i2s",
		.of_match_table = jz4740_of_matches,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
};

module_platform_driver(jz4740_i2s_driver);

MODULE_AUTHOR("Lars-Peter Clausen, <lars@metafoo.de>");
MODULE_DESCRIPTION("Ingenic JZ4740 SoC I2S driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:jz4740-i2s");
