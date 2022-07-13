/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1994 - 1999, 2000, 03, 04 Ralf Baechle
 * Copyright (C) 2000, 2002  Maciej W. Rozycki
 * Copyright (C) 1990, 1999, 2000 Silicon Graphics, Inc.
 */
#ifndef _ASM_MACH_GENERIC_SPACES_H
#define _ASM_MACH_GENERIC_SPACES_H

#include <linux/const.h>

<<<<<<< HEAD
/*
 * This gives the physical RAM offset.
 */
#ifndef PHYS_OFFSET
#define PHYS_OFFSET		_AC(0, UL)
#endif

#ifdef CONFIG_32BIT

#define CAC_BASE		_AC(0x80000000, UL)
#define IO_BASE			_AC(0xa0000000, UL)
#define UNCAC_BASE		_AC(0xa0000000, UL)
=======
#include <asm/mipsregs.h>

#ifndef IO_SPACE_LIMIT
#define IO_SPACE_LIMIT 0xffff
#endif

/*
 * This gives the physical RAM offset.
 */
#ifndef __ASSEMBLY__
# if defined(CONFIG_MIPS_AUTO_PFN_OFFSET)
#  define PHYS_OFFSET		((unsigned long)PFN_PHYS(ARCH_PFN_OFFSET))
# elif !defined(PHYS_OFFSET)
#  define PHYS_OFFSET		_AC(0, UL)
# endif
#endif /* __ASSEMBLY__ */

#ifdef CONFIG_32BIT
#define CAC_BASE		_AC(0x80000000, UL)
#ifndef IO_BASE
#define IO_BASE			_AC(0xa0000000, UL)
#endif
#ifndef UNCAC_BASE
#define UNCAC_BASE		_AC(0xa0000000, UL)
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifndef MAP_BASE
#define MAP_BASE		_AC(0xc0000000, UL)
#endif

/*
 * Memory above this physical address will be considered highmem.
 */
#ifndef HIGHMEM_START
#define HIGHMEM_START		_AC(0x20000000, UL)
#endif

<<<<<<< HEAD
=======
#define CKSEG0ADDR_OR_64BIT(x)	CKSEG0ADDR(x)
#define CKSEG1ADDR_OR_64BIT(x)	CKSEG1ADDR(x)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* CONFIG_32BIT */

#ifdef CONFIG_64BIT

#ifndef CAC_BASE
<<<<<<< HEAD
#ifdef CONFIG_DMA_NONCOHERENT
#define CAC_BASE		_AC(0x9800000000000000, UL)
#else
#define CAC_BASE		_AC(0xa800000000000000, UL)
#endif
=======
#define CAC_BASE	PHYS_TO_XKPHYS(read_c0_config() & CONF_CM_CMASK, 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

#ifndef IO_BASE
#define IO_BASE			_AC(0x9000000000000000, UL)
#endif

#ifndef UNCAC_BASE
#define UNCAC_BASE		_AC(0x9000000000000000, UL)
#endif

#ifndef MAP_BASE
#define MAP_BASE		_AC(0xc000000000000000, UL)
#endif

/*
 * Memory above this physical address will be considered highmem.
 * Fixme: 59 bits is a fictive number and makes assumptions about processors
 * in the distant future.  Nobody will care for a few years :-)
 */
#ifndef HIGHMEM_START
#define HIGHMEM_START		(_AC(1, UL) << _AC(59, UL))
#endif

<<<<<<< HEAD
#define TO_PHYS(x)		(             ((x) & TO_PHYS_MASK))
#define TO_CAC(x)		(CAC_BASE   | ((x) & TO_PHYS_MASK))
#define TO_UNCAC(x)		(UNCAC_BASE | ((x) & TO_PHYS_MASK))

=======
#define TO_PHYS(x)		(	      ((x) & TO_PHYS_MASK))
#define TO_CAC(x)		(CAC_BASE   | ((x) & TO_PHYS_MASK))
#define TO_UNCAC(x)		(UNCAC_BASE | ((x) & TO_PHYS_MASK))

#define CKSEG0ADDR_OR_64BIT(x)	TO_CAC(x)
#define CKSEG1ADDR_OR_64BIT(x)	TO_UNCAC(x)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* CONFIG_64BIT */

/*
 * This handles the memory map.
 */
#ifndef PAGE_OFFSET
#define PAGE_OFFSET		(CAC_BASE + PHYS_OFFSET)
#endif

#ifndef FIXADDR_TOP
#define FIXADDR_TOP		((unsigned long)(long)(int)0xfffe0000)
#endif

#endif /* __ASM_MACH_GENERIC_SPACES_H */
