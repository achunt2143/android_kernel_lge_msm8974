<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * header file for ADAV80X parts
 *
 * Copyright 2011 Analog Devices Inc.
<<<<<<< HEAD
 *
 * Licensed under the GPL-2 or later.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef _ADAV80X_H
#define _ADAV80X_H

<<<<<<< HEAD
=======
#include <linux/regmap.h>

struct device;

extern const struct regmap_config adav80x_regmap_config;
int adav80x_bus_probe(struct device *dev, struct regmap *regmap);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
enum adav80x_pll_src {
	ADAV80X_PLL_SRC_XIN,
	ADAV80X_PLL_SRC_XTAL,
	ADAV80X_PLL_SRC_MCLKI,
};

enum adav80x_pll {
	ADAV80X_PLL1 = 0,
	ADAV80X_PLL2 = 1,
};

enum adav80x_clk_src {
	ADAV80X_CLK_XIN = 0,
	ADAV80X_CLK_MCLKI = 1,
	ADAV80X_CLK_PLL1 = 2,
	ADAV80X_CLK_PLL2 = 3,
	ADAV80X_CLK_XTAL = 6,

	ADAV80X_CLK_SYSCLK1 = 6,
	ADAV80X_CLK_SYSCLK2 = 7,
	ADAV80X_CLK_SYSCLK3 = 8,
};

#endif
