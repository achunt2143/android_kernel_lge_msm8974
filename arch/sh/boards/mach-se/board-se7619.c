<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * arch/sh/boards/se/7619/setup.c
 *
 * Copyright (C) 2006 Yoshinori Sato
 *
 * Hitachi SH7619 SolutionEngine Support.
 */

#include <linux/init.h>
#include <linux/platform_device.h>
#include <asm/io.h>
#include <asm/machvec.h>

static int se7619_mode_pins(void)
{
	return MODE_PIN2 | MODE_PIN0;
}

/*
 * The Machine Vector
 */

static struct sh_machine_vector mv_se __initmv = {
	.mv_name		= "SolutionEngine",
<<<<<<< HEAD
	.mv_nr_irqs		= 108,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.mv_mode_pins		= se7619_mode_pins,
};
