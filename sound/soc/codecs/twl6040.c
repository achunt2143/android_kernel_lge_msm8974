<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * ALSA SoC TWL6040 codec driver
 *
 * Author:	 Misael Lopez Cruz <x0052729@ti.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/pm.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/mfd/twl6040.h>

#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/soc-dapm.h>
#include <sound/initval.h>
#include <sound/tlv.h>

#include "twl6040.h"

<<<<<<< HEAD
=======
enum twl6040_dai_id {
	TWL6040_DAI_LEGACY = 0,
	TWL6040_DAI_UL,
	TWL6040_DAI_DL1,
	TWL6040_DAI_DL2,
	TWL6040_DAI_VIB,
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define TWL6040_RATES		SNDRV_PCM_RATE_8000_96000
#define TWL6040_FORMATS	(SNDRV_PCM_FMTBIT_S32_LE)

#define TWL6040_OUTHS_0dB 0x00
#define TWL6040_OUTHS_M30dB 0x0F
#define TWL6040_OUTHF_0dB 0x03
#define TWL6040_OUTHF_M52dB 0x1D

<<<<<<< HEAD
#define TWL6040_RAMP_NONE	0
#define TWL6040_RAMP_UP		1
#define TWL6040_RAMP_DOWN	2

#define TWL6040_HSL_VOL_MASK	0x0F
#define TWL6040_HSL_VOL_SHIFT	0
#define TWL6040_HSR_VOL_MASK	0xF0
#define TWL6040_HSR_VOL_SHIFT	4
#define TWL6040_HF_VOL_MASK	0x1F
#define TWL6040_HF_VOL_SHIFT	0

/* Shadow register used by the driver */
#define TWL6040_REG_SW_SHADOW	0x2F
#define TWL6040_CACHEREGNUM	(TWL6040_REG_SW_SHADOW + 1)

/* TWL6040_REG_SW_SHADOW (0x2F) fields */
#define TWL6040_EAR_PATH_ENABLE	0x01

struct twl6040_output {
	u16 active;
	u16 left_vol;
	u16 right_vol;
	u16 left_step;
	u16 right_step;
	unsigned int step_delay;
	u16 ramp;
	struct delayed_work work;
	struct completion ramp_done;
};
=======
#define TWL6040_CACHEREGNUM	(TWL6040_REG_STATUS + 1)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct twl6040_jack_data {
	struct snd_soc_jack *jack;
	struct delayed_work work;
	int report;
};

/* codec private data */
struct twl6040_data {
	int plug_irq;
	int codec_powered;
	int pll;
	int pll_power_mode;
	int hs_power_mode;
	int hs_power_mode_locked;
<<<<<<< HEAD
	unsigned int clk_in;
	unsigned int sysclk;
	u16 hs_left_step;
	u16 hs_right_step;
	u16 hf_left_step;
	u16 hf_right_step;
	struct twl6040_jack_data hs_jack;
	struct snd_soc_codec *codec;
	struct workqueue_struct *workqueue;
	struct mutex mutex;
	struct twl6040_output headset;
	struct twl6040_output handsfree;
};

/*
 * twl6040 register cache & default register settings
 */
static const u8 twl6040_reg[TWL6040_CACHEREGNUM] = {
	0x00, /* not used	0x00	*/
	0x4B, /* REG_ASICID	0x01 (ro) */
	0x00, /* REG_ASICREV	0x02 (ro) */
	0x00, /* REG_INTID	0x03	*/
	0x00, /* REG_INTMR	0x04	*/
	0x00, /* REG_NCPCTRL	0x05	*/
	0x00, /* REG_LDOCTL	0x06	*/
	0x60, /* REG_HPPLLCTL	0x07	*/
	0x00, /* REG_LPPLLCTL	0x08	*/
	0x4A, /* REG_LPPLLDIV	0x09	*/
	0x00, /* REG_AMICBCTL	0x0A	*/
	0x00, /* REG_DMICBCTL	0x0B	*/
	0x00, /* REG_MICLCTL	0x0C	*/
	0x00, /* REG_MICRCTL	0x0D	*/
	0x00, /* REG_MICGAIN	0x0E	*/
	0x1B, /* REG_LINEGAIN	0x0F	*/
	0x00, /* REG_HSLCTL	0x10	*/
	0x00, /* REG_HSRCTL	0x11	*/
	0x00, /* REG_HSGAIN	0x12	*/
	0x00, /* REG_EARCTL	0x13	*/
	0x00, /* REG_HFLCTL	0x14	*/
	0x00, /* REG_HFLGAIN	0x15	*/
	0x00, /* REG_HFRCTL	0x16	*/
	0x00, /* REG_HFRGAIN	0x17	*/
	0x00, /* REG_VIBCTLL	0x18	*/
	0x00, /* REG_VIBDATL	0x19	*/
	0x00, /* REG_VIBCTLR	0x1A	*/
	0x00, /* REG_VIBDATR	0x1B	*/
	0x00, /* REG_HKCTL1	0x1C	*/
	0x00, /* REG_HKCTL2	0x1D	*/
	0x00, /* REG_GPOCTL	0x1E	*/
	0x00, /* REG_ALB	0x1F	*/
	0x00, /* REG_DLB	0x20	*/
	0x00, /* not used	0x21	*/
	0x00, /* not used	0x22	*/
	0x00, /* not used	0x23	*/
	0x00, /* not used	0x24	*/
	0x00, /* not used	0x25	*/
	0x00, /* not used	0x26	*/
	0x00, /* not used	0x27	*/
	0x00, /* REG_TRIM1	0x28	*/
	0x00, /* REG_TRIM2	0x29	*/
	0x00, /* REG_TRIM3	0x2A	*/
	0x00, /* REG_HSOTRIM	0x2B	*/
	0x00, /* REG_HFOTRIM	0x2C	*/
	0x09, /* REG_ACCCTL	0x2D	*/
	0x00, /* REG_STATUS	0x2E (ro) */

	0x00, /* REG_SW_SHADOW	0x2F - Shadow, non HW register */
};

/* List of registers to be restored after power up */
static const int twl6040_restore_list[] = {
	TWL6040_REG_MICLCTL,
	TWL6040_REG_MICRCTL,
	TWL6040_REG_MICGAIN,
	TWL6040_REG_LINEGAIN,
	TWL6040_REG_HSLCTL,
	TWL6040_REG_HSRCTL,
	TWL6040_REG_HSGAIN,
	TWL6040_REG_EARCTL,
	TWL6040_REG_HFLCTL,
	TWL6040_REG_HFLGAIN,
	TWL6040_REG_HFRCTL,
	TWL6040_REG_HFRGAIN,
};

/* set of rates for each pll: low-power and high-performance */
static unsigned int lp_rates[] = {
=======
	bool dl1_unmuted;
	bool dl2_unmuted;
	u8 dl12_cache[TWL6040_REG_HFRCTL - TWL6040_REG_HSLCTL + 1];
	unsigned int clk_in;
	unsigned int sysclk;
	struct twl6040_jack_data hs_jack;
	struct snd_soc_component *component;
	struct mutex mutex;
};

/* set of rates for each pll: low-power and high-performance */
static const unsigned int lp_rates[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	8000,
	11250,
	16000,
	22500,
	32000,
	44100,
	48000,
	88200,
	96000,
};

