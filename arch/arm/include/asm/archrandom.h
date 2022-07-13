<<<<<<< HEAD
/* Copyright (c) 2013, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef ARM_ASM_ARCHRANDOM_H
#define ARM_ASM_ARCHRANDOM_H

extern int arch_get_random_long(unsigned long *v);
extern int arch_get_random_int(unsigned int *v);

#endif
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_ARCHRANDOM_H
#define _ASM_ARCHRANDOM_H

static inline bool __init smccc_probe_trng(void)
{
	return false;
}

#include <asm-generic/archrandom.h>

#endif /* _ASM_ARCHRANDOM_H */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
