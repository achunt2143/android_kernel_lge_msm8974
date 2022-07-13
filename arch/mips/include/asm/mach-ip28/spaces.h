/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1994 - 1999, 2000, 03, 04 Ralf Baechle
 * Copyright (C) 2000, 2002  Maciej W. Rozycki
 * Copyright (C) 1990, 1999, 2000 Silicon Graphics, Inc.
<<<<<<< HEAD
 * 2004	pf
=======
 * 2004 pf
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#ifndef _ASM_MACH_IP28_SPACES_H
#define _ASM_MACH_IP28_SPACES_H

<<<<<<< HEAD
#define CAC_BASE		0xa800000000000000

#define HIGHMEM_START		(~0UL)

#define PHYS_OFFSET		_AC(0x20000000, UL)
=======
#define PHYS_OFFSET	_AC(0x20000000, UL)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <asm/mach-generic/spaces.h>

#endif /* _ASM_MACH_IP28_SPACES_H */
