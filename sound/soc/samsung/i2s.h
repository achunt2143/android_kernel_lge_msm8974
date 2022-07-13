<<<<<<< HEAD
/* sound/soc/samsung/i2s.h
 *
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * ALSA SoC Audio Layer - Samsung I2S Controller driver
 *
 * Copyright (c) 2010 Samsung Electronics Co. Ltd.
 *	Jaswinder Singh <jassisinghbrar@gmail.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef __SND_SOC_SAMSUNG_I2S_H
#define __SND_SOC_SAMSUNG_I2S_H

<<<<<<< HEAD
/*
 * Maximum number of I2S blocks that any SoC can have.
 * The secondary interface of a CPU dai(if there exists any),
 * is indexed at [cpu-dai's ID + SAMSUNG_I2S_SECOFF]
 */
#define SAMSUNG_I2S_SECOFF	4

#define SAMSUNG_I2S_DIV_BCLK	1

#define SAMSUNG_I2S_RCLKSRC_0	0
#define SAMSUNG_I2S_RCLKSRC_1	1
#define SAMSUNG_I2S_CDCLK		2
=======
#define SAMSUNG_I2S_DAI        "samsung-i2s"
#define SAMSUNG_I2S_DAI_SEC    "samsung-i2s-sec"

#define SAMSUNG_I2S_DIV_BCLK		1

#define SAMSUNG_I2S_RCLKSRC_0		0
#define SAMSUNG_I2S_RCLKSRC_1		1
#define SAMSUNG_I2S_CDCLK		2
/* Operation clock for IIS logic */
#define SAMSUNG_I2S_OPCLK		3
#define  SAMSUNG_I2S_OPCLK_CDCLK_OUT	0	/* CODEC clock out */
#define  SAMSUNG_I2S_OPCLK_CDCLK_IN	1	/* CODEC clock in */
#define  SAMSUNG_I2S_OPCLK_BCLK_OUT	2	/* Bit clock out */
#define  SAMSUNG_I2S_OPCLK_PCLK		3	/* Audio bus clock */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* __SND_SOC_SAMSUNG_I2S_H */
