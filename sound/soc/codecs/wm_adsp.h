<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * wm_adsp.h  --  Wolfson ADSP support
 *
 * Copyright 2012 Wolfson Microelectronics plc
 *
 * Author: Mark Brown <broonie@opensource.wolfsonmicro.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef __WM_ADSP_H
#define __WM_ADSP_H

<<<<<<< HEAD
#include <linux/circ_buf.h>
=======
#include <linux/firmware/cirrus/cs_dsp.h>
#include <linux/firmware/cirrus/wmfw.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <sound/soc.h>
#include <sound/soc-dapm.h>
#include <sound/compress_driver.h>

<<<<<<< HEAD
#include "wmfw.h"

struct regulator;

struct wm_adsp_region {
	int type;
	unsigned int base;
};

struct wm_adsp_alg_region {
	struct list_head list;
	unsigned int alg;
	int type;
	unsigned int base;
	size_t len;
};

struct wm_adsp_buffer_region {
	unsigned int offset_samps;
	unsigned int cumulative_samps;
	unsigned int mem_type;
	unsigned int base_addr;
};

struct wm_adsp {
	const char *part;
	int num;
	int type;
	struct device *dev;
	struct regmap *regmap;

	int base;

	struct list_head alg_regions;

	int fw_id;

	const struct wm_adsp_region *mem;
	int num_mems;

	int fw;
	bool running;

	struct regulator *dvfs;

	struct wm_coeff *wm_coeff;

	u32 host_buf_ptr;
	u32 low_water_mark;

	int sample_size;
	u32 *raw_capt_buf;
	int raw_buf_samps;
	struct circ_buf capt_buf;
	int capt_buf_size;
	struct wm_adsp_buffer_region *host_regions;
	bool buffer_drain_pending;
};

#define WM_ADSP1(wname, num) \
	{ .id = snd_soc_dapm_pga, .name = wname, .reg = SND_SOC_NOPM, \
	.shift = num, .event = wm_adsp1_event, \
	.event_flags = SND_SOC_DAPM_POST_PMU | SND_SOC_DAPM_PRE_PMD }

#define WM_ADSP2(wname, num) \
{	.id = snd_soc_dapm_pga, .name = wname, .reg = SND_SOC_NOPM, \
	.shift = num, .event = wm_adsp2_event, \
	.event_flags = SND_SOC_DAPM_POST_PMU | SND_SOC_DAPM_PRE_PMD }

extern const struct snd_kcontrol_new wm_adsp_fw_controls[];

int wm_adsp2_init(struct wm_adsp *adsp, bool dvfs);
int wm_adsp1_event(struct snd_soc_dapm_widget *w,
		   struct snd_kcontrol *kcontrol, int event);
int wm_adsp2_event(struct snd_soc_dapm_widget *w,
		   struct snd_kcontrol *kcontrol, int event);

extern bool wm_adsp_compress_supported(const struct wm_adsp* adsp,
				       const struct snd_compr_stream* stream);
extern bool wm_adsp_format_supported(const struct wm_adsp *adsp,
				     const struct snd_compr_stream *stream,
				     const struct snd_compr_params *params);
extern void wm_adsp_get_caps(const struct wm_adsp *adsp,
			     const struct snd_compr_stream *stream,
			     struct snd_compr_caps *caps);

extern int wm_adsp_stream_alloc(struct wm_adsp* adsp,
				const struct snd_compr_params *params);
extern int wm_adsp_stream_free(struct wm_adsp* adsp);
extern int wm_adsp_stream_start(struct wm_adsp *adsp);
extern int wm_adsp_stream_capture(struct wm_adsp *adsp);
extern int wm_adsp_stream_read(struct wm_adsp *adsp, char __user *buf,
			       size_t count);
extern int wm_adsp_stream_avail(const struct wm_adsp *adsp);
=======
/* Return values for wm_adsp_compr_handle_irq */
#define WM_ADSP_COMPR_OK                 0
#define WM_ADSP_COMPR_VOICE_TRIGGER      1

struct wm_adsp_compr;
struct wm_adsp_compr_buf;

struct wm_adsp {
	struct cs_dsp cs_dsp;
	const char *part;
	const char *fwf_name;
	const char *system_name;
	struct snd_soc_component *component;

	unsigned int sys_config_size;

	int fw;
	bool wmfw_optional;

	struct work_struct boot_work;
	int (*pre_run)(struct wm_adsp *dsp);

	bool preloaded;
	bool fatal_error;

	struct list_head compr_list;
	struct list_head buffer_list;

	/*
	 * Flag indicating the preloader widget only needs power toggled
	 * on state change rather than held on for the duration of the
	 * preload, useful for devices that can retain firmware memory
	 * across power down.
	 */
	bool toggle_preload;
};

