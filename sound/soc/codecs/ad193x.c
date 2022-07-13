<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * AD193X Audio Codec driver supporting AD1936/7/8/9
 *
 * Copyright 2010 Analog Devices Inc.
<<<<<<< HEAD
 *
 * Licensed under the GPL-2 or later.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/i2c.h>
#include <linux/spi/spi.h>
=======
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/regmap.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/slab.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/initval.h>
#include <sound/soc.h>
#include <sound/tlv.h>
<<<<<<< HEAD
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "ad193x.h"

/* codec private data */
struct ad193x_priv {
	struct regmap *regmap;
<<<<<<< HEAD
=======
	enum ad193x_type type;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int sysclk;
};

/*
 * AD193X volume/mute/de-emphasis etc. controls
 */
static const char * const ad193x_deemp[] = {"None", "48kHz", "44.1kHz", "32kHz"};

<<<<<<< HEAD
static const struct soc_enum ad193x_deemp_enum =
	SOC_ENUM_SINGLE(AD193X_DAC_CTRL2, 1, 4, ad193x_deemp);

static const DECLARE_TLV_DB_MINMAX(adau193x_tlv, -9563, 0);

=======
static SOC_ENUM_SINGLE_DECL(ad193x_deemp_enum, AD193X_DAC_CTRL2, 1,
			    ad193x_deemp);

static const DECLARE_TLV_DB_MINMAX(adau193x_tlv, -9563, 0);

static const unsigned int ad193x_sb[] = {32};

