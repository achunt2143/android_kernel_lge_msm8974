<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * arch/powerpc/platforms/embedded6xx/hlwd-pic.h
 *
 * Nintendo Wii "Hollywood" interrupt controller support.
 * Copyright (C) 2009 The GameCube Linux Team
 * Copyright (C) 2009 Albert Herranz
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef __HLWD_PIC_H
#define __HLWD_PIC_H

extern unsigned int hlwd_pic_get_irq(void);
<<<<<<< HEAD
extern void hlwd_pic_probe(void);
=======
void __init hlwd_pic_probe(void);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern void hlwd_quiesce(void);

#endif
