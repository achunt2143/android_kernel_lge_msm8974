<<<<<<< HEAD
/*
 * Copyright (C) 2000, 2001 Broadcom Corporation
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2000, 2001 Broadcom Corporation
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#ifndef __ASM_SIBYTE_SENTOSA_H
#define __ASM_SIBYTE_SENTOSA_H

#include <asm/sibyte/sb1250.h>
#include <asm/sibyte/sb1250_int.h>

#ifdef CONFIG_SIBYTE_SENTOSA
#define SIBYTE_BOARD_NAME "BCM91250E (Sentosa)"
#endif
#ifdef CONFIG_SIBYTE_RHONE
#define SIBYTE_BOARD_NAME "BCM91125E (Rhone)"
#endif

/* Generic bus chip selects */
#ifdef CONFIG_SIBYTE_RHONE
<<<<<<< HEAD
#define LEDS_CS         6
#define LEDS_PHYS       0x1d0a0000
#endif

/* GPIOs */
#define K_GPIO_DBG_LED  0
=======
#define LEDS_CS		6
#define LEDS_PHYS	0x1d0a0000
#endif

/* GPIOs */
#define K_GPIO_DBG_LED	0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* __ASM_SIBYTE_SENTOSA_H */
