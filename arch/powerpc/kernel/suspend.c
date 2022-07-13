<<<<<<< HEAD
/*
 * Suspend support specific for power.
 *
 * Distribute under GPLv2
 *
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Suspend support specific for power.
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Copyright (c) 2002 Pavel Machek <pavel@ucw.cz>
 * Copyright (c) 2001 Patrick Mochel <mochel@osdl.org>
 */

#include <linux/mm.h>
<<<<<<< HEAD
#include <asm/page.h>

/* References to section boundaries */
extern const void __nosave_begin, __nosave_end;
=======
#include <linux/suspend.h>
#include <asm/page.h>
#include <asm/sections.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 *	pfn_is_nosave - check if given pfn is in the 'nosave' section
 */

int pfn_is_nosave(unsigned long pfn)
{
	unsigned long nosave_begin_pfn = __pa(&__nosave_begin) >> PAGE_SHIFT;
	unsigned long nosave_end_pfn = PAGE_ALIGN(__pa(&__nosave_end)) >> PAGE_SHIFT;
	return (pfn >= nosave_begin_pfn) && (pfn < nosave_end_pfn);
}