#define WM_ADSP1(wname, num) \
	SND_SOC_DAPM_PGA_E(wname, SND_SOC_NOPM, num, 0, NULL, 0, \
		wm_adsp1_event, SND_SOC_DAPM_POST_PMU | SND_SOC_DAPM_PRE_PMD)

#define WM_ADSP2_PRELOAD_SWITCH(wname, num) \
	SOC_SINGLE_EXT(wname " Preload Switch", SND_SOC_NOPM, num, 1, 0, \
		wm_adsp2_preloader_get, wm_adsp2_preloader_put)

#define WM_ADSP2(wname, num, event_fn) \
	SND_SOC_DAPM_SPK(wname " Preload", NULL), \
{	.id = snd_soc_dapm_supply, .name = wname " Preloader", \
	.reg = SND_SOC_NOPM, .shift = num, .event = event_fn, \
	.event_flags = SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_PRE_PMD, \
	.subseq = 100, /* Ensure we run after SYSCLK supply widget */ }, \
{	.id = snd_soc_dapm_out_drv, .name = wname, \
	.reg = SND_SOC_NOPM, .shift = num, .event = wm_adsp_event, \
	.event_flags = SND_SOC_DAPM_POST_PMU | SND_SOC_DAPM_PRE_PMD }

#define WM_ADSP_FW_CONTROL(dspname, num) \
	SOC_ENUM_EXT(dspname " Firmware", wm_adsp_fw_enum[num], \
		     wm_adsp_fw_get, wm_adsp_fw_put)

extern const struct soc_enum wm_adsp_fw_enum[];

int wm_adsp1_init(struct wm_adsp *dsp);
int wm_adsp2_init(struct wm_adsp *dsp);
void wm_adsp2_remove(struct wm_adsp *dsp);
int wm_adsp2_component_probe(struct wm_adsp *dsp, struct snd_soc_component *component);
int wm_adsp2_component_remove(struct wm_adsp *dsp, struct snd_soc_component *component);
int wm_halo_init(struct wm_adsp *dsp);

int wm_adsp1_event(struct snd_soc_dapm_widget *w,
		   struct snd_kcontrol *kcontrol, int event);

int wm_adsp_early_event(struct snd_soc_dapm_widget *w,
			struct snd_kcontrol *kcontrol, int event);

int wm_adsp_power_up(struct wm_adsp *dsp, bool load_firmware);
void wm_adsp_power_down(struct wm_adsp *dsp);

irqreturn_t wm_adsp2_bus_error(int irq, void *data);
irqreturn_t wm_halo_bus_error(int irq, void *data);
irqreturn_t wm_halo_wdt_expire(int irq, void *data);

int wm_adsp_run(struct wm_adsp *dsp);
void wm_adsp_stop(struct wm_adsp *dsp);
int wm_adsp_event(struct snd_soc_dapm_widget *w,
		  struct snd_kcontrol *kcontrol, int event);

int wm_adsp2_set_dspclk(struct snd_soc_dapm_widget *w, unsigned int freq);

int wm_adsp2_preloader_get(struct snd_kcontrol *kcontrol,
			   struct snd_ctl_elem_value *ucontrol);
int wm_adsp2_preloader_put(struct snd_kcontrol *kcontrol,
			   struct snd_ctl_elem_value *ucontrol);
int wm_adsp_fw_get(struct snd_kcontrol *kcontrol,
		   struct snd_ctl_elem_value *ucontrol);
int wm_adsp_fw_put(struct snd_kcontrol *kcontrol,
		   struct snd_ctl_elem_value *ucontrol);

int wm_adsp_compr_open(struct wm_adsp *dsp, struct snd_compr_stream *stream);
int wm_adsp_compr_free(struct snd_soc_component *component,
		       struct snd_compr_stream *stream);
int wm_adsp_compr_set_params(struct snd_soc_component *component,
			     struct snd_compr_stream *stream,
			     struct snd_compr_params *params);
int wm_adsp_compr_get_caps(struct snd_soc_component *component,
			   struct snd_compr_stream *stream,
			   struct snd_compr_caps *caps);
int wm_adsp_compr_trigger(struct snd_soc_component *component,
			  struct snd_compr_stream *stream, int cmd);
int wm_adsp_compr_handle_irq(struct wm_adsp *dsp);
int wm_adsp_compr_pointer(struct snd_soc_component *component,
			  struct snd_compr_stream *stream,
			  struct snd_compr_tstamp *tstamp);
int wm_adsp_compr_copy(struct snd_soc_component *component,
		       struct snd_compr_stream *stream,
		       char __user *buf, size_t count);
int wm_adsp_write_ctl(struct wm_adsp *dsp, const char *name,  int type,
		      unsigned int alg, void *buf, size_t len);
int wm_adsp_read_ctl(struct wm_adsp *dsp, const char *name,  int type,
		      unsigned int alg, void *buf, size_t len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif
