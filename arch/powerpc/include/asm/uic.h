<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * IBM PPC4xx UIC external definitions and structure.
 *
 * Maintainer: David Gibson <dwg@au1.ibm.com>
 * Copyright 2007 IBM Corporation.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#ifndef _ASM_POWERPC_UIC_H
#define _ASM_POWERPC_UIC_H

#ifdef __KERNEL__

extern void __init uic_init_tree(void);
extern unsigned int uic_get_irq(void);

#endif /* __KERNEL__ */
#endif /* _ASM_POWERPC_UIC_H */
