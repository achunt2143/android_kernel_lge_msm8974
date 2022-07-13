<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  arch/arm/mach-footbridge/include/mach/memory.h
 *
 *  Copyright (C) 1996-1999 Russell King.
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *  Changelog:
 *   20-Oct-1996 RMK	Created
 *   31-Dec-1997 RMK	Fixed definitions to reduce warnings.
 *   17-May-1998 DAG	Added __virt_to_bus and __bus_to_virt functions.
 *   21-Nov-1998 RMK	Changed __virt_to_bus and __bus_to_virt to macros.
 *   21-Mar-1999 RMK	Added PAGE_OFFSET for co285 architecture.
 *			Renamed to memory.h
 *			Moved PAGE_OFFSET and TASK_SIZE here
 */
#ifndef __ASM_ARCH_MEMORY_H
#define __ASM_ARCH_MEMORY_H

<<<<<<< HEAD

#if defined(CONFIG_FOOTBRIDGE_ADDIN)
/*
 * If we may be using add-in footbridge mode, then we must
 * use the out-of-line translation that makes use of the
 * PCI BAR
 */
#ifndef __ASSEMBLY__
extern unsigned long __virt_to_bus(unsigned long);
extern unsigned long __bus_to_virt(unsigned long);
extern unsigned long __pfn_to_bus(unsigned long);
extern unsigned long __bus_to_pfn(unsigned long);
#endif
#define __virt_to_bus	__virt_to_bus
#define __bus_to_virt	__bus_to_virt

#elif defined(CONFIG_FOOTBRIDGE_HOST)

/*
 * The footbridge is programmed to expose the system RAM at 0xe0000000.
 * The requirement is that the RAM isn't placed at bus address 0, which
 * would clash with VGA cards.
 */
#define BUS_OFFSET		0xe0000000
#define __virt_to_bus(x)	((x) + (BUS_OFFSET - PAGE_OFFSET))
#define __bus_to_virt(x)	((x) - (BUS_OFFSET - PAGE_OFFSET))
#define __pfn_to_bus(x)		(__pfn_to_phys(x) + (BUS_OFFSET - PHYS_OFFSET))
#define __bus_to_pfn(x)		__phys_to_pfn((x) - (BUS_OFFSET - PHYS_OFFSET))

#else

#error "Undefined footbridge mode"

#endif

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Cache flushing area.
 */
#define FLUSH_BASE		0xf9000000

<<<<<<< HEAD
/*
 * Physical DRAM offset.
 */
#define PLAT_PHYS_OFFSET		UL(0x00000000)

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define FLUSH_BASE_PHYS		0x50000000

#endif