static struct snd_pcm_hw_constraint_list constr = {
	.list = ad193x_sb,
	.count = ARRAY_SIZE(ad193x_sb),
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static const struct snd_kcontrol_new ad193x_snd_controls[] = {
	/* DAC volume control */
	SOC_DOUBLE_R_TLV("DAC1 Volume", AD193X_DAC_L1_VOL,
			AD193X_DAC_R1_VOL, 0, 0xFF, 1, adau193x_tlv),
	SOC_DOUBLE_R_TLV("DAC2 Volume", AD193X_DAC_L2_VOL,
			AD193X_DAC_R2_VOL, 0, 0xFF, 1, adau193x_tlv),
	SOC_DOUBLE_R_TLV("DAC3 Volume", AD193X_DAC_L3_VOL,
			AD193X_DAC_R3_VOL, 0, 0xFF, 1, adau193x_tlv),
	SOC_DOUBLE_R_TLV("DAC4 Volume", AD193X_DAC_L4_VOL,
			AD193X_DAC_R4_VOL, 0, 0xFF, 1, adau193x_tlv),

<<<<<<< HEAD
	/* ADC switch control */
	SOC_DOUBLE("ADC1 Switch", AD193X_ADC_CTRL0, AD193X_ADCL1_MUTE,
		AD193X_ADCR1_MUTE, 1, 1),
	SOC_DOUBLE("ADC2 Switch", AD193X_ADC_CTRL0, AD193X_ADCL2_MUTE,
		AD193X_ADCR2_MUTE, 1, 1),

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* DAC switch control */
	SOC_DOUBLE("DAC1 Switch", AD193X_DAC_CHNL_MUTE, AD193X_DACL1_MUTE,
		AD193X_DACR1_MUTE, 1, 1),
	SOC_DOUBLE("DAC2 Switch", AD193X_DAC_CHNL_MUTE, AD193X_DACL2_MUTE,
		AD193X_DACR2_MUTE, 1, 1),
	SOC_DOUBLE("DAC3 Switch", AD193X_DAC_CHNL_MUTE, AD193X_DACL3_MUTE,
		AD193X_DACR3_MUTE, 1, 1),
	SOC_DOUBLE("DAC4 Switch", AD193X_DAC_CHNL_MUTE, AD193X_DACL4_MUTE,
		AD193X_DACR4_MUTE, 1, 1),

<<<<<<< HEAD
	/* ADC high-pass filter */
	SOC_SINGLE("ADC High Pass Filter Switch", AD193X_ADC_CTRL0,
			AD193X_ADC_HIGHPASS_FILTER, 1, 0),

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* DAC de-emphasis */
	SOC_ENUM("Playback Deemphasis", ad193x_deemp_enum),
};

<<<<<<< HEAD
static const struct snd_soc_dapm_widget ad193x_dapm_widgets[] = {
	SND_SOC_DAPM_DAC("DAC", "Playback", AD193X_DAC_CTRL0, 0, 1),
	SND_SOC_DAPM_ADC("ADC", "Capture", SND_SOC_NOPM, 0, 0),
	SND_SOC_DAPM_SUPPLY("PLL_PWR", AD193X_PLL_CLK_CTRL0, 0, 1, NULL, 0),
	SND_SOC_DAPM_SUPPLY("ADC_PWR", AD193X_ADC_CTRL0, 0, 1, NULL, 0),
	SND_SOC_DAPM_SUPPLY("SYSCLK", AD193X_PLL_CLK_CTRL0, 7, 0, NULL, 0),
=======
static const struct snd_kcontrol_new ad193x_adc_snd_controls[] = {
	/* ADC switch control */
	SOC_DOUBLE("ADC1 Switch", AD193X_ADC_CTRL0, AD193X_ADCL1_MUTE,
		AD193X_ADCR1_MUTE, 1, 1),
	SOC_DOUBLE("ADC2 Switch", AD193X_ADC_CTRL0, AD193X_ADCL2_MUTE,
		AD193X_ADCR2_MUTE, 1, 1),

	/* ADC high-pass filter */
	SOC_SINGLE("ADC High Pass Filter Switch", AD193X_ADC_CTRL0,
			AD193X_ADC_HIGHPASS_FILTER, 1, 0),
};

static const struct snd_soc_dapm_widget ad193x_dapm_widgets[] = {
	SND_SOC_DAPM_DAC("DAC", "Playback", SND_SOC_NOPM, 0, 0),
	SND_SOC_DAPM_PGA("DAC Output", AD193X_DAC_CTRL0, 0, 1, NULL, 0),
	SND_SOC_DAPM_SUPPLY("PLL_PWR", AD193X_PLL_CLK_CTRL0, 0, 1, NULL, 0),
	SND_SOC_DAPM_SUPPLY("SYSCLK", AD193X_PLL_CLK_CTRL0, 7, 0, NULL, 0),
	SND_SOC_DAPM_VMID("VMID"),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	SND_SOC_DAPM_OUTPUT("DAC1OUT"),
	SND_SOC_DAPM_OUTPUT("DAC2OUT"),
	SND_SOC_DAPM_OUTPUT("DAC3OUT"),
	SND_SOC_DAPM_OUTPUT("DAC4OUT"),
<<<<<<< HEAD
=======
};

static const struct snd_soc_dapm_widget ad193x_adc_widgets[] = {
	SND_SOC_DAPM_ADC("ADC", "Capture", SND_SOC_NOPM, 0, 0),
	SND_SOC_DAPM_SUPPLY("ADC_PWR", AD193X_ADC_CTRL0, 0, 1, NULL, 0),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	SND_SOC_DAPM_INPUT("ADC1IN"),
	SND_SOC_DAPM_INPUT("ADC2IN"),
};

<<<<<<< HEAD
static const struct snd_soc_dapm_route audio_paths[] = {
	{ "DAC", NULL, "SYSCLK" },
	{ "ADC", NULL, "SYSCLK" },
	{ "DAC", NULL, "ADC_PWR" },
	{ "ADC", NULL, "ADC_PWR" },
	{ "DAC1OUT", NULL, "DAC" },
	{ "DAC2OUT", NULL, "DAC" },
	{ "DAC3OUT", NULL, "DAC" },
	{ "DAC4OUT", NULL, "DAC" },
	{ "ADC", NULL, "ADC1IN" },
	{ "ADC", NULL, "ADC2IN" },
	{ "SYSCLK", NULL, "PLL_PWR" },
};

=======
static int ad193x_check_pll(struct snd_soc_dapm_widget *source,
			    struct snd_soc_dapm_widget *sink)
{
	struct snd_soc_component *component = snd_soc_dapm_to_component(source->dapm);
	struct ad193x_priv *ad193x = snd_soc_component_get_drvdata(component);

	return !!ad193x->sysclk;
}

static const struct snd_soc_dapm_route audio_paths[] = {
	{ "DAC", NULL, "SYSCLK" },
	{ "DAC Output", NULL, "DAC" },
	{ "DAC Output", NULL, "VMID" },
	{ "DAC1OUT", NULL, "DAC Output" },
	{ "DAC2OUT", NULL, "DAC Output" },
	{ "DAC3OUT", NULL, "DAC Output" },
	{ "DAC4OUT", NULL, "DAC Output" },
	{ "SYSCLK", NULL, "PLL_PWR", &ad193x_check_pll },
};

static const struct snd_soc_dapm_route ad193x_adc_audio_paths[] = {
	{ "ADC", NULL, "SYSCLK" },
	{ "ADC", NULL, "ADC_PWR" },
	{ "ADC", NULL, "ADC1IN" },
	{ "ADC", NULL, "ADC2IN" },
};

static inline bool ad193x_has_adc(const struct ad193x_priv *ad193x)
{
	switch (ad193x->type) {
	case AD1933:
	case AD1934:
		return false;
	default:
		break;
	}

	return true;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * DAI ops entries
 */

<<<<<<< HEAD
static int ad193x_mute(struct snd_soc_dai *dai, int mute)
{
	struct ad193x_priv *ad193x = snd_soc_codec_get_drvdata(dai->codec);
=======
static int ad193x_mute(struct snd_soc_dai *dai, int mute, int direction)
{
	struct ad193x_priv *ad193x = snd_soc_component_get_drvdata(dai->component);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (mute)
		regmap_update_bits(ad193x->regmap, AD193X_DAC_CTRL2,
				    AD193X_DAC_MASTER_MUTE,
				    AD193X_DAC_MASTER_MUTE);
	else
		regmap_update_bits(ad193x->regmap, AD193X_DAC_CTRL2,
				    AD193X_DAC_MASTER_MUTE, 0);

	return 0;
}

static int ad193x_set_tdm_slot(struct snd_soc_dai *dai, unsigned int tx_mask,
			       unsigned int rx_mask, int slots, int width)
{
<<<<<<< HEAD
	struct ad193x_priv *ad193x = snd_soc_codec_get_drvdata(dai->codec);
=======
	struct ad193x_priv *ad193x = snd_soc_component_get_drvdata(dai->component);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned int channels;

	switch (slots) {
	case 2:
		channels = AD193X_2_CHANNELS;
		break;
	case 4:
		channels = AD193X_4_CHANNELS;
		break;
	case 8:
		channels = AD193X_8_CHANNELS;
		break;
	case 16:
		channels = AD193X_16_CHANNELS;
		break;
	default:
		return -EINVAL;
	}

	regmap_update_bits(ad193x->regmap, AD193X_DAC_CTRL1,
		AD193X_DAC_CHAN_MASK, channels << AD193X_DAC_CHAN_SHFT);
<<<<<<< HEAD
	regmap_update_bits(ad193x->regmap, AD193X_ADC_CTRL2,
		AD193X_ADC_CHAN_MASK, channels << AD193X_ADC_CHAN_SHFT);
=======
	if (ad193x_has_adc(ad193x))
		regmap_update_bits(ad193x->regmap, AD193X_ADC_CTRL2,
				   AD193X_ADC_CHAN_MASK,
				   channels << AD193X_ADC_CHAN_SHFT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static int ad193x_set_dai_fmt(struct snd_soc_dai *codec_dai,
		unsigned int fmt)
{
<<<<<<< HEAD
	struct ad193x_priv *ad193x = snd_soc_codec_get_drvdata(codec_dai->codec);
	unsigned int adc_serfmt = 0;
=======
	struct ad193x_priv *ad193x = snd_soc_component_get_drvdata(codec_dai->component);
	unsigned int adc_serfmt = 0;
	unsigned int dac_serfmt = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned int adc_fmt = 0;
	unsigned int dac_fmt = 0;

	/* At present, the driver only support AUX ADC mode(SND_SOC_DAIFMT_I2S
<<<<<<< HEAD
	 * with TDM) and ADC&DAC TDM mode(SND_SOC_DAIFMT_DSP_A)
=======
	 * with TDM), ADC&DAC TDM mode(SND_SOC_DAIFMT_DSP_A) and DAC I2S mode
	 * (SND_SOC_DAIFMT_I2S)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 */
	switch (fmt & SND_SOC_DAIFMT_FORMAT_MASK) {
	case SND_SOC_DAIFMT_I2S:
		adc_serfmt |= AD193X_ADC_SERFMT_TDM;
<<<<<<< HEAD
		break;
	case SND_SOC_DAIFMT_DSP_A:
		adc_serfmt |= AD193X_ADC_SERFMT_AUX;
		break;
	default:
		return -EINVAL;
=======
		dac_serfmt |= AD193X_DAC_SERFMT_STEREO;
		break;
	case SND_SOC_DAIFMT_DSP_A:
		adc_serfmt |= AD193X_ADC_SERFMT_AUX;
		dac_serfmt |= AD193X_DAC_SERFMT_TDM;
		break;
	default:
		if (ad193x_has_adc(ad193x))
			return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	switch (fmt & SND_SOC_DAIFMT_INV_MASK) {
	case SND_SOC_DAIFMT_NB_NF: /* normal bit clock + frame */
		break;
	case SND_SOC_DAIFMT_NB_IF: /* normal bclk + invert frm */
		adc_fmt |= AD193X_ADC_LEFT_HIGH;
		dac_fmt |= AD193X_DAC_LEFT_HIGH;
		break;
	case SND_SOC_DAIFMT_IB_NF: /* invert bclk + normal frm */
		adc_fmt |= AD193X_ADC_BCLK_INV;
		dac_fmt |= AD193X_DAC_BCLK_INV;
		break;
	case SND_SOC_DAIFMT_IB_IF: /* invert bclk + frm */
		adc_fmt |= AD193X_ADC_LEFT_HIGH;
		adc_fmt |= AD193X_ADC_BCLK_INV;
		dac_fmt |= AD193X_DAC_LEFT_HIGH;
		dac_fmt |= AD193X_DAC_BCLK_INV;
		break;
	default:
		return -EINVAL;
	}

<<<<<<< HEAD
	switch (fmt & SND_SOC_DAIFMT_MASTER_MASK) {
	case SND_SOC_DAIFMT_CBM_CFM: /* codec clk & frm master */
=======
	/* For DSP_*, LRCLK's polarity must be inverted */
	if (fmt & SND_SOC_DAIFMT_DSP_A)
		dac_fmt ^= AD193X_DAC_LEFT_HIGH;

	switch (fmt & SND_SOC_DAIFMT_CLOCK_PROVIDER_MASK) {
	case SND_SOC_DAIFMT_CBP_CFP:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		adc_fmt |= AD193X_ADC_LCR_MASTER;
		adc_fmt |= AD193X_ADC_BCLK_MASTER;
		dac_fmt |= AD193X_DAC_LCR_MASTER;
		dac_fmt |= AD193X_DAC_BCLK_MASTER;
		break;
<<<<<<< HEAD
	case SND_SOC_DAIFMT_CBS_CFM: /* codec clk slave & frm master */
		adc_fmt |= AD193X_ADC_LCR_MASTER;
		dac_fmt |= AD193X_DAC_LCR_MASTER;
		break;
	case SND_SOC_DAIFMT_CBM_CFS: /* codec clk master & frame slave */
		adc_fmt |= AD193X_ADC_BCLK_MASTER;
		dac_fmt |= AD193X_DAC_BCLK_MASTER;
		break;
	case SND_SOC_DAIFMT_CBS_CFS: /* codec clk & frm slave */
=======
	case SND_SOC_DAIFMT_CBC_CFP:
		adc_fmt |= AD193X_ADC_LCR_MASTER;
		dac_fmt |= AD193X_DAC_LCR_MASTER;
		break;
	case SND_SOC_DAIFMT_CBP_CFC:
		adc_fmt |= AD193X_ADC_BCLK_MASTER;
		dac_fmt |= AD193X_DAC_BCLK_MASTER;
		break;
	case SND_SOC_DAIFMT_CBC_CFC:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	default:
		return -EINVAL;
	}

<<<<<<< HEAD
	regmap_update_bits(ad193x->regmap, AD193X_ADC_CTRL1,
		AD193X_ADC_SERFMT_MASK, adc_serfmt);
	regmap_update_bits(ad193x->regmap, AD193X_ADC_CTRL2,
		AD193X_ADC_FMT_MASK, adc_fmt);
=======
	if (ad193x_has_adc(ad193x)) {
		regmap_update_bits(ad193x->regmap, AD193X_ADC_CTRL1,
				   AD193X_ADC_SERFMT_MASK, adc_serfmt);
		regmap_update_bits(ad193x->regmap, AD193X_ADC_CTRL2,
				   AD193X_ADC_FMT_MASK, adc_fmt);
	}
	regmap_update_bits(ad193x->regmap, AD193X_DAC_CTRL0,
			   AD193X_DAC_SERFMT_MASK, dac_serfmt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	regmap_update_bits(ad193x->regmap, AD193X_DAC_CTRL1,
		AD193X_DAC_FMT_MASK, dac_fmt);

	return 0;
}

static int ad193x_set_dai_sysclk(struct snd_soc_dai *codec_dai,
		int clk_id, unsigned int freq, int dir)
{
<<<<<<< HEAD
	struct snd_soc_codec *codec = codec_dai->codec;
	struct ad193x_priv *ad193x = snd_soc_codec_get_drvdata(codec);
=======
	struct snd_soc_component *component = codec_dai->component;
	struct snd_soc_dapm_context *dapm = snd_soc_component_get_dapm(component);
	struct ad193x_priv *ad193x = snd_soc_component_get_drvdata(component);

	if (clk_id == AD193X_SYSCLK_MCLK) {
		/* MCLK must be 512 x fs */
		if (dir == SND_SOC_CLOCK_OUT || freq != 24576000)
			return -EINVAL;

		regmap_update_bits(ad193x->regmap, AD193X_PLL_CLK_CTRL1,
				   AD193X_PLL_SRC_MASK,
				   AD193X_PLL_DAC_SRC_MCLK |
				   AD193X_PLL_CLK_SRC_MCLK);

		snd_soc_dapm_sync(dapm);
		return 0;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	switch (freq) {
	case 12288000:
	case 18432000:
	case 24576000:
	case 36864000:
		ad193x->sysclk = freq;
		return 0;
	}
	return -EINVAL;
}

static int ad193x_hw_params(struct snd_pcm_substream *substream,
		struct snd_pcm_hw_params *params,
		struct snd_soc_dai *dai)
{
	int word_len = 0, master_rate = 0;
<<<<<<< HEAD

	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_codec *codec = rtd->codec;
	struct ad193x_priv *ad193x = snd_soc_codec_get_drvdata(codec);

	/* bit size */
	switch (params_format(params)) {
	case SNDRV_PCM_FORMAT_S16_LE:
		word_len = 3;
		break;
	case SNDRV_PCM_FORMAT_S20_3LE:
		word_len = 1;
		break;
	case SNDRV_PCM_FORMAT_S24_LE:
	case SNDRV_PCM_FORMAT_S32_LE:
=======
	struct snd_soc_component *component = dai->component;
	struct ad193x_priv *ad193x = snd_soc_component_get_drvdata(component);
	bool is_playback = substream->stream == SNDRV_PCM_STREAM_PLAYBACK;
	u8 dacc0;

	dev_dbg(dai->dev, "%s() rate=%u format=%#x width=%u channels=%u\n",
		__func__, params_rate(params), params_format(params),
		params_width(params), params_channels(params));


	/* bit size */
	switch (params_width(params)) {
	case 16:
		word_len = 3;
		break;
	case 20:
		word_len = 1;
		break;
	case 24:
	case 32:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		word_len = 0;
		break;
	}

	switch (ad193x->sysclk) {
	case 12288000:
		master_rate = AD193X_PLL_INPUT_256;
		break;
	case 18432000:
		master_rate = AD193X_PLL_INPUT_384;
		break;
	case 24576000:
		master_rate = AD193X_PLL_INPUT_512;
		break;
	case 36864000:
		master_rate = AD193X_PLL_INPUT_768;
		break;
	}

<<<<<<< HEAD
=======
	if (is_playback) {
		switch (params_rate(params)) {
		case 48000:
			dacc0 = AD193X_DAC_SR_48;
			break;
		case 96000:
			dacc0 = AD193X_DAC_SR_96;
			break;
		case 192000:
			dacc0 = AD193X_DAC_SR_192;
			break;
		default:
			dev_err(dai->dev, "invalid sampling rate: %d\n", params_rate(params));
			return -EINVAL;
		}

		regmap_update_bits(ad193x->regmap, AD193X_DAC_CTRL0, AD193X_DAC_SR_MASK, dacc0);
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	regmap_update_bits(ad193x->regmap, AD193X_PLL_CLK_CTRL0,
			    AD193X_PLL_INPUT_MASK, master_rate);

	regmap_update_bits(ad193x->regmap, AD193X_DAC_CTRL2,
			    AD193X_DAC_WORD_LEN_MASK,
			    word_len << AD193X_DAC_WORD_LEN_SHFT);

<<<<<<< HEAD
	regmap_update_bits(ad193x->regmap, AD193X_ADC_CTRL1,
			    AD193X_ADC_WORD_LEN_MASK, word_len);
=======
	if (ad193x_has_adc(ad193x))
		regmap_update_bits(ad193x->regmap, AD193X_ADC_CTRL1,
				   AD193X_ADC_WORD_LEN_MASK, word_len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static const struct snd_soc_dai_ops ad193x_dai_ops = {
	.hw_params = ad193x_hw_params,
	.digital_mute = ad193x_mute,
	.set_tdm_slot = ad193x_set_tdm_slot,
	.set_sysclk	= ad193x_set_dai_sysclk,
	.set_fmt = ad193x_set_dai_fmt,
=======
static int ad193x_startup(struct snd_pcm_substream *substream,
			  struct snd_soc_dai *dai)
{
	return snd_pcm_hw_constraint_list(substream->runtime, 0,
				   SNDRV_PCM_HW_PARAM_SAMPLE_BITS,
				   &constr);
}

static const struct snd_soc_dai_ops ad193x_dai_ops = {
	.startup = ad193x_startup,
	.hw_params = ad193x_hw_params,
	.mute_stream = ad193x_mute,
	.set_tdm_slot = ad193x_set_tdm_slot,
	.set_sysclk	= ad193x_set_dai_sysclk,
	.set_fmt = ad193x_set_dai_fmt,
	.no_capture_mute = 1,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/* codec DAI instance */
static struct snd_soc_dai_driver ad193x_dai = {
	.name = "ad193x-hifi",
	.playback = {
		.stream_name = "Playback",
		.channels_min = 2,
		.channels_max = 8,
<<<<<<< HEAD
		.rates = SNDRV_PCM_RATE_48000,
=======
		.rates = SNDRV_PCM_RATE_48000 | SNDRV_PCM_RATE_96000 | SNDRV_PCM_RATE_192000,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.formats = SNDRV_PCM_FMTBIT_S32_LE | SNDRV_PCM_FMTBIT_S16_LE |
			SNDRV_PCM_FMTBIT_S20_3LE | SNDRV_PCM_FMTBIT_S24_LE,
	},
	.capture = {
		.stream_name = "Capture",
		.channels_min = 2,
		.channels_max = 4,
		.rates = SNDRV_PCM_RATE_48000,
		.formats = SNDRV_PCM_FMTBIT_S32_LE | SNDRV_PCM_FMTBIT_S16_LE |
			SNDRV_PCM_FMTBIT_S20_3LE | SNDRV_PCM_FMTBIT_S24_LE,
	},
	.ops = &ad193x_dai_ops,
};

<<<<<<< HEAD
static int ad193x_probe(struct snd_soc_codec *codec)
{
	struct ad193x_priv *ad193x = snd_soc_codec_get_drvdata(codec);
	int ret;

	codec->control_data = ad193x->regmap;
	ret = snd_soc_codec_set_cache_io(codec, 0, 0, SND_SOC_REGMAP);
	if (ret < 0) {
		dev_err(codec->dev, "failed to set cache I/O: %d\n", ret);
		return ret;
	}

	/* default setting for ad193x */

	/* unmute dac channels */
	regmap_write(ad193x->regmap, AD193X_DAC_CHNL_MUTE, 0x0);
	/* de-emphasis: 48kHz, powedown dac */
	regmap_write(ad193x->regmap, AD193X_DAC_CTRL2, 0x1A);
	/* powerdown dac, dac in tdm mode */
	regmap_write(ad193x->regmap, AD193X_DAC_CTRL0, 0x41);
	/* high-pass filter enable */
	regmap_write(ad193x->regmap, AD193X_ADC_CTRL0, 0x3);
	/* sata delay=1, adc aux mode */
	regmap_write(ad193x->regmap, AD193X_ADC_CTRL1, 0x43);
	/* pll input: mclki/xi */
	regmap_write(ad193x->regmap, AD193X_PLL_CLK_CTRL0, 0x99); /* mclk=24.576Mhz: 0x9D; mclk=12.288Mhz: 0x99 */
	regmap_write(ad193x->regmap, AD193X_PLL_CLK_CTRL1, 0x04);

	return ret;
}

static struct snd_soc_codec_driver soc_codec_dev_ad193x = {
	.probe = 	ad193x_probe,
	.controls = ad193x_snd_controls,
	.num_controls = ARRAY_SIZE(ad193x_snd_controls),
	.dapm_widgets = ad193x_dapm_widgets,
	.num_dapm_widgets = ARRAY_SIZE(ad193x_dapm_widgets),
	.dapm_routes = audio_paths,
	.num_dapm_routes = ARRAY_SIZE(audio_paths),
};

static bool adau193x_reg_volatile(struct device *dev, unsigned int reg)
{
	return false;
}

#if defined(CONFIG_SPI_MASTER)

static const struct regmap_config ad193x_spi_regmap_config = {
	.val_bits = 8,
	.reg_bits = 16,
	.read_flag_mask = 0x09,
	.write_flag_mask = 0x08,

	.max_register = AD193X_NUM_REGS - 1,
	.volatile_reg = adau193x_reg_volatile,
};

static int __devinit ad193x_spi_probe(struct spi_device *spi)
{
	struct ad193x_priv *ad193x;
	int ret;

	ad193x = devm_kzalloc(&spi->dev, sizeof(struct ad193x_priv),
			      GFP_KERNEL);
	if (ad193x == NULL)
		return -ENOMEM;

	ad193x->regmap = regmap_init_spi(spi, &ad193x_spi_regmap_config);
	if (IS_ERR(ad193x->regmap)) {
		ret = PTR_ERR(ad193x->regmap);
		goto err_out;
	}

	spi_set_drvdata(spi, ad193x);

	ret = snd_soc_register_codec(&spi->dev,
			&soc_codec_dev_ad193x, &ad193x_dai, 1);
	if (ret < 0)
		goto err_regmap_exit;

	return 0;

err_regmap_exit:
	regmap_exit(ad193x->regmap);
err_out:
	return ret;
}

static int __devexit ad193x_spi_remove(struct spi_device *spi)
{
	struct ad193x_priv *ad193x = spi_get_drvdata(spi);

	snd_soc_unregister_codec(&spi->dev);
	regmap_exit(ad193x->regmap);
	return 0;
}

static struct spi_driver ad193x_spi_driver = {
	.driver = {
		.name	= "ad193x",
		.owner	= THIS_MODULE,
	},
	.probe		= ad193x_spi_probe,
	.remove		= __devexit_p(ad193x_spi_remove),
};
#endif

#if defined(CONFIG_I2C) || defined(CONFIG_I2C_MODULE)

static const struct regmap_config ad193x_i2c_regmap_config = {
	.val_bits = 8,
	.reg_bits = 8,

	.max_register = AD193X_NUM_REGS - 1,
	.volatile_reg = adau193x_reg_volatile,
};

static const struct i2c_device_id ad193x_id[] = {
	{ "ad1936", 0 },
	{ "ad1937", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, ad193x_id);

static int __devinit ad193x_i2c_probe(struct i2c_client *client,
		const struct i2c_device_id *id)
{
	struct ad193x_priv *ad193x;
	int ret;

	ad193x = devm_kzalloc(&client->dev, sizeof(struct ad193x_priv),
			      GFP_KERNEL);
	if (ad193x == NULL)
		return -ENOMEM;

	ad193x->regmap = regmap_init_i2c(client, &ad193x_i2c_regmap_config);
	if (IS_ERR(ad193x->regmap)) {
		ret = PTR_ERR(ad193x->regmap);
		goto err_out;
	}

	i2c_set_clientdata(client, ad193x);

	ret =  snd_soc_register_codec(&client->dev,
			&soc_codec_dev_ad193x, &ad193x_dai, 1);
	if (ret < 0)
		goto err_regmap_exit;

	return 0;

err_regmap_exit:
	regmap_exit(ad193x->regmap);
err_out:
	return ret;
}

static int __devexit ad193x_i2c_remove(struct i2c_client *client)
{
	struct ad193x_priv *ad193x = i2c_get_clientdata(client);

	snd_soc_unregister_codec(&client->dev);
	regmap_exit(ad193x->regmap);
	return 0;
}

static struct i2c_driver ad193x_i2c_driver = {
	.driver = {
		.name = "ad193x",
	},
	.probe    = ad193x_i2c_probe,
	.remove   = __devexit_p(ad193x_i2c_remove),
	.id_table = ad193x_id,
};
#endif

static int __init ad193x_modinit(void)
{
	int ret;

#if defined(CONFIG_I2C) || defined(CONFIG_I2C_MODULE)
	ret =  i2c_add_driver(&ad193x_i2c_driver);
	if (ret != 0) {
		printk(KERN_ERR "Failed to register AD193X I2C driver: %d\n",
				ret);
	}
#endif

#if defined(CONFIG_SPI_MASTER)
	ret = spi_register_driver(&ad193x_spi_driver);
	if (ret != 0) {
		printk(KERN_ERR "Failed to register AD193X SPI driver: %d\n",
				ret);
	}
#endif
	return ret;
}
module_init(ad193x_modinit);

static void __exit ad193x_modexit(void)
{
#if defined(CONFIG_SPI_MASTER)
	spi_unregister_driver(&ad193x_spi_driver);
#endif

#if defined(CONFIG_I2C) || defined(CONFIG_I2C_MODULE)
	i2c_del_driver(&ad193x_i2c_driver);
#endif
}
module_exit(ad193x_modexit);
=======
/* codec DAI instance for DAC only */
static struct snd_soc_dai_driver ad193x_no_adc_dai = {
	.name = "ad193x-hifi",
	.playback = {
		.stream_name = "Playback",
		.channels_min = 2,
		.channels_max = 8,
		.rates = SNDRV_PCM_RATE_48000 | SNDRV_PCM_RATE_96000 | SNDRV_PCM_RATE_192000,
		.formats = SNDRV_PCM_FMTBIT_S32_LE | SNDRV_PCM_FMTBIT_S16_LE |
			SNDRV_PCM_FMTBIT_S20_3LE | SNDRV_PCM_FMTBIT_S24_LE,
	},
	.ops = &ad193x_dai_ops,
};

/* codec register values to set after reset */
static void ad193x_reg_default_init(struct ad193x_priv *ad193x)
{
	static const struct reg_sequence reg_init[] = {
		{  0, 0x99 },	/* PLL_CLK_CTRL0: pll input: mclki/xi 12.288Mhz */
		{  1, 0x04 },	/* PLL_CLK_CTRL1: no on-chip Vref */
		{  2, 0x40 },	/* DAC_CTRL0: TDM mode */
		{  3, 0x00 },	/* DAC_CTRL1: reset */
		{  4, 0x1A },	/* DAC_CTRL2: 48kHz de-emphasis, unmute dac */
		{  5, 0x00 },	/* DAC_CHNL_MUTE: unmute DAC channels */
		{  6, 0x00 },	/* DAC_L1_VOL: no attenuation */
		{  7, 0x00 },	/* DAC_R1_VOL: no attenuation */
		{  8, 0x00 },	/* DAC_L2_VOL: no attenuation */
		{  9, 0x00 },	/* DAC_R2_VOL: no attenuation */
		{ 10, 0x00 },	/* DAC_L3_VOL: no attenuation */
		{ 11, 0x00 },	/* DAC_R3_VOL: no attenuation */
		{ 12, 0x00 },	/* DAC_L4_VOL: no attenuation */
		{ 13, 0x00 },	/* DAC_R4_VOL: no attenuation */
	};
	static const struct reg_sequence reg_adc_init[] = {
		{ 14, 0x03 },	/* ADC_CTRL0: high-pass filter enable */
		{ 15, 0x43 },	/* ADC_CTRL1: sata delay=1, adc aux mode */
		{ 16, 0x00 },	/* ADC_CTRL2: reset */
	};

	regmap_multi_reg_write(ad193x->regmap, reg_init, ARRAY_SIZE(reg_init));

	if (ad193x_has_adc(ad193x)) {
		regmap_multi_reg_write(ad193x->regmap, reg_adc_init,
				       ARRAY_SIZE(reg_adc_init));
	}
}

static int ad193x_component_probe(struct snd_soc_component *component)
{
	struct ad193x_priv *ad193x = snd_soc_component_get_drvdata(component);
	struct snd_soc_dapm_context *dapm = snd_soc_component_get_dapm(component);
	int num, ret;

	/* default setting for ad193x */
	ad193x_reg_default_init(ad193x);

	/* adc only */
	if (ad193x_has_adc(ad193x)) {
		/* add adc controls */
		num = ARRAY_SIZE(ad193x_adc_snd_controls);
		ret = snd_soc_add_component_controls(component,
						 ad193x_adc_snd_controls,
						 num);
		if (ret)
			return ret;

		/* add adc widgets */
		num = ARRAY_SIZE(ad193x_adc_widgets);
		ret = snd_soc_dapm_new_controls(dapm,
						ad193x_adc_widgets,
						num);
		if (ret)
			return ret;

		/* add adc routes */
		num = ARRAY_SIZE(ad193x_adc_audio_paths);
		ret = snd_soc_dapm_add_routes(dapm,
					      ad193x_adc_audio_paths,
					      num);
		if (ret)
			return ret;
	}

	return 0;
}

static const struct snd_soc_component_driver soc_component_dev_ad193x = {
	.probe			= ad193x_component_probe,
	.controls		= ad193x_snd_controls,
	.num_controls		= ARRAY_SIZE(ad193x_snd_controls),
	.dapm_widgets		= ad193x_dapm_widgets,
	.num_dapm_widgets	= ARRAY_SIZE(ad193x_dapm_widgets),
	.dapm_routes		= audio_paths,
	.num_dapm_routes	= ARRAY_SIZE(audio_paths),
	.idle_bias_on		= 1,
	.use_pmdown_time	= 1,
	.endianness		= 1,
};

const struct regmap_config ad193x_regmap_config = {
	.max_register = AD193X_NUM_REGS - 1,
};
EXPORT_SYMBOL_GPL(ad193x_regmap_config);

int ad193x_probe(struct device *dev, struct regmap *regmap,
		 enum ad193x_type type)
{
	struct ad193x_priv *ad193x;

	if (IS_ERR(regmap))
		return PTR_ERR(regmap);

	ad193x = devm_kzalloc(dev, sizeof(*ad193x), GFP_KERNEL);
	if (ad193x == NULL)
		return -ENOMEM;

	ad193x->regmap = regmap;
	ad193x->type = type;

	dev_set_drvdata(dev, ad193x);

	if (ad193x_has_adc(ad193x))
		return devm_snd_soc_register_component(dev, &soc_component_dev_ad193x,
						       &ad193x_dai, 1);
	return devm_snd_soc_register_component(dev, &soc_component_dev_ad193x,
		&ad193x_no_adc_dai, 1);
}
EXPORT_SYMBOL_GPL(ad193x_probe);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

MODULE_DESCRIPTION("ASoC ad193x driver");
MODULE_AUTHOR("Barry Song <21cnbao@gmail.com>");
MODULE_LICENSE("GPL");
