<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * arch/powerpc/boot/gamecube.c
 *
 * Nintendo GameCube bootwrapper support
 * Copyright (C) 2004-2009 The GameCube Linux Team
 * Copyright (C) 2008,2009 Albert Herranz
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

#include <stddef.h>
#include "stdio.h"
#include "types.h"
#include "io.h"
#include "ops.h"

#include "ugecon.h"

BSS_STACK(8192);

void platform_init(unsigned long r3, unsigned long r4, unsigned long r5)
{
	u32 heapsize = 16*1024*1024 - (u32)_end;

	simple_alloc_init(_end, heapsize, 32, 64);
	fdt_init(_dtb_start);

	if (ug_probe())
		console_ops.write = ug_console_write;
}

