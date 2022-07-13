<<<<<<< HEAD
/*
 *  (C) 2010,2011       Thomas Renninger <trenn@suse.de>, Novell Inc.
 *
 *  Licensed under the terms of the GNU GPL License version 2.
 *
 *  Based on the idea from Michael Matz <matz@suse.de>
 *
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 *  (C) 2010,2011       Thomas Renninger <trenn@suse.de>, Novell Inc.
 *
 *  Based on the idea from Michael Matz <matz@suse.de>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef _CPUIDLE_IDLE_MONITORS_H_
#define _CPUIDLE_IDLE_MONITORS_H_

#define DEF(x) extern struct cpuidle_monitor x ##_monitor;
#include "idle_monitors.def"
#undef DEF
extern struct cpuidle_monitor *all_monitors[];

#endif /* _CPUIDLE_IDLE_MONITORS_H_ */