<<<<<<< HEAD
static unsigned int hp_rates[] = {
=======
static const unsigned int hp_rates[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	8000,
	16000,
	32000,
	48000,
	96000,
};

<<<<<<< HEAD
static struct snd_pcm_hw_constraint_list sysclk_constraints[] = {
=======
static const struct snd_pcm_hw_constraint_list sysclk_constraints[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{ .count = ARRAY_SIZE(lp_rates), .list = lp_rates, },
	{ .count = ARRAY_SIZE(hp_rates), .list = hp_rates, },
};

<<<<<<< HEAD
/*
 * read twl6040 register cache
 */
static inline unsigned int twl6040_read_reg_cache(struct snd_soc_codec *codec,
						unsigned int reg)
{
	u8 *cache = codec->reg_cache;

	if (reg >= TWL6040_CACHEREGNUM)
		return -EIO;

	return cache[reg];
}

/*
 * write twl6040 register cache
 */
static inline void twl6040_write_reg_cache(struct snd_soc_codec *codec,
						u8 reg, u8 value)
{
	u8 *cache = codec->reg_cache;

	if (reg >= TWL6040_CACHEREGNUM)
		return;
	cache[reg] = value;
}

/*
 * read from twl6040 hardware register
 */
static int twl6040_read_reg_volatile(struct snd_soc_codec *codec,
			unsigned int reg)
{
	struct twl6040 *twl6040 = codec->control_data;
=======
#define to_twl6040(component)	dev_get_drvdata((component)->dev->parent)

static unsigned int twl6040_read(struct snd_soc_component *component, unsigned int reg)
{
	struct twl6040_data *priv = snd_soc_component_get_drvdata(component);
	struct twl6040 *twl6040 = to_twl6040(component);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u8 value;

	if (reg >= TWL6040_CACHEREGNUM)
		return -EIO;

<<<<<<< HEAD
	if (likely(reg < TWL6040_REG_SW_SHADOW)) {
		value = twl6040_reg_read(twl6040, reg);
		twl6040_write_reg_cache(codec, reg, value);
	} else {
		value = twl6040_read_reg_cache(codec, reg);
=======
	switch (reg) {
	case TWL6040_REG_HSLCTL:
	case TWL6040_REG_HSRCTL:
	case TWL6040_REG_EARCTL:
	case TWL6040_REG_HFLCTL:
	case TWL6040_REG_HFRCTL:
		value = priv->dl12_cache[reg - TWL6040_REG_HSLCTL];
		break;
	default:
		value = twl6040_reg_read(twl6040, reg);
		break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return value;
}

<<<<<<< HEAD
/*
 * write to the twl6040 register space
 */
static int twl6040_write(struct snd_soc_codec *codec,
			unsigned int reg, unsigned int value)
{
	struct twl6040 *twl6040 = codec->control_data;
=======
static bool twl6040_can_write_to_chip(struct snd_soc_component *component,
				  unsigned int reg)
{
	struct twl6040_data *priv = snd_soc_component_get_drvdata(component);

	switch (reg) {
	case TWL6040_REG_HSLCTL:
	case TWL6040_REG_HSRCTL:
	case TWL6040_REG_EARCTL:
		/* DL1 path */
		return priv->dl1_unmuted;
	case TWL6040_REG_HFLCTL:
	case TWL6040_REG_HFRCTL:
		return priv->dl2_unmuted;
	default:
		return true;
	}
}

static inline void twl6040_update_dl12_cache(struct snd_soc_component *component,
					     u8 reg, u8 value)
{
	struct twl6040_data *priv = snd_soc_component_get_drvdata(component);

	switch (reg) {
	case TWL6040_REG_HSLCTL:
	case TWL6040_REG_HSRCTL:
	case TWL6040_REG_EARCTL:
	case TWL6040_REG_HFLCTL:
	case TWL6040_REG_HFRCTL:
		priv->dl12_cache[reg - TWL6040_REG_HSLCTL] = value;
		break;
	default:
		break;
	}
}

static int twl6040_write(struct snd_soc_component *component,
			unsigned int reg, unsigned int value)
{
	struct twl6040 *twl6040 = to_twl6040(component);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (reg >= TWL6040_CACHEREGNUM)
		return -EIO;

<<<<<<< HEAD
	twl6040_write_reg_cache(codec, reg, value);
	if (likely(reg < TWL6040_REG_SW_SHADOW))
=======
	twl6040_update_dl12_cache(component, reg, value);
	if (twl6040_can_write_to_chip(component, reg))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return twl6040_reg_write(twl6040, reg, value);
	else
		return 0;
}

<<<<<<< HEAD
static void twl6040_init_chip(struct snd_soc_codec *codec)
{
	struct twl6040 *twl6040 = codec->control_data;
	u8 val;

	/* Update reg_cache: ASICREV, and TRIM values */
	val = twl6040_get_revid(twl6040);
	twl6040_write_reg_cache(codec, TWL6040_REG_ASICREV, val);

	twl6040_read_reg_volatile(codec, TWL6040_REG_TRIM1);
	twl6040_read_reg_volatile(codec, TWL6040_REG_TRIM2);
	twl6040_read_reg_volatile(codec, TWL6040_REG_TRIM3);
	twl6040_read_reg_volatile(codec, TWL6040_REG_HSOTRIM);
	twl6040_read_reg_volatile(codec, TWL6040_REG_HFOTRIM);

	/* Change chip defaults */
	/* No imput selected for microphone amplifiers */
	twl6040_write_reg_cache(codec, TWL6040_REG_MICLCTL, 0x18);
	twl6040_write_reg_cache(codec, TWL6040_REG_MICRCTL, 0x18);
=======
static void twl6040_init_chip(struct snd_soc_component *component)
{
	twl6040_read(component, TWL6040_REG_TRIM1);
	twl6040_read(component, TWL6040_REG_TRIM2);
	twl6040_read(component, TWL6040_REG_TRIM3);
	twl6040_read(component, TWL6040_REG_HSOTRIM);
	twl6040_read(component, TWL6040_REG_HFOTRIM);

	/* Change chip defaults */
	/* No imput selected for microphone amplifiers */
	twl6040_write(component, TWL6040_REG_MICLCTL, 0x18);
	twl6040_write(component, TWL6040_REG_MICRCTL, 0x18);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * We need to lower the default gain values, so the ramp code
	 * can work correctly for the first playback.
	 * This reduces the pop noise heard at the first playback.
	 */
<<<<<<< HEAD
	twl6040_write_reg_cache(codec, TWL6040_REG_HSGAIN, 0xff);
	twl6040_write_reg_cache(codec, TWL6040_REG_EARCTL, 0x1e);
	twl6040_write_reg_cache(codec, TWL6040_REG_HFLGAIN, 0x1d);
	twl6040_write_reg_cache(codec, TWL6040_REG_HFRGAIN, 0x1d);
	twl6040_write_reg_cache(codec, TWL6040_REG_LINEGAIN, 0);
}

static void twl6040_restore_regs(struct snd_soc_codec *codec)
{
	u8 *cache = codec->reg_cache;
	int reg, i;

	for (i = 0; i < ARRAY_SIZE(twl6040_restore_list); i++) {
		reg = twl6040_restore_list[i];
		twl6040_write(codec, reg, cache[reg]);
	}
}

/*
 * Ramp HS PGA volume to minimise pops at stream startup and shutdown.
 */
static inline int twl6040_hs_ramp_step(struct snd_soc_codec *codec,
			unsigned int left_step, unsigned int right_step)
{

	struct twl6040_data *priv = snd_soc_codec_get_drvdata(codec);
	struct twl6040_output *headset = &priv->headset;
	int left_complete = 0, right_complete = 0;
	u8 reg, val;

	/* left channel */
	left_step = (left_step > 0xF) ? 0xF : left_step;
	reg = twl6040_read_reg_cache(codec, TWL6040_REG_HSGAIN);
	val = (~reg & TWL6040_HSL_VOL_MASK);

	if (headset->ramp == TWL6040_RAMP_UP) {
		/* ramp step up */
		if (val < headset->left_vol) {
			if (val + left_step > headset->left_vol)
				val = headset->left_vol;
			else
				val += left_step;

			reg &= ~TWL6040_HSL_VOL_MASK;
			twl6040_write(codec, TWL6040_REG_HSGAIN,
					(reg | (~val & TWL6040_HSL_VOL_MASK)));
		} else {
			left_complete = 1;
		}
	} else if (headset->ramp == TWL6040_RAMP_DOWN) {
		/* ramp step down */
		if (val > 0x0) {
			if ((int)val - (int)left_step < 0)
				val = 0;
			else
				val -= left_step;

			reg &= ~TWL6040_HSL_VOL_MASK;
			twl6040_write(codec, TWL6040_REG_HSGAIN, reg |
						(~val & TWL6040_HSL_VOL_MASK));
		} else {
			left_complete = 1;
		}
	}

	/* right channel */
	right_step = (right_step > 0xF) ? 0xF : right_step;
	reg = twl6040_read_reg_cache(codec, TWL6040_REG_HSGAIN);
	val = (~reg & TWL6040_HSR_VOL_MASK) >> TWL6040_HSR_VOL_SHIFT;

	if (headset->ramp == TWL6040_RAMP_UP) {
		/* ramp step up */
		if (val < headset->right_vol) {
			if (val + right_step > headset->right_vol)
				val = headset->right_vol;
			else
				val += right_step;

			reg &= ~TWL6040_HSR_VOL_MASK;
			twl6040_write(codec, TWL6040_REG_HSGAIN,
				(reg | (~val << TWL6040_HSR_VOL_SHIFT)));
		} else {
			right_complete = 1;
		}
	} else if (headset->ramp == TWL6040_RAMP_DOWN) {
		/* ramp step down */
		if (val > 0x0) {
			if ((int)val - (int)right_step < 0)
				val = 0;
			else
				val -= right_step;

			reg &= ~TWL6040_HSR_VOL_MASK;
			twl6040_write(codec, TWL6040_REG_HSGAIN,
					 reg | (~val << TWL6040_HSR_VOL_SHIFT));
		} else {
			right_complete = 1;
		}
	}

	return left_complete & right_complete;
}

/*
 * Ramp HF PGA volume to minimise pops at stream startup and shutdown.
 */
static inline int twl6040_hf_ramp_step(struct snd_soc_codec *codec,
			unsigned int left_step, unsigned int right_step)
{
	struct twl6040_data *priv = snd_soc_codec_get_drvdata(codec);
	struct twl6040_output *handsfree = &priv->handsfree;
	int left_complete = 0, right_complete = 0;
	u16 reg, val;

	/* left channel */
	left_step = (left_step > 0x1D) ? 0x1D : left_step;
	reg = twl6040_read_reg_cache(codec, TWL6040_REG_HFLGAIN);
	reg = 0x1D - reg;
	val = (reg & TWL6040_HF_VOL_MASK);
	if (handsfree->ramp == TWL6040_RAMP_UP) {
		/* ramp step up */
		if (val < handsfree->left_vol) {
			if (val + left_step > handsfree->left_vol)
				val = handsfree->left_vol;
			else
				val += left_step;

			reg &= ~TWL6040_HF_VOL_MASK;
			twl6040_write(codec, TWL6040_REG_HFLGAIN,
						reg | (0x1D - val));
		} else {
			left_complete = 1;
		}
	} else if (handsfree->ramp == TWL6040_RAMP_DOWN) {
		/* ramp step down */
		if (val > 0) {
			if ((int)val - (int)left_step < 0)
				val = 0;
			else
				val -= left_step;

			reg &= ~TWL6040_HF_VOL_MASK;
			twl6040_write(codec, TWL6040_REG_HFLGAIN,
						reg | (0x1D - val));
		} else {
			left_complete = 1;
		}
	}

	/* right channel */
	right_step = (right_step > 0x1D) ? 0x1D : right_step;
	reg = twl6040_read_reg_cache(codec, TWL6040_REG_HFRGAIN);
	reg = 0x1D - reg;
	val = (reg & TWL6040_HF_VOL_MASK);
	if (handsfree->ramp == TWL6040_RAMP_UP) {
		/* ramp step up */
		if (val < handsfree->right_vol) {
			if (val + right_step > handsfree->right_vol)
				val = handsfree->right_vol;
			else
				val += right_step;

			reg &= ~TWL6040_HF_VOL_MASK;
			twl6040_write(codec, TWL6040_REG_HFRGAIN,
						reg | (0x1D - val));
		} else {
			right_complete = 1;
		}
	} else if (handsfree->ramp == TWL6040_RAMP_DOWN) {
		/* ramp step down */
		if (val > 0) {
			if ((int)val - (int)right_step < 0)
				val = 0;
			else
				val -= right_step;

			reg &= ~TWL6040_HF_VOL_MASK;
			twl6040_write(codec, TWL6040_REG_HFRGAIN,
						reg | (0x1D - val));
		}
	}

	return left_complete & right_complete;
}

/*
 * This work ramps both output PGAs at stream start/stop time to
 * minimise pop associated with DAPM power switching.
 */
static void twl6040_pga_hs_work(struct work_struct *work)
{
	struct twl6040_data *priv =
		container_of(work, struct twl6040_data, headset.work.work);
	struct snd_soc_codec *codec = priv->codec;
	struct twl6040_output *headset = &priv->headset;
	int i, headset_complete;

	/* do we need to ramp at all ? */
	if (headset->ramp == TWL6040_RAMP_NONE)
		return;

	/* HS PGA gain range: 0x0 - 0xf (0 - 15) */
	for (i = 0; i < 16; i++) {
		headset_complete = twl6040_hs_ramp_step(codec,
						headset->left_step,
						headset->right_step);

		/* ramp finished ? */
		if (headset_complete)
			break;

		schedule_timeout_interruptible(
				msecs_to_jiffies(headset->step_delay));
	}

	if (headset->ramp == TWL6040_RAMP_DOWN) {
		headset->active = 0;
		complete(&headset->ramp_done);
	} else {
		headset->active = 1;
	}
	headset->ramp = TWL6040_RAMP_NONE;
}

static void twl6040_pga_hf_work(struct work_struct *work)
{
	struct twl6040_data *priv =
		container_of(work, struct twl6040_data, handsfree.work.work);
	struct snd_soc_codec *codec = priv->codec;
	struct twl6040_output *handsfree = &priv->handsfree;
	int i, handsfree_complete;

	/* do we need to ramp at all ? */
	if (handsfree->ramp == TWL6040_RAMP_NONE)
		return;

	/*
	 * HF PGA gain range: 0x00 - 0x1d (0 - 29) */
	for (i = 0; i < 30; i++) {
		handsfree_complete = twl6040_hf_ramp_step(codec,
						handsfree->left_step,
						handsfree->right_step);

		/* ramp finished ? */
		if (handsfree_complete)
			break;

		schedule_timeout_interruptible(
				msecs_to_jiffies(handsfree->step_delay));
	}


	if (handsfree->ramp == TWL6040_RAMP_DOWN) {
		handsfree->active = 0;
		complete(&handsfree->ramp_done);
	} else
		handsfree->active = 1;
	handsfree->ramp = TWL6040_RAMP_NONE;
}

static int out_drv_event(struct snd_soc_dapm_widget *w,
			struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_codec *codec = w->codec;
	struct twl6040_data *priv = snd_soc_codec_get_drvdata(codec);
	struct twl6040_output *out;
	struct delayed_work *work;

	switch (w->shift) {
	case 2: /* Headset output driver */
		out = &priv->headset;
		work = &out->work;
		/*
		 * Make sure, that we do not mess up variables for already
		 * executing work.
		 */
		cancel_delayed_work_sync(work);

		out->left_step = priv->hs_left_step;
		out->right_step = priv->hs_right_step;
		out->step_delay = 5;	/* 5 ms between volume ramp steps */
		break;
	case 4: /* Handsfree output driver */
		out = &priv->handsfree;
		work = &out->work;
		/*
		 * Make sure, that we do not mess up variables for already
		 * executing work.
		 */
		cancel_delayed_work_sync(work);

		out->left_step = priv->hf_left_step;
		out->right_step = priv->hf_right_step;
		out->step_delay = 5;	/* 5 ms between volume ramp steps */
		break;
	default:
		return -1;
	}

	switch (event) {
	case SND_SOC_DAPM_POST_PMU:
		if (out->active)
			break;

		/* don't use volume ramp for power-up */
		out->ramp = TWL6040_RAMP_UP;
		out->left_step = out->left_vol;
		out->right_step = out->right_vol;

		queue_delayed_work(priv->workqueue, work, msecs_to_jiffies(1));
		break;

	case SND_SOC_DAPM_PRE_PMD:
		if (!out->active)
			break;

		/* use volume ramp for power-down */
		out->ramp = TWL6040_RAMP_DOWN;
		INIT_COMPLETION(out->ramp_done);

		queue_delayed_work(priv->workqueue, work, msecs_to_jiffies(1));

		wait_for_completion_timeout(&out->ramp_done,
					    msecs_to_jiffies(2000));
		break;
	}

	return 0;
}

/* set headset dac and driver power mode */
static int headset_power_mode(struct snd_soc_codec *codec, int high_perf)
=======
	twl6040_write(component, TWL6040_REG_HSGAIN, 0xff);
	twl6040_write(component, TWL6040_REG_EARCTL, 0x1e);
	twl6040_write(component, TWL6040_REG_HFLGAIN, 0x1d);
	twl6040_write(component, TWL6040_REG_HFRGAIN, 0x1d);
	twl6040_write(component, TWL6040_REG_LINEGAIN, 0);
}

/* set headset dac and driver power mode */
static int headset_power_mode(struct snd_soc_component *component, int high_perf)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int hslctl, hsrctl;
	int mask = TWL6040_HSDRVMODE | TWL6040_HSDACMODE;

<<<<<<< HEAD
	hslctl = twl6040_read_reg_cache(codec, TWL6040_REG_HSLCTL);
	hsrctl = twl6040_read_reg_cache(codec, TWL6040_REG_HSRCTL);
=======
	hslctl = twl6040_read(component, TWL6040_REG_HSLCTL);
	hsrctl = twl6040_read(component, TWL6040_REG_HSRCTL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (high_perf) {
		hslctl &= ~mask;
		hsrctl &= ~mask;
	} else {
		hslctl |= mask;
		hsrctl |= mask;
	}

<<<<<<< HEAD
	twl6040_write(codec, TWL6040_REG_HSLCTL, hslctl);
	twl6040_write(codec, TWL6040_REG_HSRCTL, hsrctl);
=======
	twl6040_write(component, TWL6040_REG_HSLCTL, hslctl);
	twl6040_write(component, TWL6040_REG_HSRCTL, hsrctl);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static int twl6040_hs_dac_event(struct snd_soc_dapm_widget *w,
			struct snd_kcontrol *kcontrol, int event)
{
<<<<<<< HEAD
	struct snd_soc_codec *codec = w->codec;
=======
	struct snd_soc_component *component = snd_soc_dapm_to_component(w->dapm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u8 hslctl, hsrctl;

	/*
	 * Workaround for Headset DC offset caused pop noise:
	 * Both HS DAC need to be turned on (before the HS driver) and off at
	 * the same time.
	 */
<<<<<<< HEAD
	hslctl = twl6040_read_reg_cache(codec, TWL6040_REG_HSLCTL);
	hsrctl = twl6040_read_reg_cache(codec, TWL6040_REG_HSRCTL);
=======
	hslctl = twl6040_read(component, TWL6040_REG_HSLCTL);
	hsrctl = twl6040_read(component, TWL6040_REG_HSRCTL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (SND_SOC_DAPM_EVENT_ON(event)) {
		hslctl |= TWL6040_HSDACENA;
		hsrctl |= TWL6040_HSDACENA;
	} else {
		hslctl &= ~TWL6040_HSDACENA;
		hsrctl &= ~TWL6040_HSDACENA;
	}
<<<<<<< HEAD
	twl6040_write(codec, TWL6040_REG_HSLCTL, hslctl);
	twl6040_write(codec, TWL6040_REG_HSRCTL, hsrctl);
=======
	twl6040_write(component, TWL6040_REG_HSLCTL, hslctl);
	twl6040_write(component, TWL6040_REG_HSRCTL, hsrctl);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	msleep(1);
	return 0;
}

static int twl6040_ep_drv_event(struct snd_soc_dapm_widget *w,
			struct snd_kcontrol *kcontrol, int event)
{
<<<<<<< HEAD
	struct snd_soc_codec *codec = w->codec;
	struct twl6040_data *priv = snd_soc_codec_get_drvdata(codec);
=======
	struct snd_soc_component *component = snd_soc_dapm_to_component(w->dapm);
	struct twl6040_data *priv = snd_soc_component_get_drvdata(component);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int ret = 0;

	if (SND_SOC_DAPM_EVENT_ON(event)) {
		/* Earphone doesn't support low power mode */
		priv->hs_power_mode_locked = 1;
<<<<<<< HEAD
		ret = headset_power_mode(codec, 1);
	} else {
		priv->hs_power_mode_locked = 0;
		ret = headset_power_mode(codec, priv->hs_power_mode);
=======
		ret = headset_power_mode(component, 1);
	} else {
		priv->hs_power_mode_locked = 0;
		ret = headset_power_mode(component, priv->hs_power_mode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	msleep(1);

	return ret;
}

<<<<<<< HEAD
static void twl6040_hs_jack_report(struct snd_soc_codec *codec,
				   struct snd_soc_jack *jack, int report)
{
	struct twl6040_data *priv = snd_soc_codec_get_drvdata(codec);
=======
static void twl6040_hs_jack_report(struct snd_soc_component *component,
				   struct snd_soc_jack *jack, int report)
{
	struct twl6040_data *priv = snd_soc_component_get_drvdata(component);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int status;

	mutex_lock(&priv->mutex);

	/* Sync status */
<<<<<<< HEAD
	status = twl6040_read_reg_volatile(codec, TWL6040_REG_STATUS);
=======
	status = twl6040_read(component, TWL6040_REG_STATUS);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (status & TWL6040_PLUGCOMP)
		snd_soc_jack_report(jack, report, report);
	else
		snd_soc_jack_report(jack, 0, report);

	mutex_unlock(&priv->mutex);
}

<<<<<<< HEAD
void twl6040_hs_jack_detect(struct snd_soc_codec *codec,
				struct snd_soc_jack *jack, int report)
{
	struct twl6040_data *priv = snd_soc_codec_get_drvdata(codec);
=======
void twl6040_hs_jack_detect(struct snd_soc_component *component,
				struct snd_soc_jack *jack, int report)
{
	struct twl6040_data *priv = snd_soc_component_get_drvdata(component);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct twl6040_jack_data *hs_jack = &priv->hs_jack;

	hs_jack->jack = jack;
	hs_jack->report = report;

<<<<<<< HEAD
	twl6040_hs_jack_report(codec, hs_jack->jack, hs_jack->report);
=======
	twl6040_hs_jack_report(component, hs_jack->jack, hs_jack->report);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(twl6040_hs_jack_detect);

static void twl6040_accessory_work(struct work_struct *work)
{
	struct twl6040_data *priv = container_of(work,
					struct twl6040_data, hs_jack.work.work);
<<<<<<< HEAD
	struct snd_soc_codec *codec = priv->codec;
	struct twl6040_jack_data *hs_jack = &priv->hs_jack;

	twl6040_hs_jack_report(codec, hs_jack->jack, hs_jack->report);
=======
	struct snd_soc_component *component = priv->component;
	struct twl6040_jack_data *hs_jack = &priv->hs_jack;

	twl6040_hs_jack_report(component, hs_jack->jack, hs_jack->report);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* audio interrupt handler */
static irqreturn_t twl6040_audio_handler(int irq, void *data)
{
<<<<<<< HEAD
	struct snd_soc_codec *codec = data;
	struct twl6040_data *priv = snd_soc_codec_get_drvdata(codec);

	queue_delayed_work(priv->workqueue, &priv->hs_jack.work,
			   msecs_to_jiffies(200));
=======
	struct snd_soc_component *component = data;
	struct twl6040_data *priv = snd_soc_component_get_drvdata(component);

	queue_delayed_work(system_power_efficient_wq,
			   &priv->hs_jack.work, msecs_to_jiffies(200));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return IRQ_HANDLED;
}

<<<<<<< HEAD
static int twl6040_put_volsw(struct snd_kcontrol *kcontrol,
				  struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
	struct twl6040_data *twl6040_priv = snd_soc_codec_get_drvdata(codec);
	struct twl6040_output *out = NULL;
	struct soc_mixer_control *mc =
		(struct soc_mixer_control *)kcontrol->private_value;
	int ret;

	/* For HS and HF we shadow the values and only actually write
	 * them out when active in order to ensure the amplifier comes on
	 * as quietly as possible. */
	switch (mc->reg) {
	case TWL6040_REG_HSGAIN:
		out = &twl6040_priv->headset;
		break;
	case TWL6040_REG_HFLGAIN:
		out = &twl6040_priv->handsfree;
		break;
	default:
		dev_warn(codec->dev, "%s: Unexpected register: 0x%02x\n",
					__func__, mc->reg);
		return -EINVAL;
	}

	out->left_vol = ucontrol->value.integer.value[0];
	out->right_vol = ucontrol->value.integer.value[1];
	if (!out->active)
		return 1;

	ret = snd_soc_put_volsw(kcontrol, ucontrol);
	if (ret < 0)
		return ret;

	return 1;
}

static int twl6040_get_volsw(struct snd_kcontrol *kcontrol,
				struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
	struct twl6040_data *twl6040_priv = snd_soc_codec_get_drvdata(codec);
	struct twl6040_output *out = &twl6040_priv->headset;
	struct soc_mixer_control *mc =
		(struct soc_mixer_control *)kcontrol->private_value;

	switch (mc->reg) {
	case TWL6040_REG_HSGAIN:
		out = &twl6040_priv->headset;
		break;
	case TWL6040_REG_HFLGAIN:
		out = &twl6040_priv->handsfree;
		break;
	default:
		dev_warn(codec->dev, "%s: Unexpected register: 0x%02x\n",
					__func__, mc->reg);
		return -EINVAL;
	}

	ucontrol->value.integer.value[0] = out->left_vol;
	ucontrol->value.integer.value[1] = out->right_vol;
	return 0;
}

static int twl6040_soc_dapm_put_vibra_enum(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_dapm_widget_list *wlist = snd_kcontrol_chip(kcontrol);
	struct snd_soc_dapm_widget *widget = wlist->widgets[0];
	struct snd_soc_codec *codec = widget->codec;
=======
static int twl6040_soc_dapm_put_vibra_enum(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_component *component = snd_soc_dapm_kcontrol_component(kcontrol);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct soc_enum *e = (struct soc_enum *)kcontrol->private_value;
	unsigned int val;

	/* Do not allow changes while Input/FF efect is running */
<<<<<<< HEAD
	val = twl6040_read_reg_volatile(codec, e->reg);
=======
	val = twl6040_read(component, e->reg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (val & TWL6040_VIBENA && !(val & TWL6040_VIBSEL))
		return -EBUSY;

	return snd_soc_dapm_put_enum_double(kcontrol, ucontrol);
}

/*
 * MICATT volume control:
 * from -6 to 0 dB in 6 dB steps
 */
static DECLARE_TLV_DB_SCALE(mic_preamp_tlv, -600, 600, 0);

/*
 * MICGAIN volume control:
 * from 6 to 30 dB in 6 dB steps
 */
static DECLARE_TLV_DB_SCALE(mic_amp_tlv, 600, 600, 0);

/*
 * AFMGAIN volume control:
 * from -18 to 24 dB in 6 dB steps
 */
static DECLARE_TLV_DB_SCALE(afm_amp_tlv, -1800, 600, 0);

/*
 * HSGAIN volume control:
 * from -30 to 0 dB in 2 dB steps
 */
static DECLARE_TLV_DB_SCALE(hs_tlv, -3000, 200, 0);

/*
 * HFGAIN volume control:
 * from -52 to 6 dB in 2 dB steps
 */
static DECLARE_TLV_DB_SCALE(hf_tlv, -5200, 200, 0);

/*
 * EPGAIN volume control:
 * from -24 to 6 dB in 2 dB steps
 */
static DECLARE_TLV_DB_SCALE(ep_tlv, -2400, 200, 0);

/* Left analog microphone selection */
static const char *twl6040_amicl_texts[] =
	{"Headset Mic", "Main Mic", "Aux/FM Left", "Off"};

/* Right analog microphone selection */
static const char *twl6040_amicr_texts[] =
	{"Headset Mic", "Sub Mic", "Aux/FM Right", "Off"};

static const struct soc_enum twl6040_enum[] = {
<<<<<<< HEAD
	SOC_ENUM_SINGLE(TWL6040_REG_MICLCTL, 3, 4, twl6040_amicl_texts),
	SOC_ENUM_SINGLE(TWL6040_REG_MICRCTL, 3, 4, twl6040_amicr_texts),
=======
	SOC_ENUM_SINGLE(TWL6040_REG_MICLCTL, 3,
			ARRAY_SIZE(twl6040_amicl_texts), twl6040_amicl_texts),
	SOC_ENUM_SINGLE(TWL6040_REG_MICRCTL, 3,
			ARRAY_SIZE(twl6040_amicr_texts), twl6040_amicr_texts),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static const char *twl6040_hs_texts[] = {
	"Off", "HS DAC", "Line-In amp"
};

static const struct soc_enum twl6040_hs_enum[] = {
	SOC_ENUM_SINGLE(TWL6040_REG_HSLCTL, 5, ARRAY_SIZE(twl6040_hs_texts),
			twl6040_hs_texts),
	SOC_ENUM_SINGLE(TWL6040_REG_HSRCTL, 5, ARRAY_SIZE(twl6040_hs_texts),
			twl6040_hs_texts),
};

static const char *twl6040_hf_texts[] = {
	"Off", "HF DAC", "Line-In amp"
};

static const struct soc_enum twl6040_hf_enum[] = {
	SOC_ENUM_SINGLE(TWL6040_REG_HFLCTL, 2, ARRAY_SIZE(twl6040_hf_texts),
			twl6040_hf_texts),
	SOC_ENUM_SINGLE(TWL6040_REG_HFRCTL, 2, ARRAY_SIZE(twl6040_hf_texts),
			twl6040_hf_texts),
};

static const char *twl6040_vibrapath_texts[] = {
	"Input FF", "Audio PDM"
};

static const struct soc_enum twl6040_vibra_enum[] = {
	SOC_ENUM_SINGLE(TWL6040_REG_VIBCTLL, 1,
			ARRAY_SIZE(twl6040_vibrapath_texts),
			twl6040_vibrapath_texts),
	SOC_ENUM_SINGLE(TWL6040_REG_VIBCTLR, 1,
			ARRAY_SIZE(twl6040_vibrapath_texts),
			twl6040_vibrapath_texts),
};

static const struct snd_kcontrol_new amicl_control =
	SOC_DAPM_ENUM("Route", twl6040_enum[0]);

static const struct snd_kcontrol_new amicr_control =
	SOC_DAPM_ENUM("Route", twl6040_enum[1]);

/* Headset DAC playback switches */
static const struct snd_kcontrol_new hsl_mux_controls =
	SOC_DAPM_ENUM("Route", twl6040_hs_enum[0]);

static const struct snd_kcontrol_new hsr_mux_controls =
	SOC_DAPM_ENUM("Route", twl6040_hs_enum[1]);

/* Handsfree DAC playback switches */
static const struct snd_kcontrol_new hfl_mux_controls =
	SOC_DAPM_ENUM("Route", twl6040_hf_enum[0]);

static const struct snd_kcontrol_new hfr_mux_controls =
	SOC_DAPM_ENUM("Route", twl6040_hf_enum[1]);

static const struct snd_kcontrol_new ep_path_enable_control =
<<<<<<< HEAD
	SOC_DAPM_SINGLE("Switch", TWL6040_REG_SW_SHADOW, 0, 1, 0);
=======
	SOC_DAPM_SINGLE_VIRT("Switch", 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static const struct snd_kcontrol_new auxl_switch_control =
	SOC_DAPM_SINGLE("Switch", TWL6040_REG_HFLCTL, 6, 1, 0);

static const struct snd_kcontrol_new auxr_switch_control =
	SOC_DAPM_SINGLE("Switch", TWL6040_REG_HFRCTL, 6, 1, 0);

/* Vibra playback switches */
static const struct snd_kcontrol_new vibral_mux_controls =
	SOC_DAPM_ENUM_EXT("Route", twl6040_vibra_enum[0],
		snd_soc_dapm_get_enum_double,
		twl6040_soc_dapm_put_vibra_enum);

static const struct snd_kcontrol_new vibrar_mux_controls =
	SOC_DAPM_ENUM_EXT("Route", twl6040_vibra_enum[1],
		snd_soc_dapm_get_enum_double,
		twl6040_soc_dapm_put_vibra_enum);

/* Headset power mode */
static const char *twl6040_power_mode_texts[] = {
<<<<<<< HEAD
	"Low-Power", "High-Perfomance",
};

static const struct soc_enum twl6040_power_mode_enum =
	SOC_ENUM_SINGLE_EXT(ARRAY_SIZE(twl6040_power_mode_texts),
			twl6040_power_mode_texts);
=======
	"Low-Power", "High-Performance",
};

static SOC_ENUM_SINGLE_EXT_DECL(twl6040_power_mode_enum,
				twl6040_power_mode_texts);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int twl6040_headset_power_get_enum(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol)
{
<<<<<<< HEAD
	struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
	struct twl6040_data *priv = snd_soc_codec_get_drvdata(codec);
=======
	struct snd_soc_component *component = snd_soc_kcontrol_component(kcontrol);
	struct twl6040_data *priv = snd_soc_component_get_drvdata(component);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ucontrol->value.enumerated.item[0] = priv->hs_power_mode;

	return 0;
}

static int twl6040_headset_power_put_enum(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol)
{
<<<<<<< HEAD
	struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
	struct twl6040_data *priv = snd_soc_codec_get_drvdata(codec);
=======
	struct snd_soc_component *component = snd_soc_kcontrol_component(kcontrol);
	struct twl6040_data *priv = snd_soc_component_get_drvdata(component);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int high_perf = ucontrol->value.enumerated.item[0];
	int ret = 0;

	if (!priv->hs_power_mode_locked)
<<<<<<< HEAD
		ret = headset_power_mode(codec, high_perf);
=======
		ret = headset_power_mode(component, high_perf);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!ret)
		priv->hs_power_mode = high_perf;

	return ret;
}

static int twl6040_pll_get_enum(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol)
{
<<<<<<< HEAD
	struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
	struct twl6040_data *priv = snd_soc_codec_get_drvdata(codec);
=======
	struct snd_soc_component *component = snd_soc_kcontrol_component(kcontrol);
	struct twl6040_data *priv = snd_soc_component_get_drvdata(component);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ucontrol->value.enumerated.item[0] = priv->pll_power_mode;

	return 0;
}

static int twl6040_pll_put_enum(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol)
{
<<<<<<< HEAD
	struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
	struct twl6040_data *priv = snd_soc_codec_get_drvdata(codec);
=======
	struct snd_soc_component *component = snd_soc_kcontrol_component(kcontrol);
	struct twl6040_data *priv = snd_soc_component_get_drvdata(component);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	priv->pll_power_mode = ucontrol->value.enumerated.item[0];

	return 0;
}

<<<<<<< HEAD
int twl6040_get_dl1_gain(struct snd_soc_codec *codec)
{
	struct snd_soc_dapm_context *dapm = &codec->dapm;
=======
int twl6040_get_dl1_gain(struct snd_soc_component *component)
{
	struct snd_soc_dapm_context *dapm = snd_soc_component_get_dapm(component);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (snd_soc_dapm_get_pin_status(dapm, "EP"))
		return -1; /* -1dB */

	if (snd_soc_dapm_get_pin_status(dapm, "HSOR") ||
		snd_soc_dapm_get_pin_status(dapm, "HSOL")) {

<<<<<<< HEAD
		u8 val = snd_soc_read(codec, TWL6040_REG_HSLCTL);
=======
		u8 val = twl6040_read(component, TWL6040_REG_HSLCTL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (val & TWL6040_HSDACMODE)
			/* HSDACL in LP mode */
			return -8; /* -8dB */
		else
			/* HSDACL in HP mode */
			return -1; /* -1dB */
	}
	return 0; /* 0dB */
}
EXPORT_SYMBOL_GPL(twl6040_get_dl1_gain);

<<<<<<< HEAD
int twl6040_get_clk_id(struct snd_soc_codec *codec)
{
	struct twl6040_data *priv = snd_soc_codec_get_drvdata(codec);
=======
int twl6040_get_clk_id(struct snd_soc_component *component)
{
	struct twl6040_data *priv = snd_soc_component_get_drvdata(component);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return priv->pll_power_mode;
}
EXPORT_SYMBOL_GPL(twl6040_get_clk_id);

<<<<<<< HEAD
int twl6040_get_trim_value(struct snd_soc_codec *codec, enum twl6040_trim trim)
=======
int twl6040_get_trim_value(struct snd_soc_component *component, enum twl6040_trim trim)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	if (unlikely(trim >= TWL6040_TRIM_INVAL))
		return -EINVAL;

<<<<<<< HEAD
	return twl6040_read_reg_cache(codec, TWL6040_REG_TRIM1 + trim);
}
EXPORT_SYMBOL_GPL(twl6040_get_trim_value);

int twl6040_get_hs_step_size(struct snd_soc_codec *codec)
{
	struct twl6040 *twl6040 = codec->control_data;

	if (twl6040_get_revid(twl6040) < TWL6040_REV_ES1_2)
=======
	return twl6040_read(component, TWL6040_REG_TRIM1 + trim);
}
EXPORT_SYMBOL_GPL(twl6040_get_trim_value);

int twl6040_get_hs_step_size(struct snd_soc_component *component)
{
	struct twl6040 *twl6040 = to_twl6040(component);

	if (twl6040_get_revid(twl6040) < TWL6040_REV_ES1_3)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* For ES under ES_1.3 HS step is 2 mV */
		return 2;
	else
		/* For ES_1.3 HS step is 1 mV */
		return 1;
}
EXPORT_SYMBOL_GPL(twl6040_get_hs_step_size);

static const struct snd_kcontrol_new twl6040_snd_controls[] = {
	/* Capture gains */
	SOC_DOUBLE_TLV("Capture Preamplifier Volume",
		TWL6040_REG_MICGAIN, 6, 7, 1, 1, mic_preamp_tlv),
	SOC_DOUBLE_TLV("Capture Volume",
		TWL6040_REG_MICGAIN, 0, 3, 4, 0, mic_amp_tlv),

	/* AFM gains */
	SOC_DOUBLE_TLV("Aux FM Volume",
		TWL6040_REG_LINEGAIN, 0, 3, 7, 0, afm_amp_tlv),

	/* Playback gains */
<<<<<<< HEAD
	SOC_DOUBLE_EXT_TLV("Headset Playback Volume",
		TWL6040_REG_HSGAIN, 0, 4, 0xF, 1, twl6040_get_volsw,
		twl6040_put_volsw, hs_tlv),
	SOC_DOUBLE_R_EXT_TLV("Handsfree Playback Volume",
		TWL6040_REG_HFLGAIN, TWL6040_REG_HFRGAIN, 0, 0x1D, 1,
		twl6040_get_volsw, twl6040_put_volsw, hf_tlv),
=======
	SOC_DOUBLE_TLV("Headset Playback Volume",
		TWL6040_REG_HSGAIN, 0, 4, 0xF, 1, hs_tlv),
	SOC_DOUBLE_R_TLV("Handsfree Playback Volume",
		TWL6040_REG_HFLGAIN, TWL6040_REG_HFRGAIN, 0, 0x1D, 1, hf_tlv),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	SOC_SINGLE_TLV("Earphone Playback Volume",
		TWL6040_REG_EARCTL, 1, 0xF, 1, ep_tlv),

	SOC_ENUM_EXT("Headset Power Mode", twl6040_power_mode_enum,
		twl6040_headset_power_get_enum,
		twl6040_headset_power_put_enum),

<<<<<<< HEAD
=======
	/* Left HS PDM data routed to Right HSDAC */
	SOC_SINGLE("Headset Mono to Stereo Playback Switch",
		TWL6040_REG_HSRCTL, 7, 1, 0),

	/* Left HF PDM data routed to Right HFDAC */
	SOC_SINGLE("Handsfree Mono to Stereo Playback Switch",
		TWL6040_REG_HFRCTL, 5, 1, 0),

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	SOC_ENUM_EXT("PLL Selection", twl6040_power_mode_enum,
		twl6040_pll_get_enum, twl6040_pll_put_enum),
};

static const struct snd_soc_dapm_widget twl6040_dapm_widgets[] = {
	/* Inputs */
	SND_SOC_DAPM_INPUT("MAINMIC"),
	SND_SOC_DAPM_INPUT("HSMIC"),
	SND_SOC_DAPM_INPUT("SUBMIC"),
	SND_SOC_DAPM_INPUT("AFML"),
	SND_SOC_DAPM_INPUT("AFMR"),

	/* Outputs */
	SND_SOC_DAPM_OUTPUT("HSOL"),
	SND_SOC_DAPM_OUTPUT("HSOR"),
	SND_SOC_DAPM_OUTPUT("HFL"),
	SND_SOC_DAPM_OUTPUT("HFR"),
	SND_SOC_DAPM_OUTPUT("EP"),
	SND_SOC_DAPM_OUTPUT("AUXL"),
	SND_SOC_DAPM_OUTPUT("AUXR"),
	SND_SOC_DAPM_OUTPUT("VIBRAL"),
	SND_SOC_DAPM_OUTPUT("VIBRAR"),

	/* Analog input muxes for the capture amplifiers */
	SND_SOC_DAPM_MUX("Analog Left Capture Route",
			SND_SOC_NOPM, 0, 0, &amicl_control),
	SND_SOC_DAPM_MUX("Analog Right Capture Route",
			SND_SOC_NOPM, 0, 0, &amicr_control),

	/* Analog capture PGAs */
	SND_SOC_DAPM_PGA("MicAmpL",
			TWL6040_REG_MICLCTL, 0, 0, NULL, 0),
	SND_SOC_DAPM_PGA("MicAmpR",
			TWL6040_REG_MICRCTL, 0, 0, NULL, 0),

	/* Auxiliary FM PGAs */
	SND_SOC_DAPM_PGA("AFMAmpL",
			TWL6040_REG_MICLCTL, 1, 0, NULL, 0),
	SND_SOC_DAPM_PGA("AFMAmpR",
			TWL6040_REG_MICRCTL, 1, 0, NULL, 0),

	/* ADCs */
<<<<<<< HEAD
	SND_SOC_DAPM_ADC("ADC Left", "Left Front Capture",
			TWL6040_REG_MICLCTL, 2, 0),
	SND_SOC_DAPM_ADC("ADC Right", "Right Front Capture",
			TWL6040_REG_MICRCTL, 2, 0),
=======
	SND_SOC_DAPM_ADC("ADC Left", NULL, TWL6040_REG_MICLCTL, 2, 0),
	SND_SOC_DAPM_ADC("ADC Right", NULL, TWL6040_REG_MICRCTL, 2, 0),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Microphone bias */
	SND_SOC_DAPM_SUPPLY("Headset Mic Bias",
			    TWL6040_REG_AMICBCTL, 0, 0, NULL, 0),
	SND_SOC_DAPM_SUPPLY("Main Mic Bias",
			    TWL6040_REG_AMICBCTL, 4, 0, NULL, 0),
	SND_SOC_DAPM_SUPPLY("Digital Mic1 Bias",
			    TWL6040_REG_DMICBCTL, 0, 0, NULL, 0),
	SND_SOC_DAPM_SUPPLY("Digital Mic2 Bias",
			    TWL6040_REG_DMICBCTL, 4, 0, NULL, 0),

	/* DACs */
<<<<<<< HEAD
	SND_SOC_DAPM_DAC("HSDAC Left", "Headset Playback", SND_SOC_NOPM, 0, 0),
	SND_SOC_DAPM_DAC("HSDAC Right", "Headset Playback", SND_SOC_NOPM, 0, 0),
	SND_SOC_DAPM_DAC("HFDAC Left", "Handsfree Playback",
			 TWL6040_REG_HFLCTL, 0, 0),
	SND_SOC_DAPM_DAC("HFDAC Right", "Handsfree Playback",
			 TWL6040_REG_HFRCTL, 0, 0),
	/* Virtual DAC for vibra path (DL4 channel) */
	SND_SOC_DAPM_DAC("VIBRA DAC", "Vibra Playback",
			SND_SOC_NOPM, 0, 0),
=======
	SND_SOC_DAPM_DAC("HSDAC Left", NULL, SND_SOC_NOPM, 0, 0),
	SND_SOC_DAPM_DAC("HSDAC Right", NULL, SND_SOC_NOPM, 0, 0),
	SND_SOC_DAPM_DAC("HFDAC Left", NULL, TWL6040_REG_HFLCTL, 0, 0),
	SND_SOC_DAPM_DAC("HFDAC Right", NULL, TWL6040_REG_HFRCTL, 0, 0),
	/* Virtual DAC for vibra path (DL4 channel) */
	SND_SOC_DAPM_DAC("VIBRA DAC", NULL, SND_SOC_NOPM, 0, 0),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	SND_SOC_DAPM_MUX("Handsfree Left Playback",
			SND_SOC_NOPM, 0, 0, &hfl_mux_controls),
	SND_SOC_DAPM_MUX("Handsfree Right Playback",
			SND_SOC_NOPM, 0, 0, &hfr_mux_controls),
	/* Analog playback Muxes */
	SND_SOC_DAPM_MUX("Headset Left Playback",
			SND_SOC_NOPM, 0, 0, &hsl_mux_controls),
	SND_SOC_DAPM_MUX("Headset Right Playback",
			SND_SOC_NOPM, 0, 0, &hsr_mux_controls),

	SND_SOC_DAPM_MUX("Vibra Left Playback", SND_SOC_NOPM, 0, 0,
			&vibral_mux_controls),
	SND_SOC_DAPM_MUX("Vibra Right Playback", SND_SOC_NOPM, 0, 0,
			&vibrar_mux_controls),

	SND_SOC_DAPM_SWITCH("Earphone Playback", SND_SOC_NOPM, 0, 0,
			&ep_path_enable_control),
	SND_SOC_DAPM_SWITCH("AUXL Playback", SND_SOC_NOPM, 0, 0,
			&auxl_switch_control),
	SND_SOC_DAPM_SWITCH("AUXR Playback", SND_SOC_NOPM, 0, 0,
			&auxr_switch_control),

	/* Analog playback drivers */
<<<<<<< HEAD
	SND_SOC_DAPM_OUT_DRV_E("HF Left Driver",
			TWL6040_REG_HFLCTL, 4, 0, NULL, 0,
			out_drv_event,
			SND_SOC_DAPM_POST_PMU | SND_SOC_DAPM_PRE_PMD),
	SND_SOC_DAPM_OUT_DRV_E("HF Right Driver",
			TWL6040_REG_HFRCTL, 4, 0, NULL, 0,
			out_drv_event,
			SND_SOC_DAPM_POST_PMU | SND_SOC_DAPM_PRE_PMD),
	SND_SOC_DAPM_OUT_DRV_E("HS Left Driver",
			TWL6040_REG_HSLCTL, 2, 0, NULL, 0,
			out_drv_event,
			SND_SOC_DAPM_POST_PMU | SND_SOC_DAPM_PRE_PMD),
	SND_SOC_DAPM_OUT_DRV_E("HS Right Driver",
			TWL6040_REG_HSRCTL, 2, 0, NULL, 0,
			out_drv_event,
			SND_SOC_DAPM_POST_PMU | SND_SOC_DAPM_PRE_PMD),
=======
	SND_SOC_DAPM_OUT_DRV("HF Left Driver",
			TWL6040_REG_HFLCTL, 4, 0, NULL, 0),
	SND_SOC_DAPM_OUT_DRV("HF Right Driver",
			TWL6040_REG_HFRCTL, 4, 0, NULL, 0),
	SND_SOC_DAPM_OUT_DRV("HS Left Driver",
			TWL6040_REG_HSLCTL, 2, 0, NULL, 0),
	SND_SOC_DAPM_OUT_DRV("HS Right Driver",
			TWL6040_REG_HSRCTL, 2, 0, NULL, 0),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	SND_SOC_DAPM_OUT_DRV_E("Earphone Driver",
			TWL6040_REG_EARCTL, 0, 0, NULL, 0,
			twl6040_ep_drv_event,
			SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD),
	SND_SOC_DAPM_OUT_DRV("Vibra Left Driver",
			TWL6040_REG_VIBCTLL, 0, 0, NULL, 0),
	SND_SOC_DAPM_OUT_DRV("Vibra Right Driver",
			TWL6040_REG_VIBCTLR, 0, 0, NULL, 0),

	SND_SOC_DAPM_SUPPLY("Vibra Left Control", TWL6040_REG_VIBCTLL, 2, 0,
			    NULL, 0),
	SND_SOC_DAPM_SUPPLY("Vibra Right Control", TWL6040_REG_VIBCTLR, 2, 0,
			    NULL, 0),
	SND_SOC_DAPM_SUPPLY_S("HSDAC Power", 1, SND_SOC_NOPM, 0, 0,
			      twl6040_hs_dac_event,
			      SND_SOC_DAPM_POST_PMU | SND_SOC_DAPM_POST_PMD),

	/* Analog playback PGAs */
	SND_SOC_DAPM_PGA("HF Left PGA",
			TWL6040_REG_HFLCTL, 1, 0, NULL, 0),
	SND_SOC_DAPM_PGA("HF Right PGA",
			TWL6040_REG_HFRCTL, 1, 0, NULL, 0),

};

static const struct snd_soc_dapm_route intercon[] = {
<<<<<<< HEAD
=======
	/* Stream -> DAC mapping */
	{"HSDAC Left", NULL, "Legacy Playback"},
	{"HSDAC Left", NULL, "Headset Playback"},
	{"HSDAC Right", NULL, "Legacy Playback"},
	{"HSDAC Right", NULL, "Headset Playback"},

	{"HFDAC Left", NULL, "Legacy Playback"},
	{"HFDAC Left", NULL, "Handsfree Playback"},
	{"HFDAC Right", NULL, "Legacy Playback"},
	{"HFDAC Right", NULL, "Handsfree Playback"},

	{"VIBRA DAC", NULL, "Legacy Playback"},
	{"VIBRA DAC", NULL, "Vibra Playback"},

	/* ADC -> Stream mapping */
	{"Legacy Capture" , NULL, "ADC Left"},
	{"Capture", NULL, "ADC Left"},
	{"Legacy Capture", NULL, "ADC Right"},
	{"Capture" , NULL, "ADC Right"},

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Capture path */
	{"Analog Left Capture Route", "Headset Mic", "HSMIC"},
	{"Analog Left Capture Route", "Main Mic", "MAINMIC"},
	{"Analog Left Capture Route", "Aux/FM Left", "AFML"},

	{"Analog Right Capture Route", "Headset Mic", "HSMIC"},
	{"Analog Right Capture Route", "Sub Mic", "SUBMIC"},
	{"Analog Right Capture Route", "Aux/FM Right", "AFMR"},

	{"MicAmpL", NULL, "Analog Left Capture Route"},
	{"MicAmpR", NULL, "Analog Right Capture Route"},

	{"ADC Left", NULL, "MicAmpL"},
	{"ADC Right", NULL, "MicAmpR"},

	/* AFM path */
	{"AFMAmpL", NULL, "AFML"},
	{"AFMAmpR", NULL, "AFMR"},

	{"HSDAC Left", NULL, "HSDAC Power"},
	{"HSDAC Right", NULL, "HSDAC Power"},

	{"Headset Left Playback", "HS DAC", "HSDAC Left"},
	{"Headset Left Playback", "Line-In amp", "AFMAmpL"},

	{"Headset Right Playback", "HS DAC", "HSDAC Right"},
	{"Headset Right Playback", "Line-In amp", "AFMAmpR"},

	{"HS Left Driver", NULL, "Headset Left Playback"},
	{"HS Right Driver", NULL, "Headset Right Playback"},

	{"HSOL", NULL, "HS Left Driver"},
	{"HSOR", NULL, "HS Right Driver"},

	/* Earphone playback path */
	{"Earphone Playback", "Switch", "HSDAC Left"},
	{"Earphone Driver", NULL, "Earphone Playback"},
	{"EP", NULL, "Earphone Driver"},

	{"Handsfree Left Playback", "HF DAC", "HFDAC Left"},
	{"Handsfree Left Playback", "Line-In amp", "AFMAmpL"},

	{"Handsfree Right Playback", "HF DAC", "HFDAC Right"},
	{"Handsfree Right Playback", "Line-In amp", "AFMAmpR"},

	{"HF Left PGA", NULL, "Handsfree Left Playback"},
	{"HF Right PGA", NULL, "Handsfree Right Playback"},

	{"HF Left Driver", NULL, "HF Left PGA"},
	{"HF Right Driver", NULL, "HF Right PGA"},

	{"HFL", NULL, "HF Left Driver"},
	{"HFR", NULL, "HF Right Driver"},

	{"AUXL Playback", "Switch", "HF Left PGA"},
	{"AUXR Playback", "Switch", "HF Right PGA"},

	{"AUXL", NULL, "AUXL Playback"},
	{"AUXR", NULL, "AUXR Playback"},

	/* Vibrator paths */
	{"Vibra Left Playback", "Audio PDM", "VIBRA DAC"},
	{"Vibra Right Playback", "Audio PDM", "VIBRA DAC"},

	{"Vibra Left Driver", NULL, "Vibra Left Playback"},
	{"Vibra Right Driver", NULL, "Vibra Right Playback"},
	{"Vibra Left Driver", NULL, "Vibra Left Control"},
	{"Vibra Right Driver", NULL, "Vibra Right Control"},

	{"VIBRAL", NULL, "Vibra Left Driver"},
	{"VIBRAR", NULL, "Vibra Right Driver"},
};

<<<<<<< HEAD
static int twl6040_set_bias_level(struct snd_soc_codec *codec,
				enum snd_soc_bias_level level)
{
	struct twl6040 *twl6040 = codec->control_data;
	struct twl6040_data *priv = snd_soc_codec_get_drvdata(codec);
	int ret;
=======
static int twl6040_set_bias_level(struct snd_soc_component *component,
				enum snd_soc_bias_level level)
{
	struct twl6040 *twl6040 = to_twl6040(component);
	struct twl6040_data *priv = snd_soc_component_get_drvdata(component);
	int ret = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (level) {
	case SND_SOC_BIAS_ON:
		break;
	case SND_SOC_BIAS_PREPARE:
		break;
	case SND_SOC_BIAS_STANDBY:
<<<<<<< HEAD
		if (priv->codec_powered)
			break;

		ret = twl6040_power(twl6040, 1);
		if (ret)
			return ret;

		priv->codec_powered = 1;

		twl6040_restore_regs(codec);

		/* Set external boost GPO */
		twl6040_write(codec, TWL6040_REG_GPOCTL, 0x02);
=======
		if (priv->codec_powered) {
			/* Select low power PLL in standby */
			ret = twl6040_set_pll(twl6040, TWL6040_SYSCLK_SEL_LPPLL,
					      32768, 19200000);
			break;
		}

		ret = twl6040_power(twl6040, 1);
		if (ret)
			break;

		priv->codec_powered = 1;

		/* Set external boost GPO */
		twl6040_write(component, TWL6040_REG_GPOCTL, 0x02);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case SND_SOC_BIAS_OFF:
		if (!priv->codec_powered)
			break;

		twl6040_power(twl6040, 0);
		priv->codec_powered = 0;
		break;
	}

<<<<<<< HEAD
	codec->dapm.bias_level = level;

	return 0;
=======
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int twl6040_startup(struct snd_pcm_substream *substream,
			struct snd_soc_dai *dai)
{
<<<<<<< HEAD
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_codec *codec = rtd->codec;
	struct twl6040_data *priv = snd_soc_codec_get_drvdata(codec);
=======
	struct snd_soc_component *component = dai->component;
	struct twl6040_data *priv = snd_soc_component_get_drvdata(component);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	snd_pcm_hw_constraint_list(substream->runtime, 0,
				SNDRV_PCM_HW_PARAM_RATE,
				&sysclk_constraints[priv->pll_power_mode]);

	return 0;
}

static int twl6040_hw_params(struct snd_pcm_substream *substream,
			struct snd_pcm_hw_params *params,
			struct snd_soc_dai *dai)
{
<<<<<<< HEAD
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_codec *codec = rtd->codec;
	struct twl6040_data *priv = snd_soc_codec_get_drvdata(codec);
=======
	struct snd_soc_component *component = dai->component;
	struct twl6040_data *priv = snd_soc_component_get_drvdata(component);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int rate;

	rate = params_rate(params);
	switch (rate) {
	case 11250:
	case 22500:
	case 44100:
	case 88200:
		/* These rates are not supported when HPPLL is in use */
		if (unlikely(priv->pll == TWL6040_SYSCLK_SEL_HPPLL)) {
<<<<<<< HEAD
			dev_err(codec->dev, "HPPLL does not support rate %d\n",
=======
			dev_err(component->dev, "HPPLL does not support rate %d\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				rate);
			return -EINVAL;
		}
		priv->sysclk = 17640000;
		break;
	case 8000:
	case 16000:
	case 32000:
	case 48000:
	case 96000:
		priv->sysclk = 19200000;
		break;
	default:
<<<<<<< HEAD
		dev_err(codec->dev, "unsupported rate %d\n", rate);
=======
		dev_err(component->dev, "unsupported rate %d\n", rate);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;
	}

	return 0;
}

static int twl6040_prepare(struct snd_pcm_substream *substream,
			struct snd_soc_dai *dai)
{
<<<<<<< HEAD
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_codec *codec = rtd->codec;
	struct twl6040 *twl6040 = codec->control_data;
	struct twl6040_data *priv = snd_soc_codec_get_drvdata(codec);
	int ret;

	if (!priv->sysclk) {
		dev_err(codec->dev,
=======
	struct snd_soc_component *component = dai->component;
	struct twl6040 *twl6040 = to_twl6040(component);
	struct twl6040_data *priv = snd_soc_component_get_drvdata(component);
	int ret;

	if (!priv->sysclk) {
		dev_err(component->dev,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			"no mclk configured, call set_sysclk() on init\n");
		return -EINVAL;
	}

	ret = twl6040_set_pll(twl6040, priv->pll, priv->clk_in, priv->sysclk);
	if (ret) {
<<<<<<< HEAD
		dev_err(codec->dev, "Can not set PLL (%d)\n", ret);
=======
		dev_err(component->dev, "Can not set PLL (%d)\n", ret);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EPERM;
	}

	return 0;
}

static int twl6040_set_dai_sysclk(struct snd_soc_dai *codec_dai,
		int clk_id, unsigned int freq, int dir)
{
<<<<<<< HEAD
	struct snd_soc_codec *codec = codec_dai->codec;
	struct twl6040_data *priv = snd_soc_codec_get_drvdata(codec);
=======
	struct snd_soc_component *component = codec_dai->component;
	struct twl6040_data *priv = snd_soc_component_get_drvdata(component);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (clk_id) {
	case TWL6040_SYSCLK_SEL_LPPLL:
	case TWL6040_SYSCLK_SEL_HPPLL:
		priv->pll = clk_id;
		priv->clk_in = freq;
		break;
	default:
<<<<<<< HEAD
		dev_err(codec->dev, "unknown clk_id %d\n", clk_id);
=======
		dev_err(component->dev, "unknown clk_id %d\n", clk_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;
	}

	return 0;
}

<<<<<<< HEAD
=======
static void twl6040_mute_path(struct snd_soc_component *component, enum twl6040_dai_id id,
			     int mute)
{
	struct twl6040 *twl6040 = to_twl6040(component);
	struct twl6040_data *priv = snd_soc_component_get_drvdata(component);
	int hslctl, hsrctl, earctl;
	int hflctl, hfrctl;

	switch (id) {
	case TWL6040_DAI_DL1:
		hslctl = twl6040_read(component, TWL6040_REG_HSLCTL);
		hsrctl = twl6040_read(component, TWL6040_REG_HSRCTL);
		earctl = twl6040_read(component, TWL6040_REG_EARCTL);

		if (mute) {
			/* Power down drivers and DACs */
			earctl &= ~0x01;
			hslctl &= ~(TWL6040_HSDRVENA | TWL6040_HSDACENA);
			hsrctl &= ~(TWL6040_HSDRVENA | TWL6040_HSDACENA);

		}

		twl6040_reg_write(twl6040, TWL6040_REG_EARCTL, earctl);
		twl6040_reg_write(twl6040, TWL6040_REG_HSLCTL, hslctl);
		twl6040_reg_write(twl6040, TWL6040_REG_HSRCTL, hsrctl);
		priv->dl1_unmuted = !mute;
		break;
	case TWL6040_DAI_DL2:
		hflctl = twl6040_read(component, TWL6040_REG_HFLCTL);
		hfrctl = twl6040_read(component, TWL6040_REG_HFRCTL);

		if (mute) {
			/* Power down drivers and DACs */
			hflctl &= ~(TWL6040_HFDACENA | TWL6040_HFPGAENA |
				    TWL6040_HFDRVENA | TWL6040_HFSWENA);
			hfrctl &= ~(TWL6040_HFDACENA | TWL6040_HFPGAENA |
				    TWL6040_HFDRVENA | TWL6040_HFSWENA);
		}

		twl6040_reg_write(twl6040, TWL6040_REG_HFLCTL, hflctl);
		twl6040_reg_write(twl6040, TWL6040_REG_HFRCTL, hfrctl);
		priv->dl2_unmuted = !mute;
		break;
	default:
		break;
	}
}

static int twl6040_mute_stream(struct snd_soc_dai *dai, int mute, int direction)
{
	switch (dai->id) {
	case TWL6040_DAI_LEGACY:
		twl6040_mute_path(dai->component, TWL6040_DAI_DL1, mute);
		twl6040_mute_path(dai->component, TWL6040_DAI_DL2, mute);
		break;
	case TWL6040_DAI_DL1:
	case TWL6040_DAI_DL2:
		twl6040_mute_path(dai->component, dai->id, mute);
		break;
	default:
		break;
	}

	return 0;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static const struct snd_soc_dai_ops twl6040_dai_ops = {
	.startup	= twl6040_startup,
	.hw_params	= twl6040_hw_params,
	.prepare	= twl6040_prepare,
	.set_sysclk	= twl6040_set_dai_sysclk,
<<<<<<< HEAD
=======
	.mute_stream	= twl6040_mute_stream,
	.no_capture_mute = 1,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static struct snd_soc_dai_driver twl6040_dai[] = {
{
	.name = "twl6040-legacy",
<<<<<<< HEAD
	.playback = {
		.stream_name = "Playback",
=======
	.id = TWL6040_DAI_LEGACY,
	.playback = {
		.stream_name = "Legacy Playback",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.channels_min = 1,
		.channels_max = 5,
		.rates = TWL6040_RATES,
		.formats = TWL6040_FORMATS,
	},
	.capture = {
<<<<<<< HEAD
		.stream_name = "Capture",
=======
		.stream_name = "Legacy Capture",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.channels_min = 1,
		.channels_max = 2,
		.rates = TWL6040_RATES,
		.formats = TWL6040_FORMATS,
	},
	.ops = &twl6040_dai_ops,
},
{
	.name = "twl6040-ul",
<<<<<<< HEAD
=======
	.id = TWL6040_DAI_UL,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.capture = {
		.stream_name = "Capture",
		.channels_min = 1,
		.channels_max = 2,
		.rates = TWL6040_RATES,
		.formats = TWL6040_FORMATS,
	},
	.ops = &twl6040_dai_ops,
},
{
	.name = "twl6040-dl1",
<<<<<<< HEAD
=======
	.id = TWL6040_DAI_DL1,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.playback = {
		.stream_name = "Headset Playback",
		.channels_min = 1,
		.channels_max = 2,
		.rates = TWL6040_RATES,
		.formats = TWL6040_FORMATS,
	},
	.ops = &twl6040_dai_ops,
},
{
	.name = "twl6040-dl2",
<<<<<<< HEAD
=======
	.id = TWL6040_DAI_DL2,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.playback = {
		.stream_name = "Handsfree Playback",
		.channels_min = 1,
		.channels_max = 2,
		.rates = TWL6040_RATES,
		.formats = TWL6040_FORMATS,
	},
	.ops = &twl6040_dai_ops,
},
{
	.name = "twl6040-vib",
<<<<<<< HEAD
=======
	.id = TWL6040_DAI_VIB,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.playback = {
		.stream_name = "Vibra Playback",
		.channels_min = 1,
		.channels_max = 1,
		.rates = SNDRV_PCM_RATE_CONTINUOUS,
		.formats = TWL6040_FORMATS,
	},
	.ops = &twl6040_dai_ops,
},
};

<<<<<<< HEAD
#ifdef CONFIG_PM
static int twl6040_suspend(struct snd_soc_codec *codec)
{
	twl6040_set_bias_level(codec, SND_SOC_BIAS_OFF);

	return 0;
}

static int twl6040_resume(struct snd_soc_codec *codec)
{
	twl6040_set_bias_level(codec, SND_SOC_BIAS_STANDBY);
	twl6040_set_bias_level(codec, codec->dapm.suspend_bias_level);

	return 0;
}
#else
#define twl6040_suspend NULL
#define twl6040_resume NULL
#endif

static int twl6040_probe(struct snd_soc_codec *codec)
{
	struct twl6040_data *priv;
	struct twl6040_codec_data *pdata = dev_get_platdata(codec->dev);
	struct platform_device *pdev = container_of(codec->dev,
						   struct platform_device, dev);
	int ret = 0;

	priv = kzalloc(sizeof(struct twl6040_data), GFP_KERNEL);
	if (priv == NULL)
		return -ENOMEM;
	snd_soc_codec_set_drvdata(codec, priv);

	priv->codec = codec;
	codec->control_data = dev_get_drvdata(codec->dev->parent);

	if (pdata && pdata->hs_left_step && pdata->hs_right_step) {
		priv->hs_left_step = pdata->hs_left_step;
		priv->hs_right_step = pdata->hs_right_step;
	} else {
		priv->hs_left_step = 1;
		priv->hs_right_step = 1;
	}

	if (pdata && pdata->hf_left_step && pdata->hf_right_step) {
		priv->hf_left_step = pdata->hf_left_step;
		priv->hf_right_step = pdata->hf_right_step;
	} else {
		priv->hf_left_step = 1;
		priv->hf_right_step = 1;
	}

	priv->plug_irq = platform_get_irq(pdev, 0);
	if (priv->plug_irq < 0) {
		dev_err(codec->dev, "invalid irq\n");
		ret = -EINVAL;
		goto work_err;
	}

	priv->workqueue = alloc_workqueue("twl6040-codec", 0, 0);
	if (!priv->workqueue) {
		ret = -ENOMEM;
		goto work_err;
	}

	INIT_DELAYED_WORK(&priv->hs_jack.work, twl6040_accessory_work);
	INIT_DELAYED_WORK(&priv->headset.work, twl6040_pga_hs_work);
	INIT_DELAYED_WORK(&priv->handsfree.work, twl6040_pga_hf_work);

	mutex_init(&priv->mutex);

	init_completion(&priv->headset.ramp_done);
	init_completion(&priv->handsfree.ramp_done);

	ret = request_threaded_irq(priv->plug_irq, NULL, twl6040_audio_handler,
				   0, "twl6040_irq_plug", codec);
	if (ret) {
		dev_err(codec->dev, "PLUG IRQ request failed: %d\n", ret);
		goto plugirq_err;
	}

	twl6040_init_chip(codec);

	/* power on device */
	ret = twl6040_set_bias_level(codec, SND_SOC_BIAS_STANDBY);
	if (!ret)
		return 0;

	/* Error path */
	free_irq(priv->plug_irq, codec);
plugirq_err:
	destroy_workqueue(priv->workqueue);
work_err:
	kfree(priv);
	return ret;
}

static int twl6040_remove(struct snd_soc_codec *codec)
{
	struct twl6040_data *priv = snd_soc_codec_get_drvdata(codec);

	twl6040_set_bias_level(codec, SND_SOC_BIAS_OFF);
	free_irq(priv->plug_irq, codec);
	destroy_workqueue(priv->workqueue);
	kfree(priv);
=======
static int twl6040_probe(struct snd_soc_component *component)
{
	struct twl6040_data *priv;
	struct platform_device *pdev = to_platform_device(component->dev);
	int ret = 0;

	priv = devm_kzalloc(component->dev, sizeof(*priv), GFP_KERNEL);
	if (priv == NULL)
		return -ENOMEM;

	snd_soc_component_set_drvdata(component, priv);

	priv->component = component;

	priv->plug_irq = platform_get_irq(pdev, 0);
	if (priv->plug_irq < 0)
		return priv->plug_irq;

	INIT_DELAYED_WORK(&priv->hs_jack.work, twl6040_accessory_work);

	mutex_init(&priv->mutex);

	ret = request_threaded_irq(priv->plug_irq, NULL,
					twl6040_audio_handler,
					IRQF_NO_SUSPEND | IRQF_ONESHOT,
					"twl6040_irq_plug", component);
	if (ret) {
		dev_err(component->dev, "PLUG IRQ request failed: %d\n", ret);
		return ret;
	}

	snd_soc_component_force_bias_level(component, SND_SOC_BIAS_STANDBY);
	twl6040_init_chip(component);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static struct snd_soc_codec_driver soc_codec_dev_twl6040 = {
	.probe = twl6040_probe,
	.remove = twl6040_remove,
	.suspend = twl6040_suspend,
	.resume = twl6040_resume,
	.read = twl6040_read_reg_cache,
	.write = twl6040_write,
	.set_bias_level = twl6040_set_bias_level,
	.reg_cache_size = ARRAY_SIZE(twl6040_reg),
	.reg_word_size = sizeof(u8),
	.reg_cache_default = twl6040_reg,
	.ignore_pmdown_time = true,

	.controls = twl6040_snd_controls,
	.num_controls = ARRAY_SIZE(twl6040_snd_controls),
	.dapm_widgets = twl6040_dapm_widgets,
	.num_dapm_widgets = ARRAY_SIZE(twl6040_dapm_widgets),
	.dapm_routes = intercon,
	.num_dapm_routes = ARRAY_SIZE(intercon),
};

static int __devinit twl6040_codec_probe(struct platform_device *pdev)
{
	return snd_soc_register_codec(&pdev->dev, &soc_codec_dev_twl6040,
				      twl6040_dai, ARRAY_SIZE(twl6040_dai));
}

static int __devexit twl6040_codec_remove(struct platform_device *pdev)
{
	snd_soc_unregister_codec(&pdev->dev);
	return 0;
}

static struct platform_driver twl6040_codec_driver = {
	.driver = {
		.name = "twl6040-codec",
		.owner = THIS_MODULE,
	},
	.probe = twl6040_codec_probe,
	.remove = __devexit_p(twl6040_codec_remove),
=======
static void twl6040_remove(struct snd_soc_component *component)
{
	struct twl6040_data *priv = snd_soc_component_get_drvdata(component);

	free_irq(priv->plug_irq, component);
}

static const struct snd_soc_component_driver soc_component_dev_twl6040 = {
	.probe			= twl6040_probe,
	.remove			= twl6040_remove,
	.read			= twl6040_read,
	.write			= twl6040_write,
	.set_bias_level		= twl6040_set_bias_level,
	.controls		= twl6040_snd_controls,
	.num_controls		= ARRAY_SIZE(twl6040_snd_controls),
	.dapm_widgets		= twl6040_dapm_widgets,
	.num_dapm_widgets	= ARRAY_SIZE(twl6040_dapm_widgets),
	.dapm_routes		= intercon,
	.num_dapm_routes	= ARRAY_SIZE(intercon),
	.suspend_bias_off	= 1,
	.idle_bias_on		= 1,
	.endianness		= 1,
};

static int twl6040_codec_probe(struct platform_device *pdev)
{
	return devm_snd_soc_register_component(&pdev->dev,
				      &soc_component_dev_twl6040,
				      twl6040_dai, ARRAY_SIZE(twl6040_dai));
}

static struct platform_driver twl6040_codec_driver = {
	.driver = {
		.name = "twl6040-codec",
	},
	.probe = twl6040_codec_probe,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

module_platform_driver(twl6040_codec_driver);

MODULE_DESCRIPTION("ASoC TWL6040 codec driver");
MODULE_AUTHOR("Misael Lopez Cruz");
MODULE_LICENSE("GPL");
