<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *	include/asm-mips/dec/kn230.h
 *
 *	DECsystem 5100 (MIPSmate or KN230) definitions.
 *
 *	Copyright (C) 2002, 2003  Maciej W. Rozycki
<<<<<<< HEAD
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#ifndef __ASM_MIPS_DEC_KN230_H
#define __ASM_MIPS_DEC_KN230_H

/*
 * CPU interrupt bits.
 */
#define KN230_CPU_INR_HALT	6	/* HALT button */
#define KN230_CPU_INR_BUS	5	/* memory, I/O bus read/write errors */
#define KN230_CPU_INR_RTC	4	/* DS1287 RTC */
#define KN230_CPU_INR_SII	3	/* SII (DC7061) SCSI */
#define KN230_CPU_INR_LANCE	3	/* LANCE (Am7990) Ethernet */
#define KN230_CPU_INR_DZ11	2	/* DZ11 (DC7085) serial */

#endif /* __ASM_MIPS_DEC_KN230_H */
