<<<<<<< HEAD
/*
 *  arch/arm/include/asm/bugs.h
 *
 *  Copyright (C) 1995-2003 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 *  Copyright (C) 1995-2003 Russell King
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#ifndef __ASM_BUGS_H
#define __ASM_BUGS_H

<<<<<<< HEAD
#ifdef CONFIG_MMU
extern void check_writebuffer_bugs(void);

#define check_bugs() check_writebuffer_bugs()
#else
#define check_bugs() do { } while (0)
=======
extern void check_writebuffer_bugs(void);

#ifdef CONFIG_MMU
extern void check_other_bugs(void);
#else
#define check_other_bugs() do { } while (0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

#endif
