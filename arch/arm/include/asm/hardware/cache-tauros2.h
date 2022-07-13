<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * arch/arm/include/asm/hardware/cache-tauros2.h
 *
 * Copyright (C) 2008 Marvell Semiconductor
<<<<<<< HEAD
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

extern void __init tauros2_init(void);
=======
 */

#define CACHE_TAUROS2_PREFETCH_ON	(1 << 0)
#define CACHE_TAUROS2_LINEFILL_BURST8	(1 << 1)

extern void __init tauros2_init(unsigned int features);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
