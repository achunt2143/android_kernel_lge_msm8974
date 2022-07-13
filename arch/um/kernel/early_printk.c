<<<<<<< HEAD
/*
 * Copyright (C) 2011 Richard Weinberger <richrd@nod.at>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2011 Richard Weinberger <richrd@nod.at>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/kernel.h>
#include <linux/console.h>
#include <linux/init.h>
<<<<<<< HEAD
#include "os.h"
=======
#include <os.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static void early_console_write(struct console *con, const char *s, unsigned int n)
{
	um_early_printk(s, n);
}

<<<<<<< HEAD
static struct console early_console = {
=======
static struct console early_console_dev = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.name = "earlycon",
	.write = early_console_write,
	.flags = CON_BOOT,
	.index = -1,
};

static int __init setup_early_printk(char *buf)
{
<<<<<<< HEAD
	register_console(&early_console);

=======
	if (!early_console) {
		early_console = &early_console_dev;
		register_console(&early_console_dev);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

early_param("earlyprintk", setup_early_printk);
