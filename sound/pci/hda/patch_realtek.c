<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Universal Interface for Intel High Definition Audio Codec
 *
 * HD audio interface patch for Realtek ALC codecs
 *
 * Copyright (c) 2004 Kailang Yang <kailang@realtek.com.tw>
 *                    PeiSen Hou <pshou@realtek.com.tw>
 *                    Takashi Iwai <tiwai@suse.de>
<<<<<<< HEAD
 *                    Jonathan Woithe <jwoithe@physics.adelaide.edu.au>
 *
 *  This driver is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This driver is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

=======
 *                    Jonathan Woithe <jwoithe@just42.net>
 */

#include <linux/acpi.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/pci.h>
<<<<<<< HEAD
#include <linux/module.h>
#include <sound/core.h>
#include <sound/jack.h>
#include "hda_codec.h"
#include "hda_local.h"
#include "hda_beep.h"
#include "hda_jack.h"

/* unsol event tags */
#define ALC_FRONT_EVENT		0x01
#define ALC_DCVOL_EVENT		0x02
#define ALC_HP_EVENT		0x04
#define ALC_MIC_EVENT		0x08

/* for GPIO Poll */
#define GPIO_MASK	0x03

/* extra amp-initialization sequence types */
enum {
	ALC_INIT_NONE,
	ALC_INIT_DEFAULT,
	ALC_INIT_GPIO1,
	ALC_INIT_GPIO2,
	ALC_INIT_GPIO3,
=======
#include <linux/dmi.h>
#include <linux/module.h>
#include <linux/input.h>
#include <linux/leds.h>
#include <linux/ctype.h>
#include <sound/core.h>
#include <sound/jack.h>
#include <sound/hda_codec.h>
#include "hda_local.h"
#include "hda_auto_parser.h"
#include "hda_jack.h"
#include "hda_generic.h"
#include "hda_component.h"

/* keep halting ALC5505 DSP, for power saving */
#define HALT_REALTEK_ALC5505

/* extra amp-initialization sequence types */
enum {
	ALC_INIT_UNDEFINED,
	ALC_INIT_NONE,
	ALC_INIT_DEFAULT,
};

enum {
	ALC_HEADSET_MODE_UNKNOWN,
	ALC_HEADSET_MODE_UNPLUGGED,
	ALC_HEADSET_MODE_HEADSET,
	ALC_HEADSET_MODE_MIC,
	ALC_HEADSET_MODE_HEADPHONE,
};

enum {
	ALC_HEADSET_TYPE_UNKNOWN,
	ALC_HEADSET_TYPE_CTIA,
	ALC_HEADSET_TYPE_OMTP,
};

enum {
	ALC_KEY_MICMUTE_INDEX,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct alc_customize_define {
	unsigned int  sku_cfg;
	unsigned char port_connectivity;
	unsigned char check_sum;
	unsigned char customization;
	unsigned char external_amp;
	unsigned int  enable_pcbeep:1;
	unsigned int  platform_type:1;
	unsigned int  swap:1;
	unsigned int  override:1;
	unsigned int  fixup:1; /* Means that this sku is set by driver, not read from hw */
};

<<<<<<< HEAD
struct alc_fixup;

struct alc_multi_io {
	hda_nid_t pin;		/* multi-io widget pin NID */
	hda_nid_t dac;		/* DAC to be connected */
	unsigned int ctl_in;	/* cached input-pin control value */
};

enum {
	ALC_AUTOMUTE_PIN,	/* change the pin control */
	ALC_AUTOMUTE_AMP,	/* mute/unmute the pin AMP */
	ALC_AUTOMUTE_MIXER,	/* mute/unmute mixer widget AMP */
};

#define MAX_VOL_NIDS	0x40

struct alc_spec {
	/* codec parameterization */
	const struct snd_kcontrol_new *mixers[5];	/* mixer arrays */
	unsigned int num_mixers;
	const struct snd_kcontrol_new *cap_mixer;	/* capture mixer */
	unsigned int beep_amp;	/* beep amp value, set via set_beep_amp() */

	const struct hda_verb *init_verbs[10];	/* initialization verbs
						 * don't forget NULL
						 * termination!
						 */
	unsigned int num_init_verbs;

	char stream_name_analog[32];	/* analog PCM stream */
	const struct hda_pcm_stream *stream_analog_playback;
	const struct hda_pcm_stream *stream_analog_capture;
	const struct hda_pcm_stream *stream_analog_alt_playback;
	const struct hda_pcm_stream *stream_analog_alt_capture;

	char stream_name_digital[32];	/* digital PCM stream */
	const struct hda_pcm_stream *stream_digital_playback;
	const struct hda_pcm_stream *stream_digital_capture;

	/* playback */
	struct hda_multi_out multiout;	/* playback set-up
					 * max_channels, dacs must be set
					 * dig_out_nid and hp_nid are optional
					 */
	hda_nid_t alt_dac_nid;
	hda_nid_t slave_dig_outs[3];	/* optional - for auto-parsing */
	int dig_out_type;

	/* capture */
	unsigned int num_adc_nids;
	const hda_nid_t *adc_nids;
	const hda_nid_t *capsrc_nids;
	hda_nid_t dig_in_nid;		/* digital-in NID; optional */
	hda_nid_t mixer_nid;		/* analog-mixer NID */
	DECLARE_BITMAP(vol_ctls, MAX_VOL_NIDS << 1);
	DECLARE_BITMAP(sw_ctls, MAX_VOL_NIDS << 1);

	/* capture setup for dynamic dual-adc switch */
	hda_nid_t cur_adc;
	unsigned int cur_adc_stream_tag;
	unsigned int cur_adc_format;

	/* capture source */
	unsigned int num_mux_defs;
	const struct hda_input_mux *input_mux;
	unsigned int cur_mux[3];
	hda_nid_t ext_mic_pin;
	hda_nid_t dock_mic_pin;
	hda_nid_t int_mic_pin;

	/* channel model */
	const struct hda_channel_mode *channel_mode;
	int num_channel_mode;
	int need_dac_fix;
	int const_channel_count;
	int ext_channel_count;

	/* PCM information */
	struct hda_pcm pcm_rec[3];	/* used in alc_build_pcms() */

	/* dynamic controls, init_verbs and input_mux */
	struct auto_pin_cfg autocfg;
	struct alc_customize_define cdefine;
	struct snd_array kctls;
	struct hda_input_mux private_imux[3];
	hda_nid_t private_dac_nids[AUTO_CFG_MAX_OUTS];
	hda_nid_t private_adc_nids[AUTO_CFG_MAX_OUTS];
	hda_nid_t private_capsrc_nids[AUTO_CFG_MAX_OUTS];
	hda_nid_t imux_pins[HDA_MAX_NUM_INPUTS];
	unsigned int dyn_adc_idx[HDA_MAX_NUM_INPUTS];
	int int_mic_idx, ext_mic_idx, dock_mic_idx; /* for auto-mic */

	/* hooks */
	void (*init_hook)(struct hda_codec *codec);
	void (*unsol_event)(struct hda_codec *codec, unsigned int res);
#ifdef CONFIG_SND_HDA_POWER_SAVE
	void (*power_hook)(struct hda_codec *codec);
#endif
	void (*shutup)(struct hda_codec *codec);
	void (*automute_hook)(struct hda_codec *codec);

	/* for pin sensing */
	unsigned int hp_jack_present:1;
	unsigned int line_jack_present:1;
	unsigned int master_mute:1;
	unsigned int auto_mic:1;
	unsigned int auto_mic_valid_imux:1;	/* valid imux for auto-mic */
	unsigned int automute_speaker:1; /* automute speaker outputs */
	unsigned int automute_lo:1; /* automute LO outputs */
	unsigned int detect_hp:1;	/* Headphone detection enabled */
	unsigned int detect_lo:1;	/* Line-out detection enabled */
	unsigned int automute_speaker_possible:1; /* there are speakers and either LO or HP */
	unsigned int automute_lo_possible:1;	  /* there are line outs and HP */
	unsigned int keep_vref_in_automute:1; /* Don't clear VREF in automute */

	/* other flags */
	unsigned int no_analog :1; /* digital I/O only */
	unsigned int dyn_adc_switch:1; /* switch ADCs (for ALC275) */
	unsigned int single_input_src:1;
	unsigned int vol_in_capsrc:1; /* use capsrc volume (ADC has no vol) */
	unsigned int parse_flags; /* passed to snd_hda_parse_pin_defcfg() */
	unsigned int shared_mic_hp:1; /* HP/Mic-in sharing */
	unsigned int no_primary_hp:1; /* Don't prefer HP pins to speaker pins */

	/* auto-mute control */
	int automute_mode;
	hda_nid_t automute_mixer_nid[AUTO_CFG_MAX_OUTS];

	int init_amp;
	int codec_variant;	/* flag for other variants */

	/* for virtual master */
	hda_nid_t vmaster_nid;
	struct hda_vmaster_mute_hook vmaster_mute;
#ifdef CONFIG_SND_HDA_POWER_SAVE
	struct hda_loopback_check loopback;
	int num_loopbacks;
	struct hda_amp_list loopback_list[8];
#endif
=======
struct alc_coef_led {
	unsigned int idx;
	unsigned int mask;
	unsigned int on;
	unsigned int off;
};

struct alc_spec {
	struct hda_gen_spec gen; /* must be at head */

	/* codec parameterization */
	struct alc_customize_define cdefine;
	unsigned int parse_flags; /* flag for snd_hda_parse_pin_defcfg() */

	/* GPIO bits */
	unsigned int gpio_mask;
	unsigned int gpio_dir;
	unsigned int gpio_data;
	bool gpio_write_delay;	/* add a delay before writing gpio_data */

	/* mute LED for HP laptops, see vref_mute_led_set() */
	int mute_led_polarity;
	int micmute_led_polarity;
	hda_nid_t mute_led_nid;
	hda_nid_t cap_mute_led_nid;

	unsigned int gpio_mute_led_mask;
	unsigned int gpio_mic_led_mask;
	struct alc_coef_led mute_led_coef;
	struct alc_coef_led mic_led_coef;
	struct mutex coef_mutex;

	hda_nid_t headset_mic_pin;
	hda_nid_t headphone_mic_pin;
	int current_headset_mode;
	int current_headset_type;

	/* hooks */
	void (*init_hook)(struct hda_codec *codec);
#ifdef CONFIG_PM
	void (*power_hook)(struct hda_codec *codec);
#endif
	void (*shutup)(struct hda_codec *codec);

	int init_amp;
	int codec_variant;	/* flag for other variants */
	unsigned int has_alc5505_dsp:1;
	unsigned int no_depop_delay:1;
	unsigned int done_hp_init:1;
	unsigned int no_shutup_pins:1;
	unsigned int ultra_low_power:1;
	unsigned int has_hs_key:1;
	unsigned int no_internal_mic_pin:1;
	unsigned int en_3kpull_low:1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* for PLL fix */
	hda_nid_t pll_nid;
	unsigned int pll_coef_idx, pll_coef_bit;
	unsigned int coef0;
<<<<<<< HEAD

	/* fix-up list */
	int fixup_id;
	const struct alc_fixup *fixup_list;
	const char *fixup_name;

	/* multi-io */
	int multi_ios;
	struct alc_multi_io multi_io[4];

	/* bind volumes */
	struct snd_array bind_ctls;
};

static bool check_amp_caps(struct hda_codec *codec, hda_nid_t nid,
			   int dir, unsigned int bits)
{
	if (!nid)
		return false;
	if (get_wcaps(codec, nid) & (1 << (dir + 1)))
		if (query_amp_caps(codec, nid, dir) & bits)
			return true;
	return false;
}

#define nid_has_mute(codec, nid, dir) \
	check_amp_caps(codec, nid, dir, AC_AMPCAP_MUTE)
#define nid_has_volume(codec, nid, dir) \
	check_amp_caps(codec, nid, dir, AC_AMPCAP_NUM_STEPS)

/*
 * input MUX handling
 */
static int alc_mux_enum_info(struct snd_kcontrol *kcontrol,
			     struct snd_ctl_elem_info *uinfo)
{
	struct hda_codec *codec = snd_kcontrol_chip(kcontrol);
	struct alc_spec *spec = codec->spec;
	unsigned int mux_idx = snd_ctl_get_ioffidx(kcontrol, &uinfo->id);
	if (mux_idx >= spec->num_mux_defs)
		mux_idx = 0;
	if (!spec->input_mux[mux_idx].num_items && mux_idx > 0)
		mux_idx = 0;
	return snd_hda_input_mux_info(&spec->input_mux[mux_idx], uinfo);
}

static int alc_mux_enum_get(struct snd_kcontrol *kcontrol,
			    struct snd_ctl_elem_value *ucontrol)
{
	struct hda_codec *codec = snd_kcontrol_chip(kcontrol);
	struct alc_spec *spec = codec->spec;
	unsigned int adc_idx = snd_ctl_get_ioffidx(kcontrol, &ucontrol->id);

	ucontrol->value.enumerated.item[0] = spec->cur_mux[adc_idx];
	return 0;
}

static bool alc_dyn_adc_pcm_resetup(struct hda_codec *codec, int cur)
{
	struct alc_spec *spec = codec->spec;
	hda_nid_t new_adc = spec->adc_nids[spec->dyn_adc_idx[cur]];

	if (spec->cur_adc && spec->cur_adc != new_adc) {
		/* stream is running, let's swap the current ADC */
		__snd_hda_codec_cleanup_stream(codec, spec->cur_adc, 1);
		spec->cur_adc = new_adc;
		snd_hda_codec_setup_stream(codec, new_adc,
					   spec->cur_adc_stream_tag, 0,
					   spec->cur_adc_format);
		return true;
	}
	return false;
}

static inline hda_nid_t get_capsrc(struct alc_spec *spec, int idx)
{
	return spec->capsrc_nids ?
		spec->capsrc_nids[idx] : spec->adc_nids[idx];
}

static void call_update_outputs(struct hda_codec *codec);

/* select the given imux item; either unmute exclusively or select the route */
static int alc_mux_select(struct hda_codec *codec, unsigned int adc_idx,
			  unsigned int idx, bool force)
{
	struct alc_spec *spec = codec->spec;
	const struct hda_input_mux *imux;
	unsigned int mux_idx;
	int i, type, num_conns;
	hda_nid_t nid;

	if (!spec->input_mux)
		return 0;

	mux_idx = adc_idx >= spec->num_mux_defs ? 0 : adc_idx;
	imux = &spec->input_mux[mux_idx];
	if (!imux->num_items && mux_idx > 0)
		imux = &spec->input_mux[0];
	if (!imux->num_items)
		return 0;

	if (idx >= imux->num_items)
		idx = imux->num_items - 1;
	if (spec->cur_mux[adc_idx] == idx && !force)
		return 0;
	spec->cur_mux[adc_idx] = idx;

	/* for shared I/O, change the pin-control accordingly */
	if (spec->shared_mic_hp) {
		/* NOTE: this assumes that there are only two inputs, the
		 * first is the real internal mic and the second is HP jack.
		 */
		snd_hda_codec_write(codec, spec->autocfg.inputs[1].pin, 0,
				    AC_VERB_SET_PIN_WIDGET_CONTROL,
				    spec->cur_mux[adc_idx] ?
				    PIN_VREF80 : PIN_HP);
		spec->automute_speaker = !spec->cur_mux[adc_idx];
		call_update_outputs(codec);
	}

	if (spec->dyn_adc_switch) {
		alc_dyn_adc_pcm_resetup(codec, idx);
		adc_idx = spec->dyn_adc_idx[idx];
	}

	nid = get_capsrc(spec, adc_idx);

	/* no selection? */
	num_conns = snd_hda_get_conn_list(codec, nid, NULL);
	if (num_conns <= 1)
		return 1;

	type = get_wcaps_type(get_wcaps(codec, nid));
	if (type == AC_WID_AUD_MIX) {
		/* Matrix-mixer style (e.g. ALC882) */
		int active = imux->items[idx].index;
		for (i = 0; i < num_conns; i++) {
			unsigned int v = (i == active) ? 0 : HDA_AMP_MUTE;
			snd_hda_codec_amp_stereo(codec, nid, HDA_INPUT, i,
						 HDA_AMP_MUTE, v);
		}
	} else {
		/* MUX style (e.g. ALC880) */
		snd_hda_codec_write_cache(codec, nid, 0,
					  AC_VERB_SET_CONNECT_SEL,
					  imux->items[idx].index);
	}
	return 1;
}

static int alc_mux_enum_put(struct snd_kcontrol *kcontrol,
			    struct snd_ctl_elem_value *ucontrol)
{
	struct hda_codec *codec = snd_kcontrol_chip(kcontrol);
	unsigned int adc_idx = snd_ctl_get_ioffidx(kcontrol, &ucontrol->id);
	return alc_mux_select(codec, adc_idx,
			      ucontrol->value.enumerated.item[0], false);
}

/*
 * set up the input pin config (depending on the given auto-pin type)
 */
static void alc_set_input_pin(struct hda_codec *codec, hda_nid_t nid,
			      int auto_pin_type)
{
	unsigned int val = PIN_IN;

	if (auto_pin_type == AUTO_PIN_MIC) {
		unsigned int pincap;
		unsigned int oldval;
		oldval = snd_hda_codec_read(codec, nid, 0,
					    AC_VERB_GET_PIN_WIDGET_CONTROL, 0);
		pincap = snd_hda_query_pin_caps(codec, nid);
		pincap = (pincap & AC_PINCAP_VREF) >> AC_PINCAP_VREF_SHIFT;
		/* if the default pin setup is vref50, we give it priority */
		if ((pincap & AC_PINCAP_VREF_80) && oldval != PIN_VREF50)
			val = PIN_VREF80;
		else if (pincap & AC_PINCAP_VREF_50)
			val = PIN_VREF50;
		else if (pincap & AC_PINCAP_VREF_100)
			val = PIN_VREF100;
		else if (pincap & AC_PINCAP_VREF_GRD)
			val = PIN_VREFGRD;
	}
	snd_hda_codec_write(codec, nid, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, val);
}

/*
 * Append the given mixer and verb elements for the later use
 * The mixer array is referred in build_controls(), and init_verbs are
 * called in init().
 */
static void add_mixer(struct alc_spec *spec, const struct snd_kcontrol_new *mix)
{
	if (snd_BUG_ON(spec->num_mixers >= ARRAY_SIZE(spec->mixers)))
		return;
	spec->mixers[spec->num_mixers++] = mix;
}

static void add_verb(struct alc_spec *spec, const struct hda_verb *verb)
{
	if (snd_BUG_ON(spec->num_init_verbs >= ARRAY_SIZE(spec->init_verbs)))
		return;
	spec->init_verbs[spec->num_init_verbs++] = verb;
=======
	struct input_dev *kb_dev;
	u8 alc_mute_keycode_map[1];

	/* component binding */
	struct hda_component comps[HDA_MAX_COMPONENTS];
};

/*
 * COEF access helper functions
 */

static void coef_mutex_lock(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;

	snd_hda_power_up_pm(codec);
	mutex_lock(&spec->coef_mutex);
}

static void coef_mutex_unlock(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;

	mutex_unlock(&spec->coef_mutex);
	snd_hda_power_down_pm(codec);
}

static int __alc_read_coefex_idx(struct hda_codec *codec, hda_nid_t nid,
				 unsigned int coef_idx)
{
	unsigned int val;

	snd_hda_codec_write(codec, nid, 0, AC_VERB_SET_COEF_INDEX, coef_idx);
	val = snd_hda_codec_read(codec, nid, 0, AC_VERB_GET_PROC_COEF, 0);
	return val;
}

static int alc_read_coefex_idx(struct hda_codec *codec, hda_nid_t nid,
			       unsigned int coef_idx)
{
	unsigned int val;

	coef_mutex_lock(codec);
	val = __alc_read_coefex_idx(codec, nid, coef_idx);
	coef_mutex_unlock(codec);
	return val;
}

#define alc_read_coef_idx(codec, coef_idx) \
	alc_read_coefex_idx(codec, 0x20, coef_idx)

static void __alc_write_coefex_idx(struct hda_codec *codec, hda_nid_t nid,
				   unsigned int coef_idx, unsigned int coef_val)
{
	snd_hda_codec_write(codec, nid, 0, AC_VERB_SET_COEF_INDEX, coef_idx);
	snd_hda_codec_write(codec, nid, 0, AC_VERB_SET_PROC_COEF, coef_val);
}

static void alc_write_coefex_idx(struct hda_codec *codec, hda_nid_t nid,
				 unsigned int coef_idx, unsigned int coef_val)
{
	coef_mutex_lock(codec);
	__alc_write_coefex_idx(codec, nid, coef_idx, coef_val);
	coef_mutex_unlock(codec);
}

#define alc_write_coef_idx(codec, coef_idx, coef_val) \
	alc_write_coefex_idx(codec, 0x20, coef_idx, coef_val)

static void __alc_update_coefex_idx(struct hda_codec *codec, hda_nid_t nid,
				    unsigned int coef_idx, unsigned int mask,
				    unsigned int bits_set)
{
	unsigned int val = __alc_read_coefex_idx(codec, nid, coef_idx);

	if (val != -1)
		__alc_write_coefex_idx(codec, nid, coef_idx,
				       (val & ~mask) | bits_set);
}

static void alc_update_coefex_idx(struct hda_codec *codec, hda_nid_t nid,
				  unsigned int coef_idx, unsigned int mask,
				  unsigned int bits_set)
{
	coef_mutex_lock(codec);
	__alc_update_coefex_idx(codec, nid, coef_idx, mask, bits_set);
	coef_mutex_unlock(codec);
}

#define alc_update_coef_idx(codec, coef_idx, mask, bits_set)	\
	alc_update_coefex_idx(codec, 0x20, coef_idx, mask, bits_set)

/* a special bypass for COEF 0; read the cached value at the second time */
static unsigned int alc_get_coef0(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;

	if (!spec->coef0)
		spec->coef0 = alc_read_coef_idx(codec, 0);
	return spec->coef0;
}

/* coef writes/updates batch */
struct coef_fw {
	unsigned char nid;
	unsigned char idx;
	unsigned short mask;
	unsigned short val;
};

#define UPDATE_COEFEX(_nid, _idx, _mask, _val) \
	{ .nid = (_nid), .idx = (_idx), .mask = (_mask), .val = (_val) }
#define WRITE_COEFEX(_nid, _idx, _val) UPDATE_COEFEX(_nid, _idx, -1, _val)
#define WRITE_COEF(_idx, _val) WRITE_COEFEX(0x20, _idx, _val)
#define UPDATE_COEF(_idx, _mask, _val) UPDATE_COEFEX(0x20, _idx, _mask, _val)

static void alc_process_coef_fw(struct hda_codec *codec,
				const struct coef_fw *fw)
{
	coef_mutex_lock(codec);
	for (; fw->nid; fw++) {
		if (fw->mask == (unsigned short)-1)
			__alc_write_coefex_idx(codec, fw->nid, fw->idx, fw->val);
		else
			__alc_update_coefex_idx(codec, fw->nid, fw->idx,
						fw->mask, fw->val);
	}
	coef_mutex_unlock(codec);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * GPIO setup tables, used in initialization
 */
<<<<<<< HEAD
/* Enable GPIO mask and set output */
static const struct hda_verb alc_gpio1_init_verbs[] = {
	{0x01, AC_VERB_SET_GPIO_MASK, 0x01},
	{0x01, AC_VERB_SET_GPIO_DIRECTION, 0x01},
	{0x01, AC_VERB_SET_GPIO_DATA, 0x01},
	{ }
};

static const struct hda_verb alc_gpio2_init_verbs[] = {
	{0x01, AC_VERB_SET_GPIO_MASK, 0x02},
	{0x01, AC_VERB_SET_GPIO_DIRECTION, 0x02},
	{0x01, AC_VERB_SET_GPIO_DATA, 0x02},
	{ }
};

static const struct hda_verb alc_gpio3_init_verbs[] = {
	{0x01, AC_VERB_SET_GPIO_MASK, 0x03},
	{0x01, AC_VERB_SET_GPIO_DIRECTION, 0x03},
	{0x01, AC_VERB_SET_GPIO_DATA, 0x03},
	{ }
};
=======

/* Enable GPIO mask and set output */
static void alc_setup_gpio(struct hda_codec *codec, unsigned int mask)
{
	struct alc_spec *spec = codec->spec;

	spec->gpio_mask |= mask;
	spec->gpio_dir |= mask;
	spec->gpio_data |= mask;
}

static void alc_write_gpio_data(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;

	snd_hda_codec_write(codec, 0x01, 0, AC_VERB_SET_GPIO_DATA,
			    spec->gpio_data);
}

static void alc_update_gpio_data(struct hda_codec *codec, unsigned int mask,
				 bool on)
{
	struct alc_spec *spec = codec->spec;
	unsigned int oldval = spec->gpio_data;

	if (on)
		spec->gpio_data |= mask;
	else
		spec->gpio_data &= ~mask;
	if (oldval != spec->gpio_data)
		alc_write_gpio_data(codec);
}

static void alc_write_gpio(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;

	if (!spec->gpio_mask)
		return;

	snd_hda_codec_write(codec, codec->core.afg, 0,
			    AC_VERB_SET_GPIO_MASK, spec->gpio_mask);
	snd_hda_codec_write(codec, codec->core.afg, 0,
			    AC_VERB_SET_GPIO_DIRECTION, spec->gpio_dir);
	if (spec->gpio_write_delay)
		msleep(1);
	alc_write_gpio_data(codec);
}

static void alc_fixup_gpio(struct hda_codec *codec, int action,
			   unsigned int mask)
{
	if (action == HDA_FIXUP_ACT_PRE_PROBE)
		alc_setup_gpio(codec, mask);
}

static void alc_fixup_gpio1(struct hda_codec *codec,
			    const struct hda_fixup *fix, int action)
{
	alc_fixup_gpio(codec, action, 0x01);
}

static void alc_fixup_gpio2(struct hda_codec *codec,
			    const struct hda_fixup *fix, int action)
{
	alc_fixup_gpio(codec, action, 0x02);
}

static void alc_fixup_gpio3(struct hda_codec *codec,
			    const struct hda_fixup *fix, int action)
{
	alc_fixup_gpio(codec, action, 0x03);
}

static void alc_fixup_gpio4(struct hda_codec *codec,
			    const struct hda_fixup *fix, int action)
{
	alc_fixup_gpio(codec, action, 0x04);
}

static void alc_fixup_micmute_led(struct hda_codec *codec,
				  const struct hda_fixup *fix, int action)
{
	if (action == HDA_FIXUP_ACT_PRE_PROBE)
		snd_hda_gen_add_micmute_led_cdev(codec, NULL);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Fix hardware PLL issue
 * On some codecs, the analog PLL gating control must be off while
 * the default value is 1.
 */
static void alc_fix_pll(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
<<<<<<< HEAD
	unsigned int val;

	if (!spec->pll_nid)
		return;
	snd_hda_codec_write(codec, spec->pll_nid, 0, AC_VERB_SET_COEF_INDEX,
			    spec->pll_coef_idx);
	val = snd_hda_codec_read(codec, spec->pll_nid, 0,
				 AC_VERB_GET_PROC_COEF, 0);
	if (val == -1)
		return;
	snd_hda_codec_write(codec, spec->pll_nid, 0, AC_VERB_SET_COEF_INDEX,
			    spec->pll_coef_idx);
	snd_hda_codec_write(codec, spec->pll_nid, 0, AC_VERB_SET_PROC_COEF,
			    val & ~(1 << spec->pll_coef_bit));
=======

	if (spec->pll_nid)
		alc_update_coefex_idx(codec, spec->pll_nid, spec->pll_coef_idx,
				      1 << spec->pll_coef_bit, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void alc_fix_pll_init(struct hda_codec *codec, hda_nid_t nid,
			     unsigned int coef_idx, unsigned int coef_bit)
{
	struct alc_spec *spec = codec->spec;
	spec->pll_nid = nid;
	spec->pll_coef_idx = coef_idx;
	spec->pll_coef_bit = coef_bit;
	alc_fix_pll(codec);
}

<<<<<<< HEAD
/*
 * Jack detections for HP auto-mute and mic-switch
 */

/* check each pin in the given array; returns true if any of them is plugged */
static bool detect_jacks(struct hda_codec *codec, int num_pins, hda_nid_t *pins)
{
	int i, present = 0;

	for (i = 0; i < num_pins; i++) {
		hda_nid_t nid = pins[i];
		if (!nid)
			break;
		present |= snd_hda_jack_detect(codec, nid);
	}
	return present;
}

/* standard HP/line-out auto-mute helper */
static void do_automute(struct hda_codec *codec, int num_pins, hda_nid_t *pins,
			bool mute, bool hp_out)
{
	struct alc_spec *spec = codec->spec;
	unsigned int mute_bits = mute ? HDA_AMP_MUTE : 0;
	unsigned int pin_bits = mute ? 0 : (hp_out ? PIN_HP : PIN_OUT);
	int i;

	for (i = 0; i < num_pins; i++) {
		hda_nid_t nid = pins[i];
		unsigned int val;
		if (!nid)
			break;
		switch (spec->automute_mode) {
		case ALC_AUTOMUTE_PIN:
			/* don't reset VREF value in case it's controlling
			 * the amp (see alc861_fixup_asus_amp_vref_0f())
			 */
			if (spec->keep_vref_in_automute) {
				val = snd_hda_codec_read(codec, nid, 0,
					AC_VERB_GET_PIN_WIDGET_CONTROL, 0);
				val &= ~PIN_HP;
			} else
				val = 0;
			val |= pin_bits;
			snd_hda_codec_write(codec, nid, 0,
					    AC_VERB_SET_PIN_WIDGET_CONTROL,
					    val);
			break;
		case ALC_AUTOMUTE_AMP:
			snd_hda_codec_amp_stereo(codec, nid, HDA_OUTPUT, 0,
						 HDA_AMP_MUTE, mute_bits);
			break;
		case ALC_AUTOMUTE_MIXER:
			nid = spec->automute_mixer_nid[i];
			if (!nid)
				break;
			snd_hda_codec_amp_stereo(codec, nid, HDA_INPUT, 0,
						 HDA_AMP_MUTE, mute_bits);
			snd_hda_codec_amp_stereo(codec, nid, HDA_INPUT, 1,
						 HDA_AMP_MUTE, mute_bits);
			break;
		}
	}
}

/* Toggle outputs muting */
static void update_outputs(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	int on;

	/* Control HP pins/amps depending on master_mute state;
	 * in general, HP pins/amps control should be enabled in all cases,
	 * but currently set only for master_mute, just to be safe
	 */
	if (!spec->shared_mic_hp) /* don't change HP-pin when shared with mic */
		do_automute(codec, ARRAY_SIZE(spec->autocfg.hp_pins),
		    spec->autocfg.hp_pins, spec->master_mute, true);

	if (!spec->automute_speaker)
		on = 0;
	else
		on = spec->hp_jack_present | spec->line_jack_present;
	on |= spec->master_mute;
	do_automute(codec, ARRAY_SIZE(spec->autocfg.speaker_pins),
		    spec->autocfg.speaker_pins, on, false);

	/* toggle line-out mutes if needed, too */
	/* if LO is a copy of either HP or Speaker, don't need to handle it */
	if (spec->autocfg.line_out_pins[0] == spec->autocfg.hp_pins[0] ||
	    spec->autocfg.line_out_pins[0] == spec->autocfg.speaker_pins[0])
		return;
	if (!spec->automute_lo)
		on = 0;
	else
		on = spec->hp_jack_present;
	on |= spec->master_mute;
	do_automute(codec, ARRAY_SIZE(spec->autocfg.line_out_pins),
		    spec->autocfg.line_out_pins, on, false);
}

static void call_update_outputs(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	if (spec->automute_hook)
		spec->automute_hook(codec);
	else
		update_outputs(codec);
}

/* standard HP-automute helper */
static void alc_hp_automute(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;

	spec->hp_jack_present =
		detect_jacks(codec, ARRAY_SIZE(spec->autocfg.hp_pins),
			     spec->autocfg.hp_pins);
	if (!spec->detect_hp || (!spec->automute_speaker && !spec->automute_lo))
		return;
	call_update_outputs(codec);
}

/* standard line-out-automute helper */
static void alc_line_automute(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;

	if (spec->autocfg.line_out_type == AUTO_PIN_SPEAKER_OUT)
		return;
	/* check LO jack only when it's different from HP */
	if (spec->autocfg.line_out_pins[0] == spec->autocfg.hp_pins[0])
		return;

	spec->line_jack_present =
		detect_jacks(codec, ARRAY_SIZE(spec->autocfg.line_out_pins),
			     spec->autocfg.line_out_pins);
	if (!spec->automute_speaker || !spec->detect_lo)
		return;
	call_update_outputs(codec);
}

#define get_connection_index(codec, mux, nid) \
	snd_hda_get_conn_index(codec, mux, nid, 0)

/* standard mic auto-switch helper */
static void alc_mic_automute(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	hda_nid_t *pins = spec->imux_pins;

	if (!spec->auto_mic || !spec->auto_mic_valid_imux)
		return;
	if (snd_BUG_ON(!spec->adc_nids))
		return;
	if (snd_BUG_ON(spec->int_mic_idx < 0 || spec->ext_mic_idx < 0))
		return;

	if (snd_hda_jack_detect(codec, pins[spec->ext_mic_idx]))
		alc_mux_select(codec, 0, spec->ext_mic_idx, false);
	else if (spec->dock_mic_idx >= 0 &&
		   snd_hda_jack_detect(codec, pins[spec->dock_mic_idx]))
		alc_mux_select(codec, 0, spec->dock_mic_idx, false);
	else
		alc_mux_select(codec, 0, spec->int_mic_idx, false);
}

/* handle the specified unsol action (ALC_XXX_EVENT) */
static void alc_exec_unsol_event(struct hda_codec *codec, int action)
{
	switch (action) {
	case ALC_HP_EVENT:
		alc_hp_automute(codec);
		break;
	case ALC_FRONT_EVENT:
		alc_line_automute(codec);
		break;
	case ALC_MIC_EVENT:
		alc_mic_automute(codec);
		break;
	}
	snd_hda_jack_report_sync(codec);
}

/* update the master volume per volume-knob's unsol event */
static void alc_update_knob_master(struct hda_codec *codec, hda_nid_t nid)
=======
/* update the master volume per volume-knob's unsol event */
static void alc_update_knob_master(struct hda_codec *codec,
				   struct hda_jack_callback *jack)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned int val;
	struct snd_kcontrol *kctl;
	struct snd_ctl_elem_value *uctl;

	kctl = snd_hda_find_mixer_ctl(codec, "Master Playback Volume");
	if (!kctl)
		return;
	uctl = kzalloc(sizeof(*uctl), GFP_KERNEL);
	if (!uctl)
		return;
<<<<<<< HEAD
	val = snd_hda_codec_read(codec, nid, 0,
=======
	val = snd_hda_codec_read(codec, jack->nid, 0,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				 AC_VERB_GET_VOLUME_KNOB_CONTROL, 0);
	val &= HDA_AMP_VOLMASK;
	uctl->value.integer.value[0] = val;
	uctl->value.integer.value[1] = val;
	kctl->put(kctl, uctl);
	kfree(uctl);
}

<<<<<<< HEAD
/* unsolicited event for HP jack sensing */
static void alc_sku_unsol_event(struct hda_codec *codec, unsigned int res)
{
	int action;

	if (codec->vendor_id == 0x10ec0880)
		res >>= 28;
	else
		res >>= 26;
	action = snd_hda_jack_get_action(codec, res);
	if (action == ALC_DCVOL_EVENT) {
		/* Execute the dc-vol event here as it requires the NID
		 * but we don't pass NID to alc_exec_unsol_event().
		 * Once when we convert all static quirks to the auto-parser,
		 * this can be integerated into there.
		 */
		struct hda_jack_tbl *jack;
		jack = snd_hda_jack_tbl_get_from_tag(codec, res);
		if (jack)
			alc_update_knob_master(codec, jack->nid);
		return;
	}
	alc_exec_unsol_event(codec, action);
}

/* call init functions of standard auto-mute helpers */
static void alc_inithook(struct hda_codec *codec)
{
	alc_hp_automute(codec);
	alc_line_automute(codec);
	alc_mic_automute(codec);
=======
static void alc880_unsol_event(struct hda_codec *codec, unsigned int res)
{
	/* For some reason, the res given from ALC880 is broken.
	   Here we adjust it properly. */
	snd_hda_jack_unsol_event(codec, res >> 2);
}

/* Change EAPD to verb control */
static void alc_fill_eapd_coef(struct hda_codec *codec)
{
	int coef;

	coef = alc_get_coef0(codec);

	switch (codec->core.vendor_id) {
	case 0x10ec0262:
		alc_update_coef_idx(codec, 0x7, 0, 1<<5);
		break;
	case 0x10ec0267:
	case 0x10ec0268:
		alc_update_coef_idx(codec, 0x7, 0, 1<<13);
		break;
	case 0x10ec0269:
		if ((coef & 0x00f0) == 0x0010)
			alc_update_coef_idx(codec, 0xd, 0, 1<<14);
		if ((coef & 0x00f0) == 0x0020)
			alc_update_coef_idx(codec, 0x4, 1<<15, 0);
		if ((coef & 0x00f0) == 0x0030)
			alc_update_coef_idx(codec, 0x10, 1<<9, 0);
		break;
	case 0x10ec0280:
	case 0x10ec0284:
	case 0x10ec0290:
	case 0x10ec0292:
		alc_update_coef_idx(codec, 0x4, 1<<15, 0);
		break;
	case 0x10ec0225:
	case 0x10ec0295:
	case 0x10ec0299:
		alc_update_coef_idx(codec, 0x67, 0xf000, 0x3000);
		fallthrough;
	case 0x10ec0215:
	case 0x10ec0285:
	case 0x10ec0289:
		alc_update_coef_idx(codec, 0x36, 1<<13, 0);
		fallthrough;
	case 0x10ec0230:
	case 0x10ec0233:
	case 0x10ec0235:
	case 0x10ec0236:
	case 0x10ec0245:
	case 0x10ec0255:
	case 0x10ec0256:
	case 0x19e58326:
	case 0x10ec0257:
	case 0x10ec0282:
	case 0x10ec0283:
	case 0x10ec0286:
	case 0x10ec0288:
	case 0x10ec0298:
	case 0x10ec0300:
		alc_update_coef_idx(codec, 0x10, 1<<9, 0);
		break;
	case 0x10ec0275:
		alc_update_coef_idx(codec, 0xe, 0, 1<<0);
		break;
	case 0x10ec0287:
		alc_update_coef_idx(codec, 0x10, 1<<9, 0);
		alc_write_coef_idx(codec, 0x8, 0x4ab7);
		break;
	case 0x10ec0293:
		alc_update_coef_idx(codec, 0xa, 1<<13, 0);
		break;
	case 0x10ec0234:
	case 0x10ec0274:
	case 0x10ec0294:
	case 0x10ec0700:
	case 0x10ec0701:
	case 0x10ec0703:
	case 0x10ec0711:
		alc_update_coef_idx(codec, 0x10, 1<<15, 0);
		break;
	case 0x10ec0662:
		if ((coef & 0x00f0) == 0x0030)
			alc_update_coef_idx(codec, 0x4, 1<<10, 0); /* EAPD Ctrl */
		break;
	case 0x10ec0272:
	case 0x10ec0273:
	case 0x10ec0663:
	case 0x10ec0665:
	case 0x10ec0670:
	case 0x10ec0671:
	case 0x10ec0672:
		alc_update_coef_idx(codec, 0xd, 0, 1<<14); /* EAPD Ctrl */
		break;
	case 0x10ec0222:
	case 0x10ec0623:
		alc_update_coef_idx(codec, 0x19, 1<<13, 0);
		break;
	case 0x10ec0668:
		alc_update_coef_idx(codec, 0x7, 3<<13, 0);
		break;
	case 0x10ec0867:
		alc_update_coef_idx(codec, 0x4, 1<<10, 0);
		break;
	case 0x10ec0888:
		if ((coef & 0x00f0) == 0x0020 || (coef & 0x00f0) == 0x0030)
			alc_update_coef_idx(codec, 0x7, 1<<5, 0);
		break;
	case 0x10ec0892:
	case 0x10ec0897:
		alc_update_coef_idx(codec, 0x7, 1<<5, 0);
		break;
	case 0x10ec0899:
	case 0x10ec0900:
	case 0x10ec0b00:
	case 0x10ec1168:
	case 0x10ec1220:
		alc_update_coef_idx(codec, 0x7, 1<<1, 0);
		break;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* additional initialization for ALC888 variants */
static void alc888_coef_init(struct hda_codec *codec)
{
<<<<<<< HEAD
	unsigned int tmp;

	snd_hda_codec_write(codec, 0x20, 0, AC_VERB_SET_COEF_INDEX, 0);
	tmp = snd_hda_codec_read(codec, 0x20, 0, AC_VERB_GET_PROC_COEF, 0);
	snd_hda_codec_write(codec, 0x20, 0, AC_VERB_SET_COEF_INDEX, 7);
	if ((tmp & 0xf0) == 0x20)
		/* alc888S-VC */
		snd_hda_codec_read(codec, 0x20, 0,
				   AC_VERB_SET_PROC_COEF, 0x830);
	 else
		 /* alc888-VB */
		 snd_hda_codec_read(codec, 0x20, 0,
				    AC_VERB_SET_PROC_COEF, 0x3030);
}

/* additional initialization for ALC889 variants */
static void alc889_coef_init(struct hda_codec *codec)
{
	unsigned int tmp;

	snd_hda_codec_write(codec, 0x20, 0, AC_VERB_SET_COEF_INDEX, 7);
	tmp = snd_hda_codec_read(codec, 0x20, 0, AC_VERB_GET_PROC_COEF, 0);
	snd_hda_codec_write(codec, 0x20, 0, AC_VERB_SET_COEF_INDEX, 7);
	snd_hda_codec_write(codec, 0x20, 0, AC_VERB_SET_PROC_COEF, tmp|0x2010);
=======
	switch (alc_get_coef0(codec) & 0x00f0) {
	/* alc888-VA */
	case 0x00:
	/* alc888-VB */
	case 0x10:
		alc_update_coef_idx(codec, 7, 0, 0x2030); /* Turn EAPD to High */
		break;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* turn on/off EAPD control (only if available) */
static void set_eapd(struct hda_codec *codec, hda_nid_t nid, int on)
{
	if (get_wcaps_type(get_wcaps(codec, nid)) != AC_WID_PIN)
		return;
	if (snd_hda_query_pin_caps(codec, nid) & AC_PINCAP_EAPD)
		snd_hda_codec_write(codec, nid, 0, AC_VERB_SET_EAPD_BTLENABLE,
				    on ? 2 : 0);
}

/* turn on/off EAPD controls of the codec */
static void alc_auto_setup_eapd(struct hda_codec *codec, bool on)
{
	/* We currently only handle front, HP */
<<<<<<< HEAD
	static hda_nid_t pins[] = {
		0x0f, 0x10, 0x14, 0x15, 0x17, 0
	};
	hda_nid_t *p;
=======
	static const hda_nid_t pins[] = {
		0x0f, 0x10, 0x14, 0x15, 0x17, 0
	};
	const hda_nid_t *p;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	for (p = pins; *p; p++)
		set_eapd(codec, *p, on);
}

<<<<<<< HEAD
/* generic shutup callback;
 * just turning off EPAD and a little pause for avoiding pop-noise
 */
static void alc_eapd_shutup(struct hda_codec *codec)
{
	alc_auto_setup_eapd(codec, false);
	msleep(200);
=======
static int find_ext_mic_pin(struct hda_codec *codec);

static void alc_headset_mic_no_shutup(struct hda_codec *codec)
{
	const struct hda_pincfg *pin;
	int mic_pin = find_ext_mic_pin(codec);
	int i;

	/* don't shut up pins when unloading the driver; otherwise it breaks
	 * the default pin setup at the next load of the driver
	 */
	if (codec->bus->shutdown)
		return;

	snd_array_for_each(&codec->init_pins, i, pin) {
		/* use read here for syncing after issuing each verb */
		if (pin->nid != mic_pin)
			snd_hda_codec_read(codec, pin->nid, 0,
					AC_VERB_SET_PIN_WIDGET_CONTROL, 0);
	}

	codec->pins_shutup = 1;
}

static void alc_shutup_pins(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;

	switch (codec->core.vendor_id) {
	case 0x10ec0236:
	case 0x10ec0256:
	case 0x19e58326:
	case 0x10ec0283:
	case 0x10ec0286:
	case 0x10ec0288:
	case 0x10ec0298:
		alc_headset_mic_no_shutup(codec);
		break;
	default:
		if (!spec->no_shutup_pins)
			snd_hda_shutup_pins(codec);
		break;
	}
}

/* generic shutup callback;
 * just turning off EAPD and a little pause for avoiding pop-noise
 */
static void alc_eapd_shutup(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;

	alc_auto_setup_eapd(codec, false);
	if (!spec->no_depop_delay)
		msleep(200);
	alc_shutup_pins(codec);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* generic EAPD initialization */
static void alc_auto_init_amp(struct hda_codec *codec, int type)
{
<<<<<<< HEAD
	unsigned int tmp;

	alc_auto_setup_eapd(codec, true);
	switch (type) {
	case ALC_INIT_GPIO1:
		snd_hda_sequence_write(codec, alc_gpio1_init_verbs);
		break;
	case ALC_INIT_GPIO2:
		snd_hda_sequence_write(codec, alc_gpio2_init_verbs);
		break;
	case ALC_INIT_GPIO3:
		snd_hda_sequence_write(codec, alc_gpio3_init_verbs);
		break;
	case ALC_INIT_DEFAULT:
		switch (codec->vendor_id) {
		case 0x10ec0260:
			snd_hda_codec_write(codec, 0x1a, 0,
					    AC_VERB_SET_COEF_INDEX, 7);
			tmp = snd_hda_codec_read(codec, 0x1a, 0,
						 AC_VERB_GET_PROC_COEF, 0);
			snd_hda_codec_write(codec, 0x1a, 0,
					    AC_VERB_SET_COEF_INDEX, 7);
			snd_hda_codec_write(codec, 0x1a, 0,
					    AC_VERB_SET_PROC_COEF,
					    tmp | 0x2010);
			break;
		case 0x10ec0262:
=======
	alc_auto_setup_eapd(codec, true);
	alc_write_gpio(codec);
	switch (type) {
	case ALC_INIT_DEFAULT:
		switch (codec->core.vendor_id) {
		case 0x10ec0260:
			alc_update_coefex_idx(codec, 0x1a, 7, 0, 0x2010);
			break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		case 0x10ec0880:
		case 0x10ec0882:
		case 0x10ec0883:
		case 0x10ec0885:
<<<<<<< HEAD
		case 0x10ec0887:
		/*case 0x10ec0889:*/ /* this causes an SPDIF problem */
		case 0x10ec0900:
			alc889_coef_init(codec);
=======
			alc_update_coef_idx(codec, 7, 0, 0x2030);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		case 0x10ec0888:
			alc888_coef_init(codec);
			break;
<<<<<<< HEAD
#if 0 /* XXX: This may cause the silent output on speaker on some machines */
		case 0x10ec0267:
		case 0x10ec0268:
			snd_hda_codec_write(codec, 0x20, 0,
					    AC_VERB_SET_COEF_INDEX, 7);
			tmp = snd_hda_codec_read(codec, 0x20, 0,
						 AC_VERB_GET_PROC_COEF, 0);
			snd_hda_codec_write(codec, 0x20, 0,
					    AC_VERB_SET_COEF_INDEX, 7);
			snd_hda_codec_write(codec, 0x20, 0,
					    AC_VERB_SET_PROC_COEF,
					    tmp | 0x3000);
			break;
#endif /* XXX */
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		break;
	}
}

<<<<<<< HEAD
/*
 * Auto-Mute mode mixer enum support
 */
static int alc_automute_mode_info(struct snd_kcontrol *kcontrol,
				  struct snd_ctl_elem_info *uinfo)
{
	struct hda_codec *codec = snd_kcontrol_chip(kcontrol);
	struct alc_spec *spec = codec->spec;
	static const char * const texts2[] = {
		"Disabled", "Enabled"
	};
	static const char * const texts3[] = {
		"Disabled", "Speaker Only", "Line Out+Speaker"
	};
	const char * const *texts;

	uinfo->type = SNDRV_CTL_ELEM_TYPE_ENUMERATED;
	uinfo->count = 1;
	if (spec->automute_speaker_possible && spec->automute_lo_possible) {
		uinfo->value.enumerated.items = 3;
		texts = texts3;
	} else {
		uinfo->value.enumerated.items = 2;
		texts = texts2;
	}
	if (uinfo->value.enumerated.item >= uinfo->value.enumerated.items)
		uinfo->value.enumerated.item = uinfo->value.enumerated.items - 1;
	strcpy(uinfo->value.enumerated.name,
	       texts[uinfo->value.enumerated.item]);
	return 0;
}

static int alc_automute_mode_get(struct snd_kcontrol *kcontrol,
				 struct snd_ctl_elem_value *ucontrol)
{
	struct hda_codec *codec = snd_kcontrol_chip(kcontrol);
	struct alc_spec *spec = codec->spec;
	unsigned int val = 0;
	if (spec->automute_speaker)
		val++;
	if (spec->automute_lo)
		val++;

	ucontrol->value.enumerated.item[0] = val;
	return 0;
}

static int alc_automute_mode_put(struct snd_kcontrol *kcontrol,
				 struct snd_ctl_elem_value *ucontrol)
{
	struct hda_codec *codec = snd_kcontrol_chip(kcontrol);
	struct alc_spec *spec = codec->spec;

	switch (ucontrol->value.enumerated.item[0]) {
	case 0:
		if (!spec->automute_speaker && !spec->automute_lo)
			return 0;
		spec->automute_speaker = 0;
		spec->automute_lo = 0;
		break;
	case 1:
		if (spec->automute_speaker_possible) {
			if (!spec->automute_lo && spec->automute_speaker)
				return 0;
			spec->automute_speaker = 1;
			spec->automute_lo = 0;
		} else if (spec->automute_lo_possible) {
			if (spec->automute_lo)
				return 0;
			spec->automute_lo = 1;
		} else
			return -EINVAL;
		break;
	case 2:
		if (!spec->automute_lo_possible || !spec->automute_speaker_possible)
			return -EINVAL;
		if (spec->automute_speaker && spec->automute_lo)
			return 0;
		spec->automute_speaker = 1;
		spec->automute_lo = 1;
		break;
	default:
		return -EINVAL;
	}
	call_update_outputs(codec);
	return 1;
}

static const struct snd_kcontrol_new alc_automute_mode_enum = {
	.iface = SNDRV_CTL_ELEM_IFACE_MIXER,
	.name = "Auto-Mute Mode",
	.info = alc_automute_mode_info,
	.get = alc_automute_mode_get,
	.put = alc_automute_mode_put,
};

static struct snd_kcontrol_new *alc_kcontrol_new(struct alc_spec *spec)
{
	snd_array_init(&spec->kctls, sizeof(struct snd_kcontrol_new), 32);
	return snd_array_new(&spec->kctls);
}

static int alc_add_automute_mode_enum(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	struct snd_kcontrol_new *knew;

	knew = alc_kcontrol_new(spec);
	if (!knew)
		return -ENOMEM;
	*knew = alc_automute_mode_enum;
	knew->name = kstrdup("Auto-Mute Mode", GFP_KERNEL);
	if (!knew->name)
		return -ENOMEM;
	return 0;
}

/*
 * Check the availability of HP/line-out auto-mute;
 * Set up appropriately if really supported
 */
static void alc_init_automute(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	struct auto_pin_cfg *cfg = &spec->autocfg;
	int present = 0;
	int i;

	if (cfg->hp_pins[0])
		present++;
	if (cfg->line_out_pins[0])
		present++;
	if (cfg->speaker_pins[0])
		present++;
	if (present < 2) /* need two different output types */
		return;

	if (!cfg->speaker_pins[0] &&
	    cfg->line_out_type == AUTO_PIN_SPEAKER_OUT) {
		memcpy(cfg->speaker_pins, cfg->line_out_pins,
		       sizeof(cfg->speaker_pins));
		cfg->speaker_outs = cfg->line_outs;
	}

	if (!cfg->hp_pins[0] &&
	    cfg->line_out_type == AUTO_PIN_HP_OUT) {
		memcpy(cfg->hp_pins, cfg->line_out_pins,
		       sizeof(cfg->hp_pins));
		cfg->hp_outs = cfg->line_outs;
	}

	spec->automute_mode = ALC_AUTOMUTE_PIN;

	for (i = 0; i < cfg->hp_outs; i++) {
		hda_nid_t nid = cfg->hp_pins[i];
		if (!is_jack_detectable(codec, nid))
			continue;
		snd_printdd("realtek: Enable HP auto-muting on NID 0x%x\n",
			    nid);
		snd_hda_jack_detect_enable(codec, nid, ALC_HP_EVENT);
		spec->detect_hp = 1;
	}

	if (cfg->line_out_type == AUTO_PIN_LINE_OUT && cfg->line_outs) {
		if (cfg->speaker_outs)
			for (i = 0; i < cfg->line_outs; i++) {
				hda_nid_t nid = cfg->line_out_pins[i];
				if (!is_jack_detectable(codec, nid))
					continue;
				snd_printdd("realtek: Enable Line-Out "
					    "auto-muting on NID 0x%x\n", nid);
				snd_hda_jack_detect_enable(codec, nid,
							   ALC_FRONT_EVENT);
				spec->detect_lo = 1;
		}
		spec->automute_lo_possible = spec->detect_hp;
	}

	spec->automute_speaker_possible = cfg->speaker_outs &&
		(spec->detect_hp || spec->detect_lo);

	spec->automute_lo = spec->automute_lo_possible;
	spec->automute_speaker = spec->automute_speaker_possible;

	if (spec->automute_speaker_possible || spec->automute_lo_possible) {
		/* create a control for automute mode */
		alc_add_automute_mode_enum(codec);
		spec->unsol_event = alc_sku_unsol_event;
	}
}

/* return the position of NID in the list, or -1 if not found */
static int find_idx_in_nid_list(hda_nid_t nid, const hda_nid_t *list, int nums)
{
	int i;
	for (i = 0; i < nums; i++)
		if (list[i] == nid)
			return i;
	return -1;
}

/* check whether dynamic ADC-switching is available */
static bool alc_check_dyn_adc_switch(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	struct hda_input_mux *imux = &spec->private_imux[0];
	int i, n, idx;
	hda_nid_t cap, pin;

	if (imux != spec->input_mux) /* no dynamic imux? */
		return false;

	for (n = 0; n < spec->num_adc_nids; n++) {
		cap = spec->private_capsrc_nids[n];
		for (i = 0; i < imux->num_items; i++) {
			pin = spec->imux_pins[i];
			if (!pin)
				return false;
			if (get_connection_index(codec, cap, pin) < 0)
				break;
		}
		if (i >= imux->num_items)
			return true; /* no ADC-switch is needed */
	}

	for (i = 0; i < imux->num_items; i++) {
		pin = spec->imux_pins[i];
		for (n = 0; n < spec->num_adc_nids; n++) {
			cap = spec->private_capsrc_nids[n];
			idx = get_connection_index(codec, cap, pin);
			if (idx >= 0) {
				imux->items[i].index = idx;
				spec->dyn_adc_idx[i] = n;
				break;
			}
		}
	}

	snd_printdd("realtek: enabling ADC switching\n");
	spec->dyn_adc_switch = 1;
	return true;
}

/* check whether all auto-mic pins are valid; setup indices if OK */
static bool alc_auto_mic_check_imux(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	const struct hda_input_mux *imux;

	if (!spec->auto_mic)
		return false;
	if (spec->auto_mic_valid_imux)
		return true; /* already checked */

	/* fill up imux indices */
	if (!alc_check_dyn_adc_switch(codec)) {
		spec->auto_mic = 0;
		return false;
	}

	imux = spec->input_mux;
	spec->ext_mic_idx = find_idx_in_nid_list(spec->ext_mic_pin,
					spec->imux_pins, imux->num_items);
	spec->int_mic_idx = find_idx_in_nid_list(spec->int_mic_pin,
					spec->imux_pins, imux->num_items);
	spec->dock_mic_idx = find_idx_in_nid_list(spec->dock_mic_pin,
					spec->imux_pins, imux->num_items);
	if (spec->ext_mic_idx < 0 || spec->int_mic_idx < 0) {
		spec->auto_mic = 0;
		return false; /* no corresponding imux */
	}

	snd_hda_jack_detect_enable(codec, spec->ext_mic_pin, ALC_MIC_EVENT);
	if (spec->dock_mic_pin)
		snd_hda_jack_detect_enable(codec, spec->dock_mic_pin,
					   ALC_MIC_EVENT);

	spec->auto_mic_valid_imux = 1;
	spec->auto_mic = 1;
	return true;
}

/*
 * Check the availability of auto-mic switch;
 * Set up if really supported
 */
static void alc_init_auto_mic(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	struct auto_pin_cfg *cfg = &spec->autocfg;
	hda_nid_t fixed, ext, dock;
	int i;

	if (spec->shared_mic_hp)
		return; /* no auto-mic for the shared I/O */

	spec->ext_mic_idx = spec->int_mic_idx = spec->dock_mic_idx = -1;

	fixed = ext = dock = 0;
	for (i = 0; i < cfg->num_inputs; i++) {
		hda_nid_t nid = cfg->inputs[i].pin;
		unsigned int defcfg;
		defcfg = snd_hda_codec_get_pincfg(codec, nid);
		switch (snd_hda_get_input_pin_attr(defcfg)) {
		case INPUT_PIN_ATTR_INT:
			if (fixed)
				return; /* already occupied */
			if (cfg->inputs[i].type != AUTO_PIN_MIC)
				return; /* invalid type */
			fixed = nid;
			break;
		case INPUT_PIN_ATTR_UNUSED:
			return; /* invalid entry */
		case INPUT_PIN_ATTR_DOCK:
			if (dock)
				return; /* already occupied */
			if (cfg->inputs[i].type > AUTO_PIN_LINE_IN)
				return; /* invalid type */
			dock = nid;
			break;
		default:
			if (ext)
				return; /* already occupied */
			if (cfg->inputs[i].type != AUTO_PIN_MIC)
				return; /* invalid type */
			ext = nid;
			break;
		}
	}
	if (!ext && dock) {
		ext = dock;
		dock = 0;
	}
	if (!ext || !fixed)
		return;
	if (!is_jack_detectable(codec, ext))
		return; /* no unsol support */
	if (dock && !is_jack_detectable(codec, dock))
		return; /* no unsol support */

	/* check imux indices */
	spec->ext_mic_pin = ext;
	spec->int_mic_pin = fixed;
	spec->dock_mic_pin = dock;

	spec->auto_mic = 1;
	if (!alc_auto_mic_check_imux(codec))
		return;

	snd_printdd("realtek: Enable auto-mic switch on NID 0x%x/0x%x/0x%x\n",
		    ext, fixed, dock);
	spec->unsol_event = alc_sku_unsol_event;
}

/* check the availabilities of auto-mute and auto-mic switches */
static void alc_auto_check_switches(struct hda_codec *codec)
{
	alc_init_automute(codec);
	alc_init_auto_mic(codec);
}

=======
/* get a primary headphone pin if available */
static hda_nid_t alc_get_hp_pin(struct alc_spec *spec)
{
	if (spec->gen.autocfg.hp_pins[0])
		return spec->gen.autocfg.hp_pins[0];
	if (spec->gen.autocfg.line_out_type == AC_JACK_HP_OUT)
		return spec->gen.autocfg.line_out_pins[0];
	return 0;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Realtek SSID verification
 */

/* Could be any non-zero and even value. When used as fixup, tells
 * the driver to ignore any present sku defines.
 */
#define ALC_FIXUP_SKU_IGNORE (2)

<<<<<<< HEAD
=======
static void alc_fixup_sku_ignore(struct hda_codec *codec,
				 const struct hda_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;
	if (action == HDA_FIXUP_ACT_PRE_PROBE) {
		spec->cdefine.fixup = 1;
		spec->cdefine.sku_cfg = ALC_FIXUP_SKU_IGNORE;
	}
}

static void alc_fixup_no_depop_delay(struct hda_codec *codec,
				    const struct hda_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;

	if (action == HDA_FIXUP_ACT_PROBE) {
		spec->no_depop_delay = 1;
		codec->depop_delay = 0;
	}
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int alc_auto_parse_customize_define(struct hda_codec *codec)
{
	unsigned int ass, tmp, i;
	unsigned nid = 0;
	struct alc_spec *spec = codec->spec;

	spec->cdefine.enable_pcbeep = 1; /* assume always enabled */

	if (spec->cdefine.fixup) {
		ass = spec->cdefine.sku_cfg;
		if (ass == ALC_FIXUP_SKU_IGNORE)
			return -1;
		goto do_sku;
	}

<<<<<<< HEAD
	ass = codec->subsystem_id & 0xffff;
=======
	if (!codec->bus->pci)
		return -1;
	ass = codec->core.subsystem_id & 0xffff;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ass != codec->bus->pci->subsystem_device && (ass & 1))
		goto do_sku;

	nid = 0x1d;
<<<<<<< HEAD
	if (codec->vendor_id == 0x10ec0260)
=======
	if (codec->core.vendor_id == 0x10ec0260)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		nid = 0x17;
	ass = snd_hda_codec_get_pincfg(codec, nid);

	if (!(ass & 1)) {
<<<<<<< HEAD
		printk(KERN_INFO "hda_codec: %s: SKU not ready 0x%08x\n",
		       codec->chip_name, ass);
=======
		codec_info(codec, "%s: SKU not ready 0x%08x\n",
			   codec->core.chip_name, ass);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -1;
	}

	/* check sum */
	tmp = 0;
	for (i = 1; i < 16; i++) {
		if ((ass >> i) & 1)
			tmp++;
	}
	if (((ass >> 16) & 0xf) != tmp)
		return -1;

	spec->cdefine.port_connectivity = ass >> 30;
	spec->cdefine.enable_pcbeep = (ass & 0x100000) >> 20;
	spec->cdefine.check_sum = (ass >> 16) & 0xf;
	spec->cdefine.customization = ass >> 8;
do_sku:
	spec->cdefine.sku_cfg = ass;
	spec->cdefine.external_amp = (ass & 0x38) >> 3;
	spec->cdefine.platform_type = (ass & 0x4) >> 2;
	spec->cdefine.swap = (ass & 0x2) >> 1;
	spec->cdefine.override = ass & 0x1;

<<<<<<< HEAD
	snd_printd("SKU: Nid=0x%x sku_cfg=0x%08x\n",
		   nid, spec->cdefine.sku_cfg);
	snd_printd("SKU: port_connectivity=0x%x\n",
		   spec->cdefine.port_connectivity);
	snd_printd("SKU: enable_pcbeep=0x%x\n", spec->cdefine.enable_pcbeep);
	snd_printd("SKU: check_sum=0x%08x\n", spec->cdefine.check_sum);
	snd_printd("SKU: customization=0x%08x\n", spec->cdefine.customization);
	snd_printd("SKU: external_amp=0x%x\n", spec->cdefine.external_amp);
	snd_printd("SKU: platform_type=0x%x\n", spec->cdefine.platform_type);
	snd_printd("SKU: swap=0x%x\n", spec->cdefine.swap);
	snd_printd("SKU: override=0x%x\n", spec->cdefine.override);
=======
	codec_dbg(codec, "SKU: Nid=0x%x sku_cfg=0x%08x\n",
		   nid, spec->cdefine.sku_cfg);
	codec_dbg(codec, "SKU: port_connectivity=0x%x\n",
		   spec->cdefine.port_connectivity);
	codec_dbg(codec, "SKU: enable_pcbeep=0x%x\n", spec->cdefine.enable_pcbeep);
	codec_dbg(codec, "SKU: check_sum=0x%08x\n", spec->cdefine.check_sum);
	codec_dbg(codec, "SKU: customization=0x%08x\n", spec->cdefine.customization);
	codec_dbg(codec, "SKU: external_amp=0x%x\n", spec->cdefine.external_amp);
	codec_dbg(codec, "SKU: platform_type=0x%x\n", spec->cdefine.platform_type);
	codec_dbg(codec, "SKU: swap=0x%x\n", spec->cdefine.swap);
	codec_dbg(codec, "SKU: override=0x%x\n", spec->cdefine.override);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
=======
/* return the position of NID in the list, or -1 if not found */
static int find_idx_in_nid_list(hda_nid_t nid, const hda_nid_t *list, int nums)
{
	int i;
	for (i = 0; i < nums; i++)
		if (list[i] == nid)
			return i;
	return -1;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* return true if the given NID is found in the list */
static bool found_in_nid_list(hda_nid_t nid, const hda_nid_t *list, int nums)
{
	return find_idx_in_nid_list(nid, list, nums) >= 0;
}

/* check subsystem ID and set up device-specific initialization;
 * return 1 if initialized, 0 if invalid SSID
 */
/* 32-bit subsystem ID for BIOS loading in HD Audio codec.
 *	31 ~ 16 :	Manufacture ID
 *	15 ~ 8	:	SKU ID
 *	7  ~ 0	:	Assembly ID
 *	port-A --> pin 39/41, port-E --> pin 14/15, port-D --> pin 35/36
 */
<<<<<<< HEAD
static int alc_subsystem_id(struct hda_codec *codec,
			    hda_nid_t porta, hda_nid_t porte,
			    hda_nid_t portd, hda_nid_t porti)
=======
static int alc_subsystem_id(struct hda_codec *codec, const hda_nid_t *ports)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned int ass, tmp, i;
	unsigned nid;
	struct alc_spec *spec = codec->spec;

	if (spec->cdefine.fixup) {
		ass = spec->cdefine.sku_cfg;
		if (ass == ALC_FIXUP_SKU_IGNORE)
			return 0;
		goto do_sku;
	}

<<<<<<< HEAD
	ass = codec->subsystem_id & 0xffff;
	if ((ass != codec->bus->pci->subsystem_device) && (ass & 1))
=======
	ass = codec->core.subsystem_id & 0xffff;
	if (codec->bus->pci &&
	    ass != codec->bus->pci->subsystem_device && (ass & 1))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto do_sku;

	/* invalid SSID, check the special NID pin defcfg instead */
	/*
	 * 31~30	: port connectivity
	 * 29~21	: reserve
	 * 20		: PCBEEP input
	 * 19~16	: Check sum (15:1)
	 * 15~1		: Custom
	 * 0		: override
	*/
	nid = 0x1d;
<<<<<<< HEAD
	if (codec->vendor_id == 0x10ec0260)
		nid = 0x17;
	ass = snd_hda_codec_get_pincfg(codec, nid);
	snd_printd("realtek: No valid SSID, "
		   "checking pincfg 0x%08x for NID 0x%x\n",
=======
	if (codec->core.vendor_id == 0x10ec0260)
		nid = 0x17;
	ass = snd_hda_codec_get_pincfg(codec, nid);
	codec_dbg(codec,
		  "realtek: No valid SSID, checking pincfg 0x%08x for NID 0x%x\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		   ass, nid);
	if (!(ass & 1))
		return 0;
	if ((ass >> 30) != 1)	/* no physical connection */
		return 0;

	/* check sum */
	tmp = 0;
	for (i = 1; i < 16; i++) {
		if ((ass >> i) & 1)
			tmp++;
	}
	if (((ass >> 16) & 0xf) != tmp)
		return 0;
do_sku:
<<<<<<< HEAD
	snd_printd("realtek: Enabling init ASM_ID=0x%04x CODEC_ID=%08x\n",
		   ass & 0xffff, codec->vendor_id);
=======
	codec_dbg(codec, "realtek: Enabling init ASM_ID=0x%04x CODEC_ID=%08x\n",
		   ass & 0xffff, codec->core.vendor_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * 0 : override
	 * 1 :	Swap Jack
	 * 2 : 0 --> Desktop, 1 --> Laptop
	 * 3~5 : External Amplifier control
	 * 7~6 : Reserved
	*/
	tmp = (ass & 0x38) >> 3;	/* external Amp control */
<<<<<<< HEAD
	switch (tmp) {
	case 1:
		spec->init_amp = ALC_INIT_GPIO1;
		break;
	case 3:
		spec->init_amp = ALC_INIT_GPIO2;
		break;
	case 7:
		spec->init_amp = ALC_INIT_GPIO3;
		break;
	case 5:
	default:
		spec->init_amp = ALC_INIT_DEFAULT;
		break;
=======
	if (spec->init_amp == ALC_INIT_UNDEFINED) {
		switch (tmp) {
		case 1:
			alc_setup_gpio(codec, 0x01);
			break;
		case 3:
			alc_setup_gpio(codec, 0x02);
			break;
		case 7:
			alc_setup_gpio(codec, 0x04);
			break;
		case 5:
		default:
			spec->init_amp = ALC_INIT_DEFAULT;
			break;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* is laptop or Desktop and enable the function "Mute internal speaker
	 * when the external headphone out jack is plugged"
	 */
	if (!(ass & 0x8000))
		return 1;
	/*
	 * 10~8 : Jack location
	 * 12~11: Headphone out -> 00: PortA, 01: PortE, 02: PortD, 03: Resvered
	 * 14~13: Resvered
	 * 15   : 1 --> enable the function "Mute internal speaker
	 *	        when the external headphone out jack is plugged"
	 */
<<<<<<< HEAD
	if (!spec->autocfg.hp_pins[0] &&
	    !(spec->autocfg.line_out_pins[0] &&
	      spec->autocfg.line_out_type == AUTO_PIN_HP_OUT)) {
		hda_nid_t nid;
		tmp = (ass >> 11) & 0x3;	/* HP to chassis */
		if (tmp == 0)
			nid = porta;
		else if (tmp == 1)
			nid = porte;
		else if (tmp == 2)
			nid = portd;
		else if (tmp == 3)
			nid = porti;
		else
			return 1;
		if (found_in_nid_list(nid, spec->autocfg.line_out_pins,
				      spec->autocfg.line_outs))
			return 1;
		spec->autocfg.hp_pins[0] = nid;
=======
	if (!alc_get_hp_pin(spec)) {
		hda_nid_t nid;
		tmp = (ass >> 11) & 0x3;	/* HP to chassis */
		nid = ports[tmp];
		if (found_in_nid_list(nid, spec->gen.autocfg.line_out_pins,
				      spec->gen.autocfg.line_outs))
			return 1;
		spec->gen.autocfg.hp_pins[0] = nid;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return 1;
}

/* Check the validity of ALC subsystem-id
 * ports contains an array of 4 pin NIDs for port-A, E, D and I */
static void alc_ssid_check(struct hda_codec *codec, const hda_nid_t *ports)
{
<<<<<<< HEAD
	if (!alc_subsystem_id(codec, ports[0], ports[1], ports[2], ports[3])) {
		struct alc_spec *spec = codec->spec;
		snd_printd("realtek: "
			   "Enable default setup for auto mode as fallback\n");
		spec->init_amp = ALC_INIT_DEFAULT;
	}
}

/*
 * Fix-up pin default configurations and add default verbs
 */

struct alc_pincfg {
	hda_nid_t nid;
	u32 val;
};

struct alc_model_fixup {
	const int id;
	const char *name;
};

struct alc_fixup {
	int type;
	bool chained;
	int chain_id;
	union {
		unsigned int sku;
		const struct alc_pincfg *pins;
		const struct hda_verb *verbs;
		void (*func)(struct hda_codec *codec,
			     const struct alc_fixup *fix,
			     int action);
	} v;
};

enum {
	ALC_FIXUP_INVALID,
	ALC_FIXUP_SKU,
	ALC_FIXUP_PINS,
	ALC_FIXUP_VERBS,
	ALC_FIXUP_FUNC,
};

enum {
	ALC_FIXUP_ACT_PRE_PROBE,
	ALC_FIXUP_ACT_PROBE,
	ALC_FIXUP_ACT_INIT,
	ALC_FIXUP_ACT_BUILD,
};

static void alc_apply_pincfgs(struct hda_codec *codec,
			      const struct alc_pincfg *cfg)
{
	for (; cfg->nid; cfg++)
		snd_hda_codec_set_pincfg(codec, cfg->nid, cfg->val);
}

static void alc_apply_fixup(struct hda_codec *codec, int action)
{
	struct alc_spec *spec = codec->spec;
	int id = spec->fixup_id;
#ifdef CONFIG_SND_DEBUG_VERBOSE
	const char *modelname = spec->fixup_name;
#endif
	int depth = 0;

	if (!spec->fixup_list)
		return;

	while (id >= 0) {
		const struct alc_fixup *fix = spec->fixup_list + id;
		const struct alc_pincfg *cfg;

		switch (fix->type) {
		case ALC_FIXUP_SKU:
			if (action != ALC_FIXUP_ACT_PRE_PROBE || !fix->v.sku)
				break;
			snd_printdd(KERN_INFO "hda_codec: %s: "
				    "Apply sku override for %s\n",
				    codec->chip_name, modelname);
			spec->cdefine.sku_cfg = fix->v.sku;
			spec->cdefine.fixup = 1;
			break;
		case ALC_FIXUP_PINS:
			cfg = fix->v.pins;
			if (action != ALC_FIXUP_ACT_PRE_PROBE || !cfg)
				break;
			snd_printdd(KERN_INFO "hda_codec: %s: "
				    "Apply pincfg for %s\n",
				    codec->chip_name, modelname);
			alc_apply_pincfgs(codec, cfg);
			break;
		case ALC_FIXUP_VERBS:
			if (action != ALC_FIXUP_ACT_PROBE || !fix->v.verbs)
				break;
			snd_printdd(KERN_INFO "hda_codec: %s: "
				    "Apply fix-verbs for %s\n",
				    codec->chip_name, modelname);
			add_verb(codec->spec, fix->v.verbs);
			break;
		case ALC_FIXUP_FUNC:
			if (!fix->v.func)
				break;
			snd_printdd(KERN_INFO "hda_codec: %s: "
				    "Apply fix-func for %s\n",
				    codec->chip_name, modelname);
			fix->v.func(codec, fix, action);
			break;
		default:
			snd_printk(KERN_ERR "hda_codec: %s: "
				   "Invalid fixup type %d\n",
				   codec->chip_name, fix->type);
			break;
		}
		if (!fix->chained)
			break;
		if (++depth > 10)
			break;
		id = fix->chain_id;
	}
}

static void alc_pick_fixup(struct hda_codec *codec,
			   const struct alc_model_fixup *models,
			   const struct snd_pci_quirk *quirk,
			   const struct alc_fixup *fixlist)
{
	struct alc_spec *spec = codec->spec;
	const struct snd_pci_quirk *q;
	int id = -1;
	const char *name = NULL;

	/* when model=nofixup is given, don't pick up any fixups */
	if (codec->modelname && !strcmp(codec->modelname, "nofixup")) {
		spec->fixup_list = NULL;
		spec->fixup_id = -1;
		return;
	}

	if (codec->modelname && models) {
		while (models->name) {
			if (!strcmp(codec->modelname, models->name)) {
				id = models->id;
				name = models->name;
				break;
			}
			models++;
		}
	}
	if (id < 0) {
		q = snd_pci_quirk_lookup(codec->bus->pci, quirk);
		if (q) {
			id = q->value;
#ifdef CONFIG_SND_DEBUG_VERBOSE
			name = q->name;
#endif
		}
	}
	if (id < 0) {
		for (q = quirk; q->subvendor; q++) {
			unsigned int vendorid =
				q->subdevice | (q->subvendor << 16);
			if (vendorid == codec->subsystem_id) {
				id = q->value;
#ifdef CONFIG_SND_DEBUG_VERBOSE
				name = q->name;
#endif
				break;
			}
		}
	}

	spec->fixup_id = id;
	if (id >= 0) {
		spec->fixup_list = fixlist;
		spec->fixup_name = name;
	}
}

/*
 * COEF access helper functions
 */
static int alc_read_coef_idx(struct hda_codec *codec,
			unsigned int coef_idx)
{
	unsigned int val;
	snd_hda_codec_write(codec, 0x20, 0, AC_VERB_SET_COEF_INDEX,
		    		coef_idx);
	val = snd_hda_codec_read(codec, 0x20, 0,
			 	AC_VERB_GET_PROC_COEF, 0);
	return val;
}

static void alc_write_coef_idx(struct hda_codec *codec, unsigned int coef_idx,
							unsigned int coef_val)
{
	snd_hda_codec_write(codec, 0x20, 0, AC_VERB_SET_COEF_INDEX,
			    coef_idx);
	snd_hda_codec_write(codec, 0x20, 0, AC_VERB_SET_PROC_COEF,
			    coef_val);
}

/* a special bypass for COEF 0; read the cached value at the second time */
static unsigned int alc_get_coef0(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	if (!spec->coef0)
		spec->coef0 = alc_read_coef_idx(codec, 0);
	return spec->coef0;
}

/*
 * Digital I/O handling
 */

/* set right pin controls for digital I/O */
static void alc_auto_init_digital(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	int i;
	hda_nid_t pin, dac;

	for (i = 0; i < spec->autocfg.dig_outs; i++) {
		pin = spec->autocfg.dig_out_pins[i];
		if (!pin)
			continue;
		snd_hda_codec_write(codec, pin, 0,
				    AC_VERB_SET_PIN_WIDGET_CONTROL, PIN_OUT);
		if (!i)
			dac = spec->multiout.dig_out_nid;
		else
			dac = spec->slave_dig_outs[i - 1];
		if (!dac || !(get_wcaps(codec, dac) & AC_WCAP_OUT_AMP))
			continue;
		snd_hda_codec_write(codec, dac, 0,
				    AC_VERB_SET_AMP_GAIN_MUTE,
				    AMP_OUT_UNMUTE);
	}
	pin = spec->autocfg.dig_in_pin;
	if (pin)
		snd_hda_codec_write(codec, pin, 0,
				    AC_VERB_SET_PIN_WIDGET_CONTROL,
				    PIN_IN);
}

/* parse digital I/Os and set up NIDs in BIOS auto-parse mode */
static void alc_auto_parse_digital(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	int i, err, nums;
	hda_nid_t dig_nid;

	/* support multiple SPDIFs; the secondary is set up as a slave */
	nums = 0;
	for (i = 0; i < spec->autocfg.dig_outs; i++) {
		hda_nid_t conn[4];
		err = snd_hda_get_connections(codec,
					      spec->autocfg.dig_out_pins[i],
					      conn, ARRAY_SIZE(conn));
		if (err <= 0)
			continue;
		dig_nid = conn[0]; /* assume the first element is audio-out */
		if (!nums) {
			spec->multiout.dig_out_nid = dig_nid;
			spec->dig_out_type = spec->autocfg.dig_out_type[0];
		} else {
			spec->multiout.slave_dig_outs = spec->slave_dig_outs;
			if (nums >= ARRAY_SIZE(spec->slave_dig_outs) - 1)
				break;
			spec->slave_dig_outs[nums - 1] = dig_nid;
		}
		nums++;
	}

	if (spec->autocfg.dig_in_pin) {
		dig_nid = codec->start_nid;
		for (i = 0; i < codec->num_nodes; i++, dig_nid++) {
			unsigned int wcaps = get_wcaps(codec, dig_nid);
			if (get_wcaps_type(wcaps) != AC_WID_AUD_IN)
				continue;
			if (!(wcaps & AC_WCAP_DIGITAL))
				continue;
			if (!(wcaps & AC_WCAP_CONN_LIST))
				continue;
			err = get_connection_index(codec, dig_nid,
						   spec->autocfg.dig_in_pin);
			if (err >= 0) {
				spec->dig_in_nid = dig_nid;
				break;
			}
=======
	if (!alc_subsystem_id(codec, ports)) {
		struct alc_spec *spec = codec->spec;
		if (spec->init_amp == ALC_INIT_UNDEFINED) {
			codec_dbg(codec,
				  "realtek: Enable default setup for auto mode as fallback\n");
			spec->init_amp = ALC_INIT_DEFAULT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}
}

/*
<<<<<<< HEAD
 * capture mixer elements
 */
static int alc_cap_vol_info(struct snd_kcontrol *kcontrol,
			    struct snd_ctl_elem_info *uinfo)
{
	struct hda_codec *codec = snd_kcontrol_chip(kcontrol);
	struct alc_spec *spec = codec->spec;
	unsigned long val;
	int err;

	mutex_lock(&codec->control_mutex);
	if (spec->vol_in_capsrc)
		val = HDA_COMPOSE_AMP_VAL(spec->capsrc_nids[0], 3, 0, HDA_OUTPUT);
	else
		val = HDA_COMPOSE_AMP_VAL(spec->adc_nids[0], 3, 0, HDA_INPUT);
	kcontrol->private_value = val;
	err = snd_hda_mixer_amp_volume_info(kcontrol, uinfo);
	mutex_unlock(&codec->control_mutex);
	return err;
}

static int alc_cap_vol_tlv(struct snd_kcontrol *kcontrol, int op_flag,
			   unsigned int size, unsigned int __user *tlv)
{
	struct hda_codec *codec = snd_kcontrol_chip(kcontrol);
	struct alc_spec *spec = codec->spec;
	unsigned long val;
	int err;

	mutex_lock(&codec->control_mutex);
	if (spec->vol_in_capsrc)
		val = HDA_COMPOSE_AMP_VAL(spec->capsrc_nids[0], 3, 0, HDA_OUTPUT);
	else
		val = HDA_COMPOSE_AMP_VAL(spec->adc_nids[0], 3, 0, HDA_INPUT);
	kcontrol->private_value = val;
	err = snd_hda_mixer_amp_tlv(kcontrol, op_flag, size, tlv);
	mutex_unlock(&codec->control_mutex);
	return err;
}

typedef int (*getput_call_t)(struct snd_kcontrol *kcontrol,
			     struct snd_ctl_elem_value *ucontrol);

static int alc_cap_getput_caller(struct snd_kcontrol *kcontrol,
				 struct snd_ctl_elem_value *ucontrol,
				 getput_call_t func, bool check_adc_switch)
{
	struct hda_codec *codec = snd_kcontrol_chip(kcontrol);
	struct alc_spec *spec = codec->spec;
	int i, err = 0;

	mutex_lock(&codec->control_mutex);
	if (check_adc_switch && spec->dyn_adc_switch) {
		for (i = 0; i < spec->num_adc_nids; i++) {
			kcontrol->private_value =
				HDA_COMPOSE_AMP_VAL(spec->adc_nids[i],
						    3, 0, HDA_INPUT);
			err = func(kcontrol, ucontrol);
			if (err < 0)
				goto error;
		}
	} else {
		i = snd_ctl_get_ioffidx(kcontrol, &ucontrol->id);
		if (spec->vol_in_capsrc)
			kcontrol->private_value =
				HDA_COMPOSE_AMP_VAL(spec->capsrc_nids[i],
						    3, 0, HDA_OUTPUT);
		else
			kcontrol->private_value =
				HDA_COMPOSE_AMP_VAL(spec->adc_nids[i],
						    3, 0, HDA_INPUT);
		err = func(kcontrol, ucontrol);
	}
 error:
	mutex_unlock(&codec->control_mutex);
	return err;
}

static int alc_cap_vol_get(struct snd_kcontrol *kcontrol,
			   struct snd_ctl_elem_value *ucontrol)
{
	return alc_cap_getput_caller(kcontrol, ucontrol,
				     snd_hda_mixer_amp_volume_get, false);
}

static int alc_cap_vol_put(struct snd_kcontrol *kcontrol,
			   struct snd_ctl_elem_value *ucontrol)
{
	return alc_cap_getput_caller(kcontrol, ucontrol,
				     snd_hda_mixer_amp_volume_put, true);
}

/* capture mixer elements */
#define alc_cap_sw_info		snd_ctl_boolean_stereo_info

static int alc_cap_sw_get(struct snd_kcontrol *kcontrol,
			  struct snd_ctl_elem_value *ucontrol)
{
	return alc_cap_getput_caller(kcontrol, ucontrol,
				     snd_hda_mixer_amp_switch_get, false);
}

static int alc_cap_sw_put(struct snd_kcontrol *kcontrol,
			  struct snd_ctl_elem_value *ucontrol)
{
	return alc_cap_getput_caller(kcontrol, ucontrol,
				     snd_hda_mixer_amp_switch_put, true);
}

#define _DEFINE_CAPMIX(num) \
	{ \
		.iface = SNDRV_CTL_ELEM_IFACE_MIXER, \
		.name = "Capture Switch", \
		.access = SNDRV_CTL_ELEM_ACCESS_READWRITE, \
		.count = num, \
		.info = alc_cap_sw_info, \
		.get = alc_cap_sw_get, \
		.put = alc_cap_sw_put, \
	}, \
	{ \
		.iface = SNDRV_CTL_ELEM_IFACE_MIXER, \
		.name = "Capture Volume", \
		.access = (SNDRV_CTL_ELEM_ACCESS_READWRITE | \
			   SNDRV_CTL_ELEM_ACCESS_TLV_READ | \
			   SNDRV_CTL_ELEM_ACCESS_TLV_CALLBACK), \
		.count = num, \
		.info = alc_cap_vol_info, \
		.get = alc_cap_vol_get, \
		.put = alc_cap_vol_put, \
		.tlv = { .c = alc_cap_vol_tlv }, \
	}

#define _DEFINE_CAPSRC(num) \
	{ \
		.iface = SNDRV_CTL_ELEM_IFACE_MIXER, \
		/* .name = "Capture Source", */ \
		.name = "Input Source", \
		.count = num, \
		.info = alc_mux_enum_info, \
		.get = alc_mux_enum_get, \
		.put = alc_mux_enum_put, \
	}

#define DEFINE_CAPMIX(num) \
static const struct snd_kcontrol_new alc_capture_mixer ## num[] = { \
	_DEFINE_CAPMIX(num),				      \
	_DEFINE_CAPSRC(num),				      \
	{ } /* end */					      \
}

#define DEFINE_CAPMIX_NOSRC(num) \
static const struct snd_kcontrol_new alc_capture_mixer_nosrc ## num[] = { \
	_DEFINE_CAPMIX(num),					    \
	{ } /* end */						    \
}

/* up to three ADCs */
DEFINE_CAPMIX(1);
DEFINE_CAPMIX(2);
DEFINE_CAPMIX(3);
DEFINE_CAPMIX_NOSRC(1);
DEFINE_CAPMIX_NOSRC(2);
DEFINE_CAPMIX_NOSRC(3);

/*
 * virtual master controls
 */

/*
 * slave controls for virtual master
 */
static const char * const alc_slave_pfxs[] = {
	"Front", "Surround", "Center", "LFE", "Side",
	"Headphone", "Speaker", "Mono", "Line Out",
	"CLFE", "Bass Speaker", "PCM",
	NULL,
};

/*
 * build control elements
 */

#define NID_MAPPING		(-1)

#define SUBDEV_SPEAKER_		(0 << 6)
#define SUBDEV_HP_		(1 << 6)
#define SUBDEV_LINE_		(2 << 6)
#define SUBDEV_SPEAKER(x)	(SUBDEV_SPEAKER_ | ((x) & 0x3f))
#define SUBDEV_HP(x)		(SUBDEV_HP_ | ((x) & 0x3f))
#define SUBDEV_LINE(x)		(SUBDEV_LINE_ | ((x) & 0x3f))

static void alc_free_kctls(struct hda_codec *codec);

#ifdef CONFIG_SND_HDA_INPUT_BEEP
/* additional beep mixers; the actual parameters are overwritten at build */
static const struct snd_kcontrol_new alc_beep_mixer[] = {
	HDA_CODEC_VOLUME("Beep Playback Volume", 0, 0, HDA_INPUT),
	HDA_CODEC_MUTE_BEEP("Beep Playback Switch", 0, 0, HDA_INPUT),
	{ } /* end */
};
#endif

static int __alc_build_controls(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	struct snd_kcontrol *kctl = NULL;
	const struct snd_kcontrol_new *knew;
	int i, j, err;
	unsigned int u;
	hda_nid_t nid;

	for (i = 0; i < spec->num_mixers; i++) {
		err = snd_hda_add_new_ctls(codec, spec->mixers[i]);
		if (err < 0)
			return err;
	}
	if (spec->cap_mixer) {
		err = snd_hda_add_new_ctls(codec, spec->cap_mixer);
		if (err < 0)
			return err;
	}
	if (spec->multiout.dig_out_nid) {
		err = snd_hda_create_spdif_out_ctls(codec,
						    spec->multiout.dig_out_nid,
						    spec->multiout.dig_out_nid);
		if (err < 0)
			return err;
		if (!spec->no_analog) {
			err = snd_hda_create_spdif_share_sw(codec,
							    &spec->multiout);
			if (err < 0)
				return err;
			spec->multiout.share_spdif = 1;
		}
	}
	if (spec->dig_in_nid) {
		err = snd_hda_create_spdif_in_ctls(codec, spec->dig_in_nid);
		if (err < 0)
			return err;
	}

#ifdef CONFIG_SND_HDA_INPUT_BEEP
	/* create beep controls if needed */
	if (spec->beep_amp) {
		const struct snd_kcontrol_new *knew;
		for (knew = alc_beep_mixer; knew->name; knew++) {
			struct snd_kcontrol *kctl;
			kctl = snd_ctl_new1(knew, codec);
			if (!kctl)
				return -ENOMEM;
			kctl->private_value = spec->beep_amp;
			err = snd_hda_ctl_add(codec, 0, kctl);
			if (err < 0)
				return err;
		}
	}
#endif

	/* if we have no master control, let's create it */
	if (!spec->no_analog &&
	    !snd_hda_find_mixer_ctl(codec, "Master Playback Volume")) {
		unsigned int vmaster_tlv[4];
		snd_hda_set_vmaster_tlv(codec, spec->vmaster_nid,
					HDA_OUTPUT, vmaster_tlv);
		err = snd_hda_add_vmaster(codec, "Master Playback Volume",
					  vmaster_tlv, alc_slave_pfxs,
					  "Playback Volume");
		if (err < 0)
			return err;
	}
	if (!spec->no_analog &&
	    !snd_hda_find_mixer_ctl(codec, "Master Playback Switch")) {
		err = __snd_hda_add_vmaster(codec, "Master Playback Switch",
					    NULL, alc_slave_pfxs,
					    "Playback Switch",
					    true, &spec->vmaster_mute.sw_kctl);
		if (err < 0)
			return err;
	}

	/* assign Capture Source enums to NID */
	if (spec->capsrc_nids || spec->adc_nids) {
		kctl = snd_hda_find_mixer_ctl(codec, "Capture Source");
		if (!kctl)
			kctl = snd_hda_find_mixer_ctl(codec, "Input Source");
		for (i = 0; kctl && i < kctl->count; i++) {
			err = snd_hda_add_nid(codec, kctl, i,
					      get_capsrc(spec, i));
			if (err < 0)
				return err;
		}
	}
	if (spec->cap_mixer && spec->adc_nids) {
		const char *kname = kctl ? kctl->id.name : NULL;
		for (knew = spec->cap_mixer; knew->name; knew++) {
			if (kname && strcmp(knew->name, kname) == 0)
				continue;
			kctl = snd_hda_find_mixer_ctl(codec, knew->name);
			for (i = 0; kctl && i < kctl->count; i++) {
				err = snd_hda_add_nid(codec, kctl, i,
						      spec->adc_nids[i]);
				if (err < 0)
					return err;
			}
		}
	}

	/* other nid->control mapping */
	for (i = 0; i < spec->num_mixers; i++) {
		for (knew = spec->mixers[i]; knew->name; knew++) {
			if (knew->iface != NID_MAPPING)
				continue;
			kctl = snd_hda_find_mixer_ctl(codec, knew->name);
			if (kctl == NULL)
				continue;
			u = knew->subdevice;
			for (j = 0; j < 4; j++, u >>= 8) {
				nid = u & 0x3f;
				if (nid == 0)
					continue;
				switch (u & 0xc0) {
				case SUBDEV_SPEAKER_:
					nid = spec->autocfg.speaker_pins[nid];
					break;
				case SUBDEV_LINE_:
					nid = spec->autocfg.line_out_pins[nid];
					break;
				case SUBDEV_HP_:
					nid = spec->autocfg.hp_pins[nid];
					break;
				default:
					continue;
				}
				err = snd_hda_add_nid(codec, kctl, 0, nid);
				if (err < 0)
					return err;
			}
			u = knew->private_value;
			for (j = 0; j < 4; j++, u >>= 8) {
				nid = u & 0xff;
				if (nid == 0)
					continue;
				err = snd_hda_add_nid(codec, kctl, 0, nid);
				if (err < 0)
					return err;
			}
		}
	}

	alc_free_kctls(codec); /* no longer needed */

	return 0;
}

static int alc_build_controls(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	int err = __alc_build_controls(codec);
	if (err < 0)
		return err;
	err = snd_hda_jack_add_kctls(codec, &spec->autocfg);
	if (err < 0)
		return err;
	alc_apply_fixup(codec, ALC_FIXUP_ACT_BUILD);
=======
 */

static void alc_fixup_inv_dmic(struct hda_codec *codec,
			       const struct hda_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;

	spec->gen.inv_dmic_split = 1;
}


static int alc_build_controls(struct hda_codec *codec)
{
	int err;

	err = snd_hda_gen_build_controls(codec);
	if (err < 0)
		return err;

	snd_hda_apply_fixup(codec, HDA_FIXUP_ACT_BUILD);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}


/*
 * Common callbacks
 */

<<<<<<< HEAD
static void alc_init_special_input_src(struct hda_codec *codec);
static void alc_auto_init_std(struct hda_codec *codec);
=======
static void alc_pre_init(struct hda_codec *codec)
{
	alc_fill_eapd_coef(codec);
}

#define is_s3_resume(codec) \
	((codec)->core.dev.power.power_state.event == PM_EVENT_RESUME)
#define is_s4_resume(codec) \
	((codec)->core.dev.power.power_state.event == PM_EVENT_RESTORE)
#define is_s4_suspend(codec) \
	((codec)->core.dev.power.power_state.event == PM_EVENT_FREEZE)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int alc_init(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
<<<<<<< HEAD
	unsigned int i;
=======

	/* hibernation resume needs the full chip initialization */
	if (is_s4_resume(codec))
		alc_pre_init(codec);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (spec->init_hook)
		spec->init_hook(codec);

<<<<<<< HEAD
	alc_fix_pll(codec);
	alc_auto_init_amp(codec, spec->init_amp);

	for (i = 0; i < spec->num_init_verbs; i++)
		snd_hda_sequence_write(codec, spec->init_verbs[i]);
	alc_init_special_input_src(codec);
	alc_auto_init_std(codec);

	alc_apply_fixup(codec, ALC_FIXUP_ACT_INIT);

	snd_hda_jack_report_sync(codec);

	hda_call_check_power_status(codec, 0x01);
	return 0;
}

static void alc_unsol_event(struct hda_codec *codec, unsigned int res)
{
	struct alc_spec *spec = codec->spec;

	if (spec->unsol_event)
		spec->unsol_event(codec, res);
}

#ifdef CONFIG_SND_HDA_POWER_SAVE
static int alc_check_power_status(struct hda_codec *codec, hda_nid_t nid)
{
	struct alc_spec *spec = codec->spec;
	return snd_hda_check_amp_list_power(codec, &spec->loopback, nid);
}
#endif

/*
 * Analog playback callbacks
 */
static int alc_playback_pcm_open(struct hda_pcm_stream *hinfo,
				    struct hda_codec *codec,
				    struct snd_pcm_substream *substream)
{
	struct alc_spec *spec = codec->spec;
	return snd_hda_multi_out_analog_open(codec, &spec->multiout, substream,
					     hinfo);
}

static int alc_playback_pcm_prepare(struct hda_pcm_stream *hinfo,
				       struct hda_codec *codec,
				       unsigned int stream_tag,
				       unsigned int format,
				       struct snd_pcm_substream *substream)
{
	struct alc_spec *spec = codec->spec;
	return snd_hda_multi_out_analog_prepare(codec, &spec->multiout,
						stream_tag, format, substream);
}

static int alc_playback_pcm_cleanup(struct hda_pcm_stream *hinfo,
				       struct hda_codec *codec,
				       struct snd_pcm_substream *substream)
{
	struct alc_spec *spec = codec->spec;
	return snd_hda_multi_out_analog_cleanup(codec, &spec->multiout);
}

/*
 * Digital out
 */
static int alc_dig_playback_pcm_open(struct hda_pcm_stream *hinfo,
					struct hda_codec *codec,
					struct snd_pcm_substream *substream)
{
	struct alc_spec *spec = codec->spec;
	return snd_hda_multi_out_dig_open(codec, &spec->multiout);
}

static int alc_dig_playback_pcm_prepare(struct hda_pcm_stream *hinfo,
					   struct hda_codec *codec,
					   unsigned int stream_tag,
					   unsigned int format,
					   struct snd_pcm_substream *substream)
{
	struct alc_spec *spec = codec->spec;
	return snd_hda_multi_out_dig_prepare(codec, &spec->multiout,
					     stream_tag, format, substream);
}

static int alc_dig_playback_pcm_cleanup(struct hda_pcm_stream *hinfo,
					   struct hda_codec *codec,
					   struct snd_pcm_substream *substream)
{
	struct alc_spec *spec = codec->spec;
	return snd_hda_multi_out_dig_cleanup(codec, &spec->multiout);
}

static int alc_dig_playback_pcm_close(struct hda_pcm_stream *hinfo,
					 struct hda_codec *codec,
					 struct snd_pcm_substream *substream)
{
	struct alc_spec *spec = codec->spec;
	return snd_hda_multi_out_dig_close(codec, &spec->multiout);
}

/*
 * Analog capture
 */
static int alc_alt_capture_pcm_prepare(struct hda_pcm_stream *hinfo,
				      struct hda_codec *codec,
				      unsigned int stream_tag,
				      unsigned int format,
				      struct snd_pcm_substream *substream)
{
	struct alc_spec *spec = codec->spec;

	snd_hda_codec_setup_stream(codec, spec->adc_nids[substream->number + 1],
				   stream_tag, 0, format);
	return 0;
}

static int alc_alt_capture_pcm_cleanup(struct hda_pcm_stream *hinfo,
				      struct hda_codec *codec,
				      struct snd_pcm_substream *substream)
{
	struct alc_spec *spec = codec->spec;

	snd_hda_codec_cleanup_stream(codec,
				     spec->adc_nids[substream->number + 1]);
	return 0;
}

/* analog capture with dynamic dual-adc changes */
static int dyn_adc_capture_pcm_prepare(struct hda_pcm_stream *hinfo,
				       struct hda_codec *codec,
				       unsigned int stream_tag,
				       unsigned int format,
				       struct snd_pcm_substream *substream)
{
	struct alc_spec *spec = codec->spec;
	spec->cur_adc = spec->adc_nids[spec->dyn_adc_idx[spec->cur_mux[0]]];
	spec->cur_adc_stream_tag = stream_tag;
	spec->cur_adc_format = format;
	snd_hda_codec_setup_stream(codec, spec->cur_adc, stream_tag, 0, format);
	return 0;
}

static int dyn_adc_capture_pcm_cleanup(struct hda_pcm_stream *hinfo,
				       struct hda_codec *codec,
				       struct snd_pcm_substream *substream)
{
	struct alc_spec *spec = codec->spec;
	snd_hda_codec_cleanup_stream(codec, spec->cur_adc);
	spec->cur_adc = 0;
	return 0;
}

static const struct hda_pcm_stream dyn_adc_pcm_analog_capture = {
	.substreams = 1,
	.channels_min = 2,
	.channels_max = 2,
	.nid = 0, /* fill later */
	.ops = {
		.prepare = dyn_adc_capture_pcm_prepare,
		.cleanup = dyn_adc_capture_pcm_cleanup
	},
};

/*
 */
static const struct hda_pcm_stream alc_pcm_analog_playback = {
	.substreams = 1,
	.channels_min = 2,
	.channels_max = 8,
	/* NID is set in alc_build_pcms */
	.ops = {
		.open = alc_playback_pcm_open,
		.prepare = alc_playback_pcm_prepare,
		.cleanup = alc_playback_pcm_cleanup
	},
};

static const struct hda_pcm_stream alc_pcm_analog_capture = {
	.substreams = 1,
	.channels_min = 2,
	.channels_max = 2,
	/* NID is set in alc_build_pcms */
};

static const struct hda_pcm_stream alc_pcm_analog_alt_playback = {
	.substreams = 1,
	.channels_min = 2,
	.channels_max = 2,
	/* NID is set in alc_build_pcms */
};

static const struct hda_pcm_stream alc_pcm_analog_alt_capture = {
	.substreams = 2, /* can be overridden */
	.channels_min = 2,
	.channels_max = 2,
	/* NID is set in alc_build_pcms */
	.ops = {
		.prepare = alc_alt_capture_pcm_prepare,
		.cleanup = alc_alt_capture_pcm_cleanup
	},
};

static const struct hda_pcm_stream alc_pcm_digital_playback = {
	.substreams = 1,
	.channels_min = 2,
	.channels_max = 2,
	/* NID is set in alc_build_pcms */
	.ops = {
		.open = alc_dig_playback_pcm_open,
		.close = alc_dig_playback_pcm_close,
		.prepare = alc_dig_playback_pcm_prepare,
		.cleanup = alc_dig_playback_pcm_cleanup
	},
};

static const struct hda_pcm_stream alc_pcm_digital_capture = {
	.substreams = 1,
	.channels_min = 2,
	.channels_max = 2,
	/* NID is set in alc_build_pcms */
};

/* Used by alc_build_pcms to flag that a PCM has no playback stream */
static const struct hda_pcm_stream alc_pcm_null_stream = {
	.substreams = 0,
	.channels_min = 0,
	.channels_max = 0,
};

static int alc_build_pcms(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	struct hda_pcm *info = spec->pcm_rec;
	const struct hda_pcm_stream *p;
	bool have_multi_adcs;
	int i;

	codec->num_pcms = 1;
	codec->pcm_info = info;

	if (spec->no_analog)
		goto skip_analog;

	snprintf(spec->stream_name_analog, sizeof(spec->stream_name_analog),
		 "%s Analog", codec->chip_name);
	info->name = spec->stream_name_analog;

	if (spec->multiout.num_dacs > 0) {
		p = spec->stream_analog_playback;
		if (!p)
			p = &alc_pcm_analog_playback;
		info->stream[SNDRV_PCM_STREAM_PLAYBACK] = *p;
		info->stream[SNDRV_PCM_STREAM_PLAYBACK].nid = spec->multiout.dac_nids[0];
	}
	if (spec->adc_nids) {
		p = spec->stream_analog_capture;
		if (!p) {
			if (spec->dyn_adc_switch)
				p = &dyn_adc_pcm_analog_capture;
			else
				p = &alc_pcm_analog_capture;
		}
		info->stream[SNDRV_PCM_STREAM_CAPTURE] = *p;
		info->stream[SNDRV_PCM_STREAM_CAPTURE].nid = spec->adc_nids[0];
	}

	if (spec->channel_mode) {
		info->stream[SNDRV_PCM_STREAM_PLAYBACK].channels_max = 0;
		for (i = 0; i < spec->num_channel_mode; i++) {
			if (spec->channel_mode[i].channels > info->stream[SNDRV_PCM_STREAM_PLAYBACK].channels_max) {
				info->stream[SNDRV_PCM_STREAM_PLAYBACK].channels_max = spec->channel_mode[i].channels;
			}
		}
	}

 skip_analog:
	/* SPDIF for stream index #1 */
	if (spec->multiout.dig_out_nid || spec->dig_in_nid) {
		snprintf(spec->stream_name_digital,
			 sizeof(spec->stream_name_digital),
			 "%s Digital", codec->chip_name);
		codec->num_pcms = 2;
	        codec->slave_dig_outs = spec->multiout.slave_dig_outs;
		info = spec->pcm_rec + 1;
		info->name = spec->stream_name_digital;
		if (spec->dig_out_type)
			info->pcm_type = spec->dig_out_type;
		else
			info->pcm_type = HDA_PCM_TYPE_SPDIF;
		if (spec->multiout.dig_out_nid) {
			p = spec->stream_digital_playback;
			if (!p)
				p = &alc_pcm_digital_playback;
			info->stream[SNDRV_PCM_STREAM_PLAYBACK] = *p;
			info->stream[SNDRV_PCM_STREAM_PLAYBACK].nid = spec->multiout.dig_out_nid;
		}
		if (spec->dig_in_nid) {
			p = spec->stream_digital_capture;
			if (!p)
				p = &alc_pcm_digital_capture;
			info->stream[SNDRV_PCM_STREAM_CAPTURE] = *p;
			info->stream[SNDRV_PCM_STREAM_CAPTURE].nid = spec->dig_in_nid;
		}
		/* FIXME: do we need this for all Realtek codec models? */
		codec->spdif_status_reset = 1;
	}

	if (spec->no_analog)
		return 0;

	/* If the use of more than one ADC is requested for the current
	 * model, configure a second analog capture-only PCM.
	 */
	have_multi_adcs = (spec->num_adc_nids > 1) &&
		!spec->dyn_adc_switch && !spec->auto_mic &&
		(!spec->input_mux || spec->input_mux->num_items > 1);
	/* Additional Analaog capture for index #2 */
	if (spec->alt_dac_nid || have_multi_adcs) {
		codec->num_pcms = 3;
		info = spec->pcm_rec + 2;
		info->name = spec->stream_name_analog;
		if (spec->alt_dac_nid) {
			p = spec->stream_analog_alt_playback;
			if (!p)
				p = &alc_pcm_analog_alt_playback;
			info->stream[SNDRV_PCM_STREAM_PLAYBACK] = *p;
			info->stream[SNDRV_PCM_STREAM_PLAYBACK].nid =
				spec->alt_dac_nid;
		} else {
			info->stream[SNDRV_PCM_STREAM_PLAYBACK] =
				alc_pcm_null_stream;
			info->stream[SNDRV_PCM_STREAM_PLAYBACK].nid = 0;
		}
		if (have_multi_adcs) {
			p = spec->stream_analog_alt_capture;
			if (!p)
				p = &alc_pcm_analog_alt_capture;
			info->stream[SNDRV_PCM_STREAM_CAPTURE] = *p;
			info->stream[SNDRV_PCM_STREAM_CAPTURE].nid =
				spec->adc_nids[1];
			info->stream[SNDRV_PCM_STREAM_CAPTURE].substreams =
				spec->num_adc_nids - 1;
		} else {
			info->stream[SNDRV_PCM_STREAM_CAPTURE] =
				alc_pcm_null_stream;
			info->stream[SNDRV_PCM_STREAM_CAPTURE].nid = 0;
		}
	}
=======
	spec->gen.skip_verbs = 1; /* applied in below */
	snd_hda_gen_init(codec);
	alc_fix_pll(codec);
	alc_auto_init_amp(codec, spec->init_amp);
	snd_hda_apply_verbs(codec); /* apply verbs here after own init */

	snd_hda_apply_fixup(codec, HDA_FIXUP_ACT_INIT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
=======
#define alc_free	snd_hda_gen_free

#ifdef CONFIG_PM
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline void alc_shutup(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;

<<<<<<< HEAD
	if (spec && spec->shutup)
		spec->shutup(codec);
	snd_hda_shutup_pins(codec);
}

static void alc_free_kctls(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;

	if (spec->kctls.list) {
		struct snd_kcontrol_new *kctl = spec->kctls.list;
		int i;
		for (i = 0; i < spec->kctls.used; i++)
			kfree(kctl[i].name);
	}
	snd_array_free(&spec->kctls);
}

static void alc_free_bind_ctls(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	if (spec->bind_ctls.list) {
		struct hda_bind_ctls **ctl = spec->bind_ctls.list;
		int i;
		for (i = 0; i < spec->bind_ctls.used; i++)
			kfree(ctl[i]);
	}
	snd_array_free(&spec->bind_ctls);
}

static void alc_free(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;

	if (!spec)
		return;

	alc_shutup(codec);
	alc_free_kctls(codec);
	alc_free_bind_ctls(codec);
	kfree(spec);
	snd_hda_detach_beep_device(codec);
}

#ifdef CONFIG_SND_HDA_POWER_SAVE
=======
	if (!snd_hda_get_bool_hint(codec, "shutup"))
		return; /* disabled explicitly by hints */

	if (spec && spec->shutup)
		spec->shutup(codec);
	else
		alc_shutup_pins(codec);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void alc_power_eapd(struct hda_codec *codec)
{
	alc_auto_setup_eapd(codec, false);
}

<<<<<<< HEAD
static int alc_suspend(struct hda_codec *codec, pm_message_t state)
=======
static int alc_suspend(struct hda_codec *codec)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct alc_spec *spec = codec->spec;
	alc_shutup(codec);
	if (spec && spec->power_hook)
		spec->power_hook(codec);
	return 0;
}
<<<<<<< HEAD
#endif

#ifdef CONFIG_PM
static int alc_resume(struct hda_codec *codec)
{
	msleep(150); /* to avoid pop noise */
	codec->patch_ops.init(codec);
	snd_hda_codec_resume_amp(codec);
	snd_hda_codec_resume_cache(codec);
=======

static int alc_resume(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;

	if (!spec->no_depop_delay)
		msleep(150); /* to avoid pop noise */
	codec->patch_ops.init(codec);
	snd_hda_regmap_sync(codec);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	hda_call_check_power_status(codec, 0x01);
	return 0;
}
#endif

/*
 */
static const struct hda_codec_ops alc_patch_ops = {
	.build_controls = alc_build_controls,
<<<<<<< HEAD
	.build_pcms = alc_build_pcms,
	.init = alc_init,
	.free = alc_free,
	.unsol_event = alc_unsol_event,
#ifdef CONFIG_PM
	.resume = alc_resume,
#endif
#ifdef CONFIG_SND_HDA_POWER_SAVE
	.suspend = alc_suspend,
	.check_power_status = alc_check_power_status,
#endif
	.reboot_notify = alc_shutup,
};

/* replace the codec chip_name with the given string */
static int alc_codec_rename(struct hda_codec *codec, const char *name)
{
	kfree(codec->chip_name);
	codec->chip_name = kstrdup(name, GFP_KERNEL);
	if (!codec->chip_name) {
		alc_free(codec);
		return -ENOMEM;
	}
	return 0;
}

/*
 * Rename codecs appropriately from COEF value
=======
	.build_pcms = snd_hda_gen_build_pcms,
	.init = alc_init,
	.free = alc_free,
	.unsol_event = snd_hda_jack_unsol_event,
#ifdef CONFIG_PM
	.resume = alc_resume,
	.suspend = alc_suspend,
	.check_power_status = snd_hda_gen_check_power_status,
#endif
};


#define alc_codec_rename(codec, name) snd_hda_codec_set_name(codec, name)

/*
 * Rename codecs appropriately from COEF value or subvendor id
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
struct alc_codec_rename_table {
	unsigned int vendor_id;
	unsigned short coef_mask;
	unsigned short coef_bits;
	const char *name;
};

<<<<<<< HEAD
static struct alc_codec_rename_table rename_tbl[] = {
=======
struct alc_codec_rename_pci_table {
	unsigned int codec_vendor_id;
	unsigned short pci_subvendor;
	unsigned short pci_subdevice;
	const char *name;
};

static const struct alc_codec_rename_table rename_tbl[] = {
	{ 0x10ec0221, 0xf00f, 0x1003, "ALC231" },
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{ 0x10ec0269, 0xfff0, 0x3010, "ALC277" },
	{ 0x10ec0269, 0xf0f0, 0x2010, "ALC259" },
	{ 0x10ec0269, 0xf0f0, 0x3010, "ALC258" },
	{ 0x10ec0269, 0x00f0, 0x0010, "ALC269VB" },
	{ 0x10ec0269, 0xffff, 0xa023, "ALC259" },
	{ 0x10ec0269, 0xffff, 0x6023, "ALC281X" },
	{ 0x10ec0269, 0x00f0, 0x0020, "ALC269VC" },
<<<<<<< HEAD
=======
	{ 0x10ec0269, 0x00f0, 0x0030, "ALC269VD" },
	{ 0x10ec0662, 0xffff, 0x4020, "ALC656" },
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{ 0x10ec0887, 0x00f0, 0x0030, "ALC887-VD" },
	{ 0x10ec0888, 0x00f0, 0x0030, "ALC888-VD" },
	{ 0x10ec0888, 0xf0f0, 0x3020, "ALC886" },
	{ 0x10ec0899, 0x2000, 0x2000, "ALC899" },
	{ 0x10ec0892, 0xffff, 0x8020, "ALC661" },
	{ 0x10ec0892, 0xffff, 0x8011, "ALC661" },
	{ 0x10ec0892, 0xffff, 0x4011, "ALC656" },
	{ } /* terminator */
};

<<<<<<< HEAD
static int alc_codec_rename_from_preset(struct hda_codec *codec)
{
	const struct alc_codec_rename_table *p;

	for (p = rename_tbl; p->vendor_id; p++) {
		if (p->vendor_id != codec->vendor_id)
=======
static const struct alc_codec_rename_pci_table rename_pci_tbl[] = {
	{ 0x10ec0280, 0x1028, 0, "ALC3220" },
	{ 0x10ec0282, 0x1028, 0, "ALC3221" },
	{ 0x10ec0283, 0x1028, 0, "ALC3223" },
	{ 0x10ec0288, 0x1028, 0, "ALC3263" },
	{ 0x10ec0292, 0x1028, 0, "ALC3226" },
	{ 0x10ec0293, 0x1028, 0, "ALC3235" },
	{ 0x10ec0255, 0x1028, 0, "ALC3234" },
	{ 0x10ec0668, 0x1028, 0, "ALC3661" },
	{ 0x10ec0275, 0x1028, 0, "ALC3260" },
	{ 0x10ec0899, 0x1028, 0, "ALC3861" },
	{ 0x10ec0298, 0x1028, 0, "ALC3266" },
	{ 0x10ec0236, 0x1028, 0, "ALC3204" },
	{ 0x10ec0256, 0x1028, 0, "ALC3246" },
	{ 0x10ec0225, 0x1028, 0, "ALC3253" },
	{ 0x10ec0295, 0x1028, 0, "ALC3254" },
	{ 0x10ec0299, 0x1028, 0, "ALC3271" },
	{ 0x10ec0670, 0x1025, 0, "ALC669X" },
	{ 0x10ec0676, 0x1025, 0, "ALC679X" },
	{ 0x10ec0282, 0x1043, 0, "ALC3229" },
	{ 0x10ec0233, 0x1043, 0, "ALC3236" },
	{ 0x10ec0280, 0x103c, 0, "ALC3228" },
	{ 0x10ec0282, 0x103c, 0, "ALC3227" },
	{ 0x10ec0286, 0x103c, 0, "ALC3242" },
	{ 0x10ec0290, 0x103c, 0, "ALC3241" },
	{ 0x10ec0668, 0x103c, 0, "ALC3662" },
	{ 0x10ec0283, 0x17aa, 0, "ALC3239" },
	{ 0x10ec0292, 0x17aa, 0, "ALC3232" },
	{ } /* terminator */
};

static int alc_codec_rename_from_preset(struct hda_codec *codec)
{
	const struct alc_codec_rename_table *p;
	const struct alc_codec_rename_pci_table *q;

	for (p = rename_tbl; p->vendor_id; p++) {
		if (p->vendor_id != codec->core.vendor_id)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			continue;
		if ((alc_get_coef0(codec) & p->coef_mask) == p->coef_bits)
			return alc_codec_rename(codec, p->name);
	}
<<<<<<< HEAD
	return 0;
}

/*
 * Automatic parse of I/O pins from the BIOS configuration
 */

enum {
	ALC_CTL_WIDGET_VOL,
	ALC_CTL_WIDGET_MUTE,
	ALC_CTL_BIND_MUTE,
	ALC_CTL_BIND_VOL,
	ALC_CTL_BIND_SW,
};
static const struct snd_kcontrol_new alc_control_templates[] = {
	HDA_CODEC_VOLUME(NULL, 0, 0, 0),
	HDA_CODEC_MUTE(NULL, 0, 0, 0),
	HDA_BIND_MUTE(NULL, 0, 0, 0),
	HDA_BIND_VOL(NULL, 0),
	HDA_BIND_SW(NULL, 0),
};

/* add dynamic controls */
static int add_control(struct alc_spec *spec, int type, const char *name,
		       int cidx, unsigned long val)
{
	struct snd_kcontrol_new *knew;

	knew = alc_kcontrol_new(spec);
	if (!knew)
		return -ENOMEM;
	*knew = alc_control_templates[type];
	knew->name = kstrdup(name, GFP_KERNEL);
	if (!knew->name)
		return -ENOMEM;
	knew->index = cidx;
	if (get_amp_nid_(val))
		knew->subdevice = HDA_SUBDEV_AMP_FLAG;
	knew->private_value = val;
	return 0;
}

static int add_control_with_pfx(struct alc_spec *spec, int type,
				const char *pfx, const char *dir,
				const char *sfx, int cidx, unsigned long val)
{
	char name[32];
	snprintf(name, sizeof(name), "%s %s %s", pfx, dir, sfx);
	return add_control(spec, type, name, cidx, val);
}

#define add_pb_vol_ctrl(spec, type, pfx, val)			\
	add_control_with_pfx(spec, type, pfx, "Playback", "Volume", 0, val)
#define add_pb_sw_ctrl(spec, type, pfx, val)			\
	add_control_with_pfx(spec, type, pfx, "Playback", "Switch", 0, val)
#define __add_pb_vol_ctrl(spec, type, pfx, cidx, val)			\
	add_control_with_pfx(spec, type, pfx, "Playback", "Volume", cidx, val)
#define __add_pb_sw_ctrl(spec, type, pfx, cidx, val)			\
	add_control_with_pfx(spec, type, pfx, "Playback", "Switch", cidx, val)

static const char * const channel_name[4] = {
	"Front", "Surround", "CLFE", "Side"
};

static const char *alc_get_line_out_pfx(struct alc_spec *spec, int ch,
					bool can_be_master, int *index)
{
	struct auto_pin_cfg *cfg = &spec->autocfg;

	*index = 0;
	if (cfg->line_outs == 1 && !spec->multi_ios &&
	    !cfg->hp_outs && !cfg->speaker_outs && can_be_master)
		return "Master";

	switch (cfg->line_out_type) {
	case AUTO_PIN_SPEAKER_OUT:
		if (cfg->line_outs == 1)
			return "Speaker";
		if (cfg->line_outs == 2)
			return ch ? "Bass Speaker" : "Speaker";
		break;
	case AUTO_PIN_HP_OUT:
		/* for multi-io case, only the primary out */
		if (ch && spec->multi_ios)
			break;
		*index = ch;
		return "Headphone";
	default:
		if (cfg->line_outs == 1 && !spec->multi_ios)
			return "PCM";
		break;
	}
	if (ch >= ARRAY_SIZE(channel_name)) {
		snd_BUG();
		return "PCM";
	}

	return channel_name[ch];
}

#ifdef CONFIG_SND_HDA_POWER_SAVE
/* add the powersave loopback-list entry */
static void add_loopback_list(struct alc_spec *spec, hda_nid_t mix, int idx)
{
	struct hda_amp_list *list;

	if (spec->num_loopbacks >= ARRAY_SIZE(spec->loopback_list) - 1)
		return;
	list = spec->loopback_list + spec->num_loopbacks;
	list->nid = mix;
	list->dir = HDA_INPUT;
	list->idx = idx;
	spec->num_loopbacks++;
	spec->loopback.amplist = spec->loopback_list;
}
#else
#define add_loopback_list(spec, mix, idx) /* NOP */
#endif

/* create input playback/capture controls for the given pin */
static int new_analog_input(struct alc_spec *spec, hda_nid_t pin,
			    const char *ctlname, int ctlidx,
			    int idx, hda_nid_t mix_nid)
{
	int err;

	err = __add_pb_vol_ctrl(spec, ALC_CTL_WIDGET_VOL, ctlname, ctlidx,
			  HDA_COMPOSE_AMP_VAL(mix_nid, 3, idx, HDA_INPUT));
	if (err < 0)
		return err;
	err = __add_pb_sw_ctrl(spec, ALC_CTL_WIDGET_MUTE, ctlname, ctlidx,
			  HDA_COMPOSE_AMP_VAL(mix_nid, 3, idx, HDA_INPUT));
	if (err < 0)
		return err;
	add_loopback_list(spec, mix_nid, idx);
	return 0;
}

static int alc_is_input_pin(struct hda_codec *codec, hda_nid_t nid)
{
	unsigned int pincap = snd_hda_query_pin_caps(codec, nid);
	return (pincap & AC_PINCAP_IN) != 0;
}

/* Parse the codec tree and retrieve ADCs and corresponding capsrc MUXs */
static int alc_auto_fill_adc_caps(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	hda_nid_t nid;
	hda_nid_t *adc_nids = spec->private_adc_nids;
	hda_nid_t *cap_nids = spec->private_capsrc_nids;
	int max_nums = ARRAY_SIZE(spec->private_adc_nids);
	int i, nums = 0;

	nid = codec->start_nid;
	for (i = 0; i < codec->num_nodes; i++, nid++) {
		hda_nid_t src;
		const hda_nid_t *list;
		unsigned int caps = get_wcaps(codec, nid);
		int type = get_wcaps_type(caps);

		if (type != AC_WID_AUD_IN || (caps & AC_WCAP_DIGITAL))
			continue;
		adc_nids[nums] = nid;
		cap_nids[nums] = nid;
		src = nid;
		for (;;) {
			int n;
			type = get_wcaps_type(get_wcaps(codec, src));
			if (type == AC_WID_PIN)
				break;
			if (type == AC_WID_AUD_SEL) {
				cap_nids[nums] = src;
				break;
			}
			n = snd_hda_get_conn_list(codec, src, &list);
			if (n > 1) {
				cap_nids[nums] = src;
				break;
			} else if (n != 1)
				break;
			src = *list;
		}
		if (++nums >= max_nums)
			break;
	}
	spec->adc_nids = spec->private_adc_nids;
	spec->capsrc_nids = spec->private_capsrc_nids;
	spec->num_adc_nids = nums;
	return nums;
}

/* create playback/capture controls for input pins */
static int alc_auto_create_input_ctls(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	const struct auto_pin_cfg *cfg = &spec->autocfg;
	hda_nid_t mixer = spec->mixer_nid;
	struct hda_input_mux *imux = &spec->private_imux[0];
	int num_adcs;
	int i, c, err, idx, type_idx = 0;
	const char *prev_label = NULL;

	num_adcs = alc_auto_fill_adc_caps(codec);
	if (num_adcs < 0)
		return 0;

	for (i = 0; i < cfg->num_inputs; i++) {
		hda_nid_t pin;
		const char *label;

		pin = cfg->inputs[i].pin;
		if (!alc_is_input_pin(codec, pin))
			continue;

		label = hda_get_autocfg_input_label(codec, cfg, i);
		if (spec->shared_mic_hp && !strcmp(label, "Misc"))
			label = "Headphone Mic";
		if (prev_label && !strcmp(label, prev_label))
			type_idx++;
		else
			type_idx = 0;
		prev_label = label;

		if (mixer) {
			idx = get_connection_index(codec, mixer, pin);
			if (idx >= 0) {
				err = new_analog_input(spec, pin,
						       label, type_idx,
						       idx, mixer);
				if (err < 0)
					return err;
			}
		}

		for (c = 0; c < num_adcs; c++) {
			hda_nid_t cap = get_capsrc(spec, c);
			idx = get_connection_index(codec, cap, pin);
			if (idx >= 0) {
				spec->imux_pins[imux->num_items] = pin;
				snd_hda_add_imux_item(imux, label, idx, NULL);
				break;
			}
		}
	}

	spec->num_mux_defs = 1;
	spec->input_mux = imux;

	return 0;
}

/* create a shared input with the headphone out */
static int alc_auto_create_shared_input(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	struct auto_pin_cfg *cfg = &spec->autocfg;
	unsigned int defcfg;
	hda_nid_t nid;

	/* only one internal input pin? */
	if (cfg->num_inputs != 1)
		return 0;
	defcfg = snd_hda_codec_get_pincfg(codec, cfg->inputs[0].pin);
	if (snd_hda_get_input_pin_attr(defcfg) != INPUT_PIN_ATTR_INT)
		return 0;

	if (cfg->hp_outs == 1 && cfg->line_out_type == AUTO_PIN_SPEAKER_OUT)
		nid = cfg->hp_pins[0]; /* OK, we have a single HP-out */
	else if (cfg->line_outs == 1 && cfg->line_out_type == AUTO_PIN_HP_OUT)
		nid = cfg->line_out_pins[0]; /* OK, we have a single line-out */
	else
		return 0; /* both not available */

	if (!(snd_hda_query_pin_caps(codec, nid) & AC_PINCAP_IN))
		return 0; /* no input */

	cfg->inputs[1].pin = nid;
	cfg->inputs[1].type = AUTO_PIN_MIC;
	cfg->num_inputs = 2;
	spec->shared_mic_hp = 1;
	snd_printdd("realtek: Enable shared I/O jack on NID 0x%x\n", nid);
	return 0;
}

static void alc_set_pin_output(struct hda_codec *codec, hda_nid_t nid,
			       unsigned int pin_type)
{
	snd_hda_codec_write(codec, nid, 0, AC_VERB_SET_PIN_WIDGET_CONTROL,
			    pin_type);
	/* unmute pin */
	if (nid_has_mute(codec, nid, HDA_OUTPUT))
		snd_hda_codec_write(codec, nid, 0, AC_VERB_SET_AMP_GAIN_MUTE,
			    AMP_OUT_UNMUTE);
}

static int get_pin_type(int line_out_type)
{
	if (line_out_type == AUTO_PIN_HP_OUT)
		return PIN_HP;
	else
		return PIN_OUT;
}

static void alc_auto_init_analog_input(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	struct auto_pin_cfg *cfg = &spec->autocfg;
	int i;

	for (i = 0; i < cfg->num_inputs; i++) {
		hda_nid_t nid = cfg->inputs[i].pin;
		if (alc_is_input_pin(codec, nid)) {
			alc_set_input_pin(codec, nid, cfg->inputs[i].type);
			if (get_wcaps(codec, nid) & AC_WCAP_OUT_AMP)
				snd_hda_codec_write(codec, nid, 0,
						    AC_VERB_SET_AMP_GAIN_MUTE,
						    AMP_OUT_MUTE);
		}
	}

	/* mute all loopback inputs */
	if (spec->mixer_nid) {
		int nums = snd_hda_get_conn_list(codec, spec->mixer_nid, NULL);
		for (i = 0; i < nums; i++)
			snd_hda_codec_write(codec, spec->mixer_nid, 0,
					    AC_VERB_SET_AMP_GAIN_MUTE,
					    AMP_IN_MUTE(i));
	}
}

/* convert from MIX nid to DAC */
static hda_nid_t alc_auto_mix_to_dac(struct hda_codec *codec, hda_nid_t nid)
{
	hda_nid_t list[5];
	int i, num;

	if (get_wcaps_type(get_wcaps(codec, nid)) == AC_WID_AUD_OUT)
		return nid;
	num = snd_hda_get_connections(codec, nid, list, ARRAY_SIZE(list));
	for (i = 0; i < num; i++) {
		if (get_wcaps_type(get_wcaps(codec, list[i])) == AC_WID_AUD_OUT)
			return list[i];
	}
	return 0;
}

/* go down to the selector widget before the mixer */
static hda_nid_t alc_go_down_to_selector(struct hda_codec *codec, hda_nid_t pin)
{
	hda_nid_t srcs[5];
	int num = snd_hda_get_connections(codec, pin, srcs,
					  ARRAY_SIZE(srcs));
	if (num != 1 ||
	    get_wcaps_type(get_wcaps(codec, srcs[0])) != AC_WID_AUD_SEL)
		return pin;
	return srcs[0];
}

/* get MIX nid connected to the given pin targeted to DAC */
static hda_nid_t alc_auto_dac_to_mix(struct hda_codec *codec, hda_nid_t pin,
				   hda_nid_t dac)
{
	hda_nid_t mix[5];
	int i, num;

	pin = alc_go_down_to_selector(codec, pin);
	num = snd_hda_get_connections(codec, pin, mix, ARRAY_SIZE(mix));
	for (i = 0; i < num; i++) {
		if (alc_auto_mix_to_dac(codec, mix[i]) == dac)
			return mix[i];
	}
	return 0;
}

/* select the connection from pin to DAC if needed */
static int alc_auto_select_dac(struct hda_codec *codec, hda_nid_t pin,
			       hda_nid_t dac)
{
	hda_nid_t mix[5];
	int i, num;

	pin = alc_go_down_to_selector(codec, pin);
	num = snd_hda_get_connections(codec, pin, mix, ARRAY_SIZE(mix));
	if (num < 2)
		return 0;
	for (i = 0; i < num; i++) {
		if (alc_auto_mix_to_dac(codec, mix[i]) == dac) {
			snd_hda_codec_update_cache(codec, pin, 0,
						   AC_VERB_SET_CONNECT_SEL, i);
			return 0;
		}
	}
	return 0;
}

static bool alc_is_dac_already_used(struct hda_codec *codec, hda_nid_t nid)
{
	struct alc_spec *spec = codec->spec;
	int i;
	if (found_in_nid_list(nid, spec->multiout.dac_nids,
			      ARRAY_SIZE(spec->private_dac_nids)) ||
	    found_in_nid_list(nid, spec->multiout.hp_out_nid,
			      ARRAY_SIZE(spec->multiout.hp_out_nid)) ||
	    found_in_nid_list(nid, spec->multiout.extra_out_nid,
			      ARRAY_SIZE(spec->multiout.extra_out_nid)))
		return true;
	for (i = 0; i < spec->multi_ios; i++) {
		if (spec->multi_io[i].dac == nid)
			return true;
	}
	return false;
}

/* look for an empty DAC slot */
static hda_nid_t alc_auto_look_for_dac(struct hda_codec *codec, hda_nid_t pin)
{
	hda_nid_t srcs[5];
	int i, num;

	pin = alc_go_down_to_selector(codec, pin);
	num = snd_hda_get_connections(codec, pin, srcs, ARRAY_SIZE(srcs));
	for (i = 0; i < num; i++) {
		hda_nid_t nid = alc_auto_mix_to_dac(codec, srcs[i]);
		if (!nid)
			continue;
		if (!alc_is_dac_already_used(codec, nid))
			return nid;
	}
	return 0;
}

/* check whether the DAC is reachable from the pin */
static bool alc_auto_is_dac_reachable(struct hda_codec *codec,
				      hda_nid_t pin, hda_nid_t dac)
{
	hda_nid_t srcs[5];
	int i, num;

	if (!pin || !dac)
		return false;
	pin = alc_go_down_to_selector(codec, pin);
	num = snd_hda_get_connections(codec, pin, srcs, ARRAY_SIZE(srcs));
	for (i = 0; i < num; i++) {
		hda_nid_t nid = alc_auto_mix_to_dac(codec, srcs[i]);
		if (nid == dac)
			return true;
	}
	return false;
}

static hda_nid_t get_dac_if_single(struct hda_codec *codec, hda_nid_t pin)
{
	struct alc_spec *spec = codec->spec;
	hda_nid_t sel = alc_go_down_to_selector(codec, pin);
	hda_nid_t nid, nid_found, srcs[5];
	int i, num = snd_hda_get_connections(codec, sel, srcs,
					  ARRAY_SIZE(srcs));
	if (num == 1)
		return alc_auto_look_for_dac(codec, pin);
	nid_found = 0;
	for (i = 0; i < num; i++) {
		if (srcs[i] == spec->mixer_nid)
			continue;
		nid = alc_auto_mix_to_dac(codec, srcs[i]);
		if (nid && !alc_is_dac_already_used(codec, nid)) {
			if (nid_found)
				return 0;
			nid_found = nid;
		}
	}
	return nid_found;
}

/* mark up volume and mute control NIDs: used during badness parsing and
 * at creating actual controls
 */
static inline unsigned int get_ctl_pos(unsigned int data)
{
	hda_nid_t nid = get_amp_nid_(data);
	unsigned int dir;
	if (snd_BUG_ON(nid >= MAX_VOL_NIDS))
		return 0;
	dir = get_amp_direction_(data);
	return (nid << 1) | dir;
}

#define is_ctl_used(bits, data) \
	test_bit(get_ctl_pos(data), bits)
#define mark_ctl_usage(bits, data) \
	set_bit(get_ctl_pos(data), bits)

static void clear_vol_marks(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	memset(spec->vol_ctls, 0, sizeof(spec->vol_ctls));
	memset(spec->sw_ctls, 0, sizeof(spec->sw_ctls));
}

/* badness definition */
enum {
	/* No primary DAC is found for the main output */
	BAD_NO_PRIMARY_DAC = 0x10000,
	/* No DAC is found for the extra output */
	BAD_NO_DAC = 0x4000,
	/* No possible multi-ios */
	BAD_MULTI_IO = 0x103,
	/* No individual DAC for extra output */
	BAD_NO_EXTRA_DAC = 0x102,
	/* No individual DAC for extra surrounds */
	BAD_NO_EXTRA_SURR_DAC = 0x101,
	/* Primary DAC shared with main surrounds */
	BAD_SHARED_SURROUND = 0x100,
	/* Primary DAC shared with main CLFE */
	BAD_SHARED_CLFE = 0x10,
	/* Primary DAC shared with extra surrounds */
	BAD_SHARED_EXTRA_SURROUND = 0x10,
	/* Volume widget is shared */
	BAD_SHARED_VOL = 0x10,
};

static hda_nid_t alc_look_for_out_mute_nid(struct hda_codec *codec,
					   hda_nid_t pin, hda_nid_t dac);
static hda_nid_t alc_look_for_out_vol_nid(struct hda_codec *codec,
					  hda_nid_t pin, hda_nid_t dac);

static int eval_shared_vol_badness(struct hda_codec *codec, hda_nid_t pin,
				   hda_nid_t dac)
{
	struct alc_spec *spec = codec->spec;
	hda_nid_t nid;
	unsigned int val;
	int badness = 0;

	nid = alc_look_for_out_vol_nid(codec, pin, dac);
	if (nid) {
		val = HDA_COMPOSE_AMP_VAL(nid, 3, 0, HDA_OUTPUT);
		if (is_ctl_used(spec->vol_ctls, nid))
			badness += BAD_SHARED_VOL;
		else
			mark_ctl_usage(spec->vol_ctls, val);
	} else
		badness += BAD_SHARED_VOL;
	nid = alc_look_for_out_mute_nid(codec, pin, dac);
	if (nid) {
		unsigned int wid_type = get_wcaps_type(get_wcaps(codec, nid));
		if (wid_type == AC_WID_PIN || wid_type == AC_WID_AUD_OUT)
			val = HDA_COMPOSE_AMP_VAL(nid, 3, 0, HDA_OUTPUT);
		else
			val = HDA_COMPOSE_AMP_VAL(nid, 3, 0, HDA_INPUT);
		if (is_ctl_used(spec->sw_ctls, val))
			badness += BAD_SHARED_VOL;
		else
			mark_ctl_usage(spec->sw_ctls, val);
	} else
		badness += BAD_SHARED_VOL;
	return badness;
}

struct badness_table {
	int no_primary_dac;	/* no primary DAC */
	int no_dac;		/* no secondary DACs */
	int shared_primary;	/* primary DAC is shared with main output */
	int shared_surr;	/* secondary DAC shared with main or primary */
	int shared_clfe;	/* third DAC shared with main or primary */
	int shared_surr_main;	/* secondary DAC sahred with main/DAC0 */
};

static struct badness_table main_out_badness = {
	.no_primary_dac = BAD_NO_PRIMARY_DAC,
	.no_dac = BAD_NO_DAC,
	.shared_primary = BAD_NO_PRIMARY_DAC,
	.shared_surr = BAD_SHARED_SURROUND,
	.shared_clfe = BAD_SHARED_CLFE,
	.shared_surr_main = BAD_SHARED_SURROUND,
};

static struct badness_table extra_out_badness = {
	.no_primary_dac = BAD_NO_DAC,
	.no_dac = BAD_NO_DAC,
	.shared_primary = BAD_NO_EXTRA_DAC,
	.shared_surr = BAD_SHARED_EXTRA_SURROUND,
	.shared_clfe = BAD_SHARED_EXTRA_SURROUND,
	.shared_surr_main = BAD_NO_EXTRA_SURR_DAC,
};

/* try to assign DACs to pins and return the resultant badness */
static int alc_auto_fill_dacs(struct hda_codec *codec, int num_outs,
			      const hda_nid_t *pins, hda_nid_t *dacs,
			      const struct badness_table *bad)
{
	struct alc_spec *spec = codec->spec;
	struct auto_pin_cfg *cfg = &spec->autocfg;
	int i, j;
	int badness = 0;
	hda_nid_t dac;

	if (!num_outs)
		return 0;

	for (i = 0; i < num_outs; i++) {
		hda_nid_t pin = pins[i];
		if (!dacs[i])
			dacs[i] = alc_auto_look_for_dac(codec, pin);
		if (!dacs[i] && !i) {
			for (j = 1; j < num_outs; j++) {
				if (alc_auto_is_dac_reachable(codec, pin, dacs[j])) {
					dacs[0] = dacs[j];
					dacs[j] = 0;
					break;
				}
			}
		}
		dac = dacs[i];
		if (!dac) {
			if (alc_auto_is_dac_reachable(codec, pin, dacs[0]))
				dac = dacs[0];
			else if (cfg->line_outs > i &&
				 alc_auto_is_dac_reachable(codec, pin,
					spec->private_dac_nids[i]))
				dac = spec->private_dac_nids[i];
			if (dac) {
				if (!i)
					badness += bad->shared_primary;
				else if (i == 1)
					badness += bad->shared_surr;
				else
					badness += bad->shared_clfe;
			} else if (alc_auto_is_dac_reachable(codec, pin,
					spec->private_dac_nids[0])) {
				dac = spec->private_dac_nids[0];
				badness += bad->shared_surr_main;
			} else if (!i)
				badness += bad->no_primary_dac;
			else
				badness += bad->no_dac;
		}
		if (dac)
			badness += eval_shared_vol_badness(codec, pin, dac);
	}

	return badness;
}

static int alc_auto_fill_multi_ios(struct hda_codec *codec,
				   hda_nid_t reference_pin,
				   bool hardwired, int offset);

static bool alc_map_singles(struct hda_codec *codec, int outs,
			    const hda_nid_t *pins, hda_nid_t *dacs)
{
	int i;
	bool found = false;
	for (i = 0; i < outs; i++) {
		if (dacs[i])
			continue;
		dacs[i] = get_dac_if_single(codec, pins[i]);
		if (dacs[i])
			found = true;
	}
	return found;
}

/* fill in the dac_nids table from the parsed pin configuration */
static int fill_and_eval_dacs(struct hda_codec *codec,
			      bool fill_hardwired,
			      bool fill_mio_first)
{
	struct alc_spec *spec = codec->spec;
	struct auto_pin_cfg *cfg = &spec->autocfg;
	int i, err, badness;

	/* set num_dacs once to full for alc_auto_look_for_dac() */
	spec->multiout.num_dacs = cfg->line_outs;
	spec->multiout.dac_nids = spec->private_dac_nids;
	memset(spec->private_dac_nids, 0, sizeof(spec->private_dac_nids));
	memset(spec->multiout.hp_out_nid, 0, sizeof(spec->multiout.hp_out_nid));
	memset(spec->multiout.extra_out_nid, 0, sizeof(spec->multiout.extra_out_nid));
	spec->multi_ios = 0;
	clear_vol_marks(codec);
	badness = 0;

	/* fill hard-wired DACs first */
	if (fill_hardwired) {
		bool mapped;
		do {
			mapped = alc_map_singles(codec, cfg->line_outs,
						 cfg->line_out_pins,
						 spec->private_dac_nids);
			mapped |= alc_map_singles(codec, cfg->hp_outs,
						  cfg->hp_pins,
						  spec->multiout.hp_out_nid);
			mapped |= alc_map_singles(codec, cfg->speaker_outs,
						  cfg->speaker_pins,
						  spec->multiout.extra_out_nid);
			if (fill_mio_first && cfg->line_outs == 1 &&
			    cfg->line_out_type != AUTO_PIN_SPEAKER_OUT) {
				err = alc_auto_fill_multi_ios(codec, cfg->line_out_pins[0], true, 0);
				if (!err)
					mapped = true;
			}
		} while (mapped);
	}

	badness += alc_auto_fill_dacs(codec, cfg->line_outs, cfg->line_out_pins,
				      spec->private_dac_nids,
				      &main_out_badness);

	/* re-count num_dacs and squash invalid entries */
	spec->multiout.num_dacs = 0;
	for (i = 0; i < cfg->line_outs; i++) {
		if (spec->private_dac_nids[i])
			spec->multiout.num_dacs++;
		else {
			memmove(spec->private_dac_nids + i,
				spec->private_dac_nids + i + 1,
				sizeof(hda_nid_t) * (cfg->line_outs - i - 1));
			spec->private_dac_nids[cfg->line_outs - 1] = 0;
		}
	}

	if (fill_mio_first &&
	    cfg->line_outs == 1 && cfg->line_out_type != AUTO_PIN_SPEAKER_OUT) {
		/* try to fill multi-io first */
		err = alc_auto_fill_multi_ios(codec, cfg->line_out_pins[0], false, 0);
		if (err < 0)
			return err;
		/* we don't count badness at this stage yet */
	}

	if (cfg->line_out_type != AUTO_PIN_HP_OUT) {
		err = alc_auto_fill_dacs(codec, cfg->hp_outs, cfg->hp_pins,
					 spec->multiout.hp_out_nid,
					 &extra_out_badness);
		if (err < 0)
			return err;
		badness += err;
	}
	if (cfg->line_out_type != AUTO_PIN_SPEAKER_OUT) {
		err = alc_auto_fill_dacs(codec, cfg->speaker_outs,
					 cfg->speaker_pins,
					 spec->multiout.extra_out_nid,
					 &extra_out_badness);
		if (err < 0)
			return err;
		badness += err;
	}
	if (cfg->line_outs == 1 && cfg->line_out_type != AUTO_PIN_SPEAKER_OUT) {
		err = alc_auto_fill_multi_ios(codec, cfg->line_out_pins[0], false, 0);
		if (err < 0)
			return err;
		badness += err;
	}
	if (cfg->hp_outs && cfg->line_out_type == AUTO_PIN_SPEAKER_OUT) {
		/* try multi-ios with HP + inputs */
		int offset = 0;
		if (cfg->line_outs >= 3)
			offset = 1;
		err = alc_auto_fill_multi_ios(codec, cfg->hp_pins[0], false,
					      offset);
		if (err < 0)
			return err;
		badness += err;
	}

	if (spec->multi_ios == 2) {
		for (i = 0; i < 2; i++)
			spec->private_dac_nids[spec->multiout.num_dacs++] =
				spec->multi_io[i].dac;
		spec->ext_channel_count = 2;
	} else if (spec->multi_ios) {
		spec->multi_ios = 0;
		badness += BAD_MULTI_IO;
	}

	return badness;
}

#define DEBUG_BADNESS

#ifdef DEBUG_BADNESS
#define debug_badness	snd_printdd
#else
#define debug_badness(...)
#endif

static void debug_show_configs(struct alc_spec *spec, struct auto_pin_cfg *cfg)
{
	debug_badness("multi_outs = %x/%x/%x/%x : %x/%x/%x/%x\n",
		      cfg->line_out_pins[0], cfg->line_out_pins[1],
		      cfg->line_out_pins[2], cfg->line_out_pins[2],
		      spec->multiout.dac_nids[0],
		      spec->multiout.dac_nids[1],
		      spec->multiout.dac_nids[2],
		      spec->multiout.dac_nids[3]);
	if (spec->multi_ios > 0)
		debug_badness("multi_ios(%d) = %x/%x : %x/%x\n",
			      spec->multi_ios,
			      spec->multi_io[0].pin, spec->multi_io[1].pin,
			      spec->multi_io[0].dac, spec->multi_io[1].dac);
	debug_badness("hp_outs = %x/%x/%x/%x : %x/%x/%x/%x\n",
		      cfg->hp_pins[0], cfg->hp_pins[1],
		      cfg->hp_pins[2], cfg->hp_pins[2],
		      spec->multiout.hp_out_nid[0],
		      spec->multiout.hp_out_nid[1],
		      spec->multiout.hp_out_nid[2],
		      spec->multiout.hp_out_nid[3]);
	debug_badness("spk_outs = %x/%x/%x/%x : %x/%x/%x/%x\n",
		      cfg->speaker_pins[0], cfg->speaker_pins[1],
		      cfg->speaker_pins[2], cfg->speaker_pins[3],
		      spec->multiout.extra_out_nid[0],
		      spec->multiout.extra_out_nid[1],
		      spec->multiout.extra_out_nid[2],
		      spec->multiout.extra_out_nid[3]);
}

static int alc_auto_fill_dac_nids(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	struct auto_pin_cfg *cfg = &spec->autocfg;
	struct auto_pin_cfg *best_cfg;
	int best_badness = INT_MAX;
	int badness;
	bool fill_hardwired = true, fill_mio_first = true;
	bool best_wired = true, best_mio = true;
	bool hp_spk_swapped = false;

	best_cfg = kmalloc(sizeof(*best_cfg), GFP_KERNEL);
	if (!best_cfg)
		return -ENOMEM;
	*best_cfg = *cfg;

	for (;;) {
		badness = fill_and_eval_dacs(codec, fill_hardwired,
					     fill_mio_first);
		if (badness < 0) {
			kfree(best_cfg);
			return badness;
		}
		debug_badness("==> lo_type=%d, wired=%d, mio=%d, badness=0x%x\n",
			      cfg->line_out_type, fill_hardwired, fill_mio_first,
			      badness);
		debug_show_configs(spec, cfg);
		if (badness < best_badness) {
			best_badness = badness;
			*best_cfg = *cfg;
			best_wired = fill_hardwired;
			best_mio = fill_mio_first;
		}
		if (!badness)
			break;
		fill_mio_first = !fill_mio_first;
		if (!fill_mio_first)
			continue;
		fill_hardwired = !fill_hardwired;
		if (!fill_hardwired)
			continue;
		if (hp_spk_swapped)
			break;
		hp_spk_swapped = true;
		if (cfg->speaker_outs > 0 &&
		    cfg->line_out_type == AUTO_PIN_HP_OUT) {
			cfg->hp_outs = cfg->line_outs;
			memcpy(cfg->hp_pins, cfg->line_out_pins,
			       sizeof(cfg->hp_pins));
			cfg->line_outs = cfg->speaker_outs;
			memcpy(cfg->line_out_pins, cfg->speaker_pins,
			       sizeof(cfg->speaker_pins));
			cfg->speaker_outs = 0;
			memset(cfg->speaker_pins, 0, sizeof(cfg->speaker_pins));
			cfg->line_out_type = AUTO_PIN_SPEAKER_OUT;
			fill_hardwired = true;
			continue;
		}
		if (cfg->hp_outs > 0 &&
		    cfg->line_out_type == AUTO_PIN_SPEAKER_OUT) {
			cfg->speaker_outs = cfg->line_outs;
			memcpy(cfg->speaker_pins, cfg->line_out_pins,
			       sizeof(cfg->speaker_pins));
			cfg->line_outs = cfg->hp_outs;
			memcpy(cfg->line_out_pins, cfg->hp_pins,
			       sizeof(cfg->hp_pins));
			cfg->hp_outs = 0;
			memset(cfg->hp_pins, 0, sizeof(cfg->hp_pins));
			cfg->line_out_type = AUTO_PIN_HP_OUT;
			fill_hardwired = true;
			continue;
		}
		break;
	}

	if (badness) {
		*cfg = *best_cfg;
		fill_and_eval_dacs(codec, best_wired, best_mio);
	}
	debug_badness("==> Best config: lo_type=%d, wired=%d, mio=%d\n",
		      cfg->line_out_type, best_wired, best_mio);
	debug_show_configs(spec, cfg);

	if (cfg->line_out_pins[0])
		spec->vmaster_nid =
			alc_look_for_out_vol_nid(codec, cfg->line_out_pins[0],
						 spec->multiout.dac_nids[0]);

	/* clear the bitmap flags for creating controls */
	clear_vol_marks(codec);
	kfree(best_cfg);
	return 0;
}

static int alc_auto_add_vol_ctl(struct hda_codec *codec,
			      const char *pfx, int cidx,
			      hda_nid_t nid, unsigned int chs)
{
	struct alc_spec *spec = codec->spec;
	unsigned int val;
	if (!nid)
		return 0;
	val = HDA_COMPOSE_AMP_VAL(nid, chs, 0, HDA_OUTPUT);
	if (is_ctl_used(spec->vol_ctls, val) && chs != 2) /* exclude LFE */
		return 0;
	mark_ctl_usage(spec->vol_ctls, val);
	return __add_pb_vol_ctrl(codec->spec, ALC_CTL_WIDGET_VOL, pfx, cidx,
				 val);
}

static int alc_auto_add_stereo_vol(struct hda_codec *codec,
				   const char *pfx, int cidx,
				   hda_nid_t nid)
{
	int chs = 1;
	if (get_wcaps(codec, nid) & AC_WCAP_STEREO)
		chs = 3;
	return alc_auto_add_vol_ctl(codec, pfx, cidx, nid, chs);
}

/* create a mute-switch for the given mixer widget;
 * if it has multiple sources (e.g. DAC and loopback), create a bind-mute
 */
static int alc_auto_add_sw_ctl(struct hda_codec *codec,
			     const char *pfx, int cidx,
			     hda_nid_t nid, unsigned int chs)
{
	struct alc_spec *spec = codec->spec;
	int wid_type;
	int type;
	unsigned long val;
	if (!nid)
		return 0;
	wid_type = get_wcaps_type(get_wcaps(codec, nid));
	if (wid_type == AC_WID_PIN || wid_type == AC_WID_AUD_OUT) {
		type = ALC_CTL_WIDGET_MUTE;
		val = HDA_COMPOSE_AMP_VAL(nid, chs, 0, HDA_OUTPUT);
	} else if (snd_hda_get_conn_list(codec, nid, NULL) == 1) {
		type = ALC_CTL_WIDGET_MUTE;
		val = HDA_COMPOSE_AMP_VAL(nid, chs, 0, HDA_INPUT);
	} else {
		type = ALC_CTL_BIND_MUTE;
		val = HDA_COMPOSE_AMP_VAL(nid, chs, 2, HDA_INPUT);
	}
	if (is_ctl_used(spec->sw_ctls, val) && chs != 2) /* exclude LFE */
		return 0;
	mark_ctl_usage(spec->sw_ctls, val);
	return __add_pb_sw_ctrl(codec->spec, type, pfx, cidx, val);
}

static int alc_auto_add_stereo_sw(struct hda_codec *codec, const char *pfx,
				  int cidx, hda_nid_t nid)
{
	int chs = 1;
	if (get_wcaps(codec, nid) & AC_WCAP_STEREO)
		chs = 3;
	return alc_auto_add_sw_ctl(codec, pfx, cidx, nid, chs);
}

static hda_nid_t alc_look_for_out_mute_nid(struct hda_codec *codec,
					   hda_nid_t pin, hda_nid_t dac)
{
	hda_nid_t mix = alc_auto_dac_to_mix(codec, pin, dac);
	if (nid_has_mute(codec, pin, HDA_OUTPUT))
		return pin;
	else if (mix && nid_has_mute(codec, mix, HDA_INPUT))
		return mix;
	else if (nid_has_mute(codec, dac, HDA_OUTPUT))
		return dac;
	return 0;
}

static hda_nid_t alc_look_for_out_vol_nid(struct hda_codec *codec,
					  hda_nid_t pin, hda_nid_t dac)
{
	hda_nid_t mix = alc_auto_dac_to_mix(codec, pin, dac);
	if (nid_has_volume(codec, dac, HDA_OUTPUT))
		return dac;
	else if (nid_has_volume(codec, mix, HDA_OUTPUT))
		return mix;
	else if (nid_has_volume(codec, pin, HDA_OUTPUT))
		return pin;
	return 0;
}

/* add playback controls from the parsed DAC table */
static int alc_auto_create_multi_out_ctls(struct hda_codec *codec,
					     const struct auto_pin_cfg *cfg)
{
	struct alc_spec *spec = codec->spec;
	int i, err, noutputs;

	noutputs = cfg->line_outs;
	if (spec->multi_ios > 0 && cfg->line_outs < 3)
		noutputs += spec->multi_ios;

	for (i = 0; i < noutputs; i++) {
		const char *name;
		int index;
		hda_nid_t dac, pin;
		hda_nid_t sw, vol;

		dac = spec->multiout.dac_nids[i];
		if (!dac)
			continue;
		if (i >= cfg->line_outs) {
			pin = spec->multi_io[i - 1].pin;
			index = 0;
			name = channel_name[i];
		} else {
			pin = cfg->line_out_pins[i];
			name = alc_get_line_out_pfx(spec, i, true, &index);
		}

		sw = alc_look_for_out_mute_nid(codec, pin, dac);
		vol = alc_look_for_out_vol_nid(codec, pin, dac);
		if (!name || !strcmp(name, "CLFE")) {
			/* Center/LFE */
			err = alc_auto_add_vol_ctl(codec, "Center", 0, vol, 1);
			if (err < 0)
				return err;
			err = alc_auto_add_vol_ctl(codec, "LFE", 0, vol, 2);
			if (err < 0)
				return err;
			err = alc_auto_add_sw_ctl(codec, "Center", 0, sw, 1);
			if (err < 0)
				return err;
			err = alc_auto_add_sw_ctl(codec, "LFE", 0, sw, 2);
			if (err < 0)
				return err;
		} else {
			err = alc_auto_add_stereo_vol(codec, name, index, vol);
			if (err < 0)
				return err;
			err = alc_auto_add_stereo_sw(codec, name, index, sw);
			if (err < 0)
				return err;
		}
	}
	return 0;
}

static int alc_auto_create_extra_out(struct hda_codec *codec, hda_nid_t pin,
				     hda_nid_t dac, const char *pfx,
				     int cidx)
{
	struct alc_spec *spec = codec->spec;
	hda_nid_t sw, vol;
	int err;

	if (!dac) {
		unsigned int val;
		/* the corresponding DAC is already occupied */
		if (!(get_wcaps(codec, pin) & AC_WCAP_OUT_AMP))
			return 0; /* no way */
		/* create a switch only */
		val = HDA_COMPOSE_AMP_VAL(pin, 3, 0, HDA_OUTPUT);
		if (is_ctl_used(spec->sw_ctls, val))
			return 0; /* already created */
		mark_ctl_usage(spec->sw_ctls, val);
		return __add_pb_sw_ctrl(spec, ALC_CTL_WIDGET_MUTE, pfx, cidx, val);
	}

	sw = alc_look_for_out_mute_nid(codec, pin, dac);
	vol = alc_look_for_out_vol_nid(codec, pin, dac);
	err = alc_auto_add_stereo_vol(codec, pfx, cidx, vol);
	if (err < 0)
		return err;
	err = alc_auto_add_stereo_sw(codec, pfx, cidx, sw);
	if (err < 0)
		return err;
	return 0;
}

static struct hda_bind_ctls *new_bind_ctl(struct hda_codec *codec,
					  unsigned int nums,
					  struct hda_ctl_ops *ops)
{
	struct alc_spec *spec = codec->spec;
	struct hda_bind_ctls **ctlp, *ctl;
	snd_array_init(&spec->bind_ctls, sizeof(ctl), 8);
	ctlp = snd_array_new(&spec->bind_ctls);
	if (!ctlp)
		return NULL;
	ctl = kzalloc(sizeof(*ctl) + sizeof(long) * (nums + 1), GFP_KERNEL);
	*ctlp = ctl;
	if (ctl)
		ctl->ops = ops;
	return ctl;
}

/* add playback controls for speaker and HP outputs */
static int alc_auto_create_extra_outs(struct hda_codec *codec, int num_pins,
				      const hda_nid_t *pins,
				      const hda_nid_t *dacs,
				      const char *pfx)
{
	struct alc_spec *spec = codec->spec;
	struct hda_bind_ctls *ctl;
	char name[32];
	int i, n, err;

	if (!num_pins || !pins[0])
		return 0;

	if (num_pins == 1) {
		hda_nid_t dac = *dacs;
		if (!dac)
			dac = spec->multiout.dac_nids[0];
		return alc_auto_create_extra_out(codec, *pins, dac, pfx, 0);
	}

	for (i = 0; i < num_pins; i++) {
		hda_nid_t dac;
		if (dacs[num_pins - 1])
			dac = dacs[i]; /* with individual volumes */
		else
			dac = 0;
		if (num_pins == 2 && i == 1 && !strcmp(pfx, "Speaker")) {
			err = alc_auto_create_extra_out(codec, pins[i], dac,
							"Bass Speaker", 0);
		} else if (num_pins >= 3) {
			snprintf(name, sizeof(name), "%s %s",
				 pfx, channel_name[i]);
			err = alc_auto_create_extra_out(codec, pins[i], dac,
							name, 0);
		} else {
			err = alc_auto_create_extra_out(codec, pins[i], dac,
							pfx, i);
		}
		if (err < 0)
			return err;
	}
	if (dacs[num_pins - 1])
		return 0;

	/* Let's create a bind-controls for volumes */
	ctl = new_bind_ctl(codec, num_pins, &snd_hda_bind_vol);
	if (!ctl)
		return -ENOMEM;
	n = 0;
	for (i = 0; i < num_pins; i++) {
		hda_nid_t vol;
		if (!pins[i] || !dacs[i])
			continue;
		vol = alc_look_for_out_vol_nid(codec, pins[i], dacs[i]);
		if (vol)
			ctl->values[n++] =
				HDA_COMPOSE_AMP_VAL(vol, 3, 0, HDA_OUTPUT);
	}
	if (n) {
		snprintf(name, sizeof(name), "%s Playback Volume", pfx);
		err = add_control(spec, ALC_CTL_BIND_VOL, name, 0, (long)ctl);
		if (err < 0)
			return err;
	}
	return 0;
}

static int alc_auto_create_hp_out(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	return alc_auto_create_extra_outs(codec, spec->autocfg.hp_outs,
					  spec->autocfg.hp_pins,
					  spec->multiout.hp_out_nid,
					  "Headphone");
}

static int alc_auto_create_speaker_out(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	return alc_auto_create_extra_outs(codec, spec->autocfg.speaker_outs,
					  spec->autocfg.speaker_pins,
					  spec->multiout.extra_out_nid,
					  "Speaker");
}

static void alc_auto_set_output_and_unmute(struct hda_codec *codec,
					      hda_nid_t pin, int pin_type,
					      hda_nid_t dac)
{
	int i, num;
	hda_nid_t nid, mix = 0;
	hda_nid_t srcs[HDA_MAX_CONNECTIONS];

	alc_set_pin_output(codec, pin, pin_type);
	nid = alc_go_down_to_selector(codec, pin);
	num = snd_hda_get_connections(codec, nid, srcs, ARRAY_SIZE(srcs));
	for (i = 0; i < num; i++) {
		if (alc_auto_mix_to_dac(codec, srcs[i]) != dac)
			continue;
		mix = srcs[i];
		break;
	}
	if (!mix)
		return;

	/* need the manual connection? */
	if (num > 1)
		snd_hda_codec_write(codec, nid, 0, AC_VERB_SET_CONNECT_SEL, i);
	/* unmute mixer widget inputs */
	if (nid_has_mute(codec, mix, HDA_INPUT)) {
		snd_hda_codec_write(codec, mix, 0, AC_VERB_SET_AMP_GAIN_MUTE,
			    AMP_IN_UNMUTE(0));
		snd_hda_codec_write(codec, mix, 0, AC_VERB_SET_AMP_GAIN_MUTE,
			    AMP_IN_UNMUTE(1));
	}
	/* initialize volume */
	nid = alc_look_for_out_vol_nid(codec, pin, dac);
	if (nid)
		snd_hda_codec_write(codec, nid, 0, AC_VERB_SET_AMP_GAIN_MUTE,
				    AMP_OUT_ZERO);

	/* unmute DAC if it's not assigned to a mixer */
	nid = alc_look_for_out_mute_nid(codec, pin, dac);
	if (nid == mix && nid_has_mute(codec, dac, HDA_OUTPUT))
		snd_hda_codec_write(codec, dac, 0, AC_VERB_SET_AMP_GAIN_MUTE,
				    AMP_OUT_ZERO);
}

static void alc_auto_init_multi_out(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	int pin_type = get_pin_type(spec->autocfg.line_out_type);
	int i;

	for (i = 0; i <= HDA_SIDE; i++) {
		hda_nid_t nid = spec->autocfg.line_out_pins[i];
		if (nid)
			alc_auto_set_output_and_unmute(codec, nid, pin_type,
					spec->multiout.dac_nids[i]);
	}
}

static void alc_auto_init_extra_out(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	int i;
	hda_nid_t pin, dac;

	for (i = 0; i < spec->autocfg.hp_outs; i++) {
		if (spec->autocfg.line_out_type == AUTO_PIN_HP_OUT)
			break;
		pin = spec->autocfg.hp_pins[i];
		if (!pin)
			break;
		dac = spec->multiout.hp_out_nid[i];
		if (!dac) {
			if (i > 0 && spec->multiout.hp_out_nid[0])
				dac = spec->multiout.hp_out_nid[0];
			else
				dac = spec->multiout.dac_nids[0];
		}
		alc_auto_set_output_and_unmute(codec, pin, PIN_HP, dac);
	}
	for (i = 0; i < spec->autocfg.speaker_outs; i++) {
		if (spec->autocfg.line_out_type == AUTO_PIN_SPEAKER_OUT)
			break;
		pin = spec->autocfg.speaker_pins[i];
		if (!pin)
			break;
		dac = spec->multiout.extra_out_nid[i];
		if (!dac) {
			if (i > 0 && spec->multiout.extra_out_nid[0])
				dac = spec->multiout.extra_out_nid[0];
			else
				dac = spec->multiout.dac_nids[0];
		}
		alc_auto_set_output_and_unmute(codec, pin, PIN_OUT, dac);
	}
}

/* check whether the given pin can be a multi-io pin */
static bool can_be_multiio_pin(struct hda_codec *codec,
			       unsigned int location, hda_nid_t nid)
{
	unsigned int defcfg, caps;

	defcfg = snd_hda_codec_get_pincfg(codec, nid);
	if (get_defcfg_connect(defcfg) != AC_JACK_PORT_COMPLEX)
		return false;
	if (location && get_defcfg_location(defcfg) != location)
		return false;
	caps = snd_hda_query_pin_caps(codec, nid);
	if (!(caps & AC_PINCAP_OUT))
		return false;
	return true;
}

/*
 * multi-io helper
 *
 * When hardwired is set, try to fill ony hardwired pins, and returns
 * zero if any pins are filled, non-zero if nothing found.
 * When hardwired is off, try to fill possible input pins, and returns
 * the badness value.
 */
static int alc_auto_fill_multi_ios(struct hda_codec *codec,
				   hda_nid_t reference_pin,
				   bool hardwired, int offset)
{
	struct alc_spec *spec = codec->spec;
	struct auto_pin_cfg *cfg = &spec->autocfg;
	int type, i, j, dacs, num_pins, old_pins;
	unsigned int defcfg = snd_hda_codec_get_pincfg(codec, reference_pin);
	unsigned int location = get_defcfg_location(defcfg);
	int badness = 0;

	old_pins = spec->multi_ios;
	if (old_pins >= 2)
		goto end_fill;

	num_pins = 0;
	for (type = AUTO_PIN_LINE_IN; type >= AUTO_PIN_MIC; type--) {
		for (i = 0; i < cfg->num_inputs; i++) {
			if (cfg->inputs[i].type != type)
				continue;
			if (can_be_multiio_pin(codec, location,
					       cfg->inputs[i].pin))
				num_pins++;
		}
	}
	if (num_pins < 2)
		goto end_fill;

	dacs = spec->multiout.num_dacs;
	for (type = AUTO_PIN_LINE_IN; type >= AUTO_PIN_MIC; type--) {
		for (i = 0; i < cfg->num_inputs; i++) {
			hda_nid_t nid = cfg->inputs[i].pin;
			hda_nid_t dac = 0;

			if (cfg->inputs[i].type != type)
				continue;
			if (!can_be_multiio_pin(codec, location, nid))
				continue;
			for (j = 0; j < spec->multi_ios; j++) {
				if (nid == spec->multi_io[j].pin)
					break;
			}
			if (j < spec->multi_ios)
				continue;

			if (offset && offset + spec->multi_ios < dacs) {
				dac = spec->private_dac_nids[offset + spec->multi_ios];
				if (!alc_auto_is_dac_reachable(codec, nid, dac))
					dac = 0;
			}
			if (hardwired)
				dac = get_dac_if_single(codec, nid);
			else if (!dac)
				dac = alc_auto_look_for_dac(codec, nid);
			if (!dac) {
				badness++;
				continue;
			}
			spec->multi_io[spec->multi_ios].pin = nid;
			spec->multi_io[spec->multi_ios].dac = dac;
			spec->multi_ios++;
			if (spec->multi_ios >= 2)
				break;
		}
	}
 end_fill:
	if (badness)
		badness = BAD_MULTI_IO;
	if (old_pins == spec->multi_ios) {
		if (hardwired)
			return 1; /* nothing found */
		else
			return badness; /* no badness if nothing found */
	}
	if (!hardwired && spec->multi_ios < 2) {
		spec->multi_ios = old_pins;
		return badness;
=======

	if (!codec->bus->pci)
		return 0;
	for (q = rename_pci_tbl; q->codec_vendor_id; q++) {
		if (q->codec_vendor_id != codec->core.vendor_id)
			continue;
		if (q->pci_subvendor != codec->bus->pci->subsystem_vendor)
			continue;
		if (!q->pci_subdevice ||
		    q->pci_subdevice == codec->bus->pci->subsystem_device)
			return alc_codec_rename(codec, q->name);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return 0;
}

<<<<<<< HEAD
static int alc_auto_ch_mode_info(struct snd_kcontrol *kcontrol,
				 struct snd_ctl_elem_info *uinfo)
{
	struct hda_codec *codec = snd_kcontrol_chip(kcontrol);
	struct alc_spec *spec = codec->spec;

	uinfo->type = SNDRV_CTL_ELEM_TYPE_ENUMERATED;
	uinfo->count = 1;
	uinfo->value.enumerated.items = spec->multi_ios + 1;
	if (uinfo->value.enumerated.item > spec->multi_ios)
		uinfo->value.enumerated.item = spec->multi_ios;
	sprintf(uinfo->value.enumerated.name, "%dch",
		(uinfo->value.enumerated.item + 1) * 2);
	return 0;
}

static int alc_auto_ch_mode_get(struct snd_kcontrol *kcontrol,
				struct snd_ctl_elem_value *ucontrol)
{
	struct hda_codec *codec = snd_kcontrol_chip(kcontrol);
	struct alc_spec *spec = codec->spec;
	ucontrol->value.enumerated.item[0] = (spec->ext_channel_count - 1) / 2;
	return 0;
}

static int alc_set_multi_io(struct hda_codec *codec, int idx, bool output)
{
	struct alc_spec *spec = codec->spec;
	hda_nid_t nid = spec->multi_io[idx].pin;

	if (!spec->multi_io[idx].ctl_in)
		spec->multi_io[idx].ctl_in =
			snd_hda_codec_read(codec, nid, 0,
					   AC_VERB_GET_PIN_WIDGET_CONTROL, 0);
	if (output) {
		snd_hda_codec_update_cache(codec, nid, 0,
					   AC_VERB_SET_PIN_WIDGET_CONTROL,
					   PIN_OUT);
		if (get_wcaps(codec, nid) & AC_WCAP_OUT_AMP)
			snd_hda_codec_amp_stereo(codec, nid, HDA_OUTPUT, 0,
						 HDA_AMP_MUTE, 0);
		alc_auto_select_dac(codec, nid, spec->multi_io[idx].dac);
	} else {
		if (get_wcaps(codec, nid) & AC_WCAP_OUT_AMP)
			snd_hda_codec_amp_stereo(codec, nid, HDA_OUTPUT, 0,
						 HDA_AMP_MUTE, HDA_AMP_MUTE);
		snd_hda_codec_update_cache(codec, nid, 0,
					   AC_VERB_SET_PIN_WIDGET_CONTROL,
					   spec->multi_io[idx].ctl_in);
	}
	return 0;
}

static int alc_auto_ch_mode_put(struct snd_kcontrol *kcontrol,
				struct snd_ctl_elem_value *ucontrol)
{
	struct hda_codec *codec = snd_kcontrol_chip(kcontrol);
	struct alc_spec *spec = codec->spec;
	int i, ch;

	ch = ucontrol->value.enumerated.item[0];
	if (ch < 0 || ch > spec->multi_ios)
		return -EINVAL;
	if (ch == (spec->ext_channel_count - 1) / 2)
		return 0;
	spec->ext_channel_count = (ch + 1) * 2;
	for (i = 0; i < spec->multi_ios; i++)
		alc_set_multi_io(codec, i, i < ch);
	spec->multiout.max_channels = spec->ext_channel_count;
	if (spec->need_dac_fix && !spec->const_channel_count)
		spec->multiout.num_dacs = spec->multiout.max_channels / 2;
	return 1;
}

static const struct snd_kcontrol_new alc_auto_channel_mode_enum = {
	.iface = SNDRV_CTL_ELEM_IFACE_MIXER,
	.name = "Channel Mode",
	.info = alc_auto_ch_mode_info,
	.get = alc_auto_ch_mode_get,
	.put = alc_auto_ch_mode_put,
};

static int alc_auto_add_multi_channel_mode(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;

	if (spec->multi_ios > 0) {
		struct snd_kcontrol_new *knew;

		knew = alc_kcontrol_new(spec);
		if (!knew)
			return -ENOMEM;
		*knew = alc_auto_channel_mode_enum;
		knew->name = kstrdup("Channel Mode", GFP_KERNEL);
		if (!knew->name)
			return -ENOMEM;
	}
	return 0;
}

/* filter out invalid adc_nids (and capsrc_nids) that don't give all
 * active input pins
 */
static void alc_remove_invalid_adc_nids(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	const struct hda_input_mux *imux;
	hda_nid_t adc_nids[ARRAY_SIZE(spec->private_adc_nids)];
	hda_nid_t capsrc_nids[ARRAY_SIZE(spec->private_adc_nids)];
	int i, n, nums;

	imux = spec->input_mux;
	if (!imux)
		return;
	if (spec->dyn_adc_switch)
		return;

 again:
	nums = 0;
	for (n = 0; n < spec->num_adc_nids; n++) {
		hda_nid_t cap = spec->private_capsrc_nids[n];
		int num_conns = snd_hda_get_conn_list(codec, cap, NULL);
		for (i = 0; i < imux->num_items; i++) {
			hda_nid_t pin = spec->imux_pins[i];
			if (pin) {
				if (get_connection_index(codec, cap, pin) < 0)
					break;
			} else if (num_conns <= imux->items[i].index)
				break;
		}
		if (i >= imux->num_items) {
			adc_nids[nums] = spec->private_adc_nids[n];
			capsrc_nids[nums++] = cap;
		}
	}
	if (!nums) {
		/* check whether ADC-switch is possible */
		if (!alc_check_dyn_adc_switch(codec)) {
			if (spec->shared_mic_hp) {
				spec->shared_mic_hp = 0;
				spec->private_imux[0].num_items = 1;
				goto again;
			}
			printk(KERN_WARNING "hda_codec: %s: no valid ADC found;"
			       " using fallback 0x%x\n",
			       codec->chip_name, spec->private_adc_nids[0]);
			spec->num_adc_nids = 1;
			spec->auto_mic = 0;
			return;
		}
	} else if (nums != spec->num_adc_nids) {
		memcpy(spec->private_adc_nids, adc_nids,
		       nums * sizeof(hda_nid_t));
		memcpy(spec->private_capsrc_nids, capsrc_nids,
		       nums * sizeof(hda_nid_t));
		spec->num_adc_nids = nums;
	}

	if (spec->auto_mic)
		alc_auto_mic_check_imux(codec); /* check auto-mic setups */
	else if (spec->input_mux->num_items == 1 || spec->shared_mic_hp)
		spec->num_adc_nids = 1; /* reduce to a single ADC */
}

/*
 * initialize ADC paths
 */
static void alc_auto_init_adc(struct hda_codec *codec, int adc_idx)
{
	struct alc_spec *spec = codec->spec;
	hda_nid_t nid;

	nid = spec->adc_nids[adc_idx];
	/* mute ADC */
	if (nid_has_mute(codec, nid, HDA_INPUT)) {
		snd_hda_codec_write(codec, nid, 0,
				    AC_VERB_SET_AMP_GAIN_MUTE,
				    AMP_IN_MUTE(0));
		return;
	}
	if (!spec->capsrc_nids)
		return;
	nid = spec->capsrc_nids[adc_idx];
	if (nid_has_mute(codec, nid, HDA_OUTPUT))
		snd_hda_codec_write(codec, nid, 0,
				    AC_VERB_SET_AMP_GAIN_MUTE,
				    AMP_OUT_MUTE);
}

static void alc_auto_init_input_src(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	int c, nums;

	for (c = 0; c < spec->num_adc_nids; c++)
		alc_auto_init_adc(codec, c);
	if (spec->dyn_adc_switch)
		nums = 1;
	else
		nums = spec->num_adc_nids;
	for (c = 0; c < nums; c++)
		alc_mux_select(codec, c, spec->cur_mux[c], true);
}

/* add mic boosts if needed */
static int alc_auto_add_mic_boost(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	struct auto_pin_cfg *cfg = &spec->autocfg;
	int i, err;
	int type_idx = 0;
	hda_nid_t nid;
	const char *prev_label = NULL;

	for (i = 0; i < cfg->num_inputs; i++) {
		if (cfg->inputs[i].type > AUTO_PIN_MIC)
			break;
		nid = cfg->inputs[i].pin;
		if (get_wcaps(codec, nid) & AC_WCAP_IN_AMP) {
			const char *label;
			char boost_label[32];

			label = hda_get_autocfg_input_label(codec, cfg, i);
			if (spec->shared_mic_hp && !strcmp(label, "Misc"))
				label = "Headphone Mic";
			if (prev_label && !strcmp(label, prev_label))
				type_idx++;
			else
				type_idx = 0;
			prev_label = label;

			snprintf(boost_label, sizeof(boost_label),
				 "%s Boost Volume", label);
			err = add_control(spec, ALC_CTL_WIDGET_VOL,
					  boost_label, type_idx,
				  HDA_COMPOSE_AMP_VAL(nid, 3, 0, HDA_INPUT));
			if (err < 0)
				return err;
		}
	}
	return 0;
}

/* select or unmute the given capsrc route */
static void select_or_unmute_capsrc(struct hda_codec *codec, hda_nid_t cap,
				    int idx)
{
	if (get_wcaps_type(get_wcaps(codec, cap)) == AC_WID_AUD_MIX) {
		snd_hda_codec_amp_stereo(codec, cap, HDA_INPUT, idx,
					 HDA_AMP_MUTE, 0);
	} else if (snd_hda_get_conn_list(codec, cap, NULL) > 1) {
		snd_hda_codec_write_cache(codec, cap, 0,
					  AC_VERB_SET_CONNECT_SEL, idx);
	}
}

/* set the default connection to that pin */
static int init_capsrc_for_pin(struct hda_codec *codec, hda_nid_t pin)
{
	struct alc_spec *spec = codec->spec;
	int i;

	if (!pin)
		return 0;
	for (i = 0; i < spec->num_adc_nids; i++) {
		hda_nid_t cap = get_capsrc(spec, i);
		int idx;

		idx = get_connection_index(codec, cap, pin);
		if (idx < 0)
			continue;
		select_or_unmute_capsrc(codec, cap, idx);
		return i; /* return the found index */
	}
	return -1; /* not found */
}

/* initialize some special cases for input sources */
static void alc_init_special_input_src(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	int i;

	for (i = 0; i < spec->autocfg.num_inputs; i++)
		init_capsrc_for_pin(codec, spec->autocfg.inputs[i].pin);
}

/* assign appropriate capture mixers */
static void set_capture_mixer(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	static const struct snd_kcontrol_new *caps[2][3] = {
		{ alc_capture_mixer_nosrc1,
		  alc_capture_mixer_nosrc2,
		  alc_capture_mixer_nosrc3 },
		{ alc_capture_mixer1,
		  alc_capture_mixer2,
		  alc_capture_mixer3 },
	};

	/* check whether either of ADC or MUX has a volume control */
	if (!nid_has_volume(codec, spec->adc_nids[0], HDA_INPUT)) {
		if (!spec->capsrc_nids)
			return; /* no volume */
		if (!nid_has_volume(codec, spec->capsrc_nids[0], HDA_OUTPUT))
			return; /* no volume in capsrc, too */
		spec->vol_in_capsrc = 1;
	}

	if (spec->num_adc_nids > 0) {
		int mux = 0;
		int num_adcs = 0;

		if (spec->input_mux && spec->input_mux->num_items > 1)
			mux = 1;
		if (spec->auto_mic) {
			num_adcs = 1;
			mux = 0;
		} else if (spec->dyn_adc_switch)
			num_adcs = 1;
		if (!num_adcs) {
			if (spec->num_adc_nids > 3)
				spec->num_adc_nids = 3;
			else if (!spec->num_adc_nids)
				return;
			num_adcs = spec->num_adc_nids;
		}
		spec->cap_mixer = caps[mux][num_adcs - 1];
	}
}

/*
 * standard auto-parser initializations
 */
static void alc_auto_init_std(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	alc_auto_init_multi_out(codec);
	alc_auto_init_extra_out(codec);
	alc_auto_init_analog_input(codec);
	alc_auto_init_input_src(codec);
	alc_auto_init_digital(codec);
	if (spec->unsol_event)
		alc_inithook(codec);
}
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Digital-beep handlers
 */
#ifdef CONFIG_SND_HDA_INPUT_BEEP
<<<<<<< HEAD
#define set_beep_amp(spec, nid, idx, dir) \
	((spec)->beep_amp = HDA_COMPOSE_AMP_VAL(nid, 3, idx, dir))

static const struct snd_pci_quirk beep_white_list[] = {
	SND_PCI_QUIRK(0x1043, 0x103c, "ASUS", 1),
	SND_PCI_QUIRK(0x1043, 0x115d, "ASUS", 1),
	SND_PCI_QUIRK(0x1043, 0x829f, "ASUS", 1),
=======

/* additional beep mixers; private_value will be overwritten */
static const struct snd_kcontrol_new alc_beep_mixer[] = {
	HDA_CODEC_VOLUME("Beep Playback Volume", 0, 0, HDA_INPUT),
	HDA_CODEC_MUTE_BEEP("Beep Playback Switch", 0, 0, HDA_INPUT),
};

/* set up and create beep controls */
static int set_beep_amp(struct alc_spec *spec, hda_nid_t nid,
			int idx, int dir)
{
	struct snd_kcontrol_new *knew;
	unsigned int beep_amp = HDA_COMPOSE_AMP_VAL(nid, 3, idx, dir);
	int i;

	for (i = 0; i < ARRAY_SIZE(alc_beep_mixer); i++) {
		knew = snd_hda_gen_add_kctl(&spec->gen, NULL,
					    &alc_beep_mixer[i]);
		if (!knew)
			return -ENOMEM;
		knew->private_value = beep_amp;
	}
	return 0;
}

static const struct snd_pci_quirk beep_allow_list[] = {
	SND_PCI_QUIRK(0x1043, 0x103c, "ASUS", 1),
	SND_PCI_QUIRK(0x1043, 0x115d, "ASUS", 1),
	SND_PCI_QUIRK(0x1043, 0x829f, "ASUS", 1),
	SND_PCI_QUIRK(0x1043, 0x8376, "EeePC", 1),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	SND_PCI_QUIRK(0x1043, 0x83ce, "EeePC", 1),
	SND_PCI_QUIRK(0x1043, 0x831a, "EeePC", 1),
	SND_PCI_QUIRK(0x1043, 0x834a, "EeePC", 1),
	SND_PCI_QUIRK(0x1458, 0xa002, "GA-MA790X", 1),
	SND_PCI_QUIRK(0x8086, 0xd613, "Intel", 1),
<<<<<<< HEAD
=======
	/* denylist -- no beep available */
	SND_PCI_QUIRK(0x17aa, 0x309e, "Lenovo ThinkCentre M73", 0),
	SND_PCI_QUIRK(0x17aa, 0x30a3, "Lenovo ThinkCentre M93", 0),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{}
};

static inline int has_cdefine_beep(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	const struct snd_pci_quirk *q;
<<<<<<< HEAD
	q = snd_pci_quirk_lookup(codec->bus->pci, beep_white_list);
=======
	q = snd_pci_quirk_lookup(codec->bus->pci, beep_allow_list);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (q)
		return q->value;
	return spec->cdefine.enable_pcbeep;
}
#else
<<<<<<< HEAD
#define set_beep_amp(spec, nid, idx, dir) /* NOP */
=======
#define set_beep_amp(spec, nid, idx, dir)	0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define has_cdefine_beep(codec)		0
#endif

/* parse the BIOS configuration and set up the alc_spec */
/* return 1 if successful, 0 if the proper config is not found,
 * or a negative error code
 */
static int alc_parse_auto_config(struct hda_codec *codec,
				 const hda_nid_t *ignore_nids,
				 const hda_nid_t *ssid_nids)
{
	struct alc_spec *spec = codec->spec;
<<<<<<< HEAD
	struct auto_pin_cfg *cfg = &spec->autocfg;
=======
	struct auto_pin_cfg *cfg = &spec->gen.autocfg;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int err;

	err = snd_hda_parse_pin_defcfg(codec, cfg, ignore_nids,
				       spec->parse_flags);
	if (err < 0)
		return err;
<<<<<<< HEAD
	if (!cfg->line_outs) {
		if (cfg->dig_outs || cfg->dig_in_pin) {
			spec->multiout.max_channels = 2;
			spec->no_analog = 1;
			goto dig_only;
		}
		return 0; /* can't find valid BIOS pin config */
	}

	if (!spec->no_primary_hp &&
	    cfg->line_out_type == AUTO_PIN_SPEAKER_OUT &&
	    cfg->line_outs <= cfg->hp_outs) {
		/* use HP as primary out */
		cfg->speaker_outs = cfg->line_outs;
		memcpy(cfg->speaker_pins, cfg->line_out_pins,
		       sizeof(cfg->speaker_pins));
		cfg->line_outs = cfg->hp_outs;
		memcpy(cfg->line_out_pins, cfg->hp_pins, sizeof(cfg->hp_pins));
		cfg->hp_outs = 0;
		memset(cfg->hp_pins, 0, sizeof(cfg->hp_pins));
		cfg->line_out_type = AUTO_PIN_HP_OUT;
	}

	err = alc_auto_fill_dac_nids(codec);
	if (err < 0)
		return err;
	err = alc_auto_add_multi_channel_mode(codec);
	if (err < 0)
		return err;
	err = alc_auto_create_multi_out_ctls(codec, cfg);
	if (err < 0)
		return err;
	err = alc_auto_create_hp_out(codec);
	if (err < 0)
		return err;
	err = alc_auto_create_speaker_out(codec);
	if (err < 0)
		return err;
	err = alc_auto_create_shared_input(codec);
	if (err < 0)
		return err;
	err = alc_auto_create_input_ctls(codec);
	if (err < 0)
		return err;

	spec->multiout.max_channels = spec->multiout.num_dacs * 2;

 dig_only:
	alc_auto_parse_digital(codec);

	if (!spec->no_analog)
		alc_remove_invalid_adc_nids(codec);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (ssid_nids)
		alc_ssid_check(codec, ssid_nids);

<<<<<<< HEAD
	if (!spec->no_analog) {
		alc_auto_check_switches(codec);
		err = alc_auto_add_mic_boost(codec);
		if (err < 0)
			return err;
	}

	if (spec->kctls.list)
		add_mixer(spec, spec->kctls.list);

	if (!spec->no_analog && !spec->cap_mixer)
		set_capture_mixer(codec);
=======
	err = snd_hda_gen_parse_auto_config(codec, cfg);
	if (err < 0)
		return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 1;
}

<<<<<<< HEAD
=======
/* common preparation job for alc_spec */
static int alc_alloc_spec(struct hda_codec *codec, hda_nid_t mixer_nid)
{
	struct alc_spec *spec = kzalloc(sizeof(*spec), GFP_KERNEL);
	int err;

	if (!spec)
		return -ENOMEM;
	codec->spec = spec;
	snd_hda_gen_spec_init(&spec->gen);
	spec->gen.mixer_nid = mixer_nid;
	spec->gen.own_eapd_ctl = 1;
	codec->single_adc_amp = 1;
	/* FIXME: do we need this for all Realtek codec models? */
	codec->spdif_status_reset = 1;
	codec->forced_resume = 1;
	codec->patch_ops = alc_patch_ops;
	mutex_init(&spec->coef_mutex);

	err = alc_codec_rename_from_preset(codec);
	if (err < 0) {
		kfree(spec);
		return err;
	}
	return 0;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int alc880_parse_auto_config(struct hda_codec *codec)
{
	static const hda_nid_t alc880_ignore[] = { 0x1d, 0 };
	static const hda_nid_t alc880_ssids[] = { 0x15, 0x1b, 0x14, 0 };
	return alc_parse_auto_config(codec, alc880_ignore, alc880_ssids);
}

/*
 * ALC880 fix-ups
 */
enum {
	ALC880_FIXUP_GPIO1,
	ALC880_FIXUP_GPIO2,
	ALC880_FIXUP_MEDION_RIM,
	ALC880_FIXUP_LG,
<<<<<<< HEAD
=======
	ALC880_FIXUP_LG_LW25,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ALC880_FIXUP_W810,
	ALC880_FIXUP_EAPD_COEF,
	ALC880_FIXUP_TCL_S700,
	ALC880_FIXUP_VOL_KNOB,
	ALC880_FIXUP_FUJITSU,
	ALC880_FIXUP_F1734,
	ALC880_FIXUP_UNIWILL,
	ALC880_FIXUP_UNIWILL_DIG,
	ALC880_FIXUP_Z71V,
<<<<<<< HEAD
=======
	ALC880_FIXUP_ASUS_W5A,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ALC880_FIXUP_3ST_BASE,
	ALC880_FIXUP_3ST,
	ALC880_FIXUP_3ST_DIG,
	ALC880_FIXUP_5ST_BASE,
	ALC880_FIXUP_5ST,
	ALC880_FIXUP_5ST_DIG,
	ALC880_FIXUP_6ST_BASE,
	ALC880_FIXUP_6ST,
	ALC880_FIXUP_6ST_DIG,
<<<<<<< HEAD
=======
	ALC880_FIXUP_6ST_AUTOMUTE,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/* enable the volume-knob widget support on NID 0x21 */
static void alc880_fixup_vol_knob(struct hda_codec *codec,
<<<<<<< HEAD
				  const struct alc_fixup *fix, int action)
{
	if (action == ALC_FIXUP_ACT_PROBE)
		snd_hda_jack_detect_enable(codec, 0x21, ALC_DCVOL_EVENT);
}

static const struct alc_fixup alc880_fixups[] = {
	[ALC880_FIXUP_GPIO1] = {
		.type = ALC_FIXUP_VERBS,
		.v.verbs = alc_gpio1_init_verbs,
	},
	[ALC880_FIXUP_GPIO2] = {
		.type = ALC_FIXUP_VERBS,
		.v.verbs = alc_gpio2_init_verbs,
	},
	[ALC880_FIXUP_MEDION_RIM] = {
		.type = ALC_FIXUP_VERBS,
=======
				  const struct hda_fixup *fix, int action)
{
	if (action == HDA_FIXUP_ACT_PROBE)
		snd_hda_jack_detect_enable_callback(codec, 0x21,
						    alc_update_knob_master);
}

static const struct hda_fixup alc880_fixups[] = {
	[ALC880_FIXUP_GPIO1] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_gpio1,
	},
	[ALC880_FIXUP_GPIO2] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_gpio2,
	},
	[ALC880_FIXUP_MEDION_RIM] = {
		.type = HDA_FIXUP_VERBS,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.v.verbs = (const struct hda_verb[]) {
			{ 0x20, AC_VERB_SET_COEF_INDEX, 0x07 },
			{ 0x20, AC_VERB_SET_PROC_COEF,  0x3060 },
			{ }
		},
		.chained = true,
		.chain_id = ALC880_FIXUP_GPIO2,
	},
	[ALC880_FIXUP_LG] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_PINS,
		.v.pins = (const struct alc_pincfg[]) {
=======
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/* disable bogus unused pins */
			{ 0x16, 0x411111f0 },
			{ 0x18, 0x411111f0 },
			{ 0x1a, 0x411111f0 },
			{ }
		}
	},
<<<<<<< HEAD
	[ALC880_FIXUP_W810] = {
		.type = ALC_FIXUP_PINS,
		.v.pins = (const struct alc_pincfg[]) {
=======
	[ALC880_FIXUP_LG_LW25] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x1a, 0x0181344f }, /* line-in */
			{ 0x1b, 0x0321403f }, /* headphone */
			{ }
		}
	},
	[ALC880_FIXUP_W810] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/* disable bogus unused pins */
			{ 0x17, 0x411111f0 },
			{ }
		},
		.chained = true,
		.chain_id = ALC880_FIXUP_GPIO2,
	},
	[ALC880_FIXUP_EAPD_COEF] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_VERBS,
=======
		.type = HDA_FIXUP_VERBS,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.v.verbs = (const struct hda_verb[]) {
			/* change to EAPD mode */
			{ 0x20, AC_VERB_SET_COEF_INDEX, 0x07 },
			{ 0x20, AC_VERB_SET_PROC_COEF,  0x3060 },
			{}
		},
	},
	[ALC880_FIXUP_TCL_S700] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_VERBS,
=======
		.type = HDA_FIXUP_VERBS,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.v.verbs = (const struct hda_verb[]) {
			/* change to EAPD mode */
			{ 0x20, AC_VERB_SET_COEF_INDEX, 0x07 },
			{ 0x20, AC_VERB_SET_PROC_COEF,  0x3070 },
			{}
		},
		.chained = true,
		.chain_id = ALC880_FIXUP_GPIO2,
	},
	[ALC880_FIXUP_VOL_KNOB] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_FUNC,
=======
		.type = HDA_FIXUP_FUNC,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.v.func = alc880_fixup_vol_knob,
	},
	[ALC880_FIXUP_FUJITSU] = {
		/* override all pins as BIOS on old Amilo is broken */
<<<<<<< HEAD
		.type = ALC_FIXUP_PINS,
		.v.pins = (const struct alc_pincfg[]) {
			{ 0x14, 0x0121411f }, /* HP */
=======
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x14, 0x0121401f }, /* HP */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			{ 0x15, 0x99030120 }, /* speaker */
			{ 0x16, 0x99030130 }, /* bass speaker */
			{ 0x17, 0x411111f0 }, /* N/A */
			{ 0x18, 0x411111f0 }, /* N/A */
			{ 0x19, 0x01a19950 }, /* mic-in */
			{ 0x1a, 0x411111f0 }, /* N/A */
			{ 0x1b, 0x411111f0 }, /* N/A */
			{ 0x1c, 0x411111f0 }, /* N/A */
			{ 0x1d, 0x411111f0 }, /* N/A */
			{ 0x1e, 0x01454140 }, /* SPDIF out */
			{ }
		},
		.chained = true,
		.chain_id = ALC880_FIXUP_VOL_KNOB,
	},
	[ALC880_FIXUP_F1734] = {
		/* almost compatible with FUJITSU, but no bass and SPDIF */
<<<<<<< HEAD
		.type = ALC_FIXUP_PINS,
		.v.pins = (const struct alc_pincfg[]) {
			{ 0x14, 0x0121411f }, /* HP */
=======
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x14, 0x0121401f }, /* HP */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			{ 0x15, 0x99030120 }, /* speaker */
			{ 0x16, 0x411111f0 }, /* N/A */
			{ 0x17, 0x411111f0 }, /* N/A */
			{ 0x18, 0x411111f0 }, /* N/A */
			{ 0x19, 0x01a19950 }, /* mic-in */
			{ 0x1a, 0x411111f0 }, /* N/A */
			{ 0x1b, 0x411111f0 }, /* N/A */
			{ 0x1c, 0x411111f0 }, /* N/A */
			{ 0x1d, 0x411111f0 }, /* N/A */
			{ 0x1e, 0x411111f0 }, /* N/A */
			{ }
		},
		.chained = true,
		.chain_id = ALC880_FIXUP_VOL_KNOB,
	},
	[ALC880_FIXUP_UNIWILL] = {
		/* need to fix HP and speaker pins to be parsed correctly */
<<<<<<< HEAD
		.type = ALC_FIXUP_PINS,
		.v.pins = (const struct alc_pincfg[]) {
=======
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			{ 0x14, 0x0121411f }, /* HP */
			{ 0x15, 0x99030120 }, /* speaker */
			{ 0x16, 0x99030130 }, /* bass speaker */
			{ }
		},
	},
	[ALC880_FIXUP_UNIWILL_DIG] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_PINS,
		.v.pins = (const struct alc_pincfg[]) {
=======
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/* disable bogus unused pins */
			{ 0x17, 0x411111f0 },
			{ 0x19, 0x411111f0 },
			{ 0x1b, 0x411111f0 },
			{ 0x1f, 0x411111f0 },
			{ }
		}
	},
	[ALC880_FIXUP_Z71V] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_PINS,
		.v.pins = (const struct alc_pincfg[]) {
=======
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/* set up the whole pins as BIOS is utterly broken */
			{ 0x14, 0x99030120 }, /* speaker */
			{ 0x15, 0x0121411f }, /* HP */
			{ 0x16, 0x411111f0 }, /* N/A */
			{ 0x17, 0x411111f0 }, /* N/A */
			{ 0x18, 0x01a19950 }, /* mic-in */
			{ 0x19, 0x411111f0 }, /* N/A */
			{ 0x1a, 0x01813031 }, /* line-in */
			{ 0x1b, 0x411111f0 }, /* N/A */
			{ 0x1c, 0x411111f0 }, /* N/A */
			{ 0x1d, 0x411111f0 }, /* N/A */
			{ 0x1e, 0x0144111e }, /* SPDIF */
			{ }
		}
	},
<<<<<<< HEAD
	[ALC880_FIXUP_3ST_BASE] = {
		.type = ALC_FIXUP_PINS,
		.v.pins = (const struct alc_pincfg[]) {
=======
	[ALC880_FIXUP_ASUS_W5A] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			/* set up the whole pins as BIOS is utterly broken */
			{ 0x14, 0x0121411f }, /* HP */
			{ 0x15, 0x411111f0 }, /* N/A */
			{ 0x16, 0x411111f0 }, /* N/A */
			{ 0x17, 0x411111f0 }, /* N/A */
			{ 0x18, 0x90a60160 }, /* mic */
			{ 0x19, 0x411111f0 }, /* N/A */
			{ 0x1a, 0x411111f0 }, /* N/A */
			{ 0x1b, 0x411111f0 }, /* N/A */
			{ 0x1c, 0x411111f0 }, /* N/A */
			{ 0x1d, 0x411111f0 }, /* N/A */
			{ 0x1e, 0xb743111e }, /* SPDIF out */
			{ }
		},
		.chained = true,
		.chain_id = ALC880_FIXUP_GPIO1,
	},
	[ALC880_FIXUP_3ST_BASE] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			{ 0x14, 0x01014010 }, /* line-out */
			{ 0x15, 0x411111f0 }, /* N/A */
			{ 0x16, 0x411111f0 }, /* N/A */
			{ 0x17, 0x411111f0 }, /* N/A */
			{ 0x18, 0x01a19c30 }, /* mic-in */
			{ 0x19, 0x0121411f }, /* HP */
			{ 0x1a, 0x01813031 }, /* line-in */
			{ 0x1b, 0x02a19c40 }, /* front-mic */
			{ 0x1c, 0x411111f0 }, /* N/A */
			{ 0x1d, 0x411111f0 }, /* N/A */
			/* 0x1e is filled in below */
			{ 0x1f, 0x411111f0 }, /* N/A */
			{ }
		}
	},
	[ALC880_FIXUP_3ST] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_PINS,
		.v.pins = (const struct alc_pincfg[]) {
=======
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			{ 0x1e, 0x411111f0 }, /* N/A */
			{ }
		},
		.chained = true,
		.chain_id = ALC880_FIXUP_3ST_BASE,
	},
	[ALC880_FIXUP_3ST_DIG] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_PINS,
		.v.pins = (const struct alc_pincfg[]) {
=======
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			{ 0x1e, 0x0144111e }, /* SPDIF */
			{ }
		},
		.chained = true,
		.chain_id = ALC880_FIXUP_3ST_BASE,
	},
	[ALC880_FIXUP_5ST_BASE] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_PINS,
		.v.pins = (const struct alc_pincfg[]) {
=======
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			{ 0x14, 0x01014010 }, /* front */
			{ 0x15, 0x411111f0 }, /* N/A */
			{ 0x16, 0x01011411 }, /* CLFE */
			{ 0x17, 0x01016412 }, /* surr */
			{ 0x18, 0x01a19c30 }, /* mic-in */
			{ 0x19, 0x0121411f }, /* HP */
			{ 0x1a, 0x01813031 }, /* line-in */
			{ 0x1b, 0x02a19c40 }, /* front-mic */
			{ 0x1c, 0x411111f0 }, /* N/A */
			{ 0x1d, 0x411111f0 }, /* N/A */
			/* 0x1e is filled in below */
			{ 0x1f, 0x411111f0 }, /* N/A */
			{ }
		}
	},
	[ALC880_FIXUP_5ST] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_PINS,
		.v.pins = (const struct alc_pincfg[]) {
=======
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			{ 0x1e, 0x411111f0 }, /* N/A */
			{ }
		},
		.chained = true,
		.chain_id = ALC880_FIXUP_5ST_BASE,
	},
	[ALC880_FIXUP_5ST_DIG] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_PINS,
		.v.pins = (const struct alc_pincfg[]) {
=======
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			{ 0x1e, 0x0144111e }, /* SPDIF */
			{ }
		},
		.chained = true,
		.chain_id = ALC880_FIXUP_5ST_BASE,
	},
	[ALC880_FIXUP_6ST_BASE] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_PINS,
		.v.pins = (const struct alc_pincfg[]) {
=======
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			{ 0x14, 0x01014010 }, /* front */
			{ 0x15, 0x01016412 }, /* surr */
			{ 0x16, 0x01011411 }, /* CLFE */
			{ 0x17, 0x01012414 }, /* side */
			{ 0x18, 0x01a19c30 }, /* mic-in */
			{ 0x19, 0x02a19c40 }, /* front-mic */
			{ 0x1a, 0x01813031 }, /* line-in */
			{ 0x1b, 0x0121411f }, /* HP */
			{ 0x1c, 0x411111f0 }, /* N/A */
			{ 0x1d, 0x411111f0 }, /* N/A */
			/* 0x1e is filled in below */
			{ 0x1f, 0x411111f0 }, /* N/A */
			{ }
		}
	},
	[ALC880_FIXUP_6ST] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_PINS,
		.v.pins = (const struct alc_pincfg[]) {
=======
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			{ 0x1e, 0x411111f0 }, /* N/A */
			{ }
		},
		.chained = true,
		.chain_id = ALC880_FIXUP_6ST_BASE,
	},
	[ALC880_FIXUP_6ST_DIG] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_PINS,
		.v.pins = (const struct alc_pincfg[]) {
=======
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			{ 0x1e, 0x0144111e }, /* SPDIF */
			{ }
		},
		.chained = true,
		.chain_id = ALC880_FIXUP_6ST_BASE,
	},
<<<<<<< HEAD
=======
	[ALC880_FIXUP_6ST_AUTOMUTE] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x1b, 0x0121401f }, /* HP with jack detect */
			{ }
		},
		.chained_before = true,
		.chain_id = ALC880_FIXUP_6ST_BASE,
	},
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static const struct snd_pci_quirk alc880_fixup_tbl[] = {
	SND_PCI_QUIRK(0x1019, 0x0f69, "Coeus G610P", ALC880_FIXUP_W810),
<<<<<<< HEAD
	SND_PCI_QUIRK(0x1043, 0x1964, "ASUS Z71V", ALC880_FIXUP_Z71V),
	SND_PCI_QUIRK_VENDOR(0x1043, "ASUS", ALC880_FIXUP_GPIO1),
=======
	SND_PCI_QUIRK(0x1043, 0x10c3, "ASUS W5A", ALC880_FIXUP_ASUS_W5A),
	SND_PCI_QUIRK(0x1043, 0x1964, "ASUS Z71V", ALC880_FIXUP_Z71V),
	SND_PCI_QUIRK_VENDOR(0x1043, "ASUS", ALC880_FIXUP_GPIO1),
	SND_PCI_QUIRK(0x147b, 0x1045, "ABit AA8XE", ALC880_FIXUP_6ST_AUTOMUTE),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	SND_PCI_QUIRK(0x1558, 0x5401, "Clevo GPIO2", ALC880_FIXUP_GPIO2),
	SND_PCI_QUIRK_VENDOR(0x1558, "Clevo", ALC880_FIXUP_EAPD_COEF),
	SND_PCI_QUIRK(0x1584, 0x9050, "Uniwill", ALC880_FIXUP_UNIWILL_DIG),
	SND_PCI_QUIRK(0x1584, 0x9054, "Uniwill", ALC880_FIXUP_F1734),
	SND_PCI_QUIRK(0x1584, 0x9070, "Uniwill", ALC880_FIXUP_UNIWILL),
	SND_PCI_QUIRK(0x1584, 0x9077, "Uniwill P53", ALC880_FIXUP_VOL_KNOB),
	SND_PCI_QUIRK(0x161f, 0x203d, "W810", ALC880_FIXUP_W810),
	SND_PCI_QUIRK(0x161f, 0x205d, "Medion Rim 2150", ALC880_FIXUP_MEDION_RIM),
<<<<<<< HEAD
	SND_PCI_QUIRK(0x1631, 0xe011, "PB 13201056", ALC880_FIXUP_6ST),
	SND_PCI_QUIRK(0x1734, 0x107c, "FSC F1734", ALC880_FIXUP_F1734),
=======
	SND_PCI_QUIRK(0x1631, 0xe011, "PB 13201056", ALC880_FIXUP_6ST_AUTOMUTE),
	SND_PCI_QUIRK(0x1734, 0x107c, "FSC Amilo M1437", ALC880_FIXUP_FUJITSU),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	SND_PCI_QUIRK(0x1734, 0x1094, "FSC Amilo M1451G", ALC880_FIXUP_FUJITSU),
	SND_PCI_QUIRK(0x1734, 0x10ac, "FSC AMILO Xi 1526", ALC880_FIXUP_F1734),
	SND_PCI_QUIRK(0x1734, 0x10b0, "FSC Amilo Pi1556", ALC880_FIXUP_FUJITSU),
	SND_PCI_QUIRK(0x1854, 0x003b, "LG", ALC880_FIXUP_LG),
	SND_PCI_QUIRK(0x1854, 0x005f, "LG P1 Express", ALC880_FIXUP_LG),
	SND_PCI_QUIRK(0x1854, 0x0068, "LG w1", ALC880_FIXUP_LG),
<<<<<<< HEAD
=======
	SND_PCI_QUIRK(0x1854, 0x0077, "LG LW25", ALC880_FIXUP_LG_LW25),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	SND_PCI_QUIRK(0x19db, 0x4188, "TCL S700", ALC880_FIXUP_TCL_S700),

	/* Below is the copied entries from alc880_quirks.c.
	 * It's not quite sure whether BIOS sets the correct pin-config table
	 * on these machines, thus they are kept to be compatible with
	 * the old static quirks.  Once when it's confirmed to work without
	 * these overrides, it'd be better to remove.
	 */
	SND_PCI_QUIRK(0x1019, 0xa880, "ECS", ALC880_FIXUP_5ST_DIG),
	SND_PCI_QUIRK(0x1019, 0xa884, "Acer APFV", ALC880_FIXUP_6ST),
	SND_PCI_QUIRK(0x1025, 0x0070, "ULI", ALC880_FIXUP_3ST_DIG),
	SND_PCI_QUIRK(0x1025, 0x0077, "ULI", ALC880_FIXUP_6ST_DIG),
	SND_PCI_QUIRK(0x1025, 0x0078, "ULI", ALC880_FIXUP_6ST_DIG),
	SND_PCI_QUIRK(0x1025, 0x0087, "ULI", ALC880_FIXUP_6ST_DIG),
	SND_PCI_QUIRK(0x1025, 0xe309, "ULI", ALC880_FIXUP_3ST_DIG),
	SND_PCI_QUIRK(0x1025, 0xe310, "ULI", ALC880_FIXUP_3ST),
	SND_PCI_QUIRK(0x1039, 0x1234, NULL, ALC880_FIXUP_6ST_DIG),
	SND_PCI_QUIRK(0x104d, 0x81a0, "Sony", ALC880_FIXUP_3ST),
	SND_PCI_QUIRK(0x104d, 0x81d6, "Sony", ALC880_FIXUP_3ST),
	SND_PCI_QUIRK(0x107b, 0x3032, "Gateway", ALC880_FIXUP_5ST),
	SND_PCI_QUIRK(0x107b, 0x3033, "Gateway", ALC880_FIXUP_5ST),
	SND_PCI_QUIRK(0x107b, 0x4039, "Gateway", ALC880_FIXUP_5ST),
	SND_PCI_QUIRK(0x1297, 0xc790, "Shuttle ST20G5", ALC880_FIXUP_6ST_DIG),
	SND_PCI_QUIRK(0x1458, 0xa102, "Gigabyte K8", ALC880_FIXUP_6ST_DIG),
	SND_PCI_QUIRK(0x1462, 0x1150, "MSI", ALC880_FIXUP_6ST_DIG),
	SND_PCI_QUIRK(0x1509, 0x925d, "FIC P4M", ALC880_FIXUP_6ST_DIG),
	SND_PCI_QUIRK(0x1565, 0x8202, "Biostar", ALC880_FIXUP_5ST_DIG),
	SND_PCI_QUIRK(0x1695, 0x400d, "EPoX", ALC880_FIXUP_5ST_DIG),
	SND_PCI_QUIRK(0x1695, 0x4012, "EPox EP-5LDA", ALC880_FIXUP_5ST_DIG),
	SND_PCI_QUIRK(0x2668, 0x8086, NULL, ALC880_FIXUP_6ST_DIG), /* broken BIOS */
	SND_PCI_QUIRK(0x8086, 0x2668, NULL, ALC880_FIXUP_6ST_DIG),
	SND_PCI_QUIRK(0x8086, 0xa100, "Intel mobo", ALC880_FIXUP_5ST_DIG),
	SND_PCI_QUIRK(0x8086, 0xd400, "Intel mobo", ALC880_FIXUP_5ST_DIG),
	SND_PCI_QUIRK(0x8086, 0xd401, "Intel mobo", ALC880_FIXUP_5ST_DIG),
	SND_PCI_QUIRK(0x8086, 0xd402, "Intel mobo", ALC880_FIXUP_3ST_DIG),
	SND_PCI_QUIRK(0x8086, 0xe224, "Intel mobo", ALC880_FIXUP_5ST_DIG),
	SND_PCI_QUIRK(0x8086, 0xe305, "Intel mobo", ALC880_FIXUP_3ST_DIG),
	SND_PCI_QUIRK(0x8086, 0xe308, "Intel mobo", ALC880_FIXUP_3ST_DIG),
	SND_PCI_QUIRK(0x8086, 0xe400, "Intel mobo", ALC880_FIXUP_5ST_DIG),
	SND_PCI_QUIRK(0x8086, 0xe401, "Intel mobo", ALC880_FIXUP_5ST_DIG),
	SND_PCI_QUIRK(0x8086, 0xe402, "Intel mobo", ALC880_FIXUP_5ST_DIG),
	/* default Intel */
	SND_PCI_QUIRK_VENDOR(0x8086, "Intel mobo", ALC880_FIXUP_3ST),
	SND_PCI_QUIRK(0xa0a0, 0x0560, "AOpen i915GMm-HFS", ALC880_FIXUP_5ST_DIG),
	SND_PCI_QUIRK(0xe803, 0x1019, NULL, ALC880_FIXUP_6ST_DIG),
	{}
};

<<<<<<< HEAD
static const struct alc_model_fixup alc880_fixup_models[] = {
=======
static const struct hda_model_fixup alc880_fixup_models[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{.id = ALC880_FIXUP_3ST, .name = "3stack"},
	{.id = ALC880_FIXUP_3ST_DIG, .name = "3stack-digout"},
	{.id = ALC880_FIXUP_5ST, .name = "5stack"},
	{.id = ALC880_FIXUP_5ST_DIG, .name = "5stack-digout"},
	{.id = ALC880_FIXUP_6ST, .name = "6stack"},
	{.id = ALC880_FIXUP_6ST_DIG, .name = "6stack-digout"},
<<<<<<< HEAD
=======
	{.id = ALC880_FIXUP_6ST_AUTOMUTE, .name = "6stack-automute"},
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{}
};


/*
 * OK, here we have finally the patch for ALC880
 */
static int patch_alc880(struct hda_codec *codec)
{
	struct alc_spec *spec;
	int err;

<<<<<<< HEAD
	spec = kzalloc(sizeof(*spec), GFP_KERNEL);
	if (spec == NULL)
		return -ENOMEM;

	codec->spec = spec;

	spec->mixer_nid = 0x0b;
	spec->need_dac_fix = 1;

	alc_pick_fixup(codec, alc880_fixup_models, alc880_fixup_tbl,
		       alc880_fixups);
	alc_apply_fixup(codec, ALC_FIXUP_ACT_PRE_PROBE);
=======
	err = alc_alloc_spec(codec, 0x0b);
	if (err < 0)
		return err;

	spec = codec->spec;
	spec->gen.need_dac_fix = 1;
	spec->gen.beep_nid = 0x01;

	codec->patch_ops.unsol_event = alc880_unsol_event;

	alc_pre_init(codec);

	snd_hda_pick_fixup(codec, alc880_fixup_models, alc880_fixup_tbl,
		       alc880_fixups);
	snd_hda_apply_fixup(codec, HDA_FIXUP_ACT_PRE_PROBE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* automatic parse from the BIOS config */
	err = alc880_parse_auto_config(codec);
	if (err < 0)
		goto error;

<<<<<<< HEAD
	if (!spec->no_analog) {
		err = snd_hda_attach_beep_device(codec, 0x1);
		if (err < 0)
			goto error;
		set_beep_amp(spec, 0x0b, 0x05, HDA_INPUT);
	}

	codec->patch_ops = alc_patch_ops;

	alc_apply_fixup(codec, ALC_FIXUP_ACT_PROBE);
=======
	if (!spec->gen.no_analog) {
		err = set_beep_amp(spec, 0x0b, 0x05, HDA_INPUT);
		if (err < 0)
			goto error;
	}

	snd_hda_apply_fixup(codec, HDA_FIXUP_ACT_PROBE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;

 error:
	alc_free(codec);
	return err;
}


/*
 * ALC260 support
 */
static int alc260_parse_auto_config(struct hda_codec *codec)
{
	static const hda_nid_t alc260_ignore[] = { 0x17, 0 };
	static const hda_nid_t alc260_ssids[] = { 0x10, 0x15, 0x0f, 0 };
	return alc_parse_auto_config(codec, alc260_ignore, alc260_ssids);
}

/*
 * Pin config fixes
 */
enum {
	ALC260_FIXUP_HP_DC5750,
	ALC260_FIXUP_HP_PIN_0F,
	ALC260_FIXUP_COEF,
	ALC260_FIXUP_GPIO1,
	ALC260_FIXUP_GPIO1_TOGGLE,
	ALC260_FIXUP_REPLACER,
	ALC260_FIXUP_HP_B1900,
	ALC260_FIXUP_KN1,
<<<<<<< HEAD
=======
	ALC260_FIXUP_FSC_S7020,
	ALC260_FIXUP_FSC_S7020_JWSE,
	ALC260_FIXUP_VAIO_PINS,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static void alc260_gpio1_automute(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
<<<<<<< HEAD
	snd_hda_codec_write(codec, 0x01, 0, AC_VERB_SET_GPIO_DATA,
			    spec->hp_jack_present);
}

static void alc260_fixup_gpio1_toggle(struct hda_codec *codec,
				      const struct alc_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;
	if (action == ALC_FIXUP_ACT_PROBE) {
		/* although the machine has only one output pin, we need to
		 * toggle GPIO1 according to the jack state
		 */
		spec->automute_hook = alc260_gpio1_automute;
		spec->detect_hp = 1;
		spec->automute_speaker = 1;
		spec->autocfg.hp_pins[0] = 0x0f; /* copy it for automute */
		snd_hda_jack_detect_enable(codec, 0x0f, ALC_HP_EVENT);
		spec->unsol_event = alc_sku_unsol_event;
		add_verb(codec->spec, alc_gpio1_init_verbs);
=======

	alc_update_gpio_data(codec, 0x01, spec->gen.hp_jack_present);
}

static void alc260_fixup_gpio1_toggle(struct hda_codec *codec,
				      const struct hda_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;
	if (action == HDA_FIXUP_ACT_PROBE) {
		/* although the machine has only one output pin, we need to
		 * toggle GPIO1 according to the jack state
		 */
		spec->gen.automute_hook = alc260_gpio1_automute;
		spec->gen.detect_hp = 1;
		spec->gen.automute_speaker = 1;
		spec->gen.autocfg.hp_pins[0] = 0x0f; /* copy it for automute */
		snd_hda_jack_detect_enable_callback(codec, 0x0f,
						    snd_hda_gen_hp_automute);
		alc_setup_gpio(codec, 0x01);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

static void alc260_fixup_kn1(struct hda_codec *codec,
<<<<<<< HEAD
			     const struct alc_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;
	static const struct alc_pincfg pincfgs[] = {
=======
			     const struct hda_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;
	static const struct hda_pintbl pincfgs[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		{ 0x0f, 0x02214000 }, /* HP/speaker */
		{ 0x12, 0x90a60160 }, /* int mic */
		{ 0x13, 0x02a19000 }, /* ext mic */
		{ 0x18, 0x01446000 }, /* SPDIF out */
		/* disable bogus I/O pins */
		{ 0x10, 0x411111f0 },
		{ 0x11, 0x411111f0 },
		{ 0x14, 0x411111f0 },
		{ 0x15, 0x411111f0 },
		{ 0x16, 0x411111f0 },
		{ 0x17, 0x411111f0 },
		{ 0x19, 0x411111f0 },
		{ }
	};

	switch (action) {
<<<<<<< HEAD
	case ALC_FIXUP_ACT_PRE_PROBE:
		alc_apply_pincfgs(codec, pincfgs);
		break;
	case ALC_FIXUP_ACT_PROBE:
=======
	case HDA_FIXUP_ACT_PRE_PROBE:
		snd_hda_apply_pincfgs(codec, pincfgs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		spec->init_amp = ALC_INIT_NONE;
		break;
	}
}

<<<<<<< HEAD
static const struct alc_fixup alc260_fixups[] = {
	[ALC260_FIXUP_HP_DC5750] = {
		.type = ALC_FIXUP_PINS,
		.v.pins = (const struct alc_pincfg[]) {
=======
static void alc260_fixup_fsc_s7020(struct hda_codec *codec,
				   const struct hda_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;
	if (action == HDA_FIXUP_ACT_PRE_PROBE)
		spec->init_amp = ALC_INIT_NONE;
}

static void alc260_fixup_fsc_s7020_jwse(struct hda_codec *codec,
				   const struct hda_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;
	if (action == HDA_FIXUP_ACT_PRE_PROBE) {
		spec->gen.add_jack_modes = 1;
		spec->gen.hp_mic = 1;
	}
}

static const struct hda_fixup alc260_fixups[] = {
	[ALC260_FIXUP_HP_DC5750] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			{ 0x11, 0x90130110 }, /* speaker */
			{ }
		}
	},
	[ALC260_FIXUP_HP_PIN_0F] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_PINS,
		.v.pins = (const struct alc_pincfg[]) {
=======
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			{ 0x0f, 0x01214000 }, /* HP */
			{ }
		}
	},
	[ALC260_FIXUP_COEF] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_VERBS,
=======
		.type = HDA_FIXUP_VERBS,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.v.verbs = (const struct hda_verb[]) {
			{ 0x1a, AC_VERB_SET_COEF_INDEX, 0x07 },
			{ 0x1a, AC_VERB_SET_PROC_COEF,  0x3040 },
			{ }
		},
	},
	[ALC260_FIXUP_GPIO1] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_VERBS,
		.v.verbs = alc_gpio1_init_verbs,
	},
	[ALC260_FIXUP_GPIO1_TOGGLE] = {
		.type = ALC_FIXUP_FUNC,
=======
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_gpio1,
	},
	[ALC260_FIXUP_GPIO1_TOGGLE] = {
		.type = HDA_FIXUP_FUNC,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.v.func = alc260_fixup_gpio1_toggle,
		.chained = true,
		.chain_id = ALC260_FIXUP_HP_PIN_0F,
	},
	[ALC260_FIXUP_REPLACER] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_VERBS,
=======
		.type = HDA_FIXUP_VERBS,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.v.verbs = (const struct hda_verb[]) {
			{ 0x1a, AC_VERB_SET_COEF_INDEX, 0x07 },
			{ 0x1a, AC_VERB_SET_PROC_COEF,  0x3050 },
			{ }
		},
		.chained = true,
		.chain_id = ALC260_FIXUP_GPIO1_TOGGLE,
	},
	[ALC260_FIXUP_HP_B1900] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_FUNC,
=======
		.type = HDA_FIXUP_FUNC,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.v.func = alc260_fixup_gpio1_toggle,
		.chained = true,
		.chain_id = ALC260_FIXUP_COEF,
	},
	[ALC260_FIXUP_KN1] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_FUNC,
		.v.func = alc260_fixup_kn1,
	},
=======
		.type = HDA_FIXUP_FUNC,
		.v.func = alc260_fixup_kn1,
	},
	[ALC260_FIXUP_FSC_S7020] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc260_fixup_fsc_s7020,
	},
	[ALC260_FIXUP_FSC_S7020_JWSE] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc260_fixup_fsc_s7020_jwse,
		.chained = true,
		.chain_id = ALC260_FIXUP_FSC_S7020,
	},
	[ALC260_FIXUP_VAIO_PINS] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			/* Pin configs are missing completely on some VAIOs */
			{ 0x0f, 0x01211020 },
			{ 0x10, 0x0001003f },
			{ 0x11, 0x411111f0 },
			{ 0x12, 0x01a15930 },
			{ 0x13, 0x411111f0 },
			{ 0x14, 0x411111f0 },
			{ 0x15, 0x411111f0 },
			{ 0x16, 0x411111f0 },
			{ 0x17, 0x411111f0 },
			{ 0x18, 0x411111f0 },
			{ 0x19, 0x411111f0 },
			{ }
		}
	},
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static const struct snd_pci_quirk alc260_fixup_tbl[] = {
	SND_PCI_QUIRK(0x1025, 0x007b, "Acer C20x", ALC260_FIXUP_GPIO1),
	SND_PCI_QUIRK(0x1025, 0x007f, "Acer Aspire 9500", ALC260_FIXUP_COEF),
	SND_PCI_QUIRK(0x1025, 0x008f, "Acer", ALC260_FIXUP_GPIO1),
	SND_PCI_QUIRK(0x103c, 0x280a, "HP dc5750", ALC260_FIXUP_HP_DC5750),
	SND_PCI_QUIRK(0x103c, 0x30ba, "HP Presario B1900", ALC260_FIXUP_HP_B1900),
<<<<<<< HEAD
=======
	SND_PCI_QUIRK(0x104d, 0x81bb, "Sony VAIO", ALC260_FIXUP_VAIO_PINS),
	SND_PCI_QUIRK(0x104d, 0x81e2, "Sony VAIO TX", ALC260_FIXUP_HP_PIN_0F),
	SND_PCI_QUIRK(0x10cf, 0x1326, "FSC LifeBook S7020", ALC260_FIXUP_FSC_S7020),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	SND_PCI_QUIRK(0x1509, 0x4540, "Favorit 100XS", ALC260_FIXUP_GPIO1),
	SND_PCI_QUIRK(0x152d, 0x0729, "Quanta KN1", ALC260_FIXUP_KN1),
	SND_PCI_QUIRK(0x161f, 0x2057, "Replacer 672V", ALC260_FIXUP_REPLACER),
	SND_PCI_QUIRK(0x1631, 0xc017, "PB V7900", ALC260_FIXUP_COEF),
	{}
};

<<<<<<< HEAD
=======
static const struct hda_model_fixup alc260_fixup_models[] = {
	{.id = ALC260_FIXUP_GPIO1, .name = "gpio1"},
	{.id = ALC260_FIXUP_COEF, .name = "coef"},
	{.id = ALC260_FIXUP_FSC_S7020, .name = "fujitsu"},
	{.id = ALC260_FIXUP_FSC_S7020_JWSE, .name = "fujitsu-jwse"},
	{}
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 */
static int patch_alc260(struct hda_codec *codec)
{
	struct alc_spec *spec;
	int err;

<<<<<<< HEAD
	spec = kzalloc(sizeof(*spec), GFP_KERNEL);
	if (spec == NULL)
		return -ENOMEM;

	codec->spec = spec;

	spec->mixer_nid = 0x07;

	alc_pick_fixup(codec, NULL, alc260_fixup_tbl, alc260_fixups);
	alc_apply_fixup(codec, ALC_FIXUP_ACT_PRE_PROBE);
=======
	err = alc_alloc_spec(codec, 0x07);
	if (err < 0)
		return err;

	spec = codec->spec;
	/* as quite a few machines require HP amp for speaker outputs,
	 * it's easier to enable it unconditionally; even if it's unneeded,
	 * it's almost harmless.
	 */
	spec->gen.prefer_hp_amp = 1;
	spec->gen.beep_nid = 0x01;

	spec->shutup = alc_eapd_shutup;

	alc_pre_init(codec);

	snd_hda_pick_fixup(codec, alc260_fixup_models, alc260_fixup_tbl,
			   alc260_fixups);
	snd_hda_apply_fixup(codec, HDA_FIXUP_ACT_PRE_PROBE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* automatic parse from the BIOS config */
	err = alc260_parse_auto_config(codec);
	if (err < 0)
		goto error;

<<<<<<< HEAD
	if (!spec->no_analog) {
		err = snd_hda_attach_beep_device(codec, 0x1);
		if (err < 0)
			goto error;
		set_beep_amp(spec, 0x07, 0x05, HDA_INPUT);
	}

	codec->patch_ops = alc_patch_ops;
	spec->shutup = alc_eapd_shutup;

	alc_apply_fixup(codec, ALC_FIXUP_ACT_PROBE);
=======
	if (!spec->gen.no_analog) {
		err = set_beep_amp(spec, 0x07, 0x05, HDA_INPUT);
		if (err < 0)
			goto error;
	}

	snd_hda_apply_fixup(codec, HDA_FIXUP_ACT_PROBE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;

 error:
	alc_free(codec);
	return err;
}


/*
 * ALC882/883/885/888/889 support
 *
 * ALC882 is almost identical with ALC880 but has cleaner and more flexible
 * configuration.  Each pin widget can choose any input DACs and a mixer.
 * Each ADC is connected from a mixer of all inputs.  This makes possible
 * 6-channel independent captures.
 *
 * In addition, an independent DAC for the multi-playback (not used in this
 * driver yet).
 */

/*
 * Pin config fixes
 */
enum {
	ALC882_FIXUP_ABIT_AW9D_MAX,
	ALC882_FIXUP_LENOVO_Y530,
	ALC882_FIXUP_PB_M5210,
	ALC882_FIXUP_ACER_ASPIRE_7736,
	ALC882_FIXUP_ASUS_W90V,
	ALC889_FIXUP_CD,
<<<<<<< HEAD
	ALC889_FIXUP_VAIO_TT,
	ALC888_FIXUP_EEE1601,
=======
	ALC889_FIXUP_FRONT_HP_NO_PRESENCE,
	ALC889_FIXUP_VAIO_TT,
	ALC888_FIXUP_EEE1601,
	ALC886_FIXUP_EAPD,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ALC882_FIXUP_EAPD,
	ALC883_FIXUP_EAPD,
	ALC883_FIXUP_ACER_EAPD,
	ALC882_FIXUP_GPIO1,
	ALC882_FIXUP_GPIO2,
	ALC882_FIXUP_GPIO3,
	ALC889_FIXUP_COEF,
	ALC882_FIXUP_ASUS_W2JC,
	ALC882_FIXUP_ACER_ASPIRE_4930G,
	ALC882_FIXUP_ACER_ASPIRE_8930G,
	ALC882_FIXUP_ASPIRE_8930G_VERBS,
	ALC885_FIXUP_MACPRO_GPIO,
	ALC889_FIXUP_DAC_ROUTE,
	ALC889_FIXUP_MBP_VREF,
	ALC889_FIXUP_IMAC91_VREF,
<<<<<<< HEAD
	ALC882_FIXUP_NO_PRIMARY_HP,
};

static void alc889_fixup_coef(struct hda_codec *codec,
			      const struct alc_fixup *fix, int action)
{
	if (action != ALC_FIXUP_ACT_INIT)
		return;
	alc889_coef_init(codec);
}

/* toggle speaker-output according to the hp-jack state */
static void alc882_gpio_mute(struct hda_codec *codec, int pin, int muted)
{
	unsigned int gpiostate, gpiomask, gpiodir;

	gpiostate = snd_hda_codec_read(codec, codec->afg, 0,
				       AC_VERB_GET_GPIO_DATA, 0);

	if (!muted)
		gpiostate |= (1 << pin);
	else
		gpiostate &= ~(1 << pin);

	gpiomask = snd_hda_codec_read(codec, codec->afg, 0,
				      AC_VERB_GET_GPIO_MASK, 0);
	gpiomask |= (1 << pin);

	gpiodir = snd_hda_codec_read(codec, codec->afg, 0,
				     AC_VERB_GET_GPIO_DIRECTION, 0);
	gpiodir |= (1 << pin);


	snd_hda_codec_write(codec, codec->afg, 0,
			    AC_VERB_SET_GPIO_MASK, gpiomask);
	snd_hda_codec_write(codec, codec->afg, 0,
			    AC_VERB_SET_GPIO_DIRECTION, gpiodir);

	msleep(1);

	snd_hda_codec_write(codec, codec->afg, 0,
			    AC_VERB_SET_GPIO_DATA, gpiostate);
=======
	ALC889_FIXUP_MBA11_VREF,
	ALC889_FIXUP_MBA21_VREF,
	ALC889_FIXUP_MP11_VREF,
	ALC889_FIXUP_MP41_VREF,
	ALC882_FIXUP_INV_DMIC,
	ALC882_FIXUP_NO_PRIMARY_HP,
	ALC887_FIXUP_ASUS_BASS,
	ALC887_FIXUP_BASS_CHMAP,
	ALC1220_FIXUP_GB_DUAL_CODECS,
	ALC1220_FIXUP_GB_X570,
	ALC1220_FIXUP_CLEVO_P950,
	ALC1220_FIXUP_CLEVO_PB51ED,
	ALC1220_FIXUP_CLEVO_PB51ED_PINS,
	ALC887_FIXUP_ASUS_AUDIO,
	ALC887_FIXUP_ASUS_HMIC,
	ALCS1200A_FIXUP_MIC_VREF,
	ALC888VD_FIXUP_MIC_100VREF,
};

static void alc889_fixup_coef(struct hda_codec *codec,
			      const struct hda_fixup *fix, int action)
{
	if (action != HDA_FIXUP_ACT_INIT)
		return;
	alc_update_coef_idx(codec, 7, 0, 0x2030);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* set up GPIO at initialization */
static void alc885_fixup_macpro_gpio(struct hda_codec *codec,
<<<<<<< HEAD
				     const struct alc_fixup *fix, int action)
{
	if (action != ALC_FIXUP_ACT_INIT)
		return;
	alc882_gpio_mute(codec, 0, 0);
	alc882_gpio_mute(codec, 1, 0);
=======
				     const struct hda_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;

	spec->gpio_write_delay = true;
	alc_fixup_gpio3(codec, fix, action);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Fix the connection of some pins for ALC889:
 * At least, Acer Aspire 5935 shows the connections to DAC3/4 don't
 * work correctly (bko#42740)
 */
static void alc889_fixup_dac_route(struct hda_codec *codec,
<<<<<<< HEAD
				   const struct alc_fixup *fix, int action)
{
	if (action == ALC_FIXUP_ACT_PRE_PROBE) {
		/* fake the connections during parsing the tree */
		hda_nid_t conn1[2] = { 0x0c, 0x0d };
		hda_nid_t conn2[2] = { 0x0e, 0x0f };
		snd_hda_override_conn_list(codec, 0x14, 2, conn1);
		snd_hda_override_conn_list(codec, 0x15, 2, conn1);
		snd_hda_override_conn_list(codec, 0x18, 2, conn2);
		snd_hda_override_conn_list(codec, 0x1a, 2, conn2);
	} else if (action == ALC_FIXUP_ACT_PROBE) {
		/* restore the connections */
		hda_nid_t conn[5] = { 0x0c, 0x0d, 0x0e, 0x0f, 0x26 };
		snd_hda_override_conn_list(codec, 0x14, 5, conn);
		snd_hda_override_conn_list(codec, 0x15, 5, conn);
		snd_hda_override_conn_list(codec, 0x18, 5, conn);
		snd_hda_override_conn_list(codec, 0x1a, 5, conn);
=======
				   const struct hda_fixup *fix, int action)
{
	if (action == HDA_FIXUP_ACT_PRE_PROBE) {
		/* fake the connections during parsing the tree */
		static const hda_nid_t conn1[] = { 0x0c, 0x0d };
		static const hda_nid_t conn2[] = { 0x0e, 0x0f };
		snd_hda_override_conn_list(codec, 0x14, ARRAY_SIZE(conn1), conn1);
		snd_hda_override_conn_list(codec, 0x15, ARRAY_SIZE(conn1), conn1);
		snd_hda_override_conn_list(codec, 0x18, ARRAY_SIZE(conn2), conn2);
		snd_hda_override_conn_list(codec, 0x1a, ARRAY_SIZE(conn2), conn2);
	} else if (action == HDA_FIXUP_ACT_PROBE) {
		/* restore the connections */
		static const hda_nid_t conn[] = { 0x0c, 0x0d, 0x0e, 0x0f, 0x26 };
		snd_hda_override_conn_list(codec, 0x14, ARRAY_SIZE(conn), conn);
		snd_hda_override_conn_list(codec, 0x15, ARRAY_SIZE(conn), conn);
		snd_hda_override_conn_list(codec, 0x18, ARRAY_SIZE(conn), conn);
		snd_hda_override_conn_list(codec, 0x1a, ARRAY_SIZE(conn), conn);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

/* Set VREF on HP pin */
static void alc889_fixup_mbp_vref(struct hda_codec *codec,
<<<<<<< HEAD
				  const struct alc_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;
	static hda_nid_t nids[2] = { 0x14, 0x15 };
	int i;

	if (action != ALC_FIXUP_ACT_INIT)
=======
				  const struct hda_fixup *fix, int action)
{
	static const hda_nid_t nids[] = { 0x14, 0x15, 0x19 };
	struct alc_spec *spec = codec->spec;
	int i;

	if (action != HDA_FIXUP_ACT_INIT)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;
	for (i = 0; i < ARRAY_SIZE(nids); i++) {
		unsigned int val = snd_hda_codec_get_pincfg(codec, nids[i]);
		if (get_defcfg_device(val) != AC_JACK_HP_OUT)
			continue;
<<<<<<< HEAD
		val = snd_hda_codec_read(codec, nids[i], 0,
					 AC_VERB_GET_PIN_WIDGET_CONTROL, 0);
		val |= AC_PINCTL_VREF_80;
		snd_hda_codec_write(codec, nids[i], 0,
				    AC_VERB_SET_PIN_WIDGET_CONTROL, val);
		spec->keep_vref_in_automute = 1;
=======
		val = snd_hda_codec_get_pin_target(codec, nids[i]);
		val |= AC_PINCTL_VREF_80;
		snd_hda_set_pin_ctl(codec, nids[i], val);
		spec->gen.keep_vref_in_automute = 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	}
}

<<<<<<< HEAD
/* Set VREF on speaker pins on imac91 */
static void alc889_fixup_imac91_vref(struct hda_codec *codec,
				     const struct alc_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;
	static hda_nid_t nids[2] = { 0x18, 0x1a };
	int i;

	if (action != ALC_FIXUP_ACT_INIT)
		return;
	for (i = 0; i < ARRAY_SIZE(nids); i++) {
		unsigned int val;
		val = snd_hda_codec_read(codec, nids[i], 0,
					 AC_VERB_GET_PIN_WIDGET_CONTROL, 0);
		val |= AC_PINCTL_VREF_50;
		snd_hda_codec_write(codec, nids[i], 0,
				    AC_VERB_SET_PIN_WIDGET_CONTROL, val);
	}
	spec->keep_vref_in_automute = 1;
}

/* Don't take HP output as primary
 * strangely, the speaker output doesn't work on VAIO Z through DAC 0x05
 */
static void alc882_fixup_no_primary_hp(struct hda_codec *codec,
				       const struct alc_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;
	if (action == ALC_FIXUP_ACT_PRE_PROBE)
		spec->no_primary_hp = 1;
}

static const struct alc_fixup alc882_fixups[] = {
	[ALC882_FIXUP_ABIT_AW9D_MAX] = {
		.type = ALC_FIXUP_PINS,
		.v.pins = (const struct alc_pincfg[]) {
=======
static void alc889_fixup_mac_pins(struct hda_codec *codec,
				  const hda_nid_t *nids, int num_nids)
{
	struct alc_spec *spec = codec->spec;
	int i;

	for (i = 0; i < num_nids; i++) {
		unsigned int val;
		val = snd_hda_codec_get_pin_target(codec, nids[i]);
		val |= AC_PINCTL_VREF_50;
		snd_hda_set_pin_ctl(codec, nids[i], val);
	}
	spec->gen.keep_vref_in_automute = 1;
}

/* Set VREF on speaker pins on imac91 */
static void alc889_fixup_imac91_vref(struct hda_codec *codec,
				     const struct hda_fixup *fix, int action)
{
	static const hda_nid_t nids[] = { 0x18, 0x1a };

	if (action == HDA_FIXUP_ACT_INIT)
		alc889_fixup_mac_pins(codec, nids, ARRAY_SIZE(nids));
}

/* Set VREF on speaker pins on mba11 */
static void alc889_fixup_mba11_vref(struct hda_codec *codec,
				    const struct hda_fixup *fix, int action)
{
	static const hda_nid_t nids[] = { 0x18 };

	if (action == HDA_FIXUP_ACT_INIT)
		alc889_fixup_mac_pins(codec, nids, ARRAY_SIZE(nids));
}

/* Set VREF on speaker pins on mba21 */
static void alc889_fixup_mba21_vref(struct hda_codec *codec,
				    const struct hda_fixup *fix, int action)
{
	static const hda_nid_t nids[] = { 0x18, 0x19 };

	if (action == HDA_FIXUP_ACT_INIT)
		alc889_fixup_mac_pins(codec, nids, ARRAY_SIZE(nids));
}

/* Don't take HP output as primary
 * Strangely, the speaker output doesn't work on Vaio Z and some Vaio
 * all-in-one desktop PCs (for example VGC-LN51JGB) through DAC 0x05
 */
static void alc882_fixup_no_primary_hp(struct hda_codec *codec,
				       const struct hda_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;
	if (action == HDA_FIXUP_ACT_PRE_PROBE) {
		spec->gen.no_primary_hp = 1;
		spec->gen.no_multi_io = 1;
	}
}

static void alc_fixup_bass_chmap(struct hda_codec *codec,
				 const struct hda_fixup *fix, int action);

/* For dual-codec configuration, we need to disable some features to avoid
 * conflicts of kctls and PCM streams
 */
static void alc_fixup_dual_codecs(struct hda_codec *codec,
				  const struct hda_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;

	if (action != HDA_FIXUP_ACT_PRE_PROBE)
		return;
	/* disable vmaster */
	spec->gen.suppress_vmaster = 1;
	/* auto-mute and auto-mic switch don't work with multiple codecs */
	spec->gen.suppress_auto_mute = 1;
	spec->gen.suppress_auto_mic = 1;
	/* disable aamix as well */
	spec->gen.mixer_nid = 0;
	/* add location prefix to avoid conflicts */
	codec->force_pin_prefix = 1;
}

static void rename_ctl(struct hda_codec *codec, const char *oldname,
		       const char *newname)
{
	struct snd_kcontrol *kctl;

	kctl = snd_hda_find_mixer_ctl(codec, oldname);
	if (kctl)
		snd_ctl_rename(codec->card, kctl, newname);
}

static void alc1220_fixup_gb_dual_codecs(struct hda_codec *codec,
					 const struct hda_fixup *fix,
					 int action)
{
	alc_fixup_dual_codecs(codec, fix, action);
	switch (action) {
	case HDA_FIXUP_ACT_PRE_PROBE:
		/* override card longname to provide a unique UCM profile */
		strcpy(codec->card->longname, "HDAudio-Gigabyte-ALC1220DualCodecs");
		break;
	case HDA_FIXUP_ACT_BUILD:
		/* rename Capture controls depending on the codec */
		rename_ctl(codec, "Capture Volume",
			   codec->addr == 0 ?
			   "Rear-Panel Capture Volume" :
			   "Front-Panel Capture Volume");
		rename_ctl(codec, "Capture Switch",
			   codec->addr == 0 ?
			   "Rear-Panel Capture Switch" :
			   "Front-Panel Capture Switch");
		break;
	}
}

static void alc1220_fixup_gb_x570(struct hda_codec *codec,
				     const struct hda_fixup *fix,
				     int action)
{
	static const hda_nid_t conn1[] = { 0x0c };
	static const struct coef_fw gb_x570_coefs[] = {
		WRITE_COEF(0x07, 0x03c0),
		WRITE_COEF(0x1a, 0x01c1),
		WRITE_COEF(0x1b, 0x0202),
		WRITE_COEF(0x43, 0x3005),
		{}
	};

	switch (action) {
	case HDA_FIXUP_ACT_PRE_PROBE:
		snd_hda_override_conn_list(codec, 0x14, ARRAY_SIZE(conn1), conn1);
		snd_hda_override_conn_list(codec, 0x1b, ARRAY_SIZE(conn1), conn1);
		break;
	case HDA_FIXUP_ACT_INIT:
		alc_process_coef_fw(codec, gb_x570_coefs);
		break;
	}
}

static void alc1220_fixup_clevo_p950(struct hda_codec *codec,
				     const struct hda_fixup *fix,
				     int action)
{
	static const hda_nid_t conn1[] = { 0x0c };

	if (action != HDA_FIXUP_ACT_PRE_PROBE)
		return;

	alc_update_coef_idx(codec, 0x7, 0, 0x3c3);
	/* We therefore want to make sure 0x14 (front headphone) and
	 * 0x1b (speakers) use the stereo DAC 0x02
	 */
	snd_hda_override_conn_list(codec, 0x14, ARRAY_SIZE(conn1), conn1);
	snd_hda_override_conn_list(codec, 0x1b, ARRAY_SIZE(conn1), conn1);
}

static void alc_fixup_headset_mode_no_hp_mic(struct hda_codec *codec,
				const struct hda_fixup *fix, int action);

static void alc1220_fixup_clevo_pb51ed(struct hda_codec *codec,
				     const struct hda_fixup *fix,
				     int action)
{
	alc1220_fixup_clevo_p950(codec, fix, action);
	alc_fixup_headset_mode_no_hp_mic(codec, fix, action);
}

static void alc887_asus_hp_automute_hook(struct hda_codec *codec,
					 struct hda_jack_callback *jack)
{
	struct alc_spec *spec = codec->spec;
	unsigned int vref;

	snd_hda_gen_hp_automute(codec, jack);

	if (spec->gen.hp_jack_present)
		vref = AC_PINCTL_VREF_80;
	else
		vref = AC_PINCTL_VREF_HIZ;
	snd_hda_set_pin_ctl(codec, 0x19, PIN_HP | vref);
}

static void alc887_fixup_asus_jack(struct hda_codec *codec,
				     const struct hda_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;
	if (action != HDA_FIXUP_ACT_PROBE)
		return;
	snd_hda_set_pin_ctl_cache(codec, 0x1b, PIN_HP);
	spec->gen.hp_automute_hook = alc887_asus_hp_automute_hook;
}

static const struct hda_fixup alc882_fixups[] = {
	[ALC882_FIXUP_ABIT_AW9D_MAX] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			{ 0x15, 0x01080104 }, /* side */
			{ 0x16, 0x01011012 }, /* rear */
			{ 0x17, 0x01016011 }, /* clfe */
			{ }
		}
	},
	[ALC882_FIXUP_LENOVO_Y530] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_PINS,
		.v.pins = (const struct alc_pincfg[]) {
=======
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			{ 0x15, 0x99130112 }, /* rear int speakers */
			{ 0x16, 0x99130111 }, /* subwoofer */
			{ }
		}
	},
	[ALC882_FIXUP_PB_M5210] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_VERBS,
		.v.verbs = (const struct hda_verb[]) {
			{ 0x19, AC_VERB_SET_PIN_WIDGET_CONTROL, PIN_VREF50 },
=======
		.type = HDA_FIXUP_PINCTLS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x19, PIN_VREF50 },
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			{}
		}
	},
	[ALC882_FIXUP_ACER_ASPIRE_7736] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_SKU,
		.v.sku = ALC_FIXUP_SKU_IGNORE,
	},
	[ALC882_FIXUP_ASUS_W90V] = {
		.type = ALC_FIXUP_PINS,
		.v.pins = (const struct alc_pincfg[]) {
=======
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_sku_ignore,
	},
	[ALC882_FIXUP_ASUS_W90V] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			{ 0x16, 0x99130110 }, /* fix sequence for CLFE */
			{ }
		}
	},
	[ALC889_FIXUP_CD] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_PINS,
		.v.pins = (const struct alc_pincfg[]) {
=======
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			{ 0x1c, 0x993301f0 }, /* CD */
			{ }
		}
	},
<<<<<<< HEAD
	[ALC889_FIXUP_VAIO_TT] = {
		.type = ALC_FIXUP_PINS,
		.v.pins = (const struct alc_pincfg[]) {
=======
	[ALC889_FIXUP_FRONT_HP_NO_PRESENCE] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x1b, 0x02214120 }, /* Front HP jack is flaky, disable jack detect */
			{ }
		},
		.chained = true,
		.chain_id = ALC889_FIXUP_CD,
	},
	[ALC889_FIXUP_VAIO_TT] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			{ 0x17, 0x90170111 }, /* hidden surround speaker */
			{ }
		}
	},
	[ALC888_FIXUP_EEE1601] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_VERBS,
=======
		.type = HDA_FIXUP_VERBS,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.v.verbs = (const struct hda_verb[]) {
			{ 0x20, AC_VERB_SET_COEF_INDEX, 0x0b },
			{ 0x20, AC_VERB_SET_PROC_COEF,  0x0838 },
			{ }
		}
	},
<<<<<<< HEAD
	[ALC882_FIXUP_EAPD] = {
		.type = ALC_FIXUP_VERBS,
=======
	[ALC886_FIXUP_EAPD] = {
		.type = HDA_FIXUP_VERBS,
		.v.verbs = (const struct hda_verb[]) {
			/* change to EAPD mode */
			{ 0x20, AC_VERB_SET_COEF_INDEX, 0x07 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x0068 },
			{ }
		}
	},
	[ALC882_FIXUP_EAPD] = {
		.type = HDA_FIXUP_VERBS,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.v.verbs = (const struct hda_verb[]) {
			/* change to EAPD mode */
			{ 0x20, AC_VERB_SET_COEF_INDEX, 0x07 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x3060 },
			{ }
		}
	},
	[ALC883_FIXUP_EAPD] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_VERBS,
=======
		.type = HDA_FIXUP_VERBS,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.v.verbs = (const struct hda_verb[]) {
			/* change to EAPD mode */
			{ 0x20, AC_VERB_SET_COEF_INDEX, 0x07 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x3070 },
			{ }
		}
	},
	[ALC883_FIXUP_ACER_EAPD] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_VERBS,
=======
		.type = HDA_FIXUP_VERBS,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.v.verbs = (const struct hda_verb[]) {
			/* eanable EAPD on Acer laptops */
			{ 0x20, AC_VERB_SET_COEF_INDEX, 0x07 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x3050 },
			{ }
		}
	},
	[ALC882_FIXUP_GPIO1] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_VERBS,
		.v.verbs = alc_gpio1_init_verbs,
	},
	[ALC882_FIXUP_GPIO2] = {
		.type = ALC_FIXUP_VERBS,
		.v.verbs = alc_gpio2_init_verbs,
	},
	[ALC882_FIXUP_GPIO3] = {
		.type = ALC_FIXUP_VERBS,
		.v.verbs = alc_gpio3_init_verbs,
	},
	[ALC882_FIXUP_ASUS_W2JC] = {
		.type = ALC_FIXUP_VERBS,
		.v.verbs = alc_gpio1_init_verbs,
=======
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_gpio1,
	},
	[ALC882_FIXUP_GPIO2] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_gpio2,
	},
	[ALC882_FIXUP_GPIO3] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_gpio3,
	},
	[ALC882_FIXUP_ASUS_W2JC] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_gpio1,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.chained = true,
		.chain_id = ALC882_FIXUP_EAPD,
	},
	[ALC889_FIXUP_COEF] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_FUNC,
		.v.func = alc889_fixup_coef,
	},
	[ALC882_FIXUP_ACER_ASPIRE_4930G] = {
		.type = ALC_FIXUP_PINS,
		.v.pins = (const struct alc_pincfg[]) {
=======
		.type = HDA_FIXUP_FUNC,
		.v.func = alc889_fixup_coef,
	},
	[ALC882_FIXUP_ACER_ASPIRE_4930G] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			{ 0x16, 0x99130111 }, /* CLFE speaker */
			{ 0x17, 0x99130112 }, /* surround speaker */
			{ }
		},
		.chained = true,
		.chain_id = ALC882_FIXUP_GPIO1,
	},
	[ALC882_FIXUP_ACER_ASPIRE_8930G] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_PINS,
		.v.pins = (const struct alc_pincfg[]) {
=======
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			{ 0x16, 0x99130111 }, /* CLFE speaker */
			{ 0x1b, 0x99130112 }, /* surround speaker */
			{ }
		},
		.chained = true,
		.chain_id = ALC882_FIXUP_ASPIRE_8930G_VERBS,
	},
	[ALC882_FIXUP_ASPIRE_8930G_VERBS] = {
		/* additional init verbs for Acer Aspire 8930G */
<<<<<<< HEAD
		.type = ALC_FIXUP_VERBS,
=======
		.type = HDA_FIXUP_VERBS,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.v.verbs = (const struct hda_verb[]) {
			/* Enable all DACs */
			/* DAC DISABLE/MUTE 1? */
			/*  setting bits 1-5 disables DAC nids 0x02-0x06
			 *  apparently. Init=0x38 */
			{ 0x20, AC_VERB_SET_COEF_INDEX, 0x03 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x0000 },
			/* DAC DISABLE/MUTE 2? */
			/*  some bit here disables the other DACs.
			 *  Init=0x4900 */
			{ 0x20, AC_VERB_SET_COEF_INDEX, 0x08 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x0000 },
			/* DMIC fix
			 * This laptop has a stereo digital microphone.
			 * The mics are only 1cm apart which makes the stereo
			 * useless. However, either the mic or the ALC889
			 * makes the signal become a difference/sum signal
			 * instead of standard stereo, which is annoying.
			 * So instead we flip this bit which makes the
			 * codec replicate the sum signal to both channels,
			 * turning it into a normal mono mic.
			 */
			/* DMIC_CONTROL? Init value = 0x0001 */
			{ 0x20, AC_VERB_SET_COEF_INDEX, 0x0b },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x0003 },
			{ 0x20, AC_VERB_SET_COEF_INDEX, 0x07 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x3050 },
			{ }
		},
		.chained = true,
		.chain_id = ALC882_FIXUP_GPIO1,
	},
	[ALC885_FIXUP_MACPRO_GPIO] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_FUNC,
		.v.func = alc885_fixup_macpro_gpio,
	},
	[ALC889_FIXUP_DAC_ROUTE] = {
		.type = ALC_FIXUP_FUNC,
		.v.func = alc889_fixup_dac_route,
	},
	[ALC889_FIXUP_MBP_VREF] = {
		.type = ALC_FIXUP_FUNC,
=======
		.type = HDA_FIXUP_FUNC,
		.v.func = alc885_fixup_macpro_gpio,
	},
	[ALC889_FIXUP_DAC_ROUTE] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc889_fixup_dac_route,
	},
	[ALC889_FIXUP_MBP_VREF] = {
		.type = HDA_FIXUP_FUNC,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.v.func = alc889_fixup_mbp_vref,
		.chained = true,
		.chain_id = ALC882_FIXUP_GPIO1,
	},
	[ALC889_FIXUP_IMAC91_VREF] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_FUNC,
=======
		.type = HDA_FIXUP_FUNC,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.v.func = alc889_fixup_imac91_vref,
		.chained = true,
		.chain_id = ALC882_FIXUP_GPIO1,
	},
<<<<<<< HEAD
	[ALC882_FIXUP_NO_PRIMARY_HP] = {
		.type = ALC_FIXUP_FUNC,
		.v.func = alc882_fixup_no_primary_hp,
	},
=======
	[ALC889_FIXUP_MBA11_VREF] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc889_fixup_mba11_vref,
		.chained = true,
		.chain_id = ALC889_FIXUP_MBP_VREF,
	},
	[ALC889_FIXUP_MBA21_VREF] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc889_fixup_mba21_vref,
		.chained = true,
		.chain_id = ALC889_FIXUP_MBP_VREF,
	},
	[ALC889_FIXUP_MP11_VREF] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc889_fixup_mba11_vref,
		.chained = true,
		.chain_id = ALC885_FIXUP_MACPRO_GPIO,
	},
	[ALC889_FIXUP_MP41_VREF] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc889_fixup_mbp_vref,
		.chained = true,
		.chain_id = ALC885_FIXUP_MACPRO_GPIO,
	},
	[ALC882_FIXUP_INV_DMIC] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_inv_dmic,
	},
	[ALC882_FIXUP_NO_PRIMARY_HP] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc882_fixup_no_primary_hp,
	},
	[ALC887_FIXUP_ASUS_BASS] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{0x16, 0x99130130}, /* bass speaker */
			{}
		},
		.chained = true,
		.chain_id = ALC887_FIXUP_BASS_CHMAP,
	},
	[ALC887_FIXUP_BASS_CHMAP] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_bass_chmap,
	},
	[ALC1220_FIXUP_GB_DUAL_CODECS] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc1220_fixup_gb_dual_codecs,
	},
	[ALC1220_FIXUP_GB_X570] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc1220_fixup_gb_x570,
	},
	[ALC1220_FIXUP_CLEVO_P950] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc1220_fixup_clevo_p950,
	},
	[ALC1220_FIXUP_CLEVO_PB51ED] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc1220_fixup_clevo_pb51ed,
	},
	[ALC1220_FIXUP_CLEVO_PB51ED_PINS] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x19, 0x01a1913c }, /* use as headset mic, without its own jack detect */
			{}
		},
		.chained = true,
		.chain_id = ALC1220_FIXUP_CLEVO_PB51ED,
	},
	[ALC887_FIXUP_ASUS_AUDIO] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x15, 0x02a14150 }, /* use as headset mic, without its own jack detect */
			{ 0x19, 0x22219420 },
			{}
		},
	},
	[ALC887_FIXUP_ASUS_HMIC] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc887_fixup_asus_jack,
		.chained = true,
		.chain_id = ALC887_FIXUP_ASUS_AUDIO,
	},
	[ALCS1200A_FIXUP_MIC_VREF] = {
		.type = HDA_FIXUP_PINCTLS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x18, PIN_VREF50 }, /* rear mic */
			{ 0x19, PIN_VREF50 }, /* front mic */
			{}
		}
	},
	[ALC888VD_FIXUP_MIC_100VREF] = {
		.type = HDA_FIXUP_PINCTLS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x18, PIN_VREF100 }, /* headset mic */
			{}
		}
	},
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static const struct snd_pci_quirk alc882_fixup_tbl[] = {
	SND_PCI_QUIRK(0x1025, 0x006c, "Acer Aspire 9810", ALC883_FIXUP_ACER_EAPD),
	SND_PCI_QUIRK(0x1025, 0x0090, "Acer Aspire", ALC883_FIXUP_ACER_EAPD),
	SND_PCI_QUIRK(0x1025, 0x0107, "Acer Aspire", ALC883_FIXUP_ACER_EAPD),
	SND_PCI_QUIRK(0x1025, 0x010a, "Acer Ferrari 5000", ALC883_FIXUP_ACER_EAPD),
	SND_PCI_QUIRK(0x1025, 0x0110, "Acer Aspire", ALC883_FIXUP_ACER_EAPD),
	SND_PCI_QUIRK(0x1025, 0x0112, "Acer Aspire 9303", ALC883_FIXUP_ACER_EAPD),
	SND_PCI_QUIRK(0x1025, 0x0121, "Acer Aspire 5920G", ALC883_FIXUP_ACER_EAPD),
	SND_PCI_QUIRK(0x1025, 0x013e, "Acer Aspire 4930G",
		      ALC882_FIXUP_ACER_ASPIRE_4930G),
	SND_PCI_QUIRK(0x1025, 0x013f, "Acer Aspire 5930G",
		      ALC882_FIXUP_ACER_ASPIRE_4930G),
	SND_PCI_QUIRK(0x1025, 0x0145, "Acer Aspire 8930G",
		      ALC882_FIXUP_ACER_ASPIRE_8930G),
	SND_PCI_QUIRK(0x1025, 0x0146, "Acer Aspire 6935G",
		      ALC882_FIXUP_ACER_ASPIRE_8930G),
<<<<<<< HEAD
=======
	SND_PCI_QUIRK(0x1025, 0x0142, "Acer Aspire 7730G",
		      ALC882_FIXUP_ACER_ASPIRE_4930G),
	SND_PCI_QUIRK(0x1025, 0x0155, "Packard-Bell M5120", ALC882_FIXUP_PB_M5210),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	SND_PCI_QUIRK(0x1025, 0x015e, "Acer Aspire 6930G",
		      ALC882_FIXUP_ACER_ASPIRE_4930G),
	SND_PCI_QUIRK(0x1025, 0x0166, "Acer Aspire 6530G",
		      ALC882_FIXUP_ACER_ASPIRE_4930G),
<<<<<<< HEAD
	SND_PCI_QUIRK(0x1025, 0x0142, "Acer Aspire 7730G",
		      ALC882_FIXUP_ACER_ASPIRE_4930G),
	SND_PCI_QUIRK(0x1025, 0x0155, "Packard-Bell M5120", ALC882_FIXUP_PB_M5210),
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	SND_PCI_QUIRK(0x1025, 0x021e, "Acer Aspire 5739G",
		      ALC882_FIXUP_ACER_ASPIRE_4930G),
	SND_PCI_QUIRK(0x1025, 0x0259, "Acer Aspire 5935", ALC889_FIXUP_DAC_ROUTE),
	SND_PCI_QUIRK(0x1025, 0x026b, "Acer Aspire 8940G", ALC882_FIXUP_ACER_ASPIRE_8930G),
	SND_PCI_QUIRK(0x1025, 0x0296, "Acer Aspire 7736z", ALC882_FIXUP_ACER_ASPIRE_7736),
	SND_PCI_QUIRK(0x1043, 0x13c2, "Asus A7M", ALC882_FIXUP_EAPD),
	SND_PCI_QUIRK(0x1043, 0x1873, "ASUS W90V", ALC882_FIXUP_ASUS_W90V),
	SND_PCI_QUIRK(0x1043, 0x1971, "Asus W2JC", ALC882_FIXUP_ASUS_W2JC),
<<<<<<< HEAD
	SND_PCI_QUIRK(0x1043, 0x835f, "Asus Eee 1601", ALC888_FIXUP_EEE1601),
	SND_PCI_QUIRK(0x104d, 0x9047, "Sony Vaio TT", ALC889_FIXUP_VAIO_TT),
	SND_PCI_QUIRK(0x104d, 0x905a, "Sony Vaio Z", ALC882_FIXUP_NO_PRIMARY_HP),
	SND_PCI_QUIRK(0x104d, 0x9043, "Sony Vaio VGC-LN51JGB", ALC882_FIXUP_NO_PRIMARY_HP),
=======
	SND_PCI_QUIRK(0x1043, 0x2390, "Asus D700SA", ALC887_FIXUP_ASUS_HMIC),
	SND_PCI_QUIRK(0x1043, 0x835f, "Asus Eee 1601", ALC888_FIXUP_EEE1601),
	SND_PCI_QUIRK(0x1043, 0x84bc, "ASUS ET2700", ALC887_FIXUP_ASUS_BASS),
	SND_PCI_QUIRK(0x1043, 0x8691, "ASUS ROG Ranger VIII", ALC882_FIXUP_GPIO3),
	SND_PCI_QUIRK(0x1043, 0x8797, "ASUS TUF B550M-PLUS", ALCS1200A_FIXUP_MIC_VREF),
	SND_PCI_QUIRK(0x104d, 0x9043, "Sony Vaio VGC-LN51JGB", ALC882_FIXUP_NO_PRIMARY_HP),
	SND_PCI_QUIRK(0x104d, 0x9044, "Sony VAIO AiO", ALC882_FIXUP_NO_PRIMARY_HP),
	SND_PCI_QUIRK(0x104d, 0x9047, "Sony Vaio TT", ALC889_FIXUP_VAIO_TT),
	SND_PCI_QUIRK(0x104d, 0x905a, "Sony Vaio Z", ALC882_FIXUP_NO_PRIMARY_HP),
	SND_PCI_QUIRK(0x104d, 0x9060, "Sony Vaio VPCL14M1R", ALC882_FIXUP_NO_PRIMARY_HP),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* All Apple entries are in codec SSIDs */
	SND_PCI_QUIRK(0x106b, 0x00a0, "MacBookPro 3,1", ALC889_FIXUP_MBP_VREF),
	SND_PCI_QUIRK(0x106b, 0x00a1, "Macbook", ALC889_FIXUP_MBP_VREF),
	SND_PCI_QUIRK(0x106b, 0x00a4, "MacbookPro 4,1", ALC889_FIXUP_MBP_VREF),
<<<<<<< HEAD
	SND_PCI_QUIRK(0x106b, 0x0c00, "Mac Pro", ALC885_FIXUP_MACPRO_GPIO),
=======
	SND_PCI_QUIRK(0x106b, 0x0c00, "Mac Pro", ALC889_FIXUP_MP11_VREF),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	SND_PCI_QUIRK(0x106b, 0x1000, "iMac 24", ALC885_FIXUP_MACPRO_GPIO),
	SND_PCI_QUIRK(0x106b, 0x2800, "AppleTV", ALC885_FIXUP_MACPRO_GPIO),
	SND_PCI_QUIRK(0x106b, 0x2c00, "MacbookPro rev3", ALC889_FIXUP_MBP_VREF),
	SND_PCI_QUIRK(0x106b, 0x3000, "iMac", ALC889_FIXUP_MBP_VREF),
	SND_PCI_QUIRK(0x106b, 0x3200, "iMac 7,1 Aluminum", ALC882_FIXUP_EAPD),
<<<<<<< HEAD
	SND_PCI_QUIRK(0x106b, 0x3400, "MacBookAir 1,1", ALC889_FIXUP_MBP_VREF),
	SND_PCI_QUIRK(0x106b, 0x3500, "MacBookAir 2,1", ALC889_FIXUP_MBP_VREF),
=======
	SND_PCI_QUIRK(0x106b, 0x3400, "MacBookAir 1,1", ALC889_FIXUP_MBA11_VREF),
	SND_PCI_QUIRK(0x106b, 0x3500, "MacBookAir 2,1", ALC889_FIXUP_MBA21_VREF),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	SND_PCI_QUIRK(0x106b, 0x3600, "Macbook 3,1", ALC889_FIXUP_MBP_VREF),
	SND_PCI_QUIRK(0x106b, 0x3800, "MacbookPro 4,1", ALC889_FIXUP_MBP_VREF),
	SND_PCI_QUIRK(0x106b, 0x3e00, "iMac 24 Aluminum", ALC885_FIXUP_MACPRO_GPIO),
	SND_PCI_QUIRK(0x106b, 0x3f00, "Macbook 5,1", ALC889_FIXUP_IMAC91_VREF),
	SND_PCI_QUIRK(0x106b, 0x4000, "MacbookPro 5,1", ALC889_FIXUP_IMAC91_VREF),
	SND_PCI_QUIRK(0x106b, 0x4100, "Macmini 3,1", ALC889_FIXUP_IMAC91_VREF),
<<<<<<< HEAD
	SND_PCI_QUIRK(0x106b, 0x4200, "Mac Pro 5,1", ALC885_FIXUP_MACPRO_GPIO),
	SND_PCI_QUIRK(0x106b, 0x4300, "iMac 9,1", ALC889_FIXUP_IMAC91_VREF),
	SND_PCI_QUIRK(0x106b, 0x4600, "MacbookPro 5,2", ALC889_FIXUP_IMAC91_VREF),
	SND_PCI_QUIRK(0x106b, 0x4900, "iMac 9,1 Aluminum", ALC889_FIXUP_IMAC91_VREF),
	SND_PCI_QUIRK(0x106b, 0x4a00, "Macbook 5,2", ALC889_FIXUP_IMAC91_VREF),

	SND_PCI_QUIRK(0x1071, 0x8258, "Evesham Voyaeger", ALC882_FIXUP_EAPD),
	SND_PCI_QUIRK(0x1462, 0x7350, "MSI-7350", ALC889_FIXUP_CD),
	SND_PCI_QUIRK_VENDOR(0x1462, "MSI", ALC882_FIXUP_GPIO3),
	SND_PCI_QUIRK(0x1458, 0xa002, "Gigabyte EP45-DS3", ALC889_FIXUP_CD),
	SND_PCI_QUIRK(0x147b, 0x107a, "Abit AW9D-MAX", ALC882_FIXUP_ABIT_AW9D_MAX),
=======
	SND_PCI_QUIRK(0x106b, 0x4200, "Mac Pro 4,1/5,1", ALC889_FIXUP_MP41_VREF),
	SND_PCI_QUIRK(0x106b, 0x4300, "iMac 9,1", ALC889_FIXUP_IMAC91_VREF),
	SND_PCI_QUIRK(0x106b, 0x4600, "MacbookPro 5,2", ALC889_FIXUP_IMAC91_VREF),
	SND_PCI_QUIRK(0x106b, 0x4900, "iMac 9,1 Aluminum", ALC889_FIXUP_IMAC91_VREF),
	SND_PCI_QUIRK(0x106b, 0x4a00, "Macbook 5,2", ALC889_FIXUP_MBA11_VREF),

	SND_PCI_QUIRK(0x1071, 0x8258, "Evesham Voyaeger", ALC882_FIXUP_EAPD),
	SND_PCI_QUIRK(0x10ec, 0x12d8, "iBase Elo Touch", ALC888VD_FIXUP_MIC_100VREF),
	SND_PCI_QUIRK(0x13fe, 0x1009, "Advantech MIT-W101", ALC886_FIXUP_EAPD),
	SND_PCI_QUIRK(0x1458, 0xa002, "Gigabyte EP45-DS3/Z87X-UD3H", ALC889_FIXUP_FRONT_HP_NO_PRESENCE),
	SND_PCI_QUIRK(0x1458, 0xa0b8, "Gigabyte AZ370-Gaming", ALC1220_FIXUP_GB_DUAL_CODECS),
	SND_PCI_QUIRK(0x1458, 0xa0cd, "Gigabyte X570 Aorus Master", ALC1220_FIXUP_GB_X570),
	SND_PCI_QUIRK(0x1458, 0xa0ce, "Gigabyte X570 Aorus Xtreme", ALC1220_FIXUP_GB_X570),
	SND_PCI_QUIRK(0x1458, 0xa0d5, "Gigabyte X570S Aorus Master", ALC1220_FIXUP_GB_X570),
	SND_PCI_QUIRK(0x1462, 0x11f7, "MSI-GE63", ALC1220_FIXUP_CLEVO_P950),
	SND_PCI_QUIRK(0x1462, 0x1228, "MSI-GP63", ALC1220_FIXUP_CLEVO_P950),
	SND_PCI_QUIRK(0x1462, 0x1229, "MSI-GP73", ALC1220_FIXUP_CLEVO_P950),
	SND_PCI_QUIRK(0x1462, 0x1275, "MSI-GL63", ALC1220_FIXUP_CLEVO_P950),
	SND_PCI_QUIRK(0x1462, 0x1276, "MSI-GL73", ALC1220_FIXUP_CLEVO_P950),
	SND_PCI_QUIRK(0x1462, 0x1293, "MSI-GP65", ALC1220_FIXUP_CLEVO_P950),
	SND_PCI_QUIRK(0x1462, 0x7350, "MSI-7350", ALC889_FIXUP_CD),
	SND_PCI_QUIRK(0x1462, 0xcc34, "MSI Godlike X570", ALC1220_FIXUP_GB_DUAL_CODECS),
	SND_PCI_QUIRK(0x1462, 0xda57, "MSI Z270-Gaming", ALC1220_FIXUP_GB_DUAL_CODECS),
	SND_PCI_QUIRK_VENDOR(0x1462, "MSI", ALC882_FIXUP_GPIO3),
	SND_PCI_QUIRK(0x147b, 0x107a, "Abit AW9D-MAX", ALC882_FIXUP_ABIT_AW9D_MAX),
	SND_PCI_QUIRK(0x1558, 0x3702, "Clevo X370SN[VW]", ALC1220_FIXUP_CLEVO_PB51ED_PINS),
	SND_PCI_QUIRK(0x1558, 0x50d3, "Clevo PC50[ER][CDF]", ALC1220_FIXUP_CLEVO_PB51ED_PINS),
	SND_PCI_QUIRK(0x1558, 0x65d1, "Clevo PB51[ER][CDF]", ALC1220_FIXUP_CLEVO_PB51ED_PINS),
	SND_PCI_QUIRK(0x1558, 0x65d2, "Clevo PB51R[CDF]", ALC1220_FIXUP_CLEVO_PB51ED_PINS),
	SND_PCI_QUIRK(0x1558, 0x65e1, "Clevo PB51[ED][DF]", ALC1220_FIXUP_CLEVO_PB51ED_PINS),
	SND_PCI_QUIRK(0x1558, 0x65e5, "Clevo PC50D[PRS](?:-D|-G)?", ALC1220_FIXUP_CLEVO_PB51ED_PINS),
	SND_PCI_QUIRK(0x1558, 0x65f1, "Clevo PC50HS", ALC1220_FIXUP_CLEVO_PB51ED_PINS),
	SND_PCI_QUIRK(0x1558, 0x65f5, "Clevo PD50PN[NRT]", ALC1220_FIXUP_CLEVO_PB51ED_PINS),
	SND_PCI_QUIRK(0x1558, 0x66a2, "Clevo PE60RNE", ALC1220_FIXUP_CLEVO_PB51ED_PINS),
	SND_PCI_QUIRK(0x1558, 0x66a6, "Clevo PE60SN[CDE]-[GS]", ALC1220_FIXUP_CLEVO_PB51ED_PINS),
	SND_PCI_QUIRK(0x1558, 0x67d1, "Clevo PB71[ER][CDF]", ALC1220_FIXUP_CLEVO_PB51ED_PINS),
	SND_PCI_QUIRK(0x1558, 0x67e1, "Clevo PB71[DE][CDF]", ALC1220_FIXUP_CLEVO_PB51ED_PINS),
	SND_PCI_QUIRK(0x1558, 0x67e5, "Clevo PC70D[PRS](?:-D|-G)?", ALC1220_FIXUP_CLEVO_PB51ED_PINS),
	SND_PCI_QUIRK(0x1558, 0x67f1, "Clevo PC70H[PRS]", ALC1220_FIXUP_CLEVO_PB51ED_PINS),
	SND_PCI_QUIRK(0x1558, 0x67f5, "Clevo PD70PN[NRT]", ALC1220_FIXUP_CLEVO_PB51ED_PINS),
	SND_PCI_QUIRK(0x1558, 0x70d1, "Clevo PC70[ER][CDF]", ALC1220_FIXUP_CLEVO_PB51ED_PINS),
	SND_PCI_QUIRK(0x1558, 0x7714, "Clevo X170SM", ALC1220_FIXUP_CLEVO_PB51ED_PINS),
	SND_PCI_QUIRK(0x1558, 0x7715, "Clevo X170KM-G", ALC1220_FIXUP_CLEVO_PB51ED),
	SND_PCI_QUIRK(0x1558, 0x9501, "Clevo P950HR", ALC1220_FIXUP_CLEVO_P950),
	SND_PCI_QUIRK(0x1558, 0x9506, "Clevo P955HQ", ALC1220_FIXUP_CLEVO_P950),
	SND_PCI_QUIRK(0x1558, 0x950a, "Clevo P955H[PR]", ALC1220_FIXUP_CLEVO_P950),
	SND_PCI_QUIRK(0x1558, 0x95e1, "Clevo P95xER", ALC1220_FIXUP_CLEVO_P950),
	SND_PCI_QUIRK(0x1558, 0x95e2, "Clevo P950ER", ALC1220_FIXUP_CLEVO_P950),
	SND_PCI_QUIRK(0x1558, 0x95e3, "Clevo P955[ER]T", ALC1220_FIXUP_CLEVO_P950),
	SND_PCI_QUIRK(0x1558, 0x95e4, "Clevo P955ER", ALC1220_FIXUP_CLEVO_P950),
	SND_PCI_QUIRK(0x1558, 0x95e5, "Clevo P955EE6", ALC1220_FIXUP_CLEVO_P950),
	SND_PCI_QUIRK(0x1558, 0x95e6, "Clevo P950R[CDF]", ALC1220_FIXUP_CLEVO_P950),
	SND_PCI_QUIRK(0x1558, 0x96e1, "Clevo P960[ER][CDFN]-K", ALC1220_FIXUP_CLEVO_P950),
	SND_PCI_QUIRK(0x1558, 0x97e1, "Clevo P970[ER][CDFN]", ALC1220_FIXUP_CLEVO_P950),
	SND_PCI_QUIRK(0x1558, 0x97e2, "Clevo P970RC-M", ALC1220_FIXUP_CLEVO_P950),
	SND_PCI_QUIRK(0x1558, 0xd502, "Clevo PD50SNE", ALC1220_FIXUP_CLEVO_PB51ED_PINS),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	SND_PCI_QUIRK_VENDOR(0x1558, "Clevo laptop", ALC882_FIXUP_EAPD),
	SND_PCI_QUIRK(0x161f, 0x2054, "Medion laptop", ALC883_FIXUP_EAPD),
	SND_PCI_QUIRK(0x17aa, 0x3a0d, "Lenovo Y530", ALC882_FIXUP_LENOVO_Y530),
	SND_PCI_QUIRK(0x8086, 0x0022, "DX58SO", ALC889_FIXUP_COEF),
	{}
};

<<<<<<< HEAD
static const struct alc_model_fixup alc882_fixup_models[] = {
	{.id = ALC882_FIXUP_ACER_ASPIRE_4930G, .name = "acer-aspire-4930g"},
	{.id = ALC882_FIXUP_ACER_ASPIRE_8930G, .name = "acer-aspire-8930g"},
	{.id = ALC883_FIXUP_ACER_EAPD, .name = "acer-aspire"},
	{.id = ALC882_FIXUP_NO_PRIMARY_HP, .name = "no-primary-hp"},
=======
static const struct hda_model_fixup alc882_fixup_models[] = {
	{.id = ALC882_FIXUP_ABIT_AW9D_MAX, .name = "abit-aw9d"},
	{.id = ALC882_FIXUP_LENOVO_Y530, .name = "lenovo-y530"},
	{.id = ALC882_FIXUP_ACER_ASPIRE_7736, .name = "acer-aspire-7736"},
	{.id = ALC882_FIXUP_ASUS_W90V, .name = "asus-w90v"},
	{.id = ALC889_FIXUP_CD, .name = "cd"},
	{.id = ALC889_FIXUP_FRONT_HP_NO_PRESENCE, .name = "no-front-hp"},
	{.id = ALC889_FIXUP_VAIO_TT, .name = "vaio-tt"},
	{.id = ALC888_FIXUP_EEE1601, .name = "eee1601"},
	{.id = ALC882_FIXUP_EAPD, .name = "alc882-eapd"},
	{.id = ALC883_FIXUP_EAPD, .name = "alc883-eapd"},
	{.id = ALC882_FIXUP_GPIO1, .name = "gpio1"},
	{.id = ALC882_FIXUP_GPIO2, .name = "gpio2"},
	{.id = ALC882_FIXUP_GPIO3, .name = "gpio3"},
	{.id = ALC889_FIXUP_COEF, .name = "alc889-coef"},
	{.id = ALC882_FIXUP_ASUS_W2JC, .name = "asus-w2jc"},
	{.id = ALC882_FIXUP_ACER_ASPIRE_4930G, .name = "acer-aspire-4930g"},
	{.id = ALC882_FIXUP_ACER_ASPIRE_8930G, .name = "acer-aspire-8930g"},
	{.id = ALC883_FIXUP_ACER_EAPD, .name = "acer-aspire"},
	{.id = ALC885_FIXUP_MACPRO_GPIO, .name = "macpro-gpio"},
	{.id = ALC889_FIXUP_DAC_ROUTE, .name = "dac-route"},
	{.id = ALC889_FIXUP_MBP_VREF, .name = "mbp-vref"},
	{.id = ALC889_FIXUP_IMAC91_VREF, .name = "imac91-vref"},
	{.id = ALC889_FIXUP_MBA11_VREF, .name = "mba11-vref"},
	{.id = ALC889_FIXUP_MBA21_VREF, .name = "mba21-vref"},
	{.id = ALC889_FIXUP_MP11_VREF, .name = "mp11-vref"},
	{.id = ALC889_FIXUP_MP41_VREF, .name = "mp41-vref"},
	{.id = ALC882_FIXUP_INV_DMIC, .name = "inv-dmic"},
	{.id = ALC882_FIXUP_NO_PRIMARY_HP, .name = "no-primary-hp"},
	{.id = ALC887_FIXUP_ASUS_BASS, .name = "asus-bass"},
	{.id = ALC1220_FIXUP_GB_DUAL_CODECS, .name = "dual-codecs"},
	{.id = ALC1220_FIXUP_GB_X570, .name = "gb-x570"},
	{.id = ALC1220_FIXUP_CLEVO_P950, .name = "clevo-p950"},
	{}
};

static const struct snd_hda_pin_quirk alc882_pin_fixup_tbl[] = {
	SND_HDA_PIN_QUIRK(0x10ec1220, 0x1043, "ASUS", ALC1220_FIXUP_CLEVO_P950,
		{0x14, 0x01014010},
		{0x15, 0x01011012},
		{0x16, 0x01016011},
		{0x18, 0x01a19040},
		{0x19, 0x02a19050},
		{0x1a, 0x0181304f},
		{0x1b, 0x0221401f},
		{0x1e, 0x01456130}),
	SND_HDA_PIN_QUIRK(0x10ec1220, 0x1462, "MS-7C35", ALC1220_FIXUP_CLEVO_P950,
		{0x14, 0x01015010},
		{0x15, 0x01011012},
		{0x16, 0x01011011},
		{0x18, 0x01a11040},
		{0x19, 0x02a19050},
		{0x1a, 0x0181104f},
		{0x1b, 0x0221401f},
		{0x1e, 0x01451130}),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{}
};

/*
 * BIOS auto configuration
 */
/* almost identical with ALC880 parser... */
static int alc882_parse_auto_config(struct hda_codec *codec)
{
	static const hda_nid_t alc882_ignore[] = { 0x1d, 0 };
	static const hda_nid_t alc882_ssids[] = { 0x15, 0x1b, 0x14, 0 };
	return alc_parse_auto_config(codec, alc882_ignore, alc882_ssids);
}

/*
 */
static int patch_alc882(struct hda_codec *codec)
{
	struct alc_spec *spec;
	int err;

<<<<<<< HEAD
	spec = kzalloc(sizeof(*spec), GFP_KERNEL);
	if (spec == NULL)
		return -ENOMEM;

	codec->spec = spec;

	spec->mixer_nid = 0x0b;

	switch (codec->vendor_id) {
	case 0x10ec0882:
	case 0x10ec0885:
	case 0x10ec0900:
=======
	err = alc_alloc_spec(codec, 0x0b);
	if (err < 0)
		return err;

	spec = codec->spec;

	switch (codec->core.vendor_id) {
	case 0x10ec0882:
	case 0x10ec0885:
	case 0x10ec0900:
	case 0x10ec0b00:
	case 0x10ec1220:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	default:
		/* ALC883 and variants */
		alc_fix_pll_init(codec, 0x20, 0x0a, 10);
		break;
	}

<<<<<<< HEAD
	err = alc_codec_rename_from_preset(codec);
	if (err < 0)
		goto error;

	alc_pick_fixup(codec, alc882_fixup_models, alc882_fixup_tbl,
		       alc882_fixups);
	alc_apply_fixup(codec, ALC_FIXUP_ACT_PRE_PROBE);

	alc_auto_parse_customize_define(codec);

=======
	alc_pre_init(codec);

	snd_hda_pick_fixup(codec, alc882_fixup_models, alc882_fixup_tbl,
		       alc882_fixups);
	snd_hda_pick_pin_fixup(codec, alc882_pin_fixup_tbl, alc882_fixups, true);
	snd_hda_apply_fixup(codec, HDA_FIXUP_ACT_PRE_PROBE);

	alc_auto_parse_customize_define(codec);

	if (has_cdefine_beep(codec))
		spec->gen.beep_nid = 0x01;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* automatic parse from the BIOS config */
	err = alc882_parse_auto_config(codec);
	if (err < 0)
		goto error;

<<<<<<< HEAD
	if (!spec->no_analog && has_cdefine_beep(codec)) {
		err = snd_hda_attach_beep_device(codec, 0x1);
		if (err < 0)
			goto error;
		set_beep_amp(spec, 0x0b, 0x05, HDA_INPUT);
	}

	codec->patch_ops = alc_patch_ops;

	alc_apply_fixup(codec, ALC_FIXUP_ACT_PROBE);
=======
	if (!spec->gen.no_analog && spec->gen.beep_nid) {
		err = set_beep_amp(spec, 0x0b, 0x05, HDA_INPUT);
		if (err < 0)
			goto error;
	}

	snd_hda_apply_fixup(codec, HDA_FIXUP_ACT_PROBE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;

 error:
	alc_free(codec);
	return err;
}


/*
 * ALC262 support
 */
static int alc262_parse_auto_config(struct hda_codec *codec)
{
	static const hda_nid_t alc262_ignore[] = { 0x1d, 0 };
	static const hda_nid_t alc262_ssids[] = { 0x15, 0x1b, 0x14, 0 };
	return alc_parse_auto_config(codec, alc262_ignore, alc262_ssids);
}

/*
 * Pin config fixes
 */
enum {
	ALC262_FIXUP_FSC_H270,
<<<<<<< HEAD
=======
	ALC262_FIXUP_FSC_S7110,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ALC262_FIXUP_HP_Z200,
	ALC262_FIXUP_TYAN,
	ALC262_FIXUP_LENOVO_3000,
	ALC262_FIXUP_BENQ,
	ALC262_FIXUP_BENQ_T31,
<<<<<<< HEAD
};

static const struct alc_fixup alc262_fixups[] = {
	[ALC262_FIXUP_FSC_H270] = {
		.type = ALC_FIXUP_PINS,
		.v.pins = (const struct alc_pincfg[]) {
=======
	ALC262_FIXUP_INV_DMIC,
	ALC262_FIXUP_INTEL_BAYLEYBAY,
};

static const struct hda_fixup alc262_fixups[] = {
	[ALC262_FIXUP_FSC_H270] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			{ 0x14, 0x99130110 }, /* speaker */
			{ 0x15, 0x0221142f }, /* front HP */
			{ 0x1b, 0x0121141f }, /* rear HP */
			{ }
		}
	},
<<<<<<< HEAD
	[ALC262_FIXUP_HP_Z200] = {
		.type = ALC_FIXUP_PINS,
		.v.pins = (const struct alc_pincfg[]) {
=======
	[ALC262_FIXUP_FSC_S7110] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x15, 0x90170110 }, /* speaker */
			{ }
		},
		.chained = true,
		.chain_id = ALC262_FIXUP_BENQ,
	},
	[ALC262_FIXUP_HP_Z200] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			{ 0x16, 0x99130120 }, /* internal speaker */
			{ }
		}
	},
	[ALC262_FIXUP_TYAN] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_PINS,
		.v.pins = (const struct alc_pincfg[]) {
=======
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			{ 0x14, 0x1993e1f0 }, /* int AUX */
			{ }
		}
	},
	[ALC262_FIXUP_LENOVO_3000] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_VERBS,
		.v.verbs = (const struct hda_verb[]) {
			{ 0x19, AC_VERB_SET_PIN_WIDGET_CONTROL, PIN_VREF50 },
=======
		.type = HDA_FIXUP_PINCTLS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x19, PIN_VREF50 },
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			{}
		},
		.chained = true,
		.chain_id = ALC262_FIXUP_BENQ,
	},
	[ALC262_FIXUP_BENQ] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_VERBS,
=======
		.type = HDA_FIXUP_VERBS,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.v.verbs = (const struct hda_verb[]) {
			{ 0x20, AC_VERB_SET_COEF_INDEX, 0x07 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x3070 },
			{}
		}
	},
	[ALC262_FIXUP_BENQ_T31] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_VERBS,
=======
		.type = HDA_FIXUP_VERBS,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.v.verbs = (const struct hda_verb[]) {
			{ 0x20, AC_VERB_SET_COEF_INDEX, 0x07 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x3050 },
			{}
		}
	},
<<<<<<< HEAD
=======
	[ALC262_FIXUP_INV_DMIC] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_inv_dmic,
	},
	[ALC262_FIXUP_INTEL_BAYLEYBAY] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_no_depop_delay,
	},
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static const struct snd_pci_quirk alc262_fixup_tbl[] = {
	SND_PCI_QUIRK(0x103c, 0x170b, "HP Z200", ALC262_FIXUP_HP_Z200),
<<<<<<< HEAD
	SND_PCI_QUIRK(0x10cf, 0x1397, "Fujitsu", ALC262_FIXUP_BENQ),
	SND_PCI_QUIRK(0x10cf, 0x142d, "Fujitsu Lifebook E8410", ALC262_FIXUP_BENQ),
	SND_PCI_QUIRK(0x10f1, 0x2915, "Tyan Thunder n6650W", ALC262_FIXUP_TYAN),
=======
	SND_PCI_QUIRK(0x10cf, 0x1397, "Fujitsu Lifebook S7110", ALC262_FIXUP_FSC_S7110),
	SND_PCI_QUIRK(0x10cf, 0x142d, "Fujitsu Lifebook E8410", ALC262_FIXUP_BENQ),
	SND_PCI_QUIRK(0x10f1, 0x2915, "Tyan Thunder n6650W", ALC262_FIXUP_TYAN),
	SND_PCI_QUIRK(0x1734, 0x1141, "FSC ESPRIMO U9210", ALC262_FIXUP_FSC_H270),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	SND_PCI_QUIRK(0x1734, 0x1147, "FSC Celsius H270", ALC262_FIXUP_FSC_H270),
	SND_PCI_QUIRK(0x17aa, 0x384e, "Lenovo 3000", ALC262_FIXUP_LENOVO_3000),
	SND_PCI_QUIRK(0x17ff, 0x0560, "Benq ED8", ALC262_FIXUP_BENQ),
	SND_PCI_QUIRK(0x17ff, 0x058d, "Benq T31-16", ALC262_FIXUP_BENQ_T31),
<<<<<<< HEAD
	{}
};

=======
	SND_PCI_QUIRK(0x8086, 0x7270, "BayleyBay", ALC262_FIXUP_INTEL_BAYLEYBAY),
	{}
};

static const struct hda_model_fixup alc262_fixup_models[] = {
	{.id = ALC262_FIXUP_INV_DMIC, .name = "inv-dmic"},
	{.id = ALC262_FIXUP_FSC_H270, .name = "fsc-h270"},
	{.id = ALC262_FIXUP_FSC_S7110, .name = "fsc-s7110"},
	{.id = ALC262_FIXUP_HP_Z200, .name = "hp-z200"},
	{.id = ALC262_FIXUP_TYAN, .name = "tyan"},
	{.id = ALC262_FIXUP_LENOVO_3000, .name = "lenovo-3000"},
	{.id = ALC262_FIXUP_BENQ, .name = "benq"},
	{.id = ALC262_FIXUP_BENQ_T31, .name = "benq-t31"},
	{.id = ALC262_FIXUP_INTEL_BAYLEYBAY, .name = "bayleybay"},
	{}
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 */
static int patch_alc262(struct hda_codec *codec)
{
	struct alc_spec *spec;
	int err;

<<<<<<< HEAD
	spec = kzalloc(sizeof(*spec), GFP_KERNEL);
	if (spec == NULL)
		return -ENOMEM;

	codec->spec = spec;

	spec->mixer_nid = 0x0b;
=======
	err = alc_alloc_spec(codec, 0x0b);
	if (err < 0)
		return err;

	spec = codec->spec;
	spec->gen.shared_mic_vref_pin = 0x18;

	spec->shutup = alc_eapd_shutup;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#if 0
	/* pshou 07/11/05  set a zero PCM sample to DAC when FIFO is
	 * under-run
	 */
<<<<<<< HEAD
	{
	int tmp;
	snd_hda_codec_write(codec, 0x1a, 0, AC_VERB_SET_COEF_INDEX, 7);
	tmp = snd_hda_codec_read(codec, 0x20, 0, AC_VERB_GET_PROC_COEF, 0);
	snd_hda_codec_write(codec, 0x1a, 0, AC_VERB_SET_COEF_INDEX, 7);
	snd_hda_codec_write(codec, 0x1a, 0, AC_VERB_SET_PROC_COEF, tmp | 0x80);
	}
#endif
	alc_fix_pll_init(codec, 0x20, 0x0a, 10);

	alc_pick_fixup(codec, NULL, alc262_fixup_tbl, alc262_fixups);
	alc_apply_fixup(codec, ALC_FIXUP_ACT_PRE_PROBE);

	alc_auto_parse_customize_define(codec);

=======
	alc_update_coefex_idx(codec, 0x1a, 7, 0, 0x80);
#endif
	alc_fix_pll_init(codec, 0x20, 0x0a, 10);

	alc_pre_init(codec);

	snd_hda_pick_fixup(codec, alc262_fixup_models, alc262_fixup_tbl,
		       alc262_fixups);
	snd_hda_apply_fixup(codec, HDA_FIXUP_ACT_PRE_PROBE);

	alc_auto_parse_customize_define(codec);

	if (has_cdefine_beep(codec))
		spec->gen.beep_nid = 0x01;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* automatic parse from the BIOS config */
	err = alc262_parse_auto_config(codec);
	if (err < 0)
		goto error;

<<<<<<< HEAD
	if (!spec->no_analog && has_cdefine_beep(codec)) {
		err = snd_hda_attach_beep_device(codec, 0x1);
		if (err < 0)
			goto error;
		set_beep_amp(spec, 0x0b, 0x05, HDA_INPUT);
	}

	codec->patch_ops = alc_patch_ops;
	spec->shutup = alc_eapd_shutup;

	alc_apply_fixup(codec, ALC_FIXUP_ACT_PROBE);
=======
	if (!spec->gen.no_analog && spec->gen.beep_nid) {
		err = set_beep_amp(spec, 0x0b, 0x05, HDA_INPUT);
		if (err < 0)
			goto error;
	}

	snd_hda_apply_fixup(codec, HDA_FIXUP_ACT_PROBE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;

 error:
	alc_free(codec);
	return err;
}

/*
 *  ALC268
 */
/* bind Beep switches of both NID 0x0f and 0x10 */
<<<<<<< HEAD
static const struct hda_bind_ctls alc268_bind_beep_sw = {
	.ops = &snd_hda_bind_sw,
	.values = {
		HDA_COMPOSE_AMP_VAL(0x0f, 3, 1, HDA_INPUT),
		HDA_COMPOSE_AMP_VAL(0x10, 3, 1, HDA_INPUT),
		0
	},
};

static const struct snd_kcontrol_new alc268_beep_mixer[] = {
	HDA_CODEC_VOLUME("Beep Playback Volume", 0x1d, 0x0, HDA_INPUT),
	HDA_BIND_SW("Beep Playback Switch", &alc268_bind_beep_sw),
	{ }
=======
static int alc268_beep_switch_put(struct snd_kcontrol *kcontrol,
				  struct snd_ctl_elem_value *ucontrol)
{
	struct hda_codec *codec = snd_kcontrol_chip(kcontrol);
	unsigned long pval;
	int err;

	mutex_lock(&codec->control_mutex);
	pval = kcontrol->private_value;
	kcontrol->private_value = (pval & ~0xff) | 0x0f;
	err = snd_hda_mixer_amp_switch_put(kcontrol, ucontrol);
	if (err >= 0) {
		kcontrol->private_value = (pval & ~0xff) | 0x10;
		err = snd_hda_mixer_amp_switch_put(kcontrol, ucontrol);
	}
	kcontrol->private_value = pval;
	mutex_unlock(&codec->control_mutex);
	return err;
}

static const struct snd_kcontrol_new alc268_beep_mixer[] = {
	HDA_CODEC_VOLUME("Beep Playback Volume", 0x1d, 0x0, HDA_INPUT),
	{
		.iface = SNDRV_CTL_ELEM_IFACE_MIXER,
		.name = "Beep Playback Switch",
		.subdevice = HDA_SUBDEV_AMP_FLAG,
		.info = snd_hda_mixer_amp_switch_info,
		.get = snd_hda_mixer_amp_switch_get,
		.put = alc268_beep_switch_put,
		.private_value = HDA_COMPOSE_AMP_VAL(0x0f, 3, 1, HDA_INPUT)
	},
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/* set PCBEEP vol = 0, mute connections */
static const struct hda_verb alc268_beep_init_verbs[] = {
	{0x1d, AC_VERB_SET_AMP_GAIN_MUTE, AMP_IN_UNMUTE(0)},
	{0x0f, AC_VERB_SET_AMP_GAIN_MUTE, AMP_IN_MUTE(1)},
	{0x10, AC_VERB_SET_AMP_GAIN_MUTE, AMP_IN_MUTE(1)},
	{ }
};

<<<<<<< HEAD
=======
enum {
	ALC268_FIXUP_INV_DMIC,
	ALC268_FIXUP_HP_EAPD,
	ALC268_FIXUP_SPDIF,
};

static const struct hda_fixup alc268_fixups[] = {
	[ALC268_FIXUP_INV_DMIC] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_inv_dmic,
	},
	[ALC268_FIXUP_HP_EAPD] = {
		.type = HDA_FIXUP_VERBS,
		.v.verbs = (const struct hda_verb[]) {
			{0x15, AC_VERB_SET_EAPD_BTLENABLE, 0},
			{}
		}
	},
	[ALC268_FIXUP_SPDIF] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x1e, 0x014b1180 }, /* enable SPDIF out */
			{}
		}
	},
};

static const struct hda_model_fixup alc268_fixup_models[] = {
	{.id = ALC268_FIXUP_INV_DMIC, .name = "inv-dmic"},
	{.id = ALC268_FIXUP_HP_EAPD, .name = "hp-eapd"},
	{.id = ALC268_FIXUP_SPDIF, .name = "spdif"},
	{}
};

static const struct snd_pci_quirk alc268_fixup_tbl[] = {
	SND_PCI_QUIRK(0x1025, 0x0139, "Acer TravelMate 6293", ALC268_FIXUP_SPDIF),
	SND_PCI_QUIRK(0x1025, 0x015b, "Acer AOA 150 (ZG5)", ALC268_FIXUP_INV_DMIC),
	/* below is codec SSID since multiple Toshiba laptops have the
	 * same PCI SSID 1179:ff00
	 */
	SND_PCI_QUIRK(0x1179, 0xff06, "Toshiba P200", ALC268_FIXUP_HP_EAPD),
	{}
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * BIOS auto configuration
 */
static int alc268_parse_auto_config(struct hda_codec *codec)
{
	static const hda_nid_t alc268_ssids[] = { 0x15, 0x1b, 0x14, 0 };
<<<<<<< HEAD
	struct alc_spec *spec = codec->spec;
	int err = alc_parse_auto_config(codec, NULL, alc268_ssids);
	if (err > 0) {
		if (!spec->no_analog && spec->autocfg.speaker_pins[0] != 0x1d) {
			add_mixer(spec, alc268_beep_mixer);
			add_verb(spec, alc268_beep_init_verbs);
		}
	}
	return err;
=======
	return alc_parse_auto_config(codec, NULL, alc268_ssids);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 */
static int patch_alc268(struct hda_codec *codec)
{
	struct alc_spec *spec;
<<<<<<< HEAD
	int i, has_beep, err;

	spec = kzalloc(sizeof(*spec), GFP_KERNEL);
	if (spec == NULL)
		return -ENOMEM;

	codec->spec = spec;

	/* ALC268 has no aa-loopback mixer */
=======
	int i, err;

	/* ALC268 has no aa-loopback mixer */
	err = alc_alloc_spec(codec, 0);
	if (err < 0)
		return err;

	spec = codec->spec;
	if (has_cdefine_beep(codec))
		spec->gen.beep_nid = 0x01;

	spec->shutup = alc_eapd_shutup;

	alc_pre_init(codec);

	snd_hda_pick_fixup(codec, alc268_fixup_models, alc268_fixup_tbl, alc268_fixups);
	snd_hda_apply_fixup(codec, HDA_FIXUP_ACT_PRE_PROBE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* automatic parse from the BIOS config */
	err = alc268_parse_auto_config(codec);
	if (err < 0)
		goto error;

<<<<<<< HEAD
	has_beep = 0;
	for (i = 0; i < spec->num_mixers; i++) {
		if (spec->mixers[i] == alc268_beep_mixer) {
			has_beep = 1;
			break;
		}
	}

	if (has_beep) {
		err = snd_hda_attach_beep_device(codec, 0x1);
		if (err < 0)
			goto error;
=======
	if (err > 0 && !spec->gen.no_analog &&
	    spec->gen.autocfg.speaker_pins[0] != 0x1d) {
		for (i = 0; i < ARRAY_SIZE(alc268_beep_mixer); i++) {
			if (!snd_hda_gen_add_kctl(&spec->gen, NULL,
						  &alc268_beep_mixer[i])) {
				err = -ENOMEM;
				goto error;
			}
		}
		snd_hda_add_verbs(codec, alc268_beep_init_verbs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!query_amp_caps(codec, 0x1d, HDA_INPUT))
			/* override the amp caps for beep generator */
			snd_hda_override_amp_caps(codec, 0x1d, HDA_INPUT,
					  (0x0c << AC_AMPCAP_OFFSET_SHIFT) |
					  (0x0c << AC_AMPCAP_NUM_STEPS_SHIFT) |
					  (0x07 << AC_AMPCAP_STEP_SIZE_SHIFT) |
					  (0 << AC_AMPCAP_MUTE_SHIFT));
	}

<<<<<<< HEAD
	codec->patch_ops = alc_patch_ops;
	spec->shutup = alc_eapd_shutup;
=======
	snd_hda_apply_fixup(codec, HDA_FIXUP_ACT_PROBE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;

 error:
	alc_free(codec);
	return err;
}

/*
 * ALC269
 */
<<<<<<< HEAD
static const struct hda_pcm_stream alc269_44k_pcm_analog_playback = {
	.substreams = 1,
	.channels_min = 2,
	.channels_max = 8,
	.rates = SNDRV_PCM_RATE_44100, /* fixed rate */
	/* NID is set in alc_build_pcms */
	.ops = {
		.open = alc_playback_pcm_open,
		.prepare = alc_playback_pcm_prepare,
		.cleanup = alc_playback_pcm_cleanup
	},
};

static const struct hda_pcm_stream alc269_44k_pcm_analog_capture = {
	.substreams = 1,
	.channels_min = 2,
	.channels_max = 2,
	.rates = SNDRV_PCM_RATE_44100, /* fixed rate */
	/* NID is set in alc_build_pcms */
=======

static const struct hda_pcm_stream alc269_44k_pcm_analog_playback = {
	.rates = SNDRV_PCM_RATE_44100, /* fixed rate */
};

static const struct hda_pcm_stream alc269_44k_pcm_analog_capture = {
	.rates = SNDRV_PCM_RATE_44100, /* fixed rate */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/* different alc269-variants */
enum {
	ALC269_TYPE_ALC269VA,
	ALC269_TYPE_ALC269VB,
	ALC269_TYPE_ALC269VC,
<<<<<<< HEAD
=======
	ALC269_TYPE_ALC269VD,
	ALC269_TYPE_ALC280,
	ALC269_TYPE_ALC282,
	ALC269_TYPE_ALC283,
	ALC269_TYPE_ALC284,
	ALC269_TYPE_ALC293,
	ALC269_TYPE_ALC286,
	ALC269_TYPE_ALC298,
	ALC269_TYPE_ALC255,
	ALC269_TYPE_ALC256,
	ALC269_TYPE_ALC257,
	ALC269_TYPE_ALC215,
	ALC269_TYPE_ALC225,
	ALC269_TYPE_ALC245,
	ALC269_TYPE_ALC287,
	ALC269_TYPE_ALC294,
	ALC269_TYPE_ALC300,
	ALC269_TYPE_ALC623,
	ALC269_TYPE_ALC700,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/*
 * BIOS auto configuration
 */
static int alc269_parse_auto_config(struct hda_codec *codec)
{
	static const hda_nid_t alc269_ignore[] = { 0x1d, 0 };
	static const hda_nid_t alc269_ssids[] = { 0, 0x1b, 0x14, 0x21 };
	static const hda_nid_t alc269va_ssids[] = { 0x15, 0x1b, 0x14, 0 };
	struct alc_spec *spec = codec->spec;
<<<<<<< HEAD
	const hda_nid_t *ssids = spec->codec_variant == ALC269_TYPE_ALC269VA ?
		alc269va_ssids : alc269_ssids;
=======
	const hda_nid_t *ssids;

	switch (spec->codec_variant) {
	case ALC269_TYPE_ALC269VA:
	case ALC269_TYPE_ALC269VC:
	case ALC269_TYPE_ALC280:
	case ALC269_TYPE_ALC284:
	case ALC269_TYPE_ALC293:
		ssids = alc269va_ssids;
		break;
	case ALC269_TYPE_ALC269VB:
	case ALC269_TYPE_ALC269VD:
	case ALC269_TYPE_ALC282:
	case ALC269_TYPE_ALC283:
	case ALC269_TYPE_ALC286:
	case ALC269_TYPE_ALC298:
	case ALC269_TYPE_ALC255:
	case ALC269_TYPE_ALC256:
	case ALC269_TYPE_ALC257:
	case ALC269_TYPE_ALC215:
	case ALC269_TYPE_ALC225:
	case ALC269_TYPE_ALC245:
	case ALC269_TYPE_ALC287:
	case ALC269_TYPE_ALC294:
	case ALC269_TYPE_ALC300:
	case ALC269_TYPE_ALC623:
	case ALC269_TYPE_ALC700:
		ssids = alc269_ssids;
		break;
	default:
		ssids = alc269_ssids;
		break;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return alc_parse_auto_config(codec, alc269_ignore, ssids);
}

<<<<<<< HEAD
static void alc269_toggle_power_output(struct hda_codec *codec, int power_up)
{
	int val = alc_read_coef_idx(codec, 0x04);
	if (val == -1)
		return;
	if (power_up)
		val |= 1 << 11;
	else
		val &= ~(1 << 11);
	alc_write_coef_idx(codec, 0x04, val);
=======
static const struct hda_jack_keymap alc_headset_btn_keymap[] = {
	{ SND_JACK_BTN_0, KEY_PLAYPAUSE },
	{ SND_JACK_BTN_1, KEY_VOICECOMMAND },
	{ SND_JACK_BTN_2, KEY_VOLUMEUP },
	{ SND_JACK_BTN_3, KEY_VOLUMEDOWN },
	{}
};

static void alc_headset_btn_callback(struct hda_codec *codec,
				     struct hda_jack_callback *jack)
{
	int report = 0;

	if (jack->unsol_res & (7 << 13))
		report |= SND_JACK_BTN_0;

	if (jack->unsol_res  & (1 << 16 | 3 << 8))
		report |= SND_JACK_BTN_1;

	/* Volume up key */
	if (jack->unsol_res & (7 << 23))
		report |= SND_JACK_BTN_2;

	/* Volume down key */
	if (jack->unsol_res & (7 << 10))
		report |= SND_JACK_BTN_3;

	snd_hda_jack_set_button_state(codec, jack->nid, report);
}

static void alc_disable_headset_jack_key(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;

	if (!spec->has_hs_key)
		return;

	switch (codec->core.vendor_id) {
	case 0x10ec0215:
	case 0x10ec0225:
	case 0x10ec0285:
	case 0x10ec0287:
	case 0x10ec0295:
	case 0x10ec0289:
	case 0x10ec0299:
		alc_write_coef_idx(codec, 0x48, 0x0);
		alc_update_coef_idx(codec, 0x49, 0x0045, 0x0);
		alc_update_coef_idx(codec, 0x44, 0x0045 << 8, 0x0);
		break;
	case 0x10ec0230:
	case 0x10ec0236:
	case 0x10ec0256:
	case 0x10ec0257:
	case 0x19e58326:
		alc_write_coef_idx(codec, 0x48, 0x0);
		alc_update_coef_idx(codec, 0x49, 0x0045, 0x0);
		break;
	}
}

static void alc_enable_headset_jack_key(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;

	if (!spec->has_hs_key)
		return;

	switch (codec->core.vendor_id) {
	case 0x10ec0215:
	case 0x10ec0225:
	case 0x10ec0285:
	case 0x10ec0287:
	case 0x10ec0295:
	case 0x10ec0289:
	case 0x10ec0299:
		alc_write_coef_idx(codec, 0x48, 0xd011);
		alc_update_coef_idx(codec, 0x49, 0x007f, 0x0045);
		alc_update_coef_idx(codec, 0x44, 0x007f << 8, 0x0045 << 8);
		break;
	case 0x10ec0230:
	case 0x10ec0236:
	case 0x10ec0256:
	case 0x10ec0257:
	case 0x19e58326:
		alc_write_coef_idx(codec, 0x48, 0xd011);
		alc_update_coef_idx(codec, 0x49, 0x007f, 0x0045);
		break;
	}
}

static void alc_fixup_headset_jack(struct hda_codec *codec,
				    const struct hda_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;
	hda_nid_t hp_pin;

	switch (action) {
	case HDA_FIXUP_ACT_PRE_PROBE:
		spec->has_hs_key = 1;
		snd_hda_jack_detect_enable_callback(codec, 0x55,
						    alc_headset_btn_callback);
		break;
	case HDA_FIXUP_ACT_BUILD:
		hp_pin = alc_get_hp_pin(spec);
		if (!hp_pin || snd_hda_jack_bind_keymap(codec, 0x55,
							alc_headset_btn_keymap,
							hp_pin))
			snd_hda_jack_add_kctl(codec, 0x55, "Headset Jack",
					      false, SND_JACK_HEADSET,
					      alc_headset_btn_keymap);

		alc_enable_headset_jack_key(codec);
		break;
	}
}

static void alc269vb_toggle_power_output(struct hda_codec *codec, int power_up)
{
	alc_update_coef_idx(codec, 0x04, 1 << 11, power_up ? (1 << 11) : 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void alc269_shutup(struct hda_codec *codec)
{
<<<<<<< HEAD
	if ((alc_get_coef0(codec) & 0x00ff) == 0x017)
		alc269_toggle_power_output(codec, 0);
	if ((alc_get_coef0(codec) & 0x00ff) == 0x018) {
		alc269_toggle_power_output(codec, 0);
		msleep(150);
	}
}

#ifdef CONFIG_PM
static int alc269_resume(struct hda_codec *codec)
{
	if ((alc_get_coef0(codec) & 0x00ff) == 0x018) {
		alc269_toggle_power_output(codec, 0);
=======
	struct alc_spec *spec = codec->spec;

	if (spec->codec_variant == ALC269_TYPE_ALC269VB)
		alc269vb_toggle_power_output(codec, 0);
	if (spec->codec_variant == ALC269_TYPE_ALC269VB &&
			(alc_get_coef0(codec) & 0x00ff) == 0x018) {
		msleep(150);
	}
	alc_shutup_pins(codec);
}

static const struct coef_fw alc282_coefs[] = {
	WRITE_COEF(0x03, 0x0002), /* Power Down Control */
	UPDATE_COEF(0x05, 0xff3f, 0x0700), /* FIFO and filter clock */
	WRITE_COEF(0x07, 0x0200), /* DMIC control */
	UPDATE_COEF(0x06, 0x00f0, 0), /* Analog clock */
	UPDATE_COEF(0x08, 0xfffc, 0x0c2c), /* JD */
	WRITE_COEF(0x0a, 0xcccc), /* JD offset1 */
	WRITE_COEF(0x0b, 0xcccc), /* JD offset2 */
	WRITE_COEF(0x0e, 0x6e00), /* LDO1/2/3, DAC/ADC */
	UPDATE_COEF(0x0f, 0xf800, 0x1000), /* JD */
	UPDATE_COEF(0x10, 0xfc00, 0x0c00), /* Capless */
	WRITE_COEF(0x6f, 0x0), /* Class D test 4 */
	UPDATE_COEF(0x0c, 0xfe00, 0), /* IO power down directly */
	WRITE_COEF(0x34, 0xa0c0), /* ANC */
	UPDATE_COEF(0x16, 0x0008, 0), /* AGC MUX */
	UPDATE_COEF(0x1d, 0x00e0, 0), /* DAC simple content protection */
	UPDATE_COEF(0x1f, 0x00e0, 0), /* ADC simple content protection */
	WRITE_COEF(0x21, 0x8804), /* DAC ADC Zero Detection */
	WRITE_COEF(0x63, 0x2902), /* PLL */
	WRITE_COEF(0x68, 0xa080), /* capless control 2 */
	WRITE_COEF(0x69, 0x3400), /* capless control 3 */
	WRITE_COEF(0x6a, 0x2f3e), /* capless control 4 */
	WRITE_COEF(0x6b, 0x0), /* capless control 5 */
	UPDATE_COEF(0x6d, 0x0fff, 0x0900), /* class D test 2 */
	WRITE_COEF(0x6e, 0x110a), /* class D test 3 */
	UPDATE_COEF(0x70, 0x00f8, 0x00d8), /* class D test 5 */
	WRITE_COEF(0x71, 0x0014), /* class D test 6 */
	WRITE_COEF(0x72, 0xc2ba), /* classD OCP */
	UPDATE_COEF(0x77, 0x0f80, 0), /* classD pure DC test */
	WRITE_COEF(0x6c, 0xfc06), /* Class D amp control */
	{}
};

static void alc282_restore_default_value(struct hda_codec *codec)
{
	alc_process_coef_fw(codec, alc282_coefs);
}

static void alc282_init(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	hda_nid_t hp_pin = alc_get_hp_pin(spec);
	bool hp_pin_sense;
	int coef78;

	alc282_restore_default_value(codec);

	if (!hp_pin)
		return;
	hp_pin_sense = snd_hda_jack_detect(codec, hp_pin);
	coef78 = alc_read_coef_idx(codec, 0x78);

	/* Index 0x78 Direct Drive HP AMP LPM Control 1 */
	/* Headphone capless set to high power mode */
	alc_write_coef_idx(codec, 0x78, 0x9004);

	if (hp_pin_sense)
		msleep(2);

	snd_hda_codec_write(codec, hp_pin, 0,
			    AC_VERB_SET_AMP_GAIN_MUTE, AMP_OUT_MUTE);

	if (hp_pin_sense)
		msleep(85);

	snd_hda_codec_write(codec, hp_pin, 0,
			    AC_VERB_SET_PIN_WIDGET_CONTROL, PIN_OUT);

	if (hp_pin_sense)
		msleep(100);

	/* Headphone capless set to normal mode */
	alc_write_coef_idx(codec, 0x78, coef78);
}

static void alc282_shutup(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	hda_nid_t hp_pin = alc_get_hp_pin(spec);
	bool hp_pin_sense;
	int coef78;

	if (!hp_pin) {
		alc269_shutup(codec);
		return;
	}

	hp_pin_sense = snd_hda_jack_detect(codec, hp_pin);
	coef78 = alc_read_coef_idx(codec, 0x78);
	alc_write_coef_idx(codec, 0x78, 0x9004);

	if (hp_pin_sense)
		msleep(2);

	snd_hda_codec_write(codec, hp_pin, 0,
			    AC_VERB_SET_AMP_GAIN_MUTE, AMP_OUT_MUTE);

	if (hp_pin_sense)
		msleep(85);

	if (!spec->no_shutup_pins)
		snd_hda_codec_write(codec, hp_pin, 0,
				    AC_VERB_SET_PIN_WIDGET_CONTROL, 0x0);

	if (hp_pin_sense)
		msleep(100);

	alc_auto_setup_eapd(codec, false);
	alc_shutup_pins(codec);
	alc_write_coef_idx(codec, 0x78, coef78);
}

static const struct coef_fw alc283_coefs[] = {
	WRITE_COEF(0x03, 0x0002), /* Power Down Control */
	UPDATE_COEF(0x05, 0xff3f, 0x0700), /* FIFO and filter clock */
	WRITE_COEF(0x07, 0x0200), /* DMIC control */
	UPDATE_COEF(0x06, 0x00f0, 0), /* Analog clock */
	UPDATE_COEF(0x08, 0xfffc, 0x0c2c), /* JD */
	WRITE_COEF(0x0a, 0xcccc), /* JD offset1 */
	WRITE_COEF(0x0b, 0xcccc), /* JD offset2 */
	WRITE_COEF(0x0e, 0x6fc0), /* LDO1/2/3, DAC/ADC */
	UPDATE_COEF(0x0f, 0xf800, 0x1000), /* JD */
	UPDATE_COEF(0x10, 0xfc00, 0x0c00), /* Capless */
	WRITE_COEF(0x3a, 0x0), /* Class D test 4 */
	UPDATE_COEF(0x0c, 0xfe00, 0x0), /* IO power down directly */
	WRITE_COEF(0x22, 0xa0c0), /* ANC */
	UPDATE_COEFEX(0x53, 0x01, 0x000f, 0x0008), /* AGC MUX */
	UPDATE_COEF(0x1d, 0x00e0, 0), /* DAC simple content protection */
	UPDATE_COEF(0x1f, 0x00e0, 0), /* ADC simple content protection */
	WRITE_COEF(0x21, 0x8804), /* DAC ADC Zero Detection */
	WRITE_COEF(0x2e, 0x2902), /* PLL */
	WRITE_COEF(0x33, 0xa080), /* capless control 2 */
	WRITE_COEF(0x34, 0x3400), /* capless control 3 */
	WRITE_COEF(0x35, 0x2f3e), /* capless control 4 */
	WRITE_COEF(0x36, 0x0), /* capless control 5 */
	UPDATE_COEF(0x38, 0x0fff, 0x0900), /* class D test 2 */
	WRITE_COEF(0x39, 0x110a), /* class D test 3 */
	UPDATE_COEF(0x3b, 0x00f8, 0x00d8), /* class D test 5 */
	WRITE_COEF(0x3c, 0x0014), /* class D test 6 */
	WRITE_COEF(0x3d, 0xc2ba), /* classD OCP */
	UPDATE_COEF(0x42, 0x0f80, 0x0), /* classD pure DC test */
	WRITE_COEF(0x49, 0x0), /* test mode */
	UPDATE_COEF(0x40, 0xf800, 0x9800), /* Class D DC enable */
	UPDATE_COEF(0x42, 0xf000, 0x2000), /* DC offset */
	WRITE_COEF(0x37, 0xfc06), /* Class D amp control */
	UPDATE_COEF(0x1b, 0x8000, 0), /* HP JD control */
	{}
};

static void alc283_restore_default_value(struct hda_codec *codec)
{
	alc_process_coef_fw(codec, alc283_coefs);
}

static void alc283_init(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	hda_nid_t hp_pin = alc_get_hp_pin(spec);
	bool hp_pin_sense;

	alc283_restore_default_value(codec);

	if (!hp_pin)
		return;

	msleep(30);
	hp_pin_sense = snd_hda_jack_detect(codec, hp_pin);

	/* Index 0x43 Direct Drive HP AMP LPM Control 1 */
	/* Headphone capless set to high power mode */
	alc_write_coef_idx(codec, 0x43, 0x9004);

	snd_hda_codec_write(codec, hp_pin, 0,
			    AC_VERB_SET_AMP_GAIN_MUTE, AMP_OUT_MUTE);

	if (hp_pin_sense)
		msleep(85);

	snd_hda_codec_write(codec, hp_pin, 0,
			    AC_VERB_SET_PIN_WIDGET_CONTROL, PIN_OUT);

	if (hp_pin_sense)
		msleep(85);
	/* Index 0x46 Combo jack auto switch control 2 */
	/* 3k pull low control for Headset jack. */
	alc_update_coef_idx(codec, 0x46, 3 << 12, 0);
	/* Headphone capless set to normal mode */
	alc_write_coef_idx(codec, 0x43, 0x9614);
}

static void alc283_shutup(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	hda_nid_t hp_pin = alc_get_hp_pin(spec);
	bool hp_pin_sense;

	if (!hp_pin) {
		alc269_shutup(codec);
		return;
	}

	hp_pin_sense = snd_hda_jack_detect(codec, hp_pin);

	alc_write_coef_idx(codec, 0x43, 0x9004);

	/*depop hp during suspend*/
	alc_write_coef_idx(codec, 0x06, 0x2100);

	snd_hda_codec_write(codec, hp_pin, 0,
			    AC_VERB_SET_AMP_GAIN_MUTE, AMP_OUT_MUTE);

	if (hp_pin_sense)
		msleep(100);

	if (!spec->no_shutup_pins)
		snd_hda_codec_write(codec, hp_pin, 0,
				    AC_VERB_SET_PIN_WIDGET_CONTROL, 0x0);

	alc_update_coef_idx(codec, 0x46, 0, 3 << 12);

	if (hp_pin_sense)
		msleep(100);
	alc_auto_setup_eapd(codec, false);
	alc_shutup_pins(codec);
	alc_write_coef_idx(codec, 0x43, 0x9614);
}

static void alc256_init(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	hda_nid_t hp_pin = alc_get_hp_pin(spec);
	bool hp_pin_sense;

	if (spec->ultra_low_power) {
		alc_update_coef_idx(codec, 0x03, 1<<1, 1<<1);
		alc_update_coef_idx(codec, 0x08, 3<<2, 3<<2);
		alc_update_coef_idx(codec, 0x08, 7<<4, 0);
		alc_update_coef_idx(codec, 0x3b, 1<<15, 0);
		alc_update_coef_idx(codec, 0x0e, 7<<6, 7<<6);
		msleep(30);
	}

	if (!hp_pin)
		hp_pin = 0x21;

	msleep(30);

	hp_pin_sense = snd_hda_jack_detect(codec, hp_pin);

	if (hp_pin_sense)
		msleep(2);

	alc_update_coefex_idx(codec, 0x57, 0x04, 0x0007, 0x1); /* Low power */

	snd_hda_codec_write(codec, hp_pin, 0,
			    AC_VERB_SET_AMP_GAIN_MUTE, AMP_OUT_MUTE);

	if (hp_pin_sense || spec->ultra_low_power)
		msleep(85);

	snd_hda_codec_write(codec, hp_pin, 0,
			    AC_VERB_SET_PIN_WIDGET_CONTROL, PIN_OUT);

	if (hp_pin_sense || spec->ultra_low_power)
		msleep(100);

	alc_update_coef_idx(codec, 0x46, 3 << 12, 0);
	alc_update_coefex_idx(codec, 0x57, 0x04, 0x0007, 0x4); /* Hight power */
	alc_update_coefex_idx(codec, 0x53, 0x02, 0x8000, 1 << 15); /* Clear bit */
	alc_update_coefex_idx(codec, 0x53, 0x02, 0x8000, 0 << 15);
	/*
	 * Expose headphone mic (or possibly Line In on some machines) instead
	 * of PC Beep on 1Ah, and disable 1Ah loopback for all outputs. See
	 * Documentation/sound/hd-audio/realtek-pc-beep.rst for details of
	 * this register.
	 */
	alc_write_coef_idx(codec, 0x36, 0x5757);
}

static void alc256_shutup(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	hda_nid_t hp_pin = alc_get_hp_pin(spec);
	bool hp_pin_sense;

	if (!hp_pin)
		hp_pin = 0x21;

	alc_update_coefex_idx(codec, 0x57, 0x04, 0x0007, 0x1); /* Low power */
	hp_pin_sense = snd_hda_jack_detect(codec, hp_pin);

	if (hp_pin_sense)
		msleep(2);

	snd_hda_codec_write(codec, hp_pin, 0,
			    AC_VERB_SET_AMP_GAIN_MUTE, AMP_OUT_MUTE);

	if (hp_pin_sense || spec->ultra_low_power)
		msleep(85);

	/* 3k pull low control for Headset jack. */
	/* NOTE: call this before clearing the pin, otherwise codec stalls */
	/* If disable 3k pulldown control for alc257, the Mic detection will not work correctly
	 * when booting with headset plugged. So skip setting it for the codec alc257
	 */
	if (spec->en_3kpull_low)
		alc_update_coef_idx(codec, 0x46, 0, 3 << 12);

	if (!spec->no_shutup_pins)
		snd_hda_codec_write(codec, hp_pin, 0,
				    AC_VERB_SET_PIN_WIDGET_CONTROL, 0x0);

	if (hp_pin_sense || spec->ultra_low_power)
		msleep(100);

	alc_auto_setup_eapd(codec, false);
	alc_shutup_pins(codec);
	if (spec->ultra_low_power) {
		msleep(50);
		alc_update_coef_idx(codec, 0x03, 1<<1, 0);
		alc_update_coef_idx(codec, 0x08, 7<<4, 7<<4);
		alc_update_coef_idx(codec, 0x08, 3<<2, 0);
		alc_update_coef_idx(codec, 0x3b, 1<<15, 1<<15);
		alc_update_coef_idx(codec, 0x0e, 7<<6, 0);
		msleep(30);
	}
}

static void alc285_hp_init(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	hda_nid_t hp_pin = alc_get_hp_pin(spec);
	int i, val;
	int coef38, coef0d, coef36;

	alc_write_coefex_idx(codec, 0x58, 0x00, 0x1888); /* write default value */
	alc_update_coef_idx(codec, 0x4a, 1<<15, 1<<15); /* Reset HP JD */
	coef38 = alc_read_coef_idx(codec, 0x38); /* Amp control */
	coef0d = alc_read_coef_idx(codec, 0x0d); /* Digital Misc control */
	coef36 = alc_read_coef_idx(codec, 0x36); /* Passthrough Control */
	alc_update_coef_idx(codec, 0x38, 1<<4, 0x0);
	alc_update_coef_idx(codec, 0x0d, 0x110, 0x0);

	alc_update_coef_idx(codec, 0x67, 0xf000, 0x3000);

	if (hp_pin)
		snd_hda_codec_write(codec, hp_pin, 0,
			    AC_VERB_SET_AMP_GAIN_MUTE, AMP_OUT_MUTE);

	msleep(130);
	alc_update_coef_idx(codec, 0x36, 1<<14, 1<<14);
	alc_update_coef_idx(codec, 0x36, 1<<13, 0x0);

	if (hp_pin)
		snd_hda_codec_write(codec, hp_pin, 0,
			    AC_VERB_SET_PIN_WIDGET_CONTROL, 0x0);
	msleep(10);
	alc_write_coef_idx(codec, 0x67, 0x0); /* Set HP depop to manual mode */
	alc_write_coefex_idx(codec, 0x58, 0x00, 0x7880);
	alc_write_coefex_idx(codec, 0x58, 0x0f, 0xf049);
	alc_update_coefex_idx(codec, 0x58, 0x03, 0x00f0, 0x00c0);

	alc_write_coefex_idx(codec, 0x58, 0x00, 0xf888); /* HP depop procedure start */
	val = alc_read_coefex_idx(codec, 0x58, 0x00);
	for (i = 0; i < 20 && val & 0x8000; i++) {
		msleep(50);
		val = alc_read_coefex_idx(codec, 0x58, 0x00);
	} /* Wait for depop procedure finish  */

	alc_write_coefex_idx(codec, 0x58, 0x00, val); /* write back the result */
	alc_update_coef_idx(codec, 0x38, 1<<4, coef38);
	alc_update_coef_idx(codec, 0x0d, 0x110, coef0d);
	alc_update_coef_idx(codec, 0x36, 3<<13, coef36);

	msleep(50);
	alc_update_coef_idx(codec, 0x4a, 1<<15, 0);
}

static void alc225_init(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	hda_nid_t hp_pin = alc_get_hp_pin(spec);
	bool hp1_pin_sense, hp2_pin_sense;

	if (spec->ultra_low_power) {
		alc_update_coef_idx(codec, 0x08, 0x0f << 2, 3<<2);
		alc_update_coef_idx(codec, 0x0e, 7<<6, 7<<6);
		alc_update_coef_idx(codec, 0x33, 1<<11, 0);
		msleep(30);
	}

	if (spec->codec_variant != ALC269_TYPE_ALC287 &&
		spec->codec_variant != ALC269_TYPE_ALC245)
		/* required only at boot or S3 and S4 resume time */
		if (!spec->done_hp_init ||
			is_s3_resume(codec) ||
			is_s4_resume(codec)) {
			alc285_hp_init(codec);
			spec->done_hp_init = true;
		}

	if (!hp_pin)
		hp_pin = 0x21;
	msleep(30);

	hp1_pin_sense = snd_hda_jack_detect(codec, hp_pin);
	hp2_pin_sense = snd_hda_jack_detect(codec, 0x16);

	if (hp1_pin_sense || hp2_pin_sense)
		msleep(2);

	alc_update_coefex_idx(codec, 0x57, 0x04, 0x0007, 0x1); /* Low power */

	if (hp1_pin_sense || spec->ultra_low_power)
		snd_hda_codec_write(codec, hp_pin, 0,
			    AC_VERB_SET_AMP_GAIN_MUTE, AMP_OUT_MUTE);
	if (hp2_pin_sense)
		snd_hda_codec_write(codec, 0x16, 0,
			    AC_VERB_SET_AMP_GAIN_MUTE, AMP_OUT_MUTE);

	if (hp1_pin_sense || hp2_pin_sense || spec->ultra_low_power)
		msleep(85);

	if (hp1_pin_sense || spec->ultra_low_power)
		snd_hda_codec_write(codec, hp_pin, 0,
			    AC_VERB_SET_PIN_WIDGET_CONTROL, PIN_OUT);
	if (hp2_pin_sense)
		snd_hda_codec_write(codec, 0x16, 0,
			    AC_VERB_SET_PIN_WIDGET_CONTROL, PIN_OUT);

	if (hp1_pin_sense || hp2_pin_sense || spec->ultra_low_power)
		msleep(100);

	alc_update_coef_idx(codec, 0x4a, 3 << 10, 0);
	alc_update_coefex_idx(codec, 0x57, 0x04, 0x0007, 0x4); /* Hight power */
}

static void alc225_shutup(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	hda_nid_t hp_pin = alc_get_hp_pin(spec);
	bool hp1_pin_sense, hp2_pin_sense;

	if (!hp_pin)
		hp_pin = 0x21;

	alc_disable_headset_jack_key(codec);
	/* 3k pull low control for Headset jack. */
	alc_update_coef_idx(codec, 0x4a, 0, 3 << 10);

	hp1_pin_sense = snd_hda_jack_detect(codec, hp_pin);
	hp2_pin_sense = snd_hda_jack_detect(codec, 0x16);

	if (hp1_pin_sense || hp2_pin_sense)
		msleep(2);

	if (hp1_pin_sense || spec->ultra_low_power)
		snd_hda_codec_write(codec, hp_pin, 0,
			    AC_VERB_SET_AMP_GAIN_MUTE, AMP_OUT_MUTE);
	if (hp2_pin_sense)
		snd_hda_codec_write(codec, 0x16, 0,
			    AC_VERB_SET_AMP_GAIN_MUTE, AMP_OUT_MUTE);

	if (hp1_pin_sense || hp2_pin_sense || spec->ultra_low_power)
		msleep(85);

	if (hp1_pin_sense || spec->ultra_low_power)
		snd_hda_codec_write(codec, hp_pin, 0,
			    AC_VERB_SET_PIN_WIDGET_CONTROL, 0x0);
	if (hp2_pin_sense)
		snd_hda_codec_write(codec, 0x16, 0,
			    AC_VERB_SET_PIN_WIDGET_CONTROL, 0x0);

	if (hp1_pin_sense || hp2_pin_sense || spec->ultra_low_power)
		msleep(100);

	alc_auto_setup_eapd(codec, false);
	alc_shutup_pins(codec);
	if (spec->ultra_low_power) {
		msleep(50);
		alc_update_coef_idx(codec, 0x08, 0x0f << 2, 0x0c << 2);
		alc_update_coef_idx(codec, 0x0e, 7<<6, 0);
		alc_update_coef_idx(codec, 0x33, 1<<11, 1<<11);
		alc_update_coef_idx(codec, 0x4a, 3<<4, 2<<4);
		msleep(30);
	}

	alc_update_coef_idx(codec, 0x4a, 3 << 10, 0);
	alc_enable_headset_jack_key(codec);
}

static void alc_default_init(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	hda_nid_t hp_pin = alc_get_hp_pin(spec);
	bool hp_pin_sense;

	if (!hp_pin)
		return;

	msleep(30);

	hp_pin_sense = snd_hda_jack_detect(codec, hp_pin);

	if (hp_pin_sense)
		msleep(2);

	snd_hda_codec_write(codec, hp_pin, 0,
			    AC_VERB_SET_AMP_GAIN_MUTE, AMP_OUT_MUTE);

	if (hp_pin_sense)
		msleep(85);

	snd_hda_codec_write(codec, hp_pin, 0,
			    AC_VERB_SET_PIN_WIDGET_CONTROL, PIN_OUT);

	if (hp_pin_sense)
		msleep(100);
}

static void alc_default_shutup(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	hda_nid_t hp_pin = alc_get_hp_pin(spec);
	bool hp_pin_sense;

	if (!hp_pin) {
		alc269_shutup(codec);
		return;
	}

	hp_pin_sense = snd_hda_jack_detect(codec, hp_pin);

	if (hp_pin_sense)
		msleep(2);

	snd_hda_codec_write(codec, hp_pin, 0,
			    AC_VERB_SET_AMP_GAIN_MUTE, AMP_OUT_MUTE);

	if (hp_pin_sense)
		msleep(85);

	if (!spec->no_shutup_pins)
		snd_hda_codec_write(codec, hp_pin, 0,
				    AC_VERB_SET_PIN_WIDGET_CONTROL, 0x0);

	if (hp_pin_sense)
		msleep(100);

	alc_auto_setup_eapd(codec, false);
	alc_shutup_pins(codec);
}

static void alc294_hp_init(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	hda_nid_t hp_pin = alc_get_hp_pin(spec);
	int i, val;

	if (!hp_pin)
		return;

	snd_hda_codec_write(codec, hp_pin, 0,
			    AC_VERB_SET_AMP_GAIN_MUTE, AMP_OUT_MUTE);

	msleep(100);

	if (!spec->no_shutup_pins)
		snd_hda_codec_write(codec, hp_pin, 0,
				    AC_VERB_SET_PIN_WIDGET_CONTROL, 0x0);

	alc_update_coef_idx(codec, 0x6f, 0x000f, 0);/* Set HP depop to manual mode */
	alc_update_coefex_idx(codec, 0x58, 0x00, 0x8000, 0x8000); /* HP depop procedure start */

	/* Wait for depop procedure finish  */
	val = alc_read_coefex_idx(codec, 0x58, 0x01);
	for (i = 0; i < 20 && val & 0x0080; i++) {
		msleep(50);
		val = alc_read_coefex_idx(codec, 0x58, 0x01);
	}
	/* Set HP depop to auto mode */
	alc_update_coef_idx(codec, 0x6f, 0x000f, 0x000b);
	msleep(50);
}

static void alc294_init(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;

	/* required only at boot or S4 resume time */
	if (!spec->done_hp_init ||
	    codec->core.dev.power.power_state.event == PM_EVENT_RESTORE) {
		alc294_hp_init(codec);
		spec->done_hp_init = true;
	}
	alc_default_init(codec);
}

static void alc5505_coef_set(struct hda_codec *codec, unsigned int index_reg,
			     unsigned int val)
{
	snd_hda_codec_write(codec, 0x51, 0, AC_VERB_SET_COEF_INDEX, index_reg >> 1);
	snd_hda_codec_write(codec, 0x51, 0, AC_VERB_SET_PROC_COEF, val & 0xffff); /* LSB */
	snd_hda_codec_write(codec, 0x51, 0, AC_VERB_SET_PROC_COEF, val >> 16); /* MSB */
}

static int alc5505_coef_get(struct hda_codec *codec, unsigned int index_reg)
{
	unsigned int val;

	snd_hda_codec_write(codec, 0x51, 0, AC_VERB_SET_COEF_INDEX, index_reg >> 1);
	val = snd_hda_codec_read(codec, 0x51, 0, AC_VERB_GET_PROC_COEF, 0)
		& 0xffff;
	val |= snd_hda_codec_read(codec, 0x51, 0, AC_VERB_GET_PROC_COEF, 0)
		<< 16;
	return val;
}

static void alc5505_dsp_halt(struct hda_codec *codec)
{
	unsigned int val;

	alc5505_coef_set(codec, 0x3000, 0x000c); /* DSP CPU stop */
	alc5505_coef_set(codec, 0x880c, 0x0008); /* DDR enter self refresh */
	alc5505_coef_set(codec, 0x61c0, 0x11110080); /* Clock control for PLL and CPU */
	alc5505_coef_set(codec, 0x6230, 0xfc0d4011); /* Disable Input OP */
	alc5505_coef_set(codec, 0x61b4, 0x040a2b03); /* Stop PLL2 */
	alc5505_coef_set(codec, 0x61b0, 0x00005b17); /* Stop PLL1 */
	alc5505_coef_set(codec, 0x61b8, 0x04133303); /* Stop PLL3 */
	val = alc5505_coef_get(codec, 0x6220);
	alc5505_coef_set(codec, 0x6220, (val | 0x3000)); /* switch Ringbuffer clock to DBUS clock */
}

static void alc5505_dsp_back_from_halt(struct hda_codec *codec)
{
	alc5505_coef_set(codec, 0x61b8, 0x04133302);
	alc5505_coef_set(codec, 0x61b0, 0x00005b16);
	alc5505_coef_set(codec, 0x61b4, 0x040a2b02);
	alc5505_coef_set(codec, 0x6230, 0xf80d4011);
	alc5505_coef_set(codec, 0x6220, 0x2002010f);
	alc5505_coef_set(codec, 0x880c, 0x00000004);
}

static void alc5505_dsp_init(struct hda_codec *codec)
{
	unsigned int val;

	alc5505_dsp_halt(codec);
	alc5505_dsp_back_from_halt(codec);
	alc5505_coef_set(codec, 0x61b0, 0x5b14); /* PLL1 control */
	alc5505_coef_set(codec, 0x61b0, 0x5b16);
	alc5505_coef_set(codec, 0x61b4, 0x04132b00); /* PLL2 control */
	alc5505_coef_set(codec, 0x61b4, 0x04132b02);
	alc5505_coef_set(codec, 0x61b8, 0x041f3300); /* PLL3 control*/
	alc5505_coef_set(codec, 0x61b8, 0x041f3302);
	snd_hda_codec_write(codec, 0x51, 0, AC_VERB_SET_CODEC_RESET, 0); /* Function reset */
	alc5505_coef_set(codec, 0x61b8, 0x041b3302);
	alc5505_coef_set(codec, 0x61b8, 0x04173302);
	alc5505_coef_set(codec, 0x61b8, 0x04163302);
	alc5505_coef_set(codec, 0x8800, 0x348b328b); /* DRAM control */
	alc5505_coef_set(codec, 0x8808, 0x00020022); /* DRAM control */
	alc5505_coef_set(codec, 0x8818, 0x00000400); /* DRAM control */

	val = alc5505_coef_get(codec, 0x6200) >> 16; /* Read revision ID */
	if (val <= 3)
		alc5505_coef_set(codec, 0x6220, 0x2002010f); /* I/O PAD Configuration */
	else
		alc5505_coef_set(codec, 0x6220, 0x6002018f);

	alc5505_coef_set(codec, 0x61ac, 0x055525f0); /**/
	alc5505_coef_set(codec, 0x61c0, 0x12230080); /* Clock control */
	alc5505_coef_set(codec, 0x61b4, 0x040e2b02); /* PLL2 control */
	alc5505_coef_set(codec, 0x61bc, 0x010234f8); /* OSC Control */
	alc5505_coef_set(codec, 0x880c, 0x00000004); /* DRAM Function control */
	alc5505_coef_set(codec, 0x880c, 0x00000003);
	alc5505_coef_set(codec, 0x880c, 0x00000010);

#ifdef HALT_REALTEK_ALC5505
	alc5505_dsp_halt(codec);
#endif
}

#ifdef HALT_REALTEK_ALC5505
#define alc5505_dsp_suspend(codec)	do { } while (0) /* NOP */
#define alc5505_dsp_resume(codec)	do { } while (0) /* NOP */
#else
#define alc5505_dsp_suspend(codec)	alc5505_dsp_halt(codec)
#define alc5505_dsp_resume(codec)	alc5505_dsp_back_from_halt(codec)
#endif

#ifdef CONFIG_PM
static int alc269_suspend(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;

	if (spec->has_alc5505_dsp)
		alc5505_dsp_suspend(codec);

	return alc_suspend(codec);
}

static int alc269_resume(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;

	if (spec->codec_variant == ALC269_TYPE_ALC269VB)
		alc269vb_toggle_power_output(codec, 0);
	if (spec->codec_variant == ALC269_TYPE_ALC269VB &&
			(alc_get_coef0(codec) & 0x00ff) == 0x018) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		msleep(150);
	}

	codec->patch_ops.init(codec);

<<<<<<< HEAD
	if ((alc_get_coef0(codec) & 0x00ff) == 0x017) {
		alc269_toggle_power_output(codec, 1);
		msleep(200);
	}

	if ((alc_get_coef0(codec) & 0x00ff) == 0x018)
		alc269_toggle_power_output(codec, 1);

	snd_hda_codec_resume_amp(codec);
	snd_hda_codec_resume_cache(codec);
	hda_call_check_power_status(codec, 0x01);
=======
	if (spec->codec_variant == ALC269_TYPE_ALC269VB)
		alc269vb_toggle_power_output(codec, 1);
	if (spec->codec_variant == ALC269_TYPE_ALC269VB &&
			(alc_get_coef0(codec) & 0x00ff) == 0x017) {
		msleep(200);
	}

	snd_hda_regmap_sync(codec);
	hda_call_check_power_status(codec, 0x01);

	/* on some machine, the BIOS will clear the codec gpio data when enter
	 * suspend, and won't restore the data after resume, so we restore it
	 * in the driver.
	 */
	if (spec->gpio_data)
		alc_write_gpio_data(codec);

	if (spec->has_alc5505_dsp)
		alc5505_dsp_resume(codec);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}
#endif /* CONFIG_PM */

static void alc269_fixup_pincfg_no_hp_to_lineout(struct hda_codec *codec,
<<<<<<< HEAD
						 const struct alc_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;

	if (action == ALC_FIXUP_ACT_PRE_PROBE)
		spec->parse_flags = HDA_PINCFG_NO_HP_FIXUP;
}

static void alc269_fixup_hweq(struct hda_codec *codec,
			       const struct alc_fixup *fix, int action)
{
	int coef;

	if (action != ALC_FIXUP_ACT_INIT)
		return;
	coef = alc_read_coef_idx(codec, 0x1e);
	alc_write_coef_idx(codec, 0x1e, coef | 0x80);
}

static void alc271_fixup_dmic(struct hda_codec *codec,
			      const struct alc_fixup *fix, int action)
=======
						 const struct hda_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;

	if (action == HDA_FIXUP_ACT_PRE_PROBE)
		spec->parse_flags = HDA_PINCFG_NO_HP_FIXUP;
}

static void alc269_fixup_pincfg_U7x7_headset_mic(struct hda_codec *codec,
						 const struct hda_fixup *fix,
						 int action)
{
	unsigned int cfg_headphone = snd_hda_codec_get_pincfg(codec, 0x21);
	unsigned int cfg_headset_mic = snd_hda_codec_get_pincfg(codec, 0x19);

	if (cfg_headphone && cfg_headset_mic == 0x411111f0)
		snd_hda_codec_set_pincfg(codec, 0x19,
			(cfg_headphone & ~AC_DEFCFG_DEVICE) |
			(AC_JACK_MIC_IN << AC_DEFCFG_DEVICE_SHIFT));
}

static void alc269_fixup_hweq(struct hda_codec *codec,
			       const struct hda_fixup *fix, int action)
{
	if (action == HDA_FIXUP_ACT_INIT)
		alc_update_coef_idx(codec, 0x1e, 0, 0x80);
}

static void alc269_fixup_headset_mic(struct hda_codec *codec,
				       const struct hda_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;

	if (action == HDA_FIXUP_ACT_PRE_PROBE)
		spec->parse_flags |= HDA_PINCFG_HEADSET_MIC;
}

static void alc271_fixup_dmic(struct hda_codec *codec,
			      const struct hda_fixup *fix, int action)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	static const struct hda_verb verbs[] = {
		{0x20, AC_VERB_SET_COEF_INDEX, 0x0d},
		{0x20, AC_VERB_SET_PROC_COEF, 0x4000},
		{}
	};
	unsigned int cfg;

<<<<<<< HEAD
	if (strcmp(codec->chip_name, "ALC271X"))
=======
	if (strcmp(codec->core.chip_name, "ALC271X") &&
	    strcmp(codec->core.chip_name, "ALC269VB"))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;
	cfg = snd_hda_codec_get_pincfg(codec, 0x12);
	if (get_defcfg_connect(cfg) == AC_JACK_PORT_FIXED)
		snd_hda_sequence_write(codec, verbs);
}

<<<<<<< HEAD
static void alc269_fixup_pcm_44k(struct hda_codec *codec,
				 const struct alc_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;

	if (action != ALC_FIXUP_ACT_PROBE)
=======
/* Fix the speaker amp after resume, etc */
static void alc269vb_fixup_aspire_e1_coef(struct hda_codec *codec,
					  const struct hda_fixup *fix,
					  int action)
{
	if (action == HDA_FIXUP_ACT_INIT)
		alc_update_coef_idx(codec, 0x0d, 0x6000, 0x6000);
}

static void alc269_fixup_pcm_44k(struct hda_codec *codec,
				 const struct hda_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;

	if (action != HDA_FIXUP_ACT_PROBE)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;

	/* Due to a hardware problem on Lenovo Ideadpad, we need to
	 * fix the sample rate of analog I/O to 44.1kHz
	 */
<<<<<<< HEAD
	spec->stream_analog_playback = &alc269_44k_pcm_analog_playback;
	spec->stream_analog_capture = &alc269_44k_pcm_analog_capture;
}

static void alc269_fixup_stereo_dmic(struct hda_codec *codec,
				     const struct alc_fixup *fix, int action)
{
	int coef;

	if (action != ALC_FIXUP_ACT_INIT)
		return;
=======
	spec->gen.stream_analog_playback = &alc269_44k_pcm_analog_playback;
	spec->gen.stream_analog_capture = &alc269_44k_pcm_analog_capture;
}

static void alc269_fixup_stereo_dmic(struct hda_codec *codec,
				     const struct hda_fixup *fix, int action)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* The digital-mic unit sends PDM (differential signal) instead of
	 * the standard PCM, thus you can't record a valid mono stream as is.
	 * Below is a workaround specific to ALC269 to control the dmic
	 * signal source as mono.
	 */
<<<<<<< HEAD
	coef = alc_read_coef_idx(codec, 0x07);
	alc_write_coef_idx(codec, 0x07, coef | 0x80);
=======
	if (action == HDA_FIXUP_ACT_INIT)
		alc_update_coef_idx(codec, 0x07, 0, 0x80);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void alc269_quanta_automute(struct hda_codec *codec)
{
<<<<<<< HEAD
	update_outputs(codec);

	snd_hda_codec_write(codec, 0x20, 0,
			AC_VERB_SET_COEF_INDEX, 0x0c);
	snd_hda_codec_write(codec, 0x20, 0,
			AC_VERB_SET_PROC_COEF, 0x680);

	snd_hda_codec_write(codec, 0x20, 0,
			AC_VERB_SET_COEF_INDEX, 0x0c);
	snd_hda_codec_write(codec, 0x20, 0,
			AC_VERB_SET_PROC_COEF, 0x480);
}

static void alc269_fixup_quanta_mute(struct hda_codec *codec,
				     const struct alc_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;
	if (action != ALC_FIXUP_ACT_PROBE)
		return;
	spec->automute_hook = alc269_quanta_automute;
}

/* update mute-LED according to the speaker mute state via mic2 VREF pin */
static void alc269_fixup_mic2_mute_hook(void *private_data, int enabled)
{
	struct hda_codec *codec = private_data;
	unsigned int pinval = enabled ? 0x20 : 0x24;
	snd_hda_codec_update_cache(codec, 0x19, 0,
				   AC_VERB_SET_PIN_WIDGET_CONTROL,
				   pinval);
}

static void alc269_fixup_mic2_mute(struct hda_codec *codec,
				   const struct alc_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;
	switch (action) {
	case ALC_FIXUP_ACT_BUILD:
		spec->vmaster_mute.hook = alc269_fixup_mic2_mute_hook;
		snd_hda_add_vmaster_hook(codec, &spec->vmaster_mute, true);
		/* fallthru */
	case ALC_FIXUP_ACT_INIT:
		snd_hda_sync_vmaster_hook(&spec->vmaster_mute);
=======
	snd_hda_gen_update_outputs(codec);

	alc_write_coef_idx(codec, 0x0c, 0x680);
	alc_write_coef_idx(codec, 0x0c, 0x480);
}

static void alc269_fixup_quanta_mute(struct hda_codec *codec,
				     const struct hda_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;
	if (action != HDA_FIXUP_ACT_PROBE)
		return;
	spec->gen.automute_hook = alc269_quanta_automute;
}

static void alc269_x101_hp_automute_hook(struct hda_codec *codec,
					 struct hda_jack_callback *jack)
{
	struct alc_spec *spec = codec->spec;
	int vref;
	msleep(200);
	snd_hda_gen_hp_automute(codec, jack);

	vref = spec->gen.hp_jack_present ? PIN_VREF80 : 0;
	msleep(100);
	snd_hda_codec_write(codec, 0x18, 0, AC_VERB_SET_PIN_WIDGET_CONTROL,
			    vref);
	msleep(500);
	snd_hda_codec_write(codec, 0x18, 0, AC_VERB_SET_PIN_WIDGET_CONTROL,
			    vref);
}

/*
 * Magic sequence to make Huawei Matebook X right speaker working (bko#197801)
 */
struct hda_alc298_mbxinit {
	unsigned char value_0x23;
	unsigned char value_0x25;
};

static void alc298_huawei_mbx_stereo_seq(struct hda_codec *codec,
					 const struct hda_alc298_mbxinit *initval,
					 bool first)
{
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_DIGI_CONVERT_3, 0x0);
	alc_write_coef_idx(codec, 0x26, 0xb000);

	if (first)
		snd_hda_codec_write(codec, 0x21, 0, AC_VERB_GET_PIN_SENSE, 0x0);

	snd_hda_codec_write(codec, 0x6, 0, AC_VERB_SET_DIGI_CONVERT_3, 0x80);
	alc_write_coef_idx(codec, 0x26, 0xf000);
	alc_write_coef_idx(codec, 0x23, initval->value_0x23);

	if (initval->value_0x23 != 0x1e)
		alc_write_coef_idx(codec, 0x25, initval->value_0x25);

	snd_hda_codec_write(codec, 0x20, 0, AC_VERB_SET_COEF_INDEX, 0x26);
	snd_hda_codec_write(codec, 0x20, 0, AC_VERB_SET_PROC_COEF, 0xb010);
}

static void alc298_fixup_huawei_mbx_stereo(struct hda_codec *codec,
					   const struct hda_fixup *fix,
					   int action)
{
	/* Initialization magic */
	static const struct hda_alc298_mbxinit dac_init[] = {
		{0x0c, 0x00}, {0x0d, 0x00}, {0x0e, 0x00}, {0x0f, 0x00},
		{0x10, 0x00}, {0x1a, 0x40}, {0x1b, 0x82}, {0x1c, 0x00},
		{0x1d, 0x00}, {0x1e, 0x00}, {0x1f, 0x00},
		{0x20, 0xc2}, {0x21, 0xc8}, {0x22, 0x26}, {0x23, 0x24},
		{0x27, 0xff}, {0x28, 0xff}, {0x29, 0xff}, {0x2a, 0x8f},
		{0x2b, 0x02}, {0x2c, 0x48}, {0x2d, 0x34}, {0x2e, 0x00},
		{0x2f, 0x00},
		{0x30, 0x00}, {0x31, 0x00}, {0x32, 0x00}, {0x33, 0x00},
		{0x34, 0x00}, {0x35, 0x01}, {0x36, 0x93}, {0x37, 0x0c},
		{0x38, 0x00}, {0x39, 0x00}, {0x3a, 0xf8}, {0x38, 0x80},
		{}
	};
	const struct hda_alc298_mbxinit *seq;

	if (action != HDA_FIXUP_ACT_INIT)
		return;

	/* Start */
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_DIGI_CONVERT_3, 0x00);
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_DIGI_CONVERT_3, 0x80);
	alc_write_coef_idx(codec, 0x26, 0xf000);
	alc_write_coef_idx(codec, 0x22, 0x31);
	alc_write_coef_idx(codec, 0x23, 0x0b);
	alc_write_coef_idx(codec, 0x25, 0x00);
	snd_hda_codec_write(codec, 0x20, 0, AC_VERB_SET_COEF_INDEX, 0x26);
	snd_hda_codec_write(codec, 0x20, 0, AC_VERB_SET_PROC_COEF, 0xb010);

	for (seq = dac_init; seq->value_0x23; seq++)
		alc298_huawei_mbx_stereo_seq(codec, seq, seq == dac_init);
}

static void alc269_fixup_x101_headset_mic(struct hda_codec *codec,
				     const struct hda_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;
	if (action == HDA_FIXUP_ACT_PRE_PROBE) {
		spec->parse_flags |= HDA_PINCFG_HEADSET_MIC;
		spec->gen.hp_automute_hook = alc269_x101_hp_automute_hook;
	}
}

static void alc_update_vref_led(struct hda_codec *codec, hda_nid_t pin,
				bool polarity, bool on)
{
	unsigned int pinval;

	if (!pin)
		return;
	if (polarity)
		on = !on;
	pinval = snd_hda_codec_get_pin_target(codec, pin);
	pinval &= ~AC_PINCTL_VREFEN;
	pinval |= on ? AC_PINCTL_VREF_80 : AC_PINCTL_VREF_HIZ;
	/* temporarily power up/down for setting VREF */
	snd_hda_power_up_pm(codec);
	snd_hda_set_pin_ctl_cache(codec, pin, pinval);
	snd_hda_power_down_pm(codec);
}

/* update mute-LED according to the speaker mute state via mic VREF pin */
static int vref_mute_led_set(struct led_classdev *led_cdev,
			     enum led_brightness brightness)
{
	struct hda_codec *codec = dev_to_hda_codec(led_cdev->dev->parent);
	struct alc_spec *spec = codec->spec;

	alc_update_vref_led(codec, spec->mute_led_nid,
			    spec->mute_led_polarity, brightness);
	return 0;
}

/* Make sure the led works even in runtime suspend */
static unsigned int led_power_filter(struct hda_codec *codec,
						  hda_nid_t nid,
						  unsigned int power_state)
{
	struct alc_spec *spec = codec->spec;

	if (power_state != AC_PWRST_D3 || nid == 0 ||
	    (nid != spec->mute_led_nid && nid != spec->cap_mute_led_nid))
		return power_state;

	/* Set pin ctl again, it might have just been set to 0 */
	snd_hda_set_pin_ctl(codec, nid,
			    snd_hda_codec_get_pin_target(codec, nid));

	return snd_hda_gen_path_power_filter(codec, nid, power_state);
}

static void alc269_fixup_hp_mute_led(struct hda_codec *codec,
				     const struct hda_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;
	const struct dmi_device *dev = NULL;

	if (action != HDA_FIXUP_ACT_PRE_PROBE)
		return;

	while ((dev = dmi_find_device(DMI_DEV_TYPE_OEM_STRING, NULL, dev))) {
		int pol, pin;
		if (sscanf(dev->name, "HP_Mute_LED_%d_%x", &pol, &pin) != 2)
			continue;
		if (pin < 0x0a || pin >= 0x10)
			break;
		spec->mute_led_polarity = pol;
		spec->mute_led_nid = pin - 0x0a + 0x18;
		snd_hda_gen_add_mute_led_cdev(codec, vref_mute_led_set);
		codec->power_filter = led_power_filter;
		codec_dbg(codec,
			  "Detected mute LED for %x:%d\n", spec->mute_led_nid,
			   spec->mute_led_polarity);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	}
}

<<<<<<< HEAD
enum {
=======
static void alc269_fixup_hp_mute_led_micx(struct hda_codec *codec,
					  const struct hda_fixup *fix,
					  int action, hda_nid_t pin)
{
	struct alc_spec *spec = codec->spec;

	if (action == HDA_FIXUP_ACT_PRE_PROBE) {
		spec->mute_led_polarity = 0;
		spec->mute_led_nid = pin;
		snd_hda_gen_add_mute_led_cdev(codec, vref_mute_led_set);
		codec->power_filter = led_power_filter;
	}
}

static void alc269_fixup_hp_mute_led_mic1(struct hda_codec *codec,
				const struct hda_fixup *fix, int action)
{
	alc269_fixup_hp_mute_led_micx(codec, fix, action, 0x18);
}

static void alc269_fixup_hp_mute_led_mic2(struct hda_codec *codec,
				const struct hda_fixup *fix, int action)
{
	alc269_fixup_hp_mute_led_micx(codec, fix, action, 0x19);
}

static void alc269_fixup_hp_mute_led_mic3(struct hda_codec *codec,
				const struct hda_fixup *fix, int action)
{
	alc269_fixup_hp_mute_led_micx(codec, fix, action, 0x1b);
}

/* update LED status via GPIO */
static void alc_update_gpio_led(struct hda_codec *codec, unsigned int mask,
				int polarity, bool enabled)
{
	if (polarity)
		enabled = !enabled;
	alc_update_gpio_data(codec, mask, !enabled); /* muted -> LED on */
}

/* turn on/off mute LED via GPIO per vmaster hook */
static int gpio_mute_led_set(struct led_classdev *led_cdev,
			     enum led_brightness brightness)
{
	struct hda_codec *codec = dev_to_hda_codec(led_cdev->dev->parent);
	struct alc_spec *spec = codec->spec;

	alc_update_gpio_led(codec, spec->gpio_mute_led_mask,
			    spec->mute_led_polarity, !brightness);
	return 0;
}

/* turn on/off mic-mute LED via GPIO per capture hook */
static int micmute_led_set(struct led_classdev *led_cdev,
			   enum led_brightness brightness)
{
	struct hda_codec *codec = dev_to_hda_codec(led_cdev->dev->parent);
	struct alc_spec *spec = codec->spec;

	alc_update_gpio_led(codec, spec->gpio_mic_led_mask,
			    spec->micmute_led_polarity, !brightness);
	return 0;
}

/* setup mute and mic-mute GPIO bits, add hooks appropriately */
static void alc_fixup_hp_gpio_led(struct hda_codec *codec,
				  int action,
				  unsigned int mute_mask,
				  unsigned int micmute_mask)
{
	struct alc_spec *spec = codec->spec;

	alc_fixup_gpio(codec, action, mute_mask | micmute_mask);

	if (action != HDA_FIXUP_ACT_PRE_PROBE)
		return;
	if (mute_mask) {
		spec->gpio_mute_led_mask = mute_mask;
		snd_hda_gen_add_mute_led_cdev(codec, gpio_mute_led_set);
	}
	if (micmute_mask) {
		spec->gpio_mic_led_mask = micmute_mask;
		snd_hda_gen_add_micmute_led_cdev(codec, micmute_led_set);
	}
}

static void alc236_fixup_hp_gpio_led(struct hda_codec *codec,
				const struct hda_fixup *fix, int action)
{
	alc_fixup_hp_gpio_led(codec, action, 0x02, 0x01);
}

static void alc269_fixup_hp_gpio_led(struct hda_codec *codec,
				const struct hda_fixup *fix, int action)
{
	alc_fixup_hp_gpio_led(codec, action, 0x08, 0x10);
}

static void alc285_fixup_hp_gpio_led(struct hda_codec *codec,
				const struct hda_fixup *fix, int action)
{
	alc_fixup_hp_gpio_led(codec, action, 0x04, 0x01);
}

static void alc286_fixup_hp_gpio_led(struct hda_codec *codec,
				const struct hda_fixup *fix, int action)
{
	alc_fixup_hp_gpio_led(codec, action, 0x02, 0x20);
}

static void alc287_fixup_hp_gpio_led(struct hda_codec *codec,
				const struct hda_fixup *fix, int action)
{
	alc_fixup_hp_gpio_led(codec, action, 0x10, 0);
}

static void alc245_fixup_hp_gpio_led(struct hda_codec *codec,
				const struct hda_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;

	if (action == HDA_FIXUP_ACT_PRE_PROBE)
		spec->micmute_led_polarity = 1;
	alc_fixup_hp_gpio_led(codec, action, 0, 0x04);
}

/* turn on/off mic-mute LED per capture hook via VREF change */
static int vref_micmute_led_set(struct led_classdev *led_cdev,
				enum led_brightness brightness)
{
	struct hda_codec *codec = dev_to_hda_codec(led_cdev->dev->parent);
	struct alc_spec *spec = codec->spec;

	alc_update_vref_led(codec, spec->cap_mute_led_nid,
			    spec->micmute_led_polarity, brightness);
	return 0;
}

static void alc269_fixup_hp_gpio_mic1_led(struct hda_codec *codec,
				const struct hda_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;

	alc_fixup_hp_gpio_led(codec, action, 0x08, 0);
	if (action == HDA_FIXUP_ACT_PRE_PROBE) {
		/* Like hp_gpio_mic1_led, but also needs GPIO4 low to
		 * enable headphone amp
		 */
		spec->gpio_mask |= 0x10;
		spec->gpio_dir |= 0x10;
		spec->cap_mute_led_nid = 0x18;
		snd_hda_gen_add_micmute_led_cdev(codec, vref_micmute_led_set);
		codec->power_filter = led_power_filter;
	}
}

static void alc280_fixup_hp_gpio4(struct hda_codec *codec,
				   const struct hda_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;

	alc_fixup_hp_gpio_led(codec, action, 0x08, 0);
	if (action == HDA_FIXUP_ACT_PRE_PROBE) {
		spec->cap_mute_led_nid = 0x18;
		snd_hda_gen_add_micmute_led_cdev(codec, vref_micmute_led_set);
		codec->power_filter = led_power_filter;
	}
}

/* HP Spectre x360 14 model needs a unique workaround for enabling the amp;
 * it needs to toggle the GPIO0 once on and off at each time (bko#210633)
 */
static void alc245_fixup_hp_x360_amp(struct hda_codec *codec,
				     const struct hda_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;

	switch (action) {
	case HDA_FIXUP_ACT_PRE_PROBE:
		spec->gpio_mask |= 0x01;
		spec->gpio_dir |= 0x01;
		break;
	case HDA_FIXUP_ACT_INIT:
		/* need to toggle GPIO to enable the amp */
		alc_update_gpio_data(codec, 0x01, true);
		msleep(100);
		alc_update_gpio_data(codec, 0x01, false);
		break;
	}
}

/* toggle GPIO2 at each time stream is started; we use PREPARE state instead */
static void alc274_hp_envy_pcm_hook(struct hda_pcm_stream *hinfo,
				    struct hda_codec *codec,
				    struct snd_pcm_substream *substream,
				    int action)
{
	switch (action) {
	case HDA_GEN_PCM_ACT_PREPARE:
		alc_update_gpio_data(codec, 0x04, true);
		break;
	case HDA_GEN_PCM_ACT_CLEANUP:
		alc_update_gpio_data(codec, 0x04, false);
		break;
	}
}

static void alc274_fixup_hp_envy_gpio(struct hda_codec *codec,
				      const struct hda_fixup *fix,
				      int action)
{
	struct alc_spec *spec = codec->spec;

	if (action == HDA_FIXUP_ACT_PROBE) {
		spec->gpio_mask |= 0x04;
		spec->gpio_dir |= 0x04;
		spec->gen.pcm_playback_hook = alc274_hp_envy_pcm_hook;
	}
}

static void alc_update_coef_led(struct hda_codec *codec,
				struct alc_coef_led *led,
				bool polarity, bool on)
{
	if (polarity)
		on = !on;
	/* temporarily power up/down for setting COEF bit */
	alc_update_coef_idx(codec, led->idx, led->mask,
			    on ? led->on : led->off);
}

/* update mute-LED according to the speaker mute state via COEF bit */
static int coef_mute_led_set(struct led_classdev *led_cdev,
			     enum led_brightness brightness)
{
	struct hda_codec *codec = dev_to_hda_codec(led_cdev->dev->parent);
	struct alc_spec *spec = codec->spec;

	alc_update_coef_led(codec, &spec->mute_led_coef,
			    spec->mute_led_polarity, brightness);
	return 0;
}

static void alc285_fixup_hp_mute_led_coefbit(struct hda_codec *codec,
					  const struct hda_fixup *fix,
					  int action)
{
	struct alc_spec *spec = codec->spec;

	if (action == HDA_FIXUP_ACT_PRE_PROBE) {
		spec->mute_led_polarity = 0;
		spec->mute_led_coef.idx = 0x0b;
		spec->mute_led_coef.mask = 1 << 3;
		spec->mute_led_coef.on = 1 << 3;
		spec->mute_led_coef.off = 0;
		snd_hda_gen_add_mute_led_cdev(codec, coef_mute_led_set);
	}
}

static void alc236_fixup_hp_mute_led_coefbit(struct hda_codec *codec,
					  const struct hda_fixup *fix,
					  int action)
{
	struct alc_spec *spec = codec->spec;

	if (action == HDA_FIXUP_ACT_PRE_PROBE) {
		spec->mute_led_polarity = 0;
		spec->mute_led_coef.idx = 0x34;
		spec->mute_led_coef.mask = 1 << 5;
		spec->mute_led_coef.on = 0;
		spec->mute_led_coef.off = 1 << 5;
		snd_hda_gen_add_mute_led_cdev(codec, coef_mute_led_set);
	}
}

static void alc236_fixup_hp_mute_led_coefbit2(struct hda_codec *codec,
					  const struct hda_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;

	if (action == HDA_FIXUP_ACT_PRE_PROBE) {
		spec->mute_led_polarity = 0;
		spec->mute_led_coef.idx = 0x07;
		spec->mute_led_coef.mask = 1;
		spec->mute_led_coef.on = 1;
		spec->mute_led_coef.off = 0;
		snd_hda_gen_add_mute_led_cdev(codec, coef_mute_led_set);
	}
}

static void alc245_fixup_hp_mute_led_coefbit(struct hda_codec *codec,
					  const struct hda_fixup *fix,
					  int action)
{
	struct alc_spec *spec = codec->spec;

	if (action == HDA_FIXUP_ACT_PRE_PROBE) {
		spec->mute_led_polarity = 0;
		spec->mute_led_coef.idx = 0x0b;
		spec->mute_led_coef.mask = 3 << 2;
		spec->mute_led_coef.on = 2 << 2;
		spec->mute_led_coef.off = 1 << 2;
		snd_hda_gen_add_mute_led_cdev(codec, coef_mute_led_set);
	}
}

/* turn on/off mic-mute LED per capture hook by coef bit */
static int coef_micmute_led_set(struct led_classdev *led_cdev,
				enum led_brightness brightness)
{
	struct hda_codec *codec = dev_to_hda_codec(led_cdev->dev->parent);
	struct alc_spec *spec = codec->spec;

	alc_update_coef_led(codec, &spec->mic_led_coef,
			    spec->micmute_led_polarity, brightness);
	return 0;
}

static void alc285_fixup_hp_coef_micmute_led(struct hda_codec *codec,
				const struct hda_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;

	if (action == HDA_FIXUP_ACT_PRE_PROBE) {
		spec->mic_led_coef.idx = 0x19;
		spec->mic_led_coef.mask = 1 << 13;
		spec->mic_led_coef.on = 1 << 13;
		spec->mic_led_coef.off = 0;
		snd_hda_gen_add_micmute_led_cdev(codec, coef_micmute_led_set);
	}
}

static void alc285_fixup_hp_gpio_micmute_led(struct hda_codec *codec,
				const struct hda_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;

	if (action == HDA_FIXUP_ACT_PRE_PROBE)
		spec->micmute_led_polarity = 1;
	alc_fixup_hp_gpio_led(codec, action, 0, 0x04);
}

static void alc236_fixup_hp_coef_micmute_led(struct hda_codec *codec,
				const struct hda_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;

	if (action == HDA_FIXUP_ACT_PRE_PROBE) {
		spec->mic_led_coef.idx = 0x35;
		spec->mic_led_coef.mask = 3 << 2;
		spec->mic_led_coef.on = 2 << 2;
		spec->mic_led_coef.off = 1 << 2;
		snd_hda_gen_add_micmute_led_cdev(codec, coef_micmute_led_set);
	}
}

static void alc285_fixup_hp_mute_led(struct hda_codec *codec,
				const struct hda_fixup *fix, int action)
{
	alc285_fixup_hp_mute_led_coefbit(codec, fix, action);
	alc285_fixup_hp_coef_micmute_led(codec, fix, action);
}

static void alc285_fixup_hp_spectre_x360_mute_led(struct hda_codec *codec,
				const struct hda_fixup *fix, int action)
{
	alc285_fixup_hp_mute_led_coefbit(codec, fix, action);
	alc285_fixup_hp_gpio_micmute_led(codec, fix, action);
}

static void alc236_fixup_hp_mute_led(struct hda_codec *codec,
				const struct hda_fixup *fix, int action)
{
	alc236_fixup_hp_mute_led_coefbit(codec, fix, action);
	alc236_fixup_hp_coef_micmute_led(codec, fix, action);
}

static void alc236_fixup_hp_micmute_led_vref(struct hda_codec *codec,
				const struct hda_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;

	if (action == HDA_FIXUP_ACT_PRE_PROBE) {
		spec->cap_mute_led_nid = 0x1a;
		snd_hda_gen_add_micmute_led_cdev(codec, vref_micmute_led_set);
		codec->power_filter = led_power_filter;
	}
}

static void alc236_fixup_hp_mute_led_micmute_vref(struct hda_codec *codec,
				const struct hda_fixup *fix, int action)
{
	alc236_fixup_hp_mute_led_coefbit(codec, fix, action);
	alc236_fixup_hp_micmute_led_vref(codec, fix, action);
}

static inline void alc298_samsung_write_coef_pack(struct hda_codec *codec,
						  const unsigned short coefs[2])
{
	alc_write_coef_idx(codec, 0x23, coefs[0]);
	alc_write_coef_idx(codec, 0x25, coefs[1]);
	alc_write_coef_idx(codec, 0x26, 0xb011);
}

struct alc298_samsung_amp_desc {
	unsigned char nid;
	unsigned short init_seq[2][2];
};

static void alc298_fixup_samsung_amp(struct hda_codec *codec,
				     const struct hda_fixup *fix, int action)
{
	int i, j;
	static const unsigned short init_seq[][2] = {
		{ 0x19, 0x00 }, { 0x20, 0xc0 }, { 0x22, 0x44 }, { 0x23, 0x08 },
		{ 0x24, 0x85 }, { 0x25, 0x41 }, { 0x35, 0x40 }, { 0x36, 0x01 },
		{ 0x38, 0x81 }, { 0x3a, 0x03 }, { 0x3b, 0x81 }, { 0x40, 0x3e },
		{ 0x41, 0x07 }, { 0x400, 0x1 }
	};
	static const struct alc298_samsung_amp_desc amps[] = {
		{ 0x3a, { { 0x18, 0x1 }, { 0x26, 0x0 } } },
		{ 0x39, { { 0x18, 0x2 }, { 0x26, 0x1 } } }
	};

	if (action != HDA_FIXUP_ACT_INIT)
		return;

	for (i = 0; i < ARRAY_SIZE(amps); i++) {
		alc_write_coef_idx(codec, 0x22, amps[i].nid);

		for (j = 0; j < ARRAY_SIZE(amps[i].init_seq); j++)
			alc298_samsung_write_coef_pack(codec, amps[i].init_seq[j]);

		for (j = 0; j < ARRAY_SIZE(init_seq); j++)
			alc298_samsung_write_coef_pack(codec, init_seq[j]);
	}
}

#if IS_REACHABLE(CONFIG_INPUT)
static void gpio2_mic_hotkey_event(struct hda_codec *codec,
				   struct hda_jack_callback *event)
{
	struct alc_spec *spec = codec->spec;

	/* GPIO2 just toggles on a keypress/keyrelease cycle. Therefore
	   send both key on and key off event for every interrupt. */
	input_report_key(spec->kb_dev, spec->alc_mute_keycode_map[ALC_KEY_MICMUTE_INDEX], 1);
	input_sync(spec->kb_dev);
	input_report_key(spec->kb_dev, spec->alc_mute_keycode_map[ALC_KEY_MICMUTE_INDEX], 0);
	input_sync(spec->kb_dev);
}

static int alc_register_micmute_input_device(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	int i;

	spec->kb_dev = input_allocate_device();
	if (!spec->kb_dev) {
		codec_err(codec, "Out of memory (input_allocate_device)\n");
		return -ENOMEM;
	}

	spec->alc_mute_keycode_map[ALC_KEY_MICMUTE_INDEX] = KEY_MICMUTE;

	spec->kb_dev->name = "Microphone Mute Button";
	spec->kb_dev->evbit[0] = BIT_MASK(EV_KEY);
	spec->kb_dev->keycodesize = sizeof(spec->alc_mute_keycode_map[0]);
	spec->kb_dev->keycodemax = ARRAY_SIZE(spec->alc_mute_keycode_map);
	spec->kb_dev->keycode = spec->alc_mute_keycode_map;
	for (i = 0; i < ARRAY_SIZE(spec->alc_mute_keycode_map); i++)
		set_bit(spec->alc_mute_keycode_map[i], spec->kb_dev->keybit);

	if (input_register_device(spec->kb_dev)) {
		codec_err(codec, "input_register_device failed\n");
		input_free_device(spec->kb_dev);
		spec->kb_dev = NULL;
		return -ENOMEM;
	}

	return 0;
}

/* GPIO1 = set according to SKU external amp
 * GPIO2 = mic mute hotkey
 * GPIO3 = mute LED
 * GPIO4 = mic mute LED
 */
static void alc280_fixup_hp_gpio2_mic_hotkey(struct hda_codec *codec,
					     const struct hda_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;

	alc_fixup_hp_gpio_led(codec, action, 0x08, 0x10);
	if (action == HDA_FIXUP_ACT_PRE_PROBE) {
		spec->init_amp = ALC_INIT_DEFAULT;
		if (alc_register_micmute_input_device(codec) != 0)
			return;

		spec->gpio_mask |= 0x06;
		spec->gpio_dir |= 0x02;
		spec->gpio_data |= 0x02;
		snd_hda_codec_write_cache(codec, codec->core.afg, 0,
					  AC_VERB_SET_GPIO_UNSOLICITED_RSP_MASK, 0x04);
		snd_hda_jack_detect_enable_callback(codec, codec->core.afg,
						    gpio2_mic_hotkey_event);
		return;
	}

	if (!spec->kb_dev)
		return;

	switch (action) {
	case HDA_FIXUP_ACT_FREE:
		input_unregister_device(spec->kb_dev);
		spec->kb_dev = NULL;
	}
}

/* Line2 = mic mute hotkey
 * GPIO2 = mic mute LED
 */
static void alc233_fixup_lenovo_line2_mic_hotkey(struct hda_codec *codec,
					     const struct hda_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;

	alc_fixup_hp_gpio_led(codec, action, 0, 0x04);
	if (action == HDA_FIXUP_ACT_PRE_PROBE) {
		spec->init_amp = ALC_INIT_DEFAULT;
		if (alc_register_micmute_input_device(codec) != 0)
			return;

		snd_hda_jack_detect_enable_callback(codec, 0x1b,
						    gpio2_mic_hotkey_event);
		return;
	}

	if (!spec->kb_dev)
		return;

	switch (action) {
	case HDA_FIXUP_ACT_FREE:
		input_unregister_device(spec->kb_dev);
		spec->kb_dev = NULL;
	}
}
#else /* INPUT */
#define alc280_fixup_hp_gpio2_mic_hotkey	NULL
#define alc233_fixup_lenovo_line2_mic_hotkey	NULL
#endif /* INPUT */

static void alc269_fixup_hp_line1_mic1_led(struct hda_codec *codec,
				const struct hda_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;

	alc269_fixup_hp_mute_led_micx(codec, fix, action, 0x1a);
	if (action == HDA_FIXUP_ACT_PRE_PROBE) {
		spec->cap_mute_led_nid = 0x18;
		snd_hda_gen_add_micmute_led_cdev(codec, vref_micmute_led_set);
	}
}

static const struct coef_fw alc225_pre_hsmode[] = {
	UPDATE_COEF(0x4a, 1<<8, 0),
	UPDATE_COEFEX(0x57, 0x05, 1<<14, 0),
	UPDATE_COEF(0x63, 3<<14, 3<<14),
	UPDATE_COEF(0x4a, 3<<4, 2<<4),
	UPDATE_COEF(0x4a, 3<<10, 3<<10),
	UPDATE_COEF(0x45, 0x3f<<10, 0x34<<10),
	UPDATE_COEF(0x4a, 3<<10, 0),
	{}
};

static void alc_headset_mode_unplugged(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	static const struct coef_fw coef0255[] = {
		WRITE_COEF(0x1b, 0x0c0b), /* LDO and MISC control */
		WRITE_COEF(0x45, 0xd089), /* UAJ function set to menual mode */
		UPDATE_COEFEX(0x57, 0x05, 1<<14, 0), /* Direct Drive HP Amp control(Set to verb control)*/
		WRITE_COEF(0x06, 0x6104), /* Set MIC2 Vref gate with HP */
		WRITE_COEFEX(0x57, 0x03, 0x8aa6), /* Direct Drive HP Amp control */
		{}
	};
	static const struct coef_fw coef0256[] = {
		WRITE_COEF(0x1b, 0x0c4b), /* LDO and MISC control */
		WRITE_COEF(0x45, 0xd089), /* UAJ function set to menual mode */
		WRITE_COEF(0x06, 0x6104), /* Set MIC2 Vref gate with HP */
		WRITE_COEFEX(0x57, 0x03, 0x09a3), /* Direct Drive HP Amp control */
		UPDATE_COEFEX(0x57, 0x05, 1<<14, 0), /* Direct Drive HP Amp control(Set to verb control)*/
		{}
	};
	static const struct coef_fw coef0233[] = {
		WRITE_COEF(0x1b, 0x0c0b),
		WRITE_COEF(0x45, 0xc429),
		UPDATE_COEF(0x35, 0x4000, 0),
		WRITE_COEF(0x06, 0x2104),
		WRITE_COEF(0x1a, 0x0001),
		WRITE_COEF(0x26, 0x0004),
		WRITE_COEF(0x32, 0x42a3),
		{}
	};
	static const struct coef_fw coef0288[] = {
		UPDATE_COEF(0x4f, 0xfcc0, 0xc400),
		UPDATE_COEF(0x50, 0x2000, 0x2000),
		UPDATE_COEF(0x56, 0x0006, 0x0006),
		UPDATE_COEF(0x66, 0x0008, 0),
		UPDATE_COEF(0x67, 0x2000, 0),
		{}
	};
	static const struct coef_fw coef0298[] = {
		UPDATE_COEF(0x19, 0x1300, 0x0300),
		{}
	};
	static const struct coef_fw coef0292[] = {
		WRITE_COEF(0x76, 0x000e),
		WRITE_COEF(0x6c, 0x2400),
		WRITE_COEF(0x18, 0x7308),
		WRITE_COEF(0x6b, 0xc429),
		{}
	};
	static const struct coef_fw coef0293[] = {
		UPDATE_COEF(0x10, 7<<8, 6<<8), /* SET Line1 JD to 0 */
		UPDATE_COEFEX(0x57, 0x05, 1<<15|1<<13, 0x0), /* SET charge pump by verb */
		UPDATE_COEFEX(0x57, 0x03, 1<<10, 1<<10), /* SET EN_OSW to 1 */
		UPDATE_COEF(0x1a, 1<<3, 1<<3), /* Combo JD gating with LINE1-VREFO */
		WRITE_COEF(0x45, 0xc429), /* Set to TRS type */
		UPDATE_COEF(0x4a, 0x000f, 0x000e), /* Combo Jack auto detect */
		{}
	};
	static const struct coef_fw coef0668[] = {
		WRITE_COEF(0x15, 0x0d40),
		WRITE_COEF(0xb7, 0x802b),
		{}
	};
	static const struct coef_fw coef0225[] = {
		UPDATE_COEF(0x63, 3<<14, 0),
		{}
	};
	static const struct coef_fw coef0274[] = {
		UPDATE_COEF(0x4a, 0x0100, 0),
		UPDATE_COEFEX(0x57, 0x05, 0x4000, 0),
		UPDATE_COEF(0x6b, 0xf000, 0x5000),
		UPDATE_COEF(0x4a, 0x0010, 0),
		UPDATE_COEF(0x4a, 0x0c00, 0x0c00),
		WRITE_COEF(0x45, 0x5289),
		UPDATE_COEF(0x4a, 0x0c00, 0),
		{}
	};

	if (spec->no_internal_mic_pin) {
		alc_update_coef_idx(codec, 0x45, 0xf<<12 | 1<<10, 5<<12);
		return;
	}

	switch (codec->core.vendor_id) {
	case 0x10ec0255:
		alc_process_coef_fw(codec, coef0255);
		break;
	case 0x10ec0230:
	case 0x10ec0236:
	case 0x10ec0256:
	case 0x19e58326:
		alc_process_coef_fw(codec, coef0256);
		break;
	case 0x10ec0234:
	case 0x10ec0274:
	case 0x10ec0294:
		alc_process_coef_fw(codec, coef0274);
		break;
	case 0x10ec0233:
	case 0x10ec0283:
		alc_process_coef_fw(codec, coef0233);
		break;
	case 0x10ec0286:
	case 0x10ec0288:
		alc_process_coef_fw(codec, coef0288);
		break;
	case 0x10ec0298:
		alc_process_coef_fw(codec, coef0298);
		alc_process_coef_fw(codec, coef0288);
		break;
	case 0x10ec0292:
		alc_process_coef_fw(codec, coef0292);
		break;
	case 0x10ec0293:
		alc_process_coef_fw(codec, coef0293);
		break;
	case 0x10ec0668:
		alc_process_coef_fw(codec, coef0668);
		break;
	case 0x10ec0215:
	case 0x10ec0225:
	case 0x10ec0285:
	case 0x10ec0295:
	case 0x10ec0289:
	case 0x10ec0299:
		alc_process_coef_fw(codec, alc225_pre_hsmode);
		alc_process_coef_fw(codec, coef0225);
		break;
	case 0x10ec0867:
		alc_update_coefex_idx(codec, 0x57, 0x5, 1<<14, 0);
		break;
	}
	codec_dbg(codec, "Headset jack set to unplugged mode.\n");
}


static void alc_headset_mode_mic_in(struct hda_codec *codec, hda_nid_t hp_pin,
				    hda_nid_t mic_pin)
{
	static const struct coef_fw coef0255[] = {
		WRITE_COEFEX(0x57, 0x03, 0x8aa6),
		WRITE_COEF(0x06, 0x6100), /* Set MIC2 Vref gate to normal */
		{}
	};
	static const struct coef_fw coef0256[] = {
		UPDATE_COEFEX(0x57, 0x05, 1<<14, 1<<14), /* Direct Drive HP Amp control(Set to verb control)*/
		WRITE_COEFEX(0x57, 0x03, 0x09a3),
		WRITE_COEF(0x06, 0x6100), /* Set MIC2 Vref gate to normal */
		{}
	};
	static const struct coef_fw coef0233[] = {
		UPDATE_COEF(0x35, 0, 1<<14),
		WRITE_COEF(0x06, 0x2100),
		WRITE_COEF(0x1a, 0x0021),
		WRITE_COEF(0x26, 0x008c),
		{}
	};
	static const struct coef_fw coef0288[] = {
		UPDATE_COEF(0x4f, 0x00c0, 0),
		UPDATE_COEF(0x50, 0x2000, 0),
		UPDATE_COEF(0x56, 0x0006, 0),
		UPDATE_COEF(0x4f, 0xfcc0, 0xc400),
		UPDATE_COEF(0x66, 0x0008, 0x0008),
		UPDATE_COEF(0x67, 0x2000, 0x2000),
		{}
	};
	static const struct coef_fw coef0292[] = {
		WRITE_COEF(0x19, 0xa208),
		WRITE_COEF(0x2e, 0xacf0),
		{}
	};
	static const struct coef_fw coef0293[] = {
		UPDATE_COEFEX(0x57, 0x05, 0, 1<<15|1<<13), /* SET charge pump by verb */
		UPDATE_COEFEX(0x57, 0x03, 1<<10, 0), /* SET EN_OSW to 0 */
		UPDATE_COEF(0x1a, 1<<3, 0), /* Combo JD gating without LINE1-VREFO */
		{}
	};
	static const struct coef_fw coef0688[] = {
		WRITE_COEF(0xb7, 0x802b),
		WRITE_COEF(0xb5, 0x1040),
		UPDATE_COEF(0xc3, 0, 1<<12),
		{}
	};
	static const struct coef_fw coef0225[] = {
		UPDATE_COEFEX(0x57, 0x05, 1<<14, 1<<14),
		UPDATE_COEF(0x4a, 3<<4, 2<<4),
		UPDATE_COEF(0x63, 3<<14, 0),
		{}
	};
	static const struct coef_fw coef0274[] = {
		UPDATE_COEFEX(0x57, 0x05, 0x4000, 0x4000),
		UPDATE_COEF(0x4a, 0x0010, 0),
		UPDATE_COEF(0x6b, 0xf000, 0),
		{}
	};

	switch (codec->core.vendor_id) {
	case 0x10ec0255:
		alc_write_coef_idx(codec, 0x45, 0xc489);
		snd_hda_set_pin_ctl_cache(codec, hp_pin, 0);
		alc_process_coef_fw(codec, coef0255);
		snd_hda_set_pin_ctl_cache(codec, mic_pin, PIN_VREF50);
		break;
	case 0x10ec0230:
	case 0x10ec0236:
	case 0x10ec0256:
	case 0x19e58326:
		alc_write_coef_idx(codec, 0x45, 0xc489);
		snd_hda_set_pin_ctl_cache(codec, hp_pin, 0);
		alc_process_coef_fw(codec, coef0256);
		snd_hda_set_pin_ctl_cache(codec, mic_pin, PIN_VREF50);
		break;
	case 0x10ec0234:
	case 0x10ec0274:
	case 0x10ec0294:
		alc_write_coef_idx(codec, 0x45, 0x4689);
		snd_hda_set_pin_ctl_cache(codec, hp_pin, 0);
		alc_process_coef_fw(codec, coef0274);
		snd_hda_set_pin_ctl_cache(codec, mic_pin, PIN_VREF50);
		break;
	case 0x10ec0233:
	case 0x10ec0283:
		alc_write_coef_idx(codec, 0x45, 0xc429);
		snd_hda_set_pin_ctl_cache(codec, hp_pin, 0);
		alc_process_coef_fw(codec, coef0233);
		snd_hda_set_pin_ctl_cache(codec, mic_pin, PIN_VREF50);
		break;
	case 0x10ec0286:
	case 0x10ec0288:
	case 0x10ec0298:
		snd_hda_set_pin_ctl_cache(codec, hp_pin, 0);
		alc_process_coef_fw(codec, coef0288);
		snd_hda_set_pin_ctl_cache(codec, mic_pin, PIN_VREF50);
		break;
	case 0x10ec0292:
		snd_hda_set_pin_ctl_cache(codec, hp_pin, 0);
		alc_process_coef_fw(codec, coef0292);
		break;
	case 0x10ec0293:
		/* Set to TRS mode */
		alc_write_coef_idx(codec, 0x45, 0xc429);
		snd_hda_set_pin_ctl_cache(codec, hp_pin, 0);
		alc_process_coef_fw(codec, coef0293);
		snd_hda_set_pin_ctl_cache(codec, mic_pin, PIN_VREF50);
		break;
	case 0x10ec0867:
		alc_update_coefex_idx(codec, 0x57, 0x5, 0, 1<<14);
		fallthrough;
	case 0x10ec0221:
	case 0x10ec0662:
		snd_hda_set_pin_ctl_cache(codec, hp_pin, 0);
		snd_hda_set_pin_ctl_cache(codec, mic_pin, PIN_VREF50);
		break;
	case 0x10ec0668:
		alc_write_coef_idx(codec, 0x11, 0x0001);
		snd_hda_set_pin_ctl_cache(codec, hp_pin, 0);
		alc_process_coef_fw(codec, coef0688);
		snd_hda_set_pin_ctl_cache(codec, mic_pin, PIN_VREF50);
		break;
	case 0x10ec0215:
	case 0x10ec0225:
	case 0x10ec0285:
	case 0x10ec0295:
	case 0x10ec0289:
	case 0x10ec0299:
		alc_process_coef_fw(codec, alc225_pre_hsmode);
		alc_update_coef_idx(codec, 0x45, 0x3f<<10, 0x31<<10);
		snd_hda_set_pin_ctl_cache(codec, hp_pin, 0);
		alc_process_coef_fw(codec, coef0225);
		snd_hda_set_pin_ctl_cache(codec, mic_pin, PIN_VREF50);
		break;
	}
	codec_dbg(codec, "Headset jack set to mic-in mode.\n");
}

static void alc_headset_mode_default(struct hda_codec *codec)
{
	static const struct coef_fw coef0225[] = {
		UPDATE_COEF(0x45, 0x3f<<10, 0x30<<10),
		UPDATE_COEF(0x45, 0x3f<<10, 0x31<<10),
		UPDATE_COEF(0x49, 3<<8, 0<<8),
		UPDATE_COEF(0x4a, 3<<4, 3<<4),
		UPDATE_COEF(0x63, 3<<14, 0),
		UPDATE_COEF(0x67, 0xf000, 0x3000),
		{}
	};
	static const struct coef_fw coef0255[] = {
		WRITE_COEF(0x45, 0xc089),
		WRITE_COEF(0x45, 0xc489),
		WRITE_COEFEX(0x57, 0x03, 0x8ea6),
		WRITE_COEF(0x49, 0x0049),
		{}
	};
	static const struct coef_fw coef0256[] = {
		WRITE_COEF(0x45, 0xc489),
		WRITE_COEFEX(0x57, 0x03, 0x0da3),
		WRITE_COEF(0x49, 0x0049),
		UPDATE_COEFEX(0x57, 0x05, 1<<14, 0), /* Direct Drive HP Amp control(Set to verb control)*/
		WRITE_COEF(0x06, 0x6100),
		{}
	};
	static const struct coef_fw coef0233[] = {
		WRITE_COEF(0x06, 0x2100),
		WRITE_COEF(0x32, 0x4ea3),
		{}
	};
	static const struct coef_fw coef0288[] = {
		UPDATE_COEF(0x4f, 0xfcc0, 0xc400), /* Set to TRS type */
		UPDATE_COEF(0x50, 0x2000, 0x2000),
		UPDATE_COEF(0x56, 0x0006, 0x0006),
		UPDATE_COEF(0x66, 0x0008, 0),
		UPDATE_COEF(0x67, 0x2000, 0),
		{}
	};
	static const struct coef_fw coef0292[] = {
		WRITE_COEF(0x76, 0x000e),
		WRITE_COEF(0x6c, 0x2400),
		WRITE_COEF(0x6b, 0xc429),
		WRITE_COEF(0x18, 0x7308),
		{}
	};
	static const struct coef_fw coef0293[] = {
		UPDATE_COEF(0x4a, 0x000f, 0x000e), /* Combo Jack auto detect */
		WRITE_COEF(0x45, 0xC429), /* Set to TRS type */
		UPDATE_COEF(0x1a, 1<<3, 0), /* Combo JD gating without LINE1-VREFO */
		{}
	};
	static const struct coef_fw coef0688[] = {
		WRITE_COEF(0x11, 0x0041),
		WRITE_COEF(0x15, 0x0d40),
		WRITE_COEF(0xb7, 0x802b),
		{}
	};
	static const struct coef_fw coef0274[] = {
		WRITE_COEF(0x45, 0x4289),
		UPDATE_COEF(0x4a, 0x0010, 0x0010),
		UPDATE_COEF(0x6b, 0x0f00, 0),
		UPDATE_COEF(0x49, 0x0300, 0x0300),
		{}
	};

	switch (codec->core.vendor_id) {
	case 0x10ec0215:
	case 0x10ec0225:
	case 0x10ec0285:
	case 0x10ec0295:
	case 0x10ec0289:
	case 0x10ec0299:
		alc_process_coef_fw(codec, alc225_pre_hsmode);
		alc_process_coef_fw(codec, coef0225);
		break;
	case 0x10ec0255:
		alc_process_coef_fw(codec, coef0255);
		break;
	case 0x10ec0230:
	case 0x10ec0236:
	case 0x10ec0256:
	case 0x19e58326:
		alc_write_coef_idx(codec, 0x1b, 0x0e4b);
		alc_write_coef_idx(codec, 0x45, 0xc089);
		msleep(50);
		alc_process_coef_fw(codec, coef0256);
		break;
	case 0x10ec0234:
	case 0x10ec0274:
	case 0x10ec0294:
		alc_process_coef_fw(codec, coef0274);
		break;
	case 0x10ec0233:
	case 0x10ec0283:
		alc_process_coef_fw(codec, coef0233);
		break;
	case 0x10ec0286:
	case 0x10ec0288:
	case 0x10ec0298:
		alc_process_coef_fw(codec, coef0288);
		break;
	case 0x10ec0292:
		alc_process_coef_fw(codec, coef0292);
		break;
	case 0x10ec0293:
		alc_process_coef_fw(codec, coef0293);
		break;
	case 0x10ec0668:
		alc_process_coef_fw(codec, coef0688);
		break;
	case 0x10ec0867:
		alc_update_coefex_idx(codec, 0x57, 0x5, 1<<14, 0);
		break;
	}
	codec_dbg(codec, "Headset jack set to headphone (default) mode.\n");
}

/* Iphone type */
static void alc_headset_mode_ctia(struct hda_codec *codec)
{
	int val;

	static const struct coef_fw coef0255[] = {
		WRITE_COEF(0x45, 0xd489), /* Set to CTIA type */
		WRITE_COEF(0x1b, 0x0c2b),
		WRITE_COEFEX(0x57, 0x03, 0x8ea6),
		{}
	};
	static const struct coef_fw coef0256[] = {
		WRITE_COEF(0x45, 0xd489), /* Set to CTIA type */
		WRITE_COEF(0x1b, 0x0e6b),
		{}
	};
	static const struct coef_fw coef0233[] = {
		WRITE_COEF(0x45, 0xd429),
		WRITE_COEF(0x1b, 0x0c2b),
		WRITE_COEF(0x32, 0x4ea3),
		{}
	};
	static const struct coef_fw coef0288[] = {
		UPDATE_COEF(0x50, 0x2000, 0x2000),
		UPDATE_COEF(0x56, 0x0006, 0x0006),
		UPDATE_COEF(0x66, 0x0008, 0),
		UPDATE_COEF(0x67, 0x2000, 0),
		{}
	};
	static const struct coef_fw coef0292[] = {
		WRITE_COEF(0x6b, 0xd429),
		WRITE_COEF(0x76, 0x0008),
		WRITE_COEF(0x18, 0x7388),
		{}
	};
	static const struct coef_fw coef0293[] = {
		WRITE_COEF(0x45, 0xd429), /* Set to ctia type */
		UPDATE_COEF(0x10, 7<<8, 7<<8), /* SET Line1 JD to 1 */
		{}
	};
	static const struct coef_fw coef0688[] = {
		WRITE_COEF(0x11, 0x0001),
		WRITE_COEF(0x15, 0x0d60),
		WRITE_COEF(0xc3, 0x0000),
		{}
	};
	static const struct coef_fw coef0225_1[] = {
		UPDATE_COEF(0x45, 0x3f<<10, 0x35<<10),
		UPDATE_COEF(0x63, 3<<14, 2<<14),
		{}
	};
	static const struct coef_fw coef0225_2[] = {
		UPDATE_COEF(0x45, 0x3f<<10, 0x35<<10),
		UPDATE_COEF(0x63, 3<<14, 1<<14),
		{}
	};

	switch (codec->core.vendor_id) {
	case 0x10ec0255:
		alc_process_coef_fw(codec, coef0255);
		break;
	case 0x10ec0230:
	case 0x10ec0236:
	case 0x10ec0256:
	case 0x19e58326:
		alc_process_coef_fw(codec, coef0256);
		break;
	case 0x10ec0234:
	case 0x10ec0274:
	case 0x10ec0294:
		alc_write_coef_idx(codec, 0x45, 0xd689);
		break;
	case 0x10ec0233:
	case 0x10ec0283:
		alc_process_coef_fw(codec, coef0233);
		break;
	case 0x10ec0298:
		val = alc_read_coef_idx(codec, 0x50);
		if (val & (1 << 12)) {
			alc_update_coef_idx(codec, 0x8e, 0x0070, 0x0020);
			alc_update_coef_idx(codec, 0x4f, 0xfcc0, 0xd400);
			msleep(300);
		} else {
			alc_update_coef_idx(codec, 0x8e, 0x0070, 0x0010);
			alc_update_coef_idx(codec, 0x4f, 0xfcc0, 0xd400);
			msleep(300);
		}
		break;
	case 0x10ec0286:
	case 0x10ec0288:
		alc_update_coef_idx(codec, 0x4f, 0xfcc0, 0xd400);
		msleep(300);
		alc_process_coef_fw(codec, coef0288);
		break;
	case 0x10ec0292:
		alc_process_coef_fw(codec, coef0292);
		break;
	case 0x10ec0293:
		alc_process_coef_fw(codec, coef0293);
		break;
	case 0x10ec0668:
		alc_process_coef_fw(codec, coef0688);
		break;
	case 0x10ec0215:
	case 0x10ec0225:
	case 0x10ec0285:
	case 0x10ec0295:
	case 0x10ec0289:
	case 0x10ec0299:
		val = alc_read_coef_idx(codec, 0x45);
		if (val & (1 << 9))
			alc_process_coef_fw(codec, coef0225_2);
		else
			alc_process_coef_fw(codec, coef0225_1);
		break;
	case 0x10ec0867:
		alc_update_coefex_idx(codec, 0x57, 0x5, 1<<14, 0);
		break;
	}
	codec_dbg(codec, "Headset jack set to iPhone-style headset mode.\n");
}

/* Nokia type */
static void alc_headset_mode_omtp(struct hda_codec *codec)
{
	static const struct coef_fw coef0255[] = {
		WRITE_COEF(0x45, 0xe489), /* Set to OMTP Type */
		WRITE_COEF(0x1b, 0x0c2b),
		WRITE_COEFEX(0x57, 0x03, 0x8ea6),
		{}
	};
	static const struct coef_fw coef0256[] = {
		WRITE_COEF(0x45, 0xe489), /* Set to OMTP Type */
		WRITE_COEF(0x1b, 0x0e6b),
		{}
	};
	static const struct coef_fw coef0233[] = {
		WRITE_COEF(0x45, 0xe429),
		WRITE_COEF(0x1b, 0x0c2b),
		WRITE_COEF(0x32, 0x4ea3),
		{}
	};
	static const struct coef_fw coef0288[] = {
		UPDATE_COEF(0x50, 0x2000, 0x2000),
		UPDATE_COEF(0x56, 0x0006, 0x0006),
		UPDATE_COEF(0x66, 0x0008, 0),
		UPDATE_COEF(0x67, 0x2000, 0),
		{}
	};
	static const struct coef_fw coef0292[] = {
		WRITE_COEF(0x6b, 0xe429),
		WRITE_COEF(0x76, 0x0008),
		WRITE_COEF(0x18, 0x7388),
		{}
	};
	static const struct coef_fw coef0293[] = {
		WRITE_COEF(0x45, 0xe429), /* Set to omtp type */
		UPDATE_COEF(0x10, 7<<8, 7<<8), /* SET Line1 JD to 1 */
		{}
	};
	static const struct coef_fw coef0688[] = {
		WRITE_COEF(0x11, 0x0001),
		WRITE_COEF(0x15, 0x0d50),
		WRITE_COEF(0xc3, 0x0000),
		{}
	};
	static const struct coef_fw coef0225[] = {
		UPDATE_COEF(0x45, 0x3f<<10, 0x39<<10),
		UPDATE_COEF(0x63, 3<<14, 2<<14),
		{}
	};

	switch (codec->core.vendor_id) {
	case 0x10ec0255:
		alc_process_coef_fw(codec, coef0255);
		break;
	case 0x10ec0230:
	case 0x10ec0236:
	case 0x10ec0256:
	case 0x19e58326:
		alc_process_coef_fw(codec, coef0256);
		break;
	case 0x10ec0234:
	case 0x10ec0274:
	case 0x10ec0294:
		alc_write_coef_idx(codec, 0x45, 0xe689);
		break;
	case 0x10ec0233:
	case 0x10ec0283:
		alc_process_coef_fw(codec, coef0233);
		break;
	case 0x10ec0298:
		alc_update_coef_idx(codec, 0x8e, 0x0070, 0x0010);/* Headset output enable */
		alc_update_coef_idx(codec, 0x4f, 0xfcc0, 0xe400);
		msleep(300);
		break;
	case 0x10ec0286:
	case 0x10ec0288:
		alc_update_coef_idx(codec, 0x4f, 0xfcc0, 0xe400);
		msleep(300);
		alc_process_coef_fw(codec, coef0288);
		break;
	case 0x10ec0292:
		alc_process_coef_fw(codec, coef0292);
		break;
	case 0x10ec0293:
		alc_process_coef_fw(codec, coef0293);
		break;
	case 0x10ec0668:
		alc_process_coef_fw(codec, coef0688);
		break;
	case 0x10ec0215:
	case 0x10ec0225:
	case 0x10ec0285:
	case 0x10ec0295:
	case 0x10ec0289:
	case 0x10ec0299:
		alc_process_coef_fw(codec, coef0225);
		break;
	}
	codec_dbg(codec, "Headset jack set to Nokia-style headset mode.\n");
}

static void alc_determine_headset_type(struct hda_codec *codec)
{
	int val;
	bool is_ctia = false;
	struct alc_spec *spec = codec->spec;
	static const struct coef_fw coef0255[] = {
		WRITE_COEF(0x45, 0xd089), /* combo jack auto switch control(Check type)*/
		WRITE_COEF(0x49, 0x0149), /* combo jack auto switch control(Vref
 conteol) */
		{}
	};
	static const struct coef_fw coef0288[] = {
		UPDATE_COEF(0x4f, 0xfcc0, 0xd400), /* Check Type */
		{}
	};
	static const struct coef_fw coef0298[] = {
		UPDATE_COEF(0x50, 0x2000, 0x2000),
		UPDATE_COEF(0x56, 0x0006, 0x0006),
		UPDATE_COEF(0x66, 0x0008, 0),
		UPDATE_COEF(0x67, 0x2000, 0),
		UPDATE_COEF(0x19, 0x1300, 0x1300),
		{}
	};
	static const struct coef_fw coef0293[] = {
		UPDATE_COEF(0x4a, 0x000f, 0x0008), /* Combo Jack auto detect */
		WRITE_COEF(0x45, 0xD429), /* Set to ctia type */
		{}
	};
	static const struct coef_fw coef0688[] = {
		WRITE_COEF(0x11, 0x0001),
		WRITE_COEF(0xb7, 0x802b),
		WRITE_COEF(0x15, 0x0d60),
		WRITE_COEF(0xc3, 0x0c00),
		{}
	};
	static const struct coef_fw coef0274[] = {
		UPDATE_COEF(0x4a, 0x0010, 0),
		UPDATE_COEF(0x4a, 0x8000, 0),
		WRITE_COEF(0x45, 0xd289),
		UPDATE_COEF(0x49, 0x0300, 0x0300),
		{}
	};

	if (spec->no_internal_mic_pin) {
		alc_update_coef_idx(codec, 0x45, 0xf<<12 | 1<<10, 5<<12);
		return;
	}

	switch (codec->core.vendor_id) {
	case 0x10ec0255:
		alc_process_coef_fw(codec, coef0255);
		msleep(300);
		val = alc_read_coef_idx(codec, 0x46);
		is_ctia = (val & 0x0070) == 0x0070;
		break;
	case 0x10ec0230:
	case 0x10ec0236:
	case 0x10ec0256:
	case 0x19e58326:
		alc_write_coef_idx(codec, 0x1b, 0x0e4b);
		alc_write_coef_idx(codec, 0x06, 0x6104);
		alc_write_coefex_idx(codec, 0x57, 0x3, 0x09a3);

		snd_hda_codec_write(codec, 0x21, 0,
			    AC_VERB_SET_AMP_GAIN_MUTE, AMP_OUT_MUTE);
		msleep(80);
		snd_hda_codec_write(codec, 0x21, 0,
			    AC_VERB_SET_PIN_WIDGET_CONTROL, 0x0);

		alc_process_coef_fw(codec, coef0255);
		msleep(300);
		val = alc_read_coef_idx(codec, 0x46);
		is_ctia = (val & 0x0070) == 0x0070;

		alc_write_coefex_idx(codec, 0x57, 0x3, 0x0da3);
		alc_update_coefex_idx(codec, 0x57, 0x5, 1<<14, 0);

		snd_hda_codec_write(codec, 0x21, 0,
			    AC_VERB_SET_PIN_WIDGET_CONTROL, PIN_OUT);
		msleep(80);
		snd_hda_codec_write(codec, 0x21, 0,
			    AC_VERB_SET_AMP_GAIN_MUTE, AMP_OUT_UNMUTE);
		break;
	case 0x10ec0234:
	case 0x10ec0274:
	case 0x10ec0294:
		alc_process_coef_fw(codec, coef0274);
		msleep(850);
		val = alc_read_coef_idx(codec, 0x46);
		is_ctia = (val & 0x00f0) == 0x00f0;
		break;
	case 0x10ec0233:
	case 0x10ec0283:
		alc_write_coef_idx(codec, 0x45, 0xd029);
		msleep(300);
		val = alc_read_coef_idx(codec, 0x46);
		is_ctia = (val & 0x0070) == 0x0070;
		break;
	case 0x10ec0298:
		snd_hda_codec_write(codec, 0x21, 0,
			    AC_VERB_SET_AMP_GAIN_MUTE, AMP_OUT_MUTE);
		msleep(100);
		snd_hda_codec_write(codec, 0x21, 0,
			    AC_VERB_SET_PIN_WIDGET_CONTROL, 0x0);
		msleep(200);

		val = alc_read_coef_idx(codec, 0x50);
		if (val & (1 << 12)) {
			alc_update_coef_idx(codec, 0x8e, 0x0070, 0x0020);
			alc_process_coef_fw(codec, coef0288);
			msleep(350);
			val = alc_read_coef_idx(codec, 0x50);
			is_ctia = (val & 0x0070) == 0x0070;
		} else {
			alc_update_coef_idx(codec, 0x8e, 0x0070, 0x0010);
			alc_process_coef_fw(codec, coef0288);
			msleep(350);
			val = alc_read_coef_idx(codec, 0x50);
			is_ctia = (val & 0x0070) == 0x0070;
		}
		alc_process_coef_fw(codec, coef0298);
		snd_hda_codec_write(codec, 0x21, 0,
			    AC_VERB_SET_PIN_WIDGET_CONTROL, PIN_HP);
		msleep(75);
		snd_hda_codec_write(codec, 0x21, 0,
			    AC_VERB_SET_AMP_GAIN_MUTE, AMP_OUT_UNMUTE);
		break;
	case 0x10ec0286:
	case 0x10ec0288:
		alc_process_coef_fw(codec, coef0288);
		msleep(350);
		val = alc_read_coef_idx(codec, 0x50);
		is_ctia = (val & 0x0070) == 0x0070;
		break;
	case 0x10ec0292:
		alc_write_coef_idx(codec, 0x6b, 0xd429);
		msleep(300);
		val = alc_read_coef_idx(codec, 0x6c);
		is_ctia = (val & 0x001c) == 0x001c;
		break;
	case 0x10ec0293:
		alc_process_coef_fw(codec, coef0293);
		msleep(300);
		val = alc_read_coef_idx(codec, 0x46);
		is_ctia = (val & 0x0070) == 0x0070;
		break;
	case 0x10ec0668:
		alc_process_coef_fw(codec, coef0688);
		msleep(300);
		val = alc_read_coef_idx(codec, 0xbe);
		is_ctia = (val & 0x1c02) == 0x1c02;
		break;
	case 0x10ec0215:
	case 0x10ec0225:
	case 0x10ec0285:
	case 0x10ec0295:
	case 0x10ec0289:
	case 0x10ec0299:
		snd_hda_codec_write(codec, 0x21, 0,
			    AC_VERB_SET_AMP_GAIN_MUTE, AMP_OUT_MUTE);
		msleep(80);
		snd_hda_codec_write(codec, 0x21, 0,
			    AC_VERB_SET_PIN_WIDGET_CONTROL, 0x0);

		alc_process_coef_fw(codec, alc225_pre_hsmode);
		alc_update_coef_idx(codec, 0x67, 0xf000, 0x1000);
		val = alc_read_coef_idx(codec, 0x45);
		if (val & (1 << 9)) {
			alc_update_coef_idx(codec, 0x45, 0x3f<<10, 0x34<<10);
			alc_update_coef_idx(codec, 0x49, 3<<8, 2<<8);
			msleep(800);
			val = alc_read_coef_idx(codec, 0x46);
			is_ctia = (val & 0x00f0) == 0x00f0;
		} else {
			alc_update_coef_idx(codec, 0x45, 0x3f<<10, 0x34<<10);
			alc_update_coef_idx(codec, 0x49, 3<<8, 1<<8);
			msleep(800);
			val = alc_read_coef_idx(codec, 0x46);
			is_ctia = (val & 0x00f0) == 0x00f0;
		}
		alc_update_coef_idx(codec, 0x4a, 7<<6, 7<<6);
		alc_update_coef_idx(codec, 0x4a, 3<<4, 3<<4);
		alc_update_coef_idx(codec, 0x67, 0xf000, 0x3000);

		snd_hda_codec_write(codec, 0x21, 0,
			    AC_VERB_SET_PIN_WIDGET_CONTROL, PIN_OUT);
		msleep(80);
		snd_hda_codec_write(codec, 0x21, 0,
			    AC_VERB_SET_AMP_GAIN_MUTE, AMP_OUT_UNMUTE);
		break;
	case 0x10ec0867:
		is_ctia = true;
		break;
	}

	codec_dbg(codec, "Headset jack detected iPhone-style headset: %s\n",
		    is_ctia ? "yes" : "no");
	spec->current_headset_type = is_ctia ? ALC_HEADSET_TYPE_CTIA : ALC_HEADSET_TYPE_OMTP;
}

static void alc_update_headset_mode(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;

	hda_nid_t mux_pin = spec->gen.imux_pins[spec->gen.cur_mux[0]];
	hda_nid_t hp_pin = alc_get_hp_pin(spec);

	int new_headset_mode;

	if (!snd_hda_jack_detect(codec, hp_pin))
		new_headset_mode = ALC_HEADSET_MODE_UNPLUGGED;
	else if (mux_pin == spec->headset_mic_pin)
		new_headset_mode = ALC_HEADSET_MODE_HEADSET;
	else if (mux_pin == spec->headphone_mic_pin)
		new_headset_mode = ALC_HEADSET_MODE_MIC;
	else
		new_headset_mode = ALC_HEADSET_MODE_HEADPHONE;

	if (new_headset_mode == spec->current_headset_mode) {
		snd_hda_gen_update_outputs(codec);
		return;
	}

	switch (new_headset_mode) {
	case ALC_HEADSET_MODE_UNPLUGGED:
		alc_headset_mode_unplugged(codec);
		spec->current_headset_mode = ALC_HEADSET_MODE_UNKNOWN;
		spec->current_headset_type = ALC_HEADSET_TYPE_UNKNOWN;
		spec->gen.hp_jack_present = false;
		break;
	case ALC_HEADSET_MODE_HEADSET:
		if (spec->current_headset_type == ALC_HEADSET_TYPE_UNKNOWN)
			alc_determine_headset_type(codec);
		if (spec->current_headset_type == ALC_HEADSET_TYPE_CTIA)
			alc_headset_mode_ctia(codec);
		else if (spec->current_headset_type == ALC_HEADSET_TYPE_OMTP)
			alc_headset_mode_omtp(codec);
		spec->gen.hp_jack_present = true;
		break;
	case ALC_HEADSET_MODE_MIC:
		alc_headset_mode_mic_in(codec, hp_pin, spec->headphone_mic_pin);
		spec->gen.hp_jack_present = false;
		break;
	case ALC_HEADSET_MODE_HEADPHONE:
		alc_headset_mode_default(codec);
		spec->gen.hp_jack_present = true;
		break;
	}
	if (new_headset_mode != ALC_HEADSET_MODE_MIC) {
		snd_hda_set_pin_ctl_cache(codec, hp_pin,
					  AC_PINCTL_OUT_EN | AC_PINCTL_HP_EN);
		if (spec->headphone_mic_pin && spec->headphone_mic_pin != hp_pin)
			snd_hda_set_pin_ctl_cache(codec, spec->headphone_mic_pin,
						  PIN_VREFHIZ);
	}
	spec->current_headset_mode = new_headset_mode;

	snd_hda_gen_update_outputs(codec);
}

static void alc_update_headset_mode_hook(struct hda_codec *codec,
					 struct snd_kcontrol *kcontrol,
					 struct snd_ctl_elem_value *ucontrol)
{
	alc_update_headset_mode(codec);
}

static void alc_update_headset_jack_cb(struct hda_codec *codec,
				       struct hda_jack_callback *jack)
{
	snd_hda_gen_hp_automute(codec, jack);
	alc_update_headset_mode(codec);
}

static void alc_probe_headset_mode(struct hda_codec *codec)
{
	int i;
	struct alc_spec *spec = codec->spec;
	struct auto_pin_cfg *cfg = &spec->gen.autocfg;

	/* Find mic pins */
	for (i = 0; i < cfg->num_inputs; i++) {
		if (cfg->inputs[i].is_headset_mic && !spec->headset_mic_pin)
			spec->headset_mic_pin = cfg->inputs[i].pin;
		if (cfg->inputs[i].is_headphone_mic && !spec->headphone_mic_pin)
			spec->headphone_mic_pin = cfg->inputs[i].pin;
	}

	WARN_ON(spec->gen.cap_sync_hook);
	spec->gen.cap_sync_hook = alc_update_headset_mode_hook;
	spec->gen.automute_hook = alc_update_headset_mode;
	spec->gen.hp_automute_hook = alc_update_headset_jack_cb;
}

static void alc_fixup_headset_mode(struct hda_codec *codec,
				const struct hda_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;

	switch (action) {
	case HDA_FIXUP_ACT_PRE_PROBE:
		spec->parse_flags |= HDA_PINCFG_HEADSET_MIC | HDA_PINCFG_HEADPHONE_MIC;
		break;
	case HDA_FIXUP_ACT_PROBE:
		alc_probe_headset_mode(codec);
		break;
	case HDA_FIXUP_ACT_INIT:
		if (is_s3_resume(codec) || is_s4_resume(codec)) {
			spec->current_headset_mode = ALC_HEADSET_MODE_UNKNOWN;
			spec->current_headset_type = ALC_HEADSET_TYPE_UNKNOWN;
		}
		alc_update_headset_mode(codec);
		break;
	}
}

static void alc_fixup_headset_mode_no_hp_mic(struct hda_codec *codec,
				const struct hda_fixup *fix, int action)
{
	if (action == HDA_FIXUP_ACT_PRE_PROBE) {
		struct alc_spec *spec = codec->spec;
		spec->parse_flags |= HDA_PINCFG_HEADSET_MIC;
	}
	else
		alc_fixup_headset_mode(codec, fix, action);
}

static void alc255_set_default_jack_type(struct hda_codec *codec)
{
	/* Set to iphone type */
	static const struct coef_fw alc255fw[] = {
		WRITE_COEF(0x1b, 0x880b),
		WRITE_COEF(0x45, 0xd089),
		WRITE_COEF(0x1b, 0x080b),
		WRITE_COEF(0x46, 0x0004),
		WRITE_COEF(0x1b, 0x0c0b),
		{}
	};
	static const struct coef_fw alc256fw[] = {
		WRITE_COEF(0x1b, 0x884b),
		WRITE_COEF(0x45, 0xd089),
		WRITE_COEF(0x1b, 0x084b),
		WRITE_COEF(0x46, 0x0004),
		WRITE_COEF(0x1b, 0x0c4b),
		{}
	};
	switch (codec->core.vendor_id) {
	case 0x10ec0255:
		alc_process_coef_fw(codec, alc255fw);
		break;
	case 0x10ec0230:
	case 0x10ec0236:
	case 0x10ec0256:
	case 0x19e58326:
		alc_process_coef_fw(codec, alc256fw);
		break;
	}
	msleep(30);
}

static void alc_fixup_headset_mode_alc255(struct hda_codec *codec,
				const struct hda_fixup *fix, int action)
{
	if (action == HDA_FIXUP_ACT_PRE_PROBE) {
		alc255_set_default_jack_type(codec);
	}
	alc_fixup_headset_mode(codec, fix, action);
}

static void alc_fixup_headset_mode_alc255_no_hp_mic(struct hda_codec *codec,
				const struct hda_fixup *fix, int action)
{
	if (action == HDA_FIXUP_ACT_PRE_PROBE) {
		struct alc_spec *spec = codec->spec;
		spec->parse_flags |= HDA_PINCFG_HEADSET_MIC;
		alc255_set_default_jack_type(codec);
	}
	else
		alc_fixup_headset_mode(codec, fix, action);
}

static void alc288_update_headset_jack_cb(struct hda_codec *codec,
				       struct hda_jack_callback *jack)
{
	struct alc_spec *spec = codec->spec;

	alc_update_headset_jack_cb(codec, jack);
	/* Headset Mic enable or disable, only for Dell Dino */
	alc_update_gpio_data(codec, 0x40, spec->gen.hp_jack_present);
}

static void alc_fixup_headset_mode_dell_alc288(struct hda_codec *codec,
				const struct hda_fixup *fix, int action)
{
	alc_fixup_headset_mode(codec, fix, action);
	if (action == HDA_FIXUP_ACT_PROBE) {
		struct alc_spec *spec = codec->spec;
		/* toggled via hp_automute_hook */
		spec->gpio_mask |= 0x40;
		spec->gpio_dir |= 0x40;
		spec->gen.hp_automute_hook = alc288_update_headset_jack_cb;
	}
}

static void alc_fixup_auto_mute_via_amp(struct hda_codec *codec,
					const struct hda_fixup *fix, int action)
{
	if (action == HDA_FIXUP_ACT_PRE_PROBE) {
		struct alc_spec *spec = codec->spec;
		spec->gen.auto_mute_via_amp = 1;
	}
}

static void alc_fixup_no_shutup(struct hda_codec *codec,
				const struct hda_fixup *fix, int action)
{
	if (action == HDA_FIXUP_ACT_PRE_PROBE) {
		struct alc_spec *spec = codec->spec;
		spec->no_shutup_pins = 1;
	}
}

static void alc_fixup_disable_aamix(struct hda_codec *codec,
				    const struct hda_fixup *fix, int action)
{
	if (action == HDA_FIXUP_ACT_PRE_PROBE) {
		struct alc_spec *spec = codec->spec;
		/* Disable AA-loopback as it causes white noise */
		spec->gen.mixer_nid = 0;
	}
}

/* fixup for Thinkpad docks: add dock pins, avoid HP parser fixup */
static void alc_fixup_tpt440_dock(struct hda_codec *codec,
				  const struct hda_fixup *fix, int action)
{
	static const struct hda_pintbl pincfgs[] = {
		{ 0x16, 0x21211010 }, /* dock headphone */
		{ 0x19, 0x21a11010 }, /* dock mic */
		{ }
	};
	struct alc_spec *spec = codec->spec;

	if (action == HDA_FIXUP_ACT_PRE_PROBE) {
		spec->parse_flags = HDA_PINCFG_NO_HP_FIXUP;
		codec->power_save_node = 0; /* avoid click noises */
		snd_hda_apply_pincfgs(codec, pincfgs);
	}
}

static void alc_fixup_tpt470_dock(struct hda_codec *codec,
				  const struct hda_fixup *fix, int action)
{
	static const struct hda_pintbl pincfgs[] = {
		{ 0x17, 0x21211010 }, /* dock headphone */
		{ 0x19, 0x21a11010 }, /* dock mic */
		{ }
	};
	struct alc_spec *spec = codec->spec;

	if (action == HDA_FIXUP_ACT_PRE_PROBE) {
		spec->parse_flags = HDA_PINCFG_NO_HP_FIXUP;
		snd_hda_apply_pincfgs(codec, pincfgs);
	} else if (action == HDA_FIXUP_ACT_INIT) {
		/* Enable DOCK device */
		snd_hda_codec_write(codec, 0x17, 0,
			    AC_VERB_SET_CONFIG_DEFAULT_BYTES_3, 0);
		/* Enable DOCK device */
		snd_hda_codec_write(codec, 0x19, 0,
			    AC_VERB_SET_CONFIG_DEFAULT_BYTES_3, 0);
	}
}

static void alc_fixup_tpt470_dacs(struct hda_codec *codec,
				  const struct hda_fixup *fix, int action)
{
	/* Assure the speaker pin to be coupled with DAC NID 0x03; otherwise
	 * the speaker output becomes too low by some reason on Thinkpads with
	 * ALC298 codec
	 */
	static const hda_nid_t preferred_pairs[] = {
		0x14, 0x03, 0x17, 0x02, 0x21, 0x02,
		0
	};
	struct alc_spec *spec = codec->spec;

	if (action == HDA_FIXUP_ACT_PRE_PROBE)
		spec->gen.preferred_dacs = preferred_pairs;
}

static void alc295_fixup_asus_dacs(struct hda_codec *codec,
				   const struct hda_fixup *fix, int action)
{
	static const hda_nid_t preferred_pairs[] = {
		0x17, 0x02, 0x21, 0x03, 0
	};
	struct alc_spec *spec = codec->spec;

	if (action == HDA_FIXUP_ACT_PRE_PROBE)
		spec->gen.preferred_dacs = preferred_pairs;
}

static void alc_shutup_dell_xps13(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	int hp_pin = alc_get_hp_pin(spec);

	/* Prevent pop noises when headphones are plugged in */
	snd_hda_codec_write(codec, hp_pin, 0,
			    AC_VERB_SET_AMP_GAIN_MUTE, AMP_OUT_MUTE);
	msleep(20);
}

static void alc_fixup_dell_xps13(struct hda_codec *codec,
				const struct hda_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;
	struct hda_input_mux *imux = &spec->gen.input_mux;
	int i;

	switch (action) {
	case HDA_FIXUP_ACT_PRE_PROBE:
		/* mic pin 0x19 must be initialized with Vref Hi-Z, otherwise
		 * it causes a click noise at start up
		 */
		snd_hda_codec_set_pin_target(codec, 0x19, PIN_VREFHIZ);
		spec->shutup = alc_shutup_dell_xps13;
		break;
	case HDA_FIXUP_ACT_PROBE:
		/* Make the internal mic the default input source. */
		for (i = 0; i < imux->num_items; i++) {
			if (spec->gen.imux_pins[i] == 0x12) {
				spec->gen.cur_mux[0] = i;
				break;
			}
		}
		break;
	}
}

static void alc_fixup_headset_mode_alc662(struct hda_codec *codec,
				const struct hda_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;

	if (action == HDA_FIXUP_ACT_PRE_PROBE) {
		spec->parse_flags |= HDA_PINCFG_HEADSET_MIC;
		spec->gen.hp_mic = 1; /* Mic-in is same pin as headphone */

		/* Disable boost for mic-in permanently. (This code is only called
		   from quirks that guarantee that the headphone is at NID 0x1b.) */
		snd_hda_codec_write(codec, 0x1b, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x7000);
		snd_hda_override_wcaps(codec, 0x1b, get_wcaps(codec, 0x1b) & ~AC_WCAP_IN_AMP);
	} else
		alc_fixup_headset_mode(codec, fix, action);
}

static void alc_fixup_headset_mode_alc668(struct hda_codec *codec,
				const struct hda_fixup *fix, int action)
{
	if (action == HDA_FIXUP_ACT_PRE_PROBE) {
		alc_write_coef_idx(codec, 0xc4, 0x8000);
		alc_update_coef_idx(codec, 0xc2, ~0xfe, 0);
		snd_hda_set_pin_ctl_cache(codec, 0x18, 0);
	}
	alc_fixup_headset_mode(codec, fix, action);
}

/* Returns the nid of the external mic input pin, or 0 if it cannot be found. */
static int find_ext_mic_pin(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	struct auto_pin_cfg *cfg = &spec->gen.autocfg;
	hda_nid_t nid;
	unsigned int defcfg;
	int i;

	for (i = 0; i < cfg->num_inputs; i++) {
		if (cfg->inputs[i].type != AUTO_PIN_MIC)
			continue;
		nid = cfg->inputs[i].pin;
		defcfg = snd_hda_codec_get_pincfg(codec, nid);
		if (snd_hda_get_input_pin_attr(defcfg) == INPUT_PIN_ATTR_INT)
			continue;
		return nid;
	}

	return 0;
}

static void alc271_hp_gate_mic_jack(struct hda_codec *codec,
				    const struct hda_fixup *fix,
				    int action)
{
	struct alc_spec *spec = codec->spec;

	if (action == HDA_FIXUP_ACT_PROBE) {
		int mic_pin = find_ext_mic_pin(codec);
		int hp_pin = alc_get_hp_pin(spec);

		if (snd_BUG_ON(!mic_pin || !hp_pin))
			return;
		snd_hda_jack_set_gating_jack(codec, mic_pin, hp_pin);
	}
}

static void alc269_fixup_limit_int_mic_boost(struct hda_codec *codec,
					     const struct hda_fixup *fix,
					     int action)
{
	struct alc_spec *spec = codec->spec;
	struct auto_pin_cfg *cfg = &spec->gen.autocfg;
	int i;

	/* The mic boosts on level 2 and 3 are too noisy
	   on the internal mic input.
	   Therefore limit the boost to 0 or 1. */

	if (action != HDA_FIXUP_ACT_PROBE)
		return;

	for (i = 0; i < cfg->num_inputs; i++) {
		hda_nid_t nid = cfg->inputs[i].pin;
		unsigned int defcfg;
		if (cfg->inputs[i].type != AUTO_PIN_MIC)
			continue;
		defcfg = snd_hda_codec_get_pincfg(codec, nid);
		if (snd_hda_get_input_pin_attr(defcfg) != INPUT_PIN_ATTR_INT)
			continue;

		snd_hda_override_amp_caps(codec, nid, HDA_INPUT,
					  (0x00 << AC_AMPCAP_OFFSET_SHIFT) |
					  (0x01 << AC_AMPCAP_NUM_STEPS_SHIFT) |
					  (0x2f << AC_AMPCAP_STEP_SIZE_SHIFT) |
					  (0 << AC_AMPCAP_MUTE_SHIFT));
	}
}

static void alc283_hp_automute_hook(struct hda_codec *codec,
				    struct hda_jack_callback *jack)
{
	struct alc_spec *spec = codec->spec;
	int vref;

	msleep(200);
	snd_hda_gen_hp_automute(codec, jack);

	vref = spec->gen.hp_jack_present ? PIN_VREF80 : 0;

	msleep(600);
	snd_hda_codec_write(codec, 0x19, 0, AC_VERB_SET_PIN_WIDGET_CONTROL,
			    vref);
}

static void alc283_fixup_chromebook(struct hda_codec *codec,
				    const struct hda_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;

	switch (action) {
	case HDA_FIXUP_ACT_PRE_PROBE:
		snd_hda_override_wcaps(codec, 0x03, 0);
		/* Disable AA-loopback as it causes white noise */
		spec->gen.mixer_nid = 0;
		break;
	case HDA_FIXUP_ACT_INIT:
		/* MIC2-VREF control */
		/* Set to manual mode */
		alc_update_coef_idx(codec, 0x06, 0x000c, 0);
		/* Enable Line1 input control by verb */
		alc_update_coef_idx(codec, 0x1a, 0, 1 << 4);
		break;
	}
}

static void alc283_fixup_sense_combo_jack(struct hda_codec *codec,
				    const struct hda_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;

	switch (action) {
	case HDA_FIXUP_ACT_PRE_PROBE:
		spec->gen.hp_automute_hook = alc283_hp_automute_hook;
		break;
	case HDA_FIXUP_ACT_INIT:
		/* MIC2-VREF control */
		/* Set to manual mode */
		alc_update_coef_idx(codec, 0x06, 0x000c, 0);
		break;
	}
}

/* mute tablet speaker pin (0x14) via dock plugging in addition */
static void asus_tx300_automute(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	snd_hda_gen_update_outputs(codec);
	if (snd_hda_jack_detect(codec, 0x1b))
		spec->gen.mute_bits |= (1ULL << 0x14);
}

static void alc282_fixup_asus_tx300(struct hda_codec *codec,
				    const struct hda_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;
	static const struct hda_pintbl dock_pins[] = {
		{ 0x1b, 0x21114000 }, /* dock speaker pin */
		{}
	};

	switch (action) {
	case HDA_FIXUP_ACT_PRE_PROBE:
		spec->init_amp = ALC_INIT_DEFAULT;
		/* TX300 needs to set up GPIO2 for the speaker amp */
		alc_setup_gpio(codec, 0x04);
		snd_hda_apply_pincfgs(codec, dock_pins);
		spec->gen.auto_mute_via_amp = 1;
		spec->gen.automute_hook = asus_tx300_automute;
		snd_hda_jack_detect_enable_callback(codec, 0x1b,
						    snd_hda_gen_hp_automute);
		break;
	case HDA_FIXUP_ACT_PROBE:
		spec->init_amp = ALC_INIT_DEFAULT;
		break;
	case HDA_FIXUP_ACT_BUILD:
		/* this is a bit tricky; give more sane names for the main
		 * (tablet) speaker and the dock speaker, respectively
		 */
		rename_ctl(codec, "Speaker Playback Switch",
			   "Dock Speaker Playback Switch");
		rename_ctl(codec, "Bass Speaker Playback Switch",
			   "Speaker Playback Switch");
		break;
	}
}

static void alc290_fixup_mono_speakers(struct hda_codec *codec,
				       const struct hda_fixup *fix, int action)
{
	if (action == HDA_FIXUP_ACT_PRE_PROBE) {
		/* DAC node 0x03 is giving mono output. We therefore want to
		   make sure 0x14 (front speaker) and 0x15 (headphones) use the
		   stereo DAC, while leaving 0x17 (bass speaker) for node 0x03. */
		static const hda_nid_t conn1[] = { 0x0c };
		snd_hda_override_conn_list(codec, 0x14, ARRAY_SIZE(conn1), conn1);
		snd_hda_override_conn_list(codec, 0x15, ARRAY_SIZE(conn1), conn1);
	}
}

static void alc298_fixup_speaker_volume(struct hda_codec *codec,
					const struct hda_fixup *fix, int action)
{
	if (action == HDA_FIXUP_ACT_PRE_PROBE) {
		/* The speaker is routed to the Node 0x06 by a mistake, as a result
		   we can't adjust the speaker's volume since this node does not has
		   Amp-out capability. we change the speaker's route to:
		   Node 0x02 (Audio Output) -> Node 0x0c (Audio Mixer) -> Node 0x17 (
		   Pin Complex), since Node 0x02 has Amp-out caps, we can adjust
		   speaker's volume now. */

		static const hda_nid_t conn1[] = { 0x0c };
		snd_hda_override_conn_list(codec, 0x17, ARRAY_SIZE(conn1), conn1);
	}
}

/* disable DAC3 (0x06) selection on NID 0x17 as it has no volume amp control */
static void alc295_fixup_disable_dac3(struct hda_codec *codec,
				      const struct hda_fixup *fix, int action)
{
	if (action == HDA_FIXUP_ACT_PRE_PROBE) {
		static const hda_nid_t conn[] = { 0x02, 0x03 };
		snd_hda_override_conn_list(codec, 0x17, ARRAY_SIZE(conn), conn);
	}
}

/* force NID 0x17 (Bass Speaker) to DAC1 to share it with the main speaker */
static void alc285_fixup_speaker2_to_dac1(struct hda_codec *codec,
					  const struct hda_fixup *fix, int action)
{
	if (action == HDA_FIXUP_ACT_PRE_PROBE) {
		static const hda_nid_t conn[] = { 0x02 };
		snd_hda_override_conn_list(codec, 0x17, ARRAY_SIZE(conn), conn);
	}
}

/* Hook to update amp GPIO4 for automute */
static void alc280_hp_gpio4_automute_hook(struct hda_codec *codec,
					  struct hda_jack_callback *jack)
{
	struct alc_spec *spec = codec->spec;

	snd_hda_gen_hp_automute(codec, jack);
	/* mute_led_polarity is set to 0, so we pass inverted value here */
	alc_update_gpio_led(codec, 0x10, spec->mute_led_polarity,
			    !spec->gen.hp_jack_present);
}

/* Manage GPIOs for HP EliteBook Folio 9480m.
 *
 * GPIO4 is the headphone amplifier power control
 * GPIO3 is the audio output mute indicator LED
 */

static void alc280_fixup_hp_9480m(struct hda_codec *codec,
				  const struct hda_fixup *fix,
				  int action)
{
	struct alc_spec *spec = codec->spec;

	alc_fixup_hp_gpio_led(codec, action, 0x08, 0);
	if (action == HDA_FIXUP_ACT_PRE_PROBE) {
		/* amp at GPIO4; toggled via alc280_hp_gpio4_automute_hook() */
		spec->gpio_mask |= 0x10;
		spec->gpio_dir |= 0x10;
		spec->gen.hp_automute_hook = alc280_hp_gpio4_automute_hook;
	}
}

static void alc275_fixup_gpio4_off(struct hda_codec *codec,
				   const struct hda_fixup *fix,
				   int action)
{
	struct alc_spec *spec = codec->spec;

	if (action == HDA_FIXUP_ACT_PRE_PROBE) {
		spec->gpio_mask |= 0x04;
		spec->gpio_dir |= 0x04;
		/* set data bit low */
	}
}

/* Quirk for Thinkpad X1 7th and 8th Gen
 * The following fixed routing needed
 * DAC1 (NID 0x02) -> Speaker (NID 0x14); some eq applied secretly
 * DAC2 (NID 0x03) -> Bass (NID 0x17) & Headphone (NID 0x21); sharing a DAC
 * DAC3 (NID 0x06) -> Unused, due to the lack of volume amp
 */
static void alc285_fixup_thinkpad_x1_gen7(struct hda_codec *codec,
					  const struct hda_fixup *fix, int action)
{
	static const hda_nid_t conn[] = { 0x02, 0x03 }; /* exclude 0x06 */
	static const hda_nid_t preferred_pairs[] = {
		0x14, 0x02, 0x17, 0x03, 0x21, 0x03, 0
	};
	struct alc_spec *spec = codec->spec;

	switch (action) {
	case HDA_FIXUP_ACT_PRE_PROBE:
		snd_hda_override_conn_list(codec, 0x17, ARRAY_SIZE(conn), conn);
		spec->gen.preferred_dacs = preferred_pairs;
		break;
	case HDA_FIXUP_ACT_BUILD:
		/* The generic parser creates somewhat unintuitive volume ctls
		 * with the fixed routing above, and the shared DAC2 may be
		 * confusing for PA.
		 * Rename those to unique names so that PA doesn't touch them
		 * and use only Master volume.
		 */
		rename_ctl(codec, "Front Playback Volume", "DAC1 Playback Volume");
		rename_ctl(codec, "Bass Speaker Playback Volume", "DAC2 Playback Volume");
		break;
	}
}

static void alc233_alc662_fixup_lenovo_dual_codecs(struct hda_codec *codec,
					 const struct hda_fixup *fix,
					 int action)
{
	alc_fixup_dual_codecs(codec, fix, action);
	switch (action) {
	case HDA_FIXUP_ACT_PRE_PROBE:
		/* override card longname to provide a unique UCM profile */
		strcpy(codec->card->longname, "HDAudio-Lenovo-DualCodecs");
		break;
	case HDA_FIXUP_ACT_BUILD:
		/* rename Capture controls depending on the codec */
		rename_ctl(codec, "Capture Volume",
			   codec->addr == 0 ?
			   "Rear-Panel Capture Volume" :
			   "Front-Panel Capture Volume");
		rename_ctl(codec, "Capture Switch",
			   codec->addr == 0 ?
			   "Rear-Panel Capture Switch" :
			   "Front-Panel Capture Switch");
		break;
	}
}

static void alc225_fixup_s3_pop_noise(struct hda_codec *codec,
				      const struct hda_fixup *fix, int action)
{
	if (action != HDA_FIXUP_ACT_PRE_PROBE)
		return;

	codec->power_save_node = 1;
}

/* Forcibly assign NID 0x03 to HP/LO while NID 0x02 to SPK for EQ */
static void alc274_fixup_bind_dacs(struct hda_codec *codec,
				    const struct hda_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;
	static const hda_nid_t preferred_pairs[] = {
		0x21, 0x03, 0x1b, 0x03, 0x16, 0x02,
		0
	};

	if (action != HDA_FIXUP_ACT_PRE_PROBE)
		return;

	spec->gen.preferred_dacs = preferred_pairs;
	spec->gen.auto_mute_via_amp = 1;
	codec->power_save_node = 0;
}

/* avoid DAC 0x06 for bass speaker 0x17; it has no volume control */
static void alc289_fixup_asus_ga401(struct hda_codec *codec,
				    const struct hda_fixup *fix, int action)
{
	static const hda_nid_t preferred_pairs[] = {
		0x14, 0x02, 0x17, 0x02, 0x21, 0x03, 0
	};
	struct alc_spec *spec = codec->spec;

	if (action == HDA_FIXUP_ACT_PRE_PROBE) {
		spec->gen.preferred_dacs = preferred_pairs;
		spec->gen.obey_preferred_dacs = 1;
	}
}

/* The DAC of NID 0x3 will introduce click/pop noise on headphones, so invalidate it */
static void alc285_fixup_invalidate_dacs(struct hda_codec *codec,
			      const struct hda_fixup *fix, int action)
{
	if (action != HDA_FIXUP_ACT_PRE_PROBE)
		return;

	snd_hda_override_wcaps(codec, 0x03, 0);
}

static void alc_combo_jack_hp_jd_restart(struct hda_codec *codec)
{
	switch (codec->core.vendor_id) {
	case 0x10ec0274:
	case 0x10ec0294:
	case 0x10ec0225:
	case 0x10ec0295:
	case 0x10ec0299:
		alc_update_coef_idx(codec, 0x4a, 0x8000, 1 << 15); /* Reset HP JD */
		alc_update_coef_idx(codec, 0x4a, 0x8000, 0 << 15);
		break;
	case 0x10ec0230:
	case 0x10ec0235:
	case 0x10ec0236:
	case 0x10ec0255:
	case 0x10ec0256:
	case 0x10ec0257:
	case 0x19e58326:
		alc_update_coef_idx(codec, 0x1b, 0x8000, 1 << 15); /* Reset HP JD */
		alc_update_coef_idx(codec, 0x1b, 0x8000, 0 << 15);
		break;
	}
}

static void alc295_fixup_chromebook(struct hda_codec *codec,
				    const struct hda_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;

	switch (action) {
	case HDA_FIXUP_ACT_PRE_PROBE:
		spec->ultra_low_power = true;
		break;
	case HDA_FIXUP_ACT_INIT:
		alc_combo_jack_hp_jd_restart(codec);
		break;
	}
}

static void alc_fixup_disable_mic_vref(struct hda_codec *codec,
				  const struct hda_fixup *fix, int action)
{
	if (action == HDA_FIXUP_ACT_PRE_PROBE)
		snd_hda_codec_set_pin_target(codec, 0x19, PIN_VREFHIZ);
}


static void alc294_gx502_toggle_output(struct hda_codec *codec,
					struct hda_jack_callback *cb)
{
	/* The Windows driver sets the codec up in a very different way where
	 * it appears to leave 0x10 = 0x8a20 set. For Linux we need to toggle it
	 */
	if (snd_hda_jack_detect_state(codec, 0x21) == HDA_JACK_PRESENT)
		alc_write_coef_idx(codec, 0x10, 0x8a20);
	else
		alc_write_coef_idx(codec, 0x10, 0x0a20);
}

static void alc294_fixup_gx502_hp(struct hda_codec *codec,
					const struct hda_fixup *fix, int action)
{
	/* Pin 0x21: headphones/headset mic */
	if (!is_jack_detectable(codec, 0x21))
		return;

	switch (action) {
	case HDA_FIXUP_ACT_PRE_PROBE:
		snd_hda_jack_detect_enable_callback(codec, 0x21,
				alc294_gx502_toggle_output);
		break;
	case HDA_FIXUP_ACT_INIT:
		/* Make sure to start in a correct state, i.e. if
		 * headphones have been plugged in before powering up the system
		 */
		alc294_gx502_toggle_output(codec, NULL);
		break;
	}
}

static void alc294_gu502_toggle_output(struct hda_codec *codec,
				       struct hda_jack_callback *cb)
{
	/* Windows sets 0x10 to 0x8420 for Node 0x20 which is
	 * responsible from changes between speakers and headphones
	 */
	if (snd_hda_jack_detect_state(codec, 0x21) == HDA_JACK_PRESENT)
		alc_write_coef_idx(codec, 0x10, 0x8420);
	else
		alc_write_coef_idx(codec, 0x10, 0x0a20);
}

static void alc294_fixup_gu502_hp(struct hda_codec *codec,
				  const struct hda_fixup *fix, int action)
{
	if (!is_jack_detectable(codec, 0x21))
		return;

	switch (action) {
	case HDA_FIXUP_ACT_PRE_PROBE:
		snd_hda_jack_detect_enable_callback(codec, 0x21,
				alc294_gu502_toggle_output);
		break;
	case HDA_FIXUP_ACT_INIT:
		alc294_gu502_toggle_output(codec, NULL);
		break;
	}
}

static void  alc285_fixup_hp_gpio_amp_init(struct hda_codec *codec,
			      const struct hda_fixup *fix, int action)
{
	if (action != HDA_FIXUP_ACT_INIT)
		return;

	msleep(100);
	alc_write_coef_idx(codec, 0x65, 0x0);
}

static void alc274_fixup_hp_headset_mic(struct hda_codec *codec,
				    const struct hda_fixup *fix, int action)
{
	switch (action) {
	case HDA_FIXUP_ACT_INIT:
		alc_combo_jack_hp_jd_restart(codec);
		break;
	}
}

static void alc_fixup_no_int_mic(struct hda_codec *codec,
				    const struct hda_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;

	switch (action) {
	case HDA_FIXUP_ACT_PRE_PROBE:
		/* Mic RING SLEEVE swap for combo jack */
		alc_update_coef_idx(codec, 0x45, 0xf<<12 | 1<<10, 5<<12);
		spec->no_internal_mic_pin = true;
		break;
	case HDA_FIXUP_ACT_INIT:
		alc_combo_jack_hp_jd_restart(codec);
		break;
	}
}

/* GPIO1 = amplifier on/off
 * GPIO3 = mic mute LED
 */
static void alc285_fixup_hp_spectre_x360_eb1(struct hda_codec *codec,
					  const struct hda_fixup *fix, int action)
{
	static const hda_nid_t conn[] = { 0x02 };

	struct alc_spec *spec = codec->spec;
	static const struct hda_pintbl pincfgs[] = {
		{ 0x14, 0x90170110 },  /* front/high speakers */
		{ 0x17, 0x90170130 },  /* back/bass speakers */
		{ }
	};

	//enable micmute led
	alc_fixup_hp_gpio_led(codec, action, 0x00, 0x04);

	switch (action) {
	case HDA_FIXUP_ACT_PRE_PROBE:
		spec->micmute_led_polarity = 1;
		/* needed for amp of back speakers */
		spec->gpio_mask |= 0x01;
		spec->gpio_dir |= 0x01;
		snd_hda_apply_pincfgs(codec, pincfgs);
		/* share DAC to have unified volume control */
		snd_hda_override_conn_list(codec, 0x14, ARRAY_SIZE(conn), conn);
		snd_hda_override_conn_list(codec, 0x17, ARRAY_SIZE(conn), conn);
		break;
	case HDA_FIXUP_ACT_INIT:
		/* need to toggle GPIO to enable the amp of back speakers */
		alc_update_gpio_data(codec, 0x01, true);
		msleep(100);
		alc_update_gpio_data(codec, 0x01, false);
		break;
	}
}

static void alc285_fixup_hp_spectre_x360(struct hda_codec *codec,
					  const struct hda_fixup *fix, int action)
{
	static const hda_nid_t conn[] = { 0x02 };
	static const struct hda_pintbl pincfgs[] = {
		{ 0x14, 0x90170110 },  /* rear speaker */
		{ }
	};

	switch (action) {
	case HDA_FIXUP_ACT_PRE_PROBE:
		snd_hda_apply_pincfgs(codec, pincfgs);
		/* force front speaker to DAC1 */
		snd_hda_override_conn_list(codec, 0x17, ARRAY_SIZE(conn), conn);
		break;
	}
}

static void alc285_fixup_hp_envy_x360(struct hda_codec *codec,
				      const struct hda_fixup *fix,
				      int action)
{
	static const struct coef_fw coefs[] = {
		WRITE_COEF(0x08, 0x6a0c), WRITE_COEF(0x0d, 0xa023),
		WRITE_COEF(0x10, 0x0320), WRITE_COEF(0x1a, 0x8c03),
		WRITE_COEF(0x25, 0x1800), WRITE_COEF(0x26, 0x003a),
		WRITE_COEF(0x28, 0x1dfe), WRITE_COEF(0x29, 0xb014),
		WRITE_COEF(0x2b, 0x1dfe), WRITE_COEF(0x37, 0xfe15),
		WRITE_COEF(0x38, 0x7909), WRITE_COEF(0x45, 0xd489),
		WRITE_COEF(0x46, 0x00f4), WRITE_COEF(0x4a, 0x21e0),
		WRITE_COEF(0x66, 0x03f0), WRITE_COEF(0x67, 0x1000),
		WRITE_COEF(0x6e, 0x1005), { }
	};

	static const struct hda_pintbl pincfgs[] = {
		{ 0x12, 0xb7a60130 },  /* Internal microphone*/
		{ 0x14, 0x90170150 },  /* B&O soundbar speakers */
		{ 0x17, 0x90170153 },  /* Side speakers */
		{ 0x19, 0x03a11040 },  /* Headset microphone */
		{ }
	};

	switch (action) {
	case HDA_FIXUP_ACT_PRE_PROBE:
		snd_hda_apply_pincfgs(codec, pincfgs);

		/* Fixes volume control problem for side speakers */
		alc295_fixup_disable_dac3(codec, fix, action);

		/* Fixes no sound from headset speaker */
		snd_hda_codec_amp_stereo(codec, 0x21, HDA_OUTPUT, 0, -1, 0);

		/* Auto-enable headset mic when plugged */
		snd_hda_jack_set_gating_jack(codec, 0x19, 0x21);

		/* Headset mic volume enhancement */
		snd_hda_codec_set_pin_target(codec, 0x19, PIN_VREF50);
		break;
	case HDA_FIXUP_ACT_INIT:
		alc_process_coef_fw(codec, coefs);
		break;
	case HDA_FIXUP_ACT_BUILD:
		rename_ctl(codec, "Bass Speaker Playback Volume",
			   "B&O-Tuned Playback Volume");
		rename_ctl(codec, "Front Playback Switch",
			   "B&O Soundbar Playback Switch");
		rename_ctl(codec, "Bass Speaker Playback Switch",
			   "Side Speaker Playback Switch");
		break;
	}
}

/* for hda_fixup_thinkpad_acpi() */
#include "thinkpad_helper.c"

static void alc_fixup_thinkpad_acpi(struct hda_codec *codec,
				    const struct hda_fixup *fix, int action)
{
	alc_fixup_no_shutup(codec, fix, action); /* reduce click noise */
	hda_fixup_thinkpad_acpi(codec, fix, action);
}

/* Fixup for Lenovo Legion 15IMHg05 speaker output on headset removal. */
static void alc287_fixup_legion_15imhg05_speakers(struct hda_codec *codec,
						  const struct hda_fixup *fix,
						  int action)
{
	struct alc_spec *spec = codec->spec;

	switch (action) {
	case HDA_FIXUP_ACT_PRE_PROBE:
		spec->gen.suppress_auto_mute = 1;
		break;
	}
}

static void comp_acpi_device_notify(acpi_handle handle, u32 event, void *data)
{
	struct hda_codec *cdc = data;
	struct alc_spec *spec = cdc->spec;

	codec_info(cdc, "ACPI Notification %d\n", event);

	hda_component_acpi_device_notify(spec->comps, ARRAY_SIZE(spec->comps),
					 handle, event, data);
}

static int comp_bind(struct device *dev)
{
	struct hda_codec *cdc = dev_to_hda_codec(dev);
	struct alc_spec *spec = cdc->spec;
	int ret;

	ret = hda_component_manager_bind(cdc, spec->comps);
	if (ret)
		return ret;

	return hda_component_manager_bind_acpi_notifications(cdc,
							     spec->comps, ARRAY_SIZE(spec->comps),
							     comp_acpi_device_notify, cdc);
}

static void comp_unbind(struct device *dev)
{
	struct hda_codec *cdc = dev_to_hda_codec(dev);
	struct alc_spec *spec = cdc->spec;

	hda_component_manager_unbind_acpi_notifications(cdc, spec->comps, comp_acpi_device_notify);
	hda_component_manager_unbind(cdc, spec->comps);
}

static const struct component_master_ops comp_master_ops = {
	.bind = comp_bind,
	.unbind = comp_unbind,
};

static void comp_generic_playback_hook(struct hda_pcm_stream *hinfo, struct hda_codec *cdc,
				       struct snd_pcm_substream *sub, int action)
{
	struct alc_spec *spec = cdc->spec;

	hda_component_manager_playback_hook(spec->comps, ARRAY_SIZE(spec->comps), action);
}

static void comp_generic_fixup(struct hda_codec *cdc, int action, const char *bus,
			       const char *hid, const char *match_str, int count)
{
	struct alc_spec *spec = cdc->spec;
	int ret;

	switch (action) {
	case HDA_FIXUP_ACT_PRE_PROBE:
		ret = hda_component_manager_init(cdc, spec->comps, count, bus, hid,
						 match_str, &comp_master_ops);
		if (ret)
			return;

		spec->gen.pcm_playback_hook = comp_generic_playback_hook;
		break;
	case HDA_FIXUP_ACT_FREE:
		hda_component_manager_free(cdc, &comp_master_ops);
		break;
	}
}

static void cs35l41_fixup_i2c_two(struct hda_codec *cdc, const struct hda_fixup *fix, int action)
{
	comp_generic_fixup(cdc, action, "i2c", "CSC3551", "-%s:00-cs35l41-hda.%d", 2);
}

static void cs35l41_fixup_i2c_four(struct hda_codec *cdc, const struct hda_fixup *fix, int action)
{
	comp_generic_fixup(cdc, action, "i2c", "CSC3551", "-%s:00-cs35l41-hda.%d", 4);
}

static void cs35l41_fixup_spi_two(struct hda_codec *codec, const struct hda_fixup *fix, int action)
{
	comp_generic_fixup(codec, action, "spi", "CSC3551", "-%s:00-cs35l41-hda.%d", 2);
}

static void cs35l41_fixup_spi_four(struct hda_codec *codec, const struct hda_fixup *fix, int action)
{
	comp_generic_fixup(codec, action, "spi", "CSC3551", "-%s:00-cs35l41-hda.%d", 4);
}

static void alc287_fixup_legion_16achg6_speakers(struct hda_codec *cdc, const struct hda_fixup *fix,
						 int action)
{
	comp_generic_fixup(cdc, action, "i2c", "CLSA0100", "-%s:00-cs35l41-hda.%d", 2);
}

static void alc287_fixup_legion_16ithg6_speakers(struct hda_codec *cdc, const struct hda_fixup *fix,
						 int action)
{
	comp_generic_fixup(cdc, action, "i2c", "CLSA0101", "-%s:00-cs35l41-hda.%d", 2);
}

static void cs35l56_fixup_i2c_two(struct hda_codec *cdc, const struct hda_fixup *fix, int action)
{
	comp_generic_fixup(cdc, action, "i2c", "CSC3556", "-%s:00-cs35l56-hda.%d", 2);
}

static void cs35l56_fixup_i2c_four(struct hda_codec *cdc, const struct hda_fixup *fix, int action)
{
	comp_generic_fixup(cdc, action, "i2c", "CSC3556", "-%s:00-cs35l56-hda.%d", 4);
}

static void cs35l56_fixup_spi_two(struct hda_codec *cdc, const struct hda_fixup *fix, int action)
{
	comp_generic_fixup(cdc, action, "spi", "CSC3556", "-%s:00-cs35l56-hda.%d", 2);
}

static void cs35l56_fixup_spi_four(struct hda_codec *cdc, const struct hda_fixup *fix, int action)
{
	comp_generic_fixup(cdc, action, "spi", "CSC3556", "-%s:00-cs35l56-hda.%d", 4);
}

static void alc285_fixup_asus_ga403u(struct hda_codec *cdc, const struct hda_fixup *fix, int action)
{
	/*
	 * The same SSID has been re-used in different hardware, they have
	 * different codecs and the newer GA403U has a ALC285.
	 */
	if (cdc->core.vendor_id == 0x10ec0285)
		cs35l56_fixup_i2c_two(cdc, fix, action);
	else
		alc_fixup_inv_dmic(cdc, fix, action);
}

static void tas2781_fixup_i2c(struct hda_codec *cdc,
	const struct hda_fixup *fix, int action)
{
	comp_generic_fixup(cdc, action, "i2c", "TIAS2781", "-%s:00", 1);
}

static void yoga7_14arb7_fixup_i2c(struct hda_codec *cdc,
	const struct hda_fixup *fix, int action)
{
	comp_generic_fixup(cdc, action, "i2c", "INT8866", "-%s:00", 1);
}

static void alc256_fixup_acer_sfg16_micmute_led(struct hda_codec *codec,
	const struct hda_fixup *fix, int action)
{
	alc_fixup_hp_gpio_led(codec, action, 0, 0x04);
}


/* for alc295_fixup_hp_top_speakers */
#include "hp_x360_helper.c"

/* for alc285_fixup_ideapad_s740_coef() */
#include "ideapad_s740_helper.c"

static const struct coef_fw alc256_fixup_set_coef_defaults_coefs[] = {
	WRITE_COEF(0x10, 0x0020), WRITE_COEF(0x24, 0x0000),
	WRITE_COEF(0x26, 0x0000), WRITE_COEF(0x29, 0x3000),
	WRITE_COEF(0x37, 0xfe05), WRITE_COEF(0x45, 0x5089),
	{}
};

static void alc256_fixup_set_coef_defaults(struct hda_codec *codec,
					   const struct hda_fixup *fix,
					   int action)
{
	/*
	 * A certain other OS sets these coeffs to different values. On at least
	 * one TongFang barebone these settings might survive even a cold
	 * reboot. So to restore a clean slate the values are explicitly reset
	 * to default here. Without this, the external microphone is always in a
	 * plugged-in state, while the internal microphone is always in an
	 * unplugged state, breaking the ability to use the internal microphone.
	 */
	alc_process_coef_fw(codec, alc256_fixup_set_coef_defaults_coefs);
}

static const struct coef_fw alc233_fixup_no_audio_jack_coefs[] = {
	WRITE_COEF(0x1a, 0x9003), WRITE_COEF(0x1b, 0x0e2b), WRITE_COEF(0x37, 0xfe06),
	WRITE_COEF(0x38, 0x4981), WRITE_COEF(0x45, 0xd489), WRITE_COEF(0x46, 0x0074),
	WRITE_COEF(0x49, 0x0149),
	{}
};

static void alc233_fixup_no_audio_jack(struct hda_codec *codec,
				       const struct hda_fixup *fix,
				       int action)
{
	/*
	 * The audio jack input and output is not detected on the ASRock NUC Box
	 * 1100 series when cold booting without this fix. Warm rebooting from a
	 * certain other OS makes the audio functional, as COEF settings are
	 * preserved in this case. This fix sets these altered COEF values as
	 * the default.
	 */
	alc_process_coef_fw(codec, alc233_fixup_no_audio_jack_coefs);
}

static void alc256_fixup_mic_no_presence_and_resume(struct hda_codec *codec,
						    const struct hda_fixup *fix,
						    int action)
{
	/*
	 * The Clevo NJ51CU comes either with the ALC293 or the ALC256 codec,
	 * but uses the 0x8686 subproduct id in both cases. The ALC256 codec
	 * needs an additional quirk for sound working after suspend and resume.
	 */
	if (codec->core.vendor_id == 0x10ec0256) {
		alc_update_coef_idx(codec, 0x10, 1<<9, 0);
		snd_hda_codec_set_pincfg(codec, 0x19, 0x04a11120);
	} else {
		snd_hda_codec_set_pincfg(codec, 0x1a, 0x04a1113c);
	}
}

static void alc256_decrease_headphone_amp_val(struct hda_codec *codec,
					      const struct hda_fixup *fix, int action)
{
	u32 caps;
	u8 nsteps, offs;

	if (action != HDA_FIXUP_ACT_PRE_PROBE)
		return;

	caps = query_amp_caps(codec, 0x3, HDA_OUTPUT);
	nsteps = ((caps & AC_AMPCAP_NUM_STEPS) >> AC_AMPCAP_NUM_STEPS_SHIFT) - 10;
	offs = ((caps & AC_AMPCAP_OFFSET) >> AC_AMPCAP_OFFSET_SHIFT) - 10;
	caps &= ~AC_AMPCAP_NUM_STEPS & ~AC_AMPCAP_OFFSET;
	caps |= (nsteps << AC_AMPCAP_NUM_STEPS_SHIFT) | (offs << AC_AMPCAP_OFFSET_SHIFT);

	if (snd_hda_override_amp_caps(codec, 0x3, HDA_OUTPUT, caps))
		codec_warn(codec, "failed to override amp caps for NID 0x3\n");
}

static void alc_fixup_dell4_mic_no_presence_quiet(struct hda_codec *codec,
						  const struct hda_fixup *fix,
						  int action)
{
	struct alc_spec *spec = codec->spec;
	struct hda_input_mux *imux = &spec->gen.input_mux;
	int i;

	alc269_fixup_limit_int_mic_boost(codec, fix, action);

	switch (action) {
	case HDA_FIXUP_ACT_PRE_PROBE:
		/**
		 * Set the vref of pin 0x19 (Headset Mic) and pin 0x1b (Headphone Mic)
		 * to Hi-Z to avoid pop noises at startup and when plugging and
		 * unplugging headphones.
		 */
		snd_hda_codec_set_pin_target(codec, 0x19, PIN_VREFHIZ);
		snd_hda_codec_set_pin_target(codec, 0x1b, PIN_VREFHIZ);
		break;
	case HDA_FIXUP_ACT_PROBE:
		/**
		 * Make the internal mic (0x12) the default input source to
		 * prevent pop noises on cold boot.
		 */
		for (i = 0; i < imux->num_items; i++) {
			if (spec->gen.imux_pins[i] == 0x12) {
				spec->gen.cur_mux[0] = i;
				break;
			}
		}
		break;
	}
}

static void alc287_fixup_yoga9_14iap7_bass_spk_pin(struct hda_codec *codec,
					  const struct hda_fixup *fix, int action)
{
	/*
	 * The Pin Complex 0x17 for the bass speakers is wrongly reported as
	 * unconnected.
	 */
	static const struct hda_pintbl pincfgs[] = {
		{ 0x17, 0x90170121 },
		{ }
	};
	/*
	 * Avoid DAC 0x06 and 0x08, as they have no volume controls.
	 * DAC 0x02 and 0x03 would be fine.
	 */
	static const hda_nid_t conn[] = { 0x02, 0x03 };
	/*
	 * Prefer both speakerbar (0x14) and bass speakers (0x17) connected to DAC 0x02.
	 * Headphones (0x21) are connected to DAC 0x03.
	 */
	static const hda_nid_t preferred_pairs[] = {
		0x14, 0x02,
		0x17, 0x02,
		0x21, 0x03,
		0
	};
	struct alc_spec *spec = codec->spec;

	switch (action) {
	case HDA_FIXUP_ACT_PRE_PROBE:
		snd_hda_apply_pincfgs(codec, pincfgs);
		snd_hda_override_conn_list(codec, 0x17, ARRAY_SIZE(conn), conn);
		spec->gen.preferred_dacs = preferred_pairs;
		break;
	}
}

static void alc295_fixup_dell_inspiron_top_speakers(struct hda_codec *codec,
					  const struct hda_fixup *fix, int action)
{
	static const struct hda_pintbl pincfgs[] = {
		{ 0x14, 0x90170151 },
		{ 0x17, 0x90170150 },
		{ }
	};
	static const hda_nid_t conn[] = { 0x02, 0x03 };
	static const hda_nid_t preferred_pairs[] = {
		0x14, 0x02,
		0x17, 0x03,
		0x21, 0x02,
		0
	};
	struct alc_spec *spec = codec->spec;

	alc_fixup_no_shutup(codec, fix, action);

	switch (action) {
	case HDA_FIXUP_ACT_PRE_PROBE:
		snd_hda_apply_pincfgs(codec, pincfgs);
		snd_hda_override_conn_list(codec, 0x17, ARRAY_SIZE(conn), conn);
		spec->gen.preferred_dacs = preferred_pairs;
		break;
	}
}

/* Forcibly assign NID 0x03 to HP while NID 0x02 to SPK */
static void alc287_fixup_bind_dacs(struct hda_codec *codec,
				    const struct hda_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;
	static const hda_nid_t conn[] = { 0x02, 0x03 }; /* exclude 0x06 */
	static const hda_nid_t preferred_pairs[] = {
		0x17, 0x02, 0x21, 0x03, 0
	};

	if (action != HDA_FIXUP_ACT_PRE_PROBE)
		return;

	snd_hda_override_conn_list(codec, 0x17, ARRAY_SIZE(conn), conn);
	spec->gen.preferred_dacs = preferred_pairs;
	spec->gen.auto_mute_via_amp = 1;
	if (spec->gen.autocfg.speaker_pins[0] != 0x14) {
		snd_hda_codec_write_cache(codec, 0x14, 0, AC_VERB_SET_PIN_WIDGET_CONTROL,
					0x0); /* Make sure 0x14 was disable */
	}
}
/* Fix none verb table of Headset Mic pin */
static void alc_fixup_headset_mic(struct hda_codec *codec,
				   const struct hda_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;
	static const struct hda_pintbl pincfgs[] = {
		{ 0x19, 0x03a1103c },
		{ }
	};

	switch (action) {
	case HDA_FIXUP_ACT_PRE_PROBE:
		snd_hda_apply_pincfgs(codec, pincfgs);
		alc_update_coef_idx(codec, 0x45, 0xf<<12 | 1<<10, 5<<12);
		spec->parse_flags |= HDA_PINCFG_HEADSET_MIC;
		break;
	}
}

static void alc245_fixup_hp_spectre_x360_eu0xxx(struct hda_codec *codec,
					  const struct hda_fixup *fix, int action)
{
	/*
	 * The Pin Complex 0x14 for the treble speakers is wrongly reported as
	 * unconnected.
	 * The Pin Complex 0x17 for the bass speakers has the lowest association
	 * and sequence values so shift it up a bit to squeeze 0x14 in.
	 */
	static const struct hda_pintbl pincfgs[] = {
		{ 0x14, 0x90170110 }, // top/treble
		{ 0x17, 0x90170111 }, // bottom/bass
		{ }
	};

	/*
	 * Force DAC 0x02 for the bass speakers 0x17.
	 */
	static const hda_nid_t conn[] = { 0x02 };

	switch (action) {
	case HDA_FIXUP_ACT_PRE_PROBE:
		snd_hda_apply_pincfgs(codec, pincfgs);
		snd_hda_override_conn_list(codec, 0x17, ARRAY_SIZE(conn), conn);
		break;
	}

	cs35l41_fixup_i2c_two(codec, fix, action);
	alc245_fixup_hp_mute_led_coefbit(codec, fix, action);
	alc245_fixup_hp_gpio_led(codec, fix, action);
}

/*
 * ALC287 PCM hooks
 */
static void alc287_alc1318_playback_pcm_hook(struct hda_pcm_stream *hinfo,
				   struct hda_codec *codec,
				   struct snd_pcm_substream *substream,
				   int action)
{
	alc_write_coef_idx(codec, 0x10, 0x8806); /* Change MLK to GPIO3 */
	switch (action) {
	case HDA_GEN_PCM_ACT_OPEN:
		alc_write_coefex_idx(codec, 0x5a, 0x00, 0x954f); /* write gpio3 to high */
		break;
	case HDA_GEN_PCM_ACT_CLOSE:
		alc_write_coefex_idx(codec, 0x5a, 0x00, 0x554f); /* write gpio3 as default value */
		break;
	}
}

static void __maybe_unused alc287_s4_power_gpio3_default(struct hda_codec *codec)
{
	if (is_s4_suspend(codec)) {
		alc_write_coef_idx(codec, 0x10, 0x8806); /* Change MLK to GPIO3 */
		alc_write_coefex_idx(codec, 0x5a, 0x00, 0x554f); /* write gpio3 as default value */
	}
}

static void alc287_fixup_lenovo_thinkpad_with_alc1318(struct hda_codec *codec,
			       const struct hda_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;

	if (action != HDA_FIXUP_ACT_PRE_PROBE)
		return;
#ifdef CONFIG_PM
	spec->power_hook = alc287_s4_power_gpio3_default;
#endif
	spec->gen.pcm_playback_hook = alc287_alc1318_playback_pcm_hook;
}


enum {
	ALC269_FIXUP_GPIO2,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ALC269_FIXUP_SONY_VAIO,
	ALC275_FIXUP_SONY_VAIO_GPIO2,
	ALC269_FIXUP_DELL_M101Z,
	ALC269_FIXUP_SKU_IGNORE,
	ALC269_FIXUP_ASUS_G73JW,
<<<<<<< HEAD
	ALC269_FIXUP_LENOVO_EAPD,
	ALC275_FIXUP_SONY_HWEQ,
	ALC271_FIXUP_DMIC,
	ALC269_FIXUP_PCM_44K,
	ALC269_FIXUP_STEREO_DMIC,
	ALC269_FIXUP_QUANTA_MUTE,
	ALC269_FIXUP_LIFEBOOK,
=======
	ALC269_FIXUP_ASUS_N7601ZM_PINS,
	ALC269_FIXUP_ASUS_N7601ZM,
	ALC269_FIXUP_LENOVO_EAPD,
	ALC275_FIXUP_SONY_HWEQ,
	ALC275_FIXUP_SONY_DISABLE_AAMIX,
	ALC271_FIXUP_DMIC,
	ALC269_FIXUP_PCM_44K,
	ALC269_FIXUP_STEREO_DMIC,
	ALC269_FIXUP_HEADSET_MIC,
	ALC269_FIXUP_QUANTA_MUTE,
	ALC269_FIXUP_LIFEBOOK,
	ALC269_FIXUP_LIFEBOOK_EXTMIC,
	ALC269_FIXUP_LIFEBOOK_HP_PIN,
	ALC269_FIXUP_LIFEBOOK_NO_HP_TO_LINEOUT,
	ALC255_FIXUP_LIFEBOOK_U7x7_HEADSET_MIC,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ALC269_FIXUP_AMIC,
	ALC269_FIXUP_DMIC,
	ALC269VB_FIXUP_AMIC,
	ALC269VB_FIXUP_DMIC,
<<<<<<< HEAD
	ALC269_FIXUP_MIC2_MUTE_LED,
	ALC269_FIXUP_LENOVO_DOCK,
	ALC269_FIXUP_PINCFG_NO_HP_TO_LINEOUT,
};

static const struct alc_fixup alc269_fixups[] = {
	[ALC269_FIXUP_SONY_VAIO] = {
		.type = ALC_FIXUP_VERBS,
		.v.verbs = (const struct hda_verb[]) {
			{0x19, AC_VERB_SET_PIN_WIDGET_CONTROL, PIN_VREFGRD},
=======
	ALC269_FIXUP_HP_MUTE_LED,
	ALC269_FIXUP_HP_MUTE_LED_MIC1,
	ALC269_FIXUP_HP_MUTE_LED_MIC2,
	ALC269_FIXUP_HP_MUTE_LED_MIC3,
	ALC269_FIXUP_HP_GPIO_LED,
	ALC269_FIXUP_HP_GPIO_MIC1_LED,
	ALC269_FIXUP_HP_LINE1_MIC1_LED,
	ALC269_FIXUP_INV_DMIC,
	ALC269_FIXUP_LENOVO_DOCK,
	ALC269_FIXUP_LENOVO_DOCK_LIMIT_BOOST,
	ALC269_FIXUP_NO_SHUTUP,
	ALC286_FIXUP_SONY_MIC_NO_PRESENCE,
	ALC269_FIXUP_PINCFG_NO_HP_TO_LINEOUT,
	ALC269_FIXUP_DELL1_MIC_NO_PRESENCE,
	ALC269_FIXUP_DELL2_MIC_NO_PRESENCE,
	ALC269_FIXUP_DELL3_MIC_NO_PRESENCE,
	ALC269_FIXUP_DELL4_MIC_NO_PRESENCE,
	ALC269_FIXUP_DELL4_MIC_NO_PRESENCE_QUIET,
	ALC269_FIXUP_HEADSET_MODE,
	ALC269_FIXUP_HEADSET_MODE_NO_HP_MIC,
	ALC269_FIXUP_ASPIRE_HEADSET_MIC,
	ALC269_FIXUP_ASUS_X101_FUNC,
	ALC269_FIXUP_ASUS_X101_VERB,
	ALC269_FIXUP_ASUS_X101,
	ALC271_FIXUP_AMIC_MIC2,
	ALC271_FIXUP_HP_GATE_MIC_JACK,
	ALC271_FIXUP_HP_GATE_MIC_JACK_E1_572,
	ALC269_FIXUP_ACER_AC700,
	ALC269_FIXUP_LIMIT_INT_MIC_BOOST,
	ALC269VB_FIXUP_ASUS_ZENBOOK,
	ALC269VB_FIXUP_ASUS_ZENBOOK_UX31A,
	ALC269VB_FIXUP_ASUS_MIC_NO_PRESENCE,
	ALC269_FIXUP_LIMIT_INT_MIC_BOOST_MUTE_LED,
	ALC269VB_FIXUP_ORDISSIMO_EVE2,
	ALC283_FIXUP_CHROME_BOOK,
	ALC283_FIXUP_SENSE_COMBO_JACK,
	ALC282_FIXUP_ASUS_TX300,
	ALC283_FIXUP_INT_MIC,
	ALC290_FIXUP_MONO_SPEAKERS,
	ALC290_FIXUP_MONO_SPEAKERS_HSJACK,
	ALC290_FIXUP_SUBWOOFER,
	ALC290_FIXUP_SUBWOOFER_HSJACK,
	ALC269_FIXUP_THINKPAD_ACPI,
	ALC269_FIXUP_DMIC_THINKPAD_ACPI,
	ALC269VB_FIXUP_CHUWI_COREBOOK_XPRO,
	ALC255_FIXUP_ACER_MIC_NO_PRESENCE,
	ALC255_FIXUP_ASUS_MIC_NO_PRESENCE,
	ALC255_FIXUP_DELL1_MIC_NO_PRESENCE,
	ALC255_FIXUP_DELL2_MIC_NO_PRESENCE,
	ALC255_FIXUP_HEADSET_MODE,
	ALC255_FIXUP_HEADSET_MODE_NO_HP_MIC,
	ALC293_FIXUP_DELL1_MIC_NO_PRESENCE,
	ALC292_FIXUP_TPT440_DOCK,
	ALC292_FIXUP_TPT440,
	ALC283_FIXUP_HEADSET_MIC,
	ALC255_FIXUP_MIC_MUTE_LED,
	ALC282_FIXUP_ASPIRE_V5_PINS,
	ALC269VB_FIXUP_ASPIRE_E1_COEF,
	ALC280_FIXUP_HP_GPIO4,
	ALC286_FIXUP_HP_GPIO_LED,
	ALC280_FIXUP_HP_GPIO2_MIC_HOTKEY,
	ALC280_FIXUP_HP_DOCK_PINS,
	ALC269_FIXUP_HP_DOCK_GPIO_MIC1_LED,
	ALC280_FIXUP_HP_9480M,
	ALC245_FIXUP_HP_X360_AMP,
	ALC285_FIXUP_HP_SPECTRE_X360_EB1,
	ALC285_FIXUP_HP_ENVY_X360,
	ALC288_FIXUP_DELL_HEADSET_MODE,
	ALC288_FIXUP_DELL1_MIC_NO_PRESENCE,
	ALC288_FIXUP_DELL_XPS_13,
	ALC288_FIXUP_DISABLE_AAMIX,
	ALC292_FIXUP_DELL_E7X_AAMIX,
	ALC292_FIXUP_DELL_E7X,
	ALC292_FIXUP_DISABLE_AAMIX,
	ALC293_FIXUP_DISABLE_AAMIX_MULTIJACK,
	ALC298_FIXUP_ALIENWARE_MIC_NO_PRESENCE,
	ALC298_FIXUP_DELL1_MIC_NO_PRESENCE,
	ALC298_FIXUP_DELL_AIO_MIC_NO_PRESENCE,
	ALC275_FIXUP_DELL_XPS,
	ALC293_FIXUP_LENOVO_SPK_NOISE,
	ALC233_FIXUP_LENOVO_LINE2_MIC_HOTKEY,
	ALC255_FIXUP_DELL_SPK_NOISE,
	ALC225_FIXUP_DISABLE_MIC_VREF,
	ALC225_FIXUP_DELL1_MIC_NO_PRESENCE,
	ALC295_FIXUP_DISABLE_DAC3,
	ALC285_FIXUP_SPEAKER2_TO_DAC1,
	ALC285_FIXUP_ASUS_SPEAKER2_TO_DAC1,
	ALC285_FIXUP_ASUS_HEADSET_MIC,
	ALC285_FIXUP_ASUS_SPI_REAR_SPEAKERS,
	ALC285_FIXUP_ASUS_I2C_SPEAKER2_TO_DAC1,
	ALC285_FIXUP_ASUS_I2C_HEADSET_MIC,
	ALC280_FIXUP_HP_HEADSET_MIC,
	ALC221_FIXUP_HP_FRONT_MIC,
	ALC292_FIXUP_TPT460,
	ALC298_FIXUP_SPK_VOLUME,
	ALC298_FIXUP_LENOVO_SPK_VOLUME,
	ALC256_FIXUP_DELL_INSPIRON_7559_SUBWOOFER,
	ALC269_FIXUP_ATIV_BOOK_8,
	ALC221_FIXUP_HP_288PRO_MIC_NO_PRESENCE,
	ALC221_FIXUP_HP_MIC_NO_PRESENCE,
	ALC256_FIXUP_ASUS_HEADSET_MODE,
	ALC256_FIXUP_ASUS_MIC,
	ALC256_FIXUP_ASUS_AIO_GPIO2,
	ALC233_FIXUP_ASUS_MIC_NO_PRESENCE,
	ALC233_FIXUP_EAPD_COEF_AND_MIC_NO_PRESENCE,
	ALC233_FIXUP_LENOVO_MULTI_CODECS,
	ALC233_FIXUP_ACER_HEADSET_MIC,
	ALC294_FIXUP_LENOVO_MIC_LOCATION,
	ALC225_FIXUP_DELL_WYSE_MIC_NO_PRESENCE,
	ALC225_FIXUP_S3_POP_NOISE,
	ALC700_FIXUP_INTEL_REFERENCE,
	ALC274_FIXUP_DELL_BIND_DACS,
	ALC274_FIXUP_DELL_AIO_LINEOUT_VERB,
	ALC298_FIXUP_TPT470_DOCK_FIX,
	ALC298_FIXUP_TPT470_DOCK,
	ALC255_FIXUP_DUMMY_LINEOUT_VERB,
	ALC255_FIXUP_DELL_HEADSET_MIC,
	ALC256_FIXUP_HUAWEI_MACH_WX9_PINS,
	ALC298_FIXUP_HUAWEI_MBX_STEREO,
	ALC295_FIXUP_HP_X360,
	ALC221_FIXUP_HP_HEADSET_MIC,
	ALC285_FIXUP_LENOVO_HEADPHONE_NOISE,
	ALC295_FIXUP_HP_AUTO_MUTE,
	ALC286_FIXUP_ACER_AIO_MIC_NO_PRESENCE,
	ALC294_FIXUP_ASUS_MIC,
	ALC294_FIXUP_ASUS_HEADSET_MIC,
	ALC294_FIXUP_ASUS_SPK,
	ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE,
	ALC285_FIXUP_LENOVO_PC_BEEP_IN_NOISE,
	ALC255_FIXUP_ACER_HEADSET_MIC,
	ALC295_FIXUP_CHROME_BOOK,
	ALC225_FIXUP_HEADSET_JACK,
	ALC225_FIXUP_DELL_WYSE_AIO_MIC_NO_PRESENCE,
	ALC225_FIXUP_WYSE_AUTO_MUTE,
	ALC225_FIXUP_WYSE_DISABLE_MIC_VREF,
	ALC286_FIXUP_ACER_AIO_HEADSET_MIC,
	ALC256_FIXUP_ASUS_HEADSET_MIC,
	ALC256_FIXUP_ASUS_MIC_NO_PRESENCE,
	ALC299_FIXUP_PREDATOR_SPK,
	ALC256_FIXUP_MEDION_HEADSET_NO_PRESENCE,
	ALC289_FIXUP_DELL_SPK1,
	ALC289_FIXUP_DELL_SPK2,
	ALC289_FIXUP_DUAL_SPK,
	ALC289_FIXUP_RTK_AMP_DUAL_SPK,
	ALC294_FIXUP_SPK2_TO_DAC1,
	ALC294_FIXUP_ASUS_DUAL_SPK,
	ALC285_FIXUP_THINKPAD_X1_GEN7,
	ALC285_FIXUP_THINKPAD_HEADSET_JACK,
	ALC294_FIXUP_ASUS_ALLY,
	ALC294_FIXUP_ASUS_ALLY_PINS,
	ALC294_FIXUP_ASUS_ALLY_VERBS,
	ALC294_FIXUP_ASUS_ALLY_SPEAKER,
	ALC294_FIXUP_ASUS_HPE,
	ALC294_FIXUP_ASUS_COEF_1B,
	ALC294_FIXUP_ASUS_GX502_HP,
	ALC294_FIXUP_ASUS_GX502_PINS,
	ALC294_FIXUP_ASUS_GX502_VERBS,
	ALC294_FIXUP_ASUS_GU502_HP,
	ALC294_FIXUP_ASUS_GU502_PINS,
	ALC294_FIXUP_ASUS_GU502_VERBS,
	ALC294_FIXUP_ASUS_G513_PINS,
	ALC285_FIXUP_ASUS_G533Z_PINS,
	ALC285_FIXUP_HP_GPIO_LED,
	ALC285_FIXUP_HP_MUTE_LED,
	ALC285_FIXUP_HP_SPECTRE_X360_MUTE_LED,
	ALC236_FIXUP_HP_MUTE_LED_COEFBIT2,
	ALC236_FIXUP_HP_GPIO_LED,
	ALC236_FIXUP_HP_MUTE_LED,
	ALC236_FIXUP_HP_MUTE_LED_MICMUTE_VREF,
	ALC298_FIXUP_SAMSUNG_AMP,
	ALC298_FIXUP_SAMSUNG_HEADPHONE_VERY_QUIET,
	ALC256_FIXUP_SAMSUNG_HEADPHONE_VERY_QUIET,
	ALC295_FIXUP_ASUS_MIC_NO_PRESENCE,
	ALC269VC_FIXUP_ACER_VCOPPERBOX_PINS,
	ALC269VC_FIXUP_ACER_HEADSET_MIC,
	ALC269VC_FIXUP_ACER_MIC_NO_PRESENCE,
	ALC289_FIXUP_ASUS_GA401,
	ALC289_FIXUP_ASUS_GA502,
	ALC256_FIXUP_ACER_MIC_NO_PRESENCE,
	ALC285_FIXUP_HP_GPIO_AMP_INIT,
	ALC269_FIXUP_CZC_B20,
	ALC269_FIXUP_CZC_TMI,
	ALC269_FIXUP_CZC_L101,
	ALC269_FIXUP_LEMOTE_A1802,
	ALC269_FIXUP_LEMOTE_A190X,
	ALC256_FIXUP_INTEL_NUC8_RUGGED,
	ALC233_FIXUP_INTEL_NUC8_DMIC,
	ALC233_FIXUP_INTEL_NUC8_BOOST,
	ALC256_FIXUP_INTEL_NUC10,
	ALC255_FIXUP_XIAOMI_HEADSET_MIC,
	ALC274_FIXUP_HP_MIC,
	ALC274_FIXUP_HP_HEADSET_MIC,
	ALC274_FIXUP_HP_ENVY_GPIO,
	ALC256_FIXUP_ASUS_HPE,
	ALC285_FIXUP_THINKPAD_NO_BASS_SPK_HEADSET_JACK,
	ALC287_FIXUP_HP_GPIO_LED,
	ALC256_FIXUP_HP_HEADSET_MIC,
	ALC245_FIXUP_HP_GPIO_LED,
	ALC236_FIXUP_DELL_AIO_HEADSET_MIC,
	ALC282_FIXUP_ACER_DISABLE_LINEOUT,
	ALC255_FIXUP_ACER_LIMIT_INT_MIC_BOOST,
	ALC256_FIXUP_ACER_HEADSET_MIC,
	ALC285_FIXUP_IDEAPAD_S740_COEF,
	ALC285_FIXUP_HP_LIMIT_INT_MIC_BOOST,
	ALC295_FIXUP_ASUS_DACS,
	ALC295_FIXUP_HP_OMEN,
	ALC285_FIXUP_HP_SPECTRE_X360,
	ALC287_FIXUP_IDEAPAD_BASS_SPK_AMP,
	ALC623_FIXUP_LENOVO_THINKSTATION_P340,
	ALC255_FIXUP_ACER_HEADPHONE_AND_MIC,
	ALC236_FIXUP_HP_LIMIT_INT_MIC_BOOST,
	ALC287_FIXUP_LEGION_15IMHG05_SPEAKERS,
	ALC287_FIXUP_LEGION_15IMHG05_AUTOMUTE,
	ALC287_FIXUP_YOGA7_14ITL_SPEAKERS,
	ALC298_FIXUP_LENOVO_C940_DUET7,
	ALC287_FIXUP_LENOVO_14IRP8_DUETITL,
	ALC287_FIXUP_LENOVO_LEGION_7,
	ALC287_FIXUP_13S_GEN2_SPEAKERS,
	ALC256_FIXUP_SET_COEF_DEFAULTS,
	ALC256_FIXUP_SYSTEM76_MIC_NO_PRESENCE,
	ALC233_FIXUP_NO_AUDIO_JACK,
	ALC256_FIXUP_MIC_NO_PRESENCE_AND_RESUME,
	ALC285_FIXUP_LEGION_Y9000X_SPEAKERS,
	ALC285_FIXUP_LEGION_Y9000X_AUTOMUTE,
	ALC287_FIXUP_LEGION_16ACHG6,
	ALC287_FIXUP_CS35L41_I2C_2,
	ALC287_FIXUP_CS35L41_I2C_2_HP_GPIO_LED,
	ALC287_FIXUP_CS35L41_I2C_4,
	ALC245_FIXUP_CS35L41_SPI_2,
	ALC245_FIXUP_CS35L41_SPI_2_HP_GPIO_LED,
	ALC245_FIXUP_CS35L41_SPI_4,
	ALC245_FIXUP_CS35L41_SPI_4_HP_GPIO_LED,
	ALC285_FIXUP_HP_SPEAKERS_MICMUTE_LED,
	ALC295_FIXUP_FRAMEWORK_LAPTOP_MIC_NO_PRESENCE,
	ALC287_FIXUP_LEGION_16ITHG6,
	ALC287_FIXUP_YOGA9_14IAP7_BASS_SPK,
	ALC287_FIXUP_YOGA9_14IAP7_BASS_SPK_PIN,
	ALC287_FIXUP_YOGA9_14IMH9_BASS_SPK_PIN,
	ALC295_FIXUP_DELL_INSPIRON_TOP_SPEAKERS,
	ALC236_FIXUP_DELL_DUAL_CODECS,
	ALC287_FIXUP_CS35L41_I2C_2_THINKPAD_ACPI,
	ALC287_FIXUP_TAS2781_I2C,
	ALC287_FIXUP_YOGA7_14ARB7_I2C,
	ALC245_FIXUP_HP_MUTE_LED_COEFBIT,
	ALC245_FIXUP_HP_X360_MUTE_LEDS,
	ALC287_FIXUP_THINKPAD_I2S_SPK,
	ALC287_FIXUP_MG_RTKC_CSAMP_CS35L41_I2C_THINKPAD,
	ALC2XX_FIXUP_HEADSET_MIC,
	ALC289_FIXUP_DELL_CS35L41_SPI_2,
	ALC294_FIXUP_CS35L41_I2C_2,
	ALC245_FIXUP_CS35L56_SPI_4_HP_GPIO_LED,
	ALC256_FIXUP_ACER_SFG16_MICMUTE_LED,
	ALC256_FIXUP_HEADPHONE_AMP_VOL,
	ALC245_FIXUP_HP_SPECTRE_X360_EU0XXX,
	ALC285_FIXUP_CS35L56_SPI_2,
	ALC285_FIXUP_CS35L56_I2C_2,
	ALC285_FIXUP_CS35L56_I2C_4,
	ALC285_FIXUP_ASUS_GA403U,
	ALC285_FIXUP_ASUS_GA403U_HEADSET_MIC,
	ALC285_FIXUP_ASUS_GA403U_I2C_SPEAKER2_TO_DAC1,
	ALC285_FIXUP_ASUS_GU605_SPI_2_HEADSET_MIC,
	ALC285_FIXUP_ASUS_GU605_SPI_SPEAKER2_TO_DAC1,
	ALC287_FIXUP_LENOVO_THKPAD_WH_ALC1318,
};

/* A special fixup for Lenovo C940 and Yoga Duet 7;
 * both have the very same PCI SSID, and we need to apply different fixups
 * depending on the codec ID
 */
static void alc298_fixup_lenovo_c940_duet7(struct hda_codec *codec,
					   const struct hda_fixup *fix,
					   int action)
{
	int id;

	if (codec->core.vendor_id == 0x10ec0298)
		id = ALC298_FIXUP_LENOVO_SPK_VOLUME; /* C940 */
	else
		id = ALC287_FIXUP_YOGA7_14ITL_SPEAKERS; /* Duet 7 */
	__snd_hda_apply_fixup(codec, id, action, 0);
}

/* A special fixup for Lenovo Slim/Yoga Pro 9 14IRP8 and Yoga DuetITL 2021;
 * 14IRP8 PCI SSID will mistakenly be matched with the DuetITL codec SSID,
 * so we need to apply a different fixup in this case. The only DuetITL codec
 * SSID reported so far is the 17aa:3802 while the 14IRP8 has the 17aa:38be
 * and 17aa:38bf. If it weren't for the PCI SSID, the 14IRP8 models would
 * have matched correctly by their codecs.
 */
static void alc287_fixup_lenovo_14irp8_duetitl(struct hda_codec *codec,
					      const struct hda_fixup *fix,
					      int action)
{
	int id;

	if (codec->core.subsystem_id == 0x17aa3802)
		id = ALC287_FIXUP_YOGA7_14ITL_SPEAKERS; /* DuetITL */
	else
		id = ALC287_FIXUP_TAS2781_I2C; /* 14IRP8 */
	__snd_hda_apply_fixup(codec, id, action, 0);
}

/* Another hilarious PCI SSID conflict with Lenovo Legion Pro 7 16ARX8H (with
 * TAS2781 codec) and Legion 7i 16IAX7 (with CS35L41 codec);
 * we apply a corresponding fixup depending on the codec SSID instead
 */
static void alc287_fixup_lenovo_legion_7(struct hda_codec *codec,
					 const struct hda_fixup *fix,
					 int action)
{
	int id;

	if (codec->core.subsystem_id == 0x17aa38a8)
		id = ALC287_FIXUP_TAS2781_I2C; /* Legion Pro 7 16ARX8H */
	else
		id = ALC287_FIXUP_CS35L41_I2C_2; /* Legion 7i 16IAX7 */
	__snd_hda_apply_fixup(codec, id, action, 0);
}

static const struct hda_fixup alc269_fixups[] = {
	[ALC269_FIXUP_GPIO2] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_gpio2,
	},
	[ALC269_FIXUP_SONY_VAIO] = {
		.type = HDA_FIXUP_PINCTLS,
		.v.pins = (const struct hda_pintbl[]) {
			{0x19, PIN_VREFGRD},
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			{}
		}
	},
	[ALC275_FIXUP_SONY_VAIO_GPIO2] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_VERBS,
		.v.verbs = (const struct hda_verb[]) {
			{0x01, AC_VERB_SET_GPIO_MASK, 0x04},
			{0x01, AC_VERB_SET_GPIO_DIRECTION, 0x04},
			{0x01, AC_VERB_SET_GPIO_DATA, 0x00},
			{ }
		},
=======
		.type = HDA_FIXUP_FUNC,
		.v.func = alc275_fixup_gpio4_off,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.chained = true,
		.chain_id = ALC269_FIXUP_SONY_VAIO
	},
	[ALC269_FIXUP_DELL_M101Z] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_VERBS,
=======
		.type = HDA_FIXUP_VERBS,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.v.verbs = (const struct hda_verb[]) {
			/* Enables internal speaker */
			{0x20, AC_VERB_SET_COEF_INDEX, 13},
			{0x20, AC_VERB_SET_PROC_COEF, 0x4040},
			{}
		}
	},
	[ALC269_FIXUP_SKU_IGNORE] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_SKU,
		.v.sku = ALC_FIXUP_SKU_IGNORE,
	},
	[ALC269_FIXUP_ASUS_G73JW] = {
		.type = ALC_FIXUP_PINS,
		.v.pins = (const struct alc_pincfg[]) {
=======
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_sku_ignore,
	},
	[ALC269_FIXUP_ASUS_G73JW] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			{ 0x17, 0x99130111 }, /* subwoofer */
			{ }
		}
	},
<<<<<<< HEAD
	[ALC269_FIXUP_LENOVO_EAPD] = {
		.type = ALC_FIXUP_VERBS,
=======
	[ALC269_FIXUP_ASUS_N7601ZM_PINS] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x19, 0x03A11050 },
			{ 0x1a, 0x03A11C30 },
			{ 0x21, 0x03211420 },
			{ }
		}
	},
	[ALC269_FIXUP_ASUS_N7601ZM] = {
		.type = HDA_FIXUP_VERBS,
		.v.verbs = (const struct hda_verb[]) {
			{0x20, AC_VERB_SET_COEF_INDEX, 0x62},
			{0x20, AC_VERB_SET_PROC_COEF, 0xa007},
			{0x20, AC_VERB_SET_COEF_INDEX, 0x10},
			{0x20, AC_VERB_SET_PROC_COEF, 0x8420},
			{0x20, AC_VERB_SET_COEF_INDEX, 0x0f},
			{0x20, AC_VERB_SET_PROC_COEF, 0x7774},
			{ }
		},
		.chained = true,
		.chain_id = ALC269_FIXUP_ASUS_N7601ZM_PINS,
	},
	[ALC269_FIXUP_LENOVO_EAPD] = {
		.type = HDA_FIXUP_VERBS,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.v.verbs = (const struct hda_verb[]) {
			{0x14, AC_VERB_SET_EAPD_BTLENABLE, 0},
			{}
		}
	},
	[ALC275_FIXUP_SONY_HWEQ] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_FUNC,
=======
		.type = HDA_FIXUP_FUNC,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.v.func = alc269_fixup_hweq,
		.chained = true,
		.chain_id = ALC275_FIXUP_SONY_VAIO_GPIO2
	},
<<<<<<< HEAD
	[ALC271_FIXUP_DMIC] = {
		.type = ALC_FIXUP_FUNC,
		.v.func = alc271_fixup_dmic,
	},
	[ALC269_FIXUP_PCM_44K] = {
		.type = ALC_FIXUP_FUNC,
=======
	[ALC275_FIXUP_SONY_DISABLE_AAMIX] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_disable_aamix,
		.chained = true,
		.chain_id = ALC269_FIXUP_SONY_VAIO
	},
	[ALC271_FIXUP_DMIC] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc271_fixup_dmic,
	},
	[ALC269_FIXUP_PCM_44K] = {
		.type = HDA_FIXUP_FUNC,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.v.func = alc269_fixup_pcm_44k,
		.chained = true,
		.chain_id = ALC269_FIXUP_QUANTA_MUTE
	},
	[ALC269_FIXUP_STEREO_DMIC] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_FUNC,
		.v.func = alc269_fixup_stereo_dmic,
	},
	[ALC269_FIXUP_QUANTA_MUTE] = {
		.type = ALC_FIXUP_FUNC,
		.v.func = alc269_fixup_quanta_mute,
	},
	[ALC269_FIXUP_LIFEBOOK] = {
		.type = ALC_FIXUP_PINS,
		.v.pins = (const struct alc_pincfg[]) {
=======
		.type = HDA_FIXUP_FUNC,
		.v.func = alc269_fixup_stereo_dmic,
	},
	[ALC269_FIXUP_HEADSET_MIC] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc269_fixup_headset_mic,
	},
	[ALC269_FIXUP_QUANTA_MUTE] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc269_fixup_quanta_mute,
	},
	[ALC269_FIXUP_LIFEBOOK] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			{ 0x1a, 0x2101103f }, /* dock line-out */
			{ 0x1b, 0x23a11040 }, /* dock mic-in */
			{ }
		},
		.chained = true,
		.chain_id = ALC269_FIXUP_QUANTA_MUTE
	},
<<<<<<< HEAD
	[ALC269_FIXUP_AMIC] = {
		.type = ALC_FIXUP_PINS,
		.v.pins = (const struct alc_pincfg[]) {
=======
	[ALC269_FIXUP_LIFEBOOK_EXTMIC] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x19, 0x01a1903c }, /* headset mic, with jack detect */
			{ }
		},
	},
	[ALC269_FIXUP_LIFEBOOK_HP_PIN] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x21, 0x0221102f }, /* HP out */
			{ }
		},
	},
	[ALC269_FIXUP_LIFEBOOK_NO_HP_TO_LINEOUT] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc269_fixup_pincfg_no_hp_to_lineout,
	},
	[ALC255_FIXUP_LIFEBOOK_U7x7_HEADSET_MIC] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc269_fixup_pincfg_U7x7_headset_mic,
	},
	[ALC269VB_FIXUP_CHUWI_COREBOOK_XPRO] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x18, 0x03a19020 }, /* headset mic */
			{ 0x1b, 0x90170150 }, /* speaker */
			{ }
		},
	},
	[ALC269_FIXUP_AMIC] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			{ 0x14, 0x99130110 }, /* speaker */
			{ 0x15, 0x0121401f }, /* HP out */
			{ 0x18, 0x01a19c20 }, /* mic */
			{ 0x19, 0x99a3092f }, /* int-mic */
			{ }
		},
	},
	[ALC269_FIXUP_DMIC] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_PINS,
		.v.pins = (const struct alc_pincfg[]) {
=======
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			{ 0x12, 0x99a3092f }, /* int-mic */
			{ 0x14, 0x99130110 }, /* speaker */
			{ 0x15, 0x0121401f }, /* HP out */
			{ 0x18, 0x01a19c20 }, /* mic */
			{ }
		},
	},
	[ALC269VB_FIXUP_AMIC] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_PINS,
		.v.pins = (const struct alc_pincfg[]) {
=======
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			{ 0x14, 0x99130110 }, /* speaker */
			{ 0x18, 0x01a19c20 }, /* mic */
			{ 0x19, 0x99a3092f }, /* int-mic */
			{ 0x21, 0x0121401f }, /* HP out */
			{ }
		},
	},
	[ALC269VB_FIXUP_DMIC] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_PINS,
		.v.pins = (const struct alc_pincfg[]) {
=======
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			{ 0x12, 0x99a3092f }, /* int-mic */
			{ 0x14, 0x99130110 }, /* speaker */
			{ 0x18, 0x01a19c20 }, /* mic */
			{ 0x21, 0x0121401f }, /* HP out */
			{ }
		},
	},
<<<<<<< HEAD
	[ALC269_FIXUP_MIC2_MUTE_LED] = {
		.type = ALC_FIXUP_FUNC,
		.v.func = alc269_fixup_mic2_mute,
	},
	[ALC269_FIXUP_LENOVO_DOCK] = {
		.type = ALC_FIXUP_PINS,
		.v.pins = (const struct alc_pincfg[]) {
=======
	[ALC269_FIXUP_HP_MUTE_LED] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc269_fixup_hp_mute_led,
	},
	[ALC269_FIXUP_HP_MUTE_LED_MIC1] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc269_fixup_hp_mute_led_mic1,
	},
	[ALC269_FIXUP_HP_MUTE_LED_MIC2] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc269_fixup_hp_mute_led_mic2,
	},
	[ALC269_FIXUP_HP_MUTE_LED_MIC3] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc269_fixup_hp_mute_led_mic3,
		.chained = true,
		.chain_id = ALC295_FIXUP_HP_AUTO_MUTE
	},
	[ALC269_FIXUP_HP_GPIO_LED] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc269_fixup_hp_gpio_led,
	},
	[ALC269_FIXUP_HP_GPIO_MIC1_LED] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc269_fixup_hp_gpio_mic1_led,
	},
	[ALC269_FIXUP_HP_LINE1_MIC1_LED] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc269_fixup_hp_line1_mic1_led,
	},
	[ALC269_FIXUP_INV_DMIC] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_inv_dmic,
	},
	[ALC269_FIXUP_NO_SHUTUP] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_no_shutup,
	},
	[ALC269_FIXUP_LENOVO_DOCK] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			{ 0x19, 0x23a11040 }, /* dock mic */
			{ 0x1b, 0x2121103f }, /* dock headphone */
			{ }
		},
		.chained = true,
		.chain_id = ALC269_FIXUP_PINCFG_NO_HP_TO_LINEOUT
	},
<<<<<<< HEAD
	[ALC269_FIXUP_PINCFG_NO_HP_TO_LINEOUT] = {
		.type = ALC_FIXUP_FUNC,
		.v.func = alc269_fixup_pincfg_no_hp_to_lineout,
=======
	[ALC269_FIXUP_LENOVO_DOCK_LIMIT_BOOST] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc269_fixup_limit_int_mic_boost,
		.chained = true,
		.chain_id = ALC269_FIXUP_LENOVO_DOCK,
	},
	[ALC269_FIXUP_PINCFG_NO_HP_TO_LINEOUT] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc269_fixup_pincfg_no_hp_to_lineout,
		.chained = true,
		.chain_id = ALC269_FIXUP_THINKPAD_ACPI,
	},
	[ALC269_FIXUP_DELL1_MIC_NO_PRESENCE] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x19, 0x01a1913c }, /* use as headset mic, without its own jack detect */
			{ 0x1a, 0x01a1913d }, /* use as headphone mic, without its own jack detect */
			{ }
		},
		.chained = true,
		.chain_id = ALC269_FIXUP_HEADSET_MODE
	},
	[ALC269_FIXUP_DELL2_MIC_NO_PRESENCE] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x16, 0x21014020 }, /* dock line out */
			{ 0x19, 0x21a19030 }, /* dock mic */
			{ 0x1a, 0x01a1913c }, /* use as headset mic, without its own jack detect */
			{ }
		},
		.chained = true,
		.chain_id = ALC269_FIXUP_HEADSET_MODE_NO_HP_MIC
	},
	[ALC269_FIXUP_DELL3_MIC_NO_PRESENCE] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x1a, 0x01a1913c }, /* use as headset mic, without its own jack detect */
			{ }
		},
		.chained = true,
		.chain_id = ALC269_FIXUP_HEADSET_MODE_NO_HP_MIC
	},
	[ALC269_FIXUP_DELL4_MIC_NO_PRESENCE] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x19, 0x01a1913c }, /* use as headset mic, without its own jack detect */
			{ 0x1b, 0x01a1913d }, /* use as headphone mic, without its own jack detect */
			{ }
		},
		.chained = true,
		.chain_id = ALC269_FIXUP_HEADSET_MODE
	},
	[ALC269_FIXUP_HEADSET_MODE] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_headset_mode,
		.chained = true,
		.chain_id = ALC255_FIXUP_MIC_MUTE_LED
	},
	[ALC269_FIXUP_HEADSET_MODE_NO_HP_MIC] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_headset_mode_no_hp_mic,
	},
	[ALC269_FIXUP_ASPIRE_HEADSET_MIC] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x19, 0x01a1913c }, /* headset mic w/o jack detect */
			{ }
		},
		.chained = true,
		.chain_id = ALC269_FIXUP_HEADSET_MODE,
	},
	[ALC286_FIXUP_SONY_MIC_NO_PRESENCE] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x18, 0x01a1913c }, /* use as headset mic, without its own jack detect */
			{ }
		},
		.chained = true,
		.chain_id = ALC269_FIXUP_HEADSET_MIC
	},
	[ALC256_FIXUP_HUAWEI_MACH_WX9_PINS] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{0x12, 0x90a60130},
			{0x13, 0x40000000},
			{0x14, 0x90170110},
			{0x18, 0x411111f0},
			{0x19, 0x04a11040},
			{0x1a, 0x411111f0},
			{0x1b, 0x90170112},
			{0x1d, 0x40759a05},
			{0x1e, 0x411111f0},
			{0x21, 0x04211020},
			{ }
		},
		.chained = true,
		.chain_id = ALC255_FIXUP_MIC_MUTE_LED
	},
	[ALC298_FIXUP_HUAWEI_MBX_STEREO] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc298_fixup_huawei_mbx_stereo,
		.chained = true,
		.chain_id = ALC255_FIXUP_MIC_MUTE_LED
	},
	[ALC269_FIXUP_ASUS_X101_FUNC] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc269_fixup_x101_headset_mic,
	},
	[ALC269_FIXUP_ASUS_X101_VERB] = {
		.type = HDA_FIXUP_VERBS,
		.v.verbs = (const struct hda_verb[]) {
			{0x18, AC_VERB_SET_PIN_WIDGET_CONTROL, 0},
			{0x20, AC_VERB_SET_COEF_INDEX, 0x08},
			{0x20, AC_VERB_SET_PROC_COEF,  0x0310},
			{ }
		},
		.chained = true,
		.chain_id = ALC269_FIXUP_ASUS_X101_FUNC
	},
	[ALC269_FIXUP_ASUS_X101] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x18, 0x04a1182c }, /* Headset mic */
			{ }
		},
		.chained = true,
		.chain_id = ALC269_FIXUP_ASUS_X101_VERB
	},
	[ALC271_FIXUP_AMIC_MIC2] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x14, 0x99130110 }, /* speaker */
			{ 0x19, 0x01a19c20 }, /* mic */
			{ 0x1b, 0x99a7012f }, /* int-mic */
			{ 0x21, 0x0121401f }, /* HP out */
			{ }
		},
	},
	[ALC271_FIXUP_HP_GATE_MIC_JACK] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc271_hp_gate_mic_jack,
		.chained = true,
		.chain_id = ALC271_FIXUP_AMIC_MIC2,
	},
	[ALC271_FIXUP_HP_GATE_MIC_JACK_E1_572] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc269_fixup_limit_int_mic_boost,
		.chained = true,
		.chain_id = ALC271_FIXUP_HP_GATE_MIC_JACK,
	},
	[ALC269_FIXUP_ACER_AC700] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x12, 0x99a3092f }, /* int-mic */
			{ 0x14, 0x99130110 }, /* speaker */
			{ 0x18, 0x03a11c20 }, /* mic */
			{ 0x1e, 0x0346101e }, /* SPDIF1 */
			{ 0x21, 0x0321101f }, /* HP out */
			{ }
		},
		.chained = true,
		.chain_id = ALC271_FIXUP_DMIC,
	},
	[ALC269_FIXUP_LIMIT_INT_MIC_BOOST] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc269_fixup_limit_int_mic_boost,
		.chained = true,
		.chain_id = ALC269_FIXUP_THINKPAD_ACPI,
	},
	[ALC269VB_FIXUP_ASUS_ZENBOOK] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc269_fixup_limit_int_mic_boost,
		.chained = true,
		.chain_id = ALC269VB_FIXUP_DMIC,
	},
	[ALC269VB_FIXUP_ASUS_ZENBOOK_UX31A] = {
		.type = HDA_FIXUP_VERBS,
		.v.verbs = (const struct hda_verb[]) {
			/* class-D output amp +5dB */
			{ 0x20, AC_VERB_SET_COEF_INDEX, 0x12 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x2800 },
			{}
		},
		.chained = true,
		.chain_id = ALC269VB_FIXUP_ASUS_ZENBOOK,
	},
	[ALC269VB_FIXUP_ASUS_MIC_NO_PRESENCE] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x18, 0x01a110f0 },  /* use as headset mic */
			{ }
		},
		.chained = true,
		.chain_id = ALC269_FIXUP_HEADSET_MIC
	},
	[ALC269_FIXUP_LIMIT_INT_MIC_BOOST_MUTE_LED] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc269_fixup_limit_int_mic_boost,
		.chained = true,
		.chain_id = ALC269_FIXUP_HP_MUTE_LED_MIC1,
	},
	[ALC269VB_FIXUP_ORDISSIMO_EVE2] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x12, 0x99a3092f }, /* int-mic */
			{ 0x18, 0x03a11d20 }, /* mic */
			{ 0x19, 0x411111f0 }, /* Unused bogus pin */
			{ }
		},
	},
	[ALC283_FIXUP_CHROME_BOOK] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc283_fixup_chromebook,
	},
	[ALC283_FIXUP_SENSE_COMBO_JACK] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc283_fixup_sense_combo_jack,
		.chained = true,
		.chain_id = ALC283_FIXUP_CHROME_BOOK,
	},
	[ALC282_FIXUP_ASUS_TX300] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc282_fixup_asus_tx300,
	},
	[ALC283_FIXUP_INT_MIC] = {
		.type = HDA_FIXUP_VERBS,
		.v.verbs = (const struct hda_verb[]) {
			{0x20, AC_VERB_SET_COEF_INDEX, 0x1a},
			{0x20, AC_VERB_SET_PROC_COEF, 0x0011},
			{ }
		},
		.chained = true,
		.chain_id = ALC269_FIXUP_LIMIT_INT_MIC_BOOST
	},
	[ALC290_FIXUP_SUBWOOFER_HSJACK] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x17, 0x90170112 }, /* subwoofer */
			{ }
		},
		.chained = true,
		.chain_id = ALC290_FIXUP_MONO_SPEAKERS_HSJACK,
	},
	[ALC290_FIXUP_SUBWOOFER] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x17, 0x90170112 }, /* subwoofer */
			{ }
		},
		.chained = true,
		.chain_id = ALC290_FIXUP_MONO_SPEAKERS,
	},
	[ALC290_FIXUP_MONO_SPEAKERS] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc290_fixup_mono_speakers,
	},
	[ALC290_FIXUP_MONO_SPEAKERS_HSJACK] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc290_fixup_mono_speakers,
		.chained = true,
		.chain_id = ALC269_FIXUP_DELL3_MIC_NO_PRESENCE,
	},
	[ALC269_FIXUP_THINKPAD_ACPI] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_thinkpad_acpi,
		.chained = true,
		.chain_id = ALC269_FIXUP_SKU_IGNORE,
	},
	[ALC269_FIXUP_DMIC_THINKPAD_ACPI] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_inv_dmic,
		.chained = true,
		.chain_id = ALC269_FIXUP_THINKPAD_ACPI,
	},
	[ALC255_FIXUP_ACER_MIC_NO_PRESENCE] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x19, 0x01a1913c }, /* use as headset mic, without its own jack detect */
			{ }
		},
		.chained = true,
		.chain_id = ALC255_FIXUP_HEADSET_MODE
	},
	[ALC255_FIXUP_ASUS_MIC_NO_PRESENCE] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x19, 0x01a1913c }, /* use as headset mic, without its own jack detect */
			{ }
		},
		.chained = true,
		.chain_id = ALC255_FIXUP_HEADSET_MODE
	},
	[ALC255_FIXUP_DELL1_MIC_NO_PRESENCE] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x19, 0x01a1913c }, /* use as headset mic, without its own jack detect */
			{ 0x1a, 0x01a1913d }, /* use as headphone mic, without its own jack detect */
			{ }
		},
		.chained = true,
		.chain_id = ALC255_FIXUP_HEADSET_MODE
	},
	[ALC255_FIXUP_DELL2_MIC_NO_PRESENCE] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x19, 0x01a1913c }, /* use as headset mic, without its own jack detect */
			{ }
		},
		.chained = true,
		.chain_id = ALC255_FIXUP_HEADSET_MODE_NO_HP_MIC
	},
	[ALC255_FIXUP_HEADSET_MODE] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_headset_mode_alc255,
		.chained = true,
		.chain_id = ALC255_FIXUP_MIC_MUTE_LED
	},
	[ALC255_FIXUP_HEADSET_MODE_NO_HP_MIC] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_headset_mode_alc255_no_hp_mic,
	},
	[ALC293_FIXUP_DELL1_MIC_NO_PRESENCE] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x18, 0x01a1913d }, /* use as headphone mic, without its own jack detect */
			{ 0x1a, 0x01a1913c }, /* use as headset mic, without its own jack detect */
			{ }
		},
		.chained = true,
		.chain_id = ALC269_FIXUP_HEADSET_MODE
	},
	[ALC292_FIXUP_TPT440_DOCK] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_tpt440_dock,
		.chained = true,
		.chain_id = ALC269_FIXUP_LIMIT_INT_MIC_BOOST
	},
	[ALC292_FIXUP_TPT440] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_disable_aamix,
		.chained = true,
		.chain_id = ALC292_FIXUP_TPT440_DOCK,
	},
	[ALC283_FIXUP_HEADSET_MIC] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x19, 0x04a110f0 },
			{ },
		},
	},
	[ALC255_FIXUP_MIC_MUTE_LED] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_micmute_led,
	},
	[ALC282_FIXUP_ASPIRE_V5_PINS] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x12, 0x90a60130 },
			{ 0x14, 0x90170110 },
			{ 0x17, 0x40000008 },
			{ 0x18, 0x411111f0 },
			{ 0x19, 0x01a1913c },
			{ 0x1a, 0x411111f0 },
			{ 0x1b, 0x411111f0 },
			{ 0x1d, 0x40f89b2d },
			{ 0x1e, 0x411111f0 },
			{ 0x21, 0x0321101f },
			{ },
		},
	},
	[ALC269VB_FIXUP_ASPIRE_E1_COEF] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc269vb_fixup_aspire_e1_coef,
	},
	[ALC280_FIXUP_HP_GPIO4] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc280_fixup_hp_gpio4,
	},
	[ALC286_FIXUP_HP_GPIO_LED] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc286_fixup_hp_gpio_led,
	},
	[ALC280_FIXUP_HP_GPIO2_MIC_HOTKEY] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc280_fixup_hp_gpio2_mic_hotkey,
	},
	[ALC280_FIXUP_HP_DOCK_PINS] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x1b, 0x21011020 }, /* line-out */
			{ 0x1a, 0x01a1903c }, /* headset mic */
			{ 0x18, 0x2181103f }, /* line-in */
			{ },
		},
		.chained = true,
		.chain_id = ALC280_FIXUP_HP_GPIO4
	},
	[ALC269_FIXUP_HP_DOCK_GPIO_MIC1_LED] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x1b, 0x21011020 }, /* line-out */
			{ 0x18, 0x2181103f }, /* line-in */
			{ },
		},
		.chained = true,
		.chain_id = ALC269_FIXUP_HP_GPIO_MIC1_LED
	},
	[ALC280_FIXUP_HP_9480M] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc280_fixup_hp_9480m,
	},
	[ALC245_FIXUP_HP_X360_AMP] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc245_fixup_hp_x360_amp,
		.chained = true,
		.chain_id = ALC245_FIXUP_HP_GPIO_LED
	},
	[ALC288_FIXUP_DELL_HEADSET_MODE] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_headset_mode_dell_alc288,
		.chained = true,
		.chain_id = ALC255_FIXUP_MIC_MUTE_LED
	},
	[ALC288_FIXUP_DELL1_MIC_NO_PRESENCE] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x18, 0x01a1913c }, /* use as headset mic, without its own jack detect */
			{ 0x1a, 0x01a1913d }, /* use as headphone mic, without its own jack detect */
			{ }
		},
		.chained = true,
		.chain_id = ALC288_FIXUP_DELL_HEADSET_MODE
	},
	[ALC288_FIXUP_DISABLE_AAMIX] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_disable_aamix,
		.chained = true,
		.chain_id = ALC288_FIXUP_DELL1_MIC_NO_PRESENCE
	},
	[ALC288_FIXUP_DELL_XPS_13] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_dell_xps13,
		.chained = true,
		.chain_id = ALC288_FIXUP_DISABLE_AAMIX
	},
	[ALC292_FIXUP_DISABLE_AAMIX] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_disable_aamix,
		.chained = true,
		.chain_id = ALC269_FIXUP_DELL2_MIC_NO_PRESENCE
	},
	[ALC293_FIXUP_DISABLE_AAMIX_MULTIJACK] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_disable_aamix,
		.chained = true,
		.chain_id = ALC293_FIXUP_DELL1_MIC_NO_PRESENCE
	},
	[ALC292_FIXUP_DELL_E7X_AAMIX] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_dell_xps13,
		.chained = true,
		.chain_id = ALC292_FIXUP_DISABLE_AAMIX
	},
	[ALC292_FIXUP_DELL_E7X] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_micmute_led,
		/* micmute fixup must be applied at last */
		.chained_before = true,
		.chain_id = ALC292_FIXUP_DELL_E7X_AAMIX,
	},
	[ALC298_FIXUP_ALIENWARE_MIC_NO_PRESENCE] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x18, 0x01a1913c }, /* headset mic w/o jack detect */
			{ }
		},
		.chained_before = true,
		.chain_id = ALC269_FIXUP_HEADSET_MODE,
	},
	[ALC298_FIXUP_DELL1_MIC_NO_PRESENCE] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x18, 0x01a1913c }, /* use as headset mic, without its own jack detect */
			{ 0x1a, 0x01a1913d }, /* use as headphone mic, without its own jack detect */
			{ }
		},
		.chained = true,
		.chain_id = ALC269_FIXUP_HEADSET_MODE
	},
	[ALC298_FIXUP_DELL_AIO_MIC_NO_PRESENCE] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x18, 0x01a1913c }, /* use as headset mic, without its own jack detect */
			{ }
		},
		.chained = true,
		.chain_id = ALC269_FIXUP_HEADSET_MODE
	},
	[ALC275_FIXUP_DELL_XPS] = {
		.type = HDA_FIXUP_VERBS,
		.v.verbs = (const struct hda_verb[]) {
			/* Enables internal speaker */
			{0x20, AC_VERB_SET_COEF_INDEX, 0x1f},
			{0x20, AC_VERB_SET_PROC_COEF, 0x00c0},
			{0x20, AC_VERB_SET_COEF_INDEX, 0x30},
			{0x20, AC_VERB_SET_PROC_COEF, 0x00b1},
			{}
		}
	},
	[ALC293_FIXUP_LENOVO_SPK_NOISE] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_disable_aamix,
		.chained = true,
		.chain_id = ALC269_FIXUP_THINKPAD_ACPI
	},
	[ALC233_FIXUP_LENOVO_LINE2_MIC_HOTKEY] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc233_fixup_lenovo_line2_mic_hotkey,
	},
	[ALC233_FIXUP_INTEL_NUC8_DMIC] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_inv_dmic,
		.chained = true,
		.chain_id = ALC233_FIXUP_INTEL_NUC8_BOOST,
	},
	[ALC233_FIXUP_INTEL_NUC8_BOOST] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc269_fixup_limit_int_mic_boost
	},
	[ALC255_FIXUP_DELL_SPK_NOISE] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_disable_aamix,
		.chained = true,
		.chain_id = ALC255_FIXUP_DELL1_MIC_NO_PRESENCE
	},
	[ALC225_FIXUP_DISABLE_MIC_VREF] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_disable_mic_vref,
		.chained = true,
		.chain_id = ALC269_FIXUP_DELL1_MIC_NO_PRESENCE
	},
	[ALC225_FIXUP_DELL1_MIC_NO_PRESENCE] = {
		.type = HDA_FIXUP_VERBS,
		.v.verbs = (const struct hda_verb[]) {
			/* Disable pass-through path for FRONT 14h */
			{ 0x20, AC_VERB_SET_COEF_INDEX, 0x36 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x57d7 },
			{}
		},
		.chained = true,
		.chain_id = ALC225_FIXUP_DISABLE_MIC_VREF
	},
	[ALC280_FIXUP_HP_HEADSET_MIC] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_disable_aamix,
		.chained = true,
		.chain_id = ALC269_FIXUP_HEADSET_MIC,
	},
	[ALC221_FIXUP_HP_FRONT_MIC] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x19, 0x02a19020 }, /* Front Mic */
			{ }
		},
	},
	[ALC292_FIXUP_TPT460] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_tpt440_dock,
		.chained = true,
		.chain_id = ALC293_FIXUP_LENOVO_SPK_NOISE,
	},
	[ALC298_FIXUP_SPK_VOLUME] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc298_fixup_speaker_volume,
		.chained = true,
		.chain_id = ALC298_FIXUP_DELL_AIO_MIC_NO_PRESENCE,
	},
	[ALC298_FIXUP_LENOVO_SPK_VOLUME] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc298_fixup_speaker_volume,
	},
	[ALC295_FIXUP_DISABLE_DAC3] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc295_fixup_disable_dac3,
	},
	[ALC285_FIXUP_SPEAKER2_TO_DAC1] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc285_fixup_speaker2_to_dac1,
		.chained = true,
		.chain_id = ALC269_FIXUP_THINKPAD_ACPI
	},
	[ALC285_FIXUP_ASUS_SPEAKER2_TO_DAC1] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc285_fixup_speaker2_to_dac1,
		.chained = true,
		.chain_id = ALC245_FIXUP_CS35L41_SPI_2
	},
	[ALC285_FIXUP_ASUS_HEADSET_MIC] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x19, 0x03a11050 },
			{ 0x1b, 0x03a11c30 },
			{ }
		},
		.chained = true,
		.chain_id = ALC285_FIXUP_ASUS_SPEAKER2_TO_DAC1
	},
	[ALC285_FIXUP_ASUS_SPI_REAR_SPEAKERS] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x14, 0x90170120 },
			{ }
		},
		.chained = true,
		.chain_id = ALC285_FIXUP_ASUS_HEADSET_MIC
	},
	[ALC285_FIXUP_ASUS_I2C_SPEAKER2_TO_DAC1] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc285_fixup_speaker2_to_dac1,
		.chained = true,
		.chain_id = ALC287_FIXUP_CS35L41_I2C_2
	},
	[ALC285_FIXUP_ASUS_I2C_HEADSET_MIC] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x19, 0x03a11050 },
			{ 0x1b, 0x03a11c30 },
			{ }
		},
		.chained = true,
		.chain_id = ALC285_FIXUP_ASUS_I2C_SPEAKER2_TO_DAC1
	},
	[ALC256_FIXUP_DELL_INSPIRON_7559_SUBWOOFER] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x1b, 0x90170151 },
			{ }
		},
		.chained = true,
		.chain_id = ALC255_FIXUP_DELL1_MIC_NO_PRESENCE
	},
	[ALC269_FIXUP_ATIV_BOOK_8] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_auto_mute_via_amp,
		.chained = true,
		.chain_id = ALC269_FIXUP_NO_SHUTUP
	},
	[ALC221_FIXUP_HP_288PRO_MIC_NO_PRESENCE] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x19, 0x01a1913c }, /* use as headset mic, without its own jack detect */
			{ 0x1a, 0x01813030 }, /* use as headphone mic, without its own jack detect */
			{ }
		},
		.chained = true,
		.chain_id = ALC269_FIXUP_HEADSET_MODE
	},
	[ALC221_FIXUP_HP_MIC_NO_PRESENCE] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x18, 0x01a1913c }, /* use as headset mic, without its own jack detect */
			{ 0x1a, 0x01a1913d }, /* use as headphone mic, without its own jack detect */
			{ }
		},
		.chained = true,
		.chain_id = ALC269_FIXUP_HEADSET_MODE
	},
	[ALC256_FIXUP_ASUS_HEADSET_MODE] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_headset_mode,
	},
	[ALC256_FIXUP_ASUS_MIC] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x13, 0x90a60160 }, /* use as internal mic */
			{ 0x19, 0x04a11120 }, /* use as headset mic, without its own jack detect */
			{ }
		},
		.chained = true,
		.chain_id = ALC256_FIXUP_ASUS_HEADSET_MODE
	},
	[ALC256_FIXUP_ASUS_AIO_GPIO2] = {
		.type = HDA_FIXUP_FUNC,
		/* Set up GPIO2 for the speaker amp */
		.v.func = alc_fixup_gpio4,
	},
	[ALC233_FIXUP_ASUS_MIC_NO_PRESENCE] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x19, 0x01a1913c }, /* use as headset mic, without its own jack detect */
			{ }
		},
		.chained = true,
		.chain_id = ALC269_FIXUP_HEADSET_MIC
	},
	[ALC233_FIXUP_EAPD_COEF_AND_MIC_NO_PRESENCE] = {
		.type = HDA_FIXUP_VERBS,
		.v.verbs = (const struct hda_verb[]) {
			/* Enables internal speaker */
			{0x20, AC_VERB_SET_COEF_INDEX, 0x40},
			{0x20, AC_VERB_SET_PROC_COEF, 0x8800},
			{}
		},
		.chained = true,
		.chain_id = ALC233_FIXUP_ASUS_MIC_NO_PRESENCE
	},
	[ALC233_FIXUP_LENOVO_MULTI_CODECS] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc233_alc662_fixup_lenovo_dual_codecs,
		.chained = true,
		.chain_id = ALC269_FIXUP_GPIO2
	},
	[ALC233_FIXUP_ACER_HEADSET_MIC] = {
		.type = HDA_FIXUP_VERBS,
		.v.verbs = (const struct hda_verb[]) {
			{ 0x20, AC_VERB_SET_COEF_INDEX, 0x45 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x5089 },
			{ }
		},
		.chained = true,
		.chain_id = ALC233_FIXUP_ASUS_MIC_NO_PRESENCE
	},
	[ALC294_FIXUP_LENOVO_MIC_LOCATION] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			/* Change the mic location from front to right, otherwise there are
			   two front mics with the same name, pulseaudio can't handle them.
			   This is just a temporary workaround, after applying this fixup,
			   there will be one "Front Mic" and one "Mic" in this machine.
			 */
			{ 0x1a, 0x04a19040 },
			{ }
		},
	},
	[ALC225_FIXUP_DELL_WYSE_MIC_NO_PRESENCE] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x16, 0x0101102f }, /* Rear Headset HP */
			{ 0x19, 0x02a1913c }, /* use as Front headset mic, without its own jack detect */
			{ 0x1a, 0x01a19030 }, /* Rear Headset MIC */
			{ 0x1b, 0x02011020 },
			{ }
		},
		.chained = true,
		.chain_id = ALC225_FIXUP_S3_POP_NOISE
	},
	[ALC225_FIXUP_S3_POP_NOISE] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc225_fixup_s3_pop_noise,
		.chained = true,
		.chain_id = ALC269_FIXUP_HEADSET_MODE_NO_HP_MIC
	},
	[ALC700_FIXUP_INTEL_REFERENCE] = {
		.type = HDA_FIXUP_VERBS,
		.v.verbs = (const struct hda_verb[]) {
			/* Enables internal speaker */
			{0x20, AC_VERB_SET_COEF_INDEX, 0x45},
			{0x20, AC_VERB_SET_PROC_COEF, 0x5289},
			{0x20, AC_VERB_SET_COEF_INDEX, 0x4A},
			{0x20, AC_VERB_SET_PROC_COEF, 0x001b},
			{0x58, AC_VERB_SET_COEF_INDEX, 0x00},
			{0x58, AC_VERB_SET_PROC_COEF, 0x3888},
			{0x20, AC_VERB_SET_COEF_INDEX, 0x6f},
			{0x20, AC_VERB_SET_PROC_COEF, 0x2c0b},
			{}
		}
	},
	[ALC274_FIXUP_DELL_BIND_DACS] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc274_fixup_bind_dacs,
		.chained = true,
		.chain_id = ALC269_FIXUP_DELL1_MIC_NO_PRESENCE
	},
	[ALC274_FIXUP_DELL_AIO_LINEOUT_VERB] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x1b, 0x0401102f },
			{ }
		},
		.chained = true,
		.chain_id = ALC274_FIXUP_DELL_BIND_DACS
	},
	[ALC298_FIXUP_TPT470_DOCK_FIX] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_tpt470_dock,
		.chained = true,
		.chain_id = ALC293_FIXUP_LENOVO_SPK_NOISE
	},
	[ALC298_FIXUP_TPT470_DOCK] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_tpt470_dacs,
		.chained = true,
		.chain_id = ALC298_FIXUP_TPT470_DOCK_FIX
	},
	[ALC255_FIXUP_DUMMY_LINEOUT_VERB] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x14, 0x0201101f },
			{ }
		},
		.chained = true,
		.chain_id = ALC255_FIXUP_DELL1_MIC_NO_PRESENCE
	},
	[ALC255_FIXUP_DELL_HEADSET_MIC] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x19, 0x01a1913c }, /* use as headset mic, without its own jack detect */
			{ }
		},
		.chained = true,
		.chain_id = ALC269_FIXUP_HEADSET_MIC
	},
	[ALC295_FIXUP_HP_X360] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc295_fixup_hp_top_speakers,
		.chained = true,
		.chain_id = ALC269_FIXUP_HP_MUTE_LED_MIC3
	},
	[ALC221_FIXUP_HP_HEADSET_MIC] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x19, 0x0181313f},
			{ }
		},
		.chained = true,
		.chain_id = ALC269_FIXUP_HEADSET_MIC
	},
	[ALC285_FIXUP_LENOVO_HEADPHONE_NOISE] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc285_fixup_invalidate_dacs,
		.chained = true,
		.chain_id = ALC269_FIXUP_THINKPAD_ACPI
	},
	[ALC295_FIXUP_HP_AUTO_MUTE] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_auto_mute_via_amp,
	},
	[ALC286_FIXUP_ACER_AIO_MIC_NO_PRESENCE] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x18, 0x01a1913c }, /* use as headset mic, without its own jack detect */
			{ }
		},
		.chained = true,
		.chain_id = ALC269_FIXUP_HEADSET_MIC
	},
	[ALC294_FIXUP_ASUS_MIC] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x13, 0x90a60160 }, /* use as internal mic */
			{ 0x19, 0x04a11120 }, /* use as headset mic, without its own jack detect */
			{ }
		},
		.chained = true,
		.chain_id = ALC269_FIXUP_HEADSET_MIC
	},
	[ALC294_FIXUP_ASUS_HEADSET_MIC] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x19, 0x01a1103c }, /* use as headset mic */
			{ }
		},
		.chained = true,
		.chain_id = ALC269_FIXUP_HEADSET_MIC
	},
	[ALC294_FIXUP_ASUS_SPK] = {
		.type = HDA_FIXUP_VERBS,
		.v.verbs = (const struct hda_verb[]) {
			/* Set EAPD high */
			{ 0x20, AC_VERB_SET_COEF_INDEX, 0x40 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x8800 },
			{ 0x20, AC_VERB_SET_COEF_INDEX, 0x0f },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x7774 },
			{ }
		},
		.chained = true,
		.chain_id = ALC294_FIXUP_ASUS_HEADSET_MIC
	},
	[ALC295_FIXUP_CHROME_BOOK] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc295_fixup_chromebook,
		.chained = true,
		.chain_id = ALC225_FIXUP_HEADSET_JACK
	},
	[ALC225_FIXUP_HEADSET_JACK] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_headset_jack,
	},
	[ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x1a, 0x01a1913c }, /* use as headset mic, without its own jack detect */
			{ }
		},
		.chained = true,
		.chain_id = ALC269_FIXUP_HEADSET_MODE_NO_HP_MIC
	},
	[ALC285_FIXUP_LENOVO_PC_BEEP_IN_NOISE] = {
		.type = HDA_FIXUP_VERBS,
		.v.verbs = (const struct hda_verb[]) {
			/* Disable PCBEEP-IN passthrough */
			{ 0x20, AC_VERB_SET_COEF_INDEX, 0x36 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x57d7 },
			{ }
		},
		.chained = true,
		.chain_id = ALC285_FIXUP_LENOVO_HEADPHONE_NOISE
	},
	[ALC255_FIXUP_ACER_HEADSET_MIC] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x19, 0x03a11130 },
			{ 0x1a, 0x90a60140 }, /* use as internal mic */
			{ }
		},
		.chained = true,
		.chain_id = ALC255_FIXUP_HEADSET_MODE_NO_HP_MIC
	},
	[ALC225_FIXUP_DELL_WYSE_AIO_MIC_NO_PRESENCE] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x16, 0x01011020 }, /* Rear Line out */
			{ 0x19, 0x01a1913c }, /* use as Front headset mic, without its own jack detect */
			{ }
		},
		.chained = true,
		.chain_id = ALC225_FIXUP_WYSE_AUTO_MUTE
	},
	[ALC225_FIXUP_WYSE_AUTO_MUTE] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_auto_mute_via_amp,
		.chained = true,
		.chain_id = ALC225_FIXUP_WYSE_DISABLE_MIC_VREF
	},
	[ALC225_FIXUP_WYSE_DISABLE_MIC_VREF] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_disable_mic_vref,
		.chained = true,
		.chain_id = ALC269_FIXUP_HEADSET_MODE_NO_HP_MIC
	},
	[ALC286_FIXUP_ACER_AIO_HEADSET_MIC] = {
		.type = HDA_FIXUP_VERBS,
		.v.verbs = (const struct hda_verb[]) {
			{ 0x20, AC_VERB_SET_COEF_INDEX, 0x4f },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x5029 },
			{ }
		},
		.chained = true,
		.chain_id = ALC286_FIXUP_ACER_AIO_MIC_NO_PRESENCE
	},
	[ALC256_FIXUP_ASUS_HEADSET_MIC] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x19, 0x03a11020 }, /* headset mic with jack detect */
			{ }
		},
		.chained = true,
		.chain_id = ALC256_FIXUP_ASUS_HEADSET_MODE
	},
	[ALC256_FIXUP_ASUS_MIC_NO_PRESENCE] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x19, 0x04a11120 }, /* use as headset mic, without its own jack detect */
			{ }
		},
		.chained = true,
		.chain_id = ALC256_FIXUP_ASUS_HEADSET_MODE
	},
	[ALC299_FIXUP_PREDATOR_SPK] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x21, 0x90170150 }, /* use as headset mic, without its own jack detect */
			{ }
		}
	},
	[ALC256_FIXUP_MEDION_HEADSET_NO_PRESENCE] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x19, 0x04a11040 },
			{ 0x21, 0x04211020 },
			{ }
		},
		.chained = true,
		.chain_id = ALC256_FIXUP_ASUS_HEADSET_MODE
	},
	[ALC289_FIXUP_DELL_SPK1] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x14, 0x90170140 },
			{ }
		},
		.chained = true,
		.chain_id = ALC269_FIXUP_DELL4_MIC_NO_PRESENCE
	},
	[ALC289_FIXUP_DELL_SPK2] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x17, 0x90170130 }, /* bass spk */
			{ }
		},
		.chained = true,
		.chain_id = ALC269_FIXUP_DELL4_MIC_NO_PRESENCE
	},
	[ALC289_FIXUP_DUAL_SPK] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc285_fixup_speaker2_to_dac1,
		.chained = true,
		.chain_id = ALC289_FIXUP_DELL_SPK2
	},
	[ALC289_FIXUP_RTK_AMP_DUAL_SPK] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc285_fixup_speaker2_to_dac1,
		.chained = true,
		.chain_id = ALC289_FIXUP_DELL_SPK1
	},
	[ALC294_FIXUP_SPK2_TO_DAC1] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc285_fixup_speaker2_to_dac1,
		.chained = true,
		.chain_id = ALC294_FIXUP_ASUS_HEADSET_MIC
	},
	[ALC294_FIXUP_ASUS_DUAL_SPK] = {
		.type = HDA_FIXUP_FUNC,
		/* The GPIO must be pulled to initialize the AMP */
		.v.func = alc_fixup_gpio4,
		.chained = true,
		.chain_id = ALC294_FIXUP_SPK2_TO_DAC1
	},
	[ALC294_FIXUP_ASUS_ALLY] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = cs35l41_fixup_i2c_two,
		.chained = true,
		.chain_id = ALC294_FIXUP_ASUS_ALLY_PINS
	},
	[ALC294_FIXUP_ASUS_ALLY_PINS] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x19, 0x03a11050 },
			{ 0x1a, 0x03a11c30 },
			{ 0x21, 0x03211420 },
			{ }
		},
		.chained = true,
		.chain_id = ALC294_FIXUP_ASUS_ALLY_VERBS
	},
	[ALC294_FIXUP_ASUS_ALLY_VERBS] = {
		.type = HDA_FIXUP_VERBS,
		.v.verbs = (const struct hda_verb[]) {
			{ 0x20, AC_VERB_SET_COEF_INDEX, 0x45 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x5089 },
			{ 0x20, AC_VERB_SET_COEF_INDEX, 0x46 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x0004 },
			{ 0x20, AC_VERB_SET_COEF_INDEX, 0x47 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0xa47a },
			{ 0x20, AC_VERB_SET_COEF_INDEX, 0x49 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x0049},
			{ 0x20, AC_VERB_SET_COEF_INDEX, 0x4a },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x201b },
			{ 0x20, AC_VERB_SET_COEF_INDEX, 0x6b },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x4278},
			{ }
		},
		.chained = true,
		.chain_id = ALC294_FIXUP_ASUS_ALLY_SPEAKER
	},
	[ALC294_FIXUP_ASUS_ALLY_SPEAKER] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc285_fixup_speaker2_to_dac1,
	},
	[ALC285_FIXUP_THINKPAD_X1_GEN7] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc285_fixup_thinkpad_x1_gen7,
		.chained = true,
		.chain_id = ALC269_FIXUP_THINKPAD_ACPI
	},
	[ALC285_FIXUP_THINKPAD_HEADSET_JACK] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_headset_jack,
		.chained = true,
		.chain_id = ALC285_FIXUP_THINKPAD_X1_GEN7
	},
	[ALC294_FIXUP_ASUS_HPE] = {
		.type = HDA_FIXUP_VERBS,
		.v.verbs = (const struct hda_verb[]) {
			/* Set EAPD high */
			{ 0x20, AC_VERB_SET_COEF_INDEX, 0x0f },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x7774 },
			{ }
		},
		.chained = true,
		.chain_id = ALC294_FIXUP_ASUS_HEADSET_MIC
	},
	[ALC294_FIXUP_ASUS_GX502_PINS] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x19, 0x03a11050 }, /* front HP mic */
			{ 0x1a, 0x01a11830 }, /* rear external mic */
			{ 0x21, 0x03211020 }, /* front HP out */
			{ }
		},
		.chained = true,
		.chain_id = ALC294_FIXUP_ASUS_GX502_VERBS
	},
	[ALC294_FIXUP_ASUS_GX502_VERBS] = {
		.type = HDA_FIXUP_VERBS,
		.v.verbs = (const struct hda_verb[]) {
			/* set 0x15 to HP-OUT ctrl */
			{ 0x15, AC_VERB_SET_PIN_WIDGET_CONTROL, 0xc0 },
			/* unmute the 0x15 amp */
			{ 0x15, AC_VERB_SET_AMP_GAIN_MUTE, 0xb000 },
			{ }
		},
		.chained = true,
		.chain_id = ALC294_FIXUP_ASUS_GX502_HP
	},
	[ALC294_FIXUP_ASUS_GX502_HP] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc294_fixup_gx502_hp,
	},
	[ALC294_FIXUP_ASUS_GU502_PINS] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x19, 0x01a11050 }, /* rear HP mic */
			{ 0x1a, 0x01a11830 }, /* rear external mic */
			{ 0x21, 0x012110f0 }, /* rear HP out */
			{ }
		},
		.chained = true,
		.chain_id = ALC294_FIXUP_ASUS_GU502_VERBS
	},
	[ALC294_FIXUP_ASUS_GU502_VERBS] = {
		.type = HDA_FIXUP_VERBS,
		.v.verbs = (const struct hda_verb[]) {
			/* set 0x15 to HP-OUT ctrl */
			{ 0x15, AC_VERB_SET_PIN_WIDGET_CONTROL, 0xc0 },
			/* unmute the 0x15 amp */
			{ 0x15, AC_VERB_SET_AMP_GAIN_MUTE, 0xb000 },
			/* set 0x1b to HP-OUT */
			{ 0x1b, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x24 },
			{ }
		},
		.chained = true,
		.chain_id = ALC294_FIXUP_ASUS_GU502_HP
	},
	[ALC294_FIXUP_ASUS_GU502_HP] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc294_fixup_gu502_hp,
	},
	 [ALC294_FIXUP_ASUS_G513_PINS] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
				{ 0x19, 0x03a11050 }, /* front HP mic */
				{ 0x1a, 0x03a11c30 }, /* rear external mic */
				{ 0x21, 0x03211420 }, /* front HP out */
				{ }
		},
	},
	[ALC285_FIXUP_ASUS_G533Z_PINS] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x14, 0x90170152 }, /* Speaker Surround Playback Switch */
			{ 0x19, 0x03a19020 }, /* Mic Boost Volume */
			{ 0x1a, 0x03a11c30 }, /* Mic Boost Volume */
			{ 0x1e, 0x90170151 }, /* Rear jack, IN OUT EAPD Detect */
			{ 0x21, 0x03211420 },
			{ }
		},
	},
	[ALC294_FIXUP_ASUS_COEF_1B] = {
		.type = HDA_FIXUP_VERBS,
		.v.verbs = (const struct hda_verb[]) {
			/* Set bit 10 to correct noisy output after reboot from
			 * Windows 10 (due to pop noise reduction?)
			 */
			{ 0x20, AC_VERB_SET_COEF_INDEX, 0x1b },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x4e4b },
			{ }
		},
		.chained = true,
		.chain_id = ALC289_FIXUP_ASUS_GA401,
	},
	[ALC285_FIXUP_HP_GPIO_LED] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc285_fixup_hp_gpio_led,
	},
	[ALC285_FIXUP_HP_MUTE_LED] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc285_fixup_hp_mute_led,
	},
	[ALC285_FIXUP_HP_SPECTRE_X360_MUTE_LED] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc285_fixup_hp_spectre_x360_mute_led,
	},
	[ALC236_FIXUP_HP_MUTE_LED_COEFBIT2] = {
	    .type = HDA_FIXUP_FUNC,
	    .v.func = alc236_fixup_hp_mute_led_coefbit2,
	},
	[ALC236_FIXUP_HP_GPIO_LED] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc236_fixup_hp_gpio_led,
	},
	[ALC236_FIXUP_HP_MUTE_LED] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc236_fixup_hp_mute_led,
	},
	[ALC236_FIXUP_HP_MUTE_LED_MICMUTE_VREF] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc236_fixup_hp_mute_led_micmute_vref,
	},
	[ALC298_FIXUP_SAMSUNG_AMP] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc298_fixup_samsung_amp,
		.chained = true,
		.chain_id = ALC298_FIXUP_SAMSUNG_HEADPHONE_VERY_QUIET
	},
	[ALC298_FIXUP_SAMSUNG_HEADPHONE_VERY_QUIET] = {
		.type = HDA_FIXUP_VERBS,
		.v.verbs = (const struct hda_verb[]) {
			{ 0x1a, AC_VERB_SET_PIN_WIDGET_CONTROL, 0xc5 },
			{ }
		},
	},
	[ALC256_FIXUP_SAMSUNG_HEADPHONE_VERY_QUIET] = {
		.type = HDA_FIXUP_VERBS,
		.v.verbs = (const struct hda_verb[]) {
			{ 0x20, AC_VERB_SET_COEF_INDEX, 0x08},
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x2fcf},
			{ }
		},
	},
	[ALC295_FIXUP_ASUS_MIC_NO_PRESENCE] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x19, 0x01a1913c }, /* use as headset mic, without its own jack detect */
			{ }
		},
		.chained = true,
		.chain_id = ALC269_FIXUP_HEADSET_MODE
	},
	[ALC269VC_FIXUP_ACER_VCOPPERBOX_PINS] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x14, 0x90100120 }, /* use as internal speaker */
			{ 0x18, 0x02a111f0 }, /* use as headset mic, without its own jack detect */
			{ 0x1a, 0x01011020 }, /* use as line out */
			{ },
		},
		.chained = true,
		.chain_id = ALC269_FIXUP_HEADSET_MIC
	},
	[ALC269VC_FIXUP_ACER_HEADSET_MIC] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x18, 0x02a11030 }, /* use as headset mic */
			{ }
		},
		.chained = true,
		.chain_id = ALC269_FIXUP_HEADSET_MIC
	},
	[ALC269VC_FIXUP_ACER_MIC_NO_PRESENCE] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x18, 0x01a11130 }, /* use as headset mic, without its own jack detect */
			{ }
		},
		.chained = true,
		.chain_id = ALC269_FIXUP_HEADSET_MIC
	},
	[ALC289_FIXUP_ASUS_GA401] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc289_fixup_asus_ga401,
		.chained = true,
		.chain_id = ALC289_FIXUP_ASUS_GA502,
	},
	[ALC289_FIXUP_ASUS_GA502] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x19, 0x03a11020 }, /* headset mic with jack detect */
			{ }
		},
	},
	[ALC256_FIXUP_ACER_MIC_NO_PRESENCE] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x19, 0x02a11120 }, /* use as headset mic, without its own jack detect */
			{ }
		},
		.chained = true,
		.chain_id = ALC256_FIXUP_ASUS_HEADSET_MODE
	},
	[ALC285_FIXUP_HP_GPIO_AMP_INIT] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc285_fixup_hp_gpio_amp_init,
		.chained = true,
		.chain_id = ALC285_FIXUP_HP_GPIO_LED
	},
	[ALC269_FIXUP_CZC_B20] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x12, 0x411111f0 },
			{ 0x14, 0x90170110 }, /* speaker */
			{ 0x15, 0x032f1020 }, /* HP out */
			{ 0x17, 0x411111f0 },
			{ 0x18, 0x03ab1040 }, /* mic */
			{ 0x19, 0xb7a7013f },
			{ 0x1a, 0x0181305f },
			{ 0x1b, 0x411111f0 },
			{ 0x1d, 0x411111f0 },
			{ 0x1e, 0x411111f0 },
			{ }
		},
		.chain_id = ALC269_FIXUP_DMIC,
	},
	[ALC269_FIXUP_CZC_TMI] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x12, 0x4000c000 },
			{ 0x14, 0x90170110 }, /* speaker */
			{ 0x15, 0x0421401f }, /* HP out */
			{ 0x17, 0x411111f0 },
			{ 0x18, 0x04a19020 }, /* mic */
			{ 0x19, 0x411111f0 },
			{ 0x1a, 0x411111f0 },
			{ 0x1b, 0x411111f0 },
			{ 0x1d, 0x40448505 },
			{ 0x1e, 0x411111f0 },
			{ 0x20, 0x8000ffff },
			{ }
		},
		.chain_id = ALC269_FIXUP_DMIC,
	},
	[ALC269_FIXUP_CZC_L101] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x12, 0x40000000 },
			{ 0x14, 0x01014010 }, /* speaker */
			{ 0x15, 0x411111f0 }, /* HP out */
			{ 0x16, 0x411111f0 },
			{ 0x18, 0x01a19020 }, /* mic */
			{ 0x19, 0x02a19021 },
			{ 0x1a, 0x0181302f },
			{ 0x1b, 0x0221401f },
			{ 0x1c, 0x411111f0 },
			{ 0x1d, 0x4044c601 },
			{ 0x1e, 0x411111f0 },
			{ }
		},
		.chain_id = ALC269_FIXUP_DMIC,
	},
	[ALC269_FIXUP_LEMOTE_A1802] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x12, 0x40000000 },
			{ 0x14, 0x90170110 }, /* speaker */
			{ 0x17, 0x411111f0 },
			{ 0x18, 0x03a19040 }, /* mic1 */
			{ 0x19, 0x90a70130 }, /* mic2 */
			{ 0x1a, 0x411111f0 },
			{ 0x1b, 0x411111f0 },
			{ 0x1d, 0x40489d2d },
			{ 0x1e, 0x411111f0 },
			{ 0x20, 0x0003ffff },
			{ 0x21, 0x03214020 },
			{ }
		},
		.chain_id = ALC269_FIXUP_DMIC,
	},
	[ALC269_FIXUP_LEMOTE_A190X] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x14, 0x99130110 }, /* speaker */
			{ 0x15, 0x0121401f }, /* HP out */
			{ 0x18, 0x01a19c20 }, /* rear  mic */
			{ 0x19, 0x99a3092f }, /* front mic */
			{ 0x1b, 0x0201401f }, /* front lineout */
			{ }
		},
		.chain_id = ALC269_FIXUP_DMIC,
	},
	[ALC256_FIXUP_INTEL_NUC8_RUGGED] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x1b, 0x01a1913c }, /* use as headset mic, without its own jack detect */
			{ }
		},
		.chained = true,
		.chain_id = ALC269_FIXUP_HEADSET_MODE
	},
	[ALC256_FIXUP_INTEL_NUC10] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x19, 0x01a1913c }, /* use as headset mic, without its own jack detect */
			{ }
		},
		.chained = true,
		.chain_id = ALC269_FIXUP_HEADSET_MODE
	},
	[ALC255_FIXUP_XIAOMI_HEADSET_MIC] = {
		.type = HDA_FIXUP_VERBS,
		.v.verbs = (const struct hda_verb[]) {
			{ 0x20, AC_VERB_SET_COEF_INDEX, 0x45 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x5089 },
			{ }
		},
		.chained = true,
		.chain_id = ALC289_FIXUP_ASUS_GA502
	},
	[ALC274_FIXUP_HP_MIC] = {
		.type = HDA_FIXUP_VERBS,
		.v.verbs = (const struct hda_verb[]) {
			{ 0x20, AC_VERB_SET_COEF_INDEX, 0x45 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x5089 },
			{ }
		},
	},
	[ALC274_FIXUP_HP_HEADSET_MIC] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc274_fixup_hp_headset_mic,
		.chained = true,
		.chain_id = ALC274_FIXUP_HP_MIC
	},
	[ALC274_FIXUP_HP_ENVY_GPIO] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc274_fixup_hp_envy_gpio,
	},
	[ALC256_FIXUP_ASUS_HPE] = {
		.type = HDA_FIXUP_VERBS,
		.v.verbs = (const struct hda_verb[]) {
			/* Set EAPD high */
			{ 0x20, AC_VERB_SET_COEF_INDEX, 0x0f },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x7778 },
			{ }
		},
		.chained = true,
		.chain_id = ALC294_FIXUP_ASUS_HEADSET_MIC
	},
	[ALC285_FIXUP_THINKPAD_NO_BASS_SPK_HEADSET_JACK] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_headset_jack,
		.chained = true,
		.chain_id = ALC269_FIXUP_THINKPAD_ACPI
	},
	[ALC287_FIXUP_HP_GPIO_LED] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc287_fixup_hp_gpio_led,
	},
	[ALC256_FIXUP_HP_HEADSET_MIC] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc274_fixup_hp_headset_mic,
	},
	[ALC236_FIXUP_DELL_AIO_HEADSET_MIC] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_no_int_mic,
		.chained = true,
		.chain_id = ALC255_FIXUP_DELL1_MIC_NO_PRESENCE
	},
	[ALC282_FIXUP_ACER_DISABLE_LINEOUT] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x1b, 0x411111f0 },
			{ 0x18, 0x01a1913c }, /* use as headset mic, without its own jack detect */
			{ },
		},
		.chained = true,
		.chain_id = ALC269_FIXUP_HEADSET_MODE
	},
	[ALC255_FIXUP_ACER_LIMIT_INT_MIC_BOOST] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc269_fixup_limit_int_mic_boost,
		.chained = true,
		.chain_id = ALC255_FIXUP_ACER_MIC_NO_PRESENCE,
	},
	[ALC256_FIXUP_ACER_HEADSET_MIC] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x19, 0x02a1113c }, /* use as headset mic, without its own jack detect */
			{ 0x1a, 0x90a1092f }, /* use as internal mic */
			{ }
		},
		.chained = true,
		.chain_id = ALC269_FIXUP_HEADSET_MODE_NO_HP_MIC
	},
	[ALC285_FIXUP_IDEAPAD_S740_COEF] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc285_fixup_ideapad_s740_coef,
		.chained = true,
		.chain_id = ALC269_FIXUP_THINKPAD_ACPI,
	},
	[ALC285_FIXUP_HP_LIMIT_INT_MIC_BOOST] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc269_fixup_limit_int_mic_boost,
		.chained = true,
		.chain_id = ALC285_FIXUP_HP_MUTE_LED,
	},
	[ALC295_FIXUP_ASUS_DACS] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc295_fixup_asus_dacs,
	},
	[ALC295_FIXUP_HP_OMEN] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x12, 0xb7a60130 },
			{ 0x13, 0x40000000 },
			{ 0x14, 0x411111f0 },
			{ 0x16, 0x411111f0 },
			{ 0x17, 0x90170110 },
			{ 0x18, 0x411111f0 },
			{ 0x19, 0x02a11030 },
			{ 0x1a, 0x411111f0 },
			{ 0x1b, 0x04a19030 },
			{ 0x1d, 0x40600001 },
			{ 0x1e, 0x411111f0 },
			{ 0x21, 0x03211020 },
			{}
		},
		.chained = true,
		.chain_id = ALC269_FIXUP_HP_LINE1_MIC1_LED,
	},
	[ALC285_FIXUP_HP_SPECTRE_X360] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc285_fixup_hp_spectre_x360,
	},
	[ALC285_FIXUP_HP_SPECTRE_X360_EB1] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc285_fixup_hp_spectre_x360_eb1
	},
	[ALC285_FIXUP_HP_ENVY_X360] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc285_fixup_hp_envy_x360,
		.chained = true,
		.chain_id = ALC285_FIXUP_HP_GPIO_AMP_INIT,
	},
	[ALC287_FIXUP_IDEAPAD_BASS_SPK_AMP] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc285_fixup_ideapad_s740_coef,
		.chained = true,
		.chain_id = ALC285_FIXUP_THINKPAD_HEADSET_JACK,
	},
	[ALC623_FIXUP_LENOVO_THINKSTATION_P340] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_no_shutup,
		.chained = true,
		.chain_id = ALC283_FIXUP_HEADSET_MIC,
	},
	[ALC255_FIXUP_ACER_HEADPHONE_AND_MIC] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x21, 0x03211030 }, /* Change the Headphone location to Left */
			{ }
		},
		.chained = true,
		.chain_id = ALC255_FIXUP_XIAOMI_HEADSET_MIC
	},
	[ALC236_FIXUP_HP_LIMIT_INT_MIC_BOOST] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc269_fixup_limit_int_mic_boost,
		.chained = true,
		.chain_id = ALC236_FIXUP_HP_MUTE_LED_MICMUTE_VREF,
	},
	[ALC285_FIXUP_LEGION_Y9000X_SPEAKERS] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc285_fixup_ideapad_s740_coef,
		.chained = true,
		.chain_id = ALC285_FIXUP_LEGION_Y9000X_AUTOMUTE,
	},
	[ALC285_FIXUP_LEGION_Y9000X_AUTOMUTE] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc287_fixup_legion_15imhg05_speakers,
		.chained = true,
		.chain_id = ALC269_FIXUP_THINKPAD_ACPI,
	},
	[ALC287_FIXUP_LEGION_15IMHG05_SPEAKERS] = {
		.type = HDA_FIXUP_VERBS,
		//.v.verbs = legion_15imhg05_coefs,
		.v.verbs = (const struct hda_verb[]) {
			 // set left speaker Legion 7i.
			 { 0x20, AC_VERB_SET_COEF_INDEX, 0x24 },
			 { 0x20, AC_VERB_SET_PROC_COEF, 0x41 },

			 { 0x20, AC_VERB_SET_COEF_INDEX, 0x26 },
			 { 0x20, AC_VERB_SET_PROC_COEF, 0xc },
			 { 0x20, AC_VERB_SET_PROC_COEF, 0x0 },
			 { 0x20, AC_VERB_SET_PROC_COEF, 0x1a },
			 { 0x20, AC_VERB_SET_PROC_COEF, 0xb020 },

			 { 0x20, AC_VERB_SET_COEF_INDEX, 0x26 },
			 { 0x20, AC_VERB_SET_PROC_COEF, 0x2 },
			 { 0x20, AC_VERB_SET_PROC_COEF, 0x0 },
			 { 0x20, AC_VERB_SET_PROC_COEF, 0x0 },
			 { 0x20, AC_VERB_SET_PROC_COEF, 0xb020 },

			 // set right speaker Legion 7i.
			 { 0x20, AC_VERB_SET_COEF_INDEX, 0x24 },
			 { 0x20, AC_VERB_SET_PROC_COEF, 0x42 },

			 { 0x20, AC_VERB_SET_COEF_INDEX, 0x26 },
			 { 0x20, AC_VERB_SET_PROC_COEF, 0xc },
			 { 0x20, AC_VERB_SET_PROC_COEF, 0x0 },
			 { 0x20, AC_VERB_SET_PROC_COEF, 0x2a },
			 { 0x20, AC_VERB_SET_PROC_COEF, 0xb020 },

			 { 0x20, AC_VERB_SET_COEF_INDEX, 0x26 },
			 { 0x20, AC_VERB_SET_PROC_COEF, 0x2 },
			 { 0x20, AC_VERB_SET_PROC_COEF, 0x0 },
			 { 0x20, AC_VERB_SET_PROC_COEF, 0x0 },
			 { 0x20, AC_VERB_SET_PROC_COEF, 0xb020 },
			 {}
		},
		.chained = true,
		.chain_id = ALC287_FIXUP_LEGION_15IMHG05_AUTOMUTE,
	},
	[ALC287_FIXUP_LEGION_15IMHG05_AUTOMUTE] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc287_fixup_legion_15imhg05_speakers,
		.chained = true,
		.chain_id = ALC269_FIXUP_HEADSET_MODE,
	},
	[ALC287_FIXUP_YOGA7_14ITL_SPEAKERS] = {
		.type = HDA_FIXUP_VERBS,
		.v.verbs = (const struct hda_verb[]) {
			 // set left speaker Yoga 7i.
			 { 0x20, AC_VERB_SET_COEF_INDEX, 0x24 },
			 { 0x20, AC_VERB_SET_PROC_COEF, 0x41 },

			 { 0x20, AC_VERB_SET_COEF_INDEX, 0x26 },
			 { 0x20, AC_VERB_SET_PROC_COEF, 0xc },
			 { 0x20, AC_VERB_SET_PROC_COEF, 0x0 },
			 { 0x20, AC_VERB_SET_PROC_COEF, 0x1a },
			 { 0x20, AC_VERB_SET_PROC_COEF, 0xb020 },

			 { 0x20, AC_VERB_SET_COEF_INDEX, 0x26 },
			 { 0x20, AC_VERB_SET_PROC_COEF, 0x2 },
			 { 0x20, AC_VERB_SET_PROC_COEF, 0x0 },
			 { 0x20, AC_VERB_SET_PROC_COEF, 0x0 },
			 { 0x20, AC_VERB_SET_PROC_COEF, 0xb020 },

			 // set right speaker Yoga 7i.
			 { 0x20, AC_VERB_SET_COEF_INDEX, 0x24 },
			 { 0x20, AC_VERB_SET_PROC_COEF, 0x46 },

			 { 0x20, AC_VERB_SET_COEF_INDEX, 0x26 },
			 { 0x20, AC_VERB_SET_PROC_COEF, 0xc },
			 { 0x20, AC_VERB_SET_PROC_COEF, 0x0 },
			 { 0x20, AC_VERB_SET_PROC_COEF, 0x2a },
			 { 0x20, AC_VERB_SET_PROC_COEF, 0xb020 },

			 { 0x20, AC_VERB_SET_COEF_INDEX, 0x26 },
			 { 0x20, AC_VERB_SET_PROC_COEF, 0x2 },
			 { 0x20, AC_VERB_SET_PROC_COEF, 0x0 },
			 { 0x20, AC_VERB_SET_PROC_COEF, 0x0 },
			 { 0x20, AC_VERB_SET_PROC_COEF, 0xb020 },
			 {}
		},
		.chained = true,
		.chain_id = ALC269_FIXUP_HEADSET_MODE,
	},
	[ALC298_FIXUP_LENOVO_C940_DUET7] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc298_fixup_lenovo_c940_duet7,
	},
	[ALC287_FIXUP_LENOVO_14IRP8_DUETITL] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc287_fixup_lenovo_14irp8_duetitl,
	},
	[ALC287_FIXUP_LENOVO_LEGION_7] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc287_fixup_lenovo_legion_7,
	},
	[ALC287_FIXUP_13S_GEN2_SPEAKERS] = {
		.type = HDA_FIXUP_VERBS,
		.v.verbs = (const struct hda_verb[]) {
			{ 0x20, AC_VERB_SET_COEF_INDEX, 0x24 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x41 },
			{ 0x20, AC_VERB_SET_COEF_INDEX, 0x26 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x2 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x0 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x0 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0xb020 },
			{ 0x20, AC_VERB_SET_COEF_INDEX, 0x24 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x42 },
			{ 0x20, AC_VERB_SET_COEF_INDEX, 0x26 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x2 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x0 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x0 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0xb020 },
			{}
		},
		.chained = true,
		.chain_id = ALC269_FIXUP_HEADSET_MODE,
	},
	[ALC256_FIXUP_SET_COEF_DEFAULTS] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc256_fixup_set_coef_defaults,
	},
	[ALC245_FIXUP_HP_GPIO_LED] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc245_fixup_hp_gpio_led,
	},
	[ALC256_FIXUP_SYSTEM76_MIC_NO_PRESENCE] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x19, 0x03a11120 }, /* use as headset mic, without its own jack detect */
			{ }
		},
		.chained = true,
		.chain_id = ALC269_FIXUP_HEADSET_MODE_NO_HP_MIC,
	},
	[ALC233_FIXUP_NO_AUDIO_JACK] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc233_fixup_no_audio_jack,
	},
	[ALC256_FIXUP_MIC_NO_PRESENCE_AND_RESUME] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc256_fixup_mic_no_presence_and_resume,
		.chained = true,
		.chain_id = ALC269_FIXUP_HEADSET_MODE_NO_HP_MIC
	},
	[ALC287_FIXUP_LEGION_16ACHG6] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc287_fixup_legion_16achg6_speakers,
	},
	[ALC287_FIXUP_CS35L41_I2C_2] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = cs35l41_fixup_i2c_two,
	},
	[ALC287_FIXUP_CS35L41_I2C_2_HP_GPIO_LED] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = cs35l41_fixup_i2c_two,
		.chained = true,
		.chain_id = ALC285_FIXUP_HP_MUTE_LED,
	},
	[ALC287_FIXUP_CS35L41_I2C_4] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = cs35l41_fixup_i2c_four,
	},
	[ALC245_FIXUP_CS35L41_SPI_2] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = cs35l41_fixup_spi_two,
	},
	[ALC245_FIXUP_CS35L41_SPI_2_HP_GPIO_LED] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = cs35l41_fixup_spi_two,
		.chained = true,
		.chain_id = ALC285_FIXUP_HP_GPIO_LED,
	},
	[ALC245_FIXUP_CS35L41_SPI_4] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = cs35l41_fixup_spi_four,
	},
	[ALC245_FIXUP_CS35L41_SPI_4_HP_GPIO_LED] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = cs35l41_fixup_spi_four,
		.chained = true,
		.chain_id = ALC285_FIXUP_HP_GPIO_LED,
	},
	[ALC285_FIXUP_HP_SPEAKERS_MICMUTE_LED] = {
		.type = HDA_FIXUP_VERBS,
		.v.verbs = (const struct hda_verb[]) {
			 { 0x20, AC_VERB_SET_COEF_INDEX, 0x19 },
			 { 0x20, AC_VERB_SET_PROC_COEF, 0x8e11 },
			 { }
		},
		.chained = true,
		.chain_id = ALC285_FIXUP_HP_MUTE_LED,
	},
	[ALC269_FIXUP_DELL4_MIC_NO_PRESENCE_QUIET] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_dell4_mic_no_presence_quiet,
		.chained = true,
		.chain_id = ALC269_FIXUP_DELL4_MIC_NO_PRESENCE,
	},
	[ALC295_FIXUP_FRAMEWORK_LAPTOP_MIC_NO_PRESENCE] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x19, 0x02a1112c }, /* use as headset mic, without its own jack detect */
			{ }
		},
		.chained = true,
		.chain_id = ALC269_FIXUP_HEADSET_MODE_NO_HP_MIC
	},
	[ALC287_FIXUP_LEGION_16ITHG6] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc287_fixup_legion_16ithg6_speakers,
	},
	[ALC287_FIXUP_YOGA9_14IAP7_BASS_SPK] = {
		.type = HDA_FIXUP_VERBS,
		.v.verbs = (const struct hda_verb[]) {
			// enable left speaker
			{ 0x20, AC_VERB_SET_COEF_INDEX, 0x24 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x41 },

			{ 0x20, AC_VERB_SET_COEF_INDEX, 0x26 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0xc },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x0 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x1a },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0xb020 },

			{ 0x20, AC_VERB_SET_COEF_INDEX, 0x26 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0xf },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x0 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x42 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0xb020 },

			{ 0x20, AC_VERB_SET_COEF_INDEX, 0x26 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x10 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x0 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x40 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0xb020 },

			{ 0x20, AC_VERB_SET_COEF_INDEX, 0x26 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x2 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x0 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x0 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0xb020 },

			// enable right speaker
			{ 0x20, AC_VERB_SET_COEF_INDEX, 0x24 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x46 },

			{ 0x20, AC_VERB_SET_COEF_INDEX, 0x26 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0xc },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x0 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x2a },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0xb020 },

			{ 0x20, AC_VERB_SET_COEF_INDEX, 0x26 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0xf },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x0 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x46 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0xb020 },

			{ 0x20, AC_VERB_SET_COEF_INDEX, 0x26 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x10 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x0 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x44 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0xb020 },

			{ 0x20, AC_VERB_SET_COEF_INDEX, 0x26 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x2 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x0 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x0 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0xb020 },

			{ },
		},
	},
	[ALC287_FIXUP_YOGA9_14IAP7_BASS_SPK_PIN] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc287_fixup_yoga9_14iap7_bass_spk_pin,
		.chained = true,
		.chain_id = ALC287_FIXUP_YOGA9_14IAP7_BASS_SPK,
	},
	[ALC287_FIXUP_YOGA9_14IMH9_BASS_SPK_PIN] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc287_fixup_yoga9_14iap7_bass_spk_pin,
		.chained = true,
		.chain_id = ALC287_FIXUP_CS35L41_I2C_2,
	},
	[ALC295_FIXUP_DELL_INSPIRON_TOP_SPEAKERS] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc295_fixup_dell_inspiron_top_speakers,
		.chained = true,
		.chain_id = ALC269_FIXUP_DELL4_MIC_NO_PRESENCE,
	},
	[ALC236_FIXUP_DELL_DUAL_CODECS] = {
		.type = HDA_FIXUP_PINS,
		.v.func = alc1220_fixup_gb_dual_codecs,
		.chained = true,
		.chain_id = ALC255_FIXUP_DELL1_MIC_NO_PRESENCE,
	},
	[ALC287_FIXUP_CS35L41_I2C_2_THINKPAD_ACPI] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = cs35l41_fixup_i2c_two,
		.chained = true,
		.chain_id = ALC285_FIXUP_THINKPAD_NO_BASS_SPK_HEADSET_JACK,
	},
	[ALC287_FIXUP_TAS2781_I2C] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = tas2781_fixup_i2c,
		.chained = true,
		.chain_id = ALC285_FIXUP_THINKPAD_HEADSET_JACK,
	},
	[ALC287_FIXUP_YOGA7_14ARB7_I2C] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = yoga7_14arb7_fixup_i2c,
		.chained = true,
		.chain_id = ALC285_FIXUP_THINKPAD_HEADSET_JACK,
	},
	[ALC245_FIXUP_HP_MUTE_LED_COEFBIT] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc245_fixup_hp_mute_led_coefbit,
	},
	[ALC245_FIXUP_HP_X360_MUTE_LEDS] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc245_fixup_hp_mute_led_coefbit,
		.chained = true,
		.chain_id = ALC245_FIXUP_HP_GPIO_LED
	},
	[ALC287_FIXUP_THINKPAD_I2S_SPK] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc287_fixup_bind_dacs,
		.chained = true,
		.chain_id = ALC285_FIXUP_THINKPAD_NO_BASS_SPK_HEADSET_JACK,
	},
	[ALC287_FIXUP_MG_RTKC_CSAMP_CS35L41_I2C_THINKPAD] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc287_fixup_bind_dacs,
		.chained = true,
		.chain_id = ALC287_FIXUP_CS35L41_I2C_2_THINKPAD_ACPI,
	},
	[ALC2XX_FIXUP_HEADSET_MIC] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_headset_mic,
	},
	[ALC289_FIXUP_DELL_CS35L41_SPI_2] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = cs35l41_fixup_spi_two,
		.chained = true,
		.chain_id = ALC289_FIXUP_DUAL_SPK
	},
	[ALC294_FIXUP_CS35L41_I2C_2] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = cs35l41_fixup_i2c_two,
	},
	[ALC245_FIXUP_CS35L56_SPI_4_HP_GPIO_LED] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = cs35l56_fixup_spi_four,
		.chained = true,
		.chain_id = ALC285_FIXUP_HP_GPIO_LED,
	},
	[ALC256_FIXUP_ACER_SFG16_MICMUTE_LED] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc256_fixup_acer_sfg16_micmute_led,
	},
	[ALC256_FIXUP_HEADPHONE_AMP_VOL] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc256_decrease_headphone_amp_val,
	},
	[ALC245_FIXUP_HP_SPECTRE_X360_EU0XXX] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc245_fixup_hp_spectre_x360_eu0xxx,
	},
	[ALC285_FIXUP_CS35L56_SPI_2] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = cs35l56_fixup_spi_two,
	},
	[ALC285_FIXUP_CS35L56_I2C_2] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = cs35l56_fixup_i2c_two,
	},
	[ALC285_FIXUP_CS35L56_I2C_4] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = cs35l56_fixup_i2c_four,
	},
	[ALC285_FIXUP_ASUS_GA403U] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc285_fixup_asus_ga403u,
	},
	[ALC285_FIXUP_ASUS_GA403U_HEADSET_MIC] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x19, 0x03a11050 },
			{ 0x1b, 0x03a11c30 },
			{ }
		},
		.chained = true,
		.chain_id = ALC285_FIXUP_ASUS_GA403U_I2C_SPEAKER2_TO_DAC1
	},
	[ALC285_FIXUP_ASUS_GU605_SPI_SPEAKER2_TO_DAC1] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc285_fixup_speaker2_to_dac1,
		.chained = true,
		.chain_id = ALC285_FIXUP_ASUS_GU605_SPI_2_HEADSET_MIC,
	},
	[ALC285_FIXUP_ASUS_GU605_SPI_2_HEADSET_MIC] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x19, 0x03a11050 },
			{ 0x1b, 0x03a11c30 },
			{ }
		},
		.chained = true,
		.chain_id = ALC285_FIXUP_CS35L56_SPI_2
	},
	[ALC285_FIXUP_ASUS_GA403U_I2C_SPEAKER2_TO_DAC1] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc285_fixup_speaker2_to_dac1,
		.chained = true,
		.chain_id = ALC285_FIXUP_ASUS_GA403U,
	},
	[ALC287_FIXUP_LENOVO_THKPAD_WH_ALC1318] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc287_fixup_lenovo_thinkpad_with_alc1318,
		.chained = true,
		.chain_id = ALC269_FIXUP_THINKPAD_ACPI
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
};

static const struct snd_pci_quirk alc269_fixup_tbl[] = {
<<<<<<< HEAD
	SND_PCI_QUIRK(0x103c, 0x1586, "HP", ALC269_FIXUP_MIC2_MUTE_LED),
	SND_PCI_QUIRK(0x1043, 0x1427, "Asus Zenbook UX31E", ALC269VB_FIXUP_DMIC),
	SND_PCI_QUIRK(0x1043, 0x1a13, "Asus G73Jw", ALC269_FIXUP_ASUS_G73JW),
	SND_PCI_QUIRK(0x1043, 0x16e3, "ASUS UX50", ALC269_FIXUP_STEREO_DMIC),
=======
	SND_PCI_QUIRK(0x1025, 0x0283, "Acer TravelMate 8371", ALC269_FIXUP_INV_DMIC),
	SND_PCI_QUIRK(0x1025, 0x029b, "Acer 1810TZ", ALC269_FIXUP_INV_DMIC),
	SND_PCI_QUIRK(0x1025, 0x0349, "Acer AOD260", ALC269_FIXUP_INV_DMIC),
	SND_PCI_QUIRK(0x1025, 0x047c, "Acer AC700", ALC269_FIXUP_ACER_AC700),
	SND_PCI_QUIRK(0x1025, 0x072d, "Acer Aspire V5-571G", ALC269_FIXUP_ASPIRE_HEADSET_MIC),
	SND_PCI_QUIRK(0x1025, 0x0740, "Acer AO725", ALC271_FIXUP_HP_GATE_MIC_JACK),
	SND_PCI_QUIRK(0x1025, 0x0742, "Acer AO756", ALC271_FIXUP_HP_GATE_MIC_JACK),
	SND_PCI_QUIRK(0x1025, 0x0762, "Acer Aspire E1-472", ALC271_FIXUP_HP_GATE_MIC_JACK_E1_572),
	SND_PCI_QUIRK(0x1025, 0x0775, "Acer Aspire E1-572", ALC271_FIXUP_HP_GATE_MIC_JACK_E1_572),
	SND_PCI_QUIRK(0x1025, 0x079b, "Acer Aspire V5-573G", ALC282_FIXUP_ASPIRE_V5_PINS),
	SND_PCI_QUIRK(0x1025, 0x080d, "Acer Aspire V5-122P", ALC269_FIXUP_ASPIRE_HEADSET_MIC),
	SND_PCI_QUIRK(0x1025, 0x0840, "Acer Aspire E1", ALC269VB_FIXUP_ASPIRE_E1_COEF),
	SND_PCI_QUIRK(0x1025, 0x101c, "Acer Veriton N2510G", ALC269_FIXUP_LIFEBOOK),
	SND_PCI_QUIRK(0x1025, 0x102b, "Acer Aspire C24-860", ALC286_FIXUP_ACER_AIO_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1025, 0x1065, "Acer Aspire C20-820", ALC269VC_FIXUP_ACER_HEADSET_MIC),
	SND_PCI_QUIRK(0x1025, 0x106d, "Acer Cloudbook 14", ALC283_FIXUP_CHROME_BOOK),
	SND_PCI_QUIRK(0x1025, 0x1094, "Acer Aspire E5-575T", ALC255_FIXUP_ACER_LIMIT_INT_MIC_BOOST),
	SND_PCI_QUIRK(0x1025, 0x1099, "Acer Aspire E5-523G", ALC255_FIXUP_ACER_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1025, 0x110e, "Acer Aspire ES1-432", ALC255_FIXUP_ACER_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1025, 0x1166, "Acer Veriton N4640G", ALC269_FIXUP_LIFEBOOK),
	SND_PCI_QUIRK(0x1025, 0x1167, "Acer Veriton N6640G", ALC269_FIXUP_LIFEBOOK),
	SND_PCI_QUIRK(0x1025, 0x1246, "Acer Predator Helios 500", ALC299_FIXUP_PREDATOR_SPK),
	SND_PCI_QUIRK(0x1025, 0x1247, "Acer vCopperbox", ALC269VC_FIXUP_ACER_VCOPPERBOX_PINS),
	SND_PCI_QUIRK(0x1025, 0x1248, "Acer Veriton N4660G", ALC269VC_FIXUP_ACER_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1025, 0x1269, "Acer SWIFT SF314-54", ALC256_FIXUP_ACER_HEADSET_MIC),
	SND_PCI_QUIRK(0x1025, 0x126a, "Acer Swift SF114-32", ALC256_FIXUP_ACER_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1025, 0x128f, "Acer Veriton Z6860G", ALC286_FIXUP_ACER_AIO_HEADSET_MIC),
	SND_PCI_QUIRK(0x1025, 0x1290, "Acer Veriton Z4860G", ALC286_FIXUP_ACER_AIO_HEADSET_MIC),
	SND_PCI_QUIRK(0x1025, 0x1291, "Acer Veriton Z4660G", ALC286_FIXUP_ACER_AIO_HEADSET_MIC),
	SND_PCI_QUIRK(0x1025, 0x129c, "Acer SWIFT SF314-55", ALC256_FIXUP_ACER_HEADSET_MIC),
	SND_PCI_QUIRK(0x1025, 0x129d, "Acer SWIFT SF313-51", ALC256_FIXUP_ACER_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1025, 0x1300, "Acer SWIFT SF314-56", ALC256_FIXUP_ACER_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1025, 0x1308, "Acer Aspire Z24-890", ALC286_FIXUP_ACER_AIO_HEADSET_MIC),
	SND_PCI_QUIRK(0x1025, 0x132a, "Acer TravelMate B114-21", ALC233_FIXUP_ACER_HEADSET_MIC),
	SND_PCI_QUIRK(0x1025, 0x1330, "Acer TravelMate X514-51T", ALC255_FIXUP_ACER_HEADSET_MIC),
	SND_PCI_QUIRK(0x1025, 0x141f, "Acer Spin SP513-54N", ALC255_FIXUP_ACER_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1025, 0x142b, "Acer Swift SF314-42", ALC255_FIXUP_ACER_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1025, 0x1430, "Acer TravelMate B311R-31", ALC256_FIXUP_ACER_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1025, 0x1466, "Acer Aspire A515-56", ALC255_FIXUP_ACER_HEADPHONE_AND_MIC),
	SND_PCI_QUIRK(0x1025, 0x1534, "Acer Predator PH315-54", ALC255_FIXUP_ACER_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1025, 0x169a, "Acer Swift SFG16", ALC256_FIXUP_ACER_SFG16_MICMUTE_LED),
	SND_PCI_QUIRK(0x1028, 0x0470, "Dell M101z", ALC269_FIXUP_DELL_M101Z),
	SND_PCI_QUIRK(0x1028, 0x053c, "Dell Latitude E5430", ALC292_FIXUP_DELL_E7X),
	SND_PCI_QUIRK(0x1028, 0x054b, "Dell XPS one 2710", ALC275_FIXUP_DELL_XPS),
	SND_PCI_QUIRK(0x1028, 0x05bd, "Dell Latitude E6440", ALC292_FIXUP_DELL_E7X),
	SND_PCI_QUIRK(0x1028, 0x05be, "Dell Latitude E6540", ALC292_FIXUP_DELL_E7X),
	SND_PCI_QUIRK(0x1028, 0x05ca, "Dell Latitude E7240", ALC292_FIXUP_DELL_E7X),
	SND_PCI_QUIRK(0x1028, 0x05cb, "Dell Latitude E7440", ALC292_FIXUP_DELL_E7X),
	SND_PCI_QUIRK(0x1028, 0x05da, "Dell Vostro 5460", ALC290_FIXUP_SUBWOOFER),
	SND_PCI_QUIRK(0x1028, 0x05f4, "Dell", ALC269_FIXUP_DELL1_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1028, 0x05f5, "Dell", ALC269_FIXUP_DELL1_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1028, 0x05f6, "Dell", ALC269_FIXUP_DELL1_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1028, 0x0615, "Dell Vostro 5470", ALC290_FIXUP_SUBWOOFER_HSJACK),
	SND_PCI_QUIRK(0x1028, 0x0616, "Dell Vostro 5470", ALC290_FIXUP_SUBWOOFER_HSJACK),
	SND_PCI_QUIRK(0x1028, 0x062c, "Dell Latitude E5550", ALC292_FIXUP_DELL_E7X),
	SND_PCI_QUIRK(0x1028, 0x062e, "Dell Latitude E7450", ALC292_FIXUP_DELL_E7X),
	SND_PCI_QUIRK(0x1028, 0x0638, "Dell Inspiron 5439", ALC290_FIXUP_MONO_SPEAKERS_HSJACK),
	SND_PCI_QUIRK(0x1028, 0x064a, "Dell", ALC293_FIXUP_DELL1_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1028, 0x064b, "Dell", ALC293_FIXUP_DELL1_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1028, 0x0665, "Dell XPS 13", ALC288_FIXUP_DELL_XPS_13),
	SND_PCI_QUIRK(0x1028, 0x0669, "Dell Optiplex 9020m", ALC255_FIXUP_DELL1_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1028, 0x069a, "Dell Vostro 5480", ALC290_FIXUP_SUBWOOFER_HSJACK),
	SND_PCI_QUIRK(0x1028, 0x06c7, "Dell", ALC255_FIXUP_DELL1_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1028, 0x06d9, "Dell", ALC293_FIXUP_DELL1_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1028, 0x06da, "Dell", ALC293_FIXUP_DELL1_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1028, 0x06db, "Dell", ALC293_FIXUP_DISABLE_AAMIX_MULTIJACK),
	SND_PCI_QUIRK(0x1028, 0x06dd, "Dell", ALC293_FIXUP_DISABLE_AAMIX_MULTIJACK),
	SND_PCI_QUIRK(0x1028, 0x06de, "Dell", ALC293_FIXUP_DISABLE_AAMIX_MULTIJACK),
	SND_PCI_QUIRK(0x1028, 0x06df, "Dell", ALC293_FIXUP_DISABLE_AAMIX_MULTIJACK),
	SND_PCI_QUIRK(0x1028, 0x06e0, "Dell", ALC293_FIXUP_DISABLE_AAMIX_MULTIJACK),
	SND_PCI_QUIRK(0x1028, 0x0706, "Dell Inspiron 7559", ALC256_FIXUP_DELL_INSPIRON_7559_SUBWOOFER),
	SND_PCI_QUIRK(0x1028, 0x0725, "Dell Inspiron 3162", ALC255_FIXUP_DELL_SPK_NOISE),
	SND_PCI_QUIRK(0x1028, 0x0738, "Dell Precision 5820", ALC269_FIXUP_NO_SHUTUP),
	SND_PCI_QUIRK(0x1028, 0x075c, "Dell XPS 27 7760", ALC298_FIXUP_SPK_VOLUME),
	SND_PCI_QUIRK(0x1028, 0x075d, "Dell AIO", ALC298_FIXUP_SPK_VOLUME),
	SND_PCI_QUIRK(0x1028, 0x0798, "Dell Inspiron 17 7000 Gaming", ALC256_FIXUP_DELL_INSPIRON_7559_SUBWOOFER),
	SND_PCI_QUIRK(0x1028, 0x07b0, "Dell Precision 7520", ALC295_FIXUP_DISABLE_DAC3),
	SND_PCI_QUIRK(0x1028, 0x080c, "Dell WYSE", ALC225_FIXUP_DELL_WYSE_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1028, 0x084b, "Dell", ALC274_FIXUP_DELL_AIO_LINEOUT_VERB),
	SND_PCI_QUIRK(0x1028, 0x084e, "Dell", ALC274_FIXUP_DELL_AIO_LINEOUT_VERB),
	SND_PCI_QUIRK(0x1028, 0x0871, "Dell Precision 3630", ALC255_FIXUP_DELL_HEADSET_MIC),
	SND_PCI_QUIRK(0x1028, 0x0872, "Dell Precision 3630", ALC255_FIXUP_DELL_HEADSET_MIC),
	SND_PCI_QUIRK(0x1028, 0x0873, "Dell Precision 3930", ALC255_FIXUP_DUMMY_LINEOUT_VERB),
	SND_PCI_QUIRK(0x1028, 0x08ad, "Dell WYSE AIO", ALC225_FIXUP_DELL_WYSE_AIO_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1028, 0x08ae, "Dell WYSE NB", ALC225_FIXUP_DELL1_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1028, 0x0935, "Dell", ALC274_FIXUP_DELL_AIO_LINEOUT_VERB),
	SND_PCI_QUIRK(0x1028, 0x097d, "Dell Precision", ALC289_FIXUP_DUAL_SPK),
	SND_PCI_QUIRK(0x1028, 0x097e, "Dell Precision", ALC289_FIXUP_DUAL_SPK),
	SND_PCI_QUIRK(0x1028, 0x098d, "Dell Precision", ALC233_FIXUP_ASUS_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1028, 0x09bf, "Dell Precision", ALC233_FIXUP_ASUS_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1028, 0x0a2e, "Dell", ALC236_FIXUP_DELL_AIO_HEADSET_MIC),
	SND_PCI_QUIRK(0x1028, 0x0a30, "Dell", ALC236_FIXUP_DELL_AIO_HEADSET_MIC),
	SND_PCI_QUIRK(0x1028, 0x0a38, "Dell Latitude 7520", ALC269_FIXUP_DELL4_MIC_NO_PRESENCE_QUIET),
	SND_PCI_QUIRK(0x1028, 0x0a58, "Dell", ALC255_FIXUP_DELL_HEADSET_MIC),
	SND_PCI_QUIRK(0x1028, 0x0a61, "Dell XPS 15 9510", ALC289_FIXUP_DUAL_SPK),
	SND_PCI_QUIRK(0x1028, 0x0a62, "Dell Precision 5560", ALC289_FIXUP_DUAL_SPK),
	SND_PCI_QUIRK(0x1028, 0x0a9d, "Dell Latitude 5430", ALC269_FIXUP_DELL4_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1028, 0x0a9e, "Dell Latitude 5430", ALC269_FIXUP_DELL4_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1028, 0x0b19, "Dell XPS 15 9520", ALC289_FIXUP_DUAL_SPK),
	SND_PCI_QUIRK(0x1028, 0x0b1a, "Dell Precision 5570", ALC289_FIXUP_DUAL_SPK),
	SND_PCI_QUIRK(0x1028, 0x0b27, "Dell", ALC245_FIXUP_CS35L41_SPI_2),
	SND_PCI_QUIRK(0x1028, 0x0b28, "Dell", ALC245_FIXUP_CS35L41_SPI_2),
	SND_PCI_QUIRK(0x1028, 0x0b37, "Dell Inspiron 16 Plus 7620 2-in-1", ALC295_FIXUP_DELL_INSPIRON_TOP_SPEAKERS),
	SND_PCI_QUIRK(0x1028, 0x0b71, "Dell Inspiron 16 Plus 7620", ALC295_FIXUP_DELL_INSPIRON_TOP_SPEAKERS),
	SND_PCI_QUIRK(0x1028, 0x0beb, "Dell XPS 15 9530 (2023)", ALC289_FIXUP_DELL_CS35L41_SPI_2),
	SND_PCI_QUIRK(0x1028, 0x0c03, "Dell Precision 5340", ALC269_FIXUP_DELL4_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1028, 0x0c0b, "Dell Oasis 14 RPL-P", ALC289_FIXUP_RTK_AMP_DUAL_SPK),
	SND_PCI_QUIRK(0x1028, 0x0c0d, "Dell Oasis", ALC289_FIXUP_RTK_AMP_DUAL_SPK),
	SND_PCI_QUIRK(0x1028, 0x0c0e, "Dell Oasis 16", ALC289_FIXUP_RTK_AMP_DUAL_SPK),
	SND_PCI_QUIRK(0x1028, 0x0c19, "Dell Precision 3340", ALC236_FIXUP_DELL_DUAL_CODECS),
	SND_PCI_QUIRK(0x1028, 0x0c1a, "Dell Precision 3340", ALC236_FIXUP_DELL_DUAL_CODECS),
	SND_PCI_QUIRK(0x1028, 0x0c1b, "Dell Precision 3440", ALC236_FIXUP_DELL_DUAL_CODECS),
	SND_PCI_QUIRK(0x1028, 0x0c1c, "Dell Precision 3540", ALC236_FIXUP_DELL_DUAL_CODECS),
	SND_PCI_QUIRK(0x1028, 0x0c1d, "Dell Precision 3440", ALC236_FIXUP_DELL_DUAL_CODECS),
	SND_PCI_QUIRK(0x1028, 0x0c1e, "Dell Precision 3540", ALC236_FIXUP_DELL_DUAL_CODECS),
	SND_PCI_QUIRK(0x1028, 0x0c28, "Dell Inspiron 16 Plus 7630", ALC295_FIXUP_DELL_INSPIRON_TOP_SPEAKERS),
	SND_PCI_QUIRK(0x1028, 0x0c4d, "Dell", ALC287_FIXUP_CS35L41_I2C_4),
	SND_PCI_QUIRK(0x1028, 0x0cbd, "Dell Oasis 13 CS MTL-U", ALC289_FIXUP_DELL_CS35L41_SPI_2),
	SND_PCI_QUIRK(0x1028, 0x0cbe, "Dell Oasis 13 2-IN-1 MTL-U", ALC289_FIXUP_DELL_CS35L41_SPI_2),
	SND_PCI_QUIRK(0x1028, 0x0cbf, "Dell Oasis 13 Low Weight MTU-L", ALC289_FIXUP_DELL_CS35L41_SPI_2),
	SND_PCI_QUIRK(0x1028, 0x0cc0, "Dell Oasis 13", ALC289_FIXUP_RTK_AMP_DUAL_SPK),
	SND_PCI_QUIRK(0x1028, 0x0cc1, "Dell Oasis 14 MTL-H/U", ALC289_FIXUP_DELL_CS35L41_SPI_2),
	SND_PCI_QUIRK(0x1028, 0x0cc2, "Dell Oasis 14 2-in-1 MTL-H/U", ALC289_FIXUP_DELL_CS35L41_SPI_2),
	SND_PCI_QUIRK(0x1028, 0x0cc3, "Dell Oasis 14 Low Weight MTL-U", ALC289_FIXUP_DELL_CS35L41_SPI_2),
	SND_PCI_QUIRK(0x1028, 0x0cc4, "Dell Oasis 16 MTL-H/U", ALC289_FIXUP_DELL_CS35L41_SPI_2),
	SND_PCI_QUIRK(0x1028, 0x0cc5, "Dell Oasis 14", ALC289_FIXUP_RTK_AMP_DUAL_SPK),
	SND_PCI_QUIRK(0x1028, 0x164a, "Dell", ALC293_FIXUP_DELL1_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1028, 0x164b, "Dell", ALC293_FIXUP_DELL1_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x103c, 0x1586, "HP", ALC269_FIXUP_HP_MUTE_LED_MIC2),
	SND_PCI_QUIRK(0x103c, 0x18e6, "HP", ALC269_FIXUP_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x218b, "HP", ALC269_FIXUP_LIMIT_INT_MIC_BOOST_MUTE_LED),
	SND_PCI_QUIRK(0x103c, 0x21f9, "HP", ALC269_FIXUP_HP_MUTE_LED_MIC1),
	SND_PCI_QUIRK(0x103c, 0x2210, "HP", ALC269_FIXUP_HP_MUTE_LED_MIC1),
	SND_PCI_QUIRK(0x103c, 0x2214, "HP", ALC269_FIXUP_HP_MUTE_LED_MIC1),
	SND_PCI_QUIRK(0x103c, 0x221b, "HP", ALC269_FIXUP_HP_GPIO_MIC1_LED),
	SND_PCI_QUIRK(0x103c, 0x221c, "HP EliteBook 755 G2", ALC280_FIXUP_HP_HEADSET_MIC),
	SND_PCI_QUIRK(0x103c, 0x2221, "HP", ALC269_FIXUP_HP_GPIO_MIC1_LED),
	SND_PCI_QUIRK(0x103c, 0x2225, "HP", ALC269_FIXUP_HP_GPIO_MIC1_LED),
	SND_PCI_QUIRK(0x103c, 0x2236, "HP", ALC269_FIXUP_HP_LINE1_MIC1_LED),
	SND_PCI_QUIRK(0x103c, 0x2237, "HP", ALC269_FIXUP_HP_LINE1_MIC1_LED),
	SND_PCI_QUIRK(0x103c, 0x2238, "HP", ALC269_FIXUP_HP_LINE1_MIC1_LED),
	SND_PCI_QUIRK(0x103c, 0x2239, "HP", ALC269_FIXUP_HP_LINE1_MIC1_LED),
	SND_PCI_QUIRK(0x103c, 0x224b, "HP", ALC269_FIXUP_HP_LINE1_MIC1_LED),
	SND_PCI_QUIRK(0x103c, 0x2253, "HP", ALC269_FIXUP_HP_GPIO_MIC1_LED),
	SND_PCI_QUIRK(0x103c, 0x2254, "HP", ALC269_FIXUP_HP_GPIO_MIC1_LED),
	SND_PCI_QUIRK(0x103c, 0x2255, "HP", ALC269_FIXUP_HP_GPIO_MIC1_LED),
	SND_PCI_QUIRK(0x103c, 0x2256, "HP", ALC269_FIXUP_HP_GPIO_MIC1_LED),
	SND_PCI_QUIRK(0x103c, 0x2257, "HP", ALC269_FIXUP_HP_GPIO_MIC1_LED),
	SND_PCI_QUIRK(0x103c, 0x2259, "HP", ALC269_FIXUP_HP_GPIO_MIC1_LED),
	SND_PCI_QUIRK(0x103c, 0x225a, "HP", ALC269_FIXUP_HP_DOCK_GPIO_MIC1_LED),
	SND_PCI_QUIRK(0x103c, 0x225f, "HP", ALC280_FIXUP_HP_GPIO2_MIC_HOTKEY),
	SND_PCI_QUIRK(0x103c, 0x2260, "HP", ALC269_FIXUP_HP_MUTE_LED_MIC1),
	SND_PCI_QUIRK(0x103c, 0x2263, "HP", ALC269_FIXUP_HP_MUTE_LED_MIC1),
	SND_PCI_QUIRK(0x103c, 0x2264, "HP", ALC269_FIXUP_HP_MUTE_LED_MIC1),
	SND_PCI_QUIRK(0x103c, 0x2265, "HP", ALC269_FIXUP_HP_MUTE_LED_MIC1),
	SND_PCI_QUIRK(0x103c, 0x2268, "HP", ALC269_FIXUP_HP_MUTE_LED_MIC1),
	SND_PCI_QUIRK(0x103c, 0x226a, "HP", ALC269_FIXUP_HP_MUTE_LED_MIC1),
	SND_PCI_QUIRK(0x103c, 0x226b, "HP", ALC269_FIXUP_HP_MUTE_LED_MIC1),
	SND_PCI_QUIRK(0x103c, 0x226e, "HP", ALC269_FIXUP_HP_MUTE_LED_MIC1),
	SND_PCI_QUIRK(0x103c, 0x2271, "HP", ALC286_FIXUP_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x2272, "HP", ALC269_FIXUP_HP_GPIO_MIC1_LED),
	SND_PCI_QUIRK(0x103c, 0x2272, "HP", ALC280_FIXUP_HP_DOCK_PINS),
	SND_PCI_QUIRK(0x103c, 0x2273, "HP", ALC269_FIXUP_HP_GPIO_MIC1_LED),
	SND_PCI_QUIRK(0x103c, 0x2273, "HP", ALC280_FIXUP_HP_DOCK_PINS),
	SND_PCI_QUIRK(0x103c, 0x2278, "HP", ALC269_FIXUP_HP_GPIO_MIC1_LED),
	SND_PCI_QUIRK(0x103c, 0x227f, "HP", ALC269_FIXUP_HP_MUTE_LED_MIC1),
	SND_PCI_QUIRK(0x103c, 0x2282, "HP", ALC269_FIXUP_HP_MUTE_LED_MIC1),
	SND_PCI_QUIRK(0x103c, 0x228b, "HP", ALC269_FIXUP_HP_MUTE_LED_MIC1),
	SND_PCI_QUIRK(0x103c, 0x228e, "HP", ALC269_FIXUP_HP_MUTE_LED_MIC1),
	SND_PCI_QUIRK(0x103c, 0x229e, "HP", ALC269_FIXUP_HP_MUTE_LED_MIC1),
	SND_PCI_QUIRK(0x103c, 0x22b2, "HP", ALC269_FIXUP_HP_MUTE_LED_MIC1),
	SND_PCI_QUIRK(0x103c, 0x22b7, "HP", ALC269_FIXUP_HP_MUTE_LED_MIC1),
	SND_PCI_QUIRK(0x103c, 0x22bf, "HP", ALC269_FIXUP_HP_MUTE_LED_MIC1),
	SND_PCI_QUIRK(0x103c, 0x22c4, "HP", ALC269_FIXUP_HP_MUTE_LED_MIC1),
	SND_PCI_QUIRK(0x103c, 0x22c5, "HP", ALC269_FIXUP_HP_MUTE_LED_MIC1),
	SND_PCI_QUIRK(0x103c, 0x22c7, "HP", ALC269_FIXUP_HP_MUTE_LED_MIC1),
	SND_PCI_QUIRK(0x103c, 0x22c8, "HP", ALC269_FIXUP_HP_MUTE_LED_MIC1),
	SND_PCI_QUIRK(0x103c, 0x22cf, "HP", ALC269_FIXUP_HP_MUTE_LED_MIC1),
	SND_PCI_QUIRK(0x103c, 0x22db, "HP", ALC280_FIXUP_HP_9480M),
	SND_PCI_QUIRK(0x103c, 0x22dc, "HP", ALC269_FIXUP_HP_GPIO_MIC1_LED),
	SND_PCI_QUIRK(0x103c, 0x22fb, "HP", ALC269_FIXUP_HP_GPIO_MIC1_LED),
	SND_PCI_QUIRK(0x103c, 0x2334, "HP", ALC269_FIXUP_HP_MUTE_LED_MIC1),
	SND_PCI_QUIRK(0x103c, 0x2335, "HP", ALC269_FIXUP_HP_MUTE_LED_MIC1),
	SND_PCI_QUIRK(0x103c, 0x2336, "HP", ALC269_FIXUP_HP_MUTE_LED_MIC1),
	SND_PCI_QUIRK(0x103c, 0x2337, "HP", ALC269_FIXUP_HP_MUTE_LED_MIC1),
	SND_PCI_QUIRK(0x103c, 0x2b5e, "HP 288 Pro G2 MT", ALC221_FIXUP_HP_288PRO_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x103c, 0x802e, "HP Z240 SFF", ALC221_FIXUP_HP_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x103c, 0x802f, "HP Z240", ALC221_FIXUP_HP_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x103c, 0x8077, "HP", ALC256_FIXUP_HP_HEADSET_MIC),
	SND_PCI_QUIRK(0x103c, 0x8158, "HP", ALC256_FIXUP_HP_HEADSET_MIC),
	SND_PCI_QUIRK(0x103c, 0x820d, "HP Pavilion 15", ALC295_FIXUP_HP_X360),
	SND_PCI_QUIRK(0x103c, 0x8256, "HP", ALC221_FIXUP_HP_FRONT_MIC),
	SND_PCI_QUIRK(0x103c, 0x827e, "HP x360", ALC295_FIXUP_HP_X360),
	SND_PCI_QUIRK(0x103c, 0x827f, "HP x360", ALC269_FIXUP_HP_MUTE_LED_MIC3),
	SND_PCI_QUIRK(0x103c, 0x82bf, "HP G3 mini", ALC221_FIXUP_HP_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x103c, 0x82c0, "HP G3 mini premium", ALC221_FIXUP_HP_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x103c, 0x83b9, "HP Spectre x360", ALC269_FIXUP_HP_MUTE_LED_MIC3),
	SND_PCI_QUIRK(0x103c, 0x841c, "HP Pavilion 15-CK0xx", ALC269_FIXUP_HP_MUTE_LED_MIC3),
	SND_PCI_QUIRK(0x103c, 0x8497, "HP Envy x360", ALC269_FIXUP_HP_MUTE_LED_MIC3),
	SND_PCI_QUIRK(0x103c, 0x84ae, "HP 15-db0403ng", ALC236_FIXUP_HP_MUTE_LED_COEFBIT2),
	SND_PCI_QUIRK(0x103c, 0x84da, "HP OMEN dc0019-ur", ALC295_FIXUP_HP_OMEN),
	SND_PCI_QUIRK(0x103c, 0x84e7, "HP Pavilion 15", ALC269_FIXUP_HP_MUTE_LED_MIC3),
	SND_PCI_QUIRK(0x103c, 0x8519, "HP Spectre x360 15-df0xxx", ALC285_FIXUP_HP_SPECTRE_X360),
	SND_PCI_QUIRK(0x103c, 0x8537, "HP ProBook 440 G6", ALC236_FIXUP_HP_MUTE_LED_MICMUTE_VREF),
	SND_PCI_QUIRK(0x103c, 0x85de, "HP Envy x360 13-ar0xxx", ALC285_FIXUP_HP_ENVY_X360),
	SND_PCI_QUIRK(0x103c, 0x860f, "HP ZBook 15 G6", ALC285_FIXUP_HP_GPIO_AMP_INIT),
	SND_PCI_QUIRK(0x103c, 0x861f, "HP Elite Dragonfly G1", ALC285_FIXUP_HP_GPIO_AMP_INIT),
	SND_PCI_QUIRK(0x103c, 0x869d, "HP", ALC236_FIXUP_HP_MUTE_LED),
	SND_PCI_QUIRK(0x103c, 0x86c1, "HP Laptop 15-da3001TU", ALC236_FIXUP_HP_MUTE_LED_COEFBIT2),
	SND_PCI_QUIRK(0x103c, 0x86c7, "HP Envy AiO 32", ALC274_FIXUP_HP_ENVY_GPIO),
	SND_PCI_QUIRK(0x103c, 0x86e7, "HP Spectre x360 15-eb0xxx", ALC285_FIXUP_HP_SPECTRE_X360_EB1),
	SND_PCI_QUIRK(0x103c, 0x86e8, "HP Spectre x360 15-eb0xxx", ALC285_FIXUP_HP_SPECTRE_X360_EB1),
	SND_PCI_QUIRK(0x103c, 0x86f9, "HP Spectre x360 13-aw0xxx", ALC285_FIXUP_HP_SPECTRE_X360_MUTE_LED),
	SND_PCI_QUIRK(0x103c, 0x8716, "HP Elite Dragonfly G2 Notebook PC", ALC285_FIXUP_HP_GPIO_AMP_INIT),
	SND_PCI_QUIRK(0x103c, 0x8720, "HP EliteBook x360 1040 G8 Notebook PC", ALC285_FIXUP_HP_GPIO_AMP_INIT),
	SND_PCI_QUIRK(0x103c, 0x8724, "HP EliteBook 850 G7", ALC285_FIXUP_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8728, "HP EliteBook 840 G7", ALC285_FIXUP_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8729, "HP", ALC285_FIXUP_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8730, "HP ProBook 445 G7", ALC236_FIXUP_HP_MUTE_LED_MICMUTE_VREF),
	SND_PCI_QUIRK(0x103c, 0x8735, "HP ProBook 435 G7", ALC236_FIXUP_HP_MUTE_LED_MICMUTE_VREF),
	SND_PCI_QUIRK(0x103c, 0x8736, "HP", ALC285_FIXUP_HP_GPIO_AMP_INIT),
	SND_PCI_QUIRK(0x103c, 0x8760, "HP", ALC285_FIXUP_HP_MUTE_LED),
	SND_PCI_QUIRK(0x103c, 0x876e, "HP ENVY x360 Convertible 13-ay0xxx", ALC245_FIXUP_HP_X360_MUTE_LEDS),
	SND_PCI_QUIRK(0x103c, 0x877a, "HP", ALC285_FIXUP_HP_MUTE_LED),
	SND_PCI_QUIRK(0x103c, 0x877d, "HP", ALC236_FIXUP_HP_MUTE_LED),
	SND_PCI_QUIRK(0x103c, 0x8780, "HP ZBook Fury 17 G7 Mobile Workstation",
		      ALC285_FIXUP_HP_GPIO_AMP_INIT),
	SND_PCI_QUIRK(0x103c, 0x8783, "HP ZBook Fury 15 G7 Mobile Workstation",
		      ALC285_FIXUP_HP_GPIO_AMP_INIT),
	SND_PCI_QUIRK(0x103c, 0x8786, "HP OMEN 15", ALC285_FIXUP_HP_MUTE_LED),
	SND_PCI_QUIRK(0x103c, 0x8787, "HP OMEN 15", ALC285_FIXUP_HP_MUTE_LED),
	SND_PCI_QUIRK(0x103c, 0x8788, "HP OMEN 15", ALC285_FIXUP_HP_MUTE_LED),
	SND_PCI_QUIRK(0x103c, 0x87b7, "HP Laptop 14-fq0xxx", ALC236_FIXUP_HP_MUTE_LED_COEFBIT2),
	SND_PCI_QUIRK(0x103c, 0x87c8, "HP", ALC287_FIXUP_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x87e5, "HP ProBook 440 G8 Notebook PC", ALC236_FIXUP_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x87e7, "HP ProBook 450 G8 Notebook PC", ALC236_FIXUP_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x87f1, "HP ProBook 630 G8 Notebook PC", ALC236_FIXUP_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x87f2, "HP ProBook 640 G8 Notebook PC", ALC236_FIXUP_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x87f4, "HP", ALC287_FIXUP_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x87f5, "HP", ALC287_FIXUP_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x87f6, "HP Spectre x360 14", ALC245_FIXUP_HP_X360_AMP),
	SND_PCI_QUIRK(0x103c, 0x87f7, "HP Spectre x360 14", ALC245_FIXUP_HP_X360_AMP),
	SND_PCI_QUIRK(0x103c, 0x87fe, "HP Laptop 15s-fq2xxx", ALC236_FIXUP_HP_MUTE_LED_COEFBIT2),
	SND_PCI_QUIRK(0x103c, 0x8805, "HP ProBook 650 G8 Notebook PC", ALC236_FIXUP_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x880d, "HP EliteBook 830 G8 Notebook PC", ALC285_FIXUP_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8811, "HP Spectre x360 15-eb1xxx", ALC285_FIXUP_HP_SPECTRE_X360_EB1),
	SND_PCI_QUIRK(0x103c, 0x8812, "HP Spectre x360 15-eb1xxx", ALC285_FIXUP_HP_SPECTRE_X360_EB1),
	SND_PCI_QUIRK(0x103c, 0x881d, "HP 250 G8 Notebook PC", ALC236_FIXUP_HP_MUTE_LED_COEFBIT2),
	SND_PCI_QUIRK(0x103c, 0x8846, "HP EliteBook 850 G8 Notebook PC", ALC285_FIXUP_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8847, "HP EliteBook x360 830 G8 Notebook PC", ALC285_FIXUP_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x884b, "HP EliteBook 840 Aero G8 Notebook PC", ALC285_FIXUP_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x884c, "HP EliteBook 840 G8 Notebook PC", ALC285_FIXUP_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8862, "HP ProBook 445 G8 Notebook PC", ALC236_FIXUP_HP_LIMIT_INT_MIC_BOOST),
	SND_PCI_QUIRK(0x103c, 0x8863, "HP ProBook 445 G8 Notebook PC", ALC236_FIXUP_HP_LIMIT_INT_MIC_BOOST),
	SND_PCI_QUIRK(0x103c, 0x886d, "HP ZBook Fury 17.3 Inch G8 Mobile Workstation PC", ALC285_FIXUP_HP_GPIO_AMP_INIT),
	SND_PCI_QUIRK(0x103c, 0x8870, "HP ZBook Fury 15.6 Inch G8 Mobile Workstation PC", ALC285_FIXUP_HP_GPIO_AMP_INIT),
	SND_PCI_QUIRK(0x103c, 0x8873, "HP ZBook Studio 15.6 Inch G8 Mobile Workstation PC", ALC285_FIXUP_HP_GPIO_AMP_INIT),
	SND_PCI_QUIRK(0x103c, 0x887a, "HP Laptop 15s-eq2xxx", ALC236_FIXUP_HP_MUTE_LED_COEFBIT2),
	SND_PCI_QUIRK(0x103c, 0x888a, "HP ENVY x360 Convertible 15-eu0xxx", ALC245_FIXUP_HP_X360_MUTE_LEDS),
	SND_PCI_QUIRK(0x103c, 0x888d, "HP ZBook Power 15.6 inch G8 Mobile Workstation PC", ALC236_FIXUP_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8895, "HP EliteBook 855 G8 Notebook PC", ALC285_FIXUP_HP_SPEAKERS_MICMUTE_LED),
	SND_PCI_QUIRK(0x103c, 0x8896, "HP EliteBook 855 G8 Notebook PC", ALC285_FIXUP_HP_MUTE_LED),
	SND_PCI_QUIRK(0x103c, 0x8898, "HP EliteBook 845 G8 Notebook PC", ALC285_FIXUP_HP_LIMIT_INT_MIC_BOOST),
	SND_PCI_QUIRK(0x103c, 0x88d0, "HP Pavilion 15-eh1xxx (mainboard 88D0)", ALC287_FIXUP_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8902, "HP OMEN 16", ALC285_FIXUP_HP_MUTE_LED),
	SND_PCI_QUIRK(0x103c, 0x890e, "HP 255 G8 Notebook PC", ALC236_FIXUP_HP_MUTE_LED_COEFBIT2),
	SND_PCI_QUIRK(0x103c, 0x8919, "HP Pavilion Aero Laptop 13-be0xxx", ALC287_FIXUP_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x896d, "HP ZBook Firefly 16 G9", ALC245_FIXUP_CS35L41_SPI_2_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x896e, "HP EliteBook x360 830 G9", ALC245_FIXUP_CS35L41_SPI_2_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8971, "HP EliteBook 830 G9", ALC245_FIXUP_CS35L41_SPI_2_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8972, "HP EliteBook 840 G9", ALC245_FIXUP_CS35L41_SPI_2_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8973, "HP EliteBook 860 G9", ALC245_FIXUP_CS35L41_SPI_2_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8974, "HP EliteBook 840 Aero G9", ALC245_FIXUP_CS35L41_SPI_2_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8975, "HP EliteBook x360 840 Aero G9", ALC245_FIXUP_CS35L41_SPI_2_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x897d, "HP mt440 Mobile Thin Client U74", ALC236_FIXUP_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8981, "HP Elite Dragonfly G3", ALC245_FIXUP_CS35L41_SPI_4),
	SND_PCI_QUIRK(0x103c, 0x898e, "HP EliteBook 835 G9", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x103c, 0x898f, "HP EliteBook 835 G9", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x103c, 0x8991, "HP EliteBook 845 G9", ALC287_FIXUP_CS35L41_I2C_2_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8992, "HP EliteBook 845 G9", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x103c, 0x8994, "HP EliteBook 855 G9", ALC287_FIXUP_CS35L41_I2C_2_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8995, "HP EliteBook 855 G9", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x103c, 0x89a4, "HP ProBook 440 G9", ALC236_FIXUP_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x89a6, "HP ProBook 450 G9", ALC236_FIXUP_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x89aa, "HP EliteBook 630 G9", ALC236_FIXUP_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x89ac, "HP EliteBook 640 G9", ALC236_FIXUP_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x89ae, "HP EliteBook 650 G9", ALC236_FIXUP_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x89c0, "HP ZBook Power 15.6 G9", ALC245_FIXUP_CS35L41_SPI_2_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x89c3, "Zbook Studio G9", ALC245_FIXUP_CS35L41_SPI_4_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x89c6, "Zbook Fury 17 G9", ALC245_FIXUP_CS35L41_SPI_2_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x89ca, "HP", ALC236_FIXUP_HP_MUTE_LED_MICMUTE_VREF),
	SND_PCI_QUIRK(0x103c, 0x89d3, "HP EliteBook 645 G9 (MB 89D2)", ALC236_FIXUP_HP_MUTE_LED_MICMUTE_VREF),
	SND_PCI_QUIRK(0x103c, 0x89e7, "HP Elite x2 G9", ALC245_FIXUP_CS35L41_SPI_2_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8a0f, "HP Pavilion 14-ec1xxx", ALC287_FIXUP_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8a20, "HP Laptop 15s-fq5xxx", ALC236_FIXUP_HP_MUTE_LED_COEFBIT2),
	SND_PCI_QUIRK(0x103c, 0x8a25, "HP Victus 16-d1xxx (MB 8A25)", ALC245_FIXUP_HP_MUTE_LED_COEFBIT),
	SND_PCI_QUIRK(0x103c, 0x8a28, "HP Envy 13", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x103c, 0x8a29, "HP Envy 15", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x103c, 0x8a2a, "HP Envy 15", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x103c, 0x8a2b, "HP Envy 15", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x103c, 0x8a2c, "HP Envy 16", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x103c, 0x8a2d, "HP Envy 16", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x103c, 0x8a2e, "HP Envy 16", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x103c, 0x8a2e, "HP Envy 17", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x103c, 0x8a30, "HP Envy 17", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x103c, 0x8a31, "HP Envy 15", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x103c, 0x8a6e, "HP EDNA 360", ALC287_FIXUP_CS35L41_I2C_4),
	SND_PCI_QUIRK(0x103c, 0x8a74, "HP ProBook 440 G8 Notebook PC", ALC236_FIXUP_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8a78, "HP Dev One", ALC285_FIXUP_HP_LIMIT_INT_MIC_BOOST),
	SND_PCI_QUIRK(0x103c, 0x8aa0, "HP ProBook 440 G9 (MB 8A9E)", ALC236_FIXUP_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8aa3, "HP ProBook 450 G9 (MB 8AA1)", ALC236_FIXUP_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8aa8, "HP EliteBook 640 G9 (MB 8AA6)", ALC236_FIXUP_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8aab, "HP EliteBook 650 G9 (MB 8AA9)", ALC236_FIXUP_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8ab9, "HP EliteBook 840 G8 (MB 8AB8)", ALC285_FIXUP_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8abb, "HP ZBook Firefly 14 G9", ALC245_FIXUP_CS35L41_SPI_2_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8ad1, "HP EliteBook 840 14 inch G9 Notebook PC", ALC245_FIXUP_CS35L41_SPI_2_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8ad2, "HP EliteBook 860 16 inch G9 Notebook PC", ALC245_FIXUP_CS35L41_SPI_2_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8ad8, "HP 800 G9", ALC245_FIXUP_CS35L41_SPI_2_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8b0f, "HP Elite mt645 G7 Mobile Thin Client U81", ALC236_FIXUP_HP_MUTE_LED_MICMUTE_VREF),
	SND_PCI_QUIRK(0x103c, 0x8b2f, "HP 255 15.6 inch G10 Notebook PC", ALC236_FIXUP_HP_MUTE_LED_COEFBIT2),
	SND_PCI_QUIRK(0x103c, 0x8b3a, "HP Envy 15", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x103c, 0x8b3f, "HP mt440 Mobile Thin Client U91", ALC236_FIXUP_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8b42, "HP", ALC245_FIXUP_CS35L41_SPI_2_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8b43, "HP", ALC245_FIXUP_CS35L41_SPI_2_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8b44, "HP", ALC245_FIXUP_CS35L41_SPI_2_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8b45, "HP", ALC245_FIXUP_CS35L41_SPI_2_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8b46, "HP", ALC245_FIXUP_CS35L41_SPI_2_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8b47, "HP", ALC245_FIXUP_CS35L41_SPI_2_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8b59, "HP Elite mt645 G7 Mobile Thin Client U89", ALC236_FIXUP_HP_MUTE_LED_MICMUTE_VREF),
	SND_PCI_QUIRK(0x103c, 0x8b5d, "HP", ALC236_FIXUP_HP_MUTE_LED_MICMUTE_VREF),
	SND_PCI_QUIRK(0x103c, 0x8b5e, "HP", ALC236_FIXUP_HP_MUTE_LED_MICMUTE_VREF),
	SND_PCI_QUIRK(0x103c, 0x8b63, "HP Elite Dragonfly 13.5 inch G4", ALC245_FIXUP_CS35L41_SPI_4_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8b65, "HP ProBook 455 15.6 inch G10 Notebook PC", ALC236_FIXUP_HP_MUTE_LED_MICMUTE_VREF),
	SND_PCI_QUIRK(0x103c, 0x8b66, "HP", ALC236_FIXUP_HP_MUTE_LED_MICMUTE_VREF),
	SND_PCI_QUIRK(0x103c, 0x8b70, "HP EliteBook 835 G10", ALC287_FIXUP_CS35L41_I2C_2_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8b72, "HP EliteBook 845 G10", ALC287_FIXUP_CS35L41_I2C_2_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8b74, "HP EliteBook 845W G10", ALC287_FIXUP_CS35L41_I2C_2_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8b77, "HP ElieBook 865 G10", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x103c, 0x8b7a, "HP", ALC236_FIXUP_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8b7d, "HP", ALC236_FIXUP_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8b87, "HP", ALC236_FIXUP_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8b8a, "HP", ALC236_FIXUP_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8b8b, "HP", ALC236_FIXUP_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8b8d, "HP", ALC236_FIXUP_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8b8f, "HP", ALC245_FIXUP_CS35L41_SPI_4_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8b92, "HP", ALC245_FIXUP_CS35L41_SPI_2_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8b96, "HP", ALC236_FIXUP_HP_MUTE_LED_MICMUTE_VREF),
	SND_PCI_QUIRK(0x103c, 0x8b97, "HP", ALC236_FIXUP_HP_MUTE_LED_MICMUTE_VREF),
	SND_PCI_QUIRK(0x103c, 0x8bdd, "HP Envy 17", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x103c, 0x8bde, "HP Envy 17", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x103c, 0x8bdf, "HP Envy 15", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x103c, 0x8be0, "HP Envy 15", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x103c, 0x8be1, "HP Envy 15", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x103c, 0x8be2, "HP Envy 15", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x103c, 0x8be3, "HP Envy 15", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x103c, 0x8be5, "HP Envy 16", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x103c, 0x8be6, "HP Envy 16", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x103c, 0x8be7, "HP Envy 17", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x103c, 0x8be8, "HP Envy 17", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x103c, 0x8be9, "HP Envy 15", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x103c, 0x8bf0, "HP", ALC236_FIXUP_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8c15, "HP Spectre x360 2-in-1 Laptop 14-eu0xxx", ALC245_FIXUP_HP_SPECTRE_X360_EU0XXX),
	SND_PCI_QUIRK(0x103c, 0x8c16, "HP Spectre 16", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x103c, 0x8c17, "HP Spectre 16", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x103c, 0x8c46, "HP EliteBook 830 G11", ALC245_FIXUP_CS35L41_SPI_2_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8c47, "HP EliteBook 840 G11", ALC245_FIXUP_CS35L41_SPI_2_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8c48, "HP EliteBook 860 G11", ALC245_FIXUP_CS35L41_SPI_2_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8c49, "HP Elite x360 830 2-in-1 G11", ALC245_FIXUP_CS35L41_SPI_2_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8c4f, "HP Envy 15", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x103c, 0x8c50, "HP Envy 17", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x103c, 0x8c51, "HP Envy 17", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x103c, 0x8c52, "HP EliteBook 1040 G11", ALC245_FIXUP_CS35L56_SPI_4_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8c53, "HP Elite x360 1040 2-in-1 G11", ALC245_FIXUP_CS35L56_SPI_4_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8c66, "HP Envy 16", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x103c, 0x8c67, "HP Envy 17", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x103c, 0x8c68, "HP Envy 17", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x103c, 0x8c6a, "HP Envy 16", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x103c, 0x8c70, "HP EliteBook 835 G11", ALC287_FIXUP_CS35L41_I2C_2_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8c71, "HP EliteBook 845 G11", ALC287_FIXUP_CS35L41_I2C_2_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8c72, "HP EliteBook 865 G11", ALC287_FIXUP_CS35L41_I2C_2_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8c8a, "HP EliteBook 630", ALC236_FIXUP_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8c8c, "HP EliteBook 660", ALC236_FIXUP_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8c90, "HP EliteBook 640", ALC236_FIXUP_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8c91, "HP EliteBook 660", ALC236_FIXUP_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8c96, "HP", ALC236_FIXUP_HP_MUTE_LED_MICMUTE_VREF),
	SND_PCI_QUIRK(0x103c, 0x8c97, "HP ZBook", ALC236_FIXUP_HP_MUTE_LED_MICMUTE_VREF),
	SND_PCI_QUIRK(0x103c, 0x8ca1, "HP ZBook Power", ALC236_FIXUP_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8ca2, "HP ZBook Power", ALC236_FIXUP_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8ca4, "HP ZBook Fury", ALC245_FIXUP_CS35L41_SPI_2_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8ca7, "HP ZBook Fury", ALC245_FIXUP_CS35L41_SPI_2_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8cdd, "HP Spectre", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x103c, 0x8cde, "HP Spectre", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x103c, 0x8cdf, "HP SnowWhite", ALC287_FIXUP_CS35L41_I2C_2_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8ce0, "HP SnowWhite", ALC287_FIXUP_CS35L41_I2C_2_HP_GPIO_LED),
	SND_PCI_QUIRK(0x103c, 0x8cf5, "HP ZBook Studio 16", ALC245_FIXUP_CS35L41_SPI_4_HP_GPIO_LED),
	SND_PCI_QUIRK(0x1043, 0x103e, "ASUS X540SA", ALC256_FIXUP_ASUS_MIC),
	SND_PCI_QUIRK(0x1043, 0x103f, "ASUS TX300", ALC282_FIXUP_ASUS_TX300),
	SND_PCI_QUIRK(0x1043, 0x106d, "Asus K53BE", ALC269_FIXUP_LIMIT_INT_MIC_BOOST),
	SND_PCI_QUIRK(0x1043, 0x10a1, "ASUS UX391UA", ALC294_FIXUP_ASUS_SPK),
	SND_PCI_QUIRK(0x1043, 0x10c0, "ASUS X540SA", ALC256_FIXUP_ASUS_MIC),
	SND_PCI_QUIRK(0x1043, 0x10d0, "ASUS X540LA/X540LJ", ALC255_FIXUP_ASUS_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1043, 0x10d3, "ASUS K6500ZC", ALC294_FIXUP_ASUS_SPK),
	SND_PCI_QUIRK(0x1043, 0x115d, "Asus 1015E", ALC269_FIXUP_LIMIT_INT_MIC_BOOST),
	SND_PCI_QUIRK(0x1043, 0x11c0, "ASUS X556UR", ALC255_FIXUP_ASUS_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1043, 0x125e, "ASUS Q524UQK", ALC255_FIXUP_ASUS_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1043, 0x1271, "ASUS X430UN", ALC256_FIXUP_ASUS_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1043, 0x1290, "ASUS X441SA", ALC233_FIXUP_EAPD_COEF_AND_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1043, 0x12a0, "ASUS X441UV", ALC233_FIXUP_EAPD_COEF_AND_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1043, 0x12a3, "Asus N7691ZM", ALC269_FIXUP_ASUS_N7601ZM),
	SND_PCI_QUIRK(0x1043, 0x12af, "ASUS UX582ZS", ALC245_FIXUP_CS35L41_SPI_2),
	SND_PCI_QUIRK(0x1043, 0x12e0, "ASUS X541SA", ALC256_FIXUP_ASUS_MIC),
	SND_PCI_QUIRK(0x1043, 0x12f0, "ASUS X541UV", ALC256_FIXUP_ASUS_MIC),
	SND_PCI_QUIRK(0x1043, 0x1313, "Asus K42JZ", ALC269VB_FIXUP_ASUS_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1043, 0x13b0, "ASUS Z550SA", ALC256_FIXUP_ASUS_MIC),
	SND_PCI_QUIRK(0x1043, 0x1427, "Asus Zenbook UX31E", ALC269VB_FIXUP_ASUS_ZENBOOK),
	SND_PCI_QUIRK(0x1043, 0x1433, "ASUS GX650PY/PZ/PV/PU/PYV/PZV/PIV/PVV", ALC285_FIXUP_ASUS_I2C_HEADSET_MIC),
	SND_PCI_QUIRK(0x1043, 0x1463, "Asus GA402X/GA402N", ALC285_FIXUP_ASUS_I2C_HEADSET_MIC),
	SND_PCI_QUIRK(0x1043, 0x1473, "ASUS GU604VI/VC/VE/VG/VJ/VQ/VU/VV/VY/VZ", ALC285_FIXUP_ASUS_HEADSET_MIC),
	SND_PCI_QUIRK(0x1043, 0x1483, "ASUS GU603VQ/VU/VV/VJ/VI", ALC285_FIXUP_ASUS_HEADSET_MIC),
	SND_PCI_QUIRK(0x1043, 0x1493, "ASUS GV601VV/VU/VJ/VQ/VI", ALC285_FIXUP_ASUS_HEADSET_MIC),
	SND_PCI_QUIRK(0x1043, 0x14d3, "ASUS G614JY/JZ/JG", ALC245_FIXUP_CS35L41_SPI_2),
	SND_PCI_QUIRK(0x1043, 0x14e3, "ASUS G513PI/PU/PV", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x1043, 0x1503, "ASUS G733PY/PZ/PZV/PYV", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x1043, 0x1517, "Asus Zenbook UX31A", ALC269VB_FIXUP_ASUS_ZENBOOK_UX31A),
	SND_PCI_QUIRK(0x1043, 0x1533, "ASUS GV302XA/XJ/XQ/XU/XV/XI", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x1043, 0x1573, "ASUS GZ301VV/VQ/VU/VJ/VA/VC/VE/VVC/VQC/VUC/VJC/VEC/VCC", ALC285_FIXUP_ASUS_HEADSET_MIC),
	SND_PCI_QUIRK(0x1043, 0x1662, "ASUS GV301QH", ALC294_FIXUP_ASUS_DUAL_SPK),
	SND_PCI_QUIRK(0x1043, 0x1663, "ASUS GU603ZI/ZJ/ZQ/ZU/ZV", ALC285_FIXUP_ASUS_HEADSET_MIC),
	SND_PCI_QUIRK(0x1043, 0x1683, "ASUS UM3402YAR", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x1043, 0x16a3, "ASUS UX3402VA", ALC245_FIXUP_CS35L41_SPI_2),
	SND_PCI_QUIRK(0x1043, 0x16b2, "ASUS GU603", ALC289_FIXUP_ASUS_GA401),
	SND_PCI_QUIRK(0x1043, 0x16d3, "ASUS UX5304VA", ALC245_FIXUP_CS35L41_SPI_2),
	SND_PCI_QUIRK(0x1043, 0x16e3, "ASUS UX50", ALC269_FIXUP_STEREO_DMIC),
	SND_PCI_QUIRK(0x1043, 0x16f3, "ASUS UX7602VI/BZ", ALC245_FIXUP_CS35L41_SPI_2),
	SND_PCI_QUIRK(0x1043, 0x1740, "ASUS UX430UA", ALC295_FIXUP_ASUS_DACS),
	SND_PCI_QUIRK(0x1043, 0x17d1, "ASUS UX431FL", ALC294_FIXUP_ASUS_DUAL_SPK),
	SND_PCI_QUIRK(0x1043, 0x17f3, "ROG Ally NR2301L/X", ALC294_FIXUP_ASUS_ALLY),
	SND_PCI_QUIRK(0x1043, 0x1863, "ASUS UX6404VI/VV", ALC245_FIXUP_CS35L41_SPI_2),
	SND_PCI_QUIRK(0x1043, 0x1881, "ASUS Zephyrus S/M", ALC294_FIXUP_ASUS_GX502_PINS),
	SND_PCI_QUIRK(0x1043, 0x18b1, "Asus MJ401TA", ALC256_FIXUP_ASUS_HEADSET_MIC),
	SND_PCI_QUIRK(0x1043, 0x18d3, "ASUS UM3504DA", ALC294_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x1043, 0x18f1, "Asus FX505DT", ALC256_FIXUP_ASUS_HEADSET_MIC),
	SND_PCI_QUIRK(0x1043, 0x194e, "ASUS UX563FD", ALC294_FIXUP_ASUS_HPE),
	SND_PCI_QUIRK(0x1043, 0x1970, "ASUS UX550VE", ALC289_FIXUP_ASUS_GA401),
	SND_PCI_QUIRK(0x1043, 0x1982, "ASUS B1400CEPE", ALC256_FIXUP_ASUS_HPE),
	SND_PCI_QUIRK(0x1043, 0x19ce, "ASUS B9450FA", ALC294_FIXUP_ASUS_HPE),
	SND_PCI_QUIRK(0x1043, 0x19e1, "ASUS UX581LV", ALC295_FIXUP_ASUS_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1043, 0x1a13, "Asus G73Jw", ALC269_FIXUP_ASUS_G73JW),
	SND_PCI_QUIRK(0x1043, 0x1a30, "ASUS X705UD", ALC256_FIXUP_ASUS_MIC),
	SND_PCI_QUIRK(0x1043, 0x1a63, "ASUS UX3405MA", ALC245_FIXUP_CS35L41_SPI_2),
	SND_PCI_QUIRK(0x1043, 0x1a83, "ASUS UM5302LA", ALC294_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x1043, 0x1a8f, "ASUS UX582ZS", ALC245_FIXUP_CS35L41_SPI_2),
	SND_PCI_QUIRK(0x1043, 0x1b11, "ASUS UX431DA", ALC294_FIXUP_ASUS_COEF_1B),
	SND_PCI_QUIRK(0x1043, 0x1b13, "ASUS U41SV/GA403U", ALC285_FIXUP_ASUS_GA403U_HEADSET_MIC),
	SND_PCI_QUIRK(0x1043, 0x1b93, "ASUS G614JVR/JIR", ALC245_FIXUP_CS35L41_SPI_2),
	SND_PCI_QUIRK(0x1043, 0x1bbd, "ASUS Z550MA", ALC255_FIXUP_ASUS_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1043, 0x1c03, "ASUS UM3406HA", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x1043, 0x1c23, "Asus X55U", ALC269_FIXUP_LIMIT_INT_MIC_BOOST),
	SND_PCI_QUIRK(0x1043, 0x1c33, "ASUS UX5304MA", ALC245_FIXUP_CS35L41_SPI_2),
	SND_PCI_QUIRK(0x1043, 0x1c43, "ASUS UX8406MA", ALC245_FIXUP_CS35L41_SPI_2),
	SND_PCI_QUIRK(0x1043, 0x1c62, "ASUS GU603", ALC289_FIXUP_ASUS_GA401),
	SND_PCI_QUIRK(0x1043, 0x1c63, "ASUS GU605M", ALC285_FIXUP_ASUS_GU605_SPI_SPEAKER2_TO_DAC1),
	SND_PCI_QUIRK(0x1043, 0x1c92, "ASUS ROG Strix G15", ALC285_FIXUP_ASUS_G533Z_PINS),
	SND_PCI_QUIRK(0x1043, 0x1c9f, "ASUS G614JU/JV/JI", ALC285_FIXUP_ASUS_HEADSET_MIC),
	SND_PCI_QUIRK(0x1043, 0x1caf, "ASUS G634JY/JZ/JI/JG", ALC285_FIXUP_ASUS_SPI_REAR_SPEAKERS),
	SND_PCI_QUIRK(0x1043, 0x1ccd, "ASUS X555UB", ALC256_FIXUP_ASUS_MIC),
	SND_PCI_QUIRK(0x1043, 0x1ccf, "ASUS G814JU/JV/JI", ALC245_FIXUP_CS35L41_SPI_2),
	SND_PCI_QUIRK(0x1043, 0x1cdf, "ASUS G814JY/JZ/JG", ALC245_FIXUP_CS35L41_SPI_2),
	SND_PCI_QUIRK(0x1043, 0x1cef, "ASUS G834JY/JZ/JI/JG", ALC285_FIXUP_ASUS_HEADSET_MIC),
	SND_PCI_QUIRK(0x1043, 0x1d1f, "ASUS G713PI/PU/PV/PVN", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x1043, 0x1d42, "ASUS Zephyrus G14 2022", ALC289_FIXUP_ASUS_GA401),
	SND_PCI_QUIRK(0x1043, 0x1d4e, "ASUS TM420", ALC256_FIXUP_ASUS_HPE),
	SND_PCI_QUIRK(0x1043, 0x1da2, "ASUS UP6502ZA/ZD", ALC245_FIXUP_CS35L41_SPI_2),
	SND_PCI_QUIRK(0x1043, 0x1df3, "ASUS UM5606", ALC285_FIXUP_CS35L56_I2C_4),
	SND_PCI_QUIRK(0x1043, 0x1e02, "ASUS UX3402ZA", ALC245_FIXUP_CS35L41_SPI_2),
	SND_PCI_QUIRK(0x1043, 0x1e11, "ASUS Zephyrus G15", ALC289_FIXUP_ASUS_GA502),
	SND_PCI_QUIRK(0x1043, 0x1e12, "ASUS UM3402", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x1043, 0x1e51, "ASUS Zephyrus M15", ALC294_FIXUP_ASUS_GU502_PINS),
	SND_PCI_QUIRK(0x1043, 0x1e5e, "ASUS ROG Strix G513", ALC294_FIXUP_ASUS_G513_PINS),
	SND_PCI_QUIRK(0x1043, 0x1e63, "ASUS H7606W", ALC285_FIXUP_CS35L56_I2C_2),
	SND_PCI_QUIRK(0x1043, 0x1e83, "ASUS GA605W", ALC285_FIXUP_CS35L56_I2C_2),
	SND_PCI_QUIRK(0x1043, 0x1e8e, "ASUS Zephyrus G15", ALC289_FIXUP_ASUS_GA401),
	SND_PCI_QUIRK(0x1043, 0x1ee2, "ASUS UM6702RA/RC", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x1043, 0x1c52, "ASUS Zephyrus G15 2022", ALC289_FIXUP_ASUS_GA401),
	SND_PCI_QUIRK(0x1043, 0x1f11, "ASUS Zephyrus G14", ALC289_FIXUP_ASUS_GA401),
	SND_PCI_QUIRK(0x1043, 0x1f12, "ASUS UM5302", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x1043, 0x1f1f, "ASUS H7604JI/JV/J3D", ALC245_FIXUP_CS35L41_SPI_2),
	SND_PCI_QUIRK(0x1043, 0x1f62, "ASUS UX7602ZM", ALC245_FIXUP_CS35L41_SPI_2),
	SND_PCI_QUIRK(0x1043, 0x1f92, "ASUS ROG Flow X16", ALC289_FIXUP_ASUS_GA401),
	SND_PCI_QUIRK(0x1043, 0x3030, "ASUS ZN270IE", ALC256_FIXUP_ASUS_AIO_GPIO2),
	SND_PCI_QUIRK(0x1043, 0x3a20, "ASUS G614JZR", ALC245_FIXUP_CS35L41_SPI_2),
	SND_PCI_QUIRK(0x1043, 0x3a30, "ASUS G814JVR/JIR", ALC245_FIXUP_CS35L41_SPI_2),
	SND_PCI_QUIRK(0x1043, 0x3a40, "ASUS G814JZR", ALC245_FIXUP_CS35L41_SPI_2),
	SND_PCI_QUIRK(0x1043, 0x3a50, "ASUS G834JYR/JZR", ALC245_FIXUP_CS35L41_SPI_2),
	SND_PCI_QUIRK(0x1043, 0x3a60, "ASUS G634JYR/JZR", ALC285_FIXUP_ASUS_SPI_REAR_SPEAKERS),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	SND_PCI_QUIRK(0x1043, 0x831a, "ASUS P901", ALC269_FIXUP_STEREO_DMIC),
	SND_PCI_QUIRK(0x1043, 0x834a, "ASUS S101", ALC269_FIXUP_STEREO_DMIC),
	SND_PCI_QUIRK(0x1043, 0x8398, "ASUS P1005", ALC269_FIXUP_STEREO_DMIC),
	SND_PCI_QUIRK(0x1043, 0x83ce, "ASUS P1005", ALC269_FIXUP_STEREO_DMIC),
<<<<<<< HEAD
	SND_PCI_QUIRK(0x104d, 0x9073, "Sony VAIO", ALC275_FIXUP_SONY_VAIO_GPIO2),
	SND_PCI_QUIRK(0x104d, 0x907b, "Sony VAIO", ALC275_FIXUP_SONY_HWEQ),
	SND_PCI_QUIRK(0x104d, 0x9084, "Sony VAIO", ALC275_FIXUP_SONY_HWEQ),
	SND_PCI_QUIRK_VENDOR(0x104d, "Sony VAIO", ALC269_FIXUP_SONY_VAIO),
	SND_PCI_QUIRK(0x1028, 0x0470, "Dell M101z", ALC269_FIXUP_DELL_M101Z),
	SND_PCI_QUIRK_VENDOR(0x1025, "Acer Aspire", ALC271_FIXUP_DMIC),
	SND_PCI_QUIRK(0x10cf, 0x1475, "Lifebook", ALC269_FIXUP_LIFEBOOK),
=======
	SND_PCI_QUIRK(0x1043, 0x8516, "ASUS X101CH", ALC269_FIXUP_ASUS_X101),
	SND_PCI_QUIRK(0x104d, 0x9073, "Sony VAIO", ALC275_FIXUP_SONY_VAIO_GPIO2),
	SND_PCI_QUIRK(0x104d, 0x907b, "Sony VAIO", ALC275_FIXUP_SONY_HWEQ),
	SND_PCI_QUIRK(0x104d, 0x9084, "Sony VAIO", ALC275_FIXUP_SONY_HWEQ),
	SND_PCI_QUIRK(0x104d, 0x9099, "Sony VAIO S13", ALC275_FIXUP_SONY_DISABLE_AAMIX),
	SND_PCI_QUIRK(0x104d, 0x90b5, "Sony VAIO Pro 11", ALC286_FIXUP_SONY_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x104d, 0x90b6, "Sony VAIO Pro 13", ALC286_FIXUP_SONY_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x10cf, 0x1475, "Lifebook", ALC269_FIXUP_LIFEBOOK),
	SND_PCI_QUIRK(0x10cf, 0x159f, "Lifebook E780", ALC269_FIXUP_LIFEBOOK_NO_HP_TO_LINEOUT),
	SND_PCI_QUIRK(0x10cf, 0x15dc, "Lifebook T731", ALC269_FIXUP_LIFEBOOK_HP_PIN),
	SND_PCI_QUIRK(0x10cf, 0x1629, "Lifebook U7x7", ALC255_FIXUP_LIFEBOOK_U7x7_HEADSET_MIC),
	SND_PCI_QUIRK(0x10cf, 0x1757, "Lifebook E752", ALC269_FIXUP_LIFEBOOK_HP_PIN),
	SND_PCI_QUIRK(0x10cf, 0x1845, "Lifebook U904", ALC269_FIXUP_LIFEBOOK_EXTMIC),
	SND_PCI_QUIRK(0x10ec, 0x10f2, "Intel Reference board", ALC700_FIXUP_INTEL_REFERENCE),
	SND_PCI_QUIRK(0x10ec, 0x118c, "Medion EE4254 MD62100", ALC256_FIXUP_MEDION_HEADSET_NO_PRESENCE),
	SND_PCI_QUIRK(0x10ec, 0x1230, "Intel Reference board", ALC295_FIXUP_CHROME_BOOK),
	SND_PCI_QUIRK(0x10ec, 0x124c, "Intel Reference board", ALC295_FIXUP_CHROME_BOOK),
	SND_PCI_QUIRK(0x10ec, 0x1252, "Intel Reference board", ALC295_FIXUP_CHROME_BOOK),
	SND_PCI_QUIRK(0x10ec, 0x1254, "Intel Reference board", ALC295_FIXUP_CHROME_BOOK),
	SND_PCI_QUIRK(0x10ec, 0x12cc, "Intel Reference board", ALC295_FIXUP_CHROME_BOOK),
	SND_PCI_QUIRK(0x10ec, 0x12f6, "Intel Reference board", ALC295_FIXUP_CHROME_BOOK),
	SND_PCI_QUIRK(0x10f7, 0x8338, "Panasonic CF-SZ6", ALC269_FIXUP_ASPIRE_HEADSET_MIC),
	SND_PCI_QUIRK(0x144d, 0xc109, "Samsung Ativ book 9 (NP900X3G)", ALC269_FIXUP_INV_DMIC),
	SND_PCI_QUIRK(0x144d, 0xc169, "Samsung Notebook 9 Pen (NP930SBE-K01US)", ALC298_FIXUP_SAMSUNG_AMP),
	SND_PCI_QUIRK(0x144d, 0xc176, "Samsung Notebook 9 Pro (NP930MBE-K04US)", ALC298_FIXUP_SAMSUNG_AMP),
	SND_PCI_QUIRK(0x144d, 0xc189, "Samsung Galaxy Flex Book (NT950QCG-X716)", ALC298_FIXUP_SAMSUNG_AMP),
	SND_PCI_QUIRK(0x144d, 0xc18a, "Samsung Galaxy Book Ion (NP930XCJ-K01US)", ALC298_FIXUP_SAMSUNG_AMP),
	SND_PCI_QUIRK(0x144d, 0xc1a3, "Samsung Galaxy Book Pro (NP935XDB-KC1SE)", ALC298_FIXUP_SAMSUNG_AMP),
	SND_PCI_QUIRK(0x144d, 0xc1a6, "Samsung Galaxy Book Pro 360 (NP930QBD)", ALC298_FIXUP_SAMSUNG_AMP),
	SND_PCI_QUIRK(0x144d, 0xc740, "Samsung Ativ book 8 (NP870Z5G)", ALC269_FIXUP_ATIV_BOOK_8),
	SND_PCI_QUIRK(0x144d, 0xc812, "Samsung Notebook Pen S (NT950SBE-X58)", ALC298_FIXUP_SAMSUNG_AMP),
	SND_PCI_QUIRK(0x144d, 0xc830, "Samsung Galaxy Book Ion (NT950XCJ-X716A)", ALC298_FIXUP_SAMSUNG_AMP),
	SND_PCI_QUIRK(0x144d, 0xc832, "Samsung Galaxy Book Flex Alpha (NP730QCJ)", ALC256_FIXUP_SAMSUNG_HEADPHONE_VERY_QUIET),
	SND_PCI_QUIRK(0x144d, 0xca03, "Samsung Galaxy Book2 Pro 360 (NP930QED)", ALC298_FIXUP_SAMSUNG_AMP),
	SND_PCI_QUIRK(0x144d, 0xc868, "Samsung Galaxy Book2 Pro (NP930XED)", ALC298_FIXUP_SAMSUNG_AMP),
	SND_PCI_QUIRK(0x1458, 0xfa53, "Gigabyte BXBT-2807", ALC283_FIXUP_HEADSET_MIC),
	SND_PCI_QUIRK(0x1462, 0xb120, "MSI Cubi MS-B120", ALC283_FIXUP_HEADSET_MIC),
	SND_PCI_QUIRK(0x1462, 0xb171, "Cubi N 8GL (MS-B171)", ALC283_FIXUP_HEADSET_MIC),
	SND_PCI_QUIRK(0x152d, 0x1082, "Quanta NL3", ALC269_FIXUP_LIFEBOOK),
	SND_PCI_QUIRK(0x152d, 0x1262, "Huawei NBLB-WAX9N", ALC2XX_FIXUP_HEADSET_MIC),
	SND_PCI_QUIRK(0x1558, 0x0353, "Clevo V35[05]SN[CDE]Q", ALC256_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x1323, "Clevo N130ZU", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x1325, "Clevo N15[01][CW]U", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x1401, "Clevo L140[CZ]U", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x1403, "Clevo N140CU", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x1404, "Clevo N150CU", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x14a1, "Clevo L141MU", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x2624, "Clevo L240TU", ALC256_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x4018, "Clevo NV40M[BE]", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x4019, "Clevo NV40MZ", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x4020, "Clevo NV40MB", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x4041, "Clevo NV4[15]PZ", ALC256_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x40a1, "Clevo NL40GU", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x40c1, "Clevo NL40[CZ]U", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x40d1, "Clevo NL41DU", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x5015, "Clevo NH5[58]H[HJK]Q", ALC256_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x5017, "Clevo NH7[79]H[HJK]Q", ALC256_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x50a3, "Clevo NJ51GU", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x50b3, "Clevo NK50S[BEZ]", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x50b6, "Clevo NK50S5", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x50b8, "Clevo NK50SZ", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x50d5, "Clevo NP50D5", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x50e1, "Clevo NH5[58]HPQ", ALC256_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x50e2, "Clevo NH7[79]HPQ", ALC256_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x50f0, "Clevo NH50A[CDF]", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x50f2, "Clevo NH50E[PR]", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x50f3, "Clevo NH58DPQ", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x50f5, "Clevo NH55EPY", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x50f6, "Clevo NH55DPQ", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x5101, "Clevo S510WU", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x5157, "Clevo W517GU1", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x51a1, "Clevo NS50MU", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x51b1, "Clevo NS50AU", ALC256_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x51b3, "Clevo NS70AU", ALC256_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x5630, "Clevo NP50RNJS", ALC256_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x70a1, "Clevo NB70T[HJK]", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x70b3, "Clevo NK70SB", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x70f2, "Clevo NH79EPY", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x70f3, "Clevo NH77DPQ", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x70f4, "Clevo NH77EPY", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x70f6, "Clevo NH77DPQ-Y", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x7716, "Clevo NS50PU", ALC256_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x7717, "Clevo NS70PU", ALC256_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x7718, "Clevo L140PU", ALC256_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x7724, "Clevo L140AU", ALC256_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x8228, "Clevo NR40BU", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x8520, "Clevo NH50D[CD]", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x8521, "Clevo NH77D[CD]", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x8535, "Clevo NH50D[BE]", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x8536, "Clevo NH79D[BE]", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x8550, "Clevo NH[57][0-9][ER][ACDH]Q", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x8551, "Clevo NH[57][0-9][ER][ACDH]Q", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x8560, "Clevo NH[57][0-9][ER][ACDH]Q", ALC269_FIXUP_HEADSET_MIC),
	SND_PCI_QUIRK(0x1558, 0x8561, "Clevo NH[57][0-9][ER][ACDH]Q", ALC269_FIXUP_HEADSET_MIC),
	SND_PCI_QUIRK(0x1558, 0x8562, "Clevo NH[57][0-9]RZ[Q]", ALC269_FIXUP_DMIC),
	SND_PCI_QUIRK(0x1558, 0x8668, "Clevo NP50B[BE]", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x866d, "Clevo NP5[05]PN[HJK]", ALC256_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x867c, "Clevo NP7[01]PNP", ALC256_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x867d, "Clevo NP7[01]PN[HJK]", ALC256_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x8680, "Clevo NJ50LU", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x8686, "Clevo NH50[CZ]U", ALC256_FIXUP_MIC_NO_PRESENCE_AND_RESUME),
	SND_PCI_QUIRK(0x1558, 0x8a20, "Clevo NH55DCQ-Y", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x8a51, "Clevo NH70RCQ-Y", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x8d50, "Clevo NH55RCQ-M", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x951d, "Clevo N950T[CDF]", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x9600, "Clevo N960K[PR]", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x961d, "Clevo N960S[CDF]", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0x971d, "Clevo N970T[CDF]", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0xa500, "Clevo NL5[03]RU", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0xa600, "Clevo NL50NU", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0xa650, "Clevo NP[567]0SN[CD]", ALC256_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0xa671, "Clevo NP70SN[CDE]", ALC256_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0xb018, "Clevo NP50D[BE]", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0xb019, "Clevo NH77D[BE]Q", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0xb022, "Clevo NH77D[DC][QW]", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0xc018, "Clevo NP50D[BE]", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0xc019, "Clevo NH77D[BE]Q", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1558, 0xc022, "Clevo NH77[DC][QW]", ALC293_FIXUP_SYSTEM76_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x17aa, 0x1036, "Lenovo P520", ALC233_FIXUP_LENOVO_MULTI_CODECS),
	SND_PCI_QUIRK(0x17aa, 0x1048, "ThinkCentre Station", ALC623_FIXUP_LENOVO_THINKSTATION_P340),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	SND_PCI_QUIRK(0x17aa, 0x20f2, "Thinkpad SL410/510", ALC269_FIXUP_SKU_IGNORE),
	SND_PCI_QUIRK(0x17aa, 0x215e, "Thinkpad L512", ALC269_FIXUP_SKU_IGNORE),
	SND_PCI_QUIRK(0x17aa, 0x21b8, "Thinkpad Edge 14", ALC269_FIXUP_SKU_IGNORE),
	SND_PCI_QUIRK(0x17aa, 0x21ca, "Thinkpad L412", ALC269_FIXUP_SKU_IGNORE),
	SND_PCI_QUIRK(0x17aa, 0x21e9, "Thinkpad Edge 15", ALC269_FIXUP_SKU_IGNORE),
<<<<<<< HEAD
	SND_PCI_QUIRK(0x17aa, 0x21f6, "Thinkpad T530", ALC269_FIXUP_LENOVO_DOCK),
	SND_PCI_QUIRK(0x17aa, 0x21fa, "Thinkpad X230", ALC269_FIXUP_LENOVO_DOCK),
	SND_PCI_QUIRK(0x17aa, 0x21f3, "Thinkpad T430", ALC269_FIXUP_LENOVO_DOCK),
	SND_PCI_QUIRK(0x17aa, 0x21fb, "Thinkpad T430s", ALC269_FIXUP_LENOVO_DOCK),
	SND_PCI_QUIRK(0x17aa, 0x2203, "Thinkpad X230 Tablet", ALC269_FIXUP_LENOVO_DOCK),
	SND_PCI_QUIRK(0x17aa, 0x3bf8, "Quanta FL1", ALC269_FIXUP_PCM_44K),
	SND_PCI_QUIRK(0x17aa, 0x9e54, "LENOVO NB", ALC269_FIXUP_LENOVO_EAPD),
=======
	SND_PCI_QUIRK(0x17aa, 0x21f3, "Thinkpad T430", ALC269_FIXUP_LENOVO_DOCK),
	SND_PCI_QUIRK(0x17aa, 0x21f6, "Thinkpad T530", ALC269_FIXUP_LENOVO_DOCK_LIMIT_BOOST),
	SND_PCI_QUIRK(0x17aa, 0x21fa, "Thinkpad X230", ALC269_FIXUP_LENOVO_DOCK),
	SND_PCI_QUIRK(0x17aa, 0x21fb, "Thinkpad T430s", ALC269_FIXUP_LENOVO_DOCK),
	SND_PCI_QUIRK(0x17aa, 0x2203, "Thinkpad X230 Tablet", ALC269_FIXUP_LENOVO_DOCK),
	SND_PCI_QUIRK(0x17aa, 0x2208, "Thinkpad T431s", ALC269_FIXUP_LENOVO_DOCK),
	SND_PCI_QUIRK(0x17aa, 0x220c, "Thinkpad T440s", ALC292_FIXUP_TPT440),
	SND_PCI_QUIRK(0x17aa, 0x220e, "Thinkpad T440p", ALC292_FIXUP_TPT440_DOCK),
	SND_PCI_QUIRK(0x17aa, 0x2210, "Thinkpad T540p", ALC292_FIXUP_TPT440_DOCK),
	SND_PCI_QUIRK(0x17aa, 0x2211, "Thinkpad W541", ALC292_FIXUP_TPT440_DOCK),
	SND_PCI_QUIRK(0x17aa, 0x2212, "Thinkpad T440", ALC292_FIXUP_TPT440_DOCK),
	SND_PCI_QUIRK(0x17aa, 0x2214, "Thinkpad X240", ALC292_FIXUP_TPT440_DOCK),
	SND_PCI_QUIRK(0x17aa, 0x2215, "Thinkpad", ALC269_FIXUP_LIMIT_INT_MIC_BOOST),
	SND_PCI_QUIRK(0x17aa, 0x2218, "Thinkpad X1 Carbon 2nd", ALC292_FIXUP_TPT440_DOCK),
	SND_PCI_QUIRK(0x17aa, 0x2223, "ThinkPad T550", ALC292_FIXUP_TPT440_DOCK),
	SND_PCI_QUIRK(0x17aa, 0x2226, "ThinkPad X250", ALC292_FIXUP_TPT440_DOCK),
	SND_PCI_QUIRK(0x17aa, 0x222d, "Thinkpad", ALC298_FIXUP_TPT470_DOCK),
	SND_PCI_QUIRK(0x17aa, 0x222e, "Thinkpad", ALC298_FIXUP_TPT470_DOCK),
	SND_PCI_QUIRK(0x17aa, 0x2231, "Thinkpad T560", ALC292_FIXUP_TPT460),
	SND_PCI_QUIRK(0x17aa, 0x2233, "Thinkpad", ALC292_FIXUP_TPT460),
	SND_PCI_QUIRK(0x17aa, 0x2234, "Thinkpad ICE-1", ALC287_FIXUP_TAS2781_I2C),
	SND_PCI_QUIRK(0x17aa, 0x2245, "Thinkpad T470", ALC298_FIXUP_TPT470_DOCK),
	SND_PCI_QUIRK(0x17aa, 0x2246, "Thinkpad", ALC298_FIXUP_TPT470_DOCK),
	SND_PCI_QUIRK(0x17aa, 0x2247, "Thinkpad", ALC298_FIXUP_TPT470_DOCK),
	SND_PCI_QUIRK(0x17aa, 0x2249, "Thinkpad", ALC292_FIXUP_TPT460),
	SND_PCI_QUIRK(0x17aa, 0x224b, "Thinkpad", ALC298_FIXUP_TPT470_DOCK),
	SND_PCI_QUIRK(0x17aa, 0x224c, "Thinkpad", ALC298_FIXUP_TPT470_DOCK),
	SND_PCI_QUIRK(0x17aa, 0x224d, "Thinkpad", ALC298_FIXUP_TPT470_DOCK),
	SND_PCI_QUIRK(0x17aa, 0x225d, "Thinkpad T480", ALC269_FIXUP_LIMIT_INT_MIC_BOOST),
	SND_PCI_QUIRK(0x17aa, 0x2292, "Thinkpad X1 Carbon 7th", ALC285_FIXUP_THINKPAD_HEADSET_JACK),
	SND_PCI_QUIRK(0x17aa, 0x22be, "Thinkpad X1 Carbon 8th", ALC285_FIXUP_THINKPAD_HEADSET_JACK),
	SND_PCI_QUIRK(0x17aa, 0x22c1, "Thinkpad P1 Gen 3", ALC285_FIXUP_THINKPAD_NO_BASS_SPK_HEADSET_JACK),
	SND_PCI_QUIRK(0x17aa, 0x22c2, "Thinkpad X1 Extreme Gen 3", ALC285_FIXUP_THINKPAD_NO_BASS_SPK_HEADSET_JACK),
	SND_PCI_QUIRK(0x17aa, 0x22f1, "Thinkpad", ALC287_FIXUP_MG_RTKC_CSAMP_CS35L41_I2C_THINKPAD),
	SND_PCI_QUIRK(0x17aa, 0x22f2, "Thinkpad", ALC287_FIXUP_MG_RTKC_CSAMP_CS35L41_I2C_THINKPAD),
	SND_PCI_QUIRK(0x17aa, 0x22f3, "Thinkpad", ALC287_FIXUP_MG_RTKC_CSAMP_CS35L41_I2C_THINKPAD),
	SND_PCI_QUIRK(0x17aa, 0x2316, "Thinkpad P1 Gen 6", ALC287_FIXUP_MG_RTKC_CSAMP_CS35L41_I2C_THINKPAD),
	SND_PCI_QUIRK(0x17aa, 0x2317, "Thinkpad P1 Gen 6", ALC287_FIXUP_MG_RTKC_CSAMP_CS35L41_I2C_THINKPAD),
	SND_PCI_QUIRK(0x17aa, 0x2318, "Thinkpad Z13 Gen2", ALC287_FIXUP_MG_RTKC_CSAMP_CS35L41_I2C_THINKPAD),
	SND_PCI_QUIRK(0x17aa, 0x2319, "Thinkpad Z16 Gen2", ALC287_FIXUP_MG_RTKC_CSAMP_CS35L41_I2C_THINKPAD),
	SND_PCI_QUIRK(0x17aa, 0x231a, "Thinkpad Z16 Gen2", ALC287_FIXUP_MG_RTKC_CSAMP_CS35L41_I2C_THINKPAD),
	SND_PCI_QUIRK(0x17aa, 0x231e, "Thinkpad", ALC287_FIXUP_LENOVO_THKPAD_WH_ALC1318),
	SND_PCI_QUIRK(0x17aa, 0x231f, "Thinkpad", ALC287_FIXUP_LENOVO_THKPAD_WH_ALC1318),
	SND_PCI_QUIRK(0x17aa, 0x30bb, "ThinkCentre AIO", ALC233_FIXUP_LENOVO_LINE2_MIC_HOTKEY),
	SND_PCI_QUIRK(0x17aa, 0x30e2, "ThinkCentre AIO", ALC233_FIXUP_LENOVO_LINE2_MIC_HOTKEY),
	SND_PCI_QUIRK(0x17aa, 0x310c, "ThinkCentre Station", ALC294_FIXUP_LENOVO_MIC_LOCATION),
	SND_PCI_QUIRK(0x17aa, 0x3111, "ThinkCentre Station", ALC294_FIXUP_LENOVO_MIC_LOCATION),
	SND_PCI_QUIRK(0x17aa, 0x312a, "ThinkCentre Station", ALC294_FIXUP_LENOVO_MIC_LOCATION),
	SND_PCI_QUIRK(0x17aa, 0x312f, "ThinkCentre Station", ALC294_FIXUP_LENOVO_MIC_LOCATION),
	SND_PCI_QUIRK(0x17aa, 0x313c, "ThinkCentre Station", ALC294_FIXUP_LENOVO_MIC_LOCATION),
	SND_PCI_QUIRK(0x17aa, 0x3151, "ThinkCentre Station", ALC283_FIXUP_HEADSET_MIC),
	SND_PCI_QUIRK(0x17aa, 0x3176, "ThinkCentre Station", ALC283_FIXUP_HEADSET_MIC),
	SND_PCI_QUIRK(0x17aa, 0x3178, "ThinkCentre Station", ALC283_FIXUP_HEADSET_MIC),
	SND_PCI_QUIRK(0x17aa, 0x31af, "ThinkCentre Station", ALC623_FIXUP_LENOVO_THINKSTATION_P340),
	SND_PCI_QUIRK(0x17aa, 0x334b, "Lenovo ThinkCentre M70 Gen5", ALC283_FIXUP_HEADSET_MIC),
	SND_PCI_QUIRK(0x17aa, 0x3801, "Lenovo Yoga9 14IAP7", ALC287_FIXUP_YOGA9_14IAP7_BASS_SPK_PIN),
	SND_PCI_QUIRK(0x17aa, 0x3802, "Lenovo Yoga Pro 9 14IRP8 / DuetITL 2021", ALC287_FIXUP_LENOVO_14IRP8_DUETITL),
	SND_PCI_QUIRK(0x17aa, 0x3813, "Legion 7i 15IMHG05", ALC287_FIXUP_LEGION_15IMHG05_SPEAKERS),
	SND_PCI_QUIRK(0x17aa, 0x3818, "Lenovo C940 / Yoga Duet 7", ALC298_FIXUP_LENOVO_C940_DUET7),
	SND_PCI_QUIRK(0x17aa, 0x3819, "Lenovo 13s Gen2 ITL", ALC287_FIXUP_13S_GEN2_SPEAKERS),
	SND_PCI_QUIRK(0x17aa, 0x3820, "Yoga Duet 7 13ITL6", ALC287_FIXUP_YOGA7_14ITL_SPEAKERS),
	SND_PCI_QUIRK(0x17aa, 0x3824, "Legion Y9000X 2020", ALC285_FIXUP_LEGION_Y9000X_SPEAKERS),
	SND_PCI_QUIRK(0x17aa, 0x3827, "Ideapad S740", ALC285_FIXUP_IDEAPAD_S740_COEF),
	SND_PCI_QUIRK(0x17aa, 0x3834, "Lenovo IdeaPad Slim 9i 14ITL5", ALC287_FIXUP_YOGA7_14ITL_SPEAKERS),
	SND_PCI_QUIRK(0x17aa, 0x383d, "Legion Y9000X 2019", ALC285_FIXUP_LEGION_Y9000X_SPEAKERS),
	SND_PCI_QUIRK(0x17aa, 0x3843, "Yoga 9i", ALC287_FIXUP_IDEAPAD_BASS_SPK_AMP),
	SND_PCI_QUIRK(0x17aa, 0x3847, "Legion 7 16ACHG6", ALC287_FIXUP_LEGION_16ACHG6),
	SND_PCI_QUIRK(0x17aa, 0x384a, "Lenovo Yoga 7 15ITL5", ALC287_FIXUP_YOGA7_14ITL_SPEAKERS),
	SND_PCI_QUIRK(0x17aa, 0x3852, "Lenovo Yoga 7 14ITL5", ALC287_FIXUP_YOGA7_14ITL_SPEAKERS),
	SND_PCI_QUIRK(0x17aa, 0x3853, "Lenovo Yoga 7 15ITL5", ALC287_FIXUP_YOGA7_14ITL_SPEAKERS),
	SND_PCI_QUIRK(0x17aa, 0x3855, "Legion 7 16ITHG6", ALC287_FIXUP_LEGION_16ITHG6),
	SND_PCI_QUIRK(0x17aa, 0x3869, "Lenovo Yoga7 14IAL7", ALC287_FIXUP_YOGA9_14IAP7_BASS_SPK_PIN),
	SND_PCI_QUIRK(0x17aa, 0x386f, "Legion Pro 7/7i", ALC287_FIXUP_LENOVO_LEGION_7),
	SND_PCI_QUIRK(0x17aa, 0x3870, "Lenovo Yoga 7 14ARB7", ALC287_FIXUP_YOGA7_14ARB7_I2C),
	SND_PCI_QUIRK(0x17aa, 0x3877, "Lenovo Legion 7 Slim 16ARHA7", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x17aa, 0x3878, "Lenovo Legion 7 Slim 16ARHA7", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x17aa, 0x387d, "Yoga S780-16 pro Quad AAC", ALC287_FIXUP_TAS2781_I2C),
	SND_PCI_QUIRK(0x17aa, 0x387e, "Yoga S780-16 pro Quad YC", ALC287_FIXUP_TAS2781_I2C),
	SND_PCI_QUIRK(0x17aa, 0x3881, "YB9 dual power mode2 YC", ALC287_FIXUP_TAS2781_I2C),
	SND_PCI_QUIRK(0x17aa, 0x3882, "Lenovo Yoga Pro 7 14APH8", ALC287_FIXUP_YOGA9_14IAP7_BASS_SPK_PIN),
	SND_PCI_QUIRK(0x17aa, 0x3884, "Y780 YG DUAL", ALC287_FIXUP_TAS2781_I2C),
	SND_PCI_QUIRK(0x17aa, 0x3886, "Y780 VECO DUAL", ALC287_FIXUP_TAS2781_I2C),
	SND_PCI_QUIRK(0x17aa, 0x38a7, "Y780P AMD YG dual", ALC287_FIXUP_TAS2781_I2C),
	SND_PCI_QUIRK(0x17aa, 0x38a8, "Y780P AMD VECO dual", ALC287_FIXUP_TAS2781_I2C),
	SND_PCI_QUIRK(0x17aa, 0x38a9, "Thinkbook 16P", ALC287_FIXUP_MG_RTKC_CSAMP_CS35L41_I2C_THINKPAD),
	SND_PCI_QUIRK(0x17aa, 0x38ab, "Thinkbook 16P", ALC287_FIXUP_MG_RTKC_CSAMP_CS35L41_I2C_THINKPAD),
	SND_PCI_QUIRK(0x17aa, 0x38b4, "Legion Slim 7 16IRH8", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x17aa, 0x38b5, "Legion Slim 7 16IRH8", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x17aa, 0x38b6, "Legion Slim 7 16APH8", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x17aa, 0x38b7, "Legion Slim 7 16APH8", ALC287_FIXUP_CS35L41_I2C_2),
	SND_PCI_QUIRK(0x17aa, 0x38ba, "Yoga S780-14.5 Air AMD quad YC", ALC287_FIXUP_TAS2781_I2C),
	SND_PCI_QUIRK(0x17aa, 0x38bb, "Yoga S780-14.5 Air AMD quad AAC", ALC287_FIXUP_TAS2781_I2C),
	SND_PCI_QUIRK(0x17aa, 0x38be, "Yoga S980-14.5 proX YC Dual", ALC287_FIXUP_TAS2781_I2C),
	SND_PCI_QUIRK(0x17aa, 0x38bf, "Yoga S980-14.5 proX LX Dual", ALC287_FIXUP_TAS2781_I2C),
	SND_PCI_QUIRK(0x17aa, 0x38c3, "Y980 DUAL", ALC287_FIXUP_TAS2781_I2C),
	SND_PCI_QUIRK(0x17aa, 0x38cb, "Y790 YG DUAL", ALC287_FIXUP_TAS2781_I2C),
	SND_PCI_QUIRK(0x17aa, 0x38cd, "Y790 VECO DUAL", ALC287_FIXUP_TAS2781_I2C),
	SND_PCI_QUIRK(0x17aa, 0x38d2, "Lenovo Yoga 9 14IMH9", ALC287_FIXUP_YOGA9_14IMH9_BASS_SPK_PIN),
	SND_PCI_QUIRK(0x17aa, 0x38d7, "Lenovo Yoga 9 14IMH9", ALC287_FIXUP_YOGA9_14IMH9_BASS_SPK_PIN),
	SND_PCI_QUIRK(0x17aa, 0x3902, "Lenovo E50-80", ALC269_FIXUP_DMIC_THINKPAD_ACPI),
	SND_PCI_QUIRK(0x17aa, 0x3977, "IdeaPad S210", ALC283_FIXUP_INT_MIC),
	SND_PCI_QUIRK(0x17aa, 0x3978, "Lenovo B50-70", ALC269_FIXUP_DMIC_THINKPAD_ACPI),
	SND_PCI_QUIRK(0x17aa, 0x3bf8, "Quanta FL1", ALC269_FIXUP_PCM_44K),
	SND_PCI_QUIRK(0x17aa, 0x5013, "Thinkpad", ALC269_FIXUP_LIMIT_INT_MIC_BOOST),
	SND_PCI_QUIRK(0x17aa, 0x501a, "Thinkpad", ALC283_FIXUP_INT_MIC),
	SND_PCI_QUIRK(0x17aa, 0x501e, "Thinkpad L440", ALC292_FIXUP_TPT440_DOCK),
	SND_PCI_QUIRK(0x17aa, 0x5026, "Thinkpad", ALC269_FIXUP_LIMIT_INT_MIC_BOOST),
	SND_PCI_QUIRK(0x17aa, 0x5034, "Thinkpad T450", ALC292_FIXUP_TPT440_DOCK),
	SND_PCI_QUIRK(0x17aa, 0x5036, "Thinkpad T450s", ALC292_FIXUP_TPT440_DOCK),
	SND_PCI_QUIRK(0x17aa, 0x503c, "Thinkpad L450", ALC292_FIXUP_TPT440_DOCK),
	SND_PCI_QUIRK(0x17aa, 0x504a, "ThinkPad X260", ALC292_FIXUP_TPT440_DOCK),
	SND_PCI_QUIRK(0x17aa, 0x504b, "Thinkpad", ALC293_FIXUP_LENOVO_SPK_NOISE),
	SND_PCI_QUIRK(0x17aa, 0x5050, "Thinkpad T560p", ALC292_FIXUP_TPT460),
	SND_PCI_QUIRK(0x17aa, 0x5051, "Thinkpad L460", ALC292_FIXUP_TPT460),
	SND_PCI_QUIRK(0x17aa, 0x5053, "Thinkpad T460", ALC292_FIXUP_TPT460),
	SND_PCI_QUIRK(0x17aa, 0x505d, "Thinkpad", ALC298_FIXUP_TPT470_DOCK),
	SND_PCI_QUIRK(0x17aa, 0x505f, "Thinkpad", ALC298_FIXUP_TPT470_DOCK),
	SND_PCI_QUIRK(0x17aa, 0x5062, "Thinkpad", ALC298_FIXUP_TPT470_DOCK),
	SND_PCI_QUIRK(0x17aa, 0x508b, "Thinkpad X12 Gen 1", ALC287_FIXUP_LEGION_15IMHG05_SPEAKERS),
	SND_PCI_QUIRK(0x17aa, 0x5109, "Thinkpad", ALC269_FIXUP_LIMIT_INT_MIC_BOOST),
	SND_PCI_QUIRK(0x17aa, 0x511e, "Thinkpad", ALC298_FIXUP_TPT470_DOCK),
	SND_PCI_QUIRK(0x17aa, 0x511f, "Thinkpad", ALC298_FIXUP_TPT470_DOCK),
	SND_PCI_QUIRK(0x17aa, 0x9e54, "LENOVO NB", ALC269_FIXUP_LENOVO_EAPD),
	SND_PCI_QUIRK(0x17aa, 0x9e56, "Lenovo ZhaoYang CF4620Z", ALC286_FIXUP_SONY_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1849, 0x1233, "ASRock NUC Box 1100", ALC233_FIXUP_NO_AUDIO_JACK),
	SND_PCI_QUIRK(0x1849, 0xa233, "Positivo Master C6300", ALC269_FIXUP_HEADSET_MIC),
	SND_PCI_QUIRK(0x1854, 0x0440, "LG CQ6", ALC256_FIXUP_HEADPHONE_AMP_VOL),
	SND_PCI_QUIRK(0x1854, 0x0441, "LG CQ6 AIO", ALC256_FIXUP_HEADPHONE_AMP_VOL),
	SND_PCI_QUIRK(0x19e5, 0x3204, "Huawei MACH-WX9", ALC256_FIXUP_HUAWEI_MACH_WX9_PINS),
	SND_PCI_QUIRK(0x19e5, 0x320f, "Huawei WRT-WX9 ", ALC256_FIXUP_ASUS_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1b35, 0x1235, "CZC B20", ALC269_FIXUP_CZC_B20),
	SND_PCI_QUIRK(0x1b35, 0x1236, "CZC TMI", ALC269_FIXUP_CZC_TMI),
	SND_PCI_QUIRK(0x1b35, 0x1237, "CZC L101", ALC269_FIXUP_CZC_L101),
	SND_PCI_QUIRK(0x1b7d, 0xa831, "Ordissimo EVE2 ", ALC269VB_FIXUP_ORDISSIMO_EVE2), /* Also known as Malata PC-B1303 */
	SND_PCI_QUIRK(0x1c06, 0x2013, "Lemote A1802", ALC269_FIXUP_LEMOTE_A1802),
	SND_PCI_QUIRK(0x1c06, 0x2015, "Lemote A190X", ALC269_FIXUP_LEMOTE_A190X),
	SND_PCI_QUIRK(0x1c6c, 0x1251, "Positivo N14KP6-TG", ALC288_FIXUP_DELL1_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1d05, 0x1132, "TongFang PHxTxX1", ALC256_FIXUP_SET_COEF_DEFAULTS),
	SND_PCI_QUIRK(0x1d05, 0x1096, "TongFang GMxMRxx", ALC269_FIXUP_NO_SHUTUP),
	SND_PCI_QUIRK(0x1d05, 0x1100, "TongFang GKxNRxx", ALC269_FIXUP_NO_SHUTUP),
	SND_PCI_QUIRK(0x1d05, 0x1111, "TongFang GMxZGxx", ALC269_FIXUP_NO_SHUTUP),
	SND_PCI_QUIRK(0x1d05, 0x1119, "TongFang GMxZGxx", ALC269_FIXUP_NO_SHUTUP),
	SND_PCI_QUIRK(0x1d05, 0x1129, "TongFang GMxZGxx", ALC269_FIXUP_NO_SHUTUP),
	SND_PCI_QUIRK(0x1d05, 0x1147, "TongFang GMxTGxx", ALC269_FIXUP_NO_SHUTUP),
	SND_PCI_QUIRK(0x1d05, 0x115c, "TongFang GMxTGxx", ALC269_FIXUP_NO_SHUTUP),
	SND_PCI_QUIRK(0x1d05, 0x121b, "TongFang GMxAGxx", ALC269_FIXUP_NO_SHUTUP),
	SND_PCI_QUIRK(0x1d05, 0x1387, "TongFang GMxIXxx", ALC2XX_FIXUP_HEADSET_MIC),
	SND_PCI_QUIRK(0x1d17, 0x3288, "Haier Boyue G42", ALC269VC_FIXUP_ACER_VCOPPERBOX_PINS),
	SND_PCI_QUIRK(0x1d72, 0x1602, "RedmiBook", ALC255_FIXUP_XIAOMI_HEADSET_MIC),
	SND_PCI_QUIRK(0x1d72, 0x1701, "XiaomiNotebook Pro", ALC298_FIXUP_DELL1_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1d72, 0x1901, "RedmiBook 14", ALC256_FIXUP_ASUS_HEADSET_MIC),
	SND_PCI_QUIRK(0x1d72, 0x1945, "Redmi G", ALC256_FIXUP_ASUS_HEADSET_MIC),
	SND_PCI_QUIRK(0x1d72, 0x1947, "RedmiBook Air", ALC255_FIXUP_XIAOMI_HEADSET_MIC),
	SND_PCI_QUIRK(0x2782, 0x0232, "CHUWI CoreBook XPro", ALC269VB_FIXUP_CHUWI_COREBOOK_XPRO),
	SND_PCI_QUIRK(0x2782, 0x1707, "Vaio VJFE-ADL", ALC298_FIXUP_SPK_VOLUME),
	SND_PCI_QUIRK(0x8086, 0x2074, "Intel NUC 8", ALC233_FIXUP_INTEL_NUC8_DMIC),
	SND_PCI_QUIRK(0x8086, 0x2080, "Intel NUC 8 Rugged", ALC256_FIXUP_INTEL_NUC8_RUGGED),
	SND_PCI_QUIRK(0x8086, 0x2081, "Intel NUC 10", ALC256_FIXUP_INTEL_NUC10),
	SND_PCI_QUIRK(0x8086, 0x3038, "Intel NUC 13", ALC295_FIXUP_CHROME_BOOK),
	SND_PCI_QUIRK(0xf111, 0x0001, "Framework Laptop", ALC295_FIXUP_FRAMEWORK_LAPTOP_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0xf111, 0x0005, "Framework Laptop", ALC295_FIXUP_FRAMEWORK_LAPTOP_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0xf111, 0x0006, "Framework Laptop", ALC295_FIXUP_FRAMEWORK_LAPTOP_MIC_NO_PRESENCE),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#if 0
	/* Below is a quirk table taken from the old code.
	 * Basically the device should work as is without the fixup table.
	 * If BIOS doesn't give a proper info, enable the corresponding
	 * fixup entry.
	 */
	SND_PCI_QUIRK(0x1043, 0x8330, "ASUS Eeepc P703 P900A",
		      ALC269_FIXUP_AMIC),
	SND_PCI_QUIRK(0x1043, 0x1013, "ASUS N61Da", ALC269_FIXUP_AMIC),
	SND_PCI_QUIRK(0x1043, 0x1143, "ASUS B53f", ALC269_FIXUP_AMIC),
	SND_PCI_QUIRK(0x1043, 0x1133, "ASUS UJ20ft", ALC269_FIXUP_AMIC),
	SND_PCI_QUIRK(0x1043, 0x1183, "ASUS K72DR", ALC269_FIXUP_AMIC),
	SND_PCI_QUIRK(0x1043, 0x11b3, "ASUS K52DR", ALC269_FIXUP_AMIC),
	SND_PCI_QUIRK(0x1043, 0x11e3, "ASUS U33Jc", ALC269_FIXUP_AMIC),
	SND_PCI_QUIRK(0x1043, 0x1273, "ASUS UL80Jt", ALC269_FIXUP_AMIC),
	SND_PCI_QUIRK(0x1043, 0x1283, "ASUS U53Jc", ALC269_FIXUP_AMIC),
	SND_PCI_QUIRK(0x1043, 0x12b3, "ASUS N82JV", ALC269_FIXUP_AMIC),
	SND_PCI_QUIRK(0x1043, 0x12d3, "ASUS N61Jv", ALC269_FIXUP_AMIC),
	SND_PCI_QUIRK(0x1043, 0x13a3, "ASUS UL30Vt", ALC269_FIXUP_AMIC),
	SND_PCI_QUIRK(0x1043, 0x1373, "ASUS G73JX", ALC269_FIXUP_AMIC),
	SND_PCI_QUIRK(0x1043, 0x1383, "ASUS UJ30Jc", ALC269_FIXUP_AMIC),
	SND_PCI_QUIRK(0x1043, 0x13d3, "ASUS N61JA", ALC269_FIXUP_AMIC),
	SND_PCI_QUIRK(0x1043, 0x1413, "ASUS UL50", ALC269_FIXUP_AMIC),
	SND_PCI_QUIRK(0x1043, 0x1443, "ASUS UL30", ALC269_FIXUP_AMIC),
	SND_PCI_QUIRK(0x1043, 0x1453, "ASUS M60Jv", ALC269_FIXUP_AMIC),
	SND_PCI_QUIRK(0x1043, 0x1483, "ASUS UL80", ALC269_FIXUP_AMIC),
	SND_PCI_QUIRK(0x1043, 0x14f3, "ASUS F83Vf", ALC269_FIXUP_AMIC),
	SND_PCI_QUIRK(0x1043, 0x14e3, "ASUS UL20", ALC269_FIXUP_AMIC),
	SND_PCI_QUIRK(0x1043, 0x1513, "ASUS UX30", ALC269_FIXUP_AMIC),
	SND_PCI_QUIRK(0x1043, 0x1593, "ASUS N51Vn", ALC269_FIXUP_AMIC),
	SND_PCI_QUIRK(0x1043, 0x15a3, "ASUS N60Jv", ALC269_FIXUP_AMIC),
	SND_PCI_QUIRK(0x1043, 0x15b3, "ASUS N60Dp", ALC269_FIXUP_AMIC),
	SND_PCI_QUIRK(0x1043, 0x15c3, "ASUS N70De", ALC269_FIXUP_AMIC),
	SND_PCI_QUIRK(0x1043, 0x15e3, "ASUS F83T", ALC269_FIXUP_AMIC),
	SND_PCI_QUIRK(0x1043, 0x1643, "ASUS M60J", ALC269_FIXUP_AMIC),
	SND_PCI_QUIRK(0x1043, 0x1653, "ASUS U50", ALC269_FIXUP_AMIC),
	SND_PCI_QUIRK(0x1043, 0x1693, "ASUS F50N", ALC269_FIXUP_AMIC),
	SND_PCI_QUIRK(0x1043, 0x16a3, "ASUS F5Q", ALC269_FIXUP_AMIC),
	SND_PCI_QUIRK(0x1043, 0x1723, "ASUS P80", ALC269_FIXUP_AMIC),
	SND_PCI_QUIRK(0x1043, 0x1743, "ASUS U80", ALC269_FIXUP_AMIC),
	SND_PCI_QUIRK(0x1043, 0x1773, "ASUS U20A", ALC269_FIXUP_AMIC),
	SND_PCI_QUIRK(0x1043, 0x1883, "ASUS F81Se", ALC269_FIXUP_AMIC),
	SND_PCI_QUIRK(0x152d, 0x1778, "Quanta ON1", ALC269_FIXUP_DMIC),
	SND_PCI_QUIRK(0x17aa, 0x3be9, "Quanta Wistron", ALC269_FIXUP_AMIC),
	SND_PCI_QUIRK(0x17aa, 0x3bf8, "Quanta FL1", ALC269_FIXUP_AMIC),
	SND_PCI_QUIRK(0x17ff, 0x059a, "Quanta EL3", ALC269_FIXUP_DMIC),
	SND_PCI_QUIRK(0x17ff, 0x059b, "Quanta JR1", ALC269_FIXUP_DMIC),
#endif
	{}
};

<<<<<<< HEAD
static const struct alc_model_fixup alc269_fixup_models[] = {
	{.id = ALC269_FIXUP_AMIC, .name = "laptop-amic"},
	{.id = ALC269_FIXUP_DMIC, .name = "laptop-dmic"},
	{.id = ALC269_FIXUP_LENOVO_DOCK, .name = "lenovo-dock"},
	{}
};

=======
static const struct snd_pci_quirk alc269_fixup_vendor_tbl[] = {
	SND_PCI_QUIRK_VENDOR(0x1025, "Acer Aspire", ALC271_FIXUP_DMIC),
	SND_PCI_QUIRK_VENDOR(0x103c, "HP", ALC269_FIXUP_HP_MUTE_LED),
	SND_PCI_QUIRK_VENDOR(0x104d, "Sony VAIO", ALC269_FIXUP_SONY_VAIO),
	SND_PCI_QUIRK_VENDOR(0x17aa, "Thinkpad", ALC269_FIXUP_THINKPAD_ACPI),
	SND_PCI_QUIRK_VENDOR(0x19e5, "Huawei Matebook", ALC255_FIXUP_MIC_MUTE_LED),
	{}
};

static const struct hda_model_fixup alc269_fixup_models[] = {
	{.id = ALC269_FIXUP_AMIC, .name = "laptop-amic"},
	{.id = ALC269_FIXUP_DMIC, .name = "laptop-dmic"},
	{.id = ALC269_FIXUP_STEREO_DMIC, .name = "alc269-dmic"},
	{.id = ALC271_FIXUP_DMIC, .name = "alc271-dmic"},
	{.id = ALC269_FIXUP_INV_DMIC, .name = "inv-dmic"},
	{.id = ALC269_FIXUP_HEADSET_MIC, .name = "headset-mic"},
	{.id = ALC269_FIXUP_HEADSET_MODE, .name = "headset-mode"},
	{.id = ALC269_FIXUP_HEADSET_MODE_NO_HP_MIC, .name = "headset-mode-no-hp-mic"},
	{.id = ALC269_FIXUP_LENOVO_DOCK, .name = "lenovo-dock"},
	{.id = ALC269_FIXUP_LENOVO_DOCK_LIMIT_BOOST, .name = "lenovo-dock-limit-boost"},
	{.id = ALC269_FIXUP_HP_GPIO_LED, .name = "hp-gpio-led"},
	{.id = ALC269_FIXUP_HP_DOCK_GPIO_MIC1_LED, .name = "hp-dock-gpio-mic1-led"},
	{.id = ALC269_FIXUP_DELL1_MIC_NO_PRESENCE, .name = "dell-headset-multi"},
	{.id = ALC269_FIXUP_DELL2_MIC_NO_PRESENCE, .name = "dell-headset-dock"},
	{.id = ALC269_FIXUP_DELL3_MIC_NO_PRESENCE, .name = "dell-headset3"},
	{.id = ALC269_FIXUP_DELL4_MIC_NO_PRESENCE, .name = "dell-headset4"},
	{.id = ALC283_FIXUP_CHROME_BOOK, .name = "alc283-dac-wcaps"},
	{.id = ALC283_FIXUP_SENSE_COMBO_JACK, .name = "alc283-sense-combo"},
	{.id = ALC292_FIXUP_TPT440_DOCK, .name = "tpt440-dock"},
	{.id = ALC292_FIXUP_TPT440, .name = "tpt440"},
	{.id = ALC292_FIXUP_TPT460, .name = "tpt460"},
	{.id = ALC298_FIXUP_TPT470_DOCK_FIX, .name = "tpt470-dock-fix"},
	{.id = ALC298_FIXUP_TPT470_DOCK, .name = "tpt470-dock"},
	{.id = ALC233_FIXUP_LENOVO_MULTI_CODECS, .name = "dual-codecs"},
	{.id = ALC700_FIXUP_INTEL_REFERENCE, .name = "alc700-ref"},
	{.id = ALC269_FIXUP_SONY_VAIO, .name = "vaio"},
	{.id = ALC269_FIXUP_DELL_M101Z, .name = "dell-m101z"},
	{.id = ALC269_FIXUP_ASUS_G73JW, .name = "asus-g73jw"},
	{.id = ALC269_FIXUP_LENOVO_EAPD, .name = "lenovo-eapd"},
	{.id = ALC275_FIXUP_SONY_HWEQ, .name = "sony-hweq"},
	{.id = ALC269_FIXUP_PCM_44K, .name = "pcm44k"},
	{.id = ALC269_FIXUP_LIFEBOOK, .name = "lifebook"},
	{.id = ALC269_FIXUP_LIFEBOOK_EXTMIC, .name = "lifebook-extmic"},
	{.id = ALC269_FIXUP_LIFEBOOK_HP_PIN, .name = "lifebook-hp-pin"},
	{.id = ALC255_FIXUP_LIFEBOOK_U7x7_HEADSET_MIC, .name = "lifebook-u7x7"},
	{.id = ALC269VB_FIXUP_AMIC, .name = "alc269vb-amic"},
	{.id = ALC269VB_FIXUP_DMIC, .name = "alc269vb-dmic"},
	{.id = ALC269_FIXUP_HP_MUTE_LED_MIC1, .name = "hp-mute-led-mic1"},
	{.id = ALC269_FIXUP_HP_MUTE_LED_MIC2, .name = "hp-mute-led-mic2"},
	{.id = ALC269_FIXUP_HP_MUTE_LED_MIC3, .name = "hp-mute-led-mic3"},
	{.id = ALC269_FIXUP_HP_GPIO_MIC1_LED, .name = "hp-gpio-mic1"},
	{.id = ALC269_FIXUP_HP_LINE1_MIC1_LED, .name = "hp-line1-mic1"},
	{.id = ALC269_FIXUP_NO_SHUTUP, .name = "noshutup"},
	{.id = ALC286_FIXUP_SONY_MIC_NO_PRESENCE, .name = "sony-nomic"},
	{.id = ALC269_FIXUP_ASPIRE_HEADSET_MIC, .name = "aspire-headset-mic"},
	{.id = ALC269_FIXUP_ASUS_X101, .name = "asus-x101"},
	{.id = ALC271_FIXUP_HP_GATE_MIC_JACK, .name = "acer-ao7xx"},
	{.id = ALC271_FIXUP_HP_GATE_MIC_JACK_E1_572, .name = "acer-aspire-e1"},
	{.id = ALC269_FIXUP_ACER_AC700, .name = "acer-ac700"},
	{.id = ALC269_FIXUP_LIMIT_INT_MIC_BOOST, .name = "limit-mic-boost"},
	{.id = ALC269VB_FIXUP_ASUS_ZENBOOK, .name = "asus-zenbook"},
	{.id = ALC269VB_FIXUP_ASUS_ZENBOOK_UX31A, .name = "asus-zenbook-ux31a"},
	{.id = ALC269VB_FIXUP_ORDISSIMO_EVE2, .name = "ordissimo"},
	{.id = ALC282_FIXUP_ASUS_TX300, .name = "asus-tx300"},
	{.id = ALC283_FIXUP_INT_MIC, .name = "alc283-int-mic"},
	{.id = ALC290_FIXUP_MONO_SPEAKERS_HSJACK, .name = "mono-speakers"},
	{.id = ALC290_FIXUP_SUBWOOFER_HSJACK, .name = "alc290-subwoofer"},
	{.id = ALC269_FIXUP_THINKPAD_ACPI, .name = "thinkpad"},
	{.id = ALC269_FIXUP_DMIC_THINKPAD_ACPI, .name = "dmic-thinkpad"},
	{.id = ALC255_FIXUP_ACER_MIC_NO_PRESENCE, .name = "alc255-acer"},
	{.id = ALC255_FIXUP_ASUS_MIC_NO_PRESENCE, .name = "alc255-asus"},
	{.id = ALC255_FIXUP_DELL1_MIC_NO_PRESENCE, .name = "alc255-dell1"},
	{.id = ALC255_FIXUP_DELL2_MIC_NO_PRESENCE, .name = "alc255-dell2"},
	{.id = ALC293_FIXUP_DELL1_MIC_NO_PRESENCE, .name = "alc293-dell1"},
	{.id = ALC283_FIXUP_HEADSET_MIC, .name = "alc283-headset"},
	{.id = ALC255_FIXUP_MIC_MUTE_LED, .name = "alc255-dell-mute"},
	{.id = ALC282_FIXUP_ASPIRE_V5_PINS, .name = "aspire-v5"},
	{.id = ALC269VB_FIXUP_ASPIRE_E1_COEF, .name = "aspire-e1-coef"},
	{.id = ALC280_FIXUP_HP_GPIO4, .name = "hp-gpio4"},
	{.id = ALC286_FIXUP_HP_GPIO_LED, .name = "hp-gpio-led"},
	{.id = ALC280_FIXUP_HP_GPIO2_MIC_HOTKEY, .name = "hp-gpio2-hotkey"},
	{.id = ALC280_FIXUP_HP_DOCK_PINS, .name = "hp-dock-pins"},
	{.id = ALC269_FIXUP_HP_DOCK_GPIO_MIC1_LED, .name = "hp-dock-gpio-mic"},
	{.id = ALC280_FIXUP_HP_9480M, .name = "hp-9480m"},
	{.id = ALC288_FIXUP_DELL_HEADSET_MODE, .name = "alc288-dell-headset"},
	{.id = ALC288_FIXUP_DELL1_MIC_NO_PRESENCE, .name = "alc288-dell1"},
	{.id = ALC288_FIXUP_DELL_XPS_13, .name = "alc288-dell-xps13"},
	{.id = ALC292_FIXUP_DELL_E7X, .name = "dell-e7x"},
	{.id = ALC293_FIXUP_DISABLE_AAMIX_MULTIJACK, .name = "alc293-dell"},
	{.id = ALC298_FIXUP_DELL1_MIC_NO_PRESENCE, .name = "alc298-dell1"},
	{.id = ALC298_FIXUP_DELL_AIO_MIC_NO_PRESENCE, .name = "alc298-dell-aio"},
	{.id = ALC275_FIXUP_DELL_XPS, .name = "alc275-dell-xps"},
	{.id = ALC293_FIXUP_LENOVO_SPK_NOISE, .name = "lenovo-spk-noise"},
	{.id = ALC233_FIXUP_LENOVO_LINE2_MIC_HOTKEY, .name = "lenovo-hotkey"},
	{.id = ALC255_FIXUP_DELL_SPK_NOISE, .name = "dell-spk-noise"},
	{.id = ALC225_FIXUP_DELL1_MIC_NO_PRESENCE, .name = "alc225-dell1"},
	{.id = ALC295_FIXUP_DISABLE_DAC3, .name = "alc295-disable-dac3"},
	{.id = ALC285_FIXUP_SPEAKER2_TO_DAC1, .name = "alc285-speaker2-to-dac1"},
	{.id = ALC280_FIXUP_HP_HEADSET_MIC, .name = "alc280-hp-headset"},
	{.id = ALC221_FIXUP_HP_FRONT_MIC, .name = "alc221-hp-mic"},
	{.id = ALC298_FIXUP_SPK_VOLUME, .name = "alc298-spk-volume"},
	{.id = ALC256_FIXUP_DELL_INSPIRON_7559_SUBWOOFER, .name = "dell-inspiron-7559"},
	{.id = ALC269_FIXUP_ATIV_BOOK_8, .name = "ativ-book"},
	{.id = ALC221_FIXUP_HP_MIC_NO_PRESENCE, .name = "alc221-hp-mic"},
	{.id = ALC256_FIXUP_ASUS_HEADSET_MODE, .name = "alc256-asus-headset"},
	{.id = ALC256_FIXUP_ASUS_MIC, .name = "alc256-asus-mic"},
	{.id = ALC256_FIXUP_ASUS_AIO_GPIO2, .name = "alc256-asus-aio"},
	{.id = ALC233_FIXUP_ASUS_MIC_NO_PRESENCE, .name = "alc233-asus"},
	{.id = ALC233_FIXUP_EAPD_COEF_AND_MIC_NO_PRESENCE, .name = "alc233-eapd"},
	{.id = ALC294_FIXUP_LENOVO_MIC_LOCATION, .name = "alc294-lenovo-mic"},
	{.id = ALC225_FIXUP_DELL_WYSE_MIC_NO_PRESENCE, .name = "alc225-wyse"},
	{.id = ALC274_FIXUP_DELL_AIO_LINEOUT_VERB, .name = "alc274-dell-aio"},
	{.id = ALC255_FIXUP_DUMMY_LINEOUT_VERB, .name = "alc255-dummy-lineout"},
	{.id = ALC255_FIXUP_DELL_HEADSET_MIC, .name = "alc255-dell-headset"},
	{.id = ALC295_FIXUP_HP_X360, .name = "alc295-hp-x360"},
	{.id = ALC225_FIXUP_HEADSET_JACK, .name = "alc-headset-jack"},
	{.id = ALC295_FIXUP_CHROME_BOOK, .name = "alc-chrome-book"},
	{.id = ALC299_FIXUP_PREDATOR_SPK, .name = "predator-spk"},
	{.id = ALC298_FIXUP_HUAWEI_MBX_STEREO, .name = "huawei-mbx-stereo"},
	{.id = ALC256_FIXUP_MEDION_HEADSET_NO_PRESENCE, .name = "alc256-medion-headset"},
	{.id = ALC298_FIXUP_SAMSUNG_AMP, .name = "alc298-samsung-amp"},
	{.id = ALC256_FIXUP_SAMSUNG_HEADPHONE_VERY_QUIET, .name = "alc256-samsung-headphone"},
	{.id = ALC255_FIXUP_XIAOMI_HEADSET_MIC, .name = "alc255-xiaomi-headset"},
	{.id = ALC274_FIXUP_HP_MIC, .name = "alc274-hp-mic-detect"},
	{.id = ALC245_FIXUP_HP_X360_AMP, .name = "alc245-hp-x360-amp"},
	{.id = ALC295_FIXUP_HP_OMEN, .name = "alc295-hp-omen"},
	{.id = ALC285_FIXUP_HP_SPECTRE_X360, .name = "alc285-hp-spectre-x360"},
	{.id = ALC285_FIXUP_HP_SPECTRE_X360_EB1, .name = "alc285-hp-spectre-x360-eb1"},
	{.id = ALC285_FIXUP_HP_ENVY_X360, .name = "alc285-hp-envy-x360"},
	{.id = ALC287_FIXUP_IDEAPAD_BASS_SPK_AMP, .name = "alc287-ideapad-bass-spk-amp"},
	{.id = ALC287_FIXUP_YOGA9_14IAP7_BASS_SPK_PIN, .name = "alc287-yoga9-bass-spk-pin"},
	{.id = ALC623_FIXUP_LENOVO_THINKSTATION_P340, .name = "alc623-lenovo-thinkstation-p340"},
	{.id = ALC255_FIXUP_ACER_HEADPHONE_AND_MIC, .name = "alc255-acer-headphone-and-mic"},
	{.id = ALC285_FIXUP_HP_GPIO_AMP_INIT, .name = "alc285-hp-amp-init"},
	{}
};
#define ALC225_STANDARD_PINS \
	{0x21, 0x04211020}

#define ALC256_STANDARD_PINS \
	{0x12, 0x90a60140}, \
	{0x14, 0x90170110}, \
	{0x21, 0x02211020}

#define ALC282_STANDARD_PINS \
	{0x14, 0x90170110}

#define ALC290_STANDARD_PINS \
	{0x12, 0x99a30130}

#define ALC292_STANDARD_PINS \
	{0x14, 0x90170110}, \
	{0x15, 0x0221401f}

#define ALC295_STANDARD_PINS \
	{0x12, 0xb7a60130}, \
	{0x14, 0x90170110}, \
	{0x21, 0x04211020}

#define ALC298_STANDARD_PINS \
	{0x12, 0x90a60130}, \
	{0x21, 0x03211020}

static const struct snd_hda_pin_quirk alc269_pin_fixup_tbl[] = {
	SND_HDA_PIN_QUIRK(0x10ec0221, 0x103c, "HP Workstation", ALC221_FIXUP_HP_HEADSET_MIC,
		{0x14, 0x01014020},
		{0x17, 0x90170110},
		{0x18, 0x02a11030},
		{0x19, 0x0181303F},
		{0x21, 0x0221102f}),
	SND_HDA_PIN_QUIRK(0x10ec0255, 0x1025, "Acer", ALC255_FIXUP_ACER_MIC_NO_PRESENCE,
		{0x12, 0x90a601c0},
		{0x14, 0x90171120},
		{0x21, 0x02211030}),
	SND_HDA_PIN_QUIRK(0x10ec0255, 0x1043, "ASUS", ALC255_FIXUP_ASUS_MIC_NO_PRESENCE,
		{0x14, 0x90170110},
		{0x1b, 0x90a70130},
		{0x21, 0x03211020}),
	SND_HDA_PIN_QUIRK(0x10ec0255, 0x1043, "ASUS", ALC255_FIXUP_ASUS_MIC_NO_PRESENCE,
		{0x1a, 0x90a70130},
		{0x1b, 0x90170110},
		{0x21, 0x03211020}),
	SND_HDA_PIN_QUIRK(0x10ec0225, 0x1028, "Dell", ALC225_FIXUP_DELL1_MIC_NO_PRESENCE,
		ALC225_STANDARD_PINS,
		{0x12, 0xb7a60130},
		{0x14, 0x901701a0}),
	SND_HDA_PIN_QUIRK(0x10ec0225, 0x1028, "Dell", ALC225_FIXUP_DELL1_MIC_NO_PRESENCE,
		ALC225_STANDARD_PINS,
		{0x12, 0xb7a60130},
		{0x14, 0x901701b0}),
	SND_HDA_PIN_QUIRK(0x10ec0225, 0x1028, "Dell", ALC225_FIXUP_DELL1_MIC_NO_PRESENCE,
		ALC225_STANDARD_PINS,
		{0x12, 0xb7a60150},
		{0x14, 0x901701a0}),
	SND_HDA_PIN_QUIRK(0x10ec0225, 0x1028, "Dell", ALC225_FIXUP_DELL1_MIC_NO_PRESENCE,
		ALC225_STANDARD_PINS,
		{0x12, 0xb7a60150},
		{0x14, 0x901701b0}),
	SND_HDA_PIN_QUIRK(0x10ec0225, 0x1028, "Dell", ALC225_FIXUP_DELL1_MIC_NO_PRESENCE,
		ALC225_STANDARD_PINS,
		{0x12, 0xb7a60130},
		{0x1b, 0x90170110}),
	SND_HDA_PIN_QUIRK(0x10ec0233, 0x8086, "Intel NUC Skull Canyon", ALC269_FIXUP_DELL1_MIC_NO_PRESENCE,
		{0x1b, 0x01111010},
		{0x1e, 0x01451130},
		{0x21, 0x02211020}),
	SND_HDA_PIN_QUIRK(0x10ec0235, 0x17aa, "Lenovo", ALC233_FIXUP_LENOVO_LINE2_MIC_HOTKEY,
		{0x12, 0x90a60140},
		{0x14, 0x90170110},
		{0x19, 0x02a11030},
		{0x21, 0x02211020}),
	SND_HDA_PIN_QUIRK(0x10ec0235, 0x17aa, "Lenovo", ALC294_FIXUP_LENOVO_MIC_LOCATION,
		{0x14, 0x90170110},
		{0x19, 0x02a11030},
		{0x1a, 0x02a11040},
		{0x1b, 0x01014020},
		{0x21, 0x0221101f}),
	SND_HDA_PIN_QUIRK(0x10ec0235, 0x17aa, "Lenovo", ALC294_FIXUP_LENOVO_MIC_LOCATION,
		{0x14, 0x90170110},
		{0x19, 0x02a11030},
		{0x1a, 0x02a11040},
		{0x1b, 0x01011020},
		{0x21, 0x0221101f}),
	SND_HDA_PIN_QUIRK(0x10ec0235, 0x17aa, "Lenovo", ALC294_FIXUP_LENOVO_MIC_LOCATION,
		{0x14, 0x90170110},
		{0x19, 0x02a11020},
		{0x1a, 0x02a11030},
		{0x21, 0x0221101f}),
	SND_HDA_PIN_QUIRK(0x10ec0236, 0x1028, "Dell", ALC236_FIXUP_DELL_AIO_HEADSET_MIC,
		{0x21, 0x02211010}),
	SND_HDA_PIN_QUIRK(0x10ec0236, 0x103c, "HP", ALC256_FIXUP_HP_HEADSET_MIC,
		{0x14, 0x90170110},
		{0x19, 0x02a11020},
		{0x21, 0x02211030}),
	SND_HDA_PIN_QUIRK(0x10ec0255, 0x1028, "Dell", ALC255_FIXUP_DELL2_MIC_NO_PRESENCE,
		{0x14, 0x90170110},
		{0x21, 0x02211020}),
	SND_HDA_PIN_QUIRK(0x10ec0255, 0x1028, "Dell", ALC255_FIXUP_DELL1_MIC_NO_PRESENCE,
		{0x14, 0x90170130},
		{0x21, 0x02211040}),
	SND_HDA_PIN_QUIRK(0x10ec0255, 0x1028, "Dell", ALC255_FIXUP_DELL1_MIC_NO_PRESENCE,
		{0x12, 0x90a60140},
		{0x14, 0x90170110},
		{0x21, 0x02211020}),
	SND_HDA_PIN_QUIRK(0x10ec0255, 0x1028, "Dell", ALC255_FIXUP_DELL1_MIC_NO_PRESENCE,
		{0x12, 0x90a60160},
		{0x14, 0x90170120},
		{0x21, 0x02211030}),
	SND_HDA_PIN_QUIRK(0x10ec0255, 0x1028, "Dell", ALC255_FIXUP_DELL1_MIC_NO_PRESENCE,
		{0x14, 0x90170110},
		{0x1b, 0x02011020},
		{0x21, 0x0221101f}),
	SND_HDA_PIN_QUIRK(0x10ec0255, 0x1028, "Dell", ALC255_FIXUP_DELL1_MIC_NO_PRESENCE,
		{0x14, 0x90170110},
		{0x1b, 0x01011020},
		{0x21, 0x0221101f}),
	SND_HDA_PIN_QUIRK(0x10ec0255, 0x1028, "Dell", ALC255_FIXUP_DELL1_MIC_NO_PRESENCE,
		{0x14, 0x90170130},
		{0x1b, 0x01014020},
		{0x21, 0x0221103f}),
	SND_HDA_PIN_QUIRK(0x10ec0255, 0x1028, "Dell", ALC255_FIXUP_DELL1_MIC_NO_PRESENCE,
		{0x14, 0x90170130},
		{0x1b, 0x01011020},
		{0x21, 0x0221103f}),
	SND_HDA_PIN_QUIRK(0x10ec0255, 0x1028, "Dell", ALC255_FIXUP_DELL1_MIC_NO_PRESENCE,
		{0x14, 0x90170130},
		{0x1b, 0x02011020},
		{0x21, 0x0221103f}),
	SND_HDA_PIN_QUIRK(0x10ec0255, 0x1028, "Dell", ALC255_FIXUP_DELL1_MIC_NO_PRESENCE,
		{0x14, 0x90170150},
		{0x1b, 0x02011020},
		{0x21, 0x0221105f}),
	SND_HDA_PIN_QUIRK(0x10ec0255, 0x1028, "Dell", ALC255_FIXUP_DELL1_MIC_NO_PRESENCE,
		{0x14, 0x90170110},
		{0x1b, 0x01014020},
		{0x21, 0x0221101f}),
	SND_HDA_PIN_QUIRK(0x10ec0255, 0x1028, "Dell", ALC255_FIXUP_DELL1_MIC_NO_PRESENCE,
		{0x12, 0x90a60160},
		{0x14, 0x90170120},
		{0x17, 0x90170140},
		{0x21, 0x0321102f}),
	SND_HDA_PIN_QUIRK(0x10ec0255, 0x1028, "Dell", ALC255_FIXUP_DELL1_MIC_NO_PRESENCE,
		{0x12, 0x90a60160},
		{0x14, 0x90170130},
		{0x21, 0x02211040}),
	SND_HDA_PIN_QUIRK(0x10ec0255, 0x1028, "Dell", ALC255_FIXUP_DELL1_MIC_NO_PRESENCE,
		{0x12, 0x90a60160},
		{0x14, 0x90170140},
		{0x21, 0x02211050}),
	SND_HDA_PIN_QUIRK(0x10ec0255, 0x1028, "Dell", ALC255_FIXUP_DELL1_MIC_NO_PRESENCE,
		{0x12, 0x90a60170},
		{0x14, 0x90170120},
		{0x21, 0x02211030}),
	SND_HDA_PIN_QUIRK(0x10ec0255, 0x1028, "Dell", ALC255_FIXUP_DELL1_MIC_NO_PRESENCE,
		{0x12, 0x90a60170},
		{0x14, 0x90170130},
		{0x21, 0x02211040}),
	SND_HDA_PIN_QUIRK(0x10ec0255, 0x1028, "Dell", ALC255_FIXUP_DELL1_MIC_NO_PRESENCE,
		{0x12, 0x90a60170},
		{0x14, 0x90171130},
		{0x21, 0x02211040}),
	SND_HDA_PIN_QUIRK(0x10ec0255, 0x1028, "Dell", ALC255_FIXUP_DELL1_MIC_NO_PRESENCE,
		{0x12, 0x90a60170},
		{0x14, 0x90170140},
		{0x21, 0x02211050}),
	SND_HDA_PIN_QUIRK(0x10ec0255, 0x1028, "Dell Inspiron 5548", ALC255_FIXUP_DELL1_MIC_NO_PRESENCE,
		{0x12, 0x90a60180},
		{0x14, 0x90170130},
		{0x21, 0x02211040}),
	SND_HDA_PIN_QUIRK(0x10ec0255, 0x1028, "Dell Inspiron 5565", ALC255_FIXUP_DELL1_MIC_NO_PRESENCE,
		{0x12, 0x90a60180},
		{0x14, 0x90170120},
		{0x21, 0x02211030}),
	SND_HDA_PIN_QUIRK(0x10ec0255, 0x1028, "Dell", ALC255_FIXUP_DELL1_MIC_NO_PRESENCE,
		{0x1b, 0x01011020},
		{0x21, 0x02211010}),
	SND_HDA_PIN_QUIRK(0x10ec0256, 0x1043, "ASUS", ALC256_FIXUP_ASUS_MIC,
		{0x14, 0x90170110},
		{0x1b, 0x90a70130},
		{0x21, 0x04211020}),
	SND_HDA_PIN_QUIRK(0x10ec0256, 0x1043, "ASUS", ALC256_FIXUP_ASUS_MIC,
		{0x14, 0x90170110},
		{0x1b, 0x90a70130},
		{0x21, 0x03211020}),
	SND_HDA_PIN_QUIRK(0x10ec0256, 0x1043, "ASUS", ALC256_FIXUP_ASUS_MIC_NO_PRESENCE,
		{0x12, 0x90a60130},
		{0x14, 0x90170110},
		{0x21, 0x03211020}),
	SND_HDA_PIN_QUIRK(0x10ec0256, 0x1043, "ASUS", ALC256_FIXUP_ASUS_MIC_NO_PRESENCE,
		{0x12, 0x90a60130},
		{0x14, 0x90170110},
		{0x21, 0x04211020}),
	SND_HDA_PIN_QUIRK(0x10ec0256, 0x1043, "ASUS", ALC256_FIXUP_ASUS_MIC_NO_PRESENCE,
		{0x1a, 0x90a70130},
		{0x1b, 0x90170110},
		{0x21, 0x03211020}),
       SND_HDA_PIN_QUIRK(0x10ec0256, 0x103c, "HP", ALC256_FIXUP_HP_HEADSET_MIC,
		{0x14, 0x90170110},
		{0x19, 0x02a11020},
		{0x21, 0x0221101f}),
       SND_HDA_PIN_QUIRK(0x10ec0274, 0x103c, "HP", ALC274_FIXUP_HP_HEADSET_MIC,
		{0x17, 0x90170110},
		{0x19, 0x03a11030},
		{0x21, 0x03211020}),
	SND_HDA_PIN_QUIRK(0x10ec0280, 0x103c, "HP", ALC280_FIXUP_HP_GPIO4,
		{0x12, 0x90a60130},
		{0x14, 0x90170110},
		{0x15, 0x0421101f},
		{0x1a, 0x04a11020}),
	SND_HDA_PIN_QUIRK(0x10ec0280, 0x103c, "HP", ALC269_FIXUP_HP_GPIO_MIC1_LED,
		{0x12, 0x90a60140},
		{0x14, 0x90170110},
		{0x15, 0x0421101f},
		{0x18, 0x02811030},
		{0x1a, 0x04a1103f},
		{0x1b, 0x02011020}),
	SND_HDA_PIN_QUIRK(0x10ec0282, 0x103c, "HP 15 Touchsmart", ALC269_FIXUP_HP_MUTE_LED_MIC1,
		ALC282_STANDARD_PINS,
		{0x12, 0x99a30130},
		{0x19, 0x03a11020},
		{0x21, 0x0321101f}),
	SND_HDA_PIN_QUIRK(0x10ec0282, 0x103c, "HP", ALC269_FIXUP_HP_MUTE_LED_MIC1,
		ALC282_STANDARD_PINS,
		{0x12, 0x99a30130},
		{0x19, 0x03a11020},
		{0x21, 0x03211040}),
	SND_HDA_PIN_QUIRK(0x10ec0282, 0x103c, "HP", ALC269_FIXUP_HP_MUTE_LED_MIC1,
		ALC282_STANDARD_PINS,
		{0x12, 0x99a30130},
		{0x19, 0x03a11030},
		{0x21, 0x03211020}),
	SND_HDA_PIN_QUIRK(0x10ec0282, 0x103c, "HP", ALC269_FIXUP_HP_MUTE_LED_MIC1,
		ALC282_STANDARD_PINS,
		{0x12, 0x99a30130},
		{0x19, 0x04a11020},
		{0x21, 0x0421101f}),
	SND_HDA_PIN_QUIRK(0x10ec0282, 0x103c, "HP", ALC269_FIXUP_HP_LINE1_MIC1_LED,
		ALC282_STANDARD_PINS,
		{0x12, 0x90a60140},
		{0x19, 0x04a11030},
		{0x21, 0x04211020}),
	SND_HDA_PIN_QUIRK(0x10ec0282, 0x1025, "Acer", ALC282_FIXUP_ACER_DISABLE_LINEOUT,
		ALC282_STANDARD_PINS,
		{0x12, 0x90a609c0},
		{0x18, 0x03a11830},
		{0x19, 0x04a19831},
		{0x1a, 0x0481303f},
		{0x1b, 0x04211020},
		{0x21, 0x0321101f}),
	SND_HDA_PIN_QUIRK(0x10ec0282, 0x1025, "Acer", ALC282_FIXUP_ACER_DISABLE_LINEOUT,
		ALC282_STANDARD_PINS,
		{0x12, 0x90a60940},
		{0x18, 0x03a11830},
		{0x19, 0x04a19831},
		{0x1a, 0x0481303f},
		{0x1b, 0x04211020},
		{0x21, 0x0321101f}),
	SND_HDA_PIN_QUIRK(0x10ec0283, 0x1028, "Dell", ALC269_FIXUP_DELL1_MIC_NO_PRESENCE,
		ALC282_STANDARD_PINS,
		{0x12, 0x90a60130},
		{0x21, 0x0321101f}),
	SND_HDA_PIN_QUIRK(0x10ec0283, 0x1028, "Dell", ALC269_FIXUP_DELL1_MIC_NO_PRESENCE,
		{0x12, 0x90a60160},
		{0x14, 0x90170120},
		{0x21, 0x02211030}),
	SND_HDA_PIN_QUIRK(0x10ec0283, 0x1028, "Dell", ALC269_FIXUP_DELL1_MIC_NO_PRESENCE,
		ALC282_STANDARD_PINS,
		{0x12, 0x90a60130},
		{0x19, 0x03a11020},
		{0x21, 0x0321101f}),
	SND_HDA_PIN_QUIRK(0x10ec0285, 0x17aa, "Lenovo", ALC285_FIXUP_LENOVO_PC_BEEP_IN_NOISE,
		{0x12, 0x90a60130},
		{0x14, 0x90170110},
		{0x19, 0x04a11040},
		{0x21, 0x04211020}),
	SND_HDA_PIN_QUIRK(0x10ec0285, 0x17aa, "Lenovo", ALC285_FIXUP_LENOVO_PC_BEEP_IN_NOISE,
		{0x14, 0x90170110},
		{0x19, 0x04a11040},
		{0x1d, 0x40600001},
		{0x21, 0x04211020}),
	SND_HDA_PIN_QUIRK(0x10ec0285, 0x17aa, "Lenovo", ALC285_FIXUP_THINKPAD_NO_BASS_SPK_HEADSET_JACK,
		{0x14, 0x90170110},
		{0x19, 0x04a11040},
		{0x21, 0x04211020}),
	SND_HDA_PIN_QUIRK(0x10ec0287, 0x17aa, "Lenovo", ALC285_FIXUP_THINKPAD_HEADSET_JACK,
		{0x14, 0x90170110},
		{0x17, 0x90170111},
		{0x19, 0x03a11030},
		{0x21, 0x03211020}),
	SND_HDA_PIN_QUIRK(0x10ec0287, 0x17aa, "Lenovo", ALC287_FIXUP_THINKPAD_I2S_SPK,
		{0x17, 0x90170110},
		{0x19, 0x03a11030},
		{0x21, 0x03211020}),
	SND_HDA_PIN_QUIRK(0x10ec0287, 0x17aa, "Lenovo", ALC287_FIXUP_THINKPAD_I2S_SPK,
		{0x17, 0x90170110}, /* 0x231f with RTK I2S AMP */
		{0x19, 0x04a11040},
		{0x21, 0x04211020}),
	SND_HDA_PIN_QUIRK(0x10ec0286, 0x1025, "Acer", ALC286_FIXUP_ACER_AIO_MIC_NO_PRESENCE,
		{0x12, 0x90a60130},
		{0x17, 0x90170110},
		{0x21, 0x02211020}),
	SND_HDA_PIN_QUIRK(0x10ec0288, 0x1028, "Dell", ALC288_FIXUP_DELL1_MIC_NO_PRESENCE,
		{0x12, 0x90a60120},
		{0x14, 0x90170110},
		{0x21, 0x0321101f}),
	SND_HDA_PIN_QUIRK(0x10ec0290, 0x103c, "HP", ALC269_FIXUP_HP_MUTE_LED_MIC1,
		ALC290_STANDARD_PINS,
		{0x15, 0x04211040},
		{0x18, 0x90170112},
		{0x1a, 0x04a11020}),
	SND_HDA_PIN_QUIRK(0x10ec0290, 0x103c, "HP", ALC269_FIXUP_HP_MUTE_LED_MIC1,
		ALC290_STANDARD_PINS,
		{0x15, 0x04211040},
		{0x18, 0x90170110},
		{0x1a, 0x04a11020}),
	SND_HDA_PIN_QUIRK(0x10ec0290, 0x103c, "HP", ALC269_FIXUP_HP_MUTE_LED_MIC1,
		ALC290_STANDARD_PINS,
		{0x15, 0x0421101f},
		{0x1a, 0x04a11020}),
	SND_HDA_PIN_QUIRK(0x10ec0290, 0x103c, "HP", ALC269_FIXUP_HP_MUTE_LED_MIC1,
		ALC290_STANDARD_PINS,
		{0x15, 0x04211020},
		{0x1a, 0x04a11040}),
	SND_HDA_PIN_QUIRK(0x10ec0290, 0x103c, "HP", ALC269_FIXUP_HP_MUTE_LED_MIC1,
		ALC290_STANDARD_PINS,
		{0x14, 0x90170110},
		{0x15, 0x04211020},
		{0x1a, 0x04a11040}),
	SND_HDA_PIN_QUIRK(0x10ec0290, 0x103c, "HP", ALC269_FIXUP_HP_MUTE_LED_MIC1,
		ALC290_STANDARD_PINS,
		{0x14, 0x90170110},
		{0x15, 0x04211020},
		{0x1a, 0x04a11020}),
	SND_HDA_PIN_QUIRK(0x10ec0290, 0x103c, "HP", ALC269_FIXUP_HP_MUTE_LED_MIC1,
		ALC290_STANDARD_PINS,
		{0x14, 0x90170110},
		{0x15, 0x0421101f},
		{0x1a, 0x04a11020}),
	SND_HDA_PIN_QUIRK(0x10ec0292, 0x1028, "Dell", ALC269_FIXUP_DELL2_MIC_NO_PRESENCE,
		ALC292_STANDARD_PINS,
		{0x12, 0x90a60140},
		{0x16, 0x01014020},
		{0x19, 0x01a19030}),
	SND_HDA_PIN_QUIRK(0x10ec0292, 0x1028, "Dell", ALC269_FIXUP_DELL2_MIC_NO_PRESENCE,
		ALC292_STANDARD_PINS,
		{0x12, 0x90a60140},
		{0x16, 0x01014020},
		{0x18, 0x02a19031},
		{0x19, 0x01a1903e}),
	SND_HDA_PIN_QUIRK(0x10ec0292, 0x1028, "Dell", ALC269_FIXUP_DELL3_MIC_NO_PRESENCE,
		ALC292_STANDARD_PINS,
		{0x12, 0x90a60140}),
	SND_HDA_PIN_QUIRK(0x10ec0293, 0x1028, "Dell", ALC293_FIXUP_DELL1_MIC_NO_PRESENCE,
		ALC292_STANDARD_PINS,
		{0x13, 0x90a60140},
		{0x16, 0x21014020},
		{0x19, 0x21a19030}),
	SND_HDA_PIN_QUIRK(0x10ec0293, 0x1028, "Dell", ALC293_FIXUP_DELL1_MIC_NO_PRESENCE,
		ALC292_STANDARD_PINS,
		{0x13, 0x90a60140}),
	SND_HDA_PIN_QUIRK(0x10ec0294, 0x1043, "ASUS", ALC294_FIXUP_ASUS_HPE,
		{0x17, 0x90170110},
		{0x21, 0x04211020}),
	SND_HDA_PIN_QUIRK(0x10ec0294, 0x1043, "ASUS", ALC294_FIXUP_ASUS_MIC,
		{0x14, 0x90170110},
		{0x1b, 0x90a70130},
		{0x21, 0x04211020}),
	SND_HDA_PIN_QUIRK(0x10ec0294, 0x1043, "ASUS", ALC294_FIXUP_ASUS_SPK,
		{0x12, 0x90a60130},
		{0x17, 0x90170110},
		{0x21, 0x03211020}),
	SND_HDA_PIN_QUIRK(0x10ec0294, 0x1043, "ASUS", ALC294_FIXUP_ASUS_SPK,
		{0x12, 0x90a60130},
		{0x17, 0x90170110},
		{0x21, 0x04211020}),
	SND_HDA_PIN_QUIRK(0x10ec0295, 0x1043, "ASUS", ALC294_FIXUP_ASUS_SPK,
		{0x12, 0x90a60130},
		{0x17, 0x90170110},
		{0x21, 0x03211020}),
	SND_HDA_PIN_QUIRK(0x10ec0295, 0x1043, "ASUS", ALC295_FIXUP_ASUS_MIC_NO_PRESENCE,
		{0x12, 0x90a60120},
		{0x17, 0x90170110},
		{0x21, 0x04211030}),
	SND_HDA_PIN_QUIRK(0x10ec0295, 0x1043, "ASUS", ALC295_FIXUP_ASUS_MIC_NO_PRESENCE,
		{0x12, 0x90a60130},
		{0x17, 0x90170110},
		{0x21, 0x03211020}),
	SND_HDA_PIN_QUIRK(0x10ec0295, 0x1043, "ASUS", ALC295_FIXUP_ASUS_MIC_NO_PRESENCE,
		{0x12, 0x90a60130},
		{0x17, 0x90170110},
		{0x21, 0x03211020}),
	SND_HDA_PIN_QUIRK(0x10ec0298, 0x1028, "Dell", ALC298_FIXUP_DELL1_MIC_NO_PRESENCE,
		ALC298_STANDARD_PINS,
		{0x17, 0x90170110}),
	SND_HDA_PIN_QUIRK(0x10ec0298, 0x1028, "Dell", ALC298_FIXUP_DELL1_MIC_NO_PRESENCE,
		ALC298_STANDARD_PINS,
		{0x17, 0x90170140}),
	SND_HDA_PIN_QUIRK(0x10ec0298, 0x1028, "Dell", ALC298_FIXUP_DELL1_MIC_NO_PRESENCE,
		ALC298_STANDARD_PINS,
		{0x17, 0x90170150}),
	SND_HDA_PIN_QUIRK(0x10ec0298, 0x1028, "Dell", ALC298_FIXUP_SPK_VOLUME,
		{0x12, 0xb7a60140},
		{0x13, 0xb7a60150},
		{0x17, 0x90170110},
		{0x1a, 0x03011020},
		{0x21, 0x03211030}),
	SND_HDA_PIN_QUIRK(0x10ec0298, 0x1028, "Dell", ALC298_FIXUP_ALIENWARE_MIC_NO_PRESENCE,
		{0x12, 0xb7a60140},
		{0x17, 0x90170110},
		{0x1a, 0x03a11030},
		{0x21, 0x03211020}),
	SND_HDA_PIN_QUIRK(0x10ec0299, 0x1028, "Dell", ALC269_FIXUP_DELL4_MIC_NO_PRESENCE,
		ALC225_STANDARD_PINS,
		{0x12, 0xb7a60130},
		{0x17, 0x90170110}),
	SND_HDA_PIN_QUIRK(0x10ec0623, 0x17aa, "Lenovo", ALC283_FIXUP_HEADSET_MIC,
		{0x14, 0x01014010},
		{0x17, 0x90170120},
		{0x18, 0x02a11030},
		{0x19, 0x02a1103f},
		{0x21, 0x0221101f}),
	{}
};

/* This is the fallback pin_fixup_tbl for alc269 family, to make the tbl match
 * more machines, don't need to match all valid pins, just need to match
 * all the pins defined in the tbl. Just because of this reason, it is possible
 * that a single machine matches multiple tbls, so there is one limitation:
 *   at most one tbl is allowed to define for the same vendor and same codec
 */
static const struct snd_hda_pin_quirk alc269_fallback_pin_fixup_tbl[] = {
	SND_HDA_PIN_QUIRK(0x10ec0256, 0x1025, "Acer", ALC2XX_FIXUP_HEADSET_MIC,
		{0x19, 0x40000000}),
	SND_HDA_PIN_QUIRK(0x10ec0289, 0x1028, "Dell", ALC269_FIXUP_DELL4_MIC_NO_PRESENCE,
		{0x19, 0x40000000},
		{0x1b, 0x40000000}),
	SND_HDA_PIN_QUIRK(0x10ec0295, 0x1028, "Dell", ALC269_FIXUP_DELL4_MIC_NO_PRESENCE,
		{0x19, 0x40000000},
		{0x1b, 0x40000000}),
	SND_HDA_PIN_QUIRK(0x10ec0256, 0x1028, "Dell", ALC255_FIXUP_DELL1_MIC_NO_PRESENCE,
		{0x19, 0x40000000},
		{0x1a, 0x40000000}),
	SND_HDA_PIN_QUIRK(0x10ec0236, 0x1028, "Dell", ALC255_FIXUP_DELL1_MIC_NO_PRESENCE,
		{0x19, 0x40000000},
		{0x1a, 0x40000000}),
	SND_HDA_PIN_QUIRK(0x10ec0274, 0x1028, "Dell", ALC274_FIXUP_DELL_AIO_LINEOUT_VERB,
		{0x19, 0x40000000},
		{0x1a, 0x40000000}),
	SND_HDA_PIN_QUIRK(0x10ec0256, 0x1043, "ASUS", ALC2XX_FIXUP_HEADSET_MIC,
		{0x19, 0x40000000}),
	{}
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static void alc269_fill_coef(struct hda_codec *codec)
{
	struct alc_spec *spec = codec->spec;
	int val;

	if (spec->codec_variant != ALC269_TYPE_ALC269VB)
		return;

	if ((alc_get_coef0(codec) & 0x00ff) < 0x015) {
		alc_write_coef_idx(codec, 0xf, 0x960b);
		alc_write_coef_idx(codec, 0xe, 0x8817);
	}

	if ((alc_get_coef0(codec) & 0x00ff) == 0x016) {
		alc_write_coef_idx(codec, 0xf, 0x960b);
		alc_write_coef_idx(codec, 0xe, 0x8814);
	}

	if ((alc_get_coef0(codec) & 0x00ff) == 0x017) {
<<<<<<< HEAD
		val = alc_read_coef_idx(codec, 0x04);
		/* Power up output pin */
		if (val != -1)
			alc_write_coef_idx(codec, 0x04, val | (1<<11));
=======
		/* Power up output pin */
		alc_update_coef_idx(codec, 0x04, 0, 1<<11);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if ((alc_get_coef0(codec) & 0x00ff) == 0x018) {
		val = alc_read_coef_idx(codec, 0xd);
		if (val != -1 && (val & 0x0c00) >> 10 != 0x1) {
			/* Capless ramp up clock control */
			alc_write_coef_idx(codec, 0xd, val | (1<<10));
		}
		val = alc_read_coef_idx(codec, 0x17);
		if (val != -1 && (val & 0x01c0) >> 6 != 0x4) {
			/* Class D power on reset */
			alc_write_coef_idx(codec, 0x17, val | (1<<7));
		}
	}

<<<<<<< HEAD
	val = alc_read_coef_idx(codec, 0xd); /* Class D */
	if (val != -1)
		alc_write_coef_idx(codec, 0xd, val | (1<<14));

	val = alc_read_coef_idx(codec, 0x4); /* HP */
	if (val != -1)
		alc_write_coef_idx(codec, 0x4, val | (1<<11));
=======
	/* HP */
	alc_update_coef_idx(codec, 0x4, 0, 1<<11);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 */
static int patch_alc269(struct hda_codec *codec)
{
	struct alc_spec *spec;
<<<<<<< HEAD
	int err = 0;

	spec = kzalloc(sizeof(*spec), GFP_KERNEL);
	if (spec == NULL)
		return -ENOMEM;

	codec->spec = spec;

	spec->mixer_nid = 0x0b;

	err = alc_codec_rename_from_preset(codec);
	if (err < 0)
		goto error;

	alc_pick_fixup(codec, alc269_fixup_models,
		       alc269_fixup_tbl, alc269_fixups);
	alc_apply_fixup(codec, ALC_FIXUP_ACT_PRE_PROBE);

	alc_auto_parse_customize_define(codec);

	if (codec->vendor_id == 0x10ec0269) {
		spec->codec_variant = ALC269_TYPE_ALC269VA;
		switch (alc_get_coef0(codec) & 0x00f0) {
		case 0x0010:
			if (codec->bus->pci->subsystem_vendor == 0x1025 &&
=======
	int err;

	err = alc_alloc_spec(codec, 0x0b);
	if (err < 0)
		return err;

	spec = codec->spec;
	spec->gen.shared_mic_vref_pin = 0x18;
	codec->power_save_node = 0;
	spec->en_3kpull_low = true;

#ifdef CONFIG_PM
	codec->patch_ops.suspend = alc269_suspend;
	codec->patch_ops.resume = alc269_resume;
#endif
	spec->shutup = alc_default_shutup;
	spec->init_hook = alc_default_init;

	switch (codec->core.vendor_id) {
	case 0x10ec0269:
		spec->codec_variant = ALC269_TYPE_ALC269VA;
		switch (alc_get_coef0(codec) & 0x00f0) {
		case 0x0010:
			if (codec->bus->pci &&
			    codec->bus->pci->subsystem_vendor == 0x1025 &&
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			    spec->cdefine.platform_type == 1)
				err = alc_codec_rename(codec, "ALC271X");
			spec->codec_variant = ALC269_TYPE_ALC269VB;
			break;
		case 0x0020:
<<<<<<< HEAD
			if (codec->bus->pci->subsystem_vendor == 0x17aa &&
=======
			if (codec->bus->pci &&
			    codec->bus->pci->subsystem_vendor == 0x17aa &&
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			    codec->bus->pci->subsystem_device == 0x21f3)
				err = alc_codec_rename(codec, "ALC3202");
			spec->codec_variant = ALC269_TYPE_ALC269VC;
			break;
<<<<<<< HEAD
=======
		case 0x0030:
			spec->codec_variant = ALC269_TYPE_ALC269VD;
			break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		default:
			alc_fix_pll_init(codec, 0x20, 0x04, 15);
		}
		if (err < 0)
			goto error;
<<<<<<< HEAD
		spec->init_hook = alc269_fill_coef;
		alc269_fill_coef(codec);
	}

=======
		spec->shutup = alc269_shutup;
		spec->init_hook = alc269_fill_coef;
		alc269_fill_coef(codec);
		break;

	case 0x10ec0280:
	case 0x10ec0290:
		spec->codec_variant = ALC269_TYPE_ALC280;
		break;
	case 0x10ec0282:
		spec->codec_variant = ALC269_TYPE_ALC282;
		spec->shutup = alc282_shutup;
		spec->init_hook = alc282_init;
		break;
	case 0x10ec0233:
	case 0x10ec0283:
		spec->codec_variant = ALC269_TYPE_ALC283;
		spec->shutup = alc283_shutup;
		spec->init_hook = alc283_init;
		break;
	case 0x10ec0284:
	case 0x10ec0292:
		spec->codec_variant = ALC269_TYPE_ALC284;
		break;
	case 0x10ec0293:
		spec->codec_variant = ALC269_TYPE_ALC293;
		break;
	case 0x10ec0286:
	case 0x10ec0288:
		spec->codec_variant = ALC269_TYPE_ALC286;
		break;
	case 0x10ec0298:
		spec->codec_variant = ALC269_TYPE_ALC298;
		break;
	case 0x10ec0235:
	case 0x10ec0255:
		spec->codec_variant = ALC269_TYPE_ALC255;
		spec->shutup = alc256_shutup;
		spec->init_hook = alc256_init;
		break;
	case 0x10ec0230:
	case 0x10ec0236:
	case 0x10ec0256:
	case 0x19e58326:
		spec->codec_variant = ALC269_TYPE_ALC256;
		spec->shutup = alc256_shutup;
		spec->init_hook = alc256_init;
		spec->gen.mixer_nid = 0; /* ALC256 does not have any loopback mixer path */
		if (codec->core.vendor_id == 0x10ec0236 &&
		    codec->bus->pci->vendor != PCI_VENDOR_ID_AMD)
			spec->en_3kpull_low = false;
		break;
	case 0x10ec0257:
		spec->codec_variant = ALC269_TYPE_ALC257;
		spec->shutup = alc256_shutup;
		spec->init_hook = alc256_init;
		spec->gen.mixer_nid = 0;
		spec->en_3kpull_low = false;
		break;
	case 0x10ec0215:
	case 0x10ec0245:
	case 0x10ec0285:
	case 0x10ec0289:
		if (alc_get_coef0(codec) & 0x0010)
			spec->codec_variant = ALC269_TYPE_ALC245;
		else
			spec->codec_variant = ALC269_TYPE_ALC215;
		spec->shutup = alc225_shutup;
		spec->init_hook = alc225_init;
		spec->gen.mixer_nid = 0;
		break;
	case 0x10ec0225:
	case 0x10ec0295:
	case 0x10ec0299:
		spec->codec_variant = ALC269_TYPE_ALC225;
		spec->shutup = alc225_shutup;
		spec->init_hook = alc225_init;
		spec->gen.mixer_nid = 0; /* no loopback on ALC225, ALC295 and ALC299 */
		break;
	case 0x10ec0287:
		spec->codec_variant = ALC269_TYPE_ALC287;
		spec->shutup = alc225_shutup;
		spec->init_hook = alc225_init;
		spec->gen.mixer_nid = 0; /* no loopback on ALC287 */
		break;
	case 0x10ec0234:
	case 0x10ec0274:
	case 0x10ec0294:
		spec->codec_variant = ALC269_TYPE_ALC294;
		spec->gen.mixer_nid = 0; /* ALC2x4 does not have any loopback mixer path */
		alc_update_coef_idx(codec, 0x6b, 0x0018, (1<<4) | (1<<3)); /* UAJ MIC Vref control by verb */
		spec->init_hook = alc294_init;
		break;
	case 0x10ec0300:
		spec->codec_variant = ALC269_TYPE_ALC300;
		spec->gen.mixer_nid = 0; /* no loopback on ALC300 */
		break;
	case 0x10ec0623:
		spec->codec_variant = ALC269_TYPE_ALC623;
		break;
	case 0x10ec0700:
	case 0x10ec0701:
	case 0x10ec0703:
	case 0x10ec0711:
		spec->codec_variant = ALC269_TYPE_ALC700;
		spec->gen.mixer_nid = 0; /* ALC700 does not have any loopback mixer path */
		alc_update_coef_idx(codec, 0x4a, 1 << 15, 0); /* Combo jack auto trigger control */
		spec->init_hook = alc294_init;
		break;

	}

	if (snd_hda_codec_read(codec, 0x51, 0, AC_VERB_PARAMETERS, 0) == 0x10ec5505) {
		spec->has_alc5505_dsp = 1;
		spec->init_hook = alc5505_dsp_init;
	}

	alc_pre_init(codec);

	snd_hda_pick_fixup(codec, alc269_fixup_models,
		       alc269_fixup_tbl, alc269_fixups);
	/* FIXME: both TX300 and ROG Strix G17 have the same SSID, and
	 * the quirk breaks the latter (bko#214101).
	 * Clear the wrong entry.
	 */
	if (codec->fixup_id == ALC282_FIXUP_ASUS_TX300 &&
	    codec->core.vendor_id == 0x10ec0294) {
		codec_dbg(codec, "Clear wrong fixup for ASUS ROG Strix G17\n");
		codec->fixup_id = HDA_FIXUP_ID_NOT_SET;
	}

	snd_hda_pick_pin_fixup(codec, alc269_pin_fixup_tbl, alc269_fixups, true);
	snd_hda_pick_pin_fixup(codec, alc269_fallback_pin_fixup_tbl, alc269_fixups, false);
	snd_hda_pick_fixup(codec, NULL,	alc269_fixup_vendor_tbl,
			   alc269_fixups);
	snd_hda_apply_fixup(codec, HDA_FIXUP_ACT_PRE_PROBE);

	alc_auto_parse_customize_define(codec);

	if (has_cdefine_beep(codec))
		spec->gen.beep_nid = 0x01;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* automatic parse from the BIOS config */
	err = alc269_parse_auto_config(codec);
	if (err < 0)
		goto error;

<<<<<<< HEAD
	if (!spec->no_analog && has_cdefine_beep(codec)) {
		err = snd_hda_attach_beep_device(codec, 0x1);
		if (err < 0)
			goto error;
		set_beep_amp(spec, 0x0b, 0x04, HDA_INPUT);
	}

	codec->patch_ops = alc_patch_ops;
#ifdef CONFIG_PM
	codec->patch_ops.resume = alc269_resume;
#endif
	spec->shutup = alc269_shutup;

	alc_apply_fixup(codec, ALC_FIXUP_ACT_PROBE);
=======
	if (!spec->gen.no_analog && spec->gen.beep_nid && spec->gen.mixer_nid) {
		err = set_beep_amp(spec, spec->gen.mixer_nid, 0x04, HDA_INPUT);
		if (err < 0)
			goto error;
	}

	snd_hda_apply_fixup(codec, HDA_FIXUP_ACT_PROBE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;

 error:
	alc_free(codec);
	return err;
}

/*
 * ALC861
 */

static int alc861_parse_auto_config(struct hda_codec *codec)
{
	static const hda_nid_t alc861_ignore[] = { 0x1d, 0 };
	static const hda_nid_t alc861_ssids[] = { 0x0e, 0x0f, 0x0b, 0 };
	return alc_parse_auto_config(codec, alc861_ignore, alc861_ssids);
}

/* Pin config fixes */
enum {
	ALC861_FIXUP_FSC_AMILO_PI1505,
	ALC861_FIXUP_AMP_VREF_0F,
	ALC861_FIXUP_NO_JACK_DETECT,
	ALC861_FIXUP_ASUS_A6RP,
<<<<<<< HEAD
=======
	ALC660_FIXUP_ASUS_W7J,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/* On some laptops, VREF of pin 0x0f is abused for controlling the main amp */
static void alc861_fixup_asus_amp_vref_0f(struct hda_codec *codec,
<<<<<<< HEAD
			const struct alc_fixup *fix, int action)
=======
			const struct hda_fixup *fix, int action)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct alc_spec *spec = codec->spec;
	unsigned int val;

<<<<<<< HEAD
	if (action != ALC_FIXUP_ACT_INIT)
		return;
	val = snd_hda_codec_read(codec, 0x0f, 0,
				 AC_VERB_GET_PIN_WIDGET_CONTROL, 0);
	if (!(val & (AC_PINCTL_IN_EN | AC_PINCTL_OUT_EN)))
		val |= AC_PINCTL_IN_EN;
	val |= AC_PINCTL_VREF_50;
	snd_hda_codec_write(codec, 0x0f, 0,
			    AC_VERB_SET_PIN_WIDGET_CONTROL, val);
	spec->keep_vref_in_automute = 1;
=======
	if (action != HDA_FIXUP_ACT_INIT)
		return;
	val = snd_hda_codec_get_pin_target(codec, 0x0f);
	if (!(val & (AC_PINCTL_IN_EN | AC_PINCTL_OUT_EN)))
		val |= AC_PINCTL_IN_EN;
	val |= AC_PINCTL_VREF_50;
	snd_hda_set_pin_ctl(codec, 0x0f, val);
	spec->gen.keep_vref_in_automute = 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* suppress the jack-detection */
static void alc_fixup_no_jack_detect(struct hda_codec *codec,
<<<<<<< HEAD
				     const struct alc_fixup *fix, int action)
{
	if (action == ALC_FIXUP_ACT_PRE_PROBE)
		codec->no_jack_detect = 1;
}

static const struct alc_fixup alc861_fixups[] = {
	[ALC861_FIXUP_FSC_AMILO_PI1505] = {
		.type = ALC_FIXUP_PINS,
		.v.pins = (const struct alc_pincfg[]) {
=======
				     const struct hda_fixup *fix, int action)
{
	if (action == HDA_FIXUP_ACT_PRE_PROBE)
		codec->no_jack_detect = 1;
}

static const struct hda_fixup alc861_fixups[] = {
	[ALC861_FIXUP_FSC_AMILO_PI1505] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			{ 0x0b, 0x0221101f }, /* HP */
			{ 0x0f, 0x90170310 }, /* speaker */
			{ }
		}
	},
	[ALC861_FIXUP_AMP_VREF_0F] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_FUNC,
		.v.func = alc861_fixup_asus_amp_vref_0f,
	},
	[ALC861_FIXUP_NO_JACK_DETECT] = {
		.type = ALC_FIXUP_FUNC,
		.v.func = alc_fixup_no_jack_detect,
	},
	[ALC861_FIXUP_ASUS_A6RP] = {
		.type = ALC_FIXUP_FUNC,
		.v.func = alc861_fixup_asus_amp_vref_0f,
		.chained = true,
		.chain_id = ALC861_FIXUP_NO_JACK_DETECT,
=======
		.type = HDA_FIXUP_FUNC,
		.v.func = alc861_fixup_asus_amp_vref_0f,
	},
	[ALC861_FIXUP_NO_JACK_DETECT] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_no_jack_detect,
	},
	[ALC861_FIXUP_ASUS_A6RP] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc861_fixup_asus_amp_vref_0f,
		.chained = true,
		.chain_id = ALC861_FIXUP_NO_JACK_DETECT,
	},
	[ALC660_FIXUP_ASUS_W7J] = {
		.type = HDA_FIXUP_VERBS,
		.v.verbs = (const struct hda_verb[]) {
			/* ASUS W7J needs a magic pin setup on unused NID 0x10
			 * for enabling outputs
			 */
			{0x10, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x24},
			{ }
		},
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
};

static const struct snd_pci_quirk alc861_fixup_tbl[] = {
<<<<<<< HEAD
	SND_PCI_QUIRK(0x1043, 0x1393, "ASUS A6Rp", ALC861_FIXUP_ASUS_A6RP),
	SND_PCI_QUIRK_VENDOR(0x1043, "ASUS laptop", ALC861_FIXUP_AMP_VREF_0F),
	SND_PCI_QUIRK(0x1462, 0x7254, "HP DX2200", ALC861_FIXUP_NO_JACK_DETECT),
	SND_PCI_QUIRK(0x1584, 0x2b01, "Haier W18", ALC861_FIXUP_AMP_VREF_0F),
	SND_PCI_QUIRK(0x1584, 0x0000, "Uniwill ECS M31EI", ALC861_FIXUP_AMP_VREF_0F),
=======
	SND_PCI_QUIRK(0x1043, 0x1253, "ASUS W7J", ALC660_FIXUP_ASUS_W7J),
	SND_PCI_QUIRK(0x1043, 0x1263, "ASUS Z35HL", ALC660_FIXUP_ASUS_W7J),
	SND_PCI_QUIRK(0x1043, 0x1393, "ASUS A6Rp", ALC861_FIXUP_ASUS_A6RP),
	SND_PCI_QUIRK_VENDOR(0x1043, "ASUS laptop", ALC861_FIXUP_AMP_VREF_0F),
	SND_PCI_QUIRK(0x1462, 0x7254, "HP DX2200", ALC861_FIXUP_NO_JACK_DETECT),
	SND_PCI_QUIRK_VENDOR(0x1584, "Haier/Uniwill", ALC861_FIXUP_AMP_VREF_0F),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	SND_PCI_QUIRK(0x1734, 0x10c7, "FSC Amilo Pi1505", ALC861_FIXUP_FSC_AMILO_PI1505),
	{}
};

/*
 */
static int patch_alc861(struct hda_codec *codec)
{
	struct alc_spec *spec;
	int err;

<<<<<<< HEAD
	spec = kzalloc(sizeof(*spec), GFP_KERNEL);
	if (spec == NULL)
		return -ENOMEM;

	codec->spec = spec;

	spec->mixer_nid = 0x15;

	alc_pick_fixup(codec, NULL, alc861_fixup_tbl, alc861_fixups);
	alc_apply_fixup(codec, ALC_FIXUP_ACT_PRE_PROBE);
=======
	err = alc_alloc_spec(codec, 0x15);
	if (err < 0)
		return err;

	spec = codec->spec;
	if (has_cdefine_beep(codec))
		spec->gen.beep_nid = 0x23;

#ifdef CONFIG_PM
	spec->power_hook = alc_power_eapd;
#endif

	alc_pre_init(codec);

	snd_hda_pick_fixup(codec, NULL, alc861_fixup_tbl, alc861_fixups);
	snd_hda_apply_fixup(codec, HDA_FIXUP_ACT_PRE_PROBE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* automatic parse from the BIOS config */
	err = alc861_parse_auto_config(codec);
	if (err < 0)
		goto error;

<<<<<<< HEAD
	if (!spec->no_analog) {
		err = snd_hda_attach_beep_device(codec, 0x23);
		if (err < 0)
			goto error;
		set_beep_amp(spec, 0x23, 0, HDA_OUTPUT);
	}

	codec->patch_ops = alc_patch_ops;
#ifdef CONFIG_SND_HDA_POWER_SAVE
	spec->power_hook = alc_power_eapd;
#endif

	alc_apply_fixup(codec, ALC_FIXUP_ACT_PROBE);
=======
	if (!spec->gen.no_analog) {
		err = set_beep_amp(spec, 0x23, 0, HDA_OUTPUT);
		if (err < 0)
			goto error;
	}

	snd_hda_apply_fixup(codec, HDA_FIXUP_ACT_PROBE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;

 error:
	alc_free(codec);
	return err;
}

/*
 * ALC861-VD support
 *
 * Based on ALC882
 *
 * In addition, an independent DAC
 */
static int alc861vd_parse_auto_config(struct hda_codec *codec)
{
	static const hda_nid_t alc861vd_ignore[] = { 0x1d, 0 };
	static const hda_nid_t alc861vd_ssids[] = { 0x15, 0x1b, 0x14, 0 };
	return alc_parse_auto_config(codec, alc861vd_ignore, alc861vd_ssids);
}

enum {
	ALC660VD_FIX_ASUS_GPIO1,
	ALC861VD_FIX_DALLAS,
};

/* exclude VREF80 */
static void alc861vd_fixup_dallas(struct hda_codec *codec,
<<<<<<< HEAD
				  const struct alc_fixup *fix, int action)
{
	if (action == ALC_FIXUP_ACT_PRE_PROBE) {
=======
				  const struct hda_fixup *fix, int action)
{
	if (action == HDA_FIXUP_ACT_PRE_PROBE) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		snd_hda_override_pin_caps(codec, 0x18, 0x00000734);
		snd_hda_override_pin_caps(codec, 0x19, 0x0000073c);
	}
}

<<<<<<< HEAD
static const struct alc_fixup alc861vd_fixups[] = {
	[ALC660VD_FIX_ASUS_GPIO1] = {
		.type = ALC_FIXUP_VERBS,
		.v.verbs = (const struct hda_verb[]) {
			/* reset GPIO1 */
			{0x01, AC_VERB_SET_GPIO_MASK, 0x03},
			{0x01, AC_VERB_SET_GPIO_DIRECTION, 0x01},
			{0x01, AC_VERB_SET_GPIO_DATA, 0x01},
			{ }
		}
	},
	[ALC861VD_FIX_DALLAS] = {
		.type = ALC_FIXUP_FUNC,
=======
/* reset GPIO1 */
static void alc660vd_fixup_asus_gpio1(struct hda_codec *codec,
				      const struct hda_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;

	if (action == HDA_FIXUP_ACT_PRE_PROBE)
		spec->gpio_mask |= 0x02;
	alc_fixup_gpio(codec, action, 0x01);
}

static const struct hda_fixup alc861vd_fixups[] = {
	[ALC660VD_FIX_ASUS_GPIO1] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc660vd_fixup_asus_gpio1,
	},
	[ALC861VD_FIX_DALLAS] = {
		.type = HDA_FIXUP_FUNC,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.v.func = alc861vd_fixup_dallas,
	},
};

static const struct snd_pci_quirk alc861vd_fixup_tbl[] = {
	SND_PCI_QUIRK(0x103c, 0x30bf, "HP TX1000", ALC861VD_FIX_DALLAS),
	SND_PCI_QUIRK(0x1043, 0x1339, "ASUS A7-K", ALC660VD_FIX_ASUS_GPIO1),
	SND_PCI_QUIRK(0x1179, 0xff31, "Toshiba L30-149", ALC861VD_FIX_DALLAS),
	{}
};

<<<<<<< HEAD
static const struct hda_verb alc660vd_eapd_verbs[] = {
	{0x14, AC_VERB_SET_EAPD_BTLENABLE, 2},
	{0x15, AC_VERB_SET_EAPD_BTLENABLE, 2},
	{ }
};

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 */
static int patch_alc861vd(struct hda_codec *codec)
{
	struct alc_spec *spec;
	int err;

<<<<<<< HEAD
	spec = kzalloc(sizeof(*spec), GFP_KERNEL);
	if (spec == NULL)
		return -ENOMEM;

	codec->spec = spec;

	spec->mixer_nid = 0x0b;

	alc_pick_fixup(codec, NULL, alc861vd_fixup_tbl, alc861vd_fixups);
	alc_apply_fixup(codec, ALC_FIXUP_ACT_PRE_PROBE);
=======
	err = alc_alloc_spec(codec, 0x0b);
	if (err < 0)
		return err;

	spec = codec->spec;
	if (has_cdefine_beep(codec))
		spec->gen.beep_nid = 0x23;

	spec->shutup = alc_eapd_shutup;

	alc_pre_init(codec);

	snd_hda_pick_fixup(codec, NULL, alc861vd_fixup_tbl, alc861vd_fixups);
	snd_hda_apply_fixup(codec, HDA_FIXUP_ACT_PRE_PROBE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* automatic parse from the BIOS config */
	err = alc861vd_parse_auto_config(codec);
	if (err < 0)
		goto error;

<<<<<<< HEAD
	if (codec->vendor_id == 0x10ec0660) {
		/* always turn on EAPD */
		add_verb(spec, alc660vd_eapd_verbs);
	}

	if (!spec->no_analog) {
		err = snd_hda_attach_beep_device(codec, 0x23);
		if (err < 0)
			goto error;
		set_beep_amp(spec, 0x0b, 0x05, HDA_INPUT);
	}

	codec->patch_ops = alc_patch_ops;

	spec->shutup = alc_eapd_shutup;

	alc_apply_fixup(codec, ALC_FIXUP_ACT_PROBE);
=======
	if (!spec->gen.no_analog) {
		err = set_beep_amp(spec, 0x0b, 0x05, HDA_INPUT);
		if (err < 0)
			goto error;
	}

	snd_hda_apply_fixup(codec, HDA_FIXUP_ACT_PROBE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;

 error:
	alc_free(codec);
	return err;
}

/*
 * ALC662 support
 *
 * ALC662 is almost identical with ALC880 but has cleaner and more flexible
 * configuration.  Each pin widget can choose any input DACs and a mixer.
 * Each ADC is connected from a mixer of all inputs.  This makes possible
 * 6-channel independent captures.
 *
 * In addition, an independent DAC for the multi-playback (not used in this
 * driver yet).
 */

/*
 * BIOS auto configuration
 */

static int alc662_parse_auto_config(struct hda_codec *codec)
{
	static const hda_nid_t alc662_ignore[] = { 0x1d, 0 };
	static const hda_nid_t alc663_ssids[] = { 0x15, 0x1b, 0x14, 0x21 };
	static const hda_nid_t alc662_ssids[] = { 0x15, 0x1b, 0x14, 0 };
	const hda_nid_t *ssids;

<<<<<<< HEAD
	if (codec->vendor_id == 0x10ec0272 || codec->vendor_id == 0x10ec0663 ||
	    codec->vendor_id == 0x10ec0665 || codec->vendor_id == 0x10ec0670 ||
	    codec->vendor_id == 0x10ec0671)
=======
	if (codec->core.vendor_id == 0x10ec0272 || codec->core.vendor_id == 0x10ec0663 ||
	    codec->core.vendor_id == 0x10ec0665 || codec->core.vendor_id == 0x10ec0670 ||
	    codec->core.vendor_id == 0x10ec0671)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ssids = alc663_ssids;
	else
		ssids = alc662_ssids;
	return alc_parse_auto_config(codec, alc662_ignore, ssids);
}

static void alc272_fixup_mario(struct hda_codec *codec,
<<<<<<< HEAD
			       const struct alc_fixup *fix, int action)
{
	if (action != ALC_FIXUP_ACT_PROBE)
=======
			       const struct hda_fixup *fix, int action)
{
	if (action != HDA_FIXUP_ACT_PRE_PROBE)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;
	if (snd_hda_override_amp_caps(codec, 0x2, HDA_OUTPUT,
				      (0x3b << AC_AMPCAP_OFFSET_SHIFT) |
				      (0x3b << AC_AMPCAP_NUM_STEPS_SHIFT) |
				      (0x03 << AC_AMPCAP_STEP_SIZE_SHIFT) |
				      (0 << AC_AMPCAP_MUTE_SHIFT)))
<<<<<<< HEAD
		printk(KERN_WARNING
		       "hda_codec: failed to override amp caps for NID 0x2\n");
=======
		codec_warn(codec, "failed to override amp caps for NID 0x2\n");
}

static const struct snd_pcm_chmap_elem asus_pcm_2_1_chmaps[] = {
	{ .channels = 2,
	  .map = { SNDRV_CHMAP_FL, SNDRV_CHMAP_FR } },
	{ .channels = 4,
	  .map = { SNDRV_CHMAP_FL, SNDRV_CHMAP_FR,
		   SNDRV_CHMAP_NA, SNDRV_CHMAP_LFE } }, /* LFE only on right */
	{ }
};

/* override the 2.1 chmap */
static void alc_fixup_bass_chmap(struct hda_codec *codec,
				    const struct hda_fixup *fix, int action)
{
	if (action == HDA_FIXUP_ACT_BUILD) {
		struct alc_spec *spec = codec->spec;
		spec->gen.pcm_rec[0]->stream[0].chmap = asus_pcm_2_1_chmaps;
	}
}

/* avoid D3 for keeping GPIO up */
static unsigned int gpio_led_power_filter(struct hda_codec *codec,
					  hda_nid_t nid,
					  unsigned int power_state)
{
	struct alc_spec *spec = codec->spec;
	if (nid == codec->core.afg && power_state == AC_PWRST_D3 && spec->gpio_data)
		return AC_PWRST_D0;
	return power_state;
}

static void alc662_fixup_led_gpio1(struct hda_codec *codec,
				   const struct hda_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;

	alc_fixup_hp_gpio_led(codec, action, 0x01, 0);
	if (action == HDA_FIXUP_ACT_PRE_PROBE) {
		spec->mute_led_polarity = 1;
		codec->power_filter = gpio_led_power_filter;
	}
}

static void alc662_usi_automute_hook(struct hda_codec *codec,
					 struct hda_jack_callback *jack)
{
	struct alc_spec *spec = codec->spec;
	int vref;
	msleep(200);
	snd_hda_gen_hp_automute(codec, jack);

	vref = spec->gen.hp_jack_present ? PIN_VREF80 : 0;
	msleep(100);
	snd_hda_codec_write(codec, 0x19, 0, AC_VERB_SET_PIN_WIDGET_CONTROL,
			    vref);
}

static void alc662_fixup_usi_headset_mic(struct hda_codec *codec,
				     const struct hda_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;
	if (action == HDA_FIXUP_ACT_PRE_PROBE) {
		spec->parse_flags |= HDA_PINCFG_HEADSET_MIC;
		spec->gen.hp_automute_hook = alc662_usi_automute_hook;
	}
}

static void alc662_aspire_ethos_mute_speakers(struct hda_codec *codec,
					struct hda_jack_callback *cb)
{
	/* surround speakers at 0x1b already get muted automatically when
	 * headphones are plugged in, but we have to mute/unmute the remaining
	 * channels manually:
	 * 0x15 - front left/front right
	 * 0x18 - front center/ LFE
	 */
	if (snd_hda_jack_detect_state(codec, 0x1b) == HDA_JACK_PRESENT) {
		snd_hda_set_pin_ctl_cache(codec, 0x15, 0);
		snd_hda_set_pin_ctl_cache(codec, 0x18, 0);
	} else {
		snd_hda_set_pin_ctl_cache(codec, 0x15, PIN_OUT);
		snd_hda_set_pin_ctl_cache(codec, 0x18, PIN_OUT);
	}
}

static void alc662_fixup_aspire_ethos_hp(struct hda_codec *codec,
					const struct hda_fixup *fix, int action)
{
    /* Pin 0x1b: shared headphones jack and surround speakers */
	if (!is_jack_detectable(codec, 0x1b))
		return;

	switch (action) {
	case HDA_FIXUP_ACT_PRE_PROBE:
		snd_hda_jack_detect_enable_callback(codec, 0x1b,
				alc662_aspire_ethos_mute_speakers);
		/* subwoofer needs an extra GPIO setting to become audible */
		alc_setup_gpio(codec, 0x02);
		break;
	case HDA_FIXUP_ACT_INIT:
		/* Make sure to start in a correct state, i.e. if
		 * headphones have been plugged in before powering up the system
		 */
		alc662_aspire_ethos_mute_speakers(codec, NULL);
		break;
	}
}

static void alc671_fixup_hp_headset_mic2(struct hda_codec *codec,
					     const struct hda_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;

	static const struct hda_pintbl pincfgs[] = {
		{ 0x19, 0x02a11040 }, /* use as headset mic, with its own jack detect */
		{ 0x1b, 0x0181304f },
		{ }
	};

	switch (action) {
	case HDA_FIXUP_ACT_PRE_PROBE:
		spec->gen.mixer_nid = 0;
		spec->parse_flags |= HDA_PINCFG_HEADSET_MIC;
		snd_hda_apply_pincfgs(codec, pincfgs);
		break;
	case HDA_FIXUP_ACT_INIT:
		alc_write_coef_idx(codec, 0x19, 0xa054);
		break;
	}
}

static void alc897_hp_automute_hook(struct hda_codec *codec,
					 struct hda_jack_callback *jack)
{
	struct alc_spec *spec = codec->spec;
	int vref;

	snd_hda_gen_hp_automute(codec, jack);
	vref = spec->gen.hp_jack_present ? (PIN_HP | AC_PINCTL_VREF_100) : PIN_HP;
	snd_hda_set_pin_ctl(codec, 0x1b, vref);
}

static void alc897_fixup_lenovo_headset_mic(struct hda_codec *codec,
				     const struct hda_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;
	if (action == HDA_FIXUP_ACT_PRE_PROBE) {
		spec->gen.hp_automute_hook = alc897_hp_automute_hook;
		spec->no_shutup_pins = 1;
	}
	if (action == HDA_FIXUP_ACT_PROBE) {
		snd_hda_set_pin_ctl_cache(codec, 0x1a, PIN_IN | AC_PINCTL_VREF_100);
	}
}

static void alc897_fixup_lenovo_headset_mode(struct hda_codec *codec,
				     const struct hda_fixup *fix, int action)
{
	struct alc_spec *spec = codec->spec;

	if (action == HDA_FIXUP_ACT_PRE_PROBE) {
		spec->parse_flags |= HDA_PINCFG_HEADSET_MIC;
		spec->gen.hp_automute_hook = alc897_hp_automute_hook;
	}
}

static const struct coef_fw alc668_coefs[] = {
	WRITE_COEF(0x01, 0xbebe), WRITE_COEF(0x02, 0xaaaa), WRITE_COEF(0x03,    0x0),
	WRITE_COEF(0x04, 0x0180), WRITE_COEF(0x06,    0x0), WRITE_COEF(0x07, 0x0f80),
	WRITE_COEF(0x08, 0x0031), WRITE_COEF(0x0a, 0x0060), WRITE_COEF(0x0b,    0x0),
	WRITE_COEF(0x0c, 0x7cf7), WRITE_COEF(0x0d, 0x1080), WRITE_COEF(0x0e, 0x7f7f),
	WRITE_COEF(0x0f, 0xcccc), WRITE_COEF(0x10, 0xddcc), WRITE_COEF(0x11, 0x0001),
	WRITE_COEF(0x13,    0x0), WRITE_COEF(0x14, 0x2aa0), WRITE_COEF(0x17, 0xa940),
	WRITE_COEF(0x19,    0x0), WRITE_COEF(0x1a,    0x0), WRITE_COEF(0x1b,    0x0),
	WRITE_COEF(0x1c,    0x0), WRITE_COEF(0x1d,    0x0), WRITE_COEF(0x1e, 0x7418),
	WRITE_COEF(0x1f, 0x0804), WRITE_COEF(0x20, 0x4200), WRITE_COEF(0x21, 0x0468),
	WRITE_COEF(0x22, 0x8ccc), WRITE_COEF(0x23, 0x0250), WRITE_COEF(0x24, 0x7418),
	WRITE_COEF(0x27,    0x0), WRITE_COEF(0x28, 0x8ccc), WRITE_COEF(0x2a, 0xff00),
	WRITE_COEF(0x2b, 0x8000), WRITE_COEF(0xa7, 0xff00), WRITE_COEF(0xa8, 0x8000),
	WRITE_COEF(0xaa, 0x2e17), WRITE_COEF(0xab, 0xa0c0), WRITE_COEF(0xac,    0x0),
	WRITE_COEF(0xad,    0x0), WRITE_COEF(0xae, 0x2ac6), WRITE_COEF(0xaf, 0xa480),
	WRITE_COEF(0xb0,    0x0), WRITE_COEF(0xb1,    0x0), WRITE_COEF(0xb2,    0x0),
	WRITE_COEF(0xb3,    0x0), WRITE_COEF(0xb4,    0x0), WRITE_COEF(0xb5, 0x1040),
	WRITE_COEF(0xb6, 0xd697), WRITE_COEF(0xb7, 0x902b), WRITE_COEF(0xb8, 0xd697),
	WRITE_COEF(0xb9, 0x902b), WRITE_COEF(0xba, 0xb8ba), WRITE_COEF(0xbb, 0xaaab),
	WRITE_COEF(0xbc, 0xaaaf), WRITE_COEF(0xbd, 0x6aaa), WRITE_COEF(0xbe, 0x1c02),
	WRITE_COEF(0xc0, 0x00ff), WRITE_COEF(0xc1, 0x0fa6),
	{}
};

static void alc668_restore_default_value(struct hda_codec *codec)
{
	alc_process_coef_fw(codec, alc668_coefs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

enum {
	ALC662_FIXUP_ASPIRE,
<<<<<<< HEAD
	ALC662_FIXUP_IDEAPAD,
	ALC272_FIXUP_MARIO,
=======
	ALC662_FIXUP_LED_GPIO1,
	ALC662_FIXUP_IDEAPAD,
	ALC272_FIXUP_MARIO,
	ALC662_FIXUP_CZC_ET26,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ALC662_FIXUP_CZC_P10T,
	ALC662_FIXUP_SKU_IGNORE,
	ALC662_FIXUP_HP_RP5800,
	ALC662_FIXUP_ASUS_MODE1,
	ALC662_FIXUP_ASUS_MODE2,
	ALC662_FIXUP_ASUS_MODE3,
	ALC662_FIXUP_ASUS_MODE4,
	ALC662_FIXUP_ASUS_MODE5,
	ALC662_FIXUP_ASUS_MODE6,
	ALC662_FIXUP_ASUS_MODE7,
	ALC662_FIXUP_ASUS_MODE8,
	ALC662_FIXUP_NO_JACK_DETECT,
	ALC662_FIXUP_ZOTAC_Z68,
<<<<<<< HEAD
};

static const struct alc_fixup alc662_fixups[] = {
	[ALC662_FIXUP_ASPIRE] = {
		.type = ALC_FIXUP_PINS,
		.v.pins = (const struct alc_pincfg[]) {
=======
	ALC662_FIXUP_INV_DMIC,
	ALC662_FIXUP_DELL_MIC_NO_PRESENCE,
	ALC668_FIXUP_DELL_MIC_NO_PRESENCE,
	ALC662_FIXUP_HEADSET_MODE,
	ALC668_FIXUP_HEADSET_MODE,
	ALC662_FIXUP_BASS_MODE4_CHMAP,
	ALC662_FIXUP_BASS_16,
	ALC662_FIXUP_BASS_1A,
	ALC662_FIXUP_BASS_CHMAP,
	ALC668_FIXUP_AUTO_MUTE,
	ALC668_FIXUP_DELL_DISABLE_AAMIX,
	ALC668_FIXUP_DELL_XPS13,
	ALC662_FIXUP_ASUS_Nx50,
	ALC668_FIXUP_ASUS_Nx51_HEADSET_MODE,
	ALC668_FIXUP_ASUS_Nx51,
	ALC668_FIXUP_MIC_COEF,
	ALC668_FIXUP_ASUS_G751,
	ALC891_FIXUP_HEADSET_MODE,
	ALC891_FIXUP_DELL_MIC_NO_PRESENCE,
	ALC662_FIXUP_ACER_VERITON,
	ALC892_FIXUP_ASROCK_MOBO,
	ALC662_FIXUP_USI_FUNC,
	ALC662_FIXUP_USI_HEADSET_MODE,
	ALC662_FIXUP_LENOVO_MULTI_CODECS,
	ALC669_FIXUP_ACER_ASPIRE_ETHOS,
	ALC669_FIXUP_ACER_ASPIRE_ETHOS_HEADSET,
	ALC671_FIXUP_HP_HEADSET_MIC2,
	ALC662_FIXUP_ACER_X2660G_HEADSET_MODE,
	ALC662_FIXUP_ACER_NITRO_HEADSET_MODE,
	ALC668_FIXUP_ASUS_NO_HEADSET_MIC,
	ALC668_FIXUP_HEADSET_MIC,
	ALC668_FIXUP_MIC_DET_COEF,
	ALC897_FIXUP_LENOVO_HEADSET_MIC,
	ALC897_FIXUP_HEADSET_MIC_PIN,
	ALC897_FIXUP_HP_HSMIC_VERB,
	ALC897_FIXUP_LENOVO_HEADSET_MODE,
	ALC897_FIXUP_HEADSET_MIC_PIN2,
	ALC897_FIXUP_UNIS_H3C_X500S,
};

static const struct hda_fixup alc662_fixups[] = {
	[ALC662_FIXUP_ASPIRE] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			{ 0x15, 0x99130112 }, /* subwoofer */
			{ }
		}
	},
<<<<<<< HEAD
	[ALC662_FIXUP_IDEAPAD] = {
		.type = ALC_FIXUP_PINS,
		.v.pins = (const struct alc_pincfg[]) {
			{ 0x17, 0x99130112 }, /* subwoofer */
			{ }
		}
	},
	[ALC272_FIXUP_MARIO] = {
		.type = ALC_FIXUP_FUNC,
		.v.func = alc272_fixup_mario,
	},
	[ALC662_FIXUP_CZC_P10T] = {
		.type = ALC_FIXUP_VERBS,
=======
	[ALC662_FIXUP_LED_GPIO1] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc662_fixup_led_gpio1,
	},
	[ALC662_FIXUP_IDEAPAD] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x17, 0x99130112 }, /* subwoofer */
			{ }
		},
		.chained = true,
		.chain_id = ALC662_FIXUP_LED_GPIO1,
	},
	[ALC272_FIXUP_MARIO] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc272_fixup_mario,
	},
	[ALC662_FIXUP_CZC_ET26] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{0x12, 0x403cc000},
			{0x14, 0x90170110}, /* speaker */
			{0x15, 0x411111f0},
			{0x16, 0x411111f0},
			{0x18, 0x01a19030}, /* mic */
			{0x19, 0x90a7013f}, /* int-mic */
			{0x1a, 0x01014020},
			{0x1b, 0x0121401f},
			{0x1c, 0x411111f0},
			{0x1d, 0x411111f0},
			{0x1e, 0x40478e35},
			{}
		},
		.chained = true,
		.chain_id = ALC662_FIXUP_SKU_IGNORE
	},
	[ALC662_FIXUP_CZC_P10T] = {
		.type = HDA_FIXUP_VERBS,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.v.verbs = (const struct hda_verb[]) {
			{0x14, AC_VERB_SET_EAPD_BTLENABLE, 0},
			{}
		}
	},
	[ALC662_FIXUP_SKU_IGNORE] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_SKU,
		.v.sku = ALC_FIXUP_SKU_IGNORE,
	},
	[ALC662_FIXUP_HP_RP5800] = {
		.type = ALC_FIXUP_PINS,
		.v.pins = (const struct alc_pincfg[]) {
=======
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_sku_ignore,
	},
	[ALC662_FIXUP_HP_RP5800] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			{ 0x14, 0x0221201f }, /* HP out */
			{ }
		},
		.chained = true,
		.chain_id = ALC662_FIXUP_SKU_IGNORE
	},
	[ALC662_FIXUP_ASUS_MODE1] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_PINS,
		.v.pins = (const struct alc_pincfg[]) {
=======
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			{ 0x14, 0x99130110 }, /* speaker */
			{ 0x18, 0x01a19c20 }, /* mic */
			{ 0x19, 0x99a3092f }, /* int-mic */
			{ 0x21, 0x0121401f }, /* HP out */
			{ }
		},
		.chained = true,
		.chain_id = ALC662_FIXUP_SKU_IGNORE
	},
	[ALC662_FIXUP_ASUS_MODE2] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_PINS,
		.v.pins = (const struct alc_pincfg[]) {
=======
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			{ 0x14, 0x99130110 }, /* speaker */
			{ 0x18, 0x01a19820 }, /* mic */
			{ 0x19, 0x99a3092f }, /* int-mic */
			{ 0x1b, 0x0121401f }, /* HP out */
			{ }
		},
		.chained = true,
		.chain_id = ALC662_FIXUP_SKU_IGNORE
	},
	[ALC662_FIXUP_ASUS_MODE3] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_PINS,
		.v.pins = (const struct alc_pincfg[]) {
=======
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			{ 0x14, 0x99130110 }, /* speaker */
			{ 0x15, 0x0121441f }, /* HP */
			{ 0x18, 0x01a19840 }, /* mic */
			{ 0x19, 0x99a3094f }, /* int-mic */
			{ 0x21, 0x01211420 }, /* HP2 */
			{ }
		},
		.chained = true,
		.chain_id = ALC662_FIXUP_SKU_IGNORE
	},
	[ALC662_FIXUP_ASUS_MODE4] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_PINS,
		.v.pins = (const struct alc_pincfg[]) {
=======
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			{ 0x14, 0x99130110 }, /* speaker */
			{ 0x16, 0x99130111 }, /* speaker */
			{ 0x18, 0x01a19840 }, /* mic */
			{ 0x19, 0x99a3094f }, /* int-mic */
			{ 0x21, 0x0121441f }, /* HP */
			{ }
		},
		.chained = true,
		.chain_id = ALC662_FIXUP_SKU_IGNORE
	},
	[ALC662_FIXUP_ASUS_MODE5] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_PINS,
		.v.pins = (const struct alc_pincfg[]) {
=======
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			{ 0x14, 0x99130110 }, /* speaker */
			{ 0x15, 0x0121441f }, /* HP */
			{ 0x16, 0x99130111 }, /* speaker */
			{ 0x18, 0x01a19840 }, /* mic */
			{ 0x19, 0x99a3094f }, /* int-mic */
			{ }
		},
		.chained = true,
		.chain_id = ALC662_FIXUP_SKU_IGNORE
	},
	[ALC662_FIXUP_ASUS_MODE6] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_PINS,
		.v.pins = (const struct alc_pincfg[]) {
=======
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			{ 0x14, 0x99130110 }, /* speaker */
			{ 0x15, 0x01211420 }, /* HP2 */
			{ 0x18, 0x01a19840 }, /* mic */
			{ 0x19, 0x99a3094f }, /* int-mic */
			{ 0x1b, 0x0121441f }, /* HP */
			{ }
		},
		.chained = true,
		.chain_id = ALC662_FIXUP_SKU_IGNORE
	},
	[ALC662_FIXUP_ASUS_MODE7] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_PINS,
		.v.pins = (const struct alc_pincfg[]) {
=======
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			{ 0x14, 0x99130110 }, /* speaker */
			{ 0x17, 0x99130111 }, /* speaker */
			{ 0x18, 0x01a19840 }, /* mic */
			{ 0x19, 0x99a3094f }, /* int-mic */
			{ 0x1b, 0x01214020 }, /* HP */
			{ 0x21, 0x0121401f }, /* HP */
			{ }
		},
		.chained = true,
		.chain_id = ALC662_FIXUP_SKU_IGNORE
	},
	[ALC662_FIXUP_ASUS_MODE8] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_PINS,
		.v.pins = (const struct alc_pincfg[]) {
=======
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			{ 0x14, 0x99130110 }, /* speaker */
			{ 0x12, 0x99a30970 }, /* int-mic */
			{ 0x15, 0x01214020 }, /* HP */
			{ 0x17, 0x99130111 }, /* speaker */
			{ 0x18, 0x01a19840 }, /* mic */
			{ 0x21, 0x0121401f }, /* HP */
			{ }
		},
		.chained = true,
		.chain_id = ALC662_FIXUP_SKU_IGNORE
	},
	[ALC662_FIXUP_NO_JACK_DETECT] = {
<<<<<<< HEAD
		.type = ALC_FIXUP_FUNC,
		.v.func = alc_fixup_no_jack_detect,
	},
	[ALC662_FIXUP_ZOTAC_Z68] = {
		.type = ALC_FIXUP_PINS,
		.v.pins = (const struct alc_pincfg[]) {
=======
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_no_jack_detect,
	},
	[ALC662_FIXUP_ZOTAC_Z68] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			{ 0x1b, 0x02214020 }, /* Front HP */
			{ }
		}
	},
<<<<<<< HEAD
=======
	[ALC662_FIXUP_INV_DMIC] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_inv_dmic,
	},
	[ALC668_FIXUP_DELL_XPS13] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_dell_xps13,
		.chained = true,
		.chain_id = ALC668_FIXUP_DELL_DISABLE_AAMIX
	},
	[ALC668_FIXUP_DELL_DISABLE_AAMIX] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_disable_aamix,
		.chained = true,
		.chain_id = ALC668_FIXUP_DELL_MIC_NO_PRESENCE
	},
	[ALC668_FIXUP_AUTO_MUTE] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_auto_mute_via_amp,
		.chained = true,
		.chain_id = ALC668_FIXUP_DELL_MIC_NO_PRESENCE
	},
	[ALC662_FIXUP_DELL_MIC_NO_PRESENCE] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x19, 0x03a1113c }, /* use as headset mic, without its own jack detect */
			/* headphone mic by setting pin control of 0x1b (headphone out) to in + vref_50 */
			{ }
		},
		.chained = true,
		.chain_id = ALC662_FIXUP_HEADSET_MODE
	},
	[ALC662_FIXUP_HEADSET_MODE] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_headset_mode_alc662,
	},
	[ALC668_FIXUP_DELL_MIC_NO_PRESENCE] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x19, 0x03a1913d }, /* use as headphone mic, without its own jack detect */
			{ 0x1b, 0x03a1113c }, /* use as headset mic, without its own jack detect */
			{ }
		},
		.chained = true,
		.chain_id = ALC668_FIXUP_HEADSET_MODE
	},
	[ALC668_FIXUP_HEADSET_MODE] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_headset_mode_alc668,
	},
	[ALC662_FIXUP_BASS_MODE4_CHMAP] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_bass_chmap,
		.chained = true,
		.chain_id = ALC662_FIXUP_ASUS_MODE4
	},
	[ALC662_FIXUP_BASS_16] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{0x16, 0x80106111}, /* bass speaker */
			{}
		},
		.chained = true,
		.chain_id = ALC662_FIXUP_BASS_CHMAP,
	},
	[ALC662_FIXUP_BASS_1A] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{0x1a, 0x80106111}, /* bass speaker */
			{}
		},
		.chained = true,
		.chain_id = ALC662_FIXUP_BASS_CHMAP,
	},
	[ALC662_FIXUP_BASS_CHMAP] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_bass_chmap,
	},
	[ALC662_FIXUP_ASUS_Nx50] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_auto_mute_via_amp,
		.chained = true,
		.chain_id = ALC662_FIXUP_BASS_1A
	},
	[ALC668_FIXUP_ASUS_Nx51_HEADSET_MODE] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_headset_mode_alc668,
		.chain_id = ALC662_FIXUP_BASS_CHMAP
	},
	[ALC668_FIXUP_ASUS_Nx51] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x19, 0x03a1913d }, /* use as headphone mic, without its own jack detect */
			{ 0x1a, 0x90170151 }, /* bass speaker */
			{ 0x1b, 0x03a1113c }, /* use as headset mic, without its own jack detect */
			{}
		},
		.chained = true,
		.chain_id = ALC668_FIXUP_ASUS_Nx51_HEADSET_MODE,
	},
	[ALC668_FIXUP_MIC_COEF] = {
		.type = HDA_FIXUP_VERBS,
		.v.verbs = (const struct hda_verb[]) {
			{ 0x20, AC_VERB_SET_COEF_INDEX, 0xc3 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x4000 },
			{}
		},
	},
	[ALC668_FIXUP_ASUS_G751] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x16, 0x0421101f }, /* HP */
			{}
		},
		.chained = true,
		.chain_id = ALC668_FIXUP_MIC_COEF
	},
	[ALC891_FIXUP_HEADSET_MODE] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc_fixup_headset_mode,
	},
	[ALC891_FIXUP_DELL_MIC_NO_PRESENCE] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x19, 0x03a1913d }, /* use as headphone mic, without its own jack detect */
			{ 0x1b, 0x03a1113c }, /* use as headset mic, without its own jack detect */
			{ }
		},
		.chained = true,
		.chain_id = ALC891_FIXUP_HEADSET_MODE
	},
	[ALC662_FIXUP_ACER_VERITON] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x15, 0x50170120 }, /* no internal speaker */
			{ }
		}
	},
	[ALC892_FIXUP_ASROCK_MOBO] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x15, 0x40f000f0 }, /* disabled */
			{ 0x16, 0x40f000f0 }, /* disabled */
			{ }
		}
	},
	[ALC662_FIXUP_USI_FUNC] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc662_fixup_usi_headset_mic,
	},
	[ALC662_FIXUP_USI_HEADSET_MODE] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x19, 0x02a1913c }, /* use as headset mic, without its own jack detect */
			{ 0x18, 0x01a1903d },
			{ }
		},
		.chained = true,
		.chain_id = ALC662_FIXUP_USI_FUNC
	},
	[ALC662_FIXUP_LENOVO_MULTI_CODECS] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc233_alc662_fixup_lenovo_dual_codecs,
	},
	[ALC669_FIXUP_ACER_ASPIRE_ETHOS_HEADSET] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc662_fixup_aspire_ethos_hp,
	},
	[ALC669_FIXUP_ACER_ASPIRE_ETHOS] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x15, 0x92130110 }, /* front speakers */
			{ 0x18, 0x99130111 }, /* center/subwoofer */
			{ 0x1b, 0x11130012 }, /* surround plus jack for HP */
			{ }
		},
		.chained = true,
		.chain_id = ALC669_FIXUP_ACER_ASPIRE_ETHOS_HEADSET
	},
	[ALC671_FIXUP_HP_HEADSET_MIC2] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc671_fixup_hp_headset_mic2,
	},
	[ALC662_FIXUP_ACER_X2660G_HEADSET_MODE] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x1a, 0x02a1113c }, /* use as headset mic, without its own jack detect */
			{ }
		},
		.chained = true,
		.chain_id = ALC662_FIXUP_USI_FUNC
	},
	[ALC662_FIXUP_ACER_NITRO_HEADSET_MODE] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x1a, 0x01a11140 }, /* use as headset mic, without its own jack detect */
			{ 0x1b, 0x0221144f },
			{ }
		},
		.chained = true,
		.chain_id = ALC662_FIXUP_USI_FUNC
	},
	[ALC668_FIXUP_ASUS_NO_HEADSET_MIC] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x1b, 0x04a1112c },
			{ }
		},
		.chained = true,
		.chain_id = ALC668_FIXUP_HEADSET_MIC
	},
	[ALC668_FIXUP_HEADSET_MIC] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc269_fixup_headset_mic,
		.chained = true,
		.chain_id = ALC668_FIXUP_MIC_DET_COEF
	},
	[ALC668_FIXUP_MIC_DET_COEF] = {
		.type = HDA_FIXUP_VERBS,
		.v.verbs = (const struct hda_verb[]) {
			{ 0x20, AC_VERB_SET_COEF_INDEX, 0x15 },
			{ 0x20, AC_VERB_SET_PROC_COEF, 0x0d60 },
			{}
		},
	},
	[ALC897_FIXUP_LENOVO_HEADSET_MIC] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc897_fixup_lenovo_headset_mic,
	},
	[ALC897_FIXUP_HEADSET_MIC_PIN] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x1a, 0x03a11050 },
			{ }
		},
		.chained = true,
		.chain_id = ALC897_FIXUP_LENOVO_HEADSET_MIC
	},
	[ALC897_FIXUP_HP_HSMIC_VERB] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x19, 0x01a1913c }, /* use as headset mic, without its own jack detect */
			{ }
		},
	},
	[ALC897_FIXUP_LENOVO_HEADSET_MODE] = {
		.type = HDA_FIXUP_FUNC,
		.v.func = alc897_fixup_lenovo_headset_mode,
	},
	[ALC897_FIXUP_HEADSET_MIC_PIN2] = {
		.type = HDA_FIXUP_PINS,
		.v.pins = (const struct hda_pintbl[]) {
			{ 0x1a, 0x01a11140 }, /* use as headset mic, without its own jack detect */
			{ }
		},
		.chained = true,
		.chain_id = ALC897_FIXUP_LENOVO_HEADSET_MODE
	},
	[ALC897_FIXUP_UNIS_H3C_X500S] = {
		.type = HDA_FIXUP_VERBS,
		.v.verbs = (const struct hda_verb[]) {
			{ 0x14, AC_VERB_SET_EAPD_BTLENABLE, 0 },
			{}
		},
	},
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static const struct snd_pci_quirk alc662_fixup_tbl[] = {
	SND_PCI_QUIRK(0x1019, 0x9087, "ECS", ALC662_FIXUP_ASUS_MODE2),
<<<<<<< HEAD
	SND_PCI_QUIRK(0x1025, 0x0308, "Acer Aspire 8942G", ALC662_FIXUP_ASPIRE),
	SND_PCI_QUIRK(0x1025, 0x031c, "Gateway NV79", ALC662_FIXUP_SKU_IGNORE),
	SND_PCI_QUIRK(0x1025, 0x038b, "Acer Aspire 8943G", ALC662_FIXUP_ASPIRE),
	SND_PCI_QUIRK(0x103c, 0x1632, "HP RP5800", ALC662_FIXUP_HP_RP5800),
	SND_PCI_QUIRK(0x1043, 0x1477, "ASUS N56VZ", ALC662_FIXUP_ASUS_MODE4),
	SND_PCI_QUIRK(0x1043, 0x1bf3, "ASUS N76VZ", ALC662_FIXUP_ASUS_MODE4),
	SND_PCI_QUIRK(0x1043, 0x8469, "ASUS mobo", ALC662_FIXUP_NO_JACK_DETECT),
	SND_PCI_QUIRK(0x105b, 0x0cd6, "Foxconn", ALC662_FIXUP_ASUS_MODE2),
	SND_PCI_QUIRK(0x144d, 0xc051, "Samsung R720", ALC662_FIXUP_IDEAPAD),
	SND_PCI_QUIRK(0x17aa, 0x38af, "Lenovo Ideapad Y550P", ALC662_FIXUP_IDEAPAD),
	SND_PCI_QUIRK(0x17aa, 0x3a0d, "Lenovo Ideapad Y550", ALC662_FIXUP_IDEAPAD),
	SND_PCI_QUIRK(0x19da, 0xa130, "Zotac Z68", ALC662_FIXUP_ZOTAC_Z68),
	SND_PCI_QUIRK(0x1b35, 0x2206, "CZC P10T", ALC662_FIXUP_CZC_P10T),
=======
	SND_PCI_QUIRK(0x1025, 0x022f, "Acer Aspire One", ALC662_FIXUP_INV_DMIC),
	SND_PCI_QUIRK(0x1025, 0x0241, "Packard Bell DOTS", ALC662_FIXUP_INV_DMIC),
	SND_PCI_QUIRK(0x1025, 0x0308, "Acer Aspire 8942G", ALC662_FIXUP_ASPIRE),
	SND_PCI_QUIRK(0x1025, 0x031c, "Gateway NV79", ALC662_FIXUP_SKU_IGNORE),
	SND_PCI_QUIRK(0x1025, 0x0349, "eMachines eM250", ALC662_FIXUP_INV_DMIC),
	SND_PCI_QUIRK(0x1025, 0x034a, "Gateway LT27", ALC662_FIXUP_INV_DMIC),
	SND_PCI_QUIRK(0x1025, 0x038b, "Acer Aspire 8943G", ALC662_FIXUP_ASPIRE),
	SND_PCI_QUIRK(0x1025, 0x0566, "Acer Aspire Ethos 8951G", ALC669_FIXUP_ACER_ASPIRE_ETHOS),
	SND_PCI_QUIRK(0x1025, 0x123c, "Acer Nitro N50-600", ALC662_FIXUP_ACER_NITRO_HEADSET_MODE),
	SND_PCI_QUIRK(0x1025, 0x124e, "Acer 2660G", ALC662_FIXUP_ACER_X2660G_HEADSET_MODE),
	SND_PCI_QUIRK(0x1028, 0x05d8, "Dell", ALC668_FIXUP_DELL_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1028, 0x05db, "Dell", ALC668_FIXUP_DELL_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1028, 0x05fe, "Dell XPS 15", ALC668_FIXUP_DELL_XPS13),
	SND_PCI_QUIRK(0x1028, 0x060a, "Dell XPS 13", ALC668_FIXUP_DELL_XPS13),
	SND_PCI_QUIRK(0x1028, 0x060d, "Dell M3800", ALC668_FIXUP_DELL_XPS13),
	SND_PCI_QUIRK(0x1028, 0x0625, "Dell", ALC668_FIXUP_DELL_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1028, 0x0626, "Dell", ALC668_FIXUP_DELL_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1028, 0x0696, "Dell", ALC668_FIXUP_DELL_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1028, 0x0698, "Dell", ALC668_FIXUP_DELL_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x1028, 0x069f, "Dell", ALC668_FIXUP_DELL_MIC_NO_PRESENCE),
	SND_PCI_QUIRK(0x103c, 0x1632, "HP RP5800", ALC662_FIXUP_HP_RP5800),
	SND_PCI_QUIRK(0x103c, 0x870c, "HP", ALC897_FIXUP_HP_HSMIC_VERB),
	SND_PCI_QUIRK(0x103c, 0x8719, "HP", ALC897_FIXUP_HP_HSMIC_VERB),
	SND_PCI_QUIRK(0x103c, 0x872b, "HP", ALC897_FIXUP_HP_HSMIC_VERB),
	SND_PCI_QUIRK(0x103c, 0x873e, "HP", ALC671_FIXUP_HP_HEADSET_MIC2),
	SND_PCI_QUIRK(0x103c, 0x8768, "HP Slim Desktop S01", ALC671_FIXUP_HP_HEADSET_MIC2),
	SND_PCI_QUIRK(0x103c, 0x877e, "HP 288 Pro G6", ALC671_FIXUP_HP_HEADSET_MIC2),
	SND_PCI_QUIRK(0x103c, 0x885f, "HP 288 Pro G8", ALC671_FIXUP_HP_HEADSET_MIC2),
	SND_PCI_QUIRK(0x1043, 0x1080, "Asus UX501VW", ALC668_FIXUP_HEADSET_MODE),
	SND_PCI_QUIRK(0x1043, 0x11cd, "Asus N550", ALC662_FIXUP_ASUS_Nx50),
	SND_PCI_QUIRK(0x1043, 0x129d, "Asus N750", ALC662_FIXUP_ASUS_Nx50),
	SND_PCI_QUIRK(0x1043, 0x12ff, "ASUS G751", ALC668_FIXUP_ASUS_G751),
	SND_PCI_QUIRK(0x1043, 0x13df, "Asus N550JX", ALC662_FIXUP_BASS_1A),
	SND_PCI_QUIRK(0x1043, 0x1477, "ASUS N56VZ", ALC662_FIXUP_BASS_MODE4_CHMAP),
	SND_PCI_QUIRK(0x1043, 0x15a7, "ASUS UX51VZH", ALC662_FIXUP_BASS_16),
	SND_PCI_QUIRK(0x1043, 0x177d, "ASUS N551", ALC668_FIXUP_ASUS_Nx51),
	SND_PCI_QUIRK(0x1043, 0x17bd, "ASUS N751", ALC668_FIXUP_ASUS_Nx51),
	SND_PCI_QUIRK(0x1043, 0x185d, "ASUS G551JW", ALC668_FIXUP_ASUS_NO_HEADSET_MIC),
	SND_PCI_QUIRK(0x1043, 0x1963, "ASUS X71SL", ALC662_FIXUP_ASUS_MODE8),
	SND_PCI_QUIRK(0x1043, 0x1b73, "ASUS N55SF", ALC662_FIXUP_BASS_16),
	SND_PCI_QUIRK(0x1043, 0x1bf3, "ASUS N76VZ", ALC662_FIXUP_BASS_MODE4_CHMAP),
	SND_PCI_QUIRK(0x1043, 0x8469, "ASUS mobo", ALC662_FIXUP_NO_JACK_DETECT),
	SND_PCI_QUIRK(0x105b, 0x0cd6, "Foxconn", ALC662_FIXUP_ASUS_MODE2),
	SND_PCI_QUIRK(0x144d, 0xc051, "Samsung R720", ALC662_FIXUP_IDEAPAD),
	SND_PCI_QUIRK(0x14cd, 0x5003, "USI", ALC662_FIXUP_USI_HEADSET_MODE),
	SND_PCI_QUIRK(0x17aa, 0x1036, "Lenovo P520", ALC662_FIXUP_LENOVO_MULTI_CODECS),
	SND_PCI_QUIRK(0x17aa, 0x1057, "Lenovo P360", ALC897_FIXUP_HEADSET_MIC_PIN),
	SND_PCI_QUIRK(0x17aa, 0x1064, "Lenovo P3 Tower", ALC897_FIXUP_HEADSET_MIC_PIN),
	SND_PCI_QUIRK(0x17aa, 0x32ca, "Lenovo ThinkCentre M80", ALC897_FIXUP_HEADSET_MIC_PIN),
	SND_PCI_QUIRK(0x17aa, 0x32cb, "Lenovo ThinkCentre M70", ALC897_FIXUP_HEADSET_MIC_PIN),
	SND_PCI_QUIRK(0x17aa, 0x32cf, "Lenovo ThinkCentre M950", ALC897_FIXUP_HEADSET_MIC_PIN),
	SND_PCI_QUIRK(0x17aa, 0x32f7, "Lenovo ThinkCentre M90", ALC897_FIXUP_HEADSET_MIC_PIN),
	SND_PCI_QUIRK(0x17aa, 0x3321, "Lenovo ThinkCentre M70 Gen4", ALC897_FIXUP_HEADSET_MIC_PIN),
	SND_PCI_QUIRK(0x17aa, 0x331b, "Lenovo ThinkCentre M90 Gen4", ALC897_FIXUP_HEADSET_MIC_PIN),
	SND_PCI_QUIRK(0x17aa, 0x3364, "Lenovo ThinkCentre M90 Gen5", ALC897_FIXUP_HEADSET_MIC_PIN),
	SND_PCI_QUIRK(0x17aa, 0x3742, "Lenovo TianYi510Pro-14IOB", ALC897_FIXUP_HEADSET_MIC_PIN2),
	SND_PCI_QUIRK(0x17aa, 0x38af, "Lenovo Ideapad Y550P", ALC662_FIXUP_IDEAPAD),
	SND_PCI_QUIRK(0x17aa, 0x3a0d, "Lenovo Ideapad Y550", ALC662_FIXUP_IDEAPAD),
	SND_PCI_QUIRK(0x1849, 0x5892, "ASRock B150M", ALC892_FIXUP_ASROCK_MOBO),
	SND_PCI_QUIRK(0x19da, 0xa130, "Zotac Z68", ALC662_FIXUP_ZOTAC_Z68),
	SND_PCI_QUIRK(0x1b0a, 0x01b8, "ACER Veriton", ALC662_FIXUP_ACER_VERITON),
	SND_PCI_QUIRK(0x1b35, 0x1234, "CZC ET26", ALC662_FIXUP_CZC_ET26),
	SND_PCI_QUIRK(0x1b35, 0x2206, "CZC P10T", ALC662_FIXUP_CZC_P10T),
	SND_PCI_QUIRK(0x1c6c, 0x1239, "Compaq N14JP6-V2", ALC897_FIXUP_HP_HSMIC_VERB),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#if 0
	/* Below is a quirk table taken from the old code.
	 * Basically the device should work as is without the fixup table.
	 * If BIOS doesn't give a proper info, enable the corresponding
	 * fixup entry.
	 */
	SND_PCI_QUIRK(0x1043, 0x1000, "ASUS N50Vm", ALC662_FIXUP_ASUS_MODE1),
	SND_PCI_QUIRK(0x1043, 0x1092, "ASUS NB", ALC662_FIXUP_ASUS_MODE3),
	SND_PCI_QUIRK(0x1043, 0x1173, "ASUS K73Jn", ALC662_FIXUP_ASUS_MODE1),
	SND_PCI_QUIRK(0x1043, 0x11c3, "ASUS M70V", ALC662_FIXUP_ASUS_MODE3),
	SND_PCI_QUIRK(0x1043, 0x11d3, "ASUS NB", ALC662_FIXUP_ASUS_MODE1),
	SND_PCI_QUIRK(0x1043, 0x11f3, "ASUS NB", ALC662_FIXUP_ASUS_MODE2),
	SND_PCI_QUIRK(0x1043, 0x1203, "ASUS NB", ALC662_FIXUP_ASUS_MODE1),
	SND_PCI_QUIRK(0x1043, 0x1303, "ASUS G60J", ALC662_FIXUP_ASUS_MODE1),
	SND_PCI_QUIRK(0x1043, 0x1333, "ASUS G60Jx", ALC662_FIXUP_ASUS_MODE1),
	SND_PCI_QUIRK(0x1043, 0x1339, "ASUS NB", ALC662_FIXUP_ASUS_MODE2),
	SND_PCI_QUIRK(0x1043, 0x13e3, "ASUS N71JA", ALC662_FIXUP_ASUS_MODE7),
	SND_PCI_QUIRK(0x1043, 0x1463, "ASUS N71", ALC662_FIXUP_ASUS_MODE7),
	SND_PCI_QUIRK(0x1043, 0x14d3, "ASUS G72", ALC662_FIXUP_ASUS_MODE8),
	SND_PCI_QUIRK(0x1043, 0x1563, "ASUS N90", ALC662_FIXUP_ASUS_MODE3),
	SND_PCI_QUIRK(0x1043, 0x15d3, "ASUS N50SF F50SF", ALC662_FIXUP_ASUS_MODE1),
	SND_PCI_QUIRK(0x1043, 0x16c3, "ASUS NB", ALC662_FIXUP_ASUS_MODE2),
	SND_PCI_QUIRK(0x1043, 0x16f3, "ASUS K40C K50C", ALC662_FIXUP_ASUS_MODE2),
	SND_PCI_QUIRK(0x1043, 0x1733, "ASUS N81De", ALC662_FIXUP_ASUS_MODE1),
	SND_PCI_QUIRK(0x1043, 0x1753, "ASUS NB", ALC662_FIXUP_ASUS_MODE2),
	SND_PCI_QUIRK(0x1043, 0x1763, "ASUS NB", ALC662_FIXUP_ASUS_MODE6),
	SND_PCI_QUIRK(0x1043, 0x1765, "ASUS NB", ALC662_FIXUP_ASUS_MODE6),
	SND_PCI_QUIRK(0x1043, 0x1783, "ASUS NB", ALC662_FIXUP_ASUS_MODE2),
	SND_PCI_QUIRK(0x1043, 0x1793, "ASUS F50GX", ALC662_FIXUP_ASUS_MODE1),
	SND_PCI_QUIRK(0x1043, 0x17b3, "ASUS F70SL", ALC662_FIXUP_ASUS_MODE3),
	SND_PCI_QUIRK(0x1043, 0x17f3, "ASUS X58LE", ALC662_FIXUP_ASUS_MODE2),
	SND_PCI_QUIRK(0x1043, 0x1813, "ASUS NB", ALC662_FIXUP_ASUS_MODE2),
	SND_PCI_QUIRK(0x1043, 0x1823, "ASUS NB", ALC662_FIXUP_ASUS_MODE5),
	SND_PCI_QUIRK(0x1043, 0x1833, "ASUS NB", ALC662_FIXUP_ASUS_MODE6),
	SND_PCI_QUIRK(0x1043, 0x1843, "ASUS NB", ALC662_FIXUP_ASUS_MODE2),
	SND_PCI_QUIRK(0x1043, 0x1853, "ASUS F50Z", ALC662_FIXUP_ASUS_MODE1),
	SND_PCI_QUIRK(0x1043, 0x1864, "ASUS NB", ALC662_FIXUP_ASUS_MODE2),
	SND_PCI_QUIRK(0x1043, 0x1876, "ASUS NB", ALC662_FIXUP_ASUS_MODE2),
	SND_PCI_QUIRK(0x1043, 0x1893, "ASUS M50Vm", ALC662_FIXUP_ASUS_MODE3),
	SND_PCI_QUIRK(0x1043, 0x1894, "ASUS X55", ALC662_FIXUP_ASUS_MODE3),
	SND_PCI_QUIRK(0x1043, 0x18b3, "ASUS N80Vc", ALC662_FIXUP_ASUS_MODE1),
	SND_PCI_QUIRK(0x1043, 0x18c3, "ASUS VX5", ALC662_FIXUP_ASUS_MODE1),
	SND_PCI_QUIRK(0x1043, 0x18d3, "ASUS N81Te", ALC662_FIXUP_ASUS_MODE1),
	SND_PCI_QUIRK(0x1043, 0x18f3, "ASUS N505Tp", ALC662_FIXUP_ASUS_MODE1),
	SND_PCI_QUIRK(0x1043, 0x1903, "ASUS F5GL", ALC662_FIXUP_ASUS_MODE1),
	SND_PCI_QUIRK(0x1043, 0x1913, "ASUS NB", ALC662_FIXUP_ASUS_MODE2),
	SND_PCI_QUIRK(0x1043, 0x1933, "ASUS F80Q", ALC662_FIXUP_ASUS_MODE2),
	SND_PCI_QUIRK(0x1043, 0x1943, "ASUS Vx3V", ALC662_FIXUP_ASUS_MODE1),
	SND_PCI_QUIRK(0x1043, 0x1953, "ASUS NB", ALC662_FIXUP_ASUS_MODE1),
	SND_PCI_QUIRK(0x1043, 0x1963, "ASUS X71C", ALC662_FIXUP_ASUS_MODE3),
	SND_PCI_QUIRK(0x1043, 0x1983, "ASUS N5051A", ALC662_FIXUP_ASUS_MODE1),
	SND_PCI_QUIRK(0x1043, 0x1993, "ASUS N20", ALC662_FIXUP_ASUS_MODE1),
	SND_PCI_QUIRK(0x1043, 0x19b3, "ASUS F7Z", ALC662_FIXUP_ASUS_MODE1),
	SND_PCI_QUIRK(0x1043, 0x19c3, "ASUS F5Z/F6x", ALC662_FIXUP_ASUS_MODE2),
	SND_PCI_QUIRK(0x1043, 0x19e3, "ASUS NB", ALC662_FIXUP_ASUS_MODE1),
	SND_PCI_QUIRK(0x1043, 0x19f3, "ASUS NB", ALC662_FIXUP_ASUS_MODE4),
#endif
	{}
};

<<<<<<< HEAD
static const struct alc_model_fixup alc662_fixup_models[] = {
	{.id = ALC272_FIXUP_MARIO, .name = "mario"},
=======
static const struct hda_model_fixup alc662_fixup_models[] = {
	{.id = ALC662_FIXUP_ASPIRE, .name = "aspire"},
	{.id = ALC662_FIXUP_IDEAPAD, .name = "ideapad"},
	{.id = ALC272_FIXUP_MARIO, .name = "mario"},
	{.id = ALC662_FIXUP_HP_RP5800, .name = "hp-rp5800"},
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{.id = ALC662_FIXUP_ASUS_MODE1, .name = "asus-mode1"},
	{.id = ALC662_FIXUP_ASUS_MODE2, .name = "asus-mode2"},
	{.id = ALC662_FIXUP_ASUS_MODE3, .name = "asus-mode3"},
	{.id = ALC662_FIXUP_ASUS_MODE4, .name = "asus-mode4"},
	{.id = ALC662_FIXUP_ASUS_MODE5, .name = "asus-mode5"},
	{.id = ALC662_FIXUP_ASUS_MODE6, .name = "asus-mode6"},
	{.id = ALC662_FIXUP_ASUS_MODE7, .name = "asus-mode7"},
	{.id = ALC662_FIXUP_ASUS_MODE8, .name = "asus-mode8"},
<<<<<<< HEAD
	{}
};

=======
	{.id = ALC662_FIXUP_ZOTAC_Z68, .name = "zotac-z68"},
	{.id = ALC662_FIXUP_INV_DMIC, .name = "inv-dmic"},
	{.id = ALC662_FIXUP_DELL_MIC_NO_PRESENCE, .name = "alc662-headset-multi"},
	{.id = ALC668_FIXUP_DELL_MIC_NO_PRESENCE, .name = "dell-headset-multi"},
	{.id = ALC662_FIXUP_HEADSET_MODE, .name = "alc662-headset"},
	{.id = ALC668_FIXUP_HEADSET_MODE, .name = "alc668-headset"},
	{.id = ALC662_FIXUP_BASS_16, .name = "bass16"},
	{.id = ALC662_FIXUP_BASS_1A, .name = "bass1a"},
	{.id = ALC668_FIXUP_AUTO_MUTE, .name = "automute"},
	{.id = ALC668_FIXUP_DELL_XPS13, .name = "dell-xps13"},
	{.id = ALC662_FIXUP_ASUS_Nx50, .name = "asus-nx50"},
	{.id = ALC668_FIXUP_ASUS_Nx51, .name = "asus-nx51"},
	{.id = ALC668_FIXUP_ASUS_G751, .name = "asus-g751"},
	{.id = ALC891_FIXUP_HEADSET_MODE, .name = "alc891-headset"},
	{.id = ALC891_FIXUP_DELL_MIC_NO_PRESENCE, .name = "alc891-headset-multi"},
	{.id = ALC662_FIXUP_ACER_VERITON, .name = "acer-veriton"},
	{.id = ALC892_FIXUP_ASROCK_MOBO, .name = "asrock-mobo"},
	{.id = ALC662_FIXUP_USI_HEADSET_MODE, .name = "usi-headset"},
	{.id = ALC662_FIXUP_LENOVO_MULTI_CODECS, .name = "dual-codecs"},
	{.id = ALC669_FIXUP_ACER_ASPIRE_ETHOS, .name = "aspire-ethos"},
	{.id = ALC897_FIXUP_UNIS_H3C_X500S, .name = "unis-h3c-x500s"},
	{}
};

static const struct snd_hda_pin_quirk alc662_pin_fixup_tbl[] = {
	SND_HDA_PIN_QUIRK(0x10ec0867, 0x1028, "Dell", ALC891_FIXUP_DELL_MIC_NO_PRESENCE,
		{0x17, 0x02211010},
		{0x18, 0x01a19030},
		{0x1a, 0x01813040},
		{0x21, 0x01014020}),
	SND_HDA_PIN_QUIRK(0x10ec0867, 0x1028, "Dell", ALC891_FIXUP_DELL_MIC_NO_PRESENCE,
		{0x16, 0x01813030},
		{0x17, 0x02211010},
		{0x18, 0x01a19040},
		{0x21, 0x01014020}),
	SND_HDA_PIN_QUIRK(0x10ec0662, 0x1028, "Dell", ALC662_FIXUP_DELL_MIC_NO_PRESENCE,
		{0x14, 0x01014010},
		{0x18, 0x01a19020},
		{0x1a, 0x0181302f},
		{0x1b, 0x0221401f}),
	SND_HDA_PIN_QUIRK(0x10ec0668, 0x1028, "Dell", ALC668_FIXUP_AUTO_MUTE,
		{0x12, 0x99a30130},
		{0x14, 0x90170110},
		{0x15, 0x0321101f},
		{0x16, 0x03011020}),
	SND_HDA_PIN_QUIRK(0x10ec0668, 0x1028, "Dell", ALC668_FIXUP_AUTO_MUTE,
		{0x12, 0x99a30140},
		{0x14, 0x90170110},
		{0x15, 0x0321101f},
		{0x16, 0x03011020}),
	SND_HDA_PIN_QUIRK(0x10ec0668, 0x1028, "Dell", ALC668_FIXUP_AUTO_MUTE,
		{0x12, 0x99a30150},
		{0x14, 0x90170110},
		{0x15, 0x0321101f},
		{0x16, 0x03011020}),
	SND_HDA_PIN_QUIRK(0x10ec0668, 0x1028, "Dell", ALC668_FIXUP_AUTO_MUTE,
		{0x14, 0x90170110},
		{0x15, 0x0321101f},
		{0x16, 0x03011020}),
	SND_HDA_PIN_QUIRK(0x10ec0668, 0x1028, "Dell XPS 15", ALC668_FIXUP_AUTO_MUTE,
		{0x12, 0x90a60130},
		{0x14, 0x90170110},
		{0x15, 0x0321101f}),
	SND_HDA_PIN_QUIRK(0x10ec0671, 0x103c, "HP cPC", ALC671_FIXUP_HP_HEADSET_MIC2,
		{0x14, 0x01014010},
		{0x17, 0x90170150},
		{0x19, 0x02a11060},
		{0x1b, 0x01813030},
		{0x21, 0x02211020}),
	SND_HDA_PIN_QUIRK(0x10ec0671, 0x103c, "HP cPC", ALC671_FIXUP_HP_HEADSET_MIC2,
		{0x14, 0x01014010},
		{0x18, 0x01a19040},
		{0x1b, 0x01813030},
		{0x21, 0x02211020}),
	SND_HDA_PIN_QUIRK(0x10ec0671, 0x103c, "HP cPC", ALC671_FIXUP_HP_HEADSET_MIC2,
		{0x14, 0x01014020},
		{0x17, 0x90170110},
		{0x18, 0x01a19050},
		{0x1b, 0x01813040},
		{0x21, 0x02211030}),
	{}
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 */
static int patch_alc662(struct hda_codec *codec)
{
	struct alc_spec *spec;
<<<<<<< HEAD
	int err = 0;

	spec = kzalloc(sizeof(*spec), GFP_KERNEL);
	if (!spec)
		return -ENOMEM;

	codec->spec = spec;

	spec->mixer_nid = 0x0b;
=======
	int err;

	err = alc_alloc_spec(codec, 0x0b);
	if (err < 0)
		return err;

	spec = codec->spec;

	spec->shutup = alc_eapd_shutup;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* handle multiple HPs as is */
	spec->parse_flags = HDA_PINCFG_NO_HP_FIXUP;

	alc_fix_pll_init(codec, 0x20, 0x04, 15);

<<<<<<< HEAD
	err = alc_codec_rename_from_preset(codec);
	if (err < 0)
		goto error;

	if ((alc_get_coef0(codec) & (1 << 14)) &&
	    codec->bus->pci->subsystem_vendor == 0x1025 &&
	    spec->cdefine.platform_type == 1) {
		if (alc_codec_rename(codec, "ALC272X") < 0)
			goto error;
	}

	alc_pick_fixup(codec, alc662_fixup_models,
		       alc662_fixup_tbl, alc662_fixups);
	alc_apply_fixup(codec, ALC_FIXUP_ACT_PRE_PROBE);

	alc_auto_parse_customize_define(codec);

=======
	switch (codec->core.vendor_id) {
	case 0x10ec0668:
		spec->init_hook = alc668_restore_default_value;
		break;
	}

	alc_pre_init(codec);

	snd_hda_pick_fixup(codec, alc662_fixup_models,
		       alc662_fixup_tbl, alc662_fixups);
	snd_hda_pick_pin_fixup(codec, alc662_pin_fixup_tbl, alc662_fixups, true);
	snd_hda_apply_fixup(codec, HDA_FIXUP_ACT_PRE_PROBE);

	alc_auto_parse_customize_define(codec);

	if (has_cdefine_beep(codec))
		spec->gen.beep_nid = 0x01;

	if ((alc_get_coef0(codec) & (1 << 14)) &&
	    codec->bus->pci && codec->bus->pci->subsystem_vendor == 0x1025 &&
	    spec->cdefine.platform_type == 1) {
		err = alc_codec_rename(codec, "ALC272X");
		if (err < 0)
			goto error;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* automatic parse from the BIOS config */
	err = alc662_parse_auto_config(codec);
	if (err < 0)
		goto error;

<<<<<<< HEAD
	if (!spec->no_analog && has_cdefine_beep(codec)) {
		err = snd_hda_attach_beep_device(codec, 0x1);
		if (err < 0)
			goto error;
		switch (codec->vendor_id) {
		case 0x10ec0662:
			set_beep_amp(spec, 0x0b, 0x05, HDA_INPUT);
=======
	if (!spec->gen.no_analog && spec->gen.beep_nid) {
		switch (codec->core.vendor_id) {
		case 0x10ec0662:
			err = set_beep_amp(spec, 0x0b, 0x05, HDA_INPUT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		case 0x10ec0272:
		case 0x10ec0663:
		case 0x10ec0665:
		case 0x10ec0668:
<<<<<<< HEAD
			set_beep_amp(spec, 0x0b, 0x04, HDA_INPUT);
			break;
		case 0x10ec0273:
			set_beep_amp(spec, 0x0b, 0x03, HDA_INPUT);
			break;
		}
	}

	codec->patch_ops = alc_patch_ops;
	spec->shutup = alc_eapd_shutup;

	alc_apply_fixup(codec, ALC_FIXUP_ACT_PROBE);
=======
			err = set_beep_amp(spec, 0x0b, 0x04, HDA_INPUT);
			break;
		case 0x10ec0273:
			err = set_beep_amp(spec, 0x0b, 0x03, HDA_INPUT);
			break;
		}
		if (err < 0)
			goto error;
	}

	snd_hda_apply_fixup(codec, HDA_FIXUP_ACT_PROBE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;

 error:
	alc_free(codec);
	return err;
}

/*
 * ALC680 support
 */

static int alc680_parse_auto_config(struct hda_codec *codec)
{
	return alc_parse_auto_config(codec, NULL, NULL);
}

/*
 */
static int patch_alc680(struct hda_codec *codec)
{
<<<<<<< HEAD
	struct alc_spec *spec;
	int err;

	spec = kzalloc(sizeof(*spec), GFP_KERNEL);
	if (spec == NULL)
		return -ENOMEM;

	codec->spec = spec;

	/* ALC680 has no aa-loopback mixer */
=======
	int err;

	/* ALC680 has no aa-loopback mixer */
	err = alc_alloc_spec(codec, 0);
	if (err < 0)
		return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* automatic parse from the BIOS config */
	err = alc680_parse_auto_config(codec);
	if (err < 0) {
		alc_free(codec);
		return err;
	}

<<<<<<< HEAD
	codec->patch_ops = alc_patch_ops;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

/*
 * patch entries
 */
<<<<<<< HEAD
static const struct hda_codec_preset snd_hda_preset_realtek[] = {
	{ .id = 0x10ec0221, .name = "ALC221", .patch = patch_alc269 },
	{ .id = 0x10ec0231, .name = "ALC231", .patch = patch_alc269 },
	{ .id = 0x10ec0260, .name = "ALC260", .patch = patch_alc260 },
	{ .id = 0x10ec0262, .name = "ALC262", .patch = patch_alc262 },
	{ .id = 0x10ec0267, .name = "ALC267", .patch = patch_alc268 },
	{ .id = 0x10ec0268, .name = "ALC268", .patch = patch_alc268 },
	{ .id = 0x10ec0269, .name = "ALC269", .patch = patch_alc269 },
	{ .id = 0x10ec0270, .name = "ALC270", .patch = patch_alc269 },
	{ .id = 0x10ec0272, .name = "ALC272", .patch = patch_alc662 },
	{ .id = 0x10ec0275, .name = "ALC275", .patch = patch_alc269 },
	{ .id = 0x10ec0276, .name = "ALC276", .patch = patch_alc269 },
	{ .id = 0x10ec0280, .name = "ALC280", .patch = patch_alc269 },
	{ .id = 0x10ec0282, .name = "ALC282", .patch = patch_alc269 },
	{ .id = 0x10ec0283, .name = "ALC283", .patch = patch_alc269 },
	{ .id = 0x10ec0290, .name = "ALC290", .patch = patch_alc269 },
	{ .id = 0x10ec0292, .name = "ALC292", .patch = patch_alc269 },
	{ .id = 0x10ec0861, .rev = 0x100340, .name = "ALC660",
	  .patch = patch_alc861 },
	{ .id = 0x10ec0660, .name = "ALC660-VD", .patch = patch_alc861vd },
	{ .id = 0x10ec0861, .name = "ALC861", .patch = patch_alc861 },
	{ .id = 0x10ec0862, .name = "ALC861-VD", .patch = patch_alc861vd },
	{ .id = 0x10ec0662, .rev = 0x100002, .name = "ALC662 rev2",
	  .patch = patch_alc882 },
	{ .id = 0x10ec0662, .rev = 0x100101, .name = "ALC662 rev1",
	  .patch = patch_alc662 },
	{ .id = 0x10ec0662, .rev = 0x100300, .name = "ALC662 rev3",
	  .patch = patch_alc662 },
	{ .id = 0x10ec0663, .name = "ALC663", .patch = patch_alc662 },
	{ .id = 0x10ec0665, .name = "ALC665", .patch = patch_alc662 },
	{ .id = 0x10ec0668, .name = "ALC668", .patch = patch_alc662 },
	{ .id = 0x10ec0670, .name = "ALC670", .patch = patch_alc662 },
	{ .id = 0x10ec0671, .name = "ALC671", .patch = patch_alc662 },
	{ .id = 0x10ec0680, .name = "ALC680", .patch = patch_alc680 },
	{ .id = 0x10ec0867, .name = "ALC891", .patch = patch_alc882 },
	{ .id = 0x10ec0880, .name = "ALC880", .patch = patch_alc880 },
	{ .id = 0x10ec0882, .name = "ALC882", .patch = patch_alc882 },
	{ .id = 0x10ec0883, .name = "ALC883", .patch = patch_alc882 },
	{ .id = 0x10ec0885, .rev = 0x100101, .name = "ALC889A",
	  .patch = patch_alc882 },
	{ .id = 0x10ec0885, .rev = 0x100103, .name = "ALC889A",
	  .patch = patch_alc882 },
	{ .id = 0x10ec0885, .name = "ALC885", .patch = patch_alc882 },
	{ .id = 0x10ec0887, .name = "ALC887", .patch = patch_alc882 },
	{ .id = 0x10ec0888, .rev = 0x100101, .name = "ALC1200",
	  .patch = patch_alc882 },
	{ .id = 0x10ec0888, .name = "ALC888", .patch = patch_alc882 },
	{ .id = 0x10ec0889, .name = "ALC889", .patch = patch_alc882 },
	{ .id = 0x10ec0892, .name = "ALC892", .patch = patch_alc662 },
	{ .id = 0x10ec0899, .name = "ALC898", .patch = patch_alc882 },
	{ .id = 0x10ec0900, .name = "ALC1150", .patch = patch_alc882 },
	{} /* terminator */
};

MODULE_ALIAS("snd-hda-codec-id:10ec*");

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Realtek HD-audio codec");

static struct hda_codec_preset_list realtek_list = {
	.preset = snd_hda_preset_realtek,
	.owner = THIS_MODULE,
};

static int __init patch_realtek_init(void)
{
	return snd_hda_add_codec_preset(&realtek_list);
}

static void __exit patch_realtek_exit(void)
{
	snd_hda_delete_codec_preset(&realtek_list);
}

module_init(patch_realtek_init)
module_exit(patch_realtek_exit)
=======
static const struct hda_device_id snd_hda_id_realtek[] = {
	HDA_CODEC_ENTRY(0x10ec0215, "ALC215", patch_alc269),
	HDA_CODEC_ENTRY(0x10ec0221, "ALC221", patch_alc269),
	HDA_CODEC_ENTRY(0x10ec0222, "ALC222", patch_alc269),
	HDA_CODEC_ENTRY(0x10ec0225, "ALC225", patch_alc269),
	HDA_CODEC_ENTRY(0x10ec0230, "ALC236", patch_alc269),
	HDA_CODEC_ENTRY(0x10ec0231, "ALC231", patch_alc269),
	HDA_CODEC_ENTRY(0x10ec0233, "ALC233", patch_alc269),
	HDA_CODEC_ENTRY(0x10ec0234, "ALC234", patch_alc269),
	HDA_CODEC_ENTRY(0x10ec0235, "ALC233", patch_alc269),
	HDA_CODEC_ENTRY(0x10ec0236, "ALC236", patch_alc269),
	HDA_CODEC_ENTRY(0x10ec0245, "ALC245", patch_alc269),
	HDA_CODEC_ENTRY(0x10ec0255, "ALC255", patch_alc269),
	HDA_CODEC_ENTRY(0x10ec0256, "ALC256", patch_alc269),
	HDA_CODEC_ENTRY(0x10ec0257, "ALC257", patch_alc269),
	HDA_CODEC_ENTRY(0x10ec0260, "ALC260", patch_alc260),
	HDA_CODEC_ENTRY(0x10ec0262, "ALC262", patch_alc262),
	HDA_CODEC_ENTRY(0x10ec0267, "ALC267", patch_alc268),
	HDA_CODEC_ENTRY(0x10ec0268, "ALC268", patch_alc268),
	HDA_CODEC_ENTRY(0x10ec0269, "ALC269", patch_alc269),
	HDA_CODEC_ENTRY(0x10ec0270, "ALC270", patch_alc269),
	HDA_CODEC_ENTRY(0x10ec0272, "ALC272", patch_alc662),
	HDA_CODEC_ENTRY(0x10ec0274, "ALC274", patch_alc269),
	HDA_CODEC_ENTRY(0x10ec0275, "ALC275", patch_alc269),
	HDA_CODEC_ENTRY(0x10ec0276, "ALC276", patch_alc269),
	HDA_CODEC_ENTRY(0x10ec0280, "ALC280", patch_alc269),
	HDA_CODEC_ENTRY(0x10ec0282, "ALC282", patch_alc269),
	HDA_CODEC_ENTRY(0x10ec0283, "ALC283", patch_alc269),
	HDA_CODEC_ENTRY(0x10ec0284, "ALC284", patch_alc269),
	HDA_CODEC_ENTRY(0x10ec0285, "ALC285", patch_alc269),
	HDA_CODEC_ENTRY(0x10ec0286, "ALC286", patch_alc269),
	HDA_CODEC_ENTRY(0x10ec0287, "ALC287", patch_alc269),
	HDA_CODEC_ENTRY(0x10ec0288, "ALC288", patch_alc269),
	HDA_CODEC_ENTRY(0x10ec0289, "ALC289", patch_alc269),
	HDA_CODEC_ENTRY(0x10ec0290, "ALC290", patch_alc269),
	HDA_CODEC_ENTRY(0x10ec0292, "ALC292", patch_alc269),
	HDA_CODEC_ENTRY(0x10ec0293, "ALC293", patch_alc269),
	HDA_CODEC_ENTRY(0x10ec0294, "ALC294", patch_alc269),
	HDA_CODEC_ENTRY(0x10ec0295, "ALC295", patch_alc269),
	HDA_CODEC_ENTRY(0x10ec0298, "ALC298", patch_alc269),
	HDA_CODEC_ENTRY(0x10ec0299, "ALC299", patch_alc269),
	HDA_CODEC_ENTRY(0x10ec0300, "ALC300", patch_alc269),
	HDA_CODEC_ENTRY(0x10ec0623, "ALC623", patch_alc269),
	HDA_CODEC_REV_ENTRY(0x10ec0861, 0x100340, "ALC660", patch_alc861),
	HDA_CODEC_ENTRY(0x10ec0660, "ALC660-VD", patch_alc861vd),
	HDA_CODEC_ENTRY(0x10ec0861, "ALC861", patch_alc861),
	HDA_CODEC_ENTRY(0x10ec0862, "ALC861-VD", patch_alc861vd),
	HDA_CODEC_REV_ENTRY(0x10ec0662, 0x100002, "ALC662 rev2", patch_alc882),
	HDA_CODEC_REV_ENTRY(0x10ec0662, 0x100101, "ALC662 rev1", patch_alc662),
	HDA_CODEC_REV_ENTRY(0x10ec0662, 0x100300, "ALC662 rev3", patch_alc662),
	HDA_CODEC_ENTRY(0x10ec0663, "ALC663", patch_alc662),
	HDA_CODEC_ENTRY(0x10ec0665, "ALC665", patch_alc662),
	HDA_CODEC_ENTRY(0x10ec0667, "ALC667", patch_alc662),
	HDA_CODEC_ENTRY(0x10ec0668, "ALC668", patch_alc662),
	HDA_CODEC_ENTRY(0x10ec0670, "ALC670", patch_alc662),
	HDA_CODEC_ENTRY(0x10ec0671, "ALC671", patch_alc662),
	HDA_CODEC_ENTRY(0x10ec0680, "ALC680", patch_alc680),
	HDA_CODEC_ENTRY(0x10ec0700, "ALC700", patch_alc269),
	HDA_CODEC_ENTRY(0x10ec0701, "ALC701", patch_alc269),
	HDA_CODEC_ENTRY(0x10ec0703, "ALC703", patch_alc269),
	HDA_CODEC_ENTRY(0x10ec0711, "ALC711", patch_alc269),
	HDA_CODEC_ENTRY(0x10ec0867, "ALC891", patch_alc662),
	HDA_CODEC_ENTRY(0x10ec0880, "ALC880", patch_alc880),
	HDA_CODEC_ENTRY(0x10ec0882, "ALC882", patch_alc882),
	HDA_CODEC_ENTRY(0x10ec0883, "ALC883", patch_alc882),
	HDA_CODEC_REV_ENTRY(0x10ec0885, 0x100101, "ALC889A", patch_alc882),
	HDA_CODEC_REV_ENTRY(0x10ec0885, 0x100103, "ALC889A", patch_alc882),
	HDA_CODEC_ENTRY(0x10ec0885, "ALC885", patch_alc882),
	HDA_CODEC_ENTRY(0x10ec0887, "ALC887", patch_alc882),
	HDA_CODEC_REV_ENTRY(0x10ec0888, 0x100101, "ALC1200", patch_alc882),
	HDA_CODEC_ENTRY(0x10ec0888, "ALC888", patch_alc882),
	HDA_CODEC_ENTRY(0x10ec0889, "ALC889", patch_alc882),
	HDA_CODEC_ENTRY(0x10ec0892, "ALC892", patch_alc662),
	HDA_CODEC_ENTRY(0x10ec0897, "ALC897", patch_alc662),
	HDA_CODEC_ENTRY(0x10ec0899, "ALC898", patch_alc882),
	HDA_CODEC_ENTRY(0x10ec0900, "ALC1150", patch_alc882),
	HDA_CODEC_ENTRY(0x10ec0b00, "ALCS1200A", patch_alc882),
	HDA_CODEC_ENTRY(0x10ec1168, "ALC1220", patch_alc882),
	HDA_CODEC_ENTRY(0x10ec1220, "ALC1220", patch_alc882),
	HDA_CODEC_ENTRY(0x19e58326, "HW8326", patch_alc269),
	{} /* terminator */
};
MODULE_DEVICE_TABLE(hdaudio, snd_hda_id_realtek);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Realtek HD-audio codec");
MODULE_IMPORT_NS(SND_HDA_SCODEC_COMPONENT);

static struct hda_codec_driver realtek_driver = {
	.id = snd_hda_id_realtek,
};

module_hda_codec_driver(realtek_driver);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
