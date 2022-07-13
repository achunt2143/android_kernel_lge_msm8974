<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Suspend-to-RAM support code for SH-Mobile ARM
 *
 *  Copyright (C) 2011 Magnus Damm
<<<<<<< HEAD
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/pm.h>
#include <linux/suspend.h>
#include <linux/module.h>
#include <linux/err.h>
<<<<<<< HEAD
#include <asm/io.h>
#include <asm/system_misc.h>

=======
#include <linux/cpu.h>

#include <asm/io.h>
#include <asm/system_misc.h>

#include "common.h"

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int shmobile_suspend_default_enter(suspend_state_t suspend_state)
{
	cpu_do_idle();
	return 0;
}

static int shmobile_suspend_begin(suspend_state_t state)
{
<<<<<<< HEAD
	disable_hlt();
=======
	cpu_idle_poll_ctrl(true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static void shmobile_suspend_end(void)
{
<<<<<<< HEAD
	enable_hlt();
=======
	cpu_idle_poll_ctrl(false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

struct platform_suspend_ops shmobile_suspend_ops = {
	.begin		= shmobile_suspend_begin,
	.end		= shmobile_suspend_end,
	.enter		= shmobile_suspend_default_enter,
	.valid		= suspend_valid_only_mem,
};

<<<<<<< HEAD
static int __init shmobile_suspend_init(void)
=======
int __init shmobile_suspend_init(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	suspend_set_ops(&shmobile_suspend_ops);
	return 0;
}
<<<<<<< HEAD
late_initcall(shmobile_suspend_init);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
