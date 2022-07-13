<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * PowerPC 64-bit swsusp implementation
 *
 * Copyright 2006 Johannes Berg <johannes@sipsolutions.net>
<<<<<<< HEAD
 *
 * GPLv2
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <asm/iommu.h>
#include <linux/irq.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
<<<<<<< HEAD
=======
#include <linux/nmi.h>

void do_after_copyback(void);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

void do_after_copyback(void)
{
	iommu_restore();
	touch_softlockup_watchdog();
	mb();
}
<<<<<<< HEAD

void _iommu_save(void)
{
	iommu_save();
}
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
