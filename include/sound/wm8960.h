<<<<<<< HEAD
/*
 * wm8960.h  --  WM8960 Soc Audio driver platform data
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * wm8960.h  --  WM8960 Soc Audio driver platform data
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef _WM8960_PDATA_H
#define _WM8960_PDATA_H

#define WM8960_DRES_400R 0
#define WM8960_DRES_200R 1
#define WM8960_DRES_600R 2
#define WM8960_DRES_150R 3
#define WM8960_DRES_MAX  3

struct wm8960_data {
	bool capless;  /* Headphone outputs configured in capless mode */

<<<<<<< HEAD
	int dres;  /* Discharge resistance for headphone outputs */
=======
	bool shared_lrclk;  /* DAC and ADC LRCLKs are wired together */

	/*
	 * Setup for headphone detection
	 *
	 * hp_cfg[0]: HPSEL[1:0] of R48 (Additional Control 4)
	 * hp_cfg[1]: {HPSWEN:HPSWPOL} of R24 (Additional Control 2).
	 * hp_cfg[2]: {TOCLKSEL:TOEN} of R23 (Additional Control 1).
	 */
	u32 hp_cfg[3];

	/*
	 * Setup for gpio configuration
	 *
	 * gpio_cfg[0]: ALRCGPIO of R9 (Audio interface)
	 * gpio_cfg[1]: {GPIOPOL:GPIOSEL[2:0]} of R48 (Additional Control 4).
	 */
	u32 gpio_cfg[2];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

#endif
