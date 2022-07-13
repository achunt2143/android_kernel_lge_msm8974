<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * wm_hubs.h  --  WM899x common code
 *
 * Copyright 2009 Wolfson Microelectronics plc
 *
 * Author: Mark Brown <broonie@opensource.wolfsonmicro.com>
<<<<<<< HEAD
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef _WM_HUBS_H
#define _WM_HUBS_H

#include <linux/completion.h>
#include <linux/interrupt.h>
<<<<<<< HEAD

struct snd_soc_codec;
=======
#include <linux/list.h>
#include <sound/control.h>

struct snd_soc_component;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

extern const unsigned int wm_hubs_spkmix_tlv[];

/* This *must* be the first element of the codec->private_data struct */
struct wm_hubs_data {
	int dcs_codes_l;
	int dcs_codes_r;
	int dcs_readback_mode;
	int hp_startup_mode;
	int series_startup;
	int no_series_update;
<<<<<<< HEAD

	bool no_cache_class_w;
	bool class_w;
	u16 class_w_dcs;
=======
	bool micd_scthr;

	bool no_cache_dac_hp_direct;
	struct list_head dcs_cache;
	bool (*check_class_w_digital)(struct snd_soc_component *);

	int micb1_delay;
	int micb2_delay;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	bool lineout1_se;
	bool lineout1n_ena;
	bool lineout1p_ena;

	bool lineout2_se;
	bool lineout2n_ena;
	bool lineout2p_ena;

	bool dcs_done_irq;
	struct completion dcs_done;
<<<<<<< HEAD
};

extern int wm_hubs_add_analogue_controls(struct snd_soc_codec *);
extern int wm_hubs_add_analogue_routes(struct snd_soc_codec *, int, int);
extern int wm_hubs_handle_analogue_pdata(struct snd_soc_codec *,
					 int lineout1_diff, int lineout2_diff,
					 int lineout1fb, int lineout2fb,
					 int jd_scthr, int jd_thr,
					 int micbias1_lvl, int micbias2_lvl);

extern irqreturn_t wm_hubs_dcs_done(int irq, void *data);
extern void wm_hubs_vmid_ena(struct snd_soc_codec *codec);
extern void wm_hubs_set_bias_level(struct snd_soc_codec *codec,
				   enum snd_soc_bias_level level);
=======

	struct snd_soc_component *component;
};

extern int wm_hubs_add_analogue_controls(struct snd_soc_component *);
extern int wm_hubs_add_analogue_routes(struct snd_soc_component *, int, int);
extern int wm_hubs_handle_analogue_pdata(struct snd_soc_component *,
					 int lineout1_diff, int lineout2_diff,
					 int lineout1fb, int lineout2fb,
					 int jd_scthr, int jd_thr,
					 int micbias1_delay, int micbias2_delay,
					 int micbias1_lvl, int micbias2_lvl);

extern irqreturn_t wm_hubs_dcs_done(int irq, void *data);
extern void wm_hubs_vmid_ena(struct snd_soc_component *component);
extern void wm_hubs_set_bias_level(struct snd_soc_component *component,
				   enum snd_soc_bias_level level);
extern void wm_hubs_update_class_w(struct snd_soc_component *component);

extern const struct snd_kcontrol_new wm_hubs_hpl_mux;
extern const struct snd_kcontrol_new wm_hubs_hpr_mux;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif
